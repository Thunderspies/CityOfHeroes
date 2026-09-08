#include <utilitieslib/utils/SuperAssert.h>
#include <utilitieslib/network/netio.h>
#include <utilitieslib/network/net_packet.h>
#include <utilitieslib/network/net_masterlist.h>
#include <utilitieslib/network/sock.h>
#include <utilitieslib/utils/memcheck.h>
#include <utilitieslib/utils/error.h>
#include <conio.h>
#include <process.h>
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/network/netio_stats.h>
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/network/net_version.h>
#include "io.h"
#include "fcntl.h"
#include "resource.h"
#include "commctrl.h"
#include "tchar.h"
#include <utilitieslib/components/earray.h>
#include <utilitieslib/components/EString.h>

#define BUFFER_SIZE_DEFAULT 128*1024
#define BUFFER_SIZE_MAX 1*1024*1024
#define SENDQUEUE_SIZE 16384
#define MAX_CLIENTS 4096

// Global Variables:
//-server -logparser c:\absrc\yes\debug\yes.exe -ftp c:\absrc\yes\debug\yes.exe

NetLink *s_linkClient = NULL;

HINSTANCE hInst;                                // current instance
TCHAR szTitle[512] = TEXT("Log Parser Monitor");                    // The title bar text
TCHAR szWindowClass[] = TEXT("LogParserMon");            // the main window class name
char logparserImage[MAX_PATH] = "E:/logparser/logparser.exe";
char ftpImage[MAX_PATH] = "cmd /c e:/logparser/scripts/ftp_to_cryptic.bat";
char resultDir[MAX_PATH] = "e:/logparser/logs";



// Forward declarations of functions included in this code module:
 ATOM                MyRegisterClass(HINSTANCE hInstance);
 BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND HWND, UINT UINT, WPARAM WPARAM, LPARAM LPARAM);
// LRESULT CALLBACK    About(HWND, UINT, WPARAM, LPARAM);


typedef enum ClientMsg
{
     kClientMsg_SendQuery = 2,
    kClientMsg_SendFtp,
    kClientMsg_Count,
} ClientMsg;

typedef enum ServerMsg
{
    kServerMsg_Result = 2,
    kServerMsg_Status,
    kServerMsg_Msg,
    kServerMsg_Err,
    kServerMsg_Count, 
} ServerMsg;

typedef enum QueryStatus
{
    kQueryStatus_None,
    kQueryStatus_Started,
    kQueryStatus_Ftp,
    kQueryStatus_Count,
} QueryStatus;

char *querystatus_ToStr(QueryStatus s)
{
    char *strs[] = {
        "kQueryStatus_None",
        "kQueryStatus_Started",
        "kQueryStatus_Ftp",
        "kQueryStatus_Count",
    };
    return AINRANGE( s, strs ) ? strs[s] : "invalid enum";
}

static void AddMsg(char *type, char *msg, ...);
static void SendMsg(NetLink *link, char *type, char *msg, ...);
static void setHwndsEnabled(bool enabled);

bool querystatus_Valid( QueryStatus e )
{
    return INRANGE0(e, kQueryStatus_Count);
} 

bool servermsg_Valid( ServerMsg e )
{
    return INRANGE0(e, kServerMsg_Count);
}

bool clientcommand_Valid( ClientMsg e )
{
    return INRANGE0(e, kClientMsg_Count);
}

typedef struct ClientLnk
{
    NetLink *link;
} ClientLnk;


typedef struct Query
{
    PROCESS_INFORMATION pi;
    STARTUPINFO si;
    NetLink *link;
    char *resultsFile;
} Query;

typedef struct Ftp
{
    PROCESS_INFORMATION pi;
    STARTUPINFO si;
    NetLink *link;
    char *resultsFile;     
} Ftp;

Query **queries = NULL;
Ftp **ftps = NULL;

MP_DEFINE(Query);
static Query* query_Create(  )
{
    Query *res = NULL;

    // create the pool, arbitrary number
    MP_CREATE(Query, 64);
    res = MP_ALLOC( Query );
    if( verify( res ))
    {
        res->si.cb = sizeof(res->si);
    }
    return res;
}

static void query_Destroy( Query *hItem )
{
    if(hItem)
    {
        estrDestroy(&hItem->resultsFile);
        
        MP_FREE(Query, hItem);
    }
}


