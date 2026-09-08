#include "svr_base.h"
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/utils/utils.h>
#include "comm_game.h"
#include <utilitieslib/components/earray.h>
#include <utilitieslib/components/StashTable.h>
#include "entity/entity.h"
#include <utilitieslib/utils/process_util.h>
#include <utilitieslib/utils/log.h>
#include "dbcomm/dbcomm.h"
#include "storyarc/mission.h"        // for g_activemission, @todo should probably relocate svrLogPerf

NetLinkList net_links;

ClientLink *clientFromEnt( Entity *ent )
{
    return ent ? ent->client : NULL;
}

void svrLogPerf(void)
{
    // write mapserver statistics upon exit, only if logging level is appropriate
    if(logShouldWrite(LOG_PERFORMANCE,LOG_LEVEL_VERBOSE))
    {
        double upsecs, kernelsecs, usersecs;
        size_t peakwsbytes, peakpfbytes;
        char const* mission_info_string = NULL;

        timeProcessTimes(&upsecs, &kernelsecs, &usersecs);
        procMemorySizes(NULL, &peakwsbytes, NULL, &peakpfbytes);

        // if this was a mission map then reconstruct the mission info string so we
        // have the needed details about the mission
        // @todo, probably should be relocating this function
        if (g_activemission)
        {
            // constructs in a static which will be good until next call
            mission_info_string = MissionConstructInfoString(g_activemission->ownerDbid, g_activemission->supergroupDbid,
                g_activemission->taskforceId, g_activemission->contact, &g_activemission->sahandle,
                g_activemission->seed, g_activemission->baseLevel, &g_activemission->difficulty,
                g_activemission->completeSideObjectives, g_activemission->missionAllyGroup, g_activemission->missionGroup);
        }
        LOG( LOG_PERFORMANCE, LOG_LEVEL_VERBOSE, 0, "pid: %u upsecs:%f kernsecs:%f usersecs:%f wspeakbytes:%u pfpeakbytes:%u mapid: %d%s%s",
            _getpid(), upsecs, kernelsecs, usersecs, (U32)peakwsbytes, (U32)peakpfbytes, db_state.map_id,
            mission_info_string ? " missioninfo: ":"", mission_info_string ? mission_info_string:"");
    }
}

void *clientLinkGetDebugVar(ClientLink *client, char* var){
    if(client && client->hashDebugVars){
        StashElement element;
        
        stashFindElement(client->hashDebugVars, var, &element);
    
        if(element)
            return stashElementGetPointer(element);
    }
        
    return NULL;
}

void *clientLinkSetDebugVar(ClientLink *client, char* var, void *value){
    StashElement element;
    
    if(!client){
        return NULL;
    }
    
    if(!client->hashDebugVars){
        client->hashDebugVars = stashTableCreateWithStringKeys(100, StashDeepCopyKeys);
    }
    
    stashFindElement(client->hashDebugVars, var, &element);
    
    if(element){
        stashElementSetPointer(element, value);
    }else{
        stashAddPointer(client->hashDebugVars, var, value, false);
    }
    
    return value;
}

typedef struct OpenEntry {
    ClientSubPacketLogEntry*    entry;
    Packet*                        pak;
    int                            dbID;
} OpenEntry;

MP_DEFINE(ClientSubPacketLogEntry);
MP_DEFINE(OpenEntry);

static ClientSubPacketLogEntry        staticCmdCounterEntry[CLIENT_PAKLOG_COUNT];
static StashTable                    dbIDToPacketLogTable;
static int                            clientPacketLogEnabled;
static int                            clientPacketLogPaused;

static struct {
    OpenEntry*        entries;
    int                count;
    int                maxCount;
} openEntries;

static struct {
    CmdStats*        statsArray;
    U32                count;
    U32                maxCount;
} cmdStats[CLIENT_PAKLOG_COUNT];

int clientSubPacketLogEntryAllocatedCount(){
    return (int)mpGetAllocatedCount(MP_NAME(ClientSubPacketLogEntry));
}

CmdStats* getCmdStats(int index, U32 cmd, int create){
    CmdStats* stats;
    
    if(index < 0 || index >= CLIENT_PAKLOG_COUNT){
        return NULL;
    }
    
    if(cmd >= cmdStats[index].count){
        U32 oldCount = cmdStats[index].count;
        
        dynArrayAdd(&cmdStats[index].statsArray,
                    sizeof(cmdStats[index].statsArray[0]),
                    &cmdStats[index].count,
                    &cmdStats[index].maxCount,
                    cmd + 1 - cmdStats[index].count);
                    
        for(; oldCount < cmdStats[index].count; oldCount++){
            ZeroStruct(cmdStats[index].statsArray + oldCount);
        }
    }
    
    stats = cmdStats[index].statsArray + cmd;
    
    return stats;
}

int getCmdStatsCount(int index){
    if(index < 0 || index >= CLIENT_PAKLOG_COUNT)
        return 0;
    return cmdStats[index].count;
}

