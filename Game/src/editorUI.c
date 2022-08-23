#include <utilitieslib/stdtypes.h>
#include "editorUI.h"
#include "gameComm/wdwbase.h"
#include "UI/uiWindows.h"
#include "UI/uiUtilGame.h"
#include "UI/uiUtil.h"
#include "edit/edit_cmd.h"
#include <utilitieslib/components/earray.h>
#include "graphics/textureatlas.h"
#include "UI/uiUtilMenu.h"
#include "graphics/ttFont.h"
#include "graphics/ttFontUtil.h"
#include "UI/sprite/sprite_base.h"
#include "UI/uiInput.h"
#include "win/input.h"
#include "UI/sprite/sprite_text.h"
#include "UI/sprite/sprite_font.h"
#include "UI/uiEditText.h"
#include "UI/uiSlider.h"
#include "UI/uiDialog.h"
#include "UI/UIEdit.h"
#include <utilitieslib/components/EString.h>
#include "UI/uiComboBox.h"
#include "UI/uiBox.h"
#include "UI/uiScrollBar.h"
#include <utilitieslib/utils/mathutil.h>
#include "cmdparse/cmdcommon.h"
#include "UI/uiStatus.h"
#include <utilitieslib/components/StashTable.h>
#include "UI/uiClipper.h"
#include <utilitieslib/utils/rgb_hsv.h>

//#define CLR_WHITE            0xffffffff
//#define CLR_BLACK            0x000000ff

static StashTable editorUIWidgetHash = 0;

#define CLR_TEXT                    0x000000ff
#define CLR_TEXTGREY                0x3f3f3fff
#define CLR_TEXTLOCKED                0x4f4f4fff

#define CLR_WDW                        0xd4d0c8ff
#define CLR_WDWFRAME                0xafafafff
#define CLR_TITLE                    0x5f5f5fff
#define CLR_FOCUSTITLE                0x5075c0ff

#define CLR_BUTTON                    0xe8e4dbff
#define CLR_FRAME                    0x808080ff
#define CLR_WIDGET_BACK                0xdfdfdfff
#define CLR_CBOX_BORDER                0x1f1f1fff

#define CLR_WIDGET_BACK_LOCKED        0xccc8c0ff
#define CLR_RADIO_LOCKED            0x7f7f7fff
#define CLR_SLIDER_LOCKED            0x7f7f7fff

#define CLR_SELECTED_TEXT            0x7f0000ff
#define CLR_SELECTED_FRAME            0xf08080ff
#define CLR_SELECTED_WIDGET_BACK    0xffffffff

#define OLDUISCALE 0.6
#define SLIDER_TEXTSPACE 40

extern Wdw winDefs[MAX_CUSTOM_WINDOW_COUNT+MAX_WINDOW_COUNT];

extern TTDrawContext editorFont;

//static int editorUIDisableClicks = 0;

static TTDrawContext* getEditorFont(void)
{
    return &editorFont;
}

static void setEditorFontActive(int color)
{
    font(&editorFont);
    font_color(color,color);
}

// checks to see if the mouse is over a particular rectangle
int isOver(int x,int width,int y,int height) {
    int mx,my;
    inpMousePos(&mx,&my);
    return (mx>x && mx<(x+width) && my<y && my>(y-height));
}

#define EDITORUI_BUFFER 10
//faking polymorphism
typedef struct EditorUIWidget EditorUIWidget;

typedef struct EditorUISubWidgets {
    int (*showSubWidgets)(int);        // function that tells whether or not to show subwidgets
    EditorUIWidget ** widgets;        // earray of child widgets
    int ID;                            // ID, sent to showSubWidgets function, globally increasing
    int frame;                        // whether to draw a frame around the subwidgets
    int    *lockWidgets;                // whether to lock all sub widgets (locked means viewable but not changable)
} EditorUISubWidgets;

int editorUIGlobalWidgetID;
int editorUIGlobalSubWidgetID;
int editorUILoseFocus;
int editorUISelectedWindow;        // make sure only one window has any focus at a time

typedef struct EditorUIWidget {
    int (*draw)(EditorUIWidget * widget,int x,int y,int z,int top,int bottom,double scale,int hasFocus,int locked);        // how this widget draws itself
    void (*destroy)(void * widget);    // destructor
    double elementsTall;            // how many elements tall this widget is
    int window;                        // each widget knows what parent it belongs to
    EditorUICallback callback;        // function that gets called when this widget changes
    void * callbackParam;            // param passed to callback function when widget changes
    EditorUISubWidgets **subWidgets;// array of groups of subwidgets
    int ID;                            // ID, sent to callback functions, globally increasing by 1 per widget
    int selected;                    // if non-zero, currently selected by the user
    int canBeSelected;                // forcibly unselected if this is false
    int    isUserInteracting;            // User is currently interacting with the widget (to break cyclical dependencies)
    int startX;                        // for things like combo boxes and text boxes, so they can all line up
    EditorUIType type;                // type of widget
} EditorUIWidget;

typedef struct EditorUINullWidget {    // this only exists so subwidgets work anywhere
    EditorUIWidget myWidget;        // this widget is the first in any list, and is
} EditorUINullWidget;                // never actually drawn

typedef struct EditorUICheckBox {
    EditorUIWidget myWidget;    // parent functionality
    char * myText;                // text associated with this check box
    int * myValue;                // whether or not the box is checked
} EditorUICheckBox;

typedef struct EditorUICheckBoxList {
    EditorUIWidget myWidget;    // parent functionality
    char ** boxTexts;            // text for each check box
    int * myValue;                // bitfield that says whether or not the box is checked
    int maxWidth;                // Width of the largest string in the list
} EditorUICheckBoxList;

typedef struct EditorUITabControl {
    EditorUIWidget myWidget;    // parent functionality
    int * myValue;                // which tab is selected
    char  ** myText;            // array of strings, one for each tab
    int (*displayCountCallback)(void);    // callback to determine how many tabs to display
    int initialWidth;            // Initial width that the window started at
    float openleft,openright;    // x coordinates of opening in child frame
} EditorUITabControl;

typedef struct EditorUIRadioButtons {
    EditorUIWidget myWidget;    // parent functionality
    int * myValue;                // which radio button is selected
    char  ** myText;            // array of strings, one for each radio button
    int displayHorizontal;        // default is to display radio buttons vertically
} EditorUIRadioButtons;

typedef struct EditorUIStaticText {
    EditorUIWidget myWidget;    // parent functionality
    char * myText;                // text
} EditorUIStaticText;

typedef struct EditorUIDynamicText {
    EditorUIWidget myWidget;    // parent functionality
    void (*textCallback)(char *);    // text callback
} EditorUIDynamicText;

typedef struct EditorUITextEntry {
    EditorUIWidget myWidget;    // parent functionality
    char * myText;                // text next to the text entry box
    char * myValue;                // the text that the user has entered
    char * myOldValue;            // text last tick, so we know when it has changed
    int myCapacity;                // how much space was allocated for this edit box, static
    UIEdit * myEditState;        // necessary for using the standard text entry stuff
} EditorUITextEntry;

typedef struct EditorUIScrollableList {
    EditorUIWidget myWidget;    // parent functionality
    int height, width;            // height and width of the window
    int strWidth;                // max string width
    int * myValue;                // index of item currently selected in the list
    int lastClicked;            // last item clicked, highlighted in the list
    char ** myStrings;            // list of strings to be displayed
    ScrollBar * scrollBar;        // scrollbar for the string list
} EditorUIScrollableList;

typedef struct EditorUISlider {
    EditorUIWidget myWidget;    // parent functionality
    char * myText;                // text next to the slider
    float * myValue;            // value of the slider seen by the user
    float sliderValue;            // value of the slider on [-1,1]
    float min,max;                // minimum and maximum values for the slider
    int integer;                // only allow integer values if this is true
} EditorUISlider;

typedef struct EditorUIEditSlider {
    EditorUIWidget myWidget;    // parent functionality
    char * myText;                // text next to the slider
    float * myValue;            // value of the slider seen by the user
    float sliderValue;            // value of the slider on [-1,1]
    float smin,smax;            // minimum and maximum values for the slider
    float emin,emax;            // minimum and maximum values for the edit box
    float min,max;                // minimum and maximum values for both
    int integer;                // only allow integer values if this is true
    char myOldString[256];        // text last tick, so we know when it has changed
    UIEdit * myEditState;        // necessary for using the standard text entry stuff
} EditorUIEditSlider;

typedef struct EditorUIComboBox {
    EditorUIWidget myWidget;    // parent functionality
    int * myValue;                // index of item currently selected in the combo box
    char * myText;                // text next to the combo box
    ComboBox * myCombo;            // standard combo box UI struct
    char ** myStrings;            // EArray of strings in the combo box
    char myTarget[256];            // string that user has typed to search for in myStrings
    UIEdit * myEditState;        // used to turn off keybinds while typing in values
    int myTempSelection;        // the item that we're auto-completing
} EditorUIComboBox;

typedef struct EditorUIList {
    EditorUIWidget myWidget;    // parent functionality
    int * myValue;                // index of item currently selected in the list
    char * myText;                // list header
    char ** myStrings;            // EArray of strings in the list
    int myWidth;                // width of the list (i.e. width of the longest entry)
    int lastClicked;            // last item clicked, highlighted in the list
    int tabbing;                // flag to keep us from tabbing through 100 things at once
} EditorUIList;

typedef struct EditorUIButtonRow {
    EditorUIWidget myWidget;    // parent functionality
    char ** myTexts;            // text shown on each button
    void (**myFuncs)(char*text);// callbacks for each button
} EditorUIButtonRow;

typedef struct EditorUIIconRow {
    EditorUIWidget myWidget;    // parent functionality
    char ** myTexts;            // text shown on each button
    char ** myIcons;            // icon for each button
    void (**myFuncs)(void*data);// callbacks for each button
    void ** myData;                // passed to the callbacks
    int myWidth;                // width of each icon
    int myHeight;                // height of each icon
    ScrollBar myScrollBar;        // scrollbar
} EditorUIIconRow;

typedef struct EditorUIButton {
    EditorUIWidget myWidget;    // parent functionality
    char * buttonDesc;            // text shown to the side describing the button
    char * buttonText;            // text shown on the button
    void (*myFunc)(char*text);    // callbacks for the button
} EditorUIButton;

typedef struct EditorUIProgressBar {
    EditorUIWidget myWidget;    // parent functionality
    float (*percentCallback)(void);    // callback to get percent completion
} EditorUIProgressBar;

typedef struct EditorUIConsole {
    EditorUIWidget myWidget;    // parent functionality
    int myBGColor;                // bg color of the console
    int myLinesTall;            // number of lines that can be displayed
    char ** lines;                // all lines sent to the console
    int * colors;                // colors of those lines
} EditorUIConsole;

typedef struct EditorUIBreakdownBar {
    EditorUIWidget myWidget;    // parent functionality
    int numDivisions;
    float pulse_counter;
    float (*percentCallback)(int,int *);    // callback to get percent completion
} EditorUIBreakdownBar;

typedef struct EditorUIHueSelector {
    EditorUIWidget myWidget;    // parent functionality
    float *myValue;
} EditorUIHueSelector;

typedef struct EditorUISaturationValueSelector {
    EditorUIWidget myWidget;    // parent functionality
    float *hsv;
} EditorUISaturationValueSelector;

typedef struct EditorUIDualColorSwatch {
    EditorUIWidget myWidget;    // parent functionality
    U8* color1;
    U8* color2;
} EditorUIDualColorSwatch;


//////////////////////////////////////////////////////////////////////////

typedef struct TreeElement
{
    int child_count;
    char *name;
    struct TreeElement **children;
} TreeElement;

MP_DEFINE(TreeElement);

static TreeElement *allocTreeElement(void) {
    MP_CREATE(TreeElement, 256);
    return MP_ALLOC(TreeElement);
}

static void freeTreeElement(TreeElement *elem) {
    if (!elem)
        return;
    if (elem->children)
        eaDestroyEx(&elem->children, freeTreeElement);
    SAFE_FREE(elem->name);
    MP_FREE(TreeElement, elem);
}

typedef struct EditorUITreeControl {
    EditorUIWidget myWidget;    // parent functionality
    int multi_select;            // allow multiple selections
    int multi_open;                // allow multiple open branches
    ScrollBar * scrollBarV;        // vertical scrollbar
    ScrollBar * scrollBarH;        // horizontal scrollbar
    TreeElementAddress **selected; // selected elements
    TreeElement *root_element;    // root of the tree;
    int (*elementCallback)(char*namebuf, const TreeElementAddress *elem);    // callback to figure out what elements are named and how many children they have
} EditorUITreeControl;

//////////////////////////////////////////////////////////////////////////

typedef struct EditorUIWindow {
    EditorUIWidget ** widgets;    // NULL if the window is inactive
    ScrollBar scrollBar;        // standard UI scroll bar
    void (*closeCallback)();    // callback function for when the window is closed, may be NULL
    void (*changeCallback)();    // callback function for when the user has changed something, may be NULL
    int sliderStartX;            // X offset where all sliders start (so the line up nice 'n stuff)
    int textEntryStartX;        // X offset of where all text entry boxes start (same as above)
    int comboBoxStartX;            // Same as above, but for Combo Boxes
    int startX;                    // taking over all of the above three values into one (ideally)
    int width;
    int destroy;                // true if we should destroy this 
    int modal;                    // 1 if global modal, 0 if not modal, -x if modal w/ respect to window x
    int noClose;                // true if the close button should not be displayed
    EditorUIWidget ** current;    // stack of widgets, in case subwidgets get added during creation
    int ID;                        // just like the EditorUISubWidgets ID value
    U32 missionEditorOnly : 1;    // Only shown when in mission editing mode
} EditorUIWindow;

#define EDITORUI_MAX_WINDOWS 10
EditorUIWindow editorUIWindows[EDITORUI_MAX_WINDOWS];    // we have to hardcode all the UIs we make

int editorUIAddNullWidget(int ID,double elementsTall);
int neverShow() {
    return 0;
}

int revLookup(int index)
{
    switch (index)
    {
        xcase WDW_EDITOR_UI_WINDOW_1: return 0;
        xcase WDW_EDITOR_UI_WINDOW_2: return 1;
        xcase WDW_EDITOR_UI_WINDOW_3: return 2;
        xcase WDW_EDITOR_UI_WINDOW_4: return 3;
        xcase WDW_EDITOR_UI_WINDOW_5: return 4;
        xcase WDW_EDITOR_UI_WINDOW_6: return 5;
        xcase WDW_EDITOR_UI_WINDOW_7: return 6;
        xcase WDW_EDITOR_UI_WINDOW_8: return 7;
        xcase WDW_EDITOR_UI_WINDOW_9: return 8;
        xcase WDW_EDITOR_UI_WINDOW_10: return 9;
    }
    assert(0 && "This window does not exist");
    return 0;
}

// Because order must be preserved in the WindowName list, this function has to be done
// This may look like its the most worthless function ever, but I promise its not
int editorUIGetWindow(int offset)
{
    switch (offset)
    {
        xcase 0: return WDW_EDITOR_UI_WINDOW_1;
        xcase 1: return WDW_EDITOR_UI_WINDOW_2;
        xcase 2: return WDW_EDITOR_UI_WINDOW_3;
        xcase 3: return WDW_EDITOR_UI_WINDOW_4;
        xcase 4: return WDW_EDITOR_UI_WINDOW_5;
        xcase 5: return WDW_EDITOR_UI_WINDOW_6;
        xcase 6: return WDW_EDITOR_UI_WINDOW_7;
        xcase 7: return WDW_EDITOR_UI_WINDOW_8;
        xcase 8: return WDW_EDITOR_UI_WINDOW_9;
        xcase 9: return WDW_EDITOR_UI_WINDOW_10;
    }
    assert(0 && "This window does not exist");
    return 0;
}

// creates and returns an ID for an EditorUI window
// returns 0 if there are no available windows
int editorUICreateWindow(const char *title) {
    int i;
    for (i=0;i<EDITORUI_MAX_WINDOWS;i++)
        if (editorUIWindows[i].widgets==NULL) {
            eaCreate(&editorUIWindows[i].widgets);
            window_setMode(editorUIGetWindow(i),WINDOW_DISPLAYING);
            window_SetTitle(editorUIGetWindow(i), title);
            window_SetFlippage(editorUIGetWindow(i), 0);
            memset(&editorUIWindows[i].scrollBar,0,sizeof(ScrollBar));
            editorUIWindows[i].destroy=0;
            editorUIWindows[i].startX=0;
            editorUIWindows[i].comboBoxStartX=0;
            editorUIWindows[i].textEntryStartX=0;
            editorUIWindows[i].sliderStartX=0;
            editorUIWindows[i].ID=editorUIGlobalSubWidgetID++;
            editorUIWindows[i].modal=0;
            editorUIWindows[i].noClose=0;
            editorUIWindows[i].missionEditorOnly=0;
            editorUIWindows[i].closeCallback=NULL;
            editorUIWindows[i].width=25;
            if (title)
            {
                int wd = str_wd(&game_12, 1.2, 1.2, "%s", title);
                editorUIWindows[i].width+=35+wd;
            }
            
            editorUIAddNullWidget(i+1,0);
            return i+1;
        }
    return 0;
}

#define EDITORUI_VALIDATE_ID(ID)                        \
    EditorUIWindow * window;                            \
    ID--;                                                \
    if (ID<0 || ID>=EDITORUI_MAX_WINDOWS) return;        \
    if (editorUIWindows[ID].widgets==NULL) return;        \
    window=&editorUIWindows[ID];

#define EDITORUI_VALIDATE_ID_RET(ID)                    \
    EditorUIWindow * window;                            \
    ID--;                                                \
    if (ID<0 || ID>=EDITORUI_MAX_WINDOWS) return -1;    \
    if (editorUIWindows[ID].widgets==NULL) return -1;    \
    window=&editorUIWindows[ID];

EditorUIWidget* editorUIWidgetFromID(int ID)
{
    void* widget;
    if (stashIntFindPointer(editorUIWidgetHash, ID, &widget))
        return widget;
    return NULL;
}

void editorUISetSize(int ID, float width, float height)
{
    EDITORUI_VALIDATE_ID(ID)
    window_setDims(editorUIGetWindow(ID), -1, -1, width, height);
}

void editorUIGetSize(int ID, float *width, float *height)
{
    float x,y,z,sc;
    int color,bcolor;
    EDITORUI_VALIDATE_ID(ID)
    window_getDims(editorUIGetWindow(ID), &x, &y, &z, width, height, &sc, &color, &bcolor);
}

