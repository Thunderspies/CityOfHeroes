#include "uiAuctionHouseIncludeInternal.h"
#include "strings_opt.h"
#include "bitfield.h"
#include "StringCache.h"
#include "UIEdit.h"

// This is how many auction items we can hold in the memory pool at once.
// It will therefore grow by this many at once, too.
#define AUCTION_ITEM_POOL_CHUNK_SIZE		20000

AuctionUI g_auc = {NULL,0,0,-1,-1,0,0};

UIEdit *nameEdit;
//UIEdit *searchResultEdit[RESULT_ITEMS_PER_PAGE];
UIEdit *searchResultEdit;
UIEdit *displayItemNameEdit;
uiTreeNode *searchTreeRoot = NULL;
AuctionSearchFilter searchFilter = DEFAULT_SEARCH_FILTER;
//AuctionItem **g_auc.searchResults = NULL;
AuctionItem **localInventory = NULL;
int currentInvItem = -1;

AuctionHistoryItem *inventoryMenuHistory = NULL;
char *curSearchMenuItemName = NULL;
char *curInventoryMenuItemName = NULL;
//uiTabControl *auctionTabControl;
ComboBox searchByCombo;
ComboBox minLevelCombo;
ComboBox maxLevelCombo;
ComboBox forSaleComboBox;

int dividerOffset = DEFAULT_DIVIDER_OFFSET_EXPANDED;

int isOnBuyScreen = 1;
int isOnSellScreen = 0;
int isOnTransactionScreen = 0;
int viewingPriceHistory = 0;
int displayingExtraInfo = 0;

//////////////////////////////////////////////////////////////////////
// auction item memory handlers
static AuctionItem **allocatedSearchResults = NULL;

MP_DEFINE(AuctionItem);
AuctionItem *allocAuctionItem()
{
//	AuctionItem *ret;
//
//	MP_CREATE(AuctionItem, AUCTION_ITEM_POOL_CHUNK_SIZE);
//
//	if ( !eaSize(&allocatedSearchResults) )
//		ret = MP_ALLOC(AuctionItem);
//	else
//	{
//		char *oldSmfStr = NULL;
//		ret = eaPop(&allocatedSearchResults);
//		oldSmfStr = ret->smfStr;
//		ZeroStruct(ret);
//		ret->smfStr = oldSmfStr;
//		estrClear(&ret->smfStr);
//	}
//
//#ifndef TEST_CLIENT
//	ret->smf = smfBlock_Create();
//#endif
//	ret->cached_matchesSearchFilter = 1;
//	ret->numForSale = -1;
//	ret->numForBuy = -1;
	return 0;
}

void freeAuctionItem(AuctionItem *res)
{
//#ifndef TEST_CLIENT
//	smfBlock_Destroy(res->smf);
//	res->smf = NULL;
//#endif
//	eaPush(&allocatedSearchResults, res);
}

void freeAuctionItem_noCache(AuctionItem *res)
{
//#ifndef TEST_CLIENT
//	smfBlock_Destroy(res->smf);
//#endif
//	MP_FREE(AuctionItem, res);
}

void freeAuctionItemCache()
{
	int i = 0;
	for ( i = 0; i < eaSize(&allocatedSearchResults); ++i )
	{
		freeAuctionItem_noCache(allocatedSearchResults[i]);
	}
	eaSetSize(&allocatedSearchResults, 0);
}

void copyAuctionItem(AuctionItem *src, AuctionItem *dest)
{
//	if (!src || !dest)
//	{
//		return;
//	}
//
//#ifndef TEST_CLIENT
//	if (dest->smf)
//	{
//		smfBlock_Destroy(dest->smf);
//	}
//#endif
//
//	memcpy(dest, src, sizeof(AuctionItem));
//
//#ifndef TEST_CLIENT
//	if (src->smf)
//	{
//		dest->smf = smfBlock_CreateCopy(src->smf);
//	}
//#endif
}
//
//////////////////////////////////////////////////////////////////////

int *NumSelling = NULL;
int *NumBuying = NULL;

void clearItemStatusCache(void)
{
	if ( NumSelling )
	{
		ea32Clear(&NumSelling);
		ea32Clear(&NumBuying);
	}
	else
	{
		ea32SetSize(&NumSelling, eaSize(&g_AuctionItemDict.ppItems));
		ea32SetSize(&NumBuying, eaSize(&g_AuctionItemDict.ppItems));
	}
}

