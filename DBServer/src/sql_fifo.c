#include "sql_fifo.h"
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/network/netio.h>
#include <utilitieslib/utils/error.h>
#include "dbserver/servercfg.h"
#include <utilitieslib/network/netio_core.h>
#include "container.h"
#include "comm_backend.h"
#include "container_merge.h"
#include "dbrelay.h"
#include "dbperf.h"
#include "backup.h"
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/utils/utils.h>
#include "clientcomm.h"
#include "sql/sqltask.h"
#include <utilitieslib/components/earray.h>
#include <utilitieslib/utils/log.h>
#include <utilitieslib/components/StashTable.h>

//#define DEBUG_FIFO // Enable to force a flush after each command to make debugging easier

#ifdef DEBUG_FIFO
    #define NUM_SQL_WORKERS 1
#else
    #define NUM_SQL_WORKERS (SQLCONN_MAX - 1)
#endif
#define SQL_QUEUE_SIZES 16384 // must be power of 2
#define SQL_QUEUE_ENTRIES (SQL_QUEUE_SIZES * 4)

STATIC_ASSERT((NUM_SQL_WORKERS & (NUM_SQL_WORKERS - 1)) == 0); // check for power of 2

#define MAX_FIELD_PTRS 1024
#define MAX_CONTAINER_DEPENDENCIES 4

typedef enum
{
    SQLCMD_INVALID=0,
    SQLCMD_BARRIER,
    SQLCMD_CONTAINERUPDATE,
    SQLCMD_READCONTAINER,
    SQLCMD_READCOLUMNS,
    SQLCMD_DELETEROW,
    SQLCMD_EXEC
} SqlCmd;

typedef __declspec(align(EXPECTED_WORST_CACHE_LINE_SIZE)) struct
{
    SqlCmd cmd;
    union
    {
        struct
        {
            // input
            int id;
            ContainerTemplate *tplt;
            LineList diff_list;
        } container;

        struct
        {
            // input
            int list_id;
            int id;
            NetLink *link;
            U32 link_UID;
            GameClientLink *game;
            U32 game_linkId;
            void* input_data;

            // internal
            struct SqlDelayedContainerCallback ** notify_list;

            // output
            char *data;
        } readcontainer;

        struct
        {
            // input
            TableInfo *table;
            char *limit;
            char *col_names;
            char *restrict;
            ReadColumnCallback callback;
            void *callbackData;
            ColumnInfo **field_ptrs;
            Packet *packet;
            NetLink *link;
            U32 link_UID;

            // output
            U8 *cols;
            int col_count;
            int debugDelayMS;
        } readcolumns;

        struct {
            // input
            int id;
            char * table;
        } deleterow;

        struct {
            // input
            int str_len;
            int is_utf8;
            char * large_buf;
            char short_buf[EXPECTED_WORST_CACHE_LINE_SIZE - sizeof(char*) - 2*sizeof(int) - sizeof(SqlCmd)];
        } exec;
    };
} SqlQueueEntry;

STATIC_ASSERT(sizeof(SqlQueueEntry) <= EXPECTED_WORST_CACHE_LINE_SIZE*2);

typedef struct SqlDelayedContainerCallback {
    SqlQueueEntry entry;

    unsigned wait_count;
    unsigned waiting_for;
    int wait_list[MAX_CONTAINER_DEPENDENCIES];
    int wait_id[MAX_CONTAINER_DEPENDENCIES];
} SqlDelayedContainerCallback;

typedef __declspec(align(EXPECTED_WORST_CACHE_LINE_SIZE)) struct sqlFifoWorker
{
    SqlConn conn;
    bool using_transactions;
    bool transaction_in_progress;
} sqlFifoWorker;

struct sqlFifoGlobals
{
    int in_flight;
    int max_in_flight;
    bool running;
    bool use_transactions;
    sqlFifoWorker workers[NUM_SQL_WORKERS];
} sqlfifo;

MP_DEFINE(SqlQueueEntry);
MP_DEFINE(SqlDelayedContainerCallback);

