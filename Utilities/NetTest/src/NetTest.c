#include <utilitieslib/network/netio.h>
#include <utilitieslib/network/net_packet.h>
#include <utilitieslib/network/net_masterlist.h>
#include <utilitieslib/network/sock.h>
#include <utilitieslib/utils/memcheck.h>
#include <utilitieslib/utils/error.h>
#include <conio.h>
#include <process.h>
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/network/netio_stats.h>
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/network/net_version.h>
#include <utilitieslib/utils/wininclude.h>
#include <utilitieslib/utils/osdependent.h>

extern void testEndian(void);

int defaultPacketSize=0;
static bool s_isServer = false;
static void s_AppendConsoleStatus(char *status);


typedef struct NetStats {
    int count;
    int recv_count, send_count;
    int lost_recv_count, lost_sent_count;
    int dup_count;
    int reliable_count, ack_count, oldest_id;
    int ping_avg;
    int ping_inst;
    PingHistory pingHistory;
    int pingHistoryDirty;
    int ooo_ordered_packet_recv_count;
    int num_connects;
    char lastconnect[512];
    int num_disconnects;
    char lastdisconnect[512];
} NetStats;

NetStats stats = {0};

/*
#define BUFFER_SIZE_DEFAULT 1*1024*1024
#define BUFFER_SIZE_MAX 4*1024*1024
*/
#ifndef _XBOX
#define BUFFER_SIZE_DEFAULT 128*1024
#define BUFFER_SIZE_MAX 1*1024*1024
#else
#define BUFFER_SIZE_DEFAULT 64512
#define BUFFER_SIZE_MAX 64512
#endif
//#define BUFFER_SIZE_DEFAULT 8*1024
//#define BUFFER_SIZE_MAX 8*1024
#define SENDQUEUE_SIZE 16384

enum {
    NETCMD_BROADCAST = COMM_MAX_CMD,
    NETCMD_VERIFY,
    NETCMD_STREAMING,
};

typedef enum CmdType {
    CMD_LISTEN,
    CMD_CONNECT,
    CMD_BROADCAST,
    CMD_PAUSE,
    CMD_SIMULATE,
    CMD_VERIFY,
    CMD_FLOOD,
    CMD_STREAMING,
} CmdType;

char *StrFromCmdType(CmdType s)
{
    static char *strs[] = {
        "CMD_LISTEN",
        "CMD_CONNECT",
        "CMD_BROADCAST",
        "CMD_PAUSE",
        "CMD_SIMULATE",
        "CMD_VERIFY",
        "CMD_FLOOD",
        "CMD_STREAMING",
    };
    return AINRANGE( s, strs ) ? strs[s] : "invalid enum";
}

typedef enum SIMType {
    SIM_NONE,
    SIM_PACKETLOST,
    SIM_LAGGY,
} SIMType;

char *StrFromSIMType(SIMType s)
{
    static char *strs[] = {
        "SIM_NONE",
        "SIM_PACKETLOST",
        "SIM_LAGGY",
    };
    return AINRANGE( s, strs ) ? strs[s] : "invalid enum";
}
typedef struct Cmd {
    CmdType cmd;
    U32 port;
    U32 addr;
    U32 size;
} Cmd;

typedef struct DummyClientLink {
    NetLink *link;
    int dummy;
    int streaming_lastRecvd;
    int streaming_cursorRecvd;
} DummyClientLink;

#define DEFAULT_PORT_SERVER 7009

#define MAX_CLIENTS 4096
#define QUEUESIZE 256
#define QUEUEMASK (QUEUESIZE-1)
Cmd cmdqueue[QUEUESIZE];
int cmdqueue_write=0, cmdqueue_read=0;
CRITICAL_SECTION CriticalSection;

void simulateCallback(NetLink *link);

static NetLinkType connect_linktype = NLT_UDP;

