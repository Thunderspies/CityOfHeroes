#include <stdio.h>
#include <utilitieslib/components/estring.h>
#include <utilitieslib/components/HashFunctions.h>
#include <utilitieslib/network/crypt.h>
#include "container.h"
#include "container_flatfile.h"
#include <utilitieslib/network/netio.h>
#include <utilitieslib/network/net_linklist.h>
#include "comm_backend.h"
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/assert/assert.h>
#include "doors.h"
#include <utilitieslib/utils/utils.h>
#include "dbserver/servercfg.h"
#include "weeklyTFcfg.h"
#include "launchercomm.h"
#include <utilitieslib/utils/file.h>
#include <direct.h>
#include <utilitieslib/utils/error.h>
#include "status.h"
#include "dbinit.h"
#include "dbmsg.h"
#include "dbdispatch.h"
#include "mapxfer.h"
#include "authcomm.h"
#include "dblog.h"
#include "dbmission.h"
#include "mapcrashreport.h"
#include "clientcomm.h"
#include "namecache.h"
#include "waitingEntities.h"
#include "container_merge.h"
#include "dbrelay.h"
#include "statagg.h"
#include "container_diff.h"
#include "arenacomm.h"
#include "statservercomm.h"
#include "container_sql.h"
#include "sql_fifo.h"
#include "container_tplt.h"
#include "container/container_util.h"
#include <utilitieslib/network/netcomp.h>
#include <utilitieslib/components/earray.h>
#include "offline.h"
#include "dbperf.h"
#include "backup.h"
#include "accountservercomm.h"
#include "missionservercomm.h"
#include "entity/entVarUpdate.h"
#include "queueservercomm.h"
#include "clientcomm.h"
#include "entity/gametypes.h"
#include "turnstiledb.h"
#include "auth/authUserData.h"
#include "dbEventHistory.h"
#include <utilitieslib/utils/log.h>
#include "chatrelay.h"
#include <utilitieslib/utils/ConvertUTF.h>
#include "overloadProtection.h"
#include "logserver.h"

static F32    curr_max_latency;
static int    curr_max_latency_cmd;

int start_zone_override = 0;

static void playerUnload(int player_id)
{
    //GroupCon    *group;
    int            team_id, league_id;
    EntCon        *container;
    int tsLeaderId = 0;
    container = containerPtr(dbListPtr(CONTAINER_ENTS),player_id);
    if (!container)
        return;
    team_id = container->teamup_id;
    league_id = container->league_id;
    if (container->league_id)
    {
        GroupCon *group = containerPtr(dbListPtr(CONTAINER_LEAGUES), container->league_id);
        tsLeaderId = group ? group->leader_id : 0;
    }
    if (!tsLeaderId && container->teamup_id)
    {
        GroupCon *group = containerPtr(dbListPtr(CONTAINER_TEAMUPS), container->teamup_id);
        tsLeaderId = group ? group->leader_id : 0;
    }
    if (!tsLeaderId)
    {
        tsLeaderId = player_id;
    }
    turnstileDBserver_removeFromQueueEx(player_id, tsLeaderId, 1);
    if (team_id)
    {
        DbList        *teamups = dbListPtr(CONTAINER_TEAMUPS);
        LOG_DEBUG( "removing player from team %d",container->teamup_id);
        containerRemoveMember(teamups, team_id, container->id, 0, CONTAINERADDREM_FLAG_UPDATESQL | CONTAINERADDREM_FLAG_NOTIFYSERVERS);
        //group = containerPtr(teamups,team_id);
        //if (group)
        //    containerBroadcast(CONTAINER_TEAMUPS,team_id,group->member_ids,group->member_id_count,0);
    }
    if (league_id)
    {
        NetLink *linkStat = statLink();
        DbList        *leagues = dbListPtr(CONTAINER_LEAGUES);
        if( linkStat )
        {
            Packet* pak_out = pktCreateEx(linkStat,DBSERVER_SEND_STATSERVER_CMD);
            if( pak_out )
            {
                char cmd[512];
                _snprintf_s(cmd, sizeof(cmd), sizeof(cmd)-1, "statserver_league_quit %d 0 0", player_id);
                pktSendBitsPack(pak_out,1,player_id);
                pktSendBitsPack(pak_out,1, 0);
                pktSendString(pak_out, cmd);

                pktSend(&pak_out, linkStat);
            }
        }
        containerRemoveMember(leagues, league_id, player_id, 0, CONTAINERADDREM_FLAG_UPDATESQL | CONTAINERADDREM_FLAG_NOTIFYSERVERS);
    }
    
    if (!waitingEntitiesRemove(container)) {
        // Do this only if the container wasn't already freed in a failure callback
        //  perhaps this should just not free in the callbacks?
        // Clear these, because it's going to be unloaded (possibly asyncronously)
        container->callback_link = NULL;
        container->is_gameclient = 0;
        containerUnload(dbListPtr(CONTAINER_ENTS),player_id);
    }
}

void sendAccountLogout(U32 auth_id,int reason,int use_time)
{
    ShardAccountCon *container = containerPtr(shardaccounts_list, auth_id);
    if (container && container->logged_in)
    {
        account_handleAccountLogout(container->id);
        container->logged_in = 0;
    }

    authSendQuitGame(auth_id, reason, use_time);
}

void sendPlayerLogout(int player_id,int reason,bool keep_account_logged_in)
{
    EntCon        *container;

    container = containerPtr(dbListPtr(CONTAINER_ENTS),player_id);
    LOG_DEBUG("logout on %d, container %s", player_id, container ? "valid" : "null");
    if (!container)
        return;

    backupSaveContainer(container,0);
    if (container && container->connected)
        container->connected = 0;

    if (!keep_account_logged_in)
        sendAccountLogout(container->auth_id, reason, timerSecondsSince2000() - container->on_since);

    dbMemLog(__FUNCTION__, "%i logging out", player_id);
    playerUnload(player_id);
}

int allContainerIds(int** ilist, int* ilist_max, int list_id, int limit)
{
    char limit_str[SHORT_SQL_STRING];
    char *cols;
    int row_count;
    ColumnInfo *field_ptrs[2];
    int i;
    int idx;

    if (limit) {
        switch (gDatabaseProvider) {
            xcase DBPROV_MSSQL:
                sprintf(limit_str, "top %d", limit);
            xcase DBPROV_POSTGRESQL:
                sprintf(limit_str, "limit %d", limit);
            DBPROV_XDEFAULT();
        }
    }

    cols = sqlReadColumnsSlow(dbListPtr(list_id)->tplt->tables, limit ? limit_str : NULL, "ContainerId", "order by containerid", &row_count, field_ptrs);
    if (!cols)
        return 0;

    dynArrayFit(ilist, sizeof(**ilist), ilist_max, row_count);

    for (idx=0, i=0; i<row_count; i++)
    {
        (*ilist)[i] = *(int *)(&cols[idx]);
        idx += field_ptrs[0]->num_bytes;
    }
    free(cols);

    return row_count;
}

void handleContainerReq(Packet *pak,NetLink *link)
{
    int        list_id,count,i,cmd,user_data, cmd_all, cmd_load, cmd_lock, cmd_temp;
    static int    *ilist,ilist_max,*loadlist,loadlist_max;

    user_data    = pktGetBitsPack(pak,1);
    list_id        = pktGetBitsPack(pak,1);
    cmd            = pktGetBitsPack(pak,1);
    count        = pktGetBitsPack(pak,1);

    assert(    cmd == CONTAINER_CMD_DONT_LOCK || !cmd || // these two should be the same...
            cmd == CONTAINER_CMD_LOCK ||
            cmd == CONTAINER_CMD_LOCK_AND_LOAD ||
            cmd == CONTAINER_CMD_LOAD_ALL ||
            cmd == CONTAINER_CMD_TEMPLOAD ||
            cmd == CONTAINER_CMD_TEMPLOAD_OFFLINE );

    cmd_all =    cmd == CONTAINER_CMD_LOAD_ALL;

    cmd_load =    cmd == CONTAINER_CMD_LOCK_AND_LOAD ||
                cmd == CONTAINER_CMD_LOAD_ALL ||
                cmd == CONTAINER_CMD_TEMPLOAD ||
                cmd == CONTAINER_CMD_TEMPLOAD_OFFLINE;

    cmd_lock =    cmd == CONTAINER_CMD_LOCK ||
                cmd == CONTAINER_CMD_LOCK_AND_LOAD;

    cmd_temp =    cmd == CONTAINER_CMD_TEMPLOAD ||
                cmd == CONTAINER_CMD_TEMPLOAD_OFFLINE;

    if(cmd_all)
        count = allContainerIds(&ilist, &ilist_max, list_id, 0);
    else
        dynArrayFit(&ilist, sizeof(ilist[0]), &ilist_max, count);
    dynArrayFit(&loadlist, sizeof(loadlist[0]), &loadlist_max, count);

    for(i=0;i<count;i++)
    {
        if(!cmd_all)
            ilist[i] = pktGetBitsPack(pak,1);
        loadlist[i] = !containerPtr(dbListPtr(list_id),ilist[i]);
        if(cmd_load)
        {
            DbContainer    *c;
            c = containerLoad(dbListPtr(list_id),ilist[i]);
            if(!c && cmd == CONTAINER_CMD_TEMPLOAD_OFFLINE)
                c = offlinePlayerTempLoad(ilist[i]);
            if(c)
            {
                if(loadlist[i])
                    c->demand_loaded = 1;
                if(server_cfg.log_relay_verbose && list_id==CONTAINER_ENTS)
                    LOG_OLD( "CONTAINER_CMD_TEMPLOAD; Ent %d, demand_loaded: %d, is_map_xfer: %d", c->id, c->demand_loaded, ((EntCon*)c)->is_map_xfer);
            }
        }
    }
    containerSendList(dbListPtr(list_id),ilist,count,cmd_lock,link,user_data);
    if(cmd_temp)
    {
        for(i=0;i<count;i++)
            if (loadlist[i])
                containerUnload(dbListPtr(list_id),ilist[i]);
    }
}

void sendContainerAcks(NetLink *link,int list_id,int callback_id,int count,int *ack_list)
{
    Packet    *pak_out;
    int        i;

    pak_out = pktCreateEx(link,DBSERVER_CONTAINER_ACK);
    pktSendBitsPack(pak_out,1,list_id);
    pktSendBitsPack(pak_out,1,callback_id);
    pktSendBitsPack(pak_out,1,count);
    for(i=0;i<count;i++)
    {
        pktSendBitsPack(pak_out,1,ack_list[i]);
    }
    pktSend(&pak_out,link);
}

AnyContainer* handleContainerUpdate(int list_id,int id,int notdiff,char* str)
{
    int teamup_map_id=0;
    DbContainer* container = containerPtr(dbListPtr(list_id),id);

    // mission map hack 1/3
    if (list_id == CONTAINER_TEAMUPS)
    {
        GroupCon    *team = containerPtr(dbListPtr(list_id),id);
        if (team)
            teamup_map_id = team->map_id;
    }

    if (notdiff)
        container = containerUpdate_notdiff(dbListPtr(list_id),id,str);
    else
        container = containerUpdate(dbListPtr(list_id),id,str,1);

    if (container && list_id == CONTAINER_ENTS)
        stat_UpdateStatsForEnt(id, str); // BINFIX

    // mission map hack 2/3
    if (container && list_id == CONTAINER_TEAMUPS && ((GroupCon *)container)->map_id != teamup_map_id)
        teamCheckLeftMission(teamup_map_id, id, "handleContainerUpdate");
    
    return container;
}

void handleSendPetition(char *authname, char *name, U32 ss2000, const char *summary, const char *msg, int cat, const char *notes)
{
    char datebuf[200];
    char *buf = estrTemp();
    char *s;

    if(!devassert(authname && name && summary && msg)) // these are required for RightNow to accept
        return;

    s = (char*)escapeString(authname);
    if(strlen(s) >= 32) // HACK: special knowledge from containerloadsave.c
        s[32-1] = 0;
    estrConcatf(&buf, "AuthName \"%s\"\n", s);

    s = (char*)escapeString(name);
    if(strlen(s) >= 128) // HACK: special knowledge from containerloadsave.c
        s[128-1] = 0;
    estrConcatf(&buf, "Name \"%s\"\n", s);

    estrConcatf(&buf, "Date \"%s\"\n", timerMakeDateStringFromSecondsSince2000(datebuf, ss2000));

    s = (char*)escapeString(summary);
    if(strlen(s) >= 128) // HACK: special knowledge from containerloadsave.c
        s[128-1] = 0;
    estrConcatf(&buf, "Summary \"%s\"\n", s);

    s = (char*)escapeString(msg);
    if (strlen(s) >= 1024) // HACK: special knowledge from containerloadsave.c
        s[1024-1] = 0;

    estrConcatf(&buf, "Msg \"%s\"\n",s);
    estrConcatf(&buf, "Category %d\n", cat);

    // TODO: add notes

    handleContainerUpdate(CONTAINER_PETITIONS, -1, 1, buf);

    estrDestroy(&buf);
}

int putCharacterVerifyFreeSlot(char *str)
{
    char authName[100];
    char restrict[100];
    int row_count;
    ColumnInfo *field_ptrs[4];
    char *rows;

    if (!findFieldText(str, "AuthName", authName))
        return 0;

    sprintf(restrict, "WHERE AuthName='%s'", authName);
    rows = sqlReadColumnsSlow(ent_list->tplt->tables,0,"ContainerId, Name",restrict, &row_count, field_ptrs);
    if (!rows)
        return 1;
    if (row_count >= MAX_PLAYER_SLOTS) {
        return 0;
    }

    // Could check for copying duplicate character here
    if (0) {
        int i, idx;
        for(idx=0,i=0;i<row_count;i++)
        {
            int id;
            char *name;
            id = *(int *)(&rows[idx]);
            idx += field_ptrs[0]->num_bytes;
            name = &rows[idx];
            idx += field_ptrs[1]->num_bytes;
            printf("%d.%s\n", id, name);
        }
    }

    return 1;
}

char *putCharacterVerifyUniqueName(char *str)
{
    static char *buf=NULL;
    static int buflen=0;
    char namebuf[100];
    char authIdBuffer[20];
    int baddata = 0;
    int renamed = 0;    
    U32 authId = 0;

    if (findFieldText(str, "Name", namebuf)) 
    {
        if (findFieldText(str, "AuthId", authIdBuffer))
        {
            authId = atoi(authIdBuffer);
        }

        while (!playerNameValid(namebuf, NULL, authId)) 
        {
            strcpy(namebuf, unescapeString(namebuf));
            incrementName(namebuf, 20); //MAX_PLAYER_NAME_LEN
            strcpy(namebuf, escapeString(namebuf));
            renamed = 1;
        }
        if (renamed) 
        {
            char diff[128];
            sprintf(diff, "Name \"%s\"\n", namebuf);
            // Update the string
            dynArrayFit(&buf, 1, &buflen, (int)strlen(str)+1);
            strcpy(buf, str);
            tpltUpdateData(&buf,&buflen,diff,dbListPtr(CONTAINER_ENTS)->tplt);//,1);
            str = buf;
        }
    } 
    else
    {
        baddata = 1;
    }
    if (baddata)
    {
        printf("Improperly formatted string for putcharacter: '%s'\n", str);
        return NULL;
    }
    return str;
}