void editorUISetPosition(int ID, float x, float y)
{
    EDITORUI_VALIDATE_ID(ID)
    window_setDims(editorUIGetWindow(ID), x, y, -1, -1);
}

void editorUIGetPosition(int ID, float *x, float *y)
{
    float wd,ht,z,sc;
    int color,bcolor;
    EDITORUI_VALIDATE_ID(ID)
    window_getDims(editorUIGetWindow(ID), x, y, &z, &wd, &ht, &sc, &color, &bcolor);
}

void windowClientSize(int *width,int *height);

void editorUICenterWindow(int ID)
{
    int screen_width, screen_height;
    float window_width, window_height;
    windowClientSize(&screen_width, &screen_height);
    editorUIGetSize(ID, &window_width, &window_height);
    editorUISetPosition(ID, (screen_width-window_width)*0.5f, (screen_height-window_height)*0.5f);
}

#define EDITORUI_GLOBAL_MODAL    1
// 1:  modal to all windows
// 0:  not modal
// -x: means is modal with respect to window x
void editorUISetModal(int ID, int modal)
{
    EDITORUI_VALIDATE_ID(ID)
    editorUIWindows[ID].modal=modal;
}

void editorUISetMEOnly(int ID, int meOnly)
{
    EDITORUI_VALIDATE_ID(ID)
    editorUIWindows[ID].missionEditorOnly=meOnly;
}

// Modal relative to a certain window
void editorUISetRelativeModal(int ID, int whichWindow)
{
    EDITORUI_VALIDATE_ID(ID)
    editorUIWindows[ID].modal=-whichWindow;
}

void editorUISetNoClose(int ID, int noClose)
{
    EDITORUI_VALIDATE_ID(ID)
    editorUIWindows[ID].noClose=noClose;
}

// window->current is a stack of widgets, it signifies what widget 
// new widgets are added to as subwidgets
int editorUIAddSubWidgets(int ID,int (*showSubWidgets)(int),int showFrame,int *lockWidgets) {
    EditorUISubWidgets *** subs;
    EditorUISubWidgets * subWidget;
    EDITORUI_VALIDATE_ID_RET(ID);
    if (eaSize(&window->current)==0)
        eaPush(&window->current,window->widgets[eaSize(&window->widgets)-1]);
    else {
        EditorUIWidget * widget=window->current[eaSize(&window->current)-1];
        subWidget=widget->subWidgets[eaSize(&widget->subWidgets)-1];
        eaPush(&window->current,subWidget->widgets[eaSize(&subWidget->widgets)-1]);
    }
    subs=&window->current[eaSize(&window->current)-1]->subWidgets;
    subWidget=calloc(1,sizeof(EditorUISubWidgets));
    eaPush(subs,subWidget);
    subWidget->showSubWidgets=showSubWidgets;
    subWidget->widgets=NULL;
    subWidget->ID=editorUIGlobalSubWidgetID++;
    subWidget->frame=showFrame;
    subWidget->lockWidgets=lockWidgets;
    editorUIAddNullWidget(ID+1,0);    // ID has already been validated, normally this doesn't happen
    return subWidget->ID;
}

void editorUIEndSubWidgets(int ID) {
    EDITORUI_VALIDATE_ID(ID);
    eaSetSize(&window->current,eaSize(&window->current)-1);
}

void editorUISetCloseCallback(int ID,void (*closeCallback)()) {
    EDITORUI_VALIDATE_ID(ID);
    editorUIWindows[ID].closeCallback=closeCallback;
}

void editorUISetChangeCallback(int ID,void (*changeCallback)()) {
    EDITORUI_VALIDATE_ID(ID);
    editorUIWindows[ID].changeCallback=changeCallback;
}

// only user called functions need to validate the ID, since it decrements the ID
// it would be bad to call it twice
int addWidgetToWindow(int ID,EditorUIWidget * widget) {
    widget->ID=++editorUIGlobalWidgetID;
    if (eaSize(&editorUIWindows[ID].current)==0)
        eaPush(&editorUIWindows[ID].widgets,widget);
    else {
        EditorUISubWidgets ** subWidgets=editorUIWindows[ID].current[eaSize(&editorUIWindows[ID].current)-1]->subWidgets;
        EditorUIWidget *** widgets=&subWidgets[eaSize(&subWidgets)-1]->widgets;
        eaPush(widgets,widget);
    }
    if (!editorUIWidgetHash)
        editorUIWidgetHash = stashTableCreateInt(16);
    stashIntAddPointer(editorUIWidgetHash, widget->ID, widget, 1);
    return widget->ID;
}

EditorUIWidget * getSubWidgetParentRecurse(EditorUIWidget * widget,int widgetID) {
    int i;
    EditorUIWidget * w=NULL;
    for (i=0;i<eaSize(&widget->subWidgets);i++) {
        int j;
        if (widget->subWidgets[i]->ID==widgetID) {
            return widget;
        }
        for (j=0;j<eaSize(&widget->subWidgets[i]->widgets);j++) {
            w=getSubWidgetParentRecurse(widget->subWidgets[i]->widgets[j],widgetID);
            if (w!=NULL)
                return w;
        }
    }
    return NULL;
}

EditorUIWidget * getSubWidgetParent(int widgetID) {
    int i;
    EditorUIWidget * ret=NULL;
    for (i=0;i<EDITORUI_MAX_WINDOWS;i++) {
        int j;
        EditorUIWindow * window=&editorUIWindows[i];
        for (j=0;j<eaSize(&window->widgets);j++) {
            ret=getSubWidgetParentRecurse(window->widgets[j],widgetID);
            if (ret)
                return ret;
        }
    }
    return NULL;
}

void editorUISetWidgetCallbackParam(int widgetID, void* callbackParam)
{
    EditorUIWidget* widget = editorUIWidgetFromID(widgetID);
    widget->callbackParam = callbackParam;
}

int editorUIShowByParentSelection(int widgetID)
{
    EditorUIWidget * w=getSubWidgetParent(widgetID);
    int value = -1;

    if (!w)
        return 0;

    switch (w->type)
    {
        xcase EDITORUI_RADIOBUTTONS:
        {
            EditorUIRadioButtons * widget=(EditorUIRadioButtons *)w;
            if (widget->myValue)
                value = *widget->myValue;
        }
        xcase EDITORUI_TABCONTROL:
        {
            EditorUITabControl * widget=(EditorUITabControl *)w;
            if (widget->myValue)
                value = *widget->myValue;
        }
        xcase EDITORUI_CHECKBOX:
        {
            EditorUICheckBox * widget=(EditorUICheckBox *)w;
            if (widget->myValue)
                value = *widget->myValue;
        }
        xcase EDITORUI_COMBOBOX:
        {
            EditorUIComboBox * widget=(EditorUIComboBox *)w;
            if (widget->myValue)
                value = *widget->myValue;
        }
        xcase EDITORUI_SLIDER:
        {
            EditorUISlider * widget=(EditorUISlider *)w;
            if (widget->myValue)
                value = *widget->myValue;
        }
        xcase EDITORUI_EDITSLIDER:
        {
            EditorUIEditSlider * widget=(EditorUIEditSlider *)w;
            if (widget->myValue)
                value = *widget->myValue;
        }
        xcase EDITORUI_LIST:
        {
            EditorUIList * widget=(EditorUIList *)w;
            if (widget->myValue)
                value = *widget->myValue;
        }
        xcase EDITORUI_SCROLLABLELIST:
        {
            EditorUIScrollableList * widget=(EditorUIScrollableList *)w;
            if (widget->myValue)
                value = *widget->myValue;
        }
    }

    if (value>=eaSize(&w->subWidgets))
        return 0;
    if (value<0)
        return 0;
    return w->subWidgets[value]->ID==widgetID;
}

/*********************************************************************************/

int editorUIDrawCallback(EditorUIWidget * w,int x,int y,int z,int top,int bottom,double scale,int hasFocus,int locked) {
    return 1;
}

int editorUIDrawNullWidget(EditorUIWidget * w,int x,int y,int z,int top,int bottom,double scale,int hasFocus,int locked) {
    return 0;
}

void destroyNullWidget(EditorUIWidget * w) {
}

int editorUIAddNullWidget(int ID,double elementsTall) {
    EditorUINullWidget * widget;
    EDITORUI_VALIDATE_ID_RET(ID);
    widget=calloc(1,sizeof(EditorUINullWidget));
    widget->myWidget.type=EDITORUI_NULL;
    widget->myWidget.draw=editorUIDrawNullWidget;
    widget->myWidget.window=ID;
    widget->myWidget.destroy=destroyNullWidget;
    widget->myWidget.elementsTall=elementsTall;
    return addWidgetToWindow(ID,(EditorUIWidget *)widget);
}

/*********************************************************************************/

int editorUIDrawCheckBox(EditorUIWidget * w,int x,int y,int z,int top,int bottom,double scale,int hasFocus,int locked) {
    EditorUICheckBox * widget=(void *)w;
    int change=0;

    int over=(hasFocus && !inpIsMouseLocked() && isOver(x+scale*EDITORUI_BUFFER,scale*(7+EDITORUI_BUFFER+str_wd(getEditorFont(),scale,scale,"%s",widget->myText)),y+scale*(EDITORUI_BUFFER+15),scale*(15)));

    if (y<top || y+20*scale>bottom)
        return 0;

    display_sprite(white_tex_atlas,x+scale*(EDITORUI_BUFFER),y+scale*(EDITORUI_BUFFER+2),z,scale*10.0/8.0,scale*10.0/8.0,CLR_CBOX_BORDER);
    display_sprite(white_tex_atlas,x+scale*(EDITORUI_BUFFER+1),y+scale*(EDITORUI_BUFFER+3),z,scale*(10.0-2.0)/8.0,scale*(10.0-2.0)/8.0,locked?CLR_WIDGET_BACK_LOCKED:CLR_WIDGET_BACK);
    setEditorFontActive(locked?CLR_TEXTLOCKED:CLR_TEXT);
    if (*widget->myValue)
        cprntEx(x+scale*(EDITORUI_BUFFER+4.5),y+scale*(8+EDITORUI_BUFFER),z, scale, scale, CENTER_X|CENTER_Y|NO_MSPRINT, "X");
    cprntEx(x+scale*(EDITORUI_BUFFER*2+5),y+scale*(EDITORUI_BUFFER+7.5),z,scale,scale,CENTER_Y|NO_MSPRINT,"%s",widget->myText);
    if(over && !locked && !inpIsMouseLocked()){// && !editorUIDisableClicks){
        if(inpEdge(INP_LBUTTON)){
            eatEdge(INP_LBUTTON);
            *widget->myValue=!*widget->myValue;
            change=1;
//            editorUIDisableClicks=1;
        }
    }
    return change;
}

void destroyCheckBox(EditorUIWidget * w) {
    EditorUICheckBox * widget=(EditorUICheckBox *)w;
    free(widget->myText);
}

int editorUIAddCheckBox(int ID,int * value,EditorUICallback callback,const char * text) {
    EditorUICheckBox * widget;
    int width;
    TTDrawContext * font=getEditorFont();
    EDITORUI_VALIDATE_ID_RET(ID);
    widget=calloc(1,sizeof(EditorUICheckBox));
    widget->myWidget.type=EDITORUI_CHECKBOX;
    widget->myWidget.draw=editorUIDrawCheckBox;
    widget->myWidget.window=ID;
    widget->myWidget.elementsTall=1;
    widget->myWidget.callback=callback;
    widget->myWidget.destroy=destroyCheckBox;
    widget->myText=strdup(text);
    widget->myValue=value;
    width=EDITORUI_BUFFER*2+str_wd(font,1,1,"%s",text)+5;
    if (editorUIWindows[ID].width<width)
        editorUIWindows[ID].width=width;
    return addWidgetToWindow(ID,(EditorUIWidget *)widget);
}

/*********************************************************************************/

int editorUIDrawCheckBoxList(EditorUIWidget * w,int xInput,int y,int z,int top,int bottom,double scale,int hasFocus,int locked) {
    EditorUICheckBoxList * widget=(void *)w;
    int i, n = eaSize(&widget->boxTexts);
    int numRows = editorUIWindows[w->window].width/widget->maxWidth;
    double xChange=(scale*editorUIWindows[w->window].width-scale*EDITORUI_BUFFER)/numRows;
    
    w->elementsTall = n/numRows + (n%numRows!=0);

    for (i = 0; i < n; i++)
    {
        int columnNum = i % numRows;
        double x = xInput + columnNum*xChange;
        int over=(hasFocus && !inpIsMouseLocked() && isOver(x+scale*EDITORUI_BUFFER,scale*(15+EDITORUI_BUFFER+str_wd(getEditorFont(),1.3*scale,1.3*scale,"%s",widget->boxTexts[i])),y+scale*(EDITORUI_BUFFER+15),scale*(15)));
        if (y<top || y+20*scale>bottom)
            continue;

        display_sprite(white_tex_atlas,x+scale*(EDITORUI_BUFFER),y+scale*(EDITORUI_BUFFER+2),z,scale*10.0/8.0,scale*10.0/8.0,CLR_CBOX_BORDER);
        display_sprite(white_tex_atlas,x+scale*(EDITORUI_BUFFER+1),y+scale*(EDITORUI_BUFFER+3),z,scale*(10.0-2.0)/8.0,scale*(10.0-2.0)/8.0,locked?CLR_WIDGET_BACK_LOCKED:CLR_WIDGET_BACK);
        setEditorFontActive(locked?CLR_TEXTLOCKED:CLR_TEXT);
        if (*widget->myValue & (1 << i))
            cprntEx(x+scale*(EDITORUI_BUFFER+4.5),y+scale*(8+EDITORUI_BUFFER),z, scale, scale, CENTER_X|CENTER_Y|NO_MSPRINT, "X");
        cprntEx(x+scale*(EDITORUI_BUFFER*2+5),y+scale*(EDITORUI_BUFFER+7.5),z,scale,scale,CENTER_Y|NO_MSPRINT,"%s",widget->boxTexts[i]);
        if(over && !inpIsMouseLocked() && !locked){// && !editorUIDisableClicks){
            if(inpEdge(INP_LBUTTON)){
                eatEdge(INP_LBUTTON);
                *widget->myValue ^= (1 << i);
//                editorUIDisableClicks=1;
            }
        }
        y += (columnNum==(numRows-1))*20*scale;
    }
    return 0;
}

void destroyCheckBoxList(EditorUIWidget * w) {
    EditorUICheckBoxList * widget=(EditorUICheckBoxList *)w;
    eaDestroyEx(&widget->boxTexts, NULL);
}

int editorUIAddCheckBoxList(int ID,int * val,EditorUICallback callback,const char ** textList) {
    EditorUICheckBoxList * widget;
    TTDrawContext * font=getEditorFont();
    int i, n = eaSize(&textList);
    EDITORUI_VALIDATE_ID_RET(ID);
    widget=calloc(1,sizeof(EditorUICheckBoxList));
    widget->myWidget.type=EDITORUI_CHECKBOXLIST;
    widget->myWidget.draw=editorUIDrawCheckBoxList;
    widget->myWidget.window=ID;
    widget->myWidget.elementsTall=n;
    widget->myWidget.callback=callback;
    widget->myWidget.destroy=destroyCheckBoxList;
    widget->maxWidth = 0;
    for (i = 0; i < n; i++)
    {
        int currWidth = EDITORUI_BUFFER*2+str_wd(font,1,1,"%s",textList[i])+5;
        eaPush(&widget->boxTexts, strdup(textList[i]));
        if (currWidth > widget->maxWidth)
            widget->maxWidth = currWidth;
    }

    widget->myValue=val;
    if (editorUIWindows[ID].width<widget->maxWidth)
        editorUIWindows[ID].width=widget->maxWidth;
    return addWidgetToWindow(ID,(EditorUIWidget *)widget);
}

/*********************************************************************************/

int editorUIDrawTabControl(EditorUIWidget * w,int x,int y,int z,int top,int bottom,double sc,int hasFocus,int locked) {
    EditorUITabControl * widget=(void *)w;
    int i, cnt = widget->displayCountCallback?widget->displayCountCallback():eaSize(&widget->myText);
    int hit=-1;
    int change=0,over;
    int ht=17*sc;
    x+=10*sc;
    MIN1(cnt,eaSize(&widget->myText));
    if (*widget->myValue>=cnt) {
        *widget->myValue=cnt-1;
        change=1;
    }

    // Cache the window width for deleting/renaming clues
    if (!widget->initialWidth)
        widget->initialWidth = editorUIWindows[widget->myWidget.window].width;

    for (i=0;i<cnt;i++) {
        CBox box;
        int wd=(EDITORUI_BUFFER+25+str_wd(getEditorFont(),1,1,"%s",widget->myText[i]))*sc;
        drawFrameTabStyle(PIX2,R4,x+sc*(EDITORUI_BUFFER),y+sc*(EDITORUI_BUFFER+2),z,wd,ht,1,CLR_FRAME,kFrameStyle_3D);
        if (i == *widget->myValue) {
            widget->openleft = x+sc*(EDITORUI_BUFFER);
            widget->openright = x+sc*(EDITORUI_BUFFER) + wd;
        }
        BuildCBox(&box,x+sc*7+sc*(EDITORUI_BUFFER),y+sc*(EDITORUI_BUFFER),wd-sc*14,ht);
        over=i==*widget->myValue || (hasFocus && !inpIsMouseLocked() && mouseCollision(&box));
        setEditorFontActive((i==*widget->myValue)?(locked?CLR_TEXTLOCKED:CLR_TEXT):CLR_TEXTGREY);
        cprntEx(x+sc*EDITORUI_BUFFER+wd*0.5f, y+sc*EDITORUI_BUFFER+10*sc,z,(over?1.1:1)*sc,(over?1.1:1)*sc,CENTER_X|CENTER_Y|NO_MSPRINT,"%s",widget->myText[i]);
        if (mouseClickHit(&box, MS_LEFT))
        {
            hit=i;
//            editorUIDisableClicks=1;
        }
        x+=wd;
    }
    if (hit!=-1) {
        *widget->myValue=hit;
        change=1;
    }
    return change;
}

void destroyTabControl(EditorUIWidget * w) {
    EditorUITabControl * widget=(EditorUITabControl*)w;
    eaDestroyEx(&widget->myText,NULL);
}

