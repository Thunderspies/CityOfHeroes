#include "auction/AuctionData.h"
#include "auction/Auction.h"
#include "account/AccountCatalog.h"
#include <utilitieslib/components/earray.h>
#include <utilitieslib/components/StashTable.h>
#include "gameComm/trayCommon.h"
#include "entity/salvage.h"
#include "bases/DetailRecipe.h"
#include "entity/powers.h"
#include "entity/boostset.h"
#include "entity/origins.h"
#include "entity/attrib_names.h"
#include <utilitieslib/language/MessageStoreUtil.h>
#include <utilitieslib/utils/strings_opt.h>
#include <utilitieslib/components/StringCache.h>
#include <utilitieslib/components/EString.h>
#include <utilitieslib/utils/mathutil.h>
#include "entity/character_eval.h"

#if CLIENT
#include <utilitieslib/language/MessageStoreUtil.h>
#endif

AuctionItemDict g_AuctionItemDict;

char* auction_identifier( int type, const char *name, int level )
{
    static char *pchIdentifier;
    estrClear(&pchIdentifier);
    estrConcatf(&pchIdentifier, "%i %s %i", type, name, level);
    return pchIdentifier;
}

char* auction_identifierFromItem(AuctionItem * item, TrayItemType type, int level )
{
    static char *estr;

    estrClear(&estr);

    switch( type )
    {
        xcase kTrayItemType_Salvage:
        {
            SalvageItem *pSalvage = item->salvage;
            estrConcatf( &estr, "%i %s 0", kTrayItemType_Salvage, pSalvage->pchName );
        }
        xcase kTrayItemType_Recipe:
        {
            const DetailRecipe *pRecipe = item->recipe;
            estrConcatf( &estr, "%i %s %i", kTrayItemType_Recipe, pRecipe->pchName, pRecipe->level );
        }
        xcase kTrayItemType_Inspiration:
        {
            const BasePower *pPowBase = item->inspiration;
            estrConcatf( &estr, "%i %s 0", kTrayItemType_Inspiration, pPowBase->pchFullName );
        }
        xcase kTrayItemType_SpecializationInventory:
        {
            const BasePower *pPowBase = item->enhancement;
            estrConcatf( &estr, "%i %s %i", kTrayItemType_SpecializationInventory, pPowBase->pchFullName, level );
        }
        xcase kTrayItemType_Power:
        {
            const BasePower *pPowBase = item->temp_power;
            estrConcatf( &estr, "%i %s 0", kTrayItemType_Power, pPowBase->pchFullName );
        }
    }

    return estr;

}

int auctionData_GetID(int type, const char *name, int level)
{
    AuctionItem * pItem;
    if( stashFindPointer( auctionData_GetDict()->stAuctionItems, auction_identifier(type, name, level), &pItem) )
        return pItem->id;
    else
        return -1;
}

