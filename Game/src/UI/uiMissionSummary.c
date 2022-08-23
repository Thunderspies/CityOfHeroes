/***************************************************************************
 *     Copyright (c) 2003-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/

#include <utilitieslib/utils/utils.h>
#include <utilitieslib/components/earray.h>
#include "gameComm/wdwbase.h"
#include "UI/sprite/sprite_base.h"
#include "UI/sprite/sprite_text.h"

#include "UI/uiWindows.h"
#include "UI/uiUtil.h"
#include "UI/uiUtilGame.h"
#include "UI/uiScrollBar.h"
#include "uiSMFView.h"
#include "storyarc/contactClient.h"
#include "clientcomm/clientcomm.h"
#include "entity/entVarUpdate.h"
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/language/MessageStoreUtil.h>

static int s_dbid;
static int s_context;
static int s_handle;
static U32 s_iAskedWhen = 0;
static int s_bReparse = 0;
StuffBuff s_sb;

TaskStatus zoneEvent;

void missionSummaryShow(int dbid, int context, int handle)
{
    if (s_dbid != dbid || s_context != context || s_handle != handle)
    {
        s_iAskedWhen = 0;
    }
    s_dbid = dbid;
    s_context = context;
    s_handle = handle;

    window_openClose( WDW_MISSION_SUMMARY );
}

void missionSummaryReparse()
{
    s_bReparse = 1;
}
int missionSummaryWindow(void)
{
    float x, y, z, wd, ht, sc;
    int color, bcolor;
    static SMFView *s_pview;
    TaskStatus* task;

     if( !window_getDims( WDW_MISSION_SUMMARY, &x, &y, &z, &wd, &ht, &sc, &color, &bcolor))
        return 0;

    if( window_getMode( WDW_CONTACT_DIALOG ) != WINDOW_DOCKED )
    {
        window_setMode( WDW_MISSION_SUMMARY, WINDOW_DOCKED );
        return 0;
    }

    drawFrame( PIX3, R10, x, y, z, wd, ht, sc, color, bcolor );

    task = TaskStatusGet(s_dbid, s_context, s_handle);

    if(s_pview==NULL)
    {
        s_pview = smfview_Create(WDW_MISSION_SUMMARY);
        initStuffBuff(&s_sb, 1024);

        // Sets the location within the window of the text region
        smfview_SetLocation(s_pview, PIX3*sc, PIX3+R10*sc, 0);
    }

    if (!task)
    {
        // you can no longer view this task...
        clearStuffBuff(&s_sb);
        addSingleStringToStuffBuff(&s_sb, textStd("ClickForMissionInfo"));
        smfview_SetText(s_pview, s_sb.buff);
    }
    else
    {
        if(task->detailInvalid)
        {
            if (s_iAskedWhen == 0 
                || (timerSecondsSince2000() - s_iAskedWhen > 5))
            {
                START_INPUT_PACKET(pak, CLIENTINP_REQUEST_TASK_DETAIL);
                pktSendBitsPack(pak, 1, s_dbid);
                pktSendBitsPack(pak, 1, s_context);
                pktSendBitsPack(pak, 1, s_handle);
                END_INPUT_PACKET
                s_iAskedWhen = timerSecondsSince2000();
            }
            clearStuffBuff(&s_sb);
            addSingleStringToStuffBuff(&s_sb, textStd("GettingTaskDetails"));
            smfview_Reparse(s_pview);
            smfview_SetText(s_pview, s_sb.buff);
        }
        else 
        {
            if (s_bReparse)
            {
                smfview_Reparse(s_pview);
                s_bReparse = 0;
            }
            smfview_SetText(s_pview, task->detail);
        }

        // Sets the size of the region within the window of the text region
        // In this case, it resizes with the window
        smfview_SetSize(s_pview, wd-(PIX3*2-5)*sc, ht-(PIX3+R10)*sc);
        smfview_Draw(s_pview);
    }

    return 0;
}

/* End of File */
