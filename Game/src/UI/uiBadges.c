/***************************************************************************
 *     Copyright (c) 2004-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/components/earray.h>

#include "entity/entity.h"
#include "entity/entPlayer.h"
#include "player/badges.h"
#include "player/badges_client.h"

#include "gameComm/wdwbase.h"
#include "UI/uiUtil.h"
#include "uiSMFView.h"
#include "UI/uiWindows.h"
#include "UI/uiUtilGame.h"
#include "UI/uiScrollBar.h"
#include "UI/uiInput.h"
#include "UI/uiToolTip.h"
#include "UI/uiGame.h"
#include "UI/sprite/sprite_base.h"
#include "UI/sprite/sprite_text.h"
#include "UI/uiClipper.h"
#include "player/player.h"
#include "cmdparse/cmdgame.h"

#include "UI/uiBadges.h"
#include "player/badges_client.h"

#include "formatter/smf_main.h"

#include "UI/sprite/sprite_base.h"
#include "UI/sprite/sprite_text.h"
#include "UI/sprite/sprite_font.h"
#include "graphics/textureatlas.h"
#include <utilitieslib/utils/mathutil.h>

#include <utilitieslib/language/MessageStore.h>
#include <utilitieslib/utils/timing.h>
#include "entity/Supergroup.h"

#include "edit/ClickToSource.h"
#include "language/commonLangUtil.h"
#include <utilitieslib/language/MessageStoreUtil.h>
#include "UI/uiContextMenu.h"
#include "UI/uiComboBox.h"
#include "player/pophelp.h"
#include "UI/uiPopHelp.h"
#include "clientcomm/clientcomm.h"
#include "player/inventory_client.h"
#include "graphics/ttFontUtil.h"

#define FORCE_OPACITY           0xE6
#define NO_OPACITY              0xffffff00
#define LINE_HT                 17

static TextAttribs s_taDefaults =
{
    /* piBold        */  (int *)0,
    /* piItalic      */  (int *)0,
    /* piColor       */  (int *)0xffffffff,
    /* piColor2      */  (int *)0,
    /* piColorHover  */  (int *)0xffffffff,
    /* piColorSelect */  (int *)0,
    /* piColorSelectBG*/ (int *)0x333333ff,
    /* piScale       */  (int *)(int)(1.0f*SMF_FONT_SCALE),
    /* piFace        */  (int *)&smfSmall,
    /* piFont        */  (int *)0,
    /* piAnchor      */  (int *)0,
    /* piLink        */  (int *)0x80e080ff,
    /* piLinkBG      */  (int *)0,
    /* piLinkHover   */  (int *)0x66ff66ff,
    /* piLinkHoverBG */  (int *)0,
    /* piLinkSelect  */  (int *)0,
    /* piLinkSelectBG*/  (int *)0x666666ff,
    /* piOutline     */  (int *)1,
    /* piShadow      */  (int *)0,
};

static TextAttribs s_taTitle =
{
    /* piBold        */  (int *)0,
    /* piItalic      */  (int *)0,
    /* piColor       */  (int *)0x00deffff,
    /* piColor2      */  (int *)0,
    /* piColorHover  */  (int *)0x00deffff,
    /* piColorSelect */  (int *)0,
    /* piColorSelectBG*/ (int *)0x333333ff,
    /* piScale       */  (int *)(int)(1.0f*SMF_FONT_SCALE),
    /* piFace        */  (int *)&game_14,
    /* piFont        */  (int *)0,
    /* piAnchor      */  (int *)0,
    /* piLink        */  (int *)0x00deffff,
    /* piLinkBG      */  (int *)0,
    /* piLinkHover   */  (int *)0x00deffff,
    /* piLinkHoverBG */  (int *)0,
    /* piLinkSelect  */  (int *)0,
    /* piLinkSelectBG*/  (int *)0x666666ff,
    /* piOutline     */  (int *)1,
    /* piShadow      */  (int *)0,
};

typedef struct BadgeDisplay {
    SMFBlock *titleBlock;
    SMFBlock *textBlock;
    int height;
} BadgeDisplay;

typedef union CollectionCategory {
    void *voidPointer;
    struct {
        S16 collection;
        S16 category;
    } data;
} CollectionCategory;

static BadgeDisplay ** badgeDisplays = NULL;
static BadgeDisplay ** sgBadgeDisplays = NULL;

static bool s_bReparse = 0;

static CollectionType gSelectedCollection;
static BadgeType gSelectedTab;
static int gSelectedBadge;
static int gSupergroupBadges;

static ContextMenu *badgeContextMenu = NULL;
static ContextMenu *badgeTypeSelectMenu = NULL;
static ContextMenu *badgeTypeSelectSubMenus[kCollectionType_Count];

static ContextMenu* badgeMonitorContextMenu = NULL;

//static ComboBox comboBadges;
//static ComboCheckboxElement **cce = NULL;

static bool comboVillainText = false;

void badgeReparse(void)
{
    s_bReparse = true;
}

static void openBadgeMonitorWindow(void *data)
{
    window_setMode(WDW_BADGEMONITOR, WINDOW_GROWING);
}

const BadgeDef* getBadgeDef(const BadgeMonitorInfo* badgeInfo)
{
    if (!badgeInfo || !badgeInfo->iIdx)
    {
        return NULL;
    }

    return badge_GetAnyBadgeByIdx(badgeInfo->iIdx);
}

