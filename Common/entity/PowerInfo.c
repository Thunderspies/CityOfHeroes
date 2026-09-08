/***************************************************************************
 *     Copyright (c) 2000-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include <utilitieslib/assert/assert.h>

#include <utilitieslib/components/earray.h>
#include <utilitieslib/network/netio.h>
#include <utilitieslib/utils/timing.h>

#include "comm_game.h"

#include "entity/character_base.h"
#include "entity/character_net.h"
#include "entity/character_level.h"
#include "entity/powers.h"
#include "entity/PowerInfo.h"

#if CLIENT
#include "UI/uiInspiration.h"
#include "bases/DetailRecipe.h"
#include "UI/uiRecipeInventory.h"
#include "UI/uiIncarnate.h"
#include "UI/uiTray.h"
#include "player/player.h"
#include "entity/entity.h"
#include "entity/entPlayer.h"
#endif
#if SERVER
#include "entity/character_combat.h"
#endif

#define ACTIVE_STATUS_QUEUED_MASK 2

//-------------------------------------------------------------------------------------------
// PowerRechargeTimer
//-------------------------------------------------------------------------------------------
MP_DEFINE(PowerRechargeTimer);
PowerRechargeTimer* powerRechargeTimer_Create(void)
{
    MP_CREATE(PowerRechargeTimer, 1000);
    return MP_ALLOC(PowerRechargeTimer);
}

void powerRechargeTimer_Destroy(PowerRechargeTimer* timer)
{
    assert(timer);
    MP_FREE(PowerRechargeTimer, timer);
}

//-------------------------------------------------------------------------------------------
// PowerInfo
//-------------------------------------------------------------------------------------------

MP_DEFINE(PowerInfo);

PowerInfo* powerInfo_Create(void)
{
    PowerInfo* info;
    MP_CREATE(PowerInfo, 50);
    info = MP_ALLOC(PowerInfo);
    eaCreate(&info->activePowers);
    eaCreate(&info->rechargeTimers);
    return info;
}

static void powerRef_DestroyCallback(void* arg0)
{
    powerRef_Destroy((PowerRef*)arg0);
}

void powerRechargeTimer_Destroy(PowerRechargeTimer* timer);
static void powerRechargeTimer_DestroyCallback(void* arg0)
{
    powerRechargeTimer_Destroy((PowerRechargeTimer*)arg0);
}

void powerInfo_Destroy(PowerInfo* info)
{
    if(!info)
        return;

    eaDestroyEx(&info->activePowers, powerRef_DestroyCallback);
    eaDestroyEx(&info->rechargeTimers, powerRechargeTimer_DestroyCallback);

    MP_FREE(PowerInfo, info);
}

/* Function powerInfo_ChangeActiveState()
 *  Sets the specified power as "active" or "inactive".
 *
 */
void powerInfo_ChangeActiveState(PowerInfo* info, PowerRef ppowRef, int active)
{
    if(active)
    {
        // We are trying to mark the power as active.
        // The goal is to have a single entry in the activePowers array, which means that
        // the said power is active.

        // Is the power already active?
        PowerRef* activeRef;
        activeRef = powerInfo_FindPower(&info->activePowers, ppowRef);

        // If we can find the power in the active powers array, the power is already "active".
        if (activeRef)
            return;

        // Otherwise, add a power to the list.
        activeRef = powerRef_Create();
        *activeRef = ppowRef;
        eaPush(&info->activePowers, activeRef);
        return;
    }
    else
    {
        // We are trying to mark the power as inactive.
        PowerRef* activeRef;
        int index;

        index = powerInfo_FindPowerIndex(&info->activePowers, ppowRef);

        // If the power does not exist in the activePower array, it is "inactive" by definition.
        if(index < 0)
            return;

        activeRef = eaRemove(&info->activePowers, index);
        assert(activeRef);    // Must exist because the index was just retrieved.

        powerRef_Destroy(activeRef);
    }
}

/* Function powerInfo_ChangeActiveState()
 *  Change the timer for the specified power to count down using the specified
 *  timer value.
 *
 *  If a timer has not yet been associated with the specified power, one will
 *  be created.
 */
void powerInfo_ChangeRechargeTimer(PowerInfo* info, PowerRef ppowRef, float newTime)
{
    PowerRechargeTimer* timer;
    timer = powerInfo_PowerGetRechargeTimer(info, ppowRef);

    // If the power doesn't have a timer associated with it yet, create one for it.
    if(!timer)
    {
        timer = powerRechargeTimer_Create();
        timer->ppowRef = ppowRef;
        eaPush(&info->rechargeTimers, timer);
    }
    timer->rechargeCountdown = newTime;
}

