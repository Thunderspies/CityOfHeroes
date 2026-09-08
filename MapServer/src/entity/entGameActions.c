#include <utilitieslib/utils/wininclude.h>
#include <stdio.h>
#include <utilitieslib/network/netio.h>
#include "svr/svr_base.h"
#include "entGameActions.h"
#include "gameComm/sendToClient.h"
#include "entity/entVarUpdate.h"
#include "comm_game.h"

#include <utilitieslib/assert/assert.h>
#include "varutils.h"
#include "player/pmotion.h"
#include <utilitieslib/utils/error.h>
#include "language/langServerUtil.h"    // for svrMenuMessages
#include "gameComm/Npc.h"
#include "player/playerState.h" // client-side file. I'm going to hell for this.
#include "entity/character_base.h"
#include <utilitieslib/components/earray.h>
#include "timer_callback.h"
#include "Reward.h"
#include "TeamReward.h"
#include <utilitieslib/utils/error.h>
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/timing.h>

#include "entity/character_base.h"
#include "character_combat.h"
#include "character_animfx.h"
#include "character_pet.h"
#include "entity/character_level.h"
#include "entity/character_mods.h"
#include "entity/attribmod.h"
#include "entity/powers.h"
#include "gameComm/VillainDef.h"
#include "entserver.h"
#include "container/team.h"            // for teamGetIdFromEnt()
#include "entity/character_level.h"
#include "storyarc/storyarcinterface.h"
#include "ai/entai.h"
#include "ai/entaiprivate.h"

#include "storyarc/storyarcinterface.h"
#include "player/pl_stats.h" // for stat_...

#include "dbcomm/dbcomm.h" // for db_log()
#include "dbghelper.h"

#include "gridcoll/megaGrid.h"
#include "beacon/beacon.h"
#include "beacon/beaconPrivate.h"
#include "cmdparse/cmdcontrols.h"

#include "player/badges_server.h"

#include "group/groupscene.h"
#include "generator/encounter.h"

#include "gameSys/arenamap.h"
#include "seq/seqstate.h"
#include "seq/seq.h"
#include "entity/entity.h"
#include "script/scriptengine.h"

#include "seq/seqskeleton.h"
#include "seq/seqanimate.h"
#include "NovodeX/NwRagdoll.h"
#include "NovodeX/NwWrapper.h"
#include "entity/Supergroup.h"

#include "pvp.h"
#include "entity/character_target.h"
#include "bases/basedata.h"
#include "gameSys/sgraid.h"
#include "bases/bases.h"
#include "gameSys/sgraid_V2.h"

#include "auth/authUserData.h"
#include "entity/entPlayer.h"
#include "gameSys/petarena.h"
#include "dbcomm/staticMapInfo.h"

#include "entity/TaskforceParams.h"
#include "storyarc/mission.h"
#include "dbcomm/logcomm.h"
#include "entity/character_tick.h"
#include "cmdparse/cmdserver.h"
// #define REPORT_HIGHEST_DAMAGER // Define if we should print out who did the most damage to a villain

#define MIN_TELEPORT_DISTANCE    10
#define MAX_TELEPORT_DISTANCE     40

SHARED_MEMORY DamageDecayConfig g_DamageDecayConfig;

static int  tmpEntList[ MAX_ENTLIST_LEN ];

int entAlive( Entity *e )
{
    if( !e || entMode( e, ENT_DEAD ) )
        return FALSE;
    return TRUE;
}


// Send out a bunch of messages telling everyone who has defeated the victim.
void sendVictoryMessages(Entity* didMostDamager, Entity* attackerThatDealtFinalBlow, Entity* victim)
{
    Entity* victor = (didMostDamager ? didMostDamager : attackerThatDealtFinalBlow);
    int i;

    if (!attackerThatDealtFinalBlow)
        return;
    if((!didMostDamager && !attackerThatDealtFinalBlow) || !victim)
        return;

	// explicit hack to remove defeat spam from the Breakout and Galaxy City Tutorials.
	if ((db_state.base_map_id == 41 || db_state.base_map_id == 70) && ENTTYPE(attackerThatDealtFinalBlow) != ENTTYPE_PLAYER)
        return;

    buildCloseEntList(victim, victor, (F32)200.f, tmpEntList, MAX_ENTLIST_LEN, TRUE);
    i = 0;
    while( tmpEntList[i] != -1 )
    {
        Entity* msgTarget = entities[tmpEntList[i]];
        i++;

        // let friendly units know you rock.
        if( ENTTYPE(msgTarget) == ENTTYPE_PLAYER && entity_TargetIsInVisionPhase(attackerThatDealtFinalBlow, msgTarget))
        {
            char buffer[512];
            const char* victimname;

            if(msgTarget == attackerThatDealtFinalBlow)
                continue;

            victimname = entGetName(victim);

            strcpy( buffer, localizedPrintf(msgTarget, "HeroDefatedVillain", attackerThatDealtFinalBlow->name, victimname ));

#ifdef REPORT_HIGHEST_DAMAGER
            // Only add this section if more than one person will see it.
            if    (eaSize(&victim->who_damaged_me) > 1 &&
                (didMostDamager && attackerThatDealtFinalBlow) &&
                (attackerThatDealtFinalBlow->db_id != didMostDamager->db_id))
            {
                strcat(buffer, " ");
                if (didMostDamager->db_id == msgTarget->db_id)
                    strcat(buffer, localizedPrintf(msgTarget,"IDealtMostDmg") );
                else
                    strcat(buffer, localizedPrintf(msgTarget,"OthersDealtMostDmg", didMostDamager->name,victimname) );
            }
#endif

            sendChatMsg(msgTarget, buffer, INFO_SVR_COM, 0);
        }
    }

    // let yourself know you rock
    if(attackerThatDealtFinalBlow && ENTTYPE(attackerThatDealtFinalBlow)== ENTTYPE_PLAYER)
    {
        char buffer[512];
        const char* victimname;

        victimname = entGetName(victim);

        strcpy( buffer, localizedPrintf(attackerThatDealtFinalBlow,"IDefeatedVillain", victimname ));

#ifdef REPORT_HIGHEST_DAMAGER
        if    (eaSize(&victim->who_damaged_me) > 1 &&
            (didMostDamager && attackerThatDealtFinalBlow))
        {
            strcat(buffer, " ");
            if((attackerThatDealtFinalBlow->db_id != didMostDamager->db_id))
            {
                strcat( buffer, localizedPrintf(attackerThatDealtFinalBlow,"OthersDealtMostDmg", didMostDamager->name));
            }
            else
            {
                strcat( buffer, localizedPrintf(attackerThatDealtFinalBlow,"IDealtMostDmg"));
            }
        }
#endif

        sendChatMsg(attackerThatDealtFinalBlow, buffer, INFO_SVR_COM, 0);
    }
}

