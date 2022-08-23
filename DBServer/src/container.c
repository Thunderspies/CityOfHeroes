#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "container.h"
#include "comm_backend.h"
#include <utilitieslib/utils/file.h>
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/timing.h>
#include "dbserver/servercfg.h"
#include "container_tplt_utils.h"
#include "container_diff.h"
#include "container_merge.h"
#include "dbinit.h"
#include <utilitieslib/utils/error.h>
#include <assert.h>
#include <utilitieslib/utils/strings_opt.h>
#include "dblog.h"
#include "namecache.h"
#include "container_tplt.h"
#include <utilitieslib/components/EString.h>
#include "dbperf.h"
#include "dbmsg.h" // for containerBroadcast
#include <utilitieslib/components/StashTable.h>
#include "waitingEntities.h"
#include <utilitieslib/utils/Stackdump.h>
#include "backup.h"
#include "sql_fifo.h"
#include <utilitieslib/utils/log.h>
#include <utilitieslib/utils/ConvertUtf.h>
#include <utilitieslib/components/earray.h>

DbList    container_lists[MAX_CONTAINER_TYPES];
int        container_count;

int cmpLines(const LineTracker *a,const LineTracker *b)
{
    return a->idx - b->idx;
}

char *findFieldTplt(ContainerTemplate *tplt,LineList *list,char *field,char *buf)
{
    int            idx;
    LineTracker    *line,search;

    if (!tplt)
        return 0;
    if (!stashFindInt(tplt->all_hashes,field,&idx))
        return 0;
    search.idx = idx;
    line = bsearch(&search,list->lines,list->count,sizeof(search),cmpLines);
    if (line)
        return strcpy(buf,lineValueToText(tplt,list,line));
    return 0;
}

char *containerGetText(AnyContainer *any_container)
{
    DbList    *list;
    DbContainer    *container = any_container;

    if (!container)
        return 0;
    list = dbListPtr(container->type);
    if (list->tplt)
        return lineListToText(list->tplt,&container->line_list,0);
    return container->raw_data;
}

DbList *dbListPtr(int idx)
{
    return &container_lists[idx];
}

static int containerValid(DbContainer *container)
{
    char    buf[200];

    if (container->type != CONTAINER_ENTS)
        return 1;
    if (!container->just_created)
        return 1;
    if (findFieldTplt(dbListPtr(CONTAINER_ENTS)->tplt,&container->line_list,"LoginCount",buf))
        return 1;
    return 0;
}

static void containerFree(AnyContainer *container_void)
{
    int        i;
    DbList    *list;
    DbContainer    *container = container_void;

    SAFE_FREE(container->raw_data);
    freeLineList(&container->line_list);
    list = dbListPtr(container->type);
    unsetSpecialColumns(list,container);

    stashIntRemovePointer(list->cid_hashes, container->id, NULL);
    switch (container->type)
    {
    case CONTAINER_ENTS:
        {
            U32 auth_id = ((EntCon*)container)->auth_id;
            DbContainer *ent_con;
            if(stashIntFindPointer(list->other_hashes, auth_id, &ent_con) && ent_con == container)
            {
                stashIntRemovePointer(list->other_hashes, auth_id, NULL);
            }
        }
        eaDestroyEx(&((EntCon*)container)->completedOrders, NULL);
        break;
    case CONTAINER_SHARDACCOUNT:
        {
            DbContainer *shardaccount_con;
            if(stashFindPointer(list->other_hashes,((ShardAccountCon*)container)->account,&shardaccount_con) && shardaccount_con == container)
            {
                stashRemovePointer(list->other_hashes,((ShardAccountCon*)container)->account,NULL);
            }
        }
        eaDestroyEx(&((ShardAccountCon*)container)->inventory, NULL);
        break;
    }
    if (CONTAINER_IS_GROUP(container->type))
    {
        GroupCon    *group = container_void;

        if (group->member_ids)
            free(group->member_ids);
        if (group->member_ids_loaded)
            free(group->member_ids_loaded);
    }
    free(container);
    for(i=0;i<list->num_alloced;i++)
    {
        if (list->containers[i] == container)
        {
            memmove(&list->containers[i],&list->containers[i+1],(--list->num_alloced - i) * sizeof(void *));
            break;
        }
    }
}

