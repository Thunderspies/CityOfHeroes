#include "utilitieslib/components/ArrayOld.h"
#include "utilitieslib/components/BitArray.h"
#include "utilitieslib/components/StashTable.h"
#include "utilitieslib/components/MemoryPool.h"
#include "utilitieslib/utils/CommandFileParser.h"
#include "utilitieslib/utils/file.h"

#include <stdio.h>
#include <stdlib.h>
#include "utilitieslib/assert/assert.h"

typedef struct nbdBitDefImp {
    const char* name;
    int bitnum;
} nbdBitDefImp;

typedef struct NamedBitArrayDefImp {
    const char*        name;
    StashTable    bitNameHash;
    Array        bitDefs;
    // Collection of nbdBitDefImp that is valid for the type of sequencer being defined.

    MemoryPool    bitDefMemoryPool;
} NamedBitArrayDefImp;

typedef struct NamedBitArrayImp {
    NamedBitArrayDefImp*    def;
    BitArray                bits;
} NamedBitArrayImp;

StashTable NamedBitArrayDefNameHash;

static void nbdNameHashLazyInit(){
    if(!NamedBitArrayDefNameHash){
        NamedBitArrayDefNameHash = stashTableCreateWithStringKeys(4,  StashDeepCopyKeys | StashCaseSensitive );
    }
}


NamedBitArrayDefImp* createNamedBitArrayDef(char* defName){
    NamedBitArrayDefImp* def;

    nbdNameHashLazyInit();

    // Make sure there isn't already a definition with the same name.
    assert(!stashFindElement(NamedBitArrayDefNameHash, defName, NULL));

    def = calloc(1, sizeof(NamedBitArrayDefImp));

    {
        StashElement element;

        stashAddPointerAndGetElement(NamedBitArrayDefNameHash, defName, def, false, &element);
        def->name = stashElementGetStringKey(element);
    }

    return def;
}


#define DEFAULT_NAMED_BIT_ARRAY_SIZE 32
static NamedBitArrayDefImp* createNamedBitArrayDefRaw(){
    NamedBitArrayDefImp* def;

    def = calloc(1, sizeof(NamedBitArrayDefImp));

    def->bitDefMemoryPool = createMemoryPool();
    initMemoryPool(def->bitDefMemoryPool, sizeof(nbdBitDefImp), DEFAULT_NAMED_BIT_ARRAY_SIZE);

    def->bitNameHash = stashTableCreateWithStringKeys(DEFAULT_NAMED_BIT_ARRAY_SIZE,  StashDeepCopyKeys | StashCaseSensitive );

    return def;
}

static int NamedBitArrayDefDestructor(StashElement element){
    NamedBitArrayDefImp* def;

    // Free each definition encountered in the hash table.
    def = stashElementGetPointer(element);

    destroyArrayPartial(&def->bitDefs);
    stashTableDestroy(def->bitNameHash);
    destroyMemoryPool(def->bitDefMemoryPool);
    
    assert(def);
    free(def);

    // Alwasy continue traversing the table.
    return 1;
}

/* Function destroyAllBitArrayDef()
 *    This function destroys all bit array definitions that have ever been loaded.
 *    
 *    WARNING!
 *        This function does *not* destroy any NamedBitArrays that are still in
 *        existence.  Make sure that all NamedBitArrays are cleaned up when this
 *        function is called.
 */
void destroyAllBitArrayDef(){
    stashForEachElement(NamedBitArrayDefNameHash, NamedBitArrayDefDestructor);    
}


NamedBitArrayDefImp* nbdGetNamedBitArrayDef(char* defName){
    nbdNameHashLazyInit();
    return stashFindPointerReturnPointer(NamedBitArrayDefNameHash, defName);
}

/***************************************************************************
 * Named Bit Array definition parsing
 */

NamedBitArrayDefImp* arrayDef;