static const char* badgeMonitorGetStopDisplayText(void *data)
{
    int badgeMonitorIdx = (int)data;

    const Entity* entity = playerPtr();
    const BadgeMonitorInfo* monitorList = entity->pl->badgeMonitorInfo;
    const BadgeDef* badge = getBadgeDef(monitorList + badgeMonitorIdx);
    if (!badge)
    {
        return "";
    }

    const char* title = printLocalizedEnt(badge->pchDisplayTitle[ENT_IS_HERO(entity) ? 0 : 1], entity);
    return textStd("BadgeMonitorStopDisplayString", title);
}

static void badgeMonitor_Move(int badgeMonitorIdx, int step)
{
    if ( (badgeMonitorIdx + step < 0) || (badgeMonitorIdx + step >= MAX_BADGE_MONITOR_ENTRIES) )
    {
        return;
    }
    
    // int i;
    Entity* entity = playerPtr();
    BadgeMonitorInfo* monitorList = entity->pl->badgeMonitorInfo;
    BadgeMonitorInfo* monitoredBadge = monitorList + badgeMonitorIdx;
    BadgeMonitorInfo* monitoredBadgeOther = monitoredBadge + step;

    // Only replace with other if it's an existing badge (otherwise it's the end)
    if (!monitoredBadgeOther->iIdx)
    {
        return;
    }

    // swap the badges indices
    int tmp = monitoredBadgeOther->iIdx;
    monitoredBadgeOther->iIdx = monitoredBadge->iIdx;
    monitoredBadge->iIdx = tmp;

    // Update server on new order
    badgeMonitor_SendToServer(entity);
}

static void badgeMonitor_MoveUp(void* data)
{
    badgeMonitor_Move((int)data, -1);
}

static void badgeMonitor_MoveDown(void* data)
{
    badgeMonitor_Move((int)data, 1);
}

static void badgeMonitor_stopDisplay(void* data)
{
    int badgeMonitorIdx = (int)data;
    Entity* entity = playerPtr();
    BadgeMonitorInfo* monitorList = entity->pl->badgeMonitorInfo;

    BadgeMonitorInfo* prev = monitorList + badgeMonitorIdx;
    for (int i = badgeMonitorIdx + 1; i < MAX_BADGE_MONITOR_ENTRIES; i++)
    {
        BadgeMonitorInfo* next = monitorList + i;
        if (!next->iIdx)
        {
            prev->iIdx = 0;
            break;
        }
        prev->iIdx = next->iIdx;
        prev = next;
    }
    badgeMonitor_SendToServer(entity);
}

static void badgeMonitor_stopDisplayAll(void *data)
{
    Entity* entity = playerPtr();

    for (int i = 0; i < MAX_BADGE_MONITOR_ENTRIES; i++)
    {
        BadgeMonitorInfo* monitoredBadge = entity->pl->badgeMonitorInfo + i;
        if (!monitoredBadge)
        {
            break;
        }
        monitoredBadge->iIdx = 0;
    }
    badgeMonitor_SendToServer(entity);
}

static void setCategory(CollectionType collectionType, BadgeType category)
{
    gSelectedCollection = collectionType;
    gSelectedTab = category;
    badgeReparse();
}

static void badgeMonitor_focusBadge(void* data)
{
    int badgeMonitorIdx = (int)data;
    Entity* entity = playerPtr();
    const BadgeMonitorInfo* monitoredBadge = entity->pl->badgeMonitorInfo + badgeMonitorIdx;
    const BadgeDef* badgeDef = getBadgeDef(monitoredBadge);
    if (!badgeDef)
    {
        return;
    }

    // 1. show the badges/collect window
    window_setMode(WDW_BADGES, WINDOW_GROWING);
    window_bringToFront(WDW_BADGES);

    // 2. choose the category in it
    setCategory(badgeDef->eCollection, badgeDef->eCategory);

    // 3. scroll until the badgead
    // TODO implement (how?)
}

static void initBadgeMonitorContextMenu()
{
    if (badgeMonitorContextMenu)
    {
        return;
    }

    badgeMonitorContextMenu = contextMenu_Create(0);
    contextMenu_addTitle(badgeMonitorContextMenu, "BadgeMonitorString");
    contextMenu_addCode(badgeMonitorContextMenu, alwaysAvailable, 0, badgeMonitor_focusBadge, 0, "FocusBadgeMonitorString", 0);
    contextMenu_addVariableTextCode(badgeMonitorContextMenu, alwaysAvailable, 0, badgeMonitor_stopDisplay, 0, badgeMonitorGetStopDisplayText, 0, NULL);
    contextMenu_addCode(badgeMonitorContextMenu, alwaysAvailable, 0, badgeMonitor_MoveUp, 0, "MoveUpString", 0);
    contextMenu_addCode(badgeMonitorContextMenu, alwaysAvailable, 0, badgeMonitor_MoveDown, 0, "MoveDownString", 0);
    contextMenu_addDivider(badgeMonitorContextMenu);
    contextMenu_addCode(badgeMonitorContextMenu, alwaysAvailable, 0, badgeMonitor_stopDisplayAll, 0, "StopDisplayAllString", 0);
}

static void addToBadgeMonitorWindow(void *data)
{
    int badge_idx = (int)(data);

    if(badge_idx)
    {
        if(badgeMonitor_AddInfo(playerPtr(), badge_idx, gSupergroupBadges ? true : false))
        {
            badgeMonitor_SendToServer(playerPtr());
        }
    }
}

