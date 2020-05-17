#include "storyarc/missionClient.h"
#include <utilitieslib/network/netio.h>
#include <utilitieslib/utils/timing.h>
#include "UI/sprite/sprite_text.h"
#include "UI/uiAutomap.h"
#include "cmdparse/cmdgame.h"
#include "bases/bases.h"
#include <utilitieslib/components/Earray.h>
#include <utilitieslib/language/MessageStoreUtil.h>
#include <utilitieslib/utils/utils.h>

// *********************************************************************************
//  Current mission and task status
// *********************************************************************************
char* MissionAddTimer(char* str, TaskStatus* task, int testwidth)
{
    static char buf[1000];
    int remaining;

    if ((task->isComplete && !task->enforceTimeLimit) || !task->timeout)
        return str;

    remaining = (int)task->timezero - (int)timerSecondsSince2000WithDelta();

    if (task->timerType == 1)
    {
        remaining = -remaining;
    }

    if (remaining < 0)
        return str;

    strncpy(buf, str, 1000);
    strncat(buf, " - ", 1000 - strlen(buf));
    if (testwidth)
    {
        strncat(buf, "000:00", 1000 - strlen(buf));
    }
    else
    {
        char remainstr[100];
        if (remaining < 0) remaining = 0;
        timerMakeOffsetStringFromSeconds(remainstr, remaining);
        strncat(buf, remainstr, 1000 - strlen(buf));
    }
    buf[999] = 0;
    return buf;
}


// get a descriptive second line - either location or status
// showState is turned off for the mission window but not for the compass
char* MissionSecondLine(TaskStatus* task, int showState)
{
    if (task->isComplete)
        return task->state; // should be Return to Contact
    else if (game_state.mission_map && !eaSize(&g_base.rooms) && TaskStatusIsTeamTask(task))
        return showState?task->state:textStd("ActiveTaskString");
    else if (task->zoneTransfer)
        return textStd("GotoZoneTransfer");
    else if (task->location.mapName && task->location.mapName[0] 
                && (task->location.contactType != CONTACTDEST_EXPLICITTASK || !strstri(game_state.world_name, task->location.mapName)))
        return textStd(task->location.mapName);
    else
        return task->state;
}


// *********************************************************************************
//  Mission objective objects 
// *********************************************************************************

static char* objectiveInteractString = NULL;
static int timerRunning;
static __int64 objectiveTimerReceiveTime;    // When did the player get the last objective timer update?
static float objectiveTimerExpireTime;        // When does the objective timer expire?

void MissionReceiveObjectiveTimer(Packet* pak)
{
    char* str;

    if(objectiveInteractString)
    {
        free(objectiveInteractString);
        objectiveInteractString = NULL;
    }
    
    str = pktGetString(pak);
    objectiveTimerExpireTime = pktGetF32(pak);        // How long does the timer expire from now?
    objectiveTimerReceiveTime = timerCpuTicks64();    // Recrod when the timer was received.

    if(objectiveTimerExpireTime)
    {
        objectiveInteractString = strdup(str);
        timerRunning = 1;
    }
    else
    {
        timerRunning = 0;
    }
}


static int MissionObjectiveTimerIsRunning()
{
    // Timer is should be considered stopped if:
    //        1) it is not running or 
    //        2) the server has set the timer to 0.0.
    if(!timerRunning || objectiveTimerExpireTime == 0.0)
        return 0;
    else
        return 1;
}

void MissionObjectiveStopTimer()
{
    timerRunning = 0;
    objectiveTimerExpireTime = 0.0;
}

/* Function MissionGetObjectiveTimer()
 *    Retrieves timer info associated with the objective that the player is
 *    currently interacting with.
 *
 *    If the player is currently interacting with a mission objective, the function
 *    return 1 and fills in the timer values.
 *
 *    If the player is not interacting with a mission objective, the function returns
 *    0 and leave the timer values untouched.
 *
 *    Returns:
 *        0 - Timer not retrieved. (Timer not running)
 *        1 - Timer retrieved.
 */
int MissionGetObjectiveTimer(float* timeRemainingOut, float* totalTimeOut)
{
    float timeElapsed;
    float totalTime;

    if(!MissionObjectiveTimerIsRunning())
        return 0;

    totalTime = objectiveTimerExpireTime;
    timeElapsed = timerSeconds64(timerCpuTicks64() - objectiveTimerReceiveTime);

    // Has the timer expired?
    // If so, stop the timer.
    if(timeElapsed >= totalTime)
    {
        timeElapsed = totalTime;
        timerRunning = 0;
    }

    // Return timer values if requested.
    if(timeRemainingOut)
    {
        *timeRemainingOut = totalTime - timeElapsed;
    }
    if(totalTimeOut)
    {
        *totalTimeOut = totalTime;
    }
    return 1;
}

char* MissionGetObjectiveInteractString()
{

    if(!MissionObjectiveTimerIsRunning())
        return NULL;
    else
        return objectiveInteractString;
}