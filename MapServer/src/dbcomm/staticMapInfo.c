#include <utilitieslib/stdtypes.h>
#include "staticMapInfo.h"

#include <utilitieslib/utils/textparser.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/utils/error.h>
#include <limits.h>
#include "dbcomm.h"
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/FolderCache.h>
#include <utilitieslib/utils/file.h>
#include <utilitieslib/components/SharedMemory.h>
#include <utilitieslib/components/StashTable.h>
#include "entity/gametypes.h"
#ifdef SERVER
    #include "cmdparse/cmdserver.h"
    #include "svr/svr_base.h"
    #include "language/langServerUtil.h"
    #include "gameComm/sendToClient.h"
    #include "storyarc/storyarcutil.h"
    #include "storyarc/storyarcprivate.h"
    #include <utilitieslib/components/StringCache.h>
    #include "language/langServerUtil.h"
    #include "entity/character_eval.h"
#endif

//-------------------------------------------------------------------
// Range related functions
//-------------------------------------------------------------------
static int rangeSpecified(const Range* range)
{
    if(range->max == INT_MAX && range->min == INT_MIN)
        return 0;
    else
        return 1;
}

static int rangeMatch(const Range* range, int find)
{
    return find >= range->min && find <= range->max;
}

static int rangeIsValid(const Range* range)
{
    if(range->min <= range->max)
        return 1;
    else
        return 0;
}

static int rangeIsSubset(const Range* set, const Range* subset)
{
    if(!rangeIsValid(set) || !rangeIsValid(subset))
        return 0;

    if(set->min <= subset->min && subset->max <= set->max)
        return 1;
    else
        return 0;
}

static char* rangeGetString(const Range* range)
{
    static char buffer[64];
    static char buffer2[64];
    static char* printBuffer = buffer;

    char maxStr[10];
    char minStr[10];

    if(printBuffer == buffer)
        printBuffer = buffer2;
    else
        printBuffer = buffer;

    if(range->max == INT_MAX)
    {
        strcpy(maxStr, "inf");
    }
    else
    {
        sprintf(maxStr, "%i", range->max);
    }

    if(range->min == INT_MIN)
    {
        strcpy(minStr, "-inf");
    }
    else
    {
        sprintf(minStr, "%i", range->min);
    }

    sprintf_s(printBuffer, 64, "[%s,%s]", minStr, maxStr);
    return printBuffer;
}

// *********************************************************************************
//  Loading Map Infos, Map Specs and Monorails
// *********************************************************************************
#define MAPS_DB_FILENAME "server/db/maps.db"
#define MAPS_SPEC_FILENAME "specs/maps.spec"
#define MONORAIL_DEF_FILENAME "defs/monorails.def"

typedef struct
{
    StaticMapInfo** staticMapInfos;
} StaticMapInfoList;

StaticMapInfoList staticMapInfoList = {0};
StaticMapInfo** staticMapInfos = 0;

typedef struct MonorailList
{
    MonorailLine**        lines;
} MonorailList;

MonorailList monorailList = {0};

StaticDefineInt ParseOverridesFor[] =
{
    DEFINE_INT
    {    "Primal",        kOverridesMapFor_PrimalEarth },
    DEFINE_END
};

StaticDefineInt ParseOuroboros[] = {
    DEFINE_INT
    { "PrimalHero",            MAP_OUROBOROS_HEROES | MAP_OUROBOROS_PRIMAL },
    { "PrimalVillain",        MAP_OUROBOROS_VILLAINS | MAP_OUROBOROS_PRIMAL },
    { "Praetorian",            MAP_OUROBOROS_HEROES | MAP_OUROBOROS_VILLAINS | MAP_OUROBOROS_PRAETORIANS },
    DEFINE_END
};

