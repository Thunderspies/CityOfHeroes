/***************************************************************************
 *     Copyright (c) 2000-2003, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include <utilitieslib/stdtypes.h>
#include <utilitieslib/utils/log.h>
#include "dbcomm/logcomm.h"
#include <utilitieslib/utils/wininclude.h>
#include "bases/basestorage.h"
#include <stdio.h>
#include "svr/svr_base.h"
#include "entity/entserver.h"
#include <utilitieslib/assert/assert.h>
#include "ai/entai.h"
#include "entity/entVarUpdate.h"
#include "dbcomm/dbquery.h"
#include "gameComm/svr_chat.h"
#include "varutils.h"
#include "parseClientInput.h"
#include "clientEntityLink.h"
#include "gameComm/sendToClient.h"
#include "entity/entGameActions.h"
#include "door.h"
#include "dbcomm/dbcontainer.h"
#include "container/containerbroadcast.h"
#include "group/groupdb_util.h"
#include "dbcomm/dbdoor.h"
#include "dbcomm/dbmapxfer.h"
#include "svr/svr_player.h"
#include "dbcomm/dbcomm.h"
#include "combat/combat.h"
#include <utilitieslib/utils/error.h>
#include <utilitieslib/components/HashFunctions.h>
#include "player/pmotion.h"
#include "player/playerState.h" // client-side file
#include "entity/entity.h"
#include "language/langServerUtil.h"
#include "ai/entai.h"
#include "generator/encounter.h"
#include "entity/costume.h"
#include "entity/power_customization.h"
#include "entity/power_customization_server.h"
#include "comm_game.h"
#include "entity/character_combat.h"
#include "entity/character_tick.h"
#include "entity/character_animfx.h"
#include "entity/character_eval.h"
#include "entity/entity_power.h"
#include "entity/character_base.h"
#include "entity/character_inventory.h"
#include "bases/basedata.h"
#include "entity/classes.h"
#include "entity/origins.h"
#include "filter/titles.h"
#include "entity/SgrpServer.h"
#include "gameComm/wdwbase.h"
#include "gameComm/svr_chat.h"
#include "entity/character_net_server.h"
#include "entity/character_level.h"
#include "entity/entPlayer.h"
#include "plaque.h"
#include "zowie.h"
#include "gameComm/NpcServer.h"
#include "storyarc/storyarcinterface.h"
#include "storyarc/pnpc.h"
#include "gameComm/trading.h"
#include "gameSys/dooranim.h"
#include "gameComm/dooranimcommon.h"
#include "entity/keybinds.h"
#include "gameComm/automapServer.h"
#include "dbcomm/staticMapInfo.h"
#include "storyarc/task.h"
#include "entity/PowerInfo.h"
#include "gameData/store_net.h"
#include <utilitieslib/utils/utils.h>
#include "storyarc/TeamTask.h"
#include <utilitieslib/utils/timing.h>
#include "entity/friends.h"
#include <utilitieslib/utils/strings_opt.h>
#include "beacon/beaconPath.h"
#include "beacon/beaconPrivate.h"
#include "gameComm/gift.h"
#include "entity/character_respec.h"
#include "Reward.h"
#include "entity/chatSettings.h"
#include "dbcomm/shardcomm.h"
#include "entity/entStrings.h"
#include "storyarc/storyarcutil.h" //for saUtilEntSays.  Maybe should be rearranged
#include "gameSys/arenamapserver.h"
#include "gameSys/arenamap.h"
#include "gameSys/arenakiosk.h"
#include "gameSys/sgraid.h"
#include "gameComm/trayCommon.h"
#include "entity/character_net.h"
#include "script/scriptengine.h"
#include "cmdparse/cmdserver.h"
#include "group/group.h"
#include "bases/baseserverrecv.h"
#include "entity/Invention.h"
#include "gameSys/petarena.h"
#include "gameComm/pet.h"
#include "gameSys/raidmapserver.h"
#include "entity/character_workshop.h"
#include "bases/baseserver.h"
#include "bases/bases.h"
#include "storyarc/mission.h"
#include "gameSys/raidmapserver.h"
#include "search.h"
#include "bases/baseupkeep.h"
#include "entity/Supergroup.h"
#include "filter/validate_name.h"
#include "entity/mapHistory.h"
#include "player/badges_server.h"
#include "container/team.h"
#include "gameComm/VillainDef.h"
#include "entity/powers.h"
#include "bases/DetailRecipe.h"
#include "auction/AuctionClient.h"
#include "auction/Auction.h"
#include "storyarc/taskforce.h"
#include <utilitieslib/language/MessageStoreUtil.h>
#include "storyarc/playerCreatedStoryarc.h"
#include "storyarc/playerCreatedStoryarcServer.h"
#include "gameSys/sgraid_V2.h"
#include "mission/MissionSearch.h"
#include "mission/missionserver_meta.h"
#include "gameSys/DayJob.h"
#include "entity/pvp.h"
#include <utilitieslib/UtilsNew/meta.h>
#include "dbcomm/dbnamecache.h"
#include "entity/buddy_server.h"
#include "entity/tailor.h"
#include "entity/character_mods.h"
#include "entity/character_target.h"
#include "player/pophelp.h"
#include "gameSys/Turnstile.h"
#include "gameSys/EndGameRaid.h"
#include "auth/authUserData.h"
#include "entity/alignment_shift.h"
#include "entity/entVarUpdate.h"
#include "account/AccountInventory.h"
#include "account/AccountTypes.h"
#include "container/league.h"
#include "gameComm/search.h"


extern U32 frame_count;

#define NUM_IDS     ( 1 )

//called from resume Character and lf ParseClientInput
void completeGurneyXferWork( Entity *e )
{
    if(e && e->pchar)
    {
        // By forcing fHitPoints > 0, the character will automatically
        // resurrect in character_tick.c (phase 2) on the next tick.
        
        if (server_visible_state.isPvP && getMapType() == MAPTYPE_STATIC)
        {        
            // If we're in a PvP zone and are reviving via a hospital gurney
            // we become immune to PvP for a few seconds

            e->pchar->bPvPActive = false;
            e->pvpZoneTimer = 30.0;
            e->pvp_update = true;
        }

        e->pchar->attrCur.fHitPoints = e->pchar->attrMax.fHitPoints;
        e->pchar->attrCur.fEndurance = e->pchar->attrMax.fEndurance;
    }
}


//from resume character only
int getIdxFromCookie( int ent_idx_cookie )
{
    int i;

    for(i=1; i<entities_max; i++ )
        if( entities[i] &&
            entity_state[i] & (ENTITY_IN_USE | ENTITY_SLEEPING) &&
            ent_idx_cookie == ( int )entities[i]->dbcomm_state.login_cookie )
            return i;

    return -1;
}


static SendNewPlayerWelcome(Entity *e)
{
    char ach[1024];
    char *pch = strrchr(db_state.map_name, '/');
    if(pch==NULL) pch=db_state.map_name-1;
    strcpy(ach, "NewPlayer_");
    strcat(ach, pch+1);
    plaque_SendPlaqueNag(e, ach,10);
}


void setInitialPosition(Entity *e, int new_character)
{
    DoorEntry    *door=0;
    Vec3 dest_pos;
    Vec3 dest_pyr;
    Vec3 abs_pos;
    int use_abs_pos = 0;
    MapHistory *hist = mapHistoryLast(e->pl);

    copyVec3(e->pl->last_static_pos,dest_pos);
    copyVec3(e->pl->last_static_pyr,dest_pyr);
    // find doors, etc in preparation for moving
    if (e->db_flags & DBFLAG_DOOR_XFER)
    {
        // Receiving end of a StaticReturn.  Fake it and make it look like a teleport transfer
        if (strnicmp(e->spawn_target, "coord:", 6) == 0)
        {
            int i;
            // No door
            door = NULL;
            // Turn off the door flag, turn on the teleport flag
            e->db_flags = (e->db_flags & ~DBFLAG_DOOR_XFER) | DBFLAG_TELEPORT_XFER;
            // Use an explicit for loop to move, this kills two birds with one stone.
            // Firstly, the overlapped copy is guaranteed to work, and the commas in here
            // are replaced by colons.  We also convert exclamation points back to periods,
            // to protect this against calls to strtok("??", ".")
            for (i = 0; e->spawn_target[i + 6]; i++)
            {
                e->spawn_target[i] = e->spawn_target[i + 6];
                if (e->spawn_target[i] == ',')
                {
                    e->spawn_target[i] = ':';
                }
                else if (e->spawn_target[i] == '!')
                {
                    e->spawn_target[i] = '.';
                }
            }
            // add the trailing :1 for an exact landing.
            e->spawn_target[i++] = ':';
            e->spawn_target[i++] = '1';
            e->spawn_target[i] = 0;
        }
        else
        {
            if (strnicmp(e->spawn_target, "abspos:", 7) == 0)
            {
                int i;
                char spawn_target_str[300];
                char *spawn_target;

                spawn_target_str[0] = '\0';

                // Skip the leading identifier.
                i = sscanf(e->spawn_target + 7, "%f,%f,%f,%s", &abs_pos[0], &abs_pos[1], &abs_pos[2], spawn_target_str);

                if (i == 4)
                {
                    // Force our destination position to be what was sent
                    // to us and prepare to restore the spawn target.
                    use_abs_pos = 1;
                    spawn_target = &(spawn_target_str[0]);
                }
                else
                {
                    // We're missing something so let's hope the name is at
                    // least on the end. If it's not, we won't get a door and
                    // the usual failure path will occur.
                    spawn_target = strrchr(e->spawn_target, ',');
                }

                i = 0;

                // We've grabbed the extra data so now we can set the target
                // name as it would normally appear.
                while (spawn_target && *spawn_target)
                {
                    e->spawn_target[i] = *spawn_target;

                    spawn_target++;
                    i++;
                }

                e->spawn_target[i] = '\0';
                door = dbFindSpawnLocation(e->spawn_target);
            }

            // No need to find door again if some special-case code has
            // done it for us.
            if (!door)
                door = dbFindSpawnLocation(e->spawn_target);

            if (strstri(e->spawn_target,"Gurney") )
                completeGurneyXferWork(e);

            if (!door)
                svrSendPlayerError(e,"Can't find spawn: %s on this map(sent from previous map)\n",e->spawn_target);
        }
    }
    else if (e->db_flags & DBFLAG_INTRO_TELEPORT)
    {
        door = dbFindSpawnLocation("NewPlayer");
        if (!door) {
            if (ENT_IS_HERO(e))
                door = dbFindSpawnLocation("NewPlayerHero");
            else 
                door = dbFindSpawnLocation("NewPlayerVillain");
        }
        StoryArcInfoReset(e); // get rid of intro contacts, etc.
        SendNewPlayerWelcome(e);
        if (!door)
            svrSendPlayerError(e,"Can't find spawn: %s on this map(sent from previous map)\n",e->spawn_target);
    }
    else if (e->db_flags & DBFLAG_MISSION_ENTER)
    {
        door = dbFindSpawnLocation("MissionStart");
        if (!door)
            svrSendPlayerError(e,"Can't find MissionStart spawn location");
    }
    else if (e->db_flags & DBFLAG_MISSION_EXIT)
    {
        if (hist && hist->mapType == getMapType() && hist->mapID == getMapID())
        { //If we're returning to a map on the history, go to that location
            copyVec3(hist->last_pos,dest_pos);
            copyVec3(hist->last_pyr,dest_pyr);
        }
        // if exiting from a mission, turn around 180 degrees
        dest_pyr[1] = addAngle(dest_pyr[1], RAD(180));
        // door is NULL, so we will get position set correctly below
    }
    else if (e->db_flags & DBFLAG_BASE_EXIT)
    {
        if (hist && (hist->mapType == MAPTYPE_MISSION || hist->mapType == MAPTYPE_STATIC) && getMapType() == MAPTYPE_STATIC)
        { // If we were in a mission before this, but are returning to a static, go to a teleport dest
            door = dbFindSpawnLocation("BaseTeleportSpawn");
        } 
        else if (hist && hist->mapType == getMapType() && hist->mapID == getMapID())
        { //If we're returning to a map on the history, go to that location
            copyVec3(hist->last_pos,dest_pos);
            copyVec3(hist->last_pyr,dest_pyr);
        }
        if (!door)
        {
            // If we're not porting to a specific door, rotate 180 from last position
            dest_pyr[1] = addAngle(dest_pyr[1], RAD(180));
            // door is NULL, so we will get position set correctly below
        }
    }
    else if (e->db_flags & DBFLAG_MAPMOVE)
    {
        // make admins who are map moving come out of a random door
        door = debugGetRandomExitDoor();
    }
    else if ( new_character )
    {
        door = dbFindSpawnLocation("NewPlayer");
        if (!door) {
            if (ENT_IS_HERO(e))
                door = dbFindSpawnLocation("NewPlayerHero");
            else 
                door = dbFindSpawnLocation("NewPlayerVillain");
        }
        if (!door)
            Errorf("Can't find spawn: %s\n","NewPlayer");

        SendNewPlayerWelcome(e);
    }

    // if we have a specific location, use it
    if (door)
    {
        entUpdateMat4Instantaneous(e,door->mat);

        // The source door is forcing us to its position for seamless
        // zoning.
        if (use_abs_pos)
        {
            entUpdatePosInstantaneous(e, abs_pos);
        }
    }
    else if (e->db_flags & DBFLAG_TELEPORT_XFER)
    {
        Vec3 pos;
        int exact=0;
        int count=0;

        count = sscanf(e->spawn_target, "%f:%f:%f:%d", &pos[0], &pos[1], &pos[2],&exact);

        if (count == 4 || count == 3) // Old characters might still have a 3 value pair sitting in the database
        {
            setInitialTeleportPosition(e, pos, exact);
        }
        else
        {
            devassert(!"Bad teleport position");
        }
    }
    else if (e->db_flags & DBFLAG_BASE_ENTER && !db_state.is_static_map ) // This catches the case of a failed base enter on gurney xfer
    {
        if (g_isBaseRaid)
        {
            door = dbFindSpawnLocationForBaseRaid(e->spawn_target, e);
        }
        else
        {
            door = dbFindSpawnLocation(e->spawn_target);
        }
        if ( strstri(e->spawn_target,"Gurney") && door && door->detail_id )
        {
            completeBaseGurneyXferWork(e,door->detail_id);
        }
        if (door) 
        {
            entUpdateMat4Instantaneous(e,door->mat);
        }
        else
        {        
            // HACK - just setting to 10,10,10 right now to get player
            // in the initial room - will be reset to a proper door loc after
            // bases are more worked out
            Vec3 pos = {10.0, 10.0, 10.0};
            entUpdatePosInstantaneous(e, pos);
        }
    }
    else if (db_state.is_static_map || db_state.local_server || 
        (hist && hist->mapType == getMapType() && hist->mapID == getMapID()))
    {
        // otherwise, use our last saved position
        Mat4 loc;
        createMat3YPR(loc, dest_pyr);
        copyVec3(dest_pos, loc[3]);
        entUpdateMat4Instantaneous(e,loc);
    }
    // if all else fails, leave mat alone?

    ArenaMapSetPosition(e);    // arena allowed to override and put people where it wants to

    e->initialPositionSet = 1;

    // make sure none of the door flags stay set
    e->db_flags &= ~(DBFLAG_MISSION_EXIT | DBFLAG_MISSION_ENTER | DBFLAG_DOOR_XFER | DBFLAG_TELEPORT_XFER | DBFLAG_INTRO_TELEPORT | DBFLAG_MAPMOVE | DBFLAG_BASE_ENTER | DBFLAG_BASE_EXIT);
}


// This function is called afte the client is receiving entity updates and has acknowleged receiving himself,
//  we can now send him packets that require him to have an player entity on the client
void clientReceivedCharacter(ClientLink *client)
{
    if (client->entity) {
        client->entity->ready = 1; // The client has his own entity it's safe to send him stuff now
        // Clear out Login cookie, just in case
        client->entity->dbcomm_state.login_cookie = 0;
        // Un-freeze and un-invincify the player, he's in the game now!
        client->entity->controlsFrozen = 0;
        client->entity->untargetable &= ~UNTARGETABLE_TRANSFERING;
        client->entity->invincible &= ~INVINCIBLE_TRANSFERING;
        //client->ready = CLIENTSTATE_IN_GAME; // Should already be this!

        // Packets have a tendency to get lost at startup. We
        //   occasionally miss shapeshift costume packets at startup.
        //   So, forcibly send a costume update if the hero is in an
        //   npc costume.
        if(client->entity->pl->npc_costume)
        {
            client->entity->send_costume = 1;
        }

        ArenaMapReceivedCharacter(client->entity);    // needs to freeze them again
        badge_RecordLevelChange(client->entity);
    }
}

// This check should match uiLogin.c - ClientCheckDisallowedPlayer
int MapCheckDisallowedPlayer(Entity *player)
{
    int i;
    int allowed = 1;
    const CharacterClass *pclass = NULL;

    if (player && player->pl)
    {
        if (player->pl->doNotKick)
            return 0;

        if (player && ENT_IS_IN_PRAETORIA(player) && !player->access_level)
        {
            allowed = (authUserHasProvisionalRogueAccess((U32*)player->pl->auth_user_data)
                        || AccountHasStoreProduct(ent_GetProductInventory(player), SKU("ctexgoro")));
        }

        if (player->pl->validateCostume)
        {
//            allowed = false;
            if (costume_Validate( player, cpp_reinterpret_cast(Costume*)(player->costume), player->pl->current_costume, NULL, NULL, 0, false ) != 0)
                allowed = false;
            else
                player->pl->validateCostume = false;
                
        }

        // HYBRID : Validate class and powerset
        if (player->pchar)
            pclass = player->pchar->pclass;

        if (pclass && allowed)
        {            
            if (class_MatchesSpecialRestriction(pclass, "Kheldian") && !authUserHasKheldian(player->pl->auth_user_data))
            {
                if (pclass->pchStoreRestrictions)
                {
                    allowed &= accountEval(&player->pl->account_inventory, player->pl->loyaltyStatus, player->pl->loyaltyPointsEarned, player->pl->auth_user_data, pclass->pchStoreRestrictions);
                } else {
                    allowed = false;
                }
            } else if ((class_MatchesSpecialRestriction(pclass, "ArachnosSoldier") || class_MatchesSpecialRestriction(pclass, "ArachnosWidow")) &&
                        !authUserHasArachnosAccess(player->pl->auth_user_data))
            {
                if (pclass->pchStoreRestrictions)
                {
                    allowed &= accountEval(&player->pl->account_inventory, player->pl->loyaltyStatus, player->pl->loyaltyPointsEarned, player->pl->auth_user_data, pclass->pchStoreRestrictions);
                } else {
                    allowed = false;
                }
            } else {
                if (pclass->pchStoreRestrictions)
                {
                    allowed &= accountEval(&player->pl->account_inventory, player->pl->loyaltyStatus, player->pl->loyaltyPointsEarned, player->pl->auth_user_data, pclass->pchStoreRestrictions);
                }
            }

            // powersets                
            for(i = eaSize(&player->pchar->ppPowerSets)-1; i >= 0; i--)
            {
                PowerSet *pPowerSet = player->pchar->ppPowerSets[i];
                if (pPowerSet->psetBase != NULL && 
                    pPowerSet->psetBase->pchAccountRequires != NULL && 
                    !accountEval(&player->pl->account_inventory, player->pl->loyaltyStatus, player->pl->loyaltyPointsEarned, player->pl->auth_user_data, pPowerSet->psetBase->pchAccountRequires))
                {
                    allowed = false;
                    break;
                }
            }
        }

        if (player->pl->slotLockLevel != SLOT_LOCK_NONE)
        {
            allowed = false;
        }
    }
    return !allowed;
}

static int fixTravellingPraetorian(Entity *e)
{
    int retval = 0;
    int dest_map_id = MAP_PRAETORIAN_ARRIVE_HERO;

    if (ENT_IS_LEAVING_PRAETORIA(e))
    {
        // We directly set the player type rather than using the accessor
        // because that function does various things we don't want like trying
        // to set the location-type and update contacts and so on.
        if (e->pl && e->pl->praetorianProgress == kPraetorianProgress_TravelHero)
        {
            e->pl->playerType = kPlayerType_Hero;
        }
        else if (e->pl && e->pl->praetorianProgress == kPraetorianProgress_TravelVillain)
        {
            e->pl->playerType = kPlayerType_Villain;
            dest_map_id = MAP_PRAETORIAN_ARRIVE_VILLAIN;
        }
        else
        {
            log_printf("Unpack", "DBID %d is a travelling Praetorian in unknown state: %d\n", e->pl->praetorianProgress);
            devassert(0);
        }

        // Common aspects of travelling can be dealt with here.
        entSetPraetorianProgress(e, kPraetorianProgress_PrimalEarth);
        entSetPlayerSubType(e, kPlayerSubType_Normal);
        alignmentshift_updatePlayerTypeByLocation(e);

        updatePraetorianEventChannel(e);
        StoryArcLeavingPraetoriaReset(e);

        // If we're not their intended destination, help them on their way.
        if (db_state.base_map_id != dest_map_id)
        {
            e->adminFrozen = 1;
            e->db_flags |= DBFLAG_MAPMOVE;
            dbAsyncMapMove(e, dest_map_id, NULL, XFER_STATIC);

            retval = 1;
        }
    }

    return retval;
}

// Checks to see if a player is in a zone of the opposing faction
static int MapCheckBootPlayer(Entity* player)
{
    int retval = 0;

    // Only boot people without access level on a static map.
    if (!player->access_level && !OnInstancedMap())
    {
        const MapSpec* mapSpec = MapSpecFromMapId(db_state.base_map_id);
        StaticMapInfo *mapInfo = staticMapInfoFind(db_state.base_map_id);
        if (!mapSpec || !mapInfo)
            return 0;

        // Non-accesslevel people cannot be on dev-only maps.
        if (MAP_ALLOWS_DEV_ONLY(mapSpec))
            retval = 1;
        // Praetorians cannot be in Primal Earth and Primals cannot be in
        // Praetorian Earth.
        else if ((ENT_IS_IN_PRAETORIA(player) && !MAP_ALLOWS_PRAETORIANS(mapSpec)) ||
            (!ENT_IS_IN_PRAETORIA(player) && !MAP_ALLOWS_PRIMAL(mapSpec)))
            retval = 1;
        // Villains cannot be in hero-only maps, nor can heroes be in villain-only maps.
        else if ((!ENT_IS_ROGUE(player) && ENT_IS_VILLAIN(player) && !MAP_ALLOWS_VILLAINS(mapSpec)) ||
            (!ENT_IS_ROGUE(player) && ENT_IS_HERO(player) && !MAP_ALLOWS_HEROES(mapSpec)))
            retval = 1;
        // Nobody gets into Praetorian flashback!
        else if (db_state.base_map_id == 3650 || db_state.base_map_id == 3950 ||
            db_state.base_map_id == 3750 || db_state.base_map_id == 4250 ||
            db_state.base_map_id == 3850 || db_state.base_map_id == 5150)
            retval = 1;
        // Praetorians who've finished the tutorial need to be kicked into Nova Praetoria
        else if (mapInfo->introZone == 3 && player->pl && player->pl->praetorianProgress == kPraetorianProgress_Praetoria)
            retval = 1;

        if (retval)
        {
            int home_map_id;

            if( ENT_IS_IN_PRAETORIA(player) )
                home_map_id = MAP_PRAETORIAN_START;
            else if( ENT_IS_VILLAIN(player) )
                home_map_id = MAP_VILLAIN_START;
            else
                home_map_id = MAP_HERO_START;

            LOG_OLD( "Player %s: got onto map %i(base %i), sending him home", player->name, db_state.map_id, db_state.base_map_id);
            player->adminFrozen = 1;
            player->db_flags |= DBFLAG_MAPMOVE;
            dbAsyncMapMove(player, home_map_id, NULL, XFER_STATIC);
        }
    }
    return retval;
}

static void LeagueTeamOnEnterMapUpdate(Entity *e)
{
    if (e)
    {
        e->team_buff_full_update = true;
        e->teamlist_uptodate = 0;
        e->leaguelist_uptodate = 0;
        if (e->teamup)
        {
            int i;
            for (i = 0; i < e->teamup->members.count; ++i)
            {
                Entity *mate = entFromDbId(e->teamup->members.ids[i]);
                if (mate)
                {
                    mate->team_buff_full_update = true;
                    mate->teamlist_uptodate = 0;
                }
            }
        }

        if (e->league)
        {
            int i;
            for (i = 0; i < e->league->members.count; ++i)
            {
                Entity *mate = entFromDbId(e->league->members.ids[i]);
                if (mate)
                {
                    mate->team_buff_full_update = true;
                    mate->leaguelist_uptodate = 0;
                }
            }
        }
    }
    if (e && e->pl && !e->pl->lastTurnstileEventID && e->pl->lastTurnstileMissionID)
    {
        int instanceID = e->pl->lastTurnstileMissionID;
        e->pl->lastTurnstileMissionID = 0;
        league_AcceptOfferTurnstile(instanceID, e->db_id, e->name, e->pl->desiredTeamNumber, e->pl->isTurnstileTeamLeader);
        e->pl->desiredTeamNumber = -2;
        START_PACKET(pak_out, e, SERVER_EVENT_START_STATUS);
        pktSendBits(pak_out, 1, 1);
        pktSendBits(pak_out, 1, 0);
        END_PACKET;
    }
}

// returns 0 (FALSE), 1 (TRUE), or -1
int resumeCharacter(ClientLink* client, int ent_idx_cookie)
{
    int            i = 1;
    Entity        *e;

    i = getIdxFromCookie( ent_idx_cookie );

    //printf("Made it to resume Character, cookie is %d, ent idx is %d.\n", ent_idx_cookie, i );
    if( i == -1 )
    {
        LOG_OLD_ERR("Can't find match for login cookie in resumeCharacter! cookie: %x from %s:%d", ent_idx_cookie, makeIpStr(client->link->addr.sin_addr.S_un.S_addr), client->link->addr.sin_port);
        return FALSE;
    }

    e = entities[i];
    clientSetEntity(client, e);

    entity_state[i] = ENTITY_IN_USE;

    updateFriendList(e);
    updateCombatMonitorStats(e);
    

    aiInit(e, NULL);

    entSendPlrPosQrot(entities[i]);
    entSendPlrPosQrot(entities[i]); // need to send twice here, cuz of a slight init issue
    //entities[i]->send_costume = TRUE;

    // Enable all powers that you own.
    character_UpdateAllEnabledState(e->pchar);
    // Turn back on any auto powers.
    character_ActivateAllAutoPowers(e->pchar);
    // Make sure all active toggle powers have their FX on.
    character_TurnOnActiveTogglePowers(e->pchar);
    // Send down info on recharging powers
    powerInfo_UpdateFullRechargeStatus(e->pchar);

    //MW Exitting Door Trick 7-1-03
    if( (ENTTYPE(e) == ENTTYPE_PLAYER ) ) //Can expand to include Villains when we work out the edge cases
    {
        prepEntForEntryIntoMap( e, EE_USE_EMERGE_LOCATION_IF_NEARBY, XFER_STATIC );
    }

    // Temporarily freeze and invincify the player until he's actually received the big packet of data
    e->controlsFrozen = 1;
    e->untargetable |= UNTARGETABLE_TRANSFERING;
    e->invincible |= INVINCIBLE_TRANSFERING;

    // If they are travelling to Primal Earth and this isn't where they're
    // supposed to wind up, they'll get kicked to that other place.
    if (fixTravellingPraetorian(e))
    {
        return TRUE;
    }

    if (MapCheckBootPlayer(e))
        return TRUE;

    TaskForceResetDesaturate(e);    // make sure desaturate doesn't carry over from missions

    MissionPlayerEnteredMap(e); // mission system needs to see each player logging in
    BadgeFixCheckPlayer(e); // let the badge auto-fix system check the player logging in

    ArenaMapInitPlayer(e);
    RaidInitPlayer(e);
    LeagueTeamOnEnterMapUpdate(e);
    StoryArcPlayerEnteredMap(e);
    EndGameRaidPlayerEnteredMap(e); // Tell the end game raid system someone entered the map
    turnstileMapserver_generateTurnstilePing(e);    // Check Turnstile server status
    ScriptPlayerEnteringMap(e); //Must be after MissionInitInfo is done above in MissionPlayerEnteredMap
    RaidFullyUpdateClient(e);
    badge_RecordLevelChange(e);
    pvp_ApplyPvPPowers(e);

    //// If this isn't a Server that allows Hero and Villain or Primal and Praetorian team ups, and you are in a mixed team, quit it
    if( !OnArenaMap()  )
    {
        team_MinorityQuit(e,server_state.allowHeroAndVillainPlayerTypesToTeamUp,server_state.allowPrimalAndPraetoriansToTeamUp);
        league_MinorityQuit(e,server_state.allowHeroAndVillainPlayerTypesToTeamUp,server_state.allowPrimalAndPraetoriansToTeamUp);
    }

    if (e && e->pl)
    {
        // Clear the special return in progress flag since  we're done with the map transfer
        e->pl->specialMapReturnInProgress = 0;
    }

    // Need to find out what they've earned courtesy of other players and
    // try to get any pending status pushed up to the MissionServer.
    missionserver_map_requestInventory(e);
    missionserver_map_handlePendingStatus(e);

    // This should fix any characters that get stuck with this flag on them
    // but the map doesn't have any recollection why
    TODO(); // Refunds are disabled for now
    if (e->pl)
    {
        int pendingIndex;
    
        if (!orderIdIsNull(e->pl->pendingCertification))
        {
            OrderId *copy = (OrderId*)malloc(sizeof(OrderId));
            memcpy(copy, &e->pl->pendingCertification, sizeof(OrderId));
            eaPush(&e->pl->pending_orders, copy);

            e->pl->pendingCertification = kOrderIdInvalid;
        }

        for (pendingIndex = eaSize(&e->pl->pending_orders) - 1; pendingIndex >= 0; pendingIndex--)
        {
            AccountServerTransactionFinish(e->auth_id, *(e->pl->pending_orders[pendingIndex]), 1);
        }
    }

    return TRUE;
}

static PerformanceInfo* cmdTimer[CLIENTINP_CMD_COUNT];

const char* getClientInpCmdName(int cmd)
{
    static int                cmdTimerInit = 0;
    static char*            cmdTimerName[CLIENTINP_CMD_COUNT];

    if(!cmdTimerInit)
    {
        int i;
        int last = -1;
        char* msg = "A CLIENTINP cmd was listed out of order.  Keep these commands in order.";

        cmdTimerInit = 1;

        #define SET_NAME(x) {assertmsg(x == last + 1, msg);assert(x >= 0 && x < ARRAY_SIZE(cmdTimerName));cmdTimerName[x] = "cmd:" #x;last = x;}

        SET_NAME(CLIENTINP_SVRCMD);
        SET_NAME(CLIENTINP_VISITED_MAP_CELL);
        SET_NAME(CLIENTINP_SENDING_CHARACTER);
        SET_NAME(CLIENTINP_RECEIVED_CHARACTER);
        SET_NAME(CLIENTINP_ACK_FULL_UPDATE);
        SET_NAME(CLIENTINP_TOUCH_NPC);
        SET_NAME(CLIENTINP_RECEIVE_TRAYS);
        SET_NAME(CLIENTINP_ENTER_DOOR);
        SET_NAME(CLIENTINP_ENTER_BASE_CHOICE);
        SET_NAME(CLIENTINP_REQUEST_GURNEY_XFER);
        SET_NAME(CLIENTINP_REQUEST_WAYPOINT);
        SET_NAME(CLIENTINP_DOORANIM_DONE);
        SET_NAME(CLIENTINP_WINDOW);
        SET_NAME(CLIENTINP_DOCK_MODE);
        SET_NAME(CLIENTINP_CHAT_DIVIDER);
        SET_NAME(CLIENTINP_INSPTRAY_MODE);
        SET_NAME(CLIENTINP_TRAY_MODE);
        SET_NAME(CLIENTINP_KEYBIND);
        SET_NAME(CLIENTINP_KEYBIND_CLEAR);
        SET_NAME(CLIENTINP_KEYBIND_RESET);
        SET_NAME(CLIENTINP_KEYPROFILE);
        SET_NAME(CLIENTINP_CONTACT_DIALOG_RESPONSE);
        SET_NAME(CLIENTINP_CONTACT_CELL_CALL);
        SET_NAME(CLIENTINP_CONTACT_FLASHBACK_LIST_REQUEST);
        SET_NAME(CLIENTINP_CONTACT_FLASHBACK_DETAIL_REQUEST);
        SET_NAME(CLIENTINP_CONTACT_FLASHBACK_CHECK_ELIGIBILITY_REQUEST);
        SET_NAME(CLIENTINP_CONTACT_FLASHBACK_ACCEPT_REQUEST);
        SET_NAME(CLIENTINP_CONTACT_TASKFORCE_ACCEPT_REQUEST);
        SET_NAME(CLIENTINP_ACTIVATE_POWER);
        SET_NAME(CLIENTINP_ACTIVATE_POWER_AT_LOCATION);
        SET_NAME(CLIENTINP_ACTIVATE_INSPIRATION);
        SET_NAME(CLIENTINP_SET_DEFAULT_POWER);
        SET_NAME(CLIENTINP_CLEAR_DEFAULT_POWER);
        SET_NAME(CLIENTINP_CANCEL_QUEUED_POWER);
        SET_NAME(CLIENTINP_CANCEL_WINDUP_POWER);
        SET_NAME(CLIENTINP_MOVE_INSPIRATION);
        SET_NAME(CLIENTINP_DEAD_NOGURNEY_RESPONSE);
        SET_NAME(CLIENTINP_STANCE);
        SET_NAME(CLIENTINP_SET_CHAT_CHANNEL);
        SET_NAME(CLIENTINP_SEND_CHAT_SETTINGS);
        SET_NAME(CLIENTINP_SEND_SELECTED_CHAT_TABS);
        SET_NAME(CLIENTINP_READ_PLAQUE);

        SET_NAME(CLIENTINP_COMBINE_BOOSTS);
        SET_NAME(CLIENTINP_BOOSTER_BOOST);
        SET_NAME(CLIENTINP_CATALYST_BOOST);
        SET_NAME(CLIENTINP_SET_INVENTORY_BOOST);
        SET_NAME(CLIENTINP_SET_POWER_BOOST);
        SET_NAME(CLIENTINP_REMOVE_INVENTORY_BOOST);
        SET_NAME(CLIENTINP_REMOVE_POWER_BOOST);
        SET_NAME(CLIENTINP_UNSLOT_POWER_BOOST);

        SET_NAME(CLIENTINP_LEVEL_ASSIGN_BOOST_SLOT);
        SET_NAME(CLIENTINP_LEVEL_BUY_POWER);
        SET_NAME(CLIENTINP_LEVEL);

        // teamup commands
        SET_NAME(CLIENTINP_TEAM_OFFER);
        SET_NAME(CLIENTINP_TEAM_BUSY);
        SET_NAME(CLIENTINP_TEAM_ACCEPT);
        SET_NAME(CLIENTINP_TEAM_DECLINE);
        SET_NAME(CLIENTINP_TEAM_KICK);
        SET_NAME(CLIENTINP_LEGACYTEAMUI_NOTE);
        SET_NAME(CLIENTINP_SUPERGROUP_TOGGLE);
        SET_NAME(CLIENTINP_SUPERGROUP_LIST_REQUEST);

        // base raids
        SET_NAME(CLIENTINP_RAID_CHALLENGE);
        SET_NAME(CLIENTINP_REQ_SGRAIDLIST);
        SET_NAME(CLIENTINP_BASERAID_PARTICIPATE);
        SET_NAME(CLIENTINP_BASERAID_PARTICIPATE_TRANSFER);
        SET_NAME(CLIENTINP_RAID_SCHEDULE);
        SET_NAME(CLIENTINP_RAID_CANCEL);

        // trade
        SET_NAME(CLIENTINP_TRADE_CANCEL);
        SET_NAME(CLIENTINP_TRADE_UPDATE);

        SET_NAME(CLIENTINP_REQUEST_PLAYER_INFO);
        SET_NAME(CLIENTINP_UPDATE_COMBAT_STATS);
        SET_NAME(CLIENTINP_SGROUP_CREATE);
        SET_NAME(CLIENTINP_SEND_SGCOSTUME);
        SET_NAME(CLIENTINP_SEND_SGCOSTUME_DATA);
        SET_NAME(CLIENTINP_SGROUP_COSTUMEREQ);
        SET_NAME(CLIENTINP_HIDE_EMBLEM_UPDATE);
        SET_NAME(CLIENTINP_SEND_TAILOR);
        SET_NAME(CLIENTINP_SEND_POWER_CUST);
        SET_NAME(CLIENTINP_SEND_RESPEC);

        // visit location commands
        SET_NAME(CLIENTINP_TASK_REACHED_VISIT_LOCATION);
        SET_NAME(CLIENTINP_REQUEST_TASK_DETAIL);

        SET_NAME(CLIENTINP_THIRD);
        SET_NAME(CLIENTINP_OPTIONS);
        SET_NAME(CLIENTINP_DIVIDERS);
        SET_NAME(CLIENTINP_TITLES);
        SET_NAME(CLIENTINP_PLAYERDESC);

        SET_NAME(CLIENTINP_REQUEST_SOUVENIR_INFO);

        SET_NAME(CLIENTINP_STORE_SELL_ITEM);
        SET_NAME(CLIENTINP_STORE_BUY_ITEM);
        SET_NAME(CLIENTINP_STORE_BUY_SALVAGE);
        SET_NAME(CLIENTINP_STORE_BUY_RECIPE);

        SET_NAME(CLIENTINP_ABANDON_MISSION);

        SET_NAME(CLIENTINP_GIFTSEND);
        SET_NAME(CLIENTINP_ACCEPT_POWREQ);
        SET_NAME(CLIENTINP_ACCEPT_TEAMLEVEL);

        SET_NAME(CLIENTINP_REWARDCHOICE);

        SET_NAME(CLIENTINP_RESPEC_CANCEL);

        SET_NAME(CLIENTINP_NOTAHACKER);

        SET_NAME(CLIENTINP_ARENA_REQKIOSK);
        SET_NAME(CLIENTINP_ARENA_REQ_CREATE);
        SET_NAME(CLIENTINP_ARENA_REQ_RESULT);
        SET_NAME(CLIENTINP_ARENA_REQ_OBSERVE);
        SET_NAME(CLIENTINP_ARENA_REQ_JOIN);
        SET_NAME(CLIENTINP_ARENA_REQ_JOIN_INVITE);
        SET_NAME(CLIENTINP_ARENA_REQ_DESTROY);
        SET_NAME(CLIENTINP_ARENA_REQ_SIDE);
        SET_NAME(CLIENTINP_ARENA_REQ_READY);
        SET_NAME(CLIENTINP_ARENA_REQ_SETMAP);
        SET_NAME(CLIENTINP_ARENA_REQ_ENTERMAP);
        SET_NAME(CLIENTINP_ARENA_CREATOR_UPDATE);
        SET_NAME(CLIENTINP_ARENA_PLAYER_UPDATE);
        SET_NAME(CLIENTINP_ARENA_FULL_PLAYER_UPDATE);
        SET_NAME(CLIENTINP_ARENA_DROP);
        SET_NAME(CLIENTINP_ARENAKIOSK_OPEN);
        SET_NAME(CLIENTINP_ARENA_REQ_TELEPORT);

        SET_NAME(CLIENTINP_PETCOMMAND);
        SET_NAME(CLIENTINP_PETSAY);
        SET_NAME(CLIENTINP_PETRENAME);
        SET_NAME(CLIENTINP_ARENA_MANAGEPETS);
        SET_NAME(CLIENTINP_ARENA_COMMITPETCHANGES);

        // crafting helpers
        SET_NAME(CLIENTINP_CRAFTING_REVERSE_ENGINEER);
        SET_NAME(CLIENTINP_INVENT_SELECTRECIPE);
        SET_NAME(CLIENTINP_INVENT_SLOTCONCEPT);
        SET_NAME(CLIENTINP_INVENT_UNSLOT);
        SET_NAME(CLIENTINP_INVENT_HARDENSLOT);
        SET_NAME(CLIENTINP_INVENT_FINALIZE);
        SET_NAME(CLIENTINP_INVENT_CANCEL);

        // detail creation
        SET_NAME(CLIENTINP_WORKSHOP_CREATE);
        SET_NAME(CLIENTINP_WORKSHOP_INTERACT);
        SET_NAME(CLIENTINP_WORKSHOP_CLOSE);

        // portal doors - dynamic doors
        SET_NAME(CLIENTINP_PORTAL_INTERACT);

        // base
        SET_NAME(CLIENTINP_BASE_EDIT);
        SET_NAME(CLIENTINP_BASE_SETPOS);
        SET_NAME(CLIENTINP_BASE_INTERACT);
        SET_NAME(CLIENTINP_REQ_WORLD_UPDATE);

        // base storage
        SET_NAME(CLIENTINP_BASESTORE_ADJ_SALVAGE_FROM_INV);
        SET_NAME(CLIENTINP_BASESTORAGE_ADJ_INSPIRATION_FROM_INV);
        SET_NAME(CLIENTINP_BASESTORAGE_ADJ_ENHANCEMENT_FROM_INV);
        
        // team completion
        SET_NAME(CLIENTINP_TEAMCOMPLETE_RESPONSE);
        SET_NAME(CLIENTINP_SEARCH);
        SET_NAME(CLIENTINP_UPDATE_NAME);

        SET_NAME(CLIENTINP_DELETE_TEMPPOWER);
        SET_NAME(CLIENTINP_DELETE_SALVAGE);
        SET_NAME(CLIENTINP_DELETE_RECIPE);
        SET_NAME(CLIENTINP_SELL_RECIPE);
        SET_NAME(CLIENTINP_SELL_SALVAGE);
        SET_NAME(CLIENTINP_OPEN_SALVAGE);
        SET_NAME(CLIENTINP_GET_SALVAGE_IMMEDIATE_USE_STATUS);
        SET_NAME(CLIENTINP_BUY_RECIPE);

        SET_NAME(CLIENTINP_AUCTION_REQ_INVUPDATE);
        SET_NAME(CLIENTINP_AUCTION_REQ_HISTORYINFO);
        SET_NAME(CLIENTINP_AUCTION_ADDITEM);
        SET_NAME(CLIENTINP_AUCTION_GETITEM);
        SET_NAME(CLIENTINP_AUCTION_CHANGEITEMSTATUS);
        SET_NAME(CLIENTINP_AUCTION_GETINFSTORED);
        SET_NAME(CLIENTINP_AUCTION_GETAMTSTORED);
        SET_NAME(CLIENTINP_AUCTION_BATCH_REQ_ITEMSTATUS);
        SET_NAME(CLIENTINP_AUCTION_LIST_REQ_ITEMSTATUS);
        SET_NAME(CLIENTINP_AUCTION_REQ_BANNEDUPDATE);

        SET_NAME(CLIENTINP_MOVE_SALVAGE_TO_PERSONALSTORAGE);
        SET_NAME(CLIENTINP_MOVE_SALVAGE_FROM_PERSONALSTORAGE);

        SET_NAME(CLIENTINP_STORYARC_CREATE);
        SET_NAME(CLIENTINP_MISSIONSERVER_COMMAND);
        SET_NAME(CLIENTINP_MISSIONSERVER_PUBLISHARC);
        SET_NAME(CLIENTINP_MISSIONSERVER_UNPUBLISHARC);
        SET_NAME(CLIENTINP_MISSIONSERVER_VOTEFORARC);
        SET_NAME(CLIENTINP_MISSIONSERVER_REPORTARC);
        SET_NAME(CLIENTINP_MISSIONSERVER_SEARCHPAGE);
        SET_NAME(CLIENTINP_MISSIONSERVER_ARCINFO);
        SET_NAME(CLIENTINP_MISSIONSERVER_ARCDATA_PLAYARC);
        SET_NAME(CLIENTINP_MISSIONSERVER_ARCDATA_VIEWARC);
        SET_NAME(CLIENTINP_MISSIONSERVER_COMMENT);
        SET_NAME(CLIENTINP_MISSIONSERVER_AUTHOR_BIO);
        SET_NAME(CLIENTINP_MISSIONSERVER_SETKEYWORDSFORARC);

        SET_NAME(CLIENTINP_ARCHITECTKIOSK_INTERACT);
        SET_NAME(CLIENTINP_ARCHITECTKIOSK_REQUESTINVENTORY);

        SET_NAME(CLIENTINP_REQUEST_SG_COLORS);
        SET_NAME(CLIENTINP_UPDATE_BADGE_MONITOR_LIST);
        SET_NAME(CLIENTINP_RENAME_BUILD_RESPONSE);
        SET_NAME(CLIENTINP_GET_PLAYNC_AUTH_KEY);
        SET_NAME(CLIENTINP_COSTUMECHANGEEMOTELIST_REQUEST);
        SET_NAME(CLIENTINP_DO_PROMOTE);
        SET_NAME(CLIENTINP_REQUEST_SG_PERMISSIONS);

        SET_NAME(CLIENTINP_ZOWIE);

        SET_NAME(CLIENTINP_POP_HELP_N);
        SET_NAME(CLIENTINP_POP_HELP_LATEST);

        SET_NAME(CLIENTINP_REQUEST_EVENT_LIST);
        SET_NAME(CLIENTINP_QUEUE_FOR_EVENTS);
        SET_NAME(CLIENTINP_REMOVE_FROM_QUEUE);
        SET_NAME(CLIENTINP_EVENT_READY_ACK);
        SET_NAME(CLIENTINP_EVENT_RESPONSE);
        SET_NAME(CLIENTINP_REQUEST_REJOIN_INSTANCE);

        SET_NAME(CLIENTINP_INCARNATE_CREATE);

        SET_NAME(CLIENTINP_SEND_DISPLAYALIGNMENTSTATSTOOTHERS_SETTING);

        SET_NAME(CLIENTINP_MORAL_CHOICE_RESPONSE);

        SET_NAME(CLIENTINP_NEW_FEATURE_OPEN);
        SET_NAME(CLIENTINP_UPDATE_NEW_FEATURE_VERSION);

        #undef SET_NAME

        assertmsg(last + 1 == CLIENTINP_CMD_COUNT, "A CLIENTINP cmd was missing from the end of the list.");

        for(i = 0; i < ARRAY_SIZE(cmdTimerName); i++)
        {
            if(!cmdTimerName[i])
            {
                char buffer[1000];
                sprintf(buffer, "CLIENTINP CMD NAME NOT SET: %d\n\nPlease add it in file (%s) in function %s.", i, __FILE__, __FUNCTION__);
                assertmsg(0, buffer);
                sprintf(buffer, "CLIENTINP CMD NAME NOT SET(%d)", i);
                cmdTimerName[i] = strdup(buffer);
                
                /********************************************************************************************
                * Dear Programmers,
                *
                * If you hit this assert then there is a CLIENTINP cmd (with value i) that
                * is missing its SET_NAME(..) above.  Please add it.  Thanks,
                *
                * -Martin
                *
                * p.s. To figure out which one has value "i", get the value of i and then hover
                *      your mouse over the enums in entVarUpdate.h until you find the one with that value.
                ********************************************************************************************/
            }
        }
    }

    if(cmd >= 0 && cmd < ARRAY_SIZE(cmdTimerName))
        return cmdTimerName[cmd];
    else
        return "Unknown";
}

