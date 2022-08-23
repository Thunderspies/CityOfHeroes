
#include "UI/uiNet.h"
#include "UI/uiGame.h"
#include "UI/uiUtil.h"
#include "UI/uiTray.h"
#include "UI/uiInput.h"
#include "UI/uiDialog.h"
#include "uiPowers.h"
#include "UI/uiEditText.h"
#include "UI/uiUtilMenu.h"
#include "UI/uiScrollBar.h"
#include "UI/uiLevelPower.h"
#include "uiCombineSpec.h"
#include "UI/uiPowerInfo.h"

#include "UI/sprite/sprite_base.h"
#include "UI/sprite/sprite_text.h"
#include "UI/sprite/sprite_font.h"
#include "graphics/textureatlas.h"

#include <utilitieslib/components/earray.h>
#include "player/player.h"
#include "entity/powers.h"
#include "entity/origins.h"
#include "entity/classes.h"
#include "cmdparse/cmdgame.h"
#include "win/win_init.h"
#include "entity/entVarUpdate.h"
#include "entity/character_base.h"
#include "entity/character_level.h"
#include "clientcomm/clientcomm.h"
#include "storyarc/contactCommon.h"
#include "sound/sound.h"
#include "graphics/ttFontUtil.h"

#include "formatter/smf_main.h"
#include "entity/entity.h"
#include <utilitieslib/language/MessageStoreUtil.h>
#include <utilitieslib/utils/file.h>

static TextAttribs gTextAttr =
{
    /* piBold        */  (int *)0,
    /* piItalic      */  (int *)0,
    /* piColor       */  (int *)0x48ff48ff,
    /* piColor2      */  (int *)0,
    /* piColorHover  */  (int *)0x48ff48ff,
    /* piColorSelect */  (int *)0,
    /* piColorSelectBG*/ (int *)0x333333ff,
    /* piScale       */  (int *)(int)(1.f*SMF_FONT_SCALE),
    /* piFace        */  (int *)&game_12,
    /* piFont        */  (int *)0,
    /* piAnchor      */  (int *)0,
    /* piLink        */  (int *)0,
    /* piLinkBG      */  (int *)0,
    /* piLinkHover   */  (int *)0,
    /* piLinkHoverBG */  (int *)0,
    /* piLinkSelect  */  (int *)0,
    /* piLinkSelectBG*/  (int *)0x666666ff,
    /* piOutline     */  (int *)0,
    /* piShadow      */  (int *)0,
};


static NewPower gCurNewPower = {0};

#define FRAME_HT    520
#define POW_SPACER   50

static const char *gCurrHelpTxt = 0;
static AtlasTex * gCurrHelpIcon = 0;
static int gReparseHelpText = 0;
static PowerSet gTempSpecializationSet;

//--------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------
//
//
void powerSetHelp( AtlasTex * icon, const char * txt )
{
    gCurrHelpTxt = txt;
    gCurrHelpIcon = icon;
    gReparseHelpText = 1;
}

