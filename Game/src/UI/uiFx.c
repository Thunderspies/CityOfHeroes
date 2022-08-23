
#include <utilitieslib/stdtypes.h>
#include <utilitieslib/utils/utils.h>
#include "UI/uiUtil.h"
#include "UI/uiFx.h"
#include "UI/uiBox.h"
#include "UI/uiClipper.h"

#include "UI/sprite/sprite_base.h"
#include "UI/sprite/sprite_text.h"
#include "UI/sprite/sprite_font.h"
#include "graphics/textureatlas.h"

#include "cmdparse/cmdcommon.h"
#include "graphics/ttFontUtil.h"
#include "sound/sound.h"
#include "UI/uiWindows.h"
#include "UI/uiGame.h"
#include "gameComm/wdwbase.h"
#include "win/win_init.h"
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/components/MemoryPool.h>
#include <utilitieslib/components/earray.h>
#include "player/player.h"

int gElectricAction = 0;

//-----------------------------------------------------------------------------------------
// Electric Boogie
//-----------------------------------------------------------------------------------------

typedef struct ElectricFlash
{
    int x;
    int y;
    int z;
    int counter;
    int light;
    int id;
    float scale;

    float sc[3];
    float timer[3];
    float timeTotal[3];
    float angle[3];
    float speed[3];
    float scspeed[3];

    float totalDuration;
    float duration;

    float destwdw;

    int    *y_offset;

    int clip;
    UIBox box;

} ElectricFlash;

MP_DEFINE(ElectricFlash);

ElectricFlash **flashQueue = 0;

#define FLASH_TIMER 45
#define LIGHT_TIMER 90

