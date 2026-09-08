/*****************************************************************************
created:    2002/05/02
copyright:  2002, NCSoft. All Rights Reserved
author(s):  Peter M. Freese

purpose:    Error dialog with stack trace and ignore capability
*****************************************************************************/

#include "../../include/arda2/core/corFirst.h"

#if CORE_SYSTEM_WINAPI & !CORE_SYSTEM_XENON

#include "../../include/arda2/util/utlStackDbg.h"
#include <winuser.h>

#include "../../include/arda2/error/errErrorHandlerDialog.h"
#include "../../include/arda2/util/utlEnumOperators.h"

#define ID_ERRDLG_MESSAGE   0x8000
#define ID_ERRDLG_OK        0x8001
#define ID_ERRDLG_DEBUG     0x8002
#define ID_ERRDLG_ABORT     0x8003
#define ID_ERRDLG_IGNORE    0x8004

#define MAXLINESIZE 4096


HWND errErrorHandlerDialog::s_RootHWND = NULL;


static BOOL CALLBACK ErrorDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM /* lParam */)
{
    switch(message)
    {
    case WM_INITDIALOG:
        return TRUE;

    case WM_DESTROY:
        return FALSE;

    case WM_COMMAND:
        switch(LOWORD(wParam))
        {
        case ID_ERRDLG_OK:
            EndDialog(hDlg, EH_Logged);
            return TRUE;

        case ID_ERRDLG_ABORT:
            EndDialog(hDlg, EH_Abort);
            return TRUE;

        case ID_ERRDLG_DEBUG:
            EndDialog(hDlg, EH_Break);
            return TRUE;

        case ID_ERRDLG_IGNORE:
            EndDialog(hDlg, EH_Ignored);
            return TRUE;
        }
    }
    return FALSE;
}

template <typename S, typename T>
inline void AlignUp( T* &p )
{
    p = (T*)(((size_t)p + sizeof(S) - 1) & ~(sizeof(S) - 1));
}

template <typename T>
inline void Write( void *& p, T v )
{
    *(T*)p = v;
    p = (void *)((size_t)p + sizeof(T));
}

enum ErrorDialogOptions
{
    ED_None             = 0,
    ED_OkayButton       = 1,
    ED_DebugButton      = 2,
    ED_AbortButton      = 4,
    ED_IgnoreAllButton  = 8,
};

MAKE_ENUM_OPERATORS(ErrorDialogOptions)

