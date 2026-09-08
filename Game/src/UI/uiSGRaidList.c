/*\
 *
 *    uiSGRaidList.h/c - Copyright 2005 Cryptic Studios
 *        All Rights Reserved
 *        Confidential property of Cryptic Studios
 *
 *    Shows the list of active supergroup raids for the player
 *
 */

#include "gameComm/wdwbase.h"
#include "UI/uiUtil.h"
#include "UI/uiUtilGame.h"
#include "UI/uiWindows.h" 
#include "UI/uiComboBox.h"
#include "uiSlider.h"
#include "UI/uiScrollBar.h"
#include "UI/uiContextMenu.h"
#include "UI/uiChatUtil.h"
#include "UI/uiEditText.h"
#include "UI/uiInput.h"
#include "UI/uiReticle.h"
#include "UI/uiCursor.h"
#include "UI/uiFocus.h"
#include "UIEdit.h"
#include "UI/uiGame.h"
#include "UI/uiInfo.h"
#include "uiHelp.h"
#include "UI/uiNet.h"
#include "graphics/textureatlas.h"
#include "UI/uiSGRaidList.h"
#include "graphics/ttFontUtil.h"
#include "graphics/truetype/ttFontDraw.h"
#include "UI/sprite/sprite_base.h"
#include "UI/sprite/sprite_text.h"
#include "UI/sprite/sprite_font.h"
#include "UI/uiSuperRegistration.h"
#include "gameComm/wdwbase.h"
#include "UI/uiClipper.h"
#include "UI/uiDialog.h"

#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/components/MemoryPool.h>
#include <utilitieslib/utils/utils.h>
#include "clientcomm/clientcomm.h"
#include "entity/entVarUpdate.h"
#include "entity/entPlayer.h"
#include "player/player.h"
#include "entity/entity.h"
#include "cmdparse/cmdgame.h"
#include "gameData/raidstruct.h"
#include "gameData/sgraidClient.h"
#include "entity/Supergroup.h"
#include "UI/uiSupergroup.h"
#include "auth/authUserData.h"
#include "clientcomm/dbclient.h"
#include <utilitieslib/language/MessageStoreUtil.h>

#define SGRAID_TOTAL_WD            460
#define SGRAID_BUTTON_HT        40
#define SGRAID_LINE_INDENT        30
#define SGRAID_FACTION_WD        30
#define SGRAID_DATE_WD            75
#define SGRAID_TIME_WD            90
#define SGRAID_STATUS_WD        160
#define SGRAID_PARTBUTTON_WD    60
#define BUTTON_OFFSET            30

#define BORDER_SPACE    15
#define TAB_HEIGHT      18

#define TAB_INSET       20 // amount to push tab in from edge of window
#define BACK_TAB_OFFSET 3  // offset to kee back tab on the bar
#define OVERLAP         5  // amount to overlap tabs

#define TEAMTASK_BUTTON_PANE_HEIGHT 30


#define RADIO_CHECK_WIDTH    14
#define RADIO_Y_OVERLAY        2

int drawRadioButton(float x, float y, float wd, float z, float sc, int flags, char* text)
{
    AtlasTex * mark;
    CBox box;
    float px, py, pogsc = sc;
    float gradient = 0.0;
    U32 pogcolor;
    static float timer = 0.0;
    int ret = D_NONE;
    int highlight;
    U32 fontcolor = CLR_WHITE;

    int locked = flags & BUTTON_LOCKED;
    int checked = flags & BUTTON_CHECKED;

    // color
    pogcolor = winDefs[0].loc.color | (locked? 0x88: 0xff);

    // highlight
    BuildCBox(&box, x, y-RADIO_Y_OVERLAY, wd, RADIO_CHECK_WIDTH + 2*RADIO_Y_OVERLAY);
    highlight = mouseCollision(&box);
    if (highlight && !locked)
    {
        timer += TIMESTEP*.15;
        if (timer > 100.0) timer -= 100.0;
        gradient = sinf( timer )*.4+.6;
        interp_rgba(gradient, pogcolor, 0xffffffff, &pogcolor);
        pogsc += gradient*.10;
        ret = D_MOUSEOVER;
    }

    // pog
    px = x - (pogsc-sc)*RADIO_CHECK_WIDTH/2.0;    // correct for scale to center sprite
    py = y - (pogsc-sc)*RADIO_CHECK_WIDTH/2.0;
    if (checked)
    {
        mark = atlasLoadTexture( "Context_checkbox_filled.tga" );
        display_sprite( mark, px, py, z + 3, pogsc, pogsc, pogcolor );

        mark = atlasLoadTexture( "Context_checkbox_empty.tga" );
        display_sprite( mark, px, py, z + 2, pogsc, pogsc, pogcolor );
    }
    else
    {
        mark = atlasLoadTexture( "Context_checkbox_empty.tga" );
        display_sprite( mark, px, py, z + 2, pogsc, pogsc, pogcolor );
    }

    if (gradient)
        fontcolor = pogcolor;
    font_color(fontcolor, fontcolor);
    cprntEx(x+(R10+RADIO_CHECK_WIDTH)*sc, y+(RADIO_CHECK_WIDTH+2)*sc/2, z, sc, sc, CENTER_Y, text );

    if (mouseClickHit(&box, MS_LEFT))
    {
        if (locked)
            ret = D_MOUSELOCKEDHIT;
        else
            ret = D_MOUSEHIT;
    }

    return ret;
}