static StashTable s_reads_pending[MAX_CONTAINER_TYPES];
static StashTable s_writes_pending[MAX_CONTAINER_TYPES];

extern void handlePlayerContainerLoaded(NetLink *link, int container_id, GameClientLink *client);
extern void handlePlayerContainerUpdated(int container_id);
static void * runQueueEntry(void * data, void * user);
static void processQueueEntry(SqlQueueEntry * entry);

static void s_fifo_flush(void)
{
    printf_stderr("Flushing SQL fifo\n");
    sqlFifoShutdown();
    sqlConnShutdown();
}

void sqlFifoInit(void)
{
    unsigned i;

    assert(!sqlfifo.running);
    assert(!atexit(s_fifo_flush));

    sqlfifo.running = true;
    sqlfifo.use_transactions = true;

    sql_task_init(NUM_SQL_WORKERS, SQL_QUEUE_SIZES);
    for (i=0; i<NUM_SQL_WORKERS; i++) {
        sqlFifoWorker * worker = &sqlfifo.workers[i];
        worker->conn = (SqlConn)(i+1);
        sql_task_set_callback(i, runQueueEntry, worker);
    }
    sql_task_freeze();

    // Enough room for input + in progress + output
    MP_CREATE(SqlQueueEntry, SQL_QUEUE_ENTRIES);
    MP_CREATE(SqlDelayedContainerCallback, SQL_QUEUE_ENTRIES);

    for (i=0; i<MAX_CONTAINER_TYPES; i++) {
        s_reads_pending[i] = stashTableCreateInt(0);
        s_writes_pending[i] = stashTableCreateInt(0);
    }

    // Verify that the FIFO threads are working correctly
    sqlFifoBarrier();
    sqlFifoFinish();
}

void sqlFifoShutdown(void)
{
    int i;
    
    if (!sqlfifo.running)
        return;

    // Insert a barrier to make sure that all threads are forced to complete
    // any pending transactions before we shutdown.
    sqlFifoBarrier();
    sqlFifoFinish();

    MP_DESTROY(SqlQueueEntry);
    MP_DESTROY(SqlDelayedContainerCallback);

    for (i=0; i<MAX_CONTAINER_TYPES; i++) {
        stashTableDestroy(s_reads_pending[i]);
        stashTableDestroy(s_writes_pending[i]);
    }

    sql_task_thaw();
    sql_task_shutdown();
    sqlfifo.running = false;
}

void sqlFifoEnableTransacted(int enable)
{
    sqlfifo.use_transactions = enable;
}

int sqlFifoWorstQueueDepth()
{
    int depth = sqlfifo.max_in_flight;
    sqlfifo.max_in_flight = sqlfifo.in_flight;
    return depth;
}

void sqlFifoDisableNMMonitorIfMemoryLow(void)
{
    if (sqlfifo.in_flight >= SQL_QUEUE_ENTRIES)
    {
        // We want to avoid consuming too much memory if possible
        NMMonitorStopReceiving();
    }
}

static SqlQueueEntry * getQueueEntry(void)
{
    SqlQueueEntry * entry = MP_ALLOC(SqlQueueEntry);
    assert(entry);
    return entry;
}

static void pushQueueEntry(unsigned queue, SqlQueueEntry *entry) {
    sql_task_push(queue, entry);

    if (!sqlfifo.in_flight)
        sql_task_thaw();

    sqlfifo.in_flight++;
    if (sqlfifo.in_flight > sqlfifo.max_in_flight)
        sqlfifo.max_in_flight = sqlfifo.in_flight;

#ifdef DEBUG_FIFO
    assert(sqlfifo.in_flight == 1);
    sqlFifoFinish();
    assert(sqlfifo.in_flight == 0);
#endif
}

static unsigned pickQueue(unsigned container_id) {
    if (!container_id) {
        static unsigned next_queue = 0;
        // Assign a sequential id when none is supplied
        container_id = ++next_queue;
    }
    return container_id & (NUM_SQL_WORKERS - 1);
}

void sqlFifoFinish(void)
{
    // wait for the queue to empty
    while (sqlfifo.in_flight)
        processQueueEntry(sql_task_pop());
}