MP_DEFINE(Ftp);
static Ftp* ftp_Create()
{
    Ftp *res = NULL;

    // create the pool, arbitrary number
    MP_CREATE(Ftp, 64);
    res = MP_ALLOC( Ftp );
    if( verify( res ))
    {
        res->si.cb = sizeof(res->si);
    }
    return res;
}

static void ftp_Destroy( Ftp *hItem )
{
    if(hItem)
    {
        estrDestroy(&hItem->resultsFile);
        MP_FREE(Ftp, hItem);
    }
}


char *getLastError()
{
    LPVOID lpMsgBuf;
    FormatMessage( 
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM | 
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        GetLastError(),
        0, // Default language
        (LPTSTR) &lpMsgBuf,
        0,
        NULL 
        );
    return (char*)lpMsgBuf;
}

bool s_isServer = false;

NetLinkList *serverLinks = NULL;

int delCallback(NetLink *link)
{
    int i;
    ClientLnk    *client = link->userData;
    client->link = NULL;

    for( i = eaSize( &queries ) - 1; i >= 0; --i)
    {
        if( queries[i]->link == link )
        {
            queries[i]->link = NULL;
        }
    }
    for( i = eaSize( &ftps ) - 1; i >= 0; --i)
    {
        if( ftps[i]->link == link )
        {
            ftps[i]->link = NULL;
        }
    }


    printf("Disconnect from %s\n", makeIpStr(link->addr.sin_addr.S_un.S_addr));
    return 1;
}

static int connectCallback(NetLink *link)
{
    ClientLnk *client;
    client = link->userData;
    client->link = link;
    if (link->type == NLT_TCP) {
        netLinkSetMaxBufferSize(link, BothBuffers, BUFFER_SIZE_MAX);
        netLinkSetBufferSize(link, BothBuffers, BUFFER_SIZE_DEFAULT);
    }
    link->sendQueuePacketMax = SENDQUEUE_SIZE;
    printf("New connect from %s\n", makeIpStr(link->addr.sin_addr.S_un.S_addr));
    return 1;
}

int handleClientMsg(Packet *pak,int cmd, NetLink *link);
int handleServerMsg(Packet *pak,int cmd, NetLink *link);

static void startListen(U32 udp_port, U32 tcp_port)
{
    NetLinkList *links = calloc(sizeof(NetLinkList), 1);
    netLinkListAlloc(links,MAX_CLIENTS,sizeof(ClientLnk),connectCallback);
    if (netInit(links,udp_port,tcp_port)) {
        links->destroyCallback = delCallback;
        if (udp_port) {
            netLinkListSetMaxBufferSize(links, BothBuffers, BUFFER_SIZE_MAX);
            netLinkListSetBufferSize(links, BothBuffers, BUFFER_SIZE_DEFAULT);
        }
        NMAddLinkList(links, handleClientMsg);
        if (udp_port)
            printf("Now listening on UDP port %d\n", udp_port);
        if (tcp_port)
            printf("Now listening on TCP port %d\n", tcp_port);
        serverLinks = links;
    } else {
        printf("Failed to start listening\n");
    }
}


bool startConnect(U32 addr, U32 port, NetLinkType nlt)
{
    s_linkClient = createNetLink();
    AddMsg("Connect", "Connecting to %s:%d... ", makeIpStr(addr), port);
    if (netConnect(s_linkClient, makeIpStr(addr), port, nlt, 30.f, NULL)) {
        netLinkSetMaxBufferSize(s_linkClient, BothBuffers, BUFFER_SIZE_MAX);
        netLinkSetBufferSize(s_linkClient, BothBuffers, BUFFER_SIZE_DEFAULT);
        s_linkClient->sendQueuePacketMax = SENDQUEUE_SIZE;
        NMAddLink(s_linkClient, handleServerMsg);
        printf("done.\n");
        AddMsg("Startup", "Connected to client %s:%d", makeIpStr(addr),port);
        return true;
    } else {
        destroyNetLink(s_linkClient);
        s_linkClient = NULL;
        setHwndsEnabled(true);
        printf("failed.\n");
        AddMsg("Error", "Failed connect to client %s:%d", makeIpStr(addr),port);
        return false;
    }
}

// *********************************************************************************
//  client
// *********************************************************************************

void sendQuery(char *query, char *shard, char *startdate, char *enddate)
{
    Packet *pak = pktCreateEx(s_linkClient, kClientMsg_SendQuery);
    pktSendString(pak,query);
    pktSendString(pak, shard);
    pktSendString(pak, startdate);
    pktSendString(pak, enddate);
    pktSend(&pak,s_linkClient);
}

