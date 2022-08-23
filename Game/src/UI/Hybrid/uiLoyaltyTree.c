#include "UI/Hybrid/uiLoyaltyTree.h"
#include "gameComm/wdwbase.h"
#include "player/player.h"
#include "UI/uiWindows.h"
#include "UI/uiUtilGame.h"
#include "UI/uiUtil.h"
#include "UI/Hybrid/uiHybridMenu.h"
#include "UI/uiGame.h"
#include "graphics/textureatlas.h"
#include "UI/sprite/sprite_base.h"
#include "cmdparse/cmdcommon.h"
#include "account/AccountData.h"
#include <utilitieslib/components/earray.h>
#include "UI/sprite/sprite_font.h"
#include "UI/sprite/sprite_text.h"
#include <utilitieslib/utils/Cbox.h>
#include "UI/uiInput.h"
#include "clientcomm/dbclient.h"
#include "cmdparse/cmdgame.h"
#include "UI/uiToolTip.h"
#include "entity/entity.h"
#include "entity/entPlayer.h"
#include "UI/uiDialog.h"
#include <utilitieslib/language/MessageStoreUtil.h>
#include "player/inventory_client.h"
#include "formatter/smf_util.h"
#include "formatter/smf_main.h"
#include "UI/uiClipper.h"
#include "sound/sound.h"
#include <utilitieslib/language/AppLocale.h>
#include "UI/Hybrid/uiWebStoreFrame.h"

#define LT_MAIN_WIDTH 788.f
#define LT_MAIN_HEIGHT 694.f
#define LT_WIDTH (LT_MAIN_WIDTH*sc+HBPOPUPFRAME_LEFT_OFFSET+HBPOPUPFRAME_RIGHT_OFFSET)
#define LT_HEIGHT (LT_MAIN_HEIGHT*sc+HBPOPUPFRAME_TOP_OFFSET+HBPOPUPFRAME_BOTTOM_OFFSET)
#define LT_TOPTIER_WIDTH 475.f
#define LT_BOTTIER_WIDTH 650.f
#define LT_NODE_SPACING 60.f //(distance between nodes)
#define LT_TIER_HEIGHT 54.f  //(height of each tier)
#define LT_METER_X 767.f //(center of the meter - X)
#define LT_TOOLTIP_DELAY        12  // 400ms (default for microsoft)
#define LT_BASECOLOR_DARK 0x2e81b0ff
#define LT_BASECOLOR_LIGHT 0x29495bff
#define LT_FILLCOLOR_EARNED 0xe3d15cff
#define LT_FILLCOLOR_SPENT 0xcf3519ff

extern int gLoyaltyTreeVisible;

static TextAttribs ltTextAttr = 
{
    /* piBold        */  (int *)0,
    /* piItalic      */  (int *)0,
    /* piColor       */  (int *)CLR_WHITE,
    /* piColor2      */  (int *)0,
    /* piColorHover  */  (int *)CLR_WHITE,
    /* piColorSelect */  (int *)0,
    /* piColorSelectBG*/ (int *)CLR_WHITE,
    /* piScale       */  (int *)(int)(1.f*SMF_FONT_SCALE),
    /* piFace        */  (int *)&hybrid_12,
    /* piFont        */  (int *)0,
    /* piAnchor      */  (int *)0,
    /* piLink        */  (int *)CLR_H_HIGHLIGHT,
    /* piLinkBG      */  (int *)0,
    /* piLinkHover   */  (int *)CLR_WHITE,
    /* piLinkHoverBG */  (int *)0,
    /* piLinkSelect  */  (int *)0,
    /* piLinkSelectBG*/  (int *)CLR_WHITE,
    /* piOutline     */  (int *)0,
    /* piShadow      */  (int *)0,
};

static F32 spinAngle;
static int init;
static ToolTip ltToolTip;
static int tierFrameColor[] = { 0x9c7f3dff, 0xdbcba5ff, 0xe1c734ff };
static SMFBlock * smVIPdesc;

int loyaltyRewardBuy(char * nodeName)
{
    int retval = 0;
    Entity * e = playerPtr();
    LoyaltyRewardNode *node = accountLoyaltyRewardFindNode(nodeName);

    // pre-check to make sure this is a valid node to buy
    if (node && e && e->pl && accountLoyaltyRewardCanBuyNode(ent_GetProductInventory( e ), e->pl->account_inventory.accountStatusFlags, e->pl->loyaltyStatus, e->pl->loyaltyPointsUnspent, node))
    {
        cmdParsef("account_loyalty_client_buy %s", nodeName);
        retval = 1;
    }

    return retval;
}