void auction_AddToDict( void * item, TrayItemType type, int playerType )
{
    int auction_id = 0;

    AuctionItem *pItem = calloc(1, sizeof(AuctionItem));

    pItem->pItem = item;
    pItem->type = type;
    pItem->playerType = playerType;

    if( !g_AuctionItemDict.stAuctionItems )
        g_AuctionItemDict.stAuctionItems = stashTableCreateWithStringKeys( 4000, StashDefault );


    switch( type )
    {
        xcase kTrayItemType_Salvage:
        {
            pItem->pchIdentifier = strdup( auction_identifierFromItem(pItem, kTrayItemType_Salvage, 0) ); 
            stashAddPointer(g_AuctionItemDict.stAuctionItems ,pItem->pchIdentifier , pItem, 0 );
            pItem->displayName = pItem->salvage->ui.pchDisplayName;
        }
        xcase kTrayItemType_Recipe:
        {

            pItem->lvl = pItem->recipe->level;
            pItem->pchIdentifier = strdup( auction_identifierFromItem(pItem, kTrayItemType_Recipe, 0) ); 
            stashAddPointer(g_AuctionItemDict.stAuctionItems , pItem->pchIdentifier, pItem, 0 );
            pItem->displayName = pItem->recipe->ui.pchDisplayName;
        }
        xcase kTrayItemType_Inspiration:
        {
            pItem->pchIdentifier = strdup( auction_identifierFromItem(pItem, kTrayItemType_Inspiration, 0) ); 
            stashAddPointer(g_AuctionItemDict.stAuctionItems , pItem->pchIdentifier, pItem, 0 );
            pItem->displayName = pItem->inspiration->pchDisplayName;
        }
        xcase kTrayItemType_SpecializationInventory:
        {
            int i, minlevel = 53, maxlevel = -1;
            BasePower *pBase = (BasePower *)item;

            SAFE_FREE(pItem);

            if(baseset_IsCrafted(pBase->psetParent))
            {
                for( i = 0; i < 53; i++ )
                {    
                    const DetailRecipe *pRecipe = detailrecipedict_RecipeFromEnhancementAndLevel(pBase->pchFullName, i+1);
                    if( pRecipe )
                    {
                            AuctionItem *pItem = calloc(1, sizeof(AuctionItem));
                            pItem->pItem = item;
                            pItem->type = type;
                            pItem->lvl = i;
                            pItem->pchIdentifier = strdup( auction_identifierFromItem(pItem, kTrayItemType_SpecializationInventory, i) ); 
                            stashAddPointer(g_AuctionItemDict.stAuctionItems , pItem->pchIdentifier, pItem, 0 );
                            pItem->displayName = pBase->pchDisplayName;
                            eaPush(&g_AuctionItemDict.ppItems, pItem);
                            pItem->id = eaSize(&g_AuctionItemDict.ppItems)-1;
#if CLIENT
                            pItem->sortName = strdup( textStd(pItem->displayName) );
#endif
                    }
                }
            }
            else
            {
                minlevel = 0;
                maxlevel = 53;
            }
        
            for( i = minlevel; i < maxlevel; i++ )
            {
                AuctionItem *pItem = calloc(1, sizeof(AuctionItem));
                pItem->pItem = item;
                pItem->type = type;
                pItem->lvl = i;
                pItem->pchIdentifier = strdup( auction_identifierFromItem(pItem, kTrayItemType_SpecializationInventory, i) ); 
                stashAddPointer(g_AuctionItemDict.stAuctionItems , pItem->pchIdentifier, pItem, 0 );
                pItem->displayName = pBase->pchDisplayName;
                eaPush(&g_AuctionItemDict.ppItems, pItem);
                pItem->id = eaSize(&g_AuctionItemDict.ppItems)-1;
#if CLIENT
                pItem->sortName = strdup( textStd(pItem->displayName) );
#endif
            }
        }
        xcase kTrayItemType_Power:
        {
            pItem->pchIdentifier = strdup( auction_identifierFromItem(pItem, kTrayItemType_Power, 0) );
            pItem->buyPrice = 10000;
            stashAddPointer(g_AuctionItemDict.stAuctionItems , pItem->pchIdentifier, pItem, 0 );
            pItem->displayName = pItem->temp_power->pchDisplayName;
        }
        default:
            break;
    }

    if(pItem)
    {
#if CLIENT
        pItem->sortName = strdup( textStd(pItem->displayName) );
#endif
        eaPush(&g_AuctionItemDict.ppItems, pItem);
        pItem->id = eaSize(&g_AuctionItemDict.ppItems)-1;
    }
}


