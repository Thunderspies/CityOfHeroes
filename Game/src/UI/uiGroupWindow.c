/***************************************************************************
 *     Copyright (c) 2003-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
***************************************************************************/
#include <utilitieslib/components/Earray.h>
#include "UI/uiWindows.h"
#include "UI/uiGroupWindow.h"
#include "UI/uiTarget.h"
#include "player/player.h"
#include "entity/entclient.h"
#include "UI/sprite/sprite_text.h"
#include "graphics/textureatlas.h"
#include "UI/uiInput.h"
#include "UI/uiUtil.h"
#include "UI/uiCursor.h"
#include <utilitieslib/network/netio.h>
#include "UI/sprite/sprite_font.h"
#include "UI/sprite/sprite_base.h"
#include "UI/uiUtilGame.h"
#include "UI/uiUtilMenu.h"
#include "entity/attrib_net.h"
#include "entity/entVarUpdate.h"
#include "UI/uiChat.h"
#include "UI/uiStatus.h"
#include "UI/uiNet.h"
#include "UI/uiTeam.h"
#include "UI/uiGame.h"
#include "UI/uiReticle.h"
#include "UI/uiToolTip.h"
#include "UI/uiContextMenu.h"
#include "entity/powers.h"
#include "entity/EntPlayer.h"
#include "language/langClientUtil.h"
#include "cmdparse/cmdgame.h"
#include "gameComm/itemselect.h"
#include "UI/uiOptions.h"
#include "graphics/ttFontUtil.h"
#include "UI/uiComboBox.h"
#include "UIEdit.h"
#include "UI/uiEditText.h"
#include <utilitieslib/utils/utils.h>
#include "entity/entity.h"
#include "win/win_init.h"
#include "entity/teamCommon.h"
#include "UI/uiTray.h"
#include "uiGift.h"
#include "UI/uiBuff.h"
#include <utilitieslib/language/MessageStoreUtil.h>
#include "arena/arenagame.h"
#include "uiLeague.h"
#include "UI/uiTabControl.h"
#include "UI/uiLfg.h"
#include "UI/uiClipper.h"
#include <utilitieslib/components/estring.h>
#include "entity/character_target.h"

#define MAX_MEMBER_TIPS 2

enum
{
    TEAM_TIP_SEEK,
    TEAM_TIP_FIND,
    TEAM_TIP_LEADER,
    TEAM_TIP_TOTAL,
};

typedef enum leaderLevel
{
    GROUP_LEADER_NONE,
    GROUP_LEADER_SOLO,
    GROUP_LEADER_TEAM,
    GROUP_LEADER_ALL,
    GROUP_LEADER_COUNT,
}leaderLevel;

static SearchOption searchOptions[] = {
    {0,              "NotSeeking",        "NotSeekingTitle",    1, "search_none.tga",                CLR_GREY,    0    },
    {LFG_ANY,      "SearchAny",        "SearchAnyTitle",    1, "search_any.tga",                CLR_WHITE,    0    },
    {LFG_HUNT,      "SearchHunt",        "SearchHuntTitle",    1, "search_patrol.tga",                0xff8888ff,    0    },
    {LFG_DOOR,      "SearchDoor",        "SearchDoorTitle",    1, "search_mission.tga",            CLR_GREEN,    0    },
    {LFG_TF,      "SearchTF",        "SearchTFTitle",    1, "search_task.tga",                CLR_YELLOW,    0    },
    {LFG_TRIAL,      "SearchTrial",    "SearchTrialTitle",    1, "search_trial.tga",                CLR_BLUE,    0    },
    {LFG_ARENA,      "SearchArena",    "SearchArenaTitle", 1, "search_arena.tga",                0xff33ffff,    0   },
    {LFG_NOGROUP, "NoGroup",        "NoGroupTitle",        1, "search_none.tga",                CLR_RED,    CLR_RED    },
};

static ComboBox *comboLFGSearch = NULL;
static int s_numRows = 2;
static int s_timerTracker;

void addLFG( ComboBox * cb )
{
    int i;

    combocheckbox_add( cb, 0, atlasLoadTexture(searchOptions[0].texName), searchOptions[0].displayName, LFG_NONE );

     for( i = 1; i < ARRAY_SIZE(searchOptions); i++ )
    {
        comboboxTitle_add( cb, 0, atlasLoadTexture(searchOptions[i].texName), searchOptions[i].displayName, NULL, searchOptions[i].bitmask, searchOptions[i].color, searchOptions[i].icon_color,0  );
    }
}

SearchOption * lfg_getSearchOption( int lfg )
{
    int i;

    for( i = 0; i < ARRAY_SIZE(searchOptions); i++ )
    {
        if( lfg&searchOptions[i].bitmask || (!lfg && !searchOptions[i].bitmask) )
            return &searchOptions[i];
    }

    return &searchOptions[0];  // none is a safe choice
}

static ToolTip teamMemberTip[MAX_TEAM_MEMBERS][MAX_MEMBER_TIPS]; // 0 slot for health
static ToolTip buttonTip[TEAM_TIP_TOTAL];
static ToolTip s_activePlayerTip;
static ToolTipParent GroupTipParent = {0};

typedef enum GroupExpandedViewFlags
{
    GEV_GRID = 1,
    GEV_FULL = 1 << 1,
}GroupExpandedViewFlags;
static int gShowBuffs = FALSE;
static int gViewFlags = 0;


//typedef char MapName[256];
//MapName teamMapNames[MAX_TEAM_MEMBERS] = {0};
//MapName levelingpactMapNames[MAX_LEVELINGPACT_MEMBERS] = {0};
//MapName leagueMapNames[MAX_LEAGUE_MEMBERS] = {0};

//-----------------------------------------------------------------------------------------------
// Icon cache ///////////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------------------------
typedef struct archetypeIconCache
{
    int        dbid;
    AtlasTex *icon;
    AtlasTex *deadIcon;
    char    *className;

} archetypeIconCache;

archetypeIconCache **aic = {0};
static AtlasTex * getLeagueATIcon(Entity *e, int getDeadIcon)
{
    if(ENTTYPE(e)==ENTTYPE_PLAYER)
    {
        if (e->pchar && e->pchar->pclass)
        {
            char ach[256];
            char *pch;
            AtlasTex* tex = NULL;
            const char *archetype = e->pchar->pclass->pchName;
            if(!archetype)
                return white_tex_atlas;

            strcpy(ach, "League_icon_v_");
            pch = strchr(archetype, '_');
            if(pch)
                strcat(ach, pch+1);
            else
                strcat(ach, archetype);

            if (getDeadIcon)
                strcat(ach, "_dead");
            tex = atlasLoadTexture(ach);

            if( !tex || stricmp(tex->name, "white") == 0 )
            {
                strcpy(ach, "League_icon_");
                pch = strchr(archetype, '_');
                if(pch)
                    strcat(ach, pch+1);
                else
                    strcat(ach, archetype);
                if (getDeadIcon)
                    strcat(ach, "_dead");
                tex = atlasLoadTexture(ach);
            }
            if (getDeadIcon && stricmp(tex->name, "white") == 0)
            {
                tex = atlasLoadTexture("teamup_skull.tga");
            }
            return tex;
        }
    }

    return white_tex_atlas;
}
archetypeIconCache * aic_AddSafe( int dbid )
{
    int i;
    Entity * e;
    archetypeIconCache * ac;

    if(!aic)
        eaCreate(&aic);

    for( i = 0; i < eaSize(&aic); i++ )
    {
        if( aic[i]->dbid == dbid )
            return aic[i];
    }

    e = entFromDbId( dbid );

    if( !e )
        return NULL;

    ac = malloc( sizeof(archetypeIconCache) );
    ac->dbid = dbid;
    ac->icon = getLeagueATIcon(e, 0);
    ac->deadIcon = getLeagueATIcon(e, 1);
    ac->className = malloc( sizeof(char)*(strlen(e->pchar->pclass->pchDisplayName)+1) );
    strcpy( ac->className, e->pchar->pclass->pchDisplayName );

    if( ac->icon )
        eaPush( &aic, ac );

    return ac;
}

//-----------------------------------------------------------------------------------------------
// Display Functions ////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------------------------

void setTeamBuffMode( int mode )
{
    gShowBuffs = mode;
}

#define GROUP_HEIGHT        22
#define GROUP_SPACE         2
#define GROUP_HEALTH_XOFF   20
#define GROUP_BAR_WD        52.f
#define GROUP_BAR_HT        10.f
#define GROUP_BUTTON_WD        50
#define GROUP_ICON_WD        10
#define SIDEKICK_INDENT        10
#define TXT_XOFF            35
#define TXT_YOFF            16
#define PULSE_TIMER_MAX        90
int team_select( TeamMembers *members, int idx )
{
    Entity *e = playerPtr();
    Entity *team_mate = NULL;

    int num_team = 0;

     if( members && members->count > 1 )
        num_team = members->count;

    if( !e || idx <= 0 || idx > num_team || !isEntitySelectable(e) )
        return 0;

    idx--;

    if( idx < num_team )
        team_mate = entFromDbId( members->ids[idx] );

    if( team_mate )
    {
        if( team_mate == e )
        {
            InfoBox( INFO_USER_ERROR, "CanSelectSelf" );
            return 0;
        }
        current_target = team_mate;
        gSelectedDBID = 0;
        gSelectedHandle[0] = 0;
    }
    else
    {
        gSelectedDBID = members->ids[idx];
        strcpy( gSelectedName, members->names[idx] );
        gSelectedHandle[0] = 0;
        current_target = 0;
    }

    return 1;
}

