#include "cmdparse/cmdcommon.h"
#include "entity/entity.h"
#include "entity/entPlayer.h"
#include "player/pmotion.h"
#include <utilitieslib/utils/memcheck.h>
#include "entity/motion.h"
#include "graphics/font.h"
#include "player/player.h"
#include "entity/character_base.h"
#include "cmdparse/cmdcontrols.h"
#include "group/grouputil.h"
#include "cmdparse/cmdcommon.h"
#include "group/group.h"
#include "group/groupgrid.h"
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/error.h>
#include "seq/seqstate.h"
#include <utilitieslib/utils/mathutil.h>
#include "seq/seq.h"
#include <utils/position.h>
#include <utilitieslib/utils/file.h>
#include <utilitieslib/components/StashTable.h>
#include "storyarc/playerCreatedStoryarcValidate.h"
#include "group/groupProperties.h"
#include <utilitieslib/components/EString.h>

#if CLIENT
    #include "cmdparse/cmdgame.h"
    #include "graphics/FX/fx.h"
    #include "UI/uiArenaJoin.h"
    #include "graphics/sun.h"
    #include "UI/uiPopHelp.h"
#endif

#if SERVER
    #include "cmdparse/cmdserver.h"
    #include "svr/svr_base.h"
    #include "comm_game.h"
    #include "door.h"
    #include "entity/entGameActions.h"
    #include "ai/entai.h"
    #include "player/badges_server.h"
    #include "gameSys/arenamapserver.h"
    #include "generator/encounter.h"
    #include "entity/entserver.h"
    #include "bases/basesystems.h"
    #include "script/scriptengine.h"
    #include "storyarc/task.h"
    #include "entity/powers.h"
#endif

GlobalPlayerMotionState global_pmotion_state;
StashTable g_htAttackVolumes = NULL;
#ifdef CLIENT
char *g_activeMaterialTrackerName = NULL;
#endif

void MouseSpringInputVel(Vec3 vel, F32 max_speed); // in player.c

const char* pmotionGetBinaryControlName(ControlId id)
{
    static const char* binaryControlNames[CONTROLID_BINARY_MAX] = {
        "FORWARD",
        "BACKWARD",
        "LEFT",
        "RIGHT",
        "UP",
        "DOWN",
    };
    
    if(id >= 0 && id < CONTROLID_BINARY_MAX)
        return binaryControlNames[id];
        
    return "UNKNOWN";
}

void pmotionUpdateControlsPrePhysics(Entity* player, ControlState* controls, int curTime)
{
    int i;
    int oneSet = player->motion->jumping;

    //printf("Processing controls: %1.3f\n", timestep_acc);

    // Figure out if any of the movement keys has been held down for more than 250ms.
    
    for(i = 0; !oneSet && i < CONTROLID_BINARY_MAX; i++)
    {
        oneSet = controls->dir_key.total_ms[i] >= 250;
    }

    // Update the hold time for all keys.

    for(i = 0; i < CONTROLID_BINARY_MAX; i++)
    {
        if(controls->controls_input_ignored_this_csc && !controls->alwaysmobile)
        {
            controls->dir_key.total_ms[i] = 0;
        }
        else if(controls->dir_key.down_now & (1 << i) && !(controls->dir_key.down_now & (1 << opposite_control_id[i])))
        {
            U16 add_ms = curTime - controls->dir_key.start_ms[i];
            
            controls->dir_key.total_ms[i] += add_ms;

            if(oneSet && controls->dir_key.total_ms[i] < 250)
                controls->dir_key.total_ms[i] = 250;
            else if(controls->dir_key.total_ms[i] > 1000)
                controls->dir_key.total_ms[i] = 1000;

            if(controls->controldebug)
            {
                #if CLIENT
                    if(controls->controldebug & 1)
                    {
                        printf(    "\nAdding: %s += \t%dms (%dms total)\n",
                                pmotionGetBinaryControlName(i),
                                add_ms,
                                controls->dir_key.total_ms[i]);
                    }
                #else
                    printf(    "\nAdding: %s += \t%dms(%dms total) [%d,%d]\n",
                            pmotionGetBinaryControlName(i),
                            add_ms,
                            controls->dir_key.total_ms[i],
                            curTime,
                            controls->dir_key.start_ms[i]);
                #endif
            }
        }
    }
}

void pmotionUpdateControlsPostPhysics(ControlState* controls, int curTime)
{
    int i;

    // Restart key-hold timers.
    
    for(i = 0; i < CONTROLID_BINARY_MAX; i++)
    {
        if(!(controls->dir_key.down_now & (1 << i)) || controls->dir_key.down_now & (1 << opposite_control_id[i]))
        {
            // Key isn't held, or both key and its opposite are held, so clear the timer.

            controls->dir_key.total_ms[i] = 0;
        }

        controls->dir_key.start_ms[i] = curTime;
    }

    // Turn on any bits that weren't affected but don't match the currently held keys.
    
    controls->dir_key.use_now |= (controls->dir_key.down_now ^ controls->dir_key.use_now) & ~controls->dir_key.affected_last_frame;
    
    controls->dir_key.affected_last_frame = 0;
}

