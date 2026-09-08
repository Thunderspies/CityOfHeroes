#include <comm_backend.h>
#include <performance.h>
#include <utilitieslib/components/StashTable.h>
#include <utilitieslib/network/netio.h>
#include <utilitieslib/network/netio_core.h>
#include <utilitieslib/utils/ListView.h>
#include <utilitieslib/utils/winutil.h>
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/utils/structNet.h>
#include <utilitieslib/utils/utils.h>

#include "chatMonitor.h"
#include "resource.h"
#include "serverMonitorCommon.h"
#include "prompt.h"
#include "serverMonitor.h"
#include "shardMonitor.h"

typedef struct ChatCon
{
    char ipAddress[128];
    char hostname[128];
    char chatSvrAddr[128];    // value from 'connect' combo box

    bool connection_expected;

    int protocol;

    NetLink link;

    int lastUpdate;        // time of last update

    // stats
    int        users;
    int        channels;
    int        online;
    int        links;

    bool    reservedNames;

    int        monitors;    // # of servermonitors connected to chatserver

    F32        crossShard;
    F32        invalid;

    // resources/performance
    int        sendBytes; 
    int        recvBytes; 
    int        sendMsgs;
    int        recvMsgs;

    TrackedPerformance perfInfo;

    // timing
    U32 lastUpdateSec;
    int secSinceUpdate;

    int totalPhysMem;
    int availPhysMem;
    int totalPageMem;
    int availPageMem;

    // dialog window stuff
    int connectButtons_enabled;

}ChatCon;


typedef struct ChatShardCon {

    char    hostname[128];        
    char    ipAddress[128];    
    char    name[128];
    char    type[128];
    int     secSinceUpdate;    
    int     perfInfo;
    int     online;            
    int     sendBytes;        
    int     recvBytes;        
    int     sendMsgs;        
    int     recvMsgs;    
    BOOL    isShard;

}ChatShardCon;

static VarMap mapping[] = {
    {IDC_TXT_CHATSERVER, VMF_READ, 0, TOK_FIXEDSTR(ChatCon, chatSvrAddr), },
};

TokenizerParseInfo ChatConNetInfo[] =
{
    { "HostName",                TOK_FIXEDSTR(ChatCon, hostname), 0, TOK_FORMAT_LVWIDTH(100) | TOK_FORMAT_NOPATH},
    { "IP",                        TOK_FIXEDSTR(ChatCon, ipAddress), 0, TOK_FORMAT_LVWIDTH(100)},
    { "SecSinceLastUpdate",        TOK_INT(ChatCon, secSinceUpdate, 0), 0,    TOK_FORMAT_LVWIDTH(50)},

    { "TrackedPerfInfoCPU",        TOK_EMBEDDEDSTRUCT(ChatCon, perfInfo, TrackedPerformanceInfoCPU) },
    { "TotalPhys",                TOK_INT(ChatCon, totalPhysMem, 0), 0,    TOK_FORMAT_KBYTES | TOK_FORMAT_LVWIDTH(65)},
    { "AvailPhys",                TOK_INT(ChatCon, availPhysMem, 0), 0,    TOK_FORMAT_KBYTES | TOK_FORMAT_LVWIDTH(65)},
    { "TotalPage",                TOK_INT(ChatCon, totalPageMem, 0), 0,    TOK_FORMAT_KBYTES | TOK_FORMAT_LVWIDTH(65)},
    { "AvailPage",                TOK_INT(ChatCon, availPageMem, 0), 0,    TOK_FORMAT_KBYTES | TOK_FORMAT_LVWIDTH(65)},
    //    { "TrackedPerfInfo",    TOK_EMBEDDEDSTRUCT, offsetof(ChatCon, perfInfo), sizeof(TrackedPerformance), TrackedPerformanceInfo},

    { "TotalUsers",                TOK_INT(ChatCon, users, 0), 0,            TOK_FORMAT_LVWIDTH(70)},
    { "OnlineUsers",            TOK_INT(ChatCon, online, 0), 0,            TOK_FORMAT_LVWIDTH(90)},
    { "Channels",                TOK_INT(ChatCon, channels, 0), 0,        TOK_FORMAT_LVWIDTH(60)},
    { "Links",                    TOK_INT(ChatCon, links, 0), 0,            TOK_FORMAT_LVWIDTH(40)},
    { "Send",                    TOK_INT(ChatCon, sendBytes, 0), 0,        TOK_FORMAT_KBYTES | TOK_FORMAT_LVWIDTH(65)},
    { "Recv",                    TOK_INT(ChatCon, recvBytes, 0), 0,        TOK_FORMAT_KBYTES | TOK_FORMAT_LVWIDTH(65)},
    { "SendMsgs",                TOK_INT(ChatCon, sendMsgs, 0), 0,        TOK_FORMAT_LVWIDTH(60)},
    { "RecvMsgs",                TOK_INT(ChatCon, recvMsgs, 0), 0,        TOK_FORMAT_LVWIDTH(60)},
    { "CrossShardTraffic",        TOK_F32(ChatCon, crossShard, 0), 0,        TOK_FORMAT_LVWIDTH(155) | TOK_FORMAT_PERCENT},
    { "InvalidCommands",        TOK_F32(ChatCon, invalid, 0), 0,        TOK_FORMAT_LVWIDTH(155) | TOK_FORMAT_PERCENT},
    { "SvrMonitorsConnected",    TOK_INT(ChatCon, monitors, 0), 0,        TOK_FORMAT_LVWIDTH(120)},
    { "ReservedNames",            TOK_BOOL(ChatCon, reservedNames, 0), 0,    TOK_FORMAT_LVWIDTH(120)},


    { 0 }
};

