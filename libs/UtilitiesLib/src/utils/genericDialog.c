#ifndef _XBOX

#include "utilitieslib/utils/genericDialog.h"
#include "utilitieslib/utils/winutil.h"
#include "utilitieslib/utils/utils.h"
#include "shlobj.h"

//////////////////////////////////////////////////////////////////////
// these ugly looking things are the resources for the dialog boxes
// in hex.  because of the way the utilities lib is set up, resource
// files cannot be included.
//////////////////////////////////////////////////////////////////////
char OkToAllDialogResource[] =
{
    0x01, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc8, 0x00, 0xc0, 0x80,
    0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf5, 0x00, 0x5a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x44, 0x00,
    0x69, 0x00, 0x61, 0x00, 0x6c, 0x00, 0x6f, 0x00, 0x67, 0x00, 0x00, 0x00, 0x08, 0x00, 0x90, 0x01,
    0x00, 0x01, 0x4d, 0x00, 0x53, 0x00, 0x20, 0x00, 0x53, 0x00, 0x68, 0x00, 0x65, 0x00, 0x6c, 0x00,
    0x6c, 0x00, 0x20, 0x00, 0x44, 0x00, 0x6c, 0x00, 0x67, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x50, 0x3a, 0x00, 0x45, 0x00, 0x32, 0x00, 0x0e, 0x00,
    0x01, 0x00, 0x00, 0x00, 0xff, 0xff, 0x80, 0x00, 0x4f, 0x00, 0x4b, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x50, 0x8b, 0x00, 0x45, 0x00,
    0x32, 0x00, 0x0e, 0x00, 0x05, 0x04, 0x00, 0x00, 0xff, 0xff, 0x80, 0x00, 0x4f, 0x00, 0x4b, 0x00,
    0x20, 0x00, 0x74, 0x00, 0x6f, 0x00, 0x20, 0x00, 0x41, 0x00, 0x6c, 0x00, 0x6c, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x44, 0x08, 0x01, 0x50,
    0x07, 0x00, 0x07, 0x00, 0xe7, 0x00, 0x37, 0x00, 0x06, 0x04, 0x00, 0x00, 0xff, 0xff, 0x81, 0x00,
    0x00, 0x00, 0x00, 0x00
};

char OkToAllCancelDialogResource[] =
{
    0x01, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC8, 0x00, 0xC0, 0x80,
    0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF5, 0x00, 0x5A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x44, 0x00,
    0x69, 0x00, 0x61, 0x00, 0x6C, 0x00, 0x6F, 0x00, 0x67, 0x00, 0x00, 0x00, 0x08, 0x00, 0x90, 0x01,
    0x00, 0x01, 0x4D, 0x00, 0x53, 0x00, 0x20, 0x00, 0x53, 0x00, 0x68, 0x00, 0x65, 0x00, 0x6C, 0x00,
    0x6C, 0x00, 0x20, 0x00, 0x44, 0x00, 0x6C, 0x00, 0x67, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x50, 0x57, 0x00, 0x45, 0x00, 0x32, 0x00, 0x0E, 0x00,
    0x01, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x80, 0x00, 0x4F, 0x00, 0x4B, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x50, 0x14, 0x00, 0x45, 0x00,
    0x32, 0x00, 0x0E, 0x00, 0x05, 0x04, 0x00, 0x00, 0xFF, 0xFF, 0x80, 0x00, 0x4F, 0x00, 0x4B, 0x00,
    0x20, 0x00, 0x74, 0x00, 0x6F, 0x00, 0x20, 0x00, 0x41, 0x00, 0x6C, 0x00, 0x6C, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x44, 0x08, 0x01, 0x50,
    0x07, 0x00, 0x07, 0x00, 0xE7, 0x00, 0x37, 0x00, 0x0B, 0x04, 0x00, 0x00, 0xFF, 0xFF, 0x81, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x50,
    0x95, 0x00, 0x45, 0x00, 0x32, 0x00, 0x0E, 0x00, 0x02, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x80, 0x00,
    0x43, 0x00, 0x61, 0x00, 0x6E, 0x00, 0x63, 0x00, 0x65, 0x00, 0x6C, 0x00, 0x00, 0x00, 0x00, 0x00
};

