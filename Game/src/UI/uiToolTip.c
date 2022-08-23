

#include "UI/uiToolTip.h"
#include "UI/uiUtil.h"
#include "UI/uiUtilGame.h"
#include "UI/uiInput.h"
#include "UI/uiEditText.h"
#include "UI/uiGame.h"
#include "UI/uiWindows.h"
#include "UI/uiOptions.h"

#include "graphics/ttFontUtil.h"
#include "UI/sprite/sprite_font.h"
#include "UI/sprite/sprite_text.h"
#include "UI/sprite/sprite_base.h"

#include "entity/entVarUpdate.h"
#include "win/win_init.h"
#include <utilitieslib/components/earray.h>
#include "win/input.h"
#include <utilitieslib/utils/mathutil.h>
#include "language/langClientUtil.h"
#include "cmdparse/cmdcommon.h"
#include "cmdparse/cmdgame.h"

#include "formatter/smf_main.h"
#include "uiSMFView.h"
#include "UI/uiClipper.h"
#include <utilitieslib/language/MessageStoreUtil.h>
#include "UI/uiClipper.h"
#include "UI/uiBox.h"
#include "UI/uiMissionMaker.h"

extern int do_scissor;

//----------------------------------------------------------------------------------------------------

enum
{
    TIP_INACTIVE,    // non-displaying
    TIP_ACTIVE,        // displaying
};



int gToolTipMode = TIP_INACTIVE;

static ToolTip **tiplist = 0;
static int currentTT = 0;

static int sRepressed = 0;



static ToolTip* ToolTip_Create(void)
{
    ToolTip *tt = calloc(sizeof(ToolTip),1);
    tt->allocated_self = TRUE;
    return tt;
}

static void ToolTip_AllocSmf(ToolTip *t)
{
    if(!t)
        return;
    t->smf = smfBlock_Create();
}

static void ToolTip_Destroy(ToolTip *t)
{
    if(!t)
        return;

    if( t->smf )
        smfBlock_Destroy(t->smf);
    if( t->sourcetxt )
        free( t->sourcetxt );
    if( t->txt )
        free( t->txt );

    if(t->allocated_self)
        free(t);
    else
        ZeroStruct(t);
}

ToolTip * addWindowToolTip( float x, float y, float wd, float ht, const char* txt, ToolTipParent *parent, 
                       int menu, int window, int timer )
{
    CBox box;
    ToolTip *tip = 0;
    int i;

    // create array if nessecary
    if( !tiplist )
        eaCreate( &tiplist );

    // don't add the tip if already exists
    for( i = eaSize(&tiplist)-1; i >= 0; i-- )
    {
        if( tiplist[i]->sourcetxt && stricmp( txt, tiplist[i]->sourcetxt ) == 0 )
            tip = tiplist[i];
    }

    // make a tip to add
    if( !tip )
    {
        tip = ToolTip_Create();
        tip->smf = smfBlock_Create();
        eaPush( &tiplist, tip );
    }
    
    BuildCBox( &box, x, y, wd, ht );
    tip->rel_bounds.hx = box.hx;
    tip->rel_bounds.hy = box.hy;
    tip->rel_bounds.lx = box.lx;
    tip->rel_bounds.ly = box.ly;
    tip->constant = true;
    
    setToolTipEx( tip, &box, txt, parent, menu, window, timer, 0 );

    return tip;
}

ToolTip * addToolTipEx( float x, float y, float wd, float ht, const char* txt, ToolTipParent *parent, int menu, int window, int timer )
{
    CBox box;
    ToolTip *tip = 0;
    int i;

    // create array if nessecary
    if( !tiplist )
        eaCreate( &tiplist );

    // don't add the tip if already exists
    for( i = eaSize(&tiplist)-1; i >= 0; i-- )
    {
        if( tiplist[i]->sourcetxt && stricmp( txt, tiplist[i]->sourcetxt ) == 0 )
            tip = tiplist[i];
    }

    // make a tip to add
    if( !tip )
    {
        tip = ToolTip_Create();
        tip->smf = smfBlock_Create();
        eaPush( &tiplist, tip );
    }

    BuildCBox( &box, x, y, wd, ht );
    setToolTipEx( tip, &box, txt, parent, menu, window, timer, 0 );

    return tip;
}
//
//
void setToolTip( ToolTip * tip, CBox * box, const char * txt, ToolTipParent *parent, int menu, int window )
{
    setToolTipEx( tip, box, txt, parent, menu, window, 0, 0 );
}

