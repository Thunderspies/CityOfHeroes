
#include "uiPowers.h"
#include "UI/uiNet.h"
#include "UI/uiGame.h"
#include "UI/uiFx.h"
#include "UI/uiUtil.h"
#include "UI/uiChat.h"
#include "UI/uiInput.h"
#include "UI/uiEditText.h"
#include "UI/uiUtilMenu.h"
#include "UI/uiUtilGame.h"
#include "UI/uiScrollBar.h"
#include "UI/Hybrid/uiHybridMenu.h"

#include "UI/sprite/sprite_base.h"
#include "UI/sprite/sprite_text.h"
#include "UI/sprite/sprite_font.h"
#include "graphics/textureatlas.h"

#include "graphics/font.h"        // for xyprintf
#include "graphics/ttFontUtil.h"
#include "language/langClientUtil.h"

#include "win/win_init.h"    // for windowClientSize
#include "cmdparse/cmdgame.h"
#include "player/inventory_client.h"

#include "entity/character_base.h"
#include "sound/sound.h"
#include "entity/origins.h"
#include "entity/powers.h"
#include <utilitieslib/components/earray.h>
#include "player/player.h"
#include "entity/entity.h"
#include "entity/entPlayer.h"
#include "UI/uiClipper.h"
#include "UI/uiToolTip.h"
#include "UI/uiPowerInfo.h"
#include "entity/costume_client.h" // costume keys
#include <utilitieslib/utils/file.h>
#include "win/input.h"

#include "formatter/smf_main.h"
#include "formatter/smf_format.h"
#include <utilitieslib/language/MessageStoreUtil.h>
#include "clientcomm/dbclient.h"
#include "auth/authUserData.h"
#include "clientcomm/authclient.h"
#include "account/AccountData.h"
#include "account/AccountCatalog.h"

int gCurrentPowerSet[kCategory_Count] = {-1, -1, -1, -1}; // the primary and secondary power set (index available powersets on character)
int gCurrentPower[kCategory_Count]    = {-1, -1, -1, -1}; // the primary and secondary powers ( index into availble powers on character )
static int s_bShowPowerNumbers;


static int category = kCategory_Primary, init = FALSE;

#define FRAME_LEFT 85
#define POWER_FRAME_TOP 80
#define POWER_FRAME_HEIGHT 500
#define POWER_LFRAME_WIDTH 355
#define POWER_SEP_WIDTH 150
#define FRAME_RIGHT 950

