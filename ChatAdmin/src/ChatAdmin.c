#include <utilitieslib/utils/utils.h>
#include <utilitieslib/components/BitStream.h>
#include <utilitieslib/utils/error.h>
#include <utilitieslib/network/sock.h>
#include <utilitieslib/network/net_packet.h>
#include "resource.h"
#include <utilitieslib/utils/winutil.h>
#include <utilitieslib/utils/WinTabUtil.h>
#include <utilitieslib/components/earray.h>
#include "ChatAdmin.h"
#include "UserList.h"
#include "ChannelList.h"
#include <utilitieslib/utils/MemoryMonitor.h>
#include "ChatAdminNet.h"
#include <utilitieslib/utils/mathutil.h>
#include "ChannelMonitor.h"
#include "MessageView.h"
#include <utilitieslib/language/AppLocale.h>
#include "LangAdminUtil.h"
#include "CustomDialogs.h"
#include "Login.h"
#include "ChatAdminUtils.h"
#include <richedit.h>
#include <commctrl.h>
#include <utilitieslib/utils/log.h>

HINSTANCE g_hInst = 0; // instance handle
HWND g_hProgress;
HWND g_hDlgMain;
HWND g_hStatusBar;
HWND g_hUserListTab;
HWND g_hChannelListTab;

bool g_ChatDebug;
bool g_bShowChatTimestamps = 0;

MessageView * g_mvConsole;

void chatAdminParseArgs(LPSTR lpCmdLine)
{
    if (strstri(lpCmdLine, "-console"))
        newConsoleWindow();
    
    if(strstri(lpCmdLine, "-ChatDebug"))
        g_ChatDebug = 1;
}


void LoadTabs(HWND hwndDlg)
{
    DLGHDR *pHdr = (DLGHDR *)GetWindowLongPtr(hwndDlg, GWLP_USERDATA);
    pHdr->eaTabs = NULL;

    g_hUserListTab        = TabDlgInsertTab(g_hInst, pHdr, IDD_DLG_USERLIST,        DlgUserListProc,        localizedPrintf("CAUserTab"),        1, 0);
    g_hChannelListTab    = TabDlgInsertTab(g_hInst, pHdr, IDD_DLG_CHANNELLIST,    DlgChannelListProc,        localizedPrintf("CAChannelTab"),        0, 0);
}


void ChatAdminInit()
{
    UserListInit();
    ChannelListInit();

    ChatAdminInitNet();

    EnableControls(NULL, FALSE);
}

#define listSetBgColor(hWnd, color) \
    ListView_SetBkColor(hWnd, color); \
    ListView_SetTextBkColor(hWnd, color);

void fixConnectButtons(HWND hDlg)
{
    static bool s_connectButtonsEnabled = 0;

    if (chatAdminConnected()) {
        if (s_connectButtonsEnabled) {
            ModifyMenu(GetMenu(hDlg), IDM_CONNECT, MF_BYCOMMAND | MF_STRING, IDM_CONNECT, localizedPrintf("CAControlDisconnect")); 
            listSetBgColor(GetDlgItem(g_hUserListTab, IDC_LST_ALLUSERS), GetSysColor(COLOR_WINDOW));
            UpdateWindow(GetDlgItem(g_hUserListTab, IDC_LST_ALLUSERS));
            setStatusBar(0, localizedPrintf("CAConnected"));
            //EnableWindow(GetDlgItem(hDlg, IDC_LST_LAUNCHERS), TRUE);
            s_connectButtonsEnabled = 0;
        }
    } else {
        if ( ! s_connectButtonsEnabled) {
            ModifyMenu(GetMenu(hDlg), IDM_CONNECT, MF_BYCOMMAND | MF_STRING, IDM_CONNECT, localizedPrintf("CAControlLogin")); 
            listSetBgColor(GetDlgItem(g_hUserListTab, IDC_LST_ALLUSERS), GetSysColor(COLOR_BTNFACE));        
            UpdateWindow(GetDlgItem(g_hUserListTab, IDC_LST_ALLUSERS));
            setStatusBar(0, localizedPrintf("CADisconnected"));
            //EnableWindow(GetDlgItem(hDlg, IDC_LST_LAUNCHERS), FALSE);
            s_connectButtonsEnabled = 1;
    
        }
    }
}




