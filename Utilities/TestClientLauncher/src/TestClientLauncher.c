//#include <utilitieslib/stdtypes.h>
#include <utilitieslib/utils/wininclude.h>
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/utils/sysutil.h>

#include "resource.h"
#include "TestClientLauncher.h"
#include "DlgMain.h"
#include "DlgText.h"
#include <utilitieslib/utils/RegistryReader.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/assert/assert.h>
#include <utilitieslib/components/StashTable.h>
#include "PipeServer.h"
#include <utilitieslib/utils/error.h>
#include <utilitieslib/utils/file.h>
#include <process.h>
#include <crtdbg.h>
#include <commctrl.h>
#include <time.h>
#include <utilitieslib/utils/textparser.h>
#include <utilitieslib/utils/ListView.h>
#include <utilitieslib/version/AppVersion.h>
#include <utilitieslib/utils/MemoryMonitor.h>
#include <utilitieslib/utils/StringUtil.h>
#include "tchar.h"


/*
typedef enum {
    STATUS_TERMINATED = 0, //Process is not running
    STATUS_LOADING, // Loading resources
    STATUS_RUNNING, // Running in the game normally
    STATUS_CONNECTING, // Connecting to a server
    STATUS_QUEUED, // Waiting in queue
    STATUS_ERROR, // Error has occured
    STATUS_DEAD, // Character is dead (waiting to resurrect, etc)
    STATUS_SLAVE_LAUNCHER,
    STATUS_NOPIPE, // Pipe has been disconnected
    STATUS_RANDOM_DISCONNECT, // Testclient intentionally randomly disconnected
    STATUS_RECONNECTING, // Testclient intentionally disconnected and is reconnecting
    STATUS_CRASH, // Crashed and exception caught
} ChildStatus;
*/

char *status_text[] = {
    "Terminated",
    "Loading",
    "Running",
    "Connecting",
    "Queued",
    "ERROR",
    "DEAD",
    "SlaveLnchr",
    "BrknPipe",
    "RandDiscon",
    "Reconnect",
    "CRASH",
};

#define MAX_CHAT_CHANNELS 20

char *chat_channel_names[] = {
    "[UNKNOWN 00]",
    "[UNKNOWN 01]",
    "[UNKNOWN 02]",
    "[GROUPSYS]",
    "[UNKNOWN 04]",
    "[UNKNOWN 05]",
    "[UNKNOWN 06]",
    "[TELL]",
    "[TEAMCHAT]",
    "[SUPERGROUP]",
    "[LOCAL]",
    "[BROADCAST]",
    "[REQUEST]",
    "[FRIENDS]",
    "[ADMIN]",
    "[SYSTEM]",
    "[UNKNOWN 16]",
    "[UNKNOWN 17]",
    "[NPC]",
    "[UNKNOWN 19]",
};

#define LISTVIEW_BW_COLOR LISTVIEW_DEFAULT_COLOR,RGB(255,255,255)

COLORREF chat_channel_colors[] = {
    LISTVIEW_BW_COLOR,
    LISTVIEW_BW_COLOR,
    LISTVIEW_BW_COLOR,
    RGB(0,96,0),RGB(224,224,224),
    LISTVIEW_BW_COLOR,
    LISTVIEW_BW_COLOR,
    LISTVIEW_BW_COLOR,
    LISTVIEW_DEFAULT_COLOR,RGB(255,255,200),
    LISTVIEW_DEFAULT_COLOR,RGB(200,255,200),
    LISTVIEW_DEFAULT_COLOR,RGB(200,200,255),
    LISTVIEW_DEFAULT_COLOR,RGB(225,225,225),
    LISTVIEW_DEFAULT_COLOR,RGB(225,255,255),
    LISTVIEW_DEFAULT_COLOR,RGB(255,225,255),
    LISTVIEW_DEFAULT_COLOR,RGB(255,200,200),
    RGB(128,0,0),RGB(225,225,225),
    RGB(128,0,0),RGB(225,225,225),
    LISTVIEW_BW_COLOR,
    LISTVIEW_BW_COLOR,
    RGB(200,200,200),RGB(255,255,255),
    LISTVIEW_BW_COLOR,
};

