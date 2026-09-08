/***************************************************************************
 *     Copyright (c) 2003-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include <utilitieslib/assert/assert.h>
#include <stdlib.h>
#include <float.h>

#include <utilitieslib/components/earray.h>
#include <utilitieslib/utils/timing.h>

#include "entity.h"
#include "entPlayer.h"
#include "entworldcoll.h" // for entHeight
#include "ai/entai.h"
#include "entityRef.h"
#include <utilitieslib/components/genericlist.h>
#include <utilitieslib/utils/textparser.h>
#include "entity/entGameActions.h" // for serveDamage
#include "gameComm/sendToClient.h"   // inexplicably for sendInfoBox
#include "gameComm/Npc.h"
#include "dbcomm/dbcomm.h"
#include "dbghelper.h"
#include "player/pl_stats.h" // for stat_...
#include "cmdparse/cmdcommon.h"
#include "entVarUpdate.h" // for COMBAT_UPDATE_INTERVAL
#include "player/badges_server.h"
#include "language/langServerUtil.h"
#include "Reward.h" // for rewardFindDefAndApplyToEnt

#include "classes.h"
#include "powers.h"
#include "character_base.h"
#include "character_level.h"
#include "entity/character_animfx.h"
#include "entity/character_combat.h"
#include "entity/character_pet.h"
#include "character_karma.h"
#include "character_target.h"
#include "character_combat_eval.h"
#include "character_mods.h"
#include "entity/combat_mod.h"
#include "PowerInfo.h"
#include "attrib_names.h"
#include "player/pmotion.h"
#include "teamCommon.h"
#include "powers.h"

#include "attribmod.h"
#include "storyarc/mission.h"

#include <utilitieslib/utils/eval.h>
#include "gameComm/automapServer.h"
#include "storyarc/pnpcCommon.h" // for g_visionPhaseNames
#include "seq/seq.h"

#ifdef SERVER
#include "cmdparse/cmdserver.h"
#include <utilitieslib/utils/log.h>
#include "script/ZoneEvents/ScriptedZoneEvent.h"
#include "character_eval.h"
#include "ailib/aiBehaviorPublic.h"
#include "TeamReward.h"
#include "script/Scripthook/ScriptHookCallbacks.h"
#endif 

MP_DEFINE(AttribMod);
MP_DEFINE(AttribModEvalInfo);

static const char **combatPhaseNames = 0;

/**********************************************************************/
/**********************************************************************/

TokenizerParseInfo ParseCharacterAspects[] =
{
    { "Cur", TOK_IGNORE, offsetof(CharacterAttribSet, pattrMod)  },
    { "Max", TOK_IGNORE, offsetof(CharacterAttribSet, pattrMax)  },
    { "Str", TOK_IGNORE, offsetof(CharacterAttribSet, pattrStrength)  },
    { "Res", TOK_IGNORE, offsetof(CharacterAttribSet, pattrResistance)  },
    { "Abs", TOK_IGNORE, offsetof(CharacterAttribSet, pattrAbsolute)  },
    { 0 }
};

extern TokenizerParseInfo ParseCharacterAttributes[];
char const* dbg_GetParseName(TokenizerParseInfo atpi[] , int offset)
{
    int i = 0;
    while(atpi[i].name != NULL)
    {
        if(atpi[i].storeoffset == offset)
        {
            // HACK!
            if(atpi!=ParseCharacterAspects)
            {
                return dbg_AttribName(offset, atpi[i].name);
            }
            else
            {
                return atpi[i].name;
            }
        }
        i++;
    }

    if(atpi==ParseCharacterAttributes)
    {
        if(offset==kSpecialAttrib_Translucency)
        {
            return "(translucency)";
        }
        else if(offset==kSpecialAttrib_EntCreate)
        {
            return "(ent create)";
        }
        else if(offset==kSpecialAttrib_ClearDamagers)
        {
            return "(clear damagers)";
        }
        else if(offset==kSpecialAttrib_SilentKill)
        {
            return "(silent kill)";
        }
        else if(offset==kSpecialAttrib_XPDebtProtection)
        {
            return "(xp debt protection)";
        }
        else if(offset==kSpecialAttrib_SetMode)
        {
            return "(set mode)";
        }
        else if(offset==kSpecialAttrib_SetCostume)
        {
            return "(set costume)";
        }
        else if(offset==kSpecialAttrib_Glide)
        {
            return "(glide)";
        }
        else if(offset==kSpecialAttrib_Avoid)
        {
            return "(avoid)";
        }
        else if(offset==kSpecialAttrib_Reward)
        {
            return "(reward)";
        }
        else if(offset==kSpecialAttrib_XPDebt)
        {
            return "(xp debt)";
        }
        else if(offset==kSpecialAttrib_DropToggles)
        {
            return "(drop toggles)";
        }
        else if(offset==kSpecialAttrib_GrantPower)
        {
            return "(grant power)";
        }
        else if(offset==kSpecialAttrib_RevokePower)
        {
            return "(revoke power)";
        }
        else if(offset==kSpecialAttrib_UnsetMode)
        {
            return "(unset mode)";
        }
        else if(offset==kSpecialAttrib_GlobalChanceMod)
        {
            return "(global chance mod)";
        }
        else if(offset==kSpecialAttrib_PowerChanceMod)
        {
            return "(power chance mod)";
        }
        else if(offset==kSpecialAttrib_GrantBoostedPower)
        {
            return "(grant boosted power)";
        }
        else if(offset==kSpecialAttrib_ViewAttrib)
        {
            return "ViewAttributes";
        }            
        else if(offset==kSpecialAttrib_RewardSource)
        {
            return "(rewardsource)";
        }
        else if(offset==kSpecialAttrib_RewardSourceTeam)
        {
            return "(rewardsourceteam)";
        }
        else if(offset==kSpecialAttrib_ClearFog)
        {
            return "(clearfog)";
        }
        else if(offset==kSpecialAttrib_CombatPhase)
        {
            return "CombatPhase";
        }
        else if(offset==kSpecialAttrib_CombatModShift)
        {
            return "CombatModShift";
        }
        else if(offset==kSpecialAttrib_RechargePower)
        {
            return "RechargePower";
        }
        else if(offset==kSpecialAttrib_VisionPhase)
        {
            return "VisionPhase";
        }
        else if(offset==kSpecialAttrib_NinjaRun)
        {
            return "NinjaRun";
        }
        else if(offset==kSpecialAttrib_Walk)
        {
            return "Walk";
        }
        else if(offset==kSpecialAttrib_BeastRun)
        {
            return "BeastRun";
        }
        else if(offset==kSpecialAttrib_SteamJump)
        {
            return "SteamJump";
        }
        else if(offset==kSpecialAttrib_ExclusiveVisionPhase)
        {
            return "ExclusiveVisionPhase";
        }
        else if(offset==kSpecialAttrib_DesignerStatus)
        {
            return "DesignerStatus";
        }
        else if(offset==kSpecialAttrib_SetSZEValue)
        {
            return "SetSZEValue";
        }
        else if(offset==kSpecialAttrib_AddBehavior)
        {
            return "AddBehavior";
        }
        else if(offset==kSpecialAttrib_PowerRedirect)
        {
            return "PowerRedirect";
        }
        else if(offset==kSpecialAttrib_MagicCarpet)
        {
            return "MagicCarpet";
        }
        else if(offset==kSpecialAttrib_TokenAdd)
        {
            return "TokenAdd";
        }
        else if(offset==kSpecialAttrib_TokenSet)
        {
            return "TokenSet";
        }
        else if(offset==kSpecialAttrib_TokenClear)
        {
            return "TokenClear";
        }
        else if(offset==kSpecialAttrib_ParkourRun)
        {
            return "ParkourRun";
        }
    }

    return "(unknown)";
}

static void ReportAttribMod(Character *pchar, float fMag, float fRate, AttribMod *pmod)
{
    Entity *eSrc = erGetEnt(pmod->erSource);

    float fTmp;

    const char *pchSrc = eSrc?eSrc->name:NULL;
    const char *pchDest = pchar->entParent->name;
    const char *pchFloat = pmod->ptemplate->pchDisplayFloat;

    size_t offAttrib = pmod->ptemplate->offAttrib;
    size_t offAspect = pmod->ptemplate->offAspect;
    int translateSrcName = 0;
    int translateDestName = 0;
    int statusEffectFlipper = 1;        //    status effects (not hp or end) work a bit different
                                        //    -hp or -end means you "hurt" the opponent
                                        //    -status effect means you "helped" (or buffed their res)
                                        //    so we flip the mag when reporting to spam

    // Fix up Hitpoints and Endurance which most people think of as numbers,
    // not percentages. (Gosh, what a mess this is.)
    if(IS_HITPOINTS(offAttrib))
    {
        if(offAspect==offsetof(CharacterAttribSet, pattrMod))
        {
            // TODO: Technically, this is just an estimate
            fMag *= pchar->attrMax.fHitPoints;
        }
    }
    else if(offAttrib==offsetof(CharacterAttributes, fEndurance)
        && offAspect==offsetof(CharacterAttribSet, pattrMod))
    {
        // TODO: Technically, this is just an estimate
        fMag *= pchar->attrMax.fEndurance;
    }
    // STATUS EFFECTS
    else if( (offAttrib>=offsetof(CharacterAttributes, fConfused) && offAttrib<=offsetof(CharacterAttributes, fOnlyAffectsSelf)) ||
        (offAttrib>=offsetof(CharacterAttributes, fKnockup) && offAttrib<=offsetof(CharacterAttributes, fRepel)) )
    {
        statusEffectFlipper = -1;
    }

    if (!eSrc)
        translateSrcName = 1;
    else if(ENTTYPE(eSrc)==ENTTYPE_CRITTER && (pchDest==NULL || pchSrc[0]=='\0'))
    {
        pchSrc = npcDefList.npcDefs[eSrc->npcIndex]->displayName;
        translateSrcName = 1;
    }

    fTmp = ((int)(fMag*100+0.5f))/100.0f;

    if(ENTTYPE(pchar->entParent)==ENTTYPE_CRITTER && (pchDest==NULL || pchDest[0]=='\0'))
    {
        pchDest = npcDefList.npcDefs[pchar->entParent->npcIndex]->displayName;
        translateDestName = 1;
    }

    if(pmod->ptemplate->pchDisplayVictimHit && (!pmod->ptemplate->bDisplayTextOnlyIfNotZero || fTmp != 0.0f))
    {
        char message[1024] = {0};
        Entity * owner = erGetEnt(pchar->entParent->erOwner);
        owner = owner ? owner : pchar->entParent;
        if( ENTTYPE(owner) == ENTTYPE_PLAYER )
        {
            if (pmod->ptemplate->offAttrib != kSpecialAttrib_DesignerStatus)
            {
                sendCombatMessage(pchar->entParent, (fTmp * statusEffectFlipper) < 0 ? INFO_DAMAGE : INFO_HEAL, CombatMessageType_DamagingMod, 1, pmod->ptemplate->ppowBase->crcFullName, pmod->ptemplate->ppowBaseIndex, pchSrc ? pchSrc : "UnknownSource", fTmp < 0 ? -fTmp : fTmp, 0.f);
            }
        }
    }

    if(pmod->ptemplate->pchDisplayAttackerHit && eSrc && (!pmod->ptemplate->bDisplayTextOnlyIfNotZero || fTmp != 0.0f) )
    {
        char message[1024] = {0};
        Entity * owner = erGetEnt(eSrc->erOwner);
        owner = owner ? owner : eSrc;
        if( ENTTYPE(owner) == ENTTYPE_PLAYER)
        {
            sendCombatMessage(eSrc, (fTmp * statusEffectFlipper) < 0 ? INFO_COMBAT : INFO_HEAL_OTHER, CombatMessageType_DamagingMod, 0, pmod->ptemplate->ppowBase->crcFullName, pmod->ptemplate->ppowBaseIndex, pchDest, fTmp < 0 ? -fTmp : fTmp, 0.f);
        }
    }

    if(IS_HITPOINTS(offAttrib))
    {
        if (offAttrib == offsetof(CharacterAttributes, fAbsorb)
            ? offAspect == offsetof(CharacterAttribSet, pattrMax)
            : offAspect == offsetof(CharacterAttribSet, pattrMod)
                || offAspect == offsetof(CharacterAttribSet, pattrAbsolute))
        {
            // This artificially inflates the numbers a bit when they round, but I like that better than 0s.
            int iAmt = (int)fMag;
            if(iAmt == 0)
            {
                if(fMag<-FLT_EPSILON)
                    iAmt = -1;
                else if(fMag>FLT_EPSILON)
                    iAmt = 1;
            }
            else if(iAmt < 0)
            {
                iAmt = (int)(fMag-0.5);
            }
            else
            {
                iAmt = (int)(fMag+0.5);
            }

            if(iAmt && pmod->ptemplate->bShowFloaters)
                serveDamage(pchar->entParent, eSrc, -iAmt, pmod->bUseLocForReport?pmod->vecLocation:0, pmod->ptemplate->offAttrib == offsetof(CharacterAttributes, fAbsorb));

            // All the rest is for balancing
            // These numbers are approximate.
            {
                Power *ppow;
                if(fMag<0 && pchar->attrCur.fHitPoints>0)
                {
                    pchar->stats.fDamageReceived += -fMag;
                    if(eSrc && eSrc->pchar)
                    {
                        eSrc->pchar->stats.fDamageGiven += -fMag;
                        if((ppow = character_OwnsPower(eSrc->pchar, pmod->ptemplate->ppowBase))!=NULL)
                        {
                            ppow->stats.fDamageGiven += -fMag;
                            if (isDevelopmentMode() && !isSharedMemory(ppow->ppowBase))
                                (cpp_const_cast(BasePower*)(ppow->ppowBase))->stats.fDamageGiven += -fMag;
                        }
                    }
                }
                else if(fMag>0 && pchar->attrCur.fHitPoints<pchar->attrMax.fHitPoints)
                {
                    pchar->stats.fHealingReceived += fMag;
                    if(eSrc && eSrc->pchar)
                    {
                        eSrc->pchar->stats.fHealingGiven += fMag;
                        if((ppow = character_OwnsPower(eSrc->pchar, pmod->ptemplate->ppowBase))!=NULL)
                        {
                            ppow->stats.fHealingGiven += fMag;
                            if (isDevelopmentMode() && !isSharedMemory(ppow->ppowBase))
                                (cpp_const_cast(BasePower*)(ppow->ppowBase))->stats.fHealingGiven += fMag;
                        }
                    }
                }
            }
        }
    }

    if(pchFloat && (!pmod->ptemplate->bDisplayTextOnlyIfNotZero || fTmp != 0.0f)) // If there's one specified, always show it (unless we request not to if it's zero)
    {
        char message[1024] = {0};
        if (character_combatGetLocalizedMessage(message, ARRAY_SIZE(message), pchar->entParent, translateSrcName, translateDestName, pchFloat, pchDest,
            pmod->ptemplate->ppowBase->pchDisplayName,
            fTmp < 0 ? -fTmp : fTmp,
            pchSrc?pchSrc:"UnknownSource"))
        {
            serveDamageMessage(pchar->entParent, eSrc, message, pmod->bUseLocForReport?pmod->vecLocation:0, pmod->ptemplate->offAttrib == offsetof(CharacterAttributes, fAbsorb));
        }
    }

}