TokenizerParseInfo ParseStaticMapInfo[] =
{
    {    "",                    TOK_INT(StaticMapInfo, instanceID, 0)},
    {    "ContainerID",        TOK_STRUCTPARAM | TOK_INT(StaticMapInfo, mapID,-1)},    // Default map container ID is -1
    {    "MapName",            TOK_STRING(StaticMapInfo, name, 0)},
    {    "BaseMapID",        TOK_INT(StaticMapInfo, baseMapID, 0)},
    {    "SafePlayersLow",    TOK_INT(StaticMapInfo, safePlayersLow, 0),            },
    {    "SafePlayersHigh",    TOK_INT(StaticMapInfo, safePlayersHigh, 0),            },
    {    "MaxHeroCount",        TOK_INT(StaticMapInfo, maxHeroCount, 0),            },
    {    "MaxVillainCount",    TOK_INT(StaticMapInfo, maxVillainCount, 0),            },
    {    "MonorailLine",        TOK_INT(StaticMapInfo, monorailLine, 0),            },
    {    "DontTrackGurneys",    TOK_INT(StaticMapInfo, dontTrackGurneys, 0),        },
    {    "DontAutoStart",    TOK_INT(StaticMapInfo, dontAutoStart, 0),            },
    {    "Transient",        TOK_INT(StaticMapInfo, transient, 0),                },
    {    "IntroZone",        TOK_INT(StaticMapInfo, introZone, 0),                },
    {    "OpaqueFogOfWar",    TOK_INT(StaticMapInfo, opaqueFogOfWar, 0),            },
    {    "MinCritters",        TOK_INT(StaticMapInfo, minCritters, 0)                },
    {    "MaxCritters",        TOK_INT(StaticMapInfo, maxCritters, 0)                },
    {    "MapKey",            TOK_STRINGARRAY(StaticMapInfo, mapKeys),            },
    {    "ChatRadius",        TOK_INT(StaticMapInfo, chatRadius, 100),            },
    {    "OverridesMapID",    TOK_INT(StaticMapInfo, overridesMapID, 0),            },
    {    "OverridesMapFor",    TOK_INT(StaticMapInfo, overridesMapFor, 0),        ParseOverridesFor    },
    {    "Ouroboros",        TOK_FLAGS(StaticMapInfo, ouroboros, 0), ParseOuroboros},

    {    "ContainerEnd",        TOK_END },
    {    "", 0, 0 }
};

TokenizerParseInfo ParseStaticMapInfoList[] =
{
    {    "Container",    TOK_STRUCT(StaticMapInfoList, staticMapInfos, ParseStaticMapInfo)},
    {    "", 0, 0 }
};

TokenizerParseInfo ParseMonorailStop[] =
{
    {    "",                TOK_STRUCTPARAM | TOK_STRING(MonorailStop, name, 0) },
    {    "{",            TOK_IGNORE },
    {    "Description",    TOK_STRING(MonorailStop, desc, 0) },
    {    "Map",            TOK_INT(MonorailStop, mapID, 0) },
    {    "Spawn",        TOK_STRING(MonorailStop, spawnName, 0) },
    {    "}",            TOK_END },
    {    "", 0, 0 }
};

TokenizerParseInfo ParseMonorailLine[] =
{
    {    "",                            TOK_STRUCTPARAM | TOK_STRING(MonorailLine, name, 0) },
    {    "{",                        TOK_IGNORE },
    {    "Title",                    TOK_STRING(MonorailLine, title, 0) },
    {    "Logo",                        TOK_STRING(MonorailLine, logo, 0) },
    {    "Stop",                        TOK_STRUCT(MonorailLine, stops, ParseMonorailStop) },
    {    "ZoneTransferDoorTypes",    TOK_STRINGARRAY(MonorailLine, allowedZoneTransferType) },
    {    "}",                        TOK_END },
    {    "", 0, 0 }
};

TokenizerParseInfo ParseMonorailList[] =
{
    {    "Monorail",        TOK_STRUCT(MonorailList, lines, ParseMonorailLine) },
    {    "", 0, 0 }
};

SHARED_MEMORY MapSpecList mapSpecList = {0};

StaticDefineInt ParseMissionsAllowed[] =
{
    DEFINE_INT
    {"None",        MA_NONE},
    {"All",            MA_ALL},
    {"Hero",        MA_HERO},
    {"Villain",        MA_VILLAIN},
    DEFINE_END
};

StaticDefineInt ParsePlayersAllowed[] = {
    DEFINE_INT
    { "Hero",                MAP_ALLOW_HEROES },
    { "Villain",            MAP_ALLOW_VILLAINS },
    { "Primal",                MAP_ALLOW_PRIMAL },
    { "Praetorian",            MAP_ALLOW_PRAETORIANS },
    DEFINE_END
};

StaticDefineInt ParseTeamArea[] =
{
    DEFINE_INT
    { "Hero",                MAP_TEAM_HEROES_ONLY },
    { "Villain",            MAP_TEAM_VILLAINS_ONLY },
    { "PrimalCommon",        MAP_TEAM_PRIMAL_COMMON },
    { "Praetoria",            MAP_TEAM_PRAETORIANS },
    { "Everybody",            MAP_TEAM_EVERYBODY },
    DEFINE_END
};