int editorUIAddTabControlFromEArray(int ID,int * val,EditorUICallback callback,int (*displayCountCallback)(void),char ** tabs) {
    EditorUITabControl * widget;
    int width = EDITORUI_BUFFER+20;
    int i, n = eaSize(&tabs);
    TTDrawContext * font=getEditorFont();
    EDITORUI_VALIDATE_ID_RET(ID);
    widget=calloc(1,sizeof(EditorUITabControl));
    widget->myWidget.type=EDITORUI_TABCONTROL;
    widget->myWidget.draw=editorUIDrawTabControl;
    widget->myWidget.window=ID;
    widget->myWidget.destroy=destroyTabControl;
    widget->myWidget.callback=callback;
    widget->myText=0;
    widget->displayCountCallback=displayCountCallback;
    eaCreate(&widget->myText);
    for (i = 0; i < n; i++) {
        eaPush(&widget->myText,strdup(tabs[i]));
        width+=EDITORUI_BUFFER+str_wd(font,1,1,"%s",widget->myText[i])+25;
        if (editorUIWindows[ID].width<width)
            editorUIWindows[ID].width=width;
    }
    widget->myWidget.elementsTall=1;
    widget->myValue=val;
    widget->initialWidth=0;
    return addWidgetToWindow(ID,(EditorUIWidget *)widget);
}

int editorUIAddTabControl(int ID,int * val,EditorUICallback callback,int (*displayCountCallback)(void),...) {
    va_list args;
    char ** ea=0;
    char * str;
    int ret;
    va_start(args,displayCountCallback);
    while ((str=va_arg(args,char *))!=NULL)
        eaPush(&ea,str);
    va_end(args);
    ret=editorUIAddTabControlFromEArray(ID,val,callback,displayCountCallback, ea);
    eaDestroy(&ea);
    return ret;
}

void editorUITabControlAddTab(int widgetID, char * tabString)
{
    EditorUITabControl * widget = (EditorUITabControl*)editorUIWidgetFromID(widgetID);
    TTDrawContext * font=getEditorFont();
    int windowID = widget->myWidget.window;
    int i, width = EDITORUI_BUFFER+20;

    eaPush(&widget->myText, strdup(tabString));
    for (i = 0; i < eaSize(&widget->myText); i++)
        width+=EDITORUI_BUFFER+str_wd(font,1,1,"%s",widget->myText[i])+25;
    if (editorUIWindows[windowID].width<width)
        editorUIWindows[windowID].width=width;
}

void editorUITabControlDeleteTab(int widgetID, int tabIndex)
{
    EditorUITabControl * widget = (EditorUITabControl*)editorUIWidgetFromID(widgetID);
    TTDrawContext * font=getEditorFont();
    int windowID = widget->myWidget.window;
    if (tabIndex >= 0 && tabIndex < eaSize(&widget->myText))
    {
        editorUIWindows[windowID].width-=(EDITORUI_BUFFER+str_wd(font,1,1,"%s",widget->myText[tabIndex])+25);
        if (editorUIWindows[windowID].width < widget->initialWidth)
            editorUIWindows[windowID].width = widget->initialWidth;
        free(widget->myText[tabIndex]);
        eaRemove(&widget->myText, tabIndex);
    }
}

void editorUITabControlRenameTab(int widgetID, int tabIndex, char * newName)
{
    EditorUITabControl * widget = (EditorUITabControl*)editorUIWidgetFromID(widgetID);
    TTDrawContext * font=getEditorFont();
    int windowID = widget->myWidget.window;
    if (tabIndex >= 0 && tabIndex < eaSize(&widget->myText))
    {
        int oldWidth = (EDITORUI_BUFFER+str_wd(font,1,1,"%s",widget->myText[tabIndex])+25);
        int newWidth = (EDITORUI_BUFFER+str_wd(font,1,1,"%s",newName)+25);
        editorUIWindows[windowID].width+=(newWidth-oldWidth);
        free(widget->myText[tabIndex]);
        widget->myText[tabIndex] = strdup(newName);
    }
}

/*********************************************************************************/

#define RADIO_CHECK_WIDTH    14
#define RADIO_Y_OVERLAY        2

static int drawUIRadioButton(float x, float y, float wd, float z, float sc, int flags, int color, int textcolor, char* text)
{
    AtlasTex * mark;
    CBox box;
    float px, py, pogsc = sc*0.8;
    int ret = D_NONE;
    int buttonhit;

    int locked = flags & BUTTON_LOCKED;
    int checked = flags & BUTTON_CHECKED;

    BuildCBox(&box, x, y-RADIO_Y_OVERLAY, wd, RADIO_CHECK_WIDTH + 2*RADIO_Y_OVERLAY);

    // pog
    px = x - (pogsc-sc)*RADIO_CHECK_WIDTH/2.0;    // correct for scale to center sprite
    py = y - (pogsc-sc)*RADIO_CHECK_WIDTH/2.0;
    if (checked)
    {
        mark = atlasLoadTexture( "Context_checkbox_filled.tga" );
        display_sprite( mark, px, py, z + 3, pogsc, pogsc, color );

        mark = atlasLoadTexture( "Context_checkbox_empty.tga" );
        display_sprite( mark, px, py, z + 2, pogsc, pogsc, color );
    }
    else
    {
        mark = atlasLoadTexture( "Context_checkbox_empty.tga" );
        display_sprite( mark, px, py, z + 2, pogsc, pogsc, color );
    }

    setEditorFontActive(textcolor);
    cprntEx(x+(R4+RADIO_CHECK_WIDTH)*sc, y+(RADIO_CHECK_WIDTH+2)*sc/2, z, sc, sc, CENTER_Y, text );

    buttonhit = mouseClickHit(&box, MS_LEFT);
    if (buttonhit)
    {
        if (locked)
            ret = D_MOUSELOCKEDHIT;
        else
            ret = D_MOUSEHIT;
//        editorUIDisableClicks=1;
    }

    return ret;
}

int editorUIDrawRadioButtons(EditorUIWidget * w,int x,int y,int z,int top,int bottom,double scale,int hasFocus,int locked) {
    EditorUIRadioButtons * widget=(void *)w;
    int i;
    int hit=-1;
    int change=0;
    if (!widget->displayHorizontal)
        y-=scale*20.0;
    for (i=0;i<eaSize(&widget->myText);i++) {
        if (!widget->displayHorizontal)
        {
            y+=scale*20.0;
            if (y<top || y+20*scale>bottom)
                continue;
        }
        if (drawUIRadioButton(x+scale*(EDITORUI_BUFFER),y+scale*(EDITORUI_BUFFER),100,z,scale,(locked?BUTTON_LOCKED:0)|((*widget->myValue==i)?BUTTON_CHECKED:0),locked?CLR_RADIO_LOCKED:CLR_WHITE,locked?CLR_TEXTLOCKED:CLR_TEXT,widget->myText[i])==D_MOUSEHIT)
            hit=i;
        if (widget->displayHorizontal)
        {
            int wd = EDITORUI_BUFFER+45+str_wd(getEditorFont(),1,1,"%s",widget->myText[i]);
            x+=wd*scale;
        }
    }
    if (hit!=-1) {
        *widget->myValue=hit;
        change=1;
    }
    return change;
}

void destroyRadioButtons(EditorUIWidget * w) {
    EditorUIRadioButtons * widget=(EditorUIRadioButtons*)w;
    eaDestroyEx(&widget->myText,NULL);
}


int editorUIAddRadioButtons(int ID,int displayHorizontal,int * value,EditorUICallback callback,...) {
    EditorUIRadioButtons * widget;
    va_list args;
    char * str;
    int width = EDITORUI_BUFFER;
    EDITORUI_VALIDATE_ID_RET(ID);
    widget=calloc(1,sizeof(EditorUIRadioButtons));
    widget->myWidget.type=EDITORUI_RADIOBUTTONS;
    widget->myWidget.draw=editorUIDrawRadioButtons;
    widget->myWidget.window=ID;
    widget->myWidget.destroy=destroyRadioButtons;
    widget->myWidget.callback=callback;
    widget->displayHorizontal=displayHorizontal;
    eaCreate(&widget->myText);
    va_start(args,callback);
    while ((str=va_arg(args,char *))!=NULL) {
        eaPush(&widget->myText,strdup(str));
        if (widget->displayHorizontal)
            width+=EDITORUI_BUFFER+str_wd(getEditorFont(),1,1,"%s",str)+45;
        else
            width=EDITORUI_BUFFER*2+str_wd(getEditorFont(),1,1,"%s",str)+15;
        if (editorUIWindows[ID].width<width)
            editorUIWindows[ID].width=width;
    }
    va_end(args);
    widget->myWidget.elementsTall=widget->displayHorizontal?1:eaSize(&widget->myText);
    widget->myValue=value;
    return addWidgetToWindow(ID,(EditorUIWidget *)widget);
}

/*
void editorUIAddRadioButtonsTest(int ID,int * value,void (*callback),...) {
    int foo;
    va_list args;

    foo=0;
    editorUIAddRadioButtons(ID,value,callback,)
}
*/
//#define thunder(ID,value,callback, ...) editorUIAddRadioButtons(ID,value,callback,##__VA_ARGS__,0);

int editorUIAddRadioButtonsFromEArray(int ID,int * value,EditorUICallback callback,char ** list) {
    EditorUIRadioButtons * widget;
    int width=0;
    int i=0;
    EDITORUI_VALIDATE_ID_RET(ID);
    widget=calloc(1,sizeof(EditorUIRadioButtons));
    widget->myWidget.type=EDITORUI_RADIOBUTTONS;
    widget->myWidget.draw=editorUIDrawRadioButtons;
    widget->myWidget.window=ID;
    widget->myWidget.destroy=destroyRadioButtons;
    widget->myWidget.callback=callback;
    eaCreate(&widget->myText);
    for (i=0;i<eaSize(&list);i++) {
        eaPush(&widget->myText,strdup(list[i]));
        width=EDITORUI_BUFFER*2+str_wd(getEditorFont(),1,1,"%s",list[i])+15;
    }
    if (editorUIWindows[ID].width<width)
        editorUIWindows[ID].width=width;
    widget->myWidget.elementsTall=eaSize(&widget->myText);
    widget->myValue=value;
    return addWidgetToWindow(ID,(EditorUIWidget *)widget);
}

int editorUIAddRadioButtonsFromArray(int ID,int * value,EditorUICallback callback,char ** list) {
    char ** ea=0;
    int i=0;
    int ret;
    while (list[i][0]) {
        eaPush(&ea,list[i]);
        i++;
    }
    ret=editorUIAddRadioButtonsFromEArray(ID,value,callback,ea);
    eaDestroy(&ea);
    return ret;
}

/*********************************************************************************/

int editorUIDrawStaticText(EditorUIWidget * w,int x,int y,int z,int top,int bottom,double scale,int hasFocus,int locked) {
    EditorUIStaticText* widget=(void *)w;

    if (y<top || y+20*scale>bottom)
        return 0;

    setEditorFontActive(locked?CLR_TEXTLOCKED:CLR_TEXT);
    cprntEx(x+scale*(EDITORUI_BUFFER),y+scale*(EDITORUI_BUFFER+7.5),z,scale,scale,CENTER_Y|NO_MSPRINT,"%s",widget->myText);

    return 0;
}

void destroyStaticText(EditorUIWidget * w) {
    EditorUIStaticText * widget=(EditorUIStaticText*)w;
    free(widget->myText);
}

int editorUIAddStaticText(int ID,const char * text) {
    EditorUIStaticText * widget;
    int width;
    EDITORUI_VALIDATE_ID_RET(ID);
    widget=calloc(1,sizeof(EditorUIStaticText));
    widget->myWidget.type=EDITORUI_STATICTEXT;
    widget->myWidget.draw=editorUIDrawStaticText;
    widget->myWidget.window=ID;
    widget->myWidget.elementsTall=1;
    widget->myWidget.destroy=destroyStaticText;
    widget->myWidget.canBeSelected=1;
    widget->myText=strdup(text);
    width=str_wd(getEditorFont(),1,1,"%s",widget->myText)+EDITORUI_BUFFER+EDITORUI_BUFFER;
    if (editorUIWindows[ID].width<width)
        editorUIWindows[ID].width=width;
    return addWidgetToWindow(ID,(EditorUIWidget *)widget);
}

/*********************************************************************************/

int editorUIDrawDynamicText(EditorUIWidget * w,int x,int y,int z,int top,int bottom,double scale,int hasFocus,int locked) {
    EditorUIDynamicText* widget=(void *)w;
    char buffer[1024];
    int width;

    if (!widget->textCallback)
        return 0;

    if (y<top || y+20*scale>bottom)
        return 0;

    widget->textCallback(buffer);

    setEditorFontActive(locked?CLR_TEXTLOCKED:CLR_TEXT);
    cprntEx(x+scale*(EDITORUI_BUFFER),y+scale*(EDITORUI_BUFFER+7.5),z,scale,scale,CENTER_Y|NO_MSPRINT,"%s",buffer);

    width = str_wd(getEditorFont(),scale,scale,"%s",buffer)+scale*EDITORUI_BUFFER*4;
    if (editorUIWindows[widget->myWidget.window].width<width)
        editorUIWindows[widget->myWidget.window].width=width;

    return 0;
}

void destroyDynamicText(EditorUIWidget * w) {
    EditorUIDynamicText * widget=(EditorUIDynamicText*)w;
}

int editorUIAddDynamicText(int ID,void (*textCallback)(char *)) {
    EditorUIDynamicText * widget;
    int width;
    char buffer[1024]="";
    EDITORUI_VALIDATE_ID_RET(ID);
    widget=calloc(1,sizeof(EditorUIDynamicText));
    widget->myWidget.type=EDITORUI_DYNAMICTEXT;
    widget->myWidget.draw=editorUIDrawDynamicText;
    widget->myWidget.window=ID;
    widget->myWidget.elementsTall=1;
    widget->myWidget.destroy=destroyDynamicText;
    widget->myWidget.canBeSelected=1;
    widget->textCallback=textCallback;
    width=90+EDITORUI_BUFFER+EDITORUI_BUFFER;
    if (editorUIWindows[ID].width<width)
        editorUIWindows[ID].width=width;
    return addWidgetToWindow(ID,(EditorUIWidget *)widget);
}

/*********************************************************************************/

int doTextEntry(UIEdit **editptr,char *old_value,char *new_value,int capacity,int x,int y,int z,int wd,int ht,float scale,float textScale,int hasFocus,int locked,int *selected,int forcefit)
{
    UIBox uibox;
    UIEdit *edit = *editptr;
    int change=0;
    float strht;
    char * estr;
    int over = hasFocus && !inpIsMouseLocked() && isOver(x,wd,y+ht,ht);
    int numLines=1;
    if (*editptr && eaSize(&(*editptr)->lines)>1 && *selected) {
        numLines=eaSize(&(*editptr)->lines);
        ht=ht*numLines/1.3+.5;
    }
    drawFrame(PIX2,R4,x,y,z+(numLines>1?2:0),wd,ht+2,1,*selected?CLR_SELECTED_FRAME:CLR_FRAME,locked?CLR_WIDGET_BACK_LOCKED:(*selected?CLR_SELECTED_WIDGET_BACK:CLR_WIDGET_BACK));

    if (strcmp(old_value,new_value))
        uiEditSetUTF8Text(edit,new_value);

    if(!edit) {
        edit=*editptr=uiEditCreateSimple(new_value, capacity, getEditorFont(), CLR_TEXT, editInputHandler);
        uiEditSetClicks(edit, "type2", "mouseover");
    } else {
        CBox box;
        BuildCBox( &box,x,y,wd,ht);
        //drawBox(&box, 100000, CLR_RED, 0);
        if (mouseClickHit(&box,MS_LEFT)) {
            uiEditTakeFocus(edit);
            if (selected)
                *selected=1;
//            editorUIDisableClicks=1;
        } else{
            BuildCBox(&box,0,0,2000,2000);
            if (mouseClickHit(&box,MS_LEFT))
            {
                uiEditReturnFocus(edit);
//                editorUIDisableClicks=1;
            }
        }
    }
    if (*selected)
        uiEditTakeFocus(edit);
    if (editorUILoseFocus || locked) {
        uiEditReturnFocus(edit);
        uiEditOnLostFocus(edit,edit);
        edit->canEdit=0;
    }

    if (forcefit)
    {
        int textwd = str_wd(getEditorFont(), textScale, textScale, "%s", new_value);
        while (textwd > wd - 12*scale)
        {
            textScale *= (wd-12*scale)/textwd;
            textwd = str_wd(getEditorFont(), textScale, textScale, "%s", new_value);
        }
        uiEditSetUTF8Text(edit,new_value);
    }

    uiEditSetFontScale(edit,textScale);

    strht = str_height(getEditorFont(), textScale, textScale, 0, "%s", new_value);
    uiBoxDefine(&uibox, x+6*scale, y+2*scale, wd-12*scale, ht*scale);
    uiEditSetBounds(edit, uibox);
    uiEditSetZ(edit, z+2);
    uiEditSetFont(edit, getEditorFont());
    edit->textColor = locked?CLR_TEXTLOCKED:CLR_TEXT;
    edit->password = 0;
    uiEditProcess(edit);

    estr=uiEditGetUTF8Text(edit);
    if (estr) {
        strcpy(new_value,estr);
        estrDestroy(&estr);
    } else if (edit) {
        new_value[0]=0;
    }
    change=strcmp(old_value,new_value);
    strcpy(old_value,new_value);
    return change;
}

int editorUIDrawTextEntry(EditorUIWidget * w,int x,int y,int z,int top,int bottom,double scale,int hasFocus,int locked) {
    EditorUITextEntry * widget=(void *)w;
    int width=150;

    if (y<top || y+20*scale>bottom)
        return 0;

    setEditorFontActive(locked?CLR_TEXTLOCKED:(w->selected?CLR_SELECTED_TEXT:CLR_TEXT));
    cprntEx(x+scale*(EDITORUI_BUFFER),y+scale*(EDITORUI_BUFFER+7.5),z,scale,scale,CENTER_Y|NO_MSPRINT,"%s",widget->myText);

    width=scale*editorUIWindows[w->window].width-(scale*editorUIWindows[w->window].startX+scale*EDITORUI_BUFFER);
    x+=scale*editorUIWindows[w->window].textEntryStartX-scale;

    return doTextEntry(&widget->myEditState, widget->myOldValue, widget->myValue, widget->myCapacity, x+scale*(EDITORUI_BUFFER-2),y+scale*(EDITORUI_BUFFER-1),z,width,scale*17.f,scale,scale,hasFocus,locked,&widget->myWidget.selected,0);
}

void destroyTextEntry(EditorUIWidget * w) {
    EditorUITextEntry * widget=(EditorUITextEntry*)w;
    free(widget->myText);
    free(widget->myOldValue);
    uiEditReturnFocus(widget->myEditState);
    uiEditDestroy(widget->myEditState);
}

