#include "UI/Hybrid/uiPlaystyle.h"
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
#include <utilitieslib/language/MessageStoreUtil.h>

#include "formatter/smf_parse.h"
#include "formatter/smf_format.h"
#include "formatter/smf_main.h"

#include "UI/uiInput.h"
#include "UI/uiUtilMenu.h"
#include "UI/uiUtil.h"
#include "UI/uiUtilGame.h"
#include "UI/uiClipper.h"
#include "UI/Hybrid/uiHybridMenu.h"

#include "entity/classes.h"
#include <utilitieslib/components/EString.h>

enum
{
    kPlaystyle_Tank,
    kPlaystyle_Melee,
    kPlaystyle_Range,
    kPlaystyle_Crowd,
    kPlaystyle_Support,
    kPlaystyle_Pet,
    kPlaystyle_Count,
};

HybridElement s_playstyles[] = 
{
    { kPlaystyle_Tank,        "TankString",            "PSTankDesc",            "Icon_ps_tank_0",            "Icon_ps_tank_1",            "Icon_ps_tank_2",            "playstyle_char_tank", },
    { kPlaystyle_Melee,        "MeleeString",            "PSMeleeDesc",            "Icon_ps_meleedamage_0",    "Icon_ps_meleedamage_1",    "Icon_ps_meleedamage_2",    "playstyle_char_meleedamage", },
    { kPlaystyle_Range,        "RangeDamageString",    "PSRangedDesc",            "Icon_ps_rangedamage_0",    "Icon_ps_rangedamage_1",    "Icon_ps_rangedamage_2",    "playstyle_char_rangedamage", },
    { kPlaystyle_Crowd,        "CrowdControlString",    "PSCrowdControlDesc",    "Icon_ps_crowedcontrol_0",    "Icon_ps_crowedcontrol_1",    "Icon_ps_crowedcontrol_2",    "playstyle_char_crowedcontrol", },
    { kPlaystyle_Support,    "SupportString",        "PSSupportDesc",        "Icon_ps_support_0",        "Icon_ps_support_1",        "Icon_ps_support_2",        "playstyle_char_support", },
    { kPlaystyle_Pet,        "PetString",            "PSPetsDesc",            "Icon_ps_pet_0",            "Icon_ps_pet_1",            "Icon_ps_pet_2",            "playstyle_char_pet", },
};
HybridElement * g_selected_playstyle = 0;
static HybridElement * g_displayed_playstyle = 0;

static char * sPlayStyleSounds[] = 
{
    "N_Playstyle_Tank",
    "N_Playstyle_MeleeDamage",
    "N_Playstyle_RangedDamage",
    "N_Playstyle_CrowdControl",
    "N_Playstyle_Support",
    "N_Playstyle_Pets",
};

const CharacterClass **playStyleClasses[kPlaystyle_Count];

void resetPlaystyleMenu()
{
    g_selected_playstyle = 0;
    g_displayed_playstyle = 0;
}

const CharacterClass ***getCurrentClassList()
{
    const CharacterClass*** classes;
    if( !g_selected_playstyle )
        classes = (const CharacterClass***)(&g_CharacterClasses.ppClasses); // why need the cast here?
    else
        classes = &playStyleClasses[g_selected_playstyle->index];
    return classes;
}


