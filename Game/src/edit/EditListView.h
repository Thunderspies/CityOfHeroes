#ifndef EDIT_LIST_VIEW_H
#define EDIT_LIST_VIEW_H

#define EDIT_LIST_VIEW_UPPERLEFT    1
#define EDIT_LIST_VIEW_UPPERRIGHT    2
#define EDIT_LIST_VIEW_LOWERLEFT    3
#define EDIT_LIST_VIEW_LOWERRIGHT    4

#define EDIT_LIST_VIEW_SELECTED        0xFFFFFFFF
#define EDIT_LIST_VIEW_DIRECTORY    0xFFFF00FF
#define EDIT_LIST_VIEW_MOUSEOVER    0xFF0000FF
#define EDIT_LIST_VIEW_STANDARD        0x00FF00FF
#define EDIT_LIST_VIEW_INACTIVE        0x3F3FFFFF
#define EDIT_LIST_VIEW_ACTIVE        0xFFFF00FF

/* Generic struct for displaying a list of strings, used by Menu 
 *
 *
**/

typedef struct ClickInfo {
    int line;
    int x;        //distance from the left side of the ELV
} ClickInfo;

typedef struct EditListView EditListView;
typedef struct EditListView {
    int x,y;    //the x and y coordinates of theCorner of the menu
                //in window coordinates, relative to theWindowCorner
                //both default to reffering to the upper left corner
    int finalx,finaly;        //coorinates after transformation
    int theCorner;            //which corner of the Menu x and y apply to
    int theWindowCorner;    //which corner of the window x and y are relative to
    int width,height;
    int hasFocus;
    int level;                //determines which menu is drawn in front when there are overlaps
    int colorColumns;        //true if the mouseover color is applied to individual columns

    //scrolling information
    int amountScrolled;            //how many pixels down the user has scrolled
    int totalItemsDisplayed;    //number of items that are able to be displayed
                                //figured from the last time things were displayed
    int lineHeight;                //lineHeight
    int realHeight;                //real height, considering not the entire height is used
    ClickInfo clickInfo;        //information about what was clicked, and where
    int lineClicked;            //which line was recently clicked on the screen
    int lineOver;                //which line was recently mouse overed
    int widthOver;                //x distance from the left side of the menu that the mouse is over

    int growUp;                //1 iff the menu is supposed to grow upwards as it gets longer
                            //this means that the corner specified is actually the LOWER left corner
                            //instead of the upper left
    int titleBarMove;        //1 iff the title bar is being dragged
    int titleLastClicked;    //stores when the title bar was last clicked
    int titleBarDoubleClick;//1 iff the title bar was just double clicked
    int hidden;                //1 iff nothing should be displayed
    int hiddenTime;            //the time that hidden was set to true
    int hiddenLastMoveTime;    //the last time that the menu was moved by the hiding function
    int hiddenOffsetX;        //added to the coordinates of the Menu's location, for hiding it
    int shrunk;                //1 if only the title bar should be displayed, if anything
    int windowResize;        //1 iff the window is being resized
    int scrollBarMove;        //1 iff the scroll bar is being dragged
    int scrollBarActive;    //1 iff the scroll bar is visible (i.e. there are more items than can be visible)
    int scrollBarStart;
    int scrollBarEnd;
    int resizerActive;        //1 iff the resizer is showing, which happens iff height==maxHeight
    int resizerMove;        //1 iff the resizer is being dragged
    int clickx,clicky;        //used for dragging, stores where the menubar (for example) was clicked

    int columnWidth;        //width of the columns in pixels

    void (*getList)(int start,int end,char ** text,int * colors,int * max,void * info);    //function that gives the ListView the items to be displayed
                                                                //first string to not be used should be NULL, ListView will
                                                                //not display past that
    void (*getSelected)(int ** lineNums,int * size);        //used to find which lines are selected so they
                                                        //can be marked on the scroll bar, this can be NULL
    void * info;

    //if this ELV is latched on to another ELV, then one or more of these will be non-NULL
    EditListView * leftLatch;
    EditListView * rightLatch;
    EditListView * topLatch;
    EditListView * bottomLatch;
} EditListView;

EditListView * newELV(int x,int y,int width,int height);
void destroyELV(EditListView * elv);

void setELVTextCallback(EditListView * lv,void (*getList)(int start,int end,char ** text,int * colors,int * max,void * info));
void setELVSelectedCallback(EditListView * lv,void (*getSelected)(int ** lineNums,int * size));
void setELVInfo(EditListView * lv,void * info);

int  busyELV(EditListView *);
void hideELV(EditListView *);
void handleELV(EditListView *);
void displayELV(EditListView *);

void hideAllELV();
void handleAllELV();
void displayAllELV();
int anyHaveFocusELV();
void ELVLatchTopBottom(EditListView * top,EditListView * bottom);
void ELVLatchLeftRight(EditListView * left,EditListView * right);

#endif
