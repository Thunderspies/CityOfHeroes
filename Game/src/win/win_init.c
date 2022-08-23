//#include <winsock2.h>
#include <utilitieslib/components/StashTable.h>
#include <utilitieslib/utils/wininclude.h>
#include <mmsystem.h>
#include <winuser.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "win/win_init.h"
#include <utilitieslib/stdtypes.h>
#include <utilitieslib/utils/error.h>
#include <utilitieslib/utils/cmdoldparse.h>
#include "clientcomm/clientcomm.h"
#include "graphics/gfx.h"
#include <utilitieslib/utils/memcheck.h>
#include "win/input.h"
#include "UI/uiInput.h"
#include "cmdparse/cmdgame.h"
#include "win/win_cursor.h"
#include "sound/sound.h"
#include "resource.h"
#include <direct.h>
#include "clientcomm/autoResumeInfo.h"
#include "clientcomm/authclient.h"
#include "graphics/gfxwindow.h"
#include "UI/uiWindows.h"
#include "UI/uiWindows_init.h"
#include <utilitieslib/utils/sysutil.h>
#include <process.h>
#include "demo.h"
#include "UI/uiCursor.h"
#include "graphics/font.h"
#include <utilitieslib/utils/utils.h>
#include "graphics/seqgraphics.h"
#include <utilitieslib/utils/winutil.h>
#include "UI/sprite/sprite_text.h"
#include <utilitieslib/utils/StringUtil.h>
#include "edit/edit_net.h"
#include "render/thread/rt_queue.h"
#include <utilitieslib/utils/file.h>
#include "render/tex.h"
#include "render/renderUtil.h"
#include "UI/uiIME.h"
#include "clientError.h"
#include "render/pbuffer.h"
#include "render/renderssao.h"
#include <utilitieslib/language/AppLocale.h>
#include "shlobj.h"
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/utils/strings_opt.h>
#include "render/model.h"
#include "group/groupfileload.h"
#include "editorUI.h"
#include <utilitieslib/language/MessageStoreUtil.h>
#include "win/hwlight.h"
#include <utilitieslib/utils/log.h>
#include "clientcomm/dbclient.h"
#include <utilitieslib/components/EString.h>

void windowDestroyDisplayContexts(void);
void windowReleaseDisplayContexts(void);
void windowReInitDisplayContexts(void);


#define MOUSE_THRESHOLD 10

HWND            hwnd;
POINT            initCursorPos;

static WNDCLASSEXA wc;

static int processing_message = 0;
static int s_ignore_wm_size;            // in some cases we want to ignore WM_SIZE messages, e.g. returning to windowed mode
static float s_saved_gamma = -1.0f;
// SetLayeredWindowAttributes
typedef BOOL (__stdcall *tSetLayeredWindowAttributes)( 
    HWND hwnd,
    COLORREF crKey,
    BYTE bAlpha,
    DWORD dwFlags);
tSetLayeredWindowAttributes pSetLayeredWindowAttributes = NULL;
HINSTANCE hLayeredWindowDll = 0;    // The dll to layeredwindow from (USER)


// splash window stuff
HWND        hlogo;            // splash screen handle
static int g_quitlogo;            // signal to exit the logo screen even if it hasn't been created
static int g_show_splash = 1;

char windowName[100] = "City Of Heroes";
char className[100] = "CrypticWindow";

////////////////////////////////////////////////////////////////////// splash logo

#define SPLASH_ALPHASTART    10 // the increment we start at
#define SPLASH_ALPHAINC        100 // number of increments of alpha we will go through
#define SPLASH_ALPHATIME    20 // milliseconds for each alpha increment

#ifndef LWA_ALPHA // Not defined in Win9x headers
#    define LWA_ALPHA               0x00000002
#endif
#ifndef WS_EX_LAYERED
#    define WS_EX_LAYERED           0x00080000
#endif

LONG WINAPI DefWindowProc_timed( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    LONG ret;
    PERFINFO_AUTO_START("DefWindowProc", 1);
    ret = DefWindowProcA(hWnd, uMsg, wParam, lParam);
    PERFINFO_AUTO_STOP();
    return ret;
}

static LONG WINAPI SplashProc ( HWND    hWnd,
                          UINT    uMsg,
                          WPARAM  wParam,
                          LPARAM  lParam ) 
{
static HBITMAP hbmLogo;
static int width;
static int height;
static int curalpha = 0;

    switch (uMsg)
    {
    case WM_CREATE:
        {
            BITMAP bmpdata;
            int screenwidth;
            int screenheight;
            int opacity;

            // load bitmap and get size
            if (game_state.skin == UISKIN_PRAETORIANS)
                hbmLogo = LoadBitmapA(wc.hInstance, MAKEINTRESOURCEA(IDB_ROGUELOGO));
            else if (game_state.skin == UISKIN_VILLAINS)
                hbmLogo = LoadBitmapA(wc.hInstance, MAKEINTRESOURCEA(IDB_COVLOGO));
            else
                hbmLogo = LoadBitmapA(wc.hInstance, MAKEINTRESOURCEA(IDB_LOGO));
            GetObject(hbmLogo, sizeof(BITMAP), &bmpdata);
            width = bmpdata.bmWidth > 0? bmpdata.bmWidth : bmpdata.bmWidth * -1;
            height = bmpdata.bmHeight > 0? bmpdata.bmHeight : bmpdata.bmHeight * -1;

            // resize to fit
            screenwidth = GetSystemMetrics(SM_CXSCREEN);
            screenheight = GetSystemMetrics(SM_CYSCREEN);
            MoveWindow(hWnd, (screenwidth - width) / 2, (screenheight - height) /2, width, height, FALSE);


            // see if we can get SetLayeredWindowAttributes
            hLayeredWindowDll = LoadLibraryA( "user32.dll" );
            if (hLayeredWindowDll)
            {
                pSetLayeredWindowAttributes = (tSetLayeredWindowAttributes) GetProcAddress(hLayeredWindowDll, "SetLayeredWindowAttributes");
            }

            // set layered style and timer
            if (pSetLayeredWindowAttributes)
            {
                SetWindowLong(hWnd, GWL_EXSTYLE, 
                    GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
                curalpha = SPLASH_ALPHASTART;
                opacity = (curalpha * 255) / SPLASH_ALPHAINC;
                pSetLayeredWindowAttributes(hWnd, 0, 0, LWA_ALPHA);
                SetTimer(hWnd, 0, SPLASH_ALPHATIME, 0);
            }

            ShowWindow(hWnd, SW_SHOWNORMAL);

            return 1;
        }

    case WM_TIMER:
        {
            if (pSetLayeredWindowAttributes)
            {
                curalpha++;
                if (curalpha < SPLASH_ALPHAINC)
                {
                    int opacity = (curalpha * 255) / SPLASH_ALPHAINC;
                    pSetLayeredWindowAttributes(hWnd, 0, opacity, LWA_ALPHA);
                    SetTimer(hWnd, 0, SPLASH_ALPHATIME, 0);
                }
                else
                {
                    pSetLayeredWindowAttributes(hWnd, 0, 255, LWA_ALPHA);
                }
            }
            return 0;
        }

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc, hdcmem;
            HBITMAP oldbitmap;

            hdc = BeginPaint(hWnd, &ps);
            hdcmem = CreateCompatibleDC(hdc);
            oldbitmap = (HBITMAP)SelectObject(hdcmem, hbmLogo);
            BitBlt(hdc, 0, 0, width, height, hdcmem, 0, 0, SRCCOPY);
            SelectObject(hdcmem, oldbitmap);
            DeleteDC(hdcmem);
            EndPaint(hWnd, &ps);
        }
        return 1;

    case WM_DESTROY:
        DeleteObject(hbmLogo);
        PostQuitMessage(0);
        break;

    case WM_KEYDOWN:
    case WM_KEYUP:
    case WM_SYSKEYUP:
    case WM_SYSKEYDOWN:
    case WM_LBUTTONDBLCLK:
    case WM_DEADCHAR:
    case WM_SYSCHAR:
    case WM_SYSDEADCHAR:
    case WM_KEYLAST:
    case WM_LBUTTONDOWN:
    case WM_MBUTTONDOWN:
    case WM_RBUTTONDOWN:
    case WM_MOUSEMOVE:
        break;

    }
    return DefWindowProc_timed(hWnd, uMsg, wParam, lParam);
}