void pmotionSetState(Entity* e, ControlState* controls)
{
    S32                    input_ms[3]; // Millisecond input differentials in each direction.
    U32*                state = e->seq->state;
    MotionState*        motion = e->motion;
    ServerControlState*    scs = controls->server_state;
    int                    swap = controls->yaw_swap_timer > 1;
    int                    dir_key_down = 0;
    int                    i;
    int                    jumppackbit;
    
    // All these bits need to be reset here because physics doesn't run every tick, but states get cleared.

    #define SET_BIT(bit) SETB(state,bit)
    #define SET_BIT_IF(bit,condition) if(condition)SETB(state, bit)
    
    SET_BIT_IF(STATE_FLY,            scs->fly);
    SET_BIT_IF(STATE_STUN,            scs->stun);
//    SET_BIT_IF(STATE_JETPACK,        scs->jumppack);
    SET_BIT_IF(STATE_ICESLIDE,        scs->glide);
    SET_BIT_IF(STATE_NINJA_RUN,        scs->ninja_run);
    SET_BIT_IF(STATE_PLAYERWALK,    scs->walk);
    SET_BIT_IF(STATE_BEAST_RUN,        scs->beast_run);
    SET_BIT_IF(STATE_STEAM_JUMP,    scs->steam_jump);
    SET_BIT_IF(STATE_HOVER_BOARD,    scs->hover_board);
    SET_BIT_IF(STATE_MAGIC_CARPET,    scs->magic_carpet);
    SET_BIT_IF(STATE_PARKOUR_RUN,    scs->parkour_run);

    SET_BIT_IF(STATE_JUMP,            motion->jumping);
    SET_BIT_IF(STATE_SLIDING,        motion->sliding);
    SET_BIT_IF(STATE_BOUNCING,        motion->bouncing);

    jumppackbit = seqGetStateNumberFromName("PRESTIGEJUMPJETPACK");;
    if (scs->jumppack && jumppackbit >= 0)
        SETB(state, jumppackbit);

    if(motion->falling)
    {
        SET_BIT(STATE_AIR);

        if(!motion->sliding && motion->vel[1] < -0.6)
        {
            if(motion->heightILastTouchedTheGround - ENTPOSY(e) > 3.5)
                SET_BIT(STATE_BIGFALL); 
        }
    }
    else if(controls->recover_from_landing_timer > 0)
    {
        SET_BIT(STATE_HEADPAIN);
    }

    // If I'm not in control of my character, then don't set
    //   any of the bits that are directly based on input controls.

    if(    !controls->nocoll &&
        !controls->alwaysmobile &&
        (    scs->controls_input_ignored ||
            controls->recover_from_landing_timer ||
            controls->controls_input_ignored_this_csc))
    {
        return;
    }
 
    // Calculate the ms time differential for each direction.
    
    input_ms[0] =    controls->dir_key.total_ms[CONTROLID_RIGHT] -
                    controls->dir_key.total_ms[CONTROLID_LEFT];
                    
    input_ms[1] =    controls->dir_key.total_ms[CONTROLID_UP] -
                    controls->dir_key.total_ms[CONTROLID_DOWN];

    input_ms[2]    =    controls->dir_key.total_ms[CONTROLID_FORWARD] -
                    controls->dir_key.total_ms[CONTROLID_BACKWARD];

    // Set right/left bits.
    
    if(!controls->no_strafe && !e->seq->type->noStrafe)
    {
        if(controls->last_inp_vel[0] != 0)
        {
            dir_key_down = 1;

            if(controls->last_inp_vel[0] > 0)
            {
                SET_BIT(STATE_STEPRIGHT);
            }
            else
            {
                SET_BIT(STATE_STEPLEFT);
            }
        }
    }
    else if(controls->last_inp_vel[0] && controls->last_inp_vel[2] >= 0)
    {
        dir_key_down = 1;

        if(!swap)
        {
            SET_BIT(STATE_FORWARD);

            //if(controls->pyr.cur[0] < 0){
            //    SET_BIT(STATE_DIVE);
            //}
        }
        else
        {
            SET_BIT(STATE_BACKWARD);
        }
    }
    
    // Set fly bits.

    SET_BIT_IF(STATE_JUMP, input_ms[1] > 0 && scs->fly);

    // Set forward/backward bits.
    
    if(controls->last_inp_vel[2] != 0)
    {
        F32 value = swap ? -controls->last_inp_vel[2] : controls->last_inp_vel[2];
        
        dir_key_down = 1;
            
        if(value > 0)
        {
            SET_BIT(STATE_FORWARD);
            
            //if(controls->pyr.cur[0] < 0){
            //    SET_BIT(STATE_DIVE);
            //}
        }
        else
        {
            SET_BIT(STATE_BACKWARD);
        }
    }
    
    // Set dev-mode bits.
    
    if(isDevelopmentMode())
    {
        for(i=0;i<10;i++)
        {
            seqOrState(state,controls->anim_test[i],STATE_TEST0+i);
        }
    }

    #undef SET_BIT_IF
    #undef SET_BIT
}

