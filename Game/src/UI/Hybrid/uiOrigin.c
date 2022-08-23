
#include "UI/Hybrid/uiOrigin.h"
#include "UI/uiGame.h"                // for start_menu

#include "UI/sprite/sprite_text.h"
#include "UI/sprite/sprite_base.h"
#include "UI/sprite/sprite_font.h"

#include "win/input.h"
#include <utilitieslib/components/earray.h>
#include "win/win_init.h" // for windowClientSize
#include "player/player.h"      // for playerPtr
#include "graphics/font.h"
#include "graphics/ttFontUtil.h"
#include "sound/sound.h"
#include "language/langClientUtil.h"
#include "graphics/textureatlas.h"
#include "cmdparse/cmdgame.h"  // for timestep
#include "player/inventory_client.h"

#include "formatter/smf_parse.h"
#include "formatter/smf_format.h"
#include "formatter/smf_main.h"

#include "UI/uiInput.h"
#include "UI/uiUtilMenu.h"
#include "UI/uiUtil.h"
#include "UI/uiUtilGame.h"
#include "UI/uiClipper.h"
#include "UI/Hybrid/uiHybridMenu.h"
#include "UI/uiLogin.h"
#include "UI/Hybrid/uiRedirect.h"
#include "UI/Hybrid/uiArchetype.h"
#include "entity/classes.h"
#include "UI/uiDialog.h"

#include "entity/origins.h"
#include "clientcomm/dbclient.h"
#include "auth/authUserData.h"
#include "clientcomm/authclient.h"
#include "entity/entPlayer.h"
#include "entity/entity.h"
#include "gameComm/initClient.h"
#include "LWC.h"

char *s_origins[][3] = 
{
    { "Icon_org_science_0",        "Icon_org_science_1",    "Icon_org_science_2", },
    { "Icon_org_mutation_0",    "Icon_org_mutation_1",    "Icon_org_mutation_2", },
    { "Icon_org_magic_0",        "Icon_org_magic_1",        "Icon_org_magic_2",     },
    { "Icon_org_technology_0",    "Icon_org_technology_1","Icon_org_technology_2",     },
    { "Icon_org_natural_0",        "Icon_org_natural_1",    "Icon_org_natural_2", },
};

static HybridElement **ppOrigins;
static HybridElement * powerOriginSelected = 0;
static HybridElement * powerOriginShowing = 0;
static int charOriginSelected = 0;  //COH Freedom (primal earth) default
static int charOriginShowing = 0;
static int firstPass;
static SMFBlock * smCharacterDesc;
static SMFBlock * smPowerDesc;
static F32 scaleCharacterOrigin = 1.f;
static F32 scalePowerOrigin = 0.f;

void resetOriginMenu()
{
    powerOriginSelected = 0;
    charOriginSelected = 0;
    charOriginShowing = 0;
    firstPass = 1;
    scaleCharacterOrigin = 1.f;
    scalePowerOrigin = 0.f;
    if (smCharacterDesc)
    {
        smfBlock_Destroy(smCharacterDesc);
        smCharacterDesc = NULL;
    }
    if (smPowerDesc)
    {
        smfBlock_Destroy(smPowerDesc);
        smPowerDesc = NULL;
    }
}

const CharacterOrigin * getSelectedOrigin()
{
    if( powerOriginSelected )
        return powerOriginSelected->pData;
    
    return 0;
}

void setSelectedOrigin(const char * originName)
{
    int i;
    for (i = eaSize(&ppOrigins)-1; i >= 0; --i)
    {
        if (strcmp(ppOrigins[i]->text, originName) == 0)
        {
            powerOriginSelected = ppOrigins[i];
            return;
        }
    }
}

static void createOriginHybridBar( const CharacterOrigin *pOrigin, char **originIcons )
{
    HybridElement * hb = calloc(1, sizeof(HybridElement) );
    hb->text = pOrigin->pchName;
    hb->desc1 = pOrigin->pchDisplayShortHelp;

    hb->iconName0 = originIcons[0];
    hb->iconName1 = originIcons[1];
    hb->iconName2 = originIcons[2];
    hb->pData = pOrigin;

    hybridElementInit(hb);

    eaPush(&ppOrigins, hb);
}

