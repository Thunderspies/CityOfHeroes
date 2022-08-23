#include "missionMapInit.h"
#include <utilitieslib/components/ArrayOld.h>
#include <utilitieslib/components/StashTable.h>
#include "generator/entgen.h"
#include "generator/entgenLoader.h"
#include "comm_backend.h"

#include <utilitieslib/components/HashTableStack.h>
#include "group/group.h"
#include "group/groupProperties.h"
#include "cmdparse/cmdserver.h"
#include "group/grouputil.h"
#include <utilitieslib/utils/RegistryReader.h>
#include "group/groupnetsend.h"
#include "utils/debugUtils.h"

#include "entity/entity.h"
#include <utilitieslib/components/StringCache.h>
#include <utilitieslib/utils/error.h>
#include "beacon/beacon.h"
#include "generator/encounter.h"
#include "storyarc/mission.h"
#include "gameSys/dooranim.h"
#include "dbcomm/staticMapInfo.h"
#include "storyarc/locationTask.h"
#include "gameSys/sgraid.h"
#include "dbcomm/dbcomm.h"
#include "ai/entai.h"
#include "zowie.h"
#include "cmdparse/autoCommands.h"

#include <string.h>
#include <utilitieslib/assert/assert.h>

#define if_log_mapinit(x)

static char* ValidMissionNames[]=
{
    "Combat",                    
    "Bomb",                        
    "Hostage",                    
    "ChaseVillain",                
    "Ambush",            
    "GuardLocation",            
    "DeliverItemToItem",        
    "RecoverObjects",            
    "RecoverClues",                
};

int setMissionType(char* name){

    return 0;
}

#include "entity/entity.h"
int groupDefIsSAOverride(GroupDef* def){


    return 0;
}


// Contains arrays of scenarios that are stored by the scenario type name.
static StashTable scenariosByType;

// Contains a list of scenario, stored in the form of GroupDefTraverser, to be activated.
static Array* scenarioActivationList;

// contains a list of subtrees that were manually marked for activation.
static Array* scenarioActivationRequestList;

// Contains a list of scenario, stored in the form of GroupDefTraverser, to be activated.
static Array* spawnAreaActivationList;

// Contains a list of GroupDefTraversers that are candidates for activation.
static Array* activationCandidates;


static int mapInitialized = 0;


/**************************************************************************************************
 * Hash Table Stack Pool
 *    Stores a list of pre-allocated arrays that are not in use.  This allows
 *    faster array "allocation" and "deallocation".
 *
 *    The candidate extraction process allocates/deallocates arrays at
 *    a very high frequency, which slows down the map loading process ALOT.
 *    The pool gets around this by reusing existing arrays.
 *
 *    ENHANCE ME!!!
 *        This "object pool" seems like something that might be useful on its
 *        own.  Consider consolidating the different memory management modules,
 *        such as, "MemoryPool" and "MemoryBank".  All three of these things
 *        seem to do pretty much the same thing.
 */

Array* HashTableStackPool;
static void htspInitialize(){
    HashTableStackPool = createArray();
    initArray(HashTableStackPool, 36);
}

static void htspShutdown(){
    destroyArrayEx(HashTableStackPool, (Destructor)stashTableDestroyStack);
}

static HashTableStack htspCreateHashTableStack(){
    HashTableStack stack;
    static int i = 0;

    // Try to get an exist array from the pool.
    stack = (HashTableStack)arrayPopBack(HashTableStackPool);

    // If there are no more arrays in the pool, create one.
    if(!stack){
        stack = createHashTableStack();
        initHashTableStack(stack, 4);
        i++;
    }

    return stack;
}

static void htspDestroyHashTableStack(HashTableStack stack){
    assert(stack);
    stashTableClearStack(stack);
    arrayPushBack(HashTableStackPool, (void*)stack);
}
/*
 * Hash Table Stack Pool
 *************************************************************************************************/
/*************************************************************************************************
 * SpawnArea stack pool
 *    The SpawnAreaInitStack stores spawn areas that are currently being initialized.
 */
Array* SpawnAreaStackPool;
static void sasInitialize(){
    SpawnAreaStackPool = createArray();
    initArray(SpawnAreaStackPool, 36);
}