char *putSupergroupVerifyUniqueName(char *str)
{
    static char *buf=NULL;
    static int buflen=0;
    char namebuf[100];
    int baddata = 0;
    int renamed = 0;

    if (findFieldText(str, "Name", namebuf)) {
        while (containerIdFindByElement(dbListPtr(CONTAINER_SUPERGROUPS),"Name",namebuf) > 0) {
            strcpy(namebuf, unescapeString(namebuf));
            incrementName(namebuf, 64); //MAX_PLAYER_NAME_LEN
            strcpy(namebuf, escapeString(namebuf));
            renamed = 1;
        }
        if (renamed) {
            char diff[128];
            sprintf(diff, "Name \"%s\"\n", namebuf);
            // Update the string
            dynArrayFit(&buf, 1, &buflen, (int)strlen(str)+1);
            strcpy(buf, str);
            tpltUpdateData(&buf,&buflen,diff,dbListPtr(CONTAINER_SUPERGROUPS)->tplt);//,1);
            str = buf;
        }
    } else {
        baddata = 1;
    }
    if (baddata) {
        printf("Improperly formatted string for putsupergroup: '%s'\n", str);
        return NULL;
    }
    return str;
}


int handleCreateEntByStr(char *str)
{
    static char tpltSortDiffErrorMessage[1024];

    DbContainer    *container;
    char *renamed_str;
    int dbid;
    EntCon *ent_container;

    if(!str)
        return 0;

    dbMemLog(__FUNCTION__, "creating %s by str", ent_list->name);

    logStatSetContainer(CONTAINER_ENTS, (int)strlen(str));

    {
        static    LineList    line_list;
        ContainerTemplate    *tplt = ent_list->tplt;

        if (tplt && !textToLineList(tplt,str,&line_list,tpltSortDiffErrorMessage)) {
            // Probably garbage data
            sendFail(NULL,1,CONTAINER_ERR_CANT_COMPLETE_SERIOUS,"handleCreateEntByStr: %d.%d tpltSortDiff failed, bad data sent to DbServer (%s)",CONTAINER_ENTS,-1,tpltSortDiffErrorMessage);
            return 0;
        }
    }
    if (str[0] && (!strEndsWith(str, "\n") || strstr(str,"ContainerId"))) {
        // Bad formatted data!
        sendFail(NULL,1,CONTAINER_ERR_CANT_COMPLETE_SERIOUS,"handleCreateEntByStr: %d.%d bad data sent to DbServer, doesn't end in \\n",CONTAINER_ENTS,-1);
        return 0;
    }

    // Creating a new entity.

    // Get the AuthName, make sure they don't already have 8 characters
    if (!putCharacterVerifyFreeSlot(str))
    {
        sendFail(NULL,0,CONTAINER_ERR_CANT_COMPLETE,"handleCreateEntByStr: Account already has too many characters or data missing AuthName field");
        return 0;
    }

    // Get the name, make sure it's valid
    renamed_str = putCharacterVerifyUniqueName(str);
    if(!renamed_str)
    {
        sendFail(NULL,1,CONTAINER_ERR_CANT_COMPLETE,"handleCreateEntByStr: error parsing character name from data");
        return 0;
    }
    else if(renamed_str != str)
    {
        // grant a rename token
        char    db_flags_text[200] = "";
        U32        db_flags;
        int        data_size;

        data_size = (int)strlen(renamed_str);
        renamed_str = strdup(renamed_str); // tpltUpdateData may need to resize, expects malloc'd string

        findFieldText(renamed_str,"DbFlags",db_flags_text);
        db_flags = atoi(db_flags_text);
        db_flags |= 1 << 12; // DBFLAG_RENAMEABLE
        sprintf(db_flags_text,"DbFlags %u\n",db_flags);
        tpltUpdateData(&renamed_str, &data_size, db_flags_text,ent_list->tplt);
    }

    container = handleContainerUpdate(CONTAINER_ENTS,-1,1,renamed_str);
    if (!container)
    {
        sendFail(NULL,1,CONTAINER_ERR_DOESNT_EXIST,"handleCreateEntByStr: %d.%d container does not exist",CONTAINER_ENTS,-1);
        if(renamed_str != str)
            free(renamed_str);
        return 0;
    }

    dbid = container->id;
    ent_container = (EntCon*)container;
    playerNameCreate(ent_container->ent_name, dbid, ent_container->auth_id);
    entCatalogPopulate(ent_container->id, ent_container);
    backupSaveContainer(container,0);
    containerUnload(ent_list,dbid);

    if(renamed_str != str)
        free(renamed_str);
    return dbid;
}

void handleContainerSet(Packet *pak,NetLink *link)
{
    int            list_id,count,callback_id,i,id,cmd,t,notdiff,debugdiff,new_character=0,dbquery_insert_player=0;
    char        *str = NULL;
    DbContainer    *container;
    static        int    *ack_list,ack_max;
    static        char tpltSortDiffErrorMessage[1024];

    list_id        = pktGetBitsPack(pak,1);
    cmd            = pktGetBitsPack(pak,1);
    callback_id    = pktGetBitsPack(pak,1);
    count        = pktGetBitsPack(pak,1);
    if (!count)
        return;
    dynArrayFit(&ack_list,sizeof(ack_list[0]),&ack_max,count);
    for(i=0;i<count;i++)
    {
        char    *orig_container_data=0;

        id = pktGetBitsPack(pak,1);
        dbMemLog(__FUNCTION__, "setting %s:%i, cmd %i",    dbListPtr(list_id)->name, id, cmd);
        // DGNOTE - VISITOR - fix this, id will be very tall.
        if ((id == -1 && cmd != CONTAINER_CMD_CREATE && cmd != CONTAINER_CMD_CREATE_MODIFY) ||
            (id != -1 && cmd == CONTAINER_CMD_CREATE))
        {
            sendFail(link,1,CONTAINER_ERR_DOESNT_EXIST,"handleContainerSet: %d.%d invalid set command: %d",list_id,id,cmd);
            return;
        }
        notdiff = pktGetBits(pak,1);
        debugdiff = pktGetBits(pak,1);
        str = pktGetString(pak);
        logStatSetContainer(list_id, (int)strlen(str));

        if (list_id == CONTAINER_ENTS && cmd == CONTAINER_CMD_CREATE)
        {
            dbquery_insert_player = 1;
        }
        
        {
            static    LineList    line_list;
            ContainerTemplate    *tplt = dbListPtr(list_id)->tplt;

            if (tplt && !textToLineList(tplt,str,&line_list,dbquery_insert_player ? 0 : tpltSortDiffErrorMessage)) {
                // Probably garbage data
                sendFail(link,1,CONTAINER_ERR_CANT_COMPLETE_SERIOUS,"handleContainerSet: %d.%d tpltSortDiff failed, bad data sent to DbServer (%s)",list_id,id,tpltSortDiffErrorMessage);
                return;
            }
        }
        if (str[0] && !strEndsWith(str, "\n")) {
            // Bad formatted data!
            sendFail(link,1,CONTAINER_ERR_CANT_COMPLETE_SERIOUS,"handleContainerSet: %d.%d bad data sent to DbServer, doesn't end in \\n",list_id,id);
            return;
        }

        if (list_id == CONTAINER_MAPS)
        {
            t = getMapId(str);
            if (t >= 0)
                id = t;
        }

        if (id > 0 && dbListPtr(list_id)->max_session_id < (U32)id)
            dbListPtr(list_id)->max_session_id = id;

        if (cmd == CONTAINER_CMD_DELETE || cmd == CONTAINER_CMD_UNLOCK_NOMODIFY)
            container = containerPtr(dbListPtr(list_id),id);
        else
        {
            container = containerPtr(dbListPtr(list_id),id);

            // crazy stuff
            if (container && list_id == CONTAINER_ENTS)
            {
                EntCon* ent = (EntCon*)container;
                char* mapidfield;
                mapidfield = strstri(str, "\nMapId");
                if (mapidfield)
                {
                    int id = atoi(mapidfield + strlen("\nMapId"));
                    assert (id == ent->map_id);
                }
            }
            // Creating a new entity?
            if (!container && dbquery_insert_player)
            {
                // This code apparently only happens when a -dbquery -set 3 -1 is run,
                //  but it seems it could be useful in a normal create if we have problems
                //  with duplicate names, etc
                // Get the AuthName, make sure they don't already have 8 characters
                if (!putCharacterVerifyFreeSlot(str))
                {
                    sendFail(link,0,CONTAINER_ERR_CANT_COMPLETE,"handleContainerSet: Account already has too many characters or data missing AuthName field");
                    return;
                }
                // Get the name, make sure it's valid
                str = putCharacterVerifyUniqueName(str);
                if (!str) {
                    sendFail(link,1,CONTAINER_ERR_CANT_COMPLETE,"handleContainerSet: error parsing character name from data");
                    return;
                }
                new_character = 1;
            }

            if (container && list_id != CONTAINER_MAPS)
            {
                if (container->locked && container->lock_id != dbLockIdFromLink(link))
                {
                    sendFail(link,1,CONTAINER_ERR_ALREADY_LOCKED,"handleContainerSet: %d.%d locked by different owner",list_id,id);
                    return;
                }
                if (!container->locked && cmd != CONTAINER_CMD_CREATE_MODIFY)
                {
                    sendFail(link,1,CONTAINER_ERR_NOT_LOCKED,"handleContainerSet: %d.%d not locked",list_id,id);
                    return;
                }
            }
            if (cmd == CONTAINER_CMD_TEMPLOAD)
                containerLoad(dbListPtr(list_id),id);
            if (debugdiff)
            {
                container = containerPtr(dbListPtr(list_id),id);
                if (container)
                    strdup_alloca(orig_container_data,containerGetText(container));
            }
            if (!container && cmd == CONTAINER_CMD_CREATE_MODIFY)
                container = containerAlloc(dbListPtr(list_id),id);
            container = handleContainerUpdate(list_id,id,notdiff,str);
            if (new_character && container) {
                playerNameCreate(((EntCon*)container)->ent_name, container->id, ((EntCon*)container)->auth_id);
            }
            if (debugdiff && container)
            {
                char *orig_str;

                strdup_alloca(orig_str,str);
                checkContainerDiff(pktGetString(pak),containerGetText(container),orig_str,orig_container_data);
            }

            // For any table insertion that is referenced by a foreign key
            // constraint, insert a barrier to make sure that the insertion
            // happens before it can be referenced by another container.
            if (cmd == CONTAINER_CMD_CREATE) {
                switch (list_id) {
                    case CONTAINER_TEAMUPS:
                    case CONTAINER_SUPERGROUPS:
                    case CONTAINER_TASKFORCES:
                    case CONTAINER_LEAGUES:
                        if (!dbListPtr(list_id)->tplt->dont_write_to_sql)
                            sqlFifoBarrier();
                }
            }
        }
        if (!container)
        {
            sendFail(link,1,CONTAINER_ERR_DOESNT_EXIST,"handleContainerSet: %d.%d container does not exist",list_id,id);
            return;
        }
        ack_list[i] = container->id;
        if (cmd == CONTAINER_CMD_UNLOCK || cmd == CONTAINER_CMD_UNLOCK_NOMODIFY)
            containerUnlock(container,dbLockIdFromLink(link));
        if (cmd == CONTAINER_CMD_DELETE)
            container->is_deleting = 1;


        if (list_id == CONTAINER_MAPS && !container->is_deleting)
        {
            MapCon            *mapcon = (MapCon *)container;
            DBClientLink    *client = link->userData;
            U32                force_launcher_ip = 0;
            int                succeed=1,map_id = ack_list[0];

            if (client->static_link)
                force_launcher_ip = link->addr.sin_addr.S_un.S_addr;

            if (!mapcon->link && !mapcon->is_static)
            {
                if (!launcherCommStartProcess(myHostname(), force_launcher_ip, mapcon, false))
                {
                    // Don't delete the map container on overload protection
                    if (!overloadProtection_DoNotStartMaps())
                    {
                        printf("launchfail: deleting mapcontainer %d\n",map_id);
                        containerDelete(mapcon);
                    }
                    container = 0;
                    ack_list[0] = 0;
                    succeed = 0;
                }
                LOG_OLD( "dbserver starting mission map %i", map_id);
            }
            if (succeed)
            {
                mapcon->callback_link = link;
                mapcon->callback_idx = callback_id;
                getMapName(mapcon->raw_data,mapcon->map_name);
            }
        }

        if (CONTAINER_IS_GROUP(list_id) && id >= 0 && (cmd != CONTAINER_CMD_UNLOCK_NOMODIFY))
        {
            GroupCon    *group_con = (GroupCon *)container;

            containerBroadcast(list_id,id,group_con->member_ids,group_con->member_id_count,link);
        }
        else if (list_id == CONTAINER_AUTOCOMMANDS)
        {
            containerBroadcastAll(list_id, container->id); // container->id will be updated if id == -1 (creating new)
        }
        else
        {
            sendContainerAcks(link,list_id,callback_id,count,ack_list);
        }

        if (container && container->is_deleting)
        {
            if (container->type == CONTAINER_MAPS)
            {
                printf("containerset deleting container %d\n",container->id);
                LOG_OLD("dbserver deleting map %i", container->id);
            }
            mapOrTeamSpecialFree(container);
            containerDelete(container);
        }

        if (container && cmd == CONTAINER_CMD_TEMPLOAD)
            containerUnload(dbListPtr(list_id),id);
        else if (container && dbquery_insert_player)
            containerUnload(dbListPtr(list_id),container->id);

    } // end for(i=0;i<count;i++)
}

void containerSendReceipt(NetLink* link, U32 cmd, U32 listid, U32 cid, U32 user_cmd, U32 user_data, U32 err, char* str)
{
    Packet* ret;

    ret = pktCreateEx(link, DBSERVER_CONTAINER_RECEIPT);
    containerSendTrace(ret, cmd, listid, cid, user_cmd, user_data);
    pktSendBitsPack(ret, 1, err);
    pktSendString(ret, str);
    pktSend(&ret, link);
}

// forward container relay request to appropriate server
void handleContainerRelay(Packet *pak,NetLink *link)
{
    U32        cmd, listid, cid, user_cmd, user_data;
    DbList    *dblist;
    NetLink *serverlink;
    Packet  *out;
    int        packetdebug = pktGetDebugInfo();

    containerGetTrace(pak, &cmd, &listid, &cid, &user_cmd, &user_data);

    dblist = dbListPtr(listid);
    if (!dblist)
    {
        containerSendReceipt(link, cmd, listid, cid, user_cmd, user_data, CONTAINER_ERR_CANT_COMPLETE, "Incorrect container id");
        return;
    }

    // TODO - add support for sending a command to an ent this way?
    serverlink = dbLinkFromLockId(dblist->owner_lockid);
    if (!dblist->owner_lockid || !serverlink)
    {
        containerSendReceipt(link, cmd, listid, cid, user_cmd, user_data, CONTAINER_ERR_CANT_COMPLETE, "ServerNotRunning");
        return;
    }

    // TODO - if we want to allow syncronous waits on mapserver, need to
    // send the current cookie to the container server & skip it

    // forward message
    pktSetDebugInfoTo(pak->hasDebugInfo); // need to send same style as we received
    out = pktCreateEx(serverlink, DBSERVER_CONTAINER_RELAY);
    containerSendTrace(out, cmd, listid, cid, user_cmd, user_data);
    pktSendBitsPack(out, 1, dbLockIdFromLink(link));
    pktAppendRemainder(out, pak);
    pktSend(&out, serverlink);
    pktSetDebugInfoTo(packetdebug);
}

// forward a response from the container server to map
void handleContainerReceipt(Packet* pak, NetLink* link)
{
    int        lock_id;
    NetLink *maplink;
    Packet* out;
    int        packetdebug = pktGetDebugInfo();

    lock_id        = pktGetBitsPack(pak, 1);

    maplink = dbLinkFromLockId(lock_id);
    if (!maplink)
    {
        // should be fine to drop a response to a map that already closed
        if (!isProductionMode())
        {
            U32 cmd, listid, cid, user_cmd, user_data;
            containerGetTrace(pak, &cmd, &listid, &cid, &user_cmd, &user_data);
            printf("dropped container server response message (lock:%i,cmd:%i,list:%i,cid:%i)\n", lock_id, cmd, listid, cid);
        }
        return;
    }

    // forward message
    pktSetDebugInfoTo(pak->hasDebugInfo);    // need to send same style as we received
    out = pktCreateEx(maplink, DBSERVER_CONTAINER_RECEIPT);
    pktAppendRemainder(out, pak);
    pktSend(&out, maplink);
    pktSetDebugInfoTo(packetdebug);
}