//
//
void displayPowerLevelHelp(float screenScaleX, float screenScaleY)
{
    // check here for box collisons
    CBox box;
    float screenScale = MIN(screenScaleX, screenScaleY);
    BuildCBox( &box, FRAME_SPACE*screenScaleX, 70*screenScaleY, FRAME_WIDTH*screenScaleX, FRAME_HT*screenScaleY );
    if( !mouseCollision( &box ) )
    {
        BuildCBox( &box, (DEFAULT_SCRN_WD/2 - FRAME_WIDTH/2)*screenScaleX, 70*screenScaleY, FRAME_WIDTH*screenScaleX, FRAME_HT*screenScaleY );
        if( !mouseCollision( &box ) )
        {
            BuildCBox( &box, (DEFAULT_SCRN_WD - FRAME_WIDTH - FRAME_SPACE)*screenScaleX, 70*screenScaleY, FRAME_WIDTH*screenScaleX, FRAME_HT*screenScaleY );
            if( !mouseCollision( &box ) )
            {
                gCurrHelpTxt = 0;
                gCurrHelpIcon = 0;
            }
        }
    }

    if( gCurrHelpTxt )
    {
        static SMFBlock *sm = 0;

        if (!sm)
        {
            sm = smfBlock_Create();
        }

        if( gCurrHelpIcon && stricmp( gCurrHelpIcon->name, "white" ) != 0 )
            display_sprite( gCurrHelpIcon, (85*screenScaleX) - (gCurrHelpIcon->width*screenScale/2), (645*screenScaleY) - (gCurrHelpIcon->height*screenScale/2), 16, screenScale, screenScale, CLR_WHITE );

        gTextAttr.piColor = (int *)(SELECT_FROM_UISKIN(0x48ff48ff, 0x48ff48ff, CLR_TXT_ROGUE));
        gTextAttr.piScale = (int*)(int)(1.f*SMF_FONT_SCALE*screenScale);
        smf_ParseAndDisplay(sm, textStd(gCurrHelpTxt), 125*screenScaleX, 615*screenScaleY, 20, 815*screenScaleX, 0, gReparseHelpText, false, &gTextAttr, NULL, 0, true);
        gReparseHelpText = 0;
    }
}

int powerLevelShowNew( NewPower *np, Character *pchar, int x, int y, int category, int *pSet, int *pPow, float screenScaleX, float screenScaleY )
{
    int oldY = y;
    float screenScale = MIN(screenScaleX, screenScaleY);
    if( (category == kCategory_Pool || category == kCategory_Epic) && pSet[category] >= 0 && pPow[category] >= 0 )
    {
        // send now power to the server or something
        np->baseSet = pchar->pclass->pcat[category]->ppPowerSets[pSet[category]];
        np->basePow = pchar->pclass->pcat[category]->ppPowerSets[pSet[category]]->ppPowers[pPow[category]];

        setHeadingFontFromSkin(0);
        prnt( (x-30)*screenScaleX, y*screenScaleY, 50, 1.2f*screenScale, 1.2f*screenScale, np->baseSet->pchDisplayName );
        y += 35;

        drawCheckBar( x*screenScaleX, y*screenScaleY, 50, screenScale, (FRAME_WIDTH-FRAME_OFFSET*2)*screenScaleX, np->basePow->pchDisplayName, 1, 1, 0, 0, NULL );
        y += POW_SPACER;
    }

    return y-oldY;
}