void RegisterSplashWindow()
{
    WNDCLASSA splashwc;

    splashwc.hInstance       = glob_hinstance;    
    splashwc.style         = CS_OWNDC;
    splashwc.lpfnWndProc   = (WNDPROC)SplashProc;
    splashwc.cbClsExtra    = 0;
    splashwc.cbWndExtra    = 0;
    if ( locGetIDInRegistry() == locGetIDByWindowsLocale(LOCALE_KOREAN) )
    {
        splashwc.hIcon         = LoadImageA(glob_hinstance, MAKEINTRESOURCEA(IDI_KOREAN), IMAGE_ICON, 
            GetSystemMetrics(SM_CXICON), GetSystemMetrics(SM_CYICON), 0);
    }
    else if (game_state.skin == UISKIN_PRAETORIANS)
    {
        splashwc.hIcon         = LoadImageA(glob_hinstance, MAKEINTRESOURCEA(IDI_ROGUE), IMAGE_ICON, 
            GetSystemMetrics(SM_CXICON), GetSystemMetrics(SM_CYICON), 0);
    }
    else if (game_state.skin == UISKIN_VILLAINS)
    {
        splashwc.hIcon         = LoadImageA(glob_hinstance, MAKEINTRESOURCEA(IDI_COV), IMAGE_ICON, 
            GetSystemMetrics(SM_CXICON), GetSystemMetrics(SM_CYICON), 0);
    }
    else
    {
        splashwc.hIcon         = LoadImageA(glob_hinstance, MAKEINTRESOURCEA(IDI_OUROBOROS), IMAGE_ICON, 
            GetSystemMetrics(SM_CXICON), GetSystemMetrics(SM_CYICON), 0);
    }
    splashwc.hCursor       = 0;
    splashwc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    splashwc.lpszMenuName  = "CrypticLogo";
    splashwc.lpszClassName = "CrypticLogo";

    RegisterClassA( &splashwc );
}