void chatAdminTick(HWND hDlg)
{
    static bool s_firstTime = 1;

    if(s_firstTime)
    {
        s_firstTime = 0;
        LoginDlg(hDlg);
    }
    
    chatAdminNetTick();
    fixConnectButtons(hDlg);

//    conTransf(MSG_ERROR, "YouWereKickedFromChannel", "FunChannel");
}

void setProgressBar(F32 fraction)
{
    static int s_progress = 0;
    int progress = fraction * 100;

    progress = MINMAX(progress, 0, 100);
    if(progress != s_progress)
    {
        SendMessage(g_hProgress, PBM_SETPOS, (WPARAM)progress, 0);
        s_progress = progress;
    }
}

void setStatusBar(int elem, const char *fmt, ...) 
{
    char str[1000];
    va_list ap;

    va_start(ap, fmt);
    vsprintf(str, fmt, ap);
    va_end(ap);

    SendMessage(g_hStatusBar, SB_SETTEXT, elem, (LPARAM)str);
}


//--------------------------------------------------------------
// Filter test functions
//--------------------------------------------------------------
void CreateUsers()
{
    int i,k,r;
    int count = 500000;
    srand(1); // consistent user lists...
    printf("Adding %d users...\n", count);
    for(i=0;i<count;i++)
    {
        char handle[100];
        r = (rand() % 8) + 7;
        for(k=0;k<r;k++)
        {
            handle[k] = (rand() % 26) + 'a';
        }
        handle[k] = 0;
//        printf("Added user %s\n", handle);
        CAUserAdd(handle, i, rand() % 60, 0);
    }
    printf("Finished adding users\n", count);

    UserListFilter();
}

void conPrintf(int type, const char *fmt, ...) 
{
    char str[5000];
    va_list ap;

    va_start(ap, fmt);
    vsprintf(str, fmt, ap);
    va_end(ap);

    MessageViewAdd(g_mvConsole, str, type);
}

void ResizeConsoleView(HWND hDlg)
{
    RECT rMain, rStatus;
     GetWindowRect(hDlg, &rMain);
    GetWindowRect(g_hStatusBar, &rStatus);
    MoveWindow(GetDlgItem(hDlg, IDC_EDIT_FEEDBACK), 
                10,
                (rMain.bottom-rMain.top) - (rStatus.bottom-rStatus.top) - 150,
                (rMain.right-rMain.left) - 20,
                100,        
                TRUE);
}

void ChatAdminReset()
{
    ChanMonReset();
    ChannelListReset();
    UserListReset();

    EnableControls(NULL, FALSE);

    setStatusBar(1, "");
    setStatusBar(3, "");
    setProgressBar(0);
}


void onAcceptChatCmd(char * cmd, char * msg)
{    
    chatCmdSendf(cmd, "%s", msg);
}    

void onAcceptChatCmd1(char * cmd)
{    
    chatCmdSendf(cmd, "");
}    


void EasyCheckMenuItem(int itemID, bool checked)
{
    int state = checked ? MF_CHECKED : MF_UNCHECKED;
    CheckMenuItem(GetMenu(g_hDlgMain), itemID, state | MF_BYCOMMAND);
}