// *********************************************************************************
//  SGRaidLine - new UI below here
// *********************************************************************************

static U32 g_selected_sg;
static U32 g_selected_time;
static U32 g_selected_raidid;
static U32 g_selected_raidsize;
static U32 g_selected_raidlength;
static float wx, wy, x, y, wht;
static float z, s_sc;  // defined above, uncomment once other UI is removed
static int color, bcolor;
static U32 player_sg;
static int defending_raids;
static int ignore_info;

int drawSGRaidHeader(char* str)
{
    AtlasTex* mid = atlasLoadTexture("Headerbar_VertDivider_MID.tga");
    AtlasTex* right = atlasLoadTexture( "Headerbar_HorizDivider_R.tga" );
    int dy = 22;
    int wd, start, width;

    if( y < -dy || y > wht )
        return dy;

    font_color(CLR_WHITE, CLR_WHITE);
    wd = str_wd_notranslate(font_get(), s_sc, s_sc, str);
    cprntEx( wx+x+R10*s_sc, wy+y+17*s_sc, z, s_sc, s_sc, NO_MSPRINT, str );
    start = x+wd+2*R10*s_sc;
    width = SGRAID_TOTAL_WD*s_sc-start-R10*s_sc;
    //drawHorizontalLine(wx+start, wy+y+11*sc, , z, sc, color);
    display_sprite( mid, wx+start, wy+y+11*s_sc, z, 3*width/mid->width/4.0, 1.f, color );
    display_sprite( right, wx+start+3*width/4, wy+y+11*s_sc, z, width/right->width/4.0, 1.f, color );

    return dy;
}

int drawSGRaidLine(U32 sgid, U32 raidid, U32 raidsize, int villain_sg, U32 start_time, U32 window_length, char* status, int raid_button, int player_participating)
{
    CBox box;
    int dy = 22;
    U32 foreColor, backColor;
    AtlasTex * dot = NULL;
    char datestr[100];
    int lx = x;

    if( y < -dy || y > wht )
        return dy;

    // selection
    BuildCBox(&box, wx+lx+SGRAID_LINE_INDENT*s_sc, wy+y, (SGRAID_TOTAL_WD - SGRAID_LINE_INDENT*2)*s_sc, 20*s_sc);
    if (mouseClickHit(&box, MS_LEFT))
    {
        g_selected_sg = sgid;
        g_selected_time = start_time;
        g_selected_raidid = raidid;
        g_selected_raidsize = raidsize;
        g_selected_raidlength = window_length;
    }

    // mouse highlight
    if (mouseCollision(&box))
    {
        foreColor = CLR_MOUSEOVER_FOREGROUND;
        backColor = CLR_MOUSEOVER_BACKGROUND;
        drawFlatFrameBox(PIX2, R10, &box, z, foreColor, backColor);
    }

    // selection highlight
    if (sgid == g_selected_sg && start_time == g_selected_time && g_selected_raidid == raidid)
    {
        foreColor = CLR_SELECTION_FOREGROUND;
        backColor = CLR_SELECTION_BACKGROUND;
        drawFlatFrameBox(PIX2, R10, &box, z, foreColor, backColor);
    }

    font( &game_12 );
    font_color(CLR_WHITE, CLR_WHITE);

    // villain/hero
    lx += SGRAID_LINE_INDENT*s_sc;
    if (villain_sg)
        dot = atlasLoadTexture("createsymbol_Villain.tga");
    else
        dot = atlasLoadTexture("createsymbol_Hero.tga");
    if (dot)
    {
        float scale = s_sc * 15.0 / dot->width;
        display_sprite(dot, wx+lx+R10*s_sc, wy+y+3*s_sc, z, scale, scale, CLR_WHITE);
    }
    lx += SGRAID_FACTION_WD*s_sc;

    // Date
    timerFriendlyDateFromSS2(datestr, ARRAY_SIZE(datestr), localTimeFromServer(start_time));
    cprntEx( wx+lx+R10*s_sc, wy+y+17*s_sc, z, s_sc, s_sc, NO_MSPRINT, datestr );
    lx += SGRAID_DATE_WD*s_sc;

    // Time
    timerFriendlyHourFromSS2(datestr, ARRAY_SIZE(datestr), localTimeFromServer(start_time), 0, 1);
    if (window_length)
    {
        int len = strlen(datestr);
        strcat(datestr, " - ");
        timerFriendlyHourFromSS2(datestr + len, ARRAY_SIZE(datestr) - len, localTimeFromServer(start_time + window_length*3600), 0, 1);
    }
    cprntEx( wx+lx+R10*s_sc, wy+y+17*s_sc, z, s_sc, s_sc, NO_MSPRINT, datestr );
    lx += str_wd(font_grp, s_sc, s_sc, datestr )+ R10*s_sc;

    // Status
    cprntEx( wx+lx+R10*s_sc, wy+y+17*s_sc, z, s_sc, s_sc, 0, status );
    lx += SGRAID_STATUS_WD*s_sc;
    if (raid_button)
    {
        if (D_MOUSEHIT == drawStdButton( wx+lx+R10*s_sc, wy+y+10*s_sc, z, SGRAID_PARTBUTTON_WD*s_sc, 20*s_sc, color, 
            player_participating? "RaidButtonDrop": "RaidButtonJoin", 1.f, 0 ))
        {
            if (player_participating)
            {
                START_INPUT_PACKET(pak, CLIENTINP_BASERAID_PARTICIPATE)
                    pktSendBitsAuto(pak, raidid);
                    pktSendBits(pak, 1, !player_participating);
                END_INPUT_PACKET
            }
            else
            {
                dialogStd( DIALOG_SMF, textStd("MustBuyCoVToAccessBase"), 0, 0, 0, 0, 0 );
            }
        }
    }
    return dy;
}

