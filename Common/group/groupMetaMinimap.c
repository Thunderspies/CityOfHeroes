#ifndef TEST_CLIENT
#include "group/groupMetaMinimap.h"
#endif
#include <utilitieslib/components/earray.h>
#include "storyarc/missiongeoCommon.h"
#include "seq/anim.h"
#include <utilitieslib/utils/utils.h>
#include "group/group.h"
#include "group/grouputil.h"
#include "group/groupfileload.h"
#include <utilitieslib/utils/mathutil.h>
#include "group/groupProperties.h"
#include "bases/bases.h"
#include <utilitieslib/utils/error.h>
#if SERVER
#include "generator/encounter.h"
#endif

///////////////////////////////////////////////////////////////////////////
//stuff originally in automap, but moved into this common file so that the mapserver can use it to make minimaps
// Loads extent markers from the map file
//
static MissionExtents s_missionExtents = {0};
static int s_section_count = 0;
static Extent s_missionSection[MAX_EXTENTS] = {0};
static Extent * s_cur_extent;

MapEncounterSpawn **g_spawnList;
MapEncounterSpawn *g_curSpawn;
MapState mapState = {0, 0, 0, 0, 0, 1.f};

MapEncounterSpawn *** minimap_getSpawnList()
{
    return &g_spawnList;
}

Extent * minimap_getCurrentExtent()
{
    return s_cur_extent;
}

int withinCurrentSection( const Mat4 mat, const MapState *map_state )
{
    if ( ( mat[3][0] <= map_state->max[0] &&
        mat[3][0] >= map_state->min[0] &&
        mat[3][2] <= map_state->max[2] &&
        mat[3][2] >= map_state->min[2] ) ||
         (map_state->mission_type == MAP_OUTDOOR_MISSION) ||
         (map_state->mission_type == MAP_CITY) ||
         (map_state->mission_type == MAP_ZONE)            //    MAP_OUTDOOR_INDOOR is excluded because it is does have sections defined
         )
    {
        return TRUE;
    }
    else
        return FALSE;
}

void missionMapLoad()
{
    GroupDefTraverser traverser = {0};

    groupProcessDefExBegin(&traverser, &group_info, minimap_missionExtentsLoader);
}

static int subGroupLoader(GroupDefTraverser* traverser)
{
    PropertyEnt* spawnProp;

    // find the spawn point
    if (!traverser->def->properties) 
    {
        if (!traverser->def->child_properties)    // if none of our children have props, don't go further
            return 0;
        return 1;
    }
    stashFindPointer( traverser->def->properties, "EncounterSpawn", &spawnProp );

    // if we have a point
    if (spawnProp)
    {
        if( stricmp( spawnProp->value_str, "Mission" ) == 0 )
        {
            if( g_curSpawn->type == 2 )
                g_curSpawn->type = 3;
            else
                g_curSpawn->type = 1;
        }
        if( stricmp( spawnProp->value_str, "Around" ) == 0 )
        {
            if( g_curSpawn->type == 1 )
                g_curSpawn->type = 3;
            else
                g_curSpawn->type = 2;
        }
    }
    if (!traverser->def->child_properties)    // if none of our children have props, don't go further
        return 0;
    return 1; // traverse children
}


static int spawnEncounterLoader(GroupDefTraverser* traverser)
{
    PropertyEnt* generatorProp;

    // find the group
    if (!traverser->def->properties)
    {
        if (!traverser->def->child_properties)    // if none of our children have props, don't go further
            return 0;
        return 1;
    }

    stashFindPointer( traverser->def->properties, "EncounterGroup", &generatorProp );

    // if we have a group
    if (generatorProp)
    {
        RoomInfo *pRoom = RoomGetInfo( traverser->mat[3] );
        g_curSpawn = calloc(1, sizeof(MapEncounterSpawn));
        g_curSpawn->start = pRoom->startroom;
        g_curSpawn->end = pRoom->endroom;

        copyMat4(traverser->mat, g_curSpawn->mat);

        // load all EncounterSpawn children
        groupProcessDefExContinue(traverser, subGroupLoader);

        if( g_curSpawn->type )
            eaPush( &g_spawnList, g_curSpawn );

        return 0; // stop traversing, EncounterGroups as children should be ignored
    }
    if (!traverser->def->child_properties)    // if none of our children have props, don't go further
        return 0;
    return 1; // traverse children
}