/**********************************************************************/
/**********************************************************************/

void attribMod_decrementRefCount(AttribMod *pmod)
{
#if SERVER
    if (!pmod || !pmod->parentPowerInstance)
        return;

    pmod->parentPowerInstance->attribModRefCount--;
    if (pmod->parentPowerInstance->attribModRefCount <= 0 && pmod->parentPowerInstance->bHasBeenDeleted)
    {
        eaFindAndRemove(&g_deletedPowers, pmod->parentPowerInstance);
        if (pmod->parentPowerInstance->ppowBase)
        {
            LOG_OLD( "DeletedPower: Removed %s, new g_deletedPowers array size: %d", pmod->parentPowerInstance->ppowBase->pchName, eaSize(&g_deletedPowers));
        }
        power_Destroy(pmod->parentPowerInstance, NULL);
    }
#endif
}

/**********************************************************************func*
 * attribmod_Create
 *
 */
AttribMod *attribmod_Create(void)
{
    MP_CREATE(AttribMod, 1000);
    return MP_ALLOC(AttribMod);
}


/**********************************************************************func*
 * attribmod_Destroy
 *
 */
void attribmod_Destroy(AttribMod *pmod)
{
    if (!pmod)
        return;

    if(pmod->evalInfo)
    {
        attribModEvalInfo_Destroy(pmod->evalInfo);
        pmod->evalInfo = NULL;
    }

#if SERVER
    if (pmod->parentPowerInstance)
    {
        attribMod_decrementRefCount(pmod);
    }
#endif

    MP_FREE(AttribMod, pmod);
}

AttribModEvalInfo *attribModEvalInfo_Create(void)
{
    AttribModEvalInfo *evalInfo;

    MP_CREATE(AttribModEvalInfo, 100);
    evalInfo = MP_ALLOC(AttribModEvalInfo);
    evalInfo->refCount = 0;

    return evalInfo;
}

void attribModEvalInfo_Destroy(AttribModEvalInfo *evalInfo)
{
    if(evalInfo)
    {    
        devassertmsg(evalInfo->refCount > 0, "Free of already freed eval info");
        evalInfo->refCount--;

        if(evalInfo->refCount <= 0)
        {
            MP_FREE(AttribModEvalInfo, evalInfo);
        }
    }
}

void attribmod_SetParentPowerInstance(AttribMod *pmod, Power *ppow)
{
    if (!pmod || !ppow)
        return;

#if SERVER
    if (pmod->parentPowerInstance)
    {
        attribMod_decrementRefCount(pmod);
    }

    ppow->attribModRefCount++;
#endif

    pmod->parentPowerInstance = ppow;
}

/**********************************************************************func*
 * modlist_GetNextMod
 *
 */
AttribMod *modlist_GetNextMod(AttribModListIter *piter)
{
    assert(piter!=NULL);

    piter->pposCur = piter->pposNext;
    if(piter->pposNext!=NULL)
    {
        piter->pposNext = piter->pposNext->next;
    }

    return piter->pposCur;
}

/**********************************************************************func*
 * modlist_GetFirstMod
 *
 */
AttribMod *modlist_GetFirstMod(AttribModListIter *piter, AttribModList *plist)
{
    assert(plist!=NULL);

    piter->plist = plist;
    piter->pposNext = piter->plist->phead;

    return modlist_GetNextMod(piter);
}

/**********************************************************************func*
 * modlist_RemoveCurrentMod
 *
 */
void modlist_RemoveAndDestroyCurrentMod(AttribModListIter *piter)
{
    assert(piter!=NULL);
    assert(piter->pposCur!=NULL);

    if (piter->pposCur == piter->plist->ptail)
    {
        piter->plist->ptail = piter->plist->ptail->prev;
    }

    listRemoveMember(piter->pposCur, &piter->plist->phead);
    attribmod_Destroy(piter->pposCur);
}

/**********************************************************************func*
 * modlist_AddMod
 *
 */
void modlist_AddMod(AttribModList *plist, AttribMod *pmod)
{
    assert(plist!=NULL);
    assert(pmod!=NULL);

    listAddForeignMember(&plist->phead, pmod);

    if (!plist->ptail)
        plist->ptail = pmod;
}

// Tails aren't supported by GenericList, so I'm adding this code at this level instead of there
void modlist_AddModToTail(AttribModList *plist, AttribMod *pmod)
{
    assert(plist != NULL);
    assert(pmod != NULL);

    pmod->prev = plist->ptail;
    pmod->next = NULL;

    if (plist->ptail)
        plist->ptail->next = pmod;

    plist->ptail = pmod;

    if (!plist->phead)
        plist->phead = pmod;
}

void modlist_RemoveAndDestroyMod(AttribModList *plist, AttribMod *pmod)
{
    assert(plist!=NULL);
    assert(pmod!=NULL);

    if (plist->ptail == pmod)
    {
        plist->ptail = plist->ptail->prev;
    }

    listRemoveMember(pmod, &plist->phead);
    attribmod_Destroy(pmod);
}

AttribMod* modlist_FindEarlierMod(AttribModList *plist, AttribMod *modA, AttribMod *modB)
{
    AttribMod *iterMod;

    assert(plist != NULL);

    iterMod = plist->phead;

    while (iterMod != NULL)
    {
        if (iterMod == modA)
            return modA;

        if (iterMod == modB)
            return modB;

        iterMod = iterMod->next;
    }

    return NULL;
}

void modlist_MoveModToTail(AttribModListIter *piter, AttribMod *pmod)
{
    assert(piter != NULL);
    assert(pmod != NULL);

    // just... don't try to move the currently iterated mod.  I'm not sure what to do if you try...
    assert(pmod != piter->pposCur);

    if (pmod == piter->plist->ptail)
        return; // already there

    listRemoveMember(pmod, &piter->plist->phead); // don't destroy it!
    modlist_AddModToTail(piter->plist, pmod);

    if (pmod == piter->pposNext || !piter->pposNext)
        piter->pposNext = piter->pposCur->next;
}

/**********************************************************************func*
 * modlist_RemoveAll
 *
 */
void modlist_RemoveAll(AttribModList *plist)
{
    assert(plist!=NULL);

    listDestroyList(&plist->phead, attribmod_Destroy);
    plist->ptail = NULL;
}

AttribMod *modlist_FindUnsuppressed(AttribModList *plist, const AttribModTemplate *ptemplate, Power *ppower, Character *pSrc)
{
    EntityRef er = pSrc ? erGetRef(pSrc->entParent) : 0;
    AttribMod *pmod;
    AttribModListIter iter;
    bool templateStacking;

    for(pmod = modlist_GetFirstMod(&iter, plist);
        pmod!=NULL;
        pmod = modlist_GetNextMod(&iter))
    {
        if (pmod->bSuppressedByStacking)
            continue;

        if (ptemplate->iStackKey == -1)
        {
            templateStacking = (pmod->ptemplate == ptemplate);
        }
        else
        {
            templateStacking = (pmod->ptemplate->iStackKey == ptemplate->iStackKey);
        }

        if ((!er || pmod->erSource==er) && templateStacking)
        {
            if (!ptemplate->bMatchExactPower || ppower == pmod->parentPowerInstance)
                return pmod;
        }
    }

    return NULL;
}

AttribMod *modlist_FindLargestMagnitudeSuppressed(AttribModList *plist, const AttribModTemplate *ptemplate, Power *ppower, Character *pSrc)
{
    EntityRef er = pSrc ? erGetRef(pSrc->entParent) : 0;
    AttribMod *pmod;
    AttribMod *largestMod = NULL;
    AttribModListIter iter;
    bool templateStacking;

    for(pmod = modlist_GetFirstMod(&iter, plist);
        pmod!=NULL;
        pmod = modlist_GetNextMod(&iter))
    {
        if (!pmod->bSuppressedByStacking)
            continue;

        if (ptemplate->iStackKey == -1)
        {
            templateStacking = (pmod->ptemplate == ptemplate);
        }
        else
        {
            templateStacking = (pmod->ptemplate->iStackKey == ptemplate->iStackKey);
        }

        if ((!er || pmod->erSource==er) && templateStacking)
        {
            if (!ptemplate->bMatchExactPower || ppower == pmod->parentPowerInstance)
            {
                if (!largestMod || ABS(pmod->fMagnitude) > ABS(largestMod->fMagnitude))
                {
                    largestMod = pmod;
                }
            }
        }
    }

    return largestMod;
}

int modlist_Count(AttribModList *plist, const AttribModTemplate *ptemplate, Power *ppower, Character *pSrc)
{
    int count = 0;
    EntityRef er = pSrc ? erGetRef(pSrc->entParent) : 0;
    AttribMod *pmod;
    AttribModListIter iter;
    bool templateStacking;

    for(pmod = modlist_GetFirstMod(&iter, plist);
        pmod!=NULL;
        pmod = modlist_GetNextMod(&iter))
    {
        if (ptemplate->iStackKey == -1)
        {
            templateStacking = (pmod->ptemplate == ptemplate);
        }
        else
        {
            templateStacking = (pmod->ptemplate->iStackKey == ptemplate->iStackKey);
        }

        if ((!er || pmod->erSource==er) && templateStacking)
        {
            if (!ptemplate->bMatchExactPower || ppower == pmod->parentPowerInstance)
                count++;
        }
    }

    return count;
}

static AttribMod *modlist_GetNextMatchingMod(AttribModListIter *iter, const AttribModTemplate *ptemplate, Power *ppower, Character *pSrc)
{
    EntityRef er = pSrc ? erGetRef(pSrc->entParent) : 0;
    AttribMod *pmod;

    if(!iter)
        return NULL;

    for(pmod = modlist_GetNextMod(iter); pmod != NULL; pmod = modlist_GetNextMod(iter))
    {
        if((!er || pmod->erSource==er) && pmod->ptemplate==ptemplate)
        {
            if (ptemplate->bMatchExactPower)
            {
                if (ppower == pmod->parentPowerInstance)
                    return pmod;
            }
            else
                return pmod;
        }
    }

    return NULL;
}

static AttribMod *modlist_GetFirstMatchingMod(AttribModListIter *iter, AttribModList *plist, const AttribModTemplate *ptemplate, Power *ppower, 
                                                Character *pSrc)
{
    assert(plist!=NULL);

    iter->plist = plist;
    iter->pposNext = iter->plist->phead;

    return modlist_GetNextMatchingMod(iter, ptemplate, ppower, pSrc);
}

