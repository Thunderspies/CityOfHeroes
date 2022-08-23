#include <stdlib.h>
#include <utilitieslib/assert/assert.h>
#include <utilitieslib/assert/assert.h>
#include <crtdbg.h>

#include "entgen.h"
#include <utilitieslib/components/ArrayOld.h>
#include "group/grouputil.h"
#include "utils/position.h"
#include <utilitieslib/components/StashTable.h>

#include <utilitieslib/components/HashTableStack.h>
#include "gridcoll/gridcoll.h"
#include <utilitieslib/components/MemoryPool.h>
#include "entgenLoader.h"
#include "missionMapInit.h"
#include "cmdparse/cmdserver.h"
#include "entity/entserver.h"
#include "entity/entsend.h"
#include "ai/entai.h"
#include "gameComm/NpcServer.h"
#include "gameComm/VillainDef.h"
#include <utilitieslib/utils/error.h>
#include "svr/svr_player.h"
#include "entity/motion.h"
#include "entity/netfx.h"
#include <utilitieslib/language/AppLocale.h>

// lf stuff
#include "string.h"
#include "combat/combat.h"
#include "entity/entVarUpdate.h"
#include "entity/entserver.h"
#include <utilitieslib/components/StringCache.h>
#include "varutils.h"
#include "seq/seq.h"
#include "entity/entity.h"
#include "entity/ragdoll.h"

Array randomizedGenerators = {0, 0, 0}; // An array that holds all generators, but in randomized order
static int generatorsPaused = 0;
static SpawnAreaInst* curSpawnArea = NULL;
SpawnAreaInst* globalSpawnArea = NULL;

static void generateSpawnGroupMember(GeneratedGrpMemberDef* def, GeneratorInst* gen);
static int initEntityAIType(Entity* ent, GeneratedEntDef* generatedType);

void removeEntFromSpawnArea(Entity* ent){
    // Disable generator if its vital child has been killed
    //if(ent->generatorVitalChild)
    //    ent->parentGenerator->active = 0;

    // otherwise simply decrement the generator child count
    if(ent->parentGenerator){
        ent->entityDef->curActiveEnts--;
        ent->parentGenerator->spawnArea->curActiveEnts--;
        ent->parentGenerator->childCount--;
        if(ENTTYPE_CRITTER == ENTTYPE(ent)){
            assert(ent->parentGenerator->spawnArea->curCritterEnts > 0);
            ent->parentGenerator->spawnArea->curCritterEnts--;
        }
        else if(ENTTYPE_NPC == ENTTYPE(ent)){
            assert(ent->parentGenerator->spawnArea->curNPCEnts > 0);
            ent->parentGenerator->spawnArea->curNPCEnts--;
        }
    }
}

/************************************************************************************
 * Misc utilities
 */
//static Entity* generateEntityRaw(GeneratedEntDef* generatedType, GeneratorInst* gen);
Pair* pickPercentageWeightedPair(Array* pairs);

/* Function pickPercentageWeightedPair
 *    Given an array of pairs where the all pair::value2 is some integer percentage
 *    value that defines how often the pair should be picked, this function picks a
 *    pair at random, using the percentage as a guide.
 *
 *    If the first pair has a pick percentage of -1, all pairs will be picked at
 *    equal rates.
 *
 *    When the function is functioning in the "weighted" mode, it does not assume
 *    that all weights add up to be 100 (in other words, 100%).  It is possible for
 *    some pairs toward the end of the array to never be picked if the entire
 *    sum is more than 100.  If the sum is less than a 100, the weighted percentage
 *    of the last pair will automatically fill the sum up to 100.
 *
 *    Returns:
 *        Valid pair pointer - randomly picked pair.
 *
 */
