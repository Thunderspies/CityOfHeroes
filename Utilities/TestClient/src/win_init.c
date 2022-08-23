//#include <winsock2.h>
#include <utilitieslib/utils/wininclude.h>
#include <mmsystem.h>
#include <winuser.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "win_init.h"
#include "resource.h"
#include <utilitieslib/utils/utils.h>
#include <direct.h>
#include "testClientInclude.h"
#include "tchar.h"
#include <utilitieslib/utils/StringUtil.h>

HWND    hwnd;
static HDC        hDC;
static WNDCLASS    wc;

char nombre[100] = "City Of Heroes Tester";
wchar_t nombreW[] = L"City Of Heroes Tester";
#define MYNAME nombre
#define MYNAMEW nombreW


void windowSize(int *width,int *height)
{
    RECT    rect;

    GetClientRect(hwnd,&rect);
    *width    = rect.right - rect.left;
    *height    = rect.bottom - rect.top;
}



void windowClientSize(int *width, int *height){
    RECT clientRect;

    GetClientRect(hwnd, &clientRect);
    *width = clientRect.right - clientRect.left;
    *height = clientRect.bottom - clientRect.top;

}

int windowScaleY(int y)
{
    int        w,h;

    windowSize(&w,&h);
    return (h * y) / 480;
}

void windowPosition(int *left,int *top)
{
    WINDOWPLACEMENT placement;
    RECT windowRect;
    //RECT clientRect;

    //GetClientRect(hwnd, &clientRect);
    //*left = clientRect.left;
    //*top = clientRect.top;
    
    GetWindowRect(hwnd, &windowRect);
    
    *left = windowRect.left;
    *top = windowRect.top;
    
    return;

    placement.length = sizeof(placement);
    GetWindowPlacement(hwnd,&placement);
    *left = placement.rcNormalPosition.left;
    *top = placement.rcNormalPosition.top;
}

static LONG WINAPI MainWndProc ( HWND    hWnd,
                          UINT    uMsg,
                          WPARAM  wParam,
                          LPARAM  lParam ) {
    LONG    lRet = 1;

    switch ( uMsg ) {
    case WM_SYSCOMMAND:
        switch (wParam)
        {
            case SC_SCREENSAVE:
            case SC_MONITORPOWER:
            return 0;
        }
        lRet = DefWindowProc (hWnd, uMsg, wParam, lParam);
    case WM_SETCURSOR:
        if(LOWORD(lParam) == HTCLIENT){
        }else{
            lRet = DefWindowProc (hWnd, uMsg, wParam, lParam);
        }
        break;
    
    // Detect when the game window doesn't have focus.
    // Don't draw stuff on the "screen" if the game is inactive.
    case WM_CLOSE:
        windowExit(0);
        DestroyWindow( hWnd );
        break;
    case WM_DESTROY:
//        windowExit(0);
//        PostQuitMessage( 0 );
        break;
    case WM_QUIT:
        windowExit(0);
        break;

    
    case WM_CHAR:
        if(wParam == '\b')
            break;

        break;

    case WM_KEYDOWN:
    case WM_KEYUP:
    case WM_DEADCHAR:
    case WM_SYSKEYDOWN:
    case WM_SYSKEYUP:
    case WM_SYSCHAR:
    case WM_SYSDEADCHAR:
    case WM_KEYLAST:
        break;
    default:
        lRet = DefWindowProc (hWnd, uMsg, wParam, lParam);
        break;
    }

    return lRet;
}

void windowResize(int width,int height)
{

}

void windowExit(int exitCode)
{
    if( hDC )
        ReleaseDC( hwnd, hDC );
    DestroyWindow( hwnd );

    statusUpdate("ERROR");

    error_exit(0);
}

void windowExitDlg(void *data)
{
    windowExit(0);
}

void    windowUpdate()
{
   
}

void windowProcessMessages()
{
    MSG msg;
    int    quit=0;

    while( PeekMessage( &msg, 0, 0, 0, PM_REMOVE ) )
    {
        TranslateMessage( &msg );
        DispatchMessage( &msg );
        if(0 && msg.message == WM_QUIT)
            quit = 1;
    }
    if (quit)
        windowExit(0);
}

void windowShowError(char *str)
{
    setConsoleTitle("Fatal Error");
    statusUpdate("ERROR");
    printf("Fatal Error\n%s\n", str);
    error_exit(0);
}

void winMsgAlert(char *str)
{
    printf("winMsgAlert: %s\n", str);
}

int winMsgYesNo(char *str)
{
    if (MessageBoxA( 0, str, "COH Question?", MB_YESNO | MB_ICONQUESTION) == IDYES)
        return 1;
    return 0;
}

char *winGetFileName(char *fileMask,char *fileName,int save)
{
    __debugbreak(); // don't use
    return NULL;
}

int        main();


HINSTANCE glob_hinstance;

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
    char *s;
    char    *argv[128];
    char delim[] = " \t";
    char buf[1024] = "";
    int        argc;
    FILE    *file;

    wc.style         = CS_OWNDC;
    wc.lpfnWndProc   = (WNDPROC)MainWndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance;
    wc.hIcon         = 0;
    wc.hCursor       = 0;//LoadCursor( NULL, IDC_ARROW );
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.lpszMenuName  = MYNAMEW;
    wc.lpszClassName = MYNAMEW;
    glob_hinstance = hInstance;

    if ( !RegisterClass( &wc ) ) {
        MessageBox( 0, _T("Couldn't Register Window Class"), MYNAMEW, MB_ICONERROR );
    }

    argv[0] = MYNAME;
    file = _tfopen(_T("cmdline.txt"),_T("rt"));
    if (file)
    {
        TCHAR tmp[ARRAY_SIZE(buf)];
        int len;
        _fgetts(tmp,ARRAY_SIZE(tmp),file);
        len = WideToUTF8StrConvert( tmp, buf, ARRAY_SIZE(buf) );
        s = buf + strlen(buf) - 1;
        if (*s == '\n')
            *s = 0;
        fclose(file);
    }
    strcat(buf," ");
    strcat(buf,WideToUTF8StrTempConvert(lpCmdLine,NULL));
    for(s = strtok(buf,delim),argc=1;s && argc<ARRAY_SIZE(argv);argc++,s = strtok(0,delim))
        argv[argc] = s;
    newConsoleWindow();
    return main(argc,argv);
}