void pmotionSetVel(Entity *e,ControlState *controls, F32 inp_vel_scale)
{
    Vec3                vel = {0,0,0};
    MotionState*        motion = e->motion;
    ServerControlState*    scs = controls->server_state;
    U32                    max_ms = 0;
    F32                    max_speed_scale = inp_vel_scale;

    #if SERVER
    {
        const ServerControlState* scs = getLatestServerControlState(controls);
        int controls_input_should_be_ignored;
        int shell_disabled;
        
        // Check if the "controls_input_ignored" synchro-state has changed.

        controls_input_should_be_ignored =    controls->first > 1 ||
                                            e->controlsFrozen ||
                                            !entCanSetInpVel(e) ||
                                            !entAlive(e) ||
                                            e->adminFrozen ||
                                            e->logout_timer;

        if(controls_input_should_be_ignored != scs->controls_input_ignored)
        {
            ServerControlState* scs = getQueuedServerControlState(controls);

            scs->controls_input_ignored = controls_input_should_be_ignored;
        }
        
        // Check if disabling the shell has changed.        
        
        shell_disabled = e->doorFrozen || e->adminFrozen;
        
        if(shell_disabled != scs->shell_disabled){
            ServerControlState* scs = getQueuedServerControlState(controls);

            scs->shell_disabled = shell_disabled;
        }
    }
    #endif

    if (controls->nocoll)
        e->move_type |= MOVETYPE_NOCOLL;
    else
        e->move_type &= ~MOVETYPE_NOCOLL;

    if(    !controls->nocoll &&
        !controls->alwaysmobile &&
        (    scs->controls_input_ignored ||
            controls->recover_from_landing_timer ||
            controls->controls_input_ignored_this_csc))
    {
        zeroVec3(motion->input.vel);
    }
    else
    {
        F32        state[CONTROLID_BINARY_MAX];
        Vec3    horizVel;
        int        i;
        U8        dir_key_cur = controls->dir_key.down_now;

        for(i = 0; i < CONTROLID_BINARY_MAX; i++)
        {
            U32 in = (controls->dir_key.use_now & (1 << i)) ? controls->dir_key.total_ms[i] : 0;
            F32    out;
            
            if(in > max_ms)
            {
                max_ms = in;
            }

            if(i < CONTROLID_HORIZONTAL_MAX || scs->fly)
            {
                // Ramp the horizontal controls so that tapping a key quickly produces small movement.
                // Enabled for up and down when you are flying.
                
                // MS: This piece of code is crazy and should never be read by humans, or the Cryptic programmers.
                
                if(!in)
                    out = 0;
                else if(in >= 1000)
                    out = 1;
                else if(in <= 50 && dir_key_cur & (1 << i))
                    out = 0;
                else if(in < 75)
                    out = 0.2;
                else if(in >= 75 && in < 100)
                    out = 0.2 + 0.4 * powf((in - 75) * 0.04, 2);
                else
                    out = 0.6 + (in - 100) * 0.004 / 9.0;
            }
            else
            {
                // This makes up and down clamp to full speed when not flying.

                out = in ? 1 : 0;
            }

            state[i] = out;
        }
        
        controls->dir_key.max_ms = max_ms;

        //for(i = 0; i < CONTROLID_BINARY_MAX; i++)
        //{
        //    if(controls->dir_key.total_ms[i])
        //        printf("state %d: %d\n", i, controls->dir_key.total_ms[i]);
        //}

        // Determine movement direction.

        vel[0] = state[CONTROLID_RIGHT] -    state[CONTROLID_LEFT];
        vel[1] = state[CONTROLID_UP] -        state[CONTROLID_DOWN];
        vel[2] = state[CONTROLID_FORWARD] -    state[CONTROLID_BACKWARD];

        vel[0] *= scs->speed[0];
        vel[1] *= scs->speed[1];

        // Limit player horizontal movement to max_speed
        // if flying, also limit vertical velocity to max_speed

        copyVec3(vel, horizVel);

        if (!scs->fly && !controls->nocoll)
        {
            horizVel[1] = 0;
        }

        // Slow player's backward movement speed.
        
        if(vel[2] < 0)
        {
            max_speed_scale *= scs->speed_back;
        }
        
        // Scale for stunned.
        
        if(scs->stun)
        {
            max_speed_scale *= 0.1;
        }

        normalVec3(horizVel);

        // Check for debug speed_scale.
        
        if (controls->speed_scale)
        {
            max_speed_scale *= controls->speed_scale;
        }
            
        controls->max_speed_scale = max_speed_scale;

        if(*(int*)&horizVel[0])
            vel[0] = horizVel[0] * fabs(state[CONTROLID_RIGHT] - state[CONTROLID_LEFT]);
        else
            vel[0] = 0;
            
        if(*(int*)&horizVel[2])            
            vel[2] = horizVel[2] * fabs(state[CONTROLID_FORWARD] - state[CONTROLID_BACKWARD]);
        else
            vel[2] = 0;
        
        if (scs->fly || controls->nocoll)
        {
            if(*(int*)&horizVel[1])
                vel[1] = horizVel[1] * fabs(state[CONTROLID_UP] - state[CONTROLID_DOWN]);
            else
                vel[1] = 0;
        }
        else
        {
            if(!(controls->dir_key.use_now & (1 << CONTROLID_UP)))
            {
                vel[1] = 0;
            }
            else
            {
                F32 scale = scs->jump_height;
                
                if(scale < 0)
                {
                    scale = 0;
                }
                else if(scale > 1)
                {
                    scale = 1;
                }
                
                vel[1] *= scale;

                // Set jump_released if I want jump repeat.

                if(!scs->no_jump_repeat)
                {
                    e->motion->input.jump_released = 1;
                }
            }
        }
    }

    if (scs->fly)
        e->move_type |= MOVETYPE_FLY;
    else
        e->move_type &= ~MOVETYPE_FLY;

    if (scs->jumppack)
        e->move_type |= MOVETYPE_JUMPPACK;
    else
        e->move_type &= ~MOVETYPE_JUMPPACK;

    copyVec3(vel, motion->input.vel);

    // If they are trying to move, shut off AFK
    if(e->pl && e->pl->afk && !velIsZero(motion->input.vel))
    {
        e->pl->afk = 0;
#ifdef SERVER
        e->pl->send_afk_string = 1;
#endif
    }

#ifdef CLIENT
    // cue mouse spring with input vel
    MouseSpringInputVel(vel, max_speed_scale);
#endif
}

F32 pmotionGetMaxJumpHeight(Entity* e, ControlState* controls)
{
    #define STANDARD_JUMP_HEIGHT (4.0)
    
    return STANDARD_JUMP_HEIGHT * controls->server_state->jump_height;
}

#include "gridcoll/gridcoll.h"
#include "gridcoll/gridcollobj.h"
#if SERVER
#include "entity/entVarUpdate.h"
#endif

