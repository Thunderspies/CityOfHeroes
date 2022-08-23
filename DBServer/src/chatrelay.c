#include <stdio.h>
#include <utilitieslib/components/EString.h>
#include "chatrelay.h"
#include "comm_backend.h"
#include "dbserver/servercfg.h"
#include <utilitieslib/components/StashTable.h>
#include <utilitieslib/components/earray.h>
#include "logserver.h"
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/utils/log.h>
#include <utilitieslib/utils/utils.h>
#include "dbdispatch.h"

extern NetLinkList    log_links;

NetLink    shard_comm;
Packet    *aggregate_pak;
int g_chatserver_recv_msgs;
int g_chatserver_sent_msgs;

static StashTable    id_hashes;

void flushAggregate()
{
    if (aggregate_pak)
        pktSend(&aggregate_pak,&shard_comm);
    aggregate_pak = 0;
}

static void shardSend(U32 id,char *str)
{
    if (!server_cfg.chat_server[0])
        return;
    if (!aggregate_pak)
    {
        shard_comm.controlCommandsSeparate = 1;
        shard_comm.compressionAllowed = 1;
        shard_comm.compressionDefault = 1;
        aggregate_pak = pktCreateEx(&shard_comm,SHARDCOMM_CMD);
    }
    pktSendBits(aggregate_pak,32,id);
    pktSendString(aggregate_pak,str);
    g_chatserver_sent_msgs++;
    if (aggregate_pak->stream.size > 500000)
        flushAggregate();
}

static int unlinkEnt(U32 id,NetLink *link)
{
    NetLink    *old_link;

    if (stashIntFindPointer(id_hashes, id, &old_link) && (old_link == link || !link))
    {
        LogClientLink    *client = old_link->userData;

        stashIntRemovePointer(id_hashes, id, NULL);
        // HACK! damn sloppy cleanup code.
        if(client != (void*)(intptr_t)0xfafafafa)
            eaiFindAndRemove(&client->linked_auth_ids,id);
        return 1;
    }
    return 0;
}

static void linkEnt(U32 id,NetLink *link)
{
    LogClientLink    *client = link->userData;

    unlinkEnt(id,0);
    stashIntAddPointer(id_hashes, id, link, false);
    eaiPush(&client->linked_auth_ids,id);
}

void shardLogoutStranded(NetLink *link)
{
    int        i;
    U32        id;
    LogClientLink    *client = link->userData;

    for(i=eaiSize(&client->linked_auth_ids)-1;i>=0;i--)
    {
        id = client->linked_auth_ids[0];
        if (unlinkEnt(id,link))
            shardSend(id,"Logout");
    }
}

void shardChatRelay(Packet *pak,NetLink *link)
{
    U32        id;
    char    *str;

    if (!server_cfg.chat_server[0])
        return;
    while(!pktEnd(pak))
    {
        id = pktGetBits(pak,32);
        str = pktGetString(pak);
        if (stricmp(str,"LinkEnt")==0)
            linkEnt(id,link);
        else if (stricmp(str,"UnlinkEnt")==0)
            unlinkEnt(id,link);
        else
            shardSend(id,str);
    }
}

int msg_count,repeat_count;

// forward messages to proper maps
static int shardMessageCallback(Packet *pak_in,int cmd,NetLink *link)
{
    static char    *str = NULL;
    U32        auth_id,repeat;
    NetLink    *map_link,*last_link=0;
    Packet    *pak=0;

    if (!shard_comm.connected)
        return 1;
    if(cmd != SHARDCOMM_SVR_CMD)
        return 0;
    if(!str)
        estrCreateEx(&str,1000);
    
    while(!pktEnd(pak_in))
    {
        msg_count++;
        g_chatserver_recv_msgs++;
        auth_id = pktGetBitsPack(pak_in,31);
        repeat = pktGetBits(pak_in,1);
        if (!repeat)
            estrPrintCharString(&str,pktGetString(pak_in));
        else
            repeat_count++;
        if (stashIntFindPointer(id_hashes, auth_id, &map_link))
        {
            if (map_link != last_link)
            {
                if (pak)
                    pktSend(&pak,last_link);
                pak = 0;
                last_link = map_link;
            }
            if (!pak)
                pak = pktCreateEx(link,LOGSERVER_SHARDCHAT);
            pktSendBits(pak,32,auth_id);
            pktSendBits(pak,1,repeat);    // only necessary to make logging chat messages in dblogs cleaner
            pktSendString(pak,str);        
            
             if(pak->stream.size > 500000)
            {
                pktSend(&pak, last_link);
                pak = 0;
            }
        }

    }
    if (pak)
        pktSend(&pak,last_link);
    return 1;
}

