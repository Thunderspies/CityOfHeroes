/*\
*
*    sgraid_V2.h/c - Copyright 2008 NC Soft
*        All Rights Reserved
*        Confidential property of NC Soft
*
*    Code to run a supergroup raid - probably hosted in a supergroup base
*
*   Version 2
*
 */

#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/utils/structoldnames.h>
#include <utilitieslib/utils/textparser.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/components/EString.h>
#include "group/group.h"
#include "bases/basedata.h"
#include "bases/bases.h"
#include "container/baseloadsave.h"
#include "bases/baseparse.h"
#include "bases/basetogroup.h"
#include "entity/entity.h"
#include "script/scriptengine.h"
#include "dbcomm/dbcontainer.h"
#include "comm_backend.h"
#include "entity/Supergroup.h"
#include "ai/entaiScript.h"
#include "svr/svr_player.h"
#include "sgraid_V2.h"
#include "gameData/iopdata.h"

Vec3 g_baseraidOffsets[MAX_SGS_IN_RAID + 1];
char raidInfoString[MAX_PATH];

// add more of these by color if and when we add more base raid teams
// This is a bit of a hack.  Four teleport mechanics ....
Vec3 BlueTeleportLocs[MAX_BASE_TELEPORT];
Vec3 RedTeleportLocs[MAX_BASE_TELEPORT];
static int s_blueRaidPortType;
static int s_redRaidPortType;
IOPData BlueAnchors[MAX_SG_IOP];
IOPData RedAnchors[MAX_SG_IOP];
RoomDetail **g_blueGurneys;
BaseRoom **g_blueGurneyRooms;
RoomDetail **g_redGurneys;
BaseRoom **g_redGurneyRooms;

static Supergroup *sgrps[MAX_SGS_IN_RAID];
static BasePortalSet **basePortalSets;
static Vec3 baseraidOffset = { 0.0f, 0.0f, 0.0f };

static BaseDims baseDims[MAX_SGS_IN_RAID];

#define sgid1 g_sgRaidSGIDs[0]
#define sgid2 g_sgRaidSGIDs[1]
#define raidid1 g_sgRaidRaidIDs[0]
#define raidid2 g_sgRaidRaidIDs[1]

// Base loading stuff first

// This routine and the one immediately following are duped and trimmed in baseclientrecv.c.  Take a look over there
// if you fix a bug here.

// We found a portal anchor - process it
static void parseBasePortal(RoomDetail *detail, BaseRoom *room, BasePortalSet *basePortalSet, int which)
{
    Vec3 pos;
    int f;
    RoomDoor *door;
    // f above is the "facing" of this door.  Among other things, it indexes into this table to determine
    // which way to push the door.
    static int offsets[4][2] =
    {
        {  0, -1 },
        {  1,  0 },
        {  0,  1 },
        { -1,  0 },
    };
    // Set to 1 iff door[f][which] needs to be saved.  This could be determined at run time from the contents of
    // roomDefs.  I'm just not that motivated.
    // For now, we just set up east-west connections from the two edges that face each other.
    static int doors_to_save[4][2] =
    {
        { 0, 0 },
        { 1, 0 },
        { 0, 0 },
        { 0, 1 },
    };

    // Determine cardinal facing from a quick study of the top row of the matrix.
    // Crude, but it works.
    if (detail->mat[0][0] > 0.5f)
        f = 0;    // 1
    else if (detail->mat[0][0] < -0.5f)
        f = 2;  // 3
    else if (detail->mat[0][2] > 0.5f)
        f = 1;
    else
        f = 3;

    // save the x and z coords in "room blocks", these units make the most sense when we start drilling.
    addVec3(detail->mat[3], room->pos, pos);
    addVec3(pos, baseraidOffset, pos);
    if (basePortalSet->basePortals[f][0].x == 10000)
    {
        basePortalSet->basePortals[f][0].x = (int) floorf((pos[0] - (float) offsets[f][0]) / BASE_BLOCK_SIZE) + offsets[f][0];
        basePortalSet->basePortals[f][0].z = (int) floorf((pos[2] - (float) offsets[f][1]) / BASE_BLOCK_SIZE) + offsets[f][1];
    }
    else    // should check for [f][1] == 10000.  Failure to do so simply ignores extra portal slots - no major harm done.
    {
        basePortalSet->basePortals[f][1].x = (int) floorf((pos[0] - (float) offsets[f][0]) / BASE_BLOCK_SIZE) + offsets[f][0];
        basePortalSet->basePortals[f][1].z = (int) floorf((pos[2] - (float) offsets[f][1]) / BASE_BLOCK_SIZE) + offsets[f][1];
    }

    if (doors_to_save[f][which])
    {
        door = ParserAllocStruct(sizeof(RoomDoor));
        door->pos[0] = (int) floorf((detail->mat[3][0] - (float) offsets[f][0]) / BASE_BLOCK_SIZE) + offsets[f][0];
        door->pos[1] = (int) floorf((detail->mat[3][2] - (float) offsets[f][1]) / BASE_BLOCK_SIZE) + offsets[f][1];
        door->height[0] = 0.0f;
        door->height[1] = 48.0f;
        eaPush(&room->doors,door);
    }

    // Remove the detail in question by moving it off into left field.  28 * 32 is a hair under 1000, so eight times that to the side
    // will be plenty.  If people kick up a fuss, moving this inside the if () clause above will only remove markers for
    // doors that are actually used.
    detail->mat[3][2] += 8000;
    // TO DO mark it destroyed so that we can correctly determine the end of a "destroy all" raid?
    //detail->bDestroyed = 1;
}