int pmotionGetNeighborhoodPropertiesInternal(DefTracker *neighborhood,char **namep,char **musicp, int *villainMinp, int *villainMaxp, int *minTeamp, int *maxTeamp)
{
    char    *name,*music,*villainMin,*villainMax,*minTeam,*maxTeam = 0;


    if (!neighborhood || !neighborhood->def->properties)
        return 0;

    name = groupDefFindPropertyValue(neighborhood->def,"NHoodName");
    music = groupDefFindPropertyValue(neighborhood->def,"NHoodSound");
    villainMin = groupDefFindPropertyValue(neighborhood->def,"NHoodVillainMin");
    villainMax = groupDefFindPropertyValue(neighborhood->def,"NHoodVillainMax");
    minTeam = groupDefFindPropertyValue(neighborhood->def,"NHoodMinTeam");
    maxTeam = groupDefFindPropertyValue(neighborhood->def,"NHoodMaxTeam");

    if (name || music || villainMin || villainMax || minTeam || maxTeam)
    {
        if (namep)
            *namep = name;
        if (musicp)
            *musicp = music;
        if (villainMinp && villainMin)
            *villainMinp = atoi(villainMin);
        if (villainMaxp && villainMax)
            *villainMaxp = atoi(villainMax);
        if (minTeamp && minTeam)
            *minTeamp = atoi(minTeam);
        if (maxTeamp && maxTeam)
            *maxTeamp = atoi(maxTeam);
        return 1;
    }
    return 0;
}

int pmotionGetNeighborhoodProperties(Entity *e,char **namep,char **musicp)
{
    if (!e)
        return 0;

    //If trackers might have been reset, I need a new volume check
    if( STATE_STRUCT.groupDefVersion != e->volumeList.groupDefVersion )
    {
        extern Volume glob_dummyEmptyVolume;
        e->volumeList.volumeCount = 0;
        e->volumeList.volumeCountLastFrame = 0;
        e->volumeList.materialVolume = &glob_dummyEmptyVolume;
        e->volumeList.neighborhoodVolume = &glob_dummyEmptyVolume;
    }
    return pmotionGetNeighborhoodPropertiesInternal(e->volumeList.neighborhoodVolume->volumePropertiesTracker,namep,musicp,NULL,NULL,NULL,NULL);
}

int pmotionGetNeighborhoodFromPos(const Vec3 pos, char **namep, char **musicp, int *villainMinp, int *villainMaxp, int *minTeamp, int *maxTeamp)
{
    static VolumeList volumeList = { 0 };

    volumeList.volumeCountLastFrame = 0; //if groupFindInside has a volumeList, volumeCountLastFrame needs to be valid
    groupFindInside(pos, FINDINSIDE_VOLUMETRIGGER, &volumeList, 0 );
    
    //TO DO this continues to work like before, but I need to change it to examine the list instead
    //and find the neighborhood that it's inside, so inside water is still inside 

    if( volumeList.neighborhoodVolume->volumeTracker )
    {
        assert( volumeList.neighborhoodVolume->volumePropertiesTracker );
        if (pmotionGetNeighborhoodPropertiesInternal(volumeList.neighborhoodVolume->volumePropertiesTracker, namep, musicp, villainMinp, villainMaxp, minTeamp, maxTeamp))
            return 1;
    }
    return 0;
}

//The only purpose of this collision check is to see if you are submerged
//and get the trajectory of your entry in to the water
int checkForSubmerged( const Mat4 mat, DefTracker    * tracker, CollInfo    * coll ) 
{
    int            submerged=0;
    Vec3        dv;
    F32            depth;
    Mat4        m;

    copyVec3( mat[3], dv );
    dv[1] += 2 * tracker->def->radius;
    if ( collGridObj(tracker,mat[3],dv,coll) ) // this shouldn't happen
    {
        normScaleToMat(coll->ctri->norm,coll->ctri->scale,m);
        mulMat3(m,tracker->mat,coll->mat);

        depth = coll->mat[3][1] - mat[3][1];
        if (depth > 3.4)
            submerged = 1;
    }

    return submerged;
}

#if SERVER
void enteredNoPvpVolume( Entity * e )
{
    if( !e || !e->pchar )
        return;

    //Disable PvP
    e->pvpZoneTimer         = 0.0;
    e->pchar->bPvPActive = 0;
    e->pvp_update         = true;

    if ( server_visible_state.isPvP )
    {
        sendInfoBox( e, INFO_COMBAT_SPAM, "YouAreNowPvPInactive" );
    }
}


void exitedNoPvpVolume( Entity * e )
{
    if( !e || !e->pchar )
        return;

    e->pvpZoneTimer        = PVP_ZONE_TIME_WHEN_EXITING_A_VOLUME;
    e->pvp_update        = true;
    e->pchar->bPvPActive = 0;
}
#endif

void pmotionScriptVolumeCheck( Entity * e )
{
#if SERVER
    int i;
    VolumeList * volumeList;

    if(ENTTYPE(e)!=ENTTYPE_PLAYER)
        return;

    assert(e && "e check in pmotionScriptVolumeCheck");
    volumeList = &e->volumeList;
    assert(volumeList && "volumeList check in pmotionScriptVolumeCheck");
    
    for( i = 0 ; i < volumeList->volumeCount ; i++ )
    {
        if(volumeList->volumes[i].volumePropertiesTracker)
        {
            char *name = groupDefFindPropertyValue(volumeList->volumes[i].volumePropertiesTracker->def,"NamedVolume");
            if(name)
                ScriptPlayerEntersVolume(e, name);
        }
    }
#endif
}

