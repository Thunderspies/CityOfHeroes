//#include <winsock2.h>

#include <utilitieslib/utils/wininclude.h>
#include <utilitieslib/utils/file.h>
#include "win_init.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "resource.h"
#include <direct.h>
#include <mmsystem.h>

HWND    hwnd;
static HDC        hDC;
static WNDCLASS    wc;

char nombre[100] = "City Of Heroes Test Client Launcher";
#define MYNAME nombre


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

static LRESULT CALLBACK MainWndProc ( HWND    hWnd,
                          UINT    uMsg,
                          WPARAM  wParam,
                          LPARAM  lParam ) {
    LRESULT lRet = 1;

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
        windowExit();
        DestroyWindow( hWnd );
        break;
    case WM_DESTROY:
//        windowExit();
//        PostQuitMessage( 0 );
        break;
    case WM_QUIT:
        windowExit();
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

void windowExit()
{
    if( hDC )
        ReleaseDC( hwnd, hDC );
    DestroyWindow( hwnd );

    exit(0);
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
        windowExit();
}

void windowShowError(PTSTR str)
{
    MessageBox(hwnd,str,TEXT("Fatal error"),MB_OK | MB_ICONEXCLAMATION);
}

void winMsgAlert(PTSTR str)
{
    MessageBox( 0, str, TEXT("COH Error!"), MB_ICONERROR);
}

int winMsgYesNo(char *str)
{
    if (MessageBoxA( 0, str, "COH Question?", MB_YESNO | MB_ICONQUESTION) == IDYES)
        return 1;
    return 0;
}

char *winGetFileName(char *fileMask,char *fileName,int save)
{
    OPENFILENAMEA theFileInfo;
    //char filterStrs[256];
    int        ret;
    char    base[_MAX_PATH];

    _getcwd(base,_MAX_PATH);
    memset(&theFileInfo,0,sizeof(theFileInfo));
    theFileInfo.lStructSize = sizeof(OPENFILENAME);
    theFileInfo.hwndOwner = hwnd;
    theFileInfo.hInstance = NULL;
    theFileInfo.lpstrFilter = fileMask;
    theFileInfo.lpstrCustomFilter = NULL;
    theFileInfo.lpstrFile = fileName;
    theFileInfo.nMaxFile = 255;
    theFileInfo.lpstrFileTitle = NULL;
    theFileInfo.lpstrInitialDir = NULL;
    theFileInfo.lpstrTitle = NULL;
    theFileInfo.Flags = OFN_LONGNAMES | OFN_OVERWRITEPROMPT;// | OFN_PATHMUSTEXIST;
    theFileInfo.lpstrDefExt = NULL;

    if (save)
        ret = GetSaveFileNameA(&theFileInfo);
    else
        ret = GetOpenFileNameA(&theFileInfo);
    _chdir(base);

    if (ret)
        return fileName;
    else
        return NULL;
}

int        main();


HINSTANCE glob_hinstance;
HINSTANCE   g_hInst=NULL;                    // instance handle
HWND        g_hWnd=NULL;                        // window handle



int WINAPI WinMain ( HINSTANCE hInstance, HINSTANCE hPrevInstance, 
                     LPSTR lpCmdLine, int nCmdShow )
{
    char    *argv[20],*s,delim[] = " \t",buf[1000] = "";
    int        argc;
    FILE    *file;
    //WNDCLASS WndClass;

    g_hInst = glob_hinstance = hInstance;

/*    WndClass.style = CS_HREDRAW | CS_VREDRAW;
    WndClass.lpfnWndProc = MainWndProc;
    WndClass.cbClsExtra = 0;
    WndClass.cbWndExtra = 0;
    WndClass.hInstance = g_hInst;
    WndClass.hIcon = LoadIcon(g_hInst, "APPICON");
    WndClass.hCursor = LoadCursor(0, IDC_ARROW);
    WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    WndClass.lpszMenuName = "MainMenu";
    WndClass.lpszClassName = MYNAME;
    if ( !RegisterClass( &WndClass) ) {
        MessageBox( 0, "Couldn't Register Window Class", MYNAME, MB_ICONERROR );
    }
*/

    argv[0] = MYNAME;
    file = fopen("cmdline.txt","rt");
    if (file)
    {
        fgets(buf,sizeof(buf),file);
        s = buf + strlen(buf) - 1;
        if (*s == '\n')
            *s = 0;
        fclose(file);
    }
    strcat(buf," ");
    strcat(buf,lpCmdLine);
    for(s = strtok(buf,delim),argc=1;s && argc<ARRAY_SIZE(argv);argc++,s = strtok(0,delim))
        argv[argc] = s;
    //newConsoleWindow();
    return main(argc,argv);
}

