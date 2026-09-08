#include "entgenLoader.h"
#include <utilitieslib/utils/CommandFileParser.h>
#include <utilitieslib/components/HashTableStack.h>

#include "group/group.h"
#include "group/groupProperties.h"
#include <utilitieslib/utils/SimpleParser.h>

#include <string.h>
#include <utilitieslib/assert/assert.h>
#include <utilitieslib/utils/error.h>
#include "missionMapInit.h"
#include "storyarc/storyarcinterface.h"
#include <utilitieslib/components/StashTable.h>
#include <utilitieslib/utils/fileutil.h>
#include <utilitieslib/utils/FolderCache.h>
#include "dbcomm/dbcomm.h"

#include <utilitieslib/utils/strings_opt.h>

/**********************************************************************************
 * SpawnArea memory pools
 */
MemoryPool PairsMemPool;
MemoryPool GenEntDefMemPool;
MemoryPool GeneratorInstMemPool;
MemoryPool GeneratorDefMemPool;
MemoryPool GeneratedGrpMemberDefMemPool;
MemoryPool GeneratedGroupDefMemPool;
MemoryPool SpawnAreaInstMemPool;
MemoryPool SpawnAreaDefMemPool;
/*
 * SpawnArea memory pools
 **********************************************************************************/

/**********************************************************************************************
 * Generator Definition Loading related stuff
 */

/**********************************************************************************
 * SpawnArea command words
 *    These command words defines the layout of the SpawnArea files and tells the
 *    command file parser how to parse the file.
 */
static int NPCParseHandler(ParseContext context, char* param);
static int CarParseHandler(ParseContext context, char* param);
static int CritterParseHandler(ParseContext context, char* param);
static int DoorParseHandler(ParseContext context, char* param);
static int AutoDoorParseHandler(ParseContext context, char* param);
static int MapTransferDoorParseHandler(ParseContext context, char* param);
static int DeliveryTargetParseHandler(ParseContext context, char* param);
static int GeneratorParseHandler(ParseContext context, char* param);
static int GeneratorGroupParseHandler(ParseContext context, char* param);
static int SpawnGroupParseHandler(ParseContext context, char* param);
static int GeneratedEntType_Type_Handler(ParseContext context, char* params);
static int GeneratorType_GeneratedTypes_Handler(ParseContext context, char* params);
static int GeneratorType_GeneratedGroups_Handler(ParseContext context, char* params);
static int GeneratedGrpMemberDef_GeneratedTypes_Handler(ParseContext context, char* params);
static int GeneratorType_MaxSpawnCount_Handler(ParseContext context, char* params);
static int SpawnAreaEndHandler(ParseContext context, char* params);
static int SpawnAreaHandler(ParseContext context, char* params);
static int SpawnFxHandler(ParseContext context, char* params);
static int SpawnArea_MaxCritterSpawnCount_Handler(ParseContext context, char* params);


#pragma warning(push)
#pragma warning(disable:4047) // return type differs in lvl of indirection warning

PCommandWord SpawnGroupMemberContents[] = {
    {"SpawnPercentage",        {{EXTRACT_INT, OFFSETOF(GeneratedGrpMemberDef, spawnPercentage)}}},
    {"CountMin",            {{EXTRACT_INT, OFFSETOF(GeneratedGrpMemberDef, spawnMin)}}},
    {"CountMax",            {{EXTRACT_INT, OFFSETOF(GeneratedGrpMemberDef, spawnMax)}}},
    {"GeneratedTypes",        {{EXECUTE_HANDLER, GeneratedGrpMemberDef_GeneratedTypes_Handler}}},
    {"End",                    {{STORE_STRUCTURE, OFFSETOF(GeneratedGroupDef, generatedGrpMemberDefs)}, {POP_COMMANDWORDS}}},
    {0}
};

PCommandWord SpawnGroupContents[] = {
    {"Member",                {{CREATE_STRUCTURE, &GeneratedGrpMemberDefMemPool}, {PUSH_COMMANDWORDS, SpawnGroupMemberContents}}},
    {"End",                    {{STORE_STRUCTURE}, {POP_COMMANDWORDS}}},
    {0}
};

PCommandWord GeneratorContents[] = {
    {"Name",                {{EXTRACT_STRING, OFFSETOF(GeneratorDef, name)}}},
    {"MaxSpawnCount",        {{EXECUTE_HANDLER, GeneratorType_MaxSpawnCount_Handler}}},
    {"SpawnFx",                {{EXECUTE_HANDLER, SpawnFxHandler}}},
    {"GeneratedTypes",        {{EXECUTE_HANDLER, GeneratorType_GeneratedTypes_Handler}}},
    {"GeneratedGroups",        {{EXECUTE_HANDLER, GeneratorType_GeneratedGroups_Handler}}},
    {"MaxActiveEntities",    {{EXTRACT_PERCENTAGE, OFFSETOF(GeneratorDef, maxActiveEnts)}}},
    {"ActivationInterval",    {{EXTRACT_INT, OFFSETOF(GeneratorDef, activationInterval)}}},
    {"Nofade",                {{EXTRACT_INT, OFFSETOF(GeneratorDef, nofade)}}},
    {"End",                    {{STORE_STRUCTURE}, {POP_COMMANDWORDS}}},
    {0}
};

PCommandWord EntityContents[] = {
    {"GroundOffset",        {{EXTRACT_FLOAT,        OFFSETOF(GeneratedEntDef, groundOffset)}}},
    {"MaxActiveEntities",    {{EXTRACT_PERCENTAGE,    OFFSETOF(GeneratedEntDef, maxActiveEnts)}}},
    {"PriorityList",        {{EXTRACT_STRING,        OFFSETOF(GeneratedEntDef, priorityList)}}},
    {"RunSpeed",            {{EXTRACT_FLOAT,        OFFSETOF(GeneratedEntDef, runSpeed)}}},
    {"RunSpeedVariation",    {{EXTRACT_FLOAT,        OFFSETOF(GeneratedEntDef, runSpeedVariation)}}},
    {"BeaconTypeNumber",    {{EXTRACT_INT,            OFFSETOF(GeneratedEntDef, beaconTypeNumber)}}},
    {"ScaredType",            {{EXTRACT_INT,            OFFSETOF(GeneratedEntDef, scaredType)}}},
    {"Type",                {{EXECUTE_HANDLER, GeneratedEntType_Type_Handler}}},
    {"End",                    {{STORE_STRUCTURE}, {POP_COMMANDWORDS}}},
    {0}
};