static void setLTTooltip(CBox * box, const char * text)
{
    int currentMenu = current_menu();
    addToolTip( &ltToolTip );
    setToolTipEx( &ltToolTip, box, textStd(text), 0, currentMenu, (currentMenu == MENU_GAME) ? WDW_LOYALTY_TREE : 0, LT_TOOLTIP_DELAY, 0);
}

static void drawRewardMeter(F32 cx, F32 cy, F32 z, F32 ht, F32 sc, int rewardEarned, int rewardSpent)
{
    //meter
    AtlasTex * meterBase[] = 
    {
        atlasLoadTexture("lt_tiermeter_base_tip_0"),
        atlasLoadTexture("lt_tiermeter_base_mid_0"),
        atlasLoadTexture("lt_tiermeter_base_end_0")
    };
    AtlasTex * meterFrame[] = 
    {
        atlasLoadTexture("lt_tiermeter_tip_0"),
        atlasLoadTexture("lt_tiermeter_mid_0"),
        atlasLoadTexture("lt_tiermeter_end_0")
    };
    AtlasTex * meterNotchLarge = atlasLoadTexture("lt_tiermeter_notches_large_0");
    AtlasTex * meterNotchSmall = atlasLoadTexture("lt_tiermeter_notches_small_0");
    AtlasTex * meterArrow[] =
    {
        atlasLoadTexture("lt_tiermeter_indicator_0"),
        atlasLoadTexture("lt_tiermeter_indicator_1")
    };
    AtlasTex * tierStar[] = {atlasLoadTexture("lt_meterstar_0"), atlasLoadTexture("lt_meterstar_1")};

    int numLevels = eaSize(&g_accountLoyaltyRewardTree.levels);
    int curNotch, curLevel;
    F32 yMeterStart;
    F32 yMeterEnd;
    F32 yMeterWalk;
    F32 yMeterFillEarned;
    F32 yMeterFillSpent;
    F32 yLastLevel;
    int numNotches = g_accountLoyaltyRewardTree.levels[numLevels-1]->earnedPointsRequired;  //max
    F32 tierNotchSpacing = (ht-meterBase[0]->height*sc-meterBase[2]->height*sc)/numNotches;

    int isVIP = AccountIsVIP(inventoryClient_GetAcctInventorySet(), inventoryClient_GetAcctStatusFlags());

    yMeterWalk = yMeterStart = cy + numNotches*tierNotchSpacing/2.f;
    yMeterEnd = cy - numNotches*tierNotchSpacing/2.f;
    yMeterFillEarned = yMeterFillSpent = yMeterEnd;

    //draw top of the meter
    display_sprite(meterBase[0], cx-meterBase[0]->width*sc/2.f, yMeterEnd-meterBase[0]->height*sc, z, sc, sc, LT_BASECOLOR_DARK);
    display_sprite(meterFrame[0], cx-meterFrame[0]->width*sc/2.f, yMeterEnd-meterFrame[0]->height*sc, z+2.f, sc, sc, tierFrameColor[0]);
    //draw bottom of the meter
    display_sprite(meterBase[2], cx-meterBase[2]->width*sc/2.f, yMeterStart, z, sc, sc, LT_FILLCOLOR_SPENT);
    display_sprite(meterFrame[2], cx-meterFrame[2]->width*sc/2.f, yMeterStart, z+2.f, sc, sc, tierFrameColor[0]);
    yLastLevel = yMeterStart+meterBase[2]->height*sc;

    //draw large tier notch
    //yTierTop = yMeterEnd;
    curLevel = 0;
    for (curNotch = 0; curNotch <= numNotches; ++curNotch, yMeterWalk -= tierNotchSpacing)
    {
        LoyaltyRewardLevel * lrLevel = g_accountLoyaltyRewardTree.levels[curLevel];
        
        if (curNotch == rewardEarned)
        {
            yMeterFillEarned = yMeterWalk;
        }
        if (curNotch == rewardSpent)
        {
            yMeterFillSpent = yMeterWalk;
        }
        if (curNotch < lrLevel->earnedPointsRequired)
        {
            //draw small notch
            display_sprite(meterNotchSmall, cx+2.f*sc, yMeterWalk-meterNotchSmall->height*sc/2.f, z+1.f, sc, sc, tierFrameColor[0]);
        }
        else
        {
            CBox box;
            F32 xcbox = cx - meterBase[0]->width*sc/2.f;
            F32 ycbox = yMeterWalk;
            F32 wcbox = meterBase[0]->width*sc;
            F32 hcbox = yLastLevel-yMeterWalk;
            BuildCBox(&box, xcbox, ycbox, wcbox, hcbox);

            //draw large notch
            display_sprite(meterNotchLarge, cx+7.f*sc, yMeterWalk-meterNotchLarge->height*sc/2.f, z+3.f, sc, sc, tierFrameColor[0]);

            //tooltip + arrow
            if (mouseCollision(&box))
            {
                int lastSSD = is_scrn_scaling_disabled();
                display_sprite(meterArrow[0], cx-3.f*sc, yMeterWalk-meterArrow[0]->height*sc/2.f, z+3.f, sc, sc, tierFrameColor[0]);
                display_sprite(meterArrow[1], cx-3.f*sc, yMeterWalk-meterArrow[1]->height*sc/2.f, z+3.f, sc, sc, tierFrameColor[1]);

                set_scrn_scaling_disabled(0);
                reverseToScreenScalingFactorf(&xcbox, &ycbox);
                reverseToScreenScalingFactorf(&wcbox, &hcbox);
                set_scrn_scaling_disabled(lastSSD);
                BuildCBox(&box, xcbox, ycbox, wcbox, hcbox);
                setLTTooltip(&box, isVIP ? lrLevel->displayVIP : lrLevel->displayFree);
            }

            //draw tier star
            if (accountLoyaltyRewardHasThisLevel(rewardEarned, lrLevel))
            {
                display_sprite(tierStar[1], cx-30.f*sc, yMeterWalk-tierStar[1]->height*sc/2.f, z, sc, sc, CLR_H_GLOW);
            }
            display_sprite(tierStar[0], cx-30.f*sc, yMeterWalk-tierStar[0]->height*sc/2.f, z, sc, sc, CLR_WHITE);

            //increment
            yLastLevel = yMeterWalk;
            ++curLevel;
        }
    }

    //draw the rest of the meter
    display_sprite(meterBase[1], cx-meterBase[1]->width*sc/2.f, yMeterEnd, z, sc, (yMeterFillEarned-yMeterEnd)/meterBase[1]->height, LT_BASECOLOR_DARK);
    display_sprite(meterBase[1], cx-meterBase[1]->width*sc/2.f, yMeterFillEarned, z, sc, (yMeterFillSpent-yMeterFillEarned)/meterBase[1]->height, LT_FILLCOLOR_EARNED);
    display_sprite(meterBase[1], cx-meterBase[1]->width*sc/2.f, yMeterFillSpent, z, sc, (yMeterStart-yMeterFillSpent)/meterBase[1]->height, LT_FILLCOLOR_SPENT);
    display_sprite(meterFrame[1], cx-meterFrame[1]->width*sc/2.f, yMeterEnd, z+2.f, sc, (yMeterStart-yMeterEnd)/meterFrame[1]->height, tierFrameColor[0]);
}

