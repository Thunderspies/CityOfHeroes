#include "clientcommLogin.h"
#include <utilitieslib/utils/file.h>
#include <utilitieslib/version/AppVersion.h>
#include <utilitieslib/network/sock.h>
#include <utilitieslib/utils/log.h>
#include <utilitieslib/network/net_linklist.h>
#include "comm_backend.h"
#include <utilitieslib/utils/timing.h>

int netGameClientLinkDelCallback(NetLink *link);

//this is duplicated exactly in clientcomm.c
static void sendMsg(NetLink *link,char *msg)
{
    //this isn't allowed because of dbClient_createPacket(DBGAMESERVER_SEND_PLAYERS, client);
    Packet    *pak        = pktCreateEx(link,DBGAMESERVER_MSG);

    pktSendString(pak,msg);
    pktSend(&pak,link);
}

static U32 correct_game_checksum[4],ignore_checksum;
void initGameChecksum()
{
    char    *s,full_path[MAX_PATH];
    strcpy(full_path,fileDataDir());
    s = strrchr(full_path,'/');
    if (s)
        *s = 0;
    strcat(full_path,"/Ouroboros.exe");
    if (!checksumFile(full_path,correct_game_checksum))
    {
        ignore_checksum = 1;
        printf("game checksum = 0, assuming dev mode\n");
    }
}
int clientCommLoginVerifyChecksum(U32 game_checksum[])
{
    if(sizeof(game_checksum) != sizeof(correct_game_checksum))
        return 0;
    return (!ignore_checksum && memcmp(game_checksum,correct_game_checksum,sizeof(game_checksum))!=0);
}

char *linkIpStr(NetLink *link)
{
    return makeIpStr(link->addr.sin_addr.S_un.S_addr);
}

static int connectCallback(NetLink *link)
{
    GameClientLink    *client;

    LOG_OLD("%s connected\n",linkIpStr(link));
    client = link->userData;
    client->link = link;
    client->valid = 0;
    return 1;
}

#define GAMECLIENT_ALLOCSIZE 1000
void clientCommLoginInit(NetLinkList *gameclient_links)
{
    netLinkListAlloc(gameclient_links,GAMECLIENT_ALLOCSIZE,sizeof(GameClientLink),connectCallback);
}

void clientCommLoginInitStartListening(NetLinkList *gameclient_links,NetPacketCallback callback, NetLinkDestroyCallback destroyCallback)
{
    initGameChecksum();
    netInitEnet(gameclient_links, DEFAULT_DBGAMECLIENT_PORT, 0);
    gameclient_links->destroyCallback = destroyCallback;
    gameclient_links->encrypted = packetCanUseEncryption();
    gameclient_links->publicAccess = 1;
    gameclient_links->destroyLinkOnDiscard = 1;
    NMAddLinkList(gameclient_links, callback);//processGameClientMsg);
}

int clientCommLoginConnectedCount(NetLinkList *gameclient_links)
{
    return gameclient_links->links->size;
}

bool clientKickLogin(U32 auth_id,int reason, NetLinkList *gameclient_links)
{
    int        i;
    GameClientLink    *client;
    bool clientFound = false;

    for(i=0;i<gameclient_links->links->size;i++)
    {
        client = ((NetLink*)gameclient_links->links->storage[i])->userData;
        if (client->auth_id == auth_id)
        {
            char buf[1000];

            sprintf(buf,"DBKicked %d",reason);
            sendMsg(client->link,buf);
            lnkBatchSend(client->link);
            netRemoveLink(client->link);
            clientFound = true;
        }
    }
    return clientFound;
}

void clientCommLoginClearDeadLinks(Array* links)
{
    int        i;
    U32        seconds,curr_seconds;
    NetLink    *link;
    static int timer;
    GameClientLink *game = 0;

    if (!timer)
        timer = timerAlloc();
    if (timerElapsed(timer) < 10 || !links)
        return;
    timerStart(timer);

    curr_seconds = timerCpuSeconds();
    for(i=links->size-1;i>=0;i--)
    {
        link = (NetLink*)links->storage[i];
        game = (GameClientLink *)link->userData;
        seconds = curr_seconds - link->lastRecvTime;
        if ((seconds > 100 && !link->notimeout)|| (game && game->remove && game->remove < curr_seconds))
        {
            netRemoveLink(link);
        }
    }
}