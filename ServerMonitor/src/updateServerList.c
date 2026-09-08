#include <container/containerbroadcast.h>
#include <comm_backend.h>
#include <entity/entVarUpdate.h>
#include <utilitieslib/utils/ListView.h>
#include <utilitieslib/utils/structHist.h>
#include <utilitieslib/utils/structNet.h>
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/winutil.h>
#include <utilitieslib/network/netio.h>
#include <utilitieslib/network/sock.h>

#include "prompt.h"
#include "resource.h"
#include "serverMonitor.h"
#include "serverMonitorCommon.h"
#include "serverMonitorNet.h"
#include "shardMonitor.h"
#include "shardMonitorComm.h"
#include "shardMonitorConfigure.h"

typedef enum eConnStatus {
    eConnStatus_NotConnected,
    eConnStatus_Failed,
    eConnStatus_Connecting,
    eConnStatus_Connected,

} eConnStatus;

// keep in synch with enum above
static char* sConnStatusText[] = 
{
    "Not Connected",
    "Failed",
    "Connecting...",
    "Connected",
};

enum {
    kThreadFlag_None        = 0,
    kThreadFlag_DataPending = 1<<0,
    kThreadFlag_Failed        = 1<<1
};

typedef struct UpdateServerData
{
    // updateserver data that we track
    char clientVersion[128];
    char serverVersion[128];
    U32 numUsers;
    U32 totalDataSentMB;
    U32 dataRate;
} UpdateServerData;

typedef struct UpdateServerStats {

    UpdateServerData data;

    // Only used for internal bookkeeping
    U32 ip;
    char name[128];
    char connStatusText[32];
    eConnStatus connStatus;
    F32 nextUpdateTime;
    
    // for data collection thread
    UpdateServerData inData;
    U32 threadID;
    U8 threadFlags;
    U32 listCounter;

} UpdateServerStats;

TokenizerParseInfo UpdateServerConfigEntryDispInfo[] =    // defines display box under update servers tab
{
    { "Name",                TOK_FIXEDSTR(UpdateServerStats, name), 0,                    TOK_FORMAT_LVWIDTH(83)},
    { "IpAddress",            TOK_INT(UpdateServerStats, ip, 0), 0,                        TOK_FORMAT_IP | TOK_FORMAT_LVWIDTH(90)},
    { "Status",                TOK_FIXEDSTR(UpdateServerStats, connStatusText), 0,            TOK_FORMAT_LVWIDTH(85)},
    { "ClientVersion",        TOK_FIXEDSTR(UpdateServerStats, data.clientVersion), 0,        TOK_FORMAT_LVWIDTH(80)},
    { "NumUsers",            TOK_INT(UpdateServerStats, data.numUsers, 0), 0,            TOK_FORMAT_LVWIDTH(60)},
    { "TotalSent",            TOK_INT(UpdateServerStats, data.totalDataSentMB, 0), 0,        TOK_FORMAT_MBYTES | TOK_FORMAT_LVWIDTH(70)},
    { "DataRate",            TOK_INT(UpdateServerStats, data.dataRate, 0), 0,            TOK_FORMAT_BYTES | TOK_FORMAT_LVWIDTH(60)},    
    { 0 }
};


static bool updateServerBusy=false;
static UpdateServerStats **eaUpdateServerStats_data=NULL, ***eaUpdateServerStats = &eaUpdateServerStats_data;
static ListView *lvUpdateServers=NULL;
static int timer=0;
U32 sStatListCounter = 0;
static CRITICAL_SECTION sCritSec;

#define DEFAULT_UPDATE_INTERVAL  5.0f  // seconds between update requests

static void targettedRemoteDesktop(ListView *lv, UpdateServerStats *stat, HWND hDlg)
{
    launchRemoteDesktop(stat->ip, stat->name);
}

static void destroyUpdateServerStats(UpdateServerStats *stats)
{ 
    free(stats);
}

static UpdateServerStats *createUpdateServerStats(void)
{
    return calloc(sizeof(UpdateServerStats), 1);
}

static void setDefaultStatValues(UpdateServerStats *stat)
{
    strcpy(stat->data.clientVersion, "Unknown");
    strcpy(stat->data.serverVersion, "Unknown");
    stat->data.numUsers = 0;
    stat->data.totalDataSentMB = 0;
    stat->data.dataRate = 0;

    stat->connStatus = eConnStatus_NotConnected;
    strcpy(stat->connStatusText, sConnStatusText[stat->connStatus]);
    stat->threadID = 0;
    stat->threadFlags = kThreadFlag_None;
    stat->nextUpdateTime = 0;
    stat->listCounter = sStatListCounter;
}

