#ifndef _SERVER_MONITOR_CRASH_MSG_H
#define _SERVER_MONITOR_CRASH_MSG_H

#include <utilitieslib/utils/ListView.h>
#include <winsock2.h>
#include <windows.h>

typedef struct MapCon MapCon;

// this callback is given to List View functions to place the selected item's text
// in the Status dialog's edit control
void updateCrashMsgText(ListView *lv, MapCon* con, void *unused);
void updateCrashMsg(HWND parent, char *text);

INT_PTR CALLBACK DlgSvrMonCrashMsgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);



#endif // _SERVER_MONITOR_CRASH_MSG_H