// We found an IOP - process it
static void parseBaseIOP(RoomDetail *detail, BaseRoom *room, int baseindex, int which)
{
    Vec3 pos;

    addVec3(detail->mat[3], room->pos, pos);
    addVec3(pos, baseraidOffset, pos);

    if (baseindex)
    {
        copyMat4(detail->mat, RedAnchors[which].mat);
        copyVec3(pos, RedAnchors[which].mat[3]);
        RedAnchors[which].model = detail->info->pchName;
    }
    else
    {
        copyMat4(detail->mat, BlueAnchors[which].mat);
        copyVec3(pos, BlueAnchors[which].mat[3]);
        BlueAnchors[which].model = detail->info->pchName;
    }

    // Remove the detail in question by moving it off into left field.  28 * 32 is a hair under 1000, so eight times that to the side
    // will be plenty.
    detail->mat[3][2] += 8000;
    // TO DO mark it destroyed so that we can correctly determine the end of a "destroy all" raid?
    //detail->bDestroyed = 1;
}

// We found a base entrance teleport widget - process it
static void parseBaseEntrance(RoomDetail *detail, BaseRoom *room, int baseindex, int which)
{
    Vec3 pos;
    
    addVec3(detail->mat[3], room->pos, pos);
    addVec3(pos, baseraidOffset, pos);


    if (baseindex)
    {
        copyVec3(pos, RedTeleportLocs[which]);
    }
    else
    {
        copyVec3(pos, BlueTeleportLocs[which]);
    }
}

// We found a gurney - process it
static void parseBaseGurney(RoomDetail *detail, BaseRoom *room, int baseindex)
{
    if (baseindex)
    {
        eaPush(&g_redGurneys, detail);
        eaPush(&g_redGurneyRooms, room);
    }
    else
    {
        eaPush(&g_blueGurneys, detail);
        eaPush(&g_blueGurneyRooms, room);
    }
}

static void drillHorizontalLine(unsigned char maze[128][128], int z, int x1, int x2)
{
    int x;

    if (x2 < x1)
    {
        x = x1;
        x1 = x2;
        x2 = x;
    }
    for (x = x1; x < x2; x++)
    {
        maze[z][x] = 48;
    }
}

static void drillVerticalLine(unsigned char maze[128][128], int x, int z1, int z2)
{
    int z;

    if (z2 < z1)
    {
        z = z1;
        z1 = z2;
        z2 = z;
    }
    for (z = z1; z < z2; z++)
    {
        maze[z][x] = 48;
    }
}

// Drill a maze between the two pairs of doors, in a room of the specified  dimensions
static void buildMaze(unsigned char maze[128][128], int roomW, int roomH, int door1X, int door1Z, int door2X, int door2Z, int door3X, int door3Z, int door4X, int door4Z)
{
    int x;
    int z;
    int door1facing = -1;
    int door2facing = -1;
    int door3facing = -1;
    int door4facing = -1;
    int xdist;
    //int zdist;
    int uppermax;
    int lowermin;
    int upperx;
    int lowerx;

    //  Doors are outside the room limits, therefore we can determine their facing (i.e. the edge they're on, 
    // by analysis of the out of bounds conditions.
    if (door1X < 0)
    {
        door1X = 0;
        door1facing = 0;
    }
    else if (door1X >= roomW)
    {
        door1X = roomW - 1;
        door1facing = 2;
    }

    if (door2X < 0)
    {
        door2X = 0;
        door2facing = 0;
    }
    else if (door2X >= roomW)
    {
        door2X = roomW - 1;
        door2facing = 2;
    }

    if (door3X < 0)
    {
        door3X = 0;
        door3facing = 0;
    }
    else if (door3X >= roomW)
    {
        door3X = roomW - 1;
        door3facing = 2;
    }

    if (door4X < 0)
    {
        door4X = 0;
        door4facing = 0;
    }
    else if (door4X >= roomW)
    {
        door4X = roomW - 1;
        door4facing = 2;
    }

    // If any of these find the appropriate facing already != -1, we ought to complain.
    if (door1Z < 0)
    {
        door1Z = 0;
        door1facing = 1;
    }
    else if (door1Z >= roomH)
    {
        door1Z = roomH - 1;
        door1facing = 3;
    }

    if (door2Z < 0)
    {
        door2Z = 0;
        door2facing = 1;
    }
    else if (door2Z >= roomH)
    {
        door2Z = roomH - 1;
        door2facing = 3;
    }

    if (door3Z < 0)
    {
        door3Z = 0;
        door3facing = 1;
    }
    else if (door3Z >= roomH)
    {
        door3Z = roomH - 1;
        door3facing = 3;
    }

    if (door4Z < 0)
    {
        door4Z = 0;
        door4facing = 1;
    }
    else if (door4Z >= roomH)
    {
        door4Z = roomH - 1;
        door4facing = 3;
    }

    // If any door couldn't be located, just punt and make the room empty
    if (door1facing == -1 || door2facing == -1 || door3facing == -1 || door4facing == -1)
    {
        for (z = 0; z < roomH; z++)
        {
            for (x = 0; x < roomW; x++)
            {
                maze[z][x] = 48;
            }
        }
        return;
    }

    // Looks good.  Fill the room
    for (z = 0; z < roomH; z++)
    {
        for (x = 0; x < roomW; x++)
        {
            maze[z][x] = 0;
        }
    }

    // distance we have to cross - we assume east-west for now
    xdist = door2X - door1X;

    // uppermax is the furthest down (?) that the upper passage has to extend - larger of door1 and door2's Zs
    uppermax = door1Z > door2Z ? door1Z : door2Z;
    // lowermin is the furthest up (?) that the lower passage has to extend - smaller of door3 and door4's Zs
    lowermin = door3Z < door4Z ? door3Z : door4Z;
    if (uppermax >= lowermin - 1)
    {
        if (door3Z < door2Z)
        {
            upperx = 3; // randInt(xdist / 2) + xdist / 2;
            lowerx = 1; // randInt(xdist / 2);
        }
        else
        {
            upperx = 1; // randInt(xdist / 2);
            lowerx = 3; // randInt(xdist / 2) + xdist / 2;
        }
    }
    else
    {
        upperx = randInt(xdist); //randInt(xdist - 2) + 1;
        lowerx = randInt(xdist); //randInt(xdist - 2) + 1;
    }

    upperx += door1X;
    lowerx += door1X;

    if (door1Z == door2Z)
    {
        drillHorizontalLine(maze, door1Z, door1X, door2X);
    }
    else
    {
        drillHorizontalLine(maze, door1Z, door1X, upperx);
        drillHorizontalLine(maze, door2Z, door2X, upperx);
        drillVerticalLine(maze, upperx, door1Z, door2Z);
        maze[door1Z][upperx] = 48;
        maze[door2Z][upperx] = 48;
    }

    if (door3Z == door4Z)
    {
        drillHorizontalLine(maze, door3Z, door3X, door4X);
    }
    else
    {
        drillHorizontalLine(maze, door3Z, door3X, lowerx);
        drillHorizontalLine(maze, door4Z, door4X, lowerx);
        drillVerticalLine(maze, lowerx, door3Z, door4Z);
        maze[door3Z][lowerx] = 48;
        maze[door4Z][lowerx] = 48;
    }

    maze[door1Z][door1X] = 48;
    maze[door2Z][door2X] = 48;
    maze[door3Z][door3X] = 48;
    maze[door4Z][door4X] = 48;
}