INT_PTR CALLBACK DlgMainProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    int i=0;

    DLGHDR *pHdr = (DLGHDR*)GetWindowLongPtr(hDlg, GWLP_USERDATA);

     switch (iMsg)
    {
    case WM_INITDIALOG:
        {

            int localeID = locGetIDInRegistry();
            reloadAdminMessageStores(localeID);

            g_hDlgMain = hDlg;

            //InitCommonControls();
            TabDlgInit(g_hInst, hDlg);
            LoadTabs(hDlg);
            TabDlgFinalize(hDlg);

            // Status bar stuff
            g_hStatusBar = CreateStatusWindow( WS_CHILD | WS_VISIBLE, "", hDlg, 1);
            assert(g_hStatusBar);
            {
                int temp[5];
                temp[0]=200;
                temp[1]=350;
                temp[2]=400;
                temp[3]=500;
                temp[4]=-1;
                SendMessage(g_hStatusBar, SB_SETPARTS, ARRAY_SIZE(temp), (LPARAM)temp);
            }

            g_hProgress = CreateWindowEx(0, PROGRESS_CLASS, NULL, WS_CHILD | WS_VISIBLE | PBS_SMOOTH,
                                         251, 2, 48, 18,g_hStatusBar, NULL, g_hInst, NULL);
            assert(g_hProgress);
            SendMessage(g_hProgress, PBM_SETRANGE, 0, MAKELPARAM(0, 100)); 

            g_mvConsole = MessageViewCreate(GetDlgItem(hDlg, IDC_EDIT_FEEDBACK), 2000);
            MessageViewShowTime(g_mvConsole, regGetInt("ShowConsoleTimestamps", 0));
            EasyCheckMenuItem(IDM_OPTIONS_CONSOLE_TIMESTAMPS, g_mvConsole->showTime);
            ResizeConsoleView(hDlg);

            g_bShowChatTimestamps = regGetInt("ShowChatTimestamps", 0);
            EasyCheckMenuItem(IDM_OPTIONS_CHAT_TIMESTAMPS, g_bShowChatTimestamps);
    
            SetTimer(hDlg, 0, 100, NULL);    // main tick loop
            SetTimer(hDlg, 1, 1000, UpdateNetRateProc);

            ChatAdminInit();

            LocalizeControls(hDlg);
        }
        return FALSE;
    xcase WM_TIMER:
         chatAdminTick(hDlg);
    xcase WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDOK: // Pass it on down when someone hits Enter
            SendMessage(pHdr->hwndCurrent, iMsg, wParam, lParam);
        xcase IDM_EXIT: // Pass it on down when the user clicks the X button
        case IDCANCEL:
            EndDialog(hDlg, 0);
            PostQuitMessage(0);
        xcase IDM_CONNECT:
            if(chatAdminConnected())
                chatAdminDisconnect();
            else
            {
    //            ChatAdminReset();
                LoginDlg(hDlg);
            }
            fixConnectButtons(hDlg);
        xcase IDM_SENDALL:
            CreateDialogPrompt(hDlg, "SendAllDialogTitle", "SendAllDialogPrompt", onAcceptChatCmd, NULL, "CsrSendAll");
        xcase IDM_SHUTDOWN:
            CreateDialogPrompt(hDlg, "ShutdownDialogTitle", "ShutdownDialogPrompt", onAcceptChatCmd, NULL, "Shutdown");
        xcase IDM_SILENCEALL:    
            CreateDialogYesNo(hDlg, "SilenceAllDialogTitle", "SilenceAllDialogPrompt", onAcceptChatCmd1, NULL, "CsrSilenceAll");
        xcase IDM_UNSILENCEALL:    
            CreateDialogYesNo(hDlg, "UnsilenceAllDialogTitle", "UnsilenceAllDialogPrompt", onAcceptChatCmd1, NULL, "CsrUnsilenceAll");
        xcase IDM_OPTIONS_CONSOLE_TIMESTAMPS:
        {    
            MessageViewShowTime(g_mvConsole, ! g_mvConsole->showTime);
            EasyCheckMenuItem(IDM_OPTIONS_CONSOLE_TIMESTAMPS, g_mvConsole->showTime);
            regPutInt("ShowConsoleTimestamps", g_mvConsole->showTime);
        }
        xcase IDM_OPTIONS_CHAT_TIMESTAMPS:
        {
            g_bShowChatTimestamps = ! g_bShowChatTimestamps;
            ChanMonShowTimestamps(g_bShowChatTimestamps);
            EasyCheckMenuItem(IDM_OPTIONS_CHAT_TIMESTAMPS, g_bShowChatTimestamps);
            regPutInt("ShowChatTimestamps", g_bShowChatTimestamps);
        }
        xcase IDM_STATUS_INVISIBLE:
            chatCmdSendf( (gAdminClient.invisible ? "Visible" : "Invisible"), "");
    }
    xcase WM_GETMINMAXINFO:
            ((MINMAXINFO*)lParam)->ptMinTrackSize.x = 600;    
        ((MINMAXINFO*)lParam)->ptMinTrackSize.y = 600;
    xcase WM_SIZE:
        {
            WORD w = LOWORD(lParam);
            WORD h = HIWORD(lParam);
            RECT rcMain, rcConsole, rcStatus;
            DWORD dwDlgBase = GetDialogBaseUnits(); 
            int cxMargin = LOWORD(dwDlgBase) / 4; 
            int cyMargin = HIWORD(dwDlgBase) / 8; 

            GetWindowRect(hDlg, &rcMain);
            GetWindowRect(GetDlgItem(hDlg, IDC_EDIT_FEEDBACK), &rcConsole);
            GetWindowRect(g_hStatusBar, &rcStatus);

            // must keep ALL dialogs consistent
            for(i=0; i < eaSize(&pHdr->eaTabs); i++)
            {
                // update the size of the dialog box in the tab window
                SetWindowPos(pHdr->eaTabs[i]->hwndDisplay, NULL, 
                    cxMargin,
                    GetSystemMetrics(SM_CYCAPTION),
                    (rcMain.right - rcMain.left) - (2 * GetSystemMetrics(SM_CXDLGFRAME)), 
                    (rcMain.bottom - rcMain.top) - (2 * GetSystemMetrics(SM_CYDLGFRAME) + 2 * GetSystemMetrics(SM_CYCAPTION)) - ((rcConsole.bottom - rcConsole.top) + (rcStatus.bottom - rcStatus.top) + 30), 
                    0);

            }


             SendMessage(g_hStatusBar,iMsg,wParam,lParam);    
            InvalidateRect(g_hStatusBar, NULL, TRUE);
            UpdateWindow(g_hStatusBar);
            
            ResizeConsoleView(hDlg);

            InvalidateRect(NULL, NULL, TRUE);    // repaint all windows
        }
    xcase WM_NOTIFY:
        {
            switch (((NMHDR *) lParam)->code) 
            { 
            case TCN_SELCHANGE: 
                { 
                    TabDlgOnSelChanged(hDlg);    // processed when user clicks on new tab
                } 
            }
        }
    xcase WM_CTLCOLORSTATIC:
        {
            if(((HWND)lParam) == GetDlgItem(hDlg, IDC_EDIT_FEEDBACK))
            {
                // make read-only edit boxes a lighter shade of gray
                HDC hdcStatic = (HDC)wParam;
                COLORREF color = RGB(230,230,230);
                SetTextColor(hdcStatic, RGB(0, 0, 0));
                SetBkColor(hdcStatic, color);
                return (LRESULT)CreateSolidBrush(color);
            }
        }
        break;
    }
    return FALSE;
}

