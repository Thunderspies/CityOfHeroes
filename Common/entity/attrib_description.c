
#include <utilitieslib/utils/textparser.h>
#include "entity/attrib_description.h"
#include "entity/attrib_names.h"
#include <utilitieslib/network/netio.h>
#include <utilitieslib/network/net_packetutil.h>
#include <utilitieslib/network/net_packet.h>
#include <utilitieslib/components/earray.h>
#include "entity/entity.h"
#include "entity/character_base.h"
#include "entity/powers.h"
#include <utilitieslib/components/StringCache.h>
#include "attribmod.h"
#include "entity/character_net.h"
#include "gameComm/Npc.h"
#include "character_attribs.h"
#include "entity/character_level.h"
#include <utilitieslib/utils/error.h>
#include "entity/entPlayer.h"
#include "comm_game.h"
#include <utilitieslib/components/StashTable.h>
#include "gameComm/wdwbase.h"

extern TokenizerParseInfo ParseCharacterAspects[];

SERVER_SHARED_MEMORY AttribCategoryList g_AttribCategoryList = {0};

Entity * g_pAttribTarget = 0;

StaticDefineInt AttribTypeEnum[] =
{
    DEFINE_INT
    { "Cur", kAttribType_Cur },
    { "Str", kAttribType_Str },
    { "Res", kAttribType_Res },
    { "Max", kAttribType_Max },
    { "Mod", kAttribType_Mod },
    { "Special", kAttribType_Special },
    DEFINE_END
};

StaticDefineInt AttribStyleEnum[] =
{
    DEFINE_INT
    { "None",                kAttribStyle_None },
    { "Percent",            kAttribStyle_Percent },
    { "Magnitude",            kAttribStyle_Magnitude },
    { "Distance",            kAttribStyle_Distance },
    { "PercentMinus100",    kAttribStyle_PercentMinus100 },
    { "PerSecond",            kAttribStyle_PerSecond },
    { "Speed",                kAttribStyle_Speed },
    { "ResistanceDuration",    kAttribStyle_ResistanceDuration },
    { "Multiply",            kAttribStyle_Multiply },
    { "Integer",            kAttribStyle_Integer },
    { "EnduranceReduction",    kAttribStyle_EnduranceReduction },
    { "InversePercent",        kAttribStyle_InversePercent },
    DEFINE_END
};


TokenizerParseInfo ParseAttribDescription[] =
{
    { "{",                TOK_START,    0 },
    { "Name",            TOK_STRUCTPARAM | TOK_STRING(AttribDescription, pchName, 0),     },
    { "DisplayName",    TOK_STRING(AttribDescription, pchDisplayName, 0), },
    { "Tooltip",        TOK_STRING(AttribDescription, pchToolTip, 0), },
    { "Type",            TOK_INT(AttribDescription, eType, kAttribType_Cur), AttribTypeEnum, },
    { "Style",            TOK_INT(AttribDescription, eStyle, kAttribStyle_None), AttribStyleEnum, },
    { "Key",            TOK_INT(AttribDescription, iKey, 0), },
    { "Offset",            TOK_INT(AttribDescription, offAttrib, 0), },
    { "ShowBase",        TOK_BOOL(AttribDescription, bShowBase, 0), },
    { "}",                TOK_END,      0 },
    { "", 0, 0 }
};

TokenizerParseInfo ParseAttribCategory[] =
{
    { "{",            TOK_START,    0 },
    { "Name",        TOK_STRUCTPARAM | TOK_STRING(AttribCategory, pchDisplayName, 0), },
    { "Attrib",        TOK_STRUCT(AttribCategory, ppAttrib, ParseAttribDescription),    },
    { "}",            TOK_END,      0 },
    { "", 0, 0 }
};

TokenizerParseInfo ParseAttribCategoryList[] =
{
    { "AttribCategory", TOK_STRUCT(AttribCategoryList, ppCategories, ParseAttribCategory), },
    { "", 0, 0 }
};


char const* getParseName(TokenizerParseInfo atpi[] , int offset)
{
    int i = 0;
    while(atpi[i].name != NULL)
    {
        if(atpi[i].storeoffset == offset && atpi[i].name[0] != '{')
        {
            return atpi[i].name;
        }
        i++;
    }

    return NULL;
}