TokenizerParseInfo ChatShardConNetInfo[] =
{
    { "HostName",                TOK_FIXEDSTR(ChatShardCon, hostname), 0, TOK_FORMAT_NOPATH | TOK_FORMAT_LVWIDTH(100)},
    { "IP",                        TOK_FIXEDSTR(ChatShardCon, ipAddress), 0, TOK_FORMAT_LVWIDTH(100)},
    { "Type",                    TOK_FIXEDSTR(ChatShardCon, type), 0, TOK_FORMAT_LVWIDTH(60)},
    { "ShardName",                TOK_FIXEDSTR(ChatShardCon, name), 0, TOK_FORMAT_LVWIDTH(100)},
//    { "SecSinceLastUpdate",        TOK_INT(ChatShardCon, secSinceUpdate),    LVMakeParam(50,0)},
    { "OnlineUsers",            TOK_INT(ChatShardCon, online, 0), 0,        TOK_FORMAT_LVWIDTH(90)},
    { "Send",                    TOK_INT(ChatShardCon, sendBytes, 0), 0,        TOK_FORMAT_KBYTES | TOK_FORMAT_LVWIDTH(65)},
    { "Recv",                    TOK_INT(ChatShardCon, recvBytes, 0), 0,        TOK_FORMAT_KBYTES | TOK_FORMAT_LVWIDTH(65)},
//    { "SendMsgs",                TOK_INT(ChatShardCon, sendMsgs),        LVMakeParam(60,0)},
//    { "RecvMsgs",                TOK_INT(ChatShardCon, recvMsgs),        LVMakeParam(60,0)},

    { 0 }
};

StashTable gChatShardTable;
ListView *lvChatServers;
ListView *lvChatShards;
ChatCon gChatCon;

//typedef struct
//{
//    int i;
//
//} ChatMonitorState;

char * getShardKey(char * ipAddrStr, int isShard)
{
    static char s_key[200];
    sprintf(s_key, "%s %d", ipAddrStr, isShard);
    return &s_key[0];
}

