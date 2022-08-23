#include "alignment_shift.h"
#include "character.h"
#include "entity/entPlayer.h"
#include "entity/character_base.h"
#include "storyarc/contact.h"
#include "storyarc/storyinfo.h"
#include <utilitieslib/components/EString.h>

#include "TeamReward.h"
#include "entity/RewardToken.h"
#include "SgrpServer.h"
#include "entserver.h"
#include "storyarc/storyarcinterface.h"
//#include "storyarcprivate.h"    //should I really be including private?
#include "entGameActions.h"
#include "player/badges_server.h"
#include "auth/authUserData.h"
#include "cmdparse/cmdserver.h"
#include "entity/character_level.h"
#include "filter/titles.h"
#include "entity/character_eval.h"
#include "container/team.h"
#include "comm_game.h"
#include "character_animfx.h"
#include "dbcomm/staticMapInfo.h"
#include "dbcomm/dbcomm.h"
#include "entity/character_inventory.h"
#include "dbcomm/logcomm.h"
#include "dbcomm/dbmapxfer.h"
#include "gameSys/dooranim.h"
#include "character_pet.h"

void alignmentshift_updatePlayerTypeByLocation(Entity *e)
{
    STATIC_ASSERT(kPlayerType_Count == 2)
    const MapSpec* mapSpec = MapSpecFromMapId(db_state.base_map_id);
    
    if (mapSpec && MAP_ALLOWS_HEROES_ONLY(mapSpec))
    {
        entSetPlayerTypeByLocation(e, kPlayerType_Hero);
        entSetOnTeamHero(e, 1);
    }
    else if (mapSpec && MAP_ALLOWS_VILLAINS_ONLY(mapSpec))
    {
        entSetPlayerTypeByLocation(e, kPlayerType_Villain);
        entSetOnTeamVillain(e, 1);
    }
    else if (ENT_IS_ROGUE(e) && OnMissionMap()) 
    {
        // on a mission map, keep what influence type you were
        const ContactDef *contact = ContactDefinition(g_activemission->contact);
        // contact can be NULL if the player is a dev and using "goto" w/o an active mission
        if (contact && contact->alliance == ALLIANCE_HERO)
        {
            entSetOnTeamHero(e, 1);
        }
        else if (contact && contact->alliance == ALLIANCE_VILLAIN)
        {
            entSetOnTeamVillain(e, 1);
        }
        else if (ENT_IS_ON_RED_SIDE(e))
        {
            entSetOnTeamVillain(e, 1);
        }
        else
        {
            entSetOnTeamHero(e, 1);
        }
    }
    else
    {
        if (ENT_IS_VILLAIN(e))
        {
            entSetPlayerTypeByLocation(e, kPlayerType_Villain);
            entSetOnTeamVillain(e, 1);
        }
        else
        {
            entSetPlayerTypeByLocation(e, kPlayerType_Hero);
            entSetOnTeamHero(e, 1);
        }
    }
}

// This is only called when you shift alignments via the in-game system
// not when you buy the alignment change via MTX.
// Must be called before alignmentshift_cleanup()!
static void alignmentshift_updateStats(Entity *e, PlayerType type, PlayerSubType subType)
{
    STATIC_ASSERT(kPlayerType_Count == 2)
    STATIC_ASSERT(kPlayerSubType_Count == 3)
    if(e && e->pl)
    {
        int changed = 0;
        char *tempString = 0;

        if(e->pl->playerType != type)
        {
            if (!ENT_IS_IN_PRAETORIA(e) && !ENT_IS_LEAVING_PRAETORIA(e))
            {
                if (ENT_IS_HERO(e))
                {
                    badge_StatAdd(e, "TimesChangedAlignmentFromBlueToRed", 1);
                }
                else
                {
                    badge_StatAdd(e, "TimesChangedAlignmentFromRedToBlue", 1);
                }
            }

            changed = 1;
        }
        if(e->pl->playerSubType != subType)
        {
            changed = 1;
        }

        estrClear(&tempString);

        if (type == kPlayerType_Hero)
        {
            if (subType == kPlayerSubType_Paragon)
            {
                estrConcatStaticCharArray(&tempString, "Paragon");
            }
            else if (subType == kPlayerSubType_Rogue)
            {
                estrConcatStaticCharArray(&tempString, "Vigilante");
            }
            else
            {
                estrConcatStaticCharArray(&tempString, "Hero");
            }
        }
        else
        {
            if (subType == kPlayerSubType_Paragon)
            {
                estrConcatStaticCharArray(&tempString, "Tyrant");
            }
            else if (subType == kPlayerSubType_Rogue)
            {
                estrConcatStaticCharArray(&tempString, "Rogue");
            }
            else
            {
                estrConcatStaticCharArray(&tempString, "Villain");
            }
        }

        if(changed)
        {
            estrConcatStaticCharArray(&tempString, "Switch");
            giveRewardToken(e, "AlignmentChangeCount");
        }
        else
        {
            estrConcatStaticCharArray(&tempString, "Reinforce");
        }

        estrConcatStaticCharArray(&tempString, "MoralityMissionsCompleted");

        if (!ENT_IS_IN_PRAETORIA(e))
        {
            giveRewardToken(e, tempString);
        }
    }
}