static void drawLeagueButton(    AtlasTex *upperLeft, AtlasTex *topMid, AtlasTex *upperRight, AtlasTex *leftMid, 
                                AtlasTex *lowerLeft, AtlasTex *botMid, AtlasTex *lowerRight, AtlasTex *rightMid,
                                float x, float y, float z, float wd, float ht, float sc, int color)
{
    display_sprite(upperLeft, x, y, z, sc, sc, color);
    display_sprite(topMid, x+upperLeft->width*sc, y, z, (wd-(upperLeft->width+upperRight->width)*sc)/topMid->width, sc, color);
    display_sprite(upperRight, x+(wd-upperRight->width*sc), y, z, sc, sc, color);
    display_sprite(leftMid, x, y+upperLeft->height*sc, z, sc, (ht-(upperLeft->height+lowerRight->height)*sc)/leftMid->height, color);

    display_sprite(lowerLeft, x, y+(ht-lowerLeft->height*sc), z, sc, sc, color);
    display_sprite(botMid, x+lowerLeft->width*sc, y+(ht-botMid->height*sc), z, (wd-(lowerLeft->width+lowerRight->width)*sc)/botMid->width, sc, color);
    display_sprite(lowerRight, x+(wd-lowerRight->width*sc), y+(ht-lowerRight->height*sc), z, sc, sc, color);
    display_sprite(rightMid, x+(wd-rightMid->width*sc), y+upperRight->height*sc, z, sc, (ht-(upperRight->height+lowerRight->height)*sc)/rightMid->height, color);
}
static void drawLeaguePlayerFrame(float x, float y, float z, float wd, float ht, int selected, int active)
{
    AtlasTex *botMid = NULL;
    AtlasTex *lowerLeft = NULL;
    AtlasTex *leftMid = NULL;
    AtlasTex *lowerRight= NULL;
    AtlasTex *rightMid = NULL;
    AtlasTex *topMid = NULL;
    AtlasTex *upperLeft = NULL;
    AtlasTex *upperRight = NULL;
    float sc = 1.0f;
    int color = CLR_GREY;
    if (selected || active)
    {
        botMid = atlasLoadTexture("League_select_Bottom_Mid");
        lowerLeft = atlasLoadTexture("League_select_LL");
        leftMid = atlasLoadTexture("League_select_L_Mid");
        lowerRight = atlasLoadTexture("League_select_LR");
        rightMid = atlasLoadTexture("League_select_R_Mid");
        topMid = atlasLoadTexture("League_select_Top_Mid");
        upperLeft = atlasLoadTexture("League_select_UL");
        upperRight = atlasLoadTexture("League_select_UR");

        if (selected == 1)    //    selected can equal 2 if it is a mouseover
        {
            color = CLR_WHITE;
        }
        else if (active)
        {
            color = CLR_YELLOW;
        }
        sc = 0.5f;
    }
    else
    {
        botMid = atlasLoadTexture("League_Player_Frame_BottomMid");
        lowerLeft = atlasLoadTexture("League_Player_Frame_LL");
        leftMid = atlasLoadTexture("League_Player_Frame_LMid");
        lowerRight = atlasLoadTexture("League_Player_Frame_LR");
        rightMid = atlasLoadTexture("League_Player_Frame_RMid");
        topMid = atlasLoadTexture("League_Player_Frame_TopMid");
        upperLeft = atlasLoadTexture("League_Player_Frame_UL");
        upperRight = atlasLoadTexture("League_Player_Frame_UR");
    }
    
    drawLeagueButton(upperLeft, topMid, upperRight, leftMid, lowerLeft, botMid, lowerRight, rightMid, x, y, z, wd, ht, sc, color);
}
#define NUM_LETTERS 15
//-----------------------------------------------------------------------------------------------
// Draw Interior ////////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------------------------
//
//
void drawGroupInterior( float x, float y, float z, float wd, float scale, float hp_percent, float absorb_percent, float end_percent, int barColor,
                        char * txt, int selected, int active, int offMap, int leader, AtlasTex *icon, int i, int isSelf, int pet, 
                        int currentWindow, int oldStyleTeamUI)
{
    if (oldStyleTeamUI)
    {
        AtlasTex *bar = atlasLoadTexture( "Frame_3px_horiz.tga");
        AtlasTex *back = white_tex_atlas;
        AtlasTex *star = atlasLoadTexture( "MissionPicker_icon_star.tga" );

        CBox box = {0};
        int textWd;
        float iconSc = .6f*scale, textSc = 1.f*scale;
        drawFrame( PIX2, R6, x, y + (PIX3/2)*scale, ++z, wd-PIX3*scale/2, (GROUP_HEIGHT)*scale, scale, barColor | 0xff, 
            !offMap && (hp_percent > 0) ? ((barColor & 0xffffff00) | 0x00000044) : 0x00000088 );

        if(selected)
            drawFlatFrame( PIX2, R6, x, y + (PIX3/2)*scale, ++z, wd-PIX3*scale/2, GROUP_HEIGHT*scale, scale, CLR_WHITE, 0 );

        // draw their name
        font_color( barColor|0xff, barColor|0xff );
        font( &game_12 );
        textWd = str_wd( &game_12, textSc, textSc, "TeamMemberName", txt );

        if( textWd >= (wd-(GROUP_BAR_WD+40)*scale) )
            textSc = str_sc( &game_12, (wd-(GROUP_BAR_WD+40)*scale), "TeamMemberName", txt );

        if( pet )
        {
            prnt( x + (GROUP_BAR_WD + 9)*scale, y + (TXT_YOFF+2)*scale, z, textSc, textSc, "TeamMemberName", txt );
        }
        else
            prnt( x + (GROUP_BAR_WD+TXT_XOFF)*scale, y + (TXT_YOFF+2)*scale, z, textSc, textSc, "TeamMemberName", txt );

        // draw leader star
        if( leader )
        {
            float leaderScale = leader * (1.f/(GROUP_LEADER_COUNT-1));
            float origScale = 0.5f*scale;
            float starsc = origScale * leaderScale;
            float tx = x + (GROUP_BAR_WD + GROUP_HEALTH_XOFF)*scale;
            float ty = y + ((GROUP_HEIGHT+2)*scale - star->height*starsc)/2;

            display_sprite( star, tx, ty, z, starsc, starsc, CLR_YELLOW );

            //tooltip
            BuildCBox( &buttonTip[TEAM_TIP_LEADER].bounds, tx, ty, star->width*starsc, star->height*starsc );
            setToolTipEx( &buttonTip[TEAM_TIP_LEADER], &buttonTip[TEAM_TIP_LEADER].bounds, textStd("teamLeaderTip"), &GroupTipParent, MENU_GAME, WDW_GROUP, 0, TT_NOTRANSLATE );
        }

        //draw the icon
        if( icon )
        {
            display_sprite( icon, x+3*scale, y + (GROUP_HEIGHT+PIX2)*scale/2 - icon->height*iconSc/2, z+4, iconSc, iconSc, CLR_WHITE );
            if(!pet)
            {
                BuildCBox( &teamMemberTip[i][1].bounds, x, y + 9*scale - icon->height*iconSc/2, icon->width*iconSc, icon->height*iconSc );
            }
        }


        // draw the bars
        if( pet )
            x += 6*scale;
        else
            x += GROUP_HEALTH_XOFF*scale;

        y += (GROUP_HEIGHT+2)*scale/2;

        //if( window_getMode( WDW_GROUP ) == WINDOW_DISPLAYING )
        {
            display_sprite( back, x, y - GROUP_BAR_HT*scale/2, z, GROUP_BAR_WD*scale/back->width, GROUP_BAR_HT*scale/back->height, CLR_BLACK );
            display_sprite( back, x+1*scale, y - GROUP_BAR_HT*scale/2 + 1*scale, z+1, (GROUP_BAR_WD-2)*scale/back->width, 3.f*scale/back->height, 0x111144ff );
            display_sprite( back, x+1*scale, y+1*scale, z+1, (GROUP_BAR_WD-2)*scale/back->width, 3.f*scale/back->height, 0x111144ff );

            if( !pet )
            {
                BuildCBox( &teamMemberTip[i][0].bounds, x, y - GROUP_BAR_HT*scale/2, GROUP_BAR_WD*scale, GROUP_BAR_HT*scale );
                if( offMap )
                    setToolTipEx( &teamMemberTip[i][0], &teamMemberTip[i][0].bounds, textStd("teamUnknownHealthTip"), &GroupTipParent, MENU_GAME, currentWindow, 0, TT_NOTRANSLATE );
                else if (absorb_percent > 0)
                    setToolTipEx( &teamMemberTip[i][0], &teamMemberTip[i][0].bounds, textStd("teamHealthAndAbsorbTip", (int)(hp_percent*100), (int)(absorb_percent*100), (int)(100*end_percent)), &GroupTipParent, MENU_GAME, currentWindow, 0, TT_NOTRANSLATE );
                else
                    setToolTipEx( &teamMemberTip[i][0], &teamMemberTip[i][0].bounds, textStd("teamHealthTip", (int)(hp_percent*100), (int)(100*end_percent)), &GroupTipParent, MENU_GAME, currentWindow, 0, TT_NOTRANSLATE );
            }

            if ( hp_percent > 0 )
            {
                float displayMaxPercent = MAX(1.0f, hp_percent + absorb_percent);

                display_sprite(bar, x+1*scale, y - 3*scale, z+2, 
                    ((hp_percent + absorb_percent) * (GROUP_BAR_WD - 2) * scale) / bar->width,
                    scale, 0xff6666ff);
                display_sprite(bar, x+1*scale, y - 3*scale, z+2, 
                    (absorb_percent * (GROUP_BAR_WD - 2) * scale) / bar->width, 
                    scale, getAbsorbColor(absorb_percent));
                display_sprite(bar, x+1*scale, y + 1*scale, z+2, 
                    (end_percent * (GROUP_BAR_WD - 2) * scale) / bar->width, 
                    scale, 0x6666ffff);
            }
        }
    }
    else
    {
        AtlasTex *back = white_tex_atlas;

        CBox box = {0};
        int textWd;
        float iconSc = .7f*scale, textSc = 0.7f*scale;
        float origx = x;
        float origy = y;
        char *nameText = NULL;
        unsigned int starTxtLoss = 0;
        float starTxtOffset = 0.0f;
        wd -= icon ? 27*iconSc : 0;
        // draw their name
        if (isSelf)
        {
            font_color(CLR_GOLD_FOREGROUND | 0xff, CLR_GOLD_FOREGROUND |0xff);
        }
        else if (offMap)
        {
            font_color(CLR_GREY, CLR_GREY );
        }
        else if (!hp_percent)
        {
            //    start pulsing
            int deadColor;
            float pulsePercent = (sinf(((int)((s_timerTracker+i)*0.5f)%PULSE_TIMER_MAX) *2*PI/PULSE_TIMER_MAX)+1)/2.f;
            interp_rgba(pulsePercent, CLR_RED, CLR_DARK_RED, &deadColor);
            font_color(deadColor, deadColor);
        }
        else
        {
            font_color(CLR_WHITE, CLR_WHITE );
        }

        // draw leader star
        if( leader >= GROUP_LEADER_TEAM)
        {
            AtlasTex *star = atlasLoadTexture((leader == GROUP_LEADER_ALL) ? "League_Icon_League_Leader_Star" :  "League_Icon_Team_Leader_Icon" );
            float origScale = 0.6f*scale;
            float tx = x;
            float ty = y + ((GROUP_HEIGHT+2)*scale - star->height*origScale)/2;

            display_sprite( star, tx, ty, z+10, origScale, origScale, CLR_WHITE );

            //tooltip
            BuildCBox( &buttonTip[TEAM_TIP_LEADER].bounds, tx, ty, star->width*origScale, star->height*origScale );
            setToolTipEx( &buttonTip[TEAM_TIP_LEADER], &buttonTip[TEAM_TIP_LEADER].bounds, textStd("teamLeaderTip"), &GroupTipParent, MENU_GAME, currentWindow, 0, TT_NOTRANSLATE );
            starTxtLoss = 3;
            starTxtOffset = star->width *origScale;
        }

        font( &game_12 );
        estrConcatCharString(&nameText, txt);
        if (currentWindow == WDW_LEAGUE)
        {
            if (estrLength(&nameText) > (NUM_LETTERS-starTxtLoss))
            {
                estrSetLength(&nameText, NUM_LETTERS-(starTxtLoss+1));
                estrConcatChar(&nameText, '*');
            }
        }

        textWd = str_wd( &game_12, textSc, textSc, "TeamMemberName", nameText );

        if( textWd >= (wd- (10*scale + starTxtOffset)) )
            textSc = str_sc( &game_12, (wd- (10*scale + starTxtOffset)), "TeamMemberName", nameText );

        if( pet )
        {
            cprntEx( x + wd/2, y + (GROUP_HEIGHT-2)*scale/2, z+5, textSc, textSc, CENTER_X | CENTER_Y, nameText );
        }
        else
            cprntEx( x + starTxtOffset/4 + wd/2, y + (GROUP_HEIGHT-2)*scale/2, z+5, textSc, textSc, CENTER_X | CENTER_Y | NO_MSPRINT, nameText );

        estrDestroy(&nameText);

        //draw the icon
        if( icon )
        {
            display_sprite( icon, x+ (((hp_percent > 0.0f) | offMap) ? (wd+2*scale) : (0.5f *(wd-icon->width*iconSc))), y + 11*scale - icon->height*iconSc/2, z+4, iconSc, iconSc, CLR_WHITE );
            if(!pet)
            {
                if (currentWindow == WDW_GROUP)
                {
                    BuildCBox( &teamMemberTip[i][1].bounds, x, y + 9*scale - icon->height*iconSc/2, icon->width*iconSc, icon->height*iconSc );
                }
            }
        }


        // draw the bars
        if( pet )
            x += 6*scale;
        else
            x += GROUP_HEALTH_XOFF*scale;

        y += (GROUP_HEIGHT+2)*scale/2;

        if( !pet && currentWindow == WDW_GROUP)
        {
            char *tempString = 0;

            BuildCBox( &teamMemberTip[i][0].bounds, origx, origy, wd, GROUP_HEIGHT*scale );
            if (offMap)
            {
                estrConcatCharString(&tempString, textStd("teamUnknownHealthTip"));
            }
            else if (absorb_percent > 0)
            {
                estrConcatCharString(&tempString, textStd("teamHealthAndAbsorbTip", (int)(hp_percent*100), (int)(absorb_percent*100), (int)(100*end_percent)));
            }
            else
            {
                estrConcatCharString(&tempString, textStd("teamHealthTip", (int)(hp_percent*100), (int)(100*end_percent)));
            }

            if (active)
            {
                estrConcatStaticCharArray(&tempString, "<br>");
                estrConcatCharString(&tempString, textStd("ThisIsActivePlayer"));
            }

            setToolTipEx( &teamMemberTip[i][0], &teamMemberTip[i][0].bounds, tempString, &GroupTipParent, MENU_GAME, currentWindow, 0, TT_NOTRANSLATE );
            estrDestroy(&tempString);
        }

        if ( hp_percent > 0 )
        {
            float hpDisplayPercent = (absorb_percent + hp_percent) / (absorb_percent + 1.0f);
            float absorbDisplayPercent = absorb_percent / (absorb_percent + 1.0f);
            int absorbColor = getAbsorbColor(absorb_percent);
            AtlasTex *hpBar_lm = atlasLoadTexture( "League_HealthBar_Tintable_Meat_End");
            AtlasTex *hpBar_ls = atlasLoadTexture( "League_HealthBar_Tintable_Shadow_End");
            AtlasTex *hpBar_lh = atlasLoadTexture( "League_HealthBar_Tintable_Highlight_End");
            AtlasTex *hpBar_mm = atlasLoadTexture( "League_HealthBar_Tintable_Meat_M");
            AtlasTex *hpBar_ms = atlasLoadTexture( "League_HealthBar_Tintable_Shadow_M");
            AtlasTex *hpBar_mh = atlasLoadTexture( "League_HealthBar_Tintable_Highlight_M");
            AtlasTex *hpBar_rm = atlasLoadTexture( "League_HealthBar_Tintable_Meat_End");
            AtlasTex *hpBar_rs = atlasLoadTexture( "League_HealthBar_Tintable_Shadow_End");
            AtlasTex *hpBar_rh = atlasLoadTexture( "League_HealthBar_Tintable_Highlight_End");
            AtlasTex *endBar_l = atlasLoadTexture( "League_EnergyBar_L");
            AtlasTex *endBar_m = atlasLoadTexture( "League_EnergyBar_Mid");
            AtlasTex *endBar_r = atlasLoadTexture( "League_EnergyBar_R");
            float hpHtSc = (GROUP_HEIGHT-7)*scale/hpBar_mm->height;
            float endHtSc = 7*scale/endBar_m->height;
            UIBox *currentBox;
            CBox hpBox;
            int alphaH = 0xffffff77;
            int alphaS = 0xffffff44;
            float clippedX, clippedY, clippedWd, clippedHt;
            currentBox = clipperGetBox(clipperGetCurrent());

            // display HP
            if (currentBox)
            {
                clippedX = MAX(currentBox->x, origx +1*scale);
                clippedY = MAX(currentBox->y, origy);
                clippedWd = MIN(currentBox->x + currentBox->width, hpDisplayPercent*wd + origx + 1*scale) - clippedX;
                clippedHt = MAX(0, MIN(currentBox->y + currentBox->height, GROUP_HEIGHT*scale + origy) - clippedY);
            }
            else
            {
                clippedX = origx +1*scale;
                clippedY = origy;
                clippedWd = hpDisplayPercent*wd;
                clippedHt = hpBar_mm->height*hpHtSc;
            }
            BuildCBox(&hpBox, clippedX,clippedY, clippedWd, clippedHt);
            if (hp_percent < 0.4f)
            {
                //    start pulsing
                float pulsePercent = (sinf((((int)((s_timerTracker+i)/(3*hp_percent)))%PULSE_TIMER_MAX) *2*PI/PULSE_TIMER_MAX)+1)/6.f;
                interp_rgba(pulsePercent, barColor, CLR_WHITE, &barColor);
                alphaH = 0xffffff00 | (barColor & 0x000000ff);
                alphaH = MIN(0xffffff77, alphaH);
                alphaS = 0xffffff00 | (barColor & 0x000000ff);
                alphaS = MIN(0xffffff44, alphaS);
            }
            clipperPushCBox(&hpBox);
            display_sprite( hpBar_lm, origx+1*scale, origy, z+2, hpHtSc, hpHtSc, barColor );
            display_sprite( hpBar_ls, origx+1*scale, origy, z+3, hpHtSc, hpHtSc, alphaS );
            display_sprite( hpBar_lh, origx+1*scale, origy, z+4, hpHtSc, hpHtSc, alphaH );
            display_sprite( hpBar_mm, hpBar_lm->width*hpHtSc+origx+1*scale, origy, z+2, ((wd-(hpBar_rm->width + hpBar_lm->width)*hpHtSc)-2*scale)/hpBar_mm->width, hpHtSc, barColor );
            display_sprite( hpBar_ms, hpBar_lm->width*hpHtSc+origx+1*scale, origy, z+3, ((wd-(hpBar_rm->width + hpBar_lm->width)*hpHtSc)-2*scale)/hpBar_mm->width, hpHtSc, alphaS);
            display_sprite( hpBar_mh, hpBar_lm->width*hpHtSc+origx+1*scale, origy, z+4, ((wd-(hpBar_rm->width + hpBar_lm->width)*hpHtSc)-2*scale)/hpBar_mm->width, hpHtSc, alphaH );
            display_spriteFlip( hpBar_rm, hpBar_lm->width*hpHtSc+origx+1*scale + ((wd-(hpBar_rm->width +hpBar_lm->width)*hpHtSc)-2*scale), origy, z+2, hpHtSc, hpHtSc, barColor, FLIP_HORIZONTAL  );
            display_spriteFlip( hpBar_rs, hpBar_lm->width*hpHtSc+origx+1*scale + ((wd-(hpBar_rm->width +hpBar_lm->width)*hpHtSc)-2*scale), origy, z+3, hpHtSc, hpHtSc, alphaS, FLIP_HORIZONTAL  );
            display_spriteFlip( hpBar_rh, hpBar_lm->width*hpHtSc+origx+1*scale + ((wd-(hpBar_rm->width +hpBar_lm->width)*hpHtSc)-2*scale), origy, z+4, hpHtSc, hpHtSc, alphaH, FLIP_HORIZONTAL  );
            clipperPop();

            // display absorb
            if (currentBox)
            {
                clippedX = MAX(currentBox->x, origx +1*scale);
                clippedY = MAX(currentBox->y, origy);
                clippedWd = MIN(currentBox->x + currentBox->width, absorbDisplayPercent*wd + origx + 1*scale) - clippedX;
                clippedHt = MAX(0, MIN(currentBox->y + currentBox->height, GROUP_HEIGHT*scale + origy) - clippedY);
            }
            else
            {
                clippedX = origx +1*scale;
                clippedY = origy;
                clippedWd = absorbDisplayPercent*wd;
                clippedHt = hpBar_mm->height*hpHtSc;
            }
            BuildCBox(&hpBox, clippedX,clippedY, clippedWd, clippedHt);
            clipperPushCBox(&hpBox);
            display_sprite( hpBar_lm, origx+1*scale, origy, z+2, hpHtSc, hpHtSc, absorbColor );
            display_sprite( hpBar_ls, origx+1*scale, origy, z+3, hpHtSc, hpHtSc, alphaS );
            display_sprite( hpBar_lh, origx+1*scale, origy, z+4, hpHtSc, hpHtSc, alphaH );
            display_sprite( hpBar_mm, hpBar_lm->width*hpHtSc+origx+1*scale, origy, z+2, ((wd-(hpBar_rm->width + hpBar_lm->width)*hpHtSc)-2*scale)/hpBar_mm->width, hpHtSc, absorbColor );
            display_sprite( hpBar_ms, hpBar_lm->width*hpHtSc+origx+1*scale, origy, z+3, ((wd-(hpBar_rm->width + hpBar_lm->width)*hpHtSc)-2*scale)/hpBar_mm->width, hpHtSc, alphaS);
            display_sprite( hpBar_mh, hpBar_lm->width*hpHtSc+origx+1*scale, origy, z+4, ((wd-(hpBar_rm->width + hpBar_lm->width)*hpHtSc)-2*scale)/hpBar_mm->width, hpHtSc, alphaH );
            display_spriteFlip( hpBar_rm, hpBar_lm->width*hpHtSc+origx+1*scale + ((wd-(hpBar_rm->width +hpBar_lm->width)*hpHtSc)-2*scale), origy, z+2, hpHtSc, hpHtSc, absorbColor, FLIP_HORIZONTAL  );
            display_spriteFlip( hpBar_rs, hpBar_lm->width*hpHtSc+origx+1*scale + ((wd-(hpBar_rm->width +hpBar_lm->width)*hpHtSc)-2*scale), origy, z+3, hpHtSc, hpHtSc, alphaS, FLIP_HORIZONTAL  );
            display_spriteFlip( hpBar_rh, hpBar_lm->width*hpHtSc+origx+1*scale + ((wd-(hpBar_rm->width +hpBar_lm->width)*hpHtSc)-2*scale), origy, z+4, hpHtSc, hpHtSc, alphaH, FLIP_HORIZONTAL  );
            clipperPop();

            // display endurance
            display_sprite( endBar_l, origx+1*scale, origy + hpBar_mm->height*hpHtSc, z+2, endHtSc, endHtSc, CLR_WHITE );
            display_sprite( endBar_m, endBar_l->width*endHtSc+origx+1*scale, origy + hpBar_mm->height*hpHtSc, z+2, (end_percent*((wd-(endBar_r->width + endBar_l->width)*endHtSc)-2))/endBar_m->width, endHtSc, CLR_WHITE );
            display_sprite( endBar_r, endBar_l->width*endHtSc+origx+1*scale + (end_percent*((wd-(endBar_r->width +endBar_l->width)*endHtSc)-2*scale)), origy + hpBar_mm->height*hpHtSc, z+2, endHtSc, endHtSc, CLR_WHITE );
        }

        drawLeaguePlayerFrame(origx, origy, z+5, wd, GROUP_HEIGHT*scale, selected, active);
    }
}

