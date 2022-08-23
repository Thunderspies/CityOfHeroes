#include "utilitieslib/utils/winutil.h"
#include "utilitieslib/utils/file.h"
#include "utilitieslib/utils/utils.h"
#include "utilitieslib/utils/log.h"

static HINSTANCE g_hInstance;
void winSetHInstance(HINSTANCE hInstance)
{
    g_hInstance = hInstance;
}

HINSTANCE winGetHInstance(void)
{
    if (!g_hInstance)
    {
        g_hInstance = GetModuleHandleA(NULL);
    }
    return g_hInstance;
}


#ifndef _XBOX
#include <direct.h>
#include "locale.h"
#include "utilitieslib/utils/ConvertUtf.h"
#include "utilitieslib/utils/utils.h"
#include "utilitieslib/components/earray.h"
#include "utilitieslib/utils/StringUtil.h"
#include "utilitieslib/utils/sysutil.h"
#include "utilitieslib/utils/error.h"
#include <stdio.h>
#include "utilitieslib/assert/assert.h"
#include "utilitieslib/utils/timing.h"
#include <tlhelp32.h>
#include "utilitieslib/utils/strings_opt.h"
#include "utilitieslib/utils/memlog.h"
#include "utilitieslib/utils/RegistryReader.h"
#include "utilitieslib/utils/osdependent.h"

static void resizeControl(HWND hDlgParent, HWND hDlg, int dx, int dy, int dw, int dh)
{
    RECT rect={0};
    POINT pos = {0,0};

    GetWindowRect(hDlg, &rect);
    pos.x = rect.left;
    pos.y = rect.top;
    ScreenToClient(hDlgParent, &pos);
    SetWindowPos(hDlg, NULL, pos.x + dx, pos.y + dy, rect.right - rect.left + dw, rect.bottom - rect.top + dh, SWP_NOZORDER);
}

typedef struct ControlList {
    unsigned int stretchx:1;
    unsigned int stretchy:1;
    unsigned int translatex:1;
    unsigned int translatey:1;
} ControlList;

static HWND parent=NULL;
static RECT alignme;
static RECT upperleft;

static BOOL CALLBACK EnumChildProc(HWND hwndChild, LPARAM lParam)
{
    RECT rect;
    ControlList flags = {0,0,0,0};
    int id;
    int minx=upperleft.left+1, miny=upperleft.top+1;
    RECT rnew;

    if (GetParent(hwndChild)!=parent) {
        return TRUE;
    }

    id = GetWindowLongA(hwndChild, GWL_ID);

    GetWindowRect(hwndChild, &rect);
    if (rect.left >= alignme.left) {
        flags.translatex = 1;
        flags.stretchx = 0;
    } else {
        if (rect.left <= upperleft.left && rect.left >= upperleft.left - 10) {
            // Left aligned
            flags.translatex = 0;
            flags.stretchx = 1;
            minx = upperleft.left;
        } else {
            flags.translatex = 0;
            flags.stretchx = 0;
        }
    }
    if (rect.top >= alignme.top) {
        flags.translatey = 1;
        flags.stretchy = 0;
    } else {
        if (rect.top <= upperleft.top && rect.top >= upperleft.top - 10) {
            // Top aligned
            miny=upperleft.top;
            flags.translatey = 0;
            flags.stretchy = 1;
        } else {
            flags.translatey = 0;
            flags.stretchy = 0;
        }
    }
    CopyRect(&rnew, &rect);
    OffsetRect(&rnew, flags.translatex*alignme.right, flags.translatey*alignme.bottom);
    rnew.right = flags.stretchx*alignme.right;
    rnew.bottom = flags.stretchy*alignme.bottom;
    if (rnew.left < minx && flags.translatex) {
        OffsetRect(&rnew, minx - rnew.left, 0);
    }
    if (rnew.top < miny && flags.translatey) {
        OffsetRect(&rnew, 0, miny - rnew.top);
    }
    rnew.right *= flags.stretchx;
    rnew.bottom *= flags.stretchy;
    resizeControl(GetParent(hwndChild), hwndChild,
        rnew.left - rect.left,
        rnew.top - rect.top,
        rnew.right,
        rnew.bottom
        );

    return TRUE;
}

typedef struct DlgResizeInfo {
    HWND hDlg;
    int lastw, lasth;
    int minw, minh;
} DlgResizeInfo;

