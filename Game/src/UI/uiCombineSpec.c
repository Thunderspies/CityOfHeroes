
#include <limits.h>

#include "uiPowers.h"
#include "UI/uiNet.h"
#include "UI/uiFx.h"
#include "UI/uiGame.h"
#include <utilitieslib/utils/utils.h>
#include "UI/uiUtil.h"
#include "UI/uiTray.h"
#include "UI/uiInfo.h"
#include "UI/uiInput.h"
#include "UI/uiRespec.h"
#include "UI/uiCursor.h"
#include "UI/uidialog.h"
#include "UI/uiWindows.h"
#include "UI/uiUtilMenu.h"
#include "UI/uiEditText.h"
#include "UI/uiScrollBar.h"
#include "UI/uiLevelPower.h"
#include "uiCombineSpec.h"
#include "UI/uiPowerInventory.h"
#include "UI/uiToolTip.h"
#include "UI/Hybrid/uiHybridMenu.h"
#include "UI/uiTabControl.h"
#include "uiEmote.h"
#include "UI/Hybrid/uiWebStoreFrame.h"

#include "UI/sprite/sprite_base.h"
#include "UI/sprite/sprite_text.h"
#include "UI/sprite/sprite_font.h"
#include "graphics/textureatlas.h"

#include "language/langClientUtil.h"
#include "cmdparse/cmdcommon.h"
#include "win/win_init.h"
#include "cmdparse/cmdgame.h"
#include <utilitieslib/components/Earray.h>
#include <utilitieslib/components/estring.h>
#include <utilitieslib/components/StashTable.h>
#include "player/player.h"
#include "sound/sound.h"
#include "graphics/font.h"
#include <utilitieslib/utils/mathutil.h>

#include "win/input.h"
#include "sound/sound.h"
#include "entity/powers.h"
#include "entity/boostset.h"
#include "entity/origins.h"
#include "entity/classes.h"
#include <utilitieslib/components/Earray.h>
#include <utilitieslib/utils/timing.h>
#include "graphics/ttFontUtil.h"
#include "entity/attrib_names.h"
#include "entity/character_base.h"
#include "entity/character_level.h"
#include "entity/character_net_client.h"
#include "player/pmotion.h"
#include "UI/uiUtilGame.h"
#include "UI/uiContextMenu.h"
#include "UI/uiEnhancement.h"
#include "entity/entity.h"
#include "UI/uiOptions.h"
#include <utilitieslib/language/MessageStoreUtil.h>
#include <utilitieslib/utils/file.h>
#include "UI/uiPowerInfo.h"
#include "uiGrowBig.h"
#include "uiSalvage.h"

#include "gameComm/villainDef.h"
//-------------------------------------------------------------

void TestDimReturns(Power *ppow);
ToolTip enhanceTip;

static DialogCheckbox placeDCB[] = { { "HidePlacePrompt",0, kUO_HidePlacePrompt}};
static DialogCheckbox deleteDCB[] = { { "HideDeletePrompt", 0, kUO_HideDeletePrompt}};
static DialogCheckbox unslotDCB[] = { { "HideUnslotPrompt", 0, kUO_HideUnslotPrompt}};

static int s_ShowCombatNumbersHere=0;

static uiGrowBig growBig;

#define FRAME_HEIGHT    617
#define FRAME_HT        400
#define FRAME_BUFFER 15
#define SPACER  3
#define TXT_SC  1.5f

typedef struct SlotLoc
{
    float x;
    float y;
    float sc;
    float timer;
    int   retract;
}SlotLoc;

enum
{
    COMBO_NOOPTION,
    COMBO_EQUIP,
    COMBO_CHOOSE1,
    COMBO_CHOOSE2,
    COMBO_WAITING,
    COMBO_SUCCESS,
    COMBO_GLORY,
    COMBO_FAIL,
    COMBO_FAIL2,
    COMBO_DEFEAT,
};

typedef struct CombineBox
{

    int x;
    int y;
    int wd;
    int ht;
    int color;

    int open;
    int state;
    int waiting;
    float changeup;
    int result;

    const BasePower *pow;
    Boost * slotA;
    Boost * slotB;
    Boost * slotC;

    const SalvageItem *salSlotB;

    SlotLoc locA;
    SlotLoc locB;
    SlotLoc locC;

    SlotLoc locSalSlotB;

    float alpha;
    float alpha2;
    float failureTimer;
}CombineBox;

CombineBox combo = {0};

static void combo_set( Power * pow, int x, int y );
void spec_replace(void * data);

static Power * comboPower()
{
    return character_OwnsPower( playerPtr()->pchar, combo.pow );
}

static void combo_setLoc( int slot, float x, float y, float sc, float timer, int retract )
{
    SlotLoc * sl;

    if( slot == 0 )
        sl = &combo.locA;
    else if ( slot == 1 )
        sl = &combo.locB;
    else if ( slot == 2)
        sl = &combo.locC;
    else 
        sl = &combo.locSalSlotB;

    sl->x = x;
    sl->y = y;
    sl->sc = sc;

    if( timer >= 0 )
        sl->timer = timer;

    if( retract >= 0 )
        sl->retract = retract;
}

#define POWER_SPEC_WD   200
#define SPEC_HT         32
#define COMBO_POW_X     300
#define COMBO_POW_Y     140

static int validBoostCombo(Character *pchar, Boost *boost1, Boost *boost2)
{
    int iCharLevel;
    if (!pchar || !boost1 || !boost2)
        return false;
    if (!boost_IsValidCombination(boost1,boost2))
        return false;
    iCharLevel = character_CalcExperienceLevel(pchar);
    if (power_MinBoostUseLevel(boost1->ppowBase, boost1->iLevel+boost1->iNumCombines) >= iCharLevel)
        return false;
    if (power_MinBoostUseLevel(boost2->ppowBase, boost2->iLevel+boost2->iNumCombines) >= iCharLevel)
        return false;
    return true;
}


//-----------------------------------------------------------------------------------------
// Drawing Enhancements
//-----------------------------------------------------------------------------------------

// Returns a pointer to the pog texbind, needed for cursor dragging
//
AtlasTex *drawEnhancementOriginLevelEx( const BasePower *ppowBase, int level, int iNumCombines, bool bDrawOrigin, float centerX, float centerY, float z, float sc, float textSc, int clr, bool bDrawCentered, bool bDrawAsRecipe )
{
    Entity *e = playerPtr();
    AtlasTex *icon, *frameL = 0, *frameR = 0, *pog = 0;
    AtlasTex *lvlFrame = atlasLoadTexture( "E_Number_Frame_2digit.tga" );
    char frameBaseName[256];
    int i = 0, num = 0;
    int frameHeight = 0;
    float scPog;
    float frameScaleMin = 1.0f;

    // For now
    bDrawOrigin = true;

    icon = atlasLoadTexture( ppowBase->pchIconName );

    if ( bDrawAsRecipe )
    {
        frameL = atlasLoadTexture( "E_origin_recipe_schematic.tga" );
        frameR = NULL;

        while( !pog && i < eaiSize(&(int *)ppowBase->pBoostsAllowed) )
        {
            if( ppowBase->pBoostsAllowed[i] >= eaSize(&g_CharacterOrigins.ppOrigins) )
            {
                pog = atlasLoadTexture( g_AttribNames.ppBoost[ppowBase->pBoostsAllowed[i]-eaSize(&g_CharacterOrigins.ppOrigins)]->pchIconName );
            }
            else
                i++;
        }
    } 
    else if( strstri( textStd(ppowBase->pchName), "generic" ) )
    {
        frameL = atlasLoadTexture( "E_orgin_generic_L.tga" );
        frameR = atlasLoadTexture( "E_orgin_generic_R.tga" );

        while( !pog && i < eaiSize(&(int *)ppowBase->pBoostsAllowed) )
        {
            if( ppowBase->pBoostsAllowed[i] >= eaSize(&g_CharacterOrigins.ppOrigins) )
            {
                pog = atlasLoadTexture( g_AttribNames.ppBoost[ppowBase->pBoostsAllowed[i]-eaSize(&g_CharacterOrigins.ppOrigins)]->pchIconName );
            }
            else
                i++;
        }
    }
    else if(baseset_IsCrafted(ppowBase->psetParent))
    {
        frameL = atlasLoadTexture( "E_orgin_invention_L.tga" );
        frameR = atlasLoadTexture( "E_orgin_invention_R.tga" );

        while( !pog && i < eaiSize(&(int *)ppowBase->pBoostsAllowed) )
        {
            if( ppowBase->pBoostsAllowed[i] >= eaSize(&g_CharacterOrigins.ppOrigins) )
            {
                pog = atlasLoadTexture( g_AttribNames.ppBoost[ppowBase->pBoostsAllowed[i]-eaSize(&g_CharacterOrigins.ppOrigins)]->pchIconName );
            }
            else
                i++;
        }
    }
    else if( strstri( textStd(ppowBase->pchName), "superior_attuned" ))
    {
        frameL = atlasLoadTexture( "E_origin_superiorattune_L.tga" );
        frameR = atlasLoadTexture( "E_origin_superiorattune_R.tga" );

        while( !pog && i < eaiSize(&(int *)ppowBase->pBoostsAllowed) )
        {
            if( ppowBase->pBoostsAllowed[i] >= eaSize(&g_CharacterOrigins.ppOrigins) )
            {
                pog = atlasLoadTexture( g_AttribNames.ppBoost[ppowBase->pBoostsAllowed[i]-eaSize(&g_CharacterOrigins.ppOrigins)]->pchIconName );
            }
            else
                i++;
        }
    }
    else if( strstri( textStd(ppowBase->pchName), "attuned" ))
    {
        frameL = atlasLoadTexture( "E_origin_attune_L.tga" );
        frameR = atlasLoadTexture( "E_origin_attune_R.tga" );

        while( !pog && i < eaiSize(&(int *)ppowBase->pBoostsAllowed) )
        {
            if( ppowBase->pBoostsAllowed[i] >= eaSize(&g_CharacterOrigins.ppOrigins) )
            {
                pog = atlasLoadTexture( g_AttribNames.ppBoost[ppowBase->pBoostsAllowed[i]-eaSize(&g_CharacterOrigins.ppOrigins)]->pchIconName );
            }
            else
                i++;
        }
    }
    else if( strstri( textStd(ppowBase->pchName), "hamidon" ) || 
                strstri( textStd(ppowBase->pchName), "titan" ) || 
                strstri( textStd(ppowBase->pchName), "hydra" ) )
    {
        frameL = atlasLoadTexture( "E_orgin_uber_L.tga" );
        frameR = atlasLoadTexture( "E_orgin_uber_R.tga" );

        while( !pog && i < eaiSize(&(int *)ppowBase->pBoostsAllowed) )
        {
            if( ppowBase->pBoostsAllowed[i] >= eaSize(&g_CharacterOrigins.ppOrigins) )
            {
                pog = atlasLoadTexture( g_AttribNames.ppBoost[ppowBase->pBoostsAllowed[i]-eaSize(&g_CharacterOrigins.ppOrigins)]->pchIconName );
            }
            else
                i++;
        }
    }
    else
    {
        int iSizeOrigins = eaSize(&g_CharacterOrigins.ppOrigins);
        int iSize = eaiSize(&(int *)ppowBase->pBoostsAllowed);
        for( i = 0; i < iSize; i++ )
        {
            num = ppowBase->pBoostsAllowed[i];

            // an origin, get frame sprite
            if( num < iSizeOrigins )
            {
                char frameName[256];
                const char * origin = g_CharacterOrigins.ppOrigins[num]->pchName;

                if( stricmp( origin, "Magic" ) == 0 )
                    strcpy( frameName, "E_orgin_magic" );
                if( stricmp( origin, "Science" ) == 0 )
                    strcpy( frameName, "E_orgin_scientific" );
                if( stricmp( origin, "Mutation" ) == 0 )
                    strcpy( frameName, "E_orgin_mutant" );
                if( stricmp( origin, "Technology" ) == 0 )
                    strcpy( frameName, "E_orgin_tech" );
                if( stricmp( origin, "Natural" ) == 0 )
                    strcpy( frameName, "E_orgin_natural" );
                if( stricmp( origin, "Generic" ) == 0 )
                    strcpy( frameName, "E_orgin_generic" );

                if( !frameL )
                {
                    strcpy( frameBaseName, frameName );
                    strcat( frameName, "_L.tga" );
                    frameL = atlasLoadTexture( frameName );
                }
                else if ( !frameR )
                {
                    strcat( frameName, "_R.tga" );
                    frameR = atlasLoadTexture( frameName );
                }
            }
            else if( !pog )
            {
                num -= iSizeOrigins;
                pog = atlasLoadTexture( g_AttribNames.ppBoost[num]->pchIconName );
            }
        }
    }

    if( !frameR && !bDrawAsRecipe )
    {
        strcat( frameBaseName, "_R.tga" );
        frameR = atlasLoadTexture( frameBaseName );
    }

    if(bDrawOrigin)
    {
        if ( bDrawCentered )
        {
            display_sprite( frameL, centerX, centerY,  z,   sc, sc, clr );
            if (frameR)
                display_sprite( frameR, centerX + frameL->width*sc, centerY,  z,   sc, sc, clr );
        }
        else
        {
            if (bDrawAsRecipe)
            {
                display_sprite( frameL, centerX - 32.0f*sc,     centerY - frameL->height*sc/2,  z,   sc, sc, clr );
            } else {
                display_sprite( frameL, centerX - frameL->width*sc,     centerY - frameL->height*sc/2,  z,   sc, sc, clr );
                if (frameR)
                    display_sprite( frameR, centerX, centerY - frameR->height*sc/2,  z,   sc, sc, clr );
            }
        }
        scPog = 1.0f;
    }
    else
    {
        scPog = (float)frameL->height/(float)pog->height;
    }
    frameHeight = frameL->height;

    if(pog)
    {
        if ( bDrawCentered )
        {
            display_sprite( pog,    centerX + pog->width*scPog*sc/6, centerY + pog->height*scPog*sc/6, z+1, scPog*sc, scPog*sc, clr );
            display_sprite( icon,   centerX + pog->width*scPog*sc/6, centerY + pog->height*scPog*sc/6, z+2, scPog*sc, scPog*sc, clr );
        }
        else
        {
            display_sprite( pog,    centerX - pog->width*scPog*sc/2,      centerY - pog->height*scPog*sc/2,     z+1, scPog*sc, scPog*sc, clr );
            display_sprite( icon,   centerX - icon->width*scPog*sc/2,     centerY - icon->height*scPog*sc/2,    z+2, scPog*sc, scPog*sc, clr );
        }
    }

    if (ppowBase->bBoostIgnoreEffectiveness && !ppowBase->bBoostUsePlayerLevel)
    {
        if(sc >= frameScaleMin)
        {
            float wsc = (ppowBase->bBoostBoostable&&iNumCombines>0)?1.5f*sc:sc;
            if ( bDrawCentered )
                display_sprite( lvlFrame, centerX + wsc*frameHeight/2 - sc*lvlFrame->width/2, centerY, z+3, wsc, sc, clr );
            else
                display_sprite( lvlFrame, centerX - wsc*lvlFrame->width/2, centerY - sc*frameHeight/2, z+3, wsc, sc, clr );
        } 
    } 
    else if (!ppowBase->bBoostUsePlayerLevel)
    {
         if(sc >= frameScaleMin)
        {
            float wsc = iNumCombines>=2?1.3f*sc:sc;
            
            if ( bDrawCentered )
                display_sprite( lvlFrame, centerX + wsc*frameHeight/2 - sc*lvlFrame->width/2, centerY + sc*frameHeight - sc*lvlFrame->height, z+3, wsc, sc, clr );
            else
                display_sprite( lvlFrame, centerX - wsc*lvlFrame->width/2, centerY + sc*frameHeight/2 - sc*lvlFrame->height, z+3, wsc, sc, clr );
        }
    }

    font( &game_9 );
    font_color( CLR_WHITE, 0x88b6ffff );

    {
        int charLevel = character_CalcExperienceLevel(e->pchar); 
        float fEff = ppowBase->bBoostIgnoreEffectiveness ? 1.0f : boost_EffectivenessLevel(iNumCombines+level-1, charLevel);
        int minLevel = power_MinBoostUseLevel(ppowBase, iNumCombines+level-1);
        int maxLevel = power_MaxBoostUseLevel(ppowBase, iNumCombines+level-1);
        int levelOk = charLevel >= minLevel && charLevel <= maxLevel;

        if (ppowBase->bBoostBoostable)
        {
            fEff = boost_BoosterEffectivenessLevelByCombines(iNumCombines);
            minLevel = power_MinBoostUseLevel(ppowBase, level-1);
            maxLevel = power_MaxBoostUseLevel(ppowBase, level-1);
            levelOk = charLevel >= minLevel && charLevel <= maxLevel;
        }

        if(!levelOk || fEff==0.0f)
        {
            font_color( CLR_RED, CLR_RED );
        }
        else if (ppowBase->bBoostIgnoreEffectiveness)
        {
            font_color( CLR_PARAGON, CLR_PARAGON );
        }
        else if(fEff<1.0f)
        {
            font_color( CLR_YELLOW, CLR_YELLOW );
        }
        else if(fEff>1.0f)
        {
            font_color( CLR_GREEN, CLR_GREEN );
        } 

        if (ppowBase->bBoostUsePlayerLevel) 
        {
            frameBaseName[0] = 0;
            textSc = textSc * 2.0f;
        }
        else if(iNumCombines==0)
        {
            sprintf(frameBaseName, "%d", level);
        }
        else if(iNumCombines>2 || ppowBase->bBoostBoostable)
        {
            sprintf(frameBaseName, "%d+%d", level, iNumCombines);
        }
        else
        {
            int i;
            sprintf(frameBaseName, "%d", level);
            for(i=0; i<iNumCombines; i++)
            {
                strcat(frameBaseName, "+");
            }
        }
        
    }

    //if( isMenu(MENU_GAME) )
    font( &game_9 );

    if (ppowBase->bBoostIgnoreEffectiveness) 
    {
        float xOffset = (sc>=frameScaleMin ? 0.0f : -1.5f);
        float yOffset;
        if( bDrawCentered )
        {
            yOffset = (sc>=frameScaleMin ? 11.3*sc : 13*sc);
            cprnt( centerX + sc*(frameHeight/2 + xOffset), centerY + yOffset, z+4, textSc, textSc * 2, frameBaseName);
        }
        else
        {
            yOffset = (sc>=frameScaleMin ? (-frameHeight/2)*sc + (11.5f)*textSc : -14*sc + 3.5*(textSc - 1.0f));
            cprnt( centerX + sc*xOffset, centerY + yOffset, z+4, textSc, textSc, frameBaseName);
        }
    } else { 
        float xOffset = (sc>=frameScaleMin ? 0.2f : -1.5f);
        float yOffset;
        if( bDrawCentered )
        {
            yOffset = (sc>=frameScaleMin ? -1 : 0);
            cprnt( centerX + sc*(frameHeight/2), centerY  + sc*frameHeight + yOffset*sc, z+4, textSc, textSc, frameBaseName);
        }
        else
        {
            yOffset = (sc>=frameScaleMin ? -2 : 0);
            cprnt( centerX + sc*xOffset, centerY + sc*(frameHeight/2+yOffset), z+4, textSc, textSc, frameBaseName);
        }
    }
    return pog;
}

