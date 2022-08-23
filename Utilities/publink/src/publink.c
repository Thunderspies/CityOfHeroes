#include <utilitieslib/stdtypes.h>
#include <utilitieslib/network/sock.h>
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/network/netio.h>
#include "comm_backend.h"
#include <utilitieslib/utils/MemoryMonitor.h>
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/utils/memcheck.h>
#include <utilitieslib/components/StashTable.h>

static int public_sock;
#define DEFAULT_PUBLICCHAT_PORT 6987

typedef struct
{
    int        socket;
    U32        last_recv_time;
    U32        rand_int;
    U32        auth_id;
} PubLink;

PubLink                *pub_links;
int                    pub_min,pub_count,pub_max,pub_active;
StashTable            auth_id_hashes;

NetLink                uplink_comm;
Packet                *aggregate_pak;

static int idFromPub(PubLink *pub)
{
    if (!pub)
        return 0;
    if (pub->auth_id)
        return pub->auth_id;
    return -((pub - pub_links) + 1);
}

static PubLink *pubFromId(int id)
{
    if (id < 0)
    {
        id = -id - 1;
        if (id < 0 || id >= pub_count)
            return 0;
    }
    else if (id > 0)
    {
        PubLink    *ptr;

        if (stashIntFindPointer(auth_id_hashes, id,&ptr))
            return ptr;
        else
            return 0;
    }
    return &pub_links[id];
}

static void uplink(PubLink *pub,char *msg)
{
    if (!aggregate_pak)
    {
        uplink_comm.compressionAllowed = 1;
        uplink_comm.compressionDefault = 1;
        aggregate_pak = pktCreateEx(&uplink_comm,SHARDCOMM_CMD);
    }
    pktSendBits(aggregate_pak,32,idFromPub(pub));
    pktSendString(aggregate_pak,msg);
    printf("uplink: [%d] %s\n",idFromPub(pub),msg);
}

static void downlink(int id,char *msg)
{
    PubLink    *pub;
    int        len;
    char    buf[10000];

    pub = pubFromId(id);
    if (!pub)
        return;
    if (!pub->socket)
        return;
    printf("dnlink: [%d] %s\n",-1 - (pub - pub_links),msg);
    strcpy_s(buf,sizeof(buf),msg);
    len = (int)strlen(buf);
    buf[len] = '\r';
    buf[len+1] = '\n';
    send(pub->socket,buf,len+2,0);
}

PubLink *publinkAdd(int socket)
{
    int i;
    PubLink* pub = NULL;

    for(i = pub_min; i < pub_max; ++i)
    {
        pub = &pub_links[i];
        if (!pub->socket)
            break;
    }
    if (i >= pub_max)
        pub = dynArrayAdd(&pub_links,sizeof(pub_links[0]),&pub_count,&pub_max,1);
    pub_min = i+1;
    if (i >= pub_count)
        pub_count = i+1;
    memset(pub,0,sizeof(*pub));
    pub->socket = socket;
    return pub;
}

void publinkDelete(PubLink *pub,int tell_uplink)
{
    int        idx;

    if (!pub || !pub->socket)
        return;
    if (tell_uplink)
        uplink(pub,"Logout");
    closesocket(pub->socket);
    idx = pub - pub_links;
    if (pub->auth_id)
        stashIntRemovePointer(auth_id_hashes, pub->auth_id, 0);
    memset(pub,0,sizeof(*pub));
    if (idx < pub_min)
        pub_min = idx;
    if (idx == pub_count-1)
        pub_count--;
}

static int checkSpecialCommand(PubLink *pub,char *msg,int msg_len)
{
    static char buf[10000];
    char    *args[20];
    int        count;

    strncpy_s(buf,sizeof(buf),msg,sizeof(buf)-1);
    count = tokenize_line_safe(buf,args,ARRAY_SIZE(args),0);
    if (!count)
        return 1;
    if (stricmp(args[0],"PubLogin")==0)
    {
        sprintf_s(msg,msg_len,"%s %s",args[0],args[1]);
        return 0;
    }
    return 0;
}

void publinkScan()
{
    PubLink    *pub;
    int        i,len,amt;
    char    buf[10000];

    pub_active = 0;
    for(i=0;i<pub_count;i++)
    {
        pub = &pub_links[i];
        if (!pub->socket)
            continue;
        pub_active++;
        amt = recv(pub->socket,buf,sizeof(buf)-1,MSG_PEEK);
        if (amt > 0)
        {
            buf[amt] = 0;
            len = (int)strcspn(buf,"\n");
            if (len < amt)
            {
                amt = recv(pub->socket,buf,len+1,0);
                if (len && buf[len-1] == '\r')
                    buf[len-1] = 0;
                buf[amt-1] = 0;
                if (!checkSpecialCommand(pub,buf,sizeof(buf)))
                    uplink(pub,buf);
            }
        }
        if (amt <= 0)
        {
            int err = WSAGetLastError();
            if (err != WSAEWOULDBLOCK)
                publinkDelete(pub,1);
        }
    }
}

