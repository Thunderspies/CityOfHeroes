/***************************************************************************
 *     Copyright (c) 2005-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include "float.h"
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/utils/utils.h>

#include "gridcoll/gridcoll.h"
#include "group/group.h"

#include "bases/bases.h"
#include "bases/basesystems.h"
#include "bases/baseedit.h"
#include "bases/basedata.h"
#include "bases/basedraw.h"
#include "bases/baseinv.h"
#include "bases/baseclientsend.h"
#include "bases/baselegal.h"
#include "bases/baseparse.h"
#include "bases/baseclientrecv.h"
#include "bases/basetogroup.h"

#include "cmdparse/cmdgame.h"
#include "graphics/camera.h"
#include "graphics/gfxwindow.h"
#include "win/input.h"
#include "gameComm/wdwbase.h"
#include "UI/uiInput.h"
#include "UI/uiUtil.h"
#include "UI/uiBaseInput.h"
#include "UI/uiBaseRoom.h"
#include "UI/uiBaseProps.h"
#include "UI/uiBaseInventory.h"
#include "UI/uiWindows.h"
#include "UI/uiScrollBar.h"
#include "UI/uiOptions.h"
#include "graphics/font.h"
#include "entity/entity.h"
#include "UI/uiNet.h"
#include "player/player.h"
#include <utilitieslib/utils/timing.h>
#include "gridcoll/gridcache.h"
#include "entity/Supergroup.h"
#include "render/tex.h"
#include <utilitieslib/components/StashTable.h>
#include "render/thread/rt_state.h"

#define MAX_ZOOM_DIST  (game_state.edit_base==kBaseEdit_Plot?2000:820)
#define MIN_ZOOM_DIST  20
#define MAX_INTERSECTING_DETAILS 20

typedef struct DragBox
{
    Mat4 mat;
    Mat4 matLastGood;
    Vec3 clickPoint;
    TintedGroupDef *def;
    TintedGroupDef *defLastGood;
    TintedGroupDef *defLastGood_volume;
    int iCntIntersecting;
    int aidDetails[MAX_INTERSECTING_DETAILS];
    BaseRoom *pCurRoom;
} DragBox;

BaseRoomRef g_refCurRoom;
RoomDetailRef g_refCurDetail;
BaseBlockRef g_refCurBlock;
const RoomTemplate *g_pDragRoomTemplate;
int g_iMoveRoomId;
Vec3 s_MoveRoomVec;
int g_iDecor;
int g_iDecorFadeTimer;

int g_iXDragStart;
int g_iYDragStart;


static DragBox s_DragBox;

static TintedGroupDef *selbase_grp[5];
static Mat4 selbase_mat[5];
static Vec3 selbase_scale[5];


static int s_iDragSurface;

// room dragging globals
static int s_iXSize;
static int s_iZSize;
static int s_iRot;
StashTable roomHashPositionsVisited;


// door dragging globals
static int s_iDoorDragRoomId;
static int s_aiDoorDragBlock[2];
static int s_aiDoorDragHeight[2];

//plot dragging globals
static const BasePlot * s_pBasePlotDrag;
static Vec3 s_vPos;

// object dragging globals
static int s_yAdjusted = false;
static int s_yStart = 0;
static int s_rotAdjusted = false;
static int s_rotStart = 0;

typedef struct PotentialDoorway
{
    int side;        // which side of the current room this on
    int room;        // the room this would connect to
    int block[2];    // the block location of door
    int    invalid;    // true if there is some other reason we can't place door.
    Vec3 pos;        // the world space position of door

}PotentialDoorway;

PotentialDoorway ** potentialDoors;
PotentialDoorway ** potentialDoorDeletes;

char *s_aSurfaces[] =
{
    // needs to match enum Surface
    "floor",
    "wall",
    "ceiling"
};

typedef enum Side
{
    kSide_Top,
    kSide_Left,
    kSide_Right,
    kSide_Bottom,

    kSide_Count,
} Side;

static float s_fAngle = RAD(180);
static float s_fAzimuth = RAD(60);
static float s_fDist = 200.0f;
static float s_fX = 0.0f;
static float s_fZ = 0.0f;

static float s_fCurAngle = RAD(180);
static float s_fCurAzimuth = RAD(180);
static float s_fCurDist = 200.0f;
static float s_fCurX = 0.0f;
static float s_fCurZ = 0.0f;

static TintedGroupDef s_RedBox;
static TintedGroupDef s_BlueBox;
static TintedGroupDef s_YellowBox;    // Auxiliary Details Attachments
static TintedGroupDef s_GreenBox;    // Volumes for Volume Triggers
static TintedGroupDef s_RedPillar;
static TintedGroupDef s_BluePillar;
// Add any new GroupDefs ptrs to the base_InvalidateGroupDefs()

static int s_aiFloors[] = { 0, 4, 8, -1, 90 };
static int s_aiCeilings[] = { 32, 40, 48, -1, -10 };

#define ZOOM_DIST (50.0f)
#define ZOOM_AZIMUTH RAD(20)

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

/**********************************************************************func*
 * DrawDetailRefBox
 *
 */
static void DrawDetailRefBox(RoomDetailRef *pref, TintedGroupDef *box, TintedGroupDef *box_volume)
{
    Mat4 mat;

    if(!pref || !pref->p || !pref->refRoom.p || !box)
        return;

    copyMat4(pref->p->mat, mat);
    addVec3(mat[3], pref->refRoom.p->pos, mat[3]);

    drawDetailBox(pref->p->info, mat, box, box_volume);
}


void basedit_Clear()
{
    int i;

    g_base.def = 0;
    g_base.ref = 0;

    g_refCurRoom.p = NULL;
    s_RedBox.pDef = NULL;
    g_refCurDetail.p = NULL;

    for(i=0; i<ARRAY_SIZE(selbase_mat); i++)
        selbase_grp[i] = NULL;
}




/**********************************************************************func*
* baseedit_AddSGSwap
*
*/
static void deleteDefTexSwapAdapter(void* arg0)
{
    deleteDefTexSwap((DefTexSwap *)arg0);
}

void baseedit_AddSGSwap(GroupDef *def)
{
    Color newcolors[2];
    Entity *e = playerPtr();
    Supergroup *sg;

    if (!e || !e->supergroup || !g_base.rooms || baseedit_Mode() == kBaseEdit_None)
        return;

    // Apply the supergroup tinting client side so it previews correctly

    sg = playerPtr()->supergroup;
    newcolors[0] = colorFlip(COLOR_CAST(sg->colors[0]));
    newcolors[1] = colorFlip(COLOR_CAST(sg->colors[1]));
    memcpy(def->color,newcolors,sizeof(def->color));
    def->has_tint_color = 1;

    {
        DefTexSwap *dts;
        char texName[128];

        eaClearEx(&def->def_tex_swaps, deleteDefTexSwapAdapter);

        if (sg->emblem[0] == '!')
        {
            sprintf(texName,"%s_tint",sg->emblem);
        }
        else
        {
            sprintf(texName,"emblem_%s_tint",sg->emblem);
        }
        dts = createDefTexSwap("emblem_biohazard_tint", texName, 1);

        if (!dts->comp_tex_bind || !dts->comp_replace_bind)
            deleteDefTexSwap(dts);
        else
            eaPush(&def->def_tex_swaps, dts);
    }
}



/**********************************************************************func*
 * baseedit_SelDraw
 *
 */
void baseedit_SelDraw(void)
{
    GroupDef *def, *def_mount;
    int i, restore_override = 0;
    extern int override_window_size;

    if (!g_base.rooms)
        return; //If we're not in a base, don't try to do this.

    baseDrawFadeCheck();

    for(i=0; i<ARRAY_SIZE(selbase_mat); i++)
    {
        if(!selbase_grp[i])
            continue;

        if(!selbase_grp[i]->pDef)
        {
            selbase_grp[i] = NULL;
            continue;
        }

        if(selbase_scale[i][0]==0)
        {
            drawDefSimple(selbase_grp[i]->pDef, selbase_mat[i], selbase_grp[i]->tint);
        }
        else
        {
            drawScaledObject(selbase_grp[i], selbase_mat[i], selbase_scale[i]);
        }

        selbase_grp[i] = NULL;
    }

    if( baseEdit_state.room_moving )
    {
        BaseRoom * room = baseGetRoom( &g_base, g_iMoveRoomId );
        int i;
        int block[2];
        Vec3 min, max;
        Mat4 mat;
          float fGrid = g_base.roomBlockSize;

        for( i = eaSize(&room->blocks)-1; i>= 0; i-- )
        {
            if( room->blocks[i]->height[0] >= room->blocks[i]->height[1] )
            {
                block[0] = i % room->size[0];
                block[1] = i / room->size[0];
                 rotateCoordsIn2dArray(block, room->size, s_iRot, 0);

                  copyMat4(unitmat, mat);
                copyVec3(s_MoveRoomVec,mat[3]);
                setVec3( min, s_MoveRoomVec[0] + block[0]*fGrid, 0, s_MoveRoomVec[2] + block[1]*fGrid);
                 setVec3( max, s_MoveRoomVec[0] + (block[0]+1)*fGrid, 49, s_MoveRoomVec[2] + (block[1]+1)*fGrid);
                drawBoxExtents(mat[3],min,max,&s_YellowBox);
            }
        }
    }

    if( override_window_size )
    {
        restore_override = 1;
        override_window_size = 0;
    }

    if( baseEdit_state.detail_dragging == kDragType_Sticky && window_collision() )
        return;

    if( restore_override )
        override_window_size = 1;

    if(g_refCurDetail.p)
    {
        RoomDetail **auxlist = NULL;
        if(!baseedit_InDragMode() && g_refCurDetail.id)
        {
            DrawDetailRefBox(&g_refCurDetail, &s_BlueBox, &s_GreenBox);
            if (g_refCurDetail.p->iAuxAttachedID)
            {
                RoomDetail *pAux = baseGetDetail(&g_base, g_refCurDetail.p->iAuxAttachedID, NULL);
                if(pAux) eaPush(&auxlist,pAux);
            }
            else
            {
                auxlist = findAttachedAuxiliary(g_refCurDetail.refRoom.p,g_refCurDetail.p);
            }
        }
        else if(baseEdit_state.detail_dragging)
        {
            Vec3 vecFake;
            def = groupDefFind(g_refCurDetail.p->info->pchGroupName);
            def_mount = g_refCurDetail.p->info->bMounted?groupDefFind(g_refCurDetail.p->info->pchGroupNameMount):NULL;
            if(def)
            {
                int i;

                copyVec3(s_DragBox.mat[3],vecFake);                
                
                if (playerPtr() && playerPtr()->supergroup && strstriConst(g_refCurDetail.p->info->pchGroupName,"baselogo") 
                    && !eaSize(&def->def_tex_swaps) ) //replace with correct check for logoable object
                {
                    baseedit_AddSGSwap(def);
                }

                if(s_DragBox.def)
                    drawDetailBox(g_refCurDetail.p->info, s_DragBox.mat, s_DragBox.def, NULL);

                if(s_DragBox.defLastGood)
                {
                    drawDetailBox(g_refCurDetail.p->info, s_DragBox.matLastGood, s_DragBox.defLastGood, s_DragBox.defLastGood_volume);
                    drawDefSimple(def, s_DragBox.matLastGood, NULL);
                    if (def_mount) drawDefSimple(def_mount, s_DragBox.matLastGood, NULL);
                    copyVec3(s_DragBox.matLastGood[3],vecFake);
                }
                else if(s_DragBox.def)
                {
                    drawDefSimple(def, s_DragBox.mat, NULL);
                    if (def_mount) drawDefSimple(def_mount, s_DragBox.mat, NULL);
                }

                if(s_DragBox.pCurRoom) // Check to make sure the room didn't vanish on us.
                {
                    for(i=0; i<s_DragBox.iCntIntersecting; i++)
                    {
                        GroupDef *def;
                        RoomDetail *pdetail = roomGetDetail(s_DragBox.pCurRoom, s_DragBox.aidDetails[i]);

                        if(!pdetail || pdetail->id == g_refCurDetail.id)
                            continue;

                        def = groupDefFind(pdetail->info->pchGroupName);
                        if(def)
                        {
                            Mat4 mat;
                            copyMat4(pdetail->mat, mat);
                            addVec3(s_DragBox.pCurRoom->pos, mat[3], mat[3]);
                            drawDetailBox(pdetail->info, mat, &s_RedBox, &s_GreenBox);
                        }
                    }
                }
                subVec3(vecFake,s_DragBox.pCurRoom->pos,vecFake);
            }

            {
                int j;
                RoomDetail *fake;
                RoomDetail **ppRoomDetails = NULL;

                fake = calloc(1,sizeof(RoomDetail));
                fake->info = g_refCurDetail.p->info;
                fake->id = INT_MAX;
                copyVec3(vecFake,fake->mat[3]);

                eaCopy(&ppRoomDetails, &(g_refCurDetail.refRoom.p->details));
                j = eaFind(&ppRoomDetails,g_refCurDetail.p);

                if(j >= 0)
                {
                    // Replace the detail 
                    ppRoomDetails[j] = fake;
                }
                else
                {
                    eaPush(&ppRoomDetails,fake);
                }
                roomAttachAuxiliary(g_refCurDetail.refRoom.p,ppRoomDetails);

                if (fake->iAuxAttachedID)
                {
                    RoomDetail *pAux = baseGetDetail(&g_base, fake->iAuxAttachedID, NULL);
                    if(pAux) eaPush(&auxlist,pAux);
                }
                else
                {
                    auxlist = findAttachedAuxiliary(g_refCurDetail.refRoom.p,fake);
                }

                // Fix it
                roomAttachAuxiliary(g_refCurDetail.refRoom.p,g_refCurDetail.refRoom.p->details);

                free(fake);
            }
        }
        
        g_refCurDetail.iNumAttachedAux = eaSize(&auxlist);

        // Color the aux details
        if (auxlist)
        {
            RoomDetailRef auxref;
            int j;
            auxref.refRoom.id = g_refCurDetail.refRoom.id;
            auxref.refRoom.p = g_refCurDetail.refRoom.p;
            for (j = eaSize(&auxlist)-1; j >= 0; j--)
            {
                auxref.p = auxlist[j];
                auxref.id = auxref.p->id;
                DrawDetailRefBox(&auxref, &s_YellowBox, &s_GreenBox);
            }
            eaDestroy(&auxlist);
        }
    }
    else if( baseEdit_state.surface_dragging )
    {
        if(s_DragBox.pCurRoom == g_refCurRoom.p) // Check to make sure the room didn't vanish on us.
        {
            for(i=0; i<s_DragBox.iCntIntersecting; i++)
            {
                GroupDef *def;
                RoomDetail *pdetail = roomGetDetail(g_refCurRoom.p, s_DragBox.aidDetails[i]);

                if(!pdetail)
                    continue;

                def = groupDefFind(pdetail->info->pchGroupName);
                if(def)
                {
                    Mat4 mat;
                    copyMat4(pdetail->mat, mat);
                    addVec3(g_refCurRoom.p->pos, mat[3], mat[3]);
                    drawDetailBox(pdetail->info, mat, &s_RedBox, &s_GreenBox);
                }
            }
        }
    }
}