//-----------------------------------------------------------------------------------------------
// Draw Buttons /////////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------------------------
#define DEFAULT_WD    250

typedef struct GroupDrawFormat
{
    char *toolTipTxt;
    int (*unlockedCode)(Entity*);        //    annoying legacy thing, we make this the opposite of locked code
    char *cmdParseString;
    void (*onClickCode)(void*);
    char *iconName;
    char *icon2Name;
} GroupDrawFormat;

static GroupDrawFormat groupDrawButtonData[][3] = {
    {    
        {"KickMemberToolTip",        team_targetIsMember,    NULL,    team_KickMember,    "League_Icon_Boot", "KickMember"    },
        {"PromoteToolTip",    team_targetIsMember,    NULL,    team_makeLeader,    "League_Icon_Team_Leader", "PromoteString"},
        {"FindToolTip",        NULL,                    "sea",    NULL,    "League_Icon_Mag_Glass", "FindString"}
    },
    {    {"WithDrawTeamToolTip",    NULL, "leagueWithdrawTeam",        NULL,    "League_Icon_Withdrawal_Arrow", "League_Icon_Team"},
        {"ToggleTeamLockToolTip",    NULL, "leagueToggleTeamLock",    NULL,    "League_Icon_Lock_Open", "League_Icon_Team"},
        {"PromoteStringToolTip",    team_targetIsMember,    NULL,    team_makeLeader,    "League_Icon_Team_Leader"},
    },
    {    {"KickMemberToolTip",        league_targetIsMember,    NULL,    league_KickMember,    "League_Icon_Boot"    },
        {"PromoteToolTip",    league_targetIsMember,    NULL,    league_makeLeader,    "League_Icon_League_Leader"    },
        {"FindToolTip",        NULL,                    "sea",    NULL,    "League_Icon_Mag_Glass"}
    },
};

