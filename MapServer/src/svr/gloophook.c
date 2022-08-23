#include <utilitieslib/assert/assert.h>
#include "gloophook.h"
#include "generator/entgen.h"
#include "cmdparse/cmdserver.h"
#include "missionMapInit.h"
#include <utilitieslib/components/HashFunctions.h>
#include "entity/entityRef.h"
#include <utilitieslib/utils/timing.h>
#include "ai/entaiprivate.h"
#include "entity/entserver.h"
#include "generator/encounter.h"
#include "storyarc/storyarcinterface.h"
#include "entity/motion.h"
#include "cmdparse/cmdcommon.h"
#include "svr_player.h"
#include "gridcoll/megaGrid.h"
#include "entity/character_tick.h"
#include "seq/seq.h"
#include "entity/entity.h"
#include "gameSys/cutScene.h"
#include <utilitieslib/utils/mathutil.h>
#include "utils/position.h"
#include "script/scriptengine.h"
#include "ai/entai.h"
#include "NovodeX/NwWrapper.h"
#include "entity/entGameActions.h"

typedef struct EntityTimer {
    PerformanceInfo*    root;
    int                    enabled;
    char                name[100];
} EntityTimer;

EntityTimer* entityTimerGroups[4];
int entityTimerGroupSizes[4] = {500, 100, 10, 1};

static void initEntityTimerGroups(){
    int i;
    
    if(entityTimerGroups[0])
        return;
        
    for(i = 0; i < ARRAY_SIZE(entityTimerGroups); i++){
        int group_size = entityTimerGroupSizes[i];
        int group_count = (MAX_ENTITIES_PRIVATE + group_size - 1) / group_size;
        int k;

        assert(!i || !(entityTimerGroupSizes[i-1] % group_size));
        
        entityTimerGroups[i] = calloc(sizeof(entityTimerGroups[i][0]), group_count);
        assert(entityTimerGroups);

        for(k = 0; k < group_count; k++){
            if(group_size > 1)
                sprintf(entityTimerGroups[i][k].name, "%d-%d", k * group_size, (k+1) * group_size - 1);
            else
                sprintf(entityTimerGroups[i][k].name, "%d", k);

            entityTimerGroups[i][k].enabled = i == 0;
        }
    }
}

void enableEntityTimer(int ent_idx){
    int i;
    
    if(ent_idx < 0 || ent_idx >= MAX_ENTITIES_PRIVATE)
        return;
    
    initEntityTimerGroups();
    
    for(i = 1; i < ARRAY_SIZE(entityTimerGroups); i++){
        int group_size = entityTimerGroupSizes[i];
        int index = ent_idx / group_size;

        if(!entityTimerGroups[i][index].enabled){
            int parent_group_size = entityTimerGroupSizes[i-1];
            int parent_index = ent_idx / parent_group_size;
            int j;

            for(j = 0; j < parent_group_size / group_size; j++){
                entityTimerGroups[i][parent_index * parent_group_size / group_size + j].enabled = 1;
            }
            
            break;
        }
    }
}

void disableEntityTimers(){
    int i;
    
    initEntityTimerGroups();
    
    for(i = 1; i < ARRAY_SIZE(entityTimerGroups); i++){
        int group_size = entityTimerGroupSizes[i];
        int group_count = (MAX_ENTITIES_PRIVATE + group_size - 1) / group_size;
        int j;
        
        for(j = 0; j < group_count; j++){
            entityTimerGroups[i][j].enabled = 0;
        }
    }
}

