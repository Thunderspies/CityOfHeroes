#include "cmdparse/cmdserver.h"
#include <utilitieslib/utils/error.h>
#include <utilitieslib/components/earray.h>
#include "group/group.h"
#include "group/grouputil.h"
#include "entity/entity.h"

#include "zowie.h"
#include <utilitieslib/utils/mathutil.h>

#include "script/scriptutil.h"
#include "storyarc/storyarcprivate.h"
#include "dbcomm/dbcomm.h"

#include "group/groupProperties.h"
#include "entity/entityRef.h"
#include <utilitieslib/components/StashTable.h>
#include "entity/teamCommon.h"

static int mapname_equivilant(const char *name1, const char *name2)
{
    char n1[MAX_PATH];
    char n2[MAX_PATH];

    saUtilFilePrefix(n1, name1);
    saUtilFilePrefix(n2, name2);
    return !stricmp(n1, n2);
}


/**********************************************************************func*
* zowie_Destroy
*
*/
static void zowie_Destroy(Zowie *pZowie)
{
    if (pZowie->def != NULL)
    {
        free(pZowie->def);
    }
    if (pZowie->name != NULL)
    {
        free(pZowie->name);
    }
    if (pZowie->e != NULL)
    {
        entFree(pZowie->e);
    }

    free(pZowie);
}

static int zowie_LocationRecorder(GroupDefTraverser* traverser)
{
    char *pchZowie;
    Zowie *pZowie;

    // We are only interested in nodes with properties.
    if(!traverser->def->properties)
    {
        return traverser->def->child_properties != 0;
    }

    pchZowie = groupDefFindPropertyValue(traverser->def, "Zowie");
    if(pchZowie == NULL)
    {
        return 1;
    }

    //record the "plaque" part of the zowie
    pZowie = malloc(sizeof(Zowie));

    copyVec3(traverser->mat[3], pZowie->pos);
    pZowie->name = strdup(pchZowie);
    pZowie->e = NULL;
    pZowie->def = NULL;
    pZowie->last_zowie_bit = 0;
    pZowie->last_task = 0;
    pZowie->interact_entref = 0;

    eaPush(&g_Zowies, pZowie);

    return traverser->def->child_properties != 0;
}


void zowie_SetupEntities()
{
    int i;
    Zowie *pZowie;
    Mat4 mat;

    copyMat4(unitmat, mat);
    for (i = eaSize(&g_Zowies) - 1; i >= 0; i--)
    {
        pZowie = g_Zowies[i];
        if (pZowie->def == NULL)
        {
            pZowie->def = (void *) GlowieCreateDef("Name", "InvisibleActor", "ControlInteract", "ControlInterrupt", "ControlComplete", "ControlInteractBar", 3);
        }
        // Craft a matrix for this.  The PYR are effectively 0, 0, 0 due to copying in unitmat above.  This an invisible entity, just there for the benefit of being
        // a holding tank for the objective info.  Therefore we don't give a wet slap about the PYR, and we copy in the pos here.
        copyVec3(pZowie->pos, mat[3]);
        // Repeated calls to this will not leak Entities.  This gets called from one of two places: zowie_Load() below, which happens at map startup
        // time, at which point no entities have been allocated.  The other time is at the bottom of initMap().  Prior to reaching that point,
        // a call has been made to entKillAll(), which does the "take off and nuke the site from orbit" thing.
        pZowie->e = ZowiePlace(pZowie->def, mat, 1);
    }            
}

