#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/StringUtil.h> // Include order required here. StringUtil.h includes no headers.

#include "prompt.h"
#include "resource.h"

static DWORD dwValue;
static char cpValue[4096];
static char *cpName;
static enum {
    PROMPT_MODE_STRING,
    PROMPT_MODE_INT,
} mode;

static INT_PTR CALLBACK GetValueDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

DWORD promptGetValue(HINSTANCE hinst, HWND hwnd, char *sName, DWORD dwDefaultValue) // Calls a dialog, and asks for sName
{
    int res;

    cpName = sName;
    dwValue = dwDefaultValue;
    mode = PROMPT_MODE_INT;

    res = DialogBoxW (hinst, MAKEINTRESOURCEW (IDD_PROMPT), hwnd, GetValueDlgProc);

    return dwValue;
}

char *promptGetString(HINSTANCE hinst, HWND hwnd, char *sName, const char *sDefaultValue) // Calls a dialog, and asks for sName
{
    int res;

    cpName = sName;
    strncpyt(cpValue, sDefaultValue, ARRAY_SIZE(cpValue));
    mode = PROMPT_MODE_STRING;

    res = DialogBoxW (hinst, MAKEINTRESOURCEW (IDD_PROMPT), hwnd, GetValueDlgProc);

    return cpValue;
}


static INT_PTR CALLBACK GetValueDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    BOOL b;
    wchar_t wbuffer[2048]={0};
    switch (iMsg) {
        case WM_INITDIALOG:
            // Set Data to what was passed to GetValue
            switch(mode) {
                case PROMPT_MODE_STRING:
                    UTF8ToWideStrConvert(cpValue, wbuffer, ARRAY_SIZE(wbuffer)-1);
                    SetDlgItemTextW(hDlg, IDC_VALUE, wbuffer);
                    SetDlgItemTextA(hDlg, IDC_NAME, cpName);
                    break;
                case PROMPT_MODE_INT:
                    SetDlgItemInt(hDlg, IDC_VALUE, dwValue, FALSE);
                    SetDlgItemTextA(hDlg, IDC_NAME, cpName);
                    break;
            }
            break;

        case WM_COMMAND:

            switch (LOWORD (wParam))
            {
            case IDOK: // Syncronize Data
                switch(mode) {
                    int length;
                    case PROMPT_MODE_STRING:
                        GetDlgItemTextW(hDlg, IDC_VALUE, wbuffer, ARRAY_SIZE(wbuffer)-1);
                        strcpy(cpValue, WideToUTF8StrTempConvert(wbuffer, &length));
                        EndDialog(hDlg, LOWORD (wParam));
                        return TRUE;
                        break;
                    case PROMPT_MODE_INT:
                        dwValue = GetDlgItemInt(hDlg, IDC_VALUE, &b, FALSE);
                        if (!b) { SetDlgItemTextA(hDlg, IDC_VALUE, "#INVALID#"); break; }
                        EndDialog(hDlg, LOWORD (wParam));
                        return TRUE;
                        break;
                }
            }
            break;

    }
    return FALSE; //DefWindowProc(hDlg, iMsg, wParam, lParam);
}