void electric_display( ElectricFlash *ef )
{
    AtlasTex * spark[3] = { atlasLoadTexture( "Enhnc_Spark01.tga" ), atlasLoadTexture( "Enhnc_Spark02.tga" ), atlasLoadTexture( "Enhnc_Spark03.tga" ) };
    int color, i;
    float x, y;

     if( ef->clip )
        clipperPush(&ef->box);

    for( i = 0; i < 3; i++)
    {
        AtlasTex *spr;

        if( ef->light )
            spr = atlasLoadTexture( "Enhnc_LightRay.tga" );
        else
            spr = spark[i];

        if( ef->timeTotal[i] > 0 && ef->timeTotal[i] > ef->timer[i]  )
        {
            int green, blue, opacity;
            float opfacter;

            if(  ef->light )
            {
                green   = 153+(int)(  90.f*sinf( PI*((ef->timeTotal[i] - ef->timer[i])/ef->timeTotal[i])) );
                blue    =  54+(int)( 140.f*sinf( PI*((ef->timeTotal[i] - ef->timer[i])/ef->timeTotal[i])) );

                if ( ef->totalDuration > ef->duration-60 && ef->duration > 60 )
                    opfacter = 120*(ef->duration-ef->totalDuration)/60.0f;
                else
                    opfacter = 120.f;

                opacity =  (int)( opfacter*sinf( PI*((ef->timeTotal[i] - ef->timer[i])/ef->timeTotal[i])) );

                color = 0xff000000 | (green << 16) | (blue << 8) | opacity;
            }
            else
                color = 0xffffff00 | (int)( 255.f*sinf( PI*((ef->timeTotal[i] - ef->timer[i])/ef->timeTotal[i]) ));

            x = ef->x;

            if( ef->y_offset )
                y = ef->y - *ef->y_offset;
            else
                y = ef->y;

            if(ef->destwdw!=0 && ef->totalDuration/ef->duration > .65)
            {
                float xwnd, ywnd;
                float z, wd, ht, scale;
                int color, bcolor;

                // purposely overshoot so the shine stays at dest for a bit.
                 float pct = ((ef->totalDuration/ef->duration)-0.65)/0.15;
                if(pct>1) pct=1;

                window_getDims(ef->destwdw, &xwnd, &ywnd, &z, &wd, &ht, &scale, &color, &bcolor);
                if(ef->destwdw == WDW_STAT_BARS)
                {
                    xwnd += wd - 32;
                    ywnd += 32;
                }
                if(ef->destwdw == WDW_TRAY)
                    childButton_getCenter( &winDefs[WDW_TRAY], 2, &xwnd, &ywnd );

                x = x*(1-pct) + xwnd*pct;
                y = y*(1-pct) + ywnd*pct;
            }

            {
                int screenW, screenH;
                AtlasTex * glowL = atlasLoadTexture( "next_glowkit_L.tga" );
                AtlasTex * glowM = atlasLoadTexture( "next_glowkit_MID.tga" );
                 AtlasTex * glowR = atlasLoadTexture( "next_glowkit_R.tga" );
                float screenScaleX, screenScaleY;
                float screenScale;
                windowClientSize(&screenW, &screenH);
                screenScaleX = (float)screenW / DEFAULT_SCRN_WD;
                screenScaleY = (float)screenH / DEFAULT_SCRN_HT;
                screenScale = (screenScaleX+ screenScaleY)/2;
                 display_sprite( glowL, x - glowL->width*ef->sc[i]*1.2f*screenScale/screenScaleX, y - glowL->height*ef->sc[i]*1.2f*screenScale/(screenScaleY*2), ef->z, ef->sc[i]*1.2f*screenScale/screenScaleX, ef->sc[i]*1.2f*screenScale/screenScaleY, color );
                 display_sprite( glowR, x, y - glowR->height*ef->sc[i]*1.2f*screenScale/(2*screenScaleY), ef->z, ef->sc[i]*1.2f*screenScale/screenScaleX, ef->sc[i]*1.2f*screenScale/screenScaleY, color );
                
                display_rotated_sprite( spr, x - spr->width*ef->sc[i]*screenScale/(screenScaleX*2), y - spr->height*ef->sc[i]*screenScale/(screenScaleY*2), ef->z, ef->sc[i]*screenScale/screenScaleX, ef->sc[i]*screenScale/screenScaleY, color, ef->angle[i], 1  );
            }
            ef->timer[i] += TIMESTEP;

            if( ef->light )
                ef->sc[i] += TIMESTEP * ef->scspeed[i];

            if( ef->timer[i] > ef->timeTotal[i] )
            {
                ef->timer[i] = 0;
                ef->timeTotal[i] = -((rand()%5)+5);

                if( ef->light )
                    ef->sc[i] = ef->scale;
            }
        }
        else
        {
            ef->timer[i] -= TIMESTEP;

            if( ef->timer[i] < ef->timeTotal[i] )
            {
                ef->timer[i] = 0;
                ef->angle[i] = ((float)(rand()%360) / 360)*2*PI;

                if( ef->light )
                {
                    ef->timeTotal[i] = rand()%30+30;
                    ef->speed[i] = ((float)(rand()%10)-5)*.01;
                }
                else
                {
                    ef->timeTotal[i] = ((rand()%5)+5);
                    ef->speed[i] = ((float)(rand()%10))*.001;
                }
            }
        }

        ef->angle[i] += TIMESTEP*ef->speed[i];
    }

      if( ef->counter > 0 )
    {
        for( i = 0; i < 10; i++ )
        {
            AtlasTex * spr =  atlasLoadTexture( "Enhnc_Spark01.tga" );
            float sc = (rand()%10)*.1;

            if( ef->y_offset )
                y = ef->y - *ef->y_offset;
            else
                y = ef->y;

            display_rotated_sprite( spr, ef->x - spr->width*sc/2, y - spr->height*sc/2, ef->z+10, sc, sc, CLR_WHITE, ((float)(rand()%360) / 360)*2*PI, 1 );
        }
        ef->counter--;
    }

    if( ef->clip )
        clipperPop();

    ef->totalDuration += TIMESTEP;
}

void electric_clearAll()
{
    int i;
    for (i = 0; i < eaSize(&flashQueue); i++)
        MP_FREE(ElectricFlash, flashQueue[i]);
    eaSetSize(&flashQueue, 0);
}

static int electric_drawAll()
{
    int i;

    if( !flashQueue )
        eaCreate( &flashQueue );

    srand( timerCpuTicks64() );

    for( i = 0; i < eaSize(&flashQueue); i++ )
    {
        electric_display( flashQueue[i] );

        if( flashQueue[i]->duration > 0 && flashQueue[i]->totalDuration > flashQueue[i]->duration )
        {
            ElectricFlash * flash = eaRemove( &flashQueue, i );
            MP_FREE(ElectricFlash, flash);
            i--;
        }
    }

    return eaSize(&flashQueue);
}

