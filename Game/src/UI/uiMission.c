/***************************************************************************
 *     Copyright (c) 2003-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include <utilitieslib/components/earray.h>
#include "player/player.h"
#include "entity/entPlayer.h"
#include "storyarc/missionClient.h"
#include "language/langClientUtil.h"
#include "storyarc/storyarcCommon.h"
#include <utilitieslib/utils/utils.h>

#include "UI/uiUtil.h"
#include "UI/uiChat.h"
#include "UI/uiGame.h"
#include "UI/uiInput.h"
#include "UI/uiWindows.h"
#include "UI/uiToolTip.h"
#include "UI/uiUtilGame.h"
#include "UI/uiAutomap.h"
#include "UI/uiMissionSummary.h"
#include "UI/uiContactDialog.h"
#include "UI/uiScript.h"

#include "UI/sprite/sprite_base.h"
#include "UI/sprite/sprite_font.h"
#include "UI/sprite/sprite_text.h"
#include "graphics/ttFontUtil.h"
#include "graphics/textureatlas.h"

#include "formatter/smf_main.h"

#include "UI/uiScrollBar.h"

#include "UI/uiClipper.h"

#include "cmdparse/cmdgame.h"
#include "UI/uiContextMenu.h"
#include "UI/uiDialog.h"
#include "entity/entity.h"
#include "UI/uiSGRaidList.h"
#include "entity/teamCommon.h"
#include "UI/uiTabControl.h"

#include "clientcomm/clientcomm.h"
#include <utilitieslib/utils/timing.h>

#include "entity/Supergroup.h"
#include <utilitieslib/components/EString.h>
#include "bases/bases.h"
#include "edit/ClickToSource.h"
#include <utilitieslib/language/MessageStoreUtil.h>
#include "entity/teamup.h"

ContextMenu* taskContextMenu;

#define MISSION_PICKER_DEFAULT_SCALE 0.7
#define MISSION_PICKER_MOUSEOVER_SCALE 0.8

static TextAttribs s_taDefaults =
{
    /* piBold        */  (int *)0,
    /* piItalic      */  (int *)0,
    /* piColor       */  (int *)0x00deffff,
    /* piColor2      */  (int *)0,
    /* piColorHover  */  (int *)0x00deffff,
    /* piColorSelect */  (int *)0,
    /* piColorSelectBG*/ (int *)0x333333ff,
    /* piScale       */  (int *)(int)(1.0f*SMF_FONT_SCALE),
    /* piFace        */  (int *)&smfSmall,
    /* piFont        */  (int *)0,
    /* piAnchor      */  (int *)0,
    /* piLink        */  (int *)0x80e080ff,
    /* piLinkBG      */  (int *)0,
    /* piLinkHover   */  (int *)0x66ff66ff,
    /* piLinkHoverBG */  (int *)0,
    /* piLinkSelect  */  (int *)0,
    /* piLinkSelectBG*/  (int *)0x666666ff,
    /* piOutline     */  (int *)1,
    /* piShadow      */  (int *)0,
};

static TextAttribs s_taDefaultsCompleted =
{
    /* piBold        */  (int *)0,
    /* piItalic      */  (int *)0,
    /* piColor       */  (int *)0x00ff44ff,
    /* piColor2      */  (int *)0,
    /* piColorHover  */  (int *)0x00ff44ff,
    /* piColorSelect */  (int *)0,
    /* piColorSelectBG*/ (int *)0x333333ff,
    /* piScale       */  (int *)(int)(1.0f*SMF_FONT_SCALE),
    /* piFace        */  (int *)&smfSmall,
    /* piFont        */  (int *)0,
    /* piAnchor      */  (int *)0,
    /* piLink        */  (int *)0x80e080ff,
    /* piLinkBG      */  (int *)0,
    /* piLinkHover   */  (int *)0x66ff66ff,
    /* piLinkHoverBG */  (int *)0,
    /* piLinkSelect  */  (int *)0,
    /* piLinkSelectBG*/  (int *)0x666666ff,
    /* piOutline     */  (int *)1,
    /* piShadow      */  (int *)0,
};

#define MAX_TASK_TOOL_TIPS 50
static ToolTip compassMissionTip = {0};

static int s_bReparse = 0;

void missionReparse(void)
{
    s_bReparse = true;
    missionSummaryReparse();
}

#define BORDER_SPACE    15
#define TAB_HEIGHT      18