void powerInfo_UpdateTimers(PowerInfo* info, float elapsedTime)
{
    int i;
    for(i=eaSize(&info->rechargeTimers)-1; i>=0; i--)
    {
        PowerRechargeTimer* timer = eaGet(&info->rechargeTimers, i);
        assert(timer);  // Should always exist because the recharge timer array should be compacted on element remove.

        timer->rechargeCountdown -= elapsedTime;

        // If the timer has expired, remove the timer.
        if(timer->rechargeCountdown <= 0.0)
        {
            powerRechargeTimer_Destroy(timer);
            eaRemove(&info->rechargeTimers, i);
        }
    }
#if CLIENT 
    {    
        Entity *e = playerPtr();
        if (e)
        {        
            int iSizeSets;
            for(i=eaSize(&info->activePowers)-1; i>=0; i--)
            {        
                PowerRef *ppowRef = eaGet(&info->activePowers, i);
                Power *pow = SafeGetPowerByIdx(e->pchar, ppowRef->buildNum, ppowRef->powerSet, ppowRef->power);
                if (pow && pow->fUsageTime) {
                    // If a toggle power is on, reduce it's usage time if needed
                    pow->fUsageTime -= elapsedTime;
                }
            }
            iSizeSets = eaSize(&e->pchar->ppPowerSets);
            for(i=0; i<iSizeSets; i++)
            {
                int j;
                int iSizePows = eaSize(&e->pchar->ppPowerSets[i]->ppPowers);
                for(j=0; j<iSizePows; j++)
                {
                    Power *pow = e->pchar->ppPowerSets[i]->ppPowers[j];
                    if (!pow)
                        continue;
                    if (pow->ppowBase->eType == kPowerType_Auto && pow->fUsageTime)
                    {
                        // If an auto power exists, reduce it's usage time if needed
                        pow->fUsageTime -= elapsedTime;
                    }
                    if (pow->ppowBase->bHasLifetime && pow->ppowBase->fLifetimeInGame>0)
                    {
                        // Available time increases
                        pow->fAvailableTime += elapsedTime;
                    }
                }
            }
        }
    }
#endif    
}

int powerInfo_PowerIsActive(PowerInfo* info, PowerRef ppowRef)
{
    return (powerInfo_FindPower(&info->activePowers, ppowRef) ? 1 : 0);
}

PowerRechargeTimer* powerInfo_PowerGetRechargeTimer(PowerInfo* info, PowerRef ppowRef)
{
    return (PowerRechargeTimer*)powerInfo_FindPower(&(PowerRef**)info->rechargeTimers, ppowRef);
}

PowerRef* powerInfo_FindPower(PowerRef ***powerRefArray, PowerRef ppowRef)
{
    int i;
    for(i=eaSize(powerRefArray)-1; i>=0 ; i--)
    {
        PowerRef* element = eaGet(powerRefArray, i);
        if (element->buildNum == ppowRef.buildNum && element->powerSet == ppowRef.powerSet && element->power == ppowRef.power)
            return element;
    }

    return NULL;
}

int powerInfo_FindPowerIndex(PowerRef ***powerRefArray, PowerRef ppowRef)
{
    int i;
    for(i=eaSize(powerRefArray)-1; i>=0 ; i--)
    {
        PowerRef* element = eaGet(powerRefArray, i);
        if (element->buildNum == ppowRef.buildNum && element->powerSet == ppowRef.powerSet && element->power == ppowRef.power)
            return i;
    }

    return -1;
}
//-------------------------------------------------------------------------------------------
// PowerInfo network related stuff
//-------------------------------------------------------------------------------------------

void powerActivation_Send(PowerRef* ppowRef, Packet* pak, int activate)
{
    powerRef_Send(ppowRef, pak);
    pktSendBitsPack(pak, 1, activate);
}

void powerActivation_Receive(PowerRef* ppowRef, Packet* pak, int* activate)
{
    assert(activate);
    powerRef_Receive(ppowRef, pak);
    *activate = pktGetBitsPack(pak, 1);
}

void powerEnabledStatus_Send(PowerRef* ppowRef, Packet* pak, int enabled, int manuallyDisabled)
{
    powerRef_Send(ppowRef, pak);
    pktSendBitsAuto(pak, enabled);
    pktSendBitsAuto(pak, manuallyDisabled);
}

void powerEnabledStatus_Receive(PowerRef* ppowRef, Packet* pak, int *enabled, int *manuallyDisabled)
{
    assert(enabled && manuallyDisabled);
    powerRef_Receive(ppowRef, pak);
    *enabled = pktGetBitsAuto(pak);
    *manuallyDisabled = pktGetBitsAuto(pak);
}

void powerRechargeTimer_Send(PowerRechargeTimer* timer, Packet* pak)
{
    powerRef_Send(&timer->ppowRef, pak);
    pktSendF32(pak, timer->rechargeCountdown);
}

