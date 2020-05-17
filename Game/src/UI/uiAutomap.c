
#include "graphics/textureatlas.h"
#include "graphics/gfx.h"
#include "graphics/gfxwindow.h"
#include "render/thread/rt_state.h"
#include "render/rendermodel.h"
#include "render/renderUtil.h"

#include "UI/uiGame.h"
#include "UI/uiUtil.h"
#include "UI/uiInput.h"
#include "UI/uiStatus.h"
#include "uiSlider.h"
#include "UI/uiCursor.h"
#include "UI/uiWindows.h"
#include "UI/uiCompass.h"
#include "UI/uiAutomap.h"
#include "UI/uiCompass.h"
#include "UI/uiUtilGame.h"
#include "UI/uiGroupWindow.h"
#include "UI/uiTarget.h"
#include "UI/uiOptions.h"
#include "uiHelpButton.h"
#include "UI/uiPet.h"
#include "UI/uiWindows_init.h"
#include "UI/uiInspiration.h"

#include "cmdparse/cmdgame.h"
#include "cmdparse/cmdcommon.h"

#include "group/grouputil.h"
#include "group/groupnetrecv.h"

#include "UI/sprite/sprite_base.h"
#include "UI/sprite/sprite_text.h"
#include "UI/sprite/sprite_font.h"
#include "graphics/ttFontUtil.h"

#include "graphics/font.h"
#include <utilitieslib/utils/utils.h>
#include "graphics/camera.h"
#include <utilitieslib/utils/timing.h>
#include "player/player.h"
#include "graphics/camera.h"
#include <utilitieslib/components/Earray.h>
#include <utilitieslib/utils/textparser.h>
#include "win/win_init.h"
#include "entity/entclient.h"
#include "win/win_cursor.h"
#include "storyarc/contactClient.h"
#include "language/langClientUtil.h"
#include "UI/uiAutoMapFog.h"
#include "entity/entVarUpdate.h"
#include "UI/uiEditText.h"
#include "UI/uiComboBox.h"
#include "UI/uiNet.h"
#include "entity/entity.h"
#include "seq/anim.h"
#include "group/group.h"
#include "group/groupproperties.h"
#include "gameComm/itemselect.h"
#include "group/groupfileload.h"
#include "entity/teamCommon.h"
#include "entity/Supergroup.h"
#include "UI/uiTabControl.h"
#include "bases/bases.h"
#include "bases/basedata.h"
#include <utilitieslib/components/StashTable.h>
#include <utilitieslib/language/MessageStoreUtil.h>
#include "render/tex.h"
#include "graphics/imageCapture.h"
#include <utilitieslib/components/estring.h>
#include "UI/uiClipper.h"
#include "entity/character_target.h"
#include "storyarc/pnpcCommon.h"
#include "storyarc/zowieClient.h"

#include "entity/character_eval.h"
#include "storyarc/missiongeoCommon.h"
#include "UI/uiMissionReview.h"
#include "entity/gametypes.h"
#include "entity/EntPlayer.h"
#include <utilitieslib/utils/sysutil.h>
#include "shlobj.h"
#include <nvdxt_options.h>

#ifndef TEST_CLIENT
#include "group/groupMetaMinimap.h"
#endif
#include "sound/sound.h"
#include "graphics/FX/fx.h"


static void init_map_options();

// This file has exploded into a total CF
//
//------------------------------------------------------------------------------------------------
// constants, globals
//------------------------------------------------------------------------------------------------

#define MAX_STATIC_ICONS 256
#define MAX_ACTIVE_ICONS 256

static ComboBox comboIcons    = {0};
static ComboBox comboHeaders    = {0};
static int opt_wd = 100;    //fun with opt
static int s_showAllMaps = 0;

#define DRAWING_STATIC_ICONS (type == MAP_ZONE || type == MAP_OUTDOOR_MISSION || type == MAP_MISSION_MAP || MAP_OUTDOOR_INDOOR )
#define DRAWING_ACTIVE_ICONS (type == MAP_ZONE || type == MAP_OUTDOOR_MISSION || type == MAP_MISSION_MAP || MAP_OUTDOOR_INDOOR || MAP_BASE)


// Saved copy for zone minimap hack
static MapState zoneMapState;

static Destination staticIcons[MAX_STATIC_ICONS];
static int staticIconCount;
//static GroupDef *zowieIcons[MAX_STATIC_ICONS];
static U32 s_isHeader = 0;
static ArchitectMapHeader * s_curHeader = 0;

//static char last_map[256] = "none";    //nothing looks at this




Destination thumbtack = {0};

Destination activeIcons[MAX_ACTIVE_ICONS] = {0};
int activeIconCount = 0;

char minimap_scenefile[sizeof (group_info.scenefile)];

//static char g_zowie_type[MAX_ZOWIE_NAME];
//static char g_zowie_display_name[MAX_ZOWIE_NAME];
//static U32 g_zowie_status = 0;


//----------------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------
// Map traversal funcs
//----------------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------


int automap_getSectionCount()
{
    return minimap_getSectioncount();
}

void automap_showAllMaps(int on)
{
    s_showAllMaps = on;
}

static void drawItemLocations(int flags, int flip_red)
{
    int i, j;
    F32 x, y, z;
    MapState *mapState = minimap_getMapState();
    MapEncounterSpawn ***spawnList = minimap_getSpawnList();
      window_getDims( WDW_MAP, 0, 0, &z, 0, 0, 0, 0, 0 );

    if( !flags || flags == 1 )
    {
           for( i = 0;  i < eaSize(&g_rooms); i++ )
        {
            RoomInfo *pRoom = g_rooms[i];
            for( j = 0; j < eaSize(&pRoom->items); j++ )
            {
                ItemPosition *pItem = pRoom->items[j];

                if( !withinCurrentSection( pItem->pos, mapState ) )
                    continue;

                // coordinates to draw sprite
                x = mapState->xp + ((mapState->width - (pItem->pos[3][0] - mapState->min[0])))*mapState->zoom;
                y = mapState->yp + (((pItem->pos[3][2] - mapState->min[2])))*mapState->zoom;

                font_color(CLR_WHITE, CLR_WHITE);
                if( pRoom )
                {
                    if( pRoom->startroom )
                        font_color(CLR_GREEN, CLR_GREEN);
                    else if( pRoom->endroom )
                    {
                        if(flip_red)
                            font_color(CLR_BLUE, CLR_BLUE);
                        else
                            font_color(CLR_RED, CLR_RED);
                    }
                }

                 if( pItem->loctype == LOCATION_ITEMWALL )
                       cprntEx( x, y, z, mapState->zoom, mapState->zoom, CENTER_X|CENTER_Y, "\xE2\x96\xB6" ); // This is a right arrow U+25B6
                if( pItem->loctype == LOCATION_ITEMFLOOR )
                    cprntEx( x, y, z, mapState->zoom*.5, mapState->zoom*.5, CENTER_X|CENTER_Y, "\xE2\x96\xBC" ); // This is a down Arrow U+25B7
            }
        }
    }

    if( !flags || flags==2 )
    {
        for( i = 0; i < eaSize(spawnList); i++ )
        {
            //AtlasTex * icon;
            MapEncounterSpawn *pSpawn = (*spawnList)[i];
            F32 sc = mapState->zoom*2.f;

            if( !withinCurrentSection( pSpawn->mat, mapState ) )
                continue;

            // coordinates to draw sprite
            x = mapState->xp + ((mapState->width - (pSpawn->mat [3][0] - mapState->min[0])))*mapState->zoom;
            y = mapState->yp + (((pSpawn->mat [3][2] - mapState->min[2])))*mapState->zoom;

            font_color(CLR_WHITE, CLR_WHITE);

            if( pSpawn->start )
            {
                font_color(CLR_GREEN, CLR_GREEN);
            }
            else if( pSpawn->end)
            {
                if(flip_red)
                    font_color(CLR_BLUE, CLR_BLUE);
                else
                    font_color(CLR_RED, CLR_RED);
            }

               if( pSpawn->type == 1 )
                 cprntEx( x, y, z+1, sc, sc, CENTER_X|CENTER_Y, "X" );     
            if( pSpawn->type == 2 )
                 cprntEx( x, y, z+1, sc, sc, CENTER_X|CENTER_Y, "O" );
               if( pSpawn->type == 3 )
                cprntEx( x, y, z+1, sc*1.7, sc*1.7, CENTER_X|CENTER_Y, "\xE2\x97\x8f" ); // This is a circle U+25CF
        }
    }
}


static int drawMapTilesImage(GroupDef *def,Mat4 mat)
{
    int zonemap = (def->model && strnicmp(def->model->name,"map_",4)==0);
    float smeg, z;
    int blah;
    PropertyEnt* typeProp = stashFindPointerReturnPointer(def->properties, "MinimapTextureName" );
    MapState *mapState = minimap_getMapState();

    window_getDims( WDW_MAP, &smeg, &smeg, &z, &smeg, &smeg, &smeg, &blah, &blah );

    if (def->is_tray || zonemap || typeProp )
    {
        if (zonemap)
        {
            AtlasTex * name = atlasLoadTexture( def->model->name );
            int x = mapState->xp + (mat[3][0] - mapState->min[0])/(mapState->max[0] - mapState->min[0])*mapState->zoom - name->width/2*mapState->zoom;
            int y = mapState->xp + (mat[3][0] - mapState->min[0])/(mapState->max[0] - mapState->min[0])*mapState->zoom - name->height/2*mapState->zoom;

            display_sprite( name, x, y, z, mapState->zoom, mapState->zoom, DEFAULT_RGBA );
        }
        else
        {
            AtlasTex * name;
            const char *name_ptr;
            int x, y;
            Vec3 min,max;
            float angle;
            int color = CLR_WHITE;

            // grab the location
            mulVecMat4(def->min,mat,min);
            mulVecMat4(def->max,mat,max);

            if( !withinCurrentSection( mat, mapState ) )
                return 0;

            // get the sprite (ignore leading underscores)
            if( typeProp && typeProp->value_str )
                name_ptr = typeProp->value_str;
            else if( def->name[0] == '_' )
                name_ptr = &def->name[1];
            else
                name_ptr = def->name;

            name = atlasLoadTexture( name_ptr );

            // coordinates to draw sprite
            x = mapState->xp + ((mapState->width - (mat[3][0] - mapState->min[0])) - name->width)*mapState->zoom;
            y = mapState->yp + (((mat[3][2] - mapState->min[2])) - name->height)*mapState->zoom;

            // rotation

            angle = acos(mat[0][0]);
            if( mat[0][2] > 0 )
                angle = -angle;

            // finally draw it
            if( stricmp( name->name, "white" ) != 0 )
                display_rotated_sprite( name,x - name->width*2*mapState->zoom*.01, y - name->height*2*mapState->zoom*.01, z, 2*mapState->zoom*1.02, 2*mapState->zoom*1.02, color, angle, 0 );
        }
        return 0;
    }
    else
        blah += 0;
    if (!def->open_tracker || !def->child_ambient)
        return 0;
    return 1;
}


// Draw misssion maps
//
static int drawMapTiles(GroupDef *def,Mat4 mat)
{
    int zonemap = (def->model && strnicmp(def->model->name,"map_",4)==0);
    float smeg, z;
    int blah;
    PropertyEnt* typeProp = stashFindPointerReturnPointer(def->properties, "MinimapTextureName" );
    MapState *mapState = minimap_getMapState();

    window_getDims( WDW_MAP, &smeg, &smeg, &z, &smeg, &smeg, &smeg, &blah, &blah );

    if (def->is_tray || zonemap || typeProp )
    {
        if (zonemap)
        {
            AtlasTex * name = atlasLoadTexture( def->model->name );
            int x = mapState->xp + (mat[3][0] - mapState->min[0])/(mapState->max[0] - mapState->min[0])*mapState->zoom - name->width/2*mapState->zoom;
            int y = mapState->xp + (mat[3][0] - mapState->min[0])/(mapState->max[0] - mapState->min[0])*mapState->zoom - name->height/2*mapState->zoom;
            display_sprite( name, x, y, z, mapState->zoom, mapState->zoom, DEFAULT_RGBA );
        }
        else
        {
            AtlasTex * name;
            const char *name_ptr;
            int x, y;
            Vec3 min,max;
            float angle;
            //RoomInfo * pRoom;
            int color = CLR_WHITE;

            // grab the location
            mulVecMat4(def->min,mat,min);
            mulVecMat4(def->max,mat,max);

            if( !withinCurrentSection( mat, mapState ) )
                return 0;

            // potential crash during map transiton; wasn't being used anyway
         //    pRoom = RoomFindClosest( mat[3] );
         //    if( pRoom )
        //    {
           //        if( pRoom->roompace == 1 )
        //            color = CLR_GREY;
        //        if( pRoom->startroom )
        //            color = CLR_GREEN;
        //        else if( pRoom->endroom )
        //            color = CLR_RED;
        //    }

            // get the sprite (ignore leading underscores)
            if( typeProp && typeProp->value_str )
                name_ptr = typeProp->value_str;
            else if( def->name[0] == '_' )
                name_ptr = &def->name[1];
            else
                name_ptr = def->name;

            name = atlasLoadTexture( name_ptr );

            // coordinates to draw sprite
            x = mapState->xp + ((mapState->width - (mat[3][0] - mapState->min[0])) - name->width)*mapState->zoom;
            y = mapState->yp + (((mat[3][2] - mapState->min[2])) - name->height)*mapState->zoom;

            // rotation

            angle = acos(mat[0][0]);
            if( mat[0][2] > 0 )
                angle = -angle;

            // finally draw it
            if( stricmp( name->name, "white" ) != 0)
                display_rotated_sprite( name,x - name->width*2*mapState->zoom*.01, y - name->height*2*mapState->zoom*.01, z, 2*mapState->zoom*1.02, 2*mapState->zoom*1.02, color, angle, 0 );
        }
        return 0;
    }
    if (!def->open_tracker || !def->child_ambient)
        return 0;
    return 1;
}

void automap_drawMap(int height, int width, int floor, int flags)
{
    char filename[256], tmp[256];
    F32 widthRatio;// = width/(F32)mapState.width;
    F32 heightRatio;// = height/(F32)mapState.height;
    int section = floor; 
    MapState *mapState = minimap_getMapState();

    Extent *missionSection = minimap_getSections();

    {
        //get mapState data for this floor
        copyVec3(missionSection[section].min, mapState->min);
        copyVec3(missionSection[section].max, mapState->max);
        mapState->width = mapState->max[0] - mapState->min[0];
        mapState->height = mapState->max[2] - mapState->min[2];
    }

    widthRatio = width/(F32)mapState->width;
    heightRatio = height/(F32)mapState->height;
    mapState->zoom = (widthRatio<heightRatio)?widthRatio:heightRatio;
    mapState->xp = (-mapState->width/2.0)*mapState->zoom + width/2.0;//*mapState.zoom;
    mapState->yp = (-mapState->height/2.0)*mapState->zoom + height/2.0;

    //get the extents
    clipperPush(0);

    if( flags )
    {
         AtlasTex * white = atlasLoadTexture("white");
        display_sprite( white, 0, 0, 0, ((F32)width)/white->width,  ((F32)height)/white->height, 0xff0000ff );
        drawItemLocations(flags, 1);
    }
    else
    {
        //draw on map
        if(mapState->mission_type != MAP_MISSION_MAP && mapState->mission_type != MAP_BASE )
        {
            AtlasTex * mapImage;
            strcpy_s(SAFESTR(filename), getFileName(gMapName));
            sprintf( tmp, "map_%s", strtok(filename,".") );
            mapImage = atlasLoadTexture( tmp );
            display_sprite( mapImage, 0, 0, 0,width/(F32)mapImage->width, height/(F32)mapImage->height, DEFAULT_RGBA );
        }
        else
        {
            groupProcessDef(&group_info, drawMapTilesImage );
        }
    }
    clipperPop();
}

static int positionEdge( BaseRoom * room, int x, int y )
{
    int i;
    int idx = x + room->size[0]*y;

    if( x < 0 || x >= room->size[0] || 
        y < 0 || y >= room->size[1] ||
        idx >= eaSize(&room->blocks) )
    {
        //outside room, is it door?
        for( i = eaSize(&room->doors)-1; i>= 0; i-- )
        {
            if( room->doors[i]->pos[0] == x && room->doors[i]->pos[1] == y )
                return false;
        }
        return true;
    }
    else
    {
        if( room->blocks[idx]->height[0] >= room->blocks[idx]->height[1] )
            return true;
    }

    return false;
}