// no easy way to do this
static void sortPlayStyleClasses()
{
    int i;
    for( i = 0; i < eaSize(&g_CharacterClasses.ppClasses); i++ )
    {    
        const CharacterClass *pclass = g_CharacterClasses.ppClasses[i];

        if( stricmp(pclass->pchName, "Class_Blaster") == 0)
        {
            eaPushConst(&playStyleClasses[kPlaystyle_Range], pclass);
        }
        else if( stricmp(pclass->pchName, "Class_Brute") == 0)
        {
            eaPushConst(&playStyleClasses[kPlaystyle_Tank], pclass);
            eaPushConst(&playStyleClasses[kPlaystyle_Melee], pclass);
        }
        else if( stricmp(pclass->pchName, "Class_Controller") == 0)
        {
            eaPushConst(&playStyleClasses[kPlaystyle_Crowd], pclass);
            eaPushConst(&playStyleClasses[kPlaystyle_Support], pclass);
            eaPushConst(&playStyleClasses[kPlaystyle_Pet], pclass);
        }
        else if( stricmp(pclass->pchName, "Class_Corruptor") == 0)
        {
            eaPushConst(&playStyleClasses[kPlaystyle_Range], pclass);
            eaPushConst(&playStyleClasses[kPlaystyle_Support], pclass);
        }
        else if( stricmp(pclass->pchName, "Class_Defender") == 0)
        {
            eaPushConst(&playStyleClasses[kPlaystyle_Range], pclass);
            eaPushConst(&playStyleClasses[kPlaystyle_Support], pclass);
        }
        else if( stricmp(pclass->pchName, "Class_Dominator") == 0)
        {
            eaPushConst(&playStyleClasses[kPlaystyle_Crowd], pclass);
            eaPushConst(&playStyleClasses[kPlaystyle_Pet], pclass);
        }
        else if( stricmp(pclass->pchName, "Class_Mastermind") == 0)
        {
            eaPushConst(&playStyleClasses[kPlaystyle_Pet], pclass);
            eaPushConst(&playStyleClasses[kPlaystyle_Support], pclass);
        }
        else if( stricmp(pclass->pchName, "Class_Scrapper") == 0)
        {
            eaPushConst(&playStyleClasses[kPlaystyle_Melee], pclass);
        }
        else if( stricmp(pclass->pchName, "Class_Stalker") == 0)
        {
            eaPushConst(&playStyleClasses[kPlaystyle_Melee], pclass);
        }
        else if( stricmp(pclass->pchName, "Class_Tanker") == 0)
        {
            eaPushConst(&playStyleClasses[kPlaystyle_Tank], pclass);
        }
        else if( stricmp(pclass->pchName, "Class_Peacebringer") == 0)
        {
            eaPushConst(&playStyleClasses[kPlaystyle_Tank], pclass);
            eaPushConst(&playStyleClasses[kPlaystyle_Range], pclass);
        }
        else if( stricmp(pclass->pchName, "Class_Warshade") == 0)
        {
            eaPushConst(&playStyleClasses[kPlaystyle_Tank], pclass);
            eaPushConst(&playStyleClasses[kPlaystyle_Range], pclass);
        }
        else if( stricmp(pclass->pchName, "Class_Arachnos_Soldier") == 0)
        {
            eaPushConst(&playStyleClasses[kPlaystyle_Melee], pclass);
            eaPushConst(&playStyleClasses[kPlaystyle_Range], pclass);
            eaPushConst(&playStyleClasses[kPlaystyle_Support], pclass);
            eaPushConst(&playStyleClasses[kPlaystyle_Pet], pclass);
        }
        else if( stricmp(pclass->pchName, "Class_Arachnos_Widow") == 0)
        {
            eaPushConst(&playStyleClasses[kPlaystyle_Melee], pclass);
            eaPushConst(&playStyleClasses[kPlaystyle_Range], pclass);
            eaPushConst(&playStyleClasses[kPlaystyle_Support], pclass);
        }
        else if( stricmp(pclass->pchName, "Class_Primalist") == 0)
        {
            eaPushConst(&playStyleClasses[kPlaystyle_Melee], pclass);
            eaPushConst(&playStyleClasses[kPlaystyle_Support], pclass);
        }
    }
}