int editorUIAddTextEntry(int ID,char * value,int capacity,EditorUICallback callback,const char * text) {
    EditorUITextEntry * widget;
    int width;
    EDITORUI_VALIDATE_ID_RET(ID);
    widget=calloc(1,sizeof(EditorUITextEntry));
    widget->myWidget.type=EDITORUI_TEXTENTRY;
    widget->myWidget.draw=editorUIDrawTextEntry;
    widget->myWidget.window=ID;
    widget->myWidget.elementsTall=1;
    widget->myWidget.destroy=destroyTextEntry;
    widget->myWidget.callback=callback;
    widget->myWidget.subWidgets=NULL;
    widget->myWidget.canBeSelected=1;
    widget->myText=strdup(text);
    widget->myValue=value;
    widget->myCapacity=capacity;
    widget->myOldValue=malloc(widget->myCapacity+1);
    widget->myOldValue[0]=0;
    widget->myEditState=NULL;
    widget->myWidget.startX=str_wd(getEditorFont(),1,1,"%s",widget->myText)+EDITORUI_BUFFER+EDITORUI_BUFFER;
    width=str_wd(getEditorFont(),1,1,"%s",widget->myText)+EDITORUI_BUFFER+EDITORUI_BUFFER*2+150;
    if (editorUIWindows[ID].width<width)
        editorUIWindows[ID].width=width;
    return addWidgetToWindow(ID,(EditorUIWidget *)widget);
}

/*********************************************************************************/

int editorUIDrawScrollableList(EditorUIWidget * w,int xInput,int yInput,int z,int top,int bottom,double scale,int hasFocus,int locked) {
    EditorUIScrollableList * widget=(void *)w;
    float yDist = 20*scale;
    float dontCare, currY;
    int i, idontCare;
    int startIndex = 0;
    int numPerColumn = widget->height/20;
    int numRowsToShow = 0;
    int x = xInput;
    int y = yInput;
    int numElements = 0;
    int currRow = 0;
    int changed = 0;
    CBox sbox;
    if (y<top || y+20*scale>bottom)
        return 0;

    window_getDims(editorUIGetWindow(widget->myWidget.window), &dontCare, &currY, &dontCare, &dontCare, &dontCare, &dontCare, &idontCare, &idontCare);

    // calc the max width, num elements, etc
    if (!widget->strWidth)
    {
        for (i=0;i<eaSize(&widget->myStrings);i++)
        {
            int currWidth = str_wd(getEditorFont(),scale,scale,"%s",widget->myStrings[i])+EDITORUI_BUFFER+EDITORUI_BUFFER;
            if (currWidth > widget->strWidth)
                widget->strWidth = currWidth;
        }
        widget->myWidget.elementsTall = ((widget->height+30)*scale)/yDist;
    }

    // Scrollbar to decide offset of the files
    numElements = eaSize(&widget->myStrings)/widget->myWidget.elementsTall;
    startIndex = widget->scrollBar->offset/((widget->width-29)*scale);
    numRowsToShow = widget->width/widget->strWidth + 1;
    BuildCBox(&sbox, xInput+7*scale, yInput, (widget->width-PIX2)*scale, (widget->height+20)*scale);
    doScrollBar(widget->scrollBar, (widget->width-29)*scale, numElements*(widget->width-29)*scale, 19*scale, yInput+233*scale-currY, z+10, &sbox, 0);

    // Show all of the strings, and put them in a "window"
    x += PIX3*scale;
    y += 10*scale;
    drawFrame(PIX2,R4,xInput+scale*5,yInput+scale*(EDITORUI_BUFFER),z-1,widget->width*scale,(widget->height+20)*scale,1,CLR_FRAME,locked?CLR_WIDGET_BACK_LOCKED:CLR_WIDGET_BACK);
    set_scissor(1);
    scissor_dims(xInput+7*scale, yInput, (widget->width-PIX2)*scale, (widget->height+20)*scale);
    for (i = startIndex*numPerColumn; i < (startIndex+numRowsToShow)*numPerColumn; i++)
    {
        int textX = x+scale*8;
        int textY = y+scale*(EDITORUI_BUFFER+14.3);
        int over = isOver(textX, widget->strWidth, textY, yDist);

        if (i >= eaSize(&widget->myStrings))
            break;

        // Did they click on it?
        if (over)
        {
            CBox box;
            BuildCBox(&box,textX,textY-yDist,widget->strWidth,yDist);
            
            if (mouseClickHit(&box, MS_LEFT))
            {
                *widget->myValue=widget->lastClicked=i;
                changed=1;
//                editorUIDisableClicks=1;
            }
        }

        if (i == widget->lastClicked)
            setEditorFontActive(CLR_WHITE);
        else
            setEditorFontActive(CLR_TEXT);
        cprntEx(textX,textY,z,scale,scale,0,"%s",widget->myStrings[i]);
        y += yDist;

        if (++currRow >= numPerColumn)
        {
            y = yInput + 10*scale;
            x += widget->strWidth;
            currRow = 0;
        }
    }
    set_scissor(0);
    return changed;
}

void destroyScrollableList(EditorUIWidget * w) {
    EditorUIScrollableList * widget=(EditorUIScrollableList*)w;
    int i;
    for (i = 0; i < eaSize(&widget->myStrings); i++)
    {
        SAFE_FREE(widget->myStrings[i]);
    }
    SAFE_FREE(widget->scrollBar);
}

int editorUIAddScrollableList(int ID,int * val,char ** listItems,int height,int width,EditorUICallback callback) {
    EditorUIScrollableList * widget;
    int i, n = eaSize(&listItems);
    EDITORUI_VALIDATE_ID_RET(ID);
    widget=calloc(1,sizeof(EditorUIScrollableList));
    widget->myWidget.type=EDITORUI_SCROLLABLELIST;
    widget->myWidget.draw=editorUIDrawScrollableList;
    widget->myWidget.window=ID;
    widget->myWidget.destroy=destroyScrollableList;
    widget->myWidget.callback=callback;
    widget->myWidget.canBeSelected=1;
    widget->scrollBar = calloc(1,sizeof(ScrollBar));
    widget->scrollBar->horizontal=1;
    widget->scrollBar->wdw = editorUIGetWindow(ID);
    widget->width = width;
    widget->myValue = val;
    widget->height = height;

    for (i=0; i<n; i++)
        eaPush(&widget->myStrings, strdup(listItems[i]));
    
    if (editorUIWindows[ID].width<width)
        editorUIWindows[ID].width=width;
    return addWidgetToWindow(ID,(EditorUIWidget *)widget);
}

/*********************************************************************************/

void treeAddressPush(TreeElementAddress *address, int element)
{
    eaiPush(&address->address, element);
}

int treeAddressPop(TreeElementAddress *address)
{
    return eaiPop(&address->address);
}

void freeTreeAddressData(TreeElementAddress *address)
{
    eaiDestroy(&address->address);
    address->address = 0;
}

static void freeTreeAddress(TreeElementAddress *address)
{
    freeTreeAddressData(address);
    free(address);
}

static TreeElementAddress *dupTreeAddress(const TreeElementAddress *address)
{
    TreeElementAddress *out = calloc(1,sizeof(*address));
    int i;

    for (i = 0; i < eaiSize(&address->address); i++)
        treeAddressPush(out, address->address[i]);
    return out;
}

static void copyTreeAddress(TreeElementAddress *dest, const TreeElementAddress *src)
{
    int i;
    freeTreeAddressData(dest);
    for (i = 0; i < eaiSize(&src->address); i++)
        treeAddressPush(dest, src->address[i]);
}

static int sameTreeAddress(const TreeElementAddress *a1, const TreeElementAddress *a2)
{
    int i;
    if (eaiSize(&a1->address) != eaiSize(&a2->address))
        return 0;
    for (i = 0; i < eaiSize(&a1->address); i++)
    {
        if (a1->address[i] != a2->address[i])
            return 0;
    }
    return 1;
}

static void treeElementOpen(EditorUITreeControl *tree, TreeElement *elem, TreeElementAddress *address)
{
    char name[1024];
    int i;

    if (eaSize(&elem->children))
        return;
    
    for (i = 0; i < elem->child_count; i++)
    {
        treeAddressPush(address, i);
        eaPush(&elem->children, allocTreeElement());
        name[0] = 0;
        elem->children[i]->child_count = tree->elementCallback(name, address);
        if (name[0])
            elem->children[i]->name = strdup(name);
        treeAddressPop(address);
    }
}

static void treeElementClose(TreeElement *elem)
{
    if (elem->children)
        eaDestroyEx(&elem->children, freeTreeElement);
}

static void treeCloseAll(EditorUITreeControl *tree)
{
    char name[1024];
    TreeElementAddress address={0};

    // the tree structure is filled as it is opened in the UI
    freeTreeElement(tree->root_element);

    tree->root_element = allocTreeElement();
    tree->root_element->child_count = tree->elementCallback(name, &address);
    treeElementOpen(tree, tree->root_element, &address);
    freeTreeAddressData(&address);
}

static TreeElement *treeOpenToAddress(EditorUITreeControl *tree, const TreeElementAddress *address, int *idx)
{
    TreeElementAddress local_address={0};
    TreeElement *elem;
    int i;

    if (!tree->multi_open || !tree->root_element)
        treeCloseAll(tree);

    if (idx)
        *idx = 0;
    elem = tree->root_element;
    for (i = 0; i < eaiSize(&address->address); i++)
    {
        if (!elem || address->address[i] >= elem->child_count)
        {
            freeTreeAddressData(&local_address);
            return 0;
        }
        treeElementOpen(tree, elem, &local_address);
        treeAddressPush(&local_address, address->address[i]);
        elem = elem->children[address->address[i]];
        if (idx && i + 1 < eaiSize(&address->address))
            *idx += address->address[i] + 1;
    }
    treeElementOpen(tree, elem, &local_address);
    freeTreeAddressData(&local_address);
    return elem;
}

static TreeElement *treeTraverseToIndex(TreeElement *parent, int *idx, TreeElementAddress *address)
{
    int i;

    if (*idx == 0)
        return parent;

    (*idx)--;
    if (eaSize(&parent->children) == 0)
        return 0;

    for (i = 0; i < parent->child_count; i++)
    {
        TreeElement *elem;
        treeAddressPush(address, i);
        elem = treeTraverseToIndex(parent->children[i], idx, address);
        if (elem)
            return elem;
        treeAddressPop(address);
    }

    return 0;
}

static TreeElement *treeGetOpenElement(EditorUITreeControl *tree, int idx, TreeElementAddress *address)
{
    if (idx < 0)
        return 0;
    idx++;
    return treeTraverseToIndex(tree->root_element, &idx, address);
}

static int treeGetNumOpenElements(TreeElement *elem, int depth, float *total_width)
{
    int i, count = 0;
    if (eaSize(&elem->children) == 0)
        return 0;
    for (i = 0; i < elem->child_count; i++)
    {
        float width = 15 * depth + str_wd(getEditorFont(), 1, 1, "%s", elem->children[i]->name);
        MAX1(*total_width, width);
        count += 1 + treeGetNumOpenElements(elem->children[i], depth+1, total_width);
    }
    return count;
}

static void treeUnselectElement(EditorUITreeControl *tree, const TreeElementAddress *address)
{
    int i;
    for (i = 0; i < eaSize(&tree->selected); i++)
    {
        if (sameTreeAddress(tree->selected[i], address))
        {
            freeTreeAddress(tree->selected[i]);
            eaRemove(&tree->selected, i);
            i--;
        }
    }
}

static void treeSelectElement(EditorUITreeControl *tree, const TreeElementAddress *address)
{
    int i;
    if (!tree->multi_select)
        eaClearEx(&tree->selected, freeTreeAddress);
    for (i = 0; i < eaSize(&tree->selected); i++)
    {
        if (sameTreeAddress(tree->selected[i], address))
            return;
    }
    eaPush(&tree->selected, dupTreeAddress(address));
}

static int treeElementIsSelected(EditorUITreeControl *tree, const TreeElementAddress *address)
{
    int i;
    for (i = 0; i < eaSize(&tree->selected); i++)
    {
        if (sameTreeAddress(tree->selected[i], address))
            return 1;
    }
    return 0;
}

// Search through our selected list and remove any that start with the address given
static void treeRemoveSelectedChildren(EditorUITreeControl *tree, TreeElementAddress *address)
{
    int i, n = eaSize(&tree->selected);
    int j, addrLength = eaiSize(&address->address);

    for (i = n - 1; i >=0; i--)
    {
        TreeElementAddress* element = tree->selected[i];
        int remove = 1;
        if (eaiSize(&element->address) < addrLength)
            continue;

        for (j = 0; j < addrLength; j++)
            if (element->address[j] != address->address[j])
                remove = 0;

        if (remove)
        {
            freeTreeAddress(tree->selected[i]);
            eaRemove(&tree->selected, i);
        }
    }
}

// LH: This works for what I used it for but you should double check it does what you need it to if you use it
static void treeElementRefresh(EditorUITreeControl *tree, TreeElement *elem, TreeElementAddress *address)
{
    char name[1024];
    int i;
    int oldChildCount = elem->child_count;
    int newChildCount = tree->elementCallback(name, address);

    // Only process a tree element if it is currently open(has children)
    if (eaSize(&elem->children) || (elem->child_count != oldChildCount))
    {
        elem->child_count = newChildCount;

        // Number of children changed, deselect all that begin with the current address
        if (elem->child_count != oldChildCount)
        {
            treeElementClose(elem);
            treeRemoveSelectedChildren(tree, address);
            treeElementOpen(tree, elem, address);
        }
        else if (eaSize(&elem->children))    // Otherwise check deeper
        {
            for (i = 0; i < elem->child_count; i++)
            {
                treeAddressPush(address, i);
                treeElementRefresh(tree, elem->children[i], address);
                treeAddressPop(address);
            }
        }
    }
 }

void editorUITreeControlSelect(int widgetID, const TreeElementAddress* select_address)
{
    EditorUITreeControl * widget = (EditorUITreeControl*)editorUIWidgetFromID(widgetID);
    if (widget)
    {
        treeSelectElement(widget, select_address);
        treeOpenToAddress(widget, select_address, 0);
    }
}

void editorUITreeControlDeselect(int widgetID, const TreeElementAddress* select_address)
{
    EditorUITreeControl * widget = (EditorUITreeControl*)editorUIWidgetFromID(widgetID);
    if (widget)
        treeUnselectElement(widget, select_address);
}

void editorUITreeControlRefreshList(int widgetID)
{
    EditorUITreeControl * widget = (EditorUITreeControl*)editorUIWidgetFromID(widgetID);
    if (widget)
    {
        TreeElementAddress address = {0};
        treeElementRefresh(widget, widget->root_element, &address);
    }
}

int editorUITreeControlGetSelected(int widgetID, TreeElementAddress *addresses, int max_count)
{
    int i;
    EditorUITreeControl * widget = (EditorUITreeControl*)editorUIWidgetFromID(widgetID);

    if (!widget)
        return 0;
    
    if (!eaSize(&widget->selected))
        return 0;

    for (i = 0; i < max_count && i < eaSize(&widget->selected); i++)
        copyTreeAddress(&addresses[i], widget->selected[i]);

    return eaSize(&widget->selected);
}

int editorUIDrawTreeControl(EditorUIWidget * w,int xInput,int yInput,int z,int top,int bottom,double scale,int hasFocus,int locked)
{
    EditorUITreeControl * widget=(void *)w;
    float x,y,width,total_width=0,horiz_offset;
    int i,changed=0,displaySize=widget->myWidget.elementsTall*1.25,numElements,startIndex;
    CBox box;
    UIBox uibox={0};
    TreeElementAddress address={0};
    width=scale*editorUIWindows[w->window].width-scale*EDITORUI_BUFFER;
    drawFrame(PIX2,R4,xInput+scale*EDITORUI_BUFFER,yInput+scale*EDITORUI_BUFFER,z-1,width*scale,(widget->myWidget.elementsTall*20-EDITORUI_BUFFER)*scale,1,CLR_FRAME,locked?CLR_WIDGET_BACK_LOCKED:CLR_WIDGET_BACK);
    setEditorFontActive(locked?CLR_TEXTLOCKED:CLR_TEXT);
    x = xInput+scale*(EDITORUI_BUFFER+8);
    y = yInput+scale*(EDITORUI_BUFFER+13);


    uibox.x = xInput+scale*(8+EDITORUI_BUFFER);
    uibox.y = yInput+scale*EDITORUI_BUFFER;
    uibox.width = (width-16)*scale;
    uibox.height = (widget->myWidget.elementsTall*20-EDITORUI_BUFFER)*scale;

    // Scrollbars
    numElements = treeGetNumOpenElements(widget->root_element, 0, &total_width);

    doScrollBar(widget->scrollBarV, displaySize*13*scale, numElements*13*scale, xInput+width+scale*(EDITORUI_BUFFER+1.5), yInput+scale*(EDITORUI_BUFFER+EDITORUI_BUFFER+6), z+10, 0, &uibox);
    startIndex = widget->scrollBarV->offset/(13*scale);


    doScrollBar(widget->scrollBarH, (width-EDITORUI_BUFFER-EDITORUI_BUFFER-12)*scale, total_width*scale, scale*(6+EDITORUI_BUFFER+EDITORUI_BUFFER), scale*(EDITORUI_BUFFER)+(widget->myWidget.elementsTall*20-EDITORUI_BUFFER+1)*scale, z+10, 0, &uibox);
    horiz_offset = -widget->scrollBarH->offset/scale;

    x += horiz_offset;


    clipperPushRestrict(&uibox);

    for (i = 0; i < displaySize; i++)
    {
        int depth;
        TreeElementAddress address = {0};
        TreeElement *element = treeGetOpenElement(widget, startIndex+i, &address);
        if (!element)
        {
            freeTreeAddressData(&address);
            break;
        }

        depth = eaiSize(&address.address) - 1;

        if (element->child_count && element->children)
        {
            BuildCBox(&box,15*depth*scale+x,y-5*scale,10*scale,10*scale);
            cprntEx(15*depth*scale+x+2*scale,y+3*scale,z,scale,scale,NO_MSPRINT, "-");
            if (mouseClickHit(&box, MS_LEFT) && !locked)
            {
                treeElementClose(element);
//                editorUIDisableClicks=1;
            }
        }
        else if (element->child_count)
        {
            float twidth = element->name?str_wd(getEditorFont(), scale, scale, "%s", element->name):100*scale;
            BuildCBox(&box,15*depth*scale+x,y-5*scale,10*scale+twidth+scale*4,10*scale);
            cprntEx(15*depth*scale+x,y+3*scale,z,scale,scale,NO_MSPRINT, "+");
            if (mouseClickHit(&box, MS_LEFT) && !locked)
            {
                element = treeOpenToAddress(widget, &address, 0);
//                editorUIDisableClicks=1;
            }
        }
        else
        {
            float twidth = element->name?str_wd(getEditorFont(), scale, scale, "%s", element->name):100*scale;
            BuildCBox(&box,15*depth*scale+x+scale*8,y-7.5*scale,twidth+scale*6,15*scale);
            if (mouseClickHit(&box, MS_LEFT) && !locked)
            {
                if (treeElementIsSelected(widget, &address))
                    treeUnselectElement(widget, &address);
                else
                    treeSelectElement(widget, &address);
                changed = 1;
//                editorUIDisableClicks=1;
            }
        }
        if (element && element->name)
        {
            if (treeElementIsSelected(widget, &address))
            {
                float twidth = str_wd(getEditorFont(), scale, scale, "%s", element->name);
                display_sprite(white_tex_atlas, 15*depth*scale+x+scale*8, y-7.5*scale, z, (twidth+scale*6)/8.f,15*scale/8.f, 0x80b0ffff);
            }
            cprntEx(15*depth*scale+x+scale*10, y, z+1, scale, scale, CENTER_Y|NO_MSPRINT, "%s", element->name);
        }

        y += scale*15;
        freeTreeAddressData(&address);
    }

    clipperPop();

    return changed;
}

