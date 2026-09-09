// TurnstileServer.c
//

#include <utilitieslib/stdtypes.h>
#include <serverlib/serverlib.h>
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/UtilsNew/Str.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/utils/error.h>
#include "dbserver/servercfg.h"      // for MAX_DBSERVER
#include <utilitieslib/network/netio.h>
#include "comm_backend.h"
#include "entity/teamCommon.h"
#include "chatdefs.h"
#include "turnstile/TurnstileServerCommon.h"
#include "TurnstileServerGroup.h"
#include "TurnstileServerMsg.h"
#include "TurnstileServerEvent.h"
#include "TurnstileServer.h"

static NetLink db_links[MAX_DBSERVER];
static int retryTimers[MAX_DBSERVER];                // timers used to throttle reconnect attempts

static int register_ack = 0;

int disableVisitorTech = 0;

#ifdef _snprintf
#undef _snprintf
#endif
#define _snprintf(buff, size, fmt, ...) _snprintf_s((buff), (size), (size) - 1, (fmt), __VA_ARGS__);

// Convert a time in seconds to a reasonable display string.
static char *timeToString(int seconds)
{
    int hours;
    int minutes;
    static char buffer[16];    

    // Do something intelligent with a negative number
    if (seconds < 0)
    {
        seconds = 0;
    }

    // Over a minute, initially split to minutes and seconds
    if (seconds >= 60)
    {
        minutes = seconds / 60;
        seconds %= 60;
        // If we're over an hour split minutes into hours and minutes
        if (minutes >= 60)
        {
            hours = minutes / 60;
            minutes %= 60;
            _snprintf(buffer, sizeof(buffer), "%d:%02d:%02d", hours, minutes, seconds);
        }
        else
        {
            _snprintf(buffer, sizeof(buffer), "%d:%02d", minutes, seconds);
        }
    }
    // under a minute, format as just seconds
    else
    {
        _snprintf(buffer, sizeof(buffer), "%d", seconds);
    }
    buffer[sizeof(buffer) - 1] = 0;
    return buffer;
}

static void printf_timestamp(char *fmt, ...)
{
    char date_buf[100];
    va_list argptr;

    timerMakeDateString(date_buf);
    printf("%s ", date_buf);

    va_start(argptr, fmt);
    vprintf(fmt, argptr);
    va_end(argptr);
}

int dbserverCount()
{
    return MAX_DBSERVER;
}

NetLink *dbserverLink(int index)
{
    return index >= 0 && index < MAX_DBSERVER && db_links[index].connected ? &db_links[index] : NULL;
}

NetLink *dbserverIdentToLink(int shardID)
{
    int i;

    for (i = eaSize(&turnstileConfigCfg.dbservers) - 1; i >= 0; i--)
    {
        if (turnstileConfigCfg.dbservers[i]->shardID == shardID)
        {
            return db_links[i].connected ? &db_links[i] : NULL;
        }
    }
    return NULL;
}

static DBServerCfg *curDBServer = NULL;

static void turnstileServer_handleRegisterAck(Packet *pak)
{
    char *shardName;

    shardName = pktGetString(pak);

    if (curDBServer != NULL)
    {
        if (stricmp(curDBServer->shardName, shardName) == 0)
        {
            curDBServer->active = 1;
        }
        else
        {
            printf("**** Error mismatched shardname for dbserver at %s: %s != %s\n", curDBServer->serverName, curDBServer->shardName, shardName);
            printf("     Please check turnstile_servers.cfg and servers.cfg for this shard\n");
        }
    }
    else
    {
        // complain
    }
}