void modlist_RefreshAllMatching(AttribModList *plist, const AttribModTemplate *ptemplate, Power *ppower, Character *pSrc, float fDuration)
{
    AttribMod *pmod;
    AttribModListIter iter;

    if(!plist)
        return;

    pmod = modlist_GetFirstMatchingMod(&iter, plist, ptemplate, ppower, pSrc);

    while(pmod != NULL)
    {
        // Refresh the duration of the matching mod; this could shorten the mod if fDuration < pmod->fDuration
        pmod->fDuration = fDuration;

        pmod = modlist_GetNextMatchingMod(&iter, ptemplate, ppower, pSrc);
    }
}

/**********************************************************************func*
 * modlist_CancelNonContinuous
 *
 */
void modlist_CancelAll(AttribModList *plist, Character *pchar)
{
    AttribMod *pmod;
    AttribModListIter iter;

    for(pmod = modlist_GetFirstMod(&iter, plist); pmod != NULL; pmod = modlist_GetNextMod(&iter))
    {
        if (pmod && pmod->ptemplate && pmod->ptemplate->ppowBase && pmod->ptemplate->ppowBase->bShowBuffIcon && pchar && pchar->entParent)
        {
            pchar->entParent->team_buff_update = true;
        }
        mod_CancelFX(pmod,pchar);
        mod_Cancel(pmod,pchar);
        modlist_RemoveAndDestroyCurrentMod(&iter);
    }
}

/**********************************************************************/
/**********************************************************************/

/**********************************************************************func*
 * mod_Fill
 *
 */
void mod_Fill(AttribMod *pmod, Character *pTarget, Character *pSrc,
    unsigned int uiInvokeID, Power *ppow, const AttribModTemplate *ptemplate,
    const Vec3 vec, float fTimeBeforeHit, float fDelay, float fEffectiveness,
    float fToHitRoll, float fToHit, float fChance)
{
    float f;
    float fVal;
    float fStr = 1.0f;
    int iDelta;
    CombatMod *pcm;
    int iEffCombatLevel;
    const char *token;
    assert(pmod!=NULL);
    assert(pSrc!=NULL);
    assert(ppow!=NULL);
    assert(ptemplate!=NULL);

    pmod->ptemplate = ptemplate;
    pmod->uiTint = power_IsTinted(ppow) ? power_GetTint(ppow) : colorPairNone;
    pmod->fx = power_GetFX(ppow);
    if (ppow && ppow->psetParent && ppow->psetParent->pcharParent && 
        ppow->psetParent->pcharParent->ppowCreatedMe) // Is the character a pet?
    {
        Power* creationPower = power_GetPetCreationPower(ppow->psetParent->pcharParent);
        token = power_GetToken(creationPower);
    }
    else
        token = power_GetToken(ppow);
    pmod->customFXToken = token;
    pmod->erSource = erGetRef(pSrc->entParent);
    pmod->erCreated = 0;
    pmod->uiInvokeID = uiInvokeID;
    pmod->bReportedOnLastTick = false;
    pmod->bContinuingFXOn = false;
    pmod->bIgnoreSuppress = false;
    pmod->fChance = fChance;
    attribmod_SetParentPowerInstance(pmod, ppow);

    // if(pmod->ptemplate->ppowBase->eType == kPowerType_Click)
    // {
    //     verbose_printf("mod_Fill:          %s.%s\n", ppow->ppowBase->pchName, ptemplate->pchName);
    //     verbose_printf("mod_Fill:          %s\n", dbg_GetParseName(ParseCharacterAttributes, ptemplate->offAttrib));
    // }

    // If we're ignoring combat mods, then tweak with the effective level
    // of the attack.
    if(pSrc->bIgnoreCombatMods)
    {
        // If the source is a scary monster, then they always hit at the
        // level of the target.
        // DGNOTE 8/2/2010 However if the big scary monster has mod level shift that makes it even scarier, we apply that to make them hit harder
        iEffCombatLevel = pTarget->iCombatLevel + pSrc->ignoreCombatModLevelShift;
        // Except we clamp at zero because I suspect huge amounts of the combat system would do a major technicolor yawn on
        // a negative effective combat level.
        if (iEffCombatLevel < 0)
        {
            iEffCombatLevel = 0;
        }
    }
    else if(pTarget->bIgnoreCombatMods)
    {
        // If the source is hitting a scary monster, then hit at most at the
        // level of the monster. This keeps high level players from ganking
        // low level scary monsters.

        // The design group doesn't ALWAYS want the above to happen, though.
        // Pets and other self-targeting-ish (though not literally self-targetting)
        // AttribMods should stay at the caster's combat level.
        // I'm not too happy using these bools for this test, but Design says this
        // is exactly what is meant when they set them. I'm think there might
        // be some sloppy thinking there, but I'm rolling with it for now.
        if(!ptemplate->bUseDurationCombatMods && !ptemplate->bUseMagnitudeCombatMods)
        {
            iEffCombatLevel = pSrc->iCombatLevel;
        }
        else
        {
            iEffCombatLevel = min(pSrc->iCombatLevel, pTarget->iCombatLevel);
            // DGNOTE 8/2/2010 However if the big scary monster has mod level shift that makes it even scarier, we apply that to nerf the player
            iEffCombatLevel = iEffCombatLevel - pTarget->ignoreCombatModLevelShift;
            // Clamp at zero.
            if (iEffCombatLevel < 0)
            {
                iEffCombatLevel = 0;
            }
        }
    }
    else
    {
        // Otherwise, this is a regular attack.
        iEffCombatLevel = pSrc->iCombatLevel;
    }

    if(ptemplate->iVarIndex<0)
    {
        fVal = class_GetNamedTableValue(pSrc->pclass, ptemplate->pchTable, iEffCombatLevel);
        // if(pmod->ptemplate->ppowBase->eType == kPowerType_Click)
        //     verbose_printf("mod_Fill:          NamedTable{%s}[%d] = %6.3f\n", ptemplate->pchTable, iEffCombatLevel, fVal);
    }
    else
    {
        fVal = ppow->afVars[ptemplate->iVarIndex];
    }

    f = fVal * fEffectiveness;

#ifdef SERVER
    // hard-code the duration of all mezzes in pvp zones to be no more than 4 seconds long
    if (server_visible_state.isPvP && ptemplate->eType == kModType_Duration
        && ptemplate->offAttrib >= offsetof(CharacterAttributes, fConfused)
        && ptemplate->offAttrib <= offsetof(CharacterAttributes, fSleep)
        && ptemplate->fScale > 4.0f
        && ENTTYPE(pTarget->entParent) == ENTTYPE_PLAYER)
    {
        f *= 4.0f;
    }
    else
    {
        f *= ptemplate->fScale;
    }
#else
    f *= ptemplate->fScale;
#endif

    // if(pmod->ptemplate->ppowBase->eType == kPowerType_Click)
    //     verbose_printf("mod_Fill:          Scaled by       %6.3f to %6.3f\n", ptemplate->fScale, f);

    if(ptemplate->bAllowStrength && ptemplate->offAttrib<sizeof(CharacterAttributes))
    {
        fStr = *(ATTRIB_GET_PTR(ppow->pattrStrength, ptemplate->offAttrib));
        f *= fStr;
        // if(pmod->ptemplate->ppowBase->eType == kPowerType_Click)
        //     verbose_printf("mod_Fill:          Strengthened by %6.3f to %6.3f\n", fStr, f);
    }

    if(vec!=NULL)
    {
        copyVec3(vec, pmod->vecLocation);
    }

    switch(ptemplate->eType)
    {
        case kModType_Duration:
            pmod->fMagnitude = ptemplate->fMagnitude;
            pmod->fDuration = f;
            break;

        case kModType_Magnitude:
            pmod->fMagnitude = f;
            pmod->fDuration = ptemplate->fDuration;
            break;

        case kModType_SkillMagnitude:
            pmod->fMagnitude = f;
            f = 1+(fToHit-fToHitRoll)*4;
            if(f>1.0f) f=1.0f;
            if(f<0.0f) f=0.0f;
            pmod->fMagnitude *= f;

            pmod->fDuration = ptemplate->fDuration;
            break;

        case kModType_Expression:
            if(!ptemplate->bDelayEval)
            {
                if(ptemplate->evalFlags & ATTRIBMOD_EVAL_CALC_INFO)
                    combateval_StoreAttribCalcInfo(f, fVal, ptemplate->fScale, fEffectiveness, fStr);

                if(ptemplate->ppchMagnitude)
                {
                    pmod->fMagnitude = combateval_Eval(pSrc->entParent, pTarget->entParent, ppow, ptemplate->ppchMagnitude, ppow->ppowBase->pchSourceFile);
                }
                else
                {
                    pmod->fMagnitude = ptemplate->fMagnitude;
                }
            
                if(ptemplate->ppchDuration)
                {
                    pmod->fDuration = combateval_Eval(pSrc->entParent, pTarget->entParent, ppow, ptemplate->ppchDuration, ppow->ppowBase->pchSourceFile);
                }
                else
                {
                    pmod->fDuration = ptemplate->fDuration;
                }
            }
            else
            {
                // We are delaying evaluation of the expression until mod_process and storing the relevant eval information til then
                if(!pmod->evalInfo)
                {
                    pmod->evalInfo = attribModEvalInfo_Create();
                    pmod->evalInfo->refCount++;
                }

                pmod->evalInfo->fFinal = f;
                pmod->evalInfo->fVal = fVal;
                pmod->evalInfo->fEffectiveness = fEffectiveness;
                pmod->evalInfo->fStrength = fStr;

                pmod->fDuration = ptemplate->fDuration;
                pmod->fMagnitude = ptemplate->fMagnitude;
            }
            break;

        case kModType_Constant:
            pmod->fMagnitude = ptemplate->fMagnitude;
            pmod->fDuration = ptemplate->fDuration;
            break;
    }

    // if(pmod->ptemplate->ppowBase->eType == kPowerType_Click)
    //     verbose_printf("mod_Fill:          Mag: %6.3f  Dur: %6.3f\n", pmod->fMagnitude, pmod->fDuration);

    // OK, now modify these values depending on the combat mods
    if(ptemplate->bUseDurationCombatMods || ptemplate->bUseMagnitudeCombatMods)
    {
        if(!combatmod_Ignored(pSrc, pTarget))
        {
            combatmod_Get(pSrc, iEffCombatLevel + pSrc->iCombatModShift, pTarget->iCombatLevel + pTarget->iCombatModShift, &pcm, &iDelta);
        }
        else
        {
            combatmod_Get(pSrc, iEffCombatLevel, iEffCombatLevel, &pcm, &iDelta);
        }

        if(ptemplate->bUseMagnitudeCombatMods)
            pmod->fMagnitude *= combatmod_Magnitude(pcm, iDelta);

        if(ptemplate->bUseDurationCombatMods)
            pmod->fDuration *= combatmod_Duration(pcm, iDelta);
    }

    // if(pmod->ptemplate->ppowBase->eType == kPowerType_Click)
    //     verbose_printf("mod_Fill: postmod  Mag: %6.3f  Dur: %6.3f\n", pmod->fMagnitude, pmod->fDuration);

    // For powers which spawn things, if the magnitude is <0 then base
    // the magnitude on the character's combat level
    if(pmod->fMagnitude<0.0f && ptemplate->pchEntityDef!=NULL)
    {
        pmod->fMagnitude = iEffCombatLevel+1;
    }

    // These are increased by fDelay because this calculation is done before
    // the actual "hit". It's done immediately after the InterruptTime passes.
    pmod->fDuration += pmod->ptemplate->fDelay + fDelay;
    pmod->fTimer = pmod->ptemplate->fDelay + ((ptemplate->offAttrib == kSpecialAttrib_SetMode && !ppow->ppowBase->iTimeToConfirm) ? 0: fDelay);
    pmod->fCheckTimer = fTimeBeforeHit>pmod->fTimer ? pmod->fTimer : fTimeBeforeHit;
    pmod->bSourceChecked = false;
    pmod->bDelayEvalChecked = !pmod->ptemplate->bDelayEval;
}

/**********************************************************************func*
 * mod_IsSuppressed
 *
 */