static void sasShutdown(){
    destroyArrayEx(SpawnAreaStackPool, destroyArray);
}

static Array* sasCreateSpawnAreaStack(){
    Array* stack;
    static int i = 0;

    // Try to get an exist array from the pool.
    stack = arrayPopBack(SpawnAreaStackPool);

    // If there are no more arrays in the pool, create one.
    if(!stack){
        stack = createArray();
        initArray(stack, 4);
        i++;
    }

    return stack;
}

static void sasDestroySpawnAreaStack(Array* stack){
    assert(stack);
    clearArray(stack);
    arrayPushBack(SpawnAreaStackPool, (void*)stack);
}
/*
 * SpawnArea stack
 *************************************************************************************************/

/*************************************************************************************************
 * Mission Map Init GroupDef traverser context.
 */
Array* MissionMapInitContextPool;
void MissionMapInitContextInitialize(){
    // Startup the MissionMapInitContext
    MissionMapInitContextPool = createArray();
    initArray(MissionMapInitContextPool, 36);

    // Startup the hashtable stack pool
    htspInitialize();

    // Startup the spawn area stack pool
    sasInitialize();
}

typedef struct{
    HashTableStack    propertyStack;
    Array*            spawnAreaStack;
} MissionMapInitContext;

MP_DEFINE(MissionMapInitContext);

void MissionMapInitContextFree(MissionMapInitContext *data) {
    MP_FREE(MissionMapInitContext, data);
}

void MissionMapInitContextShutdown(){
    // Shutdown the MissionMapInitContext
    destroyArrayEx(MissionMapInitContextPool, (Destructor)MissionMapInitContextFree);

    // Shutdown the hashtable stack pool
    htspShutdown();

    // Shutdown the spawn area stack pool
    sasShutdown();
}

static MissionMapInitContext* mmicCreate(){
    MissionMapInitContext* context;
    static int i = 0;

    context = arrayPopBack(MissionMapInitContextPool);

    if(!context){
        MP_CREATE(MissionMapInitContext, 16384);
        context = MP_ALLOC(MissionMapInitContext);
        i++;
    }

    context->propertyStack = htspCreateHashTableStack();
    context->spawnAreaStack = sasCreateSpawnAreaStack();

    return context;
}

static void mmicCopy(MissionMapInitContext* src, MissionMapInitContext* dst){
    hashStackCopy(src->propertyStack, dst->propertyStack);
    arrayCopy(src->spawnAreaStack, dst->spawnAreaStack, NULL);
}

static int destroyCount = 0;
static void mmicDestroy(MissionMapInitContext* context){
    
    assert(context);
    htspDestroyHashTableStack(context->propertyStack);
    sasDestroySpawnAreaStack(context->spawnAreaStack);
    arrayPushBack(MissionMapInitContextPool, (void*)context);
    destroyCount++;
}

Array* mmicGetSpawnAreaStack(GroupDefTraverser* traverser){
    return ((MissionMapInitContext*)traverser->vContext.context)->spawnAreaStack;
}

HashTableStack mmicGetPropertyStack(GroupDefTraverser* traverser){
    return ((MissionMapInitContext*)traverser->vContext.context)->propertyStack;
}
/*
 *
 *************************************************************************************************/


void initMapSetup(){
    scenarioActivationList = createArray();
    spawnAreaActivationList = createArray();

    if(!scenariosByType){
        scenariosByType = stashTableCreateWithStringKeys(16,  StashDeepCopyKeys | StashCaseSensitive );
    }else
        stashTableClear(scenariosByType);

    MissionMapInitContextInitialize();
}

void initMapCleanup(){
    if(scenarioActivationList)
        destroyArray(scenarioActivationList);

    if(spawnAreaActivationList)
        destroyArray(spawnAreaActivationList);

    if(activationCandidates)
        destroyArrayEx(activationCandidates, destroyGroupDefTraverser);

    MissionMapInitContextShutdown();
/*
    if(scenariosByType)
        stashTableDestroy(scenariosByType);
 */
}

int count[7]={0,0,0,0,0,0,0};

