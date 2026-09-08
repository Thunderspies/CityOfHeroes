#include "svr/svr_base.h"
#include "entity/entity.h"
#include "entity/motion.h"
#include "gameComm/sendToClient.h"
#include "player/clientEntityLink.h"
#include <utilitieslib/utils/utils.h>
#include "entity/entserver.h"
#include "ai/entai.h"
#include "gameComm/Npc.h"
#include "gameComm/NpcServer.h"
#include <utilitieslib/utils/cmdoldparse.h>
#include "cmdparse/cmdserverdebug.h"
#include "generator/encounterPrivate.h"
#include "dbcomm/dbdoor.h"
#include <utilitieslib/components/earray.h>
#include "storyarc/mission.h"
#include "seq/seq.h"
#include "cmdparse/cmdserver.h"
#include "utils/position.h"
#include "group/group.h"
#include "beacon/beacon.h"
#include "beacon/beaconDebug.h"
#include "group/groupgrid.h"

void gotoNextReset(ClientLink* client, int type)
{
    client->lastEntityID = 0;
    client->findNextCursor = 0;
    client->findNextType = type;
}

void gotoNextEntity(EntType type, ClientLink* client, int awakeRequired)
{
    int i;

    if (!client->entity) return;
    if (type != client->findNextType)
        gotoNextReset(client, type);

    for (i = client->findNextCursor; i < entities_max; i++)
    {
        if (entity_state[i] & ENTITY_IN_USE){
            Entity* ent = entities[i];

            // If the entity is of the correct type and we haven't
            // seen this entity yet.
            if(    ENTTYPE_BY_ID(i) == type &&
                ent != client->entity &&
                (!awakeRequired || !ENTSLEEPING_BY_ID(i)) &&
                // If we're looking for critters, let's only choose those not on our team.
                (type != ENTTYPE_CRITTER ||
                    (ent->pchar && client->entity->pchar && ent->pchar->iAllyID!=client->entity->pchar->iAllyID)))
            {
                // goto the entity now.
                conPrintf(client, localizedPrintf(client->entity,"GoingToEnt", i));
                entUpdatePosInstantaneous(client->entity, ENTPOS(ent));
                client->lastEntityID = ent->id;
                client->findNextCursor = i + 1;
                return;
            }
        }
    }
    conPrintf(client, localizedPrintf(client->entity,"NoEntitiesOfType"));
    gotoNextReset(client, type);
}

void gotoNextSeq(char* seqName, ClientLink* client)
{
    int i;

    if (!client->entity) return;
    if (FINDTYPE_SEQ != client->findNextType)
        gotoNextReset(client, FINDTYPE_SEQ);

    for (i = client->findNextCursor; i < entities_max; i++)
    {
        if (entity_state[i] & ENTITY_IN_USE)
        {
            Entity* ent = entities[i];

            // If the entity is of the correct type and we haven't
            // seen this entity yet.
            if(ent->seq && strstriConst(ent->seq->type->name, seqName) && ent->id > client->lastEntityID){
                // goto the entity now.
                conPrintf(client, localizedPrintf(client->entity,"GoingToEnt", i));
                entUpdatePosInstantaneous(client->entity, ENTPOS(ent));
                client->lastEntityID = ent->id;
                client->findNextCursor = i + 1;
                return;
            }
        }
    }
    conPrintf(client, localizedPrintf(client->entity,"NoEntsWithSeq", seqName));
    gotoNextReset(client, FINDTYPE_SEQ);
}

void gotoNextSpawn(ClientLink* client, int type)
{
    int i;
    EncounterGroup* group;

    if (!client->entity) return;
    if (type != client->findNextType)
        gotoNextReset(client, type);

    if (client->findNextCursor <= 0) client->findNextCursor = 1;
    for (i = client->findNextCursor, group = EncounterGroupFromHandle(i);
         group;
         ++i, group = EncounterGroupFromHandle(i))
    {
        if (type == FINDTYPE_SPAWN && !group->active) continue;
        //if (type == FINDTYPE_MISSIONSPAWN && !group->missionspawn) continue;
        conPrintf(client, localizedPrintf(client->entity,"GoingTOSpawn", i+1));
        entUpdatePosInstantaneous(client->entity, group->mat[3]);
        client->findNextCursor = i + 1;
        return;
    }
    conPrintf(client, localizedPrintf(client->entity,"NoMoreSpawnsOfType"));
    gotoNextReset(client, type);
}