static int turnstileServer_HandleMsg(Packet *pak, int cmd, NetLink *link)
{
    switch(cmd)
    {
        xcase DBSERVER_REGISTER_ACK:
            register_ack = 1;
            turnstileServer_handleRegisterAck(pak);
        xcase DBSERVER_QUEUE_FOR_EVENTS:
            turnstileServer_handleQueueForEvents(pak, link);
        xcase DBSERVER_REMOVE_FROM_QUEUE:
            turnstileServer_handleRemoveFromQueue(pak, link);
        xcase DBSERVER_EVENT_READY_ACK:
            turnstileServer_handleEventReadyAck(pak);
        xcase DBSERVER_EVENT_RESPONSE:
            turnstileServer_handleEventResponse(pak);
        xcase DBSERVER_MAP_ID:
            turnstileServer_handleMapID(pak);
        xcase DBSERVER_DEBUG_SHARD_XFER_OUT:
            turnstileServer_handleShardXferOut(pak, link);
        xcase DBSERVER_DEBUG_SHARD_XFER_BACK:
            turnstileServer_handleShardXferBack(pak, link);
        xcase DBSERVER_COOKIE_REQUEST:
            turnstileServer_handleCookieRequest(pak);
        xcase DBSERVER_COOKIE_REPLY:
            turnstileServer_handleCookieReply(pak);
        xcase DBSERVER_GROUP_UPDATE:
            turnstileServer_handleGroupUpdate(pak);
        xcase DBSERVER_REQUEST_QUEUE_STATUS:
            turnstileServer_handleRequestQueueStatus(pak, link);
        xcase DBSERVER_CLOSE_INSTANCE:
            turnstileServer_handleCloseInstance(pak);
        xcase DBSERVER_REJOIN_INSTANCE:
            turnstileServer_handleRejoinRequest(pak, link);
        xcase DBSERVER_PLAYER_LEAVE:
            turnstileServer_handlePlayerLeave(pak);
        xcase DBSERVER_INCARNATETRIAL_COMPLETE:
            turnstileServer_handleIncarnateTrialComplete(pak);
        xcase DBSERVER_TS_CRASHEDMAP:
            turnstileServer_handleCrashedMap(pak);
        xcase DBSERVER_QUEUE_FOR_SPECIFIC_MISSION_INSTANCE:
            turnstileServer_handleQueueForSpecificMissionInstance(pak, link);
        xcase DBSERVER_TS_ADD_BAN_DBID:
            turnstileServer_handleAddBanID(pak);
        xdefault:
            printf("Unknown command %d\n", cmd);
            return 0;
    }
    return 1;
}

// Start a connection attempt to a Dbserver.  This is non-blocking.
static void connectDbStart(char *dbServerName, NetLink *netLink)
{
    printf_timestamp("Starting connection to %s\n", dbServerName);
    netConnectAsync(netLink, dbServerName, DEFAULT_DBTURNSTILE_PORT, NLT_TCP, 10.0f);
}

// Once a connection attempt has been started, call this each loop to see if the connection has succeeded.
static int connectDbPoll(char *dbServerName, NetLink *netLink)
{
    int i;
    int n;
    int ret;
    Packet *pak;
    DBServerCfg *dbserver;

    ret = netConnectPoll(netLink);

    // If anything other than success ....
    if (ret != 1)
    {
        // -1 means failure
        if (ret == -1)
        {
            // Print a message.  The NetLink will be left in a disconnected state, which we detct in the main loop and react as needed.
            printf_timestamp("Connection to %s failed\n", dbServerName);
        }
        // Anything else (i.e. zero) means it's still in progress so do nothing.
        return ret;
    }

    pak = pktCreateEx(netLink, TURNSTILE_REGISTER);

    pktSendBitsAuto(pak, timerSecondsSince2000());

    n = eaSize(&turnstileConfigCfg.dbservers);
    pktSendBitsAuto(pak, n);
    for (i = 0; i < n; i++)
    {
        dbserver = turnstileConfigCfg.dbservers[i];
        pktSendString(pak, dbserver->serverName);
        pktSendString(pak, dbserver->serverPublicName);
        pktSendString(pak, dbserver->shardName);
        pktSendBitsAuto(pak, dbserver->shardID);
        pktSendBitsAuto(pak, dbserver->authShardID);
        pktSendBitsAuto(pak, dbserver->serverAddr);
        pktSendBitsAuto(pak, dbserver->serverPort);
    }

    pktSend(&pak, netLink);

    register_ack = 0;
    // This is a blocking call, but unless there's a major problem, it shouldn't take more than a second or two.
    ret = netLinkMonitorBlock(netLink, DBSERVER_REGISTER_ACK, turnstileServer_HandleMsg, 10000);
    if (ret)
    {
        if (register_ack)
        {
            printf_timestamp("Connected to %s\n", dbServerName);
            register_ack = 0;
        }
        else
        {
            printf_timestamp("Handshake fail with DbServer @ %s\n", dbServerName);
            clearNetLink(netLink);
            ret = -1;
        }
    }
    else
    {
        printf_timestamp("Handshake check to %s returned 0\n", dbServerName);
        clearNetLink(netLink);
        // Artificially turn this into an error.  That way the five second delay code will cut in and prevent this from spamming over and over.
        // The cause of spam was tracked down and fixed.  However this still wants to be considered an error, therefore we will leave this returning -1
        ret = -1;
    }
    return ret;
}