TokenizerParseInfo ParseMapSpec[] =
{
    {    "MapName",            TOK_STRUCTPARAM | TOK_STRING(MapSpec, mapfile, 0),                },
    {    "{",                TOK_START },
    {    "EntryMaxLevel",    TOK_INT(MapSpec, entryLevelRange.max, INT_MAX),        },
    {    "EntryMinLevel",    TOK_INT(MapSpec, entryLevelRange.min, INT_MIN),        },
    {    "MissionMaxLevel",    TOK_INT(MapSpec, missionLevelRange.max, INT_MAX),        },
    {    "MissionMinLevel",    TOK_INT(MapSpec, missionLevelRange.min, INT_MIN),        },
    {    "SpawnOverrideMaxLevel",    TOK_INT(MapSpec, spawnOverrideLevelRange.max, INT_MAX), },
    {    "SpawnOverrideMinLevel",    TOK_INT(MapSpec, spawnOverrideLevelRange.min, INT_MIN), },
    {    "MissionDoors",        TOK_STRINGARRAY(MapSpec, missionDoorTypes),        },
    {    "PersistentNPCs",    TOK_STRINGARRAY(MapSpec, persistentNPCs),        },
    {    "VisitLocations",    TOK_STRINGARRAY(MapSpec, visitLocations),        },
    {    "VillainGroups",    TOK_STRINGARRAY(MapSpec, villainGroups),        },
    {    "EncounterLayouts",    TOK_STRINGARRAY(MapSpec, encounterLayouts),        },
    {    "ZoneScripts",        TOK_STRINGARRAY(MapSpec, zoneScripts),            },
    {    "GlobalAIBehaviors",TOK_STRING(MapSpec, globalAIBehaviors, 0),        },  
    {    "MissionsAllowed",    TOK_INT(MapSpec, missionsAllowed, MA_NONE), ParseMissionsAllowed},
    {    "PlayersAllowed",    TOK_FLAGS(MapSpec, playersAllowed, 0), ParsePlayersAllowed},
    {    "TeamArea",            TOK_INT(MapSpec, teamArea, MAP_TEAM_COUNT), ParseTeamArea},
    {    "DevOnly",            TOK_INT(MapSpec, devOnly, 0),                    },
    {    "NPCBeaconRadius",    TOK_F32(MapSpec, NPCBeaconRadius, 0),            },
    {    "DisableBaseHospital",        TOK_INT(MapSpec, disableBaseHospital, 0),        },
    {    "MonorailRequires",    TOK_STRINGARRAY(MapSpec, monorailRequires),        },
    {    "LoadScreenPages",    TOK_STRINGARRAY(MapSpec, loadScreenPages),        },
    {    "}",                TOK_END },
    {    "", 0, 0 }
};

TokenizerParseInfo ParseMapSpecList[] =
{
    {    "Map",    TOK_STRUCT(MapSpecList, mapSpecs, ParseMapSpec)},
    {    "", 0, 0 }
};

static bool staticMapInfosVerify(TokenizerParseInfo pti[], void* structptr)
{
    int errorDetected = 0;
    int i;
    int iSize;

    staticMapInfos = staticMapInfoList.staticMapInfos;
    iSize = eaSize(&staticMapInfos);
    for(i = 0; i < iSize; i++)
    {
        StaticMapInfo* mapInfo = staticMapInfos[i];
        int maxInstanceID = 0;
        int foundBase = 0;
        int j;

        // Make sure there are no duplicate mapIDs or map names.

        for(j = 0; j < iSize; j++)
        {
            StaticMapInfo* otherMap = staticMapInfos[j];

            if(j == i)
                continue;
            
            // Copy info from the base map.
            
            if(mapInfo->baseMapID && mapInfo->baseMapID == otherMap->mapID)
            {
                // Copy stuff from my base map.
                
                foundBase = 1;

                mapInfo->name                    = ParserAllocString(otherMap->name);
                mapInfo->monorailLine            = otherMap->monorailLine;
                mapInfo->opaqueFogOfWar            = otherMap->opaqueFogOfWar;
                mapInfo->dontTrackGurneys        = otherMap->dontTrackGurneys;
                
                if(!mapInfo->safePlayersLow)
                {
                    mapInfo->safePlayersLow        = otherMap->safePlayersLow;
                    mapInfo->safePlayersHigh    = otherMap->safePlayersHigh;
                }
            }
            else if(!mapInfo->baseMapID && otherMap->baseMapID == mapInfo->mapID)
            {
                // Make myself my own base map if another map has me as its base.
                
                foundBase = 1;
                
                mapInfo->baseMapID = mapInfo->mapID;
            }
            
            if(mapInfo->baseMapID == otherMap->baseMapID && otherMap->instanceID > maxInstanceID)
            {
                maxInstanceID = otherMap->instanceID;
            }
            
            if(mapInfo->safePlayersLow > mapInfo->safePlayersHigh)
            {
                mapInfo->safePlayersHigh = mapInfo->safePlayersLow;
            }

            if(otherMap->mapID == mapInfo->mapID)
            {
                ErrorFilenamef(MAPS_DB_FILENAME, "Duplicate map id %i found\n", mapInfo->mapID);
                errorDetected = 1;
            }

            if(!mapInfo->introZone &&
                mapInfo->baseMapID != otherMap->baseMapID &&
                filePathCompare(otherMap->name, mapInfo->name) == 0)
            {
                ErrorFilenamef(MAPS_DB_FILENAME, "Duplicate map name %s found\n", mapInfo->name);
                errorDetected = 1;
            }

            // Go through the maps and set up the override references. If
            // more than one map claims to override the source map for the
            // same condition, that's an error.
            if(mapInfo->overridesMapID && mapInfo->overridesMapID == otherMap->mapID)
            {
                if(mapInfo->overridesMapID == mapInfo->mapID)
                {
                    ErrorFilenamef(MAPS_DB_FILENAME, "Map %i trying to override itself\n", mapInfo->mapID);
                    errorDetected = 1;
                }

                // Base maps don't necessarily have their base IDs set yet.
                // If a map doesn't list any base map it's going to become
                // a base map in the near future.
                if(otherMap->baseMapID && otherMap->mapID != otherMap->baseMapID)
                {
                    ErrorFilenamef(MAPS_DB_FILENAME, "Map %i trying to override non-base map %i\n", mapInfo->mapID, otherMap->mapID);
                }

                if(mapInfo->overridesMapFor <= kOverridesMapFor_Nobody ||
                    mapInfo->overridesMapFor >= kOverridesMapFor_Count)
                {
                    ErrorFilenamef(MAPS_DB_FILENAME, "Map %i has a bad override type (%d)\n", i, mapInfo->overridesMapFor);
                    errorDetected = 1;
                }

                if(otherMap->overriddenBy[mapInfo->overridesMapFor])
                {
                    ErrorFilenamef(MAPS_DB_FILENAME, "Map %i trying to override type %d on map %i, already overridden by %i\n", i, mapInfo->overridesMapFor, mapInfo->overridesMapID, otherMap->overriddenBy[mapInfo->overridesMapFor]);
                    errorDetected = 1;
                }
                else
                {
                    otherMap->overriddenBy[mapInfo->overridesMapFor] = mapInfo->mapID;
                }
            }
        }
        
        assertmsg(!mapInfo->baseMapID || foundBase, "Base map not found.");

        if(!mapInfo->baseMapID)
        {
            mapInfo->baseMapID = mapInfo->mapID;
        }
        
        mapInfo->instanceID = maxInstanceID + 1;

        assert(mapInfo->name && mapInfo->name[0]);

#ifdef SERVER
        // Make sure the specified map file exists.
        if (isDevelopmentMode() && !server_state.create_bins)
        {
            char* fileName = fileLocateRead_s(mapInfo->name, NULL, 0);
            if(!fileName && !strstr(mapInfo->name, "/_") && !FolderCacheMatchesIgnorePrefixAnywhere(mapInfo->name))
            {
                ErrorFilenamef(MAPS_DB_FILENAME, "Unable to locate map file %s\n", mapInfo->name);
                errorDetected = 1;
            }
            fileLocateFree(fileName);
        }
#endif

    }
    return !errorDetected;
}