static int canBadgeBeAddedToMonitorWindow(void *data)
{
    int retval = 0;
    int badge_idx = (int)data;
    U32 field;

    if(badge_idx)
    {
        // Figure out where the badge progress is stored. We have the badge
        // index from its definition, we just need to know whether it's a
        // supergroup badge or a personal badge.
        if(gSupergroupBadges && gSelectedTab != kBadgeType_MostRecent)
        {
            field = playerPtr()->supergroup->badgeStates.eaiStates[badge_idx];
        }
        else
        {
            field = playerPtr()->pl->aiBadges[badge_idx];
        }

        if(badgeMonitor_CanInfoBeAdded(playerPtr(), badge_idx, gSupergroupBadges ? true : false) && BADGE_IS_VISIBLE(field) && !BADGE_IS_OWNED(field))
        {
            retval = CM_AVAILABLE;
        }
    }

    return retval;
}

static int superGroupVisible(void *data)
{
    if (!playerPtr()->supergroup_id || !playerPtr()->supergroup || !playerPtr()->supergroup->badgeStates.eaiStates)
    {
        return CM_HIDE;
    } else {
        return CM_AVAILABLE;
    }
}


static void setCollection(void *data)
{
    gSelectedCollection = (CollectionType) data;
    badgeReparse();
}

static void setCategoryCallback(void* data)
{
    CollectionCategory cc;

    cc.voidPointer = data;
    setCategory((CollectionType)cc.data.collection, (BadgeType)cc.data.category);
}

/**********************************************************************func*
* badge_display
*
*/
#define TITLE_HT    18
#define BADGE_SPACE    5
#define TAB_HT        20
#define BAR_HEIGHT 5

static ToolTip s_badgeTip;

static void displayProgressBar(float x, float y, float z, float wd, float sc, float percent)
{
    AtlasTex *bar = white_tex_atlas;
    AtlasTex *cap = atlasLoadTexture("healthbar_framemain_R.tga");

    display_sprite(bar, x, y, z, sc*wd/bar->width, sc*BAR_HEIGHT/bar->height, 0x00feffa0);
    display_sprite(bar, x+sc*1, y+sc*1, z, sc*(wd-2)/bar->width, sc*(BAR_HEIGHT-2)/bar->height, CLR_BLACK);

    display_sprite(bar, x+sc*1, y+sc*1, z, sc*percent*wd/bar->width, sc*(BAR_HEIGHT-2)/bar->height, 0x2b5cd2ff);
}

void badge_Select( int idx )
{
    char tmp[128];

    badge_setTitleId( playerPtr(), idx );
 
    sprintf( tmp, "set_title_id %d", idx);
    cmdParse( tmp );
}

