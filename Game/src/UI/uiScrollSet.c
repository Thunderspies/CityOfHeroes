

#include "UI/uiCostume.h"
#include "UI/uiScrollBar.h"

#include <utilitieslib/components/earray.h>
#include "cmdparse/cmdcommon.h"
#include "sound/sound.h"

#include "UI/uiUtil.h"
#include "UI/uiUtilGame.h"
#include "UI/uiUtilMenu.h"

#include "gameComm/wdwbase.h"
#include <utilitieslib/utils/textparser.h>

#include "uiScrollSet.h"
#include "uiScrollSelector.h"

#include "UI/sprite/sprite_font.h"
// Acsii Art to visualize heirarchy
//-----------------------
//    (ScrollSetRegion)
//-----------------------
//   (ScrollSetRegion) - Open
//   <- SSRegionSet ->
//-----------------------
//       SSRegionButton
//     SSRegionButton - Open
//   <-SSElementList->
//   <-SSElementList->
//     SSRegionButton
//  (ScrollSetSubRegion) - Open
//   <- SSRegionSet ->
//       SSRegionButton
//     SSRegionButton - Open
//   <-SSElementList->
//   <-SSElementList->
//     SSRegionButton
//-----------------------
//   (ScrollSetRegion)

//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------


#define LEFT_COLUMN_SQUISH .9f

#define SS_REGION_MIN_HT   (60*LEFT_COLUMN_SQUISH)
#define SS_REGION_MAX_HT   (100*LEFT_COLUMN_SQUISH)
#define SS_REGION_SPACER    20
#define SS_REGION_SPEED  15
#define SS_RSET_SPEED     15

#define SS_SELECTOR_WD  300
#define SS_SELECTOR_Y   (30*LEFT_COLUMN_SQUISH)

#define SS_LIST_HT      (40*LEFT_COLUMN_SQUISH)

#define SS_BUTTON_HT    20
#define SS_BUTTON_SPEED 15

static void regionOpen( ScrollSetRegion *pRegion )
{
    int i;

    if( pRegion->isOpen )
    {
        pRegion->isOpen = 0;
        return;
    }
    
     if( pRegion->pParentSet )
    {
        for(i = eaSize(&pRegion->pParentSet->ppRegion)-1; i>=0; i--)
            pRegion->pParentSet->ppRegion[i]->isOpen = 0;
    }
    else if( pRegion->pParentRegion )
    {
        ScrollSetRegion *pParent = pRegion->pParentRegion;
        SSRegionSet *pRegionSet = pParent->ppRegionSet[pParent->current_RegionSet];

        for(i = eaSize(&pParent->ppSubRegion)-1; i>=0; i--)
            pParent->ppSubRegion[i]->isOpen = 0;

        for(i = eaSize(&pRegionSet->ppButton)-1; i>=0; i--)
            pRegionSet->ppButton[i]->isOpen = 0;
    }

    pRegion->isOpen = 1;
}

static void buttonOpen( ScrollSetRegion *pRegion, SSRegionButton *pButton )
{
    if(pButton->isOpen)
    {
        pRegion->isOpen = 0;
        return;
    }
    else
    {
        SSRegionSet *pRegionSet = pRegion->ppRegionSet[pRegion->current_RegionSet];
        int i;

        for( i = eaSize( &pRegionSet->ppButton )-1; i >= 0; i-- )
            pRegionSet->ppButton[i]->isOpen = 0;

        for(i = eaSize(&pRegion->ppSubRegion)-1; i>=0; i--)
            pRegion->ppSubRegion[i]->isOpen = 0;
        pButton->isOpen = 1;
    }
}