void sqlFifoTick(void)
{
    SqlQueueEntry * entry;

    while((entry = sql_task_trypop()))
        processQueueEntry(entry);
}

/**
* Insert an equivalent of a memory barrier in the SQL queue to force
* serialized access without having to stall the main thread.
*/
void sqlFifoBarrier(void)
{
    unsigned i;

    // insert a barrier in the SQL queue for each worker
    for (i=0; i<NUM_SQL_WORKERS; i++) {
        SqlQueueEntry * entry = getQueueEntry();
        entry->cmd = SQLCMD_BARRIER;
        pushQueueEntry(i, entry);
    }
}

bool sqlIsAsyncReadPending(int list_id, int container_id)
{
    return stashIntFindElement(s_reads_pending[list_id], container_id, NULL);
}

void sqlFifoTickWhileReadPending(int list_id, int container_id)
{
    if (!sqlIsAsyncReadPending(list_id, container_id))
        return;

    assert(sqlfifo.in_flight);

    sqlFifoTick();
    while (sqlIsAsyncReadPending(list_id, container_id)) {
        sqlFifoTick();
        Sleep(1);
    }
}

bool sqlIsAsyncWritePending(int list_id, int container_id)
{
    return stashIntFindElement(s_writes_pending[list_id], container_id, NULL);
}

void sqlFifoTickWhileWritePending(int list_id, int container_id)
{
    if (!sqlIsAsyncWritePending(list_id, container_id))
        return;

    assert(sqlfifo.in_flight);

    sqlFifoTick();
    while (sqlIsAsyncWritePending(list_id, container_id)) {
        sqlFifoTick();
        Sleep(1);
    }
}

void sqlContainerUpdateAsync(ContainerTemplate *tplt, int container_id, LineList *diff_list)
{
    SqlQueueEntry *entry;

    if (!tplt || tplt->dont_write_to_sql)
        return;

    entry = getQueueEntry();
    entry->cmd = SQLCMD_CONTAINERUPDATE;
    entry->container.tplt = tplt;
    entry->container.id = container_id;
    copyLineList(diff_list, &entry->container.diff_list); TODO(); // optimize me
    pushQueueEntry(pickQueue(container_id), entry);

    if (!stashIntAddInt(s_writes_pending[tplt->dblist_id], container_id, 1, false)) {
        // Increment existing value
        StashElement element;
        assert(stashIntFindElement(s_writes_pending[tplt->dblist_id], container_id, &element));
        stashElementSetInt(element, stashElementGetInt(element)+1);
    }
}

void sqlContainerUpdateFromScratchAsync(ContainerTemplate *tplt, int container_id, LineList *line_list, bool single_table)
{
    static LineList full_list, dump_list, null_list;

    copyLineList(line_list, &full_list);
    mergeLineLists(tplt, &null_list, &full_list, &dump_list);

    if (single_table && full_list.cmd_count == 2)
    {
        full_list.row_cmds[0] = full_list.row_cmds[1];
        full_list.cmd_count = 1;
    }

    sqlContainerUpdateAsync(tplt, container_id, &full_list);
}

char *sqlReadColumnsSlow(TableInfo *table, char *limit, char *col_names, char *where, int *count_ptr, ColumnInfo **field_ptrs)
{
    return sqlReadColumnsInternal(table, limit, col_names, where, count_ptr, field_ptrs, SQLCONN_FOREGROUND, 0);
}

