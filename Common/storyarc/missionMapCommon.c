

#include <utilitieslib/components/earray.h>
#include <utilitieslib/utils/textparser.h>
#include "storyarc/missionMapCommon.h"
#include <utilitieslib/components/StashTable.h>
#include "storyarc/playerCreatedStoryarcValidate.h"
#include <utilitieslib/utils/mathutil.h>
#ifndef TEST_CLIENT
#include "group/groupMetaMinimap.h"
#endif

// *********************************************************************************
//  Mission map sets
// *********************************************************************************

// map sets
StaticDefineInt ParseMapSetEnum[] =
{
    DEFINE_INT
    { "Office",                            MAPSET_OFFICE },
    { "AbandonedOffice",                MAPSET_ABANDONEDOFFICE },
    { "FloodedOffice",                    MAPSET_FLOODEDOFFICE },
    { "Warehouse",                        MAPSET_WAREHOUSE },
    { "AbandonedWarehouse",                MAPSET_ABANDONEDWAREHOUSE },
    { "5thColumn",                        MAPSET_5THCOLUMN },
    { "Sewers",                            MAPSET_SEWERS },
    { "Caves",                            MAPSET_CAVES },
    { "CircleOfThorns",                    MAPSET_CIRCLEOFTHORNS },
    { "Tech",                            MAPSET_TECH },
    { "Test",                            MAPSET_TEST },
    { "SmoothCaves",                    MAPSET_SMOOTHCAVES },
    { "SS_SmoothCaves",                    MAPSET_SHADOWSHARD_SMOOTHCAVES },
    { "Council",                        MAPSET_COUNCIL },
    { "CargoShip",                        MAPSET_CARGOSHIP },
    { "VillainAbandonedOffice",            MAPSET_VILLAINABANDONEDOFFICE },
    { "VillainAbandonedWarehouse",        MAPSET_VILLAINABANDONEDWAREHOUSE },
    { "VillainCaves",                    MAPSET_VILLAINCAVES },
    { "VillainCircleOfThorns",            MAPSET_VILLAINCIRCLEOFTHORNS },
    { "VillainOffice",                    MAPSET_VILLAINOFFICE },
    { "VillainSewers",                    MAPSET_VILLAINSEWERS },
    { "VillainWarehouse",                MAPSET_VILLAINWAREHOUSE },
    { "VillainSmoothCaves",                MAPSET_VILLAINSMOOTHCAVES },
    { "Arachnos",                        MAPSET_ARACHNOS },
    { "Longbow",                        MAPSET_LONGBOW },
    { "LongbowUnderwater",                MAPSET_LONGBOWUNDERWATER },
    { "CargoShipOutdoor",                MAPSET_CARGOSHIPOUTDOOR },
    { "VillainUniqueWarehouse_Arachnos",MAPSET_VILLAINARACHNOSWAREHOUSE },
    { "VillainUniqueWarehouse_Longbow",    MAPSET_VILLAINLONGBOWWAREHOUSE },
    { "VillainArachnoidTech",            MAPSET_VILLAINARACHNOIDTECH },
    { "VillainArachnoidCave",            MAPSET_VILLAINARACHNOIDCAVE },
    { "VillainOfficeToCave",            MAPSET_VILLAINOFFICETOCAVE },
    { "VillainOfficeToSewer",            MAPSET_VILLAINOFFICETOSEWER },
    { "VillainSewerToOffice",            MAPSET_VILLAINSEWERTOSEWER },
    { "VillainMostoSmoothCave",            MAPSET_VILLAINMOSTOSMOOTHCAVE },
    { "VillainWarehousetoArach",        MAPSET_VILLAINWAREHOUSETOARACH },
    { "AbandonedTech",                    MAPSET_ABANDONEDTECH },
    { "Rikti",                            MAPSET_RIKTI },
    { "5thColumnFlashback",                MAPSET_5THCOLUMN_FLASHBACK },
    { "Caves_Mediterranean",            MAPSET_CAVES_MEDITERRANEAN },
    { "P_Office",                        MAPSET_PRAETORIAN_OFFICE },
    { "P_Office_Solo",                    MAPSET_PRAETORIAN_OFFICE_SOLO },
    { "P_Warehouse",                    MAPSET_PRAETORIAN_WAREHOUSE },
    { "P_Warehouse_Solo",                MAPSET_PRAETORIAN_WAREHOUSE_SOLO },
    { "P_Tunnels",                        MAPSET_PRAETORIAN_TUNNELS },
    { "P_Tech",                            MAPSET_PRAETORIAN_TECH },
    { "P_PPD",                            MAPSET_PRAETORIAN_PPD },
    { "P_Tech_Bio",                        MAPSET_PRAETORIAN_TECH_BIO },
    { "P_Tech_Power",                    MAPSET_PRAETORIAN_TECH_POWER },
    { "N_Warehouse",                    MAPSET_NEW_WAREHOUSE },
    { "GraniteCaves",                    MAPSET_GRANITE_CAVES },
    { "TalonsCaves",                    MAPSET_TALONS_CAVES },
    { "Random",                            MAPSET_RANDOM },
    { "Specific",                        MAPSET_SPECIFIC },

    DEFINE_END
};

