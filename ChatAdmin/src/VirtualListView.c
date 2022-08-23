#include <utilitieslib/stdtypes.h>
#include "VirtualListView.h"
#include <utilitieslib/utils/wininclude.h>
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/utils/textparser.h>
#include <utilitieslib/utils/mathutil.h>
#include "LangAdminUtil.h"
#include <commctrl.h>
#include <assert.h>

#pragma comment (lib, "comctl32.lib")

int filterNone(void * item)
{
    return 0;
}

void vListViewSort(VListView *lv);



typedef struct VListView {
    const VirtualListViewEntry *vle;
    HWND hDlgParent;
    HWND hDlgListView;
    mEArrayHandle elements;        // master list of all elements
    mEArrayHandle filtered;        // list of all elements that are displayed (chosen by 'filterFunc')



    FilterFunc    filterFunc;        // determines which elements are visible
    ColorFunc    colorFunc;        // determines row coloring

//    char *        lastFilter;

//    int iSubItem;                // When used in a callback, the column to be sorted on
    int sortCol;    // which column is currently used to sort the entries
    bool sorted;    // flag set when list view needs to be sorted (the next time vListViewFilter() is called)
} VListView;


VListView *vListViewCreate()
{
    VListView *lv = calloc(sizeof(VListView), 1);
    eaCreate(&lv->elements);
    eaCreate(&lv->filtered);
    return lv;
}

void vListViewDestroy(VListView *lv)
{
    eaDestroy(&lv->filtered);
    eaDestroy(&lv->elements);
    free(lv);
}


void vListViewDestroyEx(VListView *lv, Destructor destructor)
{
    eaDestroy(&lv->filtered);
    eaClearEx(&lv->elements, destructor);
    free(lv);
}

// find where item belongs using binary search
static int sortedListInsertHelper(mEArrayHandle *pArray, void * item, CompareFunc compare)
{
    int size = eaSizeUnsafe(pArray);
    int low = 0;
    int high = size - 1;
    mEArrayHandle array = *pArray;

    int r = 0, pos = 0;

    while( low <= high )
    {
        pos = ( low  + high ) / 2;

        r = compare(&item, &array[ pos ]); 

        if( r < 0 )            // search low end
            high = pos - 1;        
        else if( r > 0 )    //search high end
            low = pos + 1;
        else
            return pos;        // dulpicate exists... 
    }

    if(r < 0)
        return pos;
    else
        return low;
}
// assumes list has been sorted
static int sortedListInsert(mEArrayHandle *pArray, void * item, CompareFunc compare)
{
    int pos = sortedListInsertHelper(pArray, item, compare);
    eaInsert(pArray, item, pos);
    return pos;
}

void vListViewInsert(VListView * lv, void * item, bool sortAndFilter)
{
    if(sortAndFilter)
    {
        CompareFunc compare = lv->vle[lv->sortCol].compareFunc;

        assert(compare);

        if(!lv->sorted)
            vListViewSort(lv);

        sortedListInsert(&lv->elements, item, compare);
        if( ! lv->filterFunc(item))
        {
            LVITEM lvItem;

            ZeroStruct(&lvItem);
            lvItem.iItem = sortedListInsert(&lv->filtered, item, compare);
            ListView_InsertItem(lv->hDlgListView, &lvItem);    // keep current selections    
            
            vListViewRefresh(lv);
        }
    }
    else
    {
        // assume user will call vListViewSort() and vListViewFilter() later
        eaPush(&lv->elements, item);
        lv->sorted = 0;
    }
}

static int sortedListFind(mEArrayHandle *pArray, void * item, CompareFunc compare)
{
    void ** p = bsearch(&item, *pArray, eaSizeUnsafe(pArray), sizeof(void*), compare);
    if(p)
        return (p - *pArray);
    else
        return -1;

}