AtlasTex *drawMenuEnhancement( const Boost *spec, float centerX, float centerY, float z, float sc, int state, int level, 
                                bool bDrawOrigin, int menu, int showToolTip)
{
    AtlasTex    *ring        = atlasLoadTexture( "EnhncTray_Ring.tga" );
    AtlasTex    *highlight    = atlasLoadTexture( "EnhncTray_Ring_highlight.tga" );
    AtlasTex    *hole        = atlasLoadTexture( "EnhncTray_RingHole.tga" );
    AtlasTex    *pog        = 0;
    Entity        *e            = playerPtr();

    int colorIcon = CLR_WHITE;
    int colorHole = CLR_WHITE;

    if( state == kBoostState_Disabled || state == kBoostState_Hidden )
    {
        colorIcon = 0x888888ff;
        colorHole = 0x888888ff;
    }
    else if ( state == kBoostState_BeyondMaxSize )
    {
        colorIcon = 0x888888ff;
        colorHole = 0x88888800;
    }

    if( state == kBoostState_Later )
    {
        colorIcon = 0x888888ff;

        font( &game_9 );
        font_color( colorIcon, colorIcon );
        cprnt( centerX, centerY-1, z+5, sc, sc, "lvl" );
        cprnt( centerX, centerY+10, z+5, sc, sc, "%i", level );
        display_sprite(hole, centerX - hole->width*sc/2, centerY - hole->height*sc/2, z, sc, sc, 0x000000ff );
    }

    if( state == kBoostState_Selected )
    {
        ring = atlasLoadTexture( "EnhncTray_Ring_highlight.tga" );
        hole = atlasLoadTexture( "EnhncTray_RingHole_highlight.tga" );
    }


    if(spec && state != kBoostState_Hidden )
    {
        if (spec->pUI == NULL)
            uiEnhancementCreateFromBoost(spec);
        uiEnhancementSetColor(spec->pUI, colorIcon);
        pog = uiEnhancementDraw(spec->pUI, centerX, centerY, z, sc*(1.1f), 1.1f, menu, 0, showToolTip);  
    }
    else
    {
        // Add Praetorian-style empty enhancement here.
        if( !shell_menu() )
        {
            ring = atlasLoadTexture( "tray_ring_power.tga" );
            display_sprite(ring, centerX - ring->width*sc, centerY - ring->height*sc, z+2, sc*2, sc*2, colorIcon);
        }
        else
        {
            display_sprite(ring, centerX - ring->width*sc/2, centerY - ring->height*sc/2, z+2, sc, sc, colorIcon);
            display_sprite(highlight, centerX - highlight->width*sc/2, centerY - highlight->height*sc/2, z+1, sc, sc, colorIcon );
        }

        if( state == kBoostState_Later )
            display_sprite(hole, centerX - hole->width*sc/2, centerY - hole->height*sc/2, z, sc, sc, 0xffffff88 );
        else
            display_sprite(hole, centerX - hole->width*sc/2, centerY - hole->height*sc/2, z, sc, sc, colorHole );
    }

    return pog;
}

static TrayObj unslotMe = {0};
static int unslotInvLocation = 0;

static void combo_buyUnslotters(void * data)
{
    // HYRBID : Add store display here
    webStoreOpenProduct("coemunsl");
}

static void combo_unslotPowerSpec(void * data)
{
    Power *pPower = playerPtr()->pchar->ppPowerSets[unslotMe.iset]->ppPowers[unslotMe.ipow];
    int i;

    // need to flag all enhancements in this power to invalidate their tooltips
    for( i = eaSize(&pPower->ppBoosts)-1; i >= 0; i-- )
    {
        if (pPower->ppBoosts[i])
            uiEnhancementRefresh(pPower->ppBoosts[i]->pUI);
    }

    power_DestroyBoost( pPower , unslotMe.ispec, NULL );
    spec_sendUnslotFromPower( unslotMe.iset, unslotMe.ipow, unslotMe.ispec, unslotInvLocation );
    sndPlay( "Delete", SOUND_GAME );
}

// TODO: add tooltip and CM?
//
static void s_drawMenuInventoryEnhancement( Character *pchar, int ispec, float centerX, float centerY, float z, float screenScaleX, float screenScaleY )
{
    AtlasTex * ring = atlasLoadTexture( "EnhncTray_Ring.tga" );
    AtlasTex * pog  = 0;

    bool bDragging = false;
    int state;
    CBox box;

    Boost *spec = pchar->aBoosts[ispec];
    static TrayObj to_tmp = {0}; // storage area for context menu
    float screenScale = MIN(screenScaleX, screenScaleY);
    bDragging = cursor.dragging && ( cursor.drag_obj.type == kTrayItemType_SpecializationInventory  || cursor.drag_obj.type == kTrayItemType_SpecializationPower);

    if( ispec >= pchar->iNumBoostSlots )
        state = kBoostState_BeyondMaxSize;
    else if( bDragging && ispec < pchar->iNumBoostSlots )
        state = kBoostState_Selected;
    else
        state = kBoostState_Normal;

      if( combo.open && spec )
    {
        if( combo.state == COMBO_CHOOSE2
            && (!combo.slotA || ( combo.slotA->ppowBase != spec->ppowBase || combo.slotA->idx != spec->idx || combo.slotA->ppowParent != spec->ppowParent))
            && (!combo.slotB || ( combo.slotB->ppowBase != spec->ppowBase || combo.slotB->idx != spec->idx || combo.slotB->ppowParent != spec->ppowParent))
            && power_MinBoostUseLevel(spec->ppowBase, spec->iLevel+spec->iNumCombines) < character_CalcExperienceLevel(pchar)
              && boost_IsValidCombination( combo.slotA, spec ))
        {
            state = kBoostState_Normal;
        }
        else if ( (combo.state != COMBO_CHOOSE2 && combo.state != COMBO_EQUIP ||
                  (combo.state == COMBO_CHOOSE2 && !boost_IsValidCombination( combo.slotA, spec )) || combo.slotA == spec ))
        {
             if( combo.slotB && combo.slotB->ppowBase == spec->ppowBase && combo.slotB->idx == spec->idx )
            {

                if (combo.locB.x != centerX || combo.locB.y != centerY)
                    combo_setLoc( 1, centerX, centerY, 1.f, 0, 0 );
                state = kBoostState_Hidden;
            }
            else
                state = kBoostState_Disabled;
        }
    }
    else if( combo.open && ispec < pchar->iNumBoostSlots )
        state = kBoostState_Disabled;
 
    pog = drawMenuEnhancement( spec, centerX*screenScaleX, centerY*screenScaleY, z, 0.7f*screenScale, state, 0, true, current_menu(), !bDragging); 

    BuildCBox( &box, centerX*screenScaleX - ring->width*screenScale/2, centerY*screenScaleY - ring->height*screenScale/2, ring->width*screenScale, ring->height*screenScale );

    if( spec )
    {
        if( mouseCollision( &box ) )
        {
            if( !cursor.dragging && !combo.open )
                setCursor( "cursor_hand_open.tga", NULL, 0, 0, 0 );
            info_combineMenu( (void*)spec->ppowBase, 1, 1, spec->iLevel, spec->iNumCombines );
        }

        if( !combo.open )
        {
            if( mouseClickHit( &box, MS_RIGHT ) )
            {
                static TrayObj to_tmp;
                to_tmp.type = kTrayItemType_SpecializationInventory;
                to_tmp.ispec = ispec;
                enhancement_ContextMenu( &box, &to_tmp );
            }
        }

        // check for a drag if slot is inventory slot
         if( mouseDownHit( &box, MS_LEFT ) &&
            (state==kBoostState_Normal||state==kBoostState_BeyondMaxSize) )
        {
            if( combo.open && combo.state == COMBO_CHOOSE2 && combo.slotA != spec && combo.slotB != spec &&
                boost_IsValidCombination( combo.slotA, spec ) )
            {
                combo_setLoc( 1, centerX, centerY, 1.f, 0, 0 );
                combo.slotB         = boost_Clone(spec);
                combo.changeup      = 30;
                combo.locSalSlotB.retract = 1;
                sndPlay( "Tally", SOUND_GAME );
            }
            else if( (!combo.open || combo.state == COMBO_EQUIP) && !dialogInQueue(NULL,spec_replace,NULL)  )
            {
                TrayObj ts = {0};
                buildSpecializationTrayObj( &ts, TRUE, 0, 0, ispec );
                ts.icon = atlasLoadTexture( spec->ppowBase->pchIconName );
                trayobj_startDragging( &ts, ts.icon, pog );
                sndPlay( "Grab4", SOUND_GAME );
            }

            collisions_off_for_rest_of_frame = 1;
        }
    }

    //check for mouse release regardless of slot
    if( bDragging )
    {
        if( !isDown( MS_LEFT ) && mouseCollision( &box ) )
        {
            if (cursor.drag_obj.type == kTrayItemType_SpecializationInventory)
            {
                 int idx = cursor.drag_obj.ispec;
                if (ispec < pchar->iNumBoostSlots &&
                    !(pchar->aBoosts[ispec] && idx >= pchar->iNumBoostSlots))
                {
                    character_SwapBoosts( pchar, ispec, idx );
                    spec_sendSetInventory(idx, ispec);
                }
            } else if (spec == NULL) {
                // unslotting to empty inventory slot
                const SalvageItem *item = salvage_GetItem("S_EnhancementUnslotter");
                int count = character_SalvageCount(pchar, item);

                if (count > 0)
                {
                    trayobj_copy( &unslotMe, &cursor.drag_obj, 0 );
                    unslotInvLocation = ispec;
                    trayobj_stopDragging();
                    if (optionGet(kUO_HideUnslotPrompt))
                        combo_unslotPowerSpec(0);
                    else
                        dialog( DIALOG_YES_NO, -1, -1, -1, -1, "ReallyUnslotEnhancement", NULL, combo_unslotPowerSpec, NULL, NULL,0,sendOptions,unslotDCB,1,0,0,0 );    
                } else {
                    // would you like to buy some?
                    dialog( DIALOG_YES_NO, -1, -1, -1, -1, "BuyUnslotters", NULL, combo_buyUnslotters, NULL, NULL, 0, NULL, NULL, 1,0,0,0 );    
                }
            }
            trayobj_stopDragging();
        }
    }
}

typedef struct SpecReplacement
{
    float x, y, z;
    int iset, ipow, ispec, idx;
    Power * pow;
} SpecReplacement;

static SpecReplacement sr = {0};

void spec_replace(void *data)
{
    Entity *e = playerPtr();

    sndPlay( "lock2", SOUND_GAME );

    electric_init( sr.x, sr.y, sr.z+10, .8f, 0, 45, 2, 0 );

     if(e->pchar->aBoosts[sr.idx]!=NULL && power_IsValidBoost(sr.pow, e->pchar->aBoosts[sr.idx]) )
    {
        int slot_req = power_ValidBoostSlotRequired(sr.pow, e->pchar->aBoosts[sr.idx]);
        if(slot_req < 0 || slot_req == sr.ispec)
        {
            if (!power_BoostCheckRequires(sr.pow, e->pchar->aBoosts[sr.idx]))
                return;

            power_ReplaceBoostFromBoost( sr.pow, sr.ispec, e->pchar->aBoosts[sr.idx], NULL );
            character_DestroyBoost( e->pchar, sr.idx, NULL );

            // Alert the server of the change
            spec_sendSetPower( sr.iset, sr.ipow, sr.idx, sr.ispec );
        }
    }
}