void entDoFirstProcessStuff(Entity* e, AIVars* ai, int index){
    SET_ENTINFO_BY_ID(index).has_processed = 1;

    copyVec3(ENTPOS(e), ai->spawn.pos);

    getMat3YPR(ENTMAT(e), ai->spawn.pyr);

    if(e->seq->type->spawnOffsetY){
        Vec3 offset, newpos;
        scaleVec3(ENTMAT(e)[1], e->seq->type->spawnOffsetY, offset);
        addVec3(offset, ENTPOS(e), newpos);
        entUpdatePosInterpolated(e, newpos);
    }

    //CUTSCENE
    if( server_state.viewCutScene && !e->cutSceneState )
        setEntityInCutScene( e );
    //END CUTSCENE

    //CUTSCENE TO DO move this to entity creation if I can figure out everywhere that is and make sure it doesn't get overwritten
    if( server_state.viewCutScene && !e->cutSceneState )
        setEntityInCutScene( e );
    //END CUTSCENE

    //Notify any interested scripts that this guy has been created
    ScriptEntityCreated(e);

    // Do initial encounter processing
    if (ENTTYPE(e) == ENTTYPE_PLAYER)
        EncounterPlayerCreate(e);

    // Force the power system to run an update.

    if(e->pchar){
        PERFINFO_AUTO_START("TickPhaseOne", 1);
        character_TickPhaseOne(e->pchar, 0);
        PERFINFO_AUTO_STOP();

        PERFINFO_AUTO_START("TickPhaseTwo", 1);
        character_TickPhaseTwo(e->pchar, 0);
        PERFINFO_AUTO_STOP();
    }

}

/**********************************************************************func*
 * dieNow
 *
 */
void dieNow(Entity *e, DeathBy eDeathBy, bool bAnimate, int iDelay)
{
    AttribMod *pmod;
    AttribModListIter iter;
    float fXPDebtProtect = 0.0f;
    float fXPDebtProtectMod = 0.0f;
    Entity* victor = NULL;

    if( entMode( e, ENT_DEAD ) )
        return;

    //    make sure they do first process stuff at least once
    if(!ENTINFO_BY_ID(e->owner).has_processed){
        AIVars* ai = ENTAI_BY_ID(e->owner);
        PERFINFO_AUTO_START("firstProcess", 1);
        entDoFirstProcessStuff(e, ai, e->owner);
        PERFINFO_AUTO_STOP();
    }

    // Figure out if they have any debt protection on.
    pmod = modlist_GetFirstMod(&iter, &e->pchar->listMod);
    while(pmod!=NULL)
    {
        if(pmod->ptemplate->offAttrib == kSpecialAttrib_XPDebtProtection)
        {
            if(pmod->ptemplate->offAspect == offsetof(CharacterAttribSet, pattrMod))
            {
                fXPDebtProtectMod += pmod->fMagnitude;
            }
            else if(pmod->ptemplate->offAspect == offsetof(CharacterAttribSet, pattrAbsolute))
            {
                if(pmod->fMagnitude>fXPDebtProtect)
                {
                    fXPDebtProtect = pmod->fMagnitude;
                }
            }
        }

        pmod = modlist_GetNextMod(&iter);
    }

    // interrupt anything they were doing
    character_InterruptPower(e->pchar, NULL, 0.0f, false);

    // Get rid of all their pets (except those marked KeepThroughDeath)
    character_DestroyAllPetsAtDeath(e->pchar);
    // Remove all outstanding attrib mods on the character (except those marked KeepThroughDeath)
    character_RemoveModsAtDeath(e->pchar);

    // Shut off all the powers the character has on.
    character_ShutOffAllTogglePowers(e->pchar);
    character_ShutOffAllAutoPowers(e->pchar);
    // onDisable mods for live powers happen below.
    character_KillCurrentPower(e->pchar);
    character_KillQueuedPower(e->pchar);
    character_KillRetryPower(e->pchar);

    // MUST RUN character_AccrueMods() here to allow kModApplicationType_OnDeactivate,
    //   and kModApplicationType_OnExpire mods to work!
    // character_TickPhaseTwo() goes into an infinite loop.
    // Actually, I'm no longer sure this is necessary for onDeactivate mods.  I've altered
    //   mod_Process to not drop mods if the mod is on the caster of the power.  From that,
    //   I theorize that the mod would still work after death, just like onDisable mods do.
    //   However, this call was added before onDeactivate mods existed, so there's probably
    //   some other fundamental issue that would need to be addressed to remove this call.  ARM
    character_AccrueMods(e->pchar, 0.0f, NULL);

    // Make sure they stay dead on the next tick.
    e->pchar->attrCur.fHitPoints = 0.0f;
    e->pchar->attrCur.fEndurance = 0.0f;

    // if entity dying is pet, remove mods from parent
    character_PetClearParentMods(e->pchar);

    // Mark the entity as dead
    modStateBits( e, ENT_DEAD, iDelay, TRUE );

    // Trigger onDisable mods for live powers and onEnable mods for dead powers.
    character_UpdateAllEnabledState(e->pchar);

    // Turn on autopowers which work only when you're dead.
    character_ActivateAllAutoPowers(e->pchar);

    // Animate them unto death
    if(bAnimate)
    {
        int i;
        seqOrState(e->seq->state, 1, STATE_HIT);
        seqOrState(e->seq->state, 1, STATE_DEATH);

        if((i = eaSize(&e->pchar->ppLastDamageFX)) > 0)
        {
            // This is a hack since it just goes to an arbitrary last damager.
            // I didn't make it a list since I was a bit lazy and the vast,
            // vast majority of the time there's only one. Allocating an
            // int64 and deallocating all the time seemed wasteful for an
            // event which rarely, if ever, happens.
            Entity *eLastDamage = erGetEnt(e->pchar->erLastDamage);

            while(i>0)
            {
                i--;

                character_SetAnimBits(e->pchar, e->pchar->ppLastDamageFX[i]->piDeathBits, 0.0f, PLAYFX_NOT_ATTACHED);
                if(eLastDamage)
                {
                    // TODO: Do a reverse lookup for the tint of the death fx.
                    character_PlayFX(e->pchar, eLastDamage->pchar,
                        e->pchar->ppLastDamageFX[i]->pchDeathFX, colorPairNone,
                        0.0f, PLAYFX_NOT_ATTACHED, PLAYFX_DONTPITCHTOTARGET|PLAYFX_NO_TINT );
                }
            }
        }
    }

    if( ENTTYPE(e) == ENTTYPE_PLAYER )
    {
        setClientState( e, STATE_DEAD );

        // Are we on a taskforce and is the 'maximum deaths' challenge
        // active?
        if( e->taskforce_id && e->taskforce && TaskForceIsParameterEnabled(e, TFPARAM_DEATHS) )
        {
            TaskForceChalkMemberDeath( e );
        }

        if(!OnArenaMap() && erGetDbId(e->pchar->erLastDamage)==0 && !(g_ArchitectTaskForce && g_ArchitectTaskForce->architect_flags&ARCHITECT_TEST) ) // If a player is the victor, don't give the defeat penalty
        {
            // Penalize them for the defeat
            float fEnvDamage = damageTrackerFindEnvironmentalShare(e);
            float fDebtPercentage = fXPDebtProtect ? fXPDebtProtect : fXPDebtProtectMod;
            fDebtPercentage = 1.0 - fDebtPercentage;

            if(!db_state.is_static_map)
            {
                fDebtPercentage /= 2;
            }

            fDebtPercentage *= fEnvDamage;

            character_ApplyDefeatPenalty(e->pchar, fDebtPercentage);
        }

        victor = erGetEnt(e->pchar->erLastDamage);

        if( OnArenaMap() && victor)
            ArenaMapHandleKill(victor, e);

        if ( RaidIsRunning() && victor)
            RaidMapHandleKill(victor, e);

        stat_AddDeath(e->pl, victor);
        badge_RecordDefeat(e, victor);

        // Notify the AI that a player died
        if(victor)
        {
            AIEventNotifyParams params;
            victor->pchar->stats.iCntKills++;

            params.notifyType = AI_EVENT_NOTIFY_ENTITY_KILLED;
            params.source = victor;
            params.target = e;

            aiEventNotify(&params);

            if(ENTTYPE(victor)==ENTTYPE_PLAYER)
            {
                stat_AddKill(victor->pl, e);
                badge_RecordKill(victor, e, 1.f);

                ScriptPlayerDefeatedByPlayer(e, victor);
                ScriptTeamRewarded(e, victor);

                // Must happen before rep call
                if (server_visible_state.isPvP || OnArenaMap() || RaidIsRunning())
                {
                    pvp_GrantRewardTable(victor, e);
                }

                // Only give reputation in pvp zones and base raids
                // but add them to the defeat list in arenas
                if (server_visible_state.isPvP || OnArenaMap() || RaidIsRunning())
                {
                    pvp_RewardVictory(victor, e);
                    // I'm moving this into RewardVictory, so Defeat is recorded on all of victor's team members -Garth
                    //pvp_RecordDefeat(e, victor);
                }
            }

            if(server_visible_state.isPvP || RaidIsRunning())
            {
                pvp_LogPlayerDeath(e, victor);
            }

            LOG_ENT( victor, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, "RecordKill: Victim: \"%s\"\t%s,", dbg_NameStr(e),OnArenaMap()?"arena":"");

            sendVictoryMessages(NULL, victor, e);

        }
    }
    else
    {
        // Choose a victor.
        // Logically, the victor should be the person who dealt the most damage.
        // However, it is possible for that person to be offline if it is a long
        // battle.  So if the person who dealt the most damage is offline
        // (entity not available), then use the person who dealt the final blow.
        //
        // It should be impossible for the player that dealt the final blow
        // to be unavailable, since there is a quit timer.
        //

        Entity* victim = e;
        DamageTracker* dt;

        // Try to find the guy who dealt the most damage.
        dt = damageTrackerFindMaxDmg(victim->who_damaged_me);

        if(dt && erGetEnt(dt->ref))
        {
            // If possible, use the person who dealt the most damage as the victor.
            victor = erGetEnt(dt->ref);
        }
        else
        {
            // Otherwise, use the person who dealt the final blow as the victor.
            victor = erGetEnt(e->pchar->erLastDamage);
        }

        if(victor)
        {
            victor->pchar->stats.iCntKills++;

            if( !OnPetArenaMap() ) //1.18.06Changed per MMiller that nothing in pet arena should give badge credit
            {
                stat_AddKill(victor->pl, victim);
            }

            LOG_ENT( victor, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, "RecordKill Victim: \"%s\" (Lvl %d)", dbg_NameStr(victim), ((victim->pchar) ? (victim->pchar->iLevel) : -1));

            {
                AIEventNotifyParams params;

                params.notifyType = AI_EVENT_NOTIFY_ENTITY_KILLED;
                params.source = victor;
                params.target = victim;

                aiEventNotify(&params);
            }
        }

        sendVictoryMessages((dt ? erGetEnt(dt->ref) : NULL), erGetEnt(victim->pchar->erLastDamage), victim);

        // pet arena should give badge credit, also no pets on mission maker missions
        if( !OnPetArenaMap() && !(g_ArchitectTaskForce && e->erCreator) ) 
        {
            //    if they are on an AE tf, and the critter is a descaled AV
            //    hand out rewards as if they were an elite boss, not an AV
            if (g_ArchitectTaskForce)
            {
                if (e && e->villainDef)
                {
                    if (villainAE_isAVorHigher(e->villainDef->rank))
                    {
                        //    Designers wanted to make any AV that is less than or equal to 25 (combat level is base 0) give out elite boss level rewards
                        //    Also, any AV that is spawning below their designated level, will switch to elite boss rewards
                        int AV_LEVEL_CUTOFF = 25;
                        if (e->pchar)
                        {
                            if ((eaSize(&e->villainDef->levels) && (e->pchar->iCombatLevel < (e->villainDef->levels[0]->level-1) )) || 
                                (e->pchar->iCombatLevel < AV_LEVEL_CUTOFF))
                            {
                                e->villainRank = VR_ELITE;
                            }
                        }
                    }
                }
            }
            dispenseRewards(victim, eDeathBy, 0);
        }
    }

    if (e)
    {
        ScriptEntityDefeated(e, victor);
    }

    aiCritterDeathHandler(e);
    EncounterEntUpdateState(e, 0);
    MissionCountAllPlayers(1); // everyone gets a tick when something dies
}

