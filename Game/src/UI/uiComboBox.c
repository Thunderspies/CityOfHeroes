
#include "UI/uiUtil.h"
#include "UI/uiGame.h"
#include "UI/uiInput.h"
#include "graphics/ttFontUtil.h"
#include "UI/uiEditText.h"
#include "UI/uiUtilGame.h"
#include "UI/uiUtilMenu.h"
#include "UI/uiComboBox.h"
#include "UI/uiScrollBar.h"
#include "UI/uiWindows.h"
#include "UI/uiClipper.h"
#include "UI/uiToolTip.h"

#include "UI/sprite/sprite_base.h"
#include "UI/sprite/sprite_text.h"
#include "UI/sprite/sprite_font.h"
#include "graphics/textureatlas.h"

#include "cmdparse/cmdgame.h"
#include <utilitieslib/components/earray.h>
#include <utilitieslib/utils/mathutil.h>

enum
{
    NULLBOX,
    COMBOBOX,
    COMBOBOX_CHECK,
    COMBOBOX_TITLE,
};

void combobox_init( ComboBox *cb, float x, float y, float z, float wd, float ht, float expandHt, const char **strings, int wdw )
{
    memset( cb, 0, sizeof(ComboBox) );
    cb->x = x;
    cb->y = y;
     cb->z = z;
    cb->wd = cb->dropWd = wd;
    cb->wdw = wdw;
    cb->type = COMBOBOX;

    if( ht < 2*(PIX3+R10) )
        ht = 2*(PIX2+R10);
    cb->ht = ht;
    cb->expandHt = expandHt;
    cb->strings = strings;
    cb->newlySelectedItem = 1;

    if(!cb->sb)
        cb->sb = calloc( 1, sizeof(ScrollBar) );
}

void comboboxTitle_init( ComboBox *cb, float x, float y, float z, float wd, float ht, float expandHt, int wdw )
{
    memset( cb, 0, sizeof(ComboBox) );
    cb->x = x;
    cb->y = y;
    cb->z = z;
    cb->wd = cb->dropWd = wd;
    cb->wdw = wdw;
    cb->type = COMBOBOX_TITLE;

    if( ht < 2*(PIX3+R10) )
        ht = 2*(PIX2+R10);
    cb->ht = ht;
    cb->expandHt = expandHt;
    cb->newlySelectedItem = 1;

    cb->sb = calloc( 1, sizeof(ScrollBar) );
}

void combocheckbox_init( ComboBox *cb, float x, float y, float z, float wd, float drop_wd, float ht, float expandHt, char *title, int wdw, int reverse, int all )
{
    memset( cb, 0, sizeof(ComboBox) );
    cb->x = x;
     cb->y = y;
    cb->z = z;
    cb->wd = wd;
    cb->dropWd = drop_wd;
    cb->wdw = wdw;
    cb->type = COMBOBOX_CHECK;
    cb->title = malloc( sizeof(char)*(strlen(title)+1) );
    strcpy( cb->title, title );

    if( ht < 2*(PIX3+R10) )
        ht = 2*(PIX2+R10);
    cb->ht = ht;
    cb->expandHt = expandHt;
    cb->reverse = reverse;
    cb->all = all;

    cb->sb = calloc( 1, sizeof(ScrollBar) );

    if( all )
        combocheckbox_add( cb, 1, NULL, "AllString", 0 );
}


void combocheckbox_add( ComboBox *cb, int selected, AtlasTex *icon, const char * txt, int id )
{
    comboboxTitle_add( cb, selected, icon, txt, NULL, id, CLR_WHITE, 0, 0 );
}

void comboboxTitle_add( ComboBox *cb, int selected, AtlasTex *icon, const char * txt, const char * title, int id, int color, int icon_color, void * data )
{
    ComboCheckboxElement *cce = calloc(1,sizeof(ComboCheckboxElement));

    if (cb->type == NULLBOX)
        return;

    if( !cb->elements )
        eaCreate(&cb->elements);

     cce->id = id;
    cce->selected = selected;
    cce->icon = icon;
    cce->color = color;
    cce->icon_color = CLR_WHITE;
    cce->data = data;
    if(icon_color)
        cce->icon_color = icon_color;

    if( txt )
    {
        cce->txt = malloc( sizeof(char)*(strlen(txt)+1) );
        strcpy( cce->txt, txt );
    }

    if( title )
    {
        cce->title = malloc( sizeof(char)*(strlen(title)+1) );
        strcpy( cce->title, title );
    }

    eaPush(&cb->elements, cce );
}

void combocheckbox_reset( ComboBox *cb )
{
    int i;
    for( i = eaSize(&cb->elements)-1; i > 0; i-- )
    {
        cb->elements[i]->selected = 0;
    }
}

