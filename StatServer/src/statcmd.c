/***************************************************************************
 *     Copyright (c) 2005-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include "statcmd.h"
#include "dbcomm/dbcomm.h"
#include "cmdparse/AppServerCmd.h"
#include "container/containerSupergroup.h"
#include "storyarc/storyarcprivate.h"
#include "entity/RewardToken.h"
#include "bases/DetailRecipe.h"
#include "bases/basedata.h"
#include <utilitieslib/utils/timing.h>
#include "bases/BaseEntry.h"
#include "statdb.h"
#include "cmdparse/cmdstatserver.h"
#include "gameData/sgrpstatsstruct.h"
#include <utilitieslib/components/EString.h>
#include <utilitieslib/utils/file.h>
#include "dbcomm/dbcontainer.h"
#include <utilitieslib/utils/error.h>
#include "entity/entity.h"
#include "entity/entsend.h"
#include "comm_backend.h"
#include "statserver.h"
#include "entity/Supergroup.h"
#include "container/containerloadsave.h"
#include <utilitieslib/utils/textparser.h>
#include "entity/SgrpStats.h"
#include <utilitieslib/components/earray.h>
#include "Entity/stat_SgrpBadges.h"
#include "player/BadgeStats.h"
#include "entity/SgrpBadges.h"
#include <utilitieslib/utils/fileutil.h>
#include "cmdparse/cmdenum.h"
#include <utilitieslib/utils/cmdoldparse.h>
#include <utilitieslib/components/bitfield.h>
#include "bases/baseupkeep.h"
#include "statgroups.h"
#include <utilitieslib/components/StringCache.h>
#include <utilitieslib/utils/log.h>

// *********************************************************************************
// statserver command
// ********************************************************************************

static char *s_esResponseBuf = NULL;
static int s_idSgrp;

static int s_responsePrintf(const char *format, ...)
{
    int result;
    va_list argptr;

    va_start(argptr, format);
    result = estrConcatfv(&s_esResponseBuf, format, argptr );
    va_end(argptr);
    
    return result;
}

static void cmdOldFailed(const char* format, ...)
{
    int count;
    va_list args;

    estrPrintf(&s_esResponseBuf, "svr_sgstat_cmdfailed_response \"");

    va_start(args, format);
    count = estrConcatfv(&s_esResponseBuf, format, args);
    va_end(args);

    estrConcatChar(&s_esResponseBuf, '\"');
}

static void cmdFailed2(const char *arg1, const char *arg2)
{
    estrPrintf(&s_esResponseBuf, "svr_sgstat_cmdfailed2_response \"%s\" \"%s\"", arg1, arg2);
}

static void s_sgrpstatserver_ResponseRelay(int idEntSrc, char *str)
{
    char *msg = estrTemp();
    Packet *pak;

    // send the id with the response cmd, and then the response itself
    // the response itself can be multi line and include multiple commands
    estrPrintf( &msg, "server_sgstat_cmd %i\n%s", idEntSrc, str);

    pak = pktCreateEx(&db_comm_link, DBCLIENT_RELAY_CMD_BYENT);
    pktSendBitsPack(pak,1,idEntSrc);
    pktSendBitsPack(pak,1,0); // don't force
    pktSendString(pak,msg);
    pktSend(&pak, &db_comm_link);
    estrDestroy(&msg);
}

void stat_EntLocalizedMessage(int idEnt, int channel, char *msgId, char *param)
{
    char *buf = estrTemp();
    estrPrintf(&buf, "svr_sgstat_localized_message %d \"%s\" \"%s\"", channel, msgId, param ? param : "");
    s_sgrpstatserver_ResponseRelay(idEnt, buf);
    estrDestroy(&buf);
}

static void s_RevokeBadge(AppServerCmdState *state, int idSgrp, Supergroup *sg)
{
    if( sg )
    {
        const BadgeDef *pdef = stashFindPointerReturnPointerConst( g_SgroupBadges.hashByName, state->strs[0] );
        
        if( !pdef )
        {
            cmdFailed2("SgrpStatCmdCouldntFindBadgeName", state->strs[0]);
        }
        else if(!sgrpbadges_IsOwned(&sg->badgeStates, pdef->iIdx))
        {
            cmdOldFailed("SgrpStatCmdFailed_BadgeNotOwned");
        }
        else if( idSgrp && state && (sg = stat_sgrpLockAndLoad(idSgrp)))
        {
            char *cmd;

            sgrpbadges_SetOwned(&sg->badgeStates, pdef->iIdx, 0);

            // -----
            // clear the updated field for flush to clients

            sg->badgeStates.eaiStates[pdef->iIdx] = 0;

            estrCreate(&cmd);

            estrPrintf(&cmd, "sgrpBadgeStates 1 \"%d,%d\"",pdef->iIdx, BADGE_CHANGED_MASK);
            stat_sendToAllSgrpMembers( idSgrp, cmd );

            estrDestroy(&cmd);
            
            // log it
            LOG( LOG_STATSERVER, LOG_LEVEL_VERBOSE, 0, __FUNCTION__ ":revoked badge '%s' idx '%d' to sgrp '%s:%d'", state->strs[0], pdef->iIdx, sg->name, idSgrp);
            
//             // let user know
//             estrConcatf(&s_esResponseBuf,"\"SgrpStatCmdRevokedBadge\" \"(%s,%s)\"", state->strs[0], sg->name );
            
            stat_sgrpUpdateUnlock(idSgrp,sg);
        }
        else
        {
            cmdOldFailed("SgrpStatCmdFailed_LockFailed");
        }
    }
}
    
static void s_StatShow(AppServerCmdState *state, int idSgrp, Supergroup *sg)
{
    if(sg && state)
    {
        char *pchStat = state->strs[0];
        StashTableIterator iter;
        StashElement elem;
        
        estrPrintf(&s_esResponseBuf, "svr_sgstat_statshow_response ");
        // start quoted response
//         estrConcatStaticCharArray(&s_esResponseBuf, " \"");
//         estrConcatf(&s_esResponseBuf, "%s (%d,%s)\n", sg->name, idSgrp, db_state.server_name);
        estrConcatf(&s_esResponseBuf, "\"%s\"", *pchStat ? pchStat : "*");

        // enquote the stats
        estrConcatStaticCharArray(&s_esResponseBuf, " \"");
        
        stashGetIterator(g_BadgeStatsSgroup.idxStatFromName, &iter);
        while(stashGetNextElement(&iter, &elem))
        {
            const char *pchName = stashElementGetStringKey(elem);
            int idx;

            if(*pchName!='*' // Skip internal names
               && (*pchStat=='\0' || strstriConst(pchName, pchStat)!=NULL)
               && stashFindInt(g_BadgeStatsSgroup.idxStatFromName, pchName, &idx))
            {
                int iVal = sgrp_BadgeStatGet( sg, pchName );
                estrConcatf(&s_esResponseBuf, "%d,%d;", iVal, idx);
            }
        }
        
        // finish quoted response
        estrConcatChar(&s_esResponseBuf, '\"');
    }
    else
    {
        cmdOldFailed("SgrpStatCmdFailed");
    }
}


static void s_StatSet(AppServerCmdState *state, int idSgrp, Supergroup *sg)
{
    char *pchStat = state ? state->strs[0] : "";
    int idx;
    
    if( verify( sg && sg->pBadgeStats && pchStat ) 
        && stashFindInt(g_BadgeStatsSgroup.idxStatFromName, pchStat, &idx))
    {
        if( AINRANGE( idx, sg->pBadgeStats->aiStats ))
        {
            sg->pBadgeStats->aiStats[idx] = state->ints[0];
        }
    }
    else
    {
        cmdOldFailed("no stat %s", pchStat);
    }
}

static void s_BaseEntryPermissionsReq(AppServerCmdState *state)
{
    if(verify( state ))
    {
        int idEntReq = state->ints[0];
        char *idsgs = state->strs[0];
        char *response = NULL;
        estrCreate(&response);

        estrPrintf(&response, "baseaccess_response_relay \"");

         for(;idsgs && idsgs != ((char*)1) && *idsgs; idsgs = strchr(idsgs,'\t')+1)
         {
            int idSgrp;
             if( verify(1==sscanf(idsgs,"%i",&idSgrp)) )
            {
                Supergroup *sg = stat_sgrpFromIdSgrp(idSgrp, true);
                 if( sg )
                {
                    BaseAccess accessState = sgrp_BaseAccessFromSgrp(sg, kSgrpBaseEntryPermission_Coalition);
                    
                     if( accessState == kBaseAccess_Allowed )
                    {
                        const BaseUpkeepPeriod *period = sgroup_GetUpkeepPeriod( sg );
                         if( period && period->denyBaseEntry )
                        {
                             accessState = kBaseAccess_RentOwed;
                        } 
                    } 

                    estrConcatf(&response, "%i,%i,%s\t", idSgrp, accessState, sg->name);
                }
                else
                {
                    LOG( LOG_STATSERVER, LOG_LEVEL_VERBOSE, 0, __FUNCTION__ ": load failed for sgrp '%d'", idSgrp);
                }
            }
        }

        estrConcatf(&response, "\" %i", idEntReq);
        
        stat_sendToEnt( idEntReq, response );
        
        estrDestroy(&response);
    }
}

static void s_SendSgrpInfo(int idEntSrc, Supergroup *sg, int idSgrp)
{
    if( sg )
    {
        int i;
        int n;
        const char *activeTaskFilename = NULL;
        char *resp = NULL;
        estrCreate(&resp);
        estrPrintf(&resp, "svr_sgstat_conprintf_response %i \"", idEntSrc);
        estrConcatf(&resp, "name: %s\n", sg->name);
        estrConcatf(&resp, "idSgrp:\t%i\n", idSgrp);
        estrConcatf(&resp, "leader title: %s\n", sg->leaderTitle );
        estrConcatf(&resp, "captain title: %s\n", sg->captainTitle );
        estrConcatf(&resp, "member title: %s\n", sg->memberTitle );
        estrConcatf(&resp, "msg: %s\n", escapeString(sg->msg));
        estrConcatf(&resp, "motto: %s\n", escapeString(sg->motto));
        estrConcatf(&resp, "description %s\n", escapeString(sg->description));
        estrConcatf(&resp, "emblem %s\n", sg->emblem);
        estrConcatStaticCharArray(&resp, "allies:\n");
        for(i = 0; sg->allies[i].db_id; ++i)
        {
            SupergroupAlly *a = &sg->allies[i];
            estrConcatf(&resp, "\t%d:%s chat display rank: %d(%s)\n", a->db_id, a->name, a->minDisplayRank, a->dontTalkTo?"blocked":"allowed");
        }
        estrConcatf(&resp, "min talk rank:\t%i\n", sg->alliance_minTalkRank);
        estrConcatf(&resp, "demote timeout:\t%i\n", sg->demoteTimeout);
        estrConcatf(&resp, "influence:\t%i\n", sg->influence);
        estrConcatf(&resp, "player type:\t%i\n", sg->playerType);
        estrConcatf(&resp, "num places for item of power:\t%i\n", sgrp_emptyMounts(sg));
        estrConcatf(&resp, "prestige:\t%i\n", sg->prestige);
        estrConcatf(&resp, "prestigeBase:\t%i\n", sg->prestigeBase);
        estrConcatf(&resp, "prestigeAddedUpkeep:\t%i\n", sg->prestigeAddedUpkeep);
        estrConcatf(&resp, "bonus count: \t%i\n", sg->prestigeBonusCount);
        estrConcatf(&resp, "flags: \t0x%x\n", sg->flags);
        estrConcatf(&resp, "rent due \t%i\n", sg->upkeep.prtgRentDue);
        estrConcatf(&resp, "rent incurred \t%s\n", timerMakeOffsetDateStringNoYearNoSecFromSecondsSince2000(sg->upkeep.secsRentDueDate));
        estrConcatf(&resp, "rent due \t%s\n", timerMakeOffsetDateStringNoYearNoSecFromSecondsSince2000(sg->upkeep.secsRentDueDate + g_baseUpkeep.periodRent ));
        estrConcatStaticCharArray(&resp, "base entry permissions:\n");
        
        for(i = 0; i < kSgrpBaseEntryPermission_Count; ++i)
        {
            if((1<<i) & sg->entryPermission)
                estrConcatf(&resp, "\t{%s}\n", sgrpbaseentrypermission_ToMenuMsg(i));
        }

        // reward tokens
        n = eaSize(&sg->rewardTokens);
        estrConcatf(&resp, "num tokens: \t%i\n", n);
        for( i = 0; i < n; ++i ) 
        {
            RewardToken *t = sg->rewardTokens[i];
            if( t )
                estrConcatf(&resp, "\t\t%s:%i\n", t->reward, t->val);
        }
        
        // special details
        n = eaSize(&sg->specialDetails);
        estrConcatf(&resp, "special details: \t%i\n", n);
        for( i = 0; i < n; ++i ) 
        {
            SpecialDetail *s = sg->specialDetails[i];
            if( s && s->pDetail)
            {
                const Detail *d = s->pDetail;
                char *time = timerMakeOffsetDateStringNoYearNoSecFromSecondsSince2000(s->creation_time);
                estrConcatf(&resp, "\t\t'%s'\tcreated:%s\tflags:%x\n", d ? d->pchName : "(unknown)",time, s->iFlags );
            }
        }
            
        // recipes
        n = eaSize(&sg->invRecipes);
        estrConcatf(&resp, "recipes in inventory: \t%i\n", n);
        for( i = 0; i < n; ++i ) 
        {
            RecipeInvItem *r = sg->invRecipes[i];
            if(r)
            {
                DetailRecipe const *d = r->recipe;
                estrConcatf(&resp, "\t\t'%s'\t%d\n", d?d->pchName:"(unknown)", r->count);
            }
        }
        
        activeTaskFilename = sg->activetask? sgTaskFileName(sg->activetask->sahandle.context) : NULL;
        estrConcatf(&resp, "active task: \t%s\n", activeTaskFilename ? activeTaskFilename :"(none)");

        estrConcatChar(&resp, '\"');

        stat_sendToEnt( idEntSrc, resp );
        estrDestroy(&resp); 
    }
}
void lpactstats_removeInactive(Packet *pak)
{
    int size = pktGetBitsAuto(pak);
    int i;
    for(i = 0; i < size; i++)
    {
        int dbid = pktGetBitsAuto(pak);
        stat_LevelingPactRemoveInactive(dbid);
    }
}

void sgrpstatserver_ReceiveCmd(Packet *pak)
{
    static CmdList server_cmdlist =
        {
            {{ client_sgstat_cmds },
            { 0 }}
        };
    CmdContext    output = {0};
    Cmd            *cmd = NULL;
    AppServerCmdState *state = &g_appserver_cmd_state;
    Supergroup *sg;

    int            idEntSrc = pktGetBitsPack(pak, 1);
    int            idSgrpDefault     = pktGetBitsPack(pak,1); // default sgrp id if not provided
    char        *str = NULL;
    int idSgrp = state->idSgrp > 0 ? state->idSgrp : idSgrpDefault;

    strdup_alloca(str, pktGetString(pak));
    s_idSgrp = idSgrp;
    sg = stat_sgrpFromIdSgrp(idSgrp, true);

    // ----------
    // read the command 

    output.access_level = ACCESS_INTERNAL;
    LOG( LOG_STATSERVER, LOG_LEVEL_VERBOSE, 0, "CommandIn: %s", str);
    cmd = cmdOldRead(&server_cmdlist,str,&output);
    if( cmd )
    {
        // ----------
        // clear the response buf
        
        if( s_esResponseBuf )
        {
            s_esResponseBuf[0] = 0;
        }

        if( INRANGE( cmd->num, CHATCMD_TOTAL_COMMANDS, SGRPSTATCMD_TOTAL_COMMANDS))
        {
            

            switch(cmd->num)
            {
            xcase SGRPSTATCMD_STATADJ:
            {
                char *statName = state->strs[0];
                int adj = state->ints[0];

                sgrpstats_StatAdj( idSgrp, statName, adj );
                // start the default response
                estrPrintf(&s_esResponseBuf,"svr_sgstat_conprintf_response %i \"", idEntSrc);
                estrPrintf( &s_esResponseBuf, "svr_sgstat_statadj \"adjusted stat '%s'.\"",statName );
            }
            xcase SGRPSTATCMD_SGRPUPDATE:
            {
                char *res = statserver_UpdateBaseUpkeep(idSgrp);
                // start the default response
                estrPrintf(&s_esResponseBuf,"svr_sgstat_conprintf_response %i \"", idEntSrc);
                estrConcatf(&s_esResponseBuf,"%s\"", res);
            }
            xcase SGRPSTATCMD_GIVEBADGE:
            {
                Supergroup *sg = NULL;
                const BadgeDef *pdef = stashFindPointerReturnPointerConst( g_SgroupBadges.hashByName, state->strs[0] );
                // start the default response
                estrPrintf(&s_esResponseBuf,"svr_sgstat_conprintf_response %i \"", idEntSrc);

                if( !pdef )
                {
                    estrConcatf(&s_esResponseBuf, "couldn't find supergroup badge '%s' to reward", state->strs[0]);
                }
                else if(verify( (sg = stat_sgrpLockAndLoad(idSgrp)) )) // okay to do this if already set
                {
                    // give ownership
                    sgrpbadges_SetOwned(&sg->badgeStates, pdef->iIdx, 1);

                    // clear the updated field for flush to clients
                    sg->badgeStates.eaiStates[pdef->iIdx] = 0;
                    
                    // log it
                    LOG( LOG_STATSERVER, LOG_LEVEL_VERBOSE, 0, __FUNCTION__ ": awarded badge '%s' idx '%d' to sgrp '%s:%d'", state->strs[0], pdef->iIdx, sg->name, idSgrp);

                    // let user know
                    estrConcatf(&s_esResponseBuf,"\"SgrpStatCmdGaveBadge\" \"'%s' to '%s'\"", state->strs[0], sg->name );
                    // finally, unlock
                    stat_sgrpUpdateUnlock(idSgrp, sg);
                }
                else
                {
                    LOG( LOG_STATSERVER, LOG_LEVEL_VERBOSE, 0, __FUNCTION__ ": verify failed: couldn't lock supergroup %d", idSgrp);
                    estrConcatf(&s_esResponseBuf, "couldn't lock supergroup '%d'", idSgrp);
                }
            }
            xcase SGRPSTATCMD_PASSTHRU:
                statDebugHandleString( state->strs[0], s_responsePrintf );
            xcase SGRPSTATCMD_REVOKEBADGE:
                s_RevokeBadge(state,idSgrp,sg);
            xcase SGRPSTATCMD_STATSHOW:
                s_StatShow(state,idSgrp,sg);
            xcase SGRPSTATCMD_STATSET:
                s_StatSet(state,idSgrp,sg);
            xcase SGRPSTATCMD_BASEENTRYPERMISSIONS_REQ:
                s_BaseEntryPermissionsReq(state);
            xcase SGRPSTATCMD_SGRP_INFO:
            {
                Supergroup *sg = stat_sgrpFromIdSgrp( idSgrp, true );
                s_SendSgrpInfo(idEntSrc, sg, idSgrp);
            }
            xcase SGRPSTATCMD_SGRP_INFO_NAME:
            {
                char *nameSg = state->strs[0];
                int idSgrp = 0;
                Supergroup *sg = stat_sgrpFromName( nameSg, &idSgrp, true );
                s_SendSgrpInfo(idEntSrc, sg, idSgrp);
            }
            xcase SGRPSTATCMD_SGRP_INFO_ID:
            {
                Supergroup *sg = stat_sgrpFromIdSgrp( state->idSgrp, true );
                s_SendSgrpInfo(idEntSrc, sg, state->idSgrp);
            }
            xcase SGRPSTATCMD_LEVELINGPACT_JOIN:
                stat_LevelingPactJoin(state->ints[0], state->ints[1], state->ints[2], state->ints[3], state->strs[0], state->ints[4]);
            xcase SGRPSTATCMD_LEVELINGPACT_ADDXP:
                stat_LevelingPactAddXP(state->ints[0], state->ints[1], state->ints[2]);
            xcase SGRPSTATCMD_LEVELINGPACT_ADDINFLUENCE:
                stat_LevelingPactAddInf(state->ints[0], state->ints[1], state->ints[2]);
            xcase SGRPSTATCMD_LEVELINGPACT_GETINFLUENCE:
                stat_LevelingPactGetInf(state->ints[0]);
            xcase SGRPSTATCMD_LEVELINGPACT_UPDATEVERSION:
                stat_levelingPactUpdateVersion(state->ints[0], state->ints[1]);
            xcase STATCMD_LEAGUE_JOIN:
                stat_LeagueJoin(state->ints[0], state->ints[1], state->ints[2], state->strs[0], state->ints[3], state->ints[4]);
            xcase STATCMD_LEAGUE_JOIN_TURNSTILE:
                stat_LeagueJoinTurnstile(state->ints[0], state->ints[1], state->strs[0], state->ints[2], state->ints[3]);
            xcase STATCMD_LEAGUE_PROMOTE:
                stat_LeaguePromote(state->ints[0], state->ints[1]);
            xcase STATCMD_LEAGUE_CHANGE_TEAM_LEADER_TEAM:
                stat_LeagueChangeTeamLeaderTeam(state->ints[0], state->ints[1], state->ints[2]);
            xcase STATCMD_LEAGUE_CHANGE_TEAM_LEADER_SOLO:
                stat_LeagueChangeTeamLeaderSolo(state->ints[0], state->ints[1], state->ints[2]);
            xcase STATCMD_LEAGUE_UPDATE_TEAM_LOCK:
                stat_LeagueUpdateTeamLock(state->ints[0], state->ints[1], state->ints[2]);
            xcase STATCMD_LEAGUE_QUIT:
                stat_LeagueQuit(state->ints[0], state->ints[1], state->ints[2]);
            xdefault:
                // start the default response
                estrPrintf(&s_esResponseBuf,"svr_sgstat_conprintf_response %i \"", idEntSrc);
                estrConcatf(&s_esResponseBuf,"\"unhandled command %s\"", cmd->name);
            }
        }

        if( s_esResponseBuf && s_esResponseBuf[0] && idEntSrc > 0 )
        {            
            s_sgrpstatserver_ResponseRelay(idEntSrc, s_esResponseBuf);
        }
        
        estrDestroy(&s_esResponseBuf);
    }
    else
    {
        estrPrintf(&s_esResponseBuf,"svr_sgstat_conprintf_response %i \"unknown cmd %s\"", idEntSrc, str);
        s_sgrpstatserver_ResponseRelay(idEntSrc, s_esResponseBuf);
    }

    ZeroStruct(state);
}