void electric_initEx( float x, float y, float z, float scale, int light, int duration, int destwdw, int id, int *offset )
{
    int i;
    ElectricFlash *ef;
    UIBox * box;

    MP_CREATE(ElectricFlash, 32);

    if( !playerPtr() )
        return;

    ef = MP_ALLOC(ElectricFlash);

     ef->x = x;
    ef->y = y;
    ef->z = z;
    ef->id = id;
    ef->y_offset = offset;

    if( duration < 0 )
        ef->duration = 5000;
    else
        ef->duration = duration;

    srand( timerCpuTicks64() );

    box = clipperGetBox(clipperGetCurrent());
    if( box )
    {
        ef->clip = true;
        memcpy( &ef->box, box, sizeof(UIBox) );
    }
    else
        ef->clip = false;

    if( light )
    {
        ef->light = 1;
        for( i = 0; i < 3; i++ )
        {
            ef->timeTotal[i] = rand()%15;
            ef->speed[i] = ((float)(rand()%10)-5)*.01;
            ef->angle[i] = ((float)(rand()%360) / 360)*2*PI;
            ef->sc[i] = ((float)(rand()%10))*.01*scale;
            ef->scspeed[i] = ((float)(rand()%100))*.001*scale;
            ef->scale = scale;
            ef->destwdw = destwdw;
        }
    }
    else
    {
        ef->counter = 5;

        for( i = 0; i < 3; i++ )
        {
            ef->timeTotal[i] = rand()%10 - 5;
            ef->speed[i] = ((float)(rand()%10))*.002;
            ef->angle[i] = ((float)(rand()%360) / 360)*2*PI;
            ef->scspeed[i] = 0;
            ef->sc[i] = scale;
            ef->destwdw = destwdw;
        }
    }

    if( !flashQueue )
        eaCreate( &flashQueue );

    eaPush( &flashQueue, ef );
}

int electric_init( float x, float y, float z, float scale, int light, int duration, int count, int *offset )
{
    int i;
    static id_counter = 0;

    if( !playerPtr() )
        return 0;

    gElectricAction = TRUE;
    id_counter++;

    for( i = 0; i < count; i++ )
        electric_initEx(x, y, z, scale, light, duration, 0, id_counter, offset );

    return id_counter;
}

void electric_removeById( int id )
{
    int i;
    for( i = 0; i < eaSize(&flashQueue); i++ )
    {
        if( flashQueue[i]->id == id )
        {
            // FIXEAR: Is this safe considering the loop index?
            ElectricFlash * flash = eaRemove( &flashQueue, i );
            if( flash )
                MP_FREE(ElectricFlash, flash);
        }
    }
}

//-----------------------------------------------------------------------------------------
// Fading Text
//-----------------------------------------------------------------------------------------

typedef struct FadingText
{
    float x;
    float y;
    float z;

    float timer;
    float scale;
    float expand_rate;

    int duration;
    int color1;
    int color2;

    int flags;

    TTDrawContext *font;

    float destwdw;

    char text[128];

}FadingText;

MP_DEFINE(FadingText);

FadingText **fadingTextQueue = 0;

void fadingTextEx( float x, float y, float z, float scale, int color1, int color2,
    int flags, TTDrawContext *pfont, int duration, int destwdw, char * txt, float expand_rate )
{
    FadingText * ft;
    MP_CREATE(FadingText, 32);

    if( !playerPtr() )
        return;

    ft = MP_ALLOC(FadingText);

    ft->x = x;
    ft->y = y;
    ft->z = z;
    ft->scale = scale;
    ft->color1 = (0xffffff00 & color1);
    ft->color2 = (0xffffff00 & color2);
    ft->duration = duration;
    ft->font     = pfont;
    ft->flags    = flags;
    ft->destwdw  = destwdw;
    ft->expand_rate = expand_rate;

    strcpy( ft->text, txt );

    if( !fadingTextQueue )
        eaCreate( &fadingTextQueue );

    eaPush( &fadingTextQueue, ft );
}

