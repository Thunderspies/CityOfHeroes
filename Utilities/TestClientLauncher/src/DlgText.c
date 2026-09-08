
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
#include "DlgText.h"
#include "DlgMain.h"
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/utils/UnitSpec.h>
#include <utilitieslib/utils/error.h>
#include <utilitieslib/assert/assert.h>
#include "PipeServer.h"
#include <utilitieslib/utils/textparser.h>
#include <utilitieslib/utils/sysutil.h>
#include <utilitieslib/utils/StringUtil.h>
#include "tchar.h"

BatchModeData batchmode;
#undef fopen

#pragma comment (lib, "comctl32.lib")

#define ARRAY_SIZE(n) (sizeof(n) / sizeof((n)[0]))
//#define Strncpyt(dest, src) strncpyt(dest, src, ARRAY_SIZE(dest))

// Param

extern HINSTANCE   g_hInst;                    // instance handle
extern HWND        g_hWnd;                        // window handle
static HWND            g_hLB;
DLGHDR *g_pHdr = NULL;

VOID WINAPI OnSelChanged(HWND hwndDlg);

LRESULT CALLBACK DlgTextMainProc (HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK DlgTextTabProc (HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

typedef struct {
    TCHAR *displayname;
    TCHAR *printfstring;
    int udefnum;
} qcom_map_struct;

#define QCOM_MAX_MAP_SIZE 100
int qcom_map_size = 0;
qcom_map_struct qcom_map[QCOM_MAX_MAP_SIZE];

int num_servers = 0;
serverinfo_struct server_list[MAX_SERVERS];

typedef struct chatbox_string {
    char * str;
    int mtype;
    int mnum;
} chatbox_string;

TokenizerParseInfo ChatboxStringInfo[] = {
    { "Message",    TOK_STRING(chatbox_string, str, 0)},
    { 0 }
};

#define COMMAND_BUFFER_SIZE 40

#define BUF_SIZE 0x100
// rewritten getNextLine to handle struct instead of statics
int getNextBatchLine(FILE *pFile, char *line, int linesize, ChildInfo *ci) {
    int i = 0;
    char *buf = ci->batchbuf;

    if(!pFile)
    {
        // reset everything
        ci->batchOffset = 0;
        ci->batchMaxOffset = 0;
        return 0;
    }

    while(i < (linesize - 1))
    {
        if(ci->batchOffset >= ci->batchMaxOffset)
        {
            // fill up buffer
            if(-1 == (ci->batchMaxOffset = (int)fread(buf, sizeof(char), BUF_SIZE, pFile)))
            {
                printf("Error reading file!\n");
                return 0;
            }

            if(ci->batchMaxOffset)
            {
                line[i] = buf[0];
                ci->batchOffset = 1;
            }
            else
            {
                line[i] = '\0';
                return 0;
            }
        }
        else
        {
            line[i] = buf[ci->batchOffset++];
        }

        if(line[i] == '\n')
        {
            break;
        }

        i++;
    }
    line[i] = '\0';
    return 1;
}

//ab: this is a terrible terrible function
int getNextLine(FILE * pFile, char * line, int linesize)
{
    static int offset = 0;    
    static int maxOffset = 0;
    static char buf [BUF_SIZE];

    int i = 0;

    if(!pFile)
    {
        // reset everything
        offset        = 0;
        maxOffset    = 0;
        return 0;
    }

    while(i < (linesize - 1))
    {
        if(offset >= maxOffset)
        {
            // fill up buffer
            if(-1 == (maxOffset = (int)fread(buf, sizeof(char), BUF_SIZE, pFile)))
            {
                printf("Error reading file!\n");
                return 0;
            }

            if(maxOffset)
            {
                line[i] = buf[0];
                offset = 1;
            }
            else
            {
                line[i] = '\0';
                return 0;
            }
        }
        else
        {
            line[i] = buf[offset++];
        }

        if(line[i] == '\n')
        {
            break;
        }

        i++;
    }
    line[i] = '\0';
    if(!*line || (line[0] == '/' && line[1] == '/'))
        return getNextLine(pFile,line,linesize); // skip this line
    return 1;
}


void loadQuickCommandFile() {
    FILE * qcf = NULL;
    char line[1000];

    qcf = fopen("C:\\textclientcommand.txt","r");
    if (!qcf)
        qcf = fopen(".\\textclientcommand.txt","r");
    if(!qcf)
    {
        Errorf("warning: couldn't open .\\textclientcommand.txt or c:\\textclientcommand.txt");
        return;
    }
    

    getNextLine(0,0,0);

    while((qcom_map_size < QCOM_MAX_MAP_SIZE) && getNextLine(qcf,line,1000)) {
        char * c = strchr(line,':');
        qcom_map[qcom_map_size].udefnum = -1;
        if (c) {
            c[0]='\0';
            c++;
            qcom_map[qcom_map_size].udefnum = atoi(c);
        }
//         qcom_map[qcom_map_size].displayname = malloc(sizeof(char)*(strlen(line)+1));
        qcom_map[qcom_map_size].displayname = TStrDupFromChar(line);
        getNextLine(qcf,line,1000);
//         qcom_map[qcom_map_size].printfstring = malloc(sizeof(char)*(strlen(line)+1));
        qcom_map[qcom_map_size].printfstring = TStrDupFromChar(line);
        qcom_map_size++;
    }
    fclose(qcf);
}

void loadServerListFile() {
    FILE * slf=NULL;
    char line[1000];

    slf = fopen("C:\\textclientserverlist.txt","r");
    if (!slf)
        slf = fopen(".\\textclientserverlist.txt","r");
    assert(slf);

    getNextLine(0,0,0);

    while((num_servers < MAX_SERVERS) && getNextLine(slf,line,1000)) {
        char * pauth = strstr(line,":");
        if(!*line || (line[0] == '/' && line[1] == '/'))
            continue;
        assert(pauth);
        pauth[0] = '\0';
        pauth = &(pauth[1]);
        
        // -AB: strdup already mallocs the memory, this is a leak :2005 Jun 09 04:21 PM
//         server_list[num_servers].servername = malloc(sizeof(char)*(strlen(line)+1));
        server_list[num_servers].servername = TStrDupFromChar(line);
//         server_list[num_servers].serverauth = malloc(sizeof(char)*(strlen(pauth)+1));
        server_list[num_servers].serverauth = pauth[0]?TStrDupFromChar(pauth):NULL;
        num_servers++;
    }
    fclose(slf);
}

void loadLoginInfoFile() {
    FILE * lif = NULL;
    char line[1000];
    int num_logins = 0;

    lif = fopen("C:\\textclientlogininfo.txt","r");
    if (!lif)
        lif = fopen(".\\textclientlogininfo.txt","r");
    assert(lif);

    getNextLine(0,0,0);

    while((num_logins < num_servers) && getNextLine(lif,line,1000)) {
        int i;
        char * pcharn;
        char * pclient;
        char * plogin = strstr(line,":");
        char * ppass;
        assert(plogin);
        plogin[0] = '\0';
        plogin = &(plogin[1]);

        pcharn = strstr(plogin,":");
        assert(pcharn);
        pcharn[0] = '\0';
        pcharn = &(pcharn[1]);

        pclient = strstr(pcharn,":");
        if (pclient)
        {
            pclient[0] = '\0';
            pclient = &(pclient[1]);
        }

        ppass = pclient;

        for (i=0; i<num_servers; i++) {
            if (0 == CharTCharStrcmp(line,server_list[i].servername)) {
                server_list[i].loginname = TStrDupFromChar(plogin);
                server_list[i].loginchar = TStrDupFromChar(pcharn);
                server_list[i].password = ppass?TStrDupFromChar(ppass):0;
            }
        }
        num_logins++;
    }
    fclose(lif);
}

DLGTEMPLATE * WINAPI DoLockDlgRes(PTSTR lpszResName) 
{ 
    HRSRC hrsrc = FindResource(NULL, lpszResName, RT_DIALOG); 
    HGLOBAL hglb = LoadResource(g_hInst, hrsrc); 
    return (DLGTEMPLATE *) LockResource(hglb); 
} 


void InsertTab(DLGHDR *pHdr, int resource, void *dlgProc, TCHAR *title, bool showme) {
    TCITEM tie;
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

    tab->apRes        = DoLockDlgRes(MAKEINTRESOURCE(resource));
    tab->dlgProc    = dlgProc;
    lstrcpy(tab->title, title);
    tab->isConnected = FALSE;
    tab->plv = listViewCreate();
    tie.pszText    = title;
    TabCtrl_InsertItem(pHdr->hwndTab, eaSize(&pHdr->eaTabs)-1, &tie);

    // Set Window positions
    tab->hwndDisplay = CreateDialogIndirect(g_hInst, 
        tab->apRes, 
        hDlg, 
        tab->dlgProc);

    listViewInit(tab->plv,ChatboxStringInfo,tab->hwndDisplay,GetDlgItem(tab->hwndDisplay,IDC_TEXT_OUTPUT2));
    ListView_SetColumnWidth(GetDlgItem(tab->hwndDisplay,IDC_TEXT_OUTPUT2),0,800);

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
        OnSelChanged(pHdr->hwndParent);
    }
}


void LoadTabs(DLGHDR *pHdr)
{
    RECT rcTab;
    RECT rcMain;
    DWORD dwDlgBase = GetDialogBaseUnits(); 
    int cxMargin = LOWORD(dwDlgBase) / 4; 
    int cyMargin = HIWORD(dwDlgBase) / 8; 
    int i;

    // Add a tab for each of the child dialog boxes. 

    pHdr->eaTabs = NULL;

    for (i = 0; i < num_servers; i++) {

        if (batchmode.state && !wcsicmp(server_list[i].servername,batchmode.server))
        {
                InsertTab(pHdr, IDD_DLGTEXTTAB, DlgTextTabProc, server_list[i].servername, true);
                SendMessage(GetDlgItem(pHdr->eaTabs[i]->hwndDisplay, IDC_EDIT_AUTHNAME), WM_SETTEXT, 0, (LPARAM)batchmode.auth);
                SendMessage(GetDlgItem(pHdr->eaTabs[i]->hwndDisplay, IDC_EDIT_CHARACTER), WM_SETTEXT, 0, (LPARAM)batchmode.character);
                SendMessage(GetDlgItem(pHdr->eaTabs[i]->hwndDisplay, IDC_EDIT_PASSWORD), WM_SETTEXT, 0, (LPARAM)batchmode.pw);
                SendMessage(GetDlgItem(pHdr->eaTabs[i]->hwndDisplay, IDC_BATCHFILE), WM_SETTEXT, 0, (LPARAM)batchmode.batchfile);
                batchmode.tabid = i;
                batchmode.tabhandle = pHdr->eaTabs[i]->hwndDisplay;
        }
        else
        {
            InsertTab(pHdr, IDD_DLGTEXTTAB, DlgTextTabProc, server_list[i].servername, false);
            SendMessage(GetDlgItem(pHdr->eaTabs[i]->hwndDisplay, IDC_EDIT_AUTHNAME), WM_SETTEXT, 0, (LPARAM)server_list[i].loginname);
            SendMessage(GetDlgItem(pHdr->eaTabs[i]->hwndDisplay, IDC_EDIT_CHARACTER), WM_SETTEXT, 0, (LPARAM)server_list[i].loginchar);
            SendMessage(GetDlgItem(pHdr->eaTabs[i]->hwndDisplay, IDC_EDIT_PASSWORD), WM_SETTEXT, 0, (LPARAM)server_list[i].password);
        }

        {
            TCHAR logfilename[300];
            wsprintf(logfilename, TEXT(".\\textclientlog_%s.txt"),server_list[i].servername);
            SendMessage(GetDlgItem(pHdr->eaTabs[i]->hwndDisplay, IDC_LOGFILE), WM_SETTEXT, 0, (LPARAM)logfilename);
            SendMessage(GetDlgItem(pHdr->eaTabs[i]->hwndDisplay, IDC_LOGCHAT), BM_SETCHECK, BST_CHECKED, 0);
        }
    }

    // Size the dialog box based on the 0th Tab
    GetWindowRect(pHdr->eaTabs[0]->hwndDisplay, &rcTab);
    GetWindowRect(pHdr->hwndParent, &rcMain);

    SetWindowPos(pHdr->hwndParent, NULL, 
        rcMain.left, 
        rcMain.top, 
        (rcTab.right - rcTab.left)+ 2* cxMargin + 2 * GetSystemMetrics(SM_CXDLGFRAME), 
        (rcTab.bottom - rcTab.top) + 2 * cyMargin + 2 * GetSystemMetrics(SM_CYDLGFRAME) + GetSystemMetrics(SM_CYCAPTION), 
        SWP_NOZORDER);

    if (batchmode.state && batchmode.tabid == -1) batchmode.state = BMS_DONE;

}



void DlgTextMainDoDialog() {
    HWND hwndMain = NULL;
    BOOL bRet;
    MSG msg;
    InitCommonControls();
    loadQuickCommandFile();
    loadServerListFile();
    loadLoginInfoFile();
//    DialogBox (g_hInst, (LPCTSTR) (IDD_DLGTEXTMAIN), NULL, (DLGPROC)DlgTextMainProc);
    hwndMain = CreateDialog(g_hInst, (LPCTSTR) (IDD_DLGTEXTMAIN), NULL, (DLGPROC)DlgTextMainProc);
    ShowWindow(hwndMain,SW_SHOW);
    while (bRet = GetMessage(&msg,NULL,0,0)) {
        if (-1 == bRet) return;
        if (msg.message == WM_KEYDOWN) {
            if (!g_pHdr) continue;
            if (msg.wParam == VK_TAB) {
                short ctrl = GetAsyncKeyState(VK_CONTROL);
                short shift = GetAsyncKeyState(VK_SHIFT);
                int i;
                int ntabs = eaSize(&g_pHdr->eaTabs);
                if (ctrl) {
                    for (i=0; i < ntabs; i++) {
                        if (g_pHdr->hwndCurrent == g_pHdr->eaTabs[i]->hwndDisplay)
                            break;
                    }
                    i = shift?i-1:i+1;
                    if (i < 0) i = ntabs-1;
                    if (i >= ntabs) i = 0;
                    TabCtrl_SetCurSel(g_pHdr->hwndTab,i);
                    OnSelChanged(g_pHdr->hwndParent);
                    continue;
                }
/*
    if (showme) {
        TabCtrl_SetCurSel(pHdr->hwndTab, eaSize(&pHdr->eaTabs)-1);
        OnSelChanged(pHdr->hwndParent);
    }
*/
            }
            else if (msg.wParam == VK_RETURN) {
                // They hit return, so do some shit
                HWND cFocus = GetFocus();
                HWND cTab;
                if (!g_pHdr || !g_pHdr->hwndCurrent) continue;
                cTab = g_pHdr->hwndCurrent;
                if ((cFocus == GetDlgItem(cTab, IDC_EDIT_AUTHNAME)) ||
                    (cFocus == GetDlgItem(cTab, IDC_EDIT_PASSWORD)) ||
                    (cFocus == GetDlgItem(cTab, IDC_EDIT_CHARACTER))) {
                        SendMessage(GetDlgItem(cTab, IDC_LOGIN),BM_CLICK,0,0);
                        SetFocus(cFocus);
                        continue;
                }
                if ((GetParent(cFocus)) == GetDlgItem(cTab, IDC_FULL_COMMAND)) {
                    SendMessage(GetDlgItem(cTab, IDC_SEND_COMMAND),BM_CLICK,0,0);
//                    SetFocus(cFocus);
                    continue;
                }
            }
        }
        if (!IsWindow(hwndMain) || !IsDialogMessage(hwndMain,&msg)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
}

void onLoginCommand(HWND tab) 
{
    TCHAR strlogin[500] = TEXT("");
    TCHAR strentry[100];
    DLGHDR_TAB* dlgts = getTabStruct(tab);
    HWND ctab = g_pHdr->hwndCurrent;
    int i;

    if ((dlgts == NULL) || (ctab != tab)) return;

    if (!dlgts->isConnected) {
        
        if (!isValidLogin(tab)) {
            chatprintf(tab,0,CC_LAUNCHER,"[LAUNCHER] Invalid login information!\n");
            if (batchmode.state) batchmode.state = BMS_DONE;
            return;
        }

        if (batchmode.state) batchmode.state = BMS_CONNECTING;

        // Just in case
        wsprintf(strlogin, TEXT("%%s %%d %s%s"), TEXT("-cs "), TEXT("Jered"));

        // log in
        //server
        SendMessage(GetDlgItem(tab, IDC_LOGINGROUP), WM_GETTEXT, 100, (LPARAM)strentry);
        for (i=0; i<num_servers; i++) {
            if (0 == lstrcmp(strentry,server_list[i].servername))
                break;
        }
        assert(i<num_servers);

        // servers without auth are assumed local
        if (server_list[i].serverauth)
        {
            wsprintf(strlogin,TEXT("%%s %%d -auth %s"), server_list[i].serverauth);
        }
        else
        {
            wsprintf(strlogin,TEXT("%%s %%d %s%s"), TEXT("-cs "), server_list[i].servername);
        }

        lstrcat(strlogin, TEXT(" -server \""));
        lstrcat(strlogin,strentry);
        lstrcat(strlogin,TEXT("\" "));
        //authname
        SendMessage(GetDlgItem(tab, IDC_EDIT_AUTHNAME), WM_GETTEXT, 100, (LPARAM)strentry);
        lstrcat(strlogin, TEXT(" -authname "));
        lstrcat(strlogin,strentry);
        lstrcat(strlogin,TEXT(" "));
        //password
        SendMessage(GetDlgItem(tab, IDC_EDIT_PASSWORD), WM_GETTEXT, 100, (LPARAM)strentry);
        lstrcat(strlogin, TEXT(" -password "));
        lstrcat(strlogin,strentry);
        lstrcat(strlogin,TEXT(" "));
        //password
        SendMessage(GetDlgItem(tab, IDC_EDIT_CHARACTER), WM_GETTEXT, 100, (LPARAM)strentry);
        lstrcat(strlogin, TEXT(" -character \""));
        lstrcat(strlogin,strentry);
        lstrcat(strlogin,TEXT("\""));

        //misc flags
        SendMessage(GetDlgItem(tab, IDC_EDIT_VERSION), WM_GETTEXT, 100, (LPARAM)strentry);
        if (!strentry[0])
        {
            lstrcat(strlogin, TEXT(" -selfversion"));
        }

        if (!SendMessage(GetDlgItem(tab,IDC_CONSOLE),BM_GETCHECK,0,0))
        {
            lstrcat(strlogin, TEXT(" -hideconsole"));
        }

        if (SendMessage(GetDlgItem(tab,IDC_COV),BM_GETCHECK,0,0))
        {
            lstrcat(strlogin, TEXT(" -cov"));
        }

        chatprintf(tab,0,CC_LAUNCHER,"[LAUNCHER] Spawning new process\n");

        if (dlgts->ci=launchTextClient(strlogin,tab))
            dlgts->isConnected = TRUE;
    }
    else {
        // log out
        killChild(NULL,dlgts->ci,NULL);
        dlgts->isConnected = FALSE;
    }

}

void onSendFullCommand(HWND tab) {
    char strcmd[4016] = "";
    char tempcmd[ARRAY_SIZE( strcmd )] = "";
    TCHAR ttempcmd[ARRAY_SIZE( tempcmd )/3]; // assuming UTF8 worst case 3 chars per unicode
    int idx;
    int cbsize;
    DLGHDR_TAB* dlgts = getTabStruct(tab);
    HWND cbcmd = GetDlgItem(tab, IDC_FULL_COMMAND);
    if (!dlgts->isConnected) {
        chatprintf(tab,0,CC_LAUNCHER,"[LAUNCHER] You are not currently logged in!\n");
        return;
    }

    SendMessage(cbcmd, WM_GETTEXT, ARRAY_SIZE( ttempcmd ), (LPARAM)ttempcmd);
    idx = SendMessage(cbcmd, CB_FINDSTRINGEXACT, -1, (LPARAM)ttempcmd);
    if (idx == CB_ERR) {
        //This isn't in the list yet
        cbsize = SendMessage(cbcmd, CB_GETCOUNT, 0, 0);
        // if we're full, delete the top one
        if (cbsize >= COMMAND_BUFFER_SIZE) {
            SendMessage(cbcmd, CB_DELETESTRING, 0, 0);
        }
    }
    else {
        // it's in the list already
        SendMessage(cbcmd, CB_DELETESTRING, idx, 0);
    }
    
    SendMessage(cbcmd, CB_INSERTSTRING, 0, (LPARAM)ttempcmd);
//    SendMessage(cbcmd, CB_SETCURSEL, SendMessage(cbcmd, CB_GETCOUNT, 0, 0)-1, 0);
    SendMessage(cbcmd, CB_SETCURSEL, -1, 0);
    SetFocus(cbcmd);
    
    CharFromTChar( tempcmd, ttempcmd, ARRAY_SIZE( tempcmd ));
    sprintf(strcmd,"CMD %s",tempcmd);
    sendCommand(NULL,dlgts->ci,strcmd);
}

void onRunBatchCommand(HWND tab) {
    char strcmd[2004] = "";
    char strline[2000] = "";
    TCHAR bfilename[2000] = TEXT("");
//     char temp[4096];
    DLGHDR_TAB* dlgts = getTabStruct(tab);
    HWND cbbf = GetDlgItem(tab, IDC_BATCHFILE);

    if (!dlgts->isConnected) {
        chatprintf(tab,0,CC_LAUNCHER,"[LAUNCHER] You are not currently logged in!\n");
        dlgts->ci->batchFile = NULL;
        if (batchmode.state) batchmode.state = BMS_DONE;
        return;
    }

    if (dlgts->ci->batchFile == NULL) {
        // We're not yet running the batch
        SendMessage(cbbf, WM_GETTEXT, 2000, (LPARAM)bfilename);
        dlgts->ci->batchFile = _wfopen(bfilename,L"r");
        if (!dlgts->ci->batchFile) {
            chatprintfW(tab,0,CC_LAUNCHER,L"[LAUNCHER] Unable to load file: %s\n",bfilename);
            if (batchmode.state) batchmode.state = BMS_DONE;
            return;
        }
        // got our file open, so reset
        getNextBatchLine(NULL, strline, 2000,dlgts->ci);
    }

    // If we're paused, just get out
    if (SendMessage(GetDlgItem(tab,IDC_PAUSEBATCH),BM_GETCHECK,0,0))
        return;


    if (!getNextBatchLine(dlgts->ci->batchFile, strline, 2000,dlgts->ci)) {
        // Done with the file
        fclose(dlgts->ci->batchFile);
        dlgts->ci->batchFile = NULL;
        if (batchmode.state) batchmode.state = BMS_DONE;
        return;
    }

    sprintf(strcmd,"CMD %s",strline);
    sendCommand(NULL,dlgts->ci,strcmd);
    chatprintf(tab,1,CC_LAUNCHER,"[BATCH] %s",strcmd);
}

void onClientUpdateCommand(HWND tab) {
    DLGHDR_TAB* dlgts = getTabStruct(tab);
    ChildInfo* ci;
    TCHAR statusboxtext[200];
    char normaltext[200];
    //Notified that the text client's status has changed in some way
    if (dlgts->hwndDisplay) {
        ci = dlgts->ci;
        if (!ci) {
            chatprintf(tab,0,CC_LAUNCHER,"[LAUNCHER] WTF, no client you jerk!\n");
            return;
        }
        if (ci->player)
        {
            TCHAR temp[256];
            wsprintf(temp, TEXT("%s"), TCharFromCharStatic(ci->player));
            wsprintf(statusboxtext,TEXT("%s : %s"),temp, TCharFromCharStatic(ci->status_name));
        }
        else {
            TCHAR temp[256];
            SendMessage(GetDlgItem(tab, IDC_EDIT_CHARACTER), WM_GETTEXT, 200, (LPARAM)temp);
            wsprintf(statusboxtext,TEXT("%s : %s"),temp, TCharFromCharStatic(ci->status_name));
        }
        SendMessage(GetDlgItem(tab, IDC_STATUS_BOX), WM_SETTEXT, 0, (LPARAM)statusboxtext);

        sprintf(normaltext,"Map: ");
        if (ci->mapname)
            strcat(normaltext,ci->mapname);
        SendMessage(GetDlgItem(tab, IDC_MAPNAME), WM_SETTEXT, 0, (LPARAM)TCharFromCharStatic(normaltext));

        sprintf(normaltext,"HP: %d / %d",(int)ci->hp[0],(int)ci->hp[1]);
        SendMessage(GetDlgItem(tab, IDC_HITPOINTS), WM_SETTEXT, 0, (LPARAM)TCharFromCharStatic(normaltext));

        if ((ci->status == STATUS_TERMINATED) || 
            (ci->status == STATUS_ERROR) ||
            (ci->status == STATUS_NOPIPE) ||
            (ci->status == STATUS_CRASH)) {
            dlgts->isConnected = FALSE;
            }
        else {
            dlgts->isConnected = TRUE;
        }
        
        if (!dlgts->isConnected) {
            SendMessage(GetDlgItem(tab, IDC_LOGIN), WM_SETTEXT, 0, (LPARAM)TEXT("Login"));
        }
        else {
            SendMessage(GetDlgItem(tab, IDC_LOGIN), WM_SETTEXT, 0, (LPARAM)TEXT("Logout"));
        }
    }
}

void onUserDefinedCommand(HWND tab, int cmd) {
    int i;

    for (i = 0; i < qcom_map_size; i++) {
        if (qcom_map[i].udefnum == cmd) {
            TCHAR str[2000];
            TCHAR tar[1000];
            SendMessage(GetDlgItem(tab, IDC_QUICK_TARGET), WM_GETTEXT, 1000, (LPARAM)tar);
            _stprintf(str,qcom_map[i].printfstring,tar,tar);
            SendMessage(GetDlgItem(tab, IDC_FULL_COMMAND), WM_SETTEXT, 0, (LPARAM)str);
            SendMessage(GetDlgItem(tab, IDC_SEND_COMMAND), BM_CLICK,0,0);
        }
    }
}

VOID WINAPI OnTextMainDialogInit(HWND hwndDlg) {
    DLGHDR *pHdr = (DLGHDR *) GlobalAlloc(GPTR, sizeof(DLGHDR)); 
    g_pHdr = pHdr;

    // Save a pointer to the DLGHDR structure. 
    SetWindowLongPtr(hwndDlg, GWLP_USERDATA, (LONG_PTR)pHdr); 

    // initialize dialog info
    memset(pHdr, 0, sizeof(DLGHDR));
    pHdr->hwndParent = hwndDlg;

    // Create the tab control. 
    pHdr->hwndTab = CreateWindow( 
        WC_TABCONTROL, TEXT(""), 
        WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE | WS_EX_CONTROLPARENT,
        0, 0, 1600, 500, 
        hwndDlg, NULL, g_hInst, NULL 
        ); 
    
    // set tab font to default
    SendMessage(pHdr->hwndTab, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(TRUE, 0)); 

    assert(pHdr->hwndTab);

    LoadTabs(pHdr);

    doInitPipeServer();

    SetTimer(hwndDlg, 0, 500, NULL);

    OnSelChanged(hwndDlg);

    if (batchmode.state == BMS_INIT) onLoginCommand(pHdr->eaTabs[batchmode.tabid]->hwndDisplay);
}

LRESULT CALLBACK DlgTextMainProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    int i=0;

    DLGHDR *pHdr = (DLGHDR*)GetWindowLongPtr(hDlg, GWLP_USERDATA); 

    switch (iMsg) {
        case WM_INITDIALOG:
            OnTextMainDialogInit(hDlg);
            return TRUE;
        case WM_COMMAND:
            switch (LOWORD(wParam))
            {
            case IDOK: // Pass it on down when someone hits Enter
                SendMessage(pHdr->hwndCurrent, iMsg, wParam, lParam);
                return TRUE;
            case IDCANCEL: // Pass it on down when the user clicks the X button
                EndDialog(hDlg, 0);
                PostQuitMessage(0);
                return TRUE;
            }
            break;
        case WM_TIMER:
            {
                // Simple check to make sure that this function isn't called at the same time multiple times
                static int inTimer = 0;
                if (!inTimer) {
                    inTimer=1;
                    // Check the pipe
                    doPeriodicUpdate();
                    // Pass the timer message down to all the tabs
                    for(i=0; i < eaSize(&pHdr->eaTabs); i++)
                        SendMessage(pHdr->eaTabs[i]->hwndDisplay, iMsg, wParam, lParam);

                    // Pass the timer message down to the current tab
//                    SendMessage(pHdr->hwndCurrent, iMsg, wParam, lParam);
                    inTimer=0;
                    return TRUE;
                }
            }
            break;
        case WM_SIZE:
            {
                WORD w = LOWORD(lParam);
                WORD h = HIWORD(lParam);
                RECT rcMain;
                DWORD dwDlgBase = GetDialogBaseUnits(); 
                int cxMargin = LOWORD(dwDlgBase) / 4; 
                int cyMargin = HIWORD(dwDlgBase) / 8; 

                GetWindowRect(hDlg, &rcMain);

                // must keep ALL dialogs consistent
                for(i=0; i < eaSize(&pHdr->eaTabs); i++)
                {
                    // update the size of the dialog box in the tab window
                    SetWindowPos(pHdr->eaTabs[i]->hwndDisplay, NULL, 
                        cxMargin,
                        GetSystemMetrics(SM_CYCAPTION),
                        (rcMain.right - rcMain.left) - (2 * GetSystemMetrics(SM_CXDLGFRAME)), 
                        (rcMain.bottom - rcMain.top) - (2 * GetSystemMetrics(SM_CYDLGFRAME) + 2 * GetSystemMetrics(SM_CYCAPTION)), 
                        0);
        
                }
                return TRUE;
            }
            break;

        case WM_NOTIFY:
            {
                switch (((NMHDR *) lParam)->code) 
                { 
                    case TCN_SELCHANGE: 
                    { 
                        OnSelChanged(hDlg);    // processed when user clics on new tab
                        return TRUE;
                    } 
                }
            }
            break;
    }
    return FALSE;
}

LRESULT CALLBACK DlgTextTabProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam) {
    int i=0;
    DLGHDR_TAB* dlgts = getTabStruct(hDlg);

    switch (iMsg)
    {
    case WM_INITDIALOG:
        {
            //Assume this tab is the last in the earray
            TCHAR * sname = g_pHdr->eaTabs[eaSize(&g_pHdr->eaTabs)-1]->title;
            SetDlgItemText(hDlg,IDC_LOGINGROUP,sname);

            // Add Quick Commands and user-defined buttons
            for (i=0; i<qcom_map_size; i++) {
                SendMessage(GetDlgItem(hDlg, IDC_QUICK_COMMAND), CB_ADDSTRING, 0, (LPARAM)qcom_map[i].displayname);
                if ((qcom_map[i].udefnum >= 0) && (qcom_map[i].udefnum < 6))
                    SendMessage(GetDlgItem(hDlg,IDC_USER0+qcom_map[i].udefnum),WM_SETTEXT,0,(LPARAM)qcom_map[i].displayname);
            }
            SendMessage(GetDlgItem(hDlg, IDC_QUICK_COMMAND), CB_SETCURSEL, 0, 1);
            
            return FALSE;
        }
    case WM_NOTIFY:
        return listViewOnNotify(dlgts->plv,wParam,lParam,NULL);
    case WM_CTLCOLORLISTBOX:
        return FALSE;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDC_USER5:
            i++;
        case IDC_USER4:
            i++;
        case IDC_USER3:
            i++;
        case IDC_USER2:
            i++;
        case IDC_USER1:
            i++;
        case IDC_USER0:
            onUserDefinedCommand(hDlg,i);
            return TRUE;
        case IDC_LOGIN:
            onLoginCommand(hDlg);
            return TRUE;
        case IDC_SEND_COMMAND:
            onSendFullCommand(hDlg);
            return TRUE;
        case IDC_CLIENT_UPDATE:
            onClientUpdateCommand(hDlg);
            return TRUE;
        case IDC_RUNBATCH:
            onRunBatchCommand(hDlg);
            return TRUE;
        case IDC_TOGGLE_TEAM: 
            {
                char strcmd[200];
                BOOL tt = SendMessage(GetDlgItem(hDlg,IDC_TOGGLE_TEAM),BM_GETCHECK,0,0);
                sprintf(strcmd,"MODE %s%s",tt?"+":"-","TEAMACCEPT");
                sendCommand(NULL,dlgts->ci,strcmd);
                return TRUE;
            }
        case IDC_TOGGLE_SG:
            {
                char strcmd[200];
                BOOL tt = SendMessage(GetDlgItem(hDlg,IDC_TOGGLE_SG),BM_GETCHECK,0,0);
                sprintf(strcmd,"MODE %s%s",tt?"+":"-","SUPERGROUPACCEPT");
                sendCommand(NULL,dlgts->ci,strcmd);
                return TRUE;
            }
        case IDC_CONSOLE:
            {
                BOOL tt = SendMessage(GetDlgItem(hDlg,IDC_CONSOLE),BM_GETCHECK,0,0);
                if (dlgts->ci)
                {
                    if (!dlgts->ci->hwnd) dlgts->ci->hwnd = hwndFromProcessID(dlgts->ci->pinfo.dwProcessId);
                    if (dlgts->ci->hwnd) ShowWindow(dlgts->ci->hwnd,tt?SW_SHOW:SW_HIDE);
                }
                return TRUE;
            }
        case IDC_CLEAR:
            {
                ListView_DeleteAllItems(GetDlgItem(hDlg,IDC_TEXT_OUTPUT2));
            }
        }
    case IDC_COPY:
        {
        int s_top=0,s_cur=0;
        char * outstr = calloc(1,1);
        char * tempoutstr;
        chatbox_string *pcbs;
        HWND hLV = GetDlgItem(hDlg,IDC_TEXT_OUTPUT2);
        int sc = ListView_GetSelectedCount(hLV);
        int tc = ListView_GetItemCount(hLV);

        if (sc < 1) return TRUE;

        s_top=0;
        pcbs = listViewGetItem(dlgts->plv,s_top);
        while((s_cur < sc) && (s_top<tc)) {
            pcbs = listViewGetItem(dlgts->plv,s_top++);
            if (listViewIsSelected(dlgts->plv,pcbs)) {
                tempoutstr = outstr;
                outstr = calloc(2+strlen(pcbs->str)+strlen(tempoutstr),1);
                sprintf(outstr,"%s%s\n",tempoutstr,pcbs->str);
                free(tempoutstr);
                s_cur++;
            }
        }
        winCopyToClipboard(outstr);
        free(outstr);
        return TRUE;
        }
    case WM_TIMER:
        periodicUpdate(hDlg);
        break;
    }
    return FALSE;
}


