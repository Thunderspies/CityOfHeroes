#ifndef _CHAT_MONITOR_H
#define _CHAT_MONITOR_H

#include <utilitieslib/network/net_structdefs.h>
#include <utilitieslib/utils/textparser.h>
#include <utilitieslib/utils/ListView.h>

#include <winsock2.h>
#include <windows.h>


#define CHATMON_PROTOCOL_VERSION    ( 20050106 )

// Commands from ChatServer to ServerMonitor
enum
{
    CHATMON_STATUS = COMM_MAX_CMD, // Receive full status update from Chatserver
    CHATMON_PROTOCOL_MISMATCH,
};

// Commands from ServerMonitor to ChatServer
enum
{
    SVRMONTOCHATSVR_ADMIN_SENDALL = COMM_MAX_CMD, // Receive full status update from Chatserver
    SVRMONTOCHATSVR_CONNECT,
    SVRMONTOCHATSVR_SHUTDOWN,
};


LRESULT CALLBACK DlgChatMonProc (HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

extern TokenizerParseInfo ChatConNetInfo[];

void chatSetAutoConnect(bool connect);
BOOL chatMonConnected();
BOOL chatMonExpectedConnection();
int chatMonConnect(void);

#endif // _CHAT_MONITOR_H
