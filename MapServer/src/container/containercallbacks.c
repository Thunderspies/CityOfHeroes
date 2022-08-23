#include "entity/entserver.h"
#include "comm_backend.h"
#include <utilitieslib/utils/error.h>
#include "varutils.h"
#include <utilitieslib/assert/assert.h>
#include "entity/entVarUpdate.h"
#include "comm_game.h"
#include "entity/entity.h"
#include "containerloadsave.h"
#include "containercallbacks.h"
#include "containerbroadcast.h"
#include "containerSupergroup.h"
#include "svr/svr_base.h"
#include "dbcomm/dbcomm.h"
#include <utilitieslib/network/crypt.h>
#include <utilitieslib/utils/timing.h>
#include "svr/svr_base.h"
#include "svr/svr_player.h"
#include "gameComm/sendToClient.h"
#include "entity/character_base.h"
#include "entity/character_net_server.h"
#include "entity/costume.h"        // Remove! Dep: bodyName()
#include "team.h"
#include <utilitieslib/utils/strings_opt.h>
#include "svr/svr_player.h"
#include "generator/encounter.h"
#include "storyarc/storyarcinterface.h"
#include "player/parseClientInput.h"
#include "dbcomm/dbmapxfer.h"
#include "language/langServerUtil.h"
#include "entity/entPlayer.h"
#include "filter/validate_name.h"
#include "entity/entGameActions.h"
#include "player/playerState.h"
#include <utilitieslib/components/earray.h>
#include "gameComm/Npc.h"
#include "cmdparse/cmdserver.h"
#include "cmdparse/cmdservercsr.h"
#include "storyarc/storyarcprivate.h"
#include "entity/motion.h"
#include "mapgroup.h"
#include "dbcomm/shardcomm.h"
#include "entity/buddy_server.h"
#include "seq/seqstate.h"
#include "seq/seq.h"
#include "dbcomm/dbnamecache.h"
#include "entity/teamCommon.h"
#include "entity/Supergroup.h"
#include "group/group.h"
#include "door.h"
#include "entity/SgrpServer.h"
#include "gameSys/raidmapserver.h"
#include "container/container_util.h"
#include "entity/SgrpServer.h"
#include "bases/bases.h"
#include "entity/character_net_server.h"
#include "gameSys/arenamap.h"
#include "entity/TaskforceParams.h"
#include "entity/character_level.h"
#include "player/badges_server.h"
#include "player/badges.h"
#include "Reward.h"
#include "cmdparse/cmdstatserver.h"
#include "dbcomm/logcomm.h"
#include "league.h"
#include "entity/alignment_shift.h"
#include "cmdparse/autoCommands.h"
#include <utilitieslib/utils/log.h>
#include "dbcomm/logcomm.h"
#include "gameSys/Turnstile.h"
#include "auth/authUserData.h"

int bad_db_id=0;
void flagAsBadEntity(int db_id)
{
    bad_db_id = db_id;
}

bool isBadEntity(int db_id)
{
    return db_id == bad_db_id;
}

