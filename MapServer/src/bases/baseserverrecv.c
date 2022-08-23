#include <stdlib.h>
#include "bases/basestorage.h"
#include "entity/salvage.h"
#include "entity/powers.h"
#include <utilitieslib/utils/mathutil.h>
#include "bases/bases.h"
#include "bases/baseserverrecv.h"
#include <utilitieslib/network/netio.h>
#include "entity/entity.h"
#include <utilitieslib/network/netcomp.h>
#include "bases/basetogroup.h"
#include <utilitieslib/components/earray.h>
#include "bases/basedata.h"
#include "bases/baseparse.h"
#include <utilitieslib/components/EString.h>
#include <utilitieslib/utils/textparser.h>
#include "bases/baseparse.h"
#include <utilitieslib/network/bindiff.h>
#include "bases/basesend.h"
#include "group/group.h"
#include "group/groupscene.h"
#include "bases/baseserver.h"
#include "bases/baselegal.h"
#include "dbcomm/logcomm.h"
#include "entity/entPlayer.h"
#include "dbcomm/dbdoor.h"
#include "dbcomm/dbcomm.h"
#include "gameSys/raidmapserver.h"
#include "entity/SgrpServer.h"
#include "entity/character_inventory.h"
#include "gridcoll/gridcache.h"
#include "container/baseloadsave.h"
#include "entity/Supergroup.h"
#include "container/team.h"
#include "gameComm/sendToClient.h"
#include <utilitieslib/utils/utils.h>
#include "cmdparse/cmdserver.h"
#include "language/langServerUtil.h"

extern int world_modified;

static updateCurrEditId(int id)
{
    if (id > g_base.curr_id)
        g_base.curr_id = id;
}

static int deleteDoor( BaseRoom * room, int block[2] )
{
    int            i,j, retVal = false;
    BaseRoom    *r;

    if (!roomDoorwayDeleteLegal(room->id,block))
        return retVal;

    for(i=0;i<eaSize(&g_base.rooms);i++)
    {
        r = g_base.rooms[i];
        if( room->id == r->id )
        {
            for(j=eaSize(&r->doors)-1;j>=0;j--)
            {
                if( block[0] == r->doors[j]->pos[0] &&
                    block[1] == r->doors[j]->pos[1] )
                {
                    Vec3 min, max;
                    RoomDoor * rd;
                    setVec3(min, r->pos[0]+r->doors[j]->pos[0]*g_base.roomBlockSize, 0, r->pos[2]+r->doors[j]->pos[1]*g_base.roomBlockSize);
                    setVec3(max, r->pos[0]+(r->doors[j]->pos[0]+1)*g_base.roomBlockSize, 48, (r->pos[2]+r->doors[j]->pos[1]+1)*g_base.roomBlockSize);
                    teleportEntityInVolumeToEntrance( min, max, 0, true, min );

                    rd = eaRemove(&r->doors,j);
                    ParserFreeStruct(rd);
                    r->mod_time++;
                    retVal = true;
                }
            }
        }
    }
    checkAllDoors(&g_base, false);
    return retVal;
}

void baseCheckFloatingObject(Base *base, Entity * e)
{
    int i,j, save = 0;

    for(i=0;i<eaSize(&base->rooms);i++)
    {
        BaseRoom    *room = base->rooms[i];
        for( j=0; j<eaSize(&room->details); j++ )
        {
            RoomDetail * detail = room->details[j];
            if(detail->info->bDoNotBlock && !detailCanFit(room,detail,0))
            {
                baseSellRoomDetail( e, detail, room );
                roomDetailRemove(room, detail);
            }
        }
    }
}