// OnSelChanged - processes the TCN_SELCHANGE notification. 
// hwndDlg - handle to the parent dialog box. 
// Handles the message for changing tabs
VOID WINAPI OnSelChanged(HWND hwndDlg) 
{ 
    DLGHDR *pHdr = (DLGHDR*)GetWindowLongPtr(hwndDlg, GWLP_USERDATA); 

    int iSel = TabCtrl_GetCurSel(pHdr->hwndTab);

    if (!pHdr->eaTabs[iSel]->hwndDisplay) return;

    // hide the current window
    if(pHdr->hwndCurrent)
        ShowWindow(pHdr->hwndCurrent, FALSE);

    assert(pHdr->eaTabs[iSel]->hwndDisplay);

    pHdr->hwndCurrent = pHdr->eaTabs[iSel]->hwndDisplay;

    ShowWindow(pHdr->hwndCurrent, TRUE);
}

void stripstrcpy(char * str, const char * src)
{
    char *open,*close;
    const char *front = src;
    strcpy(str,"");

    while (front != NULL
           && (open = strchr(front,'<'))
           && (!strncmp(open,"<color ",7)
               || !strncmp(open,"<bgcolor ",9)))
    {
        open[0]=0;
        strcat(str,front);
        open[0]='<';
        close = strchr(open,'>');
        front = (close) ? &close[1] : NULL;
    }
    if (front) strcat(str,front);
}