/**********************************************************************func*
 * baseDrawSelObj
 *
 */
static void baseDrawSelObj(int idx,TintedGroupDef *grp,const Mat4 mat,const Vec3 scale,float gridsize)
{
    selbase_grp[idx] = grp;
    copyMat4(mat,selbase_mat[idx]);
    copyVec3(scale,selbase_scale[idx]);
}


//-----------------------------------------------------------------------------------------------------------------
// Room Dragging //////////////////////////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------------------------------------------

/**********************************************************************func*
 * baseedit_IsDoorway
 *
 */
bool baseedit_IsDoorway(BaseRoom *pRoom, int block[2], F32 height[2])
{
    int i;
    int n = eaSize(&pRoom->doors);

    for(i=0; i<n; i++)
    {
        if(pRoom->doors[i]->pos[0]==block[0] && pRoom->doors[i]->pos[1]==block[1] && pRoom->doors[i]->height[1])
        {
            if(height)
                memcpy(height, pRoom->doors[i]->height, sizeof(F32)*2);

            return true;
        }
    }

    return false;
}

/**********************************************************************func*
 * RoomHasDoorOnSameSide
 *
 */
static bool RoomHasDoorOnSameSide(BaseRoom *pRoom, int block[2])
{
    int i;
    int n = eaSize(&pRoom->doors);
    for(i=0; i<n; i++)
    {
        if(((pRoom->doors[i]->pos[0]==pRoom->size[0] || pRoom->doors[i]->pos[0]<0)
                && pRoom->doors[i]->pos[0] == block[0])
            || ((pRoom->doors[i]->pos[1]==pRoom->size[1] || pRoom->doors[i]->pos[1]<0)
                && pRoom->doors[i]->pos[1] == block[1]))
        {
            return true;
        }
    }

    return false;
}

/**********************************************************************func*
* potentialDoorCheck
*
*/
static bool potentialDoorCheck( int side, BaseRoom *pRoom, int block[2] )
{
    int i;
    for( i = eaSize(&potentialDoors)-1; i >= 0; i-- )
    {
        // only one door per side per room
        if( side == potentialDoors[i]->side && pRoom->id == potentialDoors[i]->room )
            return false;
    }
    return true;
}



/**********************************************************************func*
* ValidRoomLocation
*
*/
static bool ValidRoomLocation(Mat4 mat, Vec3 scale, BaseRoom *room )
{
    int i;
    Vec3 bl;
    Vec3 ur;
    int block[2];
    int id = 0;
    float fGridSize = g_base.roomBlockSize;
    bool bRet = false;

    if( !potentialDoors )
        eaCreate(&potentialDoors);

    eaClearEx(&potentialDoors, NULL);

    scaleVec3(scale, -.5, bl);
     addVec3(bl, mat[3], bl);
    scaleVec3(scale, .5, ur);
    addVec3(ur, mat[3], ur);

    // fist check against plot
    if( g_base.plot )
    {
        if( bl[0] < g_base.plot_pos[0] ||
            bl[2] < g_base.plot_pos[2] || 
            ur[0] > g_base.plot_pos[0] + g_base.plot_width*fGridSize ||
            ur[2] > g_base.plot_pos[2] + g_base.plot_height*fGridSize )
        {
            base_setStatusMsg( "RoomOutsidePlot", kBaseStatusMsg_Error );
            return false;
        }
    }

    for(i=0; i<eaSize(&g_base.rooms); i++)
    {
        Vec3 bl2;
        Vec3 ur2;
        Side iSide;
        BaseRoom *pRoom = g_base.rooms[i];

        copyVec3(pRoom->pos, bl2);
        copyVec3(pRoom->pos, ur2);
        ur2[0] += fGridSize*(pRoom->size[0]);
        ur2[2] += fGridSize*(pRoom->size[1]);

        if(ur[0]>=(bl2[0]+fGridSize) && bl[0]<=(ur2[0]-fGridSize)
            && (ur[2]==bl2[2]-fGridSize || bl[2]==ur2[2]+fGridSize))
        {
            // Good
            int iMin = (max(bl[0], bl2[0])-bl2[0])/fGridSize;
            int iMax = (min(ur[0], ur2[0])-bl2[0])/fGridSize;

            block[0] = iMin+(iMax-iMin)/2;
            if(ur[2]==bl2[2]-fGridSize)
            {
                // bottom edge of existing room
                block[1] = -1;

                iSide = kSide_Top; // of the new room
            }
            else
            {
                // top edge of existing room
                block[1] = pRoom->size[1];

                iSide = kSide_Bottom; // of the new room
            }
            if(potentialDoorCheck(iSide, pRoom, block))
            {
                PotentialDoorway * pd = calloc( 1, sizeof(PotentialDoorway) );
                float afHeight[] = { 4, 40 };
                
                if( room )
                {
                    int room_block[2], idx;

                    room_block[0] = ((pRoom->pos[0]+block[0]*fGridSize)-bl[0])/fGridSize;
                    room_block[1] = ((pRoom->pos[2]+block[1]*fGridSize)-bl[2])/fGridSize;
                    rotateCoordsIn2dArray(room_block, room->size, s_iRot, 1);

                    if( roomHeightIntersecting(room,room_block,afHeight,0,0) )
                        pd->invalid = true;

                    room_block[0] = MINMAX( room_block[0], 0, room->size[0]-1 );
                    room_block[1] = MINMAX( room_block[1], 0, room->size[1]-1 );
                    idx = room_block[0]+ room->size[0]*room_block[1];

                    if(room->blocks[idx]->height[0] >= room->blocks[idx]->height[1])
                        pd->invalid = true;
                }

                if(roomHeightIntersecting(pRoom,block,afHeight,0,0))
                    pd->invalid = true;

                pd->side = iSide;
                pd->room = pRoom->id;
                memcpy(pd->block, block, sizeof(block));
                eaPush(&potentialDoors, pd );
            }
        }
        else if(ur[2]>=(bl2[2]+fGridSize) && bl[2]<=(ur2[2]-fGridSize)
            && (ur[0]==bl2[0]-fGridSize || bl[0]==ur2[0]+fGridSize))
        {
            // Good
            int iMin = (max(bl[2], bl2[2])-bl2[2])/fGridSize;
            int iMax = (min(ur[2], ur2[2])-bl2[2])/fGridSize;

            block[1] = iMin+(iMax-iMin)/2;
            if(ur[0]==bl2[0]-fGridSize)
            {
                // left edge of existing room
                block[0] = -1;

                iSide = kSide_Right; // of the new room
            }
            else
            {
                // right edge of existing room
                block[0] = pRoom->size[0];

                iSide = kSide_Left; // of the new room
            }
            if( potentialDoorCheck(iSide, pRoom, block) )
            {
                PotentialDoorway * pd = calloc( 1, sizeof(PotentialDoorway) );
                float afHeight[] = { 4, 40 };

                if( room )
                {
                    int room_block[2], idx;

                    room_block[0] = ((pRoom->pos[0]+block[0]*fGridSize)-bl[0])/fGridSize;
                    room_block[1] = ((pRoom->pos[2]+block[1]*fGridSize)-bl[2])/fGridSize;
                     rotateCoordsIn2dArray(room_block, room->size, s_iRot, 1);

                    if( roomHeightIntersecting(room,room_block,afHeight,0,0) )
                        pd->invalid = true;

                    room_block[0] = MINMAX( room_block[0], 0, room->size[0]-1 );
                    room_block[1] = MINMAX( room_block[1], 0, room->size[1]-1 );
                    idx = room_block[0]+ room->size[0]*room_block[1];

                    if(room->blocks[idx]->height[0] >= room->blocks[idx]->height[1])
                        pd->invalid = true;
                }

                if(roomHeightIntersecting(pRoom,block,afHeight,0,0) )
                    pd->invalid = true;

                pd->side = iSide;
                pd->room = pRoom->id;
                memcpy(pd->block, block, sizeof(block));
                eaPush(&potentialDoors, pd );
            }
        }
        else if(!(bl[0]>=ur2[0]+fGridSize || bl[2]>=ur2[2]+fGridSize
            || ur[0]<=bl2[0]-fGridSize || ur[2]<=bl2[2]-fGridSize))
        {
            // Bad
            base_setStatusMsg( "NewRoomCoincident", kBaseStatusMsg_Error );
            return false;
        }
    }

    // any valid doors?
    for( i = eaSize(&potentialDoors)-1; i>=0; i--)
    {
        if( !potentialDoors[i]->invalid )
            return true;
    }

    base_setStatusMsg( "CantFindDoors", kBaseStatusMsg_Error );
    return false;
}

RoomDoor * create_room_doors;
static int create_room_door_count, create_room_door_max;
/**********************************************************************func*
 * UpdateRoomDrag
 *
 */
static bool UpdateRoomDrag(int iXSize, int iZSize, Vec3 vec, int x, int y)
{
    Vec3 start,end;
    float xp, zp;
    Vec3 vScale;
    Mat4 mat;
    int i, invalid = false, passes_room_check = false;
    char coords[128];

    float fGridSize = g_base.roomBlockSize;

    gfxCursor3d(x,y,10000,start,end);

    // Want to know where [start, end] intesect with xz plane
    // (x-x1)/(x2-x1) = (y-y1)/(y2-y1)
    // x = x1 + (y-y1)(x2-x1)/(y2-y1)
    // Likewise for z.
    xp = start[0] + (0 - start[1])*(end[0]-start[0])/(end[1]-start[1]);
    zp = start[2] + (0 - start[1])*(end[2]-start[2])/(end[1]-start[1]);

    if(s_iRot&1)
    {
        int i = iXSize;
        iXSize = iZSize;
        iZSize = i;
    }

    copyMat4(unitmat, mat);
    mat[3][0] = (floor(xp/fGridSize)+(iXSize&1)/2.0f)*fGridSize;
    mat[3][1] = 0;
    mat[3][2] = (floor(zp/fGridSize)+(iZSize&1)/2.0f)*fGridSize;

    vScale[0] = iXSize*fGridSize;
    vScale[1] = 48;
    vScale[2] = iZSize*fGridSize;

    if(vec)
    {
        copyVec3(mat[3], vec);
        vec[0] -= iXSize*fGridSize/2.0;
        vec[2] -= iZSize*fGridSize/2.0;
    }

    if(!ValidRoomLocation(mat, vScale, 0))
    {
        baseDrawSelObj(0, &s_RedBox, mat, vScale, fGridSize);
        invalid = true;
    }
    else
    {
        char itoabuf[18];
        Strncpyt( coords, itoa(vec[0], itoabuf, 10) );
        Strncatt( coords, itoa(vec[2], itoabuf, 10) );

        if ( stashFindElement( roomHashPositionsVisited, coords , NULL) )
        {
            passes_room_check = (int)stashFindPointerReturnPointer(roomHashPositionsVisited, coords);
        }
        else
        {
            int size[2];
            F32 afHeights[2] = { 4, 40 };
            create_room_door_count = 0;
            for( i = eaSize(&potentialDoors)-1; i >= 0; i-- )
            {
                BaseRoom *pRoom =  baseGetRoom(&g_base, potentialDoors[i]->room);
                RoomDoor *roomdoor = dynArrayAdd( &create_room_doors, sizeof(RoomDoor), &create_room_door_count, &create_room_door_max, 1 );
                copyVec2( afHeights, roomdoor->height);

                // want block relative to new room
                roomdoor->pos[0] = ((pRoom->pos[0] + potentialDoors[i]->block[0]*fGridSize)-vec[0])/fGridSize;
                roomdoor->pos[1] = ((pRoom->pos[2] + potentialDoors[i]->block[1]*fGridSize)-vec[2])/fGridSize;
            }

            if(s_iRot & 1)
            {
                size[0] = s_iZSize;
                size[1] = s_iXSize;
            }
            else
            {
                size[0] = s_iXSize;
                size[1] = s_iZSize;
            }

            passes_room_check = addOrChangeRooomIsLegal( &g_base, 0, size, 0, vec, afHeights, g_pDragRoomTemplate, create_room_doors, create_room_door_count );
            rebuildBaseFromStr( &g_base );
            setupInfoPointers();
            baseToDefs(&g_base, 0);
            baseedit_SyncUIState();

            stashAddInt( roomHashPositionsVisited, coords,passes_room_check, false);
        }

        if( !passes_room_check )
        {
            base_setStatusMsg( "BadRoomConnection", kBaseStatusMsg_Error );
            baseDrawSelObj(0, &s_RedBox, mat, vScale, fGridSize);
            invalid = true;
        }
        else
            baseDrawSelObj(0, &s_BlueBox, mat, vScale, fGridSize);
    }

    for( i = eaSize(&potentialDoors)-1; i >= 0; i-- )
    {
        Mat4 matDoor;
        Vec3 v = { fGridSize, 48, fGridSize };
        BaseRoom *pRoom = baseGetRoom(&g_base, potentialDoors[i]->room);

        copyMat4(unitmat, matDoor);
        copyVec3(pRoom->pos, matDoor[3]);
        matDoor[3][0] += fGridSize*potentialDoors[i]->block[0] + fGridSize/2 - .001;
        matDoor[3][2] += fGridSize*potentialDoors[i]->block[1] + fGridSize/2 - .001;

         if( potentialDoors[i]->invalid || !passes_room_check )
            baseDrawSelObj(i+1, &s_RedBox, matDoor, v, fGridSize);
        else
            baseDrawSelObj(i+1, &s_BlueBox, matDoor, v, fGridSize);
    }

    if( invalid )
        return false;

    return true;
}