static void blockDrawLineEdges( BaseRoom * room, int idx, CBox * box, float z )
{
    int above = 0, below = 0, left = 0, right = 0;
    int size = room->size[0];
    int x = (idx % room->size[0]);
    int y = (idx / room->size[0]);

    if( room->blocks[idx]->height[0] >= room->blocks[idx]->height[1] )
        return;

    // left
    above = positionEdge( room, x, y-1 );
    below = positionEdge( room, x, y+1 );
    right = positionEdge( room, x-1, y );
    left  = positionEdge( room, x+1, y );

    drawBoxEx( box, z, CLR_WHITE, 0, above, right, below, left );
}

static void doorDrawLineEdges( BaseRoom * room, int idx, CBox * box, float z )
{
    if( room->doors[idx]->pos[0] < 0 || room->doors[idx]->pos[0] >= room->size[0] )
    {
        //drawBoxEx( box, z, CLR_WHITE, 0, 1, 0, 1, 0 );
        drawBoxEx( box, z, 0xffffff88, 0, 0, 1, 0, 1 );
    }
    else
    {
        //drawBoxEx( box, z, CLR_WHITE, 0, 0, 1, 0, 1 );
        drawBoxEx( box, z, 0xffffff88, 0, 1, 0, 1, 0 );
    }
}

void mapDrawBaseRooms()
{
    int i, j;
    float smeg, z, fGrid = g_base.roomBlockSize;
    int blah;
    AtlasTex *white = atlasLoadTexture( "white" );
    CBox box;
    MapState *mapState = minimap_getMapState();

    window_getDims( WDW_MAP, &smeg, &smeg, &z, &smeg, &smeg, &smeg, &blah, &blah );
    display_sprite( white, mapState->xp, mapState->yp, z, ((float)mapState->width/white->width)*mapState->zoom, ((float)mapState->height/white->height)*mapState->zoom, 0x1111aaff );
    BuildCBox( &box, mapState->xp + 8*mapState->zoom, mapState->yp + 8*mapState->zoom, ((float)mapState->width - 17)*mapState->zoom, ((float)mapState->height - 16)*mapState->zoom );
    drawBox( &box, z, CLR_WHITE, 0 );

    for( i = eaSize(&g_base.rooms)-1; i >= 0; i-- )
    {
        Vec3 pos;
        BaseRoom * room = g_base.rooms[i];
        float x, y, wd, ht;


        copyVec3( g_base.rooms[i]->pos, pos );
        wd = room->size[0] * fGrid;
        ht = room->size[1] * fGrid;

        // coordinates to draw sprite
        x = mapState->xp + (mapState->width - (pos[0] - mapState->min[0]) - wd)*mapState->zoom;
        y = mapState->yp + ((pos[2] - mapState->min[2]))*mapState->zoom;

        display_sprite( white, x, y, z, (wd/white->width)*mapState->zoom, (ht/white->height)*mapState->zoom, 0xffffff88 );

        wd = ht = fGrid;

        for( j = eaSize(&room->blocks)-1; j >= 0; j-- )
        {
            if(  room->blocks[j]->height[1] <= room->blocks[j]->height[0])
            {
                pos[0] = room->pos[0] + (j % room->size[0])*fGrid;
                pos[2] = room->pos[2] + (j / room->size[0])*fGrid;

                // coordinates to draw sprite
                x = mapState->xp + (mapState->width - (pos[0] - mapState->min[0]) - wd)*mapState->zoom;
                y = mapState->yp + ((pos[2] - mapState->min[2]))*mapState->zoom;

                display_sprite( white, x, y, z, (wd/white->width)*mapState->zoom, (ht/white->height)*mapState->zoom, 0x1111aaff );
            }
        }

        for( j = eaSize(&room->blocks)-1; j >= 0; j-- )
        {
            pos[0] = room->pos[0] + (j % room->size[0])*fGrid;
            pos[2] = room->pos[2] + (j / room->size[0])*fGrid;

            // coordinates to draw sprite
            x = mapState->xp + (mapState->width - (pos[0] - mapState->min[0]) - wd)*mapState->zoom;
            y = mapState->yp + ((pos[2] - mapState->min[2]))*mapState->zoom;

            BuildCBox( &box, x, y, fGrid*mapState->zoom, fGrid*mapState->zoom );
            blockDrawLineEdges( room, j, &box, z );
        }

        for( j = eaSize(&room->doors)-1; j >= 0; j-- )
        {
            pos[0] = room->pos[0] + room->doors[j]->pos[0]*fGrid;
            pos[2] = room->pos[2] + room->doors[j]->pos[1]*fGrid;
            // coordinates to draw sprite
            x = mapState->xp + (mapState->width - (pos[0] - mapState->min[0]) - wd)*mapState->zoom;
            y = mapState->yp + ((pos[2] - mapState->min[2]))*mapState->zoom;

            display_sprite( white, x, y, z, (wd/white->width)*mapState->zoom, (ht/white->height)*mapState->zoom, 0xffffff33 );
            BuildCBox( &box, x, y, fGrid*mapState->zoom, fGrid*mapState->zoom );
            doorDrawLineEdges( room, j, &box, z );
        }
    }
}

static void doorDrawLineEdgesForRaid( BaseRoom * room, int idx, CBox * box, float z)
{
    if( room->doors[idx]->pos[0] < 0 || room->doors[idx]->pos[0] >= room->size[0] )
    {
        drawBoxEx( box, z, CLR_WHITE, 0, 1, 0, 1, 0 );
    }
    else
    {
        drawBoxEx( box, z, CLR_WHITE, 0, 0, 1, 0, 1 );
    }
}

// This is going to go *COMPLETELY* to hell when we allow more than two bases on a map.
// Don't say I didn't warn you .....
//#define        RAID_MAP_COLOR_FRIENDLY        0x77ffffff
//#define        RAID_MAP_COLOR_HOSTILE        0xff6666ff
//#define        RAID_MAP_COLOR_NEUTRAL        0xaaaaaaff

#define        RAID_MAP_COLOR_BLUE_TEAM    0x44ccffff
#define        RAID_MAP_COLOR_RED_TEAM        0xff4444ff
#define        RAID_MAP_COLOR_NEUTRAL        0x999999ff

void mapDrawRaidRooms()
{
    int i, j, k;
    float smeg, z, fGrid = g_base.roomBlockSize;
    int blah;
    AtlasTex *white = atlasLoadTexture("white");
    CBox box;
    Base *base;
    int raidRoomColors[3];
    MapState *mapState = minimap_getMapState();

    // Duh.  We don't swap the colors anyway, since we *ARE* using a red team / blue team motif.
    //    if (sgRaidGetTeamNumber(playerPtr()) == 0)
    //    {
    //        raidRoomColors[0] = RAID_MAP_COLOR_FRIENDLY;
    //        raidRoomColors[1] = RAID_MAP_COLOR_HOSTILE;
    //        raidRoomColors[2] = RAID_MAP_COLOR_NEUTRAL;
    //    }
    //    else
    //    {
    //        raidRoomColors[1] = RAID_MAP_COLOR_FRIENDLY;
    //        raidRoomColors[0] = RAID_MAP_COLOR_HOSTILE;
    //        raidRoomColors[2] = RAID_MAP_COLOR_NEUTRAL;
    //    }
    raidRoomColors[0] = RAID_MAP_COLOR_BLUE_TEAM;
    raidRoomColors[1] = RAID_MAP_COLOR_RED_TEAM;
    raidRoomColors[2] = RAID_MAP_COLOR_NEUTRAL;

    window_getDims(WDW_MAP, &smeg, &smeg, &z, &smeg, &smeg, &smeg, &blah, &blah);
    display_sprite(white, mapState->xp, mapState->yp, z, ((float) mapState->width / white->width) * mapState->zoom, ((float) mapState->height / white->height) * mapState->zoom, 0x333333ff);
    BuildCBox(&box, mapState->xp + 8 * mapState->zoom, mapState->yp + 8 * mapState->zoom, ((float) mapState->width - 17) * mapState->zoom, ((float) mapState->height - 16) * mapState->zoom);
    drawBox(&box, z, CLR_WHITE, 0);

    for (i = eaSize(&g_raidbases) - 1; i >= 0; i--)
    {
        base = g_raidbases[i];
        for (j = eaSize(&base->rooms) - 1; j >= 0; j--)
        {
            Vec3 pos;
            Vec3 roomPos;
            BaseRoom * room = base->rooms[j];
            float x, y, wd, ht;

            addVec3(base->ref->mat[3], room->pos, roomPos);
            copyVec3(roomPos, pos);
            wd = room->size[0] * fGrid;
            ht = room->size[1] * fGrid;

            // coordinates to draw sprite
            x =    mapState->xp    + (mapState->width -    (pos[0]    - mapState->min[0]) - wd) * mapState->zoom;
            y =    mapState->yp    + ((pos[2] - mapState->min[2])) * mapState->zoom;

            display_sprite(white, x, y, z, (wd / white->width) * mapState->zoom, (ht / white->height) * mapState->zoom, raidRoomColors[i]);

            wd = ht = fGrid;

            for (k = eaSize(&room->blocks) - 1; k >= 0; k--)
            {
                if (room->blocks[k]->height[1] <= room->blocks[k]->height[0])
                {
                    pos[0] = roomPos[0] + (k % room->size[0]) * fGrid;
                    pos[2] = roomPos[2] + (k / room->size[0]) * fGrid;

                    // coordinates to draw sprite
                    x = mapState->xp + (mapState->width - (pos[0] - mapState->min[0]) - wd) * mapState->zoom;
                    y =    mapState->yp + ((pos[2] - mapState->min[2])) * mapState->zoom;

                    display_sprite(white, x, y, z, (wd / white->width) * mapState->zoom, (ht / white->height) * mapState->zoom, 0x333333ff);
                }
            }

            for (k = eaSize(&room->blocks) - 1; k >= 0; k-- )
            {
                pos[0] = roomPos[0] + (k % room->size[0]) * fGrid;
                pos[2] = roomPos[2] + (k / room->size[0]) * fGrid;

                // coordinates to draw sprite
                x = mapState->xp + (mapState->width - (pos[0] - mapState->min[0]) - wd) * mapState->zoom;
                y = mapState->yp + ((pos[2] - mapState->min[2])) * mapState->zoom;

                BuildCBox(&box, x, y, fGrid * mapState->zoom, fGrid * mapState->zoom);
                blockDrawLineEdges(room, k, &box, z);
            }

            for (k = eaSize(&room->doors) - 1; k >= 0; k--)
            {
                pos[0] = roomPos[0] +    room->doors[k]->pos[0] * fGrid;
                pos[2] = roomPos[2] +    room->doors[k]->pos[1] * fGrid;

                // coordinates to draw sprite
                x = mapState->xp + (mapState->width - (pos[0] - mapState->min[0]) - wd) * mapState->zoom;
                y = mapState->yp + ((pos[2] - mapState->min[2])) * mapState->zoom;

                display_sprite(white, x, y, z, (wd / white->width) * mapState->zoom, (ht / white->height) * mapState->zoom, raidRoomColors[i]);
                BuildCBox(&box, x, y, fGrid*mapState->zoom, fGrid*mapState->zoom);
                doorDrawLineEdgesForRaid(room, k, &box, z);
            }
        }
    }
}

static int icon_name_to_number(char *name)
{
    if( stricmp( "Gate", name) == 0 )
        return ICON_GATE;
    if( stricmp( "Contact", name) == 0 )
        return ICON_CONTACT;
    if( stricmp( "Neighborhood", name) == 0 )
        return ICON_NEIGHBORHOOD;
    if( stricmp( "Hospital", name) == 0 )
        return ICON_HOSPITAL;
    if( stricmp( "Architect", name) == 0 )
        return ICON_ARCHITECT;
    if( stricmp( "Mission", name) == 0 )
        return ICON_MISSION;
    if( stricmp( "Store", name) == 0 )
        return ICON_STORE;
    if( stricmp( "Thumbtack", name) == 0 )
        return ICON_THUMBTACK;
    if( stricmp( "TrainStation", name) == 0 )
        return ICON_TRAIN_STATION;

    return ICON_GATE;
}


struct minimap_item
{
    char *name;
    int type;
    char *texture_name;
    int dir_matters;
};

struct minimap_item minimap_items[] = 
{
    {"TrainStation",    ICON_TRAIN_STATION, "map_enticon_train", 0 },
    {"SewerZone",        ICON_SEWER,            "map_enticon_gate_arrow_hazard_sewer", 1 },
    {"DoorIcon",        ICON_GATE,            "map_enticon_gate_entrysafe", 1 },
    {"Neighborhood",    ICON_NEIGHBORHOOD,    "map_enticon_neighborhood", 0},
    {"Contact_Trainer", ICON_TRAINER,        "map_enticon_trainer", 0},
    {"Contact_Contact",    ICON_CONTACT,        "map_enticon_contact", 0},
    {"Contact_Contact_Has_New_Mission",        ICON_CONTACT_HAS_NEW_MISSION,    "map_mission_available", 0},
    {"Contact_Contact_On_Mission",            ICON_CONTACT_ON_MISSION,        "map_mission_in_progress", 0},
    {"Contact_Contact_Completed_Mission",    ICON_CONTACT_COMPLETED_MISSION,    "map_mission_completed", 0},
    {"Contact_Signature_Story",    ICON_CONTACT_SIGNATURE_STORY,    "map_signature_story", 0},
    {"Contact_Task_Force",        ICON_CONTACT_TASK_FORCE,        "map_mission_available", 0},
    {"FerryStation",    ICON_GATE,            "map_enticon_ferry", 0},
    {"Arena",            ICON_ARENA,            "map_enticon_arena", 0},
    {"Store",            ICON_STORE,            "map_enticon_store", 0},
    {"Tailor",            ICON_STORE,            "map_enticon_tailor", 0},
    {"StoreMutant",        ICON_STORE,            "map_enticon_store_mutant", 0},
    {"StoreMagic",        ICON_STORE,            "map_enticon_store_magic", 0},
    {"StoreScience",    ICON_STORE,            "map_enticon_store_science", 0},
    {"StoreTech",        ICON_STORE,            "map_enticon_store_tech", 0},
    {"StoreNatural",    ICON_STORE,            "map_enticon_store_natural", 0},
    {"Architect",        ICON_ARCHITECT,        "map_enticon_architect", 0},
    {"BasePortal",        ICON_GATE,            "map_enticon_port", 0},
    {"BBMeteorIcon",    ICON_NEIGHBORHOOD,    "map_enticon_code_fullB", 0},
    {"University",        ICON_NEIGHBORHOOD,    "map_enticon_university", 0},
    {"Wentworth",        ICON_STORE,            "map_enticon_wentworth", 0},
    {"BlackMarket",        ICON_STORE,            "map_enticon_blackmarket", 0},
    {"Contact_Notoriety", ICON_NOTORIETY,    "map_enticon_notoriety", 0},
    {"VanguardTeleport", ICON_GATE,            "map_enticon_vanguard_teleport", 0},
    {"LoyalistLounge",    ICON_LOUNGE,        "map_enticon_loyalist_lounge", 0},
    {"LoyalistIcon",    ICON_GATE,            "map_enticon_loyalist", 0},
    {"Precinct04",        ICON_PRECINCT,        "map_enticon_precinct_04", 0},
    {"Precinct05",        ICON_PRECINCT,        "map_enticon_precinct_05", 0},
    {"Precinct07",        ICON_PRECINCT,        "map_enticon_precinct_07", 0},
    {"Precinct11",        ICON_PRECINCT,        "map_enticon_precinct_11", 0},
    {"ResistanceIcon",    ICON_GATE,            "map_enticon_resistance", 0},
    {"ResistanceHub",    ICON_LOUNGE,        "map_enticon_resistancehub", 0},
    {"TradingHouse",    ICON_STORE,            "map_enticon_tradinghouse", 0},
    {"RiftIcon",        ICON_GATE,            "map_enticon_rift", 0},
    {"SmugglerLine",    ICON_GATE,            "map_enticon_smugglerline", 0}
};

int find_name_in_items(char *name) {
    int j;
    
    for(j=0; j<ARRAY_SIZE(minimap_items); j++)
    {
        if(stricmp(name, minimap_items[j].name) == 0) {
            return j;
        }
    }
    return -1;
}


