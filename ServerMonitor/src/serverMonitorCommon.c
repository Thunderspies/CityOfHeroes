#include <utilitieslib/components/BitStream.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/network/net_packet.h>
#include <utilitieslib/network/net_version.h>
#include <utilitieslib/network/sock.h>
#include <utilitieslib/utils/error.h>
#include <utilitieslib/utils/file.h>
#include <utilitieslib/utils/log.h>
#include <utilitieslib/utils/MemoryMonitor.h>
#include <utilitieslib/utils/process_util.h>
#include <utilitieslib/utils/textparser.h>
#include <utilitieslib/utils/textparserUtils.h>
#include <utilitieslib/utils/tokenstore.h>
#include <utilitieslib/utils/UnitSpec.h>
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/WinTabUtil.h>
#include <utilitieslib/utils/winutil.h>

#include <conio.h>

#include "chatMonitor.h"
#include "processMonitor.h"
#include "resource.h"
#include "serverMonitor.h"
#include "serverMonitorCommon.h"
#include "serverMonitorCmdRelay.h"
#include "serverMonitorEnts.h"
#include "serverMonitorListener.h"
#include "shardMonitor.h"
#include "shardMonitorCmdRelay.h"
#include "shardMonitorComm.h"
#include "updateServerList.h"

HINSTANCE   g_hInst=NULL;                    // instance handle
HWND g_hDlg=NULL;
RegReader rr = 0;
int name_count=0;
char namelist[256][256];
int g_shardmonitor_mode=0;
int g_are_executables_in_bin=0;
bool g_primaryServerMonitor = true;
bool g_shardrelay_mode = false;

// maximum # of entries in a combo box (read from registry)
#define MAX_COMBO_BOX_ENTRIES 15



void setStatusBar(HWND hStatusBar, int elem, const char *fmt, ...) {
    char str[1000];
    va_list ap;

    va_start(ap, fmt);
    vsprintf(str, fmt, ap);
    va_end(ap);

    SendMessage(hStatusBar, SB_SETTEXT, elem, (LPARAM)str);
}

void initRR() {
    if (!rr) {
        rr = createRegReader();
        initRegReader(rr, "HKEY_CURRENT_USER\\SOFTWARE\\Cryptic\\ServerMonitor");
    }
}
const char *regGetString(const char *key, const char *deflt) {
    static char buf[512];
    initRR();
    strcpy(buf, deflt);
    rrReadString(rr, key, buf, 512);
    return buf;
}

void regPutString(const char *key, const char *value) {
    initRR();
    rrWriteString(rr, key, value);
}

void regDeleteString(const char *key) {
    initRR();
    rrDelete(rr, key);
}

int regGetInt(const char *key, int deflt) {
    static int value;
    initRR();
    value = deflt;
    rrReadInt(rr, key, &value);
    return value;
}

S64 regGetInt64(const char *key, S64 deflt) {
    static S64 value;
    initRR();
    value = deflt;
    rrReadInt64(rr, key, &value);
    return value;
}

void regPutInt(const char *key, int value) {
    initRR();
    rrWriteInt(rr, key, value);
}

void regPutInt64(const char *key, S64 value) {
    initRR();
    rrWriteInt64(rr, key, value);
}

void getNameList(const char * category) {
    char key[256];
    char *badvalue="BadValue";
    const char *name;
    int i;
    name_count = 0;
    for(i=0; i < MAX_COMBO_BOX_ENTRIES; i++) {
        sprintf(key, "%s%d", category, i);
        name = regGetString(key, badvalue);
        if (strcmp(name, badvalue) != 0)
            strcpy(namelist[name_count++], name);
    }
}