DbList *containerListRegister(    int id, char *name, int struct_size, StatusCallback status_cb,
                                ContainerUpdateCallback updt_cb, SpecialColumn *special_columns )
{
    DbList    *list;
    char    template_fname[1000];

    if (id >= ARRAY_SIZE(container_lists))
        return 0;

    if (id >= container_count)
        container_count = id+1;

    list = &container_lists[id];
    if (list->in_use)
        return 0;

    strcpy(list->name,name);
    sprintf(list->fname,"server/db/%s.db",name);
    list->in_use = 1;
    list->id = id;
    list->struct_size = struct_size;
    list->special_columns = special_columns;

    sprintf(template_fname,"server/db/templates/%s.template",name);
    list->tplt = tpltLoad(id,name,template_fname);
    list->container_status_cb = status_cb;
    list->container_updt_cb = updt_cb;

    list->cid_hashes = stashTableCreateInt(0);
    if (list->id == CONTAINER_ENTS)
        list->other_hashes = stashTableCreateInt(0);
    else if (list->id == CONTAINER_SHARDACCOUNT)
        list->other_hashes = stashTableCreateWithStringKeys(0,0);
    return list;
}

static char *findFieldEither(ContainerTemplate *tplt,LineList *list,char *data,char *element,char *buf)
{
    if (data)
        return findFieldText(data,element,buf);
    if (list)
        return findFieldTplt(tplt,list,element,buf);
    return 0;
}

void containerGroupGetMembership(DbList *list, int id)
{
    GroupCon *group;
    if(!list->tplt->member_table) // members are run-time objects,
        return;
    group = containerPtr(list, id);
    if(group && !group->member_id_count)
    {
        ColumnInfo *field_ptrs[2];
        int i, *members, member_count;

        char *buf = estrTemp();
        estrPrintf(&buf, "WHERE %s = %d", list->tplt->member_field_name, id);

        members = (int*) sqlReadColumnsSlow(list->tplt->member_table, NULL, "ContainerId", buf, &member_count, field_ptrs);
        for(i = 0; i < member_count; i++)
            containerAddMember(list, id, members[i], 0);
        free(members);

        if(group->type == CONTAINER_SUPERGROUPS)
        {
            // Backup Supergroup and Base, if necessary
            U32 timestamp = timerSecondsSince2000();
            if(    backupSaveContainer(group, timestamp))
            {
                int base_id;
                DbContainer *base;
                estrPrintf(&buf, "%i", group->id);
                base_id = containerIdFindByElement(dbListPtr(CONTAINER_BASE), "SupergroupId", buf);
                base = containerLoad(dbListPtr(CONTAINER_BASE), base_id);
                backupSaveContainer(base, timestamp);
            }
        }

        estrDestroy(&buf);
    }
}

static void setSpecialColumn(SpecialColumn *col, DbContainer *container, uintptr_t base, char * buf, char **cache_data)
{
    void *addr = (void*)(base + col->offset);

    switch (col->type)
    {
        xcase CFTYPE_ANSISTRING:
            // pass
        xcase CFTYPE_UNICODESTRING:                    
            utf16ToUtf8(addr, 8192); // this code does not know what size the buffer is, assume the SQL max of 8K
        xcase CFTYPE_INT:
        {
            if (col->cmd != CMD_SETIFNULL || !*((int*)addr))
                *((int*)addr) = atoi(buf);
            if (col->cmd == CMD_MEMBER)
            {
                DbList    *group_list;
                int        member_id;

                group_list = dbListPtr(col->member_of);
                member_id = *((int*)addr);
                if (!member_id)
                    return;

                if (!containerPtr(group_list,member_id))
                {
                    devassertmsg(!cache_data || cache_data[col->member_of], "Performance Regression: setSpecialColumn has no cache data for container %d", col->member_of);
                    containerLoadCached(group_list,member_id,cache_data);
                    containerAlloc(group_list,member_id);
                }
                containerGroupGetMembership(group_list, member_id);
                containerAddMember(group_list,member_id,container->id,CONTAINERADDREM_FLAG_NOTIFYSERVERS);
                containerSetMemberState(group_list,member_id,container->id,1);
            }
        }
        xcase CFTYPE_BYTE:
            *((U8*)addr) = (U8)atoi(buf);                
        xcase CFTYPE_FLOAT:
            *((F32*)addr) = atof(buf);
        xcase CFTYPE_DATETIME:
            *((int*)addr) = timerGetSecondsSince2000FromString(buf);
        xdefault:
            devassertmsg(0, "Unimplemented special field type: %d\n", col->type);
            return;
    }

    if (col->cmd == CMD_CALLBACK)
        col->callback(container->id, addr);
}