////Send to all clients within a vis. region.  TO DO: this should be handled with resend_hp.
//possible bug if more ents a re near than LIST_SIZE.  Should be smarter
// JE: LIST_SIZE was 200, and caused a crash.  Arbitrarily made it MAX_ENTITIES_PRIVATE, since
//      that's the size of the static array in buildCloseEntList.  This still should be smarter.

#define LIST_SIZE   ( MAX_ENTITIES_PRIVATE )
#define RADIUS_TO_SEARCH ( 100 )

void serveDamage(Entity *victim, Entity *attacker, int new_damage, float *loc, bool wasAbsorb)
{
    int i=0, list[ LIST_SIZE ];
    bool sendloc = (loc!=NULL);

    if(new_damage==0)
        return;

    buildCloseEntList( NULL, victim, RADIUS_TO_SEARCH, list, LIST_SIZE, TRUE ); // true means send info to yourself

    while( list[i] != -1 && i < LIST_SIZE )
    {
        Entity  *e = entities[ list[i] ];

        if(ENTTYPE_BY_ID(list[i]) == ENTTYPE_PLAYER
            && e->client
            && e->client->ready >= CLIENTSTATE_REQALLENTS)
        {
            START_PACKET(pak1, e, SERVER_SEND_FLOATING_DAMAGE);
            pktSendBitsPack(pak1, 1, attacker?attacker->owner:0);
            pktSendBitsPack(pak1, 1, victim->owner);
            pktSendBitsPack(pak1, 1, new_damage); //flips sign on client now for more compact xmit.
            pktSendBits(pak1, 1, sendloc); // Using ent location
            if(sendloc)
            {
                pktSendF32(pak1,loc[0]);    // Quantize this a bit?
                pktSendF32(pak1,loc[1]);    // Quantize this a bit?
                pktSendF32(pak1,loc[2]);    // Quantize this a bit?
            }
            pktSendBits(pak1, 1, wasAbsorb);
            END_PACKET
        }

        i++;
    }
}