// run the splash screen during startup
DWORD WINAPI SplashThread(void* data)
{
    MSG msg;

    // get the window started
    hlogo = CreateWindowA("CrypticLogo", windowName, WS_POPUP | WS_BORDER, 
        100, 100, 400, 200, NULL, NULL, wc.hInstance, NULL);

    // run the message pump
    while (!g_quitlogo && GetMessageA(&msg, 0, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }
    DestroyWindow(hlogo);
    hlogo = NULL;
    return 0;
}

static void CreateSplash()
{
    g_quitlogo = 0;
    RegisterSplashWindow();
    _beginthreadex(NULL, 0, SplashThread, NULL, 0, NULL);
    Sleep(10); // make sure the window starts
}

void DestroySplash()
{
    SendMessageA(hlogo, WM_DESTROY, 0, 0);
    g_quitlogo = 1;
    Sleep(10); // let the window close before we go on
}


int override_window_size=0;
int override_window_size_width,override_window_size_height;

void windowPhysicalSize(int *width,int *height)
{
    RECT    rect;

    if (game_state.fullscreen)
    {
        *width = game_state.screen_x;
        *height = game_state.screen_y;
        return;
    }

    GetClientRect(hwnd,&rect); 
    *width    = rect.right - rect.left;
    *height    = rect.bottom - rect.top;
    if (*width == 0 || *height == 0) // window minimized
    {
        *width = game_state.screen_x;
        *height = game_state.screen_y;
    }
    else
    {
        game_state.screen_x = *width;
        game_state.screen_y = *height;

        if(!game_state.maximized)
        {
            game_state.screen_x_restored = game_state.screen_x;
            game_state.screen_y_restored = game_state.screen_y;
        }
    }
}

void windowClientSize(int *width,int *height)
{
    if (override_window_size) {
        *width = override_window_size_width;
        *height = override_window_size_height;
        return;
    }

    windowPhysicalSize(width, height);
}

void windowClientSizeThisFrame(int *width,int *height)
{
    static struct {
        int    width;
        int height;
        int frame;
    } cached;
    
    if (override_window_size) {
        *width = override_window_size_width;
        *height = override_window_size_height;
        return;
    }

    if(cached.frame == global_state.global_frame_count)
    {
        *width = cached.width;
        *height = cached.height;
        return;
    }

    windowPhysicalSize(width, height);

    cached.frame = global_state.global_frame_count;
    cached.width = *width;
    cached.height = *height;
}


// we always work with client coordinates from the game code
void windowSize(int *width, int *height)
{
    windowClientSize(width, height);
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
    
    GetWindowRect(hwnd, &windowRect);
    
    *left = windowRect.left;
    *top = windowRect.top;
    
    return;

    placement.length = sizeof(placement);
    GetWindowPlacement(hwnd,&placement);
    *left = placement.rcNormalPosition.left;
    *top = placement.rcNormalPosition.top;
}

void windowSetSize(int width, int height)
{
    int x, y;
    int result;
    windowPosition(&x, &y);

    if (!game_state.fullscreen)
    {
        width += GetSystemMetrics(SM_CXSIZEFRAME) * 2;
        height += GetSystemMetrics(SM_CYSIZEFRAME) * 2 + GetSystemMetrics(SM_CYSIZE) + 1;
    }
    result = MoveWindow(hwnd, x, y, width, height, 1);
}

static LONG WINAPI MainWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

static LONG WINAPI MainWndProcCallback(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    // This makes it possible to compile-and-continue MainWndProc.

    return MainWndProc(hWnd, uMsg, wParam, lParam);
}

typedef enum ImeLangType
{
    DEFAULT,                
    TRADITIONAL_CHINESE,    
    JAPANESE,
    KOREAN,
    SIMPLIFIED_CHINESE
} ImeLangType;


static int s_CodePageCurLang()
{
    int res = CP_ACP; // default to ASCII    
    HKL hkl = GetKeyboardLayout(0); //ab: is this a leak? I suppose it'll never be released anyway...

    // see http://msdn.microsoft.com/library/default.asp?url=/library/en-us/intl/nls_238z.asp    
    switch (LOWORD(hkl))
    {
        // Traditional Chinese 
    case MAKELANGID(LANG_CHINESE,SUBLANG_CHINESE_TRADITIONAL):
        res = 950; // TRADITIONAL CHINESE
        break;
        
        // Japanese
    case MAKELANGID(LANG_JAPANESE,SUBLANG_DEFAULT):
        res = 932; // JAPANESE
        break;
        
        // Korean
    case MAKELANGID(LANG_KOREAN,SUBLANG_DEFAULT):
        res = 949; // KOREAN
        break;
        
        // Simplified Chinese
    case MAKELANGID(LANG_CHINESE,SUBLANG_CHINESE_SIMPLIFIED):
        res = 936;
        break;
        
    default:
        res = CP_ACP;
        break;
    };
    
    // --------------------
    // finally
    
    return res;
}

static int windowMessageTimerCount;

static void startWindowMessageTimer(UINT msg)
{
    #define TIMER_COUNT 1000
    
    static struct {
        #if USE_NEW_TIMING_CODE
            PerfInfoStaticData*        piStatic;
        #else
            PerformanceInfo*        piStatic;
        #endif
        char*                    name;
    }* timers;
    
    if(!timers)
    {
        int i;
        
        timers = calloc(sizeof(*timers), TIMER_COUNT);
        
        #define SET_NAME(x) if(x >= 0 && x < TIMER_COUNT)timers[x].name = #x;
        SET_NAME(WM_SYSCOMMAND);
        SET_NAME(WM_SETCURSOR);
        SET_NAME(WM_WINDOWPOSCHANGED);
        SET_NAME(WM_SIZE);
        SET_NAME(WM_ACTIVATEAPP);
        SET_NAME(WM_SETFOCUS);
        SET_NAME(WM_CLOSE);
        SET_NAME(WM_DESTROY);
        SET_NAME(WM_QUIT);
        SET_NAME(WM_CHAR);
        SET_NAME(WM_KEYDOWN);
        SET_NAME(WM_LBUTTONDBLCLK);
        SET_NAME(WM_KEYUP);
        SET_NAME(WM_SYSKEYUP);
        SET_NAME(WM_SYSKEYDOWN);
        SET_NAME(WM_DEADCHAR);
        SET_NAME(WM_SYSCHAR);
        SET_NAME(WM_SYSDEADCHAR);
        SET_NAME(WM_KEYLAST);
        #undef SET_NAME
        
        for(i = 0; i < TIMER_COUNT; i++)
        {
            if(!timers[i].name)
            {                
                char buffer[100];
                
                STR_COMBINE_BEGIN(buffer);
                STR_COMBINE_CAT("WM_(");
                STR_COMBINE_CAT_D(i);
                STR_COMBINE_CAT(")");
                STR_COMBINE_END();
                
                timers[i].name = strdup(buffer);
            }
        }
    }
    
    if(msg < TIMER_COUNT)
    {
        PERFINFO_AUTO_START_STATIC(timers[msg].name, &timers[msg].piStatic, 1);
        
        windowMessageTimerCount++;
    }
    
    #undef TIMER_COUNT 
}

static void stopWindowMessageTimer()
{
    if(windowMessageTimerCount)
    {
        PERFINFO_AUTO_STOP();
        
        windowMessageTimerCount--;
    }
}

static LONG WINAPI MainWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LONG    lRet = 1;

#if DEBUG
    if (isCrashed())
        return DefWindowProcA(hWnd, uMsg, wParam, lParam);
#endif

    startWindowMessageTimer(uMsg);
    
    if( uiIME_MsgProc( hWnd, uMsg, wParam, lParam ) )
    {
        lRet = 0;
    }
    else switch ( uMsg ) 
    {
        xcase WM_SYSCOMMAND:
            switch (wParam)
            {
            case SC_SCREENSAVE:
            case SC_MONITORPOWER:
                stopWindowMessageTimer();
                return 0;
            }
            lRet = DefWindowProc_timed (hWnd, uMsg, wParam, lParam);
            
        xcase WM_SETCURSOR:
            if(LOWORD(lParam) == HTCLIENT){
                game_state.can_set_cursor = 1;
                hwcursorSet();
            }else{
                game_state.can_set_cursor = 0;
                lRet = DefWindowProc_timed (hWnd, uMsg, wParam, lParam);
            }
            
        xcase WM_WINDOWPOSCHANGED:
        {
            // Detect when the game window is being maximized/minimized/restored.
            
            WINDOWPOS* pos = (WINDOWPOS*)lParam;
            
            // Call DefWindowProc so that WM_SIZE gets sent.
            
            DefWindowProc_timed(hWnd, uMsg, wParam, lParam);
            
            // Now set the position.
            
            //printf("WM_WINDOWPOSCHANGED: %d, %d - %d, %d\n", pos->x, pos->y, pos->cx, pos->cy);
            
            if(!game_state.fullscreen && !game_state.maximized && !game_state.minimized)
            {
                game_state.screen_x_pos = pos->x;
                game_state.screen_y_pos = pos->y;
            }
            //Why was this in autoResumeInfo?
            if (game_state.screen_x_pos < -4096) 
                game_state.screen_x_pos = 0;
            if (game_state.screen_y_pos < -4096) 
                game_state.screen_y_pos = 0;
            
            stopWindowMessageTimer();
            return 0;
        }
        
        xcase WM_SIZE:
        if (!s_ignore_wm_size)
        {
            int newClientWidth;
            int newClientHeight;
            
            // Store the new client height and width in game_state.
            //    Don't store them if they are 0.  May cause divide by 0 errors.
            newClientWidth    = LOWORD(lParam);
            newClientHeight = HIWORD(lParam);
            window_HandleResize( newClientWidth, newClientHeight, game_state.screen_x, game_state.screen_y );
            
            if (!game_state.fullscreen)
            {
                if(newClientWidth)
                {
                    game_state.screen_x = newClientWidth;
                }
                
                if(newClientHeight)
                {    
                    game_state.screen_y = newClientHeight;
                }
            }
            
            switch(wParam){
                xcase SIZE_MAXIMIZED:
                    game_state.maximized = 1;
                    game_state.minimized = 0;
                xcase SIZE_MINIMIZED:
                    game_state.maximized = 0;
                    game_state.minimized = 1;
                xcase SIZE_RESTORED:
                    game_state.maximized = 0;
                    game_state.minimized = 0;
                    game_state.screen_x_restored = game_state.screen_x;
                    game_state.screen_y_restored = game_state.screen_y;
            }
            
            gfxWindowReshape();
        }
        
        // Detect when the game window doesn't have focus.
        // Don't draw stuff on the "screen" if the game is inactive.
        xcase WM_ACTIVATEAPP:
            switch(wParam){
            case 0:
                // The app is being deactivated.
                game_state.inactiveDisplay = 1;
                break;
            case 1:
                // The app is being activated.
                game_state.inactiveDisplay = 0;
                break;
            }

            // priority changes moved to game_updateThreadPriority()

            // force gamma set
            if (wParam) {
                rdrNeedGammaReset();
            } else {
                // Play nice and restore it
                rdrRestoreGammaRamp();
            }
            
            lRet = DefWindowProc_timed (hWnd, uMsg, wParam, lParam);
            break;
            
        xcase WM_SETFOCUS:
            rdrNeedGammaReset();
            
        xcase WM_CLOSE:
            commSendQuitGame(0);

        xcase WM_DESTROY:
            rdrRestoreGammaRamp();
            //windowExit(0);
            //PostQuitMessage( 0 );

        xcase WM_QUIT:
            windowExit(0);

        xcase WM_CHAR:
        {
            KeyInputAttrib attrib = 0;
            
            // filter out edit control characters.
            if(wParam == '\b')    // backspace character?
                break;
            if(wParam == 22)    // control-v character?
                break;
            
            if(ctrlKeyState)
                attrib |= KIA_CONTROL;
            if(altKeyState)
                attrib |= KIA_ALT;
            if(shiftKeyState)
                attrib |= KIA_SHIFT;
            
            if(wParam >= 128)
            {
                wchar_t wChar;
//#ifdef _UNICODE
//                wChar = nChar;
//#else
                MultiByteToWideChar(s_CodePageCurLang(), 0, (char *)&wParam, 1, &wChar, 2);
//#endif
                
                inpKeyAddBuf(KIT_Unicode, wChar, lParam, attrib);
            }
            else
            {
                inpKeyAddBuf(KIT_Ascii, wParam, lParam, attrib);
            }
        }
        
        xcase WM_KEYDOWN:
            game_state.activity_detected = 1;
            // only allow input when not composing ime
            if(wParam == VK_CONTROL)
                ctrlKeyState = 1;
            if(wParam == VK_MENU)
                altKeyState = 1;
            if(wParam == VK_SHIFT)
                shiftKeyState = 1;
            // If the key is a key that is relevant to text editing,
            if(inpVKIsTextEditKey(wParam, ctrlKeyState))
            {
                int scancode;
                KeyInputAttrib attrib = 0;
                    
                // convert the key into its scan code and record it.
                extern int GetScancodeFromVirtualKey(WPARAM wParam, LPARAM lParam);
                scancode = GetScancodeFromVirtualKey( wParam, lParam );
                    
                if(ctrlKeyState)
                    attrib |= KIA_CONTROL;
                if(altKeyState)
                    attrib |= KIA_ALT;
                if(shiftKeyState)
                    attrib |= KIA_SHIFT;
                inpKeyAddBuf(KIT_EditKey, scancode, lParam, attrib);
            }
            
        xcase WM_KEYUP:
        {
            game_state.activity_detected = 1;
            if(wParam == VK_CONTROL)
                ctrlKeyState = 0;
            else if(wParam == VK_MENU)
                altKeyState = 0;
            else if(wParam == VK_SHIFT)
                shiftKeyState = 0;
        }

        xcase WM_SYSKEYUP:
        {
            game_state.activity_detected = 1;
            if(wParam == VK_CONTROL)
                ctrlKeyState = 0;
            else if(wParam == VK_MENU)
                altKeyState = 0;
            else if(wParam == VK_SHIFT)
                shiftKeyState = 0;
        }

        xcase WM_SYSKEYDOWN:
        {
            game_state.activity_detected = 1;
            if(wParam == VK_MENU)
                altKeyState = 1;
        }

        xcase WM_SYSCHAR:    // character key that is pressed while the ALT key is down
        {
            game_state.activity_detected = 1;
            if(wParam == 0xD)    // ALT-ENTER
            {    
                // check for keyboard ALT-ENTER to toggle full screen mode
                game_state.fullscreen_toggle = !game_state.fullscreen_toggle;
            }
        }

        xcase WM_DEADCHAR:
        case WM_SYSDEADCHAR:
        case WM_KEYLAST:
        case WM_LBUTTONDOWN:
        case WM_MBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_LBUTTONDBLCLK:
            game_state.activity_detected = 1;

        xdefault:
            lRet = DefWindowProc_timed (hWnd, uMsg, wParam, lParam);
    }

    stopWindowMessageTimer();
    return lRet;
}