static void setSpecialColumns(DbList *list,DbContainer *container,char *data,LineList *line_list,char **cache_data)
{
    char            buf[128];
    SpecialColumn    *cmds;
    int                i;
    ContainerTemplate    *tplt = list->tplt;

    if(list->container_updt_cb)
        list->container_updt_cb(container);

    cmds = list->special_columns;
    if (!cmds)
        return;

    for(i=0;cmds[i].name;i++)
    {
        int j = 0;

        while (true)
        {
            char tmpname[256];
            void ***arr = (void***)((uintptr_t)container + cmds[i].member_of);
            char * dest = (cmds[i].cmd != CMD_ARRAY && (cmds[i].type == CFTYPE_ANSISTRING || cmds[i].type == CFTYPE_UNICODESTRING)) ? ((U8*)container + cmds[i].offset) : buf;
            char * name = NULL;

            if (cmds[i].cmd == CMD_ARRAY)
            {
                sprintf_s(tmpname, ARRAY_SIZE_CHECKED(tmpname), cmds[i].name, j);
                name = tmpname;
            }
            else
                name = cmds[i].name;

            if (findFieldEither(tplt,line_list,data,name,dest))
            {
                if (cmds[i].cmd == CMD_ARRAY)
                {
                    void *base = eaGet(arr, j);
                    if (!base)
                    {
                        base = cmds[i].array_allocate();
                        eaSetForced(arr, base, j);
                    }

                    setSpecialColumn(&cmds[i], container, (uintptr_t)base, dest, cache_data);

                    j++;
                } else {
                    setSpecialColumn(&cmds[i], container, (uintptr_t)container, dest, cache_data);
                    break;
                }
            } else {
                if (cmds[i].cmd == CMD_ARRAY)
                {
                    int max = eaSize(arr);

                    // break after a gap after we have exceeded the container limit
                    if (j > max)
                        break;

                    j++;
                } else {
                    break;
                }
            }
        }
    }
}

bool containerGetField(DbList *list, DbContainer *container, int field_type, char *field_name, char * ret_str, int * ret_int, F32 *ret_f32 )
{
    bool ret = false;
    ContainerTemplate *tplt = list->tplt;
    LineList *line_list = &container->line_list;

    switch (field_type)
    {
        xcase CFTYPE_INT:
        {
            char buf[32];
            ret = (findFieldTplt(tplt, line_list, field_name, buf) != NULL);
            if (ret && ret_int)
                *ret_int = atoi(buf);
        }
        xcase CFTYPE_FLOAT:
        {
            char buf[32];
            ret = (findFieldTplt(tplt, line_list, field_name, buf) != NULL);
            if (ret && ret_f32)
                *ret_f32 = atof(buf);
        }
        xcase CFTYPE_DATETIME:
        {
            char buf[32];
            ret = (findFieldTplt(tplt, line_list, field_name, buf) != NULL);
            if (ret && ret_int)
                *ret_int = timerGetSecondsSince2000FromString(buf);
        }
        xdefault:
            if (ret_str)
                ret = (findFieldTplt(tplt, line_list, field_name, ret_str) != NULL);
    }

    return ret;
}

void unsetSpecialColumns(DbList *list,DbContainer *container)
{
    SpecialColumn    *cmds;
    int                i;

    if (!(cmds = list->special_columns))
        return;
    for(i=0;cmds[i].name;i++)
    {
        if (cmds[i].cmd == CMD_MEMBER)
        {
            DbList    *group_list;
            int        member_id;

            group_list = dbListPtr(cmds[i].member_of);
            member_id = *((int*)((U8*)container + cmds[i].offset));
            if (member_id)
                containerSetMemberState(group_list,member_id,container->id,0);
        }
    }
}

AnyContainer *containerUpdate(DbList *list,int id,char *diff_orig,int update_sql)
{
    THREADSAFE_STATIC struct
    {
        LineList    list;
        char        *diff;
        int            in_use;
    } diffs[5];
    THREADSAFE_STATIC int    static_idx;
    int            curr_idx;
    LineList    *diff_list;
    char        **diff;
    DbContainer        *container;
    int            size = (int)strlen(diff_orig);

    THREADSAFE_STATIC_MARK(diffs);

    if (!list)
        return 0;
    container = containerPtr(list,id);
    if (!list->tplt)
    {
        if (!container)
            return 0;
        tpltUpdateData(&container->raw_data,&container->container_data_size,diff_orig,list->tplt);
        setSpecialColumns(list,container,diff_orig,0,0);
        return container;
    }

    curr_idx = static_idx = (++static_idx) % ARRAY_SIZE(diffs);
    diff_list    = &diffs[curr_idx].list;
    diff        = &diffs[curr_idx].diff;
    assert(!diffs[curr_idx].in_use);
    diffs[curr_idx].in_use = 1;
 
    if (!container)
    {
        if (update_sql == 2) // update even if offline
        {
            textToLineList(list->tplt,diff_orig,diff_list,0);
            sqlContainerUpdateAsync(list->tplt,id,diff_list);
            // FIXME: list->container_updt_cb does not get called here and will
            // not be called until the container is loaded.
            // this meants the EntCatalog entry will be out of date
        }
        goto done;
    }
    if(!strstr( diff_orig, "Active " ) )
        estrPrintf(diff,"Active %d\n%s",timerSecondsSince2000() - container->on_since,diff_orig);
    else
        estrPrintCharString(diff, diff_orig);
    textToLineList(list->tplt,*diff,diff_list,0);
    tpltUpdateDataContainer(&container->line_list,diff_list,list->tplt);
    setSpecialColumns(list,container,0,diff_list,0);
    if (!update_sql)
        goto done;
    if (container->just_created)
    {
        container->just_created = 0;
        sqlContainerUpdateFromScratchAsync(list->tplt, container->id, &container->line_list, false);
    }
    else
    {
        sqlContainerUpdateAsync(list->tplt, container->id, diff_list);
    }
done:
    diffs[curr_idx].in_use = 0;
    
    if ( container && list->container_updt_cb )
        (*list->container_updt_cb)( container );
    
    return container;
}

