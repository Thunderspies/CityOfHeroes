#include "Config.h"
#include "Thread.h"
#include "GlobalAuth.h"
#include "Job.h"
#include "ServerList.h"
#include "AccountDB.h"
#include "util.h"
#include "IOServer.h"
#include "IPSessionDB.h"
#include "buildn.h"
#include "DBConn.h"
#include "LogSocket.h"
#include "Blowfish.h"
#include "WantedSocket.h"
#include "IPList.h"

#include <tchar.h>

#define BUTTON_WIDTH    160

#define RELOAD_BUTTON_ID    1
#define    LOGLEVEL_BUTTON_ID    2

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(_arr_)    (sizeof(_arr_)/sizeof(_arr_[0]))
#endif

HWND mainWnd;
HWND logWnd;
HWND reporterWnd;
HWND reloadServerButtonWnd, 
     verboseLoggingButtonWnd;
HINSTANCE g_instance;
bool globalTeminateEvent=false;

EncPwdType EncPwd;

static void ShowLoggingLevel( void )
{
    bool bVerboseON    = logger.GetMsgAllowed( LOG_VERBOSE );
    bool bDebugON    = logger.GetMsgAllowed( LOG_DEBUG );
    
    // Show the message in the text color of the related message type.
    // Helps the user know what the colors represent.
    logger.SetMsgAllowed(LOG_VERBOSE, true);
    logger.SetMsgAllowed(LOG_DEBUG, true);
    
    logger.AddLog(LOG_NORMAL,  "----------------------------------------" );
    logger.AddLog(LOG_VERBOSE, "Verbose logging... %s", ( bVerboseON ) ? "ON" : "OFF" );
    logger.AddLog(LOG_DEBUG,   "Debug logging..... %s", ( bDebugON )   ? "ON" : "OFF" );
    logger.AddLog(LOG_NORMAL,  "----------------------------------------" );

    logger.SetMsgAllowed(LOG_VERBOSE, bVerboseON);
    logger.SetMsgAllowed(LOG_DEBUG, bDebugON);
}

static void OnChangeLoggingLevel( void )
{
    static struct    {
        bool bVerboseEnabled;
        bool bDebugEnabled;
    } stateList[] = {
        { false,    false },
        { true,        false },
        { true,        true  }
    };
    static int sCurrState = 0;
    static int sIncr = 1;
    
    sCurrState += sIncr;
    if ( sCurrState == ARRAY_SIZE(stateList) )
    {
        sCurrState = ( ARRAY_SIZE(stateList) - 2 );
        sIncr = -1;
    }
    else if ( sCurrState < 0 )
    {
        sCurrState = 1;
        sIncr = 1;
    }
    logger.SetMsgAllowed(LOG_VERBOSE, stateList[sCurrState].bVerboseEnabled);
    logger.SetMsgAllowed(LOG_DEBUG, stateList[sCurrState].bDebugEnabled);
    ShowLoggingLevel();
}

static void ShowDBInitError(void)
{
    MessageBoxA(mainWnd, "An error occurred connecting to the Database.\nSee log window and log file for details.",
        "Fatal Error", MB_ICONERROR | MB_OK);
}

static void ShowConfigFileLoadError( void )
{
    char cwd[_MAX_PATH];
    char msg[_MAX_PATH + 256];
    _getcwd(cwd, _MAX_PATH);
    sprintf_s( msg, ARRAY_SIZE(msg),
        "Could not open config file: \n"
        "    %s\\%s", 
            cwd, 
            CONFIG_FILENAME );
    MessageBoxA( mainWnd, msg, "Fatal Error", MB_ICONERROR | MB_OK );
}