int WINAPI WinMain ( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
    MSG msg={0};
    int quit=0;
    HWND hDlg=0;
    BOOL bRet=0;

    g_hInst = hInstance;

    memCheckInit();

    bsAssertOnErrors(true);

    logSetDir("ChatAdmin");

    InitCommonControls();

    chatAdminParseArgs(lpCmdLine);

    sockStart();
    packetStartup(0,1);
    pktSetDebugInfo();

    hDlg = CreateDialog(hInstance, (LPCTSTR) (IDD_DLG_MAIN), NULL, DlgMainProc);
    if(!hDlg)
    {
        DWORD i = GetLastError();
        printf("Last Error: %d", i);
    }
    assert(hDlg);
    ShowWindow(hDlg, SW_SHOW);
    {
        HICON hIcon  = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));
        if(hIcon) {
            SendMessage(hDlg, WM_SETICON, ICON_BIG,        (LPARAM)hIcon);
            SendMessage(hDlg, WM_SETICON, ICON_SMALL,    (LPARAM)hIcon);
        }
    }

    while( (bRet = GetMessage( &msg, NULL, 0, 0 )) != 0)
    { 
        if (bRet == -1)
        {
            // handle the error and possibly exit
        }
        else 
        {
//            if (IsDialogMessage(hDlg, &msg)) {
////            } else if (smentsHook(&msg)) {
//            } else {
                TranslateMessage(&msg); 
                DispatchMessage(&msg); 
//            }
        }
    } 

    // Return the exit code to the system. 
    return msg.wParam; 
}

// remove underscores, spaces, punctuation, etc & make all lowercsase 
// (for faster filtering)
void stripNonAlphaNumeric(char * str)
{
    char *src = str;
    char *dst = str;
    
    while(*src)
    {
        if(isalnum((unsigned char)*src))
            *(dst++) = tolower(*src);

        ++src;
    }
    *dst = 0;
}

