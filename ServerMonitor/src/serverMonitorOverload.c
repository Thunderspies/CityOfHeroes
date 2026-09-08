#include <container.h>
#include <svrmoncomm.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/utils/ListView.h>

#include <winsock2.h>
#include <stdio.h>

#include "resource.h"
#include "serverMonitor.h"
#include "serverMonitorCommon.h"
#include "serverMonitorNet.h"
#include "serverMonitorOverload.h"

// Overload Protection dialog box

HWND hOverloadDialog = NULL;
bool bSmoverloadUp=false;
static int lastOverloadProtection = -2;
static HBRUSH hbrBkgndOff = 0;
static HBRUSH hbrBkgndOn = 0;

// @todo move this enum to a new overloadprotection_common.h shared with ServerMonitor code
// until then update code\CoH\dbserver\overloadProtection.c with any changes to these flags
enum
{
    OVERLOADPROTECTIONFLAG_MANUAL_OVERRIDE        = 1 << 0,
    OVERLOADPROTECTIONFLAG_LAUNCHERS_FULL        = 1 << 1,
    OVERLOADPROTECTIONFLAG_MONITOR_OVERRIDE        = 1 << 2,
    OVERLOADPROTECTIONFLAG_SQL_QUEUE            = 1 << 3,
};

INT_PTR CALLBACK DlgSvrMonOverloadProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    int i=0;
    ServerMonitorState *state=&g_state;

    switch (iMsg)
    {
    case WM_INITDIALOG:
        lastOverloadProtection = -2;
        return FALSE;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDOK:
        case IDCANCEL:
            bSmoverloadUp = false;
            EndDialog(hDlg, 0);
            return TRUE;

        case IDC_BUTTON1:
            svrMonSendOverloadProtection(state, smoverloadIsManualOverride(&state->stats) ? "0" : "1" );
            return TRUE;
        }
        break;

        // I was trying to get the background of the edit box to change colors
    case WM_CTLCOLORSTATIC:
        {
            HDC hdcStatic = (HDC) wParam;
            HBRUSH brush;

            if (hbrBkgndOff == NULL)
            {
                hbrBkgndOff = CreateSolidBrush(RGB(200,255,200));
                hbrBkgndOn = CreateSolidBrush(RGB(255,100,100));
            }

            if (lastOverloadProtection)
            {
                SetBkColor(hdcStatic, RGB(255,100,100));
                brush = hbrBkgndOn;
            }
            else
            {
                SetBkColor(hdcStatic, RGB(200,255,200));
                brush = hbrBkgndOff;
            }

            return (INT_PTR)brush;
        }
    }
    return FALSE;
}

void smoverloadShow(ServerMonitorState *state)
{
    if (hOverloadDialog == NULL || !bSmoverloadUp) {
        bSmoverloadUp = true;
        hOverloadDialog = CreateDialog(g_hInst, (LPCTSTR)(intptr_t)(IDD_DLG_OVERLOADPROTECTION), NULL, DlgSvrMonOverloadProc);
        smoverloadUpdate();
        ShowWindow(hOverloadDialog, SW_SHOW);
    }
}

int smoverloadHook(PMSG pmsg)
{
    return 0;
}

int smoverloadIsVisible()
{
    return bSmoverloadUp;
}

void smoverloadUpdate()
{
    static char new_text[4096];
    ServerMonitorState *state=&g_state;

    if (!bSmoverloadUp || !hOverloadDialog)
        return;

    if (state->stats.overloadProtection == lastOverloadProtection)
        return;

    lastOverloadProtection = state->stats.overloadProtection;

    // Update the edit box text and color
    new_text[0] = 0;

    if (lastOverloadProtection)
    {
        smoverloadFormat(&state->stats, new_text);
    }
    else
    {
        strcpy(new_text, "Overload Protection is not enabled");
    }

    SetDlgItemTextA(hOverloadDialog, IDC_EDIT_OVERLOAD_MSG, new_text);

    // Update the button text
    if (smoverloadIsManualOverride(&state->stats))
    {
        SetDlgItemTextA(hOverloadDialog, IDC_BUTTON1, "Disable Manual Overload Protection");
    }
    else
    {
        SetDlgItemTextA(hOverloadDialog, IDC_BUTTON1, "Enable Manual Overload Protection");
    }
    
}

void smoverloadFormat(ServerStats *stats, char *string)
{
    if (stats->overloadProtection & OVERLOADPROTECTIONFLAG_MANUAL_OVERRIDE)
    {
        strcatf(string, "Overload Protection enabled via servers.cfg\r\n");
    }
    if (stats->overloadProtection & OVERLOADPROTECTIONFLAG_LAUNCHERS_FULL)
    {
        strcatf(string, "Overload Protection enabled because too many launchers were unable to launch maps\r\n");
    }
    if (stats->overloadProtection & OVERLOADPROTECTIONFLAG_MONITOR_OVERRIDE)
    {
        strcatf(string, "Overload Protection enabled via shard or server monitor\r\n");
    }
    if (stats->overloadProtection & OVERLOADPROTECTIONFLAG_SQL_QUEUE)
    {
        strcatf(string, "Overload Protection enabled, queueing only, because of high SQL load\r\n");
    }
}

bool smoverloadIsManualOverride(ServerStats *stats)
{
    return !!(stats->overloadProtection & OVERLOADPROTECTIONFLAG_MONITOR_OVERRIDE);
}

