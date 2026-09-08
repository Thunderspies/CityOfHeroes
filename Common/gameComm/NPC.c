#include "gameComm/Npc.h"        // For NPC structure defintion

#include <utilitieslib/components/earray.h>
#include <utilitieslib/utils/error.h>

#include <utilitieslib/utils/textparser.h>
#include "entity/costume.h"

// For character stat assignment
#include "entity/entity.h"
#include "entity/character_base.h"    // For character structure definition
#include "entity/classes.h"    // For character class initialization
#include "entity/origins.h"    // For character origin initialization
#include "entity/powers.h"        // For character power assignment
#include <utilitieslib/assert/assert.h>
#include <utilitieslib/components/StashTable.h>
#include <utilitieslib/utils/file.h>
#include <utilitieslib/utils/fileutil.h>
#include <utilitieslib/utils/FolderCache.h>
#include <utilitieslib/components/SharedMemory.h>
#include <string.h>

#if CLIENT
#include "clientError.h"
#include "entity/entclient.h"
#include "storyarc/playerCreatedStoryarcClient.h"
#endif

SERVER_SHARED_MEMORY NPCDefList npcDefList;

//---------------------------------------------------------------------------------------------------------------
// NPC definition parsing
//---------------------------------------------------------------------------------------------------------------
TokenizerParseInfo ParsePowerNameRef[] =
{
    {    "PowerCategory",    TOK_STRUCTPARAM | TOK_STRING(PowerNameRef, powerCategory, 0) },
    {    "PowerSet",            TOK_STRUCTPARAM | TOK_STRING(PowerNameRef, powerSet, 0) },
    {    "Power",            TOK_STRUCTPARAM | TOK_STRING(PowerNameRef, power, 0) },
    {    "Level",            TOK_STRUCTPARAM | TOK_INT(PowerNameRef, level, 0) },
    {    "Remove",            TOK_STRUCTPARAM | TOK_INT(PowerNameRef, remove, 0) },
    {    "DontSetStance",    TOK_STRUCTPARAM | TOK_INT(PowerNameRef, dontSetStance, 0)    },
    {    "\n",                TOK_END,                                0},
    {    "", 0, 0 }
};

TokenizerParseInfo ParseNPCDef[] =
{
    {    "Name",            TOK_STRUCTPARAM | TOK_STRING(NPCDef, name, 0)},
    {    "DisplayName",    TOK_STRING(NPCDef, displayName, 0)},

    {    "Class",        TOK_IGNORE    }, //TODO remove from data
    {    "Level",        TOK_IGNORE    },
    {    "Rank",            TOK_IGNORE    },
    {    "XP",            TOK_IGNORE    },

    {    "Costume",        TOK_STRUCT(NPCDef, costumes, ParseCostume) },
    {    "{",            TOK_START,        0 },
    {    "}",            TOK_END,            0 },
    {    "FileName",        TOK_CURRENTFILE(NPCDef, fileName)},
    {    "", 0, 0 }
};

TokenizerParseInfo ParseNPCDefBegin[] =
{
    {    "NPC",        TOK_STRUCT(NPCDefList, npcDefs, ParseNPCDef) },
    {    "", 0, 0 }
};

DefineIntList ParseEnumList[] =
{
    {"MINION",            VR_MINION},
    {"LIEUTENAENT",        VR_LIEUTENANT},
    {"BOSS",            VR_BOSS},
    {    "", 0 },
};



static int __cdecl compareNPCDefNames(const void* d1Data, const void* d2Data)
{
    const NPCDef** d1 = (const NPCDef**)d1Data;
    const NPCDef** d2 = (const NPCDef**)d2Data;
    return stricmp((*d1)->name, (*d2)->name);
}

bool npcDefsReadFilesPreProcess(ParseTable* tpi, void* structptr)
{
    NPCDefList * nlist = (NPCDefList *)structptr;
    eaQSortConst(nlist->npcDefs, compareNPCDefNames);
    
    return 1;
}

bool npcDefsPostProcess(ParseTable tpi[], NPCDefList * nlist)
{
    int NPCCursor;

    // For each NPC definition...
    for(NPCCursor = eaSize(&npcDefList.npcDefs)-1; NPCCursor >= 0; NPCCursor--)
    {
        const NPCDef* npc = eaGetConst(&npcDefList.npcDefs, NPCCursor);
        int CostumeCursor;

        assert( eaSize(&npc->costumes) == 1 );

        // For each costume the NPC can have...
        for(CostumeCursor = eaSize(&npc->costumes)-1; CostumeCursor >= 0; CostumeCursor--)
        {
            Costume* costume = cpp_reinterpret_cast(Costume*)eaGetConst(&npc->costumes, CostumeCursor);
            int CostumePartCursor;

            costume->appearance.bodytype = GetBodyTypeForEntType(costume->appearance.entTypeFile);

            // For each CostumePart in the Costume...
            for(CostumePartCursor = eaSize(&costume->parts)-1; CostumePartCursor >= 0; CostumePartCursor--)
            {
                // Make sure the costume part color is sane...
                CostumePart* part = eaGet(&costume->parts, CostumePartCursor);
                part->color[0].a = 255;
                part->color[1].a = 255;
            }

            costume->appearance.iNumParts = eaSize(&costume->parts);
        }
    }

    return true;
}

