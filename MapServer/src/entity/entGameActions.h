/***************************************************************************
 *     Copyright (c) 2000-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#if (!defined(ENTGAMEACTIONS_H__)) || (defined(DAMAGEDECAY_PARSE_INFO_DEFINITIONS)&&!defined(DAMAGEDECAY_PARSE_INFO_DEFINED))
#define ENTGAMEACTIONS_H__

#include <utilitieslib/stdtypes.h> // for F32
#include "entity/entityRef.h"
#include "entity/entity_enum.h"

typedef enum SurfParamIndex SurfParamIndex;
typedef struct Entity Entity;
typedef struct OnClickCondition OnClickCondition;
typedef struct AIVars AIVars;
typedef enum DeathBy
{
    kDeathBy_HitPoints,
    kDeathBy_StatusPoints,
    kDeathBy_SkillPoints,

    kDeathBy_Count,
} DeathBy;

int entAlive( Entity *e );
void entDoFirstProcessStuff(Entity* e, AIVars* ai, int index);
void dieNow(Entity *victim, DeathBy eDeathBy, bool bAnimate, int iDelay);
void buildCloseEntList( Entity  *entToskip, Entity *centerEnt, F32 dist, int *list, int list_len, int include_centerEnt );
void setSpeed( Entity *e,SurfParamIndex surf_type, float fSpeedScale);
void setClientState(Entity  *e, int state );
void setFlying( Entity *e, int is_flying);
void setStunned(Entity *e, int is_stunned);
void setJumppack(Entity *e, int jumppack_on);
void setSpecialMovement(Entity *e, int attrib, int on);
void setNoPhysics(Entity *e, int no_physics_on);
void setHitStumble(Entity *e, int hit_stumble_on);
void setViewCutScene(Entity *e, int viewCutSceneOn);
void setSurfaceModifiers(Entity *e,SurfParamIndex surf_type,float traction, float friction, float bounce, float gravity);
void setJumpHeight(Entity* e, float height);
void setEntCollision(Entity *e, int set);
void setEntNoSelectable(Entity *e, int set);
void setNoJumpRepeat(Entity *e, int set);
void sendStats( Entity *attacker, Entity *victim, int stat_level );
void modStateBits( Entity *e, EntMode mode_num, int timer, int on );
int decModeTimer( Entity *e, EntMode mode_num );
void serveDamage( Entity *victim, Entity *attacker, int new_damage, float *loc, bool wasAbsorb);
void serveDamageMessage( Entity *victim, Entity *attacker, char *msg, float *loc, bool wasAbsorb);
void serveFloater(Entity *e, Entity *entShownOver, const char *pch, float fDelay, EFloaterStyle style, U32 color);
void servePlaySound(Entity *e, const char *filename, int channel, float volumeLevel);
void serveFadeSound(Entity *e, int channel, float fadeTime);
void serveFloatingInfo(Entity *e, Entity *entShownOver, char *pch);
void serveFloatingInfoDelayed(Entity *e, Entity *entShownOver, const char *pch, float fDelay);
void serveDesaturateInformation(Entity *e, float fDelay, float fCurrentValue, float fTargetValue, float fFadeTime);
void setInitialTeleportPosition(Entity * e, Vec3 targetPos, bool exact);
void setSayOnClick( Entity * e, const char * text );
void setRewardOnClick( Entity * e, const char * text );
void setSayOnKillHero( Entity * e, const char * text );
void addOnClickCondition( Entity *e, OnClickConditionType type, const char *condition, const char *say, const char *reward);
void clearallOnClickCondition( Entity *e );
void OnClickCondition_Destroy(void* condData);
bool OnClickCondition_Check(Entity *e, Entity *target, char **say, char **reward);
void doHitStumble( Entity * e, F32 iAmt );

//----------------------------------------------------------------------
// Entity Damage Tracker
//----------------------------------------------------------------------
// Tally of the amount of damage an entity received from each attacking player.
typedef struct DamageTracker
{
    float damage;    // Don't move this.  damageTrackerFindMaxDmg() is used on both DamageTracker && TeamDamageTracker
    float damageOriginal;
    int group_id;
    EntityRef ref;
    U32 timestamp;
} DamageTracker;
DamageTracker* damageTrackerCreate();
void damageTrackerDestroy(DamageTracker* dt);
void damageTrackerUpdate(Entity *victim, Entity *attacker, float new_damage);
void damageTrackerRegen(Entity *e, float fHealed);
float damageTrackerFindEnvironmentalShare(Entity *e);
DamageTracker* damageTrackerFindMaxDmg(DamageTracker** trackers);

typedef struct DamageDecayConfig
{
    int iDecayDelay;
    int iFullDiscardDelay;
    float fDiscardDamage;
    float fRegenFactor;
    int discardZeroDamagers;
} DamageDecayConfig;

extern SHARED_MEMORY DamageDecayConfig g_DamageDecayConfig;


#ifdef DAMAGEDECAY_PARSE_INFO_DEFINITIONS
TokenizerParseInfo ParseDamageDecayConfig[] =
{
    { "{",                TOK_START,    0 },
    { "DecayDelay",       TOK_INT(DamageDecayConfig, iDecayDelay, 0) },
    { "FullDiscardDelay", TOK_INT(DamageDecayConfig, iFullDiscardDelay, 0) },
    { "DiscardDamage",    TOK_F32(DamageDecayConfig, fDiscardDamage, 0)  },
    { "RegenFactor",      TOK_F32(DamageDecayConfig, fRegenFactor, 0) },
    { "DiscardZeroDamagers",       TOK_INT(DamageDecayConfig, discardZeroDamagers, 0) },
    { "}",                TOK_END,      0 },
    { "", 0, 0 }
};
#endif /* #ifdef DAMAGEDECAY_PARSE_INFO_DEFINITIONS */

#ifdef DAMAGEDECAY_PARSE_INFO_DEFINITIONS
#define DAMAGEDECAY_PARSE_INFO_DEFINED
#endif

#endif /* #ifndef ENTGAMEACTIONS_H__ */

/* End of File */
