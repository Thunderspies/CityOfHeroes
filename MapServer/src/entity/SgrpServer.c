#include "SgrpServer.h"
#include "entity/Sgrpbasepermissions.h"
#include "bases/baseupkeep.h"

#include <utilitieslib/utils/eval.h>
#include "gameSys/arenamapserver.h"
#include "entity/character_base.h"
#include "entity/character_eval.h"
#include "cmdparse/cmdserver.h"
#include "cmdparse/cmdservercsr.h"
#include "comm_backend.h"
#include "comm_game.h"
#include "container/containerbroadcast.h"
#include "container/containerSupergroup.h"
#include "entity/costume.h"
#include "gameData/costume_data.h"
#include "dbcomm/dbcomm.h"
#include "dbcomm/dbcontainer.h"
#include "dbcomm/dbnamecache.h"
#include "dbcomm/dbquery.h"
#include <utilitieslib/components/earray.h>
#include "entity/entity.h"
#include "entity/entPlayer.h"
#include "language/langServerUtil.h"
#include <utilitieslib/utils/memorymonitor.h>
#include <utilitieslib/language/MultiMessageStore.h>
#include "gameComm/sendToClient.h"
#include "dbcomm/staticMapInfo.h"
#include "entity/Supergroup.h"
#include "svr/svr_base.h"
#include "gameComm/svr_chat.h"
#include "svr/svr_player.h"
#include "container/team.h"
#include "entity/teamup.h"
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/utils/utils.h>
#include "filter/validate_name.h"
#include <utilitieslib/components/estring.h>
#include "bases/bases.h"
#include "bases/baseparse.h"
#include "bases/basedata.h"
#include "bases/basesystems.h"
#include "entity/powers.h"
#include "bases/DetailRecipe.h"
#include "auth/authUserData.h"
#include "entity/character_inventory.h"
#include <utilitieslib/utils/mathutil.h>
#include "storyarc/storysend.h"
#include "storyarc/contactdef.h"
#include "storyarc/storyarcprivate.h"
#include "gameSys/raidmapserver.h"
#include "container/baseloadsave.h"
#include <utilitieslib/utils/memlog.h>
#include "bases/baseserver.h"
#include "beacon/beaconClientServerPrivate.h"
#include "filter/profanity.h"
#include <utilitieslib/language/AppLocale.h>
#include "bases/baselegal.h"

#include "entity/classes.h"
#include "entity/origins.h"

#include "TeamReward.h"
#include "dbcomm/logcomm.h"

// defined in raidmapserver.c 
extern SpecialDetail *ItemOfPowerFind(Supergroup *pSG, const char *pName);

#define    RAID_POINT_TOKEN        "RaidPoints"
#define    RAID_POINT_COP_TOKEN    "RaidPointsCoP"
#define    RAID_POINT_RAID_TOKEN    "RaidPointsRaid"
#define    RAID_POINT_MAX_COP        1260
#define RAID_POINT_MAX_RAID        50000
#define RAID_POINT_MAX_TIME        (60*60*24*7)

//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------
//SUPERGROUPS - Now a new file //////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------

static bool s_genLockSgId = 0;
        // set by the sgroup_setgeneric function when it locks a
        // supergroup

static char* rankNames[NUM_SG_RANKS] =
{ 
    "defaultMemberTitle",
    "defaultLieutenantTitle",
    "defaultCaptainTitle",
    "defaultCommanderTitle",
    "defaultLeaderTitle",
    "defaultSuperLeaderTitle",
    "GM_Hidden"
};

void sgroup_clearStats( Entity* e )
{
    if( e && e->sgStats )
    {
        eaDestroyEx( &e->sgStats, destroySupergroupStats );
        //done in EArrayDestory: free( sg->stats );
        //done in EArrayDestory: sg->stats = 0;
    }
}

SupergroupStats* sgroup_memberStats(Entity* e, U32 dbid)
{
    int i, count;
    if (e && e->sgStats)
    {
        count = eaSize(&e->sgStats);
        for (i = 0; i < count; i++)
        {
            if (e->sgStats[i]->db_id == dbid)
                return e->sgStats[i];
        }
    }
    return NULL;
}

int sgroup_NameExists( char * name )
{
    int map_num;
    int id = dbSyncContainerFindByElement(CONTAINER_SUPERGROUPS,"Name",name,&map_num,0);

    if ( id > 0 )
        return TRUE;
    else
        return FALSE;
}

// handled by: receiveSuperStats
void sgroup_sendList(Packet *pak, Entity *e, int full_update)
{
    int count, i;
    Supergroup *sg = e->supergroup;

    // member count bits
    if (!e->supergroup || 0==eaSize(&e->sgStats)) {
        pktSendBitsPack( pak, 1, 0 );
        return;
    }

    count = eaSize(&e->sgStats);
    assert(count > 0);
    pktSendBitsPack( pak, 1, count );

    // update bit
    pktSendBitsPack( pak, 1, sg->influence );
    pktSendBitsPack( pak, 1, sg->prestige );
    pktSendBitsPack( pak, 1, sg->prestigeBase );
    pktSendBitsPack( pak, 1, sg->prestigeAddedUpkeep );
    pktSendBitsPack( pak, 1, sg->upkeep.prtgRentDue );
    pktSendBitsPack( pak, 1, sg->upkeep.secsRentDueDate );
    pktSendBitsPack( pak, 1, sg->playerType );

    if (!e->supergroup_update && !full_update)
    {
        pktSendBits( pak, 1, 0 );
        return;
    }
    pktSendBits( pak, 1, 1 );

    for( i = 0; i < count; i++ )
    {
        SupergroupStats * stat = e->sgStats[i];

        pktSendString( pak, stat->name );
        pktSendBitsPack( pak, 1, stat->db_id );
        pktSendBitsPack( pak, 1, stat->rank );
        pktSendBitsPack( pak, 1, stat->level );
        pktSendBits( pak, 32, stat->last_online );
        pktSendBits( pak,  1, stat->online );
        pktSendString(pak, stat->arch );
        pktSendString(pak, stat->origin );
        pktSendBits( pak, 32, stat->member_since );
        pktSendBits( pak, 32, stat->prestige );
        pktSendBitsPack( pak, 1, stat->playerType );

        if ( stat->online )
        {
            // state->zone should be updated with the translated map name
            // every time stat->map_id is changed.
            pktSendString( pak, stat->zone );
        }
    }

    pktSendString( pak, sg->name );
    pktSendString( pak, sg->motto );
    pktSendString( pak, sg->msg );
    pktSendString( pak, sg->emblem );
    pktSendString( pak, sg->description );
    pktSendBitsPack( pak, 1, sg->colors[0] );
    pktSendBitsPack( pak, 1, sg->colors[1] );
    pktSendBitsPack( pak, 1, sg->tithe );
    pktSendBitsPack( pak, 1, sg->spacesForItemsOfPower );
    pktSendBits( pak, 32, sg->demoteTimeout );

    for (i = 0; i < NUM_SG_RANKS; i++)
    {
        pktSendString(pak, sg->rankList[i].name);
        pktSendBitsArray( pak, sizeof(sg->rankList[i].permissionInt) * 8, sg->rankList[i].permissionInt );
    }

    //entry permissions
    pktSendBitsAuto(pak, sg->entryPermission);

    count = eaSize(&sg->memberranks);
    pktSendBitsPack( pak, 1, count);
    for (i = 0; i < count; i++)
    {
        pktSendBitsPack(pak, PKT_BITS_TO_REP_DB_ID, sg->memberranks[i]->dbid);
        pktSendBitsPack(pak, 1, sg->memberranks[i]->rank);
    }

    for (i = 0; i < MAX_SUPERGROUP_ALLIES; i++)
    {
        pktSendBits(pak, 32, sg->allies[i].db_id);
        if (e->supergroup->allies[i].db_id)
        {
            pktSendBits(pak,1,sg->allies[i].dontTalkTo);
            pktSendBitsAuto(pak,sg->allies[i].minDisplayRank);
            pktSendString(pak,sg->allies[i].name);
        }
    }

    pktSendBitsAuto(pak,e->supergroup->alliance_minTalkRank);
}

typedef struct
{
    SupergroupStats    **stats;
    int                sg_id;
    U32                last_update;
} SgStatEntry;

static SgStatEntry    **sgStatCache;
#define        SGSTAT_REFRESH_SECONDS    (60*15)

void *createSupergroupStatsWrapper(int fake_size)
{
    return createSupergroupStats();
}

SupergroupStats ***findOrReplaceSgStat(int sg_id,SupergroupStats ***replace)
{
    int                i;
    SgStatEntry        *entry=0;

    for(i=eaSize(&sgStatCache)-1;i>=0;i--)
    {
        if (sgStatCache[i]->last_update + SGSTAT_REFRESH_SECONDS < timerSecondsSince2000())
        {
            eaDestroyEx( &sgStatCache[i]->stats, destroySupergroupStats );
            eaRemove(&sgStatCache,i);
        }
        else if (sgStatCache[i]->sg_id == sg_id)
            entry = sgStatCache[i];
    }
    if (replace)
    {
        if (entry)
        {
            eaDestroyEx(&entry->stats, destroySupergroupStats );
            eaCopyEx(replace,&entry->stats,sizeof(SupergroupStats),createSupergroupStatsWrapper);
        }
        else
        {
            entry = calloc(sizeof(SgStatEntry),1);
            eaPush(&sgStatCache,entry);
            eaCopyEx(replace,&entry->stats,sizeof(SupergroupStats),createSupergroupStatsWrapper);
        }
        entry->sg_id = sg_id;
        entry->last_update = timerSecondsSince2000();
    }
    if (entry)
        return &entry->stats;
    return 0;
}

static void processSgStats(Entity *e)
{
    int        i;
    int        leaderIdx = 0;

    sgroup_UpdateBonusPrestige(e, s_genLockSgId ? true : false, 0);

    checkSgrpMembers(e, "processSgStats");

    e->supergroup_update = 1;
    // New-format SGs can legitimately remove all permissions for 'leader'
    // rank (yellow star) so it's not a valid test on its own. If we've set
    // up a memberranks array the SG must have been converted already.
    if(!eaSize(&e->supergroup->memberranks) && !e->supergroup->rankList[NUM_SG_RANKS - 3].permissionInt[0])
    {
        // convert from old (sgstats based) ranks
        memlog_printf(&g_teamlog, "converting ranks: %i", e->db_id);
        sgroup_setGeneric(e, SG_PERM_ANYONE, sgroup_genConvertRanks, NULL, NULL, "ConvertRanks");
    }

    for( i = 0; i < SG_PERM_COUNT; i++)
    {
        // the leader has to have all permissions (this will most likely never get hit until a new int's
        // worth of permissions get added)
        // Changed from - 1 to - 2 since both the superleader and GM's get all permissions
        if(!TSTB(e->supergroup->rankList[NUM_SG_RANKS - 2].permissionInt, i))
        {
            memlog_printf(&g_teamlog, "calling genFixLeaderPermissions: %i", e->db_id);
            sgroup_setGeneric(e, SG_PERM_ANYONE, sgroup_genFixLeaderPermissions, NULL, NULL, "FixPermissions");
            break;
        }
    }

    for (i = 0; i < NUM_SG_RANKS; i++)
    {
        // Do a little cleanup for SG's that are being handled for the first time after adding the new ranks
        if (e->supergroup->rankList[i].name[0] == 0)
        {
            memlog_printf(&g_teamlog, "calling genFixRankNames: %i", e->db_id);
            sgroup_setGeneric(e, SG_PERM_ANYONE, sgroup_genFixRankNames, NULL, NULL, "FixRankNames");
            break;
        }
    }

    if (e->pl && !(e->supergroup->flags & SG_FLAGS_LOCKED_PLAYERTYPE))
    {
        // Lock down the SG's playerType.  Set this the first time we reach here with the flag not set,
        // then set the flag casting it in stone.  CS will get a command to override this in those rare
        // cases where things go hideously wrong.
        memlog_printf(&g_teamlog, "calling genLockSGPlayerType: %i", e->db_id);
        sgroup_setGeneric(e, SG_PERM_ANYONE, sgroup_genLockSGPlayerType, NULL, NULL, "LockSGPlayerType");
    }
}

void sgroup_UpdateBonusPrestige(Entity *e, bool bLocked, int iExtraDelta)
{
    int iMemberCount;
    int iPraetCount;
    int iDelta = 0;
    int bFound = 0;
    int bLeft = 0;
    int i;

    if(!e->supergroup)
        return;

    for (i = 0; i < e->supergroup->members.count; ++i)
    {
        if (!EAINRANGE(i, e->supergroup->memberranks))
        {
            // Add a rank for this player
            // copy/pasting the method that was here:
            //    void sgroup_genConvertRanks(Entity* e, void* notused, void* notused2)
            SupergroupMemberInfo *newrank = createSupergroupMemberInfo();
            int rank = 2;
            if (EAINRANGE(i, e->sgStats))
            {
                if(e->sgStats[i]->rank <= 0)
                    rank = 0;
                else if(e->sgStats[i]->rank >= 2)
                    rank = NUM_SG_RANKS_VISIBLE - 2;    // the old check for ranks was "rank > 2 == leader" so let's stick with that i guess
                                                        // set this guy to a leader, but not a superleader, so we don't wind up with two superleaders.
                                                        // The autopromote code will take care of assigning a single superleader
            }

            newrank->dbid = e->supergroup->members.ids[i];
            newrank->rank = rank;
            eaPush(&e->supergroup->memberranks,newrank);
            LOG_SG( e->supergroup->name, 0, LOG_LEVEL_VERBOSE, 0, "Player was missing rank entry, added");
        }

        if (e->supergroup->memberranks[i]->rank == (NUM_SG_RANKS-1))
        {
            iExtraDelta--;
        }
    }

    iMemberCount = MIN(e->supergroup->members.count+iExtraDelta, SG_PRESTIGE_BONUS_MEMBER_COUNT);

    if(e->supergroup->prestigeBonusCount != iMemberCount)
    {
        if(bLocked || teamLock(e, CONTAINER_SUPERGROUPS))
        {
            iMemberCount = MIN(e->supergroup->members.count+iExtraDelta, SG_PRESTIGE_BONUS_MEMBER_COUNT);
            if(e->supergroup->prestigeBonusCount != iMemberCount)
            {
                int iDelta = iMemberCount - e->supergroup->prestigeBonusCount;
                int iPrestige = SG_PRESTIGE_BONUS_PER_MEMBER*iDelta;

                e->supergroup->prestigeBonusCount += iDelta;
                e->supergroup->prestige += iPrestige;
                e->supergroup->flags |= SG_FLAGS_BONUS_PRESTIGE_APPLIED|SG_FLAGS_BONUS_COUNT_APPLIED;

                LOG_SG_ENT( e, LOG_LEVEL_IMPORTANT, 0, "Prestige:BonusUpdate Updated prestige by %d for a %d member change.", iPrestige, iDelta);

                sendEntsMsg(CONTAINER_SUPERGROUPS, e->supergroup_id, INFO_SVR_COM, 0,
                            "%s%s",
                            DBMSG_CHAT_MSG,
                            localizedPrintf(e,
                                            iPrestige>0 ? "SgroupGainedPrestige" : "SgroupLostPrestige",
                                            e->supergroup->members.count+iExtraDelta,
                                            SG_PRESTIGE_BONUS_PER_MEMBER,
                                            SG_PRESTIGE_BONUS_MEMBER_COUNT));
            }

            if(!bLocked && !teamUpdateUnlock( e, CONTAINER_SUPERGROUPS))
                printf( "Failed to unlock teamup container, BIG TROUBLE, TELL A PARAGON PROGRAMMER!!");
        }
    }

    // We only need to see one change to know we need to lock and re-check
    // everybody.
    for(iPraetCount = 0; !bLeft && iPraetCount < MAX_SUPER_GROUP_MEMBERS; iPraetCount++)
    {
        if(e->supergroup->praetBonusIds[iPraetCount])
        {
            for(iMemberCount = 0; !bFound && iMemberCount < e->supergroup->members.count; iMemberCount++)
            {
                if(e->supergroup->members.ids[iMemberCount] == e->supergroup->praetBonusIds[iPraetCount])
                    bFound = 1;
            }

            if(!bFound)
                bLeft = 1;
        }
    }

    if(bLeft && (bLocked || teamLock(e, CONTAINER_SUPERGROUPS)))
    {
        // When ex-Praetorians who granted the bonus cannot be found in our
        // member list (whether they left, or were deleted, or whatever) we
        // need to rescind that bonus.
        for(iPraetCount = 0; iPraetCount < MAX_SUPER_GROUP_MEMBERS; iPraetCount++)
        {
            if(e->supergroup->praetBonusIds[iPraetCount])
            {
                bFound = 0;

                for(iMemberCount = 0; !bFound && iMemberCount < e->supergroup->members.count; iMemberCount++)
                {
                    if(e->supergroup->members.ids[iMemberCount] == e->supergroup->praetBonusIds[iPraetCount])
                    {
                        bFound = 1;
                    }
                }

                if(!bFound)
                {
                    e->supergroup->praetBonusIds[iPraetCount] = 0;
                    // I19 HACK:  Don't revoke prestige anymore!
                    //e->supergroup->prestige -= PRAET_SG_JOIN_BONUS;
                    //e->supergroup->prestige = MINMAX(e->supergroup->prestige, -MAX_SUPERGROUP_PRESTIGE, MAX_SUPERGROUP_PRESTIGE);
                }
            }
        }

        if(!bLocked && !teamUpdateUnlock(e, CONTAINER_SUPERGROUPS))
            printf("Failed to unlock supergroup container!");
    }
}

