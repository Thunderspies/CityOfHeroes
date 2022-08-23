#include <utilitieslib/stdtypes.h>
#include "SlaveLauncher.h"
#include "PipeClient.h"
#include <conio.h>
#include <time.h>
#include <process.h>
#include <stdio.h>
#include "stddef.h"
#include <utilitieslib/utils/error.h>
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/sysutil.h>
#include <utilitieslib/utils/memcheck.h>
#include <utilitieslib/utils/file.h>
#include <utilitieslib/utils/MemoryMonitor.h>
#include <utilitieslib/utils/StringUtil.h>


char *poss_clients[] = {
    ".\\TestClient.exe",
    ".\\bin\\TestClient.exe",
};

void launchNewClient(char *params) {
    int ret;
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    char cmdline[MAX_PATH]={0};
    static int client_loc=-1;

    if (client_loc==-1) {
        sprintf(cmdline, "cmd /k echo Could not locate test client");
        for (client_loc=0; client_loc<ARRAY_SIZE(poss_clients); client_loc++) {
            if (fileExists(poss_clients[client_loc])) {
                break;
            }
        }
    }
    if (client_loc<ARRAY_SIZE(poss_clients)) {
        sprintf(cmdline, "%s %s", poss_clients[client_loc], params);
    }

    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );
    ret = CreateProcess(NULL, UTF8ToWideStrConvertStatic(cmdline), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);

    if (ret) {
        // We don't care about our children.  In fact, we're horrible parents.
        CloseHandle(pi.hProcess);
    }
}



extern PipeClient pc;

void startSlaveLauncher(const char *server) {
    while (true) {
        int trys=0;
        loadstart_printf("PipeClientCreate(\"TestClientLauncher\")...");
        do {
            pc = PipeClientCreate("TestClientLauncher", server);
            if (pc==NULL) {
                if (trys) printf("\r"); else printf("\n");
                printf("%d: Could not open pipe \\\\%s\\pipe\\TestClientLauncher, trying again...", ++trys, server);
                Sleep(1000);
                while (kbhit()) {
                    int c = _getch();
                    if (c==27) return;
                    if (c=='m') memMonitorDisplayStats();
                }
            }
        } while (pc==NULL);
        if (trys) printf("\n");
        loadend_printf("Connected");

        PipeClientSendMessage(pc, "PID: %d", _getpid());
        PipeClientSendMessage(pc, "Host: %s", getComputerName());
        PipeClientSendMessage(pc, "Status: SlaveLnchr");
        while (pc->connected==PCS_CONNECTED) {
            char *s;
            Sleep(1000);
            do {
                s = PipeClientGetMessage(pc);
                if (s) {
                    // Handle message here (launch clients)
                    printf("Message: %s\n", s);
                    if (strnicmp(s, "Launch ", 7)==0) {
                        s+=strlen("Launch ");
                        launchNewClient(s);
                    } else if (strnicmp(s, "Exec ", 5)==0) {
                        s+=strlen("Exec ");
                        system_detach(s, 0);
                    }
                }
            } while (s);
            while (kbhit()) {
                int c = _getch();
                if (c==27) return;
                if (c=='m') memMonitorDisplayStats();
            }
        }
        PipeClientDestroy(pc);
        printf("Was disconnected, re-connecting...\n");
    }
}
