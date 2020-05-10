/***************************************************************************
 *     Copyright (c) 2000-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include <stdarg.h>
#include <utilitieslib/components/estring.h>

#include "graphics/ttFont.h"
#include <utilitieslib/language/MessageStore.h> // for msvaPrintf
#include <utilitieslib/utils/StringUtil.h>   // for UTF8ToWideStrConvert
#include "graphics/ttFontUtil.h"   // for printBasic
#include "language/langClientUtil.h"    // for menuMessages
#include <utilitieslib/utils/mathutil.h>
#include "UI/uiGame.h"
#include "win/win_init.h"
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/utils/strings_opt.h>
#include <utilitieslib/utils/utils.h>

#include "UI/sprite/sprite_base.h"
#include "UI/sprite/sprite_font.h"
#include "UI/sprite/sprite_text.h"
#include "assert.h"
#include <utilitieslib/language/AppLocale.h>
#include <utilitieslib/language/MessageStoreUtil.h>
#include "storyarc/playerCreatedStoryarcValidate.h"
#include "player/player.h"
#include "entity/entity.h"
#include "entity/EntPlayer.h"
#include "auth/authUserData.h"
#include "cmdparse/cmdgame.h"

typedef enum
{
    LQC_topLeft,
    LQC_topRight,
    LQC_bottomRight,
    LQC_bottomLeft,
} LFQuadRGBA;

int font_rgba[4];
TTDrawContext *font_grp;

static int playerPrependFlags(int flags)
{
    Entity *e = playerPtr();

    if(e && !(flags&NO_PREPEND))
    {
        if(ENT_IS_IN_PRAETORIA(e))
        {
            if(ENT_IS_VILLAIN(e))
                flags|=PREPEND_L_P;
            else
                flags|=PREPEND_P;
        }
    }

    return flags;
}

// TODO: Really should be text_color
void font_color(int clr0, int clr1)
{
    font_rgba[LQC_topLeft] = font_rgba[LQC_topRight] = clr0;
    font_rgba[LQC_bottomLeft] = font_rgba[LQC_bottomRight] = clr1;
}

int font_color_get_top()
{
    return font_rgba[LQC_topLeft];
}

int font_color_get_bottom()
{
    return font_rgba[LQC_bottomLeft];
}

void font_ital( int on )
{
    font_grp->renderParams.italicize = on;
}

void font_bold( int on )
{
    font_grp->renderParams.bold = on;
}

void font_outl( int on )
{
    font_grp->renderParams.outline = on;
}

void font_ds( int on )
{
    font_grp->renderParams.dropShadow = on;
}

// TODO: Really should be text_font
void font(TTDrawContext * f)
{
    font_grp = f;
}

TTDrawContext* font_get()
{
    return font_grp;
}

void font_set( TTDrawContext * font, int ital, int bold, int outline, int dropShadow, int clr0, int clr1 )
{
    font_grp = font;
    font_grp->renderParams.italicize    = ital;
    font_grp->renderParams.bold            = bold;
    font_grp->renderParams.outline        = outline;
    font_grp->renderParams.dropShadow    = dropShadow;
    font_color(clr0, clr1);
}

void fixFuckedApostrophes(char *s)
{
    while (s && *s)
    {
        if ((int) *s == -110)
            *s = '\'';
        s++;
    }
}

void determineTextColor(int rgba[4])
{
    // Decide what color the text is going to be.
    rgba[QC_bottomLeft] = font_rgba[LQC_bottomLeft];
    rgba[QC_topLeft] = font_rgba[LQC_topLeft];
    rgba[QC_topRight] = font_rgba[LQC_topRight];
    rgba[QC_bottomRight] = font_rgba[LQC_bottomRight];

}


static void vprnt(float x, float y, float z, float xsc, float ysc, float wd, int flags, const char *str, va_list args)
{
    static char  *buffer = NULL;
    int rgba[4];
    static int buffersize = 0;
    int translate = !(flags & NO_MSPRINT);
    int message_len;

    if (!str)
        return;

    if( translate )
        message_len = msvaPrintf(menuMessages, NULL, 0, str, args);
    else
        message_len = _vscprintf(str, args);

    if( buffersize < (int)(sizeof(char) * (MAX( (int)strlen(str), message_len )+1)) )
    {
        buffersize = (sizeof(char) * (MAX( (int)strlen(str), message_len  )+1));
        buffer = realloc( buffer, buffersize );
    }

    determineTextColor(rgba);

    if( flags & NO_MSPRINT )
        vsprintf_s(buffer, buffersize, str, args);
    else
    {
        int pflags = 0;

        if( flags & NO_PREPEND )
            pflags = TRANSLATE_NOPREPEND;
        else
        {
            if( flags & PREPEND_P )
                pflags |= TRANSLATE_PREPEND_P;
            if( flags & PREPEND_L_P )
                pflags |= TRANSLATE_PREPEND_L_P;
        }

        msvaPrintfInternalEx(menuMessages, buffer, buffersize, str, NULL, NULL, 0, pflags, args);
    }

    if(flags & FIT_WIDTH)
    {
        float strwd = str_wd(font_grp, xsc, ysc, buffer);
        applyToScreenScalingFactorf(&wd, NULL);
        if(strwd > wd)
        {
            xsc *= wd / strwd;
            ysc *= wd / strwd;
        }
    }

    printBasic(font_grp, x, y, z, xsc, ysc, flags, buffer, UTF8GetLength(smf_DecodeAllCharactersGet(buffer)), rgba);
}


int commandParameterStrcmp( char * command_key, char * string )
{
    char *postfix;
    char base_name[1024];

    if( !command_key || !string )
        return 0;
    if( stricmp( textStd(command_key), string ) == 0 )
        return 1;

    postfix = strstri( command_key, "CommandParameter");
    if( postfix )
    {
        strncpyt(base_name, command_key, postfix-command_key );
        if( stricmp( base_name, string ) == 0 )
            return 1;
    }

    return 0;
}

int commandParameterStrstr( char * command_key, char * string )
{
    char base_name[1024]; 
    char *postfix;

    if( !command_key || !string )
        return 0;
    if( strstri( textStd( command_key ), string ) )
        return 1;

    postfix = strstri( command_key, "CommandParameter");
    if( postfix )
    {
        strncpyt(base_name, command_key, postfix-command_key+1 );
        if( strstri( base_name, string ) )
            return 1;
    }

    return 0;
}




void prnt_raw(float x, float y, float z, float xsc, float ysc, const char * str)
{
    int rgba[4];
    char * fixed = strcpy(_alloca(strlen(str)+1), str);

    fixFuckedApostrophes(fixed);
    determineTextColor(rgba);
    printBasic(font_grp, x, y, z, xsc, ysc, 0, fixed, strlen(fixed), rgba);
}

/* Function prnt
 *  This function prints the entire string at the given coordinates using the given scale.
 *  Note that the scale value should be specified relative to a 640x480 screen.  Screen/resolution
 *  resizes will be automatically taken into consideration.
 */