void serveDamageMessage(Entity *victim, Entity *attacker, char *msg, float *loc, bool wasAbsorb)
{
    #define LIST_SIZE   ( MAX_ENTITIES_PRIVATE )
    #define RADIUS_TO_SEARCH ( 100 )
    int i=0, list[ LIST_SIZE ];
    bool sendloc = (loc!=NULL);

    buildCloseEntList(NULL, victim, RADIUS_TO_SEARCH, list, LIST_SIZE, TRUE); // true means send info to yourself

    while( list[i] != -1 && i < LIST_SIZE )
    {
        Entity  *e = entities[ list[i] ];

        if(ENTTYPE_BY_ID(list[i]) == ENTTYPE_PLAYER
            && e->client
            && e->client->ready >= CLIENTSTATE_REQALLENTS)
        {
            START_PACKET(pak1, e, SERVER_SEND_FLOATING_DAMAGE);
            pktSendBitsPack(pak1, 1, attacker?attacker->owner:0);
            pktSendBitsPack(pak1, 1, victim->owner);
            pktSendBitsPack(pak1, 1, 0);
            pktSendString(pak1, msg);
            pktSendBits(pak1, 1, sendloc); // Using ent location
            if(sendloc)
            {
                pktSendF32(pak1,loc[0]);    // Quantize this a bit?
                pktSendF32(pak1,loc[1]);    // Quantize this a bit?
                pktSendF32(pak1,loc[2]);    // Quantize this a bit?
            }
            pktSendBits(pak1, 1, wasAbsorb);
            END_PACKET
        }

        i++;
    }
}

void serveFloater(Entity *e, Entity *entShownOver, const char *pch, float fDelay, EFloaterStyle style, U32 color)
{
    if (pch) {
        if (ENTTYPE(e)==ENTTYPE_PLAYER
            && e->client
            && e->client->ready >= CLIENTSTATE_REQALLENTS
            && stricmp(pch, "silent")!=0)
        {
            START_PACKET(pak1, e, SERVER_SEND_FLOATING_INFO);

            // TODO: This should probably not send strings
            //       It should send an ID instead.

            // SERVER_SEND_FLOATING_INFO
            // owner
            // string
            // style
            // delay in seconds

            pktSendBitsPack(pak1, 1, entShownOver->owner);
            pktSendString(pak1, pch);
            pktSendBitsPack(pak1, 2, style);
            pktSendF32(pak1, fDelay);
            pktSendBitsPack(pak1, 32, color);
            END_PACKET
        }
    }
}

void servePlaySound(Entity *e, const char *soundName, int channel, float volumeLevel)
{
    if (e && soundName && ENTTYPE(e)==ENTTYPE_PLAYER
        && e->client && e->client->ready >= CLIENTSTATE_REQALLENTS
        && stricmp(soundName, "silent")!=0)
    {
        START_PACKET(pak1, e, SERVER_SEND_PLAY_SOUND);

        pktSendString(pak1, soundName);
        pktSendBitsAuto(pak1, channel);
        pktSendF32(pak1, volumeLevel);
        END_PACKET
    }
}

void serveFadeSound(Entity *e, int channel, float fadeTime)
{
    if (e && fadeTime >= 0.0f && ENTTYPE(e)==ENTTYPE_PLAYER
        && e->client && e->client->ready >= CLIENTSTATE_REQALLENTS)
    {
        START_PACKET(pak1, e, SERVER_SEND_FADE_SOUND);

        pktSendBitsAuto(pak1, channel);
        pktSendF32(pak1, fadeTime);
        END_PACKET
    }
}

void serveFloatingInfoDelayed(Entity *e, Entity *entShownOver, const char *pch, float fDelay)
{
    serveFloater(e, entShownOver, pch, fDelay, kFloaterStyle_Info, 0);
}

void serveFloatingInfo(Entity *e, Entity *entShownOver, char *pch)
{
    serveFloater(e, entShownOver, pch, 0.0f, kFloaterStyle_Info, 0);
}

//------------------------------------------------------------------------------------------
// Client Desaturate Information
//------------------------------------------------------------------------------------------

void serveDesaturateInformation(Entity *e, float fDelay, float fCurrentValue, float fTargetValue, float fFadeTime)
{
    if (e 
        && ENTTYPE(e)==ENTTYPE_PLAYER
        && e->client)
    {
        START_PACKET(pak1, e, SERVER_SEND_DESATURATE_INFO);

        pktSendF32(pak1, fDelay);
        pktSendF32(pak1, fCurrentValue);
        pktSendF32(pak1, fTargetValue);
        pktSendF32(pak1, fFadeTime);
        END_PACKET
    }
}

//------------------------------------------------------------------------------------------
// Entity Damage Tracker
//------------------------------------------------------------------------------------------
DamageTracker* damageTrackerCreate()
{
    return calloc(1, sizeof(DamageTracker));
}

void damageTrackerDestroy(DamageTracker* dt)
{
    if(dt)
        free(dt);
}

#define MAX_PLAYER_DAMAGERS 20
void damageTrackerUpdate(Entity *victim, Entity *attacker, float new_damage)
{
    bool bPlayer = ENTTYPE(victim) == ENTTYPE_PLAYER;

    if((bPlayer || ENTTYPE(victim)==ENTTYPE_CRITTER) && new_damage>0)
    {
        DamageTracker* dt = NULL;   // This is the tracker we want to update.
        DamageTracker** dtlist = victim->who_damaged_me;
        int i;
        int dbidAttacker = (attacker && ENTTYPE(attacker)==ENTTYPE_PLAYER) ? attacker->db_id : 0;
        int iOldest = 0;

        // Don't track damage if both parties are allied players
        if (bPlayer && dbidAttacker && character_TargetIsFriend(victim->pchar,attacker->pchar))
            return;


        // Find the damage tracker we want to update.  Also watch for the oldest tracker
        for(i=eaSize(&dtlist)-1; i>=0; i-- )
        {
            if(erGetDbId(dtlist[i]->ref) == dbidAttacker)
            {
                dt = dtlist[i];
                break;
            }

            if (dtlist[i]->timestamp < dtlist[iOldest]->timestamp)
            {
                iOldest = i;
            }
        }

        // If a damage tracker for the attacker could not be found, this is
        // the first time the attacker is attacking the entity.
        if(!dt)
        {
            // If this is an NPC or a player with room left in the list, add the attacker
            if (!bPlayer || eaSize(&dtlist) < MAX_PLAYER_DAMAGERS)
            {
                dt = damageTrackerCreate();
                i = eaPush(&victim->who_damaged_me, dt);
            }
            else
            {
                // Replace the oldest attacker with this one
                dt = dtlist[iOldest];
                dt->damage = 0;
            }
        }

        dt->damage += new_damage;
        dt->damageOriginal = dt->damage;  // Reset original contribution when new contribution is made
        dt->timestamp = dbSecondsSince2000();

        if(dbidAttacker)
        {
            // Always refersh entity reference.
            // If a player disconnects and connects to the server, the old entity reference
            // would most likely become invalid.
            dt->ref = erGetRef(attacker);
            // Update the team in case of a team switch.
            dt->group_id = attacker->teamup_id;
        }
    }
}