static int sortedListRemove(mEArrayHandle *pArray, void * item, CompareFunc compare)
{
    int pos = sortedListFind(pArray, item, compare);
    if(pos >= 0)
        eaRemove(pArray, pos);
    
    return pos;
}

int vListViewRemove(VListView * lv, void * item)
{
    int pos, ret;
    CompareFunc compare = lv->vle[lv->sortCol].compareFunc;

    ret = sortedListRemove(&lv->elements, item, compare);

    pos = sortedListRemove(&lv->filtered, item, compare);

    if(pos >= 0)
    {
        ListView_DeleteItem(lv->hDlgListView, pos);    // keep current selections    
        vListViewRefresh(lv);
    }

    return ret;
}

void vListViewRemoveAll(VListView * lv, Destructor destructor)
{
    if(destructor)
        eaClearEx(&lv->elements, destructor);

    eaSetSize(&lv->elements, 0);
    eaSetSize(&lv->filtered, 0);    
    vListViewRefresh(lv);

}



void vListViewRefresh(VListView * lv)
{
    ListView_SetItemCountEx(lv->hDlgListView, eaSizeUnsafe(&lv->filtered), LVSICF_NOINVALIDATEALL);
    InvalidateRect(lv->hDlgListView, NULL, TRUE);
    UpdateWindow(lv->hDlgListView);
}

void vListViewRefreshItem(VListView * lv, void * item)
{
    CompareFunc compare = lv->vle[lv->sortCol].compareFunc;
    int pos = sortedListFind(&lv->filtered, item, compare);

    if(pos >= 0)
        ListView_RedrawItems(lv->hDlgListView, pos, pos);
}


static int vListViewInitInternal(const VirtualListViewEntry vle[], HWND hDlgListView)
{
    LV_COLUMN lvC;
    int i;
    char buf[1024];
    int count = 0;

    // Add the columns
    lvC.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
    lvC.fmt = LVCFMT_LEFT;
    lvC.cx = 75;
    lvC.pszText = buf;

    // Load the column list from the vle
    for (i = 0; (vle[i].name && vle[i].name[0]); i++)
    {
        lvC.cx = vle[i].columnWidth;

        lvC.iSubItem = count;
        strcpy(buf, localizedPrintf(vle[i].name));
        if(ListView_InsertColumn(hDlgListView, count, &lvC) == -1) {
            assert(0);
        }
        count++;
    }

    return count;
}



void vListViewInit(VListView *lv, const VirtualListViewEntry vle[], HWND hDlg, HWND hDlgListView, FilterFunc filter, ColorFunc color)
{
    lv->vle = vle;
    lv->hDlgParent = hDlg;
    lv->hDlgListView = hDlgListView;
    lv->filterFunc = filter;
    lv->colorFunc = color;
    vListViewInitInternal(vle, hDlgListView);

    // entire row will be highlighted on selection
    ListView_SetExtendedListViewStyle(hDlgListView, LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE);
}






void vListViewSort(VListView *lv)
{
    CompareFunc compare = lv->vle[lv->sortCol].compareFunc;
    assert(compare);

    loadstart_printf("Sorting %d entries", eaSizeUnsafe(&lv->elements));

    qsort(lv->elements, eaSizeUnsafe(&lv->elements), sizeof(void*), compare);
    vListViewRefresh(lv);
    lv->sorted = 1;


    loadend_printf("..done");
}


static int findFirstFilterMatch(mEArrayHandle *pArray, FilterFunc filter)
{
    int size = eaSizeUnsafe(pArray);
    int low = 0;
    int high = size - 1;
    mEArrayHandle array = *pArray;

    int r = 0, pos;

    while( low <= high )
    {
        pos = ( low  + high ) / 2;

        r = filter(array[ pos ]); 

        if( r > 0 )            // search low end
            high = pos - 1;        
        else if( r < 0 )    //search high end
            low = pos + 1;
        else
            high = pos - 1;        // found one match, now look for beginning (this might be lumped in with 'if' above)
    }

    return low;
}

