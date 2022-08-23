#include "UI/uiAutomap.h"
#include "UI/uiAutomapFog.h"
#include "render/tex_gen.h"
#include "render/tex.h"
#include "graphics/textureatlas.h"
#include "clientcomm/clientcomm.h"
#include <utilitieslib/network/netio.h>
#include "entity/entVarUpdate.h"
#include "UI/sprite/sprite_base.h"
#include "group/groupnetrecv.h"
#include "player/player.h"
#include "cmdparse/cmdgame.h"
#include <utilitieslib/utils/error.h>
#include "entity/entity.h"
#include <utilitieslib/utils/file.h>
#include "entity/teamCommon.h"
#include "entity/Supergroup.h"
#include "UI/uiMissionReview.h"
#include "entity/entPlayer.h"

typedef struct
{
    U32                *cells_visited;
    int                cells_width,cells_height;
    BasicTexture    *bind;
    U8                *bitmap;
    int                opaque;
    Vec3            map_max;
    Vec3            map_min;
} MapFog;

static MapFog map_fog;


#define CELLFRAC 4
#define CELLSIZE ((game_state.mission_map||game_state.alwaysMission) ? (64.f/CELLFRAC) : (256.f/CELLFRAC))

static void updateFogBitmap()
{
    int        x,y;
    U8        *c;

    if (!map_fog.bitmap || !map_fog.bind || !map_fog.cells_visited )
        return;
    for(y=0;y<map_fog.cells_height;y++)
    {
        c = &map_fog.bitmap[y*map_fog.bind->width*4];
         for(x=0;x<map_fog.cells_width;x++)
        {
            *c++ = 255;
            *c++ = 255;
            *c++ = 0;
            if (!TSTB(map_fog.cells_visited,y*map_fog.cells_width+x))
                *c++ = 255;
            else
                *c++ = 0;
        }
    }
    texGenUpdate(map_fog.bind,map_fog.bitmap);
}

bool mapfogIsVisitedPos(Vec3 pos)
{
    F32        x,z;
    int        i,j,idx,offx,offy;
    Vec3    wpos;

    if (!map_fog.cells_visited)
        return false;
    x = 1 - ((pos[0]-map_fog.map_min[0])/(map_fog.map_max[0] - map_fog.map_min[0]));
    z = (pos[2]-map_fog.map_min[2])/(map_fog.map_max[2] - map_fog.map_min[2]);
    i = x*map_fog.cells_width;
    j = z*map_fog.cells_height;
    for(offy=-4;offy<4;offy++)
    {
        for(offx=-4;offx<4;offx++)
        {
            idx = (j+offy) * map_fog.cells_width + (i+offx);
            if (!(idx >= 0 && idx < map_fog.cells_width * map_fog.cells_height))
                continue;
            wpos[0] = (1-(((F32)i+offx+0.5) / (F32)map_fog.cells_width)) *(map_fog.map_max[0]-map_fog.map_min[0]) + map_fog.map_min[0];
            wpos[1] = pos[1];
            wpos[2] = (((F32)j+offy+0.5) / (F32)map_fog.cells_height) *(map_fog.map_max[2]-map_fog.map_min[2]) + map_fog.map_min[2];
            if (distance3(wpos,pos) < CELLSIZE*2 && TSTB(map_fog.cells_visited,idx))
            {
                return true;
            }
        }
    }

    return false;
}