void winGetSupportedResolutions(GfxResolution * desktop, GfxResolution supported[], int * supportedCount )
{
    static GfxResolution resolution_desktop;
    static GfxResolution resolution_supported[MAX_SUPPORTED_GFXRESOLUTIONS];
    static int resolution_supportedCount;
    static bool resolution_inited=false;

    GfxResolution * gr;
    int success, modeNum, alreadyInList, i;
    DEVMODEA dm = {0};

    if (resolution_inited) {
        *desktop = resolution_desktop;
        memcpy(supported, resolution_supported, sizeof(resolution_supported));
        *supportedCount = resolution_supportedCount;
        return;
    }
    resolution_inited = true;

    for (i = 0; i < MAX_SUPPORTED_GFXRESOLUTIONS; i++)
    {
        gr = &(resolution_supported[i]);
        eaiDestroy(&gr->refreshRates);
    }

    memset( &resolution_desktop, 0, sizeof(resolution_desktop) );
    memset( resolution_supported, 0, sizeof(resolution_supported) );

    resolution_supportedCount = 0;

    dm.dmSize       = sizeof(dm);
    success = EnumDisplaySettingsExA( NULL, ENUM_REGISTRY_SETTINGS,    &dm, 0 );

    if( success && dm.dmBitsPerPel == 32 )
    {
        resolution_desktop.x = dm.dmPelsWidth;
        resolution_desktop.y = dm.dmPelsHeight;
        resolution_desktop.depth = dm.dmBitsPerPel;
    }

    modeNum = 0;
    while( EnumDisplaySettingsExA( NULL, modeNum, &dm, 0 ) )
    {
        modeNum++;
        if( dm.dmBitsPerPel == 32 )
        {
            //TO DO when we support more frequencies, add that.
            if( dm.dmPelsWidth >= 780 && dm.dmPelsHeight >= 580)
            {
                alreadyInList = 0;
                for( i = 0 ; i < resolution_supportedCount ; i++ )
                {
                    gr = &(resolution_supported[i]);
                    if( gr->x == dm.dmPelsWidth && gr->y == dm.dmPelsHeight && gr->depth == dm.dmBitsPerPel )
                    {
                        alreadyInList = 1;
                        eaiPushUnique(&gr->refreshRates, dm.dmDisplayFrequency);
                    }
                }

                if( !alreadyInList )
                {
                    gr = &(resolution_supported[resolution_supportedCount]);
                    resolution_supportedCount++;

                    gr->x = dm.dmPelsWidth;
                    gr->y = dm.dmPelsHeight;
                    gr->depth = dm.dmBitsPerPel;
                    eaiCreate(&gr->refreshRates);
                    eaiPushUnique(&gr->refreshRates, dm.dmDisplayFrequency);

                    if( resolution_supportedCount > MAX_SUPPORTED_GFXRESOLUTIONS )
                        break;
                }
            }
        }
    }


    //Failed to find any supported resolutions, that's crazy, but it happens all
    //the time.
    //int        ws[] = { 800, 1024, 1280, 1600 };
    //int        hs[] = { 600,  768, 1024, 1200 };
    if( resolution_supportedCount == 0 )
    {
        gr = &(resolution_supported[resolution_supportedCount]);
        resolution_supportedCount++;

        gr->x = 800;
        gr->y = 600;
        gr->depth = 32;

        gr = &(resolution_supported[resolution_supportedCount]);
        (resolution_supportedCount)++;

        gr->x = 1024;
        gr->y = 768;
        gr->depth = 32;

        gr = &(resolution_supported[resolution_supportedCount]);
        (resolution_supportedCount)++;

        gr->x = 1280;
        gr->y = 1024;
        gr->depth = 32;

        gr = &(resolution_supported[resolution_supportedCount]);
        (resolution_supportedCount)++;

        gr->x = 1600;
        gr->y = 1200;
        gr->depth = 32;
    }
    //End fix

    *desktop = resolution_desktop;
    memcpy(supported, resolution_supported, sizeof(resolution_supported));
    *supportedCount = resolution_supportedCount;
}
/*
void windowResize(int width,int height)
{
1. grab pointer to original window's render context
2. set original window's rendering context to NULL
3. create second window with the same pixelformat as the original window.
4. wglMakeCurrent the new window and the rendering context
5. destroy the old window
}
*/

void windowInit()
{
    if (game_state.skin == UISKIN_VILLAINS)
        strcpy(windowName, "City of Villains");
    else if (game_state.skin == UISKIN_PRAETORIANS)
        strcpy(windowName, "City of Heroes: Going Rogue");
    else
    {
        if( locGetIDInRegistry() == 3 )
            strcpy(windowName, "City of Hero");
        else
            strcpy(windowName, "City of Heroes");
    }

    if (render_thread)
        windowDestroyDisplayContexts();

    if (hwnd)
        DestroyWindow( hwnd );
    hwnd = 0;
    
    hwnd = CreateWindowA(className,
                         windowName,
                         WS_CLIPSIBLINGS | 
                         WS_CLIPCHILDREN |
                         WS_OVERLAPPEDWINDOW, 
                         100, 100,
                         800, 600,
                         NULL,
                         NULL,
                         wc.hInstance,
                         NULL);
    if (!hwnd) {
        MessageBoxA( 0, "Couldn't Create Window", windowName, MB_ICONERROR);
        gfxResetGfxSettings();
        windowExit(0);
    }
    
    if(isGuiDisabled())    {
        g_show_splash = 0;
    }

    if (g_show_splash) {
        // do splash screen
        CreateSplash();
    }
}

void windowResize(int *widthp,int *heightp,int *refreshRatep,int x0,int y0,int fullscreen)
{
    int        old_ignore = game_state.ignore_activity;
    int        width,height,refreshRate;
    DEVMODE dm;
    DWORD    window_style;
    int useRefreshRate = 1;

    game_state.ignore_activity = 1;
    windowProcessMessages();

    width = *widthp;
    height = *heightp;
    refreshRate = *refreshRatep;
    if (refreshRate < 60)
        useRefreshRate = 0;

    if (!fullscreen)
    {
        width += GetSystemMetrics(SM_CXSIZEFRAME) * 2;
        height += GetSystemMetrics(SM_CYSIZEFRAME) * 2 + GetSystemMetrics(SM_CYSIZE) + 1;
    }
    else
    {        
        GfxResolution desktop;
        GfxResolution supported[MAX_SUPPORTED_GFXRESOLUTIONS] = {0};
        int supportedCount;

        GfxResolution * gr;
        int        i,okRes;

        winGetSupportedResolutions( &desktop, supported, &supportedCount );

        okRes = 0;
        for( i=0 ; i < supportedCount ; i++ )
        {
            int j;
            gr = &supported[i];
            if( gr->x == width && gr->y == height)
            {
                if (!eaiSize(&gr->refreshRates) || !useRefreshRate)
                {
                    okRes = 1;
                    useRefreshRate = 0;
                }
                else
                {
                    for (j = 0; j < eaiSize(&gr->refreshRates); j++)
                    {
                        if (gr->refreshRates[j] == refreshRate || gr->refreshRates[j]==0 ) // On Win98, the refresh rates are all 0
                            okRes = 1;
                    }
                }
            }
        }

        if( !okRes )
        {
            winMsgAlert( textStd("ResolutionNotSupported", width, height, supported[0].x, supported[0].y ));
            *widthp = supported[0].x;
            *heightp = supported[0].y;
            *refreshRatep = 0;
            useRefreshRate = 0;
            {
                GfxSettings gfxSettings;
                gfxGetSettings( &gfxSettings );
                gfxApplySettings( &gfxSettings, 1, false );
            }
        }
            
        width = *widthp;
        height = *heightp;
        refreshRate = *refreshRatep;

        x0 = 0;
        y0 = 0;

    }
    

    if (fullscreen)
    {
        int fullScreenResult, i;

        memset(&dm,0,sizeof(dm));
        dm.dmSize       = sizeof(dm);
        dm.dmPelsWidth  = width;
        dm.dmPelsHeight = height;
        dm.dmDisplayFrequency = refreshRate;
        dm.dmBitsPerPel = 32;//game_state.screendepth;
        dm.dmFields     = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL | ((useRefreshRate&&refreshRate)?DM_DISPLAYFREQUENCY:0);

        fullScreenResult = ChangeDisplaySettings( &dm, CDS_FULLSCREEN );

        if( fullScreenResult != DISP_CHANGE_SUCCESSFUL )
        {
            char * errormsg;
            typedef struct Errors { int errorCode; char msg[200]; } Errors;
            Errors errors[] = { /*weird that dualview isn't defined*/
                { -6 /*DISP_CHANGE_BADDUALVIEW*/,  "DualViewCapable"  },
                { DISP_CHANGE_BADFLAGS,     "BadFlags"  },
                { DISP_CHANGE_BADMODE,      "UnsupportedGraphics"  },
                { DISP_CHANGE_BADPARAM,     "BadFlagParam" }, 
                { DISP_CHANGE_FAILED,       "DriverFailed"  },
                { DISP_CHANGE_NOTUPDATED,   "RegistryFailed"  },
                { DISP_CHANGE_RESTART,      "RestartComputer"  },
                { 0, 0 },
            };

            errormsg = "UnknownFailure";
            for( i = 0; errors[i].errorCode != 0 ; i++ )
            {
                if( fullScreenResult == errors[i].errorCode ) 
                    errormsg = errors[i].msg;
            }
            winMsgAlert(textStd("FullscreenFailure", errormsg ));
            {
                GfxSettings gfxSettings;
                gfxGetSettings( &gfxSettings );
                gfxSettings.screenX = 800;
                gfxSettings.screenY = 600;
                gfxSettings.refreshRate = 60;
                gfxSettings.screenX_pos = 0;
                gfxSettings.screenX_pos = 0;
                gfxSettings.fullScreen  = 0;
                gfxApplySettings( &gfxSettings, 1, false );
            }    

            fullscreen = 0;
            width = 800;
            height = 600;
            *refreshRatep = 0;
            width += GetSystemMetrics(SM_CXSIZEFRAME) * 2;
            height += GetSystemMetrics(SM_CYSIZEFRAME) * 2 + GetSystemMetrics(SM_CYSIZE) + 1;
        } //end Failure to go to full screen
        else
        {
            if (!game_state.noTexFreeOnResChange)
            {
                texFreeAll();
            }

            *refreshRatep = refreshRate = dm.dmDisplayFrequency;
        }
    }
    
    if( !fullscreen )
    {
        EnumDisplaySettings(NULL,ENUM_CURRENT_SETTINGS,&dm);
        // unfortunately, this is before command line parameters are parsed, 
        // so checking for create_bins does not work to ignore the 32 bit mode warning.
        if (dm.dmBitsPerPel < 32 && !game_state.create_bins)
            winMsgAlert(textStd("RunIn32BitorDie"));
    }

    //printf("resizing: %d, %d - %d, %d\n", x0, y0, width, height);
    window_style = WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP;
    if(!fullscreen)
        window_style |= WS_OVERLAPPEDWINDOW;
    SetWindowLongPtr(hwnd, GWL_STYLE, window_style);
    SetWindowPos( hwnd, HWND_TOP, x0, y0, width, height, SWP_NOZORDER | SWP_FRAMECHANGED );

    //WINDOWPLACEMENT wp;
    //wp.length = sizeof(wp);
    //GetWindowPlacement(hwnd, &wp);
    //printf("restored: %d, %d, %d, %d\n", wp.rcNormalPosition.left, wp.rcNormalPosition.top, wp.rcNormalPosition.right, wp.rcNormalPosition.bottom);

    if (!old_ignore)
    {
        windowProcessMessages();
        GetCursorPos(&initCursorPos);
        game_state.ignore_activity = 0;
        game_state.activity_detected = 0;
    }
}