/**********************************************************************func*
 * EndRoomDrag
 *
 */

static void EndRoomDrag(int iXSize, int iZSize, Vec3 vec)
{
    int i;
    int size[2];
    F32 afHeights[2] = { 4, 40 };
    float fGrid = g_base.roomBlockSize;

    if(s_iRot & 1)
    {
        size[0] = s_iZSize;
        size[1] = s_iXSize;
    }
    else
    {
        size[0] = s_iXSize;
        size[1] = s_iZSize;
    }

    create_room_door_count = 0;

    for( i = eaSize(&potentialDoors)-1; i >= 0; i-- )
    {
        BaseRoom *pRoom =  baseGetRoom(&g_base, potentialDoors[i]->room);
        RoomDoor *roomdoor = dynArrayAdd( &create_room_doors, sizeof(RoomDoor), &create_room_door_count, &create_room_door_max, 1 );
        copyVec2( afHeights, roomdoor->height);

        // want block relative to new room
        roomdoor->pos[0] = ((pRoom->pos[0] + potentialDoors[i]->block[0]*fGrid)-vec[0])/fGrid;
        roomdoor->pos[1] = ((pRoom->pos[2] + potentialDoors[i]->block[1]*fGrid)-vec[2])/fGrid;
    }

    baseedit_CreateRoom(g_pDragRoomTemplate, size, vec, afHeights, create_room_doors, create_room_door_count );
    baseEdit_state.room_dragging = false;
}

/**********************************************************************func*
 * HandleRoomDrag
 *
 */
static void HandleRoomDrag(void)
{
    Vec3 vec;
    int x, y;
    inpMousePos( &x, &y );

    if(UpdateRoomDrag(s_iXSize, s_iZSize, vec, x, y))
    {
        if(baseEdit_state.room_dragging == kDragType_Drag
            &&  !baseEdit_state.dragging)
        {
            EndRoomDrag(s_iXSize, s_iZSize, vec);
        }
    }

    if(baseEdit_state.room_dragging == kDragType_Drag
        &&  !baseEdit_state.dragging)
    {
        baseEdit_state.room_dragging = false;
    }
}

/**********************************************************************func*
 * baseedit_StartRoomDrag
 *
 */
void baseedit_StartRoomDrag(int iXSize, int iZSize, const RoomTemplate *info)
{
    baseedit_CancelDrags();
    baseedit_CancelSelects();
    s_iXSize = iXSize;
    s_iZSize = iZSize;
    g_pDragRoomTemplate = info;
    s_iRot = 0;
    baseEdit_state.room_dragging = kDragType_Sticky;

    if( !roomHashPositionsVisited )
        roomHashPositionsVisited = stashTableCreateWithStringKeys(100, StashDeepCopyKeys | StashCaseSensitive );
    stashTableClear(roomHashPositionsVisited);
}

//-----------------------------------------------------------------------------------------------------------------
// Room Move //////////////////////////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------------------------------------------

/**********************************************************************func*
* UpdateRoomMove
*
*/
static bool UpdateRoomMove(Vec3 vec, int x, int y)
{
    BaseRoom * room = baseGetRoom( &g_base, g_iMoveRoomId );
    Vec3 start,end;
    float xp, zp;
    Vec3 vScale;
    Mat4 mat;
    int i, invalid = false, passes_room_check = false;
    int size[2];
    char coords[128];
    float fGridSize = g_base.roomBlockSize;

    if( s_iRot & 1 )
    {
        size[0] = room->size[1];
        size[1] = room->size[0];
    }
    else
    {
        size[0] = room->size[0];
        size[1] = room->size[1];
    }

    gfxCursor3d(x,y,10000,start,end);
    // Want to know where [start, end] intesect with xz plane
    // (x-x1)/(x2-x1) = (y-y1)/(y2-y1)
    // x = x1 + (y-y1)(x2-x1)/(y2-y1)
    // Likewise for z.
    xp = start[0] + (0 - start[1])*(end[0]-start[0])/(end[1]-start[1]);
    zp = start[2] + (0 - start[1])*(end[2]-start[2])/(end[1]-start[1]);

    copyMat4(unitmat, mat);
    mat[3][0] = (floor(xp/fGridSize)+(size[0]&1)/2.0f)*fGridSize;
    mat[3][1] = 0;
    mat[3][2] = (floor(zp/fGridSize)+(size[1]&1)/2.0f)*fGridSize;

    vScale[0] = size[0]*fGridSize;
    vScale[1] = 48;
    vScale[2] = size[1]*fGridSize;

    if(vec)
    {
        copyVec3(mat[3], vec);
        vec[0] -= size[0]*fGridSize/2.0;
        vec[2] -= size[1]*fGridSize/2.0;
    }
    copyVec3(vec,s_MoveRoomVec);
    

    // now we need to make a copy of the base without the room we are moving to perform tests on
    eaFindAndRemove(&g_base.rooms,room);
    checkAllDoors(&g_base,true);

     if(!ValidRoomLocation(mat, vScale, room))
     {
        baseDrawSelObj(0, &s_RedBox, mat, vScale, fGridSize);
        invalid = true;
    }
    else
    {
        char itoabuf[18];
        Strncpyt( coords, itoa(vec[0], itoabuf, 10) );
        Strncatt( coords, itoa(vec[2], itoabuf, 10) );

        if( !stashFindInt( roomHashPositionsVisited, coords, &passes_room_check) )
        {
            F32 afHeights[2] = { 4, 40 };
            create_room_door_count = 0;
            for( i = eaSize(&potentialDoors)-1; i >= 0; i-- )
            {
                BaseRoom *pRoom =  baseGetRoom(&g_base, potentialDoors[i]->room);
                RoomDoor *roomdoor;
                
                if( !pRoom )
                    continue;
                
                roomdoor = dynArrayAdd( &create_room_doors, sizeof(RoomDoor), &create_room_door_count, &create_room_door_max, 1 );
                copyVec2( afHeights, roomdoor->height);

                // want block relative to new room
                roomdoor->pos[0] = ((pRoom->pos[0] + potentialDoors[i]->block[0]*fGridSize)-vec[0])/fGridSize;
                roomdoor->pos[1] = ((pRoom->pos[2] + potentialDoors[i]->block[1]*fGridSize)-vec[2])/fGridSize;
            }

            passes_room_check = addOrChangeRooomIsLegal( &g_base, g_iMoveRoomId, size, s_iRot, vec, afHeights, room->info, create_room_doors, create_room_door_count );
             rebuildBaseFromStr( &g_base );
            setupInfoPointers();
            baseToDefs(&g_base, 0);
            baseedit_SyncUIState();

            room = baseGetRoom( &g_base, g_iMoveRoomId );
            eaFindAndRemove(&g_base.rooms,room);
            checkAllDoors(&g_base,true);

            stashAddInt( roomHashPositionsVisited, coords, passes_room_check, false);
        }

        if( !passes_room_check )
        {
            base_setStatusMsg( "BadRoomConnection", kBaseStatusMsg_Error );
            baseDrawSelObj(0, &s_RedBox, mat, vScale, fGridSize);
            invalid = true;
        }
        else
            baseDrawSelObj(0, &s_BlueBox, mat, vScale, fGridSize);
    }

    for( i = eaSize(&potentialDoors)-1; i >= 0; i-- )
    {
        Mat4 matDoor;
        Vec3 v = { fGridSize, 48, fGridSize };
        BaseRoom *pRoom = baseGetRoom(&g_base, potentialDoors[i]->room);

        if( !pRoom )
            continue;

        copyMat4(unitmat, matDoor);
        copyVec3(pRoom->pos, matDoor[3]);
        matDoor[3][0] += fGridSize*potentialDoors[i]->block[0] + fGridSize/2 - .001;
        matDoor[3][2] += fGridSize*potentialDoors[i]->block[1] + fGridSize/2 - .001;

        if( potentialDoors[i]->invalid || !passes_room_check )
            baseDrawSelObj(i+1, &s_RedBox, matDoor, v, fGridSize);
        else
            baseDrawSelObj(i+1, &s_BlueBox, matDoor, v, fGridSize);
    }

    rebuildBaseFromStr( &g_base );
    setupInfoPointers();
    baseToDefs(&g_base, 0);
    baseedit_SyncUIState();

    if( invalid )
        return false;

    return true;
}

/**********************************************************************func*
* EndRoomMove
*
*/

static void EndRoomMove(Vec3 vec)
{
    F32 afHeights[2] = { 4, 40 };
    BaseRoom *room = baseGetRoom(&g_base, g_iMoveRoomId);

    float fGrid = g_base.roomBlockSize;
    int i;
    
    create_room_door_count = 0;

    for( i = eaSize(&potentialDoors)-1; i >= 0; i-- )
    {
        BaseRoom *pRoom =  baseGetRoom(&g_base, potentialDoors[i]->room);
        RoomDoor *roomdoor = dynArrayAdd( &create_room_doors, sizeof(RoomDoor), &create_room_door_count, &create_room_door_max, 1 );
        copyVec2( afHeights, roomdoor->height);

        // want block relative to new room
        roomdoor->pos[0] = ((pRoom->pos[0] + potentialDoors[i]->block[0]*fGrid)-vec[0])/fGrid;
        roomdoor->pos[1] = ((pRoom->pos[2] + potentialDoors[i]->block[1]*fGrid)-vec[2])/fGrid;
    }

    baseedit_MoveRoom( room, s_iRot, vec, create_room_doors, create_room_door_count );
    baseEdit_state.room_moving = false;
}

/**********************************************************************func*
* HandleRoomMove
*
*/
static void HandleRoomMove(void)
{
    Vec3 vec;
    int x, y;
    inpMousePos( &x, &y );

    if(UpdateRoomMove(vec, x, y))
    {
        if(baseEdit_state.room_moving == kDragType_Drag
            &&  !baseEdit_state.dragging)
        {
            EndRoomMove(vec);
        }
    }

    if(baseEdit_state.room_moving == kDragType_Drag
        &&  !baseEdit_state.dragging)
    {
        baseEdit_state.room_moving = false;
    }
}

/**********************************************************************func*
* baseedit_StartRoomMove
*
*/
void baseedit_StartRoomMove( BaseRoom * room )
{
    baseedit_CancelDrags();
    baseedit_CancelSelects();
    s_iRot = 0;

    g_iMoveRoomId = room->id;
    baseEdit_state.room_moving = kDragType_Sticky;

    if( !roomHashPositionsVisited )
        roomHashPositionsVisited = stashTableCreateWithStringKeys(100, StashCaseSensitive | StashDeepCopyKeys);
    stashTableClear(roomHashPositionsVisited);
}




//-----------------------------------------------------------------------------------------------------------------
// Door Dragging //////////////////////////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------------------------------------------