static bool mod_IsSuppressed(AttribMod *pmod, Character *pchar, U32 ulNow)
{
    bool bSuppressed = false;

    if (pmod->bSuppressedByStacking)
        return true; // early out so you don't need to bother running the rest of the function.

    if(!pmod->bIgnoreSuppress)
    {
        int i;
        int n = eaSize(&pmod->ptemplate->ppSuppress);

        for(i=0; i<n; i++)
        {
            int iEvent = pmod->ptemplate->ppSuppress[i]->idxEvent;
            U32 ul = pmod->ptemplate->ppSuppress[i]->ulSeconds + pchar->aulEventTimes[iEvent];

            if(POWEREVENT_INVOKE(iEvent)
                && pmod->ptemplate->ppowBase == pchar->ppowLastActivated)
            {
                continue;
            }
            if(POWEREVENT_APPLY(iEvent)
                && pmod->uiInvokeID == pchar->uiLastApplied)
            {
                continue;
            }

            if(ulNow < ul)
            {
                bSuppressed = true;
                break;
            }
        }

        // If we didn't suppress it this tick, and we don't want to check it every tick
        //  just flag it to permanently ignore suppression
        if (!bSuppressed && !pmod->ptemplate->bSuppressMethodAlways)
            pmod->bIgnoreSuppress = true;
    }

    return bSuppressed;
}


/**********************************************************************func*
 * character_DropRandomToggles
 *
 */
static void character_DropRandomToggles(Character *pchar, int iNumToDrop)
{
    PowerListItem *ppowListItem;
    PowerListIter iter;
    Power **pppowsFirst;
    Power **pppowsMiddle;
    Power **pppowsLast;

    eaCreate(&pppowsFirst);
    eaCreate(&pppowsMiddle);
    eaCreate(&pppowsLast);

    for(ppowListItem = powlist_GetFirst(&pchar->listTogglePowers, &iter);
        ppowListItem != NULL;
        ppowListItem = powlist_GetNext(&iter))
    {
        switch(ppowListItem->ppow->ppowBase->eToggleDroppable)
        {
            case kToggleDroppable_Always:
                power_ChangeActiveStatus(ppowListItem->ppow, false);
                sendInfoBox(pchar->entParent, INFO_DAMAGE, "ToggleKnockedOff", ppowListItem->ppow->ppowBase->pchDisplayName);
                iNumToDrop--;
                break;

            case kToggleDroppable_Sometimes:
                eaPush(&pppowsMiddle, ppowListItem->ppow);
                break;

            case kToggleDroppable_First:
                eaPush(&pppowsFirst, ppowListItem->ppow);
                break;

            case kToggleDroppable_Last:
                eaPush(&pppowsLast, ppowListItem->ppow);
                break;

            case kToggleDroppable_Never:
                // Don't add it to any list.
                break;
        }
    }

    while(eaSize(&pppowsFirst) && iNumToDrop>0)
    {
        int i = rule30Rand()%eaSize(&pppowsFirst);

        power_ChangeActiveStatus(pppowsFirst[i], false);
        sendInfoBox(pchar->entParent, INFO_DAMAGE, "ToggleKnockedOff", pppowsFirst[i]->ppowBase->pchDisplayName);
        eaRemove(&pppowsFirst, i);

        iNumToDrop--;
    }
    eaDestroy(&pppowsFirst);

    while(eaSize(&pppowsMiddle) && iNumToDrop>0)
    {
        int i = rule30Rand()%eaSize(&pppowsMiddle);

        power_ChangeActiveStatus(pppowsMiddle[i], false);
        sendInfoBox(pchar->entParent, INFO_DAMAGE, "ToggleKnockedOff", pppowsMiddle[i]->ppowBase->pchDisplayName);
        eaRemove(&pppowsMiddle, i);

        iNumToDrop--;
    }
    eaDestroy(&pppowsMiddle);

    while(eaSize(&pppowsLast) && iNumToDrop>0)
    {
        int i = rule30Rand()%eaSize(&pppowsLast);

        power_ChangeActiveStatus(pppowsLast[i], false);
        sendInfoBox(pchar->entParent, INFO_DAMAGE, "ToggleKnockedOff", pppowsLast[i]->ppowBase->pchDisplayName);
        eaRemove(&pppowsLast, i);

        iNumToDrop--;
    }
    eaDestroy(&pppowsLast);

}

/**********************************************************************func*
 * HandleSpecialAttrib
 *
 */
static void damageTrackerDestroyAdapter(void* arg0)
{
    damageTrackerDestroy((DamageTracker*)arg0);
}