//static void buildRoom(char **eBase, int roomX, int roomZ, int roomW, int roomH, int door1X, int door1Z, int door2X, int door2Z)
static void buildRoom(char **eBase, int roomX, int roomZ, int roomW, int roomH, int door1X, int door1Z, int door2X, int door2Z, int door3X, int door3Z, int door4X, int door4Z)
{
    int x, z;
    unsigned char maze[128][128];
    static int ident = 0;

    char *roomStr = "Room\n\
Id 100000%d\n\
Pos %d, 0, %d\n\
Width %d\n\
Height %d\n\
BlockSize 32\n\
ModTime 1\n\
Info Entrance\n\
Light\n\
Color 8421504\n\
End\n";

    char *blockStr = "Block\n\
Ceiling %d\n\
End\n";

    char *doorStr = "Door\n\
BlockX %d\n\
BlockZ %d\n\
Floor 0\n\
Ceiling 48\n\
End\n";

    estrConcatf(eBase, roomStr, ident++, roomX * 32, roomZ * 32, roomW, roomH);
    estrConcatf(eBase, doorStr, door1X, door1Z);
    estrConcatf(eBase, doorStr, door2X, door2Z);
    estrConcatf(eBase, doorStr, door3X, door3Z);
    estrConcatf(eBase, doorStr, door4X, door4Z);
    buildMaze(maze, roomW, roomH, door1X, door1Z, door2X, door2Z, door3X, door3Z, door4X, door4Z);
    for (z = 0; z < roomH; z++)
    {
        for (x = 0; x < roomW; x++)
        {
            estrConcatf(eBase, blockStr, maze[z][x]);
        }
    }
    estrConcatStaticCharArray(eBase, "END\n");
}

// Warning.  If you ever decide to break the bases stitching by modifying the contents of this,
// make sure you alter doors_to_save[] is parseBasePortal() above
static RoomDef roomDefs[3] =
{
    { 1, 3 },    // TYPE_STRAIGHT
//    { 0, 0 },    // TYPE_NORTHSIDE
//    { 2, 2 },    // TYPE_SOUTHSIDE
};

// Initial setup.  Just tear apart the info string, and do whatever setup is needed.
int sgRaidParseInfoStringAndInit()
{
    int i;
    int raidtype;
    int result;

    result = sscanf(raidInfoString, "R:%i:%i:%i:%i:%i", &sgid1, &sgid2, &raidid1, &raidid2, &raidtype) == 5;
    if (result)
    {
        result = sgid1 > 0 && sgid2 > 0 && raidid1 > 0 && raidid2 > 0 && raidtype >= 0 && raidtype < RAID_TYPE_COUNT;
        if (result)
        {
            g_baseRaidType = raidtype;
        }
    }
    
    // check if g_raidbases is empty  to determine if this is the first time through here.  If so, load the IOP defs.
    // This gets done again later, but a second call to this routine appears to be harmless.
    for (i = 0; i < MAX_BASE_TELEPORT; i++)
    {
        BlueTeleportLocs[i][0] = 1.0e8f;
        RedTeleportLocs[i][0] = 1.0e8f;
    }

    return result;
}