void loyaltyTreeRender(F32 x, F32 y, F32 z, F32 sc)
{
    AtlasTex * topBackground = atlasLoadTexture("lt_cityscape_0");
    AtlasTex * topLogo = atlasLoadTexture("lt_logo_0");
    AtlasTex * pointBar[3][3] = 
    {
        { atlasLoadTexture("lt_pointsdisptab_tip_0"), atlasLoadTexture("lt_pointsdisptab_tip_1"), atlasLoadTexture("lt_pointsdisptab_tip_2") },
        { atlasLoadTexture("lt_pointsdisptab_mid_0"), atlasLoadTexture("lt_pointsdisptab_mid_1"), atlasLoadTexture("lt_pointsdisptab_mid_2") },
        { atlasLoadTexture("lt_pointsdisptab_end_0"), atlasLoadTexture("lt_pointsdisptab_end_1"), atlasLoadTexture("lt_pointsdisptab_end_2") }
    };
    AtlasTex * vipBanner;

    //tier frames
    AtlasTex * tierFrame[3][3] = 
    {
        { atlasLoadTexture("lt_tierstab_tip_0"), atlasLoadTexture("lt_tierstab_tip_1"), atlasLoadTexture("lt_tierstab_tip_2") },
        { atlasLoadTexture("lt_tierstab_mid1_0"), atlasLoadTexture("lt_tierstab_mid1_1"), atlasLoadTexture("lt_tierstab_mid1_2") },
        { atlasLoadTexture("lt_tierstab_mid2_0"), atlasLoadTexture("lt_tierstab_mid2_1"), atlasLoadTexture("lt_tierstab_mid2_2") }
    };
    AtlasTex * tierFrameEnd = atlasLoadTexture("lt_tierstab_end_0");

    //icons
    AtlasTex * recycle = atlasLoadTexture("icon_lt_recycleslot_0");
    AtlasTex * slotBase = atlasLoadTexture("loyatytree_slotbase");
    AtlasTex * slotBaseEmpty = atlasLoadTexture("loyatytree_slotbacking");
    AtlasTex * slotFill = atlasLoadTexture("icon_slotloyaltytree_0");
    AtlasTex * upArrow = atlasLoadTexture("icon_lt_diretonarrow");

    int colorGrayed = 0xffffff11;
    int colorActive = 0xffffff66;
    int colorHover = 0xffffffbb;
    F32 xwalk = x;
    F32 ywalk = y;
    int i, j;
    int numTiers;
    CBox box;
    int showNumToComplete = 1;
    int currentMenu = current_menu();
    static int lastHoveredNode = -1;
    int hovered = -1;
    F32 xcbox, ycbox, wcbox, hcbox;
    const int lastSSD = is_scrn_scaling_disabled();

    //point status
    U8 * loyaltyStatus = db_info.loyaltyStatus;
    int loyaltyPointsUnspent = db_info.loyaltyPointsUnspent;
    int loyaltyPointsEarned = db_info.loyaltyPointsEarned;
    int isVIP = AccountIsVIP(inventoryClient_GetAcctInventorySet(), inventoryClient_GetAcctStatusFlags());

    if (!smVIPdesc)
    {
        smVIPdesc = smfBlock_Create();
    }

    xwalk += HBPOPUPFRAME_LEFT_OFFSET;
    ywalk += HBPOPUPFRAME_TOP_OFFSET;

    //ingame menu refreshes loyalty data from the entity
    if (currentMenu == MENU_GAME)
    {
        Entity * e = playerPtr();

        if (e->pl)
        {
            loyaltyStatus = e->pl->loyaltyStatus;
            loyaltyPointsUnspent = e->pl->loyaltyPointsUnspent;
            loyaltyPointsEarned = e->pl->loyaltyPointsEarned;
        }
    }

    display_sprite(topBackground, xwalk, ywalk, z, sc, 1.f*sc, CLR_WHITE);
    display_sprite(topLogo, xwalk+LT_MAIN_WIDTH*sc/2.f - topLogo->width*sc/2.f, ywalk-4.f*sc, z+0.2f, 1.f*sc, 1.f*sc, CLR_WHITE);
    ywalk += (topBackground->height)*sc;

    //draw points bars
    font(&hybridbold_12);
    for (i = 0; i < 3; ++i)
    {
        F32 xwalk2 = xwalk;
        F32 barWd = (LT_MAIN_WIDTH)*sc/3.f;
        F32 midWd = barWd-pointBar[0][i]->width*sc-pointBar[2][i]->width*sc;
        for (j = 0; j < 3; ++j)
        {
            if (i==2)  // print text on last pass
            {
                switch (j)
                {
                case 0:
                    font_color(CLR_H_DESC_TEXT_DS, CLR_H_DESC_TEXT_DS);
                    prnt(xwalk2+barWd*0.15f+1.f*sc, ywalk+16.f*sc+1.f*sc, z+0.3f, 1.1f*sc, 1.1f*sc, "LoyaltyPointsEarnedText");
                    prnt(xwalk2+barWd*0.85f+1.f*sc, ywalk+17.f*sc+1.f*sc, z+0.3f, 1.3f*sc, 1.3f*sc, "%d", loyaltyPointsEarned);
                    font_color(CLR_WHITE, CLR_WHITE);
                    prnt(xwalk2+barWd*0.15f, ywalk+16.f*sc, z+0.3f, 1.1f*sc, 1.1f*sc, "LoyaltyPointsEarnedText");
                    font_color(CLR_GREEN, CLR_GREEN);
                    prnt(xwalk2+barWd*0.85f, ywalk+17.f*sc, z+0.3f, 1.3f*sc, 1.3f*sc, "%d", loyaltyPointsEarned);
                    break;
                case 1:
                    font_color(CLR_H_DESC_TEXT_DS, CLR_H_DESC_TEXT_DS);
                    prnt(xwalk2+barWd*0.15f+1.f*sc, ywalk+16.f*sc+1.f*sc, z+0.3f, 1.1f*sc, 1.1f*sc, "LoyaltyPointsSpentText");
                    prnt(xwalk2+barWd*0.85f+1.f*sc, ywalk+17.f*sc+1.f*sc, z+0.3f, 1.3f*sc, 1.3f*sc, "%d", loyaltyPointsEarned - loyaltyPointsUnspent);
                    font_color(CLR_WHITE, CLR_WHITE);
                    prnt(xwalk2+barWd*0.15f, ywalk+16.f*sc, z+0.3f, 1.1f*sc, 1.1f*sc, "LoyaltyPointsSpentText");
                    font_color(CLR_RED, CLR_RED);
                    prnt(xwalk2+barWd*0.85f, ywalk+17.f*sc, z+0.3f, 1.3f*sc, 1.3f*sc, "%d", loyaltyPointsEarned - loyaltyPointsUnspent);
                    break;
                case 2:
                    font_color(CLR_H_DESC_TEXT_DS, CLR_H_DESC_TEXT_DS);
                    prnt(xwalk2+barWd*0.10f+1.f*sc, ywalk+16.f*sc+1.f*sc, z+0.3f, 1.1f*sc, 1.1f*sc, "LoyaltyPointsRemainingText");
                    prnt(xwalk2+barWd*0.85f+1.f*sc, ywalk+17.f*sc+1.f*sc, z+0.3f, 1.3f*sc, 1.3f*sc, "%d", loyaltyPointsUnspent);
                    font_color(CLR_WHITE, CLR_WHITE);
                    prnt(xwalk2+barWd*0.10f, ywalk+16.f*sc, z+0.3f, 1.1f*sc, 1.1f*sc, "LoyaltyPointsRemainingText");
                    font_color(CLR_YELLOW, CLR_YELLOW);
                    prnt(xwalk2+barWd*0.85f, ywalk+17.f*sc, z+0.3f, 1.3f*sc, 1.3f*sc, "%d", loyaltyPointsUnspent);
                    break;
                default:
                    break;
                }
            }
            display_sprite(pointBar[0][i], xwalk2, ywalk, z+0.2f, sc, sc, HYBRID_FRAME_COLOR[i]);
            xwalk2 += pointBar[0][i]->width*sc;
            display_sprite(pointBar[1][i], xwalk2, ywalk, z+0.2f, midWd/pointBar[0][i]->width, sc, HYBRID_FRAME_COLOR[i]);
            xwalk2 += midWd;
            display_sprite(pointBar[2][i], xwalk2, ywalk, z+0.2f, sc, sc, HYBRID_FRAME_COLOR[i]);
            xwalk2 += pointBar[2][i]->width*sc;
        }
    }

    ywalk += pointBar[0][0]->height*sc-3.f*sc;

    //draw banner
    switch (getCurrentLocale())
    {
    case LOCALE_ID_GERMAN:
        vipBanner = atlasLoadTexture("lt_vipbanner_de_0");
        break;
    case LOCALE_ID_FRENCH:
        vipBanner = atlasLoadTexture("lt_vipbanner_fr_0");
        break;
    case LOCALE_ID_ENGLISH:
    default:
        vipBanner = atlasLoadTexture("lt_vipbanner_en_0");
        break;
    }
    //draw tooltip and link if player is not already a VIP
    if (!isVIP)
    {
        xcbox = xwalk;
        ycbox = ywalk;
        wcbox = vipBanner->width*sc;
        hcbox = vipBanner->height*sc;
        BuildCBox(&box, xcbox, ycbox, wcbox, hcbox);
        if (mouseCollision(&box))
        {
            //tooltip if account server is down
            AccountServerStatus status = inventoryClient_GetAcctAuthoritativeState();

            //since tooltips are handled elsewhere, we need to scale their collision to the screen
            set_scrn_scaling_disabled(0);
            reverseToScreenScalingFactorf(&xcbox, &ycbox);
            reverseToScreenScalingFactorf(&wcbox, &hcbox);
            set_scrn_scaling_disabled(lastSSD);
            BuildCBox(&box, xcbox, ycbox, wcbox, hcbox);

            switch (status)
            {
            case ACCOUNT_SERVER_UP:
                if (mouseClickHitNoCol(MS_LEFT))
                {
                    //webOpenUpgradeToVIP();
                }
                setLTTooltip(&box, "VIPBannerButton");
                break;
            case ACCOUNT_SERVER_SLOW:
                setLTTooltip(&box, "paragonstoreSlowButton");
                break;
            case ACCOUNT_SERVER_DOWN:
                setLTTooltip(&box, "paragonstoreDownButton");
                break;
            default:
                break;
            }
        }
    }
    display_sprite(vipBanner, xwalk, ywalk, z, 1.f*sc, 1.f*sc, CLR_WHITE);
    ywalk += vipBanner->height*sc;

    //draw tiers
    ywalk += 2.f*sc;  //gap adjustment

    //draw meter (y-center with availiable space)
    drawRewardMeter(xwalk+LT_METER_X*sc, ywalk+(y+LT_HEIGHT-HBPOPUPFRAME_BOTTOM_OFFSET-ywalk)/2.f, z, (y+LT_HEIGHT-HBPOPUPFRAME_BOTTOM_OFFSET-ywalk)-10.f*sc, sc, loyaltyPointsEarned, loyaltyPointsEarned - loyaltyPointsUnspent);

    //get total number of tiers (add 1 for VIP row)
    numTiers = eaSize(&g_accountLoyaltyRewardTree.tiers) + 1;  //add vip tier

    //for the rotational arrows
    spinAngle += TIMESTEP * -0.01f;
    if (spinAngle > 6.28318f)  //rollover
    {
        spinAngle = fmodf(spinAngle,6.28318f);
    }

    for (i = 0; i < numTiers; ++i)
    {
        F32 tabWd = 150.f*sc;
        F32 fadewidth = 75.f*sc;
        F32 xwalk2 = xwalk;
        F32 statwidth = 550.f*sc;
        F32 tierFadeWd = 100.f*sc;
        F32 tierWd = tabWd + tierFrame[1][0]->width*sc + statwidth + tierFrameEnd->width;
        F32 tierHt = LT_TIER_HEIGHT*sc;
        int numNodes, numNodesReal = 0;
        int tierNumber = numTiers - (i==0 ? 1 : i) - 1;   //reverse order for tiers (special case: top tier will have 2)
        LoyaltyRewardTier * lrTier = g_accountLoyaltyRewardTree.tiers[tierNumber];
        int nodesOwnedinTier = 0;
        int tierUnlocked = accountLoyaltyRewardUnlockedThisTier(loyaltyStatus, lrTier);
        int tierBackColor = i%2 ? LT_BASECOLOR_LIGHT : LT_BASECOLOR_DARK;
        int j;

        //tier background
        display_sprite(white_tex_atlas, xwalk2, ywalk, z, (tierWd-tierFadeWd)/white_tex_atlas->width, (tierHt-4.f*sc)/white_tex_atlas->height, tierBackColor);
        xwalk2 += (tierWd-tierFadeWd);
        display_sprite_blend(white_tex_atlas, xwalk2, ywalk, z, tierFadeWd/white_tex_atlas->width, (tierHt-4.f*sc)/white_tex_atlas->height, tierBackColor, tierBackColor&0xffffff00, tierBackColor&0xffffff00, tierBackColor);
        //highlight
        xwalk2 = xwalk;
        display_sprite_blend(white_tex_atlas, xwalk2, ywalk+(tierHt-4.f*sc), z, tierWd*0.2f/white_tex_atlas->width, 2.f*sc/white_tex_atlas->height, tierFrameColor[0]&0xffffff00, tierFrameColor[0], tierFrameColor[0], tierFrameColor[0]&0xffffff00);
        xwalk2 += tierWd*0.2f;
        display_sprite(white_tex_atlas, xwalk2, ywalk+(tierHt-4.f*sc), z, tierWd*0.6f/white_tex_atlas->width, 2.f*sc/white_tex_atlas->height, tierFrameColor[0]);
        xwalk2 += tierWd*0.6f;
        display_sprite_blend(white_tex_atlas, xwalk2, ywalk+(tierHt-4.f*sc), z, tierWd*0.2f/white_tex_atlas->width, 2.f*sc/white_tex_atlas->height, tierFrameColor[0], tierFrameColor[0]&0xffffff00, tierFrameColor[0]&0xffffff00, tierFrameColor[0]);

        //nodes
        numNodes = eaSize(&lrTier->nodeList);
        //need to count vip at top tier for centering purposes
        switch (i)
        {
        case 0:
            for (j = 0; j < numNodes; ++j)
            {
                if (lrTier->nodeList[j]->VIPOnly)
                {
                    ++numNodesReal;
                }
            }
            break;
        case 1:
            for (j = 0; j < numNodes; ++j)
            {
                if (!lrTier->nodeList[j]->VIPOnly)
                {
                    ++numNodesReal;
                }
            }
            break;
        default:
            numNodesReal = numNodes;
            break;
        }
        xwalk2 = xwalk + tabWd + tierFrame[1][0]->width*sc + statwidth/2.f - (numNodesReal-1)*LT_NODE_SPACING*sc/2.f;

        for (j = 0; j < numNodes; ++j)
        {
            LoyaltyRewardNode * lrNode = lrTier->nodeList[j];
            int nodeEnabled = (!lrNode->VIPOnly || isVIP);
            int baseColor = (tierUnlocked && nodeEnabled) ? colorActive : colorGrayed;
            int nodeOwned = 0;

            //skip non-vip for top tier
            if (i == 0 && !lrNode->VIPOnly)
            {
                continue;
            }
            //skip vip for 2nd tier
            else if (i == 1 && lrNode->VIPOnly)
            {
                continue;
            }

            //check if we own this node
            if (accountLoyaltyRewardHasThisNode(loyaltyStatus, lrNode))
            {
                ++nodesOwnedinTier;
                nodeOwned = 1;
            }

            xcbox = xwalk2 - slotBase->width*sc/2.f;
            ycbox = ywalk+6.f*sc;
            wcbox = slotBase->width*sc;
            hcbox = slotBase->height*sc;
            BuildCBox(&box, xcbox, ycbox, wcbox, hcbox);
            if (mouseCollision(&box))  
            {
                hovered = i*100+j;
                baseColor = colorHover;
                if (tierUnlocked && (!nodeOwned || lrNode->repeat) && loyaltyPointsUnspent && nodeEnabled && mouseClickHit(&box, MS_LEFT))  //locked tiers, owned non-repeatables, unaffordable
                {
                    AccountServerStatus status = inventoryClient_GetAcctAuthoritativeState();

                    if (status == ACCOUNT_SERVER_UP)
                    {
                        //buy reward
                        if (MENU_GAME == currentMenu)
                        {
                            dialogStdCB(DIALOG_ACCEPT_CANCEL, "ConfirmLoyaltyPointSpendingText", 0, 0, loyaltyRewardBuy, 0, 0, cpp_const_cast(void*)(lrNode->name) );
                        }
                        else
                        {
                            dialogStdCB(DIALOG_ACCEPT_CANCEL, "ConfirmLoyaltyPointSpendingText", 0, 0, dbLoyaltyRewardBuyItem, 0, 0, cpp_const_cast(void*)(lrNode->name) );
                        }
                    } else {
                        dialogStd(DIALOG_OK, "AccountServerUnavailable", NULL, NULL, NULL, NULL, 0);
                    }
                    sndPlay("N_SelectSmall", SOUND_GAME);
                }
                ////REMOVE: DEBUG ONLY
                //if (mouseClickHit(&box, MS_RIGHT))
                //{
                //    dbLoyaltyRewardRefundItem(lrNode->name);
                //}
                ////END REMOVE

                //since tooltips are handled elsewhere, we need to scale their collision to the screen
                set_scrn_scaling_disabled(0);
                reverseToScreenScalingFactorf(&xcbox, &ycbox);
                reverseToScreenScalingFactorf(&wcbox, &hcbox);
                set_scrn_scaling_disabled(lastSSD);
                BuildCBox(&box, xcbox, ycbox, wcbox, hcbox);
                setLTTooltip(&box, lrNode->displayDescription);
            }
            display_sprite(slotBase, xwalk2 - slotBase->width*sc/2.f, ywalk+6.f*sc, z+0.2f, 1.f*sc, 1.f*sc, baseColor);
            //no arrows below
            display_sprite(upArrow, xwalk2 - upArrow->width*sc/2.f, ywalk+44.f*sc, z+0.3f, 1.f*sc, 1.f*sc, CLR_H_HIGHLIGHT);
            if (nodeOwned)
            {
                //filled
                display_sprite(slotFill, xwalk2 - slotFill->width*sc/2.f, ywalk+6.f*sc, z+0.2f, 1.f*sc, 1.f*sc, CLR_WHITE);
            }
            else
            {
                display_sprite(slotBaseEmpty, xwalk2 - slotBaseEmpty->width*sc/2.f, ywalk+6.f*sc, z+0.1f, 1.f*sc, 1.f*sc, tierUnlocked && nodeEnabled ? 0x67ac16ff : CLR_WHITE);
            }
            if (lrNode->repeat)
            {
                //repeatable
                display_rotated_sprite(recycle, xwalk2 - recycle->width*sc/2.f, ywalk+5.f*sc, z+0.2f, 1.f*sc, 1.f*sc, 0xa9ff42ff, spinAngle, 0);
            }
            xwalk2 += LT_NODE_SPACING*sc;
        }

        //tier frame
        for (j = 0; j < 3; ++j)
        {
            xwalk2 = xwalk;
            display_sprite(tierFrame[0][j], xwalk2, ywalk, z, tabWd/tierFrame[0][j]->width, sc, tierFrameColor[j]);
            xwalk2 += tabWd;
            display_sprite(tierFrame[1][j], xwalk2, ywalk, z, sc, sc, tierFrameColor[j]);
            xwalk2 += tierFrame[1][j]->width*sc;
            display_sprite(tierFrame[2][j], xwalk2, ywalk, z, (statwidth-147.f)/tierFrame[2][j]->width, sc, tierFrameColor[j]);
            xwalk2 += statwidth-147.f;
            display_sprite_blend(tierFrame[2][j], xwalk2, ywalk, z, 147.f/tierFrame[2][j]->width, sc, tierFrameColor[j], tierFrameColor[j]&0xffffff55, tierFrameColor[j]&0xffffff55, tierFrameColor[j]);
            xwalk2 += 147.f;
        }
        display_sprite(tierFrameEnd, xwalk2, ywalk, z, sc, sc, tierFrameColor[0]);

        //tier text
        font(&title_12_no_outline);
        font_color(CLR_H_DESC_TEXT_DS, CLR_H_DESC_TEXT_DS);
        prnt(xwalk+11.f*sc, ywalk+16.f*sc, z, 1.f*sc, 1.f*sc, "%s%s", textStd(lrTier->displayName), i==0 ? " VIP" : "");
        font_color(CLR_WHITE, CLR_WHITE);
        prnt(xwalk+10.f*sc, ywalk+15.f*sc, z, 1.f*sc, 1.f*sc, "%s%s", textStd(lrTier->displayName), i==0 ? " VIP" : "");

        ywalk += tierHt;
    }

    if (lastHoveredNode != hovered)
    {
        lastHoveredNode = hovered;
        if (hovered > -1)
        {
            //new hovered node, play sound
            sndPlay("N_PopHover", SOUND_GAME);
        }
    }

    init = 1;
}

