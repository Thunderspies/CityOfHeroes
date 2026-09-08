#ifndef _XBOX

#include "utilitieslib/stdtypes.h"

#include "utilitieslib/utils/ListView.h"
#include "utilitieslib/utils/StringUtil.h"
#include <commctrl.h>
#include "utilitieslib/assert/assert.h"
#include "utilitieslib/utils/utils.h"
#include "utilitieslib/components/earray.h"
#include "utilitieslib/utils/textparser.h"
#include "utilitieslib/utils/structInternals.h"
#include "utilitieslib/utils/mathutil.h"
#include "utilitieslib/utils/file.h"
#include "utilitieslib/utils/tokenstore.h"

#pragma comment (lib, "comctl32.lib")

#define SNDMSGW SendMessageW

// #define ListView_DeleteAllItemsW(hwnd) \
//     (BOOL)SNDMSGW((hwnd), LVM_DELETEALLITEMS, 0, 0L)

// #define ListView_DeleteItemW(hwnd, i) \
//     (BOOL)SNDMSGW((hwnd), LVM_DELETEITEM, (WPARAM)(int)(i), 0L)

// #define ListView_EnsureVisibleW(hwndLV, i, fPartialOK) \
//     (BOOL)SNDMSGW((hwndLV), LVM_ENSUREVISIBLE, (WPARAM)(int)(i), MAKELPARAM((fPartialOK), 0))

// #define ListView_FindItemW(hwnd, iStart, plvfi) \
//     (int)SNDMSGW((hwnd), LVM_FINDITEMW, (WPARAM)(int)(iStart), (LPARAM)(const LV_FINDINFOW *)(plvfi))

#define ListView_GetItemW(hwnd, pitem) \
    (BOOL)SNDMSGW((hwnd), LVM_GETITEMW, 0, (LPARAM)(LV_ITEMW *)(pitem))

// #define ListView_GetItemCount W(hwnd) \
//     (int)SNDMSGW((hwnd), LVM_GETITEMCOUNT, 0, 0L)

// #define ListView_GetNextItemW(hwnd, i, flags) \
//     (int)SNDMSGW((hwnd), LVM_GETNEXTITEM, (WPARAM)(int)(i), MAKELPARAM((flags), 0))

// #define ListView_GetSelectionMarkW(hwnd) \
//     (int)SNDMSGW((hwnd), LVM_GETSELECTIONMARK, 0, 0)

// #define ListView_HitTestW(hwndLV, pinfo) \
//     (int)SNDMSGW((hwndLV), LVM_HITTEST, 0, (LPARAM)(LV_HITTESTINFO *)(pinfo))

#define ListView_InsertColumnW(hwnd, iCol, pcol) \
     (int)SNDMSGW((hwnd), LVM_INSERTCOLUMNW, (WPARAM)(int)(iCol), (LPARAM)(const LV_COLUMNW *)(pcol))

#define ListView_InsertItemW(hwnd, pitem)   \
    (int)SNDMSGW((hwnd), LVM_INSERTITEMW, 0, (LPARAM)(const LV_ITEMW *)(pitem))

// #define ListView_SetExtendedListViewStyleW(hwndLV, dw)\
//         (DWORD)SNDMSGW((hwndLV), LVM_SETEXTENDEDLISTVIEWSTYLE, 0, dw)

 #define ListView_SetItemW(hwnd, pitem) \
     (BOOL)SNDMSGW((hwnd), LVM_SETITEMW, 0, (LPARAM)(const LV_ITEMW *)(pitem))

// #define ListView_SetItemStateW(hwndLV, i, data, mask) \
// { LV_ITEM _ms_lvi;\
//   _ms_lvi.stateMask = mask;\
//   _ms_lvi.state = data;\
//   SNDMSGW((hwndLV), LVM_SETITEMSTATE, (WPARAM)(i), (LPARAM)(LV_ITEM *)&_ms_lvi);\
// }

