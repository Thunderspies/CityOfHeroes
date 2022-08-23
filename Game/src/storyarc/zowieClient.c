// Zowie stuff
// This handles two client side details of Zowies: 1) making them hum and glow in the world, and 2) making the active task Zowies appear on the minimap.

#include "group/grouputil.h"
#include "group/groupProperties.h"
#include "group/group.h"
#include "storyarc/contactClient.h"
#include <utilitieslib/components/StashTable.h>
#include "cmdparse/cmdgame.h"
#include "sound/sound.h"
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/error.h>
#include "UI/uiAutomap.h"
#include "graphics/textureatlas.h"
#include "storyarc/zowieClient.h"

// Icon to use for zowies on the minimap
#define ZOWIE_ICON    "Zowie_Minimap_Icon"

// Stash table of zowies that should appear on the minimap.
StashTable zowieDestList = NULL;

// pointer to task data for current task, but only if it's a zowie task.  This is used to track minimap icons when we get
// a task status update.
static TaskStatus *currentZowieTask = NULL;

// This flag is cleared when groupTrackersHaveBeenReset() in group.c calls zowieClearTrackers().  It's state signals us
// as to whether the def members of the zowie list are valid or not.  It gets set again when clientZowie_Load() is
// called to hook up zowies for the new map.
static int defsAreValid = 0;

// Update all zowie sounds.  If pulse_glow is set, we simply play objective_loop and call it a job well done.  If pulse_glow is
// clear, we then look at play_objective_sound.  If we find that set, it means the zowie has just stopped glowing.  In that case
// we use soundFade to cause the sound to fade out over a 1 second perid, at which point we turn off play_objective_sound.
void zowieSoundTick()
{
    int i;

    if (defsAreValid)
    {
        for (i = eaSize(&g_Zowies) - 1; i >= 0; i--)
        {
            // If this one is humming ...
            if (g_Zowies[i]->def->play_objective_sound)
            {
                // Special code to handle the fact that it's stopped glowing.
                if (g_Zowies[i]->def->pulse_glow == 0)
                {
                    // Ramp down the sound over one second.
                    g_Zowies[i]->soundFade -= TIMESTEP_NOSCALE / 30.0f;
                    if (g_Zowies[i]->soundFade <= 0.0f)
                    {
                        // And when it reaches zero turn everything off
                        g_Zowies[i]->soundFade = 0.0f;
                        g_Zowies[i]->def->play_objective_sound = 0;
                    }
                }
                sndPlaySpatial("objective_loop", SOUND_ZOWIE_BASE + i, 0, g_Zowies[i]->pos, 10.0f, 100.0f,
                    0.8f * g_Zowies[i]->soundFade, 0.8f, SOUND_PLAY_FLAGS_INTERRUPT, 0, 0.0f);
            }
        }
    }
}

// This guy is called by the tracker walker.  If it finds a tracker with the Zowie property, it creates
// an entry in the g_Zowies earray for it.
static int clientZowie_LocationRecorder(GroupDefTraverser* traverser)
{
    char *pchZowie;
    Zowie *pZowie;

    // We are only interested in nodes with properties.
    if (!traverser->def->properties)
    {
        return traverser->def->child_properties != 0;
    }

    // More specifically, we're only interested if it has the Zowie property
    pchZowie = groupDefFindPropertyValue(traverser->def, "Zowie");
    if (pchZowie == NULL)
    {
        return 1;
    }

    // FIXME - use a memory pool for this.
    pZowie = malloc(sizeof(Zowie));

    // Save the name, location
    pZowie->name = strdup(pchZowie);
    copyVec3(traverser->mat[3], pZowie->pos);
    pZowie->def = traverser->def;
    pZowie->soundFade = 0.0f;

    eaPush(&g_Zowies, pZowie);

    return traverser->def->child_properties != 0;
}

/**********************************************************************func*
* clientZowie_Destroy
*
*/
static void clientZowie_Destroy(Zowie *pZowie)
{
    if (pZowie->name)
    {
        free(pZowie->name);
    }
    free(pZowie);
}