void HandleSpecialAttrib(AttribMod *pmod, Character *pchar, float f, bool *pbDisallowDefeat)
{
    int priority = 5;

    switch(pmod->ptemplate->offAttrib)
    {
        case kSpecialAttrib_Null:
            // Do nothing.
            break;

        case kSpecialAttrib_Translucency:
            if(pchar->entParent->xlucency < 0
                || f < pchar->entParent->xlucency)
            {
                pchar->entParent->xlucency = f;
            }
            break;

        case kSpecialAttrib_EntCreate:
            if(pmod->ptemplate->pchEntityDef != NULL)
            {
                // Spawn a pet
                character_CreatePet(pchar, pmod);
            }
            break;

        case kSpecialAttrib_ClearDamagers:
            eaClearEx(&pchar->entParent->who_damaged_me, damageTrackerDestroyAdapter);
            break;

        case kSpecialAttrib_SilentKill:
            eaPushConst(&pchar->ppLastDamageFX, pmod->fx);
            pchar->attrCur.fHitPoints = -99999;
            pchar->erLastDamage = 0;
            if(pbDisallowDefeat)
            {
                *pbDisallowDefeat = false;
            }
            break;

        case kSpecialAttrib_XPDebtProtection:
            // Do nothing.
            break;

        case kSpecialAttrib_SetMode:
            character_SetPowerMode(pchar, pmod->ptemplate->fMagnitude);
            break;

        case kSpecialAttrib_SetCostume:
            if (pmod->ptemplate->pchParams != NULL)
                priority = atoi(pmod->ptemplate->pchParams);
            character_SetCostume(pchar, pmod->ptemplate->pchCostumeName, priority); 
            break;

        case kSpecialAttrib_Glide:
        case kSpecialAttrib_NinjaRun:
        case kSpecialAttrib_Walk:
        case kSpecialAttrib_BeastRun:
        case kSpecialAttrib_SteamJump:
        case kSpecialAttrib_HoverBoard:
        case kSpecialAttrib_MagicCarpet:
        case kSpecialAttrib_ParkourRun:
            setSpecialMovement(pchar->entParent, (int)pmod->ptemplate->offAttrib, true);
            break;

        case kSpecialAttrib_Avoid:
            // Do nothing.
            break;

        case kSpecialAttrib_Reward:
            {
                Entity *pentSrc = erGetEnt(pmod->erSource);
                Entity *pentSrcOwner = pentSrc ? (erGetEnt(pentSrc->erOwner) ? erGetEnt(pentSrc->erOwner) : pentSrc) : NULL;
                int iVillainGroup = VG_NONE;
                int iLevel = (f < 0.0) ? (pchar->iLevel + 1) : (int)f;

                if(pentSrcOwner && pentSrcOwner->villainDef)
                {
                    iVillainGroup = pentSrcOwner->villainDef->group;
                }

                rewardFindDefAndApplyToEnt(pchar->entParent, (const char**)eaFromPointerUnsafe(pmod->ptemplate->pchReward), iVillainGroup, iLevel, true, REWARDSOURCE_ATTRIBMOD, NULL);
            }
            break;

        case kSpecialAttrib_RewardSource:
            {
                Entity *pentSrc = erGetEnt(pmod->erSource);
                Entity *pentSrcOwner = pentSrc ? (erGetEnt(pentSrc->erOwner) ? erGetEnt(pentSrc->erOwner) : pentSrc) : NULL;
                int iVillainGroup = VG_NONE;
                int iLevel = 0;

                if (pentSrcOwner)
                {
                    iLevel = (f < 0.0) ? pentSrcOwner->pchar->iLevel + 1 : (int) f;
                    rewardFindDefAndApplyToEnt(pentSrcOwner, (const char**)eaFromPointerUnsafe(pmod->ptemplate->pchReward), VG_NONE, iLevel, false, REWARDSOURCE_ATTRIBMOD, NULL);
                }
            }
            break;
        case kSpecialAttrib_RewardSourceTeam:
            {
                Entity *pentSrc = erGetEnt(pmod->erSource);
                Entity *pentSrcOwner = pentSrc ? (erGetEnt(pentSrc->erOwner) ? erGetEnt(pentSrc->erOwner) : pentSrc) : NULL;
                Entity *pTeamMember = NULL;
                int iVillainGroup = VG_NONE;
                int iLevel = 0;
                int i, teamSize;

                if (pentSrcOwner)
                {
                    iLevel = (f < 0.0) ? pentSrcOwner->pchar->iLevel + 1 : (int) f;
                    rewardFindDefAndApplyToEnt(pentSrcOwner, (const char**)eaFromPointerUnsafe(pmod->ptemplate->pchReward), VG_NONE, iLevel, false, REWARDSOURCE_ATTRIBMOD, NULL);
                    if (pentSrcOwner->teamup && pentSrcOwner->teamup->members.count > 1)
                    {
                        teamSize = pentSrcOwner->teamup->members.count;
                        for (i = 0; i < teamSize; i++)
                        {
                            if (pentSrcOwner->teamup->members.ids[i] != pentSrcOwner->db_id)
                            {
                                rewardFindDefAndApplyToDbID(pentSrcOwner->teamup->members.ids[i], (const char**)eaFromPointerUnsafe(pmod->ptemplate->pchReward), VG_NONE, 
                                                            iLevel, 0, REWARDSOURCE_ATTRIBMOD);
                            }
                        }
                    }
                }
            }
            break;
        case kSpecialAttrib_ClearFog:
            {
                mapperClearFogForCurrentMap(pchar->entParent);
            }
            break;
        case kSpecialAttrib_XPDebt:
            if(pmod->ptemplate->offAspect==offsetof(CharacterAttribSet, pattrAbsolute))
            {
                pchar->iExperienceDebt += (int)f;
            }
            else if(pmod->ptemplate->offAspect==offsetof(CharacterAttribSet, pattrMod))
            {
                pchar->iExperienceDebt = (int)((float)pchar->iExperienceDebt*f);
            }
            character_CapExperienceDebt(pchar);
            break;

        case kSpecialAttrib_DropToggles:
            character_DropRandomToggles(pchar, (int)f);
            break;

        case kSpecialAttrib_GrantPower:
            {
                const BasePower* ppowBase = NULL;
                if(pmod->ptemplate->pchReward)
                    ppowBase = powerdict_GetBasePowerByFullName(&g_PowerDictionary, pmod->ptemplate->pchReward);
                else
                    ErrorFilenamef(SAFE_MEMBER3(pmod,ptemplate,ppowBase,pchSourceFile),"Attrib is missing reward field");

                if(ppowBase)
                {
                    int i;
                    int numAdd = 1;

                    if (pmod->ptemplate->pchParams != NULL)
                        numAdd = atoi(pmod->ptemplate->pchParams);

                    for(i = 0; i < numAdd; i++)
                    {
                        Power *ppow = character_AddRewardPower(pchar, ppowBase);
                        if(ppow)
                        {
                            character_AccrueBoosts(pchar, ppow, 0.0f);
                        }
                    }
                }

                if( pmod->ptemplate->ppowBase )
                {
                    int i;
                    for( i = eaSize(&pmod->ptemplate->ppowBase->ppchAIGroups)-1; i >=0; i-- )
                    {
                        Entity *pentSrc = erGetEnt(pmod->erSource);
                        if(pentSrc)
                        {
                            if( stricmp( pmod->ptemplate->ppowBase->ppchAIGroups[i], "kMastermindUpgrade1" ) == 0)
                                character_PetsUpgraded(pentSrc->pchar, PETFLAG_UPGRADE1);
                            if( stricmp( pmod->ptemplate->ppowBase->ppchAIGroups[i], "kMastermindUpgrade2" ) == 0)
                                character_PetsUpgraded(pentSrc->pchar, PETFLAG_UPGRADE2 );
                        }
                    }
                }
            }
            break;

        case kSpecialAttrib_GrantBoostedPower:
            {
                const BasePower *ppowBase;
                ppowBase = powerdict_GetBasePowerByFullName(&g_PowerDictionary, pmod->ptemplate->pchReward);
                if(ppowBase)
                {
                    int i;
                    int numAdd = 1;

                    if (pmod->ptemplate->pchParams != NULL)
                        numAdd = atoi(pmod->ptemplate->pchParams);

                    for(i = 0; i < numAdd; i++)
                    {
                        Power *ppow = character_AddRewardPower(pchar, ppowBase);
                        if(ppow)
                        {
                            const BasePower *ppowBase = pmod->ptemplate->ppowBase;
                            Power *ppowSrc = character_OwnsPower(pchar, ppowBase);

                            if(ppowSrc)
                            {
                                character_SlotPowerFromSource(ppow, ppowSrc, 0);
                            }

                            character_AccrueBoosts(pchar, ppow, 0.0f);
                        }
                    }
                }
            }
            break;

        case kSpecialAttrib_RevokePower:
            {
                const BasePower *ppowBase;
                ppowBase = powerdict_GetBasePowerByFullName(&g_PowerDictionary, pmod->ptemplate->pchReward);
                if(ppowBase)
                {
                    int i;
                    int numRemove = 1;

                    if (pmod->ptemplate->pchParams != NULL)
                    {
                        if(stricmp(pmod->ptemplate->pchParams, "all") == 0)
                            numRemove = character_OwnsPowerNum(pchar, ppowBase);
                        else
                            numRemove = atoi(pmod->ptemplate->pchParams);
                    }

                    for(i = 0; i < numRemove; i++)
                        character_RemoveBasePower(pchar, ppowBase);
                }
            }
            break;

        case kSpecialAttrib_UnsetMode:
            character_UnsetPowerMode(pchar, pmod->ptemplate->fMagnitude);
            break;

        case kSpecialAttrib_GlobalChanceMod:
            {
                eaPushUnique(&pchar->ppmodChanceMods, pmod);
            }
            break;
        case kSpecialAttrib_ViewAttrib:
            {
                Entity *pentSrc = erGetEnt(pmod->erSource);
                if( pentSrc && ENTTYPE(pentSrc) == ENTTYPE_PLAYER )
                {
                    Entity *eTarget = pchar->entParent;
                    if( ENTTYPE(eTarget) == ENTTYPE_CRITTER &&  eTarget->pchar->iLevel <= pmod->ptemplate->fScale )
                    {
                        pentSrc->attributesTarget = erGetRef(eTarget);
                        if( pmod->ptemplate->bAllowCombatMods )
                            pentSrc->newAttribTarget = 1;
                    }
                }
            }
            break;
        case kSpecialAttrib_CombatPhase:
            {
                if (!combatPhaseNames)
                {
                    eaCreateConst(&combatPhaseNames);
                    eaInsertConst(&combatPhaseNames, "NoPrime", 0);
                    eaInsertConst(&combatPhaseNames, "Friendly", 1);
                }
                if (pmod->ptemplate && pmod->ptemplate->pchParams)
                {
                    if (pmod->fMagnitude > pchar->fMagnitudeOfBestCombatPhase)
                    {
                        pchar->fMagnitudeOfBestCombatPhase = pmod->fMagnitude;
                        pchar->bitfieldCombatPhases = 1; // reset to default, all prior CombatPhases have no effect
                    }

                    // If the mod is a "Prime" phase, don't bother changing the bitfield.
                    // These mods are allowed to reset the phase bitfield due to higher mag (above).
                    if (pmod->fMagnitude == pchar->fMagnitudeOfBestCombatPhase
                        && stricmp(pmod->ptemplate->pchParams, "Prime"))
                    {
                        int phaseNameIndex = 0;
                        while (phaseNameIndex < eaSize(&combatPhaseNames))
                        {
                            if (!stricmp(pmod->ptemplate->pchParams, combatPhaseNames[phaseNameIndex]))
                            {
                                break;
                            }
                            phaseNameIndex++;
                        }
                        if (phaseNameIndex == eaSize(&combatPhaseNames))
                        {
                            eaInsertConst(&combatPhaseNames, pmod->ptemplate->pchParams, phaseNameIndex);
                        }

                        assert(phaseNameIndex >= 0 && phaseNameIndex < 32);

                        if (phaseNameIndex == 0)
                        {
                            // Remove from prime phase.
                            pchar->bitfieldCombatPhases &= 0xfffffffe;
                        }
                        else
                        {
                            // Add to this phase.
                            pchar->bitfieldCombatPhases |= 1 << phaseNameIndex;
                        }
                    }
                }
            }
            break;
        case kSpecialAttrib_CombatModShift:
            {
                pchar->iCombatModShift += pmod->fMagnitude;
                if (!pmod->ptemplate->pchParams || stricmp(pmod->ptemplate->pchParams, "DoNotDisplayShift"))
                {
                    pchar->iCombatModShiftToSendToClient += pmod->fMagnitude;
                }
            }
            break;
        case kSpecialAttrib_RechargePower:
            {
                Entity *entity = erGetEnt(pmod->erSource);
                const BasePower *basePower;
                const BasePowerSet *baseSet;
                const PowerCategory *category;

                if (entity)
                {
                    if (!strchr(pmod->ptemplate->pchReward, '.'))
                    {
                        if (category = powerdict_GetCategoryByName(&g_PowerDictionary, pmod->ptemplate->pchReward))
                        {
                            int setIndex, powerIndex;
                            int numPowSets = eaSize(&category->ppPowerSets);

                            for (setIndex = 0; setIndex < numPowSets; setIndex++)
                            {
                                int numPowers;
                                      
                                baseSet = category->ppPowerSets[setIndex];
                                numPowers = eaSize(&baseSet->ppPowers);
                                
                                for (powerIndex = 0; powerIndex < numPowers; powerIndex++)
                                {
                                    Power *power = character_OwnsPower(entity->pchar, baseSet->ppPowers[powerIndex]);

                                    if (power)
                                    {
                                        power_ChangeRechargeTimer(power, 0);
                                    }
                                }
                            }
                        }
                    }
                    else if (strchr(pmod->ptemplate->pchReward, '.') == strrchr(pmod->ptemplate->pchReward, '.'))
                    {
                        if (baseSet = powerdict_GetBasePowerSetByFullName(&g_PowerDictionary, pmod->ptemplate->pchReward))
                        {
                            int powerIndex;
                            int count = eaSize(&baseSet->ppPowers);

                            for (powerIndex = 0; powerIndex < count; powerIndex++)
                            {
                                Power *power = character_OwnsPower(entity->pchar, baseSet->ppPowers[powerIndex]);

                                if (power)
                                {
                                    power_ChangeRechargeTimer(power, 0);
                                }
                            }
                        }
                    }
                    else
                    {
                        if (basePower = powerdict_GetBasePowerByFullName(&g_PowerDictionary, pmod->ptemplate->pchReward))
                        {
                            Power *power = character_OwnsPower(entity->pchar, basePower);

                            if (power)
                            {
                                power_ChangeRechargeTimer(power, 0);
                            }
                        }
                    }
                }
            }
            break;
        case kSpecialAttrib_VisionPhase:
            {
                if (pmod->ptemplate && pmod->ptemplate->pchParams)
                {
                    if (pchar->entParent->seq && pchar->entParent->seq->type
                        && pchar->entParent->seq->type->collisionType == SEQ_ENTCOLL_LIBRARYPIECE)
                    {
                        Errorf("Warning:  Ents that use Geo collision cannot have VisionPhase attrib mods.");
                    }
                    else
                    {
                        if (pmod->fMagnitude > pchar->entParent->fMagnitudeOfBestVisionPhase)
                        {
                            pchar->entParent->fMagnitudeOfBestVisionPhase = pmod->fMagnitude;
                            pchar->entParent->bitfieldVisionPhases = 1; // reset to default, all prior VisionPhases have no effect
                        }

                        if (pmod->fMagnitude == pchar->entParent->fMagnitudeOfBestVisionPhase)
                        {
                            int phaseNameIndex = 0;
                            while (phaseNameIndex < eaSize(&g_visionPhaseNames.visionPhases))
                            {
                                if (!stricmp(pmod->ptemplate->pchParams, g_visionPhaseNames.visionPhases[phaseNameIndex]))
                                {
                                    break;
                                }
                                phaseNameIndex++;
                            }

                            if (phaseNameIndex == eaSize(&g_visionPhaseNames.visionPhases))
                            {
                                ErrorFilenamef(pmod->ptemplate->ppowBase->pchSourceFile, "Vision Phase '%s' not found in visionPhases.def!", pmod->ptemplate->pchParams);
                            }
                            else if (phaseNameIndex >= 0 && phaseNameIndex < 32)
                            {
                                if (phaseNameIndex == 0)
                                {
                                    // NoPrime
                                    pchar->entParent->bitfieldVisionPhases &= 0xfffffffe;
                                }
                                else
                                {
                                    pchar->entParent->bitfieldVisionPhases |= 1 << phaseNameIndex;
                                }
                            }
                        }
                    }
                }
            }
            break;
        case kSpecialAttrib_ExclusiveVisionPhase:
            {
                if (pmod->ptemplate && pmod->ptemplate->pchParams)
                {
                    if (pchar->entParent->seq && pchar->entParent->seq->type
                        && pchar->entParent->seq->type->collisionType == SEQ_ENTCOLL_LIBRARYPIECE)
                    {
                        Errorf("Warning:  Ents that use Geo collision cannot have ExclusiveVisionPhase attrib mods.");
                    }
                    else
                    {
                        int phaseNameIndex = 0;
                        while (phaseNameIndex < eaSize(&g_exclusiveVisionPhaseNames.visionPhases))
                        {
                            if (!stricmp(pmod->ptemplate->pchParams, g_exclusiveVisionPhaseNames.visionPhases[phaseNameIndex]))
                            {
                                break;
                            }
                            phaseNameIndex++;
                        }

                        if (phaseNameIndex == eaSize(&g_exclusiveVisionPhaseNames.visionPhases))
                        {
                            ErrorFilenamef(pmod->ptemplate->ppowBase->pchSourceFile, "Exclusive Vision Phase '%s' not found in visionPhasesExclusive.def!", pmod->ptemplate->pchParams);
                        }
                        else
                        {
                            pchar->entParent->exclusiveVisionPhase = phaseNameIndex;
                        }
                    }
                }
            }
            break;
        case kSpecialAttrib_DesignerStatus:
            {
                eaPush(&pchar->designerStatuses, strdup(pmod->ptemplate->pchDisplayVictimHit));
            }
            break;
        case kSpecialAttrib_SetSZEValue:
            {
                int scriptID = chareval_Eval(pchar, pmod->ptemplate->ppchPrimaryStringList, pmod->parentPowerInstance ? pmod->parentPowerInstance->ppowBase->pchSourceFile : "")-1;
                if (scriptID >= 0)
                {
                    ScriptEnvironment *preserved = g_currentScript;
                    g_currentScript = ScriptFromRunId(scriptID);
                    ScriptedZoneEventEval_StoreAttribInfo(pmod->fMagnitude, pmod->fDuration);
                    ScriptedZoneEvent_SetScriptValue(pmod->ptemplate->ppchSecondaryStringList);
                    g_currentScript = preserved;
                }
                else
                {
                    Errorf("No matching script running");
                }
            }
            break;
        case kSpecialAttrib_LuaExec:
            {
                int scriptID = chareval_Eval(pchar, pmod->ptemplate->ppchPrimaryStringList, pmod->ptemplate->ppowBase->pchSourceFile) - 1;
                if (scriptID >= 0)
                {
                    ScriptEnvironment *preserved = g_currentScript;
                    g_currentScript = ScriptFromRunId(scriptID);
                    if(g_currentScript && g_currentScript->lua_interpreter)
                        lua_exec(pmod->ptemplate->ppchSecondaryStringList[0], pmod->fMagnitude, pmod->fDuration);
                    g_currentScript = preserved;
                }
                else
                {
                    Errorf("No matching script running");
                }
            }
            break;
        case kSpecialAttrib_AddBehavior:
            {
                Entity *e = pchar->entParent;
                if (e && (ENTTYPE(e) != ENTTYPE_PLAYER))
                {
                    if (eaSize(&pmod->ptemplate->ppchPrimaryStringList) != 1)
                    {
                        Errorf("Power %s has the incorrect number of Behaviors to add. It should have 1.", pmod->ptemplate->ppowBase->pchFullName);
                    }
                    else
                    {
                        aiBehaviorAddString(e, ENTAI(e)->base, pmod->ptemplate->ppchPrimaryStringList[0]);
                    }
                }
            }
            break;
        case kSpecialAttrib_TokenAdd:
            {
                Entity *e = pchar->entParent;
                if (e)
                {
                    bool updateTime = !(pmod->ptemplate->pchParams && stricmp(pmod->ptemplate->pchParams, "NoTime") == 0);

                    modifyRewardToken(e, pmod->ptemplate->pchReward, (int)pmod->ptemplate->fMagnitude, updateTime, false);
                }
            }
            break;
        case kSpecialAttrib_TokenSet:
            {
                Entity *e = pchar->entParent;
                if (e)
                {
                    bool updateTime = !(pmod->ptemplate->pchParams && stricmp(pmod->ptemplate->pchParams, "NoTime") == 0);

                    modifyRewardToken(e, pmod->ptemplate->pchReward, (int)pmod->ptemplate->fMagnitude, updateTime, true);
                }
            }
            break;
        case kSpecialAttrib_TokenClear:
            {
                Entity *e = pchar->entParent;
                if (e)
                    removeRewardToken(e, pmod->ptemplate->pchReward);
            }
            break;
        case kSpecialAttrib_ForceMove:
            {
                // You must complete your current force move before being allowed to start another.
                if (!pchar->bForceMove)
                {
                    pchar->bForceMove = true;
                    copyVec3(pmod->vecLocation, pchar->vecLocationForceMove);
                }
            }
            break;
    }
}

