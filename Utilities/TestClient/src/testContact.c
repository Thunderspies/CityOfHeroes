#include "testContact.h"
#include "storyarc/contactCommon.h"
#include "storyarc/contactClient.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <utilitieslib/utils/utils.h>
#include "testClientInclude.h"
#include "testUtil.h"
#include <utilitieslib/components/earray.h>
#include "gameData/randomCharCreate.h"
#include "UI/uiCompass.h"
#include <utilitieslib/assert/assert.h>
#include "clientcomm/clientcomm.h"

#define commAddInput(s) printf("commAddInput(\"%s\");\n", s); commAddInput(s);


static eaiHandle options_data = NULL;
static eaiHandle *options = &options_data;
extern int promptRanged(char *str, int max);

StoryarcFlashbackHeader **storyarcFlashbackList = NULL;
int flashbackListRequested = false;

void uiContactDialogAddTaskDetail(char *pID, char *pText, char *pAccept)
{
    return;
}

void uiContactDialogSetChallengeTimeLimits(U32 bronze, U32 silver, U32 gold)
{
    return;
}

void cd_SetBasic( CDtype type, char *body, ContactResponseOption ** rep, void ( *fp)( int, void* ), void* voidptr ) {
    int i;
    char c;
    int opt;

    if (options_data==NULL) {
        eaiCreate(options);
    } else {
        eaiSetSize(options, 0);
    }
    
    printf("Received contact dialog\n");
    { // Print the body, sans HTML, find links
        char *c;
        bool inside;
        for (c=body, inside=false; *c; c++) {
            if (*c=='>')
                inside=false;
            else if (*c=='<') {
                if (strnicmp(c, "<A HREF=", 8)==0) {
                    char temp[256] = {0};
                    char *c2, c3;
                    int value;
                    strncpy(temp, c+8, 255);
                    for (c2=temp; *c2; *c2++) {
                        if (*c2=='>' || *c2==' ') {
                            *c2 = 0;
                            break;
                        }
                    }
                    value = StaticDefineIntGetInt(ParseContactLink, temp);
                    if (value < 0) value = atoi(temp);
                    c3 = '1' + eaiPush(options, value);
                    if (ask_user)
                        printf("  [%c] ", c3);
                }
                inside=true;
            } else if (!inside) {
                if (ask_user)
                    printf("%c", *c);
            } else if (strnicmp(c, "BR>", 3)==0 || strnicmp(c, "TR>", 3)==0) {
                if (ask_user)
                    printf("\n");
            } else if (strnicmp(c, "TD", 2)==0) {
                if (ask_user)
                    printf(" ");
            } else if (strnicmp(c, "LINK=", 5)==0) {
                char temp[256] = {0};
                char *c2, c3;
                int value;
                strncpy(temp, c+5, 255);
                for (c2=temp; *c2; *c2++) {
                    if (*c2=='>' || *c2==' ') {
                        *c2 = 0;
                        break;
                    }
                }
                value = StaticDefineIntGetInt(ParseContactLink, temp);
                if (value < 0) value = atoi(temp);
                c3 = '1' + eaiPush(options, value);
                if (ask_user)
                    printf("  [%c] <IMAGE> ", c3);
            }
        }
    }
    if (ask_user)
        printf("\n");

    for (i=0; i<eaSize(&rep); i++){
        if (rep[i]->responsetext[0]) {
            c = '1' + eaiPush(options, rep[i]->link);
            if (ask_user) {
                printf("  [%c] %s\n", c, rep[i]->responsetext);
            }
        } else if (ask_user) {
            printf("\n");
        }
    }
    if (eaiSize(options)) {
        int choice;

        if (ask_user) {
            choice = promptRanged("Choose an option", eaiSize(options));
        } else {
            choice = randInt2(eaiSize(options));
        }
        opt = options_data[choice];
        fp(opt, voidptr);
    } else {
        testClientLog(TCLOG_SUSPICIOUS, "No non-null options from a contact!");
    }
}

void cd_clear() {;}