// Gets the bounds of the world our map is on
//
static int getExtents(GroupDef *def,Mat4 mat)
{
    MapState *pMapState = minimap_getMapState();
    groupLoadIfPreload(def);
    if( def->name && strnicmp(def->name,"map_",4)==0 ) // def->tray ||
    {
        Vec3    min,max;
        int     i;

        mulVecMat4(def->min,mat,min);
        mulVecMat4(def->max,mat,max);
        for(i=0;i<3;i++)
        {
            pMapState->min[i] = MIN( max[i],min[i]);
            pMapState->max[i] = MAX( max[i],min[i]);
        }
        pMapState->width  = pMapState->max[0] - pMapState->min[0];
        pMapState->height = pMapState->max[2] - pMapState->min[2];

        return 0; //you've found your extents, you're done
    }

    return 1;
}

static int s_blackPolyExtentMarkerGroupNum = 0;

static int findBlackPolyExtentMarkerGroup(GroupDef *def, Mat4 mat)
{
    PropertyEnt *pairProp;

    stashFindPointer(def->properties, "outdoorextentsMarkerGroup", &pairProp);

    if (pairProp)
    {
        s_blackPolyExtentMarkerGroupNum = atoi(pairProp->value_str);
        return 0;
    }

    return 1;
}

static int getBlackPolyMissionExtent(GroupDef *def,Mat4 mat)
{
    PropertyEnt *polyProp = NULL;
    groupLoadIfPreload(def);

    stashFindPointer(def->properties, "BlackpolyExtentsGroup", &polyProp);

    if(strnicmp(def->name,"map_",4)==0 || polyProp) // def->tray ||
    {
        PropertyEnt *outdoorTextureProp = NULL;

        int groupNum = s_blackPolyExtentMarkerGroupNum;

        if (polyProp)
        {
            groupNum = atoi(polyProp->value_str);
            stashFindPointer(def->properties, "MinimapTextureName", &outdoorTextureProp);
        }
        
        if( s_section_count < MAX_EXTENTS )
        {
            Vec3 min, max;

            mulVecMat4(def->min, mat, min);
            mulVecMat4(def->max, mat, max);

            MINVEC3(min, max, s_missionExtents.vec[s_missionExtents.count]);
            s_missionExtents.num[s_missionExtents.count] = groupNum;
            s_missionExtents.outdoor[s_missionExtents.count] = true;
            
            SAFE_FREE(s_missionExtents.outdoor_texture[s_missionExtents.count]);
            if(outdoorTextureProp && outdoorTextureProp->value_str)
                s_missionExtents.outdoor_texture[s_missionExtents.count] = strdup(outdoorTextureProp->value_str);

            s_missionExtents.count++;

            MAXVEC3(min, max, s_missionExtents.vec[s_missionExtents.count]);
            s_missionExtents.num[s_missionExtents.count] = groupNum;
            s_missionExtents.outdoor[s_missionExtents.count] = true;

            SAFE_FREE(s_missionExtents.outdoor_texture[s_missionExtents.count]);
            if(outdoorTextureProp && outdoorTextureProp->value_str)
                s_missionExtents.outdoor_texture[s_missionExtents.count] = strdup(outdoorTextureProp->value_str);

            s_missionExtents.count++;
        }

        return 0; //you've found your extents, you're done
    }

    return 1;
}

int minimap_getCurrentSection(int usey, const F32* position)
{
    int i;
    Extent *missionSection = minimap_getSections();
    MapState *pMapState = minimap_getMapState();

    for( i = s_section_count-1; i >= 0; i-- )
    {
        if( position[0] < missionSection[i].max[0] &&
            position[0] > missionSection[i].min[0] &&
            position[2] < missionSection[i].max[2] &&
            position[2] > missionSection[i].min[2] &&
            (usey ? (position[1] < missionSection[i].max[1] && position[1] > missionSection[i].min[1]) : 1))
        {
            copyVec3( missionSection[i].max, pMapState->max );
            copyVec3( missionSection[i].min, pMapState->min );

            pMapState->width  = pMapState->max[0] - pMapState->min[0];
            pMapState->height = pMapState->max[2] - pMapState->min[2];
            s_cur_extent = &missionSection[i];
            return true;
        }
    }

    return false;
}


