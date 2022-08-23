

#include "gameComm/wdwbase.h"
#include "UI/uiUtil.h"
#include "UI/uiUtilGame.h"
#include "UI/uiUtilMenu.h"
#include "UI/uiWindows.h"
#include "UI/uiComboBox.h"
#include "uiSlider.h"
#include "UI/uiScrollBar.h"
#include "uiListView.h"
#include "UI/uiClipper.h"
#include "UI/uiTabControl.h"
#include "UI/uiNet.h"
#include "uiHelp.h"

#include "graphics/ttFontUtil.h"
#include "UI/sprite/sprite_base.h"
#include "UI/sprite/sprite_text.h"
#include "UI/sprite/sprite_font.h"
#include "gameData/arenastruct.h"
#include "player/player.h"
#include <utilitieslib/components/earray.h>
#include "graphics/textureatlas.h"
#include "arena/arenagame.h"
#include "cmdparse/cmdgame.h"
#include <utilitieslib/utils/timing.h>
#include "entity/character_base.h"
#include "entity/character_level.h"
#include "UI/uiToolTip.h"
#include <utilitieslib/utils/timing.h>
#include "entity/entity.h"



//------------------------------------------------------------------------------------------------------
// Main Window /////////////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------------------------------
#define MAX_ARMY_COST 2000 // TO DO have server manage this number
int arenaGladiatorPickerWindow(void)
{
    float x, y, z, wd, ht, sc;
    int i, color, bcolor;
    float xoffset = 0;
    F32 yoffset = 0; 
    F32 middleOfBottomButtons;
    static ScrollBar sb = { WDW_ARENA_GLADIATOR_PICKER };

    ArenaPetManagementInfo * info = &g_ArenaPetManagementInfo;
    int availableCnt = eaSize( &info->availablePets );
    int currentCnt = eaSize( &info->currentPets );
    int totalArmyCost = 0;
    int titleBarY, titleBarX = 0;
    int pickerY, pickerX = 0;
    char buf[1000];
    int petsToShowPerRow;
    int petsInThisRow;
    CBox box;
  
    // Do everything common windows are supposed to do. 
    {
        static lastFrameWasOpen; 
        if ( !window_getDims( WDW_ARENA_GLADIATOR_PICKER, &x, &y, &z, &wd, &ht, &sc, &color, &bcolor )) 
        {
            //TO DO this is kinda ugly -- handle the window being closed forcibly outside this function
            if( lastFrameWasOpen )
            {
                if( game_state.inPetsFromArenaList )
                    window_setMode( WDW_ARENA_LIST, WINDOW_GROWING );
                game_state.inPetsFromArenaList = 0;
                game_state.pickPets = PICKPET_NOWINDOW; 
            }
            lastFrameWasOpen = 0;
            return 0;
        }
        lastFrameWasOpen = 1;
    }
   
    drawFrame( PIX3, R10, x, y, z, wd, ht, sc, color, bcolor );       
 
    titleBarX = x+80*sc;      
    titleBarY = y+40*sc; 
    pickerX = 20*sc;    
    pickerY = 70*sc;
    middleOfBottomButtons = (y+ht) - 30*sc;     

 
    //Figure total current cost 
    for( i = 0 ; i < availableCnt ; i++ )            
    {
        int k;
        ArenaPet * pet = info->availablePets[i];
        for( k = 0 ; k < MAX_OF_PET_TYPE ; k++ )   
        {
            if(    pet->showAsInCurrentlySelectedArmy[k] )
                totalArmyCost += pet->cost;
        }    
    }

    set_scissor( TRUE );             
    scissor_dims( x + PIX3*sc, y+(pickerY-10*sc), wd-PIX3*2*sc, ht-110*sc ); 
    BuildCBox( &box, x + PIX3*sc, y+(pickerY-10*sc), wd-PIX3*2*sc, ht-110*sc ); 
    petsToShowPerRow = availableCnt/3;
    petsInThisRow = 0;
    xoffset = 0;
    yoffset = 0;
    //Display everybody

    for( i = 0 ; i < availableCnt ; i++ )             
    {
        int count;
        int j;
        ArenaPet * pet = info->availablePets[i]; 
    
        if( petsInThisRow > petsToShowPerRow )     
        {
            yoffset = 0;
            xoffset += 220*sc;
            petsInThisRow = 0;
        }
        
        //Always be collapsing the buttons  //TO DO I don't know if this is good or not
        count = 0;
        for( j = 0 ; j < MAX_OF_PET_TYPE ; j++ )   
        {
            if(    pet->showAsInCurrentlySelectedArmy[j] )
                count++;
        }
        for( j = 0 ; j < MAX_OF_PET_TYPE ; j++ )  
        {
            pet->showAsInCurrentlySelectedArmy[j] = (count > j) ? 1 : 0;
        }
        //End collapsing


        sprintf( buf, "%s (%d)", pet->name, pet->cost );  
        {
            int selectable = 1; //Only make selectalbe if you can afford him
            if( !pet->showAsInCurrentlySelectedArmy[0] && (totalArmyCost + pet->cost) > MAX_ARMY_COST )
                selectable = 0;
            drawSmallCheckbox( WDW_ARENA_GLADIATOR_PICKER, pickerX + xoffset + MAX_OF_PET_TYPE*20*sc, (pickerY + yoffset)/sc-sb.offset, buf, &pet->showAsInCurrentlySelectedArmy[0], NULL , selectable, NULL, NULL );

            for( j = MAX_OF_PET_TYPE-1 ; j > 0 ; j-- )  
            {
                selectable = 1;
                if( !pet->showAsInCurrentlySelectedArmy[j] && (totalArmyCost + pet->cost) > MAX_ARMY_COST )
                //if( (totalArmyCost + ((MAX_OF_PET_TYPE-j)+1))* pet->cost  > MAX_ARMY_COST )
                    selectable = 0;
                drawSmallCheckbox( WDW_ARENA_GLADIATOR_PICKER, pickerX + xoffset + (MAX_OF_PET_TYPE*20 - j*20)*sc, (pickerY + yoffset)/sc-sb.offset, 0, &pet->showAsInCurrentlySelectedArmy[j], NULL , selectable, NULL, NULL ); 
            }
        }

        petsInThisRow++;
        yoffset+=20*sc;
    }
     
    set_scissor( FALSE );

    if( D_MOUSEHIT == drawStdButton( (x + wd/2.0) - 100*sc, middleOfBottomButtons, z, 100*sc, 30*sc, color, "OkString", 1.0f, 0 ) )
    {
        game_state.pickPets = PICKPET_WAITINGFORACKTOCLOSE;
        arena_CommitPetChanges();
    }
    if( D_MOUSEHIT == drawStdButton( (x + wd/2.0), middleOfBottomButtons, z, 100*sc, 30*sc, color, "ApplyString", 1.0f, 0 ) )
    {
        game_state.pickPets = PICKPET_WINDOWOPEN; //No change
        arena_CommitPetChanges();
    }
    if( D_MOUSEHIT == drawStdButton( (x + wd/2.0) + 100*sc, middleOfBottomButtons, z, 100*sc, 30*sc, color, "CancelString", 1.0f, 0 ) )
    {
        //TO DO if you came from arena list
        window_setMode( WDW_ARENA_GLADIATOR_PICKER, WINDOW_SHRINKING );
        game_state.pickPets = PICKPET_NOWINDOW;
    }

    {
        AtlasTex *round;
        F32 scale; 
        round = atlasLoadTexture("MissionPicker_icon_round.tga");
        scale = ((float)30/round->width)*sc;
        cprntEx( x+wd+15*sc - (30+R10)*sc, y+ht+15*sc - (30+R10)*sc, z+1, scale, scale, CENTER_X|CENTER_Y, "?" );
        if( D_MOUSEHIT == drawGenericButton( round, x+wd - (30+R10)*sc, y+ht - (30+R10)*sc, z, scale, color, color, true ) )
        {
            helpSet(9);
        }
    }

    //TO DO xlate       
    cprntEx(titleBarX, titleBarY, z, 1.5*sc, 1.5*sc, 0, "GladiatorManagerTitle" );  
    cprntEx(titleBarX + 320*sc, titleBarY, z, 1.5*sc, 1.5*sc, 0, "GladiatorPoints", (MAX_ARMY_COST - totalArmyCost) ); 
    
    doScrollBar( &sb, ht - (80*sc), petsToShowPerRow*20*sc+70*sc, wd, (pickerY-10*sc), z, &box, 0 ); 

    return 0;
}