static StashTable stAttribDescription = 0;
 
void attribDescriptionCreateStash()
{
    int i,j;
    AttribDescription * pDesc;

    if(stAttribDescription)
        return;

    stAttribDescription = stashTableCreateInt(64);
    for( i = eaSize(&g_AttribCategoryList.ppCategories)-1; i >= 0; i-- )
    {
        for( j = eaSize(&g_AttribCategoryList.ppCategories[i]->ppAttrib)-1; j>=0; j--)
        {
            pDesc = g_AttribCategoryList.ppCategories[i]->ppAttrib[j];
            stashIntAddPointer(stAttribDescription, (sizeof(CharacterAttributes)*pDesc->eType)+pDesc->offAttrib, pDesc, 0);
        }
    }
}

AttribDescription * attribDescriptionGet( int hash )
{
    AttribDescription *pDesc;
    if(stashIntFindPointer(stAttribDescription, hash, &pDesc))
        return pDesc;

    return NULL;
}

static char **ppAttribNames;

void attribNameSetAll()
{
    eaSetSize(&ppAttribNames, sizeof(CharacterAttributes)/4 );
    
#define SET_MOD_NAME(mod) ppAttribNames[offsetof(CharacterAttributes, f##mod)/4] = strdup(#mod);

    SET_MOD_NAME(DamageType[0]);
    SET_MOD_NAME(DamageType[1]);
    SET_MOD_NAME(DamageType[2]);
    SET_MOD_NAME(DamageType[3]);
    SET_MOD_NAME(DamageType[4]);
    SET_MOD_NAME(DamageType[5]);
    SET_MOD_NAME(DamageType[6]);
    SET_MOD_NAME(DamageType[7]);
    SET_MOD_NAME(DamageType[8]);
    SET_MOD_NAME(DamageType[9]);
    SET_MOD_NAME(DamageType[10]);
    SET_MOD_NAME(DamageType[11]);
    SET_MOD_NAME(DamageType[12]);
    SET_MOD_NAME(DamageType[13]);
    SET_MOD_NAME(DamageType[14]);
    SET_MOD_NAME(DamageType[15]);
    SET_MOD_NAME(DamageType[16]);
    SET_MOD_NAME(DamageType[17]);
    SET_MOD_NAME(DamageType[18]);
    SET_MOD_NAME(DamageType[19]);
    SET_MOD_NAME(HitPoints);
    SET_MOD_NAME(Absorb);
    SET_MOD_NAME(Endurance);
    SET_MOD_NAME(Insight);
    SET_MOD_NAME(Rage);
    SET_MOD_NAME(ToHit);
    SET_MOD_NAME(DefenseType[0]);
    SET_MOD_NAME(DefenseType[1]);
    SET_MOD_NAME(DefenseType[2]);
    SET_MOD_NAME(DefenseType[3]);
    SET_MOD_NAME(DefenseType[4]);
    SET_MOD_NAME(DefenseType[5]);
    SET_MOD_NAME(DefenseType[6]);
    SET_MOD_NAME(DefenseType[7]);
    SET_MOD_NAME(DefenseType[8]);
    SET_MOD_NAME(DefenseType[9]);
    SET_MOD_NAME(DefenseType[10]);
    SET_MOD_NAME(DefenseType[11]);
    SET_MOD_NAME(DefenseType[12]);
    SET_MOD_NAME(DefenseType[13]);
    SET_MOD_NAME(DefenseType[14]);
    SET_MOD_NAME(DefenseType[15]);
    SET_MOD_NAME(DefenseType[16]);
    SET_MOD_NAME(DefenseType[17]);
    SET_MOD_NAME(DefenseType[18]);
    SET_MOD_NAME(DefenseType[19]);
    SET_MOD_NAME(Defense);
    SET_MOD_NAME(SpeedRunning);
    SET_MOD_NAME(SpeedFlying);
    SET_MOD_NAME(SpeedSwimming);
    SET_MOD_NAME(SpeedJumping);
    SET_MOD_NAME(JumpHeight);
    SET_MOD_NAME(MovementControl);
    SET_MOD_NAME(MovementFriction);
    SET_MOD_NAME(Stealth);
    SET_MOD_NAME(StealthRadius);
    SET_MOD_NAME(StealthRadiusPlayer);
    SET_MOD_NAME(PerceptionRadius);
    SET_MOD_NAME(Regeneration);
    SET_MOD_NAME(Recovery);
    SET_MOD_NAME(InsightRecovery);
    SET_MOD_NAME(ThreatLevel);
    SET_MOD_NAME(Taunt);
    SET_MOD_NAME(Placate);
    SET_MOD_NAME(Confused);
    SET_MOD_NAME(Afraid);
    SET_MOD_NAME(Terrorized);
    SET_MOD_NAME(Held);
    SET_MOD_NAME(Immobilized);
    SET_MOD_NAME(Stunned);
    SET_MOD_NAME(Sleep);
    SET_MOD_NAME(Fly);
    SET_MOD_NAME(Jumppack);
    SET_MOD_NAME(Teleport);
    SET_MOD_NAME(Untouchable);
    SET_MOD_NAME(Intangible);
    SET_MOD_NAME(OnlyAffectsSelf);
    SET_MOD_NAME(ExperienceGain);
    SET_MOD_NAME(InfluenceGain);
    SET_MOD_NAME(PrestigeGain);
    SET_MOD_NAME(NullBool);
    SET_MOD_NAME(Knockup);
    SET_MOD_NAME(Knockback);
    SET_MOD_NAME(Repel);
    SET_MOD_NAME(Accuracy);
    SET_MOD_NAME(Radius);
    SET_MOD_NAME(Arc);
    SET_MOD_NAME(Range);
    SET_MOD_NAME(TimeToActivate);
    SET_MOD_NAME(RechargeTime);
    SET_MOD_NAME(InterruptTime);
    SET_MOD_NAME(EnduranceDiscount);
}

