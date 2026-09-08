#include "mapcrashreport.h"
#include "dbserver/servercfg.h"
#include <utilitieslib/network/sock.h>
#include <utilitieslib/stdtypes.h>
#include "comm_backend.h"
#include <utilitieslib/utils/error.h>
#include "container.h"
#include <utilitieslib/network/netio.h>
#include <utilitieslib/network/net_linklist.h>
#include <stdio.h>
#include "dbinit.h"
#include "svrmoncomm.h"
#include "launchercomm.h"
#include "dbdispatch.h"
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/log.h>
#include "turnstileDb.h"

typedef struct
{
    int        id;
    int        process_id;
    int        cookie;
    char    *message;
    int        ipaddr;
} CrashMap;

#define MAX_CRASHED_MAPS 1024
static CrashMap            crashmap_queue[MAX_CRASHED_MAPS];
static volatile int        crashmap_queue_start,crashmap_queue_end;

static CrashMap *getCrashedMap()
{
    CrashMap    *entry;

    if (crashmap_queue_start == crashmap_queue_end)
        return 0;
    entry = &crashmap_queue[crashmap_queue_start];
    crashmap_queue_start = (crashmap_queue_start + 1) & (MAX_CRASHED_MAPS-1);
    return entry;
}

static int putCrashedMap(CrashMap *entry)
{
    int        t;

    t = (crashmap_queue_end + 1) & (MAX_CRASHED_MAPS-1);
    if (t == crashmap_queue_start)
        return 0;
    crashmap_queue[crashmap_queue_end] = *entry;
    crashmap_queue_end = t;
    return 1;
}

int crashmap_sock;

static bool isDuplicateCrash( U32 ip, U32 mapid, U32 pid )
{
    int i;
    static struct {
        U32 ip, mapid, pid;
    } recent_crashes[4], temp, temp2;
    temp.ip = ip;
    temp.mapid = mapid;
    temp.pid = pid;
    for (i=0; i<ARRAY_SIZE(recent_crashes); i++) 
    {
        if (recent_crashes[i].ip == ip &&
            recent_crashes[i].mapid == mapid &&
            recent_crashes[i].pid == pid)
        {
            recent_crashes[i] = temp;
            LOG_OLD_ERR( "MapServer Crashed (Duplicate?) %s", makeIpStr(ip));
            return true;
        } else {
            // Put temp there, float next entry into temp
            temp2 = recent_crashes[i];
            recent_crashes[i] = temp;
            temp = temp2;
        }
    }
    return false;
}

static unsigned __stdcall mapCrashThread( LPVOID lpParam )
{
    EXCEPTION_HANDLER_BEGIN
    int        s;
    struct sockaddr_in    addr_in,incomingIP;
    int addrLen = sizeof(incomingIP);
    CrashMap    map;

    crashmap_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    sockSetAddr(&addr_in,htonl(INADDR_ANY),DEFAULT_DBCRASHMAP_PORT);
    if (!sockBind(crashmap_sock,&addr_in)) {
        printf("Can't bind port %d, Mapserver crash autodetection/delinking will not function.\n",DEFAULT_DBCRASHMAP_PORT);
        printf("Either there is another DbServer.exe running or something is left over from a\n");
        printf("  previous crash.  If this problem persists, kill any instances of processes\n");
        printf("  that look like: vs7jit.exe\n");
        return -1;
    }

    for(;;)
    {
        if (listen(crashmap_sock,15)==0)
        {
            addrLen = sizeof(incomingIP);
            s = accept(crashmap_sock, (SOCKADDR*)&incomingIP, &addrLen);
            if (s==INVALID_SOCKET)
                continue;

            if (recv(s,(void*)&map, sizeof(int)*3, 0) == sizeof(int)*3)
            {
                int len;
                // Check for duplicate of recent caches
                if (!isDuplicateCrash(incomingIP.sin_addr.S_un.S_addr, map.id, map.process_id))
                {
                    launcherCommRecordCrash(incomingIP.sin_addr.S_un.S_addr);

                    printf("map_id %d pid %d  crashed\n",map.id,map.process_id);
                    // Attempt to get crash message
                    if (recv(s,(void*)&len, sizeof(int), 0) == sizeof(int) && len >0 && len < 100000) {
                        // Only receive the message if it's at least a byte, and less than 100K (in case of garbage data)
                        map.message = calloc(len+1,1);
                        if (recv(s, map.message, len, 0)==len) {
                            // Succeeded!
                        } else {
                            free(map.message);
                            map.message = NULL;
                        }
                    }
                    map.ipaddr = incomingIP.sin_addr.S_un.S_addr;
                    putCrashedMap(&map);
                }
                memset(&map, 0, sizeof(map));
            }
            closesocket(s);
        }
    }
    EXCEPTION_HANDLER_END
    return 0;
}

