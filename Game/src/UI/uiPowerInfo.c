#include <utilitieslib/components/Earray.h>         // for StructGetNum
#include "player/player.h"
#include "entity/entity.h"         // for entity
#include "entity/powers.h"
#include "entity/EntPlayer.h"
#include "entity/character_base.h" // for pchar
#include "entity/attrib_description.h"
#include "entity/attrib_names.h"
#include <utilitieslib/utils/file.h>
#include <utilitieslib/components/estring.h>
#include <utilitieslib/utils/structDefines.h>
#include "win/input.h"

#include "UI/sprite/sprite_font.h"    // for font definitions
#include "UI/sprite/sprite_text.h"    // for font functions
#include "UI/sprite/sprite_base.h"    // for sprites
#include "graphics/textureatlas.h"
#include <utilitieslib/language/MessageStoreUtil.h>
#include "graphics/ttFontUtil.h"

#include "UI/uiWindows.h"

#include "UI/uiInput.h"
#include "UI/uiUtilGame.h"     // for draw frame
#include "UI/uiUtilMenu.h"     //
#include "UI/uiUtil.h"         // for color definitions
#include "UI/uiNet.h"
#include "UI/uiContextMenu.h"
#include "UI/uiClipper.h"
#include "UI/uiCombatNumbers.h"
#include "UI/uiToolTip.h"
#include "UI/uiScrollBar.h"
#include "UI/uiClipper.h"
#include "uiCombineSpec.h"
#include <utilitieslib/language/AppLocale.h>
#include "gameData/PowerNameRef.h"
#include "gameComm/villainDef.h"
#include "UI/uiPowerInfo.h"
#include "uiSlider.h"
#include "UI/uiGame.h"
#include "UI/Hybrid/uiHybridMenu.h"

#include "formatter/smf_main.h"
#define SMFVIEW_PRIVATE
#include "uiSMFView.h"

const BasePower * g_pBasePowMouseover=0;
const Power *g_pPowMouseover = 0;

extern StaticDefine ParsePowerDefines[];
static TextAttribs s_taPowerInfo =
{
    /* piBold        */  (int *)0,
    /* piItalic      */  (int *)0,
    /* piColor       */  (int *)0xffffffff,
    /* piColor2      */  (int *)0,
    /* piColorHover  */  (int *)0xffffffff,
    /* piColorSelect */  (int *)0,
    /* piColorSelectBG*/ (int *)0x333333ff,
    /* piScale       */  (int *)(int)(1.0f*SMF_FONT_SCALE),
    /* piFace        */  (int *)&smfSmall,
    /* piFont        */  (int *)0,
    /* piAnchor      */  (int *)0,
    /* piLink        */  (int *)0x80e080ff,
    /* piLinkBG      */  (int *)0,
    /* piLinkHover   */  (int *)0x66ff66ff,
    /* piLinkHoverBG */  (int *)0,
    /* piLinkSelect  */  (int *)0,
    /* piLinkSelectBG*/  (int *)0x666666ff,
    /* piOutline     */  (int *)1,
    /* piShadow      */  (int *)0,
};
static int s_bReformatInfo;

static void modGetMagnitudeAndDuration(const AttribModTemplate *ptemplate, const CharacterClass *pclass, int iLevel, F32 *pfMagnitude, F32 *pfDuration )
{
    float fVal;
    float fStr = 1.0f;
    int iEffCombatLevel = iLevel;

    if(ptemplate->iVarIndex<0)
        fVal = class_GetNamedTableValue(pclass, ptemplate->pchTable, iLevel);
    else
        fVal = ptemplate->fMagnitude; //ppow->afVars[ptemplate->iVarIndex];

    fVal *= ptemplate->fScale;

    if(ptemplate->bAllowStrength && ptemplate->offAttrib<sizeof(CharacterAttributes))
    {
    //        fStr = *(ATTRIB_GET_PTR(ppow->pattrStrength, ptemplate->offAttrib));
    //        fVal *= fStr;
    }

    switch(ptemplate->eType)
    {
    case kModType_Duration:
        *pfMagnitude = ptemplate->fMagnitude;
        *pfDuration = fVal;
        break;

    case kModType_Magnitude:
        *pfMagnitude = fVal;
        *pfDuration = ptemplate->fDuration;
        break;

    case kModType_SkillMagnitude:
        break;

    case kModType_Expression:
        //combateval_StoreAttribCalcInfo(f, fVal, ptemplate->fScale, 1.f, fStr);
        if(ptemplate->ppchMagnitude)
        {
            *pfMagnitude = ptemplate->fMagnitude;
        }
        else
        {
            *pfMagnitude = ptemplate->fMagnitude;
        }

        if(ptemplate->ppchDuration)
        {
            *pfDuration = ptemplate->fDuration;
        }
        else
        {
            *pfDuration = ptemplate->fDuration;
        }
        break;

    case kModType_Constant:
        *pfMagnitude = ptemplate->fMagnitude;
        *pfDuration = ptemplate->fDuration;
        break;
    }
}

static char * getPrettyDuration( F32 time )
{
    static char pchTime[16];

    if(time < 60) // less than a minute
        sprintf( pchTime, "%.2fs", time );    
    else if( time < 3600 ) // include minutes
    {
        int minutes = (time/60.f);
        int seconds = time - (minutes*60);
        sprintf( pchTime, "%im %is", minutes, seconds );
    }
    else if( time > 86400 ) // include hours
    {
        int hours = (time/3600.f);
        int minutes = ((time - (hours*3600))/60.f);
        sprintf( pchTime, "%ih %im", hours, minutes );        
    }
    else  // include days
    {
        int days = (time/86400.f);
        int hours = ((time - (days*86400))/3600.f);
        sprintf( pchTime, "%id %ih", days, hours );    
    }

    return pchTime;
}


bool pet_IsAutoOnly(const VillainDef *pDef)
{
    int i;
    // TODO: this could be run once at startup and stored on the villain def
    for( i = eaSize(&pDef->powers)-1; i>=0; i-- )
    {
        const PowerCategory* category;
        const BasePowerSet*  basePowerSet;
        const BasePower *basePower;

        category = powerdict_GetCategoryByName(&g_PowerDictionary, pDef->powers[i]->powerCategory);
        if(!category)
            continue;
        if((basePowerSet = category_GetBaseSetByName(category, pDef->powers[i]->powerSet)) == NULL)
            continue;
        if( pDef->powers[i]->power[0] == '*' )
        {
            int k;
            for( k = 0; k < eaSize(&basePowerSet->ppPowers); k++ )
            {
                if( basePowerSet->ppPowers[k]->eType != kPowerType_Auto )
                    return 0;
            }
        }
        else
        {
            basePower = baseset_GetBasePowerByName(basePowerSet, pDef->powers[i]->power);
            if(basePower && basePower->eType != kPowerType_Auto)
                return 0;
        }
    }
    return 1;
}

static const char *getGrantRequiresString( const char *estr )
{
    static char * str;

    if(str)
        estrDestroy(&str);

    if(!estr)
        return NULL;

    estrCreate(&str);

    if( strstriConst(estr, "kSiphonModesource.Mode?") )
        estrConcatCharString(&str, textStd("GrantSiphonMode"));

    return str;
}

static const char *getChanceModString( const char *estr )
{
    static char * str;

    if(str)
        estrDestroy(&str);

    if(!estr)
        return NULL;

    estrCreate(&str);

    if( strstriConst(estr, "Lethal") || strstriConst(estr, "LethalKB10") || strstriConst(estr, "LethalKB25") || 
            strstriConst(estr, "LethalKB50") || strstriConst(estr, "LethalKB70") || strstriConst(estr, "HailofBulletsKnockdown") || 
            strstriConst(estr, "HailofBulletsEndKnockback") )
        estrConcatf( &str, "<br>&nbsp;&nbsp;&nbsp;&nbsp;<i><color #aaaa88>%s</i>", textStd("DualPistolsLethalMode") );
    else if( strstriConst(estr, "FireDamage") || strstriConst(estr, "FireDamageDoT") || strstriConst(estr, "HailofBulletsFire") )
        estrConcatf( &str, "<br>&nbsp;&nbsp;&nbsp;&nbsp;<i><color #aaaa88>%s</i>", textStd("DualPistolsFireMode") );
    else if( strstriConst(estr, "ColdDamage") || strstriConst(estr, "HailofBulletsCold") )
        estrConcatf( &str, "<br>&nbsp;&nbsp;&nbsp;&nbsp;<i><color #aaaa88>%s</i>", textStd("DualPistolsColdMode") );
    else if( strstriConst(estr, "ToxicDamage") || strstriConst(estr, "HailofBulletsToxic") )
        estrConcatf( &str, "<br>&nbsp;&nbsp;&nbsp;&nbsp;<i><color #aaaa88>%s</i>", textStd("DualPistolsToxicMode") );
    else if( strstriConst(estr, "FieryEmbrace") )
        estrConcatf( &str, "<br>&nbsp;&nbsp;&nbsp;&nbsp;<i><color #aaaa88>%s</i>", textStd("FieryEmbraceMode") );

    return str;
}

static const float getChanceModValue( const char *estr )
{
    static char * str;

    if(str)
        estrDestroy(&str);

    if(!estr)
        return 0.0f;

    estrCreate(&str);

    if( strstriConst(estr, "Lethal") )
        return 1.0f;
    else if( strstriConst(estr, "FieryEmbrace") )
        return 1.0f;
    else if ( strstriConst(estr, "LethalKB10") )
        return 0.1f;
    else if ( strstriConst(estr, "LethalKB25") )
        return 0.25f;
    else if ( strstriConst(estr, "LethalKB50") )
        return 0.5f;
    else if ( strstriConst(estr, "LethalKB70") )
        return 0.7f;
    else if ( strstriConst(estr, "HailofBulletsKnockdown") )
        return 0.05f;
    else if ( strstriConst(estr, "HailofBulletsEndKnockback") )
        return 0.4f;
    else if ( strstriConst(estr, "FireDamage") )
        return 1.0f;
    else if ( strstriConst(estr, "FireDamageDoT") )
        return 0.8f;
    else if ( strstriConst(estr, "HailofBulletsFire") )
        return 0.6f;
    else if ( strstriConst(estr, "ColdDamage") ) 
        return 1.0f;
    else if ( strstriConst(estr, "HailofBulletsCold") )
        return 0.6f;
    else if ( strstriConst(estr, "ToxicDamage") )
        return 1.0f;
    else if ( strstriConst(estr, "HailofBulletsToxic") )
        return 0.6f;

    return 0.0f;
}

static const char *getRequiresString( const char *estr, const CharacterClass *pClass, bool bPet, bool bAssumeDefaults )
{
    static char * str;

    if(str)
        estrDestroy(&str);

    if(!estr)
        return NULL;

    estrCreate(&str);

    if( strstriConst(estr, "archtarget>Class_Minion_Grunteqarchtarget>Class_Minion_Smalleq||archtarget>Class_Minion_Petseq||archtarget>Class_Minion_Swarmeq||enttypetarget>playereq||!") )
        estrConcatf( &str, "<br>&nbsp;&nbsp;&nbsp;&nbsp;<i><color #aaaa88>%s</i>", textStd("ScrapperBossCrit") );
    else if( strstriConst(estr, "archtarget>Class_Minion_Grunteqarchtarget>Class_Minion_Smalleq||archtarget>Class_Minion_Petseq||archtarget>Class_Minion_Swarmeq||" ) )
        estrConcatf( &str, "<br>&nbsp;&nbsp;&nbsp;&nbsp;<i><color #aaaa88>%s</i>", textStd("ScrapperMinionCrit") );

    if(strstriConst(estr, "kMetersource>.9<") && strstriConst(estr, "kHeldtarget>0>kSleeptarget>0>||" ))
        estrConcatf( &str, "<br>&nbsp;&nbsp;&nbsp;&nbsp;<i><color #aaaa88>%s</i>", textStd("StalkerHeldSlept") );
    else if(strstriConst(estr, "kMetersource>0>" ))
        estrConcatf( &str, "<br>&nbsp;&nbsp;&nbsp;&nbsp;<i><color #aaaa88>%s</i>", textStd("StalkerHidden") );

    if(strstriConst(estr, "kStealthsource>0.5>" ))
        estrConcatf( &str, "<br>&nbsp;&nbsp;&nbsp;&nbsp;<i><color #aaaa88>%s</i>", textStd("DominatorDomination") );

    if(strstriConst(estr, "kHitPointstarget>10-100*5010-/0100minmaxrand100*" )) 
        estrConcatf( &str, "<br>&nbsp;&nbsp;&nbsp;&nbsp;<i><color #aaaa88>%s</i>", textStd("CorruptorScourge") );

    if(strstriConst(estr, "kImmobilizedtarget>0>kHeldtarget>0>||kSleeptarget>0>||kStunnedtarget>0>||"))
        estrConcatf( &str, "<br>&nbsp;&nbsp;&nbsp;&nbsp;<i><color #aaaa88>%s</i>", textStd("ControllerContianment") );



    if( strstriConst(estr,"originsource>" ))
    {
        int first=0;
        estrConcatf(&str, "<br>&nbsp;&nbsp;&nbsp;&nbsp;<i><color #aaaa88>%s", textStd("isOrigin"));

        if(strstriConst(estr, "originsource>Magic"))
        {
            estrConcatf(&str, textStd("IsMagicOrigin"));
            first = 1;
        }
        if(strstriConst(estr, "originsource>Mutation"))
        {
            if(first)
                estrConcatStaticCharArray(&str, ", ");
            estrConcatf(&str, textStd("IsMutationOrigin"));
            first = 1;
        }
        if(strstriConst(estr, "originsource>Natural"))
        {
            if(first)
                estrConcatStaticCharArray(&str, ", ");
            estrConcatf(&str, textStd("IsNaturalOrigin"));
            first = 1;
        }
        if(strstriConst(estr, "originsource>Science"))
        {
            if(first)
                estrConcatStaticCharArray(&str, ", ");
            estrConcatf(&str, textStd("IsScienceOrigin"));
            first = 1;
        }
        if(strstriConst(estr, "originsource>Technology"))
        {
            if(first)
                estrConcatStaticCharArray(&str, ", ");
            estrConcatf(&str, textStd("IsTechnologyOrigin"));
        }
        estrConcatStaticCharArray(&str, "</i>");
        
    }

    if(strstriConst(estr, "Electronictarget.HasTag?"))
        estrConcatf( &str, "<br>&nbsp;&nbsp;&nbsp;&nbsp;<i><color #aaaa88>%s</i>", textStd("AgainstElectionics") );
    if(strstriConst(estr, "Undeadtarget.HasTag?"))
        estrConcatf( &str, "<br>&nbsp;&nbsp;&nbsp;&nbsp;<i><color #aaaa88>%s</i>", textStd("AgainstUndead") );
    if(strstriConst(estr, "Ghosttarget.HasTag?"))
        estrConcatf( &str, "<br>&nbsp;&nbsp;&nbsp;&nbsp;<i><color #aaaa88>%s</i>", textStd("AgainstGhost") );

    if(strstriConst(estr, "kDD_BonusDotMode_2source.mode?"))
        estrConcatf( &str, "<br>&nbsp;&nbsp;&nbsp;&nbsp;<i><color #aaaa88>%s</i>", textStd("ComboEffectAttackVitals") );
    if(strstriConst(estr, "kDD_DebuffMode_2source.mode?"))
        estrConcatf( &str, "<br>&nbsp;&nbsp;&nbsp;&nbsp;<i><color #aaaa88>%s</i>", textStd("ComboEffectWeaken") );
    if(strstriConst(estr, "kDD_BonusAoEMode_2source.mode?"))
        estrConcatf( &str, "<br>&nbsp;&nbsp;&nbsp;&nbsp;<i><color #aaaa88>%s</i>", textStd("ComboEffectSweep") );
    if(strstriConst(estr, "kDD_StatusMode_2source.mode?"))
        estrConcatf( &str, "<br>&nbsp;&nbsp;&nbsp;&nbsp;<i><color #aaaa88>%s</i>", textStd("ComboEffectEmpower") );

    if(strstriConst(estr, "Temporary_Powers.Temporary_Powers.Combo_Level_1source.ownPower?!Temporary_Powers.Temporary_Powers.Combo_Level_2source.ownPower?!&&Temporary_Powers.Temporary_Powers.Combo_Level_3source.ownPower?!&&") ||
        strstriConst(estr, "Temporary_Powers.Temporary_Powers.Combo_Level_1source.ownPower?!&&Temporary_Powers.Temporary_Powers.Combo_Level_2source.ownPower?!&&Temporary_Powers.Temporary_Powers.Combo_Level_3source.ownPower?!&&") )
    {
        if(!bAssumeDefaults)
            estrConcatf( &str, "<br>&nbsp;&nbsp;&nbsp;&nbsp;<i><color #aaaa88>%s</i>", textStd("ComboEffectBrawling0") );
    }
    else
    {    
        if(strstriConst(estr, "Temporary_Powers.Temporary_Powers.Combo_Level_1source.ownPower?"))
            estrConcatf( &str, "<br>&nbsp;&nbsp;&nbsp;&nbsp;<i><color #aaaa88>%s</i>", textStd("ComboEffectBrawling1") );
        if(strstriConst(estr, "Temporary_Powers.Temporary_Powers.Combo_Level_2source.ownPower?"))
            estrConcatf( &str, "<br>&nbsp;&nbsp;&nbsp;&nbsp;<i><color #aaaa88>%s</i>", textStd("ComboEffectBrawling2") );
        if(strstriConst(estr, "Temporary_Powers.Temporary_Powers.Combo_Level_3source.ownPower?"))
            estrConcatf( &str, "<br>&nbsp;&nbsp;&nbsp;&nbsp;<i><color #aaaa88>%s</i>", textStd("ComboEffectBrawling3") );
    }

    if(strstriConst(estr, "Temporary_Powers.Temporary_Powers.Temporal_Selection_Bufftarget.ownPower?!"))
    {
        if(!bAssumeDefaults)
            estrConcatf( &str, "<br>&nbsp;&nbsp;&nbsp;&nbsp;<i><color #aaaa88>%s</i>", textStd("ComboEffectAcceleratedOff") );
    }
    else if(strstriConst(estr, "Temporary_Powers.Temporary_Powers.Temporal_Selection_Bufftarget.ownPower?"))
        estrConcatf( &str, "<br>&nbsp;&nbsp;&nbsp;&nbsp;<i><color #aaaa88>%s</i>", textStd("ComboEffectAcceleratedOn") );

    if(strstriConst(estr, "Temporary_Powers.Temporary_Powers.Time_Crawl_Debufftarget.ownPower?!"))
    {
        if(!bAssumeDefaults)
            estrConcatf( &str, "<br>&nbsp;&nbsp;&nbsp;&nbsp;<i><color #aaaa88>%s</i>", textStd("ComboEffectDelayedOff") );
    }
    else if(strstriConst(estr, "Temporary_Powers.Temporary_Powers.Time_Crawl_Debufftarget.ownPower?"))
        estrConcatf( &str, "<br>&nbsp;&nbsp;&nbsp;&nbsp;<i><color #aaaa88>%s</i>", textStd("ComboEffectDelayedOn") );

    if(strstriConst(estr, "Temporary_Powers.Temporary_Powers.Beam_Rifle_Debufftarget.ownPower?!"))
    {
        if(!bAssumeDefaults)
            estrConcatf( &str, "<br>&nbsp;&nbsp;&nbsp;&nbsp;<i><color #aaaa88>%s</i>", textStd("ComboEffectDisintegratedOff") );
    }
    else if(strstriConst(estr, "Temporary_Powers.Temporary_Powers.Beam_Rifle_Debufftarget.ownPower?"))
        estrConcatf( &str, "<br>&nbsp;&nbsp;&nbsp;&nbsp;<i><color #aaaa88>%s</i>", textStd("ComboEffectDisintegratedOn") );

    return str;
}

