/***************************************************************************
 *     Copyright (c) 2007-2007, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 *
 * $Workfile: $
 * $Revision: 56548 $
 * $Date: 2010-02-19 16:07:59 -0800 (Fri, 19 Feb 2010) $
 *
 * Module Description:
 *
 * Revision History:
 *
 ***************************************************************************/
#include <utilitieslib/stdtypes.h>
#include "cmdparse/cmdaccountserver.h"
#include "AppServerCmd.h"
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/assert/assert.h>
#include <utilitieslib/utils/error.h>
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/components/EString.h>
#include <utilitieslib/components/MemoryPool.h>
#include <utilitieslib/components/StashTable.h>
#include <utilitieslib/utils/cmdoldparse.h>
#include "cmdparse/cmdcommon_enum.h"
#include "comm_backend.h"

#define TMP_STR {CMDSTR(g_appserver_cmd_state.strs[0])}
#define TMP_STR2 {CMDSTR(g_appserver_cmd_state.strs[1])}
#define TMP_INT {PARSETYPE_S32, &g_appserver_cmd_state.ints[0]}
#define TMP_INT2 {PARSETYPE_S32, &g_appserver_cmd_state.ints[1]}
#define ID_SGRP {PARSETYPE_S32, &g_appserver_cmd_state.idSgrp}
#define ID_ENT  {PARSETYPE_S32, &g_appserver_cmd_state.idEnt}
#define TMP_SENTENCE {CMDSENTENCE(g_appserver_cmd_state.strs[0])}

// access level is not validated by AccountServer!
Cmd g_accountserver_cmds_server[] =
{
    { 3, "acc_serverstatus", ClientAccountCmd_ServerStatus, {0}, 0,
        "get the status of the account server."},
    { 5, "acc_loadaccount", ClientAccountCmd_CsrSearchForAccount, {TMP_STR}, 0,
        "find the offline account. Args: <account>"},
    { 3, "acc_accountinfo", ClientAccountCmd_CsrAccountInfo, {TMP_STR}, 0,
        "get the account information for a particular account"},
    { 6, "acc_debug_buyproduct", ClientAccountCmd_BuyProductFromStore, {TMP_INT,TMP_STR,TMP_INT2}, 0,
        "buys a product from the store. acc_debug_buyproduct [authid] [sku] [quantity]."},
    { 7, "acc_debug_publish_product", ClientAccountCmd_PublishProduct, {TMP_STR,TMP_STR2,TMP_INT}, 0,
        "sets a product to published or unpublished state: [sku] [1=publish,0=unpublish]."},
    { 7, "shardxfer_source", ClientAccountCmd_ShardXferSrc, {TMP_STR,TMP_INT}, 0,
        "disable/enable character transfer from a shard.  shardxfer_source [shardname] [0/1]"},
    { 7, "shardxfer_destination", ClientAccountCmd_ShardXferDst, {TMP_STR,TMP_INT}, 0,
        "disable/enable character transfer to a shard.  shardxfer_destination [shardname] [0/1]"},
    { 7, "shardxfer_link", ClientAccountCmd_ShardXferSrc, {TMP_STR,TMP_STR2,TMP_INT}, 0,
        "disable/enable character transfer from one shard to another.  shardxfer_link [source] [destination] [0/1]"},
    { 7, "shardxfer_nocharge", ClientAccountCmd_ShardXferNoCharge, {TMP_STR, TMP_INT}, 0,
        "start a no charge character transfer to [destination] [isVIP?]"},
    { 3, "acc_tokeninfo", ClientAccountCmd_TokenInfo, {TMP_STR}, 0,
        "get (character slot/rename/transfer) token information for a particular authname"},
    { 5, "acc_set_account_flags", ClientAccountCmd_SetAccountFlags, {TMP_STR,TMP_INT,TMP_INT2}, 0,
        "sets or clears accounts flags. Args: <flags> <setOrClear>. 1=set, 0=clear."},
    { 3, "acc_account_loyalty_show", ClientAccountCmd_CsrAccountLoyaltyShow, {TMP_STR}, 0,
        "displays account loyalty status. Args: <account>"},
    { 3, "acc_account_product_show", ClientAccountCmd_CsrAccountProductShow, {TMP_STR}, 0,
        "displays account cert status. Args: <account>."},
    { 3, "acc_account_query_inventory", ClientAccountCmd_CsrAccountQueryInventory, {TMP_STR, TMP_STR2}, 0,
        "displays count of owned product by account. Args: <account> <product_sku>."},
    { 6, "acc_account_loyalty_refund", ClientAccountCmd_CsrAccountLoyaltyRefund, {TMP_STR, TMP_STR2}, 0,
        "refunds the specified loyalty node on account. Args: <account> <node_name>."},
    { 4, "acc_account_product_change", ClientAccountCmd_CsrAccountProductChange, {TMP_STR, TMP_STR2, TMP_INT}, 0,
        "change the product grant count on account (negative amounts will remove). Args: <account> <product_sku> <amount>."},
    { 4, "acc_account_product_reconcile", ClientAccountCmd_CsrAccountProductReconcile, {TMP_STR}, 0,
        "scan PlaySpan for missing products on account. Args: <account>."},
    { 0 } ,
};

#if defined(SERVER) || defined(CLIENT)
extern NetLink db_comm_link;
void AccountServer_ClientCommand(Cmd *cmd, int message_dbid, U32 auth_id, int dbid, char *source_str)
{
    Packet *pak_out = NULL;

    if(!source_str || !cmd || !(message_dbid || auth_id))
        return;

    switch(cmd->num)
    {
        default:
            pak_out = pktCreateEx(&db_comm_link,DBCLIENT_ACCOUNTSERVER_CMD);
            break;
    };

    if(!pak_out)
        return;
    
    pktSendBitsAuto(pak_out,message_dbid);
    pktSendBitsAuto(pak_out,auth_id);
    pktSendBitsAuto(pak_out,dbid);
    pktSendString(pak_out,source_str);
    pktSend(&pak_out,&db_comm_link);
}
#endif