static float badge_display( float x, float y, float z, float sc, float wd, 
                           BadgeDisplay *badgeDisplay, const BadgeDef * badge, int badgeField, int i, U32 badgeTime )
{
    AtlasTex     *icon;
    AtlasTex     *overlay = NULL;
    int            bProgress = false, 
                color = CLR_WHITE, 
                fcolor = CLR_NORMAL_FOREGROUND, 
                fbcolor = CLR_NORMAL_BACKGROUND,
                buttonRet = D_NONE; 
    float         titleWd, ty, tx, ht = TITLE_HT*sc;
    static float last_wd = 0;
    const char *pchTitle = "UnknownBadge";
    char        *pchButton = NULL;
    int idxType = ENT_IS_VILLAIN(playerPtr()) ? 1 : 0;
    CBox        box;

    if( !badgeField && g_DebugBadgeDisplayMode == 0 ) 
    {
        // user knows nothing of this badge
        badgeDisplay->height = 0;
        return 0;
    }

    PERFINFO_AUTO_START("top", 1);
    icon   = atlasLoadTexture(badge->pchIcon[idxType]);
    ty = y + PIX3*2*sc;
    tx = x + PIX3*2*sc;

    if(BADGE_IS_OWNED(badgeField))
    {
        pchTitle = printLocalizedEnt(badge->pchDisplayTitle[idxType],playerPtr());
    }
    else if (badge && badge->eCollection != kCollectionType_Badge && badge->eCollection != kCollectionType_Supergroup)
    {
        pchTitle = printLocalizedEnt(badge->pchDisplayTitle[idxType],playerPtr());
    }
    else if (g_DebugBadgeDisplayMode == 1 || g_DebugBadgeDisplayMode == 2)
    {
        pchTitle = badge->pchName;
    }

    if(BADGE_IS_OWNED(badgeField))
    {
        if(g_pchBadgeButton[badge->iIdx])
            pchButton = g_pchBadgeButton[badge->iIdx];
    }
    else
    {
        if(BADGE_IS_KNOWN(badgeField) || g_DebugBadgeDisplayMode == 1)
        {
            color = CLR_BLACK; // just show black outline
            overlay = atlasLoadTexture("badge_question_mark.tga");
        }
        else if(BADGE_IS_VISIBLE(badgeField) || g_DebugBadgeDisplayMode == 2)
        {
            icon = atlasLoadTexture("badge_question_mark.tga");
        }

        if(BADGE_COMPLETION(badgeField)>0)
        {
            bProgress = true;
        }
    }

    PERFINFO_AUTO_STOP_START("middle", 1);

    // expand icon column wd to fit largest icon
    if( last_wd < icon->width )
        last_wd = icon->width;

    // display the title
    tx += last_wd*sc + R10*sc;

    font( &game_14 );
    font_color(0x00deffff, 0x00deffff);

    // Title
    titleWd = str_wd(font_grp, sc, sc, pchTitle);
    if( pchTitle && titleWd < wd-(last_wd+4*PIX3+R10*2)*sc ) // don't need to wrap it
    {
        prnt( tx, ty + ht, z+1, sc, sc, pchTitle );
    }
    else 
    {
        s_taTitle.piScale = (int *)((int)(sc*SMF_FONT_SCALE));
        ht += smf_ParseAndDisplay(badgeDisplay->titleBlock, textStd(pchTitle), tx, ty, z+1, wd-(last_wd/sc+4*PIX3+R10)*sc, 100*sc, 
                                    s_bReparse, 0, &s_taTitle, NULL, 0, true);
        titleWd = wd-last_wd;
    }

    PERFINFO_AUTO_STOP_START("middle2", 1);

    // show progress bar
    if( bProgress )
    {
        // The completion value is now calibrated in millionths rather than
        // percentage points.
        displayProgressBar(tx, ty + ht + PIX3+PIX3/2, z+1, wd-(last_wd+4*PIX3+R10*2)*sc, sc, BADGE_COMPLETION(badgeField)/1000000.f );
        ht += PIX3*3;
    }

    PERFINFO_AUTO_STOP_START("middle3", 1);

    // display the description
    s_taDefaults.piScale = (int *)((int)(sc*SMF_FONT_SCALE));
    ht += smf_ParseAndDisplay(badgeDisplay->textBlock, gSupergroupBadges?g_pchSgBadgeText[badge->iIdx]:g_pchBadgeText[badge->iIdx], 
                                tx, ty + ht, z+1, wd-(last_wd+4*PIX3+R10)*sc, 100*sc, s_bReparse, 0, 
                                &s_taDefaults, NULL, 0, true);

    if (CTS_SHOW_BADGES)
        ClickToSourceDisplay(tx, y+12*sc, z+1, 0, 0xffffffff, gSupergroupBadges?g_pchSgBadgeFilename[badge->iIdx]:g_pchBadgeFilename[badge->iIdx], NULL, CTS_TEXT_REGULAR);

    // display the reward button if necessary
    if(pchButton)
    {
        int bOnArchitect = playerPtr() && playerPtr()->onArchitect && game_state.mission_map;
        buttonRet = drawStdButton(tx + 5*(wd-(last_wd+4*PIX3+R10)*sc)/6, ty + ht + PIX3*2*sc + 8*sc, z+1, (wd-(last_wd+4*PIX3+R10)*sc)/3, 16*sc, fcolor, pchButton, sc, bOnArchitect);
        if(buttonRet == D_MOUSEHIT)
        {
            char tmp[32];
            sprintf(tmp, "badge_button_use %i", badge->iIdx);
            cmdParse(tmp);
        }
        ht += PIX3*2*sc + 16*sc;
    }

    // determine actual ht
    ht = MAX( ht, icon->height )+PIX3*4*sc;

    PERFINFO_AUTO_STOP_START("middle4", 1);

    // fix up the tooltip
    BuildCBox( &box, x, y, wd, ht );

    if( mouseCollision(&box) )
    {
        char msg[500] = {0};

        if( badgeTime > 0 )
        {
            char *category = badge_CategoryGetName( playerPtr(), badge->eCollection, badge->eCategory );
            char timestr[100];

            // Make sure we have something to display even if the badge is
            // in an unknown category somehow.
            if( !category )
            {
                category = "MostRecentString";
            }


            timerMakeDateStringFromSecondsSince2000( timestr, badgeTime );
            msPrintf( menuMessages, SAFESTR(msg), "BadgeRecentToolTip", timestr, category );
        }
        else if( bProgress && BADGE_COMPLETION( badgeField ) < 1000000 )
        {
            if( badge->iProgressMaxValue > 0 )
            {
                // The value is millionths, so dividing by 10,000 turns it into
                // the percentage the text needs. Float to int conversion rounds
                // down, so we add 0.5 to make it round out for the real progress
                // number.
                msPrintf( menuMessages, SAFESTR(msg), "BadgeToolTipRange", (int)(0.5f + (((float)(BADGE_COMPLETION(badgeField)) * (float)(badge->iProgressMaxValue)) / 1000000.0f)), BADGE_COMPLETION(badgeField) / 10000.0f, badge->iProgressMaxValue );
            }
            else
            {
                // The value is millionths, so dividing by 10,000 turns it into
                // the percentage the text needs.
                msPrintf( menuMessages, SAFESTR(msg), "BadgeToolTip", BADGE_COMPLETION(badgeField) / 10000.0f );
            }
        }

        if( msg[0] )
        {
            setToolTipEx( &s_badgeTip, &box, msg, NULL, MENU_GAME, WDW_BADGES, 1, TT_NOTRANSLATE );
        }
    }

    // check for mouse collision and mouseover
    BuildCBox( &box, x, y, wd, ht);

    // Menu and monitor window not ready yet
    if( mouseClickHit( &box, MS_RIGHT ) )
    {
        contextMenu_displayEx( badgeContextMenu, (void *)( badge->iIdx ) );
    }

    // now we know the ht, check for mouse collision
    if( BADGE_IS_OWNED(badgeField))
    {

        if( mouseCollision(&box) && badge->eCollection == kCollectionType_Badge && 
            badge->eCategory != kBadgeType_Gladiator && ! pchButton )
        {
            fcolor = CLR_SELECTION_FOREGROUND;
            fbcolor = CLR_SELECTION_BACKGROUND;

            if(mouseDown(MS_LEFT))
            {
                badge_Select(i);
            }
        }

        if( badge->iIdx == playerPtr()->pl->titleBadge )
        {
            fcolor = CLR_SELECTION_FOREGROUND;
            fbcolor = CLR_SELECTION_BACKGROUND;
        }
        drawFlatFrame(PIX2, R10, x, y, z, wd, ht, sc, fcolor, fbcolor);
    }
    else if( g_DebugBadgeDisplayMode != 0 )
    {
        fbcolor = CLR_DARK_RED;
    }

    PERFINFO_AUTO_STOP_START("bottom", 1);

    // frame and icon
    tx -= last_wd*sc + R10*sc;
    drawFlatFrame(PIX2, R10, x, y, z, wd, ht, sc, fcolor, fbcolor);
    display_sprite( icon, ceil(tx + (last_wd - icon->width)*sc/2), ceil(y + (ht - icon->height*sc)/2), z, sc, sc, color );
    if( overlay )
        display_sprite( overlay, ceil(tx + (last_wd- overlay->width)*sc/2), ceil(y + (ht - overlay->height*sc)/2), z+1, sc, sc, CLR_WHITE );

    badgeDisplay->height = ht + BADGE_SPACE*sc;
    PERFINFO_AUTO_STOP();

    return ht + BADGE_SPACE*sc;
}