// This is called when you shift alignments via the in-game system
// OR when you buy the alignment shift via MTX.
// Must be called after alignmentshift_updateStats()!
static void alignmentshift_cleanup(Entity *e, PlayerType type, PlayerSubType subType)
{
    STATIC_ASSERT(kPlayerType_Count == 2)
    STATIC_ASSERT(kPlayerSubType_Count == 3)
    if(e && e->pl)
    {
        int changed = 0;

        if(e->pl->playerType != type)
        {
            int contactIndex, contactCount;

            changed = 1;
            entSetPlayerType(e, type);
            // iAllyId will be updated in alignmentshift_updatePlayerTypeByLocation()

            TaskAbandonInvalidContacts(e);

            sgroup_setIsDark(e);
            title_set(e, 0, 0, 0, 0, 0);

            contactCount = eaSize(&e->storyInfo->contactInfos);
            for (contactIndex = 0; contactIndex < contactCount; contactIndex++)
            {
                if (!ContactCorrectAlliance(type, e->storyInfo->contactInfos[contactIndex]->contact->def->alliance))
                {
                    LOG_ENT_OLD(e, "GoingRogue:AlignmentShift Contact %s no longer the correct alliance.", e->storyInfo->contactInfos[contactIndex]->contact->def->filename);
                }
            }
        }
        if(e->pl->playerSubType != subType)
        {
            e->pl->playerSubType = subType;
            changed = 1;

            // Subtype is stored in the DB cache so it needs to get
            // updated globally.
            entSetPlayerSubType(e, subType);
        }

        // do this whether you changed alignments or not
        alignmentshift_resetPoints(e);
        alignmentshift_revokeAllTips(e, false); // force remove all tips, even if you would still pass all requirements
        // but don't reset roguepoint_timers
        ContactMarkAllDirty(e, StoryInfoFromPlayer(e));

        if(changed)
        {
            const SalvageItem *item;
            int count;

            item = salvage_GetItem("S_HeroMerit");
            count = character_SalvageCount(e->pchar, item);
            LOG_ENT(e, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "GoingRogue:MeritRevoke HAMs revoked: %d", count);
            character_AdjustSalvage(e->pchar, item, -count, "alignmentshift", 1);

            item = salvage_GetItem("S_VillainMerit");
            count = character_SalvageCount(e->pchar, item);
            LOG_ENT(e, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "GoingRogue:MeritRevoke VAMs revoked: %d", count);
            character_AdjustSalvage(e->pchar, item, -count, "alignmentshift", 1);

            e->team_update = 1;

            LOG_ENT_OLD( e, "GoingRogue:AlignmentShift Spent %i seconds as the previous alignment", getRewardToken(e, "TimePlayedAsCurrentAlignment")->val + timerSecondsSince2000() - e->last_time);

            // set to negative time since last save
            setRewardToken(e, "TimePlayedAsCurrentAlignment", e->last_time - timerSecondsSince2000());

            if (team_MinorityQuit(e, server_state.allowHeroAndVillainPlayerTypesToTeamUp, server_state.allowPrimalAndPraetoriansToTeamUp))
            {
                LOG_ENT_OLD( e, "GoingRogue:AlignmentShiftError Error in alignment shifting, shifted player dropped from team immediately, this shouldn't happen!  Shifted to type = %i, subtype = %i", e->pl->playerType, e->pl->playerSubType);
            }
            if (league_MinorityQuit(e, server_state.allowHeroAndVillainPlayerTypesToTeamUp, server_state.allowPrimalAndPraetoriansToTeamUp))
            {
                LOG_ENT_OLD( e, "GoingRogue:AlignmentShiftError Error in alignment shifting, shifted player dropped from league immediately, this shouldn't happen!  Shifted to type = %i, subtype = %i", e->pl->playerType, e->pl->playerSubType);
            }
        }
    }

    alignmentshift_updatePlayerTypeByLocation(e);
}

#define ALIGNMENT_POINT_TIMER_PREFIX "roguepoints_timer_"

void alignmentshift_determineSecondsUntilCanEarnAlignmentPoints(Entity *e, int seconds[], int alignments[])
{
    char timerName[64];
    RewardToken *timerToken = 0;
    U32 now = timerSecondsSince2000();
    U32 *timesLeft = 0;
    U32 tokenTime = 0;
    int timerIndex, pointIndex;

    eaiCreate(&timesLeft);
    for (timerIndex = 0; timerIndex < ALIGNMENT_POINT_MAX_COUNT; timerIndex++)
    {
        sprintf(timerName, "%s%d", ALIGNMENT_POINT_TIMER_PREFIX, timerIndex);
        timerToken = getRewardToken(e, timerName);
        if (timerToken)
        {
            eaiSortedPush(&timesLeft, timerToken->timer + ALIGNMENT_POINT_COOLDOWN_TIMER - now);
        }
        else
        {
            eaiSortedPush(&timesLeft, 0);
        }
    }

    for (pointIndex = 0; pointIndex < ALIGNMENT_POINT_MAX_COUNT; pointIndex++)
    {
        seconds[pointIndex] = eaiGet(&timesLeft, ALIGNMENT_POINT_MAX_COUNT - pointIndex - 1); // longest time is first timer
        alignments[pointIndex] = 0;
    }

    for (timerIndex = 0; timerIndex < ALIGNMENT_POINT_MAX_COUNT; timerIndex++)
    {
        sprintf(timerName, "%s%d", ALIGNMENT_POINT_TIMER_PREFIX, timerIndex);
        timerToken = getRewardToken(e, timerName);
        if (timerToken)
        {
            int timeLeft = timerToken->timer + ALIGNMENT_POINT_COOLDOWN_TIMER - now;

            for (pointIndex = 0; pointIndex < ALIGNMENT_POINT_MAX_COUNT; pointIndex++)
            {
                if (timeLeft == seconds[pointIndex])
                {
                    alignments[pointIndex] = timerToken->val;
                    break;
                }
            }
        }
    }
}

U32 alignmentshift_secondsUntilCanEarnAlignmentPoint(Entity *e)
{
    char timerName[64];
    RewardToken *timerToken = 0;
    U32 now = timerSecondsSince2000();
    U32 timeLeft = ALIGNMENT_POINT_COOLDOWN_TIMER;
    U32 tokenTime = 0;
    int timerIndex;

    for (timerIndex = 0; timerIndex < ALIGNMENT_POINT_MAX_COUNT; timerIndex++)
    {
        sprintf(timerName, "%s%d", ALIGNMENT_POINT_TIMER_PREFIX, timerIndex);
        timerToken = getRewardToken(e, timerName);
        if (timerToken && timerToken->timer + ALIGNMENT_POINT_COOLDOWN_TIMER > now)
        {
            tokenTime = timerToken->timer + ALIGNMENT_POINT_COOLDOWN_TIMER - now;
            if (tokenTime < timeLeft)
            {
                timeLeft = tokenTime;
            }
        }
        else
        {
            timeLeft = 0;
        }
    }

    return timeLeft;
}

int alignmentshift_countPointsEarnedRecently(Entity *e)
{
    char timerName[64];
    int count = 0;
    RewardToken *timerToken = 0;
    U32 now = timerSecondsSince2000();
    int timerIndex;

    for (timerIndex = 0; timerIndex < ALIGNMENT_POINT_MAX_COUNT; timerIndex++)
    {
        sprintf(timerName, "%s%d", ALIGNMENT_POINT_TIMER_PREFIX, timerIndex);
        timerToken = getRewardToken(e, timerName);
        if (timerToken && now - timerToken->timer < ALIGNMENT_POINT_COOLDOWN_TIMER)
            count++;
    }

    return count;
}

