#include "incarnate_server.h"
#include "player/badges_server.h"
#include "player/badges.h"
#include "entity/powers.h"
#include "entity/origins.h"
#include "entity/character_base.h"
#include "character_combat.h"
#include "container/team.h"
#include <utilitieslib/utils/timing.h>

#include <utilitieslib/components/earray.h>
#include "gameComm/svr_chat.h"
#include "gameComm/sendToClient.h"
#include "dbcomm/dbcomm.h"
#include <utilitieslib/components/EString.h>
#include <utilitieslib/utils/structDefines.h>

SHARED_MEMORY IncarnateMods g_IncarnateMods;

static char *incarnateSlotSubsetNames[] =
{
    "Alpha_Silent",
    "Destiny_Silent",
    "Genesis_Silent",
    "Hybrid_Silent",
    "Interface_Silent",
    "Judgement_Silent",
    "Lore_Silent",
    "Mind_Silent",
    "Vitae_Silent",
    "Omega_Silent"
};

// NOTE: This function assumes that all slots only require one badgestat (except for the last one in the row, I think it'll still accidentally work)
void Incarnate_RewardPoints(Entity *e, int subtype, int pointCount)
{
    int j;
    int pointsRemaining = pointCount;
    int incarnateType = 0;
    IncarnateExpInfo *expInfo;

    if (!e || !pointCount || subtype < 0 || subtype >= eaSize(&g_IncarnateMods.incarnateExpList))
        return;

    expInfo = g_IncarnateMods.incarnateExpList[subtype];

    for (j = 0; j < eaSize(&expInfo->incarnateSlotInfo); ++j)
    {
        IncarnateSlotInfo *slotInfo = expInfo->incarnateSlotInfo[j];
        IncarnateSlot slot = slotInfo->slotUnlocked;

        if (!IncarnateSlot_isActivated(e, slot))
        {
            // You've reached an unactivated slot, apply the points here.
            float multiplier;
            int currentStat = badge_StatGet(e, slotInfo->badgeStat);
            const BadgeDef *badge = badge_GetBadgeByName(incarnateEnableBadgeNames[slot]);
            int maxStat = badge->iProgressMaxValue;
            int badgeStatRemaining;
            int origin = StaticDefineIntGetInt(OriginEnum, e->pchar->porigin->pchName);
            assertmsg(origin != -1, "Bad origin in Incarnate_RewardPoints()!");
            if (origin != -1)
            {
                multiplier = g_IncarnateMods.incarnateModifierList[origin].incarnateExpModifier[slot];
            }

            badgeStatRemaining = (int) ((float) pointsRemaining * multiplier);

            if (currentStat + badgeStatRemaining > maxStat)
            {
                pointsRemaining = (int) ((currentStat + badgeStatRemaining - maxStat) / multiplier);
                badge_StatSet(e, slotInfo->badgeStat, maxStat);
                sendInfoBox(e, INFO_REWARD, slotInfo->clientMessage, maxStat - currentStat, (multiplier - 1.0f) / 100.0f);
            }
            else
            {
                // If we arrived here we already OK'd the reward even if in Architect mode, so don't suppress it here
                badge_StatAddArchitectAllowed(e, slotInfo->badgeStat, badgeStatRemaining, 1);
                sendInfoBox(e, INFO_REWARD, slotInfo->clientMessage, badgeStatRemaining, (multiplier - 1.0f) / 100.0f);
                break;
            }
        }
    }
}

///////////////////////////////////////////////////////////
// Incarnate Slots
///////////////////////////////////////////////////////////

#define INCARNATE_RESLOT_TIMER 300.0f // in seconds

// Only returns powers in player-facing slot powersets, not internal support sets!
IncarnateSlot Incarnate_slotFromPower(const BasePower *ppow)
{
    const BasePowerSet *pset = ppow->psetParent;
    IncarnateSlot returnMe = kIncarnateSlot_Count;
    IncarnateSlot slotIter;

    if (!stricmp(pset->pcatParent->pchName, "Incarnate"))
    {
        for (slotIter = 0; slotIter < kIncarnateSlot_Count; slotIter++)
        {
            if (!stricmp(pset->pchName, StaticDefineIntRevLookup(ParseIncarnateSlotNames,slotIter)))
            {
                returnMe = slotIter;
                break;
            }
        }
    }

    return returnMe;
}

