/***************************************************************************
 *     Copyright (c) 2005-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include <stdlib.h>
#include "dbcomm/dbcontainer.h"
#include "bases/BaseEntry.h"
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/components/earray.h>
#include "bases/basedata.h"
#include "bases/basesend.h"
#include "bases/baseserver.h"
#include "bases/bases.h"
#include "bases/basedata.h"
#include "bases/baseparse.h"
#include "entity/entity.h"
#include "entity/Supergroup.h"
#include "comm_backend.h"
#include "gameComm/svr_chat.h"
#include "language/langServerUtil.h"
#include <utilitieslib/utils/file.h>
#include "dbcomm/logcomm.h"
#include "entity/entPlayer.h"
#include "svr/svr_base.h"
#include "comm_game.h"
#include "dbcomm/staticMapInfo.h"
#include "bases/basesystems.h"
#include "bases/baselegal.h"
#include "gameComm/sendToClient.h"
#include "entity/entserver.h"
#include "gameSys/sgraid.h"
#include "entity/character_base.h"
#include "entity/character_workshop.h"
#include "storyarc/contactInteraction.h"
#include "storyarc/storyarcprivate.h"
#include "storyarc/contactdef.h"
#include "script/scriptengine.h"
#include "svr/svr_base.h"
#include "gameSys/raidmapserver.h"
#include "door.h"
#include "entity/SgrpServer.h"
#include "gameData/raidstruct.h"
#include "entity/entGameActions.h"

static int g_PrestigeSpent;

static bool sgCanAffordPrice( Entity *e, int price )
{
    if( !e )
        return 0;

    if( e->access_level >= BASE_FREE_ACCESS )
        return 1;

    if(g_base.supergroup_id)
        return SAFE_MEMBER(e->supergroup,prestige) - g_PrestigeSpent >= price;
    else
        return SAFE_MEMBER(e->pchar,iInfluencePoints) - g_PrestigeSpent >= price;
}

static void sgSpendPrestige(Entity *e, int amount)
{
       if( e->access_level < BASE_FREE_ACCESS )
    {    
        g_PrestigeSpent += amount;
        printf( "SPEND: last saved %i, spent = %i, new curr = %i\n", e->supergroup->prestige, amount, e->supergroup->prestige-g_PrestigeSpent );

        START_PACKET(pak, e, SERVER_UPDATE_PRESTIGE);
        pktSendBits(pak, 32, g_PrestigeSpent);
        END_PACKET;
    }
}

static void sgRefundPrestige(Entity *e, int amount)
{
    if( e->access_level < BASE_FREE_ACCESS )
    {
         g_PrestigeSpent -= amount;
        START_PACKET(pak, e, SERVER_UPDATE_PRESTIGE);
        pktSendBits(pak, 32, g_PrestigeSpent);
        END_PACKET;
    }
}

void applySGPrestigeSpent(Supergroup *sg)
{
    int i;

    LOG( LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, "Base:Save  (SG Id: %i) Saving Base, %i old prestige + %i prestige diff = %i new prestige total.  %i in base.", g_base.supergroup_id, sg->prestige, g_PrestigeSpent, sg->prestige-g_PrestigeSpent, sg->prestigeBase);
    sg->prestige -= g_PrestigeSpent;
    g_PrestigeSpent = 0;

    for(i=1;i<entities_max;i++) //Check if someone else is using this detail
    {
        if (entity_state[i] & ENTITY_IN_USE )
        {
            Entity *e = entities[i];

            if( ENTTYPE(e) == ENTTYPE_PLAYER && e->pl->architectMode)
            { //tell all the editors that the base got saved
                START_PACKET(pak, e, SERVER_UPDATE_PRESTIGE);
                pktSendBits(pak, 32, 0);
                END_PACKET;
            }
        }
    }
}

int baseGetAndClearSpent(void)
{
    int i, spent = g_PrestigeSpent;
    g_PrestigeSpent = 0;

    for(i=1;i<entities_max;i++) //Check if someone else is using this detail
    {
        if (entity_state[i] & ENTITY_IN_USE )
        {
            Entity *e = entities[i];

            if( ENTTYPE(e) == ENTTYPE_PLAYER && e->pl->architectMode)
            { //tell all the editors that the base got saved
                START_PACKET(pak, e, SERVER_UPDATE_PRESTIGE);
                pktSendBits(pak, 32, 0);
                END_PACKET;
            }
        }
    }

    return spent;
}

void applyEntInfluenceSpent(Entity *e, int influenceSpent)
{
    int inf_start, inf_end;

    inf_start = e->pchar->iInfluencePoints;
    ent_AdjInfluence(e, -influenceSpent, "base");
    inf_end = e->pchar->iInfluencePoints;

    LOG_ENT( e, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, "Base:Save Saving Base, %i old inf + %i inf diff = %i new inf total.  %i in base.",
                            inf_start, influenceSpent, inf_end, e->pchar->iInfluenceApt);

}


/**********************************************************************func*
 * baseEditFailed
 *
 */
void baseEditFailed( Entity *e, char * str )
{
    chatSendToPlayer( e->db_id, localizedPrintf(e,"BaseEditFailed", str), INFO_USER_ERROR, 0 );
}

/**********************************************************************func*
* baseBuyPlot
*
*/