void prnt(float x, float y, float z, float xsc, float ysc, const char * str, ...)
{
    if(str && str[0])
    {
        VA_START(va, str);
        vprnt(x, y, z, xsc, ysc, 0, 0, str, va);
        VA_END();
    }
}

void prntEx(float x, float y, float z, float xsc, float ysc, int flags, const char * str, ...)
{
    if(str && str[0])
    {
        VA_START(va, str);
        vprnt(x, y, z, xsc, ysc, 0, flags, str, va);
        VA_END();
    }
}

void cprnt(float x, float y, float z, float xsc, float ysc, const char * str, ...)
{
    if(str && str[0])
    {
        int pflags;
        pflags = playerPrependFlags(CENTER_X);

        VA_START(va, str);
        vprnt(x, y, z, xsc, ysc, 0, pflags, str, va);
        VA_END();
    }
}

void cprntEx(float x, float y, float z, float xsc, float ysc, int flags, const char * str, ...)
{
    if(str && str[0])
    {
        int pflags;
        pflags = playerPrependFlags(flags & ~FIT_WIDTH);

        VA_START(va, str);
        vprnt(x, y, z, xsc, ysc, 0, pflags, str, va);
        VA_END();
    }
}

void cprntFittedEx(float x, float y, float z, float xsc, float ysc, float wd, int flags, const char * str, ...)
{
    if(str && str[0] && wd > 0)
    {
        int pflags;
        pflags = playerPrependFlags(flags | FIT_WIDTH);

        VA_START(va, str);
        vprnt(x, y, z, xsc, ysc, wd, pflags, str, va);
        VA_END();
    }
}

//
//
//
int str_wd(TTDrawContext * font, float xscale, float yscale, const char  * str, ...)
{
    va_list va;
    char  *buffer;
    int wd = 0;

    if( !str )
        return 0;

    va_start(va, str);
     buffer = vaTranslate( str, va );
    va_end(va);

     strDimensionsBasic(font, xscale, yscale, buffer, strlen(buffer), &wd, NULL, NULL);

    if( shell_menu() )
    {
        float xsc, ysc;
        getToScreenScalingFactor(&xsc, &ysc);
         return wd/xsc;
    }
    else
        return wd;
}