AnyContainer *containerUpdate_notdiff(DbList *list,int id,char *str)
{
    DbContainer    *container;
    char        *diff;

    if (id < 0)
        container = containerAlloc(list,id);
    else
        container = containerPtr(list,id);
    if (!container)
        return 0;
    if (list->tplt)
        diff = containerDiff(containerGetText(container),str);
    else
        diff = str;
    containerUpdate(list,container->id,diff,1);
    //free(diff); TODO is this needed?
    return container;
}

AnyContainer *containerLoadCached(DbList *list,int id,char **cache_data)
{
    void        *mem = 0;
    DbContainer    *container;

    if (id <= 0)
        return 0;

    container = containerPtr(list,id);
    if (container)
        return container;

    if (cache_data)
        mem = cache_data[list->id];
    if (!mem) {
        sqlFifoTickWhileWritePending(list->tplt->dblist_id, id);
        mem = sqlContainerRead(list->tplt, id, SQLCONN_FOREGROUND);
    }
    if (!mem)
        return 0;

    container = containerAlloc(list,id);
    memToLineList(mem,&container->line_list);
    setSpecialColumns(list,container,0,&container->line_list,cache_data);

    // If this has unintended fallout, try changing it back to CONTAINER_IS_STATSERVER_GROUP
    if(CONTAINER_IS_GROUP(list->id))
        containerGroupGetMembership(list, id);
    if (mem)
        free(mem);
    if (cache_data)
        cache_data[list->id] = 0;
    //containerUpdate(list,id,"",1);
    return container;
}

AnyContainer *containerLoad(DbList *list,int id)
{
    return containerLoadCached(list,id,0);
}

void containerUnload(DbList *list,int id)
{
    DbContainer            *container = containerPtr(list,id);
    int                    save_ok = 1;
    ContainerTemplate    *tplt;

    if (!container)// || sqlTagQueuedContainerForUnload(list->id,id))
        return;
    if (list->id == CONTAINER_ENTS)
        LOG_DEBUG( "unloaded ent %d",id);

    container->active = 0;

    tplt = dbListPtr(container->type)->tplt;
    // Avoid saving partial created players
    if (tplt && containerValid(container) && !container->dont_save)
    {
        THREADSAFE_STATIC LineList    diff_list;
        THREADSAFE_STATIC_MARK(diff_list);

        if (textToLineList(tplt,"Active 0\n",&diff_list,0))
            sqlContainerUpdateAsync(tplt, container->id, &diff_list);
    }
    else if (container->type == CONTAINER_ENTS)
        playerNameDelete(((EntCon *)container)->ent_name,id);

    // DGNOTE 2/11/2010 - this is an extension to SVN commit # 48562.
    //In preparation for an assert in waitingEntitiesCheckEnt(...), we check here if the container is CONTAINER_ENTS.
    // If so, study all entries in the waitingEntities array, to see if the id in question is there.  If so, dump stack
    // here.  That'll let us know who called containerUnload(...) while the entity is in the waitingEntities array.
    if (container->type == CONTAINER_ENTS && waitingEntitiesIsIDWaiting(id))
    {
        char buffer[16384];
        LOG(LOG_ERROR, LOG_LEVEL_ALERT, 0, "Someone unloaded a player during map transfer. The stack has been logged for analysis.") 
        sdDumpStackToBuffer(buffer, sizeof(buffer), NULL);
        LOG(LOG_ERROR, LOG_LEVEL_IMPORTANT, 0, "%s", buffer);
    }
    containerFree(container);
}