void sendFtp(char *name)
{
    Packet *pak = pktCreateEx(s_linkClient, kClientMsg_SendFtp);
    pktSendString(pak,name);
    pktSend(&pak,s_linkClient);
}

static char addrServer[128] = "172.31.19.5";
static U32 port = 7010;
bool connectClient()
{
    U32 iAddr = inet_addr(addrServer);
    return startConnect(iAddr, port, NLT_TCP);
}

// *********************************************************************************
//  Win32 stuff
// *********************************************************************************

bool createProcessStdIO(char *process, STARTUPINFO *si, PROCESS_INFORMATION *pi)
{
    HANDLE hOutputReadTmp,hOutputWrite;
    HANDLE hErrorWrite;
    SECURITY_ATTRIBUTES sa;
    
    
    // Set up the security attributes struct.
    sa.nLength= sizeof(SECURITY_ATTRIBUTES);
    sa.lpSecurityDescriptor = NULL;
    sa.bInheritHandle = TRUE;
    
    
    // Create the child output pipe.
    if (!CreatePipe(&hOutputReadTmp,&hOutputWrite,&sa,0))
        return false;
    
    
    // Create a duplicate of the output write handle for the std error
    // write handle. This is necessary in case the child application
    // closes one of its std output handles.
    if (!DuplicateHandle(GetCurrentProcess(),hOutputWrite,
                         GetCurrentProcess(),&hErrorWrite,0,
                         TRUE,DUPLICATE_SAME_ACCESS))
        return false;
    
    
    // Close inheritable copies of the handles you do not want to be
    // inherited.
    if (!CloseHandle(hOutputReadTmp)) return false;

    ZeroStruct( si );
    si->cb = sizeof(*si);
//     si->dwFlags = STARTF_USESTDHANDLES;
//     si->hStdOutput = hOutputWrite ;
//     si->hStdError = hErrorWrite;
    
    return CreateProcess(NULL, process, NULL, NULL, true, CREATE_NEW_CONSOLE, NULL, NULL, si, pi);
}

