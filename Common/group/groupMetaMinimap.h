#ifndef GROUPMETAMINIMAP_H
#define GROUPMETAMINIMAP_H

#include <utilitieslib/stdtypes.h>
#include "group/grouputil.h"
///////////////////////
#define MAX_EXTENTS 20

// The current map structure
typedef struct MapState
{
    int        xp;
    int        yp;
    int        width;
    int        height;
    int        textOn;

    unsigned int options; //bitfield

    float    zoom;
    Vec3    max;
    Vec3    min;

    Vec2    pixMin;
    Vec2    pixMax;

    int        mission_type;
    int        architect_flags;

} MapState;

typedef struct MissionExtents
{
    Mat4    mat[40];
    Vec3    vec[40];
    int     num[40];
    int        outdoor[40];
    char*    outdoor_texture[40];
    int     count;
} MissionExtents;

typedef struct Extent
{
    Vec3 min;
    Vec3 max;
    int     outdoor_poly;
    char* outdoor_texture;
}Extent;

///////////////////////

AUTO_STRUCT AST_STARTTOK("") AST_ENDTOK("End");
typedef struct ArchitectMapComponentPlace
{
        F32 x;            AST(NAME("X") DEFAULT(0))
        F32 y;            AST(NAME("Y") DEFAULT(0))
        F32 z;            AST(NAME("Z") DEFAULT(0))
        F32 scX;        AST(NAME("XScale") DEFAULT(0))
        F32 scY;        AST(NAME("YScale") DEFAULT(0))
        F32 angle;        AST(NAME("Angle") DEFAULT(0))
        U32 color;        AST(NAME("Color") DEFAULT(0))
        U32 color2;        AST(NAME("TextBackgroundColor") DEFAULT(0))
        U8 additive:1;    AST(NAME("Additive"))
} ArchitectMapComponentPlace;
#define FillArchitectMapComponentPlace(place, xp, yp, zp, scx, scy, Angle, icolor, Additive)\
    place->x = xp;\
    place->y = yp;\
    place->z = zp;\
    place->scX = scx;\
    place->scY = scy;\
    place->angle = Angle;\
    place->color = icolor;


AUTO_STRUCT AST_STARTTOK("") AST_ENDTOK("End");
typedef struct ArchitectMapComponent
{
    char *name;                                AST(NAME("MapImage"))
    U32 isText;                                AST(NAME("TextNotImage"))
    F32 defaultScX;        AST(NAME("defaultXScale") DEFAULT(0))
    F32 defaultScY;        AST(NAME("defaultYScale") DEFAULT(0))
    ArchitectMapComponentPlace **places;    AST(NAME("ImageLocation"))
} ArchitectMapComponent;

AUTO_STRUCT AST_STARTTOK("") AST_ENDTOK("End");
typedef struct ArchitectMapSubMap
{
    U32 id;                            AST(NAME("Floor"))
    F32 defaultScX;        AST(NAME("defaultXScale") DEFAULT(0))
    F32 defaultScY;        AST(NAME("defaultYScale") DEFAULT(0))
    ArchitectMapComponent **components;    AST(NAME("MapImage"))
} ArchitectMapSubMap;

AUTO_STRUCT;
typedef struct ArchitectMapHeader
{
    char *mapName;                        AST(NAME("Map"))
    ArchitectMapSubMap **mapFloors;        AST(NAME("FloorMap"))
    ArchitectMapSubMap **mapItems;        AST(NAME("ItemMap"))
    ArchitectMapSubMap **mapSpawns;        AST(NAME("SpawnMap"))
} ArchitectMapHeader;

AUTO_ENUM;
typedef enum ArchitectMapType
{
    ARCHITECT_MAP_TYPE_FLOOR,
    ARCHITECT_MAP_TYPE_ITEMS,
    ARCHITECT_MAP_TYPE_SPAWNS,
    ARCHITECT_MAP_TYPE_COUNT,
}ArchitectMapType;
#define ArchitectMapType_toString(type)    StaticDefineIntRevLookup(ArchitectMapType, type)

AUTO_ENUM;
typedef enum ArchitectMapColors
{
    ARCHITECT_COLOR_WHITE,
    ARCHITECT_COLOR_GREEN,
    ARCHITECT_COLOR_BLUE,
    ARCHITECT_COLOR_RED,
    ARCHITECT_COLOR_RGBA,
    ARCHITECT_COLOR_COUNT,
}ArchitectMapColors;
#define ArchitectMapColor_toString(type)    StaticDefineIntRevLookup(ArchitectMapColors, type)

typedef struct MapEncounterSpawn
{
    Mat4 mat;
    int type;  // 1 = generic, 2 = around, 3 = both.
    int start;
    int end;
}MapEncounterSpawn;

// map types
typedef enum
{
    MAP_MISSION_MAP,
    MAP_CITY,
    MAP_ZONE,
    MAP_OUTDOOR_MISSION,
    MAP_OUTDOOR_INDOOR,
    MAP_BASE,
} MapType;

#define MINIMAP_BASE_DIMENSION    255

int minimap_mapperInit(int isMissionMap, int makeImages);
int minimap_getCurrentSection(int usey, const F32 *position);
void minimap_saveHeader(char *filename);
void automap_fillHeader(ArchitectMapHeader * header, char *filename);
int withinCurrentSection( const Mat4 mat, const MapState *map_state );
int minimap_missionExtentsLoader(GroupDefTraverser* traverser);
void minimap_processMissionExtents();

Extent *minimap_getSections();
MapState *minimap_getMapState();
MissionExtents *minimap_getMissionExtents();
int minimap_getSectioncount();
MapEncounterSpawn *** minimap_getSpawnList();
Extent * minimap_getCurrentExtent();

#if (defined(SERVER) || defined(CLIENT)) && !defined(NO_TEXT_PARSER) && !defined(UNITTEST)
#include "AutoGen/groupMetaMinimap_h_ast.h"
#endif

#endif