static int badgeSortByCompletion( const BadgeDef **b1, const BadgeDef **b2 )
{
    U32 * badgeFields = playerPtr()->pl->aiBadges;


    if(  BADGE_COMPLETION(badgeFields[(*b2)->iIdx]) == BADGE_COMPLETION(badgeFields[(*b1)->iIdx]) )
        return stricmp( (*b1)->pchName, (*b2)->pchName );
    else
        return  BADGE_COMPLETION(badgeFields[(*b2)->iIdx]) - BADGE_COMPLETION(badgeFields[(*b1)->iIdx]);
}

/**********************************************************************func*
 * badgesWindow
 *
 */

static const BadgeDef ** ppBadgeList = 0;

int badgesWindow(void)
{
    float x, y, z, wd, ht, sc, view_y, edge_off;
    int i, j, color, bcolor, iSize;
    static ScrollBar sb = { WDW_BADGES, 0};
    static float lastWide = -1.0f;
    CBox box;
    const BadgeDef ** badgeArray;
    U32 * badgeFields;
    int maxBadges;
    BadgeDisplay ** badgeDispArray;
    Entity *e = playerPtr();
    bool done = false;
    int badgeField;
    const BadgeDef *badge;
    U32 badgeTime;
    int cat_wd;

    if(!window_getDims(WDW_BADGES, &x, &y, &z, &wd, &ht, &sc, &color, &bcolor))
        return 0;

    if (window_getMode(WDW_BADGES) == WINDOW_GROWING)
    {
        if (e->pl->playerType == kPlayerType_Hero && entity_OwnsBadge(e, "P_Ascended"))
        {
            int phph = getPopHelpEvent("CODEPH_Badges_Switched_To_Villain");
            popHelpEventHappenedByTag("CODEPH_Badges_Switched_To_Hero");
            setPopHelpState(e, phph, PopHelpState_Dismissed);
            commSendPopHelp(phph, PopHelpState_Dismissed);
        }
        else if (e->pl->playerType == kPlayerType_Villain && entity_OwnsBadge(e, "P_Descended"))
        {
            int phph = getPopHelpEvent("CODEPH_Badges_Switched_To_Hero");
            popHelpEventHappenedByTag("CODEPH_Badges_Switched_To_Villain");
            setPopHelpState(e, phph, PopHelpState_Dismissed);
            commSendPopHelp(phph, PopHelpState_Dismissed);
        }
    }

    if(!badgeContextMenu)
    {
        badgeContextMenu = contextMenu_Create(NULL);
        contextMenu_addTitle(badgeContextMenu, "BadgeMenuTitle");
        contextMenu_addCode(badgeContextMenu, canBadgeBeAddedToMonitorWindow, 0, addToBadgeMonitorWindow, 0, "BadgeMonitorAdd", NULL);
    }

    initBadgeMonitorContextMenu();

    if ((!e->supergroup_id || !e->supergroup || !e->supergroup->badgeStates.eaiStates) && 
            gSelectedCollection == kCollectionType_Supergroup)
    {
        gSelectedCollection = kCollectionType_Badge;
    }

    if (gSelectedCollection == kCollectionType_Supergroup)
        gSupergroupBadges = true;
    else
        gSupergroupBadges = false;

    iSize     = gSupergroupBadges?eaSize(&g_SgroupBadges.ppBadges):eaSize(&g_BadgeDefs.ppBadges);
    edge_off = (R10+PIX3)*sc;
    view_y     = y + edge_off + TAB_HT;

    // alloc text wrapping structures
    if(!badgeDisplays)
    {
        int size;

        eaCreate(&badgeDisplays);
        eaCreate(&sgBadgeDisplays);

        size = eaSize(&g_BadgeDefs.ppBadges);
        while(eaSize(&badgeDisplays) < iSize)
        {
            BadgeDisplay *display = calloc(1, sizeof(BadgeDisplay));
            display->titleBlock = smfBlock_Create();
            display->textBlock = smfBlock_Create();
            eaPush(&badgeDisplays, display);
        }

        size = eaSize(&g_SgroupBadges.ppBadges);
        while(eaSize(&sgBadgeDisplays) < iSize)
        {
            BadgeDisplay *display = calloc(1, sizeof(BadgeDisplay));
            display->titleBlock = smfBlock_Create();
            display->textBlock = smfBlock_Create();
            eaPush(&sgBadgeDisplays, display);
        }
    }

    if (!badgeTypeSelectMenu)
    {
        CollectionCategory cc;
        int (*availableFunction) (void *) = NULL;

        badgeTypeSelectMenu = contextMenu_Create(NULL);
        contextMenu_addTitle(badgeTypeSelectMenu, "BadgeMenuTypeSelect");
        for (i = kCollectionType_Badge; i < kCollectionType_Count; i++)
        {    
            badgeTypeSelectSubMenus[i] = contextMenu_Create(NULL);
            cc.data.collection = i;
            availableFunction = alwaysAvailable;

            switch (i)
            {
                case kCollectionType_Badge:
                    cc.data.category = kBadgeType_MostRecent;
                    contextMenu_addCode(badgeTypeSelectSubMenus[i], alwaysAvailable, 0, setCategoryCallback, cc.voidPointer, badge_CategoryGetName(e, i, kBadgeType_MostRecent), NULL);
                    cc.data.category = kBadgeType_NearCompletion;
                    contextMenu_addCode(badgeTypeSelectSubMenus[i], alwaysAvailable, 0, setCategoryCallback, cc.voidPointer, badge_CategoryGetName(e, i, kBadgeType_NearCompletion), NULL);
                    for (j = kBadgeType_Tourism; j < kBadgeType_LastBadgeCategory; j++)
                    {
                        cc.data.category = j;
                        contextMenu_addCode(badgeTypeSelectSubMenus[i], alwaysAvailable, 0, setCategoryCallback, cc.voidPointer, badge_CategoryGetName(e, i, j), NULL);
                    }
                    break;
                case kCollectionType_Market:
                    cc.data.category = kMarketType_Content;
                    contextMenu_addCode(badgeTypeSelectSubMenus[i], alwaysAvailable, 0, setCategoryCallback, cc.voidPointer,
                                            badge_CategoryGetName(e, i, kMarketType_Content), NULL);
                    cc.data.category = kMarketType_SignatureStoryArc1;
                    contextMenu_addCode(badgeTypeSelectSubMenus[i], alwaysAvailable, 0, setCategoryCallback, cc.voidPointer,
                                            badge_CategoryGetName(e, i, kMarketType_SignatureStoryArc1), NULL);
                    cc.data.category = kMarketType_SignatureStoryArc2;
                    contextMenu_addCode(badgeTypeSelectSubMenus[i], alwaysAvailable, 0, setCategoryCallback, cc.voidPointer,
                                            badge_CategoryGetName(e, i, kMarketType_SignatureStoryArc2), NULL);
                    break;
                case kCollectionType_SuperPack:
                    cc.data.category = kSuperPackType_HeroesAndVillains;
                    contextMenu_addCode(badgeTypeSelectSubMenus[i], alwaysAvailable, 0, setCategoryCallback, cc.voidPointer,
                                            badge_CategoryGetName(e, i, kSuperPackType_HeroesAndVillains), NULL);
                    
                    //TODO: remove this if statement to not display this sub-menu until the release of Super Pack 2
                    if(AccountHasStoreProductOrIsPublished(inventoryClient_GetAcctInventorySet(), skuIdFromString("cosprovi")))
                    {
                        cc.data.category = kSuperPackType_RoguesAndVigilantes;
                        contextMenu_addCode(badgeTypeSelectSubMenus[i], alwaysAvailable, 0, setCategoryCallback, cc.voidPointer,
                                                badge_CategoryGetName(e, i, kSuperPackType_RoguesAndVigilantes), NULL);
                    }
                    break;
                case kCollectionType_Incarnate:
                    cc.data.category = kIncarnateType_Empyrean;
                    contextMenu_addCode(badgeTypeSelectSubMenus[i], alwaysAvailable, 0, setCategoryCallback, cc.voidPointer,
                                            badge_CategoryGetName(e, i, kIncarnateType_Empyrean), NULL);
                    cc.data.category = kIncarnateType_Astral;
                    contextMenu_addCode(badgeTypeSelectSubMenus[i], alwaysAvailable, 0, setCategoryCallback, cc.voidPointer,
                                            badge_CategoryGetName(e, i, kIncarnateType_Astral), NULL);
                    break;
                case kCollectionType_Supergroup:
                    for (j = kBadgeType_Tourism; j < kBadgeType_LastBadgeCategory; j++)
                    {
                        cc.data.category = j;
                        contextMenu_addCode(badgeTypeSelectSubMenus[i], alwaysAvailable, 0, setCategoryCallback, cc.voidPointer, badge_CategoryGetName(e, i, j), NULL);
                    }
                    availableFunction = superGroupVisible;
                    break;
            }

            contextMenu_addCode(badgeTypeSelectMenu, availableFunction, 0, NULL, (void *) i, badge_CollectionGetName(i), badgeTypeSelectSubMenus[i]);
        }
        gSelectedCollection = kCollectionType_Badge;
        gSelectedTab = kBadgeType_MostRecent;
        badgeReparse();
    }

    //force opacity on these menus
    contextMenu_SetCustomColors(badgeTypeSelectMenu, (winDefs[0].loc.color & NO_OPACITY)|0x66, (winDefs[0].loc.back_color & NO_OPACITY) | FORCE_OPACITY );
    for (i = kCollectionType_Badge; i < kCollectionType_Count; ++i)
    {
        if(badgeTypeSelectSubMenus[i])
        {
            contextMenu_SetCustomColors(badgeTypeSelectSubMenus[i], (winDefs[0].loc.color & NO_OPACITY)|0x66, (winDefs[0].loc.back_color & NO_OPACITY) | FORCE_OPACITY );
    
        }
    }

    // window frame
    drawFrame(PIX3, R10, x, y, z, wd, ht, sc, color, bcolor);

      if(window_getMode(WDW_BADGES) != WINDOW_DISPLAYING)
        return 0;

    cat_wd = 26*sc + str_wd(&game_12, sc, sc, "BadgeCategoryHeader");
    cat_wd = (wd - cat_wd);

    font(&game_12);
    font_color(CLR_WHITE, CLR_WHITE);
    prnt(x + (15.0f * sc), y + (18.0f * sc), z + 10.0f, sc, sc, "BadgeCategoryHeader");
 
    drawFrame(PIX2, R10, x + (80.0f * sc), y + 2.0f*sc, z + 1, cat_wd, 20.0f * sc, sc, color, bcolor);
    prnt(x + 92.0f * sc, y + 18.0f * sc, z + 10.0f, sc, sc, "%s \\ %s", 
            textStd(badge_CollectionGetName(gSelectedCollection)), textStd(badge_CategoryGetName(e, gSelectedCollection, gSelectedTab)));

    // check for mouse collision and mouseover
    BuildCBox( &box,  x + (80.0f * sc), y + 2.0f*sc, cat_wd, 20.0f * sc);

    if( mouseClickHit( &box, MS_LEFT ) )
    {
        contextMenu_set(badgeTypeSelectMenu, x + (80.0f * sc), y + 2.0f*sc, NULL);
    }

    // display window internals
    set_scissor(1);
    scissor_dims( x+PIX3*sc, y+(PIX3+TAB_HT)*sc, wd - PIX3*2*sc, ht - (PIX3*2+TAB_HT+20)*sc );
    BuildCBox(&box, x+PIX3*sc, y+(PIX3+TAB_HT)*sc, wd - PIX3*2*sc, ht - (PIX3*2+TAB_HT+20)*sc );

    // Most Recent tab is always personal badges currently
    if(gSupergroupBadges && gSelectedTab != kBadgeType_MostRecent)
    {                
        badgeArray = g_SgroupBadges.ppBadges;
        maxBadges = g_SgroupBadges.idxMax;
        badgeFields = e->supergroup->badgeStates.eaiStates;
        badgeDispArray = sgBadgeDisplays;
    }
    else
    {
        badgeArray = g_BadgeDefs.ppBadges;
        maxBadges = g_BadgeDefs.idxMax;
        badgeFields = e->pl->aiBadges;
        badgeDispArray = badgeDisplays;
    }

    i = 0;

    // the badge display will set the tooltip if the mouse is over a badge
    addToolTip( &s_badgeTip );
    clearToolTip( &s_badgeTip );

    if( gSelectedTab == kBadgeType_NearCompletion )
    {
        if( s_bReparse )
        {
            eaSetSizeConst(&ppBadgeList, 0);
            while( i < iSize && badgeArray[i]->iIdx <= maxBadges )
            {
                badgeField = badgeFields[badgeArray[i]->iIdx];
                if(!BADGE_IS_OWNED(badgeField) && BADGE_COMPLETION(badgeField)>0)
                    eaPushConst(&ppBadgeList, badgeArray[i]);
                i++;
            }
        }
        eaQSortConst(ppBadgeList, badgeSortByCompletion);
        
        for( i = 0; i < eaSize(&ppBadgeList); i++ )
        {
            int draw = s_bReparse || (lastWide != wd); //draw if width changed or need reparse
            // Draw if top above bottom of window and bottom is below top of window
            draw |= ((view_y-sb.offset < y+ht) && (view_y-sb.offset + badgeDispArray[i]->height > y));

            if( draw  )
            {
                view_y += badge_display( x+edge_off, view_y-sb.offset, z, sc, wd-2*edge_off, 
                    badgeDispArray[i], ppBadgeList[i], badgeFields[ppBadgeList[i]->iIdx], ppBadgeList[i]->iIdx, 0 );
            }
            else
                view_y += badgeDispArray[i]->height;
        }
    }
    else
    {
        while( ( gSelectedTab == kBadgeType_MostRecent && i < BADGE_ENT_RECENT_BADGES && e->pl->recentBadges[i].idx > 0 ) ||
            ( gSelectedTab != kBadgeType_MostRecent && i < iSize && badgeArray[i]->iIdx <= maxBadges ) )
        {
            int draw;

            if( gSelectedTab == kBadgeType_MostRecent )
            {
                badgeField = badgeFields[e->pl->recentBadges[i].idx];
                badge = badge_GetBadgeByIdx(e->pl->recentBadges[i].idx);
                badgeTime = e->pl->recentBadges[i].timeAwarded;

                if (!badge || (badge && (badge->eCategory == kBadgeType_Internal)))
                {
                    i++;
                    continue;
                }
            }
            else
            {
                badgeField = badgeFields[badgeArray[i]->iIdx];
                badge = badgeArray[i];
                badgeTime = 0;

                if( !badge || badge->eCategory != gSelectedTab || badge->eCollection != gSelectedCollection)
                {
                    i++;
                    continue;
                }
            }

            PERFINFO_AUTO_START("badge_display", 1);

            draw = s_bReparse || (lastWide != wd); //draw if width changed or need reparse
            // Draw if top above bottom of window and bottom is below top of window
            draw |= ((view_y-sb.offset < y+ht) && (view_y-sb.offset + badgeDispArray[i]->height > y));

            if( draw  )
            {
                view_y += badge_display( x+edge_off, view_y-sb.offset, z, sc, wd-2*edge_off, 
                                        badgeDispArray[i], badge, badgeField, badge->iIdx, badgeTime );
            }
            else
                view_y += badgeDispArray[i]->height;
                    
            PERFINFO_AUTO_STOP();
            i++;
        }
    }
    s_bReparse = false;
    lastWide = wd;

    set_scissor(0);

    //scrollbar
     doScrollBar( &sb, ht - edge_off*2 - (TAB_HT+20)*sc, view_y - y, wd, (PIX3+R10+TAB_HT)*sc, z+5, &box, 0 );

    if( D_MOUSEHIT == drawStdButton( x + wd/2, y + ht - 12, z, 120*sc, 16*sc, color, "ClearBadgeTitle", sc, gSelectedBadge ))
        badge_Select( -1 );

    return 0;
}

