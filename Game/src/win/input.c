#include <utilitieslib/stdtypes.h>
#include <utilitieslib/utils/wininclude.h>
#include "graphics/font.h" // temporary
#include "win/input.h"
#include "win/win_init.h"
#include <ctype.h>
#include <utilitieslib/utils/timing.h>
#include "UI/uiConsole.h"
#include "cmdparse/cmdgame.h"
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/memcheck.h>
#include <utilitieslib/utils/mathutil.h>
#include "winuser.h"
#include <utilitieslib/assert/assert.h>
#include "UI/uiInput.h"
#include "UI/uiChat.h"
#include "UI/uiCursor.h"
#include "UI/uiWindows.h"
#include "UI/uiKeybind.h"
#include "UI/uiFocus.h"
#include "UI/sprite/sprite_text.h"
#include <windows.h>
#include <basetsd.h>
#include "UI/uiGame.h"
#include <utilitieslib/components/StashTable.h>
#include <utilitieslib/language/MessageStoreUtil.h>
#include "UI/uiOptions.h"

#pragma warning (disable:4996)

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

char    inp_levels[256], inp_edges[256];
KeyInput textKeyboardBuffer[64];
U32 gDoubleClickTime = 0;

int vkMouseButtons[] = {VK_LBUTTON, VK_MBUTTON, VK_RBUTTON};
int dxMouseButtons[] = {INP_LBUTTON, INP_MBUTTON, INP_RBUTTON};
int mouse_inp_edges[3];
int mouse_inp_levels[3];

int        mouse_x,mouse_y,mouse_dx,mouse_dy,mouse_lock,mtdx,mtdy,mouse_oldx,mouse_oldy;
int        mouse_lastx,mouse_lasty;

int altKeyState = 0;
int ctrlKeyState = 0;
int shiftKeyState = 0;

LPDIRECTINPUT8            DirectInput8; 
LPDIRECTINPUTDEVICE8    Keyboard;
LPDIRECTINPUTDEVICE8    gMouse;
LPDIRECTINPUTDEVICE8    gJoystick;

#define KEYBOARD_BUFFER_SIZE 128
#define MOUSE_BUFFER_SIZE  256
DIPROPDWORD dpd;

static void inpUpdateKeyClearIfActive(int keyIndex, int timeStamp);
int GetScancodeFromVirtualKey(WPARAM wParam, LPARAM lParam);


void KeyboardShutdown(){
    if(Keyboard){ 
        Keyboard->lpVtbl->Unacquire(Keyboard); 
        Keyboard->lpVtbl->Release(Keyboard);
        Keyboard = NULL; 
    } 
}

int inpKeyboardAcquire(){
    HRESULT hr;

    if(Keyboard){
        hr = Keyboard->lpVtbl->Acquire(Keyboard); 
        if(FAILED(hr)){
            return 0;
        }

        return 1;
    }

    return 0;
}

int KeyboardStartup(){
    HRESULT               hr; 
    
    hr = DirectInput8->lpVtbl->CreateDevice(DirectInput8, &GUID_SysKeyboard, &Keyboard, NULL); 
    if(FAILED(hr))
        goto KeyboardStartupError;


    hr = Keyboard->lpVtbl->SetDataFormat(Keyboard, &c_dfDIKeyboard);  
    if(FAILED(hr))
        goto KeyboardStartupError;

    // Set the cooperative level.
    //    By default:
    //        1.  The input system stops receiving keyboard input when the game window lose focus.
    //        2.  The input system allows windows messages for the keyboard to be generated.
    //        3.  The input system disables the windows key. (Cannot escape app using the windows key)
    hr = Keyboard->lpVtbl->SetCooperativeLevel(Keyboard, hwnd, 
        DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);// | DISCL_NOWINKEY);
    if(FAILED(hr))
        goto KeyboardStartupError;

    // Put the DirectInput keyboard into buffered mode.
    {
        DIPROPDWORD propWord;

        // JS:    This is totally dumb.  Why do I have to prepare this header for DirectInput
        //        if it *must* be these values when I am setting the buffer size property?
        propWord.diph.dwSize =  sizeof(DIPROPDWORD);
        propWord.diph.dwHeaderSize = sizeof(DIPROPHEADER);
        propWord.diph.dwObj = 0;
        propWord.diph.dwHow = DIPH_DEVICE;
        propWord.dwData = KEYBOARD_BUFFER_SIZE;

        Keyboard->lpVtbl->SetProperty(Keyboard, DIPROP_BUFFERSIZE, &propWord.diph);
    }
    inpKeyboardAcquire();

    return 0;
KeyboardStartupError:
    KeyboardShutdown();
    return 1;
}

static int inpMouseAcquire( void )
{
    if(gMouse)
    {
        HRESULT hr = Keyboard->lpVtbl->Acquire(gMouse); 
        if( SUCCEEDED(hr))
            return 1;
    }

    return 0;
}

int MouseStartup( void )
{
    HRESULT               hr; 
    
    hr = DirectInput8->lpVtbl->CreateDevice(DirectInput8, &GUID_SysMouse, &gMouse, NULL); 
    if( FAILED( hr ) )
        return 1;

    hr = gMouse->lpVtbl->SetDataFormat( gMouse, &c_dfDIMouse2 );  
    if( FAILED( hr ) )
        return 1;

    hr = gMouse->lpVtbl->SetCooperativeLevel( gMouse, hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE );
    if( FAILED(hr) )
        return 1;

    dpd.diph.dwSize       = sizeof(DIPROPDWORD);
    dpd.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    dpd.diph.dwObj        = 0;
    dpd.diph.dwHow        = DIPH_DEVICE;

    // the data
    dpd.dwData            = MOUSE_BUFFER_SIZE;

    hr = gMouse->lpVtbl->SetProperty( gMouse, DIPROP_BUFFERSIZE, &dpd.diph);
 
    if ( FAILED(hr) ) 
         return 1;

    inpMouseAcquire();

    {
        int x = 0, y = 0;
        inpMousePos( &x, &y );

        gMouseInpCur.x = x;
        gMouseInpCur.y = y;
    }

    return 0;
}

void MouseClearButtonState(void)
{
    gMouseInpCur.left = gMouseInpCur.mid = gMouseInpCur.right = MS_NONE;
}

void MouseShutdown( void )
{
    if(gMouse)
    { 
        gMouse->lpVtbl->Unacquire( gMouse ); 
        gMouse->lpVtbl->Release( gMouse );
        gMouse = NULL; 
    } 
}

BOOL CALLBACK EnumJoysticksCallback( const DIDEVICEINSTANCE* pdidInstance,    VOID* pContext )
{
    HRESULT hr;

    // Obtain an interface to the enumerated joystick.
    hr = DirectInput8->lpVtbl->CreateDevice( DirectInput8, &pdidInstance->guidInstance, &gJoystick, NULL );

    // If it failed, then we can't use this joystick. (Maybe the user unplugged
    // it while we were in the middle of enumerating it.)
    if( FAILED(hr) ) 
        return DIENUM_CONTINUE;

    // Stop enumeration. Note: we're just taking the first joystick we get. You
    // could store all the enumerated joysticks and let the user pick.
    return DIENUM_STOP;
}

BOOL CALLBACK EnumObjectsCallback( const DIDEVICEOBJECTINSTANCE* pdidoi,  VOID* pContext )
{
    HWND hDlg = (HWND)pContext;

    static int nSliderCount = 0;  // Number of returned slider controls
    static int nPOVCount = 0;     // Number of returned POV controls

    // For axes that are returned, set the DIPROP_RANGE property for the
    // enumerated axis in order to scale min/max values.
    if( pdidoi->dwType & DIDFT_AXIS )
    {
        DIPROPRANGE diprg; 
        diprg.diph.dwSize       = sizeof(DIPROPRANGE); 
        diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER); 
        diprg.diph.dwHow        = DIPH_BYID; 
        diprg.diph.dwObj        = pdidoi->dwType; // Specify the enumerated axis
        diprg.lMin              = -1000; 
        diprg.lMax              = +1000; 

        // Set the range for the axis
        if( FAILED( gJoystick->lpVtbl->SetProperty( gJoystick, DIPROP_RANGE, &diprg.diph ) ) ) 
            return DIENUM_STOP;

    }

    return DIENUM_CONTINUE;
}