int str_wd_notranslate(TTDrawContext * font, float xscale, float yscale, const char * str)
{
    int wd = 0;

    if( !str )
        return 0;

    strDimensionsBasic(font, xscale, yscale, str, strlen(str), &wd, NULL, NULL);

    if( shellMode() && !is_scrn_scaling_disabled())
    {
        float xsc, ysc;
        getToScreenScalingFactor(&xsc, &ysc);
        return wd/xsc;
    }
    else
        return wd;
}

int str_layoutWidth(TTDrawContext* font, float xScale, float yScale, const char * str, ...)
{
    va_list va;
    char  *buffer;
    int wd = 0;

    va_start(va, str);
    buffer = vaTranslate( str, va );
    va_end(va);

    strDimensionsBasic(font, xScale, yScale, buffer, strlen(buffer), NULL, NULL, &wd);
    return wd;
}
float str_sc( TTDrawContext * font, float avail_wd, const char * str, ...)
{
    va_list va;
    char  *buffer;
    int wd = 0;

    if( avail_wd < 1 )
        return 0.f;

    if( shellMode() && !is_scrn_scaling_disabled() )
    {
        F32 xsc, ysc;
        calc_scrn_scaling(&xsc, &ysc);
        avail_wd *= xsc;
    }

    va_start(va, str);
    buffer = vaTranslate( str, va );
    va_end(va);

     strDimensionsBasic(font, 1.f, 1.f, buffer, strlen(buffer), &wd, NULL, NULL);
    //if(avail_wd<wd)
    {
        float sc = avail_wd/wd;
        int tries = 0;
        strDimensionsBasic(font, sc, sc, buffer, strlen(buffer), &wd, NULL, NULL);
        while(wd > avail_wd && tries<20)
        {
            tries++;
            sc = sc * avail_wd/wd;
            strDimensionsBasic(font, sc, sc, buffer, strlen(buffer), &wd, NULL, NULL);
        }

        return sc;
    }

    return 1.0; 
}

/***************************************************************************
 * Function str_dims()
 *
 *  This function is used to determine the bounding rectangle of a string.
 *  Pass the function the coordinates where the string should be on the screen
 *  in screen coordinates as "lx" and "ly".  When the function returns, lx and
 *  ly should specify the upper left point and hx and hy should specify the
 *  lower right point of the bounding rectangle.
 *
 */

void str_dims_notranslate(TTDrawContext * font, float xscale, float yscale, int center_text, CBox * box, const char * str, ...)
{
    va_list va;

    va_start(va, str);
    vstr_dims(font,xscale,yscale,center_text,box,str,1,va);
    va_end(va);
}


void str_dims(TTDrawContext * font, float xscale, float yscale, int center_text, CBox * box, const char * str, ...)
{
    va_list va;
    
    va_start(va, str);
    vstr_dims(font,xscale,yscale,center_text,box,str, 0, va);
    va_end(va);
}

void vstr_dims(TTDrawContext * font, float xscale, float yscale, int center_text, CBox * box, const char *str, int noTranslate, va_list va)
{
    char  buffer[512];
    unsigned short wBuffer[ARRAY_SIZE(buffer)];
    char  *ptr = (char  *) (&buffer[0]);
    int strLength;
    float base_x, base_y;
    int height, width;

    base_x = box->hx = box->lx;
    base_y = box->hy = box->ly;

    if( noTranslate )
        strncpy( buffer, str, 500);
    else
        msvaPrintf(menuMessages, SAFESTR(buffer), str, va);

    strLength = UTF8ToWideStrConvert(buffer, wBuffer, ARRAY_SIZE(buffer));
    ttGetStringDimensionsWithScaling(font, xscale, yscale, wBuffer,
        strLength, &width, &height, NULL, true);

    box->hx = box->lx + width;
    box->hy = box->ly;
    box->ly -= height;

    if (center_text)
    {
        float centerXScale = 1.0;
        float centerYScale = 1.0;
        float half = ((box->hx - box->lx) / 2);
        applyToScreenScalingFactorf(&centerXScale, &centerYScale);

        if(center_text & CENTER_X)
        {
            box->lx = box->lx - half * 1/centerXScale;
            box->hx = box->hx - half * 1/centerXScale;
        }
        if(center_text & CENTER_Y)
        {
            box->ly = box->ly - half * 1/centerYScale;
            box->hy = box->hy - half * 1/centerYScale;
        }
    }

}

float str_height(TTDrawContext * font, float xscale, float yscale, int center_text, const char * str, ...)
{
    F32 res = 0.f;

    if( verify( font && str ))
    {
        CBox box = {0};
        va_list va;
        
        va_start(va, str);
        vstr_dims(font,xscale,yscale,center_text,&box,str,0,va);
        va_end(va);

        res = box.bottom - box.top;
    }
    // --------------------
    // finally

    return res;
}

