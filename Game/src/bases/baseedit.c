/***************************************************************************
 *     Copyright (c) 2005-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include <utilitieslib/components/earray.h>
#include <utilitieslib/utils/mathutil.h>

#include "graphics/gfxwindow.h"

#include "win/input.h"
#include "cmdparse/cmdgame.h"         // for game_state
#include "UI/uiBaseInput.h"

#include "bases/bases.h"
#include "bases/baseclientsend.h"
#include "bases/basedata.h"
#include "bases/baseinv.h"
#include "bases/baseedit.h"
#include "bases/basesend.h"

#include "entity/entity.h"
#include "player/player.h"
#include "UI/uiNet.h"
#include <utilitieslib/utils/timing.h>
#include "UI/uiDialog.h"
#include "UI/sprite/sprite_text.h"
#include <utilitieslib/components/ArrayOld.h>
#include <utilitieslib/language/MessageStore.h>
#include "language/langClientUtil.h"
#include <utilitieslib/language/MessageStoreUtil.h>

/**********************************************************************func*
 * baseedit_Mode
 *
 */
int baseedit_Mode(void)
{
    return game_state.edit_base;
}

/**********************************************************************func*
 * baseedit_SetCurDetail
 *
 */
void baseedit_SetCurDetail(BaseRoom *pRoom, RoomDetail *pDetail)
{
    g_refCurDetail.refRoom.p = pRoom;
     g_refCurDetail.refRoom.id = (pRoom ? pRoom->id : 0);

    g_refCurDetail.p = pDetail;
    g_refCurDetail.id = (pDetail ? pDetail->id : 0);

    if( pRoom )
        baseedit_SetCurRoom( pRoom, NULL );
}

/**********************************************************************func*
 * baseedit_SyncUIState
 *
 */
void baseedit_SyncUIState(void)
{
    if(g_refCurRoom.p)
    {
        g_refCurRoom.p = baseGetRoom(&g_base, g_refCurRoom.id);
        if(!g_refCurRoom.p)
        {
            memset(&g_refCurRoom, 0, sizeof(BaseRoomRef));
            memset(&g_refCurBlock, 0, sizeof(BaseBlockRef));
        }
        else
            g_refCurBlock.p = roomGetBlock(g_refCurRoom.p, g_refCurBlock.block);
    }
    else
    {
        memset(&g_refCurDetail, 0, sizeof(RoomDetailRef));
        memset(&g_refCurBlock, 0, sizeof(BaseBlockRef));
    }

    if(g_refCurDetail.p)
    {
        g_refCurDetail.p = NULL;

        g_refCurDetail.refRoom.p = baseGetRoom(&g_base, g_refCurDetail.refRoom.id);
        if(g_refCurDetail.refRoom.p)
        {
            g_refCurDetail.p = roomGetDetail(g_refCurDetail.refRoom.p, g_refCurDetail.id);
        }

        if(!g_refCurDetail.p)
            memset(&g_refCurDetail, 0, sizeof(RoomDetailRef));
    }
}



/**********************************************************************func*
 * baseedit_CopyMatFromBlock
 *
 */
void baseedit_CopyMatFromBlock(BaseRoom *pRoom, int block[2], int iSurface, Mat4 mat)
{
    BaseBlock *pBlock;
    F32 height[2];

    if(!pRoom)
        return;
    pBlock = roomGetBlock(pRoom, block);
    if(!pBlock)
    {
        baseedit_IsDoorway(pRoom, block, height);
    }
    else
    {
        height[iSurface] = pBlock->height[iSurface];
    }

    copyMat4(unitmat, mat);

    mat[3][0] = pRoom->pos[0] + block[0] * pRoom->blockSize + pRoom->blockSize/2;
    mat[3][1] = pRoom->pos[1] + height[iSurface];
    mat[3][2] = pRoom->pos[2] + block[1] * pRoom->blockSize + pRoom->blockSize/2;

    if(iSurface)
    {
        rollMat3(RAD(180), mat);
    }
}

/**********************************************************************func*
 * baseedit_SetRoomTexture
 *
 */
void baseedit_SetRoomTexture(BaseRoom *pRoom, int partIdx, char *pchTexture)
{
    baseClientSendTexSwap(pRoom, partIdx, pchTexture);
}