static void loyaltyTreeMenuPopup(F32 x, F32 y, F32 z, F32 sc)
{
    static HybridElement sButtonClose = {0, NULL, NULL, "icon_lt_closewindow_0"};
    set_scrn_scaling_disabled(1);

    //draw frame
    drawHybridPopupFrame(x, y, z, (LT_WIDTH), (LT_HEIGHT));

    //draw contents
    loyaltyTreeRender(x, y, z, sc);

    //draw close button
    if (D_MOUSEHIT == drawHybridButton(&sButtonClose, x + LT_WIDTH - 30.f, y + 21.f, z+20.f, 0.75f, CLR_WHITE, HB_DRAW_BACKING))
    {
        sndPlay("N_Deselect", SOUND_GAME);
        gLoyaltyTreeVisible = 0;
        if (smVIPdesc)
        {
            smfBlock_Destroy(smVIPdesc);
            smVIPdesc = NULL;
        }
        removeToolTip(&ltToolTip);
    }

    set_scrn_scaling_disabled(0);

    //block the rest of the input
    collisions_off_for_rest_of_frame = true;
}

void loyaltyTreeMenuTick(F32 sc)
{
    //put the loyalty tree in the middle of the screen
    F32 x = DEFAULT_SCRN_WD / 2.f;
    F32 y = DEFAULT_SCRN_HT / 2.f;
    applyToScreenScalingFactorf(&x, &y);

    loyaltyTreeMenuPopup(x-LT_WIDTH/2.f, y-LT_HEIGHT/2.f, 5001.f, sc);
}