static int BitDefHandler(ParseContext context, char* params){
    // Add a new bit definition to the bit name hash table.    
    nbdBitDefImp* bitDef;
    
    if(!params){
        printf("Missing bit name on line %i in file %s\n", parseGetLineno(context), parseGetFilename(context));
        return 0;
    }

    // Make sure a bit of the matching name does not already exist.
    {
        stashFindPointer( arrayDef->bitNameHash, params, &bitDef );
        if(bitDef){
            printf("Duplicate bit named \"%s\" found on line %i in file %s\n", params, parseGetLineno(context), parseGetFilename(context));
            return 0;
        }
    }

    // Get the newly created bit and store it into the hash table.
    bitDef = parsePopStructure(context);

    // Store the bit definition into a bits definition structure.
    {
        // Store bit definition into the hash table.
        // This causes the a copy of the key string to be created.
        // Store this newly created key string into the bit def.
        // This means the hash table owns the name string and will
        // be destroyed when the hash table is destroyed.
        StashElement element;
        stashAddPointerAndGetElement(arrayDef->bitNameHash, params, bitDef, false, &element);
        bitDef->name = stashElementGetStringKey(element);

        
    }
    // Do not add the bit to the bits definition's array.  This will be
    // done automatically by the parser when the "End" tag is encountered.

    bitDef->bitnum = arrayDef->bitDefs.size;
    arrayPushBack(&arrayDef->bitDefs, bitDef);
    
    
    return 1;
}


#pragma warning(push)
#pragma warning(disable:4047) // return type differs in lvl of indirection warning

static PCommandWord NamedBitsArrayDefContents[] = {
	{"Bit",                    {    {CREATE_STRUCTURE_FROM_PARENT,
			{.operandOffset =
				OFFSETOF(NamedBitArrayDefImp,
					bitDefMemoryPool)}},
                                {EXECUTE_HANDLER, BitDefHandler}}},
    {0}
};

#pragma warning(pop)

NamedBitArrayDefImp* nbdReadDefFromFile(char* filename){
    NamedBitArrayDefImp* def;
    char buf[MAX_PATH];

    // Does the file exist at all?
    // If not, there is no point in proceeding.
    if(!fileLocateRead(filename, buf))
        return NULL;

    // Make sure the hash table for holding bit array definitions is ready for use.
    nbdNameHashLazyInit();
    
    // Check if we have already loaded the definition before.
    //    If so, just return the definition.
    stashFindPointer( NamedBitArrayDefNameHash, filename, &def );
    if(def)
        return def;

    {
        ParseContext context;
        context = createParseContext();

        if(initParseContext(context, filename, NamedBitsArrayDefContents)){
            // The file exists and it has not been loaded before,
            // parse the file now.
            def = arrayDef = createNamedBitArrayDefRaw();
            parsePushStructure(context, arrayDef);

            parseBeginParse(context);
        }

        destroyParseContext(context);
    }
    
    {
        StashElement element;
        stashAddPointerAndGetElement(NamedBitArrayDefNameHash, filename, arrayDef, false, &element);
        if(arrayDef && element){
            arrayDef->name = stashElementGetStringKey(element);
        }
    }

    return def;
}
/*
 * Named Bit Array definition parsing
 ***************************************************************************/

/* Function nbdAddBitName()
 *    Adds the given bit name into the given bit definition.
 *
 *    Returns:
 *        0 - The bit has not been added successfully.  It has already been defined.
 *        1 - The bit has been added successfully.
 */
int nbdAddBitName(NamedBitArrayDefImp* def, char* name){
    nbdBitDefImp* bitDef;

    if ( stashFindElement(def->bitNameHash, name, NULL))
        return 0;
    
    // Get the newly created bit and store it into the hash table.
    bitDef = mpAlloc(def->bitDefMemoryPool);

    // Store the bit definition into a bits definition structure.
    {
        // Store bit definition into the hash table.
        // This causes the a copy of the key string to be created.
        // Store this newly created key string into the bit def.
        // This means the hash table owns the name string and will
        // be destroyed when the hash table is destroyed.
        StashElement element;
        stashAddPointerAndGetElement(arrayDef->bitNameHash, name, bitDef, false, &element);
        bitDef->name = stashElementGetStringKey(element);
    }
    
    bitDef->bitnum = arrayDef->bitDefs.size;
    arrayPushBack(&arrayDef->bitDefs, bitDef);
    return 1;
}

/* Function nbdFindBitName()
 *    This function tests for existence of the bit of the given name in the given definition.
 *
 *    Returns:
 *        -1 - A bit with the matching name does not exists.
 *        >=0 - The bit number of the bit being searched for.
 */
int nbdFindBitName(NamedBitArrayDefImp* def, char* name){
    nbdBitDefImp* bitDef;

    stashFindPointer( def->bitNameHash, name, &bitDef );
    if(!bitDef)
        return -1;
    else
        return bitDef->bitnum;
}