static void destroyUpdateServerStatsAdapter(void* arg0)
{
    destroyUpdateServerStats((UpdateServerStats *)arg0);
}

static void updateServerListInit()
{
    int i;

    EnterCriticalSection(&sCritSec);
    // bump the global list counter so that any pending requests are discarded 
    //    when they finish (otherwise they will access invalid stat object that 
    //    is about to be deleted).
    sStatListCounter++;
    LeaveCriticalSection(&sCritSec);

    shardMonLoadConfig("./UpdateServerListConfig.txt");
    listViewDelAllItems(lvUpdateServers, NULL);

    eaClearEx(eaUpdateServerStats, destroyUpdateServerStatsAdapter);
    for (i=0; i<eaSize(&shmConfig.shardList); i++)
    {
        UpdateServerStats *stat = createUpdateServerStats();
        eaPush(eaUpdateServerStats, stat);
        strcpy(stat->name, shmConfig.shardList[i]->name);
        stat->ip = shmConfig.shardList[i]->ip;
        setDefaultStatValues(stat);
        listViewAddItem(lvUpdateServers, stat);
        //listViewSetItemColor(lvUpdateServers, stat, COLOR_DISABLED);
    }
}

static void updateServerDisconnect(UpdateServerStats *stat)
{
    // kill any connection in progress
    if( stat->threadID ) {
        
    }

    setDefaultStatValues(stat);
    //listViewSetItemColor(lvUpdateServers, stat, COLOR_DISABLED);
    listViewItemChanged(lvUpdateServers, stat);
}

static void updateServerDisconnectAll()
{
    int i;
    if (updateServerBusy)
        return;
    updateServerBusy = true;
    for (i=0; i<eaSize(eaUpdateServerStats); i++) {
        UpdateServerStats *stat = eaGet(eaUpdateServerStats, i);
        updateServerDisconnect(stat);
    }
    updateServerBusy = false;
}

// extract value from name:value pair.  Assumes spaces only used as separators between pairs.
static char sTempBuf[1024];
static const char* getCactiValue(const char* name, const char* fullText)
{
    int len = (int)strlen(name);
    const char* pEnd = fullText + strlen(fullText);
    const char* pNameStart = strstr(fullText, name);

    sTempBuf[0] = '\0'; // if don't find name return empty string for value

    if(pNameStart)
    {
        pNameStart += len + 1; // advance past ":"
        if(pNameStart < pEnd)
        {
            const char* pSpace = strchr(pNameStart, ' ');
            if(pSpace)
            {
                len = pSpace - pNameStart;
                if( len < sizeof(sTempBuf) )
                {
                    strncpy(sTempBuf, pNameStart, len);
                    sTempBuf[len] = '\0';
                }
            }
        }
    }

    return sTempBuf;    
}

static void parseUpdateServerData(const char* text, UpdateServerData* pData )
{
    // Take advantage of known string format (cacti format): 
    // serverversion:xx clientversion:xx users:xx total:xx throughput:xx
    strncpy(pData->serverVersion, getCactiValue("serverversion", text), sizeof(pData->serverVersion)-1);
    strncpy(pData->clientVersion, getCactiValue("clientversion", text), sizeof(pData->clientVersion)-1);
    pData->numUsers = atoi( getCactiValue("users", text) );
    pData->totalDataSentMB = atoi( getCactiValue("total", text) );
    pData->dataRate = atoi( getCactiValue("throughput", text) );
}