/*
typedef struct ChildInfo {
    HANDLE process_handle;
    PROCESS_INFORMATION pinfo;
    char *host; // Computer name if a remote host
    int list_index;
    int uid; // For communicating with the PipeServer
    ChildStatus status;
    char status_name[128];
    char *player;
    char *authname;
    char *mapname;
    float hp[2];
    HWND hwnd;
    char *target;
    char *random_disconnect_stage;
    int lost_sent, lost_recv; // network stats
} ChildInfo;
*/

TokenizerParseInfo ChildInfoInfo[] =
{
    { "Host",        TOK_STRING(ChildInfo, host, 0), 0, TOK_FORMAT_LVWIDTH(40)},
    { "PID",        TOK_INT(ChildInfo, pinfo.dwProcessId, 0), 0, TOK_FORMAT_LVWIDTH(37)},
    { "Status",        TOK_FIXEDSTR(ChildInfo, status_name), 0, TOK_FORMAT_LVWIDTH(70)},
    { "Name",        TOK_STRING(ChildInfo, player, 0), 0, TOK_FORMAT_LVWIDTH(100)},
    { "CurHP",        TOK_F32(ChildInfo, hp[0], 0), 0, TOK_FORMAT_LVWIDTH(45)},
    { "MaxHP",        TOK_F32(ChildInfo, hp[1], 0), 0, TOK_FORMAT_LVWIDTH(45)},
    { "Auth",        TOK_STRING(ChildInfo, authname, 0), 0, TOK_FORMAT_LVWIDTH(80)},
    { "MapName",    TOK_STRING(ChildInfo, mapname, 0), 0, TOK_FORMAT_LVWIDTH(80)},
    { "RDC Stage",    TOK_STRING(ChildInfo, random_disconnect_stage, 0), 0, TOK_FORMAT_LVWIDTH(4)},
    { "Lost In",    TOK_INT(ChildInfo, lost_recv, 0), 0, TOK_FORMAT_LVWIDTH(30)},
    { "Lost Out",    TOK_INT(ChildInfo, lost_sent, 0), 0, TOK_FORMAT_LVWIDTH(30)},
    { 0 }
};

static int client_count;

ChildInfo **children_data;
ChildInfo ***children = &children_data;
PipeServer pipe_server = NULL;
ListView *lvChildInfo = NULL;


void sendMessageToSlaveLauncher(ChildInfo *cisl, char *command, char *buf)
{
    assert(cisl->status == STATUS_SLAVE_LAUNCHER);
    PipeServerSendMessage(pipe_server, cisl->uid, "%s %s", command, buf);
}


void launchNewClientOnSlave(char *host) {
    int i;
    for (i=0; i<eaSize(children); i++) {
        if (children_data[i]->status==STATUS_SLAVE_LAUNCHER &&
            stricmp(children_data[i]->host, host)==0)
        {
            char temp[512];
            sprintf(temp, makeParamString(), "", client_count++);
            strcat(temp, " -host ");
            strcat(temp, getComputerName());
            sendMessageToSlaveLauncher(children_data[i], "Launch", temp);
        }
    }
}

void launchNewClientOnSlaves(void) {
    int i;
    for (i=0; i<eaSize(children); i++) {
        if (children_data[i]->status==STATUS_SLAVE_LAUNCHER) {
            char temp[512];
            sprintf(temp, makeParamString(), "", client_count++);
            strcat(temp, " -host ");
            strcat(temp, getComputerName());
            sendMessageToSlaveLauncher(children_data[i], "Launch", temp);
        }
    }
}
wchar_t *poss_clients[] = {
#ifndef TEXT_CLIENT
        L".\\TestClient.exe",
        L".\\bin\\TestClient.exe",
        L"..\\bin\\TestClient.exe",
#else
        L".\\TextClient.exe",
        L".\\bin\\TextClient.exe",
        L"..\\bin\\TextClient.exe",
#endif
};

char coh_client[512];
char cohtest_client[512];