void powerRechargeTimer_Receive(PowerRechargeTimer* timer, Packet* pak)
{
    powerRef_Receive(&timer->ppowRef, pak);
    timer->rechargeCountdown = pktGetF32(pak);
}

void powerInfo_Send(PowerInfo* info, Packet* pak)
{
    int i;
    int iSize;

    iSize = eaSize(&info->activePowers);
    pktSendBitsPack(pak, 1, iSize);
    for(i = 0; i < iSize; i++)
    {
        powerRef_Send(info->activePowers[i], pak);
    }

    iSize = eaSize(&info->rechargeTimers);
    pktSendBitsPack(pak, 1, iSize);
    for(i = 0; i < iSize; i++)
    {
        PowerRechargeTimer* timer;
        timer = info->rechargeTimers[i];
        powerRechargeTimer_Send(timer, pak);
    }
}

void powerInfo_Receive(PowerInfo* info, Packet* pak)
{
    int i;
    int count;      // How many structures are we receiving?

    count = pktGetBitsPack(pak, 1);
    for(i = 0; i < count; i ++)
    {
        PowerRef* ppowRef = powerRef_Create();
        powerRef_Receive(ppowRef, pak);
        eaPush(&info->activePowers, ppowRef);
    }

    count = pktGetBitsPack(pak, 1);
    for(i = 0; i < count; i ++)
    {
        PowerRechargeTimer* timer = powerRechargeTimer_Create();
        powerRechargeTimer_Receive(timer, pak);
        eaPush(&info->rechargeTimers, timer);
    }
}


//-------------------------------------------------------------------------------------------
// PowerInfo Server only functions
//-------------------------------------------------------------------------------------------
#include "entity/entity.h"
#include "entity/powers.h"
#ifdef SERVER
#include "entity/character_net_server.h"
#include "svr/svr_base.h"

void powerInfo_UpdateFullRechargeStatus(Character *p)
{
    PowerListItem *ppowListItem;
    PowerListIter iter;

    // Recharge powers recently used.
    for(ppowListItem = powlist_GetFirst(&p->listRechargingPowers, &iter);
        ppowListItem != NULL;
        ppowListItem = powlist_GetNext(&iter))
    {
        eaPush(&p->entParent->rechargeStatusChange, powerRef_CreateFromPower(p->iCurBuild, ppowListItem->ppow));
    }
}

void power_sendTrayAdd( Entity *e, const BasePower * basePower )
{
    Power * ppow;
    START_PACKET( pak, e, SERVER_SEND_TRAY_ADD )
    ppow = character_OwnsPower( e->pchar, basePower );
    pktSendBitsPack( pak, 1, ppow->psetParent->idx );
    pktSendBitsPack( pak, 1, ppow->idx );
    END_PACKET
}

static void character_SendPowerRanges(Character *pchar, bool bSendAll, Packet *pak)
{
    int i;
    int j;

    assert(pchar!=NULL);

    for(i=eaSize(&pchar->ppPowerSets)-1; i>=0; i--)
    {
        for(j=eaSize(&pchar->ppPowerSets[i]->ppPowers)-1; j>=0; j--)
        {
            Power *ppow = pchar->ppPowerSets[i]->ppPowers[j];
            if(ppow!=NULL)
            {
                float fRange = 0.0f;
                if(ppow->ppowBase->eTargetType == kTargetType_Location
                    || ppow->ppowBase->eTargetType == kTargetType_Teleport
                    || ppow->ppowBase->eTargetType == kTargetType_Position)
                {
                    fRange = character_PowerRange(pchar, ppow);
                }
                if(ppow->ppowBase->eTargetTypeSecondary == kTargetType_Location
                    || ppow->ppowBase->eTargetTypeSecondary == kTargetType_Teleport
                    || ppow->ppowBase->eTargetTypeSecondary == kTargetType_Position)
                {
                    fRange = character_PowerRange2(pchar, ppow);
                }

                if(bSendAll || ppow->fRangeLast != fRange)
                {
                    pktSendBits(pak, 1, 1);
                    basepower_Send(pak, ppow->ppowOriginal);
                    pktSendF32(pak, fRange);
                    ppow->fRangeLast = fRange;
                }
            }
        }
    }

    pktSendBits(pak, 1, 0);
}

static void powerRef_DestroyAdapter(void* arg0)
{
    powerRef_Destroy((PowerRef*)arg0);
}