// #define ListView_SetSelectionMarkW(hwnd, i) \
//     (int)SNDMSGW((hwnd), LVM_SETSELECTIONMARK, 0, (LPARAM)(i))

// #define ListView_SortItems W(hwndLV, _pfnCompare, _lPrm) \
//   (BOOL)SNDMSGW((hwndLV), LVM_SORTITEMS, (WPARAM)(LPARAM)(_lPrm), \
//   (LPARAM)(PFNLVCOMPARE)(_pfnCompare))

// #define ListView_SubItemHitTestW(hwnd, plvhti) \
//         (int)SNDMSGW((hwnd), LVM_SUBITEMHITTEST, 0, (LPARAM)(LPLVHITTESTINFO)(plvhti))


typedef struct ListView {
    const ParseTable *tpi;
    HWND hDlgParent;
    HWND hDlgListView;
    int iNumItems; // Size of EArray, not necessarily the number of items visible in the list
    int iNumColumns;
    mEArrayHandle eaElements_data;
    mEArrayHandle *eaElements;
    meaiHandle eaForegroundColors;
    meaiHandle eaBackgroundColors;

    int iSubItem; // When used in a callback, the column to be sorted on
    intptr_t *subItemMap; // Map of what subitems go to what elements in the TPI..., includes subelements that point to more arrays
    bool sortable;
    bool colored;
} ListView;


ListView *listViewCreate(void)
{
    ListView *lv = calloc(sizeof(ListView), 1);
    lv->eaElements = &lv->eaElements_data;
    eaCreate(lv->eaElements);
    eaiCreate(&lv->eaForegroundColors);
    eaiCreate(&lv->eaBackgroundColors);
    lv->sortable = true;
    lv->colored = true;    // this is set to true when any item's color is set with listViewSetItemColor()
    return lv;
}

void listViewDestroy(ListView *lv)
{
    eaDestroy(lv->eaElements);
    eaiDestroy(&lv->eaForegroundColors);
    eaiDestroy(&lv->eaBackgroundColors);
    free(lv);
}

void listViewSetSortable(ListView *lv, bool sortable)
{
    lv->sortable = sortable;
}



static int listViewInitInternal(const ParseTable tpi[], HWND hDlgListView, int count, intptr_t **subItemMap)
{
    LV_COLUMNW lvC;
    int i;
    wchar_t buf[1024];

    // Add the columns
    lvC.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
    lvC.fmt = LVCFMT_LEFT;
    lvC.cx = 75;
    lvC.pszText = buf;
    if (*subItemMap==NULL) {
        *subItemMap = calloc(ParseTableCountFields((ParseTable*)tpi), sizeof(intptr_t));
    }

    // Load the column list from the tpi
    FORALL_PARSEINFO(tpi, i)
    {
        if(tpi[i].type & TOK_REDUNDANTNAME)
            continue;

        lvC.cx = TOK_FORMAT_GET_LVWIDTH_DEF(tpi[i].format, 75);

        switch (TOK_GET_TYPE(tpi[i].type))
        {
        case TOK_BOOLFLAG_X:
        case TOK_BOOL_X:
        case TOK_INT64_X:
        case TOK_INT16_X:
        case TOK_INT_X:
        case TOK_STRING_X:
        case TOK_U8_X:
        case TOK_F32_X:
            (*subItemMap)[i] = count;
            lvC.iSubItem = count;
            UTF8ToWideStrConvert(tpi[i].name,buf,ARRAY_SIZE(buf));
            if(ListView_InsertColumnW(hDlgListView, count, &lvC) == -1) {
                assert(0);
            }
            count++;
            break;
        case TOK_STRUCT_X:
            count=listViewInitInternal((ParseTable*)tpi[i].subtable, hDlgListView, count, (intptr_t**)&(*subItemMap)[i]);
            break;
        case TOK_START:
        case TOK_END:
        case TOK_IGNORE:
            break;
        default:
            assert(!"This type not supported by ListView");
            break;
        }
    }

    return count;
}



