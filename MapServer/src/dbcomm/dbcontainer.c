#include "dbcomm.h"
#include <utilitieslib/components/estring.h>
#include "dbcontainer.h"
#include <utilitieslib/utils/error.h>
#include "container_diff.h"
#include <utilitieslib/assert/assert.h>
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/file.h>
#include <utilitieslib/components/StashTable.h>
#include "storyarc/storyarcinterface.h"
#include "container/containerbroadcast.h"
#include "container/containercallbacks.h"
#include "entity/entity.h"
#include "container/team.h"
#include "gameSys/raidmapserver.h"
#include "gameSys/endgameraid.h"
#include <utilitieslib/components/earray.h>
#include <utilitieslib/utils/timing.h>
#include "container/container_util.h"
#include "container/container_store.h"
#include <utilitieslib/utils/log.h>

static int container_locking = 0; // don't overuse this, this is just to catch a bug
int last_db_error_code;
char last_db_error[256];
static MapNameEntry *map_names = 0;
static int            map_names_max = 0;

static StashTable container_id_hashes;

static int containerOutOfSequence(int list_id,int container_id,U32 pak_id)
{
    U32        old_pak_id=0;
    char    list_id_key[50];

    if (!container_id_hashes)
        container_id_hashes = stashTableCreateWithStringKeys(1000, StashDeepCopyKeys | StashCaseSensitive );
    sprintf(list_id_key,"%d.%d",list_id,container_id);
    if (stashFindInt(container_id_hashes,list_id_key,&old_pak_id))
    {
        if (old_pak_id >= pak_id) {
            teamlogPrintf(__FUNCTION__, "old_pak_id = %d, pak_id = %d", old_pak_id, pak_id);
            return 1;
        }
    }
    stashAddInt(container_id_hashes,list_id_key,pak_id, true);
    return 0;
}

void dbClearContainerIDCache(void)
{
    if(container_id_hashes)
        stashTableClear(container_id_hashes);
}


int dbReadContainerUncached(Packet *pak,ContainerInfo *ci,int list_id)
{
    int        i;

    memset(ci,0,sizeof(*ci));
    ci->id    = pktGetBitsPack(pak,1);
    ci->error_code        = pktGetBitsPack(pak,1);
    if (ci->error_code)
    {
        if (ci->error_code == CONTAINER_ERR_ALREADY_LOCKED)
            verbose_printf("container %d.%d already locked\n",list_id,ci->id);

        if (ci->error_code == CONTAINER_ERR_DOESNT_EXIST)
            verbose_printf("container %d.%d doesnt exist\n",list_id,ci->id);

        if (ci->error_code == CONTAINER_ERR_NOT_LOCKED)
            verbose_printf("container %d.%d is not locked\n",list_id,ci->id);

        last_db_error_code = ci->error_code;
        return 0;
    }
    ci->is_map_xfer        = pktGetBitsPack(pak,1);
    ci->is_static_map    = pktGetBitsPack(pak,1);
    ci->got_lock        = pktGetBitsPack(pak,1);
    ci->delete_me        = pktGetBitsPack(pak,1);
    ci->demand_loaded    = pktGetBitsPack(pak,1);
    ci->member_count    = pktGetBitsPack(pak,1);
    assert(ci->member_count <= MAX_CONTAINER_MEMBERS);
    for(i=0;i<ci->member_count;i++)
        ci->members[i] = pktGetBitsPack(pak,1);
    ci->data            = pktGetString(pak);
    if (containerOutOfSequence(list_id,ci->id,pak->id))
    {
        teamlogPrintf(__FUNCTION__, "discarding packet %i for %s:%i", pak->id, teamTypeName(list_id), ci->id); 
        assert(!container_locking && "We got an out of order packet while trying to lock a container");
        return 0;
    }
    if (list_id == CONTAINER_LEAGUES)
    {
        teamlogPrintf(__FUNCTION__, "updated packet hash id (%i)for container (%i)", pak->id, ci->id);
    }
    return 1;
}

int dbReadContainer(Packet *pak,ContainerInfo *ci,int list_id)
{
    if(dbReadContainerUncached(pak, ci, list_id))
    {
        containerCacheAdd(list_id,ci->id,ci->data);
        return 1;
    }

    return 0;
}

void dbReceiveEnts(int count,Packet *pak,NetLink *link)
{
    int                i;
    static    int        max_ents = 0,*ack_list = NULL,*cookie_list = NULL;
    Packet            *pak_out;
    ContainerInfo    ci;
    char *ci_data = NULL;

    if (count > max_ents)
    {
        ack_list = realloc(ack_list,count * sizeof(int));
        cookie_list = realloc(cookie_list,count * sizeof(int));
        max_ents = count;
    }

    for(i=0;i<count;i++)
    {
        if (!dbReadContainer(pak,&ci,CONTAINER_ENTS))
            continue;
        ack_list[i] = ci.id;
        
        // ci.data points to a static buffer. lets keep it around.
        estrPrintCharString(&ci_data,ci.data);  
        ci.data = ci_data;
        cookie_list[i] = dealWithContainer( &ci, CONTAINER_ENTS );
    }

    pak_out = pktCreateEx(link,DBCLIENT_CONTAINER_ACK);
    pktSendBitsPack(pak_out,1,CONTAINER_ENTS);
    pktSendBitsPack(pak_out,1,count);
    for(i=0;i<count;i++)
    {
        pktSendBitsPack(pak_out,1,ack_list[i]);
        pktSendBitsPack(pak_out,1,cookie_list[i]);
    }
    pktSend(&pak_out,link);
    
    estrDestroy(&ci_data);
}