// The playerAllowed flags are used to segment the game world into
// several 'teaming areas'. People can only feasibly team with
// people in compatible areas and they should only see other people
// in the search list who are compatible.

// H=hero, V=villain, E=earth, P=praetoria - this matches the #defines in
// staticMapInfo.h
//
// ....        Primal Common (legacy)
// ...H        Hero-only
// ..V.        Villain-only
// ..VH        Primal Common
// .E..        Primal Common
// .E.H        Hero-only
// .EV.        Villain-only
// .EVH        Primal Common
// P...        Praetorian-only
// P..H        Praetorian-only (no need for separation yet)
// P.V.        Praetorian-only (no need for separation yet)
// P.VH        Praetorian-only
// PE..        Everybody
// PE.H        Everybody (no need for separation yet)
// PEV.        Everybody (no need for separation yet)
// PEVH        Everybody

static MapTeamArea teamAreas[] =
{
    MAP_TEAM_PRIMAL_COMMON,
    MAP_TEAM_HEROES_ONLY,
    MAP_TEAM_VILLAINS_ONLY,
    MAP_TEAM_PRIMAL_COMMON,
    MAP_TEAM_PRIMAL_COMMON,
    MAP_TEAM_HEROES_ONLY,
    MAP_TEAM_VILLAINS_ONLY,
    MAP_TEAM_PRIMAL_COMMON,
    MAP_TEAM_PRAETORIANS,
    MAP_TEAM_COUNT,            // error!
    MAP_TEAM_COUNT,            // error!
    MAP_TEAM_PRAETORIANS,
    MAP_TEAM_EVERYBODY,
    MAP_TEAM_COUNT,            // error!
    MAP_TEAM_COUNT,            // error!
    MAP_TEAM_EVERYBODY
};

