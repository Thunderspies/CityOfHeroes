#include <string.h>
#include "entity/entity.h"
#include "entity/motion.h"
#include "cmdparse/cmdcommon.h"
#include <utilitieslib/network/netcomp.h>
#include <utilitieslib/assert/assert.h>
#include "seq/seq.h"
#include <utilitieslib/utils/utils.h>
#include "cmdparse/cmdserver.h"
#include "group/groupnetdb.h"
//#include "entaiprivate.h"
#include "gameComm/trayServer.h"
#include "varutils.h"
#include "svr/svr_base.h"
#include "gameComm/sendToClient.h"
#include <utilitieslib/utils/file.h>
#include "group/groupnetsend.h"
#include "dbcomm/dbcomm.h"
#include "dbcomm/dbcontainer.h"
#include "ai/entai.h"
#include "entserver.h"
#include "svr/svr_player.h"
#include "entity//costume.h"
#include "entity/power_customization.h"
#include "entity/entStrings.h"
#include "gameComm/wdwbase.h"
#include "entity/character_base.h"
#include "character_net_server.h"
#include "entity/attrib_net.h"
#include "entGameActions.h"
#include "friends.h"
#include "storyarc/storyarcinterface.h"
#include "gameComm/Npc.h"
#include "keybinds.h"
#include "generator/encounter.h"
#include "entity/entPlayer.h"
#include "gameSys/dooranim.h"
#include "storyarc/SouvenirClue.h"
#include "player/pmotion.h"
#include "gridcoll/megaGrid.h"
#include "storyarc/TeamTask.h"
#include "Reward.h"
#include "dbcomm/logcomm.h"
#include "storyarc/storyarcutil.h"
#include "generator/entgen.h"
#include <utilitieslib/network/net_packetutil.h>
#include "generator/encounter.h"
#include "gameComm/trayCommon.h"
#include "seq/seqstate.h"
#include "script/scriptengine.h"
#include "beacon/beacon.h"
#include "gridcoll/grid.h"
#include "entity/entity_enum.h"
#include "entity/entPlayer.h"
#include "NovodeX/NwWrapper.h"
#include "NovodeX/NwRagdoll.h"
#include "gameSys/petarena.h"
#include "seq/seqanimate.h"
#include "seq/seqskeleton.h"
#include "seq/seqregistration.h"
#include <utilitieslib/utils/timing.h>
#include "bases/bases.h"
#include "container_diff.h"
#include "comm_backend.h"
#include "gridcoll/megaGridPrivate.h"
#include "auth/authUserData.h"
#include "storyarc/storyarcprivate.h"
#include "gameSys/sgraid.h"
#include <utilitieslib/components/HashFunctions.h>
#include <utilitieslib/utils/Quat.h>
#include "storyarc/playerCreatedStoryarcServer.h"
#include "storyarc/contact.h"
#include "character_pet.h"
#include "comm_game.h"
#include "dbcomm/logcomm.h"
#include "gameData/BodyPart.h"

Entity*        entities[MAX_ENTITIES_PRIVATE];
U8            entity_state[MAX_ENTITIES_PRIVATE];    //ies_used[MAX_ENTITIES];
int            entities_max,num_ents;
EntityInfo    entinfo[MAX_ENTITIES_PRIVATE];
Vec3        entpos[MAX_ENTITIES_PRIVATE];

int            ent_deletes[MAX_DELETES];
int            ent_delete_ids[MAX_DELETES];
int            ent_delete_count;

Entity *entFromLoginCookie(U32 cookie)
{
    int        i;
    Entity    *e;

    // cookies of 0 and 1 are reserved.
    if(cookie==0 || cookie==1)
        return 0;

    for(i=1;i<entities_max;i++)
    {
        e = entities[i];
        // do not check just sleeping, when resuming a dead link, the entity is not sleeping!
        if ((entity_state[i]) &&
            ENTTYPE_BY_ID(i) == ENTTYPE_PLAYER &&
            e->dbcomm_state.login_cookie == cookie)
        {
            //clear out the cookie here, just in case?
            // can't clear it out here!  It's needed later!
            //e->dbcomm_state.login_cookie = 0;
            return e;
        }
    }
    return 0;
}


int entGetId(Entity* e)
{
    assert(e && e->owner);
    return e->owner;
}

//extern Grid    ent_grid;
//extern Grid    ent_collision_grid;
//extern Grid ent_player_grid;

int entgrid_check,entgrid_moved;

int proxMessage = 0;

#pragma optimize( "gtp", on )
static void entGridUpdateHelper(Entity* e, Grid* grid, GridIdxList** grid_idx, int* last_grid_ipos, float dxyz, int shift, int nodetype)
{
    #define MAXRANGE (32768.f - 2000.f)
    int        i,t,newgrid=0,ipos[3];
    Vec3    min,max,dv;
    const float*    pos;

    pos = ENTPOS(e);

    //entgrid_check++;

    //for(i=0;i<3;i++)
    //{
    //    if (!(pos[i] > -MAXRANGE && pos[i] < MAXRANGE))
    //    {
    //        printf("entpos err: %f %f %f\n",pos[0],pos[1],pos[2]);
    //        zeroVec3(pos);
    //        return;
    //    }
    //}

    qtruncVec3(pos,ipos);

    for(i=0;i<3;i++)
    {
        t = ipos[i] >> shift;
        if (last_grid_ipos[i] != t)
            newgrid = 1;
        last_grid_ipos[i] = t;
    }

    if(*grid_idx && !newgrid)
        return;

    //entgrid_moved++;
    gridClearAndFreeIdxList(grid,grid_idx);
    setVec3(dv,dxyz,dxyz,dxyz);
    subVec3(pos,dv,min);
    addVec3(pos,dv,max);

    PERFINFO_AUTO_START("gridAdd", 1);
        if(!gridAdd(grid,(void *)(intptr_t)((e->owner + 1) << 2), min, max, nodetype, grid_idx))
        {
            entUpdatePosInstantaneous(e, zerovec3);
        }
    PERFINFO_AUTO_STOP();
}

void entInitGrid(Entity* e, EntType entType)
{
    F32 dxyz;
    F32 collisionSize;

    // Init vis grid.

    if(entType == ENTTYPE_PLAYER || entType == ENTTYPE_CRITTER)
    {
        dxyz = max(MIN_PLAYER_CRITTER_RADIUS, ENTFADE(e));
    }
    else
    {
        dxyz = ENTFADE(e);
    }

    if(!server_state.ent_vis_distance_scale)
    {
        server_state.ent_vis_distance_scale = 1;
    }

    dxyz *= server_state.ent_vis_distance_scale;

    if(!e->megaGridNode)
    {
        e->megaGridNode = createMegaGridNode((void*)(intptr_t)e->owner, dxyz, 1);
    }
    else
    {
        mgUpdateNodeRadius(e->megaGridNode, dxyz, 1);
    }

    // Init collision grid.

    if( e->seq )
    {
        collisionSize = MAX(e->seq->type->capsuleSize[0], e->seq->type->capsuleSize[1]);
        collisionSize = MAX(collisionSize, e->seq->type->capsuleSize[2]);
        collisionSize += 32;
    }
    else
    {
        collisionSize = 32;
    }

    if(!e->megaGridNodeCollision)
    {
        e->megaGridNodeCollision = createMegaGridNode((void*)(intptr_t)e->owner, collisionSize, 0);
    }
    else
    {
        mgUpdateNodeRadius(e->megaGridNodeCollision, collisionSize, 0);
    }

    if(entType == ENTTYPE_PLAYER)
    {
        if(!e->megaGridNodePlayer)
        {
            e->megaGridNodePlayer = createMegaGridNode((void*)(intptr_t)e->owner, 512, 0);
        }
        else
        {
            mgUpdateNodeRadius(e->megaGridNodePlayer, 512, 0);
        }
    }
}

void entGridUpdate(Entity* e, EntType entType)
{
    if(!e->megaGridNode)
    {
        entInitGrid(e, entType);
    }

    PERFINFO_AUTO_START("mgUpdate", 1);
        mgUpdate(0, e->megaGridNode, ENTPOS(e));
    PERFINFO_AUTO_STOP();

    // Bouncy land Increase grid size by largest collision axis

    PERFINFO_AUTO_START("mgUpdateCollision", 1);
        mgUpdate(1, e->megaGridNodeCollision, ENTPOS(e));
    PERFINFO_AUTO_STOP();

    if (entType == ENTTYPE_PLAYER)
    {
        PERFINFO_AUTO_START("mgUpdatePlayer", 1);
            mgUpdate(2, e->megaGridNodePlayer, ENTPOS(e));
        PERFINFO_AUTO_STOP();
    }

    //if(ENTSLEEPING(e) || !e->grid_idx || 1 || (global_state.global_frame_count + e->owner) % 5 == 0)
    //{
    //    PERFINFO_AUTO_START("ent_grid", 1);
    //        entGridUpdateHelper(e, &ent_grid, &e->grid_idx, e->last_grid_ipos, dxyz, 7, 3);
    //    PERFINFO_AUTO_STOP();
    //}

    //PERFINFO_AUTO_START("ent_collision_grid", 1);
    //    entGridUpdateHelper(e, &ent_collision_grid, entType, &e->grid_idx_collision, e->last_grid_ipos_collision, collisionSize, 7, 4);
    //PERFINFO_AUTO_STOP();

    //if (entType == ENTTYPE_PLAYER)
    //{
    //    PERFINFO_AUTO_START("ent_player_grid", 1);
    //        entGridUpdateHelper(e, &ent_player_grid, &e->grid_idx_player, e->last_grid_ipos_player, 500, 7, 3);
    //    PERFINFO_AUTO_STOP();
    //}
}
#pragma optimize( "", on )