static U32 containerHandleEntity(ContainerInfo *ci)
{
    int        cookie,t,hash[4];
    char    *str;
    Entity    *e;
    static    int    cookie_count=1; // cookies of 0 and 1 are reserved.
    int dt;

    //checkEntCorrupted(ci->data);
    str = ci->data;
    if (e=entFromDbIdEvenSleeping(ci->id))
    {
        // Resuming broken connection
        // free existinw g link and entity
        ClientLink    *client = clientFromEnt(e);
        
        // @testing -AB: see if we can detect corruption  :07/19/06
        {
            extern void checkEntBadges(Entity *e, char *ctxt);
            checkEntBadges(e,__FUNCTION__);
        }

        teamlogPrintf(__FUNCTION__, "%s(%i) already connected, removing old connection", e->name, e->db_id);
        if (client)
        {
            LOG_ENT(client->entity, LOG_ENTITY, LOG_LEVEL_VERBOSE, 0, "Load received container for %s(%i), which already exists and has a link", e->name, e->db_id);
            svrForceLogoutLink(client->link,"AuthServerCode: 22", KICKTYPE_OLDLINK); // cant login the same guy twice!
        }
        else
        {
            LOG_ENT(client->entity, LOG_ENTITY, LOG_LEVEL_VERBOSE, 0, "Load received container for %s(%i), which already exists", e->name, e->db_id);
            playerEntDelete(PLAYER_ENTS_ACTIVE,e,0);
            playerEntDelete(PLAYER_ENTS_CONNECTING,e,0);
            playerEntDelete(PLAYER_ENTS_OWNED,e,0);
            entFree(e);
        }
    }

    e = playerCreate();
    e->initialPositionSet = 0;
    e->db_id = ci->id;    // MAK - unpackEnt needs the db_id now
    unpackEnt(e,str);

    if(e->login_count>0)
    {
        if(!e->pchar || !e->pchar->entParent)
        {
            // This character didn't unpack properly.
            ClientLink *client = clientFromEnt(e);

            flagAsBadEntity(ci->id);

            if(client)
            {
                svrForceLogoutLink(client->link, "S_DATABASE_FAIL", KICKTYPE_BADDATA);
            }
            else
            {
                entFree(e);
            }

            return 1; // Tell the dbserver that there is some data problem with this entity, but don't delete it.
        }

        flagAsBadEntity(0); // Not a bad entity, and any previous bad ones have been dealt with
    }
    else if (ValidateName(e->name,e->auth_name,true,MAX_PLAYER_NAME_LEN(e))!=VNR_Valid)
    {
        entFree(e);
        return 0; // This will delete the entity.
    }
    
    teamlogPrintf(__FUNCTION__, "%s(%i) team=%i sg=%i tf=%i%s",
        e->name, e->db_id, e->teamup_id, e->supergroup_id, e->taskforce_id, ci->demand_loaded?" (FOR RELAY COMMAND)":"");

    if(e->npcIndex)
        e->costume = npcDefsGetCostume(e->npcIndex, 0);

    svrChangeBody(e, entTypeFileName(e));

    if(!e->last_day_jobs_start) 
    {
        //this should only happen when either
        //(1) this is the character's first time logging in since the change OR
        //(2) a mapserver crash or similar prevented us from recording an exit time.
        //  in either case, it's better to take the last_time, which is the old way
        //  of doing day jobs, than it is to just drop all the accumulated time.
        e->last_day_jobs_start = e->last_time;
    }

    // If it's been more than 15 minutes since their last logon, clear
    // our active toggles, reset hitpoints, and endurance, etc.
    // Note: Due to fluctuations in the space-time continuum, this delta
    //       could be negative.
    e->last_login = e->last_time;
    dt = dbSecondsSince2000() - e->last_time;
    if(dt>15*60)
    {
        bool bIsDead = e->pchar->attrCur.fHitPoints==0.0f;

        character_Reset(e->pchar, false);

        if(bIsDead)
        {
            e->pchar->attrCur.fHitPoints = 0.0f;
            e->pchar->attrMax.fAbsorb = 0.0f;
            e->pchar->attrCur.fEndurance = 0.0f;
        }
    }

    entTrackDbId(e);

    // MAK - need this above teamGetLatest - I need to know whether the
    // entity is owned when I receive the team
    playerEntAdd(PLAYER_ENTS_CONNECTING,e);
    playerEntAdd(PLAYER_ENTS_OWNED,e);
    playerEntDelete(PLAYER_ENTS_ACTIVE,e,0);

    // MAK - I'm lazy, so these containers HAVE to be updated in this order
    // (teamup update code needs to have task force story arc info)
    if(!teamGetLatest(e, CONTAINER_TASKFORCES))
    {
        e->taskforce_id = 0;
        e->teamlist_uptodate = 0;
    }
    if(!e->taskforce_id && e->pl)
        e->pl->taskforce_mode = 0;
    e->pl->taskforce_mode = 0;
    // this needs to happen before the team update
    if( e->taskforce ) // your mode always depends on whether you have a taskforce
    {
        if(estrLength(&e->taskforce->playerCreatedArc))
            e->pl->taskforce_mode = TASKFORCE_ARCHITECT;
        else
            e->pl->taskforce_mode = TASKFORCE_DEFAULT;
    }
    
    if(!teamGetLatest(e, CONTAINER_SUPERGROUPS))
        e->supergroup_id = 0;
    if(!teamGetLatest(e, CONTAINER_TEAMUPS))
    {
        e->teamup_id = 0;
        e->teamlist_uptodate = 0;
    }
//     if(!teamGetLatest(e, CONTAINER_RAIDS))
//         e->raid_id = 0;
    if(!teamGetLatest(e, CONTAINER_LEVELINGPACTS))
        e->levelingpact_id = 0;
    if(!teamGetLatest(e, CONTAINER_LEAGUES))
    {
        e->league_id = 0;
        e->leaguelist_uptodate = 0;
    }
    // VJD - Note, that the teamup update will pull apart teams on a map if the map is either a city
    //            map that has a play together script running at load time or a mission that has the 
    //            CoedTeamsAllowed flag set.

    // MAK - trying to catch weird container bug
    if ((e->teamup_id && !e->teamup) ||    (!e->teamup_id && e->teamup))
    {
        Errorf("Teamup_id container bug.  dbid %i, teamup_id %i, teamup.count %i", e->db_id, e->teamup_id, e->teamup?e->teamup->members.count:-1);
        // HACK - fix it for now..
        if (e->teamup)
            e->teamup = NULL;
        if (e->teamup_activePlayer)
            e->teamup_activePlayer = NULL;
        if (e->teamup_id) 
            e->teamup_id = 0;
    }
    if ((e->supergroup_id && !e->supergroup) ||    (!e->supergroup_id && e->supergroup))
    {
        Errorf("supergroup_id container bug.  dbid %i, supergroup_id %i, supergroup.count %i", e->db_id, e->supergroup_id, e->supergroup?e->supergroup->members.count:-1);
        // HACK - fix it for now..
        if (e->supergroup) 
            e->supergroup = NULL;
        if (e->supergroup_id) 
            e->supergroup_id = 0;
    }
    if( !e->supergroup_id && e->pl && e->pl->supergroup_mode )
    {
        e->pl->supergroup_mode = 0;
        entSetMutableCostumeUnowned( e, costume_current(e) ); // e->costume = costume_as_const(e->pl->costume[e->pl->current_costume]);
        e->powerCust = e->pl->powerCustomizationLists[e->pl->current_powerCust];
        e->send_costume = TRUE;
        e->updated_powerCust = TRUE;
    }

    if (OnMissionMap()) // Making the PlayTogetherMission functionality default
    {
        entSetOnGang(e, GANG_OF_PLAYERS_WHEN_IN_COED_TEAMUP);
    }

    teamSendMapUpdate(e); //    This may be redundant if you are in a league. Todo: add a check. if you are in a league, don't need to do teamSendMapUpdate, since everyone in your team is in your league
    leagueSendMapUpdate(e);

    if (ENTTYPE(e) == ENTTYPE_PLAYER)
    {
        alignmentshift_updatePlayerTypeByLocation(e);
    }

    sgroup_getStats( e );
    if (!ci->demand_loaded && e->supergroup)
        RaidNotifyPlayerSG(e);

    entity_state[ e->owner ] = ENTITY_SLEEPING;

    setInitialPosition(e, false);

    if (ci->demand_loaded) {
        if (ci->is_map_xfer) {
            // This shouldn't happen anymore, but I'm scared, so I'm leaving this as just a log entry
            teamlogPrintf(__FUNCTION__, "Received entity that was both is_map_xfer and demand_loaded! (ID: %d)", ci->id);
            LOG_OLD( "Received entity that was both is_map_xfer and demand_loaded! (ID: %d)", ci->id);
            ci->demand_loaded = 0;
        } else {
            assert(!e->teamup_id);
        }
    }
    if (ci->is_map_xfer && !ci->demand_loaded)
    {
        shardCommConnect(e);
        shardCommStatus(e);
    }

    // if they aren't resuming or transferring, make sure they are
    // no longer on a team
    if (!ci->is_map_xfer && !ci->demand_loaded)
    {
        int ret;
        if (e->league_id)
        {
            league_MemberQuit(e, 0);
        }
        if (e->teamup_id)
        {    
            ret = teamDelMember(e, CONTAINER_TEAMUPS, 0, 0);
            if (!ret)
                printf("");
        }
        leagueInfoDestroy(e);
        teamInfoDestroy(e);// be extra sure in case team didnt exist or you weren't in it
        assert(!e->teamup);

        // if they are on a task force, need to get added to team again
        TaskForceResumeTeam(e);

        // also make sure they get a task selected
        TaskSelectSomething(e);
    }

    e->on_since = e->last_time = dbSecondsSince2000();

    e->demand_loaded    = ci->demand_loaded;
    e->is_map_xfer        = ci->is_map_xfer;

    if (e->taskforce_id && e->taskforce)
    {
        // Refresh any taskforce parameters.
        TaskForceResumeEffects(e);
    }

    // If they are dead, make them look dead UNLESS they are a new character
    //    or they are on the way to the gurney.
    if(e->pchar->attrCur.fHitPoints<=0.0f
        && e->login_count>0    
        && !((stricmp(e->spawn_target,"Gurney")==0
        || stricmp(e->spawn_target,"AttackerGurney")==0
        || stricmp(e->spawn_target,"DefenderGurney")==0
        || stricmp(e->spawn_target,"VillainGurney")==0
        || stricmp(e->spawn_target,"MissionGurney")==0)
        && e->db_flags & DBFLAG_DOOR_XFER))
    {
        seqOrState(e->seq->state, 1, STATE_DEATH);
        modStateBits(e, ENT_DEAD, 0, TRUE );
        setClientState(e, STATE_DEAD);
    }
    LOG_ENT( e, LOG_ENTITY, LOG_LEVEL_DEPRECATED, 0, "Auth %s Ent %s sent from DbServer to map %d",e->auth_name,e->name,db_state.map_id);
    e->motion->vel[1] = .1f;
    if (!ci->is_map_xfer && !ci->demand_loaded) 
    {
        // If it's not a transfer, must be a login
        e->login_count++;

        // Okay, we've loaded the character and done all the standard checks
        // It should be safe to fix corrupted data now

        if (player_RepairCorruption(e,0))
        {        
            svrSendEntListToDb(&e, 1);
        }

        if (e->supergroup)
        {
            if (sgroup_RepairCorruption(e->supergroup,1))
            {    
                // Don't modify the sg before locking, just see if we need to
                if( teamLock( e, CONTAINER_SUPERGROUPS ))
                {
                    sgroup_RepairCorruption(e->supergroup,0);
                    teamUpdateUnlock(e,CONTAINER_SUPERGROUPS);
                }
            }
        }

    }

    ++cookie_count;
    if (!server_state.noEncryption) {
        t = timerCpuTicks64();
        do {
            cryptMD5Update((U8*)&t,sizeof(t));
            cryptMD5Update((U8*)&cookie_count,sizeof(cookie_count));
            cryptMD5Final(hash);
            t++; // change something in case we need to loop through here again.
        } while (entFromLoginCookie(hash[2])!=NULL);
        e->dbcomm_state.login_cookie = cookie = hash[2];
    } else {
        e->dbcomm_state.login_cookie = cookie = cookie_count;
    }

    LOG_OLD( "DbServer provided Cookie: %x", cookie);
    verbose_printf("received container %d.%d, sending acknowledgment\n",CONTAINER_ENTS,ci->id);

    teamUpdatePlayerCount(e);

    return cookie;
}