static bool MapSpecVerify(TokenizerParseInfo pti[], MapSpecList* mlist)
{
    int errorDetected = 0;
    int i, n;

    n = eaSize(&(mlist->mapSpecs));

    for (i = 0; i < n; i++)
    {
        MapSpec* spec = (MapSpec*)mlist->mapSpecs[i];

        if(!rangeIsValid(&spec->entryLevelRange))
        {
            ErrorFilenamef(MAPS_DB_FILENAME, "Entry level range %s for map %s is not valid\n",
                rangeGetString(&spec->entryLevelRange), spec->mapfile);
            errorDetected = 1;
        }
        if(!rangeIsValid(&spec->spawnOverrideLevelRange))
        {
            ErrorFilenamef(MAPS_DB_FILENAME, "Entry level range %s for map %s is not valid\n",
                rangeGetString(&spec->entryLevelRange), spec->mapfile);
            errorDetected = 1;
        }

        // ok, if mission level not specified -> means no mission maps there
        if (rangeSpecified(&spec->missionLevelRange))
        {
            if(!rangeIsValid(&spec->missionLevelRange))
            {
                ErrorFilenamef(MAPS_DB_FILENAME, "Mission level range %s for map %s is not valid\n",
                    rangeGetString(&spec->missionLevelRange), spec->mapfile);
                errorDetected = 1;
            }

            // Make sure that the mission level range is a subset of the entry mission range.
            // This ensures that the players are able to get into the map where their mission is located.
            if(!rangeIsSubset(&spec->entryLevelRange, &spec->missionLevelRange)){
                ErrorFilenamef(MAPS_DB_FILENAME, "Mission level range %s is not a subset of entry level range %s for map %s\n",
                    rangeGetString(&spec->missionLevelRange),
                    rangeGetString(&spec->entryLevelRange),
                    spec->mapfile);
                errorDetected = 1;
            }
        }

        // only the first 4 bits are currently supported as flags and there
        // are only 5 distinct logical areas
        STATIC_INFUNC_ASSERT(MAP_TEAM_COUNT == 5);
        STATIC_INFUNC_ASSERT(sizeof(teamAreas) == 16 * sizeof(MapTeamArea));

        // set the team area based on the allowed player flags
        if(spec->playersAllowed < 16)
        {
            // figure out a sensible value by default if maps.spec did not
            // explicitly specify a value
            if(spec->teamArea == MAP_TEAM_COUNT)
                spec->teamArea = teamAreas[spec->playersAllowed];

            if(spec->teamArea == MAP_TEAM_COUNT)
            {
                ErrorFilenamef(MAPS_DB_FILENAME, "playersAllowed invalid combination %d for map %s\n",
                    spec->teamArea,
                    spec->mapfile);
                errorDetected = 1;
            }
        }
        else
        {
            ErrorFilenamef(MAPS_DB_FILENAME, "playersAllowed flag is %d, can only be 0 to 15 for map %s\n",
                spec->playersAllowed,
                spec->mapfile);
            errorDetected = 1;

            spec->teamArea = MAP_TEAM_EVERYBODY;
        }
    }
    return !errorDetected;
}

#ifdef SERVER
bool MapSpecFinalProcess(ParseTable pti[], MapSpecList* mspeclist, bool shared_memory)
{
    bool ret = true;
    int i, n;

    n = eaSize(&mspeclist->mapSpecs);

    assert(!mspeclist->mapSpecHash);
    mspeclist->mapSpecHash = stashTableCreateWithStringKeys(stashOptimalSize(n), stashShared(shared_memory));

    for (i = 0; i < n; i++)
    {
        char baseMapName[MAX_PATH];
        saUtilFilePrefix(baseMapName, mspeclist->mapSpecs[i]->mapfile);
        if (stashFindPointerReturnPointerConst(mspeclist->mapSpecHash, baseMapName))
        {
            ErrorFilenamef(MAPS_SPEC_FILENAME, "Map %s specified twice in maps.spec", baseMapName);
            ret = false;
        }
        else
        {
            const char * shared_map_name = shared_memory ? allocAddSharedString(baseMapName) : allocAddString(baseMapName);
            stashAddPointerConst(mspeclist->mapSpecHash, shared_map_name, mspeclist->mapSpecs[i], false);
        }

        if (mspeclist->mapSpecs[i]->monorailRequires)
            chareval_Validate(mspeclist->mapSpecs[i]->monorailRequires, MAPS_SPEC_FILENAME);
    }

    return ret;
}
#endif

// MAK - Martin is putting info into the staticmapinfo so we can't easily
// reload it.  The spec file can be reloaded however
void staticMapInfosLoad()
{
    int flags = PARSER_SERVERONLY;

    if(staticMapInfos)
    {
        ParserUnload(ParseStaticMapInfoList, &staticMapInfoList, sizeof(staticMapInfoList));
        flags |= PARSER_FORCEREBUILD;
        staticMapInfoList.staticMapInfos = NULL;
        staticMapInfos = NULL;
    }

    // Binary persistance for 'maps.db' is disallowed so that its contents can
    // be updated without being overridden by the pigg'ed binary version.
    if(!ParserLoadFiles(NULL, MAPS_DB_FILENAME, NULL, flags, ParseStaticMapInfoList, &staticMapInfoList, NULL, NULL, staticMapInfosVerify))
    {
        printf("Unable to load maps.db!\n");
    }

    staticMapInfos = staticMapInfoList.staticMapInfos;

#ifdef SERVER
    if(!db_state.local_server)
    {
        StaticMapInfo* info = staticMapInfoFind(db_state.map_id);
        
        if(info && info->baseMapID)
        {
            db_state.base_map_id = info->baseMapID;
            db_state.instance_id = info->instanceID;
        }
        else
        {
            db_state.base_map_id = db_state.map_id;
            db_state.instance_id = 0;
        }
    }
#endif
}