void setToolTipEx( ToolTip * tip, CBox * box, const char * txt, ToolTipParent *parent, int menu, int window, int timer, int flags)
{

    Clipper2D* clipper = clipperGetCurrent();
    UIBox uiBox, *clipBox = clipperGetBox(clipper); 
    
    // don't add cliped tooltip boxes
     uiBoxFromCBox(&uiBox, box);
    if( !clipper || !clipBox || uiBoxIntersects(&uiBox, clipBox) )
    {
        // copy the bounds in
        if( clipBox )
        {
            tip->bounds.hx = MIN(box->hx, clipBox->x+clipBox->width);
             tip->bounds.hy = MIN(box->hy, clipBox->y+clipBox->height);
            tip->bounds.lx = MAX(box->lx, clipBox->x);
            tip->bounds.ly = MAX(box->ly, clipBox->y);
        }
        else
        {
            tip->bounds.hx = box->hx;
            tip->bounds.hy = box->hy;
            tip->bounds.lx = box->lx;
            tip->bounds.ly = box->ly;
        }
    }
    else
        BuildCBox( &tip->bounds, 0, 0, 0, 0 );

    // if the text already exists and has changed, free the pointer
    if ( tip->sourcetxt && stricmp( tip->sourcetxt, txt ) != 0 )
    {
        free( tip->sourcetxt );
        tip->sourcetxt = 0;
        free( tip->txt );
        tip->txt = 0;
    }

    // if there is no tip text (either first time or from freeing), translate and malloc it
    if ( !tip->txt && txt )
    {
        if (flags & TT_NOTRANSLATE)
        {
            tip->txt = strdup(textStd("DontTranslate",txt));
        }
        else
        {        
            tip->txt = strdup(textStd(txt));
        }
        tip->sourcetxt = strdup(txt);
        tip->reparse = true;
    }

    if( !tip->smf )
        tip->smf = smfBlock_Create();

    tip->menu    = menu;
    tip->window = window;
    tip->parent = parent;
    tip->timer    = timer;
    tip->flags  = flags;
}

//
//
void clearToolTip( ToolTip *tip )
{
    // tips with zeroed bounds will be skipped
    BuildCBox( &tip->bounds, 0, 0, 0, 0 );
}

//
//
void addToolTip( ToolTip * tip )
{
    // create array if necessary
    if( !tiplist )
        eaCreate( &tiplist );

    // look to see if this tip is already in the list
    if( eaFind( &tiplist, tip) >= 0 )
        return;

    // didn't find it so add it
    eaPush( &tiplist, tip );

    // entire tip list persists so there is no freeing...yet
}

//
//
void removeToolTip( ToolTip * tip )
{
    int idx = -1;

    if (!tiplist)
        return;

    // look to see if this tip is already in the list
    if( (idx = eaFind( &tiplist, tip)) >= 0 )
    {
        eaRemove(&tiplist, idx);
    }
}

/**********************************************************************func*
 * Function freeToolTip(ToolTip *tip)
 *  Frees the passed tooltip if it is currently being managed by the
 *  tooltip system.
 */
void freeToolTip( ToolTip *tip )
{
    int idx;
    ToolTip * deleteme;

    if( !tiplist )
        return;

    idx = eaFind(&tiplist, tip);

    if (idx >= 0)
    {
        if (currentTT > idx)
        {
            currentTT--;
        }
        deleteme = eaRemove(&tiplist, idx);
        if( deleteme )
            ToolTip_Destroy(deleteme);
    }
}

void clearToolTipEx( const char * txt )
{
    int i;
    for( i = eaSize(&tiplist)-1; i >= 0; i-- )
    {
        if( tiplist[i]->sourcetxt && stricmp( txt, tiplist[i]->sourcetxt ) == 0 )
            freeToolTip( tiplist[i] );
    }
}