void listViewInit(ListView *lv, const ParseTable tpi[], HWND hDlg, HWND hDlgListView)
{

    lv->tpi = tpi;
    lv->hDlgParent = hDlg;
    lv->hDlgListView = hDlgListView;
    lv->iNumItems = 0;
    lv->subItemMap = NULL;
    lv->iNumColumns = listViewInitInternal(tpi, hDlgListView, 0, &lv->subItemMap);

    // entire row will be highlighted on selection
    ListView_SetExtendedListViewStyle(hDlgListView, LVS_EX_FULLROWSELECT);
}

static bool getElement(const ParseTable tpi[], void *structptr, int index, intptr_t *subItemMap,
                      OUT ParseTable const **ptpi, OUT void **element)
{
    int i;
    FORALL_PARSEINFO(tpi, i)
    {
        if(tpi[i].type & TOK_REDUNDANTNAME)
            continue;

        switch (TOK_GET_TYPE(tpi[i].type))
        {
        case TOK_BOOLFLAG_X:
        case TOK_BOOL_X:
        case TOK_INT64_X:
        case TOK_INT16_X:
        case TOK_INT_X:
        case TOK_STRING_X:
        case TOK_U8_X:
        case TOK_F32_X:
            if (subItemMap[i] == index) {
                *ptpi = &tpi[i];
                *element = structptr;
                return true;
            }
            break;
        case TOK_STRUCT_X:
            {
                void* substruct = TokenStoreGetPointer((ParseTable*)tpi, i, structptr, 0);
                if (!substruct) break;
                if (getElement((ParseTable*)tpi[i].subtable, substruct, index, (intptr_t*)(subItemMap[i]), ptpi, element))
                    return true;
            }
            break;
        case TOK_START:
        case TOK_END:
        case TOK_IGNORE:
            break;
        default:
            assert(!"This type not supported by ListView");
            break;
        }
    }
    *ptpi = NULL;
    *element = NULL;
    return false;
}