#define TAB_INSET       20 // amount to push tab in from edge of window
#define BACK_TAB_OFFSET 3  // offset to kee back tab on the bar
#define OVERLAP         5  // amount to overlap tabs

#define TEAMTASK_BUTTON_PANE_HEIGHT 30
int g_drawsgraidtab = 0;

//---------------------------------------------
// Tooltip management
//    Avoids having to deal with allocations while drawing
//    tasksets.
//---------------------------------------------
MP_DEFINE(ToolTip);
static ToolTip* missionToolTipCreate()
{
    MP_CREATE(ToolTip, MAX_TASK_TOOL_TIPS);
    return MP_ALLOC(ToolTip);
}

static missionToolTipCleanup()
{
    mpFreeAll(MP_NAME(ToolTip));
}

int uiMission_isMixedTeam()
{
    Entity* player = playerPtr();

    if (!player->teamup)
        return false;

    return isTeamupAlignmentMixedClientside(player->teamup);
}

//----------------------------------------------------------------------------------
// Compass task info display
//----------------------------------------------------------------------------------

void mission_displayCompass( float cx, float cy, float z, float wd, float ht, float scale, TaskStatus* selectedTask, int active, char ** estr, F32 *info_wd )
{
    char *task = 0;
    char *status = 0;
    int remaining;

    if (!selectedTask) 
        return;
    if (!scriptUIIsDetached && eaSize(&scriptUIWidgetList)) 
        return;    //if there is a zone event, we should draw this stuff
    if (!selectedTask)
        return;

    if (game_state.mission_map && !eaSize(&g_base.rooms) && selectedTask->isComplete && selectedTask->isMission)
    {
        task = textStd("MissionCompleted");
        if (!selectedTask->teleportOnComplete)
            status = textStd("MissionFindExit");
    }
    else if (selectedTask->isComplete)
    {
        task = selectedTask->state;    
    }
    else // otherwise, the same view you would have in the mission window
    {
        task = selectedTask->description;
        status = MissionSecondLine(selectedTask, 1);
    }
    
    remaining = (int)selectedTask->timezero - (int)timerSecondsSince2000WithDelta();

    if (selectedTask->timerType == 1)
    {
        remaining = -remaining;
    }

    // add the timer if we need it
    if (task && selectedTask->timeout 
        && (!selectedTask->isComplete || selectedTask->enforceTimeLimit) 
        && remaining >= 0 && (int)timerSecondsSince2000WithDelta() <= selectedTask->timeout)
    {
        char remainstr[100];
        
        estrConcatf(estr, "<table border=0><tr border=0><td border=0 align=center>%s", task );
        timerMakeOffsetStringFromSeconds(remainstr, remaining);
          estrConcatf(estr, "</td><td width=20 border=0 align=center valign=center>%s</td></tr></table>", remainstr );
    }
    else if(task)
    {
         estrConcatf(estr, "<span align=center>%s</span>", task );
    }

       if( status )
           estrConcatf(estr, "<scale .8><span align=center>%s</span></scale>", status );

     //--------------------------
    {
        CBox box;
         float fScale = MISSION_PICKER_DEFAULT_SCALE*scale;
        AtlasTex *ptex = atlasLoadTexture("map_enticon_mission_b.tga");

         if(task && selectedTask && selectedTask->ownerDbid && selectedTask->context )//&& selectedTask->subhandle)
        {
               F32 text_wd = wd - 2*ptex->width*fScale; //  MIN( str_wd(font_grp, scale, scale, task), wd - ptex->width*fScale );

            BuildCBox(&box,cx - text_wd/2 -(ptex->width*fScale), cy-(ptex->height*fScale)/2,
                        ptex->width*fScale, ptex->height*fScale);

            *info_wd = ptex->width*fScale;

            setToolTip( &compassMissionTip, &box, "missionInfoTip", 0, MENU_GAME, WDW_COMPASS );
            addToolTip( &compassMissionTip );

               if(mouseCollision(&box))
            {
                 fScale = MISSION_PICKER_MOUSEOVER_SCALE*scale;
            }
            if(mouseClickHit(&box, MS_LEFT))
            {
                missionSummaryShow(selectedTask->ownerDbid, selectedTask->context, selectedTask->subhandle);
            }

            fScale*=1.25;
            display_rotated_sprite(ptex, cx - text_wd/2 -(ptex->width*fScale)/2 - 8*scale, cy-(ptex->height*fScale)/2, z,
                fScale, fScale, active? CLR_RED: CLR_MISSIONYELLOW,    PI/4, 0);
            fScale/=1.25;
            ptex = atlasLoadTexture("MissionPicker_icon_info_overlay.tga");
             display_sprite(ptex, cx - text_wd/2 -(ptex->width*fScale)/2 - 8*scale, cy-(ptex->height*fScale)/2, z, fScale, fScale, CLR_WHITE);
        }
        else
            clearToolTip( &compassMissionTip );
    }
}