//#define TIP_SPEED    5                        // tip move/grow speed
#define TIP_SPACER    4                        // spacer to keep text off of frame
#define TIP_Z        1000*(MAX_WINDOW_COUNT+MAX_CUSTOM_WINDOW_COUNT+1)    // tips draw on top of everything?
#define MAX_TIP_WD    400                        // made it up
//
//
static void displayToolTip( ToolTip *tip, int mode )
{
    float x, y, wd, ht;
    int color, screen_wd, screen_ht;
    float screen_xscale, screen_yscale;
    F32 xOffset, yOffset;
    UIBox uibox;
    int *oldScale;
    TTDrawContext *oldFont;
    int oldColorTop, oldColorBottom;
    int saveScreenScalingDisabled = is_scrn_scaling_disabled();
    
    if(tip->updated || mode == TIP_INACTIVE) // hide inactive tips immediately
        return;

    tip->updated = true;

    oldColorTop = font_color_get_top();
    oldColorBottom = font_color_get_bottom();
    oldFont = font_get();
     font( &game_12 );
    font_color( CLR_WHITE, CLR_WHITE );
    oldScale = gTextAttr_White12.piScale;
    gTextAttr_White12.piScale = (int*)((int)(SMF_FONT_SCALE*1.f));

    if( tip->constant && tip->window ) // update the actual bounds
    {
        float wx, wy;
        if( !window_getDims(tip->window, &wx, &wy, NULL, NULL, NULL, NULL, NULL, NULL) )
            return;

        tip->bounds.lx = wx + tip->rel_bounds.lx;
        tip->bounds.hx = wx + tip->rel_bounds.hx;
        tip->bounds.ly = wy + tip->rel_bounds.ly;
        tip->bounds.hy = wy + tip->rel_bounds.hy;
    }

    // was the tip disabled?
    if(tip->bounds.lx == 0 && tip->bounds.hx == 0 && tip->bounds.ly == 0 && tip->bounds.hy == 0)
        return;

    // is there text?
    if(!tip->txt || !tip->txt[0])
        return;

    if (tip->disableScreenScaling)
        set_scrn_scaling_disabled(1);

    if( tip->constant_wd )
        wd = tip->constant_wd;
    else
        wd = str_wd_notranslate(&game_12,1.f,1.f,tip->txt) + TIP_SPACER*2;

    // does the text have width? do we need wrapping?
    if(!wd)
    {
        set_scrn_scaling_disabled(saveScreenScalingDisabled);
        return;
    }

    if (wd > MAX_TIP_WD)
        wd = MAX_TIP_WD;

    ht = smf_ParseAndFormat( tip->smf, tip->txt, 0, 0, TIP_Z+1, wd-TIP_SPACER*2, 0, false, tip->reparse, tip->reparse, &gTextAttr_White12, 0 );
    tip->reparse = false;

      x = gMouseInpCur.x;
    reversePointToScreenScalingFactorf(&x, NULL);
    y = tip->bounds.ly - ht/2 + PIX2;
    
    // does the tip go off the screen?
       if(y - ht/2 < 0)
//        if(ht > 100) // I'm not sure why this check is here... removing it for now
            y = (tip->bounds.hy + ht/2);

     if(wd > (tip->bounds.hx - tip->bounds.lx))    // tip is wider than bounds, so center it
        x = (tip->bounds.hx + tip->bounds.lx)/2;
    else if(x > tip->bounds.hx - wd/2)            // tip is past the right side, push it back
        x = tip->bounds.hx - wd/2;
    else if(x < tip->bounds.lx + wd/2)            // tip is past the left side, push it back
        x = tip->bounds.lx + wd/2;

    calc_scrn_scaling_with_offset(&screen_xscale, &screen_yscale, &xOffset, &yOffset);
    if(!tip->disableScreenScaling && !isMenu(MENU_GAME))
    {
        screen_wd = DEFAULT_SCRN_WD;
        screen_ht = DEFAULT_SCRN_HT;
    }
    else
    {
        windowClientSizeThisFrame(&screen_wd, &screen_ht);
        screen_wd -= 2*xOffset;
        screen_ht -= 2*yOffset;
    }

    // keep in screen bounds
    if(x - wd/2 - TIP_SPACER < 0)
        x = wd/2 + TIP_SPACER;
    else if(x + wd/2 + TIP_SPACER > screen_wd)
        x = screen_wd - wd/2 - TIP_SPACER;

    if(y - ht/2 - TIP_SPACER < 0)
        y = ht/2 + TIP_SPACER;
    else if(y + ht/2 + TIP_SPACER > screen_ht)
        y = screen_ht - ht/2 - TIP_SPACER;

    // determine frame color
    if(shell_menu())
    {
        color = SELECT_FROM_UISKIN( CLR_BLUE, CLR_RED, CLR_NAVY );
    }
    else
        color = CLR_WHITE;

    clipperPush(NULL);
    // draw the sucker!
    if( tip->flags & TT_FIXEDPOS )
    {
        drawFrame( PIX2, R4, tip->x, tip->y, TIP_Z, wd, ht, 1.f, color, tip->back_color?tip->back_color:0x000000dd );
        uiBoxDefine(&uibox,tip->x+PIX2,tip->y+PIX2, wd-2*PIX2,ht-2*PIX2);
        clipperPush(&uibox);
        smf_ParseAndDisplay( tip->smf, tip->txt, tip->x + TIP_SPACER, tip->y, TIP_Z+1, wd-TIP_SPACER*2, 0, 0, 0, 
                                &gTextAttr_White12, NULL, 0, true);
    }
    else
    {
        drawFrame( PIX2, R4, x - wd/2 - PIX2, y - ht/2 - PIX2, TIP_Z, wd + 2*PIX2, ht + 3*PIX2, 1.f, color, tip->back_color?tip->back_color:0x000000dd );
        uiBoxDefine(&uibox,x-wd/2,y-ht/2,wd,ht);
        clipperPush(&uibox);
        smf_ParseAndDisplay( tip->smf, tip->txt, ceil(x - wd/2 + TIP_SPACER), ceil(y - ht/2), TIP_Z+1, wd-TIP_SPACER*2, 0, 0, 0, 
                                &gTextAttr_White12, NULL, 0, true);
    }
    clipperPop();
    clipperPop();

    gTextAttr_White12.piScale = oldScale;
    font_color(oldColorTop, oldColorBottom);
    font(oldFont);
    set_scrn_scaling_disabled(saveScreenScalingDisabled);
}

