#include "UI/Hybrid/uiPower.h"
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
#include "UI/Hybrid/uiHybridMenu.h"
#include "UI/Hybrid/uiArchetype.h"
#include "UI/Hybrid/uiOrigin.h"
#include "UI/uiCostume.h"
#include "UI/Hybrid/uiRedirect.h"

#include "entity/classes.h"
#include "entity/powers.h"
#include "entity/costume.h"
#include "entity/character_base.h"
#include "entity/entity.h"
#include "entity/entPlayer.h"

#include "UI/uiPowerInfo.h"
#include "clientcomm/authclient.h"
#include "clientcomm/dbclient.h"
#include "UI/Hybrid/uiWebStoreFrame.h"
#include "UI/uiClipper.h"
#include "UI/uiDialog.h"
#include "account/AccountCatalog.h"

#define NUM_POWERS_SHOWING 9

static HybridElement ** ppPowerSetList;
static HybridElement ** ppPowerList;
static const BasePowerSet * pLastPrimaryPowerSet;
static const BasePowerSet * pLastSecondaryPowerSet;
static const BasePower *pLastPower1;
static const BasePower *pPower1;
static int secondarySelected = 0;
static const BasePowerSet * prev_selected = 0;
static const CharacterClass * prev_class = 0;
static SMFBlock * smfDesc;
static int showDetailedDesc  = 0;
static F32 detailScale = 0.f;
static int lastHoveredPower = -1;
static int sSwitchedSet = 0;
static int sPrimaryPowersetChanged = 0;
static int sCurrentPowerSet = -1;
static const BasePowerSet * g_pPrimaryPowerSet;
static const BasePowerSet * g_pSecondaryPowerSet;
static bool g_pSecondaryPowerSetValid = false;

static HybridElement powerTabs[] = 
{
    { 0, "PrimaryPowerString", 0, "icon_power_primary_0", "icon_power_primary_1", 0, "powertab_tab_primary_0", "powertab_tab_primary_1", "powertab_tab_primary_2"},
    { 1, "SecondaryPowerString", 0, "icon_power_secondary_0", "icon_power_secondary_1", 0, "powertab_tab_secondary_0", "powertab_tab_secondary_1", "powertab_tab_secondary_2" },
};

static PowerSet * updatePowerSet(const BasePowerSet ** prevPowerSet, const BasePowerSet * currPowerSet, int forceReBuy)
{
    Entity * e = playerPtr();

    //check if this powerset was already chosen
    if (*prevPowerSet && (forceReBuy || *prevPowerSet != currPowerSet))
    {
        //remove old powerset and costume pieces
        costumeUnawardPowerSetPartsFromPower(e, *prevPowerSet, 0);
        character_RemovePowerSet(e->pchar, *prevPowerSet, NULL);
    }

    //save previous
    *prevPowerSet = currPowerSet;

    //choose this powerset (returns null if switching to NO powerset)
    return character_BuyPowerSet( e->pchar, currPowerSet );  //buying the same powerset again is OK
}

void resetPowerMenu()
{
    g_pPrimaryPowerSet = 0;
    g_pSecondaryPowerSet = 0;
    updatePowerSet(&pLastPrimaryPowerSet, g_pPrimaryPowerSet, 0);
    updatePowerSet(&pLastSecondaryPowerSet, g_pSecondaryPowerSet, 0);
    devassert(pLastPrimaryPowerSet==0);
    devassert(pLastSecondaryPowerSet==0);

    pPower1 = 0;
    secondarySelected = 0;
    prev_selected = 0;
    prev_class = 0;
    eaClearEx(&ppPowerList, 0);
    if (smfDesc)
    {
        smf_SetRawText(smfDesc, "", 0);
    }
    showDetailedDesc = 0;
    detailScale = 0.f;
    lastHoveredPower = -1;
    sSwitchedSet = 0;
    sPrimaryPowersetChanged = 0;
    eaDestroyEx(&ppPowerSetList, 0);
    sCurrentPowerSet = -1;
}

int powersReady( int redirect)
{
    if( redirect && (!getSelectedOrigin() || !getSelectedCharacterClass()))
    {
        clearRedirects();
        if( !getSelectedOrigin() )
            addRedirect(MENU_ORIGIN, "MustChooseOrigin", BLINK_ORIGIN, powersReady );    
        if( !getSelectedCharacterClass() )
            addRedirect(MENU_ARCHETYPE, "MustChooseClass", BLINK_ARCHETYPE, powersReady );    
    }

    return getSelectedCharacterClass() && getSelectedOrigin();
}


static int primaryChosen(int foo){ return !!g_pPrimaryPowerSet; }
static int secondaryChosen(int foo){ return !!g_pSecondaryPowerSet && !!g_pSecondaryPowerSetValid;}
static int powerChosen(int foo){return !!pPower1;}

