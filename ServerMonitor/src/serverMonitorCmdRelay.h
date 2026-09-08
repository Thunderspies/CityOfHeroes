#ifndef _SERVER_MONITOR_CMD_RELAY_H
#define _SERVER_MONITOR_CMD_RELAY_H

#include <winsock2.h>
#include <windows.h>
#include "container.h"
#include "relay_types.h"
#include <utilitieslib/utils/ListView.h>
#include "serverMonitor.h"

#define RELAY_TICK_SEC    (2)        // update timer interval in seconds

LRESULT CALLBACK DlgCmdRelayProc (HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
char *OpenFileDlg(char * title, char *fileMask,char *fileName);

static StaticDefineInt cmdRelayStatusDef[] = {
    DEFINE_INT
    { "Ready",                CMDRELAY_CMDSTATUS_READY },
    { "Busy",                CMDRELAY_CMDSTATUS_BUSY },
    { "Error",                CMDRELAY_CMDSTATUS_ERROR },
    { "ApplyingPatch",        CMDRELAY_CMDSTATUS_APPLYING_PATCH},
    { "ExecutingCommand",    CMDRELAY_CMDSTATUS_CUSTOM_CMD},
    DEFINE_END
};


// Launcher
typedef struct CmdRelayCon
{
    char ipAddress[128];
    char version[128];
    char hostname[128];
    CmdRelayCmdStatus status;
    char statusStr[128];
    char lastMsg[128];
    int type;        // whether this is a DBSERVER or MAPSERVER relay
    char typeStr[128];

    int launcherCount;
    int mapserverCount;
    int dbserverCount;
    int crashedMapCount;
    int arenaserverCount;
    int crashedArenaCount;
    int logserverCount;

    int protocol;

    NetLink * link;

    int actionResult;

    int lastUpdate;        // number of seconds since last update

} CmdRelayCon;

void cmdRelayAutoApplyPatch(Packet * pak);

extern BOOL g_bRelayDevMode;

void cmdRelayCmdToAllClients(ListViewCallbackFunc callback);

void onRelayStartDbServer(ListView* lv, void* structptr, void* cmdData);
void onRelayStartLauncher(ListView* lv, void* structptr, void* cmdData);
void onRelayKillAllLauncher(ListView* lv, void* structptr, void* cmdData);
void onRelayKillAllMapserver(ListView* lv, void* structptr, void* cmdData);
void onRelayCancelAll(ListView* lv, void* structptr, void* cmdData);
void onRelayStartAll(ListView* lv, void* structptr, void* cmdData);
void onRelayStopAll(ListView* lv, void* structptr, void* cmdData);

void updateServerRelayStats(ServerStats *stats);

#endif // _SERVER_MONITOR_CMD_RELAY_H