void vListViewFilter(VListView * lv)
{
    FilterFunc filter = lv->filterFunc;
    int i, size=eaSizeUnsafe(&lv->elements);
    mEArrayHandle* pArray = &lv->filtered;


    if(!lv->sorted)
        vListViewSort(lv);

    loadstart_printf("Filtering...");

    eaSetSize(pArray, 0);

    i = findFirstFilterMatch(&lv->elements, filter);
    for(i;i<size;i++)
    {
        void * item = lv->elements[i];
        if( ! filter(item))
            eaPush(pArray, item);
        else
            break;
    }

    loadend_printf("Found %d matches",     eaSizeUnsafe(&lv->filtered));

    vListViewRefresh(lv);
}



void * vListViewGetSelected(VListView * lv)
{
    int count = ListView_GetSelectedCount(lv->hDlgListView);
    if(count)
    {
        int idx = ListView_GetNextItem(lv->hDlgListView, -1, LVNI_SELECTED);
        if(idx >= 0 && idx < eaSizeUnsafe(&lv->filtered))
            return lv->filtered[ idx ];        
    }
    
    return 0;
}

void vListViewSetSortColumn(VListView * lv, int col)
{
    if(lv->sortCol != col && lv->vle[col].compareFunc)
    {
        lv->sortCol = col;
        vListViewSort(lv);
    }
}

static LRESULT ProcessCustomDraw (VListView * lv, LPARAM lParam)
{
    LPNMLVCUSTOMDRAW lplvcd = (LPNMLVCUSTOMDRAW)lParam;

    switch(lplvcd->nmcd.dwDrawStage) 
    {
    case CDDS_PREPAINT : //Before the paint cycle begins
        //request notifications for individual listview items
        return CDRF_NOTIFYITEMDRAW;

    xcase CDDS_ITEMPREPAINT: //Before an item is drawn
        {
            int idx = lplvcd->nmcd.dwItemSpec;

            if(idx >= 0 && idx < eaSizeUnsafe(&lv->filtered))
            {
                if(lv->colorFunc)
                {
                    lv->colorFunc(lv->filtered[idx], &lplvcd->clrText, &lplvcd->clrTextBk);
                    return CDRF_NEWFONT;
                }

                //return CDRF_NEWFONT;
            }
        }
        break;
    }
    return CDRF_DODEFAULT;
}