// Loads static icons from the map file
//
static int miniMapStaticIconLoader(GroupDefTraverser* traverser)
{
    PropertyEnt* typeProp;
    PropertyEnt* iconProp;
    PropertyEnt* specialProp;
    PropertyEnt* hiddenProp;
    PropertyEnt* requiresProp;
    PropertyEnt* dirProp;
    PropertyEnt* categoryProp;

    int j;

    int dir_matters = 0;

    // find the group
    if (!traverser->def->properties)
        return (traverser->def->child_properties != 0);

    //GotoMap, GotoSpawn, and of course, miniMapLocation show on the minimap.
    stashFindPointer( traverser->def->properties, "miniMapLocation", &typeProp );

    // If not a minimaplocation, maybe a gotomap?
    if( !typeProp ) 
        stashFindPointer( traverser->def->properties, "GotoMap" , &typeProp );

    // if not either of those, then maybe one of the special gotospawns?
    if( !typeProp )
    {
        stashFindPointer( traverser->def->properties, "GotoSpawn" , &typeProp );

        if( typeProp )
        {
            if( strStartsWith(typeProp->value_str, "LinkFrom_Monorail") )
            {
                staticIcons[staticIconCount].icon = atlasLoadTexture( "map_enticon_train");
            }
            else if( strstri( typeProp->value_str, "LinkFrom_Ferry" )  ) // CoV ferry
            {
                staticIcons[staticIconCount].icon = atlasLoadTexture( "map_enticon_ferry");
            }
            else if( strstri( typeProp->value_str, "LinkFrom_Helicopter" ) ) // CoV Helicopter
            {
                staticIcons[staticIconCount].icon = atlasLoadTexture( "map_enticon_helipad");
            }
            else
            {
                typeProp = 0;
            }
        }
    }

    //if( !typeProp ) 
    //    stashFindPointer( traverser->def->properties, "Zowie" , &typeProp );

    // if this isn't something drawn on the minimap, we're not interested
    if( typeProp == 0) 
        return (traverser->def->child_properties != 0);

    // we found a minimap location, but maybe they didn't want it to be drawn.
    stashFindPointer( traverser->def->properties, "specialMapIcon", &specialProp );
    if( specialProp && ( stricmp( "DontDrawOnMiniMap", specialProp->value_str ) == 0 ) )
        return 0;

    //I think this is a bug waiting to be reported, but it's from the original.
    if( stricmp( "TrainStation", typeProp->value_str) == 0 )
        return 0;

    // start with some defaults
    staticIcons[staticIconCount].color = CLR_WHITE;
    staticIcons[staticIconCount].type = ICON_GATE;
    staticIcons[staticIconCount].iconB = 0;
    dir_matters = 1;
    //zowieIcons[staticIconCount] = NULL;

    //Check for a few special cases...
    if( stricmp( "Hospital", typeProp->value_str) == 0 )
    {
        staticIcons[staticIconCount].type = ICON_HOSPITAL;
        dir_matters = 0;
    }
    else if( strstri( typeProp->value_str, "Hazard" ) || strstri( typeProp->value_str, "Trial" ) )
    {
        staticIcons[staticIconCount].icon = atlasLoadTexture( "map_enticon_gate_entrydanger" );

        if( game_state.skin != UISKIN_VILLAINS )
            staticIcons[staticIconCount].iconB = atlasLoadTexture( "map_enticon_gate_wallhazard" );
    }
    else if( strstri( typeProp->value_str, "PvP" ) )
    {
        staticIcons[staticIconCount].icon = atlasLoadTexture( "map_enticon_gate_entrydanger" );

        if( game_state.skin != UISKIN_VILLAINS )
            staticIcons[staticIconCount].iconB = atlasLoadTexture( "map_enticon_gate_wallpvp" );
    }
#if 0
    else if( stricmp( typeProp->name_str, "Zowie" ) == 0)
    {
        staticIcons[staticIconCount].type = ICON_ZOWIE;
        staticIcons[staticIconCount].icon = atlasLoadTexture( "map_enticon_neighborhood" );// for now.
        staticIcons[staticIconCount].color = CLR_GREEN;    //for now
        zowieIcons[staticIconCount] = traverser->def;
        dir_matters = 0;
    }
#endif
    else
    {
        staticIcons[staticIconCount].icon = atlasLoadTexture( "map_enticon_gate_entrysafe" );

        if( game_state.skin != UISKIN_VILLAINS )
            staticIcons[staticIconCount].iconB = atlasLoadTexture( "map_enticon_gate_wallsafe" );
    }

    if( specialProp )
    {
        staticIcons[staticIconCount].iconB = 0;
        dir_matters = 0;

        j = find_name_in_items(specialProp->value_str);
        if (j >= 0) {
            staticIcons[staticIconCount].type = minimap_items[j].type;
            staticIcons[staticIconCount].icon = atlasLoadTexture(minimap_items[j].texture_name);
            dir_matters = minimap_items[j].dir_matters;
        }
    }

    if (DestinationIsAContact(&staticIcons[staticIconCount]) ||
        staticIcons[staticIconCount].type == ICON_STORE)
    {
        PropertyEnt *generatorProp;
        char filename[MAX_PATH];
        char *pos;

        stashFindPointer( traverser->def->properties, "PersistentNPC", &generatorProp );

        if (generatorProp)
        {
            // BEGIN saUtilCleanFileName(filename, generatorProp->value_str);
#define SCRIPTS_DIR_SUBSTR "SCRIPTS.LOC/"
            strcpy(filename, generatorProp->value_str);
            forwardSlashes(_strupr(filename));
            pos = strstr(filename, SCRIPTS_DIR_SUBSTR);
            if (pos)
            {
                pos += strlen(SCRIPTS_DIR_SUBSTR);
                memmove(filename, pos, strlen(pos)+1);
            }
#undef SCRIPTS_DIR_SUBSTR
            // END saUtilCleanFileName(filename, generatorProp->value_str);

            staticIcons[staticIconCount].destPNPC = PNPCFindDefFromFilename(filename);
            if (staticIcons[staticIconCount].destPNPC)
            {
                staticIcons[staticIconCount].destEntity = PNPCFindEntityOnClient(staticIcons[staticIconCount].destPNPC);
            }
        }
    }
    
// ADD_ME
    staticIcons[staticIconCount].id = staticIconCount;

    stashFindPointer( traverser->def->properties, "hiddenInFog", &hiddenProp );
    if(hiddenProp)
        staticIcons[staticIconCount].bHiddenInFog = true;

    stashFindPointer( traverser->def->properties, "Requires", &requiresProp );
    if (requiresProp) 
        staticIcons[staticIconCount].required = strdup(requiresProp->value_str);
    else 
        staticIcons[staticIconCount].required = 0;

    strcpy( staticIcons[staticIconCount].name, typeProp->value_str );
    strcpy( staticIcons[staticIconCount].mapName, gMapName );

    // set location
    copyMat4(traverser->mat, staticIcons[staticIconCount].world_location);


    // get any special texture for the icon
    stashFindPointer( traverser->def->properties, "icon", &iconProp );
    if (!iconProp)
        stashFindPointer( traverser->def->properties, "customMissionIcon", &iconProp );
    if (!iconProp)
        stashFindPointer( traverser->def->properties, "customStaticIcon", &iconProp );

    if( iconProp && iconProp->value_str)
    {
        staticIcons[staticIconCount].icon = atlasLoadTexture( iconProp->value_str );
        dir_matters = 0;
    }


    // Check for custom directionMatters (mostly for custom mission icons)
    stashFindPointer( traverser->def->properties, "directionMatters", &dirProp );
    if( dirProp && dirProp->value_str )
        dir_matters = atoi(dirProp->value_str);

    // set the angle if we are paying attention to directions, otherwise don't
    if( !dir_matters )
        staticIcons[staticIconCount].angle = 0;
    else
    {
        staticIcons[staticIconCount].angle = acos( traverser->mat[0][0] );
        if( traverser->mat[0][2] > 0 )
            staticIcons[staticIconCount].angle = -staticIcons[staticIconCount].angle;
    }

    // Check for custom Icon Type (for completeness, not actually used by any map currently)
    stashFindPointer( traverser->def->properties, "iconCategory", &categoryProp );
    if( categoryProp && categoryProp->value_str )
        staticIcons[staticIconCount].type = icon_name_to_number(categoryProp->value_str);

    estrPrintCharString(&staticIcons[staticIconCount].pchSourceFile, traverser->def->file->fullname);

    staticIconCount++;

    return 0;
}

static void clearDestinationIfMapChanges(Destination * dest)
{
    if( dest->mapName && stricmp( gMapName, dest->mapName ) != 0 )
    {
        clearDestination( dest );
    }
}

// initializes static map icons
//
static void clearStaticIcons()
{
    int i;

    for (i=0; i<staticIconCount; i++)
    {
        if (staticIcons[i].required)
        {
            free(staticIcons[i].required);
            staticIcons[i].required = 0;
        }
        clearDestination(&staticIcons[i]);
    }
    staticIconCount = 0;
}

// noop stubs for tree traversal
void* noopContextCreate()
{
    return NULL;
}

void noopContextCopy(void** src, void** dst)
{
    *dst = *src;
}
void noopContextDestroy(void* context) { }

// loads the mini map
void miniMapLoad()
{
    GroupDefTraverser traverser = {0};

    clearStaticIcons();
    clearDestinationIfMapChanges(&thumbtack);
    clearDestinationIfMapChanges(&waypointDest);
    groupProcessDefExBegin(&traverser, &group_info, miniMapStaticIconLoader);

    processIcons( &staticIcons[0], staticIconCount, FALSE );
    strncpy(minimap_scenefile, group_info.scenefile, sizeof(minimap_scenefile));

    init_map_options();
}

//----------------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------
// Icon functions
//----------------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------

typedef struct RemainderLocations
{
    int type;
    Mat4 mat;
    int found_live_ent;
}RemainderLocations;

RemainderLocations **g_ppRemainderLocations;

void automapClearRemainderIcons(int matchType)
{
    //    if client changed map
    //    clear out the remainder icons
    int i;
    for( i = eaSize(&g_ppRemainderLocations)-1; i>=0; i-- )
    { 
        if ((matchType == -1) || (matchType == g_ppRemainderLocations[i]->type))
        {
            RemainderLocations *pDel = eaRemove(&g_ppRemainderLocations,i);
            SAFE_FREE(pDel);
        }
    }
}
void receiveItemLocations(Packet *pak)
{
    int i;

    int map_id  = pktGetBitsAuto(pak);
    int count = pktGetBitsAuto(pak);
    
    automapClearRemainderIcons(ICON_ITEM);

    if( !count )
    {
        if( waypointDest.type == ICON_ITEM )
            clearDestination( &waypointDest );
    }
    else
    {
        for( i = 0; i < count; i++ )
        {
            RemainderLocations *pLoc = calloc(1, sizeof(RemainderLocations));
            pLoc->type = ICON_ITEM;
            copyMat4( unitmat, pLoc->mat );
            pktGetVec3(pak, pLoc->mat[3]);
            eaPush(&g_ppRemainderLocations, pLoc);
        }
    }
}

void receiveCritterLocations(Packet *pak)
{
    int i;

    int map_id = pktGetBitsAuto(pak);
    int count = pktGetBitsAuto(pak);

    automapClearRemainderIcons(ICON_ENEMY);

    if( !count )
    {
        if( waypointDest.type == ICON_ENEMY )
            clearDestination( &waypointDest );
    }
    else
    {
        for( i = 0; i < count; i++ )
        {
            RemainderLocations *pLoc = calloc(1, sizeof(RemainderLocations));
            pLoc->type = ICON_ENEMY;
            copyMat4( unitmat, pLoc->mat );
            pktGetVec3(pak, pLoc->mat[3]);
            eaPush(&g_ppRemainderLocations, pLoc);
        }
    }
}

// helper function to load icons
//
void createZoneIcon( Destination *iconList, int size, int type, const Mat4 location, float angle, int id, Entity *destEntity )
{
    Entity *e = playerPtr();
    iconList[size].type = type;
    iconList[size].angle = 0;
    iconList[size].dontTranslateName = false;
    copyMat4( location, iconList[size].world_location );

    // if these aren't initialized, drawIcons() will incorrect evaluate vision phasing
    iconList[size].destEntity = destEntity;
    iconList[size].destPNPC = NULL;

    if( type == ICON_PLAYER )
    {
        if(destEntity)
        {
            iconList[size].dontTranslateName = true;
            strncpyt( iconList[size].name, destEntity->name, 256 );
        }
        else
            strncpyt( iconList[size].name, "????", 256 );
        iconList[size].icon = atlasLoadTexture( "map_enticon_you_b" );
        iconList[size].iconB = NULL;
        iconList[size].id = destEntity->db_id;
        strcpy(iconList[size].mapName, gMapName);
        iconList[size].color = CLR_GREEN;
    }
    else if( type == ICON_TEAMMATE )
    {
        iconList[size].dontTranslateName = true;
        strncpyt(iconList[size].name, e->teamup->members.names[id], 256);
        iconList[size].icon  = atlasLoadTexture( "map_enticon_you_b" );
        iconList[size].iconB = NULL;
        iconList[size].id = e->teamup->members.ids[id];
        strcpy(iconList[size].mapName, gMapName);
        iconList[size].color = CLR_GREEN;
    }
    else if( type == ICON_LEAGUEMATE )
    {
        iconList[size].dontTranslateName = true;
        strncpyt(iconList[size].name, e->league->members.names[id], 256);
        iconList[size].icon  = atlasLoadTexture( "map_enticon_you_b" );
        iconList[size].iconB = NULL;
        iconList[size].id = e->league->members.ids[id];
        strcpy(iconList[size].mapName, gMapName);
        iconList[size].color = CLR_GREEN;
    }
    else if( type == ICON_FOLLOWER )
    {
        if(destEntity)
        {
            iconList[size].dontTranslateName = true;
            strncpyt( iconList[size].name, destEntity->name, 256 );
        }
        else
            strncpyt( iconList[size].name, "????", 256 );
        iconList[size].icon  = atlasLoadTexture( "map_enticon_you_b" );
        iconList[size].iconB = NULL;
        iconList[size].id = id;
        strcpy(iconList[size].mapName, gMapName);
        iconList[size].color = CLR_GREEN;
    }
     else if( type == ICON_ENEMY && game_state.mission_map )
    {
        Entity *e = entities[id];
        if(e)
            strncpyt( iconList[size].name, e->name, 256 );
        else
            strncpyt( iconList[size].name, "????", 256 );
        iconList[size].destEntity = e;
        iconList[size].icon = atlasLoadTexture( "map_enticon_you_b" );
        iconList[size].iconB = NULL;
        iconList[size].id = id;
        strcpy(iconList[size].mapName, gMapName);
        iconList[size].color = CLR_RED;
    }
    else if( type == ICON_ITEM  && game_state.mission_map )
    {
        Entity *e = entities[id];
        if(e)
            strncpyt( iconList[size].name, e->name, 256 );
        else
            strncpyt( iconList[size].name, "????", 256 );
        iconList[size].destEntity = e;
        iconList[size].icon = atlasLoadTexture( "map_enticon_mission_b" );
        iconList[size].iconB = NULL;
        iconList[size].id = id;
        strcpy(iconList[size].mapName, gMapName);
        iconList[size].color = CLR_WHITE;
    }
    else if( type == ICON_MISSION )
    {
        strncpyt( iconList[size].name, "MissionString", 256 );
        iconList[size].icon = atlasLoadTexture( "map_enticon_mission_a" );
        iconList[size].iconB = atlasLoadTexture( "map_enticon_mission_b" );
        iconList[size].angle = angle;
        iconList[size].id = MISSION_ID;
        iconList[size].color = CLR_WHITE;
    }
    // fill in stuff for differnet icon types here
}

//
//
static int createIconFromDestination( Destination *iconList, Destination *dest, int idx  )
{
    Mat4 location = {0};
    //  int i;

    if( dest->mapName[0] == '\0' )
        return FALSE;

    if( stricmp( gMapName, dest->mapName ) != 0 )
        return FALSE;


    if( !dest->icon )
        iconList[idx].icon  = white_tex_atlas;
    else
        iconList[idx].icon  = dest->icon;
    iconList[idx].iconB = dest->iconB;
    iconList[idx].iconBInCompass = dest->iconBInCompass;
    iconList[idx].pulseBIcon = dest->pulseBIcon;
    iconList[idx].navOn  = dest->navOn;
    iconList[idx].dontTranslateName = false;

    iconList[idx].angle = dest->angle;
    iconList[idx].id    = dest->id;
    strcpy( iconList[idx].name, dest->name );
    strcpy( iconList[idx].mapName, dest->mapName );
    iconList[idx].type = dest->type;
    copyVec3( dest->world_location[3], location[3] );
    copyMat4( location, iconList[idx].world_location );

    iconList[idx].color = CLR_WHITE;
    iconList[idx].colorB = CLR_WHITE;

    iconList[idx].destPNPC = dest->destPNPC;
    iconList[idx].destEntity = dest->destEntity;

    iconList[idx].dbid = dest->dbid;
    iconList[idx].context = dest->context;
    iconList[idx].subhandle = dest->subhandle;

    return TRUE;
}