//
//
int drawMenuPowerEnhancement( Character *pchar, int iset, int ipow, int ispec, float x, float y, float z, float sc, int color, int isCombining, float screenScaleX, float screenScaleY)
{
    AtlasTex * ring = atlasLoadTexture( "EnhncTray_Ring.tga" );
    AtlasTex * pog;

    bool bMatches = true;
    bool bDragging = false;
    int state;
    float screenScale = MIN(screenScaleX, screenScaleY);
    int xparent = x*screenScaleX - (-2 + POWER_OFFSET)*screenScaleX - ((ispec+1)*SPEC_WD)*sc*screenScale;
    int yparent = y*screenScaleY - 20*sc;

    CBox box;

    Power *pow = pchar->ppPowerSets[iset]->ppPowers[ipow];
    Boost *spec = pow->ppBoosts[ispec];

    static TrayObj to_tmp = {0}; // storage area for context menu

    if( cursor.dragging && cursor.drag_obj.type==kTrayItemType_SpecializationInventory && !pchar->aBoosts[cursor.drag_obj.ispec] )
        trayobj_stopDragging();

    bDragging = cursor.dragging && (cursor.drag_obj.type==kTrayItemType_SpecializationInventory);

    if(bDragging)
    {
        // OK, they're in a drag from the inventory. Determine if they're
        // allowed to drop it on this spot.
        bMatches = boost_IsValidToSlotAtLevel(pchar->aBoosts[cursor.drag_obj.ispec], pchar->entParent, 
                                                character_CalcExperienceLevel(pchar), pchar->entParent->erOwner);
        bMatches = bMatches && power_IsValidBoost(pow, pchar->aBoosts[cursor.drag_obj.ispec]);
        if (bMatches)
            bMatches = power_BoostCheckRequires(pow, pchar->aBoosts[cursor.drag_obj.ispec]);

        if (bMatches)
        {
            // checking for two of the same enhancement when the enhancement belongs to a set
            int slot_req = power_ValidBoostSlotRequired(pow, pchar->aBoosts[cursor.drag_obj.ispec]);
            if(!(slot_req < 0 || slot_req == ispec)) 
            {
                bMatches = false;
            }
        }

        if( bMatches )
            state = kBoostState_Selected;
        else
            state = kBoostState_Disabled;
    }
    else
        state = kBoostState_Normal;

    if( combo.open && spec )
    {
        if( (combo.slotA && combo.slotA->ppowBase == spec->ppowBase && combo.slotA->idx == spec->idx && combo.slotA->ppowParent == spec->ppowParent  ))
        {
            state = kBoostState_Hidden;
            if ((combo.locA.x != x || combo.locA.y != y) && isCombining == combo.open && combo.state < COMBO_SUCCESS)
                combo_setLoc( 0, x, y, 1.f, 0, 0 );
        }
        else if( (combo.slotB && combo.slotB->ppowBase == spec->ppowBase && combo.slotB->idx == spec->idx && combo.slotB->ppowParent == spec->ppowParent  ))
        {
            state = kBoostState_Hidden;
            if ((combo.locB.x != x || combo.locB.y != y) && isCombining == combo.open && combo.state < COMBO_SUCCESS)
                combo_setLoc( 1, x, y, 1.f, 0, 0 );
        }
        else if( (combo.slotC && combo.slotC->ppowBase == spec->ppowBase && combo.slotC->idx == spec->idx && combo.slotC->ppowParent == spec->ppowParent  ))
        {
            state = kBoostState_Hidden;
        } 
        else if(!spec->ppowBase->bBoostCombinable && !boost_IsBoostable(pchar->entParent, spec) && !boost_IsCatalyzable(spec))
            state = kBoostState_Disabled;
        else if(spec->ppowBase->bBoostCombinable && power_MinBoostUseLevel(spec->ppowBase,spec->iLevel+spec->iNumCombines) >= character_CalcExperienceLevel(pchar))
            state = kBoostState_Disabled;
        else if( (combo.state == COMBO_CHOOSE1 && 
            (boost_IsValidCombination(spec, spec) || boost_IsBoostable(pchar->entParent, spec) || boost_IsCatalyzable(spec)))
            || (combo.state == COMBO_CHOOSE2 && combo.slotA != spec && combo.slotB != spec && boost_IsValidCombination( combo.slotA, spec ) ))
            state = kBoostState_Normal;
        else if ( (combo.state != COMBO_CHOOSE2 && combo.state != COMBO_EQUIP ||
                  (combo.state == COMBO_CHOOSE2 && !boost_IsValidCombination( combo.slotA, spec )) )  )
        {
            state = kBoostState_Disabled;
        }
    }
    else if( combo.open )
        state = kBoostState_Disabled;
    else if (spec && spec->pUI)
    {
        Entity *player = playerPtr();
        int oldValue = spec->pUI->isLicenseLocked;

        if (player && boost_InventionLicenseLocked(spec, player))
        {
            spec->pUI->isLicenseLocked = 1;
        } else {
            spec->pUI->isLicenseLocked = 0;
        }

        if (oldValue != spec->pUI->isLicenseLocked)
            spec->pUI->refresh = 1;

        if (spec->pUI->isLicenseLocked == 1)
            state = kBoostState_Disabled;
    } 

    pog = drawMenuEnhancement( spec, x*screenScaleX, y*screenScaleY, z, sc*screenScale, state, 0, false, current_menu(), !combo.open && !bDragging);

    BuildCBox( &box, x*screenScaleX-ring->width*sc*screenScale/2, y*screenScaleY-ring->height*sc*screenScale/2, ring->width*sc*screenScale, ring->height*sc*screenScale );

    if( !bMatches || (combo.open && combo.pow != pow->ppowBase ) )
        return FALSE;

    if( isMenu(MENU_COMBINE_SPEC)&& mouseCollision(&box) && isCombining == combo.open)
    {
        float tx;

        if (state == kBoostState_Selected && bDragging && game_state.enableBoostDiminishing)
        {
            Boost *dragSpec = pchar->aBoosts[cursor.drag_obj.ispec];
            forceDisplayToolTip(&enhanceTip,&box,getEffectiveString(pow,spec,dragSpec,ispec),MENU_COMBINE_SPEC,0);
        }
        if( x < DEFAULT_SCRN_WD/3 || x > DEFAULT_SCRN_WD*2/3 )
            tx = DEFAULT_SCRN_WD/2;
        else
            tx = FRAME_SPACE + FRAME_WIDTH/2;

        if(spec && spec->ppowBase)
            info_combineMenu( (void*)spec->ppowBase, 1, 0, spec->iLevel, spec->iNumCombines );

        if( mouseLeftClick())
        {
            if(combo.open)
            {
                if(state == kBoostState_Normal)
                    return D_MOUSEHIT;
            }
            else
            {
                combo_set(pow, xparent, yparent);
            }
        }
        else if( spec && mouseLeftDrag( &box ) && !combo.open && !dialogInQueue( NULL, spec_replace, NULL ) )
        {
            TrayObj ts = {0};
            buildSpecializationTrayObj( &ts, FALSE, iset, ipow, ispec );
            ts.icon = atlasLoadTexture( spec->ppowBase->pchIconName );
            trayobj_startDragging( &ts, ts.icon, pog );
            sndPlay( "Grab4", SOUND_GAME );
        }
    }


    //check for mouse release regardless of slot
    if( bDragging )
    {
         if( !isDown( MS_LEFT ) && mouseCollision( &box ) )
        {
            int idx = cursor.drag_obj.ispec;
            Boost *dragSpec = pchar->aBoosts[idx];
            sr.x = x;
            sr.y = y;
            sr.z = z;
            sr.iset = iset;
            sr.ipow = ipow;
            sr.ispec = ispec;
            sr.idx = idx;
            sr.pow = pow;

            if( spec ) {
                if (validBoostCombo(pchar,spec,dragSpec))
                {
                    //spec_startcombine();
                    combo_set( pow, 0,0);
                    combo.slotA = boost_Clone(spec);
                    combo.slotB = boost_Clone(dragSpec);
                }
                else if (dragSpec->ppowBase && !dragSpec->ppowBase->bBoostIgnoreEffectiveness &&
                         dragSpec->iLevel + dragSpec->iNumCombines < spec->iLevel + spec->iNumCombines ||
                         (dragSpec->iLevel == spec->iLevel && dragSpec->iNumCombines == spec->iNumCombines && 
                          dragSpec->ppowBase == spec->ppowBase))
                { //replace this with effectiveness check eventually
                    dialog( DIALOG_YES_NO, -1, -1, -1, -1, "ReallyBadReplaceEnhancement", NULL, spec_replace, NULL, NULL,0,0,0,1,0,0,0 );
                }
                else
                {
                    if (optionGet(kUO_HidePlacePrompt))
                        spec_replace(0);
                    else
                        dialog( DIALOG_YES_NO, -1, -1, -1, -1, "ReallyReplaceEnhancement", NULL, spec_replace, NULL, NULL,0,sendOptions,placeDCB,1,0,0,0 );
                }
            }
            else 
            {
                if (optionGet(kUO_HidePlacePrompt))
                    spec_replace(0);
                else
                    dialog( DIALOG_YES_NO, -1, -1, -1, -1, "ReallyPlaceEnhancement", NULL, spec_replace, NULL, NULL,0,sendOptions,placeDCB,1,0,0,0 );
            }

            sndPlay( "Reject5", SOUND_GAME );
            trayobj_stopDragging();
        }
    }

    return FALSE;
}

#define EQ_CENTER_Y 312
#define SLOT_AX     372
#define SLOT_CX     680
#define COMBO_SLOT_SPEED .15f
#define ENHANCE_WD      75
#define BORDER_SPACE    18
#define FRAME_TOP       38
#define FRAME_BOT       714
#define COMBO_BOT       490
#define INVENTORY_HT    90
#define INV_SPEED       30

static int s_drawMenuComboEnhancement( int slot, float x, float y, float z, float sc, int color, float screenScaleX, float screenScaleY )
{
    AtlasTex * ring = atlasLoadTexture( "EnhncTray_Ring.tga" );

    Boost **spec;
    SlotLoc *sl;
    CBox box;
    float tx, ty, tsc;
    AtlasTex * icon;
    float screenScale = MIN(screenScaleX, screenScaleY);

    const SalvageItem **pSal = NULL;

     if( slot == 0 )
    {
        spec = &combo.slotA;
        sl = &combo.locA;
    }
    else if( slot == 1  )
    {
        spec = &combo.slotB;
        sl = &combo.locB;
    }
    else
    {
        spec = &combo.slotC;
        sl = &combo.locC;
        pSal = NULL;
    }

    if( spec && (*spec) )
    {
        if( !sl->retract && sl->timer < 1 )
        {
            sl->timer += TIMESTEP*COMBO_SLOT_SPEED;
            if( sl->timer > 1 )
                sl->timer = 1;
        }

        if( sl->retract && sl->timer > 0 )
        {
            sl->timer -= TIMESTEP*COMBO_SLOT_SPEED;

            if( sl->timer < 0 )
            {
                sl->retract = 0;
                sl->timer = 0;

                if( combo.state == COMBO_SUCCESS )
                {
                    electric_init( SLOT_CX, EQ_CENTER_Y, 300, screenScale, 0, 45, 3, 0 );
                    combo_setLoc( 2, SLOT_CX, EQ_CENTER_Y, 0, 0, -1 );
                    electric_init( SLOT_CX, EQ_CENTER_Y, 120, screenScale, 1, 90, 20, 0 );
                    fadingText( SLOT_CX, 384, 160, screenScale, CLR_WHITE, CLR_WHITE, 120, "SuccessString" );
                    sndPlay( "happy4", SOUND_GAME );

                    boost_Destroy( combo.slotA );
                    boost_Destroy( combo.slotB );
                    combo.slotA = combo.slotB = 0;
                    combo.salSlotB = 0;
                    combo.state = COMBO_GLORY;
                }
                else if ( combo.state == COMBO_FAIL )
                {
                     if( slot == 0 )
                    {
                        if( combo.slotB != NULL && combo.slotB->ppowParent )
                        {
                            combo_setLoc( 1, COMBO_POW_X + (-2 + POWER_OFFSET) + ((combo.slotB->idx+1)*SPEC_WD)*1.5*screenScale/screenScaleX,
                                            COMBO_POW_Y + 20*1.5f*screenScale/screenScaleY, .5*1.5f, -1, 1 );
                        }
                        else if ( combo.slotA->ppowParent )
                        {
                            combo_setLoc( 1, COMBO_POW_X + (-2 + POWER_OFFSET) + ((combo.slotA->idx+1)*SPEC_WD)*1.5*screenScale/screenScaleX,
                                COMBO_POW_Y + 20*1.5f*screenScale/screenScaleY, .5*1.5f, -1, 1 );
                        }
                        else
                        {
                            float wd = (playerPtr()->pchar->iNumBoostSlots)*ENHANCE_WD + BORDER_SPACE;
                            combo_setLoc( 1, DEFAULT_SCRN_WD/2 - wd/2 + combo.slotB->idx*ENHANCE_WD*screenScale/screenScaleX + (ENHANCE_WD + BORDER_SPACE)*screenScale/(2*screenScaleX),
                                            COMBO_BOT*screenScaleY/screenScaleY, 1.f, -1, 1 );
                        }

                        boost_Destroy( combo.slotA );
                        combo.slotA = 0;
                    }
                      else if( slot == 1 )
                    {
                        if( combo.slotA->ppowParent )
                        {
                            combo_setLoc( 0, COMBO_POW_X + (-2 + POWER_OFFSET) + ((combo.slotA->idx+1)*SPEC_WD)*1.5*screenScale/screenScaleX,
                                            COMBO_POW_Y + 20*1.5f*screenScale/screenScaleY, .5*1.5f, -1, 1 );
                        }
                        else
                        {
                            float wd = (playerPtr()->pchar->iNumBoostSlots)*ENHANCE_WD + BORDER_SPACE;
                            combo_setLoc( 0, DEFAULT_SCRN_WD/2 - wd/2 + combo.slotA->idx*ENHANCE_WD*screenScale/screenScaleX + (ENHANCE_WD + BORDER_SPACE)*screenScale/(2*screenScaleX), 
                                COMBO_BOT*screenScale/screenScaleY, 1.f, -1, 1 );
                        }

                        boost_Destroy( combo.slotB );
                        combo.slotB = 0;
                    }

                    combo.state = COMBO_FAIL2;
                    fadingText( SLOT_CX*screenScaleX, 384*screenScaleY, 160, screenScale, CLR_RED, CLR_RED, 120, "FailureString" );
                }
                else if ( combo.state == COMBO_FAIL2 )
                {
                    if( slot == 0 )
                    {
                        boost_Destroy( combo.slotA );
                        combo.slotA = 0;
                    }
                    else
                    {
                        boost_Destroy( combo.slotB );
                        combo.slotB = 0;
                    }

                    combo.state = COMBO_DEFEAT;
                }
                else if( slot == 0 )
                    combo.state = COMBO_CHOOSE1;
                else if( slot == 2 )
                    combo.state = COMBO_CHOOSE1;

                ( *spec ) = 0;
            }
        }
    }

    if( spec && (*spec) )
    {
        tx = x + (sl->x-x)*(1-sl->timer);
        ty = y + (sl->y-y)*(1-sl->timer);
        tsc = sc + (sl->sc-sc)*(1-sl->timer);

        drawMenuEnhancement( *spec, tx*screenScaleX, ty*screenScaleY, z, tsc*screenScale, kBoostState_Normal, 0, false, current_menu(), combo.open);
    }

    if (slot == 1 && combo.salSlotB != NULL)
    {
        sl = &combo.locSalSlotB;
        pSal = &combo.salSlotB;
        icon = atlasLoadTexture( (*pSal)->ui.pchIcon );

        if( !sl->retract && sl->timer < 1 )
        {
            sl->timer += TIMESTEP*COMBO_SLOT_SPEED*0.5f;
            if( sl->timer > 1 )
                sl->timer = 1;
        }

        if( sl->retract && sl->timer > 0 )
        {
            sl->timer -= TIMESTEP*COMBO_SLOT_SPEED;

            if( sl->timer < 0 )
            {
                sl->retract = 0;
                sl->timer = 0;
                (*pSal) = NULL;

                if( combo.state == COMBO_FAIL )
                {

                    combo_setLoc( 0, COMBO_POW_X + (-2 + POWER_OFFSET) + ((combo.slotA->idx+1)*SPEC_WD)*1.5*screenScale/screenScaleX,
                        COMBO_POW_Y + 20*1.5f*screenScale/screenScaleY, .5*1.5f, -1, 1 );

                    combo.state = COMBO_FAIL2;
                    fadingText( SLOT_CX*screenScaleX, 384*screenScaleY, 160, screenScale, CLR_RED, CLR_RED, 120, "FailureString" );
                }
            }
        }

        tx = x - icon->width/2*tsc*screenScale/screenScaleX + (sl->x-x)*(1-sl->timer);
        ty = y - icon->height/2*tsc*screenScale/screenScaleY + (sl->y-y)*(1-sl->timer);
        tsc = sc + (sl->sc-sc)*(1-sl->timer);

        // HYBRID
        display_sprite( icon, tx*screenScaleX, ty*screenScaleY, 1500, tsc*screenScale, tsc*screenScale, 0xffffffff );
    }

    if( !spec || !(*spec) || tx != x || combo.state == COMBO_WAITING || combo.state == COMBO_FAIL )
        drawMenuEnhancement( NULL, x*screenScaleX, y*screenScaleY, z, sc*screenScale, kBoostState_Normal, 0, false, current_menu(), combo.open);

    BuildCBox( &box, x*screenScaleX-ring->width*sc*screenScale/2, y*screenScaleY-ring->height*sc*screenScale/2, ring->width*sc*screenScale, ring->height*sc*screenScale );

    if( mouseCollision(&box) )
    {
        if(spec && (*spec) && (*spec)->ppowBase)
            info_combineMenu( (void*)(*spec)->ppowBase, 1, 0, (*spec)->iLevel, (*spec)->iNumCombines );

        if( mouseDown( MS_LEFT ) )
            return TRUE;
    }

    return FALSE;
}


//
static uiTabControl *s_enhancementTabsOfTen;
static int s_computeTotalTabCount()
{
    Entity *e = playerPtr();
    int i;

    int count = (e->pchar->iNumBoostSlots + 9) / 10;
    for (i = e->pchar->iNumBoostSlots; i < CHAR_BOOST_MAX; ++i)
    {
        if (e->pchar->aBoosts[i])
        {
            count = (i + 9) / 10;
        }
    }

    return count;
}