// ENTTYPE_NOT_SPECIFIED is ok; optional result, size = MAX_ENTITIES_PRIVATE
// if velpredict is set, distance is checked to where entity will be in velpredict timesteps
int entWithinDistance(const Mat4 pos, F32 distance, Entity** result, EntType enttype, int velpredict)
{
    int i;
    int count;
    Entity* entArray[MAX_ENTITIES_PRIVATE];
    int entCount;

    // search for points within distance
    entCount = mgGetNodesInRange(enttype == ENTTYPE_PLAYER ? 2 : 0, pos[3], (void **)entArray, 0);

    // Convert to ent pointers, count players within range
    count = 0;
    for(i = 0; i < entCount; i++){
        const Vec3* posEnt;
        Mat4 mat;
        Vec3 vel;

        Entity* proxEnt = entities[(int)(intptr_t)entArray[i]];
        if (enttype != ENTTYPE_NOT_SPECIFIED && enttype != ENTTYPE(proxEnt)) continue;

        if (velpredict && proxEnt->motion)
        {
            copyVec3(ENTPOS(proxEnt), mat[3]);
            scaleVec3(proxEnt->motion->vel, velpredict, vel);
            addVec3(mat[3], vel, mat[3]);
            posEnt = mat;
        }
        else
            posEnt = ENTMAT(proxEnt);

        if (posWithinDistance(posEnt, pos, distance))
        {
            entArray[count] = proxEnt;
            count++;
        }
    }

    if (result)
    {
        for (i = 0; i < count; i++)
            result[i] = entArray[i];
    }

    return count;
}

int entCount()
{
    return num_ents;
}

static void entNetDelete(Entity *ent)
{
int        idx;

    idx = ent->owner;
    if (ent_delete_count < MAX_DELETES-1) {
        ent_delete_ids[ent_delete_count] = ent->id;
        ent_deletes[ent_delete_count++] = idx;
    }
}

void entSetPlayerTypeByLocation(Entity *e, PlayerType type)
{
    STATIC_ASSERT(kPlayerType_Count == 2)

    if(e && e->pchar && e->pchar->playerTypeByLocation != type)
    {
        e->pchar->playerTypeByLocation = type;

        LOG_ENT_OLD( e, "GoingRogue:InfluenceType Influence type set to %s", type ? "Villain" : "Hero");

        // Need to push this up to the DbServer so the character cache gets updated on all maps
        dbSyncPlayerChangeInfluenceType(e->db_id, e->pchar->playerTypeByLocation);
    }
}

static void entCheckAndUpdateWindowColors(Entity *e, PlayerType oldType, PlayerType newType, PraetorianProgress oldProgress, PraetorianProgress newProgress)
{
    U32 oldColor = 0;
    U32 newColor = 0;

    // If was Praetorian, old colour is grey
    // If was Primal hero, old colour is blue
    // If was Primal villain, old colour is red
    // If is Praetorian, new colour is grey
    // If is Primal hero, new colour is blue
    // If is Primal villain, new colour is red
    // If old and new colours are different
    //   Go through all the windows
    //     If window is old colour, change it to new colour
    // If any windows have changed
    // Send new colours for all windows to client

    // Praetorians are grey whatever their alignment. Non-Praetorians are blue
    // for heroes and red for villains.
    if (oldProgress == kPraetorianProgress_Tutorial 
        || oldProgress == kPraetorianProgress_Praetoria 
        || oldProgress == kPraetorianProgress_TravelHero 
        || oldProgress == kPraetorianProgress_TravelVillain)
    {
        oldColor = DEFAULT_PRAETORIAN_WINDOW_COLOR;
    }
    else if (oldType == kPlayerType_Hero)
    {
        oldColor = DEFAULT_HERO_WINDOW_COLOR;
    }
    else if (oldType == kPlayerType_Villain)
    {
        oldColor = DEFAULT_VILLAIN_WINDOW_COLOR;
    }

    if (newProgress == kPraetorianProgress_Tutorial 
        || newProgress == kPraetorianProgress_Praetoria 
        || newProgress == kPraetorianProgress_TravelHero 
        || newProgress == kPraetorianProgress_TravelVillain)
    {
        newColor = DEFAULT_PRAETORIAN_WINDOW_COLOR;
    }
    else if (newType == kPlayerType_Hero)
    {
        newColor = DEFAULT_HERO_WINDOW_COLOR;
    }
    else if (newType == kPlayerType_Villain)
    {
        newColor = DEFAULT_VILLAIN_WINDOW_COLOR;
    }

    // If the colours don't match we need to tell the client to change.
    if(oldColor && newColor && oldColor != newColor && e && e->client && e->client->link)
    {
        START_PACKET(pak, e, SERVER_CHANGE_WINDOW_COLORS);
        pktSendBitsAuto(pak, oldColor);
        pktSendBitsAuto(pak, newColor);
        END_PACKET
    }
}

void entSetPlayerType(Entity *e, PlayerType type)
{
    if(e && e->pl && e->pl->playerType != type)
    {
        PlayerType oldType = e->pl->playerType;
        entSetPlayerTypeByLocation(e, type);
        e->pl->playerType = type;
        e->pl->playerSubType = kPlayerSubType_Normal;
        e->team_update = 1;

        //When you change from hero to villain, your animations can change, too
        calcSeqTypeName( e->seq, e->pl ? e->pl->playerType : 0, e->pl ? ENT_IS_IN_PRAETORIA(e) : 0, ENTTYPE(e)==ENTTYPE_PLAYER ? 1 : 0 );
        EncounterEntUpdateState(e, 0);
        entCheckAndUpdateWindowColors(e, oldType, type, e->pl->praetorianProgress, e->pl->praetorianProgress);

        // We are not really re-naming them, 
        // but we need to clear the name-cache because it also stores playertype
        dbSyncPlayerChangeType(e->db_id, e->pl->playerType );

        ContactMarkAllDirty(e, StoryInfoFromPlayer(e));

        TaskAbandonInvalidContacts(e);

        e->pl->currentAccessibleContactIndex = AccessibleContacts_FindFirst(e);
    }
}

void entSetPlayerSubType(Entity *e, PlayerSubType subType)
{
    if(e && e->pl && e->pl->playerSubType != subType)
    {
        e->pl->playerSubType = subType;
        e->team_update = 1;

        // Need to push this up to the DbServer so the character cache gets updated on all maps
        dbSyncPlayerChangeSubType(e->db_id, e->pl->playerSubType);
    }
}

void entSetPraetorianProgress(Entity *e, PraetorianProgress progress)
{
    if(e && e->pl && e->pl->praetorianProgress != progress)
    {
        PraetorianProgress oldProgress = e->pl->praetorianProgress;
        e->pl->praetorianProgress = progress;
        e->team_update = 1;
        entCheckAndUpdateWindowColors(e, e->pl->playerType, e->pl->playerType, oldProgress, progress);

        // Need to push this up to the DbServer so the character cache gets updated on all maps
        dbSyncPlayerChangePraetorianProgress(e->db_id, e->pl->praetorianProgress);

        e->pl->currentAccessibleContactIndex = AccessibleContacts_FindFirst(e);
    }
}




//For now we just hash the string into a gang ID.  In the future we might want to have a master list if legal gang names to prevent misspelling 
int entGangIDFromGangName( const char * gangName )
{
    if( !gangName )
        return 0;
    if (strIsNumeric(gangName))
    {
        return atoi(gangName);
    }
    else
    {
        int gangID = hashCalc(gangName, (ub4)strlen(gangName), 0xa74d94e3);
        gangID |= GANG_ID_IS_STATIC;
        return gangID;
    }
}

void entSetOnGang(Entity* e, int iGang)
{
    if(e && e->pchar && e->pchar->iGangID != iGang && !e->pchar->iGandID_Perm)
    {
        e->pchar->iGangID = iGang;
        e->team_update = 1;
        EncounterEntUpdateState(e, 0);
    }
}


void entSetOnTeam(Entity* e, int iTeam)
{
    if(e && e->pchar && e->pchar->iAllyID != iTeam)
    {

        e->pchar->iAllyID = iTeam;
        e->team_update = 1;
        
        if( ENTTYPE(e)==ENTTYPE_PLAYER)
            character_UpdateAllPetAllyGang(e->pchar);

        //When you change from hero to villain, your animations can change, too
//        calcSeqTypeName( e->seq, e->pchar ? e->pchar->iAllyID : 0, ENTTYPE(e)==ENTTYPE_PLAYER ? 1 : 0 );
        EncounterEntUpdateState(e, 0);
        
        if (e->storyInfo)
        {
            ContactMarkAllDirty(e, StoryInfoFromPlayer(e));
        }
    }
}

void entSetOnTeamHero(Entity* e, int onTeam)
{
    if(e && e->pchar)
    {
        if(onTeam)
        {
            entSetOnTeam(e, kAllyID_Hero);
        }
        else if(e->pchar->iAllyID==kAllyID_Hero)
        {
            entSetOnTeam(e, kAllyID_None);
        }
    }
}