static TextAttribs gSmallTextAttr =
{
    /* piBold        */  (int *)0,
    /* piItalic      */  (int *)0,
    /* piColor       */  (int *)CLR_GREEN,
    /* piColor2      */  (int *)0,
    /* piColorHover  */  (int *)CLR_GREEN,
    /* piColorSelect */  (int *)0,
    /* piColorSelectBG*/ (int *)0x333333ff,
    /* piScale       */  (int *)(int)(1.0f*SMF_FONT_SCALE),
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

static ToolTip nagTip;

//---------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------

void powerSetCategory(int iCat)
{
    category = iCat;
}

void power_reset(void)
{
    int i;

    for( i = 0; i < 3; i++ )
    {
        gCurrentPowerSet[i] = -1;
        gCurrentPower[i] = -1;
    }

    category = kCategory_Primary;
    init = FALSE;
}

// the center of the power help bullseye
#define PHELP_Y    645

// Displays the frame for the powers help
//
void displayPowerHelpBackground(float screenScaleX, float screenScaleY)
{
    AtlasTex * ring = atlasLoadTexture( "powers_description_iconring.tga" );
    AtlasTex * back = atlasLoadTexture( "powers_iconring_background.tga" );
    float ringScale;


    ringScale = MIN(screenScaleX, screenScaleY);

    drawMenuFrame( R12, round(FRAME_LEFT * screenScaleX), round(600 * screenScaleY), 10, round(865 * screenScaleX),
        round(90 * screenScaleY), CLR_WHITE, 0x00000088, 0 );
    display_sprite_menu( ring, screenScaleX * FRAME_LEFT - ringScale * ring->width/2, screenScaleY * PHELP_Y - ringScale * ring->height/2,
        15, ringScale, ringScale, CLR_WHITE );
}

static void bottom_box_of_text(const char *icon_name, const char *dot_text, const char *help_text)
{
    AtlasTex * icon;
    static SMFBlock *smf_block;
    int fh;
    int w, h;
    float screenScaleX, screenScaleY;
    float scale;
    int textX, textY, textW, textH;

    windowClientSizeThisFrame( &w, &h );

    screenScaleX = (float)w / DEFAULT_SCRN_WD;
    screenScaleY = (float)h / DEFAULT_SCRN_HT;
    scale = MIN(screenScaleX, screenScaleY);

    if (!smf_block)
        smf_block = smfBlock_Create();

    icon = atlasLoadTexture( icon_name );

    // don't display the icon if its white
    if( stricmp( icon->name, "white" ) != 0 )
    {
        float iconScale = MIN(screenScaleX, screenScaleY);
        iconScale = MIN(iconScale, 1.f);
        iconScale *= 1.5f;

        display_sprite_menu( icon, screenScaleX * FRAME_LEFT - icon->width*iconScale/2, 
            screenScaleY * PHELP_Y - icon->height*iconScale/2, 16, iconScale, iconScale, CLR_WHITE );
    }

    drawTextWithDot( 125 * screenScaleX, 620 * screenScaleY, 20, scale, dot_text);
    smf_SetRawText(smf_block, textStd(help_text), false);
    smf_SetFlags(smf_block, 0, 0, 0, 0, 0, 0, 0, 0, SMAlignment_Left, 0, 0, 0);

    gSmallTextAttr.piColor = (int *)(SELECT_FROM_UISKIN(CLR_GREEN, 0xff8800ff, CLR_TXT_ROGUE));
    gSmallTextAttr.piScale = (int *)(int)(1.05*SMF_FONT_SCALE*scale);

    textX = round(125 * screenScaleX);
    textY = round(620 * screenScaleY);
    textW = round(815 * screenScaleX);
    textH = round(64 * screenScaleY);

    fh = smf_ParseAndFormat(smf_block, textStd(help_text), textX, textY, 20, textW, textH, false, true, true, &gSmallTextAttr, 0);
    if (fh >= (66*screenScaleY))
    {
        // Won't fit, so draw again, but smaller.
        gSmallTextAttr.piScale = (int *)(int)(0.85*SMF_FONT_SCALE*scale);
        fh = smf_ParseAndFormat(smf_block, textStd(help_text), textX, textY, 20, textW, textH, false, true, true, &gSmallTextAttr, 0);
        smf_Display(smf_block, textX, textY, 20, textW, textH, false, true, &gSmallTextAttr, 0);
    } 
    else
    {
        //Fits, draw it.
        smf_Display(smf_block, textX, textY, 20, textW, textH, false, true, &gSmallTextAttr, 0);
    }
}

static int last_pset = -1;  // the last power/set the mouse was over,  these are global so the main function can
static int last_pow = -1;   // clear them when we swtich from primary to secondary
static int lostHelp = TRUE; // if the mouse leaves the bounding box of both power and set, then mousehelp will not persist

// this displays the help string for either a power or power set, it will persist the last value moused over
// (to prevent flicker) until the mouse leave the box bounding either power or set
//
static void displayPowerHelp( Character *pchar, int mouseover, int isSet, int category, int *pSet, int *pPow )
{
    Entity *e = playerPtr();
    UIBox box;
    float ht = 0;
    float screenScaleX, screenScaleY;
    int w, h;

    windowClientSizeThisFrame( &w, &h );

    screenScaleX = (float)w / DEFAULT_SCRN_WD;
    screenScaleY = (float)h / DEFAULT_SCRN_HT;

      uiBoxDefine( &box, 0, 600 * screenScaleY+PIX3, 1024 * screenScaleX, 90 * screenScaleY-2*PIX3);
    clipperPush(&box);

    if ( isSet ) // if the mouse is over the powerset box
    {
        if( mouseover >= 0 ) // if its over a power
        {
            last_pset = mouseover; // set statics
            lostHelp = FALSE;
        }

        if ( last_pset != -1 ) // if the last powerset was valid, display it
        {
            bottom_box_of_text(    pchar->pclass->pcat[category]->ppPowerSets[last_pset]->pchIconName,
                                pchar->pclass->pcat[category]->ppPowerSets[last_pset]->pchDisplayName,
                                pchar->pclass->pcat[category]->ppPowerSets[last_pset]->pchDisplayHelp);
        }
    }
    else // must be called from the power box
    {
        if( mouseover >= 0 ) // if the mouse is over a power
        {
            last_pow = mouseover; // set statics
            lostHelp = FALSE;
        }

        if ( last_pow != -1 ) // if the last power is valid
        {
            bottom_box_of_text(    pchar->pclass->pcat[category]->ppPowerSets[pSet[category]]->ppPowers[last_pow]->pchIconName,
                                pchar->pclass->pcat[category]->ppPowerSets[pSet[category]]->ppPowers[last_pow]->pchDisplayName,
                                pchar->pclass->pcat[category]->ppPowerSets[pSet[category]]->ppPowers[last_pow]->pchDisplayHelp);
        }
    }

    clipperPop(&box);
}

static void power_SetSelected( int *pSet, int *pPow, int val )
{
    int i;
    for( i = 0; i < kCategory_Count; i++ )
    {
        pSet[i] = val;
        pPow[i] = val;
    }

    last_pset = -1;
    last_pow = -1;
}
void powersMenu_resetSelectedPowers()
{
    Entity *e = playerPtr();;
    power_SetSelected( &gCurrentPowerSet[0], &gCurrentPower[0], -1 );
    if(e && e->pchar)
    {
        if (e->pchar->ppPowerSets!=NULL)
        {
            costumeUnawardPowersetParts(e, 0);
            character_Destroy(e->pchar,NULL);
        }
    }
}
static CBox powbox = {590, 110, 350, 460}; // power bounding box
static CBox psetbox = { 85, 110, 350, 460}; // power set bounding box

#define PSET_SPACER        61        // space between power sets
#define PSET_X            135        // the x coordinate
#define PSET_WD            270        // the with of the power set selection bars
#define PSET_FRAME_OFFY    48        // the indentation for the small frame inside the owerset box
#define PSET_FRAME_OFFX    10
#define PSET_FRAME_CLR    SELECT_FROM_UISKIN(0x0000ff44,0xff000044,0x00000066) // the background color for the small inner frame
//
//
char *goingRoguePowerSets[] =
{
    "Mastermind_Summon.Demon_summoning",
    "Blaster_Ranged.Dual_Pistols",
    "Corruptor_Ranged.Dual_Pistols",
    "Defender_Ranged.Dual_Pistols",
    "Controller_Control.Electric_Control",
    "Dominator_Control.Electric_Control",
    "Brute_Melee.Kinetic_Attack",
    "Scrapper_Melee.Kinetic_Attack",
    "Stalker_Melee.Kinetic_Attack",
    "Tanker_Melee.Kinetic_Attack",

    // Loop runs until it hits this terminator.
    NULL
};

// Only used to determine if we should nag when they try to use the powerset,
// so leaving it to nag when the account is a trial.
int unavailableGoingRoguePowerSet( const BasePowerSet *pSet )
{
    int retval = 0;
    int count = 0;

    // Some specific powersets are Going Rogue-only. They check the authbit
    // for GR via per-power Requires clauses. It's easier to check the
    // powerset names than to mess with the individual powers.
    // Not checking store products because that gets checked earlier.
    if (!authUserHasRogueAccess((U32 *)db_info.auth_user_data))
    {
        while (!retval && goingRoguePowerSets[count])
        {
            if (stricmp(pSet->pchFullName, goingRoguePowerSets[count]) == 0)
                retval = 1;

            count++;
        }
    }

    return retval;
}

int powerSetSelector(int y, int category, Character *pchar, int *pSet, int *pPow, float screenScaleX, float screenScaleY, int *offset, int menu)
{
    int i, displayedHelp = FALSE, size, set_count = 0, oldY = y;
    float screenScale;
    CBox clipBox;
    int z = 50;

    screenScale = MIN(screenScaleX, screenScaleY);

    addToolTip( &nagTip );
    clearToolTip( &nagTip );

    // the number of powersets
     size = set_count = eaSize( &pchar->pclass->pcat[category]->ppPowerSets );

    {
        int titleX = round((PSET_X + PSET_WD/2) * screenScaleX);
        int titleY = round(75 * screenScaleY);
        float titleScale = 1.3 * screenScale;

        // set the font
        setHeadingFontFromSkin(0);

        // display title
        if( category == kCategory_Primary )
            cprnt(titleX, titleY, 29, titleScale, titleScale, "ChoosePrimarySet" );
        else if ( category == kCategory_Secondary )
            cprnt(titleX, titleY, 29, titleScale, titleScale, "ChooseSecondarySet" );
        else if ( category == kCategory_Pool )
            cprnt(titleX, titleY, 29, titleScale, titleScale, "ChoosePoolPowerSet" );
        else
            cprnt(titleX, titleY, 29, titleScale, titleScale, "ChooseEpicSet" );
    }

    BuildCBox(&clipBox, FRAME_LEFT * screenScaleX, POWER_FRAME_TOP * screenScaleY + PIX4, POWER_LFRAME_WIDTH * screenScaleX,
        POWER_FRAME_HEIGHT * screenScaleY - PIX4*2 + 1);
    clipperPushCBox(&clipBox);

    // loop through avaialable powers
    for( i = 0; i < size; i++ )
    {
        int mouse;
        int allowedToHavePowerSet = true;
        int passesAccountRequires = true;
        const BasePowerSet *psetBase;
        char *outputMessage = NULL;

        if (category != kCategory_Primary && category != kCategory_Secondary && character_OwnsPowerSet(pchar, pchar->pclass->pcat[category]->ppPowerSets[i]))
        {
            set_count--;
            continue;
        }

        psetBase = pchar->pclass->pcat[category]->ppPowerSets[i];

        // HYBRID : must pass account requires if it exists
        if (psetBase->pchAccountRequires != NULL)
        {
            passesAccountRequires = accountEval(inventoryClient_GetAcctInventorySet(), db_info.loyaltyStatus, db_info.loyaltyPointsEarned, (U32 *) db_info.auth_user_data, 
                psetBase->pchAccountRequires);
        }

        if (!passesAccountRequires && psetBase->pchAccountProduct && !accountCatalogIsSkuPublished(skuIdFromString(psetBase->pchAccountProduct)))
        {
            set_count--;
            continue;
        }

        if (!character_IsAllowedToHavePowerSetHypotheticallyEx(pchar, psetBase, &outputMessage))
        {
            if (baseset_IsSpecialization(psetBase) || category == kCategory_Epic)
            {
                set_count--;
                continue;
            }
            else
            {
                allowedToHavePowerSet = false;
            }
        }

        mouse = drawCheckBarEx( PSET_X * screenScaleX, y * screenScaleY, z, screenScale, PSET_WD * screenScaleX, psetBase->pchDisplayName,
                                 i == pSet[category], allowedToHavePowerSet, !allowedToHavePowerSet || !passesAccountRequires, 0, 0, NULL );
        if( mouse )
        {
            // We nag the player if the powerset is disabled because they
            // don't own Going Rogue and the mouse is over it. We must make
            // sure not to double-show the dialog, or show the tooltip under
            // the dialog.
            if (!allowedToHavePowerSet && okToShowGoingRogueNag(GRNAG_POWERSET)
                && unavailableGoingRoguePowerSet(psetBase))
            {
                if( mouseLeftClick() )
                {
                    dialogGoingRogueNag(GRNAG_POWERSET);
                }
                else
                {
                    CBox box;

                    // Tooltips scale themselves, so we don't need to do it
                    // here.
                    BuildCBox( &box, PSET_X - 20, y - 16, PSET_WD + 20, 32 );
                    setToolTipEx( &nagTip, &box, textStd("BuyGoingRogueTooltip"), NULL, menu, 0, 10, 0 );
                }
            }
            else if (!passesAccountRequires && psetBase->pchAccountTooltip != NULL)
            {
                CBox box;

                // Tooltips scale themselves, so we don't need to do it here.
                BuildCBox( &box, PSET_X - 20, y - 16, PSET_WD + 20, 32 );
                setToolTipEx( &nagTip, &box, textStd(psetBase->pchAccountTooltip), NULL, menu, 0, 10, 0 );
            }
            else if (!allowedToHavePowerSet && outputMessage)
            {
                CBox box;

                // Tooltips scale themselves, so we don't need to do it here.
                BuildCBox( &box, PSET_X - 20, y - 16, PSET_WD + 20, 32 );
                setToolTipEx( &nagTip, &box, textStd(outputMessage), NULL, menu, 0, 10, 0 );
            }
            
            if (allowedToHavePowerSet)
            {
                // display help if the mouse went over
                displayPowerHelp( pchar, i, TRUE, category, pSet, pPow );
                displayedHelp = TRUE;
                if( mouse == D_MOUSEHIT && pSet[category] != i  )
                {
                    if (passesAccountRequires)
                    {
                        BuildCBox(&clipBox, FRAME_LEFT, POWER_FRAME_TOP + PIX4, POWER_LFRAME_WIDTH,
                            POWER_FRAME_HEIGHT - PIX4*2 + 1);
                        clipperPushCBox(&clipBox);
                        electric_clearAll();
                        electric_init( PSET_X, y+ (offset? *offset : 0), z, MIN(1.f, screenScale), 0, 30, 1, offset );
                        clipperPop();
                        sndPlay( "powerselect2", SOUND_GAME );
                    }
                    pPow[category] = -1;
                    pSet[category] = i;
                    g_pBasePowMouseover = 0;
                }
            }
        }

        if (!passesAccountRequires)
        {
            //draw lock
            drawHybridStoreLockIcon(PSET_X * screenScaleX, y * screenScaleY, z + 100, 0.6f, CLR_WHITE);

            //draw store icon
            drawHybridStoreMenuButton(psetBase->pchAccountProduct, (PSET_X + PSET_WD) * screenScaleX - 15, y * screenScaleY, z + 100, 0.6f, CLR_WHITE, HB_STORE_PRODUCT);
        }

        y += PSET_SPACER;
    }

    // if we made it here without displaying a power, check to see if the mouse is in
    // one of the bounding boxes, otherwise don't display help and mark it
    if(!displayedHelp && mouseCollision( &psetbox ) && !lostHelp )
        displayPowerHelp( pchar, -1, TRUE, category, pSet, pPow );
    else if ( !displayedHelp && !mouseCollision( &powbox ) )
        lostHelp = TRUE;

    y += PSET_SPACER;

    // draw the small inner frame
    drawFrame( PIX2, R6, PSET_X * screenScaleX, (oldY - PSET_FRAME_OFFY) * screenScaleY, 40, 
        (PSET_WD - PSET_FRAME_OFFX) * screenScaleX, ((set_count-1) * PSET_SPACER + 2*PSET_FRAME_OFFY) * screenScaleY, 1.f,
        0xffffffaa, PSET_FRAME_CLR );

    clipperPop();

    return y-oldY;
}

#define POW_X            636 // the x location for the power frame
#define POW_SPACER        61  // the sacing between powers
#define POW_WD            270 // the width of a power selection bar
#define POW_FRAME_OFFY  30  // the offset for the inner frame
//
//
int powerSelector( int category, int y, Character * pchar, int *pSet, int *pPow, float screenScaleX, float screenScaleY, int *offset )
{
    Entity *e = playerPtr();
    int i, sy = y, size, displayedText = FALSE;
    int availableCount = 0;
    int availableIndex = -1;
    float screenScale;
    CBox clipBox;

    screenScale = MIN(screenScaleX, screenScaleY);

    {
        int titleX = round((POW_X + POW_WD/2) * screenScaleX);
        int titleY = round(75 * screenScaleY);
        float titleScale = 1.3 * screenScale;

        setHeadingFontFromSkin(0);

        if ( category == kCategory_Primary )
            cprnt(titleX, titleY, 29, titleScale, titleScale, "ChoosePrimaryPower" );
        else if ( category == kCategory_Secondary )
            cprnt(titleX, titleY, 29, titleScale, titleScale, "ChooseSecondaryPower" );
        else if ( category == kCategory_Pool )
            cprnt(titleX, titleY, 29, titleScale, titleScale, "ChoosePoolPower" );
        else
            cprnt(titleX, titleY, 29, titleScale, titleScale, "ChooseEpicPower" );
    }

    BuildCBox(&clipBox, 0, POWER_FRAME_TOP * screenScaleY  + PIX4, 1024 * screenScaleX,
        POWER_FRAME_HEIGHT * screenScaleY - PIX4*2 + 1);
    clipperPushCBox(&clipBox);

    // if we havent selected a power set, do do anything yet
    if( pSet[category] >=  0 )
    {
        size = eaSize( &pchar->pclass->pcat[category]->ppPowerSets[pSet[category]]->ppPowers );
        drawFrame( PIX2, R6, POW_X * screenScaleX, (y - POW_FRAME_OFFY) * screenScaleY, 40,
            (PSET_WD - PSET_FRAME_OFFX) * screenScaleX, ((size-1) * POW_SPACER + 2*POW_FRAME_OFFY) * screenScaleY,
            1.f, 0xffffffaa, PSET_FRAME_CLR );
    }
    else
        size = 0;

    // loop though powers
    for( i = 0; i < size; i++ )
    {
        int mouse;
        const BasePowerSet *pset = pchar->pclass->pcat[category]->ppPowerSets[pSet[category]];
        const BasePower *ppow = pset->ppPowers[i];

        if( i == pPow[category] )
        {
            AtlasTex *icon = atlasLoadTexture(ppow->pchIconName);

            if( stricmp( icon->name, "white" ) == 0 )
                icon = atlasLoadTexture( "Power_Missing.tga" );
            mouse = drawCheckBar(POW_X * screenScaleX, y*screenScaleY, 50, screenScale, POW_WD * screenScaleX, ppow->pchDisplayName,
                                 1, 1, 1, 0, icon);
            availableCount++;
            availableIndex = i;
        }
        else
        {
            // check availability
            int available = character_IsAllowedToHavePowerHypothetically(pchar, ppow);
            if (pset->pchAccountRequires)
                available &= accountEval(inventoryClient_GetAcctInventorySet(), db_info.loyaltyStatus, db_info.loyaltyPointsEarned, (U32 *) db_info.auth_user_data, pset->pchAccountRequires);

            mouse = drawCheckBar(POW_X * screenScaleX, y*screenScaleY, 50, screenScale, POW_WD * screenScaleX, ppow->pchDisplayName, 0,
                                 available, 1, 0, NULL );
            if (available)
            {
                availableCount++;
                availableIndex = i;
            }
        }

        if(mouse)
        {
            displayPowerHelp( pchar, i, FALSE, category, pSet, pPow );
            displayedText = TRUE;
             g_pBasePowMouseover = ppow;

            if( mouse == D_MOUSEHIT )
            {
                BuildCBox(&clipBox, 0, POWER_FRAME_TOP + PIX4, 1024,
                    POWER_FRAME_HEIGHT - PIX4*2 + 1);
                clipperPushCBox(&clipBox);
                electric_clearAll();
                electric_init( POW_X, y+ (offset? *offset : 0), 50, MIN(1.f, screenScale), 0, 30, 1, offset );
                clipperPop();
                sndPlay( "powerselect2", SOUND_GAME );
                pPow[category] = i;
            }
        }

        y += POW_SPACER;
    }

    //    Minor QoL feature to auto select the one if only 1 is available
    if ((availableCount == 1) && (availableIndex >= 0))
    {
        pPow[category] = availableIndex;
    }

    clipperPop();

    // take care of help
    if( !displayedText && mouseCollision( &powbox ) )
        displayPowerHelp( pchar, -1, FALSE, category, pSet, pPow );
    else if( !displayedText && !mouseCollision( &psetbox ) )
        lostHelp = TRUE;

    return (y - sy);

}

// specialized function to draw the crazy powers frame
//
// l - left
// s - separator
// r - right
//
void drawMenuPowerFrame( float lx,  float ly,  float lwd, float lht, float swd, float sht,
                         float rwd, float rht, float frontZ, float backZ, 
                         float screenScaleX, float screenScaleY, int color, int back_color )
{
    int size = 4;
    int radius = 12;
    float w;

    // pipe pieces
    AtlasTex *ul        = atlasLoadTexture( "frame_blue_4px_12r_UL.tga" );
    AtlasTex *ll        = atlasLoadTexture( "frame_blue_4px_12r_LL.tga" );
    AtlasTex *lr        = atlasLoadTexture( "frame_blue_4px_12r_LR.tga" );
    AtlasTex *ur        = atlasLoadTexture( "frame_blue_4px_12r_UR.tga" );
    AtlasTex *ypipe    = atlasLoadTexture( "frame_blue_4px_vert.tga" );
    AtlasTex *xpipe    = atlasLoadTexture( "frame_blue_4px_horiz.tga" );

    // backgrounds
    AtlasTex *bul    = atlasLoadTexture( "frame_blue_background_UL.tga" );
    AtlasTex *bll    = atlasLoadTexture( "frame_blue_background_LL.tga" );
    AtlasTex *blr    = atlasLoadTexture( "frame_blue_background_LR.tga" );
    AtlasTex *bur    = atlasLoadTexture( "frame_blue_background_UR.tga" );
    AtlasTex * back  = white_tex_atlas;

    // elbows
    AtlasTex *eul    = atlasLoadTexture( "frame_blue_background_elbow_UL.tga" );
    AtlasTex *ell    = atlasLoadTexture( "frame_blue_background_elbow_LL.tga" );
    AtlasTex *elr    = atlasLoadTexture( "frame_blue_background_elbow_LR.tga" );
    AtlasTex *eur    = atlasLoadTexture( "frame_blue_background_elbow_UR.tga" );

    float lFrameLeft, lFrameTop, lFrameRight, lFrameBottom;
    float sepTop, sepBottom;
    float rFrameLeft, rFrameTop, rFrameRight, rFrameBottom;

    lFrameLeft = round(lx * screenScaleX);
    lFrameTop = round(ly * screenScaleY);
    lFrameRight = round((lx + lwd) * screenScaleX);
    lFrameBottom = round((ly + lht) * screenScaleY);
    sepTop = round((ly + lht/2 - sht/2) * screenScaleY);
    sepBottom = round((ly + lht/2 + sht/2) * screenScaleY);
    rFrameLeft = round((lx + lwd + swd) * screenScaleX);
    rFrameTop = round(ly + lht/2 - rht/2) * screenScaleY;
    rFrameRight = round((lx + lwd + swd + rwd) * screenScaleX);
    rFrameBottom = round((ly + lht/2 + rht/2) * screenScaleY);

    // LEFT WINDOW
    //-------------------------------------------------------------------------------------
    w = ul->width; // min size
    if ( lFrameRight < lFrameLeft + w*2 )
        lFrameRight = lFrameLeft + w*2;
    if ( lFrameBottom < lFrameTop + w*2 )
        lFrameBottom = lFrameTop + w*2;

    if( sepBottom - sepTop > lFrameBottom - lFrameTop + w*2 ) // get rid of separator if there is not room for it
        sepBottom = sepTop;

    if( sepBottom - sepTop < 2*w*MIN(1.f, MIN(screenScaleX,screenScaleY)) )
        sepBottom = sepTop;

    // top left
    display_sprite_menu( ul,  lFrameLeft, lFrameTop, frontZ, 1.0f, 1.0f, color      );
    display_sprite_menu( bul, lFrameLeft , lFrameTop , backZ, 1.0f, 1.0f, back_color );

    // top
    display_sprite_menu( xpipe, lFrameLeft + w, lFrameTop, frontZ, (lFrameRight - lFrameLeft - 2*w)/xpipe->width, 1.f, color );
    display_sprite_menu( back , lFrameLeft + w, lFrameTop + size, backZ, (lFrameRight - lFrameLeft - 2*w)/back->width, (w - size)/back->height, back_color );

    // top right
    display_sprite_menu( ur,  lFrameRight - w, lFrameTop, frontZ, 1.0f, 1.0f, color      );
    display_sprite_menu( bur, lFrameRight - w, lFrameTop ,  backZ, 1.0f, 1.0f, back_color );

    // lower right
    display_sprite_menu( lr,  lFrameRight - w, lFrameBottom - w, frontZ, 1.0f, 1.0f, color      );
    display_sprite_menu( blr, lFrameRight - w, lFrameBottom - w,  backZ, 1.0f, 1.0f, back_color );

    // bottom
    display_sprite_menu( xpipe, lFrameLeft + w, lFrameBottom - size, frontZ, (lFrameRight - lFrameLeft - 2*w)/xpipe->width, 1.f, color );
    display_sprite_menu( back,  lFrameLeft + w, lFrameBottom - w, backZ, (lFrameRight - lFrameLeft - 2*w)/back->width, (w - size)/back->height, back_color );

    // lower left
    display_sprite_menu( ll,  lFrameLeft, lFrameBottom - w, frontZ, 1.0f, 1.0f, color );
    display_sprite_menu( bll, lFrameLeft, lFrameBottom - w,   backZ, 1.0f, 1.0f, back_color );

    // left
    display_sprite_menu( ypipe, lFrameLeft, lFrameTop + w, frontZ, 1.0f, (lFrameBottom - lFrameTop - 2*w)/ypipe->height, color );
    display_sprite_menu( back,  lFrameLeft + size, lFrameTop + w, backZ, (w - size)/back->width, (lFrameBottom - lFrameTop - 2*w)/back->height, back_color );

    // middle
    display_sprite_menu(back, lFrameLeft + w, lFrameTop + w, backZ, (lFrameRight - lFrameLeft - 2*w)/back->width, (lFrameBottom - lFrameTop - 2*w)/back->height, back_color );

    // right
    if( sepBottom == sepTop ) // no separator
    {   // flat bar
        display_sprite_menu( ypipe, lFrameRight - size, lFrameTop + w, frontZ, 1.0f, (lFrameBottom - lFrameTop - 2*w)/ypipe->height, color );
        display_sprite_menu( back,  lFrameRight - w, lFrameTop + w, backZ, (w-size)/back->width, (lFrameBottom - lFrameTop - 2*w)/back->height, back_color );
    }
    else
    {    // top elbow
        display_sprite_menu( ll, lFrameRight - size, sepTop + size - w, frontZ, 1.0f, 1.0f, color      );
        display_sprite_menu( ell,lFrameRight - size, sepTop + size - w, backZ, 1.0f, 1.0f, back_color );

        // top pipe
        display_sprite_menu( ypipe, lFrameRight - size, lFrameTop + w, frontZ, 1.0f, (sepTop + size - w - (lFrameTop + w))/ypipe->height, color );

        // bottom pipe
        display_sprite_menu( ypipe, lFrameRight - size, sepBottom + w - size, frontZ, 1.0f, (lFrameBottom - w - (sepBottom + w - size))/ypipe->height, color );

        // bottom elbow
        display_sprite_menu( ul, lFrameRight - size, sepBottom - size, frontZ, 1.0f, 1.0f, color      );
        display_sprite_menu( eul,lFrameRight - size, sepBottom - size, backZ, 1.0f, 1.0f, back_color );

        // right background
        display_sprite_menu( back, lFrameRight - w, lFrameTop + w, backZ, (w - size)/back->width, (lFrameBottom - lFrameTop - 2*w)/back->height, back_color );
    }

    // SEPARATOR
    //-------------------------------------------------------------------------------------
    if( sepBottom == sepTop )
        return;

    if( rht < sht + 4*w - 2*size )
        rht = sht;

    swd = swd - 2*(w - size);

    if( swd < 2*w )
        swd = 2*w;

    if ( sht == rht ) // the second pane is not expanded at all so just draw a tabby finger
    {
        // top pipe
        display_sprite_menu( xpipe, lFrameRight - size + w, sepTop, frontZ, (rFrameRight - w - (lFrameRight - size + w))/xpipe->width, 1.0f, color );

        // bottom pipe
        display_sprite_menu( xpipe, lFrameRight - size + w, sepBottom - size, frontZ, (rFrameRight - w - (lFrameRight - size + w))/xpipe->width, 1.0f, color );

        // top cap
        display_sprite_menu( ur,  rFrameRight - w, sepTop, frontZ, 1.f, 1.f, color );
        display_sprite_menu( bur, rFrameRight - w, sepTop, backZ,   1.f, 1.f, back_color );

        // bottom cap
        display_sprite_menu( lr,  rFrameRight - w, sepBottom - w, frontZ, 1.f, 1.f, color );
        display_sprite_menu( blr, rFrameRight - w, sepBottom - w, backZ,   1.f, 1.f, back_color );

        // cap y-pipe
        display_sprite_menu( ypipe, rFrameRight - size, sepTop + w, frontZ, 1.f, (sepBottom - w - (sepTop + w))/ypipe->height, color );
        display_sprite_menu( back,  rFrameRight - w,    sepTop + w, frontZ, (w - size)/back->width, (sepBottom - w - (sepTop + w))/back->height, back_color );

        // background
        display_sprite_menu( back, lFrameRight - size, sepTop + size, backZ, (rFrameRight - w - (lFrameRight - size))/back->width, (sepBottom - sepTop - 2 * size)/back->height, back_color );
        return;
    }
    else
    {
        // top pipe
        display_sprite_menu( xpipe, lFrameRight - size + w, sepTop, frontZ, (rFrameLeft + size - w - (lFrameRight - size + w))/xpipe->width, 1.0f, color );

        // bottom pipe
        display_sprite_menu( xpipe, lFrameRight - size + w, sepBottom - size, frontZ, (rFrameLeft + size - w - (lFrameRight - size + w))/xpipe->width, 1.0f, color );

        // background
        display_sprite_menu( back, lFrameRight - size, sepTop + size, backZ, (rFrameLeft + size - (lFrameRight - size))/back->width, (sepBottom - size - (sepTop + size))/back->height, back_color );
    }

    // RIGHT WINDOW
    //-------------------------------------------------------------------------------------

    // top elbow
    display_sprite_menu( lr,  rFrameLeft + size - w, sepTop + size - w, frontZ, 1.f, 1.f, color );
    display_sprite_menu( elr, rFrameLeft + size - w, sepTop + size - w, backZ,   1.f, 1.f, back_color );

    // bottom elbow
    display_sprite_menu( ur,  rFrameLeft + size - w, sepBottom - size, frontZ, 1.f, 1.f, color );
    display_sprite_menu( eur, rFrameLeft + size - w, sepBottom - size, backZ,   1.f, 1.f, back_color );

    // top left
    display_sprite_menu( ul, rFrameLeft, rFrameTop, frontZ, 1.f, 1.f, color );
    display_sprite_menu( bul, rFrameLeft, rFrameTop, backZ,  1.f, 1.f, back_color );

    // top left pipe
    display_sprite_menu( ypipe, rFrameLeft, rFrameTop + w, frontZ, 1.f, (sepTop + size - w - (rFrameTop + w))/ypipe->height, color );

    // bottom left
    display_sprite_menu( ll,  rFrameLeft, rFrameBottom - w, frontZ, 1.f, 1.f, color );
    display_sprite_menu( bll, rFrameLeft, rFrameBottom - w, backZ,   1.f, 1.f, back_color );

    // bottom left pipe
    display_sprite_menu( ypipe, rFrameLeft, sepBottom - size + w, frontZ, 1.f, (rFrameBottom - w - (sepBottom - size + w))/ypipe->height, color );

    // left background
    display_sprite_menu( back, rFrameLeft + size, rFrameTop + w, backZ, (w - size)/back->width,(rFrameBottom - rFrameTop - 2*w)/back->height, back_color );

    // top
    display_sprite_menu( xpipe, rFrameLeft + w, rFrameTop, frontZ, (rFrameRight - rFrameLeft - 2*w)/xpipe->width, 1.f, color );

    // bottom
    display_sprite_menu( xpipe, rFrameLeft + w, rFrameBottom - size, frontZ, (rFrameRight - rFrameLeft - 2*w)/xpipe->width, 1.f, color );

    // mid background
    display_sprite_menu( back, rFrameLeft + w, rFrameTop + size, backZ, (rFrameRight - rFrameLeft - 2*w)/back->width, (rFrameBottom - rFrameTop - 2*size)/back->height, back_color );

    // top right
    display_sprite_menu( ur,  rFrameRight - w, rFrameTop, frontZ, 1.f, 1.f, color );
    display_sprite_menu( bur, rFrameRight - w, rFrameTop, backZ,   1.f, 1.f, back_color );

    // bottom right
    display_sprite_menu( lr,  rFrameRight - w, rFrameBottom - w, frontZ, 1.f, 1.f, color );
    display_sprite_menu( blr, rFrameRight - w, rFrameBottom - w, backZ,   1.f, 1.f, back_color );

    // right pipe
    display_sprite_menu( ypipe, rFrameRight - size, rFrameTop + w, frontZ, 1.f, (rFrameBottom - rFrameTop - 2*w)/ypipe->height, color );
    display_sprite_menu( back,  rFrameRight - w,    rFrameTop + w, backZ, (w-size)/back->width, (rFrameBottom - rFrameTop - 2*w)/back->height, back_color );
}

//----------------------------------------------------------------------------------------------------------
// Master Control Power Function ///////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------
static void powerMenuExitCode(void)
{            
    last_pset = -1;
    last_pow = -1;
    init = 0;
    s_bShowPowerNumbers = 0;
    g_pBasePowMouseover = 0;
}
void levelPowerPoolEpicMenu(void)
{
    Entity *e = playerPtr();
    static int docht;
    static float timer = 0;
    static ScrollBar sb[2] = {0};
    CBox box;
    AtlasTex * layover = NULL;
    int w, h, ht;
    float screenScaleX, screenScaleY;
    int screenScalingDisabled = is_scrn_scaling_disabled();
    float scale;

    calc_scrn_scaling(&screenScaleX, &screenScaleY);
    scale = MIN(screenScaleX, screenScaleY);
    // display debugging coords
    windowClientSizeThisFrame( &w, &h );
    drawBackground( layover );

    set_scrn_scaling_disabled(1);

    // title
    if (e->db_id)
    {
        if ( category == kCategory_Pool )
        {
            drawMenuTitle( 10 * screenScaleX, 30 * screenScaleY, 20, "ChoosePoolTitle", scale, !init );
            // HACK: Pretend that we're already a level higher to get the right options
            e->pchar->iLevel++;
        }
        else
        {
            drawMenuTitle( 10 * screenScaleX, 30 * screenScaleY, 20, "ChooseEpicTitle", scale, !init );
            // HACK: Pretend that we're already a level higher to get the right options
            e->pchar->iLevel++;
        }
    }
    if( !init )
    {
        init = TRUE;
        powerInfoSetLevel(e->pchar->iLevel);
        powerInfoSetClass(e->pchar->pclass);
    }

    // New Power Information
     if( D_MOUSEHIT == drawStdButton( 512 * screenScaleX, 330 * screenScaleY, 21, 120 * screenScaleX, 30 * screenScaleY,
                                    SELECT_FROM_UISKIN( CLR_BTN_HERO, CLR_BTN_VILLAIN, CLR_BTN_ROGUE ),
                                    s_bShowPowerNumbers?"HidePowerNumbers":"ShowCombatNumbers", scale,
                                    gCurrentPowerSet[category]<0 ))
    {
        s_bShowPowerNumbers = !s_bShowPowerNumbers;
    }

    // do both power selectors
    if( s_bShowPowerNumbers )
    {
        setHeadingFontFromSkin(0);

        // display title
        if( category == kCategory_Primary )
            cprnt( PSET_X * screenScaleX + PSET_WD/2, 75 * screenScaleY, 29, 1.3f * scale, 1.3f * scale, "PowerNumbersDisplay" );

        menuDisplayPowerInfo( FRAME_LEFT * screenScaleX, 80 * screenScaleY, 70, 355 * screenScaleX, 500 * screenScaleY, g_pBasePowMouseover, 0, POWERINFO_BASE );
    }
    else
    {
        int old_powerset_0 = gCurrentPowerSet[0];
        int old_powerset_1 = gCurrentPowerSet[1];
        ht = powerSetSelector(151 - sb[0].offset, category, e->pchar, &gCurrentPowerSet[0], &gCurrentPower[0], screenScaleX, screenScaleY, &(sb[0].offset), MENU_LEVEL_POOL_EPIC);
        BuildCBox(&box, FRAME_LEFT * screenScaleX, 78 * screenScaleY, 355 * screenScaleX, 500 * screenScaleY);
         doScrollBarEx( &sb[0], POWER_FRAME_HEIGHT - 48, ht, 
            FRAME_LEFT + 4*scale/screenScaleX, POWER_FRAME_TOP + 24, 70, &box, 0, screenScaleX, screenScaleY );
        if (old_powerset_0 != gCurrentPowerSet[0])
        {
            if (old_powerset_0 >= 0)
            {
                character_RemovePowerSet(e->pchar, e->pchar->pclass->pcat[0]->ppPowerSets[old_powerset_0], NULL);
            }
            if (gCurrentPower[0] >= 0)
            {
                character_BuyPowerSet( e->pchar, e->pchar->pclass->pcat[0]->ppPowerSets[gCurrentPowerSet[0]] );
            }
        }
        if (old_powerset_1 != gCurrentPowerSet[1])
        {
            if (old_powerset_1 >= 0)
            {
                character_RemovePowerSet(e->pchar, e->pchar->pclass->pcat[1]->ppPowerSets[old_powerset_1], NULL);
            }
            if (gCurrentPower[1] >= 0)
            {
                character_BuyPowerSet( e->pchar, e->pchar->pclass->pcat[1]->ppPowerSets[gCurrentPowerSet[1]] );
            }
        }
    }

    BuildCBox(&box, (85+355+150) * screenScaleX, 80 * screenScaleY, 370 * screenScaleX, 500 * screenScaleY);
    ht = powerSelector( category, 133 - sb[1].offset, e->pchar, &gCurrentPowerSet[0], &gCurrentPower[0], screenScaleX, screenScaleY, &sb[1].offset );
    doScrollBarEx( &sb[1], POWER_FRAME_HEIGHT  - 48, ht, FRAME_RIGHT, 
        POWER_FRAME_TOP + 24, 70, &box, 0, screenScaleX, screenScaleY );

    // help background
    displayPowerHelpBackground(screenScaleX, screenScaleY);

    // the crazy powers frame
    drawMenuPowerFrame( FRAME_LEFT, POWER_FRAME_TOP, 355,
        POWER_FRAME_HEIGHT, 150, 80, 
        (FRAME_RIGHT - POWER_LFRAME_WIDTH - POWER_SEP_WIDTH - FRAME_LEFT), 
        POWER_FRAME_HEIGHT, 60, 10, screenScaleX, screenScaleY, CLR_WHITE, 0x00000044 );

     if ( drawNextButton( 40 * screenScaleX,  740 * screenScaleY, 10, scale, 0, 0, 0, "BackString" ) )
    {
        electric_clearAll();
        sndPlay( "back", SOUND_GAME );

        last_pset = -1;
        last_pow = -1;
        init = 0;
        s_bShowPowerNumbers = 0;
        g_pBasePowMouseover = 0;
        start_menu( MENU_LEVEL_POWER );// go back to leveling screen
    }

    if ( drawNextButton( 980 * screenScaleX, 740 * screenScaleY, 10, scale, 1, gCurrentPower[category] >= 0 ? FALSE: TRUE, 0, (category == kCategory_Pool || category == kCategory_Epic) ?"FinishString":"NextString" )
        || game_state.editnpc || game_state.e3screenshot || (gCurrentPower[category] >= 0 && inpEdgeNoTextEntry(INP_RETURN)) )
    {
        electric_clearAll();
        sndPlay( "next", SOUND_GAME );
        
        if ( game_state.editnpc || game_state.e3screenshot )
        {
            power_SetSelected( &gCurrentPowerSet[0], &gCurrentPower[0], 0 );
        }

        if( gCurrentPower[category] >= 0 || e->access_level > 0 )
        {
            powerMenuExitCode();
            start_menu( MENU_LEVEL_POWER );
        }
    }

    drawHelpOverlay();

    // HACK: stop pretending
    e->pchar->iLevel--;

    set_scrn_scaling_disabled(screenScalingDisabled);
}