ChatShardCon *  receiveShardStatus(Packet * pak)
{        
    U32        ipAddr    = pktGetBitsPack(pak, 1);
    int        isShard = pktGetBitsPack(pak, 1);

    char*    ipAddrStr = makeIpStr(ipAddr);        

    char*    key = getShardKey(ipAddrStr, isShard);

    ChatShardCon * shard = stashFindPointerReturnPointer(gChatShardTable, key );

    if(!shard)
    {
        shard = calloc(1, sizeof(ChatShardCon));
        strcpy(shard->ipAddress, ipAddrStr);
        strcpy(shard->hostname, makeHostNameStr(ipAddr));
        listViewAddItem(lvChatShards, shard);
        stashAddPointer(gChatShardTable, key, shard, false);
    }

    shard->isShard = isShard;
    if(shard->isShard)
        strcpy(shard->type, "Shard");
    else
        strcpy(shard->type, "Public");

//    shard->secSinceUpdate    = pktGetBitsPack(pak, 1);    
    shard->online            = pktGetBitsPack(pak, 1);    

    strncpyt(shard->name, pktGetString(pak), SIZEOF2(ChatShardCon, name));

    shard->sendBytes        = pktGetBitsPack(pak, 1);        
    shard->recvBytes        = pktGetBitsPack(pak, 1);        

//    shard->sendMsgs            = pktGetBitsPack(pak, 1);        
//    shard->recvMsgs            = pktGetBitsPack(pak, 1);    

    listViewItemChanged(lvChatShards, shard);

    return shard;
}

void prepareChatShardUpdate(ListView* lv, void* shardData, void* pRemoveListData)
{
    ChatShardCon * shard = (ChatShardCon *)shardData;
    ChatShardCon *** pRemoveList = (ChatShardCon ***)pRemoveListData;
    if(shard)
        eaPush(pRemoveList, shard);
}

void receiveAllChatShards(Packet * pak)
{
    int i;
    ChatShardCon ** removeList = NULL;
    ChatShardCon * shard;

    listViewForEach(lvChatShards, prepareChatShardUpdate, (void*) &removeList);
    
    for(i=0;i<gChatCon.links;i++)
     {
        shard = receiveShardStatus(pak);
        eaFindAndRemove(&removeList, shard);
    }

    // prune
    for(i=0;i<eaSize(&removeList);i++)
    {
        shard = listViewDelItem(lvChatShards, listViewFindItem(lvChatShards, removeList[i]));
        stashRemovePointer(gChatShardTable, getShardKey(shard->ipAddress, shard->isShard), NULL);
        free(shard);
    }

    eaDestroy(&removeList);
}

void receiveChatServerStatus(Packet * pak)
{
    gChatCon.users            = pktGetBitsPack(pak, 1);
    gChatCon.channels        = pktGetBitsPack(pak, 1);
    gChatCon.online            = pktGetBitsPack(pak, 1);

    gChatCon.reservedNames    = pktGetBitsPack(pak, 1);

    gChatCon.crossShard        = pktGetF32(pak);
    gChatCon.invalid        = pktGetF32(pak);

     gChatCon.sendBytes         = pktGetBitsPack(pak, 1) / 1024;
    gChatCon.recvBytes         = pktGetBitsPack(pak, 1) / 1024;

    gChatCon.sendMsgs         = pktGetBitsPack(pak, 1);
    gChatCon.recvMsgs         = pktGetBitsPack(pak, 1);

    gChatCon.monitors         = pktGetBitsPack(pak, 1);

    // indiv shard info

    gChatCon.links            = pktGetBitsPack(pak, 1);

    receiveAllChatShards(pak);

    // memory info
    gChatCon.totalPhysMem = pktGetBits(pak, 32);
    gChatCon.availPhysMem = pktGetBits(pak, 32);
    gChatCon.totalPageMem = pktGetBits(pak, 32);
    gChatCon.availPageMem = pktGetBits(pak, 32);

    if (!pktEnd(pak))
        sdUnpackDiff(TrackedPerformanceInfo, pak, &gChatCon.perfInfo, NULL, false);

    // timing 
    gChatCon.lastUpdateSec = timerCpuSeconds();
    gChatCon.secSinceUpdate = 0;


    listViewItemChanged(lvChatServers, &gChatCon);

    // update server stats (sent to shard monitor)
    g_state.stats.chatServerConnected    = 1;
    g_state.stats.chatTotalUsers        = gChatCon.users;
    g_state.stats.chatOnlineUsers        = gChatCon.online;
    g_state.stats.chatChannels            = gChatCon.channels;
    g_state.stats.chatSecSinceUpdate    = gChatCon.secSinceUpdate;
    g_state.stats.chatLinks                = gChatCon.links;

}


BOOL chatMonConnected()
{
    return gChatCon.link.socket > 0;
}