void pmotionIJustEnteredANewVolume( Entity * e, Volume * volume )
{

    if(ENTTYPE(e)==ENTTYPE_PLAYER && volume->volumePropertiesTracker)
    {
        char *name = groupDefFindPropertyValue(volume->volumePropertiesTracker->def,"NamedVolume");
#if SERVER

        if(name)
        { 
            StashElement element;

            badge_RecordPlaque(e, name);
            ScriptPlayerEntersVolume(e, name);
            TaskEnteredVolume(e, name);

            if(stricmp(name, "ArenaLobby")==0)
            {
                ArenaSetInsideLobby(e, 1);
            } 
            if(stricmp(name, "DJ_Architect")==0)
            {
                ArchitectEnteredVolume(e);
            } 
            else if(0==stricmp(name,"AttackableVolume"))
            {
                char *locationName = detailAttackVolumesVecName(volume->volumePropertiesTracker->mid);
                // If there is an ai entity watching this volume, make sure it executes its behavior
                if (g_htAttackVolumes && stashFindElement(g_htAttackVolumes,locationName, &element))
                {
                    AIVars* ai;
                    Entity* eAttacker = erGetEnt(*(EntityRef*)(stashElementGetPointer(element)));
                    if (eAttacker && (ai = ENTAI(eAttacker)))
                    {
                        ai->accThink = 0.0;
                    }
                }
            }
            else if( 0 == stricmp(name, "EncounterVolume" ) )
            {
                notifyEncounterItsVolumeHasBeenEntered( volume->volumePropertiesTracker );
            }
            else if( 0 == stricmp(name, "NoPvpVolume" ) )
            {
                enteredNoPvpVolume( e );
            }
            else if (strStartsWith(name, "Obj_"))
            {
                if (OnMissionMap())
                    SetMissionObjectiveComplete(OBJECTIVE_SUCCESS, name+4);
            }
        }

#elif CLIENT
        if (name && e == playerPtr())
        {
            if (stricmp(name, "ArenaLobby")==0)
                enteredArenaLobby();
            if(stricmp(name, "DJ_Architect")==0)
                ArchitectEnteredVolume(e);
            else if (stricmp(name, "SkyFade")==0) {
                char *sSkyFade1 = groupDefFindPropertyValue(volume->volumePropertiesTracker->def,"SkyFade1");
                char *sSkyFade2 = groupDefFindPropertyValue(volume->volumePropertiesTracker->def,"SkyFade2");
                char *sSkyFadeWeight = groupDefFindPropertyValue(volume->volumePropertiesTracker->def,"SkyFadeWeight");
                int iSkyFade1 = sSkyFade1?atoi(sSkyFade1):0;
                int iSkyFade2 = sSkyFade2?atoi(sSkyFade2):1;
                F32 fSkyFadeWeight = sSkyFadeWeight?atof(sSkyFadeWeight):1.0;
                sunSetSkyFadeOverride(iSkyFade1, iSkyFade2, fSkyFadeWeight);
            }
        }
        name = groupDefFindPropertyValue(volume->volumePropertiesTracker->def,"PopHelp");
        if (name && e == playerPtr())
        {
            popHelpEventHappenedByTag(name);
        }
#endif

#if SERVER
        name = groupDefFindPropertyValue(volume->volumePropertiesTracker->def, "VolumePower");

        if (name && e->pchar)
        {
            const BasePower *ppowBase = powerdict_GetBasePowerByFullName(&g_PowerDictionary, name);
            character_AddRewardPower(e->pchar, ppowBase);
        }
#endif
    }
}

void pmotionIJustExitedAVolume(Entity* e, Volume* volume)
{
    // mega-hack. this volume sometimes gets freed and we 
    // have yet to figure out why.
    // some day we'll have time to figure out how to use deftrackers properly
    // so that the crappy exited-a-volume pointer is tracked.
    // Use SEH to try to catch an invalid access.
    __try
    {
        
        if(ENTTYPE(e)==ENTTYPE_PLAYER && volume->volumePropertiesTracker &&
            volume->volumePropertiesTracker->def != (GroupDef*)(uintptr_t)0xdddddddddddddddd)
// bringing back the hack because pmotionVolumeTrackersInvalidate() was not sufficient
        {
            char *name = groupDefFindPropertyValue(volume->volumePropertiesTracker->def, "NamedVolume");
#if SERVER
            if (name)
            {
                ScriptPlayerLeavesVolume(e, name);

                if(stricmp(name, "ArenaLobby")==0)
                {
                    ArenaSetInsideLobby(e, 0);
                }
                if(stricmp(name, "DJ_Architect")==0)
                {
                    ArchitectExitedVolume(e);
                } 
                else if(0 == stricmp(name, "NoPvpVolume" ) )
                {
                    exitedNoPvpVolume( e );            
                }
            }
#elif CLIENT
            if (name && e == playerPtr())
            {
                if (stricmp(name, "ArenaLobby")==0)
                    leftArenaLobby();
                else if(stricmp(name, "DJ_Architect")==0)
                    ArchitectExitedVolume(e);
                else if (stricmp(name, "SkyFade")==0) {
                    sunUnsetSkyFadeOverride();
                }
            }
#endif

#if SERVER
            name = groupDefFindPropertyValue(volume->volumePropertiesTracker->def, "VolumePower");

            if (name && e->pchar)
            {
                const BasePower *ppowBase = powerdict_GetBasePowerByFullName(&g_PowerDictionary, name);
                character_RemoveBasePower(e->pchar, ppowBase);
            }
#endif
        }
    }
    __except(EXCEPTION_EXECUTE_HANDLER)
    {
        // catch any exceptions
    }
}

void pmotionVolumeTrackersInvalidate()
{
    int i;
    for(i = 1; i < entities_max; i++)
    {
        if(entities[i])
        {
            entities[i]->volumeList.volumeCount = 0;
            entities[i]->volumeList.volumeCountLastFrame = 0;
        }
    }
}

