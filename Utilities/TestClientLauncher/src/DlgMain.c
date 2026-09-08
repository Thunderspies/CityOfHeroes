#include <utilitieslib/stdtypes.h>
#include <winsock2.h>
#include <windows.h>
#include <commctrl.h>
#include "resource.h"
#include <string.h>
#include <stdio.h>
#include "TestClientLauncher.h"
#include <commctrl.h>
#include <utilitieslib/utils/winutil.h>
#include <utilitieslib/utils/StringUtil.h>

#pragma comment (lib, "comctl32.lib")

#define ARRAY_SIZE(n) (sizeof(n) / sizeof((n)[0]))

extern HINSTANCE   g_hInst;                    // instance handle
extern HWND        g_hWnd;                        // window handle
static HWND            g_hLB;

int g_hide=1;
int g_justlogin=0;
int g_disconnect=0;
int g_mapmove=0;
int g_move=1;
int g_lms=0;
int g_timed_reconnect=0;
int g_random_disconnect=0;
int g_use_authserver=0;
int g_ask_user=0;
int g_notimeout=0;
int g_fight=1;
int g_autorez=1;
int g_missions=0;
int g_teamup=0;
int g_league=0;
int g_turnstile=0;
int g_silent=0;
int g_nolauncherchat=0;
int g_arenafighter=0;
int g_killbot=0;
int g_eventcreator=0;
int g_arenathrasher=0;
int g_arenasupergroup=0;
int g_arenascheduled=0;
int g_accountserver=0;
int g_incrementauth=0;
int g_cov=0;
int g_auctionserver=0;
int g_nosharedmemory=0;
int g_chat=0;
int g_packetdebug = 0;
int g_mission_stress = 0;
int g_static_map_stress = 0;
int g_verbose_client=0;

char strCommand[1024];
char cs[512]="Jimb";


INT_PTR CALLBACK DlgMainProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

void DlgMainDoDialog()
{
    InitCommonControls();
    DialogBox (g_hInst, (LPCTSTR) (IDD_DLGMAIN), NULL, DlgMainProc);
}


int addLineToEnd(int nIDDlgItem, char *fmt, ...) {
    HWND hLB = GetDlgItem(g_hLB, nIDDlgItem);
    LRESULT index;
    char str[1000];
    int oldindex, oldtop;
    va_list ap;

    va_start(ap, fmt);
    vsprintf(str, fmt, ap);
    va_end(ap);
    
    SendMessage(hLB, LB_SETHORIZONTALEXTENT, 1000, 0);
    oldindex = (int)SendMessage(hLB, LB_GETCARETINDEX, 0, 0);
    oldtop = (int)SendMessage(hLB, LB_GETTOPINDEX, 0, 0);

    index = SendMessage(hLB, LB_ADDSTRING, 0, (LPARAM)TCharFromCharStatic(str));

    if (index==oldindex+1) { // scroll down
        SendMessage(hLB, LB_SETTOPINDEX, index, (LPARAM)0);
        if (LB_ERR==SendMessage(hLB, LB_SETCARETINDEX, index, (LPARAM)0)) {
            // Single selection or no-selection list box
            SendMessage(hLB, LB_SETCURSEL, index, (LPARAM)0);
        }
    } else {
        SendMessage(hLB, LB_SETCARETINDEX, oldindex, (LPARAM)0);
        SendMessage(hLB, LB_SETTOPINDEX, oldtop, (LPARAM)0);
    }
    return (int)index;
}