//
//
static void createCityIcon( Destination * iconList, int size, int type, int x, int y, char * txt )
{
    iconList[size].type = type;
    iconList[size].map_location[0] = x;
    iconList[size].map_location[1] = y;
    iconList[size].dontTranslateName = false;

    if( type == ICON_CITY )
    {
        iconList[size].icon = atlasLoadTexture( "me_marker" );
    }
    else if ( type == ICON_MISSION )
    {
        iconList[size].icon = atlasLoadTexture( "map_enticon_mission_dot" );
    }
    else if ( type == ICON_TEXT )
    {
        strcpy( iconList[size].name, txt );
    }
}

// calculates screen coordinates and angle of map icons
//
void processIcons( Destination* iconList, int size, int doAngle )
{
    int i;
    MapState *mapState = minimap_getMapState();

    for( i = 0; i < size; i++ )
    {
        iconList[i].map_location[0] = (iconList[i].world_location[3][0]-mapState->min[0])
            /(mapState->max[0] - mapState->min[0]);
        iconList[i].map_location[1] = (iconList[i].world_location[3][2]-mapState->min[2])
            /(mapState->max[2] - mapState->min[2]);

        copyVec2(iconList[i].map_location, iconList[i].sep_location);

        if ((iconList[i].type == ICON_MISSION || DestinationIsAContact(&iconList[i]))
            && iconList[i].angle == 0.0f)
        {
            iconList[i].angle -= 2*PI;
        }

        if ( !iconList[i].angle && doAngle )
        {
            iconList[i].angle = acos(iconList[i].world_location[0][0]) + PI;
            if( iconList[i].world_location[0][2] > 0 )
                iconList[i].angle = -iconList[i].angle;
        }
    }
}

// City map specific functions
//------------------------------------------------------------------------------------------------------

typedef struct CityZone
{
    int hide;
    CityMap city;
    MapTeamArea teamArea;

    Vec2 loc;
    Vec2 textLoc;

    char *name;
    char *icon;

}CityZone;

typedef struct CityLayout
{
    CityZone **zone;
}CityLayout;

CityLayout cityLayout = {0};


// Parse structures
//---------------------------------------------------------------------
StaticDefineInt ParseMapTeamArea[] =
{
    DEFINE_INT
    {"Heroes",            MAP_TEAM_HEROES_ONLY},
    {"Villains",        MAP_TEAM_VILLAINS_ONLY},
    {"Praetorians",        MAP_TEAM_PRAETORIANS},
    {"Everybody",        MAP_TEAM_EVERYBODY},
    {"PrimalCommon",    MAP_TEAM_PRIMAL_COMMON},
    {"None",            MAP_TEAM_COUNT},            // Zones With No Players
    DEFINE_END
};

StaticDefineInt ParseCity[] =
{
    DEFINE_INT
    {"ParagonCity",        CITY_PARAGON},
    {"RogueIsles",        CITY_ROGUE_ISLES},
    {"Praetoria",        CITY_PRAETORIA},
    {"None",            CITY_NONE},                    // Zones which have no corresponding image on any city map
    DEFINE_END
};

TokenizerParseInfo ParseCityZone[] =
{
    { "Name",            TOK_STRING(CityZone, name,0)                        },
    { "Icon",            TOK_STRING(CityZone, icon,0)                        },
    { "Location",        TOK_VEC2(CityZone, loc)                                },
    { "TextLocation",   TOK_VEC2(CityZone, textLoc)                            },
    { "Hide",            TOK_INT(CityZone, hide, 0)                            },
    { "City",            TOK_INT(CityZone, city, CITY_NONE), ParseCity        },
    { "TeamArea",        TOK_INT(CityZone, teamArea, MAP_TEAM_COUNT), ParseMapTeamArea    },
    { "End",            TOK_END, 0                                            },
    { "", 0, 0 }
};

TokenizerParseInfo ParseCityLayout[] =
{
    { "Zone", TOK_STRUCT(CityLayout, zone, ParseCityZone) },
    { "", 0, 0 }
};

TokenizerParseInfo ParseVisitedMaps[] =
{
    { "{",                TOK_START,        0                                                                },
    { "DbId",            TOK_INT(VisitedMap, db_id, 0)                                                    },
    { "MapId",            TOK_INT(VisitedMap, map_id, 0)                                                    },
    { "OpaqueFog",        TOK_INT(VisitedMap, opaque_fog, 0)                                                },
    { "VisitedCells",    TOK_FIXED_ARRAY | TOK_INT_X, offsetof(VisitedMap, cells), (MAX_MAPVISIT_CELLS)/32    },
    { "}",                TOK_END,        0                                                                },
    { "", 0, 0 }
};

static int compareCityZones( const CityZone** c1, const CityZone** c2 )
{
    const CityZone *cz1 = *c1;
    const CityZone *cz2 = *c2;

    char name1[32];
    char name2[32];

    strncpyt( name1, textStd(cz1->name), 32);
    strncpyt( name2, textStd(cz2->name), 32);

    if( name1[0] == '\'' )
        strcpy( name1, &name1[1] );
    if( name2[0] == '\'' )
        strcpy( name2, &name2[1] );

    return -(stricmp( name1, name2 ));
}

void mapperLoadCityInfo()
{
    if (!ParserLoadFiles("menu", ".map", "map.bin", 0, ParseCityLayout, &cityLayout, NULL, NULL, NULL))
    {
        printf("Couldn't load map!!\n");
    }
}

static UISkin skinFromCityLayout(Entity *e)
{
    int i;
    STATIC_INFUNC_ASSERT(CITY_COUNT == 4);
    for( i = eaSize( &cityLayout.zone )-1; i >= 0; i-- )
    {
        CityZone *cz = cityLayout.zone[i];
        
        if(strstri(gMapName, cz->name))
        {
            switch(cz->city)
            {
                xcase CITY_PARAGON:
                    return UISKIN_HEROES;
                xcase CITY_ROGUE_ISLES:
                    return UISKIN_VILLAINS;
                xcase CITY_PRAETORIA:
                    return UISKIN_PRAETORIANS;
                default:
                    // couldn't find a matching zone, default to the team_area defined skin
                    return skinFromMapAndEnt(e);
            }
        }
    }
    
    // couldn't find a matching zone, default to the team_area defined skin
    return skinFromMapAndEnt(e);
}

static void fixTrailingSlash( char* buffer, size_t buffSz, char trailingSlash /* or 0 to remove */ )
{
    char* p;
    size_t stringLen = strlen(buffer);

    if ( stringLen == 0 )
    {
        //
        //    Special case. If the string is empty, don't add a slash.
        //
        return;
    }
    else
    {
        p = ( buffer + stringLen - 1 );
    }
    if (( *p == '\\' ) || ( *p == '/' ))
    {
        *p = trailingSlash;        // may change slash type or zero it out
    }
    else
    {
        if (( trailingSlash != '\0' ) && ( stringLen < ( buffSz - 2 ) ))
        {
            // want a trailing slash and we have room for it
            buffer[stringLen++] = trailingSlash;
            buffer[stringLen]   = '\0';
        }
    }
}

static char sVisitedMapDirectory[_MAX_PATH+1] = { '\0' };    
#define LOCAL_USER_DATA_VISISTED_MAPDIR            "\\NCSoft\\CoX\\VisitedMaps"
char * getVisitedMapsDir(void)
{
    static bool bInitialized = false;

    if ( ! bInitialized )
    {
        //    Construct a path which works with Windows7 and beyond -- i.e., something in
        //    the user's directory (c:\documents and settings\<USER_NAME>\blah...).
        //
        {
            char* buffer = sVisitedMapDirectory;
            size_t buffSz = ARRAY_SIZE(sVisitedMapDirectory);
            HRESULT hRes = SHGetFolderPathA(NULL, CSIDL_LOCAL_APPDATA|CSIDL_FLAG_CREATE, NULL, 0, buffer);
            if (SUCCEEDED(hRes))
            {
                strcat_s(buffer, buffSz, LOCAL_USER_DATA_VISISTED_MAPDIR );
                SHCreateDirectoryExA(NULL, buffer, NULL);
                forwardSlashes(buffer);
                fixTrailingSlash( buffer, buffSz, '/' ); 
            }
            else
            {
                buffer[0] = '\0';
            }
        }
        bInitialized = true;
    }

    return sVisitedMapDirectory;
}
static VisitedStaticMap **g_vsm = NULL;
static FileList s_vmBinList = 0;

static FileScanAction  processVisitedMaps(char *dir, struct _finddata32_t *data)
{
    VisitedMap tempVM = {0};
    char filename[MAX_PATH];

    sprintf(filename, "%s/%s", dir, data->name);

    if(strEndsWith(filename, ".vm" ) && fileExists(filename)) // an actual file, not a directory
    {
        if (ParserReadBinaryFile( NULL, filename, ParseVisitedMaps, &tempVM, &s_vmBinList, 0))
        {
            VisitedStaticMap *vsm = automap_getMapStruct(tempVM.db_id ? tempVM.db_id : playerPtr()->db_id);
            VisitedMap *vm = StructAllocRaw(sizeof(VisitedMap));
            StructInit(vm,sizeof(VisitedMap), ParseVisitedMaps);
            StructCopy(ParseVisitedMaps, &tempVM, vm, 0, 0);
            eaPush(&vsm->staticMaps, vm);
        }
    }
    return FSA_NO_EXPLORE_DIRECTORY;
}

VisitedStaticMap *automap_getMapStruct(int db_id)
{
    static int loaded = 0;
    int i;
    if (!loaded)
    {
        char reloadpath[MAX_PATH];
        loaded = 1;
        sprintf(reloadpath, "%s/", getVisitedMapsDir());
        mkdirtree(reloadpath);
        fileScanDirRecurseEx(getVisitedMapsDir(), processVisitedMaps);
    }

    for (i = 0; i < eaSize(&g_vsm); ++i)
    {
        if (g_vsm[i]->db_id == db_id)
        {
            return g_vsm[i];
        }
    }
    {
        VisitedStaticMap *vsm = StructAllocRaw(sizeof(VisitedStaticMap));
        vsm->db_id = db_id;
        vsm->staticMaps = NULL;
        eaPush(&g_vsm, vsm);
        return vsm;
    }
}

void automap_updateStaticMap(VisitedMap *vm)
{
    char filename[MAX_PATH];
    sprintf(filename, "%s/Player%iMap%i.vm", getVisitedMapsDir(), vm->db_id, vm->map_id);
    ParserWriteBinaryFile(filename, ParseVisitedMaps, vm, &s_vmBinList, 0, 0, 0);
}
void automap_addStaticMapCell(VisitedStaticMap *vsm, int db_id, int map_id, int opaque_fog, int num_cells, U32 *cell_array)
{
    VisitedMap *vm = StructAllocRaw(sizeof(VisitedMap));
    int i;
    StructInit(vm, sizeof(VisitedMap), ParseVisitedMaps);
    vm->db_id = db_id;
    vm->map_id = map_id;
    vm->opaque_fog = opaque_fog;
    for (i = 0; i < num_cells; ++i)
    {
        vm->cells[i] = cell_array[i];
    }
    eaPush(&vsm->staticMaps, vm);
    automap_updateStaticMap(vm);
}
void automap_clearStaticMaps(VisitedStaticMap *vsm)
{
    int i;
    for (i = eaSize(&vsm->staticMaps)-1; i >= 0; --i)
    {
        StructDestroy(ParseVisitedMaps, vsm->staticMaps[i]);
    }
    eaDestroy(&vsm->staticMaps);
}

void drawMapZones( float x, float y, float z, float scale, int text, UISkin skin )
{
    int i;

    static float timer = 0;
    timer += TIMESTEP*.2;

    for( i = eaSize( &cityLayout.zone )-1; i >= 0; i-- )
    {
        CityZone * cz = cityLayout.zone[i];
        AtlasTex * icon = atlasLoadTexture( cz->icon );
        CBox box;
        char * string1, *string2;
        char tmp[128];

        if(cz->hide || skin != cz->city) // This is ok because the CityMap draws from the UISkin enum
            continue;

        strcpy_s( SAFESTR(tmp), textStd(cz->name) );

        string1 = strtok( tmp, " " );
        string2 = strtok( NULL, "" );

        font( &game_12 );

        if(  game_state.noMapFog || strstri( gMapName, cz->name ) || onArchitectTestMap()  )
        {
            int color = 0xffffff00 | (int)(128 + (64.f*sinf(timer)));
            display_sprite( icon, x + cz->loc[0]*scale, y + cz->loc[1]*scale, z, scale, scale, color );
            font_color( CLR_WHITE, CLR_GREEN);
        }
        else
            font_color( CLR_WHITE, CLR_WHITE);

        BuildCBox(&box, x + cz->loc[0]*scale, y + cz->loc[1]*scale, icon->width*scale, icon->height*scale );

        if( text || mouseCollision(&box) )
        {
            if( !string2 )
                cprntEx( x + scale*cz->textLoc[0], y + scale*cz->textLoc[1], z+1, scale, scale, (CENTER_X | CENTER_Y), cz->name );
            else
            {
                cprntEx( x + scale*cz->textLoc[0], y + scale*cz->textLoc[1], z+1, scale, scale, (CENTER_X), string1 );
                cprntEx( x + scale*cz->textLoc[0], y + scale*cz->textLoc[1] + scale*FONT_HEIGHT, z+1, scale, scale, (CENTER_X), string2 );
            }
        }
    }
}

static int city_map_loaded = 0;

void addMapNames( ComboBox *cb )
{
    int i;
    Entity *player = playerPtr();

    if( !city_map_loaded ) // load if nessecary
    {
        mapperLoadCityInfo();
        miniMapLoad();
        city_map_loaded = TRUE;
    }

    // City Zones are sorted alphabetically, ignoring leading quotation marks
    eaQSort( cityLayout.zone, compareCityZones );

    // We look at the teamArea defined in zones.map to find out who can see what set of zones
    // - this needs to match the results of the actual LFG search on the server using the online player info
    // MAP_TEAM_HEROES_ONLY        Non-Praetorian on blue side only
    // MAP_TEAM_VILLAINS_ONLY    Non-Praetorian on red side only
    // MAP_TEAM_PRIMAL_COMMON    Non-Praetorian, any side
    // MAP_TEAM_PRAETORIANS        Praetorian only
    // MAP_TEAM_EVERYBODY        Everybody
    STATIC_INFUNC_ASSERT(MAP_TEAM_COUNT == 5);
    for( i = eaSize( &cityLayout.zone )-1; i >= 0; i-- )
    {
        bool canLFGMap;
        MapTeamArea teamArea = cityLayout.zone[i]->teamArea;

        switch(teamArea)
        {
            xcase MAP_TEAM_EVERYBODY:
                canLFGMap = true;
            xcase MAP_TEAM_PRAETORIANS:
                canLFGMap = ENT_IS_IN_PRAETORIA(player);
            xcase MAP_TEAM_PRIMAL_COMMON:
                canLFGMap = !ENT_IS_IN_PRAETORIA(player);
            xcase MAP_TEAM_HEROES_ONLY:
                canLFGMap = ENT_IS_ON_BLUE_SIDE(player) && !ENT_IS_IN_PRAETORIA(player);
            xcase MAP_TEAM_VILLAINS_ONLY:
                canLFGMap = ENT_IS_ON_RED_SIDE(player) && !ENT_IS_IN_PRAETORIA(player);
        }
        
        if(canLFGMap)
            combocheckbox_add( cb, 0, NULL, cityLayout.zone[i]->name, 0 );
    }
}

//----------------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------
// mother function that handles actual drawing of the map
//----------------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------

#define BORDER_DIM      16      // width of emptys space that frames map
#define MAX_MAP_ZOOM    2       // maz zoom scale
#define DEFAULT_MAP_X   384     // default size
#define DEFAULT_MAP_Y   384     //
#define TAB_OFFSET      20      // amout tab is inset

#define CYCLE_LENGTH    60
#define NUM_CIRCLES     3

static int gMouseOverHit = 0;