F32 scrollSet_DrawRegionButton( ScrollSet *pSet, ScrollSetRegion *pRegion, SSRegionSet *pRegionSet, SSRegionButton *pButton, F32 x, F32 y, F32 z, F32 wd, F32 sc )
{
    int i,list_count = eaSize(&pButton->ppElementList);
     F32 start_y = y;
     F32 maxHt = SS_LIST_HT*list_count;
    F32 subht = 0;

       if( pButton->isOpen && sc > 0 )
    {
        pButton->ht += TIMESTEP*SS_BUTTON_SPEED;
        if ( pButton->ht > maxHt )
            pButton->ht = maxHt;

        pButton->wd += TIMESTEP*SS_BUTTON_SPEED;
        if( pButton->wd > 300 )
            pButton->wd = 300;
    }
    else
    {
        pButton->ht -= TIMESTEP*SS_BUTTON_SPEED;
        if ( pButton->ht < 0 )
            pButton->ht = 0;

        pButton->wd -= TIMESTEP*SS_BUTTON_SPEED;
        if( pButton->wd < 250 )
            pButton->wd = 250;
    }

       if( pButton->ht*sc > 0 )
          drawFrame( PIX2, R6, x - (pButton->wd+20)*sc/2, y, z-1, (pButton->wd+20)*sc, (pButton->ht + 20*LEFT_COLUMN_SQUISH)*sc, 1.f, CLR_WHITE, 0x00ff0088 );

    if( D_MOUSEHIT == drawMenuBarSquished( x, y, z+2, sc, sc*pButton->wd, pButton->pchName, pButton->isOpen, TRUE, FALSE, FALSE, FALSE, 0, LEFT_COLUMN_SQUISH, CLR_WHITE, &game_12,0 ) )
        buttonOpen( pRegion, pButton );
        
    for( i = 0; i < list_count; i ++ )
    {
        F32 scale = MINMAX( (pButton->ht - subht)/SS_LIST_HT, 0.f, 1.f )*sc;
        y += SS_LIST_HT*sc;
        subht += SS_LIST_HT*sc;
    }

      return SS_LIST_HT + pButton->ht*sc;
}

static float scrollSet_drawRegion( ScrollSet * pSet, ScrollSetRegion * pRegion, F32 x, F32 y, F32 z, F32 sc, F32 wd )
{
    F32 selector_sc;
    F32 startY = y;
     F32 center = x+wd/2;  
    int i, subht;
    int iSize;
    SSRegionSet *pRegionSet;

     if( pRegion->isOpen )
    {
        pRegion->inner_ht += TIMESTEP*SS_RSET_SPEED;
        pRegion->inner_wd += TIMESTEP*SS_RSET_SPEED;
        if ( pRegion->inner_wd > SS_SELECTOR_WD )
            pRegion->inner_wd = SS_SELECTOR_WD;
    }
    else
    {
        if( pRegion->inner_ht> 0 )
            pRegion->inner_ht -= TIMESTEP*SS_RSET_SPEED;
        else
            pRegion->inner_ht= 0;

        if( pRegion->inner_wd > 0 )
            pRegion->inner_wd -= TIMESTEP*SS_RSET_SPEED;
        else
            pRegion->inner_wd = 0;
    }

     // first take care of the set selector
    //-----------------------------------------------------------------------------------------------------------
    if( eaSize(&pRegion->ppRegionSet) > 1 )
    {
        selector_sc = (pRegion->outer_ht-SS_REGION_MIN_HT) / (SS_REGION_MAX_HT-SS_REGION_MIN_HT);
        if( selector_sc < 0 )
            selector_sc = 0;
    
         //scrollSet_DrawRegionSelector( pRegion, center, y - selector_sc*SS_SELECTOR_Y, z+20, selector_sc, (selector_sc*wd) );
    }

    if ( drawCostumeRegionButton( pRegion->pchName, x, y, z+20, pSet->wd, pRegion->outer_ht, sc,  pRegion->isOpen, 0, 1 ) )
        regionOpen( pRegion );


    // now the Buttons
    //-----------------------------------------------------------------------------------------------------
      pRegionSet = pRegion->ppRegionSet[pRegion->current_RegionSet];
    subht = 0;
     y += 20 * LEFT_COLUMN_SQUISH;

    // First Draw the Region Buttons
     iSize = eaSize( &pRegionSet->ppButton );
    for( i = 0; i < iSize; i++ )
    {
        float maxHt, scale;
         int list_count = 1;
        SSRegionButton *pButton = pRegionSet->ppButton[i];

        if( pButton->isOpen )
            list_count += eaSize(&pButton->ppElementList);

        maxHt = (SS_LIST_HT*list_count)-10*LEFT_COLUMN_SQUISH;
        scale = MINMAX( (pRegion->inner_ht - subht)/maxHt, 0.f, 1.f )*sc;
        if(scale)
            subht += scrollSet_DrawRegionButton( pSet, pRegion, pRegionSet, pRegionSet->ppButton[i], center, y + subht, z, wd, scale );
    }

    // Now The SubRegions
    //-----------------------------------------------------------------------------------------------------
     //y += 30 * LEFT_COLUMN_SQUISH;
    iSize = eaSize( &pRegion->ppSubRegion );
    for( i = 0; i < iSize; i++)
    {
        ScrollSetRegion *pSubRegion = pRegion->ppSubRegion[i];
        float maxHt, scale;

        if(pSubRegion->isOpen)
        {
            if( pSubRegion->outer_ht < SS_REGION_MAX_HT )
                pSubRegion->outer_ht += TIMESTEP*SS_REGION_SPEED;
            if( pSubRegion->outer_ht > SS_REGION_MAX_HT )
                pSubRegion->outer_ht = SS_REGION_MAX_HT;
        }
        else
        {
            if( pSubRegion->outer_ht > SS_REGION_MIN_HT*pSet->compressSc )
                pSubRegion->outer_ht -= TIMESTEP*SS_REGION_SPEED;
            if( pSubRegion->outer_ht < SS_REGION_MIN_HT*pSet->compressSc )
                pSubRegion->outer_ht = SS_REGION_MIN_HT*pSet->compressSc;
        }

        maxHt = (pSubRegion->outer_ht + pSubRegion->inner_ht)-10*LEFT_COLUMN_SQUISH;
        scale = MINMAX( (pRegion->inner_ht - subht)/maxHt, 0.f, 1.f )*sc;
        if(scale)
        {
            subht += pSubRegion->outer_ht*scale;
            subht += scrollSet_drawRegion( pSet, pSubRegion, x+25, y + subht, z, scale, wd - 50);
             subht += SS_REGION_SPACER*pSet->compressSc*pSet->compressSc*pSet->compressSc*pSet->compressSc*pSet->compressSc*pSet->compressSc*scale; 
        }
    }

    if ( pRegion->inner_ht > subht  )
        pRegion->inner_ht = subht;

      if( pRegion->inner_ht > 0 )
           drawFrame( PIX2, R6, center - (wd-30)/2, startY - 10, z-5, wd-30, pRegion->inner_ht+20, 1, 0xfffffff88, 0x00000088 );

    return pRegion->inner_ht;
}