void pmotionCheckVolumeTrigger(Entity *e)
{
    DefTracker    *materialTracker;
    DefTracker    *prevMaterialTracker;
    CollInfo   coll; //just used to check on water collisions
    VolumeList * volumeList;
 
    assert(e && "e check in pmotionCheckVolumeTrigger");
    volumeList = &e->volumeList;
    assert(volumeList && "volumeList check in pmotionCheckVolumeTrigger");

    prevMaterialTracker = volumeList->materialVolume->volumeTracker;

    /////////// Find the new volumes //////////////////////////////////////
    {
        int    volumeCheckNeeded=0;

        //If I've moved much, I need a new volume check
        if( !nearSameVec3Tol( volumeList->posVolumeWasLastChecked, ENTPOS(e), 0.001 ) )
            volumeCheckNeeded = 1;

        //If trackers might have been reset, I need a new volume check
        if( STATE_STRUCT.groupDefVersion != volumeList->groupDefVersion )
        {
            extern Volume glob_dummyEmptyVolume;
            volumeList->volumeCount = 0;
            volumeList->volumeCountLastFrame = 0;
            volumeList->materialVolume = &glob_dummyEmptyVolume;
            volumeList->neighborhoodVolume = &glob_dummyEmptyVolume;

            volumeCheckNeeded = 1;
            prevMaterialTracker = 0;
        }

        if(  volumeCheckNeeded )
        {
            Vec3 feet;
            int        didVolumeListChange = 0;

            //Swap.  Need a swap instead of just a copy because we need to two dynArrays
            {
                void * temp  = volumeList->volumesLastFrame;    //just keep the dynArray info, count last frame isn't important                
                int  temp3 = volumeList->volumeCountMaxLastFrame;

                volumeList->volumesLastFrame        = volumeList->volumes;                    
                volumeList->volumeCountLastFrame    = volumeList->volumeCount;            
                volumeList->volumeCountMaxLastFrame    = volumeList->volumeCountMax;    

                volumeList->volumes            = temp;                    
                volumeList->volumeCountMax    = temp3;    
            }
            //end swap

            copyVec3(ENTPOS(e),feet);
            feet[1] += 0.1;
            groupFindInside(feet,FINDINSIDE_VOLUMETRIGGER, volumeList, &didVolumeListChange );

            //Check to see if anything changed, if so, do callback
            //None of these callbacks apply for anything but players currently, so let's move the check here
            if(didVolumeListChange && ENTTYPE(e)==ENTTYPE_PLAYER)
            {
                int i, j;
#ifdef SERVER
                int sonicfence = 0;
#endif

                // entering new volumes and updating flags based on current volume state
                for( i = 0 ; i < volumeList->volumeCount ; i++ )
                {
                    char *name = NULL;
#ifdef SERVER
                    char *sonicfenceprop = NULL;
#endif

                    // We don't need the name and the flag really, but it helps with debugging.
                    if( volumeList->volumes[i].volumePropertiesTracker && volumeList->volumes[i].volumePropertiesTracker->def )
                    {                
                        name = groupDefFindPropertyValue( volumeList->volumes[i].volumePropertiesTracker->def,"NamedVolume");

#ifdef SERVER
                        sonicfenceprop = groupDefFindPropertyValue( volumeList->volumes[i].volumePropertiesTracker->def,"SonicFencing" );
                        if( sonicfenceprop )
                            sonicfence = 1;
#endif
                    }
                    
                    for( j = 0 ; j < volumeList->volumeCountLastFrame ; j++ ) 
                    {
                        if( volumeList->volumes[i].volumeTracker == volumeList->volumesLastFrame[j].volumeTracker )
                            break;

                        if( name && volumeList->volumesLastFrame[j].volumePropertiesTracker && volumeList->volumesLastFrame[j].volumePropertiesTracker->def )
                        {
                            char *otherName = groupDefFindPropertyValue( volumeList->volumesLastFrame[j].volumePropertiesTracker->def,"NamedVolume");
                            if(otherName && stricmp(name, otherName) == 0) // different volume but with identical name, treat as identical volume
                                break;
                        }
                    }
                    if(j == volumeList->volumeCountLastFrame) // didn't see it this frame
                        pmotionIJustEnteredANewVolume( e, &volumeList->volumes[i] );
                }

#ifdef SERVER
                // If any volume has a property called 'SonicFencing' then it's not a valid place for the volume-reject teleport to land us.
                if( e && e->pl )
                {
                    e->pl->rejectPosVolume = sonicfence;
                }
#endif

                // exiting volumes
                for (i = 0; i < volumeList->volumeCountLastFrame; i++)
                {
                    char *name = NULL;

                    if( volumeList->volumesLastFrame[i].volumePropertiesTracker && volumeList->volumesLastFrame[i].volumePropertiesTracker->def )
                        name = groupDefFindPropertyValue( volumeList->volumesLastFrame[i].volumePropertiesTracker->def,"NamedVolume");

                    for (j = 0; j < volumeList->volumeCount; j++)
                    {
                        if (volumeList->volumes[j].volumeTracker == volumeList->volumesLastFrame[i].volumeTracker)
                            break;

                        if( name && volumeList->volumes[j].volumePropertiesTracker && volumeList->volumes[j].volumePropertiesTracker->def )
                        {
                            char *otherName = groupDefFindPropertyValue( volumeList->volumes[j].volumePropertiesTracker->def,"NamedVolume");
                            if(otherName && stricmp(name, otherName) == 0) // different volume but with identical name, treat as identical volume
                                break;
                        }
                    }
                    if(j == volumeList->volumeCount) // didn't see it this frame
                        pmotionIJustExitedAVolume( e, &volumeList->volumesLastFrame[i] );
                }
            }
            //End if anything changed check

            e->roomImIn = groupFindLightTracker(feet);

            //Bookkeeping for volumeCheck Needed
            copyVec3( ENTPOS(e), volumeList->posVolumeWasLastChecked );
            volumeList->groupDefVersion = STATE_STRUCT.groupDefVersion;
        }
    }

    materialTracker = volumeList->materialVolume->volumeTracker;

    //////Mysterious Sever Side Door Stuff, I;m not touching the logic
    #if SERVER
    if(prevMaterialTracker != materialTracker)
    {
        if(e->in_door_volume)
        {
            START_PACKET(pak, e, SERVER_MAP_XFER_LIST_CLOSE);
            END_PACKET
        }
    }

    if (materialTracker && materialTracker->def)
    {
        if (materialTracker->def->door_volume)
        {
            StashTable props = materialTracker->def->properties;
            e->in_door_volume = 1;
            if (ENTTYPE(e) == ENTTYPE_PLAYER && prevMaterialTracker != materialTracker)
                enterDoor(e,materialTracker->mat[3],"0",1,props);
        }
        else
        {
            e->in_door_volume = 0;
        }
    }
    #endif

    //Possible overkill because entering door can blow away the map
    if( STATE_STRUCT.groupDefVersion != volumeList->groupDefVersion )
    {
        extern Volume glob_dummyEmptyVolume;
        volumeList->volumeCount = 0;
        volumeList->volumeCountLastFrame = 0;
        volumeList->materialVolume = &glob_dummyEmptyVolume;
        volumeList->neighborhoodVolume = &glob_dummyEmptyVolume;
        materialTracker = volumeList->materialVolume->volumeTracker;
        prevMaterialTracker = 0;
    }

    ///////  Handle the materialTrackers //////////////////////////
    if (materialTracker && materialTracker->def)
    {
        SeqInst        *seq;

        seq = e->seq;

        if (materialTracker->def->water_volume)
        {
            SETB( seq->state, STATE_INWATER );
            if ( checkForSubmerged( ENTMAT(e), materialTracker, &coll ) )
            {
                SETB( seq->state, STATE_SWIM );
                SETB( seq->state, STATE_INDEEP );
            }
        }
        if (materialTracker->def->sewerwater_volume)
        {
            SETB( seq->state, STATE_INSEWERWATER );
            if ( checkForSubmerged( ENTMAT(e), materialTracker, &coll ) )
            {
                SETB( seq->state, STATE_SWIM );
                SETB( seq->state, STATE_INDEEP );
            }
        }
        if (materialTracker->def->redwater_volume)
        {
            SETB( seq->state, STATE_INREDWATER );
            if ( checkForSubmerged( ENTMAT(e), materialTracker, &coll ) )
            {
                SETB( seq->state, STATE_SWIM );
                SETB( seq->state, STATE_INDEEP );
            }
        }
        if (materialTracker->def->lava_volume)
        {
            SETB( seq->state, STATE_INLAVA );
        }
        //else it's a door volume or generic material volume that doesn't set bits


#ifdef CLIENT
        //TO DO, I dont need to be checking for submerged all the time
        //TO DO combine with pmotionIJustEnteredANewVolume if you can 
        {
            checkForSubmerged( ENTMAT(e), materialTracker, &coll );
            copyMat4( e->vol_collision, e->prev_vol_collision );
            copyMat4( coll.mat, e->vol_collision );
        }
#endif
    }

#ifdef CLIENT
    if( e == playerPtr() )
    {
        if (materialTracker && materialTracker->def)
        {
            estrPrintCharString(&g_activeMaterialTrackerName, materialTracker->def->name);
        }
        else
        {
            estrClear(&g_activeMaterialTrackerName);
        }
    }
#endif
    ///////////End the material tracker stuff
}