void enableToolTips(int enable)
{
    gToolTipMode = (enable ? TIP_ACTIVE : TIP_INACTIVE);
}

//
//
void updateToolTips()
{
    static float timer = 0;
    int tipcount = eaSize(&tiplist);

       int i, hit = FALSE, highest_z;

       if( !tiplist )
        return;

    if (currentTT > tipcount - 1)
    {
        currentTT = 0;
    }

    displayToolTip( tiplist[currentTT], sRepressed ? 0 : gToolTipMode );
    
    sRepressed = false;

    // for every tooltip
      for( i = highest_z = eaSize(&tiplist)-1; i >= 0; i-- )
    {
           tiplist[i]->updated = false;

         //drawBox( &tiplist[i]->bounds, 50000, CLR_GREEN, 0 );
          if( !optionGet(kUO_UseToolTips) )
        {
            continue;
        }

        // ignore flagged
        if (tiplist[i]->flags & TT_HIDE)
        {
            continue;
        }

        // don't bother with tool tips in closed windows or other menus
         if( !isMenu(tiplist[i]->menu) ||
           ( tiplist[i]->menu == MENU_GAME && winDefs[tiplist[i]->window].loc.mode != WINDOW_DISPLAYING ) )
            continue;

          if( tiplist[i]->constant && tiplist[i]->window ) // update the actual bounds
        {
            float x, y;
            if( !window_getDims(tiplist[i]->window, &x, &y, NULL, NULL, NULL, NULL, NULL, NULL) )
                continue;
            
            tiplist[i]->bounds.lx = x + tiplist[i]->rel_bounds.lx;
            tiplist[i]->bounds.hx = x + tiplist[i]->rel_bounds.hx;
            tiplist[i]->bounds.ly = y + tiplist[i]->rel_bounds.ly;
            tiplist[i]->bounds.hy = y + tiplist[i]->rel_bounds.hy;
        }

        // if there was a collision
          if( mouseCollision( &tiplist[i]->bounds ) 
            && (tiplist[i]->menu != MENU_GAME || !window_IsBlockingCollison(tiplist[i]->window)) 
            && (!tiplist[i]->parent || mouseCollision(&tiplist[i]->parent->box) ) )
        {
            if( !hit ) // this is the first (window) collision
                highest_z = i;
             hit = TRUE; // mark that we did hit something

            if( tiplist[i]->window && // if there is a window, compare it to the last window, chose whichever is on top
                winDefs[tiplist[i]->window].window_depth > winDefs[tiplist[highest_z]->window].window_depth )
            {
                highest_z = i;
            }
            else
            {
                highest_z = i;
                break;
            }
        }
    }

    if( hit )
    {
        if( currentTT != i ) // make sure we are curent
        {
            currentTT = i;
            timer = 0;
        }
        else // we are hitting the current again so increment the timer
        {
            timer += TIMESTEP;

            if( tiplist[currentTT]->timer && timer > tiplist[currentTT]->timer )
                gToolTipMode = TIP_ACTIVE;
            else if( timer > optionGetf(kUO_ToolTipDelay)*100 ) // timer reached the threshold so go live with the tip
                gToolTipMode = TIP_ACTIVE;
        }
    }

    if( !hit && tiplist[currentTT]->parent) // if there was not a hit, check the currents parent
    {
        if ( scaled_point_cbox_clsn( gMouseInpCur.x, gMouseInpCur.y, &tiplist[currentTT]->parent->box ) )
            hit = TRUE;
    }

    // no hits or input, so release the tip
      if( (!hit || inpGetKeyBuf() || isDown( MS_LEFT ) || isDown( MS_RIGHT )) && gToolTipMode == TIP_ACTIVE  )
     {
        timer = 0;
        gToolTipMode = TIP_INACTIVE;
    }

}
void repressToolTips(void)
{
    sRepressed = true;
}

void forceDisplayToolTip(ToolTip *tip, CBox * box, const char * txt, int menu, int window)
{
    int i;
    int s = do_scissor;
    int tipcount = eaSize(&tiplist);

    if(!optionGet(kUO_UseToolTips))
        return;

    for (i = tipcount - 1; i >= 0; i--)
    {
        if (tiplist[i] == tip)
        {
            currentTT = i;
            break;
        }
    }
    setToolTipEx(tip,box,txt,NULL,menu,window,0,tip->flags);

    enableToolTips(true);
}
