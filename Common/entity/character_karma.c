#include <utilitieslib/stdtypes.h>
#include "character_karma.h"
#include <utilitieslib/components/earray.h>
#include "character_base.h"
#include <utilitieslib/utils/structDefines.h>
#include <utilitieslib/utils/SuperAssert.h>
#include "entity.h"
#include "svr/svr_base.h"
#include "comm_game.h"
#include <utilitieslib/components/EString.h>
#include "powers.h"
#include "script/ZoneEvents/ScriptedZoneEventKarma.h"
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/utils/file.h>
#include <utilitieslib/components/SharedMemory.h>

StaticDefineInt CharacterKarmaReasonText [] =
{
    DEFINE_INT
    {"PowerClick",CKR_POWER_CLICK},
    {"BuffPowerClick", CKR_BUFF_POWER_CLICK},
    {"TeamObjComplete",CKR_TEAM_OBJ_COMPLETE},
    {"AllPlayObjComplete",CKR_ALL_OBJ_COMPLETE},
    DEFINE_END
};

StaticDefineInt CharacterKarmaVarText [] =
{
    DEFINE_INT
    {"Amount",CKV_AMOUNT},
    {"Lifespan", CKV_LIFESPAN},
    {"StackLimit",CKV_STACK_LIMIT}, 
    DEFINE_END
};
SHARED_MEMORY ZoneEventKarmaTable g_ZoneEventKarmaTable;