void entSetOnTeamMonster(Entity* e, int onTeam)
{
    if(e && e->pchar)
    {
        if(onTeam)
        {
            entSetOnTeam(e, kAllyID_Monster);
        }
        else if(e->pchar->iAllyID==kAllyID_Monster)
        {
            entSetOnTeam(e, kAllyID_None);
        }
    }
}

void entSetOnTeamVillain(Entity* e, int onTeam)
{
    if(e && e->pchar)
    {
        if(onTeam)
        {
            entSetOnTeam(e, kAllyID_Villain);
        }
        else if(e->pchar->iAllyID==kAllyID_Villain)
        {
            entSetOnTeam(e, kAllyID_None);
        }
    }
}

void entSetOnTeamMission(Entity* e)
{
    if(e && e->pchar && g_activemission)
    {
        VillainGroupAlly newTeam = villainGroupGetAlly(g_activemission->missionGroup);
        if (newTeam == VG_ALLY_HERO)
            entSetOnTeam(e, kAllyID_Hero);
        else if (newTeam == VG_ALLY_VILLAIN)
            entSetOnTeam(e, kAllyID_Villain);
        else if (newTeam == VG_ALLY_MONSTER)
            entSetOnTeam(e, kAllyID_Monster);
        else
        {
            char error[1024];
            snprintf(error, 1024, "entSetOnTeamMission: VG_NONE, task: %s, map: %s, vg: %i", g_activemission->task->filename, db_state.map_name, g_activemission->missionGroup);
            devassertmsg(NULL, "%s", error);
        }
    }
}

void entSetOnTeamPlayer(Entity* e)
{
    if(e && e->pchar && g_activemission)
    {
        if (g_activemission->missionAllyGroup == ENC_FOR_VILLAIN)
            entSetOnTeam(e, kAllyID_Villain);
        else if (g_activemission->missionAllyGroup == ENC_FOR_HERO)
            entSetOnTeam(e, kAllyID_Hero);
    }
}

void entSetSendOnOddSend(Entity* e, int set)
{
    if(ENTINFO(e).send_on_odd_send != set)
    {
        SET_ENTINFO(e).send_on_odd_send = set;
        e->send_on_odd_send_update = 1;
    }
}

void entSetProcessRate(int id, EntityProcessRate entProcessRate)
{
    Entity* e = entities[id];

    if(ENTINFO_BY_ID(id).entProcessRate != entProcessRate)
    {
        SET_ENTINFO_BY_ID(id).entProcessRate = entProcessRate;

        // This could be smarter and actually re-initialize the total and keep old values.

        memset(&e->dist_history, 0, sizeof(e->dist_history));

        switch(entProcessRate)
        {
            case ENT_PROCESS_RATE_ALWAYS:
                e->dist_history.maxIndex = ARRAY_SIZE(e->dist_history.dist);
                break;
            case ENT_PROCESS_RATE_ON_SEND:
                e->dist_history.maxIndex = 8;
                break;
            case ENT_PROCESS_RATE_ON_ODD_SEND:
                e->dist_history.maxIndex = 4;
                break;
            default:
                assert(0);
                break;
        }
    }
}

void entReloadSequencersServer()
{
    int i;
    //TO DO check for LocalMapServer mode (checked on client, so just a fail safe )
    seqPrepForReinit();
    printf( "Reloading Sequencers...\n");
    //TO DO (sometime) check file dates and only reload if something has changed
    for(i=1;i<entities_max;i++)
    {
        Entity* e = entities[i];

        if (entity_state[i] & ENTITY_IN_USE && e->seq ) //to do: better validating?
        {
#ifdef RAGDOLL // for ragdoll, we need all of the seq info on the server..
            seqReinitSeq( e->seq, SEQ_LOAD_FULL_SHARED, e->randSeed, e );
#else
            seqReinitSeq( e->seq, SEQ_LOAD_HEADER_ONLY, e->randSeed, e );
#endif
            entSetSeq( e, e->seq );
            entInitGrid(e, ENTTYPE(e));
        }
    }
    printf("...done reloading\n" );
    server_state.reloadSequencers = 0;
}

/*each entUpdate, check to see if any the ent has any new TriggeredMoves for the sequencer to set up.
If so, go find the move asked for by the bits.  Note this is done here so the other seqbits will be as
close to accurate as possible.
*/
void entCheckForTriggeredMoves(Entity * e)
{
    int i, j;
    TriggeredMove * tm;

    assert(e->triggered_move_count <= MAX_TRIGGEREDMOVES);

    for(j = 0 ; j < e->triggered_move_count ; j++)
    {
        tm = &e->triggered_moves[j];
        if(!tm->move_found) //if this move hasn't been evaluated yet
        {
            //Add the current bits to the bits set by the triggered move
            //And look for a new move
            for(i=0;i<STATE_ARRAY_SIZE;i++)
                tm->state[i] |= e->seq->state[i];

            tm->idx = seqFindAMove(e->seq, tm->state, STATE_HIT); //only moves that require hit can ever be hit reacts

            if(tm->idx != -1) //if you found a move, set it up to be sent out
            {
                tm->move_found    = 1;
            }
            else  //if no move, remove this one from the list
            {
                e->triggered_move_count--;
                if(j < e->triggered_move_count) //(if this isn't the last one, move the last one up)
                {
                    memmove(tm, &(e->triggered_moves[e->triggered_move_count]), sizeof(TriggeredMove) );
                    j--;
                }
            }
        }
    }
    e->set_triggered_moves = 0;  //all moves have been set up
}

static void combineStates(U32* dest, const U32** stateLists, int numStateLists)
{
    int i;
    switch(numStateLists)
    {
        case 1: for(i=0; i<STATE_ARRAY_SIZE; i++)  dest[i] |= stateLists[0][i];
        xcase 2: for(i=0; i<STATE_ARRAY_SIZE; i++) dest[i] |= stateLists[0][i] | stateLists[1][i];
        xcase 3: for(i=0; i<STATE_ARRAY_SIZE; i++) dest[i] |= stateLists[0][i] | stateLists[1][i] | stateLists[2][i];
        xcase 4: for(i=0; i<STATE_ARRAY_SIZE; i++) dest[i] |= stateLists[0][i] | stateLists[1][i] | stateLists[2][i] | stateLists[3][i];
        xdefault: FatalErrorf("combineStates passed invalid statelist count %d", numStateLists);
    }
}