Pair* pickPercentageWeightedPair(Array* pairs){
    Pair* firstPair;
    int randomRoll;
    assert(pairs->size);

    firstPair = pairs->storage[0];

    // How should we pick the pairs?  at equal rates? or at weighted rates?
    if(-1 == (int)(intptr_t)(firstPair->value2)){
        // Picking a pair at equal rates.
        randomRoll = randInt(pairs->size);
        return pairs->storage[randomRoll];
    }else{
        int i;
        Pair* pickedPair = NULL;

        // Picking a pair at weighted rates.
        randomRoll = randInt(100);

        // Use each pair's weight value to see which one the random roll has picked.
        for(i = 0; i < pairs->size; i++){
            pickedPair = pairs->storage[i];

            // Did the random roll fall into the weight range of this pair?
            if(randomRoll - (int)(intptr_t)(pickedPair->value2) <= 0)
                // Yes, we found the pair that was chosen.
                return pickedPair;
            else
                // No, the rolled value falls outside the range of the this pair's
                // weight.  Take this pair's weight into consideration when examining
                // the next pair.
                randomRoll -= (int)(intptr_t)(pickedPair->value2);
        }

        // Unable to pick a pair because all weight values add up to less than the
        // randomly rolled value.  Simply return the last encountered pair.
        return pickedPair;
    }
}

static void swaplong(long* x, long* y){
    long temp = *x;
    *x = *y;
    *y = temp;
}

Entity* entCreateEx(const char* type, EntType ent_type){
    Entity    *e;
    Vec3    crit_pos = {0,0,0}, crit_pyr = {0,0,0};
    int        fly = 0;
    const char    *enttype = type ? type : "Male";
    Mat3    newmat;

    e = entAlloc();

    if(!e){
        printf("WARNING: Out of entities!!!\n");
        return NULL;
    } 

    SET_ENTTYPE(e) = ent_type;

    e->playerLocale = getCurrentLocale();

#ifdef RAGDOLL // ragdoll requires all seq info on server
    entSetSeq(e, seqLoadInst(enttype, 0, SEQ_LOAD_FULL_SHARED, e->randSeed, e ));
#else
    entSetSeq(e, seqLoadInst(enttype, 0, SEQ_LOAD_HEADER_ONLY, e->randSeed, e ));
#endif
    e->randSeed = rand(); //for synchronizing client gfx
    copyVec3(crit_pos,e->seq->gfx_root->mat[3]);

    entSetMat3(e, unitmat);
    entUpdatePosInstantaneous(e, crit_pos);
    if (fly)
        e->move_type = MOVETYPE_NOCOLL;
    else
        e->move_type = MOVETYPE_WIRE;

    createMat3RYP(newmat,crit_pyr);
    entSetMat3(e, newmat);
    playerVarAlloc(e, ent_type);
    e->fRewardScaleOverride = -1.0f;

    return e;
}
/*
 * Misc utilities
 ************************************************************************************/

/************************************************************************************
 * Global generator control
 */
int entgenIsPaused(){
    return generatorsPaused;
}

void entgenPause(){
    generatorsPaused = 1;
}

void entgenResume(){
    generatorsPaused = 0;
}
/*
 * Global generator control
 ************************************************************************************/

///************************************************************************************
// * GeneratorTrigger related stuff
// */
///*
//GeneratorTrigger* createGeneratorTrigger(Mat4 position, float radius){
//    GeneratorTrigger* trigger = calloc(1, sizeof(GeneratorTrigger));
//    copyMat4(position, trigger->mat);
//    trigger->triggerRadius = radius;
//    return trigger;
//}
//
//void destroyGeneratorTrigger(GeneratorTrigger* trigger){
//}
//*/
/*
 * GeneratorTrigger related stuff
 ************************************************************************************/





/************************************************************************************
 * Generator Triggering/Activation related stuff
 *    Defines what a generator does once it is "triggerred".  Mainly, it goes through
 *    a series of checks to see if it is possible for the given entity to create an
 *    entity.
 */

// Returns 1 if the generator can generate something.
int entgenCheckLimiterStep1(GeneratorInst* gen){
    extern int player_count;
    GeneratorDef* generatorDef = gen->def;

    //    Reached the active entity cap for this generator?
    if(gen->childCount >= generatorDef->maxActiveEnts)
        return 0;

    //    Reached the spawn cap for this generator?
    //    Don't bother checking if the generator is supposed to be able to generate
    //    inifinite number of entities.
    if(-1 != gen->maxSpawnCount){
        if(gen->curSpawnCount >= gen->maxSpawnCount){
            // Deactivate this generator.  It can no longer spawn things on its own.
            gen->active = 0;
            return 0;
        }
    }

    //    Reached the entity cap for this spawn area?
    if(gen->spawnArea->curActiveEnts >= gen->spawnArea->def->maxActiveEnts)
        return 0;

    //    Reached critter spawn count cap?
    //    Ignore the cap if the value is -1, which means the spawn area is allowed to spawn infinite number of
    //    critters.
    if(gen->spawnArea->maxCritterSpawnCount != -1 && gen->spawnArea->curCritterSpawnCount >= gen->spawnArea->maxCritterSpawnCount * player_count)
        return 0;

    return 1;
}