// TOOD: Merge this with windowResize() after things are worked out
void windowResizeToggle(int *widthp,int *heightp,int *refreshRatep,int x0,int y0,int fullscreen, int old_fullscreen)
{
    int        old_ignore = game_state.ignore_activity;
    int        width,height,refreshRate;
    DEVMODE dm;
    DWORD    window_style;
    int useRefreshRate = 1;
    game_state.fullscreen = fullscreen;

    game_state.ignore_activity = 1;
    windowProcessMessages();

    PERFINFO_AUTO_START("winMakeCurrent", 1);
    winMakeCurrent();

    // free textures to free up video memory for
    // greater chance of success in transition.
    // @todo helpful or not?
    if (!game_state.noTexFreeOnResChange)
    {
        PERFINFO_AUTO_STOP_START("texFreeAll",1);
        texFreeAll();
    }

    rdrQueueFlush();
//    PERFINFO_AUTO_STOP_START("windowReleaseDisplayContexts",1);
    //windowReleaseDisplayContexts(); // This might not be needed?

    // currently the input params are tied directly to the game_state addresses that
    // will be modified by processing of WM_SIZE events that get generated by
    // the windows system calls below
    // store the old values before hand as a precaution (or adjust the calling paradigm)

    // This also implies when going windowed that we want to go with a windowed size
    // that matches the current full screen resolution we were coming from.
    width = *widthp;
    height = *heightp;
    refreshRate = *refreshRatep;
    if (refreshRate < 60)
        useRefreshRate = 0;

    if (old_fullscreen) {
        // restore desktop resolution

        // N.B. calls to ChangeDisplaySettings() will cause associated
        // window events like WM_SIZE to be placed in our event queue.
        // By default the handlers for these will then record the new resolution as the
        // client resolution and adjust dialog and in game window positions accordingly.

        // When returning to the default desktop resolution for windowed mode
        // we don't want the client to 'resize' to the native desktop resolution (which
        // may be large), to only then immediately be resized again when we size and
        // locate to the correct windowed extent. So, we disable wm_size message
        // processing around this call.


        PERFINFO_AUTO_STOP_START("ChangeDisplaySettings",1);
        s_ignore_wm_size = 1;
        ChangeDisplaySettings(NULL,0);
        s_ignore_wm_size = 0;
    }
    PERFINFO_AUTO_STOP_START("Misc",1);

    if (!fullscreen)
    {
        width += GetSystemMetrics(SM_CXSIZEFRAME) * 2;
        height += GetSystemMetrics(SM_CYSIZEFRAME) * 2 + GetSystemMetrics(SM_CYSIZE) + 1;
    }
    else
    {
        GfxResolution desktop;
        GfxResolution supported[MAX_SUPPORTED_GFXRESOLUTIONS] = {0};
        int supportedCount;
        int best=0;
        int bestv=1000000;

        GfxResolution * gr;
        int        i,okRes;

        winGetSupportedResolutions( &desktop, supported, &supportedCount );

        okRes = 0;
        for( i=0 ; i < supportedCount ; i++ )
        {
            int j;
            int delta;
            gr = &supported[i];
            delta = ABS(gr->x - width) + ABS(gr->y - height);
            if (delta  > bestv)
                continue;
            if (!eaiSize(&gr->refreshRates) || !useRefreshRate)
            {
                okRes = 1;
                useRefreshRate = 0;
                best = i;
                bestv = delta;
            }
            else
            {
                for (j = 0; j < eaiSize(&gr->refreshRates); j++)
                {
                    if (gr->refreshRates[j] == refreshRate) {
                        okRes = 1;
                        best = i;
                        bestv = delta;
                    }
                }
            }
        }

        if( !okRes )
        {
            winMsgAlert( textStd("ResolutionNotSupported", width, height, supported[0].x, supported[0].y ));
            *widthp = supported[0].x;
            *heightp = supported[0].y;
            *refreshRatep = 0;
            useRefreshRate = 0;
            {
                GfxSettings gfxSettings;
                gfxGetSettings( &gfxSettings );
                gfxApplySettings( &gfxSettings, 1, false );
            }
        } else {
            *widthp = supported[best].x;
            *heightp = supported[best].y;
        }

        width = *widthp;
        height = *heightp;
        refreshRate = *refreshRatep;

        x0 = 0;
        y0 = 0;
    }
    

    if (fullscreen)
    {
        int fullScreenResult, i;

        memset(&dm,0,sizeof(dm));
        dm.dmSize       = sizeof(dm);
        dm.dmPelsWidth  = width;
        dm.dmPelsHeight = height;
        dm.dmDisplayFrequency = refreshRate;
        dm.dmBitsPerPel = 32;//game_state.screendepth;
        dm.dmFields     = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL | ((useRefreshRate&&refreshRate)?DM_DISPLAYFREQUENCY:0);

        PERFINFO_AUTO_STOP_START("ChangeDisplaySettings",1);
        fullScreenResult = ChangeDisplaySettings( &dm, CDS_FULLSCREEN );

        if( fullScreenResult != DISP_CHANGE_SUCCESSFUL )
        {
            char * errormsg;
            typedef struct Errors { int errorCode; char msg[200]; } Errors;
            Errors errors[] = { /*weird that dualview isn't defined*/
                { -6 /*DISP_CHANGE_BADDUALVIEW*/,  "DualViewCapable"  },
                { DISP_CHANGE_BADFLAGS,     "BadFlags"  },
                { DISP_CHANGE_BADMODE,      "UnsupportedGraphics"  },
                { DISP_CHANGE_BADPARAM,     "BadFlagParam" }, 
                { DISP_CHANGE_FAILED,       "DriverFailed"  },
                { DISP_CHANGE_NOTUPDATED,   "RegistryFailed"  },
                { DISP_CHANGE_RESTART,      "RestartComputer"  },
                { 0, 0 },
            };

            errormsg = "UnknownFailure";
            for( i = 0; errors[i].errorCode != 0 ; i++ )
            {
                if( fullScreenResult == errors[i].errorCode ) 
                    errormsg = errors[i].msg;
            }
            winMsgAlert(textStd("FullscreenFailure", errormsg ));
            {
                GfxSettings gfxSettings;
                gfxGetSettings( &gfxSettings );
                gfxSettings.screenX = 800;
                gfxSettings.screenY = 600;
                gfxSettings.refreshRate = 60;
                gfxSettings.screenX_pos = 0;
                gfxSettings.screenX_pos = 0;
                gfxSettings.fullScreen  = 0;
                gfxApplySettings( &gfxSettings, 1, false );
            }    

            fullscreen = 0;
            width = 800;
            height = 600;
            *refreshRatep = 0;
            width += GetSystemMetrics(SM_CXSIZEFRAME) * 2;
            height += GetSystemMetrics(SM_CYSIZEFRAME) * 2 + GetSystemMetrics(SM_CYSIZE) + 1;
        } //end Failure to go to full screen
        else
        {
            *refreshRatep = refreshRate = dm.dmDisplayFrequency;
        }
    }
    
    if( !fullscreen )
    {
        PERFINFO_AUTO_STOP_START("EnumDisplaySettings",1);
        EnumDisplaySettings(NULL,ENUM_CURRENT_SETTINGS,&dm);
        // unfortunately, this is before command line parameters are parsed, 
        // so checking for create_bins does not work to ignore the 32 bit mode warning.
        if (dm.dmBitsPerPel < 32 && !game_state.create_bins)
            winMsgAlert(textStd("RunIn32BitorDie"));
    }

    PERFINFO_AUTO_STOP_START("SetWindow",1);
    //printf("resizing: %d, %d - %d, %d\n", x0, y0, width, height);
    window_style = WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP;
    if(!fullscreen)
        window_style |= WS_OVERLAPPEDWINDOW;
    SetWindowLongPtr(hwnd, GWL_STYLE, window_style);
    SetWindowPos( hwnd, HWND_TOP, x0, y0, width, height, SWP_NOZORDER | SWP_FRAMECHANGED );

    //WINDOWPLACEMENT wp;
    //wp.length = sizeof(wp);
    //GetWindowPlacement(hwnd, &wp);
    //printf("restored: %d, %d, %d, %d\n", wp.rcNormalPosition.left, wp.rcNormalPosition.top, wp.rcNormalPosition.right, wp.rcNormalPosition.bottom);

    PERFINFO_AUTO_STOP_START("windowShow",1);
    windowShow(fullscreen, game_state.maximized);

    PERFINFO_AUTO_STOP_START("windowReInitDisplayContexts",1);
    //windowReInitDisplayContexts(); // This might not be needed?

    PERFINFO_AUTO_STOP();

    if (!old_ignore)
    {
        windowProcessMessages();
        GetCursorPos(&initCursorPos);
        game_state.ignore_activity = 0;
        game_state.activity_detected = 0;
    }
}