void drawNoRaidLine(char* text)
{
    int dy = 22;
    int lx = x;

    font( &game_12 );
    font_color(CLR_WHITE, CLR_WHITE);

    // villain/hero
    lx += (SGRAID_LINE_INDENT+SGRAID_FACTION_WD)*s_sc;

    // Date
    cprntEx(wx+lx+R10*s_sc, wy+y+17*s_sc, z, s_sc, s_sc, 0, text);
    y += dy;
}

int drawScheduledRaid(ScheduledBaseRaid* raid, U32 raidid)
{
    char datestr[100];
    Entity* player = playerPtr();
    int raid_button, player_participating;

    // draw defending or attacking raids at right time
    if (defending_raids && raid->defendersg != player_sg)
        return 1;
    if (!defending_raids && raid->attackersg != player_sg)
        return 1;

    if (raid->complete_time)
    {
        if (raid->attackers_won)
            strcpy(datestr, "AttackersWon");
        else
            strcpy(datestr, "DefendersWon");
    }
    else
        sprintf(datestr, "%i/%i %s", BaseRaidNumParticipants(raid, !defending_raids), raid->max_participants,
            textStd(defending_raids? "RaidStatusDefending": "RaidStatusAttacking"));

    raid_button = timerServerTime() < raid->time;
    player_participating = BaseRaidIsParticipant(raid, player? player->db_id: 0, !defending_raids);
    y += drawSGRaidLine(raid->defendersg, raid->id, raid->max_participants, /*TODO*/ 0, raid->time, 0, datestr, raid_button, player_participating);
    return 1;
}

void drawSGRaidInfo(SupergroupRaidInfo* info)
{
    U32 first, second;

    // show next instance of each day of week
    SupergroupRaidInfoGetWindowTimes(info, timerServerTime(), &first, &second);
    if (first)
        y += drawSGRaidLine(info->id, 0, info->max_raid_size, info->playertype == kPlayerType_Villain, first, info->window_length, 
            "RaidStatusOpen", 0, 0);
    if (second)
        y += drawSGRaidLine(info->id, 0, info->max_raid_size, info->playertype == kPlayerType_Villain, second, info->window_length, 
            "RaidStatusOpen", 0, 0);
}