static void ShowLogDirectoryError( void )
{
    char cwd[_MAX_PATH];
    char msg[_MAX_PATH + 256];
    _getcwd(cwd, _MAX_PATH);
    sprintf_s( msg, ARRAY_SIZE(msg),
        "Can't create log file in folder '%s'.\n"
        "Current directory: '%s'\n\n"
        "Please make sure a '%s' folder exists in that location.\n",
            config.logDirectory,
            cwd,
            config.logDirectory );
    MessageBoxA( mainWnd, msg, "Fatal Error", MB_ICONERROR | MB_OK );
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
_BEFORE
    int mainWidth;
    int mainHeight;
    
    switch (uMsg) {

    case WM_SIZE:
        if (hwnd == mainWnd) {
            mainWidth = LOWORD(lParam);
            mainHeight = HIWORD(lParam);
            MoveWindow(reporterWnd, BUTTON_WIDTH * 2, 0, mainWidth - BUTTON_WIDTH * 2, 20, TRUE);
            MoveWindow(logWnd, 0, 20, mainWidth, mainHeight - 20, TRUE);
            MoveWindow(reloadServerButtonWnd, 0, 0, BUTTON_WIDTH, 20, TRUE);
            MoveWindow(verboseLoggingButtonWnd, BUTTON_WIDTH, 0, BUTTON_WIDTH, 20, TRUE);
        }
        else if (hwnd == logWnd) {
            logger.Resize(LOWORD(lParam), HIWORD(lParam));
        }
        else if (hwnd = reporterWnd) {
            reporter.Resize(LOWORD(lParam), HIWORD(lParam));
        }
        break;
    case WM_PAINT:
        if (hwnd == logWnd) {
            logger.Redraw();
        } else if ( hwnd == reporterWnd ) {
            reporter.Redraw();
        }
        break;
    case WM_CLOSE:
        if (hwnd == mainWnd) {
            DestroyWindow(hwnd);
        }
        break;
    case WM_DESTROY:
        if (hwnd == mainWnd) {
            g_bTerminating = true;
            logger.Enable( false );
            job.SetTerminate();
            Sleep(2000);
            PostQuitMessage(0);
        }
        break;
    case WM_TIMER:
        if (wParam == 102)
        {
            reporter.m_UserCount = accountdb.GetUserNum();
            InvalidateRect(reporterWnd, NULL, FALSE);

        }
        else if ( wParam == 103 )
        {
            g_ServerList.RequestUserCounts();
        }
        break;

    case WM_COMMAND:
        {
            int notification = HIWORD(wParam);
            int buttonId = LOWORD(wParam);
            switch (notification) {
            case BN_CLICKED:
                if (buttonId == RELOAD_BUTTON_ID) {
                    g_ServerList.Load();
                }
                else if (buttonId == LOGLEVEL_BUTTON_ID) {
                    OnChangeLoggingLevel();
                }
                break;
            }
        }
        break;
    case WM_KEYDOWN:
        break;
    }
_AFTER_FIN
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
};