static bool createFtpProcess(Ftp *ftp)
{
    bool res;
    char *process = NULL;
    estrPrintf(&process, "%s %s", ftpImage, ftp->resultsFile);
    
    printf("starting ftp: %s\n", process);
    res = createProcessStdIO(process, &ftp->si, &ftp->pi );
    estrDestroy(&process );
    return res;
}

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
     // TODO: Place code here.
    MSG msg;
    bool quitting = false;
    bool console = false;

    memCheckInit();
    pktSetDebugInfo();
    bsAssertOnErrors(true);

    sockStart();
    packetStartup(0,0); // TODO: enable encryption

      MyRegisterClass(hInstance);

    hInst = hInstance;

    // *********************************************************************************
    //  command line
    // *********************************************************************************

    {
        char *sPort = strstr(lpCmdLine,"-port");
        if(sPort)
            sscanf(sPort,"-port %d",&port);
    }

    if( strstr(lpCmdLine, "-console") )
    {
        // start the console
        newConsoleWindow();
        console = true;
    }

    if( strstr( lpCmdLine, "-logparser" ) )
    {
        char *lp = strstr( lpCmdLine, "-logparser" );
        char *tmp = logparserImage;
        bool inquote = false;
        while(*lp && *lp++ != ' ');
        while(*lp && ((!inquote && *lp != ' ') || (inquote && *lp == '"')))
        {
            if(*lp == '"')
            {
                inquote = !inquote;
                lp++;
            }
            else
            {
                *tmp++ = *lp++;
            }
        }
        *tmp = 0;
    }

    if( strstr( lpCmdLine, "-ftp" ) )
    {
        char *lp = strstr( lpCmdLine, "-ftp" );
        char *tmp = ftpImage;
        bool inquote = false;
        while(*lp && *lp++ != ' ');
        while(*lp && ((!inquote && *lp != ' ') || (inquote && *lp == '"')))
        {
            if(*lp == '"')
            {
                inquote = !inquote;
                lp++;
            }
            else
            {
                *tmp++ = *lp++;
            }
        }
        *tmp = 0;
    }

    if( strstr( lpCmdLine, "-outdir" ) )
    {
        char *lp = strstr( lpCmdLine, "-outdir" );
        char *tmp = resultDir;
        bool inquote = false;
        while(*lp && *lp++ != ' ');
        while(*lp && ((!inquote && *lp != ' ') || (inquote && *lp == '"')))
        {
            if(*lp == '"')
            {
                inquote = !inquote;
                lp++;
            }
            else
            {
                *tmp++ = *lp++;
            }
        }
        *tmp = 0;
    }

    if( strstr(lpCmdLine, "-server") )
    {
        // start the console
        if(!console)
            newConsoleWindow();
        console = true;

        startListen(0,port);
        s_isServer = true;
    }    
    else
    {
        char *sAddr = strstr(lpCmdLine,"-addr");
        if( sAddr )
        {
            char *tmp = addrServer;
            while(*sAddr && *sAddr++ != ' ');
            while(*sAddr && *sAddr != ' ')
            {
                *tmp++ = *sAddr++;
            }
            *tmp = 0;
        }
    }
    

    if(strstr(lpCmdLine,"-?") || strstr(lpCmdLine,"-help") || strstr(lpCmdLine,"--help"))
    {
        if(!console)
            newConsoleWindow();
        printf("-port N: port\n-console : console\n-parserimage <path to image>\n-server : server mode\n-addr <ip address> : server ip to connect to\nPress any key.");
        while(!getch()) Sleep(1);
        exit(0);
    }

    // *********************************************************************************
    //  main msg loop
    // *********************************************************************************

     if (!s_isServer)
    {
        if(!InitInstance (hInstance, nCmdShow)) 
        {
            return FALSE;
        }

        // client loop
        while(!quitting)
        {
            if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
            {
                //             if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
                {
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                }

                quitting = (msg.message == WM_QUIT);
            }

            if(s_linkClient )
            {
                static bool conn = false;
                if(s_linkClient->connected)
                    conn = true;
                if(s_linkClient->disconnected && conn)
                {
                    AddMsg("Done", "Disconnected from server");
                    setHwndsEnabled(true);
                    conn = false;
                }
            }
            NMMonitor(1);
            Sleep(1);
        }
        return (int) msg.wParam;
    }
    else
    {
        int i;
        for(;;)
        {
            // could use io completion for this
            for( i = eaSize(&queries) - 1; i >= 0; --i)
            {
                Query *q = queries[i];
                if( q )
                {
                    CHAR lpBuffer[1024];
                    DWORD nBytesRead;
                    
                    // ----------------------------------------
                    // read any process output

                    if (q->link && ReadFile(q->si.hStdOutput,lpBuffer,sizeof(lpBuffer),
                                  &nBytesRead,NULL) && nBytesRead)
                    {
                        SendMsg(q->link,"STDOUT",lpBuffer);
                    }

                    if (q->link && ReadFile(q->si.hStdError,lpBuffer,sizeof(lpBuffer),
                                  &nBytesRead,NULL) && nBytesRead)
                    {
                        SendMsg(q->link,"STDERR",lpBuffer);
                    }
                    
                    // ----------------------------------------
                    // if the process is done, next

                    if( WAIT_OBJECT_0 == WaitForSingleObject(q->pi.hProcess,0) )
                    {
                        Ftp *ftp = ftp_Create();

                        ftp->resultsFile = q->resultsFile;
                        q->resultsFile = NULL;

                        if( createFtpProcess(ftp) ) //createProcessStdIO(process, &ftp->si, &ftp->pi ) )
                        {
                            if(q->link)
                            {
                                Packet *pak = pktCreateEx(q->link, kServerMsg_Status);
                                pktSendBitsAuto( pak, kQueryStatus_Ftp);
                                pktSendString(pak, ftp->resultsFile);
                                pktSend(&pak,q->link);
                            }
                            
                            ftp->link = q->link;
                            eaPush(&ftps, ftp);
                        }
                        else if(q->link)
                        {
                            char *str = getLastError();
                            Packet *pak = pktCreateEx(q->link, kServerMsg_Result);
                            pktSendBitsAuto( pak, 0);
                            pktSendString( pak, str );
                            pktSend(&pak, q->link);

                            netSendDisconnect(q->link,0);
                        }

                        // cleanup
                        query_Destroy(q);
                        eaRemove(&queries, i);        
                    }
                }
            }
            for( i = eaSize(&ftps) - 1; i >= 0; --i)
            {
                Ftp *f = ftps[i];
                NetLink *link = f->link;

                if( f )
                {
                    if( WAIT_OBJECT_0 == WaitForSingleObject(f->pi.hProcess,0) )
                    {
                        if(f->link)
                        {
                            char *tmp = NULL;
                            Packet *pak;
                            estrPrintf(&tmp, "Transferred file %s",f->resultsFile);
                            pak = pktCreateEx(link, kServerMsg_Result);
                            pktSendBitsAuto( pak, 1);
                            pktSendString( pak, tmp );
                            pktSend(&pak, link);                        
                            estrDestroy(&tmp);
                            netSendDisconnect(link,0);
                        }
                        
                        ftp_Destroy(f);
                        eaRemove(&ftps,i);
                    }
                }
            }

            // input
            if (_kbhit()) {
                int c = _getch();
                switch (tolower(c)) {
                case 's':
                    printf("queries: %5i\tftps: %5i\n", eaSize(&queries), eaSize(&ftps));
                    break;
                default:
                    break;
                }
            }
            
            NMMonitor(1);
            Sleep(1);
        }
    }

    return 0;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX); 

    wcex.style            = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra        = 0;
    wcex.cbWndExtra        = 0;
    wcex.hInstance        = hInstance;
    wcex.hIcon            = LoadIcon(hInstance, (LPCTSTR)IDI_ICON1);
    wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground    = CreateSolidBrush(RGB(128,128,128));//(HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName    = NULL;//(LPCTSTR)IDC_LOGPARSERMON;
    wcex.lpszClassName    = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, (LPCTSTR)IDI_ICON1);

    return RegisterClassEx(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, CW_USEDEFAULT, 500, 400, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}