// Init routine, just read in the config for now
static void turnstileServer_init(void)
{
    int i;

    loadstart_printf("Loading configuration...");
    if (!turnstileParseTurnstileConfig(TURNSTILE_LOAD_CONFIG | TURNSTILE_LOAD_DEF))
    {
        FatalErrorf("Error: failure loading config and def files");
        exit(1);
    }
    if (eaSize(&turnstileConfigCfg.dbservers) == 0)
    {
        FatalErrorf("Error: no shards specified in turnstile_server.cfg");
        exit(2);
    }
    //if (eaSize(&turnstileConfig.missions) == 0)
    //{
    //    FatalErrorf("Error: no missions specified in turnstile_server.def");
    //    exit(3);
    //}

    loadend_printf("");

    if (eaSize(&turnstileConfigCfg.dbservers) > MAX_DBSERVER)
    {
        printf("Warning: Too many shards specified in turnstile_server.cfg: %d, max is %d\n", eaSize(&turnstileConfigCfg.dbservers), MAX_DBSERVER);
        printf("Truncating list\n");
        while (eaSize(&turnstileConfigCfg.dbservers) > MAX_DBSERVER)
        {
            eaPop(&turnstileConfigCfg.dbservers);
        }
    }

    // Initialize the connection retry timers.
    for (i = 0; i < MAX_DBSERVER; i++)
    {
        retryTimers[i] = 0;
    }

    // Set up the grouping system.
    GroupInit();
    InstanceInit();
    rule30Seed(timerSecondsSince2000());
}

static void turnstileServer_load(void)
{
    int i;
    char *publicName;
    U32 addr;

    // Convert hostnames to numeric IP addresses, and scream if something fails
    for (i = eaSize(&turnstileConfigCfg.dbservers) - 1; i >= 0; i--)
    {
        if (turnstileConfigCfg.dbservers[i]->serverPublicName && turnstileConfigCfg.dbservers[i]->serverPublicName[0])
        {
            publicName = turnstileConfigCfg.dbservers[i]->serverPublicName;
        }
        else
        {
            publicName = "<NONE>";
        }
        addr = ipFromString(publicName);
        if (addr == INADDR_NONE || addr == INADDR_ANY)
        {
            FatalErrorf("Error: invalid public server name %s in turnstile_server.cfg for shard %s\n", publicName, turnstileConfigCfg.dbservers[i]->shardName);
            exit(3);
        }
        turnstileConfigCfg.dbservers[i]->serverAddr = addr;
    }
}