/*****************************************************************************
 * Scenario Extraction
 */
static int scenarioSubtreeExtractionCritera(GroupDefTraverser* traverser){
    PropertyEnt* activateScenarios;
    int isScenario;
    int isSpawnArea;
    int isGenerator;
    int isGeneratorGroup;

    // If the group has no properties, it can't be any of the things we are looking for.
    if(!traverser->def->has_properties) {
        if (!traverser->def->child_properties) {
            return EA_NO_BRANCH_EXPLORE;
        }
        return EA_NO_EXTRACT;
    }

    isScenario = groupDefIsScenario(traverser->def);
    isSpawnArea = groupDefIsSpawnArea(traverser->def);
    isGenerator = groupDefIsGenerator(traverser->def);
    isGeneratorGroup = groupDefIsGeneratorGroup(traverser->def);

    // If the group is a scenario...
    if(isScenario){
        // PERFORMANCE WARNING! It may take a LONG time to do this for every group in a map.
        // It may be a good idea to move this into the voltile context of the traverser.

        // check to see if there are any "ActivateScenarios" flag anywhere along the currently 
        // explored path.  If so, this group is sitting in a location that is appropriate for 
        // activation, add it to the list of candidate scenarios for activation.
        HashTableStack propertyStack = ((MissionMapInitContext*)traverser->vContext.context)->propertyStack;
        activateScenarios = hashStackFindValue(propertyStack, "ActivateScenarios");

        // Activate any "EndZone" scenarios.
        if(!activateScenarios){
            activateScenarios = groupDefFindProperty(traverser->def, "Scenario"); 

            // If the scenario is an "EndZone", activate it.  Do not activate the scenario otherwise.
            if(stricmp(activateScenarios->value_str, "EndZone") && stricmp(activateScenarios->value_str, "Combat")){
                activateScenarios = NULL;
            }
        }


        // If the scenario is appropriate for activation, extract it and do not explore the
        // group's children.  They will be explored when the scenario is being activated.
        // Never explore into a scenario.
        if(activateScenarios) {
            count[0]++;
            return EA_EXTRACT | EA_NO_BRANCH_EXPLORE;
        } else {
            count[1]++;
            return EA_NO_BRANCH_EXPLORE;
        }

    }else if(isSpawnArea){
        // If the group is not a scenario and it is a spawn area, it should always be extracted
        // for activation.
        count[2]++;
        return EA_EXTRACT | EA_NO_BRANCH_EXPLORE;
    }else if(isGenerator || isGeneratorGroup){
        count[3]++;
        return EA_EXTRACT | EA_NO_BRANCH_EXPLORE;
    }else{
        // Extract named markers
        char* markerName;
        char* AIName;
        
        markerName = groupDefFindPropertyValue(traverser->def, "MarkerName");
        if(markerName){
            count[4]++;
            return EA_EXTRACT | EA_NO_BRANCH_EXPLORE;
        }

        AIName = groupDefFindPropertyValue(traverser->def, "MarkerAI");
        if(AIName){
            count[5]++;
            return EA_EXTRACT | EA_NO_BRANCH_EXPLORE;
        }
    }

    count[6]++;
    // By default, the group is not extracted and we explore further into the tree.
    if (!traverser->def->child_properties) {
        return EA_NO_BRANCH_EXPLORE;
    }
    return EA_NO_EXTRACT;
}