#ifdef SERVER
int ValidateSpecListItem(const char *specFile, const char *mapFile)
{
    size_t nameLength = strlen(mapFile);
    if (nameLength < 4 ||
        mapFile[nameLength - 4] != '.' ||
        tolower(mapFile[nameLength - 3]) != 't' ||
        tolower(mapFile[nameLength - 2]) != 'x' ||
        tolower(mapFile[nameLength - 1]) != 't')
    {
        ErrorFilenamef(specFile, "Map file does not have proper .txt extension: %s", mapFile);
        return 0;
    }

    return 1;
}

static int VerifyMapsSpecsValid(void)
{
    const MapSpec** specs = mapSpecList.mapSpecs;
    int i, n;
    int ret = 1;

    n = eaSize(&specs);
    for (i = 0; i < n; i++)
    {
        if (!specs[i]->mapfile)
            continue;

        ret = ret && ValidateSpecListItem(MAPS_SPEC_FILENAME, specs[i]->mapfile);
    }

    return ret;
}

void MapSpecReload(void)
{
    int flags = PARSER_SERVERONLY;

    if(mapSpecList.mapSpecs)
    {
        ParserUnload(ParseMapSpecList, &mapSpecList, sizeof(MapSpecList));
        flags |= PARSER_FORCEREBUILD;
    }

    if(!ParserLoadFilesShared("SM_mapsspecs.bin", NULL, MAPS_SPEC_FILENAME, "mapsspec.bin", flags, ParseMapSpecList, &mapSpecList, sizeof(MapSpecList), NULL, NULL, NULL, MapSpecVerify, MapSpecFinalProcess))
    {
        printf("Unable to load maps.spec!\n");
    }

    VerifyMapsSpecsValid();
}
#endif

// *********************************************************************************
//  Map Infos
// *********************************************************************************

// Find the StaticMapInfo with the specified mapID.
StaticMapInfo* staticMapInfoFind(int mapID)
{
    int i;
    for(i = eaSize(&staticMapInfos)-1; i >= 0; i--)
    {
        StaticMapInfo* info = staticMapInfos[i];
        if(info->mapID == mapID)
            return info;
    }

    return NULL;
}

StaticMapInfo* staticMapGetBaseInfo(int mapID)
{
    StaticMapInfo* info = staticMapInfoFind(mapID);
    if (info && info->baseMapID && info->baseMapID != mapID)
        info = staticMapInfoFind(info->baseMapID);
    return info;
}

int staticMapInfoGetClones(int baseMapID, StaticMapInfo** infos)
{
    int count = 0;
    int i;
    int mapCount = eaSize(&staticMapInfos);
    for(i = 0; i < mapCount; i++)
    {
        StaticMapInfo* info = staticMapInfos[i];
        if(info->baseMapID == baseMapID)
        {
            infos[count++] = info;
        }
    }

    return count;
}

#ifdef SERVER
void staticMapPrint(ClientLink* client)
{
    int i, n;
    n = eaSize(&staticMapInfos);
    for (i = 0; i < n; i++)
    {
        StaticMapInfo* info = staticMapInfos[i];
        char displayname[500];
        char* shortpath;
        char fullpath[500];

        if (info->baseMapID && (info->mapID != info->baseMapID)) continue;
        if (!stricmp(info->name, "maps/City_Zones/City_00_01/City_00_01.txt")) continue;
        if (info->baseMapID == 68 || info->mapID == 68) continue;

        strcpy(fullpath, info->name);
        shortpath = strrchr(fullpath, '/');
        if (shortpath)
        {
            shortpath[0] = 0;
            shortpath++;
        }
        else
        {
            shortpath = info->name;
            fullpath[0] = 0;
        }

        strcpy(displayname, "\"");
        strcat(displayname, localizedPrintf(0,info->name));
        if (strcmp(displayname+1, info->name))
            strcat(displayname, "\" ");
        else
            displayname[0] = 0;
        conPrintf(client, "%i %s %s (%s)", info->mapID, shortpath, displayname, fullpath);
    }
}
#endif

void staticMapInfoResetPlayerCount()
{
    int i;
    for(i = eaSize(&staticMapInfos)-1; i >= 0; i--)
    {
        StaticMapInfo* info = staticMapInfos[i];
        info->playerCount = 0;
        info->heroCount = 0;
        info->villainCount = 0;
    }
}

