/***************************************************************************
 *     Copyright (c) 2005-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include <utilitieslib/assert/assert.h>
#include <float.h>
#include <string.h>

#include <utilitieslib/utils/file.h>  // for isDevelopmentMode
#include <utilitieslib/utils/error.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/components/StashTable.h>

#include "entity/powers.h"
#include "attribmod.h"
#include "entity/boostset.h"
#include "gameComm/VillainDef.h"

#include "entity/character_eval.h"
#include "entity/character_combat_eval.h"
#include "character_attribs.h"
#include "entity/character_inventory.h"
#include "entity/LoadDefCommon.h"
#include <utilitieslib/components/SharedMemory.h>
#include <utilitieslib/utils/fileutil.h>
#include <utilitieslib/utils/FolderCache.h>

#include <utilitieslib/language/MessageStore.h>
#include <utilitieslib/language/MessageStoreUtil.h>
#include <utilitieslib/utils/textparser.h>

#include "gameData/costume_data.h"

#ifdef SERVER
#include "dbghelper.h"
#include "cmdparse/cmdserver.h"
#include "language/langServerUtil.h"

#define XLATE localizedPrintf(0,
#endif

#ifdef CLIENT
#include "cmdparse/cmdgame.h"
#include "UI/sprite/sprite_text.h"
#include "graphics/FX/fx.h"

#ifndef TEST_CLIENT
extern void uiEnhancementAddHelpText(Boost *pBoost, BasePower *pBase, int iLevel, StuffBuff *pBuf);
#endif

#define XLATE textStd(
#endif

extern StaticDefine ParsePowerDefines[]; // defined in load_def.c
extern TokenizerParseInfo ParseAttribModTemplate[];

PowerMode g_eRaidAttackerMode;
PowerMode g_eRaidDefenderMode;
PowerMode g_eOnCoopTeamMode;

// #define HASH_POWERS // Define to create and user name hashes to find powers.

/**********************************************************************func*
 * less
 *
 */
static int less(const void *a, const void *b)
{
    int i = *(int *)a;
    int j = *(int *)b;

    return j-i;
}


#define CHECK_TRANSLATION_CAT(pch) \
    if(pch && pch[0]!='\0') \
{ \
    char *pchXlated = XLATE pch); \
    if(stricmp(pch, pchXlated)==0) \
{ \
    pchXlated = XLATE "'%s'", pch); \
    ErrorFilenamef(pcat->pchSourceFile, "BAD TRANSLATION: %14s in field %-20s of category %s\n", pchXlated, #pch, pcat->pchName); \
} \
}


#define CHECK_TRANSLATION_SET(pch) \
    if(pch && pch[0]!='\0') \
{ \
    char *pchXlated = XLATE pch); \
    if(stricmp(pch, pchXlated)==0) \
{ \
    pchXlated = XLATE "'%s'", pch); \
    ErrorFilenamef(pset->pchSourceFile, "BAD TRANSLATION: %14s in field %-20s of power set %s\n", pchXlated, #pch, pset->pchFullName); \
} \
}


#define CHECK_TRANSLATION_POW(pch) \
    if(pch && pch[0]!='\0') \
{ \
    char *pchXlated = XLATE pch); \
    if(stricmp(pch, pchXlated)==0) \
{ \
    pchXlated = XLATE "'%s'", pch); \
    ErrorFilenamef(ppow->pchSourceFile, "BAD TRANSLATION: %14s in field %-30s of power %s\n", pchXlated, #pch, ppow->pchSourceName); \
} \
}

static void powerset_FixupForBin(const BasePowerSet *psetBase)
{
    if (psetBase->ppSpecializeRequires)
    {
        chareval_Validate(psetBase->ppSpecializeRequires, psetBase->pchSourceFile);
    }
}

static void power_FixupForBinFX(BasePower *ppow, PowerFX *fx)
{
    //
    // Do some sanity checking on the timing and animations
    //
    if(fx->iFramesBeforeHit==-1)
    {
        if(fx->iFramesAttack>35)
            ErrorFilenamef(fx->pchSourceFile, "STEVE: Long anim (%d frames) with no FramesBeforeHit: (%s)\n", fx->iFramesAttack, ppow->pchFullName);
        fx->iFramesBeforeHit = 15;
    }

    if(fx->iInitialFramesBeforeHit==-1)
        fx->iInitialFramesBeforeHit = fx->iFramesBeforeHit;

#ifdef CLIENT
    // Checks all attack and hitfx for a death condition
    if( game_state.fxdebug )
    {
        FxDebugAttributes attribs;

        if( fx->pchHitFX )
        {
            if( FxDebugGetAttributes( 0, fx->pchHitFX, &attribs ) )
            {
                if( attribs.lifeSpan <= 0 && !(attribs.type & FXTYPE_TRAVELING_PROJECTILE) )
                    printf( "FX %s used by %s as a oneshot HitFX, but wont die\n", fx->pchHitFX, ppow->pchFullName );
            }
            else
            {
                printf("Nonexistent Fx(%s %s)\n", ppow->pchFullName, fx->pchHitFX);
            }
        }

        if( fx->pchAttackFX )
        {
            if( FxDebugGetAttributes( 0, fx->pchAttackFX, &attribs ) )
            {
                if( attribs.lifeSpan <= 0 && !(attribs.type & FXTYPE_TRAVELING_PROJECTILE) )
                    printf( "FX %s used by %s as a oneshot AttackFX, but wont die\n", fx->pchAttackFX, ppow->pchFullName );
            }
            else
            {
                printf("Nonexistent Fx(%s %s)\n", ppow->pchFullName, fx->pchAttackFX);
            }
        }

        if( fx->pchInitialAttackFX )
        {
            if( FxDebugGetAttributes( 0, fx->pchInitialAttackFX, &attribs ) )
            {
                if( attribs.lifeSpan <= 0 && !(attribs.type & FXTYPE_TRAVELING_PROJECTILE) )
                    printf( "FX %s used by %s as a oneshot InitialAttackFX, but wont die\n", fx->pchInitialAttackFX, ppow->pchFullName );
            }
            else
            {
                printf("Nonexistent Fx(%s %s)\n", ppow->pchFullName, fx->pchInitialAttackFX);
            }
        }

        if( fx->pchBlockFX )
        {
            if( FxDebugGetAttributes( 0, fx->pchBlockFX, &attribs ) )
            {
                if( attribs.lifeSpan <= 0 && !(attribs.type & FXTYPE_TRAVELING_PROJECTILE) )
                    printf( "FX %s used by %s as a oneshot BlockFX, but wont die\n", fx->pchBlockFX, ppow->pchFullName );
            }
            else
            {
                printf("Nonexistent Fx(%s %s)\n", ppow->pchName, fx->pchBlockFX);
            }
        }

        if( fx->pchDeathFX)
        {
            if( FxDebugGetAttributes( 0, fx->pchDeathFX, &attribs ) )
            {
                if( attribs.lifeSpan <= 0 && !(attribs.type & FXTYPE_TRAVELING_PROJECTILE) )
                    printf( "FX %s used %s as a oneshot HitFX, but wont die", fx->pchDeathFX, ppow->pchFullName );
            }
            else
            {
                printf("Nonexistent Fx(%s %s)\n", ppow->pchFullName, fx->pchDeathFX);
            }
        }
    }
#endif

    if(fx->pchInitialAttackFX==NULL && fx->pchAttackFX!=NULL)
        fx->pchInitialAttackFX=ParserAllocString(fx->pchAttackFX);

    if(fx->piInitialAttackBits==NULL && fx->piAttackBits!=NULL)
        eaiCopy(&fx->piInitialAttackBits, &fx->piAttackBits);

    // Don't print warnings if defaulted.
    if(fx->iFramesAttack==-1)
    {
        fx->iFramesAttack = 35;
    }
    else if(ppow->eType == kPowerType_Click || ppow->eType == kPowerType_Toggle)
    {
        float fAnimTime = fx->iFramesAttack/30.0f;
        if(ppow->fTimeToActivate-ppow->fInterruptTime-fAnimTime > 0.01f
            || ppow->fTimeToActivate-ppow->fInterruptTime-fAnimTime < -0.01f)
        {
            if(fx->pchIgnoreAttackTimeErrors && *fx->pchIgnoreAttackTimeErrors)
            {
                // Suppressed
            }
            else
            {
                //ErrorFilenamef(ppow->pchSourceFile, "(%s.%s.%s)\n  TimeToActivate (%1.2fs) != FramesAttack (%1.2fs)\nIf you are certain that this is correct, you may ignore this error by putting \"IgnoreAttackTimeErrors %s\" in the power definition.\n",
                //    pcat->pchName, pset->pchName, ppow->pchName,
                //    ppow->fTimeToActivate-ppow->fInterruptTime,
                //    fAnimTime,
                //    getUserName());
#ifdef TEST_CLIENT
                printf("%s: Animation Powers (%s)\n  TimeToActivate (%1.2fs) != FramesAttack (%1.2fs)\n",
                    ppow->pchSourceFile, ppow->pchFullName, ppow->fTimeToActivate-ppow->fInterruptTime, fAnimTime);
#else
                ErrorFilenamef(ppow->pchSourceFile,
                    "(%s)\n  TimeToActivate (%1.2fs) != FramesAttack (%1.2fs)\nIf you are certain that this is correct, you may ignore this error by putting \"IgnoreAttackTimeErrors %s\" in the power definition.\n",
                    ppow->pchFullName, ppow->fTimeToActivate-ppow->fInterruptTime,
                    fAnimTime, getUserName());
#endif
            }
        }
        else
        {
            if(fx->pchIgnoreAttackTimeErrors && *fx->pchIgnoreAttackTimeErrors)
            {
                ErrorFilenamef(ppow->pchSourceFile, "(%s) %s used IgnoreAttackTimeErrors and didn't need to!  It should be removed.\n",
                    ppow->pchFullName,
                    fx->pchIgnoreAttackTimeErrors);
            }
        }
    }

    if(fx->bDelayedHit)
    {
        if(fx->fProjectileSpeed==0.0f)
            fx->fProjectileSpeed = 60.0f;
    }
    else
    {
        fx->fProjectileSpeed = FLT_MAX;
    }
}