void destroyTreeControl(EditorUIWidget * w) {
    EditorUITreeControl * widget=(EditorUITreeControl*)w;
    SAFE_FREE(widget->scrollBarV);
    freeTreeElement(widget->root_element);
}

int editorUIAddTreeControl(int ID,EditorUICallback callback,int (*elementCallback)(char*namebuf, const TreeElementAddress *elem),int multi_select,int multi_open,const TreeElementAddress *init_selected)
{
    float width;
    EditorUITreeControl * widget;
    EDITORUI_VALIDATE_ID_RET(ID);
    widget=calloc(1,sizeof(EditorUITreeControl));
    widget->myWidget.type=EDITORUI_TREECONTROL;
    widget->myWidget.draw=editorUIDrawTreeControl;
    widget->myWidget.window=ID;
    widget->myWidget.destroy=destroyTreeControl;
    widget->myWidget.callback=callback;
    widget->myWidget.subWidgets=NULL;
    widget->myWidget.canBeSelected=1;
    widget->myWidget.selected=0;
    widget->myWidget.elementsTall=10;
    widget->elementCallback=elementCallback;
    widget->multi_select=multi_select;
    widget->multi_open=multi_open;
    widget->scrollBarV=calloc(1,sizeof(ScrollBar));
    widget->scrollBarV->wdw = -1;//editorUIGetWindow(ID);
    widget->scrollBarV->use_color = 1;
    widget->scrollBarV->color = CLR_WHITE;
    widget->scrollBarH=calloc(sizeof(ScrollBar),1);
    widget->scrollBarH->wdw = editorUIGetWindow(ID);
    widget->scrollBarH->horizontal = 1;
    widget->scrollBarH->use_color = 1;
    widget->scrollBarH->color = CLR_WHITE;
    treeCloseAll(widget);
    if (init_selected)
    {
        int startIndex;
        treeOpenToAddress(widget, init_selected, &startIndex);
        treeSelectElement(widget, init_selected);
        widget->scrollBarV->offset = (startIndex - 1) * 13;
        widget->scrollBarH->offset = 999999;
    }
    width=250;
    if (editorUIWindows[ID].width<width)
        editorUIWindows[ID].width=width;
    return addWidgetToWindow(ID,(EditorUIWidget *)widget);
}

/*********************************************************************************/

int editorUIDrawSlider(EditorUIWidget * w,int x,int y,int z,int top,int bottom,double scale,int hasFocus,int locked) {
    EditorUISlider * widget=(void *)w;
    int width=editorUIWindows[w->window].width-editorUIWindows[w->window].startX-SLIDER_TEXTSPACE+10;
    float val;
    char str[16];
    float tempSliderValue;
    int i;
    int length;
    int cwidth;
    if (y<top || y+40*scale>bottom)
        return 0;
    if (*widget->myValue<widget->min)
        *widget->myValue=widget->min;
    if (*widget->myValue>widget->max)
        *widget->myValue=widget->max;
    val=*widget->myValue;
    tempSliderValue=(val-widget->min)/(widget->max-widget->min)*2-1;
    setEditorFontActive(locked?CLR_TEXTLOCKED:CLR_TEXT);
    cprntEx(x+scale*(EDITORUI_BUFFER),y+scale*(EDITORUI_BUFFER+8),z,scale,scale,CENTER_Y|NO_MSPRINT,"%s",widget->myText);
    if (widget->max - widget->min < 10 && widget->integer)
    {
        float ticks[10];
        int count = 0;
        for (i = widget->min; i <= widget->max; i++)
            ticks[count++] = (i-widget->min)/(widget->max-widget->min)*2-1;;
        widget->sliderValue=doSliderTicks(x+scale*(width/2+7+editorUIWindows[widget->myWidget.window].sliderStartX),y+scale*(EDITORUI_BUFFER+7.5),z,width/(scale*OLDUISCALE),scale*OLDUISCALE,scale*OLDUISCALE,tempSliderValue,(int)((void *)widget->myValue),locked?CLR_SLIDER_LOCKED:CLR_WHITE,locked?BUTTON_LOCKED:0,count,ticks);
    }
    else
    {
        widget->sliderValue=doSlider(x+scale*(7+width/2+editorUIWindows[widget->myWidget.window].sliderStartX),y+scale*(EDITORUI_BUFFER+7.5),z,width/(scale*OLDUISCALE),scale*OLDUISCALE,scale*OLDUISCALE,tempSliderValue,(int)((void *)widget->myValue),locked?CLR_SLIDER_LOCKED:CLR_WHITE,locked?BUTTON_LOCKED:0);
    }
    *widget->myValue=((widget->sliderValue+1)/2.0)*(widget->max-widget->min)+widget->min;
    if (widget->integer) {
        int val;
        *widget->myValue=val=round(*widget->myValue);
        sprintf(str,"%d",val);
    } else
        sprintf(str,"%.3f",*widget->myValue);
    length=strlen(str);
    cwidth=str_wd(getEditorFont(),scale,scale,"%c",'2');
    for (i=0;i<length;i++) {
        char c[2];
        c[0]=str[i];
        c[1]=0;
        setEditorFontActive(locked?CLR_TEXTLOCKED:CLR_TEXT);
        cprntEx(x+scale*(width+SLIDER_TEXTSPACE-10-cwidth*(length-i+1))+scale*editorUIWindows[widget->myWidget.window].startX,y+scale*(EDITORUI_BUFFER+7.5),z,scale,scale,CENTER_Y|NO_MSPRINT,"%s",c);
    }
    return fabs(val-*widget->myValue)>.00001; // is it possible to move so slow you never trigger this?
}

void destroySlider(EditorUIWidget * w) {
    EditorUISlider * widget=(EditorUISlider*)w;
    free(widget->myText);
}

int editorUIAddSlider(int ID,float * value,float min,float max,int integer,EditorUICallback callback,const char * text) {
    EditorUISlider * widget;
    int width;
    EDITORUI_VALIDATE_ID_RET(ID);
    widget=calloc(1,sizeof(EditorUISlider));
    widget->myWidget.type=EDITORUI_SLIDER;
    widget->myWidget.draw=editorUIDrawSlider;
    widget->myWidget.window=ID;
    widget->myWidget.elementsTall=1;
    widget->myWidget.destroy=destroySlider;
    widget->myWidget.callback=callback;
    widget->myWidget.subWidgets=NULL;
    widget->myWidget.canBeSelected=0;
    widget->myText=strdup(text);
    widget->myValue=value;
    widget->min=min;
    widget->max=max;
    widget->integer=integer;
    MAX1(*widget->myValue,widget->min);
    MIN1(*widget->myValue,widget->max);
    if (widget->max<widget->min+.001)        // to avoid crappy cases and division by zero
        widget->max=widget->min+.001;
    widget->sliderValue=(*widget->myValue-widget->min)/(widget->max-widget->min)*2-1;
    widget->myWidget.startX=str_wd(getEditorFont(),1,1,"%s",widget->myText)+EDITORUI_BUFFER;
    width=EDITORUI_BUFFER*2+str_wd(getEditorFont(),1,1,"%s",text)+150;
    if (editorUIWindows[ID].width<width)
        editorUIWindows[ID].width=width;
    return addWidgetToWindow(ID,(EditorUIWidget *)widget);
}

/*********************************************************************************/
int editorUIDrawEditSlider(EditorUIWidget * w,int x,int y,int z,int top,int bottom,double scale,int hasFocus,int locked) {
    EditorUIEditSlider * widget=(void *)w;
    int width=editorUIWindows[w->window].width-editorUIWindows[w->window].startX-SLIDER_TEXTSPACE;
    float val, newval;
    char str[16];
    float tempSliderValue;
    int i;
    int changed = 0;
    int externalTextChange = 0;
    w->isUserInteracting = 0;
    if (y<top || y+40*scale>bottom)
        return 0;

    MIN1(*widget->myValue,widget->max);
    MAX1(*widget->myValue,widget->min);

    // slider
    val=*widget->myValue;
    MIN1(val,widget->smax);
    MAX1(val,widget->smin);
    tempSliderValue=(val-widget->smin)/(widget->smax-widget->smin)*2-1;
    setEditorFontActive(locked?CLR_TEXTLOCKED:CLR_TEXT);
    cprntEx(x+scale*(EDITORUI_BUFFER),y+scale*(EDITORUI_BUFFER+7.5),z,scale,scale,CENTER_Y|NO_MSPRINT,"%s",widget->myText);
    if (widget->smax - widget->smin < 10 && widget->integer)
    {
        float ticks[10];
        int count = 0;
        for (i = widget->smin; i <= widget->smax; i++)
            ticks[count++] = (i-widget->smin)/(widget->smax-widget->smin)*2-1;;
        widget->sliderValue=doSliderTicks(x+scale*(7+width/2+editorUIWindows[widget->myWidget.window].sliderStartX),y+scale*(EDITORUI_BUFFER+7.5),z,width/(scale*OLDUISCALE),scale*OLDUISCALE,scale*OLDUISCALE,tempSliderValue,(int)((void *)widget->myValue),locked?CLR_SLIDER_LOCKED:CLR_WHITE,locked?BUTTON_LOCKED:0,count,ticks);
    }
    else
    {
        widget->sliderValue=doSlider(x+scale*(7+width/2+editorUIWindows[widget->myWidget.window].sliderStartX),y+scale*(EDITORUI_BUFFER+7.5),z,width/(scale*OLDUISCALE),scale*OLDUISCALE,scale*OLDUISCALE,tempSliderValue,(int)((void *)widget->myValue),locked?CLR_SLIDER_LOCKED:CLR_WHITE,locked?BUTTON_LOCKED:0);
    }

    if (fabs(tempSliderValue - widget->sliderValue) > 0.00001)
        w->isUserInteracting = 1;

    newval=((widget->sliderValue+1)/2.0)*(widget->smax-widget->smin)+widget->smin;
    if (widget->integer)
        newval=round(newval);
    changed = fabs(val-newval)>.00001; // is it possible to move so slow you never trigger this?
    if (changed)
        *widget->myValue = newval;

    if (widget->integer)
        sprintf(str,"%d",(int)(*widget->myValue));
    else
        sprintf(str,"%.3f",*widget->myValue);

    // text entry
    x+=scale*(width+8)+scale*editorUIWindows[widget->myWidget.window].startX;
    width=scale*(SLIDER_TEXTSPACE-5);
    externalTextChange = !!strcmp(str, widget->myOldString);
    if (doTextEntry(&widget->myEditState,widget->myOldString,str,64,x,y+8*scale,z,width,17*scale,scale,scale,hasFocus,locked,&widget->myWidget.selected,1))
    {
        *widget->myValue = atof(str);
        MIN1(*widget->myValue,widget->emax);
        MAX1(*widget->myValue,widget->emin);
        if (widget->integer)
            *widget->myValue=round(*widget->myValue);
        changed = 1;
        w->isUserInteracting |= !externalTextChange;
    }

    return changed;
}

void destroyEditSlider(EditorUIWidget * w) {
    EditorUIEditSlider * widget=(EditorUIEditSlider*)w;
    free(widget->myText);
    uiEditReturnFocus(widget->myEditState);
    uiEditDestroy(widget->myEditState);
}

int editorUIAddEditSlider(int ID,float * val,float smin,float smax,float emin, float emax, int integer,EditorUICallback callback,const char * text) {
    EditorUIEditSlider * widget;
    int width;
    EDITORUI_VALIDATE_ID_RET(ID);
    widget=calloc(1,sizeof(EditorUIEditSlider));
    widget->myWidget.type=EDITORUI_EDITSLIDER;
    widget->myWidget.draw=editorUIDrawEditSlider;
    widget->myWidget.window=ID;
    widget->myWidget.elementsTall=1;
    widget->myWidget.destroy=destroyEditSlider;
    widget->myWidget.callback=callback;
    widget->myWidget.subWidgets=NULL;
    widget->myWidget.canBeSelected=0;
    widget->myText=strdup(text);
    widget->myValue=val;
    widget->emin=emin;
    widget->emax=emax;
    widget->smin=smin;
    widget->smax=smax;
    widget->min=MIN(emin,smin);
    widget->max=MAX(emax,smax);
    widget->integer=integer;
    MAX1(*widget->myValue,widget->min);
    MIN1(*widget->myValue,widget->max);
    if (widget->max<widget->min+.001)        // to avoid crappy cases and division by zero
        widget->max=widget->min+.001;
    widget->sliderValue=(*widget->myValue-widget->min)/(widget->max-widget->min)*2-1;
    widget->myWidget.startX=str_wd(getEditorFont(),1,1,"%s",widget->myText)+EDITORUI_BUFFER;
    width=EDITORUI_BUFFER*2+str_wd(getEditorFont(),1,1,"%s",text)+150;
    if (editorUIWindows[ID].width<width)
        editorUIWindows[ID].width=width;
    return addWidgetToWindow(ID,(EditorUIWidget *)widget);
}

/*********************************************************************************/
// this isn't complete, but it doesn't really matter
static int isTextCharacter(char c) {
    return (    (c>='a' && c<='z') ||
                (c>='A' && c<='Z') ||
                (c>='0' && c<='9') ||
                (c=='!' || c=='@' || c=='#' || c=='$' || c=='%' )  ||
                (c=='^' || c=='&' || c=='*' || c=='(' || c==')' ));
}

int editorUIDrawComboBox(EditorUIWidget * w,int x,int y,int z,int top,int bottom,double scale,int hasFocus,int locked) {
    EditorUIComboBox * widget=(void *)w;
    int width=100;
    int change=0;
    int oldValue=widget->myCombo->currChoice;
    if (y<top || y+20*1.6*scale>bottom)
        return 0;

    setEditorFontActive(locked?CLR_TEXTLOCKED:(w->selected?CLR_SELECTED_TEXT:CLR_TEXT));
    cprntEx(x+scale*(EDITORUI_BUFFER),y+scale*(EDITORUI_BUFFER+7.5),z,scale,scale,CENTER_Y|NO_MSPRINT,"%s",widget->myText);
    widget->myCombo->x=x+scale*(7+editorUIWindows[w->window].comboBoxStartX);
    widget->myCombo->y=y+scale*(9);
    widget->myCombo->z=z-1;
    widget->myCombo->wd=(editorUIWindows[widget->myWidget.window].width-editorUIWindows[widget->myWidget.window].startX-EDITORUI_BUFFER)*scale;
    widget->myCombo->dropWd=widget->myCombo->wd;
    widget->myCombo->ht=19*scale;
    widget->myCombo->expandHt=500*scale;
    if (!widget->myCombo->state && widget->myValue)
        widget->myCombo->currChoice=*widget->myValue;
    if (widget->myCombo->state && !widget->myWidget.selected && !editorUILoseFocus && widget->myTempSelection!=-1)
            widget->myCombo->currChoice=widget->myTempSelection;
    widget->myCombo->state=widget->myWidget.selected;
    combobox_display(widget->myCombo);
    widget->myWidget.selected=widget->myCombo->state;
    change=oldValue!=widget->myCombo->currChoice;

    if(!widget->myEditState) {
        widget->myEditState=uiEditCreateSimple("", 64, getEditorFont(), CLR_TEXT, editInputHandler);
        uiEditSetClicks(widget->myEditState, "type2", "mouseover");
    }

    if (widget->myCombo->state)
        uiEditTakeFocus(widget->myEditState);
    else
        uiEditReturnFocus(widget->myEditState);
    
    {
        KeyInput* input;
        input = inpGetKeyBuf();
        
        if (widget->myCombo->state && input!=NULL) {
            if (widget->myTempSelection==-1)
                widget->myTempSelection=widget->myCombo->currChoice;
            if (isTextCharacter(input->asciiCharacter)) {
                int i;
                int size;
                sprintf(widget->myTarget,"%s%c",widget->myTarget,input->asciiCharacter);
                size=strlen(widget->myTarget);
                for (i=0;i<eaSize(&widget->myStrings);i++)
                    if (strnicmp(widget->myTarget,widget->myStrings[i],size)==0) {
                        widget->myTempSelection=i;
                        break;
                    }

                if (i==eaSize(&widget->myStrings)) {
                    sprintf(widget->myTarget,"%c",input->asciiCharacter);
                    widget->myTarget[0]=0;
                }
            }
            if (inpLevel(INP_RETURN)) {
                widget->myCombo->state=0;
                widget->myWidget.selected=0;
                widget->myTarget[0]=0;
                widget->myCombo->currChoice=widget->myTempSelection;
            }
            if (inpLevel(INP_BACKSPACE)) {
                if (widget->myTarget[0])
                    *((char *)(widget->myTarget+strlen(widget->myTarget)-1))=0;
            }
            if (inpLevel(INP_DOWN)) {
                if (widget->myTempSelection<eaSize(&widget->myStrings)-1)
                    widget->myTempSelection++;
                widget->myTarget[0]=0;
            }
            if (inpLevel(INP_UP)) {
                if (widget->myTempSelection>0)
                    widget->myTempSelection--;
                widget->myTarget[0]=0;
            }
            {
                int max;
                widget->myCombo->sb->offset=15*(widget->myTempSelection+1);
                max=-((widget->myCombo->scale*widget->myCombo->expandHt-R10-PIX3)*scale-(MAX(eaSize(&widget->myCombo->strings),eaSize(&widget->myCombo->elements))*FONT_HEIGHT+PIX3)*scale);
                if (max<0)
                    max=0;
                if (widget->myCombo->sb->offset>max)
                    widget->myCombo->sb->offset=max;
            }
        }
    }
    if (widget->myCombo->state==0)
        widget->myTempSelection=-1;
    {
        int size=strlen(widget->myTarget);
        if (widget->myTempSelection!=-1) {
            int black[4]={CLR_BLACK,CLR_BLACK,CLR_BLACK,CLR_BLACK};
            int white[4]={CLR_WHITE,CLR_WHITE,CLR_WHITE,CLR_WHITE};
            int grey[4]={CLR_GREY,CLR_GREY,CLR_GREY,CLR_GREY};
            printBasic(&game_12,x+editorUIWindows[w->window].startX+scale*(2*EDITORUI_BUFFER-3),y+scale*(EDITORUI_BUFFER+13),z+100,scale*1.022,scale,0,widget->myStrings[widget->myCombo->currChoice],strlen(widget->myStrings[widget->myCombo->currChoice]),black);
            printBasic(&game_12,x+editorUIWindows[w->window].startX+scale*(2*EDITORUI_BUFFER-3),y+scale*(EDITORUI_BUFFER+13),z+100,scale*1.022,scale,0,widget->myStrings[widget->myTempSelection],strlen(widget->myStrings[widget->myTempSelection]),white);
            printBasic(&game_12,x+editorUIWindows[w->window].startX+scale*(2*EDITORUI_BUFFER-3),y+scale*(EDITORUI_BUFFER+13),z+100,scale*1.022,scale,0,widget->myStrings[widget->myTempSelection],size,grey);
        }
    }

    if (widget->myValue)
        *widget->myValue=widget->myCombo->currChoice;

    if (widget->myCombo->state==1)
        edit_state.sel=0;
//    if (widget->myCombo->state || change)
//        editorUIDisableClicks=1;
    return change;
}