int processMapInitContext(GroupDefTraverser* traverser, int groupDefAccepted){
    MissionMapInitContext* context;

    context = traverser->vContext.context;

    // As the traverser moves down the tree, push the GroupDef's properties into
    // the hash stack stored in the traverser's volitile context.
    if(traverser->def->properties){
        hashStackPushTable(context->propertyStack, traverser->def->properties);


        // Also track the spawn area that are currently being initialized.
        // This will be used later to determine which spawn area to attach generators to.
        //    FIXME!!!
        //        This doesn't really need to be a stack stored as the volitile context.
        //        Instead use a persistent context and store spawn area that was most recently
        //        created.
        {
            char* spawnAreaName = NULL;
            SpawnAreaInst* saInst;

            PropertyEnt* spawnAreaProp;

            // Extract the spawn area name
            stashFindPointer( traverser->def->properties, "SpawnArea", &spawnAreaProp );
            
            if(spawnAreaProp){
                //    If a spawn area field is found, this group represents a spawn area.
                spawnAreaName = spawnAreaProp->value_str;
            
                //    Attempt to locate the corresponding spawn area definition.
                saInst = entgenCreateNewSpawnArea(spawnAreaName);

                if(saInst){
                    arrayPushBack(context->spawnAreaStack, saInst);
                }
            }
        }
    }else{
        // There are no properties on this group.
        // Auto-create a spawn area for trays that do not belong to one yet.

        // MAK - I commented this out because there is no spawnarea named "Mission" any more.
        //  This block no longer does anything (if it ever did)

        //if(traverser->def->tray){
        //    if(!hashStackFindValue(mmicGetPropertyStack(traverser), "SpawnArea")){
        //        char* spawnAreaName = NULL;
        //        SpawnAreaInst* saInst;

        //        // If a group does not have a spawn area property assigned but it is a "tray" (a room),
        //        // give it a spawn area called "Mission".
        //        spawnAreaName = "Mission";

        //        //    Attempt to locate the corresponding spawn area definition.
        //        saInst = entgenCreateNewSpawnArea(spawnAreaName);

        //        if(saInst){
        //            arrayPushBack(context->spawnAreaStack, saInst);
        //        }
        //    }
        //}
    }



    return 1;
}



void initMapExtractActivationCandidates(){
    GroupDefTraverserVContext vContext;
    MissionMapInitContext* context;
    //int i;

    context = mmicCreate();
    vContext.context = (void*) context;
    vContext.createContext = (void* (*)()) mmicCreate;
    vContext.destroyContext = (void (*)(void*)) mmicDestroy;
    vContext.copyContext = (void (*)(void*, void*)) mmicCopy;
    
    arrayPushBack(context->spawnAreaStack, globalSpawnArea);

    // Extract all group info subtrees that are candidates for activation.
    //    Use subtreeExtractionCritera() to extract only SpawnAreas and Scenarios.

    activationCandidates = groupExtractSubtreesEx(&group_info, (ExtractionCriteria)scenarioSubtreeExtractionCritera, processMapInitContext,
                                                    NULL, &vContext);
    //printf("%d\n", activationCandidates->size);
    //for (i=0; i<6; i++) printf("%d\n", count[i]);

}
/*
 * Scenario Extraction
 *****************************************************************************/