int entgenCheckLimiterStep2(GeneratorInst* gen, GeneratedEntDef* generatedDef){
    if(generatedDef->curActiveEnts >= generatedDef->maxActiveEnts)
        return 0;

    //    Reached the entity stereotype cap?
    if(ENTTYPE_CRITTER == generatedDef->entType){
        // The entity being created is a critter.

        // The spawn area is allowed to have up to maxCritterEnts number of entities in play simultaneously.
        if(gen->spawnArea->curCritterEnts >= gen->spawnArea->def->maxCritterEnts)
            return 0;

        // FIXME!!!
        // Why is this check here? This is already done in entgenCheckLimiterStep1!
        // The spawn area is allowed to spawn up to a total of "maxCritterPerPlayer * player_count number of entities.
        if(gen->spawnArea->curCritterSpawnCount >= (int)gen->spawnArea->def->maxCritterPerPlayer * player_count)
            return 0;
    }
    else if(ENTTYPE_NPC == generatedDef->entType){
        // The entity being created is an NPC/car
        if(gen->spawnArea->curNPCEnts >= gen->spawnArea->def->maxNPCEnts)
            return 0;
    }

    return 1;
}

void entgenGenerateSingle(GeneratorInst* gen, int skipLimiters){
    GeneratedEntDef* generatedDef;
    Pair* pair;

    assert(gen && gen->def && gen->def->spawnType == GSTSingle);

    // Generate an entity.
    // Pick an entity that the generator is capable of generating.
    if(gen->parentGenerator)
        pair = pickPercentageWeightedPair(&gen->parentGenerator->def->generatedDefs);
    else
        pair = pickPercentageWeightedPair(&gen->def->generatedDefs);

    assert(pair);

    // Got a pair that defines the generatedType and the generation percentage.
    generatedDef= pair->value1;

    //    Reached the entity cap for this type of entity?
    if(!skipLimiters && !entgenCheckLimiterStep2(gen, generatedDef)){
        return;
    }

    generateEntityRaw(generatedDef, gen);
}

void entgenGenerateSpawnGroup(GeneratorInst* gen, int skipLimiters){
    GeneratedGroupDef* generatedDef;
    Pair* pair;
    int i;

    assert(gen->parentGenerator);

    // Generate a group of entities.
    // Pick an entity that the generator is capable of generating.
    if(gen->parentGenerator)
        pair = pickPercentageWeightedPair(&gen->parentGenerator->def->generatedDefs);
    else
        pair = pickPercentageWeightedPair(&gen->def->generatedDefs);
    assert(pair);

    // Got a pair that defines the generatedGroupDef and the generation percentage.
    generatedDef= pair->value1;

    {
        int spawnRoll;
        int getCappedRandom(int);

        // Choose the groups to be spawned.
        spawnRoll = getCappedRandom(100);

        // Spawn all the group memebers that this roll allows.
        for(i = 0; i < generatedDef->generatedGrpMemberDefs.size; i++){
            // If the random roll is large enough to allow this group to be spawned...
            if(spawnRoll > 0){
                GeneratedGrpMemberDef* def;

                def = generatedDef->generatedGrpMemberDefs.storage[i];

                // Update spawn roll.
                // Some amount of "energy" is used up by spawning this group.
                // FIXME!!!
                // "spawnPercentage" is not a good name for the store value.
                // It should be some like "activationEnergy".
                spawnRoll -= def->spawnPercentage;

                generateSpawnGroupMember(def, gen);
            }
        }
    }

}

/* Function entgenGenericTrigger
 *    This function checks the given generator against the various limiter defined
 *    by its generator definition as well as the spawn area it is in.  If all checks
 *    passes, an entity is generated and various accounting variables are updated.
 *
 */