void sgroup_setIsDark(Entity *e)
{
    int i;

    // Null pointer checks.  The first and last of these should rarely, if ever, happen, but the middle of them will punt us out of the player is not in a SG
    if (e == NULL || e->supergroup == NULL || e->pl == NULL)
    {
        return;
    }

    if (e->pl->playerType != e->supergroup->playerType && e->pl->supergroup_mode)
    {
        // If I'm in a supergroup, I just went dark, and I'm in SG mode, force SG mode off
        sgroup_SetMode(e, 0, 0);
    }

    for (i = eaSize(&e->supergroup->memberranks) - 1; i >= 0; i--)
    {
        if (e->supergroup->memberranks[i]->dbid == e->db_id)
        {
            // If they're dark, and are the superleader, demote them.  The autopromote code will handle filling the void.
            // This will be a silent autodemote, if confirmation is needed it should be handled elsewhere.  i.e. *LONG*
            // before we get here.
            if (e->pl->playerType != e->supergroup->playerType && e->supergroup->memberranks[i]->rank == NUM_SG_RANKS - 2)
            {
                // Pass in i (index into memberranks) so we don't have to do this search a second time
                sgroup_setGeneric(e, SG_PERM_ANYONE, sgroup_genSetIsDark, &i, &i, "SetIsDark");
            }
            break;
        }
    }
}

void sgroup_readDBData(Packet *in_pak,int db_id,int count)
{
    int i, j;
    Entity *e = entFromDbIdEvenSleeping( db_id );
    Entity *member;
    bool leaderFound = false;
    int curTime = dbSecondsSince2000();

    if( !e )
        return;

    if( !e->supergroup )
        return;

    sgroup_clearStats( e );

    if( !count )
    {
        return;
    }
    memlog_printf(&g_teamlog, "sgroup stats recv: %i", e->db_id);

    eaCreate( &e->sgStats );

    for( i = 0; i < count; i++ )
    {
        SupergroupStats * stat = createSupergroupStats();
        OnlinePlayerInfo *opi;
        int supergroup_id;

        strncpyt( stat->name, pktGetString(in_pak), sizeof(stat->name));
        stat->db_id                = pktGetBitsPack(in_pak,1);
        stat->rank                = pktGetBitsPack(in_pak,1);
        stat->level                = pktGetBitsPack(in_pak,1);
        stat->last_online        = pktGetBits(in_pak,32);            // last active
        strncpyt(stat->arch,    pktGetString(in_pak), sizeof(stat->arch));
        strncpyt(stat->origin,    pktGetString(in_pak), sizeof(stat->origin));
        stat->member_since        = pktGetBits(in_pak,32);
        stat->prestige            = pktGetBitsPack(in_pak,1);
        stat->playerType        = pktGetBitsPack(in_pak,1);

        supergroup_id = pktGetBitsPack(in_pak,1);
        if(supergroup_id != e->supergroup_id)
        {
            dbLog("SuperGroup:Stats",e,"Supergroup stats for invalid member (have %i, got %i), re-requesting",e->supergroup_id,supergroup_id);
            sgroup_clearStats(e);
            sgroup_getStats(e);
            return;
        }

        // check if player is online
        member = entFromDbId(stat->db_id);
        opi = dbGetOnlinePlayerInfo(stat->db_id);

        if( member && (!(member->pl->hidden&(1<<HIDE_SG)) || e->access_level > 0 ) )
            stat->online = 1;
        else
        {
            if( opi && (!(opi->hidefield&(1<<HIDE_SG)) || e->access_level > 0) )
                stat->online = 1;
            else
                stat->online = 0;
        }

        if( opi )
        {
            strncpyt( stat->zone, getTranslatedMapName(opi->map_id), 256 );
            stat->map_id = opi->map_id;
            stat->static_info = staticMapInfoFind(stat->map_id);
            stat->level = opi->level;
        }
        eaPush( &e->sgStats, stat );

    }

    // Per CW, e->supergroup->memberranks is authoritative.  Therefore we copy from memberranks
    // to sgStats since these were extracted from the Rank field in the Ents table, which may be
    // incorrect in the event of an offline promotion or demotion.
    for(i = eaSize(&e->supergroup->memberranks) - 1; i >= 0; i--)
    {
        for(j = eaSize(&e->sgStats) - 1; j >= 0; j--)
        {
            if(e->sgStats[j]->db_id == e->supergroup->memberranks[i]->dbid)
            {
                e->sgStats[j]->rank = e->supergroup->memberranks[i]->rank;
                break;
            }
        }
    }

    for (i = eaSize(&e->supergroup->memberranks) - 1; i >= 0; i--)
    {
        SupergroupMemberInfo* curMember = e->supergroup->memberranks[i];
        // Disable autopromote to hidden rank for access level 9 and above (i.e. dev)
        // My thinking here is that the hidden rank is mainly intended for CS.
        if (e->access_level && e->access_level < 9 && curMember->dbid == e->db_id)
        {
            for(j = eaSize(&e->sgStats)-1; j >= 0; j--)
            {
                if(e->sgStats[j]->db_id == curMember->dbid)
                {
                    int GMRank = NUM_SG_RANKS - 1;                    
                    int dbid = curMember->dbid; // the curMember gets blown away because of the supergroup locking
                    sgroup_setGeneric(e, SG_PERM_ANYONE, sgroup_genSetRank, &dbid, &GMRank, "SetGMRank");
                    break;
                }
            }
        }

        // If the rank is totalled, i.e. outside the legal range, set back to zero.  They can always be promoted again later
        if (curMember->rank < 0 || curMember->rank >= NUM_SG_RANKS)
        {
            int demotedRank = 0;
            int dbid = curMember->dbid; // the curMember gets blown away because of the supergroup locking

            // Set local copies in memory to zero to match.  I don't want to risk this guy interfering with a leader
            // auto promotion.  The sgroup_genSetRank() will eventually cause us to come through here again, but
            // the damage would have been done by that time.
            curMember->rank = 0;
            for (j = eaSize(&e->sgStats) - 1; j >= 0; j--)
            {
                if(e->sgStats[j]->db_id == curMember->dbid)
                {
                    e->sgStats[j]->rank = 0;
                    break;
                }
            }

            sgroup_setGeneric(e, SG_PERM_ANYONE, sgroup_genSetRank, &dbid, &demotedRank, "SetRank");
        }
        else if (curMember->rank == NUM_SG_RANKS_VISIBLE - 1)
        {
            for(j = eaSize(&e->sgStats)-1; j >= 0; j--)
            {
                if(e->sgStats[j]->db_id == curMember->dbid)
                {
                    if(!e->sgStats[j]->online && curTime - e->sgStats[j]->last_online > e->supergroup->demoteTimeout && curMember->dbid != e->db_id)
                    {
                        int demotedRank = NUM_SG_RANKS_VISIBLE - 2;
                        int dbid = curMember->dbid; // the curMember gets blown away because of the supergroup locking
                        sgroup_setGeneric(e, SG_PERM_ANYONE, sgroup_genSetRank, &dbid, &demotedRank, "SetRank");
                    }
                    else
                    {
                        leaderFound = true;
                    }
                    break;
                }
            }
        }
    }

    // find a new leader if needed
    if(!leaderFound)
    {
        int best_time = 0;                    // Last time on of best candidate
        int cand_time = 0;                    // Last time on of current candidate
        int best_rank = -1;                    // Rank of best candidate
        int best_id = 0;                    // db_id of best candidate
        int best_joindate = 0x7fffffff;        // join date of best candidate, expressed as seconds since 2000.  Init this to max signed int, since lower is better
        int leaderRank = NUM_SG_RANKS_VISIBLE - 1;
        int candidate_indices[4096];        // This should be OK, considering that we can't have more than 150 in a SG
        int num_candidate = 0;
        int dark_yellow_stars = 0;            // Count of players that are dark yellow star leaders
        int total_yellow_stars = 0;            // Count of players that are "yellow star" leaders

        LOG_SG_ENT( e, LOG_LEVEL_IMPORTANT, 0, "Rank Autopromotion code activated");

        for(i = eaSize(&e->sgStats) - 1; i >= 0; i--)
        {
            SupergroupStats *stat = e->sgStats[i];

            if (stat->playerType != e->supergroup->playerType)
            {
                if (stat->rank == NUM_SG_RANKS - 3)
                {
                    // This player is both dark, and a yellow star leader, count that fact
                    dark_yellow_stars++;
                }
            }

            if (stat->rank == NUM_SG_RANKS - 3)
            {
                // This player is a yellow star leader, count that fact
                total_yellow_stars++;
            }
            // DGNOTE 8/5/2010
            // I've just counted the total yellow stars and the number of dark yellow stars.  However, I'm pretty certain that these two are not enough
            // on their own to determine if we should avoid promoting a "sub yellow star" player to red star because all yellows are dark.  The problem
            // arises with a yellow star that's not dark, but is inactive.  What I really need to do is limit this check to active players.  I need to
            // think about this some more, so I'm going to leave the count code in for now, but not actually do anything with it yet.

            // Precheck: this better be a real person, we also reject GM's at the hidden ber GM rank
            // and the player must have logged on within the demote timeout period
            // 1/14/2010 Also reject dark players
            if (stat->db_id && stat->rank != NUM_SG_RANKS - 1 && 
                (stat->online || 
                curTime - stat->last_online < e->supergroup->demoteTimeout || 
                stat->db_id == e->db_id) &&
                stat->playerType == e->supergroup->playerType)
            {
                if (stat->online || stat->db_id == e->db_id)
                {
                    // Candidate last online time is now if they're online (duh :) )
                    cand_time = curTime;
                }
                else
                {
                    // else it's whenever they were last on
                    cand_time = stat->last_online;
                }

                // If this person is outright higher rank, reset the window, ditch previous members, and start again
                if (stat->rank > best_rank)
                {
                    // stats may be outdated, make sure person is still in sg
                    for(j = 0; j < eaSize(&e->supergroup->memberranks); j++)
                    {
                        if(e->supergroup->memberranks[j]->dbid == stat->db_id)
                        {
                            LOG_SG_ENT( e, LOG_LEVEL_IMPORTANT, 0, "Rank New best rank: %d adding db_id %d (%s) to the candidate list, slot 0, setting window time to %d", stat->rank, stat->db_id, stat->name, cand_time);
                            // All looks good, save the new best candidate
                            best_time = cand_time;
                            best_rank = stat->rank;
                            //best_id = stat->db_id;
                            // reset the window
                            candidate_indices[0] = i;
                            num_candidate = 1;
                            break;
                        }
                    }
                }
                else if (stat->rank == best_rank)
                {
                    // stats may be outdated, make sure person is still in sg
                    for(j = 0; j < eaSize(&e->supergroup->memberranks); j++)
                    {
                        if(e->supergroup->memberranks[j]->dbid == stat->db_id)
                        {
                            // Same rank as current best, add them to the group.
                            candidate_indices[num_candidate++] = i;
                            // If this assert ever trips, we've got a major problem w/ supergroup membership.
                            // Come talk to David G. for starters
                            assert(num_candidate < 4096);
                            // If this guy is more recent than the current window start, update it
                            if (cand_time > best_time)
                            {
                                best_time = cand_time;
                                LOG_SG_ENT( e, LOG_LEVEL_IMPORTANT, 0, "Rank New char at current best rank: adding db_id %d (%s) to the candidate list, slot %d, setting window time to %d", stat->db_id, stat->name, num_candidate - 1, cand_time);
                            }
                            else
                            {
                                LOG_SG_ENT( e, LOG_LEVEL_IMPORTANT, 0, "Rank New char at current best rank: adding db_id %d (%s) to the candidate list, slot %d, leaving window alone", stat->db_id, stat->name, num_candidate - 1);
                            }
                        }
                    }
                }
            }
        }

// Second half of the operation.  We have a list of candidates, and the most recent logout time of all of them.  Scan through the list, ignoring anyone 
// who is further out than a day, and then study the join date from the remaining people.  Award the star to the person with the oldest join date.
#if 1
        for (j = 0; j < num_candidate; j++)
        {
            SupergroupStats *stat;
            i = candidate_indices[j];
            stat = e->sgStats[i];

            // First thing, check if they are in the window.
            // If stat->online says they're online, or they are the actual entity that's causing this search, we don't bother checking the time.
            // Since last_online is updated when teh character is saved, there are certain pathological edge cases where someone can be online,
            // but still have a last_online that is weeks or even months old.
            if (stat->online || stat->db_id == e->db_id || stat->last_online >= best_time - 24 * 60 * 60)
            {
                // If we find a candidate with an earlier join date
                if (stat->member_since < best_joindate)
                {
                    // They get the cookie.
                    LOG_SG_ENT( e, LOG_LEVEL_IMPORTANT, 0, "Rank Final scan: considering db_id %d (%s) join date %d < best %d", stat->db_id, stat->name, stat->member_since, best_joindate);
                    best_id = stat->db_id;
                    best_joindate = stat->member_since;
                }
                else
                {
                    LOG_SG_ENT( e, LOG_LEVEL_IMPORTANT, 0, "Rank Final scan: rejecting db_id %d (%s) join date %d >= best %d", stat->db_id, stat->name, stat->member_since, best_joindate);
                }
            }
            else
            {
                LOG_SG_ENT( e, LOG_LEVEL_IMPORTANT, 0, "Rank Final scan: rejecting db_id %d (%s) Last online too early %d < ((%d - %d) == %d)", stat->db_id, stat->name, stat->last_online, best_time, 24 * 60 * 60, best_time - 24 * 60 * 60);
            }
        }

#endif
        if (best_id)
        {
            // DGNOTE 8/5/2010 - I counted up the total and dark yellow stars above.  This is where we would use those counts to disable promotion of a sub
            // yellow star player.
            memlog_printf(&g_teamlog, "calling group_genSetRank on %i, setting %i, %i", e->db_id, best_id, leaderRank);
            LOG_SG_ENT( e, LOG_LEVEL_IMPORTANT, 0, "Rank Autopromotion code is promoting dbid %d to rank %d (leader)", best_id, leaderRank);
            sgroup_setGeneric(e, SG_PERM_ANYONE, sgroup_genSetRank, &best_id, &leaderRank, "SetRank");
        }
        else if (!num_candidate)
        {
            // DGNOTE 8/5/2010 - If we're in here, we didn't find a candidate because everyone is dark.  Just log that fact but do nothing
            LOG_SG_ENT( e, LOG_LEVEL_VERBOSE, 0, "Rank Autopromotion code found everyone dark, not promoting right now");
        }
        else if (e->pl->playerType != e->supergroup->playerType)
        {
            // DGNOTE 8/5/2010 - Last ditch check.  There are non-dark members, so we think it's OK to promote this guy.  However we still don't do so if they are dark
            LOG_SG_ENT( e, LOG_LEVEL_VERBOSE, 0, "Rank Autopromotion code wants to promote %s, but that's denied because they're dark", e->name);
        }
        else
        {
            LOG_SG_ENT( e, LOG_LEVEL_IMPORTANT, 0, "Rank Autopromotion code seems to be confused, so making %s leader (rank %d)", e->name, leaderRank);
            sgroup_setGeneric(e, SG_PERM_ANYONE, sgroup_genSetRank, &e->db_id, &leaderRank, "SetRank");
        }
    }

    findOrReplaceSgStat(e->supergroup_id,&e->sgStats);
    processSgStats(e);
}


//
// sends a refresh stats command to the selected entity
void sgroup_refreshStats(int db_id)
{
    char buf[200];

    sprintf(buf, "sgstatsrelay %i", db_id);
    serverParseClient(buf, 0);
}

int checkforcrazylog(void)
{
    int i;
    char *orig;

    orig = g_teamlog.log[(MEMLOG_NUM_LINES+g_teamlog.tail-1)%MEMLOG_NUM_LINES];
    for (i = 2; i < 50; i++)
    {
        int line = (MEMLOG_NUM_LINES+g_teamlog.tail-i)%MEMLOG_NUM_LINES;
        if (strcmp(g_teamlog.log[line], orig))
            return 0;
    }
    return 1;
}

