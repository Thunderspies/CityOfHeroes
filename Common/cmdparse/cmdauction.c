/***************************************************************************
 *     Copyright (c) 2006-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/

#include "cmdauction.h"
#include "AppServerCmd.h"
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/assert/assert.h>
#include <utilitieslib/utils/error.h>
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/components/EString.h>
#include <utilitieslib/components/MemoryPool.h>
#include <utilitieslib/components/StashTable.h>
#include <utilitieslib/utils/cmdoldparse.h>  // tied to mapserver now
#include "cmdenum.h"
#include "comm_backend.h"

#define TMP_STR {CMDSTR(g_appserver_cmd_state.strs[0])}
#define TMP_STR2 {CMDSTR(g_appserver_cmd_state.strs[1])}
#define TMP_INT {PARSETYPE_S32, &g_appserver_cmd_state.ints[0]}
#define TMP_INT2 {PARSETYPE_S32, &g_appserver_cmd_state.ints[1]}
#define ID_SGRP {PARSETYPE_S32, &g_appserver_cmd_state.idSgrp}
#define ID_ENT  {PARSETYPE_S32, &g_appserver_cmd_state.idEnt}
#define TMP_SENTENCE {CMDSENTENCE(g_appserver_cmd_state.strs[0])}

Cmd g_auction_cmds[] =
{
    { 1, "auc_getinv", ClientAuctionCmd_GetInv, {0}, 0,
        "get player's auction inventory"},
    { 1, "auc_reminv", ClientAuctionCmd_RemInv, {TMP_INT}, 0,
        "<id> remove item from player's auction inventory"},
    { 0, "auc_loginupdate", ClientAuctionCmd_LoginUpdate, {0}, 0,
        "get status of auction inventory info to the player"},
    { 9, "auc_accesslevel", ClientAuctionCmd_AccessLevel, {TMP_INT}, 0,
        "allow actions like selling to yourself"},
    { 1, "auc_PlayerCount", ClientAuctionCmd_PlayerCount, {0}, 0,
        "get the total number of characters the auctionserver knows about"},
    { 0 },
};

// these commands come from the auctionserver
Cmd g_auction_res[] = 
{
    { 9, "aucres_getinv", AuctionCmdRes_GetInv, {TMP_STR} , CMDF_HIDEPRINT,
        "[auctionserver response command]"},
    { 9, "aucres_err", AuctionCmdRes_Err, {TMP_STR} , CMDF_HIDEPRINT,
        "[auctionserver response command]"},
    { 9, "aucres_loginupdate", AuctionCmdRes_LoginUpdate, {TMP_SENTENCE}, CMDF_HIDEPRINT,
        "[auctionserver response command]"},
    { 0 },
};
