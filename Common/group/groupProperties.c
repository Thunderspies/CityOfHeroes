#include "group/groupProperties.h"
#include <utilitieslib/components/StashTable.h>
#include <utilitieslib/network/netio.h>
#include <utilitieslib/components/MemoryPool.h>
#include <utilitieslib/components/EString.h>
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/assert/assert.h>

/***************************************************
 * PropertyEnt Memory management
 */

MemoryPool PropertyEntMemoryPool;
PropertyEnt* createPropertyEnt(){
    if(!PropertyEntMemoryPool){
        PropertyEntMemoryPool = createMemoryPool();
        initMemoryPool(PropertyEntMemoryPool, sizeof(PropertyEnt), 1024);
    }

    return mpAlloc(PropertyEntMemoryPool);
}

void destroyPropertyEnt(PropertyEnt* prop){
    mpFree(PropertyEntMemoryPool, prop);
}

static void copyHashPropertyEnt(StashElement source, StashElement target){
    PropertyEnt* prop;
    prop = createPropertyEnt();

    stashElementSetPointer(target, prop);
    memcpy(prop, stashElementGetPointer(source), sizeof(PropertyEnt));
}

static void copyPropertyEnt(PropertyEnt* source, PropertyEnt* target){
    memcpy(target, source, sizeof(PropertyEnt));
}

StashTable copyPropertyStashTable(StashTable source, StashTable dest)
{
    StashTableIterator iter;
    StashElement elem;
    StashTable out = dest;
    if (!out)
    {
        out = stashTableCreateWithStringKeys(16, stashTableGetMode(source));
    }

    stashGetIterator(source, &iter);

    while (stashGetNextElement(&iter, &elem))
    {
        StashElement newElem;

        // add a temp value of null to the correct slot on the new table
        stashAddPointerAndGetElement(out, stashElementGetKey(elem), NULL, false, &newElem);

        // replace the new value with the copied version
        copyHashPropertyEnt(elem, newElem);
    }

    return out;
}

PropertyEnt* clonePropertyEnt(PropertyEnt* source){
    PropertyEnt* prop;
    prop = createPropertyEnt();
    copyPropertyEnt(source, prop);
    return prop;
}
/*
 *
 ***************************************************/


/***************************************************
 * Network read/write
 */
Packet* packet;
static int property_send_count=0;
static int PropertyTablePaketWriter(StashElement element){
    PropertyEnt* prop;
    prop = stashElementGetPointer(element);

    pktSendString(packet, prop->name_str);
    pktSendString(packet, prop->value_str);
    pktSendBits(packet, 1, prop->type);
    property_send_count++;

    return 1;
}

void WriteGroupPropertiesToPacket(Packet* pak, StashTable properties){
    property_send_count = 0;
    packet = pak;
    
    // Write how many entries are to follow.
    pktSendBits(pak, 32, stashGetValidElementCount(properties));

    // Write each entry in the hash table.
    stashForEachElement(properties, PropertyTablePaketWriter);
    assert(property_send_count == stashGetValidElementCount(properties));  // Tracking crash that EJ got when copying a persistent NPC and he said it seemed to still be linked to the original one.
}




StashTable ReadGroupPropertiesFromPacket(Packet* pak){
    StashTable properties;
    int expectedTableSize;

    // How many entries are in the table?
    expectedTableSize = pktGetBits(pak, 32);

    if(0 == expectedTableSize)
        return 0;

    // Create a table large enough to hold all the entries.
    properties = stashTableCreateWithStringKeys((expectedTableSize < 16) ? 16 : expectedTableSize,  StashDeepCopyKeys);

    // Read in the data for each hash entry, then insert
    // each entry into the hash table.
    for(; expectedTableSize > 0; expectedTableSize--){
        PropertyEnt* prop = createPropertyEnt();
        strcpy(prop->name_str, pktGetString(pak));
        estrPrintCharString(&prop->value_str, pktGetString(pak));
        prop->type = pktGetBits(pak, 1);
        stashAddPointer(properties, prop->name_str, prop, false);
    }

    return properties;
}

/*
 * Network read/write
 ***************************************************/



/***************************************************
 * Disk read/write
 */
static int getProperties(void* propData, StashElement element)
{
    PropertyEnt *** prop = (PropertyEnt ***)propData;
    (*prop)[0] = stashElementGetPointer(element);
    if(stricmp((*prop)[0]->name_str, "SharedFrom") != 0) // skip SharedFrom, it's inferred at load time
        ++*prop;
    return 1;
}

static int __cdecl sortProperties(const void* p1Data, const void* p2Data)
{
    const PropertyEnt ** p1 = (const PropertyEnt **)p1Data;
    const PropertyEnt ** p2 = (const PropertyEnt **)p2Data;
    return strcmp((*p1)->name_str, (*p2)->name_str); // names will never match, since they're stash keys
}