void entity_SendPowerInfoUpdate(Entity* e, Packet* pak)
{
    int sendPowerInfoUpdate;

    // Do we have any power info to update?
    sendPowerInfoUpdate = eaSize(&e->activeStatusChange)
        || eaSize(&e->enabledStatusChange)
        || eaSize(&e->usageStatusChange)
        || eaSize(&e->rechargeStatusChange)
        || eaiSize(&e->inspirationStatusChange)
        || eaiSize(&e->boostStatusChange)
        || eaSize(&e->powerDefChange);

    pktSendBits(pak, 1, sendPowerInfoUpdate);
    if(!sendPowerInfoUpdate)
    {
        character_SendPowerRanges(e->pchar, false, pak);
        return;
    }


    pktSendBits(pak, 4, e->pchar->iCurBuild);
    {
        int i;
        int iSize;

        bool bRedefine = false;

        for(i=eaSize(&e->powerDefChange)-1; i>=0 ; i--)
        {
            if(e->powerDefChange[i]==(void *)-1)
            {
                int j, k;
                int iSizeSets;

                bRedefine = true;

                eaClearEx(&e->powerDefChange, powerRef_DestroyAdapter);

                iSizeSets = eaSize(&e->pchar->ppPowerSets);
                for(k=0; k<iSizeSets; k++)
                {
                    int iSizePows = eaSize(&e->pchar->ppPowerSets[k]->ppPowers);
                    for(j=0; j<iSizePows; j++)
                    {
                        eaPush(&e->powerDefChange, powerRef_CreateFromIdxs(e->pchar->iCurBuild, k, j));
                    }
                }

                break;
            }
        }

        if(bRedefine)
        {
            pktSendBits(pak, 1, 1);

            if( e->pchar->ppowDefault )
            {
                pktSendBits(pak, 1, 1);
                basepower_Send( pak, e->pchar->ppowDefault->ppowOriginal );
            }
            else
                pktSendBits(pak, 1, 0);
        }
        else
        {
            pktSendBits(pak, 1, 0);
        }

        iSize = eaSize(&e->powerDefChange);
        pktSendBitsPack(pak, 1, iSize);
        for(i=0; i < iSize; i++)
        {
            PowerRef *ppowRef = e->powerDefChange[i];
            Power *ppow = SafeGetPowerByIdx(e->pchar, ppowRef->buildNum, ppowRef->powerSet, ppowRef->power);

            powerRef_Send(ppowRef, pak);

            if(ppow!=NULL)
            {
                int j;
                int iSize = eaSize(&ppow->ppBoosts);
                int iGlobalSize = eaSize(&ppow->ppGlobalBoosts);

                pktSendBits(pak, 1, 1);
                basepower_Send(pak, ppow->ppowOriginal);
                pktSendBitsPack(pak, 5, ppow->psetParent->iLevelBought);
                pktSendBitsPack(pak, 5, ppow->iLevelBought);
                pktSendBitsPack(pak, 3, ppow->iNumCharges);
                pktSendF32(pak, ppow->fUsageTime);
                pktSendF32(pak, ppow->fAvailableTime);
                pktSendBitsPack(pak, 24, ppow->ulCreationTime);
                pktSendBitsAuto(pak, ppow->bEnabled);
                pktSendBitsAuto(pak, ppow->bDisabledManually);
                pktSendBitsAuto(pak, ppow->iUniqueID);
                pktSendBitsPack(pak, 4, iSize);

                // Now the boosts
                for(j=0; j<iSize; j++)
                {
                    Boost *pboost = ppow->ppBoosts[j];
                    character_SendPowerBoost( e->pchar, pak, pboost, j );                    
                }

                // Now the global boosts
                pktSendBitsAuto(pak, iGlobalSize);
                for (j = 0; j < iGlobalSize; j++)
                {
                    Boost *pboost = ppow->ppGlobalBoosts[j];
                    character_SendPowerBoost(e->pchar, pak, pboost, j);
                }
            }
            else
            {
                pktSendBits(pak, 1, 0);
            }
        }

        eaClearEx(&e->powerDefChange, powerRef_DestroyAdapter);

        character_SendPowerRanges(e->pchar, bRedefine, pak);

        iSize = eaSize(&e->activeStatusChange);
        pktSendBitsPack(pak, 4, iSize);
        for(i = 0; i < iSize; i++)
        {
            PowerRef *ppowRef = e->activeStatusChange[i];
            Power *ppow = SafeGetPowerByIdx(e->pchar, ppowRef->buildNum, ppowRef->powerSet, ppowRef->power);

            if(ppow!=NULL)
            {
                int result = ppow->bActive;

                if(e->pchar->ppowQueued == ppow || e->pchar->ppowTried == ppow )
                {
                    result |= ACTIVE_STATUS_QUEUED_MASK;
                }

                pktSendBits(pak, 1, 1);
                powerActivation_Send(ppowRef, pak, result);
            }
            else
            {
                pktSendBits(pak, 1, 0);
            }
        }
        eaClearEx(&e->activeStatusChange, powerRef_DestroyAdapter);

        iSize = eaSize(&e->enabledStatusChange);
        pktSendBitsPack(pak, 4, iSize);
        for(i = 0; i < iSize; i++)
        {
            PowerRef *ppowRef = e->enabledStatusChange[i];
            Power *ppow = SafeGetPowerByIdx(e->pchar, ppowRef->buildNum, ppowRef->powerSet, ppowRef->power);

            if(ppow!=NULL)
            {
                pktSendBits(pak, 1, 1);
                powerEnabledStatus_Send(ppowRef, pak, ppow->bEnabled, ppow->bDisabledManually);
            }
            else
            {
                pktSendBits(pak, 1, 0);
            }
        }
        eaClearEx(&e->enabledStatusChange, powerRef_DestroyAdapter);

        iSize = eaSize(&e->usageStatusChange);
        pktSendBitsPack(pak, 4, iSize);
        for(i = 0; i < iSize; i++)
        {
            PowerRef *ppowRef = e->usageStatusChange[i];
            Power *ppow = SafeGetPowerByIdx(e->pchar, ppowRef->buildNum, ppowRef->powerSet, ppowRef->power);

            if(ppow!=NULL)
            {
                pktSendBits(pak, 1, 1);
                powerRef_Send(ppowRef, pak);
                pktSendBitsPack(pak, 3, ppow->iNumCharges);
                pktSendF32(pak, ppow->fUsageTime);
                pktSendF32(pak, ppow->fAvailableTime);
            }
            else
            {
                pktSendBits(pak, 1, 0);
            }
        }
        eaClearEx(&e->usageStatusChange, powerRef_DestroyAdapter);

        iSize = eaSize(&e->rechargeStatusChange);
        pktSendBitsPack(pak, 1, iSize);
        for(i = 0; i < iSize; i++)
        {
            PowerRef *ppowRef = e->rechargeStatusChange[i];
            Power *ppow = SafeGetPowerByIdx(e->pchar, ppowRef->buildNum, ppowRef->powerSet, ppowRef->power);

            if(ppow!=NULL)
            {
                PowerRechargeTimer timer;

                timer.ppowRef = *ppowRef;
                timer.rechargeCountdown = ppow->fTimer/(ppow->pattrStrength->fRechargeTime+0.0001f);

                pktSendBits(pak, 1, 1);
                powerRechargeTimer_Send(&timer, pak);
            }
            else
            {
                pktSendBits(pak, 1, 0);
            }
        }
        eaClearEx(&e->rechargeStatusChange, powerRef_DestroyAdapter);

        iSize = eaiSize(&e->inspirationStatusChange);
        pktSendIfSetBitsPack(pak, 4, iSize);
        for(i = 0; i < iSize; i++)
        {
            int idx = e->inspirationStatusChange[i];
            int iCol = idx/CHAR_INSP_MAX_ROWS;

            inspiration_Send(pak, e->pchar, iCol, idx-(iCol*CHAR_INSP_MAX_ROWS));
        }
        eaiSetSize(&e->inspirationStatusChange, 0);

        iSize = eaiSize(&e->boostStatusChange);
        pktSendBitsPack(pak, 1, iSize);
        for(i = 0; i < iSize; i++)
        {
            int idx = e->boostStatusChange[i];

            boost_Send(pak, e->pchar, idx);
        }
        eaiSetSize(&e->boostStatusChange, 0);
    }
}