static char* s_tabData[] = { "1", "2", "3", "4", "5", "6", "7" };
static s_initEnhancementTabs()
{
    char *pSelectedTab = NULL;
    int totalTabCount;
    int i;

    if (!s_enhancementTabsOfTen)
    {
        s_enhancementTabsOfTen = uiTabControlCreateEx(TabType_Undraggable, 0, 0, 0, 0, 0, 1);
    }
    else
    {
        pSelectedTab = (char *)uiTabControlGetSelectedData(s_enhancementTabsOfTen);
        uiTabControlRemoveAll(s_enhancementTabsOfTen);
    }

    totalTabCount = s_computeTotalTabCount();
    devassert(totalTabCount <= 7);
    if (totalTabCount > 1)
    {
        for (i = 0; i < totalTabCount; ++i)
        {
            uiTabControlAdd(s_enhancementTabsOfTen, s_tabData[i], s_tabData[i]);
        }
    }
    else
    {
        uiTabControlAdd(s_enhancementTabsOfTen, "All", s_tabData[0]);
    }

    if (pSelectedTab)
    {
        uiTabControlSelect(s_enhancementTabsOfTen, pSelectedTab);
    }
}

static int s_getTabColor()
{
    float x, y, z, wd, ht, sc;
    int color, back_color;
     if( !window_getDims( WDW_ENHANCEMENT, &x, &y, &z, &wd, &ht, &sc, &color, &back_color ) )
        return DEFAULT_HERO_WINDOW_COLOR;

    return color|0xff;
}

static void s_drawMenuEnhancementInventory( Entity * e, float centerX, float centerY, float z, float screenScaleX, float screenScaleY )
{
    int i;
    float scaledEnhanceWd = 0.7*ENHANCE_WD;
    float wd = 10*scaledEnhanceWd + BORDER_SPACE;
    float screenScale = MIN(screenScaleX, screenScaleY);
    static int y = FRAME_BOT;
    int tabColor;
    char *pSelectedTab;
    int tabIndex;

    s_initEnhancementTabs();

    if( y < centerY )
    {
        y += TIMESTEP*INV_SPEED;
        if( y > centerY )
            y = centerY;
    }
    else if( y > centerY )
    {
        y -= TIMESTEP*INV_SPEED;
        if( y < centerY )
            y = centerY;
    }

    font( &gamebold_12 );
    font_color( CLR_WHITE, SELECT_FROM_UISKIN( 0x88b6ffff, 0xaaaaaaff, CLR_WHITE ) );
    cprnt( centerX*screenScaleX, y*screenScaleY - INVENTORY_HT*screenScale/2 + FONT_HEIGHT*screenScale/2, z+5, screenScale, screenScale, "SpecializationsInventory" );

    drawLargeCapsule( centerX*screenScaleX, y*screenScaleY, z, wd*screenScale, screenScale );    

    tabColor = s_getTabColor();
    drawTabControl(s_enhancementTabsOfTen, centerX*screenScaleX-wd/2*screenScale+55.f*screenScale, y*screenScaleY-84.f/2*screenScale, z, wd - 2*R10*screenScale, TAB_HEIGHT, screenScale, tabColor, tabColor, TabDirection_Horizontal);

    pSelectedTab = (char *)uiTabControlGetSelectedData(s_enhancementTabsOfTen);
    tabIndex = atoi(pSelectedTab) - 1;

    for( i = 0; i < 10; i++ )
    {
        s_drawMenuInventoryEnhancement( e->pchar, 10*tabIndex + i, centerX - wd*screenScale/(2*screenScaleX) + (i*scaledEnhanceWd + (scaledEnhanceWd + BORDER_SPACE)/2)*screenScale/screenScaleX, y+6*screenScale, z, screenScaleX, screenScaleY );
    }
}

//
//

#define BAR_HEIGHT 38

int drawPowerAndSpecializations( Character *pchar, Power * pow, int x, int y, int z, float scale, int available, int combine, int selectable, int isCombining, float screenScaleX, float screenScaleY)
{
    int i, hit = FALSE, spec = -1, offset;
    AtlasTex *icon = atlasLoadTexture(pow->ppowBase->pchIconName);
    CBox box;
    float screenScale = MIN(screenScaleX, screenScaleY);
      if( stricmp( icon->name, "white" ) == 0 )
        icon = atlasLoadTexture( "Power_Missing.tga" );

    if (isCombining != combo.open)
        selectable = 0;

       if( pow->ppowBase->psetParent->eSystem == kPowerSystem_Powers )
    {
        offset = POWER_OFFSET;
        BuildCBox(&box,(x*screenScaleX)+(offset*screenScaleX),(y*screenScaleY)-(BAR_HEIGHT*screenScaleY/2*scale),POWER_WIDTH*scale*screenScaleX,BAR_HEIGHT*scale*screenScaleY );
        hit = drawCheckBar( (x*screenScaleX)+(offset*screenScaleX), y*screenScaleY, z, scale*screenScale, POWER_WIDTH*screenScaleX*scale, pow->ppowBase->pchDisplayName, 0, selectable, 0, 1, icon );
    }
    else
    {
        offset = SKILL_OFF;
        BuildCBox(&box,(x*screenScaleX)+(offset*screenScaleX),(y*screenScaleY)-(BAR_HEIGHT*screenScaleY/2*scale),SKILL_FRAME_WIDTH*screenScaleX-(offset*2)*scale*screenScaleX,BAR_HEIGHT*scale*screenScaleY );
        hit = drawCheckBar( (x*screenScaleX)+(offset*screenScaleX), y*screenScaleY, z, scale*screenScale, SKILL_FRAME_WIDTH*screenScaleX-(offset*2)*scale*screenScaleX, pow->ppowBase->pchDisplayName, 0, selectable, 0, 1, icon );
    }

    if( cursor.dragging )
        hit = 0;

    if (hit == D_MOUSEOVER && selectable && game_state.enableBoostDiminishing) {
        char *temp = getEffectiveString(pow,NULL,NULL,0);
        char *pTip = NULL;

        estrCreate(&pTip);
        estrConcatf(&pTip, "<color InfoBlue><b>%s</b></color><br>%s<br><br>", textStd(pow->ppowBase->pchDisplayName), 
                    textStd(pow->ppowBase->pchDisplayShortHelp));


        // Enhancements Allowed
        estrConcatf(&pTip, "%s:<br><color InfoBlue>", textStd("AllowedEnhancements"));
        for(i=0;i<eaiSize(&(int *)pow->ppowBase->pBoostsAllowed);i++)
        {
            int j;
            if(i>0)
            {
                estrConcatStaticCharArray(&pTip, ", ");
            }

            // TODO: This feels hackish. Maybe load_def should fiddle this
            //       array instead. --poz
            //mw 3.10.06 added guard here because it's everywhere else this calc is done, 
            //and there's reported crash here that I can't repro, so I'm doing this and hoping for the best
            //(subtracting off the number of origins seems insane and neither Jered nor CW can remember why its needed)
            if( pow->ppowBase->pBoostsAllowed[i] >= eaSize(&g_CharacterOrigins.ppOrigins) )
            {
                j = pow->ppowBase->pBoostsAllowed[i]-eaSize(&g_CharacterOrigins.ppOrigins);

                estrConcatCharString(&pTip, textStd(g_AttribNames.ppBoost[j]->pchDisplayName));
            }
        }
        estrConcatStaticCharArray(&pTip, "</color>");

        // Boost Set Categories Allowed
        if (eaSize(&pow->ppowBase->ppBoostSets) > 0)  
        {
            StashTable hGroups = stashTableCreateWithStringKeys(5, StashDeepCopyKeys);
            StashTableIterator iter;
            StashElement elem;

            estrConcatf(&pTip, "<br><br>%s:<br><color InfoBlue>", textStd("AllowedBoostCategories"));
            for(i=0;i<eaSize(&pow->ppowBase->ppBoostSets);i++) 
            {
                stashAddInt(hGroups, textStd(pow->ppowBase->ppBoostSets[i]->pchGroupName), 1, false);
            }    

            stashGetIterator(hGroups, &iter);
            i = 0;
            while(stashGetNextElement(&iter, &elem))
            {

                if(i>0)
                {
                    estrConcatStaticCharArray(&pTip, ", ");
                }

                estrConcatCharString(&pTip, textStd(stashElementGetStringKey(elem)));
                i++;
            }
            estrConcatStaticCharArray(&pTip, "</color>");
            stashTableDestroy(hGroups);
        }

        // current boost
        estrConcatf(&pTip, "<br>%s", temp);


        // Slotted Boosts for Boostsets
        if (eaSize(&pow->ppBoosts) > 0)
        {
            int *boostSetCount;
            const BoostSet **boostSet;

            eaCreateConst(&boostSet);
            eaiCreate(&boostSetCount);

            // make a list of which boost sets are in this power and how many of each type
            for (i = 0; i < eaSize(&pow->ppBoosts); i++) 
            {
                const Boost *pBoost = pow->ppBoosts[i];

                if (pBoost && pBoost->ppowBase->pBoostSetMember)
                {
                    int loc = eaPushUniqueConst(&boostSet, pBoost->ppowBase->pBoostSetMember);
                    if (loc + 1 > eaiSize(&boostSetCount))
                        eaiSetSize(&boostSetCount, loc + 1);
                    boostSetCount[loc]++;
                }
            }

            // find out what bonuses this makes the player eligible for
            if (eaSize(&boostSet) > 0)  
            { 
                estrConcatf(&pTip, "<br><br>%s", textStd("ActiveBoostSets"));

                for (i = 0; i < eaSize(&boostSet); i++)
                {    
                    BoostSet *pBoostSet = (BoostSet *) boostSet[i];
                    int j;

                    estrConcatf(&pTip, "<br><color InfoBlue>%s (%d/%d)</color>", textStd(pBoostSet->pchDisplayName), 
                        boostSetCount[i], eaSize(&pBoostSet->ppBoostLists));

                    for (j = 0; j < eaSize(&pBoostSet->ppBonuses); j++) 
                    {
                        if (pBoostSet->ppBonuses[j]->iMinBoosts == 1)
                            continue; 

                        if ((pBoostSet->ppBonuses[j]->iMinBoosts == 0 || boostSetCount[i] >= pBoostSet->ppBonuses[j]->iMinBoosts) &&
                            (pBoostSet->ppBonuses[j]->iMaxBoosts == 0 || boostSetCount[i] <= pBoostSet->ppBonuses[j]->iMaxBoosts) )
                        {
                            // this effect applies

                            if (pBoostSet->ppBonuses[j]->pBonusPower)
                                estrConcatf(&pTip, "<br><color SpringGreen>&nbsp;&nbsp;&nbsp;&nbsp;%s</color>", dealWithPercents(textStd(pBoostSet->ppBonuses[j]->pBonusPower->pchDisplayHelp)));

                            if (eaSize(&pBoostSet->ppBonuses[j]->ppAutoPowers) > 0)  
                            {
                                int k;
                                for (k = 0; k < eaSize(&pBoostSet->ppBonuses[j]->ppAutoPowers); k++)
                                {
                                    const BasePower *pAuto = pBoostSet->ppBonuses[j]->ppAutoPowers[k];
                                    estrConcatf(&pTip, "<br><color SpringGreen>&nbsp;&nbsp;&nbsp;&nbsp;%s</color>", dealWithPercents(textStd(pAuto->pchDisplayHelp)));
                                }
                            }
                        }
                    }
                }
            }

            eaDestroyConst(&boostSet);
            eaiDestroy(&boostSetCount);
        }

        if (isMenu(MENU_COMBINE_SPEC))
        {
            forceDisplayToolTip(&enhanceTip,&box,pTip,MENU_COMBINE_SPEC,0);
        }
        else if (isMenu(MENU_RESPEC))
            forceDisplayToolTip(&enhanceTip,&box,pTip,MENU_RESPEC,0);

    }

    if( hit == D_MOUSEOVER && (!combo.open || combo.pow == pow->ppowBase ) && selectable)
    {
        g_pPowMouseover = pow;
        info_combineMenu( pow, 0, 0, 0, 0 );
    }

    for( i = eaSize(&pow->ppBoosts)-1; i >= 0; i-- )
    {
        // do some availability checks here
        int clickable = TRUE;

        if( !available )
            clickable = FALSE;

        if(isMenu(MENU_RESPEC) )
        {
            drawRespecSpecialization( x + (-2 + offset) + ((i+1)*SPEC_WD) * scale*screenScale/screenScaleX, y + (20 * scale*screenScale/screenScaleY), //FONT_HEIGHT + (POWER_SPEC_HT-FONT_HEIGHT)/2,
                                      z+10, pchar, pow, pow->ppBoosts[i], kTrayItemType_SpecializationPower, pow->psetParent->idx, pow->idx, i, screenScaleX, screenScaleY );
        }
        else
        {
            int thit = drawMenuPowerEnhancement( pchar, pow->psetParent->idx, pow->idx, i, x + (-2 + offset) + ((i+1)*SPEC_WD)*scale*screenScale/screenScaleX,
                                                    y + 20*scale*screenScale/screenScaleY, z+40, scale*.5f, CLR_WHITE, isCombining, screenScaleX, screenScaleY );
            if( thit && (!combo.state || isCombining) )
            {
                if( combo.state == COMBO_CHOOSE1 && pow->ppBoosts[i]
                    && (boost_IsValidCombination(pow->ppBoosts[i], pow->ppBoosts[i]) || boost_IsBoostable(pchar->entParent, pow->ppBoosts[i]) || boost_IsCatalyzable(pow->ppBoosts[i])))
                {
                    combo_setLoc( 0,  x + (-2 + offset) + ((i+1)*SPEC_WD)*scale*screenScale/screenScaleX, y + 20*scale*screenScale/screenScaleY, scale*.5f, 0, 0 );
                    combo.slotA   = boost_Clone(pow->ppBoosts[i]);
                    sndPlay( "Select5", SOUND_GAME );
                }
                if( combo.state == COMBO_CHOOSE2 && combo.slotA != pow->ppBoosts[i] && pow->ppBoosts[i]
                    && boost_IsValidCombination( combo.slotA, pow->ppBoosts[i] ) )
                {
                    combo_setLoc( 1,  x + (-2 + offset) + ((i+1)*SPEC_WD)*scale*screenScale/screenScaleX, y + 20*scale*screenScale/screenScaleY, scale*.5f, 0, 0 );
                    combo.slotB  = boost_Clone(pow->ppBoosts[i]);
                    combo.changeup = 30;
                    combo.locSalSlotB.retract = 1;
                    sndPlay( "Select5", SOUND_GAME );
                }

                if( thit > hit )
                    hit = thit;
            }
        }
    }

    if( combine && combo.pow == pow->ppowBase )
    {
        drawFrame( PIX2, R27, (x-200)*screenScaleX, y*screenScaleY, 101, (DEFAULT_SCRN_WD - 2*(x-200))*screenScaleX, 352*screenScaleY, 1.f, 0xffffff60, SELECT_FROM_UISKIN( 0x0000ff0f, 0xff00000f, 0x1111550f ) );
        return spec;
    }

    if( hit == D_MOUSEHIT && !combo.open )
        return TRUE;
    else
        return FALSE;
}


#define COLOR_FADE      20.f
#define COMBO_HT        300
#define COMBO_ALPHA_MAX 240


static void combo_set( Power * pow, int x, int y )
{
    combo.open    = 1;
    combo.pow     = pow->ppowBase;
    combo.x = x;
    combo.y = y;
    electric_clearAll();
    sndPlay( "Changescreen", SOUND_GAME );
}

static void combo_clear()
{
    combo.open = 0;
    combo.pow = 0;
    combo.state = COMBO_NOOPTION;
    electric_clearAll();
    sndPlay( "Changescreen", SOUND_GAME );
}

enum
{
    HEADER_NONE,
    HEADER_X,
    HEADER_EXCLAMATION,
    HEADER_NUM1,
    HEADER_NUM2,
};

#define HEADER_CENTERY  50
#define HEADER_LEFT     60
#define HEADER_X        130
#define HEADER_WD       (DEFAULT_SCRN_WD - 2*HEADER_X)
#define HEADER_SC       1.5f