static void listViewUpdateItem(ListView *lv, bool bNewItem, int index)
{
    LV_ITEMW lvI;
    int i;
    int iColumnNum=0;
    wchar_t oldbuf[1024];
    void *structptr = eaGet(lv->eaElements, index);
    int item_num; // index into windows list view control
    static StuffBuff sbBuff;
    static FILE *fpBuff=NULL;

    if (!sbBuff.buff)
        initStuffBuff(&sbBuff, 128);
    if (!fpBuff) {
        fpBuff = fileOpenStuffBuff(&sbBuff);
    }

    if (!bNewItem) {
        LVFINDINFO lvfi;
        // Find the correct index for this item
        lvfi.flags = LVFI_PARAM;
        lvfi.lParam = index;
        item_num = ListView_FindItem(lv->hDlgListView, -1, &lvfi);
        assert(item_num!=-1);

    } else {
        item_num = ListView_GetItemCount(lv->hDlgListView);
    }

    // Insert item
    lvI.state = 0;
    lvI.stateMask = 0;
    lvI.iItem = item_num;
    lvI.cchTextMax = ARRAY_SIZE(oldbuf)-1;
    i=0;

    // Loop over each column
    for (iColumnNum = 0; iColumnNum < lv->iNumColumns; iColumnNum++) {
		const ParseTable *tpi = NULL;
        void *element;
        wchar_t *buf = NULL;
        
        if (iColumnNum==0 && bNewItem) {
            lvI.mask = LVIF_TEXT | LVIF_STATE | LVIF_PARAM;
            lvI.lParam = index;
        } else {
            lvI.mask = LVIF_TEXT | LVIF_STATE;
        }
        lvI.iSubItem = iColumnNum;

        // Get the old value
        lvI.pszText = oldbuf;
        ListView_GetItemW(lv->hDlgListView, &lvI);

        if (structptr == LISTVIEW_EMPTY_ITEM) {
            element = NULL;
        } else {
            getElement(lv->tpi, structptr, iColumnNum, lv->subItemMap, &tpi, &element);
        }
            

        if (element == NULL) {
            buf = L"";
        } else {
            int len;
            static int zero=0;
            static wchar_t *static_buf;
            static int static_buf_max=0;

            sbBuff.idx = 0;
            InnerWriteTextToken(fpBuff, tpi, 0, element, 0, 0, 0, 0);
            addBinaryDataToStuffBuff(&sbBuff, (char*)&zero, 1); // Null terminate
            len = sbBuff.idx-1;
            dynArrayFit(&static_buf, sizeof(*static_buf), &static_buf_max, len+1);
            buf = static_buf;
            buf[0] = 0;
            UTF8ToWideStrConvert(sbBuff.buff,buf,len);
            
            while (*buf==' ') buf++;

            // chop off an quotes
            if (element && sbBuff.buff[sbBuff.idx-2]=='\"' && *buf=='\"') {
                buf[wcslen(buf)-1] = 0;
                buf = buf+1;
            }
        }
        lvI.pszText = buf;

        // set the item
        if (bNewItem && iColumnNum==0) {
            if(ListView_InsertItemW(lv->hDlgListView, &lvI) == -1) {
                assert(0);
            }
        } else {
            if (wcscmp(oldbuf, lvI.pszText)!=0) {
                if(ListView_SetItemW(lv->hDlgListView, &lvI) == -1) {
                    assert(0);
                }
            }
        }
    }
/* Tooltips (didn't seem to work)
    fpBuff = fileOpenStuffBuff(&sbBuff);
    InnerWriteTextFile(fpBuff, (ParseTable*)lv->tpi, structptr, 0, 0, 0);
    fclose(fpBuff);
    buf = sbBuff.buff;
    while (*buf==' ') buf++;
    {
        WCHAR buf2[16384];
        LVSETINFOTIP lvsit;
        lvsit.cbSize = sizeof(lvsit);
        lvsit.dwFlags = 0;
        lvsit.iItem = item_num;
        lvsit.iSubItem = 0;
        mbstowcs(buf2, buf, ARRAY_SIZE(buf2));
        lvsit.pszText = buf2;
        ListView_SetInfoTip(lv->hDlgListView, &lvsit);
    }
    */

    UpdateWindow(lv->hDlgListView);

}

int listViewAddItem(ListView *lv, void *structptr)
{
    int item_num = lv->iNumItems++;
    int iSelected;

    eaPush(lv->eaElements, structptr);
    eaiPush(&lv->eaForegroundColors, LISTVIEW_DEFAULT_COLOR);
    eaiPush(&lv->eaBackgroundColors, LISTVIEW_DEFAULT_COLOR);

    assert(lv->iNumItems == eaSizeUnsafe(lv->eaElements));

    // Determine old cursor location
    iSelected = ListView_GetSelectionMark(lv->hDlgListView);

    listViewUpdateItem(lv, true, item_num);
    if (iSelected==item_num-1) {
        // We previously had the last item selected
        ListView_SetSelectionMark(lv->hDlgListView, iSelected + 1);
        ListView_EnsureVisible(lv->hDlgListView, iSelected + 1, FALSE);
    }
    return item_num;
}


void listViewSetItemDefaultColor(ListView * lv, void * structptr)
{
    listViewSetItemColor(lv, 
                         structptr, 
                         LISTVIEW_DEFAULT_COLOR, 
                         LISTVIEW_DEFAULT_COLOR);
}

void listViewSetItemColor(ListView * lv, void * structptr, int foreground, int background)
{
    int index = listViewFindItem(lv, structptr);
    bool changed=false;
    if(index != -1)
    {
        assert(index >= 0 && index < eaiSize(&lv->eaForegroundColors));
        assert(index >= 0 && index < eaiSize(&lv->eaBackgroundColors));

        lv->colored = true;

        if (lv->eaForegroundColors[index] != foreground) {
            lv->eaForegroundColors[index] = foreground;
            changed = true;
        }
        if (lv->eaBackgroundColors[index] != background) {
            lv->eaBackgroundColors[index] = background;
            changed = true;
        }
        // Invalidate the control if a whole row's color changes
        if (changed) {
            InvalidateRect(lv->hDlgListView, NULL, FALSE);
        }
    }
}