int combocheckbox_anythingselected( ComboBox *cb )
{
    int i;
    for( i = eaSize(&cb->elements)-1; i > 0; i-- )
    {
        if( cb->elements[i]->selected)
            return 1;
    }

    return 0;
}

void comboboxSharedElement_add( ComboCheckboxElement ***elements, AtlasTex *icon, char *txt, char *title, int id, void *data )
{
    ComboCheckboxElement *cce = calloc(1,sizeof(ComboCheckboxElement));

    if( !elements )
        eaCreate(elements);

    cce->id = id;
    cce->icon = icon;
    cce->icon_color = CLR_WHITE;

    if( txt )
    {
        cce->txt = malloc( sizeof(char)*(strlen(txt)+1) );
        strcpy( cce->txt, txt );
    }

    if( title )
    {
        cce->title = malloc( sizeof(char)*(strlen(title)+1) );
        strcpy( cce->title, title );
    }

    cce->data = data;

    eaPush(elements, cce );
}

void comboboxRemoveAllElements( ComboBox * cb )
{
    while( eaSize(&cb->elements) )
    {
        ComboCheckboxElement * cce = eaRemove(&cb->elements, 0);

        if( cce )
        {
            if( cce->title )
                free( cce->title );
            if( cce->txt )
                free( cce->txt );
            free( cce );
        }
    }
}

void comboboxClear(ComboBox *cb)
{
    if (cb->type == NULLBOX)
        return;
    comboboxRemoveAllElements(cb);
    SAFE_FREE(cb->title);
    SAFE_FREE(cb->sb);
    memset( cb, 0, sizeof(ComboBox) );
}