void setVisitedPos(const Vec3 pos,int net_update)
{
    F32        x,z;
    int        i,j,idx,offx,offy,changed=0;
    Vec3    wpos;

    if (!map_fog.cells_visited)
        return;

    x = 1 - ((pos[0]-map_fog.map_min[0])/(map_fog.map_max[0] - map_fog.map_min[0]));
     z = (pos[2]-map_fog.map_min[2])/(map_fog.map_max[2] - map_fog.map_min[2]);
    i = x*map_fog.cells_width;
    j = z*map_fog.cells_height;

    for(offy=-4;offy<4;offy++)
    {
        for(offx=-4;offx<4;offx++)
        {
            idx = (j+offy) * map_fog.cells_width + (i+offx);
            if (!(idx >= 0 && idx < map_fog.cells_width * map_fog.cells_height))
                continue;

            wpos[0] = (1-(((F32)i+offx+0.5) / (F32)map_fog.cells_width)) *(map_fog.map_max[0]-map_fog.map_min[0]) + map_fog.map_min[0];
            wpos[1] = pos[1];
            wpos[2] = (((F32)j+offy+0.5) / (F32)map_fog.cells_height) *(map_fog.map_max[2]-map_fog.map_min[2]) + map_fog.map_min[2];

            if( (wpos[0] < mapState.min[0] || wpos[0] > mapState.max[0] ||
                wpos[2] < mapState.min[2] || wpos[2] > mapState.max[2]) && 
                !(game_state.mission_map || game_state.alwaysMission)) 
                continue;  // out of bounds

            if (distance3(wpos,pos) < CELLSIZE*2 && !TSTB(map_fog.cells_visited,idx))
            {
                int        save_x,save_y;

                SETB(map_fog.cells_visited,idx);
                changed = 1;
                if (net_update)
                {
                    int frac_width = map_fog.cells_width;

                    save_x = (idx % map_fog.cells_width);
                    save_y = (idx / map_fog.cells_width);
                    if (!(game_state.mission_map || game_state.alwaysMission))
                    {
                        VisitedStaticMap *vsm = automap_getMapStruct(playerPtr()->db_id);
                        int i;
                        save_x /= CELLFRAC;
                        save_y /= CELLFRAC;
                        frac_width /= CELLFRAC;
                        for (i = 0; i < eaSize(&vsm->staticMaps); ++i)
                        {
                            VisitedMap *vm = vsm->staticMaps[i];
                            if (vm->map_id == game_state.base_map_id)
                            {
                                SETB(vm->cells, save_y * frac_width + save_x);
                                automap_updateStaticMap(vm);
                                break;
                            }
                        }
                    }
                    else
                    {
                        START_INPUT_PACKET( pak, CLIENTINP_VISITED_MAP_CELL );
                        pktSendBitsPack(pak, 8, save_y * frac_width + save_x );
                        END_INPUT_PACKET
                    }
                }
            }
        }
    }
    if (changed)
        updateFogBitmap();
}

void mapfogSetVisited()
{
    Entity    *e;
    int        i;

    if (!(e=playerPtr()))
        return;

    setVisitedPos(ENTPOS(e),1);
    if(e->teamup && game_state.mission_map)
    {
        for(i = 0; i < e->teamup->members.count; i++)
        {
            if(e->teamup->members.ids[i] && e->teamup->members.ids[i] != e->db_id && e->teamup->members.onMapserver[i])
            {
                Entity *eMate = entFromDbId(e->teamup->members.ids[i]) ;
                if(eMate)
                {
                    setVisitedPos(ENTPOS(eMate),0);
                }
            }
        }
    }
}

void mapfogSetVisitedCellsFromServer(U32 *cells,int num_cells,int opaque_fog)
{
    int        cell_frac,i,x,y,offx,offy,save_width,save_height;

    automap_init();

    if (!map_fog.cells_visited)
        return;
    if (opaque_fog >= 0)
        map_fog.opaque = opaque_fog;
    if (game_state.mission_map||game_state.alwaysMission)
        cell_frac = 1;
    else
        cell_frac = CELLFRAC;
    save_width = map_fog.cells_width / cell_frac;
    save_height = map_fog.cells_height / cell_frac;
    if (num_cells > save_width*save_height)
        num_cells = save_width*save_height;
    for(i=0;i<num_cells;i++)
    {
        if (!TSTB(cells,i))
            continue;
        x = cell_frac * (i % save_width);
        y = cell_frac * (i / save_width);
        for(offy=0;offy<cell_frac;offy++)
        {
            for(offx=0;offx<cell_frac;offx++)
            {
                SETB(map_fog.cells_visited,(y + offy) * map_fog.cells_width + x + offx);
            }
        }
    }
}

