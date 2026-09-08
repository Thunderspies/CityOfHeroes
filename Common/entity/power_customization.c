#include "entity/power_customization.h"
#include "entity/entity.h"
#include "player/player.h"
#include <utilitieslib/network/net_packet.h>            // for pkt.. functions
#include "entity/entPlayer.h"
#include <utilitieslib/components/earray.h>
#include "entity/powers.h"
#include <utilitieslib/components/EString.h>
#include "entity/classes.h"
#include "entity/character_base.h"
#include "gameData/costume_data.h"
#include <utilitieslib/utils/Color.h>
#include "entity/costume.h"
#include "entity/LoadDefCommon.h"
#include <utilitieslib/network/netcomp.h>
#include <utilitieslib/components/StashTable.h>
#include <utilitieslib/utils/FolderCache.h>
#include "entity/character_eval.h"
#if CLIENT
#include "entity/power_customization_client.h"
#elif SERVER
#include "entity/power_customization_server.h"
#include "Reward.h"
#include <utilitieslib/utils/error.h>
#endif

TokenizerParseInfo ParsePowerCustomization[] = 
{
    {    "PowerName",    TOK_POOL_STRING|TOK_STRING(PowerCustomization, powerFullName, "none")    },

    // This line's a bit of a hack.  We use this TPI to copy the structure, so I'm using this line to shallow copy the pointer
    // This means that the pointer value gets output to file when we write the text file.
    // Be SURE when you read this struct to overwrite this value with the real value derived from the PowerName string!
    {    "Power",        TOK_AUTOINT(PowerCustomization, power, 0)                                },

    {    "Color1",        TOK_RGBA(PowerCustomization, customTint.primary.rgba)                    },
    {    "Color2",        TOK_RGBA(PowerCustomization, customTint.secondary.rgba)                    },
    {    "Token",        TOK_POOL_STRING|TOK_STRING(PowerCustomization, token, 0)                },
    {    "{",            TOK_START                                                                },
    {    "}",            TOK_END                                                                    },
    {    "", 0, 0 }
};

TokenizerParseInfo ParsePowerCustomizationList[] = 
{
    {    "PowerCustomization",    TOK_STRUCT(PowerCustomizationList, powerCustomizations, ParsePowerCustomization)    },
    {    "{",            TOK_START                                                                },
    {    "}",            TOK_END                                                                    },
    {    "", 0, 0 }
};

TokenizerParseInfo ParseDBPowerCustomization[] =
{
    {    "CategoryName",    TOK_POOL_STRING|TOK_STRING(DBPowerCustomization, powerCatName, 0)                    },
    {    "SetName",        TOK_POOL_STRING|TOK_STRING(DBPowerCustomization, powerSetName,0)                    },
    {    "PowerName",    TOK_POOL_STRING|TOK_STRING(DBPowerCustomization, powerName, 0)                    },
    {    "Color1",        TOK_RGBA(DBPowerCustomization, customTint.primary.rgba)                                },
    {    "Color2",        TOK_RGBA(DBPowerCustomization, customTint.secondary.rgba)                                },
    {    "Token",        TOK_POOL_STRING|TOK_STRING(DBPowerCustomization, token, 0)},
    {    "SlotId",        TOK_INT(DBPowerCustomization, slotId, 0)},
    {    "{",            TOK_START                                                                },
    {    "}",            TOK_END                                                                    },
    {    "", 0, 0 }
};

TokenizerParseInfo ParsePowerCustomizationCost[] =
{
    { "MinLevel",        TOK_INT(PowerCustomizationCost, min_level, 0)        },
    { "MaxLevel",        TOK_INT(PowerCustomizationCost, max_level, 0)        },
    { "EntryFee",        TOK_INT(PowerCustomizationCost, entry_fee, 0)        },
    { "PowerTokenCost",    TOK_INT(PowerCustomizationCost, power_token_cost, 0)        },
    { "PowerColorCost",    TOK_INT(PowerCustomizationCost, power_color_cost, 0)        },
    { "End",            TOK_END                                    },
    { "", 0, 0 }
};

TokenizerParseInfo ParsePowerCustomizationLevelsCost[] =
{
    { "TailorPowersCostSet", TOK_STRUCT(PowerCustomizationLevelsCost, powerCustCostList, ParsePowerCustomizationCost) },
    { "", 0, 0 }
};

SERVER_SHARED_MEMORY PowerCustomizationLevelsCost gPowerCustomizationCostMaster = {0};