int JoystickStartup()
{
    HRESULT hr;

    // Look for a simple joystick we can use for this sample program.
    if( FAILED( hr = DirectInput8->lpVtbl->EnumDevices( DirectInput8, DI8DEVCLASS_GAMECTRL, EnumJoysticksCallback, NULL, DIEDFL_ATTACHEDONLY ) ) )
        return 1;

    if( !gJoystick )
        return 1;

    // Set the data format to "simple joystick" - a predefined data format 
    //
    // A data format specifies which controls on a device we are interested in,
    // and how they should be reported. This tells DInput that we will be
    // passing a DIJOYSTATE2 structure to IDirectInputDevice::GetDeviceState().
    if( FAILED( hr = gJoystick->lpVtbl->SetDataFormat( gJoystick, &c_dfDIJoystick2 ) ) )
        return 1;

    // Set the cooperative level to let DInput know how this device should
    // interact with the system and with other DInput applications.
    if( FAILED( hr = gJoystick->lpVtbl->SetCooperativeLevel( gJoystick, hwnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND ) ) )
        return 1;

    // Enumerate the joystick objects. The callback function enabled user
    // interface elements for objects that are found, and sets the min/max
    // values property for discovered axes.
    if( FAILED( hr = gJoystick->lpVtbl->EnumObjects( gJoystick, EnumObjectsCallback, (VOID*)hwnd, DIDFT_ALL ) ) )
        return 1;

    return 0;
}

void JoystickShutdown()
{
    // Unacquire the device one last time just in case 
    // the app tried to exit while the device is still acquired.
    if( gJoystick ) 
        gJoystick->lpVtbl->Unacquire( gJoystick );

    // Release any DirectInput objects.
    if( gJoystick )
    {
        gJoystick->lpVtbl->Release( gJoystick );
        gJoystick = NULL;
    }
}


// disable or enable joystick, if necessary
void JoystickEnable()
{
    if(!gJoystick && optionGet(kUO_EnableJoystick))
    {
        JoystickStartup();
    }
    else if(gJoystick && !optionGet(kUO_EnableJoystick) )
    {
        JoystickShutdown();
    }
}


int InputStartup()
{
    HRESULT hr;
    // Initialize DirectInput8.
    hr = DirectInput8Create(glob_hinstance, DIRECTINPUT_VERSION, &IID_IDirectInput8, (void**)&DirectInput8, NULL);

    if(FAILED(hr)) 
        goto fail;

    // If both the keyboard and the mouse starts up successfully,
    // the input system started up correctly.
    if( !KeyboardStartup() && !MouseStartup() )
    {
        // don't really care if joystick fails
        if(optionGet(kUO_EnableJoystick))
            JoystickStartup();

        return 0;
    }
    
    // Otherwise, the input system was not started up correctly.
    // Shutdown to release all acquired resources.
    printf("Input system failed to initialize\n");
    InputShutdown();
fail:
    winMsgAlert(textStd("GetDirectX"));
    return 1;
}
 
 
void InputShutdown()
{
    KeyboardShutdown();
    MouseShutdown();
    JoystickShutdown();

    if (DirectInput8){ 
        DirectInput8->lpVtbl->Release(DirectInput8);
        DirectInput8 = NULL; 
    }
}

int inpLevel(int idx)
{
    switch(idx){
    case INP_CONTROL:
        return inp_levels[INP_RCONTROL] | inp_levels[INP_LCONTROL];
    case INP_SHIFT:
        return inp_levels[INP_RSHIFT] | inp_levels[INP_LSHIFT];
    case INP_ALT:
        return inp_levels[INP_RMENU] | inp_levels[INP_LMENU];
    default:
        return inp_levels[idx];
    }
}

int inpEdge(int idx)
{
    switch(idx){
    case INP_CONTROL:
        return inp_edges[INP_RCONTROL] | inp_edges[INP_LCONTROL];
    case INP_SHIFT:
        return inp_edges[INP_RSHIFT] | inp_edges[INP_LSHIFT];
    case INP_ALT:
        return inp_edges[INP_RMENU] | inp_edges[INP_LMENU];
    default:
        return inp_edges[idx];
    }
}

bool inpEdgeNoTextEntry(int idx)
{
    return (uiNoOneHasFocus() && inpEdge(idx));
} 

bool inpEdgeThisFrame(void)
{
    int i;

    for( i = 0; i < 256; i++ )
    {
        if( inp_edges[i])
            return true;
    }

    return false;
}

void inpKeyAddBuf(KeyInputType type, int c, int lparam,  KeyInputAttrib attrib)
{
    int len;

    // Please don't try to trick the code into adding an invalid entry
    // into the keyboard buffer.
    assert(KIT_None != type);

    // throw away systen messages 'ctrl + anything'
    if( c < 32 && type != KIT_EditKey)
    {
        c = GetScancodeFromVirtualKey(c, lparam);
        type = KIT_EditKey;
    }

    // High quality solutions to finding the end of array.
    for(len = 0; len < ARRAY_SIZE(textKeyboardBuffer); len++){
        if(KIT_None == textKeyboardBuffer[len].type)
            break;
    }

    if (len > ARRAY_SIZE(textKeyboardBuffer) - 1){
        printf("Text keyboard buffer overflow!");
        return;
    }

    textKeyboardBuffer[len].type = type;
    textKeyboardBuffer[len].key = c;
    textKeyboardBuffer[len].attrib = attrib;

    if(len + 1 < ARRAY_SIZE(textKeyboardBuffer))
        textKeyboardBuffer[len+1].type = KIT_None;
}

KeyInput* inpGetKeyBuf(){
    if(KIT_None == textKeyboardBuffer[0].type)
        return NULL;
    return textKeyboardBuffer;
}

void inpGetNextKey(KeyInput** input){
    
    // Invalid parameters.
    if(!input || !*input)
        return;

    // Range check.  Is the caller trying to read a random piece of memory as a KeyInput?
    assert(*input >= textKeyboardBuffer && *input <= (textKeyboardBuffer + ARRAY_SIZE(textKeyboardBuffer)));
    
    // If the caller is already pointing to the last element (very unlikely), reply that there are no more inputs.
    if(*input >= textKeyboardBuffer + ARRAY_SIZE(textKeyboardBuffer) - 1){
        *input = NULL;
        return;
    }
    
    // If there are no more valid inputs, say so.
    if(KIT_None == ((*input)+1)->type){
        *input = NULL;
        return;
    }

    (*input)++;
}


void inpLockMouse(int lock)
{
      if (mouse_lock == lock )
         return;

       if (lock)
    {
        //xyprintf(10, 10, "mouse locked");
        mouse_oldx = mouse_x;
        mouse_oldy = mouse_y;
    }
    else
    {
          mouse_x = mouse_oldx;
        mouse_y = mouse_oldy;

         mouse_lastx = mouse_x;
        mouse_lasty = mouse_y;
         SetCursorPos(mouse_x,mouse_y);
        mouseClear();
    }

    mouse_lock = lock;
    ShowCursor(!lock );
}

int inpIsMouseLocked()
{
    return mouse_lock;
}

bool inpMousePosOnScreen(int x, int y)
{
    int        width, height;
    POINT    pClient = {0, 0};

    windowSize(&width,&height);

     if (!gWindowDragging && (x < 0 || y < 0 || x >= width || y >= height) )
        return false;

    return true;
}

int inpMousePos(int *xp,int *yp)
{
    POINT    pCursor = {0}, pClient = {0, 0};
    int        x, y, width, height, outside=0, size;

    windowSize(&width,&height);
    if (0 && inpIsMouseLocked())
    {
        x = mouse_x;
        y = mouse_y;
    }
    else
    {
        GetCursorPos(&pCursor);

        size = ClientToScreen( hwnd, &pClient );
 
        x = pCursor.x - pClient.x;
        y = pCursor.y - pClient.y;
    }

    if(xp)
        *xp = x;
    if(yp)
        *yp = y;

    if (x < 0 || y < 0 || x >= width || y >= height)
        outside = 1;

    return outside;
}

void inpLastMousePos(int *xp, int *yp)
{
    *xp = gMouseInpCur.x;
    *yp = gMouseInpCur.y;
}