///*****************************************************************************
 // * Activation Request processing
 // *    It is possible to explicitly request particular scenarios to be activated.
 // *    This is mostly intended to make level design/pacing easier by allowing
 // *    level designers to specify which combat scenarios are to be activated in
 // *    which parts of the level.
 // *
 // *    What is an Activation Request?
 // *    An activation request is specified using an "activation" node in the group
 // *    info tree.  An activation node a group def node that has a type name in 
 // *    the form of "Active_<scenario name>".  
 // *
 // *    What does an Activation Request do?
 // *    An Activation Requset specifies that at least one of the children of the 
 // *    activation node is of the given scenario type and it is to be activated 
 // *    reguardless of the mission type.
 // *
 // */
 //static int activationRequestSubtreeExtractionCritera(GroupDef* def){
 //    if(!def->type_name)
 //        return 0;
 //
 //    if(strnicmp(def->type_name, "Activate_", strlen("Activate_")) == 0)
 //        return 1;
 //    else
 //        return 0;
 //}
 //
 ///* Function initMapExtractActivationRequests()
 // *    This function is used to extract a list of "activate" nodes.  
 // *
 // *
 // */
 //static void initMapExtractActivationRequests(){
 //    scenarioActivationRequestList = groupExtractSubtrees(&group_info, activationRequestSubtreeExtractionCritera);
 //}
 //
 //static char* activationRequestScenarioType;
 //static int activationRequestServed;
 //static int activateRequestedScenarios(GroupDefTraverser* traverser){
 //    char* scenarioType;
 //
 //    // Don't explore into SpawnAreas.  They don't contain anything useful.
 //    if(groupDefIsSpawnArea(traverser->def))
 //        return 0;
 //
 //    // Keep exploring the tree until we've found a scenario to work with.
 //    if(!groupDefIsScenario(traverser->def))
 //        return 1;
 //    
 //    // At this point, the traverser holds a group def that has been marked
 //    // as a scenario.
 //    // Extract the scenario name of the current scenario under examination.
 //    scenarioType = traverser->def->type_name + strlen("Scenario_");
 //
 //    // Check scenario name against the scenario to be activated.
 //    if(0 == stricmp(activationRequestScenarioType, scenarioType)){
 //        arrayPushBack(scenarioActivationRequestList, traverser);
 //        activationRequestServed = 1;
 //    }
 //
 //    return 0;
 //}
 //
 //static void initMapProcessActivationRequests(){
 //    char* scenarioType;  // Which scenarios are to be activated?
 //    int i;
 //
 //    for(i = 0; i < scenarioActivationRequestList->size; i++){
 //        GroupDefTraverser* traverser = scenarioActivationRequestList->storage[i];
 //
 //        
 //        // What kind of scenarios are being requested for activation?
 //        scenarioType = traverser->def->type_name;
 //        scenarioType += strlen("Activate_");
 //
 //        activationRequestServed = 0;
 //
 //        // Try to activate a scenario of the given type under the activation node.
 //        groupProcessDefEx2(traverser, activateRequestedScenarios);
 //
 //        // If the activation request cannot be fulfilled.
 //        if(!activationRequestServed){
 //            log_printf("MapInit", "Cannot fulfill %s scenario activation request at %f %f %f\n",
 //                                    scenarioType, traverser->mat[3][0], traverser->mat[3][1], traverser->mat[3][2]);
 //        }
 //    }
 //}
 //
 //
 ///* Activation Request processing
 // *****************************************************************************/
 




/*****************************************************************************
 * Scenario Activation Utilities
 *    These utilities simplifies the scenario activation.
 */
static int initMapActivateAllScenariosOfType(char* scenarioName){
    Array* scenarioList;
    int i;

    assert(scenarioName);

    // Retrieve a list of all scenarios with the given name.
    stashFindPointer( scenariosByType, scenarioName, &scenarioList );
    
    // If no such scenarios can be found, report it in a log file.
    //assert(scenarioList);
    if(!scenarioList){
        printf("No %s scenarios found.\n", scenarioName);
        printf("ERROR! MapInit got an activation request for all %s scenarios found, but no matching scenarios are found.\n", scenarioName);
        return 0;
    }
    
    
    // Add all scenarios to the activation list.
    for(i = 0; i < scenarioList->size; i++){
        arrayPushBack(scenarioActivationList, scenarioList->storage[i]);
    }

    return scenarioList->size;

}

static int initMapActivateOneScenarioOfType(char* scenarioName){
    Array* scenarioList;
    int getCappedRandom(int);

    // Retrieve a list of all scenarios of the given name.
    stashFindPointer( scenariosByType, scenarioName, &scenarioList );
    
    // If no such scenarios can be found, report it in a log file.
    if(!scenarioList){
        log_printf("MapInit", "No %s scenarios found.\n", scenarioName);
        printf("ERROR! MapInit got an activation request for 1 %s scenario found, but no matching scenarios are found.\n", scenarioName);
        return 0;
    }
    
    //    Add the randomly picked EndZone to the activation list.
    arrayPushBack(scenarioActivationList, scenarioList->storage[getCappedRandom(scenarioList->size)]);
    return 1;
}