void damageTrackerRegen(Entity *e, float fHealed)
{
    DamageTracker** dtlist = e->who_damaged_me;
    int i, iDead = 0;
    bool bDiscards = false;
    float fDamage = 0.0f;
    U32 now = dbSecondsSince2000();
    U32 tDecayAllowed = (g_DamageDecayConfig.iDecayDelay >= 0) ? (now - g_DamageDecayConfig.iDecayDelay) : 0;
    U32 tMinTime = (g_DamageDecayConfig.iFullDiscardDelay >= 0) ? (now - g_DamageDecayConfig.iFullDiscardDelay) : 0;
    float fMinDamage = (e->pchar) ? e->pchar->attrMax.fHitPoints * g_DamageDecayConfig.fDiscardDamage : 0.0;

    fHealed *= g_DamageDecayConfig.fRegenFactor;

    // Find the original damage we're allowing to decay, also mark full discards
    if(fMinDamage > 0.0f || tDecayAllowed || tMinTime)
    {
        for(i=eaSize(&dtlist)-1; i>=0; i--)
        {
            U32 t = dtlist[i]->timestamp;
            if(dtlist[i]->damage <= fMinDamage)
            {
                dtlist[i]->damage = 0;
                dtlist[i]->damageOriginal = 0;
                bDiscards = true;
            }
            else if(t <= tMinTime)
            {
                dtlist[i]->damage = 0;
                dtlist[i]->damageOriginal = 0;
                bDiscards = true;
            }
            else if(t <= tDecayAllowed)
            {
                fDamage += dtlist[i]->damageOriginal;
            }
        }
    }

    // Had decayable damage, and actually healed some
    if(fDamage > 0.0f && fHealed > 0.0f)
    {
        float fPortion = fHealed / fDamage; // Portion of decayable damage that we're discarding
        for(i=eaSize(&dtlist)-1; i>=0; i-- )
        {
            if(dtlist[i]->damage > 0.0f && dtlist[i]->timestamp <= tDecayAllowed)
            {
                float fLoss = fPortion * dtlist[i]->damageOriginal;
                dtlist[i]->damage -= fLoss;
                if(dtlist[i]->damage < fMinDamage)
                {
                    dtlist[i]->damage = 0;
                    dtlist[i]->damageOriginal = 0;
                    bDiscards = true;
                }
            }
        }
    }

    // Make a new list without the irrelevant trackers
    if(bDiscards && g_DamageDecayConfig.discardZeroDamagers )
    {
        DamageTracker** newlist;
        int j = eaSize(&dtlist);
        eaCreate(&newlist);
        for(i=0; i<j; i++ )
        {
            if(dtlist[i]->damage != 0)
            {
                eaPush(&newlist,dtlist[i]);
            }
            else
            {
                damageTrackerDestroy(dtlist[i]);
                dtlist[i] = NULL;
            }
        }
        eaDestroy(&dtlist);
        e->who_damaged_me = newlist;
    }
}

float damageTrackerFindEnvironmentalShare(Entity *e)
{
    DamageTracker** dtlist = e->who_damaged_me;
    int i;
    bool bDiscards = false;
    float fEnv = 0.0f, fTotal = 0.0f;

    for(i=eaSize(&dtlist)-1; i>=0; i--)
    {
        fTotal += dtlist[i]->damage;
        if(dtlist[i]->ref==0)
            fEnv += dtlist[i]->damage;
    }
    return (fTotal) ? fEnv / fTotal : 1.0f;
}

DamageTracker* damageTrackerFindMaxDmg(DamageTracker** trackers)
{
    int i;
    DamageTracker* maxDmgEntry = NULL;
    int maxDmgEntryCount = 0;

    // If the entity doesn't have a damage tracker list, it is impossible to
    // find out where who did the most damage to it.
    if(!trackers)
        return NULL;

    for(i = eaSize(&trackers)-1; i >= 0; i--)
    {
        DamageTracker* t = trackers[i];

        // Did we find someone who dealt more dmg to the victim?
        if(!maxDmgEntry || maxDmgEntry->damage < t->damage)
        {
            maxDmgEntry = t;
            maxDmgEntryCount = 0;
        }

        // How many people dealt the same amount of dmg to the victim?
        if(maxDmgEntry && maxDmgEntry->damage == t->damage)
        {
            maxDmgEntryCount++;
        }
    }
    if(!maxDmgEntry)
        return NULL;

    // How many people dealt identical amount of dmg?
    // If there are any identical entries at all, break the tie now.
    if(maxDmgEntryCount > 0)
    {
        int theChosenOne = rand() % (maxDmgEntryCount + 1); // JS: Hey... at least I didn't name it Neo...
        int maxDmgEntryCount = 0;

        theChosenOne++;
        for(i = eaSize(&trackers)-1; i >= 0; i--)
        {
            if(trackers[i]->damage == maxDmgEntry->damage)
            {
                maxDmgEntryCount++;
                if(maxDmgEntryCount == theChosenOne)
                {
                    maxDmgEntry = trackers[i];
                    break;
                }
            }
        }
    }

    return maxDmgEntry;
}
//------------------------------------------------------------------------------------------
// End Entity Damage Tracker
//------------------------------------------------------------------------------------------

void doneFall(Entity *e, F32 fall_height, F32 yvel)
{
    F32 y;
    F32 dmg = 0.0f;

    if( !( entity_state[e->owner] & ENTITY_IN_USE ) || !entAlive(e) || !e->pchar)
        return;

    // HACK: This whole function is a hack. basically, I just twiddled
    // with it until it seemed to work well.

    // The yvel isn't perfect. Sometimes it's very small if you've fallen a
    // long way and/or you're going very fast. So, the damage is based off
    // the height of the fall.

#define MAX_FALL_DAMAGE (1000.0f)
#define MAX_FALL_HEIGHT (1000.0f)
#define SAFE_FALL_HEIGHT (5)
#define SAFE_FALL_HEIGHT_BASE (20)
#define FALL_POWER (6.0f)

    y = fall_height - (SAFE_FALL_HEIGHT_BASE + SAFE_FALL_HEIGHT*e->pchar->attrCur.fJumpHeight);

    if(y>0)
    {
        // OK, you've fallen further than you can safely.
        // The damage you take is exponential, based on how far you fell.

        // I think around 1000 feet is the farthest you can fall.
        // (We keep you from flying any higher than 900 feet, at least.
        // I suppose the ground could be less than 0, though.)

        // Around 1800 hitpoints is the max you can have as a tanker
        // Around 800 hitpoints is the max you can have as a controller

        // Let's say, then, that falling 1000 feet does 1000 HP damage.
        // And it's exponential.

        dmg = powf(FALL_POWER, y/MAX_FALL_HEIGHT);
        dmg = ((dmg-1)/(FALL_POWER-1))*MAX_FALL_DAMAGE;
    }

    if(dmg > 0)
    {
        //New parameter that reduces falling damage for certain maps
        if( scene_info.fallingDamagePercent != 1.0 )
            dmg *= scene_info.fallingDamagePercent;
            

        if((e->pchar->attrCur.fHitPoints-dmg) < 1.0f)
        {
            dmg = e->pchar->attrCur.fHitPoints - 1.0f;
        }
        if(!e->noDamageOnLanding && !e->invincible)
        {
            e->pchar->attrCur.fHitPoints -= dmg;
        }
        else
        {
            e->noDamageOnLanding = 0;
        }

        damageTrackerUpdate(e, NULL, dmg);
        serveDamage(e, e, dmg, 0, 0);
    }
}