int chatMonDisconnect()
{
    chatSetAutoConnect(false);
    if (!chatMonConnected())
        return 0;
    netSendDisconnect(&gChatCon.link,2);
    memset(&gChatCon, 0, sizeof(ChatCon));
    listViewItemChanged(lvChatServers, &gChatCon);
    return 1;
}



int chatMonHandleMsg(Packet *pak,int cmd, NetLink *link)
{
    switch (cmd) {

        case CHATMON_STATUS:
            receiveChatServerStatus(pak);
            break;
        case CHATMON_PROTOCOL_MISMATCH:
            {
                char buf[2000];

                int protocol = pktGetBits(pak, 32);

                sprintf(buf,"Protocol version mismatch:\n servermonitor: %d\n chatserver %d\n",DBSERVER_PROTOCOL_VERSION,protocol);
                
                MessageBoxA(NULL, buf, "Error", MB_ICONERROR);
                chatMonDisconnect();
            }
            break;

        default:
            assertmsgf(0,"unknown chat cmd %i",cmd);
            return 0;
    }
    return 1;
}


int chatMonConnect(void)
{
    Packet    *pak;
    NetLink * link = &gChatCon.link;

    if (!netConnect(link, gChatCon.chatSvrAddr, DEFAULT_CHATMON_PORT, NLT_TCP, 5, NULL)) {
        return 0;
    }

    pak = pktCreateEx(link, SVRMONTOCHATSVR_CONNECT);
    pktSendBits(pak, 32, CHATMON_PROTOCOL_VERSION);
    pktSend(&pak, link);

    netLinkSetMaxBufferSize(link, BothBuffers, 1*1024*1024); // Set max size to auto-grow to
    netLinkSetBufferSize(link, BothBuffers, 64*1024);

    strcpy(gChatCon.hostname,    makeHostNameStr(link->addr.sin_addr.S_un.S_addr));
    strcpy(gChatCon.ipAddress,    makeIpStr(link->addr.sin_addr.S_un.S_addr));

    gChatCon.lastUpdateSec = timerCpuSeconds();

    lnkFlush(link);
    //    received_update = false;
    return 1;
}




void fixChatConnectButtons(HWND hDlg)
{
    if (chatMonConnected()) {
        if (gChatCon.connectButtons_enabled) {
            EnableWindow(GetDlgItem(hDlg, IDC_TXT_CHATSERVER), FALSE);
            EnableWindow(GetDlgItem(hDlg, IDC_BTN_SENDALL), TRUE);
            EnableWindow(GetDlgItem(hDlg, IDC_BTN_CHATSVR_SHUTDOWN), TRUE);
            SetDlgItemTextA(hDlg, IDC_BTN_CHATCONNECT, "Disconnect");
            UpdateWindow(GetDlgItem(hDlg, IDC_LST_CHATSERVERS));
            gChatCon.connectButtons_enabled=false;
        }
    } else {
        if (!gChatCon.connectButtons_enabled) {
            EnableWindow(GetDlgItem(hDlg, IDC_TXT_CHATSERVER), TRUE);
            EnableWindow(GetDlgItem(hDlg, IDC_BTN_SENDALL), FALSE);
            EnableWindow(GetDlgItem(hDlg, IDC_BTN_CHATSVR_SHUTDOWN), FALSE);
            SetDlgItemTextA(hDlg, IDC_BTN_CHATCONNECT, "Connect");
            UpdateWindow(GetDlgItem(hDlg, IDC_LST_CHATSERVERS));
            gChatCon.connectButtons_enabled=true;
        }
    }
}



void chatMonConnectWrap(HWND hDlg)
{
    if (!chatMonConnect()) {
        MessageBoxA(hDlg, "Error connecting to ChatServer", "Error", MB_ICONWARNING);
        chatSetAutoConnect(false);
    } else {
        chatSetAutoConnect(true);
//        BringWindowToTop(hDlg);
    }
    fixChatConnectButtons(hDlg);
}

int chatMonGetNetDelay()
{
    if (!gChatCon.link.connected)
        return 0;
    return timerCpuSeconds() - gChatCon.link.lastRecvTime;
}