static int uplinkMessageCallback(Packet *pak_in,int cmd,NetLink *link)
{
    char    *args[100];
    int        id,count,repeat;
    PubLink    *pub;
    static    char    str[10000],buf[10000];

    if(cmd != SHARDCOMM_CMD)
        return 0;
    while(!pktEnd(pak_in))
    {
        id = pktGetBitsPack(pak_in,31);
        repeat = pktGetBits(pak_in,1);
        if (!repeat)
            strcpy_s(str,sizeof(str),pktGetString(pak_in));
        pub = pubFromId(id);
        if (!pub)
            continue;
        strncpy_s(buf,sizeof(buf),str,sizeof(buf)-1);
        count = tokenize_line(buf,args,0);
        if (!count)
            continue;
        if (stricmp(args[0],"Disconnect")==0)
        {
            publinkDelete(pubFromId(id),0);
        }
        else if (stricmp(args[0],"AuthId")==0)
        {
            pub->auth_id = atoi(args[1]);
            stashIntAddPointer(auth_id_hashes, pub->auth_id, pub, 1);
        }
        else
            downlink(id,str);
    }
    return 1;
}

void publinkSend(PubLink *pub,char *s)
{
    if (!pub->socket)
        return;
    send(pub->socket,s, (int)(strlen(s) + 1), 0);
}

int publinkInit(int port)
{
    struct sockaddr_in addr_in;

    public_sock = (int)socket(AF_INET,SOCK_STREAM,0);
    if (public_sock < 0)
        return 0;
    sockSetAddr(&addr_in,htonl(INADDR_ANY),port);
    if (!sockBind(public_sock,&addr_in))
    {
        printf("can't bind port %d\n",port);
        return 0;
    }
    sockSetBlocking(public_sock,0);
    listen(public_sock,50);
    return 1;
}

void publinkMonitor()
{
    struct sockaddr_in their_addr;
    int new_fd;
    int sin_size = sizeof(struct sockaddr_in);

    for(;;)
    {
        new_fd = accept(public_sock, (struct sockaddr *)&their_addr, &sin_size);
        if (new_fd > 0)
        {
            PubLink* pub;
            char buf[100];

            pub = publinkAdd(new_fd);
            sprintf_s(buf,sizeof(buf),"SessionId %d",rule30Rand());
            downlink(idFromPub(pub),buf);
        }
        else
            break;
    }
    publinkScan();
}

int publinkConnectUplink(char *server,int port)
{
    int        i;

    if (!uplink_comm.connected)
    {
        for(i=pub_count-1;i>=0;i--)
            publinkDelete(&pub_links[i],0);
        if (!netConnect(&uplink_comm,server,port,NLT_TCP,1,NULL))
        {
            printf("Connecting to %s:%d\n",server,port);
            return 0;
        }
        if (aggregate_pak)
            pktFree(aggregate_pak);
        aggregate_pak = 0;
        netLinkSetMaxBufferSize(&uplink_comm, BothBuffers, 8*1024*1024); // Set max size to auto-grow to
        netLinkSetBufferSize(&uplink_comm, BothBuffers, 1*1024*1024);
    }
    return 1;
}

int main(int argc,char **argv)
{
    char    buf[1000],*server_name = "localhost";
    int        timer = timerAlloc(), port = 6988;

    EXCEPTION_HANDLER_BEGIN
    setAssertMode(ASSERTMODE_DEBUGBUTTONS | ASSERTMODE_FULLDUMP | ASSERTMODE_DATEDMINIDUMPS | ASSERTMODE_ZIPPED);
    memMonitorInit();
    sockStart();
    packetStartup(0,0);
    auth_id_hashes;
    if (!publinkInit(DEFAULT_PUBLICCHAT_PORT))
        exit(0);
    auth_id_hashes = stashTableCreateInt(4);
    for(;;)
    {
        if (!publinkConnectUplink(server_name,port))
            continue;
        netLinkMonitor(&uplink_comm, 0, uplinkMessageCallback);
        lnkBatchSend(&uplink_comm);
        if (aggregate_pak)
            pktSend(&aggregate_pak,&uplink_comm);
        publinkMonitor();
        Sleep(1);
        if (timerElapsed(timer) > 0.5)
        {
            timerStart(timer);
            sprintf_s(buf,sizeof(buf),"PubLink  Users %d",pub_active);
            setConsoleTitle(buf);
        }
    }
    EXCEPTION_HANDLER_END
}