void pmotionApplyFuturePush(MotionState* motion, FuturePush* fp)
{
    F32 dot = dotVec3(motion->vel, fp->add_vel);
    
    motion->falling = 1;
    motion->on_surf = 0;
    motion->was_on_surf = 0; 
    motion->low_traction_steps_remaining = 10;

    // If a guy is running away from you and you hit him with knockback,
    // don't increase his velocity past the knockback force, because that 
    // will look insane
    if(dot > 0)
    {
        F32 lenSQR = lengthVec3Squared(fp->add_vel);
        F32 scale = dot / lenSQR;
        Vec3 proj;
        F32 newLenSQR;
        
        scaleVec3(fp->add_vel, scale, proj);
        newLenSQR = lengthVec3Squared(proj);
        
        if(newLenSQR < lenSQR)
        {
            copyVec3(fp->add_vel, motion->vel);
        }
        else
        {
            scale = sqrt(newLenSQR) / sqrt(lenSQR);
            scaleVec3(fp->add_vel, scale, motion->vel);
        }
    }
    else
    {
        copyVec3(fp->add_vel, motion->vel);
    }
}

static PhysicsRecording* getPhysicsRecording(Entity* e, const char* trackName, int create)
{
    char hashName[1000];
    StashElement element;
    
    if(!global_pmotion_state.htPhysicsRecording)
    {
        if(!create)
        {
            return NULL;
        }
        
        global_pmotion_state.htPhysicsRecording = stashTableCreateWithStringKeys(10, StashDeepCopyKeys);
        element = NULL;
    }
    
    sprintf(hashName, "%d.%s", e->db_id, trackName);
    
    stashFindElement(global_pmotion_state.htPhysicsRecording, hashName, &element);
    
    if(!element)
    {
        PhysicsRecording* rec;
        
        if(!create)
        {
            return NULL;
        }
        
        rec = calloc(sizeof(PhysicsRecording), 1);
        rec->name = strdup(hashName);
        
        stashAddPointerAndGetElement(global_pmotion_state.htPhysicsRecording, hashName, rec, false, &element);
    }
    
    return stashElementGetPointer(element);
}