static int s_powerIsActivating(Power *currentPower, IncarnateSlot slot)
{
    if (currentPower && currentPower->psetParent && currentPower->psetParent->psetBase && currentPower->psetParent->psetBase->pchName)
    {
        if (!stricmp(currentPower->psetParent->psetBase->pchName, StaticDefineIntRevLookup(ParseIncarnateSlotNames, slot)) ||
            !stricmp(currentPower->psetParent->psetBase->pchName, incarnateSlotSubsetNames[slot]))
        {
            return true;
        }
    }
    return false;
}
static bool s_IncarnateSlot_PowerIsActivating(Entity *e, IncarnateSlot slot)
{
    if (e && e->pchar)
    {
        if (s_powerIsActivating(e->pchar->ppowCurrent, slot))
        {
            return true;
        }

        if (s_powerIsActivating(e->pchar->ppowQueued, slot))
        {
            return true;
        }

        if (s_powerIsActivating(e->pchar->ppowTried, slot))
        {
            return true;
        }

    }
    return false;
}

bool IncarnateSlot_isRecharging(Entity *e, IncarnateSlot slot)
{
    PowerListItem *ppowListItem;
    PowerListIter iter;

    if (slot >= kIncarnateSlot_Count)
    {
        return false;
    }

    // Recharge powers recently used.
    for(ppowListItem = powlist_GetFirst(&e->pchar->listRechargingPowers, &iter);
        ppowListItem != NULL;
        ppowListItem = powlist_GetNext(&iter))
    {
        if (!stricmp(ppowListItem->ppow->psetParent->psetBase->pcatParent->pchName, "Incarnate") &&
            (!stricmp(ppowListItem->ppow->psetParent->psetBase->pchName, StaticDefineIntRevLookup(ParseIncarnateSlotNames,slot)) ||
             !stricmp(ppowListItem->ppow->psetParent->psetBase->pchName, incarnateSlotSubsetNames[slot])))
        {
            return true;
        }
    }

    if (s_IncarnateSlot_PowerIsActivating(e, slot))
    {
        return true;    //    effectively the same thing as recharging
    }

    return false;
}

// This should probably go into generic character code?
bool Incarnate_characterIsInCombat(Entity *e)
{
    int attackedTime = e->pchar->aulEventTimes[kPowerEvent_AttackedByOther];
    int attackTime = e->pchar->aulEventTimes[kPowerEvent_ActivateAttackClick];
    int now = timerSecondsSince2000();

    return (now - attackedTime < 15) || (now - attackTime < 15);
}

// This should probably go into generic character code?
bool Incarnate_charactersTeamIsInCombat(Entity *e)
{
    
    // Find all team members
    if (e->teamup_id)
    {
        TeamMembers *ptm = teamMembers(e, CONTAINER_TEAMUPS);

        if(ptm)
        {
            int i;

            for (i = 0; i < ptm->count; i++)
            {
                Entity *eMember;
                if (ptm->ids[i] != e->db_id && NULL != (eMember = entFromDbId(ptm->ids[i])) &&
                    Incarnate_characterIsInCombat(eMember))
                {
                    return true;
                }
            }
        }
    }

    return Incarnate_characterIsInCombat(e);
}

int IncarnateSlot_activate(Entity *e, IncarnateSlot slot)
{
    int errorValue = 1;

    if (slot < kIncarnateSlot_Count)
    {
        badge_Award(e, incarnateEnableBadgeNames[slot], false);
        errorValue = 0;
    }

    return errorValue;
}

void IncarnateSlot_activateAll(Entity *e)
{
    int index;

    for (index = 0; index < kIncarnateSlot_Count; index++)
    {
        badge_Award(e, incarnateEnableBadgeNames[index], false);
    }
}

int IncarnateSlot_deactivate(Entity *e, IncarnateSlot slot)
{
    int errorValue = 1;

    if (slot < kIncarnateSlot_Count)
    {
        badge_Revoke(e, incarnateEnableBadgeNames[slot]);
        errorValue = 0;
    }

    return errorValue;
}