static void power_FixupForBin(BasePower *ppow, int **crcFullNameList)
{
    int iSizeVars;
    bool bAllNearGround;
    bool hasUntilShutOffMods = false;
    bool hasOnlyUntilShutOffMods = true;
    int imod, ifx;
    int iSize;
    ppow->crcFullName = powerdict_GetCRCOfBasePower(ppow);
    assert(ppow->crcFullName);    //    make sure it doesn't CRC to 0 (which we're reserving to test for NULLs)
    assert(-1 == eaiFind(crcFullNameList, ppow->crcFullName));
    eaiPush(crcFullNameList, ppow->crcFullName);

    // Change degrees to radians
    ppow->fArc = RAD(ppow->fArc);
    ppow->fPositionYaw = RAD(ppow->fPositionYaw);

    // Fix up invention vars
    iSizeVars = eaSize(&ppow->ppVars);
    if(iSizeVars > POWER_VAR_MAX_COUNT)
    {
        ErrorFilenamef(ppow->pchSourceFile, "Too many vars defined for power (%s).\n",ppow->pchFullName);
        eaSetSize(&cpp_const_cast(PowerVar**)(ppow->ppVars), POWER_VAR_MAX_COUNT);
    }
    for(imod = 0; imod<iSizeVars; imod++)
    {
        if(ppow->ppVars[imod]->iIndex >= POWER_VAR_MAX_COUNT)
        {
            ErrorFilenamef(ppow->pchSourceFile, "Invalid index for vars on power (%s).\n",ppow->pchFullName);
            (cpp_const_cast(PowerVar*)(ppow->ppVars[imod]))->iIndex = -1;
        }
    }


    power_FixupForBinFX(ppow, &ppow->fx);
    for(ifx = eaSize(&ppow->customfx)-1; ifx >= 0; --ifx)
        power_FixupForBinFX(ppow, cpp_const_cast(PowerFX*)(&ppow->customfx[ifx]->fx));

    // Sort BoostsAllowed so it's easier to compare them.
    qsort(cpp_const_cast(BoostType*)(ppow->pBoostsAllowed), eaiSize(&(int *)ppow->pBoostsAllowed), sizeof(int), less);

    //
    // Validate the power requires.
    //
    if(ppow->ppchBuyRequires)
    {
        chareval_Validate(ppow->ppchBuyRequires,ppow->pchSourceFile);
    }
    if(ppow->ppchActivateRequires)
    {
        chareval_Validate(ppow->ppchActivateRequires,ppow->pchSourceFile);
    }
    if(ppow->ppchConfirmRequires)
    {
        chareval_Validate(ppow->ppchConfirmRequires,ppow->pchSourceFile);
    }
    if(ppow->ppchSlotRequires)
    {
        // not a character evaluation statement, can't run this anymore.
        // TODO:  Write a validator for this type of evaluation statement.
        //chareval_Validate(ppow->ppchSlotRequires,ppow->pchSourceFile);
    }
    if(ppow->ppchRewardRequires)
    {
        chareval_Validate(ppow->ppchRewardRequires,ppow->pchSourceFile);
    }
    if(ppow->ppchHighlightEval)
    {
        chareval_Validate(ppow->ppchHighlightEval,ppow->pchSourceFile);
    }
    if(ppow->ppchAuctionRequires)
    {
        chareval_Validate(ppow->ppchAuctionRequires,ppow->pchSourceFile);
    }

#if SERVER
    if(ppow->ppchTargetRequires)
    {
        combateval_Validate(dbg_BasePowerStr(ppow),ppow->ppchTargetRequires,ppow->pchSourceFile);
    }
#endif

    if (ppow->iNumAllowed > 1)
    {
        if (ppow->eType == kPowerType_GlobalBoost)
        {
            ErrorFilenamef(ppow->pchSourceFile, "GlobalBoost (%s) has NumAllowed %d which should be 1\nYou cannot have two copies of the same GlobalBoost\n",ppow->pchFullName, ppow->iNumAllowed);
            ppow->iNumAllowed = 1;
        }
        else if (strstriConst(ppow->pchFullName, "Temporary_Powers") == NULL
                    && ppow->bDoNotSave == 0)
        {
            ErrorFilenamef(ppow->pchSourceFile, "Power (%s) has NumAllowed %d, but isn't allowed to have more than one.  NumAllowed only works for powers in the Temporary Powers category or DoNotSave kTrue.\n", ppow->pchFullName, ppow->iNumAllowed);
            ppow->iNumAllowed = 1;
        }
    }

    if(ppow->bIsEnvironmentHit && (ppow->eAIReport != kAIReport_Never))
    {
        ErrorFilenamef(ppow->pchSourceFile, "Power (%s) IsEnvironmentHit but not set to AIReport Never.\nThis will cause unexpected AI results because hits will be counted as attacks.\nPower is being automatically changed to AIReport Never, please adjust data so this change is explict.",ppow->pchFullName);
        ppow->eAIReport = kAIReport_Never;
    }

    // Because Design is incredibly lazy, I'm doing a fixup on
    //   bShowBuffIcon if it's not specifically set.
    // If this is a pet getting stuck to the caster, then we want
    //   to see the icon (e.g. Fire Imps)
    // (Search for "ShowBuffIcon" for more fixups below)
    if(ppow->bShowBuffIcon == -1)
    {
        if(eaiSize(&(int*)ppow->pAffected) == 1
            && ppow->pAffected[0] == kTargetType_Caster)
        {
            ppow->bShowBuffIcon = true;
        }
    }

    //
    // Sanitize Attribmods
    //
    bAllNearGround = true;

    iSize = eaSize(&ppow->ppTemplates);

    for(imod=0; imod<iSize; imod++)
    {
        AttribModTemplate *ptemplate = cpp_const_cast(AttribModTemplate*)(ppow->ppTemplates[imod]);

        if(ptemplate->fDuration>=ATTRIBMOD_DURATION_FOREVER)
        {
            hasUntilShutOffMods = true;

            if(ptemplate->eApplicationType != kModApplicationType_OnActivate)
                ErrorFilenamef(ppow->pchSourceFile, "kUntilShutOff AttribMod not set to ApplicationType OnActivate in power (%s).\n",ppow->pchFullName);

            ptemplate->eApplicationType = kModApplicationType_OnActivate;
        }
        else
            hasOnlyUntilShutOffMods = false;

        // Clean up entity creation templates if they need it.
        if(ptemplate->pchPriorityListPassive!=NULL)
        {
            if(ptemplate->pchPriorityListOffense==NULL)
            {
                ptemplate->pchPriorityListOffense=
                    ParserAllocString(ptemplate->pchPriorityListPassive);
            }
            if(ptemplate->pchPriorityListDefense==NULL)
            {
                ptemplate->pchPriorityListDefense=
                    ParserAllocString(ptemplate->pchPriorityListPassive);
            }
        }

        // Determine if the duration and magnitude should be
        //   affected by combat mods.
        if(ptemplate->bUseDurationCombatMods == -1)
        {
            if(ptemplate->eType == kModType_Duration
                || ptemplate->eType == kModType_Constant
                || (ptemplate->eType == kModType_Expression && ptemplate->ppchDuration))
            {
                ptemplate->bUseDurationCombatMods = ptemplate->bAllowCombatMods;
            }
            else
            {
                ptemplate->bUseDurationCombatMods = false;
            }
        }

        if(ptemplate->bUseMagnitudeCombatMods == -1)
        {
            if(ptemplate->eType == kModType_Magnitude
                || ptemplate->eType == kModType_SkillMagnitude
                || ptemplate->eType == kModType_Constant
                || (ptemplate->eType == kModType_Expression && ptemplate->ppchMagnitude))
            {
                ptemplate->bUseMagnitudeCombatMods = ptemplate->bAllowCombatMods;
            }
            else
            {
                ptemplate->bUseMagnitudeCombatMods = false;
            }
        }

        // Determine if the duration and magnitude should be
        //   resisted.
        if(ptemplate->bUseDurationResistance == -1)
        {
            if(ptemplate->eType == kModType_Duration
                || ptemplate->eType == kModType_Constant
                || (ptemplate->eType == kModType_Expression && ptemplate->ppchDuration))
            {
                ptemplate->bUseDurationResistance = ptemplate->bAllowResistance;
            }
            else
            {
                ptemplate->bUseDurationResistance = false;
            }
        }

        if(ptemplate->bUseMagnitudeResistance == -1)
        {
            if(ptemplate->eType == kModType_Magnitude
                || ptemplate->eType == kModType_SkillMagnitude
                || ptemplate->eType == kModType_Constant
                || (ptemplate->eType == kModType_Expression && ptemplate->ppchMagnitude))
            {
                ptemplate->bUseMagnitudeResistance = ptemplate->bAllowResistance;
            }
            else
            {
                ptemplate->bUseMagnitudeResistance = false;
            }
        }


        bAllNearGround = bAllNearGround && ppow->ppTemplates[imod]->bNearGround;

        // More ShowBuffIcon fixups for lazy Designers.
        if(ppow->bShowBuffIcon == -1)
        {
            // If this attribmod doesn't target the caster
            //   and its an EntCreate, shut off the buff icon.
            // If the pet does anything to the target, it will use
            //   its own power which will have its own icon.
            if(ppow->ppTemplates[imod]->eTarget != kModTarget_Caster
                && ppow->ppTemplates[imod]->offAttrib == kSpecialAttrib_EntCreate)
            {
                ppow->bShowBuffIcon = false;
            }
            else
            {
                // If there are any AttribMods on this power
                //   which aren't EntCreates, then we need to see
                //   the icon no matter what.
                ppow->bShowBuffIcon = true;
            }
        }

        ptemplate->evalFlags = 0;

        // Validate the attribmod requires and gather what eval types we may need
        if(ptemplate->ppchApplicationRequires)
        {
            int i;

            if(ptemplate->offAttrib == kSpecialAttrib_PowerRedirect)
            {
                // Only the @CustomFX special eval operand is valid here
                for(i = eaSize(&ptemplate->ppchApplicationRequires) - 1; i >= 0; i--)
                {
                    const char* requiresOperand = ptemplate->ppchApplicationRequires[i];
                    if(strstriConst(requiresOperand, "@ToHit") || strstriConst(requiresOperand, "@ToHitRoll") || 
                        strstriConst(requiresOperand, "@AlwaysHit") || strstriConst(requiresOperand, "@ForceHit") ||
                        strstriConst(requiresOperand, "@Value") || strstriConst(requiresOperand, "@Scale") || 
                        strstriConst(requiresOperand, "@Effectiveness") || strstriConst(requiresOperand, "@Strength") ||
                        strstriConst(requiresOperand, "@StdResult") || strstriConst(requiresOperand, "@TargetsHit") || 
                        strstriConst(requiresOperand, "@Chance") || strstriConst(requiresOperand, "@ChanceMods"))
                            ErrorFilenamef(ppow->pchSourceFile, "Power (%s) uses PowerRedirection illegal eval operands in an ApplicationRequires.\n",ppow->pchFullName);
                    if(strstriConst(requiresOperand, "@CustomFX"))
                        ptemplate->evalFlags |= ATTRIBMOD_EVAL_CUSTOMFX;
                }
            }
            else
            {
                for(i = eaSize(&ptemplate->ppchApplicationRequires) - 1; i >= 0; i--)
                {
                    const char* requiresOperand = ptemplate->ppchApplicationRequires[i];
                    if(strstriConst(requiresOperand, "@ToHit") || strstriConst(requiresOperand, "@ToHitRoll") || 
                        strstriConst(requiresOperand, "@AlwaysHit") || strstriConst(requiresOperand, "@ForceHit"))
                            ptemplate->evalFlags |= ATTRIBMOD_EVAL_HIT_ROLL;
                    if(strstriConst(requiresOperand, "@Value") || strstriConst(requiresOperand, "@Scale") || 
                        strstriConst(requiresOperand, "@Effectiveness") || strstriConst(requiresOperand, "@Strength") ||
                        strstriConst(requiresOperand, "@StdResult"))
                            ErrorFilenamef(ppow->pchSourceFile, "Power (%s) uses Expression-only operands in an ApplicationRequires.\n",ppow->pchFullName);
                    if(strstriConst(requiresOperand, "@CustomFX"))
                        ptemplate->evalFlags |= ATTRIBMOD_EVAL_CUSTOMFX;
                    if(strstriConst(requiresOperand, "@TargetsHit"))
                        ptemplate->evalFlags |= ATTRIBMOD_EVAL_TARGETSHIT;
                    if(strstriConst(requiresOperand, "@Chance") || strstriConst(requiresOperand, "@ChanceMods"))
                        ptemplate->evalFlags |= ATTRIBMOD_EVAL_CHANCE;
                }
            }

#if SERVER
            combateval_Validate(dbg_BasePowerStr(ppow), ptemplate->ppchApplicationRequires, ppow->pchSourceFile);
#endif
        }

        // Validate the attribmod expressions and gather what eval types we may need
        if(ptemplate->ppchMagnitude)
        {
            int i;

            for(i = eaSize(&ptemplate->ppchMagnitude) - 1; i >= 0; i--)
            {
                const char* requiresOperand = ptemplate->ppchMagnitude[i];
                if(strstriConst(requiresOperand, "@ToHit") || strstriConst(requiresOperand, "@ToHitRoll") || 
                    strstriConst(requiresOperand, "@AlwaysHit") || strstriConst(requiresOperand, "@ForceHit"))
                        ptemplate->evalFlags |= ATTRIBMOD_EVAL_HIT_ROLL;
                if(strstriConst(requiresOperand, "@Value") || strstriConst(requiresOperand, "@Scale") || 
                    strstriConst(requiresOperand, "@Effectiveness") || strstriConst(requiresOperand, "@Strength") ||
                    strstriConst(requiresOperand, "@StdResult"))
                        ptemplate->evalFlags |= ATTRIBMOD_EVAL_CALC_INFO;
                if(strstriConst(requiresOperand, "@CustomFX"))
                        ptemplate->evalFlags |= ATTRIBMOD_EVAL_CUSTOMFX;
                if(strstriConst(requiresOperand, "@TargetsHit"))
                        ptemplate->evalFlags |= ATTRIBMOD_EVAL_TARGETSHIT;
                if(strstriConst(requiresOperand, "@Chance") || strstriConst(requiresOperand, "@ChanceMods"))
                        ptemplate->evalFlags |= ATTRIBMOD_EVAL_CHANCE;
            }

#if SERVER
            combateval_Validate(dbg_BasePowerStr(ppow), ptemplate->ppchMagnitude, ppow->pchSourceFile);
#endif
        }
        if(ptemplate->ppchDuration)
        {
            int i;
            for(i = eaSize(&ptemplate->ppchDuration) - 1; i >= 0; i--)
            {
                const char* requiresOperand = ptemplate->ppchDuration[i];
                if(strstriConst(requiresOperand, "@ToHit") || strstriConst(requiresOperand, "@ToHitRoll") || 
                    strstriConst(requiresOperand, "@AlwaysHit") || strstriConst(requiresOperand, "@ForceHit"))
                        ptemplate->evalFlags |= ATTRIBMOD_EVAL_HIT_ROLL;
                if(strstriConst(requiresOperand, "@Value") || strstriConst(requiresOperand, "@Scale") || 
                    strstriConst(requiresOperand, "@Effectiveness") || strstriConst(requiresOperand, "@Strength") ||
                    strstriConst(requiresOperand, "@StdResult"))
                        ptemplate->evalFlags |= ATTRIBMOD_EVAL_CALC_INFO;
                if(strstriConst(requiresOperand, "@CustomFX"))
                        ptemplate->evalFlags |= ATTRIBMOD_EVAL_CUSTOMFX;
                if(strstriConst(requiresOperand, "@TargetsHit"))
                        ptemplate->evalFlags |= ATTRIBMOD_EVAL_TARGETSHIT;
                if(strstriConst(requiresOperand, "@Chance") || strstriConst(requiresOperand, "@ChanceMods"))
                        ptemplate->evalFlags |= ATTRIBMOD_EVAL_CHANCE;
            }

#if SERVER
            combateval_Validate(dbg_BasePowerStr(ppow), ptemplate->ppchDuration, ppow->pchSourceFile);
#endif
        }

        if (ptemplate->offAttrib == kSpecialAttrib_SetSZEValue)
        {
            if (!ptemplate->ppchPrimaryStringList)
                ErrorFilenamef(ppow->pchSourceFile, "Power (%s) has a SetSZEValue attrib mod without a ppchPrimaryStringList.\n", ppow->pchFullName);
            else
                chareval_Validate(ptemplate->ppchPrimaryStringList, ppow->pchSourceFile);
        }

        // Sanity check for @TargetsHit
        if((ptemplate->evalFlags & ATTRIBMOD_EVAL_TARGETSHIT) && !ptemplate->bDelayEval)
            ErrorFilenamef(ppow->pchSourceFile, "Power (%s) uses @TargetsHit but does not have DelayEval.\n",ppow->pchFullName);

        if (ptemplate->bDelayEval && ptemplate->eStack == kStackType_Maximize)
            ErrorFilenamef(ppow->pchSourceFile, "Power (%s) uses kStackType_Maximize and has DelayEval.\n", ppow->pchFullName);

        if (ptemplate->bDelayEval && ptemplate->eStack == kStackType_Suppress)
            ErrorFilenamef(ppow->pchSourceFile, "Power (%s) uses kStackType_Suppress and has DelayEval.\n", ppow->pchFullName);

        if (ptemplate->offAttrib == offsetof(CharacterAttributes, fAbsorb) && ptemplate->eStack == kStackType_Suppress)
        {
            ErrorFilenamef(ppow->pchSourceFile, "Power (%s) uses kStackType_Suppress on an Absorb mod, this isn't supported. Changing to kStackType_Stack.\n", ppow->pchFullName);
            ptemplate->eStack = kStackType_Stack;
        }
    } // for each attrib mod template

    if(hasOnlyUntilShutOffMods && ppow->fActivatePeriod < ACTIVATE_PERIOD_FOREVER)
    {
        ErrorFilenamef(ppow->pchSourceFile, "ActivatePeriod too short for power (%s) which only has kUntilShutOff AttribMods.\n",ppow->pchFullName);
        ppow->fActivatePeriod = ACTIVATE_PERIOD_FOREVER;
    }
    else if(!hasOnlyUntilShutOffMods && ppow->fActivatePeriod >= ACTIVATE_PERIOD_FOREVER)
        ErrorFilenamef(ppow->pchSourceFile, "Power (%s) has non kUntilShutOff AttribMods and a very long ActivatePeriod.\n",ppow->pchFullName);

    if(hasUntilShutOffMods && !((ppow->eType == kPowerType_Auto) || (ppow->eType == kPowerType_Toggle)))
    {
        ErrorFilenamef(ppow->pchSourceFile, "kUntilShutOff AttribMods in non-Auto or Toggle power (%s).\n",ppow->pchFullName);
    }

    if(ppow->bTargetNearGround<0)
    {
        ppow->bTargetNearGround = bAllNearGround;
    }

#ifndef TEST_CLIENT
    // Doing this in a separate loop to make the errors easier to read.
    if (!msGetHideTranslationErrors())
    {
        CHECK_TRANSLATION_POW(ppow->pchDisplayAttackerAttackFloater);
        CHECK_TRANSLATION_POW(ppow->pchDisplayAttackerAttack);
        CHECK_TRANSLATION_POW(ppow->pchDisplayAttackerHit);
        CHECK_TRANSLATION_POW(ppow->pchDisplayHelp);
        CHECK_TRANSLATION_POW(ppow->pchDisplayName);
        CHECK_TRANSLATION_POW(ppow->pchDisplayShortHelp);
        CHECK_TRANSLATION_POW(ppow->pchDisplayVictimHit);
    }
#endif

}

// powerdict_ValidateEntCreate and power_ValidateEntCreate rely on the VillainDef hash being loaded and proper
// since powers are loaded before villains, we must call this validation after that load
static void power_ValidateEntCreate(const BasePower *ppow)
{
    int i, iSize = eaSize(&ppow->ppTemplates);
    for (i = 0; i < iSize; i++)
    {
        const AttribModTemplate *ptemplate = ppow->ppTemplates[i];
        if(ptemplate->offAttrib == kSpecialAttrib_EntCreate)
        {
            if(ptemplate->pchEntityDef == NULL)
            {
                ErrorFilenamef(ppow->pchSourceFile, "Power (%s) contains an EntCreate attrib mod with no EntityDef.\nIt won't create anything.",ppow->pchFullName);
            }
            else if(ptemplate->pchPriorityListPassive == NULL)
            {
                ErrorFilenamef(ppow->pchSourceFile, "Power (%s) contains an EntCreate attrib mod with no spawn Behavior String.\nIt won't create anything.",ppow->pchFullName);
            }
            else if(!strstriConst(ptemplate->pchEntityDef, DOPPEL_NAME)) // Doppelgangers aren't in the VillainDef hash, they're special
            {
                if(!villainFindByName(ptemplate->pchEntityDef))
                {
                    ErrorFilenamef(ppow->pchSourceFile, "Power (%s) contains EntCreate with EntityDef (%s) which is not a valid VillainDef.\nIt won't create anything.",ppow->pchFullName,ptemplate->pchEntityDef);
                }
            }
        }
        else 
        {
            if(ptemplate->pchEntityDef != NULL)
                ErrorFilenamef(ppow->pchSourceFile, "Power (%s) contains EntityDef for a non EntCreate attrib mod.",ppow->pchFullName);

            if(ptemplate->pchPriorityListPassive != NULL)
                ErrorFilenamef(ppow->pchSourceFile, "Power (%s) contains spawn Behavior for a non EntCreate attrib mod.",ppow->pchFullName);
        }
    }
}

void powerdict_ValidateEntCreate(const PowerDictionary *pdict)
{
    int i, iSize = eaSize(&pdict->ppPowers);
    for (i = 0; i < iSize; i++)
    {
        power_ValidateEntCreate(pdict->ppPowers[i]);
    }
}

/**********************************************************************func*
* powerdict_SetForwardPointers
*
* Constructs the pcat->pset->ppow pointers
*
*/
void powerdict_SetForwardPointers(PowerDictionary *pdict)
{
    int icat;
    int iSizeCat;

    iSizeCat = eaSize(&pdict->ppPowerCategories);
    for(icat=0; icat<iSizeCat; icat++)
    {
        int iset;
        PowerCategory *pcat = (PowerCategory*)pdict->ppPowerCategories[icat];
        int iSizeSets = eaSize(&pcat->ppPowerSetNames);

        eaSetSizeConst(&cpp_const_cast(BasePowerSet**)(pcat->ppPowerSets),0);

        for(iset=0; iset<iSizeSets; iset++)
        {
            int ipow;
            int iSizePows;
            BasePowerSet *pset = (BasePowerSet*)powerdict_GetBasePowerSetByFullName(pdict,pcat->ppPowerSetNames[iset]);

            if (!pset)
            {
                ErrorFilenamef(pcat->pchSourceFile,
                    "Power Category %s contains reference to nonexistent PowerSet %s!",
                    pcat->pchName,pcat->ppPowerSetNames[iset]);
                continue;
            }

            iSizePows = eaSize(&pset->ppPowerNames);

            eaSetSizeConst(&pset->ppPowers,0);

            for(ipow=0; ipow<iSizePows; ipow++)
            {
                char powername[512];
                BasePower *ppow = (BasePower*)powerdict_GetBasePowerByFullName(pdict,pset->ppPowerNames[ipow]);
                
                if (!ppow)
                {
                    ErrorFilenamef(pset->pchSourceFile,
                        "Powerset %s contains reference to nonexistent power %s! This breaks the available levels of powers!",
                        pset->pchFullName,pset->ppPowerNames[ipow]);
                    continue;
                }

                sprintf(powername,"%s.%s",pset->pchFullName,ppow->pchName);
                
                if (strcmp(powername,ppow->pchFullName) != 0)
                {
                    ppow = (BasePower*)powerdict_GetBasePowerByFullName(pdict,powername);
                }

                if (!ppow)
                {
                    ErrorFilenamef(pset->pchSourceFile,
                        "Powerset %s is missing power %s (duplicate of %s). Rebuild your bins, and get a programmer if that fails",
                        pset->pchFullName,powername,pset->ppPowerNames[ipow]);
                    continue;
                }

                eaPushConst(&pset->ppPowers,ppow);
            }
            eaPushConst(&pcat->ppPowerSets,pset);
        }
    }
}