static void turnstileServer_tick(void)
{
    int i;
    int serverCount;
    int result;
    NetLink *curLink;
    DBServerCfg *dbserver;
    char *curName;
    static U32 eventTimeTimer = 0;        // timer value used to send event times every minute
    U32 now;

    serverCount = eaSize(&turnstileConfigCfg.dbservers);
    // This should never happen, we already truncated the list in TurnstileServer_init(...)
    if (serverCount > MAX_DBSERVER)
    {
        serverCount = MAX_DBSERVER;
    }
    for (i = 0; i < serverCount; i++)
    {
        curLink = &db_links[i];
        dbserver = turnstileConfigCfg.dbservers[i];
        curName = dbserver->serverName;

        if (curLink->connected)
        {
            netLinkMonitor(curLink, 0, turnstileServer_HandleMsg);
        }
        if (!curLink->connected)
        {
            // If we're not connected, the pending flag says whether the link is disconnected and idle, or in the process of trying an async connect
            if (curLink->pending)
            {
                // If it's pending, there's a connect in progress, so poll for completion
                // This is an ugly hack.  I need the DBServerCfg struct deep inside here, but there's no provision to get a parameter where we need it,
                // which is inside turnstileServer_HandleMsg(...).  So we rely on this being single threaded and blocking, and just stuff the param
                // into a global.
                curDBServer = dbserver;
                result = connectDbPoll(curName, curLink);
                curDBServer = NULL;
                if (result == -1)
                {
                    // Handle the error
                    retryTimers[i] = timerAlloc();
                    timerStart(retryTimers[i]);
                }
            }
            // Not pending, check the retry timer
            else if (retryTimers[i] == 0)
            {
                // No active retry timer, it must have timed out, so kick start a connection attempt.
                connectDbStart(curName, curLink);
            }
            else
            {
                // Otherwise the retry timer is counting
                if (timerElapsed(retryTimers[i]) > 5.0f)
                {
                    // If it's finished clean up, this will cause the connection attempt next time through.
                    timerFree(retryTimers[i]);
                    retryTimers[i] = 0;
                }
            }
        }

        netIdle(curLink,1,10);
    }
    GroupTick();
    now = timerSecondsSince2000();
    if (eventTimeTimer == 0)
    {
        eventTimeTimer = now + WAIT_TIME_DELAY;
    }
    if (now > eventTimeTimer)
    {
        turnstileServer_generateEventWaitTimes();
        eventTimeTimer = now + WAIT_TIME_DELAY;
    }
}

static void turnstileServer_stop(void)
{
    logShutdownAndWait();
}

// Somewhat experimental, dump status to a string for http rendering
static void turnstileServer_http(char **str)
{
#define sendline(...) do { \
	Str_catf(str, __VA_ARGS__); \
	Str_catf(str, "\n"); \
} while (0)
#define rowclass            ((row++)&1)

    int i;
    int n;
    int row;
    int faQueued;
    float faAvgWait;
    TurnstileMission *mission;

    sendline("<table>");
    sendline("<tr class=rowt><th colspan=3>TurnstileServer summary.");
    sendline("<tr class=rowh><th>Mission<th>Available<th>Avg. wait");

    n = eaSize(&turnstileConfigDef.missions);
    row = 0;
    for(i = 0; i < n; ++i)
    {
        mission = turnstileConfigDef.missions[i];
        sendline("<tr class=row%d><td>%s<td>%d<td>%s", rowclass,
            mission->name, mission->totalQueued, timeToString((int) mission->avgWait));
    }
    // TODO - work these two out
    faQueued = 0;
    faAvgWait = 0.0f;
    sendline("<tr class=row%d><td>%s<td>%d<td>%s", rowclass,
            "First Available", faQueued, timeToString((int) faAvgWait));

    sendline("</table>");

}

ServerLibConfig g_serverlibconfig =
{
    "TurnstileServer",        // name
    'T',                    // icon
    0xdd66ff,                // color
    turnstileServer_init,
    turnstileServer_load,
    turnstileServer_tick,
    turnstileServer_stop,
    turnstileServer_http,
    NULL,
    kServerLibLikePiggs,
};