static int updateFromCachedSgStats(Entity *e)
{
    SupergroupStats        ***stats_p,*stat,*p_stat;
    int                    i,j,db_id,map_id=0,static_map_id=0;

    stats_p = findOrReplaceSgStat(e->supergroup_id,0);
    if (!stats_p)
        return 0;
    sgroup_clearStats( e );

    // check for updates and new members
    for(i=0;i<e->supergroup->members.count;i++)
    {
        OnlinePlayerInfo    *opi = 0;
        Entity                *member;

        db_id = e->supergroup->members.ids[i];
        member = entFromDbId(db_id);
        if (!member)
            opi = dbGetOnlinePlayerInfo(db_id);
        stat = 0;
        for(j=eaSize(stats_p)-1;j>=0;j--)
        {
            if ((*stats_p)[j]->db_id == db_id)
                stat = (*stats_p)[j];
        }
        if (!stat)
        {
            if (!member && !opi)
                return 0;
            stat = createSupergroupStats();
            eaPush(stats_p,stat);
        }
        stat->db_id = db_id;
        if( member && (!(member->pl->hidden&(1<<HIDE_SG)) || e->access_level > 0 ) )
            stat->online = 1;
        else
        {
            if( opi && (!(opi->hidefield&(1<<HIDE_SG)) || e->access_level > 0) )
                stat->online = 1;
            else
                stat->online = 0;
        }


        if (member && member->superstat)
        {
            strcpy(stat->name, member->name);
            stat->rank            = sgroup_rank( e, db_id );
            stat->level            = member->pchar->iLevel;
            strncpyt(stat->arch, member->pchar->pclass->pchName, sizeof(stat->arch));
            strncpyt(stat->origin, member->pchar->porigin->pchName, sizeof(stat->origin));
            stat->member_since        = member->superstat->member_since;
            stat->prestige            = member->pl->prestige;
            stat->playerType        = member->pl->playerType;
            map_id = db_state.map_id;
            static_map_id = db_state.base_map_id;
            stat->last_online    = timerSecondsSince2000();

        }
        else if (opi)
        {
            strcpy( stat->name, dbPlayerNameFromId(db_id));
            stat->rank = sgroup_rank( e, db_id );
            stat->level            = opi->level;

            if( opi->archetype >= 0 && opi->archetype < eaSize(&g_CharacterClasses.ppClasses) )
                strncpyt(stat->arch, g_CharacterClasses.ppClasses[opi->archetype]->pchName, sizeof(stat->arch));

            if( opi->origin >= 0 && opi->origin < eaSize(&g_CharacterOrigins.ppOrigins) )
                strncpyt(stat->origin, g_CharacterOrigins.ppOrigins[opi->origin]->pchName, sizeof(stat->origin));

            stat->member_since        = opi->member_since;
            stat->prestige            = opi->prestige;
            stat->playerType        = opi->type;
            map_id                    = opi->map_id;
            static_map_id            = opi->static_map_id;
            stat->last_online    = timerSecondsSince2000();
        }

        strncpyt( stat->zone, getTranslatedMapName(map_id), 256 );

        stat->map_id = map_id;
        stat->static_info = staticMapInfoFind(map_id);

        p_stat = createSupergroupStats();
        *p_stat = *stat;
        eaPush( &e->sgStats, p_stat );
    }
    processSgStats(e);
    return 1;
}

//#define    OFFLINE_MAX        2048

//OnlinePlayerInfo    offline_cache[OFFLINE_MAX];
//
//
void sgroup_getStats( Entity * e )
{
    char            match[200];
    static    int        timer,last_dbid;

    if( !e->supergroup_id )
        return;

    memlog_printf(&g_teamlog, "requesting sgroup stats for %i", e->db_id);
    if (checkforcrazylog())
        return;

    if (updateFromCachedSgStats(e))
        return;
    sprintf(match,"WHERE SupergroupsId = %d",e->supergroup_id);
    if (!timer)
        timer = timerAlloc();
    if (e->db_id == last_dbid && timerElapsed(timer) < 1.0) // you just asked for this, no need to ask again so soon
        return;
    timerStart(timer);
    last_dbid = e->db_id;
    dbReqCustomData(CONTAINER_ENTS,"Ents",0,match,
        "Name, ContainerId, Rank, Level, LastActive, Class, Origin, MemberSince, Prestige, PlayerType, SupergroupsId", sgroup_readDBData, e->db_id);
}

//
//
int sgroup_IsMember( Entity* leader, int db_id )
{
    if(!leader || !leader->supergroup_id || !leader->supergroup )
        return FALSE;

    return supergroup_IsMember(leader->supergroup, db_id );
}

int supergroup_IsMember(Supergroup *sg, int idEnt)
{
    int i;
    if( !sg )
        return FALSE;

    for( i = 0; i < eaSize(&sg->memberranks); i++ )
    {
        if( sg->memberranks[i]->dbid == idEnt )
            return TRUE;
    }

    return FALSE;
}


int sgroup_rank( Entity *e, int db_id )
{
    int i;
    if( !e || !e->supergroup_id || !e->supergroup )
        return 0;

    for( i = 0; i < eaSize(&e->supergroup->memberranks); i++ )
    {
        if(e->supergroup->memberranks[i]->dbid == db_id)
            return e->supergroup->memberranks[i]->rank;
    }

    return 0;
}

int sgroup_validRank(Entity* e, int rank)
{
    int i;

    // "member" and "superleader & above" ranks cannot be unused
    if(!rank || rank >= NUM_SG_RANKS_WITH_VARIABLE_PERMISSIONS)
        return TRUE;

    for(i = 0; i < SG_PERM_COUNT; i++)
    {
        if(sgroupPermissions[i].used && TSTB(e->supergroup->rankList[rank].permissionInt, i))
            return TRUE;
    }
    return FALSE;
}

int sgroup_TitheClamp(int tithe)
{
    if (tithe < SGROUP_TITHE_MIN) return SGROUP_TITHE_MIN;
    if (tithe > SGROUP_TITHE_MAX) return SGROUP_TITHE_MAX;
    return tithe;
}

int sgroup_DemoteClamp(int demotetimeout)
{
    if (demotetimeout < SGROUP_DEMOTE_MIN) return SGROUP_DEMOTE_MIN;
    if (demotetimeout > SGROUP_DEMOTE_MAX) return SGROUP_DEMOTE_MAX;
    return demotetimeout;
}

int sgroup_Update( Entity * e, Supergroup *sg, int on_create )
{
    if(on_create || teamLock( e, CONTAINER_SUPERGROUPS ))
    {
        Supergroup *newsg = e->supergroup;
        int i;

        if(on_create || sgroup_hasPermission(e, SG_PERM_MOTTO))
            strcpy(newsg->motto, sg->motto);
        if(on_create || sgroup_hasPermission(e, SG_PERM_MOTD))
            strcpy(newsg->msg, sg->msg);
        if(on_create || sgroup_hasPermission(e, SG_PERM_DESCRIPTION))
            strcpy(newsg->description, sg->description);
//        if(on_create || sgroup_hasPermission(e, SG_PERM_TITHING))
//            newsg->tithe = sgroup_TitheClamp(sg->tithe);
        if(on_create || sgroup_hasPermission(e, SG_PERM_AUTODEMOTE))
            newsg->demoteTimeout = sgroup_DemoteClamp(sg->demoteTimeout);
        if(on_create || sgroup_hasPermission(e, SG_PERM_COSTUME))
        {
            if ( stashFindElementConst( costume_HashTable, sg->emblem , NULL) )
                strcpy(newsg->emblem, sg->emblem );

            // verify
            newsg->colors[0] = sg->colors[0];
            newsg->colors[1] = sg->colors[1];
        }

        if(on_create || sgroup_hasPermission(e, SG_PERM_RANKS))
        {
            // The intent of this loop is that on SG creation we set everything up without question.
            // If this is just an edit (on_create is FALSE), then you can only change stuff that is
            // strictly less than your rank, except that the visible super-leader (NUM_SG_RANKS_VISIBLE - 1)
            // and GM's (NUM_SG_RANKS - 1 which is greater than NUM_SG_RANKS_VISIBLE - 1) can change anything.
            for(i = 0; i < NUM_SG_RANKS
                    && (on_create
                        || (i >= NUM_SG_RANKS_VISIBLE - 1)
                        || i < sgroup_rank(e, e->db_id));
                i++)
            {
                if( sg->rankList[i].name[0] && !IsAnyWordProfane(sg->rankList[i].name) )
                    strncpy(newsg->rankList[i].name, sg->rankList[i].name, sizeof(newsg->rankList[i].name));
                else
                {
                    char * title = localizedPrintf(e,rankNames[i]);
                    strncpy(newsg->rankList[i].name, title, sizeof(newsg->rankList[i].name));
                }

                if(i >= NUM_SG_RANKS_WITH_VARIABLE_PERMISSIONS)
                    memset(newsg->rankList[i].permissionInt, 0xff, sizeof(newsg->rankList[i].permissionInt));
                else
                {
                    int j;
                    U32 *myperms = sg->rankList[sgroup_rank(e, e->db_id)].permissionInt;
                    // Players can only modify permissions that they have, so
                    // we need to merge the rank's existing permissions with
                    // the new ones they've sent.
                    // Size is in bytes and we work with 4 byte words.
                    for(j = 0; j < sizeof(newsg->rankList[i].permissionInt) / 4; j++)
                    {
                        U32 update = sg->rankList[i].permissionInt[j];
                        U32 orig = newsg->rankList[i].permissionInt[j];
                        U32 mine = myperms ? myperms[j] : 0;

                        newsg->rankList[i].permissionInt[j] = (update & mine) | (orig & ~mine);
                    }
                }
            }
        }

        // permissions
        newsg->entryPermission = sg->entryPermission;

        if( on_create ? 0 : !teamUpdateUnlock( e, CONTAINER_SUPERGROUPS ))
            printf( "Failed to unlock teamup container, BIG TROUBLE, TELL BRUCE!!");

        return 1;
    }
    else
    {
        chatSendToPlayer( e->db_id, localizedPrintf(e,"UnableToCreateSupergroup"), INFO_USER_ERROR, 0 );
        return 0;
    }
}

static void addExPraetorian( Entity *creator, Supergroup *newsg )
{
    int count;
    int first = -1;
    int found = -1;

    for( count = 0; found == -1 && count < MAX_SUPER_GROUP_MEMBERS; count++)
    {
        if( first == -1 && !newsg->praetBonusIds[count] )
            first = count;

        if( newsg->praetBonusIds[count] == creator->db_id )
            found = count;
    }

    // They weren't already in the list somehow, and there is a place to
    // add them.
    if( first != -1 && found == -1 )
    {
        // Only their first SG gets the bonus, so they can never
        // reward it to another.
        creator->db_flags |= DBFLAG_PRAET_SG_JOIN;

        newsg->praetBonusIds[first] = creator->db_id;
        newsg->prestige += PRAET_SG_JOIN_BONUS;
        newsg->prestige = MINMAX(newsg->prestige, -MAX_SUPERGROUP_PRESTIGE, MAX_SUPERGROUP_PRESTIGE);
    }
}
//  0 - failure
//  1 - success
int sgroup_Create( Entity * creator, Supergroup *sg )
{
    int    sg_id;
    int count;

    if( sg->name[0] == 0 || ValidateName(sg->name, creator->auth_name, false,SG_NAME_LEN) != VNR_Valid || sgroup_NameExists(sg->name) )
    {
        chatSendToPlayer(creator->db_id, localizedPrintf(creator,"SupergroupInvalidName"), INFO_USER_ERROR, 0);
        return 0;
    }

    if (creator->pl && !AccountCanCreateSG(&creator->pl->account_inventory, creator->pl->loyaltyPointsEarned, creator->pl->account_inventory.accountStatusFlags))
    {
        chatSendToPlayer(creator->db_id, localizedPrintf(creator, "CannotCreateFreeAccountSG"), INFO_USER_ERROR, 0);
        return 0;
    }

    for( count = 0; count < MAX_SUPER_GROUP_MEMBERS; count++)
    {
        sg->praetBonusIds[count] = 0;
    }

    // first check to see if the "leader" already has a team
    if (!entIsTrial(creator))
    {
        if( !creator->supergroup_id )
        {
            if(creator->pchar->iLevel >= 9 || creator->access_level >= ACCESS_DEBUG)
            {
                sg_id = teamCreate( creator, CONTAINER_SUPERGROUPS );

                devassert(creator->supergroup->memberranks);
                // now set defaults for the supergroup
                if ( teamLock( creator, CONTAINER_SUPERGROUPS ))
                {
                    int i;
                    Supergroup* newsg = creator->supergroup;
                    SupergroupMemberInfo *member = createSupergroupMemberInfo();

                    newsg->dateCreated = dbSecondsSince2000();
                    strcpy(newsg->name, sg->name);

                    if ( stashFindElementConst( costume_HashTable, sg->emblem , NULL) )
                        strcpy(newsg->emblem, sg->emblem );
                    else
                        strcpy(newsg->emblem, "" );

                    newsg->colors[0] = sg->colors[0];
                    newsg->colors[1] = sg->colors[1];
                    newsg->playerType = creator->pl->playerType; //copy over Hero or villain type
                    for (i = 0; i < MAX_SUPERGROUP_ALLIES; i++)
                    {
                        newsg->allies[i].db_id = 0;
                    }

                    // teamCreate has already added the creator so just change the rank
                    creator->supergroup->memberranks[0]->rank = NUM_SG_RANKS_VISIBLE - 1;

                    if(server_state.beta_base==1)
                        newsg->prestige = 9000000;
                    else if(server_state.beta_base>1)
                        newsg->prestige = 36000000;

                    // Creating is the same as joining your first SG so you get
                    // the special bonus if you're Praetorian.
                    if( ENT_IS_EX_PRAETORIAN(creator) && !(creator->db_flags & DBFLAG_PRAET_SG_JOIN ) )
                    {
                        addExPraetorian( creator, newsg );
                    }

                    sgroup_Update(creator, sg, 1);

                    if( !teamUpdateUnlock( creator, CONTAINER_SUPERGROUPS ))
                        printf( "Failed to unlock teamup container, BIG TROUBLE, TELL BRUCE!!");

                    if( creator->superstat )
                        destroySupergroupStats( creator->superstat );

                    creator->superstat = createSupergroupStats();
                    creator->superstat->db_id = creator->db_id;
                    creator->superstat->member_since = timerSecondsSince2000();    // set the time they created it
                    svrSendEntListToDb(&creator, 1);

                    sgroup_getStats( creator );
                    chatSendToPlayer( creator->db_id, localizedPrintf(creator,"CreatedSupergroup", sg->name ), INFO_SVR_COM, 0 );
                    ArenaPlayerAttributeUpdate(creator);

                    return 1;
                }
                else
                {
                    chatSendToPlayer( creator->db_id, localizedPrintf(creator,"UnableToCreateSupergroup"), INFO_USER_ERROR, 0 );
                    return 0;
                }
            }
            else
            {
                chatSendToPlayer( creator->db_id, localizedPrintf(creator,"CouldNotActionReason",
                    "CreateSuperGroup", "YouMustBeLevel10"), INFO_USER_ERROR, 0 );
                return 0;
            }
        }
        else
        {
            chatSendToPlayer( creator->db_id, localizedPrintf(creator,"CouldNotActionReason",
                "CreateSuperGroup", "AlreadyInSupergroup"), INFO_USER_ERROR, 0 );
            return 0;
        }
    }
    else
    {
        chatSendToPlayer( creator->db_id, localizedPrintf(creator, "CannotInviteTrialAccountSG"), INFO_USER_ERROR, 0 );
        return 0;
    }
}

int sgroup_ReceiveUpdate( Entity * e, Packet * pak )
{
    Supergroup sg;
    int i;

    // first eat data
    strncpyt( sg.name, pktGetString( pak ), ARRAY_SIZE( sg.name ) );
    strncpyt( sg.emblem, pktGetString( pak ), ARRAY_SIZE( sg.emblem ) );
    sg.colors[0] = pktGetBits( pak, 32 );
    sg.colors[1] = pktGetBits( pak, 32 );
    strncpyt( sg.description, pktGetString( pak ), ARRAY_SIZE( sg.description ) );
    strncpyt( sg.motto, pktGetString( pak ), ARRAY_SIZE( sg.motto ) );
    strncpyt( sg.msg, pktGetString( pak ), ARRAY_SIZE( sg.msg ) );
    sg.tithe = sgroup_TitheClamp(pktGetBits( pak, 32 ));
    sg.demoteTimeout = sgroup_DemoteClamp(pktGetBits( pak, 32 ));
    for(i = 0; i < NUM_SG_RANKS; i++)
    {
        strncpyt( sg.rankList[i].name,    pktGetString( pak ), ARRAY_SIZE( sg.rankList[i].name ) );
        pktGetBitsArray( pak, sizeof(sg.rankList[i].permissionInt) * 8, sg.rankList[i].permissionInt );
    }
    sg.entryPermission = pktGetBitsAuto(pak);
    if(!sgrpbaseentrypermission_Valid(sg.entryPermission))
    {
        dbLog("cheater",NULL,"Player with authname %s tried to send invalid permission %d\n", e ? e->auth_name : "", sg.entryPermission);
        sg.entryPermission = 0;
    }
    // obvious failure somewhere, assert?
    if( !e )
        return 0;

    if( e->supergroup_id)
        return sgroup_Update( e, &sg, 0 );
    else
        return sgroup_Create( e, &sg );
}

int sgroup_CreateDebug( Entity * creator, char * name )
{
    Supergroup sg;

    ZeroStruct(&sg);
    strncpyt( sg.name, name, ARRAY_SIZE( sg.name ) );
    // defaults should make this work for cov
//    strncpyt( sg.leaderTitle, "Leader", ARRAY_SIZE( sg.leaderTitle ) );
//    strncpyt( sg.captainTitle, "Captain", ARRAY_SIZE( sg.captainTitle ) );
//    strncpyt( sg.memberTitle, "Member", ARRAY_SIZE( sg.memberTitle ) );
    strncpyt( sg.emblem, "Anarchy", ARRAY_SIZE( sg.emblem ) );
    sg.colors[0] = 0x000000ff;
    sg.colors[1] = 0xffffffff;
    strncpyt( sg.description,    "Debug Supergroup", ARRAY_SIZE( sg.description ) );
    sg.tithe = sgroup_TitheClamp(0);
    sg.demoteTimeout = sgroup_DemoteClamp(100);

    // obvious failure somewhere, assert?
    if( !creator )
        return 0;

    return sgroup_Create( creator, &sg );
}