int baseBuyPlot( Entity * e, const BasePlot * old_plot, const BasePlot * new_plot, int rot )
{
    float w, h;

    if( !e || !old_plot ) // if they don't have a plot let them have one on the house
        return true;

    if( rot )
    {
        w = new_plot->iLength;
        h = new_plot->iWidth;
    }
    else
    {
        w = new_plot->iWidth;
        h = new_plot->iLength;
    }

    if( !basePlotAllowed(&g_base, new_plot) || !basePlotFit(&g_base, w, h, NULL, 0) )
    {
        LOG_ENT( e, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, "Base:Plot:Buy (SG Id: %i) Plot (%s) buy failed, ", e->supergroup_id, new_plot->pchName );
        baseEditFailed( e, "PlotRestrictions" );
        return false;
    }

    if( !sgCanAffordPrice(e,new_plot->iCostPrestige-old_plot->iCostPrestige) )
    {
        LOG_ENT( e, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, "Base:Plot:Buy (SG Id: %i) Plot (%s) buy failed, not enough prestige (needed %d)", e->supergroup_id, new_plot->pchName, (new_plot->iCostPrestige-old_plot->iCostPrestige));
        baseEditFailed( e, "NotEnoughPrestige" );
        return false;
    }

    if(e)
    {
        sgSpendPrestige(e,new_plot->iCostPrestige);
        sgRefundPrestige(e,old_plot->iCostPrestige);
        LOG_ENT( e, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, "Base:Plot:Sell (SG Id: %i) Plot (%s) sold for %d prestige and %d influence.", e->supergroup_id,  old_plot->pchName, old_plot->iCostPrestige, old_plot->iCostInfluence );
        LOG_ENT( e, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, "Base:Plot:Buy (SG Id: %i) Plot (%s) bought for %d prestige and %d influence.", e->supergroup_id,  new_plot->pchName, new_plot->iCostPrestige, new_plot->iCostInfluence );
        chatSendToSupergroup( e, localizedPrintf(e,"BasePlotBought", e->name, new_plot->pchDisplayName, new_plot->iCostPrestige, new_plot->iCostInfluence, old_plot->pchDisplayName, old_plot->iCostPrestige, old_plot->iCostInfluence), INFO_SVR_COM );
        return true;
    }

    return true;
}

/**********************************************************************func*
 * baseBuyRoomDetail
 *
 */
int baseBuyRoomDetail( Entity *e, RoomDetail * detail, BaseRoom * room  )
{
    if( !e ) // base being created, no ent yet
        return true;

    if( !detail->info ) // something horrible happened
        return false;

    if( !e->access_level )
    {
        if( detail->info->bObsolete )
        {
            LOG_ENT( e, LOG_SUPERGROUP, LOG_LEVEL_IMPORTANT, 0, "Base:Detail:Buy (SG Id: %i, Room Id: %i) Detail (%s) buy failed obsolete item", e->supergroup_id, room->id, detail->info->pchName);
            baseEditFailed( e, "ObsoleteItem" );
            return false;
        }
        if( !baseDetailMeetsRequires( detail->info, e ) && (detail->info->ppchRequires || detail->creator_name[0]))
        { // Let them make it if it's a blank requires (recipe) and they're adding a personal item
            LOG_ENT( e, LOG_SUPERGROUP, LOG_LEVEL_IMPORTANT, 0, "Base:Detail:Buy (SG Id: %i, Room Id: %i) Detail (%s) buy failed did not meet requiremnts", e->supergroup_id, room->id, detail->info->pchName);
            baseEditFailed( e, "DidntMeetReq" );
            return false;
        }
    }
    if( !e || !sgCanAffordPrice(e,detail->info->iCostPrestige) )
    {
        LOG_ENT( e, LOG_SUPERGROUP, LOG_LEVEL_IMPORTANT, 0, "Base:Detail:Buy (SG Id: %i, Room Id: %i) Detail (%s) buy failed, not enough prestige (needed %d)", e->supergroup_id, room->id, detail->info->pchName, detail->info->iCostPrestige);
        baseEditFailed( e, "NotEnoughPrestige" );
        return false;
    }

    if(g_base.supergroup_id)
    {
        sgSpendPrestige(e,detail->info->iCostPrestige);
        LOG_ENT( e, LOG_SUPERGROUP, LOG_LEVEL_IMPORTANT, 0, "Base:Detail:Buy (SG Id: %i, Room Id: %i) Detail (%s) bought for %d prestige and %d influence.", e->supergroup_id, room->id, detail->info->pchName, detail->info->iCostPrestige, detail->info->iCostInfluence );
        chatSendToSupergroup( e, localizedPrintf(e,"BaseDetailBought", e->name, detail->info->pchDisplayName, detail->info->iCostPrestige, detail->info->iCostInfluence), INFO_SVR_COM );
        return true;
    }
    else
    {
        ent_AdjInfluence(e, detail->info->iCostPrestige, "base");
        LOG_ENT( e, LOG_SUPERGROUP, LOG_LEVEL_IMPORTANT, 0, "Base:Detail:Buy (Room Id: %i) Detail (%s) bought for %d influence.", room->id, detail->info->pchName, detail->info->iCostPrestige );
        chatSendToPlayer(e->db_id, localizedPrintf(e,"BaseDetailBoughtInf", e->name, detail->info->pchDisplayName, detail->info->iCostPrestige, detail->info->iCostInfluence), INFO_SVR_COM, e->db_id);
        return true;
    }
}

/**********************************************************************func*
 * baseSellRoomDetail
 *
 */