int chatMonConnectionLooksDead()
{
    return !chatMonConnected() ||  (chatMonGetNetDelay() > 30);
}


void chatMonNetTick()
{
    int sec; 
    static int s_wasConnected = 0;


    lnkFlushAll();
    netLinkMonitor(&gChatCon.link, 0, chatMonHandleMsg);
 
    if(chatMonConnected())
    {
        sec = (int) (timerCpuSeconds() - gChatCon.lastUpdateSec);
        if(sec != gChatCon.secSinceUpdate)
        {
            gChatCon.secSinceUpdate = sec;
            g_state.stats.chatSecSinceUpdate = gChatCon.secSinceUpdate;
            listViewItemChanged(lvChatServers, &gChatCon);
        }
    }
    else
        gChatCon.secSinceUpdate = 0;


    if(!chatMonConnectionLooksDead() && !s_wasConnected)
    {
        s_wasConnected = 1;
        g_state.stats.chatserver_in_trouble = 0;
        listViewSetItemColor(lvChatServers, &gChatCon, COLOR_CONNECTED);
    }
    else if(chatMonConnectionLooksDead() && s_wasConnected)
    {
        s_wasConnected = 0;
        g_state.stats.chatserver_in_trouble = 1;
        listViewSetItemColor(lvChatServers, &gChatCon, COLOR_TROUBLE);
    }
}

void chatSetAutoConnect(bool connect)
{
    gChatCon.connection_expected = connect;
}

BOOL chatMonExpectedConnection()
{
    return gChatCon.connection_expected;
}



void checkChatAutoReconnect(HWND hDlg)
{
    static int s_timer;
    const int RECONNECT_INTERVAL = 30; // (sec)
    
    if(!s_timer)
    {
        s_timer = timerAlloc();
        timerAdd(s_timer, RECONNECT_INTERVAL);
    }

    if (   ! chatMonConnected() 
        &&   gChatCon.connection_expected)
    {
        char buf[100];
        int sec = RECONNECT_INTERVAL - timerElapsed(s_timer);

        sprintf(buf, "Reconnecting... (%d s)", sec);

        SetDlgItemTextA(hDlg, IDC_BTN_CHATCONNECT, buf);        
        
        if(sec <= 0)
        {
            chatMonConnect();
            timerStart(s_timer);
        }
    }
}

void chatMonSendAdminMessage(char * msg, int type)
{
    if(msg && strlen(msg))
    {
        Packet * pak;
        char buf[2000];
        sprintf(buf, " %s", msg); // dumb hack to add leading space
        pak = pktCreateEx(&gChatCon.link, type);
        pktSendString(pak, buf);
        pktSend(&pak, &gChatCon.link);
    }

}

static void autoShutdownIfNecessary(HWND hDlg) {
    if (IsDlgButtonChecked(hDlg, IDC_CHK_AUTOSHUTDOWN) && chatMonConnected())
        chatMonSendAdminMessage("Auto-shutdown", SVRMONTOCHATSVR_SHUTDOWN);
}

void onTimer(HWND hDlg)
{
    static int s_lastConnected = -1;
    int connected = chatMonConnected();
    int count;
    static int s_lastCount = -1;

    chatMonNetTick();

    if(connected != s_lastConnected)
    {
        EnableWindow(GetDlgItem(hDlg, IDC_LST_CHATSERVERS), connected);
        EnableWindow(GetDlgItem(hDlg, IDC_LST_CHATSHARDS), connected);
        s_lastConnected = connected;
    }

    count = listViewGetNumItems(lvChatShards);
    if(count != s_lastCount)
    {
        SetDlgItemInt(hDlg, IDC_TXT_CONNECTED_CHAT_SHARDS, count, TRUE);
        EnableWindow(GetDlgItem(hDlg, IDC_LST_CHATSHARDS), (count != 0));
        s_lastCount = count;
    }

    UpdateWindow(GetDlgItem(hDlg, IDC_LST_CHATSERVERS));
     UpdateWindow(GetDlgItem(hDlg, IDC_LST_CHATSHARDS));
    checkChatAutoReconnect(hDlg);
    fixChatConnectButtons(hDlg);
    autoShutdownIfNecessary(hDlg);
}