// Determine the number of objectives remaining for this zowie task
int zowie_ObjectivesRemaining(StoryTaskInfo *task)
{
    int total;
    unsigned int bitCount;
    char *poolSize;

    assert(task->def->type == TASK_ZOWIE);

    // In a zowie task, task->completeSideObjectives keeps a bitmap of the still-glowing zowies.  When first set up, this is simply the least
    // significant N bits all set to 1, and everything else 0, where N is the total pool size of the task.  Therefore, we can determine the
    // objectives remaining by taking the pool size, subtracting the bit count of completeSideObjectives from that to determine the number
    // that have been clicked.  We then subtract this result from the count required to get an answer.

    // First things first: total pool size.
    assert(task->def->zowiePoolSize && "Someone created a Zowie Task without a ZowiePoolSize entry.  Get design to fix this");
    total = 0;
    strdup_alloca(poolSize, task->def->zowiePoolSize);

    poolSize = strtok(poolSize, " ");
    while (poolSize != NULL)
    {
        total += atoi(poolSize);
        poolSize = strtok(NULL , " ");
    }

    // Now the count of bits in task->completeSideObjectives
    bitCount = task->completeSideObjectives - ((task->completeSideObjectives >> 1) & 0x55555555);
    bitCount = (bitCount & 0x33333333) + ((bitCount >> 2) & 0x33333333);
    bitCount = ((bitCount + (bitCount >> 4) & 0x0f0f0f0f) * 0x01010101) >> 24;
            
    // If you're having a WTF moment looking at this, ctrl-click on this link for details.
    // http://stackoverflow.com/questions/109023/best-algorithm-to-count-the-number-of-set-bits-in-a-32-bit-integer

    STATIC_INFUNC_ASSERT(sizeof(int) == 4);

    // If this assert ever trips, you're on something other than a 32 bit machine.  Most likely a 64 bit machine.
    // In which case, fixing the above is simple.  Just double the length of the five constants by duplication, and
    // change shift right on the right of the last expression from 24 to 56.

    // Yes, I did mean to parenthesize like this.  Read the comment at the top for why.
    return task->def->zowieCount - (total - bitCount);
}


/**********************************************************************func*
* zowie_Load
*
*/
void zowie_Load(void)
{
    GroupDefTraverser traverser = {0};
    loadstart_printf("Loading zowie locations.. ");

    // If we're doing a reload, clear out old data first.
    if (g_Zowies)
    {
        eaClearEx(&g_Zowies, zowie_Destroy);
        eaSetSize(&g_Zowies, 0);
    }
    else
    {
        eaCreate(&g_Zowies);
    }

    groupProcessDefExBegin(&traverser, &group_info, zowie_LocationRecorder);

    // Unfortunately, the deftracker scan does not appear to be deterministic as far as order is concerned.  However it is vital that
    // the g_Zowie arrays on client and server are stored in the same order.  So we qsort them by location.
    eaQSort(g_Zowies, zowieSortCompare);

    zowie_SetupEntities();

    loadend_printf("%d zowies found", eaSize(&g_Zowies));
}

static void update_zowie_tasks(Entity *e, int zi)
{
    int i;
    int objectiveBit;
    int itask;
    Zowie *pZowie;
    StoryInfo *psi;
    StoryTaskInfo *task;

    psi = StoryArcLockInfo(e);

    pZowie = g_Zowies[zi];

    for (itask = eaSize(&psi->tasks) - 1; itask >= 0; itask--)
    {
        task = psi->tasks[itask];

        // We check for TASK_COMPLETE to avoid the race condition that will occur when the remaining count is one, and two
        // players on the same team concurrently click on applicable zowies.  In that case the first player that gets to here
        // will complete the task, however the second will also arrive here a moment later.  In that case just ignore the task,
        // and keep searching.
        if (task->def->type == TASK_ZOWIE && !TASK_COMPLETE(task->state))
        {
            //If we've set a zowieMap, and they aren't on it, then this isn't it.
            if (task->def->villainMap && task->def->villainMap[0] && !mapname_equivilant(db_state.map_name, task->def->villainMap))
            {
                continue;
            }
            //check that zowie is the right type for this task.
            if (!zowie_type_matches(task->def->zowieType, pZowie->name))
            {
                continue;
            }
            objectiveBit = 1;
            for (i = 0; i < MAX_GLOWING_ZOWIE_COUNT; i++)
            {
                if (task->zowieIndices[i] == zi && (task->completeSideObjectives & objectiveBit))
                {
                    // Direct hit - this zowie actually is one that belongs to this mission, and it's still glowing
                    // Turn off the bit rather than flipping it.  That guarantees that a second run through here won't do
                    // unpleasant things
                    task->completeSideObjectives &= ~objectiveBit;
                    break;
                }
                objectiveBit <<= 1;
            }
            if (i == MAX_GLOWING_ZOWIE_COUNT)
            {
                // If we arrive here, it means the zowie type was a good match for the task, but this wasn't one explicitly allocated
                // to this task.  This will happen when multiple players are teamed with the same zowie types glowing.  They'll have
                // different pools, which means that for some players, there will be the chance that this is the correct type, but not
                // explicitly in their task pool.  In that case, the above loop will not get a hit, so we reach this fallback code.
                // This simply says that the first available zowie has been hit, and nukes that bit.
                // *IF* someone notices and bugs the fact that on zowie missions that accept two zowie types, this will heavily favor the
                // first type, we can fix this by shuffling the task->zowieIndices and task->zindices arrays right after they've been created,
                // using the seed and a single routine placed in zowieCommon.c.  It'd need a prototype:
                //    void zowieShuffleIndices(int *indices, int count, unsigned int seed)
                // and should use the same Rule 30 rand code as the generator in ZowieBuildPool(...)

                // Nuke the lowest bit of task->completeSideObjectives
                task->completeSideObjectives &= task->completeSideObjectives - 1;

                // It occurs to me that an easier way to do the random selection is to just pick a random bit.  /e headpalm. :)
            }

            // Mark the task as dirty so it updates
            ContactMarkDirty(e, psi, TaskGetContact(psi, task));    //psi->taskStatusChanged = 1;
            MissionRefreshAllTeamupStatus();    

            // And flag it complete if the objectives remaining is zero
            if (zowie_ObjectivesRemaining(task) <= 0)
            {
                TaskComplete(e, psi, task, NULL, 1);
            }
            break;
        }
    }

    StoryArcUnlockInfo(e);
}