void dealWithLostMembership(int list_id, int container_id, int ent_id)
{
    Entity    *e;

    e = entFromDbIdEvenSleeping(ent_id);
    if (!e) // player may have disconnected in meantime
        return;
    teamHandleRemoveMembership(e,list_id,0);
    //if (list_id == CONTAINER_TASKFORCES) TaskForceModeChange(e, 0);
    teamlogPrintf(__FUNCTION__, "%s(%i) lost %s:%i", e->name, e->db_id, teamTypeName(list_id), container_id);
}

static int membersChanged(TeamMembers *members,int *ids,int count)
{
    int        i;

    if (members->count != count)
        return 1;
    for(i=0;i<count;i++)
    {
        if (members->ids[i] != ids[i])
            return 1;
    }
    return 0;
}

static void membersUpdate(TeamMembers *members,int *ids,int count,int container_type)
{
    int                i;
    ContainerName    *names;

    members->count = count;
    members->ids    = realloc(members->ids,sizeof(members->ids[0]) * count);
    members->mapIds    = realloc(members->mapIds,sizeof(members->mapIds[0]) * count);
    members->oldmapIds    = realloc(members->oldmapIds,sizeof(members->oldmapIds[0]) * count);

    for(i=0;i<count;i++)
    {
         members->ids[i] = ids[i];
        members->mapIds[i] = members->oldmapIds[i] = -1;
    }

    members->names = realloc(members->names,sizeof(members->names[0]) * count);
    names = dbPlayerNamesFromIds(ids,count);
    for(i=0;i<count;i++)
        strcpy(members->names[i],names?names[i].name:"BAD DATA"); // Can get bad data when the DbServer is shutting down or has already shut down
}

