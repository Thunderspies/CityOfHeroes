/***************************************************************************
 *     Copyright (c) 2005-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#ifndef UISALVAGE_H
#define UISALVAGE_H

#include "UI/uiDialog.h"
#include "UI/uiInclude.h"
#include "gameComm/wdwbase.h"
#include "UI/uiComboBox.h"
#include "uiGrowBig.h"

typedef struct SalvageInventoryItem SalvageInventoryItem;
typedef struct SalvageItem SalvageItem;
typedef struct uiGrowBig uiGrowBig;
typedef struct ContextMenu ContextMenu;
typedef struct CBox CBox;
typedef struct uiTabControl uiTabControl;
typedef struct ComboCheckboxElement ComboCheckboxElement;

typedef enum SalvageSortType
{
    kSalvageSortType_Name_Ascending,
    kSalvageSortType_Name_Descending,
    kSalvageSortType_Amount_Ascending,
    kSalvageSortType_Amount_Descending,
    kSalvageSortType_Type_Ascending,
    kSalvageSortType_Type_Descending,
    kSalvageSortType_Rarity_Ascending,
    kSalvageSortType_Rarity_Descending,

    // last
    kSalvageSortType_Count
} SalvageSortType;

typedef struct SalvageTabState
{
    SalvageSortType sortby;
    ComboBox sortbyCombo; 
} SalvageTabState; 

typedef struct SalvageWindowState
{
    uiTabControl * tc;
    SalvageTabState **tabStates;
    SalvageInventoryItem **ppItems;
    SalvageInventoryItem **(*GetSalvageInv)();
    uiGrowBig growbig; 

    // used to identify invention inventory
    int    inventionInventoryIdx;

    // used by controls
    ComboCheckboxElement **catElements;
    ComboCheckboxElement **allElements;
} SalvageWindowState;

extern DialogCheckbox deleteUsefulDCB[];
extern char * g_salvageColor[];

void salvageReparse();
int salvageWindow();
void salvage_drag(int index, AtlasTex *icon, float sc);

typedef void (*SalvageDragCb)(SalvageInventoryItem *sal, AtlasTex *icon, float sc);
typedef void (*SalvageClickCb)(SalvageInventoryItem *sal);
const char* salvage_display(SalvageInventoryItem *sal, uiGrowBig *growbig, int wdw, float x, float y, float z, float sc, CBox *resClientArea, SalvageDragCb salvage_drag_cb, SalvageClickCb salvage_click_cb, int hide_name);
void salvageContextMenu(SalvageInventoryItem *sal, int wdw);

int salvagePrintName(SalvageItem const *salvage, float x, float y, float z, float wd, float sc);

void salvageWindowFlash(void);
SalvageInventoryItem **SalvageInventoryItem_GatherMatching(SalvageInventoryItem **salvageInv, char *category );
void UISalvage_DisplayItems(WindowName wdw, F32 x, F32 y, F32 z, F32 wd, F32 ht, F32 sc, ScrollBar *sb, SalvageInventoryItem **items, uiGrowBig *growbig, bool stored_salvage, CBox *resBoxItms, CBox *resBoxBtm, SalvageDragCb salvage_drag_cb);

void uiSalvage_initSortCombo(WindowName wdw, ComboBox *sortbyCombo, bool include_type, bool include_rarity);
SalvageSortType uiSalvage_SortTypeFromCombo(ComboBox *sortbyCombo);
int uiSalvageSort(const void* item1, const void* item2, void const *context);

void uiSalvage_ReceiveSalvageImmediateUseResp( const char* salvageName, U32 flags /*SalvageImmediateUseStatus*/, const char* msgId );

void salvage_confirmOpen(const SalvageItem *salvage);

#endif //UISALVAGE_H