static HybridElement morality[] = 
{
    { 0, NULL, NULL, "icon_org_logo_freedom_0", "icon_org_logo_freedom_1"    },
    { 1, NULL, NULL, "icon_org_logo_rogue_0", "icon_org_logo_rogue_1"    },
};

int getPraetorianChoice()
{
    return charOriginSelected;
}

int startingLocationReady( int redirect )
{
     if(charOriginSelected >= 0) 
        return true;
    else
    {
        if( redirect )
        {
            clearRedirects();
            addRedirect(MENU_ORIGIN, "MustChooseHeroPraetorian", BLINK_HEROPRAETORIAN, startingLocationReady );    
        }
        return false;
            
    }
}

static void drawCharacterOrigins(F32 cx, F32 cy, F32 z, F32 wd)
{
       F32 space = 55.f;
     F32    blink = .5 + .5*getRedirectBlinkScale(BLINK_HEROPRAETORIAN);
    int canCreatePraetorian = authUserHasRogueAccess( (U32 *)db_info.auth_user_data )|| accountEval(inventoryClient_GetAcctInventorySet(), db_info.loyaltyStatus, db_info.loyaltyPointsEarned, (U32 *) db_info.auth_user_data, "ctexgoro StoreOwned?");
    int flags = 0;
    int result;
    int charOriginHover = -1;
    F32 xposSc, yposSc;
    calculatePositionScales(&xposSc, &yposSc);
    space *= yposSc;

     result = drawHybridBarLarge(&morality[0], cx, cy-space, z, wd, charOriginSelected==0, 0, blink );
    if (D_MOUSEHIT == result)
    {
        charOriginSelected = 0;
    }
    else if (D_MOUSEOVER <= result)
    {
        charOriginHover = 0;
    }

    if (!canCreatePraetorian)
    {
        flags |= HB_UNSELECTABLE;
        flags |= HB_GREY;

        //draw lock
        drawHybridStoreLockIcon(cx-25.f*xposSc, cy+space, z+1.f, 1.f, CLR_WHITE);

        //draw store icon
        if (D_MOUSEHIT == drawHybridStoreMenuButton("ctexgoro", cx+25.f*xposSc, cy+space, z+1.f, 1.f, CLR_WHITE, HB_STORE_PRODUCT))
        {
            if (LWC_IsActive() && LWC_GetLastCompletedDataStage() < LWC_GetRequiredDataStageForMapID(MAPID_NOVA_PRAETORIA) )
            {
                dialogStd( DIALOG_OK, "LWCPraetorianWarning", 0, 0, 0, 0, 0 );
            }
        }
    }
    
    result = drawHybridBarLarge(&morality[1], cx, cy+space, z, wd, charOriginSelected==1, flags, blink );
    if (D_MOUSEHIT == result)
    {
        if (LWC_IsActive() && LWC_GetLastCompletedDataStage() < LWC_GetRequiredDataStageForMapID(MAPID_NOVA_PRAETORIA) )
        {
            dialogStd( DIALOG_OK, "LWCPraetorianWarning", 0, 0, 0, 0, 0 );
        }
        charOriginSelected = 1;
    }
    else if (D_MOUSEOVER <= result)
    {
        charOriginHover = 1;
    }

    // hover scale code
    if( scaleCharacterOrigin <= 0 )
    {
        if (charOriginHover > -1)
        {
            charOriginShowing = charOriginHover;
        }
        else
        {
            charOriginShowing = charOriginSelected;
        }
    }

    if( (charOriginSelected == -1 && charOriginHover == -1) ||
        (charOriginHover == -1 && charOriginSelected != charOriginShowing) ||
        (charOriginHover > -1 && charOriginHover != charOriginShowing))
    {
        if (scaleCharacterOrigin == 1.f && charOriginHover > -1)
        {
            sndPlay("N_MouseEnter", SOUND_GAME);
        }
        scaleCharacterOrigin = stateScale(scaleCharacterOrigin,0);    
    }
    else
    {
        scaleCharacterOrigin = stateScale(scaleCharacterOrigin,1);
    }
}