#define MAX_HELPING_BUBBLE_COUNT 5
static int s_GetBubbleCount(Character *pchar, U32 currentTime)
{
    assert(pchar);
    if (pchar && pchar->karmaContainer.inZoneEvent)
    {
        int myBubble = (((int)(currentTime - pchar->karmaContainer.bubbleActivatedTimestamp)) < g_ZoneEventKarmaTable.powerBubbleDur) ? 1 : 0;
        int myGlowieBubble = (((int)(currentTime - pchar->karmaContainer.glowieBubbleActivatedTimestamp)) < g_ZoneEventKarmaTable.powerBubbleDur) ? MAX_HELPING_BUBBLE_COUNT : 0;
        return pchar->karmaContainer.numNearbyBubbles + myBubble + myGlowieBubble;
    }
    return 0;
}
void ScriptUpdateKarmaBucket(Character *pchar, int reason, U32 currentTime)
{
    assert(pchar);
    assert(reason < CKR_COUNT);
    if (pchar)
    {
        int i;
        CharacterKarmaBucket *bucket = &pchar->karmaContainer.karmaBucket[reason];

        for (i = (eaSize(&bucket->charKarma)-1); i >= 0; --i)
        {
            assert(bucket->charKarma[i]->lifespan > 0);
            if (((int)(currentTime - bucket->charKarma[i]->timeStamp)) > bucket->charKarma[i]->lifespan)
            {
                free(bucket->charKarma[i]);
                eaRemove(&bucket->charKarma, i);
            }
        }
    }
}
extern int *g_zoneEventScriptStageAwardsKarma;
CharacterKarmaBucket* GetUpdatedBucket(Character *pchar, int reason, U32 currentTime)
{
    assert(pchar);
    assert(reason < CKR_COUNT);
    assert(eaiSize(&g_zoneEventScriptStageAwardsKarma));
    if (pchar && (reason < CKR_COUNT) && eaiSize(&g_zoneEventScriptStageAwardsKarma) && ENTTYPE(pchar->entParent) == ENTTYPE_PLAYER)
    {
        ScriptUpdateKarmaBucket(pchar, reason, currentTime);
        return &pchar->karmaContainer.karmaBucket[reason];
    }
    return NULL;
}
void CharacterAddKarma(Character *pchar, int reason, U32 currentTime)
{
    CharacterAddKarmaEx(pchar, reason, currentTime, 0, 0);
}
void CharacterAddKarmaEx(Character *pchar, int reason, U32 currentTime, int amount, int lifespan)
{
    assert(pchar);
    assert(reason < CKR_COUNT);
    assert(eaiSize(&g_zoneEventScriptStageAwardsKarma));
    if (pchar && (reason < CKR_COUNT) && eaiSize(&g_zoneEventScriptStageAwardsKarma))
    {
        CharacterKarmaBucket *bucket = GetUpdatedBucket(pchar, reason, currentTime);
        int numBubbles = s_GetBubbleCount(pchar, currentTime);
        if (bucket && (numBubbles > 0))
        {
            CharacterKarma *karma = malloc(sizeof(CharacterKarma));

            assert(karma);
            if (pchar->karmaContainer.inZoneEvent)
                karma->karmaAmount = amount ? amount : g_ZoneEventKarmaTable.karmaReasonTable[reason].karmaVar[CKV_AMOUNT];
            else
                karma->karmaAmount = 0;
            karma->lifespan = lifespan ? lifespan : g_ZoneEventKarmaTable.karmaReasonTable[reason].karmaVar[CKV_LIFESPAN];
            karma->timeStamp = currentTime;
            karma->stackId = 0;
            eaPush(&bucket->charKarma, karma);
        }
    }
}
static int s_isKarmaPower(Character *pchar, const BasePower *power)
{
    assert(pchar);
    assert(power);
    if (pchar && power)
    {
        int **pAutoHit = &((int*)power->pAutoHit);
        int **pAffected = &((int*)power->pAffected);
        if (    (power->eTargetType == kTargetType_Caster) && 
            (eaiFind(pAutoHit, kTargetType_Caster) != -1) && (eaiSize(pAutoHit)==1) &&
            (eaiFind(pAffected, kTargetType_Caster) != -1) && (eaiSize(pAffected) == 1) &&
            (power->eEffectArea == kEffectArea_Character) 
            )
        {
            return 0;                                //    powers that only affects caster    (i.e. build up)
        }
        else 
        {
            int exemptSet = 0;
            int i;
            for (i = kCategory_Primary; i <= kCategory_Epic; i++)
            {
                if (power->psetParent->pcatParent == pchar->pclass->pcat[i])
                {
                    exemptSet = 1;
                    break;
                }
            }
            if (!exemptSet)
            {
                if (power->eEffectArea == kTargetType_Location)
                {
                    return 0;                        //    catching inherent, temp, incarnate powers that are directed at a location (pet summons or workbench)
                }
            }
        }
        return 1;
    }
    return 0;
}
#define LIFESPAN_CAP 30
#define LIFESPAN_DEMONINATOR 10.f
void CharacterAddPowerKarma(Character *pchar, U32 currentTime, const BasePower *power)
{
    CharacterKarmaBucket *bucket = NULL;
    assert(pchar);
    assert(eaiSize(&g_zoneEventScriptStageAwardsKarma));
    bucket = GetUpdatedBucket(pchar, CKR_POWER_CLICK, currentTime);
    if (bucket)
    {
        int numBubbles = s_GetBubbleCount(pchar, currentTime);
        if (numBubbles > 0)
        {
            if (s_isKarmaPower(pchar, power))
            {
                CharacterKarma *karma = malloc(sizeof(CharacterKarma));
                float cycleTime = power->fRechargeTime + power->fTimeToActivate;
                assert(karma);

                if (pchar->karmaContainer.inZoneEvent)
                    karma->karmaAmount = g_ZoneEventKarmaTable.karmaReasonTable[CKR_POWER_CLICK].karmaVar[CKV_AMOUNT];
                else
                    karma->karmaAmount = 0;
                karma->lifespan = g_ZoneEventKarmaTable.powerLifespanCap * (cycleTime/(cycleTime+g_ZoneEventKarmaTable.powerLifespanConst));
                karma->stackId = power->crcFullName;
                karma->timeStamp = currentTime;
                eaPush(&bucket->charKarma, karma);
            }
        }
    }
}