void fadingText( float x, float y, float z, float scale, int color1, int color2, int duration, char * txt )
{
    fadingTextEx(x, y, z, scale, color1, color2, CENTER_X, &game_18, duration, 0, txt, .01f );
}

static void fadingText_display( FadingText *ft )
{
    float vec;
    int clr1, clr2;
    float x, y, scale;

    vec = sinf( (ft->timer/ft->duration)*PI );
    
    clr1 = ft->color1 | (int)(255.f*vec);
    clr2 = ft->color2 | (int)(255.f*vec);

    x = ft->x;
    y = ft->y;
    scale = ft->scale;
    if(ft->destwdw!=0 && ft->timer/ft->duration > .65)
    {
        float xwnd, ywnd;
        float z, wd, ht, scalewnd;
        int color, bcolor;

        // purposely overshoot so the shine stays at dest for a bit.
         float pct = ((ft->timer/ft->duration)-0.65)/0.15;
        if(pct>1) pct=1;

        window_getDims(ft->destwdw, &xwnd, &ywnd, &z, &wd, &ht, &scalewnd, &color, &bcolor);
        if(ft->destwdw == WDW_STAT_BARS)
        {
            xwnd += wd - 32;
            ywnd += 32;
        }
        if(ft->destwdw == WDW_TRAY)
            childButton_getCenter( &winDefs[WDW_TRAY], 2, &xwnd, &ywnd );
        x = x*(1-pct) + xwnd*pct;
        y = y*(1-pct) + ywnd*pct;
        scale = ft->scale*(1-pct);
    }

    font( ft->font );
     font_color( clr1, clr2 );

    cprntEx( x, y, ft->z, scale, scale, ft->flags, ft->text );
    
      ft->timer += TIMESTEP;
    ft->scale += ft->expand_rate*TIMESTEP;
}


static void fadingText_displayAll()
{
    int i;

    if( !fadingTextQueue )
        return;

    for( i = 0; i < eaSize( &fadingTextQueue ); i++ )
    {
        fadingText_display( fadingTextQueue[i] );

        if( fadingTextQueue[i]->timer > fadingTextQueue[i]->duration )
        {
            FadingText *ft = eaRemove(&fadingTextQueue, i);
            MP_FREE(FadingText, ft);
            i--;
        }
    }
}


void fadingText_clearAll()
{
    int i;
    for (i = 0; i < eaSize(&fadingTextQueue); i++)
        MP_FREE(FadingText, fadingTextQueue[i]);
    eaSetSize(&fadingTextQueue, 0);
}

//-----------------------------------------------------------------------------------------
// Attention Text
//-----------------------------------------------------------------------------------------

typedef struct AttentionText
{
    char ach[128];
    char achSound[256];
    U32 color;
    int wdw;

    float timer;
} AttentionText;

MP_DEFINE(AttentionText);

AttentionText **attentionTexts = NULL;

void attentionText_add(char *pch, U32 color, int wdw, char *pchSound)
{
    AttentionText *pti;

    MP_CREATE(AttentionText, 32);

    if(!playerPtr())
        return;

    if(!attentionTexts)
        eaCreate(&attentionTexts);

    // If there are already two notifications like this one in a row at 
    //   the end, don't bother adding it. (I originally had this skip 
    //   if there were a couple anywhere in the list, but it seemed
    //   like that might change the user experience enough to worry them.
    //   So, I decided to be a little less aggressive.) --poz
    {
        int i;
        int iCntSame = 0;

        for(i=eaSize(&attentionTexts)-1; i>0; i--)
        {
            if(wdw == attentionTexts[i]->wdw
                && color == attentionTexts[i]->color
                && stricmp(attentionTexts[i]->ach, pch)==0)
            {
                iCntSame++;
            }
            else
            {
                break;
            }
        }

        if(iCntSame>=2)
        {
            // If there's 2 or more of the same at the end, then don't add another.
            return;
        }
    }

    pti = MP_ALLOC(AttentionText);
    strncpyt(pti->ach, pch, 128);

    pti->achSound[0]='\0';
    if(pchSound)
        strncpyt(pti->achSound, pchSound, 256);

    pti->color = color;
    pti->wdw = wdw;
    pti->timer = 0.0f;

    eaPush(&attentionTexts, pti);
}