// MJP: Attempt to allow wildcards... this was disabled in order to drastically speed up filtering,
//        as wildcards would not allow a binary lookup to find the first first filtered item
//        in the sorted list of all elements (lv->elements)
//int FilterMatch(char * str, char * pattern)
//{
//    bool wildcard = 0;
//    while(*str && *pattern)
//    {
//        if(*pattern == '*')
//        {
//            while(*pattern == '*')
//            {
//                ++pattern;
//            }
//            wildcard = 1;
//        }
//        else
//        {
//            if(wildcard)
//            {
//                if(FilterMatch(str, pattern))
//                    return 1;
//            }
//            else
//            {
//                if(tolower(*str) != tolower(*pattern)) // FUTURE OPTIM: preset to lowercase 
//                    return 0;
//                else
//                    ++pattern;
//            }
//
//            ++str;
//
//            while(isspace(*str))
//            {
//                ++str;
//            }
//        }
//    }
//
//    if(*pattern == 0)
//        return 1; // all chars matched
//    else
//        return 0;
//}






//---------------------------------------------------------------------------------------------------------------------------------
// Enable/Disable Controls
//---------------------------------------------------------------------------------------------------------------------------------



typedef struct {

    int accessLevel;    //        if >=0, is disabled when disconnected.
                        //        if  >0, is disabled when connected if less than access level
    int    id;
    char * localizedName;
}ControlAccess;


ControlAccess menuAccess[] = 
{
    // Menu
    {    -1,    IDM_CONNECT,            "CAControlConnect"        },
    {    -1,    IDM_EXIT,                "CAControlExit"            },

    {    3,    IDM_SENDALL,            "CAControlSendall"        },
    {    3,    IDM_SHUTDOWN,            "CAControlShutdown"        },
    {    3,    IDM_SILENCEALL,            "CAControlSilenceAll"    },
    {    3,    IDM_UNSILENCEALL,        "CAControlUnsilenceAll"    }, 

    {    0,    IDM_STATUS_INVISIBLE,    "CAControlInvisible"    },

    {    0,    IDM_OPTIONS_CONSOLE_TIMESTAMPS, "CAControlConsoleTimestamps"    },
    {    0,    IDM_OPTIONS_CHAT_TIMESTAMPS,    "CAControlChatTimestamps"        },

    { 0 },
};


ControlAccess controlAccess[] = 
{
    // UserList Tab
    {    0,    IDC_LST_USER_STATUS,        },
    {    0,    IDC_EDIT_USERLIST_FILTER,    },
    {    0,    IDC_GROUP_USERLIST_FILTER,    "CAControlFilter"            },
    {    0,    IDC_RADIO_ALL,                "CAControlFilterAll"        },
    {    0,    IDC_RADIO_ONLINE,            "CAControlFilterOnline"        },
    {    0,    IDC_RADIO_OFFLINE,            "CAControlFilterOffline"    },
    {    0,    IDC_BUTTON_USERLIST_INFO,    "CAControlUserInfo"            },
    {    2,    IDB_USERLIST_RENAME,        "CAControlUserRename"        },
    {    2,    IDB_USERLIST_CHATBAN,        "CAControlUserChatBan"        },
    {    1,    IDB_USERLIST_PRIVATECHAT,    "CAControlUserPrivateChat"    },

    // ChannelList Tab
    {    0,    IDC_LST_ALLCHANNELS,            },
    {    0,    IDC_EDIT_CHANNELLIST_FILTER,    },
    {    0,    IDC_GROUP_CHANNELLIST_FILTER,    "CAControlFilter"            },
    {    0,    IDB_CHANNELLIST_CREATE,            "CAControlChannelCreate"    },
    {    0,    IDB_CHANNELLIST_JOIN,            "CAControlChannelJoin"        },
//    {    1,    IDB_CHANNELLIST_SILENCE,                                    },
    {    2,    IDB_CHANNELLIST_KILL,            "CAControlChannelKill"        },

    // ChannelMonitor Tab
    {    0,    IDC_LST_CHANMON_MEMBERS,        },
    {    0,    IDC_GROUP_CHANMON_FILTER,    "CAControlFilter"                        },
    {    2,    IDB_CHANMON_RENAME,            "CAControlChannelMonitorRename"            },
    {    2,    IDB_CHANMON_KILL,            "CAControlChannelKill"                    },
    {    1,    IDB_CHANMON_SILENCE,        "CAControlChannelMonitorSilence"        },
    {    1,    IDB_CHANMON_OPERATOR,        "CAControlChannelMonitorMakeOperator"    },
    {    1,    IDB_CHANMON_PRIVATECHAT,    "CAControlChannelMonitorPrivateChat"    },
    {    1,    IDB_CHANMON_KICK,            "CAControlChannelMonitorKick"            },
    {    1,    IDB_CHANMON_SEND,            "CAControlChannelMonitorSend"            },
    {    1,    IDB_CHANMON_LEAVE,            "CAControlChannelMonitorLeave"            },


    // Login Dialog
    {    0,    IDC_LOGIN_SERVER,            "CAControlLoginServer"            },
    {    0,    IDC_LOGIN_HANDLE,            "CAControlLoginHandle"            },
    {    0,    IDC_LOGIN_PASSWORD,            "CAControlLoginPassword"        },
    {    0,    IDC_LOGIN_OK,                "CAControlLoginOk"                },

    { 0 },
};