unsigned char blowFishKey[] = {
    0xa4, 0xde, 0x6b, 0x64, 0xff, 0x24, 0xad, 0x74, 0x52, 0xa0,
    0x6a, 0x35, 0xaf, 0xf5, 0x37, 0x11, 0xd3, 0x5a, 0xc8, 0x42
};

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{


#ifdef _DEBUG
    int tmpFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
    tmpFlag |= _CRTDBG_LEAK_CHECK_DF;
    _CrtSetDbgFlag(tmpFlag);
#endif
    HWND prevHwnd = FindWindow(NULL, _T("AuthServer"));
    if ( prevHwnd != NULL ){
        MessageBox(NULL, _T("An instance of Authserver is already running."), _T("Error"), MB_ICONERROR | MB_OK );
        exit(0);
    }
    
    InitializeBlowfish(blowFishKey, sizeof(blowFishKey));

    extern void InitRSAParams();
    InitRSAParams();

    g_linDB = new DBEnv;
    server = new CServer;
    serverEx = new CIOServerEx;
    serverInt = new CServerInt;

    WNDCLASSEX wcx;
    wcx.cbSize = sizeof(WNDCLASSEX);
    wcx.style = CS_CLASSDC;
    wcx.lpfnWndProc = WindowProc;
    wcx.cbClsExtra = 0;
    wcx.cbWndExtra = 0;
    wcx.hInstance = hInstance;
    wcx.hIcon = 0;
    wcx.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcx.hbrBackground = (HBRUSH) NULL;
    wcx.lpszMenuName = NULL;
    wcx.lpszClassName = _T("AuthServer");
    wcx.hIconSm = NULL;
    ATOM windowClass = RegisterClassEx(&wcx);
    g_instance = hInstance;

exception_init();

    WSADATA wsaData;
    int err = WSAStartup(0x0202, &wsaData);

    if (err) {
        logger.AddLog(LOG_ERROR, "WSAStartup error 0x%x", err);
        return 0;
    }

    mainWnd = CreateWindowEx(0, (LPCTSTR)windowClass, _T("AuthServer"), WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, 860, 440, NULL, NULL, hInstance, NULL);

    logWnd = CreateWindowEx(WS_EX_CLIENTEDGE, (LPCTSTR)windowClass, _T(""), WS_CHILD, 0, 30, 640, 720,
        mainWnd, NULL, hInstance, NULL);

    reporterWnd = CreateWindowEx(WS_EX_CLIENTEDGE, (LPCTSTR)windowClass, _T(""), WS_CHILD, 0, 0, 640, 30,
        mainWnd, NULL, hInstance, NULL);

    reloadServerButtonWnd = CreateWindowEx(0, _T("BUTTON"), _T("Reload Server List"), WS_CHILD|BS_PUSHBUTTON, 600, 0, 40, 30, mainWnd,
        (HMENU)RELOAD_BUTTON_ID, hInstance, NULL);

    verboseLoggingButtonWnd = CreateWindowEx(0, _T("BUTTON"), _T("Logging Level"), WS_CHILD|BS_PUSHBUTTON, 600, 0, 40, 30, mainWnd,
        (HMENU)LOGLEVEL_BUTTON_ID, hInstance, NULL);
        
    logger.SetWnd( logWnd );
    reporter.SetWnd( reporterWnd );
    SetProcessPriorityBoost(GetCurrentProcess(), TRUE);

    ShowWindow(mainWnd, nCmdShow);
    UpdateWindow(mainWnd);

    ShowWindow(logWnd, SW_SHOW);
    UpdateWindow(logWnd);

    ShowWindow(reporterWnd, SW_SHOW);
    UpdateWindow(reporterWnd);

    ShowWindow(reloadServerButtonWnd, SW_SHOW);
    UpdateWindow(reloadServerButtonWnd);

    ShowWindow(verboseLoggingButtonWnd, SW_SHOW);
    UpdateWindow(verboseLoggingButtonWnd);

// Start Init
    DesKeyInit("TEST");
    if ( ! config.Load( CONFIG_FILENAME ) )
    {
        ShowConfigFileLoadError();
        exit(0);
    }

    unsigned listenThreadId;
    HANDLE listenThread=NULL;

    logger.SetMsgAllowed(LOG_VERBOSE, config.enableVerboseLogging );
    logger.SetMsgAllowed(LOG_DEBUG, config.enableDebugLogging );
    
    if (strlen(config.logDirectory) <= 0 )
    {
        logger.SetDirectory("log");
        logger.Enable(true );
        filelog.SetDirectory( config.logDirectory);
        actionlog.SetDirectory( config.logDirectory );
        logdfilelog.SetDirectory( config.logDirectory );
        logger.AddLog(LOG_ERROR, "Error load config.txt" );
        logdfilelog.SetDirectory( config.logDirectory );
    } else {
        if ( ! logger.SetDirectory( config.logDirectory ) )
        {
            ShowLogDirectoryError();
            exit(0);        
        }
        logger.Enable( true );    
        filelog.SetDirectory( config.logDirectory);
        actionlog.SetDirectory( config.logDirectory );
        errlog.SetDirectory( config.logDirectory );
        logdfilelog.SetDirectory( config.logDirectory );
        //Every 2 seconds, update our UI's list of players
        SetTimer(mainWnd, 102, 2000, NULL);
        //TBROWN - explanation - every minute, ping all of the connected servers and ask how many users are logged on 
        SetTimer(mainWnd, 103, 60000, NULL );
        switch ( config.gameId ) 
        {
        case 4:
            EncPwd = EncPwdShalo;
            break;
        case 8:
        case 16:
        case 32:
            EncPwd = EncPwdL2;
            break;
        default:
            EncPwd = EncPwdDev;
            break;
        }
        // write the major loaded config environment
        logger.AddLog(LOG_VERBOSE, "Loaded configuration file");
        ShowLoggingLevel();
        logger.AddLog(LOG_DEBUG, "WorldPort: %d", config.worldPort);
        logger.AddLog(LOG_DEBUG, "ServerPort: %d", config.serverPort);
        logger.AddLog(LOG_DEBUG, "ServerIntPort: %d", config.serverIntPort);
        logger.AddLog(LOG_DEBUG, "ServerExPort: %d", config.serverExPort);
        logger.AddLog(LOG_DEBUG, "Protocol Version: %d", config.ProtocolVer);
        logger.AddLog(LOG_DEBUG, "Log Directory: %s", config.logDirectory);
        logger.AddLog(LOG_DEBUG, "DBConnectionNum: %d, GameID: %d", config.numDBConn, config.gameId);
        logger.AddLog(LOG_DEBUG, "ServerThread: %d", config.numServerThread);

        if (config.encrypt) {
            logger.AddLog(LOG_DEBUG, "Encrypt: True");
        } else {
            logger.AddLog(LOG_DEBUG, "Encrypt: False");
        }

        if (config.DesApply) {
            logger.AddLog(LOG_DEBUG, "DesApply: True");
        } else {
            logger.AddLog(LOG_DEBUG, "DesApply: False");
        }

        if (config.OneTimeLogOut) {
            logger.AddLog(LOG_DEBUG, "OneTimeLogOut: True");
        } else {
            logger.AddLog(LOG_DEBUG, "OneTimeLogOut: False");
        }

        if (config.RestrictGMIP) {
            logger.AddLog(LOG_DEBUG, "RestrictGMIP: True");
        } else {
            logger.AddLog(LOG_DEBUG, "RestrictGMIP: False");
        }

        logger.AddLog(LOG_DEBUG, "GMIP: %d.%d.%d.%d",
            config.GMIP.S_un.S_un_b.s_b1,
            config.GMIP.S_un.S_un_b.s_b2,
            config.GMIP.S_un.S_un_b.s_b3,
            config.GMIP.S_un.S_un_b.s_b4);
        logger.AddLog(LOG_DEBUG, "logdPort: %d, logdReconnectInterval: %d", config.LogDPort, config.LogDReconnectInterval);
        logger.AddLog(LOG_NORMAL, "Git Commit Hash: %s", buildVersion);
        if ( config.AcceptCallNum == 0 ){
            logger.AddLog(LOG_ERROR, "AcceptCallNull" );
            config.AcceptCallNum = 1;
        }
        if ( config.SocketTimeOut == 0 ){
            logger.AddLog(LOG_ERROR, "SocketTimeOut" );
            config.SocketTimeOut = 180;
        }

        if ( config.WaitingUserLimit == 0 ){
            logger.AddLog(LOG_ERROR, "WaitingUserLimit" );
            config.WaitingUserLimit = 100;
        }

        if ( config.useForbiddenIPList ) {
            logger.AddLog(LOG_NORMAL, "LOAD FORBIDDEN IP LIST" );
            forbiddenIPList.Load( "etc\\BlockIPs.txt" );
        }

        if (!g_linDB->Init(config.numDBConn)) {
            ShowDBInitError();
            exit(0);
        }

        g_ServerList.Load();

        CDBConn conn(g_linDB);
        conn.Execute( "update worldstatus set status=0" );        

    // 2003-07-15 // logd paste

        CreateIOThread( );
        if ( config.UseLogD ) {

            SOCKET LOGSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
            sockaddr_in Destination;
            Destination.sin_family = AF_INET;
            Destination.sin_addr   = config.LogDIP;
            Destination.sin_port   = htons( (u_short)config.LogDPort );

            int ErrorCode = connect( LOGSock, ( sockaddr *)&Destination, sizeof( sockaddr ));

            pLogSocket = CLogSocket::Allocate(LOGSock);
            pLogSocket->SetAddress( config.LogDIP );
            if ( ErrorCode == SOCKET_ERROR ){
                pLogSocket->CloseSocket();
            } else {
                pLogSocket->Initialize( g_hIOCompletionPortInt );
            }
        }

        if ( config.UseIPServer ) {

            SOCKET IPSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
            sockaddr_in Destination;
            Destination.sin_family = AF_INET;
            Destination.sin_addr   = config.IPServer;
            Destination.sin_port   = htons( (u_short)config.IPPort );

            int ErrorCode = connect( IPSock, ( sockaddr *)&Destination, sizeof( sockaddr ));

            pIPSocket = new CIPSocket(IPSock);
            pIPSocket->SetAddress( config.IPServer );
            if ( ErrorCode == SOCKET_ERROR ){
                pIPSocket->CloseSocket();
            } else
                pIPSocket->Initialize( g_hIOCompletionPort );
        }

        if ( config.UseWantedSystem ) {
            SOCKET WantedSocket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
            sockaddr_in WantedAddr;
            WantedAddr.sin_family = AF_INET;
            WantedAddr.sin_addr = config.WantedIP;
            WantedAddr.sin_port = htons( (u_short)config.WantedPort );
            
            int ErrorCode = connect( WantedSocket, (sockaddr *)&WantedAddr, sizeof(sockaddr));
            pWantedSocket = new CWantedSocket( WantedSocket );
            pWantedSocket->SetAddress( config.WantedIP );
            if ( ErrorCode == SOCKET_ERROR ) {
                pWantedSocket->CloseSocket();
            } else 
                pWantedSocket->Initialize( g_hIOCompletionPortInt );
        }

        listenThread = (HANDLE)_beginthreadex(NULL, 0, ListenThread, 0, 0, &listenThreadId);
    }

    // end Init
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    if ( listenThread != NULL )
        CloseHandle(listenThread);    
    
    while( !globalTeminateEvent )
        Sleep(1000);
    
    Sleep(2000);

    server->ReleaseRef();
    serverEx->ReleaseRef();
    serverInt->ReleaseRef();
    g_linDB->ReleaseRef();

    WSACleanup();

    return 0;
}


