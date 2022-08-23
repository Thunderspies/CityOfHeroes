#ifndef UUIAUCTIONHOUSE_INCLUDE_INTERNAL_H_
#define UUIAUCTIONHOUSE_INCLUDE_INTERNAL_H_

#include "entity/powers.h"

#include "uiAuctionHouse.h"
#include "auction/Auction.h"
#include <utilitieslib/utils/utils.h>
#include "uiGame.h"
#include "uiUtil.h"
#include "uiChat.h"
#include "uiInput.h"
#include "uiTray.h"
#include "uiCursor.h"
#include "uiEditText.h"
#include "uiUtilMenu.h"
#include "uiUtilGame.h"
#include "uiScrollBar.h"
#include "uiWindows.h"
#include "UIEdit.h"
#include "uiTree.h"
#include "uiEnhancement.h"
#include "uiComboBox.h"
#include "uiTabControl.h"
#include "uiRecipeInventory.h"
#include "uiToolTip.h"
#include "auction/AuctionData.h"
#include "entity/entPlayer.h"

#include "sprite/sprite_base.h"
#include "sprite/sprite_text.h"
#include "sprite/sprite_font.h"
#include "../graphics/textureatlas.h"

#include "../graphics/font.h"   // for xyprintf
#include "../graphics/ttFontUtil.h"
#include "language/langClientUtil.h"
#include <utilitieslib/language/MessageStoreUtil.h>

#include "win/win_init.h"   // for windowClientSize
#include "cmdparse/cmdgame.h"

#include "entity/character_base.h"
#include "../sound/sound.h"
#include "entity/origins.h"
#include "entity/powers.h"
#include "entity/boostset.h"
#include <utilitieslib/components/earray.h>
#include "player/player.h"
#include "entity/entity.h" 
#include <utilitieslib/components/EString.h>
#include "uiClipper.h"
#include "entity/salvage.h" 
#include "bases/DetailRecipe.h"
#include "uiNet.h"
#include "../win/input.h"

#ifndef TEST_CLIENT
#include "formatter/smf_main.h"
#endif

// frame positions
#define FRAME_W 375
#define FRAME_H 390
#define FRAME1_X 0
//#define FRAME1_Y 70
#define FRAME1_Y 0
#define FRAME2_X (FRAME1_X + FRAME_W)
#define FRAME2_Y FRAME1_Y

#define TOOLTIPOFFSETY	-20.0f

#define AUCTION_Z z

#define DEFAULT_DIVIDER_OFFSET_EXPANDED 395
#define DEFAULT_DIVIDER_OFFSET_CONTRACTED 180

// button positions
#define TOP_BUTTON_Y 35
#define TOP_BUTTON_W 260
#define TOP_BUTTON_H 45
#define BUY_BUTTON_X 140
#define TRANSACTION_BUTTON_X (BUY_BUTTON_X + TOP_BUTTON_W)
#define SELL_BUTTON_X (TRANSACTION_BUTTON_X + TOP_BUTTON_W)

// edit box positions
#define STD_EDIT_H 24

#define SEARCHBOX_EDIT_H STD_EDIT_H
#define SEARCHBOX_NAME_X (FRAME1_X + 10)
#define SEARCHBOX_NAME_Y (FRAME1_Y + 10)
#define SEARCHBOX_NAME_W 220
#define MINLVL_X (FRAME1_X + 85)
#define MINLVL_Y (FRAME1_Y + 15 + SEARCHBOX_EDIT_H)
#define MINLVL_W 50
#define MAXLVL_X (MINLVL_X + MINLVL_W + 20)
#define MAXLVL_Y (MINLVL_Y)
#define MAXLVL_W MINLVL_W
#define SEARCHBYTYPE_X (SEARCHBOX_NAME_X + SEARCHBOX_NAME_W + 20)
#define SEARCHBYTYPE_Y SEARCHBOX_NAME_Y
#define SEARCHBYTYPE_W 120
#define SEARCHBYTYPE_H STD_EDIT_H
#define SEARCHBUTTON_X (FRAME1_X + 120)
#define SEARCHBUTTON_Y (MINLVL_Y + STD_EDIT_H + 5)
#define FORSALE_X 220
#define	FORSALE_Y (MINLVL_Y)
#define FORSALE_W 140