void windowShow(int fullscreen,int maximize)
{
    if (g_show_splash)
        DestroySplash();
    ShowWindow( hwnd, !fullscreen && maximize ? SW_SHOWMAXIMIZED : SW_SHOWDEFAULT );
    UpdateWindow(hwnd);
    SetFocus (hwnd);
    SetForegroundWindow(hwnd);
    rdrNeedGammaReset();
}

// This version used by generic dialog code which requires void(void*) signature
void windowExitDlg(void * data)
{
    windowExit(0);
}

int window_no_exit = 0;
void windowExit(int exitCode)
{
    // special case for abnormal exit: dont cleanup as we may be on another thread and will crash for other reasons
    if(exitCode < 0) {
        exit(exitCode);
        return;
    }

    if (game_state.cryptic)
    {
//        saveOnExit();
        saveAutoResumeInfoCryptic();
    }

    demoStop();
    destroyCursorHashTable();
    saveAutoResumeInfoToRegistry();
    inpEnableAltTab();
    sndExit();
    hwlightShutdown();

    rdrAmbientDeInit();
    rdrRestoreGammaRamp();
    windowDestroyDisplayContexts();
    rdrQueueFlush();
    if (hwnd)
    {
        DestroyWindow( hwnd );
        hwnd = 0;
    }

    if(game_state.fullscreen)
        ChangeDisplaySettings(NULL,0);
    ShowCursor (TRUE);
    commDisconnect();
    authLogout();
    dbSendQuit();
    logShutdownAndWait();
    timeEndPeriod(1);
    if (isDevelopmentMode()) {
        assert(heapValidateAll());
    }
    if (game_state.exit_launch[0])
        system_detach((char*)unescapeString(game_state.exit_launch), 0);
    if (!window_no_exit)
        exit(exitCode);
}

volatile int frames_buffered;
HANDLE    frame_done_signal;

void windowUpdate()
{
    int        first=1;

    if (!frame_done_signal)
        frame_done_signal = CreateEvent(0,0,0,0);

    PERFINFO_AUTO_START("while(frames_buffered)",1);
    while(frames_buffered > game_state.allow_frames_buffered)
    {
        rdrQueueMonitor();
        WaitForSingleObject( frame_done_signal, 10 );
        if (!first && frames_buffered)
            Sleep(1);
        first = 0;
    }
    PERFINFO_AUTO_STOP();

    PERFINFO_AUTO_START("queue_swap",1);
    InterlockedIncrement(&frames_buffered);

    rdrQueueCmd(DRAWCMD_SWAPBUFFER);
    PERFINFO_AUTO_STOP();                
}

void windowProcessMessages()
{
    MSG msg;
    int    quit=0;

    //ab: set the wndproc again if doing EnC work
//     {
//         static void *cbPtr = MainWndProcCallback;

//         if( cbPtr != MainWndProcCallback )
//         {
//             SetWindowLongPtr(hwnd, 
//                              GWLP_WNDPROC, 
//                              (LONG_PTR)MainWndProcCallback);
//             cbPtr = MainWndProcCallback;
//         }
//     }
    

    processing_message = 1;
    while( PeekMessageA( &msg, 0, 0, 0, PM_REMOVE ) )
    {
        TranslateMessage( &msg );
        DispatchMessageA( &msg );
        if(0 && msg.message == WM_QUIT)
            quit = 1;
    }
    if (quit)
        windowExit(0);
    processing_message = 0;
}

int g_win_ignore_popups = 0;    // sometimes used by demo playback (at least in development mode)

typedef struct WinError
{
    int highlight;
    char *str, *title, *fault;
} WinError;

enum {
    WINDIALOG_ERROR=WT_CMD_USER_START,
    WINDIALOG_MSG,
} win_error_type;

static void winErrorQueueDialog(int type, char *str, char* title, char* fault, int highlight)
{
    WinError win_error = {0};
    win_error.str = str?strdup(str):0;
    win_error.title = title?strdup(title):0;
    win_error.fault = fault?strdup(fault):0;
    win_error.highlight = highlight;
    wtQueueMsg(render_thread, type, &win_error, sizeof(win_error));
}

void winErrorDispatch(void *unused, int type, void *data)
{
    WinError *error = data;
    switch (type)
    {
        xcase WINDIALOG_ERROR:
            winErrorDialog(error->str, error->title, error->fault, error->highlight);
        xcase WINDIALOG_MSG:
            winMsgAlert(error->str);

        xdefault:
            assert(0);
    }

    SAFE_FREE(error->str);
    SAFE_FREE(error->title);
    SAFE_FREE(error->fault);
}

void winRenderStall(void)
{
    failText("Render queue stalling");
}

void winErrorDialog(char *str, char* title, char* fault, int highlight)
{
    if (rdrInThread()) {
        winErrorQueueDialog(WINDIALOG_ERROR, str, title, fault, highlight);
        wtFlushMessages(render_thread);
        return;
    }

    // Hack for holding Shift to ignore all pop-ups.  Shhh... don't tell anyone.
    if (g_win_ignore_popups || game_state.imageServer || GetAsyncKeyState(VK_SHIFT) & 0x8000000) {
        printf("ERROR: %s\n", str);
        return;
    }

    errorDialog(hwnd, str, title, fault, highlight);

    inpClear();
}

void winMsgAlert(char *str)
{
    if (rdrInThread()) {
        winErrorQueueDialog(WINDIALOG_MSG, str, "City of Heroes Alert", NULL, 0);
        wtFlushMessages(render_thread);
        return;
    }

    if (g_win_ignore_popups) {
        printf("winMsgAlert: %s\n", str);
        return;
    }

    msgAlert(hwnd, str);

    inpClear();
}

int winMsgOkCancelParented(HWND parent, char *str)
{
    int retval = 0;

    if (isGuiDisabled())
        printf("winMsgOkCancel: %s\n", str);
    else if (MessageBoxA(parent, str, "City of Heroes Prompt", MB_OKCANCEL) == IDOK)
        retval = 1;

    // JS:    DX mouse buffered input loses data in foreground mode as soon as the message box
    //        comes up.  The game will lose the mouse-up message.  Subsequent mouse commands
    //        will not be processed correctly.
    inpClear();

    return retval;
}