static LRESULT BuildErrorDialog(HINSTANCE hinst, HWND hwndOwner, const char *szTitle, const char *szMessage, 
                                ErrorDialogOptions options )
{
    LRESULT ret;
    int nchar;

#pragma pack(push,1)
    struct DLGTEMPLATEEX
    {
        WORD      dlgVer; 
        WORD      signature; 
        DWORD     helpID; 
        DWORD     exStyle; 
        DWORD     style; 
        WORD      cDlgItems; 
        short     x; 
        short     y; 
        short     cx; 
        short     cy; 
        //  sz_Or_Ord menu; 
        //  sz_Or_Ord windowClass; 
        //  WCHAR     title[titleLen]; 
        // The following members exist only if the style member is 
        // set to DS_SETFONT or DS_SHELLFONT.
        //  WORD     pointsize; 
        //  WORD     weight; 
        //  BYTE     italic;
        //  BYTE     charset; 
        //  WCHAR    typeface[stringLen];  
    } *lpdt;
#pragma pack(pop)

    /*
    IDD_ERRORDIALOG DIALOGEX 0, 0, 278, 105
    STYLE DS_SETFONT | DS_MODALFRAME | DS_3DLOOK | DS_FIXEDSYS | DS_CENTER | 
    WS_POPUP | WS_CAPTION | WS_SYSMENU
    CAPTION "Error"
    FONT 8, "MS Shell Dlg", 400, 0, 0x1
    BEGIN
    END
    */

    HGLOBAL hgbl = GlobalAlloc(GMEM_ZEROINIT, 16 * 1024);
    if (!hgbl)
        return -1;

    lpdt = (DLGTEMPLATEEX *)GlobalLock(hgbl);

    // Define a dialog box.
    lpdt->dlgVer = 1;
    lpdt->signature = 0xFFFF;
    lpdt->helpID = 0;
    lpdt->exStyle = 0;
    lpdt->style = DS_MODALFRAME | DS_3DLOOK | DS_CENTER | WS_POPUP | WS_CAPTION | DS_SETFOREGROUND | DS_SHELLFONT;

    lpdt->cDlgItems = 5;    // number of controls
    lpdt->x  = 0;  lpdt->y  = 0;
    lpdt->cx = 278; lpdt->cy = 105;

    void *p = (BYTE *)lpdt + sizeof(*lpdt);
    Write<WORD>(p, 0);  // no menu
    Write<WORD>(p, 0);  // predefined dialog box class (by default)
    nchar = MultiByteToWideChar(CP_ACP, 0, szTitle, -1, (LPWSTR)p, MAXLINESIZE);
    p = (WORD *)p + nchar;

    Write<WORD>(p, 8);      // point size
    Write<WORD>(p, 400);    // weight
    Write<BYTE>(p, 0);      // italic
    Write<BYTE>(p, 0x1);    // charset
    nchar = MultiByteToWideChar(CP_ACP, 0, "MS Shell Dlg",  -1, (LPWSTR)p, MAXLINESIZE);
    p = (WORD *)p + nchar;

#pragma pack(push, 1)
    struct DLGITEMTEMPLATEEX
    { 
        DWORD  helpID; 
        DWORD  exStyle; 
        DWORD  style; 
        short  x; 
        short  y; 
        short  cx; 
        short  cy; 
        DWORD   id; 
        //  sz_Or_Ord windowClass; 
        //  sz_Or_Ord title; 
        //  WORD   extraCount; 
    } *lpdit;
#pragma pack(pop)

    //-----------------------
    // DEFPUSHBUTTON   "OK",IDOK,17,84,50,14
    //-----------------------
    AlignUp<DWORD>(p);
    lpdit = (DLGITEMTEMPLATEEX *)p;
    lpdit->helpID = 0;
    lpdit->exStyle = 0;
    lpdit->style = WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON  | ((options & ED_OkayButton) ? 0 : WS_DISABLED);
    lpdit->x  = 17; lpdit->y  = 84;
    lpdit->cx = 50; lpdit->cy = 14;
    lpdit->id = ID_ERRDLG_OK;
    p = (BYTE *)p + sizeof(*lpdit);
    Write<WORD>(p, 0xFFFF);
    Write<WORD>(p, 0x0080); // button class
    nchar = MultiByteToWideChar(CP_ACP, 0, "OK", -1, (LPWSTR)p, MAXLINESIZE);
    p = (WORD *)p + nchar;
    Write<WORD>(p, 0);      // no creation data

    //-----------------------
    // PUSHBUTTON      "&Debug",IDDEBUG,82,84,50,14
    //-----------------------
    AlignUp<DWORD>(p);
    lpdit = (DLGITEMTEMPLATEEX *)p;
    lpdit->helpID = 0;
    lpdit->exStyle = 0;
    lpdit->style = WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | ((options & ED_DebugButton) ? 0 : WS_DISABLED);
    lpdit->x  = 82; lpdit->y  = 84;
    lpdit->cx = 50; lpdit->cy = 14;
    lpdit->id = ID_ERRDLG_DEBUG;        // OK button identifier
    p = (BYTE *)p + sizeof(*lpdit);
    Write<WORD>(p, 0xFFFF);
    Write<WORD>(p, 0x0080); // button class
    nchar = MultiByteToWideChar(CP_ACP, 0, "&Debug", -1, (LPWSTR)p, MAXLINESIZE);
    p = (WORD *)p + nchar;
    Write<WORD>(p, 0);      // no creation data

    //-----------------------
    // PUSHBUTTON      "&Abort",IDABORT,147,84,50,14
    //-----------------------
    AlignUp<DWORD>(p);
    lpdit = (DLGITEMTEMPLATEEX *)p;
    lpdit->helpID = 0;
    lpdit->exStyle = 0;
    lpdit->style = WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | ((options & ED_AbortButton) ? 0 : WS_DISABLED);
    lpdit->x  = 147; lpdit->y  = 84;
    lpdit->cx = 50; lpdit->cy = 14;
    lpdit->id = ID_ERRDLG_ABORT;
    p = (BYTE *)p + sizeof(*lpdit);
    Write<WORD>(p, 0xFFFF);
    Write<WORD>(p, 0x0080); // button class
    nchar = MultiByteToWideChar(CP_ACP, 0, "&Abort", -1, (LPWSTR)p, MAXLINESIZE);
    p = (WORD *)p + nchar;
    Write<WORD>(p, 0);      // no creation data

    //-----------------------
    // PUSHBUTTON      "&Ignore All",IDIGNORE,212,84,50,14
    //-----------------------
    AlignUp<DWORD>(p);
    lpdit = (DLGITEMTEMPLATEEX *)p;
    lpdit->helpID = 0;
    lpdit->exStyle = 0;
    lpdit->style = WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON  | ((options & ED_IgnoreAllButton) ? 0 : WS_DISABLED);
    lpdit->x  = 212; lpdit->y  = 84;
    lpdit->cx = 50; lpdit->cy = 14;
    lpdit->id = ID_ERRDLG_IGNORE;
    p = (BYTE *)p + sizeof(*lpdit);
    Write<WORD>(p, 0xFFFF);
    Write<WORD>(p, 0x0080); // button class
    nchar = MultiByteToWideChar(CP_ACP, 0, "&Ignore All", -1, (LPWSTR)p, MAXLINESIZE);
    p = (WORD *)p + nchar;
    Write<WORD>(p, 0);

    //-----------------------
    // EDITTEXT        IDC_MESSAGE,9,9,262,69,ES_MULTILINE | ES_READONLY | WS_VSCROLL
    //-----------------------
    AlignUp<DWORD>(p);
    lpdit = (DLGITEMTEMPLATEEX *)p;
    lpdit->helpID = 0;
    lpdit->exStyle = 0;
    lpdit->style = WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_READONLY | WS_VSCROLL | WS_BORDER;
    lpdit->x  = 9; lpdit->y  = 9;
    lpdit->cx = 262; lpdit->cy =    69;
    lpdit->id = ID_ERRDLG_MESSAGE;
    p = (BYTE *)p + sizeof(*lpdit);
    Write<WORD>(p, 0xFFFF);
    Write<WORD>(p, 0x0081); // edit box
    nchar = MultiByteToWideChar(CP_ACP, 0, szMessage, -1, (LPWSTR)p, MAXLINESIZE);
    p = (WORD *)p + nchar;
    Write<WORD>(p, 0);      // no creation data

    ret = DialogBoxIndirect(hinst, (LPDLGTEMPLATE)hgbl, hwndOwner, (DLGPROC)ErrorDlgProc); 
    GlobalUnlock(hgbl); 
    GlobalFree(hgbl); 
    return ret; 
}


