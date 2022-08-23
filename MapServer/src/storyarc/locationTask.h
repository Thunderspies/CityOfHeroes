/*\
 *
 *    locationtask.h/c - Copyright 2003, 2004 Cryptic Studios
 *        All Rights Reserved
 *        Confidential property of Cryptic Studios
 *
 *    functions related to locationTasks
 *
 */

#ifndef __LOCATIONTASK_H
#define __LOCATIONTASK_H

#include "storyarcinterface.h"

typedef struct
{
    char* name;
    Vec3 coord;
} VisitLocation;

extern VisitLocation** visitLocations;

// *********************************************************************************
//  Visit location list
// *********************************************************************************

VisitLocation* VisitLocationFind(const char* name);
void VisitLocationLoad(void);
void VisitLocationPrintAllOnMap(ClientLink* client);

// *********************************************************************************
//  Location task functions
// *********************************************************************************

const char* LocationTaskNextLocation(StoryTaskInfo* task);
void LocationTaskVisitReceive(Entity* player, Packet* pak);

#endif // __LOCATIONTASK_H