TokenizerParseInfo ParsePowerCustomizationMenuCategory[] =
{
    { "Category", TOK_STRING(PowerCustomizationMenuCategory, categoryName, 0) },
    { "Sets", TOK_STRINGARRAY(PowerCustomizationMenuCategory, powerSets) },
    { "HideIf", TOK_STRING(PowerCustomizationMenuCategory, hideIf, 0) },
    {    "{",            TOK_START                                                                },
    {    "}",            TOK_END                                                                    },
    { "", 0, 0 }
};

TokenizerParseInfo ParsePowerCustomizationMenu[] =
{
    { "Menu", TOK_STRUCT(PowerCustomizationMenu, pCategories, ParsePowerCustomizationMenuCategory) },
    { "", 0, 0 }
};

SERVER_SHARED_MEMORY PowerCustomizationMenu gPowerCustomizationMenu = {0};

// @todo -- power_customization_server uses this hash table, but it never has any items in it since s_addKeys is never called.
//    Not sure if this is needed or was just for debugging.  
StashTable powerCustReward_HashTable;
#if 0
static void s_addKeys(char **keys)
{
    int i;
    for(i = eaSize(&keys)-1; i >= 0; --i)
    {
        if(stashAddPointer(powerCustReward_HashTable, keys[i], 0 , false))
        {
#if SERVER
            rewardAddPowerCustThemeToHashTable(keys[i]);
#endif
        }
    }
}

void powerCust_initHashTable()
{
//    int i;
    if( !powerCustReward_HashTable )
    {
        powerCustReward_HashTable = stashTableCreateWithStringKeys(20, StashDeepCopyKeys);
    }
}
#endif

// not quite commutative:  two nulls will return 1, not 0.  did this because it shouldn't matter and it's faster
// nulls go at the end of the list
int comparePowerCustomizations(const void* lhsData, const void* rhsData)
{
    const PowerCustomization** lhs = (const PowerCustomization**)lhsData;
    const PowerCustomization** rhs = (const PowerCustomization**)rhsData;
    if (!lhs || !(*lhs)->power)
        return 1;
    else if (!rhs || !(*rhs)->power)
        return -1;
    else if ((*lhs)->power->crcFullName < (*rhs)->power->crcFullName)
        return -1;
    else if ((*lhs)->power->crcFullName > (*rhs)->power->crcFullName)
        return 1;
    else
        return 0;
}

// not quite commutative:  two nulls will return 1, not 0.  did this because it shouldn't matter and it's faster
// nulls go at the end of the list
int searchPowerCustomizations(const void* powerData, const void* custData)
{
    const BasePower** power = (const BasePower**)powerData;
    const PowerCustomization** cust = (const PowerCustomization**)custData;
    if (!power || !(*power))
        return 1;
    else if (!cust || !(*cust)->power)
        return -1;
    else if ((*power)->crcFullName < (*cust)->power->crcFullName)
        return -1;
    else if ((*power)->crcFullName > (*cust)->power->crcFullName)
        return 1;
    else
        return 0;
}

bool PowerCustomizationMenuPostProcessInternal(PowerCustomizationMenu *menu)
{
    int i;

    for (i = 0; i < eaSize(&menu->pCategories); i++)
    {
        PowerCustomizationMenuCategory *category = menu->pCategories[i];

        if (category->hideIf)
        {
            chareval_requiresValidate(category->hideIf, "Defs/powercustomizationmenu.def");
        }
    }
    return true;
}

#if CLIENT
bool PowerCustomizationMenuPostProcessClient(ParseTable* pti, void* structptr)
{
    PowerCustomizationMenu * menu = (PowerCustomizationMenu *)structptr;
    return PowerCustomizationMenuPostProcessInternal(menu);
}
#endif

#if SERVER
bool PowerCustomizationMenuPostProcessServer(ParseTable* pti, void* structptr, bool shared_memory)
{
    PowerCustomizationMenu * menu = (PowerCustomizationMenu *)structptr;
    return PowerCustomizationMenuPostProcessInternal(menu);
}
#endif