int inpFocus()
{
int        x,y;

    if (!GetFocus())
        return 0;
    return !inpMousePos(&x,&y);
}

#include <utilitieslib/utils/Stackdump.h>

void inpClear()
{
    memset(inp_levels,0,sizeof(inp_levels));
    memset(inp_edges,0,sizeof(inp_edges));
    memset(textKeyboardBuffer, 0, sizeof(textKeyboardBuffer));
    altKeyState = 0;
    ctrlKeyState = 0;
    shiftKeyState = 0;
}

static char scan2ascii(DWORD scancode)
{
    UINT vk;
    static HKL layout;
    static unsigned char State[256];
    unsigned short result;

    layout = GetKeyboardLayout(0);
    if (GetKeyboardState(State) == FALSE)
        return 0;
    vk = MapVirtualKeyEx(scancode,1,layout);
    ToAsciiEx(vk,scancode,State,&result,0,layout);
    return result;
}

// char* inpDisplayState(unsigned char keyState[256]){
//     static char buffer[1024];
//     char* cursor = buffer;
//     int i;
// 
//     *cursor = '\0';
//     for(i = 0; i < INP_VIRTUALKEY_LAST; i++){
//         if(keyState[i]){
//             cursor += sprintf(cursor, "%s");
//         }
//     }
//     return buffer;
// }

unsigned char cur_key_states[256]; //debug only
void inpKeyboardUpdate()
{
    int i;
    HRESULT hr;
    DIDEVICEOBJECTDATA keyboardData[KEYBOARD_BUFFER_SIZE];
     int keyDataSize = KEYBOARD_BUFFER_SIZE;

    // Empty the keyboard buffer.
    hr = Keyboard->lpVtbl->GetDeviceData(Keyboard, sizeof(DIDEVICEOBJECTDATA), keyboardData, &keyDataSize, 0);

    // If the attempt was unsuccessful, the most likely reason is because
    // the keyboard has become unacquired.
    // 
    // Acquire the keyboard and try to get the keyboard state again.
    if(FAILED(hr)){
        inpKeyboardAcquire();
        hr = Keyboard->lpVtbl->GetDeviceData(Keyboard, sizeof(DIDEVICEOBJECTDATA), keyboardData, &keyDataSize, 0);

        if(FAILED(hr))
        {
            inpClear();
            return;
        }
    }
    //xyprintf(10, 9, "raw Dx keys: %s", inpDisplayState(cur_key_states));
     textKeyboardBuffer[0].type = KIT_None;

    // Playback the buffered data to find out what the accumulated keyboard state
    // since the last time the keyboard was checked.
    if(!okToInput(0))
    {
        memset(inp_edges,0,INP_MOUSE_BUTTONS);
    }
 
      for(i = 0; i < keyDataSize; i++)
     {
        inpUpdateKey(keyboardData[i].dwOfs, keyboardData[i].dwData, keyboardData[i].dwTimeStamp);
    }
}

int dxJoystickToInpKeyMapping[][2] = {
    {DIJOFS_BUTTON1, INP_JOY1},
    {DIJOFS_BUTTON2, INP_JOY2},
    {DIJOFS_BUTTON3, INP_JOY3},
    {DIJOFS_BUTTON4, INP_JOY4},
    {DIJOFS_BUTTON5, INP_JOY5},
    {DIJOFS_BUTTON6, INP_JOY6},
    {DIJOFS_BUTTON7, INP_JOY7},
    {DIJOFS_BUTTON8, INP_JOY8},
    {DIJOFS_BUTTON9, INP_JOY9},
    {DIJOFS_BUTTON10, INP_JOY10},
    {DIJOFS_BUTTON11, INP_JOY11},
    {DIJOFS_BUTTON12, INP_JOY12},
    {DIJOFS_BUTTON13, INP_JOY13},
    {DIJOFS_BUTTON14, INP_JOY14},
    {DIJOFS_BUTTON15, INP_JOY15},
    {DIJOFS_BUTTON16, INP_JOY16},
    {DIJOFS_BUTTON17, INP_JOY17},
    {DIJOFS_BUTTON18, INP_JOY18},
    {DIJOFS_BUTTON19, INP_JOY19},
    {DIJOFS_BUTTON20, INP_JOY20},
    {DIJOFS_BUTTON21, INP_JOY21},
    {DIJOFS_BUTTON22, INP_JOY22},
    {DIJOFS_BUTTON23, INP_JOY23},
    {DIJOFS_BUTTON24, INP_JOY24},
    {DIJOFS_BUTTON25, INP_JOY25},
};

enum
{
    DXDIR_UP,
    DXDIR_DOWN,
    DXDIR_LEFT,
    DXDIR_RIGHT,
    DXDIR_NUM,
};

int dxPovDefines[][4] = {
    {INP_JOYPAD_UP,INP_JOYPAD_DOWN,INP_JOYPAD_LEFT,INP_JOYPAD_RIGHT},
    {INP_POV1_UP,INP_POV1_DOWN,INP_POV1_LEFT,INP_POV1_RIGHT},
    {INP_POV2_UP,INP_POV2_DOWN,INP_POV2_LEFT,INP_POV2_RIGHT},
    {INP_POV3_UP,INP_POV3_DOWN,INP_POV3_LEFT,INP_POV3_RIGHT},
};