bool processDetail(Entity *e, BaseRoom *room, RoomDetail *edit, int new_room, int ignoreCost, bool *force_save)
{
    RoomDetail    *detail;
    int err = 0;
    
    if(!room)
    {
        if(e)
        {
            LOG_ENT( e, LOG_SUPERGROUP, LOG_LEVEL_IMPORTANT, 0, "Base:Detail (SG id: %i) Detail modification failed, couldn't find room", e->supergroup_id );
            baseEditFailed(e, "NoCanFit");
        }
        return false;
    }

    if( edit->info && !detailCanFit(room,edit,&err) )
    {
        if(e)
        {
            LOG_ENT( e, LOG_SUPERGROUP, LOG_LEVEL_IMPORTANT, 0, "Base:Detail (SG id: %i, Room Id: %i) Detail (%s) modification failed, did not fit (err #%i)", e->supergroup_id, room->id, edit->info->pchName, err );
            baseEditFailed(e, "NoCanFit");
        }
        return false;
    }

    if( e && edit->info)
    {
        if( !detailAllowedInRoom(room,edit,new_room) )
        {
            LOG_ENT( e, LOG_SUPERGROUP, LOG_LEVEL_IMPORTANT, 0, "Base:Detail (SG %i, Room Id: %i) Detail (%s) modification failed, failed room restrictions", e->supergroup_id, room->id, edit->info->pchName  );
            baseEditFailed(e, "NotAllowedInRoom");
            return false;
        }

        if( !detailAllowedInBase(&g_base,edit) )
        {
            LOG_ENT( e, LOG_SUPERGROUP, LOG_LEVEL_IMPORTANT, 0, "Base:Detail (SG %i, Room Id: %i) Detail (%s) modification failed, failed base plot restrictions", e->supergroup_id, room->id, edit->info->pchName  );
            baseEditFailed(e, "NotAllowedInBase");
            return false;
        }
    }

    detail = roomGetDetail(room,edit->id);
      if (detail && memcmp(edit->mat,detail->mat,sizeof(Mat4))!=0 && detailSupportFindContacts(room,detail,0,0))
    {
        if(e)
        {
            LOG_ENT( e, LOG_SUPERGROUP, LOG_LEVEL_IMPORTANT, 0, "Base:Detail (SG %i, Room Id: %i) Detail modification failed, stacking error.", e->supergroup_id, room->id );
            baseEditFailed(e, "CantChangeStack" );
        }
        return false;
    }

    if (edit->info->bIsStorage)
    {
        edit->permissions = detail != NULL ? detail->permissions : STORAGE_BIN_DEFAULT_PERMISSIONS;
    }
    if (!detail) // this is where we add new items to room
    {
        Vec3 min, max;

        if (!ignoreCost)
        {
            if( !baseBuyRoomDetail( e, edit, room ) )
                return false;
        }
        if (edit->invID && e && e->pchar)
        {
            if (character_AdjustInventory(e->pchar, kInventoryType_BaseDetail, edit->invID-1, -1, "used", false) == -1)
                return false; //failed to remove from inventory, don't place it
            strcpy(edit->creator_name, e->name);
            *force_save = true;
        }
        edit->creation_time = dbSecondsSince2000();
        
        detail = ParserAllocStruct(sizeof(*detail));
        eaPush(&room->details,detail);

        baseDetailExtents(edit->info, edit->mat, min, max);
        addVec3(room->pos, min, min);
        addVec3(room->pos, max, max);
        teleportEntityInVolumeToClosestDoor(room, min, max, 1, 0, NULL);
        detail->parentRoom = room->id;
        detail->permissions = edit->permissions;

        if(edit->info->bSpecial)
            *force_save = true;
    }
    else
    {
        // get rid of any lingering info since it's been changed somehow.
        detailCleanup(detail);
    }

    // otherwise modifying it.
    if( edit->info) {
        Mat4 door_pos;
        copyMat4( unitmat, door_pos );
         addVec3( room->pos, edit->mat[3], door_pos[3] );

        if (!detail->info)
            *detail = *edit;
        else
            copyMat4(edit->mat,detail->mat);

        detail->parentRoom = room->id;
        detail->permissions = edit->permissions;

        if (strstriConst(detail->info->pchGroupName,"baselogo")) //replace with proper logoability check
        { 
            char texName[128];
            Supergroup *sg = e->supergroup;

            if (sg->emblem[0] == '!')
            {
                sprintf(texName,"%s_tint",sg->emblem);
            }
            else
            {
                sprintf(texName,"emblem_%s_tint",sg->emblem);
            }
        
            detailSwapSet(detail,"emblem_biohazard_tint",texName);

            detail->tints[0] = colorFlip(COLOR_CAST(sg->colors[0]));
            detail->tints[1] = colorFlip(COLOR_CAST(sg->colors[1]));            
        }

        if (stricmp("Entrance", detail->info->pchCategory)==0 )
        {
            baseToDefs(&g_base, 0);
            sgroup_SaveBase(&g_base, NULL, *force_save);
        }

        updateAllBaseDoorTypes( edit, door_pos );
    }
    else
    {
        deleteAllBaseDoorTypes( detail,0);
        *detail = *edit;
        if (!detail->info)
        {
            if(detail->info->bSpecial)
                *force_save = true;

            if (!ignoreCost)
            {
                baseSellRoomDetail( e, detail, room );
            }
            roomDetailRemove(room, detail);
        }
    }

    updateCurrEditId(edit->id);

    return true;
}

bool processPermissions(Entity *e, BaseRoom *room, RoomDetail *edit, bool *force_save)
{
    RoomDetail    *detail;
    int err = 0;
    
    if(!room)
    {
        if(e)
        {
            LOG_ENT( e, LOG_SUPERGROUP, LOG_LEVEL_IMPORTANT, 0, "Base:Detail (SG id: %i) Detail modification failed, couldn't find room", e->supergroup_id );
            baseEditFailed(e, "NoCanFit");
        }
        return false;
    }

    detail = roomGetDetail(room,edit->id);

    if (edit->info->bIsStorage)
    {
        // Audit permissions - I hope we never hit the case when e is NULL.
        if (e == NULL)
        {
            edit->permissions = STORAGE_BIN_MINIMUM_PERMISSIONS;
        }
        else
        {
            int myRank = 0;
            int myAllowed;
            int i;

            for(i = eaSize(&e->supergroup->memberranks) - 1; i >= 0; i--)
            {
                if(e->supergroup->memberranks[i]->dbid == e->db_id)
                {
                    myRank = e->supergroup->memberranks[i]->rank;
                    break;
                }
            }
            if (myRank < 0)
            {
                myAllowed = 0;
            }
            else if (myRank >= NUM_SG_RANKS_WITH_VARIABLE_PERMISSIONS)
            {
                myAllowed = 0xffffffff;
            }
            else
            {
                // Form a bitmask of what I can change
                myAllowed = (1 << myRank) - 1;
                // Set the same for the second set of bits.
                myAllowed |= myAllowed << 16;
            }
            if (!detail)
            {
                edit->permissions = (edit->permissions & myAllowed) | STORAGE_BIN_MINIMUM_PERMISSIONS;
            }
            else
            {
                edit->permissions = (edit->permissions & myAllowed) | (detail->permissions & ~myAllowed) | STORAGE_BIN_MINIMUM_PERMISSIONS;
            }
        }
    }

    if( detail && edit->info && edit->info->bIsStorage && detail->permissions != edit->permissions )
    {
        detail->permissions = edit->permissions;
        *force_save = true;
    }

    updateCurrEditId(edit->id);

    return true;
}

static BaseRoom  *getRoomHeader(Packet *pak,int *idx)
{
    BaseRoom    *room;
    int            room_id;

    room_id = pktGetBitsPack(pak,1);
    room = baseGetRoom(&g_base,room_id);
    *idx = pktGetBitsPack(pak,1);
    if (room)
        room->mod_time++;
    return room;
}