void containerDelete(AnyContainer *container_void)
{
    DbList    *list;
    DbContainer    *container = container_void;

    list = dbListPtr(container->type);

    if (CONTAINER_IS_GROUP(container->type))
    {
        int            j;
        char        *buf = estrTemp();
        GroupCon    *con = (GroupCon *) container;

        extern teamCheckLeftMission(int map_id,int team_id, char *sWhy);

        if (list->id == CONTAINER_TEAMUPS)
        {
            teamCheckLeftMission(con->map_id, con->id, "containerDelete");
        }
        else if (list->id == CONTAINER_SUPERGROUPS)
        {
            estrPrintf(&buf, "DELETE FROM dbo.Base WHERE SupergroupId = %d;", container->id);

            TODO(); // These barriers could become a problem in a high load environments
            sqlFifoBarrier();
            sqlExecAsync(buf, estrLength(&buf));
            sqlFifoBarrier();
        }

        for(j = con->member_id_count-1; j >= 0; j--)
            containerRemoveMember(list, container->id, con->member_ids[j], 1, CONTAINERADDREM_FLAG_UPDATESQL);

        if(list->tplt->member_id == CONTAINER_ENTS)
        {
            // bruce: synchronous sql call to make extra sure deleting a team deletes the ent->team link
            estrPrintf(&buf, "UPDATE dbo.%s SET %s = NULL WHERE %s = %d;",
                        list->tplt->member_table ? list->tplt->member_table->name : "Ents",
                        list->tplt->member_field_name,
                        list->tplt->member_field_name,
                        con->id);

            TODO(); // These barriers could become a problem in a high load environments
            sqlFifoBarrier();
            sqlExecAsync(buf, estrLength(&buf));
            sqlFifoBarrier();
        }

        estrDestroy(&buf);
    }
    containerServerNotify(container->type, container->id, 0, 0);

    if (list->tplt)
        sqlDeleteContainer(list->tplt, container->id);
    containerFree(container);
}

AnyContainer *containerPtr(DbList *list,int idx)
{
    DbContainer    *c;

    if (!list || idx < 0)
        return 0;
    if (!idx || !stashIntFindPointer(list->cid_hashes, idx, &c))
        c = NULL;
    assert(!c || c->id == idx);
    if (c && server_cfg.container_size_debug) {
        assert(!c->raw_data || strlen(c->raw_data) < c->container_data_size);
    }
    return c;
}

EntCon *containerFindByAuthId(U32 auth_id)
{
    EntCon *ent_con;

    if(!auth_id || !stashIntFindPointer(dbListPtr(CONTAINER_ENTS)->other_hashes, auth_id, &ent_con))
        ent_con = NULL;

    return ent_con;
}

ShardAccountCon* containerFindByAuthShardAccount(const char *auth)
{
    ShardAccountCon *shardaccount_con;

    if(!auth || !stashFindPointer(dbListPtr(CONTAINER_SHARDACCOUNT)->other_hashes,auth,&shardaccount_con))
        shardaccount_con = NULL;

    return shardaccount_con;
}

static int getFreeIdx(DbList *list)
{
    int        id;

    for(id=list->num_alloced;;id++)
    {
        if (!containerPtr(list,id))
            return id;
    }
}

AnyContainer *containerAlloc(DbList *list,int idx)
{
    int            make_new=0;
    DbContainer    *container;
    #define MIN_ENTRIES 100

    if (idx < 0) // get any open slot
    {
        make_new = 1;
        if (list->tplt)
        {
            if (!list->max_container_id)
            {
                char cmd[SHORT_SQL_STRING];
                int cmd_len;

                switch (gDatabaseProvider) {
                    xcase DBPROV_MSSQL:
                        cmd_len = sprintf(cmd, "SELECT IDENT_CURRENT('%s');", list->tplt->tables->name);
                    xcase DBPROV_POSTGRESQL:
                        cmd_len = sprintf(cmd, "SELECT setval(pg_get_serial_sequence('dbo.%s', 'containerid'), nextval(pg_get_serial_sequence('dbo.%s', 'containerid')), false);", list->tplt->tables->name, list->tplt->tables->name);
                    DBPROV_XDEFAULT();
                }

                list->max_container_id = sqlGetSingleValue(cmd, cmd_len, NULL, SQLCONN_FOREGROUND);
            }
            idx = ++list->max_container_id;
        }
        else
        {
            idx = getFreeIdx(list);
            if (!idx) {
                idx++;
            }
        }
    }

    if ((container = containerPtr(list,idx)))
        return container;

    container = calloc(list->struct_size,1);
    container->on_since = timerSecondsSince2000() - 1;
    container->type = list->id;
    container->id  = idx;
    container->just_created = make_new;
    dynArrayAddp(&list->containers,&list->num_alloced,&list->max_alloced,container);
    stashIntAddPointer(list->cid_hashes, container->id, container, false);

    return container;
}

int containerLock(AnyContainer *container_any,U32 lock_id)
{
    DbContainer *container = (DbContainer *)container_any;

    if (container->type == CONTAINER_ENTS)
        LOG_DEBUG( "Locking id %d (new lock id %d)\n", container->id, lock_id );

    dbMemLog(__FUNCTION__, "Locking %s:%i, with lock %i", dbListPtr(container->type)->name, container->id, lock_id);
    if (container->locked)
    {
#if 0
        if (lock_id == container->lock_id)
            return 1; // ok for same owner to lock it twice
#endif
        dbMemLog(__FUNCTION__, "Failed");
        return 0; // already locked
    }
    container->locked = 1;
    container->lock_id = lock_id;
    return 1;
}