void setFlying(Entity *e, int is_flying)
{
    ClientLink *client;

    client = clientFromEnt( e );

    if (client)
    {
        const ServerControlState *stateNow = getLatestServerControlState(&client->controls);

        if(stateNow->fly != is_flying)
        {
            ServerControlState* state = getQueuedServerControlState(&client->controls);

            state->fly = is_flying;

            if(e->myMaster)
            {
                state = getQueuedServerControlState(&e->myMaster->controls);

                state->fly = is_flying;
            }

            if(client->controls.controldebug)
                printf("queued flight toggle: %d\n", is_flying);
        }
    }
}

void setStunned(Entity *e, int is_stunned)
{
    ClientLink *client;

    client = clientFromEnt( e );

    if (client)
    {
        const ServerControlState *stateNow = getLatestServerControlState(&client->controls);

        if(stateNow->stun != is_stunned)
        {
            ServerControlState* state = getQueuedServerControlState(&client->controls);

            state->stun = is_stunned;

            if(e->myMaster)
            {
                state = getQueuedServerControlState(&e->myMaster->controls);

                state->stun = is_stunned;
            }

            if(client->controls.controldebug)
                printf("queued stun toggle: %d\n", is_stunned);
        }
    }
}

void setJumppack(Entity *e, int jumppack_on)
{
    ClientLink *client;

    client = clientFromEnt( e );

    if (client)
    {
        const ServerControlState *stateNow = getLatestServerControlState(&client->controls);

        if(stateNow->jumppack != jumppack_on)
        {
            ServerControlState* state = getQueuedServerControlState(&client->controls);

            state->jumppack = jumppack_on;

            if(e->myMaster)
            {
                state = getQueuedServerControlState(&e->myMaster->controls);

                state->jumppack = jumppack_on;
            }

            if(client->controls.controldebug)
                printf("queued jumppack toggle: %d\n", jumppack_on);
        }
    }
}


void setSpecialMovement(Entity *e, int attrib, int on)
{
    ClientLink *client;

    client = clientFromEnt( e );

    if (client)
    {
        int state_now = 0;
        const ServerControlState *stateNow = getLatestServerControlState(&client->controls);

        if( attrib == kSpecialAttrib_Glide )
            state_now = stateNow->glide;
        if( attrib == kSpecialAttrib_NinjaRun )
            state_now = stateNow->ninja_run;
        if( attrib == kSpecialAttrib_Walk )
            state_now = stateNow->walk;
        if( attrib == kSpecialAttrib_BeastRun )
            state_now = stateNow->beast_run;
        if( attrib == kSpecialAttrib_SteamJump )
            state_now = stateNow->steam_jump;
        if( attrib == kSpecialAttrib_HoverBoard )
            state_now = stateNow->hover_board;
        if( attrib == kSpecialAttrib_MagicCarpet )
            state_now = stateNow->magic_carpet;
        if (attrib == kSpecialAttrib_ParkourRun)
            state_now = stateNow->parkour_run;

        if(state_now != on)
        {
            ServerControlState* state = getQueuedServerControlState(&client->controls);

            if( attrib == kSpecialAttrib_Glide )
                state->glide = on;
            if( attrib == kSpecialAttrib_NinjaRun )
                state->ninja_run = on;
            if( attrib == kSpecialAttrib_Walk )
                state->walk = on;
            if( attrib == kSpecialAttrib_BeastRun )
                state->beast_run = on;
            if( attrib == kSpecialAttrib_SteamJump )
                state->steam_jump = on;
            if( attrib == kSpecialAttrib_HoverBoard )
                state->hover_board = on;
            if( attrib == kSpecialAttrib_MagicCarpet )
                state->magic_carpet = on;
            if (attrib == kSpecialAttrib_ParkourRun)
                state->parkour_run = on;

            if(e->myMaster)
            {
                state = getQueuedServerControlState(&e->myMaster->controls);
                if( attrib == kSpecialAttrib_Glide )
                    state->glide = on;
                if( attrib == kSpecialAttrib_NinjaRun )
                    state->ninja_run = on;
                if( attrib == kSpecialAttrib_Walk )
                    state->walk = on;
                if( attrib == kSpecialAttrib_BeastRun )
                    state->beast_run = on;
                if( attrib == kSpecialAttrib_SteamJump )
                    state->steam_jump = on;
                if( attrib == kSpecialAttrib_HoverBoard )
                    state->hover_board = on;
                if( attrib == kSpecialAttrib_MagicCarpet )
                    state->magic_carpet = on;
                if (attrib == kSpecialAttrib_ParkourRun)
                    state->parkour_run = on;
            }

            if(client->controls.controldebug)
                printf("queued glide, ninja, beast or walk toggle: %d\n", on);
        }
    }
}


void setNoPhysics(Entity *e, int no_physics_on)
{
    ClientLink *client;

    client = clientFromEnt( e );

    if (client)
    {
        const ServerControlState *stateNow = getLatestServerControlState(&client->controls);

        if(stateNow->no_physics != no_physics_on)
        {
            ServerControlState* state = getQueuedServerControlState(&client->controls);
            state->no_physics = no_physics_on;

            if(e->myMaster)
            {
                state = getQueuedServerControlState(&e->myMaster->controls);

                state->no_physics = no_physics_on;
            }

            if(client->controls.controldebug)
                printf("queued no physics toggle: %d\n", no_physics_on);
        }
    }
}


void setHitStumble(Entity *e, int hit_stumble_on)
{
    ClientLink *client;

    client = clientFromEnt( e );

    if (client)
    {
        //const ServerControlState *stateNow = getLatestServerControlState(&client->controls);

        //if(stateNow->hit_stumble != hit_stumble_on)
        {
            ServerControlState* state = getQueuedServerControlState(&client->controls);
            state->hit_stumble = hit_stumble_on;

            if(e->myMaster)
            {
                state = getQueuedServerControlState(&e->myMaster->controls);

                state->hit_stumble = hit_stumble_on;
            }

            if(client->controls.controldebug)
                printf("queued hit stumble: %d\n", hit_stumble_on);
        }
    }
}


void doHitStumble( Entity * e, F32 iAmt )
{
    if( ENTTYPE(e) == ENTTYPE_PLAYER )
    {
        setHitStumble( e, 1 );
    }
    else 
    {
        e->motion->hit_stumble = 1;
        e->motion->hitStumbleTractionLoss = 1.0; //TO DO base on strength of hit
    }
    
}