int powersChosen(int redirect)
{
     if( redirect && !(g_pPrimaryPowerSet && g_pSecondaryPowerSet && pPower1) )
    {
         clearRedirects();
        if( !g_pPrimaryPowerSet )
            addRedirect(MENU_POWER, "MustChoosePrimary", BLINK_PRIMARYPOWER, primaryChosen );    
        if( !g_pSecondaryPowerSet )
            addRedirect(MENU_POWER, "MustChooseSecondary", BLINK_SECONDARYPOWER, secondaryChosen );    
        if( !pPower1 )
            addRedirect(MENU_POWER, "MustChoosePower", BLINK_POWERSELECTION, powerChosen );    
    }

    return g_pPrimaryPowerSet && g_pSecondaryPowerSet && pPower1;
}

void powerMenuEnterCode()
{
    //character must be created before being able to select powers.
    Entity * e = playerPtr();
    powerInfoSetLevel(e->pchar->iLevel);
    powerInfoSetClass(e->pchar->pclass);
}

void powerMenuExitCode()
{
    Entity *e = playerPtr();
    PowerSet * pset;

    //remove power if we had previously bought it (this should happen if we are switching between 2 powers in a powerset)
    if(pLastPower1 && character_OwnsPower(e->pchar, pLastPower1) && pPower1 != pLastPower1)
    {
        character_RemoveBasePower(e->pchar, pLastPower1);
    }
    //buy primary power
    sPrimaryPowersetChanged |= pLastPrimaryPowerSet != g_pPrimaryPowerSet;
    pset = updatePowerSet(&pLastPrimaryPowerSet, g_pPrimaryPowerSet, 0);
    if (pset && pPower1 && !character_OwnsPower(e->pchar, pPower1))
    {
        character_BuyPower(e->pchar, pset, pPower1, 0);
    }
    pLastPower1 = pPower1;

    //buy secondary power
    //forceReBuy flag is set if the primary set has changed.
    //This is to make sure the order is correct and that the secondary power appears after the first.
    pset = updatePowerSet(&pLastSecondaryPowerSet, g_pSecondaryPowerSet, sPrimaryPowersetChanged);
    if (pset && !character_OwnsPower(e->pchar, g_pSecondaryPowerSet->ppPowers[0]))
    {
        character_BuyPower(e->pchar, pset, g_pSecondaryPowerSet->ppPowers[0], 0); // no choice, buy first power
    }
    sPrimaryPowersetChanged = 0;
    
    //give costume parts dependent on power sets
    costumeAwardPowersetParts(e,1,0);
    updateCostumeWeapons();

    showDetailedDesc = 0; // hide detailed power
    detailScale = 0.f;
}

static void initPowerSetSelector(const CharacterClass *pClass, int isSecondary)
{
    Entity *e = playerPtr();
    int i, count = 0;

    eaDestroyEx(&ppPowerSetList, 0); 
     ppPowerSetList = NULL;
    sCurrentPowerSet = -1;

    // we need to buy the powersets to verify legality
    if (isSecondary)
    {
        //currently switching from primary to secondary, so buy primary
        sPrimaryPowersetChanged |= pLastPrimaryPowerSet != g_pPrimaryPowerSet;
        updatePowerSet(&pLastPrimaryPowerSet, g_pPrimaryPowerSet, 0);
    }
    else
    {
        //currently switching from secondary to primary, so buy secondary
        updatePowerSet(&pLastSecondaryPowerSet, g_pSecondaryPowerSet, 0);
    }

     for( i = 0; i < eaSize(&pClass->pcat[isSecondary]->ppPowerSets); ++i )
    {
        const BasePowerSet *pSet = pClass->pcat[isSecondary]->ppPowerSets[i]; 
        HybridElement *pElem = calloc(1, sizeof(HybridElement));
        pElem->text = pSet->pchDisplayName;
        pElem->desc1 = pSet->pchDisplayHelp;
        pElem->index = i;

        // check for store locks
        if (pSet->pchAccountRequires != NULL)
        {
            pElem->storeLocked = !accountEval(inventoryClient_GetAcctInventorySet(), db_info.loyaltyStatus, 
                db_info.loyaltyPointsEarned, (U32 *) db_info.auth_user_data, pSet->pchAccountRequires);
        }

        if ( pElem->storeLocked && pSet->pchAccountProduct != NULL &&  !accountCatalogIsSkuPublished(skuIdFromString(pSet->pchAccountProduct)) )
        {
            free(pElem);
            continue;
        }

        //check if this set should be grayed (if we can purchase this in the store, it should not be grayed)
        if (pElem->storeLocked || character_IsAllowedToHavePowerSetHypothetically(e->pchar, pSet))
        {
            pElem->pData = cpp_const_cast(void*)(pSet);
        }
        else
        {
            if (baseset_IsSpecialization(e->pchar->pclass->pcat[isSecondary]->ppPowerSets[i]))
            {
                free(pElem);
                continue;
            }
            else
            {
                pElem->pData = NULL;
            }
        }

        hybridElementInit(pElem);
         eaPush(&ppPowerSetList, pElem);

        if( (!isSecondary && g_pPrimaryPowerSet == pSet) || (isSecondary && g_pSecondaryPowerSet == pSet))
        {
            sCurrentPowerSet = count;
        }
        ++count;
    }
}