void destroyComboBox(EditorUIComboBox* w) {
    EditorUIComboBox* widget=(EditorUIComboBox*)w;
    eaDestroyEx(&widget->myStrings,NULL);
    uiEditDestroy(widget->myEditState);
    free(widget->myText);
}

int editorUIAddComboBoxFromEArray(int ID,int * value,char * text,EditorUICallback callback,char ** list) {
    EditorUIComboBox* widget;
    int width;
    int i;
    EDITORUI_VALIDATE_ID_RET(ID);
    widget=calloc(1,sizeof(EditorUIComboBox));
    widget->myWidget.type=EDITORUI_COMBOBOX;
    widget->myWidget.draw=editorUIDrawComboBox;
    widget->myWidget.window=ID;
    widget->myWidget.elementsTall=1;
    widget->myWidget.destroy=destroyComboBox;
    widget->myWidget.callback=callback;
    widget->myWidget.canBeSelected=1;
    widget->myEditState=NULL;
    widget->myText=strdup(text);
    widget->myValue=value;
    widget->myWidget.subWidgets=NULL;
    widget->myTarget[0]=0;
    widget->myTempSelection=-1;
    eaCreate(&widget->myStrings);
    for (i=0;i<eaSize(&list);i++)
        eaPush(&widget->myStrings,strdup(list[i]));
    widget->myCombo=calloc(1,sizeof(ComboBox));
    combobox_init(widget->myCombo,0,0,0,250,14,500,widget->myStrings,0);
    widget->myCombo->currChoice = value?*value:0;
    if (value && (widget->myCombo->currChoice < 0 || widget->myCombo->currChoice >= eaSize(&list)))
        *value = widget->myCombo->currChoice = 0;

    widget->myWidget.startX=str_wd(getEditorFont(),1,1,"%s",widget->myText)+EDITORUI_BUFFER;
    width=widget->myWidget.startX+150;
    if (editorUIWindows[ID].width<width)
        editorUIWindows[ID].width=width;
    return addWidgetToWindow(ID,(EditorUIWidget *)widget);
}

int editorUIAddComboBoxFromArray(int ID,int * value,char * text,EditorUICallback callback,char * list[]) {
    char ** ea=0;
    int i=0;
    int ret;
    while (list[i][0]) {
        eaPush(&ea,list[i]);
        i++;
    }
    ret = editorUIAddComboBoxFromEArray(ID,value,text,callback,ea);
    eaDestroy(&ea);
    return ret;
}

int editorUIAddComboBox(int ID,int * value,char * text,EditorUICallback callback,...) {
    va_list args;
    char ** ea=0;
    char * str;
    int ret;
    va_start(args,callback);
    while ((str=va_arg(args,char *))!=NULL)
        eaPush(&ea,str);
    va_end(args);
    ret = editorUIAddComboBoxFromEArray(ID,value,text,callback,ea);
    eaDestroy(&ea);
    return ret;
}

/*********************************************************************************/

static int editorUIComboTextBoxShowWhenLast(int widgetId)
{
    int showOther = 0;
    EditorUIWidget* parentWidget = getSubWidgetParent(widgetId);
    if (parentWidget && parentWidget->type == EDITORUI_COMBOBOX && eaSize(&parentWidget->subWidgets) && eaSize(&parentWidget->subWidgets[0]->widgets) > 1)
    {
        EditorUIWidget* subWidget = parentWidget->subWidgets[0]->widgets[1];
        if (subWidget && subWidget->type == EDITORUI_TEXTENTRY)
        {
            EditorUIComboBox* combo = (void*)parentWidget;
            EditorUITextEntry* textentry = (void*)subWidget;

            // We want to show the text box if we have the last element(Other) in the list selected
            showOther = (combo->myCombo->currChoice == (eaSize(&combo->myStrings) - 1));

            // This is called everyframe, check which is currently selected and make sure we update
            if (combo->myCombo->currChoice >= 0 && combo->myCombo->currChoice < (eaSize(&combo->myStrings) - 1))
            {
                // If its different and its not user entered, find the right combobox value
                if (!showOther && strcmp(textentry->myValue, combo->myStrings[combo->myCombo->currChoice]))
                {
                    int i, n = eaSize(&combo->myStrings);
                    for (i = 0; i < n; i++)
                    {
                        if (!strcmp(textentry->myValue, combo->myStrings[i]))
                        {
                            combo->myCombo->currChoice = i;
                            return showOther;
                        }
                    }
                    // If we cant find it, it must be the other value, switch to that
                    combo->myCombo->currChoice = eaSize(&combo->myStrings) - 1;
                }
            }
        }
    }
    return showOther;
}

// The text entry will always be the 2nd element in the subWidgets->widgets array(always a NULL widget first)
static void editorUIComboTextBoxCallback(void* widgetId)
{
    int comboId = (int)widgetId;
    EditorUIWidget* parentWidget = editorUIWidgetFromID(comboId);
    if (parentWidget && parentWidget->type == EDITORUI_COMBOBOX && eaSize(&parentWidget->subWidgets) && eaSize(&parentWidget->subWidgets[0]->widgets) > 1)
    {
        EditorUIWidget* subWidget = parentWidget->subWidgets[0]->widgets[1];
        if (subWidget && subWidget->type == EDITORUI_TEXTENTRY)
        {
            EditorUIComboBox* combo = (void*)parentWidget;
            EditorUITextEntry* textentry = (void*)subWidget;
            if (combo->myCombo->currChoice >= 0 && combo->myCombo->currChoice < (eaSize(&combo->myStrings) - 1))
                strcpy(textentry->myValue, combo->myStrings[combo->myCombo->currChoice]);
        }
    }
}

int editorUIAddComboTextBoxFromEArray(int ID,char * value,int capacity,char * text,char ** list)
{
    char** comboTexts = NULL;
    int widgetId, i, n = eaSize(&list);

    // Create a copy of the strings and add other to the end of it
    for (i = 0; i < n; i++)
        eaPush(&comboTexts, list[i]);
    eaPush(&comboTexts, "Other...");

    // Default to the first value in the list
    strcpy(value, comboTexts[0]);

    // Add the combobox with the textbox as a subwidget under it
    widgetId = editorUIAddComboBoxFromEArray(ID, NULL, text, editorUIComboTextBoxCallback, comboTexts);
    editorUIAddSubWidgets(ID, editorUIComboTextBoxShowWhenLast, 0, 0);
    editorUIAddTextEntry(ID, value, capacity, NULL, "Other");
    editorUIEndSubWidgets(ID);

    // Pass in the subwidget id as the callback param for the combobox
    editorUISetWidgetCallbackParam(widgetId, (void*)widgetId);

    eaDestroy(&comboTexts);
    return widgetId;
}

/*********************************************************************************/

int editorUIDrawList(EditorUIWidget * w,int x,int y,int z,int top,int bottom,double scale,int hasFocus,int locked) {
    EditorUIList * widget=(void *)w;
    int i;
    int changed=0;

    if (!(y<top || y+20*scale>bottom))
    {
        setEditorFontActive(locked?CLR_TEXTLOCKED:CLR_TEXT);
        cprntEx(x+scale*(EDITORUI_BUFFER)+scale*widget->myWidth/2,y+scale*(EDITORUI_BUFFER+7.5),z,scale,scale,CENTER_Y|NO_MSPRINT,"%s",widget->myText);
    }
    for (i=0;i<eaSize(&widget->myStrings);i++) {
        int over;
        y+=20*scale;
        if (y<top || y+20*scale>bottom)
            continue;
        over=(!editorUIWindows[w->window].scrollBar.grabbed && widget->myWidget.callback!=NULL && isOver(x+scale*EDITORUI_BUFFER,scale*widget->myWidth,y+scale*27,scale*20));
        setEditorFontActive(locked?CLR_TEXTLOCKED:CLR_TEXT);
        cprntEx(x+scale*(EDITORUI_BUFFER),y+scale*(EDITORUI_BUFFER+7.5),z,scale,scale,CENTER_Y|NO_MSPRINT,"%s",widget->myStrings[i]);
        if (over && !locked) {
            CBox box;
            BuildCBox(&box,x+scale*EDITORUI_BUFFER,0,scale*(widget->myWidth-EDITORUI_BUFFER),10000);

            if (mouseClickHit(&box,MS_LEFT)) {
                widget->lastClicked=i;
                changed=1;
                *widget->myValue=widget->lastClicked;
//                editorUIDisableClicks=1;
            }
        }
        if (inpLevel(INP_TAB)) {
            if (!widget->tabbing) {
                int current=widget->lastClicked;
                if (inpLevel(INP_SHIFT))
                    widget->lastClicked--;
                else
                    widget->lastClicked++;
                if (widget->lastClicked<0)
                    widget->lastClicked=0;
                else
                    if (widget->lastClicked>=eaSize(&widget->myStrings))
                        widget->lastClicked =eaSize(&widget->myStrings)-1;
                    else {
                        *widget->myValue=widget->lastClicked;
                        changed=1;
                    }
                    editorUIWindows[w->window].scrollBar.offset+=(widget->lastClicked-current)*20.0;
            }
            widget->tabbing=1;
        } else {
            widget->tabbing=0;
        }
    }

    return changed;
}

void destroyList(EditorUIComboBox* w) {
    EditorUIList* widget=(EditorUIList*)w;
    eaDestroyEx(&widget->myStrings,NULL);
    free(widget->myText);
}

int editorUIAddListFromEArray(int ID,int * value,char * text,EditorUICallback callback,char ** list) {
    EditorUIList * widget;
    int width=0;
    int i;
    EDITORUI_VALIDATE_ID_RET(ID);
    widget=calloc(1,sizeof(EditorUIList));
    widget->myWidget.type=EDITORUI_LIST;
    widget->myWidget.draw=editorUIDrawList;
    widget->myWidget.window=ID;
    widget->myWidget.destroy=destroyList;
    widget->myWidget.callback=callback;
    widget->myWidget.canBeSelected=0;
    widget->myText=strdup(text);
    widget->myValue=value;
    widget->myWidth=0;
    widget->lastClicked=-1;
    widget->myWidget.subWidgets=NULL;

    eaCreate(&widget->myStrings);
    width=str_wd(getEditorFont(),1,1,widget->myText,widget->myText);
    for (i=0;i<eaSize(&list);i++) {
        int temp=str_wd(getEditorFont(),1,1,list[i],strlen(list[i]));
        eaPush(&widget->myStrings,strdup(list[i]));
        if (temp>width)
            width=temp;
    }
    if (width>widget->myWidth)
        widget->myWidth=width;
    width+=EDITORUI_BUFFER;
    if (editorUIWindows[ID].width<width)
        editorUIWindows[ID].width=width;
    if (editorUIWindows[ID].width-EDITORUI_BUFFER>widget->myWidth)
        widget->myWidth=editorUIWindows[ID].width-EDITORUI_BUFFER;
    widget->myWidget.elementsTall=eaSize(&widget->myStrings)+1;
    return addWidgetToWindow(ID,(EditorUIWidget *)widget);
}

int editorUIAddListFromArray(int ID,int * value,char * text,EditorUICallback callback,char * list[]) {
    char ** ea=0;
    int i=0;
    while (list[i][0]) {
        eaPush(&ea,list[i]);
        i++;
    }
    return editorUIAddListFromEArray(ID,value,text,callback,ea);
}

int editorUIAddList(int ID,int * value,char * text,EditorUICallback callback,...) {
    va_list args;
    char ** ea=0;
    char * str;
    int ret;
    va_start(args,callback);
    while ((str=va_arg(args,char *))!=NULL)
        eaPush(&ea,str);
    va_end(args);
    ret = editorUIAddListFromEArray(ID,value,text,callback,ea);
    eaDestroy(&ea);
    return ret;
}

/*********************************************************************************/

int editorUIDrawConsole(EditorUIWidget * w,int x,int y,int z,int top,int bottom,double scale,int hasFocus,int locked) {
    EditorUIConsole * widget=(void *)w;
    int i,start,end;
    drawFrame(PIX2,R4,x+8*scale,y+8*scale, z, (editorUIWindows[w->window].width-6)*scale, w->elementsTall*20*scale+2*scale, 1, CLR_FRAME, widget->myBGColor);
    start=0;
    if (widget->myLinesTall<eaSize(&widget->lines))
        start=eaSize(&widget->lines)-widget->myLinesTall;
    end=start+widget->myLinesTall;
    if (eaSize(&widget->lines)<widget->myLinesTall)
        end=eaSize(&widget->lines);
    for (i=start;i<end;i++) {
        setEditorFontActive(widget->colors[i]);
        prnt(x+15*scale,y+(6+(i-start+1)*15)*scale,z+5,1,1,"%s",widget->lines[i]);
    }
    return  0;
}

void destroyConsole(EditorUIComboBox* w) {
    EditorUIConsole * widget=(EditorUIConsole *)w;
    eaDestroyEx(&widget->lines,NULL);
}

void editorUIConsoleAddLine(int widgetID,char * string,int color) {
    EditorUIConsole * widget=(EditorUIConsole *)editorUIWidgetFromID(widgetID);
    eaiPush(&widget->colors,color);
    eaPush(&widget->lines,strdup(string));
}

int editorUIAddConsole(int ID,int bgColor,int linesTall,int minWidth) {
    EditorUIConsole * widget;
    EDITORUI_VALIDATE_ID_RET(ID);

    widget=calloc(1,sizeof(EditorUIConsole));
    widget->myWidget.type=EDITORUI_CONSOLE;
    widget->myWidget.draw=editorUIDrawConsole;
    widget->myWidget.window=ID;
    widget->myWidget.elementsTall=(linesTall+1)*15/20;;
    widget->myWidget.destroy=destroyConsole;
    widget->myWidget.callback=NULL;
    widget->myWidget.subWidgets=NULL;
    widget->myWidget.canBeSelected=0;

    eaiCreate(&widget->colors);
    eaCreate(&widget->lines);
    widget->myBGColor=bgColor;
    widget->myLinesTall=linesTall;
    if (editorUIWindows[ID].width<minWidth)
        editorUIWindows[ID].width=minWidth;
    return addWidgetToWindow(ID,(EditorUIWidget *)widget);
}

/*********************************************************************************/

int editorUIDrawButtonRow(EditorUIWidget * w,int x,int y,int z,int top,int bottom,double scale,int hasFocus,int locked) {
    EditorUIButtonRow * widget=(void *)w;
    double width=editorUIWindows[w->window].width;
    int numButtons=eaSize(&widget->myTexts);
    double buttonWidth=(width-(numButtons)*EDITORUI_BUFFER)/numButtons;
    int i;
    if (y<top || y+20*scale>bottom)
        return 0;
    for (i=0;i<numButtons;i++) {
        if (D_MOUSEHIT==drawStdButton(x+scale*buttonWidth/2+scale*EDITORUI_BUFFER,y+scale*(EDITORUI_BUFFER+7.5),z,scale*buttonWidth,scale*15,CLR_BUTTON,widget->myTexts[i],scale,locked?BUTTON_LOCKED:0))
            widget->myFuncs[i](widget->myTexts[i]);
        x+=scale*buttonWidth+scale*EDITORUI_BUFFER;
    }
    return 0;    // these aren't things that need change callbacks
}

void destroyButtonRow(EditorUIWidget * w) {
    EditorUIButtonRow * widget=(EditorUIButtonRow*)w;
}

int editorUIAddButtonRow(int ID,EditorUICallback callback,...) {
    EditorUIButtonRow * widget;
    va_list args;
    int width = 0;
    char * text;
    void (*func)(char *);
    EDITORUI_VALIDATE_ID_RET(ID);
    widget=calloc(1,sizeof(EditorUISlider));
    widget->myWidget.type=EDITORUI_BUTTONROW;
    widget->myWidget.draw=editorUIDrawButtonRow;
    widget->myWidget.window=ID;
    widget->myWidget.elementsTall=1;
    widget->myWidget.destroy=destroyButtonRow;
    widget->myWidget.callback=callback;
    widget->myWidget.subWidgets=NULL;
    widget->myWidget.canBeSelected=0;

    eaCreate(&widget->myTexts);
    eaCreate((void ***)&widget->myFuncs);
    va_start(args,callback);
    while ((text=va_arg(args,char *))!=NULL) {
        width+=str_wd(getEditorFont(),1,1,text)+EDITORUI_BUFFER;
        func=va_arg(args,void *);
        eaPush(&widget->myTexts,strdup(text));
        eaPush((void ***)&widget->myFuncs,func);
    }
    va_end(args);
    if (editorUIWindows[ID].width<width)
        editorUIWindows[ID].width=width;

    return addWidgetToWindow(ID,(EditorUIWidget *)widget);
}