void mapfogInit( Vec3 min, Vec3 max, int base )
{
    map_fog.cells_width        = ABS(max[0]-min[0]) / CELLSIZE;
    map_fog.cells_height    = ABS(max[2]-min[2]) / CELLSIZE;

     if( map_fog.cells_width > 1000 || map_fog.cells_height > 1000 )
    {
        if( isDevelopmentMode() )
        {
            Errorf( "Map is missing extents markers, tried to allocate a map too large." );
        }

        map_fog.cells_width = map_fog.cells_height = 0;
    }

    if( base )
        map_fog.cells_width = map_fog.cells_height = 0;
    
    copyVec3( min, map_fog.map_min );
    copyVec3( max, map_fog.map_max );

    if (map_fog.cells_visited)
        free(map_fog.cells_visited);
    map_fog.cells_visited = calloc((map_fog.cells_width*map_fog.cells_height+31)/8,1);
    free(map_fog.bitmap);
    if (map_fog.bind)
        texGenFree(map_fog.bind);
    map_fog.bitmap = 0;
    map_fog.bind = 0;
}
#include "graphics/font.h"
void mapfogDrawZoneFog(F32 scale,AtlasTex *map,F32 wz,int oudoor_indoor)
{
    int        fog_color = 0x000000c0;

    if( game_state.noMapFog || onArchitectTestMap()  )
        return;

     if (!map_fog.bind) 
    {
        int        bw,bh;

        bw = 1 << log2i(map_fog.cells_width-1);
        bh = 1 << log2i(map_fog.cells_height);
        map_fog.bind = texGenNew(bw,bh,"AutomapFog");
        map_fog.bitmap = calloc(bw*bh*4,1);
        updateFogBitmap();
    }
    {
        F32        zoomx,zoomy,relscalex,relscaley;
         if (map && !oudoor_indoor)
        {
            relscalex = (F32)map->width / map_fog.cells_width;
             relscaley = (F32)map->height / map_fog.cells_height;
             zoomx = scale*mapState.zoom * relscalex;
            zoomy = scale*mapState.zoom * relscaley;

            if (map_fog.opaque)
                fog_color = 0x000000ff;
            display_sprite_tex( map_fog.bind, mapState.xp, mapState.yp, wz+1, zoomx, zoomy, fog_color );
        }
        else
        {
            fog_color = 0x000000ff;
              if( oudoor_indoor )
            {
                   relscalex = map->width/((mapState.width/(map_fog.map_max[0]-map_fog.map_min[0])*map_fog.cells_width));
                 relscaley = map->height/((mapState.height/(map_fog.map_max[2]-map_fog.map_min[2])*map_fog.cells_height));
                   zoomx = mapState.zoom*relscalex*scale;
                   zoomy = mapState.zoom*relscaley*scale;

                display_sprite_tex( map_fog.bind, mapState.xp - (map_fog.map_max[0] - mapState.max[0])/(map_fog.map_max[0] - map_fog.map_min[0])*map_fog.cells_width*zoomx, 
                                               mapState.yp - (mapState.min[2]-map_fog.map_min[2])/(mapState.height)*map->height*scale*mapState.zoom, wz+1, zoomx, zoomy, fog_color );
             }
              else 
            {
                    zoomx = mapState.zoom * CELLSIZE;
                   zoomy = mapState.zoom * CELLSIZE;
                   display_sprite_tex( map_fog.bind, 0, 0, 100, 1.f, 1.f, 0x000000ff );
                display_sprite_tex( map_fog.bind, mapState.xp - (map_fog.map_max[0] - mapState.max[0])*mapState.zoom, 
                    mapState.yp - (mapState.min[2] - map_fog.map_min[2])*mapState.zoom, wz+1, zoomx, zoomy, fog_color );
            }
        }
    }
}