void loadPowerCustomizations()
{
#if CLIENT
    if (!ParserLoadFiles(NULL, "Menu/PowerCustomization/powercustomizationcost.ctm", "powercustomizationcost.bin", 0, 
        ParsePowerCustomizationLevelsCost, &gPowerCustomizationCostMaster, NULL, NULL, NULL))
    {
        printf("Couldn't load Power Customization Data!!\n");
    }
    TODO(); // this never did anything meaningful, need to hook it up??
    //FolderCacheSetCallback(FOLDER_CACHE_CALLBACK_UPDATE, "menu/PowerCustomization/*.ctm", reloadPowerCustCallback);

    if (!ParserLoadFiles(NULL, "Defs/powercustomizationmenu.def", "powercustomizationmenu.bin", 0, 
        ParsePowerCustomizationMenu, &gPowerCustomizationMenu, NULL, NULL, PowerCustomizationMenuPostProcessClient))
    {
        printf("Couldn't load Power Customization Data!!\n");
    }
#elif SERVER
    loadstart_printf("Loading Power Customization..." );
    if (!ParserLoadFilesShared(MakeSharedMemoryName("powercustomizationcost.bin"), NULL, "Menu/PowerCustomization/powercustomizationcost.ctm", 
        "powercustomizationcost.bin", 0, ParsePowerCustomizationLevelsCost, &gPowerCustomizationCostMaster, 
        sizeof(gPowerCustomizationCostMaster), NULL, NULL, NULL, NULL, NULL))
    {
        printf("Couldn't load Power Customization Data!!\n");
    }
    if (!ParserLoadFilesShared(MakeSharedMemoryName("powercustomizationmenu.bin"), NULL, "Defs/powercustomizationmenu.def", 
        "powercustomizationmenu.bin", 0, ParsePowerCustomizationMenu, &gPowerCustomizationMenu, 
        sizeof(gPowerCustomizationMenu), NULL, NULL, NULL, NULL, PowerCustomizationMenuPostProcessServer))
    {
        printf("Couldn't load Power Customization Data!!\n");
    }

    loadend_printf("" );
#endif
}

PowerCustomizationList * powerCustList_current( Entity * e )
{
    if (!e || !e->pl)
        return NULL;

    return e->pl->powerCustomizationLists[e->pl->current_powerCust];
}

PowerCustomization* powerCust_FindPowerCustomization(PowerCustomizationList* powerCustList, const BasePower *power)
{
    PowerCustomization **result;

    if (!powerCustList)
        return NULL;

    result = eaBSearch(powerCustList->powerCustomizations, searchPowerCustomizations, power);

    return result ? *result : NULL;
}

PowerCustomizationList * powerCustList_clone(PowerCustomizationList *src_powerCustList)
{
    PowerCustomizationList *dst_powerCustList;
    if (!src_powerCustList)
        return NULL;

    dst_powerCustList = StructAllocRaw(sizeof(PowerCustomizationList));
    StructCopy(ParsePowerCustomizationList, src_powerCustList, dst_powerCustList, 0, 0);
    return dst_powerCustList;
}

void powerCustList_initializePowerFromFullName(PowerCustomizationList *powerCustList)
{
    int custIndex;
    for (custIndex = eaSize(&powerCustList->powerCustomizations) - 1; custIndex >= 0; custIndex--)
    {
        PowerCustomization *cust = powerCustList->powerCustomizations[custIndex];
        cust->power = powerdict_GetBasePowerByFullName(&g_PowerDictionary, cust->powerFullName);
    }
}