ChildInfo* launchTextClient(wchar_t *params, HWND tab) {
    DLGHDR_TAB* dlgts = getTabStruct(tab);
    int i;
    TCHAR strentry[100];
    int ret = FALSE;
    STARTUPINFO si = {0};
    PROCESS_INFORMATION pi;
    wchar_t cmdline[MAX_PATH]={0};
    static int client_loc=-1;
    static wchar_t actualclient[512];
    static wchar_t actualclientwd[512] = L".";
    wchar_t pDir[MAX(ARRAY_SIZE(cohtest_client),ARRAY_SIZE(coh_client))];
    wchar_t tempParams[1024];

    // convert the dir
    UTF8ToWideStrConvert(strstr(WideToUTF8StrTempConvert(params,NULL),"Training Room") ? cohtest_client : coh_client, pDir, ARRAY_SIZE(pDir));

    // copy params to wide string
    wcscpy(tempParams, params);

    // Worst-case general client location
    if (client_loc==-1) {
        wsprintf(cmdline, L"cmd /k echo Could not locate test client");
        for (client_loc=0; client_loc<ARRAY_SIZE(poss_clients); client_loc++) {
            if (wfileExists(poss_clients[client_loc])) {
                wsprintf(actualclient,L"%s",poss_clients[client_loc]);
                break;
            }
        }
    }

    // Server-specific client
    SendMessage(GetDlgItem(tab, IDC_LOGINGROUP), WM_GETTEXT, 100, (LPARAM)strentry);
    for (i=0; i<num_servers; i++) {
        if (0 == lstrcmp(strentry,server_list[i].servername))
            break;
    }
    assert(i<num_servers);
    if (server_list[i].clientpath)
    {
        wcscpy(pDir,server_list[i].clientpath);
        wcscat(tempParams,L" -nosharedmemory");
    }

    if (client_loc<ARRAY_SIZE(poss_clients))
    {
        wsprintf(cmdline, tempParams, actualclient, client_count++);
    }
    else
    {
        chatprintfW(tab,0,CC_LAUNCHER,L"[LAUNCHER] Unable to locate desired client: %s\n",actualclient);
    }

    chatprintfW(tab,0,CC_LAUNCHER,L"[LAUNCHER] Launching: %s\n",actualclient);

    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );

    ret = CreateProcessW(NULL, cmdline,
        NULL, NULL, FALSE,
        0,                     // creation flags
        NULL, NULL, &si, &pi);

    if (!ret) {
        DWORD errnum = GetLastError();
        TCHAR *lpMsgBuf;
        DWORD len = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM,NULL,errnum,0,(LPTSTR) &lpMsgBuf,0,NULL);

        chatprintf(tab,0,CC_LAUNCHER,"[LAUNCHER] Error (%u) spawning new process!\n",errnum);
        chatprintfW(tab,0,CC_LAUNCHER,L"[LAUNCHER] Error: %s\n",lpMsgBuf);
        LocalFree(lpMsgBuf);

        return NULL; // BREAK IN FLOW

    } else {
        ChildInfo *ci = calloc(sizeof(ChildInfo), 1);
        ci->process_handle = pi.hProcess;
        ci->status = STATUS_LOADING;
        sprintf(ci->status_name, "%s", status_text[ci->status]);
        ci->pinfo = pi;
        chatprintf(tab,0,CC_LAUNCHER,"[LAUNCHER] Spawned Text Client as process %d...\n",pi.dwProcessId);
        eaPush(children, ci);
        ci->tab_hwnd = tab;
        ci->batchFile = NULL;
        return ci;
//        ci->list_index = listViewAddItem(lvChildInfo, ci);
    }
}


void launchNewClient(int notunique) { // hide is now ignored
    int ret;
    STARTUPINFOW si = {0};
    PROCESS_INFORMATION pi;
    wchar_t cmdline[MAX_PATH]={0};
    wchar_t wparam_string[1024] = {0};
    
    static int client_loc=-1;
    
    UTF8ToWideStrConvert( makeParamString(), wparam_string, ARRAY_SIZE( wparam_string ) );

    if (client_loc==-1) {
        wsprintf(cmdline, L"cmd /k echo Could not locate test client");
        for (client_loc=0; client_loc<ARRAY_SIZE(poss_clients); client_loc++) {
            if (wfileExists(poss_clients[client_loc])) {
                break;
            }
        }
    }
    if (!notunique && client_count==0) {
        srand(time(NULL));
        client_count = rand()%500;
    }
    if (client_loc<ARRAY_SIZE(poss_clients)) {
        wsprintf(cmdline, wparam_string, poss_clients[client_loc], client_count++);
    }

    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );

    ret = CreateProcessW(NULL, cmdline,
        NULL, NULL, FALSE,
        0,                     // creation flags
        NULL, NULL, &si, &pi);

    if (!ret) {
        addLineToEnd(IDC_LOG, "Error spawning new process!");
    } else {
        ChildInfo *ci = calloc(sizeof(ChildInfo), 1);
        ci->process_handle = pi.hProcess;
        ci->status = STATUS_LOADING;
        sprintf(ci->status_name, "%s", status_text[ci->status]);
        ci->pinfo = pi;
        addLineToEnd(IDC_LOG, "Spawned process %d", pi.dwProcessId);
        eaPush(children, ci);
        ci->list_index = listViewAddItem(lvChildInfo, ci);
    }
}