/**
* Issue an asynchronous request to read column data from a given 
* table and call the callback function with the results. 
*
* @note The @ref container_id parameter is only used to make 
*       sure that the request runs on the correct SQL connection
*       to prevent the read completing before any pending
*       writes.
*  
* @note If no @ref container_id is available or if the request 
*       scans multiple rows, a sqlFifoBarrier() call should be
*       made before this function to force all previous writes
*       to complete first.
*/
void sqlReadColumnsAsyncWithDebugDelay(TableInfo *table, char *limit, char *col_names, char *restrict, ReadColumnCallback callback, void * callbackData, Packet *pak, int container_id, int debugDelayMS)
{
    SqlQueueEntry *entry = getQueueEntry();
    entry->cmd = SQLCMD_READCOLUMNS;
    entry->readcolumns.table = table;
    entry->readcolumns.limit = strdup(limit);
    entry->readcolumns.col_names = strdup(col_names);
    entry->readcolumns.restrict = strdup(restrict);
    entry->readcolumns.callback = callback;
    entry->readcolumns.callbackData = callbackData;
    entry->readcolumns.field_ptrs = calloc(MAX_FIELD_PTRS, sizeof(ColumnInfo *));
    entry->readcolumns.packet = pak;
    entry->readcolumns.link = pak ? pak->link : NULL;
    entry->readcolumns.link_UID = entry->readcolumns.link ? entry->readcolumns.link->UID : 0;
    entry->readcolumns.debugDelayMS = debugDelayMS;
    pushQueueEntry(pickQueue(container_id), entry);
}

void sqlReadColumnsAsync(TableInfo *table, char *limit, char *col_names, char *restrict, ReadColumnCallback callback, void * callbackData, Packet *pak, int container_id)
{
    sqlReadColumnsAsyncWithDebugDelay(table, limit, col_names, restrict, callback, callbackData, pak, container_id, 0);
}

void sqlDeleteRowAsync(char *table, int container_id)
{
    SqlQueueEntry * entry = getQueueEntry();
    entry->cmd = SQLCMD_DELETEROW;
    entry->deleterow.table = strdup(table);
    entry->deleterow.id = container_id;
    pushQueueEntry(pickQueue(container_id), entry);
}

/**
* Execute a SQL statement asynchronously using a random SQL 
* connection from the pool of SQL connections. 
*/
void sqlExecAsync(char *str, int str_len)
{
    sqlExecAsyncEx(str, str_len, 0, true);
}

/**
* Execute a SQL statement asynchronously.
* 
* @param container_id Specify a nonzero value to choose a 
*                     specific SQL connection for the statement
*                     to be executed on.
*/
void sqlExecAsyncEx(char *str, int str_len, int container_id, bool utf8)
{
    SqlQueueEntry * entry;

    if (str_len == SQL_NTS)
    {
        str_len = (INT)strlen(str);
    }

    entry = getQueueEntry();
    entry->cmd = SQLCMD_EXEC;
    entry->exec.str_len = str_len;
    entry->exec.is_utf8 = utf8;
    if (str_len < ARRAY_SIZE(entry->exec.short_buf))
    {
        strcpy(entry->exec.short_buf, str);
    }
    else
    {
        entry->exec.large_buf = malloc(str_len+1);
        strcpy(entry->exec.large_buf, str);
    }
    pushQueueEntry(pickQueue(container_id), entry);
}

void sqlReadContainerAsync(int list_id, int container_id, NetLink *link, GameClientLink *game, void *input_data)
{
    SqlQueueEntry * entry = getQueueEntry();
    entry->cmd = SQLCMD_READCONTAINER;
    entry->readcontainer.list_id = list_id;
    entry->readcontainer.id = container_id;
    entry->readcontainer.link = link;
    entry->readcontainer.link_UID = link ? link->UID : 0;
    entry->readcontainer.game = game;
    entry->readcontainer.game_linkId = game ? game->linkId : 0;
    entry->readcontainer.input_data = input_data;
    pushQueueEntry(pickQueue(container_id), entry);

    assert(stashIntAddPointer(s_reads_pending[list_id], container_id, entry, false) == true);
}

static char * sqlReadContainerTask(int list_id, int container_id, SqlConn conn)
{
    return sqlContainerRead(dbListPtr(list_id)->tplt, container_id, conn);
}

static int linkValid(NetLink *link, U32 link_UID)
{
    return (link && !mpReclaimed(link) && (link->UID == link_UID));
}

static int gameClientValid(GameClientLink *game, int linkId)
{
    return game && !mpReclaimed(game) && (game->linkId == linkId);
}

