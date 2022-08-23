#ifndef __VIRTUAL_LIST_VIEW_H__
#define __VIRTUAL_LIST_VIEW_H__
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0500
#endif
#include <winsock2.h>
#include <windows.h>
#include <commctrl.h>

typedef int (*FilterFunc) (void *);    // returns non-zero if matches the current filter
typedef int (*CompareFunc) (const void *, const void *);
typedef char * (*DisplayFunc) (void *);
typedef void (*Destructor)(void* element);
typedef bool (*FindFunc)(void * element, void * data);
typedef bool (*ColorFunc)(void * element, COLORREF* pTextColor, COLORREF* pBkColor);
typedef void (*OnNotifyCallbackFunc) (void *);


int filterNone(void * item);

typedef struct{

    char * name;
    int columnWidth;
    CompareFunc compareFunc;
    DisplayFunc displayFunc;

}VirtualListViewEntry;

typedef struct VListView VListView;

VListView *vListViewCreate();
void vListViewInit(VListView *lv, const VirtualListViewEntry vle[], HWND hDlg, HWND hDlgListView, FilterFunc filter, ColorFunc func);
BOOL vListViewOnNotify(VListView *lv, WPARAM wParam, LPARAM lParam, OnNotifyCallbackFunc callback);
void vListViewFilter(VListView * lv);
void vListViewRefresh(VListView * lv);
void vListViewRefreshItem(VListView * lv, void * item);

void vListViewInsert(VListView * lv, void * item, bool filter);
int vListViewRemove(VListView * lv, void * item); // returns -1 if item was not removed
void vListViewRemoveAll(VListView * lv, Destructor destructor);
void vListViewDestroy(VListView *lv);
void vListViewDestroyEx(VListView *lv, Destructor destructor);

void * vListViewGetSelected(VListView * lv);
int vListViewGetSize(VListView * lv);
int vListViewGetFilteredSize(VListView * lv);

void vListViewSetSortColumn(VListView * lv, int col);

void * vListViewFind(VListView * lv, void * item, FindFunc func);
void vListViewFindAndRemove(VListView * lv, void * item, FindFunc func);


typedef void (*ActionFunc)( void *item, void * params );

// perform an action on all items in the list view
void vListViewDoToAllItems( VListView * lv, ActionFunc action, void * params );


#endif //__VIRTUAL_LIST_VIEW_H__
