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
#ifndef XACTSERVER_H
#define XACTSERVER_H

#include "auction/Xact.h"

#define XACTION_TIMEOUT 300 // 5 min


// TODO: this enum doesn't mean anything anymore
typedef enum XactServerMode
{
    XactServerMode_Normal,
    XactServerMode_LogPlayback,
    XactServerMode_Count    
} XactServerMode;


typedef struct AuctionInvItem AuctionInvItem;

void XactServer_AddReq(XactCmdType xact_type, char const *context, AuctionShard *shard, int ident,char *name);
void XactServer_AddUpdate(int xid, int subid, XactCmdRes res);
char *XactServer_CreateFulfillContext(const char *pchIdentifier, AuctionInvItem *seller, AuctionInvItem *buyer, int amt);
char *XactServer_CreateFulfillAutoBuyContext(const char *pchIdentifier, AuctionInvItem *seller, int amt);
char *XactServer_CreateFulfillAutoSellContext(const char *pchIdentifier, AuctionInvItem *buyer, int amt);

void Xactserver_Tick();
//void XactServer_AbortUnfinishedXactions(bool bInMerge);

#endif //XACTSERVER_H