static void refreshPowerSetSelector()
{
    int i;
    for (i = eaSize(&ppPowerSetList)-1; i >= 0; --i)
    {
        const BasePowerSet * pSet = ppPowerSetList[i]->pData;
        if (pSet && pSet->pchAccountRequires)
        {
            ppPowerSetList[i]->storeLocked = !accountEval(inventoryClient_GetAcctInventorySet(), db_info.loyaltyStatus, 
                                                                        db_info.loyaltyPointsEarned, (U32 *) db_info.auth_user_data, pSet->pchAccountRequires);
        }
    }
}

static void addPowerToList( const BasePower *pow, int i)
{
    HybridElement *pElem = calloc(1, sizeof(HybridElement));
    pElem->text = pow->pchDisplayName;
    pElem->desc1 = pow->pchDisplayHelp;
    pElem->iconName0 = pow->pchIconName;
    pElem->index = i;
    pElem->pData = pow;
    hybridElementInit(pElem);
    eaPush(&ppPowerList, pElem);
}

static int updateAndDrawPowerSets( F32 topX, F32 topY, F32 z, F32 powersetAlpha, bool *isPowerSetGray)
{
    AtlasTex * bmid = atlasLoadTexture("selectiontabround_small_mid_0");
    int onPowerSetSelector = -1;
    static F32 yPowerSetOffset = 0.f;
    int drawUp = 1, drawDn = 1;
    F32 powerWd = 350.f;
    F32 powerHt = 42.f;
    int i;
    F32 midX, topMidY, xposSc, yposSc;
    calculatePositionScales(&xposSc, &yposSc);
    midX = topX + powerWd*xposSc/2.f;
    topMidY = topY + powerHt*yposSc/2.f;

    if (sSwitchedSet)
    {
        int pslSize = eaSize(&ppPowerSetList);
        //need to make sure we can see the selected powerset if we are returning
        if (pslSize > NUM_POWERS_SHOWING && sCurrentPowerSet >= NUM_POWERS_SHOWING-2)
        {
            yPowerSetOffset = -powerHt * (sCurrentPowerSet + 1);
        }
        else
        {
            yPowerSetOffset = 0.f;
        }
    }

    //check if the list is oversized
    if( eaSize(&ppPowerSetList)  >  NUM_POWERS_SHOWING ) // more than 9 default powers, need arrows
    {
        CBox box;
        UIBox uibox;
        AtlasTex * arrow = atlasLoadTexture( "chat_separator_arrow_down.tga" );
        int clr = 0xffffff88;
        F32 sc = 2.f;
        int onArrow = 0;

        //check for mouse scroll
        BuildScaledCBox(&box, topX, topY, powerWd*xposSc, NUM_POWERS_SHOWING * powerHt*yposSc );
        if (mouseCollision(&box))
        {
            yPowerSetOffset -= mouseScrollingScaled();
        }

        //out of bounds checks
        if (yPowerSetOffset >= 0.f)
        {
            yPowerSetOffset = 0.f;
            drawUp = 0;
        }
        else if (yPowerSetOffset <= (NUM_POWERS_SHOWING - eaSize(&ppPowerSetList)) * powerHt)
        {
            yPowerSetOffset = (NUM_POWERS_SHOWING - eaSize(&ppPowerSetList)) * powerHt;
            drawDn = 0;
        }

        //remove arrows if there's no where to go
        if (drawUp)
        {
            BuildScaledCBox( &box, topX, topY, powerWd*xposSc, powerHt*yposSc );
            if( mouseCollision(&box))
            {
                if (yPowerSetOffset < 0.f)
                {
                    yPowerSetOffset += TIMESTEP*10;
                    clr = CLR_WHITE;
                    sc = 2.1;
                    onArrow = 1;
                }
            }
            display_sprite_positional_flip( arrow, midX, topMidY, z, sc, sc, clr, FLIP_VERTICAL, H_ALIGN_CENTER, V_ALIGN_CENTER );
        }

        sc = 2.f;
        if (drawDn)
        {
            clr = 0xffffff88;
            BuildScaledCBox(&box, topX, topY+powerHt*yposSc*(NUM_POWERS_SHOWING-1), powerWd*xposSc, powerHt*yposSc );
            if( mouseCollision(&box))
            {
                if (yPowerSetOffset > (NUM_POWERS_SHOWING - eaSize(&ppPowerSetList)) * powerHt)
                {
                    yPowerSetOffset -= TIMESTEP*10;
                    clr = CLR_WHITE;
                    sc = 2.1;
                    onArrow = 1;
                }
            }
            display_sprite_positional( arrow, midX, topMidY + powerHt*yposSc*8.f, z, sc, sc, clr, H_ALIGN_CENTER, V_ALIGN_CENTER );
        }

        if (onArrow)
        {
            sndPlay("N_Scroll_Loop", SOUND_UI_REPEATING);
        }
        else
        {
            sndStop(SOUND_UI_REPEATING, 0.f);
        }

        uiBoxDefineScaled(&uibox, 0, topY + drawUp*powerHt*yposSc, 5000*xposSc, powerHt*yposSc*(NUM_POWERS_SHOWING-drawUp-drawDn));
        clipperPush(&uibox);
    }


    for( i = 0; i < eaSize(&ppPowerSetList); ++i )
    {
        int flags = HB_ROUND_ENDS_THIN;
        int selected = i == sCurrentPowerSet;
        F32 alpha = 1.f;
        int barRet;

        if (ppPowerSetList[i]->pData && !ppPowerSetList[i]->storeLocked)
        {
            alpha = powersetAlpha;
            if (selected)
                (*isPowerSetGray) = false;
        }
        else
        {
            flags |= HB_GREY; //gray out bars with no data
            if (selected)
                (*isPowerSetGray) = true;
        }

        //fade edges for scrolling only
        if ((eaSize(&ppPowerSetList) > NUM_POWERS_SHOWING) &&
            ((drawUp && (190 + 42 * i + yPowerSetOffset) < (190 + 42*1.5f)) ||
            (drawDn && (190 + 42 * i + yPowerSetOffset) > (190 + 42*6.5f))))
        {
            alpha = 0.25f;
        }

        barRet = drawHybridBar(ppPowerSetList[i], selected, midX, (190 + 42 * i + yPowerSetOffset)*yposSc, z, 450.f, 1.f, flags, alpha, H_ALIGN_LEFT, V_ALIGN_CENTER );
        if( D_MOUSEHIT == barRet )
        {
            sCurrentPowerSet = i;
            (*isPowerSetGray) = !!(flags & HB_GREY);
        }
        if( D_MOUSEOVER == barRet )
        {
            CBox box;
            BuildScaledCBox(&box, topX, (190 + 42 * i - (bmid->height/2 - 4) + yPowerSetOffset)*yposSc, 450*xposSc, (bmid->height-8)*yposSc );
            onPowerSetSelector = i;
            if (ppPowerSetList[i]->pData == NULL)
            {
                setHybridTooltip(&box, "IncompatiblePowersetText");
            }
        }
        //lock check
        if (ppPowerSetList[i]->storeLocked)
        {
            const BasePowerSet * pBPset = ppPowerSetList[i]->pData;

            //draw lock
            drawHybridStoreLockIcon(midX-185.f*xposSc, (190+42*i+yPowerSetOffset)*yposSc, z, 0.6f, CLR_WHITE);

            //draw store icon
            drawHybridStoreMenuButton(pBPset->pchAccountProduct, midX+185.f*xposSc, (190+42*i+yPowerSetOffset)*yposSc, z, 0.6f, CLR_WHITE, HB_STORE_PRODUCT);
        }

    }

    if (eaSize(&ppPowerSetList) > NUM_POWERS_SHOWING)
    {
        clipperPop();
    }

    return onPowerSetSelector;
}

