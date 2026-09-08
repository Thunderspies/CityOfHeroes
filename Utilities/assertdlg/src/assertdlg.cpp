// assertdlg.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "assertdlg.h"
#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                     // current instance
TCHAR szTitle[MAX_LOADSTRING];       // The title bar text
TCHAR szWindowClass[MAX_LOADSTRING]; // the main window class name

// Forward declarations of functions included in this code module:
ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND HWND, UINT UINT, WPARAM WPARAM, LPARAM LPARAM);

TCHAR errortitle[] = "Program Error";

TCHAR errorbuf[] = "Error: we did something crazy!  Line 328, etc.";

TCHAR errorbuflongstr[] = "A huge fucking filename: 'C:/GAME/DATA/SOMEWHERE/ELSE/LEVEL1/MISSIONS/LONGMISSIONS/VERYLONGMISSIONS/1/VERYLONGMISSIONS_1.TXT\n"
                          "And some other text...";

TCHAR errorbufwide[] = "Error: we did something crazy!  Line 328, etc.  This is an incredibly long error message too.. it's going to stretch a far, far way";

TCHAR errorbuflong[] = "Error: we did something crazy!  Line 328, etc.\n"
                       "This is a really long error message...\n"
                       "This is a really long error message...\n"
                       "This is a really long error message...\n"
                       "This is a really long error message...\n"
                       "This is a really long error message...\n"
                       "This is a really long error message...\n"
                       "This is a really long error message...\n"
                       "This is a really long error message...\n"
                       "This is a really long error message...\n"
                       "This is a really long error message...\n"
                       "This is a really long error message...\n"
                       "This is a really long error message...\n"
                       "This is a really long error message...\n"
                       "This is a really long error message...\n"
                       "This is a really long error message...\n"
                       "This is a really long error message...\n"
                       "This is a really long error message...\n"
                       "This is a really long error message...\n"
                       "This is a really long error message...\n"
                       "LAST LINE...\n";

TCHAR assertbuf[] = "Assertion failed!\n"
                    "\n"
                    "Program: c:/util/mapserver.exe\n"
                    "Process ID: 3028\n"
                    "File: c:/src/mapserver/svr/svr_init.c\n"
                    "Line: 483\n"
                    "\n"
                    "Expression: 0\n"
                    "0 main\t\tLine: c:/src/mapserver/svr/svr_init.c(483)\n"
                    "1 mainCrtStartup..\n"
                    "2 etc\n"
                    "3 etc\n"
                    "4 etc\n"
                    "5 etc\n"
                    "6 etc\n"
                    "7 etc\n"
                    "8 etc\n";

#define IDC_MAINTEXT 10
#define IDC_RUNDEBUGGER 11
#define IDC_COPYTOCLIPBOARD 12
#define IDC_STOP 0
#define IDC_DEBUG 1
#define IDC_IGNORE 3

int NumLines(LPTSTR text)
{
    int ret = 1;
    while (*text)
    {
        if (*text == '\n')
            ret++;
        text++;
    }
    return ret;
}

// this isn't actually correct, but I'm lazy
int LongestWord(LPTSTR text)
{
    int length, longest = 0;
    int beforeword = -1;
    int len = (int)strlen(text);
    int i;
    for (i = 0; i < len; i++)
    {
        if (text[i] == ' ')
        {
            length = i - beforeword - 1;
            if (length > longest)
                longest = length;
            beforeword = i;
        }
    }
    length = i - beforeword - 1;
    if (length > longest)
        longest = length;
    return longest;
}

void OffsetWindow(HWND hDlg, HWND hWnd, int xdelta, int ydelta)
{
    RECT rc;
    GetWindowRect(hWnd, &rc);
    MapWindowPoints(NULL, hDlg, (LPPOINT)&rc, 2);
    rc.left += xdelta;
    rc.right += xdelta;
    rc.top += ydelta;
    rc.bottom += ydelta;
    MoveWindow(hWnd, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, FALSE);
}

void winCopyToClipboard(char* error)
{
    MessageBox(NULL, "Copied to clipboard", "Copy", MB_SETFOREGROUND);
}