static DlgResizeInfo **eaDRI_data=NULL;
static DlgResizeInfo ***eaDRI=&eaDRI_data;
DlgResizeInfo *getDRI(HWND hDlg)
{
    DlgResizeInfo *dri;
    int i;

    for (i=eaSize(eaDRI)-1; i>=0; i--) {
        dri = eaGet(eaDRI, i);
        if (dri->hDlg == hDlg) {
            return dri;
        }
    }
    dri = calloc(sizeof(DlgResizeInfo),1);
    eaPush(eaDRI, dri);
    dri->lastw = -1;
    dri->lasth = -1;
    dri->minh = -1;
    dri->minw = -1;
    dri->hDlg = hDlg;
    return dri;
}

void setDialogMinSize(HWND hDlg, WORD minw, WORD minh)
{
    DlgResizeInfo *dri = getDRI(hDlg);
    if (minw>0)
        dri->minw = minw;
    if (minh>0)
        dri->minh = minh;
}


void doDialogOnResize(HWND hDlg, WORD w, WORD h, int idAlignMe, int id)
{
    DlgResizeInfo *dri = getDRI(hDlg);
    int dw, dh;

    if (dri->lastw==-1) {
        dri->minw = dri->lastw = w;
        dri->minh = dri->lasth = h;
        return;
    }
    if (w < dri->minw)
        w = dri->minw;
    if (h < dri->minh)
        h = dri->minh;

    dw = w- dri->lastw, dh = h - dri->lasth;

    GetWindowRect(GetDlgItem(hDlg, idAlignMe), &alignme);
    GetWindowRect(GetDlgItem(hDlg, id), &upperleft);
    alignme.right = dw;
    alignme.bottom = dh;
    parent = hDlg;
    EnumChildWindows(hDlg, EnumChildProc, (LPARAM)NULL);
    dri->lastw = w;
    dri->lasth = h;
    RedrawWindow(hDlg, NULL, NULL, RDW_INVALIDATE | RDW_ALLCHILDREN);
}

int NumLines(LPSTR text)
{
    int ret = 1;
    while (*text)
    {
        if (*text == '\n') ret++;
        text++;
    }
    return ret;
}

int LongestWord(LPSTR text)
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
    rc.left += xdelta; rc.right += xdelta;
    rc.top += ydelta;  rc.bottom += ydelta;
    MoveWindow(hWnd, rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top, FALSE);
}

