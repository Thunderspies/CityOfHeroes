#include "resource.h"
#include <utilitieslib/network/sock.h>
#include <utilitieslib/utils/RegistryReader.h>
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/file.h>
#include <utilitieslib/assert/assert.h>
#include <windows.h>
#include <winsock2.h>
#include <stdio.h>

#pragma comment(lib, "ws2_32.lib")

void getNameList(void);

HINSTANCE   g_hInst=NULL;                    // instance handle
HWND        g_hWnd=NULL;                        // window handle

INT_PTR CALLBACK DlgMainProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

char *hostname=NULL;

int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{
    g_hInst = hInstance;

    if (strstr(lpCmdLine, "-host ")!=0) {
        hostname = strstr(lpCmdLine, "-host ") + strlen("-host ");
    }

    sockStart();

    getNameList();

    DialogBox (g_hInst, (LPCTSTR) (IDD_DIALOG1), NULL, DlgMainProc);

    return 0;
}

HWND g_hDlg;


RegReader rr = 0;
void initRR() {
    if (!rr) {
        rr = createRegReader();
        initRegReader(rr, "HKEY_CURRENT_USER\\SOFTWARE\\Cryptic\\RemoteDebuggerer");
    }
}
const char *regGetString(const char *key, const char *deflt) {
    static char buf[512];
    initRR();
    strcpy_s(SAFESTR(buf), deflt);
    rrReadString(rr, key, buf, 512);
    return buf;
}

void regPutString(const char *key, const char *value) {
    initRR();
    rrWriteString(rr, key, value);
}

int name_count=0;
char namelist[256][256];
void getNameList() {
    char key[256];
    char *badvalue="BadValue";
    const char *name;
    for(name_count=0; ; name_count++) {
        sprintf_s(SAFESTR(key), "NameList%d", name_count);
        name = regGetString(key, badvalue);
        if (strcmp(name, badvalue)==0)
            break;
        strcpy_s(SAFESTR(namelist[name_count]), name);
    }
}


void addNameToList(char *name) {
    int i;
    char key[256];
    for (i=0; i<name_count; i++) {
        if (stricmp(namelist[i], name)==0)
            return;
    }
    sprintf_s(SAFESTR(key), "NameList%d", name_count);
    regPutString(key, name);
    strcpy_s(SAFESTR(namelist[name_count]), name);
    name_count++;
}


SOCKET s=INVALID_SOCKET;

void sendCommand(int cmd) {
    char insturctionBuffer[2] = {0};
    if (s==INVALID_SOCKET)
        return;
    insturctionBuffer[0] = cmd;
    send(s, insturctionBuffer, 1, 0);
    /*
    case 1:
    g_listenThreadResponse = IDC_STOP;
    break;
    case 2:
    g_listenThreadResponse = IDC_DEBUG;
    break;
    case 3:
    g_listenThreadResponse = IDC_IGNORE;
    break;
    case 4:
    g_listenThreadResponse = IDC_RUNDEBUGGER;
    break;
    case 0:
    break;
    */
}

void status(char *ss) {
    SetWindowText(GetDlgItem(g_hDlg, IDC_STATUS), ss);
}

void unAttach() {
    sendCommand(0);
    closesocket(s);
    s = INVALID_SOCKET;
    //SetWindowText(GetDlgItem(g_hDlg, IDC_MAINTEXT), "Not attached");
    EnableWindow(GetDlgItem(g_hDlg, IDATTACH), TRUE);
    EnableWindow(GetDlgItem(g_hDlg, IDC_UNATTACH), FALSE);
    status("Disconnected");
}

#define CHUNK_SIZE 1400

int in_get_pdb_mode=0;

void getBytes(char *buffer, int len) {
    int pos=0;
    int i;
    int origlen = len;
    char buf[256];
    sprintf_s(SAFESTR(buf), "Downloading %d bytes...", len);
    status(buf);
    while (len) {
        i = recv(s, buffer+pos, min(CHUNK_SIZE, len), 0);
        pos += i;
        len -=i;
        if (in_get_pdb_mode) {
            static HWND hPdbButton = 0;
            if (!hPdbButton) {
                hPdbButton = GetDlgItem(g_hDlg, IDC_GETPDB);
            }
            sprintf_s(SAFESTR(buf), "%d%%", pos*100/origlen);
            SetWindowText(hPdbButton, buf);
        } else {
            sprintf_s(SAFESTR(buf), "Recv'd %d / %d bytes (%d%%)", pos, origlen, pos*100/origlen);
            status(buf);
        }
        printf("Read %d bytes\n", i);
    }
    printf("Read complete\n");
    sprintf_s(SAFESTR(buf), "Recv complete (%d bytes)", origlen);
    status(buf);
}

int getInt() {
    int i;
    getBytes((char*)&i, sizeof(i));
    return i;
}