void dbReceiveMaps(int count,Packet *pak)
{
    int        i;
    //char    *args[10];
    ContainerInfo    ci;

    for(i=0;i<count;i++)
    {
        char mapname[500];
        if (!dbReadContainer(pak,&ci,CONTAINER_MAPS))
            continue;

        //tokenize_line(ci.data,args,0);
        Strncpyt(mapname,getContainerValueStatic(ci.data, "MapName"));
        
        if (!db_state.map_registered)
        {
            db_state.map_registered = 1;
            db_state.map_id = ci.id;
            db_state.is_static_map = ci.is_static_map;
            strcpy(db_state.map_name,mapname);
            
            // order is important, this must come after the previous getContainerValueStatic call
            {
                char * missioninfo = getContainerValueStatic(ci.data, "MissionInfo");
                if (missioninfo[0] == 'A')
                    ArenaSetInfoString(missioninfo);
                else if (missioninfo[0] == 'B')
                    SGBaseSetInfoString(missioninfo);
                else if(missioninfo[0] == 'R')
                    RaidSetInfoString(missioninfo);
                else if(missioninfo[0] == 'E')
                    EndGameRaidSetInfoString(missioninfo);
                else
                    MissionSetInfoString(missioninfo,0);
            }
        }
        {
            MapNameEntry    *entry;

            entry = dynArrayFit(&map_names,sizeof(map_names[0]),&map_names_max,ci.id);
            entry->valid = 1;
            strncpy(entry->name,mapname,sizeof(entry->name));
        }
    }
}

int dbReceiveGeneric(int count,Packet *pak,int list_id)
{
    int        i;
    ContainerInfo    ci;

    for(i=0;i<count;i++)
    {
        if (!dbReadContainer(pak,&ci,list_id))
            return 0;
        dealWithContainer( &ci, list_id );
    }
    return 1;
}

int dbReceiveContainers(Packet *pak,NetLink *link)
{
    int        list_id,count;

    list_id        = pktGetBitsPack(pak,1);
    count        = pktGetBitsPack(pak,1);

    if (list_id == CONTAINER_ENTS)
        dbReceiveEnts(count,pak,link);
    else if (list_id == CONTAINER_MAPS)
        dbReceiveMaps(count,pak);
    else if (CONTAINER_IS_VALID(list_id))
        return dbReceiveGeneric(count,pak,list_id);
    else
    {
        printf("Bad id received by dbReceiveContainers: %i\n", list_id);
        return 0;
    }
    return 1;
}

static int dbReceiveContainersNoProcess(Packet *pak,int cmd,NetLink *link)
{
    int                list_id,count;

    list_id            = pktGetBitsPack(pak,1);
    count            = pktGetBitsPack(pak,1);

    return dbReceiveGeneric(count,pak,list_id);
}

static int getNameVal(char *str,char *name)
{
    char    *s;
    int        val = 0;

    s = strstri(str,name);
    if (s)
        val = atoi(s + strlen(name));
    return val;
}

static void getNameString(char* str, char *name, char* result, int len)
{
    char *s;

    result[0] = result[len-1] = 0;
    s = strstri(str,name);
    if (s)
    {
        strncpy(result, s + strlen(name), len-1);
        s = strchr(result, ' ');
        if (s) *s = 0;
    }
}

static ContainerStatus *curr_status = NULL;
static int curr_status_count = 0;

void dbReceiveContainerStatus(Packet *pak)
{
    int        list_id,count,i,valid;
    char    *str;

    list_id = pktGetBitsPack(pak,1);
    count = pktGetBitsPack(pak,1);

    // We can't receive anything if we have nowhere to put it, nor can we
    // overflow our buffer if we do have one.
    devassert((count <= curr_status_count) && (curr_status || !count));

    for(i=0;i<count;i++)
    {
        if (curr_status && i<curr_status_count)
            curr_status[i].valid = 0;
        valid = pktGetBitsPack(pak,1);
        if (!valid)
            continue;
        str = pktGetString(pak);
        if (curr_status && i<curr_status_count)
        {
            curr_status[i].valid = 1;
            curr_status[i].map_id = getNameVal(str,"MapId");
            curr_status[i].ready = !getNameVal(str,"NotReady");
            getNameString(str, "Info ", curr_status[i].mission_info, 1024);
        }
    }
}

int dbSyncContainerStatusRequest(int list_id,ContainerStatus *c_list,int count)
{
    static PerformanceInfo* perfInfo;

    int        i;
    Packet    *pak;

    curr_status = c_list;
    curr_status_count = count;

    // Can't get status if we have nowhere to store it, but setting the
    // pointer to NULL and asking for nothing is fine.
    devassert(curr_status || !curr_status_count);

    if (curr_status && curr_status_count)
    {
        pak = pktCreateEx(&db_comm_link,DBCLIENT_REQ_CONTAINER_STATUS);
        pktSendBitsPack(pak,1,0);
        pktSendBitsPack(pak,1,list_id);
        pktSendBitsPack(pak,1,count);
        for(i=0;i<count;i++)
            pktSendBitsPack(pak,1,c_list[i].id);
        pktSend(&pak,&db_comm_link);
        dbMessageScanUntil(__FUNCTION__, &perfInfo);
    }
    return 1;
}