int minimap_missionExtentsLoader(GroupDefTraverser* traverser)
{
    PropertyEnt* pairProp;

    groupLoadIfPreload(traverser->def);
    // find the group
    if (!traverser->def->properties)
    {
        if (!traverser->def->child_properties)  // if none of our children have props, don't go further
            return 0;
        return 1;
    }

    stashFindPointer( traverser->def->properties, "extentsMarkerGroup", &pairProp );

    //if(!pairProp)
    //    pairProp = stashFindPointerReturnPointer(traverser->def->properties, "outdoorextentsMarkerGroup");

    // if we found marker
    if( pairProp )
    {
        // set mat
        copyMat4(traverser->mat, s_missionExtents.mat[s_missionExtents.count] );
        s_missionExtents.num[s_missionExtents.count] = atoi( pairProp->value_str );
        s_missionExtents.count++;
        return 0;
    }

    if (!traverser->def->child_properties)  // if none of our children have props, don't go further
        return 0;
    return 1; // traverse children
}

//--------------------------------------------------------------------------


void minimap_processMissionExtents()
{
    int i, j;
    int num;
    int *sortedList = NULL;
    int *roomList = NULL;
    s_section_count = 0;

    for( i = 0; i < s_missionExtents.count; i++ )
    {
        int insertLocation = eaiSortedPush(&roomList, s_missionExtents.num[i]);
        eaiInsert(&sortedList, i, insertLocation);
    }

    for( i = 0; i < s_missionExtents.count; i++ )
    {
        int sortedIdx = sortedList[i];
        num = s_missionExtents.num[sortedIdx];

        for( j = (i+1); j <  s_missionExtents.count && s_section_count < MAX_EXTENTS; j++ )
        {
            int pairIdx = sortedList[j];
            if( num == s_missionExtents.num[pairIdx] ) // found a matching pair
            {
                SAFE_FREE(s_missionSection[s_section_count].outdoor_texture);
                if (!s_missionExtents.outdoor[sortedIdx]) // if the pair doesn't match outdoor, we've got problems...
                {
                    s_missionSection[s_section_count].max[0] = MAX(s_missionExtents.mat[sortedIdx][3][0], s_missionExtents.mat[pairIdx][3][0] );
                    s_missionSection[s_section_count].max[1] = MAX(s_missionExtents.mat[sortedIdx][3][1], s_missionExtents.mat[pairIdx][3][1] );
                    s_missionSection[s_section_count].max[2] = MAX(s_missionExtents.mat[sortedIdx][3][2], s_missionExtents.mat[pairIdx][3][2] );
                    s_missionSection[s_section_count].min[0] = MIN(s_missionExtents.mat[sortedIdx][3][0], s_missionExtents.mat[pairIdx][3][0] );
                    s_missionSection[s_section_count].min[1] = MIN(s_missionExtents.mat[sortedIdx][3][1], s_missionExtents.mat[pairIdx][3][1] );
                    s_missionSection[s_section_count].min[2] = MIN(s_missionExtents.mat[sortedIdx][3][2], s_missionExtents.mat[pairIdx][3][2] );
                    s_missionSection[s_section_count].outdoor_poly = false;
                }
                else
                {
                    s_missionSection[s_section_count].max[0] = MAX(s_missionExtents.vec[sortedIdx][0], s_missionExtents.vec[pairIdx][0] );
                    s_missionSection[s_section_count].max[1] = MAX(s_missionExtents.vec[sortedIdx][1], s_missionExtents.vec[pairIdx][1] );
                    s_missionSection[s_section_count].max[2] = MAX(s_missionExtents.vec[sortedIdx][2], s_missionExtents.vec[pairIdx][2] );
                    s_missionSection[s_section_count].min[0] = MIN(s_missionExtents.vec[sortedIdx][0], s_missionExtents.vec[pairIdx][0] );
                    s_missionSection[s_section_count].min[1] = MIN(s_missionExtents.vec[sortedIdx][1], s_missionExtents.vec[pairIdx][1] );
                    s_missionSection[s_section_count].min[2] = MIN(s_missionExtents.vec[sortedIdx][2], s_missionExtents.vec[pairIdx][2] );
                    s_missionSection[s_section_count].outdoor_poly = true;
                    
                    if(s_missionExtents.outdoor_texture[sortedIdx])
                        s_missionSection[s_section_count].outdoor_texture = strdup(s_missionExtents.outdoor_texture[sortedIdx]);

                    mapState.mission_type = MAP_OUTDOOR_INDOOR;
                }

                s_section_count++;
            }
        }
    }

    // Clear memory for unused sections
    for (i = s_section_count; i < MAX_EXTENTS; i++)
    {
        SAFE_FREE(s_missionSection[i].outdoor_texture);
        memset(&s_missionSection[i], 0, sizeof(Extent));
    }

    eaiDestroy(&roomList);
    eaiDestroy(&sortedList);
}