// this is the error dialog resource description
/* 'C' Style Array Dump created by HexToolbox - Hex editor - Copyright 2000-2001 Winsor Computing */
unsigned char ErrorResource[] =
    {0x01, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC8, 0x0A, 0xC8, 0x80,
    0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE4, 0x00, 0x96, 0x00, 0x00, 0x00, 0x00, 0x00, 0x43, 0x00,
    0x69, 0x00, 0x74, 0x00, 0x79, 0x00, 0x20, 0x00, 0x6F, 0x00, 0x66, 0x00, 0x20, 0x00, 0x48, 0x00,
    0x65, 0x00, 0x72, 0x00, 0x6F, 0x00, 0x65, 0x00, 0x73, 0x00, 0x20, 0x00, 0x2D, 0x00, 0x20, 0x00,
    0x45, 0x00, 0x72, 0x00, 0x72, 0x00, 0x6F, 0x00, 0x72, 0x00, 0x20, 0x00, 0x44, 0x00, 0x69, 0x00,
    0x61, 0x00, 0x6C, 0x00, 0x6F, 0x00, 0x67, 0x00, 0x00, 0x00, 0x08, 0x00, 0x90, 0x01, 0x00, 0x01,
    0x4D, 0x00, 0x53, 0x00, 0x20, 0x00, 0x53, 0x00, 0x68, 0x00, 0x65, 0x00, 0x6C, 0x00, 0x6C, 0x00,
    0x20, 0x00, 0x44, 0x00, 0x6C, 0x00, 0x67, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x50, 0x9C, 0x00, 0x7E, 0x00, 0x3D, 0x00, 0x0E, 0x00,
    0x01, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x80, 0x00, 0x4F, 0x00, 0x4B, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x50, 0x0A, 0x00, 0x7D, 0x00,
    0x49, 0x00, 0x0F, 0x00, 0x0C, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x80, 0x00, 0x43, 0x00, 0x6F, 0x00,
    0x70, 0x00, 0x79, 0x00, 0x20, 0x00, 0x74, 0x00, 0x6F, 0x00, 0x20, 0x00, 0x43, 0x00, 0x6C, 0x00,
    0x69, 0x00, 0x70, 0x00, 0x62, 0x00, 0x6F, 0x00, 0x61, 0x00, 0x72, 0x00, 0x64, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x50,
    0x0A, 0x00, 0x1C, 0x00, 0xCF, 0x00, 0x54, 0x00, 0xEB, 0x03, 0x00, 0x00, 0xFF, 0xFF, 0x82, 0x00,
    0x54, 0x00, 0x68, 0x00, 0x69, 0x00, 0x73, 0x00, 0x20, 0x00, 0x69, 0x00, 0x73, 0x00, 0x20, 0x00,
    0x61, 0x00, 0x20, 0x00, 0x64, 0x00, 0x65, 0x00, 0x73, 0x00, 0x63, 0x00, 0x72, 0x00, 0x69, 0x00,
    0x70, 0x00, 0x74, 0x00, 0x69, 0x00, 0x6F, 0x00, 0x6E, 0x00, 0x20, 0x00, 0x6F, 0x00, 0x66, 0x00,
    0x20, 0x00, 0x74, 0x00, 0x68, 0x00, 0x65, 0x00, 0x20, 0x00, 0x65, 0x00, 0x72, 0x00, 0x72, 0x00,
    0x6F, 0x00, 0x72, 0x00, 0x2E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x02, 0x50, 0x0A, 0x00, 0x08, 0x00, 0xCF, 0x00, 0x10, 0x00,
    0xEC, 0x03, 0x00, 0x00, 0xFF, 0xFF, 0x82, 0x00, 0x54, 0x00, 0x68, 0x00, 0x69, 0x00, 0x73, 0x00,
    0x20, 0x00, 0x69, 0x00, 0x73, 0x00, 0x20, 0x00, 0x4D, 0x00, 0x61, 0x00, 0x72, 0x00, 0x6B, 0x00,
    0x27, 0x00, 0x73, 0x00, 0x20, 0x00, 0x46, 0x00, 0x61, 0x00, 0x75, 0x00, 0x6C, 0x00, 0x74, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x48, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x06, 0x00,
    0xFF, 0xFF, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x10, 0x09, 0x04, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x0B, 0x00, 0x61, 0x00, 0x73, 0x00, 0x73, 0x00, 0x65, 0x00, 0x72, 0x00, 0x74, 0x00,
    0x64, 0x00, 0x6C, 0x00, 0x67, 0x00, 0x20, 0x00, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x09, 0x00, 0x41, 0x00, 0x53, 0x00, 0x53, 0x00, 0x45, 0x00, 0x52, 0x00};

#define IDC_ERRORTEXT 1003
#define IDC_FAULTTEXT 1004
#define IDC_COPYTOCLIPBOARD 12

typedef struct ErrorParams
{
    char* title;
    char* err;
    char* fault;
    int      highlight;
} ErrorParams;

char *utf8ToMbStatic(char *utf8string)
{
    wchar_t wbuffer[1024];
    static char sbuffer[1024];
    UTF8ToWideStrConvert(utf8string, wbuffer, ARRAY_SIZE(wbuffer));
    wcstombs(sbuffer, wbuffer, ARRAY_SIZE(sbuffer));
    return sbuffer;
}