/**********************************************************************func*
* powerdict_DuplicateMissingPowers
*
* This function sets up the power dictionary loading process for the rest of the
* loading process, and duplicates any powers needed to fulfill inter-set references
*
*/
void powerdict_DuplicateMissingPowers(PowerDictionary *pdict)
{
    int icat;
    int iSizeCat;

    iSizeCat = eaSize(&pdict->ppPowerCategories);
    for(icat=0; icat<iSizeCat; icat++)
    {
        int iset;
        PowerCategory *pcat = (PowerCategory*)pdict->ppPowerCategories[icat];
        int iSizeSets = eaSize(&pcat->ppPowerSetNames);

        eaSetSizeConst(&pcat->ppPowerSets,0);

        for(iset=0; iset<iSizeSets; iset++)
        {
            int ipow;
            int iSizePows;
            BasePowerSet *pset = (BasePowerSet*)powerdict_GetBasePowerSetByFullName(pdict,pcat->ppPowerSetNames[iset]);

            if (!pset)
            {
                Errorf("Power Category %s contains reference to nonexistent PowerSet %s!",
                    pcat->pchName,pcat->ppPowerSetNames[iset]);
                continue;
            }

            iSizePows = eaSize(&pset->ppPowerNames);

            eaSetSizeConst(&pset->ppPowers,0);

            for(ipow=0; ipow<iSizePows; ipow++)
            {
                char powername[512];
                BasePower *ppow = (BasePower*)powerdict_GetBasePowerByFullName(pdict,pset->ppPowerNames[ipow]);

                if (!ppow)
                {
                    ErrorFilenamef(pset->pchSourceFile,
                        "Powerset %s contains reference to nonexistent power %s! This breaks the available levels of powers!",
                        pset->pchFullName,pset->ppPowerNames[ipow]);
                    continue;
                }
                    

                sprintf(powername,"%s.%s",pset->pchFullName,ppow->pchName);

                if (strcmp(powername,ppow->pchFullName) != 0)
                {
                    BasePower *newpow = (BasePower*)powerdict_GetBasePowerByFullName(pdict,powername);

                    if (!newpow)
                    {                    
                        // If the power name doesn't match expected name, this refers to a power
                        // in another set, and needs to be duplicated for this set
                        BasePower *newpow = ParserAllocStruct(sizeof(BasePower));
                        ParserCopyStruct(ParseBasePower,ppow,sizeof(BasePower),newpow);
                        newpow->pchSourceName = ParserAllocString(newpow->pchFullName);
                        StructFreeStringConst(newpow->pchFullName);
                        newpow->pchFullName = ParserAllocString(powername);

                        eaPushConst(&pdict->ppPowers,newpow);
                    }
                    ppow = newpow;
                }
                else if (!ppow->pchSourceName)
                {
                    ppow->pchSourceName = ParserAllocString(ppow->pchFullName);
                }
                
                eaPushConst(&pset->ppPowers,ppow);

            }
            eaPushConst(&pcat->ppPowerSets,pset);
        }        
    }
}

/**********************************************************************func*
 * powerdict_FixPowerSetErrors
 *
 * This function is called after the power dict hierarchy is set up, but BEFORE
 * any bin files are written. It will never be called in production mode
 *
 */
void powerdict_FixPowerSetErrors(PowerDictionary *pdict)
{
    int icat;
    int iSizeCat;

    assert(pdict!=NULL);

    iSizeCat = eaSize(&pdict->ppPowerCategories);
    for(icat=0; icat<iSizeCat; icat++)
    {
        int iset;
        PowerCategory *pcat = (PowerCategory*)pdict->ppPowerCategories[icat];
        int iSizeSets = eaSize(&pcat->ppPowerSets);

        for(iset=0; iset<iSizeSets; iset++)
        {
            int ipow;
            BasePowerSet *pset = (BasePowerSet*)pcat->ppPowerSets[iset];
            int iSizePows = eaSize(&pset->ppPowers);

            for(ipow=0; ipow<iSizePows; ipow++)
            {
                BasePower *ppow = (BasePower*)pset->ppPowers[ipow];

                if(ppow)
                {
                    // All the powers in a power set have to be in the same
                    //   power system.
                    ppow->eSystem = pset->eSystem;

                    // Default Free-ness of the power for inherent powers
                    if(stricmp(pcat->pchName, "Inherent")==0)
                    {
                        ppow->bFree = true;
                        ppow->bAutoIssue = true;
                    }

                    if(stricmp(ppow->pchName, "Inherent")==0)
                    {
                        ppow->bFree = true;
                        ppow->bAutoIssue = true;
                    }

                    // Set max boosts for temporary powers to zero since you
                    //   can't slot them. Disallow all kinds of boosts in them.
                    // Temporary powers are also free.
                    if(stricmp(pcat->pchName, "Temporary_Powers")==0)
                    {
                        ppow->iMaxBoosts = 0;
                        if(ppow->eType != kPowerType_Boost && ppow->eType != kPowerType_GlobalBoost)
                            eaiDestroy(&ppow->pBoostsAllowed);

                        ppow->bFree = true;
                    }
#if CLIENT
#ifndef TEST_CLIENT

                    if (!isProductionMode())
                    {
                        if (ppow->eType == kPowerType_Boost)
                        {
                            uiEnhancementAddHelpText(NULL, ppow, 1, NULL);
                        }
                    }
#endif
#endif
                    if(ppow->bAutoIssue && !ppow->bFree)
                    {
                        ErrorFilenamef(ppow->pchSourceFile, "AutoIssue power (%s.%s.%s) not marked as free. It should be marked with \"Free  kTrue\".", pcat->pchName, pset->pchName, ppow->pchName);
                        ppow->bFree = true;
                    }
                }
                else
                {
                    ErrorFilenamef(pset->pchSourceFile, "Set %s.%s has empty power. Check set itself as well.", pcat->pchName, pset->pchName);
                } // power valid check
            } // for each power
        } // for each set
    } // for each category
}

static const DimReturnSet *pDimSetDefault = NULL;

/**********************************************************************func*
* powerdict_FinalizePower
*
* NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE
*
* This does all the per-power finalization, so we can call it seperately on power
* reloads. This must be called after FinalizeDict, as that sets the default dim return
*
*/
void powerdict_FinalizePower(BasePower *ppow, PowerDictionary *pdict)
{
    int i;
    bool bCheckSuppression = false;

    if (!ppow->psetParent || !ppow->psetParent->pcatParent)
    {
        const BasePower *temp = powerdict_GetBasePowerByFullName(pdict,ppow->pchFullName);
        if (temp && temp != ppow)
        {
            ErrorFilenamef(ppow->pchSourceFile,"Power (%s) is a duplicate of another power with the same name! Delete one of these powers.",ppow->pchFullName);
        }
        else
        {        
            ErrorFilenamef(ppow->pchSourceFile,"Power (%s) is not in a power set or category. This probably means the .powers file needs to be removed",ppow->pchFullName);
        }
        return;
    }

    if(ppow->pchChainIntoPowerName)
    {
        const BasePower *temp = powerdict_GetBasePowerByFullName(pdict,ppow->pchChainIntoPowerName);
        if(!temp)
            ErrorFilenamef(ppow->pchSourceFile,"Power (%s) links to non-existent ChainIntoPower (%s)",ppow->pchFullName,ppow->pchChainIntoPowerName);
    }
    if(ppow->pchBoostCatalystConversion)
    {
        const BasePower *temp = powerdict_GetBasePowerByFullName(pdict,ppow->pchBoostCatalystConversion);
        if(!temp)
            ErrorFilenamef(ppow->pchSourceFile,"Power (%s) links to non-existent BoostCatalystConversion (%s)",ppow->pchFullName,ppow->pchBoostCatalystConversion);
    }
    if(ppow->pchRewardFallback)
    {
        const BasePower *temp = powerdict_GetBasePowerByFullName(pdict,ppow->pchRewardFallback);
        if(!temp)
            ErrorFilenamef(ppow->pchSourceFile,"Power (%s) links to non-existent RewardFallback (%s)",ppow->pchFullName,ppow->pchRewardFallback);
    }

#if SERVER
    bCheckSuppression = (stricmp(ppow->psetParent->pcatParent->pchName, "Inherent") == 0
        || stricmp(ppow->psetParent->pcatParent->pchName, "Temporary_Powers") == 0
        || stricmp(ppow->psetParent->pcatParent->pchName, "Pool") == 0
        || stricmp(ppow->psetParent->pcatParent->pchName, "Pets") == 0
        || stricmp(ppow->psetParent->pcatParent->pchName, "Epic") == 0
        || strnicmp(ppow->psetParent->pcatParent->pchName, "Tanker",6) == 0
        || strnicmp(ppow->psetParent->pcatParent->pchName, "Scrapper",8) == 0
        || strnicmp(ppow->psetParent->pcatParent->pchName, "Blaster",7) == 0
        || strnicmp(ppow->psetParent->pcatParent->pchName, "Defender",8) == 0
        || strnicmp(ppow->psetParent->pcatParent->pchName, "Controller",10) == 0
        || strnicmp(ppow->psetParent->pcatParent->pchName, "Peacebringer",12) == 0
        || strnicmp(ppow->psetParent->pcatParent->pchName, "Warshade",8) == 0
        || strnicmp(ppow->psetParent->pcatParent->pchName, "Kheldian",8) == 0);
#endif

    if (ppow->eTargetType != kTargetType_Position
        && ppow->eTargetTypeSecondary != kTargetType_Position
        && (ppow->ePositionCenter || ppow->fPositionDistance || ppow->fPositionHeight || ppow->fPositionYaw))
    {
        ErrorFilenamef(ppow->pchSourceFile, "Power (%s) has Position information but does not have a Position target!", ppow->pchFullName);
    }
    else if (ppow->ePositionCenter != kModTarget_Caster && ppow->ePositionCenter != kModTarget_Focus)
    {
        ErrorFilenamef(ppow->pchSourceFile, "Power (%s) has an invalid PositionCenter!", ppow->pchFullName);
    }

    // NOTE ARM:  I'm not sure if this is the best spot for this, but it looks good enough...
    for (i = eaSize(&ppow->ppTemplates) - 1; i >= 0; i--)
    {
        ModApplicationType ppowApplicationType = ppow->ppTemplates[i]->eApplicationType;
        extern StaticDefineInt ModApplicationEnum[];
        const char *ppowApplicationTypeString = StaticDefineIntRevLookup(ModApplicationEnum, ppowApplicationType);

        if (ppowApplicationType == kModApplicationType_OnActivate 
            || ppowApplicationType == kModApplicationType_OnDeactivate
            || ppowApplicationType == kModApplicationType_OnExpire
            || ppowApplicationType == kModApplicationType_OnEnable
            || ppowApplicationType == kModApplicationType_OnDisable)
        {
            bool doesPowerHaveCasterAutoHit = false;
            int j;

            if (!stricmp(ppow->psetParent->pcatParent->pchName, "Set_Bonus"))
            {
                ErrorFilenamef(ppow->pchSourceFile, "Power (%s) cannot have a %s attrib mod on a set bonus power!", ppow->pchFullName, ppowApplicationTypeString);
            }

            if (ppow->eType == kPowerType_Boost)
            {
                ErrorFilenamef(ppow->pchSourceFile, "Power (%s) cannot have a %s attrib mod on a boost!", ppow->pchFullName, ppowApplicationTypeString);
            }
            else if (ppow->eType == kPowerType_GlobalBoost)
            {
                ErrorFilenamef(ppow->pchSourceFile, "Power (%s) cannot have a %s attrib mod on a global boost!", ppow->pchFullName, ppowApplicationTypeString);
            }

            if (ppow->ppTemplates[i]->eTarget != kModTarget_Caster
                        && ppow->ppTemplates[i]->eTarget != kModTarget_CastersOwnerAndAllPets)
            {
                ErrorFilenamef(ppow->pchSourceFile, "Power (%s) cannot have a %s attrib mod that doesn't target the caster!", ppow->pchFullName, ppowApplicationTypeString);
            }

            for (j = 0; j < eaiSize(&(int*)ppow->pAutoHit); j++)
            {
                if (ppow->pAutoHit[j] == kTargetType_Caster)
                {
                    doesPowerHaveCasterAutoHit = true;
                }
            }

            if (!doesPowerHaveCasterAutoHit)
            {
                ErrorFilenamef(ppow->pchSourceFile, "Power (%s) cannot have a %s attrib mod on a power that doesn't auto-hit the caster!", ppow->pchFullName, ppowApplicationTypeString);
            }
        }

        if (ppowApplicationType == kModApplicationType_OnActivate 
            || ppowApplicationType == kModApplicationType_OnDeactivate
            || ppowApplicationType == kModApplicationType_OnExpire)
        {
            if (ppow->iTimeToConfirm > 0)
            {
                ErrorFilenamef(ppow->pchSourceFile, "Power (%s) cannot have a %s attrib mod on a power that requires confirmation!", ppow->pchFullName, ppowApplicationTypeString);
            }
        }

        if (ppowApplicationType == kModApplicationType_OnDeactivate
            || ppowApplicationType == kModApplicationType_OnExpire)
        {
            if (ppow->eTargetType == kTargetType_Location)
            {
                ErrorFilenamef(ppow->pchSourceFile, "Power (%s) cannot have a %s attrib mod on a TargetType Location power!", ppow->pchFullName, ppowApplicationTypeString);
            }
            else if (ppow->eTargetType == kTargetType_Teleport)
            {
                ErrorFilenamef(ppow->pchSourceFile, "Power (%s) cannot have a %s attrib mod on a TargetType Teleport power!", ppow->pchFullName, ppowApplicationTypeString);
            }
            else if (ppow->eTargetType == kTargetType_Position)
            {
                ErrorFilenamef(ppow->pchSourceFile, "Power (%s) cannot have a %s attrib mod on a TargetType Position power!", ppow->pchFullName, ppowApplicationTypeString);
            }
        }
            
        if (ppowApplicationType == kModApplicationType_OnDeactivate
            || ppowApplicationType == kModApplicationType_OnExpire
            || ppowApplicationType == kModApplicationType_OnDisable)
        {
            if (ppow->bDoNotSave)
            {
                ErrorFilenamef(ppow->pchSourceFile, "Power (%s) cannot have a %s attrib mod on a DoNotSave power!", ppow->pchFullName, ppowApplicationTypeString);
            }
        }

        if (ppowApplicationType == kModApplicationType_OnEnable
            || ppowApplicationType == kModApplicationType_OnDisable)
        {
            if (ppow->eType == kPowerType_Inspiration)
            {
                ErrorFilenamef(ppow->pchSourceFile, "Power (%s) cannot have a %s attrib mod on an inspiration!", ppow->pchFullName, ppowApplicationTypeString);
            }
        }
    }

    //
    // See if this power has any usage limits
    //

    if(ppow->iNumCharges>0)
    {
        ppow->bHasUseLimit = true;
    }
    if(ppow->fUsageTime>0)
    {
        ppow->bHasUseLimit = true;
    }
    if(ppow->fLifetime>0 || ppow->fLifetimeInGame>0)
    {
        ppow->bHasLifetime = true;
    }

    if (ppow->bStackingUsage && ppow->iNumAllowed > 1)
        ErrorFilenamef(ppow->pchSourceFile,"Power (%s) has StackingUsage with time limits set but allows more than one copy of the power.",ppow->pchFullName);

    //
    // Set up the diminishing returns information for the power
    //
    for(i=0; i<eaSize(&g_DimReturnList.ppSets); i++)
    {
        int j;

        if(pDimSetDefault == g_DimReturnList.ppSets[i])
            continue;

        for(j=0; j<eaiSize(&g_DimReturnList.ppSets[i]->piBoosts); j++)
        {
            int k;
            for(k=0; k<eaiSize(&ppow->pBoostsAllowed); k++)
            {
                if(ppow->pBoostsAllowed[k]==g_DimReturnList.ppSets[i]->piBoosts[j])
                {
                    int l;
                    for(l=0; l<eaSize(&g_DimReturnList.ppSets[i]->ppReturns); l++)
                    {
                        int m;
                        if(g_DimReturnList.ppSets[i]->ppReturns[l]->bDefault)
                        {
                            ErrorFilenamef("defs/dim_returns.def", "Illegal AttribReturnSet marked as default. Only one AttribReturnSet can be\nmarked as default, and this set must be in the Default ReturnSet.");
                        }

                        for(m=0; m<eaiSize(&g_DimReturnList.ppSets[i]->ppReturns[l]->offAttribs); m++)
                        {
                            int idx = g_DimReturnList.ppSets[i]->ppReturns[l]->offAttribs[m]/sizeof(float);
                            if(ppow->apppDimReturns[idx] && ppow->apppDimReturns[idx]!=&g_DimReturnList.ppSets[i]->ppReturns[l]->pReturns)
                            {
                                int iter;
                                bool equivalent = true;

                                iter = eaSize(&g_DimReturnList.ppSets[i]->ppReturns[l]->pReturns) - 1;
                                if(iter != eaSize(ppow->apppDimReturns[idx]) - 1)
                                {
                                    equivalent = false;
                                }
                                else
                                {
                                    for( ; iter >= 0; iter--)
                                    {
                                        // If all the data for the diminishing returns set is the same, then there is no actual conflict
                                        const DimReturn *powDimReturn = (*(ppow->apppDimReturns[idx]))[iter];
                                        const DimReturn *setDimReturn = g_DimReturnList.ppSets[i]->ppReturns[l]->pReturns[iter];
                                        if(powDimReturn->fStart != setDimReturn->fStart || powDimReturn->fHandicap != setDimReturn->fHandicap ||
                                            powDimReturn->fBasis != setDimReturn->fBasis)
                                        {
                                            equivalent = false;
                                            break;
                                        }
                                    }
                                }
                                
                                if(!equivalent)
                                    ErrorFilenamef("defs/dim_returns.def", "Power (%s) has conflicting diminishing return info!", ppow->pchSourceName);
                            }
                            ppow->apppDimReturns[idx] = &g_DimReturnList.ppSets[i]->ppReturns[l]->pReturns;
                        }
                    }
                }
            }
        }
    }

    // And now stick in the defaults
    {
        int l;
        AttribDimReturnSet *pDefAttribDimReturnSet = NULL;

        for(l=0; l<eaSize(&pDimSetDefault->ppReturns); l++)
        {
            int m;
            if(pDimSetDefault->ppReturns[l]->bDefault)
            {
                pDefAttribDimReturnSet = pDimSetDefault->ppReturns[l];
                continue;
            }

            for(m=0; m<eaiSize(&pDimSetDefault->ppReturns[l]->offAttribs); m++)
            {
                int idx = pDimSetDefault->ppReturns[l]->offAttribs[m]/sizeof(float);
                if(!ppow->apppDimReturns[idx])
                    ppow->apppDimReturns[idx] = &pDimSetDefault->ppReturns[l]->pReturns;
            }
        }


        for(i=0; i<CHAR_ATTRIB_COUNT; i++)
        {
            if(!ppow->apppDimReturns[i])
                ppow->apppDimReturns[i] = &pDefAttribDimReturnSet->pReturns;
        }
    }

#if SERVER
    //
    // Clean up stuff in the attribmods
    //
    {
        int imod;
        int iSize = eaSize(&ppow->ppTemplates);
        bool foundRedirectPower = false;

        ppow->bHasInvokeSuppression = false; // default, set below if true
        ppow->bHasNonBoostTemplates = false; // ditto

        for(imod=0; imod<iSize; imod++)
        {
            AttribModTemplate *ptemplate = cpp_const_cast(AttribModTemplate*)(ppow->ppTemplates[imod]);

            ptemplate->iIdx = imod;

            // If this is a var lookup, then get the index. If this
            //   lookup fails, then a class table is used instead.
            ptemplate->iVarIndex = basepower_GetVarIdx(ppow, ptemplate->pchTable);

            // Validate any Power Redirection links
            if(ptemplate->offAttrib == kSpecialAttrib_PowerRedirect)
            {
                if(!ppow->bPowerRedirector)
                    ErrorFilenamef(ppow->pchSourceFile,"Power (%s) has PowerRedirect attribMods but is not a PowerRedirector",ppow->pchFullName);
                if(!ptemplate->ppchApplicationRequires)
                    ErrorFilenamef(ppow->pchSourceFile,"Power (%s) has PowerRedirect attribMod without an ApplicationRequires",ppow->pchFullName);
                
                if(!ptemplate->ppchPrimaryStringList[0])
                {
                    ErrorFilenamef(ppow->pchSourceFile,"Power (%s) has PowerRedirect attribMod without an PrimaryStringList (power to redirect)",ppow->pchFullName);
                }
                else
                {
                    const BasePower *temp = powerdict_GetBasePowerByFullName(pdict,ptemplate->ppchPrimaryStringList[0]);
                    if(!temp)
                        ErrorFilenamef(ppow->pchSourceFile,"Power (%s) has PowerRedirect attribMod link to non-existent Power (%s)",ppow->pchFullName,ptemplate->ppchPrimaryStringList[0]);
                    else
                    {
                        foundRedirectPower = true;
                        if (isDevelopmentMode())
                        {
                            if (temp->eType != ppow->eType)
                                ErrorFilenamef(ppow->pchSourceFile,"Power (%s) has PowerRedirect attribMod link to a power (%s)that isn't the same type (%i:%i)", ppow->pchFullName,temp->pchFullName, ppow->eType, temp->eType);
                            if (temp->bDeletable != ppow->bDeletable)
                                ErrorFilenamef(ppow->pchSourceFile,"Power (%s) has PowerRedirect attribMod link to a power (%s)that isn't the same deletable (%i:%i)", ppow->pchFullName,temp->pchFullName, ppow->bDeletable, temp->bDeletable);
                            if (temp->bTradeable != ppow->bTradeable)
                                ErrorFilenamef(ppow->pchSourceFile,"Power (%s) has PowerRedirect attribMod link to a power (%s)that isn't the same bTradeable (%i:%i)", ppow->pchFullName,temp->pchFullName, ppow->bTradeable, temp->bTradeable);
                            if (temp->bDoNotSave != ppow->bDoNotSave)
                                ErrorFilenamef(ppow->pchSourceFile,"Power (%s) has PowerRedirect attribMod link to a power (%s)that isn't the same bDoNotSave (%i:%i)", ppow->pchFullName,temp->pchFullName, ppow->bDoNotSave, temp->bDoNotSave);
                            if (temp->iNumCharges != ppow->iNumCharges)
                                ErrorFilenamef(ppow->pchSourceFile,"Power (%s) has PowerRedirect attribMod link to a power (%s)that isn't the same charge limit (%i:%i)", ppow->pchFullName,temp->pchFullName, ppow->iNumCharges, temp->iNumCharges);
                            if (temp->fUsageTime != ppow->fUsageTime)
                                ErrorFilenamef(ppow->pchSourceFile,"Power (%s) has PowerRedirect attribMod link to a power (%s)that isn't the same usageTime (%f:%f)", ppow->pchFullName,temp->pchFullName, ppow->fUsageTime, temp->fUsageTime);
                            if (temp->bIgnoreToggleMaxDistance != ppow->bIgnoreToggleMaxDistance)
                                ErrorFilenamef(ppow->pchSourceFile,"Power (%s) has PowerRedirect attribMod link to a power (%s)that isn't the same bIgnoreToggleMaxDistance (%i:%i)", ppow->pchFullName,temp->pchFullName, ppow->bIgnoreToggleMaxDistance, temp->bIgnoreToggleMaxDistance);
                            if (temp->eTargetTypeSecondary != ppow->eTargetTypeSecondary)
                                ErrorFilenamef(ppow->pchSourceFile,"Power (%s) has PowerRedirect attribMod link to a power (%s)that isn't the same eTargetTypeSecondary (%i:%i)", ppow->pchFullName,temp->pchFullName, ppow->eTargetTypeSecondary, temp->eTargetTypeSecondary);
                        }
                    }
                }
            }
            // Validate Granted and Revoke Power attribs
            else if(ptemplate->offAttrib == kSpecialAttrib_GrantPower || ptemplate->offAttrib == kSpecialAttrib_GrantBoostedPower ||
                ptemplate->offAttrib == kSpecialAttrib_RevokePower)
            {
                if(ptemplate->pchReward)
                {
                    const BasePower *temp = powerdict_GetBasePowerByFullName(&g_PowerDictionary, ptemplate->pchReward);
                    if(!temp)
                        ErrorFilenamef(ppow->pchSourceFile,"Power (%s) has Grant/Revoke attribMod link to non-existent Power (%s)",ppow->pchFullName,ptemplate->pchReward);
                }
                else
                    ErrorFilenamef(ppow->pchSourceFile,"Power (%s) has Grant/Revoke attribMod without a Reward (Power reference)",ppow->pchFullName);
            }
            else if(ptemplate->offAttrib == kSpecialAttrib_TokenAdd || ptemplate->offAttrib == kSpecialAttrib_TokenSet ||
                ptemplate->offAttrib == kSpecialAttrib_TokenClear)
            {
                if(!ptemplate->pchReward)
                    ErrorFilenamef(ppow->pchSourceFile,"Power (%s) has Token change attribMod without a Reward (Token reference)",ppow->pchFullName);
            }

            //
            // Calculate bSuppressMethodAlways
            //
            ptemplate->bSuppressMethodAlways = false;

            // Check BoostTemplate mode
            if(!ptemplate->bBoostTemplate)
            {
                ppow->bHasNonBoostTemplates = true;
            }

            // Designer laziness fixuppery
            if(ptemplate->fRadiusOuter == 0.0f && ptemplate->fRadiusInner < 0.0f)
            {
                ptemplate->fRadiusInner = 0.0f;;
            }

            if (ptemplate->ppSuppress)
            {
                int iSizeSup = eaSize(&ptemplate->ppSuppress);
                bool bSuppressAlways = false;
                int iSup;

                if (iSizeSup)
                {
                    bSuppressAlways = ptemplate->ppSuppress[0]->bAlways;

                    for (iSup = 1; iSup < iSizeSup; iSup++)
                    {
                        if (ptemplate->ppSuppress[iSup]->bAlways != bSuppressAlways)
                        {
                            // Mismatch! Complain, set always to false, and break
                            ErrorFilenamef(ppow->pchSourceFile, "Suppression Always flags of attribmod %d on (%s) are inconsistent.\nSetting SuppressMethodAlways false.\n", imod, ppow->pchSourceName);
                            bSuppressAlways = false;
                            break;
                        }
                    }

                    for (iSup = 0; iSup < iSizeSup; iSup++)
                    {
                        int iEvent = ptemplate->ppSuppress[iSup]->idxEvent;
                        if (POWEREVENT_INVOKE(iEvent))
                            ppow->bHasInvokeSuppression = true;
                    }
                }

                ptemplate->bSuppressMethodAlways = bSuppressAlways;
            }

            // Make sure that if this attribmod should be suppressed, that it is
            if (bCheckSuppression
                && !ptemplate->ppSuppress
                && ptemplate->eTarget != kModTarget_Caster
                && IS_MODIFIER(ptemplate->offAspect)
                && ptemplate->fScale >= 0
                && (ptemplate->offAttrib == offsetof(CharacterAttributes, fTerrorized)
                || ptemplate->offAttrib == offsetof(CharacterAttributes, fImmobilized)
                || ptemplate->offAttrib == offsetof(CharacterAttributes, fHeld)
                || ptemplate->offAttrib == offsetof(CharacterAttributes, fStunned)
                || ptemplate->offAttrib == offsetof(CharacterAttributes, fSleep))
                )
            {
                bool bHitsPlayers = !ptemplate->ppchApplicationRequires;
                if (!bHitsPlayers)
                {
                    int n = eaSize(&ptemplate->ppchApplicationRequires);
                    int i;
                    for (i = 0; i < n; i++)
                    {
                        if (strcmp("player",ptemplate->ppchApplicationRequires[i]) == 0)
                            bHitsPlayers = true;
                    }
                }

                if (bHitsPlayers)
                {
                    int n = eaiSize(&(int*)ppow->pAffected);
                    int i;
                    bHitsPlayers = false;
                    for (i = 0; i < n; i++)
                    {
                        if (ppow->pAffected[i] == kTargetType_Foe)
                            bHitsPlayers = true;
                    }
                }

                if(bHitsPlayers)
                {
                    if(ptemplate->pchIgnoreSuppressErrors && *ptemplate->pchIgnoreSuppressErrors)
                    {
                        // Suppressed
                    }
                    else
                    {
                        ErrorFilenamef(ppow->pchSourceFile, "AttribMod %d of (%s) should probably have supression on it but doesn't.\nIf you are certain that this is correct, you can put \"IgnoreSuppressErrors %s\" on this attribmod to ignore this discrepency.\n",
                            imod+1, ppow->pchSourceName,
                            getUserName());
                    }
                }
                else
                {
                    if(ptemplate->pchIgnoreSuppressErrors && *ptemplate->pchIgnoreSuppressErrors)
                    {
                        ErrorFilenamef(ppow->pchSourceFile, "AttribMod %d of (%s): %s used IgnoreSuppressErrors and didn't need to.\n",
                            imod+1, ppow->pchSourceName, ptemplate->pchIgnoreSuppressErrors);
                    }
                }
            }
        } // for each attrib mod template

        if(ppow->bPowerRedirector && !foundRedirectPower)
            ErrorFilenamef(ppow->pchSourceFile,"Power (%s) is a PowerRedirector but links to no valid powers",ppow->pchFullName);
    }
#else
    {
        int imod;
        int iSize = eaSize(&ppow->ppTemplates);
        for(imod=0; imod<iSize; imod++)
        {
            AttribModTemplate *ptemplate = cpp_const_cast(AttribModTemplate*)(ppow->ppTemplates[imod]);

            ptemplate->iIdx = imod;

            // If this is a var lookup, then get the index. If this
            //   lookup fails, then a class table is used instead.
            ptemplate->iVarIndex = basepower_GetVarIdx(ppow, ptemplate->pchTable);
        } // for each attrib mod template
    }
#endif


}