static void drawPowerOrigins(F32 x, F32 y, F32 z, F32 barWd, F32 barHt, F32 alpha)
{
    int i;
    HybridElement * powerOriginHover = 0;
    F32 xposSc, yposSc;
    calculatePositionScales(&xposSc, &yposSc);
    for( i = 0; i < eaSize(&ppOrigins); i++ )
    {
        int flags = HB_TAIL_RIGHT_ROUND | HB_FLASH;
        int barRet = drawHybridBar( ppOrigins[i], ppOrigins[i] == powerOriginSelected, x, y + i*barHt*yposSc, z, barWd, 1.f, flags, alpha, H_ALIGN_LEFT, V_ALIGN_CENTER );

        if( D_MOUSEHIT == barRet )
        {
            char originSound[32] = "N_Origin_";
            strcat(originSound, ppOrigins[i]->text);
            sndPlay( originSound, SOUND_UI_ALTERNATE );
            powerOriginSelected = ppOrigins[i];
        }
        if ( D_MOUSEOVER == barRet )
        {
            powerOriginHover = ppOrigins[i];
        }
    }

    // hover scale code
    if( scalePowerOrigin <= 0 )
    {
        if (powerOriginHover)
        {
            powerOriginShowing = powerOriginHover;
        }
        else
        {
            powerOriginShowing = powerOriginSelected;
        }
    }

    if( (!powerOriginSelected && !powerOriginHover) ||
        (!powerOriginHover && powerOriginSelected != powerOriginShowing) ||
        (powerOriginHover && powerOriginHover != powerOriginShowing))
    {
        scalePowerOrigin = stateScale(scalePowerOrigin,0);    
    }
    else
    {
        scalePowerOrigin = stateScale(scalePowerOrigin,1);
    }
}

int invalidOriginArchetype()
{
    //verify the origin works with the current archetype
    const CharacterClass * pClass = getSelectedCharacterClass();
    if (pClass)
    {
        //check to see if this a specific archetype that has origin requirements
        if(    class_MatchesSpecialRestriction(pClass, "Kheldian") ||
            class_MatchesSpecialRestriction(pClass, "ArachnosSoldier") ||
            class_MatchesSpecialRestriction(pClass, "ArachnosWidow"))
        {
            const CharacterOrigin * pOrigin = getSelectedOrigin();
            if (!pOrigin || !origin_IsAllowedForClass(pOrigin, pClass) || charOriginSelected == 1)
            {
                return 1;
            }
        }
    }

    return 0;
}

