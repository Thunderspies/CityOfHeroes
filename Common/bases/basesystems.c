/***************************************************************************
 *     Copyright (c) 2005-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/assert/assert.h>
#include <utilitieslib/utils/mathutil.h>
#include <float.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/utils/error.h>

#include "bases/bases.h"
#include "bases/basedata.h"
#include "bases/basesystems.h"

#if SERVER
#include "ailib/aiBehaviorPublic.h"
#include "container/containerSupergroup.h"
#include "cmdparse/cmdserver.h"
#include "entity/entity.h"
#include "ai/entai.h"
#include "ai/entaiprivate.h"
#include <utilitieslib/components/EString.h>
#include "entity/powers.h" // For granting powers via aux items
#include "entity/character_base.h" // For granting powers via aux items
#include "entity/character_combat.h"
#include "entity/character_animfx.h"
#include "svr/svr_player.h"
#include "container/team.h"
#include <utilitieslib/utils/timing.h>
#include "entity/Supergroup.h"
#include "gameSys/raidmapserver.h"
#include "bases/baseserverrecv.h"
#include "bases/basetogroup.h"
#include "gridcoll/gridcache.h"
#include "entity/SgrpServer.h"
#include "bases/baseupkeep.h"
#include "gameSys/sgraid.h"
#include "gameComm/svr_chat.h"
#include "language/langServerUtil.h"
#include "dbcomm/logcomm.h"
#elif CLIENT
#endif

bool g_bBaseUpdateAux = false;
bool g_bBaseUpdateEnergy = false;
bool g_bBaseUpdateControl = false;

extern int world_modified;

static void handleAuxChange(RoomDetail *detail, RoomDetail *aux, bool active);

#if SERVER
Entity* FindEntityInSupergroup(int supergroup_id)
{
    Entity *e = NULL;
    PlayerEnts *players = &player_ents[PLAYER_ENTS_ACTIVE];
    int i;

    for(i=0; i<players->count; i++)
    {
        if(players->ents[i]
           && players->ents[i]->supergroup
           && (players->ents[i]->supergroup_id == supergroup_id))
        {
            e = players->ents[i];
            break;
        }
    }

    // ----------
    // finally

    return e;
}

#define SECS_SGRP_QUERY_DELAY (15*60)

/**********************************************************************func*
 * GetSupergroupForBase
 *
 */
Supergroup *GetSupergroupForBase(Base *pBase)
{
    Supergroup *sg;

    if(!pBase || !pBase->supergroup_id)
        return NULL;

    // load sgrp from db if not here 
    // could happen if a coalition member or teammate not in sgrp enters
    // base. We need to get the base state at least once.

    sg = sgrpFromDbId(pBase->supergroup_id);

    if( !sg && (g_base.timeRanDbQueryForSgrp + SECS_SGRP_QUERY_DELAY < dbSecondsSince2000()))
    {
        dbSyncContainerRequest(CONTAINER_SUPERGROUPS, pBase->supergroup_id, CONTAINER_CMD_TEMPLOAD, false);

        // -----
        // update the baseupkeep state

        g_base.timeRanDbQueryForSgrp = dbSecondsSince2000();
        sg = sgrpFromDbId(pBase->supergroup_id); 
    }

    return sg;
}
#endif

void detailFlagBaseUpdate(RoomDetail *detail)
{
    // Update the base update flags
    if (detail->info->iEnergyConsume || detail->info->iEnergyProduce)
        g_bBaseUpdateEnergy = true;
    if (detail->info->iControlConsume || detail->info->iControlProduce)
        g_bBaseUpdateControl = true;
    if (detail->info->ppchAuxAllowed || IS_AUXILIARY(detail->info->eFunction))
        g_bBaseUpdateAux = true;
}

// Locates all auxiliary details attached to the given detail
//  TODO: Needs to be cached in a hash or something
RoomDetail **findAttachedAuxiliary(BaseRoom *room, RoomDetail *detail)
{
    int j,id = detail->id;
    RoomDetail **attached = NULL;

    for (j = eaSize(&room->details)-1; j >= 0; j--)
    {
        if (room->details[j]->iAuxAttachedID == id)
        {
            eaPush(&attached,room->details[j]);
        }
    }

    return attached;
}

void detachAuxiliary(BaseRoom *room, RoomDetail *auxiliary)
{
    RoomDetail *oldDetail = NULL;

    // Figure out the room, if it wasn't passed in
    if (!room)
        baseGetDetail(&g_base,auxiliary->id,&room);

    if (!auxiliary->iAuxAttachedID) {
        RoomDetail **attached = findAttachedAuxiliary(room,auxiliary);
        if (attached)
        {
            int i;
            for (i = eaSize(&attached)-1; i >= 0; i--)
                detachAuxiliary(room,attached[i]);
            eaDestroy(&attached);
        }
        return;
    }

    oldDetail = roomGetDetail(room, auxiliary->iAuxAttachedID);

#if SERVER
    handleAuxChange(oldDetail,auxiliary,false);
#endif
    auxiliary->iAuxAttachedID = 0;
}

static bool validAuxDetail(RoomDetail *detail, RoomDetail *auxiliary)
{
    int i;
    const char *pchAuxName;

    if (!detail->info
        || !detail->info->ppchAuxAllowed
        || !auxiliary->info
        || !IS_AUXILIARY(auxiliary->info->eFunction))
        return false;

    pchAuxName = auxiliary->info->pchName;

    for (i = eaSize(&detail->info->ppchAuxAllowed)-1; i >= 0; i--)
    {
        if (0 == stricmp(detail->info->ppchAuxAllowed[i],pchAuxName))
            return true;
    }

    return false;
}

typedef struct RoomDetailAuxPair
{
    RoomDetail *primary;
    int iPrimaryRoomIdx;
    RoomDetail *auxiliary;
    int iAuxiliaryRoomIdx;
    float dist;
} RoomDetailAuxPair;

// We sort by: distance; primary id; aux id
static int compareAuxPairPriority(const void* p1Data, const void* p2Data)
{
    const RoomDetailAuxPair ** p1 = (const RoomDetailAuxPair **)p1Data;
    const RoomDetailAuxPair ** p2 = (const RoomDetailAuxPair **)p2Data;
    int dif;
    float fdif = (*p1)->dist - (*p2)->dist;
    if (fdif > 0) return 1;
    if (fdif < 0) return -1;
    dif = (*p1)->primary->id - (*p2)->primary->id;
    if (dif) return dif;
    return (*p1)->auxiliary->id - (*p2)->auxiliary->id;
}