static void onInitDialog(HWND hDlg)
{
    RECT rect;
    int i;

    if (!g_state.debug)
        ShowWindow(GetDlgItem(hDlg, IDC_CHK_AUTOSHUTDOWN), SW_HIDE);

    if (gChatShardTable)
        stashTableDestroy(gChatShardTable);
    gChatShardTable = stashTableCreateWithStringKeys(100,StashDeepCopyKeys);

    if (lvChatServers) listViewDestroy(lvChatServers);
    lvChatServers = listViewCreate();
    listViewInit(lvChatServers, ChatConNetInfo, hDlg, GetDlgItem(hDlg, IDC_LST_CHATSERVERS));
    listViewAddItem(lvChatServers, &gChatCon);

    if (lvChatShards) listViewDestroy(lvChatShards);
    lvChatShards = listViewCreate();
    listViewInit(lvChatShards, ChatShardConNetInfo, hDlg, GetDlgItem(hDlg, IDC_LST_CHATSHARDS));

    getNameList("ChatList");
    if( name_count > 0 ) {
        for (i=0; i<name_count; i++) 
            SendMessage(GetDlgItem(hDlg, IDC_TXT_CHATSERVER), CB_ADDSTRING, 0, (LPARAM)namelist[i]);
        if( gChatCon.chatSvrAddr[0] == 0 )
            SendMessage(GetDlgItem(hDlg, IDC_TXT_CHATSERVER), CB_SETCURSEL, 0, 0); // default to first selection
    }

    SetTimer(hDlg, 0, 10, NULL);

    EnableWindow(GetDlgItem(hDlg, IDC_LST_CHATSERVERS), 0);

    GetClientRect(hDlg, &rect); 
    // Initialize initial values
    doDialogOnResize(hDlg, (WORD)(rect.right - rect.left), (WORD)(rect.bottom - rect.top), IDC_ALIGNME, IDC_UPPERLEFT);
}

LRESULT CALLBACK DlgChatMonProc (HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    
    int i=0;

    switch (iMsg)
    {
    case WM_INITDIALOG:
        onInitDialog(hDlg);
        break;
    case WM_TIMER:
        onTimer(hDlg);
        break;
    case WM_COMMAND:

        switch (LOWORD(wParam))
        {
        case IDCANCEL:
            EndDialog(hDlg, 0);
            PostQuitMessage(0);
            return TRUE;
        case IDC_BTN_CHATCONNECT:
            getText(hDlg, &gChatCon, mapping, ARRAY_SIZE(mapping));
            addNameToList("ChatList", gChatCon.chatSvrAddr);
            if (!chatMonConnected()) {
                if(chatMonExpectedConnection())
                    chatSetAutoConnect(0);
                else
                    chatMonConnectWrap(hDlg);
            } else {
                chatMonDisconnect();
            }
            fixChatConnectButtons(hDlg);

            break;
        case IDC_BTN_SENDALL:
            chatMonSendAdminMessage(promptGetString(g_hInst, hDlg, "Admin message:", ""), SVRMONTOCHATSVR_ADMIN_SENDALL);
            break;
        case IDC_BTN_CHATSVR_SHUTDOWN:
            {
                if (MessageBoxA( 0, "This command will shutdown the Chatsever. Continue?", "Shutdown Confirmation", (MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2)) == IDYES)
                {
                    chatMonSendAdminMessage(promptGetString(g_hInst, hDlg, "Shutdown message: ", ""), SVRMONTOCHATSVR_SHUTDOWN);
                }
                break;
            }
        }
        break;
    case WM_SIZE:
        {
            WORD w = LOWORD(lParam);
            WORD h = HIWORD(lParam);
            //printf("server res: %dx%d\n", w, h);
            doDialogOnResize(hDlg, w, h, IDC_ALIGNME, IDC_UPPERLEFT);
//            SendMessage(state->hStatusBar, iMsg, wParam, lParam);
        }
        break;

    case WM_NOTIFY:
        {
            int idCtrl = (int)wParam;
            if(listViewOnNotify(lvChatServers, wParam, lParam, NULL))
            {
                return TRUE;    // signal that we processed the message
            }
        }
        break;
    }

    return FALSE;
}