// these are modifiers to the surface flags, so setting them to 1 means default
void setSurfaceModifiers(Entity *e,SurfParamIndex surf_type,float traction, float friction, float bounce, float gravity)
{
    ClientLink        *client;

    client = clientFromEnt( e );

    if (client)
    {
        ServerControlState* state = getQueuedServerControlState(&client->controls);

        state->surf_mods[surf_type].traction = traction;
        state->surf_mods[surf_type].friction = friction;
        state->surf_mods[surf_type].bounce = bounce;
        state->surf_mods[surf_type].gravity = gravity;

        if(client->controls.controldebug)
        {
            printf(    "queued SCS %d: control %f, friction %f, bounce %f, gravity %f\n",
                    state->id,
                    traction,
                    friction,
                    bounce,
                    gravity);
        }

        if(e->myMaster)
        {
            state = getQueuedServerControlState(&e->myMaster->controls);

            state->surf_mods[surf_type].traction = traction;
            state->surf_mods[surf_type].friction = friction;
            state->surf_mods[surf_type].bounce = bounce;
            state->surf_mods[surf_type].gravity = gravity;
        }
    }
    else
    {
        MotionStateInput* input = &e->motion->input;
        
        input->surf_mods[surf_type].traction = traction;
        input->surf_mods[surf_type].friction = friction;
        input->surf_mods[surf_type].bounce = bounce;
        input->surf_mods[surf_type].gravity = gravity;
    }
}

void setJumpHeight(Entity* e, float height)
{
    ClientLink *client;

    client = clientFromEnt( e );

    if (client)
    {
        const ServerControlState* oldState = getLatestServerControlState(&client->controls);
        ServerControlState* state;

        if(oldState->jump_height != height)
        {
            state = getQueuedServerControlState(&client->controls);

            state->jump_height = height;

            if(client->controls.controldebug)
                printf("queued jump height: %f\n", height);
        }

        if(e->myMaster)
        {
            oldState = getLatestServerControlState(&e->myMaster->controls);

            if(oldState->jump_height != height)
            {
                state = getQueuedServerControlState(&e->myMaster->controls);

                state->jump_height = height;
            }
        }
    }
}

void setSpeed( Entity *e,SurfParamIndex surf_type, float fSpeedScale)
{
    float z_speed = fSpeedScale * BASE_PLAYER_FORWARDS_SPEED;
    ClientLink *client;

    client = clientFromEnt( e );

    if( client )
    {
        const ServerControlState* latest_state = getLatestServerControlState(&client->controls);

        if(e->unstoppable && fSpeedScale < 1)
        {
            z_speed = BASE_PLAYER_FORWARDS_SPEED;
        }

        if(latest_state->surf_mods[surf_type].max_speed != z_speed)
        {
            ServerControlState* state = getQueuedServerControlState(&client->controls);

            state->speed[0]   = BASE_PLAYER_SIDEWAYS_SPEED;
            state->speed[1]   = BASE_PLAYER_VERTICAL_SPEED;
            state->speed[2]   = -10;
            state->surf_mods[surf_type].max_speed = z_speed;
            state->speed_back = BASE_PLAYER_BACKWARDS_SPEED;
        }
    }
    else
    {
        // MS: Put stuff here for non-player entities.  Apply directly to MotionState instead of queueing.

        e->motion->input.surf_mods[surf_type].max_speed = z_speed;
    }
}

void setEntCollision(Entity *e, int set)
{
    ClientLink *client = clientFromEnt(e);

    if(client)
    {
        const ServerControlState* scs = getLatestServerControlState(&client->controls);

        if(set != scs->no_ent_collision)
        {
            ServerControlState* scs = getQueuedServerControlState(&client->controls);

            scs->no_ent_collision = set;
        }
    }
    else
    {
        MotionState* motion = e->motion;
        
        if(motion->input.no_ent_collision != set)
        {
            motion->input.no_ent_collision = set;
            e->collision_update = 1;
        }
    }
}

void setEntNoSelectable(Entity *e, int set)
{
    e->notSelectable = set ? 1 : 0;
    e->collision_update = 1;
}

void setNoJumpRepeat(Entity *e, int set)
{
    ClientLink *client = clientFromEnt(e);

    if(set)
        set = 1;

    if(client)
    {
        const ServerControlState* scs = getLatestServerControlState(&client->controls);

        if(set != scs->no_jump_repeat)
        {
            ServerControlState* scs = getQueuedServerControlState(&client->controls);

            scs->no_jump_repeat = set;
        }
    }
}

//#####################################################################################
//move elsewhere
//When you want a list of nearby entities who need to be notified or affected by a power event
// (shouldn't this be more general)
//mw This function's parameters are really weird, clean it up sometime
//mw It this done more efficiently somewhere else?
//mak - changing to use entity grid (more efficient)
void buildCloseEntList( Entity  *entToskip, Entity *centerEnt, F32 dist, int *list, int list_len, int include_centerEnt )
{
    int i;
    int cnt;
    int entArray[MAX_ENTITIES_PRIVATE];
    int entCount;
    //extern Grid ent_grid;
    int max;


    // search for points within distance

    entCount = mgGetNodesInRange(0, ENTPOS(centerEnt), (void **)entArray, 0);

    //entCount = gridFindObjectsInSphere( &ent_grid,
    //                                    (void **)entArray,
    //                                    ARRAY_SIZE(entArray),
    //                                    centerEnt->mat[3],dist);

    // Convert to ent pointers and test conditions
    cnt = 0;
    // Need to "null" terminate the list!
    if (include_centerEnt) {
        max = list_len - 2;
    } else {
        max = list_len - 1;
    }
    for(i = 0; i < entCount && cnt < max; i++) {
        int idx = entArray[i];
        Entity* proxEnt = entities[idx];
        EntType entType = ENTTYPE_BY_ID(idx);

        if(     entToskip != proxEnt                //used to skip attacker
            &&  centerEnt != proxEnt
            &&  ent_close( dist, ENTPOS(centerEnt), ENTPOS(proxEnt) )
            &&  (   entType == ENTTYPE_NPC ||
                    entType == ENTTYPE_PLAYER ||
                    entType == ENTTYPE_CRITTER )
            &&  entAlive( proxEnt )
            )
        {
            list[cnt++]=proxEnt->owner;
        }
    }

    if( include_centerEnt )
        list[cnt++]=centerEnt->owner;

    list[cnt] = -1;
}

//######## Stuff for AI or entsend only ##################

