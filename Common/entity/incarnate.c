#include "entity/incarnate.h"
#include "player/badges.h"
#include "entity/entPlayer.h"
#include "entity/character_base.h"
#include <utilitieslib/components/earray.h>
#include <utilitieslib/language/MessageStoreUtil.h>
#include <utilitieslib/utils/structDefines.h>
#include <utilitieslib/components/EString.h>
#if SERVER
#include "player/badges_server.h"
#endif

StaticDefineInt ParseIncarnateSlotNames[] = {
    DEFINE_INT
    {    "Alpha",    kIncarnateSlot_Alpha        },
    {    "Destiny",    kIncarnateSlot_Destiny        },
    {    "Genesis",    kIncarnateSlot_Genesis        },
    {    "Hybrid",    kIncarnateSlot_Hybrid        },
    {    "Interface",kIncarnateSlot_Interface    },
    {    "Judgement",kIncarnateSlot_Judgement    },
    {    "Lore",        kIncarnateSlot_Lore            },
    {    "Mind",        kIncarnateSlot_Mind            },
    {    "Vitae",    kIncarnateSlot_Vitae        },
    {    "Omega",    kIncarnateSlot_Omega        },
    DEFINE_END
};

IncarnateSlot IncarnateSlot_getByInternalName(const char *name)
{
    int index;
    for (index = 0; index < kIncarnateSlot_Count; index++)
    {
        if (!stricmp(name, StaticDefineIntRevLookup(ParseIncarnateSlotNames,index)))
        {
            break;
        }
    }

    return index; // kIncarnateSlot_Count if you couldn't find it
}

float IncarnateSlot_getActivatePercent(Entity *e, IncarnateSlot slot)
{
    float result = 0.0f;

    if (slot < kIncarnateSlot_Count)
    {
        const BadgeDef *badge = badge_GetBadgeByName(incarnateEnableBadgeNames[slot]);
        if (badge)
        {
            result = BADGE_COMPLETION(e->pl->aiBadges[badge->iIdx]) / 10000.f;
        }
    }

    return result;
}

bool IncarnateSlot_isActivated(Entity *e, IncarnateSlot slot)
{
    bool result = false;

    if (slot < kIncarnateSlot_Count)
    {
#if SERVER
        result = badge_OwnsBadge(e, incarnateEnableBadgeNames[slot]);
#else
        const BadgeDef *badge = badge_GetBadgeByName(incarnateEnableBadgeNames[slot]);
        if (e && e->pl && badge)
        {
            result = BADGE_IS_OWNED(e->pl->aiBadges[badge->iIdx]) != 0; // (bool) 0x80000000 == 0x00
        }
#endif
    }

    return result;
}

bool IsEntityIncarnate(Entity* e)
{
    return IncarnateSlot_isActivated(e, kIncarnateSlot_Alpha);
}

const BasePowerSet* IncarnateSlot_getBasePowerSet(IncarnateSlot slot)
{
    if (slot < kIncarnateSlot_Count)
    {
        return powerdict_GetBasePowerSetByName(&g_PowerDictionary, "Incarnate", StaticDefineIntRevLookup(ParseIncarnateSlotNames,slot));
    }
    else
    {
        return NULL;
    }
}

const BasePower* Incarnate_getBasePower(IncarnateSlot slot, const char *name)
{
    const BasePower *returnMe = NULL;
    const BasePowerSet *powerSet;
    int powerIndex, powerCount;

    powerSet = IncarnateSlot_getBasePowerSet(slot);
    
    if (name && powerSet)
    {
        // Check display name first
        powerCount = eaSize(&powerSet->ppPowers);
        for (powerIndex = 0; powerIndex < powerCount; powerIndex++)
        {
            const BasePower *power = powerSet->ppPowers[powerIndex];
            if (power && !stricmp(name, textStd(power->pchDisplayName)))
            {
                returnMe = power;
            }
        }

        // Check internal name second
        if (!returnMe && slot < kIncarnateSlot_Count)
        {
            returnMe = powerdict_GetBasePowerByName(&g_PowerDictionary, "Incarnate", StaticDefineIntRevLookup(ParseIncarnateSlotNames,slot), name);
        }
    }

    return returnMe;
}
Power* Incarnate_getPowerByBasePower(Entity *e, const BasePower *basePower)
{
    return (e && e->pchar && basePower) ? character_OwnsPower(e->pchar, basePower) : NULL;
}
Power* Incarnate_getPower(Entity *e, IncarnateSlot slot, const char *name)
{
    Power *returnMe = NULL;

    if (e && e->pchar && name && slot < kIncarnateSlot_Count)
    {
        const BasePower *basePower = Incarnate_getBasePower(slot, name);
        if (basePower)
        {
            return Incarnate_getPowerByBasePower(e, basePower);
        }
    }

    return returnMe;
}
bool Incarnate_hasInInventoryByPower(Entity *e, const BasePower *incarnateBasePower)
{
    return e && e->pchar && incarnateBasePower && character_OwnsPower(e->pchar, incarnateBasePower);
}
bool Incarnate_hasInInventory(Entity *e, IncarnateSlot slot, const char *name)
{
    const BasePower *incarnateBasePower = Incarnate_getBasePower(slot, name);

    return Incarnate_hasInInventoryByPower(e, incarnateBasePower);
}
bool Incarnate_isEquippedByBasePower(Entity *e, const BasePower *pPow)
{
    if (!Incarnate_hasInInventoryByPower(e, pPow))
    {
        return false;
    }
    else
    {
        Power *incarnatePower = Incarnate_getPowerByBasePower(e, pPow);

        return incarnatePower && !incarnatePower->bDisabledManually;
    }
}
bool Incarnate_isEquipped(Entity *e, IncarnateSlot slot, const char *name)
{
    if (!IncarnateSlot_isActivated(e, slot))
    {
        return false;
    }
    else
    {
        const BasePowerSet* pSet = IncarnateSlot_getBasePowerSet(slot);
        if (pSet && name)
        {
            char *fullName = NULL;
            const BasePower *pBasePower;
            estrConcatf(&fullName, "%s.%s", pSet->pchFullName, name);
            pBasePower = powerdict_GetBasePowerByFullName(&g_PowerDictionary, fullName);
            estrDestroy(&fullName);
            if (pBasePower)
            {
                return Incarnate_isEquippedByBasePower(e, pBasePower);
            }
        }
        return false;
    }

}

Power* Incarnate_getEquipped(Entity *e, IncarnateSlot slot)
{
    const BasePowerSet *incarnateBaseSet;
    int powerIndex;

    if (!IncarnateSlot_isActivated(e, slot))
    {
        return NULL;
    }

    incarnateBaseSet = IncarnateSlot_getBasePowerSet(slot);

    if (!incarnateBaseSet || !character_OwnsPowerSet(e->pchar, incarnateBaseSet))
    {
        return NULL;
    }

    for (powerIndex = 0; powerIndex < eaSize(&incarnateBaseSet->ppPowers); powerIndex++)
    {
        const BasePower *incarnateBasePower = incarnateBaseSet->ppPowers[powerIndex];
        Power *incarnatePower;

        if (!incarnateBasePower || !(incarnatePower = character_OwnsPower(e->pchar, incarnateBasePower)))
        {
            continue;
        }

        if (!incarnatePower->bDisabledManually)
        {
            return incarnatePower;
        }
    }

    return NULL;
}