///////////////////////////////////////////////////////////////////////////
//fill out the header for Architect minimaps.
static ArchitectMapSubMap * currentMap = 0;

static void automap_fillHeaderComponent(ArchitectMapSubMap * map, const char * name, float xp, float yp, float zp, float xscale, float yscale, int rgba, float angle, int additive, int istext, int rgba2)
{
    int nComponents = eaSize(&map->components);
    int i;
    int found = 0;
    ArchitectMapComponent *component = 0;
    ArchitectMapComponentPlace *place = 0;
    int isDefault = 0;
    //if there are no map defaults, set them.

    for(i=0; i < nComponents && !found; i++)
    {
        char *existingName = map->components[i]->name;
        if(map->components[i]->isText != istext)
            continue;
        if(stricmp(name,existingName)<0)//name is < existingName
            found = 1;
        else if(!stricmp(name,existingName))
        {
            component = map->components[i];
            found = 1;
        }
    }
    if(!component)
    {
        if(found)
            i--;    //we incremented at the end of the for loop.
        component = StructAllocRaw(sizeof(ArchitectMapComponent));
        component->name = StructAllocString(name);
        component->isText = istext;
        if(i == nComponents)
            eaPush(&map->components, component);
        else
            eaInsert(&map->components, component, i);
    }

    if(!map->defaultScX && !map->defaultScY)
    {
        map->defaultScX = xscale;
        map->defaultScY = yscale;
        isDefault = 1;
    }
    //if it's our first time through this component and the scale isn't the map default, set the component default.
    else if(!component->places && (map->defaultScX != xscale && map->defaultScY != yscale) && (!component->defaultScX && !component->defaultScY))
    {
        component->defaultScX = xscale;
        component->defaultScY = yscale;
        isDefault = 1;
    }
    else if((map->defaultScX == xscale && map->defaultScY == yscale) || (component->defaultScX == xscale && component->defaultScY == yscale))
    {
        isDefault = 1;
    }

    place = StructAllocRaw(sizeof(ArchitectMapComponentPlace));
    FillArchitectMapComponentPlace(place, xp, yp, zp, isDefault?0:xscale, isDefault?0:yscale, angle, rgba, additive);
    if(component->isText)
        place->color2 = rgba2;

    eaPush(&component->places, place);
}

static void fillHeaderWithItemLocations(ArchitectMapSubMap * map,int type, int flip_red)
{
    int i, j;
    F32 x, y, z= 0;

    switch (type)
    {
    case ARCHITECT_MAP_TYPE_ITEMS:
        {
            for( i = 0;  i < eaSize(&g_rooms); i++ )
            {
                RoomInfo *pRoom = g_rooms[i];
                for( j = 0; j < eaSize(&pRoom->items); j++ )
                {
                    const ItemPosition *pItem = pRoom->items[j];
                    char *text;
                    F32 zoom;
                    U32 color;
                    if(pItem->loctype == LOCATION_ITEMWALL)
                    {
                        text = "\xE2\x96\xB6"; // This is a right arrow U+25B6
                        zoom = mapState.zoom;
                    }
                    else if(pItem->loctype == LOCATION_ITEMFLOOR)
                    {
                        text = "\xE2\x96\xBC"; // This is a down Arrow U+25B7
                        zoom = mapState.zoom;
                    }
                    else
                        continue;

                    if( !withinCurrentSection( pItem->pos, &mapState ) )
                        continue;

                    color =ARCHITECT_COLOR_WHITE;
                    if( pRoom )
                    {
                        if( pRoom->startroom )
                            color =ARCHITECT_COLOR_GREEN;
                        else if( pRoom->endroom )
                        {
                            if(flip_red)
                                color =ARCHITECT_COLOR_BLUE;
                            else
                                color =ARCHITECT_COLOR_RED;
                        }
                    }

                    // coordinates to draw sprite
                    x = mapState.xp + ((mapState.width - (pItem->pos[3][0] - mapState.min[0])))*mapState.zoom;
                    y = mapState.yp + (((pItem->pos[3][2] - mapState.min[2])))*mapState.zoom;

                    automap_fillHeaderComponent(map, text, x, y, z, zoom, zoom, color, 0, 0, 1, color);
                }
            }
        }
    break;
    case ARCHITECT_MAP_TYPE_SPAWNS:
        {
            int numSpawns = eaSize(&g_spawnList);
            for( i = 0; i < numSpawns; i++ )
            {
                //AtlasTex * icon;
                MapEncounterSpawn *pSpawn = g_spawnList[i];
                F32 sc = mapState.zoom*2.f;
                char *text;
                U32 color;

                if( !withinCurrentSection( pSpawn->mat, &mapState ) )
                    continue;

                // coordinates to draw sprite
                x = mapState.xp + ((mapState.width - (pSpawn->mat [3][0] - mapState.min[0])))*mapState.zoom;
                y = mapState.yp + (((pSpawn->mat [3][2] - mapState.min[2])))*mapState.zoom;

                color = ARCHITECT_COLOR_WHITE;
                if( pSpawn->start )
                {
                    color = ARCHITECT_COLOR_GREEN;
                }
                else if( pSpawn->end)
                {
                    if(flip_red)
                        color = ARCHITECT_COLOR_BLUE;
                    else
                        color = ARCHITECT_COLOR_RED;
                }

                if(pSpawn->type == 1)
                {
                    text = "X";
                }
                else if( pSpawn->type == 2 )
                {
                    text = "O";
                }
                else if( pSpawn->type == 3 )
                {
                    text = "\xE2\x97\x8f"; // This is a circle U+25CF
                    sc*=1.7;
                }
                else 
                    continue;

                automap_fillHeaderComponent(map, text, x, y, z+1, mapState.zoom, mapState.zoom, color, 0, 0, 1, color);
            }
        }
        break;
    default:
        assert(0);    //    undefined flag
    }
}