static int initMapActivateScenariosOfType(char* scenarioName, int activationCount){
    Array* scenarioList;
    Array* chosenValues;
    int i;

    if(activationCount <= 0){
        log_printf("MapInit", "A request for %i %s scenarios found. Ignoring request.", activationCount, scenarioName);
        printf("ERROR! MapInit got an activation request for %i %s scenarios found. Ignoring request", activationCount, scenarioName);

        return 1;
    }

    stashFindPointer( scenariosByType, scenarioName, &scenarioList );
    
    // If the requested scenario type does not exist on the map, tt is impossible to run
    //    an item recovery on this map.
    if(!scenarioList){
        log_printf("MapInit", "No %s scenarios found.\n", scenarioName);
        printf("ERROR! MapInit got an activation request for %i %s scenarios found, but no matching scenarios are found.", activationCount, scenarioName);
        return 0;
    }

    //    Reqeusting more activations than there are scenarios?
    if(activationCount > scenarioList->size){
        log_printf("MapInit", "Not enough %s scenarios to fulfill activation request.\n\
                                 Requested %i activations but only %i secenarios were found.\n", 
                                 scenarioName, activationCount, scenarioList->size);

        printf("ERROR! MapInit got a request for %i %s scenarios but there are only %i on the map\n", activationCount, scenarioName, scenarioList->size);
        return 0;
    }

    //    Randomly pick and mark a specified number of scenarios for activation.
    chosenValues = createArray();    // Tracks which scenarios have already been used.
    for(i = 0; i < activationCount; i++){
        int randomValue;
        int findResult;

        // Pick a random recovery scenario for activation.
        //    Generate a new random value.
        while(1){
            randomValue = getCappedRandom(scenarioList->size);
            
            //    Make sure we haven't used this scenario before.
            findResult = arrayFindElement(chosenValues, (void*)(intptr_t)randomValue);
            
            //    This scenario has not been used yet.
            if(-1 == findResult){
                arrayPushBack(chosenValues, (void*)(intptr_t)randomValue);
                arrayPushBack(scenarioActivationList, scenarioList->storage[randomValue]);
                break;
            }

            //    This scenario has already been used.  Try to get another one.
        }
    }
    destroyArray(chosenValues);

    return activationCount;
}
/*
 * Scenario Activation Utilities
 *****************************************************************************/

static int scenarioTypeStatPrinter(StashElement element){
    Array* array;
    array = stashElementGetPointer(element);

    if(array && array->size)
        log_printf("MapInitDetails", "\t%i %s scenarios\n", array->size, stashElementGetStringKey(element));

    return 1;
}

void initMapSortActivationCandidatesByType(){
    int i;

    // Sort all found candidates by their type.
    for(i = 0; i < activationCandidates->size; i++){
        GroupDefTraverser* traverser = activationCandidates->storage[i];

        // The traverser might be pointing at some sort of Scenario.
        // Organize the traverser by its scenario type if possible.
        {
            PropertyEnt* scenarioProp;
            char* scenarioType;
            Array* scenarioList;

            // Extract scenario type;
            stashFindPointer( traverser->def->properties, "Scenario", &scenarioProp );

            // 
            if(scenarioProp){
                scenarioType = scenarioProp->value_str;
                
                // Add the scenario to a list of other scenarios of the same type.
                //    Find the list of scenarios of the matching type.
                stashFindPointer( scenariosByType, scenarioType, &scenarioList );
                
                // If no such list can be found, the is the first scenario of its type
                // to be processed.
                if(!scenarioList){
                    // Create a new list
                    scenarioList = createArray();
                    
                    // Add the list to the hashtable for lookup later.
                    stashAddPointer(scenariosByType, scenarioType, scenarioList, false);
                }
                
                // At this point, we definitely have a valid scenario list for holding scenarios
                // of this type.
                arrayPushBack(scenarioList, traverser);
                continue;
            }
        }

        // All Spawn Areas are to be activated.  There is no need to sort them.
        //    When a spawn area property is specified in the same group as a scenario property,
        //    These following statements are never executed, because such spawn areas should
        //    should only be activated when the scenario is activated.
        if(groupDefIsSpawnArea(traverser->def)){
            arrayPushBack(spawnAreaActivationList, traverser);
        }else if(groupDefIsGenerator(traverser->def)){
//            printf("Adding single generator to spawn area %s\n", ((SpawnAreaInst*)arrayGetBack(((MissionMapInitContext*)traverser->vContext.context)->spawnAreaStack))->def->name);
            // Generators that are not enclosed in spawn areas should always be activated.
            loadSingleGenerator(traverser);
        }else if(groupDefIsGeneratorGroup(traverser->def)){
//            printf("Adding group generator to spawn area %s\n", ((SpawnAreaInst*)arrayGetBack(((MissionMapInitContext*)traverser->vContext.context)->spawnAreaStack))->def->name);
            loadGeneratorGroup(traverser);
        }

        // Any named markers that are not enclosed in a scenario should be activated.
        if(traverser->def->properties){            
            Position* pos;
            char* markerName;

            markerName = groupDefFindPropertyValue(traverser->def, "MarkerName");
            if(markerName){
                pos = calloc(1, sizeof(Position));
                copyMat4(traverser->mat, pos->mat);
                stashAddPointer(namedSpatialMarkers, markerName, pos, false);
            }
        }
    }

    if_log_mapinit(
        if(0 == stashGetValidElementCount(scenariosByType)){
            log_printf("MapInitDetails",     "No scenarios found.\n");
        }else{
            log_printf("MapInitDetails", "Begin map scenario info\n");
            stashForEachElement(scenariosByType, scenarioTypeStatPrinter);
            log_printf("MapInitDetails", "End map scenario info\n");
        }
    );
}