BOOL vListViewOnNotify(VListView *lv, WPARAM wParam, LPARAM lParam, OnNotifyCallbackFunc callback)
{
    int idCtrl = (int)wParam;
    if (!lv->hDlgListView || lv->hDlgListView != GetDlgItem(lv->hDlgParent, idCtrl)) {
        // Not us!
        return FALSE;
    }
    switch (((LPNMHDR) lParam)->code)
    {
    case LVN_KEYDOWN:
    {
        if(callback)
        {
            LPNMLVKEYDOWN lpnmkey = (LPNMLVKEYDOWN)lParam;
            int idx = ListView_GetSelectionMark(lv->hDlgListView);
            switch(lpnmkey->wVKey)
            {
            case VK_UP:
                {
                    --idx;
                    if( idx >= 0 && idx < eaSizeUnsafe(&lv->filtered))
                        callback(lv->filtered[idx]);
                }
            xcase VK_DOWN:
                {    
                    ++idx;
                    if( idx >= 0 && idx < eaSizeUnsafe(&lv->filtered))
                        callback(lv->filtered[idx]);
                }
            }
        }
    }
    xcase NM_CLICK:
    {
        LPNMITEMACTIVATE lpnmitem = (LPNMITEMACTIVATE) lParam;
        //LVHITTESTINFO lvhti;
        //LVITEM lvi;
        //if (lpnmitem->iItem==-1)
        //{
        //    // The click won't be handled properly
        //    //  This is sorta a hack so that we get some functionality when you click on other subitems, but not as good as it could be
        //    lvhti.flags = LVHT_ONITEM;
        //    lvhti.pt = lpnmitem->ptAction;
        //    ListView_SubItemHitTest(lv->hDlgListView, &lvhti);
        //    if (lvhti.iItem == -1) {
        //        // We don't care about the X-coord, since we're assuming this is in Details/Report view
        //        lvhti.flags = LVHT_ONITEM;
        //        lvhti.pt.x = 2;
        //        lvhti.iItem = ListView_HitTest(lv->hDlgListView, &lvhti);
        //    }
        //    lpnmitem->iItem = lvhti.iItem;
        //    if (lpnmitem->iItem!=-1) {
        //        lvi.iItem = lpnmitem->iItem;
        //        lvi.mask = LVIF_STATE;
        //        lvi.stateMask = LVIS_SELECTED;
        //        ListView_GetItem(lv->hDlgListView, &lvi);
        //        lvi.state = ~lvi.state;
        //        ListView_SetItem(lv->hDlgListView, &lvi);
        //        if(lvi.state & LVIS_SELECTED)
        //        {
        //            int idx = lpnmitem->iItem;
        //            if(idx >= 0 && idx < eaSizeUnsafe(&lv->filtered))
        //                callback(lv->filtered[idx]);
        //        }
        //    }
        //}
        //else
        {
            if(callback)
            {
                int idx = ListView_GetSelectionMark(lv->hDlgListView);
                if(idx >= 0 && idx < eaSizeUnsafe(&lv->filtered))
                    callback(lv->filtered[idx]);
            }
        }
    }

    //xcase LVN_ITEMACTIVATE:
    //{
    //    void * item = vListViewGetSelected(lv);
    //    if(callback && item)
    //        callback(item);
    //}
    xcase LVN_COLUMNCLICK:
        {
            NMLISTVIEW *pnmv = (LPNMLISTVIEW)lParam;
            vListViewSetSortColumn(lv, pnmv->iSubItem);
        }
    xcase NM_KILLFOCUS:
        return TRUE;
    xcase NM_CUSTOMDRAW:
        {
            if(lv->colorFunc)
            {
                SetWindowLongPtr(lv->hDlgParent, DWLP_MSGRESULT, (LONG_PTR)ProcessCustomDraw(lv, lParam));
                return TRUE;
            }
        }
    xcase LVN_GETDISPINFO:
        {
            LVITEM* pItem = &(((NMLVDISPINFO*) lParam)->item);
            if(pItem->iItem < eaSizeUnsafe(&lv->filtered))
            {
                DisplayFunc display = lv->vle[pItem->iSubItem].displayFunc;
                if(display)
                    pItem->pszText = display(lv->filtered[pItem->iItem]);
                else
                    pItem->pszText = "No Display Function Specified!";
            }
        }
    }

    return FALSE;
}


void * vListViewFind(VListView * lv, void * item, FindFunc func)
{
    int i;
    for(i=eaSizeUnsafe(&lv->elements)-1;i>=0;--i)
    {
        if(func)
        {
            if(func(lv->elements[i], item))
                return lv->elements[i];
        }
        else
        {
            if(lv->elements[i] == item)
                return lv->elements[i];
        }
    }
    return 0;
}

void vListViewFindAndRemove(VListView * lv, void * item, FindFunc func)
{
    void * res = vListViewFind(lv, item, func);
    if(res)
        vListViewRemove(lv, res);   
}

int vListViewGetSize(VListView * lv)
{
    return (lv ? eaSizeUnsafe(&lv->elements) : 0);
}

int vListViewGetFilteredSize(VListView * lv)
{
    return (lv ? eaSizeUnsafe(&lv->filtered) : 0);
}

void vListViewDoToAllItems( VListView * lv, ActionFunc action, void * params )
{
    int i;
    for(i=eaSizeUnsafe(&lv->elements)-1;i>=0;--i)
    {
        action(lv->elements[i], params);
    }
}