// search tree posiiton
#define SEARCHTREE_TOP (FRAME1_Y + 100)
#define SEARCHTREE_NODE_HT 20

// displayed item positions
#define DISPLAYITEM_H 45
#define DISPLAYITEMSELECTED_H 120
#define DISPLAYITEM_X (FRAME2_X + 15)
#define DISPLAYITEM_Y (15)
#define DISPLAYITEM_INFO_X (DISPLAYITEM_X + 65)
#define DISPLAYITEM_INFO_Y (220)
#define DISPLAYITEM_EDIT_Y (40)
#define DISPLAYITEM_EDIT_X (DISPLAYITEM_INFO_X /*+ 120*/)
#define DISPLAYITEM_EDIT_W 115
#define DISPLAYITEM_SLIDER_EDIT_W (35)
#define DISPLAYITEM_SLIDER_W (DISPLAYITEM_EDIT_W-DISPLAYITEM_SLIDER_EDIT_W)
#define DISPLAYITEM_EDIT_H STD_EDIT_H
#define DISPLAYITEM_HISTORY_X (DISPLAYITEM_EDIT_X + DISPLAYITEM_EDIT_W+1)
#define DISPLAYITEM_HISTORY_Y (DISPLAYITEM_EDIT_Y)
#define DISPLAYITEM_HISTORY_W 166
#define DISPLAYITEM_HISTORY_H 65

#define INVENTORY_X (5)
#define INVENTORY_W (100)
#define INVENTORY_H (75)
#define INVENTORYITEM_X (INVENTORY_X + 15)
#define INVENTORYITEM_Y (30)
#define INVENTORYINFO_X (INVENTORYITEM_X)
#define INVENTORYINFO_Y (INVENTORYITEM_Y + 45)
#define INVENTORYNAME_Y (20)
#define INVENTORYBUTTONS_Y (425)

#define DETAILITEM_ICON_X (FRAME2_X + (FRAME_W/2))
#define DETAILITEM_ICON_Y (FRAME2_Y + 50)
#define DETAILITEM_SMFBLOCK_W (FRAME_W - 10)
#define DETAILITEM_SMFBLOCK_X (INVENTORY_X)
#define DETAILITEM_SMFBLOCK_H (180)
#define DETAILITEM_SELLBOX_X (90)
#define DETAILITEM_SELLBOX_Y (FRAME2_Y + 250)
#define DETAILITEM_SELLBOX_H STD_EDIT_H
#define DETAILITEM_SELLBOX_W 115
#define DETAILITEM_POSTBUTTON_W 70
#define DETAILITEM_POSTBUTTON_OFFSET 430
#define DETAILITEM_GETBUTTON_X (300)
#define DETAILITEM_GETBUTTON_W 70
#define DETAILITEM_GETBUTTON_Y 418
#define DETAILITEM_GETBUTTON_H 20
#define DETAILITEM_MOREBUTTON_X 675
#define DETAILITEM_MOREBUTTON_Y (INVENTORYNAME_Y-15)
#define DETAILITEM_MOREBUTTON_W 70
#define DETAILITEM_HISTORY_X (FRAME2_X + 30)
#define DETAILITEM_HISTORY_Y (DISPLAYITEM_INFO_Y+5)
#define DETAILITEM_HISTORY_W (DETAILITEM_SMFBLOCK_W-30)
#define DETAILITEM_HISTORY_H (DETAILITEM_SMFBLOCK_H-20)


#define RESULT_ITEMS_PER_PAGE 8

#define SEARCHFILTER_MAXLEVEL 53



#define INIT_EDIT_BOX(edit, size)			\
	edit = uiEditCreate();					\
	edit->textColor = CLR_WHITE;			\
	uiEditSetFont(edit, &game_12);			\
	edit->z = AUCTION_Z;					\
	edit->limitType = UIECL_UTF8_ESCAPED;	\
	edit->limitCount = size;				\
	edit->disallowReturn = 1;