/**********************************************************************func*
* zowie_Load
*
*/
void clientZowie_Load()
{
    GroupDefTraverser traverser = {0};
    writeConsole(OUTPUT_DEBUG, "Loading zowie locations");

    // If we're doing a reload, clear out old data first.
    if (g_Zowies)
    {
        eaClearEx(&g_Zowies, clientZowie_Destroy);
        eaSetSize(&g_Zowies, 0);
    }
    else
    {
        eaCreate(&g_Zowies);
    }

    groupProcessDefExBegin(&traverser, &group_info, clientZowie_LocationRecorder);

    // Unfortunately, the deftracker scan does not appear to be deterministic as far as order is concerned.  However it is vital that
    // the g_Zowie arrays on client and server are stored in the same order.  So we qsort them by location.
    eaQSort(g_Zowies, zowieSortCompare);

    // Singal that we have valid def pointers
    defsAreValid = 1;

    writeConsole(OUTPUT_INFO, "Loaded %d zowies", eaSize(&g_Zowies));
}

static void clientZowie_SetUpIndices(TaskStatus *task)
{
    int i;
    int j;
    int index;
    int total;
    int size;
    int available;
    U32 seed;
    char *type;
    int indices[MAX_GLOWING_ZOWIE_COUNT];

    // yeah - this is a cut and paste job on zowie_setup(...) over in zowie.c.  I just don't think I can get a common routine to do this, since the Zowie
    // struct changes between server and client.  Also, the handling of the poolsize on the client would have to change.
    total = 0;
    seed = task->zseed;
    strdup_alloca(type, task->ztype);

    i = 0;
    type = strtok(type, " ");
    while (type != NULL)
    {
        size = task->zpoolSize[i++];
        available = count_matching_zowies_up_to(type, eaSize(&g_Zowies));
        // This and the following asserts inside the for loop should never trip, since the server already vetted the data
        assert(size <= available);

        seed = ZowieBuildPool(size, available, seed, indices);
        for (j = 0; j < size; j++)
        {
            index = find_nth_zowie_of_this_type(type, indices[j]);
            assert(index != -1);
            assert(total < MAX_GLOWING_ZOWIE_COUNT);
            task->zIndices[total++] = index;
        }
        type = strtok(NULL, " ");
    }
    // Run through the array, and turn off any entries for which the corresponding bit in zinfo is clear.
    j = task->zinfo;
    for (i = 0; i < total; i++)
    {
        if ((j & 1) == 0)
        {
            task->zIndices[i] = -1;
        }
        j >>= 1;
    }

    // Pad the array with -1's it makes the add / remove glow code several orders of magnitude more sane
    while(total < MAX_GLOWING_ZOWIE_COUNT)
    {
        task->zIndices[total++] = -1;
    }
}