int baseSellRoomDetail( Entity * e, RoomDetail * detail, BaseRoom * room  )
{
    sgRefundPrestige(e,detail->info->iCostPrestige);
    LOG_ENT( e, LOG_SUPERGROUP, LOG_LEVEL_IMPORTANT, 0, "Base:Detail:Sell (SG Id: %i, Room Id: %i) Detail (%s) deleted, refunded %i prestige and %i influence.", e->supergroup_id, room->id, detail->info->pchName, detail->info->iCostPrestige, detail->info->iCostInfluence );
    chatSendToSupergroup( e, localizedPrintf(e,"BaseDetailSold", e->name, detail->info->pchDisplayName, detail->info->iCostPrestige, detail->info->iCostInfluence), INFO_SVR_COM );
    return true;
}

/**********************************************************************func*
 * baseBuyRoom
 *
 */
int baseBuyRoom( Entity *e, int room_id, const RoomTemplate * room )
{
    if( !sgCanAffordPrice(e,room->iCostPrestige) )
    {
        LOG_ENT( e, LOG_SUPERGROUP, LOG_LEVEL_IMPORTANT, 0, "Base:Room:Buy (SG Id: %i, Room Id: %i) Room (%s) buy failed, not enough prestige. (needed %d)", e->supergroup_id, room_id, room->pchName, room->iCostPrestige);
        baseEditFailed( e, "NotEnoughInf" );
        return false;
    }

    sgSpendPrestige(e,room->iCostPrestige);
    LOG_ENT( e, LOG_SUPERGROUP, LOG_LEVEL_IMPORTANT, 0, "Base:Room:Buy (SG Id: %i, Room Id: %i) Room (%s) bought for %i prestige and %i influence.", e->supergroup_id, room_id, room->pchName, room->iCostPrestige, room->iCostInfluence );
    chatSendToSupergroup( e, localizedPrintf(e,"BaseDetailBought", e->name, room->pchDisplayName, room->iCostPrestige, room->iCostInfluence), INFO_SVR_COM );
    return true;
}

/**********************************************************************func*
 * baseSellRoom
 *
 */
int baseSellRoom( Entity * e, BaseRoom *room )
{
    sgRefundPrestige(e,room->info->iCostPrestige);
    LOG_ENT( e, LOG_SUPERGROUP, LOG_LEVEL_IMPORTANT, 0, "Base:Room:Sell (SG Id: %i, Room Id: %i) Room (%s) deleted, refunded %i prestige and %i influence.", e->supergroup_id, room->id, room->info->pchName, room->info->iCostPrestige, room->info->iCostInfluence);
    chatSendToSupergroup( e, localizedPrintf(e,"BaseDetailSold", e->name, room->info->pchDisplayName, room->info->iCostPrestige, room->info->iCostInfluence), INFO_SVR_COM );
    return true;
}

#define FtFromGrid(gr) ((F32)(gr) * g_base.roomBlockSize)

// get a mat that faces into the room from the selected door
void getDoorwayMat(Mat4 mat, BaseRoom* room, int dooridx)
{
    Vec3 pos, dir, pyr;
    F32 yaw, dontcare;
    RoomDoor* door;

    if (dooridx >= eaSize(&room->doors))
        return;
    door = room->doors[dooridx];

    // location depends on wall door is on
    copyVec3(room->pos, pos);
    pos[1] = door->height[0];
    if (door->pos[0] < 0) // west
    {
        pos[0] -= FtFromGrid(0.25);
        pos[2] += FtFromGrid((F32)door->pos[1] + 0.5);
    }
    else if (door->pos[1] < 0) // south
    {
        pos[2] -= FtFromGrid(0.25);
        pos[0] += FtFromGrid((F32)door->pos[0] + 0.5);
    }
    else if (door->pos[0] >= room->size[0]) // east
    {
        pos[0] += FtFromGrid(room->size[0] + 0.25);
        pos[2] += FtFromGrid((F32)door->pos[1] + 0.5);
    }
    else // north
    {
        pos[2] += FtFromGrid(room->size[1] + 0.25);
        pos[0] += FtFromGrid((F32)door->pos[0] + 0.5);
    }

    // face center of room
    copyVec3(pos, mat[3]);
    pos[0] = room->pos[0] + FtFromGrid((F32)room->size[0] / 2);
    pos[2] = room->pos[2] + FtFromGrid((F32)room->size[1] / 2);
    subVec3(pos, mat[3], dir);
    getVec3YP(dir, &yaw, &dontcare);

    getMat3YPR(mat, pyr);
    pyr[1] = yaw;
    createMat3YPR(mat, pyr);
}

int getNumberOfDoorways( void )
{
    int i, n, count = 0;
    n = eaSize(&g_base.rooms);
    for (i = 0; i < n; i++)
        count += eaSize(&g_base.rooms[i]->doors);
    return count;
}

void sendPlayerToDoorway(Entity *e, int doorwayidx, int debug_print)
{
    Vec3 entrancePos, door_pos;
    Mat4 mat = {0};
    int i, n;

    n = eaSize(&g_base.rooms);
    for (i = 0; i < n; i++)
    {
        int numdoors = eaSize(&g_base.rooms[i]->doors);
        if (doorwayidx >= numdoors)
        {
            doorwayidx -= numdoors;
            continue;
        }

        if (debug_print)
            conPrintf(e->client, "Sending to room %i, door %i", i, doorwayidx);
        getDoorwayMat(mat, g_base.rooms[i], doorwayidx);
        entUpdateMat4Instantaneous(e, mat);
        return;
    }

    // otherwise, we have no doorways, or doorwayidx is wrong
    if (debug_print)
        conPrintf(e->client, "Sending to initial room");
    getSafeEntrancePos( NULL, entrancePos, 0, door_pos );
    entUpdatePosInstantaneous(e, entrancePos);
}