void addNameToList(const char * category, char *name) {
    int i;
    char key[256];

    // Reg. is ready every time, since this is now shared by "relay" and "server monitor" dialogs
    getNameList(category);

    if(name_count >= MAX_COMBO_BOX_ENTRIES)
        return;

    for (i=0; i<name_count; i++) {
        if (stricmp(namelist[i], name)==0)
            return;
    }

    sprintf(key, "%s%d", category, name_count);
    regPutString(key, name);

    // might not be needed now, since registry is ready every time
    // this function is called
    strcpy(namelist[name_count++], name);
}


void removeNameFromList(const char * category, char *name) {
    int i;
    char key[256];

    // Reg. is read every time, since this is now shared by "relay" and "server monitor" dialogs
    getNameList(category);

    for (i=0; i<name_count; i++) {
        if (stricmp(namelist[i], name)==0)
        {
            //remove the name
            sprintf(key, "%s%d", category, i);
            regDeleteString(key);
        }
    }
}

BOOL SetDlgItemInt64(IN HWND hDlg, IN int nIDDlgItem, IN S64 uValue, IN BOOL bSigned)
{
    char buf[64];
    sprintf_s(SAFESTR(buf), "%I64d", uValue);
    return SetDlgItemTextA(hDlg, nIDDlgItem, buf);
}

S64 GetDlgItemInt64(IN HWND hDlg, IN int nIDDlgItem, OUT BOOL *lpTranslated, IN BOOL bSigned)
{
    char buf[64];
    S64 ret;
    GetDlgItemTextA(hDlg, nIDDlgItem, buf, ARRAY_SIZE(buf)-1);
    errno = 0;
    ret = _atoi64(buf);
    if (lpTranslated)
        *lpTranslated = errno?FALSE:TRUE;
    return ret;
}

static void initSingleElement(HWND hDlg, char *base, VarMap *mapping)
{
    BOOL b;
    char stemp[128];
    char stemp2[128];
    bool setit=false;

    if (TOK_GET_TYPE(mapping->parse.type) == TOK_BOOL_X ||
        TOK_GET_TYPE(mapping->parse.type) == TOK_BOOLFLAG_X) // checkbox messages
    {
        bool val = TokenStoreGetU8(&mapping->parse, 0, base, 0);
        b = (int)SendMessageA(GetDlgItem(hDlg, mapping->id), BM_GETCHECK, 0, 0)? 1: 0;
        if (b != val) {
            SendMessageA(GetDlgItem(hDlg, mapping->id), BM_SETCHECK, val, 0);
        }
    }
    else // all other types just set a simple string
    {
        stemp[0] = stemp2[0] = 0;
        TokenToSimpleString(&mapping->parse, 0, base, SAFESTR(stemp), 0);
        GetDlgItemTextA(hDlg, mapping->id, SAFESTR(stemp2));
        if (strcmp(stemp, stemp2))
            SetDlgItemTextA(hDlg, mapping->id, stemp);
    }
}

void loadValuesFromReg(HWND hDlg, void *vpbase, VarMap mapping[], int count) {
    int i;
    char *base = vpbase;
    for (i=0; i<count; i++) if (mapping[i].parse.name && mapping[i].parse.name[0]) {
        TokenizerParseInfo* column = &mapping[i].parse;
        switch (TOK_GET_TYPE(mapping[i].parse.type)) {
            case TOK_INT64_X:
                TokenStoreSetInt64(column, 0, base, 0, regGetInt64(mapping[i].parse.name, TokenStoreGetInt64(column, 0, base, 0)));
                initSingleElement(hDlg, base, &mapping[i]);
                break;
            case TOK_INT16_X:
                TokenStoreSetInt16(column, 0, base, 0, regGetInt(mapping[i].parse.name, TokenStoreGetInt16(column, 0, base, 0)));
                initSingleElement(hDlg, base, &mapping[i]);
                break;
            case TOK_INT_X:
                TokenStoreSetInt(column, 0, base, 0, regGetInt(mapping[i].parse.name, TokenStoreGetInt(column, 0, base, 0)));
                initSingleElement(hDlg, base, &mapping[i]);
                break;
            case TOK_BOOL_X:
                TokenStoreSetU8(column, 0, base, 0, regGetInt(mapping[i].parse.name, TokenStoreGetU8(column, 0, base, 0)));
                initSingleElement(hDlg, base, &mapping[i]);
                break;
            case TOK_F32_X:
                TokenStoreSetF32(column, 0, base, 0, (F32)regGetInt(mapping[i].parse.name, (int)TokenStoreGetF32(column, 0, base, 0)));
                initSingleElement(hDlg, base, &mapping[i]);
                break;
            case TOK_STRING_X:
                TokenStoreSetString(column, 0, base, 0, (char*)regGetString(mapping[i].parse.name, TokenStoreGetString(column, 0, base, 0)), NULL, NULL);
                initSingleElement(hDlg, base, &mapping[i]);
                break;
        }
    }
}