//
// Error reporting dialog, Win32 version.
//
// Brings up a Windows message box, and if debugging, also prints a
// message in the debugger window.
//

errErrorHandlerDialog::errErrorHandlerDialog( HINSTANCE hInst ) :
m_hInst(hInst)
{
}


errHandlerResult errErrorHandlerDialog::Report(const char* szFileName, int iLineNumber, 
                                            errSeverity nSeverity, const char *szErrorLevel, const char* szDescription)
{
    FileLine key;
    key.m_filename = szFileName;
    key.m_line = iLineNumber;

    if ( m_ignores.find(key) != m_ignores.end() )
        return EH_Ignored;

    // mutex against multiple dialogs
    static bool bInDialog = false;

    if ( bInDialog )
        return EH_Ignored;

    bInDialog = true;

    const int nBufferSize = 4096;

    static char msg[nBufferSize];
    int nPos = 0;

    nPos += snprintf(msg, nBufferSize, "File: %s, line %d\r\n\r\n%s\r\n\r\n",
        szFileName,
        iLineNumber,
        szDescription );

#ifdef CORE_SYSTEM_WIN64
    /// @todo [tcg] stack code not working in 64-bit yet
#elif !CORE_COMPILER_GNU

    utlStackDbg::GetSingleton().BuildStackTraceStringFromAddress(3, 20, &msg[nPos], sizeof(msg) - nPos);

#endif    

    ErrorDialogOptions options = ED_None;

    if ( nSeverity < ES_Fatal )
        options |= ED_OkayButton;

    options |= ED_DebugButton;
    options |= ED_AbortButton;

    if ( nSeverity < ES_Error )
        options |= ED_IgnoreAllButton;

    LRESULT n = BuildErrorDialog(m_hInst, s_RootHWND, szErrorLevel, msg, options);

    bInDialog = false;

    if ( n >= 0)
    {
        errHandlerResult e = static_cast<errHandlerResult>(n);
        if ( e == EH_Ignored )
            m_ignores.insert(key);

        return e;
    }

    return EH_Logged;
}

#endif // CORE_SYSTEM_WINAPI & !CORE_SYSTEM_XENON