ClientSubPacketLogEntry* createClientSubPacketLogEntry(){
    MP_CREATE(ClientSubPacketLogEntry, 10000);

    if(mpGetAllocatedCount(MP_NAME(ClientSubPacketLogEntry)) >= 10000){
        return NULL;
    }
    
    return MP_ALLOC(ClientSubPacketLogEntry);
}

static void removeOpenEntry(int i){
    if(i == openEntries.count - 1){
        openEntries.count--;
        
        for(i--; i >= 0 && openEntries.entries[i].entry == NULL; i--){
            openEntries.count--;
        }        
    }
    else if(i >= 0 && i < openEntries.count){
        openEntries.entries[i].entry = NULL;
    }
}

static void destroyClientSubPacketLogEntry(ClientSubPacketLogEntry* entry){
    int i;
    
    for(i = 0; i < openEntries.count; i++){
        if(openEntries.entries[i].entry == entry){
            removeOpenEntry(i);
            break;
        }
    }

    MP_FREE(ClientSubPacketLogEntry, entry);
}

ClientPacketLog* clientGetPacketLog(Entity* e, int dbID, int create){
    ClientPacketLog* log;
    
    if(!clientPacketLogEnabled){
        return NULL;
    }
    
    if(e){
        dbID = e->db_id;
    }
    
    if(dbID <= 0){
        return NULL;
    }
    
    if(!dbIDToPacketLogTable){
        if(!create){
            return NULL;
        }
        
        dbIDToPacketLogTable = stashTableCreateInt(1000);
    }
    
    if(!stashIntFindPointer(dbIDToPacketLogTable, dbID, &log)){
        if(create){
            log = calloc(1, sizeof(*log));
            log->name = e ? strdup(e->name) : NULL;
            log->time_created = timerSecondsSince2000();        
            stashIntAddPointer(dbIDToPacketLogTable, dbID, log, false);
            
            if(!e){
                e = entFromDbId(dbID);
            }
            
            if(e && e->client && e->client->link){
                e->client->link->packetSentCallback = clientPacketSentCallback;
            }
        }else{
            log = NULL;
        }
    }
    
    return log;
}

static void destroySubPacketList(ClientSubPacketLogEntry** pakList){
    while(*pakList){
        ClientSubPacketLogEntry* next = (*pakList)->next;
        destroyClientSubPacketLogEntry(*pakList);
        *pakList = next;
    }
}

static void clearClientPacketLog(ClientPacketLog* log){
    int i;
    
    if(!log){
        return;
    }
    
    for(i = 0; i < ARRAY_SIZE(log->subPackets); i++){
        destroySubPacketList(&log->subPackets[i]);
    }
}

static int clearClientPacketLogCallback(void* userData, StashElement element){
    clearClientPacketLog(stashElementGetPointer(element));
    return 1;
}

static void destroyClientPacketLog(ClientPacketLog* log){
    if(!log){
        return;
    }
    
    clearClientPacketLog(log);
    
    free(log);
}

static void destroyClientPacketLogAdapter(void* arg0)
{
    destroyClientPacketLog((ClientPacketLog*)arg0);
}

void clientPacketLogSetEnabled(int on, int dbID){
    if(on > 0){
        clientPacketLogEnabled = 1;
    
        if(dbID > 0){
            clientGetPacketLog(NULL, dbID, 1);
        }
    }else{
        if(dbID > 0){
            ClientPacketLog* log = clientGetPacketLog(NULL, dbID, 0);
            
            if(log){
                if(on < 0){
                    clearClientPacketLog(log);
                }else{
                    destroyClientPacketLog(log);
                    stashIntRemovePointer(dbIDToPacketLogTable, dbID, NULL);
                }
            }
        }else{
            if(on < 0){
                stashForEachElementEx(dbIDToPacketLogTable, clearClientPacketLogCallback, NULL);
            }else{
                clientPacketLogEnabled = 0;

                stashTableDestroyEx(dbIDToPacketLogTable, NULL, destroyClientPacketLogAdapter);
                dbIDToPacketLogTable = 0;
            }
        }

        if(!mpGetAllocatedCount(MP_NAME(ClientSubPacketLogEntry))){
            mpFreeAllocatedMemory(MP_NAME(ClientSubPacketLogEntry));
        }
    }
}

int clientPacketLogIsEnabled(int dbID){
    if(dbID > 0){
        return clientGetPacketLog(NULL, dbID, 0) ? 1 : 0;
    }    
    
    return clientPacketLogEnabled;
}

void clientPacketLogSetPaused(int set){
    clientPacketLogPaused = set ? 1 : 0;
}

int clientPacketLogIsPaused(){
    return clientPacketLogPaused;
}

void clientSetPacketLogsSent(int index, Packet* pak, Entity* e){
    ClientPacketLog* log = clientGetPacketLog(e, 0, 0);
    ClientSubPacketLogEntry* entry;
    U32 parent_pak_bit_count = pak->stream.bitLength;
    
    if(index < 0 || index >= CLIENT_PAKLOG_COUNT){
        return;
    }
    
    if(!log){
        return;
    }

    entry = log->subPackets[index];

    // Check if there were no unsent entries.
    
    if(!entry || entry->send_abs_time){
        // Stuff a fake packet in if there's no packets or the first packet was already set.
        
        clientSubPacketLogEnd(index, clientSubPacketLogBegin(index, pak, e, ~0));
        entry = log->subPackets[index];
    }

    for(; entry && !entry->send_abs_time; entry = entry->next){
        entry->send_abs_time = ABS_TIME;
        entry->send_server_frame = global_state.global_frame_count;
        entry->parent_pak_bit_count = parent_pak_bit_count;
        entry->parent_pak_id = pak->id;
    }
}