#define XSPACE 10
//
//
const char *combobox_display( ComboBox *cb )
{
    int i, count, unselectable_count = 0, max_count, frame_color, back_color;
    CBox box, box2;
    AtlasTex *arrow;
    const char * returnString = NULL;
    float tx, ty, tz, tht, twd, sc = 1.f;
    int window_color, closed = 0;
    UIBox uibox;
    int print_flags;
    int turn_off_collisions_after_scrollbar = 0;

    if (cb->type == NULLBOX)
    { //We're trying to display an uninitialized combobox
        return NULL;
    }

    // if its linked to a get offset coordinates
     if( cb->wdw )
    {
        float wx, wy, wz, wwd,wht;
        
         if( window_getMode( cb->wdw ) != WINDOW_DISPLAYING )
            return NULL;

        window_getDims( cb->wdw, &wx, &wy, &wz, &wwd, &wht, &sc, &window_color, 0 );
        tx = cb->x*sc + wx;
        ty = cb->y*sc + wy;
        tz = cb->z + wz;
         tht = cb->ht;
        twd = cb->wd;
    }
    else
    {
        tx = cb->x;
        ty = cb->y;
        tz = cb->z;
        tht = cb->ht;
        twd = cb->wd;
        window_color =  CLR_WHITE;
        if( cb->sc )
            sc = cb->sc;
    }

     if( cb->architect ) 
    {
        if(cb->architect==COMBOBOX_ARCHITECT_UNSELECTED)
            window_color = CLR_MM_BUTTON; 
        else if(cb->architect == COMBOBOX_ARCHITECT_SELECTED)
             window_color = CLR_MM_SEARCH_HIGHLIGHT; 
        else 
            window_color = CLR_MM_BUTTON; 
        cb->sb->architect = 1;
    }

    // load the proper arrow
     if( cb->reverse )
        arrow = atlasLoadTexture( "chat_separator_arrow_up.tga" );
    else
        arrow = atlasLoadTexture( "chat_separator_arrow_down.tga" );

    // clear change notification
    cb->changed = false;

      font_color( CLR_WHITE, CLR_WHITE );
    if( cb->architect )
        font_color( CLR_MM_TEXT, CLR_MM_TEXT );

    font( &game_12 );

    // scale the percent its open
      if( cb->state )
    {
         tz += 10;
        cb->scale += TIMESTEP*.3;
        if( cb->scale > 1.f )
            cb->scale = 1.f;
    }
    else
    {
        cb->scale -= TIMESTEP*.3;
        if( cb->scale < 0.f )
            cb->scale = 0.f;
    }

    if( cb->scale != 0.f )
    {
        tz += 10;                    // push an open combobox to a higher z value to prevent overlap
        clipperPushRestrict(NULL);    // break this frame out of clipping if combobox is open
    }

    //bounds check
    if( cb->type != COMBOBOX )
    {
        if( cb->currChoice < 0 ||
            cb->currChoice >= eaSize( &cb->elements ) )
        {
            cb->currChoice = 0;
            cb->changed = true;
        }
    }

    // the old-style combobox, that just has a list of strings
       if( cb->type == COMBOBOX )
    {
        uibox.x = tx;
         uibox.y = ty;
          uibox.width = twd*sc - (arrow->width + 2*PIX3)*sc;
         uibox.height = tht*sc;
        clipperPushRestrict(&uibox);

           if(str_wd(&game_12, sc, sc, cb->strings[cb->currChoice]) > ((twd - arrow->width + R10 + 2*PIX3)*sc))
            font(&game_9);
        else
            font( &game_12 );
        cprntEx( tx+XSPACE*sc, ty + tht*sc/2, tz+2, sc, sc, CENTER_Y, cb->strings[cb->currChoice] );
        count = max_count = eaSize(&cb->strings);

        clipperPop();
    }
    // the new combobox that has a list of elements, but only one can be selected at a time
    else if( cb->type == COMBOBOX_TITLE )
    {
        int offset = 0;
           if( cb->elements[cb->currChoice]->icon )
        {
            if( cb->elements[cb->currChoice]->icon != white_tex_atlas )
            {
                  display_sprite( cb->elements[cb->currChoice]->icon, tx+XSPACE*sc, ty + tht*sc/2 - (FONT_HEIGHT-2)*sc/2, tz+3, (FONT_HEIGHT-2)*sc/cb->elements[cb->currChoice]->icon->height, (FONT_HEIGHT-2)*sc/cb->elements[cb->currChoice]->icon->height, cb->elements[cb->currChoice]->icon_color );
                offset = FONT_HEIGHT*sc;
            }
        }

        uibox.x = tx;
        uibox.y = ty;
        uibox.width = twd*sc - (arrow->width + 2*PIX3)*sc;
        uibox.height = tht*sc;
        clipperPushRestrict(&uibox);

        if( cb->elements[cb->currChoice]->color )
            font_color( cb->elements[cb->currChoice]->color,cb->elements[cb->currChoice]->color);
        print_flags = CENTER_Y;

        if (cb->elements[cb->currChoice]->flags & CCE_TITLE_NOMSPRINT)
            print_flags |= NO_MSPRINT;

        cprntEx( tx+XSPACE*sc + offset, ty + tht*sc/2, tz+2, sc, sc, print_flags, cb->elements[cb->currChoice]->title);
        clipperPop();
        // only selectable elements are visible
        count  = 0;
         for( i = eaSize(&cb->elements)-1; i>=0; i--)
        {
            int elem_width;

            if (cb->elements[i]->flags & CCE_TEXT_NOMSPRINT)
                elem_width = str_wd_notranslate(&game_12, 1.f, 1.f, cb->elements[i]->txt );
            else
                elem_width = str_wd(&game_12, 1.f, 1.f, cb->elements[i]->txt );

            if( cb->dropWd < elem_width + R10*4 )
                cb->dropWd = elem_width + R10*4;
            if( !( cb->elements[i]->flags & CCE_UNSELECTABLE ) )
                count++;
            else if( i == cb->currChoice )
            {
                 int j;
                for( j = 0; j < eaSize(&cb->elements); j++)
                {
                    if( !( cb->elements[j]->flags & CCE_UNSELECTABLE ) )
                    {
                        cb->currChoice = j;
                        cb->changed = true;
                        break;
                    }
                }
            }
        }
        max_count = eaSize(&cb->elements);
    }
    else // new style with list of elements, but any number of elements can be displayed
     {
        cprntEx( tx+XSPACE*sc, ty + tht*sc/2, tz+2, sc, sc, CENTER_Y, cb->title );

        // only selectable elements are visible
        count  = 0;
        for( i = eaSize(&cb->elements)-1; i>=0; i--)
        {
            int elem_width;

            if (cb->elements[i]->flags & CCE_TEXT_NOMSPRINT)
                elem_width = str_wd_notranslate(&game_12, 1.f, 1.f, cb->elements[i]->txt );
            else
                elem_width = str_wd(&game_12, 1.f, 1.f, cb->elements[i]->txt );

            if( cb->dropWd < elem_width )
                cb->dropWd = elem_width;

            if( !( cb->elements[i]->flags & CCE_UNSELECTABLE ) )
                count++;
            else if( i == cb->currChoice )
            {
                int j;
                for( j = 0; j < eaSize(&cb->elements); j++)
                {
                    if( !( cb->elements[j]->flags & CCE_UNSELECTABLE ) )
                    {
                        cb->currChoice = j;
                        cb->changed = true;
                        break;
                    }
                }
            }
        }
        max_count = eaSize(&cb->elements);
    }

    // state management
      BuildCBox( &box, tx, ty, (cb->dropWd+6)*sc, tht*sc );

    if( cb->reverse )
        BuildCBox( &box2, tx + (cb->wd - cb->dropWd)/2, ty-MIN(cb->scale*cb->expandHt*sc,(count+1)*FONT_HEIGHT*sc), cb->dropWd*sc,MIN(cb->scale*cb->expandHt*sc,(count+1)*FONT_HEIGHT*sc ) );
    else
        BuildCBox( &box2, tx + (cb->wd - cb->dropWd)/2, ty+tht*sc, cb->dropWd*sc, MIN(cb->scale*cb->expandHt*sc,(count+1)*FONT_HEIGHT*sc) );

       if( cb->state && !cb->sb->grabbed && 
        !mouseCollision( &box2 ) && (mouseDown( MS_LEFT ) || mouseDown( MS_RIGHT )) ) // click outside box closes box
    {
        cb->state = 0;
        closed = true;
    }

    if( cb->wdw && (winDefs[cb->wdw].being_dragged || winDefs[cb->wdw].drag_mode) )
    {
        cb->state = 0;
        closed = true;
    }

    if (cb->wdw)
    {
        if (cb->locked)
        {
            frame_color = CLR_GREY;
        }
        else if (cb->color)
        {
            frame_color = cb->color;
        }
        else
        {
            frame_color = window_color;
        }
    }
    else
    {
        frame_color = CLR_WHITE;
    }
    if (cb->back_color)
    {
        back_color = cb->back_color;
    }
    else
    {
        back_color = CLR_BLACK;
    }

    BuildCBox( &box, tx, ty, twd*sc, tht*sc );
    if( !cb->unselectable )
    {
        int arrow_color;
        if( mouseCollision( &box ) && !cb->locked)
        {
            if (cb->highlight_color)
            {
                arrow_color = cb->highlight_color;
            }
            else
            {
                arrow_color = CLR_GREEN;
            }
            if( mouseDownHit( &box, MS_LEFT ) && !closed )
            {
                cb->state = !cb->state;
            }
        }
        else
        {
            arrow_color = frame_color;
        }
        drawFrame( PIX2, R10, tx, ty, tz+1, twd*sc, tht*sc, sc, frame_color, back_color );
        display_sprite( arrow, tx + (twd - (arrow->width+PIX3*2))*sc, ty + (tht - arrow->height)*sc/2, tz+2, sc, sc, arrow_color );
    }

    if( cb->scale == 0.f )
        clipperPushRestrict(NULL);

     font( &game_12  );
    font_color( CLR_WHITE, CLR_WHITE );

    tx = tx + (cb->wd - cb->dropWd)/2;
    uibox.x = tx;
    uibox.width = (cb->dropWd - PIX3)*sc;

      if( cb->reverse )
    {
        uibox.y = ty-cb->scale*cb->expandHt*sc;
        uibox.height = cb->scale*cb->expandHt*sc;
    }
    else
    {
        uibox.y = ty+tht*sc;
         uibox.height = (cb->scale*cb->expandHt-((cb->scale==1.f)?PIX3:0))*sc;
    }
      clipperPush(&uibox);

    if (!(cb->unselectable) && !(cb->locked))
    {
        for( i = 0; i < max_count && cb->scale > 0.f; i++ )
        {
            if (i == 0)
            { //turn off tooltips for this frame
                repressToolTips();
            }
            // determine collision box
            if( cb->reverse )
                BuildCBox( &box, tx, ty - FONT_HEIGHT*(i-unselectable_count+1)*sc - cb->sb->offset, (cb->dropWd - arrow->width)*sc, FONT_HEIGHT*sc );
            else
                BuildCBox( &box, tx, ty + tht*sc + FONT_HEIGHT*(i-unselectable_count)*sc - cb->sb->offset, (cb->dropWd - arrow->width)*sc, FONT_HEIGHT*sc );

            if( cb->type == COMBOBOX ) // if its the old type display the string
            {
                if(str_wd(&game_12, sc, sc, cb->strings[i]) > (twd-10)*sc)
                    font(&game_9);
                else
                    font( &game_12 );

                if( cb->reverse)
                    prnt( tx+XSPACE*sc, ty - FONT_HEIGHT*(i+1)*sc - cb->sb->offset, tz+3, sc, sc, cb->strings[i] );
                else
                    prnt( tx+XSPACE*sc, ty + (tht + FONT_HEIGHT*(i+1))*sc - cb->sb->offset, tz+3, sc, sc, cb->strings[i] );
            }
            else // new type
            {
                AtlasTex * mark;
                float tmpx = tx+XSPACE*sc;
                float tmpy;

                // skip over unselectable elements
                if( cb->elements[i]->flags & CCE_UNSELECTABLE )
                {
                    unselectable_count++;
                    continue;
                }

                // determine y coord
                if( cb->reverse )
                    tmpy = ty - FONT_HEIGHT*((i-unselectable_count)+.5)*sc - cb->sb->offset;
                else
                    tmpy = ty + (tht + FONT_HEIGHT*((i-unselectable_count)+.5))*sc - cb->sb->offset;

                // display the checkbox
                if( (cb->type == COMBOBOX_CHECK && cb->elements[i]->selected)||
                    (cb->type == COMBOBOX_TITLE && cb->currChoice == i ) )
                {
                    mark = atlasLoadTexture( "Context_checkbox_filled.tga" );
                    display_sprite( mark, tmpx, tmpy-mark->height*sc/2, tz+4, sc, sc, (window_color & 0xffffff00)|0xff  );

                    mark = atlasLoadTexture( "Context_checkbox_empty.tga" );
                    display_sprite( mark, tmpx, tmpy-mark->height*sc/2, tz+3, sc, sc, (window_color & 0xffffff00)|0xff );
                }
                else
                {
                    mark = atlasLoadTexture( "Context_checkbox_empty.tga" );
                    display_sprite( mark, tmpx, tmpy-mark->height*sc/2, tz+3, sc, sc, window_color | 0xff );
                }

                tmpx += (mark->width+5)*sc;

                if(cb->elements[i]->color)
                    font_color(cb->elements[i]->color,cb->elements[i]->color);

                print_flags = 0;

                if( cb->elements[i]->flags & CCE_TEXT_NOMSPRINT )
                    print_flags |= NO_MSPRINT;

                // display the icon if nessecary
                if( cb->elements[i]->icon )
                {
                    if( cb->elements[i]->icon != white_tex_atlas )
                        display_sprite( cb->elements[i]->icon, tmpx, tmpy - (FONT_HEIGHT-2)*sc/2, tz+3, (FONT_HEIGHT-2)*sc/cb->elements[i]->icon->height, (FONT_HEIGHT-2)*sc/cb->elements[i]->icon->height, cb->elements[i]->icon_color );

                    if( cb->elements[i]->txt )
                    {
                        cprntEx( tmpx+20*sc, tmpy + (FONT_HEIGHT/2-2)*sc, tz+3, sc, sc, print_flags, cb->elements[i]->txt );
                    }
                }
                else
                {
                    int elem_width;

                    if (print_flags & NO_MSPRINT)
                        elem_width = str_wd_notranslate(&game_12, sc, sc, cb->elements[i]->txt);
                    else
                        elem_width = str_wd(&game_12, sc, sc, cb->elements[i]->txt);

                    if(elem_width > ((cb->dropWd-mark->width-R10)*sc))
                        font(&game_9);
                    else
                        font( &game_12 );
                    cprntEx( tmpx, tmpy + (FONT_HEIGHT/2-2)*sc, tz+3, sc, sc, print_flags, cb->elements[i]->txt );
                }
            }

            if( mouseCollision(&box) )
            {
                if( cb->reverse )
                    drawFlatFrame( PIX2, R4, tx+PIX3*sc, ty - FONT_HEIGHT*((i-unselectable_count)+1)*sc - cb->sb->offset, tz+1, (cb->dropWd  - PIX3*2)*sc, FONT_HEIGHT*sc, sc, 0xffffff22, 0xffffff22 );
                else
                    drawFlatFrame( PIX2, R4, tx+PIX3*sc, ty + (tht + FONT_HEIGHT*(i-unselectable_count))*sc - cb->sb->offset, tz+1, (cb->dropWd - PIX3*2)*sc, FONT_HEIGHT*sc, sc, 0xffffff22, 0xffffff22 );

                if( mouseUp( MS_LEFT ) && !cb->sb->grabbed )
                {
                    cb->changed = true;
                    if( cb->type == COMBOBOX || cb->type == COMBOBOX_TITLE )
                    {
                        cb->currChoice = i;
                        cb->state = 0;
                        cb->newlySelectedItem = 1;

                        if( cb->type == COMBOBOX )
                            returnString = cb->strings[i];
                    }
                    else
                    {
                        cb->elements[i]->selected = !cb->elements[i]->selected;

                        if(cb->all)
                        {
                            if( i == 0 && cb->elements[i]->selected )
                                combocheckbox_reset( cb );
                            else if( cb->elements[i]->selected )
                                cb->elements[0]->selected = 0;    
                            else if( !combocheckbox_anythingselected(cb) )
                                cb->elements[0]->selected = 1;    
                        }
                    }
                }
                turn_off_collisions_after_scrollbar = TRUE;
            }
        }
    }

    clipperPop();
    
    // draw the frame uncompassing the elements
    if( cb->reverse )
    {
        drawFrame( PIX2, R10, tx, ty - MIN(cb->scale*cb->expandHt,(0.5+count)*FONT_HEIGHT)*sc, tz, twd*sc, (tht + MIN(cb->scale*cb->expandHt,(0.5+count)*FONT_HEIGHT))*sc, sc, frame_color, back_color );
        if( cb->scale == 1.f )
                doScrollBar( cb->sb, cb->scale*cb->expandHt*sc, eaSize(&cb->strings)*FONT_HEIGHT*sc, tx+twd*sc, ty -  - MIN(cb->scale*cb->expandHt,(0.5+count)*FONT_HEIGHT)*sc, tz+2, 0, &uibox );
    }
    else
    {
            drawFrame( PIX2, R10, tx, ty, tz, cb->dropWd*sc, (tht + MIN(cb->scale*cb->expandHt,(0.5+count)*FONT_HEIGHT))*sc, sc, 
                      frame_color&(0xffffff00 | (int)(0xff*cb->scale)), back_color&(0xffffff00 | (int)(0xff*cb->scale)));
           if( cb->scale == 1.f )
              doScrollBar( cb->sb, (cb->scale*cb->expandHt-R10-PIX3)*sc, (MAX(eaSize(&cb->strings),eaSize(&cb->elements))*FONT_HEIGHT+PIX3)*sc, tx+cb->dropWd*sc, ty+tht*sc, tz+20, 0, &uibox);
    }
    collisions_off_for_rest_of_frame |= turn_off_collisions_after_scrollbar;

    clipperPop();
    return returnString;
}