// val: ALIGNMENT_POINT_XXX macro
bool alignmentshift_awardPoint(Entity *e, int val)
{
    char timerName[64];
    bool award = false;
    RewardToken *timerToken = 0;
    U32 now = timerSecondsSince2000();
    int timerIndex;

    for (timerIndex = 0; timerIndex < ALIGNMENT_POINT_MAX_COUNT; timerIndex++)
    {
        sprintf(timerName, "%s%d", ALIGNMENT_POINT_TIMER_PREFIX, timerIndex);
        timerToken = getRewardToken(e, timerName);
        if (!award && (!timerToken || now - timerToken->timer >= ALIGNMENT_POINT_COOLDOWN_TIMER))
        {
            setRewardToken(e, timerName, val);
            e->storyInfo->contactInfoChanged = true;
            award = true;
            break;
        }
    }

    return award;
}

void alignmentshift_resetPoints(Entity *e)
{
    removeRewardToken(e, "roguepoints_paragon");
    removeRewardToken(e, "roguepoints_award_paragon");
    removeRewardToken(e, "roguepoints_reset_paragon");
    removeRewardToken(e, "roguepoints_hero");
    removeRewardToken(e, "roguepoints_award_hero");
    removeRewardToken(e, "roguepoints_reset_hero");
    removeRewardToken(e, "roguepoints_vigilante");
    removeRewardToken(e, "roguepoints_award_vigilante");
    removeRewardToken(e, "roguepoints_reset_vigilante");
    removeRewardToken(e, "roguepoints_rogue");
    removeRewardToken(e, "roguepoints_award_rogue");
    removeRewardToken(e, "roguepoints_reset_rogue");
    removeRewardToken(e, "roguepoints_villain");
    removeRewardToken(e, "roguepoints_award_villain");
    removeRewardToken(e, "roguepoints_reset_villain");
    removeRewardToken(e, "roguepoints_tyrant");
    removeRewardToken(e, "roguepoints_award_tyrant");
    removeRewardToken(e, "roguepoints_reset_tyrant");
}

void alignmentshift_resetTimers(Entity *e)
{
    char timerName[64];
    int timerIndex;

    for (timerIndex = 0; timerIndex < ALIGNMENT_POINT_MAX_COUNT; timerIndex++)
    {
        sprintf(timerName, "%s%d", ALIGNMENT_POINT_TIMER_PREFIX, timerIndex);
        removeRewardToken(e, timerName);
    }
    ContactMarkAllDirty(e, e->storyInfo);
}

void alignmentshift_resetSingleTimer(Entity *e, int timerIndex)
{
    char timerName[64];

    sprintf(timerName, "%s%d", ALIGNMENT_POINT_TIMER_PREFIX, timerIndex);
    removeRewardToken(e, timerName);

    ContactMarkAllDirty(e, e->storyInfo);
}

