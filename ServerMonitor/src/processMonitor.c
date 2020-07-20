#include <utilitieslib/components/EString.h>
#include <utilitieslib/utils/process_util.h>
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/winutil.h>

#include <direct.h>
#include <psapi.h>
#include <Shellapi.h>

#include "processMonitor.h"
#include "serverMonitorCommon.h"
#include "resource.h"

static char cwd[MAX_PATH];

static char *statusToName[] = {
    "Not Running",
    "Running",
    "Crashed",
    "Not monitored",
};

ProcessMonitorEntry processmonitors[] = {
    {"AuthServer.exe", "AuthServer.exe", 1, 1, 0, 15, -1, 0, IDC_TXT_STATUS0},
    {"DbServer.exe", "DbServer.exe -startall", 1, 1, 0, 15, -1, 0, IDC_TXT_STATUS1},
    {"Launcher.exe", "Launcher.exe -noversioncheck", 1, 1, 0, 15, -1, 0, IDC_TXT_STATUS2},
//    {"LogServer.exe", "LogServer.exe", 1, 0, 0, 15, -1, 0, IDC_TXT_STATUS2},
//    {"ChatServer.exe", "ChatServer.exe", 1, 0, 0, 15, -1, 0, IDC_TXT_STATUS3},
//    {"Mapserver.exe", "Mapserver.exe -chatservernames localhost", 1, 0, 0, 15, -1, 0, IDC_TXT_STATUS4},
};