TokenizerParseInfo ParseMissionMap[] = {
    { "", TOK_STRUCTPARAM | TOK_STRING(MissionMap, mapfile,0) },
    { "\n",                TOK_END,            0 },
    { "", 0, 0 }
};



TokenizerParseInfo ParseMissionMapTime[] = {
    { "", TOK_STRUCTPARAM | TOK_INT(MissionMapTime, maptime,0) },
    { "Map",            TOK_STRUCT(MissionMapTime, maps, ParseMissionMap) },
    { "{",                TOK_START,        0 },
    { "}",                TOK_END,        0 },
    { "", 0, 0 }
};

TokenizerParseInfo ParseMissionMapSet[] = {
    { "", TOK_STRUCTPARAM | TOK_INT(MissionMapSet, mapset, MAPSET_NONE),            ParseMapSetEnum },
    { "DisplayName",    TOK_STRING(MissionMapSet, pchDisplayName,0), },
    { "RandomDoorType",    TOK_STRINGARRAY(MissionMapSet, randomDoorTypes), },
    { "Time",            TOK_STRUCT(MissionMapSet, missionmaptimes, ParseMissionMapTime) },
    { "{",                TOK_START,        0 },
    { "}",                TOK_END,        0 },
    { "", 0, 0 }
};



TokenizerParseInfo ParseMissionMapList[] = {
    { "MissionSet",        TOK_STRUCT(MissionMapList,missionmapsets,ParseMissionMapSet) },
    { "", 0, 0 }
};

SHARED_MEMORY MissionMapList g_missionmaplist;

TokenizerParseInfo parse_MissionCustomEncounterStats[] =
{
    {"",    TOK_STRUCTPARAM | TOK_STRING(MissionCustomEncounterStats, type, " ")},
    {"",    TOK_STRUCTPARAM | TOK_INT(MissionCustomEncounterStats, countGrouped, 0)},
    {"",    TOK_STRUCTPARAM | TOK_INT(MissionCustomEncounterStats, countOnly, 0)},
    {"\n",    TOK_STRUCTPARAM | TOK_END, 0},
    {"", 0, 0 }
};