// sanity check on team leader, in case dbserver removed leader from team
static void membersUpdateLeader(TeamMembers* members, int container_type)
{
    Entity            *e;
    int                i, leader = 0;

    e = entFromDbId(members->ids[0]);
    if (!e) 
        return; // only one entity has to take care of this

    // teamup, taskforce election
    if (container_type == CONTAINER_TEAMUPS)
    {
        for (i = 0; i < members->count; i++)
            if (members->ids[i] == members->leader)
                leader = 1;

        if(!leader)
        {
            if(teamLock(e, CONTAINER_TEAMUPS))
            {
                teamlogPrintf(__FUNCTION__, "had to make me leader %s(%i) %s:%i",
                    e->name, e->db_id, teamTypeName(CONTAINER_TEAMUPS), teamGetIdFromEnt(e, CONTAINER_TEAMUPS));
                league_updateTeamLeaderTeam(e, members->leader, e->db_id);
                turnstileMapserver_generateGroupUpdate(members->leader, e->db_id, 0);
                members->leader = e->db_id;
                if( !e->teamup->activetask->sahandle.context )
                {
                    e->teamup->team_mentor = e->db_id;
                    e->teamup->team_level = character_CalcExperienceLevel(e->pchar);
                }
                TaskForcePatchTeamTask(e, e->db_id);
                if(teamUpdateUnlock(e, CONTAINER_TEAMUPS))
                {
                    char* msg = localizedPrintf( e, "isNewLeader", members->names[0] );
                    sendEntsMsg(CONTAINER_TEAMUPS, members->leader, INFO_SVR_COM, 0, "%s%s", DBMSG_CHAT_MSG, msg);
                }
                else
                {
                    FatalErrorf("Failed to unlock teamup container!\nError %d: %s", last_db_error_code, last_db_error);
                }
                TaskForceMakeLeader(e, e->db_id);
                TaskForceSelectTask(e);
            }
            else
            {
                teamlogPrintf(__FUNCTION__, "failed lock %s(%i) %s:%i",
                    e->name, e->db_id, teamTypeName(CONTAINER_TEAMUPS), teamGetIdFromEnt(e, CONTAINER_TEAMUPS));
            }
        }
    }
    // leader of supergroup is verified in sgroup_readDBData
}