int countBadges(BadgeMonitorInfo *monitorList, int size)
{
    int count = 0;
    for (int i = 0; i < size; ++i)
    {
        if (monitorList[i].iIdx)
        {
            count++;
        }
    }
    return count;
}

F32 calculateBadgeMonitorWidth(Entity *player, BadgeMonitorInfo *monitorList, int size, float scale, float *outColWidth)
{
    float colWidth = 0;

    for (int i = 0; i < size; i++)
    {
        const BadgeDef* badgeDef = getBadgeDef(monitorList + i);
        if (badgeDef)
        {
            const char* title = printLocalizedEnt(badgeDef->pchDisplayTitle[ENT_IS_HERO(player) ? 0 : 1], player);
            int name_wd = str_wd(&game_12, scale, scale, title);
            colWidth = MAX(colWidth, name_wd + 10 * scale);
        }
    }
    if (outColWidth)
    {
        *outColWidth = colWidth;
    }

    float wd = 0;
    char descBuffer[BADGE_PROGRESS_STRING_BUFFER_SIZE];
    for (int i = 0; i < size; i++)
    {
        const BadgeDef* badgeDef = getBadgeDef(monitorList + i);
        if (badgeDef)
        {
            int progressStrWidth = str_wd(&game_12, scale, scale, "%s", badge_getProgressString(descBuffer, player, badgeDef));
            wd = MAX(wd, 5 * scale + colWidth + progressStrWidth);
        }
    }
    return wd;
}