/**********************************************************************func*
* ValidDoorLocation
*
*/
static bool doorAtLocation( BaseRoom * pRoom, int block[2] )
{
    float fGrid = g_base.roomBlockSize;
    float doorLX = pRoom->pos[0]+fGrid*block[0];
    float doorLY = pRoom->pos[2]+fGrid*block[1];
    int i,j,k, bRet = false;

    if( !potentialDoorDeletes )
        eaCreate(&potentialDoorDeletes);

    eaClearEx(&potentialDoorDeletes, NULL);

    for(i=0;i<eaSize(&g_base.rooms);i++)
    {
        BaseRoom *r = g_base.rooms[i];
        for(j=eaSize(&r->doors)-1;j>=0;j--)
        {
            float dLX = r->pos[0]+fGrid*r->doors[j]->pos[0];
            float dLY = r->pos[2]+fGrid*r->doors[j]->pos[1];

            if( (doorLX == dLX && doorLY == dLY) )
            {
                return true;
            }
            else if((doorLX+fGrid >= (dLX+fGrid) && doorLX <= dLX && (doorLY+fGrid==dLY || doorLY==dLY+fGrid)) ||
                    (doorLY+fGrid >= (dLY+fGrid) && doorLY <= dLY && (doorLX+fGrid==dLX || doorLX==dLX+fGrid)) )
            {
                PotentialDoorway * pd = calloc(1, sizeof(PotentialDoorway));
                bool bMatch = false;

                pd->room = r->id;
                memcpy( pd->block, r->doors[j]->pos, sizeof(r->doors[j]->pos) );
                pd->pos[0] = dLX;
                pd->pos[2] = dLY;

                // duplicate check
                for( k = eaSize(&potentialDoorDeletes)-1; k >= 0; k-- )
                {
                    if( dLX == potentialDoorDeletes[k]->pos[0] && dLY == potentialDoorDeletes[k]->pos[2] )
                        bMatch = true;
                }

                if( !bMatch )
                {
                    eaPush(&potentialDoorDeletes, pd);
                    bRet = true;
                }
            }
        }
    }

    return bRet;
}
/**********************************************************************func*
* ValidDoorLocation
*
*/
static bool ValidDoorLocation(Mat4 mat, Vec3 scale)
{
    int i;
    Vec3 bl;
    Vec3 ur;
    int id = 0;
    float fGridSize = g_base.roomBlockSize;
    int unused[2];
    float afHeight[2];
    bool bRet1 = false, bRet2 = false, bRet3 = false, bRet4 = false;
    BaseRoom *pRoom;
    scaleVec3(scale, -.5, bl);
    addVec3(bl, mat[3], bl);
    scaleVec3(scale, .5, ur);
    addVec3(ur, mat[3], ur);

    for(i=0; i<eaSize(&g_base.rooms); i++)
    {
        Vec3 bl2;
        Vec3 ur2;
        pRoom = g_base.rooms[i];

        copyVec3(pRoom->pos, bl2);
        copyVec3(pRoom->pos, ur2);
        ur2[0] += fGridSize*(pRoom->size[0]);
        ur2[2] += fGridSize*(pRoom->size[1]);

        if(ur[0]>=(bl2[0]+fGridSize) && bl[0]<=(ur2[0]-fGridSize)
            && (ur[2]==bl2[2] || bl[2]==ur2[2]))
        {
            // Good
            int iMin = (max(bl[0], bl2[0])-bl2[0])/fGridSize;
            int iMax = (min(ur[0], ur2[0])-bl2[0])/fGridSize;

            s_aiDoorDragBlock[0] = iMin+(iMax-iMin)/2;
            if(ur[2]==bl2[2]) // bottom edge of existing room
                s_aiDoorDragBlock[1] = -1;
            else // top edge of existing room
                s_aiDoorDragBlock[1] = pRoom->size[1];

            s_iDoorDragRoomId = pRoom->id;
            if( ur[2]==bl2[2] )
                bRet1 = true;
            else
                bRet2 = true;
        }
        else if(ur[2]>=(bl2[2]+fGridSize) && bl[2]<=(ur2[2]-fGridSize)
            && (ur[0]==bl2[0] || bl[0]==ur2[0]))
        {
            // Good
            int iMin = (max(bl[2], bl2[2])-bl2[2])/fGridSize;
            int iMax = (min(ur[2], ur2[2])-bl2[2])/fGridSize;

            s_aiDoorDragBlock[1] = iMin+(iMax-iMin)/2;
            if(ur[0]==bl2[0]) // left edge of existing room
                s_aiDoorDragBlock[0] = -1;
            else // right edge of existing room
                s_aiDoorDragBlock[0] = pRoom->size[0];

            s_iDoorDragRoomId = pRoom->id;
            if( ur[0]==bl2[0] )
                bRet3 = true;
            else
                bRet4 = true;
        }
        else if(!(bl[0]>=ur2[0] || bl[2]>=ur2[2]
            || ur[0]<=bl2[0]|| ur[2]<=bl2[2]))
        {
            // Bad
            base_setStatusMsg( "DoorInRoom", kBaseStatusMsg_Error );
            return false;
        }
    }

    pRoom = baseGetRoom(&g_base, s_iDoorDragRoomId );
    afHeight[0] = s_aiDoorDragHeight[0];
    afHeight[1] = s_aiDoorDragHeight[1];

    if( !pRoom || !((bRet1&&bRet2)||(bRet3&&bRet4)) )
    {
        base_setStatusMsg( "DoorNotConnected", kBaseStatusMsg_Error );
        return false;
    }
    else if(  (doorAtLocation(pRoom, s_aiDoorDragBlock) && !eaSize(&potentialDoorDeletes)))
    {
        base_setStatusMsg( "DoorInRoom", kBaseStatusMsg_Error );
        return false;
    }
    else if( !getLegalDoorway( &g_base, pRoom, s_aiDoorDragBlock, afHeight, unused ) )
    {
        base_setStatusMsg( "DoorBlocked", kBaseStatusMsg_Error );
        return false;
    }
    else 
        return true;
}

/**********************************************************************func*
* UpdateRoomDrag
*
*/
static bool UpdateDoorDrag(int x, int y)
{
    Vec3 start,end;
    float xp, zp;
    Vec3 vScale;
    Mat4 mat;
    int i;
    float fGridSize = g_base.roomBlockSize;

    gfxCursor3d(x,y,10000,start,end);

    // Want to know where [start, end] intesect with xz plane
    // (x-x1)/(x2-x1) = (y-y1)/(y2-y1)
    // x = x1 + (y-y1)(x2-x1)/(y2-y1)
    // Likewise for z.
    xp = start[0] + (0 - start[1])*(end[0]-start[0])/(end[1]-start[1]);
    zp = start[2] + (0 - start[1])*(end[2]-start[2])/(end[1]-start[1]);


    copyMat4(unitmat, mat);
    mat[3][0] = (floor(xp/fGridSize)+1/2.0f)*fGridSize;
    mat[3][1] = 0;
    mat[3][2] = (floor(zp/fGridSize)+1/2.0f)*fGridSize;

    vScale[0] = fGridSize;
    vScale[1] = 48;
    vScale[2] = fGridSize;

    if(!ValidDoorLocation(mat, vScale))
    {
        baseDrawSelObj(0, &s_RedBox, mat, vScale, fGridSize);
        return false;
    }
    else
    {
        baseDrawSelObj(0,&s_BlueBox, mat, vScale, fGridSize);

        for( i = eaSize(&potentialDoorDeletes)-1; i >= 0; i-- )
        {
            Mat4 matDoor;
            Vec3 v = { fGridSize, 48, fGridSize };
            BaseRoom *pRoom = baseGetRoom(&g_base, potentialDoorDeletes[i]->room);

            copyMat4(unitmat, matDoor);
            copyVec3(pRoom->pos, matDoor[3]);
            matDoor[3][0] += fGridSize*potentialDoorDeletes[i]->block[0] + fGridSize/2 - .001;
            matDoor[3][2] += fGridSize*potentialDoorDeletes[i]->block[1] + fGridSize/2 - .001;
            baseDrawSelObj(i+1, &s_RedBox, matDoor, v, fGridSize);
        }
    }

    return true;
}

/**********************************************************************func*
* EndRoomDrag
*
*/
static void EndDoorDrag()
{
    F32 afHeights[2] = { 4, 40 };
    BaseRoom *pRoom = baseGetRoom(&g_base, s_iDoorDragRoomId );
    int i;

    baseedit_CreateDoorway(pRoom, s_aiDoorDragBlock, afHeights);

    for( i = eaSize(&potentialDoorDeletes)-1; i >= 0; i-- )
    {
        BaseRoom *pRoom = baseGetRoom(&g_base, potentialDoorDeletes[i]->room);
        baseClientSendDoorwayDelete( pRoom, potentialDoorDeletes[i]->block );
    }

    baseEdit_state.door_dragging = false;
}

/**********************************************************************func*
* HandleRoomDrag
*
*/
static void HandleDoorDrag(void)
{
    int x, y;
    inpMousePos( &x, &y );

    if(UpdateDoorDrag(x, y))
    {
        if(baseEdit_state.door_dragging == kDragType_Drag
            &&  !baseEdit_state.dragging)
        {
            EndDoorDrag();
        }
    }

    if(baseEdit_state.door_dragging == kDragType_Drag
        &&  !baseEdit_state.dragging)
    {
        baseEdit_state.door_dragging = false;
    }
}

/**********************************************************************func*
* baseedit_StartDoorDrag
*
*/
void baseedit_StartDoorDrag()
{
    int height[] = { 4, 40 }; // default door hts

    baseedit_CancelDrags();
    baseedit_CancelSelects();

    memcpy( s_aiDoorDragHeight, height, sizeof(height) );

    baseEdit_state.door_dragging = kDragType_Sticky;
}

//-----------------------------------------------------------------------------------------------------------------
// Surface Dragging ///////////////////////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------------------------------------------

/**********************************************************************func*
 * UpdateSurfaceDrag
 *
 */
static void UpdateSurfaceDrag(int *piHeight, int x, int y)
{
    Mat4 mat;
    int aiFloors[] = { 0, 4, 8, -1, 90 };
    int aiCeilings[] = { 32, 40, 48, -1, -10 };
    int *pi;
    int iCnt = 0;
    int iWhat = 0;
    F32 afHeights[2], potHt[2] = {0, 48};
    bool bAllowWall = true;
    int player_coll = false;
    RoomDetail *isect_details[MAX_INTERSECTING_DETAILS] = {0};
    Entity *e = playerPtr();

    if(!g_refCurRoom.p)
    {
        baseEdit_state.surface_dragging = false;
        return;
    }

    if(g_refCurBlock.p)
    {
        memcpy(afHeights, g_refCurBlock.p->height, sizeof(F32)*2);
    }
    else if(baseedit_IsDoorway(g_refCurRoom.p, g_refCurBlock.block, afHeights))
    {
        bAllowWall = false;
    }
    else
    {
        baseEdit_state.surface_dragging = false;
        return;
    }

    baseedit_CopyMatFromBlock(g_refCurRoom.p, g_refCurBlock.block, s_iDragSurface, mat);

    if(s_iDragSurface)
        pi = aiCeilings;
    else
        pi = aiFloors;

    for(iCnt=0; pi[iCnt]>=0; iCnt++)
    {
        if(afHeights[s_iDragSurface]==pi[iCnt])
            iWhat = iCnt;
    }
    iWhat += (g_iYDragStart-y)/30;
    if(iWhat < 0)
    {
        iWhat = 0;
        if(bAllowWall && s_iDragSurface==1)
        {
            iWhat = 4;
        }
    }
    else if(iWhat>=iCnt)
    {
        iWhat = iCnt-1;
        if(bAllowWall && s_iDragSurface==0)
        {
            iWhat = 4;
        }
    }

    if(s_iDragSurface)
    {
        potHt[1] = pi[iWhat];
        mat[3][1] = 48;
    }
    else
    {
        potHt[0] = pi[iWhat];
        mat[3][1] = 0;
    }

    {
        Vec3 vec;
        if(iWhat!=4)
        {
            vec[0] = vec[2] = 36;
            if(s_iDragSurface)
                vec[1] = MAX(1,48-pi[iWhat])+.05f;
            else
                vec[1] = MAX(1,pi[iWhat])+.05f;
        }

        s_DragBox.iCntIntersecting = roomHeightIntersecting(g_refCurRoom.p,g_refCurBlock.block,potHt,isect_details,MAX_INTERSECTING_DETAILS);

        if( !g_refCurBlock.p || !s_DragBox.iCntIntersecting )
        {
            if(iWhat==4)
                baseDrawSelObj(0, &s_BluePillar, mat, zerovec3, g_base.roomBlockSize);
            else
                baseDrawSelObj(0, &s_BlueBox, mat, vec, g_base.roomBlockSize);
        }
        else
        {
            int i;

            base_setStatusMsg( "SurfaceBlocked", kBaseStatusMsg_Error );

            s_DragBox.pCurRoom = g_refCurRoom.p;
            for(i=0; i<MIN(s_DragBox.iCntIntersecting,MAX_INTERSECTING_DETAILS); i++)
            {
                s_DragBox.aidDetails[i] = isect_details[i]->id;
            }

            if(iWhat==4)
                baseDrawSelObj(0, &s_RedPillar, mat, zerovec3, g_base.roomBlockSize);
            else
                baseDrawSelObj(0, &s_RedBox, mat, vec, g_base.roomBlockSize);
        }
    }

    if(piHeight)
    {
        *piHeight = pi[iWhat];
    }
}

/**********************************************************************func*
 * EndSurfaceDrag
 *
 */
static void EndSurfaceDrag(int iHeight)
{
    F32 afHeights[2];

    if(g_refCurBlock.p)
    {
        int aiHeight[2];
        aiHeight[0] = (int)g_refCurBlock.p->height[0];
        aiHeight[1] = (int)g_refCurBlock.p->height[1];

        if(iHeight!=aiHeight[s_iDragSurface])
        {
            aiHeight[s_iDragSurface] = iHeight;
            baseedit_SetCurBlockHeights(aiHeight);
        }
    }
    else if(baseedit_IsDoorway(g_refCurRoom.p, g_refCurBlock.block, afHeights))
    {
        if(iHeight!=afHeights[s_iDragSurface])
        {
            afHeights[s_iDragSurface] = iHeight;

            if(afHeights[0]>=afHeights[1])
            {
                memset(afHeights, 0, sizeof(F32)*2);
                baseedit_CreateDoorway(g_refCurRoom.p, g_refCurBlock.block, afHeights);
            }
            else
            {
                baseedit_CreateDoorway(g_refCurRoom.p, g_refCurBlock.block, afHeights);
            }
        }
    }

    baseEdit_state.surface_dragging = false;
}

/**********************************************************************func*
 * HandleSurfaceDrag
 *
 */