static StoryTaskInfo *check_entity_for_zowie_tasks(Entity *e, int zi)
{
    int i;
    int itask;
    Zowie *pZowie;
    StoryInfo *psi;
    StoryTaskInfo *task;

    psi = StoryInfoFromPlayer(e);
    if (!psi)
    {
        return NULL;
    }

    pZowie = g_Zowies[zi];

    for (itask = eaSize(&psi->tasks) - 1; itask >= 0; itask--)
    {
        task = psi->tasks[itask];

        if (task->def->type == TASK_ZOWIE)
        {
            // Trap Zowie tasks with no villain map
            assertmsg(task->def->villainMap && task->def->villainMap[0], "Zowie task found with no villainMap.  Please get design to fix this ASAP");
            if (task->def->villainMap == NULL || task->def->villainMap[0] == 0)
            {
                continue;
            }
            if (!mapname_equivilant(db_state.map_name, task->def->villainMap))
            {
                continue;
            }
            //check that zowie is the right type for this task.
            if (!zowie_type_matches(task->def->zowieType, pZowie->name))
            {
                continue;
            }

            for (i = 0; i < MAX_GLOWING_ZOWIE_COUNT; i++)
            {
                // Check if this zowie is directly active for this task
                if (task->zowieIndices[i] == zi)
                {
                    return task;
                }
            }
        }
    }
    return NULL;
}

static StoryTaskInfo *check_team_for_zowie_task(Entity *e, int zi)
{
    int ti;
    Entity *te;
    StoryTaskInfo *task;

    if ((task = check_entity_for_zowie_tasks(e, zi)) != NULL)
    {
        return task;
    }

    if (e->teamup != NULL)
    {
        for (ti = 0; ti < e->teamup->members.count; ti++)
        {
            if ((te = entFromDbId(e->teamup->members.ids[ti])) != NULL && (task = check_entity_for_zowie_tasks(te, zi)) != NULL)
            { 
                return task;
            }
        }
    } 
    return NULL;
}

