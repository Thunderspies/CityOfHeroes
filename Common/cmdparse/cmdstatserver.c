/***************************************************************************
 *     Copyright (c) 2005-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include "cmdstatserver.h"
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
#include "cmdenum.h"
#include "dbcomm/dbcomm.h"
#include "comm_backend.h"

#define TMP_STR {CMDSTR(g_appserver_cmd_state.strs[0])}
#define TMP_STR2 {CMDSTR(g_appserver_cmd_state.strs[1])}
#define TMP_INT  {PARSETYPE_S32, &g_appserver_cmd_state.ints[0]}
#define TMP_INT2 {PARSETYPE_S32, &g_appserver_cmd_state.ints[1]}
#define TMP_INT3 {PARSETYPE_S32, &g_appserver_cmd_state.ints[2]}
#define TMP_INT4 {PARSETYPE_S32, &g_appserver_cmd_state.ints[3]}
#define TMP_INT5 {PARSETYPE_S32, &g_appserver_cmd_state.ints[4]}
#define TMP_INT6 {PARSETYPE_S32, &g_appserver_cmd_state.ints[5]}
#define TMP_INT7 {PARSETYPE_S32, &g_appserver_cmd_state.ints[6]}
#define ID_SGRP {PARSETYPE_S32, &g_appserver_cmd_state.idSgrp}
#define ID_ENT  {PARSETYPE_S32, &g_appserver_cmd_state.idEnt}

Cmd client_sgstat_cmds[] =
{
    { 2, "sgstat_statadj", SGRPSTATCMD_STATADJ,{TMP_STR,TMP_INT},0,"adjust named stat by amount n." },
    { 2, "sgstat_make_rent_overdue", SGRPSTATCMD_SETRENTOVERDUE, {TMP_INT} ,0,"set the rent as due for N periods."},
    { 2, "sgstat_set_rent", SGRPSTATCMD_SETRENT, {TMP_INT} ,0,"set the rent amount due. Does not set due date."},
    { 2, "sgstat_pay_rent_free", SGRPSTATCMD_PAYRENTFREE, {0} ,0,"pay group's rent without charging group prestige."},
    { 9, "sgstat_passthru", SGRPSTATCMD_PASSTHRU,{TMP_STR},0,"pass command string to statserver." },
    { 2, "sgstat_sgrp_update", SGRPSTATCMD_SGRPUPDATE,{TMP_STR},0,"pass command string to statserver." },
    { 2, "sgrp_badge_give", SGRPSTATCMD_GIVEBADGE,{TMP_STR},0,"give the named badge to current player's supergroup" },
    { 2, "sgrp_badge_revoke", SGRPSTATCMD_REVOKEBADGE,{TMP_STR},0,"remove the named badge from current player's supergroup" },
    { 2, "sgrp_stats_show", SGRPSTATCMD_STATSHOW,{TMP_STR},CMDF_RETURNONERROR,"show stats for the supergroup.  Optional parameter shows stats with the given string in the stat name." },
    { 2, "sgrp_stat_set", SGRPSTATCMD_STATSET,{TMP_STR, TMP_INT},0,"set stat for the supergroup"},
    { 9, "sgrp_stats_hammer", SGRPSTATCMD_HAMMER,{TMP_INT,TMP_INT2},0,"send the passed amount to every stat on the supergroup N times, flushing after each." },
    { 9, "sgrp_baseentrypermissions_req", SGRPSTATCMD_BASEENTRYPERMISSIONS_REQ,{TMP_INT, TMP_STR},0,"request the baseentry permissions for the given sgrps." },
    { 2, "sgrp_info", SGRPSTATCMD_SGRP_INFO,{0},0,"print info about your supergroup." },
    { 2, "sgrp_info_name", SGRPSTATCMD_SGRP_INFO_NAME,{TMP_STR},0,"print info about the named supergroup." },
    { 2, "sgrp_info_id", SGRPSTATCMD_SGRP_INFO_ID,{ID_SGRP},0,"print info about your supergroup." },
    { 9, "statserver_levelingpact_join", SGRPSTATCMD_LEVELINGPACT_JOIN, {TMP_INT,TMP_INT2,TMP_INT3,TMP_INT4,TMP_STR, TMP_INT5} },
    { 9, "statserver_levelingpact_addxp", SGRPSTATCMD_LEVELINGPACT_ADDXP, {TMP_INT,TMP_INT2, TMP_INT3} },
    { 9, "statserver_levelingpact_addinf", SGRPSTATCMD_LEVELINGPACT_ADDINFLUENCE, {TMP_INT,TMP_INT2, TMP_INT3} },
    { 9, "statserver_levelingpact_getinfluence", SGRPSTATCMD_LEVELINGPACT_GETINFLUENCE, {TMP_INT} },
    { 9, "statserver_levelingpact_updateversion", SGRPSTATCMD_LEVELINGPACT_UPDATEVERSION, {TMP_INT, TMP_INT2} },
    { 9, "statserver_league_join", STATCMD_LEAGUE_JOIN, {TMP_INT, TMP_INT2, TMP_INT3, TMP_STR, TMP_INT4, TMP_INT5} },
    { 9, "statserver_league_join_turnstile", STATCMD_LEAGUE_JOIN_TURNSTILE, {TMP_INT, TMP_INT2, TMP_STR, TMP_INT3, TMP_INT4} },
    { 9, "statserver_league_promote", STATCMD_LEAGUE_PROMOTE, {TMP_INT, TMP_INT2} },
    { 9, "statserver_league_change_team_leader_team", STATCMD_LEAGUE_CHANGE_TEAM_LEADER_TEAM, {TMP_INT, TMP_INT2, TMP_INT3}},
    { 9, "statserver_league_change_team_leader_solo", STATCMD_LEAGUE_CHANGE_TEAM_LEADER_SOLO, {TMP_INT, TMP_INT2, TMP_INT3}},
    { 9, "statserver_league_update_team_lock", STATCMD_LEAGUE_UPDATE_TEAM_LOCK, {TMP_INT, TMP_INT2, TMP_INT3}},
    { 9, "statserver_league_quit", STATCMD_LEAGUE_QUIT, {TMP_INT, TMP_INT2, TMP_INT3} },
    { 0 },
};

Cmd server_sgstat_cmds[] = 
{
    { 9, "svr_sgstat_statadj", SERVER_SGRPSTATCMD_STATADJ,{TMP_STR},0,"" },
    { 9, "svr_sgstat_conprintf_response", SERVER_SGRPSTATCMD_CONPRINTF_RESPONSE,{ID_ENT, TMP_STR},0, "prints a response to a statcmd on the invoker's console. Also translates strings enclosed in braces '{}'"},
    { 9, "sg_base_rentdue_relay", SCMD_SGRP_BASE_RENTDUE_FROMENTID_RELAY, {TMP_INT}, 0,
                            "lets each ent know when their rent has become due." },
    { 9, "svr_sgstat_cmdfailed_response", SERVER_SGRPSTATCMD_CMDFAILED_RESPONSE,{TMP_STR}},
    { 9, "svr_sgstat_cmdfailed2_response", SERVER_SGRPSTATCMD_CMDFAILED_RESPONSE,{TMP_STR, TMP_STR2}},
    { 9, "svr_sgstat_statshow_response", SERVER_SGRPSTATCMD_STATSHOW_RESPONSE, {TMP_STR,TMP_STR2}},
    { 9, "svr_sgstat_localized_message", SERVER_SGRPSTATCMD_LOCALIZED_MESSAGE, {TMP_INT,TMP_STR,TMP_STR2}},
    { 0 },
};

void SgrpStat_SendPassthru(int idEnt, int idSgrp, char *format, ...)
{
    char *cmd = NULL;
    va_list args;
    Packet *pak_out = pktCreateEx(&db_comm_link, DBCLIENT_SEND_STATSERVER_CMD);

    estrCreate(&cmd);
    va_start(args, format);
    estrClear(&cmd);
    estrConcatfv(&cmd, format, args);
    va_end(args);

    // send it
    pktSendBitsPack(pak_out, 1, idEnt); // okay if 0, won't get response
    pktSendBitsPack(pak_out, 1, idSgrp);// okay if 0 as long as id is in the cmd itself
    pktSendString(pak_out, cmd);
    pktSend(&pak_out,&db_comm_link);

    estrDestroy(&cmd);
}