// Phase one of a base load.  Get the string, convert to a base, and determine the dimensions.
int sgRaidLoadPhase1(int index)
{
    int i, j;
    int x, z;
    int minx, minz, maxx, maxz;
    int px, pz;
    Base *base;
    int sgid = g_sgRaidSGIDs[index];
    char *map_str = baseLoadMapFromDb(sgid, 0); // TODO: lock the base from editing?
    char *map_data;
    BaseRoom *room;
    RoomDoor *door;

    static int id = 0;

    map_data = NULL;
    estrCreate(&map_data);
    estrConcatCharString(&map_data, map_str);

    devassert(index < MAX_SGS_IN_RAID);
    dbSyncContainerRequest(CONTAINER_SUPERGROUPS, sgid, CONTAINER_CMD_TEMPLOAD, 0 );
    sgrps[index] = sgrpFromDbId(sgid);
    devassert(sgrps[index]);
    if ((unsigned int) (index + 1) > g_sgRaidNumSgInRaid)
    {
        g_sgRaidNumSgInRaid = index + 1;
    }

    devassert(map_data[0]); // TODO: handle this gracefully
    g_base.roomBlockSize = BASE_BLOCK_SIZE; // TODO: remove this hack (for roomDoorwayHeight() in bases.c)
    base = calloc(1, sizeof(*base));
    devassert(base);
    baseFromStr(map_data, base);
    // TODO: abort if a base is broken

    // Set the sgid of this base to the teamid of the team that considers this to be home base.
    base->supergroup_id = g_sgRaidRaidIDs[index];

    for(i = eaSize(&base->rooms) - 1; i >= 0; --i)
    {
        for(j = eaSize(&base->rooms[i]->details) - 1; j >= 0; --j)
        {
            base->rooms[i]->details[j]->iAuxAttachedID = 0; // Aux will be reattached during baseToDefs
        }
    }
    baseResetIdsForBaseRaid(base, id);
    id = base->curr_id + 1;

    base->data_str = map_data;
    base->full.unpack_size = (U32)strlen(map_data);

    eaPush(&g_raidbases, base);

    minx = 100000;
    minz = 100000;
    maxx = -100000;
    maxz = -100000;
    for (i = eaSize(&base->rooms) - 1; i >= 0; i--)
    {
        room = base->rooms[i];
        px = (int) (room->pos[0] + 1.0f) / BASE_BLOCK_SIZE;
        pz = (int) (room->pos[2] + 1.0f) / BASE_BLOCK_SIZE;
        for (x = 0; x < room->size[0]; x++)
        {
            for (z = 0; z < room->size[1]; z++)
            {
                float *heights = room->blocks[z * room->size[0] + x]->height;
                if (heights[0] < heights[1])
                {
                    checkMinMax(px + x, pz + z);
                }
            }
        }
        for (j = eaSize(&room->doors) - 1; j >= 0; j--)
        {
            door = room->doors[j];
            checkMinMax(px + door->pos[0], pz + door->pos[1]);
        }
    }

    baseDims[index].minx = minx;
    baseDims[index].minz = minz;
    baseDims[index].maxx = maxx;
    baseDims[index].maxz = maxz; 

    return 1;
}

int sgRaidLoadPhase2(int index)
{
    int i, j, k;
    int id;
    float x, y, z;
    Base *base = g_raidbases[index];
    BasePortalSet *basePortalSet;
    Mat4 mat;
    Entity *e;

    devassert(base);

    if (index == 0)
    {
        x = (float) (32 * BASE_BLOCK_SIZE);
        y = 0.0f;
        z = (float) (32 * BASE_BLOCK_SIZE);
    }
    else
    {
        // What's the 8 here?  6 + 1 + 1.
        // 6 for the actual room width, 1 each for the two doors.
        x = (float) ((32 + baseDims[0].maxx - baseDims[1].minx + 8) * BASE_BLOCK_SIZE);
        // TODO: adjust x based on an optional d:n parameter to allow bigger "stitch zones" for PvP
        y = 0.0f;
        // TODO: adjust z to get better linup of the doors
        z = (float) (32 * BASE_BLOCK_SIZE);
    }
    id = g_sgRaidRaidIDs[index];

    setVec3(baseraidOffset, x, y, z); // TODO: use a marker from the base map instead
    copyVec3(baseraidOffset, g_baseraidOffsets[index]);

    basePortalSet = calloc(1, sizeof(*basePortalSet));
    devassert(basePortalSet);
    for (i = 0; i < 4; i++)
    {
        basePortalSet->basePortals[i][0].x = 10000;
        basePortalSet->basePortals[i][1].x = 10000;
    }

    k = 0;
    // This loop, and the first two routines called from within it are duplicated (somewhat trimmed) over in 
    // baseclientrecv.c, to duplicate this functionality on the client.  If you alter this loop or either of
    // the two routines, take a look over there.
    // scan through the details, looking for things we care about
    for(i = eaSize(&base->rooms) - 1; i >= 0; --i)
    {
        for(j = eaSize(&base->rooms[i]->details) - 1; j >= 0; --j)
        {
            // Look for portal anchors, ...
            // TODO - change this to use the real base portal item
            if (stricmp(base->rooms[i]->details[j]->info->pchName, "Base_Deco_PictureFrame2") == 0)
            {
                parseBasePortal(base->rooms[i]->details[j], base->rooms[i], basePortalSet, index);
            }
            // IOP's, ...
            if (k < MAX_SG_IOP && (base->rooms[i]->details[j]->info->eFunction == kDetailFunction_ItemOfPower || 
                                base->rooms[i]->details[j]->info->eFunction == kDetailFunction_ItemOfPowerMount))
            {
                if (g_baseRaidType == RAID_TYPE_IOP || g_baseRaidType == RAID_TYPE_CTF)
                {
                    parseBaseIOP(base->rooms[i]->details[j], base->rooms[i], index, k++);
                }
            }
            // Entrances, ...
            if (stricmp(base->rooms[i]->details[j]->info->pchName, "Entrance") == 0)
            {
                parseBaseEntrance(base->rooms[i]->details[j], base->rooms[i], index, BASE_TELEPORT_ENTRANCE);
            }
            // Raid porters, ...
            if (base->rooms[i]->details[j]->info->eFunction == kDetailFunction_RaidTeleporter)
            {
                parseBaseEntrance(base->rooms[i]->details[j], base->rooms[i], index, BASE_TELEPORT_RAIDPORT);
                if (index)
                {
                    s_redRaidPortType = stricmp(base->rooms[i]->details[j]->info->pchName, "Base_BasicTeamTelepad_Tech") == 0 ? RAID_PORT_TYPE_TECH : RAID_PORT_TYPE_ARCANE;
                }
                else
                {
                    s_blueRaidPortType = stricmp(base->rooms[i]->details[j]->info->pchName, "Base_BasicTeamTelepad_Tech") == 0 ? RAID_PORT_TYPE_TECH : RAID_PORT_TYPE_ARCANE;
                }
            }
            // and Gurnies.
            if (base->rooms[i]->details[j]->info->eFunction == kDetailFunction_Medivac)
            {
                parseBaseGurney(base->rooms[i]->details[j], base->rooms[i], index);
            }
        }
    }

    baseToDefsWithOffset(base, index, baseraidOffset);

    // slide everything over
    setVec3(base->ref->mat[3], x, y, z); // TODO: use a marker from the base map instead
    for(i = eaSize(&base->rooms)-1; i >= 0; --i)
    {
        for(j = eaSize(&base->rooms[i]->details)-1; j >= 0; --j)
        {
            if (e = base->rooms[i]->details[j]->e)
            {
                copyMat4(e->fork, mat);
                addVec3(base->ref->mat[3], mat[3], mat[3]);
                entUpdateMat4Instantaneous(e, mat);

                // Set the gang ID for everything except the raid porter and the medivac - those must stay indestructible
                if (base->rooms[i]->details[j]->info->eFunction != kDetailFunction_RaidTeleporter && 
                    base->rooms[i]->details[j]->info->eFunction != kDetailFunction_Medivac)
                {
                    aiScriptSetGang(e, id);
                }
            }
            addVec3(base->ref->mat[3], base->rooms[i]->details[j]->mat[3], base->rooms[i]->details[j]->mat[3]);
        }
    }

    eaPush(&basePortalSets, basePortalSet);
    // TODO - make this a bit more forgiving
    for (i = 0; i < 4; i++)
    {
        if (basePortalSet->basePortals[i][1].x == 10000)
        {
            return 0;
        }
    }
    return 1;
}

