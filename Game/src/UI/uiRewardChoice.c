

#include "gameComm/wdwbase.h"
#include "UI/uiWindows.h"
#include "UI/uiUtil.h"
#include "UI/uiUtilGame.h"
#include "UI/uiScrollBar.h"
#include "UI/uiNet.h"
#include "UI/uiInput.h"
#include "UI/uiDialog.h"

#include "formatter/smf_main.h"

#include "UI/sprite/sprite_text.h"
#include "UI/sprite/sprite_base.h"
#include "UI/sprite/sprite_font.h"
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/language/MessageStoreUtil.h>
#include "cmdparse/cmdgame.h"

//------------------------------------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------------------------------------------------

typedef struct RewardChoice
{
    SMFBlock    *sm;
    int            disabled;
    int            visible;
    char        *txt;
}RewardChoice;

static RewardChoice** s_RewardChoices = 0;

static char *s_RewardDesc = 0;

static bool s_RewardDisableChooseNothing = false;

void setRewardDesc( char *desc )
{
    if( s_RewardDesc )
        free( s_RewardDesc );

    s_RewardDesc = strdup(desc);
}

void setRewardDisableChooseNothing( bool flag )
{
    s_RewardDisableChooseNothing = flag;
}

void addRewardChoice(int visible, int disabled, char *txt)
{
    RewardChoice *choice = calloc(1,sizeof(RewardChoice));
    if(visible)
    {
        choice->sm = smfBlock_Create();
        choice->disabled = disabled;
        choice->txt = strdup(textStd(txt));
        choice->visible = 1;
    }
    eaPush(&s_RewardChoices, choice);
}

static void destroyRewardChoice( RewardChoice * choice )
{
    if (choice->sm)
    {
        smfBlock_Destroy(choice->sm);
    }
    if(choice->txt)
        free(choice->txt);
    free(choice);
}

void clearRewardChoices(void)
{
    eaDestroyEx(&s_RewardChoices, destroyRewardChoice);
    if(s_RewardDesc)
        free(s_RewardDesc);
    s_RewardDesc = 0;
}


//------------------------------------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------------------------------------------------

#define BUTTON_HT 70
#define CHOICE_SPACE 10
static int s_CurrentRewardChoice = -1;

int rewardChoiceDisplay( float x, float y, float z, float sc, float wd, int i )
{
    CBox box;
    RewardChoice *choice = s_RewardChoices[i];
      int foreColor;
    int backColor;
       float ht;

    if(!choice->visible || !choice->txt[0])
        return 0;
    
    ht = smf_ParseAndDisplay( choice->sm, choice->txt, x+(2*R10)*sc, y+PIX3*sc, z+5, wd - 4*R10*sc, 0, 0, 1, &gTextAttr_White12, NULL, 0, true);
    ht = ht+2*PIX4*sc;

    if(!choice->disabled)
    {
        BuildCBox(&box, x, y, wd, ht+2*PIX3 );

        if( mouseCollision(&box) )
            drawFlatFrame( PIX3, R10, x+R10*sc, y, z, wd-R10*2*sc, ht, sc, CLR_MOUSEOVER_FOREGROUND, CLR_MOUSEOVER_BACKGROUND );
        if( mouseDownHit(&box, MS_LEFT) )
            s_CurrentRewardChoice = i;
        if( i == s_CurrentRewardChoice )
        {
            foreColor = CLR_SELECTION_FOREGROUND;
            backColor = CLR_SELECTION_BACKGROUND;
        }
        else
        {
            foreColor = CLR_NORMAL_FOREGROUND;
            backColor = CLR_NORMAL_BACKGROUND;
        }
    }
    else
    {
        foreColor = CLR_DISABLED_FOREGROUND;
        backColor = CLR_DISABLED_BACKGROUND;
    }

    drawFlatFrame( PIX3, R10, x+R10*sc, y, z, wd-R10*2*sc, ht, sc, foreColor, backColor );

    return ht+(CHOICE_SPACE)*sc;
}

void chooseNothing(void * data)
{
    cmdParse("clearRewardChoice");
}

int rewardChoiceWindow(void)
{
    int i, count = eaSize(&s_RewardChoices);
    float x, y, ty, starty, z, wd, ht, sc, desc_ht;
    int color, bcolor;
    static ScrollBar sb = {WDW_REWARD_CHOICE,0};
    static SMFBlock *sm = 0;
    static bool init;

    if(!window_getDims(WDW_REWARD_CHOICE, &x, &y, &z, &wd, &ht, &sc, &color, &bcolor) || !count )
    {
        init = false;
        return 0;
    }

     drawFrame(PIX3, R10, x, y, z, wd, ht, sc, color, bcolor);

    gTextAttr_White12.piScale = (int*)((int)(SMF_FONT_SCALE*sc));

    if (!sm)
    {
        sm = smfBlock_Create();
    }

    desc_ht = smf_ParseAndDisplay(sm, textStd(s_RewardDesc), x+R10+PIX3, y+(R10+PIX3)*sc, z, wd-2*(R10+PIX3)*sc, 0, !init, 0, &gTextAttr_White12, NULL, 0, true); 

    if(!init)
    {
        sb.offset = 0;
        init = true;
    }

    starty = y+desc_ht+(R10+PIX3+CHOICE_SPACE*2)*sc;

    set_scissor(true);
     scissor_dims(x+PIX3*sc,starty,wd-2*PIX3*sc,ht-BUTTON_HT*sc-(starty-y));

    ty = starty - sb.offset;

     for( i = 0; i < count; i++ )
    {
        ty += rewardChoiceDisplay( x, ty, z, sc, wd, i );
    }

    if(window_getMode(WDW_REWARD_CHOICE) == WINDOW_DISPLAYING)// && ty - starty > ht-BUTTON_HT*sc-(starty-y) && ht < 500*sc )
    {
        ht = ty - y + BUTTON_HT*sc;
        ht = MIN(ht,500);
        window_setDims(WDW_REWARD_CHOICE, x, y, wd, ht);
    }

    set_scissor(false);
       doScrollBar(&sb, ht-BUTTON_HT*sc-(starty-y), ty + sb.offset - starty, x+wd, starty-y, z+1, 0, 0 );

       if( D_MOUSEHIT == drawStdButton( x+wd/2, y + ht - BUTTON_HT*sc/2, z+1, 250*sc, 40*sc, color, "AcceptReward", 2.f*sc, (s_CurrentRewardChoice<0) ) )
    {
        sendRewardChoice( s_CurrentRewardChoice );
    }

    if(!s_RewardDisableChooseNothing)
    {
         if( D_MOUSEHIT == drawStdButton( x+wd-100*sc, y + ht - BUTTON_HT*sc/2, z+1, 90*sc, 20*sc, CLR_RED, "ChooseNothingString", sc, 0 ) )
        {
             dialogStdWidth( DIALOG_TWO_RESPONSE, "ChooseNothingConfirmString", "IWantSomething", "IWantNothing", 0, chooseNothing, 0, 500*sc);
        }
    }

    return 0;
}