static PhysicsRecordingStep* pmotionAddPhysicsRecordingStep(Entity* e, const char* trackName, int index)
{
    PhysicsRecording* rec = getPhysicsRecording(e, trackName, 1);
    PhysicsRecordingStep* step;
    
    dynArrayAdd(&rec->steps.step, sizeof(rec->steps.step[0]), &rec->steps.count, &rec->steps.maxCount, 1);
    
    if(index < 0){
        index = rec->steps.count - 1;
    }
    
    assert(index < rec->steps.count);

    memmove(rec->steps.step + index + 1, rec->steps.step + index, sizeof(rec->steps.step[0]) * (rec->steps.count - index - 1));
    
    step = rec->steps.step + index;
    
    ZeroStruct(step);
    
    return step;
}

static struct {
    struct {
        PhysicsRecording*    rec;
        int                    index;
        ControlStateChange*    csc;
    }* entries;
    int count;
    int maxCount;
} update_net_id_list;

void pmotionRecordStateBeforePhysics(Entity* e, const char* trackName, ControlStateChange* csc)
{
    PhysicsRecordingStep* step = pmotionAddPhysicsRecordingStep(e, trackName, -1);
    MotionState* motion = e->motion;
    
    step->csc_net_id = csc ? csc->net_id : 0;
    step->before.motion = *motion;
    copyVec3(ENTPOS(e), step->before.pos);
    
    #if CLIENT
        if(csc && !csc->sent)
        {
            PhysicsRecording* rec = getPhysicsRecording(e, trackName, 1);
            
            // Offset by one to distinguish from uninitialized.
            
            dynArrayAdd(&update_net_id_list.entries,
                        sizeof(update_net_id_list.entries[0]),
                        &update_net_id_list.count,
                        &update_net_id_list.maxCount, 1);
            
            ZeroStruct(update_net_id_list.entries + update_net_id_list.count - 1);
            update_net_id_list.entries[update_net_id_list.count-1].rec = rec;
            update_net_id_list.entries[update_net_id_list.count-1].index = rec->steps.count - 1;
            update_net_id_list.entries[update_net_id_list.count-1].csc = csc;
        }
    #endif
}

void pmotionRecordStateAfterPhysics(Entity* e, const char* trackName)
{
    PhysicsRecording* rec = getPhysicsRecording(e, trackName, 0);
    PhysicsRecordingStep* step;
    
    if(!rec || !rec->steps.count)
    {
        return;
    }
    
    step = rec->steps.step + rec->steps.count - 1;
    step->after.motion = *e->motion;
    copyVec3(ENTPOS(e), step->after.pos);
}

#if SERVER
void pmotionSendPhysicsRecording(Packet* pak, Entity* e, const char* trackName)
{
    PhysicsRecording* rec = getPhysicsRecording(e, trackName, 0);
    int i;
    
    if(!rec)
    {
        pktSendBitsPack(pak, 1, 0);
        return;
    }
    
    pktSendBitsPack(pak, 1, sizeof(MotionState));
    pktSendString(pak, trackName);
    
    for(i = 0; i < rec->steps.count; i++)
    {
        PhysicsRecordingStep* step = rec->steps.step + i;
        
        pktSendBits(pak, 1, 1);
        pktSendBits(pak, sizeof(step->csc_net_id) * 8, step->csc_net_id);
        pktSendBitsArray(pak, sizeof(step->before) * 8, &step->before);
        pktSendBitsArray(pak, sizeof(step->after) * 8, &step->after);
    }
    
    rec->steps.count = 0;
    
    pktSendBits(pak, 1, 0);
}
#endif

#if CLIENT
void pmotionReceivePhysicsRecording(Packet* pak, Entity* e)
{
    int size = pktGetBitsPack(pak, 1);
    PhysicsRecording* rec;
    int insertHere;
    char trackName[1000];
    
    if(!size)
    {
        return;
    }
    
    assert(size == sizeof(MotionState));
    
    strncpyt(trackName, pktGetString(pak), ARRAY_SIZE(trackName) - 1);
    
    rec = getPhysicsRecording(e, trackName, 1);
    
    for(insertHere = 0; insertHere < rec->steps.count; insertHere++)
    {
        if(rec->steps.step[insertHere].pak_id >= pak->id)
        {
            break;
        }
    }
    
    while(pktGetBits(pak, 1) == 1)
    {
        PhysicsRecordingStep* step = pmotionAddPhysicsRecordingStep(e, trackName, insertHere);
        
        insertHere++;
        
        step->pak_id = pak->id;
        step->csc_net_id = pktGetBits(pak, sizeof(step->csc_net_id) * 8);
        pktGetBitsArray(pak, sizeof(step->before) * 8, &step->before);
        pktGetBitsArray(pak, sizeof(step->after) * 8, &step->after);
    }
}

void pmotionUpdateNetID(ControlStateChange* csc)
{
    int i;
    
    for(i = 0; i < update_net_id_list.count; i++)
    {
        if(!csc || update_net_id_list.entries[i].csc == csc)
        {
            if(csc)
            {
                PhysicsRecording* rec = update_net_id_list.entries[i].rec;
                
                int index = update_net_id_list.entries[i].index;
                
                if(index >= 0 && index < rec->steps.count)
                {
                    if(!rec->steps.step[index].csc_net_id)
                    {
                        rec->steps.step[index].csc_net_id = csc->net_id;
                    }
                }
            }
                        
            memmove(update_net_id_list.entries + i,
                    update_net_id_list.entries + i + 1,
                    (update_net_id_list.count - i - 1) * sizeof(update_net_id_list.entries[0]));
                    
            update_net_id_list.count--;
        }
    }
}
#endif