void containerHandleGroup(ContainerInfo *ci,ContainerType type)
{
    int            i,send_to_client,members_changed=0;
    Entity        *e;
    TeamMembers    *members = NULL;
    char        *container_data;
    char        cmdbuf[2000];

    container_data = strdup(ci->data);
    // If this was a lock request, don't update the client - will happen when we unlock
    send_to_client = !ci->got_lock;
    teamlogPrintf(__FUNCTION__, "%s:%i send:%i del:%i", teamTypeName(type), ci->id, send_to_client, ci->delete_me);
 
    // ----------
    // if this is a supergroup, track it.

    if( type == CONTAINER_SUPERGROUPS)
    {
        int idSgrp = ci->id;

        if( ci->delete_me )
        {
            sgrp_UnTrackDbId(idSgrp);
        }
        else
        {
            Entity tmpEnt = {0};
            Supergroup *sg = sgrpFromDbId(idSgrp); // try to re-use current entry

            // if no entry for this one currently, create new one.
            if( !sg )
            {
                sg = createSupergroup();
            }

            tmpEnt.supergroup = sg;
            tmpEnt.supergroup_id = idSgrp;
            unpackSupergroup(&tmpEnt,container_data,send_to_client);
            sgrp_TrackDbId(sg, idSgrp);
        }
    }

    if(type == CONTAINER_TASKFORCES && g_ArchitectTaskForce_dbid == ci->id)
        unpackMapTaskForce(ci->id, container_data);

    // do all member updates - can't perform any locked operations here
    for(i=0;i<ci->member_count;i++)
    {
        e = entFromDbIdEvenSleeping(ci->members[i]);
        if(!e)
            continue;

        if(ci->delete_me)
        {
            teamHandleRemoveMembership(e,type,1);
            if (type == CONTAINER_TEAMUPS)
                shardCommStatus(e);
            continue;
        }

        members = NULL;
        switch(type)
        {
            xcase CONTAINER_TEAMUPS:
                e->teamup_id = ci->id;
                unpackTeamup(e,container_data);
                members = &e->teamup->members;
                e->teamlist_uptodate = 0;

            xcase CONTAINER_SUPERGROUPS:
                e->supergroup_id = ci->id;
                unpackSupergroup(e,container_data,send_to_client);
                members = &e->supergroup->members;

            xcase CONTAINER_TASKFORCES:
                e->taskforce_id = ci->id;
                unpackTaskForce(e,container_data,send_to_client);
                members = &e->taskforce->members;
                e->teamlist_uptodate = 0;

            xcase CONTAINER_RAIDS:
                e->raid_id = ci->id;
//                 unpackRaid(e,container_data,send_to_client);
//                 members = &e->raid->members;

            xcase CONTAINER_LEVELINGPACTS:
                e->levelingpact_id = ci->id;
                unpackLevelingPact(e,container_data,send_to_client);
                //check the version.  If the version is out of date, we have to request an update.
                if(e->levelingpact->version < LEVELINGPACT_VERSION)
                {
                    U32 isVillain = SAFE_MEMBER2(e, pchar, playerTypeByLocation) == kPlayerType_Villain;
                    SgrpStat_SendPassthru(e->db_id, 0, "statserver_levelingpact_updateversion %d %d", e->db_id, isVillain);
                }

                members = &e->levelingpact->members;

            xcase CONTAINER_LEAGUES:
                e->league_id = ci->id;
                unpackLeague(e,container_data);
                members = &e->league->members;
                e->leaguelist_uptodate = 0;
                //    function needed to handle member change
            xdefault:
                devassertmsg(0, "unknown group container %d", type);
        }

        if(members && membersChanged(members,ci->members,ci->member_count))
        {
            members_changed = 1;
            membersUpdate(members,ci->members,ci->member_count,type);
            if (type == CONTAINER_TEAMUPS)
                shardCommStatus(e);

            if(type == CONTAINER_LEVELINGPACTS)
            {
                e->levelingpact_update = 1;
                if(e->pl)    //make sure that the badges know what's going on.
                {
                    MarkModifiedBadges(g_hashBadgeStatUsage, e->pl->aiBadges, "*char");
                }
            }
        }
    }
    free(container_data);
    container_data = 0;

    // we can only do locked operations if we have the group unlocked now
    // MAK - an additional complication here:  Any particular ent getting this
    // update may not actually be owned right now (during map transfer).  You may
    // have to do a relay command to get a reliable update.
    if (send_to_client)
    {
        // remove myself if the task force is shutting down
        if (!ci->delete_me && type == CONTAINER_TASKFORCES)
        {
            for (i = 0; i < ci->member_count; i++)
            {
                e = entFromDbIdEvenSleeping(ci->members[i]);
                if (e && e->owned && e->taskforce && e->taskforce->deleteMe)
                {
                    teamDelMember(e, CONTAINER_TASKFORCES, 0, 0);
                    // I'll have invalidated members, etc. after this call
                    return;
                }
            }
        }

        // update leader
        if (!ci->delete_me)
        {
            e = entFromDbIdEvenSleeping(ci->members[0]);
            if (e && e->owned) // if not owned, I'll see this again on next mapserver
            {
                members = teamMembers(e, type);
                if (members)
                {
                    membersUpdateLeader(members, type);
                }
            }
        }

        // task force mode change
        if (ci->delete_me && type == CONTAINER_TASKFORCES)
        {
            for(i=0;i<ci->member_count;i++)
            {
                e = entFromDbIdEvenSleeping(ci->members[i]);
                if (e)
                    TaskForceModeChange(e, 0); // this handled non-owned ents
            }
        }

        // update map id's
        if (members_changed && type == CONTAINER_TEAMUPS)
        {
            int j;
            for(j=0; j<members->count; j++)
            {
                char buf[256];
                sprintf( buf, "team_map_relay %i", members->ids[j] );
                serverParseClient( buf, NULL );
            }
        }

        // update map id's
        if (members_changed && type == CONTAINER_LEAGUES)
        {
            int j;
            for(j=0; j<members->count; j++)
            {
                char buf[256];
                sprintf( buf, "league_map_relay %i", members->ids[j] );
                serverParseClient( buf, NULL );
            }
        }

        if(members_changed && type == CONTAINER_SUPERGROUPS)
        {
            int j;
            Entity * super_member;
            for(j=0; j<members->count; j++)
            {
                super_member = entFromDbId( members->ids[j]);
                if( super_member )
                    super_member->supergroup_update = true;
            }
        }

        // update mission map with new task force leader
        if (type == CONTAINER_TASKFORCES && members)
        {
            e = entFromDbId(members->leader);
            if (e && e->teamup && e->teamup->map_id)
            {
                sprintf(cmdbuf, "cmdrelay\nmissionchangeowner %i %i %i %i %i",    e->db_id, SAHANDLE_ARGSPOS(e->teamup->activetask->sahandle) );
                sendToServer(e->teamup->map_id, cmdbuf);
                // MAK - ok for this message to happen more often than necessary, no other good place to put it
            }
        }


        // mission status checks need to happen after the atomic update above
        if (type == CONTAINER_TEAMUPS)
        {
            for(i=0;i<ci->member_count;i++)
            {
                e = entFromDbId(ci->members[i]);
                if (!e || !e->owned || !(e->teamup_id == ci->id)) // ent may have been removed from teamup above
                    continue;
                MissionTeamupUpdated(e);
                MissionSendRefresh(e);
                e->team_buff_full_update = true; // Make sure his buffs get sent to his new teammates
                e->teamlist_uptodate = 0;
            }
        }
        if (type == CONTAINER_LEAGUES)
        {
            for(i=0;i<ci->member_count;i++)
            {
                e = entFromDbId(ci->members[i]);
                if (!e || !e->owned || !(e->league_id == ci->id)) // ent may have been removed from teamup above
                    continue;
                e->team_buff_full_update = true; // Make sure his buffs get sent to his new teammates
                e->leaguelist_uptodate = 0;
            }
        }

        //If your team is coed (Heroes and Villains) and you are not on a coed server, drop from the team.
        if ( type == CONTAINER_TEAMUPS && !OnArenaMap()  )
        {
            for(i=0;i<ci->member_count;i++) 
            {
                int kickflag;

                e = entFromDbId(ci->members[i]);
                if (!e || !e->owned || !(e->teamup_id == ci->id)) // ent may have been removed from teamup above
                    continue;
                kickflag = team_MinorityQuit( e,server_state.allowHeroAndVillainPlayerTypesToTeamUp,server_state.allowPrimalAndPraetoriansToTeamUp );
                if( kickflag )
                {
                    if( kickflag == MINORITYQUIT_FACTION )
                    {
                        sendEntsMsg(CONTAINER_TEAMUPS, ci->id, INFO_SVR_COM, 0, "%s%s", DBMSG_CHAT_MSG, localizedPrintf(e,"PlayerDroppedFromCoedTeam", e->name));
                        sendChatMsg( e, localizedPrintf( e,"YouDroppedFromCoedTeam" ), INFO_SVR_COM, 0 );
                    }
                    else if( kickflag == MINORITYQUIT_UNIVERSE )
                    {
                        sendEntsMsg(CONTAINER_TEAMUPS, ci->id, INFO_SVR_COM, 0, "%s%s", DBMSG_CHAT_MSG, localizedPrintf(e,"PlayerDroppedFromCoUniverseTeam", e->name));
                        sendChatMsg( e, localizedPrintf( e,"YouDroppedFromCoUniverseTeam" ), INFO_SVR_COM, 0 );
                    }
                    else
                    {
                        // We don't know why they were kicked, use the default message for now.
                        sendEntsMsg(CONTAINER_TEAMUPS, ci->id, INFO_SVR_COM, 0, "%s%s", DBMSG_CHAT_MSG, localizedPrintf(e,"PlayerDroppedFromCoedTeam", e->name));
                        sendChatMsg( e, localizedPrintf( e,"YouDroppedFromCoedTeam" ), INFO_SVR_COM, 0 );
                    }

                    if (OnMissionMap())
                        MissionKickPlayer(e);
                }
            }
        }

        //If your team is coed (Heroes and Villains) and you are not on a coed server, drop from the team.
        if ( type == CONTAINER_LEAGUES && !OnArenaMap()  )
        {
            for(i=0;i<ci->member_count;i++) 
            {
                int kickflag;

                e = entFromDbId(ci->members[i]);
                if (!e || !e->owned || !(e->league_id == ci->id)) // ent may have been removed from league above
                    continue;
                kickflag = league_MinorityQuit( e,server_state.allowHeroAndVillainPlayerTypesToTeamUp,server_state.allowPrimalAndPraetoriansToTeamUp );
                if( kickflag )
                {
                    if( kickflag == MINORITYQUIT_FACTION )
                    {
                        sendEntsMsg(CONTAINER_LEAGUES, ci->id, INFO_SVR_COM, 0, "%s%s", DBMSG_CHAT_MSG, localizedPrintf(e,"PlayerDroppedFromCoedLeague", e->name));
                        sendChatMsg( e, localizedPrintf( e,"YouDroppedFromCoedLeague" ), INFO_SVR_COM, 0 );
                    }
                    else if( kickflag == MINORITYQUIT_UNIVERSE )
                    {
                        sendEntsMsg(CONTAINER_LEAGUES, ci->id, INFO_SVR_COM, 0, "%s%s", DBMSG_CHAT_MSG, localizedPrintf(e,"PlayerDroppedFromCoUniverseLeague", e->name));
                        sendChatMsg( e, localizedPrintf( e,"YouDroppedFromCoUniverseLeague" ), INFO_SVR_COM, 0 );
                    }
                    else
                    {
                        // We don't know why they were kicked, use the default message for now.
                        sendEntsMsg(CONTAINER_LEAGUES, ci->id, INFO_SVR_COM, 0, "%s%s", DBMSG_CHAT_MSG, localizedPrintf(e,"PlayerDroppedFromCoedLeague", e->name));
                        sendChatMsg( e, localizedPrintf( e,"YouDroppedFromCoedLeague" ), INFO_SVR_COM, 0 );
                    }

                    if (OnMissionMap())
                        MissionKickPlayer(e);
                }
            }
        }
    }

    if (type == CONTAINER_TEAMUPS)
    {
        for (i = 0; i < ci->member_count; i++)
        {
            e = entFromDbIdEvenSleeping(ci->members[i]);
            if (!e || !e->owned || e->teamup_id != ci->id) // ent may have been removed from teamup above
                continue;
            if (e->revokeBadTipsOnTeamupLoad)
            {
                alignmentshift_revokeAllBadTips(e);
                e->revokeBadTipsOnTeamupLoad = 0;
                // BEWARE:  The above call can possibly make e->teamup NULL!
                // As of this writing, nothing else can happen within this function after this call.
            }
        }
    }
    else if (type == CONTAINER_LEVELINGPACTS)
    {
        int j;
        EArray32 *ea = _alloca(EARRAY32_HEADER_SIZE + sizeof(int)*ci->member_count); // hack until i make temporary earrays
        int *memberids = (int*)ea->values;
        ea->count = 0;
        ea->size = ci->member_count;
        ea->flags = EARRAY_FLAG_CUSTOMALLOC;

        for(i=0;i<ci->member_count;i++) 
        {
            e = entFromDbId(ci->members[i]);

            if(e && e->owned && e->levelingpact) // ent may have been removed from teamup above
            {
                int sortedidx; 
                int oldLevel = character_CalcExperienceLevel(e->pchar);
                int iXPShouldBe = ((e->levelingpact->count)?e->levelingpact->experience/e->levelingpact->count:0);
                int iXPReceived = character_IncreaseExperienceNoDebt(e->pchar, iXPShouldBe);
                int totalTimeToLevel = ((e->levelingpact->count)?e->levelingpact->timeLogged/e->levelingpact->count:0);
                e->last_levelingpact_time = totalTimeToLevel;
                stat_AddXPReceived(e, iXPReceived);
                if(iXPReceived > 0) // don't mention fallout from rounding errors
                {
                    //sendInfoBox(e, INFO_REWARD, "ExperienceYouReceivedLevelingPact", iXPReceived);
                    if(character_CalcExperienceLevelByExp(iXPShouldBe) > oldLevel)
                        levelupApply(e, oldLevel);
                }
                if(!eaiSize(&memberids))
                    for(j = 0; j < ci->member_count; j++)
                        eaiSortedPush(&memberids, ci->members[j]);
                sortedidx = eaiFind(&memberids, e->db_id);
                if(    devassert(AINRANGE(sortedidx, e->levelingpact->influence)) &&
                    e->levelingpact->influence[sortedidx] )
                {
                    SgrpStat_SendPassthru(e->db_id, 0, "statserver_levelingpact_getinfluence %d", e->db_id);
                }
                //Right now we don't need this to be updated every time we get xp.  Maybe sometime in the future.
                //e->levelingpact_update = 1;
            }
        }
    }
}