TokenizerParseInfo parse_MissionPlaceStats[] =
{
    {"{",                        TOK_START, 0},
    {"place",                    TOK_INT(MissionPlaceStats, place, -1), ParseMissionPlaceEnum},
    {"genericEncounterCount",    TOK_INT(MissionPlaceStats, genericEncounterGroupedCount, -1)},
    {"genericEncounterOnlyCount",    TOK_INT(MissionPlaceStats, genericEncounterOnlyCount, 0)},
    {"hostageLocationCount",    TOK_INT(MissionPlaceStats, hostageLocationCount, -1)},
    {"wallObjectiveCount",        TOK_INT(MissionPlaceStats, wallObjectiveCount, -1)},
    {"floorObjectiveCount",        TOK_INT(MissionPlaceStats, floorObjectiveCount, -1)},
    {"roofObjectiveCount",        TOK_INT(MissionPlaceStats, roofObjectiveCount, -1)},
    {"customEncounters",        TOK_STRUCT(MissionPlaceStats, customEncounters, parse_MissionCustomEncounterStats)},
    {"}",                        TOK_END, 0},
    {"", 0, 0 }
};

TokenizerParseInfo parse_MissionMapStats[] =
{
    {"{",                    TOK_START, 0 },
    {"Name",                TOK_STRING(MissionMapStats, pchMapName, 0) },
    {"roomCount",            TOK_INT(MissionMapStats, roomCount, -1), NULL },
    {"floorCount",            TOK_INT(MissionMapStats, floorCount, 0), NULL},
    {"objectiveRoomCount",    TOK_INT(MissionMapStats, objectiveRoomCount, -1), NULL },
    {"placeCount",            TOK_INT(MissionMapStats, placeCount, -1), NULL },
    {"places",                TOK_STRUCT(MissionMapStats, places, parse_MissionPlaceStats) },
    {"}",                    TOK_END, 0 },
    {"", 0, 0 }
};

TokenizerParseInfo parse_MissionMapStatList[] = 
{
    { "{", TOK_STRUCT(MissionMapStatList, ppStat, parse_MissionMapStats), },
    {0}
};

#ifndef TEST_CLIENT
TokenizerParseInfo parse_ArchitectMapHeaderList[] = 
{
    { "{", TOK_STRUCT(MinimapHeaderList, ppHeader, parse_ArchitectMapHeader), },
    {0}
};
#endif
MissionMapStatList gMissionMapStats = {0};
#ifndef TEST_CLIENT
MinimapHeaderList gArchitectMapHeaders = {0};
#endif
StashTable stMissionMapStats;
StashTable stMissionMapImages;
StashTable stMissionMapHeaders = {0};


static bool map_preprocess(ParseTable* unused, void* structptr)
{
    MissionMapList * missionmaplist = (MissionMapList *)structptr;
    int i, j, k;

    for( i = 0; i < eaSize(&missionmaplist->missionmapsets); i++ )
    {
        MissionMapSet *pMapSet = cpp_const_cast(MissionMapSet*)(missionmaplist->missionmapsets[i]);
        for( j = 0; j < eaSize(&pMapSet->missionmaptimes); j++ )
        {        
            MissionMapTime *pTime = cpp_const_cast(MissionMapTime*)(pMapSet->missionmaptimes[j]);
            pTime->randomLimit.wall[0] = pTime->randomLimit.floor[0] = pTime->randomLimit.around_only[0] = pTime->randomLimit.regular_only[0] = pTime->randomLimit.around_or_regular[0] = 0x7fffffff;
            // gather mapstats 
            for( k = 0; k < eaSize(&pTime->maps); k++ )
            {
                MissionMap * pMap = pTime->maps[k];
                MissionMapStats * pStat = missionMapStat_Get( pMap->mapfile );
                int x;
                if( pStat )
                {
                    MissionPlaceStats * pPlace = pStat->places[0];
                    MIN1(pTime->randomLimit.regular_only[0], pPlace->genericEncounterOnlyCount);
                    for( x = 0; x < eaSize(&pPlace->customEncounters); x++ )
                    {
                        if( stricmp( "Around", pPlace->customEncounters[x]->type ) == 0 || stricmp( "GiantMonster", pPlace->customEncounters[x]->type ) == 0)
                        {
                            MIN1(pTime->randomLimit.around_only[0], pPlace->customEncounters[x]->countOnly);
                            MIN1(pTime->randomLimit.around_or_regular[0], pPlace->customEncounters[x]->countGrouped);
                        }
                    }
                    MIN1(pTime->randomLimit.wall[0], pPlace->wallObjectiveCount);
                    MIN1(pTime->randomLimit.floor[0], pPlace->floorObjectiveCount);
                }
            }
        }
    }
    return true;
}