int drawSGRaidList(int offset)
{
    float wd, ystart;
    int i;
    Entity* player = playerPtr();

       window_getDims( WDW_SGRAID_LIST, &wx, &wy, &z, &wd, &wht, &s_sc, &color, &bcolor );
    x = R10*s_sc;
    ystart = y = R10*s_sc-offset;

    if (player && player->supergroup)
    {
        // defending raids
        y += drawSGRaidHeader(textStd("RaidHeaderDefend", player->supergroup->name));
        player_sg = player->supergroup_id;
        defending_raids = 1;
        if (!BaseRaidCountDefending(player_sg) && !eaSize(&g_raidinfos))
            drawNoRaidLine("NotOpenForRaid");
        else
        {
            BaseRaidForEachSorted(drawScheduledRaid);
            // open defending slots
            if (eaSize(&g_raidinfos) && g_raidinfos[0]->id == player->supergroup_id)
                drawSGRaidInfo(g_raidinfos[0]);
                // TODO - hide any days that we already have challenges for?
        }

        // attacking raids
        y += drawSGRaidHeader(textStd("RaidHeaderAttack", player->supergroup->name));
        defending_raids = 0;
        if( !BaseRaidCountAttacking(player_sg) )
            drawNoRaidLine("NoScheduledRaids");
        else
            BaseRaidForEachSorted(drawScheduledRaid);
    }
    y += drawSGRaidHeader(textStd("RaidHeaderOtherSG"));
    for (i = 0; i < eaSize(&g_raidinfos); i++)
    {
        if (player && g_raidinfos[i]->id == player->supergroup_id)
            continue;
        drawSGRaidInfo(g_raidinfos[i]);
    }
    return y - ystart;
}

void sgRaidListRefresh(void)
{
    if (window_getMode(WDW_SGRAID_LIST) != WINDOW_UNINITIALIZED &&
        window_getMode(WDW_SGRAID_LIST) != WINDOW_DOCKED)
        EMPTY_INPUT_PACKET(CLIENTINP_REQ_SGRAIDLIST);
}

void sgraid_initSettings(void)
{
    sgRaidListRefresh();
}

void sgRaidListToggle(void)
{
    if (window_getMode(WDW_SGRAID_LIST) != WINDOW_DISPLAYING)
    {
        window_setMode(WDW_SGRAID_LIST, WINDOW_GROWING);
        sgRaidListRefresh();
    }
    else
        window_setMode(WDW_SGRAID_LIST, WINDOW_SHRINKING);
}

int sgRaidListWindow(void)
{
    float x, y, z, wd, ht, sc;
    int color, bcolor;
    int listht;
    int enabled, cancelbutton;
    Entity* player = playerPtr();

    static bool init = false;
    static ScrollBar sgroupSb = { WDW_SGRAID_LIST };

    raidClientTick(); // general stuff for raids

    // Do everything common windows are supposed to do.
       if ( !window_getDims( WDW_SGRAID_LIST, &x, &y, &z, &wd, &ht, &sc, &color, &bcolor ))
    {
        return 0;
    }

      if( !init ) // one time inits
    {
          sgraid_initSettings();
        init = true;
    }

    drawFrame( PIX3, R10, x, y, z, wd, ht, sc, color, bcolor );                                                // window 

    // Draw the list of supergroups
    set_scissor(true);
      scissor_dims( x+PIX3*sc, y + (PIX3)*sc, 5000, ht-(SGRAID_BUTTON_HT+2*PIX3)*sc);
    listht = drawSGRaidList( sgroupSb.offset );
    set_scissor(false);
 
     doScrollBar( &sgroupSb, ht-(SGRAID_BUTTON_HT+2*PIX3)*sc, listht+R10*sc, wd, PIX3*sc, z, 0, 0 );

    // draw buttons
       if( D_MOUSEHIT == drawStdButton( x + 75*sc + 2*PIX3, y + ht - BUTTON_OFFSET*sc/2 - 2*PIX3, z, 100*sc, 20*sc, color, "RaidRefreshButton", 1.f, 0 ) )
    {
        sgRaidListRefresh();
    }

    // window & raid buttons only for people who can schedule raids
    if (player && sgroup_hasPermission(player, SG_PERM_RAID_SCHEDULE))
    {
           if( D_MOUSEHIT == drawStdButton( x + 235*sc + 2*PIX3, y + ht - BUTTON_OFFSET*sc/2 - 2*PIX3, z, 110*sc, 20*sc, color, "RaidWindowButton", 1.f, 0 ) )
        {
            sgRaidTimeToggle();
        }

        // show raid button if we've selected an appropriate slot
        cancelbutton = g_selected_raidid && g_selected_sg != player->supergroup_id;
        if (cancelbutton)
        {
            ScheduledBaseRaid* raid = cstoreGet(g_ScheduledBaseRaidStore, g_selected_raidid);
            enabled = raid && raid->attackersg == player->supergroup_id && raid->time > timerServerTime();
        }
        else
        {
            enabled = g_selected_sg && player->supergroup_id != g_selected_sg;
        }
           if( D_MOUSEHIT == drawStdButton( x + 395*sc + 2*PIX3, y + ht - BUTTON_OFFSET*sc/2 - 2*PIX3, z, 100*sc, 20*sc, color, 
            cancelbutton? "RaidCancelButton": "RaidScheduleButton", 1.f, !enabled ) )
        {
            if (cancelbutton)
            {
                START_INPUT_PACKET(pak, CLIENTINP_RAID_CANCEL)
                    pktSendBitsAuto(pak, g_selected_raidid);
                END_INPUT_PACKET
            }
            else
            {
                sgRaidStartTimeToggle();
            }
            sgRaidListRefresh();
        }
    }

    return 0;
}

