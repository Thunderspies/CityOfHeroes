#ifndef _RELAY_UTILS_H
#define _RELAY_UTILS_H

#include <utilitieslib/utils/ListView.h>
#include "serverMonitorCommon.h"
#include "serverMonitorCmdRelay.h"
#include <utilitieslib/network/netio_core.h>
#include "relay_types.h"
#include "resource.h"

typedef struct 
{
    int size;
    char * data;
} FileAllocInfo;

BOOL OpenAndAllocFile(char * title, char * pattern, FileAllocInfo * file);

void sendBatchFileToClient(ListView* lv, void* conData, void* fileData);

char *OpenFileDlg(char * title, char *fileMask,char *fileName);


extern char g_updateServerAddr[512];
extern char g_customCmd[1024];


static VarMap relayMapping[2] = {
    {IDC_COMBO_RELAY_UPDATE_SVR, true, 0, TOK_STRING_X, (size_t)&g_updateServerAddr, sizeof(g_updateServerAddr)},
    {IDC_COMBO_RELAY_CUSTOM_CMD, true, 0, TOK_STRING_X, (size_t)&g_customCmd, sizeof(g_customCmd) },
};

#endif // _RELAY_UTILS_H
