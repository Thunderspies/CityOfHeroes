#include "entity/power_customization.h"
#include "cmdparse/cmdgame.h"
#include "UI/uiNet.h"
#include "entity/powers.h"
#include "entity/classes.h"
#include "player/player.h"
#include "entity/entity.h"
#include <utilitieslib/components/earray.h>
#include "entity/character_base.h"
#include <utilitieslib/components/StashTable.h>
#include <utilitieslib/utils/fileutil.h>

StashTable g_hashPowerCustRewards;
extern StashTable powerCustReward_HashTable;
static Color parseColor(char* asHex)
{
    int resultInt = 0;
    Color result;

    while (*asHex)
    {
        resultInt <<= 4;

        if (*asHex >= '0' && *asHex <= '9')
            resultInt += *asHex - '0';
        else if (*asHex >= 'a' && *asHex <= 'f')
            resultInt += *asHex - 'a' + 10;
        else if (*asHex >= 'A' && *asHex <= 'F')
            resultInt += *asHex - 'A' + 10;
        ++asHex;
    }

    resultInt <<= 8;
    resultInt += 0xff;
    result.integer = resultInt;
    return colorFlip(result);
}
void testPowerCust(int themeIndex, char* color1, char* color2)
{
    Category category;
    Entity* e = playerPtr();
    int i;

    for (i = (eaSize(&e->powerCust->powerCustomizations)-1); i >= 0; --i)
    {
        StructDestroy(ParsePowerCustomization, e->powerCust->powerCustomizations[i]);
        eaRemove(&e->powerCust->powerCustomizations, i);
    }

    for (category = kCategory_Primary; category <= kCategory_Secondary; ++category)
    {
        PowerSet** powerSet = e->pchar->ppPowerSets;
        int numPowerSets = eaSize(&e->pchar->ppPowerSets);
        for (powerSet = e->pchar->ppPowerSets; powerSet < e->pchar->ppPowerSets + numPowerSets; ++powerSet)
        {
            const BasePower** power;

            if ((**powerSet).psetBase->pcatParent != e->pchar->pclass->pcat[category])
                continue;

            for (power = (**powerSet).psetBase->ppPowers; 
                power < (**powerSet).psetBase->ppPowers + eaSize(&(**powerSet).psetBase->ppPowers); 
                ++power)
            {
                char buffer[256];
                PowerCustomization* cust;
                sprintf(buffer, "powers_buypower_dev %s %s %s", (**power).psetParent->pcatParent->pchName, (**power).psetParent->pchName, (**power).pchName);
                cmdParse(buffer);

                cust = StructAllocRaw(sizeof(PowerCustomization));
                StructInitFields(ParsePowerCustomization, cust);
                cust->power = (*power);
                cust->powerFullName = (**power).pchFullName;

                if (themeIndex > 0 && themeIndex <= eaSize(&(**power).customfx))
                    cust->token = (**power).customfx[themeIndex - 1]->pchToken;

                cust->customTint.primary = parseColor(color1);
                cust->customTint.secondary = parseColor(color2);
                eaSortedInsert(&e->powerCust->powerCustomizations, comparePowerCustomizations, cust);
            }
        }
    }

    sendTailoredPowerCust(e->powerCust);
}


void powerCustreward_add( char * name )
{
    StashTable *rewardTable;
    rewardTable = &g_hashPowerCustRewards;
    if( !(*rewardTable) )
        (*rewardTable) = stashTableCreateWithStringKeys(20, StashDeepCopyKeys);

    stashAddPointer((*rewardTable), name, 0, false);
}

void powerCustreward_remove( char * name )
{
    StashTable *rewardTable;
    rewardTable = &g_hashPowerCustRewards;

    if((*rewardTable))
        stashRemovePointer((*rewardTable), name, NULL);
}

void powerCustreward_clear()
{
    StashTable *rewardTable;
    rewardTable = &g_hashPowerCustRewards;

    if ((*rewardTable)) stashTableClear((*rewardTable));
}

bool powerCustreward_find( char * name )
{
    StashTable *rewardTable;
    rewardTable = &g_hashPowerCustRewards;

    if( game_state.editnpc ) // show everything in editnpc mode
        return true;

    if(!name) // unkeyed
        return true;

    if( !(*rewardTable) )
        return false;

    return stashFindElement((*rewardTable), name, NULL);
}

bool powerCustreward_findall(char **keys)
{
    int i;
    for(i = eaSize(&keys)-1; i >= 0; --i)
        if(!powerCustreward_find(keys[i]))
            return false;
    return true;
}

#if 0 // not used
static void reloadPowerCust()
{
    stashTableDestroy(powerCustReward_HashTable);
    powerCustReward_HashTable = NULL;
    powerCust_initHashTable();
}
void reloadPowerCustCallback(const char *relpath, int when)
{
    int enpcsave=0;
    fileWaitForExclusiveAccess(relpath);
    errorLogFileIsBeingReloaded(relpath);
    enpcsave = game_state.editnpc;
    game_state.editnpc = 1;
    reloadPowerCust();
    game_state.editnpc = enpcsave;
}
#endif