void updateLocalItemStatus( int id, int buying, int selling )
{
	PERFINFO_AUTO_START("updateLocalItemStatus", 1);
	if ( !NumSelling )
		clearItemStatusCache();

	if ( !(id >= ea32Size(&NumSelling)) )
    {
    	NumSelling[id] = selling;
	    NumBuying[id] = buying;
    }

	PERFINFO_AUTO_STOP();
}

void makeLookupName( AuctionItem *item )
{
//	char newName[512] = {0};
//	char lvlStr[3] = {0};
//	STR_COMBINE_BEGIN(newName);
//	STR_COMBINE_CAT(item->name);
//	STR_COMBINE_CAT_C('.');
//	STR_COMBINE_CAT(itoa(item->lvl, lvlStr, 10));
//	STR_COMBINE_END();
//	strcpy_s(SAFESTR(item->lookupName), newName);
}

int getNumSellingById( int id )
{
	//if ( NumSelling )
	//{
	//	if ( g_AuctionItemDict.items[id]->buyPrice )
	//		return 1;
	//	return NumSelling[id];
	//}
	return -1;
}

int getNumSelling( AuctionItem *item )
{
	//if (item)
	//	return getNumSellingById(item->id);
	//else 
		return -1;
}

int getNumBuyingById( int id )
{
	if ( NumBuying )
		return NumBuying[id];
	return -1;
}

int getNumBuying( AuctionItem *item )
{
	if ( item )
		return getNumBuyingById(item->id);
	return -1;
}

void newLocalInvItem(TrayItemType type, char *name, int level, int id, int status, int amtStored, int infStored, int amtOther, int infPrice)
{
	//AuctionItem *newInvItem = allocAuctionItem();
	//newInvItem->type = type;
	//newInvItem->name = strdup(name);
	//newInvItem->lvl = level;
	//newInvItem->auction_id = id;
	//newInvItem->auction_status = status;
	//newInvItem->amtStored = amtStored;
	//newInvItem->infStored = infStored;
	//newInvItem->amtOther = amtOther;
	//newInvItem->infPrice = infPrice;

	//switch ( newInvItem->type )
	//{
	//case kTrayItemType_Salvage:
	//	if ( !stashFindPointer(g_SalvageDict.haItemNames, newInvItem->name, &newInvItem->salvage) )
	//		assert("Failed to get salvage from dictionary"==0);
	//	xcase kTrayItemType_Recipe:
	//	if ( !stashFindPointer(g_DetailRecipeDict.haItemNames, newInvItem->name, &newInvItem->recipe) )
	//		assert("Failed to get recipe from dictionary"==0);
	//	xcase kTrayItemType_Inspiration:
	//	newInvItem->inspiration = basepower_FromPath(newInvItem->name);
	//	if ( !newInvItem->inspiration )
	//		assert("Failed to get inspiration from dictionary"==0);
	//	xcase kTrayItemType_SpecializationInventory:
	//	newInvItem->enhancement = basepower_FromPath(newInvItem->name);
	//	if ( !newInvItem->enhancement )
	//		assert("Failed to get enhancement from dictionary"==0);
	//	break;
	//}
	//eaPush(&localInventory, newInvItem);
}

void updateAuctionInventory(Entity *e)
{
//	int i;
//	int lastSize = 0;
//	static int timer = 0;
//
// 	if ( !timer )
//	{
//		timer = timerAlloc();
//		timerStart(timer);
//	}
//
//	if ( timerElapsed(timer) >= 30.0f )
//	{
//		auction_updateInventory();
//		timerStart(timer);
//	}
//	
//	if ( e->pchar->auctionInv && !e->pchar->auctionInvUpdated )
//		return;
//
//	PERFINFO_AUTO_START("updateAuctionInventory", 1);
//
//	if ( !e->pchar->auctionInv )
//	{
//		if ( !localInventory )
//			eaSetSize(&localInventory, 0);
//		PERFINFO_AUTO_STOP();
//		return;
//	}
//
//	lastSize = eaSize(&localInventory);
//	for ( i = 0; i < lastSize; ++i )
//	{
//		free(localInventory[i]->name);
//		freeAuctionItem(localInventory[i]);
//	}
//	eaSetSize(&localInventory, 0);
//
//	for ( i = 0; i < eaSize(&e->pchar->auctionInv->items); ++i )
//	{
//		TrayItemIdentifier *tii = TrayItemIdentifier_Struct(e->pchar->auctionInv->items[i]->pchIdentifier);
//		if ( tii )
//		{
//			newLocalInvItem(tii->type, tii->name, tii->level, e->pchar->auctionInv->items[i]->id, 
//				e->pchar->auctionInv->items[i]->auction_status, e->pchar->auctionInv->items[i]->amtStored,
//				e->pchar->auctionInv->items[i]->infStored, e->pchar->auctionInv->items[i]->amtOther,
//				e->pchar->auctionInv->items[i]->infPrice);
//		}
//	}
//
//	if ( eaSize(&localInventory) > lastSize )
//	{
//		currentInvItem = eaSize(&localInventory) - 1;
//#ifndef TEST_CLIENT
//		if ( sellItemEdit )
//			uiEditTakeFocus(sellItemEdit);
//#endif
//	}
//	PERFINFO_AUTO_STOP();
}