#define PF_HT   18

int combobox_displayRegister( ComboBox *cb, int radius, int color, int back_color, int text_color, char *defaultText, int locked, float screenScaleX, float screenScaleY, int xOffset, int yOffset )
{
    int i = 0, count;
     CBox box;
    AtlasTex *arrow = atlasLoadTexture( "chat_separator_arrow_down.tga" );
    float screenScale = MIN(screenScaleX, screenScaleY);
    int turnOffCollisions = 0;
      BuildCBox( &box, xOffset+(cb->x*screenScaleX), yOffset+(cb->y*screenScaleY), (cb->wd*screenScaleX), (cb->ht*screenScaleY) );

    if( cb->state && !cb->sb->grabbed && !mouseCollision( &box ) && (mouseUp( MS_LEFT ) || mouseUp( MS_RIGHT )) ) // click outside box closes box
        cb->state = 0;

     if( mouseCollision( &box ) && !locked)
    {
        back_color |= 0xff;
        if( mouseDown( MS_LEFT ) )
            cb->state = !cb->state;
    }

    //border
    drawFlatFrameBox( PIX2, radius, &box, cb->z, color, 0 );
    display_sprite( arrow, xOffset + (cb->x*screenScaleX)+ 25.f*screenScale - (arrow->width*screenScale) - (2*PIX2*screenScale), yOffset + (cb->y*screenScaleY) + (cb->ht*screenScaleY)/2 - arrow->height*screenScale/2, cb->z, screenScale, screenScale, CLR_WHITE );

    if( cb->state )
    {
        cb->scale += TIMESTEP*.3;
        if( cb->scale > 1.f )
            cb->scale = 1.f;
    }
    else
    {
        cb->scale -= TIMESTEP*.3;
        if( cb->scale < 0.f )
            cb->scale = 0.f;
    }

    font( &hybrid_14 );
     font_color( text_color, text_color );
    font_outl(0);

    if( !cb->strings || strcmp(cb->strings[cb->currChoice], "None" ) == 0 )
        cprntEx( xOffset+arrow->width*screenScale+(cb->x*screenScaleX)+(XSPACE*screenScaleX), yOffset+(cb->y*screenScaleY) + (cb->ht*screenScaleY)/2, cb->z, screenScale, screenScale, CENTER_Y, defaultText ? defaultText : ". . . ." );
    else
        cprntEx( xOffset+arrow->width*screenScale+(cb->x*screenScaleX)+(XSPACE*screenScaleX), yOffset+(cb->y*screenScaleY) + (cb->ht*screenScaleY)/2, cb->z, screenScale, screenScale, CENTER_Y, cb->strings[cb->currChoice] );

    set_scissor(1);
    if (cb->state)
        scissor_dims(xOffset+(cb->x*screenScaleX), yOffset+(cb->y*screenScaleY)+(cb->ht*screenScaleY), (cb->wd*screenScaleX), (cb->scale*cb->expandHt*screenScaleY)-(PIX3*screenScaleY) );
    else
        scissor_dims(xOffset+(cb->x*screenScaleX), yOffset+(cb->y*screenScaleY)+(cb->ht*screenScaleY)-(3*PIX2*screenScaleY), (cb->wd*screenScaleX), (cb->scale*cb->expandHt*screenScaleY)-(PIX3*screenScaleY) );

    if (cb->state)
    {
        count = eaSize(&cb->strings);
        for( i = 0; i <count; i++ )
        {
            BuildCBox( &box, xOffset+(cb->x*screenScaleX), yOffset+(cb->y*screenScaleY) + (cb->ht*screenScaleY) + PF_HT*(i)*screenScaleY - cb->sb->offset*screenScaleY, (cb->wd*screenScaleX), 25*screenScaleY);

            prnt( xOffset+(cb->x*screenScaleX)+(XSPACE*screenScaleX), yOffset+(cb->y*screenScaleY) + (cb->ht*screenScaleY) + PF_HT*(i+1)*screenScaleY - cb->sb->offset*screenScaleY, cb->z+1, screenScale, screenScale, cb->strings[i] );
            if( mouseCollision(&box) )
            {
                drawFlatFrameBox( PIX2, radius, &box, cb->z, 0, 0x66666666 );
                if( mouseDown( MS_LEFT ) )
                {
                    cb->currChoice = i;
                    cb->state = 0;
                }

                back_color |= 0xff;
                turnOffCollisions = true;
            }
        }
    }
    set_scissor(0);

    BuildCBox( &box, xOffset+(cb->x*screenScaleX), yOffset+(cb->y*screenScaleY), (cb->wd*screenScaleX), MIN(cb->scale*cb->expandHt*screenScaleY+(cb->ht*screenScaleY), (cb->ht*screenScaleY)+(((i)*PF_HT+10))*screenScaleY) );
    if (!mouseCollision(&box))
    {
        cb->state = 0;
    }
    drawFlatFrameBox( PIX2, radius, &box, cb->z-1, 0, back_color );

    BuildCBox( &box, xOffset+(cb->x*screenScaleX), yOffset+(cb->y*screenScaleY)+(cb->ht*screenScaleY), (cb->wd*screenScaleX) - (arrow->width*screenScale), cb->scale*cb->expandHt*screenScaleY);
    if( cb->scale == 1.f )
    {
        doScrollBarEx( cb->sb, (cb->scale*cb->expandHt) -30, eaSize(&cb->strings)*PF_HT, (cb->x)+(cb->wd), (cb->y)+(cb->ht)+5, cb->z, &box, 0, screenScaleX, screenScaleY );
        if (cb->sb->grabbed)
        {
            cb->state = 1;
        }
    }
    font_outl(1);
    
    collisions_off_for_rest_of_frame |= turnOffCollisions;
    return cb->currChoice;
}