// forward a container update to interested parties, optionally do an unlocked save at the same time
void handleContainerReflect(Packet* pak, NetLink* link)
{
    U32            list_id;
    int            cmd, saving, deleting, i, m;
    char*        str;
    DbContainer* container;
    EntCon*        ent_con;
    MapCon*        map_con;
    ContainerReflectInfo**    reflect_in = 0;
    ContainerReflectInfo**    reflect_out = 0;
    static int    *ids;
    static int  *locks;
    int        packetdebug = pktGetDebugInfo();

    if (!locks)
        eaiCreate(&locks);
    if (!ids)
        eaiCreate(&ids);

    list_id = pktGetBitsPack(pak, 1);
    cmd = pktGetBitsPack(pak, 1);
    containerReflectGet(pak, &reflect_in);

    // only if we are setting, do we break apart the input packet
    saving = cmd == CONTAINER_CMD_CREATE_MODIFY;
    deleting = cmd == CONTAINER_CMD_DELETE;
    if (saving || deleting)
    {
        // array of ids first, then contents follow if saving
        eaiSetSize(&ids, pktGetBitsPack(pak, 1));        // count
        for (i = 0; i < eaiSize(&ids); i++)
            ids[i] = pktGetBitsPack(pak, 1);

        for (i = 0; i < eaiSize(&ids); i++)
        {
            container = containerPtr(dbListPtr(list_id),ids[i]);
            if (saving)
            {
                str = pktGetString(pak);
                if (!container)
                    container = containerAlloc(dbListPtr(list_id),ids[i]);
                container = handleContainerUpdate(list_id,ids[i],1,str);
            }
            else // deleting
            {
                if (container)
                    containerDelete(container);
            }
        }
    }

    // multiply groups to get dbids of affected parties,
    // also build list of servers (by lock_id) that host those dbids
#define PUSH_REFLECTION(dbid, lockid) \
    containerReflectAdd(&reflect_out, reflect_in[i]->target_list, reflect_in[i]->target_cid, dbid, reflect_in[i]->del); \
    reflect_out[eaSize(&reflect_out)-1]->lock_id = lockid; \
    if (eaiFind(&locks, lockid) == -1) \
        eaiPush(&locks, lockid);

    eaiSetSize(&locks, 0);
    for (i = 0; i < eaSize(&reflect_in); i++)
    {
        // groups get multiplied out to dbid's
        if (CONTAINER_IS_GROUP(reflect_in[i]->target_list))
        {
            GroupCon* group = containerPtr(dbListPtr(reflect_in[i]->target_list), reflect_in[i]->target_cid);
            if (!group) continue; // ok, just not loaded
            for (m = 0; m < group->member_id_count; m++)
            {
                if (TSTB(group->member_ids_loaded,m))
                {
                    ent_con = containerPtr(ent_list, group->member_ids[m]);
                    if (ent_con && ent_con->lock_id)
                    {
                        PUSH_REFLECTION(group->member_ids[m], ent_con->lock_id);
                    }
                }
            }
        }
        else if (reflect_in[i]->target_list == CONTAINER_ENTS)
        {
            ent_con = containerPtr(ent_list, reflect_in[i]->target_cid);
            if (ent_con && ent_con->lock_id)
            { 
                PUSH_REFLECTION(0, ent_con->lock_id);
            }
        }
        else if (reflect_in[i]->target_list == CONTAINER_MAPS)
        {
            if (reflect_in[i]->target_cid == -1) // request to send to all maps
            {
                for (m = 0; m < map_list->num_alloced; m++)
                {
                    map_con = (MapCon*)map_list->containers[m];
                    if (map_con && map_con->link)
                    {
                        PUSH_REFLECTION(0, dbLockIdFromLink(map_con->link));
                    }
                }
            }
            else // a single map id
            {
                map_con = containerPtr(map_list, reflect_in[i]->target_cid);
                if (map_con && map_con->link)
                {
                    PUSH_REFLECTION(0, dbLockIdFromLink(map_con->link));
                }
            }
        }
        else if (reflect_in[i]->target_list == REFLECT_LOCKID) // signal that cid is actually the lock_id, derived from a relay trace
        {
            PUSH_REFLECTION(0, reflect_in[i]->target_cid);
        }
    }

    // now, send to each map link
    for (i = 0; i < eaiSize(&locks); i++)
    {
        NetLink* map_link = dbLinkFromLockId(locks[i]);
        Packet* ret;

        if (!map_link) continue; // no problem, we've dropped connection since last request

        pktSetDebugInfoTo(pak->hasDebugInfo);
        ret = pktCreateEx(map_link, DBSERVER_CONTAINER_REFLECT);
        pktSendBitsPack(ret, 1, list_id);
        pktSendBitsPack(ret, 1, cmd);
        for (m = 0; m < eaSize(&reflect_out); m++)
            reflect_out[m]->filter = (reflect_out[m]->lock_id != locks[i])? 1: 0;
        containerReflectSend(ret, &reflect_out);

        // if we broke apart input packet earlier, get data from containers
        if (saving || deleting)
        {
            // ids first
            pktSendBitsPack(ret, 1, eaiSize(&ids));
            for (m = 0; m < eaiSize(&ids); m++)
                pktSendBitsPack(ret, 1, ids[m]);
            if (!deleting)
            {
                for (m = 0; m < eaiSize(&ids); m++)
                {
                    container = containerPtr(dbListPtr(list_id), ids[m]);
                    pktSendString(ret, containerGetText(container));
                }
            }
        }
        else
            pktAppendRemainder(ret, pak);

        pktSend(&ret, map_link);
        pktSetDebugInfoTo(packetdebug);
    }

    containerReflectDestroy(&reflect_in);
    containerReflectDestroy(&reflect_out);
#undef PUSH_REFLECTION
}

void handleStatusReq(Packet *pak,NetLink *link)
{
    int        list_id,count,*ilist,i,active_only=1;
    U32        user_data;

    user_data    = pktGetBitsPack(pak,1);
    list_id        = pktGetBitsPack(pak,1);
    count        = pktGetBitsPack(pak,1);
    ilist        = malloc(sizeof(ilist[0]) * count);
    for(i=0;i<count;i++)
    {
        ilist[i] = pktGetBitsPack(pak,1);
        if (ilist[i] < 0)
        {
            if (ilist[i] == -2)
                active_only = 0;
            free(ilist);
            ilist = listGetAll(dbListPtr(list_id),&count,active_only);
            break;
        }
    }
    containerSendStatus(dbListPtr(list_id),ilist,count,link,user_data);
    free(ilist);
}

void handleMapXferRequest(Packet *pak,NetLink *link)
{
    // Start a map if needed
    int        map_id,id,fix_map_id,find_best_map;
    EntCon    *ent_con;
    MapCon    *map_con;
    char    mapinfo[1024];
    char    errmsg[1000];
    char source[1000];
    U8 *extra_data = NULL;
    U32 extra_size = 0, extra_zsize = 0, extra_crc = 0;

    // A user has requested a mapxfer, find him a new map and start it, if it's ready, just notifiy

    map_id = pktGetBitsPack(pak,1);
    strncpyt(mapinfo, pktGetString(pak), ARRAY_SIZE(mapinfo));
    find_best_map = pktGetBits(pak,1);
    id = pktGetBitsPack(pak,1);
    if(pktGetBits(pak, 1))
    {
        extra_crc = pktGetBitsAuto(pak);
        extra_data = pktGetZippedInfo(pak, &extra_zsize, &extra_size); // mallocs data
        if(!devassert(mapinfo[0]))
            SAFE_FREE(extra_data);
    }

    ent_con = containerPtr(dbListPtr(CONTAINER_ENTS),id);
    if (!ent_con)
        return;

    if(extra_data)
    {
        sprintf(errmsg, "BadMapRequest");
        doneWaitingMapXferFailed(ent_con, errmsg);
    }

    fix_map_id = checkMapIDForOverrides(map_id, ent_con);
    if (fix_map_id < 0)
        fix_map_id = ent_con->static_map_id;
    // client asking for a instanced map that matches mapinfo
    if (mapinfo[0])
    {
        sprintf(source, "MapXfer(%d/%d : %s)", ent_con->map_id, ent_con->static_map_id, mapinfo);
    }
    else if (find_best_map)
    {
        int orig = fix_map_id;
        fix_map_id = findBestMapByBaseId(ent_con, fix_map_id);
        sprintf(source, "MapXfer(%d/%d : %d to %d)", ent_con->map_id, ent_con->static_map_id, orig, fix_map_id);
    }
    else
    {
        sprintf(source, "MapXfer(%d/%d : %d)", ent_con->map_id, ent_con->static_map_id, fix_map_id);
    }
    ent_con->callback_link = link;
    ent_con->is_gameclient = 0;
    if (mapinfo[0])
        map_con = startInstancedMap(link, ent_con, mapinfo, errmsg, source);
    else
    {
        map_con = startMapIfStatic(link, ent_con, fix_map_id, errmsg, source);
    }
    if (!map_con) {
        // Map failed to start or doesn't exist
        doneWaitingMapXferFailed(ent_con, errmsg);
    } else {
        // When this map finishes loading, we need to notify the mapserver the entity is currently on
        // if it's already started this will call the callback immediately
        waitingEntitiesAddEnt(ent_con, map_con->id, 1, map_con);
    }
}

void handleMapXfer(Packet *pak,NetLink *link)
{
    int        count,i,map_id,id,fix_map_id,ret;
    EntCon    *ent_con;
    char    errmsg[1000];

    // At this point the mapserver has reliquished ownership of the entity, he
    // should be sent immediately to the new map, if the new map has failed to start,
    // start a new one and deny the map transfer request

    map_id = pktGetBitsPack(pak,1);
    count = pktGetBitsPack(pak,1);
    for(i=0;i<count;i++)
    {
        id = pktGetBitsPack(pak,1);
        ent_con = containerPtr(dbListPtr(CONTAINER_ENTS),id);
        if (!ent_con)
            continue;
        fix_map_id = checkMapIDForOverrides(map_id, ent_con);
        if (fix_map_id < 0)
            fix_map_id = ent_con->static_map_id;
        dbMemLog(__FUNCTION__, "Received DBCLIENT_MAP_XFER to map %d for %s:%d", fix_map_id, ent_con->ent_name, ent_con->id);
        if (fix_map_id == ent_con->map_id) {
            // Trying to move from this map to the same map!  Invalid.  This shouldn't ever happen
            doneWaitingMapXferFailed(ent_con, "Request to mapmove to the same map denied");
        } else {
            containerUnlock(ent_con,0); // force unlock since the mapserver he was on reliquished ownership
            ret = setNewMap(ent_con,fix_map_id,link,0,errmsg,1);
            if (!ret) {
                assert(ent_con->callback_link == link);
                // handle immediate failure (can immediately re-lock and let the mapserver know)
                doneWaitingMapXferFailed(ent_con, errmsg);
            } else {
                // The map transfer should have gone through immediately and it should now be owned by a different map
                assert(ent_con->map_id == fix_map_id && ent_con->lock_id != dbLockIdFromLink(link));
            }
        }
    }
}

void doneWaitingMapXferFailed(EntCon *ent_con, char *errmsg)
{
    NetLink *link = ent_con->callback_link;
    if (link && !mpReclaimed(link)) {
        dbMemLog(__FUNCTION__, "MapXfer failed (%s) for %s:%d", errmsg, ent_con->ent_name, ent_con->id);
        assert(!ent_con->is_gameclient);
        sendLoginFailure(link,ent_con->is_gameclient,ent_con->id,errmsg); // Mapserver resume ownership here
        assert(!ent_con->is_gameclient);
        if (!ent_con->is_gameclient) {
            if (ent_con->lock_id == 0) {
                containerLock(ent_con,dbLockIdFromLink(link)); // mapserver regains ownership in this case
            } else {
                // The mapserver on which the entity is on should own the lock
                assert(ent_con->lock_id == dbLockIdFromLink(link));
            }
        }
    }
}

void doneWaitingMapXferSucceeded(EntCon *ent_con, int new_map_id)
{
    // The map has finished loading, let the mapserver the entity is on know he can hand off the
    // entity now.
    NetLink *link = ent_con->callback_link;
    Packet *pak;
    if (link && !mpReclaimed(link)) {
        dbMemLog(__FUNCTION__, "Sending DBSERVER_MAP_XFER_READY to old map %d for %s:%d moving to %d", ent_con->map_id, ent_con->ent_name, ent_con->id, new_map_id);
        assert(!ent_con->is_gameclient);
        pak = pktCreateEx(link, DBSERVER_MAP_XFER_READY);
        pktSendBitsPack(pak, 1, ent_con->id);
        pktSendBitsPack(pak, 1, new_map_id);
        pktSend(&pak, link);
    }
}

void handleContainerAck(Packet *pak)
{
    int        list_id,count,i,id,cookie;
    DbList    *list;
    EntCon    *container;

    list_id = pktGetBitsPack(pak,1);
    list = dbListPtr(list_id);
    count = pktGetBitsPack(pak,1);
    for(i=0;i<count;i++)
    {
        id = pktGetBitsPack(pak,1);
        container = containerPtr(list,id);
        cookie = pktGetBitsPack(pak,1);
        sendLoginInfoToGameClient(container,cookie);
        if (cookie==0)
        {
            tempLockRemovebyName(container->ent_name); //remove the name reserve
            deletePlayer(id,false,0,0,0);
        }
        else if(cookie==1)
            containerUnload(list,id);
    }
}

void handleMapXferTest(Packet *pak_in,NetLink *link)
{
    int list_id, map_id, container_id, count;
    Packet* pak;
    MapCon* map_con = NULL;
    EntCon* ent_con;

    list_id            = pktGetBitsPack(pak_in,1);
    map_id            = pktGetBitsPack(pak_in,1);
    count            = pktGetBitsPack(pak_in,1); // not used
    container_id    = pktGetBitsPack(pak_in,1);
    ent_con = containerPtr(dbListPtr(list_id),container_id);
    if (ent_con)
        map_con = containerPtr(dbListPtr(CONTAINER_MAPS),ent_con->map_id);
    if (!(map_con && map_con->active && map_con->link))
        return;

    pak = pktCreateEx(map_con->link,DBSERVER_TEST_MAP_XFER);
    pktSendBitsPack(pak,1,list_id);
    pktSendBitsPack(pak,1,map_id);
    pktSendBitsPack(pak,1,count);
    pktSendBitsPack(pak,1,container_id);
    pktSend(&pak,map_con->link);
    pktFree(pak);
}

void handleSaveLists(Packet *pak)
{
    containerListSave(doors_list);
}

void sendFail(NetLink *link,int errorlog,int fail_code,char const *fmt, ...)
{
    char *str = estrTemp();

    VA_START(ap, fmt);
    estrConcatfv(&str, fmt, ap);
    VA_END();

    LOG_OLD_ERR("sendfail(%d): %s",fail_code,str);

    if(link)
    {
        Packet    *pak = pktCreateEx(link,DBSERVER_CLIENT_CMD_FAILED);
        pktSendBitsPack(pak,1,fail_code);
        pktSendString(pak,str);
        pktSend(&pak,link);
    }

    estrDestroy(&str);
}