void startMapCrashReportThread()
{
    CreateThread(0,0,mapCrashThread,0,0,0);
}

void addCrashedMap(MapCon *map_con, char * message, char *status, int ipaddr)
{
    static int crashed_map_id=CRASHED_MAP_BASE_ID;
    MapCon *crashed;

    // Log
    if (map_con && message)
    {
        LOG(LOG_CRASH, LOG_LEVEL_IMPORTANT, LOG_DEFAULT, "%s\t%d\t%d\t%s", 
            makeIpStr(ipaddr), map_con->remote_process_info.process_id, map_con->num_players+map_con->num_players_connecting, escapeString(message) );
    }
    else if (message) 
    {
        // no map_con
        LOG(LOG_CRASH, LOG_LEVEL_IMPORTANT, LOG_DEFAULT, "%s\tUNKNOWN\tUNKNOWN\t%s", makeIpStr(ipaddr), escapeString(message));
    } 
    else if (map_con)
    {
        // no message
        LOG(LOG_CRASH, LOG_LEVEL_IMPORTANT, LOG_DEFAULT, "%s\t%d\t%d\tUNKNOWN", makeIpStr(ipaddr), map_con->remote_process_info.process_id, map_con->num_players+map_con->num_players_connecting);
    } 
    else 
    {
        // Nothing at all
        LOG(LOG_CRASH, LOG_LEVEL_IMPORTANT, LOG_DEFAULT, "%s\tUNKNOWN\tUNKNOWN\tUNKNOWN", makeIpStr(ipaddr));
    }

    crashed = containerAlloc(crashedmap_list,crashed_map_id++);
    assert(crashed);
    // Copy relevant fields
    crashed->raw_data = message;
    crashed->container_data_size = message ? (int)(strlen(message) + 1) : 0;
    if (map_con) {
#define COPY(field) crashed->field = map_con->field
        COPY(on_since);
        COPY(remote_process_info);
        COPY(seconds_since_update);
        COPY(ip_list[0]);
        COPY(ip_list[1]);
        COPY(udp_port);
        strcpy(crashed->map_name, map_con->map_name);
        strcpy(crashed->mission_info, map_con->mission_info);
        COPY(map_running);
        COPY(num_players);
        COPY(num_players_connecting);
        COPY(num_monsters);
        COPY(num_ents);
        COPY(launcher_id);
        COPY(dontAutoStart);
        COPY(transient);
        COPY(introZone);
        COPY(is_static);
        crashed->base_map_id = map_con->id;
    } else {
        crashed->ip_list[0] = ipaddr;
        strcpy(crashed->remote_process_info.hostname, makeIpStr(ipaddr));
    }
    crashed->active = 1;

    assert(strlen(status) < ARRAY_SIZE(crashed->status));
    strcpy(crashed->status, status);
    svrMonUpdate();
}

void handleCrashedMapReport(int id, int process_id, int cookie, char *message, int ipaddr)
{
    MapCon        *map_con;
    int            pid;
    map_con = containerPtr(dbListPtr(CONTAINER_MAPS),id);
    pid = map_con? map_con->remote_process_info.process_id: 0;
    if (map_con && (!pid || pid == process_id) && (cookie == map_con->cookie || cookie == -map_con->cookie))
    {
        if (map_con && !map_con->is_static)
        {
            if (map_con->mission_info && map_con->mission_info[0] == 'E')
                turnstileDBserver_handleCrashedIncarnateMap(id);
        }

        // Move map to CrashedMaps and delink!
        addCrashedMap(map_con, message, "CRASHED", ipaddr);
        //printf("delinking map %d\n",map_con->id);
        //netDiscardDeadLink(map_con->link);
        dbDelinkMapserver(map_con->id);
    } else {
        addCrashedMap(NULL, message, "CRASHED", ipaddr);
    }
}

void delinkCrashedMaps()
{
    CrashMap    *map;

    while(map = getCrashedMap())
    {
        handleCrashedMapReport(map->id, map->process_id, map->cookie, map->message, map->ipaddr);
        map->message = NULL;
    }
}