void entgenGenericTrigger(GeneratorInst* gen, int skipLimiters){
    GeneratorDef* generatorDef;

    generatorDef = (GeneratorDef*)gen->def;

    // Reset activation timer.
    gen->timeToActivation = generatorDef->activationInterval;

    // Before generating an entity of this type, check if some entity cap
    // has been hit.

    //    Reached the entity cap for server?
    //  This limiter cannot be bypassed.
    if(gen->spawnArea->curActiveEnts > MAX_ENTITIES_PRIVATE)
        return;

    if(!skipLimiters && !entgenCheckLimiterStep1(gen))
        return;

    // Make sure the generator def type field has been filled in so
    // we know if we are supposed to spawn a single entity or an entire
    // group.
    if(gen->def->spawnType == GSTInvalid)
        Errorf("SpawnArea error: Generator \"%s\" does not have valid entity types to spawn\n", gen->def->name);

    if(gen->def->pointType == GPTSingle){
        if(gen->def->spawnType == GSTSingle){
            entgenGenerateSingle(gen, skipLimiters);
        }

        if(gen->def->spawnType == GSTGroup){
            entgenGenerateSpawnGroup(gen, skipLimiters);
        }
    }

    if(gen->def->pointType == GPTMultiple){
        int spawnCount;        // How many points in the generator group needs to be activated?
        int activateAllPoints = 0;

        {
            int activePoints = 0;    // How many points are currently active?
            int i;

            if(!gen->spawnPoints)
                return;
            for(i = 0; i < gen->spawnPoints->size; i++){
                GeneratorInst* childGenerator;

                childGenerator = gen->spawnPoints->storage[i];
                if(childGenerator->childCount)
                    activePoints++;
            }

            spawnCount = gen->def->maxActiveEnts - activePoints;

            // If all possible spawn points
            if(activePoints == gen->spawnPoints->size)
                return;
            if(gen->def->maxActiveEnts >= (unsigned int)gen->spawnPoints->size){
//                if(gen->def->maxActiveEnts > (unsigned int)gen->spawnPoints->size)
//                    printf("GeneratorGroup at %f %f %f reqested activation on %i points but there are only %i spawn points available\n",
//                        gen->mat[3][0], gen->mat[3][0], gen->mat[3][0], gen->def->maxActiveEnts, gen->spawnPoints->size);

                activateAllPoints = 1;
            }
        }


        if(spawnCount > 0){
            int i;

            // Activate the reqested number of spawn points.
            for(i = 0; i < spawnCount; i++){
                GeneratorInst* childGenerator;

                // If not all spawn points are being activated, then pick the points
                // at random and activate them if possible.
                if(!activateAllPoints){
                    int randomPoint;

                    // Get a random child generator.
                    randomPoint = getCappedRandom(gen->spawnPoints->size);
                    childGenerator = gen->spawnPoints->storage[randomPoint];

                    // Ignore the child generator if it is already occupied.
                    if(childGenerator->childCount){
                        i--;
                        continue;
                    }
                }else{
                    // Otherwise, all spawn points are to be activated.  Just go
                    // through all points sequencially.
                    if(i >= gen->spawnPoints->size)
                        break;
                    childGenerator = gen->spawnPoints->storage[i];
                }


                if(gen->def->spawnType == GSTSingle){
                    entgenGenerateSingle(childGenerator, skipLimiters);
                }

                if(gen->def->spawnType == GSTGroup){
                    entgenGenerateSpawnGroup(childGenerator, skipLimiters);
                }
            }
        }
    }
}

void entgenTrigger(GeneratorInst* gen){
    if(gen->active)
        entgenGenericTrigger(gen, 0);
}

void entgenForceTrigger(GeneratorInst* gen){
    entgenGenericTrigger(gen, 0);
}

