/*\
 *
 *    missionClient.h/c - Copyright 2003, 2004 Cryptic Studios
 *        All Rights Reserved
 *        Confidential property of Cryptic Studios
 *
 *    client UI stuff for tasks and missions, includes objective object code
 *
 */

#ifndef MISSIONCLIENT_H
#define MISSIONCLIENT_H

#include "storyarc/contactClient.h"

typedef struct Packet Packet;

// *********************************************************************************
//  Current mission and task status
// *********************************************************************************

char* MissionAddTimer(char* str, TaskStatus* task, int testwidth); // adds the mission timer to the string; if testwidth is set, maximum width string given
char* MissionSecondLine(TaskStatus* task, int showState);    // get a descriptive second line - either location or status
void MissionObjectiveStopTimer();

// *********************************************************************************
//  Mission objective objects 
// *********************************************************************************

void MissionReceiveObjectiveTimer(Packet* pak);
int MissionGetObjectiveTimer(float* timeRemaining, float* totalTime);
char* MissionGetObjectiveInteractString();

#endif