static ToolTip buttonToolTip;
static int drawGroupButton(float x, float y, float z, float wd, float ht, float sc, int color, int isLocked, char*toolTipTxt, char *iconName, char *icon2Name, int currentWindow)
{
    
    AtlasTex *botMid = NULL;
    AtlasTex *lowerLeft = NULL;
    AtlasTex *leftMid = NULL;
    AtlasTex *lowerRight= NULL;
    AtlasTex *rightMid = NULL;
    AtlasTex *topMid = NULL;
    AtlasTex *upperLeft = NULL;
    AtlasTex *upperRight = NULL;
    AtlasTex *icon = atlasLoadTexture(iconName);
    AtlasTex *center = atlasLoadTexture("League_Button_Meat_Center");
    int iconColor = 0xffffffcc;
    float buttonSc = MAX(0.5f, sc*0.5f);
    float iconSc = 0.6f*sc;
    CBox box;
    
    x -= wd/2;
    BuildCBox(&box, x, y, wd, ht);

    if (isLocked)
    {
        iconColor = CLR_GREY;
    }
    if (mouseCollision(&box))
    {
        if (!isLocked)
        {
            iconSc = 0.7f *sc;
            iconColor = CLR_WHITE;
        }
        addToolTip( &buttonToolTip );
        setToolTipEx( &buttonToolTip, &box, textStd(toolTipTxt), 0, MENU_GAME, currentWindow, 0, TT_NOTRANSLATE );
    }

    botMid = atlasLoadTexture("League_Button_Stroke_Bottom");
    lowerLeft = atlasLoadTexture("League_Button_Stroke_LL");
    leftMid = atlasLoadTexture("League_Button_Stroke_Left");
    lowerRight = atlasLoadTexture("League_Button_Stroke_LR");
    rightMid = atlasLoadTexture("League_Button_Stroke_Right");
    topMid = atlasLoadTexture("League_Button_Stroke_Top");
    upperLeft = atlasLoadTexture("League_Button_Stroke_UL");
    upperRight = atlasLoadTexture("League_Button_Stroke_UR");

    drawLeagueButton(upperLeft, topMid, upperRight, leftMid, lowerLeft, botMid, lowerRight, rightMid, x, y, z++, wd, ht, buttonSc, color);

    //    arg. artists need to stick to a naming convention.
    botMid = atlasLoadTexture("League_Button_Meat_Bottom_Side");
    lowerLeft = atlasLoadTexture("League_Button_Meat_LL");
    leftMid = atlasLoadTexture("League_Button_Meat_Left_Side");
    lowerRight = atlasLoadTexture("League_Button_Meat_LR");
    rightMid = atlasLoadTexture("League_Button_Meat_Right_Side");
    topMid = atlasLoadTexture("League_Button_Meat_Top_Side");
    upperLeft = atlasLoadTexture("League_Button_Meat_UL");
    upperRight = atlasLoadTexture("League_Button_Meat_UR");

    display_sprite(center, x + upperLeft->width*buttonSc, y+1*buttonSc, z, (wd-(upperLeft->width+upperRight->width)*buttonSc)/center->width, (ht-(2*buttonSc))/center->height, color);
    display_sprite(center, x+1*buttonSc, y + upperLeft->height*buttonSc, z, (wd-(2*buttonSc))/center->width, (ht-(upperLeft->height+lowerLeft->height)*buttonSc)/center->height, color);
    drawLeagueButton(upperLeft, topMid, upperRight, leftMid, lowerLeft, botMid, lowerRight, rightMid, x, y, z++, wd, ht, buttonSc, color);

    botMid = atlasLoadTexture("League_Button_Shadow_Bottom_Mid");
    lowerLeft = atlasLoadTexture("League_Button_Shadow_LL");
    leftMid = atlasLoadTexture("League_Button_Shadow_L_Side");
    lowerRight = atlasLoadTexture("League_Button_Shadow_LR");
    rightMid = atlasLoadTexture("League_Button_Shadow_R_Side");
    topMid = atlasLoadTexture("League_Button_Highlight_Mid");        //    intentional duplicate of meat
    upperLeft = atlasLoadTexture("League_Button_Highlight_L");
    upperRight = atlasLoadTexture("League_Button_Highlight_R");

    drawLeagueButton(upperLeft, topMid, upperRight, leftMid, lowerLeft, botMid, lowerRight, rightMid, x, y, z++, wd, ht, buttonSc, CLR_WHITE);

    if (icon2Name)
    {
        AtlasTex *icon2 = atlasLoadTexture(icon2Name);
        display_sprite(icon2, x+(wd-(icon2->width+icon->width)*iconSc)/2, y+1*sc+(ht-icon2->height*iconSc)/2, z+5, iconSc, iconSc, iconColor);
        display_sprite(icon, x+(wd-(icon->width-icon2->width)*iconSc)/2, y+1*sc+(ht-icon->height*iconSc)/2, z+5, iconSc, iconSc, iconColor);
    }
    else
    {
        display_sprite(icon, x+(wd-icon->width*iconSc)/2, y+1*sc+(ht-icon->height*iconSc)/2, z+5, iconSc, iconSc, iconColor);
    }

    if (!isLocked && mouseClickHit(&box, MS_LEFT))
    {
        return D_MOUSEHIT;
    }
    return D_NONE;
}
static int drawButtonSet(Entity *e, float x, float y, float z, float wd, float scale, int color, int num_buttons_start, int currentSet, int currentWindow, int oldStyleTeamUI)
{
    int i;
    int num_buttons = num_buttons_start+ARRAY_SIZE(groupDrawButtonData[currentSet]);
    for (i = 0; i < ARRAY_SIZE(groupDrawButtonData[currentSet]); ++i)
    {
        GroupDrawFormat *currentButton = &groupDrawButtonData[currentSet][i];
        float xoff;
        int mouseHit;
        xoff = (i+1)*wd/(num_buttons+1);
        if (oldStyleTeamUI)
        {
            mouseHit = drawStdButton( x + xoff, y + 10*scale, z, wd/(num_buttons+1)-2*scale, 20*scale, color, 
                currentButton->icon2Name, scale, currentButton->unlockedCode ? !currentButton->unlockedCode(e) : 0);
        }
        else
        {
            
            mouseHit = drawGroupButton( x + xoff, y, z, wd/(num_buttons+1)-2*scale, 20*scale, scale, color, 
                currentButton->unlockedCode ? !currentButton->unlockedCode(e) : 0, currentButton->toolTipTxt, 
                currentButton->iconName, currentSet ? currentButton->icon2Name : NULL, currentWindow );
        }
        if (mouseHit == D_MOUSEHIT)
        {
            if (currentButton->cmdParseString)    cmdParse(currentButton->cmdParseString);
            if (currentButton->onClickCode)        currentButton->onClickCode(NULL);
        }
    }
    return num_buttons;
}
static float groupDrawButtons( Entity *e, float x, float y, float z, float wd, float scale, int color, int currentWindow, int oldStyleTeamUI )
{
    int num_buttons    = 1; // always have quit
    float xoff        = 0;
    float returnHt = 25*scale;

    switch (currentWindow)
    {
    case WDW_GROUP:
        {
            int mouseHit;
            if( e->teamup && e->db_id == e->teamup->members.leader && e->teamup->members.count > 1 )
            {
                num_buttons = drawButtonSet(e, x, y, z, wd, scale, color, 1, 0, currentWindow, oldStyleTeamUI);
            }
            xoff = num_buttons*wd/(num_buttons+1);
            if (oldStyleTeamUI)
            {
                mouseHit = drawStdButton( x + xoff + ((num_buttons==2)?10*scale:0), y+10*scale, z, wd/(num_buttons+1)-2*scale, 20*scale, color,
                    (e->pl->taskforce_mode&&num_buttons<4)?"QuitTaskForce":"QuitTeam", scale, 0);
            }
            else
            {
                mouseHit = drawGroupButton( x + xoff + ((num_buttons==2)?10*scale:0), y, z, wd/(num_buttons+1)-2*scale, 20*scale, scale, color, amOnArenaMap(), "QuitToolTip","League_Icon_Withdrawal_Exit", NULL, currentWindow);
            }
            if( D_MOUSEHIT == mouseHit )
                team_Quit(0);
        }
        break;
    case WDW_LEAGUE:
        if( e->league && e->db_id == e->league->members.leader && e->league->members.count > 1 )
        {
            num_buttons = drawButtonSet(e, x, y, z, wd, scale, color, 1, 2, currentWindow, oldStyleTeamUI);
        }
        xoff = num_buttons*wd/(num_buttons+1);
        if( D_MOUSEHIT == drawGroupButton( x + xoff + ((num_buttons==2)?10*scale:0), y, z, wd/(num_buttons+1)-2*scale, 20*scale, scale, color, amOnArenaMap(), "QuitToolTip","League_Icon_Withdrawal_Exit", NULL, currentWindow))
            cmdParse("leaveTeam");
        if (gViewFlags & GEV_GRID)
        {
            AtlasTex *dirArrows = atlasLoadTexture("teamup_arrow_contract");
            if (drawGenericRotatedButton(dirArrows, x+ wd-25*scale, y+17*scale, z, scale*0.85f, color, color, 1, -PI*0.5f) == D_MOUSEHIT)
            {
                s_numRows++;
            }
            if (drawGenericRotatedButton(dirArrows, x+ wd-10*scale, y, z, scale*0.85f, color, color, 1, PI*1.f) == D_MOUSEHIT)
            {
                s_numRows--;
            }
        }
        break;
    }
    return returnHt;
}

#define OPTION_HT    18
#define SPACER        5
static int tmp_lfg = 0;
static UIEdit *gCommentText;

void searchClearComment()
{
     if( gCommentText )
    {
        uiEditClear(gCommentText);
        uiEditDestroy(gCommentText);
        gCommentText = 0;
    }
}

static void drawSearchOptions( float x, float y, float z, float wd, float ht, float sc, int currentWindow )
{
    int i;
    Entity * e = playerPtr();
    static bool send_comment = 0;
    static int currentWindowFocus = 0;
    CBox box;

    // this is a hacky little override to keep selector in same place until
    // server resends actual value.
      if( tmp_lfg != e->pl->lfg )
        e->pl->lfg = tmp_lfg;

    if( !comboLFGSearch )
    {
        comboLFGSearch = calloc(1, sizeof(ComboBox));
        comboboxTitle_init( comboLFGSearch, R10, R10, 1, 280, 30, 270, currentWindow );

        for( i = 0; i < ARRAY_SIZE(searchOptions); i++ )
        {
            comboboxTitle_add( comboLFGSearch, 0, atlasLoadTexture(searchOptions[i].texName), searchOptions[i].displayName, searchOptions[i].displayTitle, searchOptions[i].bitmask, searchOptions[i].color, searchOptions[i].icon_color,0  );
        }

        if(gCommentText)
            uiEditClear(gCommentText);
    }

    if(!gCommentText)
    {
        gCommentText = uiEditCreateSimple(e->pl->comment, 128, &game_12, CLR_WHITE, editInputHandler);
        uiEditSetClicks(gCommentText, "type2", "mouseover");
    }

    font( &game_12 );
    font_color( CLR_WHITE, CLR_WHITE );
    comboLFGSearch->wdw = currentWindow;
    if ((e->teamup ? e->teamup->members.count : 0) > 1 || (e->league ? e->league->members.count : 0) > 1)
    {
        comboLFGSearch->locked = 1;
        e->pl->lfg = 0;
    }
    else
    {
        comboLFGSearch->locked = 0;
    }
    combobox_display( comboLFGSearch );

    if( comboLFGSearch->newlySelectedItem )
    {
        char buf[32];
        comboLFGSearch->newlySelectedItem = 0;

        e->pl->lfg = comboLFGSearch->elements[comboLFGSearch->currChoice]->id;
        if( e->pl->lfg & LFG_NONE )
            e->pl->lfg = 0;

        tmp_lfg = e->pl->lfg;
        sprintf( buf, "lfgset %i", e->pl->lfg );
        cmdParse( buf );
    }

    y += 60*sc;

     prnt( x+R10*2, y, z, sc, sc, "CommentString" );
     BuildCBox( &box, x+R10, y, wd, 35*sc );

       if( mouseCollision(&box) )
        drawFlatFrame( PIX3, R10, x+R10, y, z, wd, 35*sc, sc, 0xffffff22, 0xffffff44 );
    else
        drawFlatFrame( PIX3, R10, x+R10, y, z, wd, 35*sc, sc, 0xffffff22, 0xffffff22 );

    if( mouseClickHit( &box, MS_LEFT) )
    {
        uiEditTakeFocus(gCommentText);
        send_comment = true;
        currentWindowFocus = currentWindow;
    }

    if( !mouseCollision(&box) && mouseDown(MS_LEFT) )
        uiEditReturnFocus(gCommentText);

    if (currentWindowFocus == currentWindow)
    {
           uiEditEditSimple(gCommentText,x+R10*2, y+PIX3*sc, z+2, wd-R10*2, (35-PIX3*2)*sc, &game_12, INFO_HELP_TEXT, FALSE);
    }
    else
    {
        int returnFocus = uiEditHasFocus(gCommentText);
        uiEditReturnFocus(gCommentText);
        uiEditEditSimple(gCommentText,x+R10*2, y+PIX3*sc, z+2, wd-R10*2, (35-PIX3*2)*sc, &game_12, INFO_HELP_TEXT, FALSE);
        if (returnFocus)
            uiEditTakeFocus(gCommentText);
    }
 

    if( !uiEditHasFocus(gCommentText) && send_comment )
    {
        char buf[1024];
        char * string = uiEditGetUTF8Text(gCommentText);

        if( string && string[0] )
            strchrReplace( string, '\n', ' ' );
        sprintf( buf, "comment %s", string?string:"" );
         cmdParse( buf );
        send_comment = false;

        if(string)
            strcpy( e->pl->comment, uiEditGetUTF8Text(gCommentText) );
        else
            e->pl->comment[0] = 0;
    }
}