void inpJoystickState()
{
    HRESULT     hr;
    DIJOYSTATE2 js;           // DInput joystick state 
    int i;
    const S32 curTime = timeGetTime();

    static int joystick_sensitivity = 500;

    if( NULL == gJoystick ) 
        return;

    // Poll the device to read the current state
    hr = gJoystick->lpVtbl->Poll( gJoystick ); 
    if( FAILED(hr) )  
    {
        // DInput is telling us that the input stream has been
        // interrupted. We aren't tracking any state between polls, so
        // we don't have any special reset that needs to be done. We
        // just re-acquire and try again.
        hr = gJoystick->lpVtbl->Acquire( gJoystick );
        while( hr == DIERR_INPUTLOST ) 
            hr = gJoystick->lpVtbl->Acquire( gJoystick );

        // hr may be DIERR_OTHERAPPHASPRIO or other errors.  This
        // may occur when the app is minimized or in the process of 
        // switching, so just try again later 
        return; 
    }

    // Get the input's device state
    if( FAILED( hr = gJoystick->lpVtbl->GetDeviceState( gJoystick, sizeof(DIJOYSTATE2), &js ) ) )
        return; // The device should have been acquired during the Poll()

    // Convert joystick state to keyscan codes system can read

    // XY axis (Joystick 1)
    if( js.lX < -joystick_sensitivity )
        inpUpdateKey(INP_JOYSTICK1_LEFT, 1, curTime);
    else 
        inpUpdateKeyClearIfActive(INP_JOYSTICK1_LEFT, curTime);

    if( js.lX > joystick_sensitivity )
        inpUpdateKey(INP_JOYSTICK1_RIGHT, 1, curTime);
    else 
        inpUpdateKeyClearIfActive(INP_JOYSTICK1_RIGHT, curTime);

    if( js.lY < -joystick_sensitivity )
        inpUpdateKey(INP_JOYSTICK1_UP, 1, curTime);
    else
        inpUpdateKeyClearIfActive(INP_JOYSTICK1_UP, curTime);

    if( js.lY > joystick_sensitivity )
        inpUpdateKey(INP_JOYSTICK1_DOWN, 1, curTime);
    else
        inpUpdateKeyClearIfActive(INP_JOYSTICK1_DOWN, curTime);

    // Z/Zrot axis (Joystick 2)
    if( js.lZ < -joystick_sensitivity )
        inpUpdateKey(INP_JOYSTICK2_LEFT, 1, curTime);
    else
        inpUpdateKeyClearIfActive(INP_JOYSTICK2_LEFT, curTime);

    if( js.lZ > joystick_sensitivity )
        inpUpdateKey(INP_JOYSTICK2_RIGHT, 1, curTime);
    else
        inpUpdateKeyClearIfActive(INP_JOYSTICK2_RIGHT, curTime);

    if( js.lRz < -joystick_sensitivity )
        inpUpdateKey(INP_JOYSTICK2_UP, 1, curTime);
    else
        inpUpdateKeyClearIfActive(INP_JOYSTICK2_UP, curTime);

    if( js.lRz > joystick_sensitivity )
        inpUpdateKey(INP_JOYSTICK2_DOWN, 1, curTime);
    else
        inpUpdateKeyClearIfActive(INP_JOYSTICK2_DOWN, curTime);

    // Xrot/Yrot axis (Joystick 3?)
    if( js.lZ < -joystick_sensitivity )
        inpUpdateKey(INP_JOYSTICK3_LEFT, 1, curTime);
    else
        inpUpdateKeyClearIfActive(INP_JOYSTICK3_LEFT, curTime);

    if( js.lZ > joystick_sensitivity )
        inpUpdateKey(INP_JOYSTICK3_RIGHT, 1, curTime);
    else
        inpUpdateKeyClearIfActive(INP_JOYSTICK3_RIGHT, curTime);

    if( js.lRz < -joystick_sensitivity )
        inpUpdateKey(INP_JOYSTICK3_UP, 1, curTime);
    else
        inpUpdateKeyClearIfActive(INP_JOYSTICK3_UP, curTime);

    if( js.lRz > joystick_sensitivity )
        inpUpdateKey(INP_JOYSTICK3_DOWN, 1, curTime);
    else
        inpUpdateKeyClearIfActive(INP_JOYSTICK3_DOWN, curTime);

    
    // Pov[0] corresponds to the numpad: 0 = up, 9000 = right, 18000 = down, 27000 = left
    // I don't know where these insane numbers come from, this was by trial and error
    for( i = 0; i < DXDIR_NUM; i++ )
    {
         if( ((int)js.rgdwPOV[i] >= 0 && (int)js.rgdwPOV[i] <= 4500) || ((int)js.rgdwPOV[i] >= 31500) )
            inpUpdateKey(dxPovDefines[i][DXDIR_UP], 1, curTime);
        else 
            inpUpdateKeyClearIfActive(dxPovDefines[i][DXDIR_UP], curTime);

        if( (int)js.rgdwPOV[i] >= 4500 && (int)js.rgdwPOV[i] <= 13500 )
            inpUpdateKey(dxPovDefines[i][DXDIR_RIGHT], 1, curTime);
        else 
            inpUpdateKeyClearIfActive(dxPovDefines[i][DXDIR_RIGHT], curTime);

        if( (int)js.rgdwPOV[i] >= 13500 && (int)js.rgdwPOV[i] <= 22500 )
            inpUpdateKey(dxPovDefines[i][DXDIR_DOWN], 1, curTime);
        else 
            inpUpdateKeyClearIfActive(dxPovDefines[i][DXDIR_DOWN], curTime);

        if( (int)js.rgdwPOV[i] >= 22500 )
            inpUpdateKey(dxPovDefines[i][DXDIR_LEFT], 1, curTime);
        else
            inpUpdateKeyClearIfActive(dxPovDefines[i][DXDIR_LEFT], curTime);
    }

    // Note: ignoring rglSlider[0], rglSlider[1]

    //Now onto the buttons that are pressed
    for( i = 0; i < 24; i++ )
    {
        if ( js.rgbButtons[i] & 0x80 )
            inpUpdateKey(dxJoystickToInpKeyMapping[i][1], 1, curTime);
        else
            inpUpdateKeyClearIfActive(dxJoystickToInpKeyMapping[i][1], curTime);
    }
}


int dxMouseToInpKeyMapping[][2] = {
    {DIMOFS_BUTTON1, INP_LBUTTON},
    {DIMOFS_BUTTON2, INP_MBUTTON},
    {DIMOFS_BUTTON0, INP_RBUTTON},
    {DIMOFS_BUTTON3, INP_BUTTON4},
    {DIMOFS_BUTTON4, INP_BUTTON5},
    {DIMOFS_BUTTON5, INP_BUTTON6},
    {DIMOFS_BUTTON6, INP_BUTTON7},
    {DIMOFS_BUTTON7, INP_BUTTON8},
    {DIMOFS_Z, INP_MOUSEWHEEL},
};

int inpKeyFromDxMouseKey(int dxMouseKey)
{
    int mouseKeyCursor;

     for(mouseKeyCursor = 0; mouseKeyCursor < ARRAY_SIZE(dxMouseToInpKeyMapping); mouseKeyCursor++)
    {
        if(dxMouseToInpKeyMapping[mouseKeyCursor][0] == dxMouseKey)
        {
            return dxMouseToInpKeyMapping[mouseKeyCursor][1];
        }
    }
    return 0;
}




// state for tracking right mouse clicks - yay for lazy!
mouseState rmbstate = MS_NONE;
int rmbdownx = 0;
int rmbdowny = 0;
int rmbcurx = 0;    
int rmbcury = 0;
int rmbclickx = 0;
int rmbclicky = 0;
DWORD rmbtime = 0;
DWORD rdctime = 0;
int lmbdrag = 0;

mouseState lmbstate = MS_NONE;
int lmbdownx = 0;
int lmbdowny = 0;
int lmbcurx = 0;    
int lmbcury = 0;
int lmbclickx = 0;
int lmbclicky = 0;
DWORD lmbtime = 0;
DWORD ldctime = 0;
int rmbdrag = 0;

mouseState mmbstate = MS_NONE;
int mmbdownx = 0;
int mmbdowny = 0;
int mmbcurx = 0;    
int mmbcury = 0;
int mmbclickx = 0;
int mmbclicky = 0;
DWORD mmbtime = 0;
DWORD mdctime = 0;
int mmbdrag = 0;

// have to keep track of my own coords here, because mouse look code screws with gMouseInpCur

#define CLICK_TIME 250
#define CLICK_SIZE  16

///////////////////////////////////////////////////////////////////////////////////////////
// SubFunctions for handling mouse clicks
///////////////////////////////////////////////////////////////////////////////////////////
void inpMouseLeftClick(DIDEVICEOBJECTDATA *didod )
{
    int click = false, double_click = false;

    if ( (didod->dwData & 0x80) && gMouseInpCur.left == MS_NONE )
    {
        if (inpMousePosOnScreen(gMouseInpCur.x, gMouseInpCur.y)) {
            gMouseInpBuf[gInpBufferSize].left = MS_DOWN;
            gMouseInpBuf[gInpBufferSize].x = gMouseInpCur.x;
            gMouseInpBuf[gInpBufferSize].y = gMouseInpCur.y;
            gLastDownX = gMouseInpCur.x;
            gLastDownY = gMouseInpCur.y;
            gMouseInpCur.left = MS_DOWN;
            gInpBufferSize++;

            // doing these double-click messages as additive on down/up
            if (lmbtime != ldctime && // last click wasn't a double-click event
                didod->dwTimeStamp - lmbtime < gDoubleClickTime && 
                (ABS(lmbclickx-gMouseInpCur.x) <= CLICK_SIZE) &&
                (ABS(lmbclicky-gMouseInpCur.y) <= CLICK_SIZE) )
            {
                gMouseInpBuf[gInpBufferSize].left = MS_DBLCLICK;
                gMouseInpBuf[gInpBufferSize].x = gMouseInpCur.x;
                gMouseInpBuf[gInpBufferSize].y = gMouseInpCur.y;
                gInpBufferSize++;
                ldctime = didod->dwTimeStamp;
                inpUpdateKeyEx(INP_LDBLCLICK, true, didod->dwTimeStamp, lmbdownx, lmbdowny);
                double_click = true;
            }

            // start tracking LMB click
            lmbstate = MS_DOWN;
            lmbcurx = lmbdownx = gMouseInpCur.x;
            lmbcury = lmbdowny = gMouseInpCur.y;
            lmbtime = didod->dwTimeStamp; 
        }
    }
    else if ( gMouseInpCur.left == MS_DOWN && !(didod->dwData & 0x80) )
    {
        gMouseInpBuf[gInpBufferSize].left = MS_UP;
        gMouseInpBuf[gInpBufferSize].x = gMouseInpCur.x;
        gMouseInpBuf[gInpBufferSize].y = gMouseInpCur.y;
        gMouseInpCur.left = MS_NONE;
        gInpBufferSize++;
        lmbdrag = false;

        // decide if we have a RMB click
        if (lmbstate == MS_DOWN)
        {
            // allow a mouse movement of up to 20 pixels on each axis,
            // and a time difference of 400 ms
            if ((ABS(lmbdownx - lmbcurx) <= CLICK_SIZE) &&
                (ABS(lmbdowny - lmbcury) <= CLICK_SIZE) &&
                ((lmbtime - didod->dwTimeStamp <= CLICK_TIME) ||
                (didod->dwTimeStamp - lmbtime <= CLICK_TIME)))
            {
                gMouseInpBuf[gInpBufferSize].left = MS_CLICK;
                gMouseInpBuf[gInpBufferSize].x = lmbdownx;
                gMouseInpBuf[gInpBufferSize].y = lmbdowny;
                lmbclickx = lmbdownx;
                lmbclicky = lmbdowny;
                gInpBufferSize++;
                inpUpdateKeyEx(INP_LCLICK, true, didod->dwTimeStamp, lmbdownx, lmbdowny );
                click = true;
            }
            lmbstate = MS_NONE;
        }
    }

    if( !click )
        inpUpdateKeyClearIfActive(INP_LCLICK, didod->dwTimeStamp );
    if( !double_click )
        inpUpdateKeyClearIfActive(INP_LDBLCLICK, didod->dwTimeStamp );
}

