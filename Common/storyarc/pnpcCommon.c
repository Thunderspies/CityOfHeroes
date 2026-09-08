
#include <utilitieslib/stdtypes.h>
#include "storyarc/pnpcCommon.h"
#include "comm_game.h"
#include "gameComm/Npc.h"
#include <utilitieslib/components/ArrayOld.h>
#include <utilitieslib/components/SharedMemory.h>
#include <utilitieslib/components/StashTable.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/utils/textparser.h>
#include <utilitieslib/utils/error.h>
#include <utilitieslib/utils/utils.h>
#include "entity/entity.h"
#include "entity/attribmod.h"

#if SERVER
    #include "storyarc/pnpc.h"
#endif

#define DEFAULT_SHOUT_FREQUENCY        5
#define DEFAULT_SHOUT_VARIATION        10

TokenizerParseInfo ParseFakeScriptDef[] = {
    { "{",                TOK_START,        0},
    { "ScriptName",        TOK_IGNORE },
    { "}",                TOK_END,            0},
    { "var",            TOK_IGNORE },
    { "vargroup",        TOK_IGNORE },
    { "", 0, 0 }
};

TokenizerParseInfo ParsePNPCDef[] = {
    { "{",                TOK_START,                        0},
    { "",                TOK_CURRENTFILE(PNPCDef,filename) },
    { "Model",            TOK_STRING(PNPCDef,model,0) },
    { "Name",            TOK_STRUCTPARAM | TOK_STRING(PNPCDef,name,0) },
    { "DisplayName",    TOK_STRING(PNPCDef,displayname,0) },
    { "SupergroupContact", TOK_STRING(PNPCDef,deprecated,0) }, // DEPRECATED
    { "AI",                TOK_STRING(PNPCDef,ai,0) },
    { "Contact",        TOK_STRING(PNPCDef,contact,0) },
    { "RegistersSupergroup", TOK_INT(PNPCDef,canRegisterSupergroup,0) },
    { "StoreCount",     TOK_INT(PNPCDef,store.iCnt,0) },
    { "Store",             TOK_STRINGARRAY(PNPCDef,store.ppchStores) },
    { "CanTailor",         TOK_INT(PNPCDef,canTailor,0) },
    { "CanRespec",         TOK_INT(PNPCDef,canRespec,0) },
    { "CanAuction",        TOK_INT(PNPCDef,canAuction,0) },
    { "NoHeadshot",        TOK_INT(PNPCDef,noheadshot,0) },
    { "Dialog",            TOK_STRINGARRAY(PNPCDef,dialog) }, // really means ClickDialog now
    { "ShoutDialog",    TOK_STRINGARRAY(PNPCDef,shoutDialog) }, // random text that will be said all the time
    { "ShoutFrequency", TOK_INT(PNPCDef,shoutFrequency,    DEFAULT_SHOUT_FREQUENCY)        },
    { "ShoutVariation", TOK_INT(PNPCDef,shoutVariation,    DEFAULT_SHOUT_VARIATION)        },
    { "AutoRewards",    TOK_STRINGARRAY(PNPCDef,autoRewards) },
    { "TalksToStrangers",TOK_INT(PNPCDef,talksToStrangers,0) },
    { "VisionPhases",    TOK_STRINGARRAY(PNPCDef,visionPhaseRawStrings) },
    { "ExclusiveVisionPhase",    TOK_STRING(PNPCDef,exclusiveVisionPhaseRawString,0) },
    { "WrongAllianceString",    TOK_STRING(PNPCDef,wrongAllianceString,0) },
#if SERVER
    { "Script",            TOK_STRUCT(PNPCDef,scripts, ParseScriptDef) },
#else
    { "Script",            TOK_NULLSTRUCT(ParseFakeScriptDef) },
#endif
    { "}",                TOK_END,                            0},
    { "", 0, 0 }
};

TokenizerParseInfo ParsePNPCDefList[] = {
    { "NPCDef",            TOK_STRUCT(PNPCDefList,pnpcdefs,ParsePNPCDef) },
    { "", 0, 0 }
};

SHARED_MEMORY PNPCDefList g_pnpcdeflist = {0};

cStashTable g_pnpcnames = 0;        // list of PNPCDefs, indexed by pnpc name

SHARED_MEMORY VisionPhaseNames g_visionPhaseNames = {0};
SHARED_MEMORY VisionPhaseNames g_exclusiveVisionPhaseNames = {0};