/*********************************************************************************/

void editorUIDrawIcon(char * name,int x,int y,int z,double width,double height,int over) {
    AtlasTex * icon;
    CBox box;

    icon = atlasLoadTexture(name);
    if (icon==NULL)
        return;
    BuildCBox(&box, x, y, icon->width, icon->height);
    display_sprite( icon, x, y, z, width/(double)icon->width, height/(double)icon->height, over?0xffffffff:0xcfcfcfff);
}

int editorUIDrawIconRow(EditorUIWidget * w,int x,int y,int z,int top,int bottom,double scale,int hasFocus,int locked) {
    EditorUIIconRow * widget=(void *)w;
    double width=editorUIWindows[w->window].width;
    int numButtons=eaSize(&widget->myTexts);
    int i;
    double space=(double)(editorUIWindows[w->window].width-(numButtons*widget->myWidth+EDITORUI_BUFFER))/(double)(numButtons-1);
    float dontCare;
    int idontCare;
    float currX,currY;
    double totalWidth=numButtons*(widget->myWidth+10)-20;
    double offset=0;
    
    if (y<top || y+20*scale>bottom)
        return 0;

    window_getDims(editorUIGetWindow(widget->myWidget.window), &currX, &currY, &dontCare, &dontCare, &dontCare, &dontCare, &idontCare, &idontCare);

    x+=scale*EDITORUI_BUFFER;
    y+=scale*EDITORUI_BUFFER;
    drawFrame(PIX2,R4, x, y, z, (editorUIWindows[w->window].width-EDITORUI_BUFFER)*scale, (widget->myHeight+10)*scale, 1, CLR_FRAME, 0x000000ff);
    if (editorUIWindows[w->window].width-30<totalWidth)
    {
        CBox box;
        BuildCBox(&box, x+4, y+4, editorUIWindows[w->window].width-EDITORUI_BUFFER-7,widget->myHeight+10);
        doScrollBar(&widget->myScrollBar,editorUIWindows[w->window].width-30,totalWidth,x-currX+10,y-currY+widget->myHeight+16,z, &box, 0);
        offset=widget->myScrollBar.offset;
    }
    set_scissor(1);
    scissor_dims(x+4, y+4, editorUIWindows[w->window].width-EDITORUI_BUFFER-7,widget->myHeight+10);
    x-=offset;
    x+=4;
    for (i=0;i<numButtons;i++) {
        int selected=isOver(x,widget->myWidth,y+widget->myHeight,widget->myHeight);
        double factor=1;
        if (selected)
            factor=1.1;
        editorUIDrawIcon(widget->myIcons[i],x-(widget->myWidth/2.0*(factor-1)),y+5-(widget->myHeight/2.0*(factor-1)),z,widget->myWidth*factor,widget->myHeight*factor,selected);
        if(selected && inpEdge(INP_LBUTTON)){// && !editorUIDisableClicks){
            eatEdge(INP_LBUTTON);
            widget->myFuncs[i](widget->myData[i]);
        }
        x+=scale*(10+widget->myWidth);
    }
    set_scissor(0);

    return 0;    // these aren't things that need change callbacks
}

void destroyIconRow(EditorUIWidget * w) {
    EditorUIIconRow * widget=(EditorUIIconRow*)w;
    eaDestroy(&widget->myIcons);
    eaDestroy((void ***)&widget->myFuncs);
    eaDestroy(&widget->myTexts);
    eaDestroy(&widget->myData);
}

int editorUIAddIconRow(int ID,int width,int height,...) {
    EditorUIIconRow * widget;
    va_list args;
    char * icon;
    char * text;
    void (*func)(char *);
    void * data;
    double windowWidth=150;
    EDITORUI_VALIDATE_ID_RET(ID);
    widget=calloc(1,sizeof(EditorUIIconRow));
    widget->myScrollBar.horizontal=1;
    widget->myScrollBar.wdw=editorUIGetWindow(ID);
    widget->myWidget.type=EDITORUI_BUTTONROW;
    widget->myWidget.draw=editorUIDrawIconRow;
    widget->myWidget.window=ID;
    widget->myWidget.elementsTall=1+(double)height/20.0;
    widget->myWidget.destroy=destroyIconRow;
    widget->myWidget.subWidgets=NULL;
    widget->myWidget.canBeSelected=0;
    widget->myWidth=width;
    widget->myHeight=height;

    eaCreate(&widget->myIcons);
    eaCreate(&widget->myTexts);
    eaCreate((void ***)&widget->myFuncs);
    eaCreate(&widget->myData);
    va_start(args,height);
    while ((icon=va_arg(args,char *))!=NULL) {
        text=va_arg(args,char *);
        func=va_arg(args,void *);
        data=va_arg(args,void *);
        eaPush(&widget->myIcons,strdup(icon));
        eaPush(&widget->myTexts,strdup(text));
        eaPush((void ***)&widget->myFuncs,func);
        eaPush(&widget->myData,data);
    }
    va_end(args);
    if (eaSize(&widget->myIcons)*(widget->myWidth+EDITORUI_BUFFER)<windowWidth)
        windowWidth=eaSize(&widget->myIcons)*(widget->myWidth+EDITORUI_BUFFER);
    if (editorUIWindows[ID].width<windowWidth)
        editorUIWindows[ID].width=windowWidth;

    return addWidgetToWindow(ID,(EditorUIWidget *)widget);
}

/*********************************************************************************/

int editorUIDrawButton(EditorUIWidget * w,int x,int y,int z,int top,int bottom,double scale,int hasFocus,int locked) {
    EditorUIButton * widget=(void *)w;
    double width=scale*editorUIWindows[w->window].width-(scale*editorUIWindows[w->window].startX+scale*EDITORUI_BUFFER);

    if (y<top || y+20*scale>bottom)
        return 0;
    setEditorFontActive(locked?CLR_TEXTLOCKED:CLR_TEXT);
    if (widget->buttonDesc)
        cprntEx(x+scale*(EDITORUI_BUFFER),y+scale*(EDITORUI_BUFFER+7.5),z,scale,scale,CENTER_Y|NO_MSPRINT,"%s",widget->buttonDesc);
    if (D_MOUSEHIT==drawStdButton(x+scale*editorUIWindows[w->window].textEntryStartX-scale+width/2+scale*(EDITORUI_BUFFER-2),y+scale*(EDITORUI_BUFFER+7.5),z,width,scale*15,CLR_BUTTON,widget->buttonText,scale,locked?BUTTON_LOCKED:0))
        widget->myFunc(widget->buttonDesc?widget->buttonDesc:widget->buttonText);
    return 0;    // these aren't things that need change callbacks
}

void destroyButton(EditorUIWidget * w) {
    EditorUIButton * widget=(EditorUIButton*)w;
}

int editorUIAddButton(int ID,char* buttonDesc,char* buttonText,void (*clickFunction)(char *)) {
    EditorUIButton * widget;
    EDITORUI_VALIDATE_ID_RET(ID);
    widget=calloc(1,sizeof(EditorUIButton));
    widget->myWidget.type=EDITORUI_BUTTON;
    widget->myWidget.draw=editorUIDrawButton;
    widget->myWidget.window=ID;
    widget->myWidget.elementsTall=1;
    widget->myWidget.destroy=destroyButton;
    widget->myWidget.callback=NULL;
    widget->myWidget.subWidgets=NULL;
    widget->myWidget.canBeSelected=0;

    widget->buttonDesc=strdup(buttonDesc);
    widget->buttonText=strdup(buttonText);
    widget->myFunc=clickFunction;
    if (buttonDesc)
        widget->myWidget.startX=str_wd(getEditorFont(),1,1,"%s",widget->buttonDesc)+EDITORUI_BUFFER+EDITORUI_BUFFER;
    return addWidgetToWindow(ID,(EditorUIWidget *)widget);
}

void editorUIButtonChangeText(int widgetID, char *buttonText)
{
    EditorUIButton * widget = (EditorUIButton*)editorUIWidgetFromID(widgetID);

    if (!widget)
        return;

    SAFE_FREE(widget->buttonText);
    widget->buttonText=strdup(buttonText);
}


/*********************************************************************************/

int editorUIDrawProgressBar(EditorUIWidget * w,int x,int y,int z,int top,int bottom,double scale,int hasFocus,int locked) {
    EditorUIProgressBar * widget=(void *)w;
    double width=scale*editorUIWindows[w->window].width-scale*EDITORUI_BUFFER;
    float value=0;
    if (y<top || y+20*scale>bottom)
        return 0;
    if (widget->percentCallback)
        value = widget->percentCallback();
    MIN1(value,1);
    MAX1(value,0);
    drawHealthBar(x+scale*(EDITORUI_BUFFER),y+scale*(EDITORUI_BUFFER),z,width,value,1,1,CLR_WHITE,0x0005ffff,HEALTHBAR_LOADING);
    return 0;    // these aren't things that need change callbacks
}

void destroyProgressBar(EditorUIWidget * w) {
    EditorUIProgressBar * widget=(EditorUIProgressBar*)w;
}

int editorUIAddProgressBar(int ID,float (*getPercentCallback)(void))
{
    EditorUIProgressBar * widget;
    EDITORUI_VALIDATE_ID_RET(ID);
    widget=calloc(1,sizeof(EditorUIProgressBar));
    widget->myWidget.type=EDITORUI_PROGRESSBAR;
    widget->myWidget.draw=editorUIDrawProgressBar;
    widget->myWidget.window=ID;
    widget->myWidget.elementsTall=1;
    widget->myWidget.destroy=destroyProgressBar;
    widget->percentCallback=getPercentCallback;
    if (editorUIWindows[ID].width<100)
        editorUIWindows[ID].width=100;
    return addWidgetToWindow(ID,(EditorUIWidget *)widget);
}

/*********************************************************************************/

static int breakdown_colors[] = { CLR_TXT_BLUE, CLR_TXT_LRED, 0x00d919ff, CLR_TXT_RED };

int editorUIDrawBreakdownBar(EditorUIWidget * w,int x,int y,int z,int top,int bottom,double scale,int hasFocus,int locked) {
    EditorUIBreakdownBar * widget=(void *)w;
    double width=scale*editorUIWindows[w->window].width-scale*EDITORUI_BUFFER;
    float start=0;
    int i, selected;
    if (y<top || y+20*scale>bottom)
        return 0;

    widget->pulse_counter += 0.25f * TIMESTEP;
    while (widget->pulse_counter >= 2*PI)
        widget->pulse_counter -= 2*PI;

    drawFrame(PIX2, R4, x+scale*EDITORUI_BUFFER, y+scale*(3.5+EDITORUI_BUFFER), z, width, 8*scale, 1, CLR_FRAME, CLR_BLACK);
    for (i = 0; i < widget->numDivisions; i++)
    {
        float value = 0;
        if (widget->percentCallback)
            value = widget->percentCallback(i, &selected);
        MIN1(value,1);
        MAX1(value,0);
        if (value)
        {
            int color = breakdown_colors[i%ARRAY_SIZE(breakdown_colors)];
            if (selected)
                brightenColor(&color, 25.f*sinf(widget->pulse_counter));
            drawFrame(PIX2, R4, x+scale*(1+EDITORUI_BUFFER)+start*(width-2), y+scale*(3.5+EDITORUI_BUFFER), z+1, value*(width-2), 8*scale, 1, 0, color);
        }
        start += value;
    }
    return 0;    // these aren't things that need change callbacks
}

void destroyBreakdownBar(EditorUIWidget * w) {
    EditorUIBreakdownBar * widget=(EditorUIBreakdownBar*)w;
}

int editorUIAddBreakdownBar(int ID,float (*getPercentCallback)(int,int *),int numDivisions)
{
    EditorUIBreakdownBar * widget;
    EDITORUI_VALIDATE_ID_RET(ID);
    widget=calloc(1,sizeof(EditorUIBreakdownBar));
    widget->myWidget.type=EDITORUI_BREAKDOWNBAR;
    widget->myWidget.draw=editorUIDrawBreakdownBar;
    widget->myWidget.window=ID;
    widget->myWidget.elementsTall=1;
    widget->myWidget.destroy=destroyBreakdownBar;
    widget->numDivisions=numDivisions;
    widget->percentCallback=getPercentCallback;
    if (editorUIWindows[ID].width<100)
        editorUIWindows[ID].width=100;
    return addWidgetToWindow(ID,(EditorUIWidget *)widget);
}

/*********************************************************************************/

static int editorUIDrawDualColorSwatch(EditorUIWidget * w,int x,int y,int z,int top,int bottom,double scale,int hasFocus,int locked) {
    EditorUIDualColorSwatch * widget=(void *)w;
    int size = 62 * scale;
    int color1 = widget->color1[0] << 24 |
        widget->color1[1] << 16 |
        widget->color1[2] << 8 |
        0xff;
    int color2 = widget->color2[0] << 24 |
        widget->color2[1] << 16 |
        widget->color2[2] << 8 |
        0xff;

    x += (editorUIWindows[w->window].width + EDITORUI_BUFFER) / 2 - size - 1;
    y += scale * EDITORUI_BUFFER;
    
    display_sprite(white_tex_atlas, x, y, z, (2.0 * size + 2) / white_tex_atlas->width, 
        (size + 2.0) / white_tex_atlas->height, 0x000000ff);
    display_sprite(white_tex_atlas, x + 1, y + 1, z, 
        (float)size / white_tex_atlas->width, 
        (float)size / white_tex_atlas->height, color1);
    display_sprite(white_tex_atlas, x + 1 + size, y + 1, z, 
        (float)size / white_tex_atlas->width, 
        (float)size / white_tex_atlas->height, color2);
    return 0;
}

static void destroyDualColorSwatch(EditorUIWidget * w) {
}

int editorUIAddDualColorSwatch(int ID, U8* color1, U8* color2) {
    EditorUIDualColorSwatch * widget;
    int width;
    EDITORUI_VALIDATE_ID_RET(ID);
    widget=calloc(1,sizeof(EditorUIDualColorSwatch));
    widget->myWidget.type=EDITORUI_COLORSWATCH;
    widget->myWidget.draw=editorUIDrawDualColorSwatch;
    widget->myWidget.window=ID;
    widget->myWidget.elementsTall=3.5;
    widget->myWidget.destroy=destroyDualColorSwatch;
    widget->myWidget.callback=NULL;
    widget->myWidget.subWidgets=NULL;
    widget->myWidget.canBeSelected=0;
    widget->color1=color1;
    widget->color2=color2;
    widget->myWidget.startX=EDITORUI_BUFFER;
    width=EDITORUI_BUFFER*2+126;
    if (editorUIWindows[ID].width<width)
        editorUIWindows[ID].width=width;
    return addWidgetToWindow(ID,(EditorUIWidget *)widget);
}

/*********************************************************************************/

static int editorUIDrawHueSelector(EditorUIWidget * w,int x,int y,int z,int top,int bottom,double scale,int hasFocus,int locked) {
    EditorUIHueSelector * widget=(void *)w;
    int width=editorUIWindows[w->window].width-EDITORUI_BUFFER;
    int height = 27.0;
    int i;
    CBox cbox;
    int result = 0;

    x += scale * EDITORUI_BUFFER;
    y += scale * EDITORUI_BUFFER;

    if (y<top || y+40*scale>bottom)
        return 0;

    display_sprite(white_tex_atlas, x, y, z, (float)width / white_tex_atlas->width, 
        scale * height / white_tex_atlas->height, 0x000000ff);

    for (i = 0; i < 6; ++i) {
        float startX = x + 1 + scale * (i * (width - 2) / 6.0f);
        float endX = x + 1 + scale * ((i + 1) * (width - 2) / 6.0f);
        Vec3 rgb;
        Vec3 hsv = { 0, 1, 1 };
        Color color;
        int startColor, endColor;
        hsv[0] = i * 60.0f;
        hsvToRgb(hsv, rgb);
        vec3ToColor(&color, rgb);
        startColor = RGBAFromColor(color); 
        hsv[0] += 60.0f;
        hsvToRgb(hsv, rgb);
        vec3ToColor(&color, rgb);
        endColor = RGBAFromColor(color);
        display_sprite_UV_4Color(white_tex_atlas, startX, y + 1, z,
            (float)(endX - startX) / white_tex_atlas->width,
            scale * (height - 2) / white_tex_atlas->width,
            startColor,endColor,endColor,startColor,0,0,1,1);
    }

    BuildCBox(&cbox, x, y, width, height);

    if (!locked && mouseDownHit(&cbox, MS_LEFT))
        w->isUserInteracting = 1;
    else if (!isDown(MS_LEFT))
        w->isUserInteracting = 0;

    if (w->isUserInteracting) {
        int mouseX,mouseY;
        float newValue;

        inpMousePos(&mouseX, &mouseY);
        newValue = 360.0 * ((float)mouseX - (x + 1)) / (width - 2);

        if (newValue < 0.0)
            newValue = 0.0;
        if (newValue > 360.0)
            newValue = 360.0;

        if (fabs(newValue - *widget->myValue) > 0.00001) {
            *widget->myValue = newValue;
            result = 1;
        }
    }

    {
        AtlasTex *arrowTex = atlasLoadTexture("map_zoom_uparrow.tga");
        int pos = x + 1 + (width - 2) * *widget->myValue / 360.0f - arrowTex->width / 2;
        display_sprite(arrowTex, pos, y + height - arrowTex->height / 2, z + 2, 1, 1, 0xffffffff);
    }

    return result;
}

static void destroyHueSelector(EditorUIWidget * w) {
}

int editorUIAddHueSelector(int ID,float * value,EditorUICallback callback) {
    EditorUIHueSelector * widget;
    int width;
    EDITORUI_VALIDATE_ID_RET(ID);
    widget=calloc(1,sizeof(EditorUIHueSelector));
    widget->myWidget.type=EDITORUI_HUESELECTOR;
    widget->myWidget.draw=editorUIDrawHueSelector;
    widget->myWidget.window=ID;
    widget->myWidget.elementsTall=1.5;
    widget->myWidget.destroy=destroyHueSelector;
    widget->myWidget.callback=callback;
    widget->myWidget.subWidgets=NULL;
    widget->myWidget.canBeSelected=0;
    widget->myValue=value;
    MAX1(*widget->myValue,0);
    MIN1(*widget->myValue,360);
    widget->myWidget.startX=EDITORUI_BUFFER;
    width=EDITORUI_BUFFER*2+256;
    if (editorUIWindows[ID].width<width)
        editorUIWindows[ID].width=width;
    return addWidgetToWindow(ID,(EditorUIWidget *)widget);
}

