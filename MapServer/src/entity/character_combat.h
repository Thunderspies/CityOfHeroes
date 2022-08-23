/***************************************************************************
 *     Copyright (c) 2003-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#ifndef CHARACTER_COMBAT_H__
#define CHARACTER_COMBAT_H__

#include "entity/entityRef.h" // for EntityRef

typedef struct Character Character;
typedef struct BasePower BasePower;
typedef struct Power Power;
typedef struct DefTracker DefTracker;
typedef struct AttribMod AttribMod;

typedef struct HitLocation
{
    EntityRef erTarget;
    Vec3 vecHitLoc;
    F32 fDistChecked; // for bone collision, this is the smallest distance from src to dst
    bool bReached;
} HitLocation;

U32 nextInvokeId();

void MarkLargestModForPromotionOutOfSuppression(Character *p, AttribMod *pmod);
void PromoteModsOutOfSuppression(Character *p);

int character_combatGetLocalizedMessage(char *outputMessage, int bufferLength, Entity* e, int translateSourceName, int translateDestinationName, const char* messageID, ...);

bool character_ApplyPower(Character *pSrc, Power *ppow, EntityRef erTarget, const Vec3 vecTarget, unsigned int invokeID, int ignoreLocation );
bool character_ApplyPowerAtActivation(Character *pSrc, Power *ppow, EntityRef erTarget, const Vec3 vecTarget, unsigned int invokeID );
bool character_ApplyPowerAtDeactivation(Character *pSrc, Power *ppow, EntityRef erTarget, unsigned int invokeID );

// only call this when you're about to switch builds or something like that.
// As soon as character_UpdateEnabledState() gets called again, it will re-enable mods!
// THIS TRIGGERS ONDISABLED MODS!
void character_DisableAllPowers(Character *p);
// only call this when doing a character_Reset() or something very similar!
// This hacks everything up pretty raw.
// THIS DOES NOT TRIGGER ONDISABLED MODS!
void character_OverrideAllPowersToDisabled(Character *p);

void character_ManuallyEnablePower(Character *p, Power *ppow);
void character_ManuallyDisablePower(Character *p, Power *ppow);
void character_UpdateEnabledState(Character *p, Power *ppow);
void character_UpdateAllEnabledState(Character *p);

float character_PowerRange(Character *pSrc, Power *ppow);
float character_PowerRange2(Character *pSrc, Power *ppow);
bool character_EntUsesGeomColl(Entity *pent);
bool character_FindHitLoc(Entity *pSrc, Entity *pTarget, Vec3 loc);
bool character_LocationInRange(Character *pSrc, Vec3 vecTarget, Power *ppow);
bool character_LocationInRange2(Character *pSrc, Vec3 vecTarget, Power *ppow);
bool character_TargetIsPerceptible(Character *pSrc, Character *pTarget);
bool character_InRange(Character *pSrc, Character *pTarget, Power *ppow);
bool character_WithinDist(Entity *pSrc, Entity *pTarget, float fDist);
bool character_LocationIsReachable(const Vec3 vecSrc, const Vec3 vecTarget, float fRadius, DefTracker ** res_tracker);
bool character_CharacterIsReachable(Entity *entSrc, Entity *entTarget);
bool ValidateConeTarget(Character *pSrc, Entity *pent, const Vec3 vecTarget, Power *ppow, float *pfDistSqr, Vec3 vecTargetReachableOffset);
void getLocationReachableVecOffset( Entity * e, Vec3 vecRealLocVisOffset );

float character_TargetCollisionRadius(Entity *entSrc, Entity *entTarget);
    // Gets a collision radius the same way that character_InRange checks for hits
    // i.e. it uses geometry-based collision if need be

void character_ActivateInspiration(Character *pSrc, Character *pTarget, int icol, int irow);
    // Activates an inspiration. Allocates a Power for the purposes of
    // activation. This is eventually freed when the recharge timer dies out.
    // The source is where inspiration comes from, the target is entity (possible pet) using it

int character_ActivatePower(Character *p, EntityRef erTarget, const Vec3 loc, Power *ppow);
void character_ActivatePowerAtLocation(Character *p, Character *pTarget, Vec3 loc, Power *ppow);
int character_ActivatePowerOnCharacter(Character *p, Character *pTarget, Power *ppow);
    // Given a power pointer, attempts to activate it. It handles toggle,
    // auto, and click powers appropriately. "Activation" may not mean that
    // the power gets executed, only that it is considered for execution.
    // The power may not get executed because of the queue being full, or
    // being out of endurance, etc.

void character_ActivateAllAutoPowers(Character *p);
    // Looks through all the character's powers and activates all which
    // are marked as auto powers.

void character_InterruptMissionTeleport(Character *p);
int character_GetOnactivateInvokeID(Character *p, const BasePower *bpow);
void character_TurnOnActiveTogglePowers(Character *p);
    // Used when resuming a character. Reactivates all active toggle powers,
    // turning on visual effects, etc.

void character_ShutOffContinuingPower(Character *p, Power *ppow);

void character_ShutOffAllAutoPowers(Character *p);
void character_ShutOffAndRemoveAutoPower(Character *p, Power *ppow);

void character_ShutOffAllTogglePowers(Character *p);
void character_ShutOffTogglePowers_Hold(Character *p);
void character_ShutOffTogglePowers_Sleep(Character *p);
void character_ShutOffTogglePowers_Stun(Character *p);
void character_ShutOffTogglePower(Character *p, Power *ppow);
void character_ShutOffAllTogglePowersInSameGroup(Character *p, const BasePower *ppowBase);
void character_ShutOffAndRemoveTogglePower(Character *p, Power *ppow);

void character_ShutOffGlobalBoost(Character *p, Power *ppow);
void character_ShutOffAndRemoveGlobalBoost(Character *p, Power *ppow);

void character_CancelModsByBasePower(Character *p, const BasePower *ppowBase);

void character_RemoveAllMods(Character *p);
void character_RemoveModsAtDeath(Character *p);

void character_InterruptPower(Character *p, Character *pSrc, float fDelay, bool bAwaken);
void character_KillQueuedPower(Character *p);
void character_KillRetryPower(Character *p);
void character_KillCurrentPower(Character *p);

void character_AssistByName(Character *p, char *pchName);
void character_Unassist(Character *p);

bool character_AllowedToActivatePower(Character *p, Power *ppow, bool bReport);
bool character_IsNamedTogglePowerActive(Character *p, const char *name);

bool character_NeedPowerConfirm(Character *p, Power *ppow);
bool character_ConfirmPower(Character *p, unsigned int ui, bool bAccept);

bool character_IsInspirationSlotInUse(Character *p, int iCol, int iRow);

void character_BecomeCamera(Entity* e);
void character_UnCamera(Entity* e);

void character_ReleasePowerChain(Character *p, Power*ppowCurrentLink);

#endif /* #ifndef CHARACTER_COMBAT_H__ */

/* End of File */