//TO DO where does this go????
void setClientState(Entity  *e, int state )
{
    Supergroup *sg = e->supergroup;
    int i;
    bool baseGurney = false;
    
    // TODO for destructible base gurnies
    if (g_isBaseRaid)
    {
        // set baseGurney true or false based on whether there are any
        // gurnies still functional in e's base.
    }
    /* else */if (sg)
    {
        StaticMapInfo *info = staticMapInfoFind(e->static_map_id);
        const MapSpec* mapSpec = info?MapSpecFromMapName(info->name):NULL;

        // Do not let players teleport out of tutorial zones, or out of maps that disabled it, or missions that disable it
        if ((info && !info->introZone) && (mapSpec && !mapSpec->disableBaseHospital) && MissionAllowBaseHospital())
        {
            for (i = eaSize(&sg->specialDetails) - 1; i >= 0; i--)
            {
                if (sg->specialDetails[i] && sg->specialDetails[i]->pDetail &&
                    sg->specialDetails[i]->iFlags & DETAIL_FLAGS_IN_BASE && 
                    sg->specialDetails[i]->iFlags & DETAIL_FLAGS_ACTIVATED && 
                    sg->specialDetails[i]->pDetail->eFunction == kDetailFunction_Medivac &&
                    !areMembersAlignmentMixed(e->teamup ? &e->teamup->members : NULL))
                {
                    baseGurney = true;
                }
            }
        }
    }
    START_PACKET( pak1, e, SERVER_SET_CLIENT_STATE );
    pktSendBitsPack( pak1, 1, state );

    pktSendBitsPack(pak1, 1, baseGurney);
    END_PACKET
}

//
//
//
void modStateBits( Entity *e, EntMode mode_num, int timer, int on )
{
    int bit = ( 1 << mode_num );

    if( on )
        e->state.mode |= bit;
    else
        e->state.mode &= ( bit ^ 0xffffffff );

    e->state.timer[ mode_num ] = timer;

    //if(ENTTYPE(e) == ENTTYPE_PLAYER && mode_num == ENT_DEAD && on && e->invincible)
    //    assertmsg(0, "Killing invincible player");
}

//
//
//
int decModeTimer( Entity *e, EntMode mode_num )
{
    e->state.timer[ mode_num ] -= global_state.frame_time_30;

    if( e->state.timer[ mode_num ] <= 0 )
    {
        e->state.timer[ mode_num ] = 0;
        modStateBits( e, mode_num, 0, FALSE );
        return TRUE;
    }

    return FALSE;
}




typedef struct ClosestBeaconSearchData
{
    float minDistSQR;
    Beacon * beacon;
    Vec3 pos;
} ClosestBeaconSearchData;

void chooseClosestBeacon(Array* beaconArray, void* userData)
{
    ClosestBeaconSearchData * pData = (ClosestBeaconSearchData*) userData;
    int i;
    int count = beaconArray->size;
    Beacon** beacons = (Beacon**)beaconArray->storage;

    for(i = 0; i < count; i++)
    {
        Beacon* beacon = beacons[i];
        float distSQR = distance3Squared(pData->pos, posPoint(beacon));

        if(       distSQR < pData->minDistSQR
            && distSQR > (MIN_TELEPORT_DISTANCE * MIN_TELEPORT_DISTANCE))
        {
            pData->minDistSQR    = distSQR;
            pData->beacon        = beacon;
        }
    }
}



// Used to place character near teleport target
// If no beacon is nearby, will place player directly on target
void setInitialTeleportPosition(Entity * e, Vec3 targetPos, bool exact)
{
    ClosestBeaconSearchData data;
    data.beacon = 0;
    data.minDistSQR = (MAX_TELEPORT_DISTANCE * MAX_TELEPORT_DISTANCE);
    copyVec3(targetPos, data.pos);

    if (!exact) // Don't do a beacon search if we need an exact position
        beaconForEachBlock(targetPos, MAX_TELEPORT_DISTANCE, MAX_TELEPORT_DISTANCE, MAX_TELEPORT_DISTANCE, chooseClosestBeacon, &data);

    if (data.beacon)
    {
        Vec3 pos;
        copyVec3(posPoint(data.beacon), pos);
        pos[1] -= beaconGetFloorDistance(data.beacon);    // drop player to the ground

        entUpdatePosInstantaneous(e, pos);
        character_FaceLocation(e->pchar, targetPos);
    }
    else
    {
        // place entity directly on target
        entUpdatePosInstantaneous(e, targetPos);
    }
}

//TO DO : Is this the right place to park this?
void setSayOnClick( Entity * e, const char * text )
{    
    if (!e) return;

    if( e->sayOnClick )
    {
        free(e->sayOnClick );
        e->sayOnClick = NULL;
    }
    if (text && text[0])
        e->sayOnClick = strdup( text );
}

//TO DO : Is this the right place to park this?
void setRewardOnClick( Entity * e, const char * text )
{    
    if (!e) return;

    if( e->rewardOnClick )
    {
        free(e->rewardOnClick );
        e->rewardOnClick = NULL;
    }
    if (text && text[0])
        e->rewardOnClick = strdup( text );
}


//TO DO : Is this the right place to park this?
void setSayOnKillHero( Entity * e, const char * text )
{    
    if (!e) return;

    if( e->sayOnKillHero )
    {
        free(e->sayOnKillHero );
        e->sayOnKillHero = NULL;
    }
    if (text && text[0])
        e->sayOnKillHero = strdup( text );
}

void addOnClickCondition( Entity *e, OnClickConditionType type, const char *condition, const char *say, const char *reward)
{
    OnClickCondition *newCondition = malloc(sizeof(OnClickCondition));
    memset(newCondition, 0, sizeof(OnClickCondition));

    newCondition->type = type;
    if (condition)
        newCondition->condition = strdup (condition);
    if (say)
        newCondition->say = strdup (say);
    if (reward)
        newCondition->reward = strdup (reward);

    if (e->onClickConditionList == NULL)
        eaCreate(&e->onClickConditionList);

    eaPush(&e->onClickConditionList, newCondition);
}

void clearallOnClickCondition( Entity *e )
{
    if( e->onClickConditionList ) {
        eaDestroyEx(&e->onClickConditionList, OnClickCondition_Destroy);
    }
}

void OnClickCondition_Destroy(void* condData)
{
    OnClickCondition* cond = (OnClickCondition*)condData;
    if (cond)
    {
        free(cond->condition);
        free(cond->say);
        free(cond->reward);
    }
}
 
bool OnClickCondition_Check(Entity *e, Entity *target, char **say, char **reward)
{
    int i;
    int count = eaSize(&e->onClickConditionList);
 
    for (i = 0; i < count; i++) {
        switch (e->onClickConditionList[i]->type) {
            case ONCLICK_HAS_REWARD_TOKEN:
                if (getRewardToken(target, e->onClickConditionList[i]->condition)) {
                    if (e->onClickConditionList[i]->say)
                        *say = strdup(e->onClickConditionList[i]->say);
                    if (e->onClickConditionList[i]->reward)
                        *reward = strdup(e->onClickConditionList[i]->reward);
                    return true;
                }
                break;
            case ONCLICK_HAS_BADGE:
                if (badge_OwnsBadge(target, e->onClickConditionList[i]->condition)) {
                    if (e->onClickConditionList[i]->say)
                        *say = strdup(e->onClickConditionList[i]->say);
                    if (e->onClickConditionList[i]->reward)
                        *reward = strdup(e->onClickConditionList[i]->reward);
                    return true;
                }
                break;
            case ONCLICK_DEFAULT:
                if (e->onClickConditionList[i]->say)
                    *say = strdup(e->onClickConditionList[i]->say);
                if (e->onClickConditionList[i]->reward)
                    *reward = strdup(e->onClickConditionList[i]->reward);
                return true;
                break;
            default:
                // don't know what this is?
                break;
        }
    }
    return false;
}