static void combo_header( int type, int clr, char * txt, float screenScaleX, float screenScaleY )
{
    AtlasTex * ring      = atlasLoadTexture( "EnhncComb_instruction_ring.tga" );
    AtlasTex * streakL   = atlasLoadTexture( "EnhncComb_instruction_streak_L.tga" );
    AtlasTex * streakM   = atlasLoadTexture( "EnhncComb_instruction_streak_MID.tga" );
    AtlasTex * streakR   = atlasLoadTexture( "EnhncComb_instruction_streak_R.tga" );
    AtlasTex * icon = NULL;

    float sc = HEADER_SC;
    float screenScale = MIN(screenScaleX, screenScaleY);
    display_sprite( ring, HEADER_LEFT*screenScaleX, HEADER_CENTERY*screenScaleY - ring->height*screenScale/2, 150, screenScale, screenScale, clr );

    display_sprite( streakR, HEADER_X*screenScaleX, HEADER_CENTERY*screenScaleY - streakR->height*screenScale/2, 150, screenScale, screenScale, clr );
    display_sprite( streakM, HEADER_X*screenScaleX + streakR->width*screenScale, HEADER_CENTERY*screenScaleY - streakM->height*screenScale/2, 150, (HEADER_WD*screenScaleX-streakL->width*screenScale-streakR->width*screenScale)/streakM->width, screenScale, clr );
    display_sprite( streakL, HEADER_X*screenScaleX + HEADER_WD*screenScaleX - streakL->width*screenScale, HEADER_CENTERY*screenScaleY - streakL->height*screenScale/2, 150, screenScale, screenScale, clr );

    font( &game_18 );

    font_color( (0xffffff00 | clr), (0xffffff00 | clr) );

    if( str_wd( font_grp, sc*screenScale, sc*screenScale, txt ) > 1024*.75*screenScaleX )
        sc = str_sc( font_grp, 1024*.75*screenScaleX, txt );
    else
        sc *= screenScale;
    cprntEx( DEFAULT_SCRN_WD*screenScaleX/2, HEADER_CENTERY*screenScaleY, 151, sc, sc, CENTER_Y | CENTER_X, txt );

    switch( type )
    {
    case HEADER_NUM1:
        {
            icon = atlasLoadTexture( "EnhncComb_instruction_ring_1.tga" );
        }break;
    case HEADER_NUM2:
        {
            icon = atlasLoadTexture( "EnhncComb_instruction_ring_2.tga" );
        }break;
    case HEADER_X:
        {
            icon = atlasLoadTexture( "EnhncComb_instruction_ring_x.tga" );
        }break;
    case HEADER_NONE:
        {
            icon = atlasLoadTexture( "EnhncComb_instruction_ring_wait.tga" );
        }break;
    case HEADER_EXCLAMATION:
        {
            icon = atlasLoadTexture( "EnhncComb_instruction_ring_exclamation.tga" );
        }break;
    }

    display_sprite( icon, HEADER_LEFT*screenScaleX + (ring->width - icon->width)*screenScale/2, HEADER_CENTERY*screenScaleY - icon->height*screenScale/2, 160, screenScale, screenScale, CLR_WHITE );

}

static int combo_powerEquipped()
{
    int i;
    Power *pow = comboPower();

    for( i = eaSize(&pow->ppBoosts)-1; i >= 0; i-- )
    {
        if( pow->ppBoosts[i] && pow->ppBoosts[i]->ppowBase )
            return TRUE;
    }

    return FALSE;
}

static int combo_inventoryEquipped()
{
    int i;
    Entity * e = playerPtr();

    for( i = 0; i < CHAR_BOOST_MAX; i++ )
    {
        if( e->pchar->aBoosts[i] && e->pchar->aBoosts[i]->ppowBase )
            return TRUE;
    }

    return FALSE;
}

static int combo_powerOrInventory()
{
    if( combo_inventoryEquipped() )
        return TRUE;
    else
        return combo_powerEquipped();
}


int combo_matchableSlot1()
{
    int i;
    Entity * e = playerPtr();
    Power * pow = comboPower();

    if( !combo.slotA )
        return FALSE;

    for( i = eaSize(&pow->ppBoosts)-1; i >= 0; i-- )
    {
        if( pow->ppBoosts[i] && pow->ppBoosts[i]->ppowBase )
        {
            if( combo.slotA != pow->ppBoosts[i] && boost_IsValidCombination( combo.slotA, pow->ppBoosts[i] ) )
                return TRUE;
        }
    }

    for( i = 0; i < e->pchar->iNumBoostSlots; i++ )
    {
        if( e->pchar->aBoosts[i] && e->pchar->aBoosts[i]->ppowBase )
        {
            if( boost_IsValidCombination( combo.slotA, e->pchar->aBoosts[i] ) )
                return TRUE;
        }
    }

    // check for Boostable
    if (boost_IsBoostable(e, combo.slotA))
    {
        SalvageInventoryItem *pSal = character_FindSalvageInv(e->pchar, "S_EnhancementBooster");

        if (pSal != NULL && pSal->amount > 0)
            return TRUE;
    }

    // check for Catalyzable
    if (boost_IsCatalyzable(combo.slotA))
    {
        SalvageInventoryItem *pSal = character_FindSalvageInv(e->pchar, "S_EnhancementCatalyst");

        if (pSal != NULL && pSal->amount > 0)
            return TRUE;
    }

    return FALSE;
}

static SalvageInventoryItem *pSelectedSal = NULL;

void onSalvageClick(SalvageInventoryItem *sal)
{
    pSelectedSal = sal;
}

#define LOOP_CLR        0x75ff6300
#define ALPHA2_SPEED    10
//
//
static void combo_drawArrowLoop( int top, float arrow_x, float arrow_y, float sc, int inc, float screenScaleX, float screenScaleY )
{
    AtlasTex * arrow     = atlasLoadTexture( "EnhncComb_guide_arrow.tga" );
    AtlasTex * arrow_ovr = atlasLoadTexture( "EnhncComb_arrow_overlay.tga" );
    AtlasTex * loopL     = atlasLoadTexture( "EnhncComb_loop_L.tga" );
    AtlasTex * loopM     = atlasLoadTexture( "EnhncComb_loop_MID.tga" );
    AtlasTex * loopR     = atlasLoadTexture( "EnhncComb_loop_R.tga" );
    AtlasTex * circ      = atlasLoadTexture( "EnhncComb_circular_highlight.tga" );

    static float timer = 0;

    float loopx, loopy, wd, angle, arrow_sc, circ_sc;
    int clr, over_clr;
    float screenScale = MIN(screenScaleX, screenScaleY);

    SalvageItem const *salBooster = salvage_GetItem("S_EnhancementBooster");
    SalvageItem const *salCatalyst = salvage_GetItem("S_EnhancementCatalyst");

    Entity *e = playerPtr();

    if( combo.failureTimer )
        return;

    if( top )
    {
        wd       = POWER_WIDTH*1.5f*screenScaleX;
        loopx    = (COMBO_POW_X+POWER_OFFSET)*screenScaleX-(32*sc*screenScale/2);
        loopy    = COMBO_POW_Y*screenScaleY;
        angle    = 0;
        arrow_y *= screenScaleY;
        arrow_sc = ABS(arrow_y-(loopy+loopL->height*sc*screenScale))/arrow->height;
        arrow_y -= arrow_sc*arrow->height;
    }
    else
    {
        wd = 10*0.7*ENHANCE_WD*screenScale + BORDER_SPACE*screenScale;
        loopx = (DEFAULT_SCRN_WD*screenScaleX - wd)/2;
        loopy = COMBO_BOT*screenScaleY - loopL->height*sc*screenScale;
        angle = PI;
        arrow_y *= screenScaleY;
        arrow_sc =  ABS(arrow_y-loopy)/arrow->height;
    }

    if( inc )
        timer += TIMESTEP*.075f;
    over_clr = (0xffffff00 | (int)((sinf(timer)+1)*128));
    clr = (LOOP_CLR | (int)(combo.alpha2 + (sinf(timer)+1)*31));

    display_sprite( loopL, loopx - loopL->width*sc*screenScale, loopy, 110, sc*screenScale, sc*screenScale, clr  );
    display_sprite( loopR, loopx + wd, loopy, 110, sc*screenScale, sc*screenScale, clr);
    display_sprite( loopM, loopx, loopy, 110, wd/loopM->width, sc*screenScale, clr );

    // checking for enhancement boosters and catalysts
    if (top && combo.state == COMBO_CHOOSE2)
    {
        float iconWidths = 0.0f;
        float iconHeight = 0.0f;
        SalvageInventoryItem *pSalHover = NULL;

        pSelectedSal = NULL;

        if (e && e->pchar && salBooster != NULL && character_SalvageCount(e->pchar, salBooster) > 0 && boost_IsBoostable(e, combo.slotA))
        {
            AtlasTex * icon = atlasLoadTexture( salBooster->ui.pchIcon );
            float xPos = ((DEFAULT_SCRN_WD-250)*screenScaleX - wd)/2;
            SalvageInventoryItem *pSal = character_FindSalvageInv(e->pchar, "S_EnhancementBooster");

            iconWidths = icon->width*sc*screenScale*1.3f + 40.0f*sc*screenScale;
            iconHeight = icon->height*sc*screenScale*1.5f;

            if (salvage_display(pSal, &growBig, 0, xPos , loopy - sc*screenScale*40.0f, 1000, sc*screenScale*1.3f, 
                                    NULL, NULL, onSalvageClick, false) != NULL)
                pSalHover = pSal;

            // HYBRID
            if (pSelectedSal == pSal)
            {
                combo.salSlotB = pSelectedSal->salvage;
                combo_setLoc(3, xPos/screenScaleX +icon->width/2, loopy/screenScaleY - 40.0f + icon->height/2, 1.2f, 0, 0);
                combo.locB.retract = 1;
                combo.changeup = 30;
                sndPlay( "Tally", SOUND_GAME );

            }
        }

        if (e && e->pchar && salCatalyst != NULL && character_SalvageCount(e->pchar, salCatalyst) > 0 && boost_IsCatalyzable(combo.slotA))
        {
            AtlasTex * icon = atlasLoadTexture( salCatalyst->ui.pchIcon );
            float xPos = ((DEFAULT_SCRN_WD-250)*screenScaleX - wd)/2 - iconWidths;
            SalvageInventoryItem *pSal = character_FindSalvageInv(e->pchar, "S_EnhancementCatalyst");

            iconWidths += icon->width*sc*screenScale*1.3f + 40.0f*sc*screenScale;
            iconHeight = icon->height*sc*screenScale*1.4f;

            if (salvage_display(pSal, &growBig, 0, xPos , loopy - sc*screenScale*40.0f, 1000, sc*screenScale*1.3f, 
                NULL, NULL, onSalvageClick, false) != NULL)
                pSalHover = pSal;

            if (pSelectedSal == pSal)
            {
                combo.salSlotB = pSelectedSal->salvage;
                combo_setLoc(3, xPos/screenScaleX +icon->width/2, loopy/screenScaleY - 40.0f + icon->height/2, 1.2f, 0, 0);
                combo.locB.retract = 1;
                combo.changeup = 30;
                sndPlay( "Tally", SOUND_GAME );
            }

        }

        if (iconWidths > 0.0f)
            drawMenuFrame( R12, ((DEFAULT_SCRN_WD-250)*screenScaleX - wd)/2 , loopy-60*sc*screenScale, 101, 
                            iconWidths + 20*sc*screenScale, iconHeight+ 75*sc*screenScale, 0xffffffff, 0x000000ff, 0 );

        // grow the current icon
        uigrowbig_Update(&growBig, pSalHover );

    }

    sc*=.8f;

    if( !top )
        arrow_y -= sinf(timer)*.1*arrow_sc*arrow->height;

    arrow_sc *= (1.0 + sinf(timer)*.1);

    display_rotated_sprite( arrow, arrow_x*screenScaleX - arrow->width*sc*screenScale/2, arrow_y, 110, sc*screenScale, arrow_sc, clr, angle, 0 );
    display_rotated_sprite( arrow_ovr, arrow_x*screenScaleX - arrow_ovr->width*sc*screenScale/2,  arrow_y + (top?0:(arrow->height - arrow_ovr->height)*arrow_sc),
                            111, sc*screenScale, arrow_sc, over_clr, angle, 0 );

    circ_sc = (.65 - sinf(timer)*.1);
    if( combo.state == COMBO_CHOOSE1 )
        display_sprite( circ, SLOT_AX*screenScaleX - circ->width*circ_sc*screenScale/2, EQ_CENTER_Y*screenScaleY - circ->height*circ_sc*screenScale/2, 110, circ_sc*screenScale, circ_sc*screenScale, clr );
    else if( combo.state == COMBO_CHOOSE2 )
        display_sprite( circ, (DEFAULT_SCRN_WD*screenScaleX - circ->width*circ_sc*screenScale)/2, EQ_CENTER_Y*screenScaleY - circ->height*circ_sc*screenScale/2, 110, circ_sc*screenScale, circ_sc*screenScale, clr );
}

#define FAILURE_TIME 90
//
//
static void combo_drawEquation(float screenScaleX, float screenScaleY)
{
    AtlasTex * equal     = atlasLoadTexture( "EnhncComb_equals.tga" );
    AtlasTex * plus      = atlasLoadTexture( "EnhncComb_plus.tga" );
    AtlasTex * result    = atlasLoadTexture( "EnhncComb_result_ring.tga" );
    AtlasTex * circ      = atlasLoadTexture( "EnhncComb_circular_highlight.tga" );

    int combine   = BUTTON_LOCKED;
    int resetable = 0;
    float screenScale = MIN(screenScaleX, screenScaleY);
    combo.changeup -= TIMESTEP;
    combo.failureTimer -= TIMESTEP;
    if( combo.failureTimer < 0 )
        combo.failureTimer = 0;

    if( !combo.failureTimer )
    {
        combo.alpha2 += TIMESTEP*ALPHA2_SPEED/2;
        if( combo.alpha2 > 64 )
            combo.alpha2 = 64;
    }
    else if( combo.failureTimer > FAILURE_TIME/2 )
    {
        combo.alpha2 += TIMESTEP*ALPHA2_SPEED;
        if( combo.alpha2 > 200 )
            combo.alpha2 = 200;
    }
    else if( combo.failureTimer < FAILURE_TIME/2 )
    {
        combo.alpha2 -= TIMESTEP*ALPHA2_SPEED;
        if( combo.alpha2 < 0 )
            combo.alpha2 = 0;
    } 

    display_sprite( plus,  (DEFAULT_SCRN_WD*screenScaleX/2 + SLOT_AX*screenScaleX - plus->width*screenScale)/2, EQ_CENTER_Y*screenScaleY - plus->height*screenScale/2, 150, screenScale, screenScale, (0xffffff00 | (int)combo.alpha) );
    display_sprite( equal, (DEFAULT_SCRN_WD*screenScaleX/2 + SLOT_CX*screenScaleX - equal->width*screenScale)/2 - 10, EQ_CENTER_Y*screenScaleY - equal->height*screenScale/2, 150, screenScale, screenScale, (0xffffff00 | (int)combo.alpha) );

    if( s_drawMenuComboEnhancement( 0, SLOT_AX, EQ_CENTER_Y, 200, 1.f, (0xffffff00 | (int)combo.alpha), screenScaleX, screenScaleY ) &&
        (combo.state == COMBO_CHOOSE1 || (combo.state == COMBO_CHOOSE2 && !combo.slotB && !combo.salSlotB)) )
    {
        combo.locA.retract = 1;
    }

    if( s_drawMenuComboEnhancement( 1, DEFAULT_SCRN_WD/2, EQ_CENTER_Y, 200, 1.f, (0xffffff00 | (int)combo.alpha), screenScaleX, screenScaleY ) && combo.state == COMBO_CHOOSE2 )
    {
        combo.locB.retract = 1;
        combo.locSalSlotB.retract = 1;
    }


    s_drawMenuComboEnhancement( 2, SLOT_CX, EQ_CENTER_Y, 200, 1.f, (0xffffff00 | (int)combo.alpha), screenScaleX, screenScaleY );

     display_sprite( result, SLOT_CX*screenScaleX - result->width*screenScale/2, EQ_CENTER_Y*screenScaleY - result->height*screenScale/2, 140, screenScale, screenScale, 0x22ff2288 );
       drawMenuFrame( R10, -50*screenScaleX, EQ_CENTER_Y*screenScaleY - result->height*screenScale/2 - PIX4*screenScaleX, 101*screenScaleY, DEFAULT_SCRN_WD*screenScaleX + 100*screenScaleX, (result->height + 2*PIX4)*screenScaleY, SELECT_FROM_UISKIN( 0x4488ff44, 0xffffff44, 0x33228844 ), SELECT_FROM_UISKIN( 0x4488ff22, 0xff000022, 0x11115522 ), 0 );

    font( &game_18 );
    font_color( CLR_WHITE, CLR_WHITE );

    if( combo.failureTimer > 0 )
    {
        display_sprite( circ, SLOT_CX*screenScaleX - circ->width*1.f*screenScale/2, EQ_CENTER_Y*screenScaleY - circ->height*1.f*screenScale/2, 110, screenScale, screenScale, (0xff000000 | (int)combo.alpha2) );
    }
    else if( combo.slotA && (combo.slotB || combo.salSlotB ) )
    {
        float chance = 0.0f;
            
        if (combo.slotB)
            chance = boost_CombineChance( combo.slotA, combo.slotB );
        else
            chance = boost_BoosterChance(combo.slotA);

        cprnt( SLOT_CX*screenScaleX, 250*screenScaleY, 160, screenScale, screenScale, "ChanceOfSuccess" );

        if( combo.changeup<=0 )
        {
            cprnt( SLOT_CX*screenScaleX, 384*screenScaleY, 160, 2.f*screenScale, 2.f*screenScale, "%2.0f%%", chance*100 );
            combine = 0;
        }
        else
        {
            cprnt( SLOT_CX*screenScaleX, 384*screenScaleY, 160, (1.f+(1.0*(30-combo.changeup)/30.0))*screenScale,( 1.f+(1.0*(30-combo.changeup)/30.0))*screenScale, "%2.0f%%", (float)(rand()%90+10) );
        }
    }
    else if ( !combo.slotA && (!combo.slotB || combo.salSlotB))
        resetable = BUTTON_LOCKED;

      if( D_MOUSEHIT == drawStdButton( 850*screenScaleX, EQ_CENTER_Y*screenScaleY, 150, 100*screenScaleX, 40*screenScaleY, CLR_GREEN, "CombineString", 1.5f*screenScale, (combine || combo.state >= COMBO_WAITING) ) )
    {
        if( combo.slotA && combo.slotB)
        {
            combo.state = COMBO_WAITING;
            combineSpec_send( combo.slotA, combo.slotB );
        } 
        else if (combo.slotA && combo.salSlotB)
        {
            // HYBRID
            combo.state = COMBO_WAITING;
            if (stricmp(combo.salSlotB->pchName, "S_EnhancementBooster") == 0)
            {
                combineBooster_send( combo.slotA );
            } else {
                combineCatalyst_send( combo.slotA );
            }
        }
    }

     if( D_MOUSEHIT == drawStdButton( 219*screenScaleX, EQ_CENTER_Y*screenScaleY, 150, 100*screenScaleX, 40*screenScaleY, 0xffff00ff, "ResetString", 1.5f*screenScale, resetable ) )
    {
        combo.locA.retract = 1;
        combo.locB.retract = 1;
        combo.locSalSlotB.retract = 1;
        sndPlay( "Minus", SOUND_GAME );
    }
}