int requiresArchetypePvPEval( const char ** ppReq, const char *pchClass, const char *creatorClass, int bPvP )
{
    int retval = 0, i = 0;
    int * iStack=0;

     for( i = 0; i < eaSize(&ppReq); i++ )
    {
        int len = strlen(ppReq[i]);

        if( stricmp(ppReq[i], "arch") == 0 )
        {
            if( stricmp(ppReq[i+1], "source>") == 0 )
            {
                if( stricmp( ppReq[i+2], pchClass) == 0 )
                    eaiPush(&iStack, 1);
                else
                    eaiPush(&iStack, 0);
            }
            else if ( (stricmp(ppReq[i+1], "source.owner>") == 0) || (stricmp(ppReq[i+1], "source.creator>") == 0) )
            {
                if( creatorClass && stricmp( ppReq[i+2], creatorClass) == 0 )
                    eaiPush(&iStack, 1);
                else
                    eaiPush(&iStack, 0);
            }
            else
                eaiPush(&iStack, 1);
            eaiPush(&iStack, 1);

            i+=2;
        }
        else if ( (stricmp(ppReq[i], "source.ownPower?") == 0) || (stricmp(ppReq[i], "target.ownPower?") == 0) )
        {
            // Whether or not they own the power, we don't care in this evaluator. Assuming they own the power doesn't work if we are negating the statement later.
            eaiPop(&iStack);
            eaiPush(&iStack, 1);
            if( (i < eaSize(&ppReq) - 1) && (stricmp( ppReq[i+1], "!") == 0) )
                    i++;
        }
        else if ( stricmp(ppReq[i], "enttype") == 0 )
        {
            if(  stricmp(ppReq[i+1], "target>") == 0 )
            {
                if( (bPvP && stricmp( ppReq[i+2], "player") == 0) ||
                    (!bPvP && stricmp( ppReq[i+2], "critter") == 0) )
                    eaiPush(&iStack, 1);
                else
                    eaiPush(&iStack, 0);
            }
            else
                eaiPush(&iStack, 1);
            eaiPush(&iStack, 1);
            i+=2;
        }
        else if( stricmp(ppReq[i], "isPVPMap?") == 0 )
        {
            eaiPush(&iStack, bPvP?1:0);
        }
        else if( ppReq[i][len-1] == '>' && len > 1 )
        {
            eaiPop(&iStack);
            eaiPush(&iStack, 1);
        }    
        else if( stricmp( ppReq[i], "<") == 0 || stricmp( ppReq[i], "<=") == 0  || stricmp( ppReq[i], ">=") == 0 || 
            ppReq[i][0] == '>'  || ppReq[i][0] == '?' || ppReq[i][0] == '+'  || ppReq[i][0] == '-' ||
            ppReq[i][0] == '/'  || ppReq[i][0] == '-' || ppReq[i][0] == '%' || ppReq[i][0] == '*' || ppReq[i][len-1] == '?' )
        {
            // don't care what this operation is, it won't involve archetypes, so it passes
            eaiPop(&iStack); 
            eaiPop(&iStack);
            eaiPush(&iStack, 1);
        }
        else if( stricmp( ppReq[i], "&&" ) == 0 )
        {
            int val1 = eaiPop(&iStack);
            int val2 = eaiPop(&iStack);
            eaiPush(&iStack, val1 && val2);
        }
        else if( stricmp( ppReq[i], "||" ) == 0 )
        {
            int val1 = eaiPop(&iStack);
            int val2 = eaiPop(&iStack);
            eaiPush(&iStack, val1 || val2 );
        }
        else if( stricmp( ppReq[i], "!" ) == 0 )
        {
            int val = eaiPop(&iStack);
            eaiPush(&iStack, !val);
        }
        else if( stricmp( ppReq[i], "eq" ) == 0 || stricmp( ppReq[i], "==" ) == 0 )
        {
            int val = (eaiPop(&iStack) == eaiPop(&iStack));
            eaiPush(&iStack, val);
        }
        else if( stricmp( ppReq[i], "minmax" ) == 0 )
        {
            eaiPop(&iStack); 
            eaiPop(&iStack);
            eaiPop(&iStack);
            eaiPush(&iStack, 1);
        }
        else // just add random variable
            eaiPush(&iStack, 1);
    }

    retval = iStack[0];
    eaiDestroy(&iStack);

    return retval;
}

static void getTotalDamage( const BasePower * ppow, const CharacterAttributes *pAttr, const CharacterClass *pClass, const CharacterClass *creatorClass, int iLevel, F32 *fDamageBase, F32 * fDamageTotal, int bPvP )
{
    int i,j,k, first = 0;
    F32 fBase=0, fTotal=0;
    const char *pchReq = 0;
        
    for(i=0;i<eaSize(&ppow->ppTemplates); i++)
    {
        const AttribModTemplate *pTemplate = ppow->ppTemplates[i];
        F32 fMag, fDuration;
        char * estr;
        const F32 *fBoosted = pAttr?ATTRIB_GET_PTR(pAttr, pTemplate->offAttrib):0;
        
        // Add in Pet damage first
        if( ppow->ppTemplates[i]->offAttrib==kSpecialAttrib_EntCreate )
        {
            const VillainDef *pDef = villainFindByName(ppow->ppTemplates[i]->pchEntityDef);
            if( pDef && pet_IsAutoOnly(pDef) )
            {
                const CharacterClass * villianClass = classes_GetPtrFromName(&g_VillainClasses, pDef->characterClassName);
                F32 fPetDamageBase=0, fPetDamageTotal=0;

                for( j = eaSize(&pDef->powers)-1; j>=0; j-- )
                {
                    const PowerCategory*  category;
                    const BasePowerSet*   basePowerSet;
                    const BasePower *basePower;
                    category = powerdict_GetCategoryByName(&g_PowerDictionary, pDef->powers[j]->powerCategory);
                    if(!category)
                        continue;
                    if((basePowerSet = category_GetBaseSetByName(category, pDef->powers[j]->powerSet)) == NULL)
                        continue;
                    if( pDef->powers[j]->power[0] == '*' )
                    {
                        int k;
                        for( k = 0; k < eaSize(&basePowerSet->ppPowers); k++ )
                        {
                            getTotalDamage( basePowerSet->ppPowers[k], pAttr, villianClass, pClass, iLevel, &fPetDamageBase, &fPetDamageTotal, bPvP );
                            if( basePowerSet->ppPowers[k]->fActivatePeriod )
                            {

                                fBase += (fPetDamageBase * MAX(1,floor(ppow->ppTemplates[i]->fDuration/basePowerSet->ppPowers[k]->fActivatePeriod)));
                                fTotal += (fPetDamageTotal * MAX(1,floor(ppow->ppTemplates[i]->fDuration/basePowerSet->ppPowers[k]->fActivatePeriod)));
                            }
                            else
                            {
                                fBase += fPetDamageBase;
                                fTotal += fPetDamageTotal;
                            }
                        }
                    }
                    else
                    {
                        basePower = baseset_GetBasePowerByName(basePowerSet, pDef->powers[j]->power);
                        if(basePower)
                        {
                            getTotalDamage( basePower, pAttr, villianClass, pClass, iLevel, &fPetDamageBase, &fPetDamageTotal, bPvP );
                            if(basePower->fActivatePeriod )
                            {
                                fBase += (fPetDamageBase * MAX(1,floor(ppow->ppTemplates[i]->fDuration/basePower->fActivatePeriod)));
                                fTotal += (fPetDamageTotal * MAX(1,floor(ppow->ppTemplates[i]->fDuration/basePower->fActivatePeriod)));
                            }
                            else
                            {
                                fBase += fPetDamageBase;
                                fTotal += fPetDamageTotal;
                            }
                        }
                    }
                }
            }
        }

        if( (pTemplate->offAttrib<offsetof(CharacterAttributes, fDamageType) || pTemplate->offAttrib>=offsetof(CharacterAttributes, fHitPoints))
            || pTemplate->offAspect != offsetof(CharacterAttribSet, pattrAbsolute) )
        {
            continue; // skip non-damage attribs
        }

        if( eaSize(&pTemplate->ppchApplicationRequires) && !requiresArchetypePvPEval( pTemplate->ppchApplicationRequires, pClass->pchName, creatorClass ? creatorClass->pchName : NULL, bPvP ) )
            continue;
 
        estrCreate(&estr);
        for( j = 0; j < eaSize(&pTemplate->ppchApplicationRequires); j++ )
            estrConcatCharString(&estr, pTemplate->ppchApplicationRequires[j]);
        pchReq = getRequiresString(estr, pClass, 0, 1);
        if( strlen(pchReq) )
            continue;
        estrDestroy(&estr);

        modGetMagnitudeAndDuration(pTemplate, pClass, iLevel, &fMag, &fDuration );
        if( fMag < 0 )
        {
            if( pTemplate->fDuration )
            {
                int iTicks = floor( pTemplate->fDuration / pTemplate->fPeriod )+1;

                if( pTemplate->bCancelOnMiss ) // Bit of math for cancel on miss average
                {
                    F32 fAvgTicks = 0;
                    for( k=1; k < iTicks; k++ )
                        fAvgTicks += (powf(pTemplate->fChance, k) * (1-pTemplate->fChance) * k);
                    fAvgTicks += powf(pTemplate->fChance, k) * iTicks;

                    fBase += (-fMag)*fAvgTicks;
                    fTotal += (-fMag)*fAvgTicks;
                    if( pTemplate->bAllowStrength && pAttr )
                        fTotal += (-fMag)*fAvgTicks*(*fBoosted);
                }
                else // every tick has a chance always
                {
                    fBase += (-fMag)*iTicks*pTemplate->fChance;
                    fTotal += (-fMag)*iTicks*pTemplate->fChance;
                    if( pTemplate->bAllowStrength && pAttr )
                        fTotal += ((-fMag)*iTicks*pTemplate->fChance)*(*fBoosted);
                }
            }
            else // not DoT
            {
                fBase += (-fMag)*pTemplate->fChance;
                fTotal += (-fMag)*pTemplate->fChance;
                if( pTemplate->bAllowStrength && pAttr )
                    fTotal += (-fMag)*pTemplate->fChance*(*fBoosted);
            }
        }
    }
    
    *fDamageBase = fBase;
    *fDamageTotal = fTotal;

}

typedef enum GroupType
{
    kGroupType_Damage,
    kGroupType_Defense,
    kGroupType_Count,
}GroupType;


static F32 equalForAllTypes[kModTarget_Count][kGroupType_Count][kAttribType_Count];
static F32 equalSetsForAllTypes[kModTarget_Count][kGroupType_Count][kAttribType_Count];
static F32 displayedForAllTypes[kModTarget_Count][kGroupType_Count][kAttribType_Count];
static int defenseTypes = 1023; // Defense Type offsets we use, or'd together
static int defenseTypesCount = 10;
static int damageTypes = 639; // Damage Type offsets we use, or'd together
static int damageTypesCount = 8; // Damage Type offsets we use, or'd together