int sgroup_JoinDebug(Entity *e, char *name)
{
    int map_num;
    int id, ret = 0;
    char *container_str;
    id = dbSyncContainerFindByElement(CONTAINER_SUPERGROUPS,"Name",name,&map_num,0);

    //if supergroup does not exist...
    if (id == -1)
    {
        ///...inform player and do nothing else
        chatSendToPlayer(e->db_id, localizedPrintf(e, "CouldNotActionReason", "JoinString", "SuperGroupNotExist"), INFO_USER_ERROR, 0);
    }
    ///...otherwise
    else
    {
        if ( sgrpFromDbId(id) == NULL )
        {
            // I ought to check for failure here, in the event that someone else has the container locked.
            dbSyncContainerRequest(CONTAINER_SUPERGROUPS, id, CONTAINER_CMD_TEMPLOAD, 0 );
        }
        ret = teamAddMember( e, CONTAINER_SUPERGROUPS, id, -1);
        if (sgrpFromDbId(id))
        {
            container_str = packageSuperGroup(sgrpFromDbId(id));
            dbSyncContainerUpdate(CONTAINER_SUPERGROUPS,id,CONTAINER_CMD_UNLOCK,container_str);
        }
        else
        {
            dbSyncContainerUpdate(CONTAINER_SUPERGROUPS,id,CONTAINER_CMD_UNLOCK_NOMODIFY,"");
        }
    }
    return ret;
}

void sgroup_rename(Entity* player, char* oldname, char* newname, int dbid_response)
{
    char *buf;
    int success = 0;

    if (!player->supergroup_id || !player->supergroup)
    {
        buf = localizedPrintf( player, "NotOnSgroup", player->name);
    }
    else if (!newname || !newname[0])
    {
        buf = localizedPrintf( player,"BadRename" );
    }
    else if (strlen(newname) >= SG_NAME_LEN)
    {
        buf = localizedPrintf( player,"SgroupNameTooLong", newname);
    }
    else if (ValidateName(newname, player->auth_name, false,SG_NAME_LEN) != VNR_Valid)
    {
        buf = localizedPrintf( player,"InvalidSgroupName", newname);
    }
    else if (sgroup_NameExists(newname))
    {
        buf = localizedPrintf( player,"AlreadySgroupName", newname);
    }
    else if (teamLock(player, CONTAINER_SUPERGROUPS))
    {
        if (stricmp(oldname, player->supergroup->name)!=0)
        {
            buf = localizedPrintf( player,"SgroupNameConfusion", player->name, player->supergroup->name, oldname);
        }
        else
        {
            buf = localizedPrintf( player,"SgroupRenamed", player->name, newname, player->supergroup->name);
            strcpy(player->supergroup->name, newname);
            success = 1;
        }
        teamUpdateUnlock(player, CONTAINER_SUPERGROUPS);
    }
    else
    {
        buf = localizedPrintf( player,"SgroupRenameErr", player->name);
    }
    chatSendToPlayer(dbid_response, buf, INFO_USER_ERROR, 0);
    sgroup_UpdateAllMembers(player->supergroup);
}

void sgroup_CsrWho( Entity * e, int leaderOnly )
{
    int i, j, k;
    static char* buf = NULL;

    if(!e->supergroup)
    {
        conPrintf( e->client, "%s is not a member of a supergroup.", e->name );
        return;
    }

    conPrintf( e->client, "Supergroup Name: %s", e->supergroup->name );
    conPrintf( e->client, "Motto: %s", e->supergroup->motto );
    conPrintf( e->client, "Motd: %s", e->supergroup->msg );
    estrPrintCharString( &buf, "Titles: ");
    for(i = 0; i < NUM_SG_RANKS; i++)
        estrConcatf( &buf, "%s%s", e->supergroup->rankList[i].name, i < NUM_SG_RANKS - 1 ? ", " : "");
    conPrintf( e->client, buf);
    conPrintf( e->client, "Has %d prestige", e->supergroup->prestige );
    conPrintf( e->client, "Member Count: %d", e->supergroup->members.count );
    conPrintf( e->client, "-----------------------------------------------------------" );
    for( i = NUM_SG_RANKS - 1; i >= 0; i--)
    {
        if (leaderOnly && i != NUM_SG_RANKS - 2)
        {
            continue;
        }
        conPrintf( e->client, "%s (%s):", localizedPrintf(e, rankNames[i]), e->supergroup->rankList[i].name );
        for( j = 0; j < eaSize(&e->supergroup->memberranks); j++ )
        {
            if(e->supergroup->memberranks[j]->rank == i)
            {
                for( k = 0; k < e->supergroup->members.count; k++)
                {
                    if( e->supergroup->memberranks[j]->dbid == e->supergroup->members.ids[k])
                    {
                        conPrintf( e->client, "  %s", e->supergroup->members.names[k]);
                        break;
                    }
                }
            }
        }
    }
}


//
//
void sgroup_AcceptOffer( Entity *leader, int new_dbid, char * new_name )
{
    char tmp[512];

    // first verify the "leader" already has a team
    if( !leader->supergroup_id )
    {
        sendChatMsg( leader, localizedPrintf(leader,"CouldNotActionReason",
            "InviteString", "YouAreNotInSupergroup" ), INFO_USER_ERROR, 0 );
        return;
    }

    if( !sgroup_hasPermission(leader, SG_PERM_INVITE ) ) // verify the leader has rank to invite someone
    {
        sendChatMsg( leader, localizedPrintf(leader,"NotHighEnoughRankAction",
            "InviteString"), INFO_USER_ERROR, 0 );
        return;
    }

    if (leader->supergroup->members.count >= MAX_SUPER_GROUP_MEMBERS)
    {
        sendChatMsg( leader, localizedPrintf(leader,"CouldNotActionReason",
            "InviteString", "SuperGroupFull"), INFO_USER_ERROR, 0 );
        return;
    }

    // must be done with relay
    sprintf(tmp, "sg_accept_relay %i %i %i", new_dbid, leader->supergroup_id, leader->db_id);
    serverParseClient(tmp, NULL);
}

void sgroup_AcceptRelay(Entity* e, int sg_id, int invited_by)
{

    if( teamAddMember( e, CONTAINER_SUPERGROUPS, sg_id, invited_by) )
    {
        sgroup_initiate(e);
        chatSendToSupergroup( e, localizedPrintf(e, "hasJoinedYourSgroup", e->name, e->supergroup->name), INFO_SVR_COM );
        sgroup_refreshStats(invited_by);
        ArenaPlayerAttributeUpdate(e);

        // Praetorians grant a bonus in prestige to the first SG they join.
        if( ENT_IS_EX_PRAETORIAN(e) && !( e->db_flags & DBFLAG_PRAET_SG_JOIN ) )
        {
            if( teamLock( e, CONTAINER_SUPERGROUPS ) )
            {
                addExPraetorian( e, e->supergroup );
                teamUpdateUnlock( e, CONTAINER_SUPERGROUPS );
            }
        }
    }
    else // failure
    {
        // just let the leader and the offered user know something wasn't valid
        chatSendToPlayer( invited_by, localizedPrintf(e,"couldNotJoinYourSgroup", e->name  ), INFO_USER_ERROR, 0 );
        sendChatMsg( e, localizedPrintf(e, "youCouldNotJoinSgroup"), INFO_USER_ERROR, 0 );
    }
}

//
//
void sgroup_AltRelay(Entity* e, int sg_id, int sg_type, int inviter_id, int inviter_auth_id)
{
	// Freaking dirty cheaters
	if (e->auth_id != inviter_auth_id)
	{
		chatSendToPlayer(inviter_id, localizedPrintf(e, "%s is not on your account, cheater!", e->name, e->auth_id, inviter_auth_id), INFO_USER_ERROR, 0 );
		return;
	}

	if (e->supergroup_id)
	{
		chatSendToPlayer(inviter_id, localizedPrintf(e, "CouldNotActionPlayerReason", "InviteString", e->name, "PlayerIsAlreadyInSuperGroup"), INFO_USER_ERROR, 0);
		return;
	}

	// Shouldn't be possible since this ent had to have been offline, but just in case...
	if (e->pl->inviter_dbid)
	{
		chatSendToPlayer(inviter_id, localizedPrintf(e, "CouldNotActionPlayerReason", "InviteString", e->name, "PlayerIsConsideringAnotherOffer"), INFO_USER_ERROR, 0);
		return;
	}

	// Hand off to the real function that does this
	sgroup_AcceptRelay(e, sg_id, inviter_id);
}

//
//
void sgroup_KickMember( Entity *leader, char *name_param )
{
    char tmp[512],name[512];
    Entity *kicked;
    int kickedID;

    strcpy(name, name_param); // name_param sometimes gets invalidated by serverParseClient
    kickedID = dbPlayerIdFromName( name );
    if (kickedID <= 0)
        return;

    kicked = entFromDbId( kickedID );

    // make sure leader on supergroup
    if (!leader->supergroup)
    {
        sendChatMsg( leader, localizedPrintf(leader,"CouldNotActionReason",
            "KickString", "YouAreNotInSupergroup"), INFO_USER_ERROR, 0 );
        return;
    }

    // first make sure they are on the team
    if( !sgroup_IsMember( leader, kickedID ) )
    {
        sendChatMsg( leader, localizedPrintf(leader,"CouldNotActionPlayerReason",
            "KickString", name, "playerNotInSgroup"), INFO_USER_ERROR, 0 );
        return;
    }

    // verify this person has the rank to kick
    if( !sgroup_hasPermission( leader, SG_PERM_KICK ) )
    {
        sendChatMsg( leader, localizedPrintf(leader,"NotHighEnoughRankAction",
            "KickString"), INFO_USER_ERROR, 0 );
        return;
    }

    if ( sgroup_rank(leader, leader->db_id) <= sgroup_rank(leader, kickedID) )
    {
        sendChatMsg( leader, localizedPrintf(leader,"CouldNotActionPlayerReason",
            "KickString", name, "CannotKickHigherRank"), INFO_USER_ERROR, 0 );
        return;
    }

    // this is done with a relay to guarantee the receiving mapserver doesn't have to
    // worry about losing the teamup asynchronously
    LOG_SG_ENT( leader, LOG_LEVEL_IMPORTANT, 0, "SuperGroup:Kick %s kicked.", name);
    sprintf(tmp, "sg_kick_relay %i %i", kickedID, leader->db_id);
    serverParseClient(tmp, NULL);
}

void sgroup_ClearRemovedMember(Entity *e)
{
    e->pl->prestige = 0;
    character_ClearInventory(e->pchar,kInventoryType_BaseDetail,__FUNCTION__);
    ArenaPlayerAttributeUpdate(e);
}

void sgroup_KickRelay(Entity* e, int kicked_by)
{
    int sg_id = e->supergroup_id;

    // Removing them from the SG in this way also deals with revoking the
    // Praetorian bonus - by the time they've been deleted they have no
    // supergroup pointer so we can't do it here.
    if (teamDelMember(e, CONTAINER_SUPERGROUPS, 1, kicked_by))
    {
        sgroup_ClearRemovedMember(e);

        sendEntsMsg( CONTAINER_SUPERGROUPS, sg_id, INFO_SVR_COM, 0, "%s%s", DBMSG_CHAT_MSG, localizedPrintf(e,"wasKickedFromSgroup", e->name) );
        chatSendToPlayer( e->db_id, localizedPrintf(e,"youWereKickedFromSgroup") , INFO_SVR_COM, 0 );
    }
    else
    {
        chatSendToPlayer( kicked_by, localizedPrintf(e,"failedToKick", e->name) , INFO_USER_ERROR, 0 );
    }
}

//
//
void sgroup_MemberQuit( Entity *quitter )
{
    int team_id = quitter->supergroup_id;

    if( !team_id )
    {
        sendChatMsg( quitter, localizedPrintf(quitter,"CouldNotActionReason",
            "QuitSupergroup", "NotInSuperGroup"), INFO_USER_ERROR, 0 );
        return;
    }

    if (quitter->supergroup_id && quitter->supergroup->members.count == 1)
        alliance_CancelAllAlliances(quitter->supergroup_id);

    // now remove them
    LOG_SG_ENT( quitter, LOG_LEVEL_IMPORTANT, 0, "Quit" );

    // Removing them from the container like this also deals with the
    // Praetorian bonus, if any.
    if( teamDelMember( quitter, CONTAINER_SUPERGROUPS, 0, 0 ))
    {
        sgroup_ClearRemovedMember(quitter);

        sendEntsMsg(CONTAINER_SUPERGROUPS, team_id, INFO_SVR_COM, 0, "%s%s", DBMSG_CHAT_MSG, localizedPrintf(quitter,"hasQuitSgroup", quitter->name));
        sendChatMsg( quitter, localizedPrintf(quitter,"youHaveQuitSgroup" ), INFO_SVR_COM, 0 );
    }
    else
    {
        sendChatMsg( quitter, localizedPrintf(quitter,"UnableToQuitSgroup"), INFO_USER_ERROR, 0 );
    }
}

void sgroup_initiate( Entity * e )
{
    if( !e->superstat )
        e->superstat = createSupergroupStats();

    e->superstat->member_since = timerSecondsSince2000();
    e->superstat->rank = 0;
    e->pl->prestige = 0;
    svrSendEntListToDb(&e, 1); // make sure this gets sent so that the rank is cleared
    sgroup_getStats( e );
    sgroup_setSuperCostume( e , 1);
}

// same as sgroup_promote, but doesnt check to see if promoted_by is in the same supergroup or has permission to promote/demote
void sgroup_promoteCsr( Entity *e, int promoted_by, int promoter_group, int promoter_rank, int new_rank )
{
    char action[32];
    char promoter_name[64];
    int modified_rank = sgroup_rank(e, e->db_id);

    strncpyt(promoter_name, dbPlayerNameFromId(promoted_by), sizeof(promoter_name));

    if( new_rank > 0 )
        strcpy( action, "PromoteString" );
    else
        strcpy( action, "DemoteString" );

    modified_rank += new_rank;

    // skip ranks without permissions while promoting/demoting (lowest and highest are always valid)
    //while(!sgroup_validRank(e, modified_rank))
    //    new_rank > 0 ? modified_rank++ : modified_rank--;

    // Just to be on the safe side, CSR cannot promote people to the CSR rank.
    if( modified_rank >= NUM_SG_RANKS - 1 )
    {
        chatSendToPlayer( promoted_by, localizedPrintf(e,"CouldNotActionPlayerReason", action, e->name, "CantPromoteGM"), INFO_USER_ERROR, 0 );
        return;
    }
    if( modified_rank == NUM_SG_RANKS - 2 )
    {
        // Tell CS what they just did
        chatSendToPlayer( promoted_by, localizedPrintf(e,"PromotedToSupremeLeader", e->name), INFO_SVR_COM, 0 );
    }
    if( modified_rank < 0 )
    {
        chatSendToPlayer( promoted_by, localizedPrintf(e,"CouldNotActionPlayerReason", action, e->name, "AlreadyLowestRank"), INFO_USER_ERROR, 0 );
        return;
    }

    sendChatMsg( e, localizedPrintf(e, "YouAreNowRank", e->supergroup->rankList[modified_rank].name ), INFO_SVR_COM, 0 );

    if( new_rank > 0 )
        strcpy( action, "PromotedString" );
    else
        strcpy( action, "DemotedString" );

    chatSendToPlayer( promoted_by, localizedPrintf(e,"YouHaveBlankedPlayer", action, e->name), INFO_USER_ERROR, 0 );

    sgroup_setGeneric(e, SG_PERM_ANYONE, sgroup_genSetRank, &e->db_id, &modified_rank, "SetRank");
    LOG_SG_ENT( e, LOG_LEVEL_IMPORTANT, 0, "Rank changed to %d by %s", modified_rank, promoter_name);

    svrSendEntListToDb(&e, 1);
    sgroup_getStats( e );
    ArenaPlayerAttributeUpdate(e);

    sgroup_UpdateAllMembers(e->supergroup);
    if (promoted_by)
        sgroup_refreshStats(promoted_by);
}

void sgroup_promote( Entity *e, int promoted_by, int promoter_group, int promoter_rank, int new_rank )
{
    char action[32];
    char* promoter_name = dbPlayerNameFromId(promoted_by);
    int modified_rank = sgroup_rank(e, e->db_id);

    if( new_rank > 0 )
        strcpy( action, "PromoteString" );
    else
        strcpy( action, "DemoteString" );

    if( promoter_group == e->supergroup_id) // make sure the demoter is in the same group as demotee
    {
        if( promoter_rank > modified_rank) // make sure the demoter is higher ranking than demotee
        {
            // permissions are now checked in cmdchat.c on the demoter's mapserver instead of the demotee's mapserver!

            modified_rank += new_rank;

            // skip ranks without permissions while promoting/demoting (lowest and highest are always valid)
            //while(!sgroup_validRank(e, modified_rank))
            //    new_rank > 0 ? modified_rank++ : modified_rank--;

            if( modified_rank > NUM_SG_RANKS_VISIBLE - 1 )
            {
                chatSendToPlayer( promoted_by, localizedPrintf(e,"CouldNotActionPlayerReason", action, e->name, "AlreadyLeader"), INFO_USER_ERROR, 0 );
                return;
            }
            else if( modified_rank < 0 )
            {
                chatSendToPlayer( promoted_by, localizedPrintf(e,"CouldNotActionPlayerReason", action, e->name, "AlreadyLowestRank"), INFO_USER_ERROR, 0 );
                return;
            }

            sendChatMsg( e, localizedPrintf(e, "YouAreNowRank", e->supergroup->rankList[modified_rank].name ), INFO_SVR_COM, 0 );

            if( new_rank > 0 )
                strcpy( action, "PromotedString" );
            else
                strcpy( action, "DemotedString" );

            chatSendToPlayer( promoted_by, localizedPrintf(e,"YouHaveBlankedPlayer", action, e->name), INFO_USER_ERROR, 0 );

            sgroup_setGeneric(e, SG_PERM_ANYONE, sgroup_genSetRank, &e->db_id, &modified_rank, "SetRank");

            LOG_SG_ENT( e, LOG_LEVEL_IMPORTANT, 0, "Rank changed to %d by %s", modified_rank, promoter_name);

            svrSendEntListToDb(&e, 1);
            sgroup_getStats( e );
            ArenaPlayerAttributeUpdate(e);
        }
        else
            chatSendToPlayer( promoted_by, localizedPrintf(e,"CouldNotActionPlayerReason", action, e->name, "NotHigherRank"), INFO_USER_ERROR, 0 );

    }
    else
        chatSendToPlayer( promoted_by, localizedPrintf(e,"CouldNotActionPlayerReason", action, e->name, "NotInSupergroup"), INFO_USER_ERROR, 0 );

    sgroup_UpdateAllMembers(e->supergroup);
    if (promoted_by)
        sgroup_refreshStats(promoted_by);
}

