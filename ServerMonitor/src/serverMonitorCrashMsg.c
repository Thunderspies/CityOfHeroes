#include <utilitieslib/network/netio.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/components/EString.h>
#include <utilitieslib/utils/sysutil.h>
#include <utilitieslib/utils/utils.h>

#include "container.h"
#include "serverMonitorCommon.h"
#include "serverMonitorCrashMsg.h"
#include "resource.h"

static HWND hCrashMsgDialog    = NULL;
static bool bSmStatusUp        = false;
static bool bSmStatusHidden = false;

void smCrashMsgShow()
{
    if (hCrashMsgDialog == NULL || !bSmStatusUp) {
        hCrashMsgDialog = CreateDialog(g_hInst, (LPCTSTR)(intptr_t)(IDD_DLG_STATUS), NULL, (DLGPROC)DlgSvrMonCrashMsgProc); 
        ShowWindow(hCrashMsgDialog, SW_SHOW);
        bSmStatusUp = true;
    }
    if (bSmStatusHidden) {
        bSmStatusHidden = false;
        ShowWindow(hCrashMsgDialog, SW_SHOW);
    }
}


void smCrashMsgHide()
{
    if (hCrashMsgDialog && bSmStatusUp && !bSmStatusHidden) {
        bSmStatusHidden = true;
        ShowWindow(hCrashMsgDialog, SW_HIDE);
    }
}

void updateCrashMsg(HWND parent, char *text)
{
    BOOL ret;
    static char old_text[4096];
    if (text && text[0] && strcmp(old_text, text)==0)
        return;
    if (text && text[0]) {
        smCrashMsgShow();    // make sure that it's up
        SetFocus(parent);    // status dialog should not steal the focus

        ret = SetDlgItemTextA(hCrashMsgDialog, IDC_EDIT_CRASH_MSG, text);
        assert(ret);
    } else {
        smCrashMsgHide();
    }
    if (!text)
        text = "";
    Strncpyt(old_text, text);

}

void updateCrashMsgText(ListView *lv, MapCon* con, void *unused)
{
//    MapCon *con; 
//    assert(item_index >= 0 && item_index < eaSize(&(lv->eaElements_data)));
//    con  = (MapCon*) lv->eaElements_data[item_index];

    if (con && con->raw_data) {
        
        int count = 0;
        char * p = con->raw_data;
        char * estr = NULL;

        // determine size of new string, noting that '\n' is to be replaced with '\r\n'
    
        estrConcatf(&estr, "IP ADDR: %s\r\n\r\n", makeIpStr(con->ip_list[0]));

        // add on crash message
        while(*p)
        {
            if(*p == '\n')
                estrConcatStaticCharArray(&estr, "\r\n");
            else
                estrConcatChar(&estr, *p);

            ++p;
        }
        
        updateCrashMsg(listViewGetListViewWindow(lv), estr);

        estrDestroy(&estr);
    } else {
        updateCrashMsg(listViewGetListViewWindow(lv), NULL);
    }
}


LRESULT CALLBACK DlgSvrMonCrashMsgProc (HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    switch (iMsg)
    {
    case WM_INITDIALOG:
        return FALSE;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDOK:
        case IDCANCEL:
            {
                bSmStatusUp = false;
                EndDialog(hDlg, 0);
                return TRUE;
            }
        case IDC_BUTTON_STATUS_COPY:
            {
                char buf[10000];
                GetDlgItemTextA(hCrashMsgDialog, IDC_EDIT_CRASH_MSG, buf, (sizeof(buf) / sizeof(char)));
                winCopyToClipboard(buf);
                break;
            }
        }
        break;

    case WM_NOTIFY:
        {
            // NOTHING (for now?)
        }
        break;
    }
    return FALSE;
}
