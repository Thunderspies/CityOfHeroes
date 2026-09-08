#include <utilitieslib/components/earray.h>
#include <utilitieslib/utils/file.h>
#include <utilitieslib/utils/ListView.h>
#include <utilitieslib/utils/utils.h>

#include "resource.h"
#include "serverMonitorCommon.h"
#include "shardMonitor.h"
#include "shardMonitorConfigure.h"
#include "shardMonitorConfigureNewShard.h"

TokenizerParseInfo shardMonitorConfigEntryInfo[] = {
    { "Name",        TOK_STRUCTPARAM | TOK_FIXEDSTR(ShardMonitorConfigEntry, name)},
    { "{",            TOK_START,        0 },
    { "IP",            TOK_MINBITS(32) | TOK_INT(ShardMonitorConfigEntry, ip, 0), 0, TOK_FORMAT_IP | TOK_FORMAT_LVWIDTH(100)},
    { "}",            TOK_END,            0 },
    { 0 }
};

TokenizerParseInfo shardMonitorConfigEntryDispInfo[] = {
    { "Name",        TOK_STRUCTPARAM | TOK_FIXEDSTR(ShardMonitorConfigEntry, name), 0, TOK_FORMAT_LVWIDTH(120)},
    { "IP",            TOK_MINBITS(32) | TOK_INT(ShardMonitorConfigEntry, ip, 0), 0, TOK_FORMAT_IP | TOK_FORMAT_LVWIDTH(100)},
    { 0 }
};

TokenizerParseInfo shardMonitorConfigInfo[] = {
    { "Shard",        TOK_STRUCT(ShardMonitorConfig, shardList, shardMonitorConfigEntryInfo) },
    { 0 }
};

static INT_PTR CALLBACK shardMonConfigureDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

ShardMonitorConfig shmConfig = {0};
ListView *lvShmConfigure = NULL;
char *g_configfile=NULL;

void shardMonLoadConfig(char *configfile)
{
    if (shmConfig.shardList) {
        ParserDestroyStruct(shardMonitorConfigInfo, &shmConfig);
    }
    ParserLoadFiles(NULL, configfile, NULL, 0, shardMonitorConfigInfo, &shmConfig, NULL, NULL, NULL);
}

void shardMonSaveConfig(char *configfile)
{
    char bak[MAX_PATH];
    strcpy(bak, configfile);
    strcat(bak, ".bak");
    fileForceRemove(bak);
    rename(configfile, bak);
    ParserWriteTextFile(configfile, shardMonitorConfigInfo, &shmConfig, 0, 0);
}

void shardMonConfigure(HINSTANCE hinst, HWND hwndParent, char *configfile)
{
    int res;

    g_configfile = configfile;
    res = DialogBox (hinst, MAKEINTRESOURCE (IDD_DLG_SHARDMON_CONFIGURE), hwndParent, shardMonConfigureDlgProc);
}

void shardMonConfigAdd(HWND hDlg)
{
    char name[128]={0};
    U32 ip=0;
    if (shardMonConfigureNewShard(g_hInst, hDlg, name, &ip)) {
        ShardMonitorConfigEntry *newEntry = ParserAllocStruct(sizeof(ShardMonitorConfigEntry));
        newEntry->ip = ip;
        Strncpyt(newEntry->name, name);
        eaPush(&shmConfig.shardList, newEntry);
        listViewAddItem(lvShmConfigure, newEntry);
    }
}

static void editEntry(ListView *lv, ShardMonitorConfigEntry *smce, HWND hDlg)
{
    if (shardMonConfigureNewShard(g_hInst, hDlg, smce->name, &smce->ip)) {
        listViewItemChanged(lvShmConfigure, smce);
    }
}

static void shardMonConfigEdit(HWND hDlg)
{
    int count = listViewDoOnSelected(lvShmConfigure, NULL, NULL);
    if (count==1) {
        listViewDoOnSelected(lvShmConfigure, editEntry, hDlg);
    }
}

static void removeEntry(ListView *lv, ShardMonitorConfigEntry *smce, HWND hDlg)
{
    eaRemove(&shmConfig.shardList, eaFind(&shmConfig.shardList, smce));
    listViewDelItem(lvShmConfigure, listViewFindItem(lvShmConfigure, smce));
}

static void shardMonConfigRemove(HWND hDlg)
{
    listViewDoOnSelected(lvShmConfigure, removeEntry, hDlg);
}