char* tabVersionRequest(HWND tab)
{
    TCHAR strentry[100];
    static char charstrentry[300];
    SendMessage(GetDlgItem(tab, IDC_EDIT_VERSION), WM_GETTEXT, 100, (LPARAM)strentry);
    WideToUTF8StrConvert(strentry,charstrentry,300);
    return charstrentry;
}

// Does a printf to the chat window
int chatprintf(HWND tab, int chattype, COLORREF ccfore, COLORREF ccback, char *fmt, ...) {
    HWND hLB = GetDlgItem(tab, IDC_TEXT_OUTPUT2);
    DLGHDR_TAB* dlgts = getTabStruct(tab);
    LRESULT index = 0;
    char str[32000];
    char *pstr, *eopstr;
    int oldindex, oldtop;
    va_list ap;
    int mnum=0;
    BOOL bscroll = (hLB != GetFocus()) && (!SendMessage(GetDlgItem(tab,IDC_LOCKCHAT),BM_GETCHECK,0,0));
    BOOL bauto = SendMessage(GetDlgItem(tab,IDC_TOGGLE_AUTOREPLY),BM_GETCHECK,0,0);
    BOOL log = SendMessage(GetDlgItem(tab,IDC_LOGCHAT),BM_GETCHECK,0,0);
    

    va_start(ap, fmt);
    vsprintf(str, fmt, ap);
    va_end(ap);

    SendMessage(hLB, LB_SETHORIZONTALEXTENT, 3200, 0);

    pstr = str;

    if(log)
    {
        TCHAR logfilename[1000] = {0};
        SendMessage(GetDlgItem(tab, IDC_LOGFILE), WM_GETTEXT, 1000, (LPARAM)logfilename);
        if(wcslen(logfilename)>0)
        {
            FILE *logfile = _wfopen(logfilename,L"a");
            if(logfile)
            {
                SYSTEMTIME st;
                GetLocalTime(&st);
                fprintf(logfile,"%02d-%02d-%02d %02d:%02d:%02d : %s%s", st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond,str,str[strlen(str)-1]!='\n'?"\n":"");
                fclose(logfile);
            }
        }
        
    }

    while (eopstr = strstr(pstr,"\n")) {
        chatbox_string *pcbs = malloc(sizeof(chatbox_string));
        eopstr[0] = '\0';

        oldindex = (int)SendMessage(hLB, LB_GETCARETINDEX, 0, 0);
        oldtop = ListView_GetTopIndex(hLB);
        //(int)SendMessage(hLB, LB_GETTOPINDEX, 0, 0);

//        index = SendMessage(hLB, LB_ADDSTRING, 0, (LPARAM)pstr);

        pcbs->str = calloc(strlen(pstr)+1,1);
        stripstrcpy(pcbs->str,pstr);
        pcbs->mtype = chattype;
        pcbs->mnum = mnum++;
        index = listViewAddItem(dlgts->plv,pcbs);
        listViewSetItemColor(dlgts->plv,pcbs,ccfore,ccback);

        if (bscroll) { // scroll down
            ListView_EnsureVisible(hLB,index,FALSE);
/*            SendMessage(hLB, LB_SETTOPINDEX, index, (LPARAM)0);
            if (LB_ERR==SendMessage(hLB, LB_SETCARETINDEX, index, (LPARAM)0)) {
                // Single selection or no-selection list box
                SendMessage(hLB, LB_SETCURSEL, index, (LPARAM)0);
            }
*/
        } else {
            ListView_EnsureVisible(hLB,oldtop,FALSE);
//            SendMessage(hLB, LB_SETCARETINDEX, oldindex, (LPARAM)0);
//            SendMessage(hLB, LB_SETTOPINDEX, oldtop, (LPARAM)0);
        }
        pstr = &(eopstr[1]);
    }

    if (strlen(pstr)>0) {
        chatbox_string *pcbs = malloc(sizeof(chatbox_string));
        oldindex = (int)SendMessage(hLB, LB_GETCARETINDEX, 0, 0);
        oldtop = ListView_GetTopIndex(hLB);
        //(int)SendMessage(hLB, LB_GETTOPINDEX, 0, 0);

//        index = SendMessage(hLB, LB_ADDSTRING, 0, (LPARAM)pstr);
        pcbs->str = calloc(strlen(pstr)+1,1);
        stripstrcpy(pcbs->str,pstr);
        pcbs->mtype = chattype;
        pcbs->mnum = mnum++;
        index = listViewAddItem(dlgts->plv,pcbs);
        listViewSetItemColor(dlgts->plv,pcbs,ccfore,ccback);

        if (bscroll) { // scroll down
            ListView_EnsureVisible(hLB,index,FALSE);
/*            SendMessage(hLB, LB_SETTOPINDEX, index, (LPARAM)0);
            if (LB_ERR==SendMessage(hLB, LB_SETCARETINDEX, index, (LPARAM)0)) {
                // Single selection or no-selection list box
                SendMessage(hLB, LB_SETCURSEL, index, (LPARAM)0);
            }
*/
        } else {
            ListView_EnsureVisible(hLB,oldtop,FALSE);
//            SendMessage(hLB, LB_SETCARETINDEX, oldindex, (LPARAM)0);
//            SendMessage(hLB, LB_SETTOPINDEX, oldtop, (LPARAM)0);
        }
    }

    if (bauto 
        && dlgts->isConnected
        && chattype == 107 
        && strstr(str, "[TELL] -->") == NULL)
    {
        char strauto[2000] = "";
        char strcmd[2048];
        char strname[32];
        char *strcolon = strchr(str,':');
        strcolon[0] = '\0';
        strcpy(strname,&str[7]);
        strcolon[0] = ':';
        SendMessageW(GetDlgItem(tab,IDC_EDIT_AUTOREPLY), WM_GETTEXT, 2000, (LPARAM)strauto);
        sprintf(strcmd,"CMD TELL %s, %s",strname,strauto);
        sendCommand(NULL,dlgts->ci,strcmd);
    }

    return (int)index;
}