//-----------------------------------------------------------------------------------------------
// Draw Window //////////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------------------------

#define GROUP_MEMBER_WD    150
#define LEAGUE_MEMBER_WD 110
#define GW_X_INSET    6
#define DEATH_COLOR 0x888888ff
#define BUFF_SPEED    15
#define LEAGUE_GROUP_FOCUS -2
#define LEAGUE_GROUP_ALL -1
#define OLD_MEMBER_WD 220
static int getTeamMemberIndex(Entity *e, int originalIndex, int currentWindow, int currentTeam)
{
    int teamRosterIndex = originalIndex;
    switch (currentWindow)
    {
    case WDW_LEAGUE:
        {
            int i;
            if (currentTeam == LEAGUE_GROUP_FOCUS)
            {
                if (EAINTINRANGE(originalIndex, e->league->focusGroupRosterIndex))
                {
                    for (i = 0; i < eaSize(&e->league->teamRoster); ++i)
                    {
                        int returnIndex;
                        if ((returnIndex = eaiFind(&e->league->teamRoster[i],e->league->focusGroupRosterIndex[originalIndex])) != -1)
                        {
                            return e->league->teamRoster[i][returnIndex];
                        }
                    }
                }
                return -1;
            }
            else
            {
                if ((currentTeam == LEAGUE_GROUP_ALL) && (gViewFlags & GEV_FULL))
                {
                    int i = teamRosterIndex / MAX_TEAM_MEMBERS;
                    teamRosterIndex = teamRosterIndex - (i *MAX_TEAM_MEMBERS);
                    if (teamRosterIndex >= eaiSize(&e->league->teamRoster[i]))
                    {
                        return -1;
                    }
                    return e->league->teamRoster[i][teamRosterIndex];
                }
                else
                {
                    for (i = 0; i < eaSize(&e->league->teamRoster); ++i)
                    {
                        if (eaiSize(&e->league->teamRoster[i]) > teamRosterIndex)
                        {
                            if (currentTeam >= 0 && (i != currentTeam))
                            {
                                return -1;
                            }
                            return e->league->teamRoster[i][teamRosterIndex];
                        }
                        else
                        {
                            teamRosterIndex -= eaiSize(&e->league->teamRoster[i]);
                        }
                    }
                }
            }
            assert(0);    //    we couldn't find the index =/
            return teamRosterIndex;
        }
    case WDW_GROUP:
    default:                        //    intentional fallthrough
        return teamRosterIndex;        //    for the team tab, the index is the same
    }
}
static void initMemberInfo(Entity *e, int currentWindow, int currentTeam, int *max_members, int *num_members, TeamMembers **members)
{
    switch (currentWindow)
    {
    case WDW_GROUP:
        *max_members = MAX_TEAM_MEMBERS;
        if(e->teamup && e->teamup->members.count > 1)
        {
            *num_members = e->teamup->members.count;
            *members = &e->teamup->members;
        }
        break;
    case WDW_LEAGUE:
        *max_members = MAX_LEAGUE_MEMBERS;
        if ((currentTeam == LEAGUE_GROUP_ALL) && (gViewFlags&GEV_FULL))
        {
            if(e->league && e->league->members.count > 1)
            {
                *num_members = eaSize(&e->league->teamRoster)*MAX_TEAM_MEMBERS;
            }
        }
        else
        {
            if(e->league && e->league->members.count > 1)
            {
                *num_members = e->league->members.count;
            }
        }
        *members = &e->league->members;
        break;
    }
}
static char *getMapName(int currentWindow, int memberIndex)
{
    switch (currentWindow)
    {
    case WDW_GROUP:
        return textStd(teamMapNames[memberIndex]);
    case WDW_LEAGUE:
        return textStd(leagueMapNames[memberIndex]);
    default:
        assert(0);    //    invalid currentWindow
        return NULL;
    }
}
static int numMembersToShowWidth(int currentWindow, int currentTeam)
{
    Entity *e = playerPtr();
    assert((currentWindow == WDW_GROUP) || (currentWindow == WDW_LEAGUE));
    assert(currentTeam < MAX_LEAGUE_TEAMS);
    if (e && ((currentWindow == WDW_GROUP) || (currentWindow == WDW_LEAGUE)) && (currentTeam < MAX_LEAGUE_TEAMS))
    {
        if (e && e->league && (currentWindow == WDW_LEAGUE) && (currentTeam < MAX_LEAGUE_TEAMS))
        {
            if ((currentTeam == LEAGUE_GROUP_ALL) && (gViewFlags&GEV_GRID))
            {
                return ceil((eaSize(&e->league->teamRoster)*1.f/s_numRows));
            }
        }
    }
    return 1;
}
static void numMembersFillGridSizes(Entity *e, int **sizeArrays)
{
    if (e && e->league)
    {
        int i;
        for (i = 0; i < eaSize(&e->league->teamRoster); ++i)
        {
            int numMembers = eaiSize(&e->league->teamRoster[i]);
            if (i < s_numRows)
                eaiPush(sizeArrays, numMembers);
            else
                (*sizeArrays)[i%s_numRows] = MAX((*sizeArrays)[i%s_numRows], numMembers);
        }
        for (i = 1; i < eaiSize(sizeArrays); ++i)
        {
            (*sizeArrays)[i] += (*sizeArrays)[i-1];
        }
    }
    
}
static int numMembersToShowHeight(Entity *e, int currentWindow, int currentTeam)
{
    assert(e);
    assert((currentWindow == WDW_GROUP) || (currentWindow == WDW_LEAGUE));
    assert(currentTeam < MAX_LEAGUE_TEAMS);
    if (e && ((currentWindow == WDW_GROUP) || (currentWindow == WDW_LEAGUE)) && (currentTeam < MAX_LEAGUE_TEAMS))
    {
        switch (currentWindow)
        {
        case WDW_GROUP:
            return e->teamup ? e->teamup->members.count : 0;
            break;
        case WDW_LEAGUE:
            if (!e->league)
                return 0;
            if (currentTeam == LEAGUE_GROUP_ALL)
            {
                if ((gViewFlags&GEV_GRID) && (eaSize(&e->league->teamRoster) > 1))
                {
                    if (gViewFlags&GEV_FULL)
                    {
                        return (MAX_TEAM_MEMBERS*s_numRows);
                    }
                    else
                    {
                        int sum = 0;
                        int *sizesArray = NULL;
                        numMembersFillGridSizes(e, &sizesArray);
                        sum = eaiLast(&sizesArray);
                        eaiDestroy(&sizesArray);
                        return sum;
                    }
                }
                else
                {
                    if (gViewFlags & GEV_FULL)
                    {
                        return MAX_TEAM_MEMBERS;
                    }
                    return MIN(e->league->members.count, MAX_TEAM_MEMBERS);
                }
            }
            else if (currentTeam == LEAGUE_GROUP_FOCUS)
            {
                return MIN(eaiSize(&e->league->focusGroupRosterIndex), MAX_TEAM_MEMBERS);
            }
            else
            {
                int i;
                int max = 0;
                for (i = 0; i < eaSize(&e->league->teamRoster); ++i)
                {
                    max = MAX(eaiSize(&e->league->teamRoster[i]), max);
                }
                return max;
            }
            break;
        }
    }
    return 0;
}
typedef struct GroupMemberDisplayInfo
{
    int memberIndex;
    int memberDBid;
    float hp; // percentage: Cur HP / Max HP
    float absorb; // percentage: Cur Absorb / Max HP
    float endurance; // percentage: Cur End / Max End
    int fcolor;
    int selected;
    bool bOnMap;
    bool bDead;
    bool bHit;
    bool bLockStatus;
    int bLeader;
    int team;
}GroupMemberDisplayInfo;
static void initGroupMemberInfo(GroupMemberDisplayInfo *memberInfo)
{
    memberInfo->memberIndex = 0;
    memberInfo->hp = 0;
    memberInfo->absorb = 0;
    memberInfo->endurance = 0;
    memberInfo->fcolor = 0;
    memberInfo->selected = 0;
    memberInfo->bOnMap = 1;
    memberInfo->bDead = 0;
    memberInfo->bHit = 0;
    memberInfo->bLeader = 0;
    memberInfo->bLockStatus = 0;
    //    debugName gets set externally
    memberInfo->team = 0;
}
static void detGroupMemberTeam(Entity *e, int currentWindow, GroupMemberDisplayInfo *memberInfo)
{
    if (currentWindow == WDW_LEAGUE)
    {
        int i, j;
        memberInfo->bLeader = (e->league->members.leader == e->league->members.ids[memberInfo->memberIndex]) ? GROUP_LEADER_ALL : GROUP_LEADER_NONE;
        for (i = 0; (i < eaSize(&e->league->teamRoster)); ++i)
        {
            for (j = 0; (j < eaiSize(&e->league->teamRoster[i])); ++j)
            {
                if (memberInfo->memberIndex == e->league->teamRoster[i][j])
                {
                    memberInfo->team = i;
                    if (j == 0)        //    first person is leader
                    {
                        if (eaiSize(&e->league->teamRoster[i]) > 1)
                        {
                            memberInfo->bLeader = MAX(memberInfo->bLeader, GROUP_LEADER_TEAM);
                        }
                        else
                        {
                            memberInfo->bLeader = MAX(memberInfo->bLeader, GROUP_LEADER_SOLO);
                        }
                    }
                    return;
                }
            }
        }
    }
    else if (currentWindow == WDW_GROUP)
    {
        memberInfo->bLeader = (e->teamup->members.leader == e->teamup->members.ids[memberInfo->memberIndex]) ? GROUP_LEADER_ALL : GROUP_LEADER_NONE;
    }
}