void alignmentshift_UpdateAlignment(Entity *e)
{
    if(e && e->pl && !e->doorFrozen) // !e->doorFrozen is to prevent getting the Hero/Villain FX for alignment MTX early when the map being transferred to is spinning up.
    {
        RewardToken * ret = 0;
        PlayerType type=0; 
        PlayerSubType subType=0;
        const MapSpec *mapSpec = 0;
        const BasePower *alignmentCountdownPower = 0, *alignmentPower = 0;
        int updateStats = 0;
        int cleanup = 0;
        int newMapId = 0;
        char *spawnTarget = NULL;

        //********************************
        // RESET ALIGNMENT POINTS TOKENS
        //********************************
        ret = getRewardToken( e, "roguepoints_reset_hero" );
        if(ret && ret->val>0)
        {
            removeRewardToken(e, "roguepoints_hero");
            removeRewardToken(e, "roguepoints_award_hero");
            removeRewardToken(e, "roguepoints_reset_hero");
            LOG_ENT_OLD( e, "GoingRogue:AlignmentPointReset Player reset all Hero points.");
            ContactMarkAllDirty(e, e->storyInfo);
        }
        ret = getRewardToken( e, "roguepoints_reset_vigilante" );
        if(ret && ret->val>0)
        {
            removeRewardToken(e, "roguepoints_vigilante");
            removeRewardToken(e, "roguepoints_award_vigilante");
            removeRewardToken(e, "roguepoints_reset_vigilante");
            LOG_ENT_OLD( e, "GoingRogue:AlignmentPointReset Player reset all Vigilante points.");
            ContactMarkAllDirty(e, e->storyInfo);
        }
        ret = getRewardToken( e, "roguepoints_reset_rogue" );
        if(ret && ret->val>0)
        {
            removeRewardToken(e, "roguepoints_rogue");
            removeRewardToken(e, "roguepoints_award_rogue");
            removeRewardToken(e, "roguepoints_reset_rogue");
            LOG_ENT_OLD( e, "GoingRogue:AlignmentPointReset Player reset all Rogue points.");
            ContactMarkAllDirty(e, e->storyInfo);
        }
        ret = getRewardToken( e, "roguepoints_reset_villain" );
        if(ret && ret->val>0)
        {
            removeRewardToken(e, "roguepoints_villain");
            removeRewardToken(e, "roguepoints_award_villain");
            removeRewardToken(e, "roguepoints_reset_villain");
            LOG_ENT_OLD( e, "GoingRogue:AlignmentPointReset Player reset all Villain points.");
            ContactMarkAllDirty(e, e->storyInfo);
        }

        //********************************
        // RESET ALIGNMENT TIMER TOKEN
        //********************************
        ret = getRewardToken(e, "roguepoints_reset_timers");
        if (ret && ret->val > 0)
        {
            removeRewardToken(e, "roguepoints_reset_timers");
            alignmentshift_resetTimers(e);
        }

        //********************************
        // AWARD ALIGNMENT POINTS TOKENS
        //********************************
        ret = getRewardToken(e, "roguepoints_award_hero");
        if (ret && ret->val > 0)
        {
            bool award = alignmentshift_awardPoint(e, ALIGNMENT_POINT_HERO);
            
            if (award)
            {
                RewardToken *points = getRewardToken(e, "roguepoints_hero");
                if (!points || points->val < maxAlignmentPoints_Hero)
                {
                    giveRewardToken(e, "roguepoints_hero");
                    LOG_ENT_OLD( e, "GoingRogue:AlignmentPoint Player earned a Hero point.");
                }
            }

            if (!ENT_IS_IN_PRAETORIA(e))
                badge_StatAdd(e, "HeroAlignmentMissionsCompleted", 1);

            removeRewardToken(e, "roguepoints_award_hero");
        }
        ret = getRewardToken(e, "roguepoints_award_vigilante");
        if (ret && ret->val > 0)
        {
            bool award = alignmentshift_awardPoint(e, ALIGNMENT_POINT_VIGILANTE);
            
            if (award)
            {
                RewardToken *points = getRewardToken(e, "roguepoints_vigilante");
                if (!points || points->val < maxAlignmentPoints_Vigilante)
                {
                    giveRewardToken(e, "roguepoints_vigilante");
                    LOG_ENT_OLD( e, "GoingRogue:AlignmentPoint Player earned a Vigilante point.");
                }
            }

            if (!ENT_IS_IN_PRAETORIA(e))
                badge_StatAdd(e, "VigilanteAlignmentMissionsCompleted", 1);

            removeRewardToken(e, "roguepoints_award_vigilante");
        }
        ret = getRewardToken(e, "roguepoints_award_rogue");
        if (ret && ret->val > 0)
        {
            bool award = alignmentshift_awardPoint(e, ALIGNMENT_POINT_ROGUE);
            
            if (award)
            {
                RewardToken *points = getRewardToken(e, "roguepoints_rogue");
                if (!points || points->val < maxAlignmentPoints_Rogue)
                {
                    giveRewardToken(e, "roguepoints_rogue");
                    LOG_ENT_OLD( e, "GoingRogue:AlignmentPoint Player earned a Rogue point.");
                }
            }

            if (!ENT_IS_IN_PRAETORIA(e))
                badge_StatAdd(e, "RogueAlignmentMissionsCompleted", 1);

            removeRewardToken(e, "roguepoints_award_rogue");
        }
        ret = getRewardToken(e, "roguepoints_award_villain");
        if (ret && ret->val > 0)
        {
            bool award = alignmentshift_awardPoint(e, ALIGNMENT_POINT_VILLAIN);
            
            if (award)
            {
                RewardToken *points = getRewardToken(e, "roguepoints_villain");
                if (!points || points->val < maxAlignmentPoints_Villain)
                {
                    giveRewardToken(e, "roguepoints_villain");
                    LOG_ENT_OLD( e, "GoingRogue:AlignmentPoin Player earned a Villain point.");
                }
            }

            if (!ENT_IS_IN_PRAETORIA(e))
                badge_StatAdd(e, "VillainAlignmentMissionsCompleted", 1);

            removeRewardToken(e, "roguepoints_award_villain");
        }

        //**************************
        // CHANGE ALIGNMENT TOKENS
        //**************************
        ret = getRewardToken( e, "newalignment_hero" );
        if(ret && ret->val>0)
        {
            removeRewardToken(e, "newalignment_hero");

            mapSpec = MapSpecFromMapId(e->static_map_id);
            if (e->access_level || (mapSpec && MAP_ALLOWS_HEROES(mapSpec)))
            {
                updateStats = 1;
                cleanup = 1;
                subType = kPlayerSubType_Normal;
                type = kPlayerType_Hero;

                if (e->pl->praetorianProgress == kPraetorianProgress_NeutralInPrimalTutorial)
                {
                    // don't grant badge stats!
                    updateStats = 0;
                    e->pl->praetorianProgress = kPraetorianProgress_PrimalBorn;
                    e->team_update = 1; // needed because we should have already been a hero, but we need to update praetorianProgress

                    character_PlayFX(e->pchar, e->pchar, "generic/AlignmentChange/Alignment_Hero.fx", colorPairNone, 0, 0, PLAYFX_NO_TINT);
                    LOG_ENT(e, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, "GoingRogue:AlignmentShift Player has started a Hero!");
                }
                else if (authUserHasProvisionalRogueAccess(e->pl->auth_user_data)
                         || AccountHasStoreProduct(ent_GetProductInventory(e), ENT_IS_IN_PRAETORIA(e) ? SKU("ctexgoro") : SKU("ctsygoro")))
                {
                    if (ENT_IS_IN_PRAETORIA(e))
                    {
                        character_PlayFX(e->pchar, e->pchar, "generic/AlignmentChange/Alignment_Resistance.fx", colorPairNone, 0, 0, PLAYFX_NO_TINT);
                        LOG_ENT_OLD( e, "GoingRogue:AlignmentShift:Praetoria Player has become Resistance!");
                    }
                    else
                    {
                        character_PlayFX(e->pchar, e->pchar, "generic/AlignmentChange/Alignment_Hero.fx", colorPairNone, 0, 0, PLAYFX_NO_TINT);
                        LOG_ENT_OLD( e, "GoingRogue:AlignmentShift Player has become a Hero!");

                        alignmentCountdownPower = powerdict_GetBasePowerByFullName(&g_PowerDictionary, "Temporary_Powers.Temporary_Powers.GrantHeroAlignmentPower");
                        alignmentPower = powerdict_GetBasePowerByFullName(&g_PowerDictionary, "Temporary_Powers.Temporary_Powers.GRHeroAlignmentPower");
                        if (alignmentCountdownPower && alignmentPower
                            && !character_OwnsPower(e->pchar, alignmentCountdownPower)
                            && !character_OwnsPower(e->pchar, alignmentPower))
                        {
                            character_AddRewardPower(e->pchar, alignmentCountdownPower);
                        }
                    }
                    //serveFloater(e, e, "FloatYouAreNowAHero", 0.0f, kFloaterStyle_Attention, 0);
                }
            }
            else
            {
                sendInfoBox(e, INFO_SVR_COM, "CantChangeAlignmentBadZone");
            }
        }
        ret = getRewardToken( e, "newalignment_pending_hero" );
        if(ret && ret->val>0)
        {
            // We just pretend that you're changing alignment.
            // - This is for use in the mission where you leave Praetoria so that if you
            //   don't complete the mission your character is undisturbed.
            // - This is used in the MTX alignment switch to display the FX after you've
            //   transfered maps to the other side.
            removeRewardToken( e, "newalignment_pending_hero" );
            character_PlayFX( e->pchar, e->pchar, "generic/AlignmentChange/Alignment_Hero.fx", colorPairNone, 0, 0, PLAYFX_NO_TINT );
        }
        ret = getRewardToken( e, "newalignment_vigilante" );
        if(ret && ret->val>0)
        {
            removeRewardToken(e, "newalignment_vigilante");

            mapSpec = MapSpecFromMapId(e->static_map_id);
            if (e->access_level || (mapSpec && MAP_ALLOWS_HEROES(mapSpec)))
            {
                if (authUserHasProvisionalRogueAccess(e->pl->auth_user_data)
                    || AccountHasStoreProduct(ent_GetProductInventory(e), SKU("ctsygoro")))
                {
                    updateStats = 1;
                    cleanup = 1;
                    subType = kPlayerSubType_Rogue;
                    type = kPlayerType_Hero;
                    
                    character_PlayFX(e->pchar, e->pchar, "generic/AlignmentChange/Alignment_Vigilante.fx", colorPairNone, 0, 0, PLAYFX_NO_TINT);
                    //serveFloater(e, e, "FloatYouAreNowAVigilante", 0.0f, kFloaterStyle_Attention, 0);
                    LOG_ENT_OLD( e, "GoingRogue:AlignmentShift Player has become a Vigilante!");

                    alignmentCountdownPower = powerdict_GetBasePowerByFullName(&g_PowerDictionary, "Temporary_Powers.Temporary_Powers.GrantVigilanteAlignmentPower");
                    alignmentPower = powerdict_GetBasePowerByFullName(&g_PowerDictionary, "Temporary_Powers.Temporary_Powers.GRVigilanteAlignmentPower");
                    if (alignmentCountdownPower && alignmentPower
                        && !character_OwnsPower(e->pchar, alignmentCountdownPower)
                        && !character_OwnsPower(e->pchar, alignmentPower))
                    {
                        character_AddRewardPower(e->pchar, alignmentCountdownPower);
                    }
                }
            }
            else
            {
                sendInfoBox(e, INFO_SVR_COM, "CantChangeAlignmentBadZone");
            }
        }
        ret = getRewardToken( e, "newalignment_rogue" );
        if(ret && ret->val>0)
        {
            removeRewardToken(e, "newalignment_rogue");

            mapSpec = MapSpecFromMapId(e->static_map_id);
            if (e->access_level || (mapSpec && MAP_ALLOWS_VILLAINS(mapSpec)))
            {
                if (authUserHasProvisionalRogueAccess(e->pl->auth_user_data)
                    || AccountHasStoreProduct(ent_GetProductInventory(e), SKU("ctsygoro")))
                {
                    updateStats = 1;
                    cleanup = 1;
                    subType = kPlayerSubType_Rogue;
                    type = kPlayerType_Villain;
                    
                    character_PlayFX(e->pchar, e->pchar, "generic/AlignmentChange/Alignment_Rogue.fx", colorPairNone, 0, 0, PLAYFX_NO_TINT);
                    //serveFloater(e, e, "FloatYouAreNowARogue", 0.0f, kFloaterStyle_Attention, 0);
                    LOG_ENT_OLD( e, "GoingRogue:AlignmentShift Player has become a Rogue!");

                    alignmentCountdownPower = powerdict_GetBasePowerByFullName(&g_PowerDictionary, "Temporary_Powers.Temporary_Powers.GrantRogueAlignmentPower");
                    alignmentPower = powerdict_GetBasePowerByFullName(&g_PowerDictionary, "Temporary_Powers.Temporary_Powers.GRRogueAlignmentPower");
                    if (alignmentCountdownPower && alignmentPower
                        && !character_OwnsPower(e->pchar, alignmentCountdownPower)
                        && !character_OwnsPower(e->pchar, alignmentPower))
                    {
                        character_AddRewardPower(e->pchar, alignmentCountdownPower);
                    }
                }
            }
            else
            {
                sendInfoBox(e, INFO_SVR_COM, "CantChangeAlignmentBadZone");
            }
        }
        ret = getRewardToken( e, "newalignment_villain" );
        if(ret && ret->val>0)
        {
            removeRewardToken(e, "newalignment_villain");

            mapSpec = MapSpecFromMapId(e->static_map_id);
            if (e->access_level || (mapSpec && MAP_ALLOWS_VILLAINS(mapSpec)))
            {
                updateStats = 1;
                cleanup = 1;
                subType = kPlayerSubType_Normal;
                type = kPlayerType_Villain;

                if (e->pl->praetorianProgress == kPraetorianProgress_NeutralInPrimalTutorial)
                {
                    // don't grant badge stats!
                    updateStats = 0;
                    e->pl->praetorianProgress = kPraetorianProgress_PrimalBorn;

                    character_PlayFX(e->pchar, e->pchar, "generic/AlignmentChange/Alignment_Villain.fx", colorPairNone, 0, 0, PLAYFX_NO_TINT);
                    LOG_ENT(e, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, "GoingRogue:AlignmentShift Player has started a Villain!");
                }
                else if (authUserHasProvisionalRogueAccess(e->pl->auth_user_data)
                         || AccountHasStoreProduct(ent_GetProductInventory(e), ENT_IS_IN_PRAETORIA(e) ? SKU("ctexgoro") : SKU("ctsygoro")))
                {
                    if (ENT_IS_IN_PRAETORIA(e))
                    {
                        character_PlayFX(e->pchar, e->pchar, "generic/AlignmentChange/Alignment_Loyalist.fx", colorPairNone, 0, 0, PLAYFX_NO_TINT);
                        LOG_ENT_OLD( e, "GoingRogue:AlignmentShift:Praetoria Player has become Loyalist!");
                    }
                    else
                    {
                        character_PlayFX(e->pchar, e->pchar, "generic/AlignmentChange/Alignment_Villain.fx", colorPairNone, 0, 0, PLAYFX_NO_TINT);
                        LOG_ENT_OLD( e, "GoingRogue:AlignmentShift Player has become a Villain!");

                        alignmentCountdownPower = powerdict_GetBasePowerByFullName(&g_PowerDictionary, "Temporary_Powers.Temporary_Powers.GrantVillainAlignmentPower");
                        alignmentPower = powerdict_GetBasePowerByFullName(&g_PowerDictionary, "Temporary_Powers.Temporary_Powers.GRVillainAlignmentPower");
                        if (alignmentCountdownPower && alignmentPower
                            && !character_OwnsPower(e->pchar, alignmentCountdownPower)
                            && !character_OwnsPower(e->pchar, alignmentPower))
                        {
                            character_AddRewardPower(e->pchar, alignmentCountdownPower);
                        }
                    }
                    //serveFloater(e, e, "FloatYouAreNowAVillain", 0.0f, kFloaterStyle_Attention, 0);
                }
            }
            else
            {
                sendInfoBox(e, INFO_SVR_COM, "CantChangeAlignmentBadZone");
            }
        }
        ret = getRewardToken( e, "newalignment_pending_villain" );
        if(ret && ret->val>0)
        {
            // We just pretend that you're changing alignment.
            // - This is for use in the mission where you leave Praetoria so that if you
            //   don't complete the mission your character is undisturbed.
            // - This is used in the MTX alignment switch to display the FX after you've
            //   transfered maps to the other side.
            removeRewardToken( e, "newalignment_pending_villain" );
            character_PlayFX( e->pchar, e->pchar, "generic/AlignmentChange/Alignment_Villain.fx", colorPairNone, 0, 0, PLAYFX_NO_TINT );
        }

        ret = getRewardToken(e, "newalignment_MTX_switch");
        if (ret && ret->val > 0)
        {
            removeRewardToken(e, "newalignment_MTX_switch");

            // updateStats remains zero
            cleanup = 1;
            subType = kPlayerSubType_Normal;
            spawnTarget = "NewPlayer";

            if (ENT_IS_VILLAIN(e))
            {
                giveRewardToken(e, "newalignment_pending_hero");
                LOG_ENT(e, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, "GoingRogue:AlignmentShift Player has MTX'd a Hero!");

                type = kPlayerType_Hero;

                newMapId = MAP_HERO_START;
            }
            else
            {
                giveRewardToken(e, "newalignment_pending_villain");
                LOG_ENT(e, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, "GoingRogue:AlignmentShift Player has MTX'd a Villain!");

                type = kPlayerType_Villain;

                newMapId = MAP_VILLAIN_START;
            }

            character_AddRewardPower(e->pchar, powerdict_GetBasePowerByFullName(&g_PowerDictionary, "Temporary_Powers.Temporary_Powers.ClearAlignmentPower"));

            // We only destroy pets for the MTX alignment change because the other switches
            // only happen in controlled situations where pets aren't a problem.
            character_DestroyAllPetsEvenWithoutAttribMod(e->pchar);
        }

        // must call updateStats then cleanup!
        if (updateStats)
        {
            alignmentshift_updateStats(e, type, subType);
        }

        if (cleanup)
        {
            alignmentshift_cleanup(e, type, subType);
        }

        if (newMapId)
        {
            char errmsg[200];
            Vec3 nowhere = {0};
            errmsg[0] = 0;

            DoorAnimEnter(e, NULL, nowhere, newMapId, NULL, spawnTarget, XFER_STATIC, errmsg);
        }

        //deal with tips
        ret = getRewardToken(e, "alignmenttiptoken_generic");
        if(ret && ret->val>0)
        {
            alignmentshift_awardTip(e);
            removeRewardToken(e, "alignmenttiptoken_generic");
        }
        if (eaSize(&g_DesignerContactTipTypes.designerTips))
        {
            int i;
            for (i = 0; i < eaSize(&g_DesignerContactTipTypes.designerTips); ++i)
            {
                const DesignerContactTip *d_tip = g_DesignerContactTipTypes.designerTips[i];
                ret = getRewardToken(e, d_tip->rewardTokenName );
                if(ret && ret->val>0)
                {
                    tipContacts_awardDesignerTip(e, i);
                    removeRewardToken(e, d_tip->rewardTokenName);
                }
            }
        }
    }
}