static int fillHeaderWithMapTilesImage(GroupDef *def,Mat4 mat)
{
    int zonemap = (def->model && strnicmp(def->model->name,"map_",4)==0);
    float z = 0;
    PropertyEnt* typeProp = stashFindPointerReturnPointer(def->properties, "MinimapTextureName" );

    if (def->is_tray || zonemap || typeProp )
    {
        if (zonemap)
        {
            int x = mapState.xp + (mat[3][0] - mapState.min[0])/(mapState.max[0] - mapState.min[0])*mapState.zoom;
            int y = mapState.xp + (mat[3][0] - mapState.min[0])/(mapState.max[0] - mapState.min[0])*mapState.zoom;
            automap_fillHeaderComponent(currentMap, def->model->name, x, y, z, mapState.zoom, mapState.zoom, ARCHITECT_COLOR_RGBA, 0, 0, 0, 0);
            //how we originally got the right posiiton.
            /*AtlasTex * name = atlasLoadTexture( def->model->name );
            int x = mapState.xp + (mat[3][0] - mapState.min[0])/(mapState.max[0] - mapState.min[0])*mapState.zoom - name->width/2*mapState.zoom;
            int y = mapState.xp + (mat[3][0] - mapState.min[0])/(mapState.max[0] - mapState.min[0])*mapState.zoom - name->height/2*mapState.zoom;

            automap_fillHeaderComponent(currentMap, def->model->name, x, y, z, mapState.zoom, mapState.zoom, DEFAULT_RGBA, 0, 0, 0, 0);*/
        }
        else
        {
            //AtlasTex * name;
            const char *name_ptr;
            int x, y;
            Vec3 min,max;
            float angle;
            int color = ARCHITECT_COLOR_WHITE;

            // grab the location
            mulVecMat4(def->min,mat,min);
            mulVecMat4(def->max,mat,max);

            if( !withinCurrentSection( mat, &mapState ) )
                return 0;

            // get the sprite (ignore leading underscores)
            if( typeProp && typeProp->value_str )
                name_ptr = typeProp->value_str;
            else if( def->name[0] == '_' )
                name_ptr = &def->name[1];
            else
                name_ptr = def->name;

            /*name = atlasLoadTexture( name_ptr );

            // coordinates to draw sprite
            x = mapState.xp + ((mapState.width - (mat[3][0] - mapState.min[0])) - name->width)*mapState.zoom;
            y = mapState.yp + (((mat[3][2] - mapState.min[2])) - name->height)*mapState.zoom;*/

            x = mapState.xp + ((mapState.width - (mat[3][0] - mapState.min[0])))*mapState.zoom;
            y = mapState.yp + (((mat[3][2] - mapState.min[2])))*mapState.zoom;

            // rotation

            angle = acos(mat[0][0]);
            if( mat[0][2] > 0 )
                angle = -angle;

            automap_fillHeaderComponent(currentMap, name_ptr, x, y, z, 2*mapState.zoom*1.02, 2*mapState.zoom*1.02, color, angle, 0,0,0);
        }
        return 0;
    }

    if (!def->open_tracker || !def->child_ambient)
        return 0;
    return 1;
}