static int drawPowerSet(int x, int y, Character *pchar, int specLevel, PowerSet *pset, NewPower *np, int *pSet, int *pPow, bool forceUnavailable, float screenScaleX, float screenScaleY)
{
    int j;
    const BasePowerSet *psetBase = pset->psetBase;
    float screenScale = MIN(screenScaleX, screenScaleY);
    setHeadingFontFromSkin(1);
    prnt( (x-30)*screenScaleX, y*screenScaleY, 50, 1.2f*screenScale, 1.2f*screenScale, psetBase->pchDisplayName );
    y += 35;

    // loop though powers
    for( j = 0; j < eaSize( &psetBase->ppPowers ); j++ )
    {
        int mouse = FALSE;

        if ( character_OwnsPower( pchar, psetBase->ppPowers[j] ) )
        {
            mouse = drawCheckBar( x*screenScaleX, y*screenScaleY, 50, screenScale, (FRAME_WIDTH-FRAME_OFFSET*2)*screenScaleX, psetBase->ppPowers[j]->pchDisplayName, 1, 0, 0, 0, NULL );
        }
        else if ( np->basePow == psetBase->ppPowers[j] )
        {
            mouse = drawCheckBar( x*screenScaleX, y*screenScaleY, 50, screenScale, (FRAME_WIDTH-FRAME_OFFSET*2)*screenScaleX, psetBase->ppPowers[j]->pchDisplayName, 1, 1, 0, 0, NULL );
        }
        else
        {
            // check availability
            int available = character_CanBuyPower( pchar );
            if(forceUnavailable
                || powerset_BasePowerAvailableByIdx( pset, j, pchar->iLevel ) > 0
                || !character_IsAllowedToHavePowerAtSpecializedLevel(pchar, specLevel, psetBase->ppPowers[j]))
            {
                available = FALSE;
            }

            mouse = drawCheckBar( x*screenScaleX, y*screenScaleY, 50, screenScale, (FRAME_WIDTH-FRAME_OFFSET*2)*screenScaleX, psetBase->ppPowers[j]->pchDisplayName, 0, available, 0, 0, NULL );
        }

        if(mouse)
        {
            powerSetHelp( atlasLoadTexture(psetBase->ppPowers[j]->pchIconName), psetBase->ppPowers[j]->pchDisplayHelp );
            g_pBasePowMouseover = psetBase->ppPowers[j];
            g_pPowMouseover = character_OwnsPower( pchar, psetBase->ppPowers[j] );
            if( mouse == D_MOUSEHIT && !character_OwnsPower( pchar, psetBase->ppPowers[j] ))
            {
                sndPlay( "powerselect2", SOUND_GAME );
                np->pSet = pset;
                np->basePow = psetBase->ppPowers[j];

                pPow[kCategory_Pool] = -1;
                pSet[kCategory_Pool] = -1;
                pPow[kCategory_Epic] = -1;
                pSet[kCategory_Epic] = -1;

                // a negative index means this is the temporary powerset
                // for the specialization sets and thus the player will need
                // to buy the set on picking a power from it
                if (pset->idx < 0)
                {
                    np->baseSet = psetBase;
                }
                else
                {
                    np->baseSet = NULL;
                }
            }
        }

        y += POW_SPACER;
    } // end for every power

    return y;
}

int powerLevelSelector( NewPower *np, Character *pchar, int specLevel, int x, int y, int category, int *pSet, int *pPow, bool forceUnavailable, float screenScaleX, float screenScaleY )
{
    Entity *e = playerPtr();
    int i, j, displayedText = FALSE;
    PowerSet *pset;
    int oldY = y;

    setHeadingFontFromSkin(1);

    // find the power set corresponding to this category
    // The zeroth slot in power sets is for temporary powers, they don't show up here.
    for( i = 1; i < eaSize(&pchar->ppPowerSets); i++)
    {
        pset = pchar->ppPowerSets[i];

        if( pset->psetBase->pcatParent == pchar->pclass->pcat[category] )
        {
            y = drawPowerSet( x, y, pchar, specLevel, pset, np, pSet, pPow, forceUnavailable, screenScaleX, screenScaleY );
        } // end if power set is in category
    } // end for every power set

    // find any specialization sets for this category that could be available
    // in future (ie. only barred by level, not by requirements) but haven't
    // already been shown (ie. don't already have powers picked from)
    for( i = 0; i < eaSize(&pchar->pclass->pcat[category]->ppPowerSets); i++ )
    {
        const BasePowerSet *psetBase = pchar->pclass->pcat[category]->ppPowerSets[i];
        if( character_WillBeAllowedToSpecializeInPowerSet( pchar, psetBase ) )
        {
            bool found = false;

            for( j = 0; !found && j < eaSize(&pchar->ppPowerSets); j++)
            {
                if( psetBase == pchar->ppPowerSets[j]->psetBase )
                {
                    found = true;
                }
            }

            if( !found )
            {
                gTempSpecializationSet.idx = -1;
                gTempSpecializationSet.pcharParent = pchar;
                gTempSpecializationSet.psetBase = psetBase;
                gTempSpecializationSet.iLevelBought = pchar->iLevel;
                gTempSpecializationSet.ppPowers = NULL;

                y = drawPowerSet( x, y, pchar, specLevel, &gTempSpecializationSet, np, pSet, pPow, forceUnavailable, screenScaleX, screenScaleY );
            }
        }
    }

    return (y-oldY);
}