int getSafeRoomDoorPos( Entity *e, BaseRoom * room, Vec3 pos, int isDoor, Vec3 door_pos )
{
    float fGrid = g_base.roomBlockSize;
    int i;

    if(!room)
        return 0;

    if( e )
    {
        Vec3 min, max;
        copyVec3( room->pos, min );
        setVec3( max, room->pos[0] + fGrid*room->size[0], 48, room->pos[2] + fGrid*room->size[1] );
        if( pointBoxCollision( ENTPOS(e), min, max ) )
        {
            copyVec3( ENTPOS(e), pos );
            return 1;
        }
    }

    for( i = 0; i < eaSize(&room->doors); i++ )
    {
        if( isDoor )
        {
            if( door_pos[0] == room->pos[0] + fGrid*room->doors[i]->pos[0] &&
                door_pos[2] == room->pos[2] + fGrid*room->doors[i]->pos[1] )
                continue;
        }

        if( room->doors[i]->pos[0] < 0 )
            setVec3( pos, room->pos[0] - fGrid/4, room->doors[i]->height[0], room->pos[2] +fGrid*( room->doors[i]->pos[1]+.5f) );
        else if( room->doors[i]->pos[0] >= room->size[0] ) // door in X axis
            setVec3( pos, room->pos[0] + (room->size[0] + .25f)*fGrid, room->doors[i]->height[0], room->pos[2] + fGrid*( room->doors[i]->pos[1]+.5f) );
        else if( room->doors[i]->pos[1] < 0 )
            setVec3( pos, room->pos[0] + (room->doors[i]->pos[0] + .5f)*fGrid, room->doors[0]->height[0], room->pos[2] - fGrid/4 );
        else if( room->doors[i]->pos[1] >= room->size[1] )
            setVec3( pos, room->pos[0] + (room->doors[i]->pos[0] + .5f)*fGrid, room->doors[0]->height[0], room->pos[2] + fGrid*(room->size[1]+.25f) );

        return 1;
    }

    return 0;
}

int getSafeEntrancePos( Entity *e, Vec3 pos, int isDoor, Vec3 door_pos )
{
    BaseRoom * room = g_base.rooms?g_base.rooms[0]:NULL;
    if(!room)
        return 0;

    if( !getSafeRoomDoorPos( e, room, pos, isDoor, door_pos ) )
    {
        int i;

        for( i = 0; i < eaSize(&room->blocks); i++ )
        {
            if( room->blocks[i]->height[1] <= room->blocks[i]->height[0] )
                continue;

            copyVec3( room->pos, pos );
            pos[0] += (i % room->size[0])*g_base.roomBlockSize + g_base.roomBlockSize/2;
            pos[1] += room->blocks[i]->height[0];
            pos[2] += (i / room->size[0])*g_base.roomBlockSize + g_base.roomBlockSize/2;
        }
    }
    return 1;
}

void teleportEntityInVolumeToEntrance( Vec3 min, Vec3 max, int not_architect, int isDoor, Vec3 door_pos )
{
    Vec3 entrancePos;
    getSafeEntrancePos( NULL, entrancePos, isDoor, door_pos );
    teleportEntityInVolumeToPos( min, max, entrancePos, not_architect );
}

void teleportEntityInVolumeToClosestDoor( BaseRoom *room, Vec3 min, Vec3 max, int not_architect, int isDoor, Vec3 door_pos )
{
    Vec3 safePos;

    if( !getSafeRoomDoorPos( NULL, room, safePos, isDoor, door_pos ) )
        getSafeEntrancePos(NULL, safePos, isDoor, door_pos );

    teleportEntityInVolumeToPos( min, max, safePos, not_architect );
}

void teleportEntityToNearestDoor( Entity *e )
{
    Vec3 min, max, safe_pos;
    BaseRoom * room;
    int i;

    for( i = eaSize(&g_base.rooms)-1; i>=0; i-- )
    {
        room = g_base.rooms[i];
        copyVec3( room->pos, min );
        setVec3( max, room->pos[0] + g_base.roomBlockSize*room->size[0], 48, room->pos[2] + g_base.roomBlockSize*room->size[1] );
        if( pointBoxCollision( ENTPOS(e), min, max ) )
        {
            if( getSafeRoomDoorPos( NULL, room, safe_pos, 0, 0 ) )
            {
                entUpdatePosInstantaneous(e, safe_pos);
                return;
            }
        }
    }
    
    getSafeEntrancePos(NULL, safe_pos, 0, 0 );
    entUpdatePosInstantaneous(e, safe_pos);
}

void teleportEntityInVolumeToHt( Vec3 min, Vec3 max, float ht )
{
    int i;

    for(i=1;i<entities_max;i++)
    {
        if (entity_state[i] & ENTITY_IN_USE )
        {
            Entity *e = entities[i];

            if( ENTTYPE(e) == ENTTYPE_PLAYER && pointBoxCollision( ENTPOS_BY_ID(i), min, max ) )
            {
                Vec3 dest;
                setVec3( dest, ENTPOS_BY_ID(i)[0], ht, ENTPOS_BY_ID(i)[2] );
                entUpdatePosInstantaneous( e, dest );
            }
        }
    }
}

