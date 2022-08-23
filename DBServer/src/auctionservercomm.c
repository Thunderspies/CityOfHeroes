/***************************************************************************
 *     Copyright (c) 2006-2007, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include <utilitieslib/stdtypes.h>
#include "dbserver/auctionservercomm.h"
#include <utilitieslib/network/sock.h>
#include "dbrelay.h"
#include "auction/Xact.h"
#include "sql_fifo.h"
#include <utilitieslib/components/EString.h>
#include "dbserver/servercfg.h"
#include "dbinit.h"
#include "container.h"
#include "comm_backend.h"
#include <utilitieslib/network/netio.h>
#include <utilitieslib/network/net_packetutil.h>
#include <utilitieslib/network/net_packet.h>
#include "container_sql.h"
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/network/net_structdefs.h>
#include "auction/Auction.h"
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/assert/assert.h>
#include <utilitieslib/utils/error.h>
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/components/MemoryPool.h>
#include <utilitieslib/utils/file.h>
#include "turnstile/TurnstileServerCommon.h"

#include "accountservercomm.h"
#include "mapxfer.h"
#include <utilitieslib/network/netcomp.h>
#include <utilitieslib/utils/log.h>
#include "dbdispatch.h"

#define AUCTION_DEADMAN_TIME (60*2)
 
typedef struct AuctionServerInfo {
     NetLink **links;        // maintain multiple, round-robin links
    NetLink *sqlcb_link;    // keep track of which link is getting the recently-seen ents list
    U32 last_received;        // deadman's switch
    int link_rotor;
    bool bThrottle;
} AuctionServerInfo;

static AuctionServerInfo auction_state = {0};
static NetLinkList auction_links = {0};

NetLink *auctionLink(bool bThrottle)
{
    int count;

    if(bThrottle && auction_state.bThrottle)
        return NULL;

    count = eaSize(&auction_state.links);
    if(!count)
        return NULL;

    auction_state.link_rotor = (auction_state.link_rotor+1) % count;

    return eaGet(&auction_state.links,auction_state.link_rotor);
}


Packet *auctionCreatePkt(NetLink *auc_link, int cmd)
{
    return pktCreateEx(auc_link,cmd);
}

int auctionServerSecondsSinceUpdate(void)
{
    int secs = 9999;
    if(eaSize(&auction_state.links))
        secs = timerSecondsSince2000() - auction_state.last_received;
    return secs;
}

static void sqlAuctionGetActiveCallback(Packet *pak,U8 *cols,int col_count,ColumnInfo **field_ptrs,void* udata)  
{
    int i;
    int idx = 0;
    AuctionServerType type = (AuctionServerType)udata;
    NetLink *svr_lnk = (type == kAuctionServerType_Heroes ? auction_state.sqlcb_link : NULL);

    if(!svr_lnk || !svr_lnk->connected)
    {
        LOG_OLD_ERR("lost link to auction while fetching sql data");
        pktFree(pak);
        return;
    }

     pktSendBitsAuto( pak, col_count ); 

    for( i = 0; i < col_count; ++i ) 
    {
        int ContainerId = *(int *)(&cols[idx]);
        idx += field_ptrs[0]->num_bytes;
        pktSendBitsAuto(pak, ContainerId);
    }

    pktSend(&pak, svr_lnk); 
}

// *********************************************************************************
//  auction svr message handler
// *********************************************************************************

#define AUC_CLIENT_RECV_THROTTLED(xx) \
    int ident = pktGetBitsAuto(pak_in); \
    char *name; \
    NetLink *auc_link = auctionLink((xx));  \
    strdup_alloca( name,pktGetString(pak_in)); 
    
#define AUC_CLIENT_RECV() AUC_CLIENT_RECV_THROTTLED(true)
    
#define AUC_CLIENT_HDR() \
        pktSendBitsAuto( pak, ident); \
        pktSendString( pak, name);


#define AUC_SVR_RECV() \
            int ident = pktGetBitsAuto(pak_in); \
            EntCon *ent_con = containerPtr(ent_list, ident);   \
            NetLink *map_link = ent_con?dbLinkFromLockId(ent_con->lock_id):NULL;

// msgs from auction house.
static int auctionMsgCallback(Packet *pak_in,int cmd,NetLink *auc_link)
{
    AuctionServerType type = (AuctionServerType)auc_link->userData;

    if(type == kAuctionServerType_Heroes)
    {
        auction_state.last_received = timerSecondsSince2000();
    }
    else if(cmd!=AUCTION_SVR_CONNECT)
    {
        LOG_OLD_ERR("AuctionErrs","Got cmd %i before connect",cmd);
        return 1;
    }

    switch (cmd)
    {
    case AUCTION_SVR_CONNECT:
    {
        // run once after connection established
        int i;
        Packet *pak_out;

        for(i = eaSize(&auction_state.links)-1; i >= 0; --i)
        {
            NetLink *link = auction_state.links[i];
            if(link && link->connected &&
                auc_link->addr.sin_addr.S_un.S_addr != link->addr.sin_addr.S_un.S_addr)
            {
                LOG_OLD_ERR("AuctionErrs","Got a second AuctionServer link request from %s! Keeping the existing connection.", stringFromAddr(&auc_link->addr));
                netSendDisconnect(auc_link,0.f);
                break;
            }
        }
        if(i >= 0)
            break;

        auction_state.bThrottle = false;
        auction_state.last_received = timerSecondsSince2000();
        auc_link->compressionDefault = 1;
        auc_link->userData = (void*)kAuctionServerType_Heroes;
        eaPush(&auction_state.links,auc_link);

        // handshake
        pak_out = auctionCreatePkt(auc_link,AUCTION_CLIENT_CONNECT);
        pktSendBitsPack(pak_out,1,DBSERVER_AUCTION_PROTOCOL_VERSION);
        pktSendString(pak_out,*server_cfg.shard_name?server_cfg.shard_name:server_cfg.db_server);

        // ----------
        // send the update of players who haven't logged in in a while.
        // (only on the first connection)

        if(!auction_state.sqlcb_link && eaSize(&auction_state.links) == 1)
        {
            char *restrict = NULL;
            int days_active;

            if(!server_cfg.auction_last_login_delay)
            {
                server_cfg.auction_last_login_delay = 60;
                printf("Auction Inventory History duration not specified. using %i\n",server_cfg.auction_last_login_delay);
            }

            pktSendBits(pak_out,1,1);

            days_active = server_cfg.auction_last_login_delay;
            auction_state.sqlcb_link = auc_link;

            estrPrintf(&restrict,"WHERE LastActive > DATEADD(d, -%i, GETDATE())", days_active);
            sqlReadColumnsAsync(ent_list->tplt->tables,0,"ContainerId",restrict,sqlAuctionGetActiveCallback,0,pak_out,0);

            estrDestroy(&restrict);
        }
        else
        {
            pktSendBits(pak_out,1,0);
            pktSend(&pak_out,auc_link);
        }

        if( server_cfg.isLoggingMaster )
            sendLogLevels(auc_link,AUCTION_CLIENT_SET_LOG_LEVELS);
    }
    break;
    case AUCTION_SVR_SEND_INV:  
    {
        AUC_SVR_RECV();
        if( map_link )
        {
            Packet *pak_out = pktCreateEx(map_link, DBSERVER_AUCTION_SEND_INV);
            pktSendBitsAuto( pak_out, ident );                
            pktSendString(pak_out,pktGetString(pak_in));
            pktSend(&pak_out,map_link);
        }
    }
    break;
    case AUCTION_SVR_SEND_HISTORYINFO:  
    {
        AUC_SVR_RECV();
        if( map_link ) // TODO: forward this to auction maps, like batch info
        {
            int i, count;

            Packet *pak_out = pktCreateEx(map_link, DBSERVER_AUCTION_SEND_HIST);
            pktSendBitsAuto(pak_out,ident);
            pktSendString(pak_out,pktGetString(pak_in)); // identifier
            pktSendBitsAuto(pak_out,pktGetBitsAuto(pak_in)); // number buying
            pktSendBitsAuto(pak_out,pktGetBitsAuto(pak_in)); // number selling

            count = pktGetBitsAuto(pak_in); // history count
            pktSendBitsAuto(pak_out,count);
            for(i = 0; i < count; ++i)
            {
                pktSendBitsAuto(pak_out,pktGetBitsAuto(pak_in)); // date
                pktSendBitsAuto(pak_out,pktGetBitsAuto(pak_in)); // price
            }

            pktSend(&pak_out,map_link);
        }
    }
    break;
    case AUCTION_SVR_BATCH_SEND_ITEMINFO:
    {
        int        i;
        U8 *zipped_data = NULL;
        U32 zip_bytes = 0;
        U32 raw_bytes = 0;
        Packet *pak_out;
        AUC_SVR_RECV();

        zipped_data = pktGetZippedInfo(pak_in,&zip_bytes,&raw_bytes);

        for(i=0;i<map_list->num_alloced;i++)
        {
            MapCon    *map = (MapCon *) map_list->containers[i];
            if(map->is_static && map->active && map->link) // TODO: allow non-static maps
            {
                DBClientLink *client = map->link->userData;

                if (client->has_auctionhouse)
                {
                    Packet *pak = pktCreateEx(map->link,DBSERVER_AUCTION_BATCH_INFO);
                    pktSendZippedAlready(pak,raw_bytes,zip_bytes,zipped_data);
                    pktSend(&pak,map->link);
                }
            }
        }

        free(zipped_data);

        // Let the auctionserver know it's okay to send more
        pak_out = auctionCreatePkt(auc_link,AUCTION_CLIENT_DATAOK);
        pktSend(&pak_out,auc_link);
    }
    break;
    case AUCTION_SVR_XACT_CMD:
    {
        XactUpdateType type;
        XactCmd *cmd = NULL;
        AUC_SVR_RECV();

        type = pktGetBitsAuto(pak_in);
        XactCmd_Recv(&cmd,pak_in);

        if( map_link )
        {
            Packet *pak_out;
            
            pak_out = pktCreateEx(map_link,DBSERVER_AUCTION_XACT_CMD);
            pktSendBitsAuto( pak_out, ident);
            pktSendBitsAuto( pak_out, type);
            XactCmd_Send(cmd,pak_out);
            pktSend(&pak_out,map_link);
        }
        else if(type == kXactUpdateType_Run)
        {
            // abort the transaction
            Packet *pak_out = auctionCreatePkt(auc_link,AUCTION_CLIENT_XACT_UPDATE);
            pktSendBitsAuto(pak_out,cmd->xid);
            pktSendBitsAuto(pak_out,cmd->subid);
            pktSendBitsAuto(pak_out,kXactCmdRes_No);
            pktSend(&pak_out,auc_link);
        }
        else
            LOG_OLD_ERR("Player %d not loaded for commit or abort, type %d",ident,type);

        XactCmd_Destroy(cmd);
    }
    break;
    case AUCTION_SVR_RELAY_CMD_BYENT:
    {
        char    *msg;
        int        ent_id, force;
        
        ent_id = pktGetBitsPack(pak_in,1);
        force = pktGetBitsPack(pak_in,1);
        msg = pktGetString(pak_in);
        
        sendToEnt(NULL, ent_id, force, msg); 
    }
    break;
    case AUCTION_SVR_THROTTLE_ON:
    {
        auction_state.bThrottle = true;
    }
    break;
    case AUCTION_SVR_THROTTLE_OFF:
    {
        auction_state.bThrottle = false;
    }
    break;
    case AUCTION_SVR_ACCOUNT_SHARDXFER:
    {
        OrderId order_id = { pktGetBitsAuto2(pak_in), pktGetBitsAuto2(pak_in) };
        int dbid = pktGetBitsAuto(pak_in);
        accountAuctionShardXfer(order_id, dbid);
    }
    break;

    default:
        LOG_OLD_ERR("invalid net cmd %i",cmd);
    };
    return 1;
}
#undef AUC_SVR_RECV

static int auction_clientCreateCb(NetLink *link)
{
    netLinkSetMaxBufferSize(link, BothBuffers, 8*1024*1024); // Set max size to auto-grow to
    netLinkSetBufferSize(link, BothBuffers, 1*1024*1024);
    link->userData = (void*)kAuctionServerType_Invalid;
    return 1;
}

static int auction_clientDeleteCb(NetLink *link)
{
    AuctionServerType type = (AuctionServerType)link->userData;
    if(type == kAuctionServerType_Heroes)
    {
        eaFindAndRemove(&auction_state.links,link);
        if(auction_state.sqlcb_link == link)
            auction_state.sqlcb_link = NULL;
    }
    return 1;
}


void auctionMonitorInit(void)
{
    netLinkListAlloc(&auction_links,2,0,auction_clientCreateCb);
    auction_links.destroyCallback = auction_clientDeleteCb;
    if(netInit(&auction_links,0, DEFAULT_AUCTIONSERVER_HEROES_PORT))
        NMAddLinkList(&auction_links, auctionMsgCallback);
    else
        FatalErrorf("AuctionServerComm","Error listening for auction server.\n");
}

void auctionMonitorTick(void)
{
    U32 now = timerSecondsSince2000();

    if(eaSize(&auction_state.links) && auction_state.last_received + AUCTION_DEADMAN_TIME < now)
    {
        int j;

        printf("Haven't heard from auctionserver in over %is, disconnecting.\n",AUCTION_DEADMAN_TIME);
        LOG_OLD_ERR("Haven't heard from auctionserver in over %is, disconnecting.\n",AUCTION_DEADMAN_TIME);

        for(j = eaSize(&auction_state.links)-1; j >= 0; --j)
            if(auction_state.links[j])
                netSendDisconnect(auction_state.links[j],0);
    }
}



// *********************************************************************************
//  misc dbserver handlers
// *********************************************************************************

void auctionServerShardXfer(OrderId order_id, int dbid, char *dst_shard, int dst_dbid)
{
    NetLink *auc_link = auctionLink(false);

    if (auc_link)
    {
        Packet *pak_out = auctionCreatePkt(auc_link,AUCTION_CLIENT_ACCOUNT_SHARDXFER);
        pktSendBitsAuto2(pak_out,order_id.u64[0]);
        pktSendBitsAuto2(pak_out,order_id.u64[1]);
        pktSendBitsAuto(pak_out,dbid);
        pktSendString(pak_out,dst_shard);
        pktSendBitsAuto(pak_out,dst_dbid);
        pktSend(&pak_out,auc_link);
    }
    else if (isDevelopmentMode())
    {
        // Handle the case when there's no auction server running in dev mode.
        accountAuctionShardXfer(order_id, dbid);
    }
}


// *********************************************************************************
//  dbdispatch handlers
// *********************************************************************************

void handleAuctionClientReqInv(Packet *pak_in, NetLink *link)
{
    Packet *pak;
    AUC_CLIENT_RECV();

    if (link)
    {
        DBClientLink    *client = link->userData;
        client->has_auctionhouse = true;
    }

    if(!auc_link) 
        return;

    pak = auctionCreatePkt(auc_link,AUCTION_CLIENT_INVENTORY_REQUEST);
    if( pak )
    {
        AUC_CLIENT_HDR();
        pktSend(&pak, auc_link);
    }
}

void handleAuctionClientReqHistInfo(Packet *pak_in, NetLink *link)
{
    char *pchIdentifier;
    Packet *pak;
    AUC_CLIENT_RECV();
    pchIdentifier = pktGetString(pak_in);
    if(!auc_link) 
        return;    

    pak = auctionCreatePkt(auc_link,AUCTION_CLIENT_HISTORYINFO_REQUEST);
    if( pak )
    {
        AUC_CLIENT_HDR();
        pktSendString( pak, pchIdentifier);
        pktSend(&pak, auc_link);
    }
}

void handleAuctionXactReq(Packet *pak_in, NetLink *map_link)
{
    int xact_type;
    char *context;

    AUC_CLIENT_RECV();

    xact_type = pktGetBitsAuto(pak_in);
    strdup_alloca(context,pktGetString(pak_in));

    if(auc_link)
    {
        Packet *pak = auctionCreatePkt(auc_link, AUCTION_CLIENT_XACT_REQ);
        AUC_CLIENT_HDR();
        pktSendBitsAuto( pak, xact_type);
        pktSendString( pak, context );
        pktSend( &pak, auc_link);
    }
    else
    {
        Packet *pak = pktCreateEx(map_link, DBSERVER_AUCTION_XACT_FAIL);
        pktSendBitsAuto(pak,ident);
        pktSendBitsAuto(pak,xact_type);
        pktSendString(pak,context);
        pktSend(&pak,map_link);
    }
}

void handleAuctionXactMultiReq(Packet *pak_in, NetLink *map_link)
{
    char* context = NULL;
    int xact_type;
    int xacts;
    int count;

    AUC_CLIENT_RECV();

    xacts = pktGetBitsAuto(pak_in);

    if(auc_link)
    {
        Packet *pak = auctionCreatePkt(auc_link, AUCTION_CLIENT_XACT_MULTI_REQ);
        AUC_CLIENT_HDR();
        pktSendBitsAuto(pak,xacts);

        for(count = 0; count < xacts; count++)
        {
            pktSendGetBitsAuto(pak,pak_in);        // Xact type
            pktSendGetString(pak,pak_in);        // Context string
        }
        pktSend( &pak, auc_link);
    }
    else
    {
        Packet *pak = pktCreateEx(map_link, DBSERVER_AUCTION_XACT_FAIL);
        for(count = 0; count < xacts; count++)
        {
            xact_type = pktGetBitsAuto(pak_in);
            context = pktGetString(pak_in);
        }
        pktSendBitsAuto(pak,ident);
        pktSendBitsAuto(pak,XactCmdType_None);
        pktSendString(pak,context);
        pktSend(&pak,map_link);
    }
}

void handleAuctionXactUpdate(Packet *pak_in, NetLink *map_link)
{
    NetLink *auc_link = auctionLink(false);
    if(auc_link)
    {
        Packet *pak_out = auctionCreatePkt(auc_link,AUCTION_CLIENT_XACT_UPDATE);
        pktSendBitsAuto(pak_out,pktGetBitsAuto(pak_in)); // xid
        pktSendBitsAuto(pak_out,pktGetBitsAuto(pak_in)); // subid
        pktSendBitsAuto(pak_out,pktGetBitsAuto(pak_in)); // res
        pktSend(&pak_out,auc_link);
    }
}

void handleSendAuctionCmd(Packet *pak_in, NetLink *link)
{
    char *str;
    Packet *pak;
    int message_dbid;

    AUC_CLIENT_RECV();

    message_dbid = pktGetBitsAuto(pak_in);
    strdup_alloca(str,pktGetString(pak_in));

    if(!auc_link)
    {
        char *msg = NULL;
        int target_ident = message_dbid ? message_dbid : ident;
        estrPrintf(&msg,"cmdrelay_dbid %i\naucres_err \"auctionhouse not connected\"",target_ident);
        sendToEnt(NULL,target_ident,0,msg);
        estrDestroy(&msg);
        return;
    }
    
    pak = auctionCreatePkt(auc_link,AUCTION_CLIENT_SLASHCMD);
    if(pak)
    {
        AUC_CLIENT_HDR();
        pktSendBitsAuto(pak,message_dbid);
        pktSendString(pak, str);
        pktSend(&pak,auc_link);
    }
}

void handleAuctionPurgeFake(Packet *pak_in, NetLink *link)
{
    Packet *pak;

    AUC_CLIENT_RECV();

    if(auc_link)
    {
        pak = auctionCreatePkt(auc_link,AUCTION_CLIENT_PURGE_FAKE);
        if(pak)
        {
            AUC_CLIENT_HDR();
            pktSend(&pak,auc_link);
        }
    }
}