static HWND s_hwnd = 0;
static HWND s_hwndSearchBtn = 0;
static HWND s_hwndMsgs = 0;
static HWND *s_hwnds = NULL;
static HWND *s_hwndsEdit = NULL;

static void setHwndsEnabled(bool enabled)
{
    int i;
    for( i = eaSize(&s_hwnds) - 1; i >= 0; --i)
    {
        EnableWindow(s_hwnds[i],enabled);
    }
}

static WNDPROC s_origEditWndProc = 0; 
LRESULT CALLBACK wndproc_Edit(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if(message == WM_KEYUP && wParam == VK_TAB)
    {
        HWND foc = GetFocus();
        int i;
        int n = eaSize(&s_hwndsEdit);
        for( i = n - 1; i >= 0; --i)
        {
            if( s_hwndsEdit[i] == foc )
            {
                SetFocus(s_hwndsEdit[(i+1)%n]);
                break;
            }
        }
    }
    else
        return CallWindowProc(s_origEditWndProc, hwnd, message, wParam, lParam);
    return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HWND hwndQueryEdit = 0;
    static HWND hwndShardEdit = 0;
    static HWND hwndStartdateEdit = 0;
    static HWND hwndEnddateEdit = 0;
    int wmId, wmEvent;
    PAINTSTRUCT ps;
    HDC hdc;

    enum CtrlIds
        {
            idList = 2,
            idText,
            idEaterieEdit,
            idAddBtn,
            idSearchBtn,
            idSearchEdit,
        };

    switch (message) 
    {
    case WM_CREATE:
    {
        int cxChar = LOWORD(GetDialogBaseUnits());
        int cyChar = HIWORD(GetDialogBaseUnits());
        int i;
        
        
        // search
        
//         TCHAR compname[512] = {0};
//         DWORD len = ARRAY_SIZE( compname );
//         GetComputerName(compname, &len);
        
        s_hwndSearchBtn = CreateWindowEx( 0L, WC_BUTTON, _T("Start LogParser"), 
                                         WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|WS_BORDER, 
                                         0, 0, CW_USEDEFAULT, CW_USEDEFAULT,
                                         hwnd, (HMENU)idSearchBtn, hInst, NULL );
        eaPush(&s_hwnds,s_hwndSearchBtn);
        
        hwndQueryEdit = CreateWindowEx( 0L, WC_EDIT, _T("Query"), WS_CHILD|WS_VISIBLE|WS_BORDER|ES_LEFT,
                                        0, 0, 0, 0,
                                        hwnd, (HMENU)idSearchEdit, hInst, NULL );        
        eaPush(&s_hwnds, hwndQueryEdit);
        eaPush(&s_hwndsEdit, hwndQueryEdit);
        SetFocus(hwndQueryEdit);

        hwndShardEdit = CreateWindowEx( 0L, WC_EDIT, _T("Shard (USCOV18)"), WS_CHILD|WS_VISIBLE|WS_BORDER|ES_LEFT,
                                        0, 0, 0, 0,
                                        hwnd, (HMENU)idSearchEdit, hInst, NULL );        
        eaPush(&s_hwnds, hwndShardEdit);
        eaPush(&s_hwndsEdit, hwndShardEdit);

        
        hwndStartdateEdit = CreateWindowEx( 0L, WC_EDIT, _T("060201 12:00:00") , WS_CHILD|WS_VISIBLE|WS_BORDER|ES_LEFT,
                                        0, 0, 0, 0,
                                        hwnd, (HMENU)idSearchEdit, hInst, NULL );        
        eaPush(&s_hwnds, hwndStartdateEdit);
        eaPush(&s_hwndsEdit, hwndStartdateEdit);
        
        hwndEnddateEdit = CreateWindowEx( 0L, WC_EDIT, _T("070201 12:00:00") , WS_CHILD|WS_VISIBLE|WS_BORDER|ES_LEFT,
                                        0, 0, 0, 0,
                                        hwnd, (HMENU)idSearchEdit, hInst, NULL );
        eaPush(&s_hwnds, hwndEnddateEdit);
        eaPush(&s_hwndsEdit, hwndEnddateEdit);
        
        s_hwndMsgs = CreateWindowEx( 0, WC_LISTVIEW, _T("Ready") , 
                                     WS_CHILD|WS_VISIBLE| WS_BORDER | LVS_REPORT,
                                     0, 0, 0, 0,
                                     hwnd, (HMENU)idSearchEdit, hInst, NULL );

        for( i = eaSize(&s_hwndsEdit) - 1; i >= 0; --i)
        {
            s_origEditWndProc = (WNDPROC)SetWindowLongPtr(s_hwndsEdit[i], GWLP_WNDPROC, (LONG_PTR)wndproc_Edit);
//            SetVisible(s_hwndsEdit[i],true);
        }


        {
            // add a column
            LV_COLUMN lvC = {0};
            lvC.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
            lvC.fmt = LVCFMT_LEFT;
            
            {
                TCHAR *title = _T("Type");
                lvC.cx = cxChar*20; //(lstrlen(title)+1);
                lvC.iSubItem = 0;
                lvC.pszText = title;
                assert(-1 != ListView_InsertColumn( s_hwndMsgs, 0, &lvC ));
            }

            lvC.cx =cxChar * 100;
            lvC.iSubItem = 1;
            lvC.pszText = _T("Info");
            assert(-1 != ListView_InsertColumn( s_hwndMsgs, 1, &lvC ));
        }        
    }
        break;
    case WM_SIZE:
    {
        int cxClient = LOWORD(lParam);
        int cyClient = HIWORD(lParam);
        int cxChar = LOWORD(GetDialogBaseUnits());
        int cyChar = HIWORD(GetDialogBaseUnits());
        
        int xSearch = cxChar;
        int ySearch = cyChar/2;
        int cxSearch = 20*cxChar;
        int cySearch = cyChar+4;
        int xSearchBtn = xSearch + cxSearch + cxChar;
        int ySearchBtn = ySearch;
        int cxSearchBtn = 15*cxChar;
        int cySearchBtn = cySearch;
        int cyPad = cyChar/2;

        verify(MoveWindow( hwndQueryEdit, xSearch, ySearch, cxSearch, cySearch, TRUE ));
        ySearch+=cySearch+cyPad;
        verify(MoveWindow( hwndShardEdit, xSearch, ySearch, cxSearch, cySearch, TRUE ));
        ySearch+=cySearch+cyPad;
        verify(MoveWindow( hwndStartdateEdit, xSearch, ySearch, cxSearch, cySearch, TRUE ));
        ySearch+=cySearch+cyPad;
        verify(MoveWindow( hwndEnddateEdit, xSearch, ySearch, cxSearch, cySearch, TRUE ));
        ySearch+=cySearch+cyPad;
        verify(MoveWindow( s_hwndSearchBtn, xSearch, ySearch, cxSearchBtn, cySearchBtn, TRUE));
        // last one, gets remaining real estate
        ySearch+=cySearchBtn+cyPad;
        verify(MoveWindow( s_hwndMsgs, xSearch, ySearch, cxClient - xSearch - cxChar*3, cyClient - ySearch - cyPad, TRUE));
    }
    break;
    case WM_COMMAND:
        {
            HWND hwndCmd = (HWND)lParam;
            wmId    = LOWORD(wParam); 
            wmEvent = HIWORD(wParam);

            if( s_hwndSearchBtn == hwndCmd && connectClient() )
            {
                char query[1024];
                char shard[1024];
                char startdate[1024];
                char enddate[1024];

                SendMessage(hwndQueryEdit, WM_GETTEXT, ARRAY_SIZE( query ), (LPARAM)query);
                SendMessage(hwndShardEdit, WM_GETTEXT, ARRAY_SIZE( shard ), (LPARAM)shard);
                SendMessage(hwndStartdateEdit, WM_GETTEXT, ARRAY_SIZE( startdate ), (LPARAM)startdate);
                SendMessage(hwndEnddateEdit, WM_GETTEXT, ARRAY_SIZE( enddate ), (LPARAM)enddate);
                setHwndsEnabled(false);                
                
                if(GetAsyncKeyState(VK_SHIFT) & 0x8000000)
                {
                    sendFtp(query);
                }
                else
                {
                    sendQuery(query,shard,startdate,enddate);
                }
            }
        }
        break;
    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);
        // TODO: Add any drawing code here...
        EndPaint(hwnd, &ps);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, message, wParam, lParam);
    }
    return 0;
}