void sgroup_setSuperCostume( Entity *e, int send )
{
    costume_makeSupergroup( e );

    if( send )
    {
        START_PACKET( pak, e, SERVER_SUPERGROUP_COSTUME )
            costume_send(pak, e->pl->superCostume, 1 );
        END_PACKET
    }
}

void sgroup_startConfirmation(Entity *e, char *player_name)
{
    START_PACKET(pak, e, SERVER_CONFIRM_SG_PROMOTE)
    pktSendString(pak, player_name);
    END_PACKET
}

int sgroup_verifyCostume( Packet *pak, Entity *e )
{
    int i, j;

    SGColorBits primaryU;
    primaryU.SGBitSetLow = pktGetBitsPack(pak, 32);
    primaryU.SGBitSetHigh = pktGetBitsPack(pak, 32);
    SGColorBits secondaryU;
    secondaryU.SGBitSetLow = pktGetBitsPack(pak, 32);
    secondaryU.SGBitSetHigh = pktGetBitsPack(pak, 32);
    SGColorBits primary2U;
    primary2U.SGBitSetLow = pktGetBitsPack(pak, 32);
    primary2U.SGBitSetHigh = pktGetBitsPack(pak, 32);
    SGColorBits secondary2U;
    secondary2U.SGBitSetLow = pktGetBitsPack(pak, 32);
    secondary2U.SGBitSetHigh = pktGetBitsPack(pak, 32);
    SGColorBits tertiaryU;
    tertiaryU.SGBitSetLow = pktGetBitsPack(pak, 32);
    tertiaryU.SGBitSetHigh = pktGetBitsPack(pak, 32);
    SGColorBits quaternaryU;
    quaternaryU.SGBitSetLow = pktGetBitsPack(pak, 32);
    quaternaryU.SGBitSetHigh = pktGetBitsPack(pak, 32);
    int hide_emblem            = pktGetBits( pak, 1 );

    for( i = 0; i < e->pl->costume[0]->appearance.iNumParts && i < MAX_COSTUME_PARTS/2; i++ )
    {
        if( ((primaryU.SGBitSet >> (i*2)) & 3) == 0 || ((secondaryU.SGBitSet >> (i*2)) & 3) == 0  )
            return false;
    }

    for( j = 0; i < e->pl->costume[0]->appearance.iNumParts; j++, i++ )
    {
        if( ((primary2U.SGBitSet >> (j*2)) & 3) == 0 || ((secondary2U.SGBitSet >> (j*2)) & 3) == 0  )
            return false;

        if( ((tertiaryU.SGBitSet >> (j*2)) & 3) == 0 || ((quaternaryU.SGBitSet >> (j*2)) & 3) == 0  )
            return false;
    }

    e->pl->superColorsPrimaryU        = primaryU;
    e->pl->superColorsSecondaryU        = secondaryU;
    e->pl->superColorsPrimary2U        = primary2U;
    e->pl->superColorsSecondary2U    = secondary2U;
    e->pl->superColorsTertiaryU        = tertiaryU;
    e->pl->superColorsQuaternaryU    = quaternaryU;
    e->pl->hide_supergroup_emblem    = hide_emblem;
    e->send_costume = 1;
    return true;
}

int sgroup_verifyCostumeEx( Packet *pak, Entity *e )
{
    int num_slot;
    int hide_emblem;
    int i, j, k;
    Costume *costume;
    SGColorBits sgColorsU[NUM_SG_COLOR_SLOTS][6];

    num_slot = pktGetBits( pak, 8 );
    for (k = 0; k < num_slot; k++)
    {
        sgColorsU[k][0].SGBitSetLow = pktGetBitsPack(pak, 32);
        sgColorsU[k][0].SGBitSetHigh = pktGetBitsPack(pak, 32);
        sgColorsU[k][1].SGBitSetLow = pktGetBitsPack(pak, 32);
        sgColorsU[k][1].SGBitSetHigh = pktGetBitsPack(pak, 32);
        sgColorsU[k][2].SGBitSetLow = pktGetBitsPack(pak, 32);
        sgColorsU[k][2].SGBitSetHigh = pktGetBitsPack(pak, 32);
        sgColorsU[k][3].SGBitSetLow = pktGetBitsPack(pak, 32);
        sgColorsU[k][3].SGBitSetHigh = pktGetBitsPack(pak, 32);
        sgColorsU[k][4].SGBitSetLow = pktGetBitsPack(pak, 32);
        sgColorsU[k][4].SGBitSetHigh = pktGetBitsPack(pak, 32);
        sgColorsU[k][5].SGBitSetLow = pktGetBitsPack(pak, 32);
        sgColorsU[k][5].SGBitSetHigh = pktGetBitsPack(pak, 32);
    }
    hide_emblem = pktGetBits( pak, 1 );
    costume = e->pl->costume[e->pl->current_costume];
    for (k = 0; k < num_slot; k++)
    {
        for( i = 0; i < costume->appearance.iNumParts && i < MAX_COSTUME_PARTS/2; i++ )
        {
            if( ((sgColorsU[k][0].SGBitSet >> (i*2)) & 3) == 0 || ((sgColorsU[k][1].SGBitSet >> (i*2)) & 3) == 0  )
                return false;
        }

        for( j = 0; i < costume->appearance.iNumParts; j++, i++ )
        {
            if( ((sgColorsU[k][2].SGBitSet >> (j*2)) & 3) == 0 || ((sgColorsU[k][3].SGBitSet >> (j*2)) & 3) == 0  )
                return false;

            if( ((sgColorsU[k][4].SGBitSet >> (j*2)) & 3) == 0 || ((sgColorsU[k][5].SGBitSet >> (j*2)) & 3) == 0  )
                return false;
        }
    }

    for (k = 0; k < num_slot; k++)
    {
        costume->appearance.superColorsPrimaryU[k] = sgColorsU[k][0];
        costume->appearance.superColorsSecondaryU[k] = sgColorsU[k][1];
        costume->appearance.superColorsPrimary2U[k] = sgColorsU[k][2];
        costume->appearance.superColorsSecondary2U[k] = sgColorsU[k][3];
        costume->appearance.superColorsTertiaryU[k] = sgColorsU[k][4];
        costume->appearance.superColorsQuaternaryU[k] = sgColorsU[k][5];
        if (k == costume->appearance.currentSuperColorSet)
        {
            e->pl->superColorsPrimaryU = sgColorsU[k][0];
            e->pl->superColorsSecondaryU = sgColorsU[k][1];
            e->pl->superColorsPrimary2U = sgColorsU[k][2];
            e->pl->superColorsSecondary2U = sgColorsU[k][3];
            e->pl->superColorsTertiaryU = sgColorsU[k][4];
            e->pl->superColorsQuaternaryU = sgColorsU[k][5];
            e->pl->hide_supergroup_emblem = hide_emblem;
            e->send_costume = 1;
        }
    }
    return true;
}


void sgroup_sendResponse( Entity * e, int valid )
{
    START_PACKET( pak, e, SERVER_SGROUP_CREATE_REPLY )

        pktSendBits( pak, 1, valid );

    if( valid )
    {
        sgroup_setSuperCostume( e, 0 );
        costume_send(pak, e->pl->superCostume, 1 );
    }

    END_PACKET
}

void sgroup_setGeneric(Entity *e, int permission, void(*func)(Entity*, void*, void*), void* param, void* param2, char* actionName)
{
    assert( e );
    assert( e->pchar );

    if( e->supergroup_id )
    {
        if( sgroup_IsMember( e, e->db_id ) && sgroup_hasPermission(e, permission) )
        {
            memlog_printf(&g_teamlog, "doing setGeneric lock for %i", e->db_id);
            teamLock( e, CONTAINER_SUPERGROUPS );

            // ab: hackery. don't know if we'll ever add lock state to 
            // the supergroup, but if we do: remove this
            s_genLockSgId = e->supergroup_id;
            {
                func(e, param, param2);
            }
            s_genLockSgId = 0;;

            if( !teamUpdateUnlock( e, CONTAINER_SUPERGROUPS ))
                printf( "Failed to unlock teamup container, BIG TROUBLE, TELL BRUCE!!");
        }
        else
            chatSendToPlayer( e->db_id, localizedPrintf(e,"CouldNotActionReason", actionName, "NotHighEnoughRank") , INFO_USER_ERROR, 0 );
    }
    else
        chatSendToPlayer( e->db_id, localizedPrintf(e, "CouldNotActionReason", actionName, "NotInSupergroup"), INFO_USER_ERROR, 0 );

    sgroup_UpdateAllMembers(e->supergroup);
}

void sgroup_genRenameRank(Entity *e, void* rank, void* name)
{
    if( ((char*)name)[0] == 0 )
    {
        chatSendToPlayer(e->db_id, localizedPrintf(e,"incorrectFormat",
            "sgrenameString", "rankNumberString", "sgTitleString", "sgrenameSynonyms"  ), INFO_USER_ERROR, 0);
        return;
    }
    else if( *(int*)rank >= NUM_SG_RANKS_VISIBLE)
    {
        chatSendToPlayer(e->db_id, localizedPrintf(e,"incorrectFormat",
            "sgrenameString", "rankNumberString", "sgTitleString", "sgrenameSynonyms"), INFO_USER_ERROR, 0);
        return;
    }

    strncpyt(e->supergroup->rankList[*(int*)rank].name, (char*)name, SG_TITLE_LEN);

    sgroup_getStats( e );

    LOG_SG_ENT( e, LOG_LEVEL_IMPORTANT, 0, "RankName %d is now '%s'", *(int*)rank, (char*)name);
}

void sgroup_genSetMOTD(Entity *e, void* msg, void* notused)
{
    strncpyt(e->supergroup->msg, (char*)msg, sizeof(e->supergroup->msg));
    LOG_SG_ENT( e, LOG_LEVEL_IMPORTANT, 0, "MOTD %s", escapeLogString((char*)msg));
}

void sgroup_genSetMotto(Entity *e, void* msg, void* notused)
{
    strncpyt(e->supergroup->motto, (char*)msg, sizeof(e->supergroup->motto));
    LOG_SG_ENT( e, LOG_LEVEL_IMPORTANT, 0, "Motto %s", escapeLogString(msg));
}

void sgroup_genSetDescription(Entity* e, void* str, void* notused)
{
    strncpyt(e->supergroup->description, (char*)str, sizeof(e->supergroup->description));
    LOG_SG_ENT( e, LOG_LEVEL_IMPORTANT, 0, "Description %s", escapeLogString((char*)str));
}

void sgroup_genSetTithe(Entity* e, void* tithe, void* notused)
{
    int t = sgroup_TitheClamp(*(int*)tithe);
    e->supergroup->tithe = t;
    LOG_SG_ENT( e, LOG_LEVEL_IMPORTANT, 0, "Tithe %i (was %i)", t, *(int*)tithe);
}

void sgroup_genSetDemoteTimeout(Entity* e, void* demote, void* notused)
{
    int d = sgroup_DemoteClamp(*(int*)demote);
    e->supergroup->demoteTimeout = d;
    LOG_SG_ENT( e, LOG_LEVEL_IMPORTANT, 0, "DemoteTimeout %i (was %i)", d, *(int*)demote);
}

void sgroup_genSetRankName(Entity* e, void* rank, void* name)
{
    strncpyt(e->supergroup->rankList[*(int*)rank].name, (char*)name, SG_TITLE_LEN);
}

// warning: this only uses the entity for finding the supergroup, it sets the rank of dbid
void sgroup_genSetRank(Entity* e, void* dbid, void* newrank)
{
    int i;
    int found;

    found = 0;
    for(i = eaSize(&e->supergroup->memberranks) - 1; i >= 0; i--)
    {
        if(e->supergroup->memberranks[i]->dbid == *(int*)dbid)
        {
            LOG_SG_ENT( e, LOG_LEVEL_IMPORTANT, 0, "Rank %i (was %i)", *(int*)newrank, e->supergroup->memberranks[i]->rank);
            e->supergroup->memberranks[i]->rank = *(int*)newrank;
            found = 1;
            break;
        }
    }

    if (found)
    {
        if (*(int*)newrank == NUM_SG_RANKS - 2)
        {
            for(i = eaSize(&e->supergroup->memberranks) - 1; i >= 0; i--)
            {
                if (e->supergroup->memberranks[i]->dbid != *(int*)dbid && 
                    e->supergroup->memberranks[i]->rank == NUM_SG_RANKS - 2)
                {
                    LOG_SG_ENT( e, LOG_LEVEL_IMPORTANT, 0, "Rank supreme leader self demotion: %i (was %i)", NUM_SG_RANKS - 3, e->supergroup->memberranks[i]->rank);
                    e->supergroup->memberranks[i]->rank = NUM_SG_RANKS - 3;
                }
            }
        }
    }
    else
    {
        devassertmsg(0, "tried to set someone's rank who wasn't in the memberranks list!!");
    }
}

// Handle autodemotion of a SG supreme leader going dark.
void sgroup_genSetIsDark(Entity* e, void* index, void* unUsed)
{
    int i;

    // Be silent!
    (void) unUsed;

    // If any of these tests ever fail, there's some really weird mojo going on.
    if (e == NULL || e->pl == NULL || e->supergroup == NULL || index == NULL)
    {
        return;
    }

    i = *(int *) index;
    // If this test ever fails something really wacky is going on.
    if (e->supergroup->memberranks[i]->dbid != e->db_id)
    {
        // Somehow the index got broken in transit.  Search again to find the guy
        for (i = eaSize(&e->supergroup->memberranks) - 1; i >= 0; i--)
        {
            if (e->supergroup->memberranks[i]->dbid == e->db_id)
            {
                break;
            }
        }
        if (i < 0)
        {
            // This should never happen.  That said ... Oh crap.  We can't find this guy.
            // Do nothing.  In the worst case, this will leave us with a SG that has a superleader that's
            // gone dark.  There's two ways to handle that.
            // 1. Add some code to the SG fixup that handles autodemoting based on offline duration, and
            // add a check there.  That's somewhere in this file ...
            // 2. Let CS clean the mess up.
            return;
        }
    }
    if (e->pl->playerType != e->supergroup->playerType && e->supergroup->memberranks[i]->rank == NUM_SG_RANKS - 2)
    {
        e->supergroup->memberranks[i]->rank = NUM_SG_RANKS - 3;
        LOG_SG_ENT( e, LOG_LEVEL_IMPORTANT, 0, "IsDark Autodemoting supreme leader because they went dark");
    }
}

void sgroup_genConvertRanks(Entity* e, void* notused, void* notused2)
{
    int i, j, k;

    LOG_SG_ENT( e, LOG_LEVEL_IMPORTANT, 0, "ConvertRanks Converting ranks");
    strncpyt(e->supergroup->rankList[0].name, e->supergroup->memberTitle, SG_TITLE_LEN);
    strncpyt(e->supergroup->rankList[1].name, localizedPrintf(e,"defaultLieutenantTitle"), SG_TITLE_LEN);
    strncpyt(e->supergroup->rankList[2].name, e->supergroup->captainTitle, SG_TITLE_LEN);
    strncpyt(e->supergroup->rankList[3].name, localizedPrintf(e,"defaultCommanderTitle"), SG_TITLE_LEN);
    strncpyt(e->supergroup->rankList[4].name, e->supergroup->leaderTitle, SG_TITLE_LEN);
    strncpyt(e->supergroup->rankList[5].name, localizedPrintf(e,"defaultSuperLeaderTitle"), SG_TITLE_LEN);

    sgroup_setDefaultPermissions(e->supergroup);

    for(i = 4; i >= e->supergroup->alliance_minTalkRank; i--)
        SETB(e->supergroup->rankList[i].permissionInt, SG_PERM_ALLIANCECHAT);

    for(i = eaSize(&e->sgStats)-1; i >= 0; i--)
    {
        for(j = e->supergroup->members.count - 1; j>= 0; j--)
        {
            if(e->supergroup->members.ids[j] == e->sgStats[i]->db_id)
            {
                SupergroupMemberInfo* member = NULL;
                int rank;

                if(e->sgStats[i]->rank <= 0)
                    rank = 0;
                else if(e->sgStats[i]->rank >= 2)
                    rank = NUM_SG_RANKS_VISIBLE - 2;    // the old check for ranks was "rank > 2 == leader" so let's stick with that i guess
                                                        // set this guy to a leader, but not a superleader, so we don't wind up with two superleaders.
                                                        // The autopromote code will take care of assigning a single superleader
                else
                    rank = 2;

                for(k = 0; !member && k < eaSize(&e->supergroup->memberranks); k++)
                {
                    if(e->supergroup->memberranks[k]->dbid == e->sgStats[i]->db_id)
                        member = e->supergroup->memberranks[k];
                }

                if(!member)
                {
                    member = createSupergroupMemberInfo();
                    member->dbid = e->sgStats[i]->db_id;
                    eaPush(&e->supergroup->memberranks, member);
                }

                member->rank = rank;
                break;
            }
        }
    }

    checkSgrpMembers(e, "genConvertRanks");
}