PCommandWord SpawnAreaContents[] = {
    {"MaxActiveEntities",    {{EXTRACT_INT, OFFSETOF(SpawnAreaDef, maxActiveEnts)}}},
    {"MaxNPCCount",            {{EXTRACT_INT, OFFSETOF(SpawnAreaDef, maxNPCEnts)}}},
    {"MaxCritterCount",        {{EXTRACT_INT, OFFSETOF(SpawnAreaDef, maxCritterEnts)}}},
    {"MaxNPCPerPlayer",        {{EXTRACT_INT, OFFSETOF(SpawnAreaDef, maxNPCPerPlayer)}}},
    {"MaxCritterPerPlayer",    {{EXTRACT_INT, OFFSETOF(SpawnAreaDef, maxCritterPerPlayer)}}},
    {"MaxTriggerCount",     {{EXTRACT_INT, OFFSETOF(SpawnAreaDef, maxActiveTriggers)}}},
    {"MaxCritterSpawnCount",{{EXECUTE_HANDLER, SpawnArea_MaxCritterSpawnCount_Handler}}},

    // MS: I wrote this macro to make this redundant parsing slightly more clear.
    //     Basically, use the same thing for every entity type, just change the handler.
    //     The handler will set the entType field of the GeneratedEntDef to the right thing.

    #define CREATE_ENT_DEF_PARAMS(handler) \
        {    {CREATE_STRUCTURE, &GenEntDefMemPool},                {PUSH_COMMANDWORDS, &EntityContents}, \
            {EXTRACT_STRING, OFFSETOF(GeneratedEntDef, name)},    {EXECUTE_HANDLER, handler}}

    {"NPC",                    CREATE_ENT_DEF_PARAMS(NPCParseHandler)},

    {"Car",                    CREATE_ENT_DEF_PARAMS(CarParseHandler)},

    {"Critter",                CREATE_ENT_DEF_PARAMS(CritterParseHandler)},

    {"Door",                CREATE_ENT_DEF_PARAMS(DoorParseHandler)},

    {"AutoDoor",            CREATE_ENT_DEF_PARAMS(AutoDoorParseHandler)},

    {"MapTransferDoor",        CREATE_ENT_DEF_PARAMS(MapTransferDoorParseHandler)},

    {"DeliveryTarget",        CREATE_ENT_DEF_PARAMS(DeliveryTargetParseHandler)},

    #undef CREATE_ENT_DEF_PARAMS

    {"Generator",            {    {CREATE_STRUCTURE, &GeneratorDefMemPool},            {PUSH_COMMANDWORDS, &GeneratorContents},
                                {EXTRACT_STRING, OFFSETOF(GeneratedEntDef, name)},    {EXECUTE_HANDLER, GeneratorParseHandler}}},

    {"GeneratorGroup",        {    {CREATE_STRUCTURE, &GeneratorDefMemPool},            {PUSH_COMMANDWORDS, &GeneratorContents},
                                {EXTRACT_STRING, OFFSETOF(GeneratedEntDef, name)},    {EXECUTE_HANDLER, GeneratorGroupParseHandler}}},

    {"SpawnGroup",            {    {CREATE_STRUCTURE, &GeneratedGroupDefMemPool},        {PUSH_COMMANDWORDS, &SpawnGroupContents},
                                {EXTRACT_STRING, OFFSETOF(GeneratedGroupDef, name)},{EXECUTE_HANDLER, &SpawnGroupParseHandler}}},

    {"End",                    {{EXECUTE_HANDLER, SpawnAreaEndHandler}, {STORE_STRUCTURE}, {POP_COMMANDWORDS}}},

    {0}
};

PCommandWord SpawnAreaStart[] = {
    {"SpawnArea", {        {CREATE_STRUCTURE, &SpawnAreaDefMemPool},        {PUSH_COMMANDWORDS, SpawnAreaContents},
                        {EXTRACT_STRING, OFFSETOF(SpawnAreaDef, name)}, {EXECUTE_HANDLER, SpawnAreaHandler}}},
    {0}
};
#pragma warning(pop)
/*
 * SpawnArea command words
 **********************************************************************************/



/**********************************************************************************
 * SpawnArea Info lookup
 *    Often, it is desirable to be able to find the correct Generator Definition or
 *    Generated Entity Definition by name, taking into consideration the scope/overriding
 *    rules.  These stacks are kept up to date to make this simple.  To find a definition,
 *    simply look in the stacks.  If the definitions have been overriden, the most recent
 *    one will be found.
 */

HashTableStack spawnAreaDefHashStack;
// This hash table stack contains all spawn area definitions found across one or more files.
// Look into this hash stack when looking for spawn area definitions by name.


HashTableStack generatorDefHashStack;
// This hash table stack contains several sets of generator definitions accumulated across
// several spawn areas.  It is expected that this hash table stack is at most 3 levels deep,
// and always always contains generator definitions from "Global Global Spawn Area",
// "Local Global Spawn Area", and local spawn area.
//
// "Global Global spawn area" is the SpawnArea named "Global" found in /SpawnArea/Globals.txt.
// This spawn area contains definitions that are valid in all maps.
//
// "Local global spawn area" is the SpawnArea named "Global" found in /SpawnArea/<mapname>.txt
// This spawn area contains definitions are valid for all other spawn areas in the map.  It can
// contain entries that overrides entries in the "Global Global spawn area".
//
// "Local spawn area" is any user defined SpawnArea in a map.  These spawn areas can override
// settings from both "Global Global" and "Local Global" spawn areas.


HashTableStack generatedEntDefHashStack;
// This hash table is similar to the generatorDefHashStack, except it contains GeneratedEntDefs.