char * getAttribNameForTranslation( int offset )
{
    static char buffer[5][128];
    static int rotor;

    rotor++;
    sprintf( buffer[rotor%5], "Attr%s", getAttribName(offset) );
    return buffer[rotor%5];
}

char * getAttribName( int offset )
{
    if( offset >= 0 && offset < sizeof(CharacterAttributes) )
        return ppAttribNames[offset/4];
    else
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
        else if(offset==kSpecialAttrib_XPDebt)
        {
            return "(xp debt)";
        }
        else if(offset==kSpecialAttrib_DropToggles)
        {
            return "DropToggles";
        }
        else if(offset==kSpecialAttrib_GrantPower)
        {
            return "GrantPower";
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
        else if(offset==kSpecialAttrib_DesignerStatus)
        {
            return "DesignerStatus";
        }
        else if(offset==kSpecialAttrib_ExclusiveVisionPhase)
        {
            return "ExclusiveVisionPhase";
        }
        else if(offset==kSpecialAttrib_HoverBoard)
        {
            return "HoverBoard";
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

    return "Unknown";
}

bool attribDescriptionPreprocess(TokenizerParseInfo *pti, void* structptr)
{
    int i,j;
    AttribDescription * pDesc;

#define GET_MOD_OFFSET(mod) if(stricmp(pDesc->pchName,#mod)==0){ pDesc->offAttrib = (int)offsetof(CharacterAttributes, f##mod); continue;}

    for( i = eaSize(&g_AttribCategoryList.ppCategories)-1; i >= 0; i-- )
    {
        for( j = eaSize(&g_AttribCategoryList.ppCategories[i]->ppAttrib)-1; j>=0; j--)
        {
            pDesc = g_AttribCategoryList.ppCategories[i]->ppAttrib[j];
            GET_MOD_OFFSET(DamageType[0]);
            GET_MOD_OFFSET(DamageType[1]);
            GET_MOD_OFFSET(DamageType[2]);
            GET_MOD_OFFSET(DamageType[3]);
            GET_MOD_OFFSET(DamageType[4]);
            GET_MOD_OFFSET(DamageType[5]);
            GET_MOD_OFFSET(DamageType[6]);
            GET_MOD_OFFSET(DamageType[7]);
            GET_MOD_OFFSET(DamageType[8]);
            GET_MOD_OFFSET(DamageType[9]);
            GET_MOD_OFFSET(DamageType[10]);
            GET_MOD_OFFSET(DamageType[11]);
            GET_MOD_OFFSET(DamageType[12]);
            GET_MOD_OFFSET(DamageType[13]);
            GET_MOD_OFFSET(DamageType[14]);
            GET_MOD_OFFSET(DamageType[15]);
            GET_MOD_OFFSET(DamageType[16]);
            GET_MOD_OFFSET(DamageType[17]);
            GET_MOD_OFFSET(DamageType[18]);
            GET_MOD_OFFSET(DamageType[19]);
            GET_MOD_OFFSET(HitPoints);
            GET_MOD_OFFSET(Absorb);
            GET_MOD_OFFSET(Endurance);
            GET_MOD_OFFSET(Insight);
            GET_MOD_OFFSET(Rage);
            GET_MOD_OFFSET(ToHit);
            GET_MOD_OFFSET(DefenseType[0]);
            GET_MOD_OFFSET(DefenseType[1]);
            GET_MOD_OFFSET(DefenseType[2]);
            GET_MOD_OFFSET(DefenseType[3]);
            GET_MOD_OFFSET(DefenseType[4]);
            GET_MOD_OFFSET(DefenseType[5]);
            GET_MOD_OFFSET(DefenseType[6]);
            GET_MOD_OFFSET(DefenseType[7]);
            GET_MOD_OFFSET(DefenseType[8]);
            GET_MOD_OFFSET(DefenseType[9]);
            GET_MOD_OFFSET(DefenseType[10]);
            GET_MOD_OFFSET(DefenseType[11]);
            GET_MOD_OFFSET(DefenseType[12]);
            GET_MOD_OFFSET(DefenseType[13]);
            GET_MOD_OFFSET(DefenseType[14]);
            GET_MOD_OFFSET(DefenseType[15]);
            GET_MOD_OFFSET(DefenseType[16]);
            GET_MOD_OFFSET(DefenseType[17]);
            GET_MOD_OFFSET(DefenseType[18]);
            GET_MOD_OFFSET(DefenseType[19]);
            GET_MOD_OFFSET(Defense);
            GET_MOD_OFFSET(SpeedRunning);
            GET_MOD_OFFSET(SpeedFlying);
            GET_MOD_OFFSET(SpeedSwimming);
            GET_MOD_OFFSET(SpeedJumping);
            GET_MOD_OFFSET(JumpHeight);
            GET_MOD_OFFSET(MovementControl);
            GET_MOD_OFFSET(MovementFriction);
            GET_MOD_OFFSET(Stealth);
            GET_MOD_OFFSET(StealthRadius);
            GET_MOD_OFFSET(StealthRadiusPlayer);
            GET_MOD_OFFSET(PerceptionRadius);
            GET_MOD_OFFSET(Regeneration);
            GET_MOD_OFFSET(Recovery);
            GET_MOD_OFFSET(InsightRecovery);
            GET_MOD_OFFSET(ThreatLevel);
            GET_MOD_OFFSET(Taunt);
            GET_MOD_OFFSET(Placate);
            GET_MOD_OFFSET(Confused);
            GET_MOD_OFFSET(Afraid);
            GET_MOD_OFFSET(Terrorized);
            GET_MOD_OFFSET(Held);
            GET_MOD_OFFSET(Immobilized);
            GET_MOD_OFFSET(Stunned);
            GET_MOD_OFFSET(Sleep);
            GET_MOD_OFFSET(Fly);
            GET_MOD_OFFSET(Jumppack);
            GET_MOD_OFFSET(Teleport);
            GET_MOD_OFFSET(Untouchable);
            GET_MOD_OFFSET(Intangible);
            GET_MOD_OFFSET(OnlyAffectsSelf);
            GET_MOD_OFFSET(ExperienceGain);
            GET_MOD_OFFSET(InfluenceGain);
            GET_MOD_OFFSET(PrestigeGain);
            GET_MOD_OFFSET(NullBool);
            GET_MOD_OFFSET(Knockup);
            GET_MOD_OFFSET(Knockback);
            GET_MOD_OFFSET(Repel);
            GET_MOD_OFFSET(Accuracy);
            GET_MOD_OFFSET(Radius);
            GET_MOD_OFFSET(Arc);
            GET_MOD_OFFSET(Range);
            GET_MOD_OFFSET(TimeToActivate);
            GET_MOD_OFFSET(RechargeTime);
            GET_MOD_OFFSET(InterruptTime);
            GET_MOD_OFFSET(EnduranceDiscount);

            if( pDesc->eType == kAttribType_Special )
                pDesc->offAttrib = pDesc->iKey;
        }
    }

    return true;
}

int modGetBuffHash( const AttribModTemplate * t )
{
    AttribDescription *pDesc;
    AttribType eType;

    if( t->offAttrib >= sizeof(CharacterAttributes) || offsetof(CharacterAttribSet, pattrAbsolute) == t->offAspect )
        return -1;

    if( offsetof(CharacterAttribSet, pattrMod) == t->offAspect )
    {
        if(!stashIntFindPointer(stAttribDescription, (int)t->offAttrib, &pDesc))
            eType = kAttribType_Mod;
        else
            return (int)t->offAttrib;
    }
    else if ( offsetof(CharacterAttribSet, pattrStrength) == t->offAspect )
        eType = kAttribType_Str;
    else if ( offsetof(CharacterAttribSet, pattrResistance) == t->offAspect )
        eType = kAttribType_Res;
    else if ( offsetof(CharacterAttribSet, pattrMax) == t->offAspect )
        eType = kAttribType_Max;

    if(!stashIntFindPointer(stAttribDescription, (int)(eType * sizeof(CharacterAttributes) + t->offAttrib), &pDesc))
        return -1;
    else
        return (int)(eType * sizeof(CharacterAttributes) + t->offAttrib);
}

#if SERVER

#include "svr/svr_base.h"
#include "entity/entserver.h"
#include "cmdparse/cmdserver.h"

F32 attrib_GetVal( Character *pchar, AttribDescription *pDesc, int * buffOrDebuff ) // this will be slow
{
    F32 fOrig, fTest, fVal, fMax=0;
    *buffOrDebuff = 0;

    if (!pchar)
    {
        return 0;
    }
    switch(pDesc->eType)
    {
        xcase kAttribType_Cur:
        {    
            fTest = *(ATTRIB_GET_PTR(&pchar->attrMod, pDesc->offAttrib));
            fOrig = *(ATTRIB_GET_PTR(&g_attrModBase, pDesc->offAttrib));
            fVal = *(ATTRIB_GET_PTR(&pchar->attrCur, pDesc->offAttrib));
            fMax = *(ATTRIB_GET_PTR(&pchar->attrMax, pDesc->offAttrib));
        }
        xcase kAttribType_Mod:
        {
            fTest = *(ATTRIB_GET_PTR(&pchar->attrMod, pDesc->offAttrib));
            fOrig = *(ATTRIB_GET_PTR(&g_attrModBase, pDesc->offAttrib));
            fVal = *(ATTRIB_GET_PTR(&pchar->attrMod, pDesc->offAttrib));
        }
        xcase kAttribType_Str: 
        {
            fTest = *(ATTRIB_GET_PTR(&pchar->attrStrength, pDesc->offAttrib));
            fOrig = 1.f;
            fVal = *(ATTRIB_GET_PTR(&pchar->attrStrength, pDesc->offAttrib));
            fMax = *(ATTRIB_GET_PTR(&pchar->pclass->pattrStrengthMax[pchar->iCombatLevel], pDesc->offAttrib));
        }
        xcase kAttribType_Res: 
        {
            fTest = *(ATTRIB_GET_PTR(&pchar->attrResistance, pDesc->offAttrib));
            fOrig = 0.f;
            fVal = *(ATTRIB_GET_PTR(&pchar->attrResistance, pDesc->offAttrib));
            fMax = *(ATTRIB_GET_PTR(&pchar->pclass->pattrResistanceMax[pchar->iCombatLevel], pDesc->offAttrib));
        }
        xcase kAttribType_Max: 
        {
            fTest = *(ATTRIB_GET_PTR(&pchar->attrMax, pDesc->offAttrib));
            fOrig = *(ATTRIB_GET_PTR(&pchar->pclass->pattrMax[pchar->iCombatLevel], pDesc->offAttrib));
            fVal = *(ATTRIB_GET_PTR(&pchar->attrMax, pDesc->offAttrib));
            fMax = *(ATTRIB_GET_PTR(&pchar->pclass->pattrMaxMax[pchar->iCombatLevel], pDesc->offAttrib));
        }
        xcase kAttribType_Special:
        {
            *buffOrDebuff = 0;
            if(stricmp(pDesc->pchName, "Influence")==0)
            {
                return pchar->iInfluencePoints;
            }
            if(stricmp(pDesc->pchName, "CombatModShift")==0)
            {
                return pchar->iCombatModShiftToSendToClient;
            }
            if(stricmp(pDesc->pchName, "CurrentXP")==0)
            {
                int experience_level = character_CalcExperienceLevel(pchar);
                return g_ExperienceTables.aTables.piRequired[experience_level+1]-pchar->iExperiencePoints;
            }
            else if( stricmp( pDesc->pchName, "CurrentDebt")==0)
            {
                return pchar->iExperienceDebt;
            }
            else if( stricmp( pDesc->pchName, "LastHitChance")==0)
            {
                return pchar->fLastChance;
            }
            else if( stricmp( pDesc->pchName, "EnduranceConsumption")==0)
            {
                PowerListItem *ppowListItem;
                PowerListIter iter;
                F32 fCostEnd=0;
                for (ppowListItem = powlist_GetFirst(&pchar->listTogglePowers, &iter);
                    ppowListItem != NULL;
                    ppowListItem = powlist_GetNext(&iter))
                {
                    Power * ppow = ppowListItem->ppow;
                    if (!ppow && ppow->ppowBase->fActivatePeriod )
                        continue;
                    fCostEnd += (ppow->ppowBase->fEnduranceCost/(ppow->pattrStrength->fEnduranceDiscount+0.0001f))/ppow->ppowBase->fActivatePeriod;
                }
                return fCostEnd;
            }

        }
    }

    if(fMax && fVal>=fMax) 
    {
        *buffOrDebuff = 3;
        fVal = fMax;
    }
    else if(fTest<fOrig) 
        *buffOrDebuff = 1;
    else if(fTest>fOrig)
        *buffOrDebuff = 2;

    return fVal;
}



void sendAllMatchingMods( Entity * e, Packet *pak )
{
    if (e->pchar)
    {
        AttribDescription *pDesc;
        AttribMod *pmod;
        AttribModListIter iter;

        pmod = modlist_GetFirstMod(&iter, &e->pchar->listMod);
        while(pmod!=NULL)
        {
            const AttribModTemplate *t = pmod->ptemplate;
            int iHash = modGetBuffHash( t );
            pDesc = attribDescriptionGet( iHash );        

            if (!(t->fDelay && pmod->fTimer > 0)
                && pDesc && pmod->fChance > 0
                && !pmod->bSuppressedByStacking)
            {    
                Entity *src = erGetEnt(pmod->erSource);
                AttribSource eSrcType;
                const char *pchSrc = "";

                if(!src)
                    eSrcType = kAttribSource_Unknown;
                else if(src==e)
                    eSrcType = kAttribSource_Self;
                else if(ENTTYPE(src)==ENTTYPE_CRITTER)        
                {
                    eSrcType = kAttribSource_Critter;
                    pchSrc = src->name;
                    if(ENTTYPE(src)==ENTTYPE_CRITTER && (pchSrc==NULL || pchSrc[0]=='\0'))
                    {
                        pchSrc = npcDefList.npcDefs[src->npcIndex]->displayName;
                    }
                }
                else if(ENTTYPE(src)==ENTTYPE_PLAYER)
                {
                    pchSrc = src->name;
                    eSrcType = kAttribSource_Player;
                }

                pktSendBits(pak,1,1);
                pktSendBitsPack(pak,10,iHash);
                basepower_Send(pak,t->ppowBase);
                pktSendBits(pak,2,eSrcType);
                if(eSrcType==kAttribSource_Critter || eSrcType==kAttribSource_Player)
                {
                    pktSendString(pak,pchSrc);
                    pktSendBitsPack(pak,20,src->id);
                }
                pktSendF32(pak,pmod->fChance);
                if (server_state.enablePowerDiminishing)
                {
                    pktSendF32(pak,pmod->fMagnitude); // need to calculate diminished values here
                } else {
                    pktSendF32(pak,pmod->fMagnitude);
                }
            }
            pmod = modlist_GetNextMod(&iter);
        }
    }
    pktSendBits(pak,1,0);
}

void csrViewAttributesTarget( Entity *e )
{
    e->attributesTarget = e->pchar->erTargetInstantaneous;
    e->newAttribTarget = 1;
}

void clearViewAttributes( Entity *e )
{
    e->attributesTarget = erGetRef(NULL);
}

void petViewAttributes( Entity *e, int svr_id )
{
    Entity *pet = entFromId( svr_id );

    if ( !isPetDisplayed(pet) || !isPetCommadable(pet) )
        return;
    else
    {
        e->attributesTarget = erGetRef(pet);
        e->newAttribTarget = 1;
    }
}

void sendAttribDescription( Entity *player, Packet *pak, int send_mods )
{
    F32 fVal;
    U32 buffOrDebuff = 0;
    int i,j; 
    Entity * e;
    int bCombatNumbersOpen =  player->pl && player->pl->winLocs &&  player->pl->winLocs[WDW_COMBATNUMBERS].mode==WINDOW_DISPLAYING; 

    pktSendBits(pak, 1, player->newAttribTarget );
    player->newAttribTarget = 0;

    if(  bCombatNumbersOpen  ) // Send Everything
    {
        pktSendBits(pak,1,1);

        e = erGetEnt(player->attributesTarget);
        if( !e )
        {
            e = player;
            pktSendBits(pak,1,0);
        }
        else
        {
            pktSendBits(pak,1,1);
            pktSendBits(pak,32,e->owner);
        }

        for( i = eaSize(&g_AttribCategoryList.ppCategories)-1; i >= 0; i-- )
        {
            for( j = eaSize(&g_AttribCategoryList.ppCategories[i]->ppAttrib)-1; j>=0; j--)
            {
                AttribDescription * pDesc = g_AttribCategoryList.ppCategories[i]->ppAttrib[j];
                fVal = attrib_GetVal( e->pchar, pDesc, &buffOrDebuff );
                pktSendF32( pak, fVal );
                pktSendBits(pak, 2,buffOrDebuff);
            }
        }
        sendAllMatchingMods(e,pak);
    }
    else
    {
        pktSendBits(pak,1,0);

        // loop over combat monitor list
        for( i = 0; i < MAX_COMBAT_STATS+1; i++ )
        {
            AttribDescription * pDesc;
            int key = (i==MAX_COMBAT_STATS)?offsetof(CharacterAttributes, fDefense):player->pl->combatMonitorStats[i].iKey;
            
            pDesc = attribDescriptionGet(key);
            if( pDesc )
            {
                pktSendBits(pak,1,1);
                pktSendBits( pak, 32, key );
                fVal = attrib_GetVal( player->pchar, pDesc, &buffOrDebuff );
                pktSendF32( pak, fVal );
                pktSendBits(pak, 2,buffOrDebuff);
            }
            else
            {
                if(i < MAX_COMBAT_STATS)
                    player->pl->combatMonitorStats[i].iKey = 0;
                pktSendBits(pak,1,0);
            }
        }
    }

}

void updateCombatMonitorStats( Entity *e )
{
    START_PACKET( pak, e, SERVER_SEND_COMBAT_STATS );
    sendCombatMonitorStats( e, pak );
    END_PACKET
}

#elif CLIENT

#include "entity/entclient.h"
#include "gameComm/wdwbase.h"
#include "UI/uiWindows.h"

AttribContributer *findMatchingMod(AttribDescription *pDesc, AttribContributer *pContrib)
{
    int i;
    for( i = eaSize(&pDesc->ppContributers)-1; i>=0; i-- )
    {
        AttribContributer *pOrig = pDesc->ppContributers[i];
        if(pOrig->svr_id && pOrig->svr_id == pContrib->svr_id && pOrig->eSrcType == pContrib->eSrcType && pOrig->ppow == pContrib->ppow )
            return pOrig;
    }
    return NULL;
}

void getAllMatchingMods( Entity * e, Packet *pak )
{
    while(pktGetBits(pak,1))
    {
        AttribContributer *pContrib    = malloc(sizeof(AttribContributer));
        int iHash = pktGetBitsPack(pak,10);
        AttribDescription *pDesc = attribDescriptionGet( iHash );
        pContrib->ppow = basepower_Receive(pak, &g_PowerDictionary, e);
        pContrib->eSrcType = pktGetBits(pak,2);
        if(pContrib->eSrcType==kAttribSource_Critter || pContrib->eSrcType==kAttribSource_Player)
        {
            pContrib->pchSrcDisplayName = (char*)allocAddString(pktGetString(pak));
            pContrib->svr_id = pktGetBitsPack(pak,20);
        }
        pContrib->fChance = pktGetF32(pak);
        pContrib->fMag = pktGetF32(pak);
        if( pDesc )
        {
            AttribContributer *pOrig = findMatchingMod(pDesc,pContrib);
            if(pOrig)
            {
                pOrig->fMag += pContrib->fMag;
            }
            else
            {
                //F32 *pf = ATTRIB_GET_PTR(e->pchar->pclass->ppattrBase[0],pDesc->offAttrib);
                //pContrib->fMag += *pf;
                eaPush(&pDesc->ppContributers, pContrib);
            }
        }
    }
}



void receiveAttribDescription( Entity *e, Packet *pak, int get_mod, int oo_pak )
{
    int i, j;

    if(pktGetBits(pak,1))
        window_setMode(WDW_COMBATNUMBERS,WINDOW_DISPLAYING);

    if(pktGetBits(pak,1))
    {
        if(pktGetBits(pak,1) )
        {
            int id = pktGetBits(pak,32);
            g_pAttribTarget = validEntFromId( id );
        }
        else
            g_pAttribTarget = 0;

        for( i = eaSize(&g_AttribCategoryList.ppCategories)-1; i >= 0; i-- )
        {
            for( j = eaSize(&g_AttribCategoryList.ppCategories[i]->ppAttrib)-1; j>=0; j--)
            {
                AttribDescription * pDesc = g_AttribCategoryList.ppCategories[i]->ppAttrib[j];
                pDesc->fVal = pktGetF32(pak);
                pDesc->buffOrDebuff = (int)pktGetBits(pak,2);
                if(get_mod)
                    eaClearEx(&pDesc->ppContributers,NULL);
            }
        }

        if(get_mod)
            getAllMatchingMods(g_pAttribTarget?g_pAttribTarget:e, pak);
    }
    else
    {
        // loop over combat monitor list
        for( i = 0; i < MAX_COMBAT_STATS+1; i++ )
        {
            if( pktGetBits(pak,1) )
            {
                int iKey =     pktGetBits( pak, 32 );
                float fVal = pktGetF32(pak);
                int buff = (int)pktGetBits(pak,2);
                AttribDescription * pDesc = attribDescriptionGet(iKey);
                if (pDesc)
                {
                    pDesc->fVal = fVal;
                    pDesc->buffOrDebuff = buff;
                }
                else
                {
                    Errorf("Attrib description missing? keyID(%i) CombatStat(%i)", iKey, i);
                }
            }
        }
    }
}

#endif

void sendCombatMonitorStats( Entity *e, Packet *pak)
{
    int i;
    for( i = 0; i < MAX_COMBAT_STATS; i++ )
    {
        pktSendIfSetBitsPack(pak, 8, e->pl->combatMonitorStats[i].iKey );
        pktSendIfSetBits(pak, 4, e->pl->combatMonitorStats[i].iOrder );
    }
}

void receiveCombatMonitorStats(Entity *e, Packet *pak)
{
    int i;
    for( i = 0; i < MAX_COMBAT_STATS; i++ )
    {
        e->pl->combatMonitorStats[i].iKey = pktGetIfSetBitsPack(pak, 8);
        e->pl->combatMonitorStats[i].iOrder = pktGetIfSetBits(pak, 4);
    }
}