void handleLostMembership(Packet *pak)
{
    int        list_id,container_id,ent_id, oos;

    list_id = pktGetBitsPack(pak,1);
    container_id = pktGetBitsPack(pak,1);
    ent_id = pktGetBitsPack(pak,1);
    dealWithLostMembership(list_id,container_id,ent_id);
    // Update the packet ID of this container update
    oos = containerOutOfSequence(list_id, container_id, pak->id);

    if (list_id == CONTAINER_LEAGUES)
    {
        teamlogPrintf(__FUNCTION__, "updated packet hash id (%i)for container (%i) for ent (%i)", pak->id, container_id, ent_id);
    }
    if (oos) { // Debug check
        Entity *e = entFromDbId(ent_id);
        // It's fine for this to be "out of sequence" if the entity isn't even on this map
        //  this will happen when the DbServer sends a kick player message immediately followed
        //  by a remove from teamup message.
        teamlogPrintf(__FUNCTION__, "had oos packet for %i", ent_id); 
#ifdef SERVER
        // JWG: These asserts are ordered from most lenient, to least lenient, so we can see how far we get.
        assert(!e || !e->ready || CONTAINER_IS_STATSERVER_OWNED(list_id));
        assert(!e || !e->ready || list_id == CONTAINER_LEAGUES);
//        assert(!e || !e->ready);
#endif // SERVER
    }
}

void handleBroadcastMsg(Packet *pak)
{
    int        list_id,id,members[200],i,count,msg_type,senderID;
    char    *msg,*temp_msg;

    list_id  = pktGetBitsPack(pak,1);
    msg_type = pktGetBitsPack(pak,1);
    senderID = pktGetBitsPack(pak,1);
    id         = pktGetBitsPack(pak,1);
    temp_msg = pktGetString(pak);
    count    = pktGetBitsPack(pak,1); // count of zero means it's being sent to an ent rather than a group
    assert(count <= ARRAY_SIZE(members));

    strdup_alloca(msg, temp_msg);

    for(i=0;i<count;i++)
        members[i] = pktGetBitsPack(pak,1);
    //printf("list %d, container %d got broadcast msg %s\n",list_id,id,msg);

    dealWithBroadcast( id, members, count, msg, msg_type, senderID, list_id );
}

int dbSyncContainerUpdate(int list_id,int container_id,int cmd,char *data)
{
    static PerformanceInfo* perfInfo;

    dbAsyncContainerUpdate( list_id, container_id, cmd, data, 0);
    return dbMessageScanUntil(__FUNCTION__, &perfInfo);
}

void dbBroadcastMsg(int list_id,int *ids, int msg_type, int senderID, int count,char *msg)
{
    int        i;
    Packet    *pak;

    pak = pktCreateEx(&db_comm_link,DBCLIENT_SEND_MSG);
    pktSendBitsPack(pak,1,list_id);
    pktSendBitsPack(pak,1,msg_type);
    pktSendBitsPack(pak,1,senderID);
    pktSendBitsPack(pak,1,count);
    pktSendString(pak,msg);
    for(i=0;i<count;i++)
        pktSendBitsPack(pak,1,ids[i]);
    pktSend(&pak,&db_comm_link);
}

void dbAsyncContainerCreate(int list_id,int *container_id)
{
    int        cookie;

    cookie = dbSetDataCallback(container_id);
    dbAsyncContainerUpdate(list_id,-1,CONTAINER_CMD_CREATE,"",cookie);
}

// cmd can be 0 or CONTAINER_CMD_LOCK
void dbAsyncContainerRequest(int list_id,int container_id,int cmd,NetPacketCallback *cb_func)
{
    Packet    *pak;

    pak = pktCreateEx(&db_comm_link,DBCLIENT_REQ_CONTAINERS);
#if defined(_M_X64)
        #pragma message("The line below needs to be proper ported to 64 bit compatible code")
#endif
    pktSendBitsPack(pak,1,(U32)(uintptr_t)cb_func);
    pktSendBitsPack(pak,1,list_id);
    pktSendBitsPack(pak,1,cmd);
    pktSendBitsPack(pak,1,1);
    pktSendBitsPack(pak,1,container_id);
    pktSend(&pak,&db_comm_link);
}

void dbAsyncContainersRequest(int list_id,int *eaContainer_ids,int cmd,NetPacketCallback *cb_func)
{
    if(verify( eaContainer_ids && eaiSize(&eaContainer_ids) > 0))
    {        
        Packet    *pak;
        int n = eaiSize(&eaContainer_ids);
        int i;
        
        pak = pktCreateEx(&db_comm_link,DBCLIENT_REQ_CONTAINERS);
#if defined(_M_X64)
        #pragma message("The line below needs to be proper ported to 64 bit compatible code")
#endif
        pktSendBitsPack(pak,1,(U32)(uintptr_t)cb_func);
        pktSendBitsPack(pak,1,list_id);
        pktSendBitsPack(pak,1,cmd);
        pktSendBitsPack(pak,1,n);

        for( i = 0; i < n; ++i ) 
        {
            int container_id = eaContainer_ids[i];
            pktSendBitsPack(pak,1,container_id);
        }

        pktSend(&pak,&db_comm_link);
    }
}