// this function should only get called when a new int's worth of permissions gets added, i.e. probably never
void sgroup_genFixLeaderPermissions(Entity* e, void* notused, void* notused2)
{
    memset(e->supergroup->rankList[NUM_SG_RANKS-1].permissionInt, 0xff, sizeof(e->supergroup->rankList[NUM_SG_RANKS-1].permissionInt));
    memset(e->supergroup->rankList[NUM_SG_RANKS-2].permissionInt, 0xff, sizeof(e->supergroup->rankList[NUM_SG_RANKS-2].permissionInt));
    LOG_SG_ENT( e, LOG_LEVEL_IMPORTANT, 0, "FixLeaderPermissions Fixing leader's permissions");
}

// this function should get called once only per SG, the first time it's loaded after adding the new ranks
void sgroup_genFixRankNames(Entity* e, void* notused, void* notused2)
{
    int i;

    // Do a little cleanup for SG's that are being handled for the first time after adding the new ranks
    for (i = 0; i < NUM_SG_RANKS; i++)
    {
        if (e->supergroup->rankList[i].name[0] == 0)
        {
            char *title = localizedPrintf(e, rankNames[i]);
            strncpy(e->supergroup->rankList[i].name, title, sizeof(e->supergroup->rankList[i].name));
        }
    }
    LOG_SG_ENT( e, LOG_LEVEL_IMPORTANT, 0, "FixRankNames");
}

void sgroup_genLockSGPlayerType(Entity* e, void* notused, void* notused2)
{
    e->supergroup->playerType = e->pl->playerType;
    e->supergroup->flags |= SG_FLAGS_LOCKED_PLAYERTYPE;
}

void sgroup_SetMode( Entity* player, int mode, int forceMode )
{
    int changed = 0;
    U32 timeCur;

    // Player not in supergroup?
    if( !player->supergroup_id )
    {
        // Non-SG players should never already be in SG mode.
        verify(player->pl->supergroup_mode == 0);

        if ( mode )
        {
            chatSendToPlayer( player->db_id, localizedPrintf(player,"CantEnterSuperMode"), INFO_USER_ERROR, 0 );
        }

        return;
    }

    if (mode && sgroup_isPlayerDark(player))
    {
        // Only send this message of the player is actually trying to enter SG mode.  This routine is called a couple of times when zoning as part of the fixup
        // code.  In the case when the player is dark, and hence not in SG mode, and the request is to exit SG mode, it'd still come through here and complain
        // at the player.  By only allowing this if mode is true, it kills the spam.
        chatSendToPlayer( player->db_id, localizedPrintf(player, ENT_IS_VILLAIN(player) ? "CantEnterSGModeVillain" : "CantEnterSGModeHero"), INFO_USER_ERROR, 0 );
        mode = 0;
    }

    // Player already in requested supergroup mode?
    mode = (mode ? 1 : 0);
    changed = player->pl->supergroup_mode != mode;
    if(!forceMode && !changed)
    {
        return;
    }

    // Update the time spent in SG mode, even if we didn't change modes
    timeCur = dbSecondsSince2000();
    if(player->pl->supergroup_mode && player->pl->lastSGModeChange)
    {
        player->pl->timeInSGMode += MAX(0,timeCur - player->pl->lastSGModeChange);
    }
    player->pl->lastSGModeChange = timeCur;

    // Change mode flag.
    player->pl->supergroup_mode  = mode;

    // Switch costume.
    if( player->pl->npc_costume == 0 )
    {
        if( player->pl->supergroup_mode )
            player->costume = costume_as_const(costume_makeSupergroup(player));
        else
            player->costume = costume_as_const(player->pl->costume[player->pl->current_costume]);
    }

    if( changed )
    {
        player->send_costume = TRUE;
        LOG_SG_ENT( player, LOG_LEVEL_VERBOSE, 0, "Mode %s %d", player->pl->supergroup_mode ? "On" : "Off", player->pl->timeInSGMode);
    }
}

// player is toggling into or out of supergroup mode
void sgroup_ToggleMode( Entity* player )
{
    if( !player->supergroup_id && !player->pl->supergroup_mode )
        return;

    sgroup_SetMode(player, !player->pl->supergroup_mode, 0);
}


// Find all group members on this mapserver and send them a supergroup update.
void sgroup_UpdateAllMembers(Supergroup* group)
{
    int i;
    if(!group)
        return;

    for(i = 0; i < group->members.count; i++)
    {
        int dbid = group->members.ids[i];
        char buf[32];
        Entity* e = entFromDbId(dbid);

        sprintf( buf, "sgstatsrelay %i", dbid);
        serverParseClient( buf, NULL );

        if(e)
            e->supergroup_update = 1;
    }

}

// Find all group members on any mapserver and send them a supergroup update.
void sgroup_UpdateAllMembersAnywhere(Supergroup* group)
{
    int i;
    if(!group)
        return;

    for(i = 0; i < group->members.count; i++)
    {
        int dbid = group->members.ids[i];
        char buf[32];
        sprintf( buf, "sg_updatemember %i", dbid);
        serverParseClient( buf, NULL );
    }
}


static Supergroup * s_sgroupForReceivecontainers = NULL;
static int sgroup_ReceiveContainers(Packet *pak,int cmd,NetLink *link)
{
    int                list_id,count;
    ContainerInfo    ci;

    list_id            = pktGetBitsPack(pak,1);
    count            = pktGetBitsPack(pak,1);

    assert(count == 1);

    if (!dbReadContainer(pak,&ci,list_id))
        return false;

    if(verify( s_sgroupForReceivecontainers ))
    {
        clearSupergroup(s_sgroupForReceivecontainers);
        sgroup_DbContainerUnpack( s_sgroupForReceivecontainers, ci.data );
    }
    return true;
}

//----------------------------------------
// fills the dummy supergroup with supergroup fields
//----------------------------------------
bool sgroup_LockAndLoad(int sgid, Supergroup *sg)
{
    bool res = false;

    if(verify( sg ))
    {
        s_sgroupForReceivecontainers = sg;
        res = dbSyncContainerRequestCustom(CONTAINER_SUPERGROUPS, sgid, CONTAINER_CMD_LOCK_AND_LOAD, sgroup_ReceiveContainers);

        LOG_OLD( "group_LockAndLoad(%s)", res ? "success" : "failure" );
    }

    // ----------
    // finally

    return res;
}

//----------------------------------------
// unlocks and updates the dummy supergroup, and frees any fields attached to it
// DO NOT PASS A 'REAL' SUPERGROUP TO THIS, JUST THE ONE FILLED BY 'sgroup_LockAndLoad'
//----------------------------------------
void sgroup_UpdateUnlock(int sgid, Supergroup *sg)
{
    if(verify( sg ))
    {
        // ----------
        // update the container

        char *container_str = packageSuperGroup(sg);
        bool ret = dbSyncContainerUpdate(CONTAINER_SUPERGROUPS,sgid,CONTAINER_CMD_UNLOCK,container_str);
        free(container_str);

        // ----------
        // clear the supergroup

        clearSupergroup(sg);

        // ----------
        // log

        LOG_OLD( "sgroup_UpdateUnlock(%s)", ret ? "success" : "failed" );
    }
}

char *sgroup_FindLongestWithPermission(Entity* ent, sgroupPermissionEnum permission)
{
    int minrank = 0;
    int i, count = 0;
    SupergroupStats* stats;

    // find out what rank is required for this permission
    while(!sgroup_rankHasPermission(ent, minrank, permission))
        minrank++;

    // find a player who is online and has permission
    count = eaSize(&ent->supergroup->memberranks);
    for (i = 0; i < count; i++)
    {
        Entity * e;
        OnlinePlayerInfo *opi;
        if (ent->supergroup->memberranks[i]->rank < minrank)
            continue;

        stats = sgroup_memberStats(ent, ent->supergroup->memberranks[i]->dbid);
        if (!stats) 
            continue;  

        e = entFromDbId(stats->db_id);
        opi = dbGetOnlinePlayerInfo(stats->db_id);
        if (!e && !opi && !stats->online)
            continue;
        return stats->name;
    }
    return NULL;
}

void sgroup_RefreshClient(Entity *e)
{
    e->supergroup_update = 1;
}

void sgroup_RefreshSGMembers(int sg_dbid)
{
    sendToAllGroupMembers(sg_dbid, CONTAINER_SUPERGROUPS, "cmdrelay\nsgrefreshrelay");
}

typedef struct SgroupInfo
{
    char    name[SG_NAME_LEN];
    char    description[SG_DESCRIPTION_LEN];
    int        prestige;
    int        sgid;
    int        member_count;
} SgroupInfo;

SgroupInfo **cachedSgroupInfoAll;
SgroupInfo **cachedSgroupInfoPrestige=0;
SgroupInfo **cachedSgroupInfoMemberCount=0;

static SgroupInfo * findSgroupInfoById( int sgid )
{
    int i;
    for( i=eaSize(&cachedSgroupInfoAll)-1; i>= 0; i-- )
    {
        if( cachedSgroupInfoAll[i]->sgid == sgid )
            return cachedSgroupInfoAll[i];
    }
    return NULL;
}

static SgroupInfo * findSgroupInfoByName( char * name )
{
    int i;
    for( i=eaSize(&cachedSgroupInfoAll)-1; i>= 0; i-- )
    {
        if( stricmp(cachedSgroupInfoAll[i]->name, name) == 0 )
            return cachedSgroupInfoAll[i];
    }
    return NULL;
}

static void sgroup_SendRegistration(Entity *e, int sort_by_prestige)
{
    int i,count = eaSize(&cachedSgroupInfoPrestige);

    START_PACKET(pak, e, SERVER_SUPERGROUP_LIST);

    pktSendBitsPack(pak, 1, count);
    for (i = 0; i < count; i++)
    {
        pktSendBitsPack(pak, 1, cachedSgroupInfoPrestige[i]->sgid);
        pktSendString(pak, cachedSgroupInfoPrestige[i]->name);
        pktSendString(pak, cachedSgroupInfoPrestige[i]->description);
        pktSendBitsPack(pak, 1, cachedSgroupInfoPrestige[i]->prestige);
        //pktSendBitsPack(pak, 1, cachedSgroupInfo[i]->member_count);
    }

    END_PACKET;
}


static void sgroup_readRegistration(SgroupInfo ***cachedSgroupInfo, Packet *in_pak,int db_id,int count, int sort_by_prestige)
{
    int i, sgid;
    Entity *e = entFromDbIdEvenSleeping( db_id );

    if( !e )
        return;

    if( !cachedSgroupInfoAll )
        eaCreate(&cachedSgroupInfoAll);

    for (i = 0; i < count; i++)
    {
        SgroupInfo *newInfo = 0;
        int add_to_all = 0;

        sgid = pktGetBitsPack(in_pak, 1);
        newInfo = findSgroupInfoById(sgid);

        if( !newInfo ) // if we didn't find an older entry to update, we make a new one
        {
            newInfo = calloc(1,sizeof(SgroupInfo));
            add_to_all = true;
        }

        strncpyt( newInfo->name, pktGetString(in_pak), SG_NAME_LEN);
        strncpyt( newInfo->description, pktGetString(in_pak), SG_DESCRIPTION_LEN);

        newInfo->prestige = pktGetBitsPack(in_pak, 1);    // Fluid prestige
        newInfo->prestige += pktGetBitsPack(in_pak, 1); // Base Value
        newInfo->sgid = sgid;

        if( add_to_all )
            eaPush(&cachedSgroupInfoAll, newInfo);
         if( cachedSgroupInfo ) // This is null if it was specific req
            eaPush(cachedSgroupInfo, newInfo);
    }

    if( count )
        sgroup_SendRegistration(e,sort_by_prestige);
}

// These three fuctions because the callback is standard and needed a way to differentiate the results
static void sgroup_readRegistrationPrestige(Packet *in_pak,int db_id,int count)
{
    if( !cachedSgroupInfoPrestige)
        eaCreate(&cachedSgroupInfoPrestige);
    eaSetSize(&cachedSgroupInfoPrestige,0); // Not freed because results are held in another list that is persistant

    sgroup_readRegistration(&cachedSgroupInfoPrestige, in_pak, db_id, count, 1);
}

static void sgroup_readRegistrationMembers(Packet *in_pak,int db_id,int count)
{
    if( !cachedSgroupInfoMemberCount)
        eaCreate(&cachedSgroupInfoMemberCount);

    eaSetSize(&cachedSgroupInfoMemberCount,0); // Not freed because results are held in another list that is persistant

    sgroup_readRegistration(&cachedSgroupInfoMemberCount, in_pak, db_id, count, 0);
}

static void sgroup_readRegistrationSpecific(Packet *in_pak,int db_id,int count)
{
    Entity *e = entFromDbIdEvenSleeping( db_id );
    int i;

    if( !e )
        return;

    START_PACKET(pak, e, SERVER_SUPERGROUP_LIST);
    pktSendBitsPack(pak, 1, count);
    for (i = 0; i < count; i++)
    {
        pktSendBitsPack(pak, 1, pktGetBitsPack(in_pak, 1) );
        pktSendString(pak, pktGetString(in_pak));
        pktSendString(pak, pktGetString(in_pak));
        pktSendBitsPack(pak, 1, pktGetBitsPack(in_pak, 1)+pktGetBitsPack(in_pak, 1) );
        //pktSendBitsPack(pak, 1, cachedSgroupInfo[i]->member_count);
    }
    END_PACKET
}


void sgroup_sendRegistrationList(Entity* e, int sort_by_prestige, char *name)
{
    static int last_request_time = 0;
    /* This is the command I want to run if I can ever figure out how to grab it from SQL

        SELECT TOP 100 
            Supergroups.ContainerId, 
            Supergroups.name, 
            Supergroups.description, 
            (isNull(Supergroups.prestige,0)+isNull(Supergroups.prestigeBase)) AS Prestige, 
            COUNT(1) AS MemberCount
        FROM
            Supergroups, Ents
        WHERE
            Supergroups.ContainerId = Ents.SupergroupsId 
        GROUP BY 
            Supergroups.name, 
            Supergroups.ContainerId, 
            Supergroups.descroption, 
            Supergroups.prestige, 
            Supergroups.prestigeBase 
        ORDER BY 
            MemberCount DESC
    */

    if( name && name[0] )
    {
        strchrReplace( name, '\'', '\0' );
        if( name[0] )
        {
            char specific_query[1000];

            stripNonRegionChars(name, getCurrentLocale() );
            sprintf( specific_query, "WHERE Name like '%%%s%%'", name );
            dbReqCustomData(CONTAINER_SUPERGROUPS,"Supergroups","Top 100",specific_query,
                "ContainerId, Name, Description, Prestige, PrestigeBase", sgroup_readRegistrationSpecific, e->db_id);
        }
    }

     if( dbSecondsSince2000() - last_request_time > 600 || !cachedSgroupInfoAll )
    {
        dbReqCustomData(CONTAINER_SUPERGROUPS,"Supergroups","Top 100","ORDER BY (isNull(Supergroups.prestige,0)+isNull(Supergroups.PrestigeBase,0)) DESC",
                "ContainerId, Name, Description, Prestige, PrestigeBase", sgroup_readRegistrationPrestige, e->db_id);

        last_request_time =  dbSecondsSince2000() ;
    }
    else
        sgroup_SendRegistration(e,sort_by_prestige);
}

//------------------------------------------------------------
//  returns index of added token, -1 if fail
//----------------------------------------------------------
int entity_AddSupergroupRewardToken(Entity *e, const char *tokenName)
{
    int res = -1;

    if( verify( e && e->supergroup && tokenName ))
    {
        // first see if its in supergroup
        // then lock
        // then make sure its still in the supergroup
        if(verify( teamLock( e, CONTAINER_SUPERGROUPS )))
        {
            res = rewardtoken_Award(&e->supergroup->rewardTokens, tokenName, e->db_id );
            teamUpdateUnlock( e, CONTAINER_SUPERGROUPS );
            e->supergroup_update = 1;
        }
    }

    // --------------------
    // finally

    return res;
}

//------------------------------------------------------------
//  remove token from entity
//----------------------------------------------------------
bool entity_RemoveSupergroupRewardToken(Entity *e, const char *tokenName)
{
    bool res = false;

    if( verify(e && e->supergroup && tokenName)
        && rewardtoken_IdxFromName( &e->supergroup->rewardTokens, tokenName ) >= 0
        && verify( teamLock( e, CONTAINER_SUPERGROUPS )))

    {
        res = rewardtoken_Unaward(&e->supergroup->rewardTokens, tokenName );

        // finally, unlock
        teamUpdateUnlock( e, CONTAINER_SUPERGROUPS );
    }

    // --------------------
    // finally

    return res;
}