// Will work with new spawning scheme.
Entity* generateEntityRaw(GeneratedEntDef* generatedType, GeneratorInst* gen){
    Entity* ent;
    char* villainName;

    Pair* pair;
    pair = pickPercentageWeightedPair(&generatedType->NPCTypeName);
    villainName = pair->value1;

    if(server_state.printEntgenMessages){
        printf("|--------------------------------------------------\n");
        printf("| Generating entity\n");
        printf("|    Name: %s\n", generatedType->name);
        printf("|    Villain name: %s\n", villainName);
    }

    ent = npcCreate(villainName, generatedType->entType);

    if(!ent){
        Errorf("Generator %s: Unknown NPC name: %s\n", gen->def->name, villainName);
        return NULL;
    }

    if (generatedType->entType == ENTTYPE_NPC || generatedType->entType == ENTTYPE_CAR)
    {
        ent->exclusiveVisionPhaseDefault = EXCLUSIVE_VISION_PHASE_ALL_BUT_NO_TRAFFIC;
    }

    if(server_state.printEntgenMessages){
        printf("|    SeqName: %s\n", ent->seq->type->name);
        printf("|    EntIndex: %d\n", ent->owner);
    }

    // The current spawn area is the spawn area the generator is in.
    if(gen){
        curSpawnArea = gen->spawnArea;
        entUpdateMat4Instantaneous(ent, gen->mat);
    }

    initEntityAIType(ent, generatedType);

    if(gen)
        curSpawnArea = NULL;

    if(gen){
        ent->parentGenerator = gen;
        gen->childCount++;
        gen->curSpawnCount++;

        // Fade the entity in and create Entity Spawning Fx
        if(gen->def->spawnFxName)
        {
            NetFx netfx = {0};
            netfx.command    = CREATE_ONESHOT_FX;
            netfx.handle    = stringToReference( gen->def->spawnFxName );
            entAddFx( ent, &netfx);
        }

        if(ENTTYPE(ent) == ENTTYPE_CRITTER){
            gen->spawnArea->curCritterSpawnCount++;

            // TODO: This spawn command is a mess. Why isn't it using the
            // villain create stuff?

            // Give some random villain def so other code works.
            ent->villainDef = villainFindByName("5thColumn_Fog_Soldier");
        }
        /* NPCs always float off of the ground on the server.  The client performs a magic number offset
           so that they appear to be walking on the ground.  This works because most beacons are 2 feet off
           the ground.  There should probably be a more robust way to deal with this, in case someone messes up
           some beacons.
        */
    }

    if(server_state.printEntgenMessages){
        printf("|    Pos: %f %f %f\n", ENTPOSPARAMS(ent));
        printf("|    NetPos: %f %f %f\n", ent->net_pos[0], ent->net_pos[1], ent->net_pos[2]);
        printf("|--------------------------------------------------\n");
    }

    if( !gen )
        ent->fade = 1;
    else if (!gen->def->nofade)
        ent->fade = 1;

    return ent;
}

static GeneratedEntDef* getGeneratedEntDef(char* GeneratedEntDefName){
    GeneratedEntDef* generatedType;

    generatedType = hashStackFindValue(generatedEntDefHashStack, GeneratedEntDefName);

    return generatedType;
}

/* Function generateEntity()
 *    This function generates an entity corresponding to the given GeneratedEntDefName.
 *    The name must have been defined in the currently active spawn area files for this
 *    function to work.
 *
 *    Returns:
 *        NULL - No such GeneratedEntDef was found.  Cannot generate entity.
 *        Valid Entity* - GeneratedEntDef was found and entity generated.
 */
Entity* generateEntity(char* GeneratedEntDefName){
    GeneratedEntDef* generatedType;

    generatedType = getGeneratedEntDef(GeneratedEntDefName);

    if(!generatedType)
        return NULL;

    return generateEntityRaw(generatedType, NULL);
}
/*
Entity* generateEntityEx(char* SpawnAreaName, char* GeneratedEntDefName){
    SpawnAreaDef* def;

    // Find the given spawn area.
    if(SpawnAreaName){
        def = hashStackFindValue(spawnAreaDefHashStack, SpawnAreaName);
        // A bad SpawnArea name was found?
        assert(def);

        entgenAddToLookupTables(def);
    }

    generatedType = getGeneratedEntDef(GeneratedEntDefName);
    if(!generatedType)
        return 0;

    return generateEntityRaw(generatedType, NULL);
}
*/

