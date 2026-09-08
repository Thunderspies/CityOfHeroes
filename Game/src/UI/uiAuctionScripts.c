#include <utilitieslib/components/earray.h>
#include <utilitieslib/utils/mathutil.h>
#include "clientcomm/clientcomm.h"
#include "uiAuctionHouseIncludeInternal.h"

#include "entity/salvage.h"
#include "bases/DetailRecipe.h"

// goals:
// 1. sell a ton of stuff
// 2. buy it back
// 3. check that everything worked out.
// deadlock?
// multiple running, someone buys my thing but doesn't sell it.
// - make sure you always sell what you try to get.

#define AUCSCRIPT_INVSIZE        50    // some of the scripts will overflow this in their zeal, don't make it too big
#define AUCSCRIPT_TIMEOUTSECS    5
#define AUCSCRIPT_BUYVALUE        100
#define AUCSCRIPT_VALUEMIN        100
#define AUCSCRIPT_VALUEMAX        1000000

static AuctionItem **ppLocalInventory;

void addItemToLocalInventory(char * pchIdent, int id, int status, int amtStored, int infStored, int amtOther, int infPrice )
{
    AuctionItem *newInvItem = calloc(1, sizeof(AuctionItem));
    AuctionItem *baseItem;
    int idx;

    if( !stashFindPointer( g_AuctionItemDict.stAuctionItems, pchIdent, &baseItem ) )
        return;

    newInvItem->pchIdentifier = pchIdent;
    newInvItem->type = baseItem->type;
    newInvItem->lvl = baseItem->lvl;
    newInvItem->pItem = baseItem->pItem;

    newInvItem->auction_id = id;
    newInvItem->auction_status = status;
    newInvItem->amtStored = amtStored;
    newInvItem->infStored = infStored;
    newInvItem->amtOther = amtOther;
    newInvItem->infPrice = infPrice;

    switch ( newInvItem->type )
    {
        xcase kTrayItemType_Salvage:     newInvItem->displayName = newInvItem->salvage->ui.pchDisplayName;
        xcase kTrayItemType_Recipe:         newInvItem->displayName = newInvItem->recipe->ui.pchDisplayName;
        xcase kTrayItemType_Inspiration: newInvItem->displayName = newInvItem->inspiration->pchDisplayName;
        xcase kTrayItemType_SpecializationInventory: newInvItem->displayName = newInvItem->inspiration->pchDisplayName;
    }

    idx = eaPush(&ppLocalInventory, newInvItem);
}

static void auctionUpdateInventory()
{
    int i;
    int lastSize = 0;
    static int timer = 0;
    Entity * e = playerPtr();

    if ( !timer )
    {
        timer = timerAlloc();
        timerStart(timer);
        auction_updateInventory();
    }

    if ( timerElapsed(timer) >= 30.0f )
    {
        auction_updateInventory();
        timerStart(timer);
    }

    if ( e->pchar->auctionInv && !e->pchar->auctionInvUpdated )
        return;

    if ( !e->pchar->auctionInv )
        return;

    while(eaSize(&ppLocalInventory))
    {
        AuctionItem * pItem = eaRemove(&ppLocalInventory,0);
        free(pItem);
    }
    
    for ( i = 0; i < eaSize(&e->pchar->auctionInv->items); ++i )
    {
        addItemToLocalInventory((char*)e->pchar->auctionInv->items[i]->pchIdentifier, e->pchar->auctionInv->items[i]->id, 
                e->pchar->auctionInv->items[i]->auction_status, e->pchar->auctionInv->items[i]->amtStored,
                e->pchar->auctionInv->items[i]->infStored, e->pchar->auctionInv->items[i]->amtOther,
                e->pchar->auctionInv->items[i]->infPrice);
    }
}

int randRange(int low, int high)
{
    if(high < low)
    {
        int temp = low;
        low = high;
        high = temp;
    }
    return randInt(high-low+1)+low;
}

void aucscript_TrustMe(void)
{
    static bool bTrusted = false;
    if(!bTrusted)
    {
        commAddInput("auc_trust_gameclient 1");
        bTrusted = true;
    }
}

void aucscript_GetAll(void)
{
    int i;
    for(i = eaSize(&ppLocalInventory)-1; i >= 0; --i)
        auction_placeItemInInventory(ppLocalInventory[i]->auction_id);
}