void entity_SendPowerModeUpdate(Entity* e, Packet* pak)
{
    if(e->power_modes_update)
    {
        int i;
        int n = eaiSize(&e->pchar->pPowerModes);

        pktSendBits(pak, 1, 1);
        pktSendBitsPack(pak, 3, n);
        for(i=0; i<n; i++)
        {
            pktSendBitsPack(pak, 3, e->pchar->pPowerModes[i]);
        }
        e->power_modes_update = false;
    }
    else
    {
        pktSendBits(pak, 1, 0);
    }
}

#else
#include "player/player.h"
#include "entity/character_net_client.h"

static void character_ReceivePowerRanges(Character *pchar, Packet *pak)
{
    while(pktGetBits(pak, 1)==1)
    {
        Power *ppow;
        const BasePower *ppowBase = basepower_Receive(pak, &g_PowerDictionary, (pchar ? pchar->entParent : 0));
        float fRange = pktGetF32(pak);

        if(ppowBase)
        {
            if((ppow = character_OwnsPower(pchar, ppowBase))!=NULL)
            {
                ppow->fRangeLast = fRange;
            }
        }
    }
}

void entity_ReceivePowerInfoUpdate(Entity* e, Packet* pak)
{
    int hasPowerInfoUpdates;
    int iCurBuild;
    const BasePower * pBaseDefault = NULL;

    hasPowerInfoUpdates = pktGetBits(pak, 1);
    if(!hasPowerInfoUpdates)
    {
        character_ReceivePowerRanges(e->pchar, pak);
        return;
    }

    iCurBuild = pktGetBits(pak, 4);
    if (iCurBuild != e->pchar->iCurBuild)
    {
        character_SelectBuildClient(e, iCurBuild);
    }
    if(pktGetBits(pak,1))
    {
        int iset;

        e->pchar->ppowStance = NULL; // just to be safe...

        for(iset=eaSize(&e->pchar->ppPowerSets)-1; iset>=0; iset--)
        {
            powerset_Destroy(e->pchar->ppPowerSets[iset],NULL);
        }
        eaDestroy(&e->pchar->ppPowerSets);

        character_GrantAutoIssueBuildSharedPowerSets(e->pchar);
        for(iset=eaSize(&e->pchar->ppPowerSets)-1; iset>=0; iset--)
        {
            powerset_Empty(e->pchar->ppPowerSets[iset],NULL);
        }

        // Give them their auto issue, build-specific sets
        // (not shared sets, since we didn't destroy those)
        character_GrantAutoIssueBuildSpecificPowerSets(e->pchar);

        // Do we know the original sets they started with? These should be
        // the first-added if so, to keep in sync with the server.
        if(strlen(e->pl->primarySet) > 0 && strlen(e->pl->secondarySet) > 0)
        {
            const BasePowerSet* pset;

            pset = category_GetBaseSetByName(e->pchar->pclass->pcat[kCategory_Primary], e->pl->primarySet);
            character_BuyPowerSet(e->pchar, pset);
            pset = category_GetBaseSetByName(e->pchar->pclass->pcat[kCategory_Secondary], e->pl->secondarySet);
            character_BuyPowerSet(e->pchar, pset);
        }

        if(pktGetBits(pak,1))
        {
            pBaseDefault = basepower_Receive( pak, &g_PowerDictionary, e );
        }
        else
        {
            e->pchar->ppowDefault = NULL;
        }
    }

    {
        int iCnt;
        int i;

        iCnt = pktGetBitsPack(pak, 1);
        for(i = 0; i<iCnt; i++)
        {
            Power *ppow;
            int j;

            PowerRef ppowRef;
            const BasePower *ppowBase = NULL;
            int iSetLevelBought;
            int iLevelBought;
            int iNumCharges;
            float fUsageTime;
            float fAvailableTime;
            U32 ulCreationTime;
            int bEnabled;
            int bDisabledManually;
            int uniqueID;

            int iCntBoosts = 0;
            int iCntGlobalBoosts = 0;

            powerRef_Receive(&ppowRef, pak);

            if(pktGetBits(pak,1))
            {
                ppowBase = basepower_Receive(pak, &g_PowerDictionary, e);
                iSetLevelBought = pktGetBitsPack(pak, 5);
                iLevelBought = pktGetBitsPack(pak, 5);
                iNumCharges = pktGetBitsPack(pak, 3);
                fUsageTime = pktGetF32(pak);
                fAvailableTime = pktGetF32(pak);
                ulCreationTime = pktGetBitsPack(pak, 24);
                bEnabled = pktGetBitsAuto(pak);
                bDisabledManually = pktGetBitsAuto(pak);
                uniqueID = pktGetBitsAuto(pak);

                iCntBoosts = pktGetBitsPack(pak, 4);

                if(ppowBase && ppowRef.powerSet>=eaSize(&e->pchar->ppBuildPowerSets[ppowRef.buildNum]))
                {
                    // We don't have this power set yet.
                    // (Or something bad happened and the client and server
                    //   are out of sync.)
                    // Omnipotent is the only reason I can think of for
                    //   this to happen. When it does, the powers are all
                    //   redefined and then defined in order. So, the power
                    //   set index should match on the client and server.
                    // A second reason why this could happen is the Force
                    //   Feedback invented enhancement has just proc'd for
                    //   the first time since this player has entered the map.
                    PowerSet *pset;
                    //e->pchar->ppowStance = NULL;
                    //e->pchar->ppowDefault = NULL;
                    pset = character_AddNewPowerSet(e->pchar, ppowBase->psetParent);
                    pset->iLevelBought = iSetLevelBought;
                }

                // If we need to, add the power to the set.
                ppow = SafeGetPowerByIdx(e->pchar, ppowRef.buildNum, ppowRef.powerSet, ppowRef.power);
                if(ppowBase && (ppow==NULL || ppow->ppowBase!=ppowBase))
                {
                    // If there is already a power in the spot, then it's
                    // about to be destroyed. Clear out any references to it.
                    if(ppow==e->pchar->ppowStance)
                    {
                        e->pchar->ppowStance = NULL;
                    }
                    if(ppow==e->pchar->ppowDefault)
                    {
                        e->pchar->ppowDefault = NULL;
                    }
                    ppow = powerset_SetNewPowerAt(e->pchar->ppBuildPowerSets[ppowRef.buildNum][ppowRef.powerSet], ppowBase, ppowRef.power, NULL, uniqueID, NULL);
                }

                // If ppow is NULL, then something bad is happening.

                if(ppow!=NULL)
                {
                    int iCntAllowed = CountForLevel(e->pchar->iLevel-iLevelBought, g_Schedules.aSchedules.piFreeBoostSlotsOnPower);

                    power_DestroyAllBoosts(ppow,NULL);
                    eaSetSize(&ppow->ppBoosts, iCntBoosts);
                    ppow->iLevelBought = iLevelBought;
                    ppow->iNumCharges = iNumCharges;
                    ppow->fUsageTime = fUsageTime;
                    ppow->fAvailableTime = fAvailableTime;
                    ppow->ulCreationTime = ulCreationTime;
                    ppow->bEnabled = bEnabled;
                    ppow->bDisabledManually = bDisabledManually;

                    iCntAllowed = min(iCntAllowed, ppow->ppowBase->iMaxBoosts);
                    ppow->iNumBoostsBought = iCntBoosts-iCntAllowed;
                    if(ppow->iNumBoostsBought<0) ppow->iNumBoostsBought = 0; // Just for safety
                }
                for(j=0; j<iCntBoosts; j++)
                {
                    character_ReceivePowerBoost( e->pchar, pak, ppow, 0, j );                    
                }

                if (ppow)
                {
                    for (j = eaSize(&ppow->ppGlobalBoosts) - 1; j >= 0; j--)
                    {
                        eaRemoveAndDestroy(&ppow->ppGlobalBoosts, j, boost_Destroy);
                    }
                }

                iCntGlobalBoosts = pktGetBitsAuto(pak);
                for (j = 0; j < iCntGlobalBoosts; j++)
                {
                    character_ReceivePowerBoost(e->pchar, pak, ppow, 1, j);
                }
            }
            else
            {
                ppow = SafeGetPowerByIdx(e->pchar, ppowRef.buildNum, ppowRef.powerSet, ppowRef.power);
                if(ppow!=NULL)
                {
                    if(ppow == e->pchar->ppowStance)  e->pchar->ppowStance = NULL;
                    if(ppow == e->pchar->ppowDefault) e->pchar->ppowDefault = NULL;

                    powerset_DeletePowerAt(e->pchar->ppBuildPowerSets[ppowRef.buildNum][ppowRef.powerSet], ppowRef.power, NULL);
                }
            }
        }

        if( pBaseDefault )
        {
            e->pchar->ppowDefault = character_OwnsPower( e->pchar, pBaseDefault );
        }
    }

    character_ReceivePowerRanges(e->pchar, pak);

    {
        int activationCount;
        int i;
        // How many power activation updates are there?
        activationCount = pktGetBitsPack(pak, 4);
        for(i = 0; i < activationCount; i++)
        {
            // Retrieve and apply each one.
            PowerRef ppowRef;
            int activate;
            Power *ppow = NULL;

            if(pktGetBits(pak, 1))
            {
                powerActivation_Receive(&ppowRef, pak, &activate);

                if (ppowRef.buildNum < MAX_BUILD_NUM
                    && eaSize(&e->pchar->ppBuildPowerSets[ppowRef.buildNum]) > ppowRef.powerSet
                    && eaSize(&e->pchar->ppBuildPowerSets[ppowRef.buildNum][ppowRef.powerSet]->ppPowers) > ppowRef.power)
                {
                    ppow = e->pchar->ppBuildPowerSets[ppowRef.buildNum][ppowRef.powerSet]->ppPowers[ppowRef.power];
                }

                if(ppow!=NULL)
                {
                    if(activate & ACTIVE_STATUS_QUEUED_MASK)
                    {
                        e->pchar->ppowQueued = ppow;
                        activate &= ~ACTIVE_STATUS_QUEUED_MASK;
                    }
                    else if(e->pchar->ppowQueued==ppow)
                    {
                        e->pchar->ppowQueued = NULL;
                    }

                    powerInfo_ChangeActiveState(e->powerInfo, ppowRef, activate);

                    if(e->pchar->ppowStance==ppow)
                    {
                        g_timeLastAttack = g_timeLastPower = timerSecondsSince2000();
                    }
                }
            }
        }
    }

    {
        int enabledStatusCount;
        int i;
        // How many power disabled status updates are there?
        enabledStatusCount = pktGetBitsPack(pak, 4);
        for(i = 0; i < enabledStatusCount; i++)
        {
            // Retrieve and apply each one.
            PowerRef ppowRef;
            int enabled;
            int manuallyDisabled;
            Power *ppow = NULL;

            if(pktGetBits(pak, 1))
            {
                powerEnabledStatus_Receive(&ppowRef, pak, &enabled, &manuallyDisabled);

                if (ppowRef.buildNum < MAX_BUILD_NUM
                    && eaSize(&e->pchar->ppBuildPowerSets[ppowRef.buildNum]) > ppowRef.powerSet
                    && eaSize(&e->pchar->ppBuildPowerSets[ppowRef.buildNum][ppowRef.powerSet]->ppPowers) > ppowRef.power)
                {
                    ppow = e->pchar->ppBuildPowerSets[ppowRef.buildNum][ppowRef.powerSet]->ppPowers[ppowRef.power];
                }

                if(ppow!=NULL)
                {
                    if (!enabled && e->pchar->ppowQueued==ppow)
                    {
                        e->pchar->ppowQueued = NULL;
                    }

                    ppow->bEnabled = enabled;
                    ppow->bDisabledManually = manuallyDisabled;
                }
            }
        }
    }

    {
        int usageCount;
        int i;
        int iNumCharges;
        float fUsageTime;
        float fAvailableTime;
        // How many power usage updates are there?
        usageCount = pktGetBitsPack(pak, 4);
        for(i = 0; i < usageCount; i++)
        {
            // Retrieve and apply each one.
            PowerRef ppowRef;
            Power *ppow = NULL;

            if(pktGetBits(pak, 1))
            {
                powerRef_Receive(&ppowRef, pak);
                iNumCharges = pktGetBitsPack(pak, 3);
                fUsageTime = pktGetF32(pak);
                fAvailableTime = pktGetF32(pak);
                ppow = SafeGetPowerByIdx(e->pchar, ppowRef.buildNum, ppowRef.powerSet, ppowRef.power);
                if (ppow != NULL)
                {
                    ppow->iNumCharges = iNumCharges;
                    ppow->fUsageTime = fUsageTime;
                    ppow->fAvailableTime = fAvailableTime;
                }
            }
        }
    }

    {
        int timerCount;
        int i;

        // How many recharge timer updates are there?
        timerCount = pktGetBitsPack(pak, 1);
        for(i = 0; i < timerCount; i++)
        {
            // Retrieve and apply each one.
            PowerRechargeTimer timer;

            if(pktGetBits(pak, 1))
            {
                powerRechargeTimer_Receive(&timer, pak);
                powerInfo_ChangeRechargeTimer(e->powerInfo, timer.ppowRef, timer.rechargeCountdown);
            }
        }
    }

    {
        int iCnt;
        int i;

        // How many inspiration updates are there?
        iCnt = pktGetIfSetBitsPack(pak, 4);
        for(i = 0; i < iCnt; i++)
        {
            inspiration_Receive(pak, e->pchar);
        }
#if CLIENT && !TEST_CLIENT
        if (iCnt > 0)
        {
            recipeInventoryReparse();
            uiIncarnateReparse();
        }

#endif

    }

    {
        int iCnt;
        int i;

        // How many boost updates are there?
        iCnt = pktGetBitsPack(pak, 1);
        for(i = 0; i < iCnt; i++)
        {
            boost_Receive(pak, e->pchar);
        }
#if CLIENT && !TEST_CLIENT
        if (iCnt > 0)
        {
            recipeInventoryReparse();
            uiIncarnateReparse();
        }
#endif
    }

    {
        int i;
        // sanity check, turn off powers that don't exist
         for( i = 0; i < eaSize(&e->powerInfo->activePowers); i++ )
        {
            PowerRef *ppowRef = e->powerInfo->activePowers[i];
            if (ppowRef->buildNum < 0 || ppowRef->buildNum >= MAX_BUILD_NUM
                || ppowRef->powerSet < 0 || ppowRef->powerSet >= eaSize(&e->pchar->ppBuildPowerSets[ppowRef->buildNum])
                || ppowRef->power < 0 || ppowRef->power >= eaSize(&e->pchar->ppBuildPowerSets[ppowRef->buildNum][ppowRef->powerSet]->ppPowers)
                || e->pchar->ppBuildPowerSets[ppowRef->buildNum][ppowRef->powerSet]->ppPowers[ppowRef->power] == NULL )
            {
                powerInfo_ChangeActiveState(e->powerInfo, *ppowRef, 0 );
            }
        }
    } 
}

void entity_ReceivePowerModeUpdate(Entity* e, Packet* pak)
{
    int iHasUpdate = pktGetBits(pak, 1);

    if(iHasUpdate)
    {
        int i;
        int n;

        eaiSetSize(&e->pchar->pPowerModes, 0);

        n = pktGetBitsPack(pak, 3);
        for(i=0; i<n; i++)
        {
            eaiPush(&e->pchar->pPowerModes, pktGetBitsPack(pak, 3));
        }
    }
}

#endif

