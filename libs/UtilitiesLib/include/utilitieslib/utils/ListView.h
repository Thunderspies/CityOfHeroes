#ifndef _XBOX

#pragma once
#include "../stdtypes.h"
#include <winsock2.h>
#include <windows.h>
#include "../components/earray.h"

typedef struct ParseTable ParseTable;

typedef struct ListView ListView;

typedef void (*Destructor)(void* element);


ListView *listViewCreate(void);
void listViewDestroy(ListView *lv);

void listViewSetSortable(ListView *lv, bool sortable);

// set the color for an item/row --- use RGB(r,g,b) macro for the two color params
void listViewSetItemColor(ListView * lv, void * structptr, int foreground, int background);
void listViewSetItemDefaultColor(ListView * lv, void * structptr);
// If color value is set to this, the row will use the default background or text color
// Since RGB values are 24 bit, this value does not match any valid color.
#define LISTVIEW_DEFAULT_COLOR    (0xee << 24)
#define LISTVIEW_BAD_VERSION_COLOR    RGB(255,255,0)

// Call this to attach this ListView to an dialog box and specific ListView control
// the tpi describes the columns that will be displayed
void listViewInit(ListView *lv, const ParseTable *tpi, HWND hDlg, HWND hDlgListView);

#define LISTVIEW_EMPTY_ITEM ((void*)(intptr_t)-1) // To be used for a spacer
// Adds an item to the list, returns the index
int listViewAddItem(ListView *lv, void *structptr);

// Removes an item from the list, returns the pointer to the struct removed (to be freed by caller)
void *listViewDelItem(ListView *lv, int index);

// Removes all items from the list
void listViewDelAllItems(ListView *lv, Destructor destructor);

// Call this when you have changed data in an item already in the listView
void listViewItemChanged(ListView *lv, void *structptr);

// Passes the data associated with each selected element back through the callback
typedef void (*ListViewCallbackFunc)(ListView *lv, void *structptr, void *data);
// Returns the number of selected items.  If callback==NULL it simply counts
int listViewDoOnSelected(ListView *lv, ListViewCallbackFunc callback, void *data);

// execute a callback function on each element
void listViewForEach(ListView * lv, ListViewCallbackFunc callback, void * data);

// Convenience functions if you don't keep your own EArray of items
void *listViewGetItem(ListView *lv, int index);
int listViewFindItem(ListView *lv, void *structptr); // Returns -1 if not found
int listViewGetNumItems(ListView *lv); // Returns the number of active items in the list view
int listViewGetLargestItemIndex(ListView *lv); // Returns the largest index into our sparse array that contains an item

HWND listViewGetParentWindow(ListView *lv);
HWND listViewGetListViewWindow(ListView *lv);
bool listViewIsSelected(ListView *lv, void *structptr);
void listViewSetSelected(ListView *lv, void *structptr, bool bSelect);

void listViewSelectAll(ListView *lv, bool bSelect);

// called when an item is selected by the user
typedef void (*ListViewItemSelectedCallbackFunc)(ListView *lv, int item_index);
// Processes WM_NOTIFY messages (will only handle ones for this specific ListView)
BOOL listViewOnNotify(ListView *lv, WPARAM wParam, LPARAM lParam, ListViewCallbackFunc callback);

// Forces the listView to get sorted (normally, listViewOnNotify will handle user requests to sort)
void listViewSort(ListView *lv, int column);
void listViewReverseSort(ListView *lv, int column);
void listViewEnableColor(ListView * lv, bool enable);

#endif