int combobox_displayHybridRegister( ComboBox *cb, int radius, int color, int back_color, int text_color )
{
    int i = 0, count;
    CBox box;
    AtlasTex *arrow = atlasLoadTexture( "chat_separator_arrow_down.tga" );
    int turnOffCollisions = 0;
    F32 xposSc, yposSc;
    F32 textXSc = 1.f;
    F32 textYSc = 1.f;
    SpriteScalingMode ssm = getSpriteScaleMode();
    calculatePositionScales(&xposSc, &yposSc);

    BuildScaledCBox( &box, cb->x, cb->y, cb->wd*xposSc, cb->ht*yposSc );
    
    if( cb->state && !cb->sb->grabbed && !mouseCollision( &box ) && (mouseUp( MS_LEFT ) || mouseUp( MS_RIGHT )) ) // click outside box closes box
        cb->state = 0;

    if( mouseCollision( &box ))
    {
        back_color |= 0xff;
        if( mouseDown( MS_LEFT ) )
            cb->state = !cb->state;
    }

    //border
    drawFlatFrameBox( PIX2, radius, &box, cb->z, color, 0 );
    display_sprite_positional( arrow, cb->x + (25.f - 2*PIX2)*xposSc, cb->y + cb->ht*yposSc/2.f, cb->z, 1.f, 1.f, CLR_WHITE, H_ALIGN_RIGHT, V_ALIGN_CENTER );

    if( cb->state )
    {
        cb->scale += TIMESTEP*.3;
        if( cb->scale > 1.f )
            cb->scale = 1.f;
    }
    else
    {
        cb->scale -= TIMESTEP*.3;
        if( cb->scale < 0.f )
            cb->scale = 0.f;
    }
    

    if (isTextScalingRequired())
    {
        calculateSpriteScales(&textXSc, &textYSc);
        setSpriteScaleMode(SPRITE_XY_NO_SCALE);
    }
    font( &hybrid_14 );
    font_color( text_color, text_color );
    font_outl(0);

    if( !cb->strings || strcmp(cb->strings[cb->currChoice], "None" ) == 0 )
        cprntEx( cb->x + (arrow->width + XSPACE)*xposSc, cb->y + cb->ht*yposSc/2.f, cb->z, textXSc, textYSc, CENTER_Y, ". . . ." );
    else
        cprntEx( cb->x + (arrow->width + XSPACE)*xposSc, cb->y + cb->ht*yposSc/2.f, cb->z, textXSc, textYSc, CENTER_Y, cb->strings[cb->currChoice] );

    if (cb->state)
    {
        count = eaSize(&cb->strings);
        for( i = 0; i <count; i++ )
        {
            BuildScaledCBox( &box, cb->x, cb->y + (cb->ht + PF_HT*(i) - cb->sb->offset)*yposSc, cb->wd*xposSc, 25.f*yposSc );

            prnt( cb->x + XSPACE*xposSc, cb->y + (cb->ht + PF_HT*(i+1) - cb->sb->offset)*yposSc, cb->z+1, textXSc, textYSc, cb->strings[i] );
            if( mouseCollision(&box) )
            {
                drawFlatFrameBox( PIX2, radius, &box, cb->z, 0, 0x66666666 );
                if( mouseDown( MS_LEFT ) )
                {
                    cb->currChoice = i;
                    cb->state = 0;
                }

                back_color |= 0xff;
                turnOffCollisions = true;
            }
        }
    }
    setSpriteScaleMode(ssm);

    BuildScaledCBox( &box, cb->x, cb->y, cb->wd*xposSc, MIN((cb->scale*cb->expandHt+cb->ht)*yposSc, (cb->ht+((i)*PF_HT+10))*yposSc) );
    if (!mouseCollision(&box))
    {
        cb->state = 0;
    }
    drawFlatFrameBox( PIX2, radius, &box, cb->z-1, 0, back_color );

    font_outl(1);

    collisions_off_for_rest_of_frame |= turnOffCollisions;
    return cb->currChoice;
}