static int roomDelete(Entity *e, int room_id, bool *needs_save)
{
    BaseRoom *room;
    Vec3 max;
    int i;
    room = baseGetRoom(&g_base,room_id);

    if (!room || !room->info || 
        room->info == basedata_GetRoomTemplateByName( "Entrance" ) || !roomDeleteLegal(room_id))
        return false;

    for( i = eaSize(&room->details)-1; i >=0; i-- )
    {
        RoomDetail *detail = room->details[i];
        if (detail->info->bCannotDelete && server_state.beta_base != 2)
            return false;
        if (detail->info->eFunction == kDetailFunction_ItemOfPowerMount && RaidSGIsScheduledAttacking(e->supergroup_id)
            && sgrp_emptyMounts(e->supergroup) <= 1)
        {
            LOG_ENT( e, LOG_SUPERGROUP, LOG_LEVEL_IMPORTANT, 0, "Base:Detail (SG id: %i) Detail Remove failed, can't remove base when raiding.", e->supergroup_id );
            baseEditFailed(e, "CantChangeRaidable" );
            return false;
        }
        if( eaSize(&detail->storedEnhancement) || 
            eaSize(&detail->storedInspiration) ||
            eaSize(&detail->storedSalvage) )
        {
            LOG_ENT( e, LOG_SUPERGROUP, LOG_LEVEL_IMPORTANT, 0, "Base:Detail (SG id: %i) Detail Remove failed, items in storage container.", e->supergroup_id );
            baseEditFailed(e, "CantDeleteStoredItems" );
            return false;
        }
    }

    if( !baseSellRoom(e,room))
        return false;

    // refund everything inside room
    for( i = eaSize(&room->details)-1; i >=0; i-- )
    {
        if(room->details[i]->info->bSpecial)
            *needs_save = true;

        if( baseSellRoomDetail( e, room->details[i], room ) )
            roomDetailRemove(room, room->details[i]);
        else
            return false;
    }

    max[0] = room->pos[0] + room->size[0]*g_base.roomBlockSize;
    max[1] = 48;
    max[2] = room->pos[2] + room->size[1]*g_base.roomBlockSize;
    teleportEntityInVolumeToEntrance( room->pos, max, 0, 0, NULL );

    eaFindAndRemove(&g_base.rooms,room);
    destroyRoom( room );
    checkAllDoors(&g_base,true);
    updateCurrEditId(room_id);
    return true;
}

static void setupInfoPointers(void)
{
    int        i;

    for(i=0; i<eaSize(&g_base.rooms); i++)
    {
        g_base.rooms[i]->info = basedata_GetRoomTemplateByName(g_base.rooms[i]->name);

        if(!g_base.rooms[i]->info)
            g_base.rooms[i]->info = basedata_GetRoomTemplateByName("Studio_3x3");
    }
}

int roomUpdate(Entity *e, int room_id,int size[2],Vec3 pos,F32 height[2], const RoomTemplate *info,RoomDoor *doors,int door_count)
{
     if(!addOrChangeRooomIsLegal(&g_base, room_id, size, 0, pos, height, info, doors, door_count) || (e && !baseBuyRoom(e, room_id, info)) )
    {
        int i,j;
        BaseRoom *pRoom;

        for( i = eaSize(&g_base.rooms)-1; i>=0; i-- )
        {
            pRoom = g_base.rooms[i];
            for( j = eaSize(&pRoom->details)-1; j>=0; j-- )
                roomDetailRemove( pRoom, pRoom->details[j] );
        }

        rebuildBaseFromStr(&g_base);
        setupInfoPointers();

        for( i = eaSize(&g_base.rooms)-1; i>=0; i-- )
        {
            pRoom = g_base.rooms[i];
            for( j = eaSize(&pRoom->details)-1; j>=0; j-- )
            {
                if( pRoom->details[j]->info->pchEntityDef )
                    roomDetailRebuildEntity( pRoom, pRoom->details[j] );
            }
        }

        pRoom = baseGetRoom(&g_base,room_id);
        if( pRoom && pRoom->info )
            LOG_ENT( e, LOG_SUPERGROUP, LOG_LEVEL_IMPORTANT, 0, "Base:Room:Buy (SG Id: %i, Room Id: %i) Room (%s) buy failed, illegal change.", e->supergroup_id, room_id, pRoom->info->pchName );
        baseEditFailed( e, "IllegalChange" );

        return false;
    }

    return true;
}

static int roomMove( int room_id, int rotation, Vec3 pos ,RoomDoor *doors, int door_count  )
{
    int i, fail = 0, size[2];
    Vec3 min, max;
    BaseRoom * room;

      room = baseGetRoom(&g_base,room_id);
    if( !room )
        return false;

    copyVec3(room->pos, min);
    copyVec3(room->pos, max);
    max[0] += room->size[0]*g_base.roomBlockSize;
    min[1] = 0;
    max[1] = 40;
    max[2] += room->size[1]*g_base.roomBlockSize;

    if( rotation&1 )
    {
        size[0] = room->size[1];
        size[1] = room->size[0];
    }
    else
    {
        size[0] = room->size[0];
        size[1] = room->size[1];
    }
    
    // fun fun
       if(!addOrChangeRooomIsLegal(&g_base, room_id, size, rotation, pos, NULL, room->info, doors, door_count))
        fail = true;

    if( !fail ) // check all items in room
    {
        BaseRoom * room = baseGetRoom(&g_base, room_id);
        room->mod_time++;
        for( i = eaSize(&room->details)-1; i >= 0 ; i--)
        {
              if(!detailCanFit( room, room->details[i], 0))
                 fail = true;
        }

    }

    if( fail )
    {
        int j;
        BaseRoom * pRoom;

        for( i = eaSize(&g_base.rooms)-1; i>=0; i-- )
        {
            pRoom = g_base.rooms[i];
             for( j = eaSize(&pRoom->details)-1; j>=0; j-- )
                roomDetailRemove( pRoom, pRoom->details[j] );
        }

         rebuildBaseFromStr(&g_base);
        setupInfoPointers();

        for( i = eaSize(&g_base.rooms)-1; i>=0; i-- )
        {
            pRoom = g_base.rooms[i];
            for( j = eaSize(&pRoom->details)-1; j>=0; j-- )
            {
                if( pRoom->details[j]->info->pchEntityDef )
                    roomDetailRebuildEntity( pRoom, pRoom->details[j] );
            }
        }
        return false;
    }

    teleportEntityInVolumeToClosestDoor( room, min, max, 0, 0, NULL );

    for( i = eaSize(&room->details)-1; i >= 0 ; i--)
        deleteAllBaseDoorTypes( room->details[i], 1);

    for( i = eaSize(&room->details)-1; i >= 0 ; i--)
    {
        RoomDetail * detail = room->details[i];
        Mat4 door_pos;

        copyMat4( unitmat, door_pos ); //Construct a real matrix from room + detail
        addVec3( room->pos, detail->mat[3], door_pos[3] );

        updateAllBaseDoorTypes( detail, door_pos );
        detailCleanup(detail);
    }

    updateCurrEditId(room_id);

    return true;
}

