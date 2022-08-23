/***************************************************************************
 *     Copyright (c) 2006-2007, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include "auctioncmds.h"
#include "AuctionServer.h"
#include "cmdparse/cmdenum.h"
#include <utilitieslib/components/EString.h>
#include "comm_backend.h"
#include "auction/Auction.h"
#include "AuctionDb.h"
#include "cmdparse/cmdauction.h"
#include <utilitieslib/utils/cmdoldparse.h>
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/components/earray.h>

typedef struct CmdServerContext
{
     AuctionEnt *ent;
    int message_dbid;            // controlling entity db_id for csr commands
} CmdServerContext;


#define AUCTIONSVRCMD_HDR() \
    CmdServerContext *ctxt = cmd_context?cmd_context->svr_context:NULL; \
    AuctionEnt *ent = ctxt?ctxt->ent:NULL; \
    int message_dbid = ctxt?ctxt->message_dbid:0; \
    if(!ctxt) return;

// 'force' makes the command csroffline'able
// 'message_dbid' directs the output for csr'ed commands
static void s_RelayCmdRes(AuctionEnt *ent, int message_dbid, bool force, char *str) 
{
    char *res = NULL;
    Packet *pak = NULL;
    NetLink *link = AuctionShard_GetLink(SAFE_MEMBER(ent,shard));

    if(!link || !str)
        return;

    estrPrintf(&res,"cmdrelay_dbid %i %i\n%s",ent->ident,message_dbid,str);

    pak = pktCreateEx(link,AUCTION_SVR_RELAY_CMD_BYENT); 
    pktSendBitsPack(pak,1,ent->ident);
    pktSendBitsPack(pak,1,force);
    pktSendString(pak,res);
    pktSend(&pak,link);

    estrDestroy(&res);
}

static void AuctionSvrCmd_GetInv(CMDARGS)
{
    char *res = NULL;
    char *tmp = NULL;
    AUCTIONSVRCMD_HDR();
    if(!ent || !ent->shard)
        return;
    AuctionServer_EntInvToStr(&tmp,ent);
    estrPrintf(&res,"aucres_getinv \"%s\"",escapeString(tmp));
    s_RelayCmdRes(ent,message_dbid,true,res);
    estrDestroy(&tmp);
    estrDestroy(&res);
}

static void AuctionSvrCmd_RemInv(CMDARGS)
{
    char *res = NULL;
    char *tmp = NULL;
    int id;
    int argIndex = 0;
    AuctionInvItem *itm;
    AUCTIONSVRCMD_HDR();

    GETARG(id,int);

    if(!ent || !ent->shard)
        return;

    itm = AuctionServer_FindInvItm(ent, id);
    if (itm)
    {
        AuctionServer_RemInvItm(itm);
        AuctionDb_EntInsertOrUpdate(ent);

        AuctionServer_EntInvToStr(&tmp,ent);
        estrPrintf(&res,"aucres_getinv \"%s\"",escapeString(tmp));
        s_RelayCmdRes(ent,message_dbid,true,res);
    }
    estrDestroy(&tmp);
    estrDestroy(&res);
}

static void AuctionSvrCmd_LoginUpdate(CMDARGS)
{
    char *res = NULL;
    char *tmp = NULL;
    AUCTIONSVRCMD_HDR();
    if(!ent || !ent->shard)
        return;
    AuctionInventory_ToStr(&ent->inv, &tmp);
    estrPrintf(&res,"aucres_loginupdate \"%s\"",tmp);
    s_RelayCmdRes(ent,message_dbid,false,res);
    estrDestroy(&tmp);
    estrDestroy(&res);
}

static void AuctionSvrCmd_AccessLevel(CMDARGS)
{
    char *res = NULL;
    char *tmp = NULL;
    int accesslevel;
    int argIndex = 0;
    AUCTIONSVRCMD_HDR();

    GETARG(accesslevel,int);

    if(!ent || !ent->shard)
        return;

    AuctionEnt_SetAccessLevel(ent, accesslevel);
}

static void AuctionSvrCmd_CountEnts(CMDARGS)
{
    char *res = NULL;
    AUCTIONSVRCMD_HDR();

    if(!ent || !ent->shard)
        return;

    estrPrintf(&res,"conprint %i",eaSize(&g_AuctionDb.ents));
    s_RelayCmdRes(ent,message_dbid,false,res);
    estrDestroy(&res);
}


static void s_AucCmdHandler(CMDARGS)
{
    int i;
    static struct
    {
        ClientAuctionCmd cmd;
        CmdHandler fp;
    } s_handlers[] = 
        {
            {ClientAuctionCmd_GetInv,AuctionSvrCmd_GetInv},
            {ClientAuctionCmd_RemInv,AuctionSvrCmd_RemInv},
            {ClientAuctionCmd_LoginUpdate,AuctionSvrCmd_LoginUpdate},
            {ClientAuctionCmd_AccessLevel,AuctionSvrCmd_AccessLevel},
            {ClientAuctionCmd_PlayerCount,AuctionSvrCmd_CountEnts},
        };
    STATIC_INFUNC_ASSERT(ARRAY_SIZE(s_handlers)==ClientAuctionCmd_Count-ClientAuctionCmd_None-1);

    if(!cmd_context || !cmd_context->svr_context)
        return;

    for( i = 0; i < ARRAY_SIZE( s_handlers ); ++i ) 
    {
        if(s_handlers[i].cmd == cmd->num)
        {
            CmdHandler fp = s_handlers[i].fp;
            if(fp)
                fp(cmd,cmd_context);
            break;
        }
    }
}


static int cmdlist_initted = 0;

void AuctionServer_HandleSlashCmd(AuctionEnt *ent, int message_dbid, char *str)
{
    static CmdList server_cmdlist =  
        {
            {{ g_auction_cmds }, 
             { 0 }},
            s_AucCmdHandler // default function
        };
    CmdContext    context = {0};
    CmdServerContext ctxt = {ent, message_dbid};

    if(!devassert(ent && str))
        return;
    
    if(!cmdlist_initted)
    {
        cmdOldInit(g_auction_cmds);
        cmdlist_initted = 1;
    }
    
    context.access_level = 9;
    context.svr_context = &ctxt;
    cmdOldExecute(&server_cmdlist,str,&context);
}
