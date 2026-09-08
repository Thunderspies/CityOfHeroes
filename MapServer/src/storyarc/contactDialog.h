#include "entity/teamCommon.h"
/*\
 *
 *    contactDialog.h/c - Copyright 2003, 2004 Cryptic Studios
 *        All Rights Reserved
 *        Confidential property of Cryptic Studios
 *
 *    the main contact dialog function
 *
 */

#ifndef __CONTACTDIALOG_H
#define __CONTACTDIALOG_H

#include "storyarcinterface.h"
#include "entity/character_base.h"

typedef struct
{
    int link;
    Entity* player;
    StoryInfo* info;
    StoryContactInfo* contactInfo;
    ContactResponse* response;
    ScriptVarsTable* vars;
} ContactInteractionContext;

// *********************************************************************************
//  Main contact dialog
// *********************************************************************************

void AddResponse(ContactResponse* response, const char* text, int link);
int ContactInteraction(Entity* player, ContactHandle contactHandle, int link, ContactResponse* response);
char* ContactHeadshot(Contact* contact, Entity* player, int aboutlink);

// *********************************************************************************
//  Contact task picking
// *********************************************************************************

void ContactHideNonUrgentTask(ContactTaskPickInfo* shortpick, ContactTaskPickInfo* longpick);
int ContactPickTask(Entity* player, StoryInfo* info, StoryContactInfo* contactInfo, int taskLength, ContactTaskPickInfo* pickinfo, int justChecking);
int ContactPickSpecificTask(Entity* player, StoryInfo* info, StoryContactInfo* contactInfo, const char *taskName, ContactTaskPickInfo* pickinfo);
int ContactPickHeistTask(Entity* player, StoryInfo* info, StoryContactInfo* contactInfo, ContactTaskPickInfo* pickinfo);
int ContactPickNewspaperTask(Entity* player, StoryInfo* info, StoryContactInfo* contactInfo, ContactTaskPickInfo* pickinfo);
int ContactPickPvPTask(Entity* player, StoryInfo* info, StoryContactInfo* contactInfo, int index, ContactTaskPickInfo* pickinfo);
int ContactShouldGiveHeist(StoryContactInfo *contactInfo);
int ContactHasOnlyStoryArcTasks(const ContactDef* contactdef);
int ContactHasNewTasksForPlayer(Entity *pPlayer, StoryContactInfo *pContact, StoryInfo *pInfo);
int ContactCanIntroduceNewContacts(Entity *player, StoryInfo *info, StoryContactInfo *contactInfo);
void ContactInteractionPromptNewTasks(ContactInteractionContext *context, int index, const char *forcedMissionName);

int ContactNewspaperTaskHistoryLookup(StoryInfo* info, const char* unique, int historyIndex);
int ContactNewspaperTaskHistoryUpdate(StoryInfo* info, const char* unique, int historyIndex);

void TaskforceAccept(Entity *player, TFParamTimeLimits timeLimits, int limitedLives, TFParamPowers powerLimits,
                     bool debuff, bool buff, bool noEnhancements, bool noInspirations, char *fileID);

void TaskForceArchitectStart( Entity * e, char* pchPlayerArc, int mission_id, int test_mode, int arc_flags, int authorid );

// exposed so they can be called in DeliveryTaskContact().
void ContactTaskForceInteraction(ContactInteractionContext* context);
void ContactFrontPage(ContactInteractionContext* context, int showbusy, int hideHeadShot);
void ContactInteractionChooseContact(ContactInteractionContext* context);
void ContactInteractionAbout(ContactInteractionContext* context);
void ContactInteractionAutoComplete(ContactInteractionContext* context);
void ContactInteractionAbandonTask(ContactInteractionContext *context);
void ContactInteractionDismiss(ContactInteractionContext* context);
void ContactInteractionDisplayWebStore(ContactInteractionContext* context);
#endif //__CONTACTDIALOG_H
