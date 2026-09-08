#include <container.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/network/netio_core.h>
#include <utilitieslib/utils/ListView.h>
#include <utilitieslib/utils/MemoryMonitor.h>
#include <utilitieslib/utils/RegistryReader.h>
#include <utilitieslib/utils/textparser.h>
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/winutil.h>

#include <assert.h>
#include <commctrl.h>
#include <direct.h>
#include <process.h>
#include <shellapi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#include "relaycomm.h"
#include "resource.h"
#include "relay_utils.h"
#include "prompt.h"
#include "serverMonitor.h"
#include "serverMonitorCommon.h"
#include "shardMonitorComm.h"

extern ListView *lvShardRelays;
extern ListView *lvSmShards2;
HWND g_hShardRelayStatusBar;

void onShardRelayTick(HWND hDlg)
{
    // update list view - will be grayed out if no relays are connected
    static int s_lastCount = -1;
    static int lock=0;

    int count = listViewGetNumItems(lvShardRelays);
    if(count > 5)
        count -= 5;  // quick hack to skip over MIN/MAX/AVG/TOTAL rows

    if (lock!=0)
        return;
    lock++;

    if(count != s_lastCount)
    {
        SetDlgItemInt(hDlg, IDC_RELAY_CONNECTED_COUNT, count, TRUE);
        EnableWindow(GetDlgItem(hDlg, IDC_LST_RELAY_CLIENTS), (count != 0));
        s_lastCount = count;
    }


    // handle networking stuff
    if(count)
    {
        // TODO: check status of commands sent to servers

        UpdateWindow(GetDlgItem(hDlg, IDC_LST_RELAY_CLIENTS));
    }
    lock--;
}

void sendBatchFileToSvrMon(ListView* lv, void* structptr, void* fileData)
{
    FileAllocInfo * file = (FileAllocInfo *)fileData;
    NetLink * link = &(((ServerStats*) structptr)->link);

    if(link)
    {
        // send the file to each relay
        Packet * pak = pktCreate();
        pktSendBitsPack(pak, 1,CMDRELAY_REQUEST_RUN_BATCH_FILE);
        pktSendBitsPack(pak, 1, file->size);
        pktSendBitsArray(pak, (file->size * 8), file->data);
        pktSend(&pak,link);
        lnkFlush(link);
    }
}



void onShardRelayRunBatchFile()
{
    FileAllocInfo file;

    if(OpenAndAllocFile("Select Batch File", "*.bat", &file))
    {
        listViewDoOnSelected(lvShardRelays, sendBatchFileToSvrMon, &file);
        free(file.data);
    }
}


// send an "apply patch" command to all server monitors
void onShardRelayApplyPatch(ListView* lv, void* structptr, void* updateSvrData)
{
    char * updateSvr = (char *)updateSvrData;
    NetLink * link = &(((ServerStats*) structptr)->link);
    
    if(link)
    {
        Packet    *pak = pktCreateEx(link,SVRMONSHARDMON_RELAY_APPLY_PATCH);
        pktSendString(pak, updateSvr);
        pktSend(&pak, link);
        lnkFlush(link);
    }
}

void ShardRelaySendCommand(ListView* lv, void* statData, void* cmdData)
{
    ServerStats * stat = (ServerStats *)statData;
    int cmd = (int)(intptr_t)cmdData;
    NetLink * link = &(stat->link);
    Packet    *pak = pktCreateEx(link,cmd);
    pktSend(&pak, link);
    lnkFlush(link);
}

void shardRelayInit()
{
    int i;

    if (!lvShardRelays) // JE: Added this because the ShardMonitor would crash when you clicked Configure
        return;

    assert(lvShardRelays);

    listViewDelAllItems(lvShardRelays, NULL);

    // copy the contents of lvSmShards2 into lvShardRelays (This only needs to be done once,
    // new additions will go into both lists)
    for(i=0; i<listViewGetNumItems(lvSmShards2);i++)
    {
        ServerStats * stat = (ServerStats*) listViewGetItem(lvSmShards2, i);
        if( stat != LISTVIEW_EMPTY_ITEM
            && !stat->special)
            listViewAddItem(lvShardRelays, stat);
    }
}


