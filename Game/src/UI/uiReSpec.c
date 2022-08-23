/***************************************************************************
 *     Copyright (c) 2004-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/

#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/wininclude.h>

#include "UI/uiGame.h"
#include "UI/uiNet.h"
#include "UI/uiUtil.h"
#include "UI/uiInfo.h"
#include "UI/uiUtilGame.h"
#include "UI/uiUtilMenu.h"
#include "UI/uiScrollBar.h"
#include "UI/uiInput.h"
#include "UI/uiCursor.h"
#include "uiPowers.h"
#include "UI/uiTray.h"
#include "UI/uiDialog.h"
#include "UI/uiLevelPower.h"
#include "UI/uiLevelSpec.h"
#include "uiCombineSpec.h"
#include "UI/uiEnhancement.h"
#include "UI/uiPowerInventory.h"
#include "UI/uiEditText.h"
#include "UI/uiFx.h"
#include "UI/uiPowerInfo.h"
#include "UI/uiWindows.h"
#include "UI/uiTabControl.h"

#include "gameData/store.h"
#include <utilitieslib/utils/error.h>

#include "UI/sprite/sprite_base.h"
#include "UI/sprite/sprite_text.h"
#include "UI/sprite/sprite_font.h"

#include "graphics/font.h"        // for xyprintf
#include "graphics/ttFontUtil.h"
#include "language/langClientUtil.h"
#include "graphics/textureatlas.h"

#include "cmdparse/cmdgame.h"
#include "win/win_init.h"
#include "sound/sound.h"
#include "entity/entity.h"
#include "player/player.h"
#include "entity/origins.h"
#include "entity/classes.h"
#include "entity/powers.h"
#include <utilitieslib/components/earray.h>
#include "entity/entVarUpdate.h"
#include "entity/character_base.h"
#include "entity/character_level.h"
#include "entity/character_net.h"
#include "entity/PowerInfo.h"
#include "clientcomm/clientcomm.h"
#include "entity/entPlayer.h"
#include "UI/uiToolTip.h"
#include <utilitieslib/language/MessageStoreUtil.h>
#include <utilitieslib/utils/file.h>
#include "win/input.h"
#include "UI/uiClipper.h"

#ifndef TEST_CLIENT
//#include "formatter/smf_parse.h"
//#include "formatter/smf_format.h"
#include "formatter/smf_main.h"
#endif

extern ToolTip enhanceTip;

#define SL_FRAME_TOP 92
#define FRAME_TOP    28

// size & location of pile
#define RESPEC_PILE_X            (FRAME_SPACE + 20)
#define RESPEC_PILE_Y            (583)
#define RESPEC_PILE_WIDTH        (600)
#define RESPEC_PILE_HEIGHT        (118)
#define RESPEC_SLOTS_PER_ROW    (RESPEC_PILE_WIDTH / (SPEC_WD + 2))

// centered on pile
#define RESPEC_INV_WIDTH        (382)
#define RESPEC_INV_HEIGHT        (SPEC_WD + 10)
#define RESPEC_INV_Y            (486)

//------------------------------------------------------------------------------------------
// externs necessary to use regular levelup UI code for Respec stuff
//------------------------------------------------------------------------------------------
bool gWaitingVerifyRespec = false;    // set to true while game waits for respec verification
static bool sRespecLevelUp = false;    // if true allow to level up as part of the respec

extern int gNumEnhancesLeft;    // global from uiLevelSpec used to set number of enhancements slots that can be assigned
extern EnhancementAnim ** enhancementAnimQueue;

//------------------------------------------------------------------------------------------
// DEFINE/STRUCTS/GLOBAL ///////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------------------
void respec_powerLevel(void);
void respec_assignment(void);
void respec_powerLevel(void);
static void respec_placement(void);
static void respec_clearSpecs(void);
static void respec_destroyPile(void);

typedef struct RespecData
{
    void (* menuFunc )(void);    // the current menu

    int iOrigin;        // Original origin
    int iClass;            // selected class
    int    maxLevel;        // pre-respec character level (NOT the "Combat" level)

    bool init;                    // whether we've initialized the power selection window (#1) and the basic respec info
    bool initPowerSet;                // whether we've initialized the power set (pool) selection window
    bool assignmentInit;        // whether we've initialized the slot assignment window (#2)
    bool placementInit;            // whether we've initialized the placement window (#3)

    Entity e;
    Character *pchar;    // Temporary character

    int    oldPatron;                            // zero or greater if player has a patron
    int    newPatron;                            // new patron player has chosen

    bool pickingPrimary;            // whether they've picked their primary power at level 1
    int category;                    // category of power we are looking at
    int powerSet[kCategory_Count];    // primary/secondary powers sets
    int power[kCategory_Count];        // primary/secondary powers

    NewPower **powers;    // new powers ; )

    int free_boost_slots;    // number of unassigned boost slots
    Power **slot_history;    // records the order in which slots were assigned
    int lastSlotLevel;        // the last level at which slots were assigned (used for special case when returning BACK to slot assignment screen)

    Boost **pile;        // boost in the big heap - all boosts get thrown here for charactter to place, unplaced boosts are converted to influence
} RespecData;

RespecData respec = {0};
int selectedSpec[3];

static int s_ShowCombatNumbersHere;

#ifndef TEST_CLIENT
#define PATRON_COUNT    4

typedef struct respecPatronData {
    char            *name;
    char            *nameDisplay;
    char            *badgeName;
    char            *description;
    SMFBlock        *descriptionFormat;
    int                badgeID;
} respecPatronData;

respecPatronData respec_Patrons[PATRON_COUNT] =
{
    {"GhostWidowNameString", NULL, "SpidersKissPatron", "GhostWidowPatronChoice", {0}, 0},
    {"SciroccoNameString", NULL, "MiragePatron", "SciroccoPatronChoice", {0}, 0},
    {"CaptainMakoNameString", NULL, "BloodInTheWaterPatron", "CaptainMakoPatronChoice", {0}, 0},
    {"BlackScorponNameString", NULL, "TheStingerPatron", "BlackScorponPatronChoice", {0}, 0},
};

static TextAttribs s_taDefaults =
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

static int s_currentPatronSelection = -1;

#endif

int maxRespecLevel()
{
    return respec.maxLevel;
}

#ifndef TEST_CLIENT
static int respec_HasPatron(void)
{
    Entity *e = playerPtr();
    int i;
    
    if (e) 
    {
        for (i = 0; i < PATRON_COUNT; i++)
        {
            if (respec_Patrons[i].badgeID == 0)
            {
                badge_GetIdxFromName(respec_Patrons[i].badgeName, &respec_Patrons[i].badgeID);
                respec_Patrons[i].nameDisplay = strdup(textStd(respec_Patrons[i].name));
            }
        }

        for (i = 0; i < PATRON_COUNT; i++)
        {
            if (respec_Patrons[i].badgeID && BADGE_IS_OWNED(e->pl->aiBadges[respec_Patrons[i].badgeID]))
                return i;
        }
    }
    return -1;
}
#endif

//
// remove all boosts from inventory & (temporary) player powers
static void respec_clearPowerBoosts(void)
{
    int i,j,k;

    // clear all enhancements from temp pchar powers
    for( i = 0; i < eaSize(&respec.pchar->ppPowerSets); i++)
    {
        for( j = 0; j < eaSize(&respec.pchar->ppPowerSets[i]->ppPowers); j++ )
        {
            Power *ppow = respec.pchar->ppPowerSets[i]->ppPowers[j];
            if( ppow )
            {
                for( k = 0; k < eaSize(&ppow->ppBoosts); k++)
                {
                    if( ppow->ppBoosts[k] && ppow->ppBoosts[k]->ppowBase )
                    {
                        boost_Destroy(ppow->ppBoosts[k]);
                        ppow->ppBoosts[k] = NULL;
                    }
                }
            }
        }
    }

    // clear all enhancements placed in temp char inventory
    for( i = 0; i < CHAR_BOOST_MAX; i++ )
    {
        if(respec.pchar->aBoosts[i] && respec.pchar->aBoosts[i]->ppowBase )
        {
            boost_Destroy(respec.pchar->aBoosts[i]);
            respec.pchar->aBoosts[i] = 0;
        }
    }
}

// clears the respec data struct and returns user to game
// This assumes that the respec operation was cancelled ?
void respec_ClearData(int updateserver)
{
    if(respec.pchar)
        respec_clearPowerBoosts();

    respec_destroyPile();
//    eaDestroy( &respec.pile );

    eaDestroy( &respec.slot_history);

    eaDestroyEx(&respec.powers, 0);

    if(respec.pchar)
        character_Destroy(respec.pchar,NULL);

    memset( &respec, 0, sizeof(RespecData) );

    info_clear();

    dialogClearQueue(0);

    if (updateserver)
    {    
        // notify server
        EMPTY_INPUT_PACKET(CLIENTINP_RESPEC_CANCEL);
    }

    gWaitingVerifyRespec = false;
}





// creates an earray of all the characters enhancements
static void respec_createPile(void)
{
    int i, j, k;
    Entity *e = playerPtr();

    if( !respec.pile )
        eaCreate( &respec.pile );

    // get enhancements placed on powers
    for( i = 0; i < eaSize(&e->pchar->ppPowerSets); i++)
    {
        for( j = 0; j < eaSize(&e->pchar->ppPowerSets[i]->ppPowers); j++ )
        {
            Power *ppow = e->pchar->ppPowerSets[i]->ppPowers[j];
            if( ppow )
            {
                for( k = 0; k < eaSize(&ppow->ppBoosts); k++)
                {
                    if( ppow->ppBoosts[k] && ppow->ppBoosts[k]->ppowBase )
                    {
                        Boost *pBoost = boost_Clone(ppow->ppBoosts[k]);
                        pBoost->ppowParent = NULL;
                        pBoost->ppowParentRespec = ppow->ppBoosts[k]->ppowParent;
                        eaPush( &respec.pile, pBoost);
                    }
                }
            }
        }
    }

    // get enhancements place in inventory
    for( i = 0; i < CHAR_BOOST_MAX; i++ )
    {
        if( e->pchar->aBoosts[i] && e->pchar->aBoosts[i]->ppowBase )
        {
            Boost *pBoost = boost_Clone(e->pchar->aBoosts[i]);
            pBoost->ppowParent = NULL;
            pBoost->ppowParentRespec = e->pchar->aBoosts[i]->ppowParent;
            eaPush( &respec.pile,  pBoost );
        }
    }

    //-------------------------------------------------------------------

    respec_clearPowerBoosts();
}


// destroy the pile array
//
static void respec_destroyPile(void)
{
    while (eaSize(&respec.pile) > 0)
    {
        Boost *pBoost = eaPop(&respec.pile);
        boost_Destroy(pBoost);
    }

    eaDestroy( &respec.pile );
}

//------------------------------------------------------------------------------------------
// NETWORKING //////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------------------

// send the bought powers -- send everything, because we no longer use the creation function
void respec_sendPowers( Packet *pak )
{
    int i;
    int powerCount = character_CountPowersBought(respec.pchar);

    // powers we've bought plus default first 2ndary power
    assert((eaSize(&respec.powers)) == powerCount);
    assert(powerCount > 2);    // this should only always be true!

    pktSendBitsPack( pak, 32, powerCount );

    // send all the powers
    for( i = 0; i < powerCount; i++ )
    {
        basepower_Send( pak, respec.powers[i]->basePow );
        verbose_printf("Sent power %s to server\n", respec.powers[i]->basePow->pchName);
    }

    verbose_printf("Sent %d powers to server\n", powerCount);
}

// send the boost slots bought
void respec_sendBoostSlots( Packet *pak )
{
    int i, j, m;

    pktSendBitsPack( pak, 32, character_CountBoostsBought(respec.pchar) );

    assert(eaSize(&respec.slot_history) == character_CountBoostsBought(respec.pchar));

    // loop over each power and send indices
    for(m = 0; m < eaSize(&respec.slot_history); m++)
    {
        bool bFound = false;

        for( i = 0; i < eaSize(&respec.pchar->ppPowerSets); i++ )
        {
            for( j = 0; j < eaSize(&respec.pchar->ppPowerSets[i]->ppPowers); j++)
            {
                if(respec.slot_history[m] == respec.pchar->ppPowerSets[i]->ppPowers[j])
                {
                    verbose_printf("Sent boost slot for power %s\n", respec.pchar->ppPowerSets[i]->ppPowers[j]->ppowBase->pchName);
                    basepower_Send(pak,respec.pchar->ppPowerSets[i]->ppPowers[j]->ppowBase);
                    bFound = true;
                }
            }
        }

        assert(bFound);        // otherwise slot_history does not match actual slot assignments!
    }
}

//
// send boosts placed into powers
void respec_sendPowerBoosts( Packet *pak )
{
    int i, j, k, count = 0;

    // sigh, get a count of them first
    for( i = 0; i < eaSize(&respec.pchar->ppPowerSets); i++ )
    {
        for( j = 0; j < eaSize(&respec.pchar->ppPowerSets[i]->ppPowers); j++)
        {
            for( k = 0; k < eaSize(&respec.pchar->ppPowerSets[i]->ppPowers[j]->ppBoosts); k++ )
            {
                if( respec.pchar->ppPowerSets[i]->ppPowers[j]->ppBoosts[k] && respec.pchar->ppPowerSets[i]->ppPowers[j]->ppBoosts[k]->ppowBase )
                    count++;
            }
        }
    }

    pktSendBitsPack( pak, 32, count );

    // now send where they all came from and where they are going
    for( i = 0; i < eaSize(&respec.pchar->ppPowerSets); i++ )
    {
        for( j = 0; j < eaSize(&respec.pchar->ppPowerSets[i]->ppPowers); j++)
        {
            Power *ppow = respec.pchar->ppPowerSets[i]->ppPowers[j];

            for( k = 0; k < eaSize(&ppow->ppBoosts); k++ )
            {
                if( ppow->ppBoosts[k] && ppow->ppBoosts[k]->ppowBase )
                {
                    basepower_Send( pak, ppow->ppowBase ); // the power its going to

                    // destination slot #
                    pktSendBitsPack( pak, 32, k ) ;

                    pktSendBitsPack( pak, 32, ppow->ppBoosts[k]->idx ) ;

                    // came from power
                    if( ppow->ppBoosts[k]->ppowParentRespec )
                    {
                        pktSendBitsPack( pak, 1, 0 );
                        pktSendBitsPack( pak, 32, ppow->ppBoosts[k]->ppowParentRespec->idx );
                        pktSendBitsPack( pak, 32, ppow->ppBoosts[k]->ppowParentRespec->psetParent->idx );
                    }
                    else
                        pktSendBitsPack( pak, 1, 1 ); // came from inventory
                }
            }
        }
    }

}

//
// send boosts being placed in the inventory
void repsec_sendCharBoosts( Packet *pak )
{
    int i;

    // loop though inventory send occupied slots
    for( i = 0; i < CHAR_BOOST_MAX; i++ )
    {
        if( respec.pchar->aBoosts[i] && respec.pchar->aBoosts[i]->ppowBase )
        {
            pktSendBitsPack( pak, 1, 1 );

            // place it came from
            pktSendBitsPack( pak, 32, respec.pchar->aBoosts[i]->idx );

            // came from power
            if( respec.pchar->aBoosts[i] && respec.pchar->aBoosts[i]->ppowParentRespec )
            {
                pktSendBitsPack( pak, 1, 0 );
                pktSendBitsPack( pak, 32, respec.pchar->aBoosts[i]->ppowParentRespec->idx );
                pktSendBitsPack( pak, 32, respec.pchar->aBoosts[i]->ppowParentRespec->psetParent->idx );
            }
            else
                pktSendBitsPack( pak, 1, 1 ); // came from inventory
        }
        else
            pktSendBitsPack( pak, 1, 0 );
    }
}


// send leftover boosts to be converted to influence
//
void respec_sendPile( Packet * pak )
{
    int i;

    // count
    pktSendBitsPack( pak, 32, eaSize( &respec.pile ) );

    // send each one in inventory
    for( i = 0; i < eaSize( &respec.pile ); i++ )
    {
        // place it came from
        pktSendBitsPack( pak, 32, respec.pile[i]->idx );

        // came from power
        if( respec.pile[i]->ppowParentRespec )
        {
            pktSendBitsPack( pak, 1, 0 );
            pktSendBitsPack( pak, 32, respec.pile[i]->ppowParentRespec->idx );
            pktSendBitsPack( pak, 32, respec.pile[i]->ppowParentRespec->psetParent->idx );
        }
        else
            pktSendBitsPack( pak, 1, 1 ); // came from inventory
    }
}

//------------------------------------------------------------------------------------------
// CLASS SELECTION /////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------------------
// This screen mimics uiOriginClass, except that a class cannot be picked


// find the current origin/class
void respec_setOriginClass(void)
{
    Entity *e = playerPtr();
    int i;

    for( i = 0; i < eaSize(&g_CharacterOrigins.ppOrigins); i++ )
    {
        if( stricmp( g_CharacterOrigins.ppOrigins[i]->pchName, e->pchar->porigin->pchName ) == 0 )
            respec.iOrigin = i;
    }

    for( i = 0; i < eaSize(&g_CharacterClasses.ppClasses); i++ )
    {
        if( stricmp( g_CharacterClasses.ppClasses[i]->pchName , e->pchar->pclass->pchName ) == 0 )
            respec.iClass = i;
    }
}

//------------------------------------------------------------------------------------------
// EARLY OUT TEST /////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------------------
// Check if the character is valid for a respec and abort the respec if they
// are not.
bool respec_earlyOut(void)
{
    bool retval = false;

    // make sure player is minimum level (should only be needed in dev mode)
    if(playerPtr()->pchar->iLevel < 2)
    {
        // Make sure any powers get turned back on and any 'character is
        // unavailable' flags are cleared.
        EMPTY_INPUT_PACKET(CLIENTINP_RESPEC_CANCEL);

#ifndef TEST_CLIENT
        dialogStd(DIALOG_OK, textStd("TooLowForRespec"), NULL, NULL, NULL, NULL, 0);
#endif
        retval = true;
    }

    return retval;
}

void respec_allowLevelUp(bool levelUp)
{
    sRespecLevelUp = levelUp;
}

#ifndef TEST_CLIENT




//------------------------------------------------------------------------------------------
// POWER SELECTION /////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------------------

// used for power pool selection
void respec_power(void)
{
    static ScrollBar sb = {0};
    CBox box;
    int ht;
    float screenScaleX, screenScaleY;
    int screenScalingDisabled = is_scrn_scaling_disabled();
    float screenScale;

    calc_scrn_scaling(&screenScaleX, &screenScaleY);
    screenScale = MIN(screenScaleX, screenScaleY);

    set_scrn_scaling_disabled(1);

    if(respec.category == kCategory_Pool)
        drawMenuTitle( 10*screenScaleX, 30*screenScaleY, 20, "ChoosePoolTitle", screenScale, !respec.initPowerSet );
    else
        drawMenuTitle( 10*screenScaleX, 30*screenScaleY, 20, "ChooseEpicTitle", screenScale, !respec.initPowerSet );

    if( !respec.initPowerSet )
    {
        respec.initPowerSet = TRUE;
        sb.offset = 0;
        s_ShowCombatNumbersHere = 0;
    }

    if( D_MOUSEHIT == drawStdButton( 512*screenScaleX, 330*screenScaleY, 10, 120*screenScaleX, 30*screenScaleY, SELECT_FROM_UISKIN( CLR_BTN_HERO, CLR_BTN_VILLAIN, CLR_BTN_ROGUE ), 
                                        s_ShowCombatNumbersHere?"HidePowerNumbers":"ShowCombatNumbers", screenScale, (respec.powerSet[respec.category]<0) ))
        s_ShowCombatNumbersHere = !s_ShowCombatNumbersHere;

    // do both power selectors
    if( s_ShowCombatNumbersHere )
    {
        menuDisplayPowerInfo( 85*screenScaleX, 80*screenScaleY, 30, 355*screenScaleX, 500*screenScaleY, g_pBasePowMouseover, 0, POWERINFO_BASE  );
    }
    else
    {
        ht = powerSetSelector(153 - sb.offset, respec.category, respec.pchar, respec.powerSet, respec.power, screenScaleX, screenScaleY, &sb.offset, MENU_RESPEC);
        BuildCBox(&box, 85*screenScaleX,80*screenScaleY,355*screenScaleX,500*screenScaleY);
        doScrollBarEx( &sb, (475-PIX4*2-R10*2), ht, 88, 110, 30, &box, 0, screenScaleX, screenScaleY );
    }

    powerSelector( respec.category, 158, respec.pchar, respec.powerSet, respec.power, screenScaleX, screenScaleY, 0);

    // the crazy powers frame
    drawMenuPowerFrame( 85, 80, 355, 500, 150, 80, 358, 500, 21, 20, screenScaleX, screenScaleY, CLR_WHITE, 0x00000044 );

    // explantation at bottom
    displayPowerHelpBackground(screenScaleX, screenScaleY);

    if ( drawNextButton( 40*screenScaleX, 740*screenScaleY, 10, screenScale, 0, 0, 0, "BackString" ))
    {
        respec.initPowerSet = FALSE;
        sndPlay( "back", SOUND_GAME );
        s_ShowCombatNumbersHere = 0;
        respec.menuFunc = respec_powerLevel;
    }

    if ( drawNextButton( 980*screenScaleX, 740*screenScaleY, 10, screenScale, 1, (respec.power[respec.category] < 0) , 0, "NextString" ) || (respec.power[respec.category] >= 0 && inpEdgeNoTextEntry(INP_RETURN)) )
    {
        sndPlay( "next", SOUND_GAME );
        respec.initPowerSet = FALSE;
        s_ShowCombatNumbersHere = 0;
        respec.menuFunc = respec_powerLevel;
    }
    set_scrn_scaling_disabled(screenScalingDisabled);
}

// display a count of how many powers are remaining to be bought
static void respec_powerCounter(bool powerSelected, float screenScaleX, float screenScaleY)
{
    Entity *e = playerPtr();
    float screenScale = MIN(screenScaleX, screenScaleY);
    // subtract 1 because we auto-assign the 1st power in the secondary powerset
    int remaining = character_CountPowersForLevel( respec.pchar, maxRespecLevel() );
    int color;

//    cprntEx( DEFAULT_SCRN_WD/2, SL_FRAME_TOP+FRAME_HEIGHT-5+15 + 15 , 155, 1.f, 1.f, (CENTER_X), "TotalSlotsRemaining", respec.free_boost_slots + gNumEnhancesLeft );

    font( &game_12 );
    if(powerSelected)
    {
        color = CLR_GREEN;

        font_color( CLR_WHITE, CLR_WHITE );
        cprnt( DEFAULT_SCRN_WD*screenScaleX/2, 725*screenScaleY , 155, screenScale, screenScale, "ClickNextToContinue");

        font_color( CLR_GREEN, CLR_GREEN);
        cprnt( DEFAULT_SCRN_WD*screenScaleX/2, 740*screenScaleY , 155, screenScale, screenScale, "PowerRemaining", (remaining - 1));

    }
    else
    {
        color = CLR_RED;
        font_color( CLR_RED, CLR_RED );
        cprnt( DEFAULT_SCRN_WD*screenScaleX/2, 725*screenScaleY , 155, screenScale, screenScale, "PowersBought", character_CountPowersBought(respec.pchar));
        cprnt( DEFAULT_SCRN_WD*screenScaleX/2, 740*screenScaleY , 155, screenScale, screenScale, "PowerRemaining", remaining);
    }

    drawFlatFrame( PIX3, R10, (DEFAULT_SCRN_WD/2 - 100)*screenScaleX, (715-10)*screenScaleY, 40, 200*screenScaleX, 45*screenScaleY, 1.f, color, CLR_BLACK ); // middle
}

// finalize all of the powers for slot picking
static void respec_finalizePowers(void)
{
    int i, j;
    for( i = 0; i < eaSize(&respec.pchar->ppPowerSets); i++ )
    {
        for( j = 0; j < eaSize(&respec.pchar->ppPowerSets[i]->ppPowers); j++ )
        {
            Power * pow = respec.pchar->ppPowerSets[i]->ppPowers[j];
            if(pow)
                power_LevelFinalize(pow);
        }
    }
}

void respec_Init()
{
    Entity *e = playerPtr();

    if ((respec.oldPatron = respec_HasPatron()) >= 0)
    {
        // DGNOTE 10/27/2008
        // Patron respecs are no longer necessary.
        // respec.newPatron = -1;
        respec.newPatron = respec.oldPatron;
    }

    // create a character based on their origin and class type
    if ( !respec.pchar ) {
        respec.pchar = calloc(1, sizeof(Character));
        respec.e.pchar = respec.pchar;
        SET_ENTTYPE(&respec.e)=ENTTYPE_PLAYER;
        respec.e.pl = calloc(1, sizeof(EntPlayer));
        *respec.e.pl = *e->pl;
    }

    respec_setOriginClass();

    // hope that the player's real entity is not abused...
    character_Create(respec.pchar, &respec.e, g_CharacterOrigins.ppOrigins[respec.iOrigin],
        g_CharacterClasses.ppClasses[respec.iClass]);

    respec.pchar->iExperiencePoints = e->pchar->iExperiencePoints;
    respec.pchar->iWisdomPoints = e->pchar->iWisdomPoints;
    respec.pchar->iNumBoostSlots = e->pchar->iNumBoostSlots;

    // TODO: Handle Skill Respec
    respec.pchar->iWisdomLevel = e->pchar->iWisdomLevel;

    respec.maxLevel = e->pchar->iLevel;

    if (sRespecLevelUp)
    {
        respec.maxLevel++;
    }

    respec.pchar->iLevel = 0;//-2;    // this allows an extra power to be selected at level 0
    respec.pickingPrimary = false;    // they need to pick their level 1 primary power yet

    // assign player the default power categories
    {
        int i;
        int dbgBoughtSecondary = 0;
        int dbgBoughtPrimary = 0;
        int iSize = eaSize(&e->pchar->ppPowerSets);

        eaCreate( &respec.powers );

        for( i = 1; i < iSize; i++ )
        {
            if(     e->pchar->ppPowerSets[i]->psetBase->pcatParent == e->pchar->pclass->pcat[kCategory_Primary]
               && ! dbgBoughtPrimary)
            {
                dbgBoughtPrimary = 1;
                character_BuyPowerSet(respec.pchar, e->pchar->ppPowerSets[i]->psetBase);
            }

            if(  e->pchar->ppPowerSets[i]->psetBase->pcatParent == e->pchar->pclass->pcat[kCategory_Secondary]
            && ! dbgBoughtSecondary)
            {
                NewPower * np = calloc(1, sizeof(NewPower));
                PowerSet *pset;

                dbgBoughtSecondary = 1;
                pset = character_BuyPowerSet(respec.pchar, e->pchar->ppPowerSets[i]->psetBase);

                // Villain EATs can pick entirely from specialization sets so
                // they don't get the original secondary for free.
                if (!class_MatchesSpecialRestriction(e->pchar->pclass, "ArachnosSoldier") &&
                    !class_MatchesSpecialRestriction(e->pchar->pclass, "ArachnosWidow"))
                {
                    // auto-buy the first secondary power, since the user really doesn't have a choice
                    // keep the same uniqueID, if possible
                    const BasePower *firstSecondaryPower = e->pchar->ppPowerSets[i]->psetBase->ppPowers[0];
                    Power *oldPower = character_OwnsPower(e->pchar, firstSecondaryPower);
                    character_BuyPowerEx(respec.pchar, pset, firstSecondaryPower, NULL, 0, oldPower ? oldPower->iUniqueID : 0, e->pchar);
                    np->basePow = e->pchar->ppPowerSets[i]->psetBase->ppPowers[0];
                    np->baseSet = e->pchar->ppPowerSets[i]->psetBase;
                    eaPush(&respec.powers, np);
                    respec.pickingPrimary = true;
                }
            }
        }

        assert(dbgBoughtPrimary == 1 && dbgBoughtSecondary == 1);    // make sure that primary & secondary powers were bought 1 time each

        for(i=0; i < kCategory_Count; i++)
        {
            respec.powerSet[i] = -1;
            respec.power[i] = -1;
        }
    }

    respec.init = TRUE;
}


// (debug only) make sure that there are no NULL powers
void respec_dbgVerifyPowers()
{
    int i,j;
    for( i = 0; i < eaSize(&respec.pchar->ppPowerSets); i++ )
    {
        for( j = 0; j < eaSize(&respec.pchar->ppPowerSets[i]->ppPowers); j++)
        {
            assert(respec.pchar->ppPowerSets[i]->ppPowers[j]);
        }
    }
}

void respec_RemoveLastPower()
{
    respec_dbgVerifyPowers();

    if(eaSize(&respec.powers) > 0)
    {
        PowerSet *pInherentPowerSet = respec.pchar->ppPowerSets[1];
        int i;
        NewPower * remove = eaPop( &respec.powers);

        // may have been a power pool set
        if( remove->baseSet )
            character_RemovePowerSet( respec.pchar, remove->baseSet, NULL );
        else
        {
            if (respec.pchar->ppowStance && respec.pchar->ppowStance->ppowBase == remove->basePow)
            {
                respec.pchar->ppowStance = NULL; // should I call Character_EnterStance() here instead?
            }

            powerset_DeleteBasePower( remove->pSet, remove->basePow, NULL );

            // compact the power array -- other respec code expects this
            while(-1 != (i=eaFind(&remove->pSet->ppPowers, NULL)))
            {
                verbose_printf("Pruning null power from index %d\n", i);
                eaRemove(&remove->pSet->ppPowers, i);
            }
        }

        if( remove )
            free( remove );

        // reduce character level until they need to choose a power
        while(   0 < respec.pchar->iLevel
              && 0 < character_CountPowersForLevel(respec.pchar, (respec.pchar->iLevel - 1)))
        {
            respec.pchar->iLevel--;
        }

        // Kheldians causing trouble again, remove all illegal inherents, compact the set and re-add them..
        for(i=eaSize(&pInherentPowerSet->ppPowers)-1; i>=0; i--)
        {
            if (pInherentPowerSet->ppPowers[i]
                && pInherentPowerSet->ppPowers[i]->iLevelBought >= respec.pchar->iLevel)
            {
                if (respec.pchar->ppowStance->ppowBase == remove->basePow)
                {
                    respec.pchar->ppowStance = NULL; // should I call Character_EnterStance() here instead?
                }

                powerset_DeletePowerAt(pInherentPowerSet, i, NULL);
            }
        }
    
        // compact the power array -- other respec code expects this
        while(-1 != (i=eaFind(&pInherentPowerSet->ppPowers, NULL)))
        {
            eaRemove(&pInherentPowerSet->ppPowers, i);
        }

        // Giveth after we tooketh awayeth
        character_GrantAutoIssuePowers(respec.pchar);

    }

    respec_dbgVerifyPowers();
}

static void patron_select(void * data)
{
    // Entity *e = playerPtr();

    if(s_currentPatronSelection < 0)
        return;

    respec.newPatron = s_currentPatronSelection;

    // set appropriate badges 
    //        remove old 
    respec.e.pl->aiBadges[respec_Patrons[respec.oldPatron].badgeID] = BADGE_CHANGED_MASK;

    //        grant new 
    respec.e.pl->aiBadges[respec_Patrons[respec.newPatron].badgeID] = BADGE_FULL_OWNERSHIP | BADGE_CHANGED_MASK;
}

void respec_powerLevel(void)
{
    static ScrollBar sb[3] = {0};

    static NewPower *np = 0; // place to hold the info on the power we are currently selecting
    int ht[3];
    CBox box[3];
    int y;
    int screenScalingDisabled = is_scrn_scaling_disabled();
    float screenScaleX = 1.f, screenScaleY = 1.f, screenScale;

    calc_scrn_scaling(&screenScaleX, &screenScaleY); 
    screenScale = MIN(screenScaleX, screenScaleY);

    if( !np )
        np = calloc(1, sizeof(NewPower));

    if( !respec.init )
    {
        if (respec_earlyOut())
        {
            start_menu( MENU_GAME );
            return;
        }

        respec_Init();
        memset(np, 0, sizeof(NewPower));
    }

    set_scrn_scaling_disabled(1);
    drawMenuTitle( 10*screenScaleX, 30*screenScaleY, 20, "ChooseANewPower", screenScale, !respec.init );


    font( &title_12 );
    font_color( CLR_WHITE, CLR_GREY );
       prnt( 450*screenScaleX, 30*screenScaleY, 50, screenScaleX, screenScaleY, textStd("EquivilentLevel", respec.pchar->iLevel) );

    respec_dbgVerifyPowers();


    if( D_MOUSEHIT == drawStdButton( (FRAME_SPACE+FRAME_WIDTH/2)*screenScaleX, 70*screenScaleY, 50, 120*screenScaleX, 20*screenScaleY, SELECT_FROM_UISKIN( CLR_BTN_HERO, CLR_BTN_VILLAIN, CLR_BTN_ROGUE ), (s_ShowCombatNumbersHere==1)?"HidePowerNumbers":"ShowCombatNumbersHere", screenScale, 0 ))
        (s_ShowCombatNumbersHere==1)?(s_ShowCombatNumbersHere = 0):(s_ShowCombatNumbersHere = 1);
    if( D_MOUSEHIT == drawStdButton( DEFAULT_SCRN_WD*screenScaleX/2 , 70*screenScaleY, 50, 120*screenScaleX, 20*screenScaleY, SELECT_FROM_UISKIN( CLR_BTN_HERO, CLR_BTN_VILLAIN, CLR_BTN_ROGUE ), (s_ShowCombatNumbersHere==2)?"HidePowerNumbers":"ShowCombatNumbersHere", screenScale, 0 ))
        (s_ShowCombatNumbersHere==2)?(s_ShowCombatNumbersHere = 0):(s_ShowCombatNumbersHere = 2);

    if( s_ShowCombatNumbersHere == 1)
        menuDisplayPowerInfo( FRAME_SPACE*screenScaleX, 70*screenScaleY, 30, FRAME_WIDTH*screenScaleX, 520*screenScaleY, g_pBasePowMouseover, 0, POWERINFO_BASE );
    if( s_ShowCombatNumbersHere == 2)
        menuDisplayPowerInfo( (DEFAULT_SCRN_WD/2 - FRAME_WIDTH/2)*screenScaleX, 70*screenScaleY, 30, FRAME_WIDTH*screenScaleX, 520*screenScaleY, g_pBasePowMouseover, 0, POWERINFO_BASE );

    // do power selectors
    // primary powers
    BuildCBox(&box[0],FRAME_SPACE*screenScaleX, 73*screenScaleY, FRAME_WIDTH*screenScaleX, 513*screenScaleY); 

    if(s_ShowCombatNumbersHere!=1)
    {
        bool category_locked = false;

        if(respec.pchar->iLevel == 0 && !respec.pickingPrimary)
        {
            category_locked = true;
        }
        clipperPushCBox(&box[0]);
         y = 99 - sb[0].offset;
        ht[0] = powerLevelSelector( np, respec.pchar, respec.maxLevel, FRAME_SPACE + FRAME_OFFSET, y, kCategory_Primary, respec.powerSet, respec.power, category_locked, screenScaleX, screenScaleY );
        clipperPop();
    }
    
    // secondary powers
    BuildCBox(&box[1],(DEFAULT_SCRN_WD/2 - FRAME_WIDTH/2)*screenScaleX, 73*screenScaleY, FRAME_WIDTH*screenScaleX, 513*screenScaleY );
    if(s_ShowCombatNumbersHere!=2)
    {    
        bool category_locked = false;

        if(respec.pchar->iLevel == 0 && respec.pickingPrimary)
        {
            category_locked = true;
        }
        clipperPushCBox(&box[1]);
        y = 99 - sb[1].offset;
        ht[1] = powerLevelSelector( np, respec.pchar, respec.maxLevel, DEFAULT_SCRN_WD/2 - FRAME_WIDTH/2 + FRAME_OFFSET, y, kCategory_Secondary, respec.powerSet, respec.power, category_locked, screenScaleX, screenScaleY  );
        clipperPop();
    }

    // power pool powers (a bit funky)
    BuildCBox(&box[2], (DEFAULT_SCRN_WD - FRAME_WIDTH - FRAME_SPACE)*screenScaleX, 99*screenScaleY, FRAME_WIDTH*screenScaleX, 486*screenScaleY );
    clipperPushCBox(&box[2]);
    y = 130 - sb[2].offset;
    ht[2]  = powerLevelShowNew( np, respec.pchar, DEFAULT_SCRN_WD - FRAME_WIDTH + FRAME_OFFSET, y, kCategory_Pool, respec.powerSet, respec.power, screenScaleX, screenScaleY );
    ht[2] += powerLevelShowNew( np, respec.pchar, DEFAULT_SCRN_WD - FRAME_WIDTH + FRAME_OFFSET, y+ht[2], kCategory_Epic, respec.powerSet, respec.power, screenScaleX, screenScaleY );
    ht[2] += powerLevelSelector( np, respec.pchar, respec.maxLevel, DEFAULT_SCRN_WD - FRAME_WIDTH + FRAME_OFFSET, y+ht[2], kCategory_Pool, respec.powerSet, respec.power, false, screenScaleX, screenScaleY );
    ht[2] += powerLevelSelector( np, respec.pchar, respec.maxLevel, DEFAULT_SCRN_WD - FRAME_WIDTH + FRAME_OFFSET, y+ht[2], kCategory_Epic, respec.powerSet, respec.power, false, screenScaleX, screenScaleY );
    clipperPop();

    doScrollBarEx( &sb[0], 461, ht[0], (FRAME_SPACE + FRAME_WIDTH), 99, 20, &box[0], 0, screenScaleX, screenScaleY );
    doScrollBarEx( &sb[1], 461, ht[1], (DEFAULT_SCRN_WD/2 - FRAME_WIDTH/2 + FRAME_WIDTH), 99, 20, &box[1], 0, screenScaleX, screenScaleY );
     doScrollBarEx( &sb[2], 461, ht[2], (DEFAULT_SCRN_WD - FRAME_SPACE), 99, 20, &box[2], 0, screenScaleX, screenScaleY );

    // if we can display the "buy pool" button
     if(character_CanBuyPower(respec.pchar))
    {
        bool bEpic = character_CanBuyEpicPowerSet(respec.pchar) &&
            eaSize(&respec.pchar->pclass->pcat[kCategory_Epic]->ppPowerSets);
        bool bPool = character_CanBuyPoolPowerSet(respec.pchar);
        int iOffset = 0;

        if(bEpic)
        {
            int i;
            int n = eaSize(&respec.pchar->pclass->pcat[kCategory_Epic]->ppPowerSets);

            bEpic = false;
            for(i=0; i<n; i++)
            {
                if(character_IsAllowedToHavePowerSetHypothetically(respec.pchar, respec.pchar->pclass->pcat[kCategory_Epic]->ppPowerSets[i]))
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

        if(bEpic && bPool)
        {
            iOffset = -30;
        }

        if(bPool)
        {
            int mouse = drawMenuBar(  (DEFAULT_SCRN_WD - FRAME_SPACE - FRAME_WIDTH/2)*screenScaleX, (85*screenScaleY)+(iOffset*screenScale), 50, screenScale, FRAME_WIDTH*screenScaleX/screenScale, "BuyNewPowerPoolSet", TRUE, TRUE, FALSE, FALSE, FALSE, CLR_WHITE );
            iOffset+=30;

            if ( mouse == D_MOUSEOVER ) // give some info on mouseover
                powerSetHelp( 0, "BuySetExplanation" );
            if ( mouse == D_MOUSEHIT ) // on mousehit go to pool selection
            {
                respec.powerSet[kCategory_Epic] = -1;
                respec.power[kCategory_Epic] = -1;
                respec.category = kCategory_Pool;
                respec.menuFunc = respec_power;
                s_ShowCombatNumbersHere = 0;
            }
        }
        if(bEpic)
        {
            int mouse = drawMenuBar(  (DEFAULT_SCRN_WD - FRAME_SPACE - FRAME_WIDTH/2)*screenScaleX, (85+iOffset)*screenScaleY, 50, screenScale, FRAME_WIDTH*screenScaleX/screenScale, "BuyNewEpicSet", TRUE, TRUE, FALSE, FALSE, FALSE, CLR_WHITE );

            if ( mouse == D_MOUSEOVER ) // give some info on mouseover
                powerSetHelp( 0, "BuyEpicSetExplanation" );
            if ( mouse == D_MOUSEHIT ) // on mousehit go to pool selection
            {
                respec.powerSet[kCategory_Pool] = -1;
                respec.power[kCategory_Pool] = -1;
                respec.category = kCategory_Epic;
                respec.menuFunc = respec_power;
                s_ShowCombatNumbersHere = 0;
            }
        }
    }

    // the frames
    drawMenuFrame( R12, FRAME_SPACE*screenScaleX, 70*screenScaleY, 10, FRAME_WIDTH*screenScaleX, 520*screenScaleY, CLR_WHITE, 0x00000088, 0 ); // left
    drawMenuFrame( R12, (DEFAULT_SCRN_WD/2 - FRAME_WIDTH/2)*screenScaleX, 70*screenScaleY, 10, FRAME_WIDTH*screenScaleX, 520*screenScaleY, CLR_WHITE, 0x00000088, 0 ); // middle
    drawMenuFrame( R12, (DEFAULT_SCRN_WD - FRAME_WIDTH - FRAME_SPACE)*screenScaleX, 70*screenScaleY, 10, FRAME_WIDTH*screenScaleX, 520*screenScaleY, CLR_WHITE, 0x00000088, 0 ); // right

    // help background
    displayPowerHelpBackground(screenScaleX, screenScaleY);
    displayPowerLevelHelp(screenScaleX, screenScaleY);

    // power counter
    respec_powerCounter(np->basePow!=NULL, screenScaleX, screenScaleY);

    if ( drawNextButton( 40*screenScaleX,  740*screenScaleY, 10, screenScale, 0, 0, 0, "BackString" ))
    {
        sndPlay( "back", SOUND_GAME );

        powerInfoSetLevel(respec.pchar->iLevel);

        // if we have only one power bought (the default 2ndary power), go back to creation
        if(!eaSize(&respec.powers))
        {
            respec_ClearData(1);
            start_menu( MENU_GAME );
        }
        else // remove the last bought power from the list
        {
            respec_RemoveLastPower();

            // clear selected power?
            memset(np, 0, sizeof(NewPower));
            respec.powerSet[kCategory_Pool] = -1;
            respec.power[kCategory_Pool] = -1;
            respec.powerSet[kCategory_Epic] = -1;
            respec.power[kCategory_Epic] = -1;
            if(eaSize(&respec.powers) < 1)
            {
                Entity * e = playerPtr();
                if (class_MatchesSpecialRestriction(e->pchar->pclass, "ArachnosSoldier") ||
                    class_MatchesSpecialRestriction(e->pchar->pclass, "ArachnosWidow"))
                {
                    respec.pickingPrimary = 0;
                }
                else
                {
                    respec_ClearData(1);
                    start_menu( MENU_GAME );
                }
            }
        }
    }

    if ( drawNextButton( 980*screenScaleX, 740*screenScaleY, 10, screenScale, 1, np->basePow!=NULL ? FALSE:TRUE, 0, "NextString" ) || (np->basePow!=NULL && inpEdgeNoTextEntry(INP_RETURN)) )
    {
        sndPlay( "next", SOUND_GAME );

        if(np->basePow!=NULL) // make sure something was selected
        {
            Entity *oldEntity = playerPtr();

            if( !np->baseSet ) // picked a power from primary or secondary
            {
                // Locals for debugging minidumps of a crash in this area
                char strBasePow[256], strPowSet[256];
                int iNumPowsBought = eaSize(&respec.powers);
                Power *oldPower = character_OwnsPower(oldEntity->pchar, np->basePow);
                strncpy(strBasePow, np->basePow->pchName,255);
                strncpy(strPowSet, np->pSet->psetBase->pchName,255);

                // Keep the same uniqueID, if possible
                character_BuyPowerEx(respec.pchar, np->pSet, np->basePow, NULL, 0, oldPower ? oldPower->iUniqueID : 0, oldEntity->pchar);

            }
            else // picked a power pool or epic power
            {
                PowerSet *pset;
                Power *oldPower = character_OwnsPower(oldEntity->pchar, np->basePow);                

                pset = character_BuyPowerSet( respec.pchar, np->baseSet );

                // Keep the same uniqueID, if possible
                character_BuyPowerEx(respec.pchar, pset, np->basePow, NULL, 0, oldPower ? oldPower->iUniqueID : 0, oldEntity->pchar);
            }

            // The first power picked at level 1 is from the primary sets
            // and anything thereafter implies that too.
            if( respec.pchar->iLevel == 0 )
                respec.pickingPrimary = true;

            eaPush( &respec.powers, np );

            // get any inherent and free powers (thanks again, Kheldians)
            character_GrantAutoIssuePowers(respec.pchar);

            // increase characters level until the next level that receives a power
            while(   0 >= character_CountPowersForLevel(respec.pchar, respec.pchar->iLevel)
                  && respec.pchar->iLevel <= maxRespecLevel())
            {
                respec.pchar->iLevel++;
            }
        
            // if player's level is now more than 40 and they have a patron, allow them the choice to respec that patron
            if (respec.pchar->iLevel > 40)
            {

            }


            powerInfoSetLevel(respec.pchar->iLevel);

            np = calloc( 1, sizeof(NewPower) ); // clear info
            respec.powerSet[kCategory_Pool] = -1;
            respec.power[kCategory_Pool] = -1;
            respec.powerSet[kCategory_Epic] = -1;
            respec.power[kCategory_Epic] = -1;

            // check for end condition
            if((respec.pchar->iLevel) > maxRespecLevel())
            {
                // set ilevel to be exactly right
                respec.pchar->iLevel = maxRespecLevel();

                // thanks to Kheldians we have to grant inherents we may have received from this last pick
                character_GrantAutoIssuePowers(respec.pchar);

                // finalize powers
                respec_finalizePowers();

                // size the specialization inventory
                if(!respec.slot_history)
                    eaCreate(&respec.slot_history);

                eaClear(&respec.slot_history);
                respec.menuFunc = respec_assignment;
                s_ShowCombatNumbersHere = 0;
                respec_clearSpecs();
            }
        }
    }
    set_scrn_scaling_disabled(screenScalingDisabled);
}

//------------------------------------------------------------------------------------------
// SPEC SELECTION /////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------------------
//
// remove all the boost slots bought
static void respec_clearSpecs(void)
{
    int i, j, k;
    for( i = 0; i < eaSize(&respec.pchar->ppPowerSets); i++ )
    {
        for( j = 0; j < eaSize(&respec.pchar->ppPowerSets[i]->ppPowers); j++ )
        {
            Power * pow = respec.pchar->ppPowerSets[i]->ppPowers[j];
            if(pow)
            {
                for( k = 0; k < pow->iNumBoostsBought; k++ )
                    eaPop( &pow->ppBoosts );

                pow->iNumBoostsBought = 0;
            }
        }
    }
}

//
//
#define FRAME_HEIGHT 623
#define POOL_HEIGHT 423
static void respec_specCounter(float screenScaleX, float screenScaleY)
{
    int color;
    char * str;
    float screenScale = MIN(screenScaleX, screenScaleY);
    font( &game_12 );
    if( !gNumEnhancesLeft )
    {
        font_color( CLR_GREEN, CLR_GREEN );
        color = CLR_GREEN;
    }
    else
    {
        font_color( CLR_RED, CLR_RED );
        color = CLR_RED;
    }

    drawFlatFrame( PIX3, R10, (DEFAULT_SCRN_WD/2 - 100)*screenScaleX, 705*screenScaleY, 150, 200*screenScaleX, (30+15)*screenScaleY, 1.f, color, CLR_BLACK ); // middle
    cprntEx( DEFAULT_SCRN_WD*screenScaleX/2, 740*screenScaleY , 155, screenScale, screenScale, (CENTER_X), "TotalSlotsRemaining", respec.free_boost_slots + gNumEnhancesLeft );

    if(gNumEnhancesLeft)
        str = "EnhancmentsAllocated";
    else
    {
        font_color(CLR_WHITE, CLR_WHITE);
        str = "ClickNextToContinue";
    }
    cprntEx( DEFAULT_SCRN_WD*screenScaleX/2, 725*screenScaleY, 155, screenScale, screenScale, (CENTER_X), str, gNumEnhancesLeft );
}



// use the enhancement animation state to assign actual slots to powers
void respec_assignSlots()
{
    int i,j;

    for( i = 0; i < eaSize(&enhancementAnimQueue); i++ )
    {
        EnhancementAnim *ea = enhancementAnimQueue[i];
        for( j = 0; j < ea->count; j++ )
        {
            character_BuyBoostSlot(respec.pchar, ea->pow);
            eaPush(&respec.slot_history, ea->pow);
            assert(eaSize(&respec.slot_history) <= CountForLevel(maxRespecLevel(), g_Schedules.aSchedules.piAssignableBoost));
        }
    }
}


// Increase a player's level up to the next level that additional slots are received.
// Then see if it can be increased further by clumping slots together (ie consective levels that add slots but no powers)
void respec_slotAdvanceLevel()
{
    // skip forward through levels that don't add boost slots
    while(   respec.pchar->iLevel < maxRespecLevel()
          && CountForLevel(respec.pchar->iLevel, g_Schedules.aSchedules.piAssignableBoost) == CountForLevel((respec.pchar->iLevel + 1), g_Schedules.aSchedules.piAssignableBoost))
    {
        respec.pchar->iLevel++;
    }

    // this is where we actually advance to a level that adds new boost slots
    respec.pchar->iLevel++;

    // skip forward over levels that do not hand out powers
    while(   respec.pchar->iLevel < maxRespecLevel()
          && CountForLevel(respec.pchar->iLevel, g_Schedules.aSchedules.piPower) == CountForLevel((respec.pchar->iLevel + 1), g_Schedules.aSchedules.piPower))
    {
        respec.pchar->iLevel++;
    }

    gNumEnhancesLeft = character_CountBuyBoostForLevel( respec.pchar, respec.pchar->iLevel);

    respec.free_boost_slots = character_CountBuyBoostForLevel(respec.pchar, maxRespecLevel()) - gNumEnhancesLeft;
}



static int findPreviousSlotLevel(int level)
{
    // skip backward over levels that do not hand out powers

    while(   level > 0
          && CountForLevel(level, g_Schedules.aSchedules.piPower) == CountForLevel((level - 1), g_Schedules.aSchedules.piPower))
    {
        level--;
    }

    // this is where we actually rewind to a level with fewer boost slots
    if(level > 0)
        level--;

    // skip backward through levels that don't add boost slots
    while(   level > 0
          && CountForLevel(level, g_Schedules.aSchedules.piAssignableBoost) == CountForLevel((level - 1), g_Schedules.aSchedules.piAssignableBoost))
    {
        level--;
    }

    return level;
}

// Rewind a player's level until we find the beginning of the previous block of levels that added slots.
// As levels are unwound, slots assigned at that level are placed back in the available pool
// (Essentially the reverse of respec_slotAdvanceLevel())
int respec_slotRewindLevel(bool end)
{
    int removed = 0;
    int prev;
    int prev2;


    // special case in which we are hitting the "Back" button and the character's actual
    // experience level was the last level at which slots were assigned
    if(end && respec.pchar->iLevel == respec.lastSlotLevel)
    {
        prev = respec.pchar->iLevel;
    }
    else
    {
        prev = findPreviousSlotLevel(respec.pchar->iLevel);
        assert(prev < respec.pchar->iLevel);
    }

    // We now have the new "rewound" level. Now we need to determine how many slots we assigned at the previous level

    prev2 = findPreviousSlotLevel(prev);

    if(prev != prev2)
    {
        // now remove slots that were assigned over the levels that we have just "undone"
        int i;

        removed = CountForLevel(prev, g_Schedules.aSchedules.piAssignableBoost) - CountForLevel(prev2, g_Schedules.aSchedules.piAssignableBoost);

        assert(eaSize(&respec.slot_history) == CountForLevel(prev, g_Schedules.aSchedules.piAssignableBoost));
        assert(eaSize(&respec.slot_history) >= removed);

        for(i=0; i<removed; i++)
        {
            Power * pow = eaPop(&respec.slot_history);

            assert(pow->iNumBoostsBought > 0);

            // at this point, we know that no enhancements are linked to this slot
            character_RemoveBoostSlot(respec.pchar, pow);
        }
    }

    respec.pchar->iLevel = prev;

    gNumEnhancesLeft = character_CountBuyBoostForLevel( respec.pchar, respec.pchar->iLevel);

    respec.free_boost_slots = character_CountBuyBoostForLevel(respec.pchar, maxRespecLevel()) - gNumEnhancesLeft;

    return removed;
}



//    allow player to assign (empty) boost slots
//
void respec_assignment(void)
{
    static ScrollBar sb[3] = { 0 };
    CBox box[3];
    float primY = (SL_FRAME_TOP + 50) - sb[0].offset;
    float secY = (SL_FRAME_TOP + 50) - sb[1].offset;
    float poolY = (SL_FRAME_TOP + 50) - sb[2].offset;
    int i;
    int screenScalingDisabled = is_scrn_scaling_disabled();
    float screenScaleX = 1.f, screenScaleY = 1.f, screenScale;

    calc_scrn_scaling(&screenScaleX, &screenScaleY);
    screenScale = MIN(screenScaleX, screenScaleY);

    set_scrn_scaling_disabled(1);
     drawMenuTitle( 10*screenScaleX, 30*screenScaleY, 20, "RespecChooseEnhancements", screenScale, !respec.assignmentInit );

    if(!respec.assignmentInit)
    {
        respec.pchar->iLevel = 0;    // assume no slots are given at level 0
        powerInfoSetLevel(respec.pchar->iLevel);
        respec_slotAdvanceLevel();

        respec.assignmentInit = TRUE;
    }

    font( &title_12 );
    font_color( CLR_WHITE, CLR_GREY );
    prnt( 1000, 30, 50, 1.f, 1.f, textStd("EquivilentLevel", respec.pchar->iLevel) );

    // the frames
    drawMenuFrame( R12, FRAME_SPACE*screenScaleX, SL_FRAME_TOP*screenScaleY, 10, FRAME_WIDTH*screenScaleX, FRAME_HEIGHT*screenScaleY, CLR_WHITE, 0x00000088, 0 ); // left
    BuildCBox( &box[0], FRAME_SPACE*screenScaleX, SL_FRAME_TOP*screenScaleY, FRAME_WIDTH*screenScaleX, FRAME_HEIGHT );
    drawMenuFrame( R12, (DEFAULT_SCRN_WD/2 - FRAME_WIDTH/2)*screenScaleX, SL_FRAME_TOP*screenScaleY, 10, FRAME_WIDTH*screenScaleX, FRAME_HEIGHT*screenScaleY, CLR_WHITE, 0x00000088, 0 ); // middle
    BuildCBox( &box[1], (DEFAULT_SCRN_WD/2 - FRAME_WIDTH/2)*screenScaleX, SL_FRAME_TOP*screenScaleY, FRAME_WIDTH*screenScaleX, FRAME_HEIGHT*screenScaleY );
    drawMenuFrame( R12, (DEFAULT_SCRN_WD - FRAME_WIDTH - FRAME_SPACE)*screenScaleX, SL_FRAME_TOP*screenScaleY, 10, FRAME_WIDTH*screenScaleX, POOL_HEIGHT*screenScaleY, CLR_WHITE, 0x00000088, 0 ); // right
    BuildCBox( &box[2], (DEFAULT_SCRN_WD - FRAME_WIDTH - FRAME_SPACE)*screenScaleX, SL_FRAME_TOP*screenScaleY, FRAME_WIDTH*screenScaleX, POOL_HEIGHT*screenScaleY );

    if( D_MOUSEHIT == drawStdButton( (FRAME_SPACE+FRAME_WIDTH/2)*screenScaleX, (SL_FRAME_TOP-20)*screenScaleY, 45, 120*screenScaleX, 20*screenScaleY, SELECT_FROM_UISKIN( CLR_BTN_HERO, CLR_BTN_VILLAIN, CLR_BTN_ROGUE ), (s_ShowCombatNumbersHere==1)?"HidePowerNumbers":"ShowCombatNumbersHere", 1.f, 0 ))
        (s_ShowCombatNumbersHere==1)?(s_ShowCombatNumbersHere = 0):(s_ShowCombatNumbersHere = 1);
    if( D_MOUSEHIT == drawStdButton( (DEFAULT_SCRN_WD/2)*screenScaleX, (SL_FRAME_TOP-20)*screenScaleY, 45, 120*screenScaleX, 20*screenScaleY, SELECT_FROM_UISKIN( CLR_BTN_HERO, CLR_BTN_VILLAIN, CLR_BTN_ROGUE ), (s_ShowCombatNumbersHere==2)?"HidePowerNumbers":"ShowCombatNumbersHere", 1.f, 0 ))
        (s_ShowCombatNumbersHere==2)?(s_ShowCombatNumbersHere = 0):(s_ShowCombatNumbersHere = 2);
    if( D_MOUSEHIT == drawStdButton( (DEFAULT_SCRN_WD - FRAME_WIDTH/2 - FRAME_SPACE)*screenScaleX, (SL_FRAME_TOP-20)*screenScaleY, 45, 120*screenScaleX, 20*screenScaleY, SELECT_FROM_UISKIN( CLR_BTN_HERO, CLR_BTN_VILLAIN, CLR_BTN_ROGUE ), (s_ShowCombatNumbersHere==3)?"HidePowerNumbers":"ShowCombatNumbersHere", 1.f, 0 ))
        (s_ShowCombatNumbersHere==3)?(s_ShowCombatNumbersHere = 0):(s_ShowCombatNumbersHere = 3);

    if( s_ShowCombatNumbersHere == 1)
        menuDisplayPowerInfo( FRAME_SPACE*screenScaleX, SL_FRAME_TOP*screenScaleY, 30, FRAME_WIDTH*screenScaleX, FRAME_HEIGHT*screenScaleY, g_pPowMouseover?g_pPowMouseover->ppowBase:0, 0,  POWERINFO_BASE  );
    if( s_ShowCombatNumbersHere == 2)
        menuDisplayPowerInfo( (DEFAULT_SCRN_WD/2 - FRAME_WIDTH/2)*screenScaleX, SL_FRAME_TOP*screenScaleY, 30, FRAME_WIDTH*screenScaleX, FRAME_HEIGHT*screenScaleY,  g_pPowMouseover?g_pPowMouseover->ppowBase:0, 0,  POWERINFO_BASE );
    if( s_ShowCombatNumbersHere == 3)
        menuDisplayPowerInfo( (DEFAULT_SCRN_WD - FRAME_WIDTH - FRAME_SPACE)*screenScaleX, SL_FRAME_TOP*screenScaleY, 30, FRAME_WIDTH*screenScaleX, POOL_HEIGHT*screenScaleY,  g_pPowMouseover?g_pPowMouseover->ppowBase:0, 0, POWERINFO_BASE );


    // remeber the maxium amount to buy
    respec_specCounter(screenScaleX, screenScaleY);

    

    enhancementAnim_update(kPowerSystem_Powers);

    // Skip all temp powers
    for( i = 0; i < eaSize(&respec.pchar->ppPowerSets); i++ )
    {
        int j, top;
        if(stricmp(respec.pchar->ppPowerSets[i]->psetBase->pcatParent->pchName,"Temporary_Powers") == 0)
            continue;
        if( respec.pchar->ppPowerSets[i]->psetBase->bShowInManage )
        {
            if( respec.pchar->ppPowerSets[i]->psetBase->pcatParent == respec.pchar->pclass->pcat[0] && s_ShowCombatNumbersHere!=1)
            {
                CBox clipBox;
                BuildCBox(&clipBox, 0, (SL_FRAME_TOP + 5)*screenScaleY, DEFAULT_SCRN_WD*screenScaleX, (FRAME_HEIGHT - 11)*screenScaleY );
                clipperPushCBox(&clipBox);

                top = primY;

                setHeadingFontFromSkin(0);
                cprnt( (FRAME_SPACE+FRAME_WIDTH/2)*screenScaleX, (top-23)*screenScaleY, 20, screenScale, screenScale, respec.pchar->ppPowerSets[i]->psetBase->pchDisplayName );

                for( j = 0; j < eaSize(&respec.pchar->ppPowerSets[i]->ppPowers); j++ )
                {
                    Power *pow = respec.pchar->ppPowerSets[i]->ppPowers[j];
                    if(pow && pow->ppowBase->bShowInManage)
                    {
                        int selectable = pow->iLevelBought <= respec.pchar->iLevel;
                        drawPowerAndEnhancements( respec.pchar, pow, FRAME_SPACE, primY, 20, 1.f, FALSE, &sb[0].offset, selectable, screenScaleX, screenScaleY, &clipBox );
                        primY += POWER_SPEC_HT;
                    }
                }

                // draw the small inner frame
                drawFrame( PIX2, R6, (FRAME_SPACE + POWER_OFFSET)*screenScaleX, (top - 30)*screenScaleY, 5, (POWER_WIDTH - 10)*screenScaleX, (primY - top - 35)*screenScaleY, screenScale, 0xffffffaa, SELECT_FROM_UISKIN( 0x0000ff44, 0xff000044, 0x00000066 ) );

                primY += POWER_SPEC_HT;
                clipperPop();
            }
            else if( respec.pchar->ppPowerSets[i]->psetBase->pcatParent == respec.pchar->pclass->pcat[1] && s_ShowCombatNumbersHere!=2)
            {
                CBox clipBox;
                BuildCBox(&clipBox, 0, (SL_FRAME_TOP + 5)*screenScaleY, DEFAULT_SCRN_WD*screenScaleX, (FRAME_HEIGHT - 11)*screenScaleY );
                clipperPushCBox(&clipBox);

                top = secY;

                setHeadingFontFromSkin(0);
                cprnt( DEFAULT_SCRN_WD*screenScaleX/2, (top-23)*screenScaleY, 20, screenScale, screenScale, respec.pchar->ppPowerSets[i]->psetBase->pchDisplayName );

                for( j = 0; j < eaSize(&respec.pchar->ppPowerSets[i]->ppPowers); j++ )
                {
                    Power *pow = respec.pchar->ppPowerSets[i]->ppPowers[j];
                    if(pow && pow->ppowBase->bShowInManage)
                    {
                        int selectable = pow->iLevelBought <= respec.pchar->iLevel;
                        drawPowerAndEnhancements( respec.pchar, pow, DEFAULT_SCRN_WD/2 - FRAME_WIDTH/2, secY, 20, 1.f, FALSE, &sb[1].offset, selectable, screenScaleX, screenScaleY, &clipBox );
                        secY += POWER_SPEC_HT;
                    }
                }

                // draw the small inner frame
                drawFrame( PIX2, R6, (DEFAULT_SCRN_WD/2 - FRAME_WIDTH/2 + POWER_OFFSET)*screenScaleX, (top - 30)*screenScaleY, 5, (POWER_WIDTH - 10)*screenScaleX, (secY - top - 35)*screenScaleY, screenScale, 0xffffffaa, SELECT_FROM_UISKIN( 0x0000ff44, 0xff000044, 0x00000066 ) );

                secY += POWER_SPEC_HT;
                clipperPop();
            }
            else if(s_ShowCombatNumbersHere!=3)
            {
                CBox clipBox;
                BuildCBox(&clipBox, 0, (SL_FRAME_TOP + 5)*screenScaleY, DEFAULT_SCRN_WD*screenScaleX, (POOL_HEIGHT - 11)*screenScaleY );
                clipperPushCBox(&clipBox);

                top = poolY;

                setHeadingFontFromSkin(0);
                cprnt( (DEFAULT_SCRN_WD - FRAME_WIDTH/2 - FRAME_SPACE)*screenScaleX, (top-23)*screenScaleY, 20, screenScale, screenScale, respec.pchar->ppPowerSets[i]->psetBase->pchDisplayName );

                for( j = 0; j < eaSize(&respec.pchar->ppPowerSets[i]->ppPowers); j++ )
                {
                    Power *pow = respec.pchar->ppPowerSets[i]->ppPowers[j];
                    if(pow && pow->ppowBase->bShowInManage)
                    {
                        int selectable = pow->iLevelBought <= respec.pchar->iLevel;
                        drawPowerAndEnhancements( respec.pchar, pow, DEFAULT_SCRN_WD - FRAME_WIDTH - FRAME_SPACE, poolY, 20, 1.f, FALSE, &sb[2].offset, selectable, screenScaleX, screenScaleY,&clipBox );
                        poolY += POWER_SPEC_HT;
                    }
                }

                // draw the small inner frame
                drawFrame( PIX2, R6, (DEFAULT_SCRN_WD - FRAME_WIDTH - FRAME_SPACE + POWER_OFFSET)*screenScaleX, (top - 30)*screenScaleY, 5, (POWER_WIDTH - 10)*screenScaleX, (poolY - top - 35)*screenScaleY, screenScale, 0xffffffaa, SELECT_FROM_UISKIN( 0x0000ff44, 0xff000044, 0x00000066 ) );

                poolY += POWER_SPEC_HT;
                clipperPop();
            }
        }
    }

    // scroll bars of doom (first two power scrollbars have been disabled)
    doScrollBarEx( &sb[0], (FRAME_HEIGHT - 42), (primY + sb[0].offset - (3 * POWER_SPEC_HT)), (FRAME_WIDTH + FRAME_SPACE), 112, 20, &box[0], 0, screenScaleX, screenScaleY );
       doScrollBarEx( &sb[1], (FRAME_HEIGHT - 42), (secY  + sb[1].offset - (3 * POWER_SPEC_HT)),  (DEFAULT_SCRN_WD/2 + FRAME_WIDTH/2), 112, 20, &box[1], 0, screenScaleX, screenScaleY );
       doScrollBarEx( &sb[2], (POOL_HEIGHT - 42), (poolY + sb[2].offset - (3 * POWER_SPEC_HT)), (DEFAULT_SCRN_WD - FRAME_SPACE), 112, 20, &box[2], 0, screenScaleX, screenScaleY );

    info_PowManagement(kPowInfo_SpecLevel, screenScaleX, screenScaleY );

    if ( drawNextButton( 40*screenScaleX,  740*screenScaleY, 10, screenScale, 0, 0, 0, "BackString" ))
    {
        sndPlay( "back", SOUND_GAME );

        enhancementAnim_clear();
        fadingText_clearAll();
        electric_clearAll();

        if(!respec_slotRewindLevel(false))
        {
            // clean up this menu
            respec_assignSlots();
            respec_clearSpecs();

            respec.assignmentInit = false;

            // power menu prep
            respec.pchar->iLevel = maxRespecLevel();
            respec_RemoveLastPower();
            respec.menuFunc = respec_powerLevel;
            s_ShowCombatNumbersHere = 0;
            powerInfoSetLevel(respec.pchar->iLevel);
        }
    }

    // don't let them continue until they've bought all their slots
    if (drawNextButton( 980*screenScaleX, 740*screenScaleY, 20, screenScale, 1, (gNumEnhancesLeft>0), 0, "NextString" ) || (!gNumEnhancesLeft && inpEdgeNoTextEntry(INP_RETURN))  )
    {
        sndPlay( "next", SOUND_GAME );

        respec_assignSlots();
        enhancementAnim_clear();
        fadingText_clearAll();
        electric_clearAll();

        respec.lastSlotLevel = respec.pchar->iLevel;

        respec_slotAdvanceLevel();
        powerInfoSetLevel(respec.pchar->iLevel);

        if(respec.pchar->iLevel > maxRespecLevel())
        {
            // placement prep
            respec.pchar->iLevel = maxRespecLevel();
            respec_createPile();
            respec.menuFunc = respec_placement;
        }
    }
    set_scrn_scaling_disabled(screenScalingDisabled);

}

//------------------------------------------------------------------------------------------
// PLACE ENHANCEMENT ///////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------------------
//



//
// sort by boost name, and then by boost level
// static int __cdecl comparePileBoost (const void *a, const void *b)
int comparePileBoost(const void *a, const void *b, const void *context)
{
    const Boost * boostA = *((const Boost**) a);
    const Boost * boostB = *((const Boost**) b);

    if(boostA && boostA->ppowBase && boostB && boostB->ppowBase)
    {
        int res = stricmp(boostA->ppowBase->pchName, boostB->ppowBase->pchName);
        if(res == 0)
        {
            int a = boostA->iLevel + boostA->iNumCombines;
            int b = boostB->iLevel + boostB->iNumCombines;
            if(a == b)
            {
                // boosts with combines come before uncombined boosts
                if(boostA->iNumCombines < boostB->iNumCombines)
                    return 1;
                else
                    return -1;
            }
            else if(a < b)
                return -1;
            else
                return 1;
        }
        else
            return res;
    }
    else
    {
        return -1;    // arbitrary
    }
}










//
//
void drawRespecSpecialization( int x, int y, int z, Character *pchar, Power *pow, Boost *pBoost, int type, int iset, int ipow, int ispec, float screenScaleX, float screenScaleY )
{
    AtlasTex * border = atlasLoadTexture("Powers_specialization_border.tga");
    AtlasTex * back   = atlasLoadTexture("Powers_specialization_back.tga");
    AtlasTex * pog = NULL;

    bool bMatches = true;
    bool bDragging = false;
    int state = kBoostState_Normal;
    float screenScale = MIN(screenScaleX, screenScaleY);
    CBox box;
    static TrayObj to_tmp = {0}; // storage area for context menu

    bDragging = cursor.dragging;

    if(bDragging && type != kTrayItemType_RespecPile )
    {
        // swapping from inventory to power or power to power is disallowed
        if( (type == kTrayItemType_SpecializationInventory || type == kTrayItemType_SpecializationPower) && cursor.drag_obj.type == kTrayItemType_SpecializationPower && pBoost )
        {
            bMatches = FALSE;
        }
        else if(pow && bMatches)
        {
            // OK, they're in a drag from the inventory. Determine if they're
            // allowed to drop it on this spot.

            Boost * draggedBoost = 0;

            bMatches = false;

            if( cursor.drag_obj.type == kTrayItemType_RespecPile )
                draggedBoost = respec.pile[cursor.drag_obj.ispec];
            else if( cursor.drag_obj.type == kTrayItemType_SpecializationInventory )
                draggedBoost = pchar->aBoosts[cursor.drag_obj.ispec];
            else if( cursor.drag_obj.type == kTrayItemType_SpecializationPower )
                draggedBoost = pchar->ppPowerSets[cursor.drag_obj.iset]->ppPowers[cursor.drag_obj.ipow]->ppBoosts[cursor.drag_obj.ispec];

            assert(draggedBoost);
            if(draggedBoost) 
            {
                int iCharLevel = character_CalcExperienceLevel(pchar);
                int iBoostLevel = draggedBoost->iLevel+draggedBoost->iNumCombines;

                if (draggedBoost->ppowBase && draggedBoost->ppowBase->bBoostBoostable)
                    iBoostLevel = draggedBoost->iLevel;

                bMatches = ( iCharLevel >= power_MinBoostUseLevel(draggedBoost->ppowBase,iBoostLevel) &&
                    iCharLevel <= power_MaxBoostUseLevel(draggedBoost->ppowBase,iBoostLevel) &&
                    power_IsValidBoost(pow, draggedBoost) &&
                    power_ValidBoostSlotRequired(pow, draggedBoost) &&
                    power_BoostCheckRequires(pow, draggedBoost) &&
                    !boost_InventionLicenseLocked(draggedBoost, pchar->entParent));
            }
        }

        if(bMatches)
            state = kBoostState_Selected;
        else
            state = kBoostState_Disabled;
    }

    // set info
    BuildCBox( &box, x*screenScaleX-border->width*screenScale/2, y*screenScaleY-border->height*screenScale/2, border->width*screenScale, border->height*screenScale );

    pog = drawMenuEnhancement( pBoost,x*screenScaleX, y*screenScaleY, z, 0.5f*screenScale, state, 0, false, MENU_RESPEC, true);

    // no need to go any further if not dragginf something
    if( !bMatches )
        return;

    if(pBoost && mouseCollision( &box ) )
    {
        if( !cursor.dragging)
            setCursor( "cursor_hand_open.tga", NULL, 0, 0, 0 );
        info_CombineSpec( pBoost->ppowBase, 1, pBoost->iLevel, pBoost->iNumCombines );
    }

    // check for a drag if slot is inventory slot
    if( pBoost && mouseDownHit( &box, MS_LEFT ))
    {
        TrayObj ts = {0};

        ts.type = type;
        ts.iset = iset;
        ts.ipow = ipow;
        ts.ispec = ispec;
        ts.scale = 1.f;
        ts.icon = atlasLoadTexture( pBoost->ppowBase->pchIconName );

        trayobj_startDragging( &ts, ts.icon, pog ); 
    }

    //check for mouse release regardless of slot
    if( bDragging && mouseCollision(&box)) 
    {
        if (game_state.enableBoostDiminishing && pow)
        {
            Boost *dragSpec = NULL;
            Boost *replacedBoost = pow->ppBoosts[ispec];
            char *temp;
            if( cursor.drag_obj.type == kTrayItemType_RespecPile )
                dragSpec = respec.pile[cursor.drag_obj.ispec];
            if( cursor.drag_obj.type == kTrayItemType_SpecializationInventory )
                dragSpec = pchar->aBoosts[cursor.drag_obj.ispec];
            if( cursor.drag_obj.type == kTrayItemType_SpecializationPower ) {
                if (pow != pchar->ppPowerSets[cursor.drag_obj.iset]->ppPowers[cursor.drag_obj.ipow])
                    dragSpec = pchar->ppPowerSets[cursor.drag_obj.iset]->ppPowers[cursor.drag_obj.ipow]->ppBoosts[cursor.drag_obj.ispec];
            }
            if (dragSpec) {            
                temp = getEffectiveString(pow,pow->ppBoosts[ispec],dragSpec,ispec);
                if (temp && temp[0])
                    forceDisplayToolTip(&enhanceTip,&box,temp,MENU_RESPEC,0);
            }
        }
        if( !isDown( MS_LEFT ))
        {
            Boost *replacedBoost = NULL;
            Boost *slottedBoost = NULL;

            // Going into player's inventory
            if( type == kTrayItemType_SpecializationInventory )
            {
                if( pchar->aBoosts[ispec] && pchar->aBoosts[ispec]->ppowBase )
                    replacedBoost = pchar->aBoosts[ispec];

                // coming from respec pile
                if( cursor.drag_obj.type == kTrayItemType_RespecPile )
                {
                    slottedBoost = respec.pile[cursor.drag_obj.ispec];

                    // add to inventory
                    respec.pchar->aBoosts[ispec] = respec.pile[cursor.drag_obj.ispec];

                    //remove from pile
                    eaRemove( &respec.pile, cursor.drag_obj.ispec );

                    //swap to pile if nessecary
                    if( replacedBoost && replacedBoost->ppowBase ) 
                    {
                        eaPush( &respec.pile, replacedBoost );
                    }

                }

                // coming from player's inventory
                if( cursor.drag_obj.type == kTrayItemType_SpecializationInventory )
                {
                    slottedBoost = pchar->aBoosts[cursor.drag_obj.ispec];

                    // add to inventory
                    respec.pchar->aBoosts[ispec] = pchar->aBoosts[cursor.drag_obj.ispec];

                    // swap inventory or destroy from slot
                    if( replacedBoost && replacedBoost->ppowBase )
                        pchar->aBoosts[cursor.drag_obj.ispec] =  replacedBoost;
                    else
                        pchar->aBoosts[cursor.drag_obj.ispec] = NULL;
                }

                // coming from a power
                if( cursor.drag_obj.type == kTrayItemType_SpecializationPower )
                {
                    slottedBoost = pchar->ppPowerSets[cursor.drag_obj.iset]->ppPowers[cursor.drag_obj.ipow]->ppBoosts[cursor.drag_obj.ispec];
                    slottedBoost->ppowParent = NULL;

                    // add to inventory (assumed can't drag here from power if slot is occupied)
                    respec.pchar->aBoosts[ispec] = slottedBoost;

                    power_RemoveBoost( pchar->ppPowerSets[cursor.drag_obj.iset]->ppPowers[cursor.drag_obj.ipow], cursor.drag_obj.ispec, NULL );
                    uiEnhancementRefreshPower(pchar->ppPowerSets[cursor.drag_obj.iset]->ppPowers[cursor.drag_obj.ipow]);

                }
            }
            else if ( type == kTrayItemType_SpecializationPower ) // going into a power
            {
                Boost *dragSpec = NULL;
                int iValidToIdx;
                // There are way too many copies of code like this in here already
                if( cursor.drag_obj.type == kTrayItemType_RespecPile )
                    dragSpec = respec.pile[cursor.drag_obj.ispec];
                if( cursor.drag_obj.type == kTrayItemType_SpecializationInventory )
                    dragSpec = pchar->aBoosts[cursor.drag_obj.ispec];
                if( cursor.drag_obj.type == kTrayItemType_SpecializationPower )
                    dragSpec = pchar->ppPowerSets[cursor.drag_obj.iset]->ppPowers[cursor.drag_obj.ipow]->ppBoosts[cursor.drag_obj.ispec];

                slottedBoost = dragSpec;

                iValidToIdx = power_ValidBoostSlotRequired(pow, dragSpec);
                if(iValidToIdx<0 || iValidToIdx == ispec)
                {
                    if( pow->ppBoosts[ispec] && pow->ppBoosts[ispec]->ppowBase )
                        replacedBoost = pow->ppBoosts[ispec];

                    // coming from the respec pile
                    if( cursor.drag_obj.type == kTrayItemType_RespecPile )
                    {
                        // add boost to power
                        pow->ppBoosts[ispec] = respec.pile[cursor.drag_obj.ispec];
                        pow->ppBoosts[ispec]->ppowParent = pow;
                        uiEnhancementRefreshPower(pow);

                        // remove boost from pile
                        eaRemove( &respec.pile, cursor.drag_obj.ispec );

                        // add replaced boost to pile if needed
                        if( replacedBoost && replacedBoost->ppowBase ) {
                            eaPush( &respec.pile, replacedBoost );
                            replacedBoost->ppowParent = NULL;
                        }
                    }

                    // coming from the player's inventory
                    if( cursor.drag_obj.type == kTrayItemType_SpecializationInventory )
                    {
                        // add boost to power
                        pow->ppBoosts[ispec] = pchar->aBoosts[cursor.drag_obj.ispec];
                        pow->ppBoosts[ispec]->ppowParent = pow;
                        uiEnhancementRefreshPower(pow);

                        // remove boost from inventory
                        pchar->aBoosts[cursor.drag_obj.ispec] = 0;

                        // add replaced boost to inventory if needed
                        if( replacedBoost && replacedBoost->ppowBase )
                        {
                            pchar->aBoosts[cursor.drag_obj.ispec] = replacedBoost;
                            replacedBoost->ppowParent = NULL;
                        }
                    }

                    // coming from a power
                    if( cursor.drag_obj.type == kTrayItemType_SpecializationPower )
                    {
                        // add boost to power (assumed there is no swapping in this case)
                        pow->ppBoosts[ispec] = pchar->ppPowerSets[cursor.drag_obj.iset]->ppPowers[cursor.drag_obj.ipow]->ppBoosts[cursor.drag_obj.ispec];
                        pow->ppBoosts[ispec]->ppowParent = pow;
                        uiEnhancementRefreshPower(pow);

                        power_RemoveBoost( pchar->ppPowerSets[cursor.drag_obj.iset]->ppPowers[cursor.drag_obj.ipow], cursor.drag_obj.ispec, NULL );
                        uiEnhancementRefreshPower ( pchar->ppPowerSets[cursor.drag_obj.iset]->ppPowers[cursor.drag_obj.ipow] );
                    }
                }

            }
            else if( type == kTrayItemType_RespecPile ) // going to the respec pile
            {
                // coming from the player's inventory
                if( cursor.drag_obj.type == kTrayItemType_SpecializationInventory )
                {
                    assert(respec.pchar->aBoosts[cursor.drag_obj.ispec]);

                    slottedBoost = respec.pchar->aBoosts[cursor.drag_obj.ispec];

                    // add to pile
                    eaPush(&respec.pile, respec.pchar->aBoosts[cursor.drag_obj.ispec]);

                    //remove from inventory
                    pchar->aBoosts[cursor.drag_obj.ispec] = NULL;
                }
                else if( cursor.drag_obj.type == kTrayItemType_SpecializationPower ) // coming from a power
                {
                    slottedBoost = pchar->ppPowerSets[cursor.drag_obj.iset]->ppPowers[cursor.drag_obj.ipow]->ppBoosts[cursor.drag_obj.ispec];

                    // add to pile
                    eaPush(&respec.pile, pchar->ppPowerSets[cursor.drag_obj.iset]->ppPowers[cursor.drag_obj.ipow]->ppBoosts[cursor.drag_obj.ispec]);
                    pchar->ppPowerSets[cursor.drag_obj.iset]->ppPowers[cursor.drag_obj.ipow]->ppBoosts[cursor.drag_obj.ispec]->ppowParent = NULL;

                    // remove from power
                    power_RemoveBoost( pchar->ppPowerSets[cursor.drag_obj.iset]->ppPowers[cursor.drag_obj.ipow], cursor.drag_obj.ispec, NULL );
                }
            }
 
            if (slottedBoost)
                uiEnhancementRefresh(slottedBoost->pUI);
            if (replacedBoost)
                uiEnhancementRefresh(replacedBoost->pUI);

            // whew
            trayobj_stopDragging();
        }
    }
}


//
// total up the value of all enhancements in the pile. We use the "Buy" value since the player is in the "store" role.
static int respec_computePileValue()
{
    int influence = 0;
    int i;

    for( i = 0; i < eaSize(&respec.pile); i++ )
    {
        Boost * boost = respec.pile[i];
        const StoreItem *psi = store_FindItem(boost->ppowBase, (boost->iLevel + boost->iNumCombines));
        if(psi)
        {
            influence += psi->iSell;
        }
    }

    return influence;
}



static void s_respec_drawpile(float screenScaleX, float screenScaleY)
{
    int i;
    int pile_ht = 0;    // keep track of number of pile rows drawn
    static ScrollBar sb = {0};
    float screenScale = MIN(screenScaleX, screenScaleY);
    int x = (RESPEC_PILE_X + ((SPEC_WD * 0.5) + 16)*screenScale/screenScaleX);
    int y = RESPEC_PILE_Y + ((SPEC_WD * 0.5) + 10)*screenScale/screenScaleY - sb.offset;
    int doc_ht = 30 + (SPEC_WD + 4) * (1 + (eaSize(&respec.pile) / (RESPEC_SLOTS_PER_ROW)));
    CBox box;
    // sort the pile
//    eaQSort(respec.pile, comparePileBoost);
    stableSort(respec.pile, eaSize(&respec.pile), sizeof(ToolTip *), NULL, comparePileBoost);

    font( &gamebold_12 );
    font_color( CLR_WHITE, 0x88b6ffff );
    cprnt( (RESPEC_PILE_X + (RESPEC_PILE_WIDTH / 2))*screenScaleX, RESPEC_PILE_Y *screenScaleY, 25, screenScale, screenScale, "RespecPile", respec_computePileValue() );

    BuildCBox(&box,(RESPEC_PILE_X*screenScaleX) + (15*screenScale), (RESPEC_PILE_Y*screenScaleY) + (7*screenScale), RESPEC_PILE_WIDTH*screenScaleX, (RESPEC_PILE_HEIGHT*screenScaleY) - (12*screenScale));
    clipperPushCBox(&box);
    // draw the pile
    for( i = 0; i < eaSize(&respec.pile); i++ )
    {
        drawRespecSpecialization( x, y, 30, respec.pchar, NULL, respec.pile[i], kTrayItemType_RespecPile, 0, 0, i, screenScaleX, screenScaleY);

        x += (SPEC_WD + 12)*screenScale/screenScaleX;
        if( x > ((RESPEC_PILE_X + RESPEC_PILE_WIDTH) - (SPEC_WD * 0.5)) )
        {
            x = (RESPEC_PILE_X + ((SPEC_WD * 0.5) + 16)*screenScale/screenScaleX);
            y += (SPEC_WD + 4)*screenScale/screenScaleY;
            pile_ht += (SPEC_WD + 4)*screenScale/screenScaleY;
        }
    }

    clipperPop();

    if(pile_ht > RESPEC_PILE_HEIGHT*screenScaleY)
        doScrollBarEx(&sb, (RESPEC_PILE_HEIGHT - 35), doc_ht, (RESPEC_PILE_X + RESPEC_PILE_WIDTH), (RESPEC_PILE_Y + 17), 40, &box, 0, screenScaleX, screenScaleY);
    else
        sb.offset = 0;


    // check for mouse hits to the large frame
    if( cursor.dragging )
    {
        CBox box;

        BuildCBox( &box, RESPEC_PILE_X*screenScaleX, RESPEC_PILE_Y*screenScaleY, RESPEC_PILE_WIDTH*screenScaleX, RESPEC_PILE_HEIGHT*screenScaleY);
        if( mouseCollision( &box ) )
        {
            drawMenuFrame( R12, RESPEC_PILE_X*screenScaleX, RESPEC_PILE_Y*screenScaleY, 10, RESPEC_PILE_WIDTH*screenScaleX, RESPEC_PILE_HEIGHT*screenScaleY, CLR_GREEN, 0x33333388, 0 );

            if( !isDown(MS_LEFT) )
            {
                if( cursor.drag_obj.type == kTrayItemType_SpecializationInventory )
                {
                    // add to pile
                    eaPush( &respec.pile, respec.pchar->aBoosts[cursor.drag_obj.ispec] );
                    
                    uiEnhancementRefresh(respec.pchar->aBoosts[cursor.drag_obj.ispec]->pUI);
                    respec.pchar->aBoosts[cursor.drag_obj.ispec]->ppowParent = NULL;

                    //destroy from slot
                    respec.pchar->aBoosts[cursor.drag_obj.ispec] = NULL;
                }

                if( cursor.drag_obj.type == kTrayItemType_SpecializationPower )
                {
                    Power *pPower = respec.pchar->ppPowerSets[cursor.drag_obj.iset]->ppPowers[cursor.drag_obj.ipow];
                    Boost *pBoost = pPower->ppBoosts[cursor.drag_obj.ispec];

                    // add to pile
                    eaPush( &respec.pile,  pBoost);
                    uiEnhancementRefresh(pBoost->pUI);
                    pBoost->ppowParent = NULL;

                    // remove from power
                    power_RemoveBoost( pPower, cursor.drag_obj.ispec, NULL );
                    uiEnhancementRefreshPower(pPower);

                }

                trayobj_stopDragging();
            }
        }
        else
            drawMenuFrame( R12, RESPEC_PILE_X*screenScaleX, RESPEC_PILE_Y*screenScaleY, 10, RESPEC_PILE_WIDTH*screenScaleX, RESPEC_PILE_HEIGHT*screenScaleY, CLR_GREEN, CLR_BLACK, 0 );
    }
    else
          drawMenuFrame( R12, RESPEC_PILE_X*screenScaleX, RESPEC_PILE_Y*screenScaleY, 10, RESPEC_PILE_WIDTH*screenScaleX, RESPEC_PILE_HEIGHT*screenScaleY, CLR_WHITE, CLR_BLACK, 0 );

}

// draws the inventory with respec specialization slots
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

static void s_drawReSpecInventory( int x, int y, int z, float screenScaleX, float screenScaleY )
{
    int i;
    float screenScale = MIN(screenScaleX, screenScaleY);
    int tabColor;
    char *pSelectedTab;
    int tabIndex;

    s_initEnhancementTabs();

    font( &gamebold_12 );
    font_color( CLR_WHITE, 0x88b6ffff );
    cprnt( ((RESPEC_INV_WIDTH*screenScale / (2*screenScaleX)) + x)*screenScaleX, y*screenScaleY, 20, screenScale, screenScale, "SpecializationsInventory" );

    tabColor = s_getTabColor();
    drawTabControl(s_enhancementTabsOfTen, (x+R16)*screenScaleX, (y+2)*screenScaleY, z, RESPEC_INV_WIDTH - 2*R10*screenScale, TAB_HEIGHT*screenScaleY, screenScale, tabColor, tabColor, TabDirection_Horizontal);
    y += 16 * screenScale;

    pSelectedTab = (char *)uiTabControlGetSelectedData(s_enhancementTabsOfTen);
    tabIndex = atoi(pSelectedTab) - 1;

    for( i = 0; i < 10; i++ )
    {
        int tx = x + (10 + ((i+0.5) * SPEC_WD))*screenScale/screenScaleX;
        int ty = y + (10 + (0.5 * SPEC_WD))*screenScale/screenScaleY;
        int ispec = 10 * tabIndex + i;

        drawRespecSpecialization( tx, ty, z, respec.pchar, NULL, respec.pchar->aBoosts[ispec], kTrayItemType_SpecializationInventory, 0, 0, 10 * tabIndex + i, screenScaleX, screenScaleY );
    }
}






// Complete the respec -- send info to server, assume it's correct, swap entity info, & set flag to wait for server verification
//
void respec_finalize(void * data)
{
    Entity *e = playerPtr();
    int i, j, tray = 0, slot = 0;
    PowerRef ppowRef;

    // for debugging only
    static int dbgCallCount = 0;
    dbgCallCount++;

    ppowRef.buildNum = e->pchar->iCurBuild;
    // clear all the timers
    for( i = 0; i <eaSize(&e->pchar->ppPowerSets); i++ )
    {
        ppowRef.powerSet = i;
        for( j = 0; j < eaSize(&e->pchar->ppPowerSets[i]->ppPowers); j++ )
        {
            ppowRef.power = j;
            powerInfo_ChangeActiveState(e->powerInfo, ppowRef, 0 );
            powerInfo_ChangeRechargeTimer(e->powerInfo, ppowRef, 0);
        }
    }

    // make sure that level is finalized -- this is probably redundant
    character_LevelFinalize(respec.pchar, true);

    // send respec to server
    sendCharacterRespec( respec.pchar, respec.newPatron );

    // need to clear out inventory on client (server will update)
    {
        int i;
        for(i=0;i<CHAR_BOOST_MAX;i++)
        {
            boost_Destroy(e->pchar->aBoosts[i]);
            e->pchar->aBoosts[i] = 0;
        }
    }

    // clean up temporary boosts
    respec_clearPowerBoosts();

    // now we wait...
    gWaitingVerifyRespec = true;    // set flag to wait for server verification
}


// give the user a final warning before we irreversibly swap their character stats. If they say "No", we'll return to the calling function.
static void respec_finalWarning(void * data)
{
    char *buffer = textStd( "RespecFinal" );
    // use "_YES_NO" since "_TWO_RESPONSE" draws funny button sizes
    dialogStd( DIALOG_YES_NO, buffer, "RespecWarningContinue", "RespecWarningCancel", respec_finalize, NULL, 0 );
}



//
// called by net comm code after mapserver validates respec.
//        On success, it will finish the character swap.
//        On failure, it restore the old character.
// The game will resume after this function is called.
void respec_validate(Packet * pak)
{
//    int i,j,k;
    Entity * e = playerPtr();
    bool success;

    if(!gWaitingVerifyRespec)
        return;

    gWaitingVerifyRespec = false;

    success = pktGetBits(pak,1);

    respec_ClearData(1);
    start_menu( MENU_GAME );

    if(success)
    {
        // make new default tray
        character_RespecTrayReset(e);
    }
    else
        dialogStd(DIALOG_OK, "RespecFailed", NULL, NULL, NULL, NULL, 0);
}


//
// allow player to place enhancements in open power slots

static void respec_placement(void)
{
    int i;
    static ScrollBar sb[3] = {0};
    CBox box[3];
    int iSize;
    int primY = 120 - sb[0].offset;
    int secY  = 120 - sb[1].offset;
    int poolY = 120 - sb[2].offset;
    int screenScalingDisabled = is_scrn_scaling_disabled();
    float screenScaleX = 1.f, screenScaleY = 1.f, screenScale;

    calc_scrn_scaling(&screenScaleX, &screenScaleY);
    screenScale = MIN(screenScaleX, screenScaleY);

    set_scrn_scaling_disabled(1);
    drawMenuTitle( 10*screenScaleX, 30*screenScaleY, 20, "RespecPlaceEnhancements", screenScale, !respec.placementInit );

    if( !respec.placementInit )
    {
        respec.placementInit = TRUE;
        sb[0].offset = 0;
        sb[1].offset = 0;
        sb[2].offset = 0;
    }

    // the frames
    drawMenuFrame( R12, FRAME_SPACE*screenScaleX, 70*screenScaleY, 10, FRAME_WIDTH*screenScaleX, 392*screenScaleY, CLR_WHITE, 0x00000088, 0 ); // left
    drawMenuFrame( R12, (DEFAULT_SCRN_WD/2 - FRAME_WIDTH/2)*screenScaleX, 70*screenScaleY, 10, FRAME_WIDTH*screenScaleX, 392*screenScaleY, CLR_WHITE, 0x00000088, 0 ); // middle
    drawMenuFrame( R12, (DEFAULT_SCRN_WD - FRAME_WIDTH - FRAME_SPACE)*screenScaleX, 70*screenScaleY, 10, FRAME_WIDTH*screenScaleX, 392*screenScaleY, CLR_WHITE, 0x00000088, 0 ); // right
    drawMenuFrame( R12, ((RESPEC_PILE_X + (RESPEC_PILE_WIDTH / 2)) - (RESPEC_INV_WIDTH*screenScale / (2*screenScaleX)))*screenScaleX, RESPEC_INV_Y*screenScaleY, 10, RESPEC_INV_WIDTH*screenScale, (SPEC_WD + 36)*screenScale, CLR_WHITE, CLR_BLACK, 0 ); // inventory

    BuildCBox(&box[0], FRAME_SPACE*screenScaleX, (71*screenScaleY)+PIX3, FRAME_WIDTH*screenScaleX, (390*screenScaleY)-(2*PIX3) );
    BuildCBox(&box[1], (DEFAULT_SCRN_WD/2 - FRAME_WIDTH/2)*screenScaleX, (71*screenScaleY)+PIX3, FRAME_WIDTH*screenScaleX, (390*screenScaleY)-(2*PIX3) );
    BuildCBox(&box[2], (DEFAULT_SCRN_WD - FRAME_WIDTH - FRAME_SPACE)*screenScaleX, (71*screenScaleY)+PIX3, FRAME_WIDTH*screenScaleX, (390*screenScaleY)-(2*PIX3) );

    if( D_MOUSEHIT == drawStdButton( (FRAME_SPACE+FRAME_WIDTH/2)*screenScaleX, (SL_FRAME_TOP-32)*screenScaleY, 45, 120*screenScaleX, 20*screenScaleY, SELECT_FROM_UISKIN( CLR_BTN_HERO, CLR_BTN_VILLAIN, CLR_BTN_ROGUE ), (s_ShowCombatNumbersHere==1)?"HidePowerNumbers":"ShowCombatNumbersHere", screenScale, 0 ))
        (s_ShowCombatNumbersHere==1)?(s_ShowCombatNumbersHere = 0):(s_ShowCombatNumbersHere = 1);
    if( D_MOUSEHIT == drawStdButton( DEFAULT_SCRN_WD*screenScaleX/2 , (SL_FRAME_TOP-32)*screenScaleY, 45, 120*screenScaleX, 20*screenScaleY, SELECT_FROM_UISKIN( CLR_BTN_HERO, CLR_BTN_VILLAIN, CLR_BTN_ROGUE ), (s_ShowCombatNumbersHere==2)?"HidePowerNumbers":"ShowCombatNumbersHere", screenScale, 0 ))
        (s_ShowCombatNumbersHere==2)?(s_ShowCombatNumbersHere = 0):(s_ShowCombatNumbersHere = 2);
    if( D_MOUSEHIT == drawStdButton( (DEFAULT_SCRN_WD - FRAME_WIDTH/2 - FRAME_SPACE)*screenScaleX, (SL_FRAME_TOP-32)*screenScaleY, 45, 120*screenScaleX, 20*screenScaleY, SELECT_FROM_UISKIN( CLR_BTN_HERO, CLR_BTN_VILLAIN, CLR_BTN_ROGUE ), (s_ShowCombatNumbersHere==3)?"HidePowerNumbers":"ShowCombatNumbersHere", screenScale, 0 ))
        (s_ShowCombatNumbersHere==3)?(s_ShowCombatNumbersHere = 0):(s_ShowCombatNumbersHere = 3);

    if( s_ShowCombatNumbersHere == 1)
        menuDisplayPowerInfo( FRAME_SPACE*screenScaleX, 70*screenScaleY, 30, FRAME_WIDTH*screenScaleX, 392*screenScaleY, g_pPowMouseover->ppowBase, g_pPowMouseover, POWERINFO_BOTH );
    if( s_ShowCombatNumbersHere == 2)
        menuDisplayPowerInfo( (DEFAULT_SCRN_WD/2 - FRAME_WIDTH/2)*screenScaleX, 70*screenScaleY, 30, FRAME_WIDTH*screenScaleX, 392*screenScaleY, g_pPowMouseover->ppowBase, g_pPowMouseover, POWERINFO_BOTH  );
    if( s_ShowCombatNumbersHere == 3)
        menuDisplayPowerInfo( (DEFAULT_SCRN_WD - FRAME_WIDTH - FRAME_SPACE)*screenScaleX, 70*screenScaleY, 30, FRAME_WIDTH*screenScaleX, 392*screenScaleY, g_pPowMouseover->ppowBase, g_pPowMouseover, POWERINFO_BOTH  );

    iSize = eaSize(&respec.pchar->ppPowerSets);
    for( i = 0; i < iSize; i++ )
    {
        int j, top;
        int iSize;
        if (!respec.pchar->ppPowerSets[i]->psetBase->bShowInManage)
            continue;

        if( respec.pchar->ppPowerSets[i]->psetBase->pcatParent == respec.pchar->pclass->pcat[0] && s_ShowCombatNumbersHere!=1)
        {
            clipperPushCBox(&box[0]);
            top = primY;

            setHeadingFontFromSkin(0);
            cprnt( (FRAME_SPACE+FRAME_WIDTH/2)*screenScaleX, (top-23)*screenScaleY, 20, screenScale, screenScale, respec.pchar->ppPowerSets[i]->psetBase->pchDisplayName );

            iSize = eaSize(&respec.pchar->ppPowerSets[i]->ppPowers);
            for( j = 0; j < iSize; j++ )
            {
                Power *pow = respec.pchar->ppPowerSets[i]->ppPowers[j];
                if(pow && pow->ppowBase->bShowInManage)
                {    
                    drawPowerAndSpecializations( respec.pchar, pow, FRAME_SPACE, primY, 20, 1.f, TRUE, 0, 1, 0, screenScaleX, screenScaleY);
                    primY += POWER_SPEC_HT;
                }
            }

            // draw the small inner frame
            drawFrame( PIX2, R6, (FRAME_SPACE + POWER_OFFSET)*screenScaleX, (top - 30)*screenScaleY, 5, (POWER_WIDTH - 10)*screenScaleX, (primY - top - 35)*screenScaleY, screenScale, 0xffffffaa, SELECT_FROM_UISKIN( 0x0000ff44, 0xff000044, 0x00000066 ) );

            primY += POWER_SPEC_HT;
            clipperPop();
        }
        else if( respec.pchar->ppPowerSets[i]->psetBase->pcatParent == respec.pchar->pclass->pcat[1] && s_ShowCombatNumbersHere!=2)
        {
            clipperPushCBox(&box[1]);
            top = secY;

            setHeadingFontFromSkin(0);
            cprnt( DEFAULT_SCRN_WD*screenScaleX/2, (top-23)*screenScaleY, 20, screenScale, screenScale, respec.pchar->ppPowerSets[i]->psetBase->pchDisplayName );

            iSize = eaSize(&respec.pchar->ppPowerSets[i]->ppPowers);
            for( j = 0; j < iSize; j++ )
            {
                Power *pow = respec.pchar->ppPowerSets[i]->ppPowers[j];
                if(pow && pow->ppowBase->bShowInManage)
                {
                    drawPowerAndSpecializations( respec.pchar, pow, DEFAULT_SCRN_WD/2 - FRAME_WIDTH/2, secY, 20, 1.f, FALSE, 0, 1, 0, screenScaleX, screenScaleY);
                    secY += POWER_SPEC_HT;
                }
            }

            // draw the small inner frame
            drawFrame( PIX2, R6, (DEFAULT_SCRN_WD/2 - FRAME_WIDTH/2 + POWER_OFFSET)*screenScaleX, (top - 30)*screenScaleY, 5, (POWER_WIDTH - 10)*screenScaleX, (secY - top - 35)*screenScaleY, screenScale, 0xffffffaa, SELECT_FROM_UISKIN( 0x0000ff44, 0xff000044, 0x00000066 ) );

            secY += POWER_SPEC_HT;
            clipperPop();
        }
        else if(s_ShowCombatNumbersHere!=3)
        {
            clipperPushCBox(&box[2]);
            top = poolY;

            setHeadingFontFromSkin(0);
            cprnt( (DEFAULT_SCRN_WD - FRAME_WIDTH/2 - FRAME_SPACE)*screenScaleX, (top-23)*screenScaleY, 20, screenScale, screenScale, respec.pchar->ppPowerSets[i]->psetBase->pchDisplayName );

            iSize = eaSize(&respec.pchar->ppPowerSets[i]->ppPowers);
            for( j = 0; j < iSize; j++ )
            {
                Power *pow = respec.pchar->ppPowerSets[i]->ppPowers[j];
                if(pow && pow->ppowBase->bShowInManage)
                {
                    drawPowerAndSpecializations( respec.pchar, pow, DEFAULT_SCRN_WD - FRAME_WIDTH - FRAME_SPACE, poolY, 20, 1.f, FALSE, 0, 1, 0, screenScaleX, screenScaleY );
                    poolY += POWER_SPEC_HT;
                }
            }

            // draw the small inner frame
            drawFrame( PIX2, R6, (DEFAULT_SCRN_WD - FRAME_WIDTH - FRAME_SPACE + POWER_OFFSET)*screenScaleX, (top - 30)*screenScaleY, 5, (POWER_WIDTH - 10)*screenScaleX, (poolY - top - 35)*screenScaleY, screenScale, 0xffffffaa, SELECT_FROM_UISKIN( 0x0000ff44, 0xff000044, 0x00000066 ) );

            poolY += POWER_SPEC_HT;
            clipperPop();
        }
    }

    doScrollBarEx( &sb[0], 356, (primY + sb[0].offset - (2 * POWER_SPEC_HT) - 10), (FRAME_WIDTH + FRAME_SPACE), 87, 20, &box[0], 0, screenScaleX, screenScaleY );
    doScrollBarEx( &sb[1], 356, (secY  + sb[1].offset - (2 * POWER_SPEC_HT) - 10), (DEFAULT_SCRN_WD/2 + FRAME_WIDTH/2), 87, 20, &box[1], 0, screenScaleX, screenScaleY );
    doScrollBarEx( &sb[2], 356, (poolY + sb[2].offset - (2 * POWER_SPEC_HT) - 10), (DEFAULT_SCRN_WD - FRAME_SPACE), 87, 20, &box[2], 0, screenScaleX, screenScaleY );

    // the inventory
    s_drawReSpecInventory( ((RESPEC_PILE_X + (RESPEC_PILE_WIDTH / 2)) - (RESPEC_INV_WIDTH*screenScale / (2*screenScaleX))), RESPEC_INV_Y, 20, screenScaleX, screenScaleY );

    // the info box
    info_PowManagement(kPowInfo_Respec, screenScaleX, screenScaleY);

    // the pile
    s_respec_drawpile(screenScaleX, screenScaleY);

    if ( drawNextButton( 40*screenScaleX,  740*screenScaleY, 10, screenScale, 0, 0, 0, "BackString" ))
    {
        respec.placementInit = false;
        respec_clearPowerBoosts();
        respec_destroyPile(); // clear the pile before going back

        // make sure that we are not higher than the max possible level (this should never happen in normal use, but playing it safe)
        respec.pchar->iLevel = min(respec.pchar->iLevel, (MAX_PLAYER_SECURITY_LEVEL - 1));

        respec_slotRewindLevel(true);
        respec.menuFunc = respec_assignment;
        s_ShowCombatNumbersHere = 0;
    }

    if ( drawNextButton( 980*screenScaleX, 740*screenScaleY, 10, screenScale, 1, 0, 0, "RespecFinish" ))
    {
        // dialog message
        if( eaSize(&respec.pile) )
        {
            // display leftover enhancement warning before the final warning
            char * buffer = textStd( "RespecEnhancementWarning", eaSize(&respec.pile), respec_computePileValue());
            // use "_YES_NO" since "_TWO_RESPONSE" draws funny button sizes
            dialogStd( DIALOG_YES_NO, buffer, "RespecWarningContinue", "RespecWarningCancel", respec_finalWarning, NULL, 0 );
        }
        else
        {
            // skip the enhancement warning & go directly to final warning
            respec_finalWarning(0);
        }
    }
    set_scrn_scaling_disabled(screenScalingDisabled);
}

//------------------------------------------------------------------------------------------
// CONTROL MENU ////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------------------
// this will recreate a bastardized version of character creation
//

void respecMenu()
{
    Entity *e = playerPtr();
    int w, h;
    static int init = 0;

    if( !init )
    {
        init = TRUE;
        info_clear();
        addToolTip(&enhanceTip);
        powerInfoSetClass( e->pchar->pclass );
    }

    toggle_3d_game_modes( SHOW_NONE );

    // the moving background
    drawBackground( NULL );
    if(!commConnected())
    {
        respec_ClearData(0);
        start_menu( MENU_GAME );        
        return;
    }

    if( !e || gWaitingVerifyRespec )
        return;

    windowClientSizeThisFrame( &w, &h );

    if(!respec.menuFunc)
        respec.menuFunc = respec_powerLevel;

    // execute current menu
    respec.menuFunc();
    if (!enhanceTip.updated)
        clearToolTip(&enhanceTip);
}

#endif //ifndef TEST_CLIENT

/* End of File */