HashTableStack generatedGroupDefHashStack;
// This hash table is similar to the generatorDefHashStack, except it contains GeneratedGroupDefs.

Array hashStackSpawnAreaDefs;
// This array keeps track of what has been placed into generatedEntDefHashStack and generatorDefHashStack.
// It keeps a list of SpawnAreaDefs that are on the hashstacks.

void entgenAddToLookupTables(SpawnAreaDef* def){
    assert(def);
    arrayPushBack(&hashStackSpawnAreaDefs, def);
    hashStackPushTable(generatorDefHashStack, def->generatorDefNameHash);
    hashStackPushTable(generatedEntDefHashStack, def->generatedEntDefNameHash);
    hashStackPushTable(generatedGroupDefHashStack, def->generatedGroupDefNameHash);
}

void entgenRemoveFromLookupTables(int retainGolobalSpawnAreas){
    SpawnAreaDef* def = arrayGetBack(&hashStackSpawnAreaDefs);
    assert(def);

    // SpawnAreas that are named "global" stays on the hash stacks.
    // This way, later "global" spawn area settings will always override
    // earlier spawn area settings.
    if(retainGolobalSpawnAreas && (stricmp(def->name, "global") == 0)){
        return;
    }

    arrayPopBack(&hashStackSpawnAreaDefs);
    hashStackPopTable(generatedEntDefHashStack);
    hashStackPopTable(generatorDefHashStack);
    hashStackPopTable(generatedGroupDefHashStack);
}
/*
 * SpawnArea Info lookup
 **********************************************************************************/

/**********************************************************************************
 * SpawnArea command handlers
 *    The parser can't handled all of the commands words completely as they require
 *    some custom behavior.  The parser is instructed to call these handlers for this
 *    purpose.
 */
static SpawnAreaDef* curSpawnAreaDef;    // The SpawnArea currently being defined by the parser.
static StashTable curSpawnAreaDefHash;    // Contains SpawnAreaDefs for the current spawn area file being parsed.

// For debugging.
// Useful when checking if all generated ent defs have a valid villain name.
Array generatedEntDefs;

/* Variable SpawnAreaFileHashStack
 *    Each hash table in this stack should contain all SpawnArea definitions found in a
 *    single SpawnArea file.  This hash stack provides a simple way of looking up
 *    a particular SpawnArea definition by name, without having to worry about override
 *    rules.
 */
//HashTableStack spawnAreaFileHashStack;

static GeneratedEntDef* addEntDef(ParseContext context, EntType type){
    // Add this "generated" definition into the spawn area currently being defined.

    GeneratedEntDef* def = parseGetCurrentStructure(context);
    def->entType = type;
    stashAddPointer(curSpawnAreaDef->generatedEntDefNameHash, def->name, def, false);
    arrayPushBack(&generatedEntDefs, def);
    return def;
}

// Called every time a "NPC" block beginning is found
static int NPCParseHandler(ParseContext context, char* param){
    addEntDef(context, ENTTYPE_NPC);
    return 0;
}

// Called every time a "Car" block beginning is found
static int CarParseHandler(ParseContext context, char* param){
    addEntDef(context, ENTTYPE_CAR);
    return 0;
}

// Called every time a "Critter" block beginning is found
static int CritterParseHandler(ParseContext context, char* param){
    addEntDef(context, ENTTYPE_CRITTER);
    return 0;
}

// Called every time a "Door" block beginning is found
static int DoorParseHandler(ParseContext context, char* param){
    addEntDef(context, ENTTYPE_DOOR);
    return 0;
}

// Called every time an "AutoDoor" block beginning is found
static int AutoDoorParseHandler(ParseContext context, char* param){
    GeneratedEntDef* def = addEntDef(context, ENTTYPE_DOOR);
    def->autoDoor = 1;
    return 0;
}

// Called every time a "MapTransferDoor" block beginning is found
static int MapTransferDoorParseHandler(ParseContext context, char* param){
    addEntDef(context, ENTTYPE_DOOR);
    return 0;
}

// Called every time a "DeliveryTarget" block beginning is found
static int DeliveryTargetParseHandler(ParseContext context, char* param){
    addEntDef(context, ENTTYPE_MISSION_ITEM); 
    return 0;
}

// Called every time a "Generator" block beginning is found
static int GeneratorParseHandler(ParseContext context, char* param){
    // Add this generator definition into the spawn area currently being defined.
    GeneratorDef* def = parseGetCurrentStructure(context);
    stashAddPointer(curSpawnAreaDef->generatorDefNameHash, def->name, def, false);
    def->pointType = GPTSingle;
    return 0;
}

static int GeneratorGroupParseHandler(ParseContext context, char* param){
    // Add this generator definition into the spawn area currently being defined.
    GeneratorDef* def = parseGetCurrentStructure(context);
    stashAddPointer(curSpawnAreaDef->generatorDefNameHash, def->name, def, false);
    def->pointType = GPTMultiple;
    return 0;
}

// Called every time a "SpawnGroup" block beginning is found
static int SpawnGroupParseHandler(ParseContext context, char* param){
    // Add this spawn group definition into the spawn area currently being defined.
    GeneratedGroupDef* def = parseGetCurrentStructure(context);
    stashAddPointer(curSpawnAreaDef->generatedGroupDefNameHash, def->name, def, false);
    return 0;
}

static int GeneratedEntType_Type_Handler(ParseContext context, char* params){
    Pair* p;
    char* classTypeName;
    int percentage;
    int result = 1;
    GeneratedEntDef* entType;

    beginParse(params);
    // Extract entity "type" name.  This field refers to an entry in NPC.dta
    // This field is required.  If it is not present, exit.
    result &= getString(&classTypeName);
    if(!result)
        goto exit;

    // Extract percentage.  Optional field.
    result &= getInt(&percentage);
    endParse();

    // Get the EntType structure being processed right now...
    entType = parseGetCurrentStructure(context);

    // Create a new pair structure to hold the string/int pair
    p = mpAlloc(PairsMemPool);
    p->value1 = strdup(classTypeName);
    // If a percentage was extracted...
    if(result)
        p->value2 = (void*)(intptr_t)percentage;
    else
        p->value2 = (void*)-1;

    // Attach the pair to the EntType structure.
    arrayPushBack(&entType->NPCTypeName, p);
    return 0;

exit:
    endParse();
    return 0;
}