static void markAllTypes(const BasePower *ppow, const char * pchClass, const char * pchCreatorClass, int bPvP)
{
    int i, w, x, y;

    for( w = 0; w < kModTarget_Count; w++ )
    {
        for( x = 0; x < kGroupType_Count; x++ )
        {
            for( y = 0; y < kAttribType_Count; y++)
            {
                int check = 0;
                equalForAllTypes[w][x][y] = 0.f;
                equalSetsForAllTypes[w][x][y] = 0.f;
                displayedForAllTypes[w][x][y] = 0.f;
                
                for(i=0;i<eaSize(&ppow->ppTemplates);i++)
                {
                    int offset = ppow->ppTemplates[i]->offAttrib;
                    int aspect = ppow->ppTemplates[i]->offAspect;
                    AttribType eType;

                    if( ppow->ppTemplates[i]->eTarget !=  w )
                        continue;

                    if( eaSize(&ppow->ppTemplates[i]->ppchApplicationRequires) && !requiresArchetypePvPEval( ppow->ppTemplates[i]->ppchApplicationRequires, pchClass, pchCreatorClass, bPvP ) )
                        continue;

                    if(aspect==offsetof(CharacterAttribSet, pattrMod))
                        eType = kAttribType_Cur;
                    else if(aspect==offsetof(CharacterAttribSet, pattrMax))
                        eType = kAttribType_Max;
                    else if(aspect==offsetof(CharacterAttribSet, pattrStrength))
                        eType = kAttribType_Str;
                    else if(aspect==offsetof(CharacterAttribSet, pattrResistance))
                        eType = kAttribType_Res;
                    else if(aspect==offsetof(CharacterAttribSet, pattrAbsolute))
                        eType = kAttribType_Abs;

                    if( y == eType )
                    {
                        if(kGroupType_Damage == x && offset >= (int)offsetof(CharacterAttributes, fDamageType) && offset < (int)offsetof(CharacterAttributes, fHitPoints))
                        {
                            if(!equalForAllTypes[w][x][y] )
                                equalForAllTypes[w][x][y] = ppow->ppTemplates[i]->fMagnitude+ppow->ppTemplates[i]->fScale;
                            else if (equalForAllTypes[w][x][y] != ppow->ppTemplates[i]->fMagnitude+ppow->ppTemplates[i]->fScale )
                                break;
                            check |= (1<<(offset/4));
                        }
                        else if(kGroupType_Defense == x && offset >= (int)offsetof(CharacterAttributes, fDefenseType)    && offset < (int)offsetof(CharacterAttributes, fDefense))
                        {
                            if(!equalForAllTypes[w][x][y] )
                                equalForAllTypes[w][x][y] = ppow->ppTemplates[i]->fMagnitude+ppow->ppTemplates[i]->fScale;
                            else if (equalForAllTypes[w][x][y] != ppow->ppTemplates[i]->fMagnitude+ppow->ppTemplates[i]->fScale )
                                break;
                            check |= (1<<((offset-offsetof(CharacterAttributes, fDefenseType))/4));
                        }
                    }

                    if( (kGroupType_Damage == x && check == damageTypes) ||    (kGroupType_Defense == x && check == defenseTypes) )
                    {
                        equalForAllTypes[w][x][y] = 0;
                        equalSetsForAllTypes[w][x][y] += 1;
                    }
                }
                
                if(!equalSetsForAllTypes[w][x][y])
                    equalForAllTypes[w][x][y] = 0;
                else
                    equalForAllTypes[w][x][y] = 1;
            }
        }
    }
}

static void power_AddBaseStats( const BasePower *ppow, const CharacterAttributes *pAttr, const CharacterClass *pClass, const CharacterClass *creatorClass, int iLevel, char ** str, int ShowEnhance, bool bPvP, int bPet )
{
    F32 fDamageTotal = 0, fDamageBase = 0;
    F32 fEnhancedRechargeTime = ppow->fRechargeTime/(1+((pAttr && !bPet)?pAttr->fRechargeTime:0));
    F32 fEnhancedEndCost = ppow->fEnduranceCost/(1+(pAttr?pAttr->fEnduranceDiscount:0));
    F32 fEnhancedAcc = ppow->fAccuracy*(1+(pAttr?pAttr->fAccuracy:0));
    F32 fEnhancedInterupt = ppow->fInterruptTime/(1+(pAttr?pAttr->fInterruptTime:0));

    if(!pAttr)
        ShowEnhance = POWERINFO_BASE;
    
    // TOTAL AVERAGE DAMAGE -----------------------------------------------------------------------
    estrConcatStaticCharArray(str, "<table><tr border=0>");
    getTotalDamage( ppow, pAttr, pClass, creatorClass, iLevel, &fDamageBase, &fDamageTotal, bPvP );
    if( fDamageBase )
    {
        estrConcatf(str, "<td border=0><color #aaaaaa>%s</td>", textStd("PowerInfoTotalDamage") );
        if( ShowEnhance == POWERINFO_BOTH && fDamageTotal!=fDamageBase )
            estrConcatf(str, "<td border=0><color #ff4b4b>%.2f (%.2f)</td>", fDamageTotal, fDamageBase );
        else
            estrConcatf(str, "<td border=0><color #ff4b4b>%.2f</td>", ShowEnhance==POWERINFO_ENHANCED?fDamageTotal:fDamageBase );
    }
    estrConcatStaticCharArray(str, "</tr>");

    if (ppow->eType != kPowerType_Auto)                //    auto powers don't really do any of these
    {
        // DAMAGE PER ACTIVATION -----------------------------------------------------------------------
        if( fDamageBase && ppow->fTimeToActivate )
        {
            estrConcatf(str, "<tr border=0><td border=0><color #aaaaaa>%s</td>", textStd("PowerInfoDamagePerAct") );
            if( ShowEnhance == POWERINFO_BOTH && fDamageTotal!=fDamageBase )
                estrConcatf(str, "<td border=0><color #ff4b4b>%.2f (%.2f)</td>", fDamageTotal/ppow->fTimeToActivate, fDamageBase/ppow->fTimeToActivate );
            else
                estrConcatf(str, "<td border=0><color #ff4b4b>%.2f</td>", (ShowEnhance==POWERINFO_ENHANCED?fDamageTotal:fDamageBase)/ppow->fTimeToActivate );
            estrConcatStaticCharArray(str, "</tr>");
        }

        // DAMAGE PER CAST TIME -----------------------------------------------------------------------
        if( fDamageBase )
        {
            F32 fDPC = fDamageBase/(ppow->fTimeToActivate+ppow->fRechargeTime);
            F32 dDPCEnhanced = fDamageTotal/(ppow->fTimeToActivate+fEnhancedRechargeTime);
            estrConcatf(str, "<tr border=0><td border=0><color #aaaaaa>%s</td>", textStd("PowerInfoDamagePerCast") );
            if( ShowEnhance == POWERINFO_BOTH && dDPCEnhanced!=fDPC  )
                estrConcatf(str, "<td border=0><color #ff4b4b>%.2f (%.2f)</td>", dDPCEnhanced, fDPC );
            else
                estrConcatf(str, "<td border=0><color #ff4b4b>%.2f</td>", ShowEnhance==POWERINFO_ENHANCED?dDPCEnhanced:fDPC );
            estrConcatStaticCharArray(str, "</tr>");
        }

        // ACTIVATION TIME -----------------------------------------------------------------------
        estrConcatf(str, "<tr border=0><td border=0><color #aaaaaa>%s</td>", textStd("PowerInfoActivationTime") );
        estrConcatf(str, "<td border=0><color #ffffff>%s</td></tr>", getPrettyDuration(ppow->fTimeToActivate) );

        // RECHARGE TIME -----------------------------------------------------------------------
        estrConcatf(str, "<tr border=0><td border=0><color #aaaaaa>%s</td>", textStd("PowerInfoRechargeTime") );
        if( ShowEnhance == POWERINFO_BOTH && fEnhancedRechargeTime!=ppow->fRechargeTime )
            estrConcatf(str, "<td border=0><color #ffffff>%s (%s)</td>", getPrettyDuration(fEnhancedRechargeTime), getPrettyDuration(ppow->fRechargeTime));
        else
            estrConcatf(str, "<td border=0><color #ffffff>%s</td>", getPrettyDuration(ShowEnhance==POWERINFO_ENHANCED?fEnhancedRechargeTime:ppow->fRechargeTime) );
        estrConcatStaticCharArray(str, "</tr>");

        // ENDURANCE -----------------------------------------------------------------------
        if( ppow->fEnduranceCost )
        {
            F32 fEndCost = ppow->fEnduranceCost;
            estrConcatf(str, "<tr border=0><td border=0><color #aaaaaa>%s</td>", textStd("PowerInfoEnduranceCost") );

            if( ppow->eType == kPowerType_Toggle )
            {
                if( ShowEnhance == POWERINFO_BOTH && fEnhancedEndCost!=fEndCost )
                    estrConcatf(str, "<td border=0><color #45bdff> %.2f/s (%.2f/s)</td>", fEnhancedEndCost/ppow->fActivatePeriod, fEndCost/ppow->fActivatePeriod  );
                else
                    estrConcatf(str, "<td border=0><color #45bdff> %.2f/s</td>", (ShowEnhance==POWERINFO_ENHANCED?fEnhancedEndCost:fEndCost)/ppow->fActivatePeriod );
            }
            else
            {
                if( ShowEnhance == POWERINFO_BOTH && fEnhancedEndCost!=fEndCost )
                    estrConcatf(str, "<td border=0><color #45bdff> %.2f (%.2f)</td>", fEnhancedEndCost, fEndCost );
                else
                    estrConcatf(str, "<td border=0><color #45bdff> %.2f</td>", ShowEnhance==POWERINFO_ENHANCED?fEnhancedEndCost:fEndCost );
            }
            estrConcatStaticCharArray(str, "</tr>");
        }
    }

    // ACCURACY -----------------------------------------------------------------------
    if( ppow->fAccuracy )
    {
        estrConcatf(str, "<tr border=0><td border=0><color #aaaaaa>%s</td>", textStd("PowerInfoAccuracy") );
        if( ShowEnhance == POWERINFO_BOTH && fEnhancedAcc!=ppow->fAccuracy )
            estrConcatf(str, "<td border=0><color #ffed61> %.2fX (%.2fX)</td>", fEnhancedAcc,  ppow->fAccuracy);
        else
            estrConcatf(str, "<td border=0><color #ffed61> %.2fX</td>", ShowEnhance==POWERINFO_ENHANCED?fEnhancedAcc:ppow->fAccuracy );
        estrConcatStaticCharArray(str, "</tr>");
    }

    // INTERRUPT TIME -----------------------------------------------------------------------
    if(ppow->fInterruptTime)
    {
        estrConcatf(str, "<tr border=0><td border=0><color #aaaaaa>%s</td>", textStd("PowerInfoInterruptTime") );
        if( ShowEnhance == POWERINFO_BOTH && fEnhancedInterupt!=ppow->fInterruptTime )
            estrConcatf(str, "<td border=0><color #ffffff> %s (%s)</td>", getPrettyDuration(fEnhancedInterupt), getPrettyDuration(ppow->fInterruptTime));
        else
            estrConcatf(str, "<td border=0><color #ffffff> %s</td>", getPrettyDuration(ShowEnhance==POWERINFO_ENHANCED?fEnhancedInterupt:ppow->fInterruptTime) );
        estrConcatStaticCharArray(str, "</tr>");
    }

    // AVAILABLE AT -----------------------------------------------------------------------
     if( ppow->psetParent->piAvailable && ppow->psetParent->piAvailable[ppow->id.ipow] )
    {
        estrConcatf(str, "<tr border=0><td border=0><color #aaaaaa>%s</td>", textStd("PowerInfoAvailableAtLevel") );
        estrConcatf(str, "<td border=0><color #ffffff>%i</td></tr>", ppow->psetParent->piAvailable[ppow->id.ipow]+1 );
    }

    // Values for Custom Critter Powers
     if( ppow->fPointVal )
    {
        estrConcatf(str, "<tr border=0><td border=0><color #aaaaaa>%s</td>", textStd("AlphaPointValue") );
        estrConcatf(str, "<td border=0><color #ffffff>%.1f</td></tr>", ppow->fPointVal );
        if( ppow->fPointMultiplier )
        {
            estrConcatf(str, "<tr border=0><td border=0><color #aaaaaa>%s</td>", textStd("BetaPointValue") );
            estrConcatf(str, "<td border=0><color #ffffff>%.3f</td></tr>", ppow->fPointMultiplier );
        }
    }

    estrConcatStaticCharArray(str, "</table>");
}