void gotoNextMissionSpawn(ClientLink* client, int unconqueredOnly)
{
    int room, spawn, cursor;
    int type = unconqueredOnly? FINDTYPE_UMISSIONSPAWN: FINDTYPE_MISSIONSPAWN;

    if (!client->entity) return;
    if (type != client->findNextType)
        gotoNextReset(client, type);

    // cycle through rooms and spawns, look for next cursor point
    cursor = 0;
    for (room = 1; room < eaSize(&g_rooms); room++)
    {
        for (spawn = 0; spawn < eaSize(&g_rooms[room]->standardEncounters); spawn++, cursor++)
        {
            EncounterGroup* group = g_rooms[room]->standardEncounters[spawn];
            int conquered = group->conquered || (group->active && group->active->villaincount == 0);
            if (unconqueredOnly && conquered) continue;
            if ((group->missiongroup || group->missionspawn) &&
                (cursor >= client->findNextCursor))
            {
                conPrintf(client, localizedPrintf(client->entity,"GoingTOMissionSpawn", g_rooms[room]->roompace, spawn+1,
                    conquered? "Conquered": "UnConquered"));
                entUpdatePosInstantaneous(client->entity, group->mat[3]);
                client->findNextCursor = cursor + 1;
                return;
            }
        }
        for (spawn = 0; spawn < eaSize(&g_rooms[room]->hostageEncounters); spawn++, cursor++)
        {
            EncounterGroup* group = g_rooms[room]->hostageEncounters[spawn];
            int conquered = group->conquered || (group->active && group->active->villaincount == 0);
            if (unconqueredOnly && conquered) continue;
            if (group->missionspawn && cursor >= client->findNextCursor)
            {
                conPrintf(client, localizedPrintf(client->entity,"GoingToHostageSpawn", g_rooms[room]->roompace, spawn+1,
                    conquered? "Conquered": "UnConquered"));
                entUpdatePosInstantaneous(client->entity, group->mat[3]);
                client->findNextCursor = cursor + 1;
                return;
            }
        }
    }
    conPrintf(client, localizedPrintf(client->entity,"NoMoreEncounters"));
    gotoNextReset(client, type);
}

void gotoNextBadSpawn(ClientLink* client)
{
    if (!client->entity) return;
    if (FINDTYPE_BADSPAWN != client->findNextType)
        gotoNextReset(client, FINDTYPE_BADSPAWN);

    if (client->findNextCursor < server_state.bad_spawn_count)
    {
        entUpdatePosInstantaneous(client->entity, server_state.bad_spawns[client->findNextCursor]);
        conPrintf(client, "going to bad spawn %d/%d", client->findNextCursor + 1, server_state.bad_spawn_count);
        client->findNextCursor++;
    }
    else
    {
        conPrintf(client, "There are no more bad spawns");
        gotoNextReset(client, FINDTYPE_BADSPAWN);
    }
}

void gotoNextBadVolume(ClientLink* client)
{
    Vec3 point;
    int current;
    int count;

    if (!client->entity)
    {
        return;
    }
    if (!nextOverlapPoint(point, &current, &count))
    {
        return;
    }

    entUpdatePosInstantaneous(client->entity, point);
    conPrintf(client, "going to bad volume %d/%d", current, count);
}

void rescanBadVolume()
{
    overlappingVolume_Load();
}

void gotoNextObjective(ClientLink* client)
{
    if (!client->entity)
        return;
    if (FINDTYPE_OBJECTIVE != client->findNextType)
        gotoNextReset(client, FINDTYPE_OBJECTIVE);

    if (client->findNextCursor < MissionNumObjectives())
    {
        MissionGotoObjective(client, client->findNextCursor);
        client->findNextCursor++;
    }
    else
    {
        conPrintf(client, localizedPrintf(client->entity,"NoMoreObjectives"));
        gotoNextReset(client, FINDTYPE_OBJECTIVE);
    }
}

void gotoNextDoor(ClientLink* client, int missiononly)
{
    DoorEntry* doors;
    int count, i;
    if (!client->entity) return;
    if (FINDTYPE_DOOR != client->findNextType)
        gotoNextReset(client, FINDTYPE_DOOR);

    doors = dbGetLocalDoors(&count);
    for (i = client->findNextCursor; i < count; i++)
    {
        if (missiononly && doors[i].type != DOORTYPE_MISSIONLOCATION)
            continue;
        conPrintf(client, "going to door %d", i+1);
        entUpdatePosInstantaneous(client->entity, doors[i].mat[3]);
        client->findNextCursor = i+1;
        return;
    }
    conPrintf(client, "There are no more doors on this map");
    gotoNextReset(client, FINDTYPE_DOOR);
}

void gotoEntity(int id, ClientLink* link){
    Entity* e = validEntFromId(id);

    if(e){
        entUpdatePosInstantaneous(link->entity, ENTPOS(e));
    }
}

void gotoEntityByName(char *name, ClientLink* link)
{
    Entity* e = findValidEntByName(name);

    if(e){
        entUpdatePosInstantaneous(link->entity, ENTPOS(e));
    }
}

void gotoNextNPCCluster(ClientLink* client)
{
    if (!client->entity) return;
    if (FINDTYPE_NPC_CLUSTER != client->findNextType)
        gotoNextReset(client, FINDTYPE_NPC_CLUSTER);

    client->findNextCursor = beaconGotoNPCCluster(client, client->findNextCursor);
}