void displayBadgeProgress(float x, float y, float width, float z, float scale, float colWidth, const Entity *entity, const BadgeDef* badgeDef, int badgeMonitorIdx)
{
    const char* title = printLocalizedEnt(badgeDef->pchDisplayTitle[ENT_IS_HERO(entity) ? 0 : 1], entity);

    font(&game_12);
    font_color(CLR_WHITE, CLR_WHITE);
    cprntEx(x + 5 * scale, y + LINE_HT * scale, z, scale, scale, 0, title);

    char buffer[BADGE_PROGRESS_STRING_BUFFER_SIZE];
    badge_getProgressString(buffer, entity, badgeDef);

    cprntEx(x + colWidth, y + LINE_HT * scale, z, scale, scale, NO_MSPRINT, "%s", buffer);

    CBox box;
    BuildCBox(&box, x, y, width, LINE_HT * scale);
    if (mouseClickHit(&box, MS_RIGHT))
    {
        contextMenu_displayEx(badgeMonitorContextMenu, (void*)badgeMonitorIdx);
    }
}

int badgeMonitorWindow(void)
{
    float x, y, z, width, height, scale;
    U32 color, bcolor;

    Entity *player = playerPtr();
    if(player && player->pl)
    {
        window_getDims(WDW_BADGEMONITOR, &x, &y, &z, &width, &height, &scale, &color, &bcolor);

        BadgeMonitorInfo *monitorList = player->pl->badgeMonitorInfo;

        // count current monitored badges
        int count = countBadges(monitorList, MAX_BADGE_MONITOR_ENTRIES);
        if (!count)
        {
            window_setMode(WDW_BADGEMONITOR, WINDOW_SHRINKING);
        }
        else
        {
            window_setMode(WDW_BADGEMONITOR, WINDOW_GROWING);

            float colWidth;
            float newWidth = calculateBadgeMonitorWidth(player, monitorList, MAX_BADGE_MONITOR_ENTRIES, scale, &colWidth);
            window_setDims(WDW_BADGEMONITOR, -1, -1, newWidth, (count * LINE_HT + 4) * scale);

            //
            CBox box;
            BuildCBox(&box, x, y, width, height);
            drawFrame(PIX2, R4, x, y, z, width, height, scale, color, bcolor);
            clipperPushCBox(&box);

            y += 2 * scale;

            for (int i = 0; i < count; i++)
            {
                const BadgeDef* badgeDef = getBadgeDef(monitorList + i);
                if (badgeDef)
                {
                    displayBadgeProgress(x, y + i * LINE_HT * scale, width, z, scale, colWidth, player, badgeDef, i);
                }
            }

            clipperPop();

            return 0;
        }
    }

    return 0;
}

/* End of File */