typedef struct ErrorParams
{
    char* title;
    char* err;
    char* fault;
} ErrorParams;

// Message handler for error box.
INT_PTR CALLBACK ErrorDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    static char* errorbuf;
    static HFONT bigfont = 0;
    switch (message)
    {
        case WM_INITDIALOG:
        {
            ErrorParams* param = (ErrorParams*)lParam;
            RECT rc, rc2;
            int heightneeded, widthneeded;
            int xdelta, ydelta;

            if (!param->title)
                param->title = const_cast<char*>("Error Dialog");
            SetWindowText(hDlg, param->title);
            errorbuf = param->err;
            SetWindowText(GetDlgItem(hDlg, IDC_ERRORTEXT), param->err);
            EnableWindow(GetDlgItem(hDlg, IDCANCEL), FALSE);
            ShowCursor(1);

            // optionally show who is at fault
            if (param->fault)
            {
                LOGFONT lf;
                memset(&lf, 0, sizeof(LOGFONT)); // zero out structure
                lf.lfHeight = 20;                // request a 12-pixel-height font
                // strcpy(lf.lfFaceName, "Arial");        // request a face name "Arial"
                lf.lfWeight = FW_BOLD;
                bigfont = CreateFontIndirect(&lf);
                SetWindowText(GetDlgItem(hDlg, IDC_FAULTTEXT), param->fault);
                SendDlgItemMessage(hDlg, IDC_FAULTTEXT, WM_SETFONT, (WPARAM)bigfont, 0);
            }
            else
            {
                GetWindowRect(GetDlgItem(hDlg, IDC_ERRORTEXT), &rc);
                MapWindowPoints(NULL, hDlg, (LPPOINT)&rc, 2);
                GetWindowRect(GetDlgItem(hDlg, IDC_FAULTTEXT), &rc2);
                MapWindowPoints(NULL, hDlg, (LPPOINT)&rc2, 2);
                ShowWindow(GetDlgItem(hDlg, IDC_FAULTTEXT), SW_HIDE);
                MoveWindow(GetDlgItem(hDlg, IDC_ERRORTEXT), rc.left, rc2.top, rc.right - rc.left, rc.bottom - rc2.top, FALSE);
            }

            // figure out the height and width needed for text - this
            // is incorrect for a scaled font system, and the width is just
            // an approximation
            heightneeded = (int)(((float)12.5) * NumLines(param->err));
            widthneeded = (int)(((float)7) * LongestWord(param->err));

            // resize to correct height
            GetWindowRect(GetDlgItem(hDlg, IDC_ERRORTEXT), &rc);
            xdelta = 6 + widthneeded - rc.right + rc.left;
            ydelta = 6 + heightneeded - rc.bottom + rc.top;
            if (xdelta > 300)
                xdelta = 300; // cap growth
            if (ydelta > 200)
                ydelta = 200;
            if (xdelta < 0)
                xdelta = 0;
            if (ydelta < 0)
                ydelta = 0;
            if (xdelta || ydelta)
            {
                MapWindowPoints(NULL, hDlg, (LPPOINT)&rc, 2);
                MoveWindow(GetDlgItem(hDlg, IDC_ERRORTEXT), rc.left, rc.top, rc.right - rc.left + xdelta, rc.bottom - rc.top + ydelta, FALSE);

                // resize fault text
                GetWindowRect(GetDlgItem(hDlg, IDC_FAULTTEXT), &rc);
                MapWindowPoints(NULL, hDlg, (LPPOINT)&rc, 2);
                MoveWindow(GetDlgItem(hDlg, IDC_FAULTTEXT), rc.left, rc.top, rc.right - rc.left + xdelta, rc.bottom - rc.top, FALSE);

                // resize main window
                GetWindowRect(hDlg, &rc);
                MoveWindow(hDlg, rc.left - xdelta / 2, rc.top, rc.right - rc.left + xdelta, rc.bottom - rc.top + ydelta, FALSE);

                // move buttons
                OffsetWindow(hDlg, GetDlgItem(hDlg, IDOK), xdelta / 2, ydelta);
                OffsetWindow(hDlg, GetDlgItem(hDlg, IDC_COPYTOCLIPBOARD), xdelta / 2, ydelta);
            }

            return FALSE;
        }
        case WM_COMMAND:
            if (LOWORD(wParam) == IDOK)
            {
                EndDialog(hDlg, 0);
                ShowCursor(0);
                if (bigfont)
                {
                    DeleteObject(bigfont);
                    bigfont = 0;
                }
                return TRUE;
            }
            else if (LOWORD(wParam) == IDC_COPYTOCLIPBOARD)
            {
                winCopyToClipboard(errorbuf);
                return TRUE;
            }
            break;
    }
    return FALSE;
}