//Called when player finishes clicking a zowie
int ZowieComplete(Entity *player, MissionObjectiveInfo *target)
{
    int zi;
    EntityRef er;
    Entity *e;
    int ti;
    Entity *te;

    er = erGetRef(player);
    if (er == 0)
    {
        return 0;
    }

    //This might seem silly (why not just use player?)
    // it's because player may not actually be the same pointer if the player left the map
    // between starting to click and finishing
    e = erGetEnt(er);
    if (e == NULL)
    {
        return 0;
    }

    //Figure out which zowie was being clicked 
    // We added 1 when clickedZowieIndex was set, so it'll never be zero, which represents "unset".  Thus we subtract 1 now.
    zi = e->pl->clickedZowieIndex - 1;
    // And reset to zero
    e->pl->clickedZowieIndex = 0;

    if (zi < 0 || zi >= eaSize(&g_Zowies))
    {
        return 0;
    }

    //Whenever a zowie is hit, need to check every task of every team member to see if any had a task that zowie advances.
    if (e->teamup)
    {
        for (ti = 0; ti < e->teamup->members.count; ti++)
        {
            te = entFromDbId(e->teamup->members.ids[ti]);
            if (te)
            {
                update_zowie_tasks(te, zi);
            }
        }
    }
    else
    {
        update_zowie_tasks(e, zi);
    }
    return 0;
}


bool zowie_ReceiveReadZowie(Packet *pak, Entity *e)
{
    char *pch;
    Vec3 pos;
    int i;
    Zowie *pZowie;
    Entity *ze;
    StoryTaskInfo *task;
    MissionObjectiveInfo *zinfo;    

    // CLIENTINP_ZOWIE
    // string name of zowie
    // position:
    //   float vec3[0]
    //   float vec3[1]
    //   float vec3[2]
    pch = pktGetString(pak);
    pos[0] = pktGetF32(pak);
    pos[1] = pktGetF32(pak);
    pos[2] = pktGetF32(pak);

    i = find_zowie_from_pos_and_name(pch, pos);
    if (i < 0)
    {
        return false;
    }

    pZowie = g_Zowies[i];

    //check that the player is actually close enough to click the zowie.
    if(distance3Squared(ENTPOS(e), pZowie->pos) > SQR(21))
    {
        return false;
    }

    ze = pZowie->e;
    if (ze == NULL)
    {
        return true;    //This should never happen ....
    }

    //Does this player have that zowie as a mission objective?
    // or does anyone on their team?

    task = check_team_for_zowie_task(e, i);
    zinfo = ze->missionObjective;
    if (task != NULL && zinfo != NULL)
    {
        if (zinfo->interactPlayerDBID == 0)
        {
            MissionObjective *zdef = cpp_const_cast(MissionObjective*)(zinfo->def); // these are mutable for zowies

            // these are are probably in the wrong structure
            zdef->interactDelay = task->def->interactDelay;

            if (zdef->interactBeginString != NULL)
            {
                free(zdef->interactBeginString);
            }
            zdef->interactBeginString = strdup(task->def->interactBeginString);

            if (zdef->interactCompleteString != NULL)
            {
                free(zdef->interactCompleteString);
            }
            zdef->interactCompleteString = strdup(task->def->interactCompleteString);

            if (zdef->interactInterruptedString != NULL)
            {
                free(zdef->interactInterruptedString);
            }
            zdef->interactInterruptedString = strdup(task->def->interactInterruptedString);

            if (zdef->interactActionString != NULL)
            {
                free(zdef->interactActionString);
            }
            zdef->interactActionString = strdup(task->def->interactActionString);

            pZowie->interact_entref = erGetRef(e);

            // clickedZowieIndex will be inited to zero by the code that sets up a player entity.  Therefore we add one so 0 will remain our "not set" value
            e->pl->clickedZowieIndex = i + 1;
        }
        MissionItemInteract(ze, e);
    }
    return true;
}