/**********************************************************************func*
 * powerdict_FinalizeDict
 *
 * NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE
 *
 * This function is done after loading the powers information from the bin
 * file. Anything which is saved to the bin file should be done in
 * powerdict_FinalizeForBin (above). This function is called after all
 * the information that the client doesn't need has been stripped out.
 * This is called before the data is put in shared memory, so pointers to
 * this data will break.
 *
 * This is all the finalization that cares about power sets
 *
 */
static void powerdict_FinalizeDictFx(PowerFX *fx)
{
    fx->fTimeToHit = fx->iFramesBeforeHit/30.0f;
    fx->fInitialTimeToHit = fx->iInitialFramesBeforeHit/30.0f;
    fx->fTimeToBlock = fx->iFramesBeforeBlock/30.0f;
    fx->fInitialTimeToBlock = fx->iInitialFramesBeforeBlock/30.0f;
}

int powerdict_compareCategories(const void* leftData, const void* rightData)
{
    const PowerCategory ** left = (const PowerCategory **)leftData;
    const PowerCategory ** right = (const PowerCategory **)rightData;
    return strcmp((*left)->pchName, (*right)->pchName);
}

void powerdict_FinalizeDict(PowerDictionary *pdict)
{
    int i;
    int icat;
    int iSizeCat;
    pDimSetDefault = NULL;

    assert(pdict!=NULL);

    // Find the default DimReturnSet
    for(i=0; i<eaSize(&g_DimReturnList.ppSets); i++)
    {
        if(g_DimReturnList.ppSets[i]->bDefault)
        {
            pDimSetDefault = g_DimReturnList.ppSets[i];
            break;
        }
    }
    if(!pDimSetDefault)
        ErrorFilenamef("defs/dim_returns.def", "No default ReturnSet defined!");

    eaQSortConst(pdict->ppPowerCategories, powerdict_compareCategories);

    iSizeCat = eaSize(&pdict->ppPowerCategories);
    for(icat=0; icat<iSizeCat; icat++)
    {
        int iset;
        PowerCategory *pcat = (PowerCategory*)pdict->ppPowerCategories[icat];
        int iSizeSets = eaSize(&pcat->ppPowerSets);

        for(iset=0; iset<iSizeSets; iset++)
        {
            int ipow;
            BasePowerSet *pset = (BasePowerSet*)pcat->ppPowerSets[iset];
            int iSizePows = eaSize(&pset->ppPowers);

            pset->pcatParent = pcat;

            for(ipow=0; ipow<iSizePows; ipow++)
            {
                int imod, ifx;
                BasePower *ppow = (BasePower*)pset->ppPowers[ipow];

                ppow->psetParent = pset;

                basepower_SetID(ppow, icat, iset, ipow);

                //Allows artist their own special pfx testing file, loads the tpfx.txt into all art test powers
                {
                    extern int artTestPowerLoaded;
                    if( isDevelopmentMode() && artTestPowerLoaded && 0 == stricmp( pset->pchName, "Art_Test" ) )
                    {
                        extern BasePower artTestPower;
                        ppow->fx = artTestPower.fx;  //Structure copy
                        ppow->fTimeToActivate = ((F32)ppow->fx.iFramesAttack)/30.0; //Because AL said they need to be the same
                    }
                }
                //End pfx testing file

                powerdict_FinalizeDictFx(&ppow->fx);
                for(ifx = eaSize(&ppow->customfx)-1; ifx >= 0; --ifx)
                {
                    PowerCustomFX *customfx = cpp_const_cast(PowerCustomFX*)(ppow->customfx[ifx]); 
                    powerdict_FinalizeDictFx(&customfx->fx);
                }

                for(imod=eaSize(&ppow->ppTemplates)-1; imod>=0; imod--)
                {
                    AttribModTemplate *mod = cpp_const_cast(AttribModTemplate*)(ppow->ppTemplates[imod]);
                    mod->ppowBase = ppow;
                }

            } // for each power
        } // for each set
    } // for each category

    powerdict_ResetStats(pdict);
}

void powerdict_FindSharedPowersets(const PowerDictionary *pdict)
{
    int icat;
    int iSizeCat;

    iSizeCat = eaSize(&pdict->ppPowerCategories);
    for(icat=0; icat<iSizeCat; icat++)
    {
        int iset;
        PowerCategory *pcat = (PowerCategory*)pdict->ppPowerCategories[icat];
        int iSizeSets = eaSize(&pcat->ppPowerSets);

        for(iset=0; iset<iSizeSets; iset++)
        {
            BasePowerSet *pset = (BasePowerSet*)pcat->ppPowerSets[iset];

            if (pset->bIsShared)
            {
                eaPush(&g_SharedPowersets, pset);
            }
        }
    }

    g_numSharedPowersets = eaSize(&g_SharedPowersets);
}

/**********************************************************************func*
 * powerdict_SetBackPointers
 *
 * Sets the backpointers on powers. This is called after things are loaded into 
 * shared memory while we can still write to it
 *
 */
void powerdict_SetBackPointers(PowerDictionary *pdict)
{
    int icat;

    assert(pdict!=NULL);

    // Put all the backreferences in the power sets, powers, and templates
    for(icat=eaSize(&pdict->ppPowerCategories)-1; icat>=0; icat--)
    {
        int iset;
        PowerCategory *pcat = (PowerCategory*)pdict->ppPowerCategories[icat];

        for(iset=eaSize(&pcat->ppPowerSets)-1; iset>=0; iset--)
        {
            int ipow;
            BasePowerSet *pset = (BasePowerSet*)pcat->ppPowerSets[iset];

            pset->pcatParent = pcat;    

            for(ipow=eaSize(&pset->ppPowers)-1; ipow>=0; ipow--)
            {
                int imod;
                BasePower *ppow = (BasePower*)pset->ppPowers[ipow];

                ppow->psetParent = pset;

                for(imod=eaSize(&ppow->ppTemplates)-1; imod>=0; imod--)
                {
                    AttribModTemplate *mod = cpp_const_cast(AttribModTemplate*)(ppow->ppTemplates[imod]);
                    mod->ppowBase = ppow;
                    mod->ppowBaseIndex = imod;
                }
            }
        }
    }
}

/**********************************************************************func*
 * powerdict_FinalizeUnshared
 *
 * This function is called after all the shared memory crap has been done.
 * Unshared globals based on the shared data are set in here.
 *
 */