U32 dealWithContainer(ContainerInfo *ci,int type)
{
    if(type == CONTAINER_ENTS)
    {
        return containerHandleEntity(ci);
    }
    else if(type == CONTAINER_MAPGROUPS)
    {
        containerHandleMapGroup(ci);
        return 1;
    }
    else if(CONTAINER_IS_GROUP(type))
    {
        containerHandleGroup(ci, type);
        return 1;
    }
    else if (type == CONTAINER_AUTOCOMMANDS)
    {
        containerHandleAutoCommand(ci);
        return 1;
     }
    else if(type == CONTAINER_ARENAEVENTS ||
            type == CONTAINER_ARENAPLAYERS ||
            type == CONTAINER_BASE ||
            type == CONTAINER_BASERAIDS ||
            type == CONTAINER_ITEMSOFPOWER ||
            type == CONTAINER_ITEMOFPOWERGAMES ||
            type == CONTAINER_SGRPSTATS ||
            type == CONTAINER_SGRAIDINFO )
    {
        return 0;
    }
    else
    {
        devassertmsg(0, "unknown container type %d", type);
        return 0;
    }
}

// look at reflect info, and make a callback for each dbid mentioned
void SendReflectionToEnts(int* ids, int deleting, ContainerReflectInfo** reflectinfo, void (*func)(int*, int, Entity*))
{
    int i, count = eaSize(&reflectinfo);
    U32 dbid;
    Entity* ent;

    for (i = 0; i < count; i++)
    {
        if (reflectinfo[i]->target_list == CONTAINER_ENTS)
            dbid = reflectinfo[i]->target_cid;
        else
            dbid = reflectinfo[i]->target_dbid; // otherwise we should have been advised of our dbid
        ent = entFromDbIdEvenSleeping(dbid);
        if (ent)
            func(ids, deleting || reflectinfo[i]->del, ent);

    }
}