//returns 0 on failure.
int zowie_setup(StoryTaskInfo* task, int fullInit)
{
    //U32 bits_to_set;
    //char *ztype;
    //int zcount;
    int zneeded;
    char *type;
    char *poolSize;
    int total;
    int size;
    int available;
    U32 seed;
    int i;
    int j;
    int index;
    int poolCount;
    int indices[MAX_GLOWING_ZOWIE_COUNT];
    int poolSizes[MAX_ZOWIE_TYPE];

    // First thing, do some parameter sanity checks.  Make sure all pointers are valid, and that this actually is a Zowie task.
    if (task == NULL || task->def == NULL || task->def->type != TASK_ZOWIE)
    {
        return 1;
    }
    
    //ztype = task->def->zowieType;
    zneeded = task->def->zowieCount;

    if (fullInit)
    {
        // Do some more sanity checking when we first hand the mission out
        if (zneeded <= 0 || zneeded > 32)
        {
            ErrorFilenamef(task->def->filename, "TaskZowie: Task has bad or missing zowie click value %d, must be between 1 and 32", zneeded);
            return 0;
        }
        if (task->def->zowieType == NULL || task->def->zowiePoolSize == NULL)
        {
            ErrorFilenamef(task->def->filename, "TaskZowie: Task is missing either ZowieType or ZowiePoolSize or both");
            return 0;
        }
        // Compound zowie tasks all inherit the seed from the parent taskdef.  Therefore if they all use the same type of zowie they will always
        // select the same pools.  So we fix the problem by setting the seed here.
        task->seed = timerSecondsSince2000();
    }

    // In hindsight, this should probably be two routines, one called from StoryArcPlayerEnteredMap(...), and the other
    // from SubTaskInstantiate(...)

    // This first portion is relevant only when first setting up the task, as in when we're called from SubTaskInstantiate(...)
    total = 0;
    strdup_alloca(poolSize, task->def->zowiePoolSize);

    poolSize = strtok(poolSize, " ");
    for (poolCount = 0; poolSize != NULL && poolCount < MAX_ZOWIE_TYPE; poolCount++)
    {
        poolSizes[poolCount] = atoi(poolSize);
        total += poolSizes[poolCount];
        poolSize = strtok(NULL , " ");
    }

    if (poolSize != NULL)
    {
        ErrorFilenamef(task->def->filename, "TaskZowie: Too many pools %d, max is %d", poolCount, MAX_ZOWIE_TYPE);
        return 0;
    }

    if (zneeded > total)
    {
        ErrorFilenamef(task->def->filename, "TaskZowie: Requested pool size %d too small, %d zowies needed", total, zneeded);
        return 0;
    }
    if (total > MAX_GLOWING_ZOWIE_COUNT)
    {
        ErrorFilenamef(task->def->filename, "TaskZowie: Requested pool size %d too large, max is %d", total, MAX_GLOWING_ZOWIE_COUNT);
        return 0;
    }

    if (fullInit)
    {
        task->completeSideObjectives = (1 << total) - 1;
    }

    // This part is the section that is only relevant when called from StoryArcPlayerEnteredMap(...), it just happens to get
    // benignly executed if the zowie task is instantiated while the player is on the map where the zowies live.
    if (task->def->villainMap && task->def->villainMap[0] && mapname_equivilant(task->def->villainMap, db_state.map_name))
    {
        seed = task->seed;
        total = 0;
        strdup_alloca(type, task->def->zowieType);

        type = strtok(type, " ");
        for (i = 0; type != NULL && i < MAX_ZOWIE_TYPE; i++)
        {
            // detect more ZowieTypes than ZowiePoolSizes
            if (i >= poolCount)
            {
                i = poolCount + 1;
                break;
            }
            size = poolSizes[i];
            available = count_matching_zowies_up_to(type, eaSize(&g_Zowies));
            if (size > available)
            {
                ErrorFilenamef(task->def->filename, "TaskZowie: pool size %d of %s requested too large, only %d available", size, type, available);
                return 0;
            }

            seed = ZowieBuildPool(size, available, seed, indices);
            for (j = 0; j < size; j++)
            {
                index = find_nth_zowie_of_this_type(type, indices[j]);
                if (index == -1)
                {
                    ErrorFilenamef(task->def->filename, "TaskZowie: something went horribly wrong in find_nth_zowie_of_this_type().  Talk to David G.");
                    return 0;
                }
                // This won't overflow due to the "total > MAX_GLOWING_ZOWIE_COUNT" check above
                task->zowieIndices[total++] = index;
            }

            type = strtok(NULL, " ");
        }
        if (type != NULL || i != poolCount)
        {
            ErrorFilenamef(task->def->filename, "TaskZowie: mismatched element counts between ZowieType and ZowiePoolSize");
            return 0;
        }
    }
    return 1;
}