static int gPulseCount = 0;    // should be cycled to zero on each tick
static void drawPulse( float x, float y, float z, int active )
{
    int i, clr;
    static float timer[5] = {0};
    float t[NUM_CIRCLES];
    AtlasTex * ring = atlasLoadTexture( "map_select_ring" );
    if (active) ring = atlasLoadTexture( "map_select_mission" );

    if (gPulseCount >= 5) gPulseCount = 0;
    timer[gPulseCount] += TIMESTEP;
    if( timer[gPulseCount] > CYCLE_LENGTH )
        timer[gPulseCount] -= CYCLE_LENGTH;

    for( i = 0; i < NUM_CIRCLES; i++ )
    {
        t[i] = timer[gPulseCount] + (i*CYCLE_LENGTH)/NUM_CIRCLES;
        if( t[i] > CYCLE_LENGTH )
            t[i] -= CYCLE_LENGTH;
    }

    for( i = 0; i < NUM_CIRCLES; i++ )
    {
        float sc = 1 - (t[i]/CYCLE_LENGTH);
        float opacity = t[i]/(CYCLE_LENGTH/NUM_CIRCLES);

        if( opacity > 1.f )
            opacity = 1.f;

        clr = (0? CLR_RED: CLR_WHITE) & 0xffffff00 | (int)(255*opacity);
        display_sprite( ring, x - ring->width*sc/2, y - ring->height*sc/2, z, sc, sc, clr );
    }
    gPulseCount++;
}

static int gHotSpotPulseCount = 0;    // should be cycled to zero on each tick
static void drawPulseIcon( AtlasTex *ring, float x, float y, float z)
{
    int i = 0, clr;
    int cycleLength = 30;
    int numImages = 1;
    static float timerPulse[5] = {0};
    float t[1];
    //    AtlasTex * ring = atlasLoadTexture( "map_enticon_mission_b" );

    if (gHotSpotPulseCount >= 5) 
        gHotSpotPulseCount = 0;
    timerPulse[gHotSpotPulseCount] += TIMESTEP;
    if( timerPulse[gHotSpotPulseCount] > cycleLength )
        timerPulse[gHotSpotPulseCount] -= cycleLength;

    t[i] = timerPulse[gHotSpotPulseCount] + ((i+1)*cycleLength)/numImages;
    if( t[i] > cycleLength )
        t[i] -= cycleLength;

    for( i = 0; i < numImages; i++ )
    {
        float sc = 1.3f * (t[i]/cycleLength);
        float op = ((t[i]/(cycleLength/numImages))*(t[i]/(cycleLength/numImages)));
        float opacity = 1.0f - (op*op);

        if( opacity > 1.f )
            opacity = 1.f;

        clr = (1? CLR_RED: CLR_WHITE) & 0xffffff00 | (int)(255*opacity);
        display_sprite( ring, x - ring->width*sc/2, y - ring->height*sc/2, z-1.1, sc, sc, clr );
    }
    //    gHotSpotPulseCount++;
}


// constants governing separating icons on the mini-map
int g_mapSepType = 2;
#define MIN_MAP_SIZE    100        // if smaller, we don't push stuff around
#define MAX_PUSH_DIST    900    // if farther, we don't get a push
#define MIN_PUSH_DIST    500    // if we get pushed this or less, dont bother.
#define PUSH_SCALE        (.0002)    // strength to push icons away (scaled to d squared)
#define PUSH_LIMIT        900        // max factor to use as push
#define TENSION_SCALE    (4)        // strength to pull back to correct location
#define MIN_MOTION_2    (0.1)    // below this limit, stop moving
#define MOUSE_INNER        (35)    // inside here, you see separated location
#define MOUSE_OUTER        (45)    // outside here, you see correct location
#define MAX_SEP_DIST    (30)    // can't be farther than this distance from correct

F32 distance2(Vec2 a, Vec2 b)
{
    return sqrt((a[0]-b[0])*(a[0]-b[0]) + (a[1]-b[1])*(a[1]-b[1]));
}

F32 mag2Squared(Vec2 a)
{
    return a[0]*a[0]+a[1]*a[1];
}

void addImpulse(Destination* icons, int count, Vec2 from, Vec2 impulse, int mapWidth, int mapHeight)
{
    Vec2 ray;
    F32 d2, push;
    int i;

    for (i = 0; i < count; i++)
    {
        if (!icons[i].shownLastFrame)
            continue;
        subVec2(from, icons[i].sep_location, ray);
        ray[0] *= mapWidth;
        ray[1] *= mapHeight;
        d2 = mag2Squared(ray);
        if (d2 == 0.0) 
            continue;
        push = MAX_PUSH_DIST - d2; // distance from max push distance
        if (push < 0)
            continue;
        scaleVec2(ray, 1/sqrt(d2), ray);        // unit vector
        if (push > PUSH_LIMIT)
            push = PUSH_LIMIT;
        scaleVec2(ray, PUSH_SCALE*push*push, ray);    // push is a squared factor
        addVec2(impulse, ray, impulse);
    }
}

void calcViewLocation(Destination* icon, int mapWidth, int mapHeight, int type, int x, int y)
{
    // push the sep location around
//    if ((mapWidth < MIN_MAP_SIZE && mapHeight < MIN_MAP_SIZE) || !icon->shownLastFrame)
    if (!icon->shownLastFrame)
        copyVec2(icon->map_location, icon->sep_location);
    else
    {
        // add push from each surrounding point
        Vec2 ray = {0};
        Vec2 impulse = {0};
        F32 d2;

        if (DRAWING_STATIC_ICONS)
            addImpulse( staticIcons, staticIconCount, icon->sep_location, impulse, mapWidth, mapHeight );
        if (DRAWING_ACTIVE_ICONS)
        {
            addImpulse( activeIcons, activeIconCount, icon->sep_location, impulse, mapWidth, mapHeight );
            addImpulse( &thumbtack, 1, icon->sep_location, impulse, mapWidth, mapHeight );
        }
        // MAKDEBUG
        if (0 && icon->type == ICON_PLAYER)
        {
            xyprintf(1, 6, "push impulse on player %f, %f", impulse[0], impulse[1]);
        }

        // add tension to correct location
        subVec2(icon->map_location, icon->sep_location, ray);
        ray[0] *= mapWidth;
        ray[1] *= mapHeight;
        d2 = mag2Squared(ray);
        if (!nearf(d2, 0.0))
        {
            scaleVec2(ray, TENSION_SCALE, ray);    // pull vector
            addVec2(ray, impulse, impulse);
        }

        // MAKDEBUG
        if (0 && icon->type == ICON_PLAYER)
        {
            xyprintf(1, 7, "pull impulse on player %f, %f", ray[0], ray[1]);
            xyprintf(1, 8, "total impulse on player %f, %f", impulse[0], impulse[1]);
        }

        // add impulse, and clamp
        if (mag2Squared(impulse) > .1)
        {
            impulse[0] /= mapWidth*10;
            impulse[1] /= mapHeight*10;
            addVec2(icon->sep_location, impulse, icon->sep_location);
        }

        // max out the distance
        subVec2(icon->sep_location, icon->map_location, ray);
        ray[0] *= mapWidth;
        ray[1] *= mapHeight;
        d2 = mag2Squared(ray);
        if (d2 > MAX_SEP_DIST*MAX_SEP_DIST)
        {
            scaleVec2(ray, 1/sqrt(d2)*MAX_SEP_DIST, ray);
            ray[0] /= mapWidth;
            ray[1] /= mapHeight;
            addVec2(ray, icon->map_location, icon->sep_location);
        }
    }

    // view location depends on user setting
    if (distance2(icon->sep_location, icon->map_location)*mapWidth*mapHeight < MIN_PUSH_DIST)
        copyVec2(icon->map_location, icon->view_location);
    else
    {
        switch (g_mapSepType)
        {
        case 1: copyVec2(icon->sep_location, icon->view_location);
            xcase 2: 
                {
                    int mx = gMouseInpCur.x;
                    int my = gMouseInpCur.y;
                    int mouseDist2 = sqrt((mx-x)*(mx-x)+(my-y)*(my-y));
                    if (mouseDist2 >= MOUSE_OUTER || mouseActive())
                        copyVec2(icon->map_location, icon->view_location);
                    else if (mouseDist2 <= MOUSE_INNER)
                        copyVec2(icon->sep_location, icon->view_location);
                    else // linear interpolation in between
                    {
                        Vec2 res;
                        F32 r = (F32)(mouseDist2 - MOUSE_INNER) / (F32)(MOUSE_OUTER - MOUSE_INNER);
                        subVec2(icon->map_location, icon->sep_location, res);
                        scaleVec2(res, r, res);
                        addVec2(icon->sep_location, res, icon->view_location);
                    }
                }
xdefault:copyVec2(icon->map_location, icon->view_location); break;
        }
    }

    // MAKDEBUG
    if (0 && icon->type == ICON_PLAYER)
    {
        xyprintf(1, 5, "player angle %f", icon->angle);
        xyprintf(1, 9, "player map location %f, %f", icon->map_location[0]*mapWidth, icon->map_location[1]*mapHeight);
        xyprintf(1, 10, "player sep location %f, %f", icon->sep_location[0]*mapWidth, icon->sep_location[1]*mapHeight);
        xyprintf(1, 11, "player view location %f, %f", icon->view_location[0]*mapWidth, icon->view_location[1]*mapHeight);
    }
}

// reset seperations and watch them happen again
void resetSepIter(Destination* icons, int count)
{
    int i;
    for (i = 0; i < count; i++)
        copyVec2(icons[i].map_location, icons[i].sep_location);
}

void resetSeps(void)
{
    resetSepIter( staticIcons, staticIconCount );
    resetSepIter( activeIcons, activeIconCount );
    resetSepIter( &thumbtack, 1 );
}


static unsigned int old_icon_types_on_map[2]={0, 0};
static unsigned int icon_types_on_map[2]={0, 0};

static int cmp_space_list(char *needle, char *haystack)
{
    char *sp;
    char *lp;
    int rv;

    sp = haystack;
    for (;;)
    {
        lp = strchr(sp, ' ');
        if (lp)
        {
            *lp = 0;
            rv = stricmp(needle, sp);
            *lp = ' ';
            if (rv == 0)
                return 1;
            sp = lp+1;
        } else {
            if (stricmp(needle, sp) == 0)
                return 1;
            return 0;
        }
    }
}

static int haystack_cmp(char *h1, char *h2)
{
    char *sp;
    char *lp;
    int rv;

    sp = h1;
    for (;;)
    {
        lp = strchr(sp, ' ');
        if (lp)
        {
            *lp = 0;
            rv = cmp_space_list(sp, h2);
            *lp = ' ';
            if (rv)
                return 1;
            sp = lp+1;
        } else {
            return cmp_space_list(sp, h2);
        }
    }
}

int drawIcons( Destination *icons, int iconcount, int mapWidth, int mapHeight, float scale, int z, int type )
{
    int i;
    int itype_converted;
    static  float glow_timer = 0;
    AtlasTex * white = atlasLoadTexture( "White" );
    Entity*        e;
     MapState *mapState = minimap_getMapState();
    int zcount;

     e = playerPtr();

    font_color( CLR_WHITE, CLR_WHITE);
    glow_timer += TIMESTEP*.1;
    zcount = -1;
    for( i = 0; i < iconcount; i++ )
    {
        int clr = DEFAULT_RGBA;
        float szoom = 1;
        int x, y, text_y, text_x;
        CBox box;
        char display_name[1024];
        int width;

        icons[i].shownLastFrame = 0;

        if (icons[i].dontTranslateName)
        {
            strcpy_s(display_name, 1024, icons[i].name);
        }
        else
        {
            strcpy_s(display_name, 1024, textStd(icons[i].name));
        }

#if 0
        if (icons[i].type == ICON_ZOWIE)
        {
            if (zowieIcons[i]->pulse_glow == 0)
                continue;
            display_name = g_zowie_display_name;
            strncpyt(icons[i].name, g_zowie_display_name, 256);
        }
#endif

        // you might not have the Entity for the minimap icon, but you still need to check vision phase
        if (icons[i].destEntity ? !entity_TargetIsInVisionPhase(e, icons[i].destEntity) : !entity_PNPCIsInVisionPhase(e, icons[i].destPNPC))
            continue;
        
        itype_converted = icons[i].type;
        if (itype_converted == ICON_FOLLOWER)
            itype_converted = ICON_TEAMMATE;

        if(itype_converted < 32)
            icon_types_on_map[0] |= (1<<itype_converted);
        else if(itype_converted < 64)
            icon_types_on_map[1] |= (1<<(itype_converted - 32));
        else
            assert(0 && "need more icon type on map bitfields!");

        if (!icons[i].icon || 
            !withinCurrentSection(icons[i].world_location, mapState) ||
            (combobox_hasID(&comboIcons, itype_converted) &&
            !combobox_idSelected(&comboIcons, itype_converted)))
        {
            continue;
        }

        if (icons[i].required)
        {
            if (e) 
            {
                if (!chareval_requires(e->pchar, icons[i].required, icons[i].pchSourceFile))
                {
                    continue; 
                }
            }
        }

        // need real location for fog
        x = mapState->xp + mapWidth - icons[i].map_location[0]*mapWidth - (icons[i].icon->width/2)*szoom;
        y = icons[i].map_location[1]*mapHeight + mapState->yp - (icons[i].icon->height/2)*szoom;

        if (icons[i].bHiddenInFog && !mapfogIsVisitedPos(icons[i].world_location[3]))
            continue;
        icons[i].shownLastFrame = 1;

        // now based on view location
        calcViewLocation(&icons[i], mapWidth, mapHeight, type, x, y); 
        x = mapState->xp + mapWidth - icons[i].view_location[0]*mapWidth - (icons[i].icon->width/2)*szoom;
        y = icons[i].view_location[1]*mapHeight + mapState->yp - (icons[i].icon->height/2)*szoom;

        BuildCBox( &box, x - 5, y - 5, icons[i].icon->width*szoom + 10, icons[i].icon->height*szoom +10 );

        if (1) // (!gMouseOverHit) // MAK - this doesn't seem right, I'm taking it out
        {
            int active = 0;
            if (isDestination(&activeTaskDest, &icons[i])) 
                active = 1;
            if (active || isDestination(&waypointDest, &icons[i]))
                drawPulse( x + icons[i].icon->width/2, y + icons[i].icon->height/2, z+5, active );
        }

        if( box.ly - 20 < mapState->yp + 25 )
            text_y = mapState->yp + 25;
        else
            text_y = box.ly -5;

        // Already translated if necessary
        width = str_wd_notranslate( font_grp, scale, scale, display_name );

        if( (box.lx + box.hx - width)/2 < mapState->xp )
            text_x = mapState->xp + width/2;
        else
            text_x = (box.lx + box.hx)/2;

        if( (box.lx + box.hx + width)/2 > mapState->xp + mapWidth )
            text_x = mapState->xp + mapWidth - width/2;

        if ( mouseCollision( &box ) && !gMouseOverHit )
        {
            AtlasTex * ring = atlasLoadTexture( "map_select_ring" );

            gMouseOverHit = TRUE;

            display_sprite( ring, x + icons[i].icon->width/2 - ring->width/2, y + icons[i].icon->height/2 - ring->height/2, z+4, 1.f, 1.f, 0xffffff90 );
            setCursor( "cursor_friendly.tga", NULL, FALSE, 2, 2 );

            // and maybe its text
            if( !mapState->textOn ) {
                if( icons[i].type == ICON_PLAYER || icons[i].type == ICON_TEAMMATE || icons[i].type == ICON_FOLLOWER || (icons[i].type == ICON_LEAGUEMATE))
                    cprntEx( text_x, text_y, z+6, scale, scale, NO_MSPRINT|CENTER_X, display_name );
                if( icons[i].type == ICON_GATE )
                    cprntEx( text_x, text_y, z+6, scale, scale, NO_MSPRINT|NO_PREPEND|CENTER_X, display_name );
                else
                    cprntEx( text_x, text_y, z+6, scale, scale, NO_MSPRINT|NO_PREPEND|CENTER_X, display_name );
            }

            if( stricmp( icons[i].name, "ThumbtackString" ) == 0 )
            {
                if( mouseClickHit(&box, MS_RIGHT) )
                {
                    if( isDestination(&waypointDest, &icons[i]) )
                        clearDestination( &waypointDest );

                    clearDestination( &icons[i] );
                    return 1;
                }
            }
        }

        if( mapState->textOn )
        {
            if( icons[i].type == ICON_PLAYER || icons[i].type == ICON_TEAMMATE || icons[i].type == ICON_FOLLOWER || (icons[i].type == ICON_LEAGUEMATE))
                cprntEx( text_x, text_y, z+5, scale, scale, NO_MSPRINT|CENTER_X, display_name );
            if( icons[i].type == ICON_GATE )
                cprntEx( text_x, text_y, z+5, scale, scale, NO_MSPRINT|NO_PREPEND|CENTER_X, display_name );
            else
                cprntEx( text_x, text_y, z+5, scale, scale, NO_MSPRINT|NO_PREPEND|CENTER_X, display_name );
        }

        if ( icons[i].type == ICON_PLAYER )
        {
            AtlasTex * dot = atlasLoadTexture("map_enticon_you_a");
            z += 4;
            display_rotated_sprite( dot, x + (icons[i].icon->width - dot->width)/2,
                y + (icons[i].icon->height - dot->height)/2, z+5, szoom, szoom, CLR_WHITE, icons[i].angle, 0 );
        }
        if( (icons[i].type == ICON_TEAMMATE) || (icons[i].type == ICON_FOLLOWER) || (icons[i].type == ICON_LEAGUEMATE))
            z += 3;


        // toggle destination status
        if( mouseDownHit( &box, MS_LEFT ) && (icons[i].type != ICON_PLAYER || icons[i].id != playerPtr()->db_id ) )
        {
            collisions_off_for_rest_of_frame = 1;
            if(icons[i].type == ICON_MISSION_KEYDOOR)
            {
                int key, numKeys = eaSize(&keydoorDest);
                for (key = 0; key < numKeys; key++)
                    keydoorDest[key]->showWaypoint = keydoorDest[key]->navOn = (keydoorDest[key]->id == icons[i].id) && !keydoorDest[key]->navOn;
            }
            else if( isDestination(&waypointDest, &icons[i]))
            {
                clearDestination(&waypointDest);
            }
            else
            {
                compass_SetDestination(&waypointDest, &icons[i]);
            }
            if( icons[i].type == ICON_TEAMMATE || icons[i].type == ICON_LEAGUEMATE)
                targetSelect(entFromDbId(icons[i].id));
            else
                targetClear();
        }

        // show the keydoor selected pulse
        if(icons[i].type == ICON_MISSION_KEYDOOR && icons[i].navOn)
            drawPulse(x + icons[i].icon->width/2, y + icons[i].icon->height/2, z+5, 0);

        // finally draw it
        TaskUpdateColor(&icons[i]);
         if (icons[i].type == ICON_FOLLOWER)
            display_rotated_sprite( icons[i].icon, x+(icons[i].icon->width*0.125), y+(icons[i].icon->height*0.125), z+4, szoom*0.75, szoom*0.75, icons[i].color, icons[i].angle, 0 );
        else
            display_rotated_sprite( icons[i].icon, x, y, z+4, szoom, szoom, icons[i].color, icons[i].angle, 0 );


        if( icons[i].iconB )
        {
            if (icons[i].pulseBIcon)
            {
                drawPulseIcon(icons[i].iconB, x + icons[i].icon->width/2,
                    y + icons[i].icon->height/2, z+3);
            }
            else 
            {
                display_rotated_sprite( icons[i].iconB, x + (icons[i].icon->width - icons[i].iconB->width)/2, 
                    y + (icons[i].icon->height - icons[i].iconB->height)/2, z+3, szoom, szoom, icons[i].colorB, icons[i].angle, 0 );
            }
        }

        if ( icons[i].type == ICON_PLAYER )
            z -= 4;
        if( (icons[i].type == ICON_TEAMMATE) || (icons[i].type == ICON_FOLLOWER) || (icons[i].type == ICON_LEAGUEMATE))
            z -= 3;
    }
    return 0;
}