void aucscript_RandomItem(bool buy, bool sell, bool history)
{
    bool finished = false;
    TrayItemType type = kTrayItemType_None;
    const char *name = NULL;
    int level = 0;
    int amount = 1;
    int price = 0;
    AuctionInvItemStatus status;

    if(buy)
    {
        if(sell)
            status = randInt(2) ? AuctionInvItemStatus_Bidding : AuctionInvItemStatus_Stored;
        else
            status = AuctionInvItemStatus_Bidding;
    }
    else
    {
        if(sell)
            status = AuctionInvItemStatus_Stored;
        else
            status = AuctionInvItemStatus_None;
    }

    if(status == AuctionInvItemStatus_Bidding)
        price = randRange(AUCSCRIPT_VALUEMIN,AUCSCRIPT_VALUEMAX);// AUCSCRIPT_BUYVALUE;

    while(!finished)
    {
        switch(randInt(4))
        {
        case 0: // salvage
            if(eaSize(&g_SalvageDict.ppSalvageItems))
            {
                const SalvageItem *s = g_SalvageDict.ppSalvageItems[randInt(eaSize(&g_SalvageDict.ppSalvageItems))];
                if(s)
                {
                    type = kTrayItemType_Salvage;
                    name = s->pchName;
                    amount = randRange(1,10);
                    finished = true;
                }
            }
        xcase 1: // recipes
            if(eaSize(&g_DetailRecipeDict.ppRecipes))
            {
                const DetailRecipe *r = g_DetailRecipeDict.ppRecipes[randInt(eaSize(&g_DetailRecipeDict.ppRecipes))];
                if(r)
                {
                    type = kTrayItemType_Recipe;
                    name = r->pchName;
                    level = r->level;
                    amount = randRange(1,10);
                    finished = true;
                }
            }
        xcase 2: // inspirations
        {
            const PowerCategory *pcat = powerdict_GetCategoryByName(&g_PowerDictionary, "Inspirations");
            if(pcat && eaSize(&pcat->ppPowerSets))
            {
                const BasePowerSet *pset = pcat->ppPowerSets[randInt(eaSize(&pcat->ppPowerSets))];
                if(pset && eaSize(&pset->ppPowers) && stricmp(pset->pchName,"Holiday")) // no Presents
                 {
                     const BasePower *ppow = pset->ppPowers[randInt(eaSize(&pset->ppPowers))];
                    if(ppow && stricmp(ppow->pchName,"Consignment_House_Transporter") // no etc.
                            && stricmp(ppow->pchName,"Black_Market_Transporter")
                            && stricmp(ppow->pchName,"Hide") )
                    {
                        type = kTrayItemType_Inspiration;
                        name = basepower_ToPath(ppow);
                        finished = true;
                    }
                }
            }
        }
        xcase 3: // enhancements
        {
            const PowerCategory *pcat = powerdict_GetCategoryByName(&g_PowerDictionary, "Boosts");
            if(pcat && eaSize(&pcat->ppPowerSets))
            {
                const BasePowerSet *pset = pcat->ppPowerSets[randInt(eaSize(&pcat->ppPowerSets))];
                if(pset && eaSize(&pset->ppPowers)
                        && strstriConst(pset->pchName,"hamidon")        != pset->pchName // no Specials
                        && strstriConst(pset->pchName,"hydra")        != pset->pchName
                        && strstriConst(pset->pchName,"synthetic")    != pset->pchName
                        && strstriConst(pset->pchName,"titan")        != pset->pchName
                        && strstriConst(pset->pchName,"yins")        != pset->pchName )
                {
                    const BasePower *ppow = pset->ppPowers[randInt(eaSize(&pset->ppPowers))];
                    if(ppow && !power_GetReplacementPowerName(ppow->pchName))
                    {
                        int crafted = baseset_IsCrafted(ppow->psetParent);
                        int found = 0;
                        int i;

                        for(i = 0; i <= SEARCHFILTER_MAXLEVEL; ++i)
                            if(!crafted || detailrecipedict_RecipeFromEnhancementAndLevel(ppow->pchFullName, i+1))
                                ++found;

                        found = randInt(found) + 1;
                        for(i = 0; i <= SEARCHFILTER_MAXLEVEL; ++i)
                            if(!crafted || detailrecipedict_RecipeFromEnhancementAndLevel(ppow->pchFullName, i+1))
                                if(!--found)
                                {
                                    type = kTrayItemType_SpecializationInventory;
                                    name = basepower_ToPath(ppow);
                                    level = i;
                                    finished = true;
                                    break;
                                }
                        }
                    }
                }
            }
        }
    }

    if(status != AuctionInvItemStatus_None)
        auction_addItem(name,0,amount,price,status,0);
    if(history)
        auction_updateHistory(name);
}