void scrollSet_Draw( ScrollSet * pSet, F32 x, F32 y, F32 z, F32 sc )
{
    // state variables for the region tabs
    ScrollSetRegion *rset;
    int region_total = eaSize(&pSet->ppRegion);
    int i;
    F32 start_y = y;

    updateScrollSelectors(1.f, 1.f); // do this first so it can intercept all clicks
    
    // manage the button expanding
    for( i = 0; i < region_total; i++)
    {
        rset = pSet->ppRegion[i];
        if(rset->isOpen)
        {
            if( rset->outer_ht < SS_REGION_MAX_HT )
                rset->outer_ht += TIMESTEP*SS_REGION_SPEED;
            if( rset->outer_ht > SS_REGION_MAX_HT )
                rset->outer_ht = SS_REGION_MAX_HT;
        }
        else
        {
            if( rset->outer_ht > SS_REGION_MIN_HT*pSet->compressSc )
                rset->outer_ht -= TIMESTEP*SS_REGION_SPEED;
            if( rset->outer_ht < SS_REGION_MIN_HT*pSet->compressSc )
                rset->outer_ht = SS_REGION_MIN_HT*pSet->compressSc;
        }

        y += rset->outer_ht;
        y += scrollSet_drawRegion( pSet, rset, x, y, z, sc, pSet->wd );
        y += SS_REGION_SPACER*pSet->compressSc*pSet->compressSc*pSet->compressSc*pSet->compressSc*pSet->compressSc*pSet->compressSc; 
        // shrink this by a lot more because its worthless space
    }

    // compress the regions if needed
    {
        float newcompress = pSet->compressSc * pSet->ht / (y-start_y);
        MINMAX1(newcompress,0.1f,1.f);
        // prevent jitter by keeping the precision low
        // if we get jitter later, raising the constant below should fix it
        if(fabs(newcompress-pSet->compressSc) > 0.01f*LEFT_COLUMN_SQUISH)
            pSet->compressSc = newcompress; 
    }    
}

#include "AutoGen/uiScrollSet_h_ast.h"
#include "AutoGen/uiScrollSet_h_ast.c"

ScrollSet testSet;

void scrollSet_Test( F32 x, F32 y, F32 z, F32 wd, F32 ht, F32 sc )
{
    static int first = 0;
    if(!first)
    {
        int i,j;

        ParserLoadFiles(NULL, "test2.scrollset", NULL, 0, parse_ScrollSet, &testSet, NULL, NULL, NULL);

        // set up back pointers
         for( i = eaSize(&testSet.ppRegion)-1; i>=0; i-- )
        {
            ScrollSetRegion * pRegion = testSet.ppRegion[i];
            pRegion->pParentSet = &testSet;
            for( j = eaSize(&pRegion->ppSubRegion)-1; j>=0; j-- )
            {
                pRegion->ppSubRegion[j]->pParentRegion = pRegion;
            }
        }

    }
    first = 1;
     testSet.ht = 10000;
    scrollSet_Draw(&testSet, x, y, z, sc );
}