/////////////////////////////////////////////////////////////////////////////////////
// returns false if not modified, true if completed successfully
int entity_AddSupergroupRaidPoints(Entity *e, int Points, int bFromRaid)
{
    int res = false;
    int maxPoints = 0;
    char *pTimeTokenName = NULL;
    RewardToken *pTimeToken = NULL;
    RewardToken *pToken = getRewardToken(e, RAID_POINT_TOKEN);

    if ( verify( e && e->supergroup ) )
    {
        if (Points > 0)
        {
            // check to see if team can still get more 
            if (bFromRaid)
            {
                pTimeTokenName = RAID_POINT_RAID_TOKEN;
                maxPoints = RAID_POINT_MAX_RAID;
            } else {
                pTimeTokenName = RAID_POINT_COP_TOKEN;
                maxPoints = RAID_POINT_MAX_COP;
            }

            pTimeToken = getRewardToken(e, pTimeTokenName);

            if (pTimeToken && pTimeToken->timer + RAID_POINT_MAX_TIME > timerSecondsSince2000())
            {
                Points = MIN(maxPoints - pTimeToken->val, Points);
            } else {
                Points = MIN(maxPoints, Points);
            }
            Points = MAX(0, Points);
        } else {
            // make sure we don't end up with negative points
            if (!pToken)
            {
                Points = 0;
            } else {
                if (pToken->val + Points < 0)
                {
                    Points = -pToken->val;
                }
            }
            Points = MIN(0, Points);
        }

        // do we need to modify it?
        if(Points != 0 && verify( teamLock( e, CONTAINER_SUPERGROUPS )))
        {
            pToken = getRewardToken(e, RAID_POINT_TOKEN);
            pTimeToken = getRewardToken(e, pTimeTokenName);

            // modify the point count
            if (pToken)
            {
                pToken->val += Points;
                pToken->timer = timerSecondsSince2000();
                res = true;
            } 
            else if ( Points > 0) 
            {
                res = (rewardtoken_Award(&e->supergroup->rewardTokens, RAID_POINT_TOKEN, Points ) >=0 );                    
            }

            // modify timer if needed
            if (Points > 0)
            {
                if (!pTimeToken)
                {
                    rewardtoken_Award(&e->supergroup->rewardTokens, pTimeTokenName, Points );
                } else {
                    if (pTimeToken->timer + RAID_POINT_MAX_TIME > timerSecondsSince2000())
                    {
                        pTimeToken->val += Points;
                    } else {
                        pTimeToken->val = Points;
                        pTimeToken->timer = timerSecondsSince2000();
                    }
                }
            }

            teamUpdateUnlock( e, CONTAINER_SUPERGROUPS );
            e->supergroup_update = 1;
        }
    }

    // --------------------
    // finally

    return res;
}


/////////////////////////////////////////////////////////////////////////////////////
void entity_GetSupergroupRaidPoints(Entity *e, int *Points, int *raidPoints, int *raidTime, int *copPoints, int *copTime)
{
    if ( verify( e && e->supergroup ) )
    {
        RewardToken *pToken = getRewardToken(e, RAID_POINT_TOKEN);
        RewardToken *pRaidTimeToken = getRewardToken(e, RAID_POINT_RAID_TOKEN);
        RewardToken *pCopTimeToken = getRewardToken(e, RAID_POINT_COP_TOKEN);

        if (pToken && Points)
        {
            *Points = pToken->val;
        }

        if (pRaidTimeToken)
        {
            int expired = (pRaidTimeToken->timer + RAID_POINT_MAX_TIME <= timerSecondsSince2000());
            if (raidPoints)
            {
                if (expired)
                {
                    *raidPoints = 0;
                } else {
                    *raidPoints = pRaidTimeToken->val;
                }
            }
            if (raidTime)
            {
                if (expired)
                {
                    *raidTime = 0;
                } else {
                    *raidTime = pRaidTimeToken->timer + RAID_POINT_MAX_TIME - timerSecondsSince2000();
                }
            }
        }

        if (pCopTimeToken)
        {
            int expired = (pCopTimeToken->timer + RAID_POINT_MAX_TIME <= timerSecondsSince2000());
            if (copPoints)
            {
                if (expired)
                {
                    *copPoints = 0;
                } else {
                    *copPoints = pCopTimeToken->val;
                }
            }
            if (copTime)
            {
                if (expired)
                {
                    *copTime = 0;
                } else {
                    *copTime = pCopTimeToken->timer + RAID_POINT_MAX_TIME - timerSecondsSince2000();
                }
            }
        }
    }
}



/**********************************************************************func*
 * FindSpecialDetail
 *
 */
SpecialDetail *FindSpecialDetail(Supergroup *supergroup, const Detail *pDetail, U32 timeCreation)
{
    int i;

    for(i=0; i<eaSize(&supergroup->specialDetails); i++)
    {
        if(supergroup->specialDetails[i] != NULL
            && supergroup->specialDetails[i]->pDetail == pDetail
            && supergroup->specialDetails[i]->creation_time == timeCreation)
        {
            return supergroup->specialDetails[i];
        }
    }

    return NULL;
}

/**********************************************************************func*
* FindSpecialDetailByName
*
*/
SpecialDetail *FindSpecialDetailByName(Supergroup *supergroup, const char *pchName)
{
    int i;

    if (!supergroup)
        return NULL;

    for(i=0; i<eaSize(&supergroup->specialDetails); i++)
    {
        if(supergroup->specialDetails[i] != NULL
            && supergroup->specialDetails[i]->pDetail != NULL
            && stricmp(supergroup->specialDetails[i]->pDetail->pchName, pchName) == 0)
        {
            return supergroup->specialDetails[i];
        }
    }

    return NULL;
}

/**********************************************************************func*
 * AddSpecialDetail
 *
 */
SpecialDetail *AddSpecialDetail(Supergroup *supergroup, const Detail *pDetail, U32 timeCreation, U32 timeExpiration, U32 iFlags)
{
    int i;
    SpecialDetail *pNew = NULL;
    int iSize = eaSize(&supergroup->specialDetails);

    for(i=0; i<iSize; i++)
    {
        if(supergroup->specialDetails[i] == NULL)
        {
            pNew = supergroup->specialDetails[i] = CreateSpecialDetail();
            break;
        }
        else if(supergroup->specialDetails[i]->pDetail == NULL)
        {
            pNew = supergroup->specialDetails[i];
            break;
        }
    }
    if(i==iSize)
    {
        pNew = CreateSpecialDetail();
        eaPush(&supergroup->specialDetails, pNew);
    }

    pNew->pDetail = pDetail;
    pNew->creation_time = timeCreation;
    pNew->expiration_time = timeExpiration;
    pNew->iFlags = iFlags;

    return pNew;
}


/**********************************************************************func*
 * sgroup_AddSpecialDetail
 *
 */
void sgroup_AddSpecialDetail(Entity *e, const Detail *pDetail, U32 timeCreation, U32 timeExpiration, U32 iFlags)
{
    if(verify(e && e->supergroup && pDetail) )
    {
        if( verify(teamLock(e, CONTAINER_SUPERGROUPS)))
        {
            AddSpecialDetail(e->supergroup, pDetail, timeCreation, timeExpiration, iFlags);

            teamUpdateUnlock( e, CONTAINER_SUPERGROUPS );
        }
    }
}

/**********************************************************************func*
 * RemoveSpecialDetail
 *
 */
void RemoveSpecialDetail(Supergroup *supergroup, const Detail *pDetail, U32 timeCreation)
{
    int i;
    for(i=0; i<eaSize(&supergroup->specialDetails); i++)
    {
        if(supergroup->specialDetails[i] != NULL
            && supergroup->specialDetails[i]->pDetail == pDetail
            && supergroup->specialDetails[i]->creation_time == timeCreation)
        {
            DestroySpecialDetail(supergroup->specialDetails[i]);
            supergroup->specialDetails[i] = NULL;
        }
    }
}

/**********************************************************************func*
 * sgroup_RemoveSpecialDetail
 *
 */
void sgroup_RemoveSpecialDetail(Entity *e, const Detail *pDetail, U32 timeCreation)
{
    if(verify(e && e->supergroup && pDetail) )
    {
        if( verify(teamLock(e, CONTAINER_SUPERGROUPS)))
        {
            RemoveSpecialDetail(e->supergroup, pDetail, timeCreation);

            teamUpdateUnlock( e, CONTAINER_SUPERGROUPS );
        }
    }
}

/**********************************************************************func*
* ExtendSpecialDetail
*
*/
void ExtendSpecialDetail(Supergroup *supergroup, const Detail *pDetail, U32 timeCreation, U32 extendAdditionalTime)
{
    int i;
    for(i=0; i<eaSize(&supergroup->specialDetails); i++)
    {
        if(supergroup->specialDetails[i] != NULL
            && supergroup->specialDetails[i]->pDetail == pDetail
            && supergroup->specialDetails[i]->creation_time == timeCreation)
        {
            supergroup->specialDetails[i]->expiration_time += extendAdditionalTime;
        }
    }
}

/**********************************************************************func*
* sgroup_RemoveSpecialDetail
*
*/
void sgroup_ExtendSpecialDetail(Entity *e, const Detail *pDetail, U32 timeCreation, U32 extendAdditionalTime)
{
    if(verify(e && e->supergroup && pDetail) )
    {
        if( verify(teamLock(e, CONTAINER_SUPERGROUPS)))
        {
            ExtendSpecialDetail(e->supergroup, pDetail, timeCreation, extendAdditionalTime);

            teamUpdateUnlock( e, CONTAINER_SUPERGROUPS );
        }
    }
}

/**********************************************************************func*
* sgroup_AddSpecialDetail
*
*/
void sgroup_ExchangeIoPs(Entity *e, const Detail *pOldDetail, U32 timeOldCreation, const Detail *pNewDetail, U32 timeNewCreation, U32 timeExpiration, U32 iFlags)
{
    if(verify(e && e->supergroup && pOldDetail && pNewDetail) )
    {
        if( verify(teamLock(e, CONTAINER_SUPERGROUPS)))
        {
            if (ItemOfPowerFind(e->supergroup, pOldDetail->pchName) != NULL &&
                ItemOfPowerFind(e->supergroup, pNewDetail->pchName) == NULL)
            {
                RemoveSpecialDetail(e->supergroup, pOldDetail, timeOldCreation);
                AddSpecialDetail(e->supergroup, pNewDetail, timeNewCreation, timeExpiration, iFlags);
            }
    
            teamUpdateUnlock( e, CONTAINER_SUPERGROUPS );
        }
    }
}


/**********************************************************************func*
 * sgroup_ValidateSpecialDetails
 *
 */
void sgroup_ValidateSpecialDetails(Base *pBase, Supergroup *sg)
{
    Supergroup sgroup = {0};
    int raidServerSynchNeeded = 0;
    int i, update = false;

    if( !sg )
    {
        sg = &sgroup;
        update = true;
        if(!verify(sgroup_LockAndLoad(pBase->supergroup_id, sg)))
            return;
    }

    // Mark all is invalid
    for(i=0; i<eaSize(&sg->specialDetails); i++)
    {
        if(sg->specialDetails[i] != NULL)
        {
            sg->specialDetails[i]->bValid = false;
        }
    }

    // Go through everything in the base and find or add the special
    //   details to the list.
    for(i=0; i<eaSize(&pBase->rooms); i++)
    {
        int iDetail;
        for(iDetail=0; iDetail<eaSize(&pBase->rooms[i]->details); iDetail++)
        {
            RoomDetail *pRoomDetail = pBase->rooms[i]->details[iDetail];

            if(pRoomDetail->info->bSpecial)
            {
                bool isActivated = true;
                SpecialDetail *p;

                if(roomDetailLookUnpowered(pRoomDetail))
                {
                    isActivated = false; //it's not powered or controlled
                }

                if(pRoomDetail->info->eFunction == kDetailFunction_AuxTeleBeacon
                    && !pRoomDetail->iAuxAttachedID)
                {
                    isActivated = false; //it needs to be attached to function, but isn't
                }

                p = FindSpecialDetail(sg, pRoomDetail->info, pRoomDetail->creation_time);
                if(p)
                {
                    p->bValid = true;
                }
                else
                {
                    // Not in the list!
                    p = AddSpecialDetail(sg, pRoomDetail->info, pRoomDetail->creation_time, 0, DETAIL_FLAGS_IN_BASE);
                    p->bValid = true;
                    if( p->pDetail->eFunction== kDetailFunction_ItemOfPower )
                    {
                        p->iFlags |= DETAIL_FLAGS_NEW_IOP_RAIDSERVER_NEEDS_TO_KNOW_ABOUT;
                        p->iFlags |= DETAIL_FLAGS_RAIDSERVER_OWNED_ITEM_OF_POWER;
                        p->expiration_time = 604800; // One week in seconds.  This shouldn't really be happening in production.
                        raidServerSynchNeeded = 1;
                    }
                    LOG_SG(sg->name, pBase->supergroup_id, LOG_LEVEL_IMPORTANT, 0, "Special detail %s was added to special detail list", pRoomDetail->info->pchName);
                }
                p->iFlags |= DETAIL_FLAGS_IN_BASE;

                if (isActivated)
                    p->iFlags |= DETAIL_FLAGS_ACTIVATED;
                else
                    p->iFlags &= ~DETAIL_FLAGS_ACTIVATED;
            }
        }
    }

    // Anything still invalid is removed
    for(i=0; i<eaSize(&sg->specialDetails); i++)
    {
        SpecialDetail *sd = sg->specialDetails[i];

        if(sd != NULL
            && ((sd->iFlags & DETAIL_FLAGS_IN_BASE) || !sd->iFlags)
            && sd->pDetail != NULL
            && sd->bValid == false)
        {
            if( sd->pDetail->eFunction == kDetailFunction_ItemOfPower )
            {
                if( !(sd->iFlags & DETAIL_FLAGS_RAIDSERVER_REMOVE_THIS_ITEM_OF_POWER) )
                {
                    sd->iFlags &= ~DETAIL_FLAGS_IN_BASE;
                    sd->iFlags &= ~DETAIL_FLAGS_ACTIVATED;
                    sd->iFlags &= ~DETAIL_FLAGS_AUTOPLACE;
                    sd->iFlags |= DETAIL_FLAGS_RAIDSERVER_REMOVE_THIS_ITEM_OF_POWER;
                    raidServerSynchNeeded = 1;
                }
            }
            else
            {
                LOG_SG(sg->name, pBase->supergroup_id, LOG_LEVEL_IMPORTANT, 0, "Special detail %s was removed from special detail list",sd->pDetail->pchName);
                RemoveSpecialDetail(sg,sd->pDetail, sd->creation_time);

                i--;
            }
        }
    }

    // finally, unlock
    if(update)
        sgroup_UpdateUnlock(pBase->supergroup_id, sg);

    if( raidServerSynchNeeded )
        ItemOfPowerSGSynch( pBase->supergroup_id );
}

/**********************************************************************func*
 * sgroup_CalcSpacesForItemsOfPower
 *
 * returns 1 if the value changed
 */
int sgroup_CalcSpacesForItemsOfPower(Base *pBase, Supergroup *sg)
{
    int newSpaces;

    if(!pBase || !sg)
        return 0;

    newSpaces = baseSpacesForItemsOfPower(pBase);

    if( newSpaces != sg->spacesForItemsOfPower )
    {
        sg->spacesForItemsOfPower = newSpaces;
        return 1;
    }

    return 0;
}


/**********************************************************************func*
 * sgroup_SaveBase
 *
 */
void sgroup_SaveBase(Base *pBase, Supergroup *sg, int force_save)
{
    static int timer = 0;
    bool save = false;

    if(!timer || dbSecondsSince2000()-timer > 600)
    {
        save = true;
    }

    beaconRequestBeaconizing(db_state.map_name);

    verify(pBase && (pBase->supergroup_id || pBase->user_id));
    if((force_save || save || sg) && pBase && pBase->supergroup_id)
    {
        int idSgrp = pBase->supergroup_id;
        Supergroup sgroup = {0};

        if(!sg)
            sg = &sgroup;

        if(sg->name[0] || sgroup_LockAndLoad(idSgrp, sg))
        {
            sgroup_ValidateSpecialDetails(pBase, sg);
            sgroup_CalcSpacesForItemsOfPower(pBase, sg);

            if(OnSGBase() || db_state.local_server)
            {
                baseSaveMapToDb();
                applySGPrestigeSpent(sg);
                timer = dbSecondsSince2000();
            }

            if(pBase->plot)
            {
                if (!baseIsLegal(pBase,0))
                {
                    // If base isn't legal, say we have no mounts for raiding purposes
                    RaidSetBaseInfo(pBase->supergroup_id, pBase->plot->iMaxRaidParty, 0);
                }
                else
                {                
                    RaidSetBaseInfo(pBase->supergroup_id, pBase->plot->iMaxRaidParty, sgrp_emptyMounts(sg));
                }
            }

            if( idSgrp == pBase->supergroup_id )
                sgroup_UpdatePrestigeBaseAndUpkeep(sg, idSgrp, true);
            
            // finally, unlock
            if(sg == &sgroup)
                sgroup_UpdateUnlock(idSgrp, &sgroup);
        }
    }
    if((force_save || save) && pBase && pBase->user_id)
    {
        int spent = 0, cost, upkeep;

        if(OnSGBase() || db_state.local_server)
        {
            baseSaveMapToDb();
            spent = baseGetAndClearSpent();
            timer = dbSecondsSince2000();
        }

        baseCostAndUpkeep(&cost, &upkeep);
        // TODO: relay to the apt owner: e->pchar->iInfluenceApt = cost; applyEntInfluenceSpent(e, spent);
    }
}


/**********************************************************************func*
 * sgroup_CanPortToBase
 *
 */