#ifdef SERVER
int staticMapInfoMatchNames( char *name )
{
    int i;

    // first look for exact match to internal name
    for(i = eaSize(&staticMapInfos)-1; i >= 0; i--)
    {
        StaticMapInfo* info = staticMapInfos[i];
        if( stricmp( staticMapInfos[i]->name, name ) == 0 )
        {
            return staticMapInfos[i]->baseMapID;
        }
    }

    //next compare display names
    // trim trailing spaces
    i = (int)(strlen(name) -1);
    while( name[i] == ' ' )
    {
        name[i] = '\0';
        i--;
    }

    for(i = eaSize(&staticMapInfos)-1; i >= 0; i--)
    {
        StaticMapInfo* info = staticMapInfos[i];
        if( strstri(localizedPrintf(0,staticMapInfos[i]->name), name ))
        {
            return staticMapInfos[i]->baseMapID;
        }
    }

    return -1;
}


char * getTranslatedMapName( int map_id )
{
    StaticMapInfo *smi = staticMapInfoFind(map_id);
    char * mapname = dbGetMapName(map_id);

    if( smi )
    {
        if( smi->instanceID > 1 )
        {
            char full_name[1000];
            sprintf( full_name, "%s_%d", mapname, smi->instanceID );
            return localizedPrintf(0,full_name);
        }
        else
            return localizedPrintf(0,mapname);
    }
    else
    {
        if( mapname && strstri( mapname, "Arena" ) )
            return localizedPrintf(0,"OnArenaMap");
        else if( mapname && strstri( mapname, "Supergroupid=" ) )
            return localizedPrintf(0,"OnBaseMap");
        else
            return localizedPrintf(0,"OnMissionString");
    }
}


bool g_MapIsTutorial;
bool g_MapIsMissionMap;

// 
// Set various bits of information about the current map,
// so they don't need to be recalculated all the time.
//
void setMapInfoFlags(void)
{
    StaticMapInfo* info;
    int baseMapId;
    char *cname;

    g_MapIsMissionMap = OnMissionMap();

    if (g_MapIsMissionMap)
    {
		g_MapIsTutorial = OnPraetorianTutorialMission();
    } else {
        if(db_state.local_server) {
            cname = saUtilCurrentMapFile();
            baseMapId = staticMapInfoMatchNames(cname);
            info = staticMapInfoFind(baseMapId);
        } else {
            info = staticMapInfoFind(db_state.map_id);
        }
		g_MapIsTutorial = (info && info->introZone > 0);
    }
}

#endif

int staticMapInfoGetHighestBaseID()
{
    int i, max = 0;
    for(i = eaSize(&staticMapInfos)-1; i >= 0; i--)
    {
        StaticMapInfo* info = staticMapInfos[i];
        if(info->baseMapID > max)
            max = info->baseMapID;
    }

    return max;
}

int staticMapInfoFindOuroboros(int villain, int praetorian)
{
    int retval = 0;
    int count;
    StaticMapInfo *info;

    for (count = 0; !retval && count < eaSize(&staticMapInfos); count++)
    {
        info = staticMapInfos[count];

        if ((info->ouroboros & (villain ? MAP_OUROBOROS_VILLAINS : MAP_OUROBOROS_HEROES)) &&
            (info->ouroboros & (praetorian ? MAP_OUROBOROS_PRAETORIANS : MAP_OUROBOROS_PRIMAL)))
        {
            retval = info->baseMapID;
        }
    }

    return retval;
}

#if SERVER

int staticMapInfo_IsZoneAllowedToHaveMission(MapSpec* mapSpec, int alliance)
{
    int allowed;

    if (!mapSpec)
        return true; // don't redo a mission unless I'm sure

    switch (alliance)
    {
    case SA_ALLIANCE_HERO:
        allowed = (mapSpec->missionsAllowed == MA_ALL) || (mapSpec->missionsAllowed == MA_HERO);
    xcase SA_ALLIANCE_VILLAIN:
        allowed = (mapSpec->missionsAllowed == MA_ALL) || (mapSpec->missionsAllowed == MA_VILLAIN);
    xcase SA_ALLIANCE_BOTH:
    default:
        allowed = (mapSpec->missionsAllowed != MA_NONE);
        break;
    }

    return allowed;
}

#endif

// *********************************************************************************
//  Map Specs
// *********************************************************************************

#ifdef SERVER
const MapSpec* MapSpecFromMapId(int mapID)
{
    StaticMapInfo* info = staticMapInfoFind(mapID);
    if (!info) 
        return NULL;
    return MapSpecFromMapName(info->name);
}

const MapSpec* MapSpecFromMapName(const char* mapfile)
{
    char baseMapName[MAX_PATH];
    saUtilFilePrefix(baseMapName, mapfile);
    return (const MapSpec*)stashFindPointerReturnPointerConst(mapSpecList.mapSpecHash, baseMapName);
}

int MapSpecCanHaveMission(const MapSpec* spec, int level)
{
    if (!spec || !rangeSpecified(&spec->missionLevelRange))
        return 0;
    return rangeMatch(&spec->missionLevelRange, level);
}