// for periodic sending of n packets of m size set these variables
int send_pkts=0;
int send_size=0;
int send_nPkts=1;

CmdType send_type = CMD_BROADCAST;
static CmdType send_types[] = { CMD_BROADCAST, CMD_STREAMING };

static int send_type_i=0;
typedef void (*send_fp)(U32 size, int quiet);

void doBroadcast(U32 size, int quiet);
void doStreaming(U32 size, int quiet);
void compressionSetCallback(NetLink *link);

static send_fp send_type_fps[] = 
{
    doBroadcast,
    doStreaming
};

void addCmd(Cmd cmd)
{
    assert(cmdqueue_write != cmdqueue_read-1);
    cmdqueue[cmdqueue_write] = cmd;
    cmdqueue_write++;
    if (cmdqueue_write == QUEUESIZE)
        cmdqueue_write = 0;
}

int connectCallback(NetLink *link)
{
    DummyClientLink    *client;
    client = link->userData;
    client->link = link;
    compressionSetCallback(link);
    simulateCallback(link); // Set initial network values
    if (link->type == NLT_TCP) {
        netLinkSetMaxBufferSize(link, BothBuffers, BUFFER_SIZE_MAX);
        netLinkSetBufferSize(link, BothBuffers, BUFFER_SIZE_DEFAULT);
    }
    link->sendQueuePacketMax = SENDQUEUE_SIZE;
    sprintf(stats.lastconnect, "%s", makeIpStr(link->addr.sin_addr.S_un.S_addr));
    stats.num_connects++;
    
    return 1;
}

int delCallback(NetLink *link)
{
    DummyClientLink    *client = link->userData;
    sprintf(stats.lastdisconnect, "%s", makeIpStr(link->addr.sin_addr.S_un.S_addr));
    stats.num_disconnects++;
    
    return 1;
}

int incrLinkVersCallback(NetLink *link)
{
    U32 ver = link->protocolVersion+1;
    applyNetworkVersionToLink(link, ver%(getMaxNetworkVersion()+1));;
    return 1;
}

int decrLinkVersCallback(NetLink *link)
{
    U32 ver = link->protocolVersion==0?getMaxNetworkVersion():(link->protocolVersion-1);
    applyNetworkVersionToLink(link,ver);
    return 1;
}

int handleServerMsg(Packet *pak,int cmd, NetLink *link)
{
    DummyClientLink *client = link->userData;

    // Handles either side's messages
    switch (cmd) {
        case NETCMD_BROADCAST:
            {
                int quiet = pktGetBitsPack(pak, 1);
                if (!quiet) {
                    printf("Broadcast received, %d bytes\n", pktGetBitsPack(pak, 1));
                }
            }
            break;
        case NETCMD_VERIFY:
            {
                int i;
                int size = pktGetBitsPack(pak, 1);
                unsigned char *data = _alloca(size);
                pktGetBitsArray(pak, size*8, data);
                for (i=0; i<size; i++) {
                    if (data[i] != (i& 0xff)) {
                        printf("Bad data!\n");
                    }
                }
                printf("Verify received, %d bytes\n", size);
            }
            break;
        case NETCMD_STREAMING:
            if(client)
            {
                int i;
                int size = pktGetBitsPack(pak, 1);
                unsigned char *data = _alloca(size);
                pktGetBitsArray(pak, size*8, data);
                for (i=0; i<size; i++) {
                    if (data[i] != ((client->streaming_lastRecvd+i)& 0xff)) {
                        static int iBad = 0;
                        printf("streamed - Bad data at %i. bad packet #%i!\n", client->streaming_cursorRecvd, ++iBad);
                    }
                    client->streaming_cursorRecvd++;
                }
                client->streaming_lastRecvd = data[i-1];
            }
            else
            {
                printf("null client got streaming...?");
            }
            break;
        default:
            printf("Unknown command %d!\n", cmd);
            break;
    }
    return 1;
}