void IncarnateSlot_deactivateAll(Entity *e)
{
    int index;

    for (index = 0; index < kIncarnateSlot_Count; index++)
    {
        badge_Revoke(e, incarnateEnableBadgeNames[index]);
    }
}

void IncarnateSlot_debugPrint(Entity *e, ClientLink *client, IncarnateSlot slot)
{
    if (slot < kIncarnateSlot_Count)
    {
        conPrintf(client, "Slot %s is %s.", StaticDefineIntRevLookup(ParseIncarnateSlotNames,slot), IncarnateSlot_isActivated(e, slot) ? "enabled" : "disabled");
    }
}

void IncarnateSlot_debugPrintAll(Entity *e, ClientLink *client)
{
    int index;

    for (index = 0; index < kIncarnateSlot_Count; index++)
    {
        IncarnateSlot_debugPrint(e, client, index);
    }
}

///////////////////////////////////////////////////////////
// Incarnate Abilities
///////////////////////////////////////////////////////////

int Incarnate_grant(Entity *e, IncarnateSlot slot, char *name)
{
    int errorValue = 1;

    const BasePower *incarnateBasePower = Incarnate_getBasePower(slot, name);
    Power *incarnatePower = 0;
    const BasePowerSet *incarnateBaseSet = 0;
    PowerSet *incarnateSet = 0;

    if (!incarnateBasePower || character_OwnsPower(e->pchar, incarnateBasePower))
    {
        return errorValue;
    }

    incarnateBaseSet = IncarnateSlot_getBasePowerSet(slot);

    if (!(incarnateSet = character_OwnsPowerSet(e->pchar, incarnateBaseSet)))
    {
        incarnateSet = character_BuyPowerSet(e->pchar, incarnateBaseSet);
    }

    if (incarnateSet)
    {
        incarnatePower = character_BuyPower(e->pchar, incarnateSet, incarnateBasePower, 0);
    }

    if (incarnatePower)
    {
        // This forces a full redefine to be sent.
        eaPush(&e->powerDefChange, (void *)-1);

        character_ManuallyDisablePower(e->pchar, incarnatePower);

        errorValue = 0;
    }

    return errorValue;
}

int Incarnate_revoke(Entity *e, IncarnateSlot slot, char *name)
{
    const BasePower *incarnateBasePower = Incarnate_getBasePower(slot, name);

    if (!incarnateBasePower || !character_OwnsPower(e->pchar, incarnateBasePower))
    {
        return 1;
    }

    character_RemoveBasePower(e->pchar, incarnateBasePower);
    return 0;
}

void Incarnate_grantAll(Entity *e)
{
    IncarnateSlot slotIndex;

    for (slotIndex = 0; slotIndex < kIncarnateSlot_Count; slotIndex++)
    {
        Incarnate_grantAllBySlot(e, slotIndex);
    }
}

void Incarnate_revokeAll(Entity *e)
{
    IncarnateSlot slotIndex;

    for (slotIndex = 0; slotIndex < kIncarnateSlot_Count; slotIndex++)
    {
        Incarnate_revokeAllBySlot(e, slotIndex);
    }
}

void Incarnate_grantAllBySlot(Entity *e, IncarnateSlot slot)
{
    const BasePowerSet *incarnateBaseSet = IncarnateSlot_getBasePowerSet(slot);
    PowerSet *incarnateSet;
    int powerIndex;

    if (!incarnateBaseSet)
    {
        return;
    }

    if (!(incarnateSet = character_OwnsPowerSet(e->pchar, incarnateBaseSet)))
    {
        incarnateSet = character_BuyPowerSet(e->pchar, incarnateBaseSet);
    }

    if (!incarnateSet)
    {
        return;
    }

    for (powerIndex = 0; powerIndex < eaSize(&incarnateBaseSet->ppPowers); powerIndex++)
    {
        const BasePower *incarnateBasePower = incarnateBaseSet->ppPowers[powerIndex];
        Power *incarnatePower;

        if (!incarnateBasePower || character_OwnsPower(e->pchar, incarnateBasePower))
        {
            continue;
        }

        incarnatePower = character_BuyPower(e->pchar, incarnateSet, incarnateBasePower, 0);

        if (incarnatePower)
        {
            character_ManuallyDisablePower(e->pchar, incarnatePower);
        }
    }

    // This forces a full redefine to be sent.
    eaPush(&e->powerDefChange, (void *)-1);
}