bool sgroup_CanPortToBase(Entity *e)
{
    int i;
    Supergroup *sg;
    StaticMapInfo *info;
    char temp[100];

    if (!e || !e->pl || !e->supergroup)
    { //no base available
        return false;
    }

    info = staticMapInfoFind(db_state.base_map_id);
    if (!info)
    { //Not on a static map
        return false;
    }
    sg = e->supergroup;
    for (i = eaSize(&sg->specialDetails) - 1; i >= 0; i--)
    {
        if (sg->specialDetails[i] && sg->specialDetails[i]->pDetail &&
            sg->specialDetails[i]->iFlags & DETAIL_FLAGS_IN_BASE &&
            sg->specialDetails[i]->iFlags & DETAIL_FLAGS_ACTIVATED &&
            sg->specialDetails[i]->pDetail->eFunction == kDetailFunction_AuxTeleBeacon)
        {
            char *period;
            int j;
            const Detail *det = sg->specialDetails[i]->pDetail;
            for (j = 0; j < eaSize(&det->ppchFunctionParams); j++)
            {
                sprintf(temp,"/%s",det->ppchFunctionParams[j]);

                period = strchr(temp,'.');
                if (period) //ignore everything after map name
                    *period = '\0';

                if (strstr(info->name,temp))
                { //do we have a match to the current map?
                    return true;
                }
            }
        }
    }
    return false; // No matching aux items
}

/**********************************************************************func*
 * sgroup_XPBonus
 *
 * Checks for Items of Power which have an XPBonus. Sums up the first five
 * of them.
 *
 */
float sgroup_XPBonus(Entity *e)
{
    int i;
    Supergroup *sg;
    float fTotal = 0.0f;
    int iCnt = 0;

    if (!e || !e->pl || !e->supergroup)
        return 0.0f;

    sg = e->supergroup;
    for (i = eaSize(&sg->specialDetails) - 1; i >= 0 && iCnt<5; i--)
    {
        if (sg->specialDetails[i] && sg->specialDetails[i]->pDetail &&
            sg->specialDetails[i]->iFlags & DETAIL_FLAGS_IN_BASE
            && sg->specialDetails[i]->pDetail->eFunction == kDetailFunction_ItemOfPower
            && eaSize(&sg->specialDetails[i]->pDetail->ppchFunctionParams)>0
            && stricmp(sg->specialDetails[i]->pDetail->ppchFunctionParams[0], "XPBonus")==0)
        {
            chareval_JustEval(e->pchar, sg->specialDetails[i]->pDetail->ppchFunctionParams);
            fTotal += eval_FloatPeek(chareval_Context());
            iCnt++;
        }
    }

    return fTotal;
}

/**********************************************************************func*
 * sgrp_SetRecipe
 *
 * Forcibly sets the recipe inventory item to the given values. This can
 * be used to re-limit an unlimited recipe, for example, or set the number
 * of recipes to an exact value.
 *
 * If the supergroup doesn't yet have the recipe, it is added.
 *
 */
void sgrp_SetRecipe(Entity *e, DetailRecipe const *recipe, int count, bool isUnlimited)
{
    if(verify(e && recipe)
        && verify(teamLock(e, CONTAINER_SUPERGROUPS)))
    {
        sgrp_SetRecipeNoLock(e->supergroup, recipe, count, isUnlimited);
        teamUpdateUnlock(e, CONTAINER_SUPERGROUPS);
        e->supergroup_update = true;
    }
}

/**********************************************************************func*
 * sgrp_AdjRecipe
 *
 * Modifies an existing recipe inventory item (or adds one if it doesn't
 * exist yet). A negative number can be given as the count to decrease the
 * number of those recipes a supergroup has. If the number drops to zero,
 * it is removed from the list of recipes the SG has.
 *
 * If the adjustment sets isUnlimited to true, then the recipe is set to
 * never run out.
 *
 * Adding or deducting from an unlimited recipe has no effect. If a recipe is
 * unlimited, setting isUnlimited to false has no effect. Making an
 * unlimited recipe limited again can be done with sgrp_SetRecipe.
 *
 */
void sgrp_AdjRecipe(Entity *e, DetailRecipe const *recipe, int count, bool isUnlimited)
{
    if(verify(e && recipe)
        && verify(teamLock(e, CONTAINER_SUPERGROUPS)))
    {
        sgrp_AdjRecipeNoLock(e->supergroup, recipe, count, isUnlimited);
        teamUpdateUnlock(e, CONTAINER_SUPERGROUPS);
        e->supergroup_update = true;
    }
}


/**********************************************************************func*
 * sgroup_Tick
 *
 */
void sgroup_Tick(Entity *e)
{
    if(e && e->pchar)
    {
        // Make sure that they have all the right powers

        const BasePowerSet *psetBase = powerdict_GetBasePowerSetByFullName(&g_PowerDictionary, "Items_Of_Power.Items_Of_Power");
        PowerSet *pset = character_OwnsPowerSet(e->pchar, psetBase);
        int i;

        // check for supergroup special details expiring
        if (e->supergroup && e->supergroup_id != 0)
        {
            for (i = 0; i < eaSize(&e->supergroup->specialDetails); i++)
            {
                SpecialDetail *pSDetail = e->supergroup->specialDetails[i];

                if (pSDetail->pDetail != NULL && 
                    pSDetail->expiration_time > 0 && 
                    pSDetail->expiration_time < timerSecondsSince2000())
                {
                    // detail has expired
                    
                    // does it decay to something?
                    if (pSDetail->pDetail->pchDecaysTo != NULL && ItemOfPowerFind(e->supergroup, pSDetail->pDetail->pchDecaysTo) == NULL)
                    {                            
                        const Detail *pDetail = basedata_GetDetailByName(pSDetail->pDetail->pchDecaysTo);
                        int timeToDecay = 0;

                        if (pSDetail->pDetail->iDecayLife > 0)
                            timeToDecay = timerSecondsSince2000() + pSDetail->pDetail->iDecayLife;

                        if (pDetail)
                            sgroup_ExchangeIoPs(e, pSDetail->pDetail, pSDetail->creation_time, pDetail, timerSecondsSince2000(), timeToDecay, DETAIL_FLAGS_AUTOPLACE);

                    } else {
                        // remove IoP
                        sgroup_RemoveSpecialDetail(e, pSDetail->pDetail, pSDetail->creation_time);
                    }
                }
            }
        }

        if(e->supergroup && e->supergroup_id != 0 && e->pl->supergroup_mode)
        {
            int iSize;
            int iSizePows;
            Power **ppPowers = NULL;

            if(pset)
            {
                // Get a list of all the Item of Power powers they have now.
                for(i=0; i<eaSize(&pset->ppPowers); i++)
                {
                    if(pset->ppPowers[i])
                    {
                        eaPush(&ppPowers, pset->ppPowers[i]);
                    }
                }
            }

            iSizePows = eaSize(&ppPowers);

            iSize = eaSize(&e->supergroup->specialDetails);
            for(i=0; i<iSize; i++)
            {
                int j;

                // Only check details which actually have a power on them
                if(!e->supergroup->specialDetails[i]
                    || !e->supergroup->specialDetails[i]->pDetail
                    || !e->supergroup->specialDetails[i]->pDetail->pchPower)
                {
                    continue;
                }

                 //Only check details that are actually placed in the base, not those awaiting auto-placement
                if( !(e->supergroup->specialDetails[i]->iFlags & DETAIL_FLAGS_IN_BASE ) )
                    continue;

                // Look for a power to match to this detail. If you find
                //   one, remove it from the list. Anything in the list
                //   after looping over all of the details is extra and needs
                //   to be removed.
                for(j=0; j<iSizePows; j++)
                {
                    if(ppPowers[j]
                        && stricmp(e->supergroup->specialDetails[i]->pDetail->pchPower,ppPowers[j]->ppowBase->pchFullName) == 0)
                    {
                        ppPowers[j] = NULL;
                        break;
                    }
                }

                // If we didn't find a power for the detail. Add it.
                if(j==iSizePows)
                {
                    const BasePower *base = powerdict_GetBasePowerByFullName(&g_PowerDictionary,e->supergroup->specialDetails[i]->pDetail->pchPower);
                    Power *ppow;
                    if (base && (ppow = character_AddRewardPower(e->pchar, base)))                                        
                    {
                        // Set the level bought so that exemplaring doesn't
                        //    render the power obsolete.
                        ppow->iLevelBought = 0;
                    }
                }
            }

            // Anything left in the powers list is extra and needs to
            //   be removed.
            for(i=0; i<iSizePows; i++)
            {
                if(ppPowers[i])
                {
                    character_RemovePower(e->pchar, ppPowers[i]);
                }
            }

            eaDestroy(&ppPowers);
        }
        else
        {
            // not in supergroup mode, shut off all supergroup powers
            if(pset)
            {
                for(i=0; i<eaSize(&pset->ppPowers); i++)
                {
                    if (pset->ppPowers[i])
                        character_RemovePower(e->pchar, pset->ppPowers[i]);
                }
            }
        }
    }
}

/**********************************************************************func*
 * sgroup_canPayRent
 *
 */
bool sgroup_canPayRent( Entity * e )
{
    return e
        && e->supergroup
        && sgroup_hasPermission(e, SG_PERM_PAYRENT);
}

/**********************************************************************func*
 * sgroup_payRent
 *
 */
int sgroup_payRent( Entity * e )
{
    int resPaid = 0;

    if(verify(e && e->supergroup && sgroup_canPayRent(e)
              && e->supergroup->prestige >= e->supergroup->upkeep.prtgRentDue)
              && e->supergroup->upkeep.prtgRentDue > 0
              && verify(teamLock(e,CONTAINER_SUPERGROUPS)))
    {
        int periodsLate = sgroup_nUpkeepPeriodsLate(e->supergroup);

        resPaid = e->supergroup->upkeep.prtgRentDue;
        e->supergroup->prestige = e->supergroup->prestige - e->supergroup->upkeep.prtgRentDue;
        e->supergroup->upkeep.prtgRentDue = 0;

        // --------------------
        // set the next period to charge rent

        //a little tricky: if this is in the first N rent periods just add it to
        //the rent due time until it surpasses the current time,
        // otherwise: the next due date is added to the current

        if( periodsLate < g_baseUpkeep.periodResetRentDue )
        {
            // increment by the number of periods late + 1.
            // make sure move forward at least one period.
            e->supergroup->upkeep.secsRentDueDate += g_baseUpkeep.periodRent*(MAX(periodsLate,0) + 1);
        }
        else
        {
            e->supergroup->upkeep.secsRentDueDate = dbSecondsSince2000() + g_baseUpkeep.periodRent;
        }

        // --------------------
        // unlock

        teamUpdateUnlock(e,CONTAINER_SUPERGROUPS);

        dbLog(__FILE__, e, "paid rent %i due for supergroup %s. next due %i", resPaid, e->supergroup->name, e->supergroup->upkeep.secsRentDueDate);
    }
    else
    {
        dbLog(__FILE__, e, "failed to be able to pay rent");
    }

    return resPaid;
}

/**********************************************************************func*
 * sgroup_buyBase
 *
 */
int sgroup_buyBase( Entity * e )
{
    return true;
}

int checkSgrpMembers(Entity* e, char* calledFrom)
{
    int i, j;
    Supergroup* sg = e->supergroup;

    for(i = 0; i < sg->members.count; i++)
    {
        bool found = false;

        for(j = 0; j < eaSize(&sg->memberranks); j++)
        {
            if(sg->members.ids[i] == sg->memberranks[j]->dbid)
            {
                found = true;
                break;
            }
        }

        if(!devassertmsg(found, "Supergroup members struct has an id that has no rank"))
        {
            dbLog("Supergroup:checkSgrpMembers", e, "Supergroup members struct has an id that has no rank (%s)", calledFrom);
            return 0;
        }
    }
    for(j = eaSize(&sg->memberranks) - 1; j >= 0 ; j--)
    {
        bool found = false;

        for(i = 0; i < sg->members.count; i++)
        {
            if(sg->members.ids[i] == sg->memberranks[j]->dbid)
            {
                found = true;
                break;
            }
        }
        // We get into the condition of a vestigial entry in the memberranks array from time to time.
        // It's not immediately obvious how we get there, so for now we just clean up the mess when we find it.
        // Update.  It's definitely related to destroying characters at the login screen that are in a SG.
//        if(!devassertmsg(found, "Supergroup memberranks has a rank for someone not in the supergroup"))
//        {
//            dbLog("Supergroup:checkSgrpMembers", e, "Supergroup memberranks has a rank for someone not in the supergroup (%s)", calledFrom);
//            return 0;
//        }
        if (!found)
        {
            destroySupergroupMemberInfo((SupergroupMemberInfo *)eaRemoveFast(&sg->memberranks, j));
            // Restart the loop.  We don't subtract 1 here, the j-- as the third expression in the for loop takes care of that
            // Yes, this is inefficient.  No, I don't care.  This doesn't happen often enough for it to be a problem.
            j = eaSize(&sg->memberranks);
        }
    }

    if(!devassert(sg->members.count == eaSize(&sg->memberranks)))
    {
        dbLog("Supergroup:checkSgrpMembers", e, "Supergroup memberranks size does not match members struct (%s)", calledFrom);
        return 0;
    }

    return 1;
}

/**********************************************************************func*
* sgroup_RepairMemberList
* Repairs corruption on SgrpMembers
*/
int sgroup_RepairMemberList(Supergroup *sg, int justChecking)
{
    int i,j;
    int changed = 0;

    for (i = 0; i < eaSize(&sg->memberranks); i++)
    {
        if (!sg->memberranks[i] || !sg->memberranks[i]->dbid)
        { 
            // Empty entry, let's remove the row
            if (justChecking)
            {
                return 1;
            }
            else
            {
                LOG_SG( sg->name, 0, LOG_LEVEL_VERBOSE, 0, "Empty rank row found, deleted");
                destroySupergroupMemberInfo((SupergroupMemberInfo *)eaRemoveFast(&sg->memberranks,i));
                changed = 1;
                i--;
                continue;
            }
        }

        if (!supergroup_IsMember(sg,sg->memberranks[i]->dbid))
        { 
            // Not in supergroup, remove the row
            if (justChecking)
            {
                return 1;
            }
            else
            {
                LOG_SG( sg->name, 0, LOG_LEVEL_VERBOSE, 0, "Rank for non-member found, removed");
                destroySupergroupMemberInfo((SupergroupMemberInfo *)eaRemoveFast(&sg->memberranks,i));
                changed = 1;
                i--;                
                continue;
            }
        }

        for (j = i+1; j < eaSize(&sg->memberranks); j++)
        {
            // Searching for duplicates
            if (sg->memberranks[j] && sg->memberranks[i]->dbid == sg->memberranks[j]->dbid)
            {
                if (justChecking)
                {
                    return 1;
                }
                else
                {
                    // The order is no longer valid due to corruption, so use highest rank
                    if (sg->memberranks[j]->rank > sg->memberranks[i]->rank)                    
                        sg->memberranks[i]->rank = sg->memberranks[j]->rank;
                                        LOG_SG( sg->name, 0, LOG_LEVEL_VERBOSE, 0, "Duplicate rank entry found for player, removed");
                    destroySupergroupMemberInfo((SupergroupMemberInfo *)eaRemoveFast(&sg->memberranks,j));
                    changed = 1;
                    j--;
                }
            }
        }

        // Clamp rank

        // DGNOTE 1/29/2009 - Ought to determine if this guy has accesslevel or not, so we can set rank
        // appropriately.
        if (sg->memberranks[i]->rank < 0 || sg->memberranks[i]->rank >= NUM_SG_RANKS)
        {
            if (justChecking)
            {
                return 1;
            }
            else
            {
                if (sg->memberranks[i]->rank < 0)
                    sg->memberranks[i]->rank = 0;
                if (sg->memberranks[i]->rank >= NUM_SG_RANKS)
                    sg->memberranks[i]->rank = NUM_SG_RANKS-1;
                LOG_SG( sg->name, 0, LOG_LEVEL_VERBOSE, 0, "Player rank was outside bounds, clamped");
                changed = 1;
            }
        }

    }

    // Search for missing entries
    for (i = 0; i < sg->members.count; i++)
    {
        for (j = eaSize(&sg->memberranks) - 1; j >= 0; j--)
        {
            if (sg->memberranks[j]->dbid == sg->members.ids[i])
            {
                break;
            }
        }
        if (j == -1) // not found in ranks
        {
            if (justChecking)
            {
                return 1;
            }
            else
            {
                // Add a rank for this player
                SupergroupMemberInfo *newrank = createSupergroupMemberInfo();
                newrank->dbid = sg->members.ids[i];
                newrank->rank = 0;
                eaPush(&sg->memberranks,newrank);
                changed = 1;
                LOG_SG( sg->name, 0, LOG_LEVEL_VERBOSE, 0, "Player was missing rank entry, added");
            }
        }
    }

    return changed;
}


/**********************************************************************func*
* sgroup_RepairCorruption
* Repairs corruption on a supergroup, returning 1 if any found
*/

int sgroup_RepairCorruption(Supergroup *sg, int justChecking)
{
    int save_changes = 0;
    
    // Put in corruption repair here
    // If any repair occurs and we need to save the sg, it should return 1
    // If justChecking is passed on, don't actually modify the ent, just return if need repair

    save_changes |= sgroup_RepairMemberList(sg, justChecking);

    return save_changes;
}

int sgrp_CanSetPermissions(Entity *e)
{
    if (e == NULL || e->supergroup == NULL)
    {
        return 0;
    }
    return sgroup_hasPermission(e, SG_PERM_RANKS) ? 1 : 0;
}