void originMenu()
{
    int i;
       F32 alphasc = .5f + (getRedirectBlinkScale(BLINK_ORIGIN)*.5f);
    F32 xposSc, yposSc, textXSc, textYSc, xp, yp;

    if(drawHybridCommon(HYBRID_MENU_CREATION))
        return;

    setScalingModeAndOption(SPRITE_Y_UNIFORM_SCALE, SSO_SCALE_TEXTURE);
    calculatePositionScales(&xposSc, &yposSc);
    calculateSpriteScales(&textXSc, &textYSc);
    xp = DEFAULT_SCRN_WD / 2.f;
    yp = 0.f;
    applyPointToScreenScalingFactorf(&xp, &yp);
    if (firstPass)
    {
        firstPass = 0;
        doCreatePlayer(0);

        if (ppOrigins == NULL)
        {
            for( i = 0; i < eaSize(&g_CharacterOrigins.ppOrigins); i++ )
            {
                createOriginHybridBar(g_CharacterOrigins.ppOrigins[i], s_origins[i] );    
            }

            for(i = 0; i < ARRAY_SIZE(morality); i++ )
                hybridElementInit(&morality[i]);
        }
    }
    else
    {
        newCharacter();
    }

    if( !smPowerDesc )
    {
        smPowerDesc = smfBlock_Create();
    }
    if ( !smCharacterDesc )
    {
        smCharacterDesc = smfBlock_Create();
    }

     drawCharacterOrigins( xp - 270.f*xposSc, 230.f*yposSc, 5.f, 400.f );
    drawPowerOrigins( xp - 430.f*xposSc, 420.f*yposSc, 5.f, 354.f, 57.f, alphasc);

    drawHybridDescFrame(xp - 82.f*xposSc, 85.f*yposSc, 5.f, 550.f, 295.f, HB_DESCFRAME_FADELEFT, 0.5f, 1.f, 0.5f, H_ALIGN_LEFT, V_ALIGN_TOP);
    drawHybridDescFrame(xp - 82.f*xposSc, 385.f*yposSc, 5.f, 550.f, 295.f, HB_DESCFRAME_FADELEFT, 0.5f, 1.f, 0.5f, H_ALIGN_LEFT, V_ALIGN_TOP);
    setSpriteScaleMode(SPRITE_XY_NO_SCALE);
    font(&title_14);
    font_outl(0);
    font_color(CLR_H_DESC_TEXT_DS, CLR_H_DESC_TEXT_DS);
    prnt(xp + 60.f*xposSc+2.f, 109.f*yposSc+2.f, 5.f, textXSc, textYSc, "CharacterOriginString");
    prnt(xp + 60.f*xposSc+2.f, 409.f*yposSc+2.f, 5.f, textXSc, textYSc, "PowerOriginString");
    font_color(CLR_WHITE, CLR_WHITE);
    prnt(xp + 60.f*xposSc, 109.f*yposSc, 5.f, textXSc, textYSc, "CharacterOriginString");
    prnt(xp + 60.f*xposSc, 409.f*yposSc, 5.f, textXSc, textYSc, "PowerOriginString");
    font_outl(1);
    setSpriteScaleMode(SPRITE_Y_UNIFORM_SCALE);
    drawHybridDesc( smCharacterDesc, xp - 22*xposSc, 125.f*yposSc, 5.f, 1.f, 470.f, 300.f, charOriginShowing ? "PraetoriaIntroString" : "HeroIntroString", 0, 0xff*scaleCharacterOrigin );
     if( powerOriginShowing )
    {
        drawHybridDesc( smPowerDesc, xp - 22*xposSc, 425.f*yposSc, 5.f, 1.f, 470.f, 300.f, powerOriginShowing->desc1, 0, 0xff*scalePowerOrigin );
    }
    else
    {
        drawHybridDesc( smPowerDesc, xp - 22*xposSc, 425.f*yposSc, 5.f, 1.f, 470.f, 300.f, "SelectPowerOriginString", 0, 0xff*(1.f-scalePowerOrigin) );
    }

    setScalingModeAndOption(SPRITE_XY_SCALE, SSO_SCALE_ALL);

    if( drawHybridNext( DIR_LEFT, 0, "BackString" ) )
    {
        restartCharacterSelectScreen();
    }
    if (game_state.editnpc)
    {
        //set origins (if not set) and continue
        if (charOriginSelected < 0)
        {
            charOriginSelected = 0;
        }
        if (!powerOriginSelected)
        {
            assert(eaSize(&ppOrigins));
            powerOriginSelected = ppOrigins[0];
        }
        goHybridNext(DIR_RIGHT);
    }
    else
    {
          drawHybridNext( DIR_RIGHT, 0, "NextString" );
    }

    return;
}

void originMenuExitCode()
{
    if (invalidOriginArchetype())
    {
        //deselect the archetype
        deselectArchetype();

        //and popup a message to tell the player
        dialogStd( DIALOG_OK, "ArchetypeDeselectedText", 0, 0, 0, 0, 0 );
    }
}