int handleClientMsg(Packet *pak,int cmd, NetLink *link)
{
    DummyClientLink    *client = link->userData;
    if (!client)
        return 0;
    return handleServerMsg(pak, cmd, link);
}

NetLinkList *serverLinks = NULL;
void startListen(U32 udp_port, U32 tcp_port)
{
    NetLinkList *links = calloc(sizeof(NetLinkList), 1);
    netLinkListAlloc(links,MAX_CLIENTS,sizeof(DummyClientLink),connectCallback);
    if (netInit(links,udp_port,tcp_port)) {
        links->destroyCallback = delCallback;
        if (udp_port) {
            netLinkListSetMaxBufferSize(links, BothBuffers, BUFFER_SIZE_MAX);
            netLinkListSetBufferSize(links, BothBuffers, BUFFER_SIZE_DEFAULT);
        }
        NMAddLinkList(links, handleClientMsg);
        if (udp_port)
            printf("Now listening on UDP port %d\n", udp_port);
        if (tcp_port)
            printf("Now listening on TCP port %d\n", tcp_port);
        serverLinks = links;
    } else {
        printf("Failed to start listening\n");
    }
}

void startConnect(U32 addr, U32 port, NetLinkType nlt)
{
    NetLink *link = createNetLink();
    simulateCallback(link);
    printf("Connecting to %s:%d... ", makeIpStr(addr), port);
    if (netConnect(link, makeIpStr(addr), port, nlt, 30.f, NULL)) {
        netLinkSetMaxBufferSize(link, BothBuffers, BUFFER_SIZE_MAX);
        netLinkSetBufferSize(link, BothBuffers, BUFFER_SIZE_DEFAULT);
        link->sendQueuePacketMax = SENDQUEUE_SIZE;
        NMAddLink(link, handleServerMsg);
        printf("done.\n");
    } else {
        destroyNetLink(link);
        printf("failed.\n");
    }
}

typedef enum BroadcastType
{
    kBroadcastType_None,
    kBroadcastType_Ordered,
    kBroadcastType_Compressed,    
} BroadcastType;

unsigned char *broadcastdata=NULL;
int broadcastsize=0;
int broadcastquiet=0;
BroadcastType broadcasttype = 0;
void broadcastCallback(NetLink *link)
{
    Packet *pak = pktCreateEx(link, NETCMD_BROADCAST);
    switch ( broadcasttype )
    {
    case kBroadcastType_Ordered:
        pak->ordered = 1;;
        break;
    case kBroadcastType_Compressed:
        //pktSetCompression(pak,1);
        break;
    default:
        // nothing
        break;
    };
    pktSendBitsPack(pak, 1, broadcastquiet);
    pktSendBitsPack(pak, 1, broadcastsize);
    pktSendBitsArray(pak, broadcastsize*8, broadcastdata);
    pktSend(&pak, link);
}

void doBroadcast(U32 size, int quiet)
{
    broadcastdata = size < 1024*1024 ? _alloca(size) : malloc(size);
    broadcastsize = size;
    broadcastquiet = quiet;

    NMForAllLinks(broadcastCallback);
    if (size >= 1024 * 1024)
        free(broadcastdata);
}

void verifyCallback(NetLink *link)
{
    Packet *pak = pktCreateEx(link, NETCMD_VERIFY);
    pktSendBitsPack(pak, 1, broadcastsize);
    pktSendBitsArray(pak, broadcastsize*8, broadcastdata);
    pktSend(&pak, link);
}

void doVerify(U32 size, int quiet)
{
    unsigned int i;
    broadcastdata = size < 1024*1024 ? _alloca(size) : malloc(size);
    broadcastsize = size;
    broadcastquiet = quiet;

    for (i=0; i<size; i++) {
        broadcastdata[i] = i & 0xff;
    }

    NMForAllLinks(verifyCallback);
    if (size >= 1024 * 1024)
    {
        free(broadcastdata);
    }
}


