/* Runs against the production container serializer and FIFO in DbServer. */
#include "database.h"
#include "comm_backend.h"
#include "sql/sqlinclude.h"
#include "database_sqlite_schema.h"
#include "container.h"
#include "container_tplt.h"
#include "container_merge.h"
#include "sql_fifo.h"
#include "dbserver/servercfg.h"
#include <utilitieslib/components/StashTable.h>
#include <utilitieslib/utils/wininclude.h>
#include <stdio.h>

#define CHECK(x) do { if (!(x)) { fprintf(stderr, "Container test line %d: %s\n", __LINE__, #x); return 1; } } while (0)
static int callbackCount, callbackValue;
static DWORD mainThread;
static void readComplete(Packet *packet, U8 *data, int count, ColumnInfo **fields, void *context)
{
    if (GetCurrentThreadId() != mainThread || count != 1 || !data) callbackValue = -1;
    else callbackValue = *(int *)data;
    ++callbackCount;
}

int dbStorageContainerTests(DbStorage **storage, const char *path)
{
    ContainerTemplate tplt = {0};
    TableInfo tables[2] = {0};
    ColumnInfo parent[10] = {0}, child[3] = {0};
    DbStorageForeignKey key = {"ContainerId", "Heroes", "ContainerId"};
    DbStorageError error = {0};
    DbStorageConnection *connection;
    LineList list = {0}, loaded = {0};
    char *memory, *text;
    int count, i;
    bool emptyRowFound = false;
    char *fixed;
    ColumnInfo *fields[8];
    static const char *names[] = {"ContainerId","Active","Name","Small","Short","Number","Fraction","Date","Ansi","Binary"};
    static const char *types[] = {"INTEGER","INTEGER","UTEXT(64)","INT1","INT2","INTEGER","REAL","DTEXT","TEXT(32)","BLOB"};
    static const ContainerFieldType fieldTypes[] = {CFTYPE_INT,CFTYPE_INT,CFTYPE_UNICODESTRING,CFTYPE_BYTE,CFTYPE_SHORT,CFTYPE_INT,CFTYPE_FLOAT,CFTYPE_DATETIME,CFTYPE_ANSISTRING,CFTYPE_BINARY_MAX};
    static const int widths[] = {4,4,130,1,2,4,4,16,33,-1};
    char input[] = "Active 1\nName \"dbo.O'Brien \xc3\xa9 \xf0\x9f\x98\x80\"\nSmall 255\nShort -32768\nNumber -2147483648\nFraction 1.25\nDate \"2026-10-07 12:34:56\"\nAnsi \"Quoted ' value\"\nBinary \"\\0\\n\\r\\\\\"\nItems[0].Value 7\nItems[2].Value 9\n";
    char update[] = "Number 42\nItems[0].Value 8\n";
    mainThread = GetCurrentThreadId();
    gDatabaseProvider = DBPROV_SQLITE; server_cfg.sql_allow_all_ddl = 1;
    connection = (*storage)->ops->connection(*storage, 0);
    tplt.tables = tables; tplt.table_count = 2; tplt.dblist_id = CONTAINER_TESTDATABASETYPES;
    strcpy(tables[0].name, "Heroes"); tables[0].columns = parent; tables[0].num_columns = 10; tables[0].table_type = TT_CONTAINER;
    for (i = 0; i < 10; ++i) {
        strcpy(parent[i].name, names[i]); strcpy(parent[i].data_type_name, types[i]); parent[i].data_type = fieldTypes[i]; parent[i].num_bytes = widths[i]; parent[i].exists_in_sql = 1;
        parent[i].column_size = i == 2 ? 64 : i == 8 ? 32 : 0;
        parent[i].reserved_word = i < 2;
    }
    strcpy(tables[1].name, "Items"); tables[1].columns = child; tables[1].num_columns = 3; tables[1].table_type = TT_SUBCONTAINER; tables[1].array_count = 4;
    child[0] = parent[0]; child[1] = parent[1]; strcpy(child[1].name, "SubId"); child[1].is_sub_id_field = 1;
    child[2] = parent[5]; strcpy(child[2].name, "Value");
    CHECK(dbStorageSqliteEnsureTable(connection, &tables[0], NULL, 0, &error));
    CHECK(dbStorageSqliteEnsureTable(connection, &tables[1], &key, 1, &error));
    hashAllNames(&tplt, "SQLite storage fixture");
    sqlFifoInit();
    CHECK(textToLineList(&tplt, input, &list, NULL));
    sqlContainerUpdateFromScratchAsync(&tplt, 17, &list, false);
    CHECK(sqlIsAsyncWritePending(tplt.dblist_id, 17));
    sqlReadColumnsAsync(&tables[0], "top 1", "Number", "WHERE ContainerId=17", readComplete, NULL, NULL, 17);
    CHECK(callbackCount == 0);
    sqlExecAsync("UPDATE dbo.Heroes SET Number=41 WHERE ContainerId=17", SQL_NTS);
    CHECK(sqlGetSingleValue("SELECT Number FROM dbo.Heroes WHERE ContainerId=17", SQL_NTS, NULL, 0) == 41);
    CHECK(callbackCount == 1 && callbackValue == -2147483647 - 1);
    CHECK(!sqlIsAsyncWritePending(tplt.dblist_id, 17));
    clearLineList(&list); CHECK(textToLineList(&tplt, update, &list, NULL));
    sqlContainerUpdateAsync(&tplt, 17, &list);
    sqlExecAsync("INSERT INTO dbo.Items(ContainerId,SubId) VALUES(17,3)", SQL_NTS);
    sqlFifoFinish();
    memory = sqlContainerRead(&tplt, 17, 0); CHECK(memory);
    memToLineList(memory, &loaded); text = lineListToText(&tplt, &loaded, 0);
    CHECK(strstr(text, "dbo.O'Brien") && strstr(text, "Number 42") && strstr(text, "Small 255") && strstr(text, "Short -32768"));
    CHECK(strstr(text, "Items[0].Value 8") && strstr(text, "Items[2].Value 9") && strstr(text, "Binary \"\\0\\n\\r\\\\\""));
    for (i = 0; i < loaded.count; ++i) if (loaded.lines[i].is_str && loaded.lines[i].str_idx == FAKE_STR_IDX &&
        loaded.lines[i].idx == tables[1].all_hash_first_idx + 3 * tables[1].num_columns) emptyRowFound = true;
    CHECK(emptyRowFound); /* Preserve an empty child after nonempty parent/child rows. */
    free(memory); freeLineList(&loaded);
    fixed = sqlReadColumnsSlow(&tables[0], "distinct", "Small, Short, Number, Name, Date", "WHERE ContainerId=17", &count, fields);
    CHECK(count == 1 && fixed && *(U8 *)fixed == 255 && *(S16 *)(fixed + 1) == -32768 && *(int *)(fixed + 3) == 42);
    CHECK(((WCHAR *)(fixed + 7))[0] == 'd');
    CHECK(((SQL_TIMESTAMP_STRUCT *)(fixed + 7 + 130))->year == 2026);
    free(fixed);
    sqlFifoShutdown(); (*storage)->ops->destroy(*storage);
    *storage = dbStorageCreateSqlite(path, &error); CHECK(*storage);
    sqlFifoInit();
    CHECK(sqlGetSingleValue("SELECT Number FROM Heroes WHERE ContainerId=17", SQL_NTS, NULL, 0) == 42);
    CHECK(sqlGetSingleValue("SELECT seq FROM sqlite_sequence WHERE name='Heroes'", SQL_NTS, NULL, 0) == 17);
    sqlDeleteContainer(&tplt, 17);
    CHECK(sqlGetSingleValue("SELECT COUNT(*) FROM Items", SQL_NTS, NULL, 0) == 0);
    CHECK(!sqlContainerRead(&tplt, 17, 0));
    sqlFifoShutdown();
    freeLineList(&list);
    for (i = 0; i < 2; ++i) stashTableDestroy(tables[i].name_hashes);
    stashTableDestroy(tplt.all_hashes); free(tplt.lines); free(tplt.slots);
    puts("Container SQLite persistence and FIFO contracts passed");
    return 0;
}