static void sqlReadContainerNotify(SqlDelayedContainerCallback * notify, int list_id, int container_id)
{
    unsigned i;
    unsigned prev_waiting_for = notify->waiting_for;

    // Clear the entry for this object in the list
    for (i=0; i<notify->wait_count; i++) {
        if (notify->wait_list[i] == list_id && notify->wait_id[i] == container_id) {
            notify->wait_id[i] = 0;
            notify->waiting_for--;
            break;
        }
    }
    assert(prev_waiting_for && notify->waiting_for < prev_waiting_for && notify->waiting_for >= 0);
    
    // If this is the last entry in the wait list, handle the delayed load
    if (notify->waiting_for == 0) {
        static void sqlReadContainerLoadComplete(SqlQueueEntry * entry);
        sqlReadContainerLoadComplete(&notify->entry);
        MP_FREE(SqlDelayedContainerCallback, notify);
    }
}

static void sqlReadContainerLoadComplete(SqlQueueEntry * entry)
{
    DbContainer *container = NULL;
    SqlQueueEntry * found_entry;

    assert(stashIntRemovePointer(s_reads_pending[entry->readcontainer.list_id], entry->readcontainer.id, &found_entry) == true);
    assert(found_entry == entry);

    if (entry->readcontainer.data) {
        char * cache_data_array[MAX_CONTAINER_TYPES];

        TODO(); // Do away with this array if it is too slow since it is no longer required
        ZeroArray(cache_data_array);
        cache_data_array[entry->readcontainer.list_id] = entry->readcontainer.data;

        container = containerLoadCached(dbListPtr(entry->readcontainer.list_id), entry->readcontainer.id, cache_data_array);

        // Data will not be freed if we issued multiple reads of the same container
        if (cache_data_array[entry->readcontainer.list_id])
            free(cache_data_array[entry->readcontainer.list_id]);
    }

    if (entry->readcontainer.notify_list) {
        int i;

        for (i=0; i<eaSize(&entry->readcontainer.notify_list); i++)
            sqlReadContainerNotify(entry->readcontainer.notify_list[i], entry->readcontainer.list_id, entry->readcontainer.id);

        eaDestroy(&entry->readcontainer.notify_list);
    }

    switch (entry->readcontainer.list_id) {
        GameClientLink *client;

        case CONTAINER_ENTS:
            if (!container)
                dbRelayQueueRemove(entry->readcontainer.id);
            if (linkValid(entry->readcontainer.link, entry->readcontainer.link_UID))
            {
                if(gameClientValid(entry->readcontainer.game, entry->readcontainer.game_linkId))
                    client = entry->readcontainer.game;
                else
                    client = clientcomm_getGameClient(entry->readcontainer.link, NULL);
                devassert(client);
                handlePlayerContainerLoaded(entry->readcontainer.link, entry->readcontainer.id, client);
            }
            break;

        case CONTAINER_SHARDACCOUNT:
            if (linkValid(entry->readcontainer.link, entry->readcontainer.link_UID))
            {
                if(gameClientValid(entry->readcontainer.game, entry->readcontainer.game_linkId))
                    client = entry->readcontainer.game;
                else
                    client = clientcomm_getGameClient(entry->readcontainer.link, NULL);
                if (client) // client might leave queue before this code gets called
                {
                    handleShardAccountContainerLoaded(client, entry->readcontainer.input_data, container);
                }
            }
            break;
    }
}