void teleportEntityInVolumeToPos( Vec3 min, Vec3 max, Vec3 dest, int not_architect )
{
    int i;

    for(i=1;i<entities_max;i++)
    {
        if (entity_state[i] & ENTITY_IN_USE )
        {
            Entity *e = entities[i];

            if( ENTTYPE(e) == ENTTYPE_PLAYER
                && (!not_architect || !e->pl->architectMode)
                && pointBoxCollision( ENTPOS_BY_ID(i), min, max ) )
            {
                entUpdatePosInstantaneous( e, dest );
            }
        }
    }
}

Entity *baseDetailInteracting(RoomDetail *det)
{
    int i;
    for(i=1;i<entities_max;i++) //Check if someone else is using this detail
    {
        if (entity_state[i] & ENTITY_IN_USE )
        {
            Entity *e = entities[i];

            if( ENTTYPE(e) == ENTTYPE_PLAYER && e->pl->detailInteracting == det->id)
            {
                return e;
            }
        }
    }
    return NULL;
}


void sendActivateUpdate(Entity *player, bool activate, int detailID, int interactType)
{
    if( player )
    {
        START_PACKET( pak, player, SERVER_UPDATE_BASE_INTERACT );

        pktSendBitsPack( pak, 1, activate );
        pktSendBitsPack( pak, 5, detailID );
        pktSendBitsPack( pak, 3, interactType );

        END_PACKET
    }
}

void sendBaseMode(Entity *player, int mode)
{
    if( player )
    {
        START_PACKET( pak, player, SERVER_UPDATE_BASE_MODE );

        pktSendBitsPack( pak, 1, mode );

        END_PACKET
    }
}

void sendRaidState(Entity *player, int mode)
{
    if( player )
    {
        START_PACKET( pak, player, SERVER_UPDATE_RAID_STATE );

        pktSendBitsPack( pak, 1, mode );

        END_PACKET
    }
}

void baseDeactivateDetail(Entity *player, int type) {
    if (type == kDetailFunction_Contact
        || type == kDetailFunction_AuxContact)
    {
        ContactInteractionEnd(player);
    }
    player->pl->detailInteracting = 0;
    sendActivateUpdate(player,0,0,type);
}

int baseCheckContactActivateAll(RoomDetail *det)
{
    int i, count = 0;
    
    if (!det || !det->info)
        return false;

    count = eaSize(&det->info->ppchFunctionParams);
    for (i = 1; i < count; i++)        // 0 is the contact name, option params start at 1
    {
        if (!stricmp(det->info->ppchFunctionParams[i], "ActivateAll"))
            return true;
    }
    return false;
}

void baseActivateDetail(Entity *player, int detailID, char *group)
{
    RoomDetail *det, *olddet;
    Entity *other;
    if (!g_base.rooms) {
        // This is bad, log or error
        return;
    }

    det = baseGetDetail(&g_base,detailID,NULL);
    if(!det) // Item was deleted at same time person clicked on it
        return;

    other = baseDetailInteracting(det);
    if (other && other != player)
    { //someone else interacting with this detail
        chatSendToPlayer( player->db_id, localizedPrintf(player,"PlayerAlreadyInteractingWithObjective",other->name), INFO_USER_ERROR, 0 );
        return;
    }

    // Check if we have the ability to use this detail
    if (RaidIsRunning() && (!player->supergroup || player->supergroup_id != g_base.supergroup_id))
    { // Are we an enemy on a raid?    
        if (det->info->eFunction == kDetailFunction_ItemOfPower )
        {
            RaidItemInteract(player, det);
            return;
        }    
        else
        { // only let attackers use items of power
            chatSendToPlayer( player->db_id, localizedPrintf(player,"NoInteractPermission"), INFO_USER_ERROR, 0 );
            return;
        }
    }
    else if (
                (    (det->info->eFunction == kDetailFunction_Contact && !baseCheckContactActivateAll(det)) ||
                    (det->info->eFunction != kDetailFunction_Store && det->info->eFunction != kDetailFunction_Contact)) &&
                (!player->supergroup || player->supergroup_id != g_base.supergroup_id)
             )
    {
        chatSendToPlayer( player->db_id, localizedPrintf(player,"NoInteractPermission"), INFO_USER_ERROR, 0 );
        return;
    }
    else if (roomDetailLookUnpowered(det)) 
    { //we're powered off, dont let them interact
        chatSendToPlayer( player->db_id, localizedPrintf(player,"DetailPoweredOff"), INFO_USER_ERROR, 0 );
        return;
    }

    olddet = baseGetDetail(&g_base,player->pl->detailInteracting,NULL);
    if (player->pl->detailInteracting &&
        det->info->eFunction != olddet->info->eFunction)
    { //tell the player to close the old window
        baseDeactivateDetail(player,olddet->info->eFunction);
    }


    // Set this as the currently interacting detail
    player->pl->detailInteracting = det->id;
    if ((det->info->eFunction == kDetailFunction_Contact || det->info->eFunction == kDetailFunction_AuxContact)
        && eaSize(&det->info->ppchFunctionParams) > 0)
    {
        ContactHandle contact;
        Vec3 loc;
        BaseRoom *room;
        room = baseGetRoom(&g_base,det->parentRoom);
        addVec3(room->pos,det->mat[3],loc);
        contact = ContactGetHandleLoose(det->info->ppchFunctionParams[0]);
        if (!contact)
        {        
            chatSendToPlayer(player->db_id,localizedPrintf(player,"CouldntFindContact", det->info->ppchFunctionParams[0]),INFO_SVR_COM,0);
            return;
        }
        ContactOpen(player,loc,contact,det->e);
    }
    else if (det->info->eFunction == kDetailFunction_Store && eaSize(&det->info->ppchFunctionParams) > 0)
    {
        OpenStoreExternal(det->e,player,det->info->ppchFunctionParams[0]);
    }
    sendActivateUpdate(player,1,player->pl->detailInteracting,det->info->eFunction);
}