int g_clients_left_to_launch;
int g_clients_launch_delay;

void launchNewClients(int num, int delay_in_ms)
{
    int ii;
    if (delay_in_ms == 0)
    {
        // launch immediately
        g_clients_left_to_launch = 0;
        for (ii=0; ii<num; ii++)
            launchNewClient(1);
    }
    else
    {
        //setup data for periodic launch
        g_clients_launch_delay = delay_in_ms;
        g_clients_left_to_launch = num;
    }    
}

void tick_periodic_launch(void)
{
    if (g_clients_left_to_launch)
    {
        static int lastSend = 0;
        int elapsedTime = (timeGetTime() - lastSend);
        if (elapsedTime > g_clients_launch_delay)
        {
            launchNewClient(1);
            g_clients_left_to_launch--;
            lastSend = timeGetTime();
        }
    }
}

BOOL CALLBACK searchCallback(HWND hwnd, LPARAM desired) {
    DWORD processid;
    DWORD test;
    GetWindowThreadProcessId(hwnd, &processid);

    test = *(DWORD*)desired;
    if (processid==test) {
        *(HWND*)desired = hwnd;
        return FALSE;
    }
    return TRUE;
}

HWND hwndFromProcessID(DWORD dwProcessID) {
    HWND ret;
    void *data = &ret;
    assert(sizeof(HWND)>=sizeof(DWORD));
    *(DWORD*)data = dwProcessID;
    if (EnumWindows(searchCallback, (LPARAM)data)) {
        return NULL;
    }
    return ret;
}

ChildInfo *ciFromListIndex(int index) {
    int i;
    for (i=0; i<eaSize(children); i++) 
        if (children_data[i]->list_index==index)
            return children_data[i];
    return NULL;
}

ChildInfo *findSlaveLauncherOnHost(char *hostname)
{
    int i;
    for (i=0; i<eaSize(children); i++) 
        if (children_data[i]->status==STATUS_SLAVE_LAUNCHER &&
                stricmp(children_data[i]->host, hostname)==0)
        {
            return children_data[i];
        }
    return NULL;
}

void setNextChild(ListView *lv, ChildInfo *ci, void *junk) {
    if (!ci) return;
    sscanf(ci->authname, "Dummy%d", &client_count);
    client_count--;
}

void grabConsole(ListView *lv, ChildInfo *ci, void *junk) {
    if (!ci) return;
    PipeServerSendMessage(pipe_server, ci->uid, "SCREEN");
}


void showChild(ListView *lv, ChildInfo *ci, int cmd) {
    if (!ci || !ci->host) return;
    if (stricmp(ci->host, getComputerName())==0) {
        if (ci->hwnd==NULL) {
            ci->hwnd = hwndFromProcessID(ci->pinfo.dwProcessId);
        }
        if (ci->hwnd)
            ShowWindow(ci->hwnd, cmd);
    } else {
        // Find a SlaveLauncher for this host
        ChildInfo *cisl = findSlaveLauncherOnHost(ci->host);
        if (cisl) {
            char temp[512];
            sprintf(temp, "SHOW %d %d", ci->pinfo.dwProcessId, cmd);
            sendMessageToSlaveLauncher(cisl, "Exec", temp);
        }
    }
}

void updateStatusChildInfo(ChildInfo *ci)
{
    sprintf(ci->status_name, status_text[ci->status]);
#ifndef TEXT_CLIENT
//    if (!gbTextClient) {
        // Replaced with new ListView stuff
        listViewItemChanged(lvChildInfo, ci);
//    }
#else
//    else {
//        SendMessage(GetDlgItem(ci->tab_hwnd, IDC_CLIENT_UPDATE),BM_CLICK,0,0);
        onClientUpdateCommand(ci->tab_hwnd);
//    }
#endif 
}

void updateStatusChildIndex(int i) {
    updateStatusChildInfo(children_data[i]);
}