//
//
//
int dbSyncContainerRequest(int list_id,int container_id,int cmd, int no_process)
{
    static PerformanceInfo* perfInfo;

    int        i,ret;

    // retry until you get a lock
    for(i=0;i<2000;i++)
    {
        container_locking = (cmd == CONTAINER_CMD_LOCK);
        last_db_error_code = 0;
        if (no_process)
            dbAsyncContainerRequest(list_id,container_id,cmd,dbReceiveContainersNoProcess);
        else
            dbAsyncContainerRequest(list_id,container_id,cmd,0);
        ret = dbMessageScanUntil(__FUNCTION__, &perfInfo);
        container_locking = 0;
        if (ret)
            return ret;
        if (last_db_error_code != CONTAINER_ERR_ALREADY_LOCKED)
        {
            // MAK - this shouldn't be happening
            // Is this an alert level error or meaningless?
            LOG_OLD_ERR( "dbSyncContainerRequest failed: %s, container %i:%i\n",last_db_error, list_id, container_id );
            return 0;
        }
        Sleep(20);
    }
    if (!ret)
        FatalErrorf("timeout waiting for lock on: %d.%d (last_db_error_code: %d)",list_id,container_id,last_db_error_code);
    return ret;
}

//
//
//
int dbSyncContainerRequestCustom(int list_id,int container_id,int cmd, NetPacketCallback *cb_func)
{
    static PerformanceInfo* perfInfo;

    int        i,ret;

    // retry until you get a lock
    for(i=0;i<2000;i++)
    {
        container_locking = ((cmd == CONTAINER_CMD_LOCK) || (cmd == CONTAINER_CMD_LOCK_AND_LOAD));
        last_db_error_code = 0;
        dbAsyncContainerRequest(list_id,container_id,cmd,cb_func);
        ret = dbMessageScanUntil(__FUNCTION__, &perfInfo);
        container_locking = 0;
        if (ret)
            return ret;
        if (last_db_error_code != CONTAINER_ERR_ALREADY_LOCKED)
        {
            // MAK - this shouldn't be happening
            // Is this an alert level error or meaningless?
            LOG_OLD_ERR("dbSyncContainerRequest failed: %s, container %i:%i\n",last_db_error, list_id, container_id);
            return 0;
        }
        Sleep(20);
    }
    if (!ret)
        FatalErrorf("timeout waiting for lock on: %d.%d",list_id,container_id);
    return ret;
}
//
//
//
int dbSyncContainerCreate(int list_id,int *container_id)
{
    static PerformanceInfo* perfInfo;

    
    dbAsyncContainerCreate( list_id, container_id );
    
    return dbMessageScanUntil(__FUNCTION__, &perfInfo);
}


int dbContainerAddDelMembers(int list_id, int add, int autolock, int group_id,
                             int count, int *members, char* data)
{
    static PerformanceInfo* perfInfo;

    Packet    *pak;
    int        i,j,ret;
    int delay, salt;            // DGNOTE use to try an avoid collisions, maybe these cause the mapGroupInit problem?

    delay = 1;
    salt = (int) __rdtsc();        // Get a random salt.  Come talk to David G if you really want the horror story on why I'm not using a "normal" RNG.
    for(j=0;j<10;j++)
    {
        pak = pktCreateEx(&db_comm_link,DBCLIENT_ADDDEL_MEMBERS);
        pktSendBitsPack(pak,1,list_id);
        pktSendBitsPack(pak,1,add);
        pktSendBitsPack(pak,1,group_id);
        if(data && data[0])
        {
            assert(!autolock || !add); // when adding with autolocking, we don't have the container locked
            containerCacheSendDiff(pak,list_id,group_id,data,db_state.diff_debug);
        }
        else
        {
            pktSendBits(pak,1,0);
            pktSendBits(pak,1,0);
            pktSendString(pak,"");
        }
        pktSendBitsPack(pak,1,count);
        for(i=0;i<count;i++)
            pktSendBitsPack(pak,1,members[i]);

        //// JWG: Logging, for debug purposes.
        //if (list_id == CONTAINER_TEAMUPS)
        //{
        //    if (autolock)
        //    {
        //        if (add)
        //        {
        //            LOG(LOG_DEBUG_LOG, LOG_LEVEL_IMPORTANT, 0, __FUNCTION__ ": autolock on add member.");
        //        }
        //        else
        //        {
        //            LOG(LOG_DEBUG_LOG, LOG_LEVEL_IMPORTANT, 0, __FUNCTION__, ": autolock on del member.")
        //        }
        //    }
        //}
        pktSendBool(pak, autolock);
        pktSend(&pak,&db_comm_link);
        last_db_error_code = 0;
        ret = dbMessageScanUntil(__FUNCTION__, &perfInfo);
        if (!ret)
        {
            // MAK - this is to be expected every once in a long while,
            //       if the team got deleted that I was invited to
            printf("dbContainerAddDelMembers failed: %s\n",last_db_error);
            if (last_db_error_code != CONTAINER_ERR_ALREADY_LOCKED)
                break;
        }
        else
            break;
        if (list_id == CONTAINER_MAPGROUPS)    // DGNOTE - HACK ALERT!  We're getting fairly rare failures in here because CONTAINER_MAPGROUPS is showing up
                                            // perma locked, despite the Sleep(1) delay / retry code.  So we steal an idea from network code, and retry
                                            // with increasing delay in the event of a lock collision.
        {
            if (delay >= 32)                // If the delay has grown to 32, don't let it get any bigger, but do randomize a little
            {
                Sleep(delay + (salt & 3));    // salt the delay with a couple of random bits now we're at the cap
                salt = salt >> 2;
            }
            else                            // If it's < 32, double it.
            {
                Sleep(delay);
                delay += delay;
            }
        }
        else
        {
            Sleep(1);                        // Not CONTAINER_MAPGROUPS, so just do the usual Sleep(1)
                                            // Just for the sake of my curiosity, log when this happens, so we have some idea how often *other* containers are colliding.
            LOG_DEBUG("dbContainerAddDelMembers() non-CONTAINER_MAPGROUPS retrying");    // DGNOTE - debugging
        }
    }
    return ret;
}