void updateLine(int nIDDlgItem, int index, char *fmt, ...) { // May not work on single/no-selection list boxes
    HWND hLB = GetDlgItem(g_hLB, nIDDlgItem);
    char str[1000];
    int oldindex, oldtop, oldsel;
    va_list ap;

    va_start(ap, fmt);
    vsprintf(str, fmt, ap);
    va_end(ap);

    oldindex = (int)SendMessage(hLB, LB_GETCARETINDEX, 0, 0);
    oldtop = (int)SendMessage(hLB, LB_GETTOPINDEX, 0, 0);
    oldsel = (int)SendMessage(hLB, LB_GETSEL, index, 0);

    SendMessage(hLB, LB_DELETESTRING, index, (LPARAM)0);
    SendMessage(hLB, LB_INSERTSTRING, index, (LPARAM)str);
    if (oldsel) {
        SendMessage(hLB, LB_SETSEL, TRUE, index);
    }

    SendMessage(hLB, LB_SETCARETINDEX, oldindex, (LPARAM)0);
    SendMessage(hLB, LB_SETTOPINDEX, oldtop, (LPARAM)0);
}

static HWND hStatusBar=NULL;
static HWND hRadio1, hRadio2;

void setStatusBar(int elem, const char *fmt, ...) {
    char str[1000];
    TCHAR tmp[ARRAY_SIZE(str)];
    va_list ap;
    va_start(ap, fmt);
    vsprintf(str, fmt, ap);
    va_end(ap);
    TCharFromChar(tmp, str, ARRAY_SIZE(tmp));
    SendMessage(hStatusBar, SB_SETTEXT, elem, (LPARAM)tmp);
}

void onShowChild(HWND hDlg, int cmd) {
    doOnSelected(hDlg, (ListViewCallbackFunc)showChild, (void*)(intptr_t)cmd);
}

void onKillChild(HWND hDlg) {
    doOnSelected(hDlg, killChild, NULL);
}

void onSendCommand(HWND hDlg) {
    doOnSelected(hDlg, sendCommand, strCommand);
}

void onSetNextChild(HWND hDlg) {
    doOnSelected(hDlg, (ListViewCallbackFunc)setNextChild, NULL);
}

void onGrabConsole(HWND hDlg) {
    doOnSelected(hDlg, (ListViewCallbackFunc)grabConsole, NULL);
}

void onQuitChild(HWND hDlg)
{
    doOnSelected(hDlg, quitChild, NULL);
}

void disableRB2(void) {
    SendMessage(hRadio1, BM_SETCHECK, BST_CHECKED, 0);
    EnableWindow(hRadio2, FALSE);
}

void enableRB2(void) {
    SendMessage(hRadio1, BM_SETCHECK, BST_UNCHECKED, 0);
    SendMessage(hRadio2, BM_SETCHECK, BST_CHECKED, 0);
    EnableWindow(hRadio2, TRUE);
}

void doLaunch() {
    char cs[512] = {0};
    TCHAR tcs[ARRAY_SIZE(cs)];

    int launch_count = 0;
    SendMessage(GetDlgItem(g_hLB, IDC_EDIT_LAUNCH_COUNT), WM_GETTEXT, ARRAY_SIZE(tcs), (LPARAM)tcs);
    CharFromTChar(cs, tcs, ARRAY_SIZE(cs));
    launch_count = atoi(cs);

    if (g_ask_user && g_hide) {
        addLineToEnd(IDC_LOG, "Will not launch with both -askuser and -hide, it's probably not what you want");
    } else if (g_ask_user && launch_count) {
        addLineToEnd(IDC_LOG, "Will not launch more than 1 client with -askuser at a time");
    } else {
        if (SendMessage(hRadio2, BM_GETCHECK, 0, 0)) {
            if (g_ask_user) {
                addLineToEnd(IDC_LOG, "Cannot launch on slaves with -askuser");
            } else {
                if (launch_count) {
                    launchNewClientsOnSlaves(launch_count);
                } else {
                    launchNewClientOnSlaves();
                }
            }
        } else {
            if (launch_count) {
                int delay_in_ms = 0;

                SendMessage(GetDlgItem(g_hLB, IDC_EDIT_LAUNCH_DELAY), WM_GETTEXT, ARRAY_SIZE(tcs), (LPARAM)tcs);
                CharFromTChar(cs, tcs, ARRAY_SIZE(cs));
                delay_in_ms = atoi(cs);

                // get periodic launch
                launchNewClients(launch_count, delay_in_ms);
            } else {
                launchNewClient(0);
            }
        }
    }
}