static int detGroupMemberHealth(GroupMemberDisplayInfo *memberInfo, Entity *group_mate, int currentWindow, int currentTeam, archetypeIconCache *ac)
{
    if( memberInfo->bOnMap && group_mate )
    {
        if( group_mate->pchar->attrCur.fHitPoints == 0 )
        {
            if (currentWindow == WDW_GROUP)
            {
                setToolTipEx( &teamMemberTip[memberInfo->memberIndex][0], 
                    &teamMemberTip[memberInfo->memberIndex][0].bounds, textStd("deadTip"), 
                    &GroupTipParent, MENU_GAME, currentWindow, 0, TT_NOTRANSLATE );
            }
            memberInfo->fcolor = 0xffffff88;
            return 0;
        }
        else
        {
            memberInfo->hp = group_mate->pchar->attrCur.fHitPoints / group_mate->pchar->attrMax.fHitPoints;
            memberInfo->absorb = group_mate->pchar->attrMax.fAbsorb / group_mate->pchar->attrMax.fHitPoints;
            memberInfo->endurance = group_mate->pchar->attrCur.fEndurance / group_mate->pchar->attrMax.fEndurance;
            memberInfo->fcolor = getHealthColor(entity_TargetIsInVisionPhase(playerPtr(), group_mate), memberInfo->hp);

            if (currentTeam == LEAGUE_GROUP_ALL)
            {
                if (memberInfo->team % 2)
                {
                    memberInfo->fcolor &= 0xaaaaaaff;
                }
            }

            if (currentWindow == WDW_GROUP)
            {
            setToolTipEx( &teamMemberTip[memberInfo->memberIndex][1], 
                &teamMemberTip[memberInfo->memberIndex][1].bounds, textStd(group_mate->pchar->pclass->pchDisplayName), 
                &GroupTipParent, MENU_GAME, currentWindow, 0, TT_NOTRANSLATE );
            }
        }
    }
    else
    {
        memberInfo->fcolor = 0xffffff88;

        if( ac )
        {
            if (currentWindow == WDW_GROUP)
            {
                setToolTipEx( &teamMemberTip[memberInfo->memberIndex][1], &teamMemberTip[memberInfo->memberIndex][1].bounds, textStd(ac->className), &GroupTipParent, MENU_GAME, currentWindow, 0, TT_NOTRANSLATE );
            }
        }
    }
    return 1;
}

static void groupMemberDrag(GroupMemberDisplayInfo *memberInfo, AtlasTex * icon)
{
    if(memberInfo && icon)
    {
        TrayObj to = {0};
        to.type = kTrayItemType_GroupMember;
        to.invIdx = memberInfo->memberDBid;
        to.idDetail = memberInfo->team;
        trayobj_startDragging(&to,icon, NULL);
    }
}

static void handleGroupMemberDragging(float x, float y, float z, float wd, float ht, int isGridView, Entity *memberToSwapWith, int teamToMoveTo)
{
    if (cursor.dragging && cursor.drag_obj.type == kTrayItemType_GroupMember)
    {
        if(!isDown(MS_LEFT) )
        {
            if (isGridView)
            {
                if (!(INRANGE(teamToMoveTo, 0, MAX_LEAGUE_TEAMS)))
                {
                    //    didn't drop into a team box, so give them a new team
                    Entity *e = playerPtr();
                    CBox windowBox;
                    BuildCBox(&windowBox, x, y, wd, ht);
                    if (mouseCollision(&windowBox))
                    {
                        teamToMoveTo = eaSize(&e->league->teamRoster);    //    create a new team for them
                    }
                }
                //    sanity check in case we gave them a number over the max team 
                //  (when moving when the teams are already full)
                if (INRANGE(teamToMoveTo, 0, MAX_LEAGUE_TEAMS))    
                {
                    if (league_CanMoveMember((void*)teamToMoveTo))
                    {
                        league_MoveMembers((void*)teamToMoveTo);
                    }
                }
            }
            trayobj_stopDragging();
        }
    }
    else if (memberToSwapWith)
    {
        league_SetLeftSwapMember(NULL);        //    left swap is the current target (the one being dragged)
        current_target = memberToSwapWith;
        if (league_CanSwapMembers(NULL))
        {
            league_SwapMembers(NULL);
        }
    }

}
static void drawGroupMemberBar(float x, float yp, float z, float member_wd, float scale, GroupMemberDisplayInfo *memberInfo,
                                char *txt, int selected, int active, AtlasTex *icon, int memberIndex, int displayIndex, int isSelf, 
                                int currentWindow, int oldStyleTeamUI)
{
    int fcolor;
    float hp, absorb, end;
    int offMap, bLeader, isLocked;
    int team;
    if (memberInfo)
    {
        fcolor = memberInfo->fcolor | 0xff;
        hp = memberInfo->hp;
        absorb = memberInfo->absorb;
        end = memberInfo->endurance;
        offMap = !memberInfo->bOnMap;
        bLeader = memberInfo->bLeader;
        isLocked = memberInfo->bLockStatus;
        team = memberInfo->team;
    }
    else
    {
        fcolor = 0xffffff88 | 0xff;
        hp = absorb = end = 0.0f;
        offMap = 1;
        bLeader = isLocked = 0;
        team = displayIndex/MAX_TEAM_MEMBERS;
    }

    if (currentWindow == WDW_LEAGUE)
    {
        AtlasTex *coloredTabShadow = atlasLoadTexture("League_Team_Indicator_Shadow");
        AtlasTex *coloredTabMeat = atlasLoadTexture( "League_Team_Indicator_Meat" );
        AtlasTex *coloredTabHighlight = atlasLoadTexture("League_Team_Indicator_Highlight");
        float tabScale = GROUP_HEIGHT *scale/ coloredTabMeat->height;
        float tabXScale = tabScale *2;
        int teamColors[] = {CLR_MAGENTA, CLR_YELLOW, CLR_CYAN, CLR_RED, CLR_ORANGE, CLR_NAVY};
        float coloredTabWd = coloredTabMeat->width *tabXScale;
        STATIC_ASSERT(ARRAY_SIZE(teamColors) == MAX_LEAGUE_TEAMS);

        display_sprite( coloredTabMeat, x, yp, z+10, tabXScale, tabScale, teamColors[team] );
        display_sprite( coloredTabShadow, x, yp, z+11, tabXScale, tabScale, teamColors[team] );
        display_sprite( coloredTabHighlight, x, yp, z+12, tabXScale, tabScale, teamColors[team] );

        if (bLeader)
        {
            font(&game_12);
            font_color(CLR_WHITE, CLR_GREY);
            cprntEx(x+coloredTabWd/2, yp+coloredTabMeat->height*tabScale/2, z+15, tabScale*0.8f, tabScale*0.8f, CENTER_X | CENTER_Y, "%i", team+1);
        }
        x += coloredTabWd;
        member_wd -= coloredTabWd;
        if (isLocked)
        {
            AtlasTex *closedLock = atlasLoadTexture("League_Icon_Lock_Closed");
            float iconSc = scale *0.5f;
            display_sprite( closedLock, x+member_wd-closedLock->width*iconSc, yp, z+12, iconSc, iconSc, teamColors[team] );
        }
    }
    drawGroupInterior( x, yp, ++z, member_wd, scale, hp, absorb, end, fcolor, txt, selected, active, offMap, bLeader, icon, 
        memberIndex, isSelf, false, currentWindow, oldStyleTeamUI );
    {
        char temp[8]; //arbitrary length, 10^7
        float number_height_offset = (GROUP_HEIGHT-1)*scale;
        assert(displayIndex < 10000000);

        itoa( displayIndex + 1, temp, 10 );
        font( &game_9 );
        font_color( 0xffffff88, 0xffffff88 );
        cprnt( x + (oldStyleTeamUI ? ((GW_X_INSET-2) *scale) : (member_wd-15*scale)), yp + number_height_offset, z+5, scale*0.5f, scale*0.5f, temp );
    }


}