// only alignment and morality tips
int alignmentshift_countCurrentTips(Entity *e)
{
    int count = 0;
    int i;

    for (i = eaSize(&e->storyInfo->contactInfos) - 1; i >= 0; i--)
    {
        StoryContactInfo *contactInfo = e->storyInfo->contactInfos[i];
        if (contactInfo && (contactInfo->contact->def->tipType == TIPTYPE_ALIGNMENT || contactInfo->contact->def->tipType == TIPTYPE_MORALITY)
            && (!contactInfo->contact->def->interactionRequires || chareval_Eval(e->pchar, contactInfo->contact->def->interactionRequires, contactInfo->contact->def->filename)))
        {
            count++;
        }
    }

    return count;
}

// only alignment tips, not morality or other tips
int alignmentshift_countCurrentAlignmentTips(Entity *e)
{
    int count = 0;
    int i;

    for (i = eaSize(&e->storyInfo->contactInfos) - 1; i >= 0; i--)
    {
        StoryContactInfo *contactInfo = e->storyInfo->contactInfos[i];
        if (contactInfo && contactInfo->contact->def->tipType == TIPTYPE_ALIGNMENT &&
            (!contactInfo->contact->def->interactionRequires || chareval_Eval(e->pchar, contactInfo->contact->def->interactionRequires, contactInfo->contact->def->filename)))
        {
            count++;
        }
    }

    return count;
}