static void power_AddTargetInfo( const BasePower *ppow, const CharacterAttributes *pAttr, char ** str, int ShowEnhance )
{
    char * pString = NULL;
    if(!pAttr)
        ShowEnhance = POWERINFO_BASE;

    // POWER TYPE -----------------------------------------------------------------------
    estrConcatf(str, "<table><tr border=0><td border=0><color #aaaaaa>%s</td>", textStd("PowerInfoPowerType"));
    switch( ppow->eType )
    {
        xcase kPowerType_Click:  pString = textStd( "ClickString" );
        xcase kPowerType_Auto:     pString = textStd( "AutoString" );
        xcase kPowerType_Toggle: pString = textStd( "ToggleString" );
        xcase kPowerType_Inspiration: pString = textStd( "InspirationString" );
        xcase kPowerType_Boost: pString = textStd("EnhancementString");
        xcase kPowerType_GlobalBoost: pString = textStd("GlobalEnhancementString");
        xdefault: 
            assert(0);    //    undefined
            pString = textStd("Unknown");
    }
    estrConcatf(str, "<td border=0><color #ffffff>%s</td></tr>", pString );

    // TARGET TYPE -----------------------------------------------------------------------
    estrConcatf(str, "<tr border=0><td border=0><color #aaaaaa>%s</td>", textStd("PowerInfoTargetType") );
    switch( ppow->eTargetType )
    {
        xcase kTargetType_None:                    pString = textStd("NoneString");
        xcase kTargetType_Caster:                pString = textStd("TargetCaster");

        xcase kTargetType_Player:               pString = textStd("TargetPlayer");
        xcase kTargetType_PlayerHero:           pString = textStd("TargetPlayerHero");
        xcase kTargetType_PlayerVillain:        pString = textStd("TargetPlayerVillain");
        xcase kTargetType_DeadPlayer:           pString = textStd("TargetPlayerDead");
        xcase kTargetType_DeadPlayerFriend:     pString = textStd("TargetPlayerFriendDead");
        xcase kTargetType_DeadPlayerFoe:        pString = textStd("TargetPlayerFoeDead");

        xcase kTargetType_Villain:                pString = textStd("TargetVillain");
        xcase kTargetType_DeadVillain:            pString = textStd("TargetVillainDead");
        xcase kTargetType_NPC:                    pString = textStd("TargetNPC");

        xcase kTargetType_DeadOrAliveFriend:    pString = textStd("TargetFriend");
        xcase kTargetType_DeadFriend:            pString = textStd("TargetFriendDead");
        xcase kTargetType_Friend:                pString = textStd("TargetFriendAlive");
        

        xcase kTargetType_DeadOrAliveFoe:        pString = textStd("TargetFoe");
        xcase kTargetType_DeadFoe:                pString = textStd("TargetFoeDead");
        xcase kTargetType_Foe:                    pString = textStd("TargetFoeAlive");

        xcase kTargetType_Location:                pString = textStd("TargetLocation");

        xcase kTargetType_Teammate:                pString = textStd("TargetTeammateAlive");
        xcase kTargetType_DeadOrAliveTeammate:    pString = textStd("TargetTeammate");
        xcase kTargetType_DeadTeammate:            pString = textStd("TargetTeammateDead");

        xcase kTargetType_DeadOrAliveMyPet:        pString = textStd("TargetPet");
        xcase kTargetType_DeadMyPet:            pString = textStd("TargetPetDead");
        xcase kTargetType_MyPet:                pString = textStd("TargetPetAlive");

        xcase kTargetType_Teleport:                pString = textStd("TargetLocation");
        xcase kTargetType_Any:                    pString = textStd("TargetAny");

        xcase kTargetType_MyOwner:                pString = textStd("TargetOwner");
        xcase kTargetType_MyCreator:            pString = textStd("TargetCreator");

        xcase kTargetType_MyCreation:            pString = textStd("TargetCreationAlive");
        xcase kTargetType_DeadMyCreation:        pString = textStd("TargetCreationDead");
        xcase kTargetType_DeadOrAliveMyCreation:pString = textStd("TargetCreation");

        xcase kTargetType_Leaguemate:            pString = textStd("TargetLeaguemateAlive");
        xcase kTargetType_DeadLeaguemate:        pString = textStd("TargetLeaguemateDead");
        xcase kTargetType_DeadOrAliveLeaguemate:pString = textStd("TargetLeaguemate");

        xcase kTargetType_Position:                pString = textStd("TargetPosition");

        xdefault: 
            assert(0);    //    undefined
            pString = textStd("Unknown");

    }
    estrConcatf(str, "<td border=0><color #ffffff>%s</td></tr>", pString);

    // RANGE -----------------------------------------------------------------------
    if( ppow->fRange )
    {
        F32 fEnhancedRange = ppow->fRange*(1+(pAttr?pAttr->fRange:0));

        estrConcatf(str, "<tr border=0><td border=0><color #aaaaaa>%s</td>", textStd("PowerInfoPowerRange"));
        if( !getCurrentLocale() )
        {
            if( ShowEnhance == POWERINFO_BOTH && fEnhancedRange!=ppow->fRange )
                estrConcatf(str, "<td border=0><color #ffffff>%.2f ft. (%.2f ft.)</td>", fEnhancedRange, ppow->fRange );
            else
                estrConcatf(str, "<td border=0><color #ffffff>%.2f ft.</td>", ShowEnhance==POWERINFO_ENHANCED?fEnhancedRange:ppow->fRange );
        }
        else
        {
            if( ShowEnhance == POWERINFO_BOTH && fEnhancedRange!=ppow->fRange )
                estrConcatf(str, "<td border=0><color #ffffff>%.2f m. (%.2f m.)</td>", fEnhancedRange*0.3048f, ppow->fRange*0.3048f  );
            else
                estrConcatf(str, "<td border=0><color #ffffff>%.2f m.</td>", (ShowEnhance==POWERINFO_ENHANCED?fEnhancedRange:ppow->fRange)*0.3048f  );
        }
        estrConcatStaticCharArray(str, "</tr>");
    }

    // EFFECT AREA -----------------------------------------------------------------------
    if( ppow->eEffectArea >= kEffectArea_Character && ppow->eEffectArea <=kEffectArea_Location )
    {
        estrConcatf(str, "<tr border=0><td border=0><color #aaaaaa>%s</td>", textStd("PowerInfoEffectArea") );
        switch( ppow->eEffectArea )
        {
            xcase kEffectArea_Character: pString = textStd("SingleTarget");
            xcase kEffectArea_Location: pString = textStd("LocationTarget");
            xcase kEffectArea_Cone:
            {
                if( !getCurrentLocale() )
                    pString = textStd("ConeTarget", DEG(ppow->fArc), ppow->fRadius);
                else
                    pString = textStd("ConeTargetMetric", DEG(ppow->fArc), ppow->fRadius*0.3048f);
            }
            xcase kEffectArea_Sphere:
            {
                if( !getCurrentLocale() )
                    pString = textStd("SphereTarget", ppow->fRadius);
                else
                    pString = textStd("SphereTargetMetric", ppow->fRadius*0.3048f);
            }
            xdefault: 
            assert(0);    //    undefined
            pString = textStd("Unknown");
        }

        estrConcatf(str, "<td border=0><color #ffffff>%s", pString );
        if(ppow->iMaxTargetsHit)
            estrConcatf(str, "<br>%s</td>", textStd("MaxTargets", ppow->iMaxTargetsHit) );
        else
            estrConcatStaticCharArray(str, "</td>");
        estrConcatStaticCharArray(str, "</tr>");
    }
    // ATTACK TYPES-----------------------------------------------------------------------
    if( eaiSize(&ppow->pAttackTypes) )
    {
        int i;
        char buffer[128];

        estrConcatf(str, "<tr border=0><td border=0><color #aaaaaa>%s</td><td border=0><color #ffffff>", textStd("ListedAttackTypes") );
        for( i = 0; i < eaiSize(&ppow->pAttackTypes); i++ )
        {
            itoa( ppow->pAttackTypes[i], buffer, 10 );
            estrConcatCharString(str, textStd( getAttribNameForTranslation( ppow->pAttackTypes[i])));
            if(i < eaiSize(&ppow->pAttackTypes)-1 )
                estrConcatStaticCharArray(str, ", ");
        }
        estrConcatStaticCharArray(str, "</td></tr>");
    }

    // AI REPORT -----------------------------------------------------------------------
    if( ppow->eAIReport == kAIReport_Never || ppow->eAIReport == kAIReport_HitOnly  )
    {
        estrConcatf(str, "<tr border=0><td border=0><color #aaaaaa>%s</td>", textStd( "AggroReport" ) );
        if( kAIReport_Never )
            estrConcatf(str, "<td border=0><color #ffffff>%s</td>", textStd("NoAggro") );
        else
            estrConcatf(str, "<td border=0><color #ffffff>%s</td>", textStd("HitAggro") );
        estrConcatStaticCharArray(str, "</tr>");
    }
    // VISIBILITY -----------------------------------------------------------------------
    if( ppow->eTargetVisibility == kTargetVisibility_None )
    {
        estrConcatf(str, "</table><br><color #aaaaaa>%s", textStd("NoLos"));
    }
    else
        estrConcatStaticCharArray(str, "</table>");
}

static void addAttribValue(char **str, AttribType eType, const AttribModTemplate *pTemplate, F32 fMag, F32 fBoosted, int ShowEnhance, char * pchMag )
{
    int offset = pTemplate->offAttrib;
    int bBoost = fBoosted && pTemplate->bAllowStrength && pTemplate->eType == kModType_Magnitude && ShowEnhance==POWERINFO_ENHANCED;
    int bBoth = fBoosted && pTemplate->bAllowStrength && pTemplate->eType == kModType_Magnitude && ShowEnhance==POWERINFO_BOTH;

    if(offset == offsetof(CharacterAttributes, fRechargeTime) )
    {
        bBoth = bBoost = 0;
    }

    
    if( offset == offsetof(CharacterAttributes, fDamageType[7]) ||
        offset == offsetof(CharacterAttributes, fHitPoints) ||
        offset == offsetof(CharacterAttributes, fRegeneration))
    {
        estrConcatStaticCharArray(str, "<color #58e849>"); // Healing
    }
    else if( offset == offsetof(CharacterAttributes, fEndurance) || offset == offsetof(CharacterAttributes, fRecovery)  )
    {
        estrConcatStaticCharArray(str, "<color #45bdff>"); // Endurance things
    }
    else if( offset == offsetof(CharacterAttributes, fAccuracy) || offset == offsetof(CharacterAttributes, fToHit) )
    {
        estrConcatStaticCharArray(str, "<color #ffed61>"); // ToHit
    }
    else if(offset>= (int)offsetof(CharacterAttributes, fDamageType) && offset < (int)offsetof(CharacterAttributes, fHitPoints))
    {
        if( eType == kAttribType_Res) // Resistance
            estrConcatStaticCharArray(str, "<color #ff944e>");
        else // Damage
        {
            if(eType == kAttribType_Abs) // Flip damage for readability
                fMag = -fMag;
            estrConcatStaticCharArray(str, "<color #ff4b4b>");
        }
    }
    // DEFENSE TYPES
    else if(offset >= (int)offsetof(CharacterAttributes, fDefenseType)    && offset <= (int)offsetof(CharacterAttributes, fDefense))
    {
        estrConcatStaticCharArray(str, "<color #bd54d9>"); // Defense
    }
    // STATUS EFFECTS
    else if( (offset >= (int)offsetof(CharacterAttributes, fConfused) && offset <= (int)offsetof(CharacterAttributes, fOnlyAffectsSelf)) ||
             (offset >= (int)offsetof(CharacterAttributes, fKnockup) && offset <= (int)offsetof(CharacterAttributes, fRepel)) )
    {
        estrConcatStaticCharArray(str, "<color #b594ff>");
        
        if( eType == kAttribType_Cur )
        {
            if(fMag < 0)
                fMag *= -1;
            estrConcatf(str, "%.2f<color #aaaaaa> ", fMag ); // Status cur is special
            return;
        }
    }
    else if( offset >= (int)offsetof(CharacterAttributes, fPerceptionRadius) )
    {
        if( bBoth )
            estrConcatf(str, "<color #ffffff>%+.2f%%%% (%+.2f%%%%)<color #aaaaaa> ", fMag*100*(1+fBoosted), fMag*100 );
        else
            estrConcatf(str, "<color #ffffff>%+.2f%%%%<color #aaaaaa> ", (bBoost?(1+fBoosted):1)*fMag*100 );
        return;
    }
    else
    {
        estrConcatStaticCharArray(str, "<color #ffffff>"); // No specific color
    }

    if( pchMag )
    {
        estrConcatf(str, "%s<color #aaaaaa>", pchMag );
        return;
    }
    
    switch( eType )
    {
        xcase kAttribType_Cur:
        {
            if( bBoth )
                estrConcatf(str, "%+.2f%%%% (%+.2f%%%%)<color #aaaaaa> ", fMag*100*(1+fBoosted), fMag*100 );
            else
                estrConcatf(str, "%+.2f%%%%<color #aaaaaa> ", (bBoost?(1+fBoosted):1)*fMag*100 );
        }
        xcase kAttribType_Str:
        {
            if( bBoth )
                estrConcatf(str, "%+.2f%%%% (%+.2f%%%%)<color #aaaaaa> ", fMag*100*(1+fBoosted), fMag*100 );
            else
                estrConcatf(str, "%+.2f%%%%<color #aaaaaa> ", (bBoost?(1+fBoosted):1)*fMag*100 );
        }
        xcase kAttribType_Res:
        {
            if( bBoth )
                estrConcatf(str, "%.2f%%%% (%.2f%%%%)<color #aaaaaa> ", fMag*(1+fBoosted)*100, fMag*100 );
            else
                estrConcatf(str, "%.2f%%%%<color #aaaaaa> ", (bBoost?(1+fBoosted):1)*fMag*100 );
        }
        xcase kAttribType_Max:
        {
            if( bBoth )
                estrConcatf(str, "%.2f (%.2f)<color #aaaaaa> ", fMag*(1+fBoosted), fMag );
            else
                estrConcatf(str, "%.2f<color #aaaaaa> ", (bBoost?(1+fBoosted):1)*fMag );
        }
        xcase kAttribType_Mod:
        {
            if( bBoth )
                estrConcatf(str, "%.2f (%.2f)<color #aaaaaa> ", fMag*(1+fBoosted), fMag );
            else
                estrConcatf(str, "%.2f<color #aaaaaa> ", (bBoost?(1+fBoosted):1)*fMag );
        }
        xcase kAttribType_Abs:
        {
            if( bBoth )
                estrConcatf(str, "%.2f (%.2f)<color #aaaaaa> ", fMag*(1+fBoosted), fMag );
            else
                estrConcatf(str, "%.2f<color #aaaaaa> ", (bBoost?(1+fBoosted):1)*fMag );
        }
    }
}

static void  power_AddPetEffects( char ** str, const AttribModTemplate *pTemplate, const CharacterAttributes *pAttr, const CharacterClass *creatorClass, int iLevel, int ShowEnhance, int ShowPvP );
static void power_AddPets( char ** str, const BasePower * pPowBase, const CharacterAttributes *pAttr, const CharacterClass *creatorClass, int iLevel, int ShowEnhance, int ShowPvP, int hidden );
static int iMarkedOpenPowers;
static int s_iPowCnt;
void markPowerOpen(int idx)
{
    if(idx >= 0 && idx <= 32 )
        iMarkedOpenPowers |= (1<<idx);
}

void markPowerClosed(int idx)
{
    if(idx >= 0 && idx <= 32 )
        iMarkedOpenPowers &= ~(1<<idx);
}

static char * translateEffect( char * pchChanceForMiss, char * pchTicksOf, char * pchChanceForMiss2, char *pchValue, char * pchType, char * pchDuration, char * pchOverTime, char *pchTarget, char *pchDelay )
{
    if( pchChanceForMiss )
    {
        if( pchTicksOf )
        {
            if( pchChanceForMiss2 )
            {
                if( pchOverTime && pchDelay )
                    return textStd( "ChanceForMiss_Ticksof_ChanceForMiss2_Value_Type_OverTime_Target_AfterDelay", pchChanceForMiss, pchTicksOf, pchChanceForMiss2, pchValue, pchType, pchOverTime, pchTarget, pchDelay );
                else if( pchOverTime )
                    return textStd( "ChanceForMiss_TicksOf_ChanceForMiss2_Value_Type_OverTime_Target", pchChanceForMiss, pchTicksOf, pchChanceForMiss2, pchValue, pchType, pchOverTime, pchTarget );
                else if( pchDelay )
                    return textStd( "ChanceForMiss_TicksOf_ChanceForMiss2_Value_Type_Target_AfterDelay", pchChanceForMiss, pchTicksOf, pchChanceForMiss2, pchValue, pchType, pchTarget, pchDelay );
                else
                    return textStd( "ChanceForMiss_TicksOf_ChanceForMiss2_Value_Type_Target", pchChanceForMiss, pchTicksOf, pchChanceForMiss2, pchValue, pchType, pchTarget );
            }
            else if( pchOverTime && pchDelay )
                return textStd( "ChanceForMiss_Ticksof_Value_Type_OverTime_Target_AfterDelay", pchChanceForMiss, pchTicksOf, pchValue, pchType, pchOverTime, pchTarget, pchDelay );
            else if( pchOverTime )
                return textStd( "ChanceForMiss_TicksOf_Value_Type_OverTime_Target", pchChanceForMiss, pchTicksOf, pchValue, pchType, pchOverTime, pchTarget );
            else if( pchDelay )
                return textStd( "ChanceForMiss_TicksOf_Value_Type_Target_AfterDelay", pchChanceForMiss, pchTicksOf, pchValue, pchType, pchTarget, pchDelay );
            else
                return textStd( "ChanceForMiss_TicksOf_Value_Type_Target", pchChanceForMiss, pchTicksOf, pchValue, pchType, pchTarget );
        }
        else if( pchDuration && pchDelay )
            return textStd( "ChanceForMiss_Value_Type_ForDuration_Target_AfterDelay", pchChanceForMiss, pchValue, pchType, pchDuration, pchTarget, pchDelay );
        else if ( pchDuration )
            return textStd( "ChanceForMiss_Value_Type_ForDuration_Target", pchChanceForMiss, pchValue, pchType, pchDuration, pchTarget );
        else if ( pchDelay )
            return textStd( "ChanceForMiss_Value_Type_Target_AfterDelay", pchChanceForMiss, pchValue, pchType, pchTarget, pchDelay );
        else
            return textStd( "ChanceForMiss_Value_Type_Target", pchChanceForMiss, pchValue, pchType, pchTarget, pchDelay );
    }
    else if( pchTicksOf )
    {
        if( pchChanceForMiss2 )
        {
            if( pchOverTime && pchDelay )
                return textStd( "Ticksof_ChanceForMiss2_Value_Type_OverTime_Target_AfterDelay", pchTicksOf, pchChanceForMiss2, pchValue, pchType, pchOverTime, pchTarget, pchDelay );
            else if( pchOverTime )
                return textStd( "TicksOf_ChanceForMiss2_Value_Type_OverTime_Target", pchTicksOf, pchChanceForMiss2, pchValue, pchType, pchOverTime, pchTarget );
            else if( pchDelay )
                return textStd( "TicksOf_ChanceForMiss2_Value_Type_Target_AfterDelay", pchTicksOf, pchChanceForMiss2, pchValue, pchType, pchTarget, pchDelay );
            else
                return textStd( "TicksOf_ChanceForMiss2_Value_Type_Target", pchTicksOf, pchChanceForMiss2, pchValue, pchType, pchTarget );
        }
        else if( pchOverTime && pchDelay )
            return textStd( "Ticksof_Value_Type_OverTime_Target_AfterDelay", pchTicksOf, pchValue, pchType, pchOverTime, pchTarget, pchDelay );
        else if( pchOverTime )
            return textStd( "TicksOf_Value_Type_OverTime_Target", pchTicksOf, pchValue, pchType, pchOverTime, pchTarget );
        else if( pchDelay )
            return textStd( "TicksOf_Value_Type_Target_AfterDelay", pchTicksOf, pchValue, pchType, pchTarget, pchDelay );
        else
            return textStd( "TicksOf_Value_Type_Target", pchTicksOf, pchValue, pchType, pchTarget );
    }
    else if( pchDuration && pchDelay )
        return textStd( "Value_Type_ForDuration_Target_AfterDelay", pchValue, pchType, pchDuration, pchTarget, pchDelay );
    else if ( pchDuration )
        return textStd( "Value_Type_ForDuration_Target", pchValue, pchType, pchDuration, pchTarget );
    else if ( pchDelay )
        return textStd( "Value_Type_Target_AfterDelay", pchValue, pchType, pchTarget, pchDelay );
    else
        return textStd( "Value_Type_Target", pchValue, pchType, pchTarget );
}