void printContactInfo(void)
{
    ContactStatus * cs;
    TaskStatus * ts;
    int i;
    
    printf("Contacts:\n");
    for (i=0; i<eaSize(&contacts); i++) {
        cs = contacts[i];
        if (!cs || !cs->name || !cs->name[0]) continue;
        printf("\t%d Name: %s\n", i, cs->name);
        ts = TaskStatusFromContact(cs);
        if (ts) {
            printf("\t\tHasTask\n");
            if (ts->description && ts->description[0])
                printf("\t\t\tDescription: %s\n", ts->description);
            if (ts->isComplete)
                printf("\t\t\t* completed\n");
            if (ts->isMission)
                printf("\t\t\t* mission\n");
            printf("\t\tTask location:\n");
            if (ts->hasLocation) {
                printf("\t\t\t(%f, %f, %f)\n", ts->location.world_location[3][0], ts->location.world_location[3][1], ts->location.world_location[3][2]);
            }
        }
        if (cs->notifyPlayer)
            printf("\t\tnotifyPlayer\n");

        printf("\t\tContact location:\n");
        if (cs->hasLocation) {
            printf("\t\t\t(%f, %f, %f)\n", cs->location.world_location[3][0], cs->location.world_location[3][1], cs->location.world_location[3][2]);
        } else {
            printf("\t\t\tNONE\n");
        }
    }
}

ContactStatus* getContactWithAMission(void)
{
    ContactStatus * cs;
    int i;
    for (i=0; i<eaSize(&contacts); i++) {
        cs = contacts[i];
        if (!cs || !cs->name || !cs->name[0]) continue;
        if (TaskStatusFromContact(cs))
            return cs;
    }
    return NULL;
}

void completeCurrentTask(void)
{
    ContactStatus * cs = getContactWithAMission();
    char buf[1024];
    if (!cs) {
        sprintf(buf, "completetask 0");
    } else {
        TaskStatus** tasks = TaskStatusGetPlayerTasks();
        int index = eaFind(&tasks, TaskStatusFromContact(cs));
        sprintf(buf, "completetask %d", index==-1?0:index); 
    }
    commAddInput(buf);
}

TaskType getCurrentTaskType(void)
{
    ContactStatus * cs = getContactWithAMission();
    TaskStatus * ts = TaskStatusFromContact(cs);
    if (!cs || !ts)
        return TASK_NOTYPE;
    if (ts->isComplete)
        return TASK_COMPLETED;
    if (ts->isMission)
        return TASK_MISSION;
    if (strstri(ts->description, "warehouse") || strstri(ts->description, "office") || strstri(ts->description, "sewers") || strstri(ts->description, "rescue")) {
        return TASK_MISSION;
    }
    if (strstri(ts->description, "Visit")!=0) {
        return TASK_VISITLOCATION;
    }
    if (strstri(ts->description, "deliver")!=0 || strstri(ts->description, "pick up")!=0 || strstri(ts->description, "warn")!=0) {
        return TASK_DELIVERITEM;
    }
    if (strstri(ts->description, "Defeat")!=0) {
        return TASK_KILLX;
    }
    assert(!"Don't know what kind of task this is!");
    return TASK_NOTYPE;
}


Destination *findTaskDest(void)
{
    Destination *currentDest=NULL;
    ContactStatus * cs = getContactWithAMission();
    TaskStatus * ts = TaskStatusFromContact(cs);

    if (!cs || !ts)
        return NULL;
    if (ts->isComplete || cs->notifyPlayer) {
        // We want to go back to the contact!
        if (!cs->hasLocation)
            return NULL;
        return &cs->location;
    } else {
        // We have a task to do!
        if (ts->hasLocation) {
            return &ts->location;
        }
        // Our task has no destination!
    }
    return NULL;
}

bool isDestOnThisMap(Destination *dest)
{
    if (!dest)
        return false;
    if (stricmp(dest->mapName, glob_map_name)==0) {
        return true;
    }
    return false;
}

bool isTaskDestOnThisMap(void)
{
    Destination *dest = findTaskDest();
    return isDestOnThisMap(dest);
}

Destination *findContactLocation(void)
{
    ContactStatus * cs;
    int i;
    for (i=0; i<eaSize(&contacts); i++) {
        cs = contacts[i];
        if (!cs || !cs->name || !cs->name[0] || !cs->hasLocation) continue;
        return &cs->location;
    }
    return NULL;
}