#define TAB_WD 25
static void drawGroupMembers(Entity *e, float x_orig, float y, float z, float wd, float ht, float scale, int color, int bcolor, 
                              int flipped, int currentWindow, int currentTeam, TeamMembers *members,int num_members, int activePlayerDbid, int oldStyleTeamUI)
{
    static ScrollBar sb;
    int i;
    float y_orig = y;
    float docHt = 0;
    CBox cbox;
    int leagueAllTab = (currentWindow == WDW_LEAGUE) && (currentTeam == LEAGUE_GROUP_ALL)  && e->league && (e->league->members.count > 1);
    int showSB =  ((currentWindow == WDW_LEAGUE) && (currentTeam == LEAGUE_GROUP_FOCUS)) || (leagueAllTab  && !(gViewFlags&GEV_GRID));
    int shownCount = 0;
    int currentDisplayTeam = -1;
    Entity *memberToSwapWith = NULL;
    int memberSwapMouseOverId = 0;
    int teamToMoveTo = -1;
    int *sizesArray = NULL;
    if (showSB)
    {
        if (!flipped)
        {
            x_orig+= 5*scale;
        }
        wd-= 5*scale;
        BuildCBox(&cbox, x_orig-1000, y, wd+2000, ht);
        y_orig -= sb.offset;
        clipperPushCBox(&cbox);
        BuildCBox(&cbox, x_orig, y, wd, ht);
    }
    if (leagueAllTab && (gViewFlags&GEV_GRID) && !(gViewFlags&GEV_FULL))
    {
        numMembersFillGridSizes(e, &sizesArray);
    }
    for( i = 0; i < num_members; i++ )
    {
        CBox box;
        float xp, member_wd = (oldStyleTeamUI ? OLD_MEMBER_WD-15 : ((currentWindow == WDW_LEAGUE) ? LEAGUE_MEMBER_WD : GROUP_MEMBER_WD))*scale;
        GroupMemberDisplayInfo memberInfo;
        Entity * group_mate;
        archetypeIconCache *ac = 0;
        AtlasTex *icon = 0;
        int memberIndex = getTeamMemberIndex(e, i, currentWindow, currentTeam);
        float x = x_orig;
        float yp = y_orig;
        if (memberIndex < 0)
        {
            if (leagueAllTab && (gViewFlags&GEV_FULL))
            {
                if (gViewFlags & GEV_GRID)
                {
                    float startTeamY = y_orig + (memberInfo.team%s_numRows)*(MAX_TEAM_MEMBERS+0.5f)*(GROUP_HEIGHT+GROUP_SPACE)*scale;
                    float startTeamX = x + GW_X_INSET*scale+((memberInfo.team)/s_numRows)*(((currentWindow == WDW_LEAGUE) ? LEAGUE_MEMBER_WD : GROUP_MEMBER_WD)+15)*scale;
                    yp = startTeamY + shownCount*(GROUP_HEIGHT+GROUP_SPACE)*scale;
                    drawGroupMemberBar(startTeamX, yp, z, member_wd, scale, NULL, textStd("leagueEmptyString"), 
                        0, 0, NULL, i, i, 0, currentWindow, oldStyleTeamUI);
                }
                else
                {
                    //    draw the empty's
                    docHt += (GROUP_HEIGHT+GROUP_SPACE)*scale;
                    yp = y_orig + (((i/MAX_TEAM_MEMBERS)*0.5f)+shownCount)*(GROUP_HEIGHT+GROUP_SPACE)*scale;
                    drawGroupMemberBar(x + GW_X_INSET*scale, yp, z, member_wd, scale, NULL,
                        textStd("leagueEmptyString"), 0, 0, NULL, i, i, 0, currentWindow, oldStyleTeamUI);
                }
                shownCount++;
            }
            continue;
        }

        initGroupMemberInfo(&memberInfo);
        memberInfo.memberIndex = memberIndex;
        memberInfo.memberDBid = members->ids[memberIndex];
        memberInfo.bLockStatus = ((currentWindow == WDW_LEAGUE) && (e->league)) ? e->league->lockStatus[memberIndex] : 0;

        ac = aic_AddSafe( memberInfo.memberDBid );
        group_mate = entFromDbId( memberInfo.memberDBid );
        memberInfo.bOnMap = members->onMapserver[memberIndex];

        if( group_mate && current_target && current_target->db_id == group_mate->db_id )
            memberInfo.selected = 1;
        else if( gSelectedDBID == memberInfo.memberDBid )
            memberInfo.selected = 1;

        detGroupMemberTeam( e, currentWindow, &memberInfo);
        if ((detGroupMemberHealth(&memberInfo, group_mate, currentWindow, currentTeam, ac) == 0) || (!memberInfo.bOnMap && ac))
        {
            icon = ac->deadIcon;
        }
        else if (ac)
        {
            icon = ac->icon;
        }
        if( (currentWindow == WDW_GROUP) && e->teamup->team_mentor && (e->teamup->team_mentor != e->teamup->members.ids[memberIndex]) )
        {
            xp = x + SIDEKICK_INDENT*scale;
            member_wd = ((oldStyleTeamUI ? OLD_MEMBER_WD-15 : ((currentWindow == WDW_LEAGUE) ? LEAGUE_MEMBER_WD : GROUP_MEMBER_WD)) - SIDEKICK_INDENT)*scale;
        }
        else
        {
            xp = x;
        }

        if (leagueAllTab && gViewFlags&GEV_GRID)
        {
            float teamSpace;
            float startTeamY;
            if (gViewFlags&GEV_FULL)
            {
                teamSpace = MAX_TEAM_MEMBERS*(GROUP_HEIGHT+GROUP_SPACE)*scale;
                startTeamY = y_orig + (memberInfo.team%s_numRows)*(teamSpace+(0.5f*(GROUP_HEIGHT+GROUP_SPACE)*scale));
            }
            else
            {
                int rowIndex = memberInfo.team%s_numRows;
                int prevNumRows = (rowIndex ? sizesArray[rowIndex-1] : 0);
                startTeamY = y_orig + (prevNumRows ? (prevNumRows + rowIndex*0.5f) : 0)*(GROUP_HEIGHT+GROUP_SPACE)*scale;
                teamSpace = (sizesArray[rowIndex]-prevNumRows)*(GROUP_HEIGHT+GROUP_SPACE)*scale;
            }
            xp += ((memberInfo.team)/s_numRows)*(((currentWindow == WDW_LEAGUE) ? LEAGUE_MEMBER_WD : GROUP_MEMBER_WD)+15)*scale;
            if (currentDisplayTeam != memberInfo.team)
            {
                currentDisplayTeam = memberInfo.team;
                shownCount = 0;
                if (eaSize(&e->league->teamRoster) > 1)
                {
                    CBox teamBox;
                    int teamBColor = 0x77777733;
                    BuildCBox(&teamBox, xp, startTeamY,(((currentWindow == WDW_LEAGUE) ? LEAGUE_MEMBER_WD : GROUP_MEMBER_WD)+10)*scale, teamSpace);
                    if (mouseCollision(&teamBox) && (cursor.drag_obj.type == kTrayItemType_GroupMember) && cursor.drag_obj.idDetail != currentDisplayTeam)
                    {
                        teamBColor = 0x77777777;
                        if (!isDown(MS_LEFT))
                        {
                            teamToMoveTo = memberInfo.team;
                            //    trayobj_stopDragging();        //    dont always drop the drag since other things below may use it
                        }
                    }
                    drawFlatFrameBox(PIX2, R6, &teamBox, z, 0, teamBColor);
                }
            }
            yp = startTeamY + shownCount*(GROUP_HEIGHT+GROUP_SPACE)*scale;
        }
        else
        {
            float y_off = 0.0f;
            if (leagueAllTab && !(gViewFlags&GEV_GRID) && (gViewFlags&GEV_FULL))
            {
                float startTeamY = y_orig + memberInfo.team*(MAX_TEAM_MEMBERS+0.5f)*(GROUP_HEIGHT+GROUP_SPACE)*scale;
                if (currentDisplayTeam != memberInfo.team)
                {
                    currentDisplayTeam = memberInfo.team;
                    if (eaSize(&e->league->teamRoster) > 1)
                    {
                        CBox teamBox;
                        int teamBColor = 0x77777733;
                        BuildCBox(&teamBox, xp, startTeamY,(((currentWindow == WDW_LEAGUE) ? LEAGUE_MEMBER_WD : GROUP_MEMBER_WD)+10)*scale, MAX_TEAM_MEMBERS*(GROUP_HEIGHT+GROUP_SPACE)*scale);
                        if (mouseCollision(&teamBox) && (cursor.drag_obj.type == kTrayItemType_GroupMember) && cursor.drag_obj.idDetail != currentDisplayTeam)
                        {
                            teamBColor = 0x77777777;
                            if (!isDown(MS_LEFT))
                            {
                                teamToMoveTo = memberInfo.team;
                                //    trayobj_stopDragging();        //    dont always drop the drag since other things below may use it
                            }
                        }
                        drawFlatFrameBox(PIX2, R6, &teamBox, z, 0, teamBColor);
                    }
                }
                y_off = (memberInfo.team*0.5f);
            }
            yp = y_orig + (y_off + shownCount)*(GROUP_HEIGHT+GROUP_SPACE)*scale;
        }

        shownCount++;

        BuildCBox( &box, xp + GW_X_INSET*scale, yp, member_wd, GROUP_HEIGHT*scale );
        if( mouseCollision( &box ) )
        {
            memberInfo.bHit = true;
            if(mouseClickHit(&box, MS_LEFT ) )
            {
                team_select(members, memberIndex+1);
            }

            if( !isDown(MS_LEFT) && group_mate  )
            {
                if( cursor.drag_obj.type == kTrayItemType_Inspiration ||
                    cursor.drag_obj.type == kTrayItemType_SpecializationInventory )
                {
                    gift_SendAndRemove( group_mate->svr_idx, &cursor.drag_obj );
                }
                if (cursor.drag_obj.type == kTrayItemType_GroupMember)
                    memberToSwapWith = group_mate;
                trayobj_stopDragging();
            }

            if (cursor.dragging && (cursor.drag_obj.type == kTrayItemType_GroupMember) && (cursor.drag_obj.invIdx != memberInfo.memberDBid ))
            {
                memberSwapMouseOverId = memberInfo.memberDBid;
            }
        }

        if ( mouseClickHit(&box, MS_RIGHT) )
        {
            memberInfo.selected = team_select(members, memberIndex+1);

            if( memberInfo.selected )
            {
                if ( group_mate && current_target)
                    contextMenuForTarget(current_target);
                else
                    contextMenuForOffMap();
            }
        }
        
        if (leagueAllTab && gViewFlags&GEV_GRID && e->league)
        {
            if (memberInfo.bOnMap && (e->league->members.leader == e->db_id) && 
                mouseLeftDrag(&box) && (e->db_id != memberInfo.memberDBid))
            {
                memberInfo.selected = team_select(members, memberIndex+1);
                groupMemberDrag(&memberInfo, ac ? ac->icon : atlasLoadTexture("white") );
            }
        }

        // --------------------
        // draw the group item
        drawGroupMemberBar(xp + GW_X_INSET*scale, yp, z, member_wd, scale, &memberInfo, 
            group_mate ? group_mate->name : members->names[memberIndex], 
            memberInfo.selected ? 1 : ((memberSwapMouseOverId == memberInfo.memberDBid) ? 2 : 0),
            memberInfo.memberDBid == activePlayerDbid, icon, memberIndex, i, memberInfo.memberDBid == e->db_id, currentWindow, oldStyleTeamUI);

        // Draw buff list
        if( gShowBuffs && !leagueAllTab)
        {
            float wdAdjust = (flipped && (currentWindow == WDW_LEAGUE)) ? TAB_WD*MIN(1.f, scale) : 0;
            if( !memberInfo.bOnMap || !group_mate )
            {
                char * mapname = getMapName(currentWindow, memberIndex);
                
                font_color( CLR_WHITE, CLR_WHITE );
                if( flipped )
                {
                    float ywd = str_wd(font_grp, scale, scale, mapname );
                    prnt( x - (PIX3*scale + ywd + 15*scale + wdAdjust), yp + TXT_YOFF*scale, z, scale, scale, mapname );
                }
                else
                    prnt( x+wd+PIX3*scale, yp + TXT_YOFF*scale, z, scale, scale, mapname );
            }
            else if (group_mate)
            {
                linedrawBuffs( kBuff_Group, group_mate, x-wdAdjust, yp, z, wd, ht, scale, flipped, &GroupTipParent );
            }
        }
        docHt += (GROUP_HEIGHT+GROUP_SPACE)*scale;
    }
    if (showSB)
    {
        int sbXPos = x_orig;
        clipperPop();
        wd+= 5*scale;
        if (!flipped)
        {
            x_orig -=5*scale;
        }
        else
        {
            sbXPos += wd;
        }

        if (leagueAllTab && gViewFlags&GEV_FULL)
        {
            docHt += eaSize(&e->league->teamRoster) * 0.5f *(GROUP_HEIGHT+GROUP_SPACE) *scale;
        }

        if ((num_members > MAX_TEAM_MEMBERS) || (gViewFlags & GEV_FULL))
        {
            doScrollBarEx(&sb, ht-20*scale,docHt, sbXPos+PIX3*scale, y+10*scale, z, &cbox, NULL, 1.f, 1.f);
        }
        else
        {
            sb.offset = 0;
        }
    }
    eaiDestroy(&sizesArray);
    //    do all the move/swaps after
    handleGroupMemberDragging(x_orig, y, z, wd, ht, leagueAllTab && gViewFlags&GEV_GRID, memberToSwapWith, teamToMoveTo);
}