// *********************************************************************************
//  SGRaidTime - scheduling when the raid window will be open
// *********************************************************************************

SupergroupRaidInfo raidinfo;
int lastdaychecked = -1;

static U32 reparseTimeList;

void sgRaidTimeToggle(void)
{
    if (window_getMode(WDW_SGRAID_TIME) != WINDOW_DISPLAYING)
        window_setMode(WDW_SGRAID_TIME, WINDOW_GROWING);
    else
        window_setMode(WDW_SGRAID_TIME, WINDOW_SHRINKING);
}

void sgRaidStartTimeToggle(void)
{
    if (window_getMode(WDW_SGRAID_STARTTIME) != WINDOW_DISPLAYING)
        window_setMode(WDW_SGRAID_STARTTIME, WINDOW_GROWING);
    else
        window_setMode(WDW_SGRAID_STARTTIME, WINDOW_SHRINKING);
    reparseTimeList = 1;
}

#define DAYCHECK_SPACING    20
#define DAYTEXT_INDENT        15
#define DAYCHECK_WIDTH        8
#define HOURCOMBO_WIDTH        120
#define HOURCOMBO_INDENT    15

int dayCheckedCount(int* nonlast)
{
    int i, numchecked = 0;
    for (i = 0; i < 7; i++)
    {
        if (raidinfo.window_days & (1 << i))
        {
            numchecked++;
            if (nonlast && lastdaychecked != i)
                *nonlast = i;
        }
    }
    return numchecked;
}

void showRaidDay(int day, float x, float y, float z, float sc)
{
    int checked;
    char* text;

    switch (day)
    {
    case 0: text = "SundayFull"; break;
    case 1: text = "MondayFull"; break;
    case 2: text = "TuesdayFull"; break;
    case 3: text = "WednesdayFull"; break;
    case 4: text = "ThursdayFull"; break;
    case 5: text = "FridayFull"; break;
    default: text = "SaturdayFull"; break;
    }

    checked = raidinfo.window_days & (1 << day);
    if (drawRadioButton(x, y, DAYTEXT_INDENT+100, z, sc, checked? BUTTON_CHECKED: 0, text) == D_MOUSEHIT)
    {
        if (checked)
        {
            raidinfo.window_days &= ~(1 << day);
            if (lastdaychecked == day)
                lastdaychecked = -1;
        }
        else
        {
            // clear another day if required
            int notlastchecked;
            if (dayCheckedCount(&notlastchecked) >= 2)
                raidinfo.window_days &= ~(1 << notlastchecked);
            raidinfo.window_days |= (1 << day);
            lastdaychecked = day;
        }
    }
}

#define RAID_HOUR_START        10        // raid hours start at 10 am - server time
#define RAID_HOUR_COUNT        19        // 19 possible time periods from 10 am to 4 am - server time
static int raid_hours[19]; // filled with local hours that correspond to server 10am-4am, depends on tz_adjust
static int last_tz_adjust = -10000; 

static int day_part(int hour)
{
    int ret = 0;
    while (hour < 0) { hour += 24; ret--; }
    while (hour > 23) { hour -= 24; ret++; }
    return ret;
}
static int hour_part(int hour)
{
    while (hour < 0) hour += 24;
    return hour % 24;
}
static int compare_hours(const void* leftData, const void* rightData)
{
    int* left = (int*)leftData;
    int* right = (int*)rightData;
    return hour_part(*left) - hour_part(*right);
}

static U32 windowServerDays;
static U32 windowServerHours;
static U32 windowConflict;

#define SGRAID_WINDOWCHANGE_OK                0
#define SGRAID_WINDOWCHANGE_MOREDEFENDING    1
#define SGRAID_WINDOWCHANGE_OVERLAPSATTACK    2