static void auctionHouseAddToLocalInventory(TrayItemType type, char *name, int level, int index, int amt, int price, int status, int id)
{
	// combine stacks
	//if ( type == kTrayItemType_Salvage || type == kTrayItemType_Recipe )
	//{
	//	int i, itemId;
	//	TrayItemIdentifier tii;
	//	tii.name = name;
	//	tii.level = level;
	//	tii.type = type;
	//	tii.buyPrice = 0;
	//	if ( stashFindInt(g_AuctionItemDict.itemToIDHash, TrayItemIdentifier_Str(&tii), &itemId) )
	//	{
	//		if ( g_AuctionItemDict.items[itemId]->buyPrice > 0 )
	//			return;
	//		for ( i = 0; i < eaSize(&localInventory); ++i )
	//		{
	//			if ( localInventory[i]->type == type &&
	//				localInventory[i]->id == itemId && localInventory[i]->infPrice == price && 
	//				localInventory[i]->lvl == level &&
	//				localInventory[i]->amtOther + localInventory[i]->amtStored + amt <= MAX_AUCTION_STACK_SIZE
	//				)
	//			{
	//				if ( status == AuctionInvItemStatus_Bidding )
	//				{
	//					localInventory[i]->amtOther += amt;
	//					currentInvItem = i;
	//					return;
	//				}
	//				else if ( status != AuctionInvItemStatus_Bidding )
	//				{
	//					localInventory[i]->amtStored += amt;
	//					currentInvItem = i;
	//					return;
	//				}
	//			}
	//		}
	//	}
	//}
	//else if ( EAINRANGE(id,g_AuctionItemDict.items) && g_AuctionItemDict.items[id]->buyPrice > 0 )
	//	return;
	//if ( status == AuctionInvItemStatus_Bidding )
	//	newLocalInvItem(type, name, level, id, status, 0, 0, amt, price);
	//else
	//	newLocalInvItem(type, name, level, id, status, amt, 0, 0, price);
	//currentInvItem = eaSize(&localInventory) - 1;
}

void auctionHouseRemoveFromLocalInventory(int localInvIdx)
{
	eaRemove(&localInventory, localInvIdx);

	if(currentInvItem > eaSize(&localInventory) - 1)
		currentInvItem = eaSize(&localInventory) - 1;
}

void auctionHouseAddItem(TrayItemType type, char *name, int level, int index, int amt, int price, int status, int id)
{
	//auction_addItem(type, name, level, index, amt, price, status, id);
	//auctionHouseAddToLocalInventory(type, name, level, index, amt, price, status, id);
}

void auctionHouseRemoveItem(int localInvIdx)
{
	auction_placeItemInInventory(localInventory[localInvIdx]->auction_id);
	auctionHouseRemoveFromLocalInventory(localInvIdx);
}

void auctionHouseGetInfStored(int localInvIdx)
{
	auction_getInfStored(localInventory[localInvIdx]->auction_id);
	localInventory[localInvIdx]->infStored = 0;
	localInventory[localInvIdx]->amtOther = 0;
	if ( !localInventory[localInvIdx]->amtStored )
		auctionHouseRemoveFromLocalInventory(localInvIdx);
}

void auctionHouseGetAmtStored(int localInvIdx)
{
	auction_getAmtStored(localInventory[localInvIdx]->auction_id);
	localInventory[localInvIdx]->amtStored = 0;
	if ( !localInventory[localInvIdx]->infStored )
		auctionHouseRemoveFromLocalInventory(localInvIdx);
}