static void power_AddEffects( const BasePower *ppow, const CharacterAttributes *pAttr, const CharacterClass *pClass, const CharacterClass *creatorClass, int iLevel, char **str, bool bShowEnhance, const VillainDef * pDef, F32 fPetDuration, bool bShowPvP )
{
    int i;
    const char * pRequires;
    const char * pChanceMod;

    if( bShowEnhance && !pAttr )
        bShowEnhance = 0;

    for(i=0;i<eaSize(&ppow->ppTemplates);i++)
        power_AddPetEffects( str, ppow->ppTemplates[i], pAttr, pClass, iLevel, bShowEnhance, bShowPvP );

    markAllTypes(ppow, pClass->pchName, creatorClass ? creatorClass->pchName : NULL, bShowPvP);
    for(i=0;i<eaSize(&ppow->ppTemplates);i++)
    {
        const AttribModTemplate *pTemplate = ppow->ppTemplates[i];
        int j, aspect = pTemplate->offAspect, eTarget = pTemplate->eTarget;
        int offset = pTemplate->offAttrib, iHash = modGetBuffHash( pTemplate );
        AttribDescription *pDesc = attribDescriptionGet( iHash );
        AttribType eType;
        F32 fMag, fDuration, fChance = pTemplate->fChance*100.f;
        bool bDisplay = 0;
        char *estr, *attribName = getAttribNameForTranslation(offset);
        int iTicks = 0;
        bool bPvPEffect = 0;
        char * pchMag = 0;
        char *pchChanceForMiss=0, *pchTicksOf=0, *pchChanceForMiss2=0, *pchValue=0, *pchType=0, *pchDuration=0, *pchOverTime=0, *pchTarget=0, *pchDelay=0; 

        F32 *fBoosted = pAttr?ATTRIB_GET_PTR(pAttr, pTemplate->offAttrib):0;

        pChanceMod = getChanceModString(pTemplate->pchName);
        if(!pTemplate->fChance && !strlen(pChanceMod)) // don't show items with no chance which don't have a known GlobalChanceMod type
            continue;
        else if(strlen(pChanceMod))
            fChance = getChanceModValue(pTemplate->pchName)*100.f; // fake the percentages for GlobalChanceMod attrib mods

        if( pDef && pTemplate->eTarget == kModTarget_Caster ) // don't show effects a pet does to itself yet
            continue;

         if( eaSize(&pTemplate->ppchApplicationRequires) && !requiresArchetypePvPEval( pTemplate->ppchApplicationRequires, pClass->pchName, creatorClass ? creatorClass->pchName : NULL, bShowPvP ) )
            continue;



        estrCreate(&estr); 
        for( j = 0; j < eaSize(&pTemplate->ppchApplicationRequires); j++ )
            estrConcatCharString(&estr, pTemplate->ppchApplicationRequires[j] );

        if( offset == kSpecialAttrib_GrantPower || offset == kSpecialAttrib_GrantBoostedPower )
        {
            const BasePower *pPowerGrant = powerdict_GetBasePowerByFullName(&g_PowerDictionary, pTemplate->pchReward);
            if( pPowerGrant && pPowerGrant->eShowInInventory != kShowPowerSetting_Never ) // blatant hack, since I don't have the Character....
            {
                if(  iMarkedOpenPowers & (1<<s_iPowCnt) )
                {
                    const CharacterClass * pTempClass = pClass;
                    estrConcatf( str, "<linkhoverbg #55555555><linkhover #a7ff6c><link #52ff7d><a href='cmd:markPowClosed %i'>-", s_iPowCnt  );

                    if( pTemplate->ppchApplicationRequires && pTemplate->ppchApplicationRequires[0] ) // this is granted to a pet...probably
                    {
                        const VillainDef *pDef = villainFindByName(pTemplate->ppchApplicationRequires[0]);
                        if( pDef )
                        {
                            const char* villainTargetName = GetNameofVillainTarget(pDef, 0);
                            devassertmsg(villainTargetName, "The displayname for a pet target was not set!");
                            pTempClass = classes_GetPtrFromName(&g_VillainClasses, pDef->characterClassName);
                            estrConcatCharString(str, textStd("PowerGrantTarget", pPowerGrant->pchDisplayName, villainTargetName ? villainTargetName : pTemplate->ppchApplicationRequires[0]));
                        }
                        else    // maybe this is one of those powers which grants a power under specific conditions like Power Siphon
                        {
                            const char * pGrantRequires;
                            pGrantRequires = getGrantRequiresString(estr);
                            if(pGrantRequires && strlen(pGrantRequires))
                                estrConcatCharString(str, textStd("PowerGrantRequires", pPowerGrant->pchDisplayName, pGrantRequires));
                        }
                    }
                    else
                        estrConcatCharString(str, textStd("PowerGrant", pPowerGrant->pchDisplayName));

                    estrConcatStaticCharArray(str, "</a><br></linkhover></linkhoverbg></linkbg></link><scale .8f><table border=5><tr><td>");
                    power_AddBaseStats( pPowerGrant, pAttr, pTempClass, creatorClass, iLevel, str, bShowEnhance, bShowPvP, 0);
                    estrConcatStaticCharArray(str, "<br>");
                    power_AddTargetInfo( pPowerGrant, pAttr, str, bShowEnhance );
                    estrConcatStaticCharArray(str, "<br><br>");
                    power_AddEffects( pPowerGrant, pAttr,  pTempClass, creatorClass, iLevel, str, bShowEnhance, 0, 0, bShowPvP);
                    estrConcatStaticCharArray(str, "<br>");
                    power_AddPets(str, pPowerGrant, pAttr, creatorClass, iLevel, bShowEnhance, bShowPvP, 0 );
                    estrConcatStaticCharArray(str, "</td></tr></table></scale>");
                }
                else
                {
                    if( pTemplate->ppchApplicationRequires && pTemplate->ppchApplicationRequires[0] )
                    {
                        const VillainDef *pDef = villainFindByName(pTemplate->ppchApplicationRequires[0]);
                        if(pDef)
                        {
                            const char* villainTargetName = GetNameofVillainTarget(pDef, 0);
                            devassertmsg(villainTargetName, "The displayname for a pet target was not set!");                        
                            estrConcatf( str, "<linkhoverbg #55555555><linkhover #a7ff6c><link #52ff7d><a href='cmd:markPowOpen %i'>+%s</a><br></linkhover></linkhoverbg></linkbg></link>", s_iPowCnt, textStd("PowerGrantTarget", pPowerGrant->pchDisplayName, villainTargetName ? villainTargetName : pTemplate->ppchApplicationRequires[0] ));
                        }
                        else
                        {
                            const char * pGrantRequires;
                            pGrantRequires = getGrantRequiresString(estr);
                            if(pGrantRequires && strlen(pGrantRequires))
                                estrConcatf( str, "<linkhoverbg #55555555><linkhover #a7ff6c><link #52ff7d><a href='cmd:markPowOpen %i'>+%s</a><br></linkhover></linkhoverbg></linkbg></link>", s_iPowCnt, textStd("PowerGrantRequires", pPowerGrant->pchDisplayName, pGrantRequires) );
                        }
                    }
                    else
                        estrConcatf( str, "<linkhoverbg #55555555><linkhover #a7ff6c><link #52ff7d><a href='cmd:markPowOpen %i'>+%s</a><br></linkhover></linkhoverbg></linkbg></link>", s_iPowCnt, textStd("PowerGrant", pPowerGrant->pchDisplayName) );
                }
                s_iPowCnt++;
            }

            estrDestroy(&estr);
            continue;
        }
        pRequires = getRequiresString(estr, pClass, pDef?1:0, 0);
        estrClear(&estr);

        for( j = 0; j < eaSize(&pTemplate->ppchMagnitude); j++ )
            estrConcatCharString(&estr, pTemplate->ppchMagnitude[j]);
        if( stricmp( estr, "60kHitPointssource>-0100minmax60/0.2*" ) == 0 )
            pchMag = textStd( "VariesOnHitpoints" );
        else if( stricmp( estr, "75kHitPointssource>-0100minmax60/0.5*" ) == 0 )
            pchMag = textStd( "VariesOnHitpoints2" );
        else if( strlen(estr) > 0 )
            pchMag = textStd( "SpecialString" );
        estrDestroy(&estr);

        if( !attribName
            || stricmp(attribName, "AttrUnknown") == 0
            || (offset >= kSpecialAttrib_Translucency && offset != kSpecialAttrib_DropToggles )
            || offset == offsetof(CharacterAttributes, fMovementControl)
            || offset == offsetof(CharacterAttributes, fMovementFriction)
            || offset == offsetof(CharacterAttributes, fNullBool)
            || (offset == offsetof(CharacterAttributes, fStealthRadius) && bShowPvP)
            || (offset == offsetof(CharacterAttributes, fStealthRadiusPlayer) && !bShowPvP))
            continue;

        modGetMagnitudeAndDuration(pTemplate, pClass, iLevel, &fMag, &fDuration );

        if(aspect==offsetof(CharacterAttribSet, pattrMod))
            eType = kAttribType_Cur;
        if(aspect==offsetof(CharacterAttribSet, pattrMax))
            eType = kAttribType_Max;
        if(aspect==offsetof(CharacterAttribSet, pattrStrength))
            eType = kAttribType_Str;
        if(aspect==offsetof(CharacterAttribSet, pattrResistance))
            eType = kAttribType_Res;
        if(aspect==offsetof(CharacterAttribSet, pattrAbsolute))
            eType = kAttribType_Abs;

        if( fChance != 100.f && (!pTemplate->fDuration || !pTemplate->bCancelOnMiss) )
            pchChanceForMiss = textStd("ChanceForMiss", fChance);

        if(offset >= (int)offsetof(CharacterAttributes, fDamageType) && offset < (int)offsetof(CharacterAttributes, fHitPoints))
        {

            if( equalForAllTypes[eTarget][kGroupType_Damage][eType] > 1  )
            {
                displayedForAllTypes[eTarget][kGroupType_Damage][eType]++;
                if(displayedForAllTypes[eTarget][kGroupType_Damage][eType] >= damageTypesCount)
                {
                    if(--equalSetsForAllTypes[eTarget][kGroupType_Damage][eType] > 0)
                    {
                        equalForAllTypes[eTarget][kGroupType_Damage][eType] = 1;
                        displayedForAllTypes[eTarget][kGroupType_Damage][eType] = 0;
                    }
                }
                continue;
            }
            else if( equalForAllTypes[eTarget][kGroupType_Damage][eType] == 1 )
            {
                equalForAllTypes[eTarget][kGroupType_Damage][eType]++;
                displayedForAllTypes[eTarget][kGroupType_Damage][eType]++;
            }

            if(eType == kAttribType_Abs)
            {
                if(pDef && fPetDuration && ppow->fActivatePeriod)
                    iTicks = (int)(floor(fPetDuration/ppow->fActivatePeriod)+1);
                else if( fDuration )
                    iTicks = (int)floor( fDuration / pTemplate->fPeriod )+1;

                if(iTicks > 1)
                {
                    pchTicksOf = textStd( "TicksOf", iTicks );
                    if( fChance != 100.f && pTemplate->bCancelOnMiss )
                        pchChanceForMiss2 = textStd("ChanceForMiss", fChance);                    
                }
            }
        }
        else if(offset >= (int)offsetof(CharacterAttributes, fDefenseType)    && offset < (int)offsetof(CharacterAttributes, fDefense))
        {
            if( equalForAllTypes[eTarget][kGroupType_Defense][eType] > 1  )
            {
                displayedForAllTypes[eTarget][kGroupType_Defense][eType]++;
                if(displayedForAllTypes[eTarget][kGroupType_Defense][eType] >= defenseTypesCount)
                {
                    if(--equalSetsForAllTypes[eTarget][kGroupType_Defense][eType] > 0)
                    {
                        equalForAllTypes[eTarget][kGroupType_Defense][eType] = 1;
                        displayedForAllTypes[eTarget][kGroupType_Defense][eType] = 0;
                    }
                }
                continue;
            }
            else if( equalForAllTypes[eTarget][kGroupType_Defense][eType] == 1 )
            {
                equalForAllTypes[eTarget][kGroupType_Defense][eType]++;
                displayedForAllTypes[eTarget][kGroupType_Defense][eType]++;
            }
        }

        estrCreate(&pchValue);
        addAttribValue(&pchValue, eType, pTemplate, fMag, fBoosted?*fBoosted:0, bShowEnhance, pchMag );

        // DAMAGE TYPES
        if(offset >= (int)offsetof(CharacterAttributes, fDamageType) && offset < (int)offsetof(CharacterAttributes, fHitPoints))
        {
            switch(eType)
            {
                xcase kAttribType_Str:
                {
                    if( equalForAllTypes[eTarget][kGroupType_Damage][eType] )
                        pchType = textStd("GenericStr", "AllDamage");
                    else
                        pchType = textStd("GenericStr", attribName );
                }
                xcase kAttribType_Cur:
                {
                    if( equalForAllTypes[eTarget][kGroupType_Damage][eType] )
                        pchType =  textStd("GenericCur", "AllDamage");
                    else
                        pchType =  textStd("GenericCur", attribName );
                }
                xcase kAttribType_Res:
                {
                    if( equalForAllTypes[eTarget][kGroupType_Damage][eType] )
                        pchType = textStd("GenericRes", "AllResistance" );
                    else
                        pchType = textStd("GenericRes", attribName );
                }
                xcase kAttribType_Abs:
                {    
                    if( offset == offsetof(CharacterAttributes, fDamageType[7]) )
                        pchType = textStd( "HealDirect");
                    else
                        pchType =  textStd( "DamageDirect", attribName);
                }
            }
        }
        // DEFENSE TYPES
        else if(offset >= (int)offsetof(CharacterAttributes, fDefenseType)    && offset < (int)offsetof(CharacterAttributes, fDefense))
        {
            switch(eType)
            {
                xcase kAttribType_Cur:
                {
                    if( equalForAllTypes[eTarget][kGroupType_Defense][eType] )
                        pchType =  textStd("GenericStr", "AllDefense");
                    else
                        pchType = textStd("GenericStr", attribName );
                }
                xcase kAttribType_Str:
                {
                    if( equalForAllTypes[eTarget][kGroupType_Defense][eType] )
                        pchType = textStd("GenericStr", "AllDefense");
                    else
                        pchType = textStd("GenericStr", attribName );
                }
            }
        }
        // STATUS EFFECTS
        else if( (offset >= (int)offsetof(CharacterAttributes, fConfused) && offset <= (int)offsetof(CharacterAttributes, fOnlyAffectsSelf)) ||
                 (offset >= (int)offsetof(CharacterAttributes, fKnockup) && offset <= (int)offsetof(CharacterAttributes, fRepel)) )
        {
            switch(eType)
            {
                xcase kAttribType_Cur:
                {
                    if( fMag < 0 )
                        pchType = textStd( "StatusProtection", attribName);
                    else
                        pchType = textStd( "StatusDealt", attribName);
                }
                xcase kAttribType_Str:
                {
                    pchType = textStd("GenericStr", attribName );
                }
                xcase kAttribType_Res:
                {
                    pchType = textStd("GenericRes", attribName );
                }
            }
        }
        else
        {
            switch(eType)
            {
                xcase kAttribType_Abs:
                {
                    pchType = textStd("GenericAbs", attribName );
                }
                xcase kAttribType_Cur:
                {
                    pchType = textStd("GenericCur", attribName );
                }
                xcase kAttribType_Res:
                {
                    pchType = textStd("GenericRes", attribName );
                }
                xcase kAttribType_Max:
                {
                    pchType = textStd("GenericMax", attribName );
                }
                xcase kAttribType_Str:
                {
                    pchType = textStd("GenericStr", attribName );
                }
            }
        }

        if (pDef && fPetDuration && 
            (!pTemplate->ppSuppress || !eaSize(&pTemplate->ppSuppress))) // Assume everything that suppresses will suppress later instances of itself
        {
            if(iTicks > 1) // only true for damage mods
            {
                fDuration = fPetDuration;
            }
            else
            {
                fDuration = fDuration * (((int) (fPetDuration / fDuration)) + 1);
            }
        }

        if( iTicks > 1 )
        {
            pchOverTime = textStd( "OverTime", getPrettyDuration(fDuration) );
        }
        else if( (pDef || ppow->eType == kPowerType_Click) && fDuration )
        {
            estrCreate(&pchDuration);
            if( bShowEnhance && fBoosted && eType == kAttribType_Cur && // Boosted hold effects increase the duration not magnitude
                (offset >= (int)offsetof(CharacterAttributes, fConfused) && offset <= (int)offsetof(CharacterAttributes, fOnlyAffectsSelf)) )
            {
                estrConcatCharString(&pchDuration, textStd("ForDurationBoosted", getPrettyDuration(fDuration*(1+*fBoosted))));
                estrConcatf( &pchDuration, "(%s)<color #aaaaaa>", getPrettyDuration(fDuration) );
            }
            else
                estrConcatCharString(&pchDuration, textStd("ForDuration", getPrettyDuration(fDuration)));
        }

        if( pTemplate->eTarget == kModTarget_Caster )
            pchTarget = textStd("OnSelf");
        else
            pchTarget = textStd("OnTarget");

        if( pTemplate->fDelay )
            pchDelay = textStd("AfterDelay", getPrettyDuration(pTemplate->fDelay));


        estrConcatCharString(str, translateEffect( pchChanceForMiss, pchTicksOf, pchChanceForMiss2, pchValue, pchType, pchDuration, pchOverTime, pchTarget, pchDelay));
        estrDestroy(&pchValue);
        if( pchDuration )
            estrDestroy(&pchDuration);

        if(pChanceMod)
            estrConcatCharString(str, pChanceMod);
        if(pRequires)
            estrConcatCharString(str, pRequires);
        if( !pTemplate->bAllowStrength )
            estrConcatf( str, "<br>&nbsp;&nbsp;&nbsp;&nbsp;<i><color #aaaa88>%s</i>", textStd( "IgnoresBuffsAndEnhancements" ) );
        if( !pTemplate->bAllowResistance )
            estrConcatf( str, "<br>&nbsp;&nbsp;&nbsp;&nbsp;<i><color #aaaa88>%s</i>", textStd( "Unresistable" ) );

        if( 0 && isDevelopmentMode() )
        {
            switch(  pTemplate->eStack )
            {
                xcase kStackType_Stack: estrConcatStaticCharArray(str, " <color #111111>(Stacks)</color><br>");
                xcase kStackType_Ignore: estrConcatStaticCharArray(str, " <color #111111>(Ignores Duplicates)</color><br>");
                xcase kStackType_Extend: estrConcatStaticCharArray(str, " <color #111111>(Updates)</color><br>");
                xcase kStackType_Replace: estrConcatStaticCharArray(str, " <color #111111>(Updates and Replaces)</color><br>");
            }
        }
        else
            estrConcatStaticCharArray(str, "<br><br>");
    }
}