AttribMod *modlist_BlameDefenseMod(AttribModList *plist, float fPercentile, int iType)
{
    int i;
    AttribModListIter iter;
    AttribMod *pmod;
    AttribMod **ppDefenses=NULL;
    float fDefense=0.0f;

    if(plist==NULL)
        return NULL;

    for(pmod = modlist_GetFirstMod(&iter, plist);
        pmod!=NULL;
        pmod = modlist_GetNextMod(&iter))
    {
        int offAttrib = (int)pmod->ptemplate->offAttrib;
        if(pmod->fMagnitude>0.0f
            && (offAttrib==offsetof(CharacterAttributes, fDefense) || offAttrib==iType)
            && IS_MODIFIER(pmod->ptemplate->offAspect))
        {
            eaPush(&ppDefenses, pmod);
            fDefense += pmod->fMagnitude;
        }
    }

    fDefense *= fPercentile;

    for(i=eaSize(&ppDefenses)-1; i>=0; i--)
    {
        pmod = ppDefenses[i];
        fDefense -= pmod->fMagnitude;
        if(fDefense<=0.0f)
        {
            eaDestroy(&ppDefenses);
            return pmod;
        }
    }

    eaDestroy(&ppDefenses);

    return NULL;
}


/**********************************************************************func*
 * NotifyAI
 *
 */
static void NotifyAI(AttribMod *pmod, Character *pchar, float f, float fRate)
{
    if(!pmod->bReportedOnLastTick && 
        (pmod->ptemplate->ppowBase->eAIReport==kAIReport_Always|| 
         pmod->ptemplate->ppowBase->eAIReport==kAIReport_HitOnly || 
         pmod->ptemplate->offAttrib == offsetof(CharacterAttributes, fPlacate)))
    {
        AIEventNotifyParams params;
        const BasePower* ppowBase = pmod->ptemplate->ppowBase;

        params.notifyType = AI_EVENT_NOTIFY_POWER_SUCCEEDED;
        params.source = erGetEnt(pmod->erSource);
        params.target = pchar->entParent;
        params.powerSucceeded.offAttrib = (S32)pmod->ptemplate->offAttrib;
        params.powerSucceeded.offAspect = (S32)pmod->ptemplate->offAspect;
        params.powerSucceeded.ppowBase = ppowBase;
        params.powerSucceeded.uiInvokeID = pmod->uiInvokeID;
        params.powerSucceeded.revealSource = ppowBase->eAIReport==kAIReport_Always || ppowBase->eAIReport==kAIReport_HitOnly;

        // TODO: Make this the actual radius of the power (which might have been enhanced)
        params.powerSucceeded.fRadius = ppowBase->fRadius;

        if(pmod->fDuration>0 && pmod->ptemplate->fPeriod==0)
        {
#define COMBAT_UPDATE_INTERVAL 0.125f
            params.powerSucceeded.fAmount = (pmod->fDuration/(fRate+0.00001))*COMBAT_UPDATE_INTERVAL*30;
        }
        else
        {
            params.powerSucceeded.fAmount = f;
        }

        if(params.source)
        {
            aiEventNotify(&params);
        }
    }
}

#define MAX_TAUNT_DIST_SQR 200*200
#define TAUNT_LOS_SPEEDUP 4.0f
static void HandlePlayerTaunt(AttribMod *pmod, Character *pchar, Entity *pentSrc, float fRate)
{
    if(!pentSrc || entMode(pentSrc,ENT_DEAD))
    {
        pmod->fDuration = 0.0; // Kill the mod
    }
    else
    {
        if(distance3Squared(ENTPOS(pchar->entParent),ENTPOS(pentSrc)) > MAX_TAUNT_DIST_SQR)
        {
            // Clear mods from sources that are too far away
            pmod->fDuration = 0.0f;
        }
        else if(!character_CharacterIsReachable(pentSrc, pchar->entParent))
        {
            // Extra decrement to the duration if source isn't visible
            pmod->fDuration -= (TAUNT_LOS_SPEEDUP-1.0f)*fRate;
        }

        if(pmod->fDuration > 0
           && (!pchar->pmodTaunt || pmod->fDuration > 2.0*pchar->pmodTaunt->fDuration)
           && pmod->fMagnitude > 0 )
        {
            // New taunt
            pchar->pmodTaunt = pmod;
            pchar->entParent->target_update = 1;
            pchar->entParent->status_effects_update = true;

            // Send taunt message and floater
            sendInfoBox(pchar->entParent, INFO_COMBAT_SPAM, "YouAreTaunted");
            serveFloatingInfo(pchar->entParent,pchar->entParent,"FloatTaunted");
        }
    }
}

/**********************************************************************func*
 * CancelTauntPlayer
 *
 */
static void CancelTauntPlayer(Character *pchar, Entity *pentSrc)
{
    AttribModListIter iter;
    AttribMod *pmod = modlist_GetFirstMod(&iter, &pchar->listMod);
    EntityRef erSource = erGetRef(pentSrc);

    while(pmod!=NULL)
    {
        if(pmod->erSource == erSource 
            && IS_MODIFIER(pmod->ptemplate->offAspect) 
            && pmod->ptemplate->offAttrib == offsetof(CharacterAttributes, fTaunt))
        {
            pmod->fDuration = 0.0; // Kills the taunt mod next time around
        }

        pmod = modlist_GetNextMod(&iter);
    }
}


static void HandlePlayerPlacate(AttribMod *pmod, Character *pchar, Entity *pentSrc, float fRate)
{
    if(eaFind(&pchar->ppmodPlacates, pmod)<0)
    {
        pchar->entParent->target_update = 1;
        pchar->entParent->status_effects_update = true;

        // Send placate message and floater
        if( pmod->fMagnitude > 0 )
        {
            CancelTauntPlayer(pchar,pentSrc); // Cancels any taunts the source may have had on the target

            eaPush(&pchar->ppmodPlacates, pmod);
            sendInfoBox(pchar->entParent, INFO_COMBAT_SPAM, "YouArePlacated", pentSrc->name);
            serveFloatingInfo(pchar->entParent,pchar->entParent,"FloatPlacated");
        }
    }
}


/**********************************************************************func*
 * CancelPlacatePlayer
 *
 */
static void CancelPlacatePlayer(Character *pchar, Entity *pentSrc, unsigned int uiID)
{
    if (pchar->ppmodPlacates && pentSrc)
    {
        int i;
        EntityRef er = pentSrc->erOwner;
        if (!er) er = erGetRef(pentSrc);
        for (i = eaSize(&pchar->ppmodPlacates)-1; i >= 0; i--)
        {
            if (pchar->ppmodPlacates[i]->erSource == er && pchar->ppmodPlacates[i]->uiInvokeID != uiID)
                pchar->ppmodPlacates[i]->fDuration = 0.0;    // Kills the placate mod next time around
        }
    }
}

extern int *g_zoneEventScriptStageAwardsKarma;
/**********************************************************************func*
 * TrackHitpointChange
 *
 */
static void TrackHitpointChange(AttribMod *pmod, Character *pchar, Entity *pentSrc, float f, U32 ulNow)
{
    float fDamage = -f;
    Entity *pentSrcOwner = 0; 
    EntityRef erSrcOwner = pmod->erSource;


    if( pentSrc )
    {
        if( pentSrc->erOwner  )
            erSrcOwner = pentSrc->erOwner;

        pentSrcOwner = erGetEnt(pentSrc->erOwner); 

        // If we are on a architect map, don't credit owner for pet damage (if its not a actual pet)
        if( !pentSrcOwner || (g_ArchitectTaskForce && pentSrc->petByScript) )
            pentSrcOwner = pentSrc;
    }

    if(pmod->ptemplate->offAspect==offsetof(CharacterAttribSet, pattrMod))
    {
        // This is just an estimate.
        fDamage *= pchar->attrMax.fHitPoints;
    }

    // Do this first, so that damage and healing always trigger their respective events
    if(fDamage>0)
    {
        //    if this doesn't count as a hit
        //    don't mark it as the player was damaged
        int countAsDamaged = !pmod->ptemplate->ppowBase || !pmod->ptemplate->ppowBase->bIsEnvironmentHit;

        if (countAsDamaged)
            character_RecordEvent(pchar, kPowerEvent_Damaged, ulNow);

        // Remember who hurt me, assuming it wasn't me
        if (pchar->entParent != pentSrcOwner)
            pchar->erLastDamage = erSrcOwner;

        // If you have taken damage after the terrorize timer has expired,
        //  we set the timer to a negative value that indicates how long
        //  we can run around and use powers for
        if(pchar->fTerrorizePowerTimer == 0.0f)
        {
            pchar->fTerrorizePowerTimer = -10.0f;
        }
    }
    else
    {
        character_RecordEvent(pchar, kPowerEvent_Healed, ulNow);
    }

    // Don't let people get credit for overkilling or
    //   overhealing things
    if(fDamage > pchar->attrCur.fHitPoints)
    {
        fDamage = pchar->attrCur.fHitPoints;
    }
    else if(fDamage<pchar->attrCur.fHitPoints-pchar->attrMax.fHitPoints)
    {
        fDamage = pchar->attrCur.fHitPoints-pchar->attrMax.fHitPoints;

        // TODO: Send the current attrMax.[fHitPoints, fEndurance] to this function.
        // As stated repeatedly elsewhere in the code, attrMax is just
        //   an estimate. This is because we are busy modifying attrMax
        //   and also using it to calculate these values. Up to this
        //   point, no one has noticed that these values have been off
        //   and I don't want to rock the boat much right now, so I'm
        //   not fixing this now.
        // The side effect of attrMax being mid-change is (a) that it hasn't
        //   been clamped, (b) not all buffs are added in yet, and (c) not
        //   all debuffs are added in yet. So, it's possible for the
        //   cur.hitpoints to be bigger than the instantaneous max.hitpoints
        //   right here.
        // BUG: As it turns out, since attribmods are processed in newest to oldest
        //   order, damage is almost always going to be processed before a
        //   change in max hp.  Thus, if the damage is less than the difference
        //   between cur and base max, it will get set to 0 here, and not tracked
        //   for any badges.
        // If we get here, then we know that the healing done is greater than
        //   what can reasonably be healed. If max is bigger than cur, then
        //   it's the normal case. If max is less than cur, then we won't
        //   give out any credit.
        if(fDamage>0) fDamage=0;
    }

    // Remember what hurt me
    eaPushConst(&pchar->ppLastDamageFX, pmod->fx);

    if(pentSrcOwner!=NULL)
    {
        if(!pmod->ptemplate->ppowBase->bIsEnvironmentHit)  // don't damage track powers specifically flagged as IsEnvironmentalHit
        {
            if(character_IsConfused(pentSrcOwner->pchar))
            {
                damageTrackerUpdate(pchar->entParent, pentSrcOwner, fDamage*0.25);
            }
            else
            {
                damageTrackerUpdate(pchar->entParent, pentSrcOwner, fDamage);
            }
        }

        if(ENTTYPE(pentSrcOwner)==ENTTYPE_PLAYER)
        {
            if(pchar->entParent!=pentSrcOwner)
            {
                // Players who cannot be killed are in no risk, so they don't
                // get the benefit of the badge stats.
                if(!pchar->bCannotDie && fDamage>=0)
                {
                    stat_AddDamageGiven(pentSrcOwner->pl, (int)fDamage);
                    badge_RecordDamageGiven(pentSrcOwner, (int)fDamage);
                    if (eaiSize(&g_zoneEventScriptStageAwardsKarma))
                    {
                        //    if attacker is a player, and the victim is not
                        if ((ENTTYPE(pentSrcOwner) == ENTTYPE_PLAYER) && (ENTTYPE(pchar->entParent) != ENTTYPE_PLAYER))
                        {
                            CharacterKarmaActivateBubble(pentSrcOwner->pchar, g_currentKarmaTime);
                        }
                    }
                }
                else if(!pchar->bCannotDie && pmod->ptemplate->offAttrib==g_offHealAttrib)
                {
                    // Only give healing credit when healing someone
                    //   your own team.
                    stat_AddDamageGiven(pentSrcOwner->pl, (int)fDamage);
                    badge_RecordHealingGiven(pentSrcOwner, (int)fDamage);
                }
            }
        }
    }
    else if(!pmod->ptemplate->ppowBase->bIsEnvironmentHit)
    {
        //    the entity that damaged them has been freed
        //    instead of not tracking the damage at all
        //    track the damage as NULL (same as falling damage)
        damageTrackerUpdate(pchar->entParent, NULL, fDamage);

        //    Note:    try to track down the actual source of the damage when this isn't a high priority bug
    }

    if(ENTTYPE(pchar->entParent)==ENTTYPE_PLAYER && !pchar->bCannotDie && pchar->entParent!=pentSrcOwner)
    {
        // You only get credit for damage if you aren't
        //   doing it to yourself (or through your pet).
        // If you're immortal, you don't get credit for damage since you
        // have no incentive to avoid taking any.
        stat_AddDamageTaken(pchar->entParent->pl, (int)fDamage);
        badge_RecordDamageTaken(pchar->entParent, (int)fDamage);
    }
}