void auctiondata_Init() // this creates a list of all auctionable items
{
    int i, j;
    const PowerCategory *pcat;

    // Salvage
    for( i = eaSize( &g_SalvageDict.ppSalvageItems ) - 1; i >= 0; --i)
    {
        if (!(g_SalvageDict.ppSalvageItems [i]->flags & SALVAGE_NOAUCTION))
            auction_AddToDict( (void*)g_SalvageDict.ppSalvageItems[i], kTrayItemType_Salvage, 0 );
    }

    // Recipes
    for( i = eaSize( &g_DetailRecipeDict.ppRecipes ) - 1; i >= 0; --i)
    {
        if ( (g_DetailRecipeDict.ppRecipes[i]->flags & RECIPE_NO_TRADE) || ( g_DetailRecipeDict.ppRecipes[i]->Type != kRecipeType_Drop ) )
            continue;

        auction_AddToDict( (void*)g_DetailRecipeDict.ppRecipes[i], kTrayItemType_Recipe, 0 );
    }

    // Inspirations
    pcat = powerdict_GetCategoryByName(&g_PowerDictionary, "Inspirations");
    if ( pcat )
    {
        for ( i = 0; i < eaSize(&pcat->ppPowerSets); ++i )
        {
            const BasePowerSet *pset = pcat->ppPowerSets[i];
            if ( pset )
            {
                for ( j = 0; j < eaSize(&pset->ppPowers); ++j )
                {
                    const BasePower *ppow = pset->ppPowers[j];
                    // bBoostAccountBound is prevented from being used in the store in AuctionClient.c::VerifyAddInvItem
                    // Account bound inspirations need to be added to the auction dictionary because global email uses that same dictionary?
                    if (ppow && ppow->bBoostTradeable)
                        auction_AddToDict((void*)ppow, kTrayItemType_Inspiration, 0 );
                }
            }
        }
    }

    // Enhancements
    pcat = powerdict_GetCategoryByName(&g_PowerDictionary, "Boosts");
    if ( pcat )
    {
        for ( i = 0; i < eaSize(&pcat->ppPowerSets); ++i )
        {
            const BasePowerSet *pset = pcat->ppPowerSets[i];
            if ( pset )
            {
                for ( j = 0; j < eaSize(&pset->ppPowers); ++j )
                {
                    const BasePower *ppow = pset->ppPowers[j];
                    // bBoostAccountBound is prevented from being used in the store in AuctionClient.c::VerifyAddInvItem
                    // Account bound boosts need to be added to the auction dictionary because global email uses that same dictionary?

                    if ( ppow && !power_GetReplacementPowerName(ppow->pchName) && ppow->bBoostTradeable )
                        auction_AddToDict((void*)ppow, kTrayItemType_SpecializationInventory, 0 );
                }
            }
        }
    }

    // Temp Powers
    {
        const BasePower *ppow;
        ppow = powerdict_GetBasePowerByName(&g_PowerDictionary, "Temporary_Powers", "Temporary_Powers", "Consignment_House_Transporter");
        if(ppow)
            auction_AddToDict((void*)ppow, kTrayItemType_Power, 0 );
        ppow = powerdict_GetBasePowerByName(&g_PowerDictionary, "Temporary_Powers", "Temporary_Powers", "Black_Market_Transporter");
        if(ppow)
            auction_AddToDict((void*)ppow, kTrayItemType_Power, 0 );
        ppow = powerdict_GetBasePowerByName(&g_PowerDictionary, "Temporary_Powers", "Temporary_Powers", "loyalist_transporter");
        if(ppow)
            auction_AddToDict((void*)ppow, kTrayItemType_Power, 0 );
        ppow = powerdict_GetBasePowerByName(&g_PowerDictionary, "Temporary_Powers", "Temporary_Powers", "resistance_transporter");
        if(ppow)
            auction_AddToDict((void*)ppow, kTrayItemType_Power, 0 );
    }
}

//make sure to check if ppow->bBoostUsePlayerLevel is true before calling this function
bool IsAttunedEnhancementAllowedInAuctionHouse(BasePower *ppow, const int level)
{
    const DetailRecipe *pRecipe = detailrecipedict_RecipeFromEnhancementAndLevel(ppow->pchFullName, level+1);
    return pRecipe ? true : false;
}

static bool IsSalvageItemAllowedInAuctionHouse(const SalvageItem *salvage)
{
    if (!salvage)
    {
        return false;
    }

    if (salvage->pchStoreProduct != NULL && !accountCatalogIsSkuPublished(skuIdFromString(salvage->pchStoreProduct)))
    {
        return false;
    }

    return true;
}