int checkChildren() {
    int i; 
    for (i=0; i<eaSize(children); i++) {
        if (children_data[i]->status!=STATUS_TERMINATED && children_data[i]->process_handle) {
            switch (WaitForSingleObject(children_data[i]->process_handle, 0)) {
                case WAIT_TIMEOUT:
                    // Still running
                    break;
                case WAIT_OBJECT_0:
                    // Process exited!
                    children_data[i]->status = STATUS_TERMINATED;
                    updateStatusChildIndex(i);
                    // Could possibly remove from list here, perhaps after a timeout
                    return 1;
                    break;
                default:
                    assert(0);
            }
        }
    }
    return 0;
}

static void killChildInfo(ChildInfo *ci) 
{
    if (!ci || ci->status == STATUS_TERMINATED) 
    {
        return;
    }

    if (ci->process_handle) 
    {
        TerminateProcess(ci->process_handle, -1);
        CloseHandle(ci->process_handle);
        ci->process_handle = NULL;
        ci->status = STATUS_TERMINATED;
    } 
    else 
    {
        // Probably a remote process, just close the pipe 
        PipeServerClosePipe(pipe_server, ci->uid);
        ci->status = STATUS_NOPIPE;
    }
    updateStatusChildInfo(ci);
}

void callback(int UID, const char *msg) {
    char keybuf[256];
    int index;
    static StashTable htIDtoIndex=0;
    int i;

    sprintf(keybuf, "%d", UID);

    if (!htIDtoIndex) {
        htIDtoIndex = stashTableCreateWithStringKeys(1000, StashDeepCopyKeys);
    }

    if (!stashFindInt(htIDtoIndex, keybuf, &index)) {
        DWORD pid;
        // This is a new client! (or it's not in the hashtable)
        // the first message must be the PID: message
        if (msg==NULL || strnicmp(msg, "PID:", 4)!=0) {
            addLineToEnd(IDC_LOG, "Warning: received a first line of \"%s\", expected PID", msg);
            return;
        }
        sscanf(msg, "PID: %d", &pid);
        // Find entry with this PID
        index = -1;
        for (i=0; i<eaSize(children); i++) {
            if (children_data[i]->pinfo.dwProcessId == pid && children_data[i]->uid==0) {
                index = i;
                break;
            }
        }
        if (index==-1) {
            // PID not ours, it's probably a slavelauncher connecting, or a process on another system
            ChildInfo *ci = calloc(sizeof(ChildInfo), 1);

            ci->process_handle = NULL;
            ci->status = STATUS_ERROR;
            ci->pinfo.dwProcessId = pid;
            ci->pinfo.dwThreadId = 0;
            ci->pinfo.hProcess = NULL;
            ci->pinfo.hThread = NULL;
            //addLineToEnd(IDC_LOG, "Process %d connceted", pid);
            index = eaPush(children, ci);
            sprintf(ci->status_name, "UNKNOWN");
            ci->list_index = listViewAddItem(lvChildInfo, ci);
        }
        stashAddInt(htIDtoIndex, keybuf, (index+1), false);
        children_data[index]->uid = UID;
    } else {
        ChildInfo *ci = children_data[index-1];
        int i;
        // otherwise, handle the message
        char *command;
        char *s;

        if (msg==NULL) {
            // disconnect
            //addLineToEnd(IDC_LOG, "Disconnect from process %d", ci->pinfo.dwProcessId);
            ci->status = STATUS_TERMINATED;
            updateStatusChildInfo(ci);
            return;
        }

        command = strdup(msg);

        if (!(s=strchr(command, ':'))) {
            ci->status = STATUS_ERROR;
#ifndef TEXT_CLIENT
            addLineToEnd(IDC_LOG, "Error: got bad command from client : %s", command);
#else
            chatprintf(ci->tab_hwnd, 0,CC_LAUNCHER,"[LAUNCHER] Client sent bad command: %s\n",command);
#endif
        } else {
            *s=0;
            s++;
            while (*s==' ') s++;
            if (stricmp(command, "status")==0) {
                ci->status = -1;
                for (i=0; i<ARRAY_SIZE(status_text); i++)  {
                    if (strnicmp(s, status_text[i], strlen(status_text[i]))==0) {
                        ci->status = i;
                    }
                }
                if (ci->status==-1) {
                    ci->status = STATUS_ERROR;
#ifndef TEXT_CLIENT
                    addLineToEnd(IDC_LOG, "Error: got bad status string from client : %s", s);
#else
                    chatprintf(ci->tab_hwnd, 0,CC_LAUNCHER,"[LAUNCHER] Client sent bad status: %s\n", s);
#endif
                } else if (ci->status==STATUS_SLAVE_LAUNCHER) {
                    char temp[256];
                    addLineToEnd(IDC_LOG, "SlaveLauncher connected from %s", ci->host);
                    enableRB2();
                    if (ci->player) free(ci->player);
                    sprintf(temp, "\\\\%s\\SlaveLauncher", ci->host);
                    ci->player = strdup(temp); //TStrDupFromChar(temp);
                } else if (ci->status==STATUS_RECONNECTING) {
                    if (stricmp(ci->host, getComputerName())==0) {
                        launchNewClient(1);
                    } else {
                        launchNewClientOnSlave(ci->host);
                    }
                }
            } else if (stricmp(command, "Player")==0) {
                if (ci->player) free(ci->player);
                ci->player = strdup(s);//TStrDupFromChar(s);
            } else if (stricmp(command, "AuthName")==0) {
                if (ci->authname) free(ci->authname);
                ci->authname = strdup(s);
            } else if (stricmp(command, "RandDiscon")==0) {
                if (ci->random_disconnect_stage) free(ci->random_disconnect_stage);
                ci->random_disconnect_stage = strdup(s);
            } else if (stricmp(command, "HP")==0) {
                sscanf(s, "%f/%f", &ci->hp[0], &ci->hp[1]);
            } else if (stricmp(command, "NewTarget")==0) {
                if (ci->target) free(ci->target);
                ci->target = strdup(s);
            } else if (stricmp(command, "Host")==0) {
                if (ci->host) free(ci->host);
                ci->host = strdup(s);
            } else if (stricmp(command, "MapName")==0) {
                if (ci->mapname) free(ci->mapname);
                if (strnicmp(s, "maps", 4)==0) {
                    s+=5;
                }
                if (strrchr(s, '/')) {
                    ci->mapname= strdup(strrchr(s, '/')+1);
                } else {
                    ci->mapname= strdup(s);
                }
            } else if (stricmp(command, "LostCount")==0) {
                sscanf(s, "%d/%d", &ci->lost_sent, &ci->lost_recv);
            } else if (stricmp(command, "VersionRequest")==0) {
#ifndef TEXT_CLIENT
                PipeServerSendMessage(pipe_server, UID, "%s", getCompatibleGameVersion());
#else
                PipeServerSendMessage(pipe_server, UID, "%s", tabVersionRequest(ci->tab_hwnd));
#endif
            } else if (stricmp(command, "ChatText")==0) {
#ifndef TEXT_CLIENT
//                if (!gbTextClient)
                    addLineToEnd(IDC_LOG, "[%d] %s", ci->pinfo.dwProcessId,s);
#else
//                else {
                    char * t = strchr(s, ':');
                    char * u;
                    int tval;
                    *t = 0;
                    t++;
                    u = strchr(t,':');
                    *u = 0;
                    u++;
                    tval = atoi(t);
                    if (stricmp(s,"InfoBox")==0) {
                        if (tval == 1) chatprintf(ci->tab_hwnd,CC_LAUNCHER,tval+200,"[COMBAT] %s\n", u);
                        else if (tval == 2) chatprintf(ci->tab_hwnd,tval+200,CC_LAUNCHER,"[DAMAGE] %s\n", u);
                        else if (tval == 3) chatprintf(ci->tab_hwnd,tval+200,CC_LAUNCHER,"[INFO] %s\n", u);
                        else chatprintf(ci->tab_hwnd,tval+200,LISTVIEW_BW_COLOR,"[InfoBox %d] %s\n",tval,u);
                    }
                    else if ((stricmp(s,"Chat")==0) && (tval < MAX_CHAT_CHANNELS)) {
                        chatprintf(ci->tab_hwnd,tval+100,chat_channel_colors[tval*2],chat_channel_colors[1+(tval*2)],"%s %s\n",chat_channel_names[tval],u);
                    }
                    else if ((stricmp(s,"conPrintf")==0)) {
                        chatprintf(ci->tab_hwnd,300,LISTVIEW_BW_COLOR,"[CONSOLE] %s\n",u);
                    }
                    else {
                        chatprintf(ci->tab_hwnd,tval+400,LISTVIEW_BW_COLOR,"[%s %d] %s\n",s,tval,u);
                    }
//                }
#endif
            } else if (stricmp(command, "SCREEN")==0) {
                char    buf[1000];
                FILE *f;
                sprintf(buf,"%s/console.txt",fileTempDir());
                mkdirtree(buf);
                f = fopen(buf, "wt");
                if (f) {
                    fwrite(s, 1, strlen(s), f);
                    fclose(f);
                    fileOpenWithEditor(buf);
                }
            } 
            else if (stricmp(command, "QuitNow")==0)
            {
                killChildInfo(ci);
            }
            else {
#ifndef TEXT_CLIENT
//                if (!gbTextClient)
                    addLineToEnd(IDC_LOG, "ERROR: got unknown command: %s from UID: %d PID: \\\\%s\\%d", command, UID, ci->host, ci->pinfo.dwThreadId);
#else
//                else
                    chatprintf(ci->tab_hwnd, 0,CC_LAUNCHER,"[LAUNCHER] Received unknown command: %s from UID: %d PID: \\\\%s\\%d", command, UID, ci->host, ci->pinfo.dwThreadId);
#endif
            }
        }
        updateStatusChildIndex(index-1);
        free(command);
    }
//    addLineToEnd(IDC_LOG, "got message: %s", msg);
}