static void initMapChooseActivationCandidates()
{
    // Decide which canididates are really going to be activated.
    //    The kind of scenarios to be activated depend on the mission type.
}

static int activateChosenScenarios(GroupDefTraverser* traverser){
    GroupDef* def = traverser->def;

    // Activate Spatial Markers
    if(def->has_properties){
        PropertyEnt* prop;

        if(prop = stashFindPointerReturnPointer(def->properties, "MarkerName")){
            // Initialize named markers
            Position* pos;
            char* markerName;

            markerName = prop->value_str;
            pos = calloc(1, sizeof(Position));
            copyMat4(traverser->mat, pos->mat);
            stashAddPointer(namedSpatialMarkers, markerName, pos, false);
        }

        // Found a spawn area?
        if(groupDefIsSpawnArea(def)){
            // Add it to the list of spawn areas to be activated.
            arrayPushBack(spawnAreaActivationList, traverser);
            
            // Don't go further into the tree.
            // Spawn Areas will be activated later.
            return 0;
        }else if(groupDefIsGenerator(traverser->def)){
            loadSingleGenerator(traverser);
        }else if(groupDefIsGeneratorGroup(traverser->def)){
            loadGeneratorGroup(traverser);
        }
    }



    return 1;
}

void initMapActivateChosenScenarios(){
    int i;
    GroupDefTraverser* traverser;

    // Activate all groups that were marked for activation.
    for(i = 0; i < scenarioActivationList->size; i++){
        traverser = scenarioActivationList->storage[i];
        
        groupProcessDefExContinue(traverser, activateChosenScenarios);
    }
}

#include "generator/entgen.h"


static int foundOverride = 0;
static int overrideSALocator(GroupDefTraverser* traverser){
    if(groupDefIsSAOverride(traverser->def)){
        // Found a Spawn Area override for this mission type?
        // Stop exploring the tree.
        foundOverride = 1;
        return 0;
    }
    else{
        return 1;
    }
}

void initMapActivateSpawnAreas(){
    int i;
    GroupDefTraverser* traverser;
    GroupDefTraverser* clone;

    // Activate all groups that were marked for activation.
    for(i = 0; i < spawnAreaActivationList->size; i++){
        traverser = spawnAreaActivationList->storage[i];
        clone = createGroupDefTraverser();
        copyGroupDefTraverser(traverser, clone);

        // Look for a SpawnArea override that might be hidding further
        // down the tree.
        foundOverride = 0;
        //groupProcessDefEx2(traverser, overrideSALocator);
        if(foundOverride){
            // If an override was fuond, activate generators
            // that are in the override subtree.
            groupProcessDefExContinue(traverser, entgenLoader);
        }
        else{
            // If an override was not found, activate the
            // entire SpawnArea subtree.
            groupProcessDefExContinue(traverser, entgenLoader);
        }
        
        // Finished with the new spawn area.
        // entgenEndNewSpawnArea();

        // Clean up the clone made ealier.
        destroyGroupDefTraverser(clone);
    }
}