int containerUnlock(AnyContainer *container_any,U32 lock_id)
{
    DbContainer *container = (DbContainer *)container_any;

    if (container->type == CONTAINER_ENTS)
        LOG_DEBUG( "Unlocking id %d (old lock id %d)\n", container->id, container->lock_id );

    dbMemLog(__FUNCTION__, "Unlocking %s:%i, with lock %i",    dbListPtr(container->type)->name, container->id, lock_id);
    if (!container->locked)
    {
        dbMemLog(__FUNCTION__, "Failed");
        return 0; // already unlocked
    }
    if (lock_id && container->lock_id != lock_id)
    {
        assertmsgf(0, "%d.%d got unlock from bad lock_id!\n",container->type,container->id );
        return 0;
    }
    container->locked = 0;
    container->lock_id = 0;
    return 1;
}

void containerUnlockById(int lock_id)
{
    int            i,j;
    DbList        *list;
    DbContainer    *con;

    for(i=0;i<container_count;i++)
    {
        list = &container_lists[i];
        for(j=0;j<list->num_alloced;j++)
        {
            con = list->containers[j];

            if (con && con->lock_id == lock_id)
                containerUnlock(con,lock_id);
        }
    }
}


void containerSendList(DbList *list,int *selections,int count,int lock,NetLink *link,U32 user_data)
{
    char        *text;
    int            i,j;
    Packet        *pak;
    DbContainer *container;

    pak = pktCreateEx(link,DBSERVER_CONTAINERS);
    pktSendBitsPack(pak,1,user_data);
    pktSendBitsPack(pak,1,list->id);
    pktSendBitsPack(pak,1,count);
    for(i=0;i<count;i++)
    {
        pktSendBitsPack(pak,1,selections[i]);
        container = containerPtr(list,selections[i]);
        if (lock && container && container->type == CONTAINER_ENTS)
            LOG_DEBUG( "containerSendList lock %d",container->id);
        if (!container || (lock && !containerLock(container,dbLockIdFromLink(link))))
        {
            if (!container) {
                pktSendBitsPack(pak,1,CONTAINER_ERR_DOESNT_EXIST);
            } else {
                if (list == ent_list) 
                {
                    EntCon * ent_con = (EntCon*)container;
                    LOG_OLD_ERR( "Error sending \"%s\" (%d) to mapserver\n", ent_con->ent_name, ent_con->id);
                }
                pktSendBitsPack(pak,1,CONTAINER_ERR_ALREADY_LOCKED);
            }
            continue;
        }
        pktSendBitsPack(pak,1,0); // no error
        if (container->type == CONTAINER_ENTS)
        {
            EntCon *con = (EntCon *) container;
            if (server_cfg.log_relay_verbose) 
            {
                LOG_OLD("containerSendList Sending Ent %d, demand_loaded: %d, is_map_xfer: %d", con->id, con->demand_loaded, con->is_map_xfer);
            }
            pktSendBitsPack(pak,1,con->is_map_xfer);
        }
        else
            pktSendBitsPack(pak,1,0);

        if (container->type == CONTAINER_MAPS)
        {
            MapCon *con = (MapCon *) container;

            pktSendBitsPack(pak,1,con->is_static);
        }
        else
            pktSendBitsPack(pak,1,0);

        pktSendBitsPack(pak,1,lock);
        pktSendBitsPack(pak,1,container->is_deleting);
        pktSendBitsPack(pak,1,container->demand_loaded);
        if (CONTAINER_IS_GROUP(container->type))
        {
            GroupCon *con = (GroupCon *) container;

            pktSendBitsPack(pak,1,con->member_id_count);
            for(j=0;j<con->member_id_count;j++)
                pktSendBitsPack(pak,1,con->member_ids[j]);
        }
        else
            pktSendBitsPack(pak,1,0);
        text = containerGetText(container);
        logStatGetContainer(list->id, (int)strlen(text));
        pktSendString(pak,text);

        // @testing -AB: see if we can detect corruption  :07/19/06
        if( container && container->type == CONTAINER_ENTS && text )
        {
            extern bool checkContainerBadges_Internal(const char *container);
            static bool saveit = 10;
            if(!checkContainerBadges_Internal(text))
                LOG( LOG_ERROR, LOG_LEVEL_ALERT, 0, "id=%d bad badges container sent=%s",(saveit-- > 0)?text:"");
        }
    }
    pktSend(&pak,link);
}