static void entUpdate(Entity *e,ControlState *controls,EntType entType)
{
    SeqInst * seq;
    int numStateLists;
    const U32* stateLists[4];

    if(e->noPhysics)
        e->motion->falling = 0;

    if(e->IAmInEmoteChatMode)
        handleFutureChatQueue( e );

    // entMotion
    if(!e->noPhysics && e->move_type && e->move_type != MOVETYPE_WIRE && !e->client 
#ifdef RAGDOLL
        && !e->ragdoll
#endif
        )
    {
        PERFINFO_AUTO_START("entMotion", 1);
            entMotion(e, NULL);
        PERFINFO_AUTO_STOP();


    }

    //If I'm a pet, handle my special power status

    if( e->erOwner )
        updatePetState( e );

    // entMotionUpdateCollGrid

    seq = e->seq;

    if(entinfo[e->owner].seq_coll_door_or_worldgroup)
    {
        PERFINFO_AUTO_START("entMotionUpdateCollGrid", 1);
            entMotionUpdateCollGrid(e);
        PERFINFO_AUTO_STOP();
    }

    if(    ENTTYPE(e) != ENTTYPE_DOOR &&
        ENTTYPE(e) != ENTTYPE_CAR &&
        ENTTYPE(e) != ENTTYPE_NPC)
    {
        PERFINFO_AUTO_START("pmotionCheckVolumeTrigger", 1);
        pmotionCheckVolumeTrigger(e);
        PERFINFO_AUTO_STOP();
    }

    PERFINFO_AUTO_START("sequencer", 1);

    if(entType == ENTTYPE_PLAYER)
    {
        SETB( seq->state, STATE_PLAYER );
    }

    // gather state lists for combining
    numStateLists = 0;
    if( seq->type->constantStateStr )
        stateLists[numStateLists++] = seq->type->constantState;
    if( e->favoriteWeaponAnimList )
        stateLists[numStateLists++] = e->favoriteWeaponAnimListState;
    if( e->aiAnimList )
        stateLists[numStateLists++] = e->aiAnimListState;
    //if( e->IAmInEmoteChatMode != 0)
    stateLists[numStateLists++] = seq->sticky_state;
    combineStates(seq->state, stateLists, numStateLists);

    if( server_state.show_entity_state ) //Debug
        stateParse(seq, seq->state);

    seqSynchCycles(seq, e->owner);

    { //processInst, plus set time new move was set
        const SeqMove * before_move;
        const SeqMove * newMove;
        before_move = e->seq->animation.move_to_send;

        newMove = seqProcessInst(seq, e->timestep); //SEQQ

        //Don't just check new move because predictable moves right on heels of regular move
        //could reset the timer incorrectly
        if ( before_move != e->seq->animation.move_to_send )
        {
            e->timeMoveChanged = ((F32)ABS_TIME/100.0) + 0.0001; //so assert later will work on first frame

            //Mild hack: running out of door animation should never get delayed
            if( testSparseBit(&e->seq->animation.move_to_send->raw.required, STATE_EMERGE ) )
                e->timeMoveChanged = 0;
        }

        if( newMove )
        {
            encounterApplySetsOnReactorBits( e );

            if(seqScaledOnServer(seq))
                animSetHeader(seq, 0);
        }
    }

    copyVec3( seq->gfx_root->mat[3], seq->posLastFrame );

    entCheckForTriggeredMoves(e);

    seqClearToStickyState(seq);

    copyMat4(ENTMAT(e),seq->gfx_root->mat);
    if(seqScaledOnServer(seq))
    {
        scaleMat3Vec3( seq->gfx_root->mat, seq->currgeomscale ); 
        if(seq->type->placement == SEQ_PLACE_SETBACK)
            scaleAddVec3(seq->gfx_root->mat[2], -COMBATBIAS_DISTANCE, seq->gfx_root->mat[3], seq->gfx_root->mat[3]);
        // FIXME: add these
        //addYSpringToCharacterGraphics(seq);// Adds a little spring to the gfx_root->mat[3][1] to ease you character going over little rocks 
        //copyVec3( seq->gfx_root->mat[3], seq->posLastFrame ); // Record Position Last Frame for Run Animation scaling
    }

    PERFINFO_AUTO_STOP();

#ifdef RAGDOLL
    if ( nwEnabled() && e->seq->animation.bRagdoll && ENTTYPE(e) != ENTTYPE_PLAYER )
    {
        if (!e->ragdoll )
        {
            Vec3 vOldPos;
            Vec3 vCombatBiasAdjustment;
            Vec3 vCombatBiasAdjustmentWorld;
            Vec3 newpos;
            animSetHeader(e->seq, 0);
            animPlayInst( e->seq );
            // We don't need this once we've played our animation
            freeLastBoneTransforms(e->seq);
            copyVec3(ENTPOS(e), vOldPos);
            zeroVec3(vCombatBiasAdjustment);
            vCombatBiasAdjustment[2] = -0.8f; // FIXME: shouldn't be hardcoded like this
            mulVecMat3(vCombatBiasAdjustment, ENTMAT(e), vCombatBiasAdjustmentWorld);
            addVec3(ENTPOS(e), vCombatBiasAdjustmentWorld, newpos);
            entUpdatePosInterpolated(e, newpos);
            nwPushRagdoll(e);
            entUpdatePosInterpolated(e, vOldPos);

            e->numFramesWithoutChange = 0;

        }
        else
        {
            Mat4 newmat;
            Vec3 vChangeInPos;
            nwSetRagdollFromQuaternions( e->ragdoll, unitquat );
            nwGetActorMat4(e->ragdoll->pActor, (float*)newmat);
            if (!entUpdateMat4Interpolated(e, newmat))
            {
                Errorf("Ragdoll returned invalid matrix, deleting and moving on\n");
                nwDeleteRagdoll(e->ragdoll);
                e->ragdoll = NULL;
            }
            else
            {
                if ( ENTMAT(e)[2][1] < 0.0f )
                {
                    SETB(e->seq->state, STATE_RAGDOLLBELLY);
                }
                entSetMat3(e, unitmat);
                subVec3(ENTPOS(e), e->motion->last_pos, vChangeInPos);
                copyVec3(ENTPOS(e), e->motion->last_pos);
                //copyVec3(e->mat[3], e->pos_history.pos[e->pos_history.end]);

                if ( lengthVec3Squared(vChangeInPos) < 0.01f )
                    e->numFramesWithoutChange++;
                else
                    e->numFramesWithoutChange = 0;


                if ( e->numFramesWithoutChange < 40 )
                    SETB(e->seq->state, STATE_AIR);
            }
        }
    }
    else if (e->ragdoll) // stop ragdoll
    {
        Mat4 mWorldSpaceHips;
        Mat3 newmat;
        //Mat3 mNewObjectSpaceHips;
        //                Mat3 mInverseObjectSpaceHips;
        //Mat3 mNewWorldSpaceHips;
        //                Mat3 mWorld;
        //                F32 fYaw;
        //                Vec3 vPYR;
        F32 fScaleFactor = -1.0f;

        if (nwEnabled())
        {
            nwGetActorMat4(e->ragdoll->pActor, (float*)mWorldSpaceHips);

            // Destroy the real ragdoll
            nwDeleteRagdoll(e->ragdoll);
        }
        else
        {
            copyMat4(ENTMAT(e), mWorldSpaceHips);
        }
        if ( mWorldSpaceHips[2][1] < 0.0f )
        {
            SETB(e->seq->state, STATE_RAGDOLLBELLY);
            fScaleFactor = 1.0f;
        }

        e->ragdoll = NULL;

        // Set the animations to the correct spot
        animSetHeader(e->seq, 0);
        animPlayInst( e->seq );
        // We don't need this once we've played our animation
        freeLastBoneTransforms(e->seq);

    /*
        copyMat3(e->seq->gfx_root->child->mat, mNewObjectSpaceHips);
        invertMat3Copy(mNewObjectSpaceHips, mInverseObjectSpaceHips);
        mulMat3(mWorldSpaceHips, mInverseObjectSpaceHips, mWorld);
        getMat3PYR(mWorld, vPYR);

        copyMat3(unitmat, e->mat);
        yawMat3(vPYR[1], e->mat);
    */
        copyMat3(unitmat, newmat);

        scaleVec3(mWorldSpaceHips[1], fScaleFactor, newmat[2]);
        newmat[2][1] = 0.0f;
        if ( vec3IsZero(newmat[2]) )
        {
            newmat[2][0] = 1.0f;
        }
        normalVec3(newmat[2]);
        crossVec3(newmat[1], newmat[2], newmat[0]);

        entSetMat3(e, newmat);

        copyVec3(ENTPOS(e), e->motion->last_pos);
        zeroVec3(e->motion->vel);

        e->fakeRagdoll = 0;
        e->fakeRagdollFramesSent = 0;
        e->lastRagdollFrame = 1;
    }
    else // !e->ragdoll
#endif
    {
        if(seqScaledOnServer(seq))
        {
            // This mirrors the entClientUpdate() animation section
            // anything giant monsters use needs to be executed here

            //FIXME: add this
            // seqDoAnimationPitching(seq);

            animPlayInst(seq);

            //FIXME: add this
            //if(seq->moveRateAnimScale > 1.05 && (seq->animation.move->flags & SEQMOVE_SMOOTHSPRINT)) 
            //    smoothSprintAnimation(seq, seq->moveRateAnimScale);  //Mild hack smooth out the sprint animations 
        }
    }

#ifdef RAGDOLL
    e->seq->bCurrentlyInRagdollMode = (e->ragdoll != NULL);
#endif

    if(seqScaledOnServer(seq))
        adjustLimbs(seq);

    

    //MW 3.22.06 added this to server so HealthFx can change collision states of an ent that 
    //Uses Library Piece collision (right now it always uses the 100% health library piece collision
    //and there is just an on or off falg in the healthFx range, but really, the thing should use the 
    //collision of the library piece at that Health level.  Currently this works fine for things that 
    //only get smaller, but if they get larger, characters could get caught in the new collsion.  If 
    //we fix the function collGridUpdateHandleNearbyEntities() to work with inserted collisions we can 
    //actually use the displayed library piece 
    PERFINFO_AUTO_START("manageEntityHitPointTriggeredfx", 1);
    manageEntityHitPointTriggeredfx( e );
    PERFINFO_AUTO_STOP();
}

void entProcess(Entity *e, AIVars* ai, EntType entType)
{
    int i;

    PERFINFO_RUN(
        switch(entType){
            xcase ENTTYPE_CRITTER:
                PERFINFO_AUTO_START("critter", 1);
            xcase ENTTYPE_NPC:
                PERFINFO_AUTO_START("npc", 1);
            xcase ENTTYPE_PLAYER:
                PERFINFO_AUTO_START("player", 1);
            xcase ENTTYPE_CAR:
                PERFINFO_AUTO_START("car", 1);
            xcase ENTTYPE_DOOR:
                PERFINFO_AUTO_START("door", 1);
            xdefault:
                PERFINFO_AUTO_START("other", 1);
        }
    );

        PERFINFO_AUTO_START("gridUpdate", 1);

            // Update grid position.

            entGridUpdate(e, entType);

        PERFINFO_AUTO_STOP_START("entUpdate", 1);

            // Do motion and various other things.

            entUpdate(e, NULL, entType);

        PERFINFO_AUTO_STOP();

        if(entType == ENTTYPE_PLAYER)
        {
            PERFINFO_AUTO_START("DoorAnimCheckMove", 1);

                DoorAnimCheckMove(e);

            PERFINFO_AUTO_STOP();

            PERFINFO_AUTO_START("RandomFameTick", 1);

                RandomFameTick(e);

            PERFINFO_AUTO_STOP();

            PERFINFO_AUTO_START("StoryTick", 1);

                StoryTick(e);

            PERFINFO_AUTO_STOP();

            PERFINFO_AUTO_START("InventoryOrderTick", 1);

                InventoryOrderTick(e);

            PERFINFO_AUTO_STOP();
        }

        if (entType == ENTTYPE_NPC && e->contactOrPnpc)
        {
            PERFINFO_AUTO_START("PNPCTick", 1);
                PNPCTick(e);
            PERFINFO_AUTO_STOP();
        }

        PERFINFO_AUTO_START("storeThings", 1);
            // Save position history.

            i = e->pos_history.end = (e->pos_history.end + 1) % ARRAY_SIZE(e->pos_history.pos);

            copyVec3(ENTPOS(e), e->pos_history.pos[i]);

            mat3ToQuat(ENTMAT(e), e->pos_history.qrot[i]);

            // Optimization doesn't work with quats
            /*
            if(entType == ENTTYPE_PLAYER)
            {
//                getMat3YPR(e->mat, e->pos_history.pyr[i]);
            }
            else
            {
                PYRToQuat(ai->pyr, e->pos_history.qrot[i]);
                //copyVec3(ai->pyr, e->pos_history.pyr[i]);
            }
            */

            e->pos_history.time[i] = ABS_TIME;

            if(e->pos_history.count < ARRAY_SIZE(e->pos_history.pos))
            {
                e->pos_history.count++;
            }

            // Save distance history.

            i = e->dist_history.nextIndex = (e->dist_history.nextIndex + 1) % e->dist_history.maxIndex;

            e->dist_history.total -= e->dist_history.dist[i];

            if(sameVec3(e->dist_history.last_pos, ENTPOS(e)))
            {
                e->dist_history.dist[i] = 0;
            }
            else
            {
                e->dist_history.dist[i] = (U16)ceil(distance3(e->dist_history.last_pos, ENTPOS(e)));

                e->dist_history.total += e->dist_history.dist[i];

                copyVec3(ENTPOS(e), e->dist_history.last_pos);
            }
        PERFINFO_AUTO_STOP();
    PERFINFO_AUTO_STOP();
}