LRESULT CALLBACK DlgShardRelayProc (HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    int i=0;

    switch (iMsg)
    {
    case WM_INITDIALOG:
        {
            assert(!lvShardRelays);    // should be initialized in shardMonInit() to stay consistent with shard data
            lvShardRelays = listViewCreate();
            listViewInit(lvShardRelays, ShardRelayDispInfo, hDlg, GetDlgItem(hDlg, IDC_LST_RELAY_CLIENTS));
            listViewSetSortable(lvShardRelays, false);
            
            shardRelayInit();

            getNameList("UpdateSvr");
            for (i=0; i<name_count; i++) 
                SendMessageA(GetDlgItem(hDlg, IDC_COMBO_RELAY_UPDATE_SVR), CB_ADDSTRING, 0, (LPARAM)namelist[i]);

            initText(hDlg, NULL, relayMapping, ARRAY_SIZE(relayMapping));

            SetDlgItemTextA(hDlg, IDC_STATIC_CONNECTED_RELAYS, "Connected Shards:");

            // Status bar stuff
            g_hShardRelayStatusBar = CreateStatusWindowA( WS_CHILD | WS_VISIBLE, "Status bar", hDlg, 1);
            {
                int temp[4];
                temp[0]=100;
                temp[1]=200;
                temp[2]=300;
                temp[3]=-1;
                SendMessageA(g_hShardRelayStatusBar, SB_SETPARTS, ARRAY_SIZE(temp), (LPARAM)temp);
            }

            SetTimer(hDlg, 0, 1000, NULL);
        }
        return FALSE;

    case WM_TIMER:
        onShardRelayTick(hDlg);
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

        xcase IDC_RELAY_APPLY_PATCH:
            getText(hDlg, NULL, relayMapping, ARRAY_SIZE(relayMapping));
            addNameToList("UpdateSvr", g_updateServerAddr);
            listViewDoOnSelected(lvShardRelays, onShardRelayApplyPatch, g_updateServerAddr);
            return TRUE;
        xcase IDC_RELAY_START_ALL:
            listViewDoOnSelected(lvShardRelays, ShardRelaySendCommand, (void*)(intptr_t)SVRMONSHARDMON_RELAY_START_ALL);
            return TRUE;
        xcase IDC_RELAY_STOP_ALL:
            listViewDoOnSelected(lvShardRelays, ShardRelaySendCommand, (void*)(intptr_t)SVRMONSHARDMON_RELAY_STOP_ALL);
            return TRUE;
        xcase IDC_RELAY_KILLALL_MAPSERVER:
            listViewDoOnSelected(lvShardRelays, ShardRelaySendCommand, (void*)(intptr_t)SVRMONSHARDMON_RELAY_KILL_ALL_MAPSERVER);
            return TRUE;
        xcase IDC_RELAY_KILLALL_LAUNCHER:
            listViewDoOnSelected(lvShardRelays, ShardRelaySendCommand, (void*)(intptr_t)SVRMONSHARDMON_RELAY_KILL_ALL_LAUNCHER);
            return TRUE;
        xcase IDC_RELAY_START_LAUNCHER:
            listViewDoOnSelected(lvShardRelays, ShardRelaySendCommand, (void*)(intptr_t)SVRMONSHARDMON_RELAY_START_LAUNCHER);
            return TRUE;
        xcase IDC_RELAY_START_DBSERVER:
            listViewDoOnSelected(lvShardRelays, ShardRelaySendCommand, (void*)(intptr_t)SVRMONSHARDMON_RELAY_START_DBSERVER);
            return TRUE;
        xcase IDC_BUTTON_RELAY_CANCEL_ALL:
            listViewDoOnSelected(lvShardRelays, ShardRelaySendCommand, (void*)(intptr_t)SVRMONSHARDMON_RELAY_CANCEL_ALL);
            return TRUE;
        xcase IDC_RELAY_SELECT_ALL:
            listViewSelectAll(lvShardRelays, TRUE);
            return TRUE;
        xcase IDC_RELAY_SELECT_NONE:
            listViewSelectAll(lvShardRelays, FALSE);
            return TRUE;
        xcase IDC_RELAY_RUN_BATCH_FILE:
            onShardRelayRunBatchFile();
            return TRUE;
        }
        break;
    case WM_SIZE:
        {
            WORD w = LOWORD(lParam);
            WORD h = HIWORD(lParam);
            //SendMessage(hStatusBar, iMsg, wParam, lParam);
            doDialogOnResize(hDlg, w, h, IDC_RELAY_ALIGNME, IDC_RELAY_UPPERLEFT);
        }
        break;
    case WM_NOTIFY:
        {
            if(lvShardRelays)
            {
                int idCtrl = (int)wParam;
                return listViewOnNotify(lvShardRelays, wParam, lParam, NULL);
            }
        }
        break;
    }
    return FALSE;
}