// Set up the stitching between the bases
// This has been through several iterations.  Until we lock down what works, I'm leaving a ton of old code in here as reference material.
// TODO add a couple of side passages if a c:n option says they are wanted
int sgRaidSetupNoMansLand()
{
    char *eBase;
    Base *base;

    int tmp;

    int room1X, room1Z, room1W, room1H;        // x, z, w, h for five rooms
    //int room2X, room2Z, room2W, room2H;
    //int room3X, room3Z, room3W, room3H;
    //int room4X, room4Z, room4W, room4H;
    //int room5X, room5Z, room5W, room5H;

    int door1X, door1Z;                        // x, z for 8 doors
    int door2X, door2Z;
    int door3X, door3Z;
    int door4X, door4Z;
    //int door5X, door5Z;
    //int door6X, door6Z;
    //int door7X, door7Z;
    //int door8X, door8Z;

    char *baseHeader = "\n\
Base\n\
Version 1\n\
RoomBlockSize 32\n\
SupergroupId 0\n\
EnergyProduced 1\n\
EnergyConsumed 0\n\
ControlProduced 1\n\
ControlConsumed 0\n";

    // Init
    eBase = NULL;
    estrCreate(&eBase);
    estrConcatCharString(&eBase, baseHeader);

#if 0
    // This is how it used to be for straight, north, south
    door1X = basePortalSets[0]->basePortals[roomDefs[TYPE_STRAIGHT].base1Door].x;
    door1Z = basePortalSets[0]->basePortals[roomDefs[TYPE_STRAIGHT].base1Door].z;
    door2X = basePortalSets[1]->basePortals[roomDefs[TYPE_STRAIGHT].base2Door].x;
    door2Z = basePortalSets[1]->basePortals[roomDefs[TYPE_STRAIGHT].base2Door].z;
    door3X = basePortalSets[0]->basePortals[roomDefs[TYPE_NORTHSIDE].base1Door].x;
    door3Z = basePortalSets[0]->basePortals[roomDefs[TYPE_NORTHSIDE].base1Door].z;
    door4X = basePortalSets[1]->basePortals[roomDefs[TYPE_NORTHSIDE].base2Door].x;
    door4Z = basePortalSets[1]->basePortals[roomDefs[TYPE_NORTHSIDE].base2Door].z;
    door5X = basePortalSets[0]->basePortals[roomDefs[TYPE_SOUTHSIDE].base1Door].x;
    door5Z = basePortalSets[0]->basePortals[roomDefs[TYPE_SOUTHSIDE].base1Door].z;
    door6X = basePortalSets[1]->basePortals[roomDefs[TYPE_SOUTHSIDE].base2Door].x;
    door6Z = basePortalSets[1]->basePortals[roomDefs[TYPE_SOUTHSIDE].base2Door].z;
#endif

    door1X = basePortalSets[0]->basePortals[roomDefs[TYPE_STRAIGHT].base1Door][0].x;
    door1Z = basePortalSets[0]->basePortals[roomDefs[TYPE_STRAIGHT].base1Door][0].z;
    door2X = basePortalSets[1]->basePortals[roomDefs[TYPE_STRAIGHT].base2Door][0].x;
    door2Z = basePortalSets[1]->basePortals[roomDefs[TYPE_STRAIGHT].base2Door][0].z;
    door3X = basePortalSets[0]->basePortals[roomDefs[TYPE_STRAIGHT].base1Door][1].x;
    door3Z = basePortalSets[0]->basePortals[roomDefs[TYPE_STRAIGHT].base1Door][1].z;
    door4X = basePortalSets[1]->basePortals[roomDefs[TYPE_STRAIGHT].base2Door][1].x;
    door4Z = basePortalSets[1]->basePortals[roomDefs[TYPE_STRAIGHT].base2Door][1].z;

    if (door1Z > door3Z)
    {
        tmp = door1Z;
        door1Z = door3Z;
        door3Z = tmp;
    }
    if (door2Z > door4Z)
    {
        tmp = door2Z;
        door2Z = door4Z;
        door4Z = tmp;
    }
    devassertmsg(door1X == door3X, "left base has bad doors");
    devassertmsg(door2X == door4X, "right base has bad doors");
    devassertmsg(door1X < door2X - 4, "bases too close together");

    // Straight
    // Room 1 - straight through
    // Set x bounds on the basis of the immediate conecting doors: 1 and 2
    if (door1X < door2X)
    {
        room1W = door2X - door1X - 1;
        room1X = door1X + 1;
    }
    else
    {
        room1W = door1X - door2X - 1;    // [1] untested
        room1X = door2X + 1;
    }

#if 0
    // Old style
    // Intent here is to make this room as tall as it can be.  Doors 3 thru 6 are the north and south doors, so we take the min of
    // the two north ones as the north limit, and max of the two south as the south limit.
    // set z position to the smaller of doors 3 and 4 z
    room1Z = door3Z < door4Z ? door3Z + 1 : door4Z + 1;

    // figure height as distance from room's z to greater of doors 5 and 6 z
    room1H = door5Z > door6Z ? door5Z - room1Z : door6Z - room1Z;
#endif

    // We make this room as tall as it needs to be to hold both passageways, and then a little bit more besides.
    // set z position to the smaller of doors 1 and 2 z
    room1Z = door1Z < door2Z ? door1Z - 1 : door2Z - 1;

    // figure height as distance from room's z to greater of doors 3 and 4 z
    room1H = door3Z > door4Z ? door3Z - room1Z + 1 : door4Z - room1Z + 1;

    // build the room, place doors, and create the maze array
    buildRoom(&eBase, room1X, room1Z, room1W, room1H, door1X - room1X, door1Z - room1Z, door2X - room1X, door2Z - room1Z, door3X - room1X, door3Z - room1Z, door4X - room1X, door4Z - room1Z);

#if 0
    // Keep this around.  We might figure how to use it some day.
    // Northside
    // two north extents are the same
    if (door3Z == door4Z)
    {
        room2Z = door3Z - 8;
        room2H = 8;
        if (door3X < door4X)
        {
            room2X = door3X;
            room2W = door4X - door3X + 1;
        }
        else
        {
            room2X = door4X;    // untested
            room2W = door3X - door4X + 1;
        }
        buildRoom(&eBase, room2X, room2Z, room2W, room2H, door3X - room2X, door3Z - room2Z, door4X - room2X, door4Z - room2Z);
    }
    else if (door3Z < door4Z)
    {
        room2Z = door3Z - 8;    // [1] untested
        room2H = 8;
        room3Z = door3Z - 8;
        room3H = 8 + door4Z - door3Z;
        door7Z = door3Z - 2 - randInt(6);
        if (door3X < door4X)
        {
            door7X = door2X;    // [1] untested
            room2X = door3X;
            room2W = door7X - door3X;
            room3X = door7X + 1;
            room3W = 8;
        }
        else
        {
            door7X = door1X;    // [1] untested - and screwed up. :/
            room2X = door7X - 8;
            room2W = 8;
            room3X = door4X;
            room3W = door4X - door7X;
        }
        buildRoom(&eBase, room2X, room2Z, room2W, room2H, door3X - room2X, door3Z - room2Z, door7X - room2X, door7Z - room2Z);
        buildRoom(&eBase, room3X, room3Z, room3W, room3H, door4X - room3X, door4Z - room3Z, door7X - room3X, door7Z - room3Z);
    }
    else // (door3Z > door4Z)
    {
        room2Z = door4Z - 8;    // [1] untested *
        room2H = 8;
        room3Z = door4Z - 8;
        room3H = 8 + door3Z - door4Z;
        door7Z = door4Z - 2 - randInt(6);
        if (door3X < door4X)
        {
            door7X = door1X;    // [1] untested *
            room2X = door7X + 1;
            room2W = door4X - door7X;
            room3X = door7X - 8;
            room3W = 8;
        }
        else
        {
            door7X = door2X;    // [1] untested
            room2X = door4X;
            room2W = door7X - door4X;
            room3X = door7X + 1;
            room3W = 8;
        }
        buildRoom(&eBase, room2X, room2Z, room2W, room2H, door4X - room2X, door4Z - room2Z, door7X - room2X, door7Z - room2Z); // ??
        buildRoom(&eBase, room3X, room3Z, room3W, room3H, door3X - room3X, door3Z - room3Z, door7X - room3X, door7Z - room3Z); // ??
    }

    // Southside
    // two south extents are the same
    if (door5Z == door6Z)
    {
        room4Z = door5Z + 1;    // [1] untested
        room4H = 8;
        if (door5X < door6X)
        {
            room4X = door5X;    // [1] untested
            room4W = door6X - door5X + 1;
        }
        else
        {
            room4X = door6X;    // [1] untested
            room4W = door5X - door6X + 1;
        }
        buildRoom(&eBase, room4X, room4Z, room4W, room4H, door5X - room4X, door5Z - room4Z, door6X - room4X, door6Z - room4Z);
    }
    else if (door5Z < door6Z)
    {
        room4Z = door6Z + 1;    // [1] untested
        room4H = 8;
        room5Z = door5Z + 1;
        room5H = 8 + door6Z - door5Z;
        door8Z = door6Z + 2 + randInt(6);
        if (door5X < door6X)
        {
            door8X = door1X;    // [1] untested
            room4X = door1X + 1;
            room4W = door6X - door8X;
            room5X = door8X - 8;
            room5W = 8;
        }
        else
        {
            door8X = door2X;    // [1] untested
            room4X = door6X;
            room4W = door8X - door6X;
            room5X = door8X + 1;
            room5W = 8;
        }
        buildRoom(&eBase, room4X, room4Z, room4W, room4H, door6X - room4X, door6Z - room4Z, door8X - room4X, door8Z - room4Z);
        buildRoom(&eBase, room5X, room5Z, room5W, room5H, door5X - room5X, door5Z - room5Z, door8X - room5X, door8Z - room5Z);
    }
    else // (door5Z > door6Z)
    {
        room4Z = door5Z + 1;    // [1] untested *
        room4H = 8;
        room5Z = door6Z + 1;
        room5H = 8 + door5Z - door6Z;
        door8Z = door5Z + 2 + randInt(6);
        if (door5X < door6X)
        {
            door8X = door2X;    // [1] untested *
            room4X = door5X;
            room4W = door8X - door5X;
            room5X = door8X + 1;
            room5W = 8;
        }
        else
        {
            door8X = door1X;    // [1] untested
            room4X = door8X + 1;
            room4W = door8X - door5X;
            room5X = door8X - 9;
            room5W = 8;
        }
        buildRoom(&eBase, room4X, room4Z, room4W, room4H, door5X - room4X, door5Z - room4Z, door8X - room4X, door8Z - room4Z); // ??
        buildRoom(&eBase, room5X, room5Z, room5W, room5H, door6X - room5X, door6Z - room5Z, door8X - room5X, door8Z - room5Z); // ??
    }
#endif

    estrConcatStaticCharArray(&eBase, "END\n");
    // Finish
    setVec3(baseraidOffset, 0.0f, 0.0f, 0.0f);
    copyVec3(baseraidOffset, g_baseraidOffsets[eaSize(&g_raidbases)]);

    g_base.roomBlockSize = BASE_BLOCK_SIZE; // TODO: remove this hack (for roomDoorwayHeight() in bases.c)
    base = calloc(1,sizeof(*base));
    assert(base);
    baseFromStr(eBase, base);
    // TODO: abort if a base is broken

    base->supergroup_id = 0;
    baseToDefs(base, eaSize(&g_raidbases));

    base->data_str = eBase;
    base->full.unpack_size = (U32)strlen(eBase);

    eaPush(&g_raidbases, base);
    // Don't destroy eBase, we need to keep the string around so we can send the base to the clients
    //estrDestroy(&eBase);
    return 1;
}

