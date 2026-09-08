#ifndef UITABCONTROL_H__
#define UITABCONTROL_H__


#include <utilitieslib/stdtypes.h>
#include "gameComm/wdwbase.h"

typedef void* uiTabData;
typedef struct uiTabControl uiTabControl;
typedef struct ContextMenu ContextMenu;

// can only drag & drop between tab controls of the same TYPE
typedef enum TabType {
    TabType_Undraggable = 0,
    TabType_ChatTab,
}TabType;

typedef enum TabDirection {
    TabDirection_Horizontal = 0,
    TabDirection_Vertical,
}TabDirection;
typedef void (*uiTabActionFunc)(uiTabData);    
typedef void (*uiTabActionFunc2)(uiTabData, uiTabData);
typedef void (*uiTabFontColorFunc)(uiTabData, bool selected);

uiTabControl *    uiTabControlCreate(TabType type, uiTabActionFunc onSelectedCode, uiTabActionFunc2 onMovedCode, uiTabActionFunc onDestroy, uiTabFontColorFunc fontColorFunc, ContextMenu *cm);
uiTabControl *    uiTabControlCreateEx(TabType type, uiTabActionFunc onSelectedCode, uiTabActionFunc2 onMovedCode, uiTabActionFunc onDestroy, uiTabFontColorFunc fontColorFunc, ContextMenu *cm, int dragHoverSelect);
void            uiTabControlDestroy(uiTabControl * tc);

void            uiTabControlSetParentWindow(uiTabControl * tc, WindowName wdw);    // use this if tab control is inside a draggable (resizable) window

void             uiTabControlAdd(uiTabControl * tc, const char * text, uiTabData data);
void            uiTabControlAddColorOverride(uiTabControl * tc, const char * text, uiTabData data, int color, int activeColor);
void            uiTabControlAddCopy(uiTabControl * tc, const char * text, uiTabData data);
void            uiTabControlRename(uiTabControl * tc, const char * text, uiTabData data);
void             uiTabControlRemove(uiTabControl * tc, uiTabData data);
int             uiTabControlRemoveByName(uiTabControl * tc, const char * str);
void            uiTabControlRemoveAll(uiTabControl * tc);

uiTabData        drawTabControl(uiTabControl * tc, float x, float y, float z, float width, float height, float scale, int color, int activeColor, TabDirection dir);

void            uiTabControlSelect(uiTabControl * tc, uiTabData data);
void            uiTabControlSelectByName(uiTabControl *tc, const char *name);
bool            uiTabControlSelectEx(uiTabControl * tc, uiTabData data, uiTabActionFunc onSelectedCode);
int                uiTabControlGetSelectedIdx(uiTabControl * tc);
void            uiTabControlSelectByIdx(uiTabControl *tc, int idx);
uiTabData        uiTabControlGetSelectedData(uiTabControl * tc);
bool            uiTabControlSelectNext(uiTabControl * tc);  // return true if we wrapped off the end
bool            uiTabControlSelectPrev(uiTabControl * tc);
char*            uiTabControlGetTabName(uiTabControl * tc, int idx);

// return true if at least one tab in the control has 'data'
bool            uiTabControlHasData(uiTabControl * tc, uiTabData data);

// move all tabs in src to dest
void uiTabControlMergeControls(uiTabControl * dest, uiTabControl * src); // used by the chat window to collapse two panes into one
typedef struct TrayObj TrayObj;
int uiTabControlChatPhantomAllowable( uiTabControl *tc, TrayObj * obj );                // used by the chat window to determine if a phantom frame should be drawn when a tab is being dragged
int uiTabControlNumTabs( uiTabControl * tc );                                            // used by chat context menus to determine pan switching availability
int uiTabControlMoveTabPublic(uiTabControl *dest, uiTabControl* src, uiTabData data);    // used by chat tab context menu to move a tab to opposite pane
float uiTabControlGetWidth( uiTabControl *tc );                                            // yet another function for chat, this one is so the divider bar can ignore input from areas where tabs are
float uiTabControlGetMinDrawHeight(uiTabControl *tc);

// iteration through elements -- returns NULL when end of list is reached
uiTabData        uiTabControlGetFirst(uiTabControl * tc);
uiTabData        uiTabControlGetNext(uiTabControl * tc);

uiTabData        uiTabControlGetLast(uiTabControl * tc);

void uiTabDestroyFreeData(void * tab);
void uiTabControl_SetSelectCb(uiTabControl *tc, uiTabActionFunc onSelectedCode );

#define TAB_HEIGHT            18

#endif // UITABCONTROL_H__