//----------------------------------------------------------------------------------
// Task entry display
//----------------------------------------------------------------------------------
typedef enum
{
    DTS_NONE,
    DTS_TASK_CLICKED,
    DTS_TASK_RIGHT_CLICKED,
    DTS_INFO_CLICKED,
} DTSResult;

// Helper function to get the appropriate width
static int getWidth(char* mission, char* state)
{
    int width = 100;
    if (state && state[0])
    {
        int lenMish = strlen(mission);
        int lenState = strlen(state);
        width = (lenMish*100)/(lenMish+lenState);
    }
    return width;
}

// Construct a formatted mission window
static char* ConstructFormattedMission(char* mission, char* state, char* intro, int level, int alliance, int inactive, float scale)
{
    static char* formatted = 0;
    int width = getWidth(mission, state);
    float iconSize = 20.0f; 
    
    estrSetLength(&formatted, 2000);

    estrPrintf(&formatted, "<table><tr border=0><td border=0 align=left width=%i>", width);

    if (inactive)
        estrConcatStaticCharArray(&formatted, "<font color=gray>"); 
    else
        estrConcatStaticCharArray(&formatted, "<font color=wheat>");

    estrConcatf(&formatted, "(%i) %s", level, mission);

    
    if (alliance != SA_ALLIANCE_BOTH && alliance <= SA_ALLIANCE_VILLAIN && alliance >=  SA_ALLIANCE_HERO)
    {
        estrConcatStaticCharArray(&formatted, "</td><td border=0 align=left>");
        if (alliance == SA_ALLIANCE_HERO)
            estrConcatf(&formatted, "<font color=paragon>%s</font></td><td>", textStd("HeroOnlyString"));
//            estrConcatf(&formatted, "<img src=missionfaction_icon_hero.tga align=left width=%d height=%d>", (int) (iconSize * scale * 0.9f), (int) (iconSize * scale * 0.9f)); 
        else if (alliance == SA_ALLIANCE_VILLAIN)
//            estrConcatf(&formatted, "<img src=missionfaction_icon_villain.tga align=left width=%d height=%d>", (int) (iconSize * scale * 0.9f), (int) (iconSize * scale * 0.9f));
            estrConcatf(&formatted, "<font color=red>%s</font></td><td>", textStd("VillainOnlyString"));

    }
    

    if (state && state[0])
    {
        estrConcatf(&formatted, "</td><td border=0 align=right>%s", state);
    }
    if (intro)
    {
        estrConcatf(&formatted, "</td></tr></table></font><font face=small color=LightGrey scale=0.8><nolink>%s</font></nolink>", intro);
    }
    return formatted;
}