BOOL CALLBACK EnableControlEnumProc(HWND hWnd, LPARAM enable);


// Iterate through all controls in the 'controlAccess' list
//        If(enable), Then enable all windows that meet minimum access level requirements
//        Else, disable all windows
void EnableControls(HWND hDlg, BOOL enable)
{
    ControlAccess * control = &menuAccess[0];
    HMENU hMenu;

    if(!hDlg)
        hDlg = g_hDlgMain;

    // do non-menu controls (buttons, lists, etc)
    EnumChildWindows(hDlg, EnableControlEnumProc, enable);

    // do menu controls
    hMenu = GetMenu(hDlg);
    if(hMenu)
    {
        while(control->id)
        {
            int state = MF_BYCOMMAND;
            if(    (enable && (control->accessLevel <= gAdminClient.accessLevel))
                || control->accessLevel < 0)
                state |= MF_ENABLED;
            else
                state |= (MF_GRAYED | MF_DISABLED);
            
            EnableMenuItem(hMenu, control->id, state);
            control++;
        }
    }
}

BOOL CALLBACK EnableControlEnumProc(HWND hWnd, LPARAM enable)
{
    HWND parent = GetParent(hWnd);
    ControlAccess * control = &controlAccess[0];
    while(control->id)
    {
        if(hWnd == GetDlgItem(parent, control->id))
        {
            if(enable)
                EnableWindow(hWnd, (control->accessLevel <= gAdminClient.accessLevel) ?  TRUE : FALSE);
            else
                EnableWindow(hWnd, FALSE);
        }
        control++;
    }

    return 1;
}



BOOL CALLBACK LocalizeControlEnumProc(HWND hWnd, LPARAM unused)
{
    HWND parent = GetParent(hWnd);
    ControlAccess * control = &controlAccess[0];
    while(control->id)
    {
        if(hWnd == GetDlgItem(parent, control->id))
        {
            if(control->localizedName)
                SetWindowText(hWnd, localizedPrintf(control->localizedName));
        }
        control++;
    }

    return 1;
}


void LocalizeControls(HWND hDlg)
{
    ControlAccess * control = &menuAccess[0];
    HMENU hMenu;

    if(!hDlg)
        hDlg = g_hDlgMain;

    // do non-menu controls (buttons, lists, etc)
    EnumChildWindows(hDlg, (WNDENUMPROC) LocalizeControlEnumProc, 0);

    // do menu controls
    hMenu = GetMenu(hDlg);
    if(hMenu)
    {    
        MENUITEMINFO info;
        info.cbSize = sizeof(MENUITEMINFO);
        info.fMask = MIIM_STRING;

        while(control->id)
        {
            if(control->localizedName)
            {
                info.dwTypeData = localizedPrintf(control->localizedName);
                SetMenuItemInfo(hMenu, control->id, FALSE, &info);
            }

            control++;
        }
    }
}