void automap_fillHeaderSectionType(ArchitectMapSubMap * map, char *mapname, ArchitectMapType type, F32 widthRatio, F32 heightRatio, int isOutdoor)
{
    char filename[256], tmp[256];


    switch(type)
    {
        xcase ARCHITECT_MAP_TYPE_FLOOR:
    if(mapState.mission_type != MAP_MISSION_MAP && mapState.mission_type != MAP_BASE && (mapState.mission_type != MAP_OUTDOOR_INDOOR || isOutdoor))
    {
        strcpy_s(SAFESTR(filename), getFileName(mapname));
        sprintf( tmp, "map_%s", strtok(filename,".") );
        automap_fillHeaderComponent(map, tmp, mapState.xp, mapState.yp, 0, widthRatio, heightRatio, ARCHITECT_COLOR_RGBA, 0, 0,0,0);
    }
    else
    {
        currentMap = map;
        groupProcessDef(&group_info, fillHeaderWithMapTilesImage);
        map = 0;
    }
    xcase ARCHITECT_MAP_TYPE_ITEMS:
    fillHeaderWithItemLocations(map,ARCHITECT_MAP_TYPE_ITEMS, 1);
    xcase ARCHITECT_MAP_TYPE_SPAWNS:
    fillHeaderWithItemLocations(map,ARCHITECT_MAP_TYPE_SPAWNS, 1);
xdefault:
    return;
    }
}

Extent *minimap_getSections()
{
    return s_missionSection;
}

MapState *minimap_getMapState()
{
    return &mapState;
}

MissionExtents *minimap_getMissionExtents()
{
    return &s_missionExtents;
}

int minimap_getSectioncount()
{
    return s_section_count;
}

