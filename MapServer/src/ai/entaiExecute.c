
#include "entaiprivate.h"
#include "seq/seq.h"
#include "entity/entity.h"
#include <utilitieslib/utils/timing.h>
#include "cmdparse/cmdserver.h"
#include "ai/entaiLog.h"
#include "ai/entai.h"

// The main AI functions for each entity type.  These are mainly just wrappers so I can put performance
// monitoring code around the whole function.

static void aiMainPlayer(Entity* e, AIVars* ai){
    PERFINFO_AUTO_START("player", 1);
        aiPlayer(e, ai);
        
        AI_LOG(    AI_LOG_SEQ,
                (e,
                "Move: ^3%s ^0(^3%s^0 + ^3%s^0)\n",
                e->seq->animation.move_lastframe ? e->seq->animation.move_lastframe->name : "NONE",
                seqGetStateString(e->seq->state_lastframe),
                seqGetStateString(e->seq->sticky_state)));
    PERFINFO_AUTO_STOP();
}

static void aiMainCritter(Entity* e, AIVars* ai){
    PERFINFO_AUTO_START("critter", 1);
        // Run the critter-specific AI.
        
        aiCritter(e, ai);
        
        AI_LOG(    AI_LOG_SEQ,
                (e,
                "Move: ^3%s ^0(^3%s^0 + ^3%s^0)\n",
                e->seq->animation.move_lastframe ? e->seq->animation.move_lastframe->name : "NONE",
                seqGetStateString(e->seq->state_lastframe),
                seqGetStateString(e->seq->sticky_state)));
    PERFINFO_AUTO_STOP();
}

static void aiMainCar(Entity* e, AIVars* ai){
    PERFINFO_AUTO_START("car", 1);
        aiCar(e, ai);
    PERFINFO_AUTO_STOP();
}

static void aiMainNPC(Entity* e, AIVars* ai){
    PERFINFO_AUTO_START("npc", 1);
        aiNPC(e, ai);
    PERFINFO_AUTO_STOP();
}

static void aiMainDoor(Entity* e, AIVars* ai){
    PERFINFO_AUTO_START("door", 1);
        aiDoor(e, ai);
    PERFINFO_AUTO_STOP();
}

// The type-specific entity main AI function lookup table.

typedef void (*AIMainFunction)(Entity* e, AIVars* ai);

static AIMainFunction aiMainFunctionsLUT[ENTTYPE_COUNT] = {0};

static void aiMainFunctionsLUTInit(){
    aiMainFunctionsLUT[ENTTYPE_PLAYER]    = aiMainPlayer;
    aiMainFunctionsLUT[ENTTYPE_CRITTER]    = aiMainCritter;
    aiMainFunctionsLUT[ENTTYPE_CAR]        = aiMainCar;
    aiMainFunctionsLUT[ENTTYPE_NPC]        = aiMainNPC;
    aiMainFunctionsLUT[ENTTYPE_DOOR]    = aiMainDoor;
}

// Init AI function LUT.

void aiInitMain(){
    static int aiMainFunctionsLUTInitialized = 0;

    if(!aiMainFunctionsLUTInitialized){
        // Create the lookup table for entity ai functions.
        
        aiMainFunctionsLUTInitialized = 1;
        
        aiMainFunctionsLUTInit();
    }
}

// Main entrypoint for AI.  This will figure out what needs to be called for this entity.

void aiMain(Entity* e, AIVars* ai, EntType entType){
    // Run the type-specific AI.
    
    if(aiMainFunctionsLUT[entType]){
        PERFINFO_AUTO_START("ai", 1);
            aiMainFunctionsLUT[entType](e, ai);
        PERFINFO_AUTO_STOP();
    }
}