// Removes an item from the list, returns the pointer to the struct removed (to be freed by caller)
void *listViewDelItem(ListView *lv, int index)
{
    LVFINDINFO lvfi;
    int item_num;
    void *ptr;

    if (index<0) {
        assert(!"Invalid item number being removed from listView");
        return NULL;
    }
    // Find the correct index for this item
    lvfi.flags = LVFI_PARAM;
    lvfi.lParam = index;
    item_num = ListView_FindItem(lv->hDlgListView, -1, &lvfi);
    assert(item_num!=-1);
    if (item_num>=0) {
        ListView_DeleteItem(lv->hDlgListView, item_num);
    }

    // Remove from EArray
    ptr = eaGet(lv->eaElements, index);
    eaSet(lv->eaElements, NULL, index);
    // Prune EArray
    while (eaSizeUnsafe(lv->eaElements)>0 && eaGet(lv->eaElements, eaSizeUnsafe(lv->eaElements)-1)==NULL) {
        eaRemove(lv->eaElements, eaSizeUnsafe(lv->eaElements)-1);
        eaiRemove(&lv->eaBackgroundColors, eaiSize(&lv->eaBackgroundColors)-1);
        eaiRemove(&lv->eaForegroundColors, eaiSize(&lv->eaForegroundColors)-1);
        lv->iNumItems = eaSizeUnsafe(lv->eaElements);
    }
    return ptr;
}

// Removes all items from the list
void listViewDelAllItems(ListView *lv, Destructor destructor)
{
    if (destructor) {
        eaClearEx(lv->eaElements, destructor);
    } else {
        eaDestroy(lv->eaElements);
    }
    eaiDestroy(&lv->eaBackgroundColors);
    eaiDestroy(&lv->eaForegroundColors);
    ListView_DeleteAllItems(lv->hDlgListView);
    lv->iNumItems=0;
}



// Comparator
static int CALLBACK listViewCompare(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
    ListView *lv = (ListView*)lParamSort;
	const ParseTable *tpi;
    void *structptr1, *structptr2;
    int ret;
    int mult = 1;
    int iSubItem = lv->iSubItem;
    if (iSubItem & 0x80000000) {
        iSubItem &= ~0x80000000;
        mult = -1;
    }
    structptr1 = eaGet(lv->eaElements, lParam1);
    structptr2 = eaGet(lv->eaElements, lParam2);
    getElement(lv->tpi, structptr1, iSubItem, lv->subItemMap, &tpi, &structptr1);
    getElement(lv->tpi, structptr2, iSubItem, lv->subItemMap, &tpi, &structptr2);
    ret = TokenCompare(tpi, 0, structptr1, structptr2); // HACK - should be pointing to base table here, but TokenCompare won't actually care
    return ret * mult;
}

void listViewSort(ListView *lv, int column)
{
    lv->iSubItem = column;
    ListView_SortItems(lv->hDlgListView, listViewCompare, lv);
}

void listViewReverseSort(ListView *lv, int column)
{
    lv->iSubItem = column;
    lv->iSubItem |= 0x80000000;
    ListView_SortItems(lv->hDlgListView, listViewCompare, lv);
}

static void doCallback(ListView *lv, ListViewCallbackFunc callback, int index)
{
    if (callback) {
        LVITEM lvi = {0};
        lvi.mask = LVIF_PARAM;
        lvi.iItem = index;
        if (ListView_GetItem(lv->hDlgListView, &lvi)) {
            callback(lv, lv->eaElements_data[lvi.lParam], NULL);
        }
    }
}