static void sgRaidWindowChange(void * data)
{
    char date[200];
    sprintf(date, "sgraid_window %i %i", windowServerDays, windowServerHours);
    cmdParse(date);
    sgRaidTimeToggle();
    sgRaidListRefresh();
}

static void sgRaidPromptWindowChangeWarning()
{
    dialogStd(DIALOG_TWO_RESPONSE, "RaidWindowChangePrompt", "RaidWindowChangeAccept", "RaidWindowChangeCancel", sgRaidWindowChange, NULL, 0);
}

static void checkNewWindow(ScheduledBaseRaid* raid, U32 raidid)
{
    int i, n = eaSize(&g_raidinfos);
    SupergroupRaidInfo* raidInfo = NULL;
    U32 window1, window2;
    U32 oldDays, oldHours;
    Entity* player = playerPtr();
    if (!player) return;
    if (!player->supergroup_id) return;

    // Find the players raid info
    for (i = 0; i < n; i++)
        if (g_raidinfos[i]->id == player->supergroup_id)
            raidInfo = g_raidinfos[i];
    if (!raidInfo)
        return;

    // Ignore the raid if they are not currently in our window(they already chose to ignore the warning)
    SupergroupRaidInfoGetWindowTimes(raidInfo, timerServerTime(), &window1, &window2);
    if ((player->supergroup_id == raid->defendersg) &&
        ((window1 && (raid->time < window1 || raid->time >= (window1 + raidInfo->window_length*3600))) &&
        (window2 && (raid->time < window2 || raid->time >= (window2 + raidInfo->window_length*3600)))))
    {
        return;
    }

    // Now see if the raid is outside of our window
    oldDays = raidInfo->window_days;
    oldHours = raidInfo->window_hour;
    raidInfo->window_days = windowServerDays;
    raidInfo->window_hour = windowServerHours;
    SupergroupRaidInfoGetWindowTimes(raidInfo, timerServerTime(), &window1, &window2);

    // show warning if we have a raid that is no longer in our scheduled window
    if ((player->supergroup_id == raid->defendersg) &&
        ((!window1 || (raid->time < window1 || raid->time >= (window1 + raidInfo->window_length*3600))) &&
        (!window2 || (raid->time < window2 || raid->time >= (window2 + raidInfo->window_length*3600)))))
    {
        windowConflict = SGRAID_WINDOWCHANGE_MOREDEFENDING;
    }

    // Disallow the window change if this will allow you to schedule an attacking and defending raid at the same time
    if ((player->supergroup_id == raid->attackersg) &&
        ((window1 && (raid->time >= window1 && raid->time < (window1 + raidInfo->window_length*3600))) ||
        (window2 && (raid->time >= window2 && raid->time < (window2 + raidInfo->window_length*3600)))))
    {
        windowConflict = SGRAID_WINDOWCHANGE_OVERLAPSATTACK;
    }

    // Restore the old times since we havent actually changed yet
    raidInfo->window_days = oldDays;
    raidInfo->window_hour = oldHours;
}