void dealWithContainerReflection(int list_id, int* ids, int deleting, Packet* pak, ContainerReflectInfo** reflectinfo)
{
    if (list_id == CONTAINER_BASERAIDS)
        RaidHandleReflection(ids, deleting, pak, reflectinfo);
    else if (list_id == CONTAINER_ITEMSOFPOWER)
        ItemOfPowerHandleReflection(ids, deleting, pak, reflectinfo);
    else if (list_id == CONTAINER_ITEMOFPOWERGAMES)
        ItemOfPowerGameHandleReflection(ids, deleting, pak, reflectinfo);
    else
        Errorf("dealWithContainerReflection: unhandled list %i", list_id);
}

void dealWithEntAck(int list_id,int id)
{
}

void dealWithShardAccountAck(int list_id,int id)
{
}

void forceLogout(int db_id,int reason)
{
    NetLink    *link;
    char    *buf;

    link = findLinkById(db_id);
    if (!link)
    {
        Entity *e = entFromDbIdEvenSleeping(db_id);

        if (e)
            svrCompletelyUnloadPlayerAndMaybeLogout(e, "no link", 0);
        else
            dbSendPlayerDisconnect(db_id, 0, 0);
    }
    else
    {
        char * reasons[] = 
        {
                "S_ALL_OK",                        // 0
                "S_DATABASE_FAIL",
                "S_INVALID_ACCOUNT",
                "S_INCORRECT_PWD",
                "S_ACCOUNT_LOAD_FAIL",
                "S_LOAD_SSN_ERROR",                // 5
                "S_NO_SERVERLIST",
                "S_ALREADY_LOGIN",
                "S_SERVER_DOWN",
                "S_INCORRECT_MD5Key",
                "S_NO_LOGININFO",                // 10 If SessionId is not exists.
                "S_KICKED_BY_WEB",
                "S_UNDER_AGE",
                "S_KICKED_DOUBLE_LOGIN",
                "S_ALREADY_PLAY_GAME",
                "S_LIMIT_EXCEED",                // 15
                "S_SERVER_CHECK",
                "S_MODIFY_PASSWORD",
                "S_NOT_PAID",
                "S_NO_SPECIFICTIME",
                "S_SYSTEM_ERROR",                // 20
                "S_ALREADY_USED_IP",
                "S_BLOCKED_IP",
                "S_VIP_ONLY",
                "S_DELETE_CHRARACTER_OK",
                "S_CREATE_CHARACTER_OK",        // 25
                "S_INVALID_NAME",
                "S_INVALID_GENDER",
                "S_INVALID_CLASS",
                "S_INVALID_ATTR", // Unknown Protocol ID
                "S_MAX_CHAR_NUM_OVER",            // 30    
                "S_TIME_SERVER_LIMIT_EXCEED",
                "S_INVALID_USER_STATUS",
                "S_INCORRECT_AUTHKEY",
                "S_INVALID_CHARACTER",
        };

        if( reason >= 0 && reason < ARRAY_SIZE(reasons) )
            buf = localizedPrintf(NULL,reasons[reason]);
        else
            buf = localizedPrintf(NULL,"UnknownAuthCode",reason); 
        svrForceLogoutLink(link,buf,KICKTYPE_FROMDBSERVER);
    }

    teamlogPrintf(__FUNCTION__, "%i reason %i", db_id, reason);
}