void handleAddDelGroupMembers(Packet *pak, NetLink *link)
{
    int i, list_id, add, id, count, notdiff, debug, spam=0, autolock;
    int* ptr = NULL;
    char* update;
    DbList* list;
    GroupCon* group;
    static int* members;
    static int max_members;

    list_id    = pktGetBitsPack(pak,1);
    add        = pktGetBitsPack(pak,1);
    id        = pktGetBitsPack(pak,1);
    notdiff    = pktGetBits(pak,1);
    debug    = pktGetBits(pak,1);
    strdup_alloca(update, pktGetString(pak));
    if(debug)
        pktGetString(pak); // just eat this out of the queue
    count    = pktGetBitsPack(pak,1);
    for(i = 0; i < count; i++)
    {
        ptr = dynArrayFit(&members,sizeof(members[0]),&max_members,i);
        *ptr = pktGetBitsPack(pak,1);
    }
    autolock = pktGetBool(pak);

    list = dbListPtr(list_id);
    // Special handling of supergroups.  In order to allow CS to join SG's that are not loaded (i.e. no members currently online), we 
    // containerLoad() supergroups rather than the usual containerPtr () call
    group = list_id == CONTAINER_SUPERGROUPS ? containerLoad(list, id) : containerPtr(list, id);

    if (group == NULL)
    {
        sendFail(link,1,CONTAINER_ERR_DOESNT_EXIST,"handleAddDelGroupMembers: %s member on non-existent group: %d.%d",add ? "add" : "del",list_id,id);
    }
    else if(add) // add does a lock operation if autolock is set
    {
        if (list_id == CONTAINER_MAPGROUPS) // DGNOTE debugging
        {
            LOG_OLD("CONTAINER_MAPGROUPS handleAddDelGroupMembers() - add: %d, id: %d, /diff: %d, update: '%s', count: %d (1), payload: %d, locked: %d, bywhom: %d, iam: %d",
                add, id, notdiff, update, count, ptr ? *ptr : -1, group->locked, group->lock_id, dbLockIdFromLink(link));
        }

        if(group->locked && group->lock_id != dbLockIdFromLink(link))
        {
            sendFail(link,1,CONTAINER_ERR_ALREADY_LOCKED,"handleAddDelGroupMembers: %d.%d is locked by another mapserver",list_id,id);
        }
        else if(autolock && group->locked)
        {
            sendFail(link,1,CONTAINER_ERR_ALREADY_LOCKED,"handleAddDelGroupMembers: %d.%d is already locked, but lock is requested",list_id,id);
        }
        else if(autolock && update[0])
        {
            sendFail(link,1,CONTAINER_ERR_NOT_LOCKED,"handleAddDelGroupMembers: %d.%d is not locked, but update is requested",list_id,id);
        }
        else
        {
            if(update[0])
                group = handleContainerUpdate(list_id, id, notdiff, update);

            for(i = 0; i < count; i++)
                containerAddMember(list, id, members[i], CONTAINERADDREM_FLAG_NOTIFYSERVERS | CONTAINERADDREM_FLAG_UPDATESQL);

            if(autolock)
                containerSendList(list, &id, 1, 1, link, 0);
            else if(group->locked)
                containerSendList(list, &id, 1, 0, link, 0);
            else
                containerBroadcast(list_id, id, group->member_ids, group->member_id_count, link);
        }
    }
    else // remove unlocks if autolock is set
    {
        if(autolock && !group->locked)
        {
            sendFail(link,1,CONTAINER_ERR_NOT_LOCKED,"handleAddDelGroupMembers: %d.%d is not locked",list_id,id);
        }
        else if(group->locked && group->lock_id != dbLockIdFromLink(link))
        {
            sendFail(link,1,CONTAINER_ERR_ALREADY_LOCKED,"handleAddDelGroupMembers: %d.%d was locked by another mapserver, but you tried to remove a member",list_id,id);
        }
        else
        {
            if(update[0])
                handleContainerUpdate(list_id, id, notdiff, update);

            for(i = 0; i < count; i++)
                containerRemoveMember(list, id, members[i], 0, CONTAINERADDREM_FLAG_UPDATESQL );

            group = containerPtr(list, id);
            if(group)
            {
                if(autolock)
                    containerUnlock(group, dbLockIdFromLink(link));

                if(group->locked)
                    containerSendList(list, &id, 1, 0, link, 0);
                else
                    containerBroadcast(list_id,id,group->member_ids,group->member_id_count,link);
            }
        }
    }
}

void handlePlayerDisconnect(Packet *pak,DBClientLink *client)
{
    int        plr_id,logout,logout_login;
    EntCon    *ent_con;

    plr_id = pktGetBitsPack(pak,1);
    logout = pktGetBitsPack(pak,1);
    logout_login = pktGetBitsPack(pak,2);

    ent_con = containerPtr(dbListPtr(CONTAINER_ENTS),plr_id);
    dbMemLog(__FUNCTION__, "Ent %i, lock was %i, logout %i", plr_id, ent_con? ent_con->lock_id: -1, logout);
    if (!ent_con)
    {
        LOG_OLD_ERR("Ent %d does not exist, aborting disconnect (request: %d)\n",plr_id,dbLockIdFromLink(client->link));
        return;
    }
    if (dbLockIdFromLink(client->link) != ent_con->lock_id)
    {
        LOG_OLD_ERR("aborting disconnect on player %d because another mapserver has it locked (request: %d; lock: %d)\n",plr_id,dbLockIdFromLink(client->link),ent_con->lock_id);
        return;
    }

     if (logout_login == 2 && server_cfg.queue_server)
     {
         addSingleAutoPass(ent_con->account);
     }

    if (ent_con->callback_link && ent_con->is_gameclient) 
    {
        // This character is currently in the queue trying to log in, this case happens if a relay/lock and load command was
        //  executed on him while he was waiting for his map to start up
        LOG_DEBUG( "handlePlayerDisconnect %d: just UNLOCKING (request: %d)",plr_id,dbLockIdFromLink(client->link));
        containerUnlock(ent_con, dbLockIdFromLink(client->link));
    } 
    else 
    {
        LOG_DEBUG( "handlePlayerDisconnect %d: unloading (request: %d)",plr_id,dbLockIdFromLink(client->link));
        sendPlayerLogout(plr_id, 4, (logout==0) || (logout_login==2));
    }
}

void handleReqEntNames(Packet *pak_in,NetLink *link)
{
    static int            *id_list,id_max;
    int                    i,list_id,count,ask_sql = 0;
    Packet                *pak;
    DbList                *list;
    char                *name;

    list_id = pktGetBitsPack(pak_in,1);
    count    = pktGetBitsPack(pak_in,1);
    list    = dbListPtr(list_id);
    dynArrayFit(&id_list,sizeof(id_list[0]),&id_max,count);

    pak = pktCreateEx(link,DBSERVER_SEND_ENT_NAMES);
    pktSendBitsPack(pak,1,list_id);
    pktSendBitsPack(pak,1,count);
    for(i=0;i<count;i++)
    {
        EntCatalogInfo* entInfo = NULL;
        id_list[i] = pktGetBitsPack(pak_in,1);
        // assert( id_list[i] != 0 ); // this should never happen, but we don't want the dbserver asserting on external agents

        name = pnameFindById( id_list[i] );
        entInfo = entCatalogGet( id_list[i], false );
        
        pktSendBitsPack(pak,1,id_list[i]);
        pktSendString(pak,name ? name : "empty");
        pktSendBitsPack(pak,3,SAFE_MEMBER(entInfo,gender));
        pktSendBitsPack(pak,3,SAFE_MEMBER(entInfo,nameGender));
        pktSendBitsPack(pak,1,SAFE_MEMBER(entInfo,playerType));
        pktSendBitsAuto(pak, SAFE_MEMBER(entInfo, playerSubType));
        pktSendBitsAuto(pak, SAFE_MEMBER(entInfo, playerTypeByLocation));
        pktSendBitsAuto(pak, SAFE_MEMBER(entInfo, praetorianProgress));
        pktSendBitsPack(pak,1,SAFE_MEMBER(entInfo,authId));
    }
    pktSend(&pak,link);
}

void handleReqGroupNames(Packet *pak_in,NetLink *link)
{
    static StuffBuff    sb;
    static int            *id_list,id_max;
    int                    i,list_id,count,ask_sql = 0;
    Packet                *pak;
    DbList                *list;
    char                *name;
    GroupCon*            container;

    list_id = pktGetBitsPack(pak_in,1);
    count    = pktGetBitsPack(pak_in,1);
    list    = dbListPtr(list_id);
    dynArrayFit(&id_list,sizeof(id_list[0]),&id_max,count);

    for(i=0;i<count;i++)
    {
        id_list[i] = pktGetBitsPack(pak_in,1);
        container = (GroupCon*)containerPtr(dbListPtr(list_id),id_list[i]);
        if (!container)
            ask_sql = 1;
    }
    if (ask_sql)
    {
        int            db_id,idx,row_count;
        char        *cols;
        ColumnInfo    *field_ptrs[4];

        if (!sb.size)
            initStuffBuff(&sb,10000);
        sb.idx = 0;
        addStringToStuffBuff(&sb,"WHERE ContainerId = %d\n",id_list[0]);
        for(i=1;i<count;i++)
            addStringToStuffBuff(&sb,"OR ContainerId = %d\n",id_list[i]);
        cols = sqlReadColumnsSlow(list->tplt->tables,0,"ContainerId, Name",sb.buff,&row_count,field_ptrs);

        pak = pktCreateEx(link, DBSERVER_SEND_GROUP_NAMES);
        pktSendBitsPack(pak,1,list_id);
        pktSendBitsPack(pak,1,row_count);
        for(idx=0,i=0;i<row_count;i++)
        {
            db_id = *(int *)(&cols[idx]);
            idx += field_ptrs[0]->num_bytes;
            name = &cols[idx];
            idx += field_ptrs[1]->num_bytes;
            pktSendBitsPack(pak,1,db_id);
            pktSendString(pak,name);
        }
        free(cols);
        pktSend(&pak,link);
    }
    else
    {
        pak = pktCreateEx(link,DBSERVER_SEND_GROUP_NAMES);
        pktSendBitsPack(pak,1,list_id);
        pktSendBitsPack(pak,1,count);
        for(i=0;i<count;i++)
        {
            container = (GroupCon*)containerPtr(dbListPtr(list_id),id_list[i]);
            pktSendBitsPack(pak,1,id_list[i]);
            pktSendString(pak,container->name);
        }
        pktSend(&pak,link);
    }
}

void handleContainerFindByElement(Packet *pak_in,NetLink *link)
{
    int            db_id,online_only,search_offline_ents;
    char        element[100],value[1000];
    Packet        *pak;
    int            id=0,got_id=0;

    db_id = pktGetBitsPack(pak_in,1);
    strcpy(element,pktGetString(pak_in));
    strcpy(value,pktGetString(pak_in));
    online_only = pktGetBitsPack(pak_in,1);
    search_offline_ents = pktGetBits(pak_in,1);
    if (db_id == CONTAINER_MAPS)
        id = containerVerifyMapId(dbListPtr(db_id),value);
    else
    {
        DbContainer    *c;

        if (stricmp(element,"ContainerId")==0)
        {
            c = containerPtr(dbListPtr(db_id),atoi(value));
            if (c)
                id = c->id;
            if (id > 0 || online_only)
                got_id = 1;
        }
        if (db_id == CONTAINER_ENTS && stricmp(element,"Name")==0)
        {
            id = pnameFindByName(value);
            if (id < 0)
            {
                id = containerIdFindByElement(dbListPtr(CONTAINER_ENTS),element,value);
                pnameAdd(value,id);
            }
            got_id = 1;
        }
        if ((db_id == CONTAINER_SUPERGROUPS || db_id == CONTAINER_MAPGROUPS) && stricmp(element,"Name")==0)
        {
            int            i;
            GroupCon    *team;
            DbList        *list = dbListPtr(db_id);

            for(i=0;i<list->num_alloced;i++)
            {
                team = (GroupCon *)list->containers[i];
                if (stricmp(team->name,value)==0)
                {
                    id = team->id;
                    got_id = 1;
                    break;
                }
            }
            if (id > 0 || online_only)
                got_id = 1;
        }
        if (!got_id)
            id = containerIdFindByElement(dbListPtr(db_id),element,value);
    }
    if (!id && db_id == CONTAINER_ENTS && search_offline_ents && stricmp(element,"Name")==0)
        id = offlinePlayerFindByName(value);
    pak = pktCreateEx(link,DBSERVER_CONTAINER_ID);
    pktSendBitsPack(pak,1,db_id);
    pktSendBitsPack(pak,1,id ? id : -1);
    if (id && db_id == CONTAINER_ENTS)
    {
        EntCon    *e = (EntCon*) containerPtr(ent_list,id);
        if (!e || !e->active)
            pktSendBitsPack(pak,1,-1);
        else
            pktSendBitsPack(pak,1,e->map_id);
    }
    else
        pktSendBitsPack(pak,1,-1);
    pktSend(&pak,link);
}

void sendMapserverShutdown(NetLink *link,int terminal, const char *msg, int mapID)
{
    Packet *pak;

    if (!link)
        return;
    pak = pktCreateEx(link,DBSERVER_SHUTDOWN);
    pktSendBitsPack(pak,1,terminal);
    pktSendString(pak,msg);
    pktSend(&pak,link);

    if (mapID)
        turnstileDBserver_handleCrashedIncarnateMap(mapID);
}

static int dbserver_shutting_down = 0;
static char *dbserver_shutting_down_msg = NULL;
static dbserver_req_quit_map_id = 0;

int dbIsShuttingDown(void)
{
    return dbserver_shutting_down;
}

void dbPrepShutdown(const char *msg, int req_quit_map_id)
{
    dbserver_shutting_down = 1;
    dbserver_shutting_down_msg = msg?strdup(msg): NULL;
    dbserver_req_quit_map_id = req_quit_map_id;
}

void dbShutdownTick()
{
    if(dbserver_shutting_down)
        dbShutdown(dbserver_shutting_down_msg,dbserver_req_quit_map_id);
}

void dbShutdown(const char *msg, int req_quit_map_id)
{
    int        i,still_waiting,timer;
    MapCon    *map_con, *req_map_con;
    F32        maxwait = 60;

    dbserver_shutting_down = 1;
    backupUnloadIndexFiles();

    req_map_con = containerPtr(map_list,req_quit_map_id);
    for(i=0;i<map_list->num_alloced;i++)
    {
        int turnstileCrashID = 0;
        map_con = (MapCon *)map_list->containers[i];
        if (!map_con->link || map_con == req_map_con)
            continue;
        if (map_con && !map_con->is_static)
        {
            if (map_con->mission_info && map_con->mission_info[0] == 'E')
            {
                turnstileCrashID = map_con->id;
            }
        }

        sendMapserverShutdown(map_con->link,1,msg, turnstileCrashID);
    }
    still_waiting = 1;
    timer = timerAlloc();
    timerStart(timer);
    while(still_waiting && timerElapsed(timer) < maxwait)
    {
        sqlFifoTick();
        sqlFifoDisableNMMonitorIfMemoryLow();
        NMMonitor(1000.f);
        still_waiting = 0;
        for(i=0;i<map_list->num_alloced;i++)
        {
            map_con = (MapCon *)map_list->containers[i];
            if (map_con->active && !map_con->edit_mode && map_con  != req_map_con)
                still_waiting++;
        }
        printf("waiting for mapservers to sync (timeout in %f seconds)\n",maxwait - timerElapsed(timer));
    }

    if (req_map_con) {
        int turnstileCrashID = 0;
        if (req_map_con && !req_map_con->is_static)
        {
            if (req_map_con->mission_info && req_map_con->mission_info[0] == 'E')
            {
                turnstileCrashID = req_map_con->id;
            }
        }
        sendMapserverShutdown(req_map_con->link,1,msg, turnstileCrashID);
        timerStart(timer);
        while(timerElapsed(timer) < maxwait)
        {
            sqlFifoTick();
            sqlFifoDisableNMMonitorIfMemoryLow();
            NMMonitor(1.f);
            map_con = containerPtr(map_list,req_quit_map_id);
            if (!map_con || map_con->edit_mode || !map_con->active)
                break;
        }
    }

    sqlFifoShutdown();
    sqlConnShutdown();
    logShutdownAndWait();
    exit(0);
}