//DEBUG!! fname not used!
void entSaveAll(Entity *client_ent)
{
    char    *s;

    s = strstr(world_name,"/data/");
    if (!s)
        s = strstr(world_name,"\\data\\");
    if (s)
        s += strlen("/data/");
    else
        s = world_name;

    dbSaveEditMapName(s);
    if (svrSendAllPlayerEntsToDb())
        dbWaitContainersAcked();
    logFlush(1);
    dbSendSaveCmd();
}


//Quick hack for today...follow up with real system here
int fxTravelTimeForFxToHitTarget(char * fxname, Entity * s_ent, Entity * t_ent)
{
    Vec3 distance;
    F32  length;
    F32  time;
    if(!t_ent || !s_ent) return 0;
    if(!t_ent->seq || !s_ent->seq) return 0;
    if( !t_ent->seq->gfx_root || !s_ent->seq->gfx_root ) return 0;
    subVec3(t_ent->seq->gfx_root->mat[3], s_ent->seq->gfx_root->mat[3], distance);
    length = lengthVec3(distance);

    time = length/3.0;

    return (int)time;
}

void entSendPlrPosQrot(Entity *e)
{
    if(e->client)
    {
        e->client->controls.server_pos_id++;
        e->client->controls.server_pos_id_sent = 0;
    }
}

void svrChangeBody(Entity * e, const char * type)
{
    int reset_body = 1;

    assert(e);

    if( !type || !type[0] )
        return;

    if( e->seq )
    {
        if( stricmp( e->seq->type->name, type ) == 0 )
            reset_body = 0;
        else
            entSetSeq(e, NULL);
    }

    if( reset_body )
#ifdef RAGDOLL // ragdoll requires all seq info on server
        entSetSeq(e, seqLoadInst( type, 0, SEQ_LOAD_FULL_SHARED, e->randSeed, e ));
#else
        entSetSeq(e, seqLoadInst( type, 0, SEQ_LOAD_HEADER_ONLY, e->randSeed, e ));
#endif


    if(!e->seq)
        assert(0 == "bad body name");
}