int chatprintfW(HWND tab, int chattype, COLORREF ccfore, COLORREF ccback, wchar_t *fmt, ...)
{
    va_list ap;
    int n;
    wchar_t *str;

    va_start(ap, fmt);
    n = _vscwprintf( fmt, ap ) + 2;
    str = _alloca(n*sizeof(*str));
    if( verify(str) )
    {
#if _MSC_VER < 1400
        vswprintf(str, fmt, ap);
#else
        vswprintf_s(str, n, fmt, ap);
#endif
    }
    va_end(ap);

    if( str )
    {
        return chatprintf(tab,chattype, ccfore, ccback, WideToUTF8StrTempConvert(str,NULL));
    }
    return 0;
}

// Returns if the is text in all three login edits
BOOL isValidLogin(HWND tab) {
    char strentry[100];

    SendMessage(GetDlgItem(tab, IDC_EDIT_AUTHNAME), WM_GETTEXT, 100, (LPARAM)strentry);
    if (strcmp(strentry,"")==0) return FALSE;
    SendMessage(GetDlgItem(tab, IDC_EDIT_PASSWORD), WM_GETTEXT, 100, (LPARAM)strentry);
    if (strcmp(strentry,"")==0) return FALSE;
    SendMessage(GetDlgItem(tab, IDC_EDIT_CHARACTER), WM_GETTEXT, 100, (LPARAM)strentry);
    if (strcmp(strentry,"")==0) return FALSE;
    return TRUE;
}