// Returns height of the task element.
// Alter bounds.
static DTSResult displayTaskStatus(TaskStatus* task, UIBox* bounds, float z, int live, int selected, int active, 
                                   int starred, int isMixedTeam, ToolTip* tooltip, float scale)
{
//    int blockDueToAlliance = (task->alliance != SA_ALLIANCE_BOTH && isMixedTeam);
    int blockDueToAlliance = false;
    DTSResult result = DTS_NONE;
    assert(task);

    if (!task->pSMFBlock)
    {
        task->pSMFBlock = smfBlock_Create();
    }

    // If the task status appears to be empty, do nothing.
    if(!task->description[0])
        return result;

    // If the task status cannot be seen, do nothing.
    //{
    //    UIBox testBounds = *bounds;
    //    testBounds.height = 100;
    //    if(!clipperIntersects(&testBounds))
    //        return result;
    //}

    {
        int textoffset = 0;
        int ht;
        TextAttribs *pta = task->isComplete ? &s_taDefaultsCompleted : &s_taDefaults;
        CBox box;
        int foreColor = CLR_NORMAL_FOREGROUND;
        int backColor = CLR_NORMAL_BACKGROUND;
        char* formatted = 0;
        char* state = 0;
        char* missionstr = 0;
        char* nextline = 0;

        if (blockDueToAlliance)
        {
            foreColor = CLR_DISABLED_FOREGROUND;
            backColor = CLR_DISABLED_BACKGROUND;
        }

        // Get the mission details
        if (selected)
        {
            if(task->detailInvalid)
            {
                if (task->iAskedWhen == 0 
                    || (timerSecondsSince2000() - task->iAskedWhen > 5))
                {
                    START_INPUT_PACKET(pak, CLIENTINP_REQUEST_TASK_DETAIL);
                    pktSendBitsPack(pak, 1, task->ownerDbid);
                    pktSendBitsPack(pak, 1, task->context);
                    pktSendBitsPack(pak, 1, task->subhandle);
                    END_INPUT_PACKET
                    task->iAskedWhen = timerSecondsSince2000();
                }
            }
            else
            {
                // Need to consider the current clipper as well
                int yscissor = bounds->y;
                int htscissor = 48*scale;
                UIBox *curr = clipperGetBox(clipperGetCurrent());
                if (curr && yscissor < curr->y)
                {
                    htscissor -= (curr->y - yscissor);
                    htscissor = max(htscissor, 0);
                    yscissor = curr->y;
                }
                if (curr && (yscissor + htscissor) > (curr->y + curr->height))
                {
                    htscissor -= ((yscissor + htscissor) - (curr->y + curr->height));
                    htscissor = max(htscissor, 0);
                }
                nextline = task->intro;
                set_scissor(1);
                scissor_dims(bounds->x, yscissor, bounds->width, htscissor);
            }
        }
        missionstr = MissionAddTimer(task->description, task, 0);
        state = MissionSecondLine(task, 0);
        formatted = ConstructFormattedMission(missionstr, state, nextline, task->level, task->alliance, blockDueToAlliance, scale);
         
        pta->piScale = (int*)((int)(scale*SMF_FONT_SCALE));

        ht = smf_ParseAndDisplay(task->pSMFBlock,    formatted,
            bounds->x+15*scale, bounds->y + 5*scale, z+1,
            bounds->width-40*scale, (nextline ? 40*scale : 20*scale),
            s_bReparse, false, pta, NULL, 
            0, true); // needs to reparse on clock ticks and whenever string changes

        ht = MIN(ht+10*scale, 53*scale);

        BuildCBox(&box, bounds->x-3*scale, bounds->y, bounds->width-20*scale, ht);

        // Set mouse over color.
        if(!blockDueToAlliance && live && mouseCollision(&box))
        {
            foreColor = CLR_MOUSEOVER_FOREGROUND;
            backColor = CLR_MOUSEOVER_BACKGROUND;
            drawFlatFrame(PIX2, R10, bounds->x+5*scale, bounds->y, z, bounds->width-20*scale, ht, scale, foreColor, backColor);
        }

        if(!blockDueToAlliance && mouseClickHit(&box, MS_LEFT))
        {
            result = DTS_TASK_CLICKED;
        }
        else if(!blockDueToAlliance && mouseClickHit(&box, MS_RIGHT))
        {
            result = DTS_TASK_RIGHT_CLICKED;
        }

         if (nextline)
        {
            set_scissor(0);
              if (D_MOUSEHIT == drawStdButton(bounds->x + bounds->width - 38*scale, bounds->y + ht - 10*scale, z, 42*scale, 18*scale, window_GetColor(WDW_MISSION), "MissionMoreButton", 1.0, 0))
                missionSummaryShow(task->ownerDbid, task->context, task->subhandle);
        }

        if (selected)
        {
            foreColor = CLR_SELECTION_FOREGROUND;
            backColor = CLR_SELECTION_BACKGROUND;
        }
        if (CTS_SHOW_STORY)
            ClickToSourceDisplay(bounds->x+15*scale, bounds->y+ht+5*scale, z+1, 0, 0xffffffff, task->filename, NULL, CTS_TEXT_REGULAR);

        drawFlatFrame(PIX2, R10, bounds->x+5*scale, bounds->y, z, bounds->width-20*scale, ht, scale, foreColor, backColor);
        bounds->height = ht;

        return result;
    }
    return 0;
}

