
#include <stdlib.h>
#include <utilitieslib/stdtypes.h>
#include "bases/baseclientrecv.h"
#include <utilitieslib/utils/mathutil.h>
#include "bases/bases.h"
#include <utilitieslib/network/netio.h>
#include <utilitieslib/network/netcomp.h>
#include "bases/basetogroup.h"
#include <utilitieslib/components/earray.h>
#include "bases/basedata.h"
#include <utilitieslib/utils/textparser.h>
#include "bases/baseparse.h"
#include <utilitieslib/network/bindiff.h>
#include "group/group.h"
#include "group/groupscene.h"
#include "graphics/sun.h"
#include "bases/baseedit.h"
#include <utilitieslib/components/EString.h>
#include "UI/uiAutomap.h"

#include "seq/seq.h"
#include "entity/entity.h"
#include "entity/entclient.h"
#include "clientcomm/clientcomm.h"
#include "entity/entVarUpdate.h"
#include <utilitieslib/network/crypt.h>

int sBadBase = 0;

void baseUpdateEntityLighting(void)
{
    int i;
    for(i=0;i<entities_max;i++)
    {
        if(entities[i] && entity_state[i]&ENTITY_IN_USE && entities[i]->seq)
        {
            setVec3(entities[i]->seq->seqGfxData.light.calc_pos,8e10,8e10,8e10);
            seqResetWorldFxStaticLighting(entities[i]->seq);
        }
    }
}

void setupInfoPointers(void)
{
    int        i;

    for(i=0; i<eaSize(&g_base.rooms); i++)
    {
        if(!g_base.rooms[i]->info)
        {
            g_base.rooms[i]->info = basedata_GetRoomTemplateByName(g_base.rooms[i]->name);

            if(!g_base.rooms[i]->info)
                g_base.rooms[i]->info = basedata_GetRoomTemplateByName("Studio_3x3");
        }
    }
}

// Return 0 on failure
static int baseReceiveAndLoad(Packet *pak, int raidIndex)
{
    int not_diff;
    int len = -1;
    char* str = NULL;
    Base    *base;

    if (raidIndex)
    {
        base = calloc(1,sizeof(*base));
        devassert(base);
        g_base.roomBlockSize = BASE_BLOCK_SIZE; // TODO: remove this hack (for roomDoorwayHeight() in bases.c)
    }
    else
    {
        base = &g_base;
    }

    ParserDestroyStruct(parse_base,base);
    not_diff = pktGetBits(pak,1);
    if (not_diff)
    {
        base->curr_id = pktGetBitsPack(pak,1);
        str = pktGetZipped(pak,&len);
        sBadBase = 0;
    }
    else
    {
        char    *diff;
        int        diff_size;
        U32        crc;
        int same = 0;
                
        crc = pktGetBits(pak,32);
        base->curr_id = pktGetBitsPack(pak,1);
        diff = pktGetZipped(pak,&diff_size);

        cryptAdler32Init();
        if (sBadBase)
            same = 0; //We have a bad base, don't accept any patches
        else if (!base->data_str) 
            same = 1; //Null string, let it try to apply patch-from-null
        else if (cryptAdler32(base->data_str, base->data_len) == crc)
            same = 1; //The crc of what we're patching from matches current, try patch

        if (same)
            len = bindiffApplyPatch(base->data_str,diff,&str);

        if (len == -1 || !same)
        { // If patching didn't work, error out and let the client request a full update
            free(diff);            
            return 0;
        }
        free(diff);
    }
    if (base->data_str)
        free(base->data_str);

    base->data_str = str;
    base->data_len = len;
    if (raidIndex)
    {
        baseFromStr(base->data_str, base);
    }
    else
    {
        baseFromBin(base->data_str, len, base);
    }

    if (raidIndex)
    {
        eaPush(&g_raidbases, base);
        devassert(raidIndex == eaSize(&g_raidbases));
    }

    base->update_time = timerSecondsSince2000();

    return 1;
}

int checkBaseCorrupted(Base *base)
{
    int i;
    for (i = eaSize(&base->rooms) - 1; i >= 0; i--)
    {
        BaseRoom *room = base->rooms[i];
        if (!room || !room->info)
            return 1;
        if (!room->size || !room->blocks)
            return 1; //missing data
        if (room->size[0] * room->size[1] != eaSize(&room->blocks))
            return 1; //bad blocks
        if (room->size[0] != room->info->iWidth && room->size[1] != room->info->iWidth)
            return 1; //bad size
        if (room->size[0] != room->info->iLength && room->size[1] != room->info->iLength)
            return 1; //bad size
    }

    // TODO: add more corruption checks

    // not corrupted
    return 0;
}