void Incarnate_revokeAllBySlot(Entity *e, IncarnateSlot slot)
{
    const BasePowerSet *incarnateBaseSet = IncarnateSlot_getBasePowerSet(slot);
    int powerIndex;

    if (!incarnateBaseSet || !character_OwnsPowerSet(e->pchar, incarnateBaseSet))
    {
        return;
    }

    for (powerIndex = 0; powerIndex < eaSize(&incarnateBaseSet->ppPowers); powerIndex++)
    {
        const BasePower *incarnateBasePower = incarnateBaseSet->ppPowers[powerIndex];

        if (!incarnateBasePower || !character_OwnsPower(e->pchar, incarnateBasePower))
        {
            continue;
        }

        character_RemoveBasePower(e->pchar, incarnateBasePower);
    }
}

IncarnateEquipResult Incarnate_equip(Entity *e, IncarnateSlot slot, char *name)
{
    int time = dbSecondsSince2000();

    if (slot >= kIncarnateSlot_Count)
    {
        return kIncarnateEquipResult_Failure;
    }

    if (Incarnate_charactersTeamIsInCombat(e))
    {
        return kIncarnateEquipResult_FailureInCombat;
    }

    if (IncarnateSlot_isRecharging(e, slot))
    {
        return kIncarnateEquipResult_FailurePowerRecharging;
    }

    if (e->pchar->incarnateSlotTimesSlottable[slot] > time)
    {
        return kIncarnateEquipResult_FailureSlotCooldown;
    }

    return Incarnate_equipForced(e, slot, name);
}

IncarnateEquipResult Incarnate_equipForced(Entity *e, IncarnateSlot slot, char *name)
{
    Power *incarnatePower;
    int time;

    if (!e || !e->pchar || !name
        || !IncarnateSlot_isActivated(e, slot)
        || !Incarnate_hasInInventory(e, slot, name)
        || Incarnate_isEquipped(e, slot, name))
    {
        return kIncarnateEquipResult_Failure;
    }

    time = dbSecondsSince2000();

    Incarnate_unequipBySlotForced(e, slot);
    e->pchar->incarnateSlotTimesSlottable[slot] = time + INCARNATE_RESLOT_TIMER;
    incarnatePower = Incarnate_getPower(e, slot, name);

    if (!incarnatePower)
    {
        return kIncarnateEquipResult_Failure;
    }

    character_ManuallyEnablePower(e->pchar, incarnatePower);

    if (incarnatePower->ppowBase->eType == kPowerType_Auto || incarnatePower->ppowBase->eType == kPowerType_GlobalBoost)
    {
        character_ActivateAllAutoPowers(e->pchar);
    }

    return kIncarnateEquipResult_Success;
}

IncarnateEquipResult Incarnate_unequip(Entity *e, IncarnateSlot slot, char *name)
{
    int time = dbSecondsSince2000();

    if (Incarnate_charactersTeamIsInCombat(e))
    {
        return kIncarnateEquipResult_FailureInCombat;
    }

    if (IncarnateSlot_isRecharging(e, slot))
    {
        return kIncarnateEquipResult_FailurePowerRecharging;
    }

    if (e->pchar->incarnateSlotTimesSlottable[slot] > time)
    {
        return kIncarnateEquipResult_FailureSlotCooldown;
    }

    return Incarnate_unequipForced(e, slot, name);
}

IncarnateEquipResult Incarnate_unequipForced(Entity *e, IncarnateSlot slot, char *name)
{
    Power *incarnatePower;

    if (!e || !e->pchar || !name
        || !IncarnateSlot_isActivated(e, slot)
        || !Incarnate_hasInInventory(e, slot, name)
        || !Incarnate_isEquipped(e, slot, name))
    {
        return kIncarnateEquipResult_Failure;
    }

    incarnatePower = Incarnate_getPower(e, slot, name);

    if (!incarnatePower)
    {
        return kIncarnateEquipResult_Failure;
    }

    character_ManuallyDisablePower(e->pchar, incarnatePower);
    return kIncarnateEquipResult_Success;
}