void MissionMapPreload()
{
    int flags = 0;

    if (g_missionmaplist.missionmapsets) 
    {
        flags = PARSER_FORCEREBUILD;
        ParserUnload(ParseMissionMapList, &g_missionmaplist, sizeof(g_missionmaplist));
    }

#ifndef TEST_CLIENT
    {
        int i = 0;
        for( i = 0; i < eaSize(&gMissionMapStats.ppStat) ; i++ )
        {
            if( gMissionMapStats.ppStat[i] )
                missionMapStats_destroy( gMissionMapStats.ppStat[i] );
        }
        eaDestroy( &gMissionMapStats.ppStat );
        if(ParserLoadFiles("maps/missions", ".mapstats", "mapstats.bin", 0, parse_MissionMapStatList, &gMissionMapStats, NULL, NULL, NULL))
        {
            stMissionMapStats = stashTableCreateWithStringKeys(1024, StashDefault);
            for( i = 0; i < eaSize(&gMissionMapStats.ppStat); i++ )
            {
                MissionMapStats *pStat = gMissionMapStats.ppStat[i];
                stashAddPointer(stMissionMapStats, pStat->pchMapName, pStat, 0 );
            }
        }
        }
#endif

#ifndef TEST_CLIENT
    missionMapHeader_Destroy(&gArchitectMapHeaders);
    if(ParserLoadFiles("maps/missions", ".minimap", "minimap.bin", 0, parse_ArchitectMapHeaderList, &gArchitectMapHeaders, NULL, NULL, NULL))
    {
        int i = 0;
        stMissionMapHeaders = stashTableCreateWithStringKeys(1024, StashDefault);
        for( i = 0; i < eaSize(&gArchitectMapHeaders.ppHeader); i++ )
        {
            ArchitectMapHeader *pHeader = gArchitectMapHeaders.ppHeader[i];
            stashAddPointer(stMissionMapHeaders, pHeader->mapName, pHeader, 0 );
        }
    }
#endif

    ParserLoadFilesShared("SM_mapspecs.bin", "maps/Missions", ".mapspecs", "mapspecs.bin", flags, ParseMissionMapList, &g_missionmaplist, sizeof(g_missionmaplist), NULL, NULL, NULL, map_preprocess, NULL);
}
void missionMapHeader_GetMapNames(char ***names)
{
#ifndef TEST_CLIENT
    int i;
    eaSetSize(names, 0);
    for( i = 0; i < eaSize(&gArchitectMapHeaders.ppHeader); i++ )
    {
        ArchitectMapHeader *pHeader = gArchitectMapHeaders.ppHeader[i];
        eaPush(names, pHeader->mapName);
    }
#endif
}

MissionMap **MissionMapGetList(int mapset, int maptime)
{

    int s, nsets, t, ntimes, nummaps;
    MissionMap** maps;

    // look for correct mapset
    nsets = eaSize(&g_missionmaplist.missionmapsets);
    for (s = 0; s < nsets; s++)
    {
        // found it..
        if (g_missionmaplist.missionmapsets[s]->mapset == mapset)
        {
            ntimes = eaSize(&g_missionmaplist.missionmapsets[s]->missionmaptimes);
            for (t = 0; t < ntimes; t++)
            {
                maps = g_missionmaplist.missionmapsets[s]->missionmaptimes[t]->maps;
                nummaps = eaSize(&maps);
                if (g_missionmaplist.missionmapsets[s]->missionmaptimes[t]->maptime == maptime && nummaps)
                {
                    return maps;
                }
            }
        }
    }
    return NULL;
}