static int entHasTask(Entity *e)
{
    int listSize;
    TaskStatusSet* set;

    // Does a task set exist for the entity at all?
    set = TaskStatusSetGet(e->db_id);
    if(!set)
        return 0;

    // Does the task set contain valid entries?
    //    TaskStatusSets should always have at least one valid entry, because
    //    empty TaskSets should be deleted.
    listSize = eaSize(&set->list);
    if(listSize == 0)
        return 0;
    if(listSize == 1 && !set->list[0]->description[0])    // empty entry?
        return 0;
    if(listSize == 1 && set->list[0]->description[0] && e->teamup && e->teamup->members.count == 1)    // filled entry that shouldn't be displayed?
        return 0;

    return 1;
}

static int playerHasTask(void)
{
    return entHasTask(playerPtr());
}

//-----------------------------------------------------------------------------------
// Team task tab
//-----------------------------------------------------------------------------------

int TaskStatusSetGetTaskCount(TaskStatusSet* set)
{
    if(!set)
        return 0;

    return MAX(0, eaSize(&set->list) - 1);
}


// return status pointed at by selectedTeamTask
TaskStatus* GetUISelectedTeamTask()
{
    int i, n;
    TaskStatusSet *set;
    Entity* player = playerPtr();

    if (waypointDest.type != ICON_MISSION) return NULL;
    set = TaskStatusSetGet(waypointDest.dbid);

    if (player->supergroup && player->supergroup->sgMission)
    {
        TaskStatus* sgMission = player->supergroup->sgMission;
        if (sgMission && sgMission->ownerDbid == waypointDest.dbid && sgMission->context == waypointDest.context && sgMission->subhandle == waypointDest.subhandle)
            return sgMission;
    }

    if (!set) return NULL;
    n = eaSize(&set->list);
    for (i = 0; i < n; i++)
    {
        if (set->list[i]->ownerDbid == waypointDest.dbid &&
            set->list[i]->context == waypointDest.context &&
            set->list[i]->subhandle == waypointDest.subhandle)
            return set->list[i];
    }

    return NULL;
}

static Destination lastSelectedTask = {0};
static int deselected = 0;
static ToolTip s_teamTaskSetDisplay_nameTip;