/**********************************************************************func*
 * baseedit_SetRoomGeometry
 *
 */
void baseedit_SetRoomGeometry(BaseRoom *pRoom, int partIdx, char *pchGeometry)
{
    baseClientSendGeoSwap(pRoom, partIdx, pchGeometry);
}

void baseedit_SetRoomTint( BaseRoom *pRoom, int partIdx, int c1, int c2 )
{
    Color colors[2];

    colors[0].integer = c1;
    colors[1].integer = c2;

    baseClientSendTint( pRoom, partIdx, colors );
}

/**********************************************************************func*
 * baseedit_SetRoomLight
 *
 */
void baseedit_SetRoomLight(BaseRoom *pRoom, int which, Vec3 color)
{
    Color byte_color;
    int i,c;

    for(i=0;i<3;i++)
    {
        c = 63 * color[i];
        if(c > 255)
            c = 255;
        if(c < 0)
            c = 0;
        byte_color.rgba[i] = c;
    }
    baseClientSendLight(pRoom, which, byte_color);
}

/**********************************************************************func*
 * baseedit_CreateDetail
 *
 */
void baseedit_CreateDetail(BaseRoom *pRoom, const Detail *pDetail, Mat4 matWorld, int invID)
{
    RoomDetail detail = {0};

    detail.info = pDetail;
    detail.invID = invID;
    copyMat4(matWorld, detail.mat);
    subVec3(detail.mat[3], pRoom->pos, detail.mat[3]);
    
    baseClientSendDetailCreate(pRoom, &detail);
}

/**********************************************************************func*
 * baseedit_DeleteDetail
 *
 */
static RoomDetail * pPendingDetailDelete;
static BaseRoom * pPendingDetailDeleteRoom;

void baseDetailDelete(void *data)
{
     if( baseedit_Mode() != kBaseEdit_Architect )
        return;

    if(!pPendingDetailDelete||!pPendingDetailDeleteRoom)
        return;

    if(g_refCurDetail.p == pPendingDetailDelete)
        memset(&g_refCurDetail, 0, sizeof(g_refCurDetail));

    if( !pPendingDetailDelete->id ) // tried to delete while placing item
    {
        baseedit_CancelDrags();
        baseedit_CancelSelects();
        return;
    }

    baseClientSendDetailDelete(pPendingDetailDeleteRoom, pPendingDetailDelete);
}

void baseedit_DeleteDetail(BaseRoom *pRoom, RoomDetail *pDetail)
{
    if( stricmp("Entrance", pDetail->info->pchCategory)==0 )
        return;

    if( dialogInQueue( NULL, baseDetailDelete, NULL ) )
        return;

    if( eaSize(&pDetail->storedEnhancement) || 
        eaSize(&pDetail->storedInspiration) ||
        eaSize(&pDetail->storedSalvage) )
    {
        dialogStd( DIALOG_OK, textStd("CantDeleteStoredItemsName", pDetail->info->pchDisplayName), NULL, NULL, NULL, NULL, 0 );
        return;
    }

    pPendingDetailDelete = pDetail;
    pPendingDetailDeleteRoom = pRoom;



    if( pDetail->info->bObsolete )
        dialogStd( DIALOG_ACCEPT_CANCEL, textStd("ObsoleteItemDeleteWarning"), NULL, NULL, baseDetailDelete, NULL, 0 );
    else if( pDetail->creator_name[0] )
        dialogStd( DIALOG_ACCEPT_CANCEL, textStd("PersonalItemDeleteWarning"), NULL, NULL, baseDetailDelete, NULL, 0 );
    else if( pDetail->info->bWarnDelete)
        dialogStd( DIALOG_ACCEPT_CANCEL, textStd("GenericItemDeleteWarning"), NULL, NULL, baseDetailDelete, NULL, 0 );
    else
        baseDetailDelete(0);
}

/**********************************************************************func*
* baseedit_DeleteDetail
*
*/
void baseedit_DeleteDoorway( BaseRoom * pRoom, int block[2])
{
    if( block[0] < 0 || block[0] >= pRoom->size[0] ||
        block[1] < 0 || block[1] >= pRoom->size[1] )
    {
        baseClientSendDoorwayDelete(pRoom, g_refCurBlock.block);
        if( block[0] == g_refCurBlock.block[0] &&
            block[1] == g_refCurBlock.block[1] )
        {
            memset(&g_refCurBlock, 0, sizeof(BaseBlockRef));
        }
    }
}