NamedBitArrayImp* createNamedBitArray(NamedBitArrayDefImp* def){
    NamedBitArrayImp* array;

    if(!def){
        return NULL;
    }

    array = calloc(1, sizeof(NamedBitArrayImp));
    array->def = def;

    array->bits = createBitArray(def->bitDefs.size);
    return array;
}

NamedBitArrayImp* createNamedBitArrayByDefName(char* filename){
    NamedBitArrayDefImp* def;

    // Check if the definition has already been loaded.
    nbdNameHashLazyInit();
    stashFindPointer( NamedBitArrayDefNameHash, filename, &def );
    if(!def){
        // If not, load the definition now.
        def = nbdReadDefFromFile(filename);

        // If the defintion cannot be loaded,
        // it is impossible to proceed.
        // Notify the calling function.
        if(!def)
            return NULL;
    }

    // Create the bit array with the requested definition.
    return createNamedBitArray(def);
}

void destroyNamedBitArray(NamedBitArrayImp* array){
    assert(array);
    destroyBitArray(array->bits);
    free(array);
}

int nbSetBitValue(NamedBitArrayImp* array, unsigned int bitnum, int value){
    assert(array);

    return baSetBitValue(array->bits, bitnum, value);
}

int nbSetBitValueByName(NamedBitArrayImp* array, char* name, int value){
    nbdBitDefImp* def;

    assert(array && name);
    stashFindPointer( array->def->bitNameHash, name, &def );
    if(!def)
        return 0;

    return baSetBitValue(array->bits, def->bitnum, value);
}

int    nbSetBit(NamedBitArrayImp* array, unsigned int bitnum){
    assert(array);
    return baSetBit(array->bits, bitnum);
}

int    nbSetBitByName(NamedBitArrayImp* array, char* name){
    nbdBitDefImp* def;

    assert(array && name);
    stashFindPointer( array->def->bitNameHash, name, &def );
    if(!def)
        return 0;

    return baSetBit(array->bits, def->bitnum);
}

int nbClearBit(NamedBitArrayImp* array, unsigned int bitnum){
    assert(array);
    return baClearBit(array->bits, bitnum);
}

int nbClearBitByName(NamedBitArrayImp* array, char* name){
    nbdBitDefImp* def;

    assert(array && name);
    stashFindPointer( array->def->bitNameHash, name, &def );
    if(!def)
        return 0;

    return baClearBit(array->bits, def->bitnum);
}

int nbClearBits(NamedBitArrayImp* target, NamedBitArrayImp* mask){
    assert(target && mask);
    return baClearBits(target->bits, mask->bits);
}

void nbSetAllBits(NamedBitArrayImp* array){
    baSetAllBits(array->bits);
}

void nbClearAllBits(NamedBitArrayImp* array){
    baClearAllBits(array->bits);
}

void nbNotAllBits(NamedBitArrayImp* array){
    baNotAllBits(array->bits);
}

// Bit testing.
int nbIsSet(NamedBitArrayImp* array, unsigned int bitnum){
    return baIsSet(array->bits, bitnum);
}

int nbIsSetByName(NamedBitArrayImp* array, char* name){
    nbdBitDefImp* def;

    assert(array && name);
    stashFindPointer( array->def->bitNameHash, name, &def );
    if(!def)
        return 0;

    return baIsSet(array->bits, def->bitnum);
}

int nbGetMaxBits(NamedBitArrayImp* array){
    return baGetMaxBits(array->bits);
}

int nbCountSetBits(NamedBitArrayImp* array){
    return baCountSetBits(array->bits);
}

int nbOrArray(NamedBitArrayImp* array1, NamedBitArrayImp* array2, NamedBitArrayImp* output){
    return baOrArray(array1->bits, array2->bits, output->bits);
}

int nbAndArray(NamedBitArrayImp* array1, NamedBitArrayImp* array2, NamedBitArrayImp* output){
    return baAndArray(array1->bits, array2->bits, output->bits);
}

int nbCopyArray(NamedBitArrayImp* input, NamedBitArrayImp* output){
    return baCopyArray(input->bits, output->bits);
}

int nbBitsAreEqual(NamedBitArrayImp* array1, NamedBitArrayImp* array2){
    return baBitsAreEqual(array1->bits, array2->bits);
}

const char* nbGetBitName(NamedBitArrayImp* array, int bit){
    if(bit >= 0 && bit < array->def->bitDefs.size){
        return ((nbdBitDefImp*)array->def->bitDefs.storage[bit])->name;
    }
    
    return NULL;
}