// Returns the height of the drawn item.
#define FADE_HEIGHT 20.f
#define FADE_WIDTH 150.f
static int teamTaskSetDisplay(TaskStatusSet* set, int dbid, char *teammateName, UIBox bounds, float z, int firstentry, int onteam, 
                              int isSGMission, int active, float sc)
{
    Entity* player = playerPtr();
    Entity *teammate;
    UIBox taskItemBounds = bounds;
    int isMixedTeam = uiMission_isMixedTeam();
    CBox box;

    if (active)
    {
        BuildCBox(&box, bounds.x, bounds.y, bounds.width, (FADE_HEIGHT + 2)*sc);
        addToolTip(&s_teamTaskSetDisplay_nameTip);
        setToolTip(&s_teamTaskSetDisplay_nameTip, &box, textStd("ThisIsActivePlayer"), NULL, MENU_GAME, WDW_MISSION);
    }

    teammate = entFromDbId(dbid);

    // two pixels lead-in
    uiBoxAlter(&taskItemBounds, UIBAT_SHRINK, UIBAD_TOP, (2)*sc);

    // Draw a title bar with a title.
    {
        AtlasTex* barRight = atlasLoadTexture("Contact_fadebar_R.tga");
        AtlasTex* barMid = atlasLoadTexture("Contact_fadebar_MID.tga");
        AtlasTex* barLeft = atlasLoadTexture("Contact_fadebar_L.tga");
        float xscale, yscale;
        char* comment = 0;
        int w;
        int color = active ? 0xffff0055 : 0x00000055;

        xscale = sc;
        yscale = FADE_HEIGHT*sc/barLeft->height;
        display_sprite(barLeft, taskItemBounds.x, taskItemBounds.y, z, xscale, yscale, color);

        xscale = (taskItemBounds.width - (FADE_WIDTH + barLeft->width)*sc)/barMid->width;
        yscale = FADE_HEIGHT*sc/barMid->height;
        display_sprite(barMid, taskItemBounds.x + barLeft->width*sc, taskItemBounds.y, z, xscale, yscale, color);

        xscale = FADE_WIDTH*sc/barRight->width;
        yscale = FADE_HEIGHT*sc/barRight->height;
        display_sprite(barRight, taskItemBounds.x + taskItemBounds.width - FADE_WIDTH*sc, taskItemBounds.y, z, xscale, yscale, color);

         font(&game_14);
        font_color(0x00deffff, 0x00deffff);

        prnt(taskItemBounds.x+10*sc, taskItemBounds.y+16*sc, z, .90f*sc, .90f*sc, "TeamMemberName", teammateName);

        font(&game_12);
        font_color(0xccccccff, 0xccccccff);
        if (isSGMission)
            comment = "SGMissionTitle";
        else if (!onteam)
            comment = "MissionNotOnTeam";
        else if(!teammate)
            comment = "MissionNotOnMap";
        else if(teammate && !entHasTask(teammate))
            comment = "MissionNoMissions";
        else
            comment = difficultyString(TaskStatusGetNotoriety(dbid));

        w = str_wd(&game_12, 1.0f*sc, 1.0f*sc, comment);
        prnt(taskItemBounds.x+taskItemBounds.width-w-barLeft->width*sc-20*sc, taskItemBounds.y+16*sc, z, 1.0f*sc, 1.0f*sc, comment);

        uiBoxAlter(&taskItemBounds, UIBAT_SHRINK, UIBAD_TOP, (FADE_HEIGHT+5)*sc);
    }

    // Draw all of the tasks in the set.
    if(set && (TaskStatusSetGetTaskCount(set)>0 || isSGMission))
    {
        int i;
        taskItemBounds.width -= 15*sc;
        for(i = 0; i < eaSize(&set->list); i++)
        {
            DTSResult result;
            int active = 0;
            int selected = 0;
            int teamTask = 0;
            TaskStatus* task = set->list[i];
            ToolTip* tooltip = NULL;

            if (i == 0 && !firstentry) 
                continue;
            if (i > 0 && firstentry) 
                continue;

            taskItemBounds.height = 0;
            if (activeTaskDest.type == ICON_MISSION &&
                activeTaskDest.dbid == task->ownerDbid &&
                activeTaskDest.context == task->context &&
                activeTaskDest.subhandle == task->subhandle)
                active = 1;
            if (waypointDest.type == ICON_MISSION &&
                waypointDest.dbid == task->ownerDbid &&
                waypointDest.context == task->context &&
                waypointDest.subhandle == task->subhandle)
                selected = 1;

            // If it's the team task, go get the info from the player's
            // slot 0 since it is (supposed to be) the most up to date.
            if (TaskStatusIsTeamTask(task))
            {
                task = PlayerGetActiveTask();
                teamTask = 1;
            }

            if(teammate || teamTask)
            {
                tooltip = missionToolTipCreate();

                result = displayTaskStatus(task, &taskItemBounds, z, 1, selected, active, selected, isMixedTeam, tooltip, sc);
                if(result == DTS_INFO_CLICKED)
                {
                    missionSummaryShow(task->ownerDbid, task->context, task->subhandle);
                }
                if(result == DTS_TASK_CLICKED)
                {
                    compass_SetTaskDestination(&waypointDest, task);
                }
                taskItemBounds.y += taskItemBounds.height + 5*sc;
            }
        }

    }
    return taskItemBounds.y - bounds.y;
}

static void sendTeamTaskSelect(void * data)
{
    char buf[128];

    TaskStatus* selectedTeamTask = GetUISelectedTeamTask();
    if (!selectedTeamTask) return;

    sprintf( buf, "teamtask %i %i %i", selectedTeamTask->ownerDbid, selectedTeamTask->context, selectedTeamTask->subhandle);
    cmdParse( buf );
}

static void TeamTaskSelectClient()
{
    TaskStatus* activetask = PlayerGetActiveTask();
    TaskStatus* selectedTeamTask = GetUISelectedTeamTask();

    if (!selectedTeamTask) return;

    // ignore selection if the selected task is already correct
    if (activetask && TaskStatusIsSame(activetask, selectedTeamTask)) return;

    // don't allow selection if we are on a mission map
    if(game_state.mission_map && !isBaseMap())
    {
        InfoBox(INFO_USER_ERROR, "TeamTaskAbandonOnMissionMap");
        return;
    }

    // Ask the user for confirmation if a mission is already running for the current task.
    if(activetask && activetask->hasRunningMission)
    {
        dialogStd(DIALOG_YES_NO, "TaskAbandonMissionPrompt", NULL, NULL, sendTeamTaskSelect, NULL,1);
        return;
    }

    sendTeamTaskSelect(0);
}