COLORREF darkenColor(COLORREF c0)
{
    COLORREF c = (c0==0xff000000)?GetSysColor(COLOR_WINDOW):c0;
    int r = GetRValue(c);
    int g = GetGValue(c);
    int b = GetBValue(c);
    r = MAX(r-16, 0);
    g = MAX(g-16, 0);
    b = MAX(b-6, 0);
    return RGB(r, g, b);
}

COLORREF lightenColor(COLORREF c0)
{
    COLORREF c = (c0==0xff000000)?GetSysColor(COLOR_WINDOWTEXT):c0;
    int r = GetRValue(c);
    int g = GetGValue(c);
    int b = GetBValue(c);
    r = MIN(r+128, 0xff);
    g = MIN(g+128, 0xff);
    b = MIN(b+128, 0xff);
    //b = 0xff;
    return RGB(r, g, b);
}

// Rainbow or gradient
COLORREF filterColor(COLORREF c0, int index)
{
    COLORREF c = (c0==0xff000000)?GetSysColor(COLOR_WINDOW):c0;
    int r = GetRValue(c);
    int g = GetGValue(c);
    int b = GetBValue(c);
    int d;
    d = ABS((index % 6) - 2)*48/3;
    r = MAX(r-d, 0);

    //d = ABS(((index+1) % 6) - 2)*48/3;
    g = MAX(g-d, 0);

    //d = ABS(((index+2) % 6) - 2)*48/3;
    b = MAX(b-d, 0);
    return RGB(r, g, b);
}



LRESULT ProcessCustomDraw (ListView * lv, LPARAM lParam)
{
    LPNMLVCUSTOMDRAW lplvcd = (LPNMLVCUSTOMDRAW)lParam;
    
    switch(lplvcd->nmcd.dwDrawStage) 
    {
    case CDDS_PREPAINT : //Before the paint cycle begins
        //request notifications for individual listview items
        return CDRF_NOTIFYITEMDRAW;

    case CDDS_ITEMPREPAINT: //Before an item is drawn
        {
            int foreground;
            int background;
            int sel = lplvcd->nmcd.lItemlParam;
            int index = lplvcd->nmcd.lItemlParam; //lplvcd->nmcd.dwItemSpec - ListView_GetTopIndex(lv->hDlgListView);

            assert(sel >= 0 && sel <= eaiSize(&lv->eaBackgroundColors));
            assert(eaiSize(&lv->eaForegroundColors) == eaiSize(&lv->eaBackgroundColors));
            
            foreground = lv->eaForegroundColors[sel];
            background = lv->eaBackgroundColors[sel];            

            // if default is specified, don't modify the color value

            if(foreground != LISTVIEW_DEFAULT_COLOR)
                lplvcd->clrText = foreground;
            
            if(background != LISTVIEW_DEFAULT_COLOR)
                lplvcd->clrTextBk = background;

            //if (index %2>=1) {
            //    lplvcd->clrText = lightenColor(lplvcd->clrTextBk);
            //}
            if (index %6>=3 && (background == LISTVIEW_DEFAULT_COLOR || background == LISTVIEW_BAD_VERSION_COLOR)) {
                lplvcd->clrTextBk = darkenColor(lplvcd->clrTextBk);
            }
            //lplvcd->clrTextBk = filterColor(lplvcd->clrTextBk, index);

            return CDRF_NEWFONT;
        }
        break;

    // MJP: THIS CODE MAY BE USEFUL FOR COLORING COLUMNS OR SINGLE CELLS
    ////
    ////    //Before a subitem is drawn
    ////case CDDS_SUBITEM | CDDS_ITEMPREPAINT: 
    ////    if (iSelect == (int)lplvcd->nmcd.dwItemSpec)
    ////    {
    ////        if (0 == lplvcd->iSubItem)
    ////        {
    ////            //customize subitem appearance for column 0
    ////            lplvcd->clrText   = RGB(255,0,0);
    ////            lplvcd->clrTextBk = RGB(255,255,255);

    ////            //To set a custom font:
    ////            //SelectObject(lplvcd->nmcd.hdc, 
    ////            //    <your custom HFONT>);

    ////            return CDRF_NEWFONT;
    ////        }
    ////        else if (1 == lplvcd->iSubItem)
    ////        {
    ////            //customize subitem appearance for columns 1..n
    ////            //Note: setting for column i 
    ////            //carries over to columnn i+1 unless
    ////            //      it is explicitly reset
    ////            lplvcd->clrTextBk = RGB(255,0,0);
    ////            lplvcd->clrTextBk = RGB(255,255,255);

    ////            return CDRF_NEWFONT;
    ////        }
    ////    }
    }
    return CDRF_DODEFAULT;
}