// count tips of the specified type
int tipContacts_countDesignerTips(Entity *e, int type)
{
    int count = 0;
    int i;

    for (i = eaSize(&e->storyInfo->contactInfos) - 1; i >= 0; i--)
    {
        StoryContactInfo *contactInfo = e->storyInfo->contactInfos[i];
        if (contactInfo && contactInfo->contact->def->tipType == type)
            count++;
    }

    return count;
}

void alignmentshift_awardTip(Entity *e)
{
    int * currentTipContacts = 0;
    eaiPopAll(&currentTipContacts);    //initializing
    //check if we can award a tip.
    if(alignmentshift_countCurrentTips(e) < 3 && !OnPlayerCreatedMissionMap())
    {
        int *contactHandles = 0;
        int nGoodTips = 0;
        int bShowMoralityNag = 0;
        TipType type = TIPTYPE_GRSYSTEM_MAX-1; // specifically doesn't award TIPTYPE_SPECIAL or designer tips
        //choose the tip, if any are available.
        // ARM: I rewrote this loop to ensure that type represents the type of the tips found
        // after you're through with the loop.
        while (!nGoodTips && type > TIPTYPE_NONE)
        {
            // special hack to ensure you don't drop alignment tips if you wouldn't get
            // credit for completing that tip's mission AFTER completing all the alignment
            // tip missions you already have.
            if (type != TIPTYPE_ALIGNMENT ||
                alignmentshift_countCurrentAlignmentTips(e) + alignmentshift_countPointsEarnedRecently(e) < ALIGNMENT_POINT_MAX_COUNT)
            {
                ContactGetValidTips(e, &contactHandles, type);
                eaiSortedDifference(&contactHandles, &contactHandles, &currentTipContacts);
                //contact handles is now all of the available contacts that the player can use.
                nGoodTips = eaiSize(&contactHandles);

                // players that haven't bought Going Rogue aren't allowed access to Morality Missions.
                // this INCLUDES trial accounts (shouldn't matter, since this stuff is 20+ and trials cap ~14)
                if (type == TIPTYPE_MORALITY && // we're looking for Morality tips
                    !authUserHasRogueAccess((U32 *)e->pl->auth_user_data) && !AccountHasStoreProduct(ent_GetProductInventory(e), SKU("ctsygoro")) // but they can't have them
                    && nGoodTips > 0) // and they would have gotten one if they could have
                {
                    bShowMoralityNag = 1;

                    // the array must've been empty before checking these tips
                    // otherwise we would've fallen out of the loop
                    eaiPopAll(&contactHandles);
                    nGoodTips = 0;
                }
            }

            if (!nGoodTips)
            {
                type--;
            }
        }

        if(bShowMoralityNag)
        {
            START_PACKET(pak, e, SERVER_GOINGROGUE_NAG)
                pktSendBitsAuto(pak, 2);
            END_PACKET
        }

        if(nGoodTips > 0)
        {
            int randomTipId = rand()%nGoodTips;
            int specialIndex;
            ContactHandle chandle = 0;
            StoryInfo *info = e->storyInfo; // NOT StoryInfoFromPlayer(e) because that can return the taskforce's info

            chandle = contactHandles[randomTipId];

            // Only add contacts known to the system but not the
            // player, so their contact list stays valid with no
            // duplicates.
            if(chandle && !ContactGetInfo(info, chandle))
            {
                const ContactDef *contactDef = ContactDefinition(chandle);

                ContactAdd(e, info, chandle, "GR Tip");
                serveFloater(e, e, "FloatFoundTip", 0.0f, kFloaterStyle_Attention, 0);
                sendInfoBox(e, INFO_REWARD, "TipYouReceived");
                badge_StatAdd(e, "GoingRogueTipsAwarded", 1);
                LOG_ENT(e, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, "GoingRogue:TipAwarded Player received tip %s", contactDef->filename);

                if (type == TIPTYPE_ALIGNMENT)
                {
                    char *streakbreakTokenHeader;
                    char streakbreakTokenName[32];
                    int streakbreakIndex;
                    RewardToken *streakbreakTokenCurr, *streakbreakTokenNext;

                    if (ENT_IS_ON_RED_SIDE(e))
                    {
                        streakbreakTokenHeader = GRTIP_STREAKBREAKER_REDNAME;
                    }
                    else
                    {
                        streakbreakTokenHeader = GRTIP_STREAKBREAKER_BLUENAME;
                    }

                    for (streakbreakIndex = GRTIP_STREAKBREAKER_COUNT - 2; streakbreakIndex >= 0; streakbreakIndex--)
                    {
                        sprintf(streakbreakTokenName, "%s%d", streakbreakTokenHeader, streakbreakIndex);
                        if (streakbreakTokenCurr = getRewardToken(e, streakbreakTokenName))
                        {
                            sprintf(streakbreakTokenName, "%s%d", streakbreakTokenHeader, streakbreakIndex + 1);
                            if (!(streakbreakTokenNext = getRewardToken(e, streakbreakTokenName)))
                            {
                                streakbreakTokenNext = giveRewardToken(e, streakbreakTokenName);
                            }
                            streakbreakTokenNext->val = streakbreakTokenCurr->val;
                        }
                    }

                    sprintf(streakbreakTokenName, "%s%d", streakbreakTokenHeader, 0);
                    if (!(streakbreakTokenCurr = getRewardToken(e, streakbreakTokenName)))
                    {
                        streakbreakTokenCurr = giveRewardToken(e, streakbreakTokenName);
                    }
                    // BAD CODE!  HACK!  Don't store contact handles permanently,
                    // since they can change whenever a designer adds a new contact!
                    // ...we just don't have access to vars.attribute on the mapserver.
                    // TODO:  stop using tokens for this and make it standard database fare?
                    streakbreakTokenCurr->val = chandle;
                }
            }

            // Award all special tips now.
            ContactGetValidTips(e, &contactHandles, TIPTYPE_SPECIAL);

            for (specialIndex = eaiSize(&contactHandles) - 1; specialIndex >= 0; specialIndex--)
            {
                chandle = contactHandles[specialIndex];

                if (chandle && !ContactGetInfo(info, chandle))
                {
                    const ContactDef *contactDef = ContactDefinition(chandle);
                    
                    ContactAdd(e, info, chandle, "Special Tip");
                    LOG_ENT_OLD( e, "GoingRogue:SpecialTipAwarded Player received special tip %s", contactDef->filename);
                }
            }
        }
    }
    eaiDestroy(&currentTipContacts);
}