//------------------------------------------------------------------------------------------------------
// I'm in charge here! /////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------------------------------
static int init = FALSE;

void levelPowerMenu()
{
    Entity *e = playerPtr();
    static int iShowedPoolMessage = FALSE;
    static int ht;
    static int s_ShowCombatNumbersHere = 0;
    static ScrollBar sb[3] = { 0 };
    int y;
    bool bFakedLevel = false;
    bool bCanBuy = character_CanBuyPower(e->pchar);
    CBox box;
    int totalPowerCount = character_CountPowersBought(e->pchar);
    int screenScalingDisabled = is_scrn_scaling_disabled();
    float screenScaleX, screenScaleY, screenScale;

    calc_scrn_scaling(&screenScaleX, &screenScaleY);
    screenScale = MIN(screenScaleX, screenScaleY);

    // HACK: Pretend that we're already a level higher to get the right options
    if(totalPowerCount >= 2 && character_CalcExperienceLevel(e->pchar) > e->pchar->iLevel)
    {
        e->pchar->iLevel++;
        bFakedLevel = true;
    }

    drawBackground( NULL );
    toggle_3d_game_modes( SHOW_NONE );
    set_scrn_scaling_disabled(1);
    drawMenuTitle( 10*screenScaleX, 30*screenScaleY, 20, "ChooseANewPower", screenScale, !init );

    if( !init )
    {
        init = TRUE;
        gCurNewPower.basePow = NULL;
        gCurNewPower.pSet = NULL;
        g_pBasePowMouseover = 0;
        s_ShowCombatNumbersHere = 0;
        powerInfoSetLevel(e->pchar->iLevel);
        powerInfoSetClass(e->pchar->pclass);
    }

    if(!iShowedPoolMessage
        && eaiSize(&g_Schedules.aSchedules.piPoolPowerSet)>0
        && e->pchar->iLevel==g_Schedules.aSchedules.piPoolPowerSet[0])
    {
        dialogStd(DIALOG_OK, "FirstPowerPoolHelp", NULL, NULL, NULL, NULL, 0);
        iShowedPoolMessage = TRUE;
    }

    // 3 buttons to pick the frame to show the combat numbers
    if( D_MOUSEHIT == drawStdButton( (FRAME_SPACE+FRAME_WIDTH/2)*screenScaleX, 70*screenScaleY, 45, 120*screenScaleX, 30*screenScaleY, SELECT_FROM_UISKIN( CLR_BTN_HERO, CLR_BTN_VILLAIN, CLR_BTN_ROGUE ), (s_ShowCombatNumbersHere==1)?"HidePowerNumbers":"ShowCombatNumbersHere", screenScale, 0 ))
        (s_ShowCombatNumbersHere==1)?(s_ShowCombatNumbersHere = 0):(s_ShowCombatNumbersHere = 1);
    if( D_MOUSEHIT == drawStdButton( (DEFAULT_SCRN_WD/2)*screenScaleX , 70*screenScaleY, 45, 120*screenScaleX, 30*screenScaleY, SELECT_FROM_UISKIN( CLR_BTN_HERO, CLR_BTN_VILLAIN, CLR_BTN_ROGUE ), (s_ShowCombatNumbersHere==2)?"HidePowerNumbers":"ShowCombatNumbersHere", screenScale, 0 ))
        (s_ShowCombatNumbersHere==2)?(s_ShowCombatNumbersHere = 0):(s_ShowCombatNumbersHere = 2);

    set_scissor( TRUE );
    scissor_dims( 0, 99*screenScaleY, 1024*screenScaleX, (FRAME_HT - 34)*screenScaleY );

    // do power selectors
    if( s_ShowCombatNumbersHere == 1)
        menuDisplayPowerInfo( FRAME_SPACE*screenScaleX, 70*screenScaleY, 30, FRAME_WIDTH*screenScaleX, FRAME_HT*screenScaleY, g_pBasePowMouseover, g_pPowMouseover, POWERINFO_BASE );
    else
    {
        y = 130 - sb[0].offset;
        ht = powerLevelSelector( &gCurNewPower, e->pchar, e->pchar->iLevel, FRAME_SPACE + FRAME_OFFSET, y, kCategory_Primary, gCurrentPowerSet, gCurrentPower, totalPowerCount == 1, screenScaleX, screenScaleY );
        BuildCBox(&box, (FRAME_SPACE + FRAME_OFFSET)*screenScaleX, 99*screenScaleY, FRAME_WIDTH*screenScaleX, (FRAME_HT - 34)*screenScaleY );
        doScrollBarEx( &sb[0], (FRAME_HT - 34), ht, (FRAME_SPACE + FRAME_WIDTH), 99, 20, &box, 0, screenScaleX, screenScaleY );
    }

    if( s_ShowCombatNumbersHere == 2 )
        menuDisplayPowerInfo( (DEFAULT_SCRN_WD/2 - FRAME_WIDTH/2)*screenScaleX, 70*screenScaleY, 30, FRAME_WIDTH*screenScaleX, FRAME_HT*screenScaleY, g_pBasePowMouseover, g_pPowMouseover, POWERINFO_BASE );
    else
    {
        y = 130 - sb[1].offset;
        ht = powerLevelSelector( &gCurNewPower, e->pchar, e->pchar->iLevel, DEFAULT_SCRN_WD/2 - FRAME_WIDTH/2 + FRAME_OFFSET, y, kCategory_Secondary, gCurrentPowerSet, gCurrentPower, totalPowerCount == 0, screenScaleX, screenScaleY );
        BuildCBox(&box, (DEFAULT_SCRN_WD/2 - FRAME_WIDTH/2 + FRAME_OFFSET)*screenScaleX, 99*screenScaleY, FRAME_WIDTH*screenScaleX, (FRAME_HT - 34)*screenScaleY );
        doScrollBarEx( &sb[1], (FRAME_HT - 34), ht, (DEFAULT_SCRN_WD/2 + FRAME_WIDTH/2), 99, 20, &box, 0, screenScaleX, screenScaleY );
    }

    y = 130 - sb[2].offset;
    ht = powerLevelShowNew( &gCurNewPower, e->pchar, DEFAULT_SCRN_WD - FRAME_WIDTH + FRAME_OFFSET, y, kCategory_Pool, gCurrentPowerSet, gCurrentPower, screenScaleX, screenScaleY );
    ht += powerLevelShowNew( &gCurNewPower, e->pchar, DEFAULT_SCRN_WD - FRAME_WIDTH + FRAME_OFFSET, y+ht, kCategory_Epic, gCurrentPowerSet, gCurrentPower, screenScaleX, screenScaleY );
    ht += powerLevelSelector( &gCurNewPower, e->pchar, e->pchar->iLevel, DEFAULT_SCRN_WD - FRAME_WIDTH + FRAME_OFFSET, y+ht, kCategory_Pool, gCurrentPowerSet, gCurrentPower, false, screenScaleX, screenScaleY);
    ht += powerLevelSelector( &gCurNewPower, e->pchar, e->pchar->iLevel, DEFAULT_SCRN_WD - FRAME_WIDTH + FRAME_OFFSET, y+ht, kCategory_Epic, gCurrentPowerSet, gCurrentPower, false, screenScaleX, screenScaleY);
    set_scissor( FALSE );

    BuildCBox(&box, (DEFAULT_SCRN_WD - FRAME_WIDTH + FRAME_OFFSET)*screenScaleX, 99*screenScaleY, FRAME_WIDTH*screenScaleX, (FRAME_HT - 34)*screenScaleY);
    doScrollBarEx( &sb[2], (FRAME_HT - 34), ht, (DEFAULT_SCRN_WD - FRAME_SPACE), 99, 20, &box, 0, screenScaleX, screenScaleY );

    if(character_CanBuyPower(e->pchar))
    {
        int iOffset = 0;
        bool bPool = character_CanBuyPoolPowerSet(e->pchar);
        bool bEpic = character_CanBuyEpicPowerSet(e->pchar) &&
            eaSize(&e->pchar->pclass->pcat[kCategory_Epic]->ppPowerSets);

        if(bEpic)
        {
            int i;
            int n = eaSize(&e->pchar->pclass->pcat[kCategory_Epic]->ppPowerSets);

            bEpic = false;
            for(i=0; i<n; i++)
            {
                if(character_IsAllowedToHavePowerSetHypothetically(e->pchar, e->pchar->pclass->pcat[kCategory_Epic]->ppPowerSets[i]))
                {
                    bEpic = true;
                    break;
                }
            }
        }

        if(bEpic && bPool)
        {
            iOffset = -30;
        }

        if(bPool)
        {
            int mouse = drawMenuBar(  (DEFAULT_SCRN_WD - FRAME_SPACE - FRAME_WIDTH/2)*screenScaleX, (85+iOffset)*screenScaleY, 50, screenScale, FRAME_WIDTH*screenScaleX/screenScale, "BuyNewPowerPoolSet", TRUE, TRUE, FALSE, FALSE, FALSE, CLR_WHITE );
            iOffset+=30;
            if ( mouse == D_MOUSEOVER )
            {
                powerSetHelp( 0, "BuySetExplanation" );
            }
            if ( mouse == D_MOUSEHIT )
            {
                gCurrentPowerSet[kCategory_Epic] = -1;
                gCurrentPower[kCategory_Epic]    = -1;
                powerSetCategory(kCategory_Pool);
                start_menu( MENU_LEVEL_POOL_EPIC );
            }
        }
        if(bEpic)
        {
            int mouse = drawMenuBar(  (DEFAULT_SCRN_WD - FRAME_SPACE - FRAME_WIDTH/2)*screenScaleX, (85+iOffset)*screenScaleY, 50, screenScale, FRAME_WIDTH*screenScaleX/screenScale, "BuyNewEpicSet", TRUE, TRUE, FALSE, FALSE, FALSE, CLR_WHITE );

            if ( mouse == D_MOUSEOVER )
            {
                powerSetHelp( 0, "BuyEpicSetExplanation" );
            }
            if ( mouse == D_MOUSEHIT )
            {
                gCurrentPowerSet[kCategory_Pool] = -1;
                gCurrentPower[kCategory_Pool]    = -1;
                powerSetCategory(kCategory_Epic);
                start_menu( MENU_LEVEL_POOL_EPIC );
            }
        }
    }
    // help background

    // the frames
    drawMenuFrame( R12, FRAME_SPACE*screenScaleX, 70*screenScaleY, 10, FRAME_WIDTH*screenScaleX, FRAME_HT*screenScaleY, CLR_WHITE, 0x00000088, 0 ); // left
    drawMenuFrame( R12, (DEFAULT_SCRN_WD/2 - FRAME_WIDTH/2)*screenScaleX, 70*screenScaleY, 10, FRAME_WIDTH*screenScaleX, FRAME_HT*screenScaleY, CLR_WHITE, 0x00000088, 0 ); // middle
    drawMenuFrame( R12, (DEFAULT_SCRN_WD - FRAME_WIDTH - FRAME_SPACE)*screenScaleX, 70*screenScaleY, 10, FRAME_WIDTH*screenScaleX, FRAME_HT*screenScaleY, CLR_WHITE, 0x00000088, 0 ); // right

    displayPowerHelpBackground(screenScaleX, screenScaleY);
    displayPowerLevelHelp(screenScaleX, screenScaleY);

    if ( drawNextButton( 40*screenScaleX,  740*screenScaleY, 10, screenScale, 0, 0, 0, "CancelString" ))
    {
        gCurNewPower.pSet = NULL;
        gCurNewPower.basePow = NULL;
        g_pBasePowMouseover = 0;
        s_ShowCombatNumbersHere = 0;
        
        // Continue the dialog with the NPC
        START_INPUT_PACKET(pak, CLIENTINP_CONTACT_DIALOG_RESPONSE);
        pktSendBitsPack(pak, 1, CONTACTLINK_BYE);
        END_INPUT_PACKET

        // Get rid of dialog if it is up.
        dialogRemove("FirstPowerPoolHelp", NULL, NULL);

        start_menu( MENU_GAME );
    }

    if ( drawNextButton( 980*screenScaleX, 740*screenScaleY, 10, screenScale, 1, gCurNewPower.basePow!=NULL?FALSE:TRUE, 0, "FinishString" ))
    {
        if (gCurNewPower.basePow != NULL)
        {
            Power *ppow;
            int tray = 0, slot = 0, found = 0;

            if (!gCurNewPower.baseSet) // picked a power from primary or secondary
            {
                // update player's powers now
                ppow = character_BuyPower(e->pchar, gCurNewPower.pSet, gCurNewPower.basePow, 0);
            }
            else // picked a power pool power
            {
                PowerSet *pset;

                // send now power to the server or something
                pset = character_BuyPowerSet(e->pchar, gCurNewPower.baseSet);
                ppow = character_BuyPower(e->pchar, pset, gCurNewPower.basePow, 0);
            }

            level_buyNewPower(ppow, kPowerSystem_Powers);

            // add new power to tray
            if( ppow->ppowBase->eType != kPowerType_Auto )
            {
                TrayObj to;
                buildPowerTrayObj( &to, ppow->psetParent->idx, ppow->idx, 0 );
                tray_AddObj( &to );
            }

            // TODO: This doesn't seem like the right place for this.
            //    Shouldn't there be some initialization done each time we
            //    enter this menu?
            gCurNewPower.basePow = NULL;
            gCurNewPower.baseSet = NULL;
            gCurrentPower[kCategory_Pool]    = -1;
            gCurrentPowerSet[kCategory_Pool] = -1;
            gCurrentPower[kCategory_Epic]    = -1;
            gCurrentPowerSet[kCategory_Epic] = -1;
            g_pBasePowMouseover = 0;
            s_ShowCombatNumbersHere = 0;

            // Continue the dialog with the NPC
            START_INPUT_PACKET(pak, CLIENTINP_CONTACT_DIALOG_RESPONSE);
            pktSendBitsPack(pak, 1, CONTACTLINK_ENDTRAIN);
            END_INPUT_PACKET

            // Do this before adding Rest to the list because it properly
            // grants Rest. Do everything else before it.
            if (totalPowerCount >= 2)
            {
                level_increaseLevel(); // Make the level increase permanent.
            }

            // HACK: Add rest power to tray
            if( e->pchar->iLevel == 2 )
            {
                TrayObj to;
                Power *ppow = character_OwnsPower(e->pchar, powerdict_GetBasePowerByName(&g_PowerDictionary, "inherent", "inherent", "Rest"));

                if(ppow)
                {
                    int ipow, iset;
                    power_GetIndices(ppow, &iset, &ipow);
                    buildPowerTrayObj(&to, iset, ipow, 0);
                    tray_AddObjTryAt(&to, 9);
                }
            }

            // Get rid of dialog if it is up.
            dialogRemove("FirstPowerPoolHelp", NULL, NULL);

            //go back to game if we're done levelling
            // DGNOTE 10/1/2008 - not yet on this.  See the comment with the same DGNOTE tag in contactInteraction.c for reasons why.
            //if (!character_CanLevel(e->pchar))
            {
                start_menu( MENU_GAME );
            }
        }
    }

    // HACK: stop pretending
    if(bFakedLevel)
    {
        e->pchar->iBuildLevels[e->pchar->iCurBuild] = --e->pchar->iLevel;
    }
    set_scrn_scaling_disabled(screenScalingDisabled);
}

void levelPowerMenu_ForceInit()
{
    init = FALSE;
}