static bool BuildPNPCVisionPhaseBitfields(ParseTable* pti, void* structptr)
{
    PNPCDefList* plist = (PNPCDefList*)structptr;
    int i, n = eaSize(&g_pnpcdeflist.pnpcdefs);
    const char *visionPhaseName = 0;
    int defPhaseNameIndex = 0;
    int globalPhaseNameIndex = 0;
    
    for (i = 0; i < n; i++)
    {
        PNPCDef *def = cpp_const_cast(PNPCDef*)(plist->pnpcdefs[i]);

        if (def)
        {
            def->bitfieldVisionPhases = 1; // default to be in prime phase
            def->exclusiveVisionPhase = 1; // default PNPCs to be in the "All" phase
            
            if (def->visionPhaseRawStrings && eaSize(&def->visionPhaseRawStrings))
            {
                for (defPhaseNameIndex = eaSize(&def->visionPhaseRawStrings) - 1; defPhaseNameIndex >= 0; defPhaseNameIndex--)
                {
                    globalPhaseNameIndex = 0;
                    visionPhaseName = def->visionPhaseRawStrings[defPhaseNameIndex];

                    while (globalPhaseNameIndex < eaSize(&g_visionPhaseNames.visionPhases))
                    {
                        if (!stricmp(visionPhaseName, g_visionPhaseNames.visionPhases[globalPhaseNameIndex]))
                        {
                            break;
                        }
                        globalPhaseNameIndex++;
                    }

                    if (globalPhaseNameIndex == eaSize(&g_visionPhaseNames.visionPhases))
                    {
                        ErrorFilenamef(def->filename, "Vision Phase '%s' not found in visionPhases.def!", visionPhaseName);
                    }
                    else if (globalPhaseNameIndex >= 0 && globalPhaseNameIndex < 32)
                    {
                        if (globalPhaseNameIndex == 0)
                        {
                            // NoPrime
                            def->bitfieldVisionPhases &= 0xfffffffe;
                        }
                        else
                        {
                            def->bitfieldVisionPhases |= 1 << globalPhaseNameIndex;
                        }
                    }
                }
            }

            if (def->exclusiveVisionPhaseRawString)
            {
                globalPhaseNameIndex = 0;

                while (globalPhaseNameIndex < eaSize(&g_exclusiveVisionPhaseNames.visionPhases))
                {
                    if (!stricmp(def->exclusiveVisionPhaseRawString, g_exclusiveVisionPhaseNames.visionPhases[globalPhaseNameIndex]))
                    {
                        break;
                    }
                    globalPhaseNameIndex++;
                }

                if (globalPhaseNameIndex == eaSize(&g_exclusiveVisionPhaseNames.visionPhases))
                {
                    ErrorFilenamef(def->filename, "Exclusive Vision Phase '%s' not found in visionPhasesExclusive.def!", def->exclusiveVisionPhaseRawString);
                }
                else
                {
                    def->exclusiveVisionPhase = globalPhaseNameIndex;
                }
            }
        }
    }

    return true;
}

static void BuildPNPCHashes(void)
{
    int i, n;

    if (g_pnpcnames)
        stashTableDestroyConst(g_pnpcnames);
    g_pnpcnames = stashTableCreateWithStringKeys(100, StashDeepCopyKeys);
    n = eaSize(&g_pnpcdeflist.pnpcdefs);
    for (i = 0; i < n; i++)
    {
        const PNPCDef* def = g_pnpcdeflist.pnpcdefs[i];
        const PNPCDef* other = stashFindPointerReturnPointerConst(g_pnpcnames, def->name);
        if (other)
        {
            ErrorFilenamef(def->filename, "Error: duplicated pnpc name in %s and %s", def->filename, other->filename);
        }
        else
        {
            stashAddPointerConst(g_pnpcnames, def->name, def, false);
        }
    }
}

// preload the script files for persistent npcs
void PNPCPreload(void)
{
    int flags = 0;
    ParserLoadPreProcessFunc func = 0;

    if (g_pnpcdeflist.pnpcdefs) 
    {
        flags = PARSER_FORCEREBUILD;
        ParserUnload(ParsePNPCDefList, &g_pnpcdeflist, sizeof(g_pnpcdeflist));
    }

#if SERVER
    func = PNPCValidateData;
    flags |= PARSER_SERVERONLY;
    if (!ParserLoadFilesShared("SM_npcs_server.bin", "SCRIPTS.LOC", ".npc", "npcs_server.bin", flags, ParsePNPCDefList, &g_pnpcdeflist, sizeof(g_pnpcdeflist), NULL, NULL, func, BuildPNPCVisionPhaseBitfields, NULL))
    {
        printf("Error loading persistent npc definitions\n");
    }
#elif CLIENT
    if (!ParserLoadFilesShared("SM_npcs_client.bin", "SCRIPTS.LOC", ".npc", "npcs_client.bin", flags, ParsePNPCDefList, &g_pnpcdeflist, sizeof(g_pnpcdeflist), NULL, NULL, func, BuildPNPCVisionPhaseBitfields, NULL))
    {
        printf("Error loading persistent npc definitions\n");
    }
#endif

    BuildPNPCHashes();
}

const PNPCDef* PNPCFindDef(const char* pnpcName)
{
    return stashFindPointerReturnPointerConst(g_pnpcnames, pnpcName);
}

// copied from pnpcLoader @ pnpc.c:~150
const PNPCDef *PNPCFindDefFromFilename(char *filename)
{
    int i;
    const PNPCDef *def = NULL;
    int n = eaSize(&g_pnpcdeflist.pnpcdefs);

    for (i = 0; i < n; i++)
    {
        // the strstr is to advance past "scripts.loc/" which used to be stripped out 
        // as part of the preparsing.  The client doesn't know about scripts.loc though
        if (strstriConst(g_pnpcdeflist.pnpcdefs[i]->filename, filename))
        {
            def = g_pnpcdeflist.pnpcdefs[i];
            break;
        }
    }

    return def;
}

#if CLIENT

Entity *PNPCFindEntityOnClient(const PNPCDef *pnpcDef)
{
    int i;
    Entity *returnMe = 0;

    for (i = 1; i < entities_max; i++)
    {
        if (entities[i]->persistentNPCDef == pnpcDef)
        {
            returnMe = entities[i];
            break;
        }
    }

    return returnMe;
}

#endif