void WriteGroupPropertiesToMemSimple(char **filedatap, StashTable properties)
{
    int i, count = stashGetValidElementCount(properties);
    PropertyEnt *props[100], **prop = props;

    // grab all the properties into an array
    assert(count < ARRAY_SIZE(props));
    stashForEachElementEx(properties, getProperties, &prop);
    assert(prop - props >= 0 && prop - props <= count);
    count = prop - props;
    prop = NULL;

    //Make sure they are always written in the same order, so we aren't always getting file changes
    qsort(props, count, sizeof(props[0]), sortProperties);

    // write them out. 
    for(i = 0; i < count; i++)
        estrConcatf(filedatap, "\tProperty\t\"%s\"\t\"%s\"\t%i\n", props[i]->name_str, props[i]->value_str, props[i]->type);
}


/*static int PropertyTableFileWriter(StashElement element){
    PropertyEnt* prop;
    prop = stashElementGetPointer(element);

    fprintf(outFile, "\t\tkey: \"%s\"\t\tvalue: \"%s\"\n", prop->name_str, prop->value_str);
    return 1;    
}
*/
/*
void WriteGroupPropertiesToFile(FILE* file, StashTable properties){
    outFile = file;
    
    fprintf(file, "\tProperties\n");
    fprintf(file, "\t\tEntryCount: %i\n", stashGetValidElementCount(properties));

    // Write each entry in the hash table.
    stashForEachElement(properties, PropertyTableFileWriter);
    fprintf(file, "\tPropertiesEnd\n");
}




StashTable ReadGroupPropertiesFromFile(FILE* file){
    StashTable properties;
    int expectedTableSize;

    // How many entries are in the table?
    fscanf(file, "\tProperties\n");
    fscanf(file, "\t\tEntryCount: %i\n", &expectedTableSize);

    // Create a table large enough to hold all the entries.
    properties = createHashTable();
    properties = stashTableCreateWithStringKeys(STASHSIZEPLEASE,  StashDeepCopyKeys);

    if(expectedTableSize < 16){
        initHashTable(properties, 16);
    }else
        initHashTable(properties, expectedTableSize);
    

    // Read in the data for each hash entry, then insert
    // each entry into the hash table.
    for(; expectedTableSize > 0; expectedTableSize--){
        PropertyEnt* prop = mpAlloc(PropertyEntMemoryPool);
        fscanf(file, "\t\tkey: \"%s\"\t\tvalue: \"%s\"\n", prop->name_str, prop->value_str);
        stashAddPointer(properties, prop->name_str, prop, false);
    }

    return properties;
}
*/
/*
 * Network read/write
 ***************************************************/

static int propertyByteLength;
static int getStringLength(char* format, ...){
    va_list va;
    char buffer[1024];

    va_start(va, format);
    vsprintf(buffer, format, va);
    va_end(va);

    return (int)strlen(buffer);
}

static int groupPropertiesAsStringByteCountHelper(StashElement element){
    PropertyEnt* prop = stashElementGetPointer(element);
    assert(prop);

    // Count the length of the given property.
    propertyByteLength += getStringLength("%s\n%s\n%d\n", prop->name_str, prop->value_str, prop->type);
    return 1;
}

static int groupPropertiesAsStringByteCount(StashTable properties){
    // Reset byte count.
    propertyByteLength = 0;

    // Output the number of properties in the table
    propertyByteLength += getStringLength("%d\n", stashGetValidElementCount(properties));

    // Account for the length of all entries added together.
    stashForEachElement(properties, groupPropertiesAsStringByteCountHelper);

    return propertyByteLength;
}


static char* groupPropertiesStringBuffer;
static char* groupPropertiesStringCursor;
static int WriteGroupPropertiesToStringHelper(StashElement element){
    PropertyEnt* prop = stashElementGetPointer(element);
    assert(prop);

    groupPropertiesStringCursor += sprintf(groupPropertiesStringCursor, "%s\n%s\n%d\n", prop->name_str, prop->value_str, prop->type);
    return 1;
}

char* WriteGroupPropertiesToString(StashTable properties){
    int bufferSize;

    bufferSize = groupPropertiesAsStringByteCount(properties);
    groupPropertiesStringCursor = groupPropertiesStringBuffer = malloc(bufferSize + 1);
    assert(groupPropertiesStringBuffer);

    groupPropertiesStringCursor += sprintf(groupPropertiesStringCursor, "%d\n", stashGetValidElementCount(properties));
    stashForEachElement(properties, WriteGroupPropertiesToStringHelper);
    return groupPropertiesStringBuffer;
}