static void sqlReadContainerLoadDependencies(SqlQueueEntry * entry)
{    
    unsigned i;
    SpecialColumn * cmds = dbListPtr(entry->readcontainer.list_id)->special_columns;
    struct SqlDelayedContainerCallback * delayed = NULL;

    if (cmds) {
        LineList list = {0,};
        memToLineList(entry->readcontainer.data, &list); TODO(); // Avoid copying the line list arrays around so much

        for(i=0; cmds[i].name; i++)
        {
            if (cmds[i].type == CFTYPE_INT && cmds[i].cmd == CMD_MEMBER)
            {
                char buf[128];
                if (findFieldTplt(dbListPtr(entry->readcontainer.list_id)->tplt, &list, cmds[i].name, buf))
                {
                    int member_list_id = cmds[i].member_of;
                    int    member_id = atoi(buf);
                    DbList * group_list = dbListPtr(member_list_id);

                    if (!member_id)
                        continue;

                    // Check to see if the container this object is a member of is loaded
                    if (!containerPtr(group_list, member_id))
                    {
                        SqlQueueEntry * in_progress_entry;                        

                        // Start a read for this object if one is not already in progress
                        if (!sqlIsAsyncReadPending(member_list_id, member_id))
                            sqlReadContainerAsync(member_list_id, member_id, NULL, NULL, NULL);

                        // Find the in flight object
                        assert(stashIntFindPointer(s_reads_pending[member_list_id], member_id, &in_progress_entry) == true);

                        // store off data for when the dependent read comes back
                        if (!delayed) {
                            delayed = MP_ALLOC(SqlDelayedContainerCallback);
                            memcpy(&delayed->entry, entry, sizeof(SqlQueueEntry));
                            assert(stashIntAddPointer(s_reads_pending[entry->readcontainer.list_id], entry->readcontainer.id, &delayed->entry, true) == true);
                        }

                        delayed->wait_list[delayed->wait_count] = member_list_id;
                        delayed->wait_id[delayed->wait_count++] = member_id;
                        delayed->waiting_for = delayed->wait_count;

                        // Add this object to the parent packet's notification list
                        if (!in_progress_entry->readcontainer.notify_list)
                            eaCreate(&in_progress_entry->readcontainer.notify_list);

                        assert(eaPush(&in_progress_entry->readcontainer.notify_list, delayed) >= 0);
                    }
                }
            }
        }

        freeLineList(&list);
    }

    // No dependent loads needed, complete now
    if (!delayed)
        sqlReadContainerLoadComplete(entry);
}

static void sqlUpdateContainerComplete(ContainerTemplate *tplt, int container_id)
{
    switch(tplt->dblist_id) {
        case CONTAINER_ENTS:
            handlePlayerContainerUpdated(container_id);
            break;
    }
}

static void workerStartUsingManualTransactions(sqlFifoWorker * worker)
{
    // Manually end SQL transactions to hint to the SQL server that it does not
    // need to continously update the table indexes until the entire
    // transaction has completed.
    if(!sqlConnEnableAutoTransactions(!sqlfifo.use_transactions, worker->conn))
    {
        sqlfifo.use_transactions = 0;
    }

    if(sqlfifo.use_transactions && !sqlConnEnableAsynchDBC(sqlfifo.use_transactions, worker->conn))
    {
        static bool once = true;
        if (once)
        {
            once = false;
            printf("No support for asynchronous SQL transactions, using synchronous instead\n");
        }
    }

    worker->using_transactions = sqlfifo.use_transactions;
}

static void * runQueueEntry(void * data, void * user)
{
    SqlQueueEntry * entry = (SqlQueueEntry*)data;
    sqlFifoWorker * worker = (sqlFifoWorker*)user;

    if(worker->using_transactions != sqlfifo.use_transactions)
    {
        workerStartUsingManualTransactions(worker);
    }

    if(worker->transaction_in_progress)
    {
        // Synchronous wait for end of transaction
        sqlConnEndTransaction(true, true, worker->conn);
        worker->transaction_in_progress = 0;
    }

    switch(entry->cmd)
    {
        case SQLCMD_BARRIER:
        {
            sql_task_worker_barrier();
            break;
        }
        case SQLCMD_CONTAINERUPDATE:
        {
            sqlContainerUpdateInternal(entry->container.tplt, entry->container.id, &entry->container.diff_list, worker->conn);
            break;
        }
        case SQLCMD_READCONTAINER:
        {
            entry->readcontainer.data = sqlReadContainerTask(entry->readcontainer.list_id, entry->readcontainer.id, worker->conn);
            break;
        }
        case SQLCMD_READCOLUMNS:
        {            
            entry->readcolumns.cols = sqlReadColumnsInternal(entry->readcolumns.table, entry->readcolumns.limit, entry->readcolumns.col_names, entry->readcolumns.restrict,
                &entry->readcolumns.col_count, entry->readcolumns.field_ptrs, worker->conn, entry->readcolumns.debugDelayMS);
            break;
        }
        case SQLCMD_DELETEROW:
        {
            sqlDeleteRowInternal(entry->deleterow.table, entry->deleterow.id, worker->conn);
            break;
        }
        case SQLCMD_EXEC:
        {
            char * buf = entry->exec.large_buf ? entry->exec.large_buf : entry->exec.short_buf;
            sqlConnExecDirectMany(buf, entry->exec.str_len, worker->conn, entry->exec.is_utf8);
            break;
        }
        default:
        {
            FatalErrorf("Invalid SQL FIFO comand %d", entry->cmd);
            break;
        }
    }

    if(worker->using_transactions)
    {
        #define SQL_STILL_EXECUTING        2

        // Asynchronous start committing the transaction
        worker->transaction_in_progress = (sqlConnEndTransaction(true, false, worker->conn) == SQL_STILL_EXECUTING);
    }

    return entry;
}

