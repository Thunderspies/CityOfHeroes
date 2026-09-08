//
// compass.c - draws a compass on the screen
//
//-------------------------------------------------------------------


#include "UI/uiUtil.h"
#include "UI/uiWindows.h"
#include "UI/uiUtilGame.h"
#include "uiHelpButton.h"
#include "UI/uiCompass.h"

#include "UI/sprite/sprite_base.h"
#include "UI/sprite/sprite_text.h"
#include "UI/sprite/sprite_font.h"
#include "graphics/ttFontUtil.h"
#include "graphics/textureatlas.h"
#include <utilitieslib/network/net_packet.h>
#include <utilitieslib/utils/timing.h>
#include "player/player.h"
#include "entity/entity.h"
#include <utilitieslib/components/earray.h>
#include "gameComm/wdwbase.h"
#include <utilitieslib/language/AppLocale.h>
#include <utilitieslib/language/MessageStoreUtil.h>
#include <utilitieslib/utils/SuperAssert.h>
#include "UI/uiInput.h"
#include "UI/uiCursor.h"
#include "entity/entclient.h"

#include "UI/uiWindows_init.h"
#include "UI/uiScript.h"

// Control whether the script UI is within the compass or detached in its own window
int scriptUIIsDetached = 0;

static int atox(char *s)
{
    int value;

    if (sscanf(s, "%x", &value) != 1)
    {
        value = 0;
    }
    return value;
}

static void    scriptUIDrawColoredMeter(float x, float y, float z, float wd, float sc, float filled, 
                                        AtlasTex *default_L, AtlasTex *default_M, AtlasTex *default_R,
                                        AtlasTex *glow_L, AtlasTex *glow_M, AtlasTex *glow_R, AtlasTex *edge)
{
    float avail;
    float barwd;
    float filledwd;

    avail = wd - 20 * sc;
    if (avail < 30)
    {
        // The window will never be this narrow, but even so, the code below will fail if this happens,  So we punt.
        return;
    }
    barwd = avail - 22.0f * sc;
    filledwd = avail * filled - 11.0f * sc;

    if (filledwd <= 0.0f)
    {
        display_sprite(default_L,    x + 10.0f * sc,                y, z, sc,                                sc, CLR_WHITE);
        display_sprite(default_M,    x + 21.0f * sc,                y, z, barwd / 2.0f,                        sc, CLR_WHITE);
        display_sprite(default_R,    x + 21.0f * sc + barwd,        y, z, sc,                                sc, CLR_WHITE);
    }
    else if (filledwd >= barwd)
    {
        display_sprite(glow_L,        x + 10.0f * sc,                y, z, sc,                                sc, CLR_WHITE);
        display_sprite(glow_M,        x + 21.0f * sc,                y, z, barwd / 2.0f,                        sc, CLR_WHITE);
        display_sprite(glow_R,        x + 21.0f * sc + barwd,        y, z, sc,                                sc, CLR_WHITE);
    }
    else
    {
        display_sprite(glow_L,        x + 10.0f * sc,                y, z, sc,                                sc, CLR_WHITE);
        display_sprite(glow_M,        x + 21.0f * sc,                y, z, filledwd / 2.0f,                    sc, CLR_WHITE);
        display_sprite(default_M,    x + 21.0f * sc + filledwd,    y, z, (barwd - filledwd) / 2.0f,        sc, CLR_WHITE);
        display_sprite(default_R,    x + 21.0f * sc + barwd,        y, z, sc,                                sc, CLR_WHITE);
        display_sprite(edge,        x + 20.0f * sc + filledwd ,    y, z, sc,                                sc, CLR_WHITE);
    }
}

