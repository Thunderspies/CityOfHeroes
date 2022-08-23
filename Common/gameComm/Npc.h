#ifndef NPC_H
#define NPC_H

typedef struct cCostume cCostume;
typedef struct Entity Entity;

#include <utilitieslib/utils/textparser.h>
#include "gameComm/VillainDef.h"

#include "gameData/PowerNameRef.h"


//---------------------------------------------------------------------------
// NPCDef
//---------------------------------------------------------------------------
typedef struct NPCDef
{
    const char*        name;                // Internal name?  NPCs should be referenced by this name.
    const char*        displayName;        // Name displayed in game client.  The name will be looked up in a message store for the actual string.
    const cCostume**    costumes;        //Currently there are never multiple costumes in a single NPC def.  Costume Idx is always 0
    const cCostume *    pLocalOverride;    // If this is set, use it instead

    const char*        fileName;            // For debugging
} NPCDef;

typedef struct{
    const NPCDef** npcDefs;
    cStashTable npcHashes;
} NPCDefList;

extern SERVER_SHARED_MEMORY NPCDefList npcDefList;

//---------------------------------------------------------------------------------------------------------------
// NPC definition parsing
//---------------------------------------------------------------------------------------------------------------
void npcReadDefFiles();

const cCostume* npcDefsGetCostume(int npcIndex, int costumeIndex);

const NPCDef* npcFindByName(const char* name, int* index);
const NPCDef* npcFindByIdx(int npcIndex);

//---------------------------------------------------------------------------------------------------------------
// NPC initialization
//---------------------------------------------------------------------------------------------------------------
int npcInitClassOrigin(Entity* e, int npcIndex);
int npcInitClassOriginByName(Entity* e, const char* characterClassName);
int npcInitPowers(Entity* e, const PowerNameRef** powers, int autopower_only);
#endif