// cmd can be 0 or CONTAINER_CMD_UNLOCK, CONTAINER_CMD_DELETE, or CONTAINER_CMD_UNLOCK_NOMODIFY
// returns network cookie
void dbAsyncContainerUpdate(int list_id,int container_id,int cmd,const char *data,int callback_id)
{
    Packet    *pak;
    
    // @testing -AB: trying to find badge corruption :07/19/06
    if(list_id == CONTAINER_ENTS && data )
    {
        extern bool checkContainerBadges_Internal(const char *container);
        static bool saveit = 10;
        if(!checkContainerBadges_Internal(data))
            LOG( LOG_ERROR, LOG_LEVEL_VERBOSE, 0, "id=%d bad container sent=%s", container_id, (saveit-- > 0)?data:"");
    }
    
    assert(cmd == CONTAINER_CMD_CREATE || container_id > 0);
    pak = pktCreateEx(&db_comm_link,DBCLIENT_SET_CONTAINERS);
    pktSendBitsPack(pak,1,list_id);
    pktSendBitsPack(pak,1,cmd);
    pktSendBitsPack(pak,1,callback_id);
    pktSendBitsPack(pak,1,1); // number of containers
    pktSendBitsPack(pak,1,container_id);
    
    containerCacheSendDiff(pak,list_id,container_id,data,db_state.diff_debug);
    pktSend(&pak,&db_comm_link);
}

void dbWaitContainersAcked()
{
    static PerformanceInfo* perfInfo;

    dbMessageScanUntil(__FUNCTION__, &perfInfo);
}

static found_list_id,found_container_id,found_map_id,found_num_players;

void handleContainerID(Packet *pak)
{
    found_list_id = pktGetBitsPack(pak,1);
    found_container_id = pktGetBitsPack(pak,1);
    found_map_id = pktGetBitsPack(pak,1);
}

void handleMissionPlayerCountResponse(Packet *pak)
{
    found_num_players = pktGetBitsPack(pak,1);
}

// returns -1 if it couldn't find it.
int dbSyncContainerFindByElementEx(int list_id,char *element,char *value,int *curr_map,int online_only,int search_offline_ents)
{
    static PerformanceInfo* perfInfo;

    Packet    *pak;

    pak = pktCreateEx(&db_comm_link,DBCLIENT_CONTAINER_FIND_BY_ELEMENT);
    pktSendBitsPack(pak,1,list_id);
    pktSendString(pak,element);
    pktSendString(pak,escapeString(value)); // Should this be done here?  Or is it already escaped by here?
    pktSendBitsPack(pak,1,online_only);
    pktSendBits(pak,1,search_offline_ents);        // search offline ents
    pktSend(&pak,&db_comm_link);

    dbMessageScanUntil(__FUNCTION__, &perfInfo);
    if (curr_map)
        *curr_map = found_map_id;
    return found_container_id;
}

// returns -1 if it couldn't find it.
int dbSyncContainerFindByElement(int list_id,char *element,char *value,int *curr_map,int online_only)
{
    return dbSyncContainerFindByElementEx(list_id,element,value,curr_map,online_only,0);
}

// returns -1 if it couldn't find the mission
int dbSyncMissionPlayerCount(char* missioninfo)
{
    static PerformanceInfo* perfInfo;

    Packet    *pak;

    pak = pktCreateEx(&db_comm_link,DBCLIENT_MISSION_PLAYER_COUNT);
    pktSendString(pak,missioninfo);
    pktSend(&pak,&db_comm_link);

    dbMessageScanUntil(__FUNCTION__, &perfInfo);
    
    return found_num_players;
}

void dbWriteTemplate(char *dir,char *fname,char *data)
{
    char    dirslash[1000] = "",dirpath[1000];
    FILE    *file;

    if (dir)
        sprintf(dirslash,"%s/",dir);

    sprintf(dirpath,"%sserver/db/templates/%s",dirslash,fname);
    file = fileOpen(dirpath, "wt" );
    if (!file)
        FatalErrorf("Can't open %s for writing!",dirpath);
    fwrite( data, strlen(data), 1, file );
    memFree( data );
    fileClose( file );

}

void dbWriteSchema(char *dir,char *fname,char *data)
{
    char    dirslash[1000] = "",dirpath[1000];
    FILE    *file;

    if (dir)
        sprintf(dirslash,"%s/",dir);

    sprintf(dirpath,"%sserver/db/schemas/%s",dirslash,fname);
    file = fileOpen(dirpath, "wt" );
    if (!file)
        FatalErrorf("Can't open %s for writing!",dirpath);
    fwrite( data, strlen(data), 1, file );
    memFree( data );
    fileClose( file );
}