void CharacterDestroyKarmaBuckets(Character *pchar)
{
    assert(pchar);
    if (pchar)
    {
        int i;
        for (i = 0; i < CKR_COUNT; ++i)
        {
            int j;
            CharacterKarmaBucket *bucket = &pchar->karmaContainer.karmaBucket[i];
            for (j = (eaSize(&bucket->charKarma)-1); j >= 0; --j)
            {
                free(bucket->charKarma[j]);
            }
            eaDestroy(&bucket->charKarma);
        }
        CharacterKarmaInit(pchar);
    }
}
#define STACK_MODIFIER 0.5f
static int ScripCalculatePowerKarma(CharacterKarmaBucket *bucket)
{
    int i, sum = 0;
    int *idList = NULL;
    int *karmaAmountList = NULL;
    int *sortedList = NULL;
    int stackLimit;
    assert(bucket);
    for (i = 0; i < eaSize(&bucket->charKarma); ++i)
    {
        int id = bucket->charKarma[i]->stackId;
        int index;
        if ((index = eaiFind(&idList, id)) == -1)
        {
            int amount = bucket->charKarma[i]->karmaAmount;
            eaiPush(&idList, id);
            eaiPush(&karmaAmountList, amount);
            eaiSortedPush(&sortedList, amount);
        }
        else
        {
            karmaAmountList[index] *= (1.0f/g_ZoneEventKarmaTable.powerStackModifier);
            eaiSortedPush(&sortedList, karmaAmountList[index]);
        }
    }
    stackLimit = g_ZoneEventKarmaTable.karmaReasonTable[CKR_POWER_CLICK].karmaVar[CKV_STACK_LIMIT];
    stackLimit = (stackLimit < 0) ? eaiSize(&sortedList) : (MIN(stackLimit, eaiSize(&sortedList)));
    for (i = 0; i < stackLimit; ++i)
    {
        sum += sortedList[i];
    }
    eaiDestroy(&sortedList);
    eaiDestroy(&idList);
    eaiDestroy(&karmaAmountList);
    return sum;
}
int CharacterCalculateKarmaBucket(Character *pchar, int i, U32 currentTime)
{
    int sum = 0;
    assert(pchar);
    if (pchar && pchar->karmaContainer.inZoneEvent)
    {
        CharacterKarmaBucket *bucket = &pchar->karmaContainer.karmaBucket[i];
        int numBubbles = s_GetBubbleCount(pchar, currentTime);
        ScriptUpdateKarmaBucket(pchar, i, currentTime);
        switch (i)
        {
        case CKR_POWER_CLICK:
            sum += ScripCalculatePowerKarma(bucket);
            break;
        default:
            {
                int j;
                int *valueList = NULL;
                int stackLimit = g_ZoneEventKarmaTable.karmaReasonTable[i].karmaVar[CKV_STACK_LIMIT];
                for (j = 0; j < eaSize(&bucket->charKarma); ++j)
                {
                    eaiSortedPush(&valueList, bucket->charKarma[j]->karmaAmount);
                }
                stackLimit = (stackLimit < 0) ? eaiSize(&valueList) : (MIN(eaiSize(&valueList), stackLimit));
                for (j = 0; j < stackLimit; ++j)
                {
                    sum += valueList[j];
                }
                eaiDestroy(&valueList);
            }
        }
        sum *= (1 + (MIN((numBubbles-1), MAX_HELPING_BUBBLE_COUNT)*0.1));
    }
    return sum;
}