static void setThumbtack( int mapWidth, int mapHeight )
{
    int x, y;
    Vec2 map_location;
    Vec3 world_location;
    MapState *mapState = minimap_getMapState();

    rightClickCoords( &x, &y );

    map_location[0] = (1-(float)(x - mapState->xp)/mapWidth);
    map_location[1] = (float)(y - mapState->yp)/mapHeight;

    world_location[0] = map_location[0]*(mapState->max[0]-mapState->min[0]) + mapState->min[0];
    world_location[1] = 0;
    world_location[2] = map_location[1]*(mapState->max[2]-mapState->min[2]) + mapState->min[2];

    // mapsetpos
    if (game_state.mapsetpos)
    {
        Vec3 end;
        bool hitWorld;
        Mat4 mat;
        ItemSelect item_sel;
        char buf[1024];

        world_location[1] = 1000.f;
        copyVec3(world_location, end);
        end[1] = -1000.f;

        //printf("\nMapWarp: %f, %f, %f\n", world_location[0], world_location[1], world_location[2]);
        cursorFind3D(world_location, end, &item_sel, mat, &hitWorld);

        if (hitWorld)
        {
            copyVec3(mat[3],end);
            //printf(" Hit World: %f %f %f\n", end[0], end[1], end[2]);
            end[1] += 10;
        }
        else
        {
            end[1] = 500.f;
            //printf(" Missed World: %f %f %f\n", end[0], end[1], end[2]);
        }

        sprintf(buf, "setpos %f %f %f", end[0], end[1], end[2]);
        cmdParse(buf);

        return;
    }

    copyVec2(map_location, thumbtack.map_location);
    copyVec3(world_location, thumbtack.world_location[3]);


    thumbtack.icon = atlasLoadTexture( "map_enticon_pushpin" );
    thumbtack.color = CLR_WHITE;
    thumbtack.type = ICON_THUMBTACK;
    strcpy(thumbtack.mapName, gMapName);
    strcpy(thumbtack.name, "ThumbtackString" );

    if(!isDestination(&waypointDest, &thumbtack))
        compass_SetDestination(&waypointDest, &thumbtack);
}

// Actually, the width of the widest string.
// This should probably be in uiComboBox, not here, and be global.
static int width_of_selectable_options(ComboBox *cb)
{
    int i, widest, wd, count;

    count = eaSize(&cb->elements);
    widest = 0;
    for( i = 0; i < count; i++ )
    {
        if( !( cb->elements[i]->flags & CCE_UNSELECTABLE ) )
        {
            wd = str_wd(&game_12, 1.f, 1.f, comboIcons.elements[i]->txt );
            if (widest < wd)
            {
                widest = wd;
            }    
        }
    }
    return widest;
}



//-----------------------------------------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------------------------------------
// should be broken into much smaller functions
// this is where most of the work gets done

void mapUnavailable()
{
    float x, y, z, sc, wd, ht;
    int color, bcolor;

    if( !window_getDims( WDW_MAP, &x, &y, &z, &wd, &ht, &sc, &color, &bcolor ) )
        return;

    font_color( CLR_WHITE, CLR_WHITE );
    cprntEx( x + wd/2, y + ht/2, z+10, sc, sc, (CENTER_X|CENTER_Y), "MapUnavailable" );
}

void drawMap( char * map_name, int type )
{
    float wx, wy, wz, wsc, winwd, winht, wd, ht, wxs, wys, scale;
    char tmp[128];
    int color, bcolor, mapWidth, mapHeight;
    float sliderZoom, minZoom, normalizedZoom, xzoom, yzoom, newZoom;
    static int panDiffX, panDiffY, panning = FALSE, resizing = FALSE,
        lastx = 0, lasty = 0, last_player_x = 0, last_player_y = 0, last_type = MAP_ZONE;
    static float cprX, cprY,zoomratio;
    CBox box;

     AtlasTex * iconON        = atlasLoadTexture( "map_button_names_on" );
    AtlasTex * iconOFF       = atlasLoadTexture( "map_button_names_off" );
    AtlasTex * compass       = atlasLoadTexture( "map_compass" );
    AtlasTex * map = 0;
    AtlasTex * hand = 0;

    MapState *mapState = minimap_getMapState();
    Entity *player = playerPtr();
    UISkin skin = player?skinFromMapAndEnt(player):game_state.skin;

    //Checking for map change here, since mission maps don't seem to call miniMapLoad for some reason.
    if (strcmp(minimap_scenefile, group_info.scenefile))
        miniMapLoad();


    // first get the window information
    if( !window_getDims( WDW_MAP, &wx, &wy, &wz, &wd, &ht, &wsc, &color, &bcolor ) )
        return;


    if( window_getMode(WDW_MAP) != WINDOW_DISPLAYING )
        return;

    display_sprite( compass, wx + wd - (compass->width)*wsc, wy, wz+5, wsc, wsc, CLR_WHITE );

    //if we're using a header, just draw it.
    if(s_isHeader && s_curHeader)
    {
        automap_drawMapHeader(s_curHeader, wx, wy, wz, wsc, wd, ht );
        return;
    }

    if( type == MAP_CITY )
    {
        skin = player?skinFromCityLayout(player):game_state.skin;
        if (skin == UISKIN_PRAETORIANS)
        {
            map = atlasLoadTexture("praetoria_citymap");
        }
        else if (skin == UISKIN_VILLAINS)
        {
            map = atlasLoadTexture("rogueisles_citymap");
        }
        else
        {
            map = atlasLoadTexture("paragon_citymap");
        }
    }
    else if ( type == MAP_ZONE || type == MAP_OUTDOOR_MISSION )
    {
        if(minimap_getCurrentExtent() && minimap_getCurrentExtent()->outdoor_poly && minimap_getCurrentExtent()->outdoor_texture)
        {
            map = atlasLoadTexture( minimap_getCurrentExtent()->outdoor_texture );
        }
        else
        {
            char filename[256];

            strcpy_s(SAFESTR(filename), getFileName(map_name));
            sprintf( tmp, "map_%s", strtok(filename,".") );
            map = atlasLoadTexture( tmp );
        }

        if( stricmp( map->name, "white" ) == 0 )
        {
            mapUnavailable();
            if (!isDevelopmentMode())
                return;
        }
    }

    // figure out some frequently used numbers
    winwd = wd; // store actual window size
    winht = ht;

    wd -= BORDER_DIM*2; // map screen size
    ht -= BORDER_DIM*2;

    wxs = wd / DEFAULT_MAP_X; // scale from default
    wys = ht / DEFAULT_MAP_Y;
    scale = MAX( wxs, wys );

    mapState->xp -= lastx - wx;  // location
    mapState->yp -= lasty - wy;

    // if first time open, center map on player
    //------------------------------------------------------------------------------------
    if( last_player_x != (int)activeIcons[0].world_location[3][0] || last_player_y != (int)activeIcons[0].world_location[3][2] || type != last_type  )
    {
        if( type == MAP_MISSION_MAP || type == MAP_BASE )
        {
            mapState->xp = (wx + (winwd/2))- (((mapState->width)*mapState->zoom) - activeIcons[0].map_location[0]*((mapState->width)*mapState->zoom));
            mapState->yp =  wy + (winht/2) - activeIcons[0].map_location[1]*((mapState->height)*mapState->zoom);
        }
        else
        {
            mapState->xp = (wx + (winwd/2))- ((map->width*scale*mapState->zoom) - activeIcons[0].map_location[0]*(map->width*scale*mapState->zoom));
            mapState->yp =  wy + (winht/2) - activeIcons[0].map_location[1]*(map->height*scale*mapState->zoom);
        }
    }

    last_player_x = activeIcons[0].world_location[3][0];
    last_player_y = activeIcons[0].world_location[3][2];
    last_type = type;

    // handle input (draw buttons/frame)
    //------------------------------------------------------------------------------------
    
    // dev click
    BuildCBox( &box, wx + (winwd - wd)/2, wy + (winht - ht)/2, wd, ht );

    // zoom
    if( type == last_type )
    {
        float centerPointRatioX = (wx + (winwd/2) - mapState->xp)/(mapState->zoom);
        float centerPointRatioY = (wy + (winht/2) - mapState->yp)/(mapState->zoom);

        // you can only zoom out to the extents of the map
        if( type == MAP_MISSION_MAP || type == MAP_BASE )
        {
            xzoom = wd/mapState->width;
            yzoom = ht/mapState->height;
        }
        else
        {
            xzoom = wd/(map->width*scale);
            yzoom = ht/(map->height*scale);
        }

        // the slider function takes a normalized number
        minZoom = MIN( xzoom, yzoom );
        normalizedZoom = (( mapState->zoom - minZoom )/( MAX_MAP_ZOOM - minZoom ))*2 - 1;


        if( wsc > .15f )
            sliderZoom = doVerticalSlider( wx + winwd - PIX3/2, wy + winht/2, wz + 4, winht/2, 1.f, 1.f, normalizedZoom, SLIDER_MAP_SCALE, color );
        else
            sliderZoom = 0;

        newZoom = ((sliderZoom + 1)/2 * ( MAX_MAP_ZOOM - minZoom )) + minZoom;

        // if we did change the zoom scale, recalculate map coordinates to keep centered
        if ( newZoom != mapState->zoom && window_getMode(WDW_MAP) == WINDOW_DISPLAYING )
        {
            if ( newZoom > MAX_MAP_ZOOM )
                newZoom = MAX_MAP_ZOOM;
            if ( newZoom < minZoom )
                newZoom = minZoom;

            mapState->zoom = newZoom;
            mapState->xp -= (centerPointRatioX * (mapState->zoom) + mapState->xp) - (wx + winwd/2);
            mapState->yp -= (centerPointRatioY * (mapState->zoom) + mapState->yp) - (wy + winht/2);
        }
    }

    // pan
    BuildCBox( &box, wx + (winwd - wd)/2, wy + (winht - ht)/2, wd, ht );

    // load special mouse cursor for panning
    if( !panning )
    {
        if ( mouseCollision( &box ) )
            setCursor( "cursor_hand_open.tga", NULL, FALSE, 2, 2 );
    }

    if( mouseDownHit( &box, MS_LEFT ) )
    {
        panning = TRUE;
        panDiffX = gMouseInpCur.x - mapState->xp;
        panDiffY = gMouseInpCur.y - mapState->yp;
    }

    if( panning ) // move map
    {
        setCursor( "CURSOR_HAND_CLOSED.tga", NULL, TRUE, 2, 2 );

        mapState->xp = gMouseInpCur.x - panDiffX;
        mapState->yp = gMouseInpCur.y - panDiffY;

        if( !isDown( MS_LEFT ) )
        {
            panning = FALSE;
        }
    }

    // re-size
    if ( !resizing && winDefs[WDW_MAP].drag_mode )
    {
        resizing    = TRUE;
        zoomratio   = (mapState->zoom-minZoom)/(float)(MAX_MAP_ZOOM-minZoom);
        cprX          = (wx + (winwd/2) - mapState->xp)/(mapState->zoom*scale);
        cprY          = (wy + (winht/2) - mapState->yp)/(mapState->zoom*scale);
    }

    if( resizing ) // resizing effects the scale of the map because we are trying to keep it proportional
    { 
        mapState->zoom = zoomratio*(MAX_MAP_ZOOM-minZoom)+minZoom;
        mapState->xp -= (cprX * (mapState->zoom)*scale + mapState->xp) - (wx + winwd/2);
        mapState->yp -= (cprY * (mapState->zoom)*scale + mapState->yp) - (wy + winht/2);

        if( !isDown( MS_LEFT ) )
            resizing = FALSE;
    }

    // figure out scale/location/clipping
    //------------------------------------------------------------------------------------------
    // ok we finally finished moved everything around, now fit the map if we need to

    if( type == MAP_MISSION_MAP || type == MAP_BASE )
    {
        mapWidth = mapState->width*mapState->zoom;
        mapHeight = mapState->height*mapState->zoom;
    }
    else
    {
        mapWidth  = map->width  * scale * mapState->zoom;
        mapHeight = map->height * scale * mapState->zoom;
    }


    // push each corner to the edge
    if( mapState->xp > wx + (winwd - wd)/2 )
        mapState->xp = wx + (winwd - wd)/2;

    if( mapState->yp > wy + (winht - ht)/2 )
        mapState->yp = (wy + (winht - ht)/2);

    if( mapState->xp + mapWidth < wx + winwd - BORDER_DIM )
        mapState->xp = (wx + winwd - BORDER_DIM - mapWidth);

    if( mapState->yp + mapHeight < wy + winht - BORDER_DIM )
        mapState->yp = (wy + winht - BORDER_DIM - mapHeight);


    // check to see if we are letter boxing, if so center
    // we should never be able to have letterboxing on both axis
    if( mapWidth < wd )
        mapState->xp = ( wx + winwd/2 - mapWidth/2 );
    if( mapHeight < ht )
        mapState->yp = ( wy + winht/2 - mapHeight/2 );

    font( &game_12 );
    font_color( CLR_WHITE, CLR_WHITE );

    // deal with the little text toggle
    if( window_getMode(WDW_MAP) == WINDOW_DISPLAYING )
    {
        if( (type == MAP_CITY || type == MAP_ZONE) && ( isDevelopmentMode() || (!game_state.mission_map && (strstri( gMapName, "outdoor" ) == NULL)) ))
        {
            char full_name[1000];

            if(game_state.map_instance_id > 1)
                sprintf(full_name, "%s_%d", gMapName, game_state.map_instance_id);
            else
                strcpy(full_name, gMapName);

            prnt( wx + 10*wsc, wy + winht-7*wsc, wz+7*wsc, wsc, wsc, full_name );
        }
    }

    // draw the map pieces now
    //-----------------------------------------------------------------------------------------

    set_scissor( TRUE );
    scissor_dims( wx + (winwd - wd)/2, wy + (winht -ht)/2, wd, ht );

    // the map itself
    if(type != MAP_MISSION_MAP && type != MAP_BASE )
        display_sprite( map, mapState->xp, mapState->yp, wz+1, scale*mapState->zoom, scale*mapState->zoom, DEFAULT_RGBA );

    // draw live icons (entities, mission)
    icon_types_on_map[0] = 0;
    icon_types_on_map[1] = 0;

    gPulseCount = 0;
    if ( type == MAP_CITY )
    {
        drawMapZones( mapState->xp, mapState->yp, wz+1, scale*mapState->zoom, mapState->textOn, skin );
    }
    else 
    {
        if (DRAWING_STATIC_ICONS) // a little hinky but I need to be syncronized with calcViewLocation
        {
            processIcons( &staticIcons[0], staticIconCount, FALSE );
            drawIcons( staticIcons, staticIconCount, mapWidth, mapHeight, wsc, wz, type );
        }
        if (DRAWING_ACTIVE_ICONS)
        {
            drawIcons( activeIcons, activeIconCount, mapWidth, mapHeight, wsc, wz, type );
            if( !drawIcons( &thumbtack, 1, mapWidth, mapHeight, wsc, wz, type ) && mouseClickHit(&box, MS_RIGHT) )
            {
                if( isDestination(&waypointDest, &thumbtack ) )
                    clearDestination( &waypointDest );

                setThumbtack( mapWidth, mapHeight );

                compass_SetDestination( &waypointDest, &thumbtack );
            }

            if (type == MAP_MISSION_MAP)
            {
                groupProcessDef(&group_info,drawMapTiles );

                if( onArchitectTestMap() )
                    drawItemLocations(mapState->architect_flags, 0);

                mapfogDrawZoneFog(scale,map,wz,0);
            }
            else if( type == MAP_BASE )
            {
                if (g_isBaseRaid)
                {
                    mapDrawRaidRooms();
                }
                else
                {
                    mapDrawBaseRooms();
                }
            }
            else
                mapfogDrawZoneFog(scale,map,wz, mapState->mission_type == MAP_OUTDOOR_INDOOR);
        }
    }

    set_scissor( FALSE );

    if (old_icon_types_on_map[0] != icon_types_on_map[0] || old_icon_types_on_map[1] != icon_types_on_map[1])
    {
        old_icon_types_on_map[0] = icon_types_on_map[0];
        // If these aren't in the zone, mark as unselectable so they don't show on the option/combo menu
        combobox_setSelectable(&comboIcons, ICON_ARCHITECT,        icon_types_on_map[0] & (1<<ICON_ARCHITECT));
        combobox_setSelectable(&comboIcons, ICON_TRAIN_STATION,    icon_types_on_map[0] & (1<<ICON_TRAIN_STATION));
        combobox_setSelectable(&comboIcons, ICON_HOSPITAL,        icon_types_on_map[0] & (1<<ICON_HOSPITAL));
        combobox_setSelectable(&comboIcons, ICON_ARENA,            icon_types_on_map[0] & (1<<ICON_ARENA));
        combobox_setSelectable(&comboIcons, ICON_SEWER,            icon_types_on_map[0] & (1<<ICON_SEWER));
        combobox_setSelectable(&comboIcons, ICON_ZOWIE,            icon_types_on_map[0] & (1<<ICON_ZOWIE));
        combobox_setSelectable(&comboIcons, ICON_PRECINCT,        icon_types_on_map[0] & (1<<ICON_PRECINCT));
        combobox_setSelectable(&comboIcons, ICON_LOUNGE,        icon_types_on_map[0] & (1<<ICON_LOUNGE));

        old_icon_types_on_map[1] = icon_types_on_map[1];
        combobox_setSelectable(&comboIcons, ICON_CONTACT_SIGNATURE_STORY,    icon_types_on_map[1] & (1<<(ICON_CONTACT_SIGNATURE_STORY - 32)));
        combobox_setSelectable(&comboIcons, ICON_CONTACT_TASK_FORCE,        icon_types_on_map[1] & (1<<(ICON_CONTACT_TASK_FORCE - 32)));

        //resize option combo click map thing 
        opt_wd = width_of_selectable_options(&comboIcons) + 60;
        comboIcons.wd = opt_wd;
        comboIcons.dropWd = opt_wd;

    }

    gMouseOverHit = 0;
    lastx = wx;
    lasty = wy;
}