void tipContacts_awardDesignerTip(Entity *e, int type)
{
    int * currentTipContacts = 0;
    int *contactHandles = 0;
    int nGoodTips = 0;
    int tipOffset = TIPTYPE_STANDARD_COUNT+1;
    int tipAdjustedIndex = type+tipOffset;
    int numExistingTips;
    eaiPopAll(&currentTipContacts);    //initializing
    
    numExistingTips = tipContacts_countDesignerTips(e, tipAdjustedIndex);
    if(numExistingTips >= g_DesignerContactTipTypes.designerTips[type]->tipLimit)
        return;

    ContactGetValidTips(e, &contactHandles, tipAdjustedIndex);
    eaiSortedDifference(&contactHandles, &contactHandles, &currentTipContacts);
    //contact handles is now all of the available contacts that the player can use.
    nGoodTips = eaiSize(&contactHandles);
            
    if(nGoodTips > 0)
    {
        int randomTipId = rand()%nGoodTips;
        ContactHandle chandle = 0;
        StoryInfo *info = e->storyInfo; // NOT StoryInfoFromPlayer(e) because that can return the taskforce's info

        chandle = contactHandles[randomTipId];

        // Only add contacts known to the system but not the
        // player, so their contact list stays valid with no
        // duplicates.
        if(chandle && !ContactGetInfo(info, chandle))
        {
            const ContactDef *contactDef = ContactDefinition(chandle);
            const char *desc = g_DesignerContactTipTypes.designerTips[type]->tokenTypeStr;
            ContactAdd(e, info, chandle, desc);
            serveFloater(e, e, "FloatFoundTip", 0.0f, kFloaterStyle_Attention, 0);
            sendInfoBox(e, INFO_REWARD, "TipYouReceived");
            dbLog("TipContacts:Awarded:CustomDesigner", e, "Player received tip %s, %s", desc, contactDef->filename);
        }
    }
    eaiDestroy(&currentTipContacts);
}

void alignmentshift_revokeTip(Entity *e, int contactHandle)
{
    ContactDismiss(e, contactHandle, 0);
}

void alignmentshift_revokeAllBadTips(Entity *e)
{
    int index = 0;
    int contactHandle = 0;
    const ContactDef *contactDef = 0;
    int player_level = character_GetExperienceLevelExtern(e->pchar);

    for (index = eaSize(&e->storyInfo->contactInfos) - 1; index >= 0; index--)
    {
        contactHandle = e->storyInfo->contactInfos[index]->handle;
        contactDef = e->storyInfo->contactInfos[index]->contact->def;

        // Specifically doesn't revoke TIPTYPE_SPECIAL or designer tips
        if (contactDef->tipType != TIPTYPE_ALIGNMENT && contactDef->tipType != TIPTYPE_MORALITY)
            continue;

        if ((contactDef->minPlayerLevel && player_level < contactDef->minPlayerLevel) ||
            (contactDef->maxPlayerLevel && player_level > contactDef->maxPlayerLevel))
        {
            if (e->access_level < 9)
            {
                LOG_ENT_OLD( e, "GoingRogue:TipRevoked Player has lost tip \"%s\" because they no longer qualify.", contactDef->filename);
                alignmentshift_revokeTip(e, contactHandle);
            }
        }
    }
}