BOOL listViewOnNotify(ListView *lv, WPARAM wParam, LPARAM lParam, ListViewCallbackFunc callback)
{
    static int last_sort_id=-1;
    static ListView *last_sort_listview=NULL;
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
                int index = ListView_GetSelectionMark(lv->hDlgListView);
                switch(lpnmkey->wVKey)
                {
                case VK_UP:
                    {
                        doCallback(lv, callback, max((index - 1), 0));
                        break;
                    }
                case VK_DOWN:
                    {    
                        int count = ListView_GetItemCount(lv->hDlgListView);
                        doCallback(lv, callback, min((index + 1), (count - 1)));
                        break;
                    }
                }
            }
            break;
        }
    case LVN_COLUMNCLICK:
        if (lv->sortable) {
            NMLISTVIEW *pnmv = (LPNMLISTVIEW)lParam;
            lv->iSubItem = pnmv->iSubItem;
            if (lv == last_sort_listview && pnmv->iSubItem == last_sort_id) {
                lv->iSubItem |= 0x80000000;
                last_sort_id = -1;
            } else {
                last_sort_listview = lv;
                last_sort_id = pnmv->iSubItem;
            }
            ListView_SortItems(lv->hDlgListView, listViewCompare, lv);
            break;
        }
    case NM_CLICK:
        {
            LPNMITEMACTIVATE lpnmitem = (LPNMITEMACTIVATE) lParam;
            LVHITTESTINFO lvhti;
            LVITEM lvi;
            if (lpnmitem->iItem==-1)
            {
                // The click won't be handled properly
                //  This is sorta a hack so that we get some functionality when you click on other subitems, but not as good as it could be
                lvhti.flags = LVHT_ONITEM;
                lvhti.pt = lpnmitem->ptAction;
                ListView_SubItemHitTest(lv->hDlgListView, &lvhti);
                if (lvhti.iItem == -1) {
                    // We don't care about the X-coord, since we're assuming this is in Details/Report view
                    lvhti.flags = LVHT_ONITEM;
                    lvhti.pt.x = 2;
                    lvhti.iItem = ListView_HitTest(lv->hDlgListView, &lvhti);
                }
                lpnmitem->iItem = lvhti.iItem;
                if (lpnmitem->iItem!=-1) {
                    lvi.iItem = lpnmitem->iItem;
                    lvi.mask = LVIF_STATE;
                    lvi.stateMask = LVIS_SELECTED;
                    ListView_GetItem(lv->hDlgListView, &lvi);
                    lvi.state = ~lvi.state;
                    ListView_SetItem(lv->hDlgListView, &lvi);
                    if(lvi.state & LVIS_SELECTED)
                    {
                        doCallback(lv, callback, lpnmitem->iItem);
                    }
                }
            }
            else
            {
                if(callback)
                {
                    int index = ListView_GetSelectionMark(lv->hDlgListView);
                    if(index != -1) {
                        doCallback(lv, callback, index);
                    }
                }
            }
        }
        break;
    case NM_CUSTOMDRAW:
        {
            if(lv->colored)
            {
                SetWindowLongPtr(lv->hDlgParent, DWLP_MSGRESULT, (LONG_PTR)ProcessCustomDraw(lv, lParam));
                return TRUE;
            }
        }
    }

    return FALSE;
}








