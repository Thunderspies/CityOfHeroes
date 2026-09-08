#ifndef EDITORUI_H
#define EDITORUI_H

/***********************************************************************
 *
 * The EditorUI - Making sweet UIs is now both fun *and* easy!
 *
 * EditorUIs are standard game UIs that appear in the editor, they are 
 * easy to create, and once created will maintain themselves.  Creating
 * new widgets for them is also easy.
 *
 * Creating an Editor UI
 * 1. Create the UI
 *        Call editorUICreateWindow().  This returns an ID which you will
 *        use to add widgets to your window.
 *
 * 2. Add the widgets
 *        Call editorUIAdd*().  This will add various widgets to your UI,
 *        which will show up in the UI in the order you add them.  You
 *        will need to pass these functions the ID of your window.
 *
 * 3. Add callbacks
 *        You can add a close callback that will be called when your
 *        window is closed (either by closing normally or if you destroy
 *        it yourself).  You can add this by calling
 *        editorUISetCloseCallback().  This is not required.
 *        Widgets also take callback functions.  These will be called when
 *        the individual widget is changed, and before the main window
 *        callback is called.
 *
 * 4. ...
 *        Now the window will activate and things will just work.  At
 *        this points you no longer need to keep track of the ID, unless
 *        you want to close the window yourself at some point (like as
 *        a result of them clicking a 'Cancel' button or something.
 *
 * 5. Destroy the window
 *        The window will be destroyed automatically when it is closed.
 *        You can also do this yourself by calling editorUIDestroyWindow()
 *        and passing it the ID of your window.
 *
 ***********************************************************************
 *
 * Creating new widgets
 *
 * To add a new widget to the EditorUI you need to create one struct and
 * three functions.  As an example we will create an EditorUIFoo.  The 
 * first member of EditorUIFoo must be EditorUIWidget.  This allows an
 * EditorUIFoo pointer to be cast to an EditorUIWidget pointer.  Now we
 * must create editorUIAddFoo.  This takes an ID which tells us what 
 * window it is being added to, as well as any other parameters that we
 * need for our Foo widget.  This function sets up an EditorUIFoo struct
 * including the parent information in EditorUIWidget.  And when it is
 * done it pushes it onto the list of widgets for the appropriate window
 * like this: eaPush(&editorUIWindows[ID].widgets,widget);
 * Now we also need a destroy function.  This function takes an
 * EditorUIWidget pointer, casts it to an EditorUIFoo pointer, and 
 * releases any memory it has allocated.
 * Finally we need editorUIDisplayFoo().  This function takes parameters
 * telling us where it needs to get displayed, and we are responsible
 * for displaying it.  This function should return true if the user has
 * changed something (checked a box, for example), and false otherwise.
 * Looking at other widgets should give you a pretty good idea of how 
 * to make new widgets.
 *
 ***********************************************************************/

// enums, if needed
typedef enum {
    EDITORUI_NULL,
    EDITORUI_CHECKBOX,
    EDITORUI_CHECKBOXLIST,
    EDITORUI_RADIOBUTTONS,
    EDITORUI_STATICTEXT,
    EDITORUI_DYNAMICTEXT,
    EDITORUI_TEXTENTRY,
    EDITORUI_SLIDER,
    EDITORUI_EDITSLIDER,
    EDITORUI_LIST,
    EDITORUI_COMBOBOX,
    EDITORUI_BUTTON,
    EDITORUI_BUTTONROW,
    EDITORUI_TABCONTROL,
    EDITORUI_PROGRESSBAR,
    EDITORUI_CONSOLE,
    EDITORUI_SCROLLABLELIST,
    EDITORUI_TREECONTROL,
    EDITORUI_BREAKDOWNBAR,
    EDITORUI_COLORSWATCH,
    EDITORUI_HUESELECTOR,
    EDITORUI_SATURATIONVALUESELECTOR,
} EditorUIType;

typedef void(*EditorUICallback)(int);

// this function is used by every editor window to get drawn
int editorUIDrawWindow(void);


// call this to get an ID for a window.  A return value of -1 indicates that
// there are no windows available.
int editorUICreateWindow(const char *title);

// Set the dimensions of the window
void editorUISetSize(int ID, float width, float height);
void editorUIGetSize(int ID, float *width, float *height);
void editorUISetPosition(int ID, float x, float y);
void editorUIGetPosition(int ID, float *x, float *y);
void editorUICenterWindow(int ID);
void editorUISetModal(int ID, int isModal);
void editorUISetRelativeModal(int ID, int whichWindow);
void editorUISetNoClose(int ID, int noClose);
void editorUISetMEOnly(int ID, int meOnly);

// Use these functions to add widgets to your window
int editorUIAddCheckBox(int ID,int * val,EditorUICallback callback,const char * text);
int editorUIAddCheckBoxList(int ID,int * val,EditorUICallback callback,const char ** textList);