char RequestStringDialogResource[] =
{
    0x01, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc8, 0x00, 0xc8, 0x80,
    0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd7, 0x00, 0x6f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x44, 0x00,
    0x69, 0x00, 0x61, 0x00, 0x6c, 0x00, 0x6f, 0x00, 0x67, 0x00, 0x00, 0x00, 0x08, 0x00, 0x90, 0x01,
    0x00, 0x01, 0x4d, 0x00, 0x53, 0x00, 0x20, 0x00, 0x53, 0x00, 0x68, 0x00, 0x65, 0x00, 0x6c, 0x00,
    0x6c, 0x00, 0x20, 0x00, 0x44, 0x00, 0x6c, 0x00, 0x67, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x50, 0x5f, 0x00, 0x5a, 0x00, 0x32, 0x00, 0x0e, 0x00,
    0x01, 0x00, 0x00, 0x00, 0xff, 0xff, 0x80, 0x00, 0x4f, 0x00, 0x4b, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x50, 0x9e, 0x00, 0x5a, 0x00,
    0x32, 0x00, 0x0e, 0x00, 0x02, 0x00, 0x00, 0x00, 0xff, 0xff, 0x80, 0x00, 0x43, 0x00, 0x61, 0x00,
    0x6e, 0x00, 0x63, 0x00, 0x65, 0x00, 0x6c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x80, 0x08, 0x81, 0x50, 0x07, 0x00, 0x07, 0x00, 0xc9, 0x00, 0x33, 0x00,
    0x09, 0x04, 0x00, 0x00, 0xff, 0xff, 0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x81, 0x50, 0x07, 0x00, 0x41, 0x00, 0xc9, 0x00, 0x10, 0x00,
    0x0a, 0x04, 0x00, 0x00, 0xff, 0xff, 0x81, 0x00, 0x00, 0x00, 0x00, 0x00
};



void flashWindow( HWND hWnd )
{
    FLASHWINFO fw;
    fw.cbSize = sizeof(FLASHWINFO);
    fw.hwnd = hWnd;
    fw.uCount = 0;
    fw.dwTimeout = 0;
    fw.dwFlags = FLASHW_TRAY | FLASHW_TIMERNOFG;
    //counter = GetTickCount();
    //FlashWindow( hWnd, TRUE );
    FlashWindowEx( &fw );
}

///////////////////////////////////////////////////////////
// OK TO ALL WITH CANCEL MESSAGE BOX
///////////////////////////////////////////////////////////

static char g_OkToAllCancelString[1024];
static char g_OkToAllCancelCaption[512];
GenericDialogCallback g_OkToAllCancelCallback = NULL;

INT_PTR CALLBACK DlgOkToAllCancelProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    switch (iMsg)
    {
    case WM_INITDIALOG:
        SetWindowTextA( hDlg, g_OkToAllCancelCaption );
        SetWindowTextA(GetDlgItem(hDlg, IDC_OKTOALLCANCEL_EDIT), g_OkToAllCancelString);
        // flash the window if it is not the focus
        flashWindow(hDlg);
        break;
    case WM_COMMAND:
        if ( LOWORD(wParam) == IDCANCEL )
        {
            EndDialog(hDlg, IDCANCEL);
            return TRUE;
        }
        if ( LOWORD(wParam) == IDOK )
        {
            EndDialog(hDlg, IDOK);
            return TRUE;
        }
        if ( LOWORD(wParam) == IDC_OKTOALL )
        {
            EndDialog(hDlg, IDC_OKTOALL);
            return TRUE;
        }
        break;
    }

    return g_OkToAllCancelCallback == NULL ? FALSE : g_OkToAllCancelCallback(hDlg, iMsg, wParam, lParam);
}