// These next three routines are scaled down versions of ones found in sgraid_V2.c, run on the mapserver.
// If you fixc a bug in any of them, you probably want to take a look at their opposite numbers over there.

// We found a portal anchor - process it
static void parseBasePortalOnClient(RoomDetail *detail, BaseRoom *room, int which)
{
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
}

// We found an IOP - process it
static void parseBaseIOPOnClient(RoomDetail *detail)
{
    detail->mat[3][2] += 8000;
}

static void basePostProcessForRaid(int raidIndex)
{
    int i, j;
    int k;
    Base *base = g_raidbases[raidIndex - 1];

    k = 0;
    for(i = eaSize(&base->rooms) - 1; i >= 0; --i)
    {
        for(j = eaSize(&base->rooms[i]->details) - 1; j >= 0; --j)
        {
            // Look for portal anchors, ...
            // TODO - change this to use the real base portal item
            if (stricmp(base->rooms[i]->details[j]->info->pchName, "Base_Deco_PictureFrame2") == 0)
            {
                parseBasePortalOnClient(base->rooms[i]->details[j], base->rooms[i], raidIndex - 1);
            }
            // IOP's, ...
            if (k < MAX_SG_IOP && (base->rooms[i]->details[j]->info->eFunction == kDetailFunction_ItemOfPower || 
                                base->rooms[i]->details[j]->info->eFunction == kDetailFunction_ItemOfPowerMount))
            {
                if (g_baseRaidType == RAID_TYPE_IOP || g_baseRaidType == RAID_TYPE_CTF)
                {
                    parseBaseIOPOnClient(base->rooms[i]->details[j]);
                    k++;
                }
            }
        }
    }
}

void baseReceive(Packet *pak, int raidIndex, Vec3 offset, int isStitch)
{
    if (baseReceiveAndLoad(pak, raidIndex))
    { 
        //Only set up the base if we received it correctly
        if (raidIndex && !isStitch)
        {
            // Do some prost processing specific to raids - mostly concerned with IOP tweaks, and doors for stitching
            basePostProcessForRaid(raidIndex);
        }
        baseAfterLoad(raidIndex, offset, isStitch);
    }
    else
    {
        sBadBase = 1;
        EMPTY_INPUT_PACKET(CLIENTINP_REQ_WORLD_UPDATE);
    }
}

bool baseAfterLoad(int raidIndex, Vec3 offset, int isStitch)
{
    int i, j;
    bool res = false;
    Base *base = raidIndex ? g_raidbases[raidIndex - 1] : &g_base;
    
    setupInfoPointers();
    if (strcmp(group_info.scenefile,"scenes/default_base.txt")!=0)
    {
        strcpy(group_info.scenefile,"scenes/default_base.txt");
        sunSetSkyFadeClient(0, 1, 0.0);
        sceneLoad(group_info.scenefile);
    }

    if( isStitch || !checkBaseCorrupted(base) )
    {
        baseToDefsWithOffset(base, raidIndex ? raidIndex - 1 : 0, offset);
        if (offset)
        {
            // slide everything over
            copyVec3(offset, base->ref->mat[3]); // TODO: use a marker from the base map instead
            for(i = eaSize(&base->rooms) - 1; i >= 0; --i)
            {
                for(j = eaSize(&base->rooms[i]->details) - 1; j >= 0; --j)
                {
                    Mat4 mat;
                    Entity *de = base->rooms[i]->details[j]->e;
                    if(de)
                    {
                        copyMat4(de->fork, mat);
                        addVec3(base->ref->mat[3], mat[3], mat[3]);
                        entUpdateMat4Instantaneous(de, mat);
                    }
                    addVec3(base->ref->mat[3], base->rooms[i]->details[j]->mat[3], base->rooms[i]->details[j]->mat[3]);
                }
            }
        }
        baseUpdateEntityLighting();
        baseedit_SyncUIState();
        if (isStitch)
        {
            sgRaidConvertRaidBasesToMapData();
        }
        automap_init();
        res = true;
    }

    return res;
}