static void  power_AddPetEffects( char ** str, const AttribModTemplate *pTemplate, const CharacterAttributes *pAttr, const CharacterClass *creatorClass, int iLevel, int ShowEnhance, int ShowPvP )
{
    int i;

    if( pTemplate->offAttrib==kSpecialAttrib_EntCreate )
    {
        const VillainDef *pDef = villainFindByName(pTemplate->pchEntityDef);
        
        if( pDef && pet_IsAutoOnly(pDef) )
        {
            const CharacterClass * villianClass = classes_GetPtrFromName(&g_VillainClasses, pDef->characterClassName);
            for( i = eaSize(&pDef->powers)-1; i>=0; i-- )
            {
                const PowerCategory*  category;
                const BasePowerSet*   basePowerSet;
                const BasePower*    basePower;

                category = powerdict_GetCategoryByName(&g_PowerDictionary, pDef->powers[i]->powerCategory);
                if(!category)
                    continue;
                if((basePowerSet = category_GetBaseSetByName(category, pDef->powers[i]->powerSet)) == NULL)
                    continue;
                if( pDef->powers[i]->power[0] == '*' )
                {
                    int j;
                    for( j = 0; j < eaSize(&basePowerSet->ppPowers); j++ )
                        power_AddEffects( basePowerSet->ppPowers[j], pAttr, villianClass, creatorClass, iLevel, str, ShowEnhance, pDef, pTemplate->fDuration, ShowPvP );
                }
                else
                {
                    basePower = baseset_GetBasePowerByName(basePowerSet, pDef->powers[i]->power);
                    if(basePower)
                        power_AddEffects( basePower, pAttr, villianClass, creatorClass, iLevel, str, ShowEnhance, pDef, pTemplate->fDuration, ShowPvP );
                }
            }
        }
    }
}



static void power_AddPets( char ** str, const BasePower * pPowBase, const CharacterAttributes *pAttr, const CharacterClass *creatorClass, int iLevel, int ShowEnhance, int ShowPvP, int hidden )
{
    int i, j;

    // next list all pets that do stuff, and what that stuff is
    for(i=0;i<eaSize(&pPowBase->ppTemplates);i++)
    {
        if( pPowBase->ppTemplates[i]->offAttrib==kSpecialAttrib_EntCreate )
        {
            const VillainDef *pDef = villainFindByName(pPowBase->ppTemplates[i]->pchEntityDef);
            
            if( pDef && !pet_IsAutoOnly(pDef) )
            {
                const CharacterClass * villianClass = classes_GetPtrFromName(&g_VillainClasses, pDef->characterClassName);
                estrConcatf( str, "<br><color #ffffff>%s %s", textStd("CreatesPet"), textStd(pDef->levels[0]->displayNames[0]) );

                 if( pPowBase->ppTemplates[i]->fDuration && pPowBase->ppTemplates[i]->fDuration < 9999 )
                    estrConcatf(str, textStd("ForDuration", getPrettyDuration(pPowBase->ppTemplates[i]->fDuration)));

                estrConcatf( str, "<br><color #aaaaaa>%s: <color #ffffff>%.2f<br>", textStd("BaseHitpoints"), villianClass->pattrMax[iLevel].fHitPoints );

                if( eaSize(&pDef->powers)  )
                {
                    for( j = eaSize(&pDef->powers)-1; j>=0; j-- )
                    {
                        const PowerCategory*  category;
                        const BasePowerSet*   basePowerSet;
                        const BasePower *basePower;

                        category = powerdict_GetCategoryByName(&g_PowerDictionary, pDef->powers[j]->powerCategory);
                        if(!category)
                            continue;

                        if((basePowerSet = category_GetBaseSetByName(category, pDef->powers[j]->powerSet)) == NULL)
                            continue;

                        if( pDef->powers[j]->power[0] == '*' )
                        {
                            int k;
                            for( k = 0; k < eaSize(&basePowerSet->ppPowers); k++ )
                            {
                                if( basePowerSet->ppPowers[k]->eShowInInventory == kShowPowerSetting_Never ) // blatant hack, since I don't have the Character....
                                    continue;

                                if(!hidden)
                                {
                                    if( iMarkedOpenPowers & (1<<s_iPowCnt) )
                                    {
                                        estrConcatf( str, "<linkhoverbg #55555555><linkhover #a7ff6c><link #52ff7d><a href='cmd:markPowClosed %i'>-%s</a><br></linkhover></linkhoverbg></linkbg></link><scale .8f>", s_iPowCnt, textStd(basePowerSet->ppPowers[k]->pchDisplayName) );
                                        
                                        estrConcatStaticCharArray(str, "<table border=5><tr><td>");
                                        power_AddBaseStats( basePowerSet->ppPowers[k], pAttr, villianClass, creatorClass, iLevel, str, ShowEnhance, ShowPvP, 1);
                                        estrConcatStaticCharArray(str, "<br>");
                                        power_AddTargetInfo( basePowerSet->ppPowers[k], pAttr, str, ShowEnhance );
                                        estrConcatStaticCharArray(str, "<br><br>");
                                        power_AddEffects( basePowerSet->ppPowers[k], pAttr,  villianClass, creatorClass, iLevel, str, ShowEnhance, 0, 0, ShowPvP);
                                        estrConcatStaticCharArray(str, "<br>");
                                        power_AddPets(str, basePowerSet->ppPowers[k], pAttr, creatorClass, iLevel, ShowEnhance, ShowPvP, 0 );
                                        estrConcatStaticCharArray(str, "</td></tr></table></scale>");
                                    }
                                    else
                                    {
                                        power_AddPets(str, basePowerSet->ppPowers[k], pAttr, creatorClass, iLevel, ShowEnhance, ShowPvP, 1 );
                                        estrConcatf( str, "<linkhoverbg #55555555><linkhover #a7ff6c><link #52ff7d><a href='cmd:markPowOpen %i'>+%s</a><br></linkhover></linkhoverbg></linkbg></link>", s_iPowCnt, textStd(basePowerSet->ppPowers[k]->pchDisplayName) );
                                    }
                                }
                                s_iPowCnt++; // we are limited to a 4 level recursion
                            }
                        }
                        else
                        {
                            basePower = baseset_GetBasePowerByName(basePowerSet, pDef->powers[j]->power);
                            if(basePower && basePower->eShowInInventory != kShowPowerSetting_Never) // blatant hack, since I don't have the Character....
                            {
                                if(!hidden)
                                {
                                    if( iMarkedOpenPowers & (1<<s_iPowCnt) )
                                    {
                                        estrConcatf( str, "<linkhoverbg #55555555><linkhover #a7ff6c><link #52ff7d><a href='cmd:markPowClosed %i'>-%s</a><br></linkhover></linkhoverbg></linkbg></link><scale .8f>", s_iPowCnt, textStd(basePower->pchDisplayName) );
                                        
                                        estrConcatStaticCharArray(str, "<table border=5><tr><td>");
                                        power_AddBaseStats( basePower, pAttr, villianClass, creatorClass, iLevel, str, ShowEnhance, ShowPvP, 0);
                                        estrConcatStaticCharArray(str, "<br>");
                                        power_AddTargetInfo( basePower, pAttr, str, ShowEnhance );    
                                        estrConcatStaticCharArray(str, "<br>");
                                        power_AddEffects( basePower, pAttr, villianClass, creatorClass, iLevel, str, ShowEnhance, 0, 0, ShowPvP);
                                        estrConcatStaticCharArray(str, "<br>");
                                        power_AddPets(str, basePower, pAttr, creatorClass, iLevel, ShowEnhance, ShowPvP, 0 );
                                        estrConcatStaticCharArray(str, "</td></tr></table></scale>");
                                    }
                                    else
                                    {
                                        power_AddPets(str, basePower, pAttr, creatorClass, iLevel, ShowEnhance, ShowPvP, 1 );
                                        estrConcatf( str, "<linkhoverbg #55555555><linkhover #a7ff6c><link #52ff7d><a href='cmd:markPowClosed %i'>+%s</a><br></linkhover></linkhoverbg></linkbg></link>", s_iPowCnt, textStd(basePower->pchDisplayName) );
                                    }
                                }
                                s_iPowCnt++;
                            }
                        }
                    }
                }
            }
        }
    }
}


static F32 s_fLevel = -1.f;
void powerInfoSetLevel(int iLevel)
{
    if(s_fLevel = ((F32)(iLevel)/24.5)-1)
        s_bReformatInfo = 1;
    s_fLevel = ((F32)(iLevel)/24.5)-1;
}

static const CharacterClass *s_pClass=0;
void powerInfoSetClass(const CharacterClass *pClass)
{
    if( s_pClass != pClass )
        s_bReformatInfo = 1;
    s_pClass = pClass;
}

static ContextMenu * cmClassSelector;
static int powerInfo_cmAvailable(const CharacterClass *pClass){ return (pClass==s_pClass)?CM_CHECKED:CM_AVAILABLE; }
//static void powerInfo_cmSetClass(CharacterClass *pClass){ powerInfoSetClass( cpp_const_cast(const CharacterClass*)(pClass) ); }
void powerInfoClassSelector()
{
    int i, x, y;
    float xsc, ysc;
    

    if(!cmClassSelector)
    {
        cmClassSelector = contextMenu_Create(0);
        for( i = 0; i < eaSize(&g_CharacterClasses.ppClasses); i++ )
        {
            const CharacterClass *pClass = g_CharacterClasses.ppClasses[i];
            void* context_menu_data = cpp_const_cast(CharacterClass*)(pClass);
            contextMenu_addCheckBox( cmClassSelector, powerInfo_cmAvailable, context_menu_data, powerInfoSetClass, context_menu_data, pClass->pchDisplayName );
        }
    }

    inpMousePos(&x, &y);
    calc_scrn_scaling(&xsc, &ysc);
    contextMenu_set(cmClassSelector, x/xsc, y/ysc, 0 );
}