int okToAllCancelDialog(const char *dialogText, const char *caption )
{
    int ret;
    if(isGuiDisabled())
    {
        printf("Dialog boxes not supported when running with -nogui.  Returning IDCANCEL\n");
        return IDCANCEL;
    }
    strcpy( g_OkToAllCancelString, dialogText );
    strcpy( g_OkToAllCancelCaption, caption );
    ret = DialogBoxIndirect(winGetHInstance(), (LPDLGTEMPLATE)OkToAllCancelDialogResource, NULL, DlgOkToAllCancelProc);
    return (IDC_OKTOALL == ret ) ? IDOKTOALL : ((IDOK == ret) ? IDOK : IDCANCEL) ;
    //return (IDC_OKTOALL == DialogBox(winGetHInstance(), (LPCTSTR) (IDD_MB_OKTOALL), NULL, DlgOkToAllProc) );
}
int okToAllCancelDialogEx(char *dialogText, char *caption, GenericDialogCallback callback_proc)
{
    int ret;
    g_OkToAllCancelCallback = callback_proc;
    ret = okToAllCancelDialog(dialogText, caption );
    g_OkToAllCancelCallback = NULL;
    return ret;
}

///////////////////////////////////////////////////////////
// END OK TO ALL WITH CANCEL MESSAGE BOX
///////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////
// OK TO ALL MESSAGE BOX
///////////////////////////////////////////////////////////

static char g_OkToAllString[1024];
static char g_OkToAllCaption[512];
GenericDialogCallback g_OkToAllCallback = NULL;

INT_PTR CALLBACK DlgOkToAllProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    switch (iMsg)
    {
    case WM_INITDIALOG:
        SetWindowTextA( hDlg, g_OkToAllCaption );
        SetWindowTextA(GetDlgItem(hDlg, IDC_OKTOALL_EDIT), g_OkToAllString);
        // flash the window if it is not the focus
        flashWindow(hDlg);
        break;
    case WM_COMMAND:
        if ( LOWORD(wParam) == IDOK )
        {
            EndDialog(hDlg, IDOK);
            return TRUE;
        }
        if ( LOWORD(wParam) == IDC_OKTOALL )
        {
            EndDialog(hDlg, IDC_OKTOALL);
            return TRUE;
        }
        break;
    }

    return g_OkToAllCallback == NULL ? FALSE : g_OkToAllCallback(hDlg, iMsg, wParam, lParam);
    return FALSE;
}

int okToAllDialog(const char *dialogText, const char *caption)
{
    if(isGuiDisabled())
    {
        printf("Dialog boxes not supported when running with -nogui.  Returning IDOK\n");
        return IDOK;
    }
    strcpy( g_OkToAllString, dialogText );
    strcpy( g_OkToAllCaption, caption );
    return (IDC_OKTOALL == DialogBoxIndirect(winGetHInstance(), (LPDLGTEMPLATE)OkToAllDialogResource, NULL, DlgOkToAllProc) ) ? IDOKTOALL : IDOK;
    //return (IDC_OKTOALL == DialogBox(winGetHInstance(), (LPCTSTR) (IDD_MB_OKTOALL), NULL, DlgOkToAllProc) );
}
int okToAllDialogEx(char *dialogText, char *caption, GenericDialogCallback callback_proc)
{
    int ret;
    g_OkToAllCallback = callback_proc;
    ret = okToAllDialog(dialogText, caption);
    g_OkToAllCallback = NULL;
    return ret;
}

///////////////////////////////////////////////////////////
// END OK TO ALL MESSAGE BOX
///////////////////////////////////////////////////////////


int okCancelDialog(const char *dialogText, const char *caption)
{
    if(isGuiDisabled())
    {
        printf("Dialog boxes not supported when running with -nogui.  Returning IDOK\n");
        return IDOK;
    }

    return MessageBoxA(NULL, dialogText, caption, MB_OKCANCEL);
}


///////////////////////////////////////////////////////////
// REQUEST STRING DIALOG
///////////////////////////////////////////////////////////

static char g_RequestStringText[1024];
static char g_RequestStringCaption[512];
static char g_RequestStringInput[512];
GenericDialogCallback g_RequestStringCallback = NULL;