/**********************************************************************func*
 * baseedit_MoveDetail
 *
 */
void baseedit_MoveDetail(BaseRoom *pRoom, RoomDetail *pDetail, Mat4 matWorld)
{
    RoomDetail detail = *pDetail;

    copyMat4(matWorld, detail.mat);
    subVec3(detail.mat[3], pRoom->pos, detail.mat[3]);

    baseClientSendDetailUpdate(pRoom, &detail, NULL);
}

void baseedit_MoveDetailToRoom(BaseRoom *pRoom, RoomDetail *pDetail, BaseRoom *pNewRoom, Mat4 matWorld)
{
    RoomDetail detail = *pDetail;

    copyMat4(matWorld, detail.mat);
    subVec3(detail.mat[3], pNewRoom->pos, detail.mat[3]);

    baseClientSendDetailUpdate(pRoom, &detail,pNewRoom);
}

/**********************************************************************func*
* baseedit_RotateDetail
*
*/
void baseedit_RotateDetail(BaseRoom *pRoom, RoomDetail *pDetail, int clockwise)
{
    if(baseEdit_state.detail_dragging && g_refCurDetail.id == pDetail->id)
    {
        baseedit_RotateDraggedDetail(clockwise);
    }
    else if(pDetail->id)
    {
        RoomDetail detail = *pDetail;
        if( clockwise )
            yawMat3(PI/2, detail.mat);
        else
            yawMat3(-PI/2, detail.mat);

        if( !detailCanFit(g_refCurRoom.p, &detail, 0) )
        {
            FindReasonableSpot(g_refCurRoom.p, &detail, detail.mat[3], detail.mat[3], NULL);
        }
        baseClientSendDetailUpdate(pRoom, &detail,0);
    }
}

/**********************************************************************func*
 * SetPermissions
 *
 */
void baseedit_SetDetailPermissions(BaseRoom *pRoom, RoomDetail *pDetail)
{
    RoomDetail detail = *pDetail;

    if (pDetail && pDetail->info && pDetail->info->bIsStorage)
    {
        baseClientSendPermissionUpdate(pRoom, &detail);
    }
}

/**********************************************************************func*
 * BlockHeights
 *
 */
void baseedit_SetBlockHeights(BaseRoom *pRoom, int block[2], int height[2])
{
    float float_ht[2];
    Entity * e = playerPtr();
    float_ht[0] = height[0];
    float_ht[1] = height[1];

    if( baseedit_IsDoorway(pRoom, block, NULL ) )
    {
        baseedit_CreateDoorway(pRoom, block, float_ht);
    }
    else
    {
        RoomDetail *isect_details[20];

        if (!roomHeightIntersecting(pRoom,block,float_ht,isect_details,ARRAY_SIZE(isect_details)))
        {
            if(height[1]<height[0] )
                baseClientSendHeight(pRoom, kBaseSend_Single, block[0] + block[1]*pRoom->size[0], 0.0f, 0.0f);
            else
                baseClientSendHeight(pRoom, kBaseSend_Single, block[0] + block[1]*pRoom->size[0], height[0], height[1]);
        }
    }
}

void baseedit_SetRoomHeights(BaseRoom *pRoom,int height[2])
{
    if(height[1]>height[0] )
        baseClientSendHeight(pRoom, kBaseSend_Room, 0, height[0], height[1]);
}

void baseedit_SetBaseHeights(int height[2])
{
    if(height[1]>height[0] )
        baseClientSendHeight(NULL, kBaseSend_Base, 0, height[0], height[1]);
}

/**********************************************************************func*
 * baseedit_InDragMode
 *
 */
bool baseedit_InDragMode(void)
{
    if(baseEdit_state.dragging
        || baseEdit_state.detail_dragging
        || baseEdit_state.surface_dragging
        || baseEdit_state.room_dragging
        || baseEdit_state.door_dragging
        || baseEdit_state.plot_dragging)
    {
        return true;
    }

    return false;
}

/**********************************************************************func*
 * baseedit_CancelDrags
 *
 */