static unsigned __stdcall retrieveDataThread(void* pData)
{
    // steps: 
    //    1. connect to updateserver
    //    2. read data and close connection
    //    3. parse into inData field of struct
    //    4. lock crit section, copy to main data field
    //    5. next time thru tick on main thread, check that status is connecting, copy data over.
    int n, res;
    U32 inCounter;
    char tmp[1024];
    SOCKET s = INVALID_SOCKET;
    struct sockaddr_in    addr_in;
    UpdateServerData localData;
    UpdateServerStats *stat = (UpdateServerStats*)pData;
    bool bSuccess = false;

    memset( &localData, 0, sizeof(UpdateServerData) ); // shut compiler up about possibly uninitialized data

    inCounter = stat->listCounter; // counter to track that incoming stat memory is still active

    sockStart();  
    s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    sockSetAddr( &addr_in, stat->ip, DEFAULT_UPDATESERVER_LISTEN_PORT );
    
    res = connect( s,(struct sockaddr *)&addr_in,sizeof(addr_in) );
    if( res == 0 )
    {
        // retrieve data
        n = recv( s,tmp,sizeof(tmp)-1,0 );
        closesocket(s);
        if( n > 0 )
        {
            tmp[n] = '\0';
            parseUpdateServerData(tmp, &localData);
            bSuccess = true;
        }
    }

    // first check to make sure that the underlying stat object hasn't been deleted
    //    (eg if somebody hit configure).  Only update stat object if it's still valid.
    EnterCriticalSection(&sCritSec);
    if( inCounter == sStatListCounter )
    {
        if(bSuccess) {
            memcpy( &stat->inData, &localData, sizeof(UpdateServerData) ); 
            stat->threadFlags |= kThreadFlag_DataPending;
        } else {
            stat->threadFlags |= kThreadFlag_Failed;
        }
    }
    LeaveCriticalSection(&sCritSec);

    return (bSuccess ? 0 : -1);
}

static void updateServerConnectSingle(UpdateServerStats *stat)
{
    int ret=0;
    if (updateServerBusy)
        return;

    // We dont make a permanent connection, just a one-shot connection to get the data
    //    If successful, we continuously ping the server every 5 seconds to update our data.
    //    So the UI shows as "connected" when we successfully get data and are going to get more.
    //    If not connected, or failed to connect, we still come thru here and try to start the
    //    continuous connect mode, only bail here if already in the process of connecting to 
    //    this server.
    if( stat->connStatus == eConnStatus_Connecting )
        return;

    updateServerBusy = true;
    stat->connStatus = eConnStatus_Connecting;
    stat->threadFlags = kThreadFlag_None;
    strcpy(stat->connStatusText, sConnStatusText[stat->connStatus]);
    listViewItemChanged(lvUpdateServers, stat);
    
    // kick off connection thread    
    if( !_beginthreadex(NULL, 0, retrieveDataThread, stat, 0, &stat->threadID) )
    {
        stat->connStatus = eConnStatus_NotConnected;
        strcpy(stat->connStatusText, sConnStatusText[stat->connStatus]);
        listViewItemChanged(lvUpdateServers, stat);
    }

    updateServerBusy = false;
}

static void updateServerConnectAll()
{
    int i;
    for (i=0; i<eaSize(eaUpdateServerStats); i++) {
        UpdateServerStats *stat = eaGet(eaUpdateServerStats, i);
        updateServerConnectSingle(stat);
        if (serverMonitorExiting())
            break;
    }
}

static void connectSingleWrapper(ListView *lv, UpdateServerStats *stat, void *junk)
{
    updateServerConnectSingle(stat);
}

static void disconnectSingleWrapper(ListView *lv, UpdateServerStats *stat, void *junk)
{
    updateServerDisconnect(stat);
}

static void updateButtons(HWND hDlg, int count)
{
    EnableWindow(GetDlgItem(hDlg, IDC_BTN_CONNECT), true);
    EnableWindow(GetDlgItem(hDlg, IDC_BTN_DISCONNECT), true);
    EnableWindow(GetDlgItem(hDlg, IDC_BTN_CONNECT_SINGLE), count >= 1);
    EnableWindow(GetDlgItem(hDlg, IDC_BTN_DISCONNECT_SINGLE), count >= 1 && !updateServerBusy);
    EnableWindow(GetDlgItem(hDlg, IDC_BTN_REMOTEDESKTOP), (count == 1 && !updateServerBusy));
}