int MapSpecCanOverrideSpawn(const MapSpec* spec, int level)
{
    if (!spec || !rangeSpecified(&spec->spawnOverrideLevelRange))
        return 0;
    return rangeMatch(&spec->spawnOverrideLevelRange, level);
}

const char** MapSpecDoorList(const char* mapfile)
{
    const MapSpec* spec = MapSpecFromMapName(mapfile);
    if (!spec)
        return 0;
    return spec->missionDoorTypes;
}

const char** MapSpecNPCList(const char* mapfile)
{
    const MapSpec* spec = MapSpecFromMapName(mapfile);
    if (!spec)
        return 0;
    return spec->persistentNPCs;
}

const char** MapSpecVisitLocations(const char* mapfile)
{
    const MapSpec* spec = MapSpecFromMapName(mapfile);
    if (!spec)
        return 0;
    return spec->visitLocations;
}

const char** MapSpecVillainGroups(const char* mapfile)
{
    const MapSpec* spec = MapSpecFromMapName(mapfile);
    if (!spec)
        return 0;
    return spec->villainGroups;
}

const char** MapSpecEncounterLayouts(const char* mapfile)
{
    const MapSpec* spec = MapSpecFromMapName(mapfile);
    if (!spec)
        return 0;
    return spec->encounterLayouts;
}
#endif

int MapSpecAreaCanReachArea( U8 src, U8 dst)
{
    int retval = 0;

    // If either person is in 'everywhere' then the other can come to them.
    // If they're in the same logical region then obviously they can meet.
    // Heroes can meet people in hero/villain shared areas.
    // Similarly, villains can meet people in such shared areas.
    if( src == MAP_TEAM_EVERYBODY || dst == MAP_TEAM_EVERYBODY )
        retval = 1;
    else if( src == dst )
        retval = 1;
    else if( ( src == MAP_TEAM_HEROES_ONLY || src == MAP_TEAM_PRIMAL_COMMON ) && ( dst == MAP_TEAM_HEROES_ONLY || dst == MAP_TEAM_PRIMAL_COMMON ) )
        retval = 1;
    else if( ( src == MAP_TEAM_VILLAINS_ONLY || src == MAP_TEAM_PRIMAL_COMMON ) && ( dst == MAP_TEAM_VILLAINS_ONLY || dst == MAP_TEAM_PRIMAL_COMMON ) )
        retval = 1;

    // Everything else is no-go. Note that Praetorians are taken care of
    // implicitly by the first two rules.
    return retval;
}

static void monorailListVerify(void)
{
    int line;
    int stop;
    int map;

    // Find all the static maps which apply to all the stops on all the lines.
    // This is so we can use them to build the destinations when a monorail
    // door is activated.
    for (line = 0; line < eaSize(&monorailList.lines); line++)
    {
        if (monorailList.lines[line])
        {
            for (stop = 0; stop < eaSize(&monorailList.lines[line]->stops); stop++)
            {
                if (monorailList.lines[line]->stops[stop])
                {
                    if (monorailList.lines[line]->stops[stop]->spawnName)
                    {
                        // This number matches MapSelection in door.c. I think
                        // it's just arbitrary.
                        if (strlen(monorailList.lines[line]->stops[stop]->spawnName) >= 32)
                        {
                            ErrorFilenamef(MONORAIL_DEF_FILENAME, "Spawn name '%s' too long at stop %s.%s\n",
                                monorailList.lines[line]->stops[stop]->spawnName,
                                monorailList.lines[line]->name,
                                monorailList.lines[line]->stops[stop]->name);
                        }
                    }

                    if (monorailList.lines[line]->stops[stop]->mapID)
                    {
                        for (map = 0; map < eaSize(&staticMapInfos); map++)
                        {
                            if (staticMapInfos[map]->baseMapID == monorailList.lines[line]->stops[stop]->mapID)
                            {
                                eaPush(&(monorailList.lines[line]->stops[stop]->mapInfos), staticMapInfos[map]);
                            }
                        }
                    }
                }
            }
        }
    }
}

void MonorailsLoad(void)
{
    // Monorails can't be loaded into shared memory because they have cached
    // pointers to StaticMapInfo structures, which are not shared.
    if (!ParserLoadFiles(NULL, MONORAIL_DEF_FILENAME,"monorails.bin", PARSER_SERVERONLY, ParseMonorailList, &monorailList, NULL, NULL, NULL))
    {
        printf("Unable to load monorails.def!\n");
        return;
    }

    monorailListVerify();
}

MonorailLine* MonorailGetLine(char* linename)
{
    MonorailLine* retval = NULL;
    int count;

    if (linename && monorailList.lines)
    {
        for (count = 0; count < eaSize(&monorailList.lines); count++)
        {
            if (monorailList.lines[count] && monorailList.lines[count]->name)
            {
                if (stricmp(linename, monorailList.lines[count]->name) == 0)
                {
                    retval = monorailList.lines[count];
                    break;
                }
            }
        }
    }

    return retval;
}