static bool someone_needs_chat_status;
void shardChatFlagForSendStatusToMap(NetLink *link)
{
    LogClientLink    *client = link->userData;
    client->needs_chat_status = 1;
    someone_needs_chat_status = true;

}

void shardChatSendStatusToMap(NetLink *link)
{
    LogClientLink    *client = link->userData;
    char    buf[200];
    Packet    *pak;

    link->controlCommandsSeparate = 1;
    pak = pktCreateEx(link,LOGSERVER_SHARDCHAT);
    sprintf(buf,"ChatServerStatus %d",shard_comm.connected);
    pktSendBits(pak,32,0);
    pktSendBits(pak,1,0);
    pktSendString(pak,buf);
    pktSend(&pak,link);
    client->needs_chat_status = 0;
}

static void sendStatusAll()
{
    int        i;
    static int was_connected;
    int full_update = shard_comm.connected != was_connected;

    if (!full_update && !someone_needs_chat_status)
        return;
    was_connected = shard_comm.connected;
    someone_needs_chat_status = false;
    for (i=0; i<log_links.links->size; i++) {
        NetLink *link = log_links.links->storage[i]; 
        if (link->connected) {
            if (full_update || ((LogClientLink*)link->userData)->needs_chat_status)
                shardChatSendStatusToMap(log_links.links->storage[i]);
        } else {
            // Didn't send an update
            someone_needs_chat_status = true;
        }
    }
}

void shardChatInit()
{
    if (id_hashes)
        stashTableDestroy(id_hashes);
    id_hashes = stashTableCreateInt(4);
}

void shardChatMonitor()
{
    char    buf[1000];
    static int timer;

    if (!server_cfg.chat_server[0])
        return;
    sendStatusAll();
    if (!shard_comm.connected)
    {
        if (!shard_comm.pending)
        {
            shardChatInit();
            if (timer && timerElapsed(timer) < 10)
                return;
            if (!timer)
                timer = timerAlloc();
            timerStart(timer);

            int connectResult = netConnectAsync(&shard_comm, server_cfg.chat_server, DEFAULT_CHATSERVER_PORT, NLT_TCP, 1);
            if (connectResult != 1)
            {
                writeConsole(OUTPUT_WARNING, "Can't start connecting to chatserver - make sure it's running\n");
            }
            return;
        }

        int result = netConnectPoll(&shard_comm);
        switch (result)
        {
        case -1: // Error; fall through
            writeConsole(OUTPUT_WARNING, "Can't connect to chatserver - make sure it's running\n");
            return;
        case 0: // Still trying
            return;
        }

        // Success
        writeConsole(OUTPUT_INFO, "Successfully connected to chatserver!\n");
        if (server_cfg.shard_name[0])
        {
            sprintf(buf, "ShardLogin \"%s\"", server_cfg.shard_name);
            shardSend(0, buf);
            flushAggregate();
        }
        if (server_cfg.isLoggingMaster)
        {
            sendLogLevels(&shard_comm, SHARDCOMM_SET_LOG_LEVELS);
        }
        netLinkSetMaxBufferSize(&shard_comm, BothBuffers, 8 * 1024 * 1024); // Set max size to auto-grow to
        netLinkSetBufferSize(&shard_comm, BothBuffers, 1 * 1024 * 1024);
    }

    netLinkMonitor(&shard_comm, 0, shardMessageCallback);
    lnkBatchSend(&shard_comm);
    flushAggregate();
}

