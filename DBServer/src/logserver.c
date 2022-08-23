#include <stdio.h>
#include <utilitieslib/components/StashTable.h>
#include <utilitieslib/network/netio.h>
#include "comm_backend.h"
#include "dblog.h"
#include <utilitieslib/utils/error.h>
#include <utilitieslib/utils/MemoryMonitor.h>
#include <utilitieslib/utils/file.h>
#include <conio.h>
#include <utilitieslib/network/sock.h>
#include <utilitieslib/utils/utils.h>
#include "logserver.h"
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/network/netcomp.h>
#include <utilitieslib/utils/memlog.h>
#include "chatrelay.h"
#include <utilitieslib/components/EString.h>
#include "dbserver/servercfg.h"
#include <utilitieslib/utils/log.h>
#include "zeromqSocket.h"

#define MAX_CLIENTS 1000
NetLinkList        log_links;

static ZmqSocket mqSocket;

int logNewLinkCallback(NetLink *link)
{
    LogClientLink    *client = link->userData;

    client->link = link;
    shardChatFlagForSendStatusToMap(link); // Can't send packets yet, we're not connected!
    return 1;
}

int logDelLinkCallback(NetLink *link)
{
    shardLogoutStranded(link);
    return 1;
}

// read the zip and post the message when all that is
// left on the packet is the zip.
static void s_handleLogInternal(Packet *pak, char *log_name, char *msgDelim)
{
    char *zip_data;
    U32 zipsize;
    U32 rawsize;
    static U32 *zipblock,zipblock_max;

    zip_data = pktGetZippedInfo(pak,&zipsize,&rawsize);
    zmqSend(&mqSocket, zip_data, zipsize+8);
    dynArrayFit(&zipblock,1,&zipblock_max,zipsize+8);
    zipblock[0] = zipsize;
    zipblock[1] = rawsize;
    memcpy(&zipblock[2],zip_data,zipsize);
    free(zip_data);
    {
        extern void logPutMsgSub(char *msg_str,int len,char *filename,int zip_file,int zipped,int text_file, char *msgDelim);
        logPutMsgSub((char*)zipblock,zipsize+8,log_name,0,1,0, msgDelim);
    }
}

static void handleSetZmqConnectState(Packet *pak_in)
{
    int flag = pktGetBitsAuto(pak_in);
    setZMQsocketConnectState(flag);
    printf("Setting ZMQ Socket with flag: %d\n", flag);
}

#define STRING_SIZE 1000

static void handleGetZmqStatus(Packet *pak_in, NetLink *link)
{
    U32 dbid = pktGetBitsAuto(pak_in);

    //TODO: Once system is in place for mapserver to receive messages from logserver,
    //uncomment the lines that send the packet to the mapserver. For now, print
    //ZeroMQ status to logserver window.
    //Packet *pakRes = pktCreateEx(link, LOGSERVER_SEND_ZEROMQ_STATUS);

    char zmqStatusString[STRING_SIZE];;
    storeZMQStatusInString(zmqStatusString, STRING_SIZE);

    printf("Received Get ZMQ Status command. %s\n", zmqStatusString);

    //pktSendBitsAuto(pakRes, dbid);
    //pktSendString(pakRes, zmqStatusString);
    //pktSend(&pakRes,link);
}

// static char *s_makeLogfileName(char *stem, U32 secondsSince2000, char *log_name, int len)
// {
//     char *s;
//     char datestr[1000];
//     timerMakeDateStringFromSecondsSince2000(datestr,secondsSince2000);
//     for(s=datestr;*s;s++)
//     {
//         if (*s == ':' || *s == ' ')
//             *s = '-';
//     }
//     sprintf(log_name,"%s_%s_1",stem, datestr);
//     assert(strlen(log_name) < (U32)len);
//     return log_name;
// }


void handleLog(Packet *pak)
{
    static U32 *zipblock,zipblock_max;
    s_handleLogInternal(pak, "db", "\n");
}

void handleBugLog(Packet *pak)
{
    LOG( LOG_BUG, LOG_LEVEL_IMPORTANT, 0, "%s", pktGetString(pak) );
}