typedef struct {
    int id;
    int *var;
    char *key;
    char *param;
} idpair_t;

idpair_t buttons[] = {
    {IDC_CHK_HIDE,                &g_hide,                "Hide",                "-hideconsole"},
    {IDC_CHK_JUSTLOGIN,            &g_justlogin,            "JustLogin",        "-justlogin"},
    {IDC_CHK_DISCONNCT,            &g_disconnect,            "Disconnect",        "-disconnect"},
    {IDC_CHK_MAPMOVE,            &g_mapmove,                "MapMove",            "+mapmove"},
    {IDC_CHK_MOVE,                &g_move,                "Move",                "+move"},
    {IDC_CHK_LMS,                &g_lms,                    "LocalMapServer",    NULL},
    {IDC_CHK_TIMED_RECONNECT,    &g_timed_reconnect,        "TimedReconnect",    "-timedreconnect"},
    {IDC_CHK_RANDOM_DISCONNECT,    &g_random_disconnect,    "RandomeDisconnect","-randomdisconnect"},
    {IDC_CHK_AUTHSERVER,        &g_use_authserver,        "UseAuthServer",    NULL},
    {IDC_CHK_ASK_USER,            &g_ask_user,            "AskUser",            "-askuser"},
    {IDC_CHK_NOTIMEOUT,            &g_notimeout,            "NoTimeout",        "-notimeout"},
    {IDC_CHK_FIGHT,                &g_fight,                "Fight",            "+fight"},
    {IDC_CHK_AUTOREZ,            &g_autorez,                "AutoRez",            "+autorez"},
    {IDC_CHK_MISSIONS,            &g_missions,            "Missions",            "+missions"},
    {IDC_CHK_TEAMUP,            &g_teamup,                "Teamup",            "+teaminvite"},
    {IDC_CHK_SILENT,            &g_silent,                "Silent",            "-silent"},
    {IDC_CHK_NOLAUNCHERCHAT,    &g_nolauncherchat,        "NoLauncherChat",    "-nolauncherchat"},
    {IDC_CHK_ARENAFIGHTER,        &g_arenafighter,        "ArenaFighter",        "+arenafighter"},
    {IDC_CHK_KILLBOT,            &g_killbot,                "KillBot",            "+killbot"},
    {IDC_CHK_EVENTCREATOR,        &g_eventcreator,        "EventCreator",        "+eventcreator"},
    {IDC_CHK_ARENATHRASHER,        &g_arenathrasher,        "ArenaThrasher",    "+arenathrasher"},
    {IDC_CHK_ARENASUPERGROUP,    &g_arenasupergroup,        "ArenaSupergroup",    "+arenasupergroup"},
    {IDC_CHK_ARENASCHEDULED,    &g_arenascheduled,        "ArenaScheduled",    "+arenascheduled"    },
    {IDC_CHK_ACCOUNTSERVER,        &g_accountserver,        "AccountServer",    "+accountserver"    },
    {IDC_CHK_AUCTIONSERVER,        &g_auctionserver,        "AuctionServer",    "+auction"},
    {IDC_CHK_INCREMENTAUTH,        &g_incrementauth,        "IncrementalAuth",    NULL    },
    {IDC_CHK_LEAGUE,            &g_league,                "League",            "+leagueinvite"},
    {IDC_CHK_TURNSTILE,            &g_turnstile,            "Turnstile",        "+turnstile"},
    {IDC_CHK_COV,                &g_cov,                    "CoV",                "-cov"},
    {IDC_CHK_NOSHAREDMEM,        &g_nosharedmemory,        "NoSharedMemory",    "-nosharedmemory"},
    {IDC_CHK_CHAT,                &g_chat,                "Chat",                "+chat"},
    {IDC_CHK_PACKETDEBUG,        &g_packetdebug,            "Packet Debug",        "-packetdebug"},
    {IDC_CHK_MISSION_STRESS,    &g_mission_stress,        "Mission Map Stress","+missionstress"},
    {IDC_CHK_MAPMOVE_STRESS,    &g_static_map_stress,    "Static Map Stress", "+mapstress"},
    {IDC_CHK_VERBOSE,            &g_verbose_client,        "Verbose Client",     "+verbose"},

};