void plotUpdate( Entity *e, const BasePlot *plot, Vec3 pos )
{

    if (plot == NULL)
        return;

    if( g_base.plot != plot )
    {
        if( baseBuyPlot(e, g_base.plot, plot, pos[1] ) )
        {
            g_base.plot = plot;
        } 
        else 
        {
            if (e)
                dbLog("cheater", e, "invalid base change (%s)", plot->pchName);
        }
    }

     if( pos[1] )
    {
        g_base.plot_width = g_base.plot->iLength;
        g_base.plot_height = g_base.plot->iWidth;
    }
    else
    {
        g_base.plot_width = g_base.plot->iWidth;
        g_base.plot_height = g_base.plot->iLength;
    }
    pos[1] = 0;

    copyVec3( pos, g_base.plot_pos );
    basePlotFit( &g_base, g_base.plot_width, g_base.plot_height, pos, 1 );

    if( e )
        LOG_ENT( e, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, "Base:Plo (SG id: %i) Plot Updated (type:%s) Pos(%f,%f)", e->supergroup_id, plot->pchName, pos[0], pos[2] );
}

void baseCreateDefault(Base *base)
{
    int size[2] = {3, 2};
    F32 height[2] = {4, 32};
    Vec3 vec = {0, 0, 0};

    RoomDetail entrance = {0};
    const RoomTemplate * room = basedata_GetRoomTemplateByName( "Entrance" );
    const BasePlot *plot = basedata_GetBasePlotByName( "TinyHiddenSpot_8x8" );
    const DetailCategory *detailCat = basedata_GetDetailCategoryByName( "Entrance" );
    Detail * detail = detailCat->ppDetails[0];

    if(!base)
        return;

    // this is hack to set default room size
    if (!base->roomBlockSize)
        base->roomBlockSize = BASE_BLOCK_SIZE;

    if( !base->supergroup_id )
        base->supergroup_id = db_state.map_supergroupid;

    base->plot_width = plot->iWidth;
    base->plot_height = plot->iLength;

    size[0] = room->iLength;
    size[1] = room->iWidth;

    if (!group_info.file_count) // file-less defs go to group_info.files[0]
        groupFileNew();

    plotUpdate( NULL, plot, vec );
    roomUpdate( NULL, 1, size, vec, height, room, 0, 0);

    if( detail )
    {
        bool save;
        copyMat4(unitmat,entrance.mat);
        setVec3( entrance.mat[3], room->iWidth*base->roomBlockSize/2, height[0], room->iLength*base->roomBlockSize/2 );
        entrance.info = detail;
        processDetail(NULL, base->rooms[0], &entrance, 0, 1,&save);
    }
}



