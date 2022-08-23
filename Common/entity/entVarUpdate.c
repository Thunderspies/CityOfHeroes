#include <utilitieslib/utils/wininclude.h>
#include <stdio.h>
#include <utilitieslib/assert/assert.h>
#include "entity.h"
#include "entity/entGameActions.h"
#include "seq/seq.h"
#include "varutils.h"
#include "entVarUpdate.h"
#include "combat/combat.h"
#include "ai/entai.h"
#include <utilitieslib/utils/timing.h>
#include "gameComm/trading.h"
#include "player/pmotion.h"
#include "entity/entserver.h"
#include "character_base.h"
#include "character_tick.h"
#include "comm_game.h"
#include "kiosk.h"
#include "gameData/store.h"
#include "player/badges_server.h"
#include "gameSys/arenamap.h"
#include "door.h"
#include "entPlayer.h"
#include "seq/seqstate.h"
#include "cmdparse/cmdserver.h"
#include "bases/bases.h"
#include "bases/basedata.h"
#include "bases/basesystems.h"
#include "entity/SgrpServer.h"
#include "bases/baseserver.h"
#include "gameSys/DayJob.h"
#include "storyarc/playerCreatedStoryarcServer.h"
#include "entity/pvp.h"
#include "entity/alignment_shift.h"
#include "storyarc/contactdef.h"
#include "storyarc/pnpcCommon.h"
#if SERVER
#include "storyarc/pnpc.h"
#include "MARTY.h"
#endif

extern void updateExpireables(Entity *e);

static void HandleEntityDeath( Entity *e )
{
    if( entMode( e, ENT_DEAD ) )
    {
        if(ENTTYPE(e) == ENTTYPE_PLAYER && OnArenaMap() && !e->pl->arenaDeathProcessed)
        {
            ArenaMapHandleDeath(e);
        }

        play_bits_anim1( e->seq->state, STATE_DEATH, STATE_JUSTFUCKINDOIT, -1 );

        if(e->idDetail!=0)
        {
            // This is a base object
            BaseRoom *room = baseGetRoom(&g_base, e->idRoom);
            if(room)
            {
                RoomDetail *detail = roomGetDetail(room, e->idDetail);
                if(detail && detail->e == e)
                {
                    detailSetDestroyed(detail,true,false,true);
                }
                else
                    entFree(e);
            }
            else
                entFree(e);
        }
        if( ENTTYPE(e) != ENTTYPE_PLAYER && decModeTimer( e, ENT_DEAD ) )
        {
            //Non players lay dead for a little bit, then are removed from the world.
            //This triggers a rebuild in destroyed mode for Base Objects
            //Players stay dead until something brings them back to life.

            entFree( e ); //Getting here means we just finished lying dead, and need to be removed
        }
    }
}

