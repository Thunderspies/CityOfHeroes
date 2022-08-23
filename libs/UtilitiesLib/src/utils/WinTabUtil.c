#ifndef _WIN_TAB_UTIL_H
#define _WIN_TAB_UTIL_H

#ifdef UNICODE
#undef UNICODE
#define WAS_UNICODE
#endif // UNICODE

#include "utilitieslib/utils/WinTabUtil.h"
#include "utilitieslib/components/earray.h"
#include "utilitieslib/utils/strings_opt.h"
#include "utilitieslib/utils/SuperAssert.h"
#include "utilitieslib/utils/utils.h"

#include <commctrl.h>

//---------------------------------------------------------------------------------------
// TAB CONTROL UTILITIES
//---------------------------------------------------------------------------------------

static DLGHDR * g_pHdr;

DLGTEMPLATE * WINAPI DoLockDlgRes(HINSTANCE hInst, LPCSTR lpszResName) 
{ 
    HRSRC hrsrc = FindResourceA(NULL, lpszResName, MAKEINTRESOURCEA(RT_DIALOG));
    HGLOBAL hglb = LoadResource(hInst, hrsrc); 
    return (DLGTEMPLATE *) LockResource(hglb); 
} 

bool TabDlgActivateTabIfAvailable(DLGHDR *pHdr, char *title)
{
    int i;
    if (pHdr == NULL) {
        pHdr = g_pHdr;
    }
    assert(pHdr);

    for (i=0; i<eaSize(&pHdr->eaTabs); i++) {
        if (stricmp(pHdr->eaTabs[i]->title, title)==0) {
            TabCtrl_SetCurSel(pHdr->hwndTab, i);
            TabDlgOnSelChanged(pHdr->hwndParent);
            return true;
        }
    }
    return false;
}


// TabDlgOnSelChanged - processes the TCN_SELCHANGE notification (ie User clicks on a tab)
// hwndDlg - handle to the parent dialog box. 
VOID WINAPI TabDlgOnSelChanged(HWND hwndDlg) 
{ 
#pragma warning (disable: 4312)    // GetWindowLongPtr produces a warning under /Wp64, but in fact correctly compiles for _x64
    DLGHDR *pHdr = (DLGHDR *) GetWindowLongPtr(hwndDlg, GWLP_USERDATA); 
#pragma warning (error: 4312)

    int iSel = TabCtrl_GetCurSel(pHdr->hwndTab); 

    // hide the current window
    if(pHdr->hwndCurrent)
        ShowWindow(pHdr->hwndCurrent, FALSE);

    assert(pHdr->eaTabs[iSel]->hwndDisplay);

    pHdr->hwndCurrent = pHdr->eaTabs[iSel]->hwndDisplay;

    ShowWindow(pHdr->hwndCurrent, TRUE);
} 

HWND TabDlgInsertTab(HINSTANCE hInst, DLGHDR *pHdr, int resource, void *dlgProc, char *title, bool showme, void * param)
{
    TCITEMA tie;
    RECT rcTab;
    HWND hDlg;
    DWORD dwDlgBase = GetDialogBaseUnits(); 
    int cxMargin = LOWORD(dwDlgBase) / 4;
    DLGHDR_TAB *tab = calloc(sizeof(DLGHDR_TAB),1);

    if (pHdr == NULL) {
        pHdr = g_pHdr;
    }
    assert(pHdr);

    eaPush(&pHdr->eaTabs, tab); // Add the new tab to the list

    hDlg = pHdr->hwndParent;

    memset(&tie, 0, sizeof(tie));
    tie.mask = TCIF_TEXT | TCIF_IMAGE; 
    tie.iImage = -1; 

    tab->apRes        = DoLockDlgRes(hInst, MAKEINTRESOURCEA(resource));
    tab->dlgProc    = dlgProc;
    Strncpyt(tab->title, title);
    tie.pszText        = title;
    TabCtrl_InsertItem(pHdr->hwndTab, eaSize(&pHdr->eaTabs)-1, &tie);

    // Set Window positions
    tab->hwndDisplay = CreateDialogIndirectParam(hInst,tab->apRes,hDlg,tab->dlgProc, (LPARAM)param);

    // position the dialog below the tabs
    ShowWindow(tab->hwndDisplay, FALSE);
    GetWindowRect(tab->hwndDisplay, &rcTab);
    SetWindowPos(tab->hwndDisplay, NULL, 
        cxMargin,
        GetSystemMetrics(SM_CYCAPTION),
        (rcTab.right - rcTab.left) + 2 * GetSystemMetrics(SM_CXDLGFRAME), 
        (rcTab.bottom - rcTab.top) + 2 * GetSystemMetrics(SM_CYDLGFRAME) + 2 * GetSystemMetrics(SM_CYCAPTION), 
        0);

    if (showme) {
        TabCtrl_SetCurSel(pHdr->hwndTab, eaSize(&pHdr->eaTabs)-1);
        TabDlgOnSelChanged(pHdr->hwndParent);
    }

    return tab->hwndDisplay;
}