/*********************************************************************************/

static int editorUIDrawSaturationValueSelector(EditorUIWidget * w,int x,int y,int z,int top,int bottom,double scale,int hasFocus,int locked) {
    EditorUISaturationValueSelector * widget=(void *)w;
    int width=editorUIWindows[w->window].width-EDITORUI_BUFFER;
    int height = 256;
    CBox cbox;
    int result = 0;

    x += scale * EDITORUI_BUFFER;
    y += scale * EDITORUI_BUFFER;

    if (y<top || y+40*scale>bottom)
        return 0;

    display_sprite(white_tex_atlas, x, y, z, (float)width / white_tex_atlas->width, 
        scale * height / white_tex_atlas->height, 0x000000ff);

    {
        int i, j;
        int nsteps = 8;
        Vec3 hsv = { 0, 0, 0 };
        hsv[0] = widget->hsv[0];

        for (i = 0; i < nsteps; ++i) {
            float valTop = 1.0 - (float)i / nsteps;
            float valBottom = 1.0 - (float)(i + 1) / nsteps;
            int patchY = y + 1 + i * (height - 2) / nsteps;
            int patchHeight = y + 1 + (i + 1) * (height - 2) / nsteps - patchY;

            for (j = 0; j < nsteps; ++j) {
                Vec3 rgb;
                Color color;
                int topLeft, topRight, bottomLeft, bottomRight;
                float satLeft = (float)j / nsteps;
                float satRight = (float)(j + 1) / nsteps;
                int patchX = x + 1 + j * (width - 2) / nsteps;
                int patchWidth = x + 1 + (j + 1) * (width - 2) / nsteps - patchX;
                hsv[1] = satLeft;
                hsv[2] = valTop;
                hsvToRgb(hsv, rgb);
                vec3ToColor(&color, rgb);
                topLeft = RGBAFromColor(color);
                hsv[1] = satRight;
                hsvToRgb(hsv, rgb);
                vec3ToColor(&color, rgb);
                topRight = RGBAFromColor(color);
                hsv[2] = valBottom;
                hsvToRgb(hsv, rgb);
                vec3ToColor(&color, rgb);
                bottomRight = RGBAFromColor(color);
                hsv[1] = satLeft;
                hsvToRgb(hsv, rgb);
                vec3ToColor(&color, rgb);
                bottomLeft = RGBAFromColor(color);
                display_sprite_UV_4Color(white_tex_atlas, patchX, patchY, z,
                    (float)(patchWidth) / white_tex_atlas->width,
                    (float)(patchHeight) / white_tex_atlas->width,
                    topLeft,topRight,bottomRight,bottomLeft,0,0,1,1);
            }
        }
    }

    BuildCBox(&cbox, x, y, width, height);

    if (!locked && mouseDownHit(&cbox, MS_LEFT))
        w->isUserInteracting = 1;
    else if (!isDown(MS_LEFT))
        w->isUserInteracting = 0;

    if (w->isUserInteracting) {
        int mouseX,mouseY;
        float newSaturation, newValue;

        inpMousePos(&mouseX, &mouseY);
        newSaturation = 100 * ((float)mouseX - (x + 1)) / (width - 2);
        newValue = 100 * (1.0 - ((float)mouseY - (y + 1)) / (height - 2));

        if (newSaturation < 0.0)
            newSaturation = 0.0;
        if (newSaturation > 100.0)
            newSaturation = 100.0;
        if (newValue < 0.0)
            newValue = 0.0;
        if (newValue > 100.0)
            newValue = 100.0;

        if (fabs(newSaturation - widget->hsv[1]) > 0.00001 || fabs(newValue - widget->hsv[2]) > 0.00001) {
            widget->hsv[1] = newSaturation;
            widget->hsv[2] = newValue;
            result = 1;
        }
    }

    {
        AtlasTex *pickerTex = atlasLoadTexture("ColorPicker_Icon.tga");
        display_sprite(pickerTex, x + 1 + (width - 2) * widget->hsv[1] / 100,
            y + 1 + (height - 2) * (1 - widget->hsv[2] / 100) - pickerTex->height, z + 2, 1, 1,
            0xffffffff);
    }

    return result;
}

static void destroySaturationValueSelector(EditorUIWidget * w) {
}

int editorUIAddSaturationValueSelector(int ID,float * hsv,EditorUICallback callback) {
    EditorUISaturationValueSelector * widget;
    int width;
    EDITORUI_VALIDATE_ID_RET(ID);
    widget=calloc(1,sizeof(EditorUISaturationValueSelector));
    widget->myWidget.type=EDITORUI_SATURATIONVALUESELECTOR;
    widget->myWidget.draw=editorUIDrawSaturationValueSelector;
    widget->myWidget.window=ID;
    widget->myWidget.elementsTall=13;
    widget->myWidget.destroy=destroySaturationValueSelector;
    widget->myWidget.callback=callback;
    widget->myWidget.subWidgets=NULL;
    widget->myWidget.canBeSelected=0;
    widget->hsv=hsv;
    MAX1(widget->hsv[1],0);
    MIN1(widget->hsv[1],100);
    MAX1(widget->hsv[2],0);
    MIN1(widget->hsv[2],100);
    widget->myWidget.startX=EDITORUI_BUFFER;
    width=EDITORUI_BUFFER*2+256;
    if (editorUIWindows[ID].width<width)
        editorUIWindows[ID].width=width;
    return addWidgetToWindow(ID,(EditorUIWidget *)widget);
}

/*********************************************************************************/

int editorUIAddSpace(int ID)
{
    return editorUIAddNullWidget(ID,0.5);
}

/*********************************************************************************/

int editorUIAddDrawCallback(int ID, EditorUICallback callback)
{
    EditorUINullWidget * widget;
    EDITORUI_VALIDATE_ID_RET(ID);
    widget=calloc(1,sizeof(EditorUINullWidget));
    widget->myWidget.type=EDITORUI_NULL;
    widget->myWidget.draw=editorUIDrawCallback;
    widget->myWidget.window=ID;
    widget->myWidget.destroy=destroyNullWidget;
    widget->myWidget.callback=callback;
    return addWidgetToWindow(ID,(EditorUIWidget *)widget);
}

/*********************************************************************************/

void destroyWidget(EditorUIWidget * widget);

void destroySubWidget(EditorUISubWidgets * subWidget) {
    eaDestroyEx(&subWidget->widgets,destroyWidget);
    free(subWidget);
}

void destroyWidget(EditorUIWidget * widget) {
    stashIntRemovePointer(editorUIWidgetHash, widget->ID, NULL);
    widget->destroy(widget);
    eaDestroyEx(&widget->subWidgets,destroySubWidget);
    free(widget);
}

void editorUIDestroyWindowNow(int ID) {
    EDITORUI_VALIDATE_ID(ID);
    if (editorUIWindows[ID].closeCallback!=NULL)
        editorUIWindows[ID].closeCallback();
    eaDestroyEx(&editorUIWindows[ID].widgets,destroyWidget);
    eaDestroy(&editorUIWindows[ID].current);
    editorUIWindows[ID].widgets=NULL;
    window_setMode(editorUIGetWindow(ID),WINDOW_DOCKED);
}

void editorUIDestroyWindow(int ID) {
    EDITORUI_VALIDATE_ID(ID);
    editorUIWindows[ID].destroy=1;
}

void editorUIRemoveLastWidget(int ID) {
    EDITORUI_VALIDATE_ID(ID);
    if (eaSize(&editorUIWindows[ID].widgets)==0)
        return;
    destroyWidget(editorUIWindows[ID].widgets[eaSize(&editorUIWindows[ID].widgets)-1]);
    eaSetSize(&editorUIWindows[ID].widgets,eaSize(&editorUIWindows[ID].widgets)-1);
}

// returns height of what was drawn
EditorUIWidget ** drawn;
int drawnWidgetSelected;

int drawStuff(EditorUIWidget ** widgets,int x,int y,int z,int top,int bottom,float sc,int hasFocus,int * change,int parentID,int locked) {
    int i;
    int height=0;
    int selected;
    for (i=0;i<eaSize(&widgets);i++) {
        int j;
        eaPush(&drawn,widgets[i]);
        selected=widgets[i]->selected;
        if (widgets[i]->draw(widgets[i],x,y+height,z+1,top,bottom,sc,hasFocus,locked)) {
            *change=1;
            if (widgets[i]->callback!=NULL)
                widgets[i]->callback(widgets[i]->callbackParam);
        }
        if (!selected && widgets[i]->selected)
            drawnWidgetSelected=eaSize(&drawn)-1;
        height+=widgets[i]->elementsTall*20.0*sc;
        for (j=0;j<eaSize(&widgets[i]->subWidgets);j++)
            if (!widgets[i]->subWidgets[j]->showSubWidgets || widgets[i]->subWidgets[j]->showSubWidgets(widgets[i]->subWidgets[j]->ID)) {
                int h;
                if (widgets[i]->subWidgets[j]->frame || widgets[i]->type==EDITORUI_TABCONTROL)
                    height+=4*sc;
                h=drawStuff(widgets[i]->subWidgets[j]->widgets,x,y+height,z+1,top,bottom,sc,hasFocus,change,widgets[i]->subWidgets[j]->ID,widgets[i]->subWidgets[j]->lockWidgets?(locked||(*widgets[i]->subWidgets[j]->lockWidgets)):locked);
                if (widgets[i]->subWidgets[j]->frame || widgets[i]->type==EDITORUI_TABCONTROL)
                {
                    h+=15*sc;
                    if (widgets[i]->type==EDITORUI_TABCONTROL)
                    {
                        EditorUITabControl *tabcontrol = (EditorUITabControl*)widgets[i];
                        drawFrameOpenTopStyle(PIX2, R4, x+4*sc, y+height, z+1, (editorUIWindows[widgets[i]->window].width+2)*sc, h, tabcontrol->openleft, tabcontrol->openright, 1, CLR_FRAME, 0x00000000, kFrameStyle_3D);
                    }
                    else
                    {
                        drawFrame(PIX2, R4, x+4*sc, y+height, z, (editorUIWindows[widgets[i]->window].width+2)*sc, h, 1, CLR_FRAME, 0x00000000);
                    }
                }
                height+=h;
            }
    }
    return height;
}

// returns height of what was drawn
float preDrawStuff(EditorUIWidget ** widgets,float sc) {
    int i;
    float height=0;
    for (i=0;i<eaSize(&widgets);i++) {
        int j;
        if (widgets[i]->startX>editorUIWindows[widgets[i]->window].startX)
            editorUIWindows[widgets[i]->window].startX=widgets[i]->startX;
        height+=widgets[i]->elementsTall*20.0*sc;
        for (j=0;j<eaSize(&widgets[i]->subWidgets);j++)
            if (!widgets[i]->subWidgets[j]->showSubWidgets || widgets[i]->subWidgets[j]->showSubWidgets(widgets[i]->subWidgets[j]->ID)) {
                if (widgets[i]->subWidgets[j]->frame || widgets[i]->type==EDITORUI_TABCONTROL)
                    height+=19*sc;
                height+=preDrawStuff(widgets[i]->subWidgets[j]->widgets,sc);
            }
    }
    return height;
}

extern int gCurrentWindefIndex;
extern int collisions_off_for_rest_of_frame;
static int lastCollOff = 0;

int editorUIDrawWindow() 
{
    static int init = 0;
    int i;
    float x,y,z,wd,ht,sc;
    int color,bcolor;
    int mode;
    static int window=-1;
    int hasFocus, winHasFocus;
    int change=0, locked=0, isAModalWindow=0;
    int maxheight=600;
    int top,bottom,fudge;

    if (!init)
    {
        init=1;
        editorUISelectedWindow=-1;
        for (i=0;i<EDITORUI_MAX_WINDOWS;i++)
        {
            window_set_edit_mode(editorUIGetWindow(i),1);
            window_setMode(editorUIGetWindow(i),WINDOW_DOCKED);
        }
    }

        if (!editMode()) return 0;
    window = revLookup(gCurrentWindefIndex);
    if (editorUIWindows[window].widgets==NULL)
        return 0;
    if (editorUIWindows[window].missionEditorOnly )
        return 1;
    {
        float best=0, tbest=0;
        int dex=-1, focus=-1;
        for (i=0;i<EDITORUI_MAX_WINDOWS;i++) 
        {
            if (editorUIWindows[i].widgets==NULL)
                continue;
            if (window_getMode(editorUIGetWindow(i))==WINDOW_DOCKED) 
            {
                editorUIDestroyWindowNow(i+1);
                continue;
            }
            if (editorUIWindows[i].modal == EDITORUI_GLOBAL_MODAL || (-editorUIWindows[i].modal == (window+1)))
                isAModalWindow=1;
            window_getDims(editorUIGetWindow(i), &x, &y, &z, &wd, &ht, &sc, &color, &bcolor );
            if ((z>best || dex==-1) && isOver(x,wd,y+ht,ht+sc*15)) {
                dex=i;
                best=z;
            }
            if (z>tbest || focus==-1)
            {
                tbest=z;
                focus=i;
            }
        }
        hasFocus=(dex==window);
        if (hasFocus)
            edit_state.sel=0;
        winHasFocus=(focus==window);
    }

    // Do everything common windows are supposed to do.
    if ( !window_getDims(editorUIGetWindow(window), &x, &y, &z, &wd, &ht, &sc, &color, &bcolor ))
    {
        return 0;
    }
    sc=1;
    mode=window_getMode(editorUIGetWindow(window));
    window_EnableCloseButton(editorUIGetWindow(window), !isAModalWindow && !editorUIWindows[window].noClose);
    if (editorUIWindows[window].modal)
    {
        window_bringToFront(editorUIGetWindow(window));
    }
    else if (isAModalWindow)
    {
        locked = 1;
    }

    editorUIWindows[window].startX=0;
    ht=preDrawStuff(editorUIWindows[window].widgets,sc);
    ht+=20.0*sc;
    fudge=0;
    maxheight*=sc;
    window_SetColor(editorUIGetWindow(window), winHasFocus?CLR_FOCUSTITLE:CLR_TITLE);
    winDefs[editorUIGetWindow(window)].forceColor = 1;
    winDefs[editorUIGetWindow(window)].min_opacity = 1;
     if (ht>maxheight)
    {
        CBox box;
        BuildCBox(&box, x, y, wd, ht );
        fudge=ht;

        doScrollBar(&editorUIWindows[window].scrollBar,maxheight-40*sc,ht,x+wd-sc*5,y+20*sc,z, &box, 0);
        fudge=((double)editorUIWindows[window].scrollBar.offset/(double)(ht))*(ht-40);
        ht=maxheight;
        drawFrame(PIX3, R10, x, y, z-2, wd, ht, 1, CLR_WDWFRAME, CLR_WDW);
        window_setDims(editorUIGetWindow(window), x, y, (editorUIWindows[window].width+2*EDITORUI_BUFFER)*sc, ht);
        y-=fudge;//*(maxheight-40*sc)/maxheight;
    } else {
        drawFrame(PIX3, R10, x, y, z-2, wd, ht, 1, CLR_WDWFRAME, CLR_WDW);
        window_setDims(editorUIGetWindow(window), x, y, (editorUIWindows[window].width+EDITORUI_BUFFER)*sc, ht);
    }
    //tidy up the window a little, this could really be handled upon creation
    {
        editorUIWindows[window].sliderStartX=editorUIWindows[window].startX;
        editorUIWindows[window].comboBoxStartX=editorUIWindows[window].startX;
        editorUIWindows[window].textEntryStartX=editorUIWindows[window].startX;
    }
    top=y+(fudge)-5*sc;
    bottom=top+maxheight-5*sc;
    drawn=NULL;
    drawnWidgetSelected=-1;
    editorUILoseFocus=inpLevel(INP_ESCAPE);
    if (editorUISelectedWindow!=-1) {
        if (editorUISelectedWindow==window)
            editorUISelectedWindow=-1;
        else
            editorUILoseFocus=1;
    }
    y+=drawStuff(editorUIWindows[window].widgets,x,y,z,top,bottom,sc,hasFocus,&change,editorUIWindows[window].ID,locked);
    if (editorUILoseFocus) {
        int i=0;
        for (i=0;i<eaSize(&drawn);i++)
            drawn[i]->selected=0;
        drawnWidgetSelected=-1;
    }
    if (drawnWidgetSelected!=-1) {
        int i=0;
        for (i=0;i<eaSize(&drawn);i++)
            drawn[i]->selected=0;
        if (drawn[drawnWidgetSelected]->canBeSelected)
            drawn[drawnWidgetSelected]->selected=1;
        editorUISelectedWindow=window;
    }
    if (inpEdge(INP_TAB) && drawnWidgetSelected==-1 && !editorUILoseFocus) {
        int direction;
        int i;
        if (inpLevel(INP_SHIFT))
            direction=-1;
        else
            direction=1;
        for (i=0;i<eaSize(&drawn);i++) {
            if (drawn[i]->selected) {
                int j;
                for (j=i+direction;j>=0 && j<eaSize(&drawn);j+=direction) {
                    if (drawn[j]->canBeSelected) {
                        drawn[j]->selected=1;
                        direction=0;
                        break;
                    }
                }
                if (direction) {
                    if (direction==1)
                        j=0;
                    else
                        j=eaSize(&drawn)-1;
                    for (;j>=0 && j<eaSize(&drawn);j+=direction) {
                        if (drawn[j]->canBeSelected) {
                            drawn[j]->selected=1;
                            direction=0;
                            break;
                        }
                    }
                }
                drawn[i]->selected=0;
            }
            if (direction==0)
                break;
        }
    }
    eaDestroy(&drawn);
/*    for (i=0;i<eaSize(&editorUIWindows[window].widgets);i++) {
        if (editorUIWindows[window].widgets[i]->draw(editorUIWindows[window].widgets[i],x,y,z,top,bottom,sc,hasFocus)) {
            change=1;
            if (editorUIWindows[window].widgets[i]->callback!=NULL)
                editorUIWindows[window].widgets[i]->callback();
        }
        y+=editorUIWindows[window].widgets[i]->elementsTall*20.0*sc;
    }
*/
    if (change && editorUIWindows[window].changeCallback)
        editorUIWindows[window].changeCallback();
    // if the window was marked for destruction, go ahead and destroy it now, since we
    // are done with it for now
    if (editorUIWindows[window].destroy)
        editorUIDestroyWindowNow(window+1);
    return 0;
}

bool editorUIIsUserInteracting(int widgetID)
{
    EditorUIWidget* parentWidget = editorUIWidgetFromID(widgetID);
    return parentWidget && parentWidget->isUserInteracting;
}