static void stripInvalidChars(char *fn)
{
    for(;;)
    {
        size_t i = strcspn(fn, " |'\"/\\$:");
        fn+=i;
        if(*fn)
            *fn = '_';
        else
            break;
    }
}

static void s_makeResultsFilename(char **res, char *resultDir, char *query)
{
    char *filename;    
    strdup_alloca(filename,query);
    stripInvalidChars(filename);
    estrPrintf(res,"\"%s/qs_%s.txt\"", resultDir, filename );
}


static void handleQuery(NetLink *link, Packet *pak)
{
    char *query; 
    char *shard; 
    char *startdate;
    char *enddate;
    Query *q = query_Create();    
    char *process = NULL;

    strdup_alloca( query, pktGetString(pak) );
    strdup_alloca( shard, pktGetString(pak) );
    strdup_alloca( startdate, pktGetString(pak) );
    strdup_alloca( enddate, pktGetString(pak) );    
    
    s_makeResultsFilename(&q->resultsFile, resultDir, query);

    estrCreate(&process);
    // C:\logparser\logparser.exe -log "C:\logparser\logs\USCOH09_ImpsGoneWild\*.log.gz" -quickscan "SuperGroup:Rank" ".\QS_impsgonewild.txt" "060201 12:00:00" "061230 00:00:00"
    estrPrintf(&process, "%s", logparserImage);
    estrConcatf(&process," -log \"Z:/%s/City Of Heroes/logs/logserver/db_*.log.gz\"", shard );
    estrConcatf(&process, " -iquickscan \"%s\"", query);
    estrConcatf(&process, " %s", q->resultsFile);
    estrConcatf(&process, " \"%s\" \"%s\"", startdate, enddate );

    printf("creating process: %s\n", process);
    if( createProcessStdIO(process, &q->si, &q->pi) )
    {
        Packet *pak = pktCreateEx(link, kServerMsg_Status);
        pktSendBitsAuto( pak, kQueryStatus_Started );
        pktSendString(pak, q->resultsFile);
        pktSend(&pak,link);
        
        q->link = link;
        eaPush(&queries, q);
    }
    else
    {
        char *tmp = NULL;
        char *str = getLastError();
        Packet *pak = pktCreateEx(link, kServerMsg_Result);
        pktSendBitsAuto( pak, 0);

        estrPrintf(&tmp,"couldn't start process '%s' error: '%s'", process, str);
        pktSendString( pak, tmp );
        pktSend(&pak, link);
        
        netSendDisconnect(link,0);
    }

    
    estrDestroy(&process);
}