int CharacterCalculateKarma(Character *pchar, U32 currentTime)
{
    int i, sum = 0;
    assert(pchar);
    if (pchar)
    {
        const char *pchName = (pchar && pchar->pclass) ? pchar->pclass->pchName : NULL;
        F32 classMod = 1.f;

        for (i = 0; i < CKR_COUNT; ++i)
        {
            sum += CharacterCalculateKarmaBucket(pchar, i, currentTime);
        }
        if (pchName)
        {
            int j;
            for (j = 0; j < eaSize(&g_ZoneEventKarmaTable.ppClassMod); ++j)
            {
                if (!stricmp(g_ZoneEventKarmaTable.ppClassMod[j]->pchClassName, pchName))
                {
                    classMod = g_ZoneEventKarmaTable.ppClassMod[j]->classMod;
                    break;
                }
            }
        }
        sum *= classMod;
    }
    return sum;
}
void CharacterKarmaStatsSend(Entity *e, U32 currentTime)
{
    assert(e);
    if (e)
    {
        START_PACKET( pak, e, SERVER_SEND_KARMA_STATS )
        if (e->pchar->karmaContainer.inZoneEvent && (eaiFind(&g_zoneEventScriptStageAwardsKarma, e->pchar->karmaContainer.inZoneEvent)!= -1))
        {
            int i;
            pktSendBits(pak, 1, 1);
            
            pktSendBitsAuto(pak, CKR_COUNT+5);    //    5 for the extra stat sends (total, average, median, active players, num bubbles)
            for (i = 0; i < CKR_COUNT; ++i)
            {
                pktSendBitsAuto(pak, CharacterCalculateKarmaBucket(e->pchar, i, currentTime));
            }

            pktSendBitsAuto(pak, CharacterCalculateKarma(e->pchar, currentTime));
            pktSendBitsAuto(pak, ScriptKarmaGetMedian(e, e->pchar->karmaContainer.inZoneEvent, SKT_SORTED_STAT));
            pktSendBitsAuto(pak, ScriptKarmaGetAverage(e, e->pchar->karmaContainer.inZoneEvent, SKT_SORTED_STAT));
            pktSendBitsAuto(pak, ScriptKarmaGetActivePlayers(e->pchar->karmaContainer.inZoneEvent));
            pktSendBitsAuto(pak, s_GetBubbleCount(e->pchar, currentTime));

            //    If you add any more stats to send here, make sure you increment the count of pak sends above
        }
        else
        {
            pktSendBits(pak, 1, 0);
        }
                    END_PACKET
    }
}
int CharacterKarma_SetKarmaValue(char *reasonStr, char *varStr, int newKarmaValue)
{
    ZoneEventKarmaTable *mutableKarmaTable = cpp_const_cast(ZoneEventKarmaTable*)(&g_ZoneEventKarmaTable);
    int reason = StaticDefineIntGetInt(CharacterKarmaReasonText, reasonStr);
    if (reason != -1)
    {
        int var = StaticDefineIntGetInt(CharacterKarmaVarText, varStr);
        if (var != -1)
        {
            mutableKarmaTable->karmaReasonTable[reason].karmaVar[var] = newKarmaValue;
            return 0;
        }
        return 1;
    }
    if (stricmp("Power", reasonStr) == 0)
    {
        if (stricmp("LifeCap", varStr) == 0)
        {
            mutableKarmaTable->powerLifespanCap = newKarmaValue;
            return 0;
        }
        if (stricmp("LifeConst", varStr) == 0)
        {
            mutableKarmaTable->powerLifespanConst = newKarmaValue;
            return 0;
        }
        if (stricmp("StackMod", varStr) == 0)
        {
            mutableKarmaTable->powerStackModifier = newKarmaValue;
            return 0;
        }
        if (stricmp("BubbleDur", varStr) == 0)
        {
            mutableKarmaTable->powerBubbleDur = newKarmaValue;
            return 0;
        }
        if (stricmp("BubbleRad", varStr) == 0)
        {
            mutableKarmaTable->powerBubbleRad = newKarmaValue;
            return 0;
        }
        return 1;
    }

    return 2;
}
void CharacterKarmaActivateBubble(Character *pchar, U32 currentTime)
{
    if (pchar->karmaContainer.inZoneEvent)
        pchar->karmaContainer.bubbleActivatedTimestamp = currentTime;
}

void CharacterKarmaInit(Character *pchar)
{
    pchar->karmaContainer.glowieBubbleActivatedTimestamp = 0;
    pchar->karmaContainer.bubbleActivatedTimestamp = 0;
    pchar->karmaContainer.numNearbyBubbles = 0;
    pchar->karmaContainer.inZoneEvent = 0;
}