// void ScriptUIMeter(WIDGET widgetName, STRING meterVar, STRING meterName, STRING showValues, STRING min, STRING max, STRING color, STRING color2, STRING tooltip)
static float scriptUIDrawDetachedMeter(ScriptUIClientWidget* widget, float x, float y, float z, float wd, float sc, int showLarge)
{
    float meterVar;
    char *meterName;
    int showValues;
    float minValue;
    float maxValue;
    int color;
    int color2;
    char *tooltip;

    meterVar = atof(widget->varList[0]);
    meterName = widget->varList[1];
    showValues = atoi(widget->varList[2]);
    minValue = atof(widget->varList[3]);
    maxValue = atof(widget->varList[4]);
    color = atox(widget->varList[5]);
    color2 = atox(widget->varList[6]);
    tooltip = widget->varList[7];

    if (meterVar < minValue)
    {
        meterVar = minValue;
    }
    else if (meterVar > maxValue)
    {
        meterVar = maxValue;
    }

    font_color(CLR_WHITE, CLR_WHITE);
    font(&game_12);
    prnt(x + 10 * sc, y + 12 * sc, z, sc, sc, "%s: %.1f [ %.1f .. %.1f ]", meterName, meterVar, minValue, maxValue);

    return 15 * sc;
}

//void ScriptUITimer(WIDGET widgetName, STRING timerVar, STRING timerText, STRING tooltip)
static float scriptUIDrawDetachedTimer(ScriptUIClientWidget* widget, float x, float y, float z, float wd, float sc, int showLarge)
{
    int endTime;
    char *timerText;
    char *tooltip;
    int timeLeft;
    char *timerString;

    endTime = atoi(widget->varList[0]);
    timerText = widget->varList[1];
    tooltip = widget->varList[2];
    timeLeft = endTime - timerSecondsSince2000WithDelta();
    if (timeLeft < 0)
    {
        timeLeft = -timeLeft;
    }
    timerString = calcTimerString(timeLeft, widget->showHours);

    font_color(CLR_WHITE, CLR_WHITE);
    font(&game_12);

    prnt(x + 15 * sc, y + 16 * sc, z, sc, sc, "%s", timerText);

    prnt(x + wd - 20 * sc - str_wd(&game_12, sc, sc, timerString), y + 16 * sc, z, sc, sc, timerString);

    return 23 * sc;
}

//void ScriptUICollectItems(WIDGET widgetName, NUMBER numCollectItems, STRING itemVar, STRING itemText, STRING itemImage, ...)
static float scriptUIDrawDetachedCollectItems(ScriptUIClientWidget* widget, float x, float y, float z, float wd, float sc, int showLarge)
{
    int i;
    int n;
    char buff[256];

    n = eaSize(&widget->varList);
    buff[0] = 0;
    for (i = 0; i < n; i += 3)
    {
        if (atoi(widget->varList[i]))
        {
            strcat(buff, "[*]");
        }
        else
        {
            strcat(buff, "[ ]");
        }
        if (i < n - 3)
        {
            strcat(buff, "    ");
        }
    }

    font_color(CLR_WHITE, CLR_WHITE);
    font(&game_12);
    prnt(x + 10 * sc, y + 12 * sc, z, sc, sc, "%s", buff);

    return 15 * sc;
}

//void ScriptUITitle(WIDGET widgetName, STRING titleVar, STRING infoText)
static float scriptUIDrawDetachedTitleBar(ScriptUIClientWidget* widget, float x, float y, float z, float wd, float sc, int showLarge)
{
    char *title;

    title = widget->varList[0];

    font_color(CLR_WHITE, CLR_WHITE);
    font(&game_14);
    prnt(x + 10 * sc, y + 12 * sc, z, sc, sc, "%s", title);

    return 18 * sc;
}

//void ScriptUIList(WIDGET widgetName, STRING tooltip, STRING item, STRING itemColor, STRING value, STRING valueColor)
static float scriptUIDrawDetachedList(ScriptUIClientWidget* widget, float x, float y, float z, float wd, float sc, int showLarge)
{
    char *text;
    char *value;

    text = widget->varList[1];
    value = widget->varList[3];

    font_color(CLR_WHITE, CLR_WHITE);
    font(&game_12);
    prnt(x + 10 * sc, y + 12 * sc, z, sc, sc, "%s %s", text, value);

    return 15 * sc;
}