static int GeneratorType_GeneratedTypes_Handler(ParseContext context, char* params){
    Pair* p;
    char* generatedDefName;
    int percentage;
    int result = 1;
    GeneratorDef* genType;
    GeneratedEntDef* entType;

    beginParse(params);
    // Extract generated entity type name
    // This field is required.  If it is not present, exit.
    result &= getString(&generatedDefName);
    if(!result)
        goto exit;

    // Extract percentage.  Optional field.
    result &= getInt(&percentage);
    endParse();

    // Try to find the entity type using the type name
    entType = hashStackFindValue(generatedEntDefHashStack, generatedDefName);

    if(!entType){
        // If the entity type cannot be found, stop further processing...
        Errorf("SpawnArea Error: Unknown entity type with name \"%s\" encountered\n", generatedDefName);
        goto exit;
    }

    // Get the generator type being defined right now...
    genType = parseGetCurrentStructure(context);

    // Has this generator def already been marked to generate single entities?
    if(genType->spawnType == GSTGroup){
        // If the entity type cannot be found, stop further processing...
        Errorf("SpawnArea Error: Bad \"GeneratedGroups\" field.  The generator \"%s\" already has a \"GeneratedTypes\".", genType->name);
        goto exit;
    }

    // Attach the GeneratedEntType* + spawn percentage pair to the generator definition.
    // Create a new pair structure to hold the GeneratedType*/int pair
    p = mpAlloc(PairsMemPool);
    p->value1 = entType;
    // If a percentage was extracted...
    if(result)
        p->value2 = (void*)(intptr_t)percentage;
    else
        p->value2 = (void*)-1;


    // Attach the pair to the EntType structure.
    arrayPushBack(&genType->generatedDefs, p);

    // Mark the generator as something that generates single entities.
    genType->spawnType = GSTSingle;

    return 0;

exit:
    endParse();
    return 0;
}

static int GeneratorType_GeneratedGroups_Handler(ParseContext context, char* params){
    Pair* p;
    char* generatedGrpDefName;
    int percentage;
    int result = 1;
    GeneratorDef* genType;
    GeneratedGroupDef* groupDef;

    beginParse(params);
    // Extract generated entity type name
    // This field is required.  If it is not present, exit.
    result &= getString(&generatedGrpDefName);
    if(!result)
        goto exit;

    // Extract percentage.  Optional field.
    result &= getInt(&percentage);
    endParse();

    // Try to find the spawn group definition using the type name
    groupDef = hashStackFindValue(generatedGroupDefHashStack, generatedGrpDefName);

    if(!groupDef){
        // If the entity type cannot be found, stop further processing...
        Errorf("SpawnArea Error: Unknown entity group def with name \"%s\" encountered\n", generatedGrpDefName);
        goto exit;
    }

    // Get the generator type being defined right now...
    genType = parseGetCurrentStructure(context);

    // Has this generator def already been marked to generate single entities?
    if(genType->spawnType == GSTSingle){
        // If the entity type cannot be found, stop further processing...
        Errorf("SpawnArea Error: Bad \"GeneratedTypes\" field.  The generator \"%s\" already has a \"GeneratedGroups\".", genType->name);
        goto exit;
    }

    // Attach the GeneratedEntType* + spawn percentage pair to the generator definition.
    // Create a new pair structure to hold the GeneratedType*/int pair
    p = mpAlloc(PairsMemPool);
    p->value1 = groupDef;
    // If a percentage was extracted...
    if(result)
        p->value2 = (void*)(intptr_t)percentage;
    else
        p->value2 = (void*)-1;


    // Attach the pair to the EntType structure.
    arrayPushBack(&genType->generatedDefs, p);

    // Mark the generator as something that generates single entities.
    genType->spawnType = GSTGroup;

    return 0;

exit:
    endParse();
    return 0;
}

static int GeneratedGrpMemberDef_GeneratedTypes_Handler(ParseContext context, char* params){
    Pair* p;
    char* generatedDefName;
    int percentage;
    int result = 1;
    GeneratedGrpMemberDef* memberType;
    GeneratedEntDef* entType;

    beginParse(params);
    // Extract generated entity type name
    // This field is required.  If it is not present, exit.
    result &= getString(&generatedDefName);
    if(!result)
        goto exit;

    // Extract percentage.  Optional field.
    result &= getInt(&percentage);
    endParse();

    // Try to find the entity type using the type name
    entType = hashStackFindValue(generatedEntDefHashStack, generatedDefName);

    if(!entType){
        // If the entity type cannot be found, stop further processing...
        Errorf("SpawnArea Error: Unknown entity member def with name \"%s\" encountered\n", generatedDefName);
        goto exit;
    }

    // Get the generator type being defined right now...
    memberType = parseGetCurrentStructure(context);

    // Attach the GeneratedEntType* + spawn percentage pair to the generator definition.
    // Create a new pair structure to hold the GeneratedType*/int pair
    p = mpAlloc(PairsMemPool);
    p->value1 = entType;
    // If a percentage was extracted...
    if(result)
        p->value2 = (void*)(intptr_t)percentage;
    else
        p->value2 = (void*)-1;


    // Attach the pair to the EntType structure.
    arrayPushBack(&memberType->generatedEntDefs, p);
    return 0;

exit:
    endParse();
    return 0;
}

static int GeneratorType_MaxSpawnCount_Handler(ParseContext context, char* params){
    GeneratorDef* genType;
    char* maxSpawnCount;
    int result = 1;

    beginParse(params);
    result &= getString(&maxSpawnCount);
    endParse();

    if(!result)
        return 0;

    // Get the generator type being defined right now...
    genType = parseGetCurrentStructure(context);

    if(stricmp(maxSpawnCount, "infinite") == 0)
        genType->maxSpawnCount = -1;
    else
        genType->maxSpawnCount = atoi(maxSpawnCount);

    return 0;
}

