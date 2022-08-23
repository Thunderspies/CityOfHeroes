/*\
 *
 *    locationtask.h/c - Copyright 2003, 2004 Cryptic Studios
 *        All Rights Reserved
 *        Confidential property of Cryptic Studios
 *
 *    functions related to locationTasks
 *
 */

#include "task.h"
#include "storyarcprivate.h"
#include "group/group.h"
#include "group/groupProperties.h"
#include "group/grouputil.h"
#include "group/groupnetsend.h"
#include "dbcomm/staticMapInfo.h"
#include "entity/entity.h"

// *********************************************************************************
//  Visit location list
// *********************************************************************************

VisitLocation** visitLocations;
// visitLocations contains only locations on THIS MAP.  It can only be used for proximity checking
// when the player is actually touching the location.

VisitLocation* VisitLocationCreate()
{
    return calloc(1, sizeof(VisitLocation));
}

void VisitLocationDestroy(VisitLocation* location)
{
    if(location->name)
        free(location->name);
    free(location);
}

// Returns NULL if no locations with the given name is found.
VisitLocation* VisitLocationFind(const char* name)
{
    int i;
    if (!name) return NULL;
    for(i = eaSize(&visitLocations)-1; i >= 0; i--)
    {
        VisitLocation* location = visitLocations[i];

        if(stricmp(location->name, name) == 0)
            return location;
    }

    return NULL;
}

int visitLocationRecorder(GroupDefTraverser* traverser)
{
    // We are only interested in nodes with properties.
    if(!traverser->def->properties)
    {
        // We're only interested in subtrees with properties.
        if(!traverser->def->child_properties)
            return 0;

        return 1;
    }

    {
        char* locationName;
        VisitLocation* loc;
        int i;

        // Does this thing belong to a location group?
        locationName = groupDefFindPropertyValue(traverser->def, "VisitLocation");
        if(!locationName)
            return 1;
        if (strcmp(locationName, "0")==0) // if not set, ignore
            return 1;

        for(i = eaSize(&visitLocations)-1; i >= 0; i--)
        {
            VisitLocation* loc = visitLocations[i];
            if(stricmp(loc->name, locationName) == 0)
            {
                ErrorFilenamef(saUtilCurrentMapFile(), "Duplicate VisitLocation \"%s\" found at (%0.2f, %0.2f, %0.2f) and (%0.2f, %0.2f, %0.2f)\n",
                    locationName,
                    loc->coord[0], loc->coord[1], loc->coord[2],
                    traverser->mat[3][0], traverser->mat[3][1], traverser->mat[3][2]);
                return 0;
            }
        }

        // Create a structure to hold info about this particular location.
        loc = VisitLocationCreate();
        loc->name = strdup(locationName);
        copyVec3(traverser->mat[3], loc->coord);

        eaPush(&visitLocations, loc);
    }

    // Technically, we should be able to return 0 here since there shouldn't
    // be any nested locations. Reality being what it is, though, there
    // seem to be some due to improper grouping by designers. Since there's
    // no (known) bad side-effects to it, we'll keep traversing.
    return 1;
}

static void VerifyMapLocations(void)
{
    const char** requiredLocations;
    int i, n = 0;
    
    requiredLocations = MapSpecVisitLocations(saUtilCurrentMapFile());
    if (requiredLocations)
        n = eaSize(&requiredLocations);
    for (i = 0; i < n; i++)
    {
        int loc, numloc;
        int found = 0;
        const char* location = requiredLocations[i];
        numloc = eaSize(&visitLocations);
        for (loc = 0; loc < numloc; loc++)
        {
            if (!stricmp(visitLocations[loc]->name, location))
            {
                found = 1;
                break;
            }
        }
        if (!found)
        {
            ErrorFilenamef(saUtilCurrentMapFile(), "Map required to have a %s visit location, but does not", location);
        }
    }
}