//
//
void combo_ActOnResult( int success, int slot )
{
    Entity * e = playerPtr();

    if( combo.state != COMBO_WAITING )
        return; // something went horribly wrong here....what to do...fail silently?

    if( success )
    {
           if( (slot == 0 || (!combo.slotB || !combo.slotB->ppowParent)) &&
             e->pchar->ppPowerSets[combo.slotA->ppowParent->psetParent->idx]->ppPowers[combo.slotA->ppowParent->idx]->ppBoosts[combo.slotA->idx] )
            combo.slotC = e->pchar->ppPowerSets[combo.slotA->ppowParent->psetParent->idx]->ppPowers[combo.slotA->ppowParent->idx]->ppBoosts[combo.slotA->idx];
        else
            combo.slotC = e->pchar->ppPowerSets[combo.slotB->ppowParent->psetParent->idx]->ppPowers[combo.slotB->ppowParent->idx]->ppBoosts[combo.slotB->idx];

        combo_setLoc( 0, SLOT_CX, EQ_CENTER_Y, 1.f, 1, 1 );
        combo_setLoc( 1, SLOT_CX, EQ_CENTER_Y, 1.f, 1, 1 );
          combo.locC.timer = 1;
        combo.state = COMBO_SUCCESS;
    }
    else
    {
         combo.state = COMBO_FAIL;
        combo.failureTimer = FAILURE_TIME;
        sndPlay( "reject3", SOUND_GAME );

         if( slot == 1 )
            combo_setLoc( 0, SLOT_AX, 800, 1.f, -1, 1 );
        else
        {
            if (combo.slotB != NULL)
                combo_setLoc( 1, DEFAULT_SCRN_WD/2, 800, 1.f, -1, 1 );
            else
                combo_setLoc( 3, DEFAULT_SCRN_WD/2, 800, 1.f, -1, 1 );
        }
    }
}

//
//
static void s_combo_update(float screenScaleX, float screenScaleY)
{
    Entity  *e = playerPtr();
     AtlasTex *back = white_tex_atlas;
    float   scale;

    if( !combo.open )
    {
        combo.alpha -= TIMESTEP*COLOR_FADE;

        if( combo.alpha < 0 )
        {
            memset( &combo, 0, sizeof(CombineBox) );
            combo.alpha = 0;
        }
    }

    if( combo.open )
    {
        combo.alpha += TIMESTEP*COLOR_FADE;

        if( combo.alpha > COMBO_ALPHA_MAX )
            combo.alpha = COMBO_ALPHA_MAX;
    }

    scale = (float)combo.alpha/COMBO_ALPHA_MAX;

    display_sprite( back, 0, 0, 100, (float)DEFAULT_SCRN_WD*screenScaleX/back->width, (float)DEFAULT_SCRN_HT*screenScaleY/back->height, combo.alpha );

    if( !combo.pow || !comboPower() )
        return;

    if( combo.alpha )
    {
        drawPowerAndSpecializations( e->pchar, comboPower(), COMBO_POW_X + (combo.x-COMBO_POW_X)*(1-scale),
                                     COMBO_POW_Y + (combo.y-COMBO_POW_Y)*(1-scale), 120, 1+(.5f*scale),  FALSE, 1, 1, 1, screenScaleX, screenScaleY );
    }

    if( combo.state == COMBO_WAITING ||combo.state == COMBO_FAIL || combo.state == COMBO_FAIL2 || combo.state == COMBO_SUCCESS ||
        combo.state == COMBO_GLORY || combo.state == COMBO_DEFEAT )
    {
        combo_header( HEADER_NONE, (0x22ff2200 | (int)combo.alpha), "CombiningInProgress", screenScaleX, screenScaleY );
        combo_drawEquation(screenScaleX, screenScaleY);
        return;
    }

    if( combo_powerOrInventory() )
    {
        if( combo_powerEquipped() )
        {
            if( combo_matchableSlot1() )
            {
                combo.state = COMBO_CHOOSE2;
                combo_drawEquation(screenScaleX, screenScaleY);
                combo_drawArrowLoop( 0, DEFAULT_SCRN_WD/2, 355, .7f, 1, screenScaleX, screenScaleY );
                combo_drawArrowLoop( 1, DEFAULT_SCRN_WD/2, 270, .7f, 0, screenScaleX, screenScaleY );
                combo_header( HEADER_NUM2, (0x22ff2200 | (int)combo.alpha), "ChooseSecond", screenScaleX, screenScaleY );
            }
            else
            {
                combo.state = COMBO_CHOOSE1;

                if( !combo.slotA )
                {
                     combo_drawEquation(screenScaleX, screenScaleY);
                     combo_header( HEADER_NUM1, (0x22ff2200 | (int)combo.alpha), "ChooseFirst", screenScaleX, screenScaleY );
                    combo_drawArrowLoop( 1, SLOT_AX, 270, .7f, 1, screenScaleX, screenScaleY );
                }
                else
                {
                    combo_drawEquation(screenScaleX, screenScaleY);
                    combo_header( HEADER_EXCLAMATION, (0xffff0000 | (int)combo.alpha), "ChooseBetter", screenScaleX, screenScaleY );
                    combo_drawArrowLoop( 1, SLOT_AX, 270, .7f, 1, screenScaleX, screenScaleY );
                }
            }
        }
        else
        {
            combo.state = COMBO_NOOPTION;
            combo_header( HEADER_X, (0xff000000 | (int)combo.alpha), "CantDoAnything", screenScaleX, screenScaleY );
        }
    }
    else
    {
        combo.state = COMBO_NOOPTION;
        combo_header( HEADER_X, (0xff000000 | (int)combo.alpha), "CantDoAnything", screenScaleX, screenScaleY );
    }


}

static TrayObj deleteMe = {0};

static void combo_deleteSpec(void * data)
{
    character_DestroyBoost(playerPtr()->pchar, deleteMe.ispec, NULL );
    spec_sendRemove( deleteMe.ispec );
    sndPlay( "Delete", SOUND_GAME );
///    dialogClearQueue(0);
}

static void combo_deletePowerSpec(void * data)
{
    Power *pPower = playerPtr()->pchar->ppPowerSets[deleteMe.iset]->ppPowers[deleteMe.ipow];
    int i;

    // need to flag all enhancements in this power to invalidate their tooltips
    for( i = eaSize(&pPower->ppBoosts)-1; i >= 0; i-- )
    {
        if (pPower->ppBoosts[i])
            uiEnhancementRefresh(pPower->ppBoosts[i]->pUI);
    }

    power_DestroyBoost( pPower , deleteMe.ispec, NULL );
    spec_sendRemoveFromPower( deleteMe.iset, deleteMe.ipow, deleteMe.ispec );
    sndPlay( "Delete", SOUND_GAME );

//    dialogClearQueue(0);
}




static void s_combo_trash( float tx, float ty, float z, float screenScaleX, float screenScaleY )
{
    static float timer = 0;
    AtlasTex *trash      = atlasLoadTexture( "trade_TrashIcon_rest.tga" );
    AtlasTex *trashover  = atlasLoadTexture( "trade_TrashIcon_mouseover.tga" );
    CBox box;
    float screenScale = MIN(screenScaleX, screenScaleY);
    BuildCBox( &box, tx*screenScaleX - 16*screenScale, ty*screenScaleY - 16*screenScale, 32*screenScale, 32*screenScale );
    drawMenuEnhancement( NULL, tx*screenScaleX, ty*screenScaleY, z, .5f*screenScale, kBoostState_Normal, 0, true, current_menu(), !combo.open && !cursor.dragging);

    if( cursor.dragging && ( mouseCollision(&box) || cursor.drag_obj.type == kTrayItemType_SpecializationPower ) )
    {
        timer += TIMESTEP;
        display_sprite( trashover, tx*screenScaleX - trashover->width*screenScale/2, ty*screenScaleY - 16*screenScale - trash->height*screenScale/2 + 5.f*sinf(timer)*screenScale, z+11, screenScale, screenScale, CLR_WHITE );
    }

    if( cursor.dragging && mouseCollision(&box) && !isDown(MS_LEFT) )
    {
        if( cursor.drag_obj.type == kTrayItemType_SpecializationInventory )
        {
            trayobj_copy( &deleteMe, &cursor.drag_obj, 0 );
            trayobj_stopDragging();
            if (optionGet(kUO_HideDeletePrompt))
                combo_deleteSpec(0);
            else
                dialog( DIALOG_YES_NO, -1, -1, -1, -1, "ReallyDeleteEnhancement", NULL, combo_deleteSpec, NULL, NULL,0,sendOptions,deleteDCB,1,0,0,0 );
            sndPlay( "Reject5", SOUND_GAME );
        }
        if( cursor.drag_obj.type == kTrayItemType_SpecializationPower )
        {
            trayobj_copy( &deleteMe, &cursor.drag_obj, 0 );
            trayobj_stopDragging();
            if (optionGet(kUO_HideDeletePrompt))
                combo_deletePowerSpec(0);
            else
                dialog( DIALOG_YES_NO, -1, -1, -1, -1, "ReallyDeleteEnhancement", NULL, combo_deletePowerSpec, NULL, NULL,0,sendOptions,deleteDCB,1,0,0,0 );
            sndPlay( "Reject5", SOUND_GAME );
        }
    }

    display_sprite( trash, tx*screenScaleX - trash->width*screenScale/2, ty*screenScaleY - 16*screenScale - trash->height*screenScale/2, z+10, screenScale, screenScale, CLR_WHITE );
}

//-----------------------------------------------------------------------------------------
// Main Loop /////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------------------
//