int listViewFindItem(ListView *lv, void *structptr)
{
    return eaFind(lv->eaElements, structptr);
}

void listViewItemChanged(ListView *lv, void *structptr)
{
    int index = listViewFindItem(lv, structptr);
    if (index==-1) {
        assert(!"listViewItemChanged called with an item that is not in the list!");
        return;
    }

    listViewUpdateItem(lv, false, index);
}

void *listViewGetItem(ListView *lv, int index)
{
    return eaGet(lv->eaElements, index);
}

int listViewGetNumItems(ListView *lv)
{
    return ListView_GetItemCount(lv->hDlgListView);
}

int listViewGetLargestItemIndex(ListView *lv)
{
    return eaSizeUnsafe(lv->eaElements);
}

HWND listViewGetParentWindow(ListView *lv)
{
    return lv->hDlgParent;
}

HWND listViewGetListViewWindow(ListView *lv)
{
    return lv->hDlgListView;
}

bool listViewIsSelected(ListView *lv, void *structptr)
{
    LVFINDINFO lvfi;
    LVITEM lvi;
    int item_num;
    int i = listViewFindItem(lv, structptr);
    if (i==-1)
        return false;

    lvfi.flags = LVFI_PARAM;
    lvfi.lParam = i;
    item_num = ListView_FindItem(lv->hDlgListView, -1, &lvfi);
    assert(item_num!=-1);
    if (item_num>=0) {
        lvi.iItem = item_num;
        lvi.mask = LVIF_STATE;
        lvi.stateMask = LVIS_SELECTED;
        ListView_GetItem(lv->hDlgListView, &lvi);
        if(lvi.state & LVIS_SELECTED)
            return true;
        else
            return false;
    }
    return false;
}

void listViewSetSelected(ListView *lv, void *structptr, bool bSelect)
{
    LVFINDINFO lvfi;
    int item_num;
    int i = listViewFindItem(lv, structptr);
    if (i==-1)
        return;

    // Find the correct index for this item
    lvfi.flags = LVFI_PARAM;
    lvfi.lParam = i;
    item_num = ListView_FindItem(lv->hDlgListView, -1, &lvfi);
    assert(item_num!=-1);
    if (item_num>=0) {
        ListView_SetItemState(lv->hDlgListView, item_num, (bSelect ? LVIS_SELECTED : 0), LVIS_SELECTED);
    }
}

void listViewSelectAll(ListView *lv, bool bSelect)
{
    int index=-1;
    while (-1!=(index=ListView_GetNextItem(lv->hDlgListView, index, LVNI_ALL))) {
        ListView_SetItemState(lv->hDlgListView, index, (bSelect ? LVIS_SELECTED : 0), LVIS_SELECTED);
    }
}

void listViewForEach(ListView * lv, ListViewCallbackFunc callback, void * data)
{
    int i, size = eaSizeUnsafe(lv->eaElements);

    for(i=0;i<size;i++)
    {
        callback(lv, lv->eaElements_data[i], data);
    }
}

int listViewDoOnSelected(ListView *lv, ListViewCallbackFunc callback, void *data)
{
    int index=-1;
    LVITEM lvi;
    eaiHandle eai;
    int i;

    eaiCreate(&eai);
    while (-1!=(index=ListView_GetNextItem(lv->hDlgListView, index, LVNI_SELECTED))) {
        lvi.mask = LVIF_PARAM;
        lvi.iItem = index;
        if (!ListView_GetItem(lv->hDlgListView, &lvi)) {
            assert(0);
        }
        eaiPush(&eai, lvi.lParam);
        //printf("  %d (%d)\n", index, lvi.lParam);
    }
    //printf("%d selected items\n", eaiSize(&eai));
    for (i=0; i<eaiSize(&eai); i++) {
        if (callback) {
            callback(lv, lv->eaElements_data[eai[i]], data);
        }
    }
    eaiDestroy(&eai);
    return i;
}

void listViewEnableColor(ListView * lv, bool enable)
{
    lv->colored = false;
}

#endif