static void HandleSurfaceDrag(void)
{
    int iHeight, x, y;

    inpMousePos(&x, &y);

    UpdateSurfaceDrag(&iHeight, x, y);

    if(baseEdit_state.surface_dragging == kDragType_Drag
        && !baseEdit_state.dragging)
    {
        EndSurfaceDrag(iHeight);
    }
}

/**********************************************************************func*
 * baseedit_StartSurfaceDrag
 *
 */
void baseedit_StartSurfaceDrag(int iSurface)
{
    baseedit_CancelDrags();
    baseedit_CancelSelects();
    s_iDragSurface = iSurface;
    baseEdit_state.surface_dragging = kDragType_Sticky;
}

//-----------------------------------------------------------------------------------------------------------------
// Detail Dragging ////////////////////////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------------------------------------------

/**********************************************************************func*
 * CollNodeCallback
 *
 */
static int CollNodeIgnoreEditorOnlyAndVolumeTriggersCb(void *nd, int facing)
{
    DefTracker *tracker = nd;
    GroupDef    *def;
    int dummy;
    if (!tracker || !tracker->parent)
        return 1;
    def = tracker->parent->def;
    if (!def)
        return 1;
    if(strstriConst(def->name, "editoronly"))
        return 0;
    if (tracker->def && tracker->def->volume_trigger)
        return 0;
    else if (rejectByCameraHeight(tracker->def,&dummy))
        return 0;
    else
        return 1;
}

/**********************************************************************func*
 * CollNodeCallback
 *
 */
static int CollNodeCallback(void *nd, int facing)
{
    DefTracker    *tracker = nd;
    GroupDef    *def = tracker->def;
    Surface        surf = g_refCurDetail.p->info->eSurface;

    if(g_refCurDetail.p->info->eSurface < 0 || g_refCurDetail.p->info->eSurface > ARRAY_SIZE(s_aSurfaces))
        return 0;
    if(!strstriConst(tracker->def->name, s_aSurfaces[g_refCurDetail.p->info->eSurface]))
    {
        if (    !(surf == kSurface_Floor && def->stackable_floor)
            &&    !(surf == kSurface_Wall && def->stackable_wall)
            &&    !(surf == kSurface_Ceiling && def->stackable_ceiling))
            return 0;
    }
    return CollNodeIgnoreEditorOnlyAndVolumeTriggersCb(nd,facing);
}
static int BaseEditCollCallback(void *nd, int facing)
{
    DefTracker *tracker = nd;
    int dummy;
    if(strstr(tracker->def->name, "_Wall_") || strstr(tracker->def->name, "_Ceiling_") 
        || strstr(tracker->def->name, "_Floor_") || strnicmp(tracker->def->name, "grp", 3)==0)
    {
        if (rejectByCameraHeight(tracker->def,&dummy))
            return 0;
        else
            return 1;
    }
    else
        return 0;
}
/**********************************************************************func*
 * FindReasonableSpot
 *
 */
void clampVecToRoom( Vec3 v, BaseRoom *pRoom )
{
    // Clamp to room borders
    if(v[0]<0)
        v[0] = 0;
    //else if(v[0] > pRoom->size[0]*pRoom->blockSize)
    //    v[0] = pRoom->size[0]*pRoom->blockSize;

    if(v[2]<0)
        v[2] = 0;
    //else if(v[2] > pRoom->size[1]*pRoom->blockSize)
    //    v[2] = pRoom->size[1]*pRoom->blockSize;
}

typedef struct PositionDist
{
    Vec3 pos;
    float dist;
}PositionDist;

MP_DEFINE(PositionDist);

static int comparePositionDist(const void* pd1Data, const void* pd2Data)
{
    const PositionDist** pd1 = (const PositionDist**)pd1Data;
    const PositionDist** pd2 = (const PositionDist**)pd2Data;
    if( ((*pd1)->dist) - ((*pd2)->dist) > 0.f )
        return 1;
    else
        return -1;
}

bool FindReasonableSpot(BaseRoom *pRoom, RoomDetail *pDetail, Vec3 vecGood, Vec3 vecDest, float *hts )
{
    Vec3 v;

    subVec3(vecDest, pRoom->pos, v);
    MP_CREATE(PositionDist, 1024);

    // Clamp to room borders
    if(v[0]<0)
        v[0] = 0;
    else if(v[0] > pRoom->size[0]*pRoom->blockSize)
        v[0] = pRoom->size[0]*pRoom->blockSize;

    if(v[2]<0)
        v[2] = 0;
    else if(v[2] > pRoom->size[1]*pRoom->blockSize)
        v[2] = pRoom->size[1]*pRoom->blockSize;

    copyVec3(v, pDetail->mat[3]);
    if(detailCanFit(pRoom, pDetail,0))
    {
        // Success
        addVec3(v, pRoom->pos, vecDest);
        return true;
    }

    // OK, Still blocked by something.
    // v is now the new destination for us.

    // I tried an actual spiral algorithm, but it turned out I couldn't check one radius at a time
    // because the actual distance could be shorter from a greater radius.
    // Which meant I needed to gather up all the possible blocks and distance sort them anyways,
    // so figured I'd just do that with the simple old way.
    {
        Vec3 v1;
        Vec3 v2;
        Vec3 vSize;
        Vec3 vTry;
        Mat4 mat;
        float fStepX;
        float fStepZ;
        float fDistSqr = FLT_MAX;
        PositionDist ** testPositions = 0;
        int i, count;

        copyMat4(pDetail->mat, mat);
        copyVec3(v, mat[3]);
        baseDetailExtents(g_refCurDetail.p->info, mat, v1, v2);
        subVec3(v2, v1, vSize);
        subVec3(mat[3], vSize, v1);
        addVec3(mat[3], vSize, v2);

        if(v1[0]<0) v1[0]=0;
        if(v2[0]<0) v2[0]=0;
        if(v1[2]<0) v1[2]=0;
        if(v2[2]<0) v2[2]=0;

        fStepX = (v2[0]-v1[0])/50.0;
        if(fStepX<1.0) fStepX=1.0f;
        fStepZ = (v2[2]-v1[2])/50.0;
        if(fStepZ<1.0) fStepZ=1.0f;

        eaCreate(&testPositions);

        for(vTry[0]=v1[0]; vTry[0]<=v2[0]; vTry[0]+=fStepX)
        {
            for(vTry[2]=v1[2]; vTry[2]<=v2[2]; vTry[2]+=fStepZ)
            {
                if( !hts )
                {
                    PositionDist * pd = MP_ALLOC(PositionDist);
                    vTry[1] = vecGood[1];
                    copyVec3(vTry, pd->pos);
                    pd->dist = distance3Squared(vTry, mat[3]);
                    eaPush(&testPositions, pd);
                }
                else
                {
                    for( i = 0; i < 3; i++ )
                    {
                        PositionDist * pd = MP_ALLOC(PositionDist);
                        vTry[1] = hts[i];
                        copyVec3(vTry, pd->pos);
                        pd->dist = distance3Squared(vTry, mat[3]);
                        eaPush(&testPositions, pd);
                    }
                }
            }
        }

        count = eaSize(&testPositions);
        qsort( testPositions, count, sizeof(PositionDist*), comparePositionDist );

        // run slow check on them in zDist order
        for( i = 0; i < count; i++ )
        {
            copyVec3( testPositions[i]->pos, pDetail->mat[3] );
            if( detailCanFit(pRoom, pDetail,0) )
            {
                // Success
                addVec3(testPositions[i]->pos, pRoom->pos, vecDest);
                while(eaSize(&testPositions))
                {
                    PositionDist *deleteme = eaRemove(&testPositions,0);
                    MP_FREE( PositionDist, deleteme );
                }
                eaDestroy(&testPositions);
                return true;
            }
        }

        while(eaSize(&testPositions))
        {
            PositionDist *deleteme = eaRemove(&testPositions,0);
            MP_FREE( PositionDist, deleteme );
        }
        eaDestroy(&testPositions);
    }

    // Walk backwards from v towards vecGood until we find something which will fit.
    {
        Vec3 v2;
        Vec3 step;
        float dist;

        subVec3(vecGood, pRoom->pos, v2);
        subVec3(v2, v, step);
        normalVec3(step);
        dist = distance3Squared(v, v2);

        while(dist>0 && distance3Squared(step, zerovec3)<dist)
        {
            addVec3(v, step, v2);
            copyVec3(v2, pDetail->mat[3]);
            if(detailCanFit(pRoom, pDetail,0))
            {
                // Success
                addVec3(v2, pRoom->pos, vecDest);
                return true;
            }

            addVec3(step, step, step);
        }
    }

    copyVec3(vecGood, vecDest);
    subVec3(vecGood, pRoom->pos, pDetail->mat[3]);
    if(detailCanFit(pRoom, pDetail,0))
    {
        return true;
    }

    return false;
}

/**********************************************************************func*
 * UpdateDetailDrag
 *
 */