void inpMouseRightClick(DIDEVICEOBJECTDATA *didod)
{
    int click = false, double_click = false;

    // register mouse downs or mouse ups 
    if ( (didod->dwData & 0x80 ) && gMouseInpCur.right == MS_NONE )
    {
        if (inpMousePosOnScreen(gMouseInpCur.x, gMouseInpCur.y)) {
            gMouseInpBuf[gInpBufferSize].right = MS_DOWN;
            gMouseInpBuf[gInpBufferSize].x = gMouseInpCur.x;
            gMouseInpBuf[gInpBufferSize].y = gMouseInpCur.y;
            gLastDownX = gMouseInpCur.x;
            gLastDownY = gMouseInpCur.y;
            gMouseInpCur.right = MS_DOWN;
            gInpBufferSize++;

            // doing these double-click messages as additive on down/up
            if (rmbtime != rdctime && // last click wasn't a double-click event
                didod->dwTimeStamp - rmbtime < gDoubleClickTime &&
                (ABS(rmbclickx-rmbdownx) <= CLICK_SIZE) &&
                (ABS(rmbclicky-rmbdowny) <= CLICK_SIZE) )
            {
                gMouseInpBuf[gInpBufferSize].right = MS_DBLCLICK;
                gMouseInpBuf[gInpBufferSize].x = gMouseInpCur.x;
                gMouseInpBuf[gInpBufferSize].y = gMouseInpCur.y;
                gInpBufferSize++;
                rdctime = didod->dwTimeStamp; 
                inpUpdateKeyEx(INP_RDBLCLICK, true, didod->dwTimeStamp, rmbdownx, rmbdowny);
                double_click = true;
            }

            // start tracking RMB click
            rmbstate = MS_DOWN;
            rmbcurx = rmbdownx = gMouseInpCur.x;
            rmbcury = rmbdowny = gMouseInpCur.y;
            rmbtime = didod->dwTimeStamp; 
        }
    }
    else if ( gMouseInpCur.right == MS_DOWN && !(didod->dwData & 0x80) )
    {
        gMouseInpBuf[gInpBufferSize].right = MS_UP;
        gMouseInpBuf[gInpBufferSize].x = gMouseInpCur.x;
        gMouseInpBuf[gInpBufferSize].y = gMouseInpCur.y;
        gMouseInpCur.right = MS_NONE;
        gInpBufferSize++;
        rmbdrag = false;

        // decide if we have a RMB click
        if (rmbstate == MS_DOWN)
        {
            // allow a mouse movement of up to 20 pixels on each axis,
            // and a time difference of 400 ms
            if ((ABS(rmbdownx - rmbcurx) <= CLICK_SIZE) &&
                (ABS(rmbdowny - rmbcury) <= CLICK_SIZE) &&
                ((rmbtime - didod->dwTimeStamp <= CLICK_TIME) ||
                (didod->dwTimeStamp - rmbtime <= CLICK_TIME)))
            {
                gMouseInpBuf[gInpBufferSize].right = MS_CLICK;
                gMouseInpBuf[gInpBufferSize].x = rmbdownx;
                gMouseInpBuf[gInpBufferSize].y = rmbdowny;
                rmbclickx = rmbdownx;
                rmbclicky = rmbdowny;
                gInpBufferSize++;
                inpUpdateKeyEx(INP_RCLICK, true, didod->dwTimeStamp, rmbdownx, rmbdowny);
                click = true;
            }
            rmbstate = MS_NONE;
        }
    }

    if( !click )
        inpUpdateKeyClearIfActive(INP_RCLICK, didod->dwTimeStamp);
    if( !double_click )
        inpUpdateKeyClearIfActive(INP_RDBLCLICK, didod->dwTimeStamp);
}

void inpMouseMiddleClick(DIDEVICEOBJECTDATA *didod)
{
    int click = false, double_click = false;

    // register mouse downs or mouse ups 
    if ( (didod->dwData & 0x80 ) && gMouseInpCur.mid == MS_NONE )
    {
        if (inpMousePosOnScreen(gMouseInpCur.x, gMouseInpCur.y)) {
            gMouseInpBuf[gInpBufferSize].mid = MS_DOWN;
            gMouseInpBuf[gInpBufferSize].x = gMouseInpCur.x;
            gMouseInpBuf[gInpBufferSize].y = gMouseInpCur.y;
            gLastDownX = gMouseInpCur.x;
            gLastDownY = gMouseInpCur.y;
            gMouseInpCur.mid = MS_DOWN;
            gInpBufferSize++;

            // doing these double-click messages as additive on down/up
            if (mmbtime != mdctime && // last click wasn't a double-click event
                didod->dwTimeStamp - mmbtime < gDoubleClickTime &&
                (ABS(mmbclickx-mmbdownx) <= CLICK_SIZE) &&
                (ABS(mmbclicky-mmbdowny) <= CLICK_SIZE) )
         {
             gMouseInpBuf[gInpBufferSize].mid = MS_DBLCLICK;
             gMouseInpBuf[gInpBufferSize].x = gMouseInpCur.x;
             gMouseInpBuf[gInpBufferSize].y = gMouseInpCur.y;
             gInpBufferSize++;
             mdctime = didod->dwTimeStamp; 
             inpUpdateKeyEx(INP_MDBLCLICK, true, didod->dwTimeStamp, mmbdownx, mmbdowny);
             double_click = true;
         }

         // start tracking RMB click
         mmbstate = MS_DOWN;
         mmbcurx = mmbdownx = gMouseInpCur.x;
         mmbcury = mmbdowny = gMouseInpCur.y;
         mmbtime = didod->dwTimeStamp; 
        }
    }
    else if ( gMouseInpCur.mid == MS_DOWN && !(didod->dwData & 0x80) )
    {
        gMouseInpBuf[gInpBufferSize].mid = MS_UP;
        gMouseInpBuf[gInpBufferSize].x = gMouseInpCur.x;
        gMouseInpBuf[gInpBufferSize].y = gMouseInpCur.y;
        gMouseInpCur.mid = MS_NONE;
        gInpBufferSize++;
        mmbdrag = false;

        // decide if we have a RMB click
        if (mmbstate == MS_DOWN)
        {
            // allow a mouse movement of up to 20 pixels on each axis,
            // and a time difference of 400 ms
            if ((ABS(mmbdownx - mmbcurx) <= CLICK_SIZE) &&
                (ABS(mmbdowny - mmbcury) <= CLICK_SIZE) &&
                ((mmbtime - didod->dwTimeStamp <= CLICK_TIME) ||
                (didod->dwTimeStamp - mmbtime <= CLICK_TIME)))
            {
                gMouseInpBuf[gInpBufferSize].mid = MS_CLICK;
                gMouseInpBuf[gInpBufferSize].x = mmbdownx;
                gMouseInpBuf[gInpBufferSize].y = mmbdowny;
                mmbclickx = mmbdownx;
                mmbclicky = mmbdowny;
                gInpBufferSize++;
                inpUpdateKeyEx(INP_MCLICK, true, didod->dwTimeStamp, mmbdownx, mmbdowny);
                click = true;
            }
            mmbstate = MS_NONE;
        }
    }

    if( !click )
        inpUpdateKeyClearIfActive(INP_MCLICK, didod->dwTimeStamp);
    if( !double_click )
        inpUpdateKeyClearIfActive(INP_MDBLCLICK, didod->dwTimeStamp);
}