int sgRaidTimeWindow(void)
{
    float x, y, z, wd, ht, sc;
    int color, bcolor, i, enabled, ends;
    Entity* player = playerPtr();
    static ComboBox hourCB;
    char date[200];
    static bool init = false;
    static ScrollBar sgroupSb = { WDW_SGRAID_TIME };
    int tz_adjust = 0; 
    
    //For converting the raid_hours array from CST to the timezone we guess you are in
    tz_adjust = (-1*timerServerTZAdjust()) - game_state.dbTimeZoneDelta;
    // returns -23..23 //(CST is -6 )

    // Do everything common windows are supposed to do.
    if ( !window_getDims( WDW_SGRAID_TIME, &x, &y, &z, &wd, &ht, &sc, &color, &bcolor ))
    {
        return 0;
    }
    if( !init )
    {
        sgraid_initSettings();
        init = true;
    }  
    drawFrame( PIX3, R10, x, y, z, wd, ht, sc, color, bcolor );                                                // window 

    // set up our raid hours array based on client tz,
    // sort so that midnight comes first, without regard to the day (modulo 24)
    if (last_tz_adjust != tz_adjust)
    { 
        last_tz_adjust = tz_adjust;
        for (i = 0; i < RAID_HOUR_COUNT; i++)
            raid_hours[i] = hour_part(RAID_HOUR_START + i + tz_adjust); 
        qsort(raid_hours, RAID_HOUR_COUNT, sizeof(raid_hours[0]), compare_hours);

        eaDestroyConst( &hourCB.strings );
    }

    // title
    font_color(CLR_WHITE, CLR_WHITE);
    cprntEx(x+wd/2, y + 30, z, sc*1.2, sc*1.2, CENTER_X,  "RaidWindowTitle");
    font_color(CLR_GREY, CLR_GREY);
    cprntEx(x+35, y+70, z, sc, sc, 0, "RaidWindowHelpLine1");
    cprntEx(x+35, y+85, z, sc, sc, 0, "RaidWindowHelpLine2");
    font_color(CLR_WHITE, CLR_WHITE); 

    // days of the week
    for (i = 0; i < 7; i++)
        showRaidDay(i, x+wd-120, y + i*DAYCHECK_SPACING + 60, z, sc);

    // hour drop-down
    font_color(CLR_WHITE, CLR_WHITE);
    cprntEx(x+15, y+120, z, sc, sc, 0, "RaidWindowBegin");
    if (!hourCB.strings)
    {
        char** strings = 0;
        eaCreate(&strings);
        for (i = 0; i < ARRAY_SIZE(raid_hours); i++)
        {
            timerFriendlyHour(date, ARRAY_SIZE(date), hour_part(raid_hours[i]), 0, 0);
            eaPush(&strings, strdup(date));
        }
        combobox_init(&hourCB, x+15+HOURCOMBO_INDENT, y+130, z, HOURCOMBO_WIDTH, 15, 120, strings, 0);
        hourCB.currChoice = 0;
    }
    combobox_setLoc(&hourCB,x+15+HOURCOMBO_INDENT,y+130,z);
    combobox_display(&hourCB);

    // window display
    ends = raid_hours[hourCB.currChoice] + 2;
    timerFriendlyHour(date, ARRAY_SIZE(date), hour_part(ends), 0, 0);
    cprntEx(x+15, y+180, z, sc, sc, 0, "RaidWindowEnd", date);

    // draw buttons
    if( D_MOUSEHIT == drawStdButton( x + 75*sc + 2*PIX3, y + ht - BUTTON_OFFSET*sc/2 - 2*PIX3, z, 75*sc, 20*sc, color, "CancelString", 1.f, 0 ) )
    {
        sgRaidTimeToggle();
    }
 
    enabled = (dayCheckedCount(0) == 2)? 1:0;
    if( D_MOUSEHIT == drawStdButton( x + wd - 75*sc - 2*PIX3, y + ht - BUTTON_OFFSET*sc/2 - 2*PIX3, z, 75*sc, 20*sc, color, "OkString", 1.f, !enabled ) )
    {
        // LH: Design change, check existing scheduled raids against the new times
        // Prompt them with dialog to make sure they actually want to change their window
        windowServerHours = raid_hours[hourCB.currChoice] - ( tz_adjust + game_state.dbTimeZoneDelta ); //MW Convert time zone we guessed you were in to GMT 
        windowServerDays = RaidWindowRotate(raidinfo.window_days, day_part(windowServerHours));
        windowServerHours = hour_part(windowServerHours);

        // Check to see if they currently have a scheduled raid in a slot they are changing
        windowConflict = SGRAID_WINDOWCHANGE_OK;
        cstoreForEach(g_ScheduledBaseRaidStore, checkNewWindow);

        if (windowConflict == SGRAID_WINDOWCHANGE_OK)
            sgRaidWindowChange(0);
        else if (windowConflict == SGRAID_WINDOWCHANGE_MOREDEFENDING)
            sgRaidPromptWindowChangeWarning();
        else if (windowConflict == SGRAID_WINDOWCHANGE_OVERLAPSATTACK)
            dialogStd(DIALOG_OK, textStd("RaidWindowChangeOverlap"), 0, 0, 0, 0, 0);
    }

    return 0;
}