static bool UpdateDetailDrag(Mat4 matDestWorld, int x, int y )
{
    int hit;
    Vec3 start,end;
    CollInfo coll;
    bool bRet = false;
    GroupDef *def = 0;
    Mat4 mat;
    RoomDetail detail;

    if( g_refCurDetail.p )
        def = groupDefFindWithLoad(g_refCurDetail.p->info->pchGroupName);

    if(!def || !g_refCurDetail.p )
    {
        baseEdit_state.detail_dragging = false;
        s_yAdjusted = false;
        s_rotAdjusted = false;
        return false;
    }

    detail = *g_refCurDetail.p;

    if (shiftKeyState && detail.parentRoom && detail.info->eSurface != kSurface_Wall)
    {
        int error_num;

        s_DragBox.def = &s_RedBox;
        s_DragBox.defLastGood = NULL;
        s_DragBox.defLastGood_volume = NULL;

        copyMat4(detail.mat, mat);
        mat[3][0] += s_DragBox.pCurRoom->pos[0];
        mat[3][2] += s_DragBox.pCurRoom->pos[2];
        mat[3][1] = s_DragBox.mat[3][1];
        if (!s_yAdjusted)
        {
            s_yStart = y;
            s_yAdjusted = true;
            s_rotAdjusted = false;
        } else {
            float yDelta = (float) (s_yStart - y) * 0.05f;

            mat[3][1] += yDelta;
            s_yStart = y;
        }

        copyVec3(mat[3], s_DragBox.mat[3]);

        copyMat4(mat, detail.mat);
        detail.mat[3][0] -= s_DragBox.pCurRoom->pos[0];
        detail.mat[3][2] -= s_DragBox.pCurRoom->pos[2];

        if(detailCanFit(s_DragBox.pCurRoom, &detail, &error_num) && 
            detailAllowedInRoom(s_DragBox.pCurRoom, &detail, 0) && 
            detailAllowedInBase(&g_base, &detail))
        {
            s_DragBox.def = NULL;
            s_DragBox.defLastGood = &s_BlueBox;
            s_DragBox.defLastGood_volume = &s_GreenBox;
            copyMat4(mat, s_DragBox.matLastGood);
            bRet = true;
        }

        if(!bRet)
            base_setStatusMsg( "ItemCantFindSpot", kBaseStatusMsg_Error );

    } 
    else if (altKeyState && detail.parentRoom && detail.info->eSurface != kSurface_Wall)
    {
        RoomDetail detail = *g_refCurDetail.p;
        int error_num;

        s_DragBox.def = &s_RedBox;  
        s_DragBox.defLastGood = NULL;
        s_DragBox.defLastGood_volume = NULL;

        copyMat4(s_DragBox.mat, mat);
        mat[3][0] = detail.mat[3][0] + s_DragBox.pCurRoom->pos[0];
        mat[3][2] = detail.mat[3][2] + s_DragBox.pCurRoom->pos[2];
        mat[3][1] = s_DragBox.mat[3][1];
        if (!s_rotAdjusted)
        {
            s_rotAdjusted = true;
            s_rotStart = x;
        } else {
            float rotDelta = (float) (s_rotStart - x) * 0.05f;

            yawMat3(rotDelta, mat);
            s_rotStart = x;
        }

        copyMat4(mat, s_DragBox.mat);
        copyMat4(mat, detail.mat);
        detail.mat[3][0] -= s_DragBox.pCurRoom->pos[0];
        detail.mat[3][2] -= s_DragBox.pCurRoom->pos[2];

        if(detailCanFit(s_DragBox.pCurRoom, &detail, &error_num) && 
            detailAllowedInRoom(s_DragBox.pCurRoom, &detail, 0) && 
            detailAllowedInBase(&g_base, &detail))
        {
            s_DragBox.def = NULL;
            s_DragBox.defLastGood = &s_BlueBox;
            s_DragBox.defLastGood_volume = &s_GreenBox;
            copyMat4(mat, s_DragBox.matLastGood);
            bRet = true;
        }

        if(!bRet)
            base_setStatusMsg( "ItemCantFindSpot", kBaseStatusMsg_Error );

    } else {
        s_DragBox.def = &s_RedBox;
        s_DragBox.defLastGood = NULL;
        s_DragBox.defLastGood_volume = NULL;

        gfxCursor3d(x,y,10000,start,end);

        coll.node_callback = CollNodeCallback;

        hit = collGrid(0,start,end,&coll,0,COLL_DISTFROMSTART | COLL_HITANYSURF | COLL_NODECALLBACK);
        if(hit)
        {
            Vec3 v1;
            Vec3 v2;
            RoomDetail detail = *g_refCurDetail.p;
            DefTracker *tracker = coll.node;
            bool bAllowedInRoom, bAllowedInBase;
            int block[2], error_num;
            BaseRoom *pRoom;

            copyVec3(coll.mat[3], mat[3]);
            if (s_yAdjusted || ctrlKeyState)            
            {
                mat[3][1] = s_DragBox.mat[3][1];
            }

            copyVec3(mat[3], s_DragBox.mat[3]);
            if (detail.info->eSurface == kSurface_Wall)
            {
                Vec3    pyr;

                getMat3YPR(coll.mat,pyr);
                pyr[0] = pyr[2] = 0;
                pyr[1] = fixAngle(pyr[1] + RAD(180));
                createMat3YPR(mat,pyr);
                copyMat4(mat, s_DragBox.mat);
            } 
            else
            {
                copyMat4(s_DragBox.mat, mat);
            }
            copyMat4(mat, detail.mat);
            detail.mat[3][0] -= s_DragBox.pCurRoom->pos[0];
            detail.mat[3][2] -= s_DragBox.pCurRoom->pos[2];

            // If they have dragged over another room, change the current room
            {
                pRoom = baseRoomLocFromPos(&g_base, mat[3], block);
                if( pRoom &&  s_DragBox.pCurRoom != pRoom && detailAllowedInRoom(pRoom, &detail, pRoom != g_refCurDetail.refRoom.p  ))
                {
                    s_DragBox.pCurRoom = pRoom;
                }
            }

            bAllowedInRoom = detailAllowedInRoom(s_DragBox.pCurRoom, &detail, 0);
            bAllowedInBase = detailAllowedInBase(&g_base, &detail);

            if( detailCanFit(s_DragBox.pCurRoom, &detail, &error_num) && bAllowedInRoom && bAllowedInBase )
            {
                s_DragBox.def = NULL;
                s_DragBox.defLastGood = &s_BlueBox;
                s_DragBox.defLastGood_volume = &s_GreenBox;
                s_DragBox.iCntIntersecting = 0;

                copyMat4(mat, s_DragBox.matLastGood);

                bRet = true;
            }
            else if( bAllowedInRoom && bAllowedInBase  )
            {
                int i;
                RoomDetail *apDetails[MAX_INTERSECTING_DETAILS];

                switch( error_num )
                {
                case kDetErr_NoExtents:
                    base_setStatusMsg( "kDetErr_NoExtents", kBaseStatusMsg_Warning );
                    xcase kDetErr_BadPillarFit:
                    base_setStatusMsg( "kDetErr_BadPillarFit", kBaseStatusMsg_Warning );
                    xcase kDetErr_UsedBlock:
                    base_setStatusMsg( "kDetErr_UsedBlock", kBaseStatusMsg_Warning );
                    xcase kDetErr_NoSteps:
                    base_setStatusMsg( "kDetErr_NoSteps", kBaseStatusMsg_Warning );
                    xcase kDetErr_NoStack:
                    base_setStatusMsg( "kDetErr_NoStack", kBaseStatusMsg_Warning );
                    xcase kDetErr_Intersection:
                    base_setStatusMsg( "kDetErr_Intersection", kBaseStatusMsg_Warning );
                    xcase kDetErr_BlocksDoors:
                    base_setStatusMsg( "kDetErr_BlocksDoors", kBaseStatusMsg_Warning );
                    xcase kDetErr_BlocksDetail:
                    base_setStatusMsg( "kDetErr_BlocksDetail", kBaseStatusMsg_Warning );
                default:
                    break;
                }

                baseDetailExtents(g_refCurDetail.p->info, detail.mat, v1, v2);

                s_DragBox.iCntIntersecting = detailFindIntersecting(
                    s_DragBox.pCurRoom, v1, v2,
                    apDetails, MAX_INTERSECTING_DETAILS);

                for(i=0; i<MIN(s_DragBox.iCntIntersecting,MAX_INTERSECTING_DETAILS); i++)
                {
                    s_DragBox.aidDetails[i] = apDetails[i]->id;
                }

                if(FindReasonableSpot(s_DragBox.pCurRoom, &detail, s_DragBox.matLastGood[3], mat[3], NULL))
                {
                    s_DragBox.defLastGood = &s_BlueBox;
                    s_DragBox.defLastGood_volume = &s_GreenBox;
                    copyMat4(mat, s_DragBox.matLastGood);
                    bRet = true;
                }
                else
                    base_setStatusMsg( "ItemCantFindSpot", kBaseStatusMsg_Error );
            }            
            else if ( !bAllowedInRoom )
                base_setStatusMsg( "ItemNotAllowed", kBaseStatusMsg_Error );
            else if    ( !bAllowedInBase  )
                base_setStatusMsg( "ItemNotAllowedPlot", kBaseStatusMsg_Error );
        }
        else if(g_refCurDetail.p->info->eSurface!=kSurface_Wall)
        {
            // This for when the cursor doesn't intersect with anything.
            // I don't know what should be done here. For now, the same Y value
            //   as the last good location is assumed as the intersection plane
            //   for the mouse.
            Vec3 v;
            RoomDetail detail = *g_refCurDetail.p;
            float * ht_ptr, floor_hts[3] = { 0.f, 4.0f, 8.f }, ceil_hts[3] = { 32.f, 40.f, 48.f };

            s_DragBox.def = NULL;

            v[0] = start[0] + (s_DragBox.matLastGood[3][1] - start[1])*(end[0]-start[0])/(end[1]-start[1]);
            v[1] = s_DragBox.matLastGood[3][1];
            v[2] = start[2] + (s_DragBox.matLastGood[3][1] - start[1])*(end[2]-start[2])/(end[1]-start[1]);

            copyMat4(s_DragBox.matLastGood, detail.mat);
            detail.mat[3][0] -= s_DragBox.pCurRoom->pos[0];
            detail.mat[3][2] -= s_DragBox.pCurRoom->pos[2];

            if(g_refCurDetail.p->info->eSurface==kSurface_Floor)
                ht_ptr = floor_hts;
            else
                ht_ptr = ceil_hts;

            base_setStatusMsg( "OutsideRoomFindClose", kBaseStatusMsg_Warning );

            if(FindReasonableSpot(s_DragBox.pCurRoom, &detail, s_DragBox.matLastGood[3], v, ht_ptr) &&
                detailAllowedInRoom(s_DragBox.pCurRoom, &detail, 0) && detailAllowedInBase(&g_base, &detail))
            {
                s_DragBox.defLastGood = &s_BlueBox;
                s_DragBox.defLastGood_volume = &s_GreenBox;
                copyVec3(v, s_DragBox.matLastGood[3]);            
                bRet = true;
            }

            if(!bRet)
                base_setStatusMsg( "ItemCantFindSpot", kBaseStatusMsg_Error );
        }
    }

    if(matDestWorld)
        copyMat4(s_DragBox.matLastGood, matDestWorld);

    return bRet;
}

/**********************************************************************func*
 * EndDetailDrag
 *
 */
static void EndDetailDrag(Mat4 matDestWorld)
{
    if( detailAllowedInRoom(s_DragBox.pCurRoom, g_refCurDetail.p, 0) &&  // can't be placed in room
        detailAllowedInBase( &g_base, g_refCurDetail.p ) &&
        !(baseEdit_state.detail_dragging == kDragType_Sticky && window_collision()) ) // object hidden becuase mouse is over window
    {
        if(g_refCurDetail.id) // This detail was already placed in the room.
        {
            if(s_DragBox.pCurRoom != g_refCurDetail.refRoom.p )
            {
                // They've changed rooms
                const Detail *info = g_refCurDetail.p->info;
                int invID = g_refCurDetail.p->invID;

                baseedit_MoveDetailToRoom(g_refCurDetail.refRoom.p,g_refCurDetail.p,s_DragBox.pCurRoom, matDestWorld);
            }
            else
            {
                baseedit_MoveCurDetail(matDestWorld);
            }
        }
        else // This detail was from the inventory
        {
            baseedit_CreateDetail(s_DragBox.pCurRoom, g_refCurDetail.p->info, matDestWorld,g_refCurDetail.p->invID);
        }
    }
    baseEdit_state.detail_dragging = false;
    s_yAdjusted = false;
    s_rotAdjusted = false;
    s_DragBox.pCurRoom = NULL;
}

/**********************************************************************func*
 * HandleDetailDrag
 *
 */
static void HandleDetailDrag(void)
{
    Mat4 matDestWorld;
    int x, y;
    inpMousePos( &x, &y );

    if(UpdateDetailDrag(matDestWorld, x, y))
    {
        if(baseEdit_state.detail_dragging == kDragType_Drag
            &&  !baseEdit_state.dragging)
        {
            EndDetailDrag(matDestWorld);
        }
    }

    if(baseEdit_state.detail_dragging == kDragType_Drag
        &&  !baseEdit_state.dragging)
    {
        baseEdit_state.detail_dragging = false;
        s_yAdjusted = false;
        s_rotAdjusted = false;
    }
}

/**********************************************************************func*
 * baseedit_StartDetailDrag
 *
 */
void baseedit_StartDetailDrag(BaseRoom *pRoom, RoomDetail *pDetail, int sticky)
{
    int x, y;

    baseedit_CancelDrags();
    baseedit_CancelSelects();
    baseedit_SetCurDetail(pRoom, pDetail);
    if(!pDetail->id)
        copyMat4(unitmat, pDetail->mat);

    copyMat4(pDetail->mat, s_DragBox.mat);
    copyMat4(pDetail->mat, s_DragBox.matLastGood);
    scaleVec3(s_DragBox.clickPoint,0,s_DragBox.clickPoint);
    baseEdit_state.detail_dragging = sticky;

    inpMousePos(&x, &y);
    s_DragBox.pCurRoom = pRoom;
    UpdateDetailDrag(NULL, x, y);
}

//-----------------------------------------------------------------------------------------------------------------
// Plot Dragging ////////////////////////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------------------------------------------

/**********************************************************************func*
* UpdatePlotDrag
*
*/
static bool UpdatePlotDrag(int x, int y)
{
    Vec3 start,end;
    float xp, zp;
    Vec3 vScale;
    Mat4 mat;
    float fGridSize = g_base.roomBlockSize;
    int w, h;

    gfxCursor3d(x,y,10000,start,end);

    // Want to know where [start, end] intesect with xz plane
    // (x-x1)/(x2-x1) = (y-y1)/(y2-y1)
    // x = x1 + (y-y1)(x2-x1)/(y2-y1)
    // Likewise for z.
    xp = start[0] + (0 - start[1])*(end[0]-start[0])/(end[1]-start[1]);
    zp = start[2] + (0 - start[1])*(end[2]-start[2])/(end[1]-start[1]);

    if( s_iRot )
    {
        w = s_pBasePlotDrag->iLength;
        h = s_pBasePlotDrag->iWidth;
    }
    else
    {
        w = s_pBasePlotDrag->iWidth;
        h = s_pBasePlotDrag->iLength;
    }

    copyMat4(unitmat, mat);
     mat[3][0] = (floor(xp/fGridSize))*fGridSize;
      mat[3][1] = 0;
    mat[3][2] = (floor(zp/fGridSize))*fGridSize;

    vScale[0] = fGridSize*w;
     vScale[1] = 9;
    vScale[2] = fGridSize*h;

    if(!basePlotFit(&g_base, w, h, mat[3], 1))
    {
        baseDrawSelObj(0, &s_RedBox, mat, vScale, fGridSize);
        base_setStatusMsg( "PlotTooSmall", kBaseStatusMsg_Error );
        return false;
    }
    else if( !basePlotAllowed(&g_base, s_pBasePlotDrag) )
    {
        baseDrawSelObj(0, &s_RedBox, mat, vScale, fGridSize);
        base_setStatusMsg( "PlotRestrictions", kBaseStatusMsg_Error );
        return false;
    }
    else
    {
         baseDrawSelObj(0,&s_BlueBox, mat, vScale, fGridSize);
        copyVec3( mat[3], s_vPos);
        s_vPos[0] -= fGridSize*w/2;
        s_vPos[2] -= fGridSize*h/2;
    }

    return true;
}

/**********************************************************************func*
* EndRoomDrag
*
*/
static void EndPlotDrag()
{
    baseClientSendPlot( s_pBasePlotDrag, s_vPos, s_iRot );
    baseEdit_state.plot_dragging = false;
}

/**********************************************************************func*
* HandleRoomDrag
*
*/
static void HandlePlotDrag(void)
{
    int x, y;
    inpMousePos( &x, &y );

    if(UpdatePlotDrag(x, y))
    {
        if(baseEdit_state.plot_dragging == kDragType_Drag
            &&  !baseEdit_state.dragging)
        {
            EndPlotDrag();
        }
    }

    if(baseEdit_state.plot_dragging == kDragType_Drag
        &&  !baseEdit_state.dragging)
    {
        baseEdit_state.plot_dragging = false;
    }
}

/**********************************************************************func*
* baseedit_StartPlotDrag
*
*/
void baseedit_StartPlotDrag( int type, const BasePlot *plot )
{
    baseedit_CancelDrags();
    baseedit_CancelSelects();

    s_pBasePlotDrag = plot;
    s_iRot = false;
    baseEdit_state.plot_dragging = type;
}

//-----------------------------------------------------------------------------------------------------------------
// Mouse Input ////////////////////////////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------------------------------------------

/**********************************************************************func*
 * UpdateMouseOver
 *
 */