char *getCS() {
    static char cs[512] = {0};
    static char oldcs[512] = {0};
    static TCHAR tcs[ARRAY_SIZE(cs)];

    SendMessage(GetDlgItem(g_hLB, IDC_CS), WM_GETTEXT, ARRAY_SIZE(tcs), (LPARAM)tcs);
    CharFromTChar( cs, tcs, ARRAY_SIZE(cs) );

    if (strcmp(cs, oldcs)!=0) {
        strcpy(oldcs, cs);
        regPutString("CS", cs);
    }
    return cs;
}

void getCommand() {
    static char newCommand[1024] = {0};
    static TCHAR tnewCommand[1024] = {0};
    SendMessage(GetDlgItem(g_hLB, IDC_COMMANDEDIT), WM_GETTEXT, 1024, (LPARAM)tnewCommand);
    CharFromTChar( newCommand, tnewCommand, 1024 );

    if (strcmp(strCommand,newCommand)!=0) {
        strcpy(strCommand,newCommand);
    }
}

char *makeParamString(void) 
{
    char *cs = getCS();
    static char last[1024];
    static char entry[1024];
    static TCHAR tentry[1024];
    static char param_string[512];
    int i;
    sprintf(param_string, "%%s %%d %s%s -parsechat", g_use_authserver?"-auth ":"-cs ", cs);
    for (i=0; i<ARRAY_SIZE(buttons); i++) {
        if (*(buttons[i].var) && (buttons[i].param)) {
            strcat(param_string, " ");
            strcat(param_string, buttons[i].param);
        }
    }
    if (g_lms) {
        strcat(param_string, " -localmapserver -server ");
        strcat(param_string, getComputerName());
    }
    
    //authname
    SendMessage(GetDlgItem(g_hLB, IDC_AUTHNAME), WM_GETTEXT, 1024, (LPARAM)tentry);
    CharFromTChar( entry, tentry, 1024 );
    if (0!=strcmp(entry, "")) {
        strcat(param_string, " -authname ");
        strcat(param_string, entry);
    }
    if(g_incrementauth)
    {
        int n = (int)strlen(entry);
        char *s = entry;
        char *e = entry + n;
        for(;!isdigit(*s) && s < e; s++){
        }
        if(s < e)
        {
            int v = atoi(s)+1;
            char tmp[ARRAY_SIZE(entry)];
            TCHAR *new_auth;
            *s = 0;
            sprintf(tmp,"%s%05i",entry,v);
            new_auth = TCharFromCharStatic(tmp);
            SendMessage(GetDlgItem(g_hLB, IDC_AUTHNAME), WM_SETTEXT, 0, (LPARAM)new_auth);
        }
    } 

    //password
    SendMessage(GetDlgItem(g_hLB, IDC_PASSWORD), WM_GETTEXT, 1024, (LPARAM)tentry);
    CharFromTChar( entry, tentry, 1024 );
    if (0!=strcmp(entry, "")) {
        strcat(param_string, " -password ");
        strcat(param_string, entry);
        strcpy(entry,"");
    }

    //version
    SendMessage(GetDlgItem(g_hLB, IDC_EDIT_VERSION), WM_GETTEXT, 1024, (LPARAM)tentry);
    CharFromTChar( entry, tentry, 1024 );
    if (0!=strcmp(entry, "")) {
        strcat(param_string, " -version ");
        strcat(param_string, entry);
        strcpy(entry,"");
    }

    //character
    SendMessage(GetDlgItem(g_hLB, IDC_CHARACTER), WM_GETTEXT, 1024, (LPARAM)tentry);
    CharFromTChar( entry, tentry, 1024 );
    if (0!=strcmp(entry, "")) {
        strcat(param_string, " -character \"");
        strcat(param_string, entry);
        strcat(param_string, "\"");
        strcpy(entry,"");
    }
    if (stricmp(param_string, last)!=0)
    {
        TCHAR tmp[ARRAY_SIZE(param_string)];
        TCharFromChar(tmp, param_string+6, ARRAY_SIZE(tmp));
        SetDlgItemText(g_hLB, IDC_PARAMS, tmp);
    }

    SendMessage(GetDlgItem(g_hLB, IDC_EDIT_EXTRA_ARGS), WM_GETTEXT, 1024, (LPARAM)tentry);
    CharFromTChar( entry, tentry, 1024 );
    if (0!=strcmp(entry, "")) {
        strcat(param_string, " ");
        strcat(param_string, entry);
        strcpy(entry,"");
    }

    strcpy(last, param_string);
    return param_string;
}