//-----------------------------------------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------------------------------------

void addActiveIcons(void)
{
    int                    count = 0;
    Entity                *e = playerPtr();
    int                    i = 0;
    TaskStatus            **tasks = TaskStatusGetPlayerTasks();
    StashTableIterator    iter;
    StashElement        waypoint;
    int                    bShowEnemies = 0;
    int                    bShowItem = 0;

    // now the thumbtack icon
    // first the player icon
    createZoneIcon( activeIcons, count, ICON_PLAYER, ENTMAT(e), 0, e->owner, e);
    activeIcons[0].color = getHealthColor( true, e->pchar->attrCur.fHitPoints/e->pchar->attrMax.fHitPoints);
    count++;

    // Show all players
    if(game_state.bMapShowPlayers)
    {
        for(i=1; i<entities_max && count < MAX_ACTIVE_ICONS; i++)
        {
            if( (entity_state[i]&ENTITY_IN_USE) && ENTTYPE(entities[i])==ENTTYPE_PLAYER)
            {
                createZoneIcon(activeIcons, count, ICON_PLAYER, ENTMAT(entities[i]), 0, i, entities[i]);
                count++;
            }
        }
    }

    //    set up the drawing
    for( i = eaSize(&g_ppRemainderLocations)-1; i>=0; i-- )
    { 
        g_ppRemainderLocations[i]->found_live_ent = 0;
        if( g_ppRemainderLocations[i]->type == ICON_ENEMY )
            bShowEnemies = 1;
        if( g_ppRemainderLocations[i]->type == ICON_ITEM )
            bShowItem = 1;
    }

     if( game_state.mission_map && eaSize(&g_ppRemainderLocations) )
    {
        int i;
        for(i=1; i<entities_max && count < MAX_ACTIVE_ICONS; i++)
        {
             if(entity_state[i]&ENTITY_IN_USE)
            {
                int j, type = ICON_NONE;
                 if( bShowEnemies && ENTTYPE(entities[i])==ENTTYPE_CRITTER && entities[i]->pchar->attrCur.fHitPoints>0 && !entIsMyTeamPet(entities[i]))
                {
                    if( character_TargetMatchesType(e->pchar, entities[i]->pchar, kTargetType_Foe, false) )
                    {
                        createZoneIcon(activeIcons, count, ICON_ENEMY, ENTMAT(entities[i]), 0, i, entities[i]);
                        type = ICON_ENEMY;
                    }
                    else
                    {
                        createZoneIcon(activeIcons, count, ICON_PLAYER, ENTMAT(entities[i]), 0, i, entities[i]);
                        type = ICON_TEAMMATE;
                    }
                    count++;
                }

                if( bShowItem && ENTTYPE(entities[i])==ENTTYPE_MISSION_ITEM )
                {
                    int still_active=0;
                      for( j = 0; j < eaSize(&g_ppRemainderLocations); j++ )
                    {
                        if( g_ppRemainderLocations[j]->type == ICON_ITEM && distance3Squared( g_ppRemainderLocations[j]->mat[3], ENTPOS(entities[i]) ) < 25.f )
                        {
                            still_active = 1;
                            g_ppRemainderLocations[j]->found_live_ent = 1;
                        }
                    }
                    if( still_active )
                    {
                        createZoneIcon(activeIcons, count, ICON_ITEM, ENTMAT(entities[i]), 0, i, entities[i]);
                        count++;
                        type = ICON_ITEM;
                    }
                }

                 if( type != ICON_NONE )
                {
                    for( j = 0; j < eaSize(&g_ppRemainderLocations); j++ )
                    {
                        if(  !g_ppRemainderLocations[j]->found_live_ent && g_ppRemainderLocations[j]->type == type && 
                              distance3Squared( g_ppRemainderLocations[j]->mat[3], ENTPOS(entities[i]) ) < 10000.f )
                            g_ppRemainderLocations[j]->found_live_ent = 1;
                    }
                }
            }
        }

        for( i = 0; i < eaSize(&g_ppRemainderLocations) && count < MAX_ACTIVE_ICONS; i++ )
        {
            if( !g_ppRemainderLocations[i]->found_live_ent )
            {
                createZoneIcon(activeIcons, count, g_ppRemainderLocations[i]->type, g_ppRemainderLocations[i]->mat, 0, i+entities_max, NULL);
                count++;
            }
        }
    }

    // server waypoint(s)
    if (serverDestList != NULL) 
    {
        stashGetIterator(serverDestList, &iter);
        while (stashGetNextElement(&iter, &waypoint)  && count < MAX_ACTIVE_ICONS)
        {
            Destination *dest = stashElementGetPointer(waypoint);
            if (dest) 
            {
                createIconFromDestination(activeIcons, dest, count);
                count++;
            }
        }
    }

    // zowie waypoint(s)
    if (zowieDestList != NULL) 
    {
        stashGetIterator(zowieDestList, &iter);
        while (stashGetNextElement(&iter, &waypoint))
        {
            Destination *dest = stashElementGetPointer(waypoint);
            if (dest) 
            {
                createIconFromDestination(&activeIcons[0], dest, count);
                count++;
            }
        }
    }

    // Mission Locations
    if (game_state.mission_map)
        for (i = eaSize(&missionDest)-1; i >= 0 && count < MAX_ACTIVE_ICONS; i--)
            if (createIconFromDestination(activeIcons, missionDest[i], count))
                count++;

    // Mission Keydoors
    if (game_state.mission_map)
        for (i = eaSize(&keydoorDest)-1; i >= 0 && count < MAX_ACTIVE_ICONS; i--)
            if (createIconFromDestination(activeIcons, keydoorDest[i], count))
                count++;

    // contacts
    for( i = eaSize(&contacts)-1; i >= 0 && count < MAX_ACTIVE_ICONS; i-- )
    {
        if (contacts[i]->hasLocation)
        {
            if( createIconFromDestination( activeIcons, &contacts[i]->location, count ) )
                count++;
        }
    }

    // tasks
    for( i = eaSize(&tasks)-1; i >= 0 && count < MAX_ACTIVE_ICONS; i-- )
    {
        // complete tasks aren't allowed to set locations any more
        if (tasks[i]->hasLocation && !tasks[i]->isComplete  && 
            (i == 0 || !tasks[0]->hasLocation || !(tasks[i]->ownerDbid == tasks[0]->ownerDbid && 
            tasks[i]->context == tasks[0]->context && 
            tasks[i]->subhandle == tasks[0]->subhandle)))
        {
            if( createIconFromDestination( activeIcons, &tasks[i]->location, count ) )
                count++;
        }
    }

    // teammates
    if(e->teamup)
    {
        for(i = 0; i < e->teamup->members.count && count < MAX_ACTIVE_ICONS; i++)
        {
            if(e->teamup->members.ids[i]
            && e->teamup->members.ids[i] != e->db_id
                && e->teamup->members.onMapserver[i])
            {
                Entity *eMate = entFromDbId(e->teamup->members.ids[i]) ;
                if(eMate)
                {
                    createZoneIcon(activeIcons, count, ICON_TEAMMATE, ENTMAT(eMate), 0, i, eMate);
                    activeIcons[count].color = getHealthColor(entity_TargetIsInVisionPhase(e, eMate), eMate->pchar->attrCur.fHitPoints/eMate->pchar->attrMax.fHitPoints);
                    count++;
                }
            }
        }
    }

    // leaguemates
    if(e->league)
    {
        for(i = 0; i < e->league->members.count && count < MAX_ACTIVE_ICONS; i++)
        {
            if(e->league->members.ids[i]
            && e->league->members.ids[i] != e->db_id
                && e->league->members.onMapserver[i])
            {
                Entity *eMate = entFromDbId(e->league->members.ids[i]) ;
                if(eMate)
                {
                    createZoneIcon(activeIcons, count, ICON_LEAGUEMATE, ENTMAT(eMate), 0, i, eMate);
                    activeIcons[count].color = getHealthColor(entity_TargetIsInVisionPhase(e, eMate), eMate->pchar->attrCur.fHitPoints/eMate->pchar->attrMax.fHitPoints);
                    activeIcons[count].color &= 0xffffff00;
                    activeIcons[count].color |= 0x00000088;
                    count++;
                }
            }
        }
    }

    //Followers
    if (game_state.mission_map)
    {
        for(i=1; i<entities_max && count < MAX_ACTIVE_ICONS; i++)
        {
            if(entity_state[i]&ENTITY_IN_USE)
            {
                if (entities[i]->showOnMap)
                {
                    createZoneIcon(activeIcons, count, ICON_FOLLOWER, ENTMAT(entities[i]), 0, i, entities[i]);
                    strncpyt(activeIcons[count].name, entities[i]->name, 256);
                    if (entities[i]->pchar)
                        activeIcons[count].color = getHealthColor(entity_TargetIsInVisionPhase(e, entities[i]), entities[i]->pchar->attrCur.fHitPoints/entities[i]->pchar->attrMax.fHitPoints);
                    count++;
                }
            }
        }
    }

    devassert(count <= MAX_ACTIVE_ICONS);
    processIcons( activeIcons, count, TRUE );
    activeIconCount = count;
}


// master control function that fills out the icon lists for the different map types
//
void miniMap( int current_map_type )
{
    static int city_map_loaded = 0;
    int count = 0;
    MapState *mapState = minimap_getMapState();

    mapState->textOn = combobox_idSelected( &comboIcons, ICON_DISPLAYNAMES );
    addActiveIcons();

    switch ( current_map_type )
    {
    case MAP_CITY:
        {
            if( !city_map_loaded ) // load if necessary
            {
                mapperLoadCityInfo();
                miniMapLoad();
                city_map_loaded = TRUE;
            }
            drawMap(NULL, MAP_CITY);

        }break;

    case MAP_ZONE:
        {
            static int last_state = 1;
            static int last_width = -1;
            static int last_height = -1;
            if( !minimap_getCurrentSection(1, ENTPOS(playerPtr())) )
            {
                if (last_state != 0)
                {
                    *mapState= zoneMapState;
                    mapState->zoom = 0;
                    last_state = 0;
                }
                drawMap( gMapName, MAP_ZONE);
            }
            else
            {
                if (last_state == 0)
                {
                    mapState->zoom = 0;
                    last_width = mapState->width;
                    last_height = mapState->height;
                    last_state = 1;
                }
                else if (last_width != mapState->width || last_height != mapState->height)
                {
                    mapState->zoom = 0;
                    last_width = mapState->width;
                    last_height = mapState->height;
                }
                drawMap( gMapName, MAP_MISSION_MAP);
            }
        }break;

    case MAP_OUTDOOR_MISSION:
        {
            drawMap( gMapName, MAP_OUTDOOR_MISSION);
        }break;

    case MAP_MISSION_MAP:
        {
            if( !minimap_getCurrentSection(0, ENTPOS(playerPtr())) )
                mapUnavailable();
            else
                drawMap( gMapName, MAP_MISSION_MAP);
        }break;

    case MAP_OUTDOOR_INDOOR:
        {
            if( !minimap_getCurrentSection(0, ENTPOS(playerPtr())) )
                mapUnavailable();
            else
            {
                if( minimap_getCurrentExtent()->outdoor_poly )
                    drawMap( gMapName, MAP_OUTDOOR_MISSION);
                else
                    drawMap( gMapName, MAP_MISSION_MAP);
            }
        }break;

    case MAP_BASE:
        {
            // Note - when we switch to base raids, make this test based on whether the current base considers me "hostile"
            if(0 && playerPtr()->supergroup_id != g_base.supergroup_id)
                mapUnavailable();
            else
                drawMap( gMapName, MAP_BASE);
        }break;


    }
}

//----------------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------
// CodeButtons
//----------------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------

static  MapType map_type = MAP_ZONE;

#define TAB_INSET       20 // amount to push tab in from edge of window
#define TAB_HT        20
#define BACK_TAB_OFFSET 3  // offset to kee back tab on the bar
#define OVERLAP         5  // amount to overlap tabs
//
//

typedef struct IconChoice
{
    int id;
    char *name;
    char *iconName;
} IconChoice;