void* clientSubPacketLogBegin(int index, Packet* pak, Entity* e, U32 cmd){
    ClientPacketLog* log = clientGetPacketLog(e, 0, 0);
    ClientSubPacketLogEntry* entry;
    OpenEntry* openEntry;
    
    if(index < 0 || index >= CLIENT_PAKLOG_COUNT){
        return 0;
    }
    
    if(!log || clientPacketLogIsPaused()){
        entry = &staticCmdCounterEntry[index];
    }else{
        entry = createClientSubPacketLogEntry();
        
        if(!entry){
            return 0;
        }

        // Add to the list.
        
        entry->next = log->subPackets[index];
        log->subPackets[index] = entry;
    }
    
    entry->client_connect_time = e ? e->birthtime : 0;
    entry->create_abs_time = ABS_TIME;
    entry->cmd = cmd;
    entry->bitCount = pak->stream.cursor.byte * 8 + pak->stream.cursor.bit;
    
    openEntry = dynArrayAdd(&openEntries.entries, sizeof(openEntries.entries[0]), &openEntries.count, &openEntries.maxCount, 1);
    
    ZeroStruct(openEntry);
    
    openEntry->entry = entry;
    openEntry->pak = pak;
    openEntry->dbID = e ? e->db_id : 0;
    
    return (void*)(intptr_t)openEntries.count;
}

void clientSubPacketLogEnd(int index, void* indexParam){
    int openEntryIndex = ((int)(intptr_t)indexParam) - 1;
    ClientSubPacketLogEntry* entry;
    OpenEntry openEntry;
    CmdStats* stats;
    Packet* pak;
    
    if(index < 0 || index >= CLIENT_PAKLOG_COUNT){
        return;
    }
        
    if(openEntryIndex < 0 || openEntryIndex >= openEntries.count){
        return;
    }
    
    openEntry = openEntries.entries[openEntryIndex];
    
    removeOpenEntry(openEntryIndex);
    
    if(!openEntry.entry || !openEntry.pak){
        return;
    }
    
    entry = openEntry.entry;
    pak = openEntry.pak;
    entry->bitCount = pak->stream.cursor.byte * 8 + pak->stream.cursor.bit - entry->bitCount;
    
    // Update the global stats.
    
    if(entry->cmd != ~0){
        stats = getCmdStats(index, entry->cmd, 1);
        
        if(stats){
            if(!stats->sentCount){
                stats->bitCountMin = entry->bitCount;
                stats->bitCountMax = entry->bitCount;
                stats->dbIDMin = openEntry.dbID;
                stats->dbIDMax = openEntry.dbID;
            }
            else if(entry->bitCount > stats->bitCountMax){
                stats->bitCountMax = entry->bitCount;
                stats->dbIDMax = openEntry.dbID;
            }
            else if(entry->bitCount < stats->bitCountMin){
                stats->bitCountMin = entry->bitCount;
                stats->dbIDMin = openEntry.dbID;
            }

            stats->sentCount++;
            stats->bitCountTotal += entry->bitCount;
        }
    }
}

static int gatherDBIDs(void* dbIDsData, StashElement element){
    int** dbIDs = (int**)dbIDsData;
    int iKey = stashElementGetIntKey(element);
    if(iKey) {
        eaiPush(dbIDs, iKey);
    }
    return 1;
}

void clientPacketLogGetAllDBIDS(int** dbIDs){
    eaiSetSize(dbIDs, 0);
    
    stashForEachElementEx(dbIDToPacketLogTable, gatherDBIDs, dbIDs);
}

void clientPacketSentCallback(NetLink* link, Packet* pak, int realPakSize, int result){
    ClientLink* client = link->userData;
    ClientSubPacketLogEntry* entry;
    ClientPacketLog* log;
    Entity* e;
        
    if(!client || !client->entity || clientPacketLogIsPaused()){
        return;        
    }
    
    e = client->entity;
    
    log = clientGetPacketLog(e, 0, 0);
    
    if(!log){
        return;
    }
    
    entry = createClientSubPacketLogEntry();
    
    if(!entry){
        return;
    }
    
    entry->cmd = ~0;
    entry->parent_pak_id = pak->id;
    entry->parent_pak_bit_count = realPakSize * 8;
    entry->create_abs_time = timerCpuTicks();
    entry->send_server_frame = pak->retransCount;
    
    entry->next = log->subPackets[CLIENT_PAKLOG_RAW_OUT];
    log->subPackets[CLIENT_PAKLOG_RAW_OUT] = entry;
}

// this creates getServerCmdName
#define SERVERCMDNAME_FUNCTION
#include "comm_game.h"
#undef SERVERCMDNAME_FUNCTION