void saveValuesToReg(HWND hDlg, void *vpbase, VarMap mapping[], int count) {
    int i;
    char *base = vpbase;
    for (i=0; i<count; i++) if (mapping[i].parse.name && mapping[i].parse.name[0]) {
        TokenizerParseInfo* column = &mapping[i].parse;
        switch (TOK_GET_TYPE(mapping[i].parse.type)) {
            case TOK_INT64_X:
                regPutInt64(mapping[i].parse.name, TokenStoreGetInt64(column, 0, base, 0));
                break;
            case TOK_INT16_X:
                regPutInt(mapping[i].parse.name, TokenStoreGetInt16(column, 0, base, 0));
                break;
            case TOK_INT_X:
                regPutInt(mapping[i].parse.name, TokenStoreGetInt(column, 0, base, 0));
                break;
            case TOK_BOOL_X:
                regPutInt(mapping[i].parse.name, TokenStoreGetU8(column, 0, base, 0));
                break;
            case TOK_F32_X:
                regPutInt(mapping[i].parse.name, (int)TokenStoreGetF32(column, 0, base, 0));
                break;
            case TOK_STRING_X:
                regPutString(mapping[i].parse.name, TokenStoreGetString(column, 0, base, 0));
                break;
        }
    }
}

void initText(HWND hDlg, void *base, VarMap mapping[], int count)
{
    int i;
    for (i=0; i<count; i++) if (mapping[i].flags & VMF_WRITE) {
        initSingleElement(hDlg, base, &mapping[i]);
    }
}

void getText(HWND hDlg, void *vpbase, VarMap mapping[], int count)
{
    BOOL b;
    int i;
    char oldval[128];
    char stemp[128];
    bool bChanged = false;
    char *base = vpbase;

    for (i=0; i<count; i++) if (mapping[i].flags & VMF_READ) 
    {
        if (TOK_GET_TYPE(mapping[i].parse.type) == TOK_BOOL_X ||
            TOK_GET_TYPE(mapping[i].parse.type) == TOK_BOOLFLAG_X) // checkbox control
        {
            b = (int)SendMessage(GetDlgItem(hDlg, mapping[i].id), BM_GETCHECK, 0, 0)? 1: 0;
            if (b != TokenStoreGetU8(&mapping[i].parse, 0, base, 0))
            {
                bChanged = true;
                TokenStoreSetU8(&mapping[i].parse, 0, base, 0, b);
            }
        }
        else // all other token types are just text controls
        {
            TokenToSimpleString(&mapping[i].parse, 0, base, SAFESTR(oldval), 0);
            GetDlgItemTextA(hDlg, mapping[i].id, SAFESTR(stemp));
            if (strcmp(oldval, stemp))
            {
                bChanged = true;
                TokenFromSimpleString(&mapping[i].parse, 0, base, stemp);
            }
        }
    }
    if (bChanged) {
        saveValuesToReg(hDlg, base, mapping, count);
    }
}