// Message handler for error box.
LRESULT CALLBACK ErrorDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    static char* errorbuf;
    static HFONT bigfont = 0;
    static int highlight;
    switch (message)
    {
    case WM_INITDIALOG:
        {
            char title[1024];
            static char error[4096];

            ErrorParams* param = (ErrorParams*)lParam;
            RECT rc, rc2;
            int heightneeded, widthneeded;
            int xdelta, ydelta;

            if (!param->title) param->title = "Program Error";

            Strncpyt(title,param->title);
            utf8ToMbcs(title,ARRAY_SIZE( title ));
            SetWindowTextA(hDlg, title);

            Strncpyt(error,param->err);
             utf8ToMbcs(error, ARRAY_SIZE( error ));
            SetWindowTextA(GetDlgItem(hDlg, IDC_ERRORTEXT), error );
            errorbuf = error;

            highlight = param->highlight;
            EnableWindow(GetDlgItem(hDlg, IDCANCEL), FALSE);
            ShowCursor(1);

            // optionally show who is at fault
            if (param->fault)
            {
                LOGFONTA lf;
                memset(&lf, 0, sizeof(LOGFONTA));       // zero out structure
                lf.lfHeight = 20;                      // request a 12-pixel-height font
                //strcpy(lf.lfFaceName, "Arial");        // request a face name "Arial"
                lf.lfWeight = FW_BOLD;
                bigfont = CreateFontIndirectA(&lf);
                SetWindowTextA(GetDlgItem(hDlg, IDC_FAULTTEXT), param->fault);
                SendDlgItemMessageA(hDlg, IDC_FAULTTEXT, WM_SETFONT, (WPARAM)bigfont, 0);
            }
            else
            {
                GetWindowRect(GetDlgItem(hDlg, IDC_ERRORTEXT), &rc);
                MapWindowPoints(NULL, hDlg, (LPPOINT)&rc, 2);
                GetWindowRect(GetDlgItem(hDlg, IDC_FAULTTEXT), &rc2);
                MapWindowPoints(NULL, hDlg, (LPPOINT)&rc2, 2);
                ShowWindow(GetDlgItem(hDlg, IDC_FAULTTEXT), SW_HIDE);
                MoveWindow(GetDlgItem(hDlg, IDC_ERRORTEXT), rc.left, rc2.top, rc.right-rc.left, rc.bottom-rc2.top, FALSE);
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
            if (xdelta > 300) xdelta = 300; // cap growth
            if (ydelta > 200) ydelta = 200;
            if (xdelta < 0) xdelta = 0;
            if (ydelta < 0) ydelta = 0;
            if (xdelta || ydelta)
            {
                MapWindowPoints(NULL, hDlg, (LPPOINT)&rc, 2);
                MoveWindow(GetDlgItem(hDlg, IDC_ERRORTEXT), rc.left, rc.top, rc.right-rc.left+xdelta, rc.bottom-rc.top+ydelta, FALSE);

                // resize fault text
                GetWindowRect(GetDlgItem(hDlg, IDC_FAULTTEXT), &rc);
                MapWindowPoints(NULL, hDlg, (LPPOINT)&rc, 2);
                MoveWindow(GetDlgItem(hDlg, IDC_FAULTTEXT), rc.left, rc.top, rc.right-rc.left+xdelta, rc.bottom-rc.top, FALSE);

                // resize main window
                GetWindowRect(hDlg, &rc);
                MoveWindow(hDlg, rc.left-xdelta/2, rc.top, rc.right-rc.left+xdelta, rc.bottom-rc.top+ydelta, FALSE);

                // move buttons
                OffsetWindow(hDlg, GetDlgItem(hDlg, IDOK), xdelta/2, ydelta);
                OffsetWindow(hDlg, GetDlgItem(hDlg, IDC_COPYTOCLIPBOARD), xdelta/2, ydelta);
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
    case WM_CTLCOLORSTATIC:
        if ((HWND)lParam == GetDlgItem(hDlg, IDC_FAULTTEXT) && highlight) // turn the fault text red
        {
            SetTextColor((HDC)wParam, RGB(200, 0, 0));
            SetBkColor((HDC)wParam, GetSysColor(COLOR_BTNFACE));
            return (LRESULT)GetSysColorBrush(COLOR_BTNFACE);
        }
        break;
    }
    return FALSE;
}

static void writeStderrAndAbort(const char * fmt, ...)
{
    va_list va;

    va_start(va, fmt);
    fflush(fileWrap(stdout));
    printv_stderr(fmt, va);
    fflush(fileWrap(stderr));
    va_end(va);

    abort();

    // Just in case...
    exit(3);
}

void errorDialog(HWND hwnd, char *str, char* title, char* fault, int highlight) // title & fault optional
{
    ErrorParams params = {0};

     if(isGuiDisabled()) {
         writeStderrAndAbort("errorDialog: %s %s %s\n", title, str, fault);
         return;
     }

    // Hack for holding Shift to ignore all pop-ups.  Shhh... don't tell anyone.
    if (GetAsyncKeyState(VK_SHIFT) & 0x8000000 || errorGetVerboseLevel()==2) {
        return;
    }

    if (IsUsingCider())
    {
        MessageBoxA(NULL, str, title, MB_OK);
        return;
    }

    if (hwnd)
        ShowWindow(hwnd, SW_SHOW);

    params.title = title;
    params.err = str;
    params.fault = fault;
    params.highlight = highlight;
    DialogBoxIndirectParamA(winGetHInstance(), (LPDLGTEMPLATEA)ErrorResource, hwnd, ErrorDlg, (LPARAM)&params);
}

void msgAlert(HWND hwnd, char *str)
{
    ErrorParams params = {0};

     if(isGuiDisabled()) {
         writeStderrAndAbort("msgAlert: %s\n", str);
         return;
     }

    // Hack for holding Shift to ignore all pop-ups.  Shhh... don't tell anyone.
    if (GetAsyncKeyState(VK_SHIFT) & 0x8000000) {
        printf("msgAlert: %s\n", str);
        return;
    }

    if (IsUsingCider())
    {
        MessageBoxA(NULL, str, "Alert", MB_OK);
        return;
    }

    if (hwnd)
        ShowWindow(hwnd, SW_SHOW);

    params.err = str;
    DialogBoxIndirectParamA(winGetHInstance(), (LPDLGTEMPLATEA)ErrorResource, hwnd, ErrorDlg, (LPARAM)&params);
}

/// TODO: Render the letter with a black outlined font.
///       Call TextOut twice, one with the font set to FW_HEAVY and another with the font set to FW_NORMAL? How would that look at 16x16 anyways?
/// TODO: Add a conditional statement to pick between a plain black background or a proper monochromatic mask.
HICON getIconColoredLetter(wchar_t letter, U32 colorRGB, U32 sizeX, U32 sizeY) {
    // Check if GUI support is intended, skip wasting the processing power otherwise.
    if (isGuiDisabled()) {
        return NULL;
    }

    /// Set up our canvas.
    // Create a device context compatible with all active monitors.
    HDC hDC = CreateDCA("DISPLAY", NULL, NULL, NULL);
    // Create a device context in memory following the attributes from `hDC`. `memDC` is essentially a monochromatic 1x1 context until a bitmap is selected into it.
    HDC memDC = CreateCompatibleDC(hDC);
    // Create a bitmap to select the new working area into `memDC`.
    HBITMAP hBitmapColor = CreateCompatibleBitmap(hDC, sizeX, sizeY); // `hDC` must be used, `memDC` does not have the required attributes at this point.
    // We can de-reference `hDC` now, all work is done via `memDC` now.
    DeleteDC(hDC);
    // Set `memDC`'s working area to `sizeX` * `sizeY`.
    SelectObject(memDC, hBitmapColor);

    /// Write to our canvas.
    // Find the nearest compatible font based on these attributes.
    HFONT hFont = CreateFontW(sizeY, 0, 0, 0, FW_HEAVY, FALSE, FALSE, FALSE, ANSI_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_NATURAL_QUALITY, VARIABLE_PITCH, L"Segoe UI");
    // Set `memDC`'s default font.
    SelectObject(memDC, hFont);
    // Apply `colorRGB` before writing `letter` to `memDC`.
    SetTextColor(memDC, RGB((colorRGB >> 16) & 0xFF, (colorRGB >> 8) & 0xFF, colorRGB & 0xFF)); // `colorRGB` would be read as BGR without this conversion.
    // Explicitly say to leave the background untouched before any draw operations.
    SetBkMode(memDC, TRANSPARENT);
    // Get the size of `letter` so it can be positioned in the center.
    SIZE size = { 0 };
    GetTextExtentPoint32W(memDC, &letter, 1, &size);
    // Write `letter` to the center of the working area.
    TextOutW(memDC, (sizeX / 2) - (size.cx / 2), (sizeY / 2) - (size.cy / 2), &letter, 1);
    // We no longer need the font, let's free a little memory.
    DeleteObject(hFont);
    // Same with `memDC`. `hBitmapColor` stores all of our work that was just done.
    DeleteDC(memDC);

    /// Generate an icon based on our canvas' data.
    // Set up the internal icon structure.
    ICONINFO info = { 0 };
    info.fIcon = TRUE; // This means our ICONINFO structure describes an icon, not a cursor.
    info.hbmColor = hBitmapColor;

    HICON hIcon = NULL;
    U32 *buffer = calloc(sizeX * sizeY, sizeof(U32)); // The stack potentially isn't large enough, allocate on the heap.
    // Create a monochromatic bitmap mask.
    for (int i = 0; i < 5; ++i)
    {
        // I've seen CreateBitmap fail inconsistently for no good reason.
        // Calling CreateBitmap in this loop ensures that by the time we go to generate the icon, a valid mask is in place.
        info.hbmMask = CreateBitmap(sizeX, sizeY, 4, 8, buffer);
        if (info.hbmMask != NULL)
        {
            // It's finally time! Create an icon using our two bitmaps.
            hIcon = CreateIconIndirect(&info);
            DeleteObject(info.hbmMask);
            break;
        }
    }
    free(buffer); // We're guests. Might as well clean up after ourselves.
    // No longer referencing these bitmaps, return a little more memory to the system before we exit.
    DeleteObject(hBitmapColor);

    // By this point `hIcon` is a valid `sizeX` * `sizeY` icon.
    return hIcon;
}

void setWindowIconColoredLetter(HWND hwnd, wchar_t letter, U32 colorRGB) {
    // This icon is used by various OS interfaces
    HICON hIcon = getIconColoredLetter(letter, colorRGB, 64, 64);
    if (hIcon) {
        SendMessageW(hwnd, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)hIcon);
        DeleteObject(hIcon);
    }

    // This icon is used by the application directly and appears in the corner of the console
    HICON hSmallIcon = getIconColoredLetter(letter, colorRGB, 16, 16);
    if (hSmallIcon) {
        SendMessageW(hwnd, WM_SETICON, (WPARAM)ICON_SMALL, (LPARAM)hSmallIcon);
        DeleteObject(hSmallIcon);
    }
}

#include "utilitieslib/UtilsNew/Array.h"
char* getIconColoredLetterBytes(int letter, U32 colorRGB)
{
    // I admit this is horrendous on many levels
#pragma pack(push, 1)
    typedef struct __declspec(align(1)) SingleIconHeader
    {
        U16 reserved1;
        U16 type;
        U16 count;
        U8 width;
        U8 height;
        U8 colorcount;
        U8 reserved2;
        U16 planes;
        U16 bpp;
        U32 size;
        U32 offset;
    } SingleIconHeader;
#pragma pack(pop)


    char *data = NULL;
    SingleIconHeader *icondata;
    BITMAPINFOHEADER *header;
    struct {
        BITMAPINFOHEADER h;
        RGBQUAD p[256];
    } getbitsinfo = {0};
    int colorsize, masksize;

    HDC hdc = CreateDCA("DISPLAY", NULL, NULL, NULL);
    HICON icon = getIconColoredLetter(letter, colorRGB, 16, 16);
    ICONINFO iconinfo;
    GetIconInfo(icon, &iconinfo);

    colorsize = 16 * ((16 * 24 + 31) & ~31) / 8; // 32bit aligned
    masksize = 16 * ((16 + 31) & ~31) / 8; // 32bit aligned

    icondata = (SingleIconHeader*)achr_pushn(&data, sizeof(SingleIconHeader));
    icondata->type = 1; // icon
    icondata->count = 1;
    icondata->width = 16;
    icondata->height = 16;
    icondata->planes = 1;
    icondata->bpp = 24;
    icondata->size = sizeof(BITMAPINFOHEADER) + colorsize + masksize;
    icondata->offset = sizeof(SingleIconHeader);

    header = (BITMAPINFOHEADER*)achr_pushn(&data, sizeof(BITMAPINFOHEADER));
    header->biSize = sizeof(BITMAPINFOHEADER);
    header->biWidth = 16;
    header->biHeight = 16 * 2; // combined bitmap height
    header->biPlanes = 1;
    header->biBitCount = 24;

    getbitsinfo.h.biSize = sizeof(BITMAPINFOHEADER);
    getbitsinfo.h.biWidth = 16;
    getbitsinfo.h.biHeight = 16;
    getbitsinfo.h.biPlanes = 1;
    getbitsinfo.h.biCompression = BI_RGB;

    getbitsinfo.h.biBitCount = 24;
    GetDIBits(hdc, iconinfo.hbmColor, 0, 16, (void*)achr_pushn(&data, colorsize), (LPBITMAPINFO)&getbitsinfo, DIB_RGB_COLORS);

    getbitsinfo.h.biBitCount = 1;
    GetDIBits(hdc, iconinfo.hbmMask, 0, 16, (void*)achr_pushn(&data, masksize), (LPBITMAPINFO)&getbitsinfo, DIB_RGB_COLORS);

    DeleteObject(iconinfo.hbmMask);
    DeleteObject(iconinfo.hbmColor);
    DestroyIcon(icon);
    DeleteDC(hdc);

    return data;
}

void winRegisterMe(const char *command, const char *extension) // Registers the current executable to handle files of the given extension
{
    char prog[MAX_PATH];
    char classname[MAX_PATH];
    RegReader reader;
    char key[MAX_PATH];
    char openstring[MAX_PATH];
    assert(extension[0]=='.');
    sprintf_s(SAFESTR(classname), "%s_auto_file", extension+1);
    Strcpy(prog,getExecutableName());
    backSlashes(prog);

    reader = createRegReader();
    sprintf_s(SAFESTR(key), "HKEY_CLASSES_ROOT\\%s", extension);
    initRegReader(reader, key);
    rrWriteString(reader, "", classname);
    destroyRegReader(reader);

    reader = createRegReader();
    sprintf_s(SAFESTR(key), "HKEY_CLASSES_ROOT\\%s\\shell\\%s\\command", classname, command);
    sprintf_s(SAFESTR(openstring), "\"%s\" \"%%1\"", prog);
    initRegReader(reader, key);
    rrWriteString(reader, "", openstring);
    destroyRegReader(reader);

}

char *winGetFileName_s(HWND hwnd, const char *fileMask, char *fileName, size_t fileName_size, bool save)
{
    OPENFILENAMEA theFileInfo;
    //char filterStrs[256];
    int        ret;
    char    base[_MAX_PATH];

    _getcwd(base,_MAX_PATH);
    memset(&theFileInfo,0,sizeof(theFileInfo));
    theFileInfo.lStructSize = sizeof(OPENFILENAME);
    theFileInfo.hwndOwner = hwnd;
    theFileInfo.hInstance = NULL;
    theFileInfo.lpstrFilter = fileMask;
    theFileInfo.lpstrCustomFilter = NULL;
    backSlashes(fileName);
    if (strEndsWith(fileName, "\\")) {
        fileName[strlen(fileName)-1] = '\0';
    }
    theFileInfo.lpstrFile = fileName;
    theFileInfo.nMaxFile = 255;
    theFileInfo.lpstrFileTitle = NULL;
    theFileInfo.lpstrInitialDir = NULL;
    theFileInfo.lpstrTitle = NULL;
    theFileInfo.Flags = OFN_LONGNAMES | OFN_OVERWRITEPROMPT;// | OFN_PATHMUSTEXIST;
    theFileInfo.lpstrDefExt = NULL;

    if (save)
        ret = GetSaveFileNameA(&theFileInfo);
    else
        ret = GetOpenFileNameA(&theFileInfo);
    _chdir(base);

    if (ret)
        return fileName;
    else
        return NULL;
}

bool winExistsInRegPath(const char *path)
{
    char key[2048];
    char path_local[MAX_PATH];
    char oldpath[4096];
    RegReader rr = createRegReader();
    strcpy(path_local, path);
    backSlashes(path_local);    sprintf(key, "%s\\Environment", "HKEY_CURRENT_USER");
    initRegReader(rr, key);
    if (!rrReadString(rr, "PATH", SAFESTR(oldpath)))
        strcpy(oldpath, "");
    backSlashes(oldpath);

    return !!strstri(oldpath, path_local);
}

bool winExistsInEnvPath(const char *path)
{
    char *path_env;
    char path_local[MAX_PATH];
    strcpy(path_local, path);
    backSlashes(path_local);
    _dupenv_s(&path_env, NULL, "PATH");
    backSlashes(path_env);

    return !!strstri(path_env, path_local);
}


void winAddToPath(const char *path, int prefix)
{
    char key[2048];
    char oldpath[4096];
    char oldpath_orig[4096];
    char newpath[4096]="";
    char path_local[MAX_PATH];
    char *last;
    char *str;
    RegReader rr = createRegReader();
    bool failedToRead=false;
    bool foundItAlready=false;
    strcpy(path_local, path);
    backSlashes(path_local);
    if (strEndsWith(path_local, "\\"))
        path_local[strlen(path_local)-1]='\0';
    sprintf(key, "%s\\Environment", "HKEY_CURRENT_USER");
    initRegReader(rr, key);
    if (!rrReadString(rr, "PATH", SAFESTR(oldpath))) {
        strcpy(oldpath, "");
        failedToRead = true;
    }
    strcpy(oldpath_orig, oldpath);

    if (prefix) {
        strcpy(newpath, path_local);
        strcat(newpath, ";");
    }
    str = strtok_r(oldpath, ";", &last);
    while (str) {
        backSlashes(str);
        if (strEndsWith(str, "\\"))
            str[strlen(str)-1]='\0';
        if (stricmp(path_local, str)==0) {
            // skip it
            foundItAlready = true;
        } else {
            strcat(newpath, str);
            strcat(newpath, ";");
        }
        str = strtok_r(NULL, ";", &last);
    }
    if (!prefix) {
        strcat(newpath, path_local);
        strcat(newpath, ";");
    }
    if (!foundItAlready && stricmp(newpath, oldpath_orig)!=0)
    {
        printf("Adding \"%s\" to system path.\n", path_local);
        rrWriteString(rr, "PATH", newpath);
        destroyRegReader(rr);
        {
            DWORD_PTR dw;
            SendMessageTimeoutA(HWND_BROADCAST, WM_WININICHANGE, 0, (LPARAM)"Environment", SMTO_NORMAL, 5000, &dw);
        }
    } else {
        destroyRegReader(rr);
    }
}



//////////////////////////////////////////////////////////////////////////

#else

#include "wininclude.h"
#include "StringUtil.h"
#include "WorkerThread.h"
#include "RegistryReader.h"

typedef struct DlgData
{
    WCHAR wtitle[1024];
    WCHAR wstr[1024];
    DWORD flags;
} DlgData;

static WorkerThread *dialog_thread = 0;

static void dispatchdialog(void *user_data, int type, void *data)
{
    DlgData *dlg = (DlgData *)data;
    LPCWSTR OK = L"OK";
    XOVERLAPPED overlapped = {0};
    MESSAGEBOX_RESULT mbresult = {0};

    // this is non blocking, and the render thread must keep going for the message box to display
    XShowMessageBoxUI(0,dlg->wtitle,dlg->wstr,1,&OK,0,dlg->flags,&mbresult,&overlapped);
    mbresult.dwButtonPressed=10000;
    Sleep(500);
    while (mbresult.dwButtonPressed==10000)
        Sleep(5);

    if (dialog_thread)
        wtQueueMsg(dialog_thread, WT_CMD_USER_START, 0, 0);
}

static void dispatchdialogdone(void *user_data, int type, void *data)
{
    // ok has been pressed, do something with it?
}

static void showxdialog(DlgData *data)
{
#if 0
    dispatchdialog(0, 0, data);
#else
    if (!dialog_thread)
    {
        dialog_thread = wtCreate(dispatchdialog, 1<<20, dispatchdialogdone, 1<<10, NULL);
        wtSetThreaded(dialog_thread, 1);
        wtStart(dialog_thread);
    }

    wtQueueCmd(dialog_thread, WT_CMD_USER_START, data, sizeof(DlgData));
#endif
}

void errorDialog(HWND hwnd, char *str, char* title, char* fault, int highlight) // title & fault optional
{
    DlgData data;

    UTF8ToWideStrConvert(title?title:"", data.wtitle, ARRAY_SIZE_CHECKED(data.wtitle));
    UTF8ToWideStrConvert(str?str:"", data.wstr, ARRAY_SIZE_CHECKED(data.wstr));
    data.flags = XMB_ERRORICON;
    showxdialog(&data);
}

void msgAlert(HWND hwnd, char *str)
{
    DlgData data;

    UTF8ToWideStrConvert("Program Error", data.wtitle, ARRAY_SIZE_CHECKED(data.wtitle));
    UTF8ToWideStrConvert(str, data.wstr, ARRAY_SIZE_CHECKED(data.wstr));
    data.flags = XMB_ALERTICON;
    showxdialog(&data);
}


char *winGetFileName_s(HWND hwnd, const char *fileMask, char *fileName, size_t fileName_size, bool save)
{
    return NULL;
}

void winAddToPath(const char *path, int prefix)
{
}
bool winExistsInRegPath(const char *path)
{
    return true;
}

bool winExistsInEnvPath(const char *path)
{
    return true;
}
#endif // else defined(XBOX)

char *winGetLastErrorStr()
{
    static char cBuf[1024];
    sprintf(cBuf, "%i:",GetLastError());
    FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), 0, cBuf+strlen(cBuf), 1000, NULL);
    return cBuf;
}

bool winCreateProcess(char *command_line, PROCESS_INFORMATION *res_pi)
{
    STARTUPINFOA si = {sizeof(si)};
    if(!res_pi)
        return false;
    ZeroStruct( res_pi );
    return CreateProcessA(NULL,command_line,NULL,NULL,false,0,NULL,NULL,&si,res_pi);
}

bool winProcessRunning(PROCESS_INFORMATION *pi)
{
    return pi && WaitForSingleObject(pi->hProcess, 0); // could be used to wait for exit
}

bool winProcessExitCode(PROCESS_INFORMATION *pi, U32 *res_exit)
{
    return res_exit && pi && GetExitCodeProcess(pi->hProcess,res_exit);
}