// missionMap is a lie!  We call this function with missionMap == 1 on City Zones
// (at least via minimap_saveHeader()...)
int minimap_mapperInit(int missionMap, int makingImages)
{
    Extent *missionSection = minimap_getSections();
    MapState *pMapState = minimap_getMapState();
    MissionExtents *missionExtents = minimap_getMissionExtents();
#if SERVER
    int wasInMissionMode = EncounterInMissionMode();
#endif

    pMapState->mission_type = (!missionMap) ? MAP_CITY : MAP_MISSION_MAP;

    if (g_isBaseRaid)
    {
        pMapState->xp = 0;
        pMapState->yp = 0;
        //strcpy(last_map,gMapName);
        pMapState->zoom = 0;

        pMapState->mission_type = MAP_BASE;

        pMapState->min[0] = (float) ((g_sgRaidMapData.minx - 1) * g_base.roomBlockSize);
        pMapState->min[1] = 0.0f;
        pMapState->min[2] = (float) ((g_sgRaidMapData.minz - 1) * g_base.roomBlockSize);
        pMapState->max[0] = (float) ((g_sgRaidMapData.maxx + 1) * g_base.roomBlockSize);
        pMapState->max[1] = 0.0f;
        pMapState->max[2] = (float) ((g_sgRaidMapData.maxz + 1) * g_base.roomBlockSize);
        pMapState->width  = pMapState->max[0] - pMapState->min[0];
        pMapState->height = pMapState->max[2] - pMapState->min[2];


        return MAP_BASE;

    }
    if( g_base.rooms )
    {
        if( pMapState->width !=  g_base.plot_width * g_base.roomBlockSize ||
            pMapState->height != g_base.plot_height * g_base.roomBlockSize )
        {
            zeroVec3( pMapState->max );
            zeroVec3( pMapState->min );
            pMapState->xp = 0;
            pMapState->yp = 0;
            //strcpy(last_map,gMapName);
            pMapState->zoom = 0;

            pMapState->mission_type = MAP_BASE;

            copyVec3(g_base.plot_pos,pMapState->min);
            copyVec3(g_base.plot_pos,pMapState->max); 
            pMapState->width  = (g_base.plot_width+2) * (g_base.roomBlockSize);
            pMapState->height = (g_base.plot_height+2) * (g_base.roomBlockSize);
            pMapState->max[0] += pMapState->width;
            pMapState->max[2] += pMapState->height;
            pMapState->min[0] -= g_base.roomBlockSize;
            pMapState->min[2] -= g_base.roomBlockSize;
            pMapState->max[0] -= g_base.roomBlockSize;
            pMapState->max[2] -= g_base.roomBlockSize;
        }

        return MAP_BASE;
    }

    zeroVec3( pMapState->max );
    zeroVec3( pMapState->min );
    pMapState->xp = 0;
    pMapState->yp = 0;
    //strcpy(last_map,gMapName);
    pMapState->zoom = 0;

    if( missionMap ) // mission map
    {
        Vec3    min = {8e8,8e8,8e8},max = {-8e8,-8e8,-8e8};
        int     i,j;

        memset( missionExtents, 0, sizeof(MissionExtents));
        missionMapLoad();
        groupProcessDef(&group_info, findBlackPolyExtentMarkerGroup);
        groupProcessDef(&group_info, getBlackPolyMissionExtent);
        pMapState->mission_type = MAP_MISSION_MAP;
        minimap_processMissionExtents();

        for(i=0;i<s_section_count;i++)
        {
            for(j=0;j<3;j++)
            {
                min[j] = MIN(min[j],missionSection[i].min[j]);
                max[j] = MAX(max[j],missionSection[i].max[j]);
            }
        }

        copyVec3(min,pMapState->min);
        copyVec3(max,pMapState->max); 
        pMapState->width  = pMapState->max[0] - pMapState->min[0];
        pMapState->height = pMapState->max[2] - pMapState->min[2];

        if(!s_section_count) // outdoor only, treat like static map
        {
            groupProcessDef(&group_info,getExtents);
            pMapState->mission_type = MAP_OUTDOOR_MISSION;
            if(makingImages)
            {
                GroupDefTraverser traverser = {0};
                MissionLoad(1,0,0);
                while(eaSize(&g_spawnList))  // clear last list.
                {
                    MapEncounterSpawn *pEncounter = eaRemove(&g_spawnList, 0);
                    SAFE_FREE(pEncounter);
                }
                groupProcessDefExBegin(&traverser, &group_info, spawnEncounterLoader);
            }

#if SERVER
            if (!wasInMissionMode)
                EncounterSetMissionMode(0); // blatant hack, I don't think we should be calling MissionLoad() on city zones, but I don't understand this code well enough to want to change it
#endif

            return MAP_OUTDOOR_MISSION;
        }
    }
    else // static map
    {
        GroupDefTraverser traverser = {0};
        memset( missionExtents, 0, sizeof(MissionExtents));
        groupProcessDefExBegin(&traverser, &group_info, minimap_missionExtentsLoader);
        minimap_processMissionExtents();
        groupProcessDef(&group_info,getExtents);
    }

    MissionLoad(1,0,0);

#if SERVER
    if (!wasInMissionMode)
        EncounterSetMissionMode(0); // blatant hack, I don't think we should be calling MissionLoad() on city zones, but I don't understand this code well enough to want to change it
#endif

    if(makingImages)
    {
        GroupDefTraverser traverser = {0};
        while(eaSize(&g_spawnList))  // clear last list.
        {
            MapEncounterSpawn *pEncounter = eaRemove(&g_spawnList, 0);
            SAFE_FREE(pEncounter);
        }
        groupProcessDefExBegin(&traverser, &group_info, spawnEncounterLoader);
    }
    return (!missionMap) ? MAP_CITY : MAP_MISSION_MAP; // meh.. assume that static maps are city maps, could be city or zone but doesnt really matter
}