void powerdict_FinalizeUnshared(const PowerDictionary *pdict)
{
    assert(pdict!=NULL);

    // Get the values for the automatically-set modes.
    {
        const char *pch;

        if((pch=StaticDefineLookup(ParsePowerDefines, "kRaid_Attacker_Mode"))==0)
            ErrorFilenamef("Defs/attrib_names.def", "Unable to find required mode: Raid_Attacker_Mode\n");
        else
            g_eRaidAttackerMode = atoi(pch);

        if((pch=StaticDefineLookup(ParsePowerDefines, "kRaid_Defender_Mode"))==0)
            ErrorFilenamef("Defs/attrib_names.def", "Unable to find required mode: Raid_Defender_Mode\n");
        else
            g_eRaidDefenderMode = atoi(pch);

        if((pch=StaticDefineLookup(ParsePowerDefines, "kCoOpTeam"))==0)
            ErrorFilenamef("Defs/attrib_names.def", "Unable to find required mode: CoOpTeam\n");
        else
            g_eOnCoopTeamMode = atoi(pch);
    }

}

#ifdef HASH_POWERS
/**********************************************************************func*
 * powerdict_CreateHashes
 *
 */
void powerdict_CreateHashes(const PowerDictionary *pdict)
{

    int icat;

    assert(pdict!=NULL);

    pdict->haCategoryNames = stashTableCreateWithStringKeys(2*eaSize(&pdict->ppPowerCategories), StashDeepCopyKeys);

    for(icat=eaSize(&pdict->ppPowerCategories)-1; icat>=0; icat--)
    {
        int iset;
        PowerCategory *pcat = pdict->ppPowerCategories[icat];

        stashAddPointer(pdict->haCategoryNames, pcat->pchName, pcat, false);
        pcat->haSetNames = stashTableCreateWithStringKeys(2*eaSize(&pcat->ppPowerSets), StashDeepCopyKeys);

        for(iset=eaSize(&pcat->ppPowerSets)-1; iset>=0; iset--)
        {
            int ipow;
            BasePowerSet *pset = pcat->ppPowerSets[iset];

            stashAddPointer(pcat->haSetNames, pset->pchName, pset, false)
            pset->haPowerNames = stashTableCreateWithStringKeys(2*eaSize(&pset->ppPowers), StashDeepCopyKeys);

            for(ipow=eaSize(&pset->ppPowers)-1; ipow>=0; ipow--)
            {
                BasePower *ppow = pset->ppPowers[ipow];

                stashAddPointer(pset->haPowerNames, ppow->pchName, ppow, false);

            }
        }
    }

    // --------------------
    // build the recipes inventory

    //powerdictionary_CreateRecipeHashes( pdict );

}
#endif

/**********************************************************************func*
* powerdict_CreateDictHashes
* 
* Create the hashes for the power, set, and category dictionaries
*
*/

bool powerdict_CreateDictHashes(PowerDictionary *pdict, bool shared_memory)
{
    bool ret = true;
    int i;

    assert(!pdict->haCategoryNames);
    pdict->haCategoryNames = stashTableCreateWithStringKeys(stashOptimalSize(eaSize(&pdict->ppPowerCategories)), stashShared(shared_memory));

    for(i=0; i<eaSize(&pdict->ppPowerCategories); i++)
    {
        PowerCategory *pcat = (PowerCategory*)pdict->ppPowerCategories[i];        
        if (!stashAddPointerConst(pdict->haCategoryNames, pcat->pchName, pcat, false))
        {
            ErrorFilenamef(pcat->pchSourceFile,"Duplicate power category name: %s",pcat->pchName);
            ret = false;
        }
    }

    assert(!pdict->haSetNames);
    pdict->haSetNames = stashTableCreateWithStringKeys(stashOptimalSize(eaSize(&pdict->ppPowerSets)), stashShared(shared_memory));

    for(i=0; i<eaSize(&pdict->ppPowerSets); i++)
    {
        BasePowerSet *pset = (BasePowerSet*)pdict->ppPowerSets[i];

        if (!stashAddPointerConst(pdict->haSetNames, pset->pchFullName, pset, false))
        {
            ErrorFilenamef(pset->pchSourceFile,"Duplicate power set name: %s",pset->pchFullName);
            ret = false;
        }
    }

    assert(!pdict->haPowerCRCs);
    pdict->haPowerCRCs = stashTableCreate(stashOptimalSize(eaSize(&pdict->ppPowers)), stashShared(shared_memory), StashKeyTypeInts, sizeof(int));

    for(i=0; i<eaSize(&pdict->ppPowers); i++)
    {
        BasePower *ppow = (BasePower*)pdict->ppPowers[i];
        int CRC = powerdict_GetCRCOfBasePower(ppow);
        if (!stashIntAddPointerConst(pdict->haPowerCRCs, CRC, ppow, false))
        {
            ErrorFilenamef(ppow->pchSourceFile,"Duplicate power name: %s",ppow->pchFullName);
            ret = false;
        }
    }

    return ret;
}

/**********************************************************************func*
* powerdict_DestroyDictHashes
* 
* Destroy the dictionary hashes, which is good if we need to reload things
*
*/

void powerdict_DestroyDictHashes(PowerDictionary *pdict)
{
    if (pdict->haCategoryNames)
    {
        stashTableDestroyConst(pdict->haCategoryNames);
        pdict->haCategoryNames = NULL;
    }
    if (pdict->haSetNames)
    {
        stashTableDestroyConst(pdict->haSetNames);
        pdict->haSetNames = NULL;
    }
    if (pdict->haPowerCRCs)
    {
        stashTableDestroyConst(pdict->haPowerCRCs);
        pdict->haPowerCRCs = NULL;
    }
}

/***************************************************************************/
/***************************************************************************/
/***************************************************************************/

StaticDefineInt AIReportEnum[] =
{
    DEFINE_INT
    { "kAlways",             kAIReport_Always    },
    { "kNever",              kAIReport_Never     },
    { "kHitOnly",            kAIReport_HitOnly   },
    { "kMissOnly",           kAIReport_MissOnly  },
    DEFINE_END
};

StaticDefineInt EffectAreaEnum[] =
{
    DEFINE_INT
    { "kCharacter",          kEffectArea_Character  },
    { "kChar",               kEffectArea_Character  },
    { "kCone",               kEffectArea_Cone       },
    { "kSphere",             kEffectArea_Sphere     },
    { "kLocation",           kEffectArea_Location   },
    { "kVolume",             kEffectArea_Volume     },
    { "kNamedVolume",        kEffectArea_NamedVolume},  //Not implemented
    { "kMap",                kEffectArea_Map        },
    { "kRoom",               kEffectArea_Room       },
    { "kTouch",              kEffectArea_Touch      },
    { "kBox",                kEffectArea_Box        },
    DEFINE_END
};

StaticDefineInt TargetVisibilityEnum[] =
{
    DEFINE_INT
    { "kLineOfSight",        kTargetVisibility_LineOfSight  },
    { "kNone",               kTargetVisibility_None         },
    DEFINE_END
};

StaticDefineInt TargetTypeEnum[] =
{
    DEFINE_INT
    { "kCaster",                            kTargetType_Caster                                },
    { "kPlayer",                            kTargetType_Player                                },
    { "kPlayerHero",                        kTargetType_PlayerHero                            },
    { "kPlayerVillain",                        kTargetType_PlayerVillain                        },
    { "kDeadPlayer",                        kTargetType_DeadPlayer                            },
    { "kDeadPlayerFriend",                    kTargetType_DeadPlayerFriend                    },
    { "kDeadPlayerFoe",                        kTargetType_DeadPlayerFoe                        },
    { "kTeammate",                            kTargetType_Teammate                            },
    { "kDeadTeammate",                        kTargetType_DeadTeammate                        },
    { "kDeadOrAliveTeammate",                kTargetType_DeadOrAliveTeammate                    },
    { "kFriend",                            kTargetType_Friend                                },
    { "kEnemy",                                kTargetType_Villain                                },
    { "kVillain",                            kTargetType_Villain                                },
    { "kDeadVillain",                        kTargetType_DeadVillain                            },
    { "kFoe",                                kTargetType_Foe                                    },
    { "kNPC",                                kTargetType_NPC                                    },
    { "kLocation",                            kTargetType_Location                            },
    { "kTeleport",                            kTargetType_Teleport                            },
    { "kDeadFoe",                            kTargetType_DeadFoe                                },
    { "kDeadOrAliveFoe",                    kTargetType_DeadOrAliveFoe                        },
    { "kDeadFriend",                        kTargetType_DeadFriend                            },
    { "kDeadOrAliveFriend",                    kTargetType_DeadOrAliveFriend                    },
    { "kMyPet",                                kTargetType_MyPet                                },
    { "kDeadMyPet",                            kTargetType_DeadMyPet                            },
    { "kDeadOrAliveMyPet",                    kTargetType_DeadOrAliveMyPet                    },
    { "kMyOwner",                            kTargetType_MyOwner                                },
    { "kMyCreator",                            kTargetType_MyCreator                            },
    { "kMyCreation",                        kTargetType_MyCreation                            },
    { "kDeadMyCreation",                    kTargetType_DeadMyCreation                        },
    { "kDeadOrAliveMyCreation",                kTargetType_DeadOrAliveMyCreation                },
    { "kLeaguemate",                        kTargetType_Leaguemate                            },
    { "kDeadLeaguemate",                    kTargetType_DeadLeaguemate                        },
    { "kDeadOrAliveLeaguemate",                kTargetType_DeadOrAliveLeaguemate                },
    { "kAny",                                kTargetType_Any                                    },
    { "kPosition",                            kTargetType_Position                            },
    { "kNone",                                0                                                },
    DEFINE_END
};

StaticDefineInt PowerTypeEnum[] =
{
    DEFINE_INT
    { "kClick",            kPowerType_Click        },
    { "kAuto",            kPowerType_Auto            },
    { "kToggle",        kPowerType_Toggle        },
    { "kBoost",            kPowerType_Boost        },
    { "kInspiration",    kPowerType_Inspiration    },
    { "kGlobalBoost",    kPowerType_GlobalBoost    },
    DEFINE_END
};

StaticDefineInt BoolEnum[] =
{
    DEFINE_INT
    { "false",               0  },
    { "kFalse",              0  },
    { "true",                1  },
    { "kTrue",               1  },
    DEFINE_END
};

StaticDefineInt ToggleDroppableEnum[] =
{
    DEFINE_INT
    { "kSometimes", kToggleDroppable_Sometimes },
    { "kAlways",    kToggleDroppable_Always    },
    { "kFirst",     kToggleDroppable_First     },
    { "kLast",      kToggleDroppable_Last      },
    { "kNever",     kToggleDroppable_Never     },

    { "Sometimes",  kToggleDroppable_Sometimes },
    { "Always",     kToggleDroppable_Always    },
    { "First",      kToggleDroppable_First     },
    { "Last",       kToggleDroppable_Last      },
    { "Never",      kToggleDroppable_Never     },
    DEFINE_END
};

StaticDefineInt ShowPowerSettingEnum[] =
{
    DEFINE_INT
    // Based on BoolEnum
    { "false",        kShowPowerSetting_Never        },
    { "kFalse",        kShowPowerSetting_Never        },
    { "true",        kShowPowerSetting_Default    },
    { "kTrue",        kShowPowerSetting_Default    },
    // New strings
    { "Never",        kShowPowerSetting_Never        },
    { "kNever",        kShowPowerSetting_Never        },
    { "Always",        kShowPowerSetting_Always    },
    { "kAlways",    kShowPowerSetting_Always    },
    { "Default",    kShowPowerSetting_Default    },
    { "kDefault",    kShowPowerSetting_Default    },
    { "IfUsable",    kShowPowerSetting_IfUsable    },
    { "kIfUsable",    kShowPowerSetting_IfUsable    },
    { "IfOwned",    kShowPowerSetting_IfOwned    },
    { "kIfOwned",    kShowPowerSetting_IfOwned    },
    DEFINE_END
};

TokenizerParseInfo ParsePowerVar[] =
{
    { "Index",              TOK_STRUCTPARAM | TOK_INT(PowerVar, iIndex, 0),  },
    { "Name",               TOK_STRUCTPARAM | TOK_STRING(PowerVar, pchName, 0)  },
    { "Min",                TOK_STRUCTPARAM | TOK_F32(PowerVar, fMin, 0)     },
    { "Max",                TOK_STRUCTPARAM | TOK_F32(PowerVar, fMax, 0)     },
    { "\n",                 TOK_END, 0 },
    { "", 0, 0 }
};

// These are all for the visual effects. This supports the old, goofy
// lf_gamer names as well as the new, sensible ones.
#define ParsePowerFX(type, fx) \
    { "AttackBits",                   TOK_INTARRAY(type, fx.piAttackBits),          ParsePowerDefines }, \
    { "BlockBits",                    TOK_INTARRAY(type, fx.piBlockBits),           ParsePowerDefines }, \
    { "WindUpBits",                   TOK_INTARRAY(type, fx.piWindUpBits),          ParsePowerDefines }, \
    { "HitBits",                      TOK_INTARRAY(type, fx.piHitBits),             ParsePowerDefines }, \
    { "DeathBits",                    TOK_INTARRAY(type, fx.piDeathBits),           ParsePowerDefines }, \
    { "ActivationBits",               TOK_INTARRAY(type, fx.piActivationBits),      ParsePowerDefines }, \
    { "DeactivationBits",             TOK_INTARRAY(type, fx.piDeactivationBits),    ParsePowerDefines }, \
    { "InitialAttackBits",            TOK_INTARRAY(type, fx.piInitialAttackBits),   ParsePowerDefines }, \
    { "ContinuingBits",               TOK_INTARRAY(type, fx.piContinuingBits),      ParsePowerDefines }, \
    { "ConditionalBits",              TOK_INTARRAY(type, fx.piConditionalBits),     ParsePowerDefines }, \
    { "ActivationFX",                 TOK_FILENAME(type, fx.pchActivationFX, 0)       }, \
    { "DeactivationFX",               TOK_FILENAME(type, fx.pchDeactivationFX, 0)     }, \
    { "AttackFX",                     TOK_FILENAME(type, fx.pchAttackFX, 0)           }, \
    { "HitFX",                        TOK_FILENAME(type, fx.pchHitFX, 0)              }, \
    { "WindUpFX",                     TOK_FILENAME(type, fx.pchWindUpFX, 0)           }, \
    { "BlockFX",                      TOK_FILENAME(type, fx.pchBlockFX, 0)            }, \
    { "DeathFX",                      TOK_FILENAME(type, fx.pchDeathFX, 0)            }, \
    { "InitialAttackFX",              TOK_FILENAME(type, fx.pchInitialAttackFX, 0)    }, \
    { "ContinuingFX",                 TOK_FILENAME(type, fx.pchContinuingFX[0], 0)       }, \
    { "ContinuingFX1",                 TOK_FILENAME(type, fx.pchContinuingFX[0], 0)       }, \
    { "ContinuingFX2",                 TOK_FILENAME(type, fx.pchContinuingFX[1], 0)       }, \
    { "ContinuingFX3",                 TOK_FILENAME(type, fx.pchContinuingFX[2], 0)       }, \
    { "ContinuingFX4",                 TOK_FILENAME(type, fx.pchContinuingFX[3], 0)       }, \
    { "ConditionalFX",                 TOK_FILENAME(type, fx.pchConditionalFX[0], 0)       }, \
    { "ConditionalFX1",             TOK_FILENAME(type, fx.pchConditionalFX[0], 0)       }, \
    { "ConditionalFX2",             TOK_FILENAME(type, fx.pchConditionalFX[1], 0)       }, \
    { "ConditionalFX3",             TOK_FILENAME(type, fx.pchConditionalFX[2], 0)       }, \
    { "ConditionalFX4",             TOK_FILENAME(type, fx.pchConditionalFX[3], 0)       }, \
    { "ModeBits",                   TOK_INTARRAY(type, fx.piModeBits), ParsePowerDefines }, \
    { "FramesBeforeHit",            TOK_INT(type, fx.iFramesBeforeHit, -1)   }, \
    { "SeqBits",                    TOK_REDUNDANTNAME|TOK_INTARRAY(type, fx.piModeBits), ParsePowerDefines }, \
    { "cast_anim",                  TOK_REDUNDANTNAME|TOK_INTARRAY(type, fx.piAttackBits), ParsePowerDefines }, \
    { "hit_anim",                   TOK_REDUNDANTNAME|TOK_INTARRAY(type, fx.piHitBits), ParsePowerDefines }, \
    { "deathanimbits",              TOK_REDUNDANTNAME|TOK_INTARRAY(type, fx.piDeathBits), ParsePowerDefines }, \
    { "AttachedAnim",               TOK_REDUNDANTNAME|TOK_INTARRAY(type, fx.piActivationBits), ParsePowerDefines }, \
    { "AttachedFxName",             TOK_REDUNDANTNAME|TOK_FILENAME(type, fx.pchActivationFX, 0)    }, \
    { "TravellingProjectileEffect", TOK_REDUNDANTNAME|TOK_FILENAME(type, fx.pchAttackFX, 0)        }, \
    { "AttachedToVictimFxName",     TOK_REDUNDANTNAME|TOK_FILENAME(type, fx.pchHitFX, 0)           }, \
    { "TimeBeforePunchHits",        TOK_REDUNDANTNAME|TOK_INT(type, fx.iFramesBeforeHit,  -1)   }, \
    { "TimeBeforeMissileSpawns",    TOK_REDUNDANTNAME|TOK_INT(type, fx.iFramesBeforeHit,  -1)   }, \
    { "DelayedHit",                 TOK_BOOL(type, fx.bDelayedHit, 0), BoolEnum }, \
    { "TogglePower",                TOK_IGNORE,                        0 }, \
    \
    { "AttackFrames",               TOK_INT(type, fx.iFramesAttack, -1)      }, \
    { "NonInterruptFrames",         TOK_REDUNDANTNAME|TOK_INT(type, fx.iFramesAttack, -1)      }, \
    \
    { "InitialFramesBeforeHit",     TOK_INT(type, fx.iInitialFramesBeforeHit, -1)    }, \
    { "InitialAttackFXFrameDelay",    TOK_INT(type, fx.iInitialAttackFXFrameDelay, 0)     }, \
    { "ProjectileSpeed",            TOK_F32(type, fx.fProjectileSpeed, 0)           }, \
    { "InitialFramesBeforeBlock",   TOK_INT(type, fx.iInitialFramesBeforeBlock, 0)  }, \
    { "IgnoreAttackTimeErrors",     TOK_STRING(type, fx.pchIgnoreAttackTimeErrors, 0)  }, \
    { "FramesBeforeBlock",          TOK_INT(type, fx.iFramesBeforeBlock, 0)         }, \
    { "FXImportant",                TOK_BOOL(type, fx.bImportant,    0), BoolEnum }, \
    { "PrimaryTint",                TOK_CONDRGB(type, fx.defaultTint.primary) }, \
    { "SecondaryTint",                TOK_CONDRGB(type, fx.defaultTint.secondary) },