int combobox_hasID(ComboBox *cb, int id)
{
    int i;

    for( i = eaSize(&cb->elements)-1; i >= 0; i-- )
    {
        if( cb->elements[i]->id == id)
            return 1;
    }

    return 0;
}

int combobox_idSelected( ComboBox *cb, int id)
{
    int i;

    for( i = eaSize(&cb->elements)-1; i >= 0; i-- )
    {
        if( cb->elements[i]->id == id && cb->elements[i]->selected)
            return 1;
    }

    return 0;
}

int combobox_GetSelected( ComboBox *cb )
{
    int i;

    if( cb->currChoice >= 0 )
        return cb->elements[cb->currChoice]->id;

    for( i = eaSize(&cb->elements)-1; i >= 0; i-- )
    {
        if( cb->elements[i]->selected )
            return cb->elements[i]->id;
    }

    return -1;
}
void combobox_setId( ComboBox *cb, int id, int selected )
{
    int i;
    for( i = eaSize(&cb->elements)-1; i >= 0; i-- )
    {
        if( cb->elements[i]->id == id )
            cb->elements[i]->selected = selected;
    }
}

void combobox_setSelectable( ComboBox *cb, int id, int selectable )
{
    int i;
    for( i = eaSize(&cb->elements)-1; i >= 0; i-- )
    {
        if( cb->elements[i]->id == id )
        {
            if( selectable )
                cb->elements[i]->flags &= ~CCE_UNSELECTABLE;
            else
                cb->elements[i]->flags |= CCE_UNSELECTABLE;
        }
    }
}