int addEnhanceStatLine( StuffBuff *buff, char *pchName, int resistance, char * pchExtra, F32 fBase, F32 fEnhance, F32 fReplace, F32 fCombine, int noPlus, int divide, int time )
{
    F32 fEnhanceVal = fBase*(1+fEnhance);
    F32 fReplaceVal = fBase*(1+fEnhance+fReplace);
    F32 fCombineVal = fBase*(1+fEnhance+fCombine);
    char *name;

    if(divide)
    {
        fEnhanceVal = fBase*(1/(1+fEnhance));
        fReplaceVal = fBase*(1/(1+fEnhance+fReplace));
        fCombineVal = fBase*(1/(1+fEnhance+fCombine));
    }

    addSingleStringToStuffBuff(buff,"<tr border=0>");
     name = textStd(pchName);
    name[0] = toupper(name[0]);
    if( !strchr(name, ':' ))
    {
        addStringToStuffBuff(buff, "<td>%s", name );
        if( resistance )
            addStringToStuffBuff(buff, " %s:</td>", textStd("ResistanceString"));
        else
            addStringToStuffBuff(buff, ":</td>", name );
    }
    else
    {
        addStringToStuffBuff(buff, "<td>%s", name );
        if( resistance )
            addStringToStuffBuff(buff, " %s</td>", textStd("ResistanceString"));
        else
            addStringToStuffBuff(buff, "</td>", name );
    }

    // Unenhanced
    if( time )
        addStringToStuffBuff(buff, "<td>%s</td>", getPrettyDuration(fBase) );
    else if( pchExtra )
        addStringToStuffBuff(buff, "<td>%.2f%s</td>", fBase, pchExtra );
    else
        addStringToStuffBuff(buff, "<td>%.2f</td>", fBase );

    // Current
    if( time )
    {        
        if( noPlus )
            addStringToStuffBuff(buff, "<td>%s (<color paragon>%.1f%%%%</color>)</td>", getPrettyDuration(fEnhanceVal), fEnhance*100.f );
        else
            addStringToStuffBuff(buff, "<td>%s (<color paragon>%+.1f%%%%</color>)</td>", getPrettyDuration(fEnhanceVal), fEnhance*100.f );
    }
    else if( pchExtra )
    {
        if( noPlus )
            addStringToStuffBuff(buff, "<td>%.2f%s (<color paragon>%.1f%%%%</color>)</td>", fEnhanceVal, pchExtra, fEnhance*100.f );
        else
            addStringToStuffBuff(buff, "<td>%.2f%s (<color paragon>%+.1f%%%%</color>)</td>", fEnhanceVal, pchExtra, fEnhance*100.f );
    }
    else
    {
        if( noPlus )
            addStringToStuffBuff(buff, "<td>%.2f (<color paragon>%.1f%%%%</color>)</td>", fEnhanceVal, fEnhance*100.f );
        else
            addStringToStuffBuff(buff, "<td>%.2f (<color paragon>%+.1f%%%%</color>)</td>", fEnhanceVal, fEnhance*100.f );
    }

    if(fReplace != 0.0f && fCombine == 0.0f )
    {
        if (fReplace >= 0.0f)
        {
            if( time )
                addStringToStuffBuff(buff, "<td>%s (<color #00ff00>%+.1f%%%%</color>)</td>", getPrettyDuration(fReplaceVal), fReplace*100.f );
            else if( pchExtra )
                addStringToStuffBuff(buff, "<td>%.2f%s (<color #00ff00>%+.1f%%%%</color>)</td>", fReplaceVal, pchExtra, fReplace*100.f );
            else
                addStringToStuffBuff(buff, "<td>%.2f (<color #00ff00>%+.1f%%%%</color>)</td>", fReplaceVal, fReplace*100.f );
        }
        else
        {
            if( time )
                addStringToStuffBuff(buff, "<td>%s (<color #00ff00>%+.1f%%%%</color>)</td>", getPrettyDuration(fReplaceVal), fReplace*100.f );
            else if( pchExtra )
                addStringToStuffBuff(buff, "<td>%.2f%s (<color #ff0000>%+.1f%%%%</color>)</td>", fReplaceVal, pchExtra, fReplace*100.f );
            else
                addStringToStuffBuff(buff, "<td>%.2f (<color #ff0000>%+.1f%%%%</color>)</td>", fReplaceVal, fReplace*100.f );
        }
    }
    if(fCombine != 0.0f)
    {
        if (fCombine >= 0.0f)
        {
            if( time )
                addStringToStuffBuff(buff, "<td>%s (<color #00ff00>%+.1f%%%%</color>)</td>", getPrettyDuration(fCombineVal), fCombine*100.f );
            else if( pchExtra )
                addStringToStuffBuff(buff, "<td>%.2f%s (<color #00ff00>%+.1f%%%%</color>)</td>", fCombineVal, pchExtra, fCombine*100.f );
            else
                addStringToStuffBuff(buff, "<td>%.2f (<color #00ff00>%+.1f%%%%</color>)</td>", fCombineVal, fCombine*100.f );
        }
        else
        {
            if( time )
                addStringToStuffBuff(buff, "<td>%s (<color #00ff00>%+.1f%%%%</color>)</td>", getPrettyDuration(fCombineVal), fCombine*100.f );
            else if( pchExtra )
                addStringToStuffBuff(buff, "<td>%.2f%s (<color #ff0000>%+.1f%%%%</color>)</td>", fCombineVal, pchExtra, fCombine*100.f );
            else
                addStringToStuffBuff(buff, "<td>%.2f (<color #ff0000>%+.1f%%%%</color>)</td>", fCombineVal, fCombine*100.f );
        }
    }
    addStringToStuffBuff(buff,"</tr>");
    return 1;
}

int power_addPetEnhanceStats( const AttribModTemplate *pTemplate, const BasePower *ppow, const CharacterClass *creatorClass, F32 baseVal, F32 combineVal, F32 replaceVal, StuffBuff * str, int noPlus, int offset )
{
    int i, found_attrib = 0;

    if( pTemplate->offAttrib==kSpecialAttrib_EntCreate )
    {
        const VillainDef *pDef = villainFindByName(pTemplate->pchEntityDef);
        if( pDef && pet_IsAutoOnly(pDef) )
        {
            const CharacterClass * villianClass = classes_GetPtrFromName(&g_VillainClasses, pDef->characterClassName);
            for( i = eaSize(&pDef->powers)-1; i>=0; i-- )
            {
                const PowerCategory*  category;
                const BasePowerSet*   basePowerSet;
                const BasePower*        basePower;

                category = powerdict_GetCategoryByName(&g_PowerDictionary, pDef->powers[i]->powerCategory);
                if(!category)
                    continue;
                if((basePowerSet = category_GetBaseSetByName(category, pDef->powers[i]->powerSet)) == NULL)
                    continue;
                if( pDef->powers[i]->power[0] == '*' )
                {
                    int j;
                    for( j = 0; j < eaSize(&basePowerSet->ppPowers); j++ )
                        found_attrib |= power_addEnhanceStats( basePowerSet->ppPowers[j], baseVal, combineVal, replaceVal, str, noPlus, offset, villianClass, creatorClass );
                }
                else
                {
                    basePower = baseset_GetBasePowerByName(basePowerSet, pDef->powers[i]->power);
                    if(basePower)
                        found_attrib |= power_addEnhanceStats( basePower, baseVal, combineVal, replaceVal, str, noPlus, offset, villianClass, creatorClass );
                }
            }
        }
    }

    return found_attrib;
}

int power_addEnhanceStats( const BasePower * pPowBase, F32 baseVal, F32 combineVal, F32 replaceVal, StuffBuff * str, int noPlus, int offset, const CharacterClass *petClass, const CharacterClass *creatorClass )
{
    Entity *e = playerPtr();
    const CharacterClass *pClass = petClass?petClass:e->pchar->pclass;
    int i, iLevel =    e->pchar->iCombatLevel, found_attrib = 0;
    
    // first the base non-template stuff
    // DAMAGE ----------------------------------------------------------------------------------------------------
    if(!petClass)
    {
        if(offset == offsetof(CharacterAttributes, fDamageType[0]) )
        {
            F32 fDamageBase, fDamageTotal;
            getTotalDamage( pPowBase, 0, pClass, creatorClass, iLevel, &fDamageBase, &fDamageTotal, 0 );

            if( fDamageBase )
                found_attrib |= addEnhanceStatLine( str, textStd("PowerInfoTotalDamage"), 0, 0, fDamageBase, baseVal, replaceVal, combineVal, noPlus, 0, 0 );
        }
        // ACCURACY ----------------------------------------------------------------------------------------------------
        else if(offset == offsetof(CharacterAttributes, fAccuracy) && pPowBase->fAccuracy)
        {
            found_attrib |= addEnhanceStatLine( str, textStd("PowerInfoAccuracy"), 0, "x", pPowBase->fAccuracy, baseVal, replaceVal, combineVal, noPlus, 0, 0 );
        }
        // RANGE ----------------------------------------------------------------------------------------------------
        else if(offset == offsetof(CharacterAttributes, fRange) && pPowBase->fRange )
        {
            if(!getCurrentLocale() )
                found_attrib |= addEnhanceStatLine( str, textStd("PowerInfoPowerRange"), 0, " ft.", pPowBase->fRange, baseVal, replaceVal, combineVal, noPlus, 0, 0 );
            else
                found_attrib |= addEnhanceStatLine( str, textStd("PowerInfoPowerRange"), 0, " m.", pPowBase->fRange*0.3048f, baseVal, replaceVal, combineVal, noPlus, 0, 0 );
        }
        // ENDURANCECOST ----------------------------------------------------------------------------------------------------
        else if(offset == offsetof(CharacterAttributes, fEnduranceDiscount) && pPowBase->fEnduranceCost )
        {
            if( pPowBase->eType == kPowerType_Toggle )
                found_attrib |= addEnhanceStatLine( str, textStd("PowerInfoEnduranceCost"), 0, "/s", pPowBase->fEnduranceCost/pPowBase->fActivatePeriod, baseVal, replaceVal, combineVal, noPlus, 1, 0 );
            else
                found_attrib |= addEnhanceStatLine( str, textStd("PowerInfoEnduranceCost"), 0, 0, pPowBase->fEnduranceCost, baseVal, replaceVal, combineVal, noPlus, 1, 0 );
        }
        // RECHARGE TIME ----------------------------------------------------------------------------------------------------
        else if(offset == offsetof(CharacterAttributes, fRechargeTime) && pPowBase->fRechargeTime )
        {
            found_attrib |= addEnhanceStatLine( str, textStd("PowerInfoRechargeTime"), 0, "s", pPowBase->fRechargeTime, baseVal, replaceVal, combineVal, noPlus, 1, 1 );
        }
        // INTERRUPT TIME -----------------------------------------------------------------------
        else if(offset == offsetof(CharacterAttributes, fInterruptTime) && pPowBase->fInterruptTime )
        {
            found_attrib |= addEnhanceStatLine( str, textStd("PowerInfoInteruptTime"), 0, "s", pPowBase->fInterruptTime, baseVal, replaceVal, combineVal, noPlus, 1, 1 );
        }
    }

    for(i=0;i<eaSize(&pPowBase->ppTemplates);i++)
        found_attrib |= power_addPetEnhanceStats( pPowBase->ppTemplates[i], pPowBase, creatorClass, baseVal, combineVal, replaceVal, str, noPlus, offset );

    // go over all the mods now and look for specific matches
    markAllTypes(pPowBase, pClass->pchName, creatorClass ? creatorClass->pchName : NULL, 0 );
    for(i=0;i<eaSize(&pPowBase->ppTemplates);i++)
    {
        const AttribModTemplate *pTemplate = pPowBase->ppTemplates[i];
        F32 fMag, fDuration;
        AttribType eType;
        int eTarget = pPowBase->ppTemplates[i]->eTarget;
        int iOff, j;
        char * pchAttrib, *estr;
        const char* pRequires;

         // look for any kind of damage or defense
        if( (offset==offsetof(CharacterAttributes, fDamageType) && (pTemplate->offAttrib>=offsetof(CharacterAttributes, fDamageType) && pTemplate->offAttrib<offsetof(CharacterAttributes, fHitPoints))) ||
            (offset==offsetof(CharacterAttributes, fDefense) && (pTemplate->offAttrib>=offsetof(CharacterAttributes, fDefenseType)    && pTemplate->offAttrib<offsetof(CharacterAttributes, fDefense))) ||
            (offset==offsetof(CharacterAttributes, fDamageType[7] ) && (pTemplate->offAttrib==offsetof(CharacterAttributes, fRegeneration) || pTemplate->offAttrib==offsetof(CharacterAttributes, fHitPoints))) )
        {
            iOff = pTemplate->offAttrib;
        }
        else
            iOff = offset;

        pchAttrib = getAttribNameForTranslation(iOff);
        if(iOff != pTemplate->offAttrib || !pTemplate->bAllowStrength )
            continue;

        if( eaSize(&pTemplate->ppchApplicationRequires) && !requiresArchetypePvPEval( pTemplate->ppchApplicationRequires, pClass->pchName, creatorClass ? creatorClass->pchName : NULL, 0 ) )
            continue;

        // don't bother with special stuff
        estrCreate(&estr);
        for( j = 0; j < eaSize(&pTemplate->ppchApplicationRequires); j++ )
            estrConcatCharString(&estr, pTemplate->ppchApplicationRequires[j]);
        pRequires = getRequiresString(estr, pClass, 0, 1);
        estrDestroy(&estr);
        if(pRequires && strlen(pRequires))
            continue;

        if(pTemplate->offAspect==offsetof(CharacterAttribSet, pattrMod))
            eType = kAttribType_Cur;
        if(pTemplate->offAspect==offsetof(CharacterAttribSet, pattrMax))
            eType = kAttribType_Max;
        if(pTemplate->offAspect==offsetof(CharacterAttribSet, pattrStrength))
            eType = kAttribType_Str;
        if(pTemplate->offAspect==offsetof(CharacterAttribSet, pattrResistance))
            eType = kAttribType_Res;
        if(pTemplate->offAspect==offsetof(CharacterAttribSet, pattrAbsolute))
            eType = kAttribType_Abs;

        if(iOff >= (int)offsetof(CharacterAttributes, fDamageType) && iOff < (int)offsetof(CharacterAttributes, fHitPoints))
        {
            if( equalForAllTypes[eTarget][kGroupType_Damage][eType] > 1  )
                continue;
            else if( equalForAllTypes[eTarget][kGroupType_Damage][eType] == 1 )
            {
                equalForAllTypes[eTarget][kGroupType_Damage][eType]++;
                pchAttrib = textStd("AllDamage");
            }

            // Damage accounted for in average damage, but let healing pass through
            if(eType == kAttribType_Abs && offset!=offsetof(CharacterAttributes, fDamageType[7]) )
                continue;
        }
        else if(offset >= (int)offsetof(CharacterAttributes, fDefenseType) && offset < (int)offsetof(CharacterAttributes, fDefense))
        {
            if( equalForAllTypes[eTarget][kGroupType_Defense][eType] > 1  )
                continue;
            else if( equalForAllTypes[eTarget][kGroupType_Defense][eType] == 1 )
                equalForAllTypes[eTarget][kGroupType_Defense][eType]++;
            pchAttrib = textStd("AllDefense");
        }

        modGetMagnitudeAndDuration(pTemplate, pClass, iLevel, &fMag, &fDuration );

        // flip damage for readability
         if(iOff >= (int)offsetof(CharacterAttributes, fDamageType) && iOff < (int)offsetof(CharacterAttributes, fHitPoints) && (eType == kAttribType_Abs) && iOff!= offsetof(CharacterAttributes, fDamageType[7]) )
            fMag = -fMag;

        // status effects use duration
         if( iOff >= (int)offsetof(CharacterAttributes, fTaunt) && iOff <= (int)offsetof(CharacterAttributes, fOnlyAffectsSelf) )
        {
            fMag = fDuration;
            found_attrib |= addEnhanceStatLine( str, pchAttrib,  0, "s", fMag, baseVal, replaceVal, combineVal, noPlus, 0, 1 );
        }
        else if(iOff >= (int)offsetof(CharacterAttributes, fKnockup) && iOff <= (int)offsetof(CharacterAttributes, fRepel))
        {
            found_attrib |= addEnhanceStatLine( str, pchAttrib, 0, 0, fMag, baseVal, replaceVal, combineVal, noPlus, 0, 0 );
        }
        else if( eType == kAttribType_Cur || eType == kAttribType_Str || eType == kAttribType_Res )
        {
            fMag *=100;
            found_attrib |= addEnhanceStatLine( str, pchAttrib, eType == kAttribType_Res, "%%", fMag, baseVal, replaceVal, combineVal, noPlus, 0, 0 );
        }
        else
            found_attrib |= addEnhanceStatLine( str, pchAttrib, eType == kAttribType_Res, 0, fMag, baseVal, replaceVal, combineVal, noPlus, 0, 0 );
    }

    return found_attrib;
}