void getPDB() {
    int len;
    char *tempbuff;
    char filename[MAX_PATH];
    char *ss;
    FILE *fout;

    sendCommand(5);
    // Get fn
    len = getInt();
    if (len==0) {
        // Error!
        len = getInt();
        getBytes(filename, len);
        status(filename);
        return;
    }
    assert(len < MAX_PATH);
    getBytes(filename, len);
    filename[len]=0;

    ss = strdup(getFileName(filename));
    sprintf_s(SAFESTR(filename), "C:/util/%s", ss);
    free(ss);

    // Get file
    len = getInt();
    tempbuff = malloc(len+1);
    EnableWindow(GetDlgItem(g_hDlg, IDC_GETPDB), FALSE);
    in_get_pdb_mode=1;
    getBytes(tempbuff, len);
    in_get_pdb_mode=0;
    EnableWindow(GetDlgItem(g_hDlg, IDC_GETPDB), TRUE);
    SetWindowText(GetDlgItem(g_hDlg, IDC_GETPDB), "Get PDB");

    // Save file
    if (0!=fileForceRemove(filename)) {
        if (0!=fileRenameToBak(filename)) {
            status("Error writing to PDB file");
            return;
        }
    }
    fout = fopen(filename, "wb");
    fwrite(tempbuff, 1, len, fout);
    fclose(fout);
    status("PDB Grabbed and saved successfully");
}

#define BASE_LISTEN_PORT 52015 // ab: 16 bit port numbers means 65k is max. 52015 is lower 16 bits of 314159 // Why?  Because I like pie.
void doAttach() {
    int port=BASE_LISTEN_PORT;
    int result;
    struct sockaddr_in addr;
    char name[MAX_PATH];
    char *buf;
    int len;

    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s==INVALID_SOCKET) {
        status("Couldn't create socket");
        return;
    }

    SendMessage(GetDlgItem(g_hDlg, IDC_NAME), WM_GETTEXT, MAX_PATH, (LPARAM)name);

    addNameToList(name);

    do {
        sockSetAddr(&addr,ipFromString(name),port);
        result = connect(s,(struct sockaddr *)&addr,sizeof(addr));
        if (result == 0)
            break;
        status("Scanning for open port...");
        port++;
        if (port > BASE_LISTEN_PORT + 8) {// If these ports are all aready taken, just give up!
            status("No open port found on host");
            return;
        }
    } while (1);
    {
        char temp[256];
        sprintf_s(SAFESTR(temp), "Connected on port %d, waiting for call stack\n", port);
        status(temp);
    }
    sockSetDelay(s, 0);

    len = getInt();
    buf = calloc(len+1, 1);
    getBytes(buf, len);
    {
        FILE *f = fopen("C:\\temp\\laststack.txt", "w");
        if (f) {
            fwrite(buf, 1, len, f);
            fclose(f);
        }
    }

    SetWindowText(GetDlgItem(g_hDlg, IDC_MAINTEXT), buf);
    status("Attached");

    EnableWindow(GetDlgItem(g_hDlg, IDATTACH), FALSE);
    EnableWindow(GetDlgItem(g_hDlg, IDC_UNATTACH), TRUE);

}

INT_PTR CALLBACK DlgMainProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    int i=0;
    //BOOL b;
    switch (iMsg) {
        case WM_INITDIALOG:
            {
                // Set initial values
                g_hDlg = hDlg;

                //SetTimer(hDlg, 0, 500, NULL);
                for (i=0; i<name_count; i++) 
                    SendMessage(GetDlgItem(hDlg, IDC_NAME), CB_ADDSTRING, 0, (LPARAM)namelist[i]);

                EnableWindow(GetDlgItem(g_hDlg, IDATTACH), TRUE);
                EnableWindow(GetDlgItem(g_hDlg, IDC_UNATTACH), FALSE);
                SetWindowText(GetDlgItem(g_hDlg, IDC_STEPS), "\
Steps:\n\
1. [Attach] here\n\
2. [Run Remote Debugger]\n\
3. Attach in VS\n\
4. [Debug] here\n\
5. Get to work!\n");
                if (hostname!=NULL) {
                    SendMessage(GetDlgItem(hDlg, IDC_NAME), CB_SETCURSEL, 0, 1);
                    SendMessage(GetDlgItem(hDlg, IDC_NAME), WM_SETTEXT, 0, (LPARAM)hostname);
                    SetTimer(hDlg, 1, 100, NULL);
                }
            }
            return 0;
            break;

        case WM_TIMER:
            KillTimer(hDlg, 1);
            SendMessage(hDlg, WM_COMMAND, IDATTACH, 0);
            break;

        case WM_COMMAND:

            switch (LOWORD (wParam))
            {
            case IDC_STOP:
                sendCommand(1);
                unAttach();
                return TRUE;
            case IDC_DEBUG:
                sendCommand(2);
                unAttach();
                return TRUE;
            case IDC_IGNORE:
                sendCommand(3);
                unAttach();
                return TRUE;
            case IDC_RUNDEBUGGER:
                sendCommand(4);
                return TRUE;
            case IDC_UNATTACH:
                unAttach();
                return TRUE;
            case IDATTACH:
                doAttach();
                return TRUE;
            case IDC_GETPDB:
                getPDB();
                return TRUE;
            case IDCANCEL:
                unAttach();
                EndDialog(hDlg, LOWORD (wParam));
                return TRUE;
            }
            break;
        case WM_CLOSE:
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_SIZE:
            return FALSE;
    }
    //return DefWindowProc(hDlg, iMsg, wParam, lParam);
    return FALSE;
}