int powerCustList_receive(Packet *pak, Entity *e, PowerCustomizationList *powerCustList)
{
    if (pktGetBits(pak, 1))
    {
        int numCusts = pktGetBitsAuto(pak);
        int i;
        PowerCustomizationList *tempList = StructAllocRaw(sizeof(PowerCustomizationList));
        StructInit(tempList, sizeof(PowerCustomizationList), ParsePowerCustomizationList);
        for (i = 0; i < numCusts; ++i)
        {
            PowerCustomization *cust = StructAllocRaw(sizeof(PowerCustomization));
            int crcValue = pktGetBitsAuto(pak);
            cust->power = powerdict_GetBasePowerByCRC(&g_PowerDictionary, crcValue);
            cust->powerFullName = cust->power ? cust->power->pchFullName : "none";
            cust->customTint.primary.integer = pktGetBitsAuto(pak);
            cust->customTint.secondary.integer = pktGetBitsAuto(pak);
            cust->token = StructAllocString(pktGetString(pak));
            eaPush(&tempList->powerCustomizations, cust);
        }
        eaQSort(tempList->powerCustomizations, comparePowerCustomizations);

#if SERVER
        //    validate
        if (!powerCustList_validate(tempList, e, 1))
        {
            StructDestroy(ParsePowerCustomizationList, tempList);
            return 1;
        }
#endif
        StructCopy(ParsePowerCustomizationList, tempList, powerCustList, 0, 0);
        StructDestroy(ParsePowerCustomizationList, tempList);
    }
    return 1;
}
void powerCustList_send(Packet* pak, PowerCustomizationList * powerCustList)
{
    pktSendBits(pak, 1, powerCustList ? 1 : 0);
    if (powerCustList)
    {
        int i;
        int numCusts = eaSize(&powerCustList->powerCustomizations);
        pktSendBitsAuto(pak, numCusts);
        for (i = 0; i < numCusts; ++i)
        {
            PowerCustomization *cust = powerCustList->powerCustomizations[i];
            pktSendBitsAuto(pak, cust->power->crcFullName);
            pktSendBitsAuto(pak, cust->customTint.primary.integer);
            pktSendBitsAuto(pak, cust->customTint.secondary.integer);
            pktSendString(pak, cust->token);
        }
    }
}
static PowerCustomizationCost *powerCustCost_getCurrent(Entity *e)
{
    int i;
    for (i = 0; i < eaSize(&gPowerCustomizationCostMaster.powerCustCostList); ++i)
    {
        if (    ((e->pchar->iLevel+1) >= gPowerCustomizationCostMaster.powerCustCostList[i]->min_level) &&
            ((e->pchar->iLevel+1) <= gPowerCustomizationCostMaster.powerCustCostList[i]->max_level)    )
        {
            return gPowerCustomizationCostMaster.powerCustCostList[i];
        }
    }
    assertmsgf(0, "PowerCustomizationCost was not defined for all levels. Missing level %d", e->pchar->iLevel+1);
    return NULL;
}
static PowerCustomizationCost *powerCustCost_getNext(Entity *e)
{
    int i;
    for (i = 0; i < eaSize(&gPowerCustomizationCostMaster.powerCustCostList); ++i)
    {
        if (    ((e->pchar->iLevel+1) >= gPowerCustomizationCostMaster.powerCustCostList[i]->min_level) &&
            ((e->pchar->iLevel+1) <= gPowerCustomizationCostMaster.powerCustCostList[i]->max_level)    )
        {
            if (EAINRANGE(i+1, gPowerCustomizationCostMaster.powerCustCostList))
                return gPowerCustomizationCostMaster.powerCustCostList[i+1];
            else
                return gPowerCustomizationCostMaster.powerCustCostList[i];
        }
    }
    assertmsgf(0, "PowerCustomizationCost was not defined for all levels. Missing level %d", e->pchar->iLevel+1);
    return NULL;
}