void doPeriodicUpdate() {
    while (checkChildren());
    PipeServerQuery(pipe_server, callback);

    {
        // Status bar update
        int running=0;
        int dead=0;
        int other=0;
        int error=0;
        int slaves=0;
        int i;

        for (i=0; i<eaSize(children); i++) {
            switch(children_data[i]->status) {
            case STATUS_NOPIPE:
            case STATUS_TERMINATED:
            case STATUS_RANDOM_DISCONNECT:
            case STATUS_RECONNECTING:
                dead++;
                break;
            case STATUS_DEAD:
            case STATUS_RUNNING:
                running++;
                break;
            case STATUS_ERROR:
            case STATUS_CRASH:
                error++;
                break;
            case STATUS_SLAVE_LAUNCHER:
                slaves++;
                break;
            default:
                other++;
            }
        }
        if (error) {
            setStatusBar(0 | SBT_POPOUT, "ERRORS: %d", error);
        } else {
            setStatusBar(0, "Errors: %d", error);
        }
        setStatusBar(1, "Running: %d", running);
        setStatusBar(2, "Loading: %d", other);
        setStatusBar(3, "Slaves: %d", slaves);
        setStatusBar(4, "Dead: %d", dead);

        // Hack to launch lots of them constantly!
        //if (running+other < 50) {
        //    launchNewClient(1);
        //}
    }
    tick_periodic_launch();

}