IncarnateEquipResult Incarnate_unequipBySlot(Entity *e, IncarnateSlot slot)
{    
    int time = dbSecondsSince2000();

    if (Incarnate_charactersTeamIsInCombat(e))
    {
        return kIncarnateEquipResult_FailureInCombat;
    }

    if (IncarnateSlot_isRecharging(e, slot))
    {
        return kIncarnateEquipResult_FailurePowerRecharging;
    }

    if (e->pchar->incarnateSlotTimesSlottable[slot] > time)
    {
        return kIncarnateEquipResult_FailureSlotCooldown;
    }

    return Incarnate_unequipBySlotForced(e, slot);
}

IncarnateEquipResult Incarnate_unequipBySlotForced(Entity *e, IncarnateSlot slot)
{
    const BasePowerSet *incarnateBaseSet = IncarnateSlot_getBasePowerSet(slot);
    int powerIndex;

    if (!incarnateBaseSet || !character_OwnsPowerSet(e->pchar, incarnateBaseSet) ||
        !IncarnateSlot_isActivated(e, slot))
    {
        return kIncarnateEquipResult_Failure;
    }

    for (powerIndex = 0; powerIndex < eaSize(&incarnateBaseSet->ppPowers); powerIndex++)
    {
        const BasePower *incarnateBasePower = incarnateBaseSet->ppPowers[powerIndex];
        Power *incarnatePower;

        if (!incarnateBasePower || !(incarnatePower = character_OwnsPower(e->pchar, incarnateBasePower)))
        {
            continue;
        }

        character_ManuallyDisablePower(e->pchar, incarnatePower);
    }

    return kIncarnateEquipResult_Success;
}

void Incarnate_unequipAll(Entity *e)
{
    int slotIndex;

    for (slotIndex = 0; slotIndex < kIncarnateSlot_Count; slotIndex++)
    {
        Incarnate_unequipBySlot(e, slotIndex);
    }
}

#define MAX_NEGATIVE_COMBAT_MOD_SHIFT -50
#define MAX_POSITIVE_COMBAT_MOD_SHIFT 50

void SetCombatModShift(Entity *e, int levelShift)
{
    int index;
    static char *powerName;
    char levelNumber[10];
    const BasePower *incarnateBasePower;

    for (index = MAX_NEGATIVE_COMBAT_MOD_SHIFT; index <= MAX_POSITIVE_COMBAT_MOD_SHIFT; index++)
    {
        if (index == 0) // no power for level shift of zero
            continue;

        estrClear(&powerName);
        estrConcatStaticCharArray(&powerName, "LevelShift");
        if (index < 0)
        {
            estrConcatStaticCharArray(&powerName, "Minus");
        }
        if (index > -10 && index < 10)
        {
            estrConcatChar(&powerName, '0');
        }
        itoa(abs(index), levelNumber, 10);
        estrConcatCharString(&powerName, levelNumber);
        
        incarnateBasePower = powerdict_GetBasePowerByName(&g_PowerDictionary, "Temporary_Powers", "Level_Shift", powerName);

        if (incarnateBasePower)
        {
            character_RemoveBasePower(e->pchar, incarnateBasePower);
        }
    }

    if (levelShift != 0 && levelShift >= MAX_NEGATIVE_COMBAT_MOD_SHIFT 
        && levelShift <= MAX_POSITIVE_COMBAT_MOD_SHIFT)
    {
        estrClear(&powerName);
        estrConcatStaticCharArray(&powerName, "LevelShift");
        if (levelShift < 0)
        {
            estrConcatStaticCharArray(&powerName, "Minus");
        }
        if (levelShift > -10 && levelShift < 10)
        {
            estrConcatChar(&powerName, '0');
        }
        itoa(abs(levelShift), levelNumber, 10);
        estrConcatCharString(&powerName, levelNumber);

        incarnateBasePower = powerdict_GetBasePowerByName(&g_PowerDictionary, "Temporary_Powers", "Level_Shift", powerName);

        if (incarnateBasePower)
        {
            character_AddRewardPower(e->pchar, incarnateBasePower);
        }
    }
}