void auctionItemMakeName(AuctionItem *item)
{
	//if ( !item || item->name )
	//	return;

	//PERFINFO_AUTO_START("auctionItemMakeName", 1);

	//switch ( item->type )
	//{
	//	case kTrayItemType_Inspiration:
	//		item->origItem->name = item->inspiration->pchFullName;//basepower_ToPath(item->inspiration);
	//		item->name = item->origItem->name;
	//		break;
	//	case kTrayItemType_SpecializationInventory:
	//		item->origItem->name = item->enhancement->pchFullName;//basepower_ToPath(item->enhancement);
	//		item->name = item->origItem->name;
	//		break;
	//	default:
	//		assert("AuctionItem has no name"==0);
	//}
	//PERFINFO_AUTO_STOP();
}

static char **infoRequests = NULL;

int auctionItemGetInfoRequestId(AuctionItem *item)
{
	//int i;

	//if ( !item->lookupName[0] )
	//	makeLookupName(item);

	//for ( i = 0; i < eaSize(&infoRequests); ++i )
	//{
	//	if ( !infoRequests[i] )
	//	{
	//		estrCopy2(&infoRequests[i], item->lookupName);
	//		return i;
	//	}
	//}
	//i = eaPush(&infoRequests, NULL);
	//estrCopy2(&infoRequests[i], item->lookupName);
	return 0;
}


void uiAuctionHandleBannedUpdate(Packet *pak)
{
	g_auc.uiDisabled = pktGetBits(pak, 1);
}

static void uiAuctionUpdateBannedStatus(void)
{
	static bannedTimer = 0;

	// if i am currently banned from performing auction actions, bug the 
	// map server every second until it tells us we arent banned anymore
	if ( g_auc.uiDisabled )
	{
		if ( !bannedTimer )
			bannedTimer = timerAlloc();

		if ( timerElapsed(bannedTimer) > 1.0f )
		{
			auction_updateAuctionBannedStatus();
			timerStart(bannedTimer);
		}
	}
}

#ifndef TEST_CLIENT
AtlasTex *drawAuctionItemIconEx(AuctionItem *item, float x, float y, float z, float sc, float textsc, int rgba, float tooltipOffsetY)
{
	AtlasTex *icon = NULL;
	static uiEnhancement *toolTipEnhancement = NULL;
	uiEnhancement *enhancement = NULL;

	PERFINFO_AUTO_START("drawAuctionItemIcon", 1);
	switch ( item->type )
	{
		case kTrayItemType_Salvage:
			icon = atlasLoadTexture(item->salvage->ui.pchIcon);
			display_sprite(icon, x, y, z, sc, sc, rgba);

		xcase kTrayItemType_Recipe:
			icon = uiRecipeDrawIconEx(item->recipe, item->recipe->level, x-16.f*sc,  y-12.f*sc,  z, sc*1.31f, WDW_AUCTIONHOUSE, true, rgba, tooltipOffsetY, false);

		xcase kTrayItemType_Inspiration:
			icon = atlasLoadTexture(item->inspiration->pchIconName);
			// the icons for inspirations are smaller, so i double the scale for them.
			//display_sprite(icon, x + (icon->width/2)*sc, y + (icon->height/2)*sc, z, 2.0f*sc, 2.0f*sc, rgba);
			display_sprite(icon, x+8*sc, y+8*sc, z, 1.5f*sc, 1.5f*sc, rgba);

		xcase kTrayItemType_SpecializationInventory:
			enhancement = uiEnhancementCreate(item->enhancement->pchFullName, item->lvl);
			uiEnhancementSetColor(enhancement, rgba);
			icon = uiEnhancementDrawCentered(enhancement, x, y, z, sc, textsc, MENU_GAME, WDW_AUCTIONHOUSE, 1, tooltipOffsetY);
			if (enhancement->isDisplayingToolTip)
			{
				uiEnhancementFree(&toolTipEnhancement);
				toolTipEnhancement = enhancement;
			}
			else
			{
				uiEnhancementFree(&enhancement); 
			}

		xcase kTrayItemType_Power:
			icon = atlasLoadTexture(item->temp_power->pchIconName);
			display_sprite(icon, x+8*sc, y+8*sc, z, 1.5*sc, 1.5*sc, rgba);
	}
	PERFINFO_AUTO_STOP();

	return icon;
}

