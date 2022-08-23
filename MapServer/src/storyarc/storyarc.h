/*\
 *
 *    storyarc.h/c - Copyright 2003, 2004 Cryptic Studios
 *        All Rights Reserved
 *        Confidential property of Cryptic Studios
 *
 *    storyarc static definitions, and storyarc functions
 *      - storyarcs are a way of linking tasks and encounters
 *        together.  they accomplish this by hooking into those
 *        systems and replacing them
 *
 */

#ifndef __STORYARC_H
#define __STORYARC_H

#include "storyarcinterface.h"

// *********************************************************************************
//  Handles and utility functions
// *********************************************************************************

int StoryArcProcessAndCheckLevels(StoryArc* story, int minlevel, int maxlevel);
void StoryArcPreload();
void StoryArcValidateAll();
int StoryArcNumDefinitions();
int StoryArcTotalTasks(void);


void StoryHandleSet(StoryTaskHandle *sahandle, int context, int subhandle, int cpos, bool playerCreated);
void StoryHandleCopy(StoryTaskHandle *dst, StoryTaskHandle *src);

ContactHandle StoryArcGetCurrentBroker(Entity* player, StoryInfo* info);
intptr_t StoryArcContextFromFileName(const char* filename);
StoryTaskHandle StoryArcGetHandle(const char* filename);
StoryTaskHandle StoryArcGetHandleLoose(const char* filename);
int ContactReferencesStoryArc(StoryContactInfo* contactInfo, StoryTaskHandle *sahandle);
const StoryArc* StoryArcDefinition(const StoryTaskHandle *sahandle);
int StoryArcCountTasks(const StoryTaskHandle *sahandle);
const char* StoryArcFileName(intptr_t i);
StoryTask* StoryArcTaskDefinition(const StoryTaskHandle *sahandle);
TaskSubHandle StoryArcGetTaskSubHandle(StoryTaskHandle *sahandle, StoryTask* taskdef);
int StoryArcForceGetTask(ClientLink* client, char* logicalname, char* episodename);

unsigned int StoryArcGetSeed(StoryInfo* info, StoryTaskHandle *sahandle);
StoryArcInfo* StoryArcGetInfo(StoryInfo* info, StoryContactInfo* contactInfo);
void StoryArcTaskEpisodeOffset(StoryTaskHandle *sahandle, int *episode, int *offset);
ContactHandle StoryArcGetContactHandle(StoryInfo* info, StoryTaskHandle *sahandle);
ContactHandle StoryArcFindContactHandle(StoryTaskHandle *sahandle);

StoryTaskInfo* StoryArcGetAssignedTask(StoryInfo* info, StoryTaskHandle *sahandle);
StoryTaskHandle PickStoryArc(Entity *pPlayer, StoryInfo* info, StoryContactInfo* contactInfo, int* isLong, int level, int* minlevel, const char **requiresFailed);
StoryArcInfo* AddStoryArc(StoryInfo* info, StoryContactInfo* contactInfo, StoryTaskHandle *sahandle);

// *********************************************************************************
//  Story arc processing - advancing episodes, etc.
// *********************************************************************************

StoryArcInfo* StoryArcForceAdd(Entity *pPlayer, StoryInfo* info, StoryContactInfo* contactInfo);
int StoryArcAddPlayerCreated(Entity *pPlayer, StoryInfo* info, int tf_id);
void RemoveStoryArc(Entity* pPlayer, StoryInfo* info, StoryArcInfo* storyarc, StoryContactInfo* contactInfo, int bClearAssignedFlag);
void FlashbackLeftRewardApplyToTaskforce(Entity *player);
void fixupStoryArcs(Entity *player);

// *********************************************************************************
//  Story arc hooks - basically all entrances to story arcs are by replacement
// *********************************************************************************

StoryArcInfo* StoryArcTryAdding(Entity* player, StoryInfo* info, StoryContactInfo* contactInfo, int mustGetArc, int* minlevel, const char **requiresFailed);
int StoryArcGetTask(Entity* player, StoryInfo* info, StoryContactInfo* contactInfo, int taskLength, ContactTaskPickInfo* pickinfo, int mustReplace, int mustGetArc,
                    const char **requiresFailedString, int justChecking);
TaskSubHandle StoryArcPickRandomEncounter(EncounterGroup* group, StoryInfo* info, StoryArcInfo* storyarc);
int StoryArcGetEncounter(EncounterGroup* group, Entity* player);
void StoryArcEncounterComplete(EncounterGroup* group, int success);
int StoryArcEncounterReward(EncounterGroup* group, Entity* rescuer, StoryReward* reward, Entity* speaker);
void StoryArcCloseTask(Entity* player, StoryContactInfo* contactInfo, StoryTaskInfo* task, ContactResponse* response);
void StoryArcPlayerEnteredMap(Entity* player);
void StoryArc_Tick(Entity* player);
void StoryArcAddCurrentBroker(Entity* player, StoryInfo* info);
void StoryArcAddCurrentPvPContact(Entity* player, StoryInfo* info);
int entity_IsOnStoryArc(Entity *e, const char *storyArcName);

int AllTeamMembersPassRequires(Entity *player, char **required, char *dataFilename);

// *********************************************************************************
//  Story arc debugging
// *********************************************************************************

void StoryArcDebugShowMineHelper(char ** estr, StoryInfo* info);

void DumpSpawndefs();

#endif // __STORYARC_H