void dbWriteAttributes(char *dir, char *fname, char *data)
{
    char        *s,*mem,dirslash[1000] = "",dirpath[1000],*args[10];
    StashTable    hash_table;
    int            idx,max_idx=0,count;
    FILE        *file;

    if (dir)
        sprintf(dirslash,"%s/",dir);

    sprintf(dirpath,"%sserver/db/templates/%s", dirslash, fname);
    mem = fileAlloc(dirpath, 0);
    file = fileOpen(dirpath, "wt" );
    if(!file)
        FatalErrorf("Failed to open attribute file %s for writing", fname);
    hash_table = stashTableCreateWithStringKeys(10000,StashDeepCopyKeys);
    s = mem;

    while((count=tokenize_line(s,args,&s)))
    {
        if (count != 2)
            continue;
        idx = atoi(args[0]);
        stashAddInt(hash_table,args[1], idx + 1, false);
        if (idx > max_idx)
            max_idx = idx;
        fprintf(file,"%d \"%s\"\n",idx,args[1]);
    }
    free(mem);

    s = data;
    while(tokenize_line(s,args,&s))
    {
        if (!stashFindInt(hash_table,args[0],&idx))
        {
            idx = ++max_idx;
            stashAddInt(hash_table,args[0],idx,false);
            fprintf(file,"%d \"%s\"\n",idx,args[0]);
        }
    }
    free(data);
    fclose(file);
    stashTableDestroy(hash_table);
}

char *dbGetMapName(int idx)
{
    int        i,ret;

    if (idx <= 0)
        return 0;
    for(i=0;i<2;i++)
    {
        if (idx < map_names_max && map_names[idx].valid)
            return map_names[idx].name;

        ret = dbSyncContainerRequest(CONTAINER_MAPS,idx,CONTAINER_CMD_DONT_LOCK,0);
    }
    return 0;
}

int dbGetMapId(const char *mapname)
{
    int        i;
    const char *s;

    if (!mapname)
        return -1;
    s = getFileName(mapname);
    for(i=0;i<map_names_max;i++)
    {
        if (map_names[i].valid && stricmp(s,getFileName(map_names[i].name))==0)
            return i;
    }
    return -1;
}

void dbHandleContainerAck(Packet *pak)
{
    int        list_id,count,i,id,callback_id;

    list_id = pktGetBitsPack(pak,1);
    callback_id = pktGetBitsPack(pak,1);
    count = pktGetBitsPack(pak,1);
    for(i=0;i<count;i++)
    {
        id = pktGetBitsPack(pak,1);
        db_state.last_id_acked = id;
        switch (list_id)
        {
        case CONTAINER_ENTS:
            dealWithEntAck(list_id,id);
            break;
        case CONTAINER_SHARDACCOUNT:
            dealWithShardAccountAck(list_id,id);
            break;
        default:
            dbProcessDataCallback(callback_id,id);
            break;
        }
    }
}

//----------------------------------------
//  Send a bunch of rows with the same container id to the db
// param cmd: the command to send. 0 is fine.
//----------------------------------------
void dbContainerSendList(int list_id,char **data,int *ids,int count, ContainerCmd cmd)
{
    int            i;
    Packet        *pak;

    if (!count)
        return;
    pak = pktCreateEx(&db_comm_link,DBCLIENT_SET_CONTAINERS);
    pktSendBitsPack(pak,1,list_id);
    pktSendBitsPack(pak,1,cmd); 
    pktSendBitsPack(pak,1,0); // callback id (ignored here)
    pktSendBitsPack(pak,1,count);
    for(i=0;i<count;i++)
    {
        // @testing -AB: trying to find badge corruption :07/19/06
        if(list_id == CONTAINER_ENTS && data && data[i])
        {
            extern bool checkContainerBadges_Internal(const char *container);
            static bool saveit = 10;
            if(!checkContainerBadges_Internal(data[i]))
                LOG( LOG_ERROR, LOG_LEVEL_VERBOSE, 0, "id=%d bad container sent=%s", ids[i], (saveit-- > 0)?data[i]:"");
        }
        
        pktSendBitsPack(pak,1,ids[i]);
        containerCacheSendDiff(pak,list_id,ids[i],data[i],db_state.diff_debug);
    }
    pktSend(&pak,&db_comm_link);
}

int dbSyncPlayerRename(int db_id, char* newname)
{
    static PerformanceInfo* perfInfo;
    Packet        *pak;

    pak = pktCreateEx(&db_comm_link,DBCLIENT_PLAYER_RENAME);
    pktSendBitsPack(pak,1,0);    // callback id
    pktSendBitsPack(pak,1,db_id);
    pktSendString(pak,newname);
    pktSend(&pak,&db_comm_link);
    last_db_error_code = 0;
    dbMessageScanUntil(__FUNCTION__, &perfInfo);
    return last_db_error_code == 0;
}

int dbSyncPlayerUnlock(int db_id)
{
    static PerformanceInfo* perfInfo;
    Packet        *pak;

    pak = pktCreateEx(&db_comm_link,DBCLIENT_PLAYER_UNLOCK);
    pktSendBitsPack(pak,1,0);    // callback id
    pktSendBitsPack(pak,1,db_id);
    pktSend(&pak,&db_comm_link);
    last_db_error_code = 0;
    dbMessageScanUntil(__FUNCTION__, &perfInfo);
    return last_db_error_code == 0;
}