// *********************************************************************************
//  Streaming is like verify, but with a continuous counter between packets.
// *********************************************************************************
void compressionSetCallback(NetLink *link)
{
    link->compressionDefault = (broadcasttype == kBroadcastType_Compressed);
}

void orderedSetCallback(NetLink *link)
{
    link->orderedDefault = (broadcasttype == kBroadcastType_Ordered);
}

void streamingCallback(NetLink *link)
{
    Packet *pak = pktCreateEx(link, NETCMD_STREAMING);
    int bcSize = broadcastsize*8;
    unsigned char *bcData = broadcastdata;

    if(broadcasttype == kBroadcastType_Compressed)
    {
        // handled in link default
        //pktSetCompression(pak,1);
    }
    else
    {
        pak->ordered = 1;
        pak->reliable = 1;
    }
    pktSendBitsPack(pak, 1, broadcastsize);
    pktSendBitsArray(pak, bcSize, bcData);
    pktSend(&pak, link);
}


void doStreaming(U32 size, int quiet)
{
    static int cursorSent = 0;
    static unsigned char lastSent = 0;

    unsigned int i;

    if( (size%256) == 0)
    {
        static bool once = false;
        if(!once)
        {
            printf("streaming size is multiple of 256. adding 1\n");
            once = true;
        }
        
        size++;
    }
    broadcastdata = _alloca(size);
    broadcastsize = size;
    broadcastquiet = quiet;

    for (i=0; i<size; i++) {
        broadcastdata[i] = lastSent++;
    }
    lastSent--;
    
    cursorSent+=size;
    
    NMForAllLinks(streamingCallback);
}


int simulate_bad=0;
void simulateCallback(NetLink *link)
{
    switch ( simulate_bad )
    {
    case SIM_PACKETLOST:
        lnkSimulateNetworkConditions(link, 0, 0, 20, 1);
        break;
    case SIM_LAGGY:
        lnkSimulateNetworkConditions(link, 500, 100, 20, 0);
        break;
    case SIM_NONE:
    default:
        lnkSimulateNetworkConditions(link, 0, 0, 0, 0);
    };
}

void doSimulate(int bad)
{
    simulate_bad = bad;

    NMForAllLinks(simulateCallback);
}


void gatherStatsCalback(NetLink *link)
{
//    if (link->parentNetLinkList) {
//        inout = STAT_INCOMING;
//    } else {
//        inout = STAT_OUTGOING;
//    }
    stats.count++;
    stats.recv_count+=link->last_recv_id;
    stats.send_count+=link->nextID;
    stats.lost_recv_count+=link->lost_packet_recv_count;
    stats.lost_sent_count+=link->lost_packet_sent_count;
    stats.dup_count+=link->duplicate_packet_recv_count;
    stats.ooo_ordered_packet_recv_count+=link->ooo_ordered_packet_recv_count;
    stats.reliable_count+=link->reliablePacketsArray.size;
    stats.ack_count+=link->ack_count;
    stats.ping_avg+=pingAvgRate(&link->pingHistory);
    stats.ping_inst+=pingInstantRate(&link->pingHistory);
    if (link->reliablePacketsArray.size) {
        int id = ((Packet*)link->reliablePacketsArray.storage[0])->id;
        if (stats.oldest_id==0) {
            stats.oldest_id = id;
        } else {
            stats.oldest_id = MIN(stats.oldest_id, id);
        }
    }
//    assert(link->retransmit);
//    if (!link->retransmit) {
//        printf("Link without retrans, type %d\n", link->type);
//    }
}