static int SpawnAreaEndHandler(ParseContext context, char* params){
    // Clean up the hash table stacks.

    entgenRemoveFromLookupTables(1);


    return 0;
}

static int SpawnArea_MaxCritterSpawnCount_Handler(ParseContext context, char* params){
    SpawnAreaDef* def;
    char* maxSpawnCount;
    int result = 1;

    beginParse(params);
    result &= getString(&maxSpawnCount);
    endParse();

    if(!result)
        return 0;

    // Get the generator type being defined right now...
    def = parseGetCurrentStructure(context);

    if(stricmp(maxSpawnCount, "infinite") == 0)
        def->maxCritterSpawnCount = -1;
    else
        def->maxCritterSpawnCount = atoi(maxSpawnCount);

    return 0;
}

static int SpawnAreaHandler(ParseContext context, char* params){
    curSpawnAreaDef = parseGetCurrentStructure(context);

    // The parser grabbed a piece of memory to be used as a spawn area structure,
    // but it hasn't been initialized yet.
    // Initiailzie some spawn area variables.
    curSpawnAreaDef->generatedEntDefNameHash = stashTableCreateWithStringKeys(4,  StashDeepCopyKeys);

    curSpawnAreaDef->generatorDefNameHash = stashTableCreateWithStringKeys(4,  StashDeepCopyKeys);

    curSpawnAreaDef->generatedGroupDefNameHash = stashTableCreateWithStringKeys(4,  StashDeepCopyKeys);

    stashAddPointer(curSpawnAreaDefHash, curSpawnAreaDef->name, curSpawnAreaDef, false);

    entgenAddToLookupTables(curSpawnAreaDef);
    return 0;
}

static int SpawnFxHandler(ParseContext context, char* params){
    unsigned int i;
    char* fxName;
    GeneratorDef* def = parseGetCurrentStructure(context);

    // Copy the specified fx name.
    fxName = strdup(params);

    // Convert the string to its uppercased form.  LF's string table stuff
    // will not be able to produce a handle for the string otherwise.
    for(i = 0; i < strlen(fxName); i++){
        fxName[i] = toupper(fxName[i]);
        if(fxName[i] == '\\')
            fxName[i] = '/';
    }
    def->spawnFxName = fxName;
    return 0;
}

/*
 * SpawnArea command handlers
 **********************************************************************************/

/**********************************************************************************
 * Misc functions
 */

/* Function initSpawnAreaParser()
 *    Initializes SpawnArea parser related variables.  It makes sure that all the variables
 *    have been properly allocated and cleaned.  Other parse related functions assumes that
 *    all shared variables can now be used.
 */
void initSpawnAreaParser(){
    // Initialize all memory pools
    PairsMemPool = initMemoryPoolLazy(PairsMemPool, sizeof(Pair), INIT_PAIRS);
    GenEntDefMemPool = initMemoryPoolLazy(GenEntDefMemPool, sizeof(GeneratedEntDef), INIT_GEN_ENT_DEFS);
    GeneratorDefMemPool = initMemoryPoolLazy(GeneratorDefMemPool, sizeof(GeneratorDef), INIT_GENERATOR_DEFS);
    GeneratorInstMemPool = initMemoryPoolLazy(GeneratorInstMemPool, sizeof(GeneratorDef), INIT_GENERATOR_INSTS);
    GeneratedGrpMemberDefMemPool = initMemoryPoolLazy(GeneratedGrpMemberDefMemPool, sizeof(GeneratorDef), INIT_GENERATOR_INSTS);
    GeneratedGroupDefMemPool = initMemoryPoolLazy(GeneratedGroupDefMemPool, sizeof(GeneratorDef), INIT_GENERATOR_INSTS);
    SpawnAreaDefMemPool = initMemoryPoolLazy(SpawnAreaDefMemPool, sizeof(SpawnAreaDef), INIT_SPAWN_AREA_DEFS);
    SpawnAreaInstMemPool = initMemoryPoolLazy(SpawnAreaInstMemPool, sizeof(SpawnAreaDef), INIT_SPAWN_AREA_INSTS);

    if(!generatedEntDefHashStack){
        generatedEntDefHashStack = createHashTableStack();
    }else
        stashTableClearStack(generatedEntDefHashStack);

    if(!generatorDefHashStack){
        generatorDefHashStack = createHashTableStack();
    }else
        stashTableClearStack(generatorDefHashStack);

    if(!generatedGroupDefHashStack){
        generatedGroupDefHashStack = createHashTableStack();
    }else
        stashTableClearStack(generatedGroupDefHashStack);

    if(!spawnAreaDefHashStack){
        spawnAreaDefHashStack = createHashTableStack();
    }else
        stashTableClearStack(spawnAreaDefHashStack);

    // Initialize all hashtables and hashtable stacks.
    // Prepare the generator names hash table to store new generator names.
    //    If the hash table doesn't exist yet, create one...
    /*
    if(!generatorNameHash){
        generatorNameHash = createHashTable();
        initHashTable(generatorNameHash, 100);
    }else
        //    Otherwise, clear out the old generator names...
        stashTableClear(generatorNameHash);

    if(!generatedTypeNameHash){
        generatedTypeNameHash = createHashTable();
        initHashTable(generatedTypeNameHash, 100);
    }else
        stashTableClear(generatedTypeNameHash);

    if(!globalGeneratorNameHash){
        globalGeneratorNameHash = createHashTable();
        initHashTable(globalGeneratorNameHash, 100);
    }else
        stashTableClear(globalGeneratorNameHash);

    if(!globalGeneratedTypeNameHash){
        globalGeneratedTypeNameHash = createHashTable();
        initHashTable(globalGeneratedTypeNameHash, 100);
    }else
        stashTableClear(globalGeneratedTypeNameHash);

    if(!generatedTypeNameHashStack){
        generatedTypeNameHashStack = createHashTableStack();
        initHashTableStack(generatedTypeNameHashStack, 2);
    }else
        stashTableClearStack(generatedTypeNameHashStack);

    if(!generatorNameHashStack){
        generatorNameHashStack = createHashTableStack();
        initHashTableStack(generatorNameHashStack, 2);
    }else
        stashTableClearStack(generatorNameHashStack);

    hashStackPushTable(generatorNameHashStack, generatorNameHash);
    hashStackPushTable(generatedTypeNameHashStack, generatedTypeNameHash);
    */
}