void sgRaidDumpMapToConsole()
{
    int x, z;

    sgRaidConvertRaidBasesToMapData();
    printf("\n");
    for (z = 0; z <= g_sgRaidMapData.maxz - g_sgRaidMapData.minz; z++)
    {
        for (x = 0; x < g_sgRaidMapData.maxx - g_sgRaidMapData.minx; x++)
        {
            printf("%c", ".BRN"[g_sgRaidMapData.mapArray[x][z]]);
        }
        printf("\n");
    }
    printf("\n");
}

int sgRaidGetBaseRaidSGID(unsigned int index)
{
    devassert(index < MAX_SGS_IN_RAID && index < g_sgRaidNumSgInRaid);
    return g_sgRaidSGIDs[index];
}

int sgRaidGetBaseRaidRaidID(unsigned int index)
{
    devassert(index < MAX_SGS_IN_RAID && index < g_sgRaidNumSgInRaid);
    return g_sgRaidRaidIDs[index];
}

char *sgRaidGetBaseRaidSGName(unsigned int index)
{
    devassert(index < MAX_SGS_IN_RAID && index < g_sgRaidNumSgInRaid);
    return sgrps[index]->name;
}

// Teleport locations come in matched pairs - this sees if there's one close to the start location, if so it fills in the target with the dest location
// If we ever put > 2 bases on a map, we will need to:
// 1. Design how the interaction between teleports is to work
// 2. Adjust this routine to match.
int sgRaidFindOtherEndOfBaseRaidTeleportPair(Vec3 start, Vec3 target, int index)
{
    float distToBlue;
    float distToRed;

    if (BlueTeleportLocs[index][0] > 1.0e7f || RedTeleportLocs[index][0] > 1.0e7f)
    {
        return 0;
    }

    distToBlue = distance3SquaredXZ(start, BlueTeleportLocs[index]);
    distToRed = distance3SquaredXZ(start, RedTeleportLocs[index]);

    if (distToBlue < distToRed)
    {
        copyVec3(RedTeleportLocs[index], target);
    }
    else
    {
        copyVec3(BlueTeleportLocs[index], target);
    }
    return 1;
}