static void handleShutdown(Packet *pak_in,NetLink *link,DBClientLink *client)
{
    char    *msg;
    printf("Shutdown command sent from: %s\n",makeIpStr(link->addr.sin_addr.S_un.S_addr));
    msg = pktGetString(pak_in);
    dbPrepShutdown(msg, client->map_id);
}


#define ANON_OFFSET        15
#define HIDDEN_OFFSET    16

void handleReqAllOnline(Packet *pak_in, NetLink *link)
{
    int        i;
    int arena_map;
    int mission_map;
    int coed_faction_map;
    int coed_universe_map;
    int ent_updates;
    int dbid;
    int faction_same_map;
    int universe_same_map;
    EntCon    *e;
    static Packet *cached_pak = 0;
    Packet *pak;
    static void * cached_zipped_stream = 0;
    static int zipsize = 0;

    arena_map = pktGetBitsPack(pak_in, 1);
    mission_map = pktGetBitsPack(pak_in, 1);
    coed_faction_map = pktGetBitsPack(pak_in, 1);
    coed_universe_map = pktGetBitsPack(pak_in, 1);
    ent_updates = pktGetBitsAuto(pak_in);

    for( i = 0; i < ent_updates; i++ )
    {
        dbid = pktGetBitsAuto(pak_in);
        faction_same_map = pktGetBitsPack(pak_in, 1);
        universe_same_map = pktGetBitsPack(pak_in, 1);

        e = (EntCon*)containerPtr(dbListPtr(CONTAINER_ENTS), dbid);

        if( e )
        {
            e->arena_map = arena_map;
            e->mission_map = mission_map;
            e->can_co_faction = coed_faction_map;
            e->can_co_universe = coed_universe_map;
            e->faction_same_map = faction_same_map;
            e->universe_same_map = universe_same_map;
        }
    }

    if( !cached_pak || // check sto see if we want to create a new cached_pak
        cached_pak->hasDebugInfo != pktGetDebugInfo() ||
        timerCpuSeconds() - cached_pak->creationTime >= 5 )
    {
        if( cached_pak )
        {
            pktFree(cached_pak);
            cached_pak = 0;
        }

        SAFE_FREE(cached_zipped_stream );

        cached_pak = pktCreate();
        pktSetByteAlignment(cached_pak,true);

        // Since we use 2 or 3 bits for these quantities we need to know if
        // they grow past 4 or 8 valid values!
        STATIC_INFUNC_ASSERT(kPlayerType_Count <= 4);
        STATIC_INFUNC_ASSERT(kPlayerSubType_Count <= 4);
        STATIC_INFUNC_ASSERT(kPraetorianProgress_Count <= 8);

        for(i=0;i<ent_list->num_alloced;i++)
        {
            e = (EntCon*)ent_list->containers[i];
            if (e->connected) { // Don't send players who aren't logged in!

                GroupCon *team = containerPtr(dbListPtr(CONTAINER_TEAMUPS),e->teamup_id);
                GroupCon *league = containerPtr(dbListPtr(CONTAINER_LEAGUES),e->league_id);
                pktSendBitsPack(cached_pak,20,e->id);
                pktSendBitsPack(cached_pak,8,e->map_id);
                pktSendBitsPack(cached_pak,8,e->static_map_id);

                if( team )
                {
                    pktSendBitsPack(cached_pak,4,team->member_id_count);
                    pktSendBitsPack(cached_pak,1,team->leader_id == e->id);
                }
                else
                {
                    pktSendBitsPack(cached_pak,4,0);
                    pktSendBitsPack(cached_pak,1,0);
                }

                pktSendBitsPack(cached_pak,10,e->hidefield);
                pktSendBitsPack(cached_pak,10,e->lfgfield);
                pktSendBitsPack(cached_pak,6,e->level);

                pktSendString(cached_pak,e->archetype);
                pktSendString(cached_pak,e->origin);
                pktSendBitsPack(cached_pak,2,e->playerType);
                pktSendBitsPack(cached_pak,2,e->playerTypeByLocation);
                pktSendBitsPack(cached_pak,2,e->playerSubType);
                pktSendBitsPack(cached_pak,3,e->praetorianProgress);
                pktSendBitsPack(cached_pak,1,e->arena_map);
                pktSendBitsPack(cached_pak,1,e->mission_map);
                pktSendBitsPack(cached_pak,1,e->can_co_faction);
                pktSendBitsPack(cached_pak,1,e->faction_same_map);
                pktSendBitsPack(cached_pak,1,e->can_co_universe);
                pktSendBitsPack(cached_pak,1,e->universe_same_map);
                pktSendBits(cached_pak,32,e->last_active);
                pktSendBits(cached_pak,32,e->member_since);
                pktSendBits(cached_pak,32,e->prestige);
                if (league)
                {
                    pktSendBitsPack(cached_pak,8,league->member_id_count);
                    pktSendBitsPack(cached_pak,1,league->leader_id == e->id);
                }
                else
                {
                    pktSendBitsPack(cached_pak,8,0);
                    pktSendBitsPack(cached_pak,1,0);
                }
            }
        }
        pktSendBitsPack(cached_pak,20,0);

        cached_zipped_stream = zipData(cached_pak->stream.data, cached_pak->stream.size, &zipsize); 

    }

    pak = pktCreateEx(link,DBSERVER_ONLINE_ENTS);
    pktSendBits(pak, 32, cached_pak->stream.bitLength );
    pktSendBits(pak, 32, cached_pak->stream.size );
    pktSendZippedAlready(pak, cached_pak->stream.size, zipsize, cached_zipped_stream);
    pktSend(&pak,link);
}

void handleReqAllOnlineComments(NetLink *link)
{
    int        i;
    EntCon    *e;
    static Packet *cached_pak = 0;
    Packet *pak;

    if( !cached_pak || // check sto see if we want to create a new cached_pak
        cached_pak->hasDebugInfo != pktGetDebugInfo() ||
        timerCpuSeconds() - cached_pak->creationTime >= 5 )
    {
        if( cached_pak )
            pktFree(cached_pak);

        cached_pak = pktCreate();

        for(i=0;i<ent_list->num_alloced;i++)
        {
            e = (EntCon*)ent_list->containers[i];
            if (e->connected && e->comment) { // Don't send players who aren't logged in!
                pktSendBitsPack(cached_pak,20,e->id); // send id with string for matching
                pktSendString(cached_pak,e->comment);
            }
        }
        pktSendBitsPack(cached_pak,20,0);
    }

    pak = pktCreateEx(link,DBSERVER_ONLINE_ENT_COMMENTS);
    pktAppend(pak,cached_pak);
    pktSend(&pak,link);
}

void handleReqSomeOnlineEnts(Packet *pak, NetLink *link)
{
    U32 userdata = pktGetBitsPack(pak,1);
    int count = pktGetBitsAuto(pak);
    int i;
    Packet *pakRes = pktCreateEx(link, DBSERVER_SOME_ONLINE_ENTS);

    pktSendBitsPack( pakRes, 1, userdata );
    pktSendBitsAuto( pakRes, count );
    for( i = 0; i < count; ++i ) 
    {
        int idEnt = pktGetBitsAuto(pak);
        EntCon *e = (EntCon*)containerPtr(ent_list, idEnt);
        pktSendBitsAuto( pakRes, idEnt );
        pktSendBitsPack( pakRes, 1, (e && e->connected)?1:0 );
    }
    pktSend(&pakRes,link);
}

void handleOverrrideStartZone(Packet *pak)
{
    start_zone_override = pktGetBitsPack(pak,1);
}



void handlePlayerKicked(Packet *pak)
{
    int        player_id,ban;
    EntCon    *ent;

    player_id    = pktGetBitsPack(pak,1);
    ban            = pktGetBitsPack(pak,1);

    ent = containerPtr(dbListPtr(CONTAINER_ENTS),player_id);
    if (!ent)
        return;
    if (!ban)
        authSendKickAccount(ent->auth_id,0);
    else
        authSendBanAccount(ent->auth_id,0);
}

void handlePlayerRename(Packet *pak, NetLink *link)
{
    int callback_id, player_id;
    char *name;
    EntCon* container;
    char cmd_buf[500];
    char escapedName[128];
    bool change_case = false;

    callback_id        = pktGetBitsPack(pak,1);
    player_id        = pktGetBitsPack(pak,1);
    name            = pktGetString(pak);

    container = (EntCon*)containerPtr(dbListPtr(CONTAINER_ENTS), player_id);

    if (!container)
    {
        sendFail(link, 1, CONTAINER_ERR_DOESNT_EXIST, "handlePlayerRename(%i): dbid doesn't exist", player_id);
        return;
    }
    strncpy(escapedName, escapeString(name), sizeof(escapedName));
    escapedName[sizeof(escapedName)-1] = 0;

    // Allow for 'same' name changes only on the same character. This
    // lets people change the case of the name or set the name to the
    // same thing to eat up rename tokens (in case they have one they
    // don't want for some reason).
    if(stricmp(escapedName, container->ent_name) == 0)
    {
        change_case = true;
    }

    if(!change_case && !playerNameValid(escapedName, NULL, container->auth_id))
    {
        sendFail(link, 0, CONTAINER_ERR_CANT_COMPLETE, "handlePlayerRename(%i): %s already exists", player_id, name);
        return;
    }

    // update caches on rename
    playerNameDelete(container->ent_name, player_id);
    playerNameClearCacheEntry(container->ent_name, player_id);
    playerNameCreate(escapedName,player_id, container->auth_id);

    // do container update
    strncpy(container->ent_name, escapedName, sizeof(container->ent_name));
    container->ent_name[sizeof(container->ent_name)-1] = 0;
    sprintf(cmd_buf, "Name \"%s\"\n", escapedName);
    containerUpdate(dbListPtr(CONTAINER_ENTS),player_id,cmd_buf,1);

    sendContainerAcks(link, CONTAINER_ENTS, callback_id, 1, &player_id);
}

void handlePlayerUnlock(Packet *pak, NetLink *link)
{
    int callback_id, player_id;
    EntCon *ent_con;
    ShardAccountCon *shardaccount_con;

    callback_id        = pktGetBitsPack(pak,1);
    player_id        = pktGetBitsPack(pak,1);

    ent_con = (EntCon*)containerPtr(ent_list, player_id);
    if (!ent_con)
    {
        sendFail(link, 1, CONTAINER_ERR_DOESNT_EXIST, "handlePlayerUnlock(%i): dbid doesn't exist", player_id);
        return;
    }

    shardaccount_con = (ShardAccountCon*)containerPtr(shardaccounts_list, ent_con->auth_id);

    // do container updates
    containerUpdate(ent_list, player_id, "IsSlotLocked 0\n", 1);
    containerUpdate(shardaccounts_list, shardaccount_con->id, "ShowPremiumSlotLockNag 0\n", 1);

    sendContainerAcks(link, CONTAINER_ENTS, callback_id, 1, &player_id);
}

void handlePlayerAdjustServerSlots(Packet *pak, NetLink *link)
{
    int callback_id;
    U32 auth_id;
    int delta;
    ShardAccountCon *shardaccount_con;
    int new_count;
    char *estr = estrTemp();

    callback_id        = pktGetBitsPack(pak,1);
    auth_id            = pktGetBitsAuto(pak);
    delta            = pktGetBitsAuto(pak);

    shardaccount_con = (ShardAccountCon*)containerLoad(shardaccounts_list, auth_id);
    if (!shardaccount_con)
    {
        sendFail(link, 1, CONTAINER_ERR_DOESNT_EXIST, "handlePlayerAdjustServerSlots(%i): authid doesn't exist", auth_id);
        return;
    }

    // do container update
    new_count = max(shardaccount_con->slot_count + delta, 0);
    estrPrintf(&estr, "SlotCount %d\n", new_count);
    containerUpdate(shardaccounts_list, shardaccount_con->id, estr, 1);
    estrDestroy(&estr);
    LOG(LOG_CHAR_SLOT_APPLY,
        LOG_LEVEL_IMPORTANT,
        0,
        "\"%s\" (%d) slot applied (CSR). current: server %d\n",
        shardaccount_con->account,
        auth_id,
        new_count);

    sendContainerAcks(link, CONTAINER_SHARDACCOUNT, callback_id, 1, &auth_id);
}

void handlePlayerChangeType(Packet *pak, NetLink *link)
{
    int callback_id, player_id, playerType;
    EntCon* container;
    char cmd_buf[500];

    callback_id        = pktGetBitsPack(pak,1);
    player_id        = pktGetBitsPack(pak,1);
    playerType            = pktGetBitsPack(pak,1);

    container = (EntCon*)containerPtr(dbListPtr(CONTAINER_ENTS), player_id);

    if (!container)
    {
        sendFail(link, 1, CONTAINER_ERR_DOESNT_EXIST, "handleChangePlayerType(%i): dbid doesn't exist", player_id);
    }

    // update caches on rename
    playerNameClearCacheEntry(container->ent_name, player_id);

    // do container update
    container->playerType = playerType;
    container->ent_name[sizeof(container->ent_name)-1] = 0;
    sprintf(cmd_buf, "PlayerType %d\n", playerType);
    containerUpdate(dbListPtr(CONTAINER_ENTS),player_id,cmd_buf,1);
    sendContainerAcks(link, CONTAINER_ENTS, callback_id, 1, &player_id);
}

void handlePlayerChangeSubType(Packet *pak, NetLink *link)
{
    int callback_id, player_id, playerSubType;
    EntCon* container;
    char cmd_buf[500];

    callback_id        = pktGetBitsPack(pak,1);
    player_id        = pktGetBitsPack(pak,1);
    playerSubType    = pktGetBitsPack(pak,1);

    container = (EntCon*)containerPtr(dbListPtr(CONTAINER_ENTS), player_id);

    if (!container)
    {
        sendFail(link, 1, CONTAINER_ERR_DOESNT_EXIST, "handleChangePlayerType(%i): dbid doesn't exist", player_id);
    }

    // update caches on rename
    playerNameClearCacheEntry(container->ent_name, player_id);

    // do container update
    container->playerSubType = playerSubType;
    container->ent_name[sizeof(container->ent_name)-1] = 0;
    sprintf(cmd_buf, "PlayerSubType %d\n", playerSubType);
    containerUpdate(dbListPtr(CONTAINER_ENTS),player_id,cmd_buf,1);
    sendContainerAcks(link, CONTAINER_ENTS, callback_id, 1, &player_id);
}

void handlePlayerChangePraetorianProgress(Packet *pak, NetLink *link)
{
    int callback_id, player_id, praetorianProgress;
    EntCon* container;
    char cmd_buf[500];

    callback_id            = pktGetBitsPack(pak,1);
    player_id            = pktGetBitsPack(pak,1);
    praetorianProgress    = pktGetBitsPack(pak,1);

    container = (EntCon*)containerPtr(dbListPtr(CONTAINER_ENTS), player_id);

    if (!container)
    {
        sendFail(link, 1, CONTAINER_ERR_DOESNT_EXIST, "handleChangePraetorianProgress(%i): dbid doesn't exist", player_id);
    }

    // update caches on rename
    playerNameClearCacheEntry(container->ent_name, player_id);

    // do container update
    container->praetorianProgress = praetorianProgress;
    container->ent_name[sizeof(container->ent_name)-1] = 0;
    sprintf(cmd_buf, "PraetorianProgress %d\n", praetorianProgress);
    containerUpdate(dbListPtr(CONTAINER_ENTS),player_id,cmd_buf,1);
    sendContainerAcks(link, CONTAINER_ENTS, callback_id, 1, &player_id);
}