static void s_drawMenuForPowers( Entity * e, float screenScaleX, float screenScaleY ) 
{
    int i, primY, secY, poolY;
    static ScrollBar sb[3] = {0};
    int iSize;
    CBox box[3];
    int bShownPowersNumbers = false;
    float screenScale = MIN(screenScaleX, screenScaleY);

    // the frames
    drawMenuFrame( R12, FRAME_SPACE*screenScaleX, FRAME_TOP*screenScaleY, 10, FRAME_WIDTH*screenScaleX, FRAME_HEIGHT*screenScaleY, CLR_WHITE, 0x00000088, 0 ); // left
    BuildCBox(&box[0], FRAME_SPACE*screenScaleX, FRAME_TOP*screenScaleY, FRAME_WIDTH*screenScaleX, FRAME_HEIGHT*screenScaleY );
    drawMenuFrame( R12, (DEFAULT_SCRN_WD/2 - FRAME_WIDTH/2)*screenScaleX, FRAME_TOP*screenScaleY, 10, FRAME_WIDTH*screenScaleX, FRAME_HEIGHT*screenScaleY, CLR_WHITE, 0x00000088, 0 ); // middle
    BuildCBox(&box[1], (DEFAULT_SCRN_WD/2 - FRAME_WIDTH/2)*screenScaleX, FRAME_TOP*screenScaleY, FRAME_WIDTH*screenScaleX, FRAME_HEIGHT*screenScaleY );
    drawMenuFrame( R12, (DEFAULT_SCRN_WD - FRAME_WIDTH - FRAME_SPACE)*screenScaleX, FRAME_TOP*screenScaleY, 10, FRAME_WIDTH*screenScaleX, FRAME_HEIGHT*screenScaleY, CLR_WHITE, 0x00000088, 0 ); // right
    BuildCBox(&box[2], (DEFAULT_SCRN_WD - FRAME_WIDTH - FRAME_SPACE)*screenScaleX, FRAME_TOP*screenScaleY, FRAME_WIDTH*screenScaleX, FRAME_HEIGHT*screenScaleY );
    
    primY = secY = poolY = 85;
    primY -= sb[0].offset;
    secY  -= sb[1].offset;
    poolY -= sb[2].offset;

    // buttons to pick the frame to show the combat numbers
    if( D_MOUSEHIT == drawStdButton( (FRAME_SPACE+FRAME_WIDTH/2)*screenScaleX, 19*screenScaleY, 45, 120*screenScaleX, 20*screenScaleY, SELECT_FROM_UISKIN( CLR_BTN_HERO, CLR_BTN_VILLAIN, CLR_BTN_ROGUE ), (s_ShowCombatNumbersHere==1)?"HidePowerNumbers":"ShowCombatNumbersHere", screenScale, 0 ))
        (s_ShowCombatNumbersHere==1)?(s_ShowCombatNumbersHere = 0):(s_ShowCombatNumbersHere = 1);
    if( D_MOUSEHIT == drawStdButton( DEFAULT_SCRN_WD*screenScaleX/2, 19*screenScaleY, 45, 120*screenScaleX, 20*screenScaleY, SELECT_FROM_UISKIN( CLR_BTN_HERO, CLR_BTN_VILLAIN, CLR_BTN_ROGUE ), (s_ShowCombatNumbersHere==2)?"HidePowerNumbers":"ShowCombatNumbersHere", screenScale, 0 ))
        (s_ShowCombatNumbersHere==2)?(s_ShowCombatNumbersHere = 0):(s_ShowCombatNumbersHere = 2);
    if( D_MOUSEHIT == drawStdButton( (DEFAULT_SCRN_WD - FRAME_WIDTH/2 - FRAME_SPACE)*screenScaleX, 19*screenScaleY, 45, 120*screenScaleX, 20*screenScaleY, SELECT_FROM_UISKIN( CLR_BTN_HERO, CLR_BTN_VILLAIN, CLR_BTN_ROGUE ), (s_ShowCombatNumbersHere==3)?"HidePowerNumbers":"ShowCombatNumbersHere", screenScale, 0 ))
        (s_ShowCombatNumbersHere==3)?(s_ShowCombatNumbersHere = 0):(s_ShowCombatNumbersHere = 3);
    // draw the inventory
    if( !combo.open )
        s_drawMenuEnhancementInventory( e, DEFAULT_SCRN_WD/2, FRAME_BOT, 150, screenScaleX, screenScaleY );
    else
        s_drawMenuEnhancementInventory( e, DEFAULT_SCRN_WD/2, COMBO_BOT, 150, screenScaleX, screenScaleY );

    // draw all of the powers with specializations
    // The zeroth slot in power sets is for temporary powers, no specs for them
    set_scissor(1);

    iSize = eaSize(&e->pchar->ppPowerSets);
     for( i = 1; i < iSize; i++ )
    {
        int j, top;
        int iSize;
        scissor_dims( 0, (FRAME_TOP+PIX4)*screenScaleY, DEFAULT_SCRN_WD*screenScaleX, (FRAME_HEIGHT-2*PIX4-1)*screenScaleY );

        if(e->pchar->ppPowerSets[i]->psetBase->bShowInManage)
        {
            if( e->pchar->ppPowerSets[i]->psetBase->pcatParent == e->pchar->pclass->pcat[kCategory_Primary] )
            {
                if( s_ShowCombatNumbersHere == 1)
                {
                    if (!bShownPowersNumbers)
                    {
                        menuDisplayPowerInfo( FRAME_SPACE*screenScaleX, FRAME_TOP*screenScaleY, 30, FRAME_WIDTH*screenScaleX, FRAME_HEIGHT*screenScaleY,  g_pPowMouseover?g_pPowMouseover->ppowBase:0, g_pPowMouseover, POWERINFO_BOTH );
                        bShownPowersNumbers = true;
                    }
                }
                else
                {
                    top = primY;
                    setHeadingFontFromSkin(1);
                    cprnt( (FRAME_SPACE+FRAME_WIDTH/2)*screenScaleX, (top-23)*screenScaleY, 20, screenScale, screenScale, e->pchar->ppPowerSets[i]->psetBase->pchDisplayName );

                    iSize = eaSize(&e->pchar->ppPowerSets[i]->ppPowers);
                    for( j = 0; j < iSize; j++ )
                    {
                        Power *pow = e->pchar->ppPowerSets[i]->ppPowers[j];
                        if(pow && pow->ppowBase->bShowInManage)
                        {
                            if( drawPowerAndSpecializations( e->pchar, pow, FRAME_SPACE, primY, 20, 1.f, FALSE, 0, 1, 0, screenScaleX, screenScaleY ) && !combo.pow )
                            {
                                combo_set( pow, FRAME_SPACE, primY );
                            }
                            primY += POWER_SPEC_HT;
                        }
                    }

                    // draw the small inner frame
                    drawFrame( PIX2, R6, (FRAME_SPACE + POWER_OFFSET)*screenScaleX, (top - 30)*screenScaleY, 5, (POWER_WIDTH - 10)*screenScaleX, (primY - top - 35)*screenScaleY, 1.f, 0xffffffaa, SELECT_FROM_UISKIN( 0x0000ff44, 0xff000044, 0x00000066 ) );

                    primY += POWER_SPEC_HT;
                }
            }
            else if( e->pchar->ppPowerSets[i]->psetBase->pcatParent == e->pchar->pclass->pcat[kCategory_Secondary] )
            {
                if( s_ShowCombatNumbersHere == 2 )
                {
                    if (!bShownPowersNumbers)
                    {
                        menuDisplayPowerInfo( (DEFAULT_SCRN_WD/2 - FRAME_WIDTH/2)*screenScaleX, FRAME_TOP*screenScaleY, 30, FRAME_WIDTH*screenScaleX, FRAME_HEIGHT*screenScaleY, g_pPowMouseover?g_pPowMouseover->ppowBase:0, g_pPowMouseover, POWERINFO_BOTH );
                        bShownPowersNumbers = true;
                    }
                }
                else
                {
                    top = secY;

                    setHeadingFontFromSkin(1);
                    cprnt( DEFAULT_SCRN_WD*screenScaleX/2, (top-23)*screenScaleY, 20, screenScale, screenScale, e->pchar->ppPowerSets[i]->psetBase->pchDisplayName );

                    iSize = eaSize(&e->pchar->ppPowerSets[i]->ppPowers);
                    for( j = 0; j < iSize; j++ )
                    {
                        Power *pow = e->pchar->ppPowerSets[i]->ppPowers[j];
                        if(pow && pow->ppowBase->bShowInManage)
                        {
                            if( drawPowerAndSpecializations( e->pchar, pow, DEFAULT_SCRN_WD/2 - FRAME_WIDTH/2, secY, 20, 1.f, FALSE, 0, 1, 0, screenScaleX, screenScaleY ) && !combo.pow )
                            {
                                combo_set( pow, DEFAULT_SCRN_WD/2 - FRAME_WIDTH/2, secY );
                            }
                            secY += POWER_SPEC_HT;
                        }
                    }

                    // draw the small inner frame
                    drawFrame( PIX2, R6, (DEFAULT_SCRN_WD/2 - FRAME_WIDTH/2 + POWER_OFFSET)*screenScaleX, (top - 30)*screenScaleY, 5, (POWER_WIDTH - 10)*screenScaleX, (secY - top - 35)*screenScaleY, 1.f, 0xffffffaa, SELECT_FROM_UISKIN( 0x0000ff44, 0xff000044, 0x00000066 ) );

                    secY += POWER_SPEC_HT;
                }
            }
            else
            {
                if( s_ShowCombatNumbersHere == 3 )
                {
                    if (!bShownPowersNumbers)
                    {
                        menuDisplayPowerInfo( (DEFAULT_SCRN_WD - FRAME_WIDTH - FRAME_SPACE)*screenScaleX, FRAME_TOP*screenScaleY, 30, FRAME_WIDTH*screenScaleX, FRAME_HEIGHT*screenScaleY,  g_pPowMouseover?g_pPowMouseover->ppowBase:0, g_pPowMouseover, POWERINFO_BOTH );
                        bShownPowersNumbers = true;
                    }
                }
                else
                {
                    top = poolY;

                    setHeadingFontFromSkin(1);
                    cprnt( (DEFAULT_SCRN_WD - FRAME_WIDTH/2 - FRAME_SPACE)*screenScaleX, (top-23)*screenScaleY, 20, screenScale, screenScale, e->pchar->ppPowerSets[i]->psetBase->pchDisplayName );

                    iSize = eaSize(&e->pchar->ppPowerSets[i]->ppPowers);
                    for( j = 0; j < iSize; j++ )
                    {
                        Power *pow = e->pchar->ppPowerSets[i]->ppPowers[j];
                        if(pow && pow->ppowBase->bShowInManage)
                        {
                            if ( drawPowerAndSpecializations( e->pchar, pow, DEFAULT_SCRN_WD - FRAME_WIDTH - FRAME_SPACE, poolY, 20, 1.f, FALSE, 0, 1, 0, screenScaleX, screenScaleY ) && !combo.pow )
                            {
                                combo_set( pow, DEFAULT_SCRN_WD - FRAME_WIDTH - FRAME_SPACE, poolY  );
                            }
                            poolY += POWER_SPEC_HT;
                        }
                    }

                    // draw the small inner frame
                    drawFrame( PIX2, R6, (DEFAULT_SCRN_WD - FRAME_WIDTH - FRAME_SPACE + POWER_OFFSET)*screenScaleX, (top - 30)*screenScaleY, 5, (POWER_WIDTH - 10)*screenScaleX, (poolY - top - 35)*screenScaleY, 1.f, 0xffffffaa, SELECT_FROM_UISKIN( 0x0000ff44, 0xff000044, 0x00000066 ) );

                    poolY += POWER_SPEC_HT;
                }
            }
        }
    }

    set_scissor(0);

     doScrollBarEx( &sb[0], (FRAME_HEIGHT-2*PIX4-1), (primY + sb[0].offset - POWER_SPEC_HT), (FRAME_SPACE+FRAME_WIDTH), FRAME_TOP, 50, &box[0], 0, screenScaleX, screenScaleY );
    doScrollBarEx( &sb[1], (FRAME_HEIGHT-2*PIX4-1), (secY  + sb[1].offset - POWER_SPEC_HT), (DEFAULT_SCRN_WD/2 + FRAME_WIDTH/2), FRAME_TOP, 50, &box[1], 0, screenScaleX, screenScaleY );
    doScrollBarEx( &sb[2], (FRAME_HEIGHT-2*PIX4-1), (poolY + sb[2].offset - POWER_SPEC_HT), (DEFAULT_SCRN_WD - FRAME_SPACE), (FRAME_TOP + R12), 50, &box[2], 0, screenScaleX, screenScaleY );

    if( sb[0].grabbed || sb[1].grabbed || sb[2].grabbed )
    {
        electric_clearAll();
        fadingText_clearAll();
    }
}

void combineSpecMenu()
{
    Entity *e = playerPtr();
    static int init = FALSE;
    static bool bShowSkills = false;

    int screenScalingDisabled = is_scrn_scaling_disabled();
    float screenScaleX = 1.f, screenScaleY = 1.f, screenScale;

    calc_scrn_scaling(&screenScaleX, &screenScaleY);
    screenScale = MIN(screenScaleX, screenScaleY);

     drawBackground( NULL ); 
    toggle_3d_game_modes( SHOW_NONE );

    set_scrn_scaling_disabled(1);
    if( !init )
    {
        init = TRUE;
        info_clear();
        enhanceTip.disableScreenScaling = true;
        addToolTip(&enhanceTip);
        powerInfoSetLevel(e->pchar->iCombatLevel);
        powerInfoSetClass( e->pchar->pclass );
        uigrowbig_Init( &growBig, 1.1, 10 );
    }

    if( entMode( e, ENT_DEAD ) )
    {
        info_clear();
        fadingText_clearAll();
        electric_clearAll();
        init = FALSE;
        window_setMode( WDW_INFO, WINDOW_DOCKED );
        start_menu( MENU_GAME );
    }

    s_combo_update(screenScaleX, screenScaleY);
    s_drawMenuForPowers(e, screenScaleX, screenScaleY);

    if( combo.open && !gElectricAction )
    {
        if( combo.state == COMBO_GLORY )
        {
            int i;

            Power * pow = comboPower();

              for( i = eaSize(&pow->ppBoosts)-1; i >= 0; i-- )
            {
                if( pow->ppBoosts[i] == combo.slotC )
                {
                    combo.locC.x  = COMBO_POW_X + (-2 + POWER_OFFSET + (i+1)*SPEC_WD)*1.5;
                    break;
                }
            }

            if( i < 0 )
            {
                 int meh = 0;
            }

            combo.locC.y  = COMBO_POW_Y + 20*1.5f;
            combo.locC.sc = 1.5f*.5f;
            combo.locC.retract = 1;
        }
        else if( combo.state == COMBO_DEFEAT )
            combo.state = COMBO_CHOOSE1;
    }

    // trash icon
    s_combo_trash( FRAME_SPACE + 30, FRAME_BOT - 25, 50, screenScaleX, screenScaleY );

    if( combo.open )
    {
        if( D_MOUSEHIT == drawStdButton( (DEFAULT_SCRN_WD - FRAME_SPACE - 60)*screenScaleX, (FRAME_BOT+25)*screenScaleY, 120, 100*screenScaleX, 40*screenScaleY, 0xff0000ff, "ExitString", 2.f*screenScale, 0 ) )
            combo_clear();
    }
    else if( D_MOUSEHIT == drawStdButton( (DEFAULT_SCRN_WD - FRAME_SPACE - 60)*screenScaleX, (FRAME_BOT+25)*screenScaleY, 20, 100*screenScaleX, 40*screenScaleY, 0x00ff00ff, "ExitString", 2.f*screenScale, 0 ) )
    {
        info_clear();
        fadingText_clearAll();
        electric_clearAll();
        dialogClearQueue( 0 );
        init = FALSE;
        window_setMode( WDW_INFO, WINDOW_DOCKED );
        start_menu( MENU_GAME );
        g_pPowMouseover = 0;
        s_ShowCombatNumbersHere=0;
    }

    drawHelpOverlay();
    set_scrn_scaling_disabled(screenScalingDisabled);
}


//#include "character_attribs.h"
/**********************************************************************func*
 * CalcBoosts
 *
 * Calculates the total boost for the given power. If a pBoost is given,
 * it replaces the Boost at the index it specifies.
 *
 */
void ClientCalcBoosts(const Power *ppow, CharacterAttributes *pattrStr, Boost *pboostReplace)
{
    // This is largely stolen from character_mod.c:character_AccrueBoosts()
    // It makes sense to maybe factor that function more, but to get stuff
    // working, I'll make a copy (since we're not exactly doing the same
    // thing here).

    Character *p = playerPtr()->pchar;
    int i;
    int iLevelEff;
    CharacterAttributes attrBoosts = {0};

    assert(ppow!=NULL);

    // This sets the actual set to use. Anyone who calls this function
    //   (players) will be using the power's scratchpad. Everyone else
    //   (critters) will be using the Character::attrStrength.
    // ppow->pattrStrength = &ppow->attrStrength;

    memset(pattrStr, 0, sizeof(*pattrStr));

    iLevelEff = character_CalcExperienceLevel(p);

    for(i=eaSize(&ppow->ppBoosts)-1; i>=0; i--)
    {
        Boost *pboost = ppow->ppBoosts[i];

        if(pboostReplace && pboostReplace->idx==i)
            pboost = pboostReplace;

        if(pboost!=NULL && pboost->ppowBase!=NULL)
        {
            int j;
            float fEff = boost_Effectiveness(pboost, playerPtr(), iLevelEff, 0);

            for(j=eaSize(&pboost->ppowBase->ppTemplates)-1; j>=0; j--)
            {
                float fMagnitude;
                const AttribModTemplate *ptemplate = pboost->ppowBase->ppTemplates[j];

                if(ptemplate->bBoostTemplate)
                {
                    if(ptemplate->boostModAllowed && !power_IsValidBoostMod(ppow, ptemplate))
                        continue;

                    // Theoretically, we should use mod_Fill here. However,
                    //   boosts are defined in a very limited way so it's been
                    //   optimized down to the basics.
                    if(ptemplate->offAttrib!=kSpecialAttrib_PowerChanceMod)
                    {
                        int iLevelBoost = pboost->iLevel + pboost->iNumCombines;

                        if (pboost->ppowBase->bBoostBoostable)
                            iLevelBoost = pboost->iLevel;
                        else if (pboost->ppowBase->bBoostUsePlayerLevel)
                            iLevelBoost = MIN(iLevelEff, pboost->ppowBase->iMaxBoostLevel - 1);

                        if(ptemplate->iVarIndex<0)
                            fMagnitude = class_GetNamedTableValue(p->pclass, ptemplate->pchTable, iLevelBoost);
                        else
                            fMagnitude = pboost->afVars[ptemplate->iVarIndex];

                        fMagnitude *= ptemplate->fScale;

                        // MAK - I modified this from IsExemplar to just testing for combat level limiting.
                        // I believe this is the intent
                        if (p->iCombatLevel < iLevelEff)
                        {
                            fMagnitude = boost_HandicapExemplar(p->iCombatLevel, iLevelEff, fMagnitude);
                        }

                        fMagnitude *= fEff;

                        // Theoretically, we should use mod_Process here. However,
                        //   nothing special is ever done with boosts at this point
                        //   so it's been optimized down to its bare essentials.
                        {
                            float *pf;
                            if (ptemplate->offAspect == offsetof(CharacterAttribSet, pattrStrength) &&
                                ptemplate->bIgnoresBoostDiminishing)
                            {
                                pf = ATTRIB_GET_PTR(pattrStr, ptemplate->offAttrib);
                            }
                            else
                            {
                                pf = ATTRIB_GET_PTR(&attrBoosts, ptemplate->offAttrib);
                            }
                            *pf += fMagnitude;
                        }
                    }
                }
            }
        }
    }

    for(i=eaSize(&ppow->ppGlobalBoosts)-1; i>=0; i--)
    {
        Boost *pboost = ppow->ppGlobalBoosts[i];
        
        if (pboost != NULL && pboost->ppowBase != NULL)
        {
            Power *globalBoost = character_OwnsPower(p, pboost->ppowBase);
            
            if (globalBoost)
            {
                int j;
                float fEff = boost_Effectiveness(pboost, playerPtr(), iLevelEff, 0);

                for(j=eaSize(&pboost->ppowBase->ppTemplates)-1; j>=0; j--)
                {
                    float fMagnitude;
                    const AttribModTemplate *ptemplate = pboost->ppowBase->ppTemplates[j];

                    if(ptemplate->bBoostTemplate)
                    {
                        if(ptemplate->boostModAllowed && !power_IsValidBoostMod(ppow, ptemplate))
                            continue;

                        // Theoretically, we should use mod_Fill here. However,
                        //   boosts are defined in a very limited way so it's been
                        //   optimized down to the basics.
                        if(ptemplate->offAttrib!=kSpecialAttrib_PowerChanceMod)
                        {
                            int iLevelBoost = pboost->iLevel + pboost->iNumCombines;

                            if(ptemplate->iVarIndex<0)
                                fMagnitude = class_GetNamedTableValue(p->pclass, ptemplate->pchTable, iLevelBoost);
                            else
                                fMagnitude = pboost->afVars[ptemplate->iVarIndex];

                            fMagnitude *= ptemplate->fScale;

                            // MAK - I modified this from IsExemplar to just testing for combat level limiting.
                            // I believe this is the intent
                            if (p->iCombatLevel < iLevelEff)
                            {
                                fMagnitude = boost_HandicapExemplar(p->iCombatLevel, iLevelEff, fMagnitude);
                            }

                            fMagnitude *= fEff;

                            // Theoretically, we should use mod_Process here. However,
                            //   nothing special is ever done with boosts at this point
                            //   so it's been optimized down to its bare essentials.
                            {
                                float *pf;
                                if (ptemplate->offAspect == offsetof(CharacterAttribSet, pattrStrength) &&
                                    ptemplate->bIgnoresBoostDiminishing)
                                {
                                    pf = ATTRIB_GET_PTR(pattrStr, ptemplate->offAttrib);
                                }
                                else
                                {
                                    pf = ATTRIB_GET_PTR(&attrBoosts, ptemplate->offAttrib);
                                }
                                *pf += fMagnitude;
                            }
                        }
                    }
                }
            }
        }
    }

    // Add the boost to the base strength and apply diminishing returns
    AggregateStrength(p, ppow->ppowBase, pattrStr, &attrBoosts);
}