void handleLogPrintf(Packet *pak)
{
    static StashTable logFileFromFname = 0;
    char *fname;
    
    if( !logFileFromFname)
    {
        // intern the logfile strings in this stashtable (note deep copy)
        logFileFromFname = stashTableCreateWithStringKeys(128, StashDeepCopyKeys);
    }
    strdup_alloca(fname,pktGetString(pak)); 
    if(fname)
    {
        char *logfile = NULL;
        
        // the logs are mapped by fname. find existing or add new
        if( !stashFindPointer( logFileFromFname, fname, &logfile ) )
        {
            // make the logfilename if it hasn't already been.
            estrCreateEx(&logfile, (int)strlen(fname)+12);
            estrPrintCharString(&logfile,fname);
            stashAddPointer(logFileFromFname, fname, logfile, true );
        }
        
        s_handleLogInternal(pak, logfile, "\t\t\n");
    }
}

int logHandleClientMsg(Packet *pak,int cmd, NetLink *link)
{
    int        ret = 1;

    switch(cmd)
    {
        xcase LOGCLIENT_LOG:
            handleLog(pak);
        xcase LOGCLIENT_LOGBUG:
            handleBugLog(pak);
        xcase LOGCLIENT_SAVELISTS:
            logWaitForQueueToEmpty();
        xcase LOGCLIENT_SHARDCHAT:
            shardChatRelay(pak,link);
        xcase LOGCLIENT_ZEROMQ_SET_CONNECT_STATE:
            handleSetZmqConnectState(pak);
        xcase LOGCLIENT_ZEROMQ_GET_STATUS:
            handleGetZmqStatus(pak, link);
        break;
        case LOGCLIENT_LOGPRINTF:
            handleLogPrintf(pak);
        break;
        default:
            LOG_OLD_ERR( "Unknown command %d\n",cmd);
            ret = 0;
    }
    return ret;
}

int logNetInit()
{
    shardChatInit();
    netLinkListAlloc(&log_links,MAX_CLIENTS,sizeof(LogClientLink),logNewLinkCallback);
    if (!netInit(&log_links,0,DEFAULT_LOG_PORT))
    {
        printf("Error binding to port %d!\n", DEFAULT_LOG_PORT);
        return false;
    }
    log_links.destroyCallback = logDelLinkCallback;
    NMAddLinkList(&log_links, logHandleClientMsg);
    return true;
}

void updateLogServerTitle()
{
    static int    timer;
    char        buf[1000], buf2[100], buf3[100];
    F32            time;

    // for chat stats
    extern NetLink    shard_comm;
    extern int        g_chatserver_recv_msgs;
    extern int        g_chatserver_sent_msgs;


    if (!timer)
    {
        timer = timerAlloc();
    }

    time = timerElapsed(timer);
    if (time < 1.0)
        return;

    timerStart(timer);
    if(shard_comm.connected)
    {
        sprintf(buf,
        "LogServer   Chat %s/%s (MSG %.0f/%.0f)",
        printUnit(buf2, pktRate(&shard_comm.sendHistory)),
        printUnit(buf3, pktRate(&shard_comm.recvHistory)),
        g_chatserver_sent_msgs / time,
        g_chatserver_recv_msgs / time
        );
    }
    else
    {
        strcpy(buf, "LogServer");
    }
    setConsoleTitle(buf);

    g_chatserver_sent_msgs=0;
    g_chatserver_recv_msgs=0;
}

static NetLink db_stats_link;
static int log_packet_in_progress = 0;
static int log_packet_timer = 0;
static int log_packet_count = 0;
static int log_packet_delay_usec;

static void sendLogStats(NetLink *link)
{
    size_t bytes_written;
    int queue_count, queue_max;
    size_t sorted_memory, sorted_memory_limit;
    Packet* pak;
    
    pak = pktCreateEx(link, DBSVRMON_LOGSERVERSTATS);
    if(!pak) {
        return;
    }
    
    logMsgQueueStats(&bytes_written, &queue_count, &queue_max, &sorted_memory, &sorted_memory_limit);

    pktSendBitsAuto(pak, ++log_packet_count);
    pktSendBitsAuto(pak, log_packet_delay_usec);
    pktSendBitsAuto(pak, (U32)bytes_written);
    pktSendBitsAuto(pak, queue_count);
    pktSendBitsAuto(pak, queue_max);
    pktSendBitsAuto(pak, (U32)sorted_memory);
    pktSendBitsAuto(pak, (U32)sorted_memory_limit);
    pktSend(&pak, link);

    timerStart(log_packet_timer);

    log_packet_in_progress = 1;
}