void MissionMapSetForEach(const char* matchfile, int (*func)(MapSetEnum mapset, int time))
{
    int s, nsets;
    int t, ntimes;
    int m, nummaps;
    MissionMap** maps;

    nsets = eaSize(&g_missionmaplist.missionmapsets);
    for (s = 0; s < nsets; s++)
    {
        ntimes = eaSize(&g_missionmaplist.missionmapsets[s]->missionmaptimes);
        for (t = 0; t < ntimes; t++)
        {
            maps = g_missionmaplist.missionmapsets[s]->missionmaptimes[t]->maps;
            nummaps = eaSize(&maps);
            for (m = 0; m < nummaps; m++)
            {
                if (!stricmp(matchfile, maps[m]->mapfile))
                {
                    if (!func(g_missionmaplist.missionmapsets[s]->mapset, 
                        g_missionmaplist.missionmapsets[s]->missionmaptimes[t]->maptime))
                        return;
                    // can't break here - designers repeat maps sometimes
                }
            }
        }
    }
}

int MapSetAllowsRandomDoorType(MapSetEnum mapset, char* doorType)
{
    int i, n = eaSize(&g_missionmaplist.missionmapsets);
    for (i = 0; i < n; i++)
    {
        const MissionMapSet* map = g_missionmaplist.missionmapsets[i];
        if (map->mapset == mapset)
        {
            int j, numType = eaSize(&map->randomDoorTypes);
            for (j = 0; j < numType; j++)
                if (!stricmp(map->randomDoorTypes[j], doorType))
                    return 1;
            return 0;
        }
    }
    return 0;
}


void MissionMapGetAllMapNames(char*** mapNames)
{
    int nsets, ntimes, nmaps;
    int s, t, u;
    MissionMap** maps;

    nsets = eaSize(&g_missionmaplist.missionmapsets);
    for (s = 0; s < nsets; s++)
    {
        ntimes = eaSize(&g_missionmaplist.missionmapsets[s]->missionmaptimes);
        for (t = 0; t < ntimes; t++)
        {
            maps = g_missionmaplist.missionmapsets[s]->missionmaptimes[t]->maps;
            nmaps = eaSize(&maps);
            for (u = 0; u < nmaps; u++)
            {
                eaPush(mapNames, strdup(maps[u]->mapfile));
            }
        }
    }
}


// BE SURE to call MissionMapStats_destroy() on any MissionMapStats you initialize!
MissionMapStats* missionMapStats_initialize(char * pchName)
{
    int placeIndex;
    MissionMapStats *stats = StructAllocRaw(sizeof(MissionMapStats));

    stats->pchMapName = StructAllocString(pchName);

    stats->roomCount = 0;
    stats->floorCount = 0;
    stats->objectiveRoomCount = 0;
    stats->placeCount = 6;

    for (placeIndex = 0; placeIndex < stats->placeCount; placeIndex++)
    {
        MissionPlaceStats *placeStats = StructAllocRaw(sizeof(MissionPlaceStats));
        placeStats->genericEncounterOnlyCount = 0;
        placeStats->hostageLocationCount = 0;
        placeStats->wallObjectiveCount = 0;
        placeStats->floorObjectiveCount = 0;
        placeStats->roofObjectiveCount = 0;
        eaPush(&stats->places, placeStats);
    }

    stats->places[0]->place = MISSION_ANY;
    stats->places[1]->place = MISSION_LOBBY;
    stats->places[2]->place = MISSION_FRONT;
    stats->places[3]->place = MISSION_MIDDLE;
    stats->places[4]->place = MISSION_BACK;
    stats->places[5]->place = MISSION_NONE;

    return stats;
}

void missionMapStats_destroy(MissionMapStats *mapStats)
{
    StructDestroy(parse_MissionMapStats, mapStats);
}

void missionMapStats_Save(MissionMapStats *mapStats, const char * pchFile)
{
    ParserWriteTextFile(pchFile, parse_MissionMapStats, mapStats, 0, 0);
}

MissionMapStats * missionMapStat_Get( const char * pchFile )
{
    MissionMapStats * pStat = 0;
    stashFindPointer(stMissionMapStats, pchFile, &pStat);
    return pStat;
}