//
// send character bits to client. 
// see entrecv.c:receiveCharacterFromServer for corresponding read code
//
void sendCharacterToClient( Packet *pak, Entity *e)
{
    int i,k;

    pktSendBitsPack(pak, 1, server_state.enableBoostDiminishing);
    pktSendBitsPack(pak, 1, server_state.enablePowerDiminishing);
    pktSendBitsPack(pak, 1, server_state.no_base_edit);
    pktSendBitsPack(pak, 2, server_state.beta_base);
    pktSendBitsPack(pak, 1, RaidIsRunning());

    // JW: Must send auth data before character so client doesn't set up funky powers
    pktSendBitsArray(pak, 8*sizeof(e->pl->auth_user_data), e->pl->auth_user_data);

    pktSendBits(pak, 1, e->pl->playerType ); STATIC_INFUNC_ASSERT(kPlayerType_Count <= (1<<1))
    pktSendBits(pak, 2, e->pl->playerSubType); STATIC_INFUNC_ASSERT(kPlayerSubType_Count <= (1<<2))
    pktSendBits(pak, 3, e->pl->praetorianProgress); STATIC_INFUNC_ASSERT(kPraetorianProgress_Count <= (1<<3))
    pktSendBits(pak, 1, e->pchar->playerTypeByLocation); STATIC_INFUNC_ASSERT(kPlayerType_Count <= (1<<1))

    character_Send(pak, e->pchar);

    pktSendString(pak, e->pl->primarySet);
    pktSendString(pak, e->pl->secondarySet);

    pktSendBitsPack(pak, 2, e->pchar ? e->pchar->iAllyID : 0);
    pktSendBitsPack(pak, 4, e->pchar ? e->pchar->iGangID : 0);

    pktSendBits(pak, 1, e->pl->pvpSwitch);
    pktSendBits(pak, 1, e->pchar->bPvPActive);
    pktSendBitsPack(pak, 5, (int)(e->pvpZoneTimer));
    pktSendBits(pak, 1, e->pl->bIsGanker);

    // Send all stats in full to the client.
    sdPackDiff(SendCharacterToSelf, pak, e->pcharLast, e->pchar, true, false, true, 0, 0);

    sendTray( pak, e );

    // send the alt, and alt2 bits
    pktSendBits( pak, 1, e->pl->tray->mode );
    pktSendBits( pak, 1, e->pl->tray->mode_alt2 );

    pktSendString( pak, e->name );
    sendEntStrings( pak, e->strings );

    for(i = 0; i < POP_HELP_STATUS_SIZE; i++)
        pktSendBits( pak, 32, e->pl->popHelpStatus[i]);
    
    for(i = 0; i < POP_HELP_LATEST_SIZE; i++)
        pktSendBits( pak, 32, e->pl->popHelpLatest[i]);
    
    pak->reliable = TRUE;

    assert(e->pl->winLocs != NULL);
    for(k=0; k<MAX_WINDOW_COUNT; k++)
    {
        sendWindow(pak, &e->pl->winLocs[k], k);
    }

    pktSendBitsAuto( pak, eaSize(&e->pl->rewardTokens) );
    for( i = eaSize(&e->pl->rewardTokens)-1; i>=0; i-- )
    {
        RewardToken *rewardtoken = e->pl->rewardTokens[i];
        const char *rw = rewardtoken ? rewardtoken->reward : "";
        int val = rewardtoken ? rewardtoken->val : 0;
        int time = rewardtoken ? rewardtoken->timer : 0;

        pktSendString(pak,rw);
        pktSendBitsPack(pak, 1, val);
        pktSendBitsPack(pak, 1, time);
    }

    pktSendBitsAuto( pak, eaSize(&e->pl->activePlayerRewardTokens) );
    for( i = eaSize(&e->pl->activePlayerRewardTokens)-1; i>=0; i-- )
    {
        RewardToken *rewardtoken = e->pl->activePlayerRewardTokens[i];
        const char *rw = rewardtoken ? rewardtoken->reward : "";
        int val = rewardtoken ? rewardtoken->val : 0;
        int time = rewardtoken ? rewardtoken->timer : 0;

        pktSendString(pak,rw);
        pktSendBitsPack(pak, 1, val);
        pktSendBitsPack(pak, 1, time);
    }

    // stragglers
    pktSendBits( pak, 10, e->pl->lfg );
    pktSendBits( pak, 10, e->pl->hidden );
    pktSendBits( pak, 1, e->pl->supergroup_mode );
    pktSendBits( pak, 1, e->pl->hide_supergroup_emblem );
    pktSendBits( pak, 1, e->pl->skillsUnlocked );
    pktSendBits( pak, 1, e->pl->teambuff_display );
    pktSendBits( pak, 32, e->pl->dock_mode );
    pktSendBits( pak, 32, e->pl->inspiration_mode);
    pktSendBits( pak, 32, e->pl->newFeaturesVersion);
    sendChatSettings(pak, e);

    pktSendBitsPack( pak, 5, e->pl->titleThe );
    pktSendString( pak, e->pl->titleTheText );
    pktSendString( pak, e->pl->titleCommon );
    pktSendString( pak, e->pl->titleOrigin );
    pktSendBitsAuto(pak, e->pl->titleColors[0]);
    pktSendBitsAuto(pak, e->pl->titleColors[1]);
    pktSendBits( pak, 32, e->pl->titleBadge );
    pktSendString( pak, e->pl->titleSpecial );
    pktSendString( pak, e->strings->playerDescription );
    pktSendString( pak, e->strings->motto );
    pktSendString( pak, e->pl->comment ); 

    assert( e->pl->keybinds );
    if( !e->pl->keyProfile[0] )
    {
        if( isDevelopmentMode() )
            strcpy(  e->pl->keyProfile, "Dev" );
        else
            strcpy( e->pl->keyProfile, "default" );
    }
    keybinds_send( pak, e->pl->keyProfile, e->pl->keybinds );

    if( e->pl->options_saved )  // This corresponds to receiveOptions on client
    {
        pktSendBits( pak, 1, 1 );
        // all options here
        pktSendF32( pak, e->pl->mouse_speed );
        pktSendF32( pak, e->pl->turn_speed );
        pktSendBits( pak, 1,  e->pl->mouse_invert );

        pktSendBits( pak, 1,  e->pl->fading_chat );
        pktSendBits( pak, 1,  e->pl->fading_chat1 );
        pktSendBits( pak, 1,  e->pl->fading_chat2 );
        pktSendBits( pak, 1,  e->pl->fading_chat3 );
        pktSendBits( pak, 1,  e->pl->fading_chat4 );
        pktSendBits( pak, 1,  e->pl->fading_nav );

        pktSendBits( pak, 1, e->pl->showToolTips );
        pktSendBits( pak, 1, e->pl->allowProfanity );
        pktSendBits( pak, 1, e->pl->showBalloon );
        pktSendBits( pak, 1, e->pl->declineGifts );
        pktSendBits( pak, 1, e->pl->declineTeamGifts );
        pktSendBits( pak, 1, e->pl->promptTeamTeleport );
        pktSendBits( pak, 1, e->pl->showPets );
        pktSendBits( pak, 1, e->pl->showSalvage );
        pktSendBits( pak, 1, e->pl->webHideBasics );
        pktSendBits( pak, 1, e->pl->webHideStats );
        pktSendBits( pak, 1, e->pl->webHidePowers );
        pktSendBits( pak, 1, e->pl->webHideBadges );
        pktSendBits( pak, 1, e->pl->webHideFriends );
        pktSendBits( pak, 1, e->pl->windowFade );
        pktSendBits( pak, 1, e->pl->logChat );
        pktSendBits( pak, 1, e->pl->sgHideHeader );
        pktSendBits( pak, 1, e->pl->sgHideButtons );
        pktSendBits( pak, 1, e->pl->clicktomove );
        pktSendBits( pak, 1, e->pl->disableDrag );
        pktSendBits( pak, 1, e->pl->showPetBuffs );
        pktSendBits( pak, 1, e->pl->preventPetIconDrag);
        pktSendBits( pak, 1, e->pl->showPetControls);
        pktSendBits( pak, 1, e->pl->advancedPetControls);
        pktSendBits( pak, 1, e->pl->disablePetSay );
        pktSendBits( pak, 1, e->pl->enableTeamPetSay );
        pktSendBits( pak, 2, e->pl->teamCompleteOption );
        pktSendBits( pak, 1, e->pl->disablePetNames );
        pktSendBits( pak, 1, e->pl->hidePlacePrompt );
        pktSendBits( pak, 1, e->pl->hideDeletePrompt );
        pktSendBits( pak, 1, e->pl->hideDeleteSalvagePrompt );
        pktSendBits( pak, 1, e->pl->hideDeleteRecipePrompt );
        pktSendBits( pak, 1, e->pl->hideInspirationFull );
        pktSendBits( pak, 1, e->pl->hideSalvageFull );
        pktSendBits( pak, 1, e->pl->hideRecipeFull );
        pktSendBits( pak, 1, e->pl->hideEnhancementFull );
        pktSendBits( pak, 1, e->pl->showEnemyTells );
        pktSendBits( pak, 1, e->pl->showEnemyBroadcast );
        pktSendBits( pak, 1, e->pl->hideEnemyLocal );
        pktSendBits( pak, 1, e->pl->hideCoopPrompt );
        pktSendBits( pak, 1, e->pl->staticColorsPerName );

        // contact sorting is sent as four bit int
        if (e->pl->contactSortByName)
            pktSendBits( pak, 4, 1 );
        else if (e->pl->contactSortByZone)
            pktSendBits( pak, 4, 2 );
        else if (e->pl->contactSortByRelationship)
            pktSendBits( pak, 4, 3 );
        else if (e->pl->contactSortByActive)
            pktSendBits( pak, 4, 3 );
        else 
            pktSendBits( pak, 4, 0 );

        pktSendBits( pak, 1, e->pl->recipeHideUnOwned );
        pktSendBits( pak, 1, e->pl->recipeHideMissingParts );
        pktSendBits( pak, 1, e->pl->recipeHideUnOwnedBench );
        pktSendBits( pak, 1, e->pl->recipeHideMissingPartsBench );

        pktSendBits( pak, 1, e->pl->declineSuperGroupInvite );
        pktSendBits( pak, 1, e->pl->declineTradeInvite );

        pktSendBits( pak, 1,  e->pl->freeCamera );
        pktSendF32( pak, e->pl->tooltipDelay );

        pktSendBits( pak, 3, e->pl->eShowArchetype );
        pktSendBits( pak, 3, e->pl->eShowSupergroup );
        pktSendBits( pak, 3, e->pl->eShowPlayerName );
        pktSendBits( pak, 3, e->pl->eShowPlayerBars );
        pktSendBits( pak, 3, e->pl->eShowVillainName );
        pktSendBits( pak, 3, e->pl->eShowVillainBars );
        pktSendBits( pak, 3, e->pl->eShowPlayerReticles );
        pktSendBits( pak, 3, e->pl->eShowVillainReticles );
        pktSendBits( pak, 3, e->pl->eShowAssistReticles );
        pktSendBits( pak, 3, e->pl->eShowOwnerName );
        pktSendBits( pak, 3, e->pl->mousePitchOption );

        pktSendBits( pak, 32, e->pl->mapOptions );
        pktSendBits( pak, 32, e->pl->buffSettings );
        pktSendBits( pak, 32, e->pl->chatBubbleTextColor );
        pktSendBits( pak, 32, e->pl->chatBubbleBackColor );
        pktSendBits( pak, 8, e->pl->chat_font_size );

        pktSendBits( pak, 1,  e->pl->reverseMouseButtons);
        pktSendBits( pak, 1,  e->pl->disableCameraShake);
        pktSendBits( pak, 1,  e->pl->disableMouseScroll);
        pktSendBits( pak, 1,  e->pl->logPrivateMessages);
        pktSendBits( pak, 3,  e->pl->eShowPlayerRating);
        pktSendBits( pak, 1,  e->pl->disableLoadingTips);
        pktSendF32( pak, e->pl->mouseScrollSpeed );
        pktSendBits( pak, 1,  e->pl->enableJoystick );
        pktSendBits( pak, 1,  e->pl->fading_tray );
        pktSendBits( pak, 1,  e->pl->ArchitectNav );
        pktSendBits( pak, 1,  e->pl->ArchitectTips );
        pktSendBits( pak, 1,  e->pl->ArchitectAutoSave );
        pktSendBits( pak, 1,  e->pl->noXP );
        pktSendBits( pak, 1,  e->pl->ArchitectBlockComments );
        pktSendBits( pak, 6,  e->pl->autoAcceptTeamAbove);
        pktSendBits( pak, 6,  e->pl->autoAcceptTeamBelow);
        pktSendBits( pak, 1, e->pl->disableEmail);
        pktSendBits( pak, 1, e->pl->friendSgEmailOnly); 
        pktSendBits( pak, 1, e->pl->noXPExemplar);
        pktSendBits( pak, 1, e->pl->popHelpDisable);
        pktSendBits( pak, 1,  e->pl->hideFeePrompt );
        pktSendBits( pak, 1, e->pl->hideUsefulSalvageWarning );
        pktSendBits( pak, 1, e->pl->forcedPopHelpDisable);
        pktSendBits( pak, 1, e->pl->gmailFriendOnly);
        pktSendBits( pak, 1, e->pl->useOldTeamUI);
        pktSendBits( pak, 1, e->pl->hideUnclaimableCert);
        pktSendBits( pak, 1, e->pl->blinkCertifications);
        pktSendBits( pak, 1, e->pl->voucherSingleCharacterPrompt);
        pktSendBits( pak, 1, e->pl->newCertificationPrompt);
        pktSendBits( pak, 1, e->pl->hideUnslotPrompt);
        pktSendBits( pak, 16, e->pl->mapOptionRevision);
        pktSendBits( pak, 32, e->pl->mapOptions2);
        pktSendBits( pak, 1, e->pl->hideOpenSalvageWarning );
        pktSendBits( pak, 1, e->pl->hideLoyaltyTreeAccessButton );
        pktSendBits( pak, 1, e->pl->hideStoreAccessButton );
        pktSendBits( pak, 1, e->pl->autoFlipSuperPackCards );
        pktSendBits( pak, 1, e->pl->hideConvertConfirmPrompt );
        pktSendBits( pak, 3, e->pl->hideStorePiecesState );
        pktSendF32( pak, e->pl->cursorScale );
        pktSendBits(pak, 1, e->pl->showTimer);
    }
    else
    {
        pktSendBits( pak, 1, 0 );
        pktSendBits( pak, 1, e->pl->mouse_invert );
        pktSendF32( pak, e->pl->mouse_speed );
        pktSendF32( pak, e->pl->turn_speed );
    }

    pktSendBitsPack( pak, 9, e->pl->divSuperName );
    pktSendBitsPack( pak, 9, e->pl->divSuperMap );
    pktSendBitsPack( pak, 9, e->pl->divSuperTitle );
    pktSendBitsPack( pak, 9, e->pl->divEmailFrom );
    pktSendBitsPack( pak, 9, e->pl->divEmailSubject );
    pktSendBitsPack( pak, 9, e->pl->divFriendName );
    pktSendBitsPack( pak, 9, e->pl->divLfgName );
    pktSendBitsPack( pak, 9, e->pl->divLfgMap );

    pktSendBits( pak, 1, e->pl->first );
    pktSendBits( pak, 2, e->client?e->client->controls.nocoll:0);
    sendFriendsList( e, pak, 1 );
    sendCombatMonitorStats(e,pak);

    if(e->db_flags & DBFLAG_RENAMEABLE)
        pktSendBits( pak, 1, 1 );
    else
        pktSendBits( pak, 1, 0 );

    // these all send their own packets
    ContactStatusSendAll(e);
    ContactStatusAccessibleSendAll(e);
    ContactVisitLocationSendAll(e);
    TaskStatusSendAll(e);
    TeamTaskAddMemberTaskList(e);    // Tell team members about my tasks.
    ClueSendAll(e);
    KeyClueSendAll(e);
    scSendAllHeaders(e);
    MissionSendRefresh(e);
    sendPendingReward(e);
    TaskForceSendSelect(e);
}