void LoadTabs(HWND hwndDlg)
{
    DLGHDR *pHdr = (DLGHDR *) GetWindowLongPtr (hwndDlg, GWLP_USERDATA); 
    pHdr->eaTabs = NULL;

    if (g_shardmonitor_mode) {
        TabDlgInsertTab(g_hInst, pHdr, IDD_DLG_SHARDMON, DlgShardMonProc, "ShardMonitor", false, 0);
        TabDlgInsertTab(g_hInst, pHdr, IDD_DLG_UPDATESERVERS, DlgUpdateServerListProc, "UpdateServers", false, 0);

        // must enable shard relay mode with command line arg "-shardrelay"
        if(g_shardrelay_mode)
            TabDlgInsertTab(g_hInst, pHdr, IDD_DLG_CMDRELAY, DlgShardRelayProc, "Relay", false, 0);
    }
    else {
        TabDlgInsertTab(g_hInst, pHdr, IDD_DLG_SVRMON, DlgSvrMonProc, "DbServer", false, 0);

        if (g_primaryServerMonitor) {

            TabDlgInsertTab(g_hInst, pHdr, IDD_DLG_CMDRELAY, DlgCmdRelayProc, "Relay", false, 0);

            TabDlgInsertTab(g_hInst, pHdr, IDD_DLG_PROCESSMON, DlgProcessMonProc, "Processes", false, 0);
        }

        TabDlgInsertTab(g_hInst, pHdr, IDD_DLG_CHATMON, DlgChatMonProc, "ChatServer", false, 0);
    }
}




void svrMonParseArgs(LPSTR lpCmdLine)
{
    if (strstri(lpCmdLine, "-console")!=0) { 
        newConsoleWindow();
    }
    if (strstri(lpCmdLine, "-shardmonitor")!=0) {
        g_shardmonitor_mode=1;
        g_primaryServerMonitor = 0;
    }
    if (strstri(lpCmdLine, "-connect")!=0) {
        g_state.connection_expected=1;
    }
    if (strstri(lpCmdLine, "-server ")!=0) {
        char *start = strstri(lpCmdLine, "-server ") + strlen("-server ");
        Strncpyt(g_state.dbserveraddr, start);
        if (strchr(g_state.dbserveraddr, ' ')) {
            *strchr(g_state.dbserveraddr, ' ')=0;
        }
    }
    if (strstri(lpCmdLine, "-chatconnect")!=0) {
        chatSetAutoConnect(1);;
    }
    if (strstri(lpCmdLine, "-autostart")!=0) {
        g_state.autostart_expected=1;
    }
    if(strstri(lpCmdLine, "-relaydev")!=0){
        g_bRelayDevMode=TRUE;
    }
    if(strstri(lpCmdLine, "-shardrelay")!=0){
        g_shardrelay_mode=TRUE;
    }
    if(strstri(lpCmdLine, "-shardrelay")!=0){
        g_shardrelay_mode=TRUE;
    }
    if(strstri(lpCmdLine, "-killprev")!=0){
        KillAllEx("ServerMonitor", true);
        Sleep(4000);    // necessary to allow networking connections to close on old server monitor
    }
    if(strstri(lpCmdLine, "-packetdebug")!=0){
        pktSetDebugInfo();
    }
    
    if(strstri(lpCmdLine, "-debug") !=0 )
        g_state.debug = true;
}


LRESULT CALLBACK DlgMainProc (HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    int i=0;

    DLGHDR *pHdr = (DLGHDR *) GetWindowLongPtr(hDlg, GWLP_USERDATA); 

    switch (iMsg)
    {
    case WM_INITDIALOG:
        {
            InitCommonControls();
            TabDlgInit(g_hInst, hDlg);
            LoadTabs(hDlg);
            TabDlgFinalize(hDlg);
        }
        return FALSE;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDOK: // Pass it on down when someone hits Enter
            SendMessage(pHdr->hwndCurrent, iMsg, wParam, lParam);
            break;
        case IDCANCEL: // Pass it on down when the user clicks the X button
            EndDialog(hDlg, 0);
            PostQuitMessage(0);
            break;
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
        }
        break;

    case WM_NOTIFY:
        {
            switch (((NMHDR *) lParam)->code) 
            { 
                case TCN_SELCHANGE: 
                { 
                    TabDlgOnSelChanged(hDlg);    // processed when user clics on new tab
                } 
            }
        }
        break;
    }
    return FALSE;
}

