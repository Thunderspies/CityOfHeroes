    /***************************************************************************
 *     Copyright (c) 2003-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include <utilitieslib/assert/assert.h>
#include <stddef.h>

#include <utilitieslib/components/earray.h>
#include <utilitieslib/utils/textparser.h> // ParserAllocStruct

#include "entity/attrib_names.h"
#include "character_attribs.h"
#include "entity/character_base.h"
#include "entity/powers.h"
#include <utilitieslib/utils/mathutil.h>
#include "attribmod.h"

#if CLIENT
#include "cmdparse/cmdgame.h"
#elif SERVER
#include "cmdparse/cmdserver.h"
#endif


SHARED_MEMORY DimReturnList g_DimReturnList;

CharacterAttributes g_attrModBase =
{
    // START ModBase
    { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, }, // fDamageType
    0.0, // fHitPoints
    0.0, // fAbsorb
    0.0, // fEndurance
    0.0, // fInsight
    0.0, // fRage
    0.0, // fToHit
    { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, }, // fDefenseType
    0.0, // fDefense
    1.0, // fSpeedRunning
    1.0, // fSpeedFlying
    1.0, // fSpeedSwimming
    1.0, // fSpeedJumping
    1.0, // fJumpHeight
    1.0, // fMovementControl
    1.0, // fMovementFriction
    0.0, // fStealth
    0.0, // fStealthRadius
    0.0, // fStealthRadiusPlayer
    1.0, // fPerceptionRadius
    1.0, // fRegeneration
    1.0, // fRecovery
    1.0, // fInsightRecovery
    0.0, // fThreatLevel
    1.0, // fTaunt
    1.0, // fPlacate
    0.0, // fConfused
    0.0, // fAfraid
    0.0, // fTerrorized
    0.0, // fHeld
    0.0, // fImmobilized
    0.0, // fStunned
    0.0, // fSleep
    0.0, // fFly
    0.0, // fJumppack
    0.0, // fTeleport
    0.0, // fUntouchable
    0.0, // fIntangible
    0.0, // fOnlyAffectsSelf
    0.0, // fExperienceGain
    0.0, // fInfluenceGain
    0.0, // fPrestigeGain
    0.0, // fNullBool
    0.0, // fKnockup
    0.0, // fKnockback
    0.0, // fRepel
    1.0, // fAccuracy
    1.0, // fRadius
    1.0, // fArc
    1.0, // fRange
    1.0, // fTimeToActivate
    1.0, // fRechargeTime
    1.0, // fInterruptTime
    1.0, // fEnduranceDiscount
    1.0, // fInsightDiscount
    0.0, // fMeter
    { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, }, // fDefenseType
    1.0, // fElusivityBase
    // END ModBase
};

CharacterAttributes g_attrZeroes =
{
    // START Values(0.0)
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, }, // fDamageType
    0, // fHitPoints
    0, // fAbsorb
    0, // fEndurance
    0, // fInsight
    0, // fRage
    0, // fToHit
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, }, // fDefenseType
    0, // fDefense
    0, // fSpeedRunning
    0, // fSpeedFlying
    0, // fSpeedSwimming
    0, // fSpeedJumping
    0, // fJumpHeight
    0, // fMovementControl
    0, // fMovementFriction
    0, // fStealth
    0, // fStealthRadius
    0, // fStealthRadiusPlayer
    0, // fPerceptionRadius
    0, // fRegeneration
    0, // fRecovery
    0, // fInsightRecovery
    0, // fThreatLevel
    0, // fTaunt
    0, // fPlacate
    0, // fConfused
    0, // fAfraid
    0, // fTerrorized
    0, // fHeld
    0, // fImmobilized
    0, // fStunned
    0, // fSleep
    0, // fFly
    0, // fJumppack
    0, // fTeleport
    0, // fUntouchable
    0, // fIntangible
    0, // fOnlyAffectsSelf
    0, // fExperienceGain
    0, // fInfluenceGain
    0, // fPrestigeGain
    0, // fNullBool
    0, // fKnockup
    0, // fKnockback
    0, // fRepel
    0, // fAccuracy
    0, // fRadius
    0, // fArc
    0, // fRange
    0, // fTimeToActivate
    0, // fRechargeTime
    0, // fInterruptTime
    0, // fEnduranceDiscount
    0, // fInsightDiscount
    0, // fMeter
    { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, }, // fElusivity
    0, // fElusivityBase
    // END Values(0.0)
};

/**********************************************************************func*
* DiminishValue
*
*/
#define DIMINISH_INNER_SCALE    0.33f
#define DIMINISH_OUTER_SCALE    0.8f
float attribDiminishValue(float value, float inner_scale, float outer_scale)
{
#if SERVER
    if (server_state.enablePowerDiminishing)
    {
    
#elif CLIENT 
    if (game_state.enablePowerDiminishing)
    {

#endif
        if (value == 0.0f)
            return value;

        if (value > 0)
        {
            return value - ((atan(value * inner_scale) * outer_scale * value) / HALFPI );
        } else {
            return value + ((atan(-value * inner_scale) * outer_scale * -value) / HALFPI );
        }
    }

    return value;
}

/**********************************************************************func*
 * TransposeArray
 *
 */
static void TransposeArray(float *pvals, CharacterAttributes *pattr, int iOffset, int iNum, float fDefault)
{
    int i;
    float fLast = fDefault;
    int iCount = eafSize(&pvals);

    assert(pattr!=NULL);
    assert(iOffset>=0);
    assert(pattr!=NULL||iCount==0);

    if(pattr==NULL)
    {
        return;
    }

    if(iNum < iCount)
    {
        iCount=iNum;
    }

    // Copy all the values we have
    for(i=0; i<iCount; i++)
    {
        fLast = *(float *)(((char *)(pattr+i))+iOffset) = pvals[i];
    }

    // Copy the last one out to the end in case the data was truncated
    while(i<iNum)
    {
        *(float *)(((char *)(pattr+i))+iOffset) = fLast;
        i++;
    }
}

#define TRANSPOSE_ARRAY(attrib) \
    TransposeArray(pptab[0]->p##attrib, pattr, (int)offsetof(CharacterAttributes, attrib), iNum, fDefault)

/**********************************************************************func*
 * CreateCharacterAttributes
 *
 */
const CharacterAttributes *CreateCharacterAttributes(const CharacterAttributesTable **pptab, int iNum, float fDefault)
{
    CharacterAttributes *pattr = ParserAllocStruct(iNum*sizeof(CharacterAttributes));

    // START Generic('TRANSPOSE_ARRAY')
    TRANSPOSE_ARRAY(fDamageType[0]);
    TRANSPOSE_ARRAY(fDamageType[1]);
    TRANSPOSE_ARRAY(fDamageType[2]);
    TRANSPOSE_ARRAY(fDamageType[3]);
    TRANSPOSE_ARRAY(fDamageType[4]);
    TRANSPOSE_ARRAY(fDamageType[5]);
    TRANSPOSE_ARRAY(fDamageType[6]);
    TRANSPOSE_ARRAY(fDamageType[7]);
    TRANSPOSE_ARRAY(fDamageType[8]);
    TRANSPOSE_ARRAY(fDamageType[9]);
    TRANSPOSE_ARRAY(fDamageType[10]);
    TRANSPOSE_ARRAY(fDamageType[11]);
    TRANSPOSE_ARRAY(fDamageType[12]);
    TRANSPOSE_ARRAY(fDamageType[13]);
    TRANSPOSE_ARRAY(fDamageType[14]);
    TRANSPOSE_ARRAY(fDamageType[15]);
    TRANSPOSE_ARRAY(fDamageType[16]);
    TRANSPOSE_ARRAY(fDamageType[17]);
    TRANSPOSE_ARRAY(fDamageType[18]);
    TRANSPOSE_ARRAY(fDamageType[19]);
    TRANSPOSE_ARRAY(fHitPoints);
    TRANSPOSE_ARRAY(fAbsorb);
    TRANSPOSE_ARRAY(fEndurance);
    TRANSPOSE_ARRAY(fInsight);
    TRANSPOSE_ARRAY(fRage);
    TRANSPOSE_ARRAY(fToHit);
    TRANSPOSE_ARRAY(fDefenseType[0]);
    TRANSPOSE_ARRAY(fDefenseType[1]);
    TRANSPOSE_ARRAY(fDefenseType[2]);
    TRANSPOSE_ARRAY(fDefenseType[3]);
    TRANSPOSE_ARRAY(fDefenseType[4]);
    TRANSPOSE_ARRAY(fDefenseType[5]);
    TRANSPOSE_ARRAY(fDefenseType[6]);
    TRANSPOSE_ARRAY(fDefenseType[7]);
    TRANSPOSE_ARRAY(fDefenseType[8]);
    TRANSPOSE_ARRAY(fDefenseType[9]);
    TRANSPOSE_ARRAY(fDefenseType[10]);
    TRANSPOSE_ARRAY(fDefenseType[11]);
    TRANSPOSE_ARRAY(fDefenseType[12]);
    TRANSPOSE_ARRAY(fDefenseType[13]);
    TRANSPOSE_ARRAY(fDefenseType[14]);
    TRANSPOSE_ARRAY(fDefenseType[15]);
    TRANSPOSE_ARRAY(fDefenseType[16]);
    TRANSPOSE_ARRAY(fDefenseType[17]);
    TRANSPOSE_ARRAY(fDefenseType[18]);
    TRANSPOSE_ARRAY(fDefenseType[19]);
    TRANSPOSE_ARRAY(fDefense);
    TRANSPOSE_ARRAY(fSpeedRunning);
    TRANSPOSE_ARRAY(fSpeedFlying);
    TRANSPOSE_ARRAY(fSpeedSwimming);
    TRANSPOSE_ARRAY(fSpeedJumping);
    TRANSPOSE_ARRAY(fJumpHeight);
    TRANSPOSE_ARRAY(fMovementControl);
    TRANSPOSE_ARRAY(fMovementFriction);
    TRANSPOSE_ARRAY(fStealth);
    TRANSPOSE_ARRAY(fStealthRadius);
    TRANSPOSE_ARRAY(fStealthRadiusPlayer);
    TRANSPOSE_ARRAY(fPerceptionRadius);
    TRANSPOSE_ARRAY(fRegeneration);
    TRANSPOSE_ARRAY(fRecovery);
    TRANSPOSE_ARRAY(fInsightRecovery);
    TRANSPOSE_ARRAY(fThreatLevel);
    TRANSPOSE_ARRAY(fTaunt);
    TRANSPOSE_ARRAY(fPlacate);
    TRANSPOSE_ARRAY(fConfused);
    TRANSPOSE_ARRAY(fAfraid);
    TRANSPOSE_ARRAY(fTerrorized);
    TRANSPOSE_ARRAY(fHeld);
    TRANSPOSE_ARRAY(fImmobilized);
    TRANSPOSE_ARRAY(fStunned);
    TRANSPOSE_ARRAY(fSleep);
    TRANSPOSE_ARRAY(fFly);
    TRANSPOSE_ARRAY(fJumppack);
    TRANSPOSE_ARRAY(fTeleport);
    TRANSPOSE_ARRAY(fUntouchable);
    TRANSPOSE_ARRAY(fIntangible);
    TRANSPOSE_ARRAY(fOnlyAffectsSelf);
    TRANSPOSE_ARRAY(fExperienceGain);
    TRANSPOSE_ARRAY(fInfluenceGain);
    TRANSPOSE_ARRAY(fPrestigeGain);
    TRANSPOSE_ARRAY(fNullBool);
    TRANSPOSE_ARRAY(fKnockup);
    TRANSPOSE_ARRAY(fKnockback);
    TRANSPOSE_ARRAY(fRepel);
    TRANSPOSE_ARRAY(fAccuracy);
    TRANSPOSE_ARRAY(fRadius);
    TRANSPOSE_ARRAY(fArc);
    TRANSPOSE_ARRAY(fRange);
    TRANSPOSE_ARRAY(fTimeToActivate);
    TRANSPOSE_ARRAY(fRechargeTime);
    TRANSPOSE_ARRAY(fInterruptTime);
    TRANSPOSE_ARRAY(fEnduranceDiscount);
    TRANSPOSE_ARRAY(fInsightDiscount);
    TRANSPOSE_ARRAY(fMeter);
    TRANSPOSE_ARRAY(fElusivity[0]);
    TRANSPOSE_ARRAY(fElusivity[1]);
    TRANSPOSE_ARRAY(fElusivity[2]);
    TRANSPOSE_ARRAY(fElusivity[3]);
    TRANSPOSE_ARRAY(fElusivity[4]);
    TRANSPOSE_ARRAY(fElusivity[5]);
    TRANSPOSE_ARRAY(fElusivity[6]);
    TRANSPOSE_ARRAY(fElusivity[7]);
    TRANSPOSE_ARRAY(fElusivity[8]);
    TRANSPOSE_ARRAY(fElusivity[9]);
    TRANSPOSE_ARRAY(fElusivity[10]);
    TRANSPOSE_ARRAY(fElusivity[11]);
    TRANSPOSE_ARRAY(fElusivity[12]);
    TRANSPOSE_ARRAY(fElusivity[13]);
    TRANSPOSE_ARRAY(fElusivity[14]);
    TRANSPOSE_ARRAY(fElusivity[15]);
    TRANSPOSE_ARRAY(fElusivity[16]);
    TRANSPOSE_ARRAY(fElusivity[17]);
    TRANSPOSE_ARRAY(fElusivity[18]);
    TRANSPOSE_ARRAY(fElusivity[19]);
    TRANSPOSE_ARRAY(fElusivityBase);
    // END Generic('TRANSPOSE_ARRAY')

    return pattr;
}

/**********************************************************************func*
 * ClampMax
 *
 */
void ClampMax(Character *p, CharacterAttribSet *pset)
{
    CharacterAttributes *pattr = pset->pattrMax;
    const CharacterAttributes *pattrMin = p->pclass->ppattrMin[0];
    const CharacterAttributes *pattrMax = &p->pclass->pattrMaxMax[p->iCombatLevel];

    // Hack to ensure max HP never goes below 1.  Non-hack way to implement is a MaxMin that differs from the CurMin.
    if (pattr->fHitPoints < 1.0f)
    {
        pattr->fHitPoints = 1.0f;
    }

#define CLAMP_MAX(attr) \
	CLAMPF32_IN_PLACE(&pattr->attr, pattrMin->attr, pattrMax->attr)

    // START Generic('CLAMP_MAX')
    CLAMP_MAX(fDamageType[0]);
    CLAMP_MAX(fDamageType[1]);
    CLAMP_MAX(fDamageType[2]);
    CLAMP_MAX(fDamageType[3]);
    CLAMP_MAX(fDamageType[4]);
    CLAMP_MAX(fDamageType[5]);
    CLAMP_MAX(fDamageType[6]);
    CLAMP_MAX(fDamageType[7]);
    CLAMP_MAX(fDamageType[8]);
    CLAMP_MAX(fDamageType[9]);
    CLAMP_MAX(fDamageType[10]);
    CLAMP_MAX(fDamageType[11]);
    CLAMP_MAX(fDamageType[12]);
    CLAMP_MAX(fDamageType[13]);
    CLAMP_MAX(fDamageType[14]);
    CLAMP_MAX(fDamageType[15]);
    CLAMP_MAX(fDamageType[16]);
    CLAMP_MAX(fDamageType[17]);
    CLAMP_MAX(fDamageType[18]);
    CLAMP_MAX(fDamageType[19]);
    CLAMP_MAX(fHitPoints);
    CLAMP_MAX(fAbsorb);
    CLAMP_MAX(fEndurance);
    CLAMP_MAX(fInsight);
    CLAMP_MAX(fRage);
    CLAMP_MAX(fToHit);
    CLAMP_MAX(fDefenseType[0]);
    CLAMP_MAX(fDefenseType[1]);
    CLAMP_MAX(fDefenseType[2]);
    CLAMP_MAX(fDefenseType[3]);
    CLAMP_MAX(fDefenseType[4]);
    CLAMP_MAX(fDefenseType[5]);
    CLAMP_MAX(fDefenseType[6]);
    CLAMP_MAX(fDefenseType[7]);
    CLAMP_MAX(fDefenseType[8]);
    CLAMP_MAX(fDefenseType[9]);
    CLAMP_MAX(fDefenseType[10]);
    CLAMP_MAX(fDefenseType[11]);
    CLAMP_MAX(fDefenseType[12]);
    CLAMP_MAX(fDefenseType[13]);
    CLAMP_MAX(fDefenseType[14]);
    CLAMP_MAX(fDefenseType[15]);
    CLAMP_MAX(fDefenseType[16]);
    CLAMP_MAX(fDefenseType[17]);
    CLAMP_MAX(fDefenseType[18]);
    CLAMP_MAX(fDefenseType[19]);
    CLAMP_MAX(fDefense);
    CLAMP_MAX(fSpeedRunning);
    CLAMP_MAX(fSpeedFlying);
    CLAMP_MAX(fSpeedSwimming);
    CLAMP_MAX(fSpeedJumping);
    CLAMP_MAX(fJumpHeight);
    CLAMP_MAX(fMovementControl);
    CLAMP_MAX(fMovementFriction);
    CLAMP_MAX(fStealth);
    CLAMP_MAX(fStealthRadius);
    CLAMP_MAX(fStealthRadiusPlayer);
    CLAMP_MAX(fPerceptionRadius);
    CLAMP_MAX(fRegeneration);
    CLAMP_MAX(fRecovery);
    CLAMP_MAX(fInsightRecovery);
    CLAMP_MAX(fThreatLevel);
    CLAMP_MAX(fTaunt);
    CLAMP_MAX(fPlacate);
    CLAMP_MAX(fConfused);
    CLAMP_MAX(fAfraid);
    CLAMP_MAX(fTerrorized);
    CLAMP_MAX(fHeld);
    CLAMP_MAX(fImmobilized);
    CLAMP_MAX(fStunned);
    CLAMP_MAX(fSleep);
    CLAMP_MAX(fFly);
    CLAMP_MAX(fJumppack);
    CLAMP_MAX(fTeleport);
    CLAMP_MAX(fUntouchable);
    CLAMP_MAX(fIntangible);
    CLAMP_MAX(fOnlyAffectsSelf);
    CLAMP_MAX(fExperienceGain);
    CLAMP_MAX(fInfluenceGain);
    CLAMP_MAX(fPrestigeGain);
    CLAMP_MAX(fNullBool);
    CLAMP_MAX(fKnockup);
    CLAMP_MAX(fKnockback);
    CLAMP_MAX(fRepel);
    CLAMP_MAX(fAccuracy);
    CLAMP_MAX(fRadius);
    CLAMP_MAX(fArc);
    CLAMP_MAX(fRange);
    CLAMP_MAX(fTimeToActivate);
    CLAMP_MAX(fRechargeTime);
    CLAMP_MAX(fInterruptTime);
    CLAMP_MAX(fEnduranceDiscount);
    CLAMP_MAX(fInsightDiscount);
    CLAMP_MAX(fMeter);
    CLAMP_MAX(fElusivity[0]);
    CLAMP_MAX(fElusivity[1]);
    CLAMP_MAX(fElusivity[2]);
    CLAMP_MAX(fElusivity[3]);
    CLAMP_MAX(fElusivity[4]);
    CLAMP_MAX(fElusivity[5]);
    CLAMP_MAX(fElusivity[6]);
    CLAMP_MAX(fElusivity[7]);
    CLAMP_MAX(fElusivity[8]);
    CLAMP_MAX(fElusivity[9]);
    CLAMP_MAX(fElusivity[10]);
    CLAMP_MAX(fElusivity[11]);
    CLAMP_MAX(fElusivity[12]);
    CLAMP_MAX(fElusivity[13]);
    CLAMP_MAX(fElusivity[14]);
    CLAMP_MAX(fElusivity[15]);
    CLAMP_MAX(fElusivity[16]);
    CLAMP_MAX(fElusivity[17]);
    CLAMP_MAX(fElusivity[18]);
    CLAMP_MAX(fElusivity[19]);    
    CLAMP_MAX(fElusivityBase);
    // END Generic('CLAMP_MAX')
}


/**********************************************************************func*
 * CalcResistance
 *
 * This is no longer auto-generated since it was hand-optimized.
 *
 */
void CalcResistance(Character *p, CharacterAttribSet *pset, CharacterAttributes *pattrResBuffs, CharacterAttributes *pattrResDebuffs)
{

#define DIMIN_RESISTANCE() \
    *pfResBuffs = attribDiminishValue(*pfResBuffs, *pfResDiminInner, *pfResDiminOuter); \
    *pfResDebuffs = attribDiminishValue(*pfResDebuffs, *pfResDiminInner, *pfResDiminOuter); \
    pfResBuffs++; \
    pfResDebuffs++; \
    pfResDiminOuter++; \
    pfResDiminInner++;

#if SERVER
    if (server_state.enablePowerDiminishing)
#elif CLIENT 
    if (game_state.enablePowerDiminishing)
#endif
    {
        int i;
        float *pfResBuffs = (float *)pattrResBuffs;
        float *pfResDebuffs = (float *)pattrResDebuffs;
        float *pfResDiminInner = (float *)*p->pclass->ppattrDiminRes[kClassesDiminish_Inner];
        float *pfResDiminOuter = (float *)*p->pclass->ppattrDiminRes[kClassesDiminish_Outer];

        for(i=sizeof(CharacterAttributes)/sizeof(float); i>0; i--)
        {
            DIMIN_RESISTANCE();
        }
    }


    // I iterate over pfRes twice since it allows all of the loop variables
    // to be held in registers. If I stick it all in one loop, it thrashes
    // around with constant loads and stores.

#define CALC_RESISTANCE() \
    *(int *)pfRes = *(int *)pfResBuffs; \
    if(*pfResBuffs < 1.0f) \
        *pfRes += (1.0f-(*pfResBuffs))*(*pfResDebuffs); \
    pfResBuffs++; \
    pfResDebuffs++; \
    pfRes++;

    {
        int i;
        float *pfResBuffs = (float *)pattrResBuffs;
        float *pfResDebuffs = (float *)pattrResDebuffs;
        float *pfRes = (float *)pset->pattrResistance;

        for(i=sizeof(CharacterAttributes)/sizeof(float); i>0; i--)
        {
            CALC_RESISTANCE();
        }
    }

#define CLAMP_RESISTANCE() \
    CLAMPF32_IN_PLACE(pfRes, *pfResMin, *pfResMax); \
    pfResMax++; \
    pfResMin++; \
    pfRes++;

    {
        int i;
        float *pfResMin = (float *)p->pclass->ppattrResistanceMin[0];
        float *pfResMax = (float *)&p->pclass->pattrResistanceMax[p->iCombatLevel];
        float *pfRes = (float *)pset->pattrResistance;

        for(i=sizeof(CharacterAttributes)/sizeof(float); i>0; i--)
        {
            CLAMP_RESISTANCE();
        }
    }
}

/**********************************************************************func*
 * ClampStrength
 *
 */
void ClampStrength(Character *p, CharacterAttributes *pattrStr)
{
    const CharacterAttributes *pattrStrMin = p->pclass->ppattrStrengthMin[0];
    const CharacterAttributes *pattrStrMax = &p->pclass->pattrStrengthMax[p->iCombatLevel];

#define CLAMP_STRENGTH(attr) \
	CLAMPF32_IN_PLACE(&pattrStr->attr, pattrStrMin->attr, pattrStrMax->attr)

    // START Generic('CLAMP_STRENGTH')
    CLAMP_STRENGTH(fDamageType[0]);
    CLAMP_STRENGTH(fDamageType[1]);
    CLAMP_STRENGTH(fDamageType[2]);
    CLAMP_STRENGTH(fDamageType[3]);
    CLAMP_STRENGTH(fDamageType[4]);
    CLAMP_STRENGTH(fDamageType[5]);
    CLAMP_STRENGTH(fDamageType[6]);
    CLAMP_STRENGTH(fDamageType[7]);
    CLAMP_STRENGTH(fDamageType[8]);
    CLAMP_STRENGTH(fDamageType[9]);
    CLAMP_STRENGTH(fDamageType[10]);
    CLAMP_STRENGTH(fDamageType[11]);
    CLAMP_STRENGTH(fDamageType[12]);
    CLAMP_STRENGTH(fDamageType[13]);
    CLAMP_STRENGTH(fDamageType[14]);
    CLAMP_STRENGTH(fDamageType[15]);
    CLAMP_STRENGTH(fDamageType[16]);
    CLAMP_STRENGTH(fDamageType[17]);
    CLAMP_STRENGTH(fDamageType[18]);
    CLAMP_STRENGTH(fDamageType[19]);
    CLAMP_STRENGTH(fHitPoints);
    CLAMP_STRENGTH(fAbsorb);
    CLAMP_STRENGTH(fEndurance);
    CLAMP_STRENGTH(fInsight);
    CLAMP_STRENGTH(fRage);
    CLAMP_STRENGTH(fToHit);
    CLAMP_STRENGTH(fDefenseType[0]);
    CLAMP_STRENGTH(fDefenseType[1]);
    CLAMP_STRENGTH(fDefenseType[2]);
    CLAMP_STRENGTH(fDefenseType[3]);
    CLAMP_STRENGTH(fDefenseType[4]);
    CLAMP_STRENGTH(fDefenseType[5]);
    CLAMP_STRENGTH(fDefenseType[6]);
    CLAMP_STRENGTH(fDefenseType[7]);
    CLAMP_STRENGTH(fDefenseType[8]);
    CLAMP_STRENGTH(fDefenseType[9]);
    CLAMP_STRENGTH(fDefenseType[10]);
    CLAMP_STRENGTH(fDefenseType[11]);
    CLAMP_STRENGTH(fDefenseType[12]);
    CLAMP_STRENGTH(fDefenseType[13]);
    CLAMP_STRENGTH(fDefenseType[14]);
    CLAMP_STRENGTH(fDefenseType[15]);
    CLAMP_STRENGTH(fDefenseType[16]);
    CLAMP_STRENGTH(fDefenseType[17]);
    CLAMP_STRENGTH(fDefenseType[18]);
    CLAMP_STRENGTH(fDefenseType[19]);
    CLAMP_STRENGTH(fDefense);
    CLAMP_STRENGTH(fSpeedRunning);
    CLAMP_STRENGTH(fSpeedFlying);
    CLAMP_STRENGTH(fSpeedSwimming);
    CLAMP_STRENGTH(fSpeedJumping);
    CLAMP_STRENGTH(fJumpHeight);
    CLAMP_STRENGTH(fMovementControl);
    CLAMP_STRENGTH(fMovementFriction);
    CLAMP_STRENGTH(fStealth);
    CLAMP_STRENGTH(fStealthRadius);
    CLAMP_STRENGTH(fStealthRadiusPlayer);
    CLAMP_STRENGTH(fPerceptionRadius);
    CLAMP_STRENGTH(fRegeneration);
    CLAMP_STRENGTH(fRecovery);
    CLAMP_STRENGTH(fInsightRecovery);
    CLAMP_STRENGTH(fThreatLevel);
    CLAMP_STRENGTH(fTaunt);
    CLAMP_STRENGTH(fPlacate);
    CLAMP_STRENGTH(fConfused);
    CLAMP_STRENGTH(fAfraid);
    CLAMP_STRENGTH(fTerrorized);
    CLAMP_STRENGTH(fHeld);
    CLAMP_STRENGTH(fImmobilized);
    CLAMP_STRENGTH(fStunned);
    CLAMP_STRENGTH(fSleep);
    CLAMP_STRENGTH(fFly);
    CLAMP_STRENGTH(fJumppack);
    CLAMP_STRENGTH(fTeleport);
    CLAMP_STRENGTH(fUntouchable);
    CLAMP_STRENGTH(fIntangible);
    CLAMP_STRENGTH(fOnlyAffectsSelf);
    CLAMP_STRENGTH(fExperienceGain);
    CLAMP_STRENGTH(fInfluenceGain);
    CLAMP_STRENGTH(fPrestigeGain);
    CLAMP_STRENGTH(fNullBool);
    CLAMP_STRENGTH(fKnockup);
    CLAMP_STRENGTH(fKnockback);
    CLAMP_STRENGTH(fRepel);
    CLAMP_STRENGTH(fAccuracy);
    CLAMP_STRENGTH(fRadius);
    CLAMP_STRENGTH(fArc);
    CLAMP_STRENGTH(fRange);
    CLAMP_STRENGTH(fTimeToActivate);
    CLAMP_STRENGTH(fRechargeTime);
    CLAMP_STRENGTH(fInterruptTime);
    CLAMP_STRENGTH(fEnduranceDiscount);
    CLAMP_STRENGTH(fInsightDiscount);
    CLAMP_STRENGTH(fMeter);
    CLAMP_STRENGTH(fElusivity[0]);
    CLAMP_STRENGTH(fElusivity[1]);
    CLAMP_STRENGTH(fElusivity[2]);
    CLAMP_STRENGTH(fElusivity[3]);
    CLAMP_STRENGTH(fElusivity[4]);
    CLAMP_STRENGTH(fElusivity[5]);
    CLAMP_STRENGTH(fElusivity[6]);
    CLAMP_STRENGTH(fElusivity[7]);
    CLAMP_STRENGTH(fElusivity[8]);
    CLAMP_STRENGTH(fElusivity[9]);
    CLAMP_STRENGTH(fElusivity[10]);
    CLAMP_STRENGTH(fElusivity[11]);
    CLAMP_STRENGTH(fElusivity[12]);
    CLAMP_STRENGTH(fElusivity[13]);
    CLAMP_STRENGTH(fElusivity[14]);
    CLAMP_STRENGTH(fElusivity[15]);
    CLAMP_STRENGTH(fElusivity[16]);
    CLAMP_STRENGTH(fElusivity[17]);
    CLAMP_STRENGTH(fElusivity[18]);
    CLAMP_STRENGTH(fElusivity[19]);
    CLAMP_STRENGTH(fElusivityBase);
    // END Generic('CLAMP_STRENGTH')
}

/**********************************************************************func*
 * CalcCur
 *
 */
void CalcCur(Character *p, CharacterAttribSet *pset)
{

    // diminish if required
#if SERVER
    if (server_state.enablePowerDiminishing)
#elif CLIENT 
    if (game_state.enablePowerDiminishing)
#endif
    {
        int i;
        CharacterAttributes modBase;
        float *pfMod = (float *)pset->pattrMod;
        float *pfModBase = (float *)&modBase;
        float *pfCurDiminInner = (float *)*p->pclass->ppattrDiminCur[kClassesDiminish_Inner];
        float *pfCurDiminOuter = (float *)*p->pclass->ppattrDiminCur[kClassesDiminish_Outer];
        int offset = offsetof(CharacterAttributes, fRage) / sizeof(float);

        SetModBase(&modBase);

        for(i=sizeof(CharacterAttributes)/sizeof(float); i>0; i--)
        {
            int currentOffset = (pfMod - (float *)pset->pattrMod);
            if (currentOffset >= offset)
                *pfMod = attribDiminishValue((*pfMod - *pfModBase), *pfCurDiminInner, *pfCurDiminOuter) + *pfModBase; 
            pfMod++; 
            pfModBase++; 
            pfCurDiminInner++;
            pfCurDiminOuter++;
        }
    }
 
    // The clamp ranges are set.
    // Calculate the modified attributes

    // START CalcCur
                                                                            // NOTE: These are HP's Max on purpose!
    p->attrMax.fAbsorb                += pset->pattrMod->fDamageType[0]      * pset->pattrMax->fHitPoints          + pset->pattrAbsolute->fDamageType[0];
    p->attrMax.fAbsorb                += pset->pattrMod->fDamageType[1]      * pset->pattrMax->fHitPoints          + pset->pattrAbsolute->fDamageType[1];
    p->attrMax.fAbsorb                += pset->pattrMod->fDamageType[2]      * pset->pattrMax->fHitPoints          + pset->pattrAbsolute->fDamageType[2];
    p->attrMax.fAbsorb                += pset->pattrMod->fDamageType[3]      * pset->pattrMax->fHitPoints          + pset->pattrAbsolute->fDamageType[3];
    p->attrMax.fAbsorb                += pset->pattrMod->fDamageType[4]      * pset->pattrMax->fHitPoints          + pset->pattrAbsolute->fDamageType[4];
    p->attrMax.fAbsorb                += pset->pattrMod->fDamageType[5]      * pset->pattrMax->fHitPoints          + pset->pattrAbsolute->fDamageType[5];
    p->attrMax.fAbsorb                += pset->pattrMod->fDamageType[6]      * pset->pattrMax->fHitPoints          + pset->pattrAbsolute->fDamageType[6];
    // fDamageType[7] is "Heal", apply this directly to HitPoints! 
    p->attrCur.fHitPoints            += pset->pattrMod->fDamageType[7]      * pset->pattrMax->fHitPoints          + pset->pattrAbsolute->fDamageType[7];
    // fDamageType[8] is "Special", apply this directly to HitPoints!
    p->attrCur.fHitPoints            += pset->pattrMod->fDamageType[8]      * pset->pattrMax->fHitPoints          + pset->pattrAbsolute->fDamageType[8];
    p->attrMax.fAbsorb                += pset->pattrMod->fDamageType[9]      * pset->pattrMax->fHitPoints          + pset->pattrAbsolute->fDamageType[9];
    p->attrMax.fAbsorb                += pset->pattrMod->fDamageType[10]     * pset->pattrMax->fHitPoints          + pset->pattrAbsolute->fDamageType[10];
    p->attrMax.fAbsorb                += pset->pattrMod->fDamageType[11]     * pset->pattrMax->fHitPoints          + pset->pattrAbsolute->fDamageType[11];
    p->attrMax.fAbsorb                += pset->pattrMod->fDamageType[12]     * pset->pattrMax->fHitPoints          + pset->pattrAbsolute->fDamageType[12];
    p->attrMax.fAbsorb                += pset->pattrMod->fDamageType[13]     * pset->pattrMax->fHitPoints          + pset->pattrAbsolute->fDamageType[13];
    p->attrMax.fAbsorb                += pset->pattrMod->fDamageType[14]     * pset->pattrMax->fHitPoints          + pset->pattrAbsolute->fDamageType[14];
    p->attrMax.fAbsorb                += pset->pattrMod->fDamageType[15]     * pset->pattrMax->fHitPoints          + pset->pattrAbsolute->fDamageType[15];
    p->attrMax.fAbsorb                += pset->pattrMod->fDamageType[16]     * pset->pattrMax->fHitPoints          + pset->pattrAbsolute->fDamageType[16];
    p->attrMax.fAbsorb                += pset->pattrMod->fDamageType[17]     * pset->pattrMax->fHitPoints          + pset->pattrAbsolute->fDamageType[17];
    p->attrMax.fAbsorb                += pset->pattrMod->fDamageType[18]     * pset->pattrMax->fHitPoints          + pset->pattrAbsolute->fDamageType[18];
    p->attrMax.fAbsorb                += pset->pattrMod->fDamageType[19]     * pset->pattrMax->fHitPoints          + pset->pattrAbsolute->fDamageType[19];
    if (p->attrMax.fAbsorb < 0.0f)
    {
        p->attrCur.fHitPoints += p->attrMax.fAbsorb;
        p->attrMax.fAbsorb = 0.0f;
    }
    p->attrCur.fHitPoints            += pset->pattrMod->fHitPoints          * pset->pattrMax->fHitPoints          + pset->pattrAbsolute->fHitPoints;
    p->attrCur.fEndurance            += pset->pattrMod->fEndurance          * pset->pattrMax->fEndurance          + pset->pattrAbsolute->fEndurance;
    p->attrCur.fInsight                += pset->pattrMod->fInsight            * pset->pattrMax->fInsight            + pset->pattrAbsolute->fInsight;
    p->attrCur.fRage                += pset->pattrMod->fRage               * pset->pattrMax->fRage               + pset->pattrAbsolute->fRage;
    p->attrCur.fToHit                += pset->pattrMod->fToHit;
    p->attrCur.fDefenseType[0]     += pset->pattrMod->fDefenseType[0];
    p->attrCur.fDefenseType[1]     += pset->pattrMod->fDefenseType[1];
    p->attrCur.fDefenseType[2]     += pset->pattrMod->fDefenseType[2];
    p->attrCur.fDefenseType[3]     += pset->pattrMod->fDefenseType[3];
    p->attrCur.fDefenseType[4]     += pset->pattrMod->fDefenseType[4];
    p->attrCur.fDefenseType[5]     += pset->pattrMod->fDefenseType[5];
    p->attrCur.fDefenseType[6]     += pset->pattrMod->fDefenseType[6];
    p->attrCur.fDefenseType[7]     += pset->pattrMod->fDefenseType[7];
    p->attrCur.fDefenseType[8]     += pset->pattrMod->fDefenseType[8];
    p->attrCur.fDefenseType[9]     += pset->pattrMod->fDefenseType[9];
    p->attrCur.fDefenseType[10]    += pset->pattrMod->fDefenseType[10];
    p->attrCur.fDefenseType[11]    += pset->pattrMod->fDefenseType[11];
    p->attrCur.fDefenseType[12]    += pset->pattrMod->fDefenseType[12];
    p->attrCur.fDefenseType[13]    += pset->pattrMod->fDefenseType[13];
    p->attrCur.fDefenseType[14]    += pset->pattrMod->fDefenseType[14];
    p->attrCur.fDefenseType[15]    += pset->pattrMod->fDefenseType[15];
    p->attrCur.fDefenseType[16]    += pset->pattrMod->fDefenseType[16];
    p->attrCur.fDefenseType[17]    += pset->pattrMod->fDefenseType[17];
    p->attrCur.fDefenseType[18]    += pset->pattrMod->fDefenseType[18];
    p->attrCur.fDefenseType[19]    += pset->pattrMod->fDefenseType[19];
    p->attrCur.fDefense            += pset->pattrMod->fDefense;
    p->attrCur.fSpeedRunning       *= pset->pattrMod->fSpeedRunning;
    p->attrCur.fSpeedFlying        *= pset->pattrMod->fSpeedFlying;
    p->attrCur.fSpeedSwimming      *= pset->pattrMod->fSpeedSwimming;
    p->attrCur.fSpeedJumping       *= pset->pattrMod->fSpeedJumping;
    p->attrCur.fJumpHeight         *= pset->pattrMod->fJumpHeight;
    p->attrCur.fMovementControl    *= pset->pattrMod->fMovementControl;
    p->attrCur.fMovementFriction   *= pset->pattrMod->fMovementFriction;
    p->attrCur.fStealth            += pset->pattrMod->fStealth;
    p->attrCur.fStealthRadius      += pset->pattrMod->fStealthRadius;
    p->attrCur.fStealthRadiusPlayer += pset->pattrMod->fStealthRadiusPlayer;
    p->attrCur.fPerceptionRadius   *= pset->pattrMod->fPerceptionRadius;
    p->attrCur.fPerceptionRadius   += pset->pattrAbsolute->fPerceptionRadius;
    p->attrCur.fRegeneration       *= pset->pattrMod->fRegeneration;
    p->attrCur.fRecovery           *= pset->pattrMod->fRecovery;
    p->attrCur.fInsightRecovery    *= pset->pattrMod->fInsightRecovery;
    p->attrCur.fThreatLevel        += pset->pattrMod->fThreatLevel;
    p->attrCur.fTaunt              += pset->pattrMod->fTaunt;
    p->attrCur.fPlacate            += pset->pattrMod->fPlacate;
    p->attrCur.fConfused           += pset->pattrMod->fConfused;
    p->attrCur.fAfraid             += pset->pattrMod->fAfraid;
    p->attrCur.fTerrorized         += pset->pattrMod->fTerrorized;
    p->attrCur.fHeld               += pset->pattrMod->fHeld;
    p->attrCur.fImmobilized        += pset->pattrMod->fImmobilized;
    p->attrCur.fStunned            += pset->pattrMod->fStunned;
    p->attrCur.fSleep              += pset->pattrMod->fSleep;
    p->attrCur.fFly                += pset->pattrMod->fFly;
    p->attrCur.fJumppack           += pset->pattrMod->fJumppack;
    p->attrCur.fTeleport           += pset->pattrMod->fTeleport;
    p->attrCur.fUntouchable        += pset->pattrMod->fUntouchable;
    p->attrCur.fIntangible         += pset->pattrMod->fIntangible;
    p->attrCur.fOnlyAffectsSelf    += pset->pattrMod->fOnlyAffectsSelf;
    p->attrCur.fExperienceGain     += pset->pattrMod->fExperienceGain;
    p->attrCur.fInfluenceGain      += pset->pattrMod->fInfluenceGain;
    p->attrCur.fPrestigeGain       += pset->pattrMod->fPrestigeGain;
    p->attrCur.fNullBool           += pset->pattrMod->fNullBool;
    p->attrCur.fKnockup            *= pset->pattrMod->fKnockup;
    p->attrCur.fKnockback          *= pset->pattrMod->fKnockback;
    p->attrCur.fRepel              *= pset->pattrMod->fRepel;
    p->attrCur.fAccuracy           *= pset->pattrMod->fAccuracy;
    p->attrCur.fRadius             *= pset->pattrMod->fRadius;
    p->attrCur.fArc                *= pset->pattrMod->fArc;
    p->attrCur.fRange              *= pset->pattrMod->fRange;
    p->attrCur.fTimeToActivate     *= pset->pattrMod->fTimeToActivate;
    p->attrCur.fRechargeTime       *= pset->pattrMod->fRechargeTime;
    p->attrCur.fInterruptTime      *= pset->pattrMod->fInterruptTime;
    p->attrCur.fEnduranceDiscount  *= pset->pattrMod->fEnduranceDiscount;
    p->attrCur.fInsightDiscount    *= pset->pattrMod->fInsightDiscount;
    p->attrCur.fMeter              += pset->pattrMod->fMeter;
    p->attrCur.fMeter              += pset->pattrAbsolute->fMeter;
    p->attrCur.fElusivity[0]       *= pset->pattrMod->fElusivity[0];
    p->attrCur.fElusivity[1]       *= pset->pattrMod->fElusivity[1];
    p->attrCur.fElusivity[2]       *= pset->pattrMod->fElusivity[2];
    p->attrCur.fElusivity[3]       *= pset->pattrMod->fElusivity[3];
    p->attrCur.fElusivity[4]       *= pset->pattrMod->fElusivity[4];
    p->attrCur.fElusivity[5]       *= pset->pattrMod->fElusivity[5];
    p->attrCur.fElusivity[6]       *= pset->pattrMod->fElusivity[6];
    p->attrCur.fElusivity[7]       *= pset->pattrMod->fElusivity[7];
    p->attrCur.fElusivity[8]       *= pset->pattrMod->fElusivity[8];
    p->attrCur.fElusivity[9]       *= pset->pattrMod->fElusivity[9];
    p->attrCur.fElusivity[10]       *= pset->pattrMod->fElusivity[10];
    p->attrCur.fElusivity[11]       *= pset->pattrMod->fElusivity[11];
    p->attrCur.fElusivity[12]       *= pset->pattrMod->fElusivity[12];
    p->attrCur.fElusivity[13]       *= pset->pattrMod->fElusivity[13];
    p->attrCur.fElusivity[14]       *= pset->pattrMod->fElusivity[14];
    p->attrCur.fElusivity[15]       *= pset->pattrMod->fElusivity[15];
    p->attrCur.fElusivity[16]      *= pset->pattrMod->fElusivity[16];
    p->attrCur.fElusivity[17]       *= pset->pattrMod->fElusivity[17];
    p->attrCur.fElusivity[18]      *= pset->pattrMod->fElusivity[18];
    p->attrCur.fElusivity[19]      *= pset->pattrMod->fElusivity[19];
    p->attrCur.fElusivityBase       *= pset->pattrMod->fElusivityBase;
    // END CalcCur
}

/**********************************************************************func*
 * SetModBase
 *
 */
void SetModBase(CharacterAttributes *pattr)
{
    // START SetModBase
    pattr->fDamageType[0]      = 0.0;
    pattr->fDamageType[1]      = 0.0;
    pattr->fDamageType[2]      = 0.0;
    pattr->fDamageType[3]      = 0.0;
    pattr->fDamageType[4]      = 0.0;
    pattr->fDamageType[5]      = 0.0;
    pattr->fDamageType[6]      = 0.0;
    pattr->fDamageType[7]      = 0.0;
    pattr->fDamageType[8]      = 0.0;
    pattr->fDamageType[9]      = 0.0;
    pattr->fDamageType[10]     = 0.0;
    pattr->fDamageType[11]     = 0.0;
    pattr->fDamageType[12]     = 0.0;
    pattr->fDamageType[13]     = 0.0;
    pattr->fDamageType[14]     = 0.0;
    pattr->fDamageType[15]     = 0.0;
    pattr->fDamageType[16]     = 0.0;
    pattr->fDamageType[17]     = 0.0;
    pattr->fDamageType[18]     = 0.0;
    pattr->fDamageType[19]     = 0.0;
    pattr->fHitPoints          = 0.0;
    pattr->fAbsorb             = 0.0;
    pattr->fEndurance          = 0.0;
    pattr->fInsight            = 0.0;
    pattr->fRage               = 0.0;
    pattr->fToHit              = 0.0;
    pattr->fDefenseType[0]     = 0.0;
    pattr->fDefenseType[1]     = 0.0;
    pattr->fDefenseType[2]     = 0.0;
    pattr->fDefenseType[3]     = 0.0;
    pattr->fDefenseType[4]     = 0.0;
    pattr->fDefenseType[5]     = 0.0;
    pattr->fDefenseType[6]     = 0.0;
    pattr->fDefenseType[7]     = 0.0;
    pattr->fDefenseType[8]     = 0.0;
    pattr->fDefenseType[9]     = 0.0;
    pattr->fDefenseType[10]    = 0.0;
    pattr->fDefenseType[11]    = 0.0;
    pattr->fDefenseType[12]    = 0.0;
    pattr->fDefenseType[13]    = 0.0;
    pattr->fDefenseType[14]    = 0.0;
    pattr->fDefenseType[15]    = 0.0;
    pattr->fDefenseType[16]    = 0.0;
    pattr->fDefenseType[17]    = 0.0;
    pattr->fDefenseType[18]    = 0.0;
    pattr->fDefenseType[19]    = 0.0;
    pattr->fDefense            = 0.0;
    pattr->fSpeedRunning       = 1.0;
    pattr->fSpeedFlying        = 1.0;
    pattr->fSpeedSwimming      = 1.0;
    pattr->fSpeedJumping       = 1.0;
    pattr->fJumpHeight         = 1.0;
    pattr->fMovementControl    = 1.0;
    pattr->fMovementFriction   = 1.0;
    pattr->fStealth            = 0.0;
    pattr->fStealthRadius      = 0.0;
    pattr->fStealthRadiusPlayer = 0.0;
    pattr->fPerceptionRadius   = 1.0;
    pattr->fRegeneration       = 1.0;
    pattr->fRecovery           = 1.0;
    pattr->fInsightRecovery    = 1.0;
    pattr->fThreatLevel        = 0.0;
    pattr->fTaunt              = 1.0;
    pattr->fPlacate            = 1.0;
    pattr->fConfused           = 0.0;
    pattr->fAfraid             = 0.0;
    pattr->fTerrorized         = 0.0;
    pattr->fHeld               = 0.0;
    pattr->fImmobilized        = 0.0;
    pattr->fStunned            = 0.0;
    pattr->fSleep              = 0.0;
    pattr->fFly                = 0.0;
    pattr->fJumppack           = 0.0;
    pattr->fTeleport           = 0.0;
    pattr->fUntouchable        = 0.0;
    pattr->fIntangible         = 0.0;
    pattr->fOnlyAffectsSelf    = 0.0;
    pattr->fExperienceGain     = 0.0;
    pattr->fInfluenceGain      = 0.0;
    pattr->fPrestigeGain       = 0.0;
    pattr->fNullBool           = 0.0;
    pattr->fKnockup            = 0.0;
    pattr->fKnockback          = 0.0;
    pattr->fRepel              = 0.0;
    pattr->fAccuracy           = 1.0;
    pattr->fRadius             = 1.0;
    pattr->fArc                = 1.0;
    pattr->fRange              = 1.0;
    pattr->fTimeToActivate     = 1.0;
    pattr->fRechargeTime       = 1.0;
    pattr->fInterruptTime      = 1.0;
    pattr->fEnduranceDiscount  = 1.0;
    pattr->fInsightDiscount    = 1.0;
    pattr->fMeter              = 0.0;
    pattr->fElusivity[0]       = 1.0;
    pattr->fElusivity[1]       = 1.0;
    pattr->fElusivity[2]       = 1.0;
    pattr->fElusivity[3]       = 1.0;
    pattr->fElusivity[4]       = 1.0;
    pattr->fElusivity[5]       = 1.0;
    pattr->fElusivity[6]       = 1.0;
    pattr->fElusivity[7]       = 1.0;
    pattr->fElusivity[8]       = 1.0;
    pattr->fElusivity[9]       = 1.0;
    pattr->fElusivity[10]      = 1.0;
    pattr->fElusivity[11]      = 1.0;
    pattr->fElusivity[12]      = 1.0;
    pattr->fElusivity[13]      = 1.0;
    pattr->fElusivity[14]      = 1.0;
    pattr->fElusivity[15]      = 1.0;
    pattr->fElusivity[16]      = 1.0;
    pattr->fElusivity[17]      = 1.0;
    pattr->fElusivity[18]      = 1.0;
    pattr->fElusivity[19]      = 1.0;
    pattr->fElusivityBase      = 1.0;
    // END SetModBase
}

/**********************************************************************func*
 * ClampCur
 *
 */
void ClampCur(Character *p)
{
    CharacterAttributes *pattr = &p->attrCur;
    const CharacterAttributes *pattrMin = p->pclass->ppattrMin[0];
    const CharacterAttributes *pattrMax = &p->attrMax;

#define CLAMP_CUR(attr) \
	CLAMPF32_IN_PLACE(&pattr->attr, pattrMin->attr, pattrMax->attr)

    // START Generic('CLAMP_CUR')
    CLAMP_CUR(fDamageType[0]);
    CLAMP_CUR(fDamageType[1]);
    CLAMP_CUR(fDamageType[2]);
    CLAMP_CUR(fDamageType[3]);
    CLAMP_CUR(fDamageType[4]);
    CLAMP_CUR(fDamageType[5]);
    CLAMP_CUR(fDamageType[6]);
    CLAMP_CUR(fDamageType[7]);
    CLAMP_CUR(fDamageType[8]);
    CLAMP_CUR(fDamageType[9]);
    CLAMP_CUR(fDamageType[10]);
    CLAMP_CUR(fDamageType[11]);
    CLAMP_CUR(fDamageType[12]);
    CLAMP_CUR(fDamageType[13]);
    CLAMP_CUR(fDamageType[14]);
    CLAMP_CUR(fDamageType[15]);
    CLAMP_CUR(fDamageType[16]);
    CLAMP_CUR(fDamageType[17]);
    CLAMP_CUR(fDamageType[18]);
    CLAMP_CUR(fDamageType[19]);
    CLAMP_CUR(fHitPoints);
    //CLAMP_CUR(fAbsorb); // don't bother, we don't use this stat
    CLAMP_CUR(fEndurance);
    CLAMP_CUR(fInsight);
    CLAMP_CUR(fRage);
    CLAMP_CUR(fToHit);
    CLAMP_CUR(fDefenseType[0]);
    CLAMP_CUR(fDefenseType[1]);
    CLAMP_CUR(fDefenseType[2]);
    CLAMP_CUR(fDefenseType[3]);
    CLAMP_CUR(fDefenseType[4]);
    CLAMP_CUR(fDefenseType[5]);
    CLAMP_CUR(fDefenseType[6]);
    CLAMP_CUR(fDefenseType[7]);
    CLAMP_CUR(fDefenseType[8]);
    CLAMP_CUR(fDefenseType[9]);
    CLAMP_CUR(fDefenseType[10]);
    CLAMP_CUR(fDefenseType[11]);
    CLAMP_CUR(fDefenseType[12]);
    CLAMP_CUR(fDefenseType[13]);
    CLAMP_CUR(fDefenseType[14]);
    CLAMP_CUR(fDefenseType[15]);
    CLAMP_CUR(fDefenseType[16]);
    CLAMP_CUR(fDefenseType[17]);
    CLAMP_CUR(fDefenseType[18]);
    CLAMP_CUR(fDefenseType[19]);
    CLAMP_CUR(fDefense);
    CLAMP_CUR(fSpeedRunning);
    CLAMP_CUR(fSpeedFlying);
    CLAMP_CUR(fSpeedSwimming);
    CLAMP_CUR(fSpeedJumping);
    CLAMP_CUR(fJumpHeight);
    CLAMP_CUR(fMovementControl);
    CLAMP_CUR(fMovementFriction);
    CLAMP_CUR(fStealth);
    CLAMP_CUR(fStealthRadius);
    CLAMP_CUR(fStealthRadiusPlayer);
    CLAMP_CUR(fPerceptionRadius);
    CLAMP_CUR(fRegeneration);
    CLAMP_CUR(fRecovery);
    CLAMP_CUR(fInsightRecovery);
    CLAMP_CUR(fThreatLevel);
    CLAMP_CUR(fTaunt);
    CLAMP_CUR(fPlacate);
    CLAMP_CUR(fConfused);
    CLAMP_CUR(fAfraid);
    CLAMP_CUR(fTerrorized);
    CLAMP_CUR(fHeld);
    CLAMP_CUR(fImmobilized);
    CLAMP_CUR(fStunned);
    CLAMP_CUR(fSleep);
    CLAMP_CUR(fFly);
    CLAMP_CUR(fJumppack);
    CLAMP_CUR(fTeleport);
    CLAMP_CUR(fUntouchable);
    CLAMP_CUR(fIntangible);
    CLAMP_CUR(fOnlyAffectsSelf);
    CLAMP_CUR(fExperienceGain);
    CLAMP_CUR(fInfluenceGain);
    CLAMP_CUR(fPrestigeGain);
    CLAMP_CUR(fNullBool);
    CLAMP_CUR(fKnockup);
    CLAMP_CUR(fKnockback);
    CLAMP_CUR(fRepel);
    CLAMP_CUR(fAccuracy);
    CLAMP_CUR(fRadius);
    CLAMP_CUR(fArc);
    CLAMP_CUR(fRange);
    CLAMP_CUR(fTimeToActivate);
    CLAMP_CUR(fRechargeTime);
    CLAMP_CUR(fInterruptTime);
    CLAMP_CUR(fEnduranceDiscount);
    CLAMP_CUR(fInsightDiscount);
    CLAMP_CUR(fMeter);
    CLAMP_CUR(fElusivity[0]);
    CLAMP_CUR(fElusivity[1]);
    CLAMP_CUR(fElusivity[2]);
    CLAMP_CUR(fElusivity[3]);
    CLAMP_CUR(fElusivity[4]);
    CLAMP_CUR(fElusivity[5]);
    CLAMP_CUR(fElusivity[6]);
    CLAMP_CUR(fElusivity[7]);
    CLAMP_CUR(fElusivity[8]);
    CLAMP_CUR(fElusivity[9]);
    CLAMP_CUR(fElusivity[10]);
    CLAMP_CUR(fElusivity[11]);
    CLAMP_CUR(fElusivity[12]);
    CLAMP_CUR(fElusivity[13]);
    CLAMP_CUR(fElusivity[14]);
    CLAMP_CUR(fElusivity[15]);
    CLAMP_CUR(fElusivity[16]);
    CLAMP_CUR(fElusivity[17]);
    CLAMP_CUR(fElusivity[18]);
    CLAMP_CUR(fElusivity[19]);
    CLAMP_CUR(fElusivityBase);

    // END Generic('CLAMP_CUR')

    // Characters in the Praetorian tutorial are a special case because
    // their health should never go above 50% of the max. This gets turned
    // off by a special reward.
    if(p->bHalfMaxHealth)
    {
        CLAMPF32_IN_PLACE(&pattr->fHitPoints, pattrMin->fHitPoints, pattrMax->fHitPoints * 0.5f);
    }
}

/**********************************************************************func*
 * printDiffFloat
 *
 */
static char *printDiffFloat(char *curPos, float f, float fTest, float fOrig)
{
    if(fTest<fOrig)
    {
        curPos+=sprintf(curPos, "^7");
    }
    else if(fTest>fOrig)
    {
        curPos+=sprintf(curPos, "^2");
    }
    else
    {
        curPos+=sprintf(curPos, "^4");
    }
    curPos+=sprintf(curPos, "%6.2f^0", f);\

    return curPos;
}

/**********************************************************************func*
 * DumpAttribs
 *
 */
char *DumpAttribs(Character *pchar, char *curPos)
{
    #define PRINT_DIFF_FLOAT(loc, zz, xx, yy) curPos+=sprintf(curPos, "^t" #loc "t\t"); curPos=printDiffFloat(curPos, zz, xx, yy);
    #define PRINT_DIFF_MARKER(loc) curPos+=sprintf(curPos, "^t" #loc "t\t"); curPos+=sprintf(curPos, "   -^0");

    #define DUMP_ATTR_ALWAYS(xx) \
            curPos+=sprintf(curPos, "%s:", dbg_AttribName(offsetof(CharacterAttributes, f##xx), #xx));                            \
            PRINT_DIFF_FLOAT(110, pchar->attrCur.f##xx,pchar->attrMod.f##xx,g_attrModBase.f##xx);                          \
            PRINT_DIFF_FLOAT(160, pchar->attrMax.f##xx,pchar->attrMax.f##xx,pchar->pclass->pattrMax[pchar->iLevel].f##xx); \
            PRINT_DIFF_FLOAT(210, pchar->attrStrength.f##xx,pchar->attrStrength.f##xx,1.0f);                   \
            PRINT_DIFF_FLOAT(260, pchar->attrResistance.f##xx,pchar->attrResistance.f##xx,0.0f);             \
            curPos+=sprintf(curPos, "\n");

    #define DUMP_ATTR(xx) \
        if(pchar->attrMod.f##xx != g_attrModBase.f##xx \
            || pchar->attrStrength.f##xx != 1.0f \
            || pchar->attrResistance.f##xx != 0.0f) { \
            curPos+=sprintf(curPos, "%s:", dbg_AttribName(offsetof(CharacterAttributes, f##xx), #xx));                            \
            PRINT_DIFF_FLOAT(110, pchar->attrCur.f##xx,pchar->attrMod.f##xx,g_attrModBase.f##xx);                          \
            PRINT_DIFF_FLOAT(160, pchar->attrMax.f##xx,pchar->attrMax.f##xx,pchar->pclass->pattrMax[pchar->iLevel].f##xx); \
            PRINT_DIFF_FLOAT(210, pchar->attrStrength.f##xx,pchar->attrStrength.f##xx,1.0f);                   \
            PRINT_DIFF_FLOAT(260, pchar->attrResistance.f##xx,pchar->attrResistance.f##xx,0.0f);             \
            curPos+=sprintf(curPos, "\n");                                                                                 \
        }

#define DUMP_ATTR_MAG(xx) \
            if(pchar->attrMod.f##xx != g_attrModBase.f##xx \
            || pchar->attrStrength.f##xx != 1.0f \
            || pchar->attrResistance.f##xx != 0.0f) { \
            curPos+=sprintf(curPos, "%s:", dbg_AttribName(offsetof(CharacterAttributes, f##xx), #xx));                            \
            PRINT_DIFF_FLOAT(110, pchar->attrMod.f##xx,pchar->attrMod.f##xx,g_attrModBase.f##xx);                          \
            PRINT_DIFF_FLOAT(160, pchar->attrMax.f##xx,pchar->attrMax.f##xx,pchar->pclass->pattrMax[pchar->iLevel].f##xx); \
            PRINT_DIFF_FLOAT(210, pchar->attrStrength.f##xx,pchar->attrStrength.f##xx,1.0f);                   \
            PRINT_DIFF_FLOAT(260, pchar->attrResistance.f##xx,pchar->attrResistance.f##xx,0.0f);             \
            curPos+=sprintf(curPos, "\n");                                                                                 \
        }

    #define DUMP_ATTR_NO_CUR(xx) \
        if(pchar->attrMod.f##xx != g_attrModBase.f##xx \
            || pchar->attrStrength.f##xx != 1.0f \
            || pchar->attrResistance.f##xx != 0.0f) { \
            curPos+=sprintf(curPos, "%s:", dbg_AttribName(offsetof(CharacterAttributes, f##xx), #xx));                            \
            PRINT_DIFF_MARKER(110);                                                                                        \
            PRINT_DIFF_FLOAT(160, pchar->attrMax.f##xx,pchar->attrMax.f##xx,pchar->pclass->pattrMax[pchar->iLevel].f##xx); \
            PRINT_DIFF_FLOAT(210, pchar->attrStrength.f##xx,pchar->attrStrength.f##xx,1.0f);                   \
            PRINT_DIFF_FLOAT(260, pchar->attrResistance.f##xx,pchar->attrResistance.f##xx,0.0f);             \
            curPos+=sprintf(curPos, "\n");                                                                                 \
        }

    #define DUMP_ATTR_STR_RES(xx) \
        if(pchar->attrMod.f##xx != g_attrModBase.f##xx \
            || pchar->attrStrength.f##xx != 1.0f \
            || pchar->attrResistance.f##xx != 0.0f) { \
            curPos+=sprintf(curPos, "%s:", dbg_AttribName(offsetof(CharacterAttributes, f##xx), #xx));                \
            PRINT_DIFF_MARKER(110);                                                                            \
            PRINT_DIFF_MARKER(160);                                                                            \
            PRINT_DIFF_FLOAT(210, pchar->attrStrength.f##xx,pchar->attrStrength.f##xx,1.0f);       \
            PRINT_DIFF_FLOAT(260, pchar->attrResistance.f##xx,pchar->attrResistance.f##xx,0.0f); \
            curPos+=sprintf(curPos, "\n");                                                                     \
        }

    curPos+=sprintf(curPos, "Attrib:^t110t\tCur^t160t\tMax^t210t\tStr^t260t\tRes^0\n");

    // START DumpAttribs
    DUMP_ATTR_NO_CUR(DamageType[0]);
    DUMP_ATTR_NO_CUR(DamageType[1]);
    DUMP_ATTR_NO_CUR(DamageType[2]);
    DUMP_ATTR_NO_CUR(DamageType[3]);
    DUMP_ATTR_NO_CUR(DamageType[4]);
    DUMP_ATTR_NO_CUR(DamageType[5]);
    DUMP_ATTR_NO_CUR(DamageType[6]);
    DUMP_ATTR_NO_CUR(DamageType[7]);
    DUMP_ATTR_NO_CUR(DamageType[8]);
    DUMP_ATTR_NO_CUR(DamageType[9]);
    DUMP_ATTR_NO_CUR(DamageType[10]);
    DUMP_ATTR_NO_CUR(DamageType[11]);
    DUMP_ATTR_NO_CUR(DamageType[12]);
    DUMP_ATTR_NO_CUR(DamageType[13]);
    DUMP_ATTR_NO_CUR(DamageType[14]);
    DUMP_ATTR_NO_CUR(DamageType[15]);
    DUMP_ATTR_NO_CUR(DamageType[16]);
    DUMP_ATTR_NO_CUR(DamageType[17]);
    DUMP_ATTR_NO_CUR(DamageType[18]);
    DUMP_ATTR_NO_CUR(DamageType[19]);
    DUMP_ATTR_ALWAYS(HitPoints);
    DUMP_ATTR_ALWAYS(Absorb);
    DUMP_ATTR_ALWAYS(Endurance);
    DUMP_ATTR_ALWAYS(Insight);
    DUMP_ATTR_ALWAYS(Rage);
    DUMP_ATTR(ToHit);
    DUMP_ATTR(DefenseType[0]);
    DUMP_ATTR(DefenseType[1]);
    DUMP_ATTR(DefenseType[2]);
    DUMP_ATTR(DefenseType[3]);
    DUMP_ATTR(DefenseType[4]);
    DUMP_ATTR(DefenseType[5]);
    DUMP_ATTR(DefenseType[6]);
    DUMP_ATTR(DefenseType[7]);
    DUMP_ATTR(DefenseType[8]);
    DUMP_ATTR(DefenseType[9]);
    DUMP_ATTR(DefenseType[10]);
    DUMP_ATTR(DefenseType[11]);
    DUMP_ATTR(DefenseType[12]);
    DUMP_ATTR(DefenseType[13]);
    DUMP_ATTR(DefenseType[14]);
    DUMP_ATTR(DefenseType[15]);
    DUMP_ATTR(DefenseType[16]);
    DUMP_ATTR(DefenseType[17]);
    DUMP_ATTR(DefenseType[18]);
    DUMP_ATTR(DefenseType[19]);
    DUMP_ATTR(Defense);
    DUMP_ATTR(SpeedRunning);
    DUMP_ATTR(SpeedFlying);
    DUMP_ATTR(SpeedSwimming);
    DUMP_ATTR(SpeedJumping);
    DUMP_ATTR(JumpHeight);
    DUMP_ATTR(MovementControl);
    DUMP_ATTR(MovementFriction);
    DUMP_ATTR(Stealth);
    DUMP_ATTR(StealthRadius);
    DUMP_ATTR(StealthRadiusPlayer);
    DUMP_ATTR(PerceptionRadius);
    DUMP_ATTR(Regeneration);
    DUMP_ATTR(Recovery);
    DUMP_ATTR(InsightRecovery);
    DUMP_ATTR(ThreatLevel);

    DUMP_ATTR_MAG(Taunt);
    DUMP_ATTR_MAG(Placate);
    DUMP_ATTR_MAG(Confused);
    DUMP_ATTR_MAG(Afraid);
    DUMP_ATTR_MAG(Terrorized);
    DUMP_ATTR_MAG(Held);
    DUMP_ATTR_MAG(Immobilized);
    DUMP_ATTR_MAG(Stunned);
    DUMP_ATTR_MAG(Sleep);
    DUMP_ATTR_MAG(Teleport);
    DUMP_ATTR_MAG(Knockup);
    DUMP_ATTR_MAG(Knockback);
    DUMP_ATTR_MAG(Repel);

    DUMP_ATTR(Jumppack);
    DUMP_ATTR(Untouchable);
    DUMP_ATTR(Intangible);
    DUMP_ATTR(Fly);
    DUMP_ATTR(OnlyAffectsSelf);
    DUMP_ATTR(ExperienceGain);
    DUMP_ATTR(InfluenceGain);
    DUMP_ATTR(PrestigeGain);
    DUMP_ATTR(NullBool);

    DUMP_ATTR_STR_RES(Accuracy);
    DUMP_ATTR_STR_RES(Radius);
    DUMP_ATTR_STR_RES(Arc);
    DUMP_ATTR_STR_RES(Range);
    DUMP_ATTR_STR_RES(TimeToActivate);
    DUMP_ATTR_STR_RES(RechargeTime);
    DUMP_ATTR_STR_RES(InterruptTime);
    DUMP_ATTR_STR_RES(EnduranceDiscount);
    DUMP_ATTR(Meter);
    // END DumpAttribs

    return curPos;
}

/**********************************************************************func*
 * DumpStrAttribs
 *
 */
char *DumpStrAttribs(Character *p, CharacterAttributes *pattrStrength, char *curPos)
{
    #define PRINT_DIFF_FLOAT(loc, zz, xx, yy) curPos+=sprintf(curPos, "^t" #loc "t\t"); curPos=printDiffFloat(curPos, zz, xx, yy);
    #define PRINT_DIFF_MARKER(loc) curPos+=sprintf(curPos, "^t" #loc "t\t"); curPos+=sprintf(curPos, "   -^0");

    #define DUMP_ATTR_STR(xx) \
        if(pattrStrength->f##xx != 1.0f) { \
            curPos+=sprintf(curPos, "%s:", dbg_AttribName(offsetof(CharacterAttributes, f##xx), #xx)); \
            PRINT_DIFF_MARKER(110);                                                                    \
            PRINT_DIFF_FLOAT(160, p->pclass->pattrStrengthMax[p->iCombatLevel].f##xx, p->pclass->pattrStrengthMax[p->iCombatLevel].f##xx,p->pclass->pattrStrengthMax[p->iCombatLevel].f##xx); \
            PRINT_DIFF_FLOAT(210, pattrStrength->f##xx,pattrStrength->f##xx,1.0f);         \
            PRINT_DIFF_MARKER(260);                                                                    \
            curPos+=sprintf(curPos, "\n");                                                             \
        }

    curPos+=sprintf(curPos, "Attrib:^t110t\tCur^t160t\tMax^t210t\tStr^t260t\tRes^0\n");

    // START DumpStrAttribs
    DUMP_ATTR_STR(DamageType[0]);
    DUMP_ATTR_STR(DamageType[1]);
    DUMP_ATTR_STR(DamageType[2]);
    DUMP_ATTR_STR(DamageType[3]);
    DUMP_ATTR_STR(DamageType[4]);
    DUMP_ATTR_STR(DamageType[5]);
    DUMP_ATTR_STR(DamageType[6]);
    DUMP_ATTR_STR(DamageType[7]);
    DUMP_ATTR_STR(DamageType[8]);
    DUMP_ATTR_STR(DamageType[9]);
    DUMP_ATTR_STR(DamageType[10]);
    DUMP_ATTR_STR(DamageType[11]);
    DUMP_ATTR_STR(DamageType[12]);
    DUMP_ATTR_STR(DamageType[13]);
    DUMP_ATTR_STR(DamageType[14]);
    DUMP_ATTR_STR(DamageType[15]);
    DUMP_ATTR_STR(DamageType[16]);
    DUMP_ATTR_STR(DamageType[17]);
    DUMP_ATTR_STR(DamageType[18]);
    DUMP_ATTR_STR(DamageType[19]);
    DUMP_ATTR_STR(HitPoints);
    DUMP_ATTR_STR(Absorb);
    DUMP_ATTR_STR(Endurance);
    DUMP_ATTR_STR(Insight);
    DUMP_ATTR_STR(Rage);
    DUMP_ATTR_STR(ToHit);
    DUMP_ATTR_STR(DefenseType[0]);
    DUMP_ATTR_STR(DefenseType[1]);
    DUMP_ATTR_STR(DefenseType[2]);
    DUMP_ATTR_STR(DefenseType[3]);
    DUMP_ATTR_STR(DefenseType[4]);
    DUMP_ATTR_STR(DefenseType[5]);
    DUMP_ATTR_STR(DefenseType[6]);
    DUMP_ATTR_STR(DefenseType[7]);
    DUMP_ATTR_STR(DefenseType[8]);
    DUMP_ATTR_STR(DefenseType[9]);
    DUMP_ATTR_STR(DefenseType[10]);
    DUMP_ATTR_STR(DefenseType[11]);
    DUMP_ATTR_STR(DefenseType[12]);
    DUMP_ATTR_STR(DefenseType[13]);
    DUMP_ATTR_STR(DefenseType[14]);
    DUMP_ATTR_STR(DefenseType[15]);
    DUMP_ATTR_STR(DefenseType[16]);
    DUMP_ATTR_STR(DefenseType[17]);
    DUMP_ATTR_STR(DefenseType[18]);
    DUMP_ATTR_STR(DefenseType[19]);
    DUMP_ATTR_STR(Defense);
    DUMP_ATTR_STR(SpeedRunning);
    DUMP_ATTR_STR(SpeedFlying);
    DUMP_ATTR_STR(SpeedSwimming);
    DUMP_ATTR_STR(SpeedJumping);
    DUMP_ATTR_STR(JumpHeight);
    DUMP_ATTR_STR(MovementControl);
    DUMP_ATTR_STR(MovementFriction);
    DUMP_ATTR_STR(Stealth);
    DUMP_ATTR_STR(StealthRadius);
    DUMP_ATTR_STR(StealthRadiusPlayer);
    DUMP_ATTR_STR(PerceptionRadius);
    DUMP_ATTR_STR(Regeneration);
    DUMP_ATTR_STR(Recovery);
    DUMP_ATTR_STR(InsightRecovery);
    DUMP_ATTR_STR(ThreatLevel);
    DUMP_ATTR_STR(Taunt);
    DUMP_ATTR_STR(Placate);
    DUMP_ATTR_STR(Confused);
    DUMP_ATTR_STR(Afraid);
    DUMP_ATTR_STR(Terrorized);
    DUMP_ATTR_STR(Held);
    DUMP_ATTR_STR(Immobilized);
    DUMP_ATTR_STR(Stunned);
    DUMP_ATTR_STR(Sleep);
    DUMP_ATTR_STR(Fly);
    DUMP_ATTR_STR(Jumppack);
    DUMP_ATTR_STR(Teleport);
    DUMP_ATTR_STR(Untouchable);
    DUMP_ATTR_STR(Intangible);
    DUMP_ATTR_STR(OnlyAffectsSelf);
    DUMP_ATTR_STR(ExperienceGain);
    DUMP_ATTR_STR(InfluenceGain);
    DUMP_ATTR_STR(PrestigeGain);
    DUMP_ATTR_STR(NullBool);
    DUMP_ATTR_STR(Knockup);
    DUMP_ATTR_STR(Knockback);
    DUMP_ATTR_STR(Repel);
    DUMP_ATTR_STR(Accuracy);
    DUMP_ATTR_STR(Radius);
    DUMP_ATTR_STR(Arc);
    DUMP_ATTR_STR(Range);
    DUMP_ATTR_STR(TimeToActivate);
    DUMP_ATTR_STR(RechargeTime);
    DUMP_ATTR_STR(InterruptTime);
    DUMP_ATTR_STR(EnduranceDiscount);
    DUMP_ATTR_STR(Meter);
    // END DumpStrAttribs

    return curPos;
}


/**********************************************************************func*
 * CreateDimReturnsData
 *
 */
bool CreateDimReturnsData(void)
{
    int k;

    for(k=0; k<eaSize(&g_DimReturnList.ppSets); k ++)
    {
        int i;
        int j;
        const DimReturnSet *pSet = g_DimReturnList.ppSets[k];

        // Now put in the rest, and precalc the basis for each break.
        for(i=0; i<eaSize(&pSet->ppReturns); i++)
        {
            AttribDimReturnSet *p = pSet->ppReturns[i];

            float fBasis = 0.0f;
            float fLastStart = 0.0f;
            float fLastHandicap = 1.0f;
            for(j=0; j<eaSize(&p->pReturns); j++)
            {
                fBasis += fLastHandicap*(p->pReturns[j]->fStart-fLastStart);

                p->pReturns[j]->fBasis = fBasis;

                fLastStart = p->pReturns[j]->fStart;
                fLastHandicap = p->pReturns[j]->fHandicap;
            }
        }
    }

    return true;
}

/**********************************************************************func*
 * AggregateStrength
 *
 */
void AggregateStrength(Character *p, const BasePower *ppowBase, CharacterAttributes *pDest, CharacterAttributes *pBoosts)
{
    size_t offAttr;
    for(offAttr = 0; offAttr<sizeof(CharacterAttributes); offAttr += sizeof(float))
    {
        float *pfFinal;
        float *pfBoost = ATTRIB_GET_PTR(pBoosts, offAttr);
        const DimReturn ***pppDimReturns = ppowBase->apppDimReturns[offAttr/sizeof(float)];

#if CLIENT
        if(*pppDimReturns && game_state.enableBoostDiminishing)
#else
        if(*pppDimReturns && server_state.enableBoostDiminishing)
#endif
        {
            int iSize = eaSize(pppDimReturns);
            if(iSize>0 && (*pfBoost) > (*pppDimReturns)[0]->fStart)
            {
                int i;
                for(i=0; i<iSize; i++)
                {
                    if(i==iSize-1 || (*pfBoost) < (*pppDimReturns)[i+1]->fStart)
                    {
                        *pfBoost = (*pppDimReturns)[i]->fBasis + (*pfBoost - (*pppDimReturns)[i]->fStart)*(*pppDimReturns)[i]->fHandicap;
                        break;
                    }
                }
            }
        }

        pfFinal = ATTRIB_GET_PTR(pDest, offAttr); 
        *pfFinal += *pfBoost;

#if SERVER
        if (server_state.enablePowerDiminishing)
#elif CLIENT 
        if (game_state.enablePowerDiminishing)
#endif
        {
            float *pfInner = ATTRIB_GET_PTR(*p->pclass->ppattrDiminStr[kClassesDiminish_Inner], offAttr);
            float *pfOuter = ATTRIB_GET_PTR(*p->pclass->ppattrDiminStr[kClassesDiminish_Outer], offAttr);

            *pfFinal = attribDiminishValue((*pfFinal - 1.0f), *pfInner, *pfOuter) + 1.0f;
        }
    }
}




/* End of File */