// For reloading spawn area automatically
static void spawnAreaSetupCallbacks();

// Generate an absolute path from the relative path of the map being loaded.
//    Get the name of the map only.
//    Check for both the forward and back slash in the relative path.
static void spawnAreaGetRelativeFilename(const char* relativePath, char* buf)
{
    const char* filename = strrchr(relativePath, '\\');
    if(NULL == filename)
        filename = strrchr(relativePath, '/');

    if(NULL == filename)            // the entire relative path consists of only the filename
        filename = relativePath;
    else
        filename++;

    //    Generate the relative path to the spawn area file
    sprintf(buf, "server/SpawnArea/%s", filename);
}

/* Function reloadSpawnArea
 *    Loads a spawn area definition.
 *    Reloading the definition means that all the generators are invalidated.  Generators depend on
 *    the SpawnArea structures to be properly connected in memory.  Because of this, a entgenReload
 *    must alsways follow a reloadSpawnArea.
 *
 *    Parameters:
 *        relativePath - relative path of map being loaded.
 *
 */
void spawnAreaReload(char* relativePath){
    #define buffersize 512
    char buffer[buffersize];
    FILE* file;
    char *spawn_globals = "server/SpawnArea/Globals.txt";

    // Enable dynamic reloading of spawnarea files
    spawnAreaSetupCallbacks();

    // Make sure all parser related variables are initialized.
    initSpawnAreaParser();

    // Load the global SpawnArea file
    //    Check if the global spawn area file is present
    file = fileOpen(spawn_globals, "r");
    if(NULL == file){
        printf("Could not find Global SpawnArea file...\n");
        printf("SpawnArea file expected: %s\n", spawn_globals);
    }else{
        fclose(file);

        curSpawnAreaDefHash = stashTableCreateWithStringKeys(16,  StashDeepCopyKeys | StashCaseSensitive );
        hashStackPushTable(spawnAreaDefHashStack, curSpawnAreaDefHash);

        // Process the global file using SpawnArea command words.
        parseCommandFile(spawn_globals, SpawnAreaStart);

        // The parser have filled topmost hash table in the hashstack with
        // global spawn area settings.
        // Put the global hash table set into hash handles of the appropriate name.
        /*
        swap(&globalGeneratorNameHash, &generatorNameHash);
        swap(&globalGeneratedTypeNameHash, &generatedTypeNameHash);

        // Install a new set of hash tables into the hashstack.
        hashStackPushTable(generatorNameHashStack, generatorNameHash);
        hashStackPushTable(generatedTypeNameHashStack, generatedTypeNameHash);
        */
        file = NULL;
    }

    spawnAreaGetRelativeFilename(relativePath, buffer);

    // Make sure that the generated path refers to a valid file
    file = fileOpen(buffer, "r");
    if(NULL == file){
        verbose_printf("Could not find SpawnArea file for current map...\n");
        verbose_printf("SpawnArea file expected: %s\n", buffer);

    }
    else{
        fclose(file);

        curSpawnAreaDefHash = stashTableCreateWithStringKeys(16,  StashDeepCopyKeys | StashCaseSensitive );
        hashStackPushTable(spawnAreaDefHashStack, curSpawnAreaDefHash);

        // Process the file using SpawnArea command words.
        parseCommandFile(buffer, SpawnAreaStart);

        // Push "local"/map settings into the hashstack for lookup later
        /*
        hashStackPushTable(generatorNameHashStack, generatorNameHash);
        hashStackPushTable(generatedTypeNameHashStack, generatedTypeNameHash);
        */
    }

    // Reloading SpawnArea definition invalidates all generators in the world.  Reload them.
    //entgenReload();
}

// If any spawn area changes, compare it to the current map and reload if neccessary
static void spawnAreaReloadCallback(const char* relpath, int when)
{
    void entKillAll();
    char* spawn_globals = "server/SpawnArea/Globals.txt";
    char filename[MAX_PATH];
    fileWaitForExclusiveAccess(relpath);
    errorLogFileIsBeingReloaded(relpath);

    spawnAreaGetRelativeFilename(db_state.map_name, filename);

    if (!stricmp(relpath+1, spawn_globals) || !stricmp(relpath+1, filename))
    {
        // Get rid of the local map spawn area
        if (hashStackGetSize(spawnAreaDefHashStack))
        {
            StashTable table = hashStackPopTable(spawnAreaDefHashStack);
            stashTableDestroy(table);
        }

        // And get rid of the globals
        if (hashStackGetSize(spawnAreaDefHashStack))
        {
            StashTable table = hashStackPopTable(spawnAreaDefHashStack);
            stashTableDestroy(table);
        }

        // now reload the spawn areas
        spawnAreaReload((char*)relpath);
        
        // Kill all the ents and restart the generators
        entKillAll();
        entgenDestroyAll();
        initMapSetup();
        initGlobalSpawnArea();
        initMapExtractActivationCandidates();
        initMapSortActivationCandidatesByType();
        initMapActivateChosenScenarios();
        initMapActivateSpawnAreas();
        initMapCleanup();
    }
}

static void spawnAreaSetupCallbacks()
{
    if (!isDevelopmentMode())
        return;
    FolderCacheSetCallback(FOLDER_CACHE_CALLBACK_UPDATE, "server/SpawnArea/*.txt", spawnAreaReloadCallback);
}

/*
 * Misc functions
 **********************************************************************************/

/*
 * Generator Definition Loading related stuff
 **********************************************************************************************/

static int disableOrphanGenerators = 0;
void entgenDisableOrphanGenerators(int disable){
    disableOrphanGenerators = disable;
}


/**********************************************************************************************
 * Generator Instanciation related stuff
 */
#include "entgen.h"        // For randomizedGenerators
#include "group/grouputil.h"    // For GroupDefTraverser
#include "gridcoll/gridcoll.h"    // For collgrid() in snapGenToGround()