void handlePlayerChangeInfluenceType(Packet *pak, NetLink *link)
{
    int callback_id, player_id, influenceType;
    EntCon* container;
    char cmd_buf[500];

    callback_id            = pktGetBitsPack(pak,1);
    player_id            = pktGetBitsPack(pak,1);
    influenceType        = pktGetBitsPack(pak,1);

    container = (EntCon*)containerPtr(dbListPtr(CONTAINER_ENTS), player_id);

    if (!container)
    {
        sendFail(link, 1, CONTAINER_ERR_DOESNT_EXIST, "handleChangeInfluenceType(%i): dbid doesn't exist", player_id);
    }

    // update caches on rename
    playerNameClearCacheEntry(container->ent_name, player_id);

    // do container update
    container->playerTypeByLocation = influenceType;
    container->ent_name[sizeof(container->ent_name)-1] = 0;
    sprintf(cmd_buf, "InfluenceType %d\n", influenceType);
    containerUpdate(dbListPtr(CONTAINER_ENTS),player_id,cmd_buf,1);
    sendContainerAcks(link, CONTAINER_ENTS, callback_id, 1, &player_id);
}

void handleRenameResponse(Packet *pak)
{
    char *authname;
    Packet *pak_out;
    NetLink *client_link;
    GameClientLink *client = 0;

    authname = pktGetString(pak);
    client_link = linkFromAuthName(authname, &client);

    if (client_link)
    {
        pak_out = dbClient_createPacket(DBGAMESERVER_RENAME_RESPONSE, client);
        pktSendGetString(pak_out, pak);
        pktSend(&pak_out, client_link);
    }
}

void handleCheckNameResponse(Packet *pak)
{
    char name[32], *result;
    Packet *pak_out;
    NetLink *client_link;
    GameClientLink *client = 0;
    int CheckNameSuccess;
    int authId;

    authId = pktGetBitsAuto(pak);
    client_link = linkFromAuthId(authId, &client);
    result = pktGetString(pak);
    Strcpy(name, result);
    CheckNameSuccess = pktGetBitsAuto(pak);

    if (client_link)
    {
        int tempLockName = pktGetBitsAuto(pak);
        if (tempLockName && CheckNameSuccess)
        {
            U32 time = timerSecondsSince2000();
            //add name to reserve list
            tempLockAdd(name, authId, time);
        }
        else
        {
            //name not reserved
            tempLockName = 0;
        }

        pak_out = dbClient_createPacket(DBGAMESERVER_CHECK_NAME_RESPONSE, client);
        pktSendBitsAuto(pak_out, CheckNameSuccess);
        pktSendBitsAuto(pak_out, tempLockName);
        pktSend(&pak_out, client_link);
    }
    else
    {
        //consume remaining
        pktGetBitsAuto(pak);
    }

}

void handleCharSlotGrant(Packet *pak_in)
{
    int authId = pktGetBitsAuto(pak_in);
    int delta = pktGetBitsAuto(pak_in);
    char diff[30];
    ShardAccountCon *container = containerLoad(shardaccounts_list, authId);
    sprintf(diff, "SlotCount %d", max(container->slot_count + delta, 0));
    containerUpdate(shardaccounts_list, authId, diff, 1);
}

void handleUnlockCharacterResponse(Packet *pak_in)
{
    char *authname;
    Packet *pak_out;
    NetLink *client_link;
    GameClientLink *client = 0;

    authname = pktGetString(pak_in);
    client_link = linkFromAuthName(authname, &client);

    if (client_link)
    {
        pak_out = dbClient_createPacket(DBGAMESERVER_UNLOCK_CHARACTER_RESPONSE, client);
        pktSend(&pak_out, client_link);
    }
}

static void onVipUpdateDebug(VipSqlCbData *data)
{
    ShardAccountCon *shardaccount_con = data->shardaccount_con;
    // request account inventory from account server
    NetLink *acc_link = accountLink();
    if (acc_link)
    {
        Packet *pak_out = pktCreateEx(acc_link,ACCOUNT_CLIENT_INVENTORY_REQUEST);
        pktSendBitsAuto(pak_out, shardaccount_con->id);
        pktSend(&pak_out, acc_link);
    }

    // send cached version to client
    accountInventoryRelayToClient(shardaccount_con->id, 0, 0, 0, 0, 0, false);
}

static void handleDebugSetVIP(Packet *pak_in)
{
    int authId = pktGetBitsAuto(pak_in);
    int vip = pktGetBitsAuto(pak_in);
    ShardAccountCon *container = containerLoad(shardaccounts_list, authId);

    if (container && server_cfg.fake_auth)
    {
        NetLink *acc_link = accountLink();

        container->accountStatusFlags = vip;
        updateVip(onVipUpdateDebug, NULL, container, NULL);
    }
}

static void sqlReqCustomDataCallback(Packet *pak_out,U8 *cols,int row_count,ColumnInfo **field_ptrs,void *foo)
{
    int        i,j,col_count;
    int        idx;

    if (field_ptrs)
    {
        for(col_count=0;field_ptrs[col_count];col_count++)
            ;
    }
    pktSendBitsPack(pak_out,1,row_count);
    for(idx=0,i=0;i<row_count;i++)
    {
        for(j=0;j<col_count;j++)
        {
            // Is the field an attribute?
            if(field_ptrs[j]->attr)
            {
                // If so, lookup the attribute and send it back as a string.
                if (field_ptrs[j]->idx_by_attr)
                {
                    assert(0 && "Unsupported custom data request type!");
                    // JS: No idea what to do to return something reasonable here.
                }
                else
                {
                    int iIdx = *(int *)(cols+idx);
                    assert(iIdx >= 0);
                    if (!iIdx)
                        pktSendString(pak_out,"");
                    else
                        pktSendString(pak_out,field_ptrs[j]->attr->names[iIdx]);
                }
            }
            else
            {
                // Otherwise, it might be one of these other primitives.
                switch(field_ptrs[j]->data_type)
                {
                    xcase CFTYPE_UNICODESTRING:
                        utf16ToUtf8(&cols[idx], field_ptrs[j]->num_bytes);
                        pktSendString(pak_out,&cols[idx]);
                    xcase CFTYPE_ANSISTRING:
                    case CFTYPE_TEXTBLOB:
                        pktSendString(pak_out,&cols[idx]);
                    xcase CFTYPE_BINARY_MAX:
                    case CFTYPE_UNICODESTRING_MAX:
                    case CFTYPE_ANSISTRING_MAX:
                        assert(0); // there isn't a clean way to do this right now
                    xcase CFTYPE_INT:
                        pktSendBitsPack(pak_out,1,*(int *)(&cols[idx]));
                    xcase CFTYPE_SHORT:
                        pktSendBitsPack(pak_out,1,*(U16 *)(&cols[idx]));
                    xcase CFTYPE_BYTE:
                        pktSendBitsPack(pak_out,1,*(U8 *)(&cols[idx]));
                    xcase CFTYPE_FLOAT:
                        pktSendF32(pak_out,*(F32 *)(&cols[idx]));
                    xcase CFTYPE_DATETIME:
                    {
                        U32        time;
                        char    datestr[100];
                        struct    { short    year,month,day,hour,minute,second,millisecond; } *t = (void *)(&cols[idx]);

                        sprintf(datestr,"%04d-%02d-%02d %02d:%02d:%02d",t->year,t->month,t->day,t->hour,t->minute,t->second);
                        time = timerGetSecondsSince2000FromString(datestr);
                        pktSendBits(pak_out,32,time);
                    }
                }
            }
            idx += field_ptrs[j]->num_bytes;
        }
    }
    pktSend(&pak_out,pak_out->link);
}


void handleReqCustomData(Packet *pak,NetLink *link)
{
    char        *columns,*table_name,*restrict,*limit;
    int            list_type;
    DbList        *db_list;
    TableInfo    *table;
    U32            cb,db_id;

    cb            = pktGetBits(pak,32);
    db_id        = pktGetBitsPack(pak,1);
    strdup_alloca(limit,pktGetString(pak));
    strdup_alloca(restrict,pktGetString(pak));
    strdup_alloca(columns,pktGetString(pak));
    list_type    = pktGetBitsPack(pak,1);
    strdup_alloca(table_name,pktGetString(pak));

    db_list = dbListPtr(list_type);
    table = tpltFindTable(db_list->tplt,table_name);

    if(table)
    {
        Packet        *pak_out = pktCreateEx(link,DBSERVER_CUSTOM_DATA);

        pktSendBits(pak_out,32,cb);
        pktSendBitsPack(pak_out,1,db_id);

        // If no db_id was specified, assume that we need to block for all outstanding writes to complete
        if (!db_id)
            sqlFifoBarrier();

        sqlReadColumnsAsync(table,limit,columns,restrict,sqlReqCustomDataCallback,0,pak_out,db_id);
    }
}

void handleBaseDestroy(Packet *pak, NetLink *link)
{
    int i, container_id;
    int user_id    = pktGetBits(pak,32);
    int    sg_id = pktGetBits(pak,32);
    char * mapinfo = pktGetString(pak);
    DbContainer * container;

    char cmd[SHORT_SQL_STRING];
    int cmd_len;

    // if this base map is open, delink it now.
    for (i = 0; i < map_list->num_alloced; i++)
    {
        MapCon* map_con = (MapCon*)map_list->containers[i];
        if (map_con && stricmp(map_con->mission_info, mapinfo) == 0)
            dbDelinkMapserver(map_con->id);
    }

    // now destroy the container
    cmd_len = sprintf(cmd, "SELECT ContainerId FROM Base WHERE ISNULL(SupergroupId,0) = %d AND ISNULL(UserId,0) = %d;", sg_id, user_id);
    container_id = sqlGetSingleValue(cmd, cmd_len, NULL, SQLCONN_FOREGROUND);

    container = containerPtr(dbListPtr(CONTAINER_BASE), container_id);
        
    if (!container)
        container = containerLoad(dbListPtr(CONTAINER_BASE), container_id);
        
    if (container)
        containerDelete(container);

}

void handleMissionPlayerCountRequest(Packet *pak, NetLink *link)
{
    int i, count = 0;
    char * mapinfo = pktGetString(pak);
    Packet* pak_out;

    // if this base map is open, delink it now.
    for (i = 0; i < map_list->num_alloced; i++)
    {
        MapCon* map_con = (MapCon*)map_list->containers[i];
        if (map_con && !stricmp(map_con->mission_info, mapinfo))
            count = map_con->num_players + map_con->num_players_connecting;
    }
    pak_out = pktCreateEx(link,DBSERVER_MISSION_PLAYER_COUNT);
    pktSendBitsPack(pak_out, 1, count);
    pktSend(&pak_out,link);
}

void handleSendStatserverCmd(Packet *pak, NetLink *link)
{
    Packet* pak_out = NULL;
    NetLink *linkStat = statLink();
    int idEntSrc = pktGetBitsPack(pak, 1);
    int idSgrp = pktGetBitsPack(pak,1);
    char *cmd = pktGetString(pak);

    if( linkStat )
    {
        pak_out = pktCreateEx(linkStat,DBSERVER_SEND_STATSERVER_CMD);
        if( pak_out )
        {
            pktSendBitsPack(pak_out,1,idEntSrc);
            pktSendBitsPack(pak_out,1, idSgrp);
            pktSendString(pak_out, cmd);

            pktSend(&pak_out, linkStat);
        }
    }
    else if(idEntSrc > 0)
    {
        char response[512] = {0};
        if(strStartsWith(cmd, "statserver_levelingpact"))
        {
            //channel, message, params
            sprintf( response, "cmdrelay_dbid %d\n" 
                "svr_sgstat_localized_message %d \"%s\" \"%s\"", idEntSrc, INFO_USER_ERROR, "LevelingPactStatDown", " ");
        }
        else
        {
            sprintf( response, "server_sgstat_cmd %d\n" 
                "svr_sgstat_conprintf_response %d \"statserver not connected\"", idEntSrc, idEntSrc);
        }
        sendToEnt(link->userData, idEntSrc, false, response);
    }
}

void handleRequestShutdown(Packet* pak, NetLink* link)
{
    DBClientLink    *client = link->userData;
    MapCon            *map_con;

    // if this map doesn't have any players waiting to enter, delink it
    map_con = containerPtr(dbListPtr(CONTAINER_MAPS),client->map_id);
    if (!map_con || map_con->link != link)
    {
        LOG_OLD_ERR( "handleRequestShutdown from map id %i (invalid id)",client->map_id);
        return;
    }
    if (map_con->num_players + map_con->num_players_connecting + map_con->num_players_waiting_for_start == 0)
    {
        int turnstileCrashID = 0;
        if (map_con && !map_con->is_static)
        {
            if (map_con->mission_info && map_con->mission_info[0] == 'E')
            {
                turnstileCrashID = map_con->id;
            }
        }
        sendMapserverShutdown(map_con->link,1,"MapShuttingDown", turnstileCrashID);
        return;
    }
    // otherwise, ignore the request.  there is a player logging in now
}

// Attempt to shut down a map "gracefully" if an admin needs to close a specific map
void handleEmergencyShutdown(Packet *pak, NetLink *link)
{
    char *msg = pktGetString(pak);
    sendMapserverShutdown(link,1,msg, 0);
}

void handleOfflineChar(Packet *pak, NetLink *link)
{
    int db_id;
    const char* msg = "Character moved offline.";
    S64 requestor;
    pktGetBitsArray(pak,(sizeof(requestor)*8),&requestor);
    db_id = pktGetBitsPack(pak,1);

    if ( ! offlinePlayerMoveOffline(db_id) )
    {
        msg = "Error moving character offline.";
    }

    // Send reply packet
    {
        Packet    *pak_out = pktCreateEx(link,DBSERVER_OFFLINE_CHARACTER_RSLT);
        pktSendBitsArray(pak_out,(sizeof(requestor)*8),&requestor);
        pktSendString( pak_out, msg );
        pktSend(&pak_out,link);
    }
}

void handleRestoreDeletedChar(Packet *pak, NetLink *link)
{
    // pktGetString() seems to return a static buffer, so if you call it
    // twice you'll get two copies of the same pointer = the last string
    // de-serialized from the packet... Thus, the copying to local buffers.
    int auth_id;
    tOfflineRestoreResult rslt;
    S64 requestor;
    char authName[80];
    char charName[80];
    char deletionDate[32];
    int seqNbr = -1;
    pktGetBitsArray(pak,(sizeof(requestor)*8),&requestor);
    auth_id = pktGetBitsPack(pak,1);
    strcpy_s( authName, ARRAY_SIZE(authName), pktGetString(pak) ); 
    strcpy_s( charName, ARRAY_SIZE(charName), pktGetString(pak) );
    seqNbr = pktGetBitsPack(pak,1);
    strcpy_s( deletionDate, ARRAY_SIZE(deletionDate), pktGetString(pak) ); 
    
    rslt = offlinePlayerRestoreDeleted(auth_id,authName,charName,seqNbr,deletionDate);

    // Send reply packet
    {
        const char* szRsltStrFmt;    // contains one %s sequences for the charname
        char msg[256];
        Packet    *pak_out = pktCreateEx(link,DBSERVER_OFFLINE_RESTORE_RSLT);
        switch ( rslt )
        {
            case kOfflineRestore_SUCCESS :                
                    szRsltStrFmt = "Deleted character %s restored."; break;
            case kOfflineRestore_PLAYER_NOT_FOUND :        
                    szRsltStrFmt = "%s not found! Restore failed."; break;
            case kOfflineRestore_DATA_FILE_UNKNOWN :    
                    szRsltStrFmt = "Unknown data file for %s. Restore failed."; break;
            case kOfflineRestore_NO_MATCHING_DATA :        
                    szRsltStrFmt = "No matching data for %s. Restore failed."; break;
            default:
                    szRsltStrFmt = "UNKNOWN ERROR restoring %s."; break;
        }
        pktSendBitsArray(pak_out,(sizeof(requestor)*8),&requestor);
        sprintf_s( msg, ARRAY_SIZE(msg), szRsltStrFmt, charName );
        pktSendString( pak_out, msg );
        pktSend(&pak_out,link);
    }
}