static VarMap mapping[] = {
#define SETUP_MAPPING(number)                                                                                        \
    {IDC_TXT_EXENAME##number, VMF_WRITE, 0,                        TOK_STRING_X, (size_t)&processmonitors[number].exename[0], ARRAY_SIZE(processmonitors[number].exename)},                            \
    {IDC_TXT_CMDLINE##number, VMF_READ,  "Proc" #number "CmdLine", TOK_STRING_X, (size_t)&processmonitors[number].cmdline[0], ARRAY_SIZE( processmonitors[number].cmdline )  },\
    {IDC_CHK_MONITOR##number, VMF_READ,  "Proc" #number "Monitor", TOK_BOOL_X,   (size_t)&processmonitors[number].monitor, },        \
    {IDC_CHK_START##number,   VMF_READ,  "Proc" #number "Start",   TOK_BOOL_X,   (size_t)&processmonitors[number].start, },            \
    {IDC_CHK_RESTART##number, VMF_READ,  "Proc" #number "Restart", TOK_BOOL_X,   (size_t)&processmonitors[number].restart, },        \
    {IDC_TXT_SECONDS##number, VMF_READ,  "Proc" #number "Seconds", TOK_INT_X,    (size_t)&processmonitors[number].seconds, },
    SETUP_MAPPING(0)
    SETUP_MAPPING(1)
    SETUP_MAPPING(2)
//    SETUP_MAPPING(3)
};

// keeps a running count in the LPARAM parameter
// criteria for determining a crashed mapserver: exe name is "mapserver.exe" (case-insensitive) and window title is "City Of Heroes"
static BOOL CALLBACK EnumProcCountCrashedServers(HWND hwnd, LPARAM lParam)
{
    DWORD processID;
    int * count = (int*) lParam; 
    int j;

    if(GetWindowThreadProcessId(hwnd, &processID))
    {
        ProcessMonitorEntry *entry=NULL;
        // Find match for processID
        for (j=0; j<ARRAY_SIZE(processmonitors); j++) {
            if (!processmonitors[j].monitor)
                continue;
            if (processmonitors[j].processID == processID) {
                entry = &processmonitors[j];
                break;
            }
        }

        if (entry)
        {
            char title[500];
            if(GetWindowTextA(hwnd, title, ARRAY_SIZE(title)))
            {
                if(stricmp(title, "City Of Heroes")==0)
                {
                    if (entry->status != PMS_CRASHED) {
                        printf("Detected %s (PID: %d) crashed", entry->exename, entry->processID);
                    }
                    entry->crashed = 1;
                }
            }
        }
    }
    return TRUE;    // signal to continue processing further windows
}


static void CountCrashedServers()
{
    EnumWindows(EnumProcCountCrashedServers, (LPARAM)NULL);
}

static char *s_make64Name(char *name32)
{
    static char name64[MAX_PATH] = {0};
    char tmpName32[MAX_PATH] = {0};
    char tmpExtension32[MAX_PATH] = {0};
    char *extension = strchr(name32, '.');
    if(extension)
    {
        int charsBeforePeriod = extension-name32;
        strncpy(tmpName32, name32, charsBeforePeriod);
        strcpy(tmpExtension32, extension);
        sprintf(name64, "%s64%s", tmpName32, tmpExtension32);
    }
    else
    {
        sprintf(name64, "%s64", name32);
    }
    return name64;
}

static int s_matchesProcess(char *processName, char *matchProcess)
{
    char *name64 = s_make64Name(matchProcess);
    return (stricmp(processName, matchProcess)==0 ||
        stricmp(processName, name64)==0);
}

static void updateRunningList()
{
    DWORD aProcesses[1024], cbNeeded, cProcesses;
    unsigned int i, j;
    int founddb=0, foundlauncher=0, foundlogserver=0;

    if ( !EnumProcesses( aProcesses, sizeof(aProcesses), &cbNeeded ) )
        return;

    // Calculate how many process identifiers were returned.
    cProcesses = cbNeeded / sizeof(DWORD);
    for (j=0; j<ARRAY_SIZE(processmonitors); j++) {
        processmonitors[j].running = 0;
        processmonitors[j].crashed = 0;
        processmonitors[j].processID = 0;
    }

    for ( i = 0; i < cProcesses; i++ ) {
        char szProcessName[MAX_PATH] = "unknown";

        // Get a handle to the process.
        HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION |
            PROCESS_VM_READ,
            FALSE, aProcesses[i] );

        // Get the process name.
        if (NULL != hProcess )
        {
            HMODULE hMod;

            if ( EnumProcessModules( hProcess, &hMod, sizeof(hMod), 
                &cbNeeded) )
            {
                GetModuleBaseNameA( hProcess, hMod, szProcessName, 
                    sizeof(szProcessName) );
            }
            else {
                CloseHandle( hProcess );
                continue;
            }
        }
        else continue;

        // Check the process name.
        for (j=0; j<ARRAY_SIZE(processmonitors); j++) {
            if (s_matchesProcess(szProcessName, processmonitors[j].exename)) {
                if (processmonitors[j].running)
                    printf("Warning!  Found two copies of %s running at once!\n", processmonitors[j].exename);
                processmonitors[j].running = 1;
                processmonitors[j].processID = aProcesses[i];
            }
        }
        CloseHandle( hProcess );
    }

    CountCrashedServers();

}

static void processMonTick(HWND hDlg)
{
    int j;
    int anythingToMonitor=0;
    U32    currentTime = timerCpuSeconds();
    char status[1024];

    getText(hDlg, NULL, mapping, ARRAY_SIZE(mapping));

    for (j=0; j<ARRAY_SIZE(processmonitors); j++) {
        if (processmonitors[j].monitor) {
            anythingToMonitor=1;
            break;
        }
    }
    if (anythingToMonitor) {
        updateRunningList();
    }
    for (j=0; j<ARRAY_SIZE(processmonitors); j++) {
        int setStatus = 0;
        // Update status
        if (!processmonitors[j].monitor) {
            processmonitors[j].status = PMS_NOTMONITORED;
        } else if (!processmonitors[j].running) {
            processmonitors[j].status = PMS_NOTRUNNING;
        } else if (processmonitors[j].crashed) {
            processmonitors[j].status = PMS_CRASHED;
        } else {
            processmonitors[j].status = PMS_RUNNING;
        }

        // Update text on screen
        strcpy(status, statusToName[processmonitors[j].status]);
        if (processmonitors[j].oldstatus!=processmonitors[j].status) {
            setStatus=1;
            processmonitors[j].oldstatus = processmonitors[j].status;
        }

        // Reset counter if not crashed
        if (!processmonitors[j].monitor || !processmonitors[j].crashed)
            processmonitors[j].firstSeenCrashed = 0;

        // Do actions
        if (processmonitors[j].monitor) {
            if ((processmonitors[j].start || processmonitors[j].temp_start) && !processmonitors[j].running) {
                // Start it!
                printf("Starting %s", processmonitors[j].cmdline);
                chdir(cwd);
                executeServer(processmonitors[j].cmdline, 0);
                processmonitors[j].temp_start = 0;
                strcat(status, ", Starting...");
                setStatus = 1;
            } else if (processmonitors[j].crashed) {
                if (!processmonitors[j].firstSeenCrashed) {
                    processmonitors[j].firstSeenCrashed = currentTime;
                } else {
                    strcatf(status, " (%ds)", currentTime - processmonitors[j].firstSeenCrashed);
                    setStatus = 1;
                    if (processmonitors[j].restart) {
                        // Check timer first
                        if (currentTime - processmonitors[j].firstSeenCrashed >= processmonitors[j].seconds) {
                            // Kill and restart
                            printf("Killing crashed executable %s (PID: %d)", processmonitors[j].exename, processmonitors[j].processID);
                            kill(processmonitors[j].processID);
                            processmonitors[j].crashCount++;
                            processmonitors[j].temp_start = 1;
                            strcat(status, ", Killing...");
                            setStatus = 1;
                        } else {
                            strcatf(status, ", Killing in %ds...", processmonitors[j].seconds - (currentTime - processmonitors[j].firstSeenCrashed));
                            setStatus = 1;
                        }
                    }
                }
            }
        }
        if (setStatus && processmonitors[j].idcStatus) {
            SetDlgItemTextA(hDlg, processmonitors[j].idcStatus, status);
        }
    }
}

static void onTimer(HWND hDlg)
{
    static int timer = 0; // Shared between all instances!
    F32 elapsed;
#define maxfps 2
    static F32 spf = 1./maxfps;
    static int lock=0;
    static F32 onTimer_lasttick=0;
    if (lock!=0)
        return;
    lock++;

    if (timer==0) {
        timer = timerAlloc();
    }

    elapsed = timerElapsed(timer);
    if (elapsed > onTimer_lasttick + spf) {
        onTimer_lasttick += spf;
        if (elapsed > onTimer_lasttick + spf) {
            // We're more than a tick behind! Skip 'em!
            onTimer_lasttick = elapsed;
        }
        // do tick
        processMonTick(hDlg);
    }

    lock--;
}



LRESULT CALLBACK DlgProcessMonProc (HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    int i=0;
    RECT rect;
    //static LPPROPSHEETPAGE ps = 0;

    switch (iMsg)
    {
    case WM_INITDIALOG:
        {
            // Save the PROPSHEETPAGE information.
//            ps = (PROPSHEETPAGE *)lParam;

            getcwd(cwd, sizeof(cwd));

            loadValuesFromReg(hDlg, NULL, mapping, ARRAY_SIZE(mapping));
            // Check for entry reordering
            for (i=0; i<ARRAY_SIZE(processmonitors); i++) {
                if (!strStartsWith(processmonitors[i].cmdline, processmonitors[i].exename)) {
                    if (strStartsWith(processmonitors[i].exename, "DbServer")) {
                        strcpy(processmonitors[i].cmdline, "DbServer.exe -startall");
                    } else if (strStartsWith(processmonitors[i].exename, "Launcher")) {
                        strcpy(processmonitors[i].cmdline, "Launcher.exe -noversioncheck");
                    }
                    else
                    {
                        strcpy(processmonitors[i].cmdline, processmonitors[i].exename);
                    }
                }
            }
            saveValuesToReg(hDlg, NULL, mapping, ARRAY_SIZE(mapping));
            loadValuesFromReg(hDlg, NULL, mapping, ARRAY_SIZE(mapping)); // Updates some dialog text
            // Update dialog with text
            initText(hDlg, NULL, mapping, ARRAY_SIZE(mapping));

            SetTimer(hDlg, 0, 10, NULL);

            GetClientRect(hDlg, &rect); 
            // Initialize initial values
            doDialogOnResize(hDlg, (WORD)(rect.right - rect.left), (WORD)(rect.bottom - rect.top), IDC_ALIGNME, IDC_UPPERLEFT);
        }
        return FALSE;

    case WM_TIMER:
        onTimer(hDlg);
        break;
    case WM_COMMAND:

        switch (LOWORD(wParam))
        {
        case IDCANCEL:
            KillTimer(hDlg, 0);
            SetWindowLongPtr(hDlg, GWLP_USERDATA, (LONG_PTR) NULL);
            EndDialog(hDlg, 0);
            PostQuitMessage(0);
            return TRUE;
        }
        break;
    case WM_SIZE:
        {
            WORD w = LOWORD(lParam);
            WORD h = HIWORD(lParam);
            //doDialogOnResize(hDlg, w, h, IDC_ALIGNME, IDC_UPPERLEFT);
        }
        break;

    case WM_NOTIFY:
        {
/*            int idCtrl = (int)wParam;
            if(   listViewOnNotify(state->lvMaps, wParam, lParam, NULL)
                || listViewOnNotify(state->lvMapsStuck, wParam, lParam, updateCrashMsgText)
                || listViewOnNotify(state->lvLaunchers, wParam, lParam, NULL))
            {
                return TRUE;    // signal that we processed the message
            }*/
        }
        break;
    }
    return FALSE;
}

const char *pmsToString(ProcessMonitorStatus pms)
{
    switch (pms)
    {
    case PMS_NOTRUNNING:
        return "Not running";
    case PMS_RUNNING:
        return "Running";
    case PMS_CRASHED:
        return "CRASHED";
    case PMS_NOTMONITORED:
        return "Not Monitored";
    }
    return "INTERNAL ERROR";
}

int executeServer(char *cmd, int minimized)
{
    char *actualcmd = estrTemp();
    extern int g_are_executables_in_bin;
    const char *format = g_are_executables_in_bin ? ".\\bin\\%s" : "%s";
    int ret;
    estrConcatf(&actualcmd, format, cmd);
    ret = system_detach(actualcmd, minimized);
    estrDestroy(&actualcmd);
    return ret;
}