static TokenizerParseInfo ParseColor[] =
{
    { "",            TOK_STRUCTPARAM | TOK_FIXED_ARRAY | TOK_F32_X, 0, 3 },
    { "\n",            TOK_END                                                },
    { "", 0, 0 }
};

static TokenizerParseInfo ParseColorPalette[] =
{
    { "Color",         TOK_STRUCT(ColorPalette, color, ParseColor)            },
    { "Name",        TOK_POOL_STRING|TOK_STRING(ColorPalette, name, 0)    },
    { "End",        TOK_END                                                },
    { "EndPalette",    TOK_END                                                },
    { "", 0, 0 }
};

TokenizerParseInfo ParsePowerCustomFX[] =
{
    { "Token",            TOK_STRUCTPARAM|TOK_POOL_STRING|TOK_STRING(PowerCustomFX, pchToken, 0) },
    { "AltTheme",        TOK_STRINGARRAY(PowerCustomFX, altThemes)                    },
    { "SourceFile",        TOK_CURRENTFILE(PowerCustomFX, fx.pchSourceFile)            },
    { "Category",        TOK_POOL_STRING|TOK_STRING(PowerCustomFX, pchCategory, 0)    },
    { "DisplayName",    TOK_STRING(PowerCustomFX, pchDisplayName, 0)                },
    ParsePowerFX(PowerCustomFX, fx) // included flat
    { "Palette",        TOK_POOL_STRING|TOK_STRING(PowerCustomFX, paletteName, 0)    },
    { "End",            TOK_END                                                        },
    { "", 0, 0 }
};

StaticDefineInt DeathCastableSettingEnum[] =
{
    DEFINE_INT
    // old values
    { "false",                kDeathCastableSetting_AliveOnly        },
    { "kFalse",                kDeathCastableSetting_AliveOnly        },
    { "true",                kDeathCastableSetting_DeadOnly        },
    { "kTrue",                kDeathCastableSetting_DeadOnly        },
    // new values
    { "AliveOnly",            kDeathCastableSetting_AliveOnly        },
    { "kAliveOnly",            kDeathCastableSetting_AliveOnly        },
    { "DeadOnly",            kDeathCastableSetting_DeadOnly        },
    { "kDeadOnly",            kDeathCastableSetting_DeadOnly        },
    { "DeadOrAlive",        kDeathCastableSetting_DeadOrAlive    },
    { "kDeadOrAlive",        kDeathCastableSetting_DeadOrAlive    },
    DEFINE_END
};

extern StaticDefineInt ModTargetEnum[];

TokenizerParseInfo ParseBasePower[] =
{
    { "{",                  TOK_START,    0                                         },
    { "FullName",           TOK_STRUCTPARAM | TOK_STRING(BasePower, pchFullName, 0)},
    { "CRCFullName",        TOK_INT(BasePower, crcFullName, 0) },    // Do NOT add this to .powers files!  Needed here because that's how shared memory works?
    { "SourceFile",         TOK_CURRENTFILE(BasePower, pchSourceFile) },
    { "Name",               TOK_STRING(BasePower, pchName, 0)              },
    { "SourceName",            TOK_STRING(BasePower, pchSourceName, 0)              },
    { "System",             TOK_INT(BasePower, eSystem, kPowerSystem_Powers), PowerSystemEnum },
    { "AutoIssue",          TOK_BOOL(BasePower, bAutoIssue, 0), BoolEnum },
    { "AutoIssueSaveLevel", TOK_BOOL(BasePower, bAutoIssueSaveLevel, 0), BoolEnum },
    { "Free",               TOK_BOOL(BasePower, bFree, 0), BoolEnum },
    { "DisplayName",        TOK_STRING(BasePower, pchDisplayName, 0)       },
    { "DisplayHelp",        TOK_STRING(BasePower, pchDisplayHelp, 0)       },
    { "DisplayShortHelp",   TOK_STRING(BasePower, pchDisplayShortHelp, 0)  },
    { "DisplayCasterHelp",     TOK_REDUNDANTNAME|TOK_STRING(BasePower, pchDisplayHelp, 0)       },
    { "DisplayCasterShortHelp",TOK_REDUNDANTNAME|TOK_STRING(BasePower, pchDisplayShortHelp, 0)  },
    { "DisplayTargetHelp",     TOK_STRING(BasePower, pchDisplayTargetHelp, 0)       },
    { "DisplayTargetShortHelp",TOK_STRING(BasePower, pchDisplayTargetShortHelp, 0)  },
    { "DisplayAttackerAttack", TOK_STRING(BasePower, pchDisplayAttackerAttack, 0)   },
    { "DisplayAttackerAttackFloater", TOK_STRING(BasePower, pchDisplayAttackerAttackFloater, 0)   },
    { "DisplayAttackerHit", TOK_STRING(BasePower, pchDisplayAttackerHit, 0)  },
    { "DisplayVictimHit",   TOK_STRING(BasePower, pchDisplayVictimHit, 0)    },
    { "DisplayConfirm",     TOK_STRING(BasePower, pchDisplayConfirm, 0)      },
    { "FloatRewarded",      TOK_STRING(BasePower, pchDisplayFloatRewarded, 0)},
    { "DisplayPowerDefenseFloat",TOK_STRING(BasePower, pchDisplayDefenseFloat, 0)},
    { "IconName",           TOK_STRING(BasePower, pchIconName, 0)            },
    { "FXName",             TOK_IGNORE,   0                                           },
    { "Type",               TOK_INT(BasePower, eType, kPowerType_Click), PowerTypeEnum },
    { "NumAllowed",         TOK_INT(BasePower, iNumAllowed, 1)},
    { "AttackTypes",        TOK_INTARRAY(BasePower, pAttackTypes), ParsePowerDefines },
    { "BuyRequires",        TOK_STRINGARRAY(BasePower, ppchBuyRequires),     },
    { "ActivateRequires",   TOK_STRINGARRAY(BasePower, ppchActivateRequires),     },
    { "SlotRequires",        TOK_STRINGARRAY(BasePower, ppchSlotRequires)       },
    { "TargetRequires",        TOK_STRINGARRAY(BasePower, ppchTargetRequires)    },
    { "RewardRequires",        TOK_STRINGARRAY(BasePower, ppchRewardRequires)    },
    { "AuctionRequires",    TOK_STRINGARRAY(BasePower, ppchAuctionRequires)    },
    { "RewardFallback",     TOK_STRING(BasePower, pchRewardFallback, 0)            },
    { "Accuracy",           TOK_F32(BasePower, fAccuracy, 0)            },
    { "NearGround",         TOK_BOOL(BasePower, bNearGround, 0), BoolEnum },
    { "TargetNearGround",   TOK_BOOL(BasePower, bTargetNearGround, -1), BoolEnum },
    { "CastableAfterDeath", TOK_INT(BasePower, eDeathCastableSetting, kDeathCastableSetting_AliveOnly), DeathCastableSettingEnum },
    { "CastThroughHold",    TOK_BOOL(BasePower, bCastThroughHold, 0), BoolEnum },
    { "CastThroughSleep",   TOK_BOOL(BasePower, bCastThroughSleep, 0), BoolEnum },
    { "CastThroughStun",    TOK_BOOL(BasePower, bCastThroughStun, )0, BoolEnum },
    { "CastThroughTerrorize",  TOK_BOOL(BasePower, bCastThroughTerrorize,0), BoolEnum },
    { "ToggleIgnoreHold",   TOK_BOOL(BasePower, bToggleIgnoreHold, 0), BoolEnum },
    { "ToggleIgnoreSleep",  TOK_BOOL(BasePower, bToggleIgnoreSleep, 0), BoolEnum },
    { "ToggleIgnoreStun",   TOK_BOOL(BasePower, bToggleIgnoreStun, 0), BoolEnum },
    { "IgnoreLevelBought",  TOK_BOOL(BasePower, bIgnoreLevelBought, 0), BoolEnum },
    { "ShootThroughUntouchable", TOK_BOOL(BasePower, bShootThroughUntouchable, 0), BoolEnum },
    { "InterruptLikeSleep", TOK_BOOL(BasePower, bInterruptLikeSleep, 0), BoolEnum },
    { "AIReport",           TOK_INT(BasePower, eAIReport, kAIReport_Always),      AIReportEnum   },
    { "EffectArea",         TOK_INT(BasePower, eEffectArea, kEffectArea_Character), EffectAreaEnum },
    { "MaxTargetsHit",      TOK_INT(BasePower, iMaxTargetsHit, 0)      },
    { "Radius",             TOK_F32(BasePower, fRadius, 0)              },
    { "Arc",                TOK_F32(BasePower, fArc, 0)                 },
    { "BoxOffset",          TOK_VEC3(BasePower, vecBoxOffset)         },
    { "BoxSize",            TOK_VEC3(BasePower, vecBoxSize)           },
    { "Range",              TOK_F32(BasePower, fRange, 0)               },
    { "RangeSecondary",     TOK_F32(BasePower, fRangeSecondary, 0)      },
    { "TimeToActivate",     TOK_F32(BasePower, fTimeToActivate, 0)      },
    { "RechargeTime",       TOK_F32(BasePower, fRechargeTime, 0)        },
    { "ActivatePeriod",     TOK_F32(BasePower, fActivatePeriod, 0)      },
    { "EnduranceCost",      TOK_F32(BasePower, fEnduranceCost, 0)       },
    { "InsightCost",        TOK_REDUNDANTNAME|TOK_F32(BasePower, fInsightCost, 0)         },
    { "IdeaCost",           TOK_F32(BasePower, fInsightCost, 0)         },
    { "TimeToConfirm",      TOK_INT(BasePower, iTimeToConfirm, 0),      },
    { "SelfConfirm",        TOK_INT(BasePower, iSelfConfirm, 0),        },
    { "ConfirmRequires",    TOK_STRINGARRAY(BasePower, ppchConfirmRequires)    },
    { "DestroyOnLimit",     TOK_BOOL(BasePower, bDestroyOnLimit, 1), BoolEnum },
    { "StackingUsage",      TOK_BOOL(BasePower, bStackingUsage, 0), BoolEnum },
    { "NumCharges",         TOK_INT(BasePower, iNumCharges, 0),         },
    { "MaxNumCharges",      TOK_INT(BasePower, iMaxNumCharges, 0),         },
    { "UsageTime",          TOK_F32(BasePower, fUsageTime, 0),          },
    { "MaxUsageTime",       TOK_F32(BasePower, fMaxUsageTime, 0),          },
    { "Lifetime",           TOK_F32(BasePower, fLifetime, 0),           },
    { "MaxLifetime",        TOK_F32(BasePower, fMaxLifetime, 0),           },
    { "LifetimeInGame",     TOK_F32(BasePower, fLifetimeInGame, 0),     },
    { "MaxLifetimeInGame",  TOK_F32(BasePower, fMaxLifetimeInGame, 0),     },
    { "InterruptTime",      TOK_F32(BasePower, fInterruptTime, 0)       },
    { "TargetVisibility",   TOK_INT(BasePower, eTargetVisibility, kTargetVisibility_LineOfSight), TargetVisibilityEnum },
    { "Target",             TOK_INT(BasePower, eTargetType, kTargetType_None), TargetTypeEnum },
    { "TargetSecondary",    TOK_INT(BasePower, eTargetTypeSecondary, kTargetType_None), TargetTypeEnum },
    { "EntsAutoHit",        TOK_INTARRAY(BasePower, pAutoHit), TargetTypeEnum },
    { "EntsAffected",       TOK_INTARRAY(BasePower, pAffected), TargetTypeEnum },
    { "TargetsThroughVisionPhase",    TOK_BOOL(BasePower, bTargetsThroughVisionPhase, 0), BoolEnum },
    { "BoostsAllowed",      TOK_INTARRAY(BasePower, pBoostsAllowed), ParsePowerDefines },
    { "GroupMembership",    TOK_INTARRAY(BasePower, pGroupMembership), ParsePowerDefines },
    { "ModesRequired",      TOK_INTARRAY(BasePower, pModesRequired), ParsePowerDefines },
    { "ModesDisallowed",    TOK_INTARRAY(BasePower, pModesDisallowed), ParsePowerDefines },
    { "AIGroups",           TOK_STRINGARRAY(BasePower, ppchAIGroups),     },
    { "AttribMod",          TOK_STRUCT(BasePower, ppTemplates, ParseAttribModTemplate) },
    { "IgnoreStrength",     TOK_BOOL(BasePower, bIgnoreStrength, 0), BoolEnum },
    { "IgnoreStr",          TOK_REDUNDANTNAME|TOK_BOOL(BasePower, bIgnoreStrength, 0), BoolEnum },
    { "ShowBuffIcon",       TOK_BOOL(BasePower, bShowBuffIcon, -1),        BoolEnum },
    { "ShowInInventory",    TOK_INT(BasePower, eShowInInventory, kShowPowerSetting_Default),    ShowPowerSettingEnum },
    { "ShowInManage",       TOK_BOOL(BasePower, bShowInManage, 1),        BoolEnum },
    { "ShowInInfo",            TOK_BOOL(BasePower, bShowInInfo, 1),        BoolEnum },
    { "Deletable",            TOK_BOOL(BasePower, bDeletable, 0),            BoolEnum },
    { "Tradeable",            TOK_BOOL(BasePower, bTradeable,    0),            BoolEnum },
    { "MaxBoosts",          TOK_INT(BasePower, iMaxBoosts, 6) },
    { "DoNotSave",          TOK_BOOL(BasePower, bDoNotSave, 0),            BoolEnum },
    { "DoesNotExpire",      TOK_REDUNDANTNAME|TOK_BOOL(BasePower, bBoostIgnoreEffectiveness, 0), BoolEnum },
    { "BoostIgnoreEffectiveness", TOK_BOOL(BasePower, bBoostIgnoreEffectiveness, 0), BoolEnum },
    { "BoostAlwaysCountForSet", TOK_BOOL(BasePower, bBoostAlwaysCountForSet, 0), BoolEnum },
    { "BoostTradeable",        TOK_BOOL(BasePower, bBoostTradeable, 1), BoolEnum },
    { "BoostCombinable",    TOK_BOOL(BasePower, bBoostCombinable, 1),    BoolEnum },
    { "BoostAccountBound",    TOK_BOOL(BasePower, bBoostAccountBound, 0),    BoolEnum },
    { "BoostBoostable",        TOK_BOOL(BasePower, bBoostBoostable, 0),    BoolEnum },
    { "BoostUsePlayerLevel",TOK_BOOL(BasePower, bBoostUsePlayerLevel, 0),    BoolEnum },
    { "BoostCatalystConversion",        TOK_STRING(BasePower, pchBoostCatalystConversion, 0),        },
    { "StoreProduct",        TOK_STRING(BasePower, pchStoreProduct, 0),        },
    { "BoostLicenseLevel",  TOK_INT(BasePower, iBoostInventionLicenseRequiredLevel, 999) },
    { "MinSlotLevel",       TOK_INT(BasePower, iMinSlotLevel, -3) },
    { "MaxSlotLevel",       TOK_INT(BasePower, iMaxSlotLevel, MAX_PLAYER_SECURITY_LEVEL-1) },
    { "MaxBoostLevel",      TOK_INT(BasePower, iMaxBoostLevel, MAX_PLAYER_SECURITY_LEVEL) },        // 1 based for designer sanity
    { "Var",                TOK_STRUCT(BasePower, ppVars, ParsePowerVar) },
    { "ToggleDroppable",    TOK_INT(BasePower, eToggleDroppable, kToggleDroppable_Sometimes), ToggleDroppableEnum },
    { "TogglesDroppable",   TOK_REDUNDANTNAME|TOK_INT(BasePower, eToggleDroppable, kToggleDroppable_Sometimes), ToggleDroppableEnum },
    { "StrengthsDisallowed",TOK_INTARRAY(BasePower, pStrengthsDisallowed), ParsePowerDefines },
    { "ProcMainTargetOnly", TOK_BOOL(BasePower, bUseNonBoostTemplatesOnMainTarget, 0),    BoolEnum },
    { "AnimMainTargetOnly", TOK_BOOL(BasePower, bMainTargetOnly, 0), BoolEnum }, 
    { "HighlightEval",        TOK_STRINGARRAY(BasePower, ppchHighlightEval),    },
    { "HighlightIcon",        TOK_STRING(BasePower, pchHighlightIcon, 0),        },
    { "HighlightRing",        TOK_RGBA(BasePower, rgbaHighlightRing),            },
    { "TravelSuppression",  TOK_F32(BasePower, fTravelSuppression, 0),        },
    { "PreferenceMultiplier", TOK_F32(BasePower, fPreferenceMultiplier, 1),    },
    { "DontSetStance",        TOK_BOOL(BasePower, bDontSetStance, 0),    },
    { "PointValue",             TOK_F32(BasePower, fPointVal, 0),        },
    { "PointMultiplier",     TOK_F32(BasePower, fPointMultiplier, 0),        },
    { "ChainIntoPower",        TOK_STRING(BasePower, pchChainIntoPowerName, NULL)},
    { "InstanceLocked",        TOK_BOOL(BasePower, bInstanceLocked, 0), BoolEnum},
    { "IsEnvironmentHit",    TOK_BOOL(BasePower, bIsEnvironmentHit, 0), BoolEnum},
    { "ShuffleTargets",        TOK_BOOL(BasePower, bShuffleTargetList, 0), BoolEnum},
    { "ForceLevelBought",    TOK_INT(BasePower, iForceLevelBought, -1)},
    { "RefreshesOnActivePlayerChange",    TOK_BOOL(BasePower, bRefreshesOnActivePlayerChange, 0), BoolEnum},
    { "PowerRedirector",    TOK_BOOL(BasePower, bPowerRedirector, 0), BoolEnum},
    { "Cancelable",            TOK_BOOL(BasePower, bCancelable, 0), BoolEnum},
    { "IgnoreToggleMaxDistance",    TOK_BOOL(BasePower, bIgnoreToggleMaxDistance, 0), BoolEnum},
    { "ServerTrayPriority",    TOK_INT(BasePower, iServerTrayPriority, 0) },
    { "AbusiveBuff",        TOK_BOOL(BasePower, bAbusiveBuff, true), BoolEnum },
    { "PositionCenter",        TOK_INT(BasePower, ePositionCenter, kModTarget_Caster), ModTargetEnum },
    { "PositionDistance",    TOK_F32(BasePower, fPositionDistance, 0.0f) },
    { "PositionHeight",        TOK_F32(BasePower, fPositionHeight, 0.0f) },
    { "PositionYaw",        TOK_F32(BasePower, fPositionYaw, 0.0f) },
    { "FaceTarget",            TOK_BOOL(BasePower, bFaceTarget, true), BoolEnum},
    
    // What .pfx file was included for this power. The rest of the data comes from this pfx file, via an Include
    { "VisualFX",            TOK_FILENAME(BasePower, fx.pchSourceFile, 0)        },
    ParsePowerFX(BasePower, fx) // included flat
    { "CustomFX",            TOK_STRUCT(BasePower, customfx, ParsePowerCustomFX)    },
    { "}", TOK_END, 0 },
    { "", 0, 0 }
};