//static Array spawnAreaStack;    // Holds all spawn areas that are currently being initialized.
Array generators = {0, 0, 0};

static void destroyGeneratorInst(void* genData){
    GeneratorInst* gen = (GeneratorInst*)genData;
    // FIXME!!!
    if(gen->spawnPoints){
        destroyArrayEx(gen->spawnPoints, destroyGeneratorInst);
    }
}

void initGlobalSpawnArea(){
    // Make sure a default spawn area exists.
    if(!globalSpawnArea){
        SpawnAreaDef* def;
        def = hashStackFindValue(spawnAreaDefHashStack, "Global");
        assert(def);

        globalSpawnArea = mpAlloc(SpawnAreaInstMemPool);
        globalSpawnArea->def = def;

        globalSpawnArea->maxCritterSpawnCount = globalSpawnArea->def->maxCritterSpawnCount;

        //arrayPushBack(&spawnAreaStack, globalSpawnArea);
    }
}

SpawnAreaInst* entgenCreateNewSpawnArea(char* spawnAreaDefName){
    SpawnAreaDef* def;
    SpawnAreaInst* saInst;
    static int nextSaInstID;

    // Find the Spawn Area definition with the given name.
    def = hashStackFindValue(spawnAreaDefHashStack, spawnAreaDefName);

    // If the name cannot be found, do nothing.
    if(!def){
        log_printf("EntGenCreator", "Unable to create a spawn area of name \"%s\"", spawnAreaDefName);
        return 0;
    }

    // The name has been found.
    // Create an instance of SpawnArea from the definition.
    // Place the instance in a location visible to other functions, so they may add generators
    // to this spawn area.
    saInst = mpAlloc(SpawnAreaInstMemPool);
    saInst->def = def;
    saInst->UID = nextSaInstID++;

    saInst->maxCritterSpawnCount = saInst->def->maxCritterSpawnCount;
    verbose_printf("New spawn area %s, %i will spawn %i critters\n", spawnAreaDefName, saInst->UID, saInst->maxCritterSpawnCount);

    //arrayPushBack(&spawnAreaStack, saInst);
    return saInst;
}

//void entgenEndNewSpawnArea(){
//    arrayPopBack(&spawnAreaStack);
//}

/* Function snapGenToGround
 *    Given a world coordinate, this function attempts to set the Entity Generator's
 *    entity spawn point on the ground.
 *
 */
static void snapGenToGround(GeneratorInst* gen, Mat4 mat){
    Vec3 pt1, pt2;
    CollInfo info;
    Vec3 pyr;

    // Create a little line segement that extends from below the generator to above the generator
    copyVec3(mat[3], pt1);
    copyVec3(mat[3], pt2);

    pt1[1] += 10;
    pt2[1] -= 10;


    if(!collGrid(NULL, pt1, pt2, &info, 0, COLL_NOTSELECTABLE|COLL_DISTFROMSTART)){
        printf("Generator at %f, %f, %f cannot find ground to snap to!\n", mat[3][0], mat[3][1], mat[3][2]);
        copyMat4(mat, gen->mat);
    }else{
        copyMat4(info.mat, gen->mat);
        gen->mat[3][1] += 4.0;
    }

    // MS: Turn around the generator yaw 180 degrees because all the generator geometries are BACK-FUCKING-WARDS!!!

    getMat3YPR(gen->mat, pyr);
    pyr[1] = addAngle(pyr[1], RAD(180));
    createMat3YPR(gen->mat, pyr);
}

int entgenLoader(GroupDefTraverser* traverser){
    // If the group does not have any properties, it cannot be a generator.
    // Ignore it.
    if(!traverser->def->properties)
        return 1;

    // Found a spawn area?
    // Create and store the spawn area so that the generators in this spawn area
    // can attach to it.
    /*
    {
        char* spawnAreaName;
        PropertyEnt* spawnAreaProp;

        // Extract the spawn area name
        //spawnAreaProp = hashStackFindValue((HashTableStack) traverser->vContext.context, "SpawnArea");
        stashFindPointer( traverser->def->properties, "SpawnArea", &spawnAreaProp );

        if(spawnAreaProp){

            //    If a spawn area field is found, this group represents a spawn area.
            //    Attempt to locate the corresponding spawn area definition.
            spawnAreaName = spawnAreaProp->value_str;

            if(!entgenStartNewSpawnArea(spawnAreaName))
                entgenEndNewSpawnArea();
        }
    }
    */

    if(loadSingleGenerator(traverser)){
        return 0;
    }

    // By default, continue down the tree.
    return 1;
}


StashTable generatorTypeStats;

int generatorTypeStatsDumper(StashElement element){
    verbose_printf("GeneratorType: %-25s Count: %4d\n", stashElementGetStringKey(element), stashElementGetInt(element));
    return 1;
}

void printGeneratorTypeStats(){
    if(generatorTypeStats)
        stashForEachElement(generatorTypeStats, generatorTypeStatsDumper);
}

void dumpSpawnAreaStack(Array* stack){
    int i;
    log_printf("MissionInit.log", "SpawnArea stack size: %i", stack->size);

    for(i = 0; i < stack->size; i++){
        log_printf("MissionInit.log", "%i:\t0x%p", i, stack->storage[i]);
    }
}