/**********************************************************************func*
 * PetGuardMastermind
 *
 */
#define BODYGUARD_RANGE (60.0f*60.0f) // Current range of Supremacy
#define BODYGUARD_MMSHARE (2.0f)      // Share of damage MM takes; a pet is one share
static float PetGuardMastermind(Character *pchar, Entity *pentSrc, AttribMod *pmodDamage, float magDamage, float fRate, U32 ulNow)
{
    float fBaseDamage = magDamage;
    float fMMDamage;
    Entity ** pppets = NULL;
    AttribModListIter iter;
    AttribMod *pmod = modlist_GetFirstMod(&iter, &pchar->listMod);
    int iPets = 0;

    // Only works when the MM is affecting pets with Supremacy
    if(pchar->attrCur.fOnlyAffectsSelf <= 0.0f)
    {
        int i;
        const AttribModTemplate *ptemplate = pmod->ptemplate;

        // Find all my pets
        while(pmod!=NULL)
        {
            Vec3 vecMM;
            copyVec3(ENTPOS(pchar->entParent),vecMM);

            if(pmod->ptemplate->offAttrib == kSpecialAttrib_EntCreate)
            {
                if(pmod->erCreated!=0)
                {
                    Entity *pent = erGetEnt(pmod->erCreated);
                    // Cheap check for valid pet (rather than checking explicitly for supremacy)
                    if(pent 
                       && distance3Squared(vecMM,ENTPOS(pent)) < BODYGUARD_RANGE
                       && pent->pchar
                       && pent->villainDef && pent->villainDef->petCommadability 
                       && pent->pchar->attrCur.fUntouchable <= 0.0f
                       && pent->pchar->attrCur.fHitPoints > 0.0f
                       && ENTAI(pent)->amBodyGuard)
                    {
                        eaPush(&pppets,pent);
                    }
                }
            }
            pmod = modlist_GetNextMod(&iter);
        }

        iPets = eaSize(&pppets);
        magDamage = magDamage/(float)(iPets+BODYGUARD_MMSHARE);
        for(i=iPets-1; i>=0; i--)
        {
            Entity *ppet = pppets[i];
            float fCur = ppet->pchar->attrCur.fHitPoints;
            ppet->pchar->attrCur.fHitPoints += magDamage;

            ReportAttribMod(ppet->pchar, magDamage, fRate, pmodDamage);

            if(!pmodDamage->bReportedOnLastTick && !pmodDamage->ptemplate->ppowBase->bIsEnvironmentHit)
            {
                character_InterruptPower(ppet->pchar, pentSrc ? pentSrc->pchar : NULL, 0.0, true);
            }

            NotifyAI(pmodDamage, ppet->pchar, magDamage, fRate);

            TrackHitpointChange(pmodDamage, ppet->pchar, pentSrc, magDamage, ulNow);
        }
    }

    fMMDamage = magDamage * BODYGUARD_MMSHARE;

    // Message to MM to let them know
    if(iPets>0)
    {
        float fAbsorb = fBaseDamage - fMMDamage;
        sendInfoBox(pchar->entParent, INFO_DAMAGE, "PetBodyguardsDamaged", -fAbsorb);
    }

    return fMMDamage;
}

/**********************************************************************func*
 * mod_Cancelled
 *
 */
static bool mod_Cancelled(AttribMod *pmod, Character *pchar)
{
    int i;
    int n = eaiSize(&pmod->ptemplate->piCancelEvents);

    for(i=0; i<n; i++)
    {
        int iEvent = pmod->ptemplate->piCancelEvents[i];

        if(POWEREVENT_INVOKE(iEvent)
            && pmod->ptemplate->ppowBase == pchar->ppowLastActivated)
        {
            continue;
        }
        if(POWEREVENT_APPLY(iEvent)
            && pmod->uiInvokeID == pchar->uiLastApplied)
        {
            continue;
        }

        if(pchar->aulEventCounts[iEvent]>0)
        {
            return true;
        }
    }

    return false;
}

/**********************************************************************func*
 * mod_Process
 *
 * Returns true if the modifier has timed out and should be removed from
 * the list of active modifiers.
 *
 */