void automap_fillHeaderSection(ArchitectMapHeader * header, char *filename, int section, int height, int width)
{
    F32 widthRatio;
    F32 heightRatio;
    ArchitectMapSubMap *temp;
    Extent *missionSection = minimap_getSections();
    int isOutdoorMap = missionSection[section].outdoor_poly;

    if (    (mapState.mission_type == MAP_BASE) || (mapState.mission_type == MAP_MISSION_MAP) || 
            (mapState.mission_type == MAP_OUTDOOR_INDOOR) )
    {
        copyVec3(missionSection[section].min, mapState.min);
        copyVec3(missionSection[section].max, mapState.max);
    }
    mapState.width = mapState.max[0] - mapState.min[0];
    mapState.height = mapState.max[2] - mapState.min[2];

    widthRatio = (mapState.width)?(width/(F32)mapState.width):1;
    heightRatio = (mapState.height)?(height/(F32)mapState.height):1;
    mapState.zoom = (widthRatio<heightRatio)?widthRatio:heightRatio;
    mapState.xp = (-mapState.width/2.0)*mapState.zoom + width/2.0;//*mapState.zoom;
    mapState.yp = (-mapState.height/2.0)*mapState.zoom + height/2.0;

    //if(!header->mapFloors || !header->mapFloors[section])
    //{
    temp = StructAllocRaw(sizeof(ArchitectMapSubMap));
    eaInsert(&header->mapFloors, temp, section);
    //}
    automap_fillHeaderSectionType(header->mapFloors[section], filename, ARCHITECT_MAP_TYPE_FLOOR, widthRatio, heightRatio, isOutdoorMap);
    //if(!header->mapItems || !header->mapItems[section])
    //{
    temp = StructAllocRaw(sizeof(ArchitectMapSubMap));
    eaInsert(&header->mapItems, temp, section);
    //}
    automap_fillHeaderSectionType(header->mapItems[section], filename, ARCHITECT_MAP_TYPE_ITEMS, widthRatio, heightRatio, isOutdoorMap);
    //if(!header->mapSpawns || !header->mapSpawns[section])
    //{
    temp = StructAllocRaw(sizeof(ArchitectMapSubMap));
    eaInsert(&header->mapSpawns, temp, section);
    //}
    automap_fillHeaderSectionType(header->mapSpawns[section], filename, ARCHITECT_MAP_TYPE_SPAWNS, widthRatio, heightRatio, isOutdoorMap);
}

void automap_fillHeader(ArchitectMapHeader * header, char *filename)
{
    int nSections = minimap_getSectioncount();
    int section;
    char *branchIndependentFilename = NULL;
    nSections = nSections?nSections:1;

    if ((branchIndependentFilename = strstri(filename, "maps")) == NULL)        //    start after C:/game/data/ and c:/gamefix/data/ info
    {
        branchIndependentFilename = filename;
    }
    
    if(!header->mapName)
        header->mapName = StructAllocString(branchIndependentFilename);
    else
        StructReallocString(&header->mapName, branchIndependentFilename);

    for(section = 0; section < nSections; section++)
    {
        automap_fillHeaderSection(header, branchIndependentFilename, section, MINIMAP_BASE_DIMENSION, MINIMAP_BASE_DIMENSION);
    }
}

static void s_freeComponent(ArchitectMapComponent *component)
{
    int nPlaces = eaSize(&component->places);
    int i;
    for(i = 0; i < nPlaces;i++)
        StructFree(component->places[i]);
    eaDestroy(&component->places);
    StructFree(component);
}

static void s_freeSubMap(ArchitectMapSubMap *map)
{
    int nComponents = eaSize(&map->components);
    int i;
    for(i = 0; i < nComponents;i++)
        s_freeComponent(map->components[i]);
    eaDestroy(&map->components);
    StructFree(map);
}

static void s_freeHeader(ArchitectMapHeader * header)
{
    int n;
    int i;
    n = eaSize(&header->mapFloors);
    for(i = 0; i < n; i++)
        s_freeSubMap(header->mapFloors[i]);
    eaDestroy(&header->mapFloors);

    n = eaSize(&header->mapItems);
    for(i = 0; i < n; i++)
        s_freeSubMap(header->mapItems[i]);
    eaDestroy(&header->mapItems);

    n = eaSize(&header->mapSpawns);
    for(i = 0; i < n; i++)
        s_freeSubMap(header->mapSpawns[i]);
    eaDestroy(&header->mapSpawns);

    StructFree(header);
}

void minimap_saveHeader(char *filename)
{
    static char headerName[512] = {0};
    ArchitectMapHeader * header = 0;
    header = StructAllocRaw(sizeof(ArchitectMapHeader));
    minimap_mapperInit(1, 1);
    automap_fillHeader(header, filename);
    strcpy(headerName, filename);
    //remove .txt
    {
        char *lastPeriod = strrchr(headerName,'.');
        if(lastPeriod)
            *lastPeriod = 0;
    }
    if(strlen(headerName) + strlen(".minimap") < 512)
    {
        strcpy(headerName+strlen(headerName), ".minimap");
    }
    s_freeHeader(header);
}

#include "autogen/groupMetaMinimap_h_ast.c"