void aucscript_Seed50(void)
{
//    int i;
//    auctionHouseResetSearch(true);

    //for(i = 0; i < 50; ++i)
    //{
    //    AuctionItem *item = g_auc.searchResults[randInt(eaSize(&g_auc.searchResults))];
    //    if ( !item->name && (item->type == kTrayItemType_Inspiration || item->type == kTrayItemType_SpecializationInventory) )
    //        item->name = basepower_ToPath((BasePower*)item->pItem);
    //    auction_addItem(item->type, item->name, item->lvl, 0, 1, (randInt(10)+1)*100,
    //        rand()&1 ? AuctionInvItemStatus_ForSale : AuctionInvItemStatus_Bidding, -1);
    //}
}

void aucscript_Thrash(Entity *e, bool buy, bool sell, int sleep_time)
{
    static int iFrames = 0;
    static int iLastIdx = 0;
    static U32 history_timer = 0;
    int iSize;
    int i;

    if(!history_timer)
    {
        history_timer = timerAlloc();
    }

    if(timerElapsed(history_timer) > 0.5)
    {
        timerStart(history_timer);
        aucscript_RandomItem(false,false,true);
    }

    if(buy || sell)
        aucscript_TrustMe();

    if(!buy && !sell || ++iFrames < AUCSCRIPT_TIMEOUTSECS*1000/sleep_time)
        return;

    auctionUpdateInventory(e);
    iSize = eaSize(&ppLocalInventory);

    if(iSize < AUCSCRIPT_INVSIZE && (buy || sell))
        aucscript_RandomItem(buy,sell,false);
    else if(iSize)
    {
        for(i = 0; i < iSize; ++i)
        {
            int idx = (iLastIdx + i) % iSize;
            if(sell && ppLocalInventory[idx]->auction_status == AuctionInvItemStatus_Stored)
            {
                auction_changeItemStatus(ppLocalInventory[idx]->auction_id,
                    randRange(AUCSCRIPT_VALUEMIN,AUCSCRIPT_VALUEMAX),AuctionInvItemStatus_ForSale);
                iLastIdx = idx + 1;
                break;
            }
            if(    (!buy || ppLocalInventory[idx]->auction_status != AuctionInvItemStatus_Bidding) &&
                (!sell || ppLocalInventory[idx]->auction_status != AuctionInvItemStatus_ForSale)    )
            {
                auction_placeItemInInventory(ppLocalInventory[idx]->auction_id);
                iLastIdx = idx + 1;
                break;
            }
        }
        if(i >= iSize)
        {
            auction_placeItemInInventory(ppLocalInventory[randInt(AUCSCRIPT_INVSIZE)]->auction_id);
            iFrames = 0;
        }
    }
}

void aucscript_SellAndBuy25(bool start)
{
    static enum { done, selling, buying } state = done;
    static int i;

    if(start)
    {
        i = 0;
        state = selling;
    }
    else if(state != done)
    {
        do {
            if(i >= 25 || i >= eaSize(&g_SalvageDict.ppSalvageItems))
            {
                if(state == selling && eaSize(&g_SalvageDict.ppSalvageItems))
                {
                    i = 0;
                    state = buying;
                }
                else
                {
                    state = done;
                    break;
                }
            }

            //auction_addItem(kTrayItemType_Salvage, g_SalvageDict.ppSalvageItems[i]->pchName,
            //    0, 0, 1, AUCSCRIPT_BUYVALUE, state == selling ? AuctionInvItemStatus_ForSale : AuctionInvItemStatus_Bidding, -1);

            ++i;

        } while(rand() & 1);
    }
}

void aucscript_SellOrBuy50(bool start, bool buy)
{
    static bool started = false;
    static bool buying = false;
    static int i;

    if(start)
    {
        i = 0;
        started = true;
        buying = buy;
    }
    else if(started)
    {
        do {
            if(i >= 50 || i >= eaSize(&g_SalvageDict.ppSalvageItems))
            {
                started = false;
                break;
            }

            //auction_addItem(kTrayItemType_Salvage, g_SalvageDict.ppSalvageItems[i]->pchName,
            //    0, 0, 1, AUCSCRIPT_BUYVALUE, buying ? AuctionInvItemStatus_Bidding : AuctionInvItemStatus_ForSale, -1);

            ++i;

        } while(rand() & 1);
    }
}