void killChild(ListView *lv, ChildInfo *ci, void *junk)
{
    killChildInfo(ci);
}

void sendCommand(ListView *lv, ChildInfo *ci, void *junk) 
{
    if (!ci || ci->status == STATUS_TERMINATED)
    {
        return;
    }
    PipeServerSendMessage(pipe_server, ci->uid, "%s", (char*)junk);
    updateStatusChildInfo(ci);
}

void killAllChildren(void) {
    int i; 
    while (checkChildren());
    for (i=0; i<eaSize(children); i++) 
    {
        killChildInfo(children_data[i]);
    }
}

static void quitChildInfo(ChildInfo *ci)
{
    if (!ci || ci->status == STATUS_TERMINATED) 
    {
        return;
    }

    sendCommand(NULL, ci, "cmd quit");
}

void quitChild(ListView *lv, ChildInfo *ci, void *junk)
{
    quitChildInfo(ci);
}

void quitAllChildren(void)
{
    int i; 
    while (checkChildren());
    for (i=0; i < eaSize(children); i++) 
    {
        quitChildInfo(children_data[i]);
    }
}

void doInit(HWND hDlg) {
    doInitPipeServer();
    doInitListView(hDlg);
}

void doInitPipeServer() {
    pipe_server = PipeServerCreate("TestClientLauncher");
}

void doInitListView(HWND hDlg) {
    lvChildInfo = listViewCreate();
    listViewInit(lvChildInfo, ChildInfoInfo, hDlg, GetDlgItem(hDlg, IDC_LIST1));
}