int dbSyncAccountAdjustServerSlots(U32 auth_id, int delta)
{
    static PerformanceInfo* perfInfo;
    Packet        *pak;

    pak = pktCreateEx(&db_comm_link,DBCLIENT_ACCOUNT_ADJUST_SERVER_SLOTS);
    pktSendBitsPack(pak,1,0);    // callback id
    pktSendBitsAuto(pak,auth_id);
    pktSendBitsAuto(pak,delta);
    pktSend(&pak,&db_comm_link);
    last_db_error_code = 0;
    dbMessageScanUntil(__FUNCTION__, &perfInfo);
    return last_db_error_code == 0;
}

int dbSyncPlayerChangeType(int db_id, int type)
{
    static PerformanceInfo* perfInfo;
    Packet        *pak;

    pak = pktCreateEx(&db_comm_link,DBCLIENT_PLAYER_CHANGETYPE);
    pktSendBitsPack(pak,1,0);    // callback id
    pktSendBitsPack(pak,1,db_id);
    pktSendBitsPack(pak,1,type);
    pktSend(&pak,&db_comm_link);
    last_db_error_code = 0;
    dbMessageScanUntil(__FUNCTION__, &perfInfo);
    return last_db_error_code == 0;
}

int dbSyncPlayerChangeSubType(int db_id, int subtype)
{
    static PerformanceInfo* perfInfo;
    Packet        *pak;

    pak = pktCreateEx(&db_comm_link,DBCLIENT_PLAYER_CHANGESUBTYPE);
    pktSendBitsPack(pak,1,0);    // callback id
    pktSendBitsPack(pak,1,db_id);
    pktSendBitsPack(pak,1,subtype);
    pktSend(&pak,&db_comm_link);
    last_db_error_code = 0;
    dbMessageScanUntil(__FUNCTION__, &perfInfo);
    return last_db_error_code == 0;
}

int dbSyncPlayerChangePraetorianProgress(int db_id, int progress)
{
    static PerformanceInfo* perfInfo;
    Packet *pak;

    pak = pktCreateEx(&db_comm_link,DBCLIENT_PLAYER_CHANGEPRAETORIANPROGRESS);
    pktSendBitsPack(pak,1,0);    // callback id
    pktSendBitsPack(pak,1,db_id);
    pktSendBitsPack(pak,1,progress);
    pktSend(&pak,&db_comm_link);
    last_db_error_code = 0;
    dbMessageScanUntil(__FUNCTION__, &perfInfo);
    return last_db_error_code == 0;
}

int dbSyncPlayerChangeInfluenceType(int db_id, int type)
{
    static PerformanceInfo* perfInfo;
    Packet *pak;

    pak = pktCreateEx(&db_comm_link,DBCLIENT_PLAYER_CHANGEINFLUENCETYPE);
    pktSendBitsPack(pak,1,0);    // callback id
    pktSendBitsPack(pak,1,db_id);
    pktSendBitsPack(pak,1,type);
    pktSend(&pak,&db_comm_link);
    last_db_error_code = 0;
    dbMessageScanUntil(__FUNCTION__, &perfInfo);
    return last_db_error_code == 0;
}

// 'nice' request to shut down map.  If the dbserver doesn't believe anyone is logging
// in, it will disconnect the link, and otherwise ignore the request
void dbRequestShutdown(void)
{
    Packet        *pak;

    printf("Requesting shutdown from dbserver\n");
    pak = pktCreateEx(&db_comm_link, DBCLIENT_REQUEST_SHUTDOWN);
    pktSend(&pak, &db_comm_link);
}

// *********************************************************************************
//  Relay Queue - record what relays we've sent with timeouts for giving up
// *********************************************************************************

// both for list of pending relays waiting for a response, and for
// recording the trace of the current relay being handled
typedef struct ContainerRelayInfo {
    U32            cmd;
    U32            listid;
    U32            cid;
    U32            user_cmd;
    U32            user_data;
    U32            db_data;
    __int64        time_sent;
} ContainerRelayInfo;

#define CONTAINER_RELAY_TIMEOUT        5        // in seconds

MP_DEFINE(ContainerRelayInfo);
ContainerRelayInfo** g_pendingRelays;

static void QueueRelay(U32 cmd, U32 listid, U32 cid, U32 user_cmd, U32 user_data)
{
    ContainerRelayInfo* cri;
    if (!g_pendingRelays)
    {
        eaCreate(&g_pendingRelays);
        MP_CREATE(ContainerRelayInfo, 10);
    }
    cri = MP_ALLOC(ContainerRelayInfo);
    cri->cmd = cmd;
    cri->listid = listid;
    cri->cid = cid;
    cri->user_cmd = user_cmd;
    cri->user_data = user_data;
    cri->time_sent = timerCpuTicks64();
    eaPush(&g_pendingRelays, cri);
}

static void DequeueRelay(U32 cmd, U32 listid, U32 cid, U32 user_cmd, U32 user_data)
{
    ContainerRelayInfo* cri;
    int i;
    for (i = 0; i < eaSize(&g_pendingRelays); i++)
    {
        cri = g_pendingRelays[i];
        if (cri->cmd == cmd &&
            cri->listid == listid &&
            cri->cid == cid &&
            cri->user_cmd == user_cmd &&
            cri->user_data == user_data)
        {
            eaRemove(&g_pendingRelays, i);
            MP_FREE(ContainerRelayInfo, cri);
            break; // even if we had two items with same info, only dequeue first one
        }
    }
}