void inpReadMouse( void )
{

    DIDEVICEOBJECTDATA didod[ MOUSE_BUFFER_SIZE ];  // Receives buffered data 
    DWORD dwElements = MOUSE_BUFFER_SIZE;
    DWORD i;
    HRESULT hr;
    int updatedMouseChord = 0;
    static int left_drag = false;
    const S32 curTime = timeGetTime();

     gMouseInpLast = gMouseInpCur;
    gDoubleClickTime = GetDoubleClickTime();
    if(optionGet(kUO_ReverseMouseButtons))
    {
        dxMouseToInpKeyMapping[0][0] = DIMOFS_BUTTON1;
        dxMouseToInpKeyMapping[2][0] = DIMOFS_BUTTON0;
    } 
    else
    {
        dxMouseToInpKeyMapping[0][0] = DIMOFS_BUTTON0;
        dxMouseToInpKeyMapping[2][0] = DIMOFS_BUTTON1;
    }

    if( NULL == gMouse ) 
        return;

    if(!inpMouseAcquire())
        return;

    //clear old buffer
     memset( &gMouseInpBuf, 0, sizeof(mouse_input) * MOUSE_INPUT_SIZE );
    gInpBufferSize = 0;

    hr = gMouse->lpVtbl->GetDeviceData( gMouse, sizeof(DIDEVICEOBJECTDATA), didod, &dwElements, 0 );

    // Study each of the buffer elements and process them.
    for( i = 0; i < dwElements && i < MOUSE_INPUT_SIZE; i++ ) 
    {
        switch( didod[ i ].dwOfs )
        {
            // register mouse downs or mouse ups with their coordinates
            case DIMOFS_BUTTON0: // left mouse button pressed or released
                if (!optionGet(kUO_ReverseMouseButtons))
                    inpMouseLeftClick(&(didod[i]));
                else 
                    inpMouseRightClick(&(didod[i]));
                break;
            case DIMOFS_BUTTON1: // right mouse button
                if (optionGet(kUO_ReverseMouseButtons)) 
                    inpMouseLeftClick(&(didod[i]));
                else 
                    inpMouseRightClick(&(didod[i]));
                break;
            case DIMOFS_BUTTON2: // middle button
                {
                    inpMouseMiddleClick(&(didod[i]));
                }break;
                
            case DIMOFS_X:
                {
                    gMouseInpCur.x += didod[ i ].dwData;
                    rmbcurx += didod[i].dwData;
                    lmbcurx += didod[i].dwData;
                    mouse_dx += didod[ i ].dwData;
                    
                    if (lmbstate == MS_DOWN)
                    {
                        if(    (ABS(lmbdownx - lmbcurx) >= CLICK_SIZE) ||
                            (ABS(lmbdowny - lmbcury) >= CLICK_SIZE) ||
                            (ABS((int)lmbtime - (int)didod[i].dwTimeStamp) > CLICK_TIME) )
                        {
                            lmbstate = MS_NONE;
                            gMouseInpBuf[gInpBufferSize].left = MS_DRAG;
                            gMouseInpBuf[gInpBufferSize].x = lmbdownx;
                            gMouseInpBuf[gInpBufferSize].y = lmbdowny;
                            gInpBufferSize++;
                            lmbdrag = true;
                        }
                    }

                    if (rmbstate == MS_DOWN)
                    {
                        if(    (ABS(rmbdownx - rmbcurx) >= CLICK_SIZE) ||
                            (ABS(rmbdowny - rmbcury) >= CLICK_SIZE) ||
                            (ABS((int)rmbtime - (int)didod[i].dwTimeStamp) > CLICK_TIME) )
                        {
                            rmbstate = MS_NONE;
                            gMouseInpBuf[gInpBufferSize].right = MS_DRAG;
                            gMouseInpBuf[gInpBufferSize].x = rmbdownx;
                            gMouseInpBuf[gInpBufferSize].y = rmbdowny;
                            gInpBufferSize++;
                            rmbdrag = true;
                        }
                    }

                    if (mmbstate == MS_DOWN)
                    {
                        if(    (ABS(mmbdownx - mmbcurx) >= CLICK_SIZE) ||
                            (ABS(mmbdowny - mmbcury) >= CLICK_SIZE) ||
                            (ABS((int)mmbtime - (int)didod[i].dwTimeStamp) > CLICK_TIME) )
                        {
                            mmbstate = MS_NONE;
                            gMouseInpBuf[gInpBufferSize].mid = MS_DRAG;
                            gMouseInpBuf[gInpBufferSize].x = mmbdownx;
                            gMouseInpBuf[gInpBufferSize].y = mmbdowny;
                            gInpBufferSize++;
                            mmbdrag = true;
                        }
                    }
                }break;
            case DIMOFS_Y:
                {
                    gMouseInpCur.y += didod[ i ].dwData;
                    rmbcury += didod[i].dwData;
                    lmbcury += didod[i].dwData;
                    mouse_dy += didod[ i ].dwData;

                    if (lmbstate == MS_DOWN)
                    {
                        if(    (ABS(lmbdownx - lmbcurx) >= CLICK_SIZE) ||
                            (ABS(lmbdowny - lmbcury) >= CLICK_SIZE) ||
                            (ABS((int)lmbtime - (int)didod[i].dwTimeStamp) > CLICK_TIME)    )
                        {
                            lmbstate = MS_NONE;
                            gMouseInpBuf[gInpBufferSize].left = MS_DRAG;
                            gMouseInpBuf[gInpBufferSize].x = lmbdownx;
                            gMouseInpBuf[gInpBufferSize].y = lmbdowny;
                            gInpBufferSize++;
                            lmbdrag = true;
                        }
                    }

                    if (rmbstate == MS_DOWN)
                    {
                        if(    (ABS(rmbdownx - rmbcurx) >= CLICK_SIZE) ||
                            (ABS(rmbdowny - rmbcury) >= CLICK_SIZE) ||
                            (ABS((int)rmbtime - (int)didod[i].dwTimeStamp) > CLICK_TIME)    )
                        {
                            rmbstate = MS_NONE;
                            gMouseInpBuf[gInpBufferSize].right = MS_DRAG;
                            gMouseInpBuf[gInpBufferSize].x = rmbdownx;
                            gMouseInpBuf[gInpBufferSize].y = rmbdowny;
                            gInpBufferSize++;
                            rmbdrag = true;
                        }
                    }

                    if (mmbstate == MS_DOWN)
                    {
                        if(    (ABS(mmbdownx - mmbcurx) >= CLICK_SIZE) ||
                            (ABS(mmbdowny - mmbcury) >= CLICK_SIZE) ||
                            (ABS((int)mmbtime - (int)didod[i].dwTimeStamp) > CLICK_TIME)    )
                        {
                            mmbstate = MS_NONE;
                            gMouseInpBuf[gInpBufferSize].mid = MS_DRAG;
                            gMouseInpBuf[gInpBufferSize].x = mmbdownx;
                            gMouseInpBuf[gInpBufferSize].y = mmbdowny;
                            gInpBufferSize++;
                            mmbdrag = true;
                        }
                    }
                }
                break;

            case DIMOFS_Z:
                {
                    gMouseInpCur.z += didod[i].dwData;
                }
                break;

            default:
                break;
        }

        // JS:    Read the mouse input again.  This time, into a format that can be used by
        //        the keybinding system.
        {
            // "Game key": the enumeration used by the game to specify a key.
            // See input.h
            int keyIndex = inpKeyFromDxMouseKey(didod[ i ].dwOfs); 
            int keyState;

            // If we don't know how to process this key, do nothing.
            if(keyIndex)
            {
                if(keyIndex == INP_MOUSEWHEEL)
                {
                    keyState = (int)didod[i].dwData/120;

                    if( keyState > 0 )
                        inpUpdateKey(INP_MOUSEWHEEL_FORWARD, 1, didod[i].dwTimeStamp );
                    else if( keyState < 0 )
                        inpUpdateKey(INP_MOUSEWHEEL_BACKWARD, 1, didod[i].dwTimeStamp );
                    else
                    {
                        inpUpdateKeyClearIfActive(INP_MOUSEWHEEL_FORWARD, didod[i].dwTimeStamp);
                        inpUpdateKeyClearIfActive(INP_MOUSEWHEEL_BACKWARD, didod[i].dwTimeStamp);
                    }
                }
                else
                    keyState = didod[i].dwData & 0x80;

                inpUpdateKey(keyIndex, keyState, didod[i].dwTimeStamp);
            }
        }

        // whenever we get edges on these, update the chord state
         if (didod[i].dwOfs == DIMOFS_BUTTON0 || didod[i].dwOfs == DIMOFS_BUTTON1) 
        {
             updatedMouseChord = 1;
            if (gMouseInpCur.left == MS_DOWN && gMouseInpCur.right == MS_DOWN && !mouseDragging() )
                inpUpdateKey(INP_MOUSE_CHORD, 1, didod[i].dwTimeStamp);
            else
                inpUpdateKeyClearIfActive(INP_MOUSE_CHORD, didod[i].dwTimeStamp);
        }
    }

    // check for chord buttons being down - have to keep faking key signals if held
    if (!updatedMouseChord)
    {
        if (gMouseInpCur.left == MS_DOWN && gMouseInpCur.right == MS_DOWN && !mouseDragging()) 
            inpUpdateKey(INP_MOUSE_CHORD, 1, curTime);
        else
            inpUpdateKeyClearIfActive(INP_MOUSE_CHORD, curTime);
    }

    // check for drag
     if (lmbstate == MS_DOWN && gInpBufferSize == 0)
    {
         // allow a mouse movement of up to 20 pixels on each axis,
        // and a time difference of 400 ms
        if ( ABS( (int)lmbtime - (int)GetTickCount() ) > CLICK_TIME )
        {
            gMouseInpBuf[gInpBufferSize].left = MS_DRAG;
            gMouseInpBuf[gInpBufferSize].x = lmbdownx;
            gMouseInpBuf[gInpBufferSize].y = lmbdowny;
            gInpBufferSize++;
            lmbdrag = true;
        }
    }
    if (rmbstate == MS_DOWN && gInpBufferSize == 0)
    {
        // allow a mouse movement of up to 20 pixels on each axis,
        // and a time difference of 400 ms
        if ( ABS( (int)rmbtime - (int)GetTickCount() ) > CLICK_TIME )
        {
            gMouseInpBuf[gInpBufferSize].right = MS_DRAG;
            gMouseInpBuf[gInpBufferSize].x = rmbdownx;
            gMouseInpBuf[gInpBufferSize].y = rmbdowny;
            gInpBufferSize++;
            rmbdrag = true;
        }
    }
    if (mmbstate == MS_DOWN && gInpBufferSize == 0)
    {
        // allow a mouse movement of up to 20 pixels on each axis,
        // and a time difference of 400 ms
        if ( ABS( (int)mmbtime - (int)GetTickCount() ) > CLICK_TIME )
        {
            gMouseInpBuf[gInpBufferSize].right = MS_DRAG;
            gMouseInpBuf[gInpBufferSize].x = mmbdownx;
            gMouseInpBuf[gInpBufferSize].y = mmbdowny;
            gInpBufferSize++;
            mmbdrag = true;
        }
    }

    if( lmbdrag )
        inpUpdateKeyEx( INP_LDRAG, 1, curTime, lmbdownx, lmbdowny );    
    else
        inpUpdateKeyClearIfActive(INP_LDRAG, curTime );

    if( rmbdrag )
        inpUpdateKeyEx( INP_RDRAG, 1, curTime, rmbdownx, rmbdowny );
    else
        inpUpdateKeyClearIfActive(INP_RDRAG, curTime );

    if( mmbdrag )
        inpUpdateKeyEx( INP_MDRAG, 1, curTime, mmbdownx, mmbdowny );
    else
        inpUpdateKeyClearIfActive(INP_MDRAG, curTime );

    // poll for mouse coordinates, so we have a decent starting point next time
    {
        int x = 0, y = 0;
        inpMousePos( &x, &y );

        gMouseInpCur.x = x;
        gMouseInpCur.y = y;
    }
}

