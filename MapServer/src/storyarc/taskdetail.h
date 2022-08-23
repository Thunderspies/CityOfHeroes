/*\
 *
 *    taskdetail.h/c - Copyright 2003, 2004 Cryptic Studios
 *        All Rights Reserved
 *        Confidential property of Cryptic Studios
 *
 *    produces a small HTML page detailing player's progress
 *    along a compound task
 *
 */

#ifndef __TASKDETAIL_H
#define __TASKDETAIL_H

#include "storyarcinterface.h"
#include "storyarc/contactCommon.h"

// *********************************************************************************
//  Generating the detail page
// *********************************************************************************

void TaskGenerateDetail(int owner_id, Entity* sg_player, ContactHandle contactHandle, StoryTaskInfo* task, char response[LEN_CONTACT_DIALOG], char missionIntro[LEN_MISSION_INTRO]);

#endif //__TASKDETAIL_H