void alignmentshift_revokeAllTips(Entity *e, int revokeEverythingReally)
{
    int count = 0;
    int i;
    char tokenName[128] = {0};

    for (i = eaSize(&e->storyInfo->contactInfos) - 1; i >= 0; i--)
    {
        StoryContactInfo *contactInfo = e->storyInfo->contactInfos[i];
        // Specifically doesn't revoke TIPTYPE_SPECIAL
        if (revokeEverythingReally ||
                (contactInfo && (contactInfo->contact->def->tipType == TIPTYPE_ALIGNMENT 
                                || contactInfo->contact->def->tipType == TIPTYPE_MORALITY
                                || (contactInfo->contact->def->tipType > TIPTYPE_STANDARD_COUNT
                                    && g_DesignerContactTipTypes.designerTips[contactInfo->contact->def->tipType - TIPTYPE_STANDARD_COUNT - 1]->revokeOnAlignmentSwitch))))
        {
            alignmentshift_revokeTip(e, contactInfo->handle);
        }
    }
}

void alignmentshift_resetAllTips(Entity *e)
{
    alignmentshift_revokeAllTips(e, true);
    alignmentshift_resetTimers(e);
}

static alignmentshift_appendAlignmentTimerStats(char **tempEStr, RewardToken *alignmentTimerToken)
{
    if (!alignmentTimerToken)
    {
        estrConcatStaticCharArray(tempEStr, "EMPTY\n");
    }
    else
    {
        int timeLeft = alignmentTimerToken->timer + ALIGNMENT_POINT_COOLDOWN_TIMER - timerSecondsSince2000();

        if (timeLeft <= 0)
        {
            estrConcatStaticCharArray(tempEStr, "EMPTY\n");
        }
        else
        {
            switch (alignmentTimerToken->val)
            {
                xcase ALIGNMENT_POINT_PARAGON:
                    estrConcatStaticCharArray(tempEStr, "Paragon");
                xcase ALIGNMENT_POINT_HERO:
                    estrConcatStaticCharArray(tempEStr, "Hero");
                xcase ALIGNMENT_POINT_VIGILANTE:
                    estrConcatStaticCharArray(tempEStr, "Vigilante");
                xcase ALIGNMENT_POINT_ROGUE:
                    estrConcatStaticCharArray(tempEStr, "Rogue");
                xcase ALIGNMENT_POINT_VILLAIN:
                    estrConcatStaticCharArray(tempEStr, "Villain");
                xcase ALIGNMENT_POINT_TYRANT:
                    estrConcatStaticCharArray(tempEStr, "Tyrant");
            }

            estrConcatf(tempEStr, " (time left: %d:%d:%d)\n", timeLeft / 3600, timeLeft / 60 % 60, timeLeft % 60);
        }
    }
}

void alignmentshift_printRogueStats(Entity *e, char **buf)
{
    int index;
    char rtBuf[32];
    const char *type[] = { "hero", "villain" };                    STATIC_ASSERT(ARRAY_SIZE(type) == kPlayerType_Count)
    const char *subtype[] = { "normal", "paragon", "rogue" };    STATIC_ASSERT(ARRAY_SIZE(subtype) == kPlayerSubType_Count)
    const char *alignment[] = {    "hero", "paragon", "vigilante",
                                "villain", "tyrant", "rogue" };    STATIC_ASSERT(ARRAY_SIZE(alignment) == kPlayerType_Count*kPlayerSubType_Count)
    const char *praetorian[] = { "normal", "tutorial", "praetoria", "earth", "travelhero", "travelvillain", "neutralInPrimalTutorial" }; STATIC_ASSERT(ARRAY_SIZE(praetorian) == kPraetorianProgress_Count)
    const char *allyid[] = { "none", "hero", "monster", "villain" };
    RewardToken *rt;

    estrConcatf(buf, "Alignment:      %s (%s %s)\n",
                        AINRANGE(e->pl->playerType*kPlayerSubType_Count + e->pl->playerSubType, alignment)
                            ? alignment[e->pl->playerType*kPlayerSubType_Count + e->pl->playerSubType]
                            : "unknown",
                        AINRANGE(e->pl->playerSubType, subtype)
                            ? subtype[e->pl->playerSubType]
                            : "unknown",
                        AINRANGE(e->pl->playerType, type)
                            ? type[e->pl->playerType]
                            : "unknown"
                );
    estrConcatf(buf, "Praetorian:     %s\n",
                        AINRANGE(e->pl->praetorianProgress, praetorian)
                            ? praetorian[e->pl->praetorianProgress]
                            : "unknown"
                );
    estrConcatf(buf, "AllyID:         %s\n",
                        AINRANGE(e->pchar->iAllyID, allyid)
                            ? allyid[e->pchar->iAllyID]
                            : "unknown"
                );
    rt = getRewardToken(e, "roguepoints_paragon");
    estrConcatf(buf, "Paragon Points:    %d\n", rt ? rt->val : 0);
    rt = getRewardToken(e, "roguepoints_hero");
    estrConcatf(buf, "Hero Points: %d\n", rt ? rt->val : 0);
    rt = getRewardToken(e, "roguepoints_vigilante");
    estrConcatf(buf, "Vigilante Points:    %d\n", rt ? rt->val : 0);
    rt = getRewardToken(e, "roguepoints_rogue");
    estrConcatf(buf, "Rogue Points: %d\n", rt ? rt->val : 0);
    rt = getRewardToken(e, "roguepoints_villain");
    estrConcatf(buf, "Villain Points:    %d\n", rt ? rt->val : 0);
    rt = getRewardToken(e, "roguepoints_tyrant");
    estrConcatf(buf, "Tyrant Points: %d\n", rt ? rt->val : 0);

    for (index = 0; index < ALIGNMENT_POINT_MAX_COUNT; index++)
    {
        sprintf(rtBuf, "%s%d", ALIGNMENT_POINT_TIMER_PREFIX, index);
        rt = getRewardToken(e, rtBuf);
        estrConcatf(buf, "Alignment Timer #%d: ", index + 1);
        alignmentshift_appendAlignmentTimerStats(buf, rt);
    }

    estrConcatf(buf, "Influence:      %d\n", e->pchar->iInfluencePoints);
}