void initMap(int forceReload)
{    
    void entKillAll();

    loadstart_printf("Doing initMap.. ");
    //verbose_printf("Starting initMap.\n" );

    if (forceReload)
    {
        MapSpecReload();
        MissionSpecReload();
    }

    if_log_mapinit(
        log_printf("MapInitDetails",     "Initializing map.\n");
    );

    setMapInfoFlags();

    if(server_state.noAI){
        if_log_mapinit(
            log_printf("MapInitDetails",     "Aborting map initialization due to \"no AI\" mode.\n");
        );

        printf("Bailing on initMap due to server_state.noAI being TRUE.\n" );
        loadend_printf("done");
        return;
    }

    if(mapInitialized){
        if_log_mapinit(
            log_printf("MapInitDetails",     "Map has already been initialized before...\n");
        );
        // If the map is already initialized and we're not forcing a reload...
        if(!forceReload){
            // Don't initialize the map again.
            if_log_mapinit(
                log_printf("MapInitDetails",     "Aborting map initialization...\n");
            );

            verbose_printf("Bailing on due to !forceReload.\n" );
            loadend_printf("done");
            return;
        }

        if_log_mapinit(
            log_printf("MapInitDetails",     "Destroying generators and entities\n");
        );

        // Clean up all running scripts
        ScriptEndAllRunningScripts(false);

        // Do some cleanup.
        // generators generates entities... so...
        //    This must come before generator destruction because destroying entities will
        //    involve updating their parent generators.
        entKillAll();

        // While initializing the map, generators are loaded... so...
        entgenDestroyAll();
    }
    else
        mapInitialized = 1;

    /*
    missionMapQueryResult = onMissionMap();

    // Conditionally make mission map servers goto sleep if requested by the registry.
    if(missionMapQueryResult){
        if_log_mapinit(
            log_printf("MapInitDetails",     "Initializing mission map \"%s\".\n", world_name);
        );

        if(duGetIntDebugSetting("PauseMissionServers")){
            if_log_mapinit(
                log_printf("MapInitDetails",     "Going into sleeping loop...");
            );
            while(1){
                Sleep(100);
            }
        }
    }else{
        if_log_mapinit(
            log_printf("MapInitDetails",     "Initializing static map \"%s\".\n", world_name);
        );
    }
    */

    initMapSetup();

    if(namedSpatialMarkers){
        stashTableClear(namedSpatialMarkers);
    }else{
        namedSpatialMarkers = stashTableCreateWithStringKeys(128,  StashDeepCopyKeys | StashCaseSensitive );
    }

    initGlobalSpawnArea();
    {
        // Produce a list of all available scenarios in this map.
        initMapExtractActivationCandidates();

        // Collect all scenarios of the same type and store them into a hashtable by type name.
        initMapSortActivationCandidatesByType();

        // All scenarios available in the map is now sorted and can be accessed by name.
        // Grab some scenarios, according to the mission type, and put them into a activation list.
        initMapChooseActivationCandidates();

        // Activate all scenarios that have been chosen for activation.
        // Iterate through the list of scenarios in the activation list and activate them all.
        initMapActivateChosenScenarios();

        // Some scenario activation will require more spawn areas to be activated as well.
        // Iterate through the list of spawn areas to be activated and activate them all.
        initMapActivateSpawnAreas();

    }

    initMapCleanup();

    if_log_mapinit(
        log_printf("MapInitDetails",     "Map initialization complete.\n");
    );

    //verbose_printf("Done initMap.\n" );
    printGeneratorTypeStats();
    loadend_printf("done");

    if (forceReload)
    {
        dbSetupMap();
        VisitLocationLoad();
    }
    if (!server_state.levelEditor)
    {    
        MissionLoad(0, db_state.map_id, db_state.map_name);
        StoryLoad();
        EncounterLoad();
        ScriptMarkerLoad();
        ScriptLocationLoad();
    }
    DoorAnimLoad();
    aiNPCResetBeaconRadius();

    // starting any zone scripts for this map
    ZoneScriptsStart();

    zowie_SetupEntities();

    loadstart_printf("Retrieving AutoCommands.. ");
    dbSyncContainerRequest(CONTAINER_AUTOCOMMANDS, 0, CONTAINER_CMD_LOAD_ALL, 0);
    loadend_printf("done");
}