AtlasTex *drawAuctionItemIcon(AuctionItem *item, float x, float y, float z, float sc, float textsc, float tooltipOffsetY)
{
	return drawAuctionItemIconEx(item, x, y, z, sc, textsc, CLR_WHITE, tooltipOffsetY);
}
#endif



#ifndef TEST_CLIENT
//void uiAuctionHouseItemInfoReceive(Packet *pak)
//{
//	char *pchIdentifier = pktGetString(pak);
//	int selling = pktGetBitsAuto(pak);
//	int buying = pktGetBitsAuto(pak);
//	int id;
//	// ...
//	if ( stashFindInt(g_AuctionItemDict.itemToIDHash, pchIdentifier, &id) )
//		updateLocalItemStatus(id, buying, selling);
//}

int auctionHouseWindow(void)
{
	Entity *e = playerPtr();
	AtlasTex * layover = NULL;
	float x, y, z, wd, ht, sc;
	int clr, bkclr;
	static int init = 0;
	static float divider = 1.0f;
	AtlasTex * down  = atlasLoadTexture( "Chat_separator_arrow_down.tga" );
	AtlasTex * up    = atlasLoadTexture( "Chat_separator_arrow_up.tga" );
	static int lastPlayerType = -1;
	static int openLastFrame = 0;

	PERFINFO_AUTO_START("auctionHouseWindow", 1);

	if (!init)
	{
		g_auc.requestTimer = timerAlloc();
		lastPlayerType = e->pl->playerType;
		init = 1;
	}

	// debug mode send a few transactions per frame
	aucscript_SellAndBuy25(false);
	aucscript_SellOrBuy50(false,false);

	// display debugging coords
	window_getDims( WDW_AUCTIONHOUSE, &x, &y, &z, &wd, &ht, &sc, &clr, &bkclr );

	if ( window_getMode(WDW_AUCTIONHOUSE) == WINDOW_DISPLAYING )
	{
		AuctionWindowFunc func = NULL;

		if ( lastPlayerType != e->pl->playerType )
		{
			clearItemStatusCache();
			lastPlayerType = e->pl->playerType;
		}

		if ( !openLastFrame )
			auction_updateInventory();

		uiAuctionUpdateBannedStatus();

		drawFrame(PIX3, R10, x, y, AUCTION_Z, wd, ht, sc, clr, bkclr);

		drawHorizontalLine(x, y+dividerOffset*sc, wd, AUCTION_Z, 1.0f, clr);

		//if ( isOnBuyScreen )
		{
			UIBox buyMenuClip;
			buyMenuClip.x = x;
			buyMenuClip.y = y+3*sc;
			buyMenuClip.width = wd+6*sc;
			buyMenuClip.height = (dividerOffset-4)*sc;
			clipperPush(&buyMenuClip);
			drawVerticalLine(x+(FRAME1_X+FRAME_W)*sc, y, ht-3*sc, AUCTION_Z, 1.0f, clr);
			auctionHouseBuyMenu(e, x, y, z, wd, ht, sc, clr, bkclr);
			clipperPop();
		}
		//else if ( isOnTransactionScreen )
		{
			UIBox inventoryClip;
			inventoryClip.x = x+3*sc;
			inventoryClip.y = y+dividerOffset*sc;
			inventoryClip.width = wd-6*sc;
			inventoryClip.height = ht-(dividerOffset-5)*sc;
			clipperPush(&inventoryClip);
			auctionHouseInventoryMenu(e, x, y, z, wd, ht, sc, clr, bkclr);
			clipperPop();
		}
		openLastFrame = 1;
	}
	else if( openLastFrame )	// make sure none of our edit boxes have focus
	{
		if (searchResultEdit && uiEditHasFocus(searchResultEdit))
			uiEditReturnFocus(searchResultEdit);
		if (nameEdit && uiEditHasFocus(nameEdit))
			uiEditReturnFocus(nameEdit);
		if (sellItemEdit && uiEditHasFocus(sellItemEdit))
			uiEditReturnFocus(sellItemEdit);
		if (g_auc.searchResults && (eaSize(&g_auc.searchResults) || eaSize(&allocatedSearchResults)))
		{
			freeAllSearchResults(0);
			freeAuctionItemCache();
			// TODO: do we want to actually free the tree when its not in use?
			//freeBuyMenuSearchTree();
		}
		openLastFrame = 0;
	}

	PERFINFO_AUTO_STOP();
	return 0;
}
#endif // ifndef TEST_CLIENT