void gatherStats()
{
    static PingHistory pingHistory;
    static int last_recv_count=0;
    int dirty;
    EnterCriticalSection(&CriticalSection);
    dirty = stats.pingHistoryDirty;
    memset(&stats, 0, sizeof(stats));
    NMForAllLinks(gatherStatsCalback);
    if (stats.count) {
        stats.ping_avg/=stats.count;
        stats.ping_inst/=stats.count;
        if (last_recv_count != stats.recv_count) {
            last_recv_count = stats.recv_count;
            pingAddHist(&pingHistory, stats.ping_inst);
            dirty = 1;
        }
    }
    stats.pingHistoryDirty=dirty;
    memcpy(&stats.pingHistory, &pingHistory, sizeof(pingHistory));
    LeaveCriticalSection(&CriticalSection);
}

void checkSends()
{
    static int timer=0;
    static F32 last_send=0;
    if (!timer) timer = timerAlloc();
    if (send_pkts && send_size) {        
        F32 elapsed = timerElapsed(timer) - last_send;
        if (elapsed > 10.f) {
            last_send = timerElapsed(timer);
        }
        if (elapsed > 1.f/send_pkts) {
            int i;
            for( i = 0; i < send_nPkts; ++i ) 
            {
                send_fp *fp = &send_type_fps[send_type_i];
                last_send += 1.f/send_pkts;
                //doBroadcast(send_size, 1);
                (*fp)(send_size,1);
            }
        }
    }
}

unsigned int WINAPI networkThread(void *junk)
{
    loadstart_printf("Networking startup... ");
    sockStart();
    packetStartup(defaultPacketSize,1);
    loadend_printf("done");
    while(1) {
        while (cmdqueue_read != cmdqueue_write) {
            Cmd cmd = cmdqueue[cmdqueue_read];
            cmdqueue_read = (cmdqueue_read + 1) & QUEUEMASK;
            switch (cmd.cmd) {
                case CMD_LISTEN:
                    if(connect_linktype == NLT_UDP)
                        startListen(cmd.port, 0);
                    else
                        startListen(0, cmd.port);
                    break;
                case CMD_CONNECT:
                    startConnect(cmd.addr, cmd.port, connect_linktype);
//                     startConnect(cmd.addr, cmd.port, NLT_UDP);
                    //startConnect(cmd.addr, cmd.port, NLT_TCP);
                    break;
                case CMD_BROADCAST:
                    doBroadcast(cmd.size, 0);
                    break;
                case CMD_FLOOD:
                    {
                        U32 i;
                        for (i=0; i<cmd.size; i++) 
                            doBroadcast(128, 1);
                    }
                    break;
                case CMD_VERIFY:
                    doVerify(cmd.size, 0);
                    break;
                case CMD_STREAMING:
                    doStreaming(cmd.size, 0);
                    break;
                case CMD_PAUSE:
                    printf("Sleeping for %dms...", cmd.size);
                    Sleep(cmd.size);
                    printf("done\n");
                    break;
                case CMD_SIMULATE:
                    if (cmd.size) {
                        printf("Simulating bad network conditions: %s \n", StrFromSIMType(cmd.size));
                    } else {
                        printf("No longer simulationg bad network conditions\n");
                    }
                    doSimulate(cmd.size);
                    break;
            }
        }
        checkSends();
        NMMonitor(1);
        gatherStats();
    }

    return 0;
}

static HANDLE hConsoleOutput = NULL;

void displayPing(int x, int y, int w, int h) // Too slow to be useful
{
#ifndef _XBOX
    COORD coord;
    int i, j;
    int ret;
    static char *output="X ";

    for (i=0; i<w && i<stats.pingHistory.size; i++) {
        int ping = stats.pingHistory.pings[(stats.pingHistory.nextEntry-stats.pingHistory.size+i+MAX_PINGHIST)%MAX_PINGHIST];
        int effh = MIN(h, ping*h/1000);
        coord.X=x+i;
        for (j=0; j<h-effh; j++) {
            coord.Y = y+j;
            WriteConsoleOutputCharacter( hConsoleOutput, &output[1], 1, coord, &ret);
        }
        for (j=h-effh; j<h; j++) {
            coord.Y = y+j;
            WriteConsoleOutputCharacter( hConsoleOutput, &output[0], 1, coord, &ret);
        }
    }
#endif
}

