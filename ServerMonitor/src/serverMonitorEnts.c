#include <container.h>
#include <svrmoncomm.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/utils/ListView.h>

#include <stdio.h>
#include <winsock2.h>

#include "serverMonitor.h"
#include "serverMonitorCommon.h"
#include "serverMonitorEnts.h"
#include "serverMonitorNet.h"
#include "resource.h"

HWND hEntsDialog = NULL;
bool bSmentsUp=false;
int filter_id=-1;

int smentsFilter(DbContainer *dbcon, void *filterData) // Returns 1 if it passes the filter
{
    EntCon *con = (EntCon*)dbcon;
    if (filter_id==-1 || filter_id == con->map_id)
        return 1;
    return 0;
}

INT_PTR CALLBACK DlgSvrMonEntsProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    int i=0;
    ServerMonitorState *state=&g_state;

    switch (iMsg)
    {
    case WM_INITDIALOG:
        if (state->lvEnts) listViewDestroy(state->lvEnts);
        state->lvEnts = listViewCreate();
        listViewInit(state->lvEnts, EntConNetInfo, hDlg, GetDlgItem(hDlg, IDC_LST_ENTS));
        for (i=0; i<eaSize(state->eaEnts); i++) {
            if (smentsFilter(eaGet(state->eaEnts, i), NULL))
                listViewAddItem(state->lvEnts, eaGet(state->eaEnts, i));
        }
        return FALSE;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDOK:
        case IDCANCEL:
            bSmentsUp = false;
            EndDialog(hDlg, 0);
            return TRUE;
        case IDC_BTN_REFRESH:
            svrMonRequestEnts(state, 1);
            break;
        }

    case WM_NOTIFY:
        {
            int idCtrl = (int)wParam;
            if (listViewOnNotify(state->lvEnts, wParam, lParam, NULL))
                return TRUE;
        }
        break;
    }
    return FALSE;
}

void smentsSetFilterId(ServerMonitorState *state, int id)
{
    int i;

    filter_id = id;
    if (bSmentsUp && state->lvEnts) 
    {
        listViewDelAllItems(state->lvEnts, NULL);
        for (i=0; i<eaSize(state->eaEnts); i++) {
            if (smentsFilter(eaGet(state->eaEnts, i), NULL))
                listViewAddItem(state->lvEnts, eaGet(state->eaEnts, i));
        }
    }
}

void smentsShow(ServerMonitorState *state)
{
    if (hEntsDialog == NULL || !bSmentsUp) {
        hEntsDialog = CreateDialog(g_hInst, (LPCTSTR)(intptr_t)(IDD_DLG_ENTS), NULL, DlgSvrMonEntsProc);
        ShowWindow(hEntsDialog, SW_SHOW);
        bSmentsUp = true;
        svrMonRequestEnts(state, 1);
    }
}

int smentsHook(PMSG pmsg)
{
    if (hEntsDialog) {
        if (IsDialogMessage(hEntsDialog, pmsg)) {
            if (pmsg->message==0) {
                bSmentsUp = false;
                hEntsDialog = NULL;
                svrMonRequestEnts(&g_state, 0);
            }
            return 1;
        }
    }
    return 0;
}

int smentsVisible(ServerMonitorState *state)
{
    return bSmentsUp;
}