TokenizerParseInfo ParseBasePowerSet[] =
{
    { "{",                    TOK_START,       0 },
    { "SourceFile",            TOK_CURRENTFILE(BasePowerSet, pchSourceFile)       },
    { "FullName",            TOK_STRUCTPARAM|TOK_STRING(BasePowerSet, pchFullName, 0)},
    { "Name",                TOK_STRING(BasePowerSet, pchName, 0)             },
    { "System",                TOK_INT(BasePowerSet, eSystem, kPowerSystem_Powers), PowerSystemEnum },
    { "Shared",                TOK_BOOL(BasePowerSet, bIsShared, 0), BoolEnum },
    { "DisplayName",        TOK_STRING(BasePowerSet, pchDisplayName, 0)      },
    { "DisplayHelp",        TOK_STRING(BasePowerSet, pchDisplayHelp, 0)      },
    { "DisplayShortHelp",    TOK_STRING(BasePowerSet, pchDisplayShortHelp, 0) },
    { "IconName",            TOK_STRING(BasePowerSet, pchIconName, 0)         },
    { "CostumeKeys",        TOK_STRINGARRAY(BasePowerSet, ppchCostumeKeys)   },
    { "CostumeParts",        TOK_STRINGARRAY(BasePowerSet, ppchCostumeParts)   },
    { "SetAccountRequires",    TOK_STRING(BasePowerSet, pchAccountRequires, 0)   },
    { "SetAccountTooltip",    TOK_STRING(BasePowerSet, pchAccountTooltip, 0)      },
    { "SetAccountProduct",    TOK_STRING(BasePowerSet, pchAccountProduct, 0)      },
    { "SetBuyRequires",        TOK_STRINGARRAY(BasePowerSet, ppchSetBuyRequires) },
    { "SetBuyRequiresFailedText",    TOK_STRING(BasePowerSet, pchSetBuyRequiresFailedText, 0)    },
    { "ShowInInventory",    TOK_INT(BasePowerSet, eShowInInventory, kShowPowerSetting_Default), ShowPowerSettingEnum },
    { "ShowInManage",        TOK_BOOL(BasePowerSet, bShowInManage, 1), BoolEnum },
    { "ShowInInfo",            TOK_BOOL(BasePowerSet, bShowInInfo, 1), BoolEnum },
    { "SpecializeAt",        TOK_INT(BasePowerSet, iSpecializeAt, 0) },
    { "SpecializeRequires",    TOK_STRINGARRAY(BasePowerSet, ppSpecializeRequires) },
    { "Powers",                TOK_STRINGARRAY(BasePowerSet, ppPowerNames ) },
    { "",                    TOK_AUTOINTEARRAY(BasePowerSet, ppPowers ) },
    { "Available",            TOK_INTARRAY(BasePowerSet, piAvailable)         },
    { "AIMaxLevel",            TOK_INTARRAY(BasePowerSet, piAIMaxLevel)         },
    { "AIMinRankCon",        TOK_INTARRAY(BasePowerSet, piAIMinRankCon)         },
    { "AIMaxRankCon",        TOK_INTARRAY(BasePowerSet, piAIMaxRankCon)         },
    { "MinDifficulty",        TOK_INTARRAY(BasePowerSet, piMinDifficulty)           },
    { "MaxDifficulty",        TOK_INTARRAY(BasePowerSet, piMaxDifficulty)           },
    { "ForceLevelBought",    TOK_INT(BasePowerSet, iForceLevelBought, -1)},
    { "}",                    TOK_END,         0 },
    { "", 0, 0 }
};

TokenizerParseInfo ParsePowerCategory[] =
{
    { "{",                TOK_START,       0 },
    { "SourceFile",       TOK_CURRENTFILE(PowerCategory, pchSourceFile)  },
    { "Name",             TOK_STRUCTPARAM | TOK_STRING(PowerCategory, pchName, 0)        },
    { "DisplayName",      TOK_STRING(PowerCategory, pchDisplayName, 0) },
    { "DisplayHelp",      TOK_STRING(PowerCategory, pchDisplayHelp, 0)      },
    { "DisplayShortHelp", TOK_STRING(PowerCategory, pchDisplayShortHelp, 0) },
    { "PowerSets",        TOK_STRINGARRAY(PowerCategory, ppPowerSetNames ) },
    { "",                  TOK_AUTOINTEARRAY(PowerCategory, ppPowerSets ) },
    { "Available",        TOK_IGNORE,      0 },
    { "}",                TOK_END,         0 },
    { "", 0, 0 }
};

TokenizerParseInfo ParsePowerCatDictionary[] =
{
    { "PowerCategory",   TOK_STRUCT(PowerDictionary, ppPowerCategories, ParsePowerCategory)},
    { "", 0, 0 }
};

TokenizerParseInfo ParsePowerSetDictionary[] =
{
    { "PowerSet",        TOK_STRUCT(PowerDictionary, ppPowerSets, ParseBasePowerSet)},
    { "", 0, 0 }
};

TokenizerParseInfo ParsePowerDictionary[] =
{
    { "Power",           TOK_STRUCT(PowerDictionary, ppPowers, ParseBasePower)},
    { "", 0, 0 }
};

TokenizerParseInfo ParseEntirePowerDictionary[] =
{
    { "PowerCategory",   TOK_STRUCT(PowerDictionary, ppPowerCategories, ParsePowerCategory)},
    { "PowerSet",        TOK_STRUCT(PowerDictionary, ppPowerSets, ParseBasePowerSet)},
    { "Power",           TOK_STRUCT(PowerDictionary, ppPowers, ParseBasePower)},
    { "", 0, 0 }
};

/***************************************************************************/
/***************************************************************************/
/***************************************************************************/

TokenizerParseInfo ParseDimReturn[] =
{
    { "Start",    TOK_STRUCTPARAM|TOK_F32(DimReturn, fStart, 0)     },
    { "Handicap", TOK_STRUCTPARAM|TOK_F32(DimReturn, fHandicap, 0)  },
    { "Basis",    TOK_STRUCTPARAM|TOK_F32(DimReturn, fBasis, 0)     },
    { "\n",       TOK_END,    0 },
    { 0 }
};

TokenizerParseInfo ParseAttribDimReturnSet[] =
{
    { "{",        TOK_START,      0 },
    { "Default",  TOK_INT(AttribDimReturnSet, bDefault, 0)  },
    { "Attrib",   TOK_AUTOINTEARRAY(AttribDimReturnSet, offAttribs), ParsePowerDefines  },
    { "Return",   TOK_STRUCT(AttribDimReturnSet, pReturns, ParseDimReturn) },
    { "}",        TOK_END,        0 },
    { 0 }
};


TokenizerParseInfo ParseDimReturnSet[] =
{
    { "{",               TOK_START,      0 },
    { "Default",         TOK_INT(DimReturnSet, bDefault, 0)  },
    { "Boost",           TOK_INTARRAY(DimReturnSet, piBoosts), ParsePowerDefines  },
    { "AttribReturnSet", TOK_STRUCT(DimReturnSet, ppReturns, ParseAttribDimReturnSet) },
    { "}",               TOK_END,        0 },
    { 0 }
};

TokenizerParseInfo ParseDimReturnList[] =
{
    { "ReturnSet",        TOK_STRUCT(DimReturnList, ppSets, ParseDimReturnSet) },
    { "", 0, 0 }
};

// The next few functions were copied over from load_def.c

extern StaticDefineInt DurationEnum[];
extern StaticDefineInt ModTypeEnum[];
extern StaticDefineInt CasterStackTypeEnum[];
extern StaticDefineInt StackTypeEnum[];
extern StaticDefineInt ModBoolEnum[];
extern StaticDefineInt PowerEventEnum[];
extern TokenizerParseInfo ParseSuppressPair[];

static TokenizerParseInfo ParseDestroyUnusedAttribMods[] =
{
    { "AttribMod",             TOK_STRUCT(BasePower, ppTemplates, ParseAttribModTemplate) },
    { 0 }
};

static TokenizerParseInfo ParseDestroyPartialAttribMod[] =
{
    // Used by enhancement help UI
    // { "Name",                TOK_STRING(AttribModTemplate, pchName, 0)                 },
    { "DisplayAttackerHit",  TOK_STRING(AttribModTemplate, pchDisplayAttackerHit, 0)   },
    { "DisplayVictimHit",    TOK_STRING(AttribModTemplate, pchDisplayVictimHit, 0)     },
    { "DisplayFloat",        TOK_STRING(AttribModTemplate, pchDisplayFloat, 0)         },
    { "DisplayAttribDefenseFloat", TOK_STRING(AttribModTemplate, pchDisplayDefenseFloat, 0)         },

    { "ShowFloaters",        TOK_BOOL(AttribModTemplate, bShowFloaters, 1), ModBoolEnum},

    // Used by diminishing returns UI
    // { "Attrib",              TOK_INT(AttribModTemplate, offAttrib),              0, ParsePowerDefines },
    // { "Aspect",              TOK_INT(AttribModTemplate, offAspect),              0, AspectEnum   },

    { "Target",              TOK_INT(AttribModTemplate, eTarget, kModTarget_Affected), ModTargetEnum },

    // Used by diminishing returns UI
    // { "Table" ,              TOK_STRING, offsetof(AttribModTemplate, pchTable)                },
    // { "Scale",               TOK_F32,    offsetof(AttribModTemplate, fScale)                  },

    { "Type",                    TOK_INT(AttribModTemplate, eType, kModType_Magnitude), ModTypeEnum },
    { "Delay",                    TOK_F32(AttribModTemplate, fDelay, 0)             },
    { "Period",                    TOK_F32(AttribModTemplate, fPeriod, 0)            },
    { "Chance",                    TOK_F32(AttribModTemplate, fChance, 0)            },
    { "CancelOnMiss",            TOK_BOOL(AttribModTemplate, bCancelOnMiss, 0), ModBoolEnum },
    { "CancelEvents",            TOK_INTARRAY(AttribModTemplate, piCancelEvents), PowerEventEnum },
    { "NearGround",                TOK_BOOL(AttribModTemplate, bNearGround, 0), ModBoolEnum },
    { "AllowStrength",            TOK_BOOL(AttribModTemplate, bAllowStrength, 1), ModBoolEnum },
    { "AllowResistance",        TOK_BOOL(AttribModTemplate, bAllowResistance, 1), ModBoolEnum },
    { "UseMagnitudeResistance",    TOK_BOOL(AttribModTemplate, bUseMagnitudeResistance,  -1), ModBoolEnum },
    { "UseDurationResistance",    TOK_BOOL(AttribModTemplate, bUseDurationResistance,   -1), ModBoolEnum },
    { "AllowCombatMods",        TOK_BOOL(AttribModTemplate, bAllowCombatMods,  1), ModBoolEnum },
    { "UseMagnitudeCombatMods",    TOK_BOOL(AttribModTemplate, bUseMagnitudeCombatMods,  -1), ModBoolEnum },
    { "UseDurationCombatMods",    TOK_BOOL(AttribModTemplate, bUseDurationCombatMods,   -1), ModBoolEnum },
    { "Requires",                TOK_STRINGARRAY(AttribModTemplate, ppchApplicationRequires)            },
    { "PrimaryStringList",        TOK_STRINGARRAY(AttribModTemplate, ppchPrimaryStringList)       },
    { "SecondaryStringList",    TOK_STRINGARRAY(AttribModTemplate, ppchSecondaryStringList)       },
    { "CasterStackType",        TOK_INT(AttribModTemplate, eCasterStack, kCasterStackType_Individual), CasterStackTypeEnum },
    { "StackType",                TOK_INT(AttribModTemplate, eStack, kStackType_Replace), StackTypeEnum },
    { "StackLimit",                TOK_INT(AttribModTemplate, iStackLimit, 2)        },
    { "StackKey",                TOK_INT(AttribModTemplate, iStackKey, -1), ParsePowerDefines        },
    { "Duration",                TOK_F32(AttribModTemplate, fDuration, -1), DurationEnum  },
    { "DurationExpr",            TOK_STRINGARRAY(AttribModTemplate, ppchDuration)       },
    { "Magnitude",                TOK_F32(AttribModTemplate, fMagnitude, 0), ParsePowerDefines },
    { "MagnitudeExpr",            TOK_STRINGARRAY(AttribModTemplate, ppchMagnitude)  },
    { "RadiusInner",            TOK_F32(AttribModTemplate, fRadiusInner, -1) },
    { "RadiusOuter",            TOK_F32(AttribModTemplate, fRadiusOuter, -1) },

    { "Suppress",            TOK_STRUCT(AttribModTemplate, ppSuppress, ParseSuppressPair) },
    { "IgnoreSuppressErrors",TOK_STRING(AttribModTemplate, pchIgnoreSuppressErrors, 0)},

    { "ContinuingBits",      TOK_INTARRAY(AttribModTemplate, piContinuingBits), ParsePowerDefines },
    { "ContinuingFX",        TOK_FILENAME(AttribModTemplate, pchContinuingFX, 0)    },
    { "ConditionalBits",     TOK_INTARRAY(AttribModTemplate, piConditionalBits), ParsePowerDefines },
    { "ConditionalFX",       TOK_FILENAME(AttribModTemplate, pchConditionalFX, 0)   },

    { "CostumeName",         TOK_FILENAME(AttribModTemplate, pchCostumeName, 0)     },

    { "Reward",              TOK_STRING(AttribModTemplate, pchReward, 0)               },
    { "Params",                 TOK_STRING(AttribModTemplate, pchParams, 0)                },

    { "EntityDef",           TOK_STRING(AttribModTemplate, pchEntityDef, 0)            },
    { "PriorityListDefense", TOK_STRING(AttribModTemplate, pchPriorityListDefense, 0)  },
    { "PriorityListOffense", TOK_STRING(AttribModTemplate, pchPriorityListOffense, 0)  },
    { "PriorityListPassive", TOK_STRING(AttribModTemplate, pchPriorityListPassive, 0)  },
    { "VanishEntOnTimeout",  TOK_BOOL(AttribModTemplate, bVanishEntOnTimeout, 0), ModBoolEnum},
    { "MatchExactPower",     TOK_BOOL(AttribModTemplate, bMatchExactPower, 0), ModBoolEnum},
    { "KeepThroughDeath", TOK_BOOL(AttribModTemplate, bKeepThroughDeath, 0), ModBoolEnum},
    { 0 }
};