void base_Tick(Entity *e) 
{
    RoomDetail *olddet = NULL;
    int failure = false;
    int type = 0;
    BaseRoom *room = NULL;

    // checking workshops that might exist outside of a base
    if (e->pl->workshopEntity) 
    {
        Entity *pEnt = erGetEnt(e->pl->workshopEntity);

        if (!pEnt)
        {
            character_WorkshopInterrupt(e);
        } else {
            if (distance3Squared(ENTPOS(pEnt), ENTPOS(e))>SQR(20) && stricmp(e->pl->workshopInteracting, "Worktable_Incarnate"))
                character_WorkshopInterrupt(e);
        }
    }

    if (!g_base.rooms) {
        return;
    }
    if (e->pl->detailInteracting)
    {
        olddet = baseGetDetail(&g_base,e->pl->detailInteracting,NULL);
        if (!olddet)
        {
            type = 0;
            failure = true;
        }
        else 
        {
            room = baseGetRoom(&g_base,olddet->parentRoom);
        }
        if (!room)
        {
            type = 0;
            failure = true;
        }
        if (RaidIsRunning() && olddet && (olddet->info->eFunction ==kDetailFunction_Contact || olddet->info->eFunction ==kDetailFunction_AuxContact ))
        { //Players are allowed to use all types but contacts
            type = olddet->info->eFunction;
            failure = true;
        }        
        if (!failure)
        {
            Vec3 loc;

            type = olddet->info->eFunction;
            //Check to see if the detail is in range
            addVec3(room->pos,olddet->mat[3],loc);
            if (distance3Squared(loc, ENTPOS(e))>SQR(20))
                failure = true;
        }

        if (!failure) 
        {
            // check to see if they not interacting with a contact
            if (e && e->storyInfo && e->storyInfo->interactTarget == 0 && 
                olddet->info && olddet->info->eFunction ==kDetailFunction_Contact )
            {
                baseDeactivateDetail(e, kDetailFunction_Contact);
            }
        }

        if (failure)
        {
            baseDeactivateDetail(e,type);
        }

        if( e->pl->architectMode != kBaseEdit_None )
            failure = true;
    } 

    failure = false;
    if(e->pl->architectMode)
    {
        if (e->pl->architectMode == kBaseEdit_Architect && !playerCanModifyBase(e,&g_base))
        {
            failure = true;
        }
        if (e->pl->architectMode == kBaseEdit_Plot && !playerCanModifyBase(e,&g_base))
        {
            failure = true;
        }
        if (e->pl->architectMode == kBaseEdit_AddPersonal && !playerCanPlacePersonal(e,&g_base))
        {
            failure = true;
        }
    }
    if (failure)
    { // player can't edit base any more, kick them out
        e->pl->architectMode = 0;
        sendBaseMode(e,0);
    }
    
    // This seems like a good place to send the background music. It runs every tick so it can
	// change in real time if the base builder uses /sg_music inside the base, but the ticks
	// are a couple of seconds apart so it won't hog resources.
	if (e->pl->musicTrack != g_base.musicTrack) {
		e->pl->musicTrack = g_base.musicTrack;
		if (strlen(g_base.music) == 0)
			serveFadeSound(e, SOUND_MUSIC, 5.0f);
		else
			servePlaySound(e, g_base.music, SOUND_MUSIC, 1.0f);
	}

    if(e->access_level < BASE_FREE_ACCESS) // only check kick if no access level
    {
        if (RaidIsRunning())
        {
            extern U32 running_baseraid;
            bool amDefender, amAttacker;

            ScheduledBaseRaid* currRaid = cstoreGet(g_ScheduledBaseRaidStore, running_baseraid );

            amDefender = BaseRaidIsParticipant(currRaid, e->db_id, 0);
            amAttacker = BaseRaidIsParticipant(currRaid, e->db_id, 1);

            //MW 3.16.06 if attacker was already in the base at start, kick him
            //(RaidInitPlayer(e) might work, but kicking him is safer (and more satisfying))
            if( amAttacker && (e->pchar->iAllyID != kAllyID_Evil) )
                amAttacker = false;

            if ( !amDefender && !amAttacker )
            { //Kick out uninvolved players
                LOG_ENT( e, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, "Base:Tick:KickPlayer player not attacking or defending");
                leaveMission(e,0,0);
                return;
            }
        }
        else if( g_base.supergroup_id && e->supergroup_id != g_base.supergroup_id ) // check visitors
        {
            char *kickReason = NULL;
            Supergroup *sg = sgrpFromSgId(g_base.supergroup_id);
            BaseAccess access = kBaseAccess_None;
            int i;
            int idSgVisitor = e->supergroup_id;

            if(!sg)
            {
                access = kBaseAccess_PermissionDenied; // boot if can't get supergroup
                kickReason = "couldn't load sg";
            }

			if( sg && e->pl->passcode == sg->passcode)
				access = kBaseAccess_Allowed;

			if( sg && idSgVisitor != 0 && access == kBaseAccess_None )
			{
				for (i = 0; i < MAX_SUPERGROUP_ALLIES; i++)
				{
					if (sg->allies[i].db_id == idSgVisitor)
					{
						access = sgrp_BaseAccessFromSgrp( sg, kSgrpBaseEntryPermission_Coalition );
						kickReason = "coalition access not allowed";
						break;
					}
				}
			}
            
            if( sg && idSgVisitor != 0 && access == kBaseAccess_None )
            {
                for (i = 0; i < MAX_SUPERGROUP_ALLIES; i++)
                {
                    if (sg->allies[i].db_id == idSgVisitor)
                    {
                        access = sgrp_BaseAccessFromSgrp( sg, kSgrpBaseEntryPermission_Coalition );
                        kickReason = "coalition access not allowed";
                        break;
                    }
                }
            }

            if( sg && e->teamup_id && e->teamup && access == kBaseAccess_None )
            {
                int idLeader = e->teamup->members.leader;
                if( supergroup_IsMember(sg, idLeader) )
                {
                    access = sgrp_BaseAccessFromSgrp( sg, kSgrpBaseEntryPermission_LeaderTeammates );
                    kickReason = "team member access not allowed";
                }
                else
                {
                    access = kBaseAccess_PermissionDenied;
                    kickReason = "not a member of team";
                }
            }

            // check if allowed
            if( access != kBaseAccess_Allowed )
            {
                LOG_ENT( e, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, "Base:Tick:KickPlayer %s(%d) %i, %i base id", kickReason, access,
                      e->supergroup_id, g_base.supergroup_id);
                 leaveMission(e,0,0);
            }
        }
    }
}