int sgRaidStartTimeWindow(void)
{
    float x, y, z, wd, ht, sc;
    int color, bcolor;
    Entity* player = playerPtr();
    static ComboBox hourCB;
    char date[200];
    static ScrollBar sgroupSb = { WDW_SGRAID_STARTTIME };

    // Do everything common windows are supposed to do.
    if ( !window_getDims( WDW_SGRAID_STARTTIME, &x, &y, &z, &wd, &ht, &sc, &color, &bcolor ))
    {
        return 0;
    }
    drawFrame( PIX3, R10, x, y, z, wd, ht, sc, color, bcolor );                                                // window 

    // title
    font_color(CLR_WHITE, CLR_WHITE);
    cprntEx(x+wd/2, y + 30, z, sc*1.2, sc*1.2, CENTER_X, "ChooseRaidTime");

    // hour drop-down
    if (reparseTimeList)
    {
        static char** raidStartStrings;
        U32 raidOffset, startHour;
        eaDestroyEx(&raidStartStrings, NULL);
        eaCreate(&raidStartStrings);
        startHour = timerHourFromSS2Value(localTimeFromServer(g_selected_time), 1);
        for (raidOffset = 0; raidOffset < g_selected_raidlength; raidOffset++)
        {
            timerFriendlyHour(date, ARRAY_SIZE(date), hour_part(startHour + raidOffset), 0, 0);
            eaPush(&raidStartStrings, strdup(date));
        }
        combobox_init(&hourCB, x+90, y+65, z, HOURCOMBO_WIDTH, 15, 120, raidStartStrings, 0);
        hourCB.currChoice = 0;
        reparseTimeList = 0;
    }
    combobox_setLoc(&hourCB,x+90,y+65,z);
    combobox_display(&hourCB);

    // draw buttons
    if( D_MOUSEHIT == drawStdButton( x + 75*sc + 2*PIX3, y + ht - BUTTON_OFFSET*sc/2 - 2*PIX3, z, 75*sc, 20*sc, color, "CancelString", 1.f, 0 ) )
    {
        sgRaidStartTimeToggle();
    }

    if( D_MOUSEHIT == drawStdButton( x + wd - 75*sc - 2*PIX3, y + ht - BUTTON_OFFSET*sc/2 - 2*PIX3, z, 75*sc, 20*sc, color, "OkString", 1.f, 0) )
    {
        START_INPUT_PACKET(pak, CLIENTINP_RAID_SCHEDULE)
            pktSendBitsAuto(pak, g_selected_sg);
            pktSendBitsAuto(pak, g_selected_time + hourCB.currChoice*3600);
            pktSendBitsAuto(pak, g_selected_raidsize);
        END_INPUT_PACKET
        sgRaidStartTimeToggle();
    }

    return 0;
}

#define SIZECHECK_SPACING    20
#define SIZECHECK_WIDTH        8
#define SIZETEXT_INDENT        15
int g_selectedraidsize = 0;

void sgRaidSizePrompt(void)
{
    g_selectedraidsize = 8;
    if (window_getMode(WDW_SGRAID_SIZE) != WINDOW_DISPLAYING)
        window_setMode(WDW_SGRAID_SIZE, WINDOW_GROWING);
}

void showRaidSize(int size, float x, float y, float z, float sc)
{
    int checked = size == g_selectedraidsize;

    if (drawRadioButton(x, y, 120, z, sc, checked? BUTTON_CHECKED:0, textStd("RaidSizeSelect", size)) == D_MOUSEHIT)
    {
        if (checked)
            g_selectedraidsize = 0;
        else
            g_selectedraidsize = size;
    }
}

int sgRaidSizeWindow(void)
{
    float x, y, z, wd, ht, sc;
    int color, bcolor, i, enabled;
    Entity* player = playerPtr();
    static ComboBox hourCB;

    static bool init = false;
    static ScrollBar sgroupSb = { WDW_SGRAID_SIZE };

    // Do everything common windows are supposed to do.
       if ( !window_getDims( WDW_SGRAID_SIZE, &x, &y, &z, &wd, &ht, &sc, &color, &bcolor ))
    {
        return 0;
    }

      if( !init ) // one time inits
    {
          sgraid_initSettings();
        init = true;
    }

    drawFrame( PIX3, R10, x, y, z, wd, ht, sc, color, bcolor );                                                // window 

    // title
    font_color(CLR_WHITE, CLR_WHITE);
    cprntEx(x+wd/2, y+30, z, sc, sc, CENTER_X, "RaidWindowSizePrompt");

    // size of the raid
    for (i = 1; i <= 4; i++)
        showRaidSize(i*8, x+30, y + (i-1)*SIZECHECK_SPACING + 50, z, sc);

    // draw buttons
       if( D_MOUSEHIT == drawStdButton( x + 75*sc + 2*PIX3, y + ht - BUTTON_OFFSET*sc/2 - 2*PIX3, z, 75*sc, 20*sc, color, "CancelString", 1.f, 0 ) )
    {
        window_setMode(WDW_SGRAID_SIZE, WINDOW_SHRINKING);
    }

    enabled = 1;
    if( D_MOUSEHIT == drawStdButton( x + wd - 75*sc - 2*PIX3, y + ht - BUTTON_OFFSET*sc/2 - 2*PIX3, z, 75*sc, 20*sc, color, "OkString", 1.f, !enabled ) )
    {
        window_setMode(WDW_SGRAID_SIZE, WINDOW_SHRINKING);
    }

    return 0;
}