static float drawCommandButtons(float x, float y, float z, float wd, float ht, float scale, int color, int bcolor, int currentWindow, int oldStyleTeamUI)
{
    Entity *e = playerPtr();
    float returnHt = 0;
    // now add a few buttons
    if ( window_getMode( currentWindow ) == WINDOW_DISPLAYING )
    {
        int showButtons = 0;
        switch (currentWindow)
        {
        case WDW_GROUP:
            showButtons = e->teamup && (e->teamup->members.count > 1);
            break;
        case WDW_LEAGUE:
            showButtons = e->league && (e->league->members.count > 1);
            if (showButtons)
                drawHorizontalLine(x+PIX3*scale, y-2*scale, wd-(2*PIX3*scale), z, 2.f, color);
            break;
        }
        if( showButtons )
        {
            returnHt = groupDrawButtons( e, x-10*scale, y, z, wd+20*scale, scale, color, currentWindow, oldStyleTeamUI );
            e->pl->lfg = tmp_lfg = 0;
            if (comboLFGSearch)
                comboLFGSearch->currChoice = 0;
        }
        else
        {
            float twd = 25;
            if( (currentWindow == WDW_GROUP) && e->pl->taskforce_mode )
            {
                if( D_MOUSEHIT == drawStdButton( x + wd/2, y + ht/2, z, 150*scale, twd*scale, color, "QuitTaskForce", scale, 0 ) )
                {
                    cmdParse( "leaveTeam" );
                }
            }
            else
            {
                float ty = y + (GROUP_HEIGHT+10)*scale/2;
                // draw search selector
                if( window_getMode(currentWindow)==WINDOW_DISPLAYING)
                    drawSearchOptions( x, y, z, wd - R10*2*scale, ht, scale, currentWindow );

                BuildCBox( &buttonTip[TEAM_TIP_FIND].bounds, x + (DEFAULT_WD - DEFAULT_WD/4 - 50)*scale, ty - 12*scale, 100*scale, twd*scale );
                setToolTipEx( &buttonTip[TEAM_TIP_FIND], &buttonTip[TEAM_TIP_FIND].bounds, textStd("FindMemberTip"), &GroupTipParent, MENU_GAME, currentWindow, 0, TT_NOTRANSLATE );
                if( D_MOUSEHIT == drawStdButton( x + (wd/2), y + ht - 35*scale, z, wd/2, 30*scale, color, "FindMember", scale, 0 ) )
                {
                    cmdParse( "sea +lfg" );
                    lfg_setSearchMethod(currentWindow);
                }
            }
        }
    }
    return returnHt;
}
//
//
// -AB: added numbers to group members related to shift-select number :2005 Feb 02 03:02 PM
static int drawCurrentWindow(float x, float y, float z, float wd, float ht, float scale, int color, int bcolor, int flipped, int currentWindow, int currentTeam, int oldStyleTeamUI)
{
    int num_members = 0;
    Entity * e = playerPtr();
    AtlasTex * buffArrow;
    int max_members;
    TeamMembers *members = NULL;
    float yp =0;
    int returnHt = -1;
    int groupDisplayHt = 0;
    int leagueAllTab = ((currentWindow == WDW_LEAGUE) && (currentTeam == LEAGUE_GROUP_ALL));
    int *arrowMode = leagueAllTab ? &gViewFlags : &gShowBuffs;

    assert((currentWindow == WDW_GROUP) || (currentWindow == WDW_LEAGUE));
    initMemberInfo(e, currentWindow, currentTeam, &max_members, &num_members, &members);

    // if we are drawing bars
    if( num_members )
    {
        e->pl->lfg = 0;

        // do buff the arrow
        if( (!((*arrowMode)&1) && !flipped) || (((*arrowMode)&1) && flipped) )
            buffArrow = atlasLoadTexture( "teamup_arrow_expand.tga" );
        else
            buffArrow = atlasLoadTexture( "teamup_arrow_contract.tga" );

        if( window_getMode( currentWindow ) == WINDOW_DISPLAYING )
        {
            if (D_MOUSEHIT == drawGenericButton( buffArrow, flipped?(x+5*scale):(x + wd - (buffArrow->width+5)*scale), y + (ht-(GROUP_HEIGHT+buffArrow->height)*scale)/(leagueAllTab ? 4 : 2), z, scale, color, CLR_GREEN, 1 ) )
            {
                (*arrowMode) ^= 1;
                if (arrowMode == &gShowBuffs)
                {
                    sendTeamBuffMode( gShowBuffs );
                }
                collisions_off_for_rest_of_frame = TRUE;
            }
            if (leagueAllTab)
            {
                if( (!((*arrowMode)&GEV_FULL) && !flipped) || (((*arrowMode)&GEV_FULL) && flipped) )
                    buffArrow = atlasLoadTexture( "teamup_arrow_expand.tga" );
                else
                    buffArrow = atlasLoadTexture( "teamup_arrow_contract.tga" );
                if (D_MOUSEHIT == drawGenericButton( buffArrow, flipped?(x+5*scale):(x + wd - (buffArrow->width+5)*scale), y + ((ht-(GROUP_HEIGHT+buffArrow->height)*scale)*2)/3, z, scale, color, CLR_GREEN, 1 ) )
                {
                    (*arrowMode) ^= GEV_FULL;
                    initMemberInfo(e, currentWindow, currentTeam, &max_members, &num_members, &members);
                    collisions_off_for_rest_of_frame = TRUE;
                }
            }
        }

        if( window_getMode( currentWindow) == WINDOW_DISPLAYING )
        {
            int numToShow = numMembersToShowHeight(e, currentWindow, currentTeam);
            int leagueAllTab = (currentWindow == WDW_LEAGUE) && (currentTeam == LEAGUE_GROUP_ALL);
            int offset = 0;
            if (leagueAllTab)
            {
                if (gViewFlags & GEV_GRID)
                {
                    offset = s_numRows*0.5f*scale*(GROUP_HEIGHT+GROUP_SPACE);
                }
            }
            groupDisplayHt = (GROUP_HEIGHT+GROUP_SPACE)*(numToShow)*scale;
            returnHt = groupDisplayHt+offset;
        }

        clearToolTip( &buttonTip[TEAM_TIP_SEEK] );
        clearToolTip( &buttonTip[TEAM_TIP_FIND] );
    }
    else // only  buttons
    {
        int i;
        // clear tooltips
        if (currentWindow == WDW_GROUP)
        {
            for( i = 0; i < max_members; i++ )
            {
                int j;
                for( j = 0; j < MAX_MEMBER_TIPS; j++ )
                    clearToolTip( &teamMemberTip[i][j]);
            }
        }

        // clear cache
        if( aic )
        {
            while( eaSize( &aic ) )
            {
                archetypeIconCache * tmp = eaRemove( &aic, 0 );
                if( tmp )
                    free( tmp );
            }
        }
    }

     if( flipped )
        x += 15*scale;

    drawGroupMembers(e, x, y, z, wd - (flipped ? (15*scale) : 0), MIN(ht, groupDisplayHt), scale, color, bcolor, flipped,currentWindow, currentTeam, members, num_members, e->teamup ? e->teamup->activePlayerDbid : 0, oldStyleTeamUI);

    BuildCBox( &GroupTipParent.box, x, y, wd, ht);

     return returnHt;
}
static uiTabControl *tabTeam = NULL;
static uiTabData lastSelectedTab = NULL;
void league_updateTabCount()
{
    //    this will implicitly remake the tabs
    //    when we clear, we will remake them
    if (tabTeam)
        lastSelectedTab = uiTabControlGetSelectedData(tabTeam);
    uiTabControlDestroy(tabTeam);
    tabTeam = NULL;
}
static int wdwMouseDrag[2] = {0, 0};
static void s_rescaleWindow(float x, float y, float wd, float ht, float sc, int currentWindow)
{
    CBox box, inside;
    int rad = (R10 + PIX3)*sc;
    int VOID_COL_WD = 7;
    int wdwIndex = 0;
    // lower right
    //----------------------------------------------------------
    BuildCBox( &box, x + wd - rad, y + ht - rad, rad, rad );
    BuildCBox( &inside, x + wd - rad, y + ht - rad, VOID_COL_WD, VOID_COL_WD );

    switch (currentWindow)
    {
    case WDW_GROUP:
        wdwIndex = 0;
        break;
    case WDW_LEAGUE:
        wdwIndex = 1;
        break;
    }
    if( mouseCollision(&box ) && !mouseCollision(&inside) )
    {
        setCursor( "cursor_win_diagresize_foreward.tga", NULL, FALSE, 12, 12 );

        if( mouseDown( MS_LEFT) )
        {
            wdwMouseDrag[wdwIndex] = 1;
        }
    }
    if (wdwMouseDrag[wdwIndex] && mouseUp(MS_LEFT))
    {
        wdwMouseDrag[wdwIndex] = 0;
        window_UpdateServer(currentWindow);
    }

    if (wdwMouseDrag[wdwIndex])
    {
        float m = ht/wd;
        float b = y - m*x;
        float newY = m*gMouseInpCur.x + b;
        float newScaleByX;
        float newScaleByY;
        newScaleByX = sc *(gMouseInpCur.y - y) / ht;
        newScaleByX = CLAMP(newScaleByX, 0.5f, 5.f);
        newScaleByY = sc * (gMouseInpCur.x - x) / wd;
        newScaleByY = CLAMP(newScaleByY, 0.5f, 5.f);
        winDefs[currentWindow].loc.sc = MIN(newScaleByX, newScaleByY);
        collisions_off_for_rest_of_frame = true;
    }
}
int groupWindow()
{
    float x, y, z, wd, ht, sc;
    int w, h;
    int color, bcolor, flipped =0;
    static int init = 0;
    int currentTeam = LEAGUE_GROUP_ALL;
    int currentWindow = gCurrentWindefIndex;
    int drawHt;
    int wdAdjust = 0;
    float minTabHt = 0;
    float yoff = 0;
    int oldStyleTeamUI = 0;
    Entity *e = playerPtr();

    if ((winDefs[WDW_GROUP].loc.locked && window_getMode(WDW_GROUP) == WINDOW_DOCKED) || (currentWindow == WDW_GROUP))
    {
        s_timerTracker = (s_timerTracker + 1) % PULSE_TIMER_MAX;
    }
    if( !window_getDims( currentWindow, &x, &y, &z, &wd, &ht, &sc, &color, &bcolor) )
        return 0;

    windowClientSizeThisFrame( &w, &h );
    if( x + wd/2 > w/2 )
        flipped = true;

    // add tooltips
    if( !init )
    {
        int i, j;
        for( i = 0; i < MAX_TEAM_MEMBERS; i++ )
        {
            for( j = 0; j < MAX_MEMBER_TIPS; j++ )
                addToolTip( &teamMemberTip[i][j] );
        }

        for( i = 0; i < TEAM_TIP_TOTAL; i++ )
            addToolTip( &buttonTip[i] );

        addToolTip(&s_activePlayerTip);

        init = true;
    }
    if (currentWindow == WDW_LEAGUE)
    {
        if (!tabTeam && e->league)
        {
            int i;
            tabTeam = uiTabControlCreate(TabType_Undraggable, 0, 0,0,0,0);
            uiTabControlAdd(tabTeam, textStd("AllString"), (int*) LEAGUE_GROUP_ALL );
            if (eaiSize(&e->league->focusGroupRosterIndex))
            {
                uiTabControlAdd(tabTeam, textStd("FocusGroupString"), (int*) LEAGUE_GROUP_FOCUS );
            }
            for (i = 0; i < MIN(MAX_LEAGUE_TEAMS, eaSize(&e->league->teamRoster)); ++i)
            {
                char buff[20];
                sprintf(buff, "%i", i+1);
                uiTabControlAdd(tabTeam, buff, (int*)i );
            }
            uiTabControlSelect(tabTeam, lastSelectedTab ? ((int *)lastSelectedTab) : ((int*)LEAGUE_GROUP_ALL ));
        }

        if (e && e->league && (e->league->members.count > 1))
        {
            s_numRows = CLAMP(s_numRows, 1, eaSize(&e->league->teamRoster));
            if( e->teamup && e->db_id == e->teamup->members.leader && e->teamup->members.count > 1 )
            {
                drawButtonSet(e, x, y+5*sc, z, wd, sc, color, 0, 1, currentWindow, oldStyleTeamUI);
                yoff += 30*sc;
                drawHorizontalLine(x+PIX3*sc, y+(yoff ? yoff : (R10*sc)) - PIX3*sc, wd-(2*PIX3*sc), z, 2.f, color);
            }

            wdAdjust = TAB_WD*sc;
            currentTeam = (int)drawTabControl(tabTeam, x+PIX3*sc, y+(yoff ? yoff : R10*sc), z, wd, ht-(yoff ? yoff : (2*R10*sc)), sc, color, (CLR_GREEN&0xFFFFFF00)|(color&0xff), TabDirection_Vertical); 
            minTabHt = uiTabControlGetMinDrawHeight(tabTeam) + (yoff ? 0 : (2*R10*sc));
        }
    }
    else
    {
        oldStyleTeamUI = optionGet(kUO_UseOldTeamUI);
    }
    if (!oldStyleTeamUI && currentWindow == WDW_GROUP && e->teamup && e->teamup->members.count > 1)
    {
        if (winDefs[WDW_GROUP].loc.locked)
        {
            sc *= 1.3f;
        }
    }
    drawFrame( PIX3, R10, x, y, z-5, wd, ht, sc, color, bcolor );
    drawHt = drawCurrentWindow(x+wdAdjust, y+(yoff ? yoff : (R10*sc)), z, wd-wdAdjust, ht-(yoff ? yoff : (R10*sc)), sc, color, bcolor, flipped, currentWindow, currentTeam, oldStyleTeamUI);
    drawHt = MAX(minTabHt, drawHt);
    drawHt += drawHt ? ((yoff ? yoff : R10*sc)+ PIX3*sc) : 0.0f;
    drawHt += drawCommandButtons(x, y+drawHt, z, wd, ht-drawHt, sc, color, bcolor, currentWindow, oldStyleTeamUI);
    if( window_getMode( currentWindow) == WINDOW_DISPLAYING )
    {
        int newWd, newHt;
        if (drawHt <= 0)
        {
            newWd = 300*sc;
            newHt = (3*(GROUP_HEIGHT+42))*sc;
        }
        else
        {
            int displayWd =numMembersToShowWidth(currentWindow, currentTeam) *(oldStyleTeamUI ? OLD_MEMBER_WD : (((currentWindow == WDW_LEAGUE) ? LEAGUE_MEMBER_WD : GROUP_MEMBER_WD)+15))*sc;
            newWd = displayWd + wdAdjust+ GW_X_INSET*sc +2*PIX3*sc;
            newHt = drawHt;
            if (currentWindow == WDW_LEAGUE)
            {
                if (winDefs[WDW_LEAGUE].loc.locked)
                {
                    if (newWd > winDefs[WDW_LEAGUE].parent->loc.wd)
                    {
                        winDefs[WDW_LEAGUE].loc.locked = 0;
                    }
                }
            }
        }
        window_setDims( currentWindow, x, y, newWd, newHt);
        if (winDefs[currentWindow].loc.locked == 0)
        {
            s_rescaleWindow(x, y, newWd, newHt, sc, currentWindow);
        }
    }

    return 0;
}

/* End of File */
 