static void generateSpawnGroupMember(GeneratedGrpMemberDef* def, GeneratorInst* gen){
    int spawnCount;
    int i;

    if(def->spawnMax == 0)
        return;

    // Calculate how many entities are to be spanwed for the given spawn group memeber.
    if(def->spawnMax == def->spawnMin)
        spawnCount = def->spawnMax;
    else{
        spawnCount = def->spawnMin + getCappedRandom(def->spawnMax - def->spawnMin);
    }


    // Spawn as many entities as required.
    for(i = 0; i < spawnCount; i++){
        Pair* pair;

        // Pick the type of entity to be spawned.
        pair = pickPercentageWeightedPair(&def->generatedEntDefs);
        assert(pair);

        if(!entgenCheckLimiterStep2(gen, pair->value1))
            return;

        generateEntityRaw(pair->value1, gen);
    }
}
/*
 * Generator Activation related stuff
 ************************************************************************************/

/************************************************************************************
 * Generator Processing related stuff
 */

// checks every entity generator to see if more entities should be spawned.
void entgenProcess(){
    const int triggerTime = 30;
    static int trigger = 30;
    int i;
    int entCount = 0;
    GeneratorInst* gen;

    if(generatorsPaused)
        return;

    // continue with the rest of the function only once in every so often
    if(trigger--)
        return;
    else
        trigger = triggerTime;

    // If the randomized array isn't initalized, initialize it.
    if(0 == randomizedGenerators.size){
        // Store generator pointers in the order they were loaded into the game.
        for(i = 0; i < generators.size; i++){
            arrayPushBack(&randomizedGenerators, generators.storage[i]);
        }
    }

    // Randomize the generator order
    for(i = randomizedGenerators.size - 1; i > 0; i--){
        // Decide which element in the array to swap with.
        int swapIndex = getCappedRandom(i);
        swaplong((long*)&randomizedGenerators.storage[i], (long*)&randomizedGenerators.storage[swapIndex]);
    }

    // Process all known generators.
    for(i = 0; i < randomizedGenerators.size; i++){
        gen = randomizedGenerators.storage[i];
        entgenTrigger(gen);
    }
}

/*
 * Generator Processing related stuff
 ************************************************************************************/


/************************************************************************************
 * Entity AI initialization
 *    Initializes/attaches AI to an existing entity
 */

static int initEntityAIType(Entity* ent, GeneratedEntDef* generatedType){
    assert(ent && generatedType);

    // What's the basic ENTTYPE?
    SET_ENTTYPE(ent) = generatedType->entType;

    // Store some info for SpawnArea accounting.
    ent->entityDef= generatedType;
    ent->parentGenerator = NULL;

    // If a generator has been triggered, the current spawn area will point to the
    // spawn area that the generator is in.  Update that spawn area's variables.
    if(curSpawnArea){
        // Update active entity count in various places.
        if(ENTTYPE_CRITTER == ENTTYPE(ent))
            curSpawnArea->curCritterEnts++;
        else if(ENTTYPE_NPC == ENTTYPE(ent))
            curSpawnArea->curNPCEnts++;
        curSpawnArea->curActiveEnts++;
    }

    generatedType->curActiveEnts++;

    aiInit(ent, generatedType);

    return 1;
}



/* Function initEntityAI
 *    Initializes/Attaches an AI to an existing entity.  This function sets up some variables
 *    need by the AI script to function correctly.
 *
 *    Returns:
 *        0 - initialization failed.
 *        1 - initialization completed successfully.
 *
 */
// Depend on which spawn area you're in...
int initEntityAI(Entity* ent, char* GeneratedEntDefName){
    GeneratedEntDef* generatedType;

    generatedType = getGeneratedEntDef(GeneratedEntDefName);

    if(!generatedType)
        return 0;

    return initEntityAIType(ent, generatedType);
}
/*
 * Entity AI initialization
 *    Initializes/attaches AI to an existing entity
 ************************************************************************************/



void createClones(Entity* ent, int num){
    int i;


    for(i = 0; i < num; i++){
        Entity* clone;
        Vec3 newpos;

        // Create an entity of the same body type as the given entity.
        clone = entCreateEx(ent->seq->type->name, ENTTYPE_HERO);

        // Copy all vars of the given entity.

        copyVec3(ENTPOS(ent), newpos);
        newpos[2] += 5;
        entUpdatePosInstantaneous(clone, newpos);
        clone->motion->last_pos[2] += 5;
        clone->send_costume = TRUE; //a newly sent created entitiy should do this automatically
        SET_ENTTYPE(clone) = ENTTYPE_HERO;

        initEntityAI(clone, "HeroHelper");

    }

}