// use to prevent joysticks from executing commands if sitting in idle state
static void inpUpdateKeyClearIfActive(int keyIndex, int timeStamp)
{
    if(inp_levels[keyIndex])
        inpUpdateKey(keyIndex, 0, timeStamp);
}

static int just_lost_focus_debug=0;

// Update the state of a single key.
void inpUpdateKey(int keyIndex, int keyState, int timeStamp)
{
    int x, y;
    inpMousePos( &x, &y );
    inpUpdateKeyEx( keyIndex,  keyState,  timeStamp, x, y );
}

extern int g_isBindingKey;

void inpUpdateKeyEx(int keyIndex, int keyState, int timeStamp, int x, int y )
{
    if( keyIndex > 255 )
        return;

    // If the key is being pressed...
    if (keyState)
    {
        // Is this the first time the key is being pressed?
        // If so, mark it as a keyboard edge.
        if(!inp_levels[keyIndex])
        {
            inp_edges[keyIndex] = 1;
        } else if (just_lost_focus_debug) {
            // We missed the up event because of our debugger hack
            inp_edges[keyIndex] = 1;
        }

        if(inp_levels[keyIndex] < 127)
        {
            if(INP_MOUSEWHEEL == keyIndex)
            {
                inp_levels[keyIndex] += keyState;
            }
            else
                inp_levels[keyIndex]++;
        }

        if(inp_edges[keyIndex])
        {
            // Mouse bindings always work.
            // -AB/CW: can't just pass key exec's on. will cause a crash when exiting to login screen :2005 Feb 04 04:37 PM
            // Pass on mouse, unless we're on menu, or are doing keybinding.
            if(okToInput(keyIndex) || (keyIndex >= INP_MOUSE_BUTTONS && isMenu(MENU_GAME) && !g_isBindingKey))
                bindKeyExecEx(keyIndex, keyState, timeStamp, x, y );

            if( !isMenu(MENU_GAME) && keyState && isGameReturnKeybind(keyIndex) )
                cmdParse( "gamereturn" );

        }
    }
    else
    {
        // If the key is no longer held down, then zero out
        // the input level for that key.
        inp_levels[keyIndex] = 0;

         if(okToInput(keyIndex))
            bindKeyExecEx(keyIndex, 0, timeStamp, x, y );

        // Do not zero out the input edge for the key because it is 
        // possible that the key was pressed and released since the
        // keyboard was checked last.
        //
        // In this case, we want to retain the edge signal so that
        // we know the key was at least pressed.
        //
        // Note that this means we must clean up the edge signals elsewhere,
        // before the keyboard data is processed.
    }
}