bool npcDefsReadFilesPostProcess(ParseTable pti[], NPCDefList * npclist)
{
    NPCDef* defaultNPC;

    // JW: The 0th slot of this list should not be a "valid" NPC
    //  For safety's sake, just copy the current 0th; switch to a real null or "default" eventually
    defaultNPC = ParserAllocStruct(sizeof(*defaultNPC));
    ParserCopyStruct(ParseNPCDef, npcDefList.npcDefs[0], sizeof(*defaultNPC), defaultNPC);
    StructFreeStringConst(defaultNPC->fileName);
    defaultNPC->fileName = ParserAllocString("debug"); // So reloading doesn't think it's part of the file being reloaded
    eaInsertConst(&npclist->npcDefs, defaultNPC, 0);

    npcDefsPostProcess(pti, npclist);

    return true;
}

#if CLIENT
#include "gameData/BodyPart.h"
#include "entity/costume_client.h"
#include "cmdparse/cmdgame.h"
int npcReadDefFilesVerifyTextures(void)
{
    int NPCCursor;

    if (isProductionMode() || game_state.create_bins || quickload)
        return 1;

    // For each NPC definition...
    for(NPCCursor = eaSize(&npcDefList.npcDefs)-1; NPCCursor >= 0; NPCCursor--)
    {
        const NPCDef* npc = eaGetConst(&npcDefList.npcDefs, NPCCursor);
        int CostumeCursor;

        // For each costume the NPC can have...
        for(CostumeCursor = eaSize(&npc->costumes)-1; CostumeCursor >= 0; CostumeCursor--)
        {
            const cCostume* costume = eaGetConst(&npc->costumes, CostumeCursor);
            int CostumePartCursor;

            // For each CostumePart in the Costume...
            for(CostumePartCursor = eaSize(&costume->parts)-1; CostumePartCursor >= 0; CostumePartCursor--)
            {
                // Make sure the costume part textures are sane...
                const CostumePart* part = eaGetConst(&costume->parts, CostumePartCursor);
                const BodyPart* bodyPart;
                char tmp[ MAX_NAME_LEN ], tmp1[ MAX_NAME_LEN ];
                bool found1, found2;

                if (CostumePartCursor >= eaSize(&bodyPartList.bodyParts))
                    break;

                if(part->pchName && part->pchName[0] )
                    bodyPart = bpGetBodyPartFromName(part->pchName);
                else
                    bodyPart = bodyPartList.bodyParts[CostumePartCursor];

                if (!bodyPart || !bodyPart->num_parts) // Unknown bone name - can happen in image server mode if it has too new of data
                    continue;

                determineTextureNames(bodyPart, &costume->appearance, part->pchTex1, part->pchTex2, SAFESTR(tmp), SAFESTR(tmp1), &found1, &found2);
                if (!found1) {
                    ErrorFilenamef(npc->fileName, "CUSTOM TEXTURE ERROR: texture not found: %s\nNPC: %s\nBone: %s\nEntType: %s\n", tmp, npc->name, bodyPart->name, costume->appearance.entTypeFile);
                }
                if (!found2) {
                    ErrorFilenamef(npc->fileName, "CUSTOM TEXTURE ERROR: texture not found: %s\nNPC: %s\nBone: %s\nEntType: %s\n", tmp1, npc->name, bodyPart->name, costume->appearance.entTypeFile);
                }
            }
        }
    }

    return 1;
}
#endif

bool npcDefsFinalProcess(ParseTable tpi[], NPCDefList * nlist, bool shared_memory)
{
    bool ret = true;
    int i, n;

    n = eaSize(&npcDefList.npcDefs);

    assert(!nlist->npcHashes);
    nlist->npcHashes = stashTableCreateWithStringKeys(stashOptimalSize(n), stashShared(shared_memory));

    // JW: Start from index 1 so we don't hash the "invalid" def in the 0 index
    for (i = 1; i < n; i++)
    {
        const NPCDef* def = nlist->npcDefs[i];
        assert(def);
#if CLIENT
        {
            int index = (int)stashFindPointerReturnPointerConst(npcDefList.npcHashes, def->name);
            if (index) {
                ErrorFilenamef(def->fileName, "Duplicate NPC named '%s' found in:\n  %s\nand\n  %s\n", def->name, def->fileName, npcDefList.npcDefs[index]->fileName);
                ret = false;
            }
        }
#endif
        stashAddIntConst(nlist->npcHashes, def->name, i,true);
    }

    return ret;
}