static bool UpdateMouseOver(int x, int y, Mat4 matWorld, BaseRoom **ppRoom, int aBlock[2],
    RoomDetail **ppDetail, int *piSurface, int *piDecor, Vec3 *intersectOut)
{
    Vec3 start,end, intersect;
    CollInfo coll;
    DefTracker *tracker = NULL;
    BaseRoom *pRoom = NULL;

    int hit, hit_curDetail = false;

    gfxCursor3d(x,y,10000,start,end);

    coll.node_callback = CollNodeIgnoreEditorOnlyAndVolumeTriggersCb;
    hit = collGrid(0,start,end,&coll,0,COLL_DISTFROMSTART | COLL_HITANYSURF | COLL_NODECALLBACK);

    if( g_refCurDetail.p )
    {
        Vec3 min, max;
        BaseRoom *room = baseGetRoom(&g_base,g_refCurDetail.p->parentRoom);
        baseDetailExtents( g_refCurDetail.p->info, g_refCurDetail.p->mat, min, max );
        if (room) 
        {
            addVec3(min,room->pos,min);
            addVec3(max,room->pos,max);
        }
        if( lineBoxCollision( start, end, min, max, intersect ) )
        {
            float detail_dist = distance3Squared( start, intersect);
            if( detail_dist < coll.sqdist ) {
                hit_curDetail = true;
                if (intersectOut) { //save the relative location
                    subVec3(intersect,room->pos,*intersectOut);
                    subVec3(*intersectOut,g_refCurDetail.p->mat[3],*intersectOut);
                }
            }
        }
    }

    if(hit)
    {
        tracker = coll.node;
        while(tracker->parent && strnicmp(tracker->parent->def->name, "grp", 3)!=0)
            tracker = tracker->parent;
    }

    if( hit_curDetail )
    {
        if(ppDetail)
            *ppDetail = g_refCurDetail.p;
    }
    if(tracker)
    {
        int iSurface = -1;
        RoomDetail *pDetail = NULL;
        int block[2];
        Mat4 mat;

        copyMat4(tracker->mat, mat);

        pRoom = baseRoomLocFromPos(&g_base, mat[3], block);
        if(pRoom)
        {
            iSurface = strstriConst(tracker->def->name,"ceiling") ? 1 : 0;
            pDetail = roomGetDetailFromWorldMat(pRoom, mat);
        }

        if(matWorld)
            copyMat4(mat, matWorld);

        if(ppRoom)
            *ppRoom = pRoom;

        if(aBlock)
            memcpy(aBlock, block, sizeof(block));

        if(ppDetail && !hit_curDetail)
            *ppDetail = pDetail;

        if(piSurface)
            *piSurface = iSurface;

        if(piDecor)
        {
            int iDecor = baseFindDecorIdx(tracker->def->name);
            if( iDecor >= 0 && iDecor < ROOMDECOR_MAXTYPES )
                *piDecor = iDecor;
        }
    }
    else
    {
        Mat4 mat;
        int block[2];
        float fGridSize = g_base.roomBlockSize;
        // Want to know where [start, end] intesect with xz plane
        // (x-x1)/(x2-x1) = (y-y1)/(y2-y1)
        // x = x1 + (y-y1)(x2-x1)/(y2-y1)
        // Likewise for z.
        mat[3][0] = fGridSize*floor((start[0] + (0 - start[1])*(end[0]-start[0])/(end[1]-start[1]))/fGridSize)+fGridSize/2;
        mat[3][1] = 0;
        mat[3][2] = fGridSize*floor((start[2] + (0 - start[1])*(end[2]-start[2])/(end[1]-start[1]))/fGridSize)+fGridSize/2;

        pRoom = baseRoomLocFromPos(&g_base, mat[3], block);
        if(pRoom)
        {
            baseedit_CopyMatFromBlock(pRoom, block, 0, mat);
        }

        if(matWorld)
            copyMat4(mat, matWorld);

        if(ppRoom)
            *ppRoom = pRoom;

        if(aBlock)
            memcpy(aBlock, block, sizeof(block));

        if(ppDetail)
            *ppDetail = NULL;

        if(piSurface)
            *piSurface = 0;

        if(piDecor)
            *piDecor = 0;
    }

    return (pRoom!=NULL);
}

/**********************************************************************func*
 * HandleCursor
 *
 */
static void HandleCursor(void)
{
    int iSurface = -1;
    RoomDetail *pDetail = NULL;
    BaseRoom *pRoom = NULL;
    int x, y, block[2];
    Vec3 intersect;

    inpMousePos(&x, &y);

    if(baseEdit_state.dragging)
    {
        if(UpdateMouseOver(g_iXDragStart, g_iYDragStart, NULL, &pRoom, block, &pDetail, &iSurface, NULL, &intersect))
        {
            baseedit_CancelSelects();
            
            if(pDetail)
            {
                baseEdit_state.detail_dragging = kDragType_Drag;
                baseedit_SetCurDetail(pRoom, pDetail);
                copyVec3(intersect,s_DragBox.clickPoint);
                copyMat4(pDetail->mat, s_DragBox.mat);
                copyMat4(pDetail->mat, s_DragBox.matLastGood);
                gridCacheInvalidate();
                UpdateDetailDrag(NULL, g_iXDragStart, g_iYDragStart);
            }
            else if(pRoom)
            {
                baseedit_SetCurRoom(pRoom, block);
                s_iDragSurface = iSurface;
                baseEdit_state.surface_dragging = kDragType_Drag;
            }
        }

    }
}

/**********************************************************************func*
 * HandleInput
 *
 */
#define CAM_SPEED    10

static void HandleInput(void)
{
    static bool s_bLooking = false;
    CBox box;
    Entity * e = playerPtr();
    BuildCBox(&box, 0, 0, 2000, 2000);

    s_fX = ENTPOSX(e);
    s_fZ = ENTPOSZ(e);

    if (baseEdit_state.turnleft)
        s_fAngle = addAngle(s_fAngle, -RAD(optionGetf(kUO_SpeedTurn)) * TIMESTEP);
    if (baseEdit_state.turnright)
        s_fAngle = addAngle(s_fAngle, RAD(optionGetf(kUO_SpeedTurn)) * TIMESTEP);

    if(baseEdit_state.mouselook)
    {
        extern int mouse_dx,mouse_dy;
        F32        y_sign = 1;

        if (!optionGet(kUO_MouseInvert))
            y_sign = -1;

        if(s_bLooking)
        {
            // 500 is magic number game has used since beginning of time
            s_fAzimuth = addAngle(s_fAzimuth, optionGetf(kUO_MouseSpeed)* y_sign * -mouse_dy / 500.0F);
            if(s_fAzimuth>RAD(89))
                s_fAzimuth = RAD(89);
            else if(s_fAzimuth<RAD(-89))
                s_fAzimuth = RAD(-89);
            s_fAngle = addAngle(s_fAngle, optionGetf(kUO_MouseSpeed) * mouse_dx / 500.0F);
        }
        s_bLooking = true;
    }
    else
    {
        s_bLooking = false;

        if( gWindowDragging || gScrollBarDragging )
            baseedit_CancelDrags();
        if (!mouseCollision(&box) || window_collision() )
            return;
        if(baseEdit_state.room_dragging)
        {
            HandleRoomDrag();
        }
        else if( baseEdit_state.room_moving )
        {
            HandleRoomMove();
        }
        else if(baseEdit_state.detail_dragging)
        {
            HandleDetailDrag();
        }
        else if(baseEdit_state.surface_dragging)
        {
            HandleSurfaceDrag();
        }
        else if(baseEdit_state.door_dragging)
        {
            HandleDoorDrag();
        }
        else if( baseEdit_state.plot_dragging )
        {
            HandlePlotDrag();
        }
        else if(mouseCollision(&box))
        {
            HandleCursor();
        }
    }
}

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

/**********************************************************************func*
 * baseedit_SetCurDecor
 *
 */
void baseedit_SetCurDecor(int iDecor)
{
    if( !baseInventoryIsStyle() )
        return;

    if(!g_iDecorFadeTimer)
        g_iDecorFadeTimer = timerAlloc();

    timerStart(g_iDecorFadeTimer);

    if( iDecor >= 0 && iDecor < ROOMDECOR_MAXTYPES )
        g_iDecor = iDecor;

}

/**********************************************************************func*
 * baseedit_SetCurRoom
 *
 */
void baseedit_SetCurRoom(BaseRoom *pRoom, int block[2])
{
    if(pRoom)
    {
        g_refCurRoom.p = pRoom;
        g_refCurRoom.id = pRoom->id;
        if (baseEdit_state.detail_dragging)
            s_DragBox.pCurRoom = pRoom;
    }
    pRoom = g_refCurRoom.p;

    if(block)
        memcpy(g_refCurBlock.block, block, sizeof(g_refCurBlock.block));

    block = g_refCurBlock.block;

    if(pRoom)
    {
        g_refCurBlock.p = roomGetBlock(pRoom, block);
        if(!g_refCurBlock.p)
        {
            if(!baseedit_IsDoorway(pRoom, block, NULL))
            {
                if(block[0] >= pRoom->size[0])
                    block[0] = pRoom->size[0]-1;
                else if(block[0] < 0)
                    block[0] = 0;

                if(block[1] >= pRoom->size[1])
                    block[1] = pRoom->size[1]-1;
                else if(block[1] < 0)
                    block[1] = 0;

                g_refCurBlock.p = roomGetBlock(pRoom, block);
            }
        }
    }
    else
    {
        g_refCurBlock.p = NULL;
    }
}

/**********************************************************************func*
 * baseedit_LookAtRoomBlock
 *
 */
void baseedit_LookAtRoomBlock(BaseRoom *pRoom, int block[2])
{

    if(pRoom)
    {
        BaseBlock * pBlock = roomGetBlock(pRoom, block );
        if( pBlock )
        {
            baseClientSetPos(    (float)pRoom->blockSize*block[0]+pRoom->blockSize/2 + pRoom->pos[0],
                                (float)pBlock->height[0],
                                (float)pRoom->blockSize*block[1]+pRoom->blockSize/2+ pRoom->pos[2] );
        }
    }
}

/**********************************************************************func*
* baseedit_LookAtCur
*
*/
void baseedit_LookAtCur() // Not used because may put you in bad spot.
{
    if( g_refCurDetail.p )
    {
        baseClientSetPos(    (float)g_refCurRoom.p->pos[0]+g_refCurDetail.p->mat[3][0],
                            (float)g_refCurRoom.p->pos[1]+g_refCurDetail.p->mat[3][1],
                            (float)g_refCurRoom.p->pos[2]+g_refCurDetail.p->mat[3][2] );

    }
    else if ( g_refCurRoom.p )
        baseedit_LookAtRoomBlock(g_refCurRoom.p, g_refCurBlock.block);
}

/**********************************************************************func*
 * baseedit_RotateRoom
 *
 */
void baseedit_RotateRoom(void)
{
      s_iRot--;
    if(s_iRot<0)
        s_iRot=3;
}

/**********************************************************************func*
 * baseedit_Select
 *
 */
void baseedit_Select(int x, int y)
{
    int iSurface = -1;
    int iDecor;
    RoomDetail *pDetail = NULL;
    BaseRoom *pRoom = NULL;
    int block[2];
    Mat4 matWorld;

    if(baseEdit_state.room_dragging == kDragType_Sticky)
    {
        Vec3 vec;

        if(UpdateRoomDrag(s_iXSize, s_iZSize, vec, x, y))
        {
            EndRoomDrag(s_iXSize, s_iZSize, vec);
        }
    }
    else if(baseEdit_state.room_moving == kDragType_Sticky)
    {
        Vec3 vec;
        if(UpdateRoomMove(vec, x, y))
        {
            EndRoomMove(vec);
        }
    }
    else if(baseEdit_state.detail_dragging == kDragType_Sticky)
    {
        Mat4 matWorld;
        if(UpdateDetailDrag(matWorld, x, y))
        {
            EndDetailDrag(matWorld);
        }
    }
    else if(baseEdit_state.surface_dragging == kDragType_Sticky)
    {
        int iHeight;
        UpdateSurfaceDrag(&iHeight, x, y);
        EndSurfaceDrag(iHeight);
    }
    else if(baseEdit_state.door_dragging == kDragType_Sticky)
    {
        if(UpdateDoorDrag(x, y))
            EndDoorDrag();
    }
    else if(baseEdit_state.plot_dragging == kDragType_Sticky)
    {
        if(UpdatePlotDrag(x, y))
            EndPlotDrag();
    }
    else if(UpdateMouseOver(x, y, matWorld, &pRoom, block, &pDetail, &iSurface, &iDecor, NULL))
    {
        if(pDetail)
        {
            baseedit_CancelSelects();
            baseedit_SetCurDetail(pRoom, pDetail);

            if( baseedit_Mode() == kBaseEdit_Architect )
                baseRoomSetStatTab();
        }
        else if(pRoom)
        {
            baseedit_SetCurDecor(iDecor);
            baseedit_SetCurRoom(pRoom, block);
            basepropsSetDecorIdx(iDecor);
            baseedit_SetCurDetail(NULL, NULL);
        }
    }
}

/**********************************************************************func*
 * baseCenterXY
 *
 */