#ifndef TEST_CLIENT
ArchitectMapHeader * missionMapHeader_Get( const char * pchFile )
{
    ArchitectMapHeader *pHeader = 0;
    if(stMissionMapHeaders)
        stashFindPointer(stMissionMapHeaders, pchFile, &pHeader);
    return pHeader;
}

static void architectSubMap_Destroy( ArchitectMapSubMap * subMap )
{
    int i, j;
    ArchitectMapComponent *component;

    for( i = 0; i < eaSize(&subMap->components); i++ )
    {
        component = subMap->components[i];

        if( component )
        {
            SAFE_FREE( component->name );

            for( j = 0; j < eaSize( &component->places ); j++)
            {
                SAFE_FREE( component->places[j] );
            }

            eaDestroy( &component->places );
            SAFE_FREE( component );
        }
    }

    eaDestroy( &subMap->components );
}

void missionMapHeader_Destroy( MinimapHeaderList * headerList )
{
    int i, j;
    ArchitectMapHeader *header;

    for( i = 0; i < eaSize(&headerList->ppHeader); i++ )
    {
        header = headerList->ppHeader[i];

        if( header )
        {
            SAFE_FREE( header->mapName );

            for( j = 0; j < eaSize(&header->mapFloors); j++ )
            {
                architectSubMap_Destroy( header->mapFloors[j] );
                SAFE_FREE( header->mapFloors[j] );
            }
            for( j = 0; j < eaSize(&header->mapItems); j++ )
            {
                architectSubMap_Destroy( header->mapItems[j] );
                SAFE_FREE( header->mapItems[j] );
            }
            for( j = 0; j < eaSize(&header->mapSpawns); j++ )
            {
                architectSubMap_Destroy( header->mapSpawns[j] );
                SAFE_FREE( header->mapSpawns[j] );
            }
            eaDestroy( &header->mapFloors );
            eaDestroy( &header->mapItems );
            eaDestroy( &header->mapSpawns );
            SAFE_FREE( header );
        }
    }

    eaDestroy( &headerList->ppHeader );
}
#endif

const MapLimit* missionMapStat_GetRandom( const char *pchCategory, int iTime )
{
    int i, j;

    for( i = 0; i < eaSize(&g_missionmaplist.missionmapsets); i++ )
    {
        const MissionMapSet *pMapSet = g_missionmaplist.missionmapsets[i];
        if( stricmp(pchCategory, StaticDefineIntRevLookup(ParseMapSetEnum,pMapSet->mapset) ) == 0)
        {
            for( j = 0; j < eaSize(&pMapSet->missionmaptimes); j++ )
            {
                const MissionMapTime *pTime = pMapSet->missionmaptimes[j];
                if( iTime == pTime->maptime )
                    return &pTime->randomLimit;
            }
        }
    }

    return 0;
}

int missionMapStat_isFileValid( char * pchFile, char * pchCategory, int iTime )
{
    int i, j, k;
    int eCat;

    if( !pchFile || !pchCategory )
        return 0;        
        
    eCat = StaticDefineIntGetInt(ParseMapSetEnum, pchCategory);

    for( i = 0; i < eaSize(&g_missionmaplist.missionmapsets); i++ )
    {
        const MissionMapSet *pMapSet = g_missionmaplist.missionmapsets[i];
        if( eCat == pMapSet->mapset )
        {
            for( j = 0; j < eaSize(&pMapSet->missionmaptimes); j++ )
            {
                const MissionMapTime *pTime = pMapSet->missionmaptimes[j];
                if( iTime == pTime->maptime )
                {
                    for( k = 0; k < eaSize(&pTime->maps); k++ )
                    {
                        if( pTime->maps[k]->mapfile && stricmp(pchFile, pTime->maps[k]->mapfile ) == 0 )
                            return 1;
                    }
                }
            }
        }
    }

    return 0;
}