#define BOOSTTYPES (23)

typedef struct BoostDescLine 
{
    int offset;
    char name[64];

    float baseVal;
    float replaceVal;
    float combineVal;

    char boostAllowedName1[32];
    char boostAllowedName2[32];
    char boostDisplayName1[32];
    char boostDisplayName2[32];

} BoostDescLine;

#define ADDBOOSTLINE(xx,y, z, y1, z1) {(offsetof(CharacterAttributes, f##xx)),#xx,0.0f,0.0f,0.0f, y, z, y1, z1 },

static BoostDescLine boostLines[] =
{
    ADDBOOSTLINE(DamageType[0],"Damage_Boost","Res_Damage_Boost","Damage_Boost","Res_Damage_Boost") //damage and resistance stand in for all types
    ADDBOOSTLINE(DamageType[7],"Heal_Boost","","Heal_Boost","") //Healing, oddly enough
    ADDBOOSTLINE(Endurance, "Recovery_Boost", "Endurance_Drain_Boost", "Recovery_Boost", "Endurance_Drain_Boost") //BOTH DRAIN AND REGEN - sappers
    ADDBOOSTLINE(ToHit, "Buff_ToHit_Boost", "Debuff_ToHit_Boost", "Buff_ToHit_Boost", "Debuff_ToHit_Boost") //also debuff    
    ADDBOOSTLINE(Defense, "Buff_Defense_Boost", "Debuff_Defense_Boost", "Buff_Defense_Boost", "Debuff_Defense_Boost" ) 
    ADDBOOSTLINE(SpeedRunning, "SpeedRunning_Boost", "Slow_Boost", "SpeedRunning_Boost", "Run_Slow_Boost" ) //also slow
    ADDBOOSTLINE(SpeedFlying, "SpeedFlying_Boost", "Slow_Boost", "SpeedFlying_Boost", "Fly_Slow_Boost" ) //also slow
    ADDBOOSTLINE(SpeedJumping, "Jump_Boost", "Slow_Boost", "Jump_Boost", "Jump_Slow_Boost" ) //also slow
    ADDBOOSTLINE(Taunt, "Taunt_Boost", "", "Taunt_Boost", "" )
    ADDBOOSTLINE(Confused, "Confuse_Boost", "", "Confuse_Boost", "")
    ADDBOOSTLINE(Terrorized, "Fear_Boost", "", "Fear_Boost", "")
    ADDBOOSTLINE(Held, "Hold_Boost", "", "Hold_Boost", "")
    ADDBOOSTLINE(Immobilized, "Immobilized_Boost", "", "Immobilized_Boost", "" )
    ADDBOOSTLINE(Stunned, "Stunned_Boost", "", "Stunned_Boost", "") //aka disoriented
    ADDBOOSTLINE(Sleep, "Sleep_Boost", "", "Sleep_Boost", "")
    ADDBOOSTLINE(Intangible, "Intangible_Boost", "", "Intangible_Boost", "")
    ADDBOOSTLINE(Knockback, "Knockback_Boost", "", "Knockback_Boost", "")
    ADDBOOSTLINE(Accuracy, "Accuracy_Boost", "", "Accuracy_Boost", "")
    ADDBOOSTLINE(Range, "Range_Boost", "", "Range_Boost", "")
    ADDBOOSTLINE(RechargeTime, "Recharge_Boost", "Slow_Boost", "Recharge_Boost", "Recharge_Slow_Boost")
    ADDBOOSTLINE(InterruptTime, "Interrupt_Boost", "", "Interrupt_Boost", "" )
    ADDBOOSTLINE(EnduranceDiscount, "EnduranceDiscount_Boost", "", "EnduranceDiscount_Boost", "" )
    ADDBOOSTLINE(Recovery, "Recovery_Boost", "Endurance_Drain_Boost", "Recovery_Boost", "Endurance_Drain_Boost" ) // recovery rate
};

/* Lines that are actually enhanced, but we are hiding:
ADDBOOSTLINE(DamageType[1]) // only check for smashing and healing damage for display purposes
ADDBOOSTLINE(DamageType[2])
ADDBOOSTLINE(DamageType[3])
ADDBOOSTLINE(DamageType[4])
ADDBOOSTLINE(DamageType[5])
ADDBOOSTLINE(DamageType[6])
ADDBOOSTLINE(DamageType[8])
ADDBOOSTLINE(DamageType[9])
ADDBOOSTLINE(DamageType[10])
ADDBOOSTLINE(DamageType[11])
ADDBOOSTLINE(DamageType[12])
ADDBOOSTLINE(DamageType[13])
ADDBOOSTLINE(DamageType[14])
ADDBOOSTLINE(DamageType[15])
ADDBOOSTLINE(DamageType[16])
ADDBOOSTLINE(DamageType[17])
ADDBOOSTLINE(DamageType[18])
ADDBOOSTLINE(DamageType[19])
ADDBOOSTLINE(DefenseType[0])
ADDBOOSTLINE(DefenseType[1])
ADDBOOSTLINE(DefenseType[2])
ADDBOOSTLINE(DefenseType[3])
ADDBOOSTLINE(DefenseType[4])
ADDBOOSTLINE(DefenseType[5])
ADDBOOSTLINE(DefenseType[6])
ADDBOOSTLINE(DefenseType[7])
ADDBOOSTLINE(DefenseType[8])
ADDBOOSTLINE(DefenseType[9])
ADDBOOSTLINE(DefenseType[10])
ADDBOOSTLINE(DefenseType[11])
ADDBOOSTLINE(DefenseType[12])
ADDBOOSTLINE(DefenseType[13])
ADDBOOSTLINE(DefenseType[14])
ADDBOOSTLINE(DefenseType[15])
ADDBOOSTLINE(DefenseType[16])
ADDBOOSTLINE(DefenseType[17])
ADDBOOSTLINE(DefenseType[18])
ADDBOOSTLINE(DefenseType[19])
ADDBOOSTLINE(OnlyAffectsSelf) //enhanced by intangibility
ADDBOOSTLINE(Knockup) //enhanced by knockback
ADDBOOSTLINE(Untouchable) //enhanced by intangibility
ADDBOOSTLINE(Afraid) //also enhanced by fear
ADDBOOSTLINE(JumpHeight) //enhanced by jump
ADDBOOSTLINE(Recovery) //goes with endurance
*/

char *getBoostName(char *name)
{
    static char result[64];
    strcpy(result,"boost_");
    strcat(result,name);
    return textStd(result);
}


char *getEffectiveString(Power *ppow, Boost *oldboost, Boost *newboost, int idx)
{
    static StuffBuff result;
    int i;
    CharacterAttributes attrBase;
    Boost temp;
    bool isReplace = false;
    bool isCombine = false;
    bool isNonAutoPet = 0;
    Entity *e = playerPtr();

    if (!result.buff)
        initStuffBuff(&result,1024);
    else
        clearStuffBuff(&result);

    for(i=0;i<eaSize(&ppow->ppowBase->ppTemplates);i++)
    {
        if( ppow->ppowBase->ppTemplates[i]->pchEntityDef )
        {
            const VillainDef *pDef = villainFindByName(ppow->ppowBase->ppTemplates[i]->pchEntityDef);
            if( pDef && !pet_IsAutoOnly(pDef) )
                isNonAutoPet = 1;
        }
    }

    ClientCalcBoosts(ppow, &attrBase, NULL);
    for (i = 0; i < BOOSTTYPES; i++)
    {
        float *src = (float *)((char *)&attrBase + boostLines[i].offset);
        boostLines[i].baseVal = *src;
        boostLines[i].replaceVal = 0.0f;
        boostLines[i].combineVal = 0.0f;
    }

    if (newboost) 
    {
        isReplace = true;
        temp = *newboost;
        temp.idx = idx;
        ClientCalcBoosts(ppow, &attrBase, &temp);
        for (i = 0; i < BOOSTTYPES; i++) 
        {
            float *src = (float *)((char *)&attrBase + boostLines[i].offset);
            boostLines[i].replaceVal = *src - boostLines[i].baseVal;
        }
    }
    if (newboost && oldboost && validBoostCombo(e->pchar,oldboost,newboost))
    {
        isCombine = true;
        if (newboost->iLevel > oldboost->iLevel)
            temp = *newboost;
        else
            temp = *oldboost;
        temp.idx = idx;
        temp.iNumCombines++;

        ClientCalcBoosts(ppow, &attrBase, &temp);
        for (i = 0; i < BOOSTTYPES; i++)
        {
            float *src = (float *)((char *)&attrBase + boostLines[i].offset);
            boostLines[i].combineVal = *src - boostLines[i].baseVal;
        }
    }


    for (i = 0; i < BOOSTTYPES; i++)
    {
        int j, isAllowed = 0, allowedname1 = 0, allowedname2 = 0; // are they using boostAllowedName1?

        // is it allowed by the power specified boost?
        for( j =0 ; j < eaiSize(&ppow->ppowBase->pBoostsAllowed); j++ )
        {
            if( ppow->ppowBase->pBoostsAllowed[j] >= eaSize(&g_CharacterOrigins.ppOrigins) )
            {
                 char * allowedName = g_AttribNames.ppBoost[ppow->ppowBase->pBoostsAllowed[j]-eaSize(&g_CharacterOrigins.ppOrigins)]->pchName;
                
                if( stricmp( allowedName, boostLines[i].boostAllowedName1 ) == 0 )
                {
                    isAllowed = 1;
                    allowedname1 = 1;
                }
                else if    ( boostLines[i].boostAllowedName2 && 
                     stricmp( allowedName, boostLines[i].boostAllowedName2 ) == 0 )
                {
                    allowedname2 = 1;
                    isAllowed = 1;
                }
            }
        }

        /////////////////////////

        if ( !isAllowed || ((newboost || boostLines[i].baseVal == 0.0f) && boostLines[i].replaceVal == 0.0f && boostLines[i].combineVal == 0.0f))
        {
            continue;
        }

        if (result.idx == 0) 
        { 
            //set up the header
            addStringToStuffBuff(&result,"<table border=0><tr border=0><td border=0>%s</td>",textStd("BuffDebuffTitle"));
            if(!isNonAutoPet)
                addStringToStuffBuff(&result,"<td>%s</td>",textStd("ValueBase"));
            addStringToStuffBuff(&result,"<td>%s</td>",textStd("ValueTitle"));
            if (isCombine)
                addStringToStuffBuff(&result,"<td>%s</td>",textStd("CombineTitle"));
            else if (isReplace)
                addStringToStuffBuff(&result,"<td>%s</td>",textStd("ReplaceTitle"));
            addStringToStuffBuff(&result,"</tr><scale 0.8>");
        }
        if( !power_addEnhanceStats( ppow->ppowBase, boostLines[i].baseVal, boostLines[i].combineVal, boostLines[i].replaceVal, &result, !(isCombine||isReplace), boostLines[i].offset, 0, 0 ) && 
              ( (i != 16) || !power_addEnhanceStats( ppow->ppowBase, boostLines[i].baseVal, boostLines[i].combineVal, boostLines[i].replaceVal, &result, !(isCombine||isReplace), offsetof(CharacterAttributes, fKnockup), 0, 0 ) ))
        {  // Is this knockback?  try Knockup too
            
            // if that fail we must have a summon pet, we can't predict the values so just list base percents like we used to
            if( allowedname1 )
            {
                char * tempstr = textStd(boostLines[i].boostDisplayName1);
                 addSingleStringToStuffBuff(&result,"<tr border=0>");
                addStringToStuffBuff(&result, "<td>%s:</td>",tempstr);
                if(!isNonAutoPet)
                    addStringToStuffBuff(&result,"<td></td>");
                if (isCombine || isReplace)
                    addStringToStuffBuff(&result, "<td><color paragon>%.1f%%%%</color></td>",boostLines[i].baseVal*100.f);
                else
                    addStringToStuffBuff(&result, "<td><color paragon>%+.1f%%%%</color></td>",boostLines[i].baseVal*100.f);
                if (boostLines[i].replaceVal != 0.0f && boostLines[i].combineVal == 0.0f )
                {
                    // Yey for double translating
                    if (boostLines[i].replaceVal >= 0.0f)
                        addStringToStuffBuff(&result, "<td><color #00ff00>%+.1f%%%%</color></td>",boostLines[i].replaceVal*100.f);
                    else
                        addStringToStuffBuff(&result, "<td><color #ff0000>%+.1f%%%%</color></td>",boostLines[i].replaceVal*100.f);
                }
                if (boostLines[i].combineVal != 0.0f)
                {
                    // Yey for double translating
                    if (boostLines[i].combineVal >= 0.0f)
                        addStringToStuffBuff(&result, "<td><color #00ff00>%+.1f%%%%</color></td>",boostLines[i].combineVal*100.f);
                    else
                        addStringToStuffBuff(&result, "<td><color #ff0000>%+.1f%%%%</color></td>",boostLines[i].combineVal*100.f);
                }
                addStringToStuffBuff(&result,"</tr>");
            }
            if( allowedname2 )
            {
                char * tempstr = textStd(boostLines[i].boostDisplayName2);
                addSingleStringToStuffBuff(&result,"<tr border=0>");
                addStringToStuffBuff(&result, "<td>%s:</td>",tempstr);
                if(!isNonAutoPet)
                    addStringToStuffBuff(&result,"<td></td>");
                if (isCombine || isReplace)
                    addStringToStuffBuff(&result, "<td><color paragon>%.1f%%%%</color></td>",boostLines[i].baseVal*100.f);
                else
                    addStringToStuffBuff(&result, "<td><color paragon>%+.1f%%%%</color></td>",boostLines[i].baseVal*100.f);
                if (boostLines[i].replaceVal != 0.0f && boostLines[i].combineVal == 0.0f )
                {
                    // Yey for double translating
                    if (boostLines[i].replaceVal >= 0.0f)
                        addStringToStuffBuff(&result, "<td><color #00ff00>%+.1f%%%%</color></td>",boostLines[i].replaceVal*100.f);
                    else
                        addStringToStuffBuff(&result, "<td><color #ff0000>%+.1f%%%%</color></td>",boostLines[i].replaceVal*100.f);
                }
                if (boostLines[i].combineVal != 0.0f)
                {
                    // Yey for double translating
                    if (boostLines[i].combineVal >= 0.0f)
                        addStringToStuffBuff(&result, "<td><color #00ff00>%+.1f%%%%</color></td>",boostLines[i].combineVal*100.f);
                    else
                        addStringToStuffBuff(&result, "<td><color #ff0000>%+.1f%%%%</color></td>",boostLines[i].combineVal*100.f);
                }
                addStringToStuffBuff(&result,"</tr>");
            }
        }
    }
    if (result.idx > 0) //close the smf
        addStringToStuffBuff(&result,"</table></scale>");
    return result.buff;
}

