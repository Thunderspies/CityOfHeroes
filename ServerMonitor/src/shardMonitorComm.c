#include <comm_backend.h>
#include <svrmoncomm.h>
#include <utilitieslib/utils/structNet.h>
#include <utilitieslib/utils/textparser.h>
#include <utilitieslib/utils/timing.h>

#include "serverMonitor.h"
#include "serverMonitorCommon.h"
#include "serverMonitorCmdRelay.h"
#include "serverMonitorNet.h"
#include "shardMonitorComm.h"

typedef struct
{
    NetLink    *link;
    U32        full_update:1;
    U32        shardmon_ready:1;
} SvrMonShardMonClientLink;

#define MAX_CLIENTS 50

NetLinkList        svrMonShardMonLinks;

int svrMonShardMonConnectCallback(NetLink *link)
{
    SvrMonShardMonClientLink    *client;

    client = link->userData;
    client->link = link;
    client->full_update = true;
    return 1;
}

int svrMonShardMonDelCallback(NetLink *link)
{
    SvrMonShardMonClientLink    *client = link->userData;
    return 1;
}

void svrMonShardMonHandleConnect(ServerMonitorState *state, SvrMonShardMonClientLink *client, Packet *pak)
{
    static int init=0;
    static U32 server_crcs[2];
    U32 crc;
    int i;
    Packet *pak_out;
    if (!init) {
        init = 1;
        server_crcs[0] = 20041006;
        server_crcs[1] = SVRMON_PROTOCOL_MAJOR_VERSION;
    }

    pak_out = pktCreateEx(client->link, SVRMONSHARDMON_CONNECT);

    for (i=0; i<ARRAY_SIZE(server_crcs); i++) {
        crc = pktGetBits(pak, 32);
        if (crc != server_crcs[i]) {
            pktSendBits(pak_out, 1, 0);
            pktSendBitsPack(pak_out, 1, i);
            pktSendBits(pak_out, 32, server_crcs[i]);
            pktSendBits(pak_out, 32, crc);
            pktSend(&pak_out, client->link);
            return;
        }
    }
    client->shardmon_ready = 1;
    pktSendBits(pak_out, 1, 1);
    pktSend(&pak_out, client->link);
}

void svrMonShardMonHandleDbMsg(ServerMonitorState *state, SvrMonShardMonClientLink *client, Packet *pak)
{
    Packet *pak_out;
    if (!svrMonConnected(state))
        return;
    pak_out = pktCreateEx(&state->db_link, DBSVRMON_RELAYMESSAGE);
    pktSendString(pak_out, pktGetString(pak));
    pktSendString(pak_out, pktGetString(pak));
    pktSend(&pak_out, &state->db_link);
}


int svrMonShardMonHandleClientMsg(Packet *pak,int cmd, NetLink *link)
{
    SvrMonShardMonClientLink    *client = link->userData;
    ServerMonitorState *state = &g_state;

    assert(!g_shardmonitor_mode); // This only works in stand-alone mode!

    if (!client)
        return 0;

    switch(cmd)
    {
    case SVRMONSHARDMON_CONNECT:
        svrMonShardMonHandleConnect(state, client, pak);
        break;
    case SVRMONSHARDMON_RELAYMESSAGE:
    case SVRMONSHARDMON_RECONCILE_LAUNCHER:
        svrMonShardMonHandleDbMsg(state, client, pak);
        break;
    case SVRMONSHARDMON_RELAY_START_ALL:
        cmdRelayCmdToAllClients((ListViewCallbackFunc) onRelayStartAll);
        break;
    case SVRMONSHARDMON_RELAY_STOP_ALL:
        cmdRelayCmdToAllClients((ListViewCallbackFunc) onRelayStopAll);
        break;
    case SVRMONSHARDMON_RELAY_KILL_ALL_MAPSERVER:
        cmdRelayCmdToAllClients((ListViewCallbackFunc) onRelayKillAllMapserver);
        break;
    case SVRMONSHARDMON_RELAY_KILL_ALL_LAUNCHER:
        cmdRelayCmdToAllClients((ListViewCallbackFunc) onRelayKillAllLauncher);
        break;
    case SVRMONSHARDMON_RELAY_START_LAUNCHER:
        cmdRelayCmdToAllClients((ListViewCallbackFunc) onRelayStartLauncher);
        break;
    case SVRMONSHARDMON_RELAY_START_DBSERVER:
        cmdRelayCmdToAllClients((ListViewCallbackFunc) onRelayStartDbServer);
        break;
    case SVRMONSHARDMON_RELAY_CANCEL_ALL:
        cmdRelayCmdToAllClients((ListViewCallbackFunc) onRelayCancelAll);
        break;
    case SVRMONSHARDMON_RELAY_APPLY_PATCH:
        cmdRelayAutoApplyPatch(pak);
        break;
    default:
        break;
    }
    return 1;
}


void svrMonShardMonCommInit(void)
{
    if (!g_primaryServerMonitor)
        return;
    netLinkListAlloc(&svrMonShardMonLinks,MAX_CLIENTS,sizeof(SvrMonShardMonClientLink),svrMonShardMonConnectCallback);
    netInit(&svrMonShardMonLinks,0,DEFAULT_SHARDMON_PORT);
    svrMonShardMonLinks.destroyCallback = svrMonShardMonDelCallback;
    NMAddLinkList(&svrMonShardMonLinks, svrMonShardMonHandleClientMsg);
}

void svrMonShardMonSendDiff(ServerMonitorState *state, SvrMonShardMonClientLink *client, ServerStats *oldVersion)
{
    Packet *pak = pktCreateEx(client->link, SVRMONSHARDMON_STATS);
    int full_update = !oldVersion;
    pktSendBits(pak, 1, full_update); // full update
    if (full_update) {
        sdPackParseInfo(ServerStatsNetInfo, pak);
    }
    sdPackDiff(ServerStatsNetInfo, pak, oldVersion, &state->stats, false, true, false, 0, 0);
    pktSend(&pak, client->link);
}

void svrMonShardMonCommSendUpdates(void)
{
    int i;
    SvrMonShardMonClientLink    *client;
    static int timer=0;
    static ServerStats oldStats = {0};
    ServerMonitorState *state = &g_state;

    assert(!g_shardmonitor_mode);


    if (timer == 0) {
        timer = timerAlloc();
    } else {
        if (timerElapsed(timer) < 1.0) { // Once a second
            return;
        }
    }

    timerStart(timer);

    for(i=svrMonShardMonLinks.links->size-1;i>=0;i--)
    {
        client = ((NetLink*)svrMonShardMonLinks.links->storage[i])->userData;
        assert(client->link == svrMonShardMonLinks.links->storage[i]);

        if (!client->shardmon_ready)
            continue;

        if (qGetSize(client->link->sendQueue2) > 1000) {
            printf("Dropping shardMon link, 1000 packets in output buffer");
            netRemoveLink(client->link);
        } else {
            if (client->full_update || sdHasDiff(ServerStatsNetInfo, &oldStats, &state->stats)) {
                svrMonShardMonSendDiff(state, client, client->full_update?NULL:&oldStats);
                client->full_update = false;
            }
        }
    }

    sdCopyStruct(ServerStatsNetInfo, &state->stats, &oldStats);

    NMMonitor(1); // Accept incoming connectiongs
}