void printStats()
{
#ifndef _XBOX
    char status[1024];
    
    if (!hConsoleOutput) {
        hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    }
    EnterCriticalSection(&CriticalSection);
    {
        static NetStats last_stats={-1};
        if (0!=memcmp(&last_stats, &stats, sizeof(stats))) {
            COORD coord = {0, 0};
            DWORD ret;
            size_t packetCount, bsCount;
            sprintf(status, "%d lnks; Pak R/S: %-6d/%-6d  Re-Sent:%-6d  OoOR:%-6d  DupR:%-5d", 
                stats.count, stats.recv_count, stats.send_count,
                stats.lost_sent_count, stats.lost_recv_count,
                stats.dup_count, stats.ooo_ordered_packet_recv_count);
            WriteConsoleOutputCharacter( hConsoleOutput, status, (DWORD)strlen(status), coord, &ret);

            sprintf(status,"        OoO-Ordered Recv:%i", stats.ooo_ordered_packet_recv_count);
            coord.Y++;
            WriteConsoleOutputCharacter( hConsoleOutput, status, (DWORD)strlen(status), coord, &ret);

            packetGetAllocationCounts(&packetCount, &bsCount);
            sprintf(status, "  RPA.size: %-6d  Reliable.Oldest: %-6d  ack_count: %-6d  AvgPing: %-4d  InstPing: %-4d  #Pkts: %-4d", 
                stats.reliable_count, stats.oldest_id, stats.ack_count,
                stats.ping_avg, stats.ping_inst, packetCount);
            coord.Y++;
            WriteConsoleOutputCharacter( hConsoleOutput, status, (DWORD)strlen(status), coord, &ret);
            
            sprintf(status, " #conns: %-6d #discons: %-6d. last con='%s' last discon='%s'",stats.num_connects, stats.num_disconnects, stats.lastconnect, stats.lastdisconnect);
            coord.Y++;
            WriteConsoleOutputCharacter( hConsoleOutput, status, (DWORD)strlen(status), coord, &ret);            

            //if (stats.pingHistoryDirty)
            //    displayPing(0,2,110,20);

            memcpy(&last_stats, &stats, sizeof(stats));
        }
    }
    LeaveCriticalSection(&CriticalSection);
#endif
}

static bool s_datatypeErr = false;
void s_dataTypeErrorCb(char *buffer)
{
    printf("DATA TYPE ERROR, probably a bad packet read: %s\n", buffer);
    if(!s_datatypeErr)
    {
        s_AppendConsoleStatus(", ERROR - DataType");
        s_datatypeErr = true;
    }                          
}

static char s_ConsoleStatus[1024];
static void s_AppendConsoleStatus(char *status)
{
#ifndef _XBOX
    char buf[1024];
    sprintf(buf,"%s%s", s_ConsoleStatus, status);
    SetConsoleTitle(buf);
    Strncpyt(s_ConsoleStatus, buf);
#endif
}