void sgRaidGetRaidPortOffset(Vec3 target, Vec3 offset)
{
    int portType;
    int i;
    float distToBlue;
    float distToRed;

    if (BlueTeleportLocs[BASE_TELEPORT_RAIDPORT][0] > 1.0e7f || RedTeleportLocs[BASE_TELEPORT_RAIDPORT][0] > 1.0e7f)
    {
        // This should never happen ...
        offset[0] = 10.0f;
        offset[1] = 2.0f;
        offset[2] = 10.0f;
    }
    else
    {
        distToBlue = distance3SquaredXZ(target, BlueTeleportLocs[BASE_TELEPORT_RAIDPORT]);
        distToRed = distance3SquaredXZ(target, RedTeleportLocs[BASE_TELEPORT_RAIDPORT]);

        portType = distToBlue < distToRed ? s_blueRaidPortType : s_redRaidPortType;
        if (portType == RAID_PORT_TYPE_TECH)
        {
            offset[0] = TECH_PORT_OFFSET_XZ;
            offset[1] = TECH_PORT_OFFSET_Y;
            offset[2] = TECH_PORT_OFFSET_XZ;
        }
        else
        {
            offset[0] = ARCANE_PORT_OFFSET_XZ;
            offset[1] = ARCANE_PORT_OFFSET_Y;
            offset[2] = ARCANE_PORT_OFFSET_XZ;
        }
        i = randInt(4);
        if (i & 1)
        {
            offset[0] = -offset[0];
        }
        if (i & 2)
        {
            offset[2] = -offset[2];
        }
    }
}