void updateGameStateForEachEntity(float fRate){
    static int iKiosk;
    int    i;
    Entity * e;
    float fCharacterTick = 0.0f;
    bool fCharacterTickGTZero;
    int doStuffToMe[ENTTYPE_COUNT];

    static float fTimePassed = 0.0;

    fTimePassed += fRate;
    if(fTimePassed > COMBAT_UPDATE_INTERVAL)
    {
        iKiosk++;
        fCharacterTick = fTimePassed;
        fTimePassed = 0.0f;
    }
    fCharacterTickGTZero = (fCharacterTick>0.0f);

    memset(doStuffToMe, 0, sizeof(doStuffToMe));

    doStuffToMe[ENTTYPE_CRITTER] = 1;
    doStuffToMe[ENTTYPE_PLAYER] = 1;
    doStuffToMe[ENTTYPE_HERO] = 1;

    if (server_state.enableSpikeSuppression)
    {
        PERFINFO_AUTO_START("TickUpdateTargets", 1);
        pvp_UpdateTargetTracking(timerSeconds64 (timerCpuTicks64()));
        PERFINFO_AUTO_STOP();
    }

    PERFINFO_AUTO_START("TickPhaseOneLoop", 1);
    for(i=1;i<entities_max;i++)
    {
        e = entities[i];
        if( entity_state[i] & ENTITY_IN_USE )
        {
            
            if( e &&
                doStuffToMe[ENTTYPE_BY_ID(i)] &&
                !ENTSLEEPING_BY_ID(i) &&
                !ENTPAUSED_BY_ID(i) &&
                e->ready &&
                !e->doorFrozen)
            {
                if(character_IsInitialized(e->pchar))
                {
                    if(fCharacterTickGTZero && (!server_state.viewCutScene || e->cutSceneActor ))
                    {
                        PERFINFO_AUTO_START("TickPhaseOne", 1);
                        character_TickPhaseOne(e->pchar, fCharacterTick);
                        PERFINFO_AUTO_STOP_CHECKED("TickPhaseOne");

                        // Get rid of the lingering kiosk/store window, if necessary
                        // And check to see if the special title is still good.
                        if(ENTTYPE_BY_ID(i)==ENTTYPE_PLAYER && iKiosk%(int)(2.0f/COMBAT_UPDATE_INTERVAL)==0)
                        {
                            kiosk_Tick(e);
                            store_Tick(e);
                            badge_Tick(e);
                            trade_tick(e);
                            sgroup_Tick(e);
                            base_Tick(e);
                            dayjob_Tick(e);
                            updateExpireables(e);
                            alignmentshift_UpdateAlignment(e);
                            StoryArc_Tick(e);
                            MARTY_Tick(e);
                        }
                    }
                    else
                    {
                        PERFINFO_AUTO_START("OffTick", 1);
                        character_OffTick(e->pchar, fCharacterTick);
                        PERFINFO_AUTO_STOP();
                    }

                    
                }

                // This might make the e pointer go bad.
                HandleEntityDeath(e);
            }
        }
    }

    if(fCharacterTickGTZero && iKiosk%(int)(2.0f/COMBAT_UPDATE_INTERVAL)==0)
    {
        // Let the badge system know that we're done with all the players.
        badge_TicksComplete();
    }

    PERFINFO_AUTO_STOP();

    if (fCharacterTickGTZero) {
        PERFINFO_AUTO_START("TickPhaseTwoLoop", 1);
        for(i=1;i<entities_max;i++)
        {
            if( entity_state[i] & ENTITY_IN_USE )
            {
                e = entities[i];

                if (e->bitfieldVisionPhasesLastFrame != e->bitfieldVisionPhases
                    || e->exclusiveVisionPhaseLastFrame != e->exclusiveVisionPhase)
                {
                    e->status_effects_update = true;
                }
                e->bitfieldVisionPhasesLastFrame = e->bitfieldVisionPhases;
                e->exclusiveVisionPhaseLastFrame = e->exclusiveVisionPhase;

                e->fMagnitudeOfBestVisionPhase = 0.0f;

                if (e->seq && e->seq->type && e->seq->type->collisionType == SEQ_ENTCOLL_LIBRARYPIECE)
                {
                    if (e->bitfieldVisionPhasesDefault != 1)
                        Errorf("Warning:  Ents that use Geo collision cannot have VisionPhases.");

                    if (e->exclusiveVisionPhase != 0 && e->exclusiveVisionPhase != 1)
                        Errorf("Warning:  Ents that use Geo collision cannot have ExclusiveVisionPhases.");

                    e->bitfieldVisionPhases = 0xffffffff;
                    e->exclusiveVisionPhase = EXCLUSIVE_VISION_PHASE_ALL;
                }
                else
                {
                    e->bitfieldVisionPhases = e->bitfieldVisionPhasesDefault;
                    e->exclusiveVisionPhase = e->exclusiveVisionPhaseDefault;
                }

                if( e &&
                    doStuffToMe[ENTTYPE_BY_ID(i)] &&
                    !ENTSLEEPING_BY_ID(i) &&
                    !ENTPAUSED_BY_ID(i) &&
                    (!server_state.viewCutScene || e->cutSceneActor ) &&
                    e->ready &&
                    !e->doorFrozen &&
                    character_IsInitialized(e->pchar))
                {
                    PERFINFO_AUTO_START("TickPhaseTwo", 1);
                    character_TickPhaseTwo(e->pchar, fCharacterTick);
                    PERFINFO_AUTO_STOP_CHECKED("TickPhaseTwo");
                }
            }
        }
        PERFINFO_AUTO_STOP();
    }
}