static void updateConnectionStatus(UpdateServerStats *stat, HWND hDlg)
{
    F32 elapsedTime = timerElapsed(timer);

    // check to see if new data came in, if so update main data
    EnterCriticalSection(&sCritSec);
    if( stat->threadFlags )
    {
        if( stat->threadFlags & kThreadFlag_DataPending ) {
            memcpy( &stat->data, &stat->inData, sizeof(UpdateServerData) );
            stat->connStatus = eConnStatus_Connected; // show as connected, will auto-update every 5 seconds
            stat->nextUpdateTime = elapsedTime + DEFAULT_UPDATE_INTERVAL;
            
        } else if( stat->threadFlags & kThreadFlag_Failed ) {
            stat->connStatus = eConnStatus_Failed;
            stat->nextUpdateTime = 0;
        }

        strcpy(stat->connStatusText, sConnStatusText[stat->connStatus]);
        stat->threadFlags = kThreadFlag_None;
    }
    LeaveCriticalSection(&sCritSec);

    // If connected, check to see if it's time to request another update.  If our tab is not visible, 
    //    we don't bother updating in the background until it's visible again.
    if( elapsedTime > stat->nextUpdateTime && stat->connStatus == eConnStatus_Connected && IsWindowVisible(hDlg) )
    {
        updateServerConnectSingle(stat);
    }

}

static void updateServerListTick(HWND hDlg)
{
    int i;
    bool init=true;
    int count = listViewDoOnSelected(lvUpdateServers, NULL, NULL);

    // Search and update statuses
    for (i=0; i<eaSize(eaUpdateServerStats); i++)
    {
        UpdateServerStats *stat = eaGet(eaUpdateServerStats, i);
        if (stat)
        {
            // Check connection status
            updateConnectionStatus(stat, hDlg);
            listViewItemChanged(lvUpdateServers, stat);
        }
    }

    updateButtons(hDlg, count);
}

static void onTimer(HWND hDlg)
{
    static int lock=0;
    if (lock!=0)
        return;
    lock++;
    updateServerListTick(hDlg);
    lock--;
}

LRESULT CALLBACK DlgUpdateServerListProc (HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    RECT rect;
    LRESULT ret = FALSE;

    switch (iMsg)
    {
    case WM_INITDIALOG:
    {
        static bool bFirstTime = true; // should only come here once, but to be safe
        if(bFirstTime)
        {
            lvUpdateServers = listViewCreate();
            timer = timerAlloc();
            InitializeCriticalSection(&sCritSec);
            bFirstTime = false;
        }

        listViewInit(lvUpdateServers, UpdateServerConfigEntryDispInfo, hDlg, GetDlgItem(hDlg, IDC_LST_SERVERS));
        listViewSetSortable(lvUpdateServers, false);

        updateServerListInit();
        SetTimer(hDlg, 0, 50, NULL);
        GetClientRect(hDlg, &rect); 
        doDialogOnResize(hDlg, (WORD)(rect.right - rect.left), (WORD)(rect.bottom - rect.top), IDC_ALIGNME, IDC_UPPERLEFT);
        setDialogMinSize(hDlg, 0, 200);
        break;
    }
    case WM_DESTROY:
        DeleteCriticalSection(&sCritSec);
        break;
    case WM_TIMER:
        onTimer(hDlg);
        break;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDOK:
            break;
        case IDCANCEL:
            EndDialog(hDlg, 0);
            PostQuitMessage(0);
            return TRUE;
        case IDC_BTN_CONNECT:
            updateServerConnectAll();
            break;
        case IDC_BTN_DISCONNECT:
            updateServerDisconnectAll();
            break;
        case IDC_BTN_CONNECT_SINGLE:
            listViewDoOnSelected(lvUpdateServers, connectSingleWrapper, NULL);
            break;
        case IDC_BTN_DISCONNECT_SINGLE:
            listViewDoOnSelected(lvUpdateServers, disconnectSingleWrapper, NULL);
            break;
        case IDC_BTN_CONFIGURE:
            updateServerDisconnectAll();
            shardMonConfigure(g_hInst, hDlg, "./UpdateServerListConfig.txt");
            updateServerListInit();
            updateServerConnectAll();
            break;
        case IDC_BTN_REMOTEDESKTOP:
            listViewDoOnSelected(lvUpdateServers, targettedRemoteDesktop, hDlg);
            break;
        }
        break;
    case WM_SIZE:
        {
            WORD w = LOWORD(lParam);
            WORD h = HIWORD(lParam);
            doDialogOnResize(hDlg, w, h, IDC_ALIGNME, IDC_UPPERLEFT);
        }
        break;
    case WM_NOTIFY:
        {
            int idCtrl = (int)wParam;
            int count;
            ret |= listViewOnNotify(lvUpdateServers, wParam, lParam, NULL);

            // update buttons
            count = listViewDoOnSelected(lvUpdateServers, NULL, NULL);
            updateButtons(hDlg, count);            
        }
        break;
    }

    return ret;
}