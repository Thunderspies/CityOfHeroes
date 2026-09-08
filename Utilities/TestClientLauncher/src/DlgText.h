#ifndef DLGTEXT_H
#define DLGTEXT_H

#include <utilitieslib/utils/ListView.h>

typedef struct DLGHDR_TAB {
    HWND hwndDisplay;
    DLGTEMPLATE *apRes; 
    void * dlgProc;
    TCHAR title[256];
    BOOL isConnected;
    ChildInfo *ci;
    ListView *plv;
} DLGHDR_TAB;

typedef struct tag_dlghdr { 
    HWND hwndTab;       // tab control 
    HWND hwndCurrent;   // current child dialog box
    HWND hwndParent;    // Parent dialog box
    DLGHDR_TAB **eaTabs;
} DLGHDR;

typedef enum {
    BMS_INACTIVE = 0,
    BMS_INIT,
    BMS_CONNECTING,
    BMS_BATCH,
    BMS_DONE,
} BatchModeState;


typedef struct BatchModeData {
    BatchModeState state;
    int tabid;
    int logouttimer;
    HWND tabhandle;
    bool debug;
    TCHAR *auth;
    TCHAR *pw;
    TCHAR *server;
    TCHAR *character;
    TCHAR *batchfile;
    TCHAR *logfile;
} BatchModeData;

extern BatchModeData batchmode;


typedef struct {
    TCHAR *servername;
    TCHAR *serverauth;
    TCHAR *loginname;
    TCHAR *loginchar;
     TCHAR *clientpath; // DEPRECATED
    TCHAR *password;
} serverinfo_struct;

#define MAX_SERVERS 32
extern serverinfo_struct server_list[MAX_SERVERS];
extern int num_servers;


void DlgTextMainDoDialog(void);
void WINAPI OnTextMainDialogInit(HWND hwndDlg);
//void InsertTab(DLGHDR *pHdr, int resource, void *dlgProc, char *title, bool showme);

void onClientUpdateCommand(HWND tab);

#define CC_LAUNCHER RGB(0,128,128),RGB(200,200,200)

int chatprintf(HWND tab, int chattype, COLORREF ccfore, COLORREF ccback, char *fmt, ...);
int chatprintfW(HWND tab, int chattype, COLORREF ccfore, COLORREF ccback, wchar_t *fmt, ...);
BOOL isValidLogin(HWND tab);
char* tabVersionRequest(HWND tab);
void periodicUpdate(HWND tab);
void buildQuickCommand(HWND tab, TCHAR *strtarg);

DLGHDR_TAB* getTabStruct(HWND tab);

#endif