// List of commands allowed even if the entity isn't owned
int allowed_commands[] = {CLIENTINP_SVRCMD, CLIENTINP_SVRCMD}; // twice to fool ARRAY_SIZE
int isCommandAllowedOnUnownedEnts(int cmd)
{
    int i;
    for (i=0; i < ARRAY_SIZE(allowed_commands); i++)
    {
        if (allowed_commands[i] == cmd)
        {
            return 1;
        }
    }
    return 0;
}

//Central function (possibly needs to be broken out into case: function blocks)
// returns 0 if something bad happened, causing the link to be removed
int parseClientInput( Packet *pak, ClientLink *client )
{
    Entity* e = clientGetControlledEntity(client);
    Entity* viewedEntity = clientGetViewedEntity(client);
    int     cmd_num=-1, last_cmd_num=-1;
    int        last_streampos;
    int        initial_streampos = bsGetCursorBitPosition(&pak->stream); // For debugging
    char    *s;
    int        entity_owned = e?e->owned:0;
    int        timer_started = 0;
    int        got_notahacker=0;
    int        clientPacketLogEnabled = clientPacketLogIsEnabled(0);
    void*    logPacket = NULL;
    int        controlledDbid = -1;
    int        debugCommand;

    // MS: NOTE!!!! You MUST read every piece of each command from the packet, and THEN check if there is a controlled player.
    //   Or you can jump to ErrExit if you don't care about ignoring the rest of the packet (not preferable)

    // MS: Force initialization of timer name LUT so that no one forgets to update it, even though they will just ignore the popup.

    getClientInpCmdName(0);

    if (e)
    {
        controlledDbid = e->db_id;
    }

    for(;;)
    {
        timer_started = 0;

        if (pktEnd(pak))
            break;

        last_streampos = bsGetCursorBitPosition(&pak->stream); // For debugging
        debugCommand = last_cmd_num;
        last_cmd_num = cmd_num; // For debugging

        cmd_num = pktGetBitsPack(pak,1);

        if (e && !entFromDbIdEvenSleeping(controlledDbid))
        {
            if (client->link && !client->link->disconnected)
                devassert(0); //Corrupted character
            goto ErrExit;
        }

        // If they are trying to do anything, then they aren't AFK
        if(e && e->pl && e->pl->afk && cmd_num!=CLIENTINP_WINDOW)
        {
            e->pl->afk = 0;
            e->pl->send_afk_string = 1;
        }

        if (!entity_owned && !isCommandAllowedOnUnownedEnts(cmd_num)) {
            LOG_OLD_ERR("Received unallowed command (%d) on un-owned entity", cmd_num);
            return 1;
        }

        if(client->controls.controldebug & 2)
        {
            printf("%d: got packet size %d(%d bytes) - %d\n", time(NULL), pak->id, pak->stream.size, cmd_num);
        }

        if(cmd_num >= 0 && cmd_num < CLIENTINP_CMD_COUNT)
        {
            PERFINFO_RUN(
                timer_started = 1;
                PERFINFO_AUTO_START_STATIC(getClientInpCmdName(cmd_num), &cmdTimer[cmd_num], 1);
            );
        
            logPacket = clientSubPacketLogBegin(CLIENT_PAKLOG_IN, pak, e, cmd_num);
        }

        switch(cmd_num)
        {
            case CLIENTINP_BASE_EDIT:
                baseReceiveEdit(pak, e);
            xcase CLIENTINP_WINDOW:
                {
                    int wdw, clear_entry;
                    WdwBase wdwTemp = {0}; // a place to dump the data if there's a problem.
                    WdwBase *pwdwDest = &wdwTemp;

                    clear_entry = pktGetBits(pak,1);
                    receiveWindowIdx(pak, &wdw);
                    if(e && e->pl && e->pl->winLocs &&
                        wdw >=0 && wdw < MAX_WINDOW_COUNT)
                    {
                        pwdwDest = &e->pl->winLocs[wdw];
                    }

                    if( clear_entry )
                        memset(pwdwDest, 0, sizeof(WdwBase));
                    else
                        receiveWindow(pak, pwdwDest);
                }
            break;

            case CLIENTINP_DOCK_MODE:
                {
                    U32 val = pktGetBits( pak, 32 );
                    if(e)
                    {
                        e->pl->dock_mode = val;
                    }
                }
            break;

            case CLIENTINP_CHAT_DIVIDER:
                {
                    F32 val = pktGetF32( pak );
                    if(e)
                    {
                        e->pl->chat_divider = val;
                    }
                }
            break;

            case CLIENTINP_INSPTRAY_MODE:
                {
                    U32 val = pktGetBits( pak, 32 );
                    if(e)
                    {
                        e->pl->inspiration_mode = val;
                    }
                }
                break;

            case CLIENTINP_TRAY_MODE:
                {
                    U32 val = pktGetBits( pak, NUM_CURTRAYTYPE_BITS );
                    if (e && e->pl)
                    {
                        // must change this code if change this
                        assert(NUM_CURTRAYTYPE_BITS == 2);
                        e->pl->tray->mode = val&0x1;
                        e->pl->tray->mode_alt2 = (val&0x2)>>1; 
                    }
                }
                break;


            case CLIENTINP_SENDING_CHARACTER:
                {
                    if(e){
                        if(e->pchar && e->pchar->porigin)
                        {
                            LOG_OLD_ERR( "Received CLIENTINP_SENDING_CHARACTER on a already-created character (%s, authname:%s).", e->name, e->auth_name);
                            goto ErrExit;
                        }
                        else
                        {
                            if (!receiveCharacterFromClient( pak, e )) {
                                // Got bad data, this entity is invalid
                                // send fatal error to client and disconnect
                                // Do not do this here: we kick them one layer up!  svrForceLogoutLink(client->link, "Bad data", KICKTYPE_KICK);
                                if(timer_started)
                                {
                                    PERFINFO_AUTO_STOP();
                                }
                                return 0;
                            } else {

                                if(!client->ready == CLIENTSTATE_IN_GAME) //JE: WTH?  a boolean value will never equal 3, what's this supposed to do?
                                    printf("Client is not ready\n");
                                e->send_costume = TRUE;
                                e->updated_powerCust = TRUE;
                                e->lastAutoCommandRunTime = e->pl->dateCreated = dbSecondsSince2000();
                                character_ActivateAllAutoPowers(e->pchar);
                                StoryArcNewPlayer(e);
                                setInitialPosition(e,true);
                                svrSendEntListToDb(&e,1); // make sure this gets sent when new player is all set up
                            }
                        }
                    } else {
                        // We didn't eat the data, just return, don't loop
                        goto ErrExit;
                    }
                }
            break;

            case CLIENTINP_SVRCMD: //one of these kids is doing his own thing...
                s = pktGetString(pak);
                serverParseClient(s,client);
            break;

            case CLIENTINP_RECEIVED_CHARACTER: // Client has received the full update and has created himself, he's good to go.

                if(e && e->pl){
                    clientReceivedCharacter(client);
                    shardCommLogin(client->entity,0);
                }
            break;
            
            case CLIENTINP_ACK_FULL_UPDATE:{
                U32 fullUpdateID = pktGetBitsPack(pak, 1);
                
                MAX1(client->ackedFullUpdateID, fullUpdateID);
            }
            break;

            case CLIENTINP_TOUCH_NPC:
                {
                    int     id;
                    Entity* npc;

                    id = pktGetBitsPack( pak, PKT_BITS_TO_REP_SVR_IDX );

                    if(e)
                    {
                        npc = entFromId( id );

                        if (!npc || !(entity_state[id] & ENTITY_IN_USE))
                            break;
                        switch(ENTTYPE_BY_ID(npc->owner))
                        {
                        case ENTTYPE_NPC:
                            {
                                char *say = NULL;
                                char *reward = NULL;

                                if (ScriptNpcClick(e, npc))
                                {
                                    // script handled it
                                }
                                else if (npc->persistentNPCInfo)
                                {
                                    PNPCTouched(client, npc);
                                }
                                else if (npc->missionObjective)
                                {
                                    MissionItemInteract(npc, e);
                                }
                                else if (OnClickCondition_Check(npc, e, &say, &reward)) {
                                    if (say != NULL) {
                                        if( !npc->IAmInEmoteChatMode )
                                        {
                                            if( e )
                                                npc->audience = erGetRef(e);

                                            saUtilEntSays(npc, ENTTYPE(npc) == ENTTYPE_NPC, saUtilScriptLocalize( say ));
                                        }                                            
                                        free(say);
                                    }
                                    if (reward != NULL) {
                                        if (e->pchar != NULL) 
                                            rewardFindDefAndApplyToEnt(e, (const char**)eaFromPointerUnsafe(reward), VG_NONE, character_GetCombatLevelExtern(e->pchar), false, REWARDSOURCE_NPC_ON_CLICK, NULL);
                                        free(reward);
                                    }
                                }
                                //MW I probably need to put this somewhere different, or just
                                //rationalize the interaction with those other functions
                                //For now, it seems more important than npcInteract, but less than PNPC Touched
                                else if( npc->sayOnClick )
                                {
                                    if( !npc->IAmInEmoteChatMode )
                                    {
                                        if( e )
                                            npc->audience = erGetRef(e);
                                        saUtilEntSays(npc, ENTTYPE(npc) == ENTTYPE_NPC, saUtilScriptLocalize( npc->sayOnClick ));
                                    }
                                }
                                else
                                {
                                    // Make sure that this NPC isn't busy
                                    // doing something else.
                                    if(aiHasPriorityList(npc))
                                    {
                                        npcInteract(npc, client->entity);
                                    }
                                }
                                if( npc->rewardOnClick )
                                {
                                    if (e->pchar != NULL) 
                                        rewardFindDefAndApplyToEnt(e, (const char**)eaFromPointerUnsafe(npc->rewardOnClick), VG_NONE, character_GetCombatLevelExtern(e->pchar), false, REWARDSOURCE_NPC_ON_CLICK, NULL);
                                }
                                break;
                            }
                        case ENTTYPE_MISSION_ITEM:
                            if (npc->missionObjective)
                                MissionItemInteract(npc, e);
                        
                            break;
                        case ENTTYPE_CRITTER:
                            if (npc->persistentNPCInfo)
                            {
                                if (npc->pchar && client->entity->pchar && !character_TargetIsFoe(client->entity->pchar, npc->pchar))
                                    PNPCTouched(client, npc);
                            } else {
                                if (g_base.rooms && e) {
                                    BaseRoom *pRoom = baseRoomLocFromPos(&g_base, ENTPOS(npc), NULL);
                                    RoomDetail *det;
                                    if (!pRoom)
                                        break;
                                    det = roomGetDetailFromWorldPos(pRoom,ENTPOS(npc));
                                    if (!det)
                                        break;
                                    baseActivateDetail(e,det->id,"");
                                }
                            }
                        }
                    }
                }
                break;

            case CLIENTINP_RECEIVE_TRAYS:
                {
                    receiveTray(  pak, e );
                }
            break;

            case CLIENTINP_ENTER_DOOR:
                {
                    Vec3 pos;
                    char selected_map_id[100];
                    int is_volume;

                    is_volume = pktGetBits(pak,1);

                    if(!is_volume)
                    {
                        pos[0] = pktGetF32(pak);
                        pos[1] = pktGetF32(pak);
                        pos[2] = pktGetF32(pak);
                    }

                    strncpyt(selected_map_id, pktGetString(pak), 100);

                    if(e)
                    {
                        DefTracker* volume = e->volumeList.materialVolume->volumeTracker;
                        int door_volume = is_volume && volume && volume->def && volume->def->door_volume;
                        StashTable props = ( door_volume && volume && volume->def && volume->def->properties ) ? volume->def->properties : NULL;

                        if(    door_volume ||
                            !is_volume && distance3Squared(pos, ENTPOS(e)) <= SQR(SERVER_INTERACT_DISTANCE))
                        {
                            enterDoor( e, door_volume ? volume->mat[3] : pos, selected_map_id, door_volume, props );
                        }
                        else
                            sendDoorMsg( e, DOOR_MSG_FAIL, "" );

                    }
                }
            break;
            case CLIENTINP_ENTER_BASE_CHOICE:
            {
                int idSgrp;
                char selected_map_id[100];
                
                idSgrp = pktGetBitsAuto(pak);
                Strncpyt(selected_map_id, pktGetString(pak));
                
                if(e && idSgrp > 0)
                {
                    char errmsg[2000] = "";
                    
                    SGBaseEnterFromSgid(e, "PlayerSpawn", idSgrp, errmsg);

                    if (errmsg[0])
                        chatSendToPlayer(e->db_id, errmsg,INFO_SVR_COM,0);
                }
            }
                break;
            case CLIENTINP_REQUEST_GURNEY_XFER:
                {
                    bool bBase = pktGetBits(pak,1);
                    if(e)
                    {
                        if(OnArenaMap())
                        {
                            //ArenaMapHandleEntityRespawn(e, 1);
                        }
                        else if (g_isBaseRaid && bBase)
                        {
                            sgRaidDoGurneyXfer(e);
                        }
                        else
                        {
                            if (bBase && sgroup_isPlayerDark(e))
                            {
                                // Client lied to us?  They should not have been able to request a base gurney rez if they're dark.
                                // But we "never trust the client", do we.
                                bBase = 0;
                            }
                            dbBeginGurneyXfer( e, bBase);
                        }
                    }
                }
            break;

            case CLIENTINP_REQUEST_WAYPOINT:
                {
                    Vec3 pos;
                    int uid;

                    pos[0] = pktGetF32(pak);
                    pos[1] = pktGetF32(pak);
                    pos[2] = pktGetF32(pak);
                    uid = pktGetBitsPack(pak, 1);

                    if(e && FINITE(pos[0]) && FINITE(pos[1]) && FINITE(pos[2]))
                    {
                        Beacon* sourceBeacon = entGetNearestBeacon(e);

                        if(    sourceBeacon &&
                            sourceBeacon->block &&
                            sourceBeacon->block->galaxy &&
                            sourceBeacon->block->galaxy->cluster)
                        {
                            BeaconBlock* sourceCluster = sourceBeacon->block->galaxy->cluster;
                            Beacon* targetBeacon;
                            
                            PERFINFO_AUTO_START("rw.beaconGetNearestBeacon", 1);
                                targetBeacon = beaconGetNearestBeacon(pos);
                            PERFINFO_AUTO_STOP();

                            if(    targetBeacon &&
                                targetBeacon->block &&
                                targetBeacon->block->galaxy &&
                                targetBeacon->block->galaxy->cluster)
                            {
                                BeaconBlock* targetCluster = targetBeacon->block->galaxy->cluster;

                                // Can potentially fail, but still send back pos so the user sees a waypoint
                                beaconGetNextClusterWaypoint(sourceCluster, targetCluster, pos);

                                START_PACKET(pak, e, SERVER_UPDATE_WAYPOINT);
                                pktSendBitsPack(pak, 1, uid);
                                pktSendF32(pak, pos[0]);
                                pktSendF32(pak, pos[1]);
                                pktSendF32(pak, pos[2]);
                                END_PACKET
                            }
                        }
                    }
                }
            break;

            case CLIENTINP_DOORANIM_DONE:
                if (e)
                    DoorAnimClientDone( e );
            break;

            case CLIENTINP_KEYBIND:
            {
                if(e)
                {
                    keybind_receive( pak, e );
                } else {
                    goto ErrExit;
                }

            } break;

            case CLIENTINP_KEYBIND_RESET:
            {
                if(e)
                {
                    keybind_receiveReset( e );
                } else {
                    goto ErrExit;
                }

            } break;

            case CLIENTINP_KEYBIND_CLEAR:
            {
                if(e)
                {
                    keybind_receiveClear( pak, e );
                } else {
                    goto ErrExit;
                }

            } break;

            case CLIENTINP_KEYPROFILE:
            {
                if(e)
                {
                    keyprofile_receive( pak, e );
                } else {
                    goto ErrExit;
                }

            } break;

            case CLIENTINP_THIRD:
            {
                int val = pktGetBits( pak, 1 );
                if(e && e->pl )
                {
                    EntPlayer * pl = e->pl;
                    pl->first = val;
                }
            }break;

            case CLIENTINP_DIVIDERS:
            {
                if(e && e->pl )
                {
                    EntPlayer * pl = e->pl;
                    pl->divSuperName    = pktGetBitsPack( pak, 9 );
                    pl->divSuperMap        = pktGetBitsPack( pak, 9 );
                    pl->divSuperTitle    = pktGetBitsPack( pak, 9 );
                    pl->divEmailFrom    = pktGetBitsPack( pak, 9 );
                    pl->divEmailSubject = pktGetBitsPack( pak, 9 );
                    pl->divFriendName    = pktGetBitsPack( pak, 9 );
                    pl->divLfgName        = pktGetBitsPack( pak, 9 );
                    pl->divLfgMap        = pktGetBitsPack( pak, 9 );
                }
                else
                    goto ErrExit;

            }break;

            case CLIENTINP_OPTIONS:
            {
                if(e && e->pl )
                {
                    int contactSort = 0;
                    EntPlayer * pl = e->pl;

                    pl->options_saved = 1;

                    pl->mouse_speed     = pktGetF32( pak );
                    pl->turn_speed     = pktGetF32( pak );
                    pl->mouse_invert = pktGetBits( pak, 1 );

                    pl->fading_chat = pktGetBits( pak, 1 );
                    pl->fading_chat1 = pktGetBits( pak, 1 );
                    pl->fading_chat2 = pktGetBits( pak, 1 );
                    pl->fading_chat3 = pktGetBits( pak, 1 );
                    pl->fading_chat4 = pktGetBits( pak, 1 );
                    pl->fading_nav    = pktGetBits( pak, 1 );

                    pl->showToolTips                = pktGetBits( pak, 1 );
                    pl->allowProfanity                = pktGetBits( pak, 1 );
                    pl->showBalloon                     = pktGetBits( pak, 1 );
                    pl->declineGifts                = pktGetBits( pak, 1 );
                    pl->declineTeamGifts            = pktGetBits( pak, 1 );
                    pl->promptTeamTeleport            = pktGetBits( pak, 1 );
                    pl->showPets                    = pktGetBits( pak, 1 );
                    pl->showSalvage                    = pktGetBits( pak, 1 );
                    pl->webHideBasics                = pktGetBits( pak, 1 );
                    pl->webHideStats                = pktGetBits( pak, 1 );
                    pl->webHidePowers                = pktGetBits( pak, 1 );
                    pl->webHideBadges                = pktGetBits( pak, 1 );
                    pl->webHideFriends                = pktGetBits( pak, 1 );
                    pl->windowFade                    = pktGetBits( pak, 1 );
                    pl->logChat                        = pktGetBits( pak, 1 );
                    pl->sgHideHeader                = pktGetBits( pak, 1 );
                    pl->sgHideButtons                = pktGetBits( pak, 1 );
                    pl->clicktomove                    = pktGetBits( pak, 1 );
                    pl->disableDrag                    = pktGetBits( pak, 1 );
                    pl->showPetBuffs                = pktGetBits( pak, 1 );
                    pl->preventPetIconDrag            = pktGetBits( pak, 1 );
                    pl->showPetControls                = pktGetBits( pak, 1 );
                    pl->advancedPetControls            = pktGetBits( pak, 1 );
                    pl->disablePetSay                = pktGetBits( pak, 1 );
                    pl->enableTeamPetSay            = pktGetBits( pak, 1 );
                    pl->teamCompleteOption            = pktGetBits( pak, 2 );
                    pl->disablePetNames                = pktGetBits( pak, 1 );
                    pl->hidePlacePrompt                = pktGetBits( pak, 1 );
                    pl->hideDeletePrompt            = pktGetBits( pak, 1 );
                    pl->hideDeleteSalvagePrompt        = pktGetBits( pak, 1 );
                    pl->hideDeleteRecipePrompt        = pktGetBits( pak, 1 );
                    pl->hideInspirationFull            = pktGetBits( pak, 1 );
                    pl->hideSalvageFull                = pktGetBits( pak, 1 );
                    pl->hideRecipeFull                = pktGetBits( pak, 1 );
                    pl->hideEnhancementFull            = pktGetBits( pak, 1 );
                    pl->showEnemyTells                = pktGetBits( pak, 1 );
                    pl->showEnemyBroadcast            = pktGetBits( pak, 1 );
                    pl->hideEnemyLocal                = pktGetBits( pak, 1 );
                    pl->hideCoopPrompt                = pktGetBits( pak, 1 );
                    pl->staticColorsPerName            = pktGetBits( pak, 1 );

                    contactSort                        = pktGetBits( pak, 4 );
                    switch (contactSort) 
                    {
                        default:
                        case 0:
                            pl->contactSortByName = 0;
                            pl->contactSortByZone = 0;
                            pl->contactSortByRelationship = 0;
                            pl->contactSortByActive = 0;
                            break;
                        case 1:
                            pl->contactSortByName = 1;
                            pl->contactSortByZone = 0;
                            pl->contactSortByRelationship = 0;
                            pl->contactSortByActive = 0;
                            break;
                        case 2:
                            pl->contactSortByName = 0;
                            pl->contactSortByZone = 1;
                            pl->contactSortByRelationship = 0;
                            pl->contactSortByActive = 0;
                            break;
                        case 3:
                            pl->contactSortByName = 0;
                            pl->contactSortByZone = 0;
                            pl->contactSortByRelationship = 1;
                            pl->contactSortByActive = 0;
                            break;
                        case 4:
                            pl->contactSortByName = 0;
                            pl->contactSortByZone = 0;
                            pl->contactSortByRelationship = 0;
                            pl->contactSortByActive = 1;
                            break;
                    }
                    pl->recipeHideUnOwned            = pktGetBits( pak, 1 );
                    pl->recipeHideMissingParts        = pktGetBits( pak, 1 );
                    pl->recipeHideUnOwnedBench        = pktGetBits( pak, 1 );
                    pl->recipeHideMissingPartsBench    = pktGetBits( pak, 1 );

                    pl->declineSuperGroupInvite        = pktGetBits( pak, 1 );
                    pl->declineTradeInvite            = pktGetBits( pak, 1 );

                    pl->freeCamera          = pktGetBits( pak, 1);
                    pl->tooltipDelay        = pktGetF32(pak);

                    pl->eShowArchetype         = pktGetBits( pak, 3 );
                    pl->eShowSupergroup         = pktGetBits( pak, 3 );
                    pl->eShowPlayerName         = pktGetBits( pak, 3 );
                    pl->eShowPlayerBars         = pktGetBits( pak, 3 );
                    pl->eShowVillainName     = pktGetBits( pak, 3 );
                    pl->eShowVillainBars     = pktGetBits( pak, 3 );
                    pl->eShowPlayerReticles     = pktGetBits( pak, 3 );
                    pl->eShowVillainReticles = pktGetBits( pak, 3 );
                    pl->eShowAssistReticles     = pktGetBits( pak, 3 );
                    pl->eShowOwnerName         = pktGetBits( pak, 3 );
                    pl->mousePitchOption     = pktGetBits( pak, 3 );
                    
                    pl->mapOptions             = pktGetBits( pak, 32 );
                    pl->buffSettings         = pktGetBits( pak, 32 );
                    pl->chatBubbleTextColor  = pktGetBits( pak, 32 );
                    pl->chatBubbleBackColor  = pktGetBits( pak, 32 );
                    pl->chat_font_size         = pktGetBits( pak, 8 );
        
                    pl->reverseMouseButtons = pktGetBits( pak, 1 );
                    pl->disableCameraShake = pktGetBits( pak, 1 );
                    pl->disableMouseScroll = pktGetBits( pak, 1 );
                    pl->logPrivateMessages = pktGetBits( pak, 1 );
                    pl->eShowPlayerRating = pktGetBits( pak, 3);
                    pl->disableLoadingTips = pktGetBits( pak, 1 );
                    pl->mouseScrollSpeed = pktGetF32( pak );
                    pl->enableJoystick = pktGetBits( pak, 1 );
                    pl->fading_tray = pktGetBits(pak, 1);
                    pl->ArchitectNav = pktGetBits(pak,1);
                    pl->ArchitectTips = pktGetBits(pak,1);
                    pl->ArchitectAutoSave = pktGetBits(pak,1);
                    pl->noXP = pktGetBits(pak,1);
                    pl->ArchitectBlockComments = pktGetBits(pak,1);
                    pl->autoAcceptTeamAbove = pktGetBits(pak,6);
                    pl->autoAcceptTeamBelow = pktGetBits(pak,6);
                    pl->disableEmail = pktGetBits(pak,1);
                    pl->friendSgEmailOnly = pktGetBits(pak,1);
                    pl->noXPExemplar = pktGetBits(pak,1);
                    pl->popHelpDisable = pktGetBits(pak,1);
                    pl->hideFeePrompt = pktGetBits(pak,1);
                    pl->hideUsefulSalvageWarning = pktGetBits(pak,1);
                    pl->forcedPopHelpDisable = pktGetBits(pak, 1);
                    pl->gmailFriendOnly = pktGetBits(pak, 1);
                    pl->useOldTeamUI = pktGetBits(pak, 1);
                    pl->hideUnclaimableCert = pktGetBits(pak, 1);
                    pl->blinkCertifications = pktGetBits(pak, 1);
                    pl->voucherSingleCharacterPrompt = pktGetBits(pak, 1);
                    pl->newCertificationPrompt = pktGetBits(pak, 1);
                    
                    pl->hideUnslotPrompt = pktGetBits(pak, 1);
                    pl->mapOptionRevision = pktGetBits(pak, 16);
                    pl->mapOptions2 = pktGetBits(pak, 32);
                    pl->hideOpenSalvageWarning = pktGetBits(pak,1);
                    pl->hideLoyaltyTreeAccessButton = pktGetBits(pak,1);
                    pl->hideStoreAccessButton = pktGetBits(pak,1);
                    pl->autoFlipSuperPackCards = pktGetBits(pak,1);
                    pl->hideConvertConfirmPrompt = pktGetBits(pak,1);
                    pl->hideStorePiecesState = pktGetBits(pak,3);
                    pl->cursorScale = pktGetF32(pak);
                    pl->showTimer = pktGetBits(pak, 1);

                    if(pl->gmailFriendOnly)
                        shardCommSendf(e,true,"GMailFriendOnlySet");
                    else
                        shardCommSendf(e,true,"GMailFriendOnlyUnset");
                }
                else 
                    goto ErrExit;

            }break;

            case CLIENTINP_TITLES:
            {
                int showThe = pktGetBitsPack( pak, 5 );
                int commonIdx = pktGetBitsPack( pak, 6 );
                int originIdx = pktGetBitsPack( pak, 5 );
                int color1 = pktGetBitsAuto(pak);
                int color2 = pktGetBitsAuto(pak);

                if (e && e->pl &&
                    (e->pchar->iLevel >= UNLIMITED_TITLE_LEVEL ||
                     e->pchar->iLevel >= COMMON_TITLE_LEVEL ) )
                {
                    title_set( e, showThe, commonIdx, originIdx, color1, color2 );
                    e->send_costume = 1;
                }

            }break;

            case CLIENTINP_PLAYERDESC:
            {
                if(e)
                {
                    receiveEntStrings( pak, e->strings);
                } else {
                    goto ErrExit;
                }

            }break;

            case CLIENTINP_CONTACT_DIALOG_RESPONSE:
                if (e)
                {
                    ContactDialogResponsePkt(pak, e);
                } else {
                    goto ErrExit;
                }

            break;

            case CLIENTINP_CONTACT_CELL_CALL:
                if (e)
                {
                    ContactCellCall(e, pktGetBitsPack(pak, 1));
                } else {
                    goto ErrExit;
                }

            break;
            case CLIENTINP_CONTACT_FLASHBACK_LIST_REQUEST:
                if (e)
                {
                    // send to the client a list of all the valid flashback storyarcs
                    StoryArcFlashbackList(e);
                } else {
                    goto ErrExit;
                }
            break;
            case CLIENTINP_CONTACT_FLASHBACK_DETAIL_REQUEST:
                if (e)
                {
                    // send to the client detail for the specified arc
                    StoryArcFlashbackDetails(e, pktGetString( pak ));
                } else {
                    goto ErrExit;
                }
                break;
            case CLIENTINP_CONTACT_FLASHBACK_CHECK_ELIGIBILITY_REQUEST:
                if (e)
                {
                    StoryArcFlashbackCheckEligibility(e, pktGetString(pak));
                } else {
                    goto ErrExit;
                }
                break;
            case CLIENTINP_CONTACT_FLASHBACK_ACCEPT_REQUEST:
                if (e)
                {
                    char *pId = NULL;
                    int timeLimits;
                    int limitedLives;
                    int powerLimits;
                    int debuff;
                    int buff;
                    int noEnhancements;
                    int noInspirations;

                    // Start the flashback
                    pId = strdup(pktGetString( pak ));
                    timeLimits = pktGetBitsPack(pak, 3);
                    limitedLives = pktGetBitsPack(pak, 3);
                    powerLimits = pktGetBitsPack(pak, 3);
                    debuff = pktGetBitsPack(pak, 1);
                    buff = pktGetBitsPack(pak, 1);
                    noEnhancements = pktGetBitsPack(pak, 1);
                    noInspirations = pktGetBitsPack(pak, 1);

                    TaskforceAccept(e, timeLimits, limitedLives, powerLimits,
                        debuff, buff, noEnhancements, noInspirations, pId);

                    SAFE_FREE(pId);
                } 
                else 
                    goto ErrExit;
                break;
            case CLIENTINP_CONTACT_TASKFORCE_ACCEPT_REQUEST:
                if (e)
                {
                    int timeLimits;
                    int limitedLives;
                    int powerLimits;
                    int debuff;
                    int buff;
                    int noEnhancements;
                    int noInspirations;

                    // Start the flashback
                    timeLimits = pktGetBitsPack(pak, 3);
                    limitedLives = pktGetBitsPack(pak, 3);
                    powerLimits = pktGetBitsPack(pak, 3);
                    debuff = pktGetBitsPack(pak, 1);
                    buff = pktGetBitsPack(pak, 1);
                    noEnhancements = pktGetBitsPack(pak, 1);
                    noInspirations = pktGetBitsPack(pak, 1);

                    TaskforceAccept(e, timeLimits, limitedLives, powerLimits,
                                        debuff, buff, noEnhancements, noInspirations, 0);

                } 
                else
                    goto ErrExit;
                break;
            case CLIENTINP_DEAD_NOGURNEY_RESPONSE:
                if (e && e->access_level > ACCESS_USER) 
                    completeGurneyXferWork(e);    // respawn in place if allowed
                else
                    dbBeginGurneyXfer(e, false); // otherwise do a regular respawn
            break;

            case CLIENTINP_ACTIVATE_POWER:
                if(e)
                {
                    if(!entity_ActivatePowerReceive(e, pak))
                    {
                        // The packet was malformed in some way.
                        // TODO: Mark player as naughty?
                        // This can happen validly (when a user trys to brawl a dead guy, I think)
                        //printf("Entity %s send a bad Activate_Power\n", e->name);
                    }
                } 
                else 
                    goto ErrExit;
            break;

            case CLIENTINP_ACTIVATE_POWER_AT_LOCATION:
                if(e)
                {
                    if(!entity_ActivatePowerAtLocationReceive(e, pak))
                    {
                        // The packet was malformed in some way.
                        // TODO: Mark player as naughty?
                        printf("Entity %s send a bad Activated_Power_At_Location\n", e->name);
                    }
                } else {
                    goto ErrExit;
                }

            break;

            case CLIENTINP_STANCE:
                if(e)
                {
                    if(!entity_StanceReceive(e, pak))
                    {
                        // The packet was malformed in some way.
                        // TODO: Mark player as naughty?
                        printf("Entity %s send a bad Stance\n", e->name);
                    }
                } else {
                    goto ErrExit;
                }

            break;

            case CLIENTINP_ACTIVATE_INSPIRATION:
                if(e)
                {
                    if(!entity_ActivateInspirationReceive(e, pak))
                    {
                        // The packet was malformed in some way.
                        // TODO: Mark player as naughty?
                        printf("Entity %s send a bad Activated_Inspiration\n", e->name);
                    }
                } else {
                    goto ErrExit;
                }

            break;

            case CLIENTINP_SET_DEFAULT_POWER:
                if(e)
                {
                    if(!entity_SetDefaultPowerReceive(e, pak))
                    {
                        // The packet was malformed in some way.
                        // TODO: Mark player as naughty?
                        printf("Entity %s send a bad Set_Default_Power\n", e->name);
                    }
                } else {
                    goto ErrExit;
                }

            break;

            case CLIENTINP_CLEAR_DEFAULT_POWER:
                if(e)
                {
                    if(e->pchar)
                    {
                        e->pchar->ppowDefault = NULL;
                    }
                    else
                    {
                        // TODO: Mark player as naughty?
                        printf("Entity %s send a bad Clear_Default_Power\n", e->name);
                    }
                } else {
                    goto ErrExit;
                }

            break;

            case CLIENTINP_CANCEL_QUEUED_POWER:
                if(e && e->pchar)
                {
                    character_KillQueuedPower(e->pchar);
                    character_KillRetryPower(e->pchar);
                } else {
                    goto ErrExit;
                }

            break;

            case CLIENTINP_CANCEL_WINDUP_POWER:
                if(e && e->pchar)
                {
                    character_InterruptPower(e->pchar, NULL, 0, false);
                } else {
                    goto ErrExit;
                }

            break;

            case CLIENTINP_MOVE_INSPIRATION:
                if(e)
                {
                    if(!entity_MoveInspirationReceive(e, pak))
                    {
                        // The packet was malformed in some way.
                        // TODO: Mark player as naughty?
                        printf("Entity %s send a bad Move_Inspiration\n", e->name);
                    }
                } else {
                    goto ErrExit;
                }

                break;

            case CLIENTINP_SET_CHAT_CHANNEL:
                {
                    int val = pktGetBitsPack( pak, 1);
                    int userChannel = pktGetBitsPack( pak, 1);
                    if(e)
                        setChatChannel( e, val, userChannel );
                }

            break;

            case CLIENTINP_SEND_CHAT_SETTINGS:
                {
                    if(e)
                        receiveChatSettings(pak, e);
                    else {
                        goto ErrExit;
                    }
                }
                break;

            case CLIENTINP_SEND_DISPLAYALIGNMENTSTATSTOOTHERS_SETTING:
                {
                    if (e && e->pl)
                    {
                        e->pl->displayAlignmentStatsToOthers = pktGetBitsAuto(pak);
                    }
                    else
                    {
                        goto ErrExit;
                    }
                }
                break;

            case CLIENTINP_SEND_SELECTED_CHAT_TABS:
                {
                    if(e)
                        receiveSelectedChatTabs(pak, e);
                    else {
                        goto ErrExit;
                    }
                }
                break;

            case CLIENTINP_COMBINE_BOOSTS:
                if(e && e->pchar)
                {
                    if(!power_ReceiveCombineBoosts(pak, e->pchar))
                    {
                        // TODO: Mark player as naughty?
                        printf("Entity %s send a bad Combine_Boosts\n", e->name);
                    }
                } else {
                    goto ErrExit;
                }

            break;

            case CLIENTINP_BOOSTER_BOOST:
                if(e && e->pchar)
                {
                    if(!power_ReceiveBoosterBoost(pak, e->pchar))
                    {
                        // TODO: Mark player as naughty?
                        printf("Entity %s send a bad CLIENTINP_BOOSTER_BOOST\n", e->name);
                    }
                } else {
                    goto ErrExit;
                }
                break;
            case CLIENTINP_CATALYST_BOOST:

                if(e && e->pchar)
                {
                    if(!power_ReceiveCatalystBoost(pak, e->pchar))
                    {
                        // TODO: Mark player as naughty?
                        printf("Entity %s send a bad CLIENTINP_CATALYST_BOOST\n", e->name);
                    }
                } else {
                    goto ErrExit;
                }
                break;
            case CLIENTINP_SET_INVENTORY_BOOST:
                if(e && e->pchar)
                {
                    if(!character_ReceiveSetBoost(pak, e->pchar))
                    {
                        // TODO: Mark player as naughty?
                        printf("Entity %s send a bad Set_Inventory_Boost\n", e->name);
                    }
                } else {
                    goto ErrExit;
                }

                break;

            case CLIENTINP_REMOVE_INVENTORY_BOOST:
                if(e && e->pchar)
                {
                    if(!character_ReceiveRemoveBoost(pak, e->pchar))
                    {
                        // TODO: Mark player as naughty?
                        printf("Entity %s send a bad Remove_Inventory_Boost\n", e->name);
                    }
                } else {
                    goto ErrExit;
                }

                break;

            case CLIENTINP_REMOVE_POWER_BOOST:
                if(e && e->pchar)
                {
                    if(!character_ReceiveRemoveBoostFromPower(pak, e->pchar))
                    {
                        // TODO: Mark player as naughty?
                        printf("Entity %s send a bad Remove_Power_Boost\n", e->name);
                    }
                } else {
                    goto ErrExit;
                }

                break;

            case CLIENTINP_UNSLOT_POWER_BOOST:
                if(e && e->pchar)
                {
                    if(!character_ReceiveUnslotBoostFromPower(pak, e->pchar))
                    {
                        // TODO: Mark player as naughty?
                        printf("Entity %s send a bad Unslot_Power_Boost\n", e->name);
                    }
                } else {
                    goto ErrExit;
                }

                break;

            case CLIENTINP_SET_POWER_BOOST:
                if(e && e->pchar)
                {
                    if(!power_ReceiveSetBoost(pak, e->pchar))
                    {
                        // TODO: Mark player as naughty?
                        printf("Entity %s send a bad Set_Power_Boost\n", e->name);
                    }
                } else {
                    goto ErrExit;
                }

                break;

            case CLIENTINP_LEVEL_BUY_POWER:
                if(e && e->pchar)
                {
                    if(!character_ReceiveLevelAndBuyPower(pak, e->pchar))
                    {
                        // TODO: Mark player as naughty?
                        printf("Entity %s send a bad Buy_Power\n", e->name);
                    }
                    else
                    {
                        //check if any costumes need to change after adding this power
                        costumeValidateCostumes(e);
                    }
                } else {
                    goto ErrExit;
                }

            break;

            case CLIENTINP_LEVEL_ASSIGN_BOOST_SLOT:
                if(e && e->pchar)
                {
                    if(!power_ReceiveLevelAndAssignBoostSlot(pak, e->pchar))
                    {
                        // TODO: Mark player as naughty?
                        printf("Entity %s send a bad Add_Boost_Slot\n", e->name);
                    }
                } else {
                    goto ErrExit;
                }

                break;

            case CLIENTINP_LEVEL:
                if(e && e->pchar)
                {
                    if(!character_ReceiveLevel(pak, e->pchar))
                    {
                        // TODO: Mark player as naughty?
                        printf("Entity %s send a bad Level\n", e->name);
                    }
                } else {
                    goto ErrExit;
                }

            break;

            case CLIENTINP_LEGACYTEAMUI_NOTE:
                if (e && e->pl)
                {
                    e->pl->chat_settings.options |= CSFlags_AddedLegacyUINote;
                }
            break;

            case CLIENTINP_READ_PLAQUE:
                if(e && e->pchar)
                {
                    if(!plaque_ReceiveReadPlaque(pak, e))
                    {
                        // TODO: Mark player as naughty?
                        printf("Entity %s send a bad Read_Plaque\n", e->name);
                    }
                } else {
                    goto ErrExit;
                }
            break;

            case CLIENTINP_ZOWIE:
                if(e && e->pchar)
                {
                    if(!zowie_ReceiveReadZowie(pak, e))
                    {
                        printf("Entity %s sent a bad zowie\n", e->name);
                    }
                } else {
                    goto ErrExit;
                }
            break;

            case CLIENTINP_POP_HELP_N:
                {
                    int n = pktGetBitsPack( pak, 30 );
                    int new_state = pktGetBitsPack( pak, 2 );

                    if(e && e->pl)
                        setPopHelpState(e, n, new_state);
                }
            break;

            case CLIENTINP_POP_HELP_LATEST:
                {
                    int i;
                    int val;

                    for(i=0; i<POP_HELP_LATEST_SIZE; i++)
                    {
                        val = pktGetBitsPack( pak, 32 );
                        if(e && e->pl)
                            e->pl->popHelpLatest[i] =  val;
                    }
                }
            break;

            case CLIENTINP_REQUEST_EVENT_LIST:
                if (e && e->pchar && e->pl)
                {
                    turnstileMapserver_handleRequestEventList(e);
                }
                else
                {
                    goto ErrExit;
                }
            break;

            case CLIENTINP_QUEUE_FOR_EVENTS:
                if (e && e->pchar && e->pl)
                {
                    turnstileMapserver_handleQueueForEvents(e, pak);
                }
                else
                {
                    goto ErrExit;
                }
            break;

            case CLIENTINP_REMOVE_FROM_QUEUE:
                if (e && e->pchar && e->pl)
                {
                    turnstileMapserver_handleRemoveFromQueue(e);
                }
                else
                {
                    goto ErrExit;
                }
            break;

            case CLIENTINP_EVENT_READY_ACK:
                if (e && e->pchar && e->pl)
                {
                    turnstileMapserver_handleEventReadyAck(e, pak);
                }
                else
                {
                    goto ErrExit;
                }
            break;

            case CLIENTINP_EVENT_RESPONSE:
                if (e && e->pchar && e->pl)
                {
                    turnstileMapserver_handleEventResponse(e, pak);
                }
                else
                {
                    goto ErrExit;
                }
            break;

            case CLIENTINP_REQUEST_REJOIN_INSTANCE:
                if (e && e->pl)
                {
                    turnstileMapserver_rejoinInstance(pak, e);
                }
                else
                {
                    goto ErrExit;
                }
            break;
            //------------------------------------------------------------------------------------------
            // Team Messages ///////////////////////////////////////////////////////////////////////////
            //------------------------------------------------------------------------------------------
            // clientside functions live in uiNet.c, server functions live in team.c

            case CLIENTINP_TASK_REACHED_VISIT_LOCATION:
            {
                if (e) {
                    LocationTaskVisitReceive(e, pak);
                } else {
                    goto ErrExit;
                }

            }
            break;

            case CLIENTINP_SUPERGROUP_TOGGLE:
            {
                if (e)
                    sgroup_ToggleMode(e);
            }
            break;

            case CLIENTINP_SUPERGROUP_LIST_REQUEST:
            {
                int sort_by_prestige = pktGetBits(pak,1);
                char name[SG_NAME_LEN];
                
                strncpyt(name, pktGetString(pak), SG_NAME_LEN);

                if (e)
                    sgroup_sendRegistrationList(e,sort_by_prestige,name);
            }
            break;

            case CLIENTINP_REQ_SGRAIDLIST:
            {
                if (e)
                    RaidRequestList(e);
            }
            break;

            // raid messages
            case CLIENTINP_BASERAID_PARTICIPATE:
            {
                U32 raidid = pktGetBitsAuto(pak);
                int participate = pktGetBits(pak, 1);
                if (e)
                    RaidRequestParticipate(e, raidid, participate);
            }
            break;

            case CLIENTINP_RAID_SCHEDULE:
            {
                U32 sgid = pktGetBitsAuto(pak);
                U32 time = pktGetBitsAuto(pak);
                U32 size = pktGetBitsAuto(pak);
                if (e)
                    RaidSchedule(e, sgid, time, size);
            }
            break;

            case CLIENTINP_RAID_CANCEL:
            {
                U32 raidid = pktGetBitsAuto(pak);
                if (e)
                    RaidCancel(e, raidid);
            }
            break;

            //------------------------------------------------------------------------------------------
            // Trade Messages ///////////////////////////////////////////////////////////////////////////
            //------------------------------------------------------------------------------------------
            // clientside functions live in uiNet.c, server functions live in trading.c

            case CLIENTINP_TRADE_CANCEL:
                {
                    trade_cancel( e, pktGetBitsPack(pak,1) );
                }break;

            case CLIENTINP_TRADE_UPDATE:
                {
                    trade_update( e, pak );
                }break;


            //-------------------------------------------------------------------------------------------
            case CLIENTINP_REQUEST_PLAYER_INFO:
                {
                    int id = pktGetBitsPack(pak, PKT_BITS_TO_REP_SVR_IDX);
                    int tab = pktGetBitsPack(pak, 3);
                    if (e)
                        sendEntityInfo(e, id, tab);
                } break;
            case CLIENTINP_UPDATE_COMBAT_STATS:
                {
                    if( e )
                        receiveCombatMonitorStats(e, pak);
                }break;
            case CLIENTINP_VISITED_MAP_CELL:
                if (e) {
                    if(MVC_NEW_MAP & mapperVisitCell(e,pktGetBitsPack(pak,8))
                        && db_state.is_static_map)
                    {
                        char ach[1024];

                        char *pch = strrchr(db_state.map_name, '/');
                        if(pch==NULL) pch=db_state.map_name-1;
                        strcpy(ach, "FirstVisit_");
                        strncat_count(ach, pch+1, ARRAY_SIZE(ach));

                        plaque_SendPlaqueNag(e, ach,10);
                    }
                } else {
                    goto ErrExit;
                }

                break;

            case CLIENTINP_SEND_SGCOSTUME:
                {
                    if (e && e->pl)
                    {
                        if( sgroup_verifyCostume( pak, e ) )
                        {
                            // mark player as cheater
                        }
                    } 
                    else
                    {
                        goto ErrExit;
                    }
                }
                break;

            case CLIENTINP_SEND_SGCOSTUME_DATA:
                {
                    if (e && e->pl)
                    {
                        if( sgroup_verifyCostumeEx( pak, e ) )
                        {
                            // mark player as cheater
                        }
                    } 
                    else
                    {
                        goto ErrExit;
                    }
                }
                break;

            case CLIENTINP_SGROUP_CREATE:
                {
                    if (e && e->pl)
                    {
                          int result = sgroup_ReceiveUpdate( e, pak );
                        if(result)
                            e->supergroup_update = true;
                        sgroup_sendResponse( e, result ); // send initial costume
                    } else {
                        goto ErrExit;
                    }

                }break;

            case CLIENTINP_SGROUP_COSTUMEREQ:
                {
                    if (e && e->pl)
                    {
                        if( e->supergroup ) // maybe they were kicked at same time they requested costume, if so silently ignore request
                            sgroup_sendResponse( e, 1 ); // send initial costume
                    } else {
                        goto ErrExit;
                    }

                }break;

            case CLIENTINP_HIDE_EMBLEM_UPDATE:
                {
                    if (e && e->pl && e->supergroup )
                    {
                        e->pl->hide_supergroup_emblem = pktGetBits(pak, 1);
                        START_PACKET( pakResponse, e, SERVER_HIDE_EMBLEM_UPDATE )
                            costume_sendEmblem( pakResponse, e );
                        END_PACKET
                    }
                }break;

            case CLIENTINP_SEND_TAILOR:
                {
                    if (e && e->pchar) {
                        Costume *costume;
                        PowerCustomizationList *powerCustList;
                        int genderChange = pktGetBits(pak, 2);
                        costume = costume_recieveTailor( pak, e, genderChange );
                        powerCustList = powerCustList_recieveTailor( pak, e );
                        if (costume && powerCustList)
                        {
                            if (processTailorCost(pak, e, genderChange, costume, powerCustList))
                            {
                                costume_applyTailorChanges(e, genderChange, costume);
                                powerCustList_applyTailorChanges(e, powerCustList);
                                //log player's costume data after successful tailor change
                                PrintCostumeToLog(e);
                            }
                        }
                        else
                        {
                            //    mark player as bad
                            int useFreeTailor = pktGetBits(pak,2); // gotta consume entire stream!  - This is from proccessTailorCost.
                        }
                    }
                }break;
            case CLIENTINP_SEND_POWER_CUST:
                {
                    if (e && e->pchar) {
                        PowerCustomizationList *powerCustList =    powerCustList_recieveTailor( pak, e);
                        if (powerCustList)
                        {
                            int powerCustCost = powerCust_GetTotalCost(e, e->pl->powerCustomizationLists[e->pl->current_powerCust], powerCustList);
                            if ((powerCustCost > e->pchar->iInfluencePoints) || (powerCustCost < 0))
                            {
                                powerCustList_destroy(powerCustList);
                            }
                            else
                            {
                                ent_AdjInfluence(e, -powerCustCost, NULL);
                                powerCustList_applyTailorChanges(e, powerCustList);
                            }
                        }
                    } else {
                        goto ErrExit;
                    }
                }break;
            case CLIENTINP_SEND_RESPEC:
                {
                    if (e && e->pl && e->pchar) {

                        bool res = 0;

                        //log_printf("respec_dbg", "Received client packet from %s", e->name);

                        res = character_ReceiveRespec( pak, e );

                        START_PACKET( pakResponse, e, SERVER_VALIDATE_RESPEC )
                        pktSendBits(pakResponse, 1, res);
                        END_PACKET

                    } else {
                        //log_printf("respec_dbg", "ERROR: Received bad client packet from %s", e ? e->name : "(UNKNOWN)");
                        goto ErrExit;
                    }

                }break;

            case CLIENTINP_RESPEC_CANCEL:
                {
                    if (e && e->pl)
                    {
                        // Respecs can be used to level-up (ie. choose all
                        // powers including the new one) so both flags must
                        // be cleared here.
                        e->pl->isRespeccing = 0;
                        e->pl->levelling = 0;
                        if (e->pchar) 
                            character_ActivateAllAutoPowers(e->pchar);
                    }

                }break;

            case CLIENTINP_REQUEST_TASK_DETAIL:
                {
                    int dbid = pktGetBitsPack(pak, 1);
                    int context = pktGetBitsPack(pak, 1);
                    int handle = pktGetBitsPack(pak, 1);

                    if (e) {
                        if( !TaskSendDetail(e, dbid, tempStoryTaskHandle(context, handle, 0,  TaskForceIsOnArchitect(e))) )
                        {
                            // mark player as bad
                        }
                    }
                }break;

            case CLIENTINP_REQUEST_SOUVENIR_INFO:
                {
                    int val = pktGetBitsPack(pak, 1);
                    if (e)
                        scRequestDetails(e, val);
                }
                break;

            case CLIENTINP_STORE_BUY_ITEM:
                if (e && e->pl) {
                    if(!store_ReceiveBuyItem( pak, e))
                    {
                        // mark player as bad
                    }
                } else {
                    goto ErrExit;
                }
                break;

            case CLIENTINP_STORE_BUY_SALVAGE:
                if (e && e->pl) {
                    if(!store_ReceiveBuySalvage( pak, e))
                    {
                        // mark player as bad
                    }
                } else {
                    goto ErrExit;
                }
                break;
            case CLIENTINP_STORE_BUY_RECIPE:
                if (e && e->pl) {
                    if(!store_ReceiveBuyRecipe( pak, e))
                    {
                        // mark player as bad
                    }
                } else {
                    goto ErrExit;
                }
                break;
            case CLIENTINP_STORE_SELL_ITEM:
                if (e && e->pl) {
                    if(!store_ReceiveSellItem( pak, e))
                    {
                        // mark player as bad
                    }
                } else {
                    goto ErrExit;
                }
                break;
            case CLIENTINP_ABANDON_MISSION:
                {
                    goto ErrExit;
                }
                break;

            case CLIENTINP_GIFTSEND:
                {
                    if (e)
                    {
                        gift_recieveSend(e, pak);
                    }
                    else
                        goto ErrExit;
                }break;

            case CLIENTINP_ACCEPT_POWREQ:
                {
                    int id = pktGetBitsPack(pak, 16);
                    bool accept = pktGetBits(pak, 1);

                    if(e)
                        character_ConfirmPower(e->pchar, id, accept);
                    else
                        goto ErrExit;
                }
                break;
            case CLIENTINP_ACCEPT_TEAMLEVEL:
                {
                    int levelAccepted = pktGetBitsAuto(pak);
                    character_AcceptLevelChange(e->pchar, levelAccepted);
                }break;
            case CLIENTINP_REWARDCHOICE:
                {
                    int choice = pktGetBitsPack(pak, 3);

                    if( e )
                        rewardChoiceReceive(e, choice);
                    else
                        goto ErrExit;

                }break;

            case CLIENTINP_NOTAHACKER:
                got_notahacker = pktGetBitsPack(pak, 1);
                break;

            case CLIENTINP_ARENA_REQKIOSK:
                if (e)
                {
                    int radioButtonFilter=pktGetBitsPack(pak, 1);
                    int tabFilter=pktGetBitsPack(pak, 1);
                    ArenaSendKioskInfo(e,radioButtonFilter,tabFilter);
                }
                break;

            case CLIENTINP_ARENA_REQ_CREATE:
                if (e)
                    ArenaRequestCreate(e);
                break;

            case CLIENTINP_ARENA_REQ_RESULT:
                if (e)
                {
                    int eventid = pktGetBitsPack( pak, 6);
                    int uniqueid = pktGetBitsPack( pak, 6);
                    ArenaRequestResults(e, eventid, uniqueid );
                }
                break;

            case CLIENTINP_ARENA_REQ_OBSERVE:
                if (e)
                {
                    int eventid = pktGetBitsPack( pak, 6);
                    int uniqueid = pktGetBitsPack( pak, 6);
                    ArenaRequestObserve(e, eventid, uniqueid );
                }
                break;

            case CLIENTINP_ARENA_REQ_JOIN:
                if (e)
                {
                    int eventid = pktGetBitsPack( pak, 6);
                    int uniqueid = pktGetBitsPack( pak, 6);
                    int ignore_active = pktGetBits(pak, 1);
                    ArenaRequestJoin( e, eventid, uniqueid, false, ignore_active );
                }
                break;
            case CLIENTINP_ARENA_DROP:
                if ( e )
                {
                    int eventid = pktGetBitsPack( pak, 6);
                    int uniqueid = pktGetBitsPack( pak, 6);
                    int kicked_dbid = pktGetBits(pak, 32);
                    ArenaDrop( e, eventid, uniqueid, kicked_dbid );
                }break;

            case CLIENTINP_ARENA_CREATOR_UPDATE:
                if (e)
                {
                    ArenaReceiveCreatorUpdate(e, pak);
                }break;
            case CLIENTINP_ARENA_PLAYER_UPDATE:
                if (e)
                {
                    ArenaReceivePlayerUpdate(e, pak);
                }break;
            case CLIENTINP_ARENA_FULL_PLAYER_UPDATE:
                if (e)
                {
                    ArenaReceiveFullPlayerUpdate(e, pak);
                }break;
            case CLIENTINP_ARENAKIOSK_OPEN:
                if (e)
                {
                    ArenaKioskOpen(e);
                }
            xcase CLIENTINP_ARENA_REQ_TELEPORT:
                if(e && e->pl)
                {
                    int eventid = pktGetBitsPack(pak, 1);
                    if(eventid == e->pl->arenaTeleportEventId)
                        e->pl->arenaTeleportAccepted = 1;
                }
            xcase CLIENTINP_PETCOMMAND:
                petReceiveCommand( e, pak );
            xcase CLIENTINP_PETSAY:
                petReceiveSay( e, pak );
            xcase CLIENTINP_PETRENAME:
                petReceiveRename(e,pak);
            xcase CLIENTINP_ARENA_MANAGEPETS:
                {
                    sendArenaManagePets( e );
                }
            xcase CLIENTINP_ARENA_COMMITPETCHANGES:
                {
                    PetArenaCommitPetChanges( e, pak );
                }
            xcase CLIENTINP_CRAFTING_REVERSE_ENGINEER:
             {
                 salvageinv_ReverseEngineer_Receive( pak, e->pchar );
             }
             xcase CLIENTINP_INVENT_SELECTRECIPE:
             {
                character_InventReceiveRecipeSelection( e->pchar, pak );
             }
             xcase CLIENTINP_INVENT_SLOTCONCEPT:
             {
                character_InventReceiveSlotConcept( e->pchar, pak );
             }
             xcase CLIENTINP_INVENT_UNSLOT:
             {
                character_InventReceiveUnSlot( e->pchar, pak );
             }
             xcase CLIENTINP_INVENT_HARDENSLOT:
             {
                character_InventReceiveHardenSlot( e->pchar, pak );
             }
             xcase CLIENTINP_INVENT_FINALIZE:
             {
                character_InventReceiveFinalize( e->pchar, pak );
             }                 
             xcase CLIENTINP_INVENT_CANCEL:
             {
                character_InventReceiveCancel( e->pchar, pak );
             }    
            xcase CLIENTINP_WORKSHOP_CREATE:
             {
                character_WorkshopReceiveDetailRecipeBuild( e->pchar, pak );
             }    
            xcase CLIENTINP_WORKSHOP_INTERACT:
             {
                char *value = pktGetString(pak);
                int ent_id = pktGetBitsAuto(pak);

                if( e && e->pl && strlen(value) != 0) {
                    if (ent_id != 0 && entInUse(ent_id))
                    {
                        Entity *pEnt = entities[ent_id];
                        EntityRef eRef = erGetRef(pEnt);
                        if (pEnt && eRef)
                            e->pl->workshopEntity = eRef;
                    }
                    Strncpyt(e->pl->workshopInteracting, value);
                    sendActivateUpdate( e, true, 0, kDetailFunction_Workshop );
                }
             }    
            xcase CLIENTINP_WORKSHOP_CLOSE:
            {
                character_WorkshopInterrupt(e); 
            }
            xcase CLIENTINP_INCARNATE_CREATE:
            {
                if( verify( pak ))
                {
                    char *pchRecipe = pktGetString(pak);
                    int iLevel = pktGetBitsAuto(pak);
                    Character *p = e->pchar;
                    const DetailRecipe *r = detailrecipedict_RecipeFromName(pchRecipe);
                    if(!r)
                    {
                        dbLog("cheater",NULL,"Player with authname %s tried to send invalid workshop recipe name (%s)\n", p->entParent->auth_name, pchRecipe);
                    }
                    else
                    {
                        int result = character_DetailRecipeCreate( p, r, iLevel, 0 );

                        if (result == 0)
                        { 
                            // 0 means failed to reward (inventory full)
                            chatSendToPlayer( p->entParent->db_id, localizedPrintf(p->entParent,"WorkshopFailInventory", r->ui.pchDisplayName), INFO_USER_ERROR, 0 );
                        } 
                        else if (result == -1)
                        {
                            // -1 means failure to satisfy requirements, probably cheating
                            dbLog("cheater",NULL,"Player with authname %s tried to create recipe %s without meeting requirements\n", p->entParent->auth_name,pchRecipe);
                        }
                    }
                }
            }
            xcase CLIENTINP_PORTAL_INTERACT:
            {
                char *value = pktGetString(pak);

                if( e && e->pl && strlen(value) != 0) 
                {
                    if (e->pchar && character_GetSecurityLevelExtern(e->pchar) >= 14)
                    {
                        // do the teleport thingy here. 
                        if (e->pchar && value)
                        {
                            // This is a hack for now to stop clients spoofing the destination
                            // The interaction string needs to not come from the client but instead
                            // be found serverside by searching for a nearby "door".
                            if (stricmp("Ouroboros.EntryPortal", value) == 0)
                            {
                                if (character_NamedTeleport(e->pchar, 0, value))
                                {
                                    character_PlayFX(e->pchar, e->pchar, 
                                        "POWERS/Teleportation/TeleportSelf.fx", colorPairNone, 0, 0, PLAYFX_NO_TINT);
                                }
                                else
                                {
                                    sendDialog(e, localizedPrintf(e, "OuroborosLocked"));
                                }
                            }
                            else
                            {
                                dbLog("cheater", e, "CLIENTINP_PORTAL_INTERACT with invalid string %s", value);
                            }
                        }
                    } else {
                        sendDialog(e, textStd("TooLowLevelForOurborosPortal"));
                    }
                }
            }    
            xcase CLIENTINP_BASE_SETPOS:
            {
                Vec3 pos;
                pos[0] = pktGetF32(pak);
                pos[1] = pktGetF32(pak);
                pos[2] = pktGetF32(pak);
                if( e &&
                    ((e->pl->architectMode == kBaseEdit_Architect &&
                    playerCanModifyBase(e, &g_base))||
                    (e->pl->architectMode == kBaseEdit_AddPersonal &&
                    playerCanPlacePersonal(e, &g_base)))&&
                    pos[1] >= 0 && pos[1] <= 8.f )
                {
                    entUpdatePosInstantaneous( e, pos );
                }
            }
            xcase CLIENTINP_BASE_INTERACT:
            {
                int detail_id = pktGetBitsPack(pak, 8);
                char *value = pktGetString(pak);
                // TODO: The player has clicked on Detail with the given ID
                //       value is whatever the "Interact" property was.
                if (e )
                    baseActivateDetail(e,detail_id,value);
            }
            xcase CLIENTINP_REQ_WORLD_UPDATE:
            {
                if (e)
                    e->world_update = 1;
            }
            xcase CLIENTINP_BASESTORE_ADJ_SALVAGE_FROM_INV: 
            {
                if (e)
                    ReceiveStoredSalvageAdjustFromInv( e->pchar, pak ); 
            }
            xcase CLIENTINP_BASESTORAGE_ADJ_INSPIRATION_FROM_INV: 
            {
                if (e)
                    ReceiveStoredInspirationAdjustFromInv( e->pchar, pak ); 
            }
            xcase CLIENTINP_BASESTORAGE_ADJ_ENHANCEMENT_FROM_INV: 
            {
                if (e)
                    ReceiveStoredEnhancementAdjustFromInv( e->pchar, pak ); 
            }
            xcase CLIENTINP_TEAMCOMPLETE_RESPONSE:
            {
                if (e)
                {
                    int response = pktGetBits( pak, 1 );
                    int neverask = pktGetBits( pak, 1 );
                    TaskReceiveTeamCompleteResponse(e, response, neverask);
                }
            }
            xcase CLIENTINP_SEARCH:
                search_receive( e, pak );
            xcase CLIENTINP_UPDATE_NAME:
                { 
                    char *newname = strdup(pktGetString(pak));

                     if( e && (e->db_flags&DBFLAG_RENAMEABLE))
                    {
                        char *buf; 
                        int failed = false;

                        if (ValidateName(newname, e->auth_name, 1,MAX_PLAYER_NAME_LEN(e)) != VNR_Valid)
                        {
                            buf = localizedPrintf(e,"RenameInvalid", e->name, newname);
                            failed = true;
                        }
                        else if( strcmp( newname, e->name)==0 || dbSyncPlayerRename(e->db_id, newname))
                        {
                            RandomFameWipe(e);
                            buf = localizedPrintf(e,"RenameSuccess", e->name, newname);
                            Strncpyt(e->name, newname);
                            e->send_costume = 1;
                            e->db_flags &= ~(DBFLAG_RENAMEABLE);
                        }
                        else
                        {
                            buf = localizedPrintf(e,"RenameExists", e->name, newname);
                            failed = true;
                        }
                        chatSendToPlayer(e->db_id, buf, INFO_USER_ERROR, 0);

                        if(failed)
                        {
                            START_PACKET( pakResponse, e, SERVER_UPDATE_NAME )
                            END_PACKET
                        }
                    }
                    SAFE_FREE(newname);
                }
            xcase CLIENTINP_DELETE_TEMPPOWER:
                {
                    int iPow = pktGetBitsPack(pak,8);
                    PowerSet *temporaryPowers = character_OwnsPowerSet(e->pchar, powerdict_GetBasePowerSetByName(&g_PowerDictionary, "Temporary_Powers", "Temporary_Powers"));
                    if (temporaryPowers && eaGet(&temporaryPowers->ppPowers, iPow))
                    {
                        Power* deletePow = temporaryPowers->ppPowers[iPow];
                        if( deletePow )
                        {
                            char *powname;
                            strdup_alloca(powname, deletePow->ppowBase->pchDisplayName);
                            if( deletePow->ppowBase->bDeletable )
                            {
                                chatSendToPlayer( e->db_id, localizedPrintf(e,"DeletedPower",deletePow->ppowBase->pchDisplayName), INFO_SVR_COM, 0 );
                                character_RemovePower(e->pchar, deletePow);
                                LOG_ENT(e, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[Power:Delete] deleted temp power %s", powname);
                                
                            }
                            else
                            {
                                chatSendToPlayer( e->db_id, localizedPrintf(e,"CannotDeletePower",deletePow->ppowBase->pchDisplayName), INFO_USER_ERROR, 0 );
                                LOG_ENT(e, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[Power:Delete] failed, not deletable %s", powname);
                            }
                        }
                    }
                    else
                    {
                        LOG_ENT(e, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "Power:Delete trying to delete invalid index %i", iPow);
                    }
                }
            xcase CLIENTINP_DELETE_SALVAGE:
                {
                    char * nameSalvage;
                    int amount;
                    SalvageItem const *sal;
                    bool is_stored_salvage;
                    int current_amt = 0;

                    strdup_alloca(nameSalvage,pktGetString(pak));
                    amount = pktGetBitsPack(pak, 1);
                    is_stored_salvage = pktGetBitsAuto(pak);
                    sal = salvage_GetItem(nameSalvage);

                    if(sal)
                    {
                        if(is_stored_salvage)
                            current_amt = character_StoredSalvageCount(e->pchar, sal);
                        else
                            current_amt = character_SalvageCount(e->pchar, sal);
                    }

                    if(!sal)
                        dbLog("cheater", e, "can't find salvage named %s", nameSalvage);
                    else if( current_amt == 0 || amount <= 0 || amount > current_amt )
                        dbLog("cheater", e, "bad amount (%i) of %s sent for delete", amount, nameSalvage );
                    else if( !is_stored_salvage && !character_CanSalvageBeChanged( e->pchar, sal, -amount) )
                        dbLog("cheater", e, "can't adjust salvage %s by %i", nameSalvage, -amount);
                    else
                    {
                        int count;
                        if(!is_stored_salvage)
                        {
                            character_AdjustSalvage(e->pchar, sal, -amount, "trash", false);

                            count = character_SalvageCount(e->pchar, sal);
                            LOG_ENT(e, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[Salvage:Delete] Adjusted salvage %s by %d to %d", nameSalvage, -amount, count);
                        }
                        else
                        {
                            character_AdjustStoredSalvage(e->pchar, sal, -amount, "trash", false);

                            count = character_StoredSalvageCount(e->pchar, sal);
                            LOG_ENT(e, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[StoredSalvage:Delete] Adjusted salvage %s by %d to %d", nameSalvage, -amount, count);
                        }
                    }
                }
            xcase CLIENTINP_DELETE_RECIPE:
                {

                    char * nameRecipe;
                    int amount;
                     const DetailRecipe *si = NULL;

                    strdup_alloca(nameRecipe, pktGetString(pak));
                    amount = pktGetBitsPack(pak, 1);
                     si = recipe_GetItem( nameRecipe );

                     if( si )
                     {
                        if( amount <= 0 || amount > character_RecipeCount( e->pchar, si ) )
                            dbLog( "cheater", e, "bad amount (%i) of %s sent for delete", amount, nameRecipe );
                        else if( character_CanRecipeBeChanged( e->pchar, si, -amount) )
                        {
                            int count;
                             character_AdjustRecipe( e->pchar, si, -amount, "trash" );

                            count = character_RecipeCount(e->pchar, si);
                            LOG_ENT(e, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[Recipe:Delete] Adjusted recipe %s by %i to %d", nameRecipe, -amount, count);
                        }
                        else
                        {
                            dbLog("cheater", e, "can't adjust recipe %s by %i", nameRecipe, -amount);
                        }
                     }
                     else
                     {
                        dbLog("cheater", e, "recipe %s not found", nameRecipe);
                     }
                }
            xcase CLIENTINP_SELL_RECIPE:
                {

                    char * nameRecipe;
                    int amount;
                     const DetailRecipe *si = NULL;

                    strdup_alloca(nameRecipe, pktGetString(pak));
                    amount = pktGetBitsPack(pak, 1);
                     si = recipe_GetItem( nameRecipe );                    

                     if( si )
                     {
                        if( amount <= 0 || amount > character_RecipeCount( e->pchar, si ) )
                            dbLog("cheater", e, "bad amount (%i) of %s sent for sale", amount, nameRecipe );
                        else if( character_CanRecipeBeChanged( e->pchar, si, -amount) )
                        {
                            int value = si->SellToVendor * amount;
                            dbLog("Recipe:Sell", e, "Sold %i recipe %s for %i", amount, nameRecipe, value);

                            character_AdjustRecipe( e->pchar, si, -amount, "sell" );
                            ent_AdjInfluence(e, value, NULL);
                        } else {
                            dbLog("cheater", e, "can't adjust recipe %s by %i", nameRecipe, -amount);
                        }
                     }
                     else
                     {
                        dbLog("cheater", e, "recipe %s not found", nameRecipe);
                     }
                }
            xcase CLIENTINP_SELL_SALVAGE:
                {
                    char * nameSalvage;
                    int amount;
                    SalvageItem const *sal;

                    strdup_alloca(nameSalvage,pktGetString(pak)); 
                    amount = pktGetBitsPack(pak, 1);
                    sal = salvage_GetItem(nameSalvage);

                    if(!sal)
                        dbLog("cheater", e, "can't find salvage named %s", nameSalvage);
                    else if( amount <= 0 || amount > character_SalvageCount( e->pchar, sal ) )
                        dbLog("cheater", e, "bad amount (%i) of %s sent for delete", amount, nameSalvage );
                    else if( !character_CanSalvageBeChanged( e->pchar, sal, -amount) )
                        dbLog("cheater", e, "can't adjust salvage %s by %i", nameSalvage, -amount);
                    else
                    {
                        int value = sal->sellAmount * amount;
                        character_AdjustSalvage(e->pchar, sal, -amount, "sell", false);
                        ent_AdjInfluence(e, value, NULL);
                        dbLog("Salvage:Sell", e, "Sold %i salvage %s for %i", amount, nameSalvage, value);
                    }
                }
            xcase CLIENTINP_OPEN_SALVAGE:
                {
                    char *salvageName = pktGetString(pak);

                    if (server_state.accountCertificationTestStage % 100 == 1)
                    {
                        int flags = server_state.accountCertificationTestStage / 100;

                        if (flags % 10 == 0)
                        {
                            server_state.accountCertificationTestStage = 0;
                        }

                        if (flags / 10 == 1)
                        {
                            // there's no response that truly must happen if this message fails...
                        }

                        break;
                    }

                    openSalvageByName(e, salvageName);
                }
            xcase CLIENTINP_GET_SALVAGE_IMMEDIATE_USE_STATUS:
                {
                    //
                    //    Incoming packet:
                    //        CLIENTINP_GET_SALVAGE_IMMEDIATE_USE_STATUS
                    //            salvage item name
                    //    Outgoing packet:
                    //        SERVER_SALVAGE_IMMEDIATE_USE_STATUS
                    //            salvage name
                    //            flags (SalvageImmediateUseStatus OR'd together)
                    //            msgID (message to the user about using this item now)
                    //
                    char *salvageName = pktGetString(pak);
                    char* msgID = NULL;
                    U32 flags = 0;
                    estrCreate( &msgID );
                    salvage_queryImmediateUseStatus( e, salvageName, &flags, &msgID );
                    
                    START_PACKET(pak, e, SERVER_SALVAGE_IMMEDIATE_USE_STATUS);
                    pktSendString( pak, salvageName );
                    pktSendBitsAuto( pak, flags );
                    pktSendString( pak, msgID );
                    END_PACKET                    
                }
            xcase CLIENTINP_BUY_RECIPE:
                {
                    char * nameRecipe;
                    int amount = 1;
                    const DetailRecipe *si = NULL;

                    strdup_alloca(nameRecipe, pktGetString(pak));
                    si = recipe_GetItem( nameRecipe );                    

                    if( si )
                    {
                        int value = si->BuyFromVendor * amount;

                        if( character_CanRecipeBeChanged( e->pchar, si, 1) && e->pchar->iInfluencePoints >= value)
                        {
                            dbLog("Recipe:Sell", e, "Bought %i recipe %s for %i", 1, nameRecipe, value);
                            character_AdjustRecipe( e->pchar, si, 1, "buy" );
                            ent_AdjInfluence(e, -value, NULL);
                            badge_StatAdd(e, "inf.invention.bought", value);

                        } else {
                            dbLog("cheater", e, "can't adjust recipe %s by %i", nameRecipe, 1);
                        }
                    }
                    else
                    {
                        dbLog("cheater", e, "recipe %s not found", nameRecipe);
                    }
                }
            xcase CLIENTINP_AUCTION_REQ_BANNEDUPDATE:
                {
                    sendAuctionBannedUpdateToClient(e);
                }
            xcase CLIENTINP_AUCTION_REQ_INVUPDATE:
                {
                    if ( !auctionBanned(e,0) )
                    {
                        auctionRateLimiter(e);
                        ent_ReqAuctionInv(e);
                    }
                    else
                        e->pchar->auctionInvUpdated = true;
                }
            xcase CLIENTINP_AUCTION_REQ_HISTORYINFO:
                {
                    char *pchIdent = pktGetString(pak);

                    if(canUseAuction(e,0))
                        ent_ReqAuctionHist(e, pchIdent);
                }
            xcase CLIENTINP_AUCTION_ADDITEM:
                {
                    static U32 uniqueID = 0;
                    int index, amt, price, status, id;
                    char *pchident;
                    
                    pchident = pktGetString(pak);
                    index = pktGetBitsPack(pak,1);
                    amt = pktGetBitsPack(pak,1);
                    price = pktGetBitsPack(pak,1);
                    status = pktGetBitsPack(pak,1);
                    id = pktGetBitsPack(pak,1);

                    if(canUseAuction(e,1) && AccountHasStoreProduct(ent_GetProductInventory( e ), SKU("coliauch")))
                    {
                        auctionRateLimiter(e);
                        ent_AddAuctionInvItemXact(e, pchident, index, amt, price, status, id);
                    }
                    else
                        e->pchar->auctionInvUpdated = true;
                }
            xcase CLIENTINP_AUCTION_GETITEM:
                {
                    int item_id;
                    item_id = pktGetBitsPack(pak, 1);

                    if(canUseAuction(e,1))
                    {
                        auctionRateLimiter(e);
                        ent_GetAuctionInvItemXact(e, item_id);
                    }
                    else
                        e->pchar->auctionInvUpdated = true;
                }
            xcase CLIENTINP_AUCTION_CHANGEITEMSTATUS:
                {
                    int item_id, price, status;
                    item_id = pktGetBitsPack(pak, 1);
                    price = pktGetBitsPack(pak, 1);
                    status = pktGetBitsPack(pak, 1);

                    if (status == AuctionInvItemStatus_ForSale && !AccountHasStoreProduct(ent_GetProductInventory( e ), SKU("coliauch")))
                    {
                        e->pchar->auctionInvUpdated = true;
                    } else {
                        if(canUseAuction(e,1))
                        {
                            auctionRateLimiter(e);
                            ent_ChangeAuctionInvItemStatusXact(e, item_id, price, status);
                        }
                        else
                            e->pchar->auctionInvUpdated = true;
                    }
                }
            xcase CLIENTINP_AUCTION_GETINFSTORED:
                {
                    int item_id = pktGetBitsPack(pak, 1);

                    if(canUseAuction(e,1))
                    {
                        auctionRateLimiter(e);
                        ent_GetInfStoredXact(e, item_id);
                    }
                    else
                        e->pchar->auctionInvUpdated = true;
                }
            xcase CLIENTINP_AUCTION_GETAMTSTORED:
                {
                    int item_id = pktGetBitsPack(pak, 1);

                    if(canUseAuction(e,1))
                    {
                        auctionRateLimiter(e);
                        ent_GetAmtStoredXact(e, item_id);
                    }
                    else
                        e->pchar->auctionInvUpdated = true;
                }
            xcase CLIENTINP_AUCTION_BATCH_REQ_ITEMSTATUS:
                {
                    int startIdx = pktGetBitsPack(pak,1);
                    int reqSize = pktGetBitsPack(pak,1);
                    if(canUseAuction(e,0))
                        ent_BatchReqAuctionItemStatus(e, startIdx, reqSize);
                }
            xcase CLIENTINP_AUCTION_LIST_REQ_ITEMSTATUS:
                {
                    int i, *idlist=0, size = pktGetBitsAuto(pak);
                    for( i = 0; i < size; i++ )
                        eaiPush(&idlist, pktGetBitsAuto(pak));
                    if(canUseAuction(e,0))
                        ent_ListReqAuctionItemStatus(e, idlist);
                    eaiDestroy(&idlist);
                }
            xcase CLIENTINP_MOVE_SALVAGE_TO_PERSONALSTORAGE:
            {
                int amt = pktGetBitsAuto(pak);
                char *sal_name = pktGetString(pak);
                Character *p = e->pchar;
                SalvageInventoryItem *itm = character_FindSalvageInv(p, sal_name);
                SalvageItem const *sal = itm?itm->salvage: NULL;
                int pre_capped_amt;

                if(!p)
                    break;
                
                if(!sal)
                {
                    dbLog("cheater", e, "unable to get item %s from stored salvage", sal_name);
                    break;
                }
                if(!salvageitem_IsInvention(sal))
                {
                    dbLog("cheater",e,"salvage item not invention %s",sal->pchName);
                    break;
                }
                
                if(amt < 0)
                    amt = itm->amount;
                amt = MIN(amt,(S32)itm->amount);
                pre_capped_amt = amt;
                amt = character_GetAdjustStoredSalvageCap(p,sal,amt);
                
//                if(!amt && p->entParent->access_level)
//                    amt = pre_capped_amt;

                if(amt)
                {
                    character_AdjustSalvage(p,sal,-amt,"movestoredsalvage", false);
                    character_AdjustStoredSalvage(p,sal,amt,"movestoredsalvage", false);
                    LOG_ENT_OLD(e, "storedsalvage put %i %s in personal storage",amt,sal->pchName);
                }
            }
            xcase CLIENTINP_MOVE_SALVAGE_FROM_PERSONALSTORAGE:
            {
                int amt = pktGetBitsAuto(pak);
                char *sal_name = pktGetString(pak);
                Character *p = e->pchar;
                SalvageInventoryItem *itm = character_FindStoredSalvageInv(p, sal_name);
                int pre_capped_amt;

                if(!p)
                    break;
                
                if(!itm || !itm->salvage )
                {
                    dbLog("cheater", e, "unable to get item %s from salvage", sal_name);
                    break;
                }
                
                if(amt < 0)
                    amt = itm->amount;
                amt = MIN(amt,(S32)itm->amount);
                pre_capped_amt = amt;
                amt = character_GetAdjustSalvageCap(p,itm->salvage,amt);
//                if(!amt && p->entParent->access_level)
//                    amt = pre_capped_amt;

                if(amt)
                {
                    character_AdjustSalvage(p,itm->salvage,amt,"movestoredsalvage", false);
                    character_AdjustStoredSalvage(p,itm->salvage,-amt,"movestoredsalvage", false);
                    LOG_ENT_OLD(e, "storedsalvage took %i %s from personal storage",amt,sal_name);
                }
            }
            xcase CLIENTINP_STORYARC_CREATE:
            {
                if(ArchitectCanEdit(e))
                    testStoryArc(e, playerCreatedStoryArc_Receive(pak));
            }
            xcase CLIENTINP_MISSIONSERVER_COMMAND:
            {
                if(e)
                    meta_handleRemote(NULL, pak, e->db_id, e->access_level);
            }
            xcase CLIENTINP_MISSIONSERVER_PUBLISHARC:
            {
                int arcid = pktGetBitsAuto(pak);
                char *arcstr = pktGetString(pak);
                if(ArchitectCanEdit(e) && e && e->pl && AccountCanPublishOnMissionArchitect(ent_GetProductInventory( e ), e->pl->loyaltyPointsEarned, e->pl->account_inventory.accountStatusFlags))
                    missionserver_map_publishArc(e, arcid, arcstr);
            }
            xcase CLIENTINP_MISSIONSERVER_UNPUBLISHARC:
            {
                if(ArchitectCanEdit(e))
                    missionserver_map_unpublishArc(e, pktGetBitsAuto(pak));
            }
            xcase CLIENTINP_MISSIONSERVER_VOTEFORARC:
            {
                int arcid = pktGetBitsAuto(pak);
                MissionRating vote = pktGetBitsAuto(pak);
                if( e && !entIsTrial(e) )
                    missionserver_map_voteForArc(e, arcid, vote);
            }
            xcase CLIENTINP_MISSIONSERVER_SETKEYWORDSFORARC:
            {
                int arcid = pktGetBitsAuto(pak);
                int vote = pktGetBitsAuto(pak);
                if( e && !entIsTrial(e) )
                    missionserver_map_setKeywordsForArc(e, arcid, vote);
            }
            xcase CLIENTINP_MISSIONSERVER_REPORTARC:
            {
                int arcid = pktGetBitsAuto(pak);
                int type = pktGetBitsAuto(pak);
                char *comment = pktGetString(pak);
                if( e && !entIsTrial(e) )
                {
                    if( type == kComplaint_BanMessage && !e->access_level )
                        break;

                    missionserver_map_reportArc(e, arcid, type, comment, e->pl->chat_handle[0]?e->pl->chat_handle:e->name);
                }
            }
            xcase CLIENTINP_MISSIONSERVER_SEARCHPAGE:
            {
                MissionSearchPage category = pktGetBitsAuto(pak);
                char *context = pktGetString(pak);
                int  page = pktGetBitsAuto(pak);
                int viewedFilter = pktGetBitsAuto(pak);
                int levelFilter = pktGetBitsAuto(pak);
                int authorArcId = pktGetBitsAuto(pak);
                if(levelFilter)    //if we're filtering, make sure we're in the right range.
                    MINMAX1(levelFilter, 1, MAX_PLAYER_SECURITY_LEVEL);
                if(ArchitectCanEdit(e))
                    missionserver_map_requestSearchPage(e, category, context, page, viewedFilter, levelFilter, authorArcId);
            }
            xcase CLIENTINP_MISSIONSERVER_ARCINFO:
            {
                missionserver_map_requestArcInfo(e, pktGetBitsAuto(pak));
            }
            xcase CLIENTINP_MISSIONSERVER_ARCDATA_PLAYARC:
            {
                int arcid = pktGetBitsAuto(pak);
                int test = pktGetBits(pak, 1);
                int devchoice = pktGetBits(pak, 1);
                if(e && e->pl && AccountCanPlayMissionArchitect(ent_GetProductInventory( e ), e->pl->loyaltyPointsEarned, e->pl->account_inventory.accountStatusFlags))
                {
                    missionserver_map_requestArcData_playArc(e, arcid, test, devchoice);
                }
            }
            xcase CLIENTINP_MISSIONSERVER_ARCDATA_VIEWARC:
            {
                missionserver_map_requestArcData_viewArc(e, pktGetBitsAuto(pak));
            }
            xcase CLIENTINP_MISSIONSERVER_COMMENT:
            {
                int arcid = pktGetBitsAuto(pak);
                char *msg = pktGetString(pak);

                if(e && e->pl && !entIsTrial(e) )
                    missionserver_map_comment(e, arcid, localizedPrintf( e, "AuthorSentYouAComment", e->pl->chat_handle[0]?e->pl->chat_handle:e->name, msg) );
            }
            xcase CLIENTINP_ARCHITECTKIOSK_INTERACT:
                if(e)
                    architectKiosk_recieveClick(e,pak);
            xcase CLIENTINP_ARCHITECTKIOSK_REQUESTINVENTORY:
                if(e)
                    missionserver_map_requestInventory(e);
            xcase CLIENTINP_REQUEST_SG_COLORS:
                if(e)
                    costume_sendSGColors(e);
            xcase CLIENTINP_UPDATE_BADGE_MONITOR_LIST:
                if(e)
                    badgeMonitor_ReceiveInfo(e,pak);
            xcase CLIENTINP_RENAME_BUILD_RESPONSE:
                if(e)
                    character_BuildRename(e,pak);
            xcase CLIENTINP_GET_PLAYNC_AUTH_KEY:
                if(e)
                    dbRequestPlayNCAuthKey(e,pak);
            xcase CLIENTINP_COSTUMECHANGEEMOTELIST_REQUEST:
                if(e)
                    sendCostumeChangeEmoteList(e);
            xcase CLIENTINP_DO_PROMOTE:
            {
                char promotedPlayerName[64];
                char buf[128];
                strncpyt(promotedPlayerName, pktGetString(pak), sizeof(promotedPlayerName));
                if(e)
                {
                    sprintf( buf, "promote_long %i %i %i %i \"%s\"", e->supergroup_id, sgroup_rank(e, e->db_id), 1, e->db_id, promotedPlayerName);
                    serverParseClient( buf, NULL );
                }
            }
            xcase CLIENTINP_REQUEST_SG_PERMISSIONS:
            {
                if (e && e->supergroup)
                {
                    START_PACKET(pak, e, SERVER_SG_PERMISSIONS);
                    pktSendBits(pak, 1, sgrp_CanSetPermissions(e));
                    END_PACKET
                }
            }
            xcase CLIENTINP_MORAL_CHOICE_RESPONSE:
            {
                int result = pktGetBitsPack(pak, 1);

                if (CutscenePaused())
                {
                    MadeCutsceneMoralChoice(result);
                }
                else
                {
                    rewardChoiceReceive(e, result - 1);
                }
            }
            xcase CLIENTINP_NEW_FEATURE_OPEN:
            {
                int id = pktGetBitsAuto(pak);
                char buf[128];
                sprintf(buf, "new_feature_open %d", id);
                serverParseClientEx(buf, client, ACCESS_INTERNAL);
            }
            xcase CLIENTINP_UPDATE_NEW_FEATURE_VERSION:
            {
                if (e && e->pl)
                {
                    e->pl->newFeaturesVersion = pktGetBitsAuto(pak);
                }
            }
            xdefault:
                LOG_OLD_ERR( "getInput received an unhandled cmd_num: %d last_cmd_num: %d", cmd_num, last_cmd_num);
                goto ErrExit;
            break;

        }   //end switch

        // End log of this packet.
        
        if(logPacket){
            clientSubPacketLogEnd(CLIENT_PAKLOG_IN, logPacket);
            logPacket = NULL;
        }
        
        // End timer.
        
        if(timer_started)
        {
            PERFINFO_AUTO_STOP();
        }
    }       //end while

    clientSetPacketLogsSent(CLIENT_PAKLOG_IN, pak, e);
    
    if ((pak->id == 37) == (!got_notahacker))
    {
        // This command should show up if and only if it's the 37th packet
        LOG( LOG_CHEATING, LOG_LEVEL_VERBOSE, 0, "%s NOTAHACKER packet (id: %d; client: %d), AuthName %s, IP %s", got_notahacker?"Extra":"MISSING", pak->id, got_notahacker, e->auth_name, makeIpStr(client->link->addr.sin_addr.S_un.S_addr));
    }
    return 1;

ErrExit:
    if(logPacket){
        clientSubPacketLogEnd(CLIENT_PAKLOG_IN, logPacket);
        logPacket = NULL;
    }

    if (pak->stream.errorFlags & BSE_OVERRUN)
    {
        // assert?
        LOG_OLD_ERR( "Read past end of packet while processing cmd_num: %d, last_cmd_num: %d", cmd_num, last_cmd_num);
    }

    clientSetPacketLogsSent(CLIENT_PAKLOG_IN, pak, e);

    if(timer_started)
    {
        PERFINFO_AUTO_STOP();
    }
    return 1;

}

/* End of File */