void updateAutomapZowies(TaskStatus *task, int cacheControl)
{
#ifndef TEST_CLIENT
    int i;
    int index;
    StashElement waypoint;
    StashTableIterator iter;
    Destination *dest;
    char key[20];

    // Start these at a non-zero value so they don't conflict
    // with script waypoints
    static int zowieID = 1000000;

    // DGNOTE 7/22/2010
    // cacheControl determines how we treat the first parameter and the cache
    switch (cacheControl)
    {
    xcase USE_CACHED:
        // We're not setting a new cached value, simply use the old one.
        task = currentZowieTask;
    xcase SET_CACHED:
        // Do a few sanity checks first: if the task isn't going to show any minimap icons set everything to NULL
        if (task && task->isZowie && task->zmap_id == game_state.base_map_id)
        {
            // Looks good - save it.
            currentZowieTask = task;
        }
        else
        {
            // Else nuke both the task value and the cached value;
            task = NULL;
            currentZowieTask = NULL;
        }
    xcase DESTROY_CACHED:
        // Destroying the passed parameter, clear the cache if it's the same as the cached value
        if (task == currentZowieTask)
        {
            // Nuke both the task value and the cached value;
            task = NULL;
            currentZowieTask = NULL;
        }
    }

    if (zowieDestList == NULL)
    {
        zowieDestList = stashTableCreateWithStringKeys(20, StashDeepCopyKeys | StashCaseSensitive );
    }
    else
    {
        // always remove all waypoints
        stashGetIterator(zowieDestList, &iter);
        while (stashGetNextElement(&iter, &waypoint))
        {
            if ((dest = stashElementGetPointer(waypoint)) != NULL)
            {
                compass_ClearAllMatchingId(dest->id);
                clearDestination(dest);
                stashRemovePointer(zowieDestList, stashElementGetStringKey(waypoint), NULL);
                free(dest);
            }
        }
    }

    // I'm not 100% sure it's possible to reach here with task != NULL but either of the other two conditions failing.
    // However, good defensive programming always wins.
    if (task && task->isZowie && task->zmap_id == game_state.base_map_id)
    {
        int destinationSet = 0;
        clientZowie_SetUpIndices(task);
        for (i = 0; i < MAX_GLOWING_ZOWIE_COUNT; i++)
        {
            index = task->zIndices[i];
            if (index != -1)
            {
                _snprintf_s(key, sizeof(key), sizeof(key) - 1, "%d", ++zowieID);

                dest = malloc(sizeof(Destination));
                if (dest != NULL)
                {
                    memset(dest, 0, sizeof(Destination));

                    dest->navOn = 1;
                    dest->type = ICON_ZOWIE;
                    dest->id = -zowieID;
                    dest->contactType = CONTACTDEST_EXPLICITTASK;
                    strncpyt(dest->name, task->zname, sizeof(dest->name));
                    strncpyt(dest->mapName, game_state.world_name, sizeof(dest->mapName));

                    dest->icon = atlasLoadTexture(ZOWIE_ICON);

                    copyVec3(g_Zowies[index]->pos, dest->world_location[3]);

                    stashAddPointer(zowieDestList, key, dest, false);

                    if(!destinationSet)
                    {
                        // location.id is always set on TaskStatusReceive, so this should be legit...
                        int id = task->location.id;
                        task->hasLocation = 1;
                        task->location = *dest;
                        task->location.id = id;
                        destinationSet = 1;
                    }
                }
            }
        }
    }
#endif
}

void zowieAddGlowFlags(TaskStatus *task)
{
#ifndef TEST_CLIENT
    int i;
    int index;

    if (task->zmap_id == game_state.base_map_id && defsAreValid)
    {
        // Hijack the add routine to set up the indices of the zowies in the global array.
        clientZowie_SetUpIndices(task);
        for (i = 0; i < MAX_GLOWING_ZOWIE_COUNT; i++)
        {
            index = task->zIndices[i];
            if (index != -1)
            {
                // The glow and hum flags are maintained separately, in case this tech is ever needed for something that needs to glow without
                // humming or vice versa
                g_Zowies[index]->def->pulse_glow = 1;
                g_Zowies[index]->def->play_objective_sound = 1;
                g_Zowies[index]->soundFade = 1.0f;
            }
        }
    }
#endif
}

// zowieRemoveGlowFlags()
//
// Delta tracking on zowies glowing just isn't worth the effort, so whenever we think anything has changed, clear everything
// and then turn on only these than need it.  This is the turn off routine, it simply removes the glow and hum flags from every
// zowie in the zone.
void zowieRemoveGlowFlags()
{
#ifndef TEST_CLIENT
    int i;

    if (defsAreValid)
    {
        for (i = eaSize(&g_Zowies) - 1; i >= 0; i--)
        {
            // The glow and hum flags are maintained separately, in case this tech is ever needed for something that needs to glow without
            // humming or vice versa
            g_Zowies[i]->def->pulse_glow = 0;
            g_Zowies[i]->def->play_objective_sound = 0;
            g_Zowies[i]->soundFade = 0.0f;
        }
    }
#endif
}

// This is called from groupTrackersHaveBeenReset() so that we can signal that the defs are no longer valid
void zowieReset()
{
    // TODO: remove this once the zowie crash bug is verified fixed.  This dangling if() gates the next line where we clear defsAreValid
    if (game_state.test_zowie_crash == 0)

    // Signal that def pointers are not valid
    defsAreValid = 0;
}