void VisitLocationLoad(void)
{
    GroupDefTraverser traverser = {0};

    loadstart_printf("Loading visit errand locations..");

    // If we're doing a reload, clear out old data first.
    if(visitLocations)
    {
        eaClearEx(&visitLocations, VisitLocationDestroy);
        eaSetSize(&visitLocations, 0);
    }
    else
        eaCreate(&visitLocations);

    // Extract all visit locations.
    groupProcessDefExBegin(&traverser, &group_info, visitLocationRecorder);
    VerifyMapLocations();

    loadend_printf("%i locations found", eaSize(&visitLocations));
}

void VisitLocationPrintAllOnMap(ClientLink* client)
{
    FILE* f = 0;
    int i, n;

    // print them
    if (isDevelopmentMode())
    {
        f = fopen("c:\\visitlocationlist.txt", "wb");
        fwrite("VisitLocations ", 1, strlen("VisitLocations "), f);
    }
    conPrintf(client, "Visit Locations on this map:\n");
    
    n = eaSize(&visitLocations);
    for (i = 0; i < n; i++)
    {
        conPrintf(client, "  %s\n", visitLocations[i]->name);
        if (f)
        {
            char buf[200];
            sprintf(buf, "%s, ", visitLocations[i]->name);
            fwrite(buf, 1, strlen(buf), f);
        }
    }

    if (f) fclose(f);
}

// *********************************************************************************
//  Location task functions
// *********************************************************************************

static int LocationTaskVisitedAll(StoryTaskInfo* task)
{
    int n = eaSize(&task->def->visitLocationNames);
    if (task->nextLocation >= n) return 1;
    return 0;
}

const char* LocationTaskNextLocation(StoryTaskInfo* task)
{
    if (LocationTaskVisitedAll(task)) return NULL;
    return task->def->visitLocationNames[task->nextLocation];
}

void LocationTaskVisit(Entity* player, char* locationName, Vec3 coord)
{
    VisitLocation* location;
    StoryInfo* info;
    int i;

    if (!player)
        return;

    // Is the player within tolerable proximity of the location?
    if(!(distance3Squared(ENTPOS(player), coord) <
        STORYARC_TOUCH_DISTANCE*STORYARC_TOUCH_DISTANCE))
        return;

    // Does the player specified location exist?
    location = VisitLocationFind(locationName);
    if(!location)
        return;

    // Is the specified coordinate a valid location to visit?
    if(location->coord[0] != coord[0] || location->coord[1] != coord[1] || location->coord[2] != coord[2])
        return;

    // Look for visit location tasks
    info = StoryArcLockInfo(player);

    // Walk backward through the array, because items might be removed by TaskComplete.
    for(i = eaSize(&info->tasks) - 1; i >= 0; i--)
    {
        StoryTaskInfo* task = info->tasks[i];
        const char* nextloc = NULL;

        if(TaskCompleted(task) || !(task->def->type == TASK_VISITLOCATION))
            continue;

        // did the task definition change under us?
        nextloc = LocationTaskNextLocation(task);
        if (!nextloc)
        {
            TaskComplete(player, info, task, NULL, 1);
        }
        else
        {
            // did the player visit the next location?
            if (!stricmp(LocationTaskNextLocation(task), locationName))
            {
                info->visitLocationChanged = 1;
                task->nextLocation++;
                ContactMarkDirty(player, info, TaskGetContact(info, task));

                if (LocationTaskVisitedAll(task))
                    TaskComplete(player, info, task, NULL, 1);
            }
        }
    }
    StoryArcUnlockInfo(player);
}

void LocationTaskVisitReceive(Entity* player, Packet* pak)
{
    char* str;
    char locationName[1024];
    Vec3 coord;

    str = pktGetString(pak);
    strncpy(locationName, str, 1023);
    locationName[1023] = 0;

    coord[0] = pktGetF32(pak);
    coord[1] = pktGetF32(pak);
    coord[2] = pktGetF32(pak);
    LocationTaskVisit(player, locationName, coord);
}