//
//
//
bool receiveCharacterFromClient( Packet *pak, Entity *e )
{
    bool bRet;
    Costume* mutable_costume;

    e->pl->playerType = pktGetBits( pak, 1 ); // validated in character_ReceiveCreate

    costumeAwardAuthParts( e );

    bRet = character_ReceiveCreate(pak, e->pchar, e);
    if (!bRet)
    {
        e->corrupted_dont_save = 1;
        return bRet;
    }

    receiveTray( pak, e );

    {
        // Total special-case hack to get the preferred tray power order
        // Gets primary, secondary, Brawl, Sprint
        // Also all inherent powers granted by auth bits
        const PowerSet *pPowerSet;
        const BasePowerSet *psetBase;
        const Power *ppow;
        int count = 0;    // caps at six right now, 1 primary, 1 secondary, brawl, 3 MM macros
        // if count gets past ten, getTrayObj will return NULL and cause a crash!
        int j, count2 = 0;
        int iset, ipow;

        // primary set is always in this slot on player characters
        pPowerSet = e->pchar->ppPowerSets[g_numSharedPowersets + g_numAutomaticSpecificPowersets];
        for( j = 0; j < eaSize( &pPowerSet->ppPowers ); j++)
        {
            if( pPowerSet->ppPowers[j]->ppowBase->eType != kPowerType_Auto )
            {
                // should only happen once, so no threat of overflowing the tray
                buildPowerTrayObj(getTrayObj(e->pl->tray, kTrayCategory_PlayerControlled, 0, count, e->pchar->iCurBuild, true), g_numSharedPowersets + g_numAutomaticSpecificPowersets, j, 0);
                count++;
            }
        }

        // secondary set is always in this slot on player characters
        pPowerSet = e->pchar->ppPowerSets[g_numSharedPowersets + g_numAutomaticSpecificPowersets + 1];
        for( j = 0; j < eaSize( &pPowerSet->ppPowers ); j++)
        {
            if( pPowerSet->ppPowers[j]->ppowBase->eType != kPowerType_Auto )
            {
                // should only happen once, so no threat of overflowing the tray
                buildPowerTrayObj(getTrayObj(e->pl->tray, kTrayCategory_PlayerControlled, 0, count, e->pchar->iCurBuild, true), g_numSharedPowersets + g_numAutomaticSpecificPowersets + 1, j, 0);
                count++;
            }
        }

        psetBase = powerdict_GetBasePowerSetByName(&g_PowerDictionary, "inherent", "inherent");
        pPowerSet = character_OwnsPowerSet(e->pchar, psetBase);
        for (j = 0; j < eaSize(&pPowerSet->ppPowers); j++)
        {
            ppow = pPowerSet->ppPowers[j];
            power_GetIndices(ppow, &iset, &ipow);
            if (!stricmp(ppow->ppowBase->pchName, "Brawl"))
            {
                buildPowerTrayObj(getTrayObj(e->pl->tray, kTrayCategory_PlayerControlled, 0, count, e->pchar->iCurBuild, true), iset, ipow, 0);
                count++;
            }
            else if (!stricmp(ppow->ppowBase->pchName, "Sprint"))
            {
                buildPowerTrayObj(getTrayObj(e->pl->tray, kTrayCategory_PlayerControlled, 0, 8, e->pchar->iCurBuild, true), iset, ipow, 0);
            }
            else if (ppow->ppowBase->eType != kPowerType_Auto)
            {
                buildPowerTrayObj(getTrayObj(e->pl->tray, kTrayCategory_PlayerControlled, 1 + count2 / TRAY_SLOTS, count2 % TRAY_SLOTS, e->pchar->iCurBuild, true), iset, ipow, 0);
                count2++;
            }
        }
    }

    costumeAwardPowersetParts(e, 1, 0);

    // @todo
    // characters received from clients are not part of shared memory assets
    // and they should already have a 'mutable' entity costume in that it is
    // just an entry in the player costume slot table
    // That will fall out once the costume retrofit is complete but for now
    // we just recognize and force the case here:
    e->costume_is_mutable = true;
    mutable_costume = cpp_reinterpret_cast(Costume*)(e->costume);

    bRet &= costume_receive(pak, mutable_costume );
    if (!bRet)
    {
        e->corrupted_dont_save = 1;
        return bRet;
    }
    

    powerCustList_receive(pak, e, e->powerCust );
    // The server does not need to apply a costume.
    // However, it does need to have the correct bodyType so that
    // the server can tell the client about their correct animations.
    svrChangeBody(e, entTypeFileName(e));

    e->pl->num_costumes_stored = 1; // starting costume is loaded
    e->pl->validateCostume = true;    // remember to validate costume when inventory is loaded

    // make sure they don't have any empty weapons etc.
    costumeFillPowersetDefaults(e);

    if( e->costume->appearance.bodytype == kBodyType_Female || e->costume->appearance.bodytype == kBodyType_BasicFemale )
        e->gender = GENDER_FEMALE;
    else
        e->gender = GENDER_MALE;

    e->pl->titleThe = pktGetBitsPack( pak, 5 );
    strncpyt(e->pl->titleTheText, pktGetIfSetString(pak), 10);
    e->name_gender = pktGetBits(pak, 2);
    e->pl->attachArticle = pktGetBits(pak, 1);
    receiveEntStrings( pak, e->strings );

    // do after getting class
    costumeValidateCostumes(e);

    return bRet;
}


//ent Alloc server
Entity *entAlloc()
{
    Entity    *e;
    static    U32 ent_id;

    e = entAllocCommon();

    //do server specific ent alloc stuff
    e->id = ++ent_id;
    e->db_id = -1;
    e->netfx_effective = -1;
    num_ents++;

    return e;
}


void entClearAllTargeters(Entity *ent)
{
    int i;

    for(i=1;i<entities_max;i++)
    {
        if(entInUse(i) && entities[i]->pchar)
        {
            if(entities[i]->pchar->erTargetCurrent == erGetRef(ent))
                entities[i]->pchar->erTargetCurrent = 0;

            if(entities[i]->pchar->erTargetQueued == erGetRef(ent)) 
                entities[i]->pchar->erTargetQueued = 0;
        }
    }
}

struct {
    const char* fileName;
    int            fileLine;
} freed_ents[MAX_ENTITIES_PRIVATE];

static void DialogContextDestructor(DialogContext *pContext)
{
    ScriptVarsTableClear(&pContext->vars);
    SAFE_FREE(pContext);
}

static void DialogContextDestructorAdapter(void* arg0)
{
    DialogContextDestructor((DialogContext *)arg0);
}