// Called periodically to update various interface parts (text boxes, etc)
void periodicUpdate(HWND tab) {
    DLGHDR_TAB* dlgts = getTabStruct(tab);
    HWND cfocus = GetFocus();
    TCHAR strcmd[2000] = TEXT("");

    if (batchmode.state == BMS_CONNECTING && batchmode.tabhandle == tab)
    {
        if (!dlgts->ci 
            || dlgts->ci->status == STATUS_TERMINATED 
            || dlgts->ci->status == STATUS_ERROR 
            || dlgts->ci->status == STATUS_NOPIPE 
            || dlgts->ci->status == STATUS_CRASH) 
        {
            batchmode.state = BMS_DONE;
        }
        else
        {
            if (dlgts->ci->status == STATUS_RUNNING)
            {
                batchmode.state = BMS_BATCH;
                onRunBatchCommand(tab);
            }
        }
    }

    if (batchmode.state == BMS_DONE 
        && (batchmode.tabhandle == tab || batchmode.tabid < 0))
    {
        batchmode.logouttimer--;
        if (!batchmode.debug && batchmode.logouttimer <= 0)
        {
            if (dlgts->ci && dlgts->isConnected)
                onLoginCommand(tab); // full logout
            exit(0);
        }
    }

    // If this tab is running a batch, keep running it
    if (dlgts->ci && dlgts->ci->batchFile != NULL)
        onRunBatchCommand(tab);

    // If we're messing around with the quick command stuff, update that
    if ((cfocus == GetDlgItem(tab,IDC_QUICK_TARGET)) ||
        (GetParent(cfocus) == GetDlgItem(tab,IDC_QUICK_COMMAND))) 
    {
        buildQuickCommand(tab,strcmd);
        SendMessage(GetDlgItem(tab, IDC_FULL_COMMAND), WM_SETTEXT, 0, (LPARAM)strcmd);
    }
}

// Constructs the quickcommand into strtarg
void buildQuickCommand(HWND tab, TCHAR *strtarg) {
    TCHAR qt[1000];
    TCHAR qc[1000];
    int i;

    SendMessage(GetDlgItem(tab, IDC_QUICK_TARGET), WM_GETTEXT, 1000, (LPARAM)qt);
    SendMessage(GetDlgItem(tab, IDC_QUICK_COMMAND), WM_GETTEXT, 1000, (LPARAM)qc);

    for (i=0; i<qcom_map_size; i++) {
        if (lstrcmp(qc,qcom_map[i].displayname) == 0) {
            lstrcpy(qc,qcom_map[i].printfstring);
        }
    }

    _stprintf(strtarg,qc,qt,qt);
}

// Gets the info struct for the specified tab
DLGHDR_TAB* getTabStruct(HWND tab) {
    int i;
    for(i=0; i < eaSize(&g_pHdr->eaTabs); i++) {
        if (g_pHdr->eaTabs[i]->hwndDisplay == tab) return g_pHdr->eaTabs[i];
    }
    return NULL;
}
