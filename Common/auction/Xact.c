/***************************************************************************
 *     Copyright (c) 2006-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 *
 * $Workfile: $
 * $Revision: 1.36 $
 * $Date: 2006/02/07 06:13:59 $
 *
 * Module Description:
 *
 * Revision History:
 *
 ***************************************************************************/
#include "Xact.h"
#include "auction/Auction.h"
#include "gameComm/trayCommon.h"
#include <utilitieslib/components/EString.h>
#include <utilitieslib/utils/textparser.h>
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/assert/assert.h>
#include <utilitieslib/utils/error.h>
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/components/MemoryPool.h>
#include <utilitieslib/network/net_packet.h>
#include <utilitieslib/network/net_packetutil.h>
#include <utilitieslib/network/net_linklist.h>
#include <utilitieslib/network/net_link.h>
#include <utilitieslib/network/net_masterlist.h>
#include <utilitieslib/utils/timing.h>
 
StaticDefineInt XactCmdTypeEnum[] =
{
    DEFINE_INT
     {"None",            XactCmdType_None},
    {"Test",            XactCmdType_Test},
    {"DropAucEnt",        XactCmdType_DropAucEnt},
    {"SetInvSize",        XactCmdType_SetInvSize},
    {"AddInv",            XactCmdType_AddInvItem},
     {"GetInv",            XactCmdType_GetInvItem},
    {"ChangeInvStatus", XactCmdType_ChangeInvStatus},
    {"ChangeInvAmount", XactCmdType_ChangeInvAmount},
    {"GetInfStored",    XactCmdType_GetInfStored},
    {"GetAmtStored",    XactCmdType_GetAmtStored},
    {"FulfillAuction",    XactCmdType_FulfillAuction},
    {"ShardJump",        XactCmdType_ShardJump},
    {"SlashCmd",        XactCmdType_SlashCmd},
    {"FulfillAutoBuy",    XactCmdType_FulfillAutoBuy},
    {"FulfillAutoSell",    XactCmdType_FulfillAutoSell},
    DEFINE_END
};
STATIC_ASSERT(ARRAY_SIZE(XactCmdTypeEnum) == XactCmdType_Count+2);

StaticDefineInt XactCmdResEnum[] =
{
    DEFINE_INT
    {"None",kXactCmdRes_None},
    {"Waiting",kXactCmdRes_Waiting},
    {"Yes",kXactCmdRes_Yes},
    {"No",kXactCmdRes_No},
    DEFINE_END
};
STATIC_ASSERT(ARRAY_SIZE(XactCmdResEnum) == kXactCmdRes_Count+2);


const char *XactCmdType_ToStr(XactCmdType type)
{
    return StaticDefineIntRevLookup(XactCmdTypeEnum,type);
}

XactCmdType XactCmdType_FromStr(const char *str)
{
    return StaticDefineIntGetInt(XactCmdTypeEnum,str);
}

TokenizerParseInfo parse_XactCmd[] = 
{
    { "xid",            TOK_INT(XactCmd, xid,    0)},
    { "subid",            TOK_INT(XactCmd, subid,    0)},
    { "Type",            TOK_INT(XactCmd, type,    0), XactCmdTypeEnum},
    { "res",            TOK_INT(XactCmd, res,    0), XactCmdResEnum},
    { "context",        TOK_STRING(XactCmd, context,    0)},
    { "End",            TOK_END,        0},
    { "", 0, 0 },
};


bool xacttype_Valid( XactCmdType e )
{
    return INRANGE0(e, XactCmdType_Count); 
}


void XactCmd_Send(XactCmd *cmd, Packet *pak)
{
    pktSendBitsAuto( pak, cmd->xid);
    pktSendBitsAuto( pak, cmd->subid );
    pktSendBitsAuto( pak, cmd->type);
    pktSendBitsAuto( pak, cmd->res );
    pktSendString( pak, cmd->context ); 
}

void XactCmd_Recv(XactCmd **hcmd, Packet *pak)
{
    if(!devassert(hcmd))
        return;
    if((*hcmd))
        XactCmd_Destroy(*hcmd);

    (*hcmd) = XactCmd_Create(XactCmdType_None,NULL);
    (*hcmd)->xid = pktGetBitsAuto(pak);
    (*hcmd)->subid = pktGetBitsAuto(pak);
    (*hcmd)->type = pktGetBitsAuto(pak);
    (*hcmd)->res = pktGetBitsAuto(pak);
    (*hcmd)->context = pktGetString(pak);
    (*hcmd)->context = StructAllocString((*hcmd)->context);
}

MP_DEFINE(XactCmd);

XactCmd* XactCmd_Create(XactCmdType type, const char *context)
{
    XactCmd *res = NULL;
    
    // create the pool, arbitrary number
    MP_CREATE(XactCmd, 64);
    res = MP_ALLOC( XactCmd );
    if( res )
    {
        res->type = type;
        if(context)
            res->context = StructAllocString(context);
    }
    return res;
}

void XactCmd_Destroy( XactCmd *hItem )
{
    if(hItem)
    {
        StructClear(parse_XactCmd,hItem); 
        MP_FREE(XactCmd, hItem);
    }
}

bool XactCmd_ToStr(XactCmd *cmd, char **hDest)
{
    if(!cmd||!cmd->xid||!hDest)
        return false; 
    return ParserWriteTextEscaped(hDest,parse_XactCmd,cmd,0,0);
}

bool XactCmd_FromStr(XactCmd **hrescmd, char *str)
{
    if(!hrescmd)
        return FALSE;
    if(!(*hrescmd))
        (*hrescmd) = XactCmd_Create(XactCmdType_None,NULL);
    return ParserReadTextEscaped(&str,parse_XactCmd,(*hrescmd));
}

void XactCmd_SetContext(XactCmd *cmd, const char *context)
{
    if(!cmd)
        return;

    if(cmd->context)
        StructFreeString(cmd->context);
    cmd->context = context ? StructAllocString(context) : NULL;
}

XactCmd *XactCmd_Clone(XactCmd *from, const char *context)
{
    XactCmd *res = NULL;
    if(!from)
        return NULL;

    res = XactCmd_Create(XactCmdType_None,NULL);
    if(!StructCopy(parse_XactCmd,from,res,0,0))
    {
        XactCmd_Destroy(res);
        return NULL;
    }

    if(context)
        XactCmd_SetContext(res,context);

    return res;
}


MP_DEFINE(Xaction);
Xaction* Xaction_Create()
{
    Xaction *res = NULL;

    // create the pool, arbitrary number
    MP_CREATE(Xaction, 64);
    res = MP_ALLOC( Xaction );
    if( verify( res ))
    {
        res->timeCreated = timerSecondsSince2000();
        res->cmdCtr = 1;
    }
    return res;
}

void XactCmd_Destroy( XactCmd *hItem );
static void XactCmd_DestroyCallback(void* arg0)
{
    XactCmd_Destroy((XactCmd *)arg0);
}

void Xaction_Destroy( Xaction *xact )
{
    if(xact)
    {
        eaDestroyEx(&xact->cmds,XactCmd_DestroyCallback);
        MP_FREE(Xaction, xact);
    }
}