// This function guarantees that e->powerCust will contain PowerCustomizations for each
// customizable power in each customizable power set that the player owns and no empty customizations.
void powerCust_syncPowerCustomizations(Entity* e)
{
    int categoryIndex, customizationIndex, categoryCount = 0;
    PowerCustomization **powerCustomizations = e->powerCust->powerCustomizations;
    PowerCustomization *powerCustomization;
#define SYNC_POWER_CUST_MAX_CATEGORY_COUNT 40
    const PowerCategory *allowedCategories[SYNC_POWER_CUST_MAX_CATEGORY_COUNT];
    PowerSet** powerSet;
    int numPowerSets = eaSize(&e->pchar->ppPowerSets);
    const BasePower** power;
    int shallPass;

    // Collect the list of all customizable power categories
    if (e->pchar->pclass) // this fails on character creation
    {
        allowedCategories[categoryCount] = e->pchar->pclass->pcat[kCategory_Primary];
        categoryCount++;
        allowedCategories[categoryCount] = e->pchar->pclass->pcat[kCategory_Secondary];
        categoryCount++;
        allowedCategories[categoryCount] = e->pchar->pclass->pcat[kCategory_Pool];
        categoryCount++;
        allowedCategories[categoryCount] = e->pchar->pclass->pcat[kCategory_Epic];
        categoryCount++;
    }
    for (categoryIndex = eaSize(&gPowerCustomizationMenu.pCategories) - 1; categoryIndex >= 0; categoryIndex--)
    {
        allowedCategories[categoryCount] = powerdict_GetCategoryByName(&g_PowerDictionary, gPowerCustomizationMenu.pCategories[categoryIndex]->categoryName);
        categoryCount++;
    }

    // Remove any items from the list that are empty
    // NOTE:  We don't remove items from the list that are for powers I can't possibly have.
    //  I'm not sure how to achieve that, and I don't think it's ever been implemented...
    for (customizationIndex = eaSize(&powerCustomizations) - 1; customizationIndex >= 0; customizationIndex--)
    {
        if (!powerCustomizations[customizationIndex]->power)
        {
            StructDestroy(ParsePowerCustomization, powerCustomizations[customizationIndex]);
            eaRemove(&powerCustomizations, customizationIndex);
        }
    }
    
    // Add any items to the list that should be there, but aren't.
    for (powerSet = e->pchar->ppPowerSets; powerSet < e->pchar->ppPowerSets + numPowerSets; ++powerSet)
    {
        shallPass = 0;
        for (categoryIndex = categoryCount - 1; categoryIndex >= 0; categoryIndex--)
        {
            if ((**powerSet).psetBase->pcatParent == allowedCategories[categoryIndex])
            {
                shallPass = 1;
                break;
            }
        }
        if (!shallPass)
            continue;

        for (power = (**powerSet).psetBase->ppPowers; 
            power < (**powerSet).psetBase->ppPowers + eaSize(&(**powerSet).psetBase->ppPowers); 
            ++power)
        {
            if (eaSize(&(**power).customfx) == 0)
                continue;

            powerCustomization = powerCust_FindPowerCustomization(e->powerCust, (*power));

            if (!powerCustomization)
            {
                // Customization wasn't found.  Create it
                powerCustomization = StructAllocRaw(sizeof(PowerCustomization));
                powerCustomization->power = (*power);
                powerCustomization->powerFullName = (*power)->pchFullName;
                // TODO: Select default tints from the appropriate palette
                powerCustomization->customTint = colorPairNone;
                powerCustomization->token = NULL;
                eaSortedInsert(&e->powerCust->powerCustomizations, comparePowerCustomizations, powerCustomization);
            }
        }
    }
}

int powerCust_CalcPowerCost( Entity *e, PowerCustomizationList *originalPowerCustList, PowerCustomizationList *newPowerCustList, const BasePower *power, int *colorChangeCharged)
{
    int total = 0;
    PowerCustomizationCost *current = powerCustCost_getCurrent(e);
    PowerCustomizationCost *next = powerCustCost_getNext(e);
    float lvlScale = 0.0f;
    PowerCustomization *newPower;
    PowerCustomization *oldPower;
    assertmsg(originalPowerCustList, "Missing Original Power Cust");
    assertmsg(newPowerCustList, "Missing Current Power Cust");
    assertmsg(power, "Missing Current Power");

    if (next == current)
    {
        lvlScale = 0.0f;
    }
    else
    {
        lvlScale = (float)((e->pchar->iLevel+1)-current->min_level)/(next->min_level-current->min_level);
    }

    newPower = powerCust_FindPowerCustomization(newPowerCustList, power);
    oldPower = powerCust_FindPowerCustomization(originalPowerCustList, power);

    if (newPower && oldPower)
    {
        if (isNullOrNone(oldPower->token))
        {
            //    used to not have any
            if (!isNullOrNone(newPower->token))
            {
                //    new one does
                total += current->power_token_cost + ((next->power_token_cost-current->power_token_cost)*lvlScale);
            }
        }
        else
        {
            if (isNullOrNone(newPower->token))
            {
                //    different
                total += current->power_token_cost + ((next->power_token_cost-current->power_token_cost)*lvlScale);
            }
            else if (stricmp(newPower->token, oldPower->token) != 0)
            {
                // different
                total += current->power_token_cost + ((next->power_token_cost-current->power_token_cost)*lvlScale);
            }
            else
            {
                if (!(*colorChangeCharged))
                {
                    if (oldPower->customTint.primary.integer != newPower->customTint.primary.integer)
                    {
                        total += current->power_color_cost + ((next->power_color_cost-current->power_color_cost)*lvlScale);
                        *colorChangeCharged = 1;
                    }
                    else if (oldPower->customTint.secondary.integer != newPower->customTint.secondary.integer)
                    {
                        total += current->power_color_cost + ((next->power_color_cost-current->power_color_cost)*lvlScale);
                        *colorChangeCharged = 1;
                    }
                }
            }
        }
    }
    return total;
}