//returns -1 if mouse is not on powers, otherwise returns the power index
static int updateAndDrawPowers(F32 topX, F32 topY, F32 z, F32 powerAlpha, bool isPowerSetGray)
{
    AtlasTex * bmid = atlasLoadTexture("selectiontabround_small_mid_0");
    int onPowerSelector = -1;
    static F32 yPowerOffset = 0.f;
    int drawUp = 1, drawDn = 1;
    F32 powerWd = 350.f;
    F32 powerHt = 42.f;
    int i;
    F32 midX, topMidY, xposSc, yposSc;
    calculatePositionScales(&xposSc, &yposSc);
    midX = topX + powerWd*xposSc/2.f;
    topMidY = topY + powerHt*yposSc/2.f;

    if( sCurrentPowerSet < 0 || prev_selected != ppPowerSetList[sCurrentPowerSet]->pData )
    {
        //clear old power list
        eaDestroyEx(&ppPowerList, 0);
        lastHoveredPower = -1;
        yPowerOffset = 0.f;
        prev_selected = NULL;
    }
    if( sCurrentPowerSet >= 0 && prev_selected == NULL )
    {
        const BasePowerSet *pSet = sCurrentPowerSet >= 0 ? ppPowerSetList[sCurrentPowerSet]->pData : NULL;
        int i;

        //create new power list if there are powers for this set
        if( pSet )
        {
            for( i = 0; i < eaSize(&pSet->ppPowers); i++ )
            {
                addPowerToList(pSet->ppPowers[i], i );
            }
        }

        if (!secondarySelected)
        {
            //set new primary
            g_pPrimaryPowerSet = ppPowerSetList[sCurrentPowerSet]->pData;
            if (!sSwitchedSet)
            {
                pPower1 = 0;  //reset chosen power since the powerset just changed
            }
        }
        else
        {
            //set new secondary
            g_pSecondaryPowerSet = ppPowerSetList[sCurrentPowerSet]->pData;
            g_pSecondaryPowerSetValid = !isPowerSetGray;
        }

        prev_selected = ppPowerSetList[sCurrentPowerSet]->pData;
    }

    //check if the list is oversized
    if( eaSize(&ppPowerList)  >  NUM_POWERS_SHOWING ) // more than 9 default powers, need arrows
    {
        CBox box;
        UIBox uibox;
        AtlasTex * arrow = atlasLoadTexture( "chat_separator_arrow_down.tga" );
        int clr = 0xffffff88;
        F32 sc = 2.f;
        int onArrow = 0;

        //check for mouse scroll
        BuildScaledCBox(&box, topX, topY, powerWd*xposSc, NUM_POWERS_SHOWING * powerHt*yposSc );
        if (mouseCollision(&box))
        {
            yPowerOffset -= mouseScrollingScaled();
        }

        //out of bounds checks
        if (yPowerOffset >= 0.f)
        {
            yPowerOffset = 0.f;
            drawUp = 0;
        }
        else if (yPowerOffset <= (NUM_POWERS_SHOWING - eaSize(&ppPowerList)) * powerHt)
        {
            yPowerOffset = (NUM_POWERS_SHOWING - eaSize(&ppPowerList)) * powerHt;
            drawDn = 0;
        }

        //remove arrows if there's no where to go
        if (drawUp)
        {
            BuildScaledCBox( &box, topX, topY, powerWd*xposSc, powerHt*yposSc );
            if( mouseCollision(&box))
            {
                if (yPowerOffset < 0.f)
                {
                    yPowerOffset += TIMESTEP*10;
                    clr = CLR_WHITE;
                    sc = 2.1;
                    onArrow = 1;
                }
            }
            display_sprite_positional_flip( arrow, midX, topMidY, z, sc, sc, clr, FLIP_VERTICAL, H_ALIGN_CENTER, V_ALIGN_CENTER );
        }

        sc = 2.f;
        if (drawDn)
        {
            clr = 0xffffff88;
            BuildScaledCBox(&box, topX, topY+powerHt*yposSc*(NUM_POWERS_SHOWING-1), powerWd*xposSc, powerHt*yposSc );
            if( mouseCollision(&box))
            {
                if (yPowerOffset > (NUM_POWERS_SHOWING - eaSize(&ppPowerList)) * powerHt)
                {
                    yPowerOffset -= TIMESTEP*10;
                    clr = CLR_WHITE;
                    sc = 2.1;
                    onArrow = 1;
                }
            }
            display_sprite_positional( arrow, midX, topMidY + powerHt*8.f*yposSc, z, sc, sc, clr, H_ALIGN_CENTER, V_ALIGN_CENTER );
        }

        if (onArrow)
        {
            sndPlay("N_Scroll_Loop", SOUND_UI_REPEATING);
        }
        else
        {
            sndStop(SOUND_UI_REPEATING, 0.f);
        }

        uiBoxDefineScaled(&uibox, 0, topY + drawUp*powerHt*yposSc, 5000.f*xposSc, powerHt*yposSc*(NUM_POWERS_SHOWING-drawUp-drawDn));
        clipperPush(&uibox);
    }


    for( i = 0; i < eaSize(&ppPowerList); i++ )
    {
        int flags = HB_ROUND_ENDS_THIN;
        int selected = (pPower1==ppPowerList[i]->pData);
        F32 alpha = 1.f;
        int barRet;
        if( secondarySelected )
        {
            selected = (i == 0);
        }

        if (isPowerSetGray || !character_IsAllowedToHavePowerHypothetically(playerPtr()->pchar, ppPowerList[i]->pData))
        {
            flags |= HB_GREY|HB_UNSELECTABLE;
        }
        else
            alpha = powerAlpha;

        //fade edges for scrolling only
        if ((eaSize(&ppPowerList) > NUM_POWERS_SHOWING) &&
            ((drawUp && (190 + 42 * i + yPowerOffset) < (190 + 42*1.5f)) ||
            (drawDn && (190 + 42 * i + yPowerOffset) > (190 + 42*6.5f))))
        {
            alpha = 0.25f;
        }

        barRet = drawHybridBar(ppPowerList[i], selected, midX, (190 + 42 * i + yPowerOffset)*yposSc, z, 450.f, 1.f, flags, alpha, H_ALIGN_LEFT, V_ALIGN_CENTER );
        if( D_MOUSEHIT == barRet )
        {
            if( !secondarySelected && !(flags & HB_UNSELECTABLE))  // secondary power is already chosen
            {
                pPower1 = ppPowerList[i]->pData;
            }
        }
        if( D_MOUSEOVER == barRet )
        {
            lastHoveredPower = i;
            onPowerSelector = i;
        }
    }

    if (eaSize(&ppPowerList) > NUM_POWERS_SHOWING)
    {
        clipperPop();
    }

    return onPowerSelector;
}