void roomAttachAuxiliary(BaseRoom *room, RoomDetail **ppDetails)
{
    int i,j;
    int iNumDetails = eaSize(&ppDetails);
    RoomDetailAuxPair **ppPairs = NULL;
    int iNumPairs;
    int *piAttachTo, *piAttachCount;

    for (i = 0; i < iNumDetails; i++)
    {
        // Don't hook new aux details up to destroyed details
        //  This shouldn't ever occur anyway
        if (ppDetails[i]->bDestroyed)
            continue;

        for (j = 0; j < iNumDetails; j++)
        {
            RoomDetailAuxPair *pPair = NULL;
            if (i==j || ppDetails[j]->bDestroyed || !validAuxDetail(ppDetails[i],ppDetails[j]))
                continue;

            pPair = (RoomDetailAuxPair*)calloc(1,sizeof(RoomDetailAuxPair));
            pPair->primary = ppDetails[i];
            pPair->iPrimaryRoomIdx = i;
            pPair->auxiliary = ppDetails[j];
            pPair->iAuxiliaryRoomIdx = j;
            pPair->dist = distance3Squared(ppDetails[i]->mat[3], ppDetails[j]->mat[3]);
            eaPush(&ppPairs,pPair);
        }
    }

    //if (!ppPairs) return;

    piAttachTo = calloc(iNumDetails,sizeof(int));
    piAttachCount = calloc(iNumDetails,sizeof(int));

    iNumPairs = eaSize(&ppPairs);
    qsort(ppPairs,iNumPairs,sizeof(ppPairs[0]),compareAuxPairPriority);

    // Find the good pairs
    for (i = 0; i < iNumPairs; i++)
    {
        int iPriIdx = ppPairs[i]->iPrimaryRoomIdx;
        int iAuxIdx = ppPairs[i]->iAuxiliaryRoomIdx;
        if (piAttachCount[iPriIdx] < ppPairs[i]->primary->info->iMaxAuxAllowed
            && piAttachTo[iAuxIdx] == 0)
        {
            piAttachCount[iPriIdx]++;
            piAttachTo[iAuxIdx] = iPriIdx+1; // Intentionally off by one
        }
    }

    free(piAttachCount);
    eaDestroy(&ppPairs);

    for (i = 0; i < iNumDetails; i++)
    {
        RoomDetail *aux = ppDetails[i];
        RoomDetail *pri = NULL;

        if (!piAttachTo[i])
        {
            if (aux->iAuxAttachedID) detachAuxiliary(room,aux);
            continue;
        }

        pri = ppDetails[piAttachTo[i]-1]; // Revert back to 0-index

        if (pri->id == aux->iAuxAttachedID)
            continue;  // Attached to the right thing already

        if (aux->iAuxAttachedID) detachAuxiliary(room,aux);

        aux->iAuxAttachedID = pri->id;
#if SERVER
        handleAuxChange(pri,aux,aux->bPowered && aux->bControlled);
#endif
    }

    free(piAttachTo);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//
// Server only from here down
//
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#if SERVER

RoomDetail **ppDetailRebuild = NULL;
RoomDetail **ppDetailRecostume = NULL;
RoomDetail **ppDetailUpdateActive = NULL;


static void detailGetVolumePos(RoomDetail *detail, Vec3 out_vec)
{
    Entity *e = detail->e;

    BaseRoom *room = NULL;
    baseGetDetail(&g_base,detail->id,&room);
    if (room)
    {
        Vec3 min,max;
        int i;
        baseDetailVolumeTriggerExtents(detail->info,detail->mat,min,max);
        for(i=0;i<=2;i++) min[i] = (min[i] + max[i]) / 2.0;
        addVec3(room->pos,min,out_vec);
    }
    else
        copyVec3(detail->mat[3],out_vec);
    return;
}

void detailBehaviorSetToDefault(RoomDetail *detail)
{
    if (!detail->e)
        return;

    aiBehaviorMarkAllFinished(detail->e,ENTAI(detail->e)->base,0);

    if (detail->info->eFunction == kDetailFunction_Trap)
    {
        // Handle trap default behavior
        char achAttackVolumeName[256] = "AttackVolumes(float(-1,0,0),string(";
        Vec3 vecVolume;
        detailGetVolumePos(detail,vecVolume);
        strcat(achAttackVolumeName,detailAttackVolumesVecName(vecVolume));
        strcat(achAttackVolumeName,";))");
        aiBehaviorAddString(detail->e,ENTAI(detail->e)->base,achAttackVolumeName);
    }
    else if (detail->info->eFunction == kDetailFunction_Field)
    {
        char achAttackTargetName[256];
        sprintf(achAttackTargetName,"AlwaysAttack,DoNotChangePowers,AttackTarget(EntRef(%I64x))",erGetRef(detail->e));
        aiBehaviorAddString(detail->e,ENTAI(detail->e)->base,achAttackTargetName);
        {
            // Fix up power selection
            AIPowerInfo *info;
            AIVars *ai = ENTAI(detail->e);

            for(info = ai->power.list; info; info = info->next){
                if (info->power->ppowBase->eType == kPowerType_Click)
                {
                    ai->power.current = info;
                    break;
                }
            }
        }
    }
    else if (detail->info->eFunction == kDetailFunction_StorageSalvage)
    {
        // nothing
    }
    else
    {
        aiBehaviorAddString(detail->e,ENTAI(detail->e)->base,detail->info->pchBehavior);
    }
    
    detail->bActiveBehavior = true;

//    aiBehaviorProcess(detail->e,ENTAI(detail->e)); // Makes sure the behavior starts up right away
}

void detailBehaviorUpdateActive(RoomDetail *detail)
{
    AIBehavior *cur = NULL;
    AIBehavior *root = NULL;
    char bOn = detail->bPowered && detail->bControlled;

    // Turn on or off
    if(detail->iAuxAttachedID && detail->bActiveAux != bOn)
    {
        RoomDetail *primary = baseGetDetail(&g_base,detail->iAuxAttachedID,NULL);
        handleAuxChange(primary,detail,bOn);
    }

    if (!detail->e || detail->bActiveBehavior==bOn)
        return;

    detail->bActiveBehavior = bOn;

    if (bOn)
    {
        aiBehaviorMarkFinished(detail->e,ENTAI(detail->e)->base,
            aiBehaviorGetTopBehaviorByName(detail->e,ENTAI(detail->e)->base, "DoNothing"));
    }
    else
    {
        if(detail->e->pchar) character_EnterStance(detail->e->pchar, NULL, true);
        aiBehaviorAddString(detail->e,ENTAI(detail->e)->base,"DoNothing");
    }
}

char *detailAttackVolumesVecName(Vec3 vec)
{
    static char temp[32] = "_";
    static char name[128] = "";
    itoa((int)(vec[0]+(vec[0]>=0?0.001:-0.001)),name,10); // In case not exactly aligned
    itoa((int)(vec[1]+0.05),&temp[1],10); // Fixes ceiling volumes in bases
    strcat(name,temp);
    itoa((int)(vec[2]+(vec[2]>=0?0.001:-0.001)),&temp[1],10); // In case not exactly aligned
    strcat(name,temp);
    return name;
}

static void handleAuxBehaviorAttackVolumes(RoomDetail *detail, RoomDetail *aux, bool attach)
{
    int iAttackVolumesHandle;
    int iCurBehaviorHandle;
    char *estrNewBehavior = NULL;

    Vec3 vecVolume;

    char *pchPrefix = "AttackVolumes(";//string(";

    if (!detail->e)
        return;

    iAttackVolumesHandle = aiBehaviorGetTopBehaviorByName(detail->e,ENTAI(detail->e)->base, "AttackVolumes");
    iCurBehaviorHandle = aiBehaviorGetCurBehavior(detail->e,ENTAI(detail->e)->base);

    estrCreate(&estrNewBehavior);

    if (attach)
        estrPrintCharString(&estrNewBehavior, "AddVolume");
    else
        estrPrintCharString(&estrNewBehavior, "RemoveVolume");

    detailGetVolumePos(aux,vecVolume);
    estrConcatf(&estrNewBehavior, "(%.1f,%.1f,%.1f)", vecParamsXYZ(vecVolume));

    if (!iAttackVolumesHandle)
    {
        char *estrActualString = NULL;
        estrPrintf(&estrActualString, "AttackVolumes(float(-1,0,0),%s)", estrNewBehavior);
        aiBehaviorAddString(detail->e,ENTAI(detail->e)->base, estrActualString);
    }
    else
        aiBehaviorAddFlag(detail->e,ENTAI(detail->e)->base, iAttackVolumesHandle, estrNewBehavior);

    detail->bActiveBehavior = true;

    detailBehaviorUpdateActive(detail);
    estrDestroy(&estrNewBehavior);
}

static void handleAuxBehaviorGrantPower(RoomDetail *aux, RoomDetail *detail, bool attach)
{
    int i;
    const char **ppchPowers = aux->info->ppchFunctionParams;

    if (!detail->e || !detail->e->pchar || !ppchPowers)
        return;

    for (i = eaSize(&ppchPowers)-1; i >= 0; i--)
    {
        const BasePower *ppowBase = powerdict_GetBasePowerByFullName(&g_PowerDictionary, ppchPowers[i]);

        if (!ppowBase)
        {
            Errorf("Unable to find power %s for auxiliary behavior",ppchPowers[i]);
            continue;
        }

        if (attach)
            character_AddRewardPower(detail->e->pchar, ppowBase);
        else
            character_RemoveBasePower(detail->e->pchar, ppowBase);
    }
}

static void handleAuxBehaviorApplyPower(RoomDetail *aux, RoomDetail *detail, bool attach)
{
    const BasePower *ppowBase;
    AIVars *ai;
    const char **ppchPowers = aux->info->ppchFunctionParams;

    if (!aux->e || !aux->e->pchar || !detail->e || !detail->e->pchar || !ppchPowers)
        return;

    ppowBase = powerdict_GetBasePowerByFullName(&g_PowerDictionary, ppchPowers[0]);

    if (!ppowBase)
    {
        Errorf("Unable to find power %s for auxiliary behavior of detail %s",ppchPowers[0],aux->info->pchName);
        return;
    }

    ai = ENTAI(aux->e);

    if (attach)
    {
        AIPowerInfo *info;
        for(info = ai->power.list; info; info = info->next){
            if (info->power->ppowBase == ppowBase)
                break;
        }
        if (!info) return;

        ai->power.current = info;
        ai->doNotChangePowers = true;
        aux->bAuxTick = true;
    }
    else
    {
        ai->power.current = NULL;
        ai->doNotChangePowers = true;
        aux->bAuxTick = false;
    }
}


static void handleAuxChange(RoomDetail *detail, RoomDetail *aux, bool active)
{
    int i;

    if (!detail || !aux)
        return;

    if(aux->bActiveAux == active)
        return; // Already applied

    switch (aux->info->eFunction)
    {
    case kDetailFunction_AuxSpot:
        handleAuxBehaviorAttackVolumes(detail,aux,active);
        break;
    case kDetailFunction_AuxApplyPower:
        handleAuxBehaviorApplyPower(aux,detail,active);
        break;
    case kDetailFunction_AuxGrantPower:
        handleAuxBehaviorGrantPower(aux,detail,active);
        break;
    case kDetailFunction_AuxBoostEnergy:
        i = aux->info->iEnergyProduce;
        if(!active) i *= -1;
        detail->iBonusEnergy += i;
        g_bBaseUpdateEnergy = true;
        break;
    case kDetailFunction_AuxBoostControl:
        i = aux->info->iControlProduce;
        if(!active) i *= -1;
        detail->iBonusControl += i;
        g_bBaseUpdateControl = true;
        break;
    case kDetailFunction_AuxTeleBeacon:
    case kDetailFunction_AuxMedivacImprover:
    case kDetailFunction_AuxWorkshopRepair:
    case kDetailFunction_AuxContact:
        break;
    default:
        Errorf("Unhandled auxiliary function %d used by detail %s",aux->info->eFunction,aux->info->pchName);
        break;
    }

    aux->bActiveAux = active;
    if(aux->info->pchEntityDef)
        eaPushUnique(&ppDetailRecostume,aux);
}

static void handleAuxiliaryTick(RoomDetail *aux,RoomDetail *detail,float fRate)
{
    if (aux->info->eFunction == kDetailFunction_AuxApplyPower)
    {
        AIVars *ai;

        if (!aux->e || !aux->e->pchar || !detail->e)
            return;

        ai = ENTAI(aux->e);
        if (!ai->power.current)
            handleAuxBehaviorApplyPower(aux,detail,true);

        if (ai->power.current && ai->power.current->power->fTimer <= 0.0)
            character_ActivatePower(aux->e->pchar,erGetRef(detail->e),ENTPOS(detail->e),ai->power.current->power);
    }
}

static void baseAuxiliaryTick(Base *base,float fRate)
{
    int i,j;

    PERFINFO_AUTO_START(__FUNCTION__, 1);
        for (i=eaSize(&base->rooms)-1;i>=0;i--)
        {
            BaseRoom *room = base->rooms[i];
            for (j=eaSize(&room->details)-1;j>=0;j--)
            {
                RoomDetail *detail = room->details[j];
                if (detail->bAuxTick && detail->bPowered && detail->bControlled)
                {
                    RoomDetail *primary = roomGetDetail(room,detail->iAuxAttachedID);
                    if (primary)
                        handleAuxiliaryTick(detail,primary,fRate);
                }
            }
        }
    PERFINFO_AUTO_STOP();
}

static void baseAttachAllAuxiliary(Base *base)
{
    int i;

    PERFINFO_AUTO_START(__FUNCTION__, 1);

        for (i = eaSize(&base->rooms)-1; i >= 0; i--)
        {
            roomAttachAuxiliary(base->rooms[i],base->rooms[i]->details);
        }
        g_bBaseUpdateAux = false;

    PERFINFO_AUTO_STOP();
}

// Constructs EArray for each type of detail, puts pointers to each
static RoomDetail ***baseCategorizeDetails(Base *base)
{
    int num_cats = eaSize(&g_DetailCategoryDict.ppCats);
    RoomDetail ***cats = calloc(num_cats,sizeof(RoomDetail**));
    int i,j,k;

    for (i = eaSize(&base->rooms)-1; i >= 0; i--)
    {
        BaseRoom *r = base->rooms[i];
        for (j = eaSize(&r->details)-1; j >= 0; j--)
        {
            for (k=num_cats-1; k >= 0; k--)
            {
                if (stricmp(r->details[j]->info->pchCategory, g_DetailCategoryDict.ppCats[k]->pchName)==0)
                {
                    eaPush(&cats[k],r->details[j]);
                    break;
                }
            }
        }
    }
    return cats;
}

// Unpowered look is for unpowered details or batteries/aux that are not in use
bool roomDetailLookUnpowered(RoomDetail *detail)
{
    if (!detail) return false;

    return (!detail->bPowered
            || !detail->bControlled
            || (detail->info->eFunction == kDetailFunction_Battery
                && !detail->bUsingBattery)
            || (IS_AUXILIARY(detail->info->eFunction)
                && !detail->iAuxAttachedID));
}

static void detailSetPowered(RoomDetail *detail, bool powered)
{
    if (detail->bPowered == powered)
        return;

    detail->bPowered = powered;
    eaPushUnique(&ppDetailRecostume,detail);
    eaPushUnique(&ppDetailUpdateActive,detail);
    if (detail->info->iControlConsume || detail->info->iControlProduce)
        g_bBaseUpdateControl = true;

    // Power down message for raids
    if(RaidIsRunning() 
       && !powered 
       && !detail->bDestroyed 
       && detail->info->pchDisplayFloatUnpowered)
    {
        RaidDisplayFloat(RaidDefendingSG(),detail->info->pchDisplayFloatUnpowered);
    }

    if(!powered && detail->e && detail->e->pchar)
        character_EnterStance(detail->e->pchar, NULL, true);

    if(IS_AUXILIARY(detail->info->eFunction) && detail->iAuxAttachedID)
    {
        RoomDetail *primary;
        BaseRoom *room = NULL;
        if(detail->parentRoom && (room=baseGetRoom(&g_base,detail->parentRoom))) 
            primary = roomGetDetail(room, detail->iAuxAttachedID);
        else 
            primary = baseGetDetail(&g_base,detail->iAuxAttachedID,NULL);
        handleAuxChange(primary,detail,detail->bControlled && detail->bPowered);
    }
}

static void detailSetControlled(RoomDetail *detail, bool controlled)
{
    if (detail->bControlled == controlled)
        return;

    detail->bControlled = controlled;
    eaPushUnique(&ppDetailRecostume,detail);
    eaPushUnique(&ppDetailUpdateActive,detail);

    if(!controlled && detail->e && detail->e->pchar)
        character_EnterStance(detail->e->pchar, NULL, true);

    if (detail->info->eFunction == kDetailFunction_Battery)
        g_bBaseUpdateEnergy = true;

    // Control down message for raids
    if(RaidIsRunning() 
       && !controlled 
       && !detail->bDestroyed 
       && detail->info->pchDisplayFloatUncontrolled)
    {
        RaidDisplayFloat(RaidDefendingSG(),detail->info->pchDisplayFloatUncontrolled);
    }

    if(IS_AUXILIARY(detail->info->eFunction) && detail->iAuxAttachedID)
    {
        RoomDetail *primary;
        BaseRoom *room = NULL;
        if(detail->parentRoom && (room=baseGetRoom(&g_base,detail->parentRoom))) 
            primary = roomGetDetail(room, detail->iAuxAttachedID);
        else 
            primary = baseGetDetail(&g_base,detail->iAuxAttachedID,NULL);
        handleAuxChange(primary,detail,detail->bControlled && detail->bPowered);
    }
}

// Called to set the destroyed state of a detail.  Should handle all implications of this state changing.
void detailSetDestroyed(RoomDetail *detail, bool destroyed, bool rebuild, bool floater)
{
    if (detail->bDestroyed != destroyed)
    {
        detail->bDestroyed = destroyed;

        // If it's destroyed, also make it unpowered and uncontrolled
        if (destroyed)
        {
            // destroyed message for raids
            if(floater && RaidIsRunning())
            {
                int i;
                if(detail->info->pchDisplayFloatDestroyed)
                    RaidDisplayFloat(RaidDefendingSG(),detail->info->pchDisplayFloatDestroyed);

                // additional chat message to everyone
                for( i = 1; i < entities_max; i++)
                {
                    if( entity_state[i] & ENTITY_IN_USE && ENTTYPE_BY_ID(i) == ENTTYPE_PLAYER )
                    {
                        Entity *e = entities[i];
                        chatSendToPlayer(e->db_id, localizedPrintf(e,"BaseDetailDestroyed",detail->info->pchDisplayName),INFO_COMBAT_SPAM,0);
                    }
                }
                
            }

            if (detail->iAuxAttachedID)
                detachAuxiliary(NULL,detail);
            detailSetControlled(detail,false);
            detailSetPowered(detail,false);
            detail->bUsingBattery = false;
            if(!rebuild)
                eaFindAndRemove(&ppDetailRecostume,detail);
        }

        if (detail->info->iControlConsume || detail->info->iControlProduce)
            g_bBaseUpdateControl = true;
        if (detail->info->iEnergyConsume || detail->info->iEnergyProduce)
            g_bBaseUpdateEnergy = true;
    }

    if(rebuild)
    {
        // Update how the entity is displayed
        eaPushUnique(&ppDetailRebuild,detail);
    }
}

// To maintain powering consistency, we sort by:
//  cost; current on/off status (details that are powered have priority); id
static int compareEnergyCost(const void* d1Data, const void* d2Data)
{
    const RoomDetail ** d1 = (const RoomDetail **)d1Data;
    const RoomDetail ** d2 = (const RoomDetail **)d2Data;
    int dif = (*d1)->info->iEnergyConsume - (*d2)->info->iEnergyConsume;
    if (dif) return dif;
    dif = (*d1)->bPowered ^ (*d2)->bPowered;
    if (dif) return (*d1)->bPowered ? -1 : 1;
    return (*d1)->id - (*d2)->id;
}

// Sorted by priority
static int compareEnergyPriority(const void* c1Data, const void* c2Data)
{
    const int * c1 = (const int *)c1Data;
    const int * c2 = (const int *)c2Data;
    return g_DetailCategoryDict.ppCats[(*c1)]->iEnergyPriority - g_DetailCategoryDict.ppCats[(*c2)]->iEnergyPriority;
}

static void baseRoomDistributeEnergy(BaseRoom *room, bool bUpkeepShutdown)
{
    int i,iEnergyTotal = 0;
    RoomDetail **ppDetailList = NULL;
    RoomDetail **ppBatteryList = NULL;
    RoomDetail **ppRoomDetails = room->details;
    int iNumDetails = ppRoomDetails?eaSize(&ppRoomDetails):0;
    int iNumBatteries = 0, iCurBattery, iUsedBatteries;
    int iEnergyAvailable;
    bool bActiveRaid = RaidIsRunning();


    if (!iNumDetails)
        return;

    // Make the list of details we still need to turn on and available batteries
    for (i=0; i<iNumDetails; i++)
    {
        RoomDetail *d = ppRoomDetails[i];
        if (d->bDestroyed)
            continue; // Skip stuff that is destroyed

        if (d->info->eFunction == kDetailFunction_Battery)
        {
            if (d->bControlled && d->bPowered)
                eaPush(&ppBatteryList,d); // Valid battery
            else if (d->bUsingBattery)
            {
                // Turn it off
                d->bUsingBattery = false;
                eaPushUnique(&ppDetailRecostume,d);
            }
        }
        else if (!d->bPowered) // Needy details
        {
            eaPush(&ppDetailList,ppRoomDetails[i]);
        }
    }

    // Sort batteries and details requesting energy
    iNumDetails = eaSize(&ppDetailList);
    qsort(ppDetailList,iNumDetails,sizeof(ppDetailList[0]),compareEnergyCost);
    iNumBatteries = eaSize(&ppBatteryList);
    qsort(ppBatteryList,iNumBatteries,sizeof(ppBatteryList[0]),compareEnergyCost);

    iEnergyAvailable = 0;
    iCurBattery = 0;
    iUsedBatteries = 0;

    i=0;

    if(bActiveRaid && !bUpkeepShutdown) // Only allow batteries to turn things on if we're raiding and the base isn't shut down
    {
        for (; i<iNumDetails; i++)
        {
            int iNeed = MAX(0,ppDetailList[i]->info->iEnergyConsume - ppDetailList[i]->iBonusEnergy);
            while (iEnergyAvailable < iNeed && iCurBattery < iNumBatteries)
            {
                // Add current battery to potential total
                iEnergyAvailable += ppBatteryList[iCurBattery]->info->iEnergyProduce;
                iEnergyAvailable += ppBatteryList[iCurBattery]->iBonusEnergy;
                iCurBattery++;
            }

            if (iEnergyAvailable < iNeed) // Unable to meet the needs of this detail
                break;

            // Note how many batteries we actually need to turn on to get this working
            iUsedBatteries = iCurBattery;

            // Use up the energy and turn on the detail
            iEnergyAvailable -= iNeed;
            detailSetPowered(ppDetailList[i],true);
            ppDetailList[i]->bUsingBattery = true;
        }
    }

    // Un-battery the details we couldn't get turned on
    for (;i<iNumDetails;i++)
    {
        ppDetailList[i]->bUsingBattery = false;
    }

    // Turn on used batteries
    for (i=0; i<iUsedBatteries; i++)
    {
        if (!ppBatteryList[i]->bUsingBattery)
        {
            ppBatteryList[i]->bUsingBattery = true;
            eaPushUnique(&ppDetailRecostume,ppBatteryList[i]);

            // This battery needs to start using up its life
            if (ppBatteryList[i]->fLifetimeLeft == 0.0)
            {
                ppBatteryList[i]->fLifetimeLeft = ppBatteryList[i]->info->iLifetime;
            }
        }
    }

    // Turn off unused batteries
    for(;i<iNumBatteries;i++)
    {
        if (ppBatteryList[i]->bUsingBattery)
        {
            ppBatteryList[i]->bUsingBattery = false;
            eaPushUnique(&ppDetailRecostume,ppBatteryList[i]);
        }
    }

    eaDestroy(&ppDetailList);
    eaDestroy(&ppBatteryList);
}

static int baseDistributeEnergy(Base *base, bool bUpkeepShutdown)
{
    RoomDetail ***pppDetails = baseCategorizeDetails(base);
    int i,j;
    int iEnergyTotal = 0, iEnergyDeficit = 0;

    static int iCats = 0;
    static int *piCatPriority = NULL;

    PERFINFO_AUTO_START(__FUNCTION__, 1);
        // Prioritize categories
        if (!iCats)
        {
            iCats = eaSize(&g_DetailCategoryDict.ppCats);
            piCatPriority = malloc(sizeof(int)*(iCats));
            for(i=0; i<iCats; i++) piCatPriority[i] = i;
            qsort(piCatPriority,iCats,sizeof(piCatPriority[0]),compareEnergyPriority);
        }

        // Calculate the energy pool available to the base from undestroyed energy
        //  producers (and turn them all on)
        iEnergyTotal = 0;
        for(j=0; j<iCats; j++)
        {
            RoomDetail **ppDetailList = pppDetails[piCatPriority[j]];
            int iCatSize = ppDetailList?eaSize(&ppDetailList):0;
            if (!iCatSize) continue;

            for(i=iCatSize-1; i>=0; i--)
            {
                // If it's an undestroyed non-battery non-aux energy-producer
                if (!ppDetailList[i]->bDestroyed
                    && ppDetailList[i]->info->iEnergyProduce != 0
                    && ppDetailList[i]->info->eFunction != kDetailFunction_Battery
                    && ppDetailList[i]->info->eFunction != kDetailFunction_AuxBoostEnergy)
                {
                    if( !bUpkeepShutdown )
                    {
                        iEnergyTotal += ppDetailList[i]->info->iEnergyProduce;
                        iEnergyTotal += ppDetailList[i]->iBonusEnergy;
                    }

                    detailSetPowered(ppDetailList[i],!bUpkeepShutdown);
                }
            }
        }
        base->iEnergyProUI = iEnergyTotal;

        // Distribute the energy pool
        for(i=0; i<iCats; i++)
        {
            RoomDetail **ppDetailList = pppDetails[piCatPriority[i]];
            int iCatSize = ppDetailList?eaSize(&ppDetailList):0;
            if (!iCatSize) continue;

            // Sort the details in this category by increasing consumption
            qsort(ppDetailList,iCatSize,sizeof(ppDetailList[0]),compareEnergyCost);

            // Distribute energy to undestroyed details that don't generate energy themselves
            for(j=0; j<iCatSize; j++)
            {
                if (!ppDetailList[j]->bDestroyed && ppDetailList[j]->info->iEnergyProduce == 0)
                {
                    int iNeed = MAX(0,ppDetailList[j]->info->iEnergyConsume - ppDetailList[j]->iBonusEnergy);
                    if (iNeed <= iEnergyTotal)
                    {
                        iEnergyTotal -= iNeed;
                        detailSetPowered(ppDetailList[j],true);
                        if (iNeed) ppDetailList[j]->bUsingBattery = false;
                    }
                    else
                    {
                        iEnergyDeficit -= iNeed;
                        detailSetPowered(ppDetailList[j],false);
                    }
                }
            }
            eaDestroy(&ppDetailList);
        }
        free (pppDetails);
        base->iEnergyConUI = base->iEnergyProUI - (iEnergyTotal + iEnergyDeficit);

        // Handle batteries and such (needs to be called even if total left is positive, to turn off batteries)
        for (i=eaSize(&base->rooms)-1;i>=0;i--)
        {
            baseRoomDistributeEnergy(base->rooms[i],bUpkeepShutdown);
        }

        g_bBaseUpdateEnergy = false;
    PERFINFO_AUTO_STOP();

    return(iEnergyTotal + iEnergyDeficit); // Returns surplus or deficit
}

// To maintain controlling consistency, we sort by:
//  cost; current on/off status (details that are controlled have priority); id
static int compareControlCost(const void* d1Data, const void* d2Data)
{
    const RoomDetail ** d1 = (const RoomDetail **)d1Data;
    const RoomDetail ** d2 = (const RoomDetail **)d2Data;
    int dif = (*d1)->info->iControlConsume - (*d2)->info->iControlConsume;
    if (dif) return dif;
    dif = (*d1)->bControlled ^ (*d2)->bControlled;
    if (dif) return (*d1)->bControlled ? -1 : 1;
    return (*d1)->id - (*d2)->id;
}

// Sorted by priority
static int compareControlPriority(const void* c1Data, const void* c2Data)
{
    const int * c1 = (const int *)c1Data;
    const int * c2 = (const int *)c2Data;
    return g_DetailCategoryDict.ppCats[(*c1)]->iControlPriority - g_DetailCategoryDict.ppCats[(*c2)]->iControlPriority;
}

// Basically the same function as energy, just uses different variables
static int baseDistributeControl(Base *base, bool bUpkeepShutdown)
{
    RoomDetail ***pppDetails = baseCategorizeDetails(base);
    int i,j;
    int iControlTotal = 0, iControlDeficit = 0;

    static int iCats = 0;
    static int *piCatPriority = NULL;

    PERFINFO_AUTO_START(__FUNCTION__, 1);
        // Prioritize categories
        if (!iCats)
        {
            iCats = eaSize(&g_DetailCategoryDict.ppCats);
            piCatPriority = malloc(sizeof(int)*(iCats));
            for(i=0; i<iCats; i++) piCatPriority[i] = i;
            qsort(piCatPriority,iCats,sizeof(piCatPriority[0]),compareControlPriority);
        }

        // Calculate the control pool available to the base from powered control
        //  producers (and control them all)
        iControlTotal = 0;
        for(j=0; j<iCats; j++)
        {
            RoomDetail **ppDetailList = pppDetails[piCatPriority[j]];
            int iCatSize = ppDetailList?eaSize(&ppDetailList):0;
            if (!iCatSize) continue;

            for(i=iCatSize-1; i>=0; i--)
            {
                // If it's a powered control-producer
                if (ppDetailList[i]->bPowered 
                    && ppDetailList[i]->info->iControlProduce != 0
                    && ppDetailList[i]->info->eFunction != kDetailFunction_AuxBoostControl)
                {
                    if( !bUpkeepShutdown )
                    {
                        iControlTotal += ppDetailList[i]->info->iControlProduce;
                        iControlTotal += ppDetailList[i]->iBonusControl;
                    }

                    detailSetControlled(ppDetailList[i],!bUpkeepShutdown);
                }
            }
        }
        base->iControlProUI = iControlTotal;

        // Distribute the control pool
        for(i=0; i<iCats; i++)
        {
            RoomDetail **ppDetailList = pppDetails[piCatPriority[i]];
            int iCatSize = ppDetailList?eaSize(&ppDetailList):0;
            if (!iCatSize) continue;

            // Sort the details in this category by increasing consumption
            qsort(ppDetailList,iCatSize,sizeof(ppDetailList[0]),compareControlCost);

            // Distribute control to undestroyed details that don't generate control themselves
            for(j=0; j<iCatSize; j++)
            {
                if (!ppDetailList[j]->bDestroyed && ppDetailList[j]->info->iControlProduce == 0)
                {
                    int iNeed = MAX(0,ppDetailList[j]->info->iControlConsume - ppDetailList[j]->iBonusControl);
                    if (iNeed <= iControlTotal)
                    {
                        iControlTotal -= iNeed;
                        detailSetControlled(ppDetailList[j],true);
                    }
                    else
                    {
                        iControlDeficit -= iNeed;
                        detailSetControlled(ppDetailList[j],false);
                    }
                }
            }
            eaDestroy(&ppDetailList);
        }
        free (pppDetails);
        base->iControlConUI = base->iControlProUI - (iControlTotal + iControlDeficit);

        g_bBaseUpdateControl = false;
    PERFINFO_AUTO_STOP();

    return(iControlTotal + iControlDeficit); // Returns surplus or deficit
}

// Returns the number of active anchors in this base
int baseCountActiveAnchors(Base *base)
{
    // This may need to be cached into a global and recalced when needed
    int i,j,count=0;
    for (i=eaSize(&base->rooms)-1;i>=0;i--)
    {
        BaseRoom *room = base->rooms[i];
        for (j=eaSize(&room->details)-1;j>=0;j--)
        {
            RoomDetail *detail = room->details[j];

            if (!detail->bDestroyed
                && detail->info->eFunction == kDetailFunction_Anchor)
            {
                count++;
            }
        }
    }
    return count;
}

RoomDetailPosition** baseGetItemsOfPower(Base *base)
{
    static RoomDetailPosition **pList = NULL;
    int i,j;
    if (pList)
        eaDestroyEx(&pList, NULL);
    for (i=eaSize(&base->rooms)-1;i>=0;i--)
    {
        BaseRoom *room = base->rooms[i];
        for (j=eaSize(&room->details)-1;j>=0;j--)
        {
            RoomDetail *detail = room->details[j];
            if (detail->info->eFunction == kDetailFunction_ItemOfPower)
            {
                RoomDetailPosition *pDetailpos = calloc(1,sizeof(RoomDetailPosition));
                pDetailpos->detail = room->details[j];
                addVec3(pDetailpos->detail->mat[3], room->pos, pDetailpos->pos);
                eaPush(&pList, pDetailpos);
            }
        }
    }
    return pList;
}


static void baseUpdateBatteries(Base *base, float fRate)
{
    int i,j;
    for (i=eaSize(&base->rooms)-1;i>=0;i--)
    {
        BaseRoom *room = base->rooms[i];
        for (j=eaSize(&room->details)-1;j>=0;j--)
        {
            RoomDetail *detail = room->details[j];
            if (detail->bUsingBattery && detail->fLifetimeLeft > 0.0 && detail->info->iEnergyProduce)
            {
                detail->fLifetimeLeft -= fRate;
                if (detail->fLifetimeLeft <= 0.0)
                {
                    detail->fLifetimeLeft = 0.0;
                    detailSetDestroyed(detail,true,false,true);
                }
            }
        }
    }
}

void basePostRaidUpdate(Base *base, bool bNoDestroy)
{
    int i,j;
    float fBaseBonus = 0.0;
    bool rebuild = false;

    if (!bNoDestroy)
    {
        // Find the base-wide bonus to repair
        for (i=eaSize(&base->rooms)-1;i>=0;i--)
        {
            BaseRoom *room = base->rooms[i];
            for (j=eaSize(&room->details)-1;j>=0;j--)
            {
                RoomDetail *detail = room->details[j];
                if(detail->info->eFunction == kDetailFunction_Workshop && detail->bPowered && detail->bControlled)
                {
                    float fShopBonus = detail->info->ppchFunctionParams ? atof(detail->info->ppchFunctionParams[0]) : 0.0;
                    RoomDetail **ppAux = findAttachedAuxiliary(room,detail);
                    if(ppAux)
                    {
                        int k;
                        for(k=eaSize(&ppAux)-1;k>=0;k--)
                        {
                            RoomDetail *pAuxDetail = ppAux[k];
                            if(pAuxDetail->bActiveAux && pAuxDetail->info->eFunction == kDetailFunction_AuxWorkshopRepair)
                                fShopBonus += pAuxDetail->info->ppchFunctionParams ?
                                    atof(pAuxDetail->info->ppchFunctionParams[0]) : 0.0;
                        }
                    }
                    fBaseBonus += fShopBonus;
                }
            }
        }
    }

    // Fix or delete all destroyed details
    for (i=eaSize(&base->rooms)-1;i>=0;i--)
    {
        BaseRoom *room = base->rooms[i];
        for (j=eaSize(&room->details)-1;j>=0;j--)
        {
            RoomDetail *detail = room->details[j];
            if(detail->bDestroyed)
            {
                float fRepairChance = detail->info->fRepairChance + fBaseBonus;
                float fRand = bNoDestroy ? 0.0 : (float)rand()/(float)RAND_MAX;

                // Add in helpful aux items if we need them
                if(fRepairChance < fRand)
                {
                    RoomDetail **ppAux = findAttachedAuxiliary(room,detail);
                    if(ppAux)
                    {
                        int k;
                        for(k=eaSize(&ppAux)-1;k>=0;k--)
                        {
                            RoomDetail *pAuxDetail = ppAux[k];
                            if(pAuxDetail->bActiveAux && pAuxDetail->info->eFunction == kDetailFunction_AuxWorkshopRepair)
                                fRepairChance += pAuxDetail->info->ppchFunctionParams ?
                                    atof(pAuxDetail->info->ppchFunctionParams[0]) : 0.0;
                        }
                    }
                }

                // Check if we managed to repair
                if (fRepairChance < fRand)
                {
                    // Remove the detail
                    roomDetailRemove(room,detail);
                    rebuild = true;
                }
                else
                {
                    // Fix the detail
                    detailSetDestroyed(detail,false,true,true);
                }
            }
            else
            {
                // Heal the undestroyed details
                if(detail->e && detail->e->pchar)
                {
                    detail->e->pchar->attrCur.fHitPoints = detail->e->pchar->attrMax.fHitPoints;
                }
            }
        }
    }

    // If there are any details that need to get rebuilt (undestroyed), do that here
    for (i=eaSize(&ppDetailRebuild)-1;i>=0;i--)
    {
        detailUpdateEntityCostume(ppDetailRebuild[i],true);
        eaFindAndRemove(&ppDetailRecostume,ppDetailRebuild[i]);
    }
    eaSetSize(&ppDetailRebuild, 0);

    if(rebuild)
    {
        baseToDefs(base, 0);
        sgroup_SaveBase(base,NULL,1);
    }
}

//This is a temporary function to handle a data bug that allowed players to have more IOP mounts than
//their plot size shold allow
bool playerIsNowInAnIllegalBase()
{
    if( g_base.rooms ) //clucky but universally used way to see if we are in a base
    {
        Base * pBase = &g_base;
        int i, maxMounts, iNumMounts=0;

        //Mounts possible
        maxMounts = baseDetailLimit( pBase, "ItemsOfPower" );

        //Mounts actual
        for(i=0; i<eaSize(&pBase->rooms); i++)
        {
            int iDetail;
            for( iDetail=0 ; iDetail < eaSize(&pBase->rooms[i]->details) ; iDetail++ )
            {
                RoomDetail *pRoomDetail = pBase->rooms[i]->details[iDetail];

                if(pRoomDetail->info->eFunction == kDetailFunction_ItemOfPowerMount)
                {
                    iNumMounts++;
                }
            }
        }

        if( iNumMounts > maxMounts )
            return true;
    }

     return false;
}

int baseSpacesForItemsOfPower(Base *pBase)
{
    int i;
    int iNumAnchors = 0;
    int iNumMounts = 0;
    const char *pchCat = NULL;
    Entity *e = FindEntityInSupergroup(pBase->supergroup_id);
    bool upkeepShutBaseDown = false;

    if(!pBase)
        return 0;

    // ----------
    // return 0 if the base has no power too

    if( e && e->supergroup )
    {
        const BaseUpkeepPeriod *upkeep = sgroup_GetUpkeepPeriod(e->supergroup);
        if( upkeep && upkeep->shutBaseDown )
        {
            return 0;
        }
    }

    for(i=0; i<eaSize(&pBase->rooms); i++)
    {
        int iDetail;
        for(iDetail=0; iDetail<eaSize(&pBase->rooms[i]->details); iDetail++)
        {
            RoomDetail *pRoomDetail = pBase->rooms[i]->details[iDetail];

            if(pRoomDetail->info->eFunction == kDetailFunction_Anchor)
            {
                if(!pchCat)
                    pchCat = pRoomDetail->info->pchCategory;

                iNumAnchors++;
            }
            else if(pRoomDetail->info->eFunction == kDetailFunction_ItemOfPowerMount ||
                    pRoomDetail->info->eFunction == kDetailFunction_ItemOfPower )
            {
                iNumMounts++;
            }
        }
    }

    if(pchCat)
    {
        for(i=0; i<eaSize(&pBase->plot->ppDetailCatLimits); i++)
        {
            if(stricmp(pchCat, pBase->plot->ppDetailCatLimits[i]->pchCatName)==0)
            {
                if(iNumAnchors>=pBase->plot->ppDetailCatLimits[i]->iLimit)
                    return iNumMounts;

                break;
            }
        }
    }

    return 0;
}

/**********************************************************************func*
 * baseAutoPlaceItemsOfPower
 *
 * DANGER! THIS FUNCTION MAY LOCK THE GIVEN SUPERGROUP. THE CALLER
 * MUST CALL sgroup_UpdateUnlock() IF THE SUPERGROUP WAS LOCKED.
 *
 * You can check sg->name[0]; if it's not empty then the supergroup was locked.
 *
 */
static bool baseAutoPlaceItemsOfPower(Base *base, Supergroup *sg)
{
    int i;
    Supergroup *sgroup = GetSupergroupForBase(base);

    PERFINFO_AUTO_START(__FUNCTION__, 1);

    // If we're given a supergroup, use it instead.
    if(sg->name[0])
        sgroup = sg;

    if(!sgroup)
        goto no_change;

    for(i=0; i<eaSize(&sgroup->specialDetails); i++)
    {
        SpecialDetail * specialDetail = sgroup->specialDetails[i];
        if(specialDetail != NULL
            && (specialDetail->iFlags & DETAIL_FLAGS_AUTOPLACE)
            && (specialDetail->iFlags & ~DETAIL_FLAGS_IN_BASE)
            && specialDetail->pDetail != NULL
            && specialDetail->pDetail->eFunction == kDetailFunction_ItemOfPower)
        {
            int j;
            const Detail *detail = specialDetail->pDetail;

            // OK, this IoP needs to be autoplaced on a IoP mount
            for (j=eaSize(&base->rooms)-1;j>=0;j--)
            {
                int k;
                BaseRoom *room = base->rooms[j];

                for(k = eaSize(&room->info->ppDetailCatLimits)-1; k>=0; k--)
                {
                    if(stricmp(room->info->ppDetailCatLimits[k]->pCat->pchName, detail->pchCategory)==0)
                    {
                        // OK, now find a mount.
                        int l;
                        for(l = eaSize(&room->details)-1; l>=0; l--)
                        {
                            RoomDetail * roomDetail = room->details[l];
                            if( roomDetail->info->eFunction == kDetailFunction_ItemOfPowerMount)
                            {
                                //We believe there is an item to place, and a place for it. But the data could be out of date
                                //If the sg is locked just make the change
                                if( sg->name[0] ) 
                                {
                                    world_modified=1;
                                    gridCacheInvalidate();
                                    room->mod_time++;
                                    detailCleanup(room->details[l]);
                                    roomDetail->info = detail;
                                    roomDetail->creation_time = specialDetail->creation_time;

                                    sg->specialDetails[i]->iFlags &= ~DETAIL_FLAGS_AUTOPLACE;
                                    sg->specialDetails[i]->iFlags |= DETAIL_FLAGS_IN_BASE;
                                    LOG_SG( sg->name, base->supergroup_id, LOG_LEVEL_VERBOSE, 0, "An IOP Base was converted into Item of Power %s",detail->pchName);
                                }
                                //Otherwise, lock the sg and rerun the function on the now up to date info.
                                else if ( verify(sgroup_LockAndLoad(base->supergroup_id, sg) ) )
                                {
                                    verify( sg->name[0] ); //verify because failure here would blow the stack 
                                    baseAutoPlaceItemsOfPower(base, sg);
                                }

                                goto base_changed;
                            }
                        }
                    }
                }
            }
        }
    }

no_change:
    PERFINFO_AUTO_STOP();
    return false;

base_changed:
    PERFINFO_AUTO_STOP();
    return true;
}

/**********************************************************************func*
 * baseAutoRemoveOldDetails
 *
 * This function will no longer lock the sg, as it cannot make any changes now
 *
 */
static bool baseAutoRemoveOldDetails(Base *base, Supergroup *sg)
{
    int i,j;
    const SpecialDetail **specialDetails;
    Supergroup *sgroup = GetSupergroupForBase(base);

    eaCreateConst(&specialDetails);

    PERFINFO_AUTO_START(__FUNCTION__, 1);

    // If we're given a supergroup, use it instead.
    if(sg->name[0])
        sgroup = sg;

    if(!sgroup)
        goto no_change;

    // all the special items we have
    for(i=0; i<eaSize(&sgroup->specialDetails); i++)
    {
        if (!sgroup->specialDetails[i] || !sgroup->specialDetails[i]->pDetail)
            continue;
        eaPushConst(&specialDetails, sgroup->specialDetails[i]->pDetail);
    }

    for (i=eaSize(&base->rooms)-1;i>=0;i--)
    {
        BaseRoom *room = base->rooms[i];
        for (j=eaSize(&room->details)-1;j>=0;j--)
        {
            RoomDetail *detail = room->details[j];
            if (detail->info->bSpecial)
            {
                if (eaFindAndRemove(&specialDetails,detail->info) == -1)
                {
                    // Not found in special list, must delete

                    if (detail->info->eFunction == kDetailFunction_ItemOfPower)
                    {
                        // turn an item of power back into a mount
                        const Detail *mount = basedata_GetDetailByName("Base_ItemofPowerBase");
                        LOG_SG( sg->name, base->supergroup_id, LOG_LEVEL_VERBOSE, 0, sgroup->name, base->supergroup_id, "Item of Power %s converted into a base",detail->info->pchName);
                        world_modified=1;
                        gridCacheInvalidate();
                        room->mod_time++;
                        detailCleanup(room->details[j]);

                        room->details[j]->info = mount;
                        goto base_changed;
                    }
                    else
                    {
                        // We used to remove the detail here, but that lead to data loss
                        LOG_SG( sg->name, base->supergroup_id, LOG_LEVEL_VERBOSE, 0, sgroup->name, base->supergroup_id, "Detail %s present in base but not in special detail list, something is out of sync",detail->info->pchName);
                    }
                }
            }
        }
    }

no_change:
    eaDestroyConst(&specialDetails);
    PERFINFO_AUTO_STOP();
    return false;

base_changed:
    eaDestroyConst(&specialDetails);

    PERFINFO_AUTO_STOP();
    return true;
}

/**********************************************************************func*
 * baseCheckSpecialActivated
 *
 * DANGER! THIS FUNCTION MAY LOCK THE GIVEN SUPERGROUP. THE CALLER
 * MUST CALL sgroup_UpdateUnlock() IF THE SUPERGROUP WAS LOCKED.
 *
 * You can check sg->name[0]; if it's not empty then the supergroup was locked.
 *
 */
static bool baseCheckSpecialActivated(Base *pBase, Supergroup *sg)
{
    int i;
    Supergroup *sgroup = GetSupergroupForBase(pBase);

    PERFINFO_AUTO_START(__FUNCTION__, 1);

    // If we're given a supergroup, use it instead.
    if(sg->name[0])
        sgroup = sg;

    if(!sgroup)
        goto end;

    for(i=0; i<eaSize(&pBase->rooms); i++)
    {
        int iDetail;
        for(iDetail=0; iDetail<eaSize(&pBase->rooms[i]->details); iDetail++)
        {
            RoomDetail *pRoomDetail = pBase->rooms[i]->details[iDetail];

            if(pRoomDetail->info->bSpecial)
            {
                bool isActivated = true;
                SpecialDetail *p;
                U32 flags;

                if (roomDetailLookUnpowered(pRoomDetail))
                    isActivated = false; //it's not powered or controlled

                if (pRoomDetail->info->eFunction == kDetailFunction_AuxTeleBeacon
                    && !pRoomDetail->iAuxAttachedID)
                {
                    isActivated = false; //it needs to be attached to function, but isn't
                }

                p = FindSpecialDetail(sgroup, pRoomDetail->info, pRoomDetail->creation_time);
                if(!p)
                    continue;

                flags = p->iFlags;
                if (isActivated)
                    flags |= DETAIL_FLAGS_ACTIVATED;
                else
                    flags &= ~DETAIL_FLAGS_ACTIVATED;

                if (flags != p->iFlags)
                {
                    if(*sg->name || verify(sgroup_LockAndLoad(pBase->supergroup_id, sg)))
                    {                        
                        sgroup = sg;
                        p = FindSpecialDetail(sgroup, pRoomDetail->info, pRoomDetail->creation_time);
                        if(p)
                            p->iFlags = flags;
                    }
                    // THIS FUNCTION HAS LOCKED THE SUPERGROUP, THE CALLER
                    // MUST CALL sgroup_UpdateUnlock()!!
                }
            }
        }
    }

end:
    PERFINFO_AUTO_STOP();
    return false;
}


/**********************************************************************func*
 * baseSystemsTick_PhaseOne
 *
 */
void baseSystemsTick_PhaseOne(Base *base, float fRate)
{
    bool upkeepShutBaseDown = false;

    int idSgrp = SGBaseId();
    Supergroup *sg = sgrpFromDbId(idSgrp); // get the cached supergroup

    // See if we're shutting it down
    if( sg )
    {
        const BaseUpkeepPeriod *upkeep = sgroup_GetUpkeepPeriod(sg);
        if( upkeep )
        {
            upkeepShutBaseDown = upkeep->shutBaseDown;
        }
    }

    if (RaidIsRunning())
    {
        baseUpdateBatteries(base, fRate);
    }
    else
    {
        if (g_bBaseUpdateAux)
            baseAttachAllAuxiliary(base);
    }

    if (g_bBaseUpdateEnergy)
        baseDistributeEnergy(base,upkeepShutBaseDown);
    if (g_bBaseUpdateControl)
        baseDistributeControl(base,upkeepShutBaseDown);

    // Apparently some batteries need control, and since control updates happen after energy,
    //  we might have to go back and redo all the energy.  Awesome!
    if (g_bBaseUpdateEnergy)
        baseDistributeEnergy(base,upkeepShutBaseDown);
}

static int s_sumEntPrestige;

/**********************************************************************func*
 * s_badBasePrestigeFix_EntSumCallback
 * callback for the sum of the ents
 *
 */
static void s_badBasePrestigeFix_EntSumCallback(Packet *pak,int db_id,int count)
{
    int i;
    
    s_sumEntPrestige = 0;
    
    for( i = 0; i < count; ++i ) 
    {
        s_sumEntPrestige += pktGetBitsPack(pak, 1);
    }
}


/***************************************************************************/
void baseCostAndUpkeep(int *cost_out, int *upkeep_out)
{
    int i, cost = 0, upkeep = 0;

    // add plot
    if(g_base.plot)
    {
        // it turns out that this is stored with the base itself
        // but we should use the latest data for cost instead (like we do 
        // everywhere below)
        const BasePlot *p = g_base.plot;
        cost += p->iCostPrestige;
        upkeep += p->iUpkeepPrestige;
    }

    // rooms
    for( i = eaSize( &g_base.rooms ) - 1; i >= 0; --i)
    {
        int j;
        BaseRoom *room = g_base.rooms[i];

        if( !room )
            continue;            //BREAK IN FLOW

        if( room->info )
            cost += room->info->iCostPrestige;

        // room details
        for( j = eaSize( &room->details) - 1; j >= 0; --j)
        {
            RoomDetail *rd = room->details[j]; 
            if(!rd || !rd->info)
                continue; // BREAK IN FLOW

            cost += rd->info->iCostPrestige;
            upkeep += rd->info->iUpkeepPrestige;
        }
    }

    if(cost_out)
        *cost_out = cost;
    if(upkeep_out)
        *upkeep_out = upkeep;
}

/**********************************************************************func*
 * sgroup_UpdatePrestigeBaseAndUpkeep 
 * NOTE: only locked version for now
 * todo: move to SgrpServer.c after hotfix.
 */
void sgroup_UpdatePrestigeBaseAndUpkeep(Supergroup *sg, int idSgrp, bool locked)
{
    if( sg && verify(locked) )
    {
        Supergroup sgroup = {0};
        int presBase = 0; 
        int presAddedUpkeep = 0;
        bool needChange = false;

        baseCostAndUpkeep(&presBase, &presAddedUpkeep);

        // --------------------
        // commit new data

        if(presBase != sg->prestigeBase || presAddedUpkeep != sg->prestigeAddedUpkeep)
        {
            LOG_SG(  sg->name, idSgrp, LOG_LEVEL_IMPORTANT, 0, "changed prestige from %i to %i, upkeep from %i to %i", sg->prestigeBase, presBase, sg->prestigeAddedUpkeep, presAddedUpkeep);
            sg->prestigeBase = presBase;
            sg->prestigeAddedUpkeep = presAddedUpkeep;
        }
    }
}

/**********************************************************************func*
 * Bad base prestige fix 
 *
 */
static void s_sgroup_badBasePrestigeFix(Base *base)
{
    static bool baseFixed = false;
    Supergroup sgroup = {0};
    Supergroup *sg = NULL;
    if(!base || baseFixed || !base->supergroup_id)
        return;

    sg = sgrpFromDbId(base->supergroup_id);
    if(sg && (sg->flags & SG_FLAGS_FIXED_BAD_BASE_PRESTIGE))
    {
        // supergroup found and already fixed. 
        baseFixed = true;
    }
    else if( sgroup_LockAndLoad( base->supergroup_id, &sgroup ))
    {
        int idSgrp = base->supergroup_id;

        // point to the proper supergroup
        sg = &sgroup;

        // extra check, just in case
        if(!(sg->flags & SG_FLAGS_FIXED_BAD_BASE_PRESTIGE))
        {
            static PerformanceInfo* perfInfo;
            bool success = false;
            char *search = NULL;
            
            // update the prestigeBase value
            sgroup_UpdatePrestigeBaseAndUpkeep(sg, idSgrp, true);
            
            // ----------
            // query the entity prestige
            
            estrCreate(&search);
            estrPrintf(&search, "WHERE SupergroupsId = %d", idSgrp);
            s_sumEntPrestige = 0;
            dbReqCustomData(CONTAINER_ENTS, "Ents", 0, search, "Prestige", s_badBasePrestigeFix_EntSumCallback, 0);
            
            success = dbMessageScanUntil(__FUNCTION__, &perfInfo);
            estrDestroy(&search);        
            
            if( success )
            {
                int iPrestigeSumEnts = s_sumEntPrestige;
                int iPrestigeBonus = SG_PRESTIGE_BONUS_PER_MEMBER * sg->prestigeBonusCount;
                int iPrestigeAdjusted = iPrestigeSumEnts + iPrestigeBonus - sg->prestigeBase;
            
                if( iPrestigeAdjusted > sg->prestige )
                {
                    sg->prestige = iPrestigeAdjusted;
                }
            }
            else
            {
                LOG_SG( sg->name, base->supergroup_id, LOG_LEVEL_VERBOSE, 0, sg->name, idSgrp, "request ents failed to fix prestige");
            }
            // ----------
            // finally
            
            // only set the flag if it succeeded
            if(success)
                sg->flags |= SG_FLAGS_FIXED_BAD_BASE_PRESTIGE;

            baseFixed = true;
        }
        sgroup_UpdateUnlock(idSgrp, &sgroup);
    }
}

/**********************************************************************func*
 * baseSystemsTick
 *
 * Main function for updating the state of the base systems
 *
 */
void baseSystemsTick(float fRate)
{
    static float fTimePassed = 0.0;
    float fBaseTick = 0.0;
    Base *base = &g_base;
    int i;

    if (!g_base.rooms)
        return; //don't run base tick if we don't have a base

    PERFINFO_AUTO_START(__FUNCTION__, 1);
        fTimePassed += fRate;
        if(fTimePassed > BASE_UPDATE_INTERVAL)
        {
            fBaseTick = fTimePassed;
            fTimePassed = 0.0f;
        }

        // here's the plan: 
        // - this gets called the first tick, before any editing can occur
        // - if design lowers the cost of items in the base, than the value of
        //   the base should be less than the value of the 'prestigebase' field
        // - if this is the case, debit the difference to supergroup prestige
        if(!g_base.base_prestige_balanced)
        {
            Supergroup sgroup = {0};
            if(sgroup_LockAndLoad( base->supergroup_id, &sgroup))
            {
                int cost;
                
                baseCostAndUpkeep(&cost,NULL);
                if(cost < sgroup.prestigeBase)
                {
                    int diff = MAX(sgroup.prestigeBase - cost,0);
                    char *capped = "";
                    if(MAX_INFLUENCE - sgroup.prestige < diff) 
                    {
                        diff = MAX_INFLUENCE - sgroup.prestige;
                        capped = "(capped)";
                    }
                    LOG_SG( sgroup.name, base->supergroup_id, LOG_LEVEL_IMPORTANT, 0,  "balancing base prestige. base is now worth %i, was worth %i, giving %i to sgroup %s who has %i prestige now", cost, sgroup.prestigeBase, diff, capped, sgroup.prestige);
                    sgroup.prestige += diff;
                }
                sgroup_UpdateUnlock(base->supergroup_id, &sgroup);
            }
            else // failed to lock sg
            {
                LOG_SG(  sgroup.name, base->supergroup_id, LOG_LEVEL_IMPORTANT, 0,"failed to load supergroup %i", base->supergroup_id);
            }
            g_base.base_prestige_balanced = TRUE;
        }
        

        if (fRate == 0.0 || (fBaseTick>0.0)) // Called with 0.0 means an update is demanded
        {
            Supergroup *sg;
            Supergroup sgCommit = {0}; // if sgCommit gets filled in, then the SG got locked and changed
            bool bBaseUpdate = false;  // if true, then the base needs to be committed

            baseSystemsTick_PhaseOne(base, fBaseTick);
            baseAuxiliaryTick(base, fBaseTick);

            // one time fixup of the bad base prestige
            s_sgroup_badBasePrestigeFix(base);

            // Put in place to allow a dummy raid to fix its supergroup once, instead
            //  of always deleting the special details.
            {
                static char bDummyRaidFix = 1;
                if (bDummyRaidFix)
                {
                    if(server_state.dummy_raid_base)
                    {
                        sgroup_ValidateSpecialDetails(base, 0);
                    }
                    bDummyRaidFix = 0;
                }
            }

            //TotalLoops is an ugly little safeguard against infinite loops because I'm adding this so late
            {
                bool bPlacedAnotherItemOfPower = 0;
                int iTotalLoops = 0;
                do
                {
                    bPlacedAnotherItemOfPower = baseAutoPlaceItemsOfPower( base, &sgCommit );
                    bBaseUpdate |= bPlacedAnotherItemOfPower;
                    iTotalLoops++;
                }
                while( bPlacedAnotherItemOfPower && iTotalLoops < 6 );
            }
            {
                bool bRemovedAnotherOldDetail = 0;
                int iTotalLoops = 0;
                do
                {
                    bRemovedAnotherOldDetail = baseAutoRemoveOldDetails( base, &sgCommit );
                    bBaseUpdate |= bRemovedAnotherOldDetail;
                    iTotalLoops++;
                }
                while( bRemovedAnotherOldDetail && iTotalLoops < 6 );
            }

            bBaseUpdate = baseCheckSpecialActivated(base, &sgCommit) || bBaseUpdate;

            sg = GetSupergroupForBase(&g_base);

            if (sg && (baseSpacesForItemsOfPower(base) != sg->spacesForItemsOfPower))
                bBaseUpdate = true;        

            if(bBaseUpdate)
            {
                baseToDefs(base, 0);

                if(*sgCommit.name || verify(sgroup_LockAndLoad(base->supergroup_id, &sgCommit)))
                {
                    sgroup_SaveBase(base, &sgCommit, 1);
                }
            }

            if(sgCommit.name[0])
            {
                sgroup_UpdateUnlock(base->supergroup_id, &sgCommit);
            }

            // ----------
            // upkeep

            sg = GetSupergroupForBase(&g_base);
            if( sg )
            {
                int nUpkeepsLate = sgroup_nUpkeepPeriodsLate(sg);

                if( g_base.nUpkeepsLate != nUpkeepsLate )
                {
                    // there has been a change in the upkeep periods
                    // recalc power and control distribution
                    g_bBaseUpdateEnergy = true;
                    g_bBaseUpdateControl = true;

                    // update current upkeep
                    g_base.nUpkeepsLate = nUpkeepsLate;
                    
                }
                // Check if the background music music has changed.
				if (stricmp(g_base.music, sg->music) != 0)
				{
					strcpy(g_base.music, sg->music);
					g_base.musicTrack++;
				}
            }
        }

        PERFINFO_AUTO_START("Update costumes", 1);
            // Update costumes
            for (i=eaSize(&ppDetailRebuild)-1;i>=0;i--)
            {
                if(ppDetailRebuild[i])
                {
                    detailUpdateEntityCostume(ppDetailRebuild[i],true);
                    eaFindAndRemove(&ppDetailRecostume,ppDetailRebuild[i]);
                }
            }
            for (i=eaSize(&ppDetailRecostume)-1;i>=0;i--)
            {
                if(ppDetailRecostume[i])
                    detailUpdateEntityCostume(ppDetailRecostume[i],false);
            }
            for (i=eaSize(&ppDetailUpdateActive)-1;i>=0;i--)
            {
                if(ppDetailUpdateActive[i])
                    detailBehaviorUpdateActive(ppDetailUpdateActive[i]);
            }

            eaSetSize(&ppDetailRebuild, 0);
            eaSetSize(&ppDetailRecostume, 0);
            eaSetSize(&ppDetailUpdateActive, 0);
        PERFINFO_AUTO_STOP();
    PERFINFO_AUTO_STOP();
}

#endif // SERVER

/* End of File */