void combobox_setFlags( ComboBox *cb, int id, int flags )
{
    int i;
    for( i = eaSize(&cb->elements)-1; i >= 0; i-- )
    {
        if( cb->elements[i]->id == id )
            cb->elements[i]->flags = flags;
    }
}

void combobox_setLoc( ComboBox *cb, float x, float y, float z )
{
    cb->x = x;
    cb->y = y;
    cb->z = z;
}

//------------------------------------------------------------
// find the first idx that has passed id, starting at iStart
// -1 if none found
//----------------------------------------------------------
int combobox_mpIdxId( ComboBox *cb, int id, int iStart )
{
    int res = -1;
    int i;
    if( verify( cb ))
    {
        int size = eaSize(&cb->elements);
        for( i = iStart; i < size && res < 0; ++i ) 
        {
            if( cb->elements[i] && cb->elements[i]->id == id )
            {
                res = i;
            }
        }
    }
    // --------------------
    // finally
    
    return res;
}

//------------------------------------------------------------
// set the visible item to the passed id
//----------------------------------------------------------
void combobox_setChoiceCur( ComboBox *cb, int id )
{
    int iId = combobox_mpIdxId( cb, id, 0 );
    if( verify( iId >= 0 ) )
    {
        cb->currChoice = iId;
        cb->changed = true;
    }
}


int combobox_getBitfieldFromElements( ComboBox *cb )
{
    int i, bitfield = 0;

    for( i = 0; i < eaSize(&cb->elements); i++ )
    {
        if(cb->elements[i]->selected)
            bitfield |= (1<<cb->elements[i]->id);
    }

    return bitfield;
}

void combobox_setElementsFromBitfield( ComboBox * cb, int bitfield )
{
    int i;

     for( i = 0; i < eaSize(&cb->elements); i++ )
    {
        if( bitfield & (1<<cb->elements[i]->id))
            cb->elements[i]->selected = 1;
        else
            cb->elements[i]->selected = 0;
    }
}