bool baseedit_CancelDrags(void)
{
    bool bRet = baseedit_InDragMode();

    baseEdit_state.dragging = false;

     baseEdit_state.detail_dragging = false;
    baseEdit_state.surface_dragging = false;
    baseEdit_state.room_dragging = false;
    baseEdit_state.door_dragging = false;
    baseEdit_state.plot_dragging = false;
    baseEdit_state.room_moving = false;

    return bRet;
}

/**********************************************************************func*
 * baseedit_CancelSelects
 *
 */
bool baseedit_CancelSelects(void)
{
    bool bRet;
    F32 elapsed = timerElapsed(g_iDecorFadeTimer);

    if(g_refCurDetail.p || elapsed < DECOR_FADE_TOTAL);
        bRet = true;

    timerAdd(g_iDecorFadeTimer, DECOR_FADE_TOTAL);
    memset(&g_refCurDetail, 0, sizeof(g_refCurDetail));

    return bRet;
}

/**********************************************************************func*
 * baseedit_DeleteRoom
 *
 */
void baseedit_DeleteRoom(BaseRoom *pRoom)
{
    int block[2];
    BaseRoom *pPosRoom = baseRoomLocFromPos(&g_base, ENTPOS(playerPtr()), block);

    if(!pRoom)
        return;

    if(pRoom == g_refCurRoom.p)
    {
        if(eaSize(&g_base.rooms)>1)
            baseedit_SetCurRoom(g_base.rooms[0], NULL);
    }

    baseClientSendRoomDelete(pRoom->id);
}

/**********************************************************************func*
 * baseedit_CreateRoom
 *
 */
void baseedit_CreateRoom(const RoomTemplate *pinfo, int size[2], Vec3 pos, F32 afHeights[2], RoomDoor * doors, int door_count )
{
    baseClientSendRoomCreate(size, pos, afHeights, pinfo, doors, door_count);
}

/**********************************************************************func*
* baseedit_MoveRoom
*
*/
void baseedit_MoveRoom(BaseRoom *room, int rotation, Vec3 pos, RoomDoor * doors, int door_count )
{
    baseClientSendRoomMove(room, rotation, pos, doors, door_count);
}

/**********************************************************************func*
 * baseClientSendDoorway
 *
 */
void baseedit_CreateDoorway(BaseRoom *pRoom, int block[2], F32 afHeights[2])
{
    baseClientSendDoorway(pRoom, block, afHeights);
}


static int printHelper(    MessageStore* store,
                        char* outputBuffer,
                        int bufferLength,
                        const char* messageID,
                        Array* messageTypeDef, ...) 
{
    int l;
    va_list va;
    va_start(va, messageTypeDef);
    l = msvaPrintfInternal(store, outputBuffer, bufferLength, messageID, messageTypeDef, NULL, va);
    va_end(va);
    return l;
}

// Parse and replace templates in a detail description string
char *detailGetDescription(const Detail *detail,int longDesc) {
    static char result[2048]; //Warning! watch for overflow
    const char *source;
    static Array *Message = NULL;
    int messageLength;
    int percent = 0;
    const char *param = "";
    if (!detail || !detail->pchDisplayShortHelp || !detail->pchDisplayHelp) {
        return NULL;
    }
    if (!Message)
        Message = msCompileMessageType("{Control, %d}, {Energy, %d}, {Lifetime, %d}, {RepairChance, %d}, {FunctionParam, %s}, {FunctionPercent, %d");
    if (longDesc)
        source = detail->pchDisplayHelp;
    else
        source = detail->pchDisplayShortHelp;

    if (detail->ppchFunctionParams && detail->ppchFunctionParams[0])
    {
        param = detail->ppchFunctionParams[0];
        percent = (int)(atof(detail->ppchFunctionParams[0])*100);
    }

    messageLength = printHelper(menuMessages,
                                result,
                                ARRAY_SIZE(result),
                                source,
                                Message,
                                detail->iControlProduce+detail->iControlConsume,
                                detail->iEnergyProduce+detail->iEnergyConsume,
                                detail->iLifetime, (int)(detail->fRepairChance*100),
                                param,
                                percent);

    if(!messageLength)
        return NULL;
    return result;
}

/* End of File */