// Message handler for about box.
INT_PTR CALLBACK Assert(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_INITDIALOG:
        {
            HFONT hf;
            LOGFONT lf;
            RECT rc;
            int heightneeded;
            int delta;

            SetTimer(hDlg, 0, 500, NULL);
            ShowCursor(1);

            // figure out the height and width needed for text
            memset(&lf, 0, sizeof(LOGFONT));
            lf.lfHeight = 14;
            lf.lfWeight = FW_NORMAL;
            hf = CreateFontIndirect(&lf);
            heightneeded = (14) * NumLines(assertbuf);

            // resize text box
            GetWindowRect(GetDlgItem(hDlg, IDC_MAINTEXT), &rc);
            delta = 6 + heightneeded - rc.bottom + rc.top;
            if (delta > 100)
                delta = 100; // cap growth
            MapWindowPoints(NULL, hDlg, (LPPOINT)&rc, 2);
            MoveWindow(GetDlgItem(hDlg, IDC_MAINTEXT), rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top + delta, FALSE);

            // resize main window
            GetWindowRect(hDlg, &rc);
            MoveWindow(hDlg, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top + delta, FALSE);

            // move buttons
            OffsetWindow(hDlg, GetDlgItem(hDlg, IDC_STOP), 0, delta);
            OffsetWindow(hDlg, GetDlgItem(hDlg, IDC_DEBUG), 0, delta);
            OffsetWindow(hDlg, GetDlgItem(hDlg, IDC_IGNORE), 0, delta);
            OffsetWindow(hDlg, GetDlgItem(hDlg, IDC_RUNDEBUGGER), 0, delta);
            OffsetWindow(hDlg, GetDlgItem(hDlg, IDC_COPYTOCLIPBOARD), 0, delta);

            // show what I'm doing..
            // TCHAR buf[2000];
            // wsprintf(buf, "Need to be %i high, delta %i", heightneeded, delta);
            // MessageBox(hDlg, buf, "debug", 0);

            // set text
            SendDlgItemMessage(hDlg, IDC_MAINTEXT, WM_SETFONT, (WPARAM)hf, 0);
            SetWindowText(GetDlgItem(hDlg, IDC_MAINTEXT), assertbuf);

            // disable other buttons for a bit
            EnableWindow(GetDlgItem(hDlg, IDC_STOP), FALSE);
            EnableWindow(GetDlgItem(hDlg, IDC_DEBUG), FALSE);
            EnableWindow(GetDlgItem(hDlg, IDC_IGNORE), FALSE);
            EnableWindow(GetDlgItem(hDlg, IDC_RUNDEBUGGER), FALSE);
            EnableWindow(GetDlgItem(hDlg, IDC_COPYTOCLIPBOARD), FALSE);
            EnableWindow(GetDlgItem(hDlg, IDCANCEL), FALSE);

            return FALSE;
        }

        case WM_TIMER:
            EnableWindow(GetDlgItem(hDlg, IDC_STOP), TRUE);
            EnableWindow(GetDlgItem(hDlg, IDC_DEBUG), TRUE);
            EnableWindow(GetDlgItem(hDlg, IDC_IGNORE), TRUE);
            EnableWindow(GetDlgItem(hDlg, IDC_RUNDEBUGGER), TRUE);
            EnableWindow(GetDlgItem(hDlg, IDC_COPYTOCLIPBOARD), TRUE);
            SetFocus(GetDlgItem(hDlg, IDC_DEBUG));
            break;

        case WM_COMMAND:
            if (LOWORD(wParam) == IDC_RUNDEBUGGER)
            {
                PROCESS_INFORMATION pi;
                STARTUPINFO si;
                TCHAR buf[1000];
                TCHAR debuggerpath[MAX_PATH] = "\"C:/Program Files/Microsoft Visual Studio .NET/Common7/Packages/Debugger/msvcmon.exe\"";
                memset(&si, 0, sizeof(si));
                si.cb = sizeof(si);
                memset(&pi, 0, sizeof(pi));

                if (!CreateProcess(NULL, debuggerpath, NULL, NULL, 0, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi))
                {
                    FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), 0, buf, 2000, NULL);
                    MessageBox(hDlg, buf, "Error running debugger", MB_SETFOREGROUND);
                }
            }
            else if (LOWORD(wParam) == IDC_STOP || LOWORD(wParam) == IDC_DEBUG || LOWORD(wParam) == IDC_IGNORE)
            {
                EndDialog(hDlg, LOWORD(wParam));
                ShowCursor(0);
                return TRUE;
            }
            break;
    }
    return FALSE;
}

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
    TCHAR buf[2000];
    int ret;
    ErrorParams params;

    params.title = errortitle;
    params.err = errorbuf;
    params.fault = 0;
    ret = (int)DialogBoxParam(hInst, (LPCTSTR)IDD_ERROR, NULL, ErrorDlg, (LPARAM)&params);
    params.title = const_cast<char*>("errorbufwide");
    params.err = errorbufwide;
    params.fault = const_cast<char*>("It is Mark's Fault");
    ShowCursor(0);
    ret = (int)DialogBoxParam(hInst, (LPCTSTR)IDD_ERROR, NULL, ErrorDlg, (LPARAM)&params);
    MessageBox(NULL, "Don't have cursor", "Cursor test", MB_SETFOREGROUND);
    ShowCursor(1);
    MessageBox(NULL, "Have cursor", "Cursor test", MB_SETFOREGROUND);
    params.title = const_cast<char*>("errorbuflongstr");
    params.err = errorbuflongstr;
    params.fault = const_cast<char*>("It is Somebody's Fault");
    ret = (int)DialogBoxParam(hInst, (LPCTSTR)IDD_ERROR, NULL, ErrorDlg, (LPARAM)&params);
    params.title = const_cast<char*>("errorbuflong");
    params.err = errorbuflong;
    params.fault = const_cast<char*>("It is really Mark's Fault");
    ShowCursor(0);
    MessageBox(NULL, "Don't have cursor", "Cursor test", MB_SETFOREGROUND);
    ret = (int)DialogBoxParam(hInst, (LPCTSTR)IDD_ERROR, NULL, ErrorDlg, (LPARAM)&params);
    ret = (int)DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, NULL, Assert);
    wsprintf(buf, "Dialog returned %i", ret);
    MessageBox(NULL, "Don't have cursor", "Cursor test", MB_SETFOREGROUND);
    MessageBox(NULL, buf, "Assert dialog", MB_SETFOREGROUND);
    ShowCursor(1);
    MessageBox(NULL, "Should have cursor again", "Cursor test", MB_SETFOREGROUND);
    return 0;

    // TODO: Place code here.
    MSG msg;
    HACCEL hAccelTable;

    // Initialize global strings
    LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadString(hInstance, IDC_ASSERTDLG, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_ASSERTDLG);

    // Main message loop:
    while (GetMessage(&msg, NULL, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = NULL;
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = (LPCTSTR)IDC_ASSERTDLG;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

    return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HANDLE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    HWND hWnd;

    hInst = hInstance; // Store instance handle in our global variable

    hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

//
//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND    - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY    - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    int wmId, wmEvent;
    PAINTSTRUCT ps;
    HDC hdc;

    switch (message)
    {
        case WM_COMMAND:
            wmId = LOWORD(wParam);
            wmEvent = HIWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
                case IDM_ABOUT:
                    DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, Assert);
                    break;
                case IDM_EXIT:
                    DestroyWindow(hWnd);
                    break;
                default:
                    return DefWindowProc(hWnd, message, wParam, lParam);
            }
            break;
        case WM_PAINT:
            hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code here...
            EndPaint(hWnd, &ps);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