static void handleFtp(NetLink *link, Packet *pak)
{
    char *query;
    Ftp *f = ftp_Create();
    int i;

    strdup_alloca( query ,pktGetString(pak));
    s_makeResultsFilename(&f->resultsFile, resultDir, query ); 

    for( i = eaSize( &ftps ) - 1; i >= 0; --i)
    {
        Ftp const *ftp = ftps[i];

        if(ftp && 0 == stricmp(f->resultsFile, ftp->resultsFile))
        {
            ftp_Destroy(f);
            f = NULL;
            break;
        }
    }

    if( f )
    {
        if( createFtpProcess( f ) )
        {
            Packet *pak = pktCreateEx(link, kServerMsg_Status);
            pktSendBitsAuto( pak, kQueryStatus_Ftp);
            pktSendString(pak, f->resultsFile);
            pktSend(&pak,link);
            
            f->link = link;
            
            eaPush(&ftps, f );
        }
    }
    
}

static void AddMsg(char *type, char *format, ...)
{
    LV_ITEM lvI = {0}; 
    int i;
    char *msg = NULL;
    VA_START(args, format);
    estrConcatfv(&msg, format, args);
    VA_END();

    printf("%s: %s\n", type, msg);

    i = ListView_GetItemCount(s_hwndMsgs);
    
    lvI.iItem = i;
    lvI.lParam = 0;
    lvI.cchTextMax = 10;
    lvI.mask = LVIF_TEXT | LVIF_STATE | LVIF_PARAM;
    lvI.iSubItem = 0; // column 0, must be used with insertitem
    lvI.pszText = type;
    
    verify(-1 != ListView_InsertItem(s_hwndMsgs, &lvI));
    
    lvI.mask = LVIF_TEXT | LVIF_STATE;
    lvI.iSubItem = 1; // column 1
    lvI.pszText = msg;
    verify(-1 != ListView_SetItem(s_hwndMsgs, &lvI));    

}