void inpUpdate()
{
int        j=0;
POINT    point = {0};
static int key_repeat,lost_focus, firstFrame = TRUE;
F32        key_rpt_times[2] = { 0.25,0.05 };
static DWORD lastInpTime = 0;

    // Special key cleanup.
    // Always reset the mousewheel state, otherwise, continuous mousewheel inputs
    // will not be detected.
     inp_levels[INP_MOUSEWHEEL] = 0;
    inp_edges[INP_MOUSEWHEEL] = 0;
    mouse_dx = 0;
    mouse_dy = 0;

    if (!GetFocus())
    {
         gMouse->lpVtbl->Unacquire( gMouse );
        //mouseClear();
        mouseFlush();
        ClipCursor(0);
        lost_focus = 1;
        GetCursorPos(&point);
        mouse_oldx = point.x;
        mouse_oldy = point.y;
        inpClear();
        return;
    }
    else if ( lost_focus )
    {
        lost_focus = 0;
        mouseClear();
        inpClear();
    }

    // decide if we just lost focus because of debugger
    if (!game_state.edit && !game_state.texWordEdit && (!game_state.maxfps || game_state.maxfps > 2) && IsDebuggerPresent() && game_state.fps > 2 && (GetTickCount() - lastInpTime > 1000)) // 1 second
    {
        DIDEVICEOBJECTDATA didod[ MOUSE_BUFFER_SIZE ];
        DWORD dwElements = MOUSE_BUFFER_SIZE;

        mouseClear();
        MouseClearButtonState();

        inpMouseAcquire();
        if (gMouse) gMouse->lpVtbl->GetDeviceData( gMouse, sizeof(DIDEVICEOBJECTDATA), didod, &dwElements, 0 );

        inpKeyboardAcquire();
        dwElements = MOUSE_BUFFER_SIZE;
        if (Keyboard) Keyboard->lpVtbl->GetDeviceData(Keyboard, sizeof(DIDEVICEOBJECTDATA), didod, &dwElements, 0);

        lastInpTime = GetTickCount();
        just_lost_focus_debug = 120; // Try to better catch keys that were let go while the game was paused
        return;
    }
    if (just_lost_focus_debug)
        just_lost_focus_debug--;

    lastInpTime = GetTickCount();

    memset(inp_edges, 0, sizeof(inp_edges));

    inpReadMouse();
    inpKeyboardUpdate();
    inpJoystickState();

    GetCursorPos(&point);
     mouse_lastx = point.x;
    mouse_lasty = point.y;

    if (mouse_lock)
    {
         if( firstFrame)
            firstFrame--;
        else
        {
            int    width,height,x,y;

             mouse_x += mouse_dx;
             mouse_y += mouse_dy;

            inpMousePos( &x, &y );
            windowClientSize( &width, &height );

             if( !gWindowDragging && (x <= 0 || x >= width-1 || y <= 0 || y >= height-1) )
            {
                 RECT rect;

                windowPosition(&x,&y);
                windowSize(&width,&height);

                rect.left = x;
                rect.top = y;
                rect.right = x + width;
                rect.bottom = y + height;

                x += width/2;
                y += height/2;

                SetCursorPos(x,y);

                mouse_lastx = x;
                mouse_lasty = y;
            }
        }
    }
    else
    {
         firstFrame = 2;
        mouse_x = mouse_lastx;
        mouse_y = mouse_lasty;
    }

     mtdx += mouse_dx;
    mtdy += mouse_dy;

    game_state.scrolled = 0;
}




void eatEdge( int idx )
{
    inp_edges[idx] = 0;
}

static HHOOK hHook;

static LRESULT CALLBACK LowLevelKeyboardProc (INT nCode, WPARAM wParam, LPARAM lParam)
{
    // By returning a non-zero value from the hook procedure, the
    // message does not get passed to the target window
    KBDLLHOOKSTRUCT *pkbhs = (KBDLLHOOKSTRUCT *) lParam;
    BOOL bControlKeyDown = 0;

    switch (nCode)
    {
        case HC_ACTION:
        {
            // Check to see if the CTRL key is pressed
            bControlKeyDown = GetAsyncKeyState (VK_CONTROL) >> ((sizeof(SHORT) * 8) - 1);
            
            // Disable CTRL+ESC
            if (pkbhs->vkCode == VK_ESCAPE && bControlKeyDown)
                return 1;

            // Disable ALT+TAB
            if (pkbhs->vkCode == VK_TAB && pkbhs->flags & LLKHF_ALTDOWN)
                return 1;

            // Disable ALT+ESC
            if (pkbhs->vkCode == VK_ESCAPE && pkbhs->flags & LLKHF_ALTDOWN)
                return 1;

             // Disable the WINDOWS key 
            if (pkbhs->vkCode == VK_LWIN || pkbhs->vkCode == VK_RWIN) 
                return 1; 
           break;
        }

        default:
            break;
    }
    return CallNextHookEx (hHook, nCode, wParam, lParam);
} 

void setNT4KeyHooks(int set)
{
    if (set)
        hHook = SetWindowsHookEx(WH_KEYBOARD_LL,LowLevelKeyboardProc,glob_hinstance,0);
    else if (hHook)
        UnhookWindowsHookEx(hHook);
}


void setWin95TaskDisable(int disable)
{
static UINT nPreviousState;

    SystemParametersInfo (SPI_SETSCREENSAVERRUNNING, disable, &nPreviousState, 0);
}

void inpDisableAltTab()
{
BOOL    m_isKeyRegistered;
int        m_nHotKeyID = 100, m_nHotKeyID2 = 101, m_nHotKeyID3 = 103;

    setNT4KeyHooks(1);
    setWin95TaskDisable(1);
    m_isKeyRegistered = RegisterHotKey(hwnd, m_nHotKeyID, MOD_ALT, VK_TAB);
    m_isKeyRegistered = RegisterHotKey(hwnd, m_nHotKeyID2, MOD_ALT, VK_ESCAPE);

    //RegisterHotKey(hwnd, m_nHotKeyID3, MOD_WIN, 'A');

}

void inpEnableAltTab()
{
    setWin95TaskDisable(0);
    setNT4KeyHooks(0);
}

int inpVKIsTextEditKey(int vk, int control){
    switch(vk){
        case 0x41: // a key
        case 0x43: // c key
        case 0x56: // v key
        case 0x58: // x key
            if(control)
                return 1;
            else
                return 0;
        case VK_INSERT:        
        case VK_DELETE:
        case VK_HOME:
        case VK_END:
        case VK_PRIOR:        // page up
        case VK_NEXT:        // page down
        case VK_UP:
        case VK_DOWN:
        case VK_RIGHT:
        case VK_LEFT:
        case VK_BACK:        // Backspace        
            return 1;
        default:
            return 0;
    }
}

bool inpIsNumLockOn(void)
{
    bool retval = FALSE;

    if( GetKeyState(VK_NUMLOCK) & 1 )
    {
        retval = TRUE;
    }

    return retval;
}

//----------------------------------------\
//  Helper for getting the scancode from a virtual key. It turns out
// that under Korean windows 98 certain scancodes don't get converted
// properly. for example VK_BACK doesn't become INP_BACK, or VK_RETURN
// to INP_RETURN
//
// ADD NEW KEYS HERE AS THEY ARE NEEDED.
//----------------------------------------
int GetScancodeFromVirtualKey(WPARAM wParam, LPARAM lParam)
{
    int scancode = MapVirtualKeyEx(wParam, 0, GetKeyboardLayout(0));
    if(lParam & 1 << 24)
        scancode |= 1 << 7;
    
    // ab: special case for korean windows. if the
    // scancode is zero try a custom mapping. the reason for this is that VK_BACK doesn't map to anything on korean win98
    //if( !scancode )
    // we have some values to override here
    {
        typedef struct ScancodeVkPair
        {
            int vkey;
            int scanCode;
            bool override;
        } ScancodeVkPair;
 
        static StashTable scancodeFromVk = NULL;
        ScancodeVkPair *scancodeTmp = 0;
        
        if( !scancodeFromVk )
        {
             static struct ScancodeVkPair codes[] = 
                {
                    {VK_BACK, INP_BACK},
                    {VK_RETURN, INP_RETURN},
                    {VK_LEFT, INP_LEFT, true},
                    {VK_RIGHT, INP_RIGHT, true},
                    {VK_UP, INP_UP, true},
                    {VK_DOWN, INP_DOWN, true},
                    {VK_HOME, INP_HOME, true},
                    {VK_END, INP_END, true},
                    {VK_INSERT, INP_INSERT},
                    {VK_DELETE, INP_DELETE, true},
                    {'V', INP_V},
                };
            int i;
            OSVERSIONINFO vi = { sizeof(vi) };
            GetVersionEx(&vi);

            scancodeFromVk = stashTableCreateInt( (3*ARRAY_SIZE(codes))/2 ); //arbitrary pad. pack it pretty tight, and don't waste too much space
        
            // only init the values if this is before win 2k. this
            // function is targeted at win98 korean
            if( vi.dwMajorVersion < 5 )
            {
                for( i = 0; i < ARRAY_SIZE( codes ); ++i ) 
                {
                    verify(stashIntAddPointer( scancodeFromVk, codes[i].vkey, &codes[i], false ));
                }
            }
        }
        
        
        if( stashIntFindPointer( scancodeFromVk, wParam, &scancodeTmp ) 
            && (!scancode || scancodeTmp->override ))
        {
            scancode = scancodeTmp->scanCode;
        }
    }
    return scancode;
}