static void entResetCostumes(void)
{
    int i;
    for(i=1;i<entities_max;i++)
    {
        if (entity_state[i] & ENTITY_IN_USE ) {
            Entity *e = entities[i];
            if (e->npcIndex)
                entSetImmutableCostumeUnowned( e, npcDefsGetCostume(e->npcIndex, e->costumeIndex) ); // e->costume = npcDefsGetCostume(e->npcIndex, e->costumeIndex);
#if CLIENT
            if (e->seq) {
                costume_Apply(e);
            }
#endif
        }
    }
}

static void reloadVillainCostumesCallback(const char *relpath, int when)
{
    NPCDefList * nlist = cpp_const_cast(NPCDefList*)(&npcDefList);
    fileWaitForExclusiveAccess(relpath);
    errorLogFileIsBeingReloaded(relpath);
    if (ParserReloadFile(relpath, ParseNPCDefBegin, sizeof(npcDefList), nlist, NULL, NULL))
    {
        npcDefsPostProcess(ParseNPCDefBegin, nlist);
        stashTableDestroyConst(nlist->npcHashes);
        nlist->npcHashes = NULL;
        npcDefsFinalProcess(ParseNPCDefBegin, nlist, false);
        entResetCostumes();
#if CLIENT
        status_printf("Villain costumes reloaded.");
#endif
    }
     else
     {
        Errorf("Error reloading villain costume: %s\n", relpath);
    }
}

static bool npcDefsFinalProcessCallback(ParseTable pti[], void* structptr, bool shared_memory)
{
    return npcDefsFinalProcess(pti, (NPCDefList *)structptr, shared_memory);
}

static bool npcDefsReadFilesPostProcessCallback(ParseTable pti[], void* structptr)
{
    return npcDefsReadFilesPostProcess(pti, (NPCDefList *)structptr);
}

void npcReadDefFiles()
{
    DefineContext* enumContext = DefineCreateFromIntList(ParseEnumList);

#if SERVER
    ParserLoadFilesShared("SM_VillainCostume", "Defs", ".nd", "VillainCostume.bin", 0, ParseNPCDefBegin, &npcDefList, sizeof(npcDefList), enumContext, NULL, npcDefsReadFilesPreProcess, npcDefsReadFilesPostProcessCallback, npcDefsFinalProcessCallback);
#elif CLIENT
    ParserLoadFiles("Defs", ".nd", "VillainCostume.bin", 0, ParseNPCDefBegin, &npcDefList, enumContext, NULL, npcDefsReadFilesPreProcess);
    npcDefsReadFilesPostProcess(ParseNPCDefBegin, &npcDefList);
    npcDefsFinalProcess(ParseNPCDefBegin, &npcDefList, false);
#endif

    DefineDestroy(enumContext);

#if CLIENT
    npcReadDefFilesVerifyTextures();
#endif

    FolderCacheSetCallback(FOLDER_CACHE_CALLBACK_UPDATE, "Defs/*.nd", reloadVillainCostumesCallback);

}

const NPCDef* npcFindByIdx(int npcIndex)
{
    return eaGetConst(&npcDefList.npcDefs, npcIndex);
}

const cCostume* npcDefsGetCostume(int npcIndex, int costumeIndex)
{
    const cCostume* costume;
    const NPCDef* def;

#if CLIENT && !TEST_CLIENT
    if( gMissionMakerContact.npc_idx == npcIndex && gMissionMakerContact.pCostume)
        return costume_as_const(gMissionMakerContact.pCostume);
#endif

    // Try to get the specified NPC definition.
    def = eaGetConst(&npcDefList.npcDefs, npcIndex);
    if(!def)
    {
        // If  the specified NPC doesn't exist, use the first NPC definition.
        def = eaGetConst(&npcDefList.npcDefs, 0);

        // If there are no NPC definitions at all, give up.
        if(!def)
            return NULL;
    }

    if(def->pLocalOverride)
        return def->pLocalOverride;


    costume = eaGetConst(&def->costumes, costumeIndex);
    if(!costume)
    {
        costume = eaGetConst(&def->costumes, 0);
        if(!costume)
            return NULL;
    }

    return costume;
}

const NPCDef* npcFindByName(const char* name, int* index)
{
    int i = (int)(intptr_t)stashFindPointerReturnPointerConst(npcDefList.npcHashes, name);
    if (index) 
        *index = i;
    if(npcDefList.npcDefs && i)
        return npcDefList.npcDefs[i];
    return NULL;
}

int npcInitClassOriginByName(Entity* e, const char* characterClassName)
{
    const CharacterOrigin *porigin;
    const CharacterClass *pclass;

    pclass = classes_GetPtrFromName(&g_VillainClasses, characterClassName);
    porigin = origins_GetPtrFromName(&g_VillainOrigins, "Villain_Origin");

#ifdef TEST_CLIENT
    if (!pclass)
        pclass = g_CharacterClasses.ppClasses[0];
    if (!porigin)
        porigin = g_CharacterOrigins.ppOrigins[0];
#endif

    if(pclass && porigin)
    {
        character_Create(e->pchar, e, porigin, pclass);
        return 1;
    }

    return 0;
}