void powerMenu()
{
    int clrback = 0;
    CBox box;
    int alpha;
    int onScrollSelector;
    int onPowerSelector;
    int powerCount;

    F32 primalpha = .5f + (getRedirectBlinkScale(BLINK_PRIMARYPOWER)*.5f);
    F32 secondalpha =  .5f + (getRedirectBlinkScale(BLINK_SECONDARYPOWER)*.5f);
    F32 poweralpha = .5f + (getRedirectBlinkScale(BLINK_POWERSELECTION)*.5f);
    static HybridElement sButtonClose = {0, NULL, NULL, "icon_lt_closewindow_0"};
    static HybridElement sButtonInfo = {0, NULL, "DetailedInfoText", "icon_information_0"};
    F32 xposSc, yposSc, xp, yp, textXSc, textYSc;
    static bool isPowerSetGray = false;

    if(drawHybridCommon(HYBRID_MENU_CREATION))
        return;

    if (!powerTabs[0].icon0)
    {
        //init textures once
        hybridElementInit(&powerTabs[0]);
        hybridElementInit(&powerTabs[1]);
    }
    setScalingModeAndOption(SPRITE_Y_UNIFORM_SCALE, SSO_SCALE_TEXTURE);
    calculatePositionScales(&xposSc, &yposSc);
    calculateSpriteScales(&textXSc, &textYSc);
    xp = DEFAULT_SCRN_WD / 2.f;
    yp = 0.f;
    applyPointToScreenScalingFactorf(&xp, &yp); //use real pixel location
    //detailed description
    menuHybridDisplayPowerInfo(xp-462.f*xposSc, 150.f*yposSc, 10, 500, 525, (eaSize(&ppPowerList) && lastHoveredPower >= 0) ? ppPowerList[lastHoveredPower]->pData : 0, 0, POWERINFO_HYBRID_BASE, detailScale);
    if (showDetailedDesc)
    {
        detailScale = stateScale(detailScale, 1);
        if (D_MOUSEHIT == drawHybridButton(&sButtonClose, xp+18.f*xposSc, 169.f*yposSc, 10.f, 0.6f, CLR_WHITE, HB_DRAW_BACKING))
        {
            showDetailedDesc = 0;
        }
        BuildCBox(&box, 50, 150, 500, 525);
        if( mouseCollision(&box) )
        {
            collisions_off_for_rest_of_frame = true;
        }
    }
    else
    {
        detailScale = stateScale(detailScale, 0);
    }

    //power information  
    xp = DEFAULT_SCRN_WD / 2.f;
    yp = 77.f;
    applyPointToScreenScalingFactorf(&xp, &yp); //use real pixel location

    if (D_MOUSEHIT == drawHybridButton(&sButtonInfo, xp+446.f*xposSc, 105.f*yposSc, 4.f, 1.f, CLR_GREEN, HB_DRAW_BACKING ))
    {
        showDetailedDesc = !showDetailedDesc;
    }

    // primary / secondary selector
    if (drawHybridTab(&powerTabs[0], xp-475.f*xposSc, yp, 4.f, 1.f, 1.f, secondarySelected==0, MIN(primalpha, poweralpha)))
    {
        secondarySelected = 0;
        sSwitchedSet = 1;
        showDetailedDesc = 0; // hide detailed power
    }

    if (drawHybridTab(&powerTabs[1], xp-130.f*xposSc, yp, 4.f, 1.f, 1.f, secondarySelected==1, secondalpha))
    {
        secondarySelected = 1;
        sSwitchedSet = 1;
        showDetailedDesc = 0; // hide detailed power
    }

    //tab text
    {
        int alphafont[2] = 
        {
            0xffffff00 | (int)(50.f  + powerTabs[0].percent*205.f),
            0xffffff00 | (int)(50.f  + powerTabs[1].percent*205.f)
        };
        setSpriteScaleMode(SPRITE_XY_NO_SCALE);    // fonts only really work in SPRITE_XY_SCALE or SPRITE_XY_NO_SCALE

        font( &title_18 );
        font_outl(0);
        font_color(CLR_BLACK&alphafont[0], CLR_BLACK&alphafont[0]);
        cprntEx( xp - 325.f*xposSc+2.f, 110.f*yposSc+2.f, 6.f, 0.9f*textXSc, 0.9f*textYSc, CENTER_X | CENTER_Y, powerTabs[0].text ); //dropshadow
        font_color(CLR_WHITE&alphafont[0], CLR_WHITE&alphafont[0]);
        cprntEx( xp - 325.f*xposSc, 110.f*yposSc, 6.f, 0.9f*textXSc, 0.9f*textYSc, CENTER_X | CENTER_Y, powerTabs[0].text );

        font_color(CLR_BLACK&alphafont[1], CLR_BLACK&alphafont[1]);
        cprntEx( xp + 20.f*xposSc+2.f, 110.f*yposSc+2.f, 6.f, 0.9f*textXSc, 0.9f*textYSc, CENTER_X | CENTER_Y, powerTabs[1].text ); //dropshadow
        font_color(CLR_WHITE&alphafont[1], CLR_WHITE&alphafont[1]);
        cprntEx( xp + 20.f*xposSc, 110.f*yposSc, 6.f, 0.9f*textXSc, 0.9f*textYSc, CENTER_X | CENTER_Y, powerTabs[1].text );

        font_outl(1);
        setSpriteScaleMode(SPRITE_Y_UNIFORM_SCALE);
    }

    xp = DEFAULT_SCRN_WD / 2.f;
    yp = 129.f;
    applyPointToScreenScalingFactorf(&xp, &yp); //use real pixel location
    drawHybridTabFrame(xp - 475.f*xposSc, yp, 4.f, 950.f, 500.f, 0.25f);
    {
        AtlasTex * titletip = atlasLoadTexture("powertab_subtitle_tip_0");
        AtlasTex * titlemid = atlasLoadTexture("powertab_subtitle_mid_0");
        AtlasTex * titleend = atlasLoadTexture("powertab_subtitle_end_0");

        F32 titleWd = (950.f-2*titletip->width-titleend->width)/2.f;
        display_sprite_positional(titletip, xp-=475.f*xposSc, yp, 4.5f, 1.f, 1.f, 0x9999997f, H_ALIGN_LEFT, V_ALIGN_TOP);
        display_sprite_positional(titlemid, xp+=titletip->width*xposSc, yp, 4.5f, titleWd/titlemid->width, 1.f, 0x9999997f, H_ALIGN_LEFT, V_ALIGN_TOP);
        display_sprite_positional(titleend, xp+=titleWd*xposSc, yp, 4.5f, 1.f, 1.f, 0x9999997f, H_ALIGN_LEFT, V_ALIGN_TOP);
        display_sprite_positional(titlemid, xp+=titleend->width*xposSc, yp, 4.5f, titleWd/titlemid->width, 1.f, 0x9999997f, H_ALIGN_LEFT, V_ALIGN_TOP);
        display_sprite_positional_flip(titletip, xp+=titleWd*xposSc, yp, 4.5f, 1.f, 1.f, 0x9999997f, FLIP_HORIZONTAL, H_ALIGN_LEFT, V_ALIGN_TOP);

        font(&hybridbold_18);
        xp = DEFAULT_SCRN_WD / 2.f;
        yp = 152.f;
        applyPointToScreenScalingFactorf(&xp, &yp); //use real pixel location
        setSpriteScaleMode(SPRITE_XY_NO_SCALE);    // fonts only really work in SPRITE_XY_SCALE or SPRITE_XY_NO_SCALE
        font_color(CLR_H_DESC_TEXT_DS, CLR_H_DESC_TEXT_DS);
        cprnt(xp-228.f*xposSc+2.f, yp+2.f, 5.f, textXSc, textYSc, "ChooseYourPowerSetText");  //dropshadow
        cprnt(xp+228.f*xposSc+2.f, yp+2.f, 5.f, textXSc, textYSc, "ChooseYourPowerText");  //dropshadow
        font_color(CLR_WHITE, CLR_WHITE);
        cprnt(xp-228.f*xposSc, yp, 5.f, textXSc, textYSc, "ChooseYourPowerSetText");
        cprnt(xp+228.f*xposSc, yp, 5.f, textXSc, textYSc, "ChooseYourPowerText");
        setSpriteScaleMode(SPRITE_Y_UNIFORM_SCALE);
    }

    if( prev_class != getSelectedCharacterClass() || sSwitchedSet )
    {
        sSwitchedSet = 1; //so we can reset the yPowerSetOffset later
        initPowerSetSelector(getSelectedCharacterClass(), secondarySelected );
    }
    else
    {
        //refresh the powers in case something new was bought
        refreshPowerSetSelector();
    }
    prev_class = getSelectedCharacterClass();

    xp = DEFAULT_SCRN_WD / 2.f;
    yp = 169.f;
    applyPointToScreenScalingFactorf(&xp, &yp); //use real pixel location
    onScrollSelector = updateAndDrawPowerSets( xp - 410.f*xposSc, yp, 5.f, secondarySelected?secondalpha:primalpha, &isPowerSetGray);
    onPowerSelector = updateAndDrawPowers(xp + 61.f*xposSc, yp, 5.f, secondarySelected?1.f:poweralpha, isPowerSetGray);

    //draw bottom description frame and text
    //update based on what powerset/power is being hovered
    drawHybridDescFrame(xp, 550.f*yposSc, 5.f, 920.f, 150.f, HB_DESCFRAME_FADEDOWN, 1.f, 1.f, 0.5f, H_ALIGN_CENTER, V_ALIGN_TOP);
    font(&title_12);
    font_outl(0);
    powerCount = eaSize(&ppPowerList);
    if (!smfDesc)
    {
        smfDesc = smfBlock_Create();
    }
    xp = DEFAULT_SCRN_WD / 2.f;
    yp = 0.f;
    applyPointToScreenScalingFactorf(&xp, &yp); //use real pixel location
    if (onScrollSelector >= 0)
    {
        alpha = ppPowerSetList[onScrollSelector]->percent * 0xff;
        setSpriteScaleMode(SPRITE_Y_UNIFORM_SCALE);
        drawHybridDesc(smfDesc, xp-440.f*xposSc, 590.f*yposSc, 5.f, 1.f, 880.f, 100.f, ppPowerSetList[onScrollSelector]->desc1, 0, alpha);
        setSpriteScaleMode(SPRITE_XY_NO_SCALE);    // fonts only really work in SPRITE_XY_SCALE or SPRITE_XY_NO_SCALE
        font_color(CLR_BLACK&(0xffffff00 | alpha), CLR_BLACK&(0xffffff00 | alpha));
        prnt(xp-440.f*xposSc+2.f, 575.f*yposSc+2.f, 5.f, textXSc, textYSc, ppPowerSetList[onScrollSelector]->text);
        font_color(CLR_WHITE&(0xffffff00 | alpha), CLR_WHITE&(0xffffff00 | alpha));
        prnt(xp-440.f*xposSc, 575.f*yposSc, 5.f, textXSc, textYSc, ppPowerSetList[onScrollSelector]->text);
    }
    else if (onPowerSelector >= 0 && onPowerSelector < powerCount)
    {
        const BasePower * lhpower = ppPowerList[onPowerSelector]->pData;
        alpha = ppPowerList[onPowerSelector]->percent * 0xff;
        setSpriteScaleMode(SPRITE_Y_UNIFORM_SCALE);
        drawHybridDesc(smfDesc, xp-440.f*xposSc, 590.f*yposSc, 5.f, 1.f, 880.f, 100.f, lhpower->pchDisplayHelp, 0, alpha);
        setSpriteScaleMode(SPRITE_XY_NO_SCALE);    // fonts only really work in SPRITE_XY_SCALE or SPRITE_XY_NO_SCALE
        font_color(CLR_BLACK&(0xffffff00 | alpha), CLR_BLACK&(0xffffff00 | alpha));
        prnt(xp-440.f*xposSc+2.f, 575.f*yposSc+2.f, 5.f, textXSc, textYSc, lhpower->pchDisplayName);
        font_color(CLR_WHITE&(0xffffff00 | alpha), CLR_WHITE&(0xffffff00 | alpha));
        prnt(xp-440.f*xposSc, 575.f*yposSc, 5.f, textXSc, textYSc, lhpower->pchDisplayName);
    }
    else if (!secondarySelected && !primaryChosen(0))
    {
        setSpriteScaleMode(SPRITE_Y_UNIFORM_SCALE);
        drawHybridDesc(smfDesc, xp-440.f*xposSc, 590.f*yposSc, 5.f, 1.f, 880.f, 100.f, "PrimarySetDescString", 0, 0xff);
        setSpriteScaleMode(SPRITE_XY_NO_SCALE);    // fonts only really work in SPRITE_XY_SCALE or SPRITE_XY_NO_SCALE
        font_color(CLR_H_DESC_TEXT_DS, CLR_H_DESC_TEXT_DS);
        prnt(xp-440.f*xposSc+2.f, 575.f*yposSc+2.f, 5.f, textXSc, textYSc, "PrimarySetSelectString");
        font_color(CLR_WHITE, CLR_WHITE);
        prnt(xp-440.f*xposSc, 575.f*yposSc, 5.f, textXSc, textYSc, "PrimarySetSelectString");
    }
    else if (secondarySelected && !secondaryChosen(0))
    {
        setSpriteScaleMode(SPRITE_Y_UNIFORM_SCALE);
        drawHybridDesc(smfDesc, xp-440.f*xposSc, 590.f*yposSc, 5.f, 1.f, 880.f, 100.f, "SecondarySetDescString", 0, 0xff);
        setSpriteScaleMode(SPRITE_XY_NO_SCALE);    // fonts only really work in SPRITE_XY_SCALE or SPRITE_XY_NO_SCALE
        font_color(CLR_H_DESC_TEXT_DS, CLR_H_DESC_TEXT_DS);
        prnt(xp-440.f*xposSc+2.f, 575.f*yposSc+2.f, 5.f, textXSc, textYSc, "SecondarySetSelectString");
        font_color(CLR_WHITE, CLR_WHITE);
        prnt(xp-440.f*xposSc, 575.f*yposSc, 5.f, textXSc, textYSc, "SecondarySetSelectString");
    }
    font_outl(1);
    setScalingModeAndOption(SPRITE_XY_SCALE, SSO_SCALE_ALL);
    if (drawHybridNext( DIR_LEFT, 0, "BackString" ))
    {
        showDetailedDesc = 0; // hide detailed power
        detailScale = 0.f;
    }

    sSwitchedSet = 0;

    if (game_state.editnpc)
    {
        //pick powers if not chosen
        const CharacterClass * pClass = getSelectedCharacterClass();

        assert(pClass);
        if (!g_pPrimaryPowerSet)
        {
            assert(eaSize(&pClass->pcat[0]->ppPowerSets));
            g_pPrimaryPowerSet = pClass->pcat[0]->ppPowerSets[0];
        }
        if (!pPower1)
        {
            pPower1 = g_pPrimaryPowerSet->ppPowers[0];
        }
        if (!g_pSecondaryPowerSet)
        {
            assert(eaSize(&pClass->pcat[1]->ppPowerSets));
            g_pSecondaryPowerSet = pClass->pcat[1]->ppPowerSets[0];
        }
        goHybridNext(DIR_RIGHT);
    }
    else
    {
        if (primaryChosen(0) && powerChosen(0))
        {
            if (secondaryChosen(0))
            {
                if (drawHybridNext( DIR_RIGHT, 0, "NextString" ))
                {
                    showDetailedDesc = 0; // hide detailed power
                    detailScale = 0.f;
                }
            }
            else
            {
                //next forces player to the secondary selection
                if (drawHybridNext( DIR_RIGHT_NO_NAV, secondarySelected, "ChooseSecondaryString" ))
                {
                    if (secondarySelected != 1)
                    {
                        secondarySelected = 1;
                        sSwitchedSet = 1;
                    }
                    showDetailedDesc = 0; // hide detailed power
                }
            }
        }
        else
        {
            if (drawHybridNext( DIR_RIGHT_NO_NAV, !secondarySelected, "ChoosePrimaryString" ))
            {
                if (secondarySelected != 0)
                {
                    secondarySelected = 0;
                    sSwitchedSet = 1;
                }
                showDetailedDesc = 0; // hide detailed power
            }
        }

    }
}