int editorUIAddRadioButtons(int ID,int displayHorizontal,int * val,EditorUICallback callback,...);
int editorUIAddRadioButtonsFromArray(int ID,int * val,EditorUICallback callback,char ** list);
int editorUIAddRadioButtonsFromEArray(int ID,int * val,EditorUICallback callback,char ** list);

int editorUIAddTextEntry(int ID,char * val,int capacity,EditorUICallback callback,const char * text);

int editorUIAddStaticText(int ID,const char * text);

int editorUIAddDynamicText(int ID,void (*textCallback)(char *));

int editorUIAddSlider(int ID,float * val,float min,float max,int integer,EditorUICallback callback,const char * text);

int editorUIAddEditSlider(int ID,float * val,float smin,float smax,float emin, float emax, int integer,EditorUICallback callback,const char * text);

int editorUIAddComboBox(int ID,int * value,char * text,EditorUICallback callback,...);
int editorUIAddComboBoxFromArray(int ID,int * value,char * text,EditorUICallback callback,char ** list);
int editorUIAddComboBoxFromEArray(int ID,int * value,char * text,EditorUICallback callback,char ** list);

int editorUIAddComboTextBoxFromEArray(int ID,char * value,int capacity,char * text,char ** list);

int editorUIAddButtonRow(int ID,EditorUICallback callback,...);
int editorUIAddIconRow(int ID,int width,int height,...);

int editorUIAddButton(int ID,char* buttonDesc,char* buttonText,void (*clickFunction)(char *));
void editorUIButtonChangeText(int widgetID, char *buttonText);

int editorUIAddList(int ID,int * val,char * text,EditorUICallback callback,...);
int editorUIAddListFromArray(int ID,int * val,char * text,EditorUICallback callback,char ** list);
int editorUIAddListFromEArray(int ID,int * val,char * text,EditorUICallback callback,char ** list);

int editorUIAddSpace(int ID);
int editorUIAddScrollableList(int ID,int * val,char ** listItems,int height,int width,EditorUICallback callback);

int editorUIAddTabControl(int ID,int * val,EditorUICallback callback,int (*displayCountCallback)(void),...);
int editorUIAddTabControlFromEArray(int ID,int * val,EditorUICallback callback,int (*displayCountCallback)(void),char ** tabs);

void editorUITabControlAddTab(int widgetID, char * tabString);
void editorUITabControlDeleteTab(int widgetID, int tabIndex);
void editorUITabControlRenameTab(int widgetID, int tabIndex, char * newName);

int editorUIAddConsole(int ID,int bgColor,int linesTall,int minWidth);
void editorUIConsoleAddLine(int widgetID,char * string,int color);

// UI element that calls the specified callback everytime its draw function is called.
int editorUIAddDrawCallback(int ID,EditorUICallback callback);

int editorUIAddProgressBar(int ID,float (*getPercentCallback)(void));

int editorUIAddBreakdownBar(int ID,float (*getPercentCallback)(int,int *),int numDivisions);

int editorUIAddDualColorSwatch(int ID, U8* color1, U8* color2);

int editorUIAddHueSelector(int ID, float * value, EditorUICallback callback);

int editorUIAddSaturationValueSelector(int ID, float * value, EditorUICallback callback);

typedef struct TreeElementAddress
{
    int *address;
} TreeElementAddress;
void treeAddressPush(TreeElementAddress *address, int element);
int treeAddressPop(TreeElementAddress *address);
void freeTreeAddressData(TreeElementAddress *address);

int editorUIAddTreeControl(int ID,EditorUICallback callback,int (*elementCallback)(char*namebuf, const TreeElementAddress *elem),int multi_select,int multi_open,const TreeElementAddress *init_selected);
int editorUITreeControlGetSelected(int widgetID, TreeElementAddress *addresses, int max_count);
void editorUITreeControlRefreshList(int widgetID);
void editorUITreeControlSelect(int widgetID, const TreeElementAddress* select_address);
void editorUITreeControlDeselect(int widgetID, const TreeElementAddress* select_address);

// these need explanation
int  editorUIAddSubWidgets(int ID,int (*showSubWidgets)(int),int showFrame,int *lockWidgets);
void editorUIEndSubWidgets(int ID);
int editorUIShowByParentSelection(int ID);

// use this to remove the last widget, if necessary i can make adding and removing
// widgets a little more general
void editorUIRemoveLastWidget(int ID);

// Set the callback param if you intend on using it
void editorUISetWidgetCallbackParam(int widgetID, int callbackParam);

int editorUIGetWindow(int offset);
void editorUIDestroyWindow(int ID);

void editorUISetCloseCallback(int ID,void (*closeCallback)());
void editorUISetChangeCallback(int ID,void (*changeCallback)());

bool editorUIIsUserInteracting(int widgetID);

#endif