void attentionText_clearAll(void)
{
    int count;

    for(count = 0; count < eaSize(&attentionTexts); count++)
    {
        MP_FREE(AttentionText, attentionTexts[count]);
    }

    eaDestroy(&attentionTexts);
}

void attentionText_drawAll(void)
{
    if(eaSize(&attentionTexts)>0)
    {
        if(attentionTexts[0]->timer==0.0f)
        {
            int i;
            int w, h;

             int strwd = str_wd(&title_18, 1.0f, 1.0f, attentionTexts[0]->ach);

             windowClientSize(&w, &h);

            if( shell_menu() )
            {
                w = 1024;
                h = 768;
            }
            for(i=0; i<strwd/15; i++)
            {
                electric_initEx(w/2-strwd/2+i*15, h/4, 5000, .40f, 1, 150, attentionTexts[0]->wdw, 0, 0);
            }

            fadingTextEx(w/2, h/4, 5001, 1.0f, attentionTexts[0]->color, attentionTexts[0]->color,
                CENTER_X|CENTER_Y, &title_18, 150, attentionTexts[0]->wdw, attentionTexts[0]->ach, .01f );

            if(attentionTexts[0]->achSound[0])
                sndPlay(attentionTexts[0]->achSound, SOUND_GAME);
        }

        attentionTexts[0]->timer += TIMESTEP;

        if(attentionTexts[0]->timer > 100.0f)
        {
            AttentionText *at = eaRemove(&attentionTexts, 0);
            MP_FREE(AttentionText, at);
        }
    }
}

//-----------------------------------------------------------------------------------------
// Priority Alert Text
//-----------------------------------------------------------------------------------------

typedef struct PriorityAlertText
{
    char ach[128];
    char achSound[256];
    U32 color;
    float x, y;
    float scale;
    float timer;
    float duration;
    int index;
} PriorityAlertText;

MP_DEFINE(PriorityAlertText);

PriorityAlertText **priorityAlertTexts = NULL;

void priorityAlertText_add(char *pch, U32 color, char *pchSound)
{
    PriorityAlertText *pti;

    MP_CREATE(PriorityAlertText, 32);

    if(!playerPtr())
        return;

    if(!priorityAlertTexts)
        eaCreate(&priorityAlertTexts);

    pti = MP_ALLOC(PriorityAlertText);
    strncpyt(pti->ach, pch, 128);

    pti->achSound[0]='\0';
    if(pchSound)
        strncpyt(pti->achSound, pchSound, 256);

    pti->color = color & 0xffffff00;
    pti->timer = 0.0f;
    pti->duration = 120.f;

    eaPush(&priorityAlertTexts, pti);
}

void priorityAlertText_clearAll(void)
{
    int count;

    for(count = 0; count < eaSize(&priorityAlertTexts); count++)
    {
        MP_FREE(PriorityAlertText, priorityAlertTexts[count]);
    }

    eaDestroy(&priorityAlertTexts);
}

static void priorityAlertText_display( PriorityAlertText *pat)
{
    float vec;
    int clr1, clr2;
    float x, y, scale;

    vec = cosf( MIN(1.f, (pat->timer/pat->duration)-0.25f)*PI );

    clr1 = pat->color | (int)((255.f*(vec+1))/2);
    clr2 = pat->color | (int)((255.f*(vec+1))/2);

    x = pat->x;
    y = pat->y;
    scale = pat->scale;

    font( &game_18 );
    font_color( clr1, clr2 );

    cprntEx( x, y, 5001, scale, scale, CENTER_X | CENTER_Y, pat->ach );
}