void handleListDeletedChars(Packet *pak, NetLink *link)
{
    static const int MAX_DELETED_CHAR_BATCH = 50;
    int                i;
    OfflinePlayer    *deletedPlayers,*p;
    S64                requestor;
    int                auth_id;
    char            auth_name[80];
    char            deletionDate[16];
    int                numDeleted;
    Packet*            pak_out = pktCreateEx(link,DBSERVER_SEND_DELETED_CHARS);

    pktGetBitsArray(pak,(sizeof(requestor)*8),&requestor);
    auth_id        = (int)pktGetBitsPack(pak,1);
    strncpy_s( auth_name, ARRAY_SIZE(auth_name), pktGetString(pak), _TRUNCATE );
    strncpy_s( deletionDate, ARRAY_SIZE(deletionDate), pktGetString(pak), _TRUNCATE );
    
    p = deletedPlayers = _alloca(MAX_DELETED_CHAR_BATCH * sizeof(deletedPlayers[0]));
    memset(deletedPlayers,0,MAX_DELETED_CHAR_BATCH * sizeof(deletedPlayers[0]));
    offlinePlayerListDeleted(auth_id,auth_name,deletionDate,deletedPlayers,MAX_DELETED_CHAR_BATCH,&numDeleted,&auth_id);
    // requestor
    pktSendBitsArray(pak_out,(sizeof(requestor)*8),&requestor);
    // auth id and name
    pktSendBitsPack(pak_out,1,auth_id);
    pktSendString(pak_out,auth_name);
    // num deleted
    pktSendBitsPack(pak_out,1,numDeleted);
    for(i=0;i<numDeleted;i++,p++)
    {
        pktSendString(pak_out,p->name);
        pktSendBitsPack(pak_out,1,p->dateStored);
        pktSendBitsPack(pak_out,1,p->level);
        pktSendBitsPack(pak_out,1,p->archetype);
    }
    pktSend(&pak_out, link);
}

void handleReqSgChannelInvite(Packet *pak,NetLink *link)
{
    int            auth_id, db_id, sg_id, min_rank;
    char        *channel;
    DbList        *db_list;
    TableInfo    *table;

    auth_id        = pktGetBitsPack(pak,1);
    db_id        = pktGetBitsPack(pak,1);
    sg_id        = pktGetBitsPack(pak,1);
    min_rank    = pktGetBitsPack(pak, 1);
    channel        = strdup(pktGetString(pak));

    db_list        = dbListPtr(CONTAINER_ENTS);
    table        = tpltFindTable(db_list->tplt,"Ents");

    if(table)
    {
        char restrict[200];
        Packet    *pak_out = pktCreateEx(link,DBSERVER_SG_CHANNEL_INVITE);

        sprintf(restrict,"Where SuperGroupsId = %d and ContainerId != %d",sg_id,db_id);
        
        // if rank is 0, then actual value in DB is "NULL", which doesn't work well with '>='
        if(min_rank > 0)    
            strcatf(restrict, " and Rank >= %d", min_rank);

        pktSendBitsPack(pak_out,1,auth_id);
        pktSendBitsPack(pak_out,1,db_id);
        pktSendString(pak_out,channel);
        sqlReadColumnsAsync(table,"","AuthName, AuthId",restrict,sqlReqCustomDataCallback,0,pak_out,db_id);
    }

    free(channel);
}

static void handleExecuteSql(Packet *pak)
{
    int sql_len;
    char *sql_command = pktGetStringAndLength(pak, &sql_len);

    sqlExecAsync(sql_command, sql_len);
}

void handleDisconnectMapserver(Packet *pak)
{
    int map_id = pktGetBitsPack(pak,1);
    dbDelinkMapserver(map_id);
}

static void handleDeletePlayer(Packet *pak)
{
    int dbid = pktGetBitsAuto(pak);

    EntCon *ent = containerLoad(ent_list, dbid); // deletePlayer will "unload"

    dbForceLogout(dbid, -3);
    if(ent)
        offlinePlayerDelete(ent->auth_id, ent->account, dbid);
    deletePlayer(dbid, 1, NULL, "mapserver", NULL);
}

void dbDelinkMapserver(int map_id)
{
    MapCon    *map_con;
    int dt;

    LOG_OLD("-delink %d", map_id);
    if (map_id >= 0)
    {
        bool didit = false;
        if (map_id >= CRASHED_MAP_BASE_ID) {
            map_con = containerPtr(dbListPtr(CONTAINER_CRASHEDMAPS), map_id);
            if (map_con) {
                didit = true;
                containerDelete(map_con);
            }
        }
        if (!didit) {
            map_con = containerPtr(dbListPtr(CONTAINER_MAPS),map_id);
            if (map_con)
            {
                if(map_con->is_static)
                    LOG_OLD("delinked map %d", map_id);
                printf("%s delinking map %d on %s\n", timerGetTimeString(), map_con->id, map_con->remote_process_info.hostname);
                if (!map_con->link)
                {
                    // The mapserver doesn't have a link, it must either still be starting
                    // or never have been launched.
                    if (map_con->seconds_since_update > 15) {
                        U32 ip = ipFromString(map_con->remote_process_info.hostname);
                        // This map was delinked because it was stuck
                        addCrashedMap(map_con, strdup("Delinked (STUCK STARTING)"), "Delinked", ip);
                        launcherCommRecordCrash(ip);
                    }
                    freeMapContainer(map_con);
                }
                else {
                    dt = timerCpuSeconds() - map_con->link->lastRecvTime;
                    if (dt > 15) {
                        U32 ip = ipFromString(map_con->remote_process_info.hostname);
                        // This map was delinked because it was stuck
                        addCrashedMap(map_con, strdup("Delinked (STUCK)"), "Delinked", ip);
                        launcherCommRecordCrash(ip);
                    }
                    netDiscardDeadLink(map_con->link);
                }
            }
        }
    }
    else
    {
        int        i,skip_starting=1;
        DbList    *list = dbListPtr(CONTAINER_MAPS);
        if (map_id == -2)
            skip_starting = 0;
        for(i=list->num_alloced-1;i>=0;i--)
        {
            map_con = (MapCon *)list->containers[i];
            if (skip_starting && map_con->starting)
                continue;
            if (!map_con->active && !map_con->starting)
                continue;
            if (map_con->link) {
                dt = timerCpuSeconds() - map_con->link->lastRecvTime;
            } else {
                // delink starting map if it was started more than 30 seconds ago!
                if (map_con->lastRecvTime) {
                    dt = timerCpuSeconds() - map_con->lastRecvTime;
                }
            }
            if (dt > 30)
            {
                dbDelinkMapserver(map_con->id);
            }
        }
    }
}

void sendLogLevels(NetLink * link, int cmd)
{
    int i;
    Packet *pak_out = pktCreateEx(link, cmd);
    for( i = 0; i < LOG_COUNT; i++ )  // we could probably not send all the default levels, but its a short list
        pktSendBitsPack(pak_out, 1, logLevel(i) );
    pktSend(&pak_out, link);
}

void testAllLogging()
{ 
    Packet *pak_out=0;
    NetLink * link;

    LOG( LOG_DEPRECATED, LOG_LEVEL_ALERT, LOG_CONSOLE, "DBServer Log Line Test: Alert" );
    LOG( LOG_DEPRECATED, LOG_LEVEL_IMPORTANT, LOG_CONSOLE, "DBServer Log Line Test: Important" );
    LOG( LOG_DEPRECATED, LOG_LEVEL_VERBOSE, LOG_CONSOLE, "DBServer Log Line Test: Verbose" );
    LOG( LOG_DEPRECATED, LOG_LEVEL_DEPRECATED, LOG_CONSOLE, "DBServer Log Line Test: Deprecated" );
    LOG( LOG_DEPRECATED, LOG_LEVEL_DEBUG, LOG_CONSOLE, "DBServer Log Line Test: Debug" );

    link = accountLink();
    if( link )
    {
        pak_out = pktCreateEx(link, ACCOUNT_CLIENT_TEST_LOGS);
        pktSend(&pak_out, link);
    }

    link = auctionLink(0);
    if( link )
    {
        pak_out = pktCreateEx(link, AUCTION_CLIENT_TEST_LOGS );
        pktSend(&pak_out, link);
    }

    if(shard_comm.connected)
    {
        pak_out = pktCreateEx(&shard_comm, SHARDCOMM_TEST_LOG_LEVELS );
        pktSend(&pak_out, &shard_comm);
    }


    link = missionserver_getLink();
    if( link )
    {
        pak_out = pktCreateEx(link, MISSION_CLIENT_TEST_LOG_LEVELS );
        pktSend(&pak_out, link);
    }

    link =  queueserver_getLink();
    if( link )
    {
        pak_out = pktCreateEx(link, QUEUESERVER_SVR_TEST_LOG_LEVELS );
        pktSend(&pak_out, link);
    }
     
    link = statLink();
    if( link )
    {
        pak_out = pktCreateEx(link, DBSERVER_TEST_LOG_LEVELS );
        pktSend(&pak_out, link);
    }

}

void updateAllServerLoggingInformation()
{
    int i;

    // connected mapservers
    for (i = 0; i < map_list->num_alloced; i++)
    {
        MapCon *map = (MapCon *) map_list->containers[i];
        if (map && map->active && map->link)
        {
            sendLogLevels(map->link, DBSERVER_UPDATE_LOG_LEVELS);
        }
    }

    if( server_cfg.isLoggingMaster )
    {
        NetLink *link=0;

        // accountserver
        link = accountLink();
        if( link )
            sendLogLevels( link, ACCOUNT_CLIENT_UPDATE_LOG_LEVELS );
        
        // auctionserver
        link = auctionLink(0);
        if( link )
            sendLogLevels( link, AUCTION_CLIENT_SET_LOG_LEVELS );

        // chatserver
        if(shard_comm.connected)
            sendLogLevels( &shard_comm, SHARDCOMM_SET_LOG_LEVELS );

        // missionserver
        link = missionserver_getLink();
        if( link )
            sendLogLevels( link, MISSION_CLIENT_SET_LOG_LEVELS );

        // queueserver
        link =  queueserver_getLink();
        if( link )
            sendLogLevels(link, QUEUESERVER_SVR_SET_LOG_LEVELS );

        // statserver
        link = statLink();
        if( link )
            sendLogLevels(link, DBSERVER_UPDATE_LOG_LEVELS );

    }
}

int handleInitialConnect( Packet * pak, NetLink * link ) 
{
    Packet *pak_out;
    int i;
    U32 map_protocol=0;

    if (!pktEnd(pak))
        map_protocol = pktGetBitsPack(pak,1);
    if (map_protocol != DBSERVER_PROTOCOL_VERSION)
    {
        char    buf[1000];

        sprintf(buf,"protocol version mismatch:\n dbserver: %d\n mapserver %d\n",DBSERVER_PROTOCOL_VERSION,map_protocol);
        sendFail(link,1,0,buf);
        lnkBatchSend(link);
        netRemoveLink(link); 
        return 0;
    }

    pak_out = pktCreateEx(link,DBSERVER_TIMEOFFSET);
    pktSendBits(pak_out,32,timerSecondsSince2000());
    pktSendF32( pak_out, server_cfg.timeZoneDelta );
    pktSend(&pak_out,link);

    pak_out = pktCreateEx(link,DBSERVER_OVERRIDDEN_AUTHBITS);
    pktSendBitsArray(pak_out,sizeof(g_overridden_auth_bits)*8,g_overridden_auth_bits);
    pktSend(&pak_out,link);

    if ((i = eaSize(&server_cfg.disabled_zone_events)) > 0)
    {
        pak_out = pktCreateEx(link,DBSERVER_DISABLED_ZONE_EVENTS);
        pktSendBitsAuto(pak_out, i--);
        while (i >= 0)
        {
            pktSendString(pak_out, server_cfg.disabled_zone_events[i--]);
        }
        pktSend(&pak_out,link);
    }

    if (authUserIsReactivationActive()) 
    {
        pak_out = pktCreateEx(link, DBSERVER_NOTIFY_REACTIVATION);
        pktSend(&pak_out, link);
    }

    sendLogLevels(link, DBSERVER_UPDATE_LOG_LEVELS);

    account_sendCatalogUpdateToNewMap(link);

    return 1;
}