/**********************************************************************func*
* load_PreprocPowerDictionary
*
*/
bool load_PreprocPowerDictionary(ParseTable* pti, void* structptr)
{
    PowerDictionary * pdict = (PowerDictionary *)structptr;
    int i;
    int iSizePows = eaSize(&pdict->ppPowers);
    int *crcFullNameList;

    powerdict_CreateDictHashes(pdict, false);

    combateval_StoreToHitInfo(0.0f, 0.0f, false, false);
    combateval_StoreAttribCalcInfo(0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    combateval_StoreCustomFXToken(NULL);
    combateval_StoreTargetsHit(0);
    combateval_StoreChance(0.0f, 0.0f);

    for (i = 0; i < eaSize(&pdict->ppPowerSets); i++)
    {
        // must be called after powerdicct_CreateDictHashes()!
        powerset_FixupForBin(pdict->ppPowerSets[i]);
    }

    eaiCreate(&crcFullNameList);
    for (i = 0; i < iSizePows; i++)
    {
        power_FixupForBin((BasePower*)pdict->ppPowers[i], &crcFullNameList);
    }
    eaiDestroy(&crcFullNameList);

    powerdict_DuplicateMissingPowers(pdict);
    powerdict_FixPowerSetErrors(pdict);

    return true;
}

static s_ExtraLoadFlags = 0;

/**********************************************************************func*
* load_PreprocPowerSetDictionary
*
*/
bool load_PreprocPowerSetDictionary(ParseTable* pti, void* structptr)
{
    PowerDictionary * pdict = (PowerDictionary *)structptr;
#ifndef TEST_CLIENT    
    // We only care about this when we actually parse power sets
    if (!msGetHideTranslationErrors())
    {
        int iSizeCat = eaSize(&pdict->ppPowerCategories);
        int iSizeSets = eaSize(&pdict->ppPowerSets);
        int icat, iset;
        for(icat=0; icat<iSizeCat; icat++)
        {
            const PowerCategory *pcat = pdict->ppPowerCategories[icat];
            CHECK_TRANSLATION_CAT(pcat->pchDisplayHelp);
            CHECK_TRANSLATION_CAT(pcat->pchDisplayName);
            CHECK_TRANSLATION_CAT(pcat->pchDisplayShortHelp);
        }
        
        for(iset=0; iset<iSizeSets; iset++)
        {
            const BasePowerSet *pset = pdict->ppPowerSets[iset];

            if(pset->pchAccountRequires && !pset->pchAccountProduct)
                ErrorFilenamef(pset->pchSourceFile, "Missing SetAccountProduct for powerset %s.\nThis should be a valid product code.", pset->pchFullName);
            
            CHECK_TRANSLATION_SET(pset->pchDisplayHelp);
            CHECK_TRANSLATION_SET(pset->pchDisplayName);
            CHECK_TRANSLATION_SET(pset->pchDisplayShortHelp);
        }
    }

#endif

    // If we rebinned the powersets, we need to rebin powers too, to pick up new duplicates
    s_ExtraLoadFlags = PARSER_FORCEREBUILD;

    return 1;
}


/**********************************************************************func*
* powerDefReload
*
* Reload the powers file pointed to.
*
*/
static void powerDefReload(const char* relpath, int when)
{
    PowerDictionary *pdict = cpp_const_cast(PowerDictionary*)(&g_PowerDictionary);
    fileWaitForExclusiveAccess(relpath);
    errorLogFileIsBeingReloaded(relpath);
    if (ParserReloadFile(relpath, ParsePowerDictionary, sizeof(PowerDictionary), pdict, NULL, NULL))
    {
        int i;
        int *crcFullNameList;

        combateval_StoreToHitInfo(0.0f, 0.0f, false, false);
        combateval_StoreAttribCalcInfo(0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
        combateval_StoreCustomFXToken(NULL);
        combateval_StoreTargetsHit(0);
        combateval_StoreChance(0.0f, 0.0f);

        for (i = 0; i < eaSize(&pdict->ppPowerSets); i++)
        {
            powerset_FixupForBin(pdict->ppPowerSets[i]);
        }

        eaiCreate(&crcFullNameList);
        for (i = 0; i < eaSize(&pdict->ppPowers); i++)
        {
            BasePower *ppow = (BasePower*)pdict->ppPowers[i];            
            if (ppow->pchSourceName && ppow->pchSourceName[0])
            {
                if (strcmp(ppow->pchSourceName,ppow->pchFullName) != 0 &&
                    stricmp(ppow->pchSourceFile,relpath) == 0)
                {
                    // This is a duplicated power from the reloaded file. We need to redup it
                    eaRemoveConst(&pdict->ppPowers,i);
                    i--;
                }

                //This doesn't need to be reprocessed
                continue;
            }
            power_FixupForBin(ppow, &crcFullNameList);
            power_ValidateEntCreate(ppow);
        }
        eaiDestroy(&crcFullNameList);

        powerdict_DestroyDictHashes(pdict);
        powerdict_CreateDictHashes(pdict, false);
        powerdict_DuplicateMissingPowers(pdict);
        powerdict_FixPowerSetErrors(pdict);

        powerdict_FinalizeDict(pdict);

        powerdict_SetForwardPointers(pdict);
        powerdict_SetBackPointers(pdict);

        for (i = 0; i < eaSize(&pdict->ppPowers); i++)
        {
            BasePower *ppow = (BasePower*)pdict->ppPowers[i];            
            if (ppow->pchSourceFile && stricmp(ppow->pchSourceFile,relpath) == 0)
            {
                powerdict_FinalizePower(ppow,pdict);
            }
        }
        
        boostset_DestroyDictHashes((BoostSetDictionary*)&g_BoostSetDict);
        boostset_Finalize((BoostSetDictionary*)&g_BoostSetDict, false, NULL, true);
    }
    else
    {
        Errorf("Error reloading villain: %s\n", relpath);
    }
}


/**********************************************************************func*
* powerAnimReload
*
* Given a pfx file name, reload all powers that include that pfx filename
*
*/
static void powerAnimReload(const char* relpath, int when)
{
    // An animation file changed, we need to figure out which power files use that animation
    PowerDictionary *pdict = (PowerDictionary*)&g_PowerDictionary;
    int i;
    int size = eaSize(&pdict->ppPowers);
    char **reloadFiles;
    eaCreate(&reloadFiles);
    fileWaitForExclusiveAccess(relpath);
    errorLogFileIsBeingReloaded(relpath);

    for (i = 0; i < size; i++)
    {
        BasePower *ppow = (BasePower*)pdict->ppPowers[i];
        int reload = ppow->fx.pchSourceFile && stricmp(ppow->fx.pchSourceFile,relpath) == 0; //normalize the path somehow?
        if(!reload)
        {
            int j;
            for(j = eaSize(&ppow->customfx)-1; j >= 0 && !reload; j--)
                reload = ppow->customfx[j]->fx.pchSourceFile && stricmp(ppow->customfx[j]->fx.pchSourceFile,relpath) == 0;
        }
        if(reload)
        {
            int j,found = 0;
            for (j = 0; j < eaSize(&reloadFiles); j++)
            {
                if (stricmp(ppow->pchSourceFile,reloadFiles[j]) == 0)
                    found = 1;
            }
            if (found)
                continue; //already in the array
            eaPush(&reloadFiles,strdup(ppow->pchSourceFile));
        }
    }
    size = eaSize(&reloadFiles);
    for (i = 0; i < size; i++)
    {
        powerDefReload(reloadFiles[i],when);
    }
    eaDestroyEx(&reloadFiles,NULL);

}


static void load_PowerDictionary_MoveToShared(PowerDictionary* ppow, SharedMemoryHandle *phandle, int flags)
{
    int i;

    if (eaSize(&(ppow->ppPowerCategories[0])->ppPowerSets) == 0)
    {
        // If we loaded from bins, we need to set the pointers now
        powerdict_CreateDictHashes(ppow, false);
        powerdict_SetForwardPointers(ppow);
    }

    powerdict_FinalizeDict(ppow);

    for (i = 0; i < eaSize(&ppow->ppPowers); i++)
    {
        BasePower *p = (BasePower*)ppow->ppPowers[i];            
        powerdict_FinalizePower(p,ppow);
    }

    powerdict_DestroyDictHashes(ppow);
            
    if (phandle)
    {
        ParserMoveToShared(phandle, flags, ParseEntirePowerDictionary, ppow, sizeof(*ppow), NULL);
        // switch to a manual lock
        sharedMemoryCommit(phandle);
        sharedHeapMemoryManagerLock();
    }

    powerdict_CreateDictHashes(ppow, phandle!=0);
    powerdict_SetForwardPointers(ppow);
    powerdict_SetBackPointers(ppow);

    if (phandle)
    {
        ParserCopyToShared(phandle, ppow, sizeof(*ppow));
        sharedHeapMemoryManagerUnlock();
        sharedMemoryUnlock(phandle);
    }
}

/**********************************************************************func*
* load_PowerDictionary
*
*/
int artTestPowerLoaded = 0;
BasePower artTestPower;

void load_PowerDictionary(SHARED_MEMORY_PARAM PowerDictionary *ppow, char *pchFilename, bool bNewAttribs, void (*callback)(SharedMemoryHandle *phandle, bool bNewAttribs))
{
#if SERVER 
    int flags = PARSER_SERVERONLY | (bNewAttribs?PARSER_FORCEREBUILD:0);
#else
    int flags = (bNewAttribs?PARSER_FORCEREBUILD:0);
#endif
    SharedMemoryHandle *phandle = NULL;    

    //Code to allow artists their own pfx overrides for testing
    artTestPowerLoaded = 0;
    ZeroStruct( &artTestPower );
    if( isDevelopmentMode() && fileExists("c:\\test.pfx") )
    {
        if( ParserLoadFiles( NULL, "c:\\test.pfx", NULL, 0, ParseBasePower, &artTestPower, NULL, NULL, NULL ) )
            artTestPowerLoaded = 1;
    }
    //End pfx overrides

    if (isDevelopmentMode() && artTestPowerLoaded)
    {
        // Hack for art test powers
        flags |= PARSER_DONTFREE;
    }

    if (bNewAttribs || !ParserLoadFromShared(MakeSharedMemoryName("SM_POWERS"),flags,ParseEntirePowerDictionary,ppow,sizeof(*ppow),NULL,&phandle))
    {    
        s_ExtraLoadFlags = 0;
        // We didn't succesfully load all from shared memory, so parse them
        ParserLoadFiles(pchFilename,".categories","powercats.bin",flags,
            ParsePowerCatDictionary,(void*)ppow,NULL,NULL,NULL);
        ParserLoadFiles(pchFilename,".powersets","powersets.bin",flags,
            ParsePowerSetDictionary,(void*)ppow,NULL,NULL,load_PreprocPowerSetDictionary);
        ParserLoadFiles(pchFilename,".powers","powers.bin",flags|s_ExtraLoadFlags,
            ParsePowerDictionary,(void*)ppow,NULL,NULL,load_PreprocPowerDictionary);
    
        load_PowerDictionary_MoveToShared((PowerDictionary*)ppow, phandle, flags);
    }

    powerdict_FindSharedPowersets(ppow);

    powerdict_FinalizeUnshared(ppow);

    TODO(); // Make sure  powerdict_CreateHashes() and powerdict_CreateDictHashes() are equivalent.
    // Old hash tables were created on the individual categories and sets, now the hash tables are global,
    // e.g., one global hash table for all power names instead. powerdict_CreateDictHashes() will throw
    // an error if it comes across duplicate names for the global tables. Current data is not throwing any
    // errors so as long as that remains the case (we should let design know) this should be fine.
    //powerdict_CreateHashes(ppow);

    power_LoadReplacementHash();

    // Handle callbacks that need to modify the power dictionary's shared memory
    if (callback)
        callback(phandle, bNewAttribs);

    power_verifyValidMMPowers();
    if (isDevelopmentMode())
    {
        char fullpath[512];
        sprintf(fullpath,"%s*.powers",pchFilename);
        FolderCacheSetCallback(FOLDER_CACHE_CALLBACK_UPDATE, fullpath, powerDefReload);
        FolderCacheSetCallback(FOLDER_CACHE_CALLBACK_UPDATE, "menu/Powers/AnimFX/*.pfx", powerAnimReload);
    }
}

/**********************************************************************func*
* ReloadPowers
*
*/
void ReloadPowers(PowerDictionary *pdictDest, const PowerDictionary *pdictSrc)
{
    int icat;
    int iSizeCats = eaSize(&pdictSrc->ppPowerCategories);

    for(icat=0; icat<iSizeCats; icat++)
    {
        int iset;
        const PowerCategory *pcatNew = pdictSrc->ppPowerCategories[icat];
        PowerCategory *pcatOld = (PowerCategory *)powerdict_GetCategoryByName(pdictDest, pcatNew->pchName);
        int iSizeSets = eaSize(&pcatNew->ppPowerSets);

        if(pcatOld==NULL)
        {
            Errorf("Can't find old category named %s.", pcatNew->pchName);
            continue;
        }

        //free(pcatOld->pchName);
        //free(pcatOld->pchDisplayName);
        //free(pcatOld->pchDisplayHelp);
        //free(pcatOld->pchDisplayShortHelp);

        pcatOld->pchName             = pcatNew->pchName;
        pcatOld->pchDisplayName      = pcatNew->pchDisplayName;
        pcatOld->pchDisplayHelp      = pcatNew->pchDisplayHelp;
        pcatOld->pchDisplayShortHelp = pcatNew->pchDisplayShortHelp;

        for(iset=0; iset<iSizeSets; iset++)
        {
            int ipow;
            const BasePowerSet *psetNew = pcatNew->ppPowerSets[iset];
            BasePowerSet *psetOld = (BasePowerSet*)powerdict_GetBasePowerSetByName(pdictDest, pcatOld->pchName, psetNew->pchName);
            int iSizePows = eaSize(&psetNew->ppPowers);

            if(psetOld==NULL)
            {
                Errorf("Can't find old power set named %s.", psetNew->pchName);
                continue;
            }

            //free(psetOld->pchName);
            //free(psetOld->pchDisplayName);
            //free(psetOld->pchDisplayHelp);
            //free(psetOld->pchDisplayShortHelp);
            //free(psetOld->pchIconName);
            //eaiDestroy(&psetOld->piAvailable);

            psetOld->pchName             = psetNew->pchName;
            psetOld->pchDisplayName      = psetNew->pchDisplayName;
            psetOld->pchDisplayHelp      = psetNew->pchDisplayHelp;
            psetOld->pchDisplayShortHelp = psetNew->pchDisplayShortHelp;
            psetOld->pchIconName         = psetNew->pchIconName;
            psetOld->piAvailable         = psetNew->piAvailable;

            for(ipow=0; ipow<iSizePows; ipow++)
            {
                int imod;
                const BasePower *ppowNew = psetNew->ppPowers[ipow];
                BasePower *ppowOld = (BasePower*)baseset_GetBasePowerByName(psetOld, ppowNew->pchName);
                int iSizeTemps = eaSize(&ppowNew->ppTemplates);

                if(ppowOld==NULL)
                {
                    Errorf("Can't find old power named %s.", ppowNew->pchName);
                    continue;
                }

                // Not updated
                // BasePowerSet *psetParent


                // Doing this leaks the old FX. But, since I'm leaking
                // everything else right now anyway, who cares?
                ppowOld->fx              = ppowNew->fx;
                //eaDestroy(&ppowOld->customfx);
                ppowOld->customfx        = ppowNew->customfx;

                ppowOld->eType           = ppowNew->eType;
                ppowOld->fAccuracy       = ppowNew->fAccuracy;
                ppowOld->bNearGround     = ppowNew->bNearGround;
                ppowOld->eEffectArea     = ppowNew->eEffectArea;
                ppowOld->fRadius         = ppowNew->fRadius;
                ppowOld->fArc            = ppowNew->fArc;
                ppowOld->fRange          = ppowNew->fRange;
                ppowOld->fTimeToActivate = ppowNew->fTimeToActivate;
                ppowOld->fRechargeTime   = ppowNew->fRechargeTime;
                ppowOld->fInterruptTime  = ppowNew->fInterruptTime;
                ppowOld->fActivatePeriod = ppowNew->fActivatePeriod;
                ppowOld->fEnduranceCost  = ppowNew->fEnduranceCost;

                //free(ppowOld->pchName);
                //free(ppowOld->pchDisplayName);
                //free(ppowOld->pchDisplayHelp);
                //free(ppowOld->pchDisplayShortHelp);
                //free(ppowOld->pchDisplayAttackerHit);
                //free(ppowOld->pchDisplayVictimHit);
                //free(ppowOld->pchIconName);
                //eaiDestroy(&(int *)ppowOld->pAffected);
                //eaiDestroy(&(int *)ppowOld->pAutoHit);
                //eaiDestroy(&(int *)ppowOld->pBoostsAllowed);
                //eaiDestroy(&(int *)ppowOld->pStrengthsDisallowed);

                ppowOld->pchName               = ppowNew->pchName;
                ppowOld->pchDisplayName        = ppowNew->pchDisplayName;
                ppowOld->pchDisplayHelp        = ppowNew->pchDisplayHelp;
                ppowOld->pchDisplayShortHelp   = ppowNew->pchDisplayShortHelp;
                ppowOld->pchDisplayAttackerHit = ppowNew->pchDisplayAttackerHit;
                ppowOld->pchDisplayVictimHit   = ppowNew->pchDisplayVictimHit;
                ppowOld->pchIconName           = ppowNew->pchIconName;
                ppowOld->pAffected             = ppowNew->pAffected;
                ppowOld->pAutoHit              = ppowNew->pAutoHit;
                ppowOld->pBoostsAllowed        = ppowNew->pBoostsAllowed;
                ppowOld->pStrengthsDisallowed  = ppowNew->pStrengthsDisallowed;

                eaDestroyConst(&ppowOld->ppBoostSets);

                if(eaSize(&ppowNew->ppTemplates)!=eaSize(&ppowOld->ppTemplates))
                {
                    Errorf("Unmatched number of attib mods for power %s.", ppowNew->pchName);
                    continue;
                }

                for(imod=0; imod<iSizeTemps; imod++)
                {
                    const AttribModTemplate *pmodNew = ppowNew->ppTemplates[imod];
                    AttribModTemplate *pmodOld = cpp_const_cast(AttribModTemplate*)(ppowOld->ppTemplates[imod]);
                    const BasePower *ppowBaseOld = pmodOld->ppowBase;

                    if(!pmodNew->pchName || !pmodOld->pchName)
                    {
                        Errorf("Attrib mod name is empty!");
                        continue;
                    }

                    if(stricmp(pmodNew->pchName, pmodOld->pchName)!=0)
                    {
                        Errorf("Unmatched attib mod name %s.", pmodNew->pchName);
                        continue;
                    }

                    //free(pmodOld->pchName);
                    //free(pmodOld->pchDisplayAttackerHit);
                    //free(pmodOld->pchDisplayVictimHit);
                    //free(pmodOld->pchTable);

                    *pmodOld = *pmodNew;
                    pmodOld->ppowBase = ppowBaseOld;
                }
            }
        }
    }
}





/* End of File */