static void SendMsg(NetLink *link, char *type, char *format, ...)
{
    char *msg = NULL;
    Packet *pak;
    VA_START(args, format);
    estrConcatfv(&msg, format, args);
    VA_END();

    pak = pktCreateEx(link, kServerMsg_Msg);
    pktSendString( pak, type );
    pktSendString( pak, msg );
    pktSend(&pak,link);
}


static void svrHandleResult(Packet *pak,NetLink *link)
{
    bool res = pktGetBitsAuto(pak);
    char *msg;
    strdup_alloca(msg,pktGetString(pak));
    AddMsg(res?"Success":"Failure", msg);
    setHwndsEnabled(true);
}

static void svrHandleStatus(Packet *pak,NetLink *link)
{
    QueryStatus qs = pktGetBitsAuto(pak);
    char *msg;
    strdup_alloca(msg,pktGetString(pak));
    AddMsg(querystatus_ToStr(qs), msg);
}

void handleMsg(NetLink *link, Packet *pak, bool err)
{
    char *type;
    char *msg;
    strdup_alloca(type,pktGetString(pak));
    strdup_alloca(msg,pktGetString(pak));
    AddMsg(type,msg);
}


int handleServerMsg(Packet *pak,int cmd, NetLink *link)
{
    bool err = false;
    switch ( cmd )
    {
    case kServerMsg_Result:
        svrHandleResult(pak,link);
        break;
    case kServerMsg_Status:
        svrHandleStatus(pak,link);
        break;
    case kServerMsg_Err:
        err = true; 
        // fall thru
    case kServerMsg_Msg:
        handleMsg(link,pak,err);
    break;
    default:
        AddMsg("Server Error", "invalid switch value %i\n", cmd);
    };
    return 1;
}


int handleClientMsg(Packet *pak,int cmd, NetLink *link)
{
    void *client = link->userData;
    bool err = false;
    
    if (!client)
        return 0;

    switch ( cmd )
    {
    case kClientMsg_SendQuery:
    {
        handleQuery(link, pak);
    }
    break;
    case kClientMsg_SendFtp:
    {
        handleFtp(link, pak);
    }
    break;
    default:
        printf("invalid switch value %i\n", cmd);
    };
    return 1;
}

//                     int len = 0;
//                     char *file = fileAlloc_dbg(q->resultsFile, &len);
                    
//                     if( file )
//                     {
//                         Packet *pak = pktCreateEx(link, kServerMsg_Status);
//                         U32 zipsize = 0;
//                         void *zip = zipData(file, len, &zipsize);
//                         pktSendString( pak, "beginning send, %i bytes",zipsize);
//                         pktSend(&pak, link);

//                         pak = pktCreateEx(link, kServerMsg_Results
//                     }
//                     else
//                     {
//                         char *tmp = NULL;
//                         estrPrintf(&tmp, "couldn't alloc file %s, last error:%s", q->resultsFile, getLastError());
//                         Packet *pak = pktCreateEx(link, kServerMsg_Result);
//                         pktSendBitsAuto( pak, 0);
//                         pktSendString( pak, tmp );
//                         pktSend(&pak, link); 
                        
//                         netSendDisconnect(link,0);
//                         estrDestroy(&tmp);
//                     }