int sgRaidHasBaseGurney(Entity *e)
{
    unsigned int i;

    for (i = 0; i < g_sgRaidNumSgInRaid && e->supergroup_id != g_sgRaidRaidIDs[i]; i++)
        ;
    devassert(i < g_sgRaidNumSgInRaid);
    // Assume 2 for now, if we ever get three or more in here, the whole blue/red thing is gonna have to go anyway.
    if (i == 0)
    {
        return eaSize(&g_blueGurneys) != 0;
    }
    else
    {
        return eaSize(&g_redGurneys) != 0;
    }
}

// SG raid base gurney code.  Can't use ths standard code, since it has no concept of the alignment
// of gurnies.  So we first check which SG the Entity is affiliated with, and attempt to use one
// of the gurnies in that base.
void sgRaidDoGurneyXfer(Entity *e)
{
    unsigned int i;
    RoomDetail *theGurney;
    BaseRoom *theHospital;
    Vec3 pos;

    for (i = 0; i < g_sgRaidNumSgInRaid && e->supergroup_id != g_sgRaidRaidIDs[i]; i++)
        ;
    devassert(i < g_sgRaidNumSgInRaid);
    // Assume 2 for now, if we ever get three or more in here, the whole blue/red thing is gonna have to go anyway.
    // When that happens, g_blueGurnies and g_redGurnies will become an array of earrays, and we'll use i to index
    // into it.
    if (i == 0)
    {
        // puke if gurney array and gurneyRoom array don't match in size
        devassert(eaSize(&g_blueGurneys) == eaSize(&g_blueGurneyRooms));
        if (eaSize(&g_blueGurneys) < 1)
        {
            // No gurnies left.  You're outa luck.
            return;
        }
        // Pick one at random, and get the gurney and room
        i = randInt(eaSize(&g_blueGurneys));
        theGurney = g_blueGurneys[i];
        theHospital = g_blueGurneyRooms[i];
    }
    else
    {
        devassert(eaSize(&g_redGurneys) == eaSize(&g_redGurneyRooms));
        if (eaSize(&g_redGurneys) < 1)
        {
            return;
        }
        i = randInt(eaSize(&g_redGurneys));
        theGurney = g_redGurneys[i];
        theHospital = g_redGurneyRooms[i];
    }
    // Rez the character
    e->pchar->attrCur.fHitPoints = e->pchar->attrMax.fHitPoints;
    e->pchar->attrCur.fEndurance = e->pchar->attrMax.fEndurance;
    // Figure when the gurney is
    addVec3(theGurney->mat[3], theHospital->pos, pos);
    // and send the character there.
    entUpdatePosInstantaneous(e, pos);
    // No need to send pets - they're dead by now.
}

// This is a bit of a cheat, we just peek over into svr_player.c and grab player_count from there
int sgRaidGetPlayerCount()
{
    return player_count;
}

int sgRaidGetBaseRaidIOPData(unsigned int team, unsigned int index, const char **model, Mat4 mat)
{
    IOPData *iopData;

    if (team >= g_sgRaidNumSgInRaid || index >= MAX_SG_IOP) 
    {
        return 0;
    }
    iopData = team ? &RedAnchors[index] : &BlueAnchors[index];
    if (iopData->model == NULL || iopData->model[0] == 0)
    {
        return 0;
    }
    if (model)
    {
        *model = iopData->model;
    }
    if (mat)
    {
        copyMat4(iopData->mat, mat);
    }
    return 1;
}
    
// If this fails for a new script, you probably didn't add an entry to scripttable.c
void sgRaidInitBaseRaidScript()
{
    const ScriptDef *pScriptDef;
    char *scriptName = NULL;

    switch (g_baseRaidType)
    {
    default:
        devassertmsg(0, "Invalid script type in initBaseRaidScript()");
    xcase RAID_TYPE_IOP:
        scriptName = "scriptdefs\\IOPRaid.scriptdef";
    xcase RAID_TYPE_FFA:
        scriptName = "scriptdefs\\FFARaid.scriptdef";
    xcase RAID_TYPE_CTF:
        scriptName = "scriptdefs\\CTFRaid.scriptdef";
    xcase RAID_TYPE_MAYHEM:
        scriptName = "scriptdefs\\MayhemRaid.scriptdef";
    }

    pScriptDef = ScriptDefFromFilename(scriptName);

    if (pScriptDef)
    {
        ScriptBeginDef(pScriptDef, SCRIPT_ZONE, NULL, "ZoneScript start", scriptName);
    }
    else
    {
        printf("Unable to find map script %s\n", scriptName);
    }
}