static bool IsBasePowerAllowedInAuctionHouse(BasePower *ppow)
{
    if (!ppow)
        return false;

    if (ppow->bBoostAccountBound || !ppow->bBoostTradeable)
        return false;

    if (ppow->pchStoreProduct != NULL && !accountCatalogIsSkuPublished(skuIdFromString(ppow->pchStoreProduct)))
        return false;

    return true;
}

static bool IsBasePowerAllowedInAuctionHouseByLevel(BasePower *ppow, int level)
{
    if (!IsBasePowerAllowedInAuctionHouse(ppow))
        return false;

    if (ppow->bBoostUsePlayerLevel && !IsAttunedEnhancementAllowedInAuctionHouse(ppow, level))
    {
        return false;
    }

    return true;
}

//****************************************************************
//temporary hack to remove invalid auction items (such as account-bound items) from the results list
//whenever a user performs a generic search on the auction house.  A potential solution is to modify
//g_AuctionItemDict so that it no longer has invalid auction items AND have global mail use
//a different list instead of g_AuctionItemDict.  
//
//However, we can't remove "invalid" auction items yet because some have already appeared in the AuctionHouse,
//and some players have buy bids for these "invalid" auction items.  Doing this allows players to circumvent
//the influence cap by using the auction house like a bank (players "deposit" influence by placing a buy order
//that never gets fulfilled, then "withdraw" the influence by canceling the buy order). Removing the "invalid" auction
//items from the auction house list would invalidate those buy bids, and players would lose the influence they
//spent to make the buy order.  This issue would need to be handled properly before an attempt to clean up
//g_AuctionItemDict can be made. --JH
//*****************************************************************
bool IsAuctionItemValid(AuctionItem *pItem)
{
    if (!pItem)
    {
        return false;
    }

    switch( pItem->type )
    {
        xcase kTrayItemType_Salvage:
        {
            return IsSalvageItemAllowedInAuctionHouse(pItem->salvage);
        }
        xcase kTrayItemType_Recipe:
        {
            return true;
        }
        xcase kTrayItemType_Inspiration:
        {
            return IsBasePowerAllowedInAuctionHouse(pItem->inspiration);
        }
        xcase kTrayItemType_SpecializationInventory:
        {
            return IsBasePowerAllowedInAuctionHouseByLevel(pItem->enhancement, pItem->lvl);
        }
        xcase kTrayItemType_Power:
        {
            return true;
        }
    }
    return false;
}

bool DoesAuctionItemPassAuctionRequiresCheck(AuctionItem *pItem, Character *pchar)
{
    if (!pItem || !pchar)
    {
        return false;
    }

    switch( pItem->type )
    {
        xcase kTrayItemType_Salvage:
        {
            if (pItem->salvage->ppchAuctionRequires && !chareval_Eval(pchar, pItem->salvage->ppchAuctionRequires, "DEFS/INVENTION/SALVAGE.SALVAGE"))
            {
                return false;
            }
        }
        xcase kTrayItemType_Inspiration:
        {
            if (pItem->inspiration->ppchAuctionRequires && !chareval_Eval(pchar, pItem->inspiration->ppchAuctionRequires, pItem->inspiration->pchSourceFile))
            {
                return false;
            }
        }
        xcase kTrayItemType_Recipe:
        {
            if (pItem->recipe->ppchAuctionRequires && !chareval_Eval(pchar, pItem->recipe->ppchAuctionRequires, pItem->recipe->pchSourceFile))
            {
                return false;
            }
        }
        xcase kTrayItemType_SpecializationInventory:
        {
            if (pItem->enhancement->ppchAuctionRequires && !chareval_Eval(pchar, pItem->enhancement->ppchAuctionRequires, pItem->enhancement->pchSourceFile))
            {
                return false;
            }
        }

    }
    return true;
}