INT_PTR CALLBACK DlgRequestStringProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    switch (iMsg)
    {
    case WM_INITDIALOG:
        SetWindowTextA( hDlg, g_RequestStringCaption );
        SetWindowTextA(GetDlgItem(hDlg, IDC_EDIT_REQUEST_STRING), g_RequestStringText);
        // flash the window if it is not the focus
        flashWindow(hDlg);
        break;
    case WM_COMMAND:
        if ( LOWORD(wParam) == IDOK )
        {
            GetWindowTextA(GetDlgItem(hDlg, IDC_EDIT_INPUT_STRING), g_RequestStringInput, 512);
            EndDialog(hDlg, IDOK);
            return TRUE;
        }
        if ( LOWORD(wParam) == IDCANCEL )
        {
            EndDialog(hDlg, IDCANCEL);
            return TRUE;
        }
        break;
    case WM_DESTROY:
    case WM_CLOSE:
    case 0x00000046: // no clue what this message is, but its the one passed by EndDialog
        return TRUE;
        break;
    }

    if ( !g_RequestStringCallback )
        return FALSE;
    else
        return g_RequestStringCallback(hDlg, iMsg, wParam, lParam);
}

char *requestStringDialog(char *dialogText, char *caption )
{
    if(isGuiDisabled())
    {
        printf("Dialog boxes not supported when running with -nogui.  Returning NULL\n");
        return NULL;
    }
    strcpy( g_RequestStringText, dialogText );
    strcpy( g_RequestStringCaption, caption );
    return (IDOK == DialogBoxIndirect(winGetHInstance(), (LPDLGTEMPLATE)RequestStringDialogResource, NULL,
        DlgRequestStringProc) ) ? g_RequestStringInput : NULL;
    //return (IDOK == DialogBox(winGetHInstance(), (LPCTSTR) (IDD_DLG_REQUEST_STRING), NULL, DlgRequestStringProc)) ? g_RequestStringInput : NULL;
}

char *requestStringDialogEx(char *dialogText, char *caption, GenericDialogCallback callback_proc)
{
    char *ret;
    g_RequestStringCallback = callback_proc;
    ret = requestStringDialog(dialogText, caption);
    g_RequestStringCallback = NULL;
    return ret;
}

///////////////////////////////////////////////////////////
// END REQUEST STRING DIALOG
///////////////////////////////////////////////////////////

static IProgressDialog* dialog = NULL;
static bool progress_started = false;

bool startProgressDialog(HWND parent, wchar_t * title, wchar_t * cancel)
{
    if (dialog)
        return false;

    if(isGuiDisabled())
    {
        printf("Dialog boxes not supported when running with -nogui.\n");
        return false;
    }

    if (FAILED(CoInitialize(NULL)))
        return false;

    {
        void *createdDialog;
        if (FAILED(CoCreateInstance(&CLSID_ProgressDialog, NULL, CLSCTX_INPROC_SERVER, &IID_IProgressDialog, &createdDialog)))
            return false;
        dialog = createdDialog;
    }

    dialog->lpVtbl->SetTitle(dialog, title);

    if (cancel)
        dialog->lpVtbl->SetCancelMsg(dialog, cancel, NULL);

    dialog->lpVtbl->StartProgressDialog(dialog, parent, NULL, PROGDLG_NORMAL | PROGDLG_AUTOTIME, NULL);
    progress_started = false;
    return true;
}

bool updateProgressDialog(unsigned long long progress, unsigned long long total, wchar_t * text, wchar_t * path)
{
    if (!dialog)
        return false; // assume cant cancel dlg that doesn't exist (needed for nogui case)

    if (!progress_started) {
        dialog->lpVtbl->Timer(dialog, PDTIMER_RESET, NULL);
        progress_started = true;
    }

    dialog->lpVtbl->SetProgress64(dialog, progress, total);
    dialog->lpVtbl->SetLine(dialog, 1, text, FALSE, NULL);
    dialog->lpVtbl->SetLine(dialog, 2, path, TRUE, NULL);
    return dialog->lpVtbl->HasUserCancelled(dialog) != 0;
}

void stopProgressDialog()
{
    if (!dialog)
        return;

    dialog->lpVtbl->StopProgressDialog(dialog);
    ((IUnknownVtbl*)dialog->lpVtbl)->Release((IUnknown*)dialog);
    dialog = NULL;

    CoUninitialize();
}

#endif