#define    FEET_IN_A_YARD    3.0f
#define    FEET_IN_A_MILE    5280.0f
#define FEET_IN_A_METER 3.281f

//void ScriptUIDistance(WIDGET widgetName, STRING tooltip, STRING item, STRING itemColor, STRING value, STRING valueColor)
static float scriptUIDrawDetachedDistance(ScriptUIClientWidget* widget, float x, float y, float z, float wd, float sc, int showLarge)
{
    char *item;
    Vec3 targetPos;
    Vec3 playerPos;
    Entity *player;
    float dist;
    char *distance;

    item = widget->varList[1];
    player = playerPtr();
    sscanf(widget->varList[3], "%f,%f,%f", &targetPos[0], &targetPos[1], &targetPos[2]);
    if (player)
    {
        playerPos[0] = ENTPOSX(player);
        playerPos[1] = ENTPOSY(player);
        playerPos[2] = ENTPOSZ(player);
        
        dist = distance3(playerPos, targetPos);
    }
    else
    {
        dist = 0.0f;
    }

    if (getCurrentLocale())
    {
        // convert feet to meters
        dist /= FEET_IN_A_METER;

        if (dist < 1000.0f)
        {
            distance = textStd("MetersString", dist);
        }
        else
        {
            distance = textStd("KiloMetersString", dist / 1000);
        }
    }
    else
    {
         if (dist < 100)
        {
            distance = textStd("FeetString", (int) dist);
        }
        else if (dist < FEET_IN_A_MILE / 2)
        {
            distance = textStd("YardsString", (int) (dist / FEET_IN_A_YARD));
        }
        else
        {
            distance = textStd("MilesString", (dist / FEET_IN_A_MILE));
        }
    }

    font_color(CLR_WHITE, CLR_WHITE);
    font(&game_12);
    prnt(x + 10 * sc, y + 12 * sc, z, sc, sc, "%s: %s", item, distance);

    return 15 * sc;
}