static void TeamTaskAbandonUISelection(void *useless)
{
    TaskStatus *selectedTask = GetUISelectedTeamTask();
    TaskStatus *activeTask = PlayerGetActiveTask();
    char buf[128];

    if (!selectedTask) 
        return;

    if (activeTask == selectedTask && game_state.mission_map)
    {
        InfoBox(INFO_USER_ERROR, "MissionAbandon_UnableMissionMap");
        return;
    }

    if (selectedTask->ownerDbid != playerPtr()->db_id)
    {
        InfoBox(INFO_USER_ERROR, "MissionAbandon_Unable");
        return;
    }

    sprintf(buf, "abandontask %i %i", selectedTask->context, selectedTask->subhandle);
    cmdParse(buf);
}

static void teamTaskButtonPane(UIBox box, float z, float sc)
{
    TaskStatus *selectedTask = GetUISelectedTeamTask();

    clipperPushRestrict(&box);
    if (D_MOUSEHIT == drawStdButton(box.x + box.width/2 - 60*sc, box.y + box.height/2, z, 100*sc, 26*sc, window_GetColor(WDW_MISSION), "Selecttask", 1.0, 0))
    {
        TeamTaskSelectClient();
    }
    if (D_MOUSEHIT == drawStdButton(box.x + box.width/2 + 60*sc, box.y + box.height/2, z, 100*sc, 26*sc, window_GetColor(WDW_MISSION), "MissionAbandonButton", 1.0, ((selectedTask && selectedTask->isAbandonable && selectedTask->ownerDbid == playerPtr()->db_id) ? 0 : BUTTON_LOCKED)))
    {
        dialogStd(DIALOG_YES_NO, textStd("ConfirmMissionAbandon"), NULL, NULL, TeamTaskAbandonUISelection, NULL, 0);
    }
    clipperPop();
}

// Show a sg Mission by putting it into a 1 item static Task Status Set
static int teamTaskShowSGMission(TaskStatus* sgmission, char* owner, int db_id, UIBox drawArea, float z, float sc)
{
    static TaskStatusSet sgSet;
    if (!eaSize(&sgSet.list))
        eaPush(&sgSet.list, sgmission);
    sgSet.list[0] = sgmission;
    return teamTaskSetDisplay(&sgSet, db_id, owner, drawArea, z, 1, 1, 1, 0, sc);
}