int powerCust_PowerSetCost( Entity *e, PowerCustomizationList *originalCustList, PowerCustomizationList *newCustList, const BasePowerSet *psetBase, int *colorChangeCharged )
{
    int total = 0;
    if (e && psetBase)
    {
        int j;
        for (j = 0; j < eaSize(&psetBase->ppPowers); ++j)
        {
            total += powerCust_CalcPowerCost(e, originalCustList, newCustList, psetBase->ppPowers[j], colorChangeCharged);
        }
    }
    return total;
}
int powerCust_GetChangeCost( Entity *e, PowerCustomizationList *oldList, PowerCustomizationList *newList, int *colorChangeOccurred )
{
    int total = 0;
    if (e && e->pchar && e->pchar->ppPowerSets)
    {
        int i;
        for (i = 0; i < eaSize(&e->pchar->ppPowerSets); ++i)
        {
            int j;
            for (j = kCategory_Primary; j < kCategory_Count; ++j)
            {
                if (e->pchar->ppPowerSets[i]->psetBase->pcatParent == e->pchar->pclass->pcat[j])
                {
                    total += powerCust_PowerSetCost(e, oldList, newList, e->pchar->ppPowerSets[i]->psetBase, colorChangeOccurred );
                }
            }
            for (j = 0; j < eaSize(&gPowerCustomizationMenu.pCategories); ++j)
            {
                PowerCustomizationMenuCategory *menuCategory =  gPowerCustomizationMenu.pCategories[j];
                const PowerCategory *allowedCategory = powerdict_GetCategoryByName(&g_PowerDictionary, menuCategory->categoryName);
                if (e->pchar->ppPowerSets[i]->psetBase->pcatParent == allowedCategory)
                {
                    int k;
                    for (k = 0; k < eaSize(&menuCategory->powerSets); ++k)
                    {
                        if (stricmp(menuCategory->powerSets[k], e->pchar->ppPowerSets[i]->psetBase->pchName) == 0)
                        {
                            total += powerCust_PowerSetCost(e, oldList, newList, e->pchar->ppPowerSets[i]->psetBase, colorChangeOccurred );
                        }
                    }
                }
            }
        }
    }
    return total;
}
int powerCust_GetFee(Entity *e)
{
    PowerCustomizationCost *current = powerCustCost_getCurrent(e);
    PowerCustomizationCost *next = powerCustCost_getNext(e);
    float lvlScale = 0.0f;
    assert(current);
    assert(next);
    if (next == current)
    {
        lvlScale = 0.0f;
    }
    else
    {
        lvlScale = (float)((e->pchar->iLevel+1)-current->min_level)/(next->min_level-current->min_level);
    }
    return current->entry_fee + ((next->entry_fee-current->entry_fee)*lvlScale);
}
int powerCust_GetTotalCost( Entity *e, PowerCustomizationList *oldList, PowerCustomizationList *newList )
{
    int colorChangeOccurred = 0;
    int total = powerCust_GetChangeCost(e, oldList, newList, &colorChangeOccurred);
    if (total || colorChangeOccurred)
    {
        total += powerCust_GetFee(e);
    }
    return total;
}

void powerCustList_allocPowerCustomizationList(PowerCustomizationList *powerCustList, int numPowersToAlloc)
{
    if (!powerCustList)
    {
        assertmsg(0, "power customization list was not allocated");
        return;
    }
    if (eaSize(&powerCustList->powerCustomizations) < numPowersToAlloc)
    {
        int i;
        for (i = eaSize(&powerCustList->powerCustomizations); i < numPowersToAlloc; ++i)
        {
            eaPush(&powerCustList->powerCustomizations, StructAllocRaw(sizeof(PowerCustomization)));
            StructInitFields(ParsePowerCustomization, powerCustList->powerCustomizations[i]);
        }
    }
}