const char ** baseGetMapDestinations(int detailID)
{
    RoomDetail *det = baseGetDetail(&g_base,detailID,NULL);
    RoomDetail **attached;
    BaseRoom *room;
    const char **out;
    int outIndex = 0;

    if (!det)
        return NULL;

    eaCreateConst(&out);
    room = baseGetRoom(&g_base,det->parentRoom);
    attached = findAttachedAuxiliary(room,det);
    if (attached)
    {
        int i;
        for (i = eaSize(&attached)-1; i >= 0; i--)
        {
            int j, size = eaSize(&attached[i]->info->ppchFunctionParams);
            for (j = 0; j < size; j++)
            {
                eaPushConst(&out,attached[i]->info->ppchFunctionParams[j]);
            }
        }
        eaDestroyConst(&attached);
    }
    return out;
}

void completeBaseGurneyXferWork(Entity *e, int detailID) {
    float heal = 0.0f;
    int i;
    RoomDetail *det = baseGetDetail(&g_base,detailID,NULL);
    BaseRoom *room;
    RoomDetail **auxes;
    if (!det || det->info->eFunction != kDetailFunction_Medivac || !e || !e->pchar) 
    { //if we're not a medivac, don't heal you. shouldnt even have gotten here
        return;
    }
    room = baseGetRoom(&g_base,det->parentRoom);
    auxes = findAttachedAuxiliary(room,det);
    if (eaSize(&det->info->ppchFunctionParams) > 0 && det->info->ppchFunctionParams[0][0])
    { //if we have a float paramater, use it
        heal = atof(det->info->ppchFunctionParams[0]);
    }
    for (i = 0; i < eaSize(&auxes); i++) {
        RoomDetail *aux = auxes[i];
        if (aux->info->eFunction != kDetailFunction_AuxMedivacImprover)
            continue; //not an appropriate aux item
        if (roomDetailLookUnpowered(aux))
            continue; //we're powered off
        if (eaSize(&aux->info->ppchFunctionParams) <= 0 || !aux->info->ppchFunctionParams[0][0])
            continue; //no useable paramater
        heal += atof(aux->info->ppchFunctionParams[0]);
    }

    if (heal > 1.0f)
        heal = 1.0f;
    if (verify(heal >= 0.0f))
    { //negative heal makes no sense    
        e->pchar->attrCur.fHitPoints = e->pchar->attrMax.fHitPoints * heal;
        e->pchar->attrCur.fEndurance = e->pchar->attrMax.fEndurance * heal;
    }
}

void setBlockHeight( Entity *e, BaseRoom *room, int block[2], float height[2], int idx )
{
    RoomDetail *isect_details[20];
    if (!roomHeightIntersecting(room,block,height,isect_details,ARRAY_SIZE(isect_details)) && roomBlockLegal(room,block,height))
    {
        Vec3 min, max;

        if (baseIsLegalHeight(height[0],height[1]))
        {
            LOG_ENT( e, LOG_ENTITY, LOG_LEVEL_VERBOSE, 0, "Base:Surface (SG id: %i, Room Id: %i) Block (%i,%i) height changed from (%f,%f) to (%f,%f)",
                e->supergroup_id, room->id, block[0], block[1], room->blocks[idx]->height[0], room->blocks[idx]->height[1], height[0], height[1] );
            copyVec2(height,room->blocks[idx]->height);

            // move players up if caught in floor change
            setVec3( min, room->pos[0] + block[0]*g_base.roomBlockSize, 0, room->pos[2] + block[1]*g_base.roomBlockSize);
            setVec3( max, room->pos[0] + (block[0]+1)*g_base.roomBlockSize, 48, room->pos[2] + (block[1]+1)*g_base.roomBlockSize);
            teleportEntityInVolumeToHt(min, max, height[0]);
        }
        else
        {
            LOG_ENT( e, LOG_ENTITY, LOG_LEVEL_VERBOSE, 0, "Base:Surface (SG id: %i, Room Id: %i) Block (%i,%i) changed from (%f,%f) to wall",
                e->supergroup_id, room->id, block[0], block[1], room->blocks[idx]->height[0], room->blocks[idx]->height[1] );
            baseSetIllegalHeight(room,idx);

            // flor changed to wall port players caught in change to entrance
            setVec3( min, room->pos[0] + block[0]*g_base.roomBlockSize, 0, room->pos[2] + block[1]*g_base.roomBlockSize);
            setVec3( max, room->pos[0] + (block[0]+1)*g_base.roomBlockSize, 48, room->pos[2] + (block[1]+1)*g_base.roomBlockSize);
            teleportEntityInVolumeToClosestDoor(room, min, max, 0, 0, NULL);
        }
    }
}