static bool should_quit=false;
static int should_quit_param=0;
bool serverMonitorExiting(void)
{
    return should_quit;
}

void serverMonitorMessageLoop(void)
{
    MSG msg={0};
    BOOL bRet=0;
    do {
        bRet = GetMessage( &msg, NULL, 0, 0 );
        if (bRet == 0) {
            should_quit = true;
            should_quit_param = msg.wParam;
        } else { 
            if (bRet == -1)
            {
                // handle the error and possibly exit
            }
            else 
            {
                // Check all dialogs
                /*            bool bHandled=false;
                for (int i=0; !bHandled && i<eaSize(eaDlgs); i++) {
                g_hDlg = eaGet(eaDlgs, i);
                if (IsDialogMessage(g_hDlg, &msg)) {
                bHandled=true;
                }
                }*/
                if (IsDialogMessage(g_hDlg, &msg)) {
                } else if (smentsHook(&msg)) {
                } else {
                    TranslateMessage(&msg); 
                    DispatchMessage(&msg); 
                }
            }
            if (_kbhit()) {
                char c = _getch();
                switch(c) {
                case 'm':
                    memMonitorDisplayStats();
                    break;
                }
            }
        } 
    } while (!should_quit && PeekMessage(&msg, NULL, 0, 0, 0) != 0);
}

void serverMonitorOuterLoop(void)
{
    while (!should_quit) {
        serverMonitorMessageLoop();
    }
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    int quit=0;

    g_hInst = hInstance;

    memCheckInit();

    bsAssertOnErrors(true);

    logSetDir("ServerMonitor");

    svrMonParseArgs(lpCmdLine);

    sockStart();
    packetStartup(0,0);
    setDefaultNetworkVersion(4); // Only until Aaron gets a fix in to handle connecting V5 to V4 automatically.

    if (!g_shardmonitor_mode) { // If we're running as a shard monitor, we don't want to accept incoming connections from other shard monitors!
        if (fileExists("./bin/Ouroboros.exe")) {
            g_are_executables_in_bin = 1;
            serverMonitorListenThreadBegin();
            svrMonShardMonCommInit();
        } else if (fileExists("./Ouroboros.exe")) {
            serverMonitorListenThreadBegin();
            svrMonShardMonCommInit();
        } else {
            MessageBoxA(NULL, "The ServerMonitor appears to have been started from an invalid path (can't find Ouroboros.exe), disabling Processes and CmdRelay tabs.", "Wrong Working Directory", MB_OK);
            g_primaryServerMonitor = false;
        }
    }

    g_hDlg = CreateDialogA(hInstance, (LPCSTR)(intptr_t)(IDD_DLG_MAIN), NULL, (DLGPROC)DlgMainProc); 
    assert(g_hDlg);
    ShowWindow(g_hDlg, SW_SHOW);
    {
        HICON hIcon  = LoadIconA(GetModuleHandleA(NULL), MAKEINTRESOURCEA(g_shardmonitor_mode?IDI_ICON2:IDI_ICON1));
        if(hIcon) {
            SendMessageA(g_hDlg, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
            SendMessageA(g_hDlg, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
        }
    }
    if (g_shardmonitor_mode) {
        SendMessageA(g_hDlg, WM_SETTEXT, 0, (LPARAM)"ShardMonitor");
    }

    serverMonitorOuterLoop();

    // Return the exit code to the system. 
    return should_quit_param; 
}