void entFreeDbg(Entity *ent, const char* fileName, int fileLine)
{
    if (!ent || ent->owner <= 0)
    {
        return;
    }
    
    if (ent->db_id > 0)
    {
        containerCacheRemoveEntry(CONTAINER_ENTS,ent->db_id);
    }
    
    PERFINFO_AUTO_START("entFree", 1);
        freed_ents[ent->owner].fileName = fileName;
        freed_ents[ent->owner].fileLine = fileLine;
    
#ifdef RAGDOLL
        if (ent->ragdoll)
        {
            nwDeleteRagdoll(ent->ragdoll);
            ent->ragdoll = NULL;
            ent->lastRagdollFrame = 1;
        }
#endif

        PERFINFO_AUTO_START("top", 1);

            //Notify any interested scripts that I'm dead 
            if(ENTINFO_BY_ID(ent->owner).has_processed) //(only if I was really alive and on the map)
                ScriptEntityFreed(ent);

            // debug check to make sure it's not still in a player ents array
            if (ENTTYPE(ent) == ENTTYPE_PLAYER)
            {
                int i, j;
                for (i=0; i<PLAYER_ENTTYPE_COUNT; i++) {
                    for (j=0; j<player_ents[i].count; j++) {
                        if (player_ents[i].ents[j] == ent) {
                            assert(!"Freeing ent that is still in the player_ents array!  Bad!");
                            playerEntDelete(i,ent,0);
                            break;
                        }
                    }
                }
            }

            assert(!ent->client);

            entity_state[ent->owner] = ENTITY_BEING_FREED;

        PERFINFO_AUTO_STOP_START("removeEntFromSpawnArea", 1);

            removeEntFromSpawnArea(ent);

        PERFINFO_AUTO_STOP_START("EncounterEntUpdateState", 1);

            EncounterEntDetach(ent);

        PERFINFO_AUTO_STOP_START("entFreeBase", 1);

            entFreeBase(ent);

        PERFINFO_AUTO_STOP_START("stuff1", 1);

            // Free unsent game cmds
            if (ent->entityPacket)
            {
                pktFree(ent->entityPacket);
                ent->entityPacket = 0;
            }

            // Check if I have a slave and free him from my clutches if I do.
            if(ent->client && ent->client->slave){
                freePlayerControl(ent->client);
            }                                          
            else if(ent->myMaster){                      
                freePlayerControl(ent->myMaster);      
            }                                          
                                                      
            entNetDelete(ent);
            if (ent->onClickHandler)
                ScriptClosureListDestroy(&ent->onClickHandler);

            if (ent->scriptData)
                ScriptFreeData(&ent->scriptData);

            if (ent->dialogLookup != NULL)
            {
                stashTableDestroyEx(ent->dialogLookup, NULL, DialogContextDestructorAdapter);
                ent->dialogLookup = NULL;
            }

        PERFINFO_AUTO_STOP_START("entFreeGrid", 1);

            entFreeGrid(ent);

        PERFINFO_AUTO_STOP_START("aiDestroy", 1);

            aiDestroy(ent);

        PERFINFO_AUTO_STOP_START("playerVarFree", 1);

            playerVarFree( ent );

        PERFINFO_AUTO_STOP_START("entClearAllTargeters", 1);

            entClearAllTargeters(ent);

        PERFINFO_AUTO_STOP_START("entFreeQueuedChat", 1);

            SAFE_FREE( ent->queuedChat );
            SAFE_FREE( ent->sayOnClick );
            SAFE_FREE( ent->rewardOnClick );
            SAFE_FREE( ent->sayOnKillHero );

            clearallOnClickCondition(ent);

        PERFINFO_AUTO_STOP_START("pktFree", 1);

            if(ent->send_packet)
            {
                pktFree(ent->send_packet);
                ent->send_packet = NULL;
            }

        PERFINFO_AUTO_STOP_START("entFreeCommon", 1);

            entFreeCommon( ent );

        PERFINFO_AUTO_STOP_START("bottom", 1);


            ent->owner = -1;
            ent->freed_time = dbSecondsSince2000();

            num_ents--;
        PERFINFO_AUTO_STOP();
    PERFINFO_AUTO_STOP();
}

void entKillAll(){
    int i = 0;

    for(i = 1; i < entities_max; i++){
        if(entity_state[i] & ENTITY_IN_USE){
            if(ENTTYPE_BY_ID(i) != ENTTYPE_PLAYER){
                entFree(entities[i]);
            }
        }
    }
}

Entity *entFromId(int id)
{
    if (id < 1 || id >= ARRAY_SIZE(entities))
        return 0;
    return entities[id];
}

Entity *validEntFromId(int id)
{
    // Returns an entity pointer only if that entity is in use.

    if(entInUse(id)){
        return entities[id];
    }

    return NULL;
}

Entity *findValidEntByName(char *name)
{
    int count;
    const char *ename;

    if(name && name[0])
    {
        for (count = 0; count < entities_max; count++)
        {
            if (entInUse(count))
            {
                ename = entGetName(entities[count]);

                if (ename && ename[0] && stricmp(name, ename) == 0)
                    return entities[count];
            }
        }
    }

    return NULL;
}

const char* entGetName(const Entity* e)
{
    if(!e)
        return NULL;

    if (e->name && e->name[0])
        return e->name;
    else
        return npcDefList.npcDefs[e->npcIndex]->displayName;
}

Beacon* entGetNearestBeacon(Entity* e)
{
    if(!e)
    {
        return NULL;
    }
    
    if(!e->nearestBeacon)
    {
        PERFINFO_AUTO_START("egnb.beaconGetNearestBeacon", 1);
            e->nearestBeacon = beaconGetNearestBeacon(ENTPOS(e));
        PERFINFO_AUTO_STOP();
    }

    return e->nearestBeacon;
}

void entGetPosAtTime(Entity* e, U32 find_time, Vec3 outPos, Quat outQrot){
    int count = e->pos_history.count - (e->pos_history.end - e->pos_history.net_begin + ARRAY_SIZE(e->pos_history.pos)) % ARRAY_SIZE(e->pos_history.pos);
    int lo = 0;
    int hi = count - 1;

    int pos_end = e->pos_history.net_begin + ENT_POS_HISTORY_SIZE;
    Vec3* pos_pos = e->pos_history.pos;
    Quat* pos_qrot = e->pos_history.qrot;
    U32* pos_time = e->pos_history.time;

    int lo_idx = (pos_end - lo) % ENT_POS_HISTORY_SIZE;
    int hi_idx = (pos_end - hi) % ENT_POS_HISTORY_SIZE;

    // Find where this entity was in history.

    while(hi - lo > 1)
    {
        int mid = (hi + lo) / 2;
        int mid_idx = (pos_end - mid) % ENT_POS_HISTORY_SIZE;
        U32 diff = find_time - pos_time[hi_idx];
        U32 max_diff = pos_time[mid_idx] - pos_time[hi_idx];

        if(diff >= max_diff)
        {
            hi = mid;
            hi_idx = mid_idx;
        }
        else
        {
            lo = mid;
            lo_idx = mid_idx;
        }
    }

    {
        U32 max_diff = pos_time[lo_idx] - pos_time[hi_idx];
        U32 diff = find_time - pos_time[hi_idx];

        if(diff <= max_diff && max_diff)
        {
            Vec3 lo_vec;
            F32 lo_factor = (float)diff / (float)max_diff;

            scaleVec3(pos_pos[hi_idx], 1 - lo_factor, outPos);
            scaleVec3(pos_pos[lo_idx], lo_factor, lo_vec);

            addVec3(lo_vec, outPos, outPos);

            if( outQrot )
            {
                quatInterp(lo_factor, pos_qrot[hi_idx], pos_qrot[lo_idx], outQrot);
                /*
                int i;
                for(i = 0; i < 3; i++)
                {
                    float diff = lo_factor * subAngle(pos_pyr[lo_idx][i], pos_pyr[hi_idx][i]);
                    outPyr[i] = addAngle(pos_pyr[hi_idx][i], diff);
                }
                */
            }
        }
        else
        {
            copyVec3(ENTPOS(e), outPos); 
            if( outQrot )
                copyQuat( e->net_qrot, outQrot );//getMat3YPR(e->mat, outPyr); //TO DO can I use e->net_pyr?
        }
    }

}

void entGetPosAtEntTime(Entity* e, Entity* entToGetTimeFrom, Vec3 outPos, Quat outQRot)
{
    if(entToGetTimeFrom->client && !server_state.disableMeleePrediction)
    {
        entGetPosAtTime(e, entToGetTimeFrom->client->controls.cur_abs_time, outPos, outQRot);
    }
    else
    {
        copyVec3(ENTPOS(e), outPos);
        if( outQRot )
        {
            mat3ToQuat(ENTMAT(e), outQRot);
        }
    }
}


#if MEGA_GRID_DEBUG
    void verifyGridNode(void* owner, MegaGridNode* node){
        Entity* ent = validEntFromId((int)owner);
        
        assert(ent);
        
        assert(ent->megaGridNode == node);
    }
#endif

void entFreeGrid(Entity *e)
{
    #if SERVER
        if(e->megaGridNode){
            destroyMegaGridNode(e->megaGridNode);
            e->megaGridNode = NULL;

            #if MEGA_GRID_DEBUG
                mgVerifyGrid(0, verifyGridNode);
            #endif
        }

        if(e->megaGridNodeCollision){
            destroyMegaGridNode(e->megaGridNodeCollision);
            e->megaGridNodeCollision = NULL;
        }

        if(e->megaGridNodePlayer){
            destroyMegaGridNode(e->megaGridNodePlayer);
            e->megaGridNodePlayer = NULL;
        }
    #endif
}

void entFreeGridAll()
{
    int        i;

    for(i=1;i<entities_max;i++)
    {
        entFreeGrid(entities[i]);
    }
}

void entSetHelperStatus(Entity *e, int newHelperStatus)
{
    if (newHelperStatus >= 0 && newHelperStatus <= 3 && e && e->pl)
    {
        e->pl->helperStatus = newHelperStatus;
        e->helper_status_update = 1;

        if (newHelperStatus == 1)
        {
            sendInfoBox(e, INFO_SVR_COM, "HelperMessage1");
        }
        else if (newHelperStatus == 2)
        {
            sendInfoBox(e, INFO_SVR_COM, "HelperMessage2");
        }
        else if (newHelperStatus == 3)
        {
            sendInfoBox(e, INFO_SVR_COM, "HelperMessage3");
        }
    }
}

void* orderId_Create()
{
    return calloc(1, sizeof(OrderId));
}

void entMarkOrderCompleted(Entity *e, OrderId order_id)
{
    if (e->pl)
    {
        OrderId *copy = (OrderId*)malloc(sizeof(OrderId));
        memcpy(copy, &order_id, sizeof(OrderId));
        eaPush(&e->pl->completed_orders, copy);
    }
}