void returnAllPlayers(int disconnect,char *msg)
{
    teamlogPrintf(__FUNCTION__, "");

    if(g_base.rooms)
        sgroup_SaveBase(&g_base, NULL, 1);

    if (disconnect)
    {
        printf("got shutdown command from dbserver. syncing players and quitting\n");
        if (svrSendAllPlayerEntsToDb())
            dbWaitContainersAcked();
        printf("got ent sync from dbserver\n");
        svrForceLogoutAll(msg);
        groupCleanUp(); // Clean up ctris that might be in shared memory
        svrLogPerf();
        logSendDisconnect(30);    // remote logs (logserver)
        logShutdownAndWait();    // local logs
        exit(0);
    }
    else
    {
        char *msg = NULL;
        estrConcatf(&msg,"returnAllPlayers() !disconnect, msg: %s",(msg?msg:"NULL"));
        MissionShutdown(msg);    // need to tell owner of mission about shutdown
    }
}

void containerSetDbCallbacks()
{
    //db_state.container_update_cb    = dealWithContainer;
    //db_state.container_broadcast_cb    = dealWithBroadcast;
    //db_state.container_ack_cb        = dealWithEntAck;
    //db_state.lost_membership_cb        = dealWithLostMembership;
    //db_state.force_logout_cb        = forceLogout;
    //db_state.shutdown_cb            = returnAllPlayers;
}

void dealWithContainerRelay(Packet* pak, U32 cmd, U32 listid, U32 cid)
{
    Errorf("mapserver received dealWithContainerRelay");
}

void dealWithContainerReceipt(U32 cmd, U32 listid, U32 cid, U32 user_cmd, U32 user_data, U32 err, char* str, Packet* pak)
{
    switch (listid)
    {
    case CONTAINER_BASERAIDS:
        RaidHandleReceipt(cmd, listid, cid, user_cmd, user_data, err, str, pak);
    xcase CONTAINER_ITEMSOFPOWER:
        RaidHandleReceipt(cmd, listid, cid, user_cmd, user_data, err, str, pak);  //Just forwarding error msg on to the player, so use Raid's function
    xcase CONTAINER_ITEMOFPOWERGAMES:
        RaidHandleReceipt(cmd, listid, cid, user_cmd, user_data, err, str, pak);  //Just forwarding error msg on to the player, so use Raid's function
    xdefault:
        Errorf("got unexpected container receipt");
    }
}