void containerSendStatus(DbList *list,int *selections,int count,NetLink *link,U32 user_data)
{
    int            i;
    Packet        *pak;
    DbContainer    *con;
    char        buf[10000];

    pak = pktCreateEx(link,DBSERVER_CONTAINER_STATUS);
    pktSendBitsPack(pak,1,user_data);
    pktSendBitsPack(pak,1,list->id);
    pktSendBitsPack(pak,1,count);
    for(i=0;i<count;i++)
    {
        con = containerPtr(list,selections[i]);
        if (!con ||
            (list->id == CONTAINER_MAPS && ((MapCon*)con)->shutdown)) // If this is a mission map container that has been flagged to be shutting down, pretend it doesn't exist!
        {
            pktSendBitsPack(pak,1,0);
            continue;
        }
        pktSendBitsPack(pak,1,1);
        if (list->container_status_cb)
            list->container_status_cb((AnyContainer*)con,buf);
        else
            sprintf(buf,"%3d Active %d locked %d",con->id,con->active,con->locked);
        pktSendString(pak,buf);
    }
    pktSend(&pak,link);
}

int *listGetAll(DbList *list,int *ent_count,int active_only)
{
    int    i,maxcount=0;
    int *elist=NULL;

    for(*ent_count=i=0;i<list->num_alloced;i++)
    {
        if (list->containers[i] && list->containers[i]->active >= (U32)active_only) {
            int *p = dynArrayAdd(&elist, sizeof(int), ent_count, &maxcount, 1);
            *p = list->containers[i]->id;
        }
    }
    return elist;
}


void containerSendListByMap(DbList *list,int map_id,NetLink *link)
{
    int        *ilist,count;

    ilist = listGetAll(list,&count,map_id);
    containerSendList(list,ilist,count,0,link,0);
    free(ilist);
}

int inUseCount(DbList *list)
{
    return list->num_alloced;
}

void containerSendInfo(NetLink *link)
{
    Packet    *pak;
    int        i;
    DbList    *list;
    char    buf[1000],datestr[100];
    U32        dt;

    pak = pktCreateEx(link,DBSERVER_CONTAINER_INFO);
    pktSendBitsPack(pak,1,container_count-1 + 1);

    dt = timerSecondsSince2000() - server_cfg.time_started;
    sprintf(buf,"DbServer started on %s, Up %d hours, %d minutes",
            timerMakeDateStringFromSecondsSince2000(datestr,server_cfg.time_started),
            (dt / 3600), (dt / 60) % 60);
    pktSendString(pak,buf);

    //sprintf(buf,"Total connects: %d, Total disconnects: %d",server_cfg.player_connects,server_cfg.player_disconnects);
    //pktSendString(pak,buf);

    for(i=1;i<container_count;i++)
    {
        list = &container_lists[i];
        sprintf(buf,"%4d %s (%d)",inUseCount(list),list->name,i);
        pktSendString(pak,buf);
    }
    pktSend(&pak,link);
}

MapCon *containerFindByMapName(DbList *list,char *name)
{
    int        i;
    MapCon    *con;

    for(i=0;i<list->num_alloced;i++)
    {
        con = (MapCon *)list->containers[i];
        if (con && stricmp(con->map_name,name)==0)
            return con;
    }
    return 0;
}

MapCon *containerFindByIp(DbList *list,U32 ip)
{
    int        i;
    MapCon    *c;

    for(i=0;i<list->num_alloced;i++)
    {
        c = (MapCon *)list->containers[i];
        if (c && c->active && c->ip_list[0] == ip)
            return c;
    }
    return 0;
}

MapCon *containerFindBaseBySgId(int sgid)
{
    DbList *list = dbListPtr(CONTAINER_MAPS);
    int i;
    char sgbase_name[256];
    snprintf(sgbase_name, ARRAY_SIZE( sgbase_name ), "Base(supergroupid=%d)",sgid);
    
    for( i = 0; i < list->num_alloced; ++i ) 
    {
        MapCon *c = (MapCon *)list->containers[i];
        if( c && c->active && 0==strcmp(c->map_name,sgbase_name))
            return c;
    }
    return NULL;
}


// given a map id, verify that it exists.
// returns id, or 0
int containerVerifyMapId(DbList *list,char* value)
{
    int id, i;

    if (!value) return 0;
    id = atoi(value);

    for(i=0;i<list->num_alloced;i++)
    {
        if (list->containers[i]->id == id)
            return id;
    }
    return 0;
}

int containerIdFindByElement(DbList *list, char *element, char *value)
{
    char buf[SHORT_SQL_STRING];
    int buf_len;
    BindList bind_list[2];

    ZeroArray(bind_list);
    bind_list[0].data_type = CFTYPE_ANSISTRING;
    bind_list[0].data = value;

    buf_len = sprintf(buf, "SELECT ContainerId FROM dbo.%s WHERE %s = ?;", list->tplt->tables->name, element);
    return sqlGetSingleValue(buf, buf_len, bind_list, SQLCONN_FOREGROUND);
}