bool mod_Process(AttribMod *pmod, Character *pchar, CharacterAttribSet *pattribset,
    CharacterAttributes *pattrResBuffs, CharacterAttributes *pattrResDebuffs,
    CharacterAttributes *pResistance, bool *pbRemoveSleepMods, bool *pbDisallowDefeat, float fRate)
{
    float *pf;
    float fRes = 0.0f;
    float fMag = 0.0f;
    bool bTimedOut;
    U32 ulNow = timerSecondsSince2000();
    bool bContinuingFXOnLast = pmod->bContinuingFXOn;

    if(mod_Cancelled(pmod, pchar))
    {
        pmod->fDuration = 0.0f;
    }
    else
    {
        if(!pmod->bSourceChecked)
        {
            if(pmod->fCheckTimer<=0.0)
            {
                Entity *pentSrc = erGetEnt(pmod->erSource);

                pmod->bSourceChecked = true;

                // Cancel this attribmod if the power can't be cast after death
                //   and the source is dead.
                // Don't cancel here if the source is the player, because those cancels
                //   should happen in dieNow().  This is to allow onDisable mods to function.
                // I've got a comment in dieNow() that ties into this change.  If you revert
                //   this, pull that comment as well.
                if(pmod->ptemplate->ppowBase->eDeathCastableSetting == kDeathCastableSetting_AliveOnly
                    && (!pentSrc || (!entAlive(pentSrc) && pentSrc != pchar->entParent)))
                {
                    mod_CancelFX(pmod,pchar);
                    mod_Cancel(pmod,pchar);
                    return true;
                }

                // Currently doesn't cancel attrib mods if the power can't be cast while alive
                //   and the source is alive.  Add this if it's ever needed...
                // I just noticed that character_HandleDeathAndResurrection doesn't do this
                //   either, so it's not really a bug that it's not here.  It's a "feature"
                //   that we don't kill death mods when resurrected.  *shrug*
            }
            pmod->fCheckTimer -= fRate;
        }

        if(!pmod->bDelayEvalChecked)
        {
            if(pmod->fTimer<=0.0)
            {
                Entity *pentSrc = erGetEnt(pmod->erSource);

                pmod->bDelayEvalChecked = true;
                
                // Evaluate delayed eval components of the attribmod now
                if(pmod->ptemplate->bDelayEval && pmod->evalInfo)
                {
                    // First re-set up the eval context
                    if(pmod->ptemplate->evalFlags & ATTRIBMOD_EVAL_CUSTOMFX)
                        combateval_StoreCustomFXToken(pmod->customFXToken);
                    if(pmod->ptemplate->evalFlags & ATTRIBMOD_EVAL_HIT_ROLL)
                        combateval_StoreToHitInfo(pmod->evalInfo->fRand, pmod->evalInfo->fToHit, pmod->evalInfo->bAlwaysHit, pmod->evalInfo->bForceHit);
                    if(pmod->ptemplate->evalFlags & ATTRIBMOD_EVAL_TARGETSHIT)
                        combateval_StoreTargetsHit(pmod->evalInfo->targetsHit);
                    if(pmod->ptemplate->evalFlags & ATTRIBMOD_EVAL_CHANCE)
                        combateval_StoreChance(pmod->evalInfo->fChanceFinal, pmod->evalInfo->fChanceMods);

                    devassert(pmod->parentPowerInstance && "Tell Andy he was wrong, we process mods that don't go through mod_Fill.");
                    
                    // Now run the Application Requires and early out if we don't actually attach now
                    if(pmod->ptemplate->ppchApplicationRequires && combateval_Eval(pentSrc, pchar->entParent, pmod->parentPowerInstance, pmod->ptemplate->ppchApplicationRequires, pmod->parentPowerInstance->ppowBase->pchSourceFile)==0.0f)
                    {
                        mod_CancelFX(pmod,pchar);
                        mod_Cancel(pmod,pchar);
                        return true;
                    }

                    // Now run the Expressions - the only thing different from original invocation is targetsHit, but that was unknownable at that point
                    // In the future we may change this to re-run strength, etc. recalculations now as well
                    if(pmod->ptemplate->evalFlags & ATTRIBMOD_EVAL_CALC_INFO)
                        combateval_StoreAttribCalcInfo(pmod->evalInfo->fFinal, pmod->evalInfo->fVal, pmod->ptemplate->fScale, pmod->evalInfo->fEffectiveness, pmod->evalInfo->fStrength);
                    if(pmod->ptemplate->ppchMagnitude)
                    {
                        pmod->fMagnitude = combateval_Eval(pentSrc, pchar->entParent, pmod->parentPowerInstance, pmod->ptemplate->ppchMagnitude, pmod->parentPowerInstance->ppowBase->pchSourceFile);
                    }
                    if(pmod->ptemplate->ppchDuration)
                    {
                        pmod->fDuration = combateval_Eval(pentSrc, pchar->entParent, pmod->parentPowerInstance, pmod->ptemplate->ppchDuration, pmod->parentPowerInstance->ppowBase->pchSourceFile);
                    }

                    // And now clean up the no longer needed evalInfo
                    attribModEvalInfo_Destroy(pmod->evalInfo);
                    pmod->evalInfo = NULL;
                }
            }
        }

        fMag = pmod->fMagnitude;
        // if(pmod->ptemplate->ppowBase->eType == kPowerType_Click && pmod->fTimer<=0.0f)
        //     verbose_printf("mod_Process:   Mag:%6.3f\n", f);

        if((pmod->ptemplate->bUseMagnitudeResistance || pmod->ptemplate->bUseDurationResistance)
            && pmod->ptemplate->offAttrib<sizeof(CharacterAttributes)
            && pmod->ptemplate->offAspect!=offsetof(CharacterAttribSet, pattrResistance))
        {
            // Resistance mods are resisted later on in CalcResistance.
            fRes = *(ATTRIB_GET_PTR(pResistance, pmod->ptemplate->offAttrib));

            // if(pmod->ptemplate->ppowBase->eType == kPowerType_Click && pmod->fTimer<=0.0f)
            //     verbose_printf("mod_Process:   Res:%6.3f\n", fRes);

            if(pmod->ptemplate->bUseMagnitudeResistance)
            {
                if(fRes>1.0f)
                    fRes = 1.0f;
                fMag *= (1.0f-fRes);
            }

            // if(pmod->ptemplate->ppowBase->eType == kPowerType_Click && pmod->fTimer<=0.0f)
            //     verbose_printf("mod_Process:   Tot:%6.3f\n", f);

            // Only apply duration resistance to the duration of the mod, not the timer that
            // delays it.  Note that this implementation isn't 100% perfect, because we may
            // go past the end of the timer and into a partial tick of the duration without
            // resistance, but I don't think that edge case is worth the performance hit
            // I'd get to fix it exactly.
            if (pmod->ptemplate->bUseDurationResistance && pmod->fTimer <= 0.0f)
            {
                if(fRes<(-0.95f))
                    fRes = -0.95f;
                fRate *= (1.0f+fRes);
            }
        }

        if(pmod->bSourceChecked)
        {
            size_t offAttrib  = pmod->ptemplate->offAttrib; // common subexpression
            Entity *pentSrc = erGetEnt(pmod->erSource);

            if(pmod->fTimer<=0.0f)
            {
                if(pmod->ptemplate->ppowBase->iTimeToConfirm>0
                    && pchar->entParent->pl && pchar->entParent->pl->uiDelayedInvokeID == pmod->uiInvokeID)
                {
                    // OK, this attribmod is part of a power which must be
                    //   confirmed. No confirmation has come through. So, this
                    //   attribmod is dead, as are all of its bretheren. Don't let
                    //   them confirm any more
                    pchar->entParent->pl->timeDelayedExpire = 0;
                    pmod->fDuration = 0;
                }
                else
                {
                    if(!mod_IsSuppressed(pmod, pchar, ulNow))
                    {
                        float fRand;

                        pmod->fTimer += pmod->ptemplate->fPeriod;
                        if (pmod->fTimer < -pmod->ptemplate->fPeriod)
                        {
                            // This is here in case the server gets behind or the activate
                            // period is very short. It makes sure the timer doesn't keep
                            // going further and further negative.
                            pmod->fTimer = -pmod->ptemplate->fPeriod;
                        }

                        if(pmod->fChance>=1.0f)
                        {
                            fRand = 0.0f;
                        }
                        else if(pmod->ptemplate->eType==kModType_Duration
                            && pmod->ptemplate->fPeriod==0.0f
                            && pmod->fChance<1.0f)
                        {
                            // If the AttribMod is of type duration and the period is 0, then
                            // the chance check is done when the AttribMod was originally
                            // attached. So now we'll just force it.
                            fRand = 0.0f;
                        }
                        else
                        {
                            fRand = (float)rand()/(float)RAND_MAX;
                        }

                        if(fRand < pmod->fChance && 
                            ( IS_DISABLINGSTATUS(offAttrib)  // If this is a hold, stun, immob or sleep if you are not on ground now, you will be soon
                            || !pmod->ptemplate->bNearGround || entHeight(pchar->entParent, 2.0f)<1.0f) )
                        {
                            size_t offAspect    = pmod->ptemplate->offAspect; // common subexpression
                            bool bIsHitPoints    = IS_HITPOINTS(offAttrib);
                            bool bIsKnock        = IS_KNOCK(offAttrib);
                            bool bModifiesCur    = IS_MODIFIER(offAspect);

                            if(offAttrib<sizeof(CharacterAttributes))
                            {

                                // Modify an attribute

                                // Resistance now has some special cases, so the elegant
                                // data-driver approach gets a bit skunkier up now.
                                if(offAspect == offsetof(CharacterAttribSet, pattrResistance))
                                {
                                    if(pmod->ptemplate->bUseDurationResistance || pmod->ptemplate->bUseMagnitudeResistance)
                                    {
                                        pf = ATTRIB_GET_PTR(pattrResDebuffs, offAttrib);
                                    }
                                    else
                                    {
                                        pf = ATTRIB_GET_PTR(pattrResBuffs, offAttrib);
                                    }
                                }
                                else
                                {
                                    pf = ASPECT_ATTRIB_GET_PTR(pattribset, offAspect, offAttrib);
                                }

                                // if(pmod->ptemplate->ppowBase->eType == kPowerType_Click)
                                //     verbose_printf("mod_Process:   Cur:%6.3f\n", *pf);

                                // Mastermind bodyguard feature
                                if(fMag < 0.0f
                                   && bIsHitPoints && offAttrib != offsetof(CharacterAttributes, fAbsorb)
                                   && bModifiesCur 
                                   && ENTTYPE(pchar->entParent)==ENTTYPE_PLAYER)
                                {
                                    static const CharacterClass *pClassMM = NULL;
                                    if(!pClassMM)
                                    {
                                        pClassMM = classes_GetPtrFromName(&g_CharacterClasses,"Class_Mastermind");
                                    }

                                    if(pchar->pclass == pClassMM)
                                    {
                                        fMag = PetGuardMastermind(pchar, pentSrc, pmod, fMag, fRate, ulNow);
                                    }
                                }

                                if(!bIsHitPoints
                                    || !bModifiesCur
                                    || !pchar->entParent->invincible)
                                {
                                    // This still explicitly handles Max Absorb mods,
                                    // even though they are kinda like Cur HP mods...
                                    *pf += fMag;
                                }

                                // Check to see if this attribmod wakes them up.
                                if(bModifiesCur && (bIsHitPoints || (bIsKnock && fMag > 0.0)))
                                {
                                    *pbRemoveSleepMods = true;
                                }

                                if ((bIsHitPoints && bModifiesCur) || !pmod->bReportedOnLastTick)
                                {
                                    // Still only report Max Absorb mods once instead of every tick.
                                    ReportAttribMod(pchar, fMag, fRate, pmod);
                                }

                                if(!pmod->bReportedOnLastTick)
                                {
                                    if(!pmod->ptemplate->ppowBase->bIsEnvironmentHit && (!pentSrc || pentSrc->pchar!=pchar))
                                    {
                                        character_InterruptPower(pchar, pentSrc ? pentSrc->pchar : NULL, 0.0, *pbRemoveSleepMods);
                                    }
                                }

                                // If this isn't a placate attribmod, check to cancel other placate attribmods
                                if (offAttrib != offsetof(CharacterAttributes, fPlacate))
                                    CancelPlacatePlayer(pchar, pentSrc, pmod->uiInvokeID);
                            }
                            else
                            {
                                if(!pmod->bReportedOnLastTick)
                                {
                                    ReportAttribMod(pchar, fMag, fRate, pmod);
                                }

                                // This is a special power which doesn't change the normal attributes
                                // Examples include entity creation and translucency
                                HandleSpecialAttrib(pmod, pchar, fMag, pbDisallowDefeat);
                            }

                            NotifyAI(pmod, pchar, fMag, fRate);

                            // TODO: I don't know of any better place to put this right now.

                            if(offAttrib==offsetof(CharacterAttributes, fTeleport)
                                && (offAspect==offsetof(CharacterAttribSet, pattrMod)
                                    || offAspect==offsetof(CharacterAttribSet, pattrAbsolute)))
                            {
                                // check for crossmap teleport
                                if (pmod->ptemplate->pchParams)
                                {
                                    pchar->pchTeleportPower = pmod->parentPowerInstance;
                                    pchar->pchTeleportDest = pmod->ptemplate->pchParams;
                                }
                                else
                                {
                                    pchar->pchTeleportPower = NULL;
                                    pchar->pchTeleportDest = NULL;
                                    copyVec3(pmod->vecLocation, pchar->vecTeleportDest);
                                }
                            }

                            if(fMag>0.0f
                                && (offAttrib==offsetof(CharacterAttributes, fKnockback)
                                    || offAttrib==offsetof(CharacterAttributes, fKnockup)
                                    || offAttrib==offsetof(CharacterAttributes, fRepel))
                                && (offAspect==offsetof(CharacterAttribSet, pattrMod)
                                    || offAspect==offsetof(CharacterAttribSet, pattrAbsolute)))
                            {
                                if (erGetRef(pchar->entParent) != pmod->erSource) {
                                    pchar->erKnockedFrom = pmod->erSource;
                                }
                            }

                            if(bIsHitPoints && bModifiesCur)
                            {
                                TrackHitpointChange(pmod, pchar, pentSrc, fMag, ulNow);
                            }

                            pmod->bReportedOnLastTick = true;

                            // hold off on near ground fx until they are on the ground
                            if(!pmod->ptemplate->bNearGround || entHeight(pchar->entParent, 2.0f)<1.0f) 
                                pmod->bContinuingFXOn = true;
                            else
                                pmod->bContinuingFXOn = false;    
                        }
                        else
                        {
                            if(pmod->ptemplate->bCancelOnMiss)
                            {
                                pmod->fDuration = 0.0;
                            }

                            pmod->bReportedOnLastTick = false;
                            pmod->bContinuingFXOn = false;
                        }
                    } // end if not suppressed
                }
            }
            else
            {
                pmod->bReportedOnLastTick = false;
            }

            // If this is a taunt or placate, handle that funky business!
            if (ENTTYPE(pchar->entParent) == ENTTYPE_PLAYER && IS_MODIFIER(pmod->ptemplate->offAspect))
            {
                if (offAttrib == offsetof(CharacterAttributes, fTaunt))
                    HandlePlayerTaunt(pmod, pchar, pentSrc, fRate);
                else if (offAttrib == offsetof(CharacterAttributes, fPlacate))
                    HandlePlayerPlacate(pmod, pchar, pentSrc, fRate);
            }
        }
    }

    bTimedOut = pmod->fDuration<=0.0f;

    if(bTimedOut)
    {
        mod_CancelFX(pmod, pchar);
        mod_Cancel(pmod, pchar);
    }
    else
    {
        const char *pchContinuingFX = basepower_GetAttribModFX(pmod->ptemplate->pchContinuingFX, pmod->fx->pchContinuingFX);
        if(pmod->bContinuingFXOn)
        {
            // Start up any continuing FX
            if(pchContinuingFX)
            {
                Entity *pentSrc = erGetEnt(pmod->erSource);
                if(!pentSrc || !pentSrc->pchar)
                {
                    pentSrc = pchar->entParent;
                }

                character_PlayMaintainedFX(pchar, pentSrc->pchar,
                    pchContinuingFX,
                    pmod->uiTint,
                    0.0,
                    PLAYFX_NOT_ATTACHED, PLAYFX_NO_TIMEOUT,
                    PLAYFX_CONTINUING|(pmod->fx->bImportant ? PLAYFX_IMPORTANT : 0));
            }
            // Set any mode bits for the AttribMod
            character_AddStickyStates(pchar, pmod->ptemplate->piContinuingBits);
        }
        else if(bContinuingFXOnLast)
        {
            // Kill FX if character made saving throw
            if(pmod->ptemplate->pchContinuingFX)
                character_KillMaintainedFX(pchar, pchar, pchContinuingFX);
        }
    }

    pmod->fTimer -= fRate;
    if(pmod->ptemplate->fDuration<ATTRIBMOD_DURATION_FOREVER)
    {
        pmod->fDuration -= fRate;
        if (((pmod->ptemplate->fDuration + pmod->ptemplate->fDelay) > 30) && (pmod->fDuration < 10) && (pmod->ptemplate->ppowBase->bShowBuffIcon))
        {
            pchar->entParent->team_buff_update = true;
        }
    }

    return bTimedOut;
}


/**********************************************************************func*
 * mod_CancelFX
 *
 */
void mod_CancelFX(AttribMod *pmod, Character *pchar)
{
    if(pmod->ptemplate->pchContinuingFX)
        character_KillMaintainedFX(pchar, pchar, basepower_GetAttribModFX(pmod->ptemplate->pchContinuingFX, pmod->fx->pchContinuingFX));
    if(pmod->ptemplate->pchConditionalFX)
        character_KillMaintainedFX(pchar, pchar, basepower_GetAttribModFX(pmod->ptemplate->pchConditionalFX, pmod->fx->pchConditionalFX));
}

/**********************************************************************func*
 * mod_Cancel
 *
 */
void mod_Cancel(AttribMod *pmod, Character *pchar)
{
    switch(pmod->ptemplate->offAttrib)
    {
        case kSpecialAttrib_SetMode:
            character_UnsetPowerMode(pchar, pmod->ptemplate->fMagnitude);
            break;

//         case kSpecialAttrib_UnsetMode:
//             character_SetPowerMode(pchar, pmod->ptemplate->fMagnitude);
//             break;

        case kSpecialAttrib_SetCostume:
            character_RestoreCostume(pchar);
            break;

        case kSpecialAttrib_Glide:
        case kSpecialAttrib_NinjaRun:
        case kSpecialAttrib_Walk:
        case kSpecialAttrib_BeastRun:
        case kSpecialAttrib_SteamJump:
        case kSpecialAttrib_HoverBoard:
        case kSpecialAttrib_MagicCarpet:
        case kSpecialAttrib_ParkourRun:
            setSpecialMovement(pchar->entParent, (int)pmod->ptemplate->offAttrib, false);
            break;

        case offsetof(CharacterAttributes, fTaunt):
            if(pchar->pmodTaunt == pmod)
            {
                pchar->pmodTaunt = NULL;
                pchar->entParent->target_update = 1;
                pchar->entParent->status_effects_update = true;
            }
            break;

        case offsetof(CharacterAttributes, fPerceptionRadius):
            pchar->entParent->status_effects_update = true;
            break;

        case offsetof(CharacterAttributes, fPlacate):
            eaFindAndRemove(&pchar->ppmodPlacates, pmod);
            if (eaSize(&pchar->ppmodPlacates) == 0)
            {
                eaDestroy(&pchar->ppmodPlacates);
            }
            pchar->entParent->target_update = 1;
            break;

        case kSpecialAttrib_EntCreate:
            if(pmod->erCreated!=0)
            {
                // Don't kill pets if they are meant to live on.
                if(pmod->ptemplate->fDuration!=0.0f || pmod->ptemplate->eType==kModType_Duration)
                {
                    character_DestroyPet(pchar, pmod);
                }
            }
            break;

        case kSpecialAttrib_Avoid:
            {
                AIEventNotifyParams params;
                const BasePower* ppowBase = pmod->ptemplate->ppowBase;

                params.notifyType = AI_EVENT_NOTIFY_POWER_ENDED;
                params.source = erGetEnt(pmod->erSource);
                params.target = pchar->entParent;
                params.powerEnded.offAttrib = (S32)pmod->ptemplate->offAttrib;
                params.powerEnded.offAspect = (S32)pmod->ptemplate->offAspect;
                params.powerEnded.ppowBase = pmod->ptemplate->ppowBase;
                params.powerEnded.uiInvokeID = pmod->uiInvokeID;
                if(params.source)
                {
                    aiEventNotify(&params);
                }
            }
            break;

        case kSpecialAttrib_GlobalChanceMod:
            {
                int i = eaFind(&pchar->ppmodChanceMods, pmod);
                if(i>=0)
                    eaRemoveFast(&pchar->ppmodChanceMods,i);
                if(eaSize(&pchar->ppmodChanceMods) == 0)
                    eaDestroy(&pchar->ppmodChanceMods);
            }
            break;
        case kSpecialAttrib_DesignerStatus:
            {
                pchar->entParent->status_effects_update = true;
            }
            break;

    }
}

/* End of File */