int main(int argc, char *argv[])
{
    Cmd cmd;
    int broadcastSize = 1024;
    extern int g_assert_on_netlink_overflow;
    g_assert_on_netlink_overflow = 1;
#ifndef _XBOX
    memCheckInit();

    sprintf(s_ConsoleStatus, "%d: NetTest", GetCurrentProcessId());
    SetConsoleTitle(s_ConsoleStatus);
#else
    FillCommandLine(&argc,&argv);
#endif

    InitializeCriticalSection(&CriticalSection);
    printf("\n\n");

    pktSetDebugInfo();
    bsAssertOnErrors(true);
    setDataTypeErrorCb(s_dataTypeErrorCb);
    // disableLogging(true);

    // client/server args 
    if (stricmp(argv[1], "-client")==0) {
        char *addr = "127.0.0.1";

        s_isServer  = false;

        if(argc >= 3 && argv[2][0] != '-')
            addr = argv[2];
        
        cmd.cmd = CMD_CONNECT;
        cmd.port = DEFAULT_PORT_SERVER;
        cmd.addr = inet_addr(addr); // 0x0100007f;
        addCmd(cmd);
        printf("connecting to %s\n", addr);
        s_AppendConsoleStatus(" client");
    }
    if (stricmp(argv[1], "-server")==0) {
        char *addr = "127.0.0.1";

        s_isServer = true;

        if(argc >= 3 && argv[2][0] != '-')
            addr = argv[2];
        cmd.cmd = CMD_LISTEN;
        cmd.port = DEFAULT_PORT_SERVER;
        addCmd(cmd);
        s_AppendConsoleStatus(" server");
    }
    
    // parse general args
    printf("parsing args\n");
    {
        int i;
        for( i = 0; i < argc; ++i ) 
        {
            if(0 == stricmp(argv[i],"-linkver"))
            {
                int v = atoi(argv[++i]);
                printf("default network version now %i\n", v);
                setDefaultNetworkVersion(v);
            }
            else if(0 == stricmp(argv[i],"-tcp"))
            {
                connect_linktype = NLT_TCP;
            }
            else if( 0 == stricmp(argv[i],"-udp"))
            {
                connect_linktype = NLT_UDP;
            }
            else if(0 == stricmp(argv[i],"-type"))
            {
                send_type_i = atoi(argv[++i])%ARRAY_SIZE(send_types);;
                send_type = send_types[send_type_i];
                printf("sending type %s\n", StrFromCmdType(send_type));
            }
            else if(0 == stricmp(argv[i],"-ss")) // send size
            {
                send_size = atoi(argv[++i]);
                printf("send size %i\n", send_size);
            }
            else if(0 == stricmp(argv[i],"-sp")) // send size
            {
                send_pkts = atoi(argv[++i]);
                printf("send pkts %i\n", send_size);
            }
            else if(0 == stricmp(argv[i],"-sim")) // send size
            {
                Cmd cmd = {0};
                cmd.cmd = CMD_SIMULATE;
                cmd.size = atoi(argv[++i]);
                printf("simulating bad packets: %s\n", StrFromSIMType(cmd.size));
                addCmd(cmd);
            }
        }
    }
    printf("done.\n");

    _beginthreadex(NULL, 0, networkThread, NULL, 0, NULL);

    while (true) {
#ifndef _XBOX
        if (_kbhit()) {
            int c = _getch();
            switch (tolower(c)) {
                case 'c':
                    cmd.cmd = CMD_CONNECT;
                    cmd.port = DEFAULT_PORT_SERVER;
                    cmd.addr = getHostLocalIp();
                    addCmd(cmd);
                    break;
                case 'l':
                    cmd.cmd = CMD_LISTEN;
                    cmd.port = DEFAULT_PORT_SERVER;
                    addCmd(cmd);
                    break;
                case 'b':
                    cmd.cmd = CMD_BROADCAST;
                    cmd.size = broadcastSize;
                    addCmd(cmd);
                    break;
                case 'f':
                    cmd.cmd = CMD_FLOOD;
                    cmd.size = broadcastSize;
                    addCmd(cmd);
                    break;
                case 'v':
                    cmd.cmd = CMD_VERIFY;
                    cmd.size = broadcastSize ;
                    addCmd(cmd);
                    break;
                case 't':
                    {
                        int i;
                        for(i = 0; i < 1; ++i)
                        {
                            cmd.cmd = CMD_STREAMING;
                            cmd.size = 4097;
                            addCmd(cmd);
                        }
                    }
                    break;
                case 'p':
                    cmd.cmd = CMD_PAUSE;
                    cmd.size = 10000;
                    addCmd(cmd);
                    break;
                case 's':
                    {
                        static int bad=0;
                        bad=(bad+1)%3;
                        cmd.cmd = CMD_SIMULATE;
                        cmd.size = bad;
                        addCmd(cmd);
                    }
                    break;
                case '=':
                    broadcastSize <<= 1;
                    printf("broadcastSize changed to %d\n", broadcastSize);
                    break;
                case '-':
                    NMForAllLinks(decrLinkVersCallback);
                    printf("decremented link version\n");
                    break;
                case '+':
                    NMForAllLinks(incrLinkVersCallback);
                    printf("incremented link version\n");
                    break;
                case '_':
                    broadcastSize >>= 2;
                    printf("broadcastSize changed to %d\n", broadcastSize);
                    break;
                case '0':
                    send_pkts = 0;
                    send_size = 0;
                    printf("Now sending %d pkts/second at %dB/pkt\n", send_pkts, send_size);
                    break;
                case '1':
                    send_size = 4024;
                    send_pkts = 4;
                    send_nPkts = 40;
                    printf("Now sending %d pkts at a time at %d pkts/second at %dB/pkt\n", send_nPkts, send_pkts, send_size);
                    break;
                case '2':
                    send_size = 120;
                    send_pkts = 10;
                    send_nPkts = 100;
                    printf("Now sending %d pkts at a time at %d pkts/second at %dB/pkt\n", send_nPkts, send_pkts, send_size);
                    break;
                case 'e':
                    {
                        send_type_i = (send_type_i+1)%ARRAY_SIZE(send_types);
                        send_type = send_types[send_type_i];
                        printf("now sending type %s\n", StrFromCmdType(send_type));
                    }
                    break;
                case 'o':
                    if(broadcasttype != kBroadcastType_Ordered)
                    {
                        broadcasttype = kBroadcastType_Ordered;
                        printf("client packets now ordered\n");
                    }
                    else
                    {
                        broadcasttype = kBroadcastType_None;
                        printf("client packets no longer ordered\n");
                    }
                    NMForAllLinks(orderedSetCallback);
                    break;
                case 'm':
                    if(broadcasttype != kBroadcastType_Compressed)
                    {
                        broadcasttype = kBroadcastType_Compressed;
                        printf("client packets now Compressed\n");
                    }
                    else
                    {
                        broadcasttype = kBroadcastType_None;
                        printf("client packets no longer Compressed\n");
                    }
                    NMForAllLinks(compressionSetCallback);
                    //if(srvrLinkList)
                        //serverLinks->
                    break;
                case '?':
                    printf("  c - Connect\n");
                    printf("  l - Listen (start server)\n");
                    printf("  b - Broadcast\n");
                    printf("  f - Flood \n");
                    printf("  v - verifying broadcast: check sent bits \n");
                    printf("  t - streaming broadcast: check multiple packets of sent bits \n");
                    printf("  P - Pause\n");
                    printf("  s - Toggle simulation of bad network conditions: none (default), out of order, again for lag, \n");
                    printf("  = - double broadcast size \n");
                    printf("  - - increment link version \n");
                    printf("  + - decrement link version \n");
                    printf("  _ - 1/4 broadcast size \n");
                    printf("  0 - Set send values to 0 pkts/s\n");
                    printf("  1 - Set send values to 4 pkts/s, 1K/pkt\n");
                    printf("  2 - Set send values to 10 pkts/s, 120B/pkt\n");
                    printf("  e - Set send type \n");
                    printf("  o - ordered packets (use with streaming broadast)\n");
                    printf("  m - turn on compression sends (use with streaming broadast)\n");
                    printf("  Esc - Quit\n");
                    break;
                case 27:
                    return 0;
            }
        }
#endif
        if(stats.num_disconnects && !s_isServer)
        {
            printf("disconnect from server. quitting\n");
            Sleep(1);
            exit(0);
        }
        
        Sleep(1);
        printStats();
    }

}