static int colorIsInPalette(const ColorPalette *palette, const Color *color)
{
    int i;
    Color c1 = colorFlip(*color);
    for (i = 0; i < eaSize((F32***)&palette->color); ++i)
    {
        if (c1.integer == getColorFromVec3( *(palette->color[i])))
        {
            return 1;
        }
    }
    return 0;
}
int powerCust_validate(PowerCustomization *customization, Entity *e, int fixup)
{
    const BasePower *power = customization->power;
    if (power)
    {
        const ColorPalette *palette = NULL;
        if (!isNullOrNone(customization->token))
        {
            char *paletteName = NULL;
            int i;
            for (i = 0; i < eaSize(&power->customfx); ++i)
            {
                if (stricmp(customization->token, power->customfx[i]->pchToken) == 0)
                {
                    paletteName = power->customfx[i]->paletteName ? power->customfx[i]->paletteName : "CustomPowerDefault";
                    break;
                }
            }
            if (paletteName)
            {
                int bodyType = 0;
                int origin = 0;
                const CostumeOriginSet *oset;

                getCurrentBodyTypeAndOrigin(e, costume_current(e), &bodyType, &origin);
                oset = gCostumeMaster.bodySet[bodyType]->originSet[origin];
                if (stricmp(paletteName, "CustomPowerDefault") == 0)
                {
                    palette = oset->powerColors[0];
                }
                else
                {
                    for (i = 0; i < eaSize(&oset->powerColors); ++i)
                    {
                        if (stricmp(paletteName, oset->powerColors[i]->name) == 0)
                        {
                            palette = oset->powerColors[i];
                            break;
                        }
                    }
                }
            }
            else if (fixup)
            {
                //    invalid
                customization->token = NULL;
            }
            else
            {
                return 0;
            }
        }
        else
        {
            //    its okiejay
            //    any color is fine because we aren't using any tokens
        }
        if (palette)
        {
            if (colorIsInPalette(palette, &(customization->customTint.primary)) && colorIsInPalette(palette, &(customization->customTint.secondary)))
            {
                //    the power customization is good
            }
            else if (fixup)
            {
                customization->customTint.primary.integer = getColorFromVec3( *(palette->color[0]) );
                customization->customTint.secondary.integer = getColorFromVec3( *(palette->color[0]) );
            }
            else
            {
                return 0;
            }
        }
    }
    else
    {
        //    invalid
        return 0;
    }
    return 1;
}
int powerCustList_validate(PowerCustomizationList *powerCustList, Entity *e, int fixup)
{
    int i;
    for (i = 0; i < eaSize(&powerCustList->powerCustomizations); ++i)
    {
        PowerCustomization *customization = powerCustList->powerCustomizations[i];
        if (!powerCust_validate(customization, e, fixup))
        {
            return 0;
        }
    }
    return 1;
}
void initializePowerCustomizationPower(DBPowerCustomization *dbPower, PowerCustomization *power)
{
    if (!isNullOrNone(dbPower->powerCatName) && !isNullOrNone(dbPower->powerSetName) && !isNullOrNone(dbPower->powerName))
    {
        const BasePower *p_basePower = NULL;
        if (p_basePower = powerdict_GetBasePowerByName(&g_PowerDictionary, (char*)dbPower->powerCatName, (char*)dbPower->powerSetName, (char*)dbPower->powerName))
        {
            power->power = p_basePower;
            power->powerFullName = p_basePower->pchFullName;
        }
    }
}

void powerCustList_destroy( PowerCustomizationList *powerCustList)
{
    if (powerCustList)    
        StructDestroy(ParsePowerCustomizationList, powerCustList);
}
PowerCustomizationList *powerCustList_new_slot( Entity *e)
{
    PowerCustomizationList* dst = NULL;

    if(devassert(e && e->pl))
    {
        dst = StructAllocRaw(sizeof(PowerCustomizationList));
        StructCopy(ParsePowerCustomizationList, e->pl->powerCustomizationLists[0], dst, 0,0);
    }

    return dst;
}

static void powerCust_handleCurrentEmptyPowerCust(Entity *e)
{
    assert(e);
    assertmsg(e->powerCust, "Entity does not have power customization list.");
    if (e && e->powerCust)
    {
        int i;
        int emptyPowerCustomizationList = 1;
        for (i = 0; i < eaSize(&e->powerCust->powerCustomizations); ++i)
        {
            if (e->powerCust->powerCustomizations[i]->power)
            {
                emptyPowerCustomizationList = 0;
                break;
            }
        }
        if (emptyPowerCustomizationList)
        {
            powerCust_syncPowerCustomizations(e);
        }
    }
}
void powerCust_handleAllEmptyPowerCust(Entity *e)
{
    int i;
    assert(e);
    assert(e->pl);
    if (e && e->pl)
    {
        PowerCustomizationList *temp = e->powerCust;
        for (i = 0; i < MIN(MAX_COSTUMES, costume_get_num_slots(e)); ++i)
        {
            e->powerCust = e->pl->powerCustomizationLists[i];
            powerCust_handleCurrentEmptyPowerCust(e);
        }
        e->powerCust = temp;
    }
}