//typedef enum
//{
//	BUYMENU_NAME,
//	BUYMENU_MIN_LVL,
//	BUYMENU_MAX_LVL,
//	//BUYMENU_LVL_RANGE_MIN,
//	//BUYMENU_LVL_RANGE_MAX,
//	BUYMENU_TOTAL_BLOCKS,
//} BuyMenuFields;


typedef struct AuctionSearchFilter
{
	char name[32];
	int minlvl;
	int maxlvl;
	int type;
	int forSale;
	int forBuy;
	int dirty;
	int totalProcessed;
	int processedThisFrame;
	int bitmask;

} AuctionSearchFilter;


typedef void (*AuctionWindowFunc)(Entity *e, float x, float y, float z, float wd, float ht, float sc, int clr, int bkclr);

typedef struct AuctionUI
{	
	AuctionItem **searchResults;
	int requestingItemStatus;
	int curStatusRequestIndex;
	int lowestID;
	int highestID;
	int requestTimer;
	int uiDisabled;
} AuctionUI;


#define SEARCHRESULTS_MAX_TO_PROCESS_PER_FRAME 1000

extern AuctionUI g_auc;

//extern UIEdit *buyMenuEdit[BUYMENU_TOTAL_BLOCKS];
extern UIEdit *nameEdit;
//extern UIEdit *searchResultEdit[RESULT_ITEMS_PER_PAGE];
extern UIEdit *searchResultEdit;
extern UIEdit *displayItemNameEdit;
extern UIEdit *sellItemEdit;
extern uiTreeNode *searchTreeRoot;
extern AuctionSearchFilter searchFilter;
extern int curSelection;
extern AuctionItem **localInventory;
extern int currentInvItem;
extern AuctionHistoryItem *searchMenuHistory;
extern AuctionHistoryItem *inventoryMenuHistory;
extern char *curSearchMenuItemName;
extern char *curInventoryMenuItemName;
extern ComboBox searchByCombo;
extern ComboBox minLevelCombo;
extern ComboBox maxLevelCombo;
extern ComboBox forSaleComboBox;
extern int dividerOffset;

extern int isOnBuyScreen;
extern int isOnSellScreen;
extern int isOnTransactionScreen;
extern int viewingPriceHistory;
extern int displayingExtraInfo;

AuctionItem *allocAuctionItem();
void freeAuctionItem(AuctionItem *res);
void freeAuctionItem_noCache(AuctionItem *res);
void copyAuctionItem(AuctionItem *src, AuctionItem *dest);
void freeAllSearchResults(int cache);
void freeBuyMenuSearchTree();

int getNumSellingById( int id );
int getNumSelling( AuctionItem *item );
int getNumBuyingById( int id );
int getNumBuying( AuctionItem *item );
void auctionHouseAddItem(TrayItemType type, char *name, int level, int index, int amt, int price, int status, int id);
void auctionHouseRemoveItem(int localInvIdx);
void auctionHouseGetAmtStored(int localInvIdx);
void auctionHouseGetInfStored(int localInvIdx);

AtlasTex *drawAuctionItemIconEx(AuctionItem *item, float x, float y, float z, float xsc, float textsc, int rgba, float tooltipOffsetY);
AtlasTex *drawAuctionItemIcon(AuctionItem *item, float x, float y, float z, float xsc, float textsc, float tooltipOffsetY);
void updateAuctionInventory(Entity *e);
void auctionHouseBuyMenu(Entity *e, float x, float y, float z, float wd, float ht, float sc, int clr, int bkclr);
void auctionHouseSellMenu(Entity *e, float x, float y, float z, float wd, float ht, float sc);
void auctionHouseInventoryMenu(Entity *e, float x, float y, float z, float wd, float ht, float sc, int clr, int bkclr);
void auctionHouseResetSearch(bool all);

void AuctionDoContextMenu(AuctionItem *pItem );

#define DEFAULT_SEARCH_FILTER { {0}, 0, SEARCHFILTER_MAXLEVEL, kTrayItemType_None, 1, 1, 1, 0, 0, 0 }

#endif