int dbHandleClientMsg(Packet *pak,int cmd, NetLink *link)
{
    F32        elapsed,total_elapsed;
    int        ret=1;
    DBClientLink    *client = link->userData;
    static    int timer;
    U32        recv_time = pak->xferTime;

    if (client->last_processed_packet_id) {
        if (pak->id != client->last_processed_packet_id+1
             &&    pak->id - client->last_processed_packet_id != client->last_packet_sib_count
            )
        {
            if (link->last_control_command_packet_id > client->last_processed_packet_id) {
                // It was a control command
                client->last_command = 1;
            }
            else 
            {
                char *logfile=NULL;
                if (pak->id <= client->last_processed_packet_id)
                {
                    devassert(!"Out of order or dropped packet to DbServer?");
                    if (server_cfg.dropped_packet_logging) {
                        logfile = "pkt_out_of_order";
                    } else {
                        logfile = NULL;
                    }
                } else {
                    if (server_cfg.dropped_packet_logging==2) {
                        logfile = "pkt_skipped";
                    } else {
                        logfile = NULL;
                    }
                }
                if (logfile)
                {
                    LOG_OLD_ERR("%s:%d map_id:%d cmd: %d last_command: %d  pktid: %d  lastpktid: %d  cookie_recv: %d  last_cookie_recv: %d  last_control_command_packet_id: %d",
                        makeIpStr(link->addr.sin_addr.S_un.S_addr),
                        link->addr.sin_port,
                        client->map_id,
                        cmd,
                        client->last_command,
                        pak->id,
                        client->last_processed_packet_id,
                        link->cookie_recv,
                        link->last_cookie_recv,
                        link->last_control_command_packet_id);
                }
            }
        }
    }
    client->last_command = cmd;
    client->last_processed_packet_id = pak->id;
    client->last_packet_sib_count = pak->sib_count;

    if (!timer)
        timer = timerAlloc();
    timerStart(timer);
    switch(cmd)
    {
        xcase DBCLIENT_INITIAL_CONNECT:
            if (!handleInitialConnect(pak, link))
                return 0;
        xcase DBCLIENT_READY:
            containerSendListByMap(ent_list,client->map_id,link);
        xcase DBCLIENT_REGISTER:
            client->map_id = registerDbClient(pak,link);
            if (!client->map_id)
                netRemoveLink(link);
        xcase DBCLIENT_REGISTER_CONTAINER_SERVER:
            registerContainerServer(pak, link);
        xcase DBCLIENT_REGISTER_CONTAINER_SERVER_NOTIFY:
            registerContainerServerNotify(pak, link);
        xcase DBCLIENT_READY_FOR_PLAYERS:
            dbClientReadyForPlayers(pak,link);
        xcase DBCLIENT_CONTAINER_INFO:
            containerSendInfo(link);
        xcase DBCLIENT_REQ_CONTAINERS:
            handleContainerReq(pak,link);
        xcase DBCLIENT_SET_CONTAINERS:
            handleContainerSet(pak,link);
        xcase DBCLIENT_CONTAINER_RELAY:
            handleContainerRelay(pak,link);
        xcase DBCLIENT_CONTAINER_RECEIPT:
            handleContainerReceipt(pak,link);
        xcase DBCLIENT_CONTAINER_REFLECT:
            handleContainerReflect(pak,link);
        xcase DBCLIENT_REQ_CONTAINER_STATUS:
            handleStatusReq(pak,link);
        xcase DBCLIENT_REQUEST_MAP_XFER:
            handleMapXferRequest(pak,link);
        xcase DBCLIENT_MAP_XFER:
            handleMapXfer(pak,link);
        xcase DBCLIENT_CONTAINER_ACK:
            handleContainerAck(pak);
        xcase DBCLIENT_TEST_MAP_XFER:
            handleMapXferTest(pak,link);
        xcase DBCLIENT_SEND_DOORS:
            handleNewClientDoors(pak,link);
        xcase DBCLIENT_SAVELISTS:
            handleSaveLists(pak);
            logWaitForQueueToEmpty();
        xcase DBCLIENT_ADDDEL_MEMBERS:
            handleAddDelGroupMembers(pak,link);
        xcase DBCLIENT_PLAYER_DISCONNECT:
            handlePlayerDisconnect(pak,client);
        xcase DBCLIENT_SEND_MSG:
            handleSendMsg(pak);
        xcase DBCLIENT_REQ_GROUP_NAMES:
            handleReqGroupNames(pak,link);                //***SQL READ
        xcase DBCLIENT_REQ_ENT_NAMES:
            handleReqEntNames(pak,link);            //***SQL READ
        xcase DBCLIENT_CONTAINER_FIND_BY_ELEMENT:
            handleContainerFindByElement(pak,link);    //*** SQL READ if not a map container
        xcase DBCLIENT_SHUTDOWN:
            handleShutdown(pak,link,client);
        xcase DBCLIENT_WHO:
            handleWho(pak,link);
        xcase DBCLIENT_SERVER_STATS_UPDATE:
            handleServerStatsUpdate(pak,client->map_id);
        xcase DBCLIENT_RELAY_CMD:
            handleSendToAllServers(pak);
        xcase DBCLIENT_RELAY_CMD_BYENT:
            handleSendToEnt(client, pak);
        xcase DBCLIENT_RELAY_CMD_TOGROUP:
            handleSendToGroupMembers(client, pak);
        xcase DBCLIENT_REQ_ONLINE_ENTS:
            handleReqAllOnline(pak, link);
        xcase DBCLIENT_REQ_ONLINE_ENT_COMMENTS:
            handleReqAllOnlineComments(link);
        xcase DBCLIENT_PLAYER_KICKED:
            handlePlayerKicked(pak);
        xcase DBCLIENT_REQ_CUSTOM_DATA:
            handleReqCustomData(pak,link);            //***ASYNC SQL READ
        xcase DBCLIENT_EXECUTE_SQL:
            handleExecuteSql(pak);
        xcase DBCLIENT_DISCONNECT_MAPSERVER:
            handleDisconnectMapserver(pak);
        xcase DBCLIENT_PLAYER_RENAME:
            handlePlayerRename(pak,link);            //***SQL READ
        xcase DBCLIENT_PLAYER_CHANGETYPE:
            handlePlayerChangeType(pak,link);            //***SQL READ
        xcase DBCLIENT_PLAYER_CHANGESUBTYPE:
            handlePlayerChangeSubType(pak,link);            //***SQL READ
        xcase DBCLIENT_PLAYER_CHANGEPRAETORIANPROGRESS:
            handlePlayerChangePraetorianProgress(pak,link);        //***SQL READ
        xcase DBCLIENT_PLAYER_CHANGEINFLUENCETYPE:
            handlePlayerChangeInfluenceType(pak,link);        //***SQL READ
        xcase DBCLIENT_DEPRECATED:
            // does nothing
        xcase DBCLIENT_REQ_ARENA_ADDRESS:
            handleRequestArena(pak,link);
        xcase DBCLIENT_REQ_SG_ELDEST_ON:
            handleRequestSGEldestOn(pak,link);
        xcase DBCLIENT_REQ_SG_CHANNEL_INVITE:
            handleReqSgChannelInvite(pak,link);
        xcase DBCLIENT_SGRP_STATSADJ: 
            handleSgrpStatAdj(pak,link);
        xcase DBCLIENT_DESTROY_BASE:
            handleBaseDestroy(pak,link);
        xcase DBCLIENT_MISSION_PLAYER_COUNT:
            handleMissionPlayerCountRequest(pak,link);
        xcase DBCLIENT_SEND_STATSERVER_CMD:
            handleSendStatserverCmd(pak,link);
        xcase DBCLIENT_REQUEST_SHUTDOWN:
            handleRequestShutdown(pak,link);
        xcase DBCLIENT_EMERGENCY_SHUTDOWN:
            handleEmergencyShutdown(pak, link);
        xcase DBCLIENT_OFFLINE_CHAR:
            handleOfflineChar(pak, link);
        xcase DBCLIENT_RESTORE_DELETED_CHAR:
            handleRestoreDeletedChar(pak, link);
        xcase DBCLIENT_LIST_DELETED_CHARS:
            handleListDeletedChars(pak, link);
        xcase DBCLIENT_BACKUP:
            handleBackup(pak, link);
        xcase DBCLIENT_BACKUP_SEARCH:
            handleBackupSearch(pak, link);
        xcase DBCLIENT_BACKUP_APPLY:
            handleBackupApply(pak, link);
        xcase DBCLIENT_BACKUP_VIEW:
            handleBackupView(pak, link);
        xcase DBCLIENT_REQ_SOME_ONLINE_ENTS: 
            handleReqSomeOnlineEnts(pak, link);
        xcase DBCLIENT_OVERRIDE_START_ZONE:
            handleOverrrideStartZone(pak);
        xcase DBCLIENT_AUCTION_REQ_INV:
            handleAuctionClientReqInv(pak,link);
        xcase DBCLIENT_AUCTION_REQ_HISTINFO:
            handleAuctionClientReqHistInfo(pak, link);
        xcase DBCLIENT_AUCTION_XACT_REQ:
            handleAuctionXactReq(pak,link);
        xcase DBCLIENT_AUCTION_XACT_UPDATE: 
            handleAuctionXactUpdate(pak,link);
        xcase DBCLIENT_AUCTION_PURGE_FAKE:
            handleAuctionPurgeFake(pak,link);
        xcase DBCLIENT_AUCTION_XACT_MULTI_REQ:
            handleAuctionXactMultiReq(pak,link);
        xcase DBCLIENT_MININGDATA_RELAY:
            handleMiningDataRelay(pak);
        xcase DBCLIENT_SEND_AUCTIONSERVER_CMD: 
            handleSendAuctionCmd(pak,link);
        xcase DBCLIENT_ACCOUNTSERVER_CMD: 
            handleSendAccountCmd(pak,link);
        xcase DBCLIENT_ACCOUNTSERVER_SHARDXFER:
            handleAccountShardXfer(pak);
        xcase DBCLIENT_ACCOUNTSERVER_ORDERRENAME:
            handleAccountOrderRename(pak);
        xcase DBCLIENT_GET_PLAYNC_AUTH_KEY:
            handleRequestPlayNCAuthKey(pak);
        xcase DBCLIENT_ACCOUNTSERVER_CHARCOUNT:
            relayAccountCharCount(pak);
        xcase DBCLIENT_RENAME_RESPONSE:
            handleRenameResponse(pak);
        xcase DBCLIENT_CHECK_NAME_RESPONSE:
            handleCheckNameResponse(pak);
        xcase DBCLIENT_ACCOUNTSERVER_GET_INVENTORY:
            relayAccountGetInventory(pak);
        xcase DBCLIENT_ACCOUNTSERVER_CHANGE_INV:
            relayAccountInventoryChange(pak);
        xcase DBCLIENT_ACCOUNTSERVER_UPDATE_EMAIL_STATS:
            relayAccountUpdateEmailStats(pak);
        xcase DBCLIENT_ACCOUNTSERVER_CERTIFICATION_TEST:
            relayAccountCertificationTest(pak);
        xcase DBCLIENT_ACCOUNTSERVER_CERTIFICATION_GRANT:
            relayAccountCertificationGrant(pak);
        xcase DBCLIENT_ACCOUNTSERVER_CERTIFICATION_CLAIM:
            relayAccountCertificationClaim(pak);
        xcase DBCLIENT_ACCOUNTSERVER_CERTIFICATION_REFUND:
            relayAccountCertificationRefund(pak);
        xcase DBCLIENT_ACCOUNTSERVER_MULTI_GAME_TRANSACTION:
            relayAccountMultiGameTransaction(pak);
        xcase DBCLIENT_ACCOUNTSERVER_TRANSACTION_FINISH:
            relayAccountTransactionFinish(pak);
        xcase DBCLIENT_ACCOUNTSERVER_RECOVER_UNSAVED:
            relayAccountRecoverUnsaved(pak);
        xcase DBCLIENT_ACCOUNTSERVER_LOYALTY_CHANGE:
            relayAccountLoyaltyChange(pak);
        xcase DBCLIENT_ACCOUNTSERVER_LOYALTY_EARNED_CHANGE:
            relayAccountLoyaltyEarnedChange(pak);
        xcase DBCLIENT_ACCOUNTSERVER_LOYALTY_RESET:
            relayAccountLoyaltyReset(pak);
        xcase DBCLIENT_MISSIONSERVER_COMMAND:
            missionserver_db_remoteCall(client->map_id, pak, link);
        xcase DBCLIENT_MISSIONSERVER_PUBLISHARC:
            missionserver_db_publishArc(pak, link);
        xcase DBCLIENT_MISSIONSERVER_SEARCHPAGE:
            missionserver_db_requestSearchPage(client->map_id, pak, link);
        xcase DBCLIENT_MISSIONSERVER_ARCINFO:
            missionserver_db_requestArcInfo(client->map_id, pak, link);
        xcase DBCLIENT_MISSIONSERVER_BANSTATUS:
            missionserver_db_requestBanStatus(client->map_id, pak, link);
        xcase DBCLIENT_MISSIONSERVER_ALLARCS:
            missionserver_db_requestAllArcs(client->map_id, pak, link);
        xcase DBCLIENT_MISSIONSERVER_ARCDATA:
            missionserver_db_requestArcData(client->map_id, pak, link);
        xcase DBCLIENT_MISSIONSERVER_ARCDATA_OTHERUSER:
            missionserver_db_requestArcDataOtherUser(client->map_id, pak, link);
        xcase DBCLIENT_MISSIONSERVER_INVENTORY:
            missionserver_db_requestInventory(client->map_id, pak, link);
        xcase DBCLIENT_MISSIONSERVER_CLAIM_TICKETS:
            missionserver_db_claimTickets(client->map_id, pak, link);
        xcase DBCLIENT_MISSIONSERVER_BUY_ITEM:
            missionserver_db_buyItem(client->map_id, pak, link);
        xcase DBCLIENT_ACCOUNTSERVER_ORDERRESPEC:
            handleAccountOrderRespec(pak);
        xcase DBCLIENT_DELETE_PLAYER:
            handleDeletePlayer(pak);
        xcase DBCLIENT_QUEUE_FOR_EVENTS:
            turnstileDBserver_handleQueueForEvents(pak);
        xcase DBCLIENT_REMOVE_FROM_QUEUE:
            turnstileDBserver_handleRemoveFromQueue(pak);
        xcase DBCLIENT_EVENT_READY_ACK:
            turnstileDBserver_handleEventReadyAck(pak);
        xcase DBCLIENT_EVENT_RESPONSE:
            turnstileDBserver_handleEventResponse(pak);
        xcase DBCLIENT_MAP_ID:
            turnstileDBserver_handleMapID(pak);
        xcase DBCLIENT_TURNSTILE_PING:
            turnstileDBserver_handleTurnstilePing(pak, link);
        xcase DBCLIENT_DEBUG_SHARD_XFER_OUT:
            turnstileDBserver_handleShardXferOut(pak);
        xcase DBCLIENT_DEBUG_SHARD_XFER_BACK:
            turnstileDBserver_handleShardXferBack(pak);
        xcase DBCLIENT_GROUP_UPDATE:
            turnstileDBserver_handleGroupUpdate(pak);
        xcase DBCLIENT_EVENTHISTORY_FIND:
            handleEventHistoryFind(pak, link);
        xcase DBCLIENT_CLOSE_INSTANCE: 
            turnstileDBserver_handleCloseInstance(pak);
        xcase DBCLIENT_REJOIN_INSTANCE:
            turnstileDBserver_handleRejoinRequest(pak, link);
        xcase DBCLIENT_PLAYER_LEAVE:
            turnstileDBserver_handlePlayerLeave(pak);
        xcase DBCLIENT_MAP_WEEKLY_TF_ADD_TOKEN:
            {
                //    add to list
                WeeklyTF_AddTFToken(WeeklyTFCfg_getCurrentWeek(0), pktGetString(pak));
            }        
        xcase DBCLIENT_MAP_WEEKLY_TF_REMOVE_TOKEN:
            {
                //    remove from list
                WeeklyTF_RemoveTFToken(WeeklyTFCfg_getCurrentWeek(0), pktGetString(pak));
            }
        xcase DBCLIENT_MAP_WEEKLY_TF_SET_EPOCH_TIME:
            {
                char *date = strdup(pktGetString(pak));
                char *time = strdup(pktGetString(pak));
                WeeklyTF_SetEpochTime(WeeklyTFCfg_getCurrentWeek(0), date,time);
                free(date);
                free(time);
            }
        xcase DBCLIENT_TEST_LOGGING:
            testAllLogging();
        xcase DBCLIENT_INCARNATETRIAL_COMPLETE:
            turnstileDBserver_handleIncarnateTrialComplete(pak);
        xcase DBCLIENT_QUEUE_FOR_SPECIFIC_MISSION_INSTANCE:
            turnstileDBserver_handleQueueForSpecificMissionInstance(pak);
        xcase DBCLIENT_TS_ADD_BAN_DBID:
            turnstileDBserver_addBanDBId(pak);
        xcase DBCLIENT_MAP_SET_MARTY_STATUS:
            {
                char *buff = estrTemp();
                server_cfg.MARTY_enabled = pktGetBits(pak, 1);
                estrConcatf(&buff, "cmdrelay\nSetMARTYStatusRelay %i", server_cfg.MARTY_enabled);
                sendToServers(-1, buff, 0);
                estrDestroy(&buff);
            }
        xcase DBCLIENT_GRANT_CHARSLOT:
            handleCharSlotGrant(pak);
        xcase DBCLIENT_PLAYER_UNLOCK:
            handlePlayerUnlock(pak,link);
        xcase DBCLIENT_ACCOUNT_ADJUST_SERVER_SLOTS:
            handlePlayerAdjustServerSlots(pak,link);
        xcase DBCLIENT_UNLOCK_CHARACTER_RESPONSE:
            handleUnlockCharacterResponse(pak);
        xcase DBCLIENT_DEBUG_SET_VIP:
            handleDebugSetVIP(pak);
        xdefault:
            LOG_OLD_ERR("Unknown command %d\n",cmd);
            ret = 0;
    }
    total_elapsed = timerSeconds(timerCpuTicks() - recv_time);
    elapsed = timerElapsed(timer);
    if (total_elapsed > .1)
        LOG_OLD_ERR("cmd %d took %.1f msecs total %.1f msecs (%d bytes)\n",cmd,elapsed*1000,total_elapsed*1000, pak->stream.size);
    if (elapsed > .05)
        LOG_OLD_ERR("cmd %d took %.1f msecs total %.1f msecs (%d bytes)\n",cmd,elapsed*1000,total_elapsed*1000, pak->stream.size);
    if (elapsed > .1)
        LOG_OLD_ERR("cmd %d took %.1f msecs total %.1f msecs (%d bytes)\n",cmd,elapsed*1000,total_elapsed*1000, pak->stream.size);
    if (elapsed > .2)
        LOG_OLD_ERR("cmd %d took %.1f msecs total %.1f msecs (%d bytes)\n",cmd,elapsed*1000,total_elapsed*1000, pak->stream.size);
    if (total_elapsed > curr_max_latency)
    {
        curr_max_latency = total_elapsed;
        curr_max_latency_cmd = cmd;
    }
    logStatCmdIn(cmd,pak->stream.size);
    return ret;
}

F32 dbGetCurrMaxLatency(int *cmd)
{
    F32        latency = curr_max_latency;

    *cmd = curr_max_latency_cmd;
    curr_max_latency = 0;
    curr_max_latency_cmd = 0;
    return latency;
}