int loyaltyTreeWindow()
{
    float x, y, z, wd, ht, sc;
    int color, bcolor;

    Wdw * wdw = wdwGetWindow(WDW_LOYALTY_TREE);
    Entity * e = playerPtr();
    static HybridElement sButtonClose = {0, NULL, NULL, "icon_lt_closewindow_0"};

    // Do everything common windows are supposed to do.
    if ( !window_getDims( WDW_LOYALTY_TREE, &x, &y, &z, &wd, &ht, &sc, &color, &bcolor ) || !e)
        return 0;
    if (window_getMode(WDW_LOYALTY_TREE) == WINDOW_DISPLAYING)
    {
        window_setDims(WDW_LOYALTY_TREE, x, y, LT_WIDTH, LT_HEIGHT);
    }

    //draw frame
    drawHybridPopupFrame(x, y, z, LT_WIDTH, LT_HEIGHT);

    //draw contents
    loyaltyTreeRender(x, y, z, sc);

    //draw close button
    if (D_MOUSEHIT == drawHybridButton(&sButtonClose, x + LT_WIDTH - 30.f, y + 21.f, z+20.f, 0.75f, CLR_WHITE, HB_DRAW_BACKING))
    {
        sndPlay("N_Deselect", SOUND_GAME);
        window_setMode(WDW_LOYALTY_TREE, WINDOW_SHRINKING);
        if (smVIPdesc)
        {
            smfBlock_Destroy(smVIPdesc);
            smVIPdesc = NULL;
        }
        removeToolTip(&ltToolTip);
    }

    return 0;
}