void TabDlgRemoveTab(DLGHDR *pHdr, HWND hDlg)
{
    int i;
    int index=-1;
    DLGHDR_TAB *tab;

    if (pHdr == NULL) {
        pHdr = g_pHdr;
    }
    assert(pHdr);
    for (i=0; i<eaSize(&pHdr->eaTabs); i++) {
        if (pHdr->eaTabs[i]->hwndDisplay == hDlg) {
            index = i;
            break;
        }
    }
    assert(index!=-1);
    assert(index!=0 || eaSize(&pHdr->eaTabs)>1);
    if (pHdr->hwndCurrent == hDlg) {
        // Show someone else
        if (index==0 || index < eaSize(&pHdr->eaTabs)-1) {
            i = index +1;
        } else {
            i = index-1;
        }
        TabCtrl_SetCurSel(pHdr->hwndTab, i);
        TabDlgOnSelChanged(pHdr->hwndParent);
    }
    // Remove me
    TabCtrl_DeleteItem(pHdr->hwndTab, index);
    tab = eaRemove(&pHdr->eaTabs, index);
    assert(tab);
    free(tab);
}




VOID WINAPI TabDlgInit(HINSTANCE hInst, HWND hwndDlg) 
{ 
    DLGHDR *pHdr = (DLGHDR *) GlobalAlloc(GPTR, sizeof(DLGHDR)); 
    g_pHdr = pHdr;

    // Save a pointer to the DLGHDR structure. 
    SetWindowLongPtr(hwndDlg, GWLP_USERDATA, (LONG_PTR) pHdr); 

    // initialize dialog info
    memset(pHdr, 0, sizeof(DLGHDR));
    pHdr->hwndParent = hwndDlg;

    // Create the tab control. 
    pHdr->hwndTab = CreateWindowA(WC_TABCONTROLA, "", WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE, 0, 0, 2400, 1600, 
        hwndDlg, NULL, hInst, NULL 
        ); 

    // set tab font to default
    SendMessageA(pHdr->hwndTab, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(TRUE, 0)); 

    assert(pHdr->hwndTab);
} 

void TabDlgFinalize(HWND hwndDlg)
{
#pragma warning (disable: 4312)    // GetWindowLongPtr produces a warning under /Wp64, but in fact correctly compiles for _x64
    DLGHDR *pHdr = (DLGHDR *) GetWindowLongPtr(hwndDlg, GWLP_USERDATA); 
#pragma warning (error: 4312)
    RECT rcTab;
    RECT rcMain;
    DWORD dwDlgBase = GetDialogBaseUnits(); 
    int cxMargin = LOWORD(dwDlgBase) / 4; 
    int cyMargin = HIWORD(dwDlgBase) / 8; 

    // Add a tab for each of the child dialog boxes. 
    // Size the dialog box based on the 0th Tab
    GetWindowRect(pHdr->eaTabs[0]->hwndDisplay, &rcTab);
    GetWindowRect(pHdr->hwndParent, &rcMain);

    SetWindowPos(pHdr->hwndParent, NULL, 
        rcMain.left, 
        rcMain.top, 
        (rcTab.right - rcTab.left)+ 2* cxMargin + 2 * GetSystemMetrics(SM_CXDLGFRAME), 
        (rcTab.bottom - rcTab.top) + 2 * cyMargin + 2 * GetSystemMetrics(SM_CYDLGFRAME) + GetSystemMetrics(SM_CYCAPTION), 
        SWP_NOZORDER);

    // Simulate selection of the first item. 
    TabDlgOnSelChanged(hwndDlg); 
}

#ifdef WAS_UNICODE
#define UNICODE
#undef WAS_UNICODE
#endif // WAS_UNICODE

#endif // _WIN_TAB_UTIL_H