void priorityAlertText_drawAll(void)
{
    int i;
    int count = 0;
    int NUM_DISPLAYED_ALERTS = 3;
    PriorityAlertText **drawingList = NULL;
    int numAlerts = eaSize(&priorityAlertTexts);
    int w = 0, h = 0;
    int y_offset = 0;
    if (!numAlerts)
    {
        return;
    }

    windowClientSize(&w, &h);
    for (i = 0; i < numAlerts; ++i)
    {
        PriorityAlertText *pat = priorityAlertTexts[i];
        pat->timer += TIMESTEP;
        if ((pat->timer < pat->duration) &&
            (eaSize(&drawingList) < NUM_DISPLAYED_ALERTS))
        {
            if ((numAlerts > NUM_DISPLAYED_ALERTS) && (eaSize(&drawingList) == 0))
            {
                int extra = MAX(numAlerts-NUM_DISPLAYED_ALERTS, 0);
                pat->timer += (extra/(extra+1.f))*(pat->duration-pat->timer);
            }
            eaPushFront(&drawingList, pat);
        }
    }
    for (i = 0; i < eaSize(&drawingList); ++i)
    {
        drawingList[i]->scale = 2.5f * ((NUM_DISPLAYED_ALERTS-i)+3)*1.f/(NUM_DISPLAYED_ALERTS+3);
        drawingList[i]->x = w/2;
        drawingList[i]->y = h*0.4f - y_offset;
        y_offset += (drawingList[i]->scale*18);
        priorityAlertText_display(drawingList[i]);
    }

    for (i = numAlerts-1; i >= 0; --i)
    {
        if(priorityAlertTexts[i]->timer >= priorityAlertTexts[i]->duration)
        {
            PriorityAlertText *at = eaRemove(&priorityAlertTexts, i);
            MP_FREE(PriorityAlertText, at);
        }
    }
    eaDestroy(&drawingList);
}
//------------------------------------------------------------------------------------------------------------
// Driving Icons /////////////////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------------------------------------

typedef struct MovingIcon
{
    AtlasTex * icon;

    float startSc;
    float endSc;

    float startX;
    float startY;

    float endX;
    float endY;

    float z;
    float timer;

} MovingIcon;

MP_DEFINE(MovingIcon);

MovingIcon **movingIcons = {0};

// Adds an icon to the list, safe to add same icon multiple times will simply change the end point
//
void movingIcon_add( AtlasTex *icon, float fromX, float fromY, float fromSc, float toX, float toY, float toSc, float z )
{
    MovingIcon * mi;
    int i;

    MP_CREATE(MovingIcon, 32);

    if( !playerPtr() )
        return;

    mi = MP_ALLOC(MovingIcon);

    if( !movingIcons )
        eaCreate( &movingIcons );

    for( i = 0; i < eaSize(&movingIcons); i++ )
    {
        if( movingIcons[i]->icon == icon )
        {
            movingIcons[i]->endX = toX;
            movingIcons[i]->endY = toY;
            movingIcons[i]->endSc = toSc;
            return;
        }
    }

    mi->icon = icon;
    mi->z = z;

    mi->startX = fromX;
    mi->startY = fromY;
    mi->startSc = fromSc;

    mi->endX = toX;
    mi->endY = toY;
    mi->endSc = toSc;

    eaPush( &movingIcons, mi );
}

void movingIcon_clearAll(void)
{
    int count;

    for(count = 0; count < eaSize(&movingIcons); count++)
    {
        MP_FREE(MovingIcon, movingIcons[count]);
    }

    eaDestroy(&movingIcons);
}

// draws the moving icons
#define ICON_SPEED .1f
static void movingIcon_drawAll()
{
    int i;

    if( !movingIcons )
        return;

    for( i = 0; i < eaSize(&movingIcons); i++ )
    {
        MovingIcon *mi = movingIcons[i];
        float x, y, scale;

        mi->timer += ICON_SPEED*TIMESTEP;

        // duration is fixed atm
        if( mi->timer > 1.f || !mi->icon )
        {
            MovingIcon *mi = eaRemove( &movingIcons, i );
            MP_FREE(MovingIcon, mi);
            i--;
            continue;
        }

        scale = mi->endSc + (mi->startSc - mi->endSc)*(1-mi->timer);
        x = mi->endX - (mi->endX - mi->startX)*(1-mi->timer);
        y = mi->endY - (mi->endY - mi->startY)*(1-mi->timer);

        display_sprite( mi->icon, x - mi->icon->width*scale/2, y - mi->icon->height*scale/2, mi->z, scale, scale, CLR_WHITE );
    }

}


void fx_doAll()
{
    if( !playerPtr() )
        return;

    attentionText_drawAll();
    priorityAlertText_drawAll();
     movingIcon_drawAll();
    fadingText_displayAll();
    gElectricAction = electric_drawAll();
}