void playstyleMenu()
{
    static F32 img_sc = 0;
    static SMFBlock *smDesc, *smArch;
    int i, bReparse = 0, size = ARRAY_SIZE(s_playstyles);
    HybridElement * hovered_playstyle = 0;
    F32 xp, yp, xposSc, yposSc, textXSc, textYSc;

    if(drawHybridCommon(HYBRID_MENU_CREATION))
        return;

    if( !smDesc )
    {
        smDesc = smfBlock_Create();
        smArch = smfBlock_Create();
        for( i = 0; i < ARRAY_SIZE(s_playstyles); i++)
            hybridElementInit(&s_playstyles[i]);
        sortPlayStyleClasses();
    }

    // List of playstyles
    setScalingModeAndOption(SPRITE_Y_UNIFORM_SCALE, SSO_SCALE_TEXTURE);
    calculatePositionScales(&xposSc, &yposSc);
    textXSc = textYSc = 1.f;
    calculateSpriteScales(&textXSc, &textYSc);
    xp = 512.f;
    yp = 0.f;
    applyPointToScreenScalingFactorf(&xp, &yp); //use real pixel location

    for( i = 0; i < ARRAY_SIZE(s_playstyles); i++)
    {
        int flags = HB_TAIL_RIGHT_ROUND | HB_FLASH;
        int barRet = drawHybridBar(&s_playstyles[i], &s_playstyles[i] == g_selected_playstyle, xp - 84.f*xposSc, (130.f + 57*i)*yposSc, 10, 356, 1.f, flags, 1.f, H_ALIGN_RIGHT, V_ALIGN_CENTER );

         if( D_MOUSEHIT == barRet )
        {
            bReparse = 1;
            if( &s_playstyles[i] == g_selected_playstyle )
            {
                sndPlay( "N_Deselect", SOUND_GAME );
                g_selected_playstyle = 0;
            }
            else
            {
                sndPlay(sPlayStyleSounds[i], SOUND_UI_ALTERNATE);
                g_selected_playstyle = &s_playstyles[i];
            }
        }
        if ( D_MOUSEOVER == barRet )
        {
            hovered_playstyle = &s_playstyles[i];
        }
    }

    // image of playstyle
    if( img_sc <= 0 )
    {
        if (hovered_playstyle)
        {
            g_displayed_playstyle = hovered_playstyle;
        }
        else
        {
            g_displayed_playstyle = g_selected_playstyle;
        }
    }

    if( (!g_selected_playstyle && !hovered_playstyle) ||
        (!hovered_playstyle && g_selected_playstyle != g_displayed_playstyle) ||
        (hovered_playstyle && hovered_playstyle != g_displayed_playstyle))
    {
        img_sc = stateScale(img_sc,0);    
    }
    else
        img_sc = stateScale(img_sc,1);

    if( g_displayed_playstyle && g_displayed_playstyle->tex0 )
    {
          display_sprite_positional(g_displayed_playstyle->tex0, xp+198.f*xposSc, 350*yposSc, 7.f, 1.f, 1.f, 0xffffff00 | (int)(img_sc*255.f), H_ALIGN_CENTER, V_ALIGN_CENTER );
    }

    drawHybridDescFrame(xp, 480.f*yposSc, 5.f, 950.f, 200.f, HB_DESCFRAME_FADEDOWN, 0.75f, 1.f, 0.5f, H_ALIGN_CENTER, V_ALIGN_TOP);

    // text describing playstyle
    if( g_displayed_playstyle )
    {
        int textAlpha = img_sc * 255;

        // draw playstyle text
        setSpriteScaleMode(SPRITE_XY_NO_SCALE);
        font(&title_12);
        font_outl(0);
        font_color((CLR_BLACK&0xffffff00)|textAlpha , (CLR_BLACK&0xffffff00)|textAlpha);
        prnt(xp-450.f*xposSc+2.f, 504.f*yposSc+2.f, 5.f, textXSc, textYSc, g_displayed_playstyle->text);
        font_color((CLR_WHITE&0xffffff00)|textAlpha, (CLR_WHITE&0xffffff00)|textAlpha);
        prnt(xp-450.f*xposSc, 504.f*yposSc, 5.f, textXSc, textYSc, g_displayed_playstyle->text);
        font_outl(1);
        setSpriteScaleMode(SPRITE_Y_UNIFORM_SCALE);
        drawHybridDesc( smDesc, xp-430.f*xposSc, 520.f*yposSc, 5, 1.f, 445.f, 150.f, g_displayed_playstyle->desc1, bReparse,  (int)(img_sc*255.f)  );
    }
    else  //default text
    {
        int textAlpha = (1.f-img_sc) * 255;

        setSpriteScaleMode(SPRITE_XY_NO_SCALE);
        font(&title_12);
        font_outl(0);
        font_color((CLR_BLACK&0xffffff00)|textAlpha , (CLR_BLACK&0xffffff00)|textAlpha);
        prnt(xp-450.f*xposSc+2.f, 504.f*yposSc+2.f, 5.f, textXSc, textYSc, "PlaystyleSelectString");
        font_color((CLR_WHITE&0xffffff00)|textAlpha, (CLR_WHITE&0xffffff00)|textAlpha);
        prnt(xp-450.f*xposSc, 504.f*yposSc, 5.f, textXSc, textYSc, "PlaystyleSelectString");
        font_outl(1);
    }
    setScalingModeAndOption(SPRITE_XY_SCALE, SSO_SCALE_ALL);

    drawHybridNext( DIR_LEFT, 0, "BackString" );

    if (game_state.editnpc)
    {
        //reset playstyle and continue
        g_selected_playstyle = 0;
        goHybridNext(DIR_RIGHT);
    }
    else
    {
        drawHybridNext( DIR_RIGHT, 0, "NextString" );
    }
}