IconChoice iconChoices[] = {
    {ICON_STORE,                        "StoreString",                        "map_enticon_store"                    },
    {ICON_CONTACT,                        "ContactOtherString",                "map_enticon_contact"                },
    {ICON_CONTACT_TASK_FORCE,            "ContactTaskForceString",            "map_mission_available"                },
    {ICON_CONTACT_SIGNATURE_STORY,        "ContactSignatureStoryString",        "map_signature_story"                },
    {ICON_CONTACT_HAS_NEW_MISSION,        "ContactHasNewMissionString",        "map_mission_available"                },
    {ICON_CONTACT_ON_MISSION,            "ContactOnMissionString",            "map_mission_in_progress"            },
    {ICON_CONTACT_COMPLETED_MISSION,    "ContactCompletedMissionString",    "map_mission_completed"                },
    {ICON_TRAINER,                        "TrainerString",                    "map_enticon_trainer"                },
    {ICON_NOTORIETY,                    "NotorietyString",                    "map_enticon_notoriety"                },
    {ICON_GATE,                            "GateString",                        "map_enticon_gate_entrysafe"        },
    {ICON_ARENA,                        "ArenaString",                        "map_enticon_arena"                    },
    {ICON_SEWER,                        "Sewer01",                            "map_enticon_gate_arrow_hazard_sewer"},
    {ICON_ARCHITECT,                    "ArchitectString",                    "map_enticon_architect"                },
    {ICON_NEIGHBORHOOD,                    "NeighborhoodString",                "map_enticon_neighborhood"            },
    {ICON_TEAMMATE,                        "TeammateString",                    "map_enticon_you_b"                    },
    {ICON_LEAGUEMATE,                    "LeaguemateString",                    "map_enticon_you_b"                    },
    {ICON_TRAIN_STATION,                "TrainString",                        "map_enticon_train"                    },
    {ICON_HOSPITAL,                        "HospitalString",                    "map_enticon_hospital"                },
    {ICON_MISSION,                        "MissionString",                    "map_enticon_mission_c"                },
    {ICON_THUMBTACK,                    "ThumbtackString",                    "map_enticon_pushpin"                },    
    {ICON_DISPLAYNAMES,                    "TextOnString",                        "white"                                },    
    {ICON_SERVER,                        "SpecialString",                    "white"                                },
    {ICON_ZOWIE,                        "MissionObjective",                    "white"                                },    // FIXME FIXME - change this when we get the correct zowie icon
    {ICON_PRECINCT,                        "PrecinctString",                    "map_enticon_precinct_04"            },
    {ICON_LOUNGE,                        "LoungeString",                        "map_enticon_pushpin"                },
};

static void init_map_options()
{
    int i, count;
    opt_wd = 250;

    combocheckbox_init( &comboIcons, 2*R10-opt_wd, 25, 10, opt_wd, opt_wd, 20, 20+14*ARRAY_SIZE(iconChoices)+1, "Options...",    WDW_MAP, 1 , 0);

    //************
    // set up the UI
    //************

    count = ARRAY_SIZE(iconChoices);
    for( i = 0; i < count; i++ )
    {
        combocheckbox_add( &comboIcons, mapOptionGet(iconChoices[i].id), atlasLoadTexture(iconChoices[i].iconName), textStd(iconChoices[i].name), iconChoices[i].id );
    }

    if(SAFE_MEMBER(playerPtr(),access_level)==10)
    {
        static char **mapNames = 0;
        missionMapHeader_GetMapNames(&mapNames);
        eaInsert(&mapNames,"None", 0);
        combobox_init( &comboHeaders, 2*R10-100, 25, 10, opt_wd, 20, 20+14*ARRAY_SIZE(iconChoices)+1, mapNames,    WDW_MAP);
    }
}

void handleMapOptions()
{
    combobox_display(&comboIcons);

    if( comboIcons.changed )
    {
        int i, count = ARRAY_SIZE(iconChoices);

        for( i = 0; i < count; i++ )
        {
            int id = iconChoices[i].id;

            mapOptionSet(id, combobox_idSelected(&comboIcons, id));
        }

        sendOptions(0);
    }
    if(s_showAllMaps)
    {
        combobox_display(&comboHeaders);

        if(comboHeaders.changed)
        {
            s_isHeader = !!comboHeaders.currChoice;
            if(s_isHeader)
            {
                s_curHeader = missionMapHeader_Get( comboHeaders.strings[comboHeaders.currChoice] );
            }
        }
    }
}

// also initializes some things that aren't maps!  Bit of a hack to use the revision number...
void init_map_optSettings(void)
{
    int revision;
    bool ran_fixWindowHeightValuesToNewAttachPoints = false;

    revision = optionGet(kUO_MapOptionRevision);

    switch (revision)
    {
    case 0:
        // legacy initialization, pre revision system 
        // should have just defaulted to "on" instead
        if (!mapOptionGet(ICON_INITIALIZED))
        {
            mapOptionSetBitfield(0, ~(1 << ICON_DISPLAYNAMES));
        } 
        else if (!mapOptionGet(ICON_INIT2))
        {
            mapOptionSet(ICON_ARCHITECT, 1);
            mapOptionSet(ICON_ARENA, 1);
            mapOptionSet(ICON_INIT2, 1);
            if (mapOptionGet(ICON_GATE))
                mapOptionSet(ICON_SEWER, 1);
        }

        // initialization for revision 0 to revision 1
        mapOptionSet(ICON_NOTORIETY, 0);
        mapOptionSet(ICON_CONTACT_ON_MISSION, 0);
        if (mapOptionGet(ICON_CONTACT))
        {
            mapOptionSet(ICON_CONTACT, 0);
            mapOptionSet(ICON_TRAINER, 1);
            mapOptionSet(ICON_CONTACT_HAS_NEW_MISSION, 1);
            mapOptionSet(ICON_CONTACT_COMPLETED_MISSION, 1);
        }
        // fall through
    case 1:
        mapOptionSet(ICON_CONTACT_SIGNATURE_STORY, 1);
        mapOptionSet(ICON_CONTACT_TASK_FORCE, 1);
        // fall through
    case 2:
        fixWindowHeightValuesToNewAttachPoints();
        ran_fixWindowHeightValuesToNewAttachPoints = true;
        window_UpdateServerAllWindows();

        // hack to get the tray to translate properly... 
        winDefs[WDW_TRAY].loc.yp += 50;

        {
            Entity *e = playerPtr();
            if (e->pl->tray->mode)
                winDefs[WDW_TRAY].loc.yp += 50;
            if (e->pl->tray->mode_alt2)
                winDefs[WDW_TRAY].loc.yp += 50;
        }
        // fall through
    case 3:
        if (!ran_fixWindowHeightValuesToNewAttachPoints)
        {
            // the definitions that fixWindowHeightValuesToNewAttachPoints() uses
            // have changed since the above code was written.  For characters that
            // ran that function before this code was written, they will need to update here.
            fixInspirationTrayToNewAttachPoint();
        }

        // hack to get the inspiration tray to translate properly...
        fixInspirationTrayToDealWithRealWidthAndHeight();
        // fall through
//    case N:
        // add more initialization here
        // fall through, etc. ...

        // this is last, but within the fallthrough of "old revisions"
        // don't need to run this if you're currently on the newest revision
        // this will send all the set options from this switch to the server
        optionSet(kUO_MapOptionRevision, 4, true);
    }
}
//
//

static int gSelectedTab = 0;

void mapSelectTab(void*data)
{
    MapState *mapState = minimap_getMapState();
    gSelectedTab = (int)data;

    if (gSelectedTab == 0)
        map_type = MAP_CITY;
    else
    {
        if( game_state.mission_map || game_state.alwaysMission )
            map_type = mapState->mission_type;
        else
            map_type = MAP_ZONE;
    }
    mapState->zoom = 0;
}

static int s_newMap = 0;

void automap_init()
{
    MapState *mapState = minimap_getMapState();
    int fog;
    s_newMap = true;
    fog = minimap_mapperInit((game_state.mission_map || game_state.alwaysMission), onArchitectTestMap() || game_state.making_map_images);
    switch (fog)
    {
    case MAP_BASE:                                                        //    base map or raid map
            mapfogInit(mapState->min,mapState->max,1);
            break;
    case MAP_OUTDOOR_MISSION:                                            //    outdoor map
            mapfogInit(mapState->min,mapState->max,0);
            break;
    default:
        {
            if(fog == MAP_CITY)                                            //    static map is fine
                zoneMapState = *mapState;
            miniMapLoad();
            mapfogInit(mapState->min,mapState->max,0);
            resetSeps();
        }
        break;
    }
}

int mapWindow()
{
    float x, y, z, wd, ht, scale;
    int color, bcolor;
    static int init = false;
    static uiTabControl * tc;

    int inmission;

    if ( !window_getDims( WDW_MAP, &x, &y, &z, &wd, &ht, &scale, &color, &bcolor ) )
        return 0;

    if(!init) 
    {
        if (game_state.game_mode != SHOW_LOAD_SCREEN)
        {
            tc = uiTabControlCreate(TabType_Undraggable, mapSelectTab, 0, 0, 0, 0 );
            uiTabControlAdd(tc,"CityString",(int*)0);
            uiTabControlAdd(tc,"ZoneString",(int*)1);
            uiTabControlSelect(tc,(int*)1);
    
            init = true;
        }
        else
        {
            return 0;
        }
    }
    inmission = (game_state.mission_map || game_state.alwaysMission);
    if (s_newMap) 
    {
        init_map_options();
        if (inmission) 
            uiTabControlRename(tc,"MissionString",(int*)1);
        else 
            uiTabControlRename(tc,"ZoneString",(int*)1);

        mapSelectTab((int *)gSelectedTab);
        s_newMap = 0;
        resetSeps();
    }

    drawFrame( PIX3, R10, x, y, z, wd, ht, scale, color, CLR_BLACK );
    miniMap( map_type );

    if( window_getMode(WDW_MAP) == WINDOW_DISPLAYING ) 
        drawTabControl(tc, x+TAB_INSET*scale, y, z, wd - TAB_INSET*2*scale, 15, scale, color, color, TabDirection_Horizontal);

    comboIcons.x = wd/scale - opt_wd;
    comboIcons.y = ht/scale - 25;
    if(SAFE_MEMBER(playerPtr(),access_level)==10)
    {
        comboHeaders.x = wd/scale - opt_wd;
        comboHeaders.y = 0;
    }

    handleMapOptions();

    if( onArchitectTestMap() )
    {
        static HelpButton *pHelp;
         helpButton( &pHelp, x + 20*scale, y + ht - 20*scale, z, scale, "MMShowSpawnKey", 0 );
    }
    return 0;
}

//////////////////////////////////////////////////////////////
//Draw the header information
static int s_translateArchitectColor(ArchitectMapColors color)
{
    switch(color)
    {
        xcase ARCHITECT_COLOR_WHITE:
            return CLR_WHITE;
        xcase ARCHITECT_COLOR_GREEN:
            return CLR_GREEN;
        xcase ARCHITECT_COLOR_BLUE:
            return CLR_BLUE;
        xcase ARCHITECT_COLOR_RED:
            return CLR_RED;
        xdefault:
            return DEFAULT_RGBA;
    }
}

#define AUTOMAP_STANDARD_GAP_SHRINK 1.05
static void automap_drawSubmapFloorComponents(ArchitectMapSubMap * map, F32 x, F32 y, F32 z, F32 wd, F32 ht, F32 sc, F32 useStretch, AtlasTex *zoneMap)
{
    int numComponents = eaSize(&map->components);
    int i;
    for (i = 0; i < numComponents; i++)
    {
        ArchitectMapComponent *component = map->components[i];
        AtlasTex *image = zoneMap ? zoneMap : atlasLoadTexture( component->name );
        if (component->isText || zoneMap || stricmp(image->name, "white"))
        {
            int nPlaces = eaSize(&component->places);
            int k;
            for(k = 0; k < nPlaces; k++)
            {
                ArchitectMapComponentPlace *place = component->places[k];
                F32 scx = 1.f;
                F32 scy = 1.f;
                F32 tx = place->x*useStretch;
                F32 ty = place->y*useStretch;

                if (zoneMap)
                {
                    //    If this is a zone image, and we have the map for it,
                    //    scale the map and center it
                    //    center all other locations as well
                    scx = wd/(zoneMap->width*AUTOMAP_STANDARD_GAP_SHRINK);
                    scy = ht/(zoneMap->height*AUTOMAP_STANDARD_GAP_SHRINK);
                    scx = scy = (MIN(scx, scy));
                    if (component->isText)
                    {
                        //    Since we are shrinking the map a little, we have to adjust the original position as well
                        //    once to scale to the zone map
                        //    once more to match normal item location to map offset (see below behaviour how maps are AUTOMAP_STANDARD_GAP_SHRINK larger)
                        tx /= (AUTOMAP_STANDARD_GAP_SHRINK);
                        ty /= (AUTOMAP_STANDARD_GAP_SHRINK);
                    }
                    tx += (wd-(zoneMap->width*scx))/2;
                    ty += (ht-(zoneMap->height*scy))/2;
                }
                else
                {
                    scx = place->scX?place->scX:component->defaultScX;
                    scx = scx?scx:map->defaultScX;
                    scx *=useStretch;
                    scy = place->scY?place->scY:component->defaultScY;
                    scy = scy?scy:map->defaultScY;
                    scy *=useStretch;
                    if (!component->isText)
                    {
                        scx *= AUTOMAP_STANDARD_GAP_SHRINK;
                        scy *= AUTOMAP_STANDARD_GAP_SHRINK;
                        tx -= image->width/2*scx;
                        ty -= image->width/2*scy;
                    }
                }

                if(component->isText)
                {
                    font_color(s_translateArchitectColor(place->color), s_translateArchitectColor(place->color2));
                    cprntEx( tx+x, ty+y, z+place->z, scx, scy, CENTER_X|CENTER_Y, component->name ); // This is a right arrow U+25B6
                }
                else
                {
                    display_rotated_sprite(image, tx+x, ty+y, z+place->z, sc*scx, sc*scy, s_translateArchitectColor(place->color), place->angle, place->additive);
                }
            }
        }
    }
}
void automap_drawSubmapFloor(ArchitectMapSubMap * map, ArchitectMapSubMap *itemsLoc, ArchitectMapSubMap *spawnsLoc, F32 x, F32 y, F32 z, F32 wd, F32 ht, F32 sc, F32 useStretch)
{
    int nComponents = eaSize(&map->components);
    int i;
    AtlasTex *zoneMapImage = NULL;
    assert(map);
    if (!map)
        return;

    for(i = 0; i < nComponents; i++)
    {
        ArchitectMapComponent *mapComponent = map->components[i];
        U32 zoneImage = (mapComponent->name && strnicmp(mapComponent->name,"map_",4)==0);
        AtlasTex *image = atlasLoadTexture( mapComponent->name );
        if (zoneImage && stricmp( image->name, "white" ))
        {
            zoneMapImage = image;
            break;
        }
    }

    automap_drawSubmapFloorComponents(map, x, y, z, wd, ht, sc, useStretch, zoneMapImage);
    if (itemsLoc)
        automap_drawSubmapFloorComponents(itemsLoc, x, y, z, wd, ht, sc, useStretch, zoneMapImage);
    if (spawnsLoc)
        automap_drawSubmapFloorComponents(spawnsLoc, x, y, z, wd, ht, sc, useStretch, zoneMapImage);
}


void automap_drawMapHeader(ArchitectMapHeader * header, F32 x, F32 y, F32 z, F32 sc, F32 wd, F32 ht)
{
    if(!header)
    {
        return;
    }
    else
    {
        int i;
        int nSections;

        F32 wdStretch = wd/((F32)MINIMAP_BASE_DIMENSION);
        F32 htStretch = ht/((F32)MINIMAP_BASE_DIMENSION);
        F32 useStretch = MIN(wdStretch, htStretch);

        //draw floor
        nSections = eaSize(&header->mapFloors);
        assert(eaSize(&header->mapFloors) == eaSize(&header->mapItems));
        assert(eaSize(&header->mapFloors) == eaSize(&header->mapSpawns));
        for(i = 0; i < nSections; i++)
        {
            ArchitectMapSubMap *map = header->mapFloors[i];
            ArchitectMapSubMap *itemsLoc = NULL;
            ArchitectMapSubMap *spawnsLoc = NULL;
            if (EAINRANGE(i, header->mapItems))    itemsLoc = header->mapItems[i];
            if (EAINRANGE(i, header->mapSpawns)) spawnsLoc = header->mapSpawns[i];
            automap_drawSubmapFloor(map, itemsLoc, spawnsLoc, x, y, z, wd, ht, sc, useStretch);
            x+=wd;
        }
    }
}