void baseSetBlockHeights( Entity * e, BaseRoom *room, float height[2], int idx )
{
    int block[2];
    Vec3 pos;

    if (!roomPosFromIdx(room,idx,pos))
    {
        return;
    }
    block[0] = idx % room->size[0];
    block[1] = idx / room->size[0];

    setBlockHeight( e, room, block, height, idx );
}

void baseSetRoomHeights( Entity * e, BaseRoom *room, float height[2] )
{
    int i;

    for( i = eaSize(&room->blocks)-1; i >= 0; i-- )
        baseSetBlockHeights(e,room,height,i);

    checkWalledOffAreasForPlayers( room );
}

void baseSetBaseHeights( Entity * e, float height[2] )
{
    int i,j;
    for( i = eaSize(&g_base.rooms)-1; i >= 0; i-- )
    {
        baseSetRoomHeights(e, g_base.rooms[i], height);
        g_base.rooms[i]->mod_time++;

        // also do all the doorways
        for( j = eaSize(&g_base.rooms[i]->doors)-1; j>=0; j-- )
            setDoorway(&g_base,g_base.rooms[i],g_base.rooms[i]->doors[j]->pos,height, 0);
    }
}

static markAdjacentSquares( BaseRoom * room, int *grid, int x, int y )
{
    int i, cx[4], cy[4];

      cx[0] = x+1;
    cx[1] = x-1;
    cx[2] = cx[3] = x;
    
    cy[2] = y+1;
    cy[3] = y-1;
    cy[0] = cy[1] = y;

     for( i = 0; i < 4; i++ )
    {
        BaseBlock * block;

        if( cx[i] < 0 || cy[i] < 0 ||
            cx[i] >= room->size[0] || cy[i] >= room->size[1] )
            continue;

        if( grid[cx[i]+ room->size[0]*cy[i]] )
            continue;

        block = room->blocks[cx[i]+room->size[0]*cy[i]];

        if( block->height[1] > block->height[0] )
        {
            grid[cx[i]+room->size[0]*cy[i]] = true;
            markAdjacentSquares( room, grid, cx[i], cy[i] );
        }
    }
}

void checkWalledOffAreasForPlayers( BaseRoom *room )
{
    int i;
     int *accessibleGrid;
    
    if( !eaSize(&room->doors) )
        return;
    
    accessibleGrid = calloc(1, sizeof(int)*room->size[0]*room->size[1]);

      for( i = eaSize(&room->doors)-1; i>=0; i-- )
    {
        markAdjacentSquares( room, accessibleGrid, room->doors[i]->pos[0], room->doors[i]->pos[1] );
    }

    for(i = room->size[0]*room->size[1]-1; i >= 0; i--)
    {
        if( !accessibleGrid[i] )
        {
            Vec3 min, max;
            int x,y;

            x = i % room->size[0];
            y = i / room->size[0];
            setVec3( min, room->pos[0] + x*g_base.roomBlockSize, 0, room->pos[2] + y*g_base.roomBlockSize);
            setVec3( max, room->pos[0] + (x+1)*g_base.roomBlockSize, 48, room->pos[2] + (y+1)*g_base.roomBlockSize);
            teleportEntityInVolumeToClosestDoor(room, min, max, 0, 0, NULL);
        }
    }

    SAFE_FREE(accessibleGrid);
}


void baseApplyRoomSwapsToBase( BaseRoom * master_room )
{
    int i,j;

    for( i = eaSize(&g_base.rooms)-1; i >= 0; i-- )
    {
        BaseRoom *pRoom = g_base.rooms[i];

         if( master_room == pRoom )
            continue;

        for(j=0;j<ROOMDECOR_MAXTYPES;j++)
        {
            DecorSwap *master_swap = decorSwapFindOrAdd(master_room,j);
            DecorSwap *swap = decorSwapFindOrAdd(pRoom,j);

            strcpy( swap->geo, master_swap->geo );
            strcpy( swap->tex, master_swap->tex );
        }

        pRoom->mod_time++;
    }
}

void baseApplyRoomTintsToBase( BaseRoom * master_room )
{
    int i,j;

    for( i = eaSize(&g_base.rooms)-1; i >= 0; i-- )
    {
        BaseRoom *pRoom = g_base.rooms[i];

        if( master_room == pRoom )
            continue;

        for(j=0;j<ROOMDECOR_MAXTYPES;j++)
        {
            DecorSwap *master_swap = decorSwapFindOrAdd(master_room,j);
            DecorSwap *swap = decorSwapFindOrAdd(pRoom,j);
            memcpy( swap->tints, master_swap->tints, sizeof(master_swap->tints));
        }
        pRoom->mod_time++;
    }
}

void baseApplyRoomLightToBase( BaseRoom * master_room )
{
    int i,j;

    for( i = eaSize(&g_base.rooms)-1; i >= 0; i-- )
    {
        BaseRoom *pRoom = g_base.rooms[i];

        if( master_room == pRoom )
            continue;

        for(j=2; j>=0; j--)
            pRoom->lights[j]->integer =  master_room->lights[j]->integer;

        pRoom->mod_time++;
    }
}

/* End of File */