void loadValuesFromReg() {
    int i;
    for (i=0; i<ARRAY_SIZE(buttons); i++) {
        *(buttons[i].var) = regGetInt(buttons[i].key, *(buttons[i].var));
    }
}

void saveButtonValuesToReg() {
    int i;
    for (i=0; i<ARRAY_SIZE(buttons); i++) {
        regPutInt(buttons[i].key, *(buttons[i].var));
    }
}

INT_PTR CALLBACK DlgMainProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    int i;
    //BOOL b;
    switch (iMsg) {
        case WM_INITDIALOG:
            {
                HANDLE hFont;
                RECT rect;

                // Set initial values
                g_hLB = hDlg;

                // Fixed width font
                hFont = CreateFont (-10,6,0,0,FW_LIGHT, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
                    CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, TEXT("Lucida Console")); //"Courier New");

                /* Set the font to appropriate controls */
                SendMessage(GetDlgItem(g_hLB, IDC_LIST2), WM_SETFONT, (WPARAM)hFont,(LPARAM) MAKELONG((WORD) TRUE, 0));

                // Status bar stuff
                hStatusBar = CreateStatusWindow( WS_CHILD | WS_VISIBLE, TEXT("Status bar"), hDlg, 1);
                {
                    int temp[5];
                    temp[0]=100;
                    temp[1]=200;
                    temp[2]=300;
                    temp[3]=400;
                    temp[4]=-1;
                    SendMessage(hStatusBar, SB_SETPARTS, ARRAY_SIZE(temp), (LPARAM)temp);
                }

                // Initialize button values from registry
                loadValuesFromReg();

                // Initialize check boxes
                for (i=0; i<ARRAY_SIZE(buttons); i++) {
                    SendMessage(GetDlgItem(hDlg, buttons[i].id), BM_SETCHECK, *(buttons[i].var), 0);
                }

                // Radio button
                hRadio1 = GetDlgItem(g_hLB, IDC_RADIO1);
                hRadio2 = GetDlgItem(g_hLB, IDC_RADIO2);
                disableRB2();

                // Combo box
                SendMessage(GetDlgItem(hDlg, IDC_CS), CB_ADDSTRING, 0, (LPARAM)TEXT("prgprodshardv04"));
                SendMessage(GetDlgItem(hDlg, IDC_CS), CB_ADDSTRING, 0, (LPARAM)TEXT("prgprodshardv03"));
                SendMessage(GetDlgItem(hDlg, IDC_CS), CB_ADDSTRING, 0, (LPARAM)TEXT("prgprodshardv02"));
                SendMessage(GetDlgItem(hDlg, IDC_CS), CB_ADDSTRING, 0, (LPARAM)TEXT("prgprodshardv01"));
                SendMessage(GetDlgItem(hDlg, IDC_CS), CB_ADDSTRING, 0, (LPARAM)TEXT("localhost"));
                //SendMessage(GetDlgItem(hDlg, IDC_CS), CB_SETITEMHEIGHT, 0, 16);
                SendMessage(GetDlgItem(hDlg, IDC_CS), CB_SETCURSEL, 0, 1);
                SendMessage(GetDlgItem(hDlg, IDC_CS), WM_SETTEXT, 0, (LPARAM)TCharFromCharStatic(regGetString("CS", "Test")));

                SendMessage(GetDlgItem(hDlg, IDC_EDIT_LAUNCH_COUNT), WM_SETTEXT, 0, (LPARAM)TEXT("1"));

                SetTimer(hDlg, 0, 500, NULL);
                doInit(hDlg);

                GetClientRect(hDlg, &rect); 
                doDialogOnResize(hDlg, (WORD)(rect.right - rect.left), (WORD)(rect.bottom - rect.top), IDC_ALIGNME, IDC_UPPERLEFT);
            }
            return 0;
            break;

        case WM_TIMER:
            {
                // Simple check to make sure that this function isn't called at the same time multiple times
                static int inTimer = 0;
                if (!inTimer) {
                    inTimer=1;
                    doPeriodicUpdate();
                    //SendMessage(hDlg, WM_COMMAND, IDCHECK_STATUS, 0);
                    inTimer=0;
                }
            }
            break;

        case WM_NOTIFY:
            doNotify(hDlg, wParam, lParam);
            break;

        case WM_COMMAND:

            switch (LOWORD (wParam))
            {
            case IDLAUNCH:
                doLaunch();
                break;
            case IDCHECK_STATUS:
                while (checkChildren());
                break;
            case IDKILLALL:
                killAllChildren();
                break;
            case IDSHOW:
                onShowChild(GetDlgItem(hDlg, IDC_LIST2), SW_SHOW);
                break;
            case IDHIDE:
                onShowChild(GetDlgItem(hDlg, IDC_LIST2), SW_HIDE);
                break;
            case IDC_BTN_SETNEXT:
                onSetNextChild(GetDlgItem(hDlg, IDC_LIST2));
                break;
            case IDC_BTN_CONSOLE:
                onGrabConsole(GetDlgItem(hDlg, IDC_LIST2));
                break;
            case IDKILL:
                onKillChild(GetDlgItem(hDlg, IDC_LIST2));
                break;
            case IDEXIT:
                killAllChildren();
                EndDialog(hDlg, LOWORD (wParam));
                return TRUE;
            case IDSENDCOMMAND:
                getCommand();
                onSendCommand(GetDlgItem(hDlg, IDC_LIST2));
                SendMessage(GetDlgItem(hDlg, IDC_COMMANDEDIT), EM_SETSEL, 0, -1);
                break;
            case IDC_CS:
                break;
            case IDC_HELPBTN:
                ShellExecute ( NULL, TEXT("open"), TEXT("c:\\game\\docs\\System Implementation\\Test Client Commands.txt"), NULL, NULL, SW_SHOW);
                break;
            case IDLOGOUT:
                onQuitChild(GetDlgItem(hDlg, IDC_LIST2));
                break;
            case IDLOGALLOUT:
                quitAllChildren();
                break;
            default:
                {
                    int i;
                    int is_button=0;
                    for (i=0; i<ARRAY_SIZE(buttons); i++) {
                        if (LOWORD(wParam)==buttons[i].id) {
                            is_button = 1;
                        }
                    }
                    if (is_button) {
                        for (i=0; i<ARRAY_SIZE(buttons); i++) {
                            *(buttons[i].var) = (int)SendMessage(GetDlgItem(hDlg, buttons[i].id), BM_GETCHECK, 0, 0);
                        }
                        saveButtonValuesToReg();
                    }
                }
            }
            break;
        case WM_CLOSE:
        case WM_DESTROY:
            killAllChildren();
            PostQuitMessage(0);
            return 0;
        case WM_SIZE:
            {
                WORD w = LOWORD(lParam);
                WORD h = HIWORD(lParam);
                SendMessage(hStatusBar, iMsg, wParam, lParam);
                doDialogOnResize(hDlg, w, h, IDC_ALIGNME, IDC_UPPERLEFT);
            }
            return FALSE;
    }
    //return DefWindowProc(hDlg, iMsg, wParam, lParam);
    return FALSE;
}