int winMsgOkCancel(char *str)
{
    return winMsgOkCancelParented(NULL, str);
}

int winMsgYesNoParented(HWND parent, char *str)
{
    int retval = 0;

    if (isGuiDisabled())
        printf("winMsgYesNo: %s\n", str);
    else if (MessageBoxA(parent, str, "City of Heroes Prompt", MB_YESNO | MB_ICONQUESTION) == IDYES)
        retval = 1;

    // JS:    DX mouse buffered input loses data in foreground mode as soon as the message box
    //        comes up.  The game will lose the mouse-up message.  Subsequent mouse commands
    //        will not be processed correctly.
    inpClear();

    return retval;
}

int winMsgYesNo(char *str)
{
    return winMsgYesNoParented(NULL, str);
}

void winMsgError(char *str)
{
    if (isGuiDisabled())
    {
        printf("winMsgError: %s\n", str);
        return;
    }

    MessageBoxA( 0, str, "City of Heroes Prompt", MB_OK | MB_ICONERROR);

    // JS:    DX mouse buffered input loses data in foreground mode as soon as the message box
    //        comes up.  The game will lose the mouse-up message.  Subsequent mouse commands
    //        will not be processed correctly.
    inpClear();
}


typedef struct TextDialogParams {
    wchar_t prompt[TEXT_DIALOG_MAX_STRLEN];
    wchar_t result[TEXT_DIALOG_MAX_STRLEN];
} TextDialogParams;

typedef struct TextDialogEStrings {
    wchar_t *prompt;
    wchar_t *result;
} TextDialogEStrings;

// TODO - have the call set up this string list and make it
// more context sensitive?
char *EnterTextStrings[] =
{ 
//"ActivateScenarios", this is ancient history
"Generator",
"GeneratorGroup",
"SpawnArea",
"ThreatMax",
"ThreatMin",
"Untracked",
"Door",
"DoorInfo",
"EncounterGroup",
"EncounterSpawn",
"EncounterPosition",
"CutSceneCamera",
"SpawnProbability",
"CanSpawn1",            //1, 2, 3, 4...
"EncounterEnableStart",
"EncounterEnableDuration",
"PersistentNPC",
"SpawnLocation",
"LocationName",
"LairType",
"RadiusOverride",
"ItemGroup",
"MissionObjective",
"MissionEnd",
"MissionStart",
"ItemPosition",
"MissionRoom",
"ScriptMarker",
"PersonObjective",
"EncounterAutospawnEnd",
"EncounterAutospawnStart",
"VillainRadius",
"SingleUse",
"GroupName",
"DontSubdivide",
"HonorGrouping",
"ManualSpawn",
"SquadRenumber",
"AutoSpawn",
"EmergeAnimationBits",
"EmergeFromMe",
"EnterAnimationBits",
"EnterMe",
"ExitPoint",

"Breakable",
"Global",
"Plaque",
"dbSrvrTracked",
"NHoodName",
"IndoorLighting",
"NamedVolume",
"Kiosk",
"ArenaKiosk",
"VisitLocation",
"MarkerName",
"MarkerAI",
"BeaconGroup",
"NPCNoAutoConnect",
"TypeNumber",
"KillerBeacon",
"EntryPoint",
"Radius",
"NoGroundConnections",
"OverrideEntryMin",
"OverrideEntryMax",
"MonorailLine",

//Auto Map 
"Icon",
"GotoSpawn",
"GotoMap",
"ExitToSpawn",
"ExitToMap",
"hiddenInFog",
"specialMapIcon",
"miniMapLocation",
"extentsMarkerGroup",

"Layer",
"AutoGroup",

"TexWordLayout",
"TexWordParam1",
"TexWordParam2",
"TexWordParam3",
"TexWordParam4",

"Health",        //signifies a destroyable object
"Repairing",    //indicates that this geometry will be drawn when in the repair state
"MaxHealth",    //max health the geometry is drawn at
"MinHealth",    //min health the geometry is drawn at
"Effect",        //an effect that will play as soon as the geometry begins being shown

"MinimapTextureName",    //name of the minimap texture associated with this group
"IgnoreRadius"
"VolumeTriggered"
//Encounter Position flags
"StealthCameraMovement",
"StealthCameraArc",
"StealthCameraSpeed",

"Interact",

"VisTray",
"VisOutside",
"VisOutsideOnly",
"SoundOccluder",

"DoWelding",

"Requires1",
"AllowThrough1",
"LockedText1",
"Requires2",
"AllowThrough2",
"LockedText2",
"Requires3",
"AllowThrough3",
"LockedText3",
"Requires4",
"AllowThrough4",
"LockedText4",

""
};

LRESULT CALLBACK EnterTextDialog(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp)
{
    static TextDialogParams* etd;
    RECT rc;
    int i;
    switch (msg)
    {
    case WM_INITDIALOG:
        {
            if (!lp) { EndDialog(hDlg, 0); return 0; }
            etd = (TextDialogParams*)lp;

            // set up the drop-down
            GetClientRect(GetDlgItem(hDlg, IDC_EDIT), &rc);
            MapWindowPoints(GetDlgItem(hDlg, IDC_EDIT), hDlg, (LPPOINT)&rc, 2);
            MoveWindow(GetDlgItem(hDlg, IDC_EDIT), rc.left, rc.top, rc.right-rc.left, 200, FALSE);
            for (i = 0; EnterTextStrings[i][0]; i++)
                SendMessageA(GetDlgItem(hDlg, IDC_EDIT), CB_ADDSTRING, 0, (LPARAM)EnterTextStrings[i]);
            //ActivateScenarios;Generator;Generator Group;MarkerAI;MarkerItem;Marke

            if (etd->prompt && etd->prompt[0])
                SetWindowTextW(GetDlgItem(hDlg, IDC_HELPTEXT), etd->prompt);
            if (etd->result && etd->result[0])
                SetWindowTextW(GetDlgItem(hDlg, IDC_EDIT), etd->result);
            return 1;
        }
    case WM_COMMAND:
        {
            if (LOWORD(wp) == IDCANCEL)
            {
                EndDialog(hDlg, IDCANCEL);
                return 1;
            }
            else if (LOWORD(wp) == IDOK)
            {
                GetWindowTextW(GetDlgItem(hDlg, IDC_EDIT), etd->result, TEXT_DIALOG_MAX_STRLEN); 
                EndDialog(hDlg, IDOK);
                return 1;
            }
        }
    }
    return 0;
}

LRESULT CALLBACK EnterTextDialogViaEString(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp)
{
    static TextDialogEStrings* etd;
    RECT rc;
    int i;
    switch (msg)
    {
    case WM_INITDIALOG:
        {
            if (!lp) { EndDialog(hDlg, 0); return 0; }
            etd = (TextDialogEStrings*)lp;

            // set up the drop-down
            GetClientRect(GetDlgItem(hDlg, IDC_EDIT), &rc);
            MapWindowPoints(GetDlgItem(hDlg, IDC_EDIT), hDlg, (LPPOINT)&rc, 2);
            MoveWindow(GetDlgItem(hDlg, IDC_EDIT), rc.left, rc.top, rc.right-rc.left, 200, FALSE);
            for (i = 0; EnterTextStrings[i][0]; i++)
                SendMessageA(GetDlgItem(hDlg, IDC_EDIT), CB_ADDSTRING, 0, (LPARAM)EnterTextStrings[i]);
            //ActivateScenarios;Generator;Generator Group;MarkerAI;MarkerItem;Marke

            if (etd->prompt && etd->prompt[0])
                SetWindowTextW(GetDlgItem(hDlg, IDC_HELPTEXT), etd->prompt);
            if (etd->result && etd->result[0])
                SetWindowTextW(GetDlgItem(hDlg, IDC_EDIT), etd->result);
            return 1;
        }
    case WM_COMMAND:
        {
            if (LOWORD(wp) == IDCANCEL)
            {
                EndDialog(hDlg, IDCANCEL);
                return 1;
            }
            else if (LOWORD(wp) == IDOK)
            {
                int len = GetWindowTextLength(GetDlgItem(hDlg, IDC_EDIT));
                estrWideReserveCapacity(&etd->result, len + 1);
                GetWindowTextW(GetDlgItem(hDlg, IDC_EDIT), etd->result, len + 1); 
                EndDialog(hDlg, IDOK);
                return 1;
            }
        }
    }
    return 0;
}