void dbCheckFailedRelays(void)
{
    __int64 time = timerCpuTicks64();
    ContainerRelayInfo* cri;
    int i;
    for (i = 0; i < eaSize(&g_pendingRelays); i++)
    {
        cri = g_pendingRelays[i];
        if ((time - cri->time_sent / timerCpuSpeed64()) > CONTAINER_RELAY_TIMEOUT)
        {
            dealWithContainerReceipt(cri->cmd, cri->listid, cri->cid, cri->user_cmd, cri->user_data,
                CONTAINER_ERR_CANT_COMPLETE, "ServerDisconnect", NULL);
            eaRemove(&g_pendingRelays, i);
            MP_FREE(ContainerRelayInfo, cri);
            i--; // continue
        }
    }
}

// *********************************************************************************
//  Container Relays - delivery of command to container, with receipt to sender
// *********************************************************************************

Packet* dbContainerRelayCreate(U32 cmd, U32 listid, U32 cid, U32 user_cmd, U32 user_data)
{
    Packet* ret = pktCreateEx(&db_comm_link, DBCLIENT_CONTAINER_RELAY);
    containerSendTrace(ret, cmd, listid, cid, user_cmd, user_data);
    QueueRelay(cmd, listid, cid, user_cmd, user_data);
    return ret;
}

static ContainerRelayInfo g_relay;

U32 dbRelayLockId(void) { return g_relay.db_data; }

void handleContainerRelay(Packet* pak)
{
    containerGetTrace(pak, &g_relay.cmd, &g_relay.listid, &g_relay.cid, &g_relay.user_cmd, &g_relay.user_data);
    g_relay.db_data        = pktGetBitsPack(pak, 1);
    dealWithContainerRelay(pak, g_relay.cmd, g_relay.listid, g_relay.cid);
}

// use if you need to send additional info with the response to the relay
Packet* dbContainerCreateReceipt(U32 err, char* str)
{
    Packet* ret = pktCreateEx(&db_comm_link, DBCLIENT_CONTAINER_RECEIPT);
    pktSendBitsPack(ret, 1, g_relay.db_data);
    containerSendTrace(ret, g_relay.cmd, g_relay.listid, g_relay.cid, g_relay.user_cmd, g_relay.user_data);
    pktSendBitsPack(ret, 1, err);
    pktSendString(ret, str);
    return ret;
}

// standard response to a relay command
void dbContainerSendReceipt(U32 err, char* str)
{
    Packet* ret = dbContainerCreateReceipt(err, str);
    pktSend(&ret, &db_comm_link);
}

void handleContainerReceipt(Packet* pak)
{
    U32 cmd, listid, cid, user_cmd, user_data, err;
    char* str;

    containerGetTrace(pak, &cmd, &listid, &cid, &user_cmd, &user_data);
    err            = pktGetBitsPack(pak, 1);
    str            = pktGetString(pak);

    DequeueRelay(cmd, listid, cid, user_cmd, user_data);
    dealWithContainerReceipt(cmd, listid, cid, user_cmd, user_data, err, str, pak);
}

// *********************************************************************************
//  Container Reflection - more flexible container update mechanism
// *********************************************************************************

// ok, I admit this is just my version of dbContainerSend.  But I don't want to modify
// handleContainerSet on the dbserver - at least not yet
void dbContainerSendReflection(ContainerReflectInfo*** reflectlist, int list_id, char** data, int* ids, int count, int cmd)
{
    int            i;
    Packet        *pak;

    pak = pktCreateEx(&db_comm_link, DBCLIENT_CONTAINER_REFLECT);
    pktSendBitsPack(pak, 1, list_id);
    pktSendBitsPack(pak, 1, cmd);
    containerReflectSend(pak, reflectlist);

    // list of ids first, then optionally data
    pktSendBitsPack(pak, 1, count);
    for (i = 0; i < count; i++)
    {
        pktSendBitsPack(pak, 1, ids[i]);
    }
    if (cmd != CONTAINER_CMD_DELETE)
    {
        for (i = 0; i < count; i++)
            pktSendString(pak, data[i]);
        // TODO - send a diff, and get dbserver to send full data as necessary..
    }
    pktSend(&pak, &db_comm_link);
}

void handleDbContainerReflect(Packet* pak)
{
    int list_id, cmd, i;
    ContainerReflectInfo** reflectinfo = 0;
    static int *ids = 0;

    if (!ids) eaiCreate(&ids);

    list_id = pktGetBitsPack(pak, 1);
    cmd = pktGetBitsPack(pak, 1);
    containerReflectGet(pak, &reflectinfo);
    
    eaiSetSize(&ids, pktGetBitsPack(pak, 1));
    for (i = 0; i < eaiSize(&ids); i++)
        ids[i] = pktGetBitsPack(pak, 1);

    dealWithContainerReflection(list_id, ids, cmd == CONTAINER_CMD_DELETE, pak, reflectinfo);
}

// *********************************************************************************
//  Container store - more logically in container_store.c, 
//    but the game client doesn't need this function
// *********************************************************************************

void cstoreHandleUpdate(ContainerStore* cstore, ContainerInfo* ci)
{
    void* container = cstoreGetAdd(cstore, ci->id, 1);
    if (!container)
    {
        LOG_OLD_ERR("cstoreHandleUpdate: Couldn't create struct?");
        return;
    }
    cstoreUnpack(cstore, container, ci->data, ci->id);
}