static float scriptUIDrawDetachedMeterEx(ScriptUIClientWidget* widget, float x, float y, float z, float wd, float sc, int showLarge, float longestText)
{
    char *text;
    char *shortText;
    float current;
    float target;
    char *color;
    char *tooltip;
    int targetID;
    float returnHt = showLarge ? 38*sc : 20*sc;

    AtlasTex *default_L;
    AtlasTex *default_M;
    AtlasTex *default_R;
    AtlasTex *glow_L;
    AtlasTex *glow_M;
    AtlasTex *glow_R;
    AtlasTex *edge;
    AtlasTex *bullet;
    char numbers[120];    // safe on anything up to and including a 64 bit machine to hold %d/%d
    int reticleColor = 0;

    text = widget->varList[0];
    shortText = widget->varList[1];
    current = (float) atoi(widget->varList[2]);
    target = (float) atoi(widget->varList[3]);
    color = widget->varList[4];
    tooltip = widget->varList[5];
    targetID = atoi(widget->varList[6]);
    if (stricmp(color, "red") == 0)
    {
        default_L = atlasLoadTexture("ZoneEvent_ProgressBar_Default_Red_L");
        default_M = atlasLoadTexture("ZoneEvent_ProgressBar_Default_Red_Mid");
        default_R = atlasLoadTexture("ZoneEvent_ProgressBar_Default_Red_R");
        glow_L = atlasLoadTexture("ZoneEvent_ProgressBar_Glow_Red_L");
        glow_M = atlasLoadTexture("ZoneEvent_ProgressBar_Glow_Red_Mid");
        glow_R = atlasLoadTexture("ZoneEvent_ProgressBar_Glow_Red_R");
        edge = atlasLoadTexture("ZoneEvent_ProgressBar_Glow_Red_Edge");
        bullet = atlasLoadTexture("ZoneEvent_Bullet_Red");
        reticleColor = CLR_RED;
    }
    else if (stricmp(color, "gold") == 0)
    {
        default_L = atlasLoadTexture("ZoneEvent_ProgressBar_Default_Gold_L");
        default_M = atlasLoadTexture("ZoneEvent_ProgressBar_Default_Gold_Mid");
        default_R = atlasLoadTexture("ZoneEvent_ProgressBar_Default_Gold_R");
        glow_L = atlasLoadTexture("ZoneEvent_ProgressBar_Glow_Gold_L");
        glow_M = atlasLoadTexture("ZoneEvent_ProgressBar_Glow_Gold_Mid");
        glow_R = atlasLoadTexture("ZoneEvent_ProgressBar_Glow_Gold_R");
        edge = atlasLoadTexture("ZoneEvent_ProgressBar_Glow_Gold_Edge");
        bullet = atlasLoadTexture("ZoneEvent_Bullet_Gold");
        reticleColor = CLR_YELLOW;
    }
    else
    {
        default_L = atlasLoadTexture("ZoneEvent_ProgressBar_Default_Blue_L");
        default_M = atlasLoadTexture("ZoneEvent_ProgressBar_Default_Blue_Mid");
        default_R = atlasLoadTexture("ZoneEvent_ProgressBar_Default_Blue_R");
        glow_L = atlasLoadTexture("ZoneEvent_ProgressBar_Glow_Blue_L");
        glow_M = atlasLoadTexture("ZoneEvent_ProgressBar_Glow_Blue_Mid");
        glow_R = atlasLoadTexture("ZoneEvent_ProgressBar_Glow_Blue_R");
        edge = atlasLoadTexture("ZoneEvent_ProgressBar_Glow_Blue_Edge");
        bullet = atlasLoadTexture("ZoneEvent_Bullet_Blue");
        reticleColor = CLR_PARAGON;
    }

    if (target <= 0.0f)
    {
        target = 1.0f;
    }
    if (current < 0.0f)
    {
        current = 0.0f;
    }
    else if (current > target)
    {
        current = target;
    }

    font(&game_12);
    font_color(CLR_WHITE, CLR_WHITE);

    if (targetID)
    {
        AtlasTex *angleBracket;
        AtlasTex *angleBracketGlow;
        CBox cbox;
        float bracketSc = 0.5f *sc;
        float spacer = 4*sc;
        returnHt += 20.f*sc;
        BuildCBox(&cbox, x, y, wd, returnHt);
        if (mouseCollision(&cbox))
        {
            angleBracket = atlasLoadTexture("targeted_bracket_over.tga");
            angleBracketGlow = atlasLoadTexture("targeted_bracket_over_glow.tga");
        }
        else
        {
            angleBracket = atlasLoadTexture("targeted_bracket.tga");
            angleBracketGlow = atlasLoadTexture("targeted_bracket_glow.tga");
            reticleColor &= 0xffffff99;
        }

        //    upperleft
        display_rotated_sprite(angleBracket, x+spacer, y+spacer, z+1, bracketSc, bracketSc, reticleColor, RAD(0), 0);
        display_rotated_sprite(angleBracketGlow, x+spacer, y+spacer, z+1, bracketSc, bracketSc, reticleColor, RAD(0), 0);

        //    upper right
        display_rotated_sprite(angleBracket, x+wd - (spacer*2+angleBracket->height*bracketSc), y+spacer, z+1, bracketSc, bracketSc, reticleColor, RAD(90), 0);
        display_rotated_sprite(angleBracketGlow, x+wd - (spacer*2+angleBracketGlow->height*bracketSc), y+spacer, z+1, bracketSc, bracketSc, reticleColor, RAD(90), 0);

        //    lower left
        display_rotated_sprite(angleBracket, x+spacer, y+returnHt- (spacer*2+angleBracket->height*bracketSc), z+1, bracketSc, bracketSc, reticleColor, RAD(270), 0);
        display_rotated_sprite(angleBracketGlow, x+spacer, y+returnHt- (spacer*2+angleBracketGlow->height*bracketSc), z+1, bracketSc, bracketSc, reticleColor, RAD(270), 0);

        //    lower right
        display_rotated_sprite(angleBracket, x+wd-(spacer*2+ angleBracket->height*bracketSc), y+returnHt- (spacer*2+angleBracket->width*bracketSc), z+1, bracketSc, bracketSc, reticleColor, RAD(180), 0);
        display_rotated_sprite(angleBracketGlow, x+wd-(spacer*2 + angleBracketGlow->height*bracketSc), y+returnHt- (spacer*2+angleBracketGlow->width*bracketSc), z+1, bracketSc, bracketSc, reticleColor, RAD(180), 0);

        if (mouseClickHit(&cbox, MS_LEFT))
        {
            if (targetID)
            {
                Entity *ent = entFromId(targetID);
                if (ent && ent != playerPtr())
                    current_target = ent;
            }
        }

        y += 10.f*sc;
    }

    if (showLarge)
    {
        scriptUIDrawColoredMeter(x, y, z, wd, sc, current / target, default_L, default_M, default_R, glow_L, glow_M, glow_R, edge);

        display_sprite(bullet, x + 15 * sc, y + 17 * sc, z, sc * 1.2f, sc * 1.2f, CLR_WHITE);

        prnt(x + 10 * sc + 30 * sc, y + 33 * sc, z, sc, sc, text);
        _snprintf(numbers, sizeof(numbers), "%d/%d", (int) current, (int) target);
        numbers[sizeof(numbers) - 1] = 0;
        prnt(x + wd - 20 * sc - str_wd(&game_12, sc, sc, numbers), y + 33 * sc, z, sc, sc, numbers);

    }
    else
    {
        scriptUIDrawColoredMeter(x + longestText, y, z, wd - longestText, sc, current / target, default_L, default_M, default_R, glow_L, glow_M, glow_R, edge);
        prnt(x + 12 * sc, y + 12 * sc, z, sc, sc, shortText);
    }

    return returnHt;
}
static float scriptUIDrawDetachedKarma(ScriptUIClientWidget* widget, float x, float y, float z, float wd, float sc, int showLarge)
{
    char *text;
    int current;
    float target = 100.f;

    AtlasTex *default_L;
    AtlasTex *default_M;
    AtlasTex *default_R;
    AtlasTex *glow_L;
    AtlasTex *glow_M;
    AtlasTex *glow_R;
    AtlasTex *edge;
    AtlasTex *bullet;
    char numbers[120];    // safe on anything up to and including a 64 bit machine to hold %d/%d

    text = widget->varList[0];
    current = atoi(widget->varList[1]);

    default_L = atlasLoadTexture("ZoneEvent_ProgressBar_Default_Blue_L");
    default_M = atlasLoadTexture("ZoneEvent_ProgressBar_Default_Blue_Mid");
    default_R = atlasLoadTexture("ZoneEvent_ProgressBar_Default_Blue_R");
    glow_L = atlasLoadTexture("ZoneEvent_ProgressBar_Glow_Blue_L");
    glow_M = atlasLoadTexture("ZoneEvent_ProgressBar_Glow_Blue_Mid");
    glow_R = atlasLoadTexture("ZoneEvent_ProgressBar_Glow_Blue_R");
    edge = atlasLoadTexture("ZoneEvent_ProgressBar_Glow_Blue_Edge");
    bullet = atlasLoadTexture("ZoneEvent_Bullet_Blue");

    if (target <= 0.0f)
    {
        target = 1.0f;
    }
    if (current < 0.0f)
    {
        current = 0;
    }
    else if (current > target)
    {
        current = target;
    }

    font(&game_12);
    font_color(CLR_WHITE, CLR_WHITE);


    if (showLarge)
    {
        scriptUIDrawColoredMeter(x, y, z, wd, sc, current / target, default_L, default_M, default_R, glow_L, glow_M, glow_R, edge);

        display_sprite(bullet, x + 15 * sc, y + 17 * sc, z, sc * 1.2f, sc * 1.2f, CLR_WHITE);

        prnt(x + 10 * sc + 30 * sc, y + 33 * sc, z, sc, sc, text);
        _snprintf(numbers, sizeof(numbers), "%d/%d", (int) current, (int) target);
        numbers[sizeof(numbers) - 1] = 0;
        prnt(x + wd - 20 * sc - str_wd(&game_12, sc, sc, numbers), y + 33 * sc, z, sc, sc, numbers);

        return 38 * sc;
    }
    else
    {
        scriptUIDrawColoredMeter(x+(125*sc), y, z, wd-(125*sc), sc, current / target, default_L, default_M, default_R, glow_L, glow_M, glow_R, edge);
        prnt(x + 12 * sc, y + 12 * sc, z, sc, sc, text);
        return 20 * sc;
    }
}
static float scriptUIDrawDetachedTextMeter(ScriptUIClientWidget* widget, float x, float y, float z, float wd, float sc, int showLarge)
{
    return 0.0f;
}