// NOTE: caller must allocate result buffer size to TEXT_DIALOG_MAX_STRLEN
bool winGetString(char* prompt, char* result)
{
    TextDialogParams etd = {0};
    int length;
    bool bClickedOk;

    // convert to wide
    UTF8ToWideStrConvert(prompt, etd.prompt, TEXT_DIALOG_MAX_STRLEN);
    UTF8ToWideStrConvert(result, etd.result, TEXT_DIALOG_MAX_STRLEN);

    // call wide dialog
    bClickedOk = (DialogBoxParamW(glob_hinstance, 
        MAKEINTRESOURCEW(IDD_ENTERTEXT), 
        hwnd, 
        (DLGPROC)EnterTextDialog, 
        (LPARAM)&etd) == IDOK);

    if(bClickedOk) {
        // Convert back from wide to UTF8
        strcpy(result, WideToUTF8StrTempConvert( etd.result , &length));
    }

    inpClear();

    return bClickedOk;
}

bool winGetEString(char *prompt, char **result)
{
    TextDialogEStrings etd = {0};
    bool bClickedOk;

    // convert to wide
    estrConvertToWide(&etd.prompt, prompt);
    estrConvertToWide(&etd.result, *result);

    // call wide dialog
    bClickedOk = (DialogBoxParamW(glob_hinstance, 
                                    MAKEINTRESOURCEW(IDD_ENTERTEXT), 
                                    hwnd, 
                                    (DLGPROC)EnterTextDialogViaEString, 
                                    (LPARAM)&etd)
                    == IDOK);

    if (bClickedOk)
    {
        // Convert back from wide to UTF8
        estrConvertToNarrow(result, etd.result);
    }

    inpClear();

    return bClickedOk;
}

char *winGetFileName(char *fileMask,char *fileName,int save)
{
    OPENFILENAMEA theFileInfo;
    //char filterStrs[256];
    int        ret;
    char    base[_MAX_PATH];

    _getcwd(base,_MAX_PATH);
    memset(&theFileInfo,0,sizeof(theFileInfo));
    theFileInfo.lStructSize = sizeof(OPENFILENAMEA);
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

    inpClear();
    if (ret)
        return fileName;
    else
        return NULL;
}

char *winGetFolderName(char *dirname,char *title)
{
    BROWSEINFOA bi;
    LPITEMIDLIST lpidl;
    char * ret=0;
    memset(&bi,0,sizeof(bi));
    bi.lpszTitle=title;
    lpidl=SHBrowseForFolderA(&bi);
    if (lpidl==NULL)
        return NULL;
    SHGetPathFromIDListA(lpidl,dirname);
    return dirname;
}

int winMaximized()
{
    return IsZoomed(hwnd);
}

void winSetText(char *s)
{
    wchar_t wbuffer[1024];

    if(!s || !s[0] )
        return;

    UTF8ToWideStrConvert(s, wbuffer, ARRAY_SIZE(wbuffer));
    SetWindowTextW(hwnd,wbuffer);
}

void winRegisterClass(HINSTANCE hInstance)
{
    wc.cbSize         = sizeof(wc);
    wc.style         = CS_OWNDC | CS_DBLCLKS;
    wc.lpfnWndProc   = (WNDPROC)MainWndProcCallback;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance;
    if ( locGetIDInRegistry() == locGetIDByWindowsLocale(LOCALE_KOREAN) )
    {
        wc.hIcon = LoadImageA(hInstance, MAKEINTRESOURCEA(IDI_KOREAN), IMAGE_ICON, 
            GetSystemMetrics(SM_CXICON), GetSystemMetrics(SM_CYICON), 0);
        wc.hIconSm = LoadImageA(hInstance, MAKEINTRESOURCEA(IDI_KOREAN), IMAGE_ICON,
            GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON), 0);
    }
    else if (game_state.skin == UISKIN_PRAETORIANS)
    {
        wc.hIcon = LoadImageA(hInstance, MAKEINTRESOURCEA(IDI_ROGUE), IMAGE_ICON, 
            GetSystemMetrics(SM_CXICON), GetSystemMetrics(SM_CYICON), 0);
        wc.hIconSm = LoadImageA(hInstance, MAKEINTRESOURCEA(IDI_ROGUE), IMAGE_ICON,
            GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON), 0);
    }
    else if (game_state.skin == UISKIN_VILLAINS)
    {
        wc.hIcon = LoadImageA(hInstance, MAKEINTRESOURCEA(IDI_COV), IMAGE_ICON, 
            GetSystemMetrics(SM_CXICON), GetSystemMetrics(SM_CYICON), 0);
        wc.hIconSm = LoadImageA(hInstance, MAKEINTRESOURCEA(IDI_COV), IMAGE_ICON,
            GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON), 0);
    }
    else
    {
        wc.hIcon = LoadImageA(hInstance, MAKEINTRESOURCEA(IDI_OUROBOROS), IMAGE_ICON, 
            GetSystemMetrics(SM_CXICON), GetSystemMetrics(SM_CYICON), 0);
        wc.hIconSm = LoadImageA(hInstance, MAKEINTRESOURCEA(IDI_OUROBOROS), IMAGE_ICON,
            GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON), 0);
    }
    wc.hCursor       = 0;//LoadCursor( NULL, IDC_ARROW );
    wc.hbrBackground = CreateSolidBrush(RGB(0,0,0));
    wc.lpszMenuName  = windowName;
    wc.lpszClassName = className;
    glob_hinstance = hInstance;

    if ( !RegisterClassExA( &wc ) ) {
        MessageBoxA( 0, "Couldn't Register Window Class", windowName, MB_ICONERROR );
    }

}

int    main();

void game_beforeRegisterWinClass(int argc, char **argv);

HINSTANCE glob_hinstance;

int WINAPI WinMain ( HINSTANCE hInstance, HINSTANCE hPrevInstance, 
                     LPSTR lpCmdLine, int nCmdShow )
{
    char    *argv[1000],*s,delim[] = " \t",buf[1000] = "",line2[1000]="";
    int        argc;
    int        noExceptionHandler=0;
    FILE    *file;

    memCheckInit();

    argv[0] = windowName;
    file = fopen("cmdline.txt","rt");
    if (file)
    {
        fgets(buf,sizeof(buf),file);
        s = buf + strlen(buf) - 1;
        if (*s == '\n')
            *s = 0;
        fgets(line2,sizeof(line2),file);
        if (strnicmp(line2, "noExceptionHandler", strlen("noExceptionHandler"))==0)
        {
            noExceptionHandler=1;
        }
        fclose(file);
    }
    strcat(buf," ");
    strcat(buf,lpCmdLine);
#if 1
    argc = 1 + tokenize_line_quoted_safe(buf,&argv[1],ARRAY_SIZE(argv)-1,0);
#else
    for(s = strtok(buf,delim),argc=1;s && argc<ARRAY_SIZE(argv);argc++,s = strtok(0,delim))
        argv[argc] = s;
#endif

#if 0 // for attaching remote debugger at launch
    while(!GetAsyncKeyState(VK_SHIFT) )
        ; // wait for debugger to attach
#endif

    game_beforeRegisterWinClass(argc, argv);

    winRegisterClass(hInstance);

    if (noExceptionHandler) {
        return main(argc,argv);
    } else {
        EXCEPTION_HANDLER_BEGIN
        return main(argc,argv);
        EXCEPTION_HANDLER_END
    }
    return -1;
}

void rdrSetGamma( float Gamma )
{
    // Don't set the gamma if we don't have focus
    if (game_state.inactiveDisplay)
    {
        s_saved_gamma = Gamma;
        return;
    }


    if (render_thread)
        rdrQueue(DRAWCMD_SETGAMMA, &Gamma, sizeof(float));
}

void rdrNeedGammaReset(void)
{
    if (render_thread)
        rdrQueueCmd(DRAWCMD_NEEDGAMMARESET);

    // Apply any saved gamma
    if (s_saved_gamma != -1.0f)
    {
        rdrSetGamma(s_saved_gamma);
        s_saved_gamma = -1.0f;
    }
}

void rdrRestoreGammaRamp(void)
{
    if (render_thread)
        rdrQueueCmd(DRAWCMD_RESTOREGAMMA);
}

void windowDestroyDisplayContexts(void)
{
    if (render_thread)
        rdrQueueCmd(DRAWCMD_DESTROYDISPLAYCONTEXTS);
}

void windowReleaseDisplayContexts(void)
{
    if (render_thread)
        rdrQueueCmd(DRAWCMD_RELEASEDISPLAYCONTEXTS);
}

void windowReInitDisplayContexts(void)
{
    if (render_thread)
        rdrQueueCmd(DRAWCMD_REINITDISPLAYCONTEXTS);
}

void rdrSetVSync(int enable)
{
    if (render_thread)
        rdrQueue(DRAWCMD_SETVSYNC, &enable, sizeof(int));
}