// *********************************************************************************
//  Game locale utility functions
// *********************************************************************************

// "Thu 8/29"
char* timerFriendlyDateFromSS2(char *datestr, int datestrLength, U32 seconds)
{
    S64            x;
    SYSTEMTIME    t;
    //FILETIME    local;
    char        buffer[50];

    x = seconds;
    x *= 10000000;
    x += y2kOffset();

    //MW this is accounted for with localTimeFromServer that takes into account system clock 
    //and time zone settings. (Maybe we only want to use timezone settings?  If so, change timing_s2000_delta
    //FileTimeToLocalFileTime((FILETIME*)&x,&local);
    FileTimeToSystemTime((FILETIME*)&x, &t);

    switch (t.wDayOfWeek)
    {
        xcase 0: msPrintf(menuMessages, datestr, datestrLength, "SundayAbbrev");
        xcase 1: msPrintf(menuMessages, datestr, datestrLength, "MondayAbbrev");
        xcase 2: msPrintf(menuMessages, datestr, datestrLength, "TuesdayAbbrev");
        xcase 3: msPrintf(menuMessages, datestr, datestrLength, "WednesdayAbbrev");
        xcase 4: msPrintf(menuMessages, datestr, datestrLength, "ThursdayAbbrev");
        xcase 5: msPrintf(menuMessages, datestr, datestrLength, "FridayAbbrev");
        xcase 6: msPrintf(menuMessages, datestr, datestrLength, "SaturdayAbbrev");
    }

    // todo, switch date format based on locale?
    STR_COMBINE_BEGIN(buffer);
        STR_COMBINE_CAT(" ");
        STR_COMBINE_CAT_D(t.wMonth);
        STR_COMBINE_CAT_C('/');
        STR_COMBINE_CAT_D(t.wDay);
    STR_COMBINE_END();
    
    strcpy_s(datestr + strlen(datestr), datestrLength - strlen(datestr), buffer);

    return datestr;
}

char* timerFriendlyHour(char* datestr, int datestrLength, int hour, int min, int showMin)
{
    bool        pm;
    char        ampm[100];
    int locale = getCurrentLocale();

    if (hour < 0) hour *= -1;
    hour %= 24;

    if (locale == LOCALE_ID_FRENCH || locale == LOCALE_ID_GERMAN || locale == LOCALE_ID_KOREAN)
    {
        sprintf_s(datestr, datestrLength, "%02d:%02d",hour,showMin?min:0);
    }
    else 
    {
        pm = hour >= 12;

        if (hour == 0)
            hour = 24;
        if (hour > 12)
            hour -= 12;

        // todo, switch time format based on locale?
        msPrintf(menuMessages, SAFESTR(ampm), pm? "PMTime": "AMTime");
        STR_COMBINE_BEGIN_S(datestr, datestrLength)
        STR_COMBINE_CAT_D(hour);
        if (showMin)
        {
            STR_COMBINE_CAT_C(':');
            if (min < 10)
                STR_COMBINE_CAT_C('0');
            STR_COMBINE_CAT_D(min);
        }
        STR_COMBINE_CAT(ampm);
        STR_COMBINE_END();
    }

    return datestr;
}

static SYSTEMTIME systemTimeFromSS2(U32 seconds, int rounded)
{
    S64            x;
    SYSTEMTIME    t;
    //FILETIME    local;

    x = seconds;
    if (rounded)
        x += 29;    // fudge this a bit if we are trying to round to an even hour
    x *= 10000000;
    x += y2kOffset();

    //MW this is accounted for with localTimeFromServer that takes into account system clock 
    //and time zone settings. (Maybe we only want to use timezone settings?  If so, change timing_s2000_delta
    //FileTimeToLocalFileTime((FILETIME*)&x,&local);
    //FileTimeToLocalFileTime((FILETIME*)&x,&local);
    FileTimeToSystemTime((FILETIME*)&x, &t);

    if (t.wSecond >= 30)
        t.wMinute++;
    if (t.wMinute >= 60)
    {
        t.wMinute -= 60;
        t.wHour++;
    }
    return t;
}

char* timerFriendlyHourFromSS2(char* datestr, int datestrLength, U32 seconds, int rounded, int showMin)
{
    SYSTEMTIME t = systemTimeFromSS2(seconds, rounded);
    return timerFriendlyHour(datestr, datestrLength, t.wHour, t.wMinute, showMin);
}

// Returns the hour as an int that would result from timerFriendlyHourFromSS2()
int timerHourFromSS2Value(U32 seconds, int rounded)
{
    SYSTEMTIME t = systemTimeFromSS2(seconds, rounded);
    return t.wHour;
}

/* End of File */