// Returns whether a generator was created successfully.
int loadSingleGenerator(GroupDefTraverser* traverser){
    PropertyEnt* generatorProp;
    char* generatorTypeName;
    GeneratorInst* generator;
    GeneratorDef* type;
    SpawnAreaInst* curSpawnAreaInst;
    Array* spawnAreaStack;
    static int nextGeneratorInstID;


    if(!traverser->def->properties)
        return 0;

    spawnAreaStack = mmicGetSpawnAreaStack(traverser);
    assert(spawnAreaStack);

    // The spawn area that is currently activated is the most recent spawn area that was initialized.
    curSpawnAreaInst = spawnAreaStack->storage[spawnAreaStack->size - 1];

    // If this genenerator is not specifically attached to a spawn area, it is an orphan generator.
    // Disable/Enable it depending on the option that has been set.
    if(!curSpawnAreaInst || disableOrphanGenerators)
        return 0;

    //generatorProp = hashStackFindValue((HashTableStack) traverser->vContext.context, "Generator");
    stashFindPointer( traverser->def->properties, "Generator", &generatorProp );
    if(!generatorProp)
        stashFindPointer( traverser->def->properties, "GeneratorGroup", &generatorProp );

    if(generatorProp){
        PropertyEnt* threatMinProp;
        PropertyEnt* threatMaxProp;

        // If the group has a generator field, it is supposed to be a generator.
        // Find the corresponding definition and create an instance of it.
        generatorTypeName = generatorProp->value_str;

        // Find the GeneratorDef structure using the specified name
        //    Update some global lookup tables so that all definitions within the
        //    the current spawn area are taken into consideration.
        if(curSpawnAreaInst)
            entgenAddToLookupTables(curSpawnAreaInst->def);
        else
            entgenAddToLookupTables(globalSpawnArea->def);

        type = hashStackFindValue(generatorDefHashStack, generatorTypeName);

        entgenRemoveFromLookupTables(0);

        // If the GeneratorDef can't be found, don't do anything...
        if(!type){
            printf("MapInit: %s named \"%s\" cannot be initialized.  Coord: %f %f %f\n",
                generatorProp->name_str, generatorProp->value_str, traverser->mat[3][0], traverser->mat[3][1], traverser->mat[3][2]);
            return 0;
        }

        // Track how many of which types of generators there are.
        {
            int currentCount;

            if(!generatorTypeStats){
                generatorTypeStats = stashTableCreateWithStringKeys(32, StashDeepCopyKeys);
            }

            stashFindInt(generatorTypeStats, type->name, &currentCount);
            stashAddInt(generatorTypeStats, type->name, currentCount + 1, true);
        }

        // If a generator type is found, create a GeneratorInst with the
        // appropriate settings.
        generator = calloc(1, sizeof(GeneratorInst));
        generator->def = type;
        generator->groupDef = traverser->def;
        copyMat4(traverser->mat, generator->mat);

        // MS: Turn around the generator yaw 180 degrees because all the generator geometries are BACK-FUCKING-WARDS!!!
        yawMat3(RAD(180), generator->mat);

        generator->active = 1;
        generator->timeToActivation = type->activationInterval;
        generator->UID = nextGeneratorInstID++;
        if(curSpawnAreaInst){
            generator->spawnArea = curSpawnAreaInst;
        }
        else{
            generator->spawnArea = globalSpawnArea;
        }

        // Generator group memebers should not be placed into the global generators
        // list.  Only the top level generator should be visible in the list.
        arrayPushBack(&generators, generator);

        // Grab threat level values if they are present.
        threatMinProp = hashStackFindValue(mmicGetPropertyStack(traverser), "ThreatMin");
        threatMaxProp = hashStackFindValue(mmicGetPropertyStack(traverser), "ThreatMax");

        if(threatMinProp && threatMaxProp){
            generator->minThreatLevel = atoi(threatMinProp->value_str);
            generator->maxThreatLevel = atoi(threatMaxProp->value_str);
        }

        generator->maxSpawnCount = generator->def->maxSpawnCount;

        return 1;
    }

    return 0;
}


static GeneratorInst* topLevelGenerator = NULL;
static int loadChildCount;
static int loadChildGenerator(GroupDefTraverser* traverser){
    GeneratorInst* generator;

    // Don't load the first traverser because it will be pointing at the
    // same node that generated the top level generator.
    if(!loadChildCount++)
        return 1;

    // Load the child generator.
    if(loadSingleGenerator(traverser)){
        generator = arrayPopBack(&generators);

        if(!topLevelGenerator->spawnPoints)
            topLevelGenerator->spawnPoints = createArray();

        // Add the newly loaded generator as a child of the
        // top level generator of the generator group being loaded.
        arrayPushBack(topLevelGenerator->spawnPoints, generator);

        // Make sure the child knows who its parent is.  This can
        // be used to notify the parent when something important has
        // happened.
        generator->parentGenerator = topLevelGenerator;
        return 1;
    }
    return 1;
}

/* Function loadGeneratorGroup()
 *    Given a group traverser that is pointing to a generator group, this
 *    function will load the entire generator group.
 */
int loadGeneratorGroup(GroupDefTraverser* traverser){
    // Load the top level generator.
    if(loadSingleGenerator(traverser)){
        // Using the generator loaded last as the top level generator, load all child generators.
        topLevelGenerator = arrayGetBack(&generators);
        loadChildCount = 0;
        groupProcessDefExContinue(traverser, loadChildGenerator);
        return 1;
    }
    return 0;
}

/*************************************************************************
 * Volatile context callbacks for group info tree traversal
 *    Volatile context during group info tree traversal is a integer only.
 *    Context creation and destruction are no-op's.  Context copying
 *    merely copies the pointer values.
 */
static void* entgenVContextCreate(){
    return NULL;
}

static void entgenVContextCopy(void** src, void** dst){
    *dst = *src;
}

static void entgenVContextDestroy(void* context){

}
/*
 * Volatile context callbacks for group info tree traversal
 *************************************************************************/


void entgenDestroyAll(){
    // destroy any existing generators
    clearArrayEx(&generators, NULL);
    clearArray(&randomizedGenerators);
    mpFreeAll(SpawnAreaInstMemPool);
    globalSpawnArea = NULL;
    if(generatorTypeStats) stashTableClear(generatorTypeStats);
}

void entgenReload(){
    GroupDefTraverser traverser = {0};

    entgenDestroyAll();
    groupProcessDefExBegin(&traverser, &group_info, entgenLoader);
}

void entgenValidateAllGeneratedTypes(){
    int i;
    int j;

    for(i = 0; i < generatedEntDefs.size; i++){
        GeneratedEntDef* def;
        def = generatedEntDefs.storage[i];

        for(j = 0; j < def->NPCTypeName.size; j++){
            Pair* pair;
            pair = def->NPCTypeName.storage[j];

            // todo - if the villain doesn't exist, print out an error message
        }
    }
}
/*
 * Generator Instanciation related stuff
 **********************************************************************************************/