static int sortEntRadius(const void* aData, const void* bData)
{
    const Entity ** a = (const Entity **)aData;
    const Entity ** b = (const Entity **)bData;
    if((*a)->motion->capsule.radius < (*b)->motion->capsule.radius) return -1;
    if((*a)->motion->capsule.radius > (*b)->motion->capsule.radius) return 1;
    return 0;
}

static void placeNpc(Entity * ent, Vec3 aroundPos, float spawnDist, float spawnAngle)
{
    Vec3 entOffsetVector;
    Vec3 spawnLocation;

    // Figure out where the entity is supposed to be placed.
    entOffsetVector[0] = cos(spawnAngle);
    entOffsetVector[1] = 0;
    entOffsetVector[2] = sin(spawnAngle);
    scaleVec3(entOffsetVector, spawnDist, entOffsetVector);

    // Place the entity there.
    addVec3(aroundPos, entOffsetVector, spawnLocation);
    entUpdatePosInstantaneous(ent, spawnLocation);

    aiInit(ent, NULL);
}

void testNpcs(ClientLink *client,int villain_cnt,int tmp_int,int tmp_int2)
{
    int i;
    Entity* ent;
    Vec3 aroundPos;
    float objectSpacing = 10.0;
    float spawnDist = objectSpacing;
    float totalAngle = 0.0;
    float spawnAngle = client->pyr[1];
    float angleIncrement;
    int spawnStartNum;
    int spawnCount;
    Entity ** largeEnts;

    if(tmp_int2 == -1){
        spawnStartNum = 0;
        spawnCount = villain_cnt;
    }
    else{
        spawnStartNum = tmp_int;
        spawnCount = tmp_int2;

        if(spawnStartNum < 0 || spawnStartNum >= villain_cnt){
            conPrintf(client, "Invalid initial npc number");
            conPrintf(client, "Pick a number from 0 to %i", villain_cnt);
        }
    }

    eaCreate(&largeEnts);

    copyVec3(ENTPOS(clientGetViewedEntity(client)), aroundPos);

    // Place all objects with a radius smaller than objectSpacing
    for( i = spawnStartNum; (i < spawnStartNum + spawnCount) && (i < villain_cnt); i++ ){
        const char* villainName;

        villainName = npcDefList.npcDefs[i]->name;
        printf("%i %s\n", i, villainName);

        // Try to spawn the npc.
        if(ent = npcCreate(villainName, ENTTYPE_NOT_SPECIFIED)){
            if((ent->motion->capsule.radius * 2.0) > objectSpacing) {
                eaPush(&largeEnts, ent);
                continue;
            }

            // Find the new entity position
            angleIncrement = objectSpacing / spawnDist;
            totalAngle += angleIncrement;
            if(totalAngle >= TWOPI) {
                spawnDist += objectSpacing;
                totalAngle = 0.0;
            }

            spawnAngle = addAngle(spawnAngle, angleIncrement);

            placeNpc(ent, aroundPos, spawnDist, spawnAngle);
        }
    }

    eaQSort(largeEnts, sortEntRadius);

    // Place all objects with larger radius
    for(i=0; i<eaSize(&largeEnts); i++)
    {
        ent = largeEnts[i];

        if(objectSpacing < (ent->motion->capsule.radius * 2.0))
        {
            objectSpacing = ent->motion->capsule.radius * 2.0;
        }

        // Find the new entity position
        angleIncrement = objectSpacing / (spawnDist + ent->motion->capsule.radius);
        totalAngle += angleIncrement;
        if(totalAngle >= TWOPI) {
            spawnDist += objectSpacing;
            totalAngle = 0.0;
        }

        spawnAngle = addAngle(spawnAngle, angleIncrement);

        placeNpc(ent, aroundPos, spawnDist + ent->motion->capsule.radius, spawnAngle);
    }
    
    eaDestroy(&largeEnts);
}

void printEntDebugInfo(ClientLink *client,int value,CmdContext* output)
{
    cmdOldApplyOperation(&client->entDebugInfo, value, output);
}

int GroupFindTrayTest(GroupDef* def, Mat4 parent_mat){
    DefTracker* currentRoom;
    Vec3 queryPos;

    // If we're looking at a tray, then perform a group find test.
    if(def->is_tray){
        copyVec3(parent_mat[3], queryPos);
        queryPos[1] += 0.5;

        currentRoom = groupFindInside(queryPos, FINDINSIDE_TRAYONLY,0,0);
        if(!currentRoom)
            printf("Can't find tray at %f %f %f\n", queryPos[0], queryPos[1], queryPos[2]);

        // Do not explore the tree further because trays are never enclosed by another.
        return 0;
    }

    return 1;
}
