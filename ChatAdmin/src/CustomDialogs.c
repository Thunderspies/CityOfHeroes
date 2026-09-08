#include <utilitieslib/utils/wininclude.h>
#include "CustomDialogs.h"
#include "ChatAdmin.h"
#include "resource.h"
#include <utilitieslib/utils/timing.h>
#include "ChatAdminUtils.h"

static INT_PTR CALLBACK GetStringDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK GetYesNoDlgProc (HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);


void freeData(void * data)
{
    if(data)
        free(data);
}


void CreateDialogYesNo(HWND hWnd, char * title, char * text, dialogHandler1 onAccept, dialogHandler1 onDecline, void * data)
{
    int ret;
    char textbuf[1024];
    char titlebuf[1024];
    
    strcpy(textbuf, localizedPrintf(text));
    strcpy(titlebuf, localizedPrintf(text));

    ret = MessageBox(hWnd, textbuf, titlebuf, MB_YESNO);

    if (ret == IDYES)
    {
        if (onAccept)
            onAccept(data);
    }
    else if (onDecline)
        onDecline(data);
}

typedef struct DlgParam
{
    void * data;
    dialogHandler2 onAccept;
    dialogHandler1 onDecline;
    char * title;
    char * prompt;

}DlgParam;

DlgParam * DlgParamCreate(char * title, char * prompt, dialogHandler2 onAccept, dialogHandler1 onDecline, void * data)
{
    DlgParam * dp = calloc(sizeof(DlgParam),1);
    
    dp->onAccept    = onAccept;
    dp->onDecline    = onDecline;
    dp->data        = data;
    dp->title        = strdup(title);
    dp->prompt        = strdup(prompt);
    
    return dp;
}

void DlgParamDestroy(DlgParam * dp)
{
    free(dp->title);
    free(dp->prompt);
    free(dp);
}

void CreateDialogPrompt(HWND hWnd, char * title, char * prompt, dialogHandler2 onAccept, dialogHandler1 onDecline, void * data)
{
    HWND hDlg = CreateDialogParam(g_hInst, 
                      MAKEINTRESOURCE(IDD_PROMPT_STRING), 
                      hWnd,
                      GetStringDlgProc,
                      (LPARAM)DlgParamCreate(title, prompt,onAccept,onDecline,data));

    if(!hDlg)
    {
        DWORD d = GetLastError();
        printf("Error: %d\n", d);
    }

    ShowWindow(hDlg, SW_SHOW);
}


static void fixStringButtons(HWND hDlg)
{
    int count = SendMessage(GetDlgItem(hDlg, IDC_EDIT_PROMPT_STRING),  WM_GETTEXTLENGTH, 0, 0);        
    EnableWindow(GetDlgItem(hDlg, IDOK), (count > 0) ? TRUE : FALSE);
}

static INT_PTR CALLBACK GetStringDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    DlgParam * dp = (DlgParam*)GetWindowLongPtr(hDlg, GWLP_USERDATA);
    
    switch (iMsg) {
        case WM_INITDIALOG:
            dp = (DlgParam*)lParam;
            SetWindowText(hDlg, dp->title);
            SetDlgItemText(hDlg, IDC_STATIC_PROMPT, dp->prompt);
            SetWindowLongPtr(hDlg, GWLP_USERDATA, (LONG_PTR)dp);
            fixStringButtons(hDlg);
            SetFocus((GetDlgItem(hDlg, IDC_EDIT_PROMPT_STRING)));
            LocalizeControls(hDlg);
        xcase WM_COMMAND:
            {
                switch (LOWORD (wParam))
                {
                case IDOK: // Syncronize Data
                    {
                        char text[1000];
                        GetDlgItemText(hDlg, IDC_EDIT_PROMPT_STRING, text, sizeof(text));
                        if(dp->onAccept)
                            dp->onAccept(dp->data, text);
                        DlgParamDestroy(dp);

                        EndDialog(hDlg, LOWORD (wParam));
                        return TRUE;
                    }
                xcase IDCANCEL:
                    if(dp->onDecline)
                        dp->onDecline(dp->data);
                    DlgParamDestroy(dp);
                    EndDialog(hDlg, LOWORD (wParam));
                    return TRUE;
                case IDC_EDIT_PROMPT_STRING:
                    {
                        if(EN_CHANGE == HIWORD(wParam))
                        {
                            fixStringButtons(hDlg);
                        }
                    }
                }
            }
        break;

    }
    return FALSE;
}

int computeTimeMins(HWND hDlg)
{
    int days, hours, mins;
    char dayStr[100], hourStr[100], minStr[100];
    dayStr[0] = hourStr[0] = minStr[0] = 0;

    GetDlgItemText(hDlg, IDC_EDIT_DAYS, dayStr, sizeof(dayStr));
    GetDlgItemText(hDlg, IDC_EDIT_HOURS, hourStr, sizeof(hourStr));
    GetDlgItemText(hDlg, IDC_EDIT_MINUTES, minStr, sizeof(minStr));

    days    = atoi(dayStr);
    hours    = atoi(hourStr);
    mins    = atoi(minStr);

    if(days >= 0 && hours >= 0 && mins >= 0)
        return (((days * 24) + hours) * 60) + mins;
    else
        return -1;
}