void doNotify(HWND hDlg, WPARAM wParam, LPARAM lParam)
{
    listViewOnNotify(lvChildInfo, wParam, lParam, NULL);
}

void doOnSelected(HWND hDlg, ListViewCallbackFunc func, void *data)
{
    listViewDoOnSelected(lvChildInfo, func, data);
}

RegReader rr = 0;
void initRR() {
    if (!rr) {
        rr = createRegReader();
        initRegReader(rr, "HKEY_CURRENT_USER\\SOFTWARE\\Cryptic\\TestClient");
    }
}
int regGetInt(const char *key, int deflt) {
    static int value;
    initRR();
    value = deflt;
    rrReadInt(rr, key, &value);
    return value;
}

const char *regGetString(const char *key, const char *deflt) {
    static char buf[512];
    initRR();
    strcpy(buf, deflt);
    rrReadString(rr, key, buf, 512);
    return buf;
}

void regPutInt(const char *key, int value) {
    rrWriteInt(rr, key, value);
}

void regPutString(const char *key, const char *value) {
    rrWriteString(rr, key, value);
}

void checkArgs(int argc, char **argv) {
}

int main(int argc, char **argv)
{
    //setConsoleTitle("TestClientLauncher");
    //SetPriorityClass(GetCurrentProcess(), NORMAL_PRIORITY_CLASS);
    //SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_BELOW_NORMAL);

    memMonitorInit();

    eaCreate(children);

//    checkArgs(argc, argv);

#ifndef TEXT_CLIENT
    DlgMainDoDialog();
#else
    {
        RegReader rr0 = createRegReader();
        RegReader rr1 = createRegReader();

        sprintf(coh_client,"");
        sprintf(cohtest_client,"");

        initRegReader(rr0, "HKEY_CURRENT_USER\\SOFTWARE\\Cryptic\\CoH");
        initRegReader(rr1, "HKEY_CURRENT_USER\\SOFTWARE\\Cryptic\\CohTest");
        rrReadString(rr0,"Installation Directory",coh_client,512);
        rrReadString(rr1,"Installation Directory",cohtest_client,512);

        if (argc == 7 && !strnicmp(argv[1],"-batch",6))
        {
            batchmode.state = BMS_INIT;
            batchmode.tabid = -1;
            batchmode.logouttimer = 20; // ~10s
            batchmode.auth = TStrDupFromChar(argv[2]);
            batchmode.pw = TStrDupFromChar(argv[3]);
            batchmode.server = TStrDupFromChar(argv[4]);
            batchmode.character = TStrDupFromChar(argv[5]);
            batchmode.batchfile = TStrDupFromChar(argv[6]);
            if (!stricmp(argv[1],"-batchdebug")) batchmode.debug = true;
        }
        else
        {
            batchmode.state = BMS_INACTIVE;
        }

        DlgTextMainDoDialog();
    }
#endif

    return 0;
} 

//------------------------------------------------------------
//  Helper for char* to TCHAR strdup
//----------------------------------------------------------
TCHAR* TStrDupFromChar(char *src)
{
#ifdef _UNICODE
    int len = (int)strlen(src);
    TCHAR *buf = malloc(len*sizeof(*buf) + 1);
    UTF8ToWideStrConvert( src, buf, len );
    return buf;
#else
    return strdup(src);
#endif
}

int CharTCharStrcmp(char *lhs, TCHAR *rhs)
{
#ifdef _UNICODE
    return lstrcmp( UTF8ToWideStrConvertStatic( lhs ), rhs );
#else
    return strcmp(lhs, rhs);
#endif
}

TCHAR* TCharFromChar(TCHAR *dest, char const *src, int maxLen)
{
#ifdef _UNICODE
    UTF8ToWideStrConvert((char*)src, dest, (int)strlen(src));
#else
    strcpy(dest, src);
#endif
    return dest;
}

char *CharFromTChar(char *dest, TCHAR const *src, int maxLen)
{
#ifdef _UNICODE
    WideToUTF8StrConvert( (TCHAR*)src, dest, maxLen );
#else
    strcpy(dest, src);
#endif
    return dest;
}

TCHAR *TCharFromCharStatic(char const *src)
{
    static TCHAR buf[1024];
    return TCharFromChar(buf,src,ARRAY_SIZE(buf));
}