int baseReceiveEdit(Packet *pak,Entity *e)
{
    BaseNetCmd    cmd;
    int noAccess = false; //set to true if we get an access error: stop trying to process things
    bool force_save = false;

    world_modified=1;

    cmd = pktGetBitsPack(pak,1);
    switch(cmd)
    {
    xcase BASENET_MODE:
    {
        int mode;
        mode = pktGetBitsPack(pak,1);

        if( e->pl->detailInteracting )
        {
            mode = kBaseEdit_None;
            noAccess = true;
            baseEditFailed(e, "InteractingBlock");
        }

        // Reject the mode change request if user doesnt have permission
        if( !e->access_level )
        {
            if (mode == kBaseEdit_Architect && !playerCanModifyBase(e,&g_base))
            {
                mode = kBaseEdit_None;
                noAccess = true;
                baseEditFailed(e, "NoAccess");
            }
            if (mode == kBaseEdit_Plot && !playerCanModifyBase(e,&g_base))
            {
                mode = kBaseEdit_None;
                noAccess = true;
                baseEditFailed(e, "NoAccess");
            }
            if (mode == kBaseEdit_AddPersonal && !playerCanPlacePersonal(e,&g_base))
            {
                mode = kBaseEdit_None;
                noAccess = true;
                baseEditFailed(e, "NoAccess");
            }
        }
        else
        {
            if(g_base.user_id)
            {
                // it's an apartment, let them through
            }
            else if(!g_base.supergroup_id && !e->supergroup_id)
            {
                mode = kBaseEdit_None;
                noAccess = true;
                baseEditFailed(e, "NoAccess");
            }
            else
            {
                // for admins to be able to edit bases, they have to be part of the bases supergroup
                // the alternative is INSANE
                if(!g_base.supergroup_id)
                    g_base.supergroup_id = e->supergroup_id;

                if( mode != kBaseEdit_None && e->supergroup_id != g_base.supergroup_id )
                {
                    int leader_rank = NUM_SG_RANKS-1;

                    // kick from own supergroup
                    if(e->supergroup_id)
                    {
                        conPrintf(e->client, "%s", localizedPrintf(e, "GMsgkickinfo", e->supergroup_id));
                        sgroup_MemberQuit(e);
                    }

                    // add themselves to new group
                    if( teamAddMember( e, CONTAINER_SUPERGROUPS, g_base.supergroup_id, 0) )
                    {
                        sgroup_initiate(e);
                        sgroup_refreshStats(e->db_id);
                        sgroup_setGeneric(e, SG_PERM_ANYONE, sgroup_genSetRank, &e->db_id, &leader_rank, "SetRank");
                        e->admin_changed_sg = 1;
                    }
                    else
                    {
                        conPrintf(e->client, "%s", localizedPrintf(e, "GMcouldntaddtoSG"));
                        mode = kBaseEdit_None;
                    }
                }

                if(e->supergroup_id && mode == kBaseEdit_None && e->admin_changed_sg)
                {
                    // for reasons unknown to me, we want to kick gms but not devs
                    if(e->access_level < 9)
                        sgroup_MemberQuit(e);
                    e->admin_changed_sg = 0;
                }
            }
        }

        if(e->pl->architectMode != mode)
        {
            if (mode == kBaseEdit_Architect || mode == kBaseEdit_Plot)
            {
                int i;
                for(i=1;i<entities_max;i++) //Check if someone else is architecting
                {
                    if (entity_state[i] & ENTITY_IN_USE )
                    {
                        Entity *check = entities[i];

                        if( ENTTYPE(check) == ENTTYPE_PLAYER && check != e &&
                            (check->pl->architectMode == kBaseEdit_Architect || check->pl->architectMode == kBaseEdit_Plot))
                        { //only one person can architect or edit plot at the same time
                            mode = kBaseEdit_None;
                            noAccess = true;
                            baseEditFailed(e, "TooManyArchitects");
                            break;
                        }
                    }
                }
            }
            if (!noAccess)
            {
                gridCacheInvalidate();
                e->pl->architectMode = mode;
                if( !mode )
                {
                    Vec3 dest;
                     getSafeEntrancePos( e, dest, 0, 0 );
                    entUpdatePosInstantaneous( e, dest );

                    if( db_state.local_server )
                        force_save = true;
                }
            }
        }

        sendBaseMode(e,mode);
    }
    xcase BASENET_DETAILS:
    {
        int            room_id,count,i,reset;
        RoomDetail    detail = {0};
        BaseRoom    *room;

        room_id = pktGetBitsPack(pak,1);
        room = baseGetRoom(&g_base,room_id);
        if (room)
            room->mod_time++;
        count = pktGetBitsPack(pak,1);
        reset = pktGetBits(pak,1);

        if (reset && room && playerCanModifyBase(e, &g_base) && e->pl->architectMode == kBaseEdit_Architect)
            eaDestroyEx(&room->details, detailCleanup);

        for(i=0;i<count;i++)
        {
            char    name[128];

            detail.id = pktGetBitsPack(pak,1);
            detail.invID = pktGetBitsPack(pak,1);
            detail.permissions = pktGetBitsPack(pak,1);
            getMat4(pak,detail.mat);
            strcpy(name,pktGetString(pak));

            detail.info = basedata_GetDetailByName(name);

            if(!detail.invID && !(playerCanModifyBase(e, &g_base) && e->pl->architectMode == kBaseEdit_Architect))
            {
                noAccess = true;
                baseEditFailed(e, "NoAccess");
                break;
            }
            if (detail.invID && (!playerCanPlacePersonal(e,&g_base) && e->pl->architectMode == kBaseEdit_AddPersonal))
            {
                noAccess = true;
                baseEditFailed(e, "NoAccess");
                break;
            }            
            processDetail(e,room,&detail,0,0,&force_save);
        }
    }
    xcase BASENET_DETAIL_DELETE:
    {
        int            idx;
        BaseRoom    *room = getRoomHeader(pak,&idx);
        RoomDetail    *detail = room?roomGetDetail(room,idx):NULL;

        if( !room || !detail )
        {
            LOG_ENT( e, LOG_SUPERGROUP, LOG_LEVEL_IMPORTANT, 0, "Base:Detail (SG id: %i) Detail Remove failed, couldn't find room", e->supergroup_id );
            baseEditFailed(e, "NoRoom" );
            break;
        }
        if( detail->info && detail->info->bCannotDelete && server_state.beta_base!= 2)
        {
            LOG_ENT( e, LOG_SUPERGROUP, LOG_LEVEL_IMPORTANT, 0, "Base:Detail (SG id: %i) Detail Remove failed, undeletable item.", e->supergroup_id );
            break;
        }
        if (detailSupportFindContacts(room,detail,0,0))
        {
            LOG_ENT( e, LOG_SUPERGROUP, LOG_LEVEL_IMPORTANT, 0, "Base:Detail (SG id: %i) Detail Remove failed, can't unstack.", e->supergroup_id );
            baseEditFailed(e, "CantUnStack" );
            break;
        }
        if (!(playerCanModifyBase(e,&g_base) && e->pl->architectMode == kBaseEdit_Architect))
        {
            noAccess = true;
            baseEditFailed(e, "NoAccess");
            break;
        }
        if (detail->info->eFunction == kDetailFunction_ItemOfPowerMount && RaidSGIsScheduledAttacking(e->supergroup_id)
            && sgrp_emptyMounts(e->supergroup) <= 1)
        {
            LOG_ENT( e, LOG_SUPERGROUP, LOG_LEVEL_IMPORTANT, 0, "Base:Detail (SG id: %i) Detail Remove failed, can't remove base when raiding.", e->supergroup_id );
            baseEditFailed(e, "CantChangeRaidable" );
            break;
        }

        if( !roomdetail_CanDeleteStorage(detail) )
        {
            LOG_ENT( e, LOG_SUPERGROUP, LOG_LEVEL_IMPORTANT, 0, "Base:Detail (SG id: %i) Detail Remove failed, items in storage container.", e->supergroup_id );
            baseEditFailed(e, "CantDeleteStoredItems" );
            break;
        }

        if( !baseSellRoomDetail( e, detail, room ) )
            break;

        deleteAllBaseDoorTypes(detail,0);

        if(detail->info->bSpecial)
            force_save = true;

        updateCurrEditId(detail->id);
        roomDetailRemove(room, detail);

    }
    xcase BASENET_DETAIL_MOVE:
    {
        int            room_id,newroom_id, room_switch;
        RoomDetail    detail = {0};
        RoomDetail  *olddetail;
        BaseRoom    *room, *newroom;

        room_id = pktGetBitsPack(pak,1);
        room = baseGetRoom(&g_base,room_id);
        if (room)
            room->mod_time++;
        newroom_id = pktGetBitsPack(pak,1);
        newroom = baseGetRoom(&g_base,newroom_id);
        if (newroom)
            newroom->mod_time++;

        detail.id = pktGetBitsPack(pak,1);
        getMat4(pak,detail.mat);

        if(!detail.invID && !(playerCanModifyBase(e, &g_base) && e->pl->architectMode == kBaseEdit_Architect))
        {
            noAccess = true;
            baseEditFailed(e, "NoAccess");
            break;
        }

        olddetail = baseGetDetail(&g_base,detail.id,NULL);
        if (!olddetail) //bad reference
        {
            LOG_ENT( e, LOG_SUPERGROUP, LOG_LEVEL_IMPORTANT, 0, "Base:Detail (SG id: %i) Detail Move failed, can't find item.", e->supergroup_id );
            break;
        }

        //// mimic process for deleting an item, since that is what is happening
        //if( !room || !olddetail )
        //{
        //    dbLog("Base:Detail", e, "(SG id: %i) Detail Remove failed, couldn't find room", e->supergroup_id );
        //    baseEditFailed(e, "NoRoom" );
        //    break;
        //}

        //if( olddetail->info && olddetail->info->bCannotDelete && server_state.beta_base!= 2)
        //{
        //    dbLog("Base:Detail", e, "(SG id: %i) Detail Remove failed, undeletable item.", e->supergroup_id );
        //    break;
        //}

        //if (detailSupportFindContacts(room,olddetail,0,0))
        //{
        //    dbLog("Base:Detail", e, "(SG id: %i) Detail Remove failed, can't unstack.", e->supergroup_id );
        //    baseEditFailed(e, "CantUnStack" );
        //    break;
        //}

        //if (!(playerCanModifyBase(e,&g_base) && e->pl->architectMode == kBaseEdit_Architect))
        //{
        //    noAccess = true;
        //    baseEditFailed(e, "NoAccess");
        //    break;
        //}

        //if (olddetail->info->eFunction == kDetailFunction_ItemOfPowerMount && RaidSGIsScheduledAttacking(e->supergroup_id)
        //    && sgrp_emptyMounts(e->supergroup) <= 1)
        //{
        //    dbLog("Base:Detail", e, "(SG id: %i) Detail Remove failed, can't remove base when raiding.", e->supergroup_id );
        //    baseEditFailed(e, "CantChangeRaidable" );
        //    break;
        //}

        //if( !roomdetail_CanDeleteStorage(olddetail) )
        //{
        //    dbLog("Base:Detail", e, "(SG id: %i) Detail Remove failed, items in storage container.", e->supergroup_id );
        //    baseEditFailed(e, "CantDeleteStoredItems" );
        //    break;
        //}

        //if( !baseSellRoomDetail( e, olddetail, room ) )
        //    break;

        //deleteAllBaseDoorTypes(olddetail,0);

        //if(olddetail->info->bSpecial)
        //    force_save = true;

        room_switch = (newroom && room && newroom != room);
        // Copy info from the old detail
        detail.info = olddetail->info;
        detail.creation_time = olddetail->creation_time;
        strncpyt(detail.creator_name,olddetail->creator_name,127);

        if(processDetail(e,newroom,&detail,room_switch,1,&force_save))
        {                        
            if (room_switch)
            { //Remove it from old room
                int i = 0;
                RoomDetail *newDetail = roomGetDetail(newroom,detail.id);

                if( newDetail )
                {
                    for( i = eaSize(&olddetail->storedSalvage) - 1; i >= 0; --i)
                    {
                        StoredSalvage *pi = olddetail->storedSalvage[i];
                        force_save = 1;
                        if(pi && pi->salvage)
                            roomdetail_AdjSalvage(newDetail, pi->salvage->pchName, pi->amount);
                    }

                    for( i = eaSize(&olddetail->storedInspiration) - 1; i >= 0; --i)
                    {                    
                        StoredInspiration *pi = olddetail->storedInspiration[i];
                        force_save = 1;
                        if (pi && pi->ppow)
                            roomdetail_AdjInspiration(newDetail, basepower_ToPath(pi->ppow), pi->amount);
                    }

                    for( i = eaSize(&olddetail->storedEnhancement) - 1; i >= 0; --i)
                    {
                        StoredEnhancement *pi = olddetail->storedEnhancement[i];
                        force_save = 1;
                        if(pi && eaSize(&pi->enhancement))
                        {
                            Boost *enh = pi->enhancement[0];
                            roomdetail_AdjEnhancement(newDetail, basepower_ToPath(enh->ppowBase), enh->iLevel, enh->iNumCombines, pi->amount);
                        }
                    }
                    // And copy the permissions
                    newDetail->permissions = olddetail->permissions;
                }

                roomDetailRemove(room, olddetail);
            }
        }
    }
    xcase BASENET_ROOMHEIGHT:
    {
        int idx;
        BaseRoom* room = NULL;
        F32 height[2];

        int type = pktGetBitsPack(pak,1);
        if(type!=kBaseSend_Base)
        {
            int room_id = pktGetBitsPack(pak,1);
            room = baseGetRoom(&g_base,room_id);
            if (room)
                room->mod_time++;
            if( type == kBaseSend_Single )
                idx = pktGetBitsPack(pak,1);
        }

        height[0] = pktGetF32Comp(pak);
        height[1] = pktGetF32Comp(pak);

        if (!(playerCanModifyBase(e,&g_base) && e->pl->architectMode == kBaseEdit_Architect))
        {
            noAccess = true;
            baseEditFailed(e, "NoAccess");
            break;
        }

        switch (type)
        {
        case kBaseSend_Single:
            baseSetBlockHeights( e, room, height, idx );
            checkWalledOffAreasForPlayers( room );
            break;
        case kBaseSend_Room:
            baseSetRoomHeights( e, room, height );
            break;
        case kBaseSend_Base:
            baseSetBaseHeights( e, height );
            break;
        }
    }
    xcase BASENET_TEXSWAP:
    {
        int            j,idx;
        BaseRoom    *room = getRoomHeader(pak,&idx);
        char        *texname;

        texname = pktGetString(pak);
        //BASE TODO: do we have any way to validate this texture?

        if (!(playerCanModifyBase(e,&g_base) && e->pl->architectMode == kBaseEdit_Architect))
        {
            noAccess = true;
            baseEditFailed(e, "NoAccess");
            break;
        }

          if (room)
        {
            if( idx < 0 )
            {
                for(j=0;j<ROOMDECOR_MAXTYPES;j++)
                {
                    DecorSwap *swap = decorSwapFindOrAdd(room,j);
                    LOG_ENT( e, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0,  "Base:Texture (SG id: %i, Room Id: %i) Texture changed (%s)", e->supergroup_id, room->id, texname );
                    strcpy(swap->tex,texname);
                }
            }
            else
            {
                DecorSwap    *swap = decorSwapFindOrAdd(room,idx);
                LOG_ENT( e, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, "Base:Texture (SG id: %i, Room Id: %i) Texture changed (%s)", e->supergroup_id, room->id, texname );
                strcpy(swap->tex,texname);
            }
        }
        else
            baseEditFailed(e, "NoRoom" );
    }
    xcase BASENET_TINT:
    {
        int            j,idx;
        BaseRoom    *room = getRoomHeader(pak,&idx);
        Color        colors[2];

        colors[0].integer = pktGetBits(pak,32);
        colors[1].integer = pktGetBits(pak,32);

        if (!(playerCanModifyBase(e,&g_base) && e->pl->architectMode == kBaseEdit_Architect))
        {
            noAccess = true;
            baseEditFailed(e, "NoAccess");
            break;
        }

        if (room)
        {
            if( idx < 0 )
            {
                for(j=0;j<ROOMDECOR_MAXTYPES;j++)
                {
                    DecorSwap *swap = decorSwapFindOrAdd(room,j);
                    LOG_ENT( e, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, "Base:Color (SG id: %i, Room Id: %i) Color changed to (%i,%i)", e->supergroup_id, room->id, colors[0].integer, colors[1].integer );
                    memcpy(swap->tints,colors,sizeof(colors));
                }
            }
            else
            {
                DecorSwap    *swap = decorSwapFindOrAdd(room,idx);
                LOG_ENT( e, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0,  "Base:Color (SG id: %i, Room Id: %i) Color changed to (%i,%i)", e->supergroup_id, room->id, colors[0].integer, colors[1].integer );
                memcpy(swap->tints,colors,sizeof(colors));
            }
        }
        else
            baseEditFailed(e, "NoRoom" );
    }
    xcase BASENET_BASETINT:
    {
        int            room_id = pktGetBitsPack(pak,1);
        BaseRoom    *room = baseGetRoom(&g_base,room_id);

        if (!(playerCanModifyBase(e,&g_base) && e->pl->architectMode == kBaseEdit_Architect))
        {
            noAccess = true;
            baseEditFailed(e, "NoAccess");
            break;
        }

        if( room )
        {
            LOG_ENT( e, LOG_ENTITY, LOG_LEVEL_VERBOSE, 0,  "Base:Geo:Tint (SG id: %i, Room Id: %i) Room Tint Applied to Base", e->supergroup_id, room->id );
            baseApplyRoomTintsToBase( room );
        }
        else
            baseEditFailed(e, "NoRoom" );
    }
    xcase BASENET_GEOSWAP:
    {
        int            j, idx;
        BaseRoom    *room = getRoomHeader(pak,&idx);
        char        *style;

        style = pktGetString(pak);
        //BASE TODO: do we have any way to validate this geometry?

        if (!(playerCanModifyBase(e,&g_base) && e->pl->architectMode == kBaseEdit_Architect))
        {
            noAccess = true;
            baseEditFailed(e, "NoAccess");
            break;
        }
        if (room)
        {
            if( idx < 0 )
            {
                for(j=0;j<ROOMDECOR_MAXTYPES;j++)
                {
                    DecorSwap *swap = decorSwapFindOrAdd(room,j);
                    LOG_ENT( e, LOG_ENTITY, LOG_LEVEL_VERBOSE, 0, "Base:Geo (SG id: %i, Room Id: %i) Geometry change (%s)", e->supergroup_id, room->id, style );
                    strcpy(swap->geo,style);
                }
            }
            else
            {
                DecorSwap    *swap = decorSwapFindOrAdd(room,idx);
                LOG_ENT( e, LOG_ENTITY, LOG_LEVEL_VERBOSE, 0,  "Base:Geo (SG id: %i, Room Id: %i) Geometry change (%s)", e->supergroup_id, room->id, style );
                strcpy(swap->geo,style);
            }
        }
        else
            baseEditFailed(e, "NoRoom" );
    }
    xcase BASENET_BASESTYLE:
    {
        int            room_id = pktGetBitsPack(pak,1);
        BaseRoom    *room = baseGetRoom(&g_base,room_id);

        if (!(playerCanModifyBase(e,&g_base) && e->pl->architectMode == kBaseEdit_Architect))
        {
            noAccess = true;
            baseEditFailed(e, "NoAccess");
            break;
        }

        if( room )
        {
            LOG_ENT( e, LOG_ENTITY, LOG_LEVEL_VERBOSE, 0,  "Base:Geo:Texture (SG id: %i, Room Id: %i) Room Style Applied to Base", e->supergroup_id, room->id );
            baseApplyRoomSwapsToBase( room );
        }
        else
            baseEditFailed(e, "NoRoom" );
    }
    xcase BASENET_LIGHT:
    {
        int            idx;
        BaseRoom    *room = getRoomHeader(pak,&idx);
        Color        light_color;

        light_color.integer = pktGetBits(pak,32);
        if (!(playerCanModifyBase(e,&g_base) && e->pl->architectMode == kBaseEdit_Architect))
        {
            noAccess = true;
            baseEditFailed(e, "NoAccess");
            break;
        }

        if (room)
        {
            LOG_ENT( e, LOG_ENTITY, LOG_LEVEL_VERBOSE, 0,  "Base:Light (SG id: %i, Room Id: %i) Lighting change (%i,%i)", e->supergroup_id, idx, room->lights[idx]->integer, light_color.integer );
            room->lights[idx]->integer = light_color.integer;
        }
        else
            baseEditFailed(e, "NoRoom" );
    }
    xcase BASENET_BASELIGHTING:
    {
        int            room_id = pktGetBitsPack(pak,1);
        BaseRoom    *room = baseGetRoom(&g_base,room_id);

        if (!(playerCanModifyBase(e,&g_base) && e->pl->architectMode == kBaseEdit_Architect))
        {
            noAccess = true;
            baseEditFailed(e, "NoAccess");
            break;
        }

        if( room )
        {
            LOG_ENT( e, LOG_ENTITY, LOG_LEVEL_VERBOSE, 0,  "Base:Light (SG id: %i, Room Id: %i) Room Lighting Applied to Base", e->supergroup_id, room->id );
            baseApplyRoomLightToBase( room );
        }
        else
            baseEditFailed(e, "NoRoom" );
    }
    xcase BASENET_DOORWAY:
    {
        F32            height[2];
        int            pos[2], idx;
        BaseRoom    *room = getRoomHeader(pak,&idx), *neighbor;

        pos[0] = pktGetBitsPack(pak,1);
        pos[1] = pktGetBitsPack(pak,1);
        height[0] = pktGetF32Comp(pak);
        height[1] = pktGetF32Comp(pak);
        if (!room || !(playerCanModifyBase(e,&g_base) && e->pl->architectMode == kBaseEdit_Architect))
        {
            noAccess = true;
            baseEditFailed(e, "NoAccess");
            break;
        }
        neighbor = setDoorway(&g_base,room,pos,height,true);
        LOG_ENT( e, LOG_ENTITY, LOG_LEVEL_VERBOSE, 0, "Base:Door (SG id: %i, Room Id: %i) Door added to Pos(%i,%i)", e->supergroup_id, room->id, pos[0], pos[1] );
        if (neighbor)
            neighbor->mod_time++;
        checkAllDoors(&g_base,true);
    }
    xcase BASENET_DOORWAY_DELETE:
    {
        int            idx,block[2];
        BaseRoom    *room = getRoomHeader(pak,&idx);

        block[0] = pktGetBitsPack(pak,1);
        block[1] = pktGetBitsPack(pak,1);

        if (!room || !(playerCanModifyBase(e,&g_base) && e->pl->architectMode == kBaseEdit_Architect))
        {
            noAccess = true;
            baseEditFailed(e, "NoAccess");
            break;
        }

        if( deleteDoor( room, block ) )
            LOG_ENT( e, LOG_ENTITY, LOG_LEVEL_VERBOSE, 0,  "Base:Door (SG id: %i, Room Id: %i) Door removed at Pos(%i,%i)", e->supergroup_id, room->id, block[0], block[1] );

    }
    xcase BASENET_ROOM:
    {
        const RoomTemplate *info;
        RoomDoor    doors[64];
        int            i,room_id,base_block_size,door_count;
        int            size[2];
        Vec3        pos;
        F32            height[2];

         room_id = pktGetBitsPack(pak,1);
        size[0] = pktGetBitsPack(pak,1);
        size[1] = pktGetBitsPack(pak,1);
        pktGetVec3(pak,pos);
        height[0] = pktGetBitsPack(pak,1);
        height[1] = pktGetBitsPack(pak,1);

        info = basedata_GetRoomTemplateByName(pktGetString(pak));
        base_block_size = pktGetBitsPack(pak,1);

        door_count = pktGetBitsPack(pak,1);
        if (door_count >= ARRAY_SIZE(doors))
            break;
        for(i=0;i<door_count;i++)
            pktGetBitsArray(pak,sizeof(RoomDoor)*8,&doors[i]);

        if (!(playerCanModifyBase(e,&g_base) && e->pl->architectMode == kBaseEdit_Architect))
        {
            noAccess = true;
            baseEditFailed(e, "NoAccess");
            break;
        }

        if (!g_base.roomBlockSize && base_block_size)
            g_base.roomBlockSize = base_block_size;
        else
            g_base.roomBlockSize = BASE_BLOCK_SIZE;

        if(roomUpdate(e, room_id,size,pos,height,info,doors,door_count))
            LOG_ENT( e, LOG_SUPERGROUP, LOG_LEVEL_IMPORTANT, 0, "Base:Room (SG id: %i, Room Id: %i) Room (%s) moved or added. Pos(%f,%f)", e->supergroup_id, room_id, info->pchName, pos[0], pos[1] );

    }
    xcase BASENET_ROOM_MOVE:
    {
        RoomDoor    doors[64];
        int            i,room_id,door_count,rotation;
        Vec3        pos;

        room_id = pktGetBitsPack(pak,1);
        rotation = pktGetBitsPack(pak,1);
        pktGetVec3(pak,pos);

        door_count = pktGetBitsPack(pak,1);
        if (door_count >= ARRAY_SIZE(doors))
            break;
        for(i=0;i<door_count;i++)
            pktGetBitsArray(pak,sizeof(RoomDoor)*8,&doors[i]);

        if (!(playerCanModifyBase(e,&g_base) && e->pl->architectMode == kBaseEdit_Architect))
        {
            noAccess = true;
            baseEditFailed(e, "NoAccess");
            break; 
        }

        if( roomMove( room_id, rotation, pos, doors, door_count) )
            LOG_ENT( e, LOG_SUPERGROUP, LOG_LEVEL_IMPORTANT, 0, "Base:Room (SG id: %i, Room Id: %i) Room Moved.", e->supergroup_id, room_id );
    }
    xcase BASENET_ROOM_DELETE:
    {
        int room_id = pktGetBitsPack(pak,1);
        if (!(playerCanModifyBase(e,&g_base) && e->pl->architectMode == kBaseEdit_Architect))
        {
            noAccess = true;
            baseEditFailed(e, "NoAccess");
            break;
        }

        if(roomDelete(e, room_id, &force_save))
            LOG_ENT( e, LOG_SUPERGROUP, LOG_LEVEL_IMPORTANT, 0,  "Base:Room (SG id: %i, Room Id: %i) Room deleted.", e->supergroup_id, room_id );

    }
    xcase BASENET_PLOT:
    {
        const BasePlot * plot = basedata_GetBasePlotByName( pktGetString(pak) );
        Vec3 pos = {0};

        pos[0] = pktGetF32( pak );
        pos[2] = pktGetF32( pak );
        pos[1] = pktGetBits(pak,1);

        if (!(playerCanModifyBase(e,&g_base) && e->pl->architectMode == kBaseEdit_Plot))
        {
            noAccess = true;
            baseEditFailed(e, "NoAccess");
            break;
        }

         plotUpdate( e, plot, pos );
        groupDefModify(g_base.def);
    }
    xcase BASENET_PERMISSIONS:
    {
        int            room_id;
        RoomDetail    detail = {0};
        BaseRoom    *room;
        char        name[128];

        room_id = pktGetBitsPack(pak,1);
        room = baseGetRoom(&g_base,room_id);

        if (room)
        {
            room->mod_time++;
        }

        detail.id = pktGetBitsPack(pak,1);
        detail.permissions = pktGetBitsPack(pak,1);
        strcpy(name,pktGetString(pak));

        if (!(sgroup_hasPermission(e, SG_PERM_RANKS)))
        {
            noAccess = true;
            break;
        }

        detail.info = basedata_GetDetailByName(name);

        processPermissions(e,room,&detail,&force_save);
    }
    break;

    default:
        return 0;
    }

    if( !noAccess )
    {
        //baseCheckFloatingObject(&g_base, e);
        baseToDefs(&g_base, 0);
        sgroup_SaveBase(&g_base, NULL, force_save);
    }

    return 1;
}
