#ifndef __CHAT_ADMIN_H__
#define __CHAT_ADMIN_H__

#define  WIN32_LEAN_AND_MEAN
#include <utilitieslib/utils/wininclude.h>
#include "LangAdminUtil.h"

void setStatusBar(int elem, const char *fmt, ...);
void setProgressBar(F32 fraction);
void conPrintf(int type, const char *fmt, ...);

//int FilterMatch(char * str, char * pattern);
void stripNonAlphaNumeric(char * str);

void EnableControls(HWND hDlg, BOOL enable);
void LocalizeControls(HWND hDlg);

void EasyCheckMenuItem(int itemID, bool checked);

void ChatAdminReset();

extern HINSTANCE g_hInst;    // instance handle
extern bool g_ChatDebug;            // print text commands sent to/from chatserver
extern bool g_bShowChatTimestamps;
extern HWND g_hDlgMain;

enum {
    MSG_PRIVATE,
    MSG_SYS,
    MSG_DEBUG,
    MSG_ERROR,
    MSG_FEEDBACK,
    MSG_CHANNEL,
    MSG_ADMIN,
};



#endif //__CHAT_ADMIN_H__