void containerSetMemberState(DbList *list,int id,int member_id,int loaded)
{
    GroupCon    *group_con;
    int            i,loaded_count = 0;

    group_con = containerPtr(list,id);
    if (!group_con)
        return;
    for(i=0;i<group_con->member_id_count;i++)
    {
        if (group_con->member_ids[i] == member_id)
        {
            if (loaded)
                SETB(group_con->member_ids_loaded,i);
            else
                CLRB(group_con->member_ids_loaded,i);
        }
    }
    if(!loaded && !CONTAINER_IS_STATSERVER_OWNED(list->id))
    {
        for(i=0;i<group_con->member_id_count;i++)
        {
            if (TSTB(group_con->member_ids_loaded,i))
                loaded_count++;
        }
        if (!loaded_count)
        {
            containerUnload(list,id);
        }
    }
}

void containerAddMember(DbList *list, int id, int member_id, int flags)
{
    int i, num_alloced, idx;
    DbList *member_list = dbListPtr(list->tplt->member_id);

    GroupCon *container = containerPtr(list,id);
    if (!container)
        return;

    for(i=0;i<container->member_id_count;i++)
        if (container->member_ids[i] == member_id)
            return;

    dbMemLog(__FUNCTION__, "Adding %i to %s:%i", member_id, list->name, id);
    if(flags&CONTAINERADDREM_FLAG_NOTIFYSERVERS) // otherwise we are simply loading
        containerServerNotify(list->id, id, member_id, 1);

    idx = container->member_id_count;
    num_alloced = container->member_id_max;
    dynArrayAdd(&container->member_ids,sizeof(container->member_ids[0]),&container->member_id_count,&container->member_id_max,1);
    if (num_alloced != container->member_id_max)
        container->member_ids_loaded = realloc(container->member_ids_loaded,4 * ((container->member_id_max + 31)/32));
    container->member_ids[idx] = member_id;
    if(member_list->tplt)
        CLRB(container->member_ids_loaded, idx); // set on load
    else
        SETB(container->member_ids_loaded, idx); // always loaded

    if((flags&CONTAINERADDREM_FLAG_UPDATESQL) && member_list->tplt)
    {
        char *buf = estrTemp();
        if(list->tplt->member_table != member_list->tplt->tables)
            estrConcatf(&buf, "%s[0].", list->tplt->member_table->name);
        estrConcatf(&buf, "%s %d\n", list->tplt->member_field_name, container->id);
        containerUpdate(member_list, member_id, buf, 2);
        estrDestroy(&buf);
    }
}

// this used to only be called from handleAddDelGroupMembers(), but i think it's
// correct to notify the mapservers any time an ent is removed from a group
// (the only other case currently is deletion of the group)
static void sendLostMembership(int list_id, int container_id, int ent_id)
{
    EntCon    *ent_con;
    NetLink    *link;
    Packet    *pak;

    ent_con = containerPtr(ent_list,ent_id);
    if(!ent_con)
        return;

    link = dbLinkFromLockId(ent_con->lock_id);
    if(!link)
        return;

    pak = pktCreateEx(link,DBSERVER_LOST_MEMBERSHIP);
    pktSendBitsPack(pak,1,list_id);
    pktSendBitsPack(pak,1,container_id);
    pktSendBitsPack(pak,1,ent_id);
    pktSend(&pak,link);
}

void containerRemoveMember(DbList *list, int id, int member_id, int deleting_container, int flags)
{
    int i;

    GroupCon *container = containerPtr(list,id);
    if (!container)
        return;

    dbMemLog(__FUNCTION__, "Removing %i from %s:%i", member_id, dbListPtr(container->type)->name, id);
    containerServerNotify(list->id, id, member_id, 0);

    for(i=0;i<container->member_id_count;i++)
    {
        if (container->member_ids[i] == member_id)
        {
            memmove(&container->member_ids[i],&container->member_ids[i+1],
                    (container->member_id_count-i-1) * sizeof(container->member_ids[0]));
            container->member_id_count--;

            if(flags & CONTAINERADDREM_FLAG_UPDATESQL)
            {
                if(list->tplt->member_id == CONTAINER_ENTS)
                    sendLostMembership(list->id, id, member_id);
                if(devassert(dbListPtr(list->tplt->member_id)->tplt))
                {
                    DbList    *member_list = dbListPtr(list->tplt->member_id);
                    char *buf = estrTemp();
                    if(list->tplt->member_table != member_list->tplt->tables)
                        estrConcatf(&buf, "%s[0].", list->tplt->member_table->name);
                    estrConcatf(&buf,"%s %d\n",list->tplt->member_field_name,0);
                    containerUpdate(dbListPtr(list->tplt->member_id),member_id,buf,2);
                    estrDestroy(&buf);
                }
            }
            break;
        }
    }

    if(!container->member_id_count && !deleting_container) // don't recurse back to containerDelete
        containerDelete(container);
    else if(container->member_id_count && (flags & CONTAINERADDREM_FLAG_NOTIFYSERVERS) )
        containerBroadcast(list->id,id,container->member_ids,container->member_id_count,0);
}