static int updateLogStatsCallback(Packet *pak, int cmd, NetLink *link)
{
    int pakid;
    switch(cmd) {
        case DBSVRMON_LOGSERVERSTATS:
            pakid = pktGetBitsAuto(pak);
            if(pakid != log_packet_count) {
                break;
            }
            log_packet_delay_usec = timerElapsed(log_packet_timer) * 1000.0f * 1000.0f;
            log_packet_in_progress = 0;
            break;
        default:
            break;
    }
    return 1;
}

static void updateLogStatsIdleCallback(F32 timeleft)
{
    // Need to tick NMMonitor or the netConnect will not be able to recive the connect
    // packet because NMMonitor is not running when the logserver and the dbserver are
    // running in the same process.
    if(!server_cfg.use_logserver) {
        NMMonitor(1);
    }
}

void updateLogStats()
{
    if(!log_packet_timer) {
        log_packet_timer = timerAlloc();
        timerStart(log_packet_timer);
    }

    if(!db_stats_link.connected) {
        if(timerElapsed(log_packet_timer) < 10) {
            return;
        }

        if (!netConnect(&db_stats_link,server_cfg.db_server,DEFAULT_SVRMON_PORT,NLT_TCP,1,updateLogStatsIdleCallback)) {
            printf("Can't connect to dbserver\n");
            timerStart(log_packet_timer);
            return;
        }

        // After connecting via the server monitor port, we specifically don't send
        // the connect packet to prevent us from receiving any server monitor data.
        log_packet_in_progress = 0;
    }

    netLinkMonitor(&db_stats_link, 0, updateLogStatsCallback);

    if((!log_packet_in_progress && timerElapsed(log_packet_timer) > 10) || timerElapsed(log_packet_timer) > 60) {
        if(qGetSize(db_stats_link.sendQueue2) < 1) {
            sendLogStats(&db_stats_link);
        }
    }

    lnkBatchSend(&db_stats_link);
}

void logServerShutdown()
{
}

void setZMQsocketConnectState(int flag)
{
    if (flag)
    {
        zmqCreateSocket(&mqSocket);
        zmqConnect(&mqSocket);
    }
    else
    {
        zmqCloseSocket(&mqSocket);
    }
}

void storeZMQStatusInString(char* stringArray, int arrayLength)
{
    if (mqSocket.context)
    {
        bool outgoingStatus = mqSocket.outgoing ? true : false;
        sprintf_s(stringArray, arrayLength, "ZMQ Socket Connect Status: %d, IP Address: %s, PortNumber: %d, HighWaterMark: %u", 
            outgoingStatus, mqSocket.ipAddress, mqSocket.portNumber, mqSocket.highWaterMark);
    }
    else
    {
        sprintf_s(stringArray, arrayLength,
            "No ZMQ Socket initialized. MetricsEnable 1 must be set in servers.cfg file to use ZMQ socket (reboot required).");
    }
}

void logMain()
{
    if (!logNetInit())
        exit(0);
    if (fileIsUsingDevData()) {
        bsAssertOnErrors(true);
        setAssertMode(ASSERTMODE_DEBUGBUTTONS |
            (!IsDebuggerPresent()? ASSERTMODE_MINIDUMP : 0));
    } else {
        // In production mode on the servers we want to save all dumps and do full dumps. 
        // MJP -- logserver will auto-exit to sever connection with chatserver & auto-logoff everyone.
        setAssertMode(ASSERTMODE_EXIT |
            ASSERTMODE_FULLDUMP |
            ASSERTMODE_DATEDMINIDUMPS | ASSERTMODE_ZIPPED);
    }

    logSetDir("logserver");
    logSetUseTimestamped(true);
    printf("LogServer Ready.\n");
    setConsoleTitle("LogServer");

    for(;;)
    {
        mpCompactPools();
        shardChatMonitor();
        updateLogStats();
        NMMonitor(1);
        updateLogServerTitle();
        if (_kbhit()) { // debug hack for printing out memory monitor stuff
            switch (_getch()) {
                case 'm':
                    memMonitorDisplayStats();
            }
        }
    }
}