static void teamTaskTab()
{
    int i;
    float docStartY, z, sc;
    int color, bcolor;
    UIBox drawArea;
    UIBox bounds;
    UIBox windowBounds;
    static int lastFullHeight = 0;
    static ScrollBar scrollbar={WDW_MISSION, 0, 0};
    Entity* player = playerPtr();
    int showSelectButton = 1;
    TaskStatusSet *set;
    TaskStatus* activetask;
    char buf[1000];

    if( !window_getDims( WDW_MISSION, &bounds.x, &bounds.y, &z, &bounds.width, &bounds.height, &sc, &color, &bcolor ) )
        return;

    windowBounds = bounds;
    uiBoxAlter(&bounds, UIBAT_SHRINK, UIBAD_ALL, PIX3*sc);
    uiBoxAlter(&bounds, UIBAT_SHRINK, UIBAD_BOTTOM, 1*sc);
    if (g_drawsgraidtab)
        uiBoxAlter(&bounds, UIBAT_SHRINK, UIBAD_TOP, (BORDER_SPACE + TAB_HEIGHT)*sc); //only adjust if tabs present

    showSelectButton = !player->teamup || (player->teamup && (player->teamup->members.leader == player->db_id));

    // Determine drawing bounds.
    // Draw all task sets.
    if(showSelectButton)
    {
        uiBoxAlter(&bounds, UIBAT_SHRINK, UIBAD_BOTTOM, TEAMTASK_BUTTON_PANE_HEIGHT*sc);
        doScrollBar(&scrollbar, bounds.height, lastFullHeight, 0, PIX3, z, 0, &bounds);
    }
    else
    {
        doScrollBar(&scrollbar, bounds.height-2, lastFullHeight, 0, PIX3, z, 0, &bounds);
    }
    clipperPushRestrict(&bounds);

    drawArea = bounds;
    drawArea.x += 10*sc;

    drawArea.y -= scrollbar.offset;
    docStartY = drawArea.y;

    // tweak appearance in taskforce mode
    if (player->pl->taskforce_mode)
    {
        set = TaskStatusSetGetPlayer();
        activetask = PlayerGetActiveTask();
        msPrintf(menuMessages, SAFESTR(buf), "MissionTaskForceName");
        drawArea.y += teamTaskSetDisplay(set, player->db_id, buf, drawArea, z, activetask?1:0, 1, 0, 0, sc);
    }
    else // normal mode
    {
        // First show your personal Supergroup task
        if (player->supergroup && player->supergroup->sgMission)
            drawArea.y += teamTaskShowSGMission(player->supergroup->sgMission, player->supergroup->name, player->db_id, drawArea, z, sc);

        // Now show the active SG task, if its not yours
        activetask = PlayerGetActiveTask();
        if (activetask && activetask->isSGMission && (player->supergroup_id != activetask->ownerDbid))
            drawArea.y += teamTaskShowSGMission(activetask, activetask->owner, player->db_id, drawArea, z, sc);

        // show active task at top if I don't have a team member with it
        set = TaskStatusSetGetPlayer();
        if (activetask && !activetask->isSGMission && !TaskStatusSetGet(activetask->ownerDbid))
        {
            if (!PlayerOnMyTeam(activetask->ownerDbid))
                drawArea.y += teamTaskSetDisplay(set, activetask->ownerDbid, activetask->owner, drawArea, z, 1, 0, 0, 1, sc);
        }

        // players tasks
        drawArea.y += teamTaskSetDisplay(set, player->db_id, player->name, drawArea, z, 0, 1, 0, player->teamup && player->db_id == player->teamup->activePlayerDbid, sc);

        // teammates tasks
        for(i=0; player->teamup && i<player->teamup->members.count; i++)
        {
            int firstentry = 0;
            if (player->teamup->members.ids[i] == player->db_id) continue;
            
            // if I don't have tasks, but this is owner of active task, show first entry
            set = TaskStatusSetGet(player->teamup->members.ids[i]);
            if (!set && activetask && activetask->ownerDbid == player->teamup->members.ids[i])
            {
                set = TaskStatusSetGetPlayer();
                firstentry = 1;
            }

            drawArea.y += teamTaskSetDisplay(set, player->teamup->members.ids[i],
                player->teamup->members.names[i], drawArea, z, firstentry, 1, 0, player->teamup->members.ids[i] == player->teamup->activePlayerDbid, sc);
        }
    }

    lastFullHeight = drawArea.y - docStartY;
    clipperPop();

    {
        int maxTaskListHeight = 300*sc;
        int taskListHeight = min(maxTaskListHeight, lastFullHeight);
        if(showSelectButton)
            taskListHeight += TEAMTASK_BUTTON_PANE_HEIGHT*sc;
        if (g_drawsgraidtab)
            taskListHeight += (BORDER_SPACE + TAB_HEIGHT)*sc;
        if(WINDOW_DISPLAYING == window_getMode(WDW_MISSION))
            window_setDims(WDW_MISSION, windowBounds.x, windowBounds.y, windowBounds.width, taskListHeight+(PIX3*2+5)*sc );
    }

    // Draw the button pane.
    if(showSelectButton)
    {
        bounds.y += bounds.height;
        bounds.height = TEAMTASK_BUTTON_PANE_HEIGHT*sc;
        teamTaskButtonPane(bounds, z, sc);
    }
    return;
}

typedef enum
{
    MCT_MISSION,
    MCT_SGRAID,
} MissionContentType;
static MissionContentType contentType;

void missionSelectTab(void*data)
{
    contentType = (int)data;
}

int missionWindow()
{
    float x, y, z, wd, ht, sc;
    int color, bcolor;
    static uiTabControl * tc;
    static int init = false;

    if (!init)
    { 
        tc = uiTabControlCreate(TabType_Undraggable, missionSelectTab, 0, 0, 0, 0 );
        uiTabControlAdd(tc,"MissionTab",(int*)0);
        uiTabControlAdd(tc,"SGRaidTab",(int*)1);
        uiTabControlSelect(tc,(int*)0);
        init = true;
    }

    g_drawsgraidtab = 0;

     if(!window_getDims(WDW_MISSION, &x, &y, &z, &wd, &ht, &sc, &color, &bcolor))
        return 0;

    drawFrame( PIX3, R10, x, y, z, wd, ht, sc, color, bcolor );

    missionToolTipCleanup();
    teamTaskTab();

    s_bReparse = false;
    return 0;
}

/* End of File */