static void moveUp(ListView *lv, ShardMonitorConfigEntry *smce, HWND hDlg)
{
    int i;
    int index = eaFind(&shmConfig.shardList, smce);
    if (index==-1)
        return;
    if (index==0)
        return;
    shmConfig.shardList[index] = shmConfig.shardList[index-1];
    shmConfig.shardList[index-1] = smce;
    listViewDelAllItems(lvShmConfigure, NULL);
    for (i=0; i<eaSize(&shmConfig.shardList); i++) {
        listViewAddItem(lvShmConfigure, shmConfig.shardList[i]);
    }
    listViewSetSelected(lvShmConfigure, smce, true);
}

static void shardMonConfigMoveUp(HWND hDlg)
{
    int count = listViewDoOnSelected(lvShmConfigure, NULL, NULL);
    if (count==1) { // This will die horribly on more than 1
        listViewDoOnSelected(lvShmConfigure, moveUp, hDlg);
    }
}

static void moveDown(ListView *lv, ShardMonitorConfigEntry *smce, HWND hDlg)
{
    int i;
    int index = eaFind(&shmConfig.shardList, smce);
    if (index==-1)
        return;
    if (index==eaSize(&shmConfig.shardList)-1)
        return;
    shmConfig.shardList[index] = shmConfig.shardList[index+1];
    shmConfig.shardList[index+1] = smce;
    listViewDelAllItems(lvShmConfigure, NULL);
    for (i=0; i<eaSize(&shmConfig.shardList); i++) {
        listViewAddItem(lvShmConfigure, shmConfig.shardList[i]);
    }
    listViewSetSelected(lvShmConfigure, smce, true);
}


static void shardMonConfigMoveDown(HWND hDlg)
{
    int count = listViewDoOnSelected(lvShmConfigure, NULL, NULL);
    if (count==1) { // This will die horribly on more than 1
        listViewDoOnSelected(lvShmConfigure, moveDown, hDlg);
    }
}



static INT_PTR CALLBACK shardMonConfigureDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    int i;
    char *configfile = g_configfile;
    switch (iMsg) {
    case WM_INITDIALOG:
        if (lvShmConfigure) listViewDestroy(lvShmConfigure);
        lvShmConfigure = listViewCreate();
        listViewInit(lvShmConfigure, shardMonitorConfigEntryDispInfo, hDlg, GetDlgItem(hDlg, IDC_LST_SHARDS));
        listViewSetSortable(lvShmConfigure, false);
        shardMonLoadConfig(configfile);
        for (i=0; i<eaSize(&shmConfig.shardList); i++) {
            listViewAddItem(lvShmConfigure, shmConfig.shardList[i]);
        }

        break;
    case WM_NOTIFY:
        {
            int idCtrl = (int)wParam;
            int count;
            listViewOnNotify(lvShmConfigure, wParam, lParam, NULL);
            count = listViewDoOnSelected(lvShmConfigure, NULL, NULL);
            EnableWindow(GetDlgItem(hDlg, IDC_BTN_EDIT), (count == 1));
            EnableWindow(GetDlgItem(hDlg, IDC_BTN_REMOVE), (count >= 1));
            EnableWindow(GetDlgItem(hDlg, IDC_BTN_MOVEUP), (count == 1));
            EnableWindow(GetDlgItem(hDlg, IDC_BTN_MOVEDOWN), (count == 1));
        }
        break;

    case WM_COMMAND:

        switch (LOWORD (wParam))
        {
        case IDOK:
            // Syncronize Data and exit
            shardMonSaveConfig(configfile);
            EndDialog(hDlg, 0);
            break;
        case IDCANCEL:
            // Reload data and exit
            shardMonLoadConfig(configfile);
            EndDialog(hDlg, 0);
            break;
        case IDC_BTN_ADD:
            shardMonConfigAdd(hDlg);
            break;
        case IDC_BTN_EDIT:
            shardMonConfigEdit(hDlg);
            break;
        case IDC_BTN_REMOVE:
            shardMonConfigRemove(hDlg);
            break;
        case IDC_BTN_MOVEUP:
            shardMonConfigMoveUp(hDlg);
            break;
        case IDC_BTN_MOVEDOWN:
            shardMonConfigMoveDown(hDlg);
            break;
            
        }
        break;

    }
    return FALSE;
}