static void processQueueEntry(SqlQueueEntry * entry)
{
    static int recursing = 0;

    sqlfifo.in_flight--;
    if (!sqlfifo.in_flight)
        sql_task_freeze();

    recursing++;
    switch (entry->cmd) {
        case SQLCMD_BARRIER:
        {
            break;
        }
        case SQLCMD_CONTAINERUPDATE:
        {
            int value;

            sqlUpdateContainerComplete(entry->container.tplt, entry->container.id);

            freeLineList(&entry->container.diff_list);

            // Decrement and remove from stash when zero
            assert(stashIntRemoveInt(s_writes_pending[entry->container.tplt->dblist_id], entry->container.id, &value));
            if (--value)
                assert(stashIntAddInt(s_writes_pending[entry->container.tplt->dblist_id], entry->container.id, value, false));
            break;
        }
        case SQLCMD_READCONTAINER:
        {
            if (entry->readcontainer.data)
                sqlReadContainerLoadDependencies(entry);
            else
                sqlReadContainerLoadComplete(entry);
            break;
        }
        case SQLCMD_READCOLUMNS:
        {
            free(entry->readcolumns.limit);
            free(entry->readcolumns.col_names);
            free(entry->readcolumns.restrict);

            
            if ((entry->readcolumns.packet && linkValid(entry->readcolumns.link, entry->readcolumns.link_UID)) ||
                (!entry->readcolumns.packet && !entry->readcolumns.link && !entry->readcolumns.link_UID))
            {
                entry->readcolumns.callback(entry->readcolumns.packet, entry->readcolumns.cols, entry->readcolumns.col_count, entry->readcolumns.field_ptrs, entry->readcolumns.callbackData);
            }
            else
            {
                pktFree(entry->readcolumns.packet);
            }

            free(entry->readcolumns.field_ptrs);
            free(entry->readcolumns.cols);
            break;
        }
        case SQLCMD_DELETEROW:
        {
            free(entry->deleterow.table);
            break;
        }
        case SQLCMD_EXEC:
        {
            if (entry->exec.large_buf)
                free(entry->exec.large_buf);
            break;
        }
        default:
        {
            FatalErrorf("Invalid SQL FIFO comand %d", entry->cmd);
            break;
        }
    }
    recursing--;

    MP_FREE(SqlQueueEntry, entry);

    if (!sqlfifo.in_flight && !recursing) {
        unsigned i;

        // If there are no requests in flight, make sure there are no outstanding delayed entries
        for (i=0; i<MAX_CONTAINER_TYPES; i++) {
            assert(stashGetValidElementCount(s_reads_pending[i]) == 0);
            assert(stashGetValidElementCount(s_writes_pending[i]) == 0);
        }
        assert(mpGetAllocatedCount(MP_NAME(SqlQueueEntry)) == 0);
        assert(mpGetAllocatedCount(MP_NAME(SqlDelayedContainerCallback)) == 0);
    }
}