static powerDumpData(char *msg)
{
    static FILE    *file;
    if (!file)
        file = fopen("c:/powertext.txt","wt");
    fprintf(file,"%s:",msg);
    fprintf(file,"\n");
    fflush(file);
    fclose(file);
}



void displayPowerInfo(F32 x, F32 y, F32 z, F32 wd, F32 ht, const BasePower *pPowBase, const Power * pPow, int window, int ShowEnhance, int isPlayerCreatedCritter )
{
    int iLevel, bReformat = 0;
    char * str;
    static const BasePower *pLastPow = 0;
    static SMFBlock *pBlock = 0;
    static SMFBlock *pBlockTitle = 0;
    static SMFBlock *pBlockTitle2 = 0;
    static char * str_buff = 0;
    static int ShowPvP=0, iLastLevel, iLastMarked;
    static ScrollBar sb;
    F32 sc = 1, text_ht;
    UIBox box;
    AtlasTex * white = atlasLoadTexture("white");
    int hybrid = 0;
    static F32 hbScrollOffset = 0.f;
    F32 xposSc, yposSc;
    F32 textXSc = 1.f;
    F32 textYSc = 1.f;
    SpriteScalingMode ssm = getSpriteScaleMode();
    if (isTextScalingRequired())
    {
        calculateSpriteScales(&textXSc, &textYSc);
    }
    calculatePositionScales(&xposSc, &yposSc);

    if (ShowEnhance == POWERINFO_HYBRID_BASE)
    {
        // pretty much the same as base except for a few text changes
        hybrid = 1;
        ShowEnhance = POWERINFO_BASE;
    }

    // TODO: cache this stuff?
    if(!pPowBase)
        return;

    if (!pBlock)
    {
        pBlock = smfBlock_Create();
    }

    if (!pBlockTitle)
    {
        pBlockTitle = smfBlock_Create();
    }
    if (!pBlockTitle2)
    {
        pBlockTitle2 = smfBlock_Create();
    }

    if(!s_pClass) // safety
        powerInfoSetClass(playerPtr()->pchar->pclass);

    s_iPowCnt = 0;
    if( window )
        window_getDims(window, 0, 0, 0, 0, 0,&sc,0,0);

    iLevel = round((s_fLevel+1)*24.5);

    if( pLastPow != pPowBase )
        iMarkedOpenPowers = 0;

    s_bReformatInfo |= pLastPow != pPowBase || iLastLevel != iLevel || iLastMarked != iMarkedOpenPowers;

    if(pLastPow != pPowBase)
    {
        if (hybrid)
        {
            hbScrollOffset = 0.f;
        }
        else
        {
            sb.offset = 0;
        }
    }

    if( s_bReformatInfo )
    {
        CharacterAttributes powerAttributes;
        s_bReformatInfo = 0;

        estrCreate(&str);

        if( pPow )
            ClientCalcBoosts(pPow, &powerAttributes, 0);

        estrConcatStaticCharArray(&str, "<br>");
        power_AddBaseStats( pPowBase, pPow?&powerAttributes:0, s_pClass, NULL, iLevel, &str, ShowEnhance, ShowPvP, 0);
        estrConcatf(&str, "<br><img src=white width=%f height=1><br>", wd );

        if (!isPlayerCreatedCritter)
        {
            power_AddTargetInfo( pPowBase, pPow?&powerAttributes:0, &str, ShowEnhance );
            estrConcatf(&str, "<br><img src=white width=%f height=1><br><br>", wd );
        }

        power_AddEffects( pPowBase, pPow?&powerAttributes:0, s_pClass, NULL, iLevel, &str, ShowEnhance, 0, 0, ShowPvP);

        power_AddPets( &str, pPowBase, pPow?&powerAttributes:0, s_pClass, iLevel, ShowEnhance, ShowPvP, 0);

        SAFE_FREE(str_buff);
        str_buff = strdup(str);
        estrDestroy(&str);
        bReformat = 1;
        //powerDumpData(str_buff);
    }

    s_taPowerInfo.piScale = (int *)((int)(1.3f*textYSc*sc*SMF_FONT_SCALE));
    if (hybrid)
    {
        if (isTextScalingRequired())
        {
            setSpriteScaleMode(SPRITE_XY_NO_SCALE);
        }
        s_taPowerInfo.piOutline = (int*)((int)(0));
        s_taPowerInfo.piFace = (int *)&title_9;
        s_taPowerInfo.piItalic = (int *)1;
        s_taPowerInfo.piShadow = (int *)1;
        text_ht = smf_ParseAndDisplay( pBlockTitle, textStd( "PowerInfoTitleHybrid1", pPowBase->pchDisplayName), x, y, z, wd, 0, bReformat, bReformat, &s_taPowerInfo, NULL, 0, true);
        s_taPowerInfo.piFace = (int *)&smfSmall;
        s_taPowerInfo.piItalic = (int *)0;
        text_ht = smf_ParseAndDisplay( pBlockTitle2, textStd( "PowerInfoTitleHybrid2", iLevel+1, s_pClass->pchDisplayName), x+200, y-4, z, wd, 0, bReformat, bReformat, &s_taPowerInfo, NULL, 0, true);
        s_taPowerInfo.piShadow = (int *)0;
        setSpriteScaleMode(ssm);
    }
    else
    {
        text_ht = smf_ParseAndDisplay( pBlockTitle, textStd( "PowerInfoTitle", pPowBase->pchDisplayName, iLevel+1, s_pClass->pchDisplayName), x, y, z, wd, 0, bReformat, bReformat, &s_taPowerInfo, NULL, 0, true);
    }

    iLastMarked = iMarkedOpenPowers;
    iLastLevel = iLevel;

    font(&game_9);
    font_color( 0x777777ff, 0x777777ff );
    if (hybrid)
    {
        font_outl(0);
        if (isTextScalingRequired())
        {
            setSpriteScaleMode(SPRITE_XY_NO_SCALE);
        }
        if( ShowEnhance == POWERINFO_ENHANCED )
            cprnt( x + wd*textXSc/2, y+(text_ht+45*sc)*textYSc, z, textXSc*sc, textYSc*sc, "ShowingEnhanced" );
        else if( ShowEnhance == POWERINFO_BASE )
            cprnt( x + wd*textXSc/2, y+(text_ht+45*sc)*textYSc, z, textXSc*sc, textYSc*sc, "ShowingUnenhanced" );
        else
            cprnt( x + wd*textXSc/2, y+(text_ht+45*sc)*textYSc, z, textXSc*sc, textYSc*sc, "ShowingBoth" );
        setSpriteScaleMode(ssm);
    }
    else
    {
        if( ShowEnhance == POWERINFO_ENHANCED )
            cprnt( x + wd/2, y+text_ht+45*sc, z, sc, sc, "ShowingEnhanced" );
        else if( ShowEnhance == POWERINFO_BASE )
            cprnt( x + wd/2, y+text_ht+45*sc, z, sc, sc, "ShowingUnenhanced" );
        else
            cprnt( x + wd/2, y+text_ht+45*sc, z, sc, sc, "ShowingBoth" );
    }

    if (isPlayerCreatedCritter)
    {
        s_bReformatInfo = 0;
        s_fLevel = doSliderAll(x + 5*sc + (wd-(10*sc))/2, y+text_ht+15*sc, z, wd - (10*sc), sc, sc, s_fLevel, -1.f, 1.f, SLIDER_POWER_INFO_LEVEL, CLR_WHITE, 0, 0, 0);
    }
    else
    {
        if (hybrid)
        {
            if (isTextScalingRequired())
            {
                setSpriteScaleMode(SPRITE_XY_NO_SCALE);
            }
            //shift button over
            s_bReformatInfo = drawSmallCheckboxEx( 0, x + 10*xposSc*sc, y+(text_ht+15*sc)*yposSc, z, textYSc*sc, "ShowPvP", &ShowPvP, 0, 1, 0, 0, 0, 0 );
            s_fLevel = doSliderAll(x + (150*sc + (wd-200*sc)/2)*xposSc, y+(text_ht+15*sc)*yposSc, z, (wd - 200*sc), textXSc*sc, textYSc*sc, s_fLevel, -1.f, 1.f, SLIDER_POWER_INFO_LEVEL, CLR_WHITE, 0, 0, 0);
            setSpriteScaleMode(ssm);
        }
        else
        {
            s_bReformatInfo = drawSmallCheckboxEx( 0, x + 5*sc, y+text_ht+15*sc, z, sc, "ShowPvP", &ShowPvP, 0, 1, 0, 0, 0, 0 );
            s_fLevel = doSliderAll(x + 150*sc + (wd-200*sc)/2, y+text_ht+15*sc, z, wd - 200*sc, sc, sc, s_fLevel, -1.f, 1.f, SLIDER_POWER_INFO_LEVEL, CLR_WHITE, 0, 0, 0);
        }
    }

    if (hybrid)
    {
        font_outl(1);
        display_sprite_positional(white, x+5*xposSc*sc, y+(text_ht+50*sc)*yposSc, z, (wd-10*sc)/white->width, 1.f/white->height, CLR_WHITE, H_ALIGN_LEFT, V_ALIGN_TOP );
        y += (text_ht+52*sc)*yposSc;
        ht -= text_ht+50*sc;
        uiBoxDefineScaled(&box, x, y, wd*xposSc, ht*yposSc );
    }
    else
    {
        display_sprite(white, x+5*sc, y+text_ht+50*sc, z, (wd-10*sc)/white->width, 1.f/white->height, CLR_WHITE );
        y += text_ht+52*sc;
        ht -= text_ht+50*sc;
        uiBoxDefine(&box, x, y, wd, ht );
    }
    clipperPush(&box);
    if (hybrid)
    {
        if (isTextScalingRequired())
        {
            setSpriteScaleMode(SPRITE_XY_NO_SCALE);
        }
        //shift text over
        s_taPowerInfo.piScale = (int *)((int)(sc*textYSc*SMF_FONT_SCALE));
        text_ht = smf_ParseAndDisplay( pBlock, str_buff, x+5.f*textXSc*sc, y-hbScrollOffset*textYSc, z, wd, 0, s_bReformatInfo, s_bReformatInfo, &s_taPowerInfo, NULL, 0, true);
        s_taPowerInfo.piOutline = (int*)((int)(1));
        setSpriteScaleMode(ssm);
    }
    else
    {
        s_taPowerInfo.piScale = (int *)((int)(sc*SMF_FONT_SCALE));
        text_ht = smf_ParseAndDisplay( pBlock, str_buff, x, y-sb.offset, z, wd, 0, s_bReformatInfo, s_bReformatInfo, &s_taPowerInfo, NULL, 0, true);
    }
    pLastPow = pPowBase;
    clipperPop();

    if (hybrid)
    {
        CBox cbox;
        BuildCBox(&cbox, x, y, wd, ht);
        drawHybridScrollBar(x+(wd-20)*xposSc, y+10*xposSc, z, ht-20, &hbScrollOffset, text_ht-ht, &cbox);
    }
    else
    {
        doScrollBar(&sb, ht, text_ht, x+wd+10*sc, y, z, 0, &box );
    }
}

void menuDisplayPowerInfo( F32 x, F32 y, F32 z, F32 wd, F32 ht, const BasePower *pBasePow, const Power * pPow, int ShowEnhance )        
{
     clipperPush(NULL);
    drawMenuFrame( R12, x, y, z, wd, ht, CLR_WHITE, 0x00000088, 0 );
    if( pBasePow )
        displayPowerInfo( x+10, y+10, z+10, wd-20, ht-15, pBasePow, pPow, 0, ShowEnhance, 0 );
    else
    {
        font( &game_12 );
        font_color( CLR_WHITE, CLR_WHITE );
        cprntEx( x+wd/2, y+ht/2, z+5, 1.3f, 1.3f, CENTER_X|CENTER_Y, "MouseoverPowerForInfo" );
    }
    clipperPop();
}

void menuHybridDisplayPowerInfo( F32 x, F32 y, F32 z, F32 wd, F32 ht, const BasePower *pBasePow, const Power * pPow, int ShowEnhance, F32 alphaScale )        
{
    AtlasTex * w = white_tex_atlas;
    F32 fadein = 30;
    int clrBlue = (CLR_H_BACK & (0xffffff00 | (int)(0xdd * alphaScale)));
    int clrWhite = 0x86acff00 | (int)(0xdd * alphaScale);
    int clrHighlight = (CLR_H_HIGHLIGHT & (0xffffff00 | (int)(0xdd * alphaScale)));
    int hht = 2.f;

    clipperPush(NULL);

    drawHybridDescFrame(x, y, z, wd, ht, HB_DESCFRAME_FULLFRAME, 0.f, alphaScale, 0.95f, H_ALIGN_LEFT, V_ALIGN_TOP);

    if (alphaScale >= 1.f)
    {
        F32 xposSc, yposSc;
        calculatePositionScales(&xposSc, &yposSc);
        if( pBasePow )
        {
            displayPowerInfo( x+10.f*xposSc, y+10.f*yposSc, z+10, wd-20, ht-25, pBasePow, pPow, 0, ShowEnhance, 0 );
        }
        else
        {
            F32 textXSc = 1.f;
            F32 textYSc = 1.f;
            SpriteScalingMode ssm = getSpriteScaleMode();

            if (isTextScalingRequired())
            {
                calculateSpriteScales(&textXSc, &textYSc);
                setSpriteScaleMode(SPRITE_XY_NO_SCALE);
            }
            font( &hybridbold_12 );
            font_color( CLR_WHITE, CLR_WHITE );
            cprntEx( x+wd*xposSc/2, y+ht*yposSc/2, z+5, 1.3f*textXSc, 1.3f*textYSc, CENTER_X|CENTER_Y, "MouseoverPowerForInfo" );
            setSpriteScaleMode(ssm);
        }
    }

    clipperPop();
}