void updateExpireTime(HWND hDlg)
{
    U32 mins = computeTimeMins(hDlg);
    char * text;
     char dateStr[1000];
    if(mins > 0)
    {
        timerMakeDateStringFromSecondsSince2000(dateStr, ((mins * 60) + timerSecondsSince2000()));
        text = localizedPrintf("ExpireTime", dateStr);
        text = dateStr;
    }
    else if(mins < 0)
        text =    localizedPrintf("BadExpireTime"); // invalid time in box...
    else
        text = "";

    SetDlgItemText(hDlg, IDC_STATIC_EXPIRE, text); 
}

static void fixTimeButtons(HWND hDlg)
{
    int mins = computeTimeMins(hDlg);
    EnableWindow(GetDlgItem(hDlg, IDOK), (mins > 0 ? TRUE : FALSE));
}

static INT_PTR CALLBACK GetTimeDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    DlgParam * dp = (DlgParam*)GetWindowLongPtr(hDlg, GWLP_USERDATA);
    char buf[1000];

    switch (iMsg) {
        case WM_INITDIALOG:
        {
            int val;
            dp = (DlgParam*)lParam;
            SetWindowText(hDlg, dp->title);
            SetDlgItemText(hDlg, IDC_STATIC_PROMPT, dp->prompt);
            SetDlgItemText(hDlg, IDC_STATIC_EXPIRE, "");
            SetWindowLongPtr(hDlg, GWLP_USERDATA, (LONG_PTR)dp);
            fixTimeButtons(hDlg);

            val = regGetInt("LastBanDays",  0);
            SetDlgItemText(hDlg, IDC_EDIT_DAYS,        (val ? itoa(val, buf, 10) : ""));
            
            val = regGetInt("LastBanHours",  0);
            SetDlgItemText(hDlg, IDC_EDIT_HOURS,    (val ? itoa(val, buf, 10) : ""));

            if(val)    // i expect the "hours" edit box to get the most usage...
                SendMessage(GetDlgItem(hDlg, IDC_EDIT_HOURS), EM_SETSEL, 0, -1); // select all text
            
            val = regGetInt("LastBanMins",  0);
            SetDlgItemText(hDlg, IDC_EDIT_MINUTES,    (val ? itoa(val, buf, 10) : ""));
            
            SetFocus((GetDlgItem(hDlg, IDC_EDIT_HOURS)));
        //    SetTimer(hDlg, 0, 1000, NULL);
            LocalizeControls(hDlg);
        }
        xcase WM_TIMER:
        //    updateExpireTime(hDlg);
        xcase WM_COMMAND:
            {
                switch (LOWORD (wParam))
                {
                    case IDOK: // Syncronize Data
                    {    
                        if(dp->onAccept)
                            dp->onAccept(dp->data, (void*)(intptr_t)computeTimeMins(hDlg));
                        DlgParamDestroy(dp);

                        GetDlgItemText(hDlg, IDC_EDIT_DAYS, buf, sizeof(buf));
                        regPutInt("LastBanDays",  atoi(buf));

                        GetDlgItemText(hDlg, IDC_EDIT_HOURS, buf, sizeof(buf));
                        regPutInt("LastBanHours",  atoi(buf));

                        GetDlgItemText(hDlg, IDC_EDIT_MINUTES, buf, sizeof(buf));
                        regPutInt("LastBanMins",  atoi(buf));

                        EndDialog(hDlg, LOWORD (wParam));
                        return TRUE;
                    }
                    xcase IDCANCEL:
                    {
                            if(dp->onDecline)
                            dp->onDecline(dp->data);
                        DlgParamDestroy(dp);
                        EndDialog(hDlg, LOWORD (wParam));
                        return TRUE;
                    }
                    xcase IDC_EDIT_DAYS:
                    case IDC_EDIT_HOURS:
                    case IDC_EDIT_MINUTES:
                    {
                        if(EN_CHANGE == HIWORD(wParam))
                        {
//                            updateExpireTime(hDlg);
                            fixTimeButtons(hDlg);
                        }
                    }
                }
            }
        break;

    }
    return FALSE;
}


void CreateDialogPromptTime(HWND hWnd, char * title, char * prompt, dialogHandler2 onAccept, dialogHandler1 onDecline, void * data)
{
    HWND hDlg = CreateDialogParam(g_hInst, 
        MAKEINTRESOURCE(IDD_PROMPT_TIME), 
        hWnd,
        GetTimeDlgProc,
        (LPARAM)dp);

    if(!hDlg)
    {
        DWORD d = GetLastError();
        printf("Error: %d\n", d);
    }

    ShowWindow(hDlg, SW_SHOW);
}