void baseCenterXY( int x, int y )
{
    Vec3 start,end;
    CollInfo coll;
    DefTracker *tracker = NULL;
    BaseRoom *pRoom = NULL;
    BaseBlock *pBlock = NULL;

    int hit;

    gfxCursor3d(x,y,10000,start,end);
    coll.node_callback = BaseEditCollCallback;
    gridCacheInvalidate();
    hit = collGrid(0,start,end,&coll,0,COLL_DISTFROMSTART | COLL_HITANYSURF | COLL_NODECALLBACK);
    if(hit)
    {
        int block[2];
        float height[2];
        Mat4 mat;

        tracker = coll.node;

        if( strstriConst( tracker->def->name, "wall" ) ||
            strstriConst( tracker->def->name, "ceiling") ||
            strstriConst( tracker->def->name, "plot"))
            return;

        copyMat4(tracker->mat, mat);
        pRoom = baseRoomLocFromPos(&g_base, mat[3], block);
        hit = false;
        if( pRoom ) {
            pBlock = roomGetBlock(pRoom,block);
            roomGetBlockHeight(pRoom,block,height);
        }
        if(pRoom && (pBlock || baseedit_IsDoorway(pRoom,block,NULL)))
        {
            baseClientSetPos(    (float)coll.mat[3][0],
                                (float)height[0],
                                (float)coll.mat[3][2]);
        }
    }
}

/**********************************************************************func*
 * baseedit_SetCamDist
 *
 */
void baseedit_SetCamDist(void)
{
    s_fDist += inpLevel(INP_MOUSEWHEEL)*-20.0f;
    if(s_fDist<MIN_ZOOM_DIST) s_fDist = MIN_ZOOM_DIST;
    if(s_fDist>MAX_ZOOM_DIST) s_fDist = MAX_ZOOM_DIST;
}

/**********************************************************************func*
* baseedit_SetCamDistExact
*
*/
void baseedit_SetCamDistAngle(float dist, float angle)
{
    if( dist > 0 )
        game_state.camdist = dist;
    if( angle > 0 )
        control_state.pyr.cur[0] = -angle;
    else
        control_state.pyr.cur[0] = angle;


    if(control_state.pyr.cur[0]>RAD( 89)) control_state.pyr.cur[0] = RAD(89);
    if(control_state.pyr.cur[0]<RAD(-89)) control_state.pyr.cur[0] = RAD(-89);

    if(game_state.camdist<MIN_ZOOM_DIST) game_state.camdist = MIN_ZOOM_DIST;
    if(game_state.camdist>MAX_ZOOM_DIST) game_state.camdist = MAX_ZOOM_DIST;
}

/**********************************************************************func*
 * baseedit_RotateDraggedDetail
 *
 */
void baseedit_RotateDraggedDetail(int clockwise)
{
    if(baseEdit_state.detail_dragging)
    {
        if(clockwise)
        {
            yawMat3(PI/2, s_DragBox.mat);
            yawMat3(PI/2, s_DragBox.matLastGood);
        }
        else
        {
            yawMat3(-PI/2, s_DragBox.mat);
            yawMat3(-PI/2, s_DragBox.matLastGood);
        }
    }
}


/**********************************************************************func*
 * baseedit_Rotate
 *
 */
void baseedit_Rotate(int clockwise)
{
     if( baseEdit_state.room_dragging )
        baseedit_RotateRoom();
    if( baseEdit_state.room_moving )
    {
        baseedit_RotateRoom();
        stashTableClear(roomHashPositionsVisited);
    }
    if( baseEdit_state.plot_dragging )
        s_iRot = !s_iRot;
    else
        baseedit_RotateCurDetail(clockwise);
}

/**********************************************************************func*
 * baseedit_Process
 *
 */
void baseedit_Process(void)
{
    Mat4 mat;
    Entity *e = playerPtr();

    if (!e)
        return;

    baseEditKeybindInit();

    if(!g_refCurRoom.p)
    {
        if(eaSize(&g_base.rooms))
        {
            baseedit_SetCurRoom(g_base.rooms[0], NULL);
            baseedit_LookAtCurRoomBlock();
        }
        else
        {
            s_fX = 0;
            s_fZ = 0;
        }

        s_fCurX = s_fX;
        s_fCurZ = s_fZ;

        s_fCurDist = s_fDist;
        s_fCurAngle = s_fAngle;
        s_fCurAzimuth = s_fAzimuth;

        if(!g_iDecorFadeTimer)
            g_iDecorFadeTimer = timerAlloc();
    }

    if(!s_RedBox.pDef)
    {
        char buff[256];
        sprintf( buff, "_Bases_ground_generic_%d", g_base.roomBlockSize );
        s_RedBox.pDef = groupDefFindWithLoad(buff);
        s_RedBox.tint[0] = CreateColor( 45, 0,  18, 255);
        s_RedBox.tint[1] = CreateColor(128, 0, 40, 255);

        sprintf( buff, "_Bases_ground_generic_%d", g_base.roomBlockSize );
        s_BlueBox.pDef = groupDefFindWithLoad(buff);
        s_BlueBox.tint[0] = CreateColor(0,  18,  45, 255);
        s_BlueBox.tint[1] = CreateColor(0, 90, 180, 255);

        sprintf( buff, "_Bases_ground_generic_%d", g_base.roomBlockSize );
        s_YellowBox.pDef = groupDefFindWithLoad(buff);
        s_YellowBox.tint[0] = CreateColor(18,  18,  0, 255);
        s_YellowBox.tint[1] = CreateColor(128, 128, 0, 255);

        sprintf( buff, "_Bases_ground_generic_%d", g_base.roomBlockSize );
        s_GreenBox.pDef = groupDefFindWithLoad(buff);
        s_GreenBox.tint[0] = CreateColor(0,  30,  0, 255);
        s_GreenBox.tint[1] = CreateColor(0, 128, 0, 255);

        s_RedPillar.pDef = groupDefFindWithLoad("_Bases_pillar_generic_32");
        s_RedPillar.tint[0] = CreateColor( 45, 0,  18, 255);
        s_RedPillar.tint[1] = CreateColor(128, 0, 40, 255);

        s_BluePillar.pDef = groupDefFindWithLoad("_Bases_pillar_generic_32");
        s_BluePillar.tint[0] = CreateColor(0,  18,  45, 255);
        s_BluePillar.tint[1] = CreateColor(0, 90, 180, 255);
    }

    if(!g_refCurRoom.p)
        return;

    copyMat4(ENTMAT(e),mat);

    if(!g_refCurDetail.p)
    {
         if( control_state.pyr.cur[0] > 0 )
            baseedit_CopyMatFromBlock(g_refCurRoom.p, g_refCurBlock.block, 1, mat);
        else
            baseedit_CopyMatFromBlock(g_refCurRoom.p, g_refCurBlock.block, 0, mat);

        baseDrawSelObj(0, &s_BlueBox, mat, zerovec3, g_base.roomBlockSize);
    }

    HandleInput();
}

/**********************************************************************func*
 * baseedit_InvalidateGroupDefs
 *
 */
void baseedit_InvalidateGroupDefs(void)
{
    int i;

    for(i=0; i<5; i++)
    {
        selbase_grp[i] = NULL;
    }

    s_RedBox.pDef = NULL;
    s_BlueBox.pDef = NULL;
    s_YellowBox.pDef = NULL;
    s_GreenBox.pDef = NULL;
    s_RedPillar.pDef = NULL;
    s_BluePillar.pDef = NULL;
}

/**********************************************************************func*
 * baseedit_GetDetailFromChild
 *
 */
int baseedit_GetDetailFromChild(DefTracker *tracker, RoomDetail **ppDetail)
{
    if(tracker)
    {
        while(tracker->parent && strnicmp(tracker->parent->def->name, "grp", 3)!=0)
            tracker = tracker->parent;

        if(tracker)
        {
            RoomDetail *pDetail = NULL;
            BaseRoom *pRoom = baseRoomLocFromPos(&g_base, tracker->mat[3], NULL);
            if(pRoom)
            {
                pDetail = roomGetDetailFromWorldMat(pRoom, tracker->mat);

                if(ppDetail)
                {
                    *ppDetail = pDetail;
                }

                if( pDetail )
                    return pDetail->id;
                else
                    return 0;
            }
        }
    }

    return 0;
}

int baseedit_IsClickableDetailEnt(Entity *ent)
{
    RoomDetail *pDetail = NULL;
    BaseRoom *pRoom = NULL;
    
    if (!g_base.rooms)
        return 0;

    pRoom = baseRoomLocFromPos(&g_base, ENTPOS(ent), NULL);
    if(pRoom)
    {
        pDetail = roomGetDetailFromWorldPos(pRoom, ENTPOS(ent));
        if (!pDetail)
            return 0;

        if (pDetail->info->eFunction == kDetailFunction_Contact ||
            pDetail->info->eFunction == kDetailFunction_AuxContact ||
            pDetail->info->eFunction == kDetailFunction_Workshop ||
            pDetail->info->eFunction == kDetailFunction_Store || 
            pDetail->info->eFunction == kDetailFunction_StorageSalvage)
        {
            return 1;
        }
    }
    return 0;
}

int baseedit_IsPassiveDetailEnt(Entity *ent)
{
    RoomDetail *pDetail = NULL;
    BaseRoom *pRoom = NULL;

    if (!g_base.rooms)
        return 0;

    pRoom = baseRoomLocFromPos(&g_base, ENTPOS(ent), NULL);
    if(pRoom)
    {
        pDetail = roomGetDetailFromWorldPos(pRoom, ENTPOS(ent));
        if (!pDetail)
            return 0;

        if (pDetail->info->eFunction == kDetailFunction_Turret ||
            pDetail->info->eFunction == kDetailFunction_Field)
        {
            return 0;
        }
        return 1; //everything but turrets and fields are passive
    }
    return 0;
}


static int detailVisible(RoomDetail *det)
{
    Vec3        view_pos,pos, goalpos;
    Vec3 min, max;
    CollInfo    coll;
    int            i,j;
    U32            collflags = COLL_DISTFROMSTART|COLL_HITANY|COLL_NODECALLBACK;
    BaseRoom *room;
    if (!det || !det->parentRoom)
        return false;

    room = baseGetRoom(&g_base,det->parentRoom);
    if (!room) 
        return false;
    
    baseDetailExtents( det->info, det->mat, min, max );
    addVec3(min,max,goalpos);
    scaleVec3(goalpos,0.5,goalpos); //set goal to midpoint of extents box
    addVec3(goalpos,room->pos,goalpos);
    copyVec3(goalpos,pos);    

    coll.node_callback = BaseEditCollCallback;

    for(i=0;i<3;i++)
    {
        mulVecMat4(pos,cam_info.viewmat,view_pos);
        if (gfxSphereVisible(view_pos,1.5f))
            break;
        pos[1] += 3;
    }
    if (i >= 3)
        return 0;

    for(j=0;j<3;j++)
    {
        copyVec3(goalpos,pos);        
        moveVinX(pos,cam_info.cammat,(j - 1) * 1.f);

        for(i=0;i<3;i++)
        {
            if (!collGrid(0,cam_info.cammat[3],pos,&coll,0,collflags))
                return 1;
            pos[1] += 3;
        }
    }
    return 0;
}


static int selectNextDetail(bool bBackwards) {
    int            i,j,best_idx=-1, fallbackIdx=-1;    
    F32            referenceDist, bestDist, fallbackDist;
    Entity        *player = playerPtr();
    int last_idx;
    Base *base = &g_base;
    int rooms, details;

    // Find the previously selected entity if it exists
    if (g_refCurDetail.id)
    {
        Vec3 pos;
        BaseRoom *room = g_refCurDetail.refRoom.p;
        copyVec3(g_refCurDetail.p->mat[3],pos);
        addVec3(pos,room->pos,pos);
        referenceDist = distance3Squared(ENTPOS(player), pos);
        last_idx = g_refCurDetail.id;
    }
    else {
        referenceDist = 0.0f;
        last_idx = -1;
    }
    best_idx = -1;

    rooms = eaSize(&base->rooms);
    for (i=0;i<rooms;i++)
    {
        BaseRoom *room = base->rooms[i];
        details = eaSize(&room->details);
        for (j=0;j<details;j++)
        {
            Vec3 pos;
            RoomDetail *detail = room->details[j];
            F32 dist;

            if (detail->id == last_idx)
                continue;

            if (!detailVisible(detail))
                continue;
            copyVec3(detail->mat[3],pos);
            addVec3(pos,room->pos,pos);
            dist = distance3Squared(ENTPOS(player), pos);
            if (fallbackIdx==-1
                || !bBackwards && (dist < fallbackDist || dist == fallbackDist && detail->id < fallbackIdx)
                || bBackwards && (dist > fallbackDist || dist == fallbackDist && detail->id > fallbackIdx))
            {
                fallbackDist = dist;
                fallbackIdx = detail->id;
            }

            if (!bBackwards && (dist < referenceDist || (dist == referenceDist && detail->id < last_idx))
                || bBackwards && (dist > referenceDist || (dist == referenceDist && detail->id > last_idx)))
            {
                continue;
            }

            if (best_idx!=-1  // We have a previous best
                && ( !bBackwards && (dist > bestDist || dist == bestDist && detail->id > best_idx)// AND This one is father than the previous best
                || bBackwards && (dist < bestDist || dist == bestDist && detail->id < best_idx)))
            {
                continue;
            }

            best_idx = detail->id;
            bestDist = dist;
        }
    }
    if (best_idx==-1)
    {
        if (fallbackIdx!=-1)
            return fallbackIdx;
        return last_idx;
    }
    return best_idx;
}

void baseedit_SelectNext(bool bBackwards) {
    int result = selectNextDetail(bBackwards);
    int i, j;
    Base *base = &g_base;
    for (i=eaSize(&base->rooms)-1;i>=0;i--)
    {
        BaseRoom *room = base->rooms[i];
        for (j=eaSize(&room->details)-1;j>=0;j--)
        {
            if (room->details[j]->id == result) 
            {
                baseedit_CancelSelects();
                baseedit_SetCurDetail(room, room->details[j]);
                return;
            }
        }
    }
    baseedit_CancelSelects();
}


/* End of File */