static float scriptUIDrawDetachedProgress(ScriptUIClientWidget* widget, float x, float y, float z, float wd, float sc, int showLarge)
{
    return 0.0f;
}

static float scriptUIDrawDetachedText(ScriptUIClientWidget* widget, float x, float y, float z, float wd, float sc, int showLarge)
{
    char *text;
    char *format;
    char *tooltip;
    int centered;
    float tx;

    text = widget->varList[0];
    format = widget->varList[1];
    tooltip = widget->varList[2];
    centered = strnicmp(format, "center", 6 /* == strlen("center") */) == 0;

    font(&game_12);
    font_color(0xffffffff, 0xffffffff);

    if (centered)
    {
        tx = x + (wd - str_wd(&game_12, sc, sc, text)) * 0.5;
        if (tx < x + 20 * sc)
        {
            tx = x + 20 * sc;
        }
    }
    else
    {
        tx = x + 12 * sc;
    }
    prnt(tx, y + 12 * sc, z, sc, sc, text);

    if (tooltip[0])
    {
        scriptUIAddToolTip(widget, x + 10 * sc, y - 13 * sc, wd - 20 * sc, 14 * sc, tooltip);
    }

    return 15 * sc;
}

int scriptUIWindow()
{
    int i;
    int n;
    float drawHt;
    float x, y, z, wd, ht, sc;
    float tx, ty;
    int color, back_color;
    Wdw *myWindow;
    ScriptUIClientWidget *widget;
    char *scriptTitle;
    char *stageTitle;
    char *infoText;
    int endTime;
    int timeLeft;
    char *timerString;
    float longestText;
    AtlasTex *arrow;
    static int showLarge = 1;
    static HelpButton *myHelpButton = NULL;
    static char *cachedInfoText = NULL;
    ScriptUIClientWidget **widgetList = NULL;

    switch (gCurrentWindefIndex)
    {
    case WDW_SCRIPT_UI:
        widgetList = scriptUIWidgetList;
        break;
    case WDW_KARMA_UI:
        widgetList = scriptUIKarmaWidgetList;
        break;
    default:
        assert(0);    //    undefined
        break;
    }
    
    if (!window_getDims(gCurrentWindefIndex, &x, &y, &z, &wd, &ht, &sc, &color, &back_color))
    {
        if (window_getMode(gCurrentWindefIndex) == WINDOW_DOCKED && eaSize(&widgetList) >= 3)
        {
            if ((gCurrentWindefIndex == WDW_KARMA_UI) || scriptUIIsDetached)
                window_setMode(gCurrentWindefIndex, WINDOW_GROWING);
        }
        return 0;
    }

    wd = 350 * sc;
    myWindow = wdwGetWindow(gCurrentWindefIndex);
    myWindow->forceNoFlip = 1;
    myWindow->below = 0;

    drawFrame(PIX3, R10, x, y, z, wd, ht, sc, color, back_color);
    arrow = atlasLoadTexture(showLarge ? "Jelly_arrow_down.tga" : "Jelly_arrow_up.tga");

    n = eaSize(&widgetList);
    if (n < 3 || !((gCurrentWindefIndex == WDW_KARMA_UI) || scriptUIIsDetached))
    {
        if (windowUp(gCurrentWindefIndex))
        {
            window_setMode(gCurrentWindefIndex, WINDOW_SHRINKING);
        }

        return 0;
    }
    else if (window_getMode(gCurrentWindefIndex) == WINDOW_DOCKED || window_getMode(gCurrentWindefIndex) == WINDOW_SHRINKING)
    {
        window_setMode(gCurrentWindefIndex, WINDOW_GROWING);
        return 0;
    }

    // decompose widgets 0 and 1 - these are the event and stage
    widget = widgetList[0];
    assert(widget->type == TitleWidget);
    scriptTitle = widget->varList[0];

    widget = widgetList[1];
    assert(widget->type == TimerWidget || widget->type == ListWidget);
    if (widget->type == TimerWidget)
    {
        endTime = atoi(widget->varList[0]);
        stageTitle = widget->varList[1];
        infoText = widget->varList[2];
        timeLeft = endTime - timerSecondsSince2000WithDelta();
        if (timeLeft < 0)
        {
            timeLeft = -timeLeft;
        }
        timerString = calcTimerString(timeLeft, widget->showHours);

    }
    else
    {
        infoText = widget->varList[0];
        stageTitle = widget->varList[1];
        timerString = NULL;
    }

    tx = x + (myWindow->radius + 12) * sc;
    //if( (!wdw->below && !wdw->flip) || (wdw->below && wdw->flip) )
    ty = y - JELLY_HT * sc * 0.5;
    // ty = y + ht + JELLY_HT * sc * 0.5;

    font(&game_12);
    font_color(0xffffffff, 0xffffffff);
    cprntEx(x + (myWindow->radius + 12) * sc, y - JELLY_HT * sc * 0.5, z + 1, sc, sc, CENTER_Y, scriptTitle);

    if (timerString != NULL)
    {
        // Rather than using the actual string, use one of two templates that format as wide as possible.  Since we're drawing this right aligned,
        // this stops it fluttering as the digit widths change.
        tx = x + wd - (myWindow->radius + arrow->width + 20) * sc - str_wd(&game_12, sc, sc, widget->showHours ? "5:55:55" : "55:55");
        font_color(0xffffffff, 0xffa000ff);
        cprntEx(tx, ty, z + 1, sc, sc, CENTER_Y, timerString);
    }

    tx = x + wd - (myWindow->radius + arrow->width + 12) * sc;
    if (D_MOUSEHIT == drawGenericButton(arrow, tx, y - (JELLY_HT + arrow->height) * sc * 0.5, z + 5, sc, (color & 0xffffff00) | 0x88, (color & 0xffffff00) | 0xff, 1))
    {
        showLarge = !showLarge;
    }

    font_color(0xffffffff, 0xffffffff);
    if (showLarge)
    {
        drawFrame(PIX3, R6, x + 10 * sc, y + 10 * sc, z + 1, wd - 20 * sc, 25, sc, color, back_color);
        cprntEx(x + 20 * sc, y + 22 * sc, z + 1, sc, sc, CENTER_Y, stageTitle);

        if (gCurrentWindefIndex == WDW_SCRIPT_UI)
        {
            // Help Button handling
            if (infoText && infoText[0])
            {
                // Check if we have an info text cached.  If we don't or what's cached is different from the text, blow everything away
                if (cachedInfoText == NULL || strcmp(cachedInfoText, infoText) != 0)
                {
                    // Nuke the button if it exists
                    if (myHelpButton != NULL)
                    {
                        freeHelpButton(myHelpButton);
                        myHelpButton = NULL;
                    }
                    // Nuke the cached text if it exists
                    if (cachedInfoText)
                    {
                        free(cachedInfoText);
                    }
                    // and make a copy of new text
                    cachedInfoText = strdup(infoText);
                }
                // Draw the button.  This handles rebuilding the button if we blew it away above
                helpButtonFullyConfigurable(&myHelpButton, x + wd - 26 * sc, y + 23 * sc, z, sc, infoText, NULL, 0,
                    CLR_WHITE, 0x000060ff, 0x0000c0ff, 0x202020ff, 0xffffffff);
            }
            else
            {
                // Nothing to display.
                // Nuke the button if it exists
                if (myHelpButton != NULL)
                {
                    freeHelpButton(myHelpButton);
                    myHelpButton = NULL;
                }
                // Nuke the cached text if it exists
                if (cachedInfoText)
                {
                    free(cachedInfoText);
                    cachedInfoText = NULL;
                }
            }
        }
        drawHt = 45.0f;
    }
    else
    {
        drawHt = 10.0f;
    }

    // If we're showing the compact version, find the width of the longest text from the meters
    longestText = 0;
    if (!showLarge)
    {
        for (i = 2; i < n; i++)
        {
            widget = widgetList[i];
            if (widget->hidden)
                continue;
            if (widget->type == MeterExWidget)
            {
                tx = str_wd(&game_12, sc, sc, widget->varList[1]);
                if (tx > longestText)
                {
                    longestText = tx + 10 * sc;
                }
            }
        }
    }

    // Now draw everything else
    for (i = 2; i < n; i++)
    {
        widget = widgetList[i];
        if (widget->hidden)
            continue;

        switch (widget->type)
        {
            xcase MeterWidget:
                drawHt += scriptUIDrawDetachedMeter(widget, x, y + drawHt, z, wd, sc, showLarge);
            xcase TimerWidget:
                drawHt += scriptUIDrawDetachedTimer(widget, x, y + drawHt, z, wd, sc, showLarge);
            xcase CollectItemsWidget:
                drawHt += scriptUIDrawDetachedCollectItems(widget, x, y + drawHt, z, wd, sc, showLarge);
            xcase TitleWidget:
                drawHt += scriptUIDrawDetachedTitleBar(widget, x, y + drawHt, z, wd, sc, showLarge);
            xcase ListWidget:
                drawHt += scriptUIDrawDetachedList(widget, x, y + drawHt, z, wd, sc, showLarge);
            xcase DistanceWidget:
                drawHt += scriptUIDrawDetachedDistance(widget, x, y + drawHt, z, wd, sc, showLarge);
            xcase MeterExWidget:
                drawHt += scriptUIDrawDetachedMeterEx(widget, x, y + drawHt, z, wd, sc, showLarge, longestText);
            xcase TextMeterWidget:
                drawHt += scriptUIDrawDetachedTextMeter(widget, x, y + drawHt, z, wd, sc, showLarge);
            xcase ProgressWidget:
                drawHt += scriptUIDrawDetachedProgress(widget, x, y + drawHt, z, wd, sc, showLarge);
            xcase TextWidget:
                drawHt += scriptUIDrawDetachedText(widget, x, y + drawHt, z, wd, sc, showLarge);
            xcase KarmaWidget:
                drawHt += scriptUIDrawDetachedKarma(widget, x, y + drawHt, z, wd, sc, showLarge);
        }            
    }

    window_setDims(gCurrentWindefIndex, -1, -1, wd, drawHt + 10.0f);

    return 0;
}

void detachScriptUIWindow(int detach)
{
    scriptUIIsDetached = !!detach;
    // No need to explicitly open and close here, there's logic in the main int scriptUIWindow() function
    // to handle opening and closing based on the combination of this flag and the presence of script data to show.
#if 0
    int currentMode;

    // trying to detach
    if (detach)
    {
        currentMode = window_getMode(gCurrentWindefIndex);
        if (currentMode == WINDOW_SHRINKING || currentMode == WINDOW_DOCKED)
        {
            window_setMode(gCurrentWindefIndex, WINDOW_GROWING);
        }
        scriptUIIsDetached = 1;
    }
    else
    {
        currentMode = window_getMode(gCurrentWindefIndex);
        if (currentMode == WINDOW_GROWING || currentMode == WINDOW_DISPLAYING)
        {
            window_setMode(gCurrentWindefIndex, WINDOW_SHRINKING);
        }
        scriptUIIsDetached = 0;
    }
#endif
}


////////////////////////////////////////////////////////////////////////////////////
// Receive a command telling us whether to detach or reattach the UI
void scriptUIReceiveDetach(Packet *pak)
{
    detachScriptUIWindow(pktGetBits(pak, 1));
}