void executeEntities(){
    int i;
    int runAI[ENTTYPE_COUNT] = {0};
    int runProcess[ENTTYPE_COUNT];
    int activeAI[] = {
        ENTTYPE_PLAYER,
        ENTTYPE_NPC,
        ENTTYPE_CAR,
        ENTTYPE_CRITTER,
        ENTTYPE_DOOR,
        -1,
    };
    
    if(!player_count){
        return;
    }
        
    // Flag what types of AI to run.
    
    for(i = 0; activeAI[i] >= 0; i++){
        runAI[activeAI[i]] = server_state.noAI != ~0;
    }

    if(server_state.noAI){
        runAI[ENTTYPE_CRITTER] &= !(server_state.noAI & 1);
        runAI[ENTTYPE_PLAYER] &= !(server_state.noAI & 2);
        runAI[ENTTYPE_CAR] &= !(server_state.noAI & 4);
        runAI[ENTTYPE_NPC] &= !(server_state.noAI & 8);
    }

    // Flag what types of processing to run.
    
    for(i = 0; i < ENTTYPE_COUNT; i++){
        runProcess[i] = server_state.noProcess != ~0;
    }
    
    if(server_state.noProcess){
        runProcess[ENTTYPE_CRITTER] = !(server_state.noProcess & 1);
        runProcess[ENTTYPE_PLAYER] = !(server_state.noProcess & 2);
        runProcess[ENTTYPE_CAR] = !(server_state.noProcess & 4);
        runProcess[ENTTYPE_NPC] = !(server_state.noProcess & 8);
    }
                    
    setEntCollTimes(0, 0, 0);
    
    aiInitMain();

    for(i = 1; i < entities_max; i++){
        Entity* e = entities[i];

        if(e && entity_state[i] & ENTITY_IN_USE){
            if(ENTINFO_BY_ID(i).kill_me_please){
                // Kill the entity if necessary.
            
                entFree(e);
            }
            else if(!ENTSLEEPING_BY_ID(i) && !ENTPAUSED_BY_ID(i)){
                AIVars* ai = ENTAI_BY_ID(i);
                EntType entType = ENTTYPE_BY_ID(i);
                EntityProcessRate entProcessRate = ENTINFO_BY_ID(i).entProcessRate;
                int last_max_depth;
                
                if(entProcessRate > server_state.process.max_rate_this_tick)
                    continue;
            
                // Initialize on first process.

                if(!ENTINFO_BY_ID(i).has_processed){
                    PERFINFO_AUTO_START("firstProcess", 1);
                        entDoFirstProcessStuff(e, ai, i);
                    PERFINFO_AUTO_STOP();
                }
                
                if(server_state.time_all_ents){
                    int j;

                    if(!entityTimerGroups[0]){
                        initEntityTimerGroups();
                    }
                    
                    for(j = 0; j < ARRAY_SIZE(entityTimerGroups); j++){
                        int index = i / entityTimerGroupSizes[j];

                        if(!entityTimerGroups[j][index].enabled)
                            break;
                            
                        PERFINFO_AUTO_START_STATIC(entityTimerGroups[j][index].name, &entityTimerGroups[j][index].root, 1);
                    }
                    
                    if(j != ARRAY_SIZE(entityTimerGroups)){
                        last_max_depth = timing_state.autoStackMaxDepth;
                        timing_state.autoStackMaxDepth = timing_state.autoStackDepth;
                    }else{
                        last_max_depth = -1;
                    }
                }
                                
                // Run AI.
                
                if(runAI[entType]){
                    e->timestep = server_state.process.timestep_acc[entProcessRate];

                    aiMain(e, ai, entType);
                }else{
                    if(ENTHIDE_BY_ID(i)){
                        SET_ENTHIDE_BY_ID(i) = 0;
                        e->draw_update = 1;
                    }
                    
                    e->timestep = TIMESTEP;
                }
                        
                //Temp debug
                if( !FINITE( e->timestep ) || ( e->timestep < 0.0f || e->timestep > 100000.f ) )
                {
                    e->timestep = 1.0;
                } //ENd temp debug
                
                // Run processing.

                if(runProcess[entType]){
                    PERFINFO_AUTO_START("entProcess", 1);
                        entProcess(e, ai, entType);
                    PERFINFO_AUTO_STOP();
                }
                
                if(server_state.time_all_ents){
                    int j;

                    if(last_max_depth >= 0){
                        timing_state.autoStackMaxDepth = last_max_depth;
                    }
                    
                    for(j = 0; j < ARRAY_SIZE(entityTimerGroups); j++){
                        int index = i / entityTimerGroupSizes[j];

                        if(!entityTimerGroups[j][index].enabled)
                            break;
                            
                        PERFINFO_AUTO_STOP();
                    }
                }
            }
        }
    }
}

static void resetVisGrid(){
    int i;

    for(i = 1; i < entities_max; i++){
        Entity* e = validEntFromId(i);
        
        if(e){
            if(e->megaGridNode){
                destroyMegaGridNode(e->megaGridNode);
                e->megaGridNode = NULL;
            }
            
            entInitGrid(e, ENTTYPE(e));

            mgUpdate(0, e->megaGridNode, ENTPOS(e));
        }
    }
}

static void checkServerLoad(){
    int i;
    
    int active_count = player_ents[PLAYER_ENTS_ACTIVE].count;
    
    if(    !server_state.in_lame_mode &&
        active_count >= server_state.safe_player_count &&
        ABS_TIME_SINCE(server_state.lame_mode_switch_abs_time) > SEC_TO_ABS(5))
    {
        // Go into lame mode.
        
        dbLog("serverload", NULL, "High server load, killing everything.");

        server_state.in_lame_mode = 1;
        server_state.lame_mode_switch_abs_time = ABS_TIME;
        
        server_state.ent_vis_distance_scale = 0.5;

        resetVisGrid();
        
        entgenPause();
        
        EncounterForceReset(NULL, NULL);    

        EncounterAllowProcessing(0);

        for(i = 0; i < entities_max; i++){
            Entity* e = validEntFromId(i);
            
            if(e){
                EntType entType = ENTTYPE_BY_ID(i);
                
                if(    e->parentGenerator &&
                    (    entType == ENTTYPE_NPC ||
                        entType == ENTTYPE_CAR))
                {
                    SET_ENTINFO_BY_ID(i).kill_me_please = 1;
                }
            }
        }
    }
    else if(server_state.in_lame_mode &&
            active_count < server_state.safe_player_count - 25 &&
            ABS_TIME_SINCE(server_state.lame_mode_switch_abs_time) > SEC_TO_ABS(20))
    {
        // Leaving lame mode.
        
        dbLog("serverload", NULL, "Lower server load, reviving everything.");

        server_state.in_lame_mode = 0;
        server_state.lame_mode_switch_abs_time = ABS_TIME;

        server_state.ent_vis_distance_scale = 1;

        resetVisGrid();

        entgenResume();

        EncounterAllowProcessing(1);
    }
}

void executeGameLogic(){

    PERFINFO_AUTO_START("MissionProcess", 1);

    MissionProcess(); // need to notice timers, etc. even if players not on map

    PERFINFO_AUTO_STOP();

    if(!player_count){
        return;
    }

    PERFINFO_AUTO_START("checkServerLoad", 1);
    
        checkServerLoad();

    PERFINFO_AUTO_STOP_START("entgenProcess", 1);

        // Generate more entities when appropriate

        entgenProcess();

    PERFINFO_AUTO_STOP_START("EncounterProcess", 1);

        // Process encounters as appropriate

        EncounterProcess();

    PERFINFO_AUTO_STOP_START("ContactProcess", 1);

        ContactProcess();

    PERFINFO_AUTO_STOP_START("StoryArcCheckTimeouts", 1);

        StoryArcCheckTimeouts();

    PERFINFO_AUTO_STOP();
}


