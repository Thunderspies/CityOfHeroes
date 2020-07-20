
#include <utilitieslib/stdtypes.h>
#include "UI/Hybrid/uiHybridMenu.h"
#include "UI/uiGame.h"                // for start_menu

#include "UI/sprite/sprite_text.h"
#include "UI/sprite/sprite_base.h"
#include "UI/sprite/sprite_font.h"

#include "win/input.h"
#include <utilitieslib/components/Earray.h>
#include "win/win_init.h" // for windowClientSize
#include "player/player.h"      // for playerPtr
#include "graphics/font.h"
#include "graphics/ttFontUtil.h"
#include "sound/sound.h"
#include "language/langClientUtil.h"
#include "graphics/textureatlas.h"
#include "cmdparse/cmdgame.h"  // for timestep
#include <utilitieslib/language/MessageStoreUtil.h>
#include <utilitieslib/utils/file.h>

#include "formatter/smf_format.h"
#include "formatter/smf_main.h"

#include "UI/uiInput.h"
#include "UI/uiUtilMenu.h"
#include "UI/uiUtil.h"
#include "UI/uiUtilGame.h"
#include "UI/uiClipper.h"
#include "UI/uiCostume.h"
#include "entity/entVarUpdate.h"

#include "clientcomm/dbclient.h"
#include "UI/Hybrid/uiPower.h"
#include "UI/uiPowerCust.h"
#include "UI/Hybrid/uiBody.h"
#include "UI/Hybrid/uiRegister.h"
#include "UI/Hybrid/uiOrigin.h"
#include "UI/Hybrid/uiArchetype.h"
#include "UI/Hybrid/uiPlaystyle.h"
#include "UI/Hybrid/uiRedirect.h"
#include <utilitieslib/language/AppLocale.h>
#include "filter/titles.h"
#include "entity/EntPlayer.h"
#include "UI/uiTailor.h"
#include "UI/uiComboBox.h"
#include "UI/uiToolTip.h"
#include "UI/uiScrollBar.h"
#include "entity/powers.h"
#include "LWC.h"
#include "player/inventory_client.h"
#include "formatter/smf_main.h"
#include "UI/uiBox.h"
#include "UI/Hybrid/uiWebStoreFrame.h"

//#define FORCE_LWC_UI  //comment this out to show LWC only if active

#define CHECK_NAME_TIMEOUT        4  // This is how long we wait for a response when checking if a name is available. (seconds)
#define CHECK_NAME_SPAM_LIMIT    1  // How often the user can spam the check name button. (seconds)
#define HB_TOOLTIP_DELAY        12  // 400ms (default for microsoft)

static int checkNameTouched = 0;
static ToolTip hybridToolTip;  //single tooltip (assume there won't be 2 displaying at the same time with standard hybrid menus)

TextAttribs gHybridTextAttr =
{
    /* piBold        */  (int *)0,
    /* piItalic      */  (int *)0,
    /* piColor       */  (int *)0xcfcfffff,
    /* piColor2      */  (int *)0,
    /* piColorHover  */  (int *)0xcfcfffff,
    /* piColorSelect */  (int *)0,
    /* piColorSelectBG*/ (int *)0xcfcfffff,
    /* piScale       */  (int *)(int)(1.f*SMF_FONT_SCALE),
    /* piFace        */  (int *)&hybridbold_14,
    /* piFont        */  (int *)0,
    /* piAnchor      */  (int *)0,
    /* piLink        */  (int *)CLR_H_HIGHLIGHT,
    /* piLinkBG      */  (int *)0,
    /* piLinkHover   */  (int *)CLR_WHITE,
    /* piLinkHoverBG */  (int *)0,
    /* piLinkSelect  */  (int *)0,
    /* piLinkSelectBG*/  (int *)0xcfcfffff,
    /* piOutline     */  (int *)0,
    /* piShadow      */  (int *)0,
};


typedef struct NavigationTab
{
    int menu;
    char * iconName0;
    char * iconName1;
    char * iconName2;
    char * text;

    RedirectCallback canUseFunc;
    void (* onEntryFunc)();
    void (* onExitFunc)();

    AtlasTex * icon0;
    AtlasTex * icon1;
    AtlasTex * icon2;
    F32 percent;
    // goto function
    // exit function

}NavigationTab;

static int alwaysTrue(int foo){ return 1; }
static HybridMenuType sMenuType = HYBRID_MENU_CREATION;
static HybridMenuType sLastNavMenuType = HYBRID_MENU_CREATION;
static int sIncomplete = 0;

NavigationTab navCreateSet[] = 
{    // MENU                // Icon0            // icon1            // icon2        // text                // canUse     // on entry/exit
    { MENU_ORIGIN,        "icon_orgin_1",        "icon_orgin_2",        NULL,            "OriginString",        alwaysTrue,   0, originMenuExitCode },
    { MENU_PLAYSTYLE,    "icon_playsty_1",    "icon_playsty_2",    NULL,            "PlayStyleString",    alwaysTrue,   0, 0 },
    { MENU_ARCHETYPE,    "icon_archetype_1",    "icon_archetype_2", NULL,            "ArchetypeString",    startingLocationReady,   0, archetypeMenuExitCode },
    { MENU_POWER,        "icon_power_2",        "icon_power_3",        NULL,            "PowersString",        powersReady,  powerMenuEnterCode, powerMenuExitCode },
    { MENU_BODY,        "icon_body_1",        "icon_body_2",        NULL,            "BodyString",        alwaysTrue,   bodyMenuEnterCode, bodyMenuExitCode },
    { MENU_COSTUME,        "icon_costume_1",    "icon_costume_2",    NULL,            "CostumeString",    alwaysTrue,   costumeMenuEnterCode, costumeMenuExitCode },
    { MENU_POWER_CUST,    "icon_custom_1",    "icon_custom_2",    NULL,            "CustomString",        powersChosen, powerCustMenuEnter, powerCustMenuExit },
    { MENU_REGISTER,    "icon_regist_1",    "icon_regist_2",    NULL,            "Register",            alwaysTrue,   registerMenuEnterCode, registerMenuExitCode },
};

NavigationTab navTailorSet[] =
{    // MENU                // Icon0            // icon1            // icon2        // text                // canUse     // on entry/exit
    { MENU_BODY,        "icon_body_1",        "icon_body_2",        NULL,            "BodyString",        bodyCanBeEdited,    bodyMenuEnterCode, bodyMenuExitCode },
    { MENU_TAILOR,        "icon_costume_1",    "icon_costume_2",    NULL,            "TailorString",        alwaysTrue,        tailorMenuEnterCode, 0 },
    { MENU_POWER_CUST,    "icon_custom_1",    "icon_custom_2",    NULL,            "PrimSecString",    alwaysTrue,        powerCustMenuEnter, powerCustMenuExit },
    { MENU_POWER_CUST_POWERPOOL, "icon_custom_1", "icon_custom_2", NULL,        "PowerPoolsString",    canCustomizePoolPowers,        powerCustMenuEnter, powerCustMenuExit },
    { MENU_POWER_CUST_INCARNATE, "icon_custom_1", "icon_custom_2", NULL,        "IncarnateString",    canCustomizeIncarnatePowers,        powerCustMenuEnter, powerCustMenuExit },
};

NavigationTab * navigationSet[2]        = {navCreateSet, navTailorSet};
int                navigationSetSize[2]    = {ARRAY_SIZE(navCreateSet), ARRAY_SIZE(navTailorSet)};

static int getNavTabIndexFromMenu( int menu )
{
    if (sMenuType < ARRAY_SIZE(navigationSetSize))
    {
        int i, size = navigationSetSize[sMenuType];
        for( i = 0; i < size; i++ )
        {
            if( menu == navigationSet[sMenuType][i].menu )
            {
                return i;
            }
        }
    }
    return -1;
}

void hybrid_start_menu( int menu )
{
    NavigationTab * tab;
    int idx = getNavTabIndexFromMenu(menu);

    if(idx < 0 )
        return;

    tab = &navigationSet[sMenuType][idx];
        
    if( !tab->canUseFunc(1) )
    {
        if (sMenuType == HYBRID_MENU_CREATION)
        {
            //redirect menus only for creation
            g_redirectedFrom = tab->menu;
            start_menu(MENU_REDIRECT);
            sndPlay("N_Error", SOUND_UI_ALTERNATE);
        }
    }
    else
    {
        if( tab->onEntryFunc )
            tab->onEntryFunc();
        start_menu(tab->menu);
    }
}

SMFBlock * checkNameText = NULL;

enum CheckNameResult
{
    CHECK_NAME_NONE = 0,
    CHECK_NAME_LOCKED = 1,
    CHECK_NAME_WAITING = 2,
    CHECK_NAME_REFRESH_WAITING = 3,
    CHECK_NAME_UNAVAILABLE = 4,
};

char checkNameName[32];
static int checkNameConfirmed = CHECK_NAME_NONE;
static U32 checkNameStartTime;
static U32 checkNameRefreshTime = 600; //seconds
static U32 checkNameAvailableTime = 0;
static int setTitle = 0;
static int lastTitle = -1;
static F32 spinAngle;
const int HYBRID_FRAME_COLOR[] = {CLR_H_DARK_BACK, CLR_H_BRIGHT_BACK, CLR_H_BACK};
const int HYBRID_SELECTED_COLOR[] = {0x711016ff, 0x9d585cff, 0xee5c35ff};

void resetHybridCommon()
{
    checkNameConfirmed = CHECK_NAME_NONE;
    checkNameTouched = 0;
    //clear name
    if (checkNameText)
    {
        smfBlock_Destroy(checkNameText);
        checkNameText = NULL;
    }
    sMenuType = HYBRID_MENU_CREATION;
    setTitle = 0;
    lastTitle = -1;
    sIncomplete = 0;
    //remove tooltip
    freeToolTip( &hybridToolTip );
}

void resetCreationMenus()
{
    //reset creation menus
    resetHybridCommon();
    resetOriginMenu();
    resetPlaystyleMenu();
    resetArchetypeMenu();
    resetPowerMenu();
    resetBodyMenu();
    resetCostumeMenu();
    resetPowerCustMenu();
    resetRegisterMenu();
}

F32 stateScaleSpeed( F32 sc, F32 speed, int up )
{
    if( up ) 
        sc += TIMESTEP*speed;
    else 
        sc -= TIMESTEP*speed;

    if( sc > 1.f )
        sc = 1.f;
    if( sc < 0.f )
        sc = 0.f;

    return sc;
}

F32 stateScale( F32 sc, int up )
{
    return stateScaleSpeed( sc, 0.5f, up );
}

int drawTopTab(NavigationTab * tab, F32 centerX, F32 centerY, F32 z )
{
    AtlasTex *shadow = atlasLoadTexture("toptab_-1");
    AtlasTex *back0 = atlasLoadTexture("toptab_0");
    AtlasTex *back1 = atlasLoadTexture("toptab_1");
    AtlasTex *back2 = atlasLoadTexture("toptab_2");
    AtlasTex *back3 = atlasLoadTexture("toptab_3");
    CBox box;
    F32 min_sc = 1.f;
    F32 max_sc = 1.2f;
    F32 target_sc;
    
    int ret = 0;
    int text_clr;
    int color_back[3];
    int locked = !tab->canUseFunc(0);
    int selected = isMenu(tab->menu);

    //special case for the first time through origin screen in character creation
    //force the player through the origin screen
    int creationGrayedTab = 0;
    if (sMenuType == HYBRID_MENU_CREATION && 
        ((tab->menu != MENU_ORIGIN && !(getSelectedOrigin(0) && startingLocationReady(0))) ||
        ((tab->menu == MENU_POWER_CUST || tab->menu == MENU_REGISTER) && hasStoreCostumeParts())))
    {
        locked = 1;
        creationGrayedTab = 1;
    }
    else if (sMenuType == HYBRID_MENU_TAILOR && isPowerCustomizationMenu(tab->menu) && tailor_hasStoreCostumeParts())
    {
        locked = 1;
    }

    if(!tab->icon0)
    {
        tab->icon0 = atlasLoadTexture(tab->iconName0);
        tab->icon1 = atlasLoadTexture(tab->iconName1);
        if( tab->iconName2 )
            tab->icon2 = atlasLoadTexture(tab->iconName2);
    }

    BuildCBox(&box, centerX - 122/2, centerY - back0->height/2, 122, back0->height );
    //drawBox(&box, 6, CLR_RED, 0);

    if( mouseClickHit(&box, MS_LEFT) && (!locked || (sMenuType == HYBRID_MENU_CREATION && !creationGrayedTab ) ))
    {
        int cur_idx = getNavTabIndexFromMenu(current_menu());
        // current_menu() might not return the index of a navigation set menut entry (for example could also be MENU_REDIRECT)
        // in which case getNavTabIndexFromMenu() returns -1 so make sure to check cur_idx before using it
        if (cur_idx >= 0 && navigationSet[sMenuType][cur_idx].onExitFunc != NULL)
        {
            navigationSet[sMenuType][cur_idx].onExitFunc();
        }

        hybrid_start_menu(tab->menu);

        sndPlay( "N_Select", SOUND_GAME );

        ret = 1;
    }

    if( mouseCollision(&box) || selected || redirectedFromMenu(tab->menu) ) // percent still effected when lock to allow text to appear
    {
        //play sound only if mousing over, not if we just got to this menu
        if (tab->percent == 0.f && !selected && !redirectedFromMenu(tab->menu))
        {
            sndPlay( "N_MouseEnter", SOUND_GAME );
        }
        tab->percent = stateScale(tab->percent, 1);
    }
    else
    {
        if (tab->percent == 1.f)
        {
            sndPlayNoInterrupt( "N_MouseExit", SOUND_GAME );
        }
        tab->percent = stateScale(tab->percent, 0);
    }

    if( locked && (sMenuType != HYBRID_MENU_CREATION || creationGrayedTab) )
    {
        target_sc = min_sc;
        color_back[0] = 0x222222ff;
        color_back[1] = 0;
        color_back[2] = 0;
        text_clr = 0xaaaaaa32;
    }
    else
    {
        int alpha = (int)(tab->percent*127+128);
        int alphafont = (int)(50.f + tab->percent*205.f);

        target_sc = min_sc + (max_sc - min_sc)*tab->percent;
        if( isDown(MS_LEFT) && mouseCollision(&box) )
        {
            target_sc *= .95f;
        }
        if (selected)
        {
            color_back[0] = HYBRID_SELECTED_COLOR[0]&(0xffffff00|alpha);
            color_back[1] = HYBRID_SELECTED_COLOR[1]&(0xffffff00|alpha);
            color_back[2] = HYBRID_SELECTED_COLOR[2]&(0xffffff00|alpha);
        }
        else
        {
            if (sIncomplete & (1<<tab->menu))
            {
                //flash red for incomplete
                color_back[0] = HYBRID_SELECTED_COLOR[0]&(0xffffff00 | alpha);
                color_back[1] = HYBRID_SELECTED_COLOR[1]&(0xffffff00 | (int)(alpha*(cosf(-spinAngle*30.f)+1.f)/2.f));
                color_back[2] = HYBRID_SELECTED_COLOR[2]&(0xffffff00 | (int)(alpha*(cosf(-spinAngle*30.f)+1.f)/2.f));
            }
            else
            {
                color_back[0] = HYBRID_FRAME_COLOR[0]&(0xffffff00|alpha);
                color_back[1] = HYBRID_FRAME_COLOR[1]&(0xffffff00|alpha);
                color_back[2] = HYBRID_FRAME_COLOR[2]&(0xffffff00|alpha);
            }
        }
        text_clr = 0xffffff00 | alphafont;
    }

    if( selected )
    {
        z +=5;
    }

    display_sprite(back0, centerX - back0->width*target_sc/2, centerY - back0->height*target_sc/2, z + target_sc, target_sc, target_sc, color_back[0]);
    display_sprite(back1, centerX - back1->width*target_sc/2, centerY - back1->height*target_sc/2, z + target_sc, target_sc, target_sc, CLR_H_HIGHLIGHT );
    display_sprite(back3, centerX - back3->width*target_sc/2, centerY - back3->height*target_sc/2, z + target_sc, target_sc, target_sc, color_back[1]);
    display_sprite(back2, centerX - back2->width*target_sc/2, centerY - back2->height*target_sc/2, z + target_sc, target_sc, target_sc, color_back[2]);

    setScalingModeAndOption(SPRITE_Y_UNIFORM_SCALE, SSO_SCALE_ALL);
    display_sprite_positional(tab->icon0, centerX, centerY - 5, z + target_sc, target_sc, target_sc, CLR_WHITE, H_ALIGN_CENTER, V_ALIGN_CENTER );
    //draw glow on current selection
    if (selected)
    {
        display_sprite_positional(tab->icon1, centerX, centerY - 5, z + target_sc, target_sc, target_sc, CLR_H_GLOW, H_ALIGN_CENTER, V_ALIGN_CENTER );
    }
    setScalingModeAndOption(SPRITE_XY_SCALE, SSO_SCALE_ALL);

    // text state
    font( &hybridbold_12 );
    font_color( text_clr, text_clr);
     prnt( centerX, centerY + back0->height/2 + 0.f, z, 1.f, 1.f, tab->text );

    return ret;
}



int drawNavigationSet()
{
    int ret = 0, i, size =  navigationSetSize[sMenuType];
    for( i = 0; i < size; i++ )
    {
        F32 xoffset = 0;
        ret |= drawTopTab(&navigationSet[sMenuType][i], i*124 + 164/2 + xoffset, 40, 5000.f );
    }

    return ret;
}

static void drawHybridButtonBacking(F32 cx, F32 cy, F32 z, F32 sc, int alpha, int gray);  //fwd declaration

#define HYBRID_NEXT_X_OFFSET 55.f
#define HYBRID_NEXT_Y_OFFSET 27.f

// check if this is the last valid menu of the sequence
int isHybridLastMenu(void)
{
    int cur_idx = getNavTabIndexFromMenu(current_menu());
    int dest_idx;

    for (dest_idx = cur_idx + 1; dest_idx < navigationSetSize[sMenuType]; ++dest_idx)
    {
        NavigationTab * tab = &navigationSet[sMenuType][dest_idx];
        if (tab->canUseFunc(0))
        {
            // found something, not the last menu
            return false;
        }
    }
    return true;
}

// page flip to go forward and back
int drawHybridNext( int dir, int locked, char * text )
{
    int cur_idx = getNavTabIndexFromMenu(current_menu()), dest_idx;

    F32 x;
    F32 y = 768.f-HYBRID_NEXT_Y_OFFSET;
    F32 z = 5000.f;
    int noNav = 0;
    int login;
    char * buttonText;
    int flags = 0;
    static HybridElement sButtonNext[2];

    //determine whether we should navigate using this arrow
    noNav = dir >= DIR_LEFT_NO_NAV;
    STATIC_INFUNC_ASSERT(DIR_COUNT == 6); // make sure noNav is computed correctly
    login = dir == DIR_RIGHT_LOGIN;
    dir %= 2;
    x = dir ? 1024.f-HYBRID_NEXT_X_OFFSET : HYBRID_NEXT_X_OFFSET;

    if (sMenuType == HYBRID_MENU_CREATION && dir //next
        && current_menu() == MENU_ORIGIN && !(getSelectedOrigin(0) && startingLocationReady(0)))
    {
        locked = 1;
    }
    flags |= locked ? HB_DISABLED : 0;
    flags |= dir ? HYBRID_BAR_BUTTON_ARROW_RIGHT : HYBRID_BAR_BUTTON_ARROW_LEFT;
    buttonText = dir ? "NextString" : "BackString";

    sButtonNext[dir].desc1 = text;
    sButtonNext[dir].text = buttonText;
    if (D_MOUSEHIT == drawHybridBarButton(&sButtonNext[dir], x, y, z, 150.f, 0.55f, 0, flags) ||
        (!login && dir && inpEdgeNoTextEntry(INP_RETURN) && cur_idx != navigationSetSize[sMenuType]-1))  //enter only moves fwd, but not at the last screen
    {
        if (sMenuType != HYBRID_MENU_NO_NAVIGATION && !noNav)
        {
            if( dir ) // pick the next tab we are going to.  Its ok if it ends up our of range, that means do nothing with tabs
                dest_idx = cur_idx+1;
            else
                dest_idx = cur_idx-1;

            // we may be able to skip some powercust tabs
            if (sMenuType == HYBRID_MENU_TAILOR)
            {
                while (dest_idx >= 0 && dest_idx < navigationSetSize[sMenuType] && !navigationSet[sMenuType][dest_idx].canUseFunc(0))
                {
                    if (dir)
                    {
                        ++dest_idx;
                    }
                    else
                    {
                        --dest_idx;
                    }
                }
            }

            if( navigationSet[sMenuType][cur_idx].onExitFunc )
                navigationSet[sMenuType][cur_idx].onExitFunc();

            if( dest_idx >= 0 && dest_idx < navigationSetSize[sMenuType] )
            {
                hybrid_start_menu(navigationSet[sMenuType][dest_idx].menu);
            }
        }
        sndPlay( dir ? "N_SelectTab" : "N_Deselect", SOUND_GAME );
        return 1;
    }
    
    return 0;
}

void goHybridNext(int dir)
{
    int cur_idx = getNavTabIndexFromMenu(current_menu()), dest_idx;

    devassert(dir==DIR_RIGHT || dir==DIR_LEFT);  //check that we aren't trying to use this function with no_nav, since it won't go anywhere.
    
    //normalize to left/right
    dir %= 2;

    if( dir ) // pick the next tab we are going to.  Its ok if it ends up our of range, that means do nothing with tabs
        dest_idx = cur_idx+1;
    else
        dest_idx = cur_idx-1;


    if( navigationSet[sMenuType][cur_idx].onExitFunc )
        navigationSet[sMenuType][cur_idx].onExitFunc();

    if( dest_idx >= 0 && dest_idx < navigationSetSize[sMenuType] )
    {
        hybrid_start_menu(navigationSet[sMenuType][dest_idx].menu);
    }
}

//timeout if request doesn't come back
//resend check name to re-lock after some time
void refreshCheckName()
{
    switch (checkNameConfirmed)
    {
    case CHECK_NAME_LOCKED:
        if ((timerSecondsSince2000() - checkNameStartTime) > checkNameRefreshTime)
        {
            checkNameConfirmed = CHECK_NAME_REFRESH_WAITING;
            dbCheckName(checkNameName, 0, 1);
        }
        break;
    case CHECK_NAME_WAITING:
        // Have we spent too long waiting for the name response?
        if( ( timerSecondsSince2000() - checkNameStartTime ) > CHECK_NAME_TIMEOUT )
        {
            checkNameConfirmed = CHECK_NAME_NONE;
        }
        break;
    case CHECK_NAME_REFRESH_WAITING:
        if( ( timerSecondsSince2000() - checkNameStartTime ) > CHECK_NAME_TIMEOUT )
        {
            //name has not been relocked, try again later
            checkNameConfirmed = CHECK_NAME_LOCKED;
            checkNameStartTime = timerSecondsSince2000();
        }
        break;
    default:
        break;
    }
}

void drawHybridTextEntry( SMFBlock * pBlock, F32 cx, F32 cy, F32 z, F32 wd, F32 ht  )
{
    AtlasTex * w = atlasLoadTexture("white");
    AtlasTex * end = atlasLoadTexture("checkname_circletip");
    AtlasTex * mid = atlasLoadTexture("checkname_circlemid");
    CBox box;
    int clr = 0x131320ff;
    F32 xposSc, yposSc, textXSc, textYSc;
    calculatePositionScales(&xposSc, &yposSc);
    calculateSpriteScales(&textXSc, &textYSc);

    display_sprite_positional_blend( w, cx, cy, z, (wd/2)/w->width, ht/w->height, CLR_H_HIGHLIGHT&0xffffff00, CLR_H_HIGHLIGHT, CLR_H_HIGHLIGHT, CLR_H_HIGHLIGHT&0xffffff00, H_ALIGN_RIGHT, V_ALIGN_CENTER );
    display_sprite_positional_blend( w, cx, cy, z, (wd/2)/w->width, ht/w->height, CLR_H_HIGHLIGHT, CLR_H_HIGHLIGHT&0xffffff00, CLR_H_HIGHLIGHT&0xffffff00, CLR_H_HIGHLIGHT, H_ALIGN_LEFT, V_ALIGN_CENTER );

    BuildScaledCBox(&box, cx-wd*xposSc/2, cy - ht*yposSc/2, 320.f*xposSc, ht*yposSc );
    if( mouseCollision(&box) )
    {
        clr = 0x232330ff;
    }

    // writing area
    display_sprite_positional( end, cx - wd*xposSc/2, cy, z, 1.f,  ht/end->height, clr, H_ALIGN_RIGHT, V_ALIGN_CENTER );
    display_sprite_positional( mid, cx, cy, z, wd/mid->width,  ht/end->height, clr, H_ALIGN_CENTER, V_ALIGN_CENTER );
    display_sprite_positional_flip( end, cx + wd*xposSc/2, cy, z, 1.f, ht/end->height, clr, FLIP_HORIZONTAL, H_ALIGN_LEFT, V_ALIGN_CENTER );

    // Text Input
    setSpriteScaleMode(SPRITE_XY_NO_SCALE);
    gHybridTextAttr.piScale = (int*)(int)(textYSc*SMF_FONT_SCALE);
    smf_Display(pBlock, cx - wd*xposSc/2, cy - 10.f*yposSc, z, wd, 0, 0, 0, &gHybridTextAttr, 0);
    gHybridTextAttr.piScale = (int*)(int)(1.f*SMF_FONT_SCALE);  //reset
    setSpriteScaleMode(SPRITE_Y_UNIFORM_SCALE);
}

static F32 updateTitleComboBox( F32 x, F32 y, F32 z )
{
    static ComboBox cb;
    Entity *e = playerPtr();
    int i, j, result = 0;

    //init
    if( !cb.strings )
    {
        for( i = 0; i < eaSize(&g_TitleDictionary.ppOrigins); ++i )
        {
            if( stricmp( g_TitleDictionary.ppOrigins[i]->pchOrigin, "Base" ) == 0 )
            {
                int tmp_wd = 0;
                //get the longest width so we know how wide to initialize this combo box
                for( j = 0; j < eaSize(&g_TitleDictionary.ppOrigins[i]->ppchChooseList); ++j )
                {
                    tmp_wd = MAX( tmp_wd, str_wd( &game_9, 1.f, 1.f, g_TitleDictionary.ppOrigins[i]->ppchChooseList[j] ) );
                }
                combobox_init( &cb, x, y, z, tmp_wd + (2*R10 + 2*PIX2 + 20.f), 32.f, ((j+1)*22.f), g_TitleDictionary.ppOrigins[i]->ppchChooseList, 0 );
            }
        }
    }
    else
    {
        cb.x = x;
        cb.y = y;
        cb.z = z;
    }

    //This will force a title to be selected.  (maybe if they typed "The something")
    if( setTitle )
    {
        cb.currChoice = setTitle;
        setTitle = 0;
    }

    //result is the selected title
    result = combobox_displayHybridRegister( &cb, R10, 0, CLR_H_TAB_BACK&0xffffff00, CLR_WHITE );

    //set the appropriate title if the selected title has changed
    if( lastTitle != result )
    {
        for( i = 0; i < eaSize(&g_TitleDictionary.ppOrigins); ++i )
        {
            if( stricmp( g_TitleDictionary.ppOrigins[i]->pchOrigin, "Base" ) == 0 )
            {
                e->name_gender = g_TitleDictionary.ppOrigins[i]->piGenders[result];
                e->pl->attachArticle = g_TitleDictionary.ppOrigins[i]->piAttach[result];
                strcpy(e->pl->titleTheText, g_TitleDictionary.ppOrigins[i]->ppchTitles[result]);
                break;
            }
        }
    }

    lastTitle = result;

    return cb.wd;
}

void drawCheckNameBar(HybridNameType nametype)
{
    AtlasTex * w = atlasLoadTexture("white");
    AtlasTex * end = atlasLoadTexture("checkname_circletip");
    AtlasTex * mid = atlasLoadTexture("checkname_circlemid");

    AtlasTex * icon0 = atlasLoadTexture("icon_namecheck_0");
    AtlasTex * icon1 = atlasLoadTexture("icon_namecheck_1");
    AtlasTex * icon2 = atlasLoadTexture("icon_namecheck_2");

    AtlasTex * iconFail = atlasLoadTexture("icon_namecheck_fail_0");
    AtlasTex * typingArrow = atlasLoadTexture("icon_typingarrow");

    AtlasTex * iconSearch[21];
    int iconSearchMax = ARRAY_SIZE(iconSearch);

    F32 cx = DEFAULT_SCRN_WD / 2.f;
    F32 cy = 718.f; // vertical height bar is centered on
    F32 z = 5000.f;
    F32 sc = 1.f, off = 0.f;
    int clr = 0x131320ff;
    CBox box;

    F32 nameWidth = 260.f;
    F32 iconOffset = nameWidth/2.f + 60.f;  //from cx to check name icon
    F32 textOffset = nameWidth/2.f;

    static F32 searchWaiting = 0.f;
    F32 searchSpeed = 1.f;
    static int waitingAnimDone = 1;
    Entity * e = playerPtr();
    F32 xposSc, yposSc, textXSc, textYSc;

    applyPointToScreenScalingFactorf(&cx, &cy); //use real pixel location
    calculatePositionScales(&xposSc, &yposSc);
    calculateSpriteScales(&textXSc, &textYSc);

    if (!checkNameText)
    {
        checkNameText = smfBlock_Create();    
        if (nametype == HYBRID_NAME_ID)
        {
            char fullName[128];
            int theLength = 0;

            if (e->pl)
            {
                theLength = strlen(e->pl->titleTheText);
            }

            //id card, set name directly
            Strcpy(fullName, e->pl->titleTheText);
            if (theLength > 0)
            {
                Strcat(fullName, " ");
            }
            if( e->pchar && e->pchar->iLevel >= COMMON_TITLE_LEVEL && e->pl->titleCommon[0] )
            {
                Strcat(fullName, e->pl->titleCommon);
                Strcat(fullName, " ");
            }
            if( e->pchar && e->pchar->iLevel >= ORIGIN_TITLE_LEVEL && e->pl->titleOrigin[0] )
            {
                Strcat(fullName, e->pl->titleOrigin);
                Strcat(fullName, " ");
            }
            Strcat(fullName, e->name);
            smf_SetRawText(checkNameText, fullName, 0);
            checkNameTouched = 1;
        }
    }

    smf_SetFlags(checkNameText, nametype == HYBRID_NAME_ID ? SMFEditMode_ReadOnly : SMFEditMode_ReadWrite, SMFLineBreakMode_SingleLine, SMFInputMode_PlayerNames, MAX_PLAYER_NAME_LEN(e), 
        SMFScrollMode_ExternalOnly, SMFOutputMode_StripAllTags, SMFDisplayMode_AllCharacters, SMFContextMenuMode_None, SMAlignment_Left, 0, "checkNameTextEdit", -1);

    iconSearch[0] = atlasLoadTexture("namecheck_f1");
    iconSearch[1] = atlasLoadTexture("namecheck_f2");
    iconSearch[2] = atlasLoadTexture("namecheck_f3");
    iconSearch[3] = atlasLoadTexture("namecheck_f4");
    iconSearch[4] = atlasLoadTexture("namecheck_f5");
    iconSearch[5] = atlasLoadTexture("namecheck_f6");
    iconSearch[6] = atlasLoadTexture("namecheck_f7");
    iconSearch[7] = atlasLoadTexture("namecheck_f8");
    iconSearch[8] = atlasLoadTexture("namecheck_f9");
    iconSearch[9] = atlasLoadTexture("namecheck_f10");
    iconSearch[10] = atlasLoadTexture("namecheck_f11");
    iconSearch[11] = atlasLoadTexture("namecheck_f12");
    iconSearch[12] = atlasLoadTexture("namecheck_f13");
    iconSearch[13] = atlasLoadTexture("namecheck_f14");
    iconSearch[14] = atlasLoadTexture("namecheck_f15");
    iconSearch[15] = atlasLoadTexture("namecheck_f16");
    iconSearch[16] = atlasLoadTexture("namecheck_f17");
    iconSearch[17] = atlasLoadTexture("namecheck_f18");
    iconSearch[18] = atlasLoadTexture("namecheck_f19");
    iconSearch[19] = atlasLoadTexture("namecheck_f20");
    iconSearch[20] = atlasLoadTexture("namecheck_f21");

    if (nametype != HYBRID_NAME_CREATION)
    {
        F32 xwalk;
        F32 ywalk = 75.f;
        F32 space = 25.f;
        int editColor = 0x42424200;
        int alpha = 0x99;
        F32 bcWidth = 550.f;
        F32 borderWd = 15.f;
        F32 xp = DEFAULT_SCRN_WD / 2.f;
        F32 yp = 0.f;
        applyPointToScreenScalingFactorf(&xp, &yp);
        xwalk = xp - 466.f*xposSc;
        ywalk *= yposSc;

        drawHybridDescFrame(xwalk, ywalk, z, bcWidth, 70.f, HB_DESCFRAME_FULLFRAME | HB_DESCFRAME_HOVERBACKGROUND, 0.f, 1.f, 0.5f, H_ALIGN_LEFT, V_ALIGN_TOP);

        setScalingModeAndOption(SPRITE_XY_NO_SCALE, SSO_SCALE_ALL);
        font(&title_12);
        font_outl(0);
        font_color( CLR_H_DESC_TEXT_DS, CLR_H_DESC_TEXT_DS );
        prnt( xwalk+15.f*xposSc+2.f, ywalk+24.f*yposSc+2.f, z, textXSc, textYSc, "RegisterCharacterName");
        font_color( CLR_WHITE, CLR_WHITE );
        prnt( xwalk+15.f*xposSc, ywalk+24.f*yposSc, z, textXSc, textYSc, "RegisterCharacterName");
        font_outl(1);
        setScalingModeAndOption(SPRITE_Y_UNIFORM_SCALE, SSO_SCALE_TEXTURE);

        cx = xwalk + bcWidth*xposSc/2.f;  //center of name
        cy = ywalk + 47.f*yposSc;                      //center of name
        textOffset = bcWidth/2.f - borderWd;  //radius of the name
        iconOffset = 330.f; 
        xwalk += borderWd*xposSc;

        if (nametype == HYBRID_NAME_REGISTER)
        {
            F32 xTheOffset = updateTitleComboBox( xwalk, ywalk+31.f*yposSc, z+5.f ); // the combo box

            cx += xTheOffset*xposSc/2.f;
            iconOffset -= xTheOffset;
            textOffset -= xTheOffset/2.f;

            BuildScaledCBox(&box, cx-textOffset*xposSc, cy - end->height*yposSc/2, textOffset*2.f*xposSc, end->height*yposSc );
            if( smfBlock_HasFocus(checkNameText) || mouseCollision(&box) )
            {
                alpha = 0xcc;
                if (mouseClickHit(&box, MS_LEFT))
                {
                    checkNameTouched = 1;
                }
            }
        }
    }
    else //nametype == HYBRID_NAME_CREATION
    {
        AtlasTex * botline = atlasLoadTexture("botline_0");
        static int lastMouseHovered = 0;
        BuildScaledCBox(&box, cx-textOffset*xposSc, cy - end->height*yposSc/2, textOffset*2.f*xposSc, end->height*yposSc );
        if( mouseCollision(&box) )
        {
            clr = 0x232330ff;
            if (mouseClickHit(&box, MS_LEFT))
            {
                sndPlay("N_SelectSmall", SOUND_GAME);
                checkNameTouched = 1;
            }
            if (!lastMouseHovered) //detect edge
            {
                sndPlay("N_PopHover", SOUND_GAME);
            }
            lastMouseHovered = 1;
        }
        else
        {
            lastMouseHovered = 0;
        }

        // yellow bar
        display_sprite_positional_blend(w, cx, cy, z, nameWidth/2.f/w->width, end->height/w->height, CLR_H_HIGHLIGHT&0xffffff00, CLR_H_HIGHLIGHT, CLR_H_HIGHLIGHT, CLR_H_HIGHLIGHT&0xffffff00, H_ALIGN_RIGHT, V_ALIGN_CENTER );
        display_sprite_positional_blend(w, cx, cy, z, nameWidth/2.f/w->width, end->height/w->height, CLR_H_HIGHLIGHT, CLR_H_HIGHLIGHT&0xffffff00, CLR_H_HIGHLIGHT&0xffffff00, CLR_H_HIGHLIGHT, H_ALIGN_LEFT, V_ALIGN_CENTER );

        // writing area
        display_sprite_positional(end, cx - nameWidth*xposSc/2.f, cy, z, 1.f, 1.f, clr, H_ALIGN_RIGHT, V_ALIGN_CENTER );
        display_sprite_positional(mid, cx, cy, z, nameWidth/mid->width, 1.f, clr, H_ALIGN_CENTER, V_ALIGN_CENTER );
        display_sprite_positional_flip(end, cx + nameWidth*xposSc/2.f, cy, z, 1.f, 1.f, clr, FLIP_HORIZONTAL, H_ALIGN_LEFT, V_ALIGN_CENTER );
    
        // bottom line
        display_sprite_positional(botline, cx, cy+5.f, z, 1.f, 1.f, CLR_WHITE, H_ALIGN_CENTER, V_ALIGN_TOP);
    }

    // Text Input
    setSpriteScaleMode(SPRITE_XY_NO_SCALE); // fonts only really work in SPRITE_XY_SCALE or SPRITE_XY_NO_SCALE
    gHybridTextAttr.piScale = (int*)(int)(textYSc*SMF_FONT_SCALE);
    smf_Display(checkNameText, cx - textOffset*xposSc, cy - 10*yposSc, z+1.f, textOffset*2.f*xposSc, 0, 0, 0, &gHybridTextAttr, 0);
    gHybridTextAttr.piScale = (int*)(int)(1.f*SMF_FONT_SCALE);  //reset
    if (!checkNameTouched)
    {
        font(&hybridbold_14);
        font_color(CLR_H_DESC_TEXT, CLR_H_DESC_TEXT);
        prnt(cx - (textOffset-typingArrow->width)*xposSc, cy+4.f*yposSc, z+1.f, textXSc, textYSc, "EnterNameHereText");
        setSpriteScaleMode(SPRITE_Y_UNIFORM_SCALE);
        display_sprite_positional(typingArrow, cx - textOffset*xposSc, cy - 10.f*yposSc, z+1.f, 1.f, 1.f, CLR_WHITE, H_ALIGN_LEFT, V_ALIGN_TOP);

        //flash red text in the register screen
        if (nametype == HYBRID_NAME_REGISTER)
        {
            int flashColor = CLR_RED&(0xffffff00 | (int)(0xff*(cosf(-spinAngle*15.f)+1.f)/2.f));
            setSpriteScaleMode(SPRITE_XY_NO_SCALE);
            font_color(flashColor, flashColor);
            prnt(cx - (textOffset-typingArrow->width)*xposSc, cy+4.f*yposSc, z+1.f, textXSc, textYSc, "EnterNameHereText");
            setSpriteScaleMode(SPRITE_Y_UNIFORM_SCALE);
            display_sprite_positional(typingArrow, cx - textOffset*xposSc, cy - 10.f*yposSc, z+1.f, 1.f, 1.f, flashColor, H_ALIGN_LEFT, V_ALIGN_TOP);
        }
    }
    setSpriteScaleMode(SPRITE_Y_UNIFORM_SCALE);

    if (nametype != HYBRID_NAME_ID)  // check name button only for creation/registration
    {
        int showTooltip = 0;
        int playSound = 0;
        // Strip titles and reset dropdown
        char * strippedName = getNameBarName();
        if (strippedName != checkNameText->outputString)
        {
            smf_SetRawText(checkNameText, strippedName, 0); 
            smf_SetCursor(checkNameText, 0);
        }

        // Check name button
        BuildScaledCBox(&box, cx + (iconOffset - icon0->width/2.f)*xposSc, cy - icon0->height*yposSc/2, icon0->width*xposSc, icon0->height*yposSc ); 
        if( mouseCollision(&box) )
        {
            U32 currentTime = timerSecondsSince2000();
            sc = 1.05f;
            if( isDown( MS_LEFT))
            {
                off = 2;
            }
            if (currentTime > checkNameAvailableTime)  //limit the amount we can click this button
            {
                if (mouseClickHit(&box, MS_LEFT))
                {
                    sndPlay("N_SelectSmall", SOUND_GAME);
                    checkNameConfirmed = CHECK_NAME_WAITING;
                    checkNameStartTime = currentTime;
                    Strcpy(checkNameName, getNameBarName());
                    dbCheckName(checkNameName, 0, 1);
                    checkNameAvailableTime = currentTime + CHECK_NAME_SPAM_LIMIT;
                }
            }
            showTooltip = 1;
        }

        //check if the name input has changed
        if (strcmp(checkNameName, getNameBarName()) != 0 && checkNameConfirmed != CHECK_NAME_NONE)
        {
            checkNameConfirmed = CHECK_NAME_NONE;
        }

        //search button
        if (checkNameConfirmed == CHECK_NAME_WAITING || !waitingAnimDone)
        {
            searchWaiting += TIMESTEP * searchSpeed;
            waitingAnimDone = (searchWaiting > iconSearchMax);  //finish animation
            searchWaiting = fmod(searchWaiting, iconSearchMax);
            sndPlay("N_CheckName_Loop", SOUND_UI_ALTERNATE);

            if (waitingAnimDone)
            {
                playSound = 1;
            }
            if (checkNameConfirmed == CHECK_NAME_WAITING || !waitingAnimDone)
            {
                int i;
                waitingAnimDone = 0;
                i = (int)searchWaiting;
                display_sprite_positional( iconSearch[i], cx + (iconOffset + off)*xposSc, cy + off*yposSc, z, sc, sc, CLR_WHITE, H_ALIGN_CENTER, V_ALIGN_CENTER );
                if (showTooltip)
                {
                    setHybridTooltip(&box, "CheckNameButtonWaiting");
                }
            }
        }
        if (waitingAnimDone)
        {
            //draw magnifying glass
            display_sprite_positional( icon0, cx + iconOffset*xposSc + 2.f, cy + 2.f, z, sc, sc, CLR_BLACK, H_ALIGN_CENTER, V_ALIGN_CENTER );
            display_sprite_positional( icon0, cx + (iconOffset + off)*xposSc, cy + off*yposSc, z, sc, sc, CLR_WHITE, H_ALIGN_CENTER, V_ALIGN_CENTER );
            display_sprite_positional( icon1, cx + (iconOffset + off)*xposSc, cy + off*yposSc, z, sc, sc, CLR_WHITE, H_ALIGN_CENTER, V_ALIGN_CENTER );
            switch(checkNameConfirmed)
            {
            case CHECK_NAME_LOCKED:
            case CHECK_NAME_REFRESH_WAITING:
                display_sprite_positional( icon2, cx + (iconOffset + off)*xposSc, cy + off*yposSc, z, sc, sc, CLR_GREEN, H_ALIGN_CENTER, V_ALIGN_CENTER ); //checkmark
                if (showTooltip)
                {
                    setHybridTooltip(&box, "CheckNameButtonLocked");
                }
                if (playSound)
                {
                    sndPlay("N_Success", SOUND_UI_ALTERNATE);
                }
                break;
            case CHECK_NAME_UNAVAILABLE:
                display_sprite_positional( iconFail, cx + (iconOffset + off)*xposSc, cy + off*yposSc, z, sc, sc, 0xF37961FF, H_ALIGN_CENTER, V_ALIGN_CENTER );
                if (showTooltip)
                {
                    setHybridTooltip(&box, "CheckNameButtonUnavailable");
                }
                if (playSound)
                {
                    sndPlay("N_Error", SOUND_UI_ALTERNATE);
                }
                break;
            case CHECK_NAME_WAITING:
            case CHECK_NAME_NONE:
            default:
                if (showTooltip)
                {
                    setHybridTooltip(&box, "CheckNameButtonIdle");
                }
                if (playSound)
                {
                    sndStop(SOUND_UI_ALTERNATE, 0.f);
                }
                break;
            }
        }
    }
}

void hybridCheckNameReceiveResult(int CheckNameSuccess, int locked)
{
    if (checkNameConfirmed == CHECK_NAME_WAITING)
    {
        if (!CheckNameSuccess)
        {
            locked = CHECK_NAME_UNAVAILABLE;
        }
    }

    devassert (CHECK_NAME_NONE == 0 && CHECK_NAME_LOCKED == 1);
    checkNameConfirmed = locked;
    checkNameStartTime = timerSecondsSince2000(); //update timestamp
}

//returns offset from using title
static int stripTitles(char * text)
{
    Entity * e = playerPtr();

    // "The" stripper
    if(getCurrentLocale() == LOCALE_ID_ENGLISH)    // only for english
    {
        int i;
        for(i = eaSize(&g_TitleDictionary.ppOrigins)-1; i >= 0 ; --i )
        {
            if( stricmp( "Base", g_TitleDictionary.ppOrigins[i]->pchOrigin ) == 0 )
            {
                int  j;
                for(j = eaSize(&g_TitleDictionary.ppOrigins[i]->ppchTitles)-1; j >= 0 ; --j)
                {
                    char * title = g_TitleDictionary.ppOrigins[i]->ppchTitles[j];
                    U32 titleLen = strlen(title);

                    if( strlen(text) >= titleLen + 1 && strnicmp(title, text, titleLen ) == 0 && text[titleLen] == ' ')
                    {
                        setTitle = j;
                        return titleLen + 1;                        
                    }
                }
            }
        }
    }
    return 0;
}

char * getNameBarName()
{
    if (checkNameText)
    {
        //Strip "the"
        int offset = stripTitles(checkNameText->outputString);

        return checkNameText->outputString + offset;
    }
    else
    {
        return 0;
    }
}

void hybridElementInit(HybridElement *hb)
{
    AtlasTex * w = atlasLoadTexture("white");

    if( hb->iconName0 )
    {
        hb->icon0 = atlasLoadTexture(hb->iconName0);
        if( hb->icon0 == w )
            hb->icon0 = NULL;
    }
    if( hb->iconName1 )
    {
        hb->icon1 = atlasLoadTexture(hb->iconName1);
        if( hb->icon1 == w )
            hb->icon1 = NULL;
    }
    if( hb->iconName2 )
    {
        hb->icon2 = atlasLoadTexture(hb->iconName2);
        if( hb->icon2 == w )
            hb->icon2 = NULL;
    }
    if( hb->texName0 )
    {
        hb->tex0 = atlasLoadTexture(hb->texName0); 
        if( hb->tex0 == w )
            hb->tex0 = NULL;
    }
    if( hb->texName1 )
    {
        hb->tex1 = atlasLoadTexture(hb->texName1); 
        if( hb->tex1 == w )
            hb->tex1 = NULL;
    }
    if( hb->texName2 )
    {
        hb->tex2 = atlasLoadTexture(hb->texName2); 
        if( hb->tex2 == w )
            hb->tex2 = NULL;
    }
}

int drawHybridBarLarge(HybridElement * hb, F32 cx, F32 cy, F32 z, F32 wd, int selected, int flags, F32 alpha )
{
    AtlasTex * tabLeft[3];
    AtlasTex * tabRight[3];
    AtlasTex * tabMid[3];
    AtlasTex * tabFrame = atlasLoadTexture("originselecttab_frame_mid_0");

    CBox box;
    F32 sc;
     int ret = D_NONE;
    int alphafront = 0xffffff00 | (int)(255 * alpha);
    int alphaback = 0xffffff00 | (int)(100 + 104.f*hb->percent*alpha);
    int foreColor = CLR_WHITE;
    int trimColor = CLR_H_HIGHLIGHT;
    int barColor[3];
    int i;
    F32 ht, midWd;
    int mouseCollide;
    F32 xposSc, yposSc;
    calculatePositionScales(&xposSc, &yposSc);

    tabLeft[0] = atlasLoadTexture("originselecttab_base_tip_0");
    tabLeft[1] = atlasLoadTexture("originselecttab_base_tip_1");
    tabLeft[2] = atlasLoadTexture("originselecttab_base_tip_2");
    tabRight[0] = atlasLoadTexture("originselecttab_base_end_0");
    tabRight[1] = atlasLoadTexture("originselecttab_base_end_1");
    tabRight[2] = atlasLoadTexture("originselecttab_base_end_2");
    tabMid[0] = atlasLoadTexture("originselecttab_base_mid_0");
    tabMid[1] = atlasLoadTexture("originselecttab_base_mid_1");
    tabMid[2] = atlasLoadTexture("originselecttab_base_mid_2");

    ht = tabMid[0]->height;
    midWd = wd-tabLeft[0]->width-tabRight[0]->width;

    if (flags & HB_UNSELECTABLE)
    {
        sc = 0.9f;
    }
    else
    {
        sc = .9f + .1f*hb->percent;
    }
    BuildScaledCBox(&box, cx - wd*xposSc*sc/2, cy - ht*yposSc*sc/2, wd*xposSc*sc, ht*yposSc*sc );
    mouseCollide = mouseCollision(&box);
    if (mouseCollide)
    {
        ret = D_MOUSEOVER;
        alphaback = 0xffffff00 | (int)(100 + 104.f*alpha);
    }

    if (!(flags & HB_UNSELECTABLE))
    {
        if( mouseClickHit(&box, MS_LEFT ))
        {
            sndPlay( "N_Select", SOUND_GAME );
            ret = D_MOUSEHIT;
        }
        if( mouseCollide && isDown(MS_LEFT) )
        {
            sc *= .98f;
        }
        if ( selected )
        {
            if (hb->percent == 0.f)
            {
                sndPlay("N_GenderZoom", SOUND_UI_ALTERNATE);
            }
            hb->percent = stateScale(hb->percent,1);
        }
        else
        {
            hb->percent = stateScale(hb->percent,0);
        }
    }

    if (flags & HB_GREY)
    {
        barColor[0] = 0x222222ff;
        barColor[1] = 0x666666ff;
        barColor[2] = CLR_H_GREY;
        foreColor = CLR_H_GREY;
        trimColor = CLR_H_GREY;
    }
    else if (selected)
    {
        barColor[0] = HYBRID_SELECTED_COLOR[0];
        barColor[1] = HYBRID_SELECTED_COLOR[1];
        barColor[2] = HYBRID_SELECTED_COLOR[2];
    }
    else
    {
        barColor[0] = HYBRID_FRAME_COLOR[0];
        barColor[1] = HYBRID_FRAME_COLOR[1];
        barColor[2] = HYBRID_FRAME_COLOR[2];
    }

    for (i = 0; i < 3; ++i)
    {
        F32 xwalk = cx - wd*xposSc*sc/2.f;
        F32 y = cy - ht*yposSc*sc/2.f;

        display_sprite_positional(tabLeft[i], xwalk, y, z, sc, sc, barColor[i]&alphaback, H_ALIGN_LEFT, V_ALIGN_TOP );
        xwalk += tabLeft[i]->width*xposSc*sc;
        display_sprite_positional(tabMid[i], xwalk, y, z, midWd*sc/tabMid[i]->width, sc, barColor[i]&alphaback, H_ALIGN_LEFT, V_ALIGN_TOP );
        xwalk += midWd*xposSc*sc;
        display_sprite_positional(tabRight[i], xwalk, y, z, sc, sc, barColor[i]&alphaback, H_ALIGN_LEFT, V_ALIGN_TOP );
    }
    display_sprite_positional_blend(tabFrame, cx, cy, z, midWd*sc/tabFrame->width/2.f, sc, trimColor&0xffffff00, trimColor&alphafront, trimColor&alphafront, trimColor&0xffffff00, H_ALIGN_RIGHT, V_ALIGN_CENTER );
    display_sprite_positional_blend(tabFrame, cx, cy, z, midWd*sc/tabFrame->width/2.f, sc, trimColor&alphafront, trimColor&0xffffff00, trimColor&0xffffff00, trimColor&alphafront, H_ALIGN_LEFT, V_ALIGN_CENTER );

    if(selected && hb->icon1)
    {
        //glow
        display_sprite_positional(hb->icon1, cx, cy, z, sc, sc, CLR_H_GLOW&alphafront, H_ALIGN_CENTER, V_ALIGN_CENTER );
    }
    if(hb->icon0)
    {
        display_sprite_positional(hb->icon0, cx, cy, z, sc, sc, foreColor&alphafront, H_ALIGN_CENTER, V_ALIGN_CENTER );
    }

    return ret;
}



int drawHybridBar( HybridElement * hb, int selected, F32 x, F32 cy, F32 z, F32 wd, F32 scale, int flags, F32 alpha, HAlign ha, VAlign va )
{
    CBox box;
    F32 sc;
    F32 alphaScale;
    int alphafront, alphaback, colorfront, alphafont, alphafontDS, colorback[3], ret = D_NONE;
    int fontColor;
    F32 end_wd = 40.f;
    F32 cx = x;
    F32 spr_sc, xposSc, yposSc, textXSc, textYSc;
    int i;
    int mouseCollide;
    SpriteScalingMode ssm = getSpriteScaleMode();

     AtlasTex *fmid, *bl[3], *br[3], *bmid[3];

    if (flags & HB_ROUND_ENDS_THIN)
    {
        bmid[0] = atlasLoadTexture("selectiontabround_small_mid_0");
        bmid[1] = atlasLoadTexture("selectiontabround_small_mid_1");
        bmid[2] = atlasLoadTexture("selectiontabround_small_mid_2");
    }
    else
    {
        bmid[0] = atlasLoadTexture("selectiontabsharp_mid");
        bmid[1] = atlasLoadTexture("selectiontabsharp_mid_2");
        bmid[2] = atlasLoadTexture("selectiontabsharp_mid_3");
    }
    if( scale <= 0.f)
        return ret;

    calculatePositionScales(&xposSc, &yposSc);
    switch (ha)
    {
    case H_ALIGN_CENTER:
        x -= wd*xposSc/2.f;
        break;
    case H_ALIGN_RIGHT:
        x -= wd*xposSc;
        break;
    case H_ALIGN_LEFT:
    default:
        //x already in the correct place
        break;
    }
    switch (va)
    {
    case V_ALIGN_TOP:
        cy += bmid[0]->height*yposSc*scale/2.f;
        break;
    case V_ALIGN_BOTTOM:
        cy -= bmid[0]->height*yposSc*scale/2.f;
        break;
    case V_ALIGN_CENTER:
    default:
        //y already in the correct place
        break;
    }

      if( flags&HB_FADE_ENDS || flags&HB_ROUND_ENDS || flags&HB_ROUND_ENDS_THIN )
        x = cx - wd*xposSc/2;

    if (flags&HB_TAIL_LEFT_ROUND)
    {
        BuildScaledCBox(&box, x-wd*xposSc, cy - (bmid[0]->height/2 - 4)*yposSc*scale, wd*xposSc, (bmid[0]->height-8)*yposSc*scale );
    }
    else
    {
        BuildScaledCBox(&box, x, cy - (bmid[0]->height/2 - 4)*yposSc*scale, wd*xposSc, (bmid[0]->height-8)*yposSc*scale );
    }
    mouseCollide = mouseCollision(&box);
    if (mouseCollide)
    {
        ret = D_MOUSEOVER;
        if (!(flags&HB_UNSELECTABLE))
        {
            if( mouseClickHitNoCol( MS_LEFT ) )
            {
                sndPlay( "N_Select", SOUND_GAME );
                ret = D_MOUSEHIT;
            }
            if (mouseDoubleClickHit(&box, MS_LEFT))
            {
                sndPlay( "N_Select", SOUND_GAME );
                ret = D_MOUSEDOUBLEHIT;
            }
        }
    }

    if( (selected || mouseCollide) )
    {
        if (flags & HB_NO_HOVER_SCALING)
        {
            hb->percent = 1.f;
        }
        else
        {
            if (!selected && hb->percent == 0.f)
            {
                sndPlay( "N_MouseEnter", SOUND_GAME );
            }
            hb->percent = stateScale(hb->percent,1);
        }
    }
    else
    {
        if (flags & HB_NO_HOVER_SCALING)
        {
            hb->percent = 0.f;
        }
        else
        {
            if (hb->percent == 1.f)
            {
                sndPlayNoInterrupt( "N_MouseExit", SOUND_GAME );
            }
            hb->percent = stateScale(hb->percent,0);
        }
    }

    sc = .9f + .1f*((flags&HB_UNSELECTABLE) ? 0 : hb->percent);
    if( mouseCollision(&box) && isDown(MS_LEFT) && !(flags&HB_UNSELECTABLE) )
        sc *= .98f;
    
    spr_sc = scale * sc;
    if (flags & HB_ALWAYS_FULL_ALPHA)
    {
        alphaScale = 1.f;
    }
    else
    {
        alphaScale = hb->percent;
    }

    alphafront = 0xffffff00 | (int)(201 * alpha);
    alphaback = 0xffffff00 | (int)((100 + 104.f*((flags&HB_UNSELECTABLE) ? 0 : alphaScale)) * alpha);
    alphafont = 0xffffff00 | (int)((100 + 155.f*((flags&HB_UNSELECTABLE) ? 0 : alphaScale)) * alpha);
    alphafontDS= 0xffffff00 | (int)((50 + 95.f*((flags&HB_UNSELECTABLE) ? 0 : alphaScale)) * alpha);

    colorfront = CLR_H_HIGHLIGHT&alphafront;
    if( flags&HB_GREY )
    {
        colorback[0] = CLR_H_GREY & alphaback;
        colorback[1] = 0;
        colorback[2] = 0;
    }
    else if (flags & HB_FLAT)
    {
        colorback[0] = CLR_H_BACK & alphaback;
        colorback[1] = 0;
        colorback[2] = 0;
    }
    else if (selected)
    {
        colorback[0] = HYBRID_SELECTED_COLOR[0] & alphaback;
        colorback[1] = HYBRID_SELECTED_COLOR[1] & alphaback;
        colorback[2] = HYBRID_SELECTED_COLOR[2] & alphaback;
    }
    else
    {
        colorback[0] = HYBRID_FRAME_COLOR[0] & alphaback;
        colorback[1] = HYBRID_FRAME_COLOR[1] & alphaback;
        colorback[2] = HYBRID_FRAME_COLOR[2] & alphaback;
    }

    // 
    if( flags&HB_TAIL_RIGHT)
    {
        fmid = atlasLoadTexture("selectiontabsharp_frame_mid");
        bl[0] = atlasLoadTexture("selectiontabsharp_end");
        bl[1] = atlasLoadTexture("selectiontabsharp_end_2");
        bl[2] = atlasLoadTexture("selectiontabsharp_end_3");

        for (i = 0; i < 3; ++i)
        {
             display_sprite_positional_blend( bmid[i], x, cy, z, end_wd*spr_sc/bmid[i]->width, spr_sc, colorback[i]&0xffffff00, colorback[i], colorback[i], colorback[i]&0xffffff00, H_ALIGN_LEFT, V_ALIGN_CENTER );
             display_sprite_positional( bmid[i], x + end_wd*xposSc*spr_sc, cy, z, (wd*sc-bl[i]->width*spr_sc-end_wd*spr_sc)/fmid->width, spr_sc, colorback[i], H_ALIGN_LEFT, V_ALIGN_CENTER );
            display_sprite_positional( bl[i], x + wd*xposSc*sc, cy, z, spr_sc, spr_sc, colorback[i], H_ALIGN_RIGHT, V_ALIGN_CENTER );
        }

        display_sprite_positional_blend( fmid, x, cy, z, (wd*sc/2)/fmid->width, spr_sc, colorfront&0xffffff00, colorfront, colorfront, colorfront&0xffffff00, H_ALIGN_LEFT, V_ALIGN_CENTER );
        display_sprite_positional_blend( fmid, x + wd*xposSc*sc/2.f, cy, z, (wd*sc/2 - bl[0]->width*spr_sc)/fmid->width, spr_sc, colorfront, colorfront&0xffffff00, colorfront&0xffffff00, colorfront, H_ALIGN_LEFT, V_ALIGN_CENTER );
    }
    else if(flags&HB_TAIL_LEFT_ROUND)
    {
        F32 midWd;
         fmid = atlasLoadTexture("selectiontabsharp_frame_mid");
        br[0] = atlasLoadTexture("selectiontabsharp_end");
        br[1] = atlasLoadTexture("selectiontabsharp_end_2");
        br[2] = atlasLoadTexture("selectiontabsharp_end_3");
        bl[0] = atlasLoadTexture("selectiontabsharp_tip");
        bl[1] = atlasLoadTexture("selectiontabsharp_tip_2");
        bl[2] = atlasLoadTexture("selectiontabsharp_tip_3");
        midWd = ((wd*sc)- (bl[0]->width+br[0]->width)*spr_sc);

        for (i = 0; i < 3; ++i)
        {
            display_sprite_positional( bl[i], x - wd*xposSc*sc, cy, z, spr_sc, spr_sc, colorback[i], H_ALIGN_LEFT, V_ALIGN_CENTER );
            display_sprite_positional( bmid[i], x - br[i]->width*xposSc*spr_sc, cy, z, midWd/fmid->width, spr_sc, colorback[i], H_ALIGN_RIGHT, V_ALIGN_CENTER );
            display_sprite_positional( br[i], x, cy, z, spr_sc, spr_sc, colorback[i], H_ALIGN_RIGHT, V_ALIGN_CENTER );
        }

        display_sprite_positional_blend( fmid, x - (br[0]->width*spr_sc + (midWd/2)*sc)*xposSc, cy, z, (midWd*sc/2)/fmid->width, spr_sc, colorfront&0xffffff00, colorfront, colorfront, colorfront&0xffffff00, H_ALIGN_RIGHT, V_ALIGN_CENTER );
        display_sprite_positional_blend( fmid, x - (br[0]->width*spr_sc + (midWd/2)*sc)*xposSc, cy, z, (wd*sc/2 - bl[0]->width*spr_sc)/fmid->width, spr_sc, colorfront, colorfront&0xffffff00, colorfront&0xffffff00, colorfront, H_ALIGN_LEFT, V_ALIGN_CENTER );
    }
    else if( flags&HB_TAIL_RIGHT_ROUND)
    {
        F32 midWd;
         fmid = atlasLoadTexture("selectiontabsharp_frame_mid");
        br[0] = atlasLoadTexture("selectiontabsharp_end");
        br[1] = atlasLoadTexture("selectiontabsharp_end_2");
        br[2] = atlasLoadTexture("selectiontabsharp_end_3");
        bl[0] = atlasLoadTexture("selectiontabsharp_tip");
        bl[1] = atlasLoadTexture("selectiontabsharp_tip_2");
        bl[2] = atlasLoadTexture("selectiontabsharp_tip_3");
        midWd = ((wd*sc)- (bl[0]->width+br[0]->width)*spr_sc);

        for (i = 0; i < 3; ++i)
        {
             display_sprite_positional( bl[i], x, cy, z, spr_sc, spr_sc, colorback[i], H_ALIGN_LEFT, V_ALIGN_CENTER );
            display_sprite_positional( bmid[i], x + bl[i]->width*xposSc*spr_sc, cy, z, midWd/fmid->width, spr_sc, colorback[i], H_ALIGN_LEFT, V_ALIGN_CENTER );
            display_sprite_positional( br[i], x + wd*xposSc*sc, cy, z, spr_sc, spr_sc, colorback[i], H_ALIGN_RIGHT, V_ALIGN_CENTER );
        }

        display_sprite_positional_blend( fmid, x + bl[0]->width*xposSc*spr_sc, cy, z, (midWd/2)/fmid->width, spr_sc, colorfront&0xffffff00, colorfront, colorfront, colorfront&0xffffff00, H_ALIGN_LEFT, V_ALIGN_CENTER );
        display_sprite_positional_blend( fmid, x + wd*xposSc*sc - bl[0]->width*xposSc*spr_sc, cy, z, (wd*sc/2 - bl[0]->width*spr_sc)/fmid->width, spr_sc, colorfront, colorfront&0xffffff00, colorfront&0xffffff00, colorfront, H_ALIGN_RIGHT, V_ALIGN_CENTER );
    }
    else if( flags&HB_ROUND_ENDS )
    {
        F32 midWd;
        fmid = atlasLoadTexture("selectiontabsharp_frame_mid");
        bl[0] = atlasLoadTexture("selectiontabsharp_tip");
        bl[1] = atlasLoadTexture("selectiontabsharp_tip_2");
        bl[2] = atlasLoadTexture("selectiontabsharp_tip_3");
        midWd = ((wd*sc)- (bl[0]->width*2*spr_sc));

        for (i = 0; i < 3; ++i)
        {
              display_sprite_positional( bl[i], cx - wd*xposSc*sc/2, cy, z, spr_sc, spr_sc, colorback[i], H_ALIGN_LEFT, V_ALIGN_CENTER );
            display_sprite_positional( bmid[i], cx, cy, z, midWd/fmid->width, spr_sc, colorback[i], H_ALIGN_CENTER, V_ALIGN_CENTER );
            display_sprite_positional_flip( bl[i], cx + wd*xposSc*sc/2, cy, z, spr_sc, spr_sc, colorback[i], FLIP_HORIZONTAL, H_ALIGN_RIGHT, V_ALIGN_CENTER );
        }

        display_sprite_positional_blend( fmid, cx, cy, z, (midWd*sc/2)/fmid->width, spr_sc, colorfront&0xffffff00, colorfront, colorfront, colorfront&0xffffff00, H_ALIGN_RIGHT, V_ALIGN_CENTER );
        display_sprite_positional_blend( fmid, cx, cy, z, (midWd*sc/2)/fmid->width, spr_sc, colorfront, colorfront&0xffffff00, colorfront&0xffffff00, colorfront, H_ALIGN_LEFT, V_ALIGN_CENTER );
    }
    else if( flags&HB_ROUND_ENDS_THIN )
    {
          fmid = atlasLoadTexture("selectiontabround_small_frame_mid_0");
        bl[0] = atlasLoadTexture("selectiontabround_small_tip_0");
        bl[1] = atlasLoadTexture("selectiontabround_small_tip_1");
        bl[2] = atlasLoadTexture("selectiontabround_small_tip_2");
        br[0] = atlasLoadTexture("selectiontabround_small_end_0");
        br[1] = atlasLoadTexture("selectiontabround_small_end_1");
        br[2] = atlasLoadTexture("selectiontabround_small_end_2");

        for (i = 0; i < 3; ++i)
        {
            display_sprite_positional( bl[i], cx - wd*xposSc*sc/2, cy, z, spr_sc, spr_sc, colorback[i], H_ALIGN_LEFT, V_ALIGN_CENTER );
            display_sprite_positional( bmid[i], cx - (wd*sc/2 - bl[i]->width*spr_sc)*xposSc, cy, z, ((wd*sc)- (bl[i]->width*2*spr_sc))/bmid[i]->width, spr_sc, colorback[i], H_ALIGN_LEFT, V_ALIGN_CENTER );
            display_sprite_positional( br[i], cx + wd*xposSc*sc/2, cy, z, spr_sc, spr_sc, colorback[i], H_ALIGN_RIGHT, V_ALIGN_CENTER );
        }

        display_sprite_positional_blend( fmid, cx, cy, z, (wd*sc/2)/fmid->width, spr_sc, colorfront&0xffffff00, colorfront, colorfront, colorfront&0xffffff00, H_ALIGN_RIGHT, V_ALIGN_CENTER );
        display_sprite_positional_blend( fmid, cx, cy, z, (wd*sc/2)/fmid->width, spr_sc, colorfront, colorfront&0xffffff00, colorfront&0xffffff00, colorfront, H_ALIGN_LEFT, V_ALIGN_CENTER );
    }
    else
    {
        fmid = atlasLoadTexture("selectiontabround_frame_mid");

        for (i = 0; i < 3; ++i)
        {
             display_sprite_positional_blend( bmid[i], cx - wd*xposSc*sc/2, cy, z, end_wd*spr_sc/bmid[i]->width, spr_sc, colorback[i]&0xffffff00, colorback[i], colorback[i], colorback[i]&0xffffff00, H_ALIGN_LEFT, V_ALIGN_CENTER );
             display_sprite_positional( bmid[i], cx, cy, z, ((wd*sc)- (end_wd*2*spr_sc))/fmid->width, spr_sc, colorback[i], H_ALIGN_CENTER, V_ALIGN_CENTER );
            display_sprite_positional_blend( bmid[i], cx + wd*xposSc*sc/2, cy, z, end_wd*spr_sc/fmid->width, spr_sc, colorback[i], colorback[i]&0xffffff00, colorback[i]&0xffffff00, colorback[i], H_ALIGN_RIGHT, V_ALIGN_CENTER );
        }

        display_sprite_positional_blend( fmid, cx, cy, z, (wd*sc/2)/fmid->width, spr_sc, colorfront&0xffffff00, colorfront, colorfront, colorfront&0xffffff00, H_ALIGN_RIGHT, V_ALIGN_CENTER );
        display_sprite_positional_blend( fmid, cx, cy, z, (wd*sc/2)/fmid->width, spr_sc, colorfront, colorfront&0xffffff00, colorfront&0xffffff00, colorfront, H_ALIGN_LEFT, V_ALIGN_CENTER );
    }

    // Flashing highlight
    if( selected && flags&HB_FLASH )
    {
        F32 xoff, dist = wd*xposSc-150.f;
        int alpha;
         hb->timer += TIMESTEP*.1;
        if(hb->timer > 1.f )
        {
            //wait a little bit
            if( hb->timer > 5.f )
                hb->timer = 0;
        }
        else
        {
            xoff = 75.f + hb->timer * dist;
            if( xoff < 30 )
                alpha = 0xffffff00 | (int)((xoff/30.f)*255);
            else if( xoff > 300 )
                alpha = 0xffffff00 | (int)(((100-(xoff-300))/100.f)*255);
            else
                alpha = CLR_WHITE;

             display_sprite_positional_blend(fmid, x + xoff, cy, z+1, (20*spr_sc)/fmid->width, spr_sc, CLR_H_HIGHLIGHT&alpha, colorfront&0xffffff00, colorfront&0xffffff00, CLR_H_HIGHLIGHT&alpha, H_ALIGN_LEFT, V_ALIGN_CENTER );
            display_sprite_positional_blend(fmid, x + xoff - 20, cy, z+1, (20*spr_sc)/fmid->width, spr_sc, colorfront&0xffffff00, CLR_H_HIGHLIGHT&alpha, CLR_H_HIGHLIGHT&alpha, colorfront&0xffffff00, H_ALIGN_LEFT, V_ALIGN_CENTER );
        }
    }

    if (flags&HB_ROUND_ENDS_THIN || flags&HB_ROUND_ENDS)
    {
        F32 iconSc = (flags&HB_ROUND_ENDS_THIN) ? 0.7f : 1.f;
        if(selected && hb->icon1)
        {
            display_sprite_positional(hb->icon1, cx - (wd-50.f*iconSc)*xposSc*sc/2.f, cy , z, yposSc*sc*iconSc, yposSc*sc*iconSc, CLR_H_GLOW&alphafont, H_ALIGN_CENTER, V_ALIGN_CENTER );
        }
        if(hb->icon0)
            display_sprite_positional(hb->icon0, cx - (wd-50.f*iconSc)*xposSc*sc/2.f, cy, z, sc*iconSc, sc*iconSc, CLR_WHITE&alphafont, H_ALIGN_CENTER, V_ALIGN_CENTER );
    }
    else if (flags & HB_TAIL_LEFT_ROUND)  //flip icon side
    {
        if(selected && hb->icon1)
        {
            display_sprite_positional(hb->icon1, x - 40.f*spr_sc, cy, z, spr_sc, spr_sc, CLR_H_GLOW&alphafont, H_ALIGN_CENTER, V_ALIGN_CENTER );
        }
        if(hb->icon0)
            display_sprite_positional(hb->icon0, x - 40.f*spr_sc, cy, z, spr_sc, spr_sc, CLR_WHITE&alphafont, H_ALIGN_CENTER, V_ALIGN_CENTER );
    }
    else
    {
        if(selected && hb->icon1)
        {
            display_sprite_positional(hb->icon1, x + 40.f*xposSc*spr_sc, cy, z, spr_sc, spr_sc, CLR_H_GLOW&alphafont, H_ALIGN_CENTER, V_ALIGN_CENTER );
        }
        if(hb->icon0)
            display_sprite_positional(hb->icon0, x + 40.f*xposSc*spr_sc, cy, z, spr_sc, spr_sc, CLR_WHITE&alphafont, H_ALIGN_CENTER, V_ALIGN_CENTER );
    }

    if (flags & HB_SMALL_FONT)
    {
        font(&hybridbold_18);
    }
    else
    {
         font(&title_18);
        font_outl(0);
    }
    if (selected && flags&HB_HIGHLIGHT_FONT)
    {
        fontColor = CLR_H_HIGHLIGHT;
    }
    else
    {
        fontColor = CLR_WHITE;
    }
    if (flags & HB_SCALE_DOWN_FONT)
    {
        spr_sc *= 0.85f;
    }

    textXSc = textYSc = 1.f;
    if (isTextScalingRequired())
    {
        calculateSpriteScales(&textXSc, &textYSc);
        setSpriteScaleMode(SPRITE_XY_NO_SCALE);    // fonts only really work in SPRITE_XY_SCALE or SPRITE_XY_NO_SCALE
    }
      if( flags&HB_TAIL_LEFT_ROUND)
    {
        F32 strwd;
        if (hb->text_flags&NO_MSPRINT)
            strwd = str_wd_notranslate(font_grp, spr_sc, spr_sc, hb->text );
        else
            strwd = str_wd(font_grp, spr_sc, spr_sc, hb->text );

        font_color(CLR_BLACK&alphafontDS, CLR_BLACK&alphafontDS);
        cprntEx( x - (80 + strwd)*xposSc+2.f, cy + yposSc*sc+2.f, z, textXSc*spr_sc, textYSc*spr_sc, CENTER_Y|hb->text_flags, hb->text );  //dropshadow
        font_color(fontColor&alphafont, fontColor&alphafont);
        cprntEx( x - (80 + strwd)*xposSc, cy + yposSc*sc, z, textXSc*spr_sc, textYSc*spr_sc, CENTER_Y|hb->text_flags, hb->text );
    }
    else if(!flags || flags&HB_ROUND_ENDS || flags&HB_ROUND_ENDS_THIN || flags&HB_FADE_ENDS )
    {
        font_color(CLR_BLACK&alphafontDS, CLR_BLACK&alphafontDS);
        cprntEx( cx+2.f, cy + yposSc*sc+2.f, z, textXSc*spr_sc, textYSc*spr_sc, CENTER_X|CENTER_Y|hb->text_flags, hb->text );  //dropshadow
        font_color(fontColor&alphafont, fontColor&alphafont);
        cprntEx( cx, cy + yposSc*sc, z, textXSc*spr_sc, textYSc*spr_sc, CENTER_X|CENTER_Y|hb->text_flags, hb->text );
    }
    else if(flags&HB_TAIL_RIGHT && hb->icon0 == NULL)
    {
        // no icon, so we shift it more
        font_color(CLR_BLACK&alphafontDS, CLR_BLACK&alphafontDS);
        cprntEx( x + 40*xposSc+2.f, cy + yposSc*sc+2.f, z, textXSc*spr_sc, textYSc*spr_sc, CENTER_Y|hb->text_flags, hb->text );  //dropshadow
        font_color(fontColor&alphafont, fontColor&alphafont);
        cprntEx( x + 40*xposSc, cy + yposSc*sc, z, textXSc*spr_sc, textYSc*spr_sc, CENTER_Y|hb->text_flags, hb->text );
    }
    else
    {
        font_color(CLR_BLACK&alphafontDS, CLR_BLACK&alphafontDS);
        cprntEx( x + 80.f*xposSc+2.f, cy + yposSc*sc+2.f, z, textXSc*spr_sc, textYSc*spr_sc, CENTER_Y|hb->text_flags, hb->text );  //dropshadow
        font_color(fontColor&alphafont, fontColor&alphafont);
          cprntEx( x + 80.f*xposSc,     cy + yposSc*sc,     z, textXSc*spr_sc, textYSc*spr_sc, CENTER_Y|hb->text_flags, hb->text );
    }
    setSpriteScaleMode(ssm);
    if (!(flags & HB_SMALL_FONT))
    {
        font_outl(1);
    }

    return ret;
}

int drawHybridTab(HybridElement * hb, F32 x, F32 y, F32 z, F32 xsc, F32 ysc, int selected, F32 alphasc)
{
    AtlasTex * toptab[3];
    int tabcolor[3];

    CBox box;
    int i, alpha;
    F32 xposSc, yposSc;

    toptab[0] = hb->tex0;
    toptab[1] = hb->tex1;
    toptab[2] = hb->tex2;

    calculatePositionScales(&xposSc, &yposSc);
    BuildScaledCBox(&box, x, y, toptab[0]->width*xsc, toptab[0]->height*ysc );
    if( mouseCollision(&box) || selected )
    {
        //change opacity on hover
        if (hb->percent == 0.f)
        {
            sndPlay( "N_MouseEnter", SOUND_GAME );
        }
        hb->percent = stateScale(hb->percent, 1);
    }
    else
    {
        if (hb->percent == 1.f)
        {
            sndPlayNoInterrupt( "N_MouseExit", SOUND_GAME );
        }
        hb->percent = stateScale(hb->percent, 0);
    }

    alpha =        (int)((128.f + hb->percent*127.f)*alphasc);

    if (selected)
    {
        tabcolor[0] = HYBRID_SELECTED_COLOR[0]&(0xffffff00|alpha);
        tabcolor[1] = HYBRID_SELECTED_COLOR[1]&(0xffffff00|alpha);
        tabcolor[2] = HYBRID_SELECTED_COLOR[2]&(0xffffff00|alpha);
    }
    else
    {
        tabcolor[0] = HYBRID_FRAME_COLOR[0]&(0xffffff00|alpha);
        tabcolor[1] = HYBRID_FRAME_COLOR[1]&(0xffffff00|alpha);
        tabcolor[2] = HYBRID_FRAME_COLOR[2]&(0xffffff00|alpha);
    }

    for (i = 0; i < 3; ++i)
    {
        display_sprite_positional(toptab[i], x, y, z, xsc, ysc, tabcolor[i], H_ALIGN_LEFT, V_ALIGN_TOP);
    }

    //draw icon
    if (selected)
    {
        //draw glow
        if (hb->icon1)
        {
            display_sprite_positional(hb->icon1, x + 313.f*xposSc*xsc, y+toptab[0]->height*yposSc*ysc/2.f, z+1.f, xsc, ysc, CLR_H_GLOW, H_ALIGN_CENTER, V_ALIGN_CENTER);
        }
    }
    if (hb->icon0)
    {
        display_sprite_positional(hb->icon0, x + 313.f*xposSc*xsc, y+toptab[0]->height*yposSc*ysc/2.f, z+1.f, xsc, ysc, 0xffffff00|alpha, H_ALIGN_CENTER, V_ALIGN_CENTER);
    }

    if( mouseClickHit(&box, MS_LEFT ) )
    {
        sndPlay( "N_Select", SOUND_GAME );
        return 1;
    }
    else
    {
        return 0;
    }
}

void drawHybridTabFrame(F32 x, F32 y, F32 z, F32 wd, F32 ht, F32 fadePct)
{
    AtlasTex * lcorner = atlasLoadTexture("powertab_frame_corner1_0");
    AtlasTex * rcorner = atlasLoadTexture("powertab_frame_corner2_0");
    AtlasTex * lvert = atlasLoadTexture("powertab_frame_corner1_vertical_0");
    AtlasTex * rvert = atlasLoadTexture("powertab_frame_corner2_vertical_0");
    AtlasTex * horiz = atlasLoadTexture("powertab_frame_corner1_horizontal_0");
    AtlasTex * basecorner = atlasLoadTexture("powertab_base_0");
    AtlasTex * basehoriz = atlasLoadTexture("powertab_base_horizontal_0");
    AtlasTex * basevert = atlasLoadTexture("powertab_base_vertical_0");

    F32 horizWd = wd - 2*basecorner->width;
    F32 vertHt = ht - basecorner->height;
    F32 xposSc, yposSc, xwalk, ywalk;
    calculatePositionScales(&xposSc, &yposSc);
    xwalk = x;
    ywalk = y;
    display_sprite_positional(basecorner, xwalk, ywalk, z, 1.f, 1.f, (CLR_H_TAB_BACK&0xffffff7f), H_ALIGN_LEFT, V_ALIGN_TOP );
    display_sprite_positional(basehoriz, xwalk+=basecorner->width*xposSc, ywalk, z, horizWd/basehoriz->width, 1.f, (CLR_H_TAB_BACK&0xffffff7f), H_ALIGN_LEFT, V_ALIGN_TOP );
    display_sprite_positional_flip(basecorner, xwalk+=horizWd*xposSc, ywalk, z, 1.f, 1.f, (CLR_H_TAB_BACK&0xffffff7f), FLIP_HORIZONTAL, H_ALIGN_LEFT, V_ALIGN_TOP );
    xwalk = x;
    display_sprite_positional(basevert, xwalk, ywalk+=basecorner->height*yposSc, z, 1.f, vertHt*(1.f-fadePct)/basevert->height, (CLR_H_TAB_BACK&0xffffff7f), H_ALIGN_LEFT, V_ALIGN_TOP );
    display_sprite_positional(white_tex_atlas, xwalk+=basecorner->width*xposSc, ywalk, z, horizWd/white_tex_atlas->width, vertHt*(1.f-fadePct)/white_tex_atlas->height, (CLR_H_TAB_BACK&0xffffff7f), H_ALIGN_LEFT, V_ALIGN_TOP );
    display_sprite_positional_flip(basevert, xwalk+=horizWd*xposSc, ywalk, z, 1.f, vertHt*(1.f-fadePct)/basevert->height, (CLR_H_TAB_BACK&0xffffff7f), FLIP_HORIZONTAL, H_ALIGN_LEFT, V_ALIGN_TOP );
    xwalk = x;
    display_sprite_positional_blend(basevert, xwalk, ywalk+=vertHt*(1.f-fadePct)*yposSc, z, 1.f, vertHt*fadePct/basevert->height, (CLR_H_TAB_BACK&0xffffff7f), (CLR_H_TAB_BACK&0xffffff7f), (CLR_H_TAB_BACK&0xffffff00), (CLR_H_TAB_BACK&0xffffff00), H_ALIGN_LEFT, V_ALIGN_TOP );
    display_sprite_positional_blend(white_tex_atlas, xwalk+=basecorner->width*xposSc, ywalk, z, horizWd/white_tex_atlas->width, vertHt*fadePct/white_tex_atlas->height, (CLR_H_TAB_BACK&0xffffff7f), (CLR_H_TAB_BACK&0xffffff7f), (CLR_H_TAB_BACK&0xffffff00), (CLR_H_TAB_BACK&0xffffff00), H_ALIGN_LEFT, V_ALIGN_TOP );
    display_sprite_ex(basevert, 0, xwalk+=horizWd*xposSc, ywalk, z, 1.f, vertHt*fadePct/basevert->height, (CLR_H_TAB_BACK&0xffffff7f), (CLR_H_TAB_BACK&0xffffff7f), (CLR_H_TAB_BACK&0xffffff00), (CLR_H_TAB_BACK&0xffffff00), 1, 0, 0, 1, 0, 0, clipperGetCurrent(), getScreenScaleOption(), H_ALIGN_LEFT, V_ALIGN_TOP);

    horizWd = wd - lcorner->width - rcorner->width;
    vertHt = ht - lcorner->height;
    xwalk = x;
    ywalk = y;
    display_sprite_positional(lcorner, xwalk, ywalk, z, 1.f, 1.f, CLR_H_DARK_BACK, H_ALIGN_LEFT, V_ALIGN_TOP );
    display_sprite_positional(horiz, xwalk+=lcorner->width*xposSc, ywalk, z, horizWd/horiz->width, 1.f, CLR_H_DARK_BACK, H_ALIGN_LEFT, V_ALIGN_TOP );
    display_sprite_positional(rcorner, xwalk+=horizWd*xposSc, ywalk, z, 1.f, 1.f, CLR_H_DARK_BACK, H_ALIGN_LEFT, V_ALIGN_TOP );
    xwalk = x;
    display_sprite_positional(lvert, xwalk, ywalk+=lcorner->height*yposSc, z, 1.f, vertHt*(1.f-fadePct)/lvert->height, CLR_H_DARK_BACK, H_ALIGN_LEFT, V_ALIGN_TOP );
    display_sprite_positional(rvert, xwalk+(lcorner->width+horizWd)*xposSc, ywalk, z, 1.f, vertHt*(1.f-fadePct)/lvert->height, CLR_H_DARK_BACK, H_ALIGN_LEFT, V_ALIGN_TOP );
    display_sprite_positional_blend(lvert, xwalk, ywalk+=vertHt*(1.f-fadePct)*yposSc, z, 1.f, vertHt*fadePct/lvert->height, CLR_H_DARK_BACK, CLR_H_DARK_BACK, CLR_H_DARK_BACK&0xffffff00, CLR_H_DARK_BACK&0xffffff00, H_ALIGN_LEFT, V_ALIGN_TOP );
    display_sprite_positional_blend(rvert, xwalk+(lcorner->width+horizWd)*xposSc, ywalk, z, 1.f, vertHt*fadePct/lvert->height, CLR_H_DARK_BACK, CLR_H_DARK_BACK, CLR_H_DARK_BACK&0xffffff00, CLR_H_DARK_BACK&0xffffff00, H_ALIGN_LEFT, V_ALIGN_TOP );
}

void drawLWCUI(F32 cx, F32 cy, F32 z, F32 sc)
{
#ifndef FORCE_LWC_UI
    if (LWC_IsActive())
#endif
    {
        AtlasTex *tex2 = atlasLoadTexture("circulartab_2.tga");
        CBox box;
        F32 xcbox, ycbox, wcbox, hcbox;
        F32 xposSc, yposSc;
        F32 textXSc = 1.f;
        F32 textYSc = 1.f;
        SpriteScalingMode ssm = getSpriteScaleMode();
        calculatePositionScales(&xposSc, &yposSc);
        xcbox = cx-tex2->width*xposSc*sc/2.f;
        ycbox = cy-tex2->height*yposSc*sc/2.f;
        wcbox = tex2->width*xposSc*sc;
        hcbox = tex2->height*yposSc*sc;
        display_sprite_positional( tex2, cx, cy, z, sc, sc, CLR_H_BACK, H_ALIGN_CENTER, V_ALIGN_CENTER );
        if (isTextScalingRequired())
        {
            calculateSpriteScales(&textXSc, &textYSc);
            setSpriteScaleMode(SPRITE_XY_NO_SCALE);
        }
        font(&hybrid_12);
        font_color(CLR_WHITE, CLR_WHITE);
        cprnt(cx, cy+20.f*yposSc*sc, z, textXSc*sc, textYSc*sc, "StageNum", LWC_GetLastCompletedDataStage()+1); // increment by one to show current stage
        cprnt(cx, cy, z, textXSc*sc, textYSc*sc, "%2.0f%%", floor(LWC_GetPercentDoneNextStage()*100.f));
        setSpriteScaleMode(ssm);

        BuildScaledCBox(&box, xcbox, ycbox, wcbox, hcbox);
        if (mouseCollision(&box))
        {
            setHybridTooltipWindow(&box, "LWCStageInfo", current_menu() == MENU_GAME ? WDW_LWC_UI : 0);  //this function may be called from LWC window
        }
    }
}

int drawHybridCommon(int menuType)
{
    AtlasTex * back = atlasLoadTexture("background.tga");
    int ret = 0;

    display_sprite(back, 0, 0, 2, 1.f, 1.f, CLR_WHITE );

    //set type for remaining hybrid common calls this frame
    sMenuType = menuType;
    if (menuType != HYBRID_MENU_NO_NAVIGATION)
    {
        sLastNavMenuType = menuType;
    }

    //DEBUG ONLY.. needs to be removed
    if( inpEdge(INP_F1) && isDevelopmentMode() )
        game_state.showMouseCoords = !game_state.showMouseCoords;
    //END DEBUG ONLY

    // update rotation
    spinAngle += TIMESTEP * -0.01f;
    spinAngle = fmodf(spinAngle,6.28318f);

    if (menuType != HYBRID_MENU_NO_NAVIGATION)
    {
        if( drawNavigationSet() )
            ret = 1;
    }
    sIncomplete = 0;
    if (!ret && menuType == HYBRID_MENU_CREATION)
    {
        F32 xposSc, yposSc, xp, yp;
        SpriteScalingMode ssm = getSpriteScaleMode();

        toggle_3d_game_modes(SHOW_NONE); 

        updateRedirect();
          refreshCheckName();  //not draw code: refresh name check

        setScalingModeAndOption(SPRITE_Y_UNIFORM_SCALE, SSO_SCALE_TEXTURE);
        calculatePositionScales(&xposSc, &yposSc);
        xp = DEFAULT_SCRN_WD / 2.f;
        yp = 0.f;
        applyPointToScreenScalingFactorf(&xp, &yp);
        if( !isMenu(MENU_LOADCOSTUME) && !isMenu(MENU_SAVECOSTUME) && !isMenu(MENU_LOAD_POWER_CUST) )
        {
             if(isMenu(MENU_REGISTER))
            {
                drawCheckNameBar(HYBRID_NAME_REGISTER);
            }
            else
            {
                drawCheckNameBar(HYBRID_NAME_CREATION);
            }
        }

        drawLWCUI(xp-177.f*xposSc, 710.f*yposSc, 5.f, 1.f);
        setScalingModeAndOption(SPRITE_XY_SCALE, SSO_SCALE_ALL);
    }

    return ret;
}

void drawHybridDesc( SMFBlock *sm, F32 x, F32 y, F32 z, F32 sc, F32 wd, F32 ht, const char * text, int reparse, int alpha )
{
    UIBox box;
    int textHt;
    F32 xposSc, yposSc;
    F32 textXSc = 1.f;
    F32 textYSc = 1.f;
    SpriteScalingMode ssm = getSpriteScaleMode();
    calculatePositionScales(&xposSc, &yposSc);
    if (isTextScalingRequired())
    {
        calculateSpriteScales(&textXSc, &textYSc);  //grab the scale
        setSpriteScaleMode(SPRITE_XY_NO_SCALE);  //required since we are dealing with text
    }

    uiBoxDefineScaled(&box, x, y, wd*xposSc, ht*yposSc);
    clipperPush(&box);

    gHybridTextAttr.piColor = (int*)((int)(gHybridTextAttr.piColor)&0xffffff00);
    gHybridTextAttr.piColor = (int*)((int)(gHybridTextAttr.piColor)|alpha);
    gHybridTextAttr.piScale = (int*)(int)(sc*textYSc*SMF_FONT_SCALE);
    textHt = smf_ParseAndFormat(sm, textStd(text), x, y, z, wd*textXSc, 0, false, reparse, false, &gHybridTextAttr, NULL);
    if (textHt > ht*yposSc)
    {
        //draw smaller
        gHybridTextAttr.piScale = (int*)(int)(sc*textYSc*0.85f*SMF_FONT_SCALE);
    }
    smf_Display(sm, x, y, z, wd*textXSc, 0, reparse, false, &gHybridTextAttr, NULL);
    gHybridTextAttr.piColor = (int*)((int)(gHybridTextAttr.piColor)|0xff);
    gHybridTextAttr.piScale = (int*)((int)(1.f*SMF_FONT_SCALE));  //reset

    clipperPop();
    setSpriteScaleMode(ssm);
}

void drawHybridDescFrame(F32 x, F32 y, F32 z, F32 wd, F32 ht, int flags, F32 fadePct, F32 alpha, F32 backgroundOpacity, HAlign ha, VAlign va)
{
    AtlasTex * lcorner[3] = 
    {
        atlasLoadTexture("desframe_corner1_0"),
        atlasLoadTexture("desframe_corner1_1"),
        atlasLoadTexture("desframe_corner1_2")
    };
    AtlasTex * rcorner[3] = 
    {
        atlasLoadTexture("desframe_corner2_0"),
        atlasLoadTexture("desframe_corner2_1"),
        atlasLoadTexture("desframe_corner2_2")
    };
    AtlasTex * horiz[3] = 
    {
        atlasLoadTexture("desframe_corner1_horizontal_0"),
        atlasLoadTexture("desframe_corner1_horizontal_1"),
        atlasLoadTexture("desframe_corner1_horizontal_2")
    };
    AtlasTex * blcorner = atlasLoadTexture("desframe_corner3_0");
    AtlasTex * brcorner = atlasLoadTexture("desframe_corner4_0");
    AtlasTex * bhoriz = atlasLoadTexture("desframe_corner4_horizontal_0");
    AtlasTex * lvert = atlasLoadTexture("desframe_corner1_vertical_0");
    AtlasTex * rvert = atlasLoadTexture("desframe_corner2_vertical_0");
    AtlasTex * basecorner = atlasLoadTexture("desbase_corner1_0");
    AtlasTex * basehoriz = atlasLoadTexture("desbase_horizontal_0");
    AtlasTex * basevert = atlasLoadTexture("desbase_vertical_0");
    AtlasTex * bbasecorner = atlasLoadTexture("desbase_corner3_0");
    AtlasTex * bbasehoriz = atlasLoadTexture("desbase_corner4_horizontal_0");
    int framecolor[3] = 
    {
        (CLR_H_DARK_BACK&0xffffff00) | (int)(0xff * alpha),
        (CLR_H_BRIGHT_BACK&0xffffff00) | (int)(0xff * alpha),
        (CLR_H_BACK&0xffffff00) | (int)(0xff * alpha)
    };
    int backAlpha = backgroundOpacity * 0xff;
    int backcolor[2] = 
    {
        (CLR_H_TAB_BACK&0xffffff00) | (int)(backAlpha * alpha),
        (CLR_H_TAB_BACK&0xffffff00)
    };
    int i;
    F32 xsc = 1.f;
    F32 ysc = 1.f;
    F32 xposSc, yposSc;
    F32 vertHt = ht;
    F32 horizWd = wd;
    int addVertCorner = 0;
    int addHorizCorner = 0;
    CBox box;

    calculatePositionScales(&xposSc, &yposSc);
    switch (ha)
    {
    case H_ALIGN_CENTER:
        x -= wd*xposSc/2.f;
        break;
    case H_ALIGN_RIGHT:
        x -= wd*xposSc;
        break;
    case H_ALIGN_LEFT:
    default:
        //x already in the correct place
        break;
    }
    switch (va)
    {
    case V_ALIGN_CENTER:
        y -= ht*yposSc/2.f;
        break;
    case V_ALIGN_BOTTOM:
        y -= ht*yposSc;
        break;
    case V_ALIGN_TOP:
    default:
        //y already in the correct place
        break;
    }

    if (flags & HB_DESCFRAME_HOVERBACKGROUND)
    {
        BuildScaledCBox(&box, x, y, wd*xposSc, ht*yposSc);
        if (mouseCollision(&box))
        {
            backcolor[0] = (CLR_H_TAB_BACK&0xffffff00) | (int)(0xcc * alpha);
        }
    }

    if (flags & (HB_DESCFRAME_FULLFRAME | HB_DESCFRAME_FADELEFT))
    {
        // add bottom corners
        ++addVertCorner;
    }
    // add left corners
    if (flags & (HB_DESCFRAME_FULLFRAME | HB_DESCFRAME_FADEDOWN))
    {
        ++addHorizCorner;
    }

    //take in account a compressed state
    if (ht < basecorner->height+addVertCorner*bbasecorner->height)
    {
        ysc *= ht/(basecorner->height+addVertCorner*bbasecorner->height);
    }
    if (wd < basecorner->width+addHorizCorner*basecorner->width)
    {
        xsc *= wd/(basecorner->width+addHorizCorner*basecorner->width);
    }
    vertHt -= (basecorner->height+addVertCorner*bbasecorner->height)*ysc;
    horizWd -= (basecorner->width+addHorizCorner*basecorner->width)*xsc;

    if (flags & HB_DESCFRAME_FADEDOWN)
    {
        display_sprite_positional( basecorner, x, y, z, xsc, ysc, backcolor[0], H_ALIGN_LEFT, V_ALIGN_TOP);
        display_sprite_positional( basehoriz, x+wd*xposSc/2.f, y, z, horizWd/basehoriz->width, ysc, backcolor[0], H_ALIGN_CENTER, V_ALIGN_TOP);
        display_sprite_positional_flip( basecorner, x+wd*xposSc, y, z, xsc, ysc, backcolor[0], FLIP_HORIZONTAL, H_ALIGN_RIGHT, V_ALIGN_TOP);
        display_sprite_positional( basevert, x, y+basecorner->height*yposSc*ysc, z, xsc, vertHt*(1.f-fadePct)/basevert->height, backcolor[0], H_ALIGN_LEFT, V_ALIGN_TOP);
        display_sprite_positional_flip( basevert, x+wd*xposSc, y+basecorner->height*yposSc*ysc, z, xsc, vertHt*(1.f-fadePct)/basevert->height, backcolor[0], FLIP_HORIZONTAL, H_ALIGN_RIGHT, V_ALIGN_TOP);
        display_sprite_positional( white_tex_atlas, x+wd*xposSc/2.f, y+basehoriz->height*yposSc*ysc, z, horizWd/white_tex_atlas->width, vertHt*(1.f-fadePct)/white_tex_atlas->height, backcolor[0], H_ALIGN_CENTER, V_ALIGN_TOP);
        display_sprite_positional_blend( basevert, x, y+ht*yposSc, z, xsc, vertHt*fadePct/basevert->height, backcolor[0], backcolor[0], backcolor[1], backcolor[1], H_ALIGN_LEFT, V_ALIGN_BOTTOM);
        display_sprite_ex( basevert, 0, x+wd*xposSc, y+ht*yposSc, z, xsc, vertHt*fadePct/basevert->height, backcolor[0], backcolor[0], backcolor[1], backcolor[1], 1, 0, 0, 1, 0, 0, clipperGetCurrent(), getScreenScaleOption(), H_ALIGN_RIGHT, V_ALIGN_BOTTOM);
        display_sprite_positional_blend( white_tex_atlas, x+wd*xposSc/2.f, y+ht*yposSc, z, horizWd/white_tex_atlas->width, vertHt*fadePct/white_tex_atlas->height, backcolor[0], backcolor[0], backcolor[1], backcolor[1], H_ALIGN_CENTER, V_ALIGN_BOTTOM);
    }
    else if (flags & HB_DESCFRAME_FADELEFT)
    {
        display_sprite_positional_blend( basehoriz, x, y, z, horizWd*fadePct/basehoriz->width, ysc, backcolor[1], backcolor[0], backcolor[0], backcolor[1], H_ALIGN_LEFT, V_ALIGN_TOP );
        display_sprite_positional( basehoriz, x+horizWd*fadePct*xposSc, y, z, horizWd*(1.f-fadePct)/basehoriz->width, ysc, backcolor[0], H_ALIGN_LEFT, V_ALIGN_TOP );
        display_sprite_positional_flip( basecorner, x+wd*xposSc, y, z, xsc, ysc, backcolor[0], FLIP_HORIZONTAL, H_ALIGN_RIGHT, V_ALIGN_TOP );
        display_sprite_positional_flip( basevert, x+wd*xposSc, y+basecorner->height*yposSc*ysc, z, xsc, vertHt/basevert->height, backcolor[0], FLIP_HORIZONTAL, H_ALIGN_RIGHT, V_ALIGN_TOP );
        display_sprite_positional_blend( white_tex_atlas, x, y+basecorner->height*yposSc*ysc, z, horizWd*fadePct/white_tex_atlas->width, vertHt/white_tex_atlas->height, backcolor[1], backcolor[0], backcolor[0], backcolor[1], H_ALIGN_LEFT, V_ALIGN_TOP );
        display_sprite_positional( white_tex_atlas, x+horizWd*fadePct*xposSc, y+basecorner->height*yposSc*ysc, z, horizWd*(1.f-fadePct)/white_tex_atlas->width, vertHt/white_tex_atlas->height, backcolor[0], H_ALIGN_LEFT, V_ALIGN_TOP );
    }
    else //HB_DESCFRAME_FULLFRAME
    {
        display_sprite_positional( basecorner, x, y, z, xsc, ysc, backcolor[0], H_ALIGN_LEFT, V_ALIGN_TOP );
        display_sprite_positional( basehoriz, x+wd*xposSc/2.f, y, z, horizWd/basehoriz->width, ysc, backcolor[0], H_ALIGN_CENTER, V_ALIGN_TOP );
        display_sprite_positional_flip( basecorner, x+wd*xposSc, y, z, xsc, ysc, backcolor[0], FLIP_HORIZONTAL, H_ALIGN_RIGHT, V_ALIGN_TOP );
        display_sprite_positional( basevert, x, y+basecorner->height*yposSc*ysc, z, xsc, vertHt/basevert->height, backcolor[0], H_ALIGN_LEFT, V_ALIGN_TOP );
        display_sprite_positional_flip( basevert, x+wd*xposSc, y+basecorner->height*yposSc*ysc, z, xsc, vertHt/basevert->height, backcolor[0], FLIP_HORIZONTAL, H_ALIGN_RIGHT, V_ALIGN_TOP );
        display_sprite_positional( white_tex_atlas, x+wd*xposSc/2.f, y+basecorner->height*yposSc*ysc, z, horizWd/white_tex_atlas->width, vertHt/white_tex_atlas->height, backcolor[0], H_ALIGN_CENTER, V_ALIGN_TOP );
    }

    //draw the bottom of the frame only if we're not fading out
    if (flags & (HB_DESCFRAME_FULLFRAME | HB_DESCFRAME_FADELEFT))
    {
        if (flags & HB_DESCFRAME_FADELEFT)
        {
            //faded bottom frame
            display_sprite_positional_blend( bbasehoriz, x, y+ht*yposSc, z, horizWd*fadePct/bbasehoriz->width, ysc, backcolor[1], backcolor[0], backcolor[0], backcolor[1], H_ALIGN_LEFT, V_ALIGN_BOTTOM);
            display_sprite_positional( bbasehoriz, x+horizWd*fadePct*xposSc, y+ht*yposSc, z, horizWd*(1.f-fadePct)/bbasehoriz->width, 1.f, backcolor[0], H_ALIGN_LEFT, V_ALIGN_BOTTOM);
            display_sprite_positional_flip( bbasecorner, x+wd*xposSc, y+ht*yposSc, z, xsc, ysc, backcolor[0], FLIP_HORIZONTAL, H_ALIGN_RIGHT, V_ALIGN_BOTTOM);
        }
        else
        {
            // full bottom frame
            display_sprite_positional( bbasecorner, x, y+ht*yposSc, z, xsc, ysc, backcolor[0], H_ALIGN_LEFT, V_ALIGN_BOTTOM);
            display_sprite_positional( bbasehoriz, x+wd*xposSc/2.f, y+ht*yposSc, z, horizWd/bbasehoriz->width, ysc, backcolor[0], H_ALIGN_CENTER, V_ALIGN_BOTTOM);
            display_sprite_positional_flip( bbasecorner, x+wd*xposSc, y+ht*yposSc, z, xsc, ysc, backcolor[0], FLIP_HORIZONTAL, H_ALIGN_RIGHT, V_ALIGN_BOTTOM);
        }
    }

    vertHt = ht;
    horizWd = wd;
    ysc = 1.f;
    xsc = 1.f;

    if (ht < rcorner[0]->height+addVertCorner*brcorner->height)
    {
        ysc *= ht/(rcorner[0]->height+addVertCorner*brcorner->height);
    }
    if (wd < rcorner[0]->width+addHorizCorner*lcorner[0]->width)
    {
        xsc *= wd/(rcorner[0]->width+addHorizCorner*lcorner[0]->width);
    }
    vertHt -= (rcorner[0]->height+addVertCorner*brcorner->height)*ysc;
    horizWd -= (rcorner[0]->width+addHorizCorner*lcorner[0]->width)*xsc;

    if (flags & HB_DESCFRAME_FADEDOWN)
    {
        for (i = 0; i < 3; ++i)
        {
            display_sprite_positional( lcorner[i], x, y, z, xsc, ysc, framecolor[i], H_ALIGN_LEFT, V_ALIGN_TOP);
            display_sprite_positional( horiz[i], x+lcorner[i]->width*xposSc*xsc, y, z, horizWd/horiz[i]->width, ysc, framecolor[i], H_ALIGN_LEFT, V_ALIGN_TOP);
            display_sprite_positional( rcorner[i], x+wd*xposSc, y, z, xsc, ysc, framecolor[i], H_ALIGN_RIGHT, V_ALIGN_TOP);
        }
        display_sprite_positional( lvert, x, y+lcorner[0]->height*yposSc*ysc, z, xsc, vertHt*(1.f-fadePct)/lvert->height, framecolor[0], H_ALIGN_LEFT, V_ALIGN_TOP);
        display_sprite_positional( rvert, x+wd*xposSc, y+lcorner[0]->height*yposSc*ysc, z, xsc, vertHt*(1.f-fadePct)/lvert->height, framecolor[0], H_ALIGN_RIGHT, V_ALIGN_TOP);
        display_sprite_positional_blend( lvert, x, y+ht*yposSc, z, xsc, vertHt*fadePct/lvert->height, framecolor[0], framecolor[0], framecolor[0]&0xffffff00, framecolor[0]&0xffffff00, H_ALIGN_LEFT, V_ALIGN_BOTTOM);
        display_sprite_positional_blend( rvert, x+wd*xposSc, y+ht*yposSc, z, xsc, vertHt*fadePct/lvert->height, framecolor[0], framecolor[0], framecolor[0]&0xffffff00, framecolor[0]&0xffffff00, H_ALIGN_RIGHT, V_ALIGN_BOTTOM);
    }
    else if(flags & HB_DESCFRAME_FADELEFT)
    {
        for (i = 0; i < 3; ++i)
        {
            display_sprite_positional_blend( horiz[i], x, y, z, horizWd*fadePct/horiz[i]->width, ysc, framecolor[i]&0xffffff00, framecolor[i], framecolor[i], framecolor[i]&0xffffff00, H_ALIGN_LEFT, V_ALIGN_TOP );
            display_sprite_positional( horiz[i], x+horizWd*fadePct*xposSc, y, z, horizWd*(1.f-fadePct)/horiz[i]->width, ysc, framecolor[i], H_ALIGN_LEFT, V_ALIGN_TOP );
            display_sprite_positional( rcorner[i], x+wd*xposSc, y, z, xsc, ysc, framecolor[i], H_ALIGN_RIGHT, V_ALIGN_TOP );
        }
        display_sprite_positional(rvert, x+wd*xposSc, y+lcorner[0]->height*yposSc*ysc, z, xsc, vertHt/lvert->height, framecolor[0], H_ALIGN_RIGHT, V_ALIGN_TOP );
    }
    else //HB_DESCFRAME_FULLFRAME
    {
        for (i = 0; i < 3; ++i)
        {
            display_sprite_positional( lcorner[i], x, y, z, xsc, ysc, framecolor[i], H_ALIGN_LEFT, V_ALIGN_TOP );
            display_sprite_positional( horiz[i], x+lcorner[i]->width*xposSc*xsc, y, z, horizWd/horiz[i]->width, ysc, framecolor[i], H_ALIGN_LEFT, V_ALIGN_TOP );
            display_sprite_positional( rcorner[i], x+wd*xposSc, y, z, xsc, ysc, framecolor[i], H_ALIGN_RIGHT, V_ALIGN_TOP );
        }
        display_sprite_positional( lvert, x, y+lcorner[0]->height*yposSc*ysc, z, xsc, vertHt/lvert->height, framecolor[0], H_ALIGN_LEFT, V_ALIGN_TOP );
        display_sprite_positional( rvert, x+wd*xposSc, y+lcorner[0]->height*yposSc*ysc, z, xsc, vertHt/lvert->height, framecolor[0], H_ALIGN_RIGHT, V_ALIGN_TOP );
    }

    //draw the bottom of the frame only if we're not fading out
    if (flags & (HB_DESCFRAME_FULLFRAME | HB_DESCFRAME_FADELEFT))
    {
        if (flags & HB_DESCFRAME_FADELEFT)
        {
            // faded bottom frame
            display_sprite_positional_blend( bhoriz, x, y+ht*yposSc, z, horizWd*fadePct/horiz[0]->width, ysc, framecolor[0]&0xffffff00, framecolor[0], framecolor[0], framecolor[0]&0xffffff00, H_ALIGN_LEFT, V_ALIGN_BOTTOM );
            display_sprite_positional( bhoriz, x+horizWd*fadePct*xposSc, y+ht*yposSc, z, horizWd*(1.f-fadePct)/horiz[0]->width, ysc, framecolor[0], H_ALIGN_LEFT, V_ALIGN_BOTTOM );
            display_sprite_positional( brcorner, x+wd*xposSc, y+ht*yposSc, z, xsc, ysc, framecolor[0], H_ALIGN_RIGHT, V_ALIGN_BOTTOM );
        }
        else
        {
            // full bottom frame
            display_sprite_positional( blcorner, x, y+ht*yposSc, z, xsc, ysc, framecolor[0], H_ALIGN_LEFT, V_ALIGN_BOTTOM );
            display_sprite_positional( bhoriz, x+blcorner->width*xposSc*xsc, y+ht*yposSc, z, horizWd/horiz[0]->width, ysc, framecolor[0], H_ALIGN_LEFT, V_ALIGN_BOTTOM );
            display_sprite_positional( brcorner, x+wd*xposSc, y+ht*yposSc, z, xsc, ysc, framecolor[0], H_ALIGN_RIGHT, V_ALIGN_BOTTOM );
        }
    }
}

F32 drawHybridVerticalSlider( F32 cx, F32 cy, F32 z, F32 ht, F32 val)
{
    AtlasTex * tip = atlasLoadTexture("scrollbar_tip_0");
    AtlasTex * mid = atlasLoadTexture("scrollbar_mid_0");
    AtlasTex * knob    = atlasLoadTexture( "sliderbar_node_round_0" );

    F32 wd = 8, y, prevY, sc = .8f;
    int alpha = 0xffffffcc;
    CBox box;
    static int dragOffY, dragged;
    F32 xposSc, yposSc;

    calculatePositionScales(&xposSc, &yposSc);
    display_sprite_positional(tip, cx, cy - ht*yposSc/2, z, 1.f, 1.f, CLR_H_GREY, H_ALIGN_CENTER, V_ALIGN_BOTTOM );
     display_sprite_positional(mid, cx, cy, z, 1.f, ht/mid->height, CLR_H_GREY, H_ALIGN_CENTER, V_ALIGN_CENTER );
    display_sprite_positional_flip(tip, cx, cy + ht*yposSc/2, z, 1.f, 1.f, CLR_H_GREY, FLIP_VERTICAL, H_ALIGN_CENTER, V_ALIGN_TOP );

    prevY = y = cy/yposSc + -val*(ht/2);

    // handle mouse collisions
    BuildScaledCBox( &box, cx - (knob->width/2)*xposSc, (y - knob->height/2)*yposSc, knob->width*xposSc, knob->height*yposSc );

    if( mouseCollision(&box) )
    {    
        sc = .85f;
        alpha = 0xffffffff;
    }

    if( mouseDownHit( &box, MS_LEFT ) )
    {
        F32 mcx, mcy = gMouseInpCur.y;

        if( shell_menu() && !is_scrn_scaling_disabled())
            calc_scaled_point( &mcx, &mcy );

        dragged = 1;
        dragOffY = y - mcy;
    }

    if( dragged )
    {
        F32 mcx, mcy = gMouseInpCur.y;

        if( shell_menu() && !is_scrn_scaling_disabled() )
        {
            calc_scaled_point( &mcx, &mcy );
        }

        y = mcy + dragOffY;

        gScrollBarDragging = true;
        collisions_off_for_rest_of_frame = true;
    }

    // keep slider in bounds
    if( y < cy/yposSc - ht/2  )
        y = cy/yposSc - ht/2;
    if( y > cy/yposSc + ht/2 )
        y = cy/yposSc + ht/2;

    if (dragged)
    {
        if (!isDown( MS_LEFT ))
        {
            dragged = 0;
            sndStop(SOUND_GAME, 0.f);
        }
        else if (ABS(y - prevY) < .005f)
        {
            sndStop(SOUND_GAME, 0.f);
        }
        else
        {
            // play sound only if there's some change
            sndPlay("N_Slider_loop", SOUND_GAME);
        }
    }

    display_sprite_positional( knob, cx, y*yposSc, z+1, sc, sc, CLR_WHITE&alpha, H_ALIGN_CENTER, V_ALIGN_CENTER );

    // normalize slider value back to between -1 and 1
    val = -(y - cy/yposSc)/(ht/2);

    if(val<-1.0f) 
        val=-1.0f;
    if(val>1.0f) 
        val=1.0f;

    // set the value
    return val;
}

// returns display X value, sets *val
F32 handleSliderKnob(F32 x, F32 cy, F32 z, F32 sc, F32 wd, F32 *val, int type)
{
    AtlasTex * knob    = type < 0 ? atlasLoadTexture( "sliderbar_node_0" ) : atlasLoadTexture("sliderbar_node_round_0");
    F32 sliderX, prevSliderX;
    F32 scale;
    CBox box;
    int alpha = 0xffffffcc;
    static int dragged = -1;
    static F32 dragOffX;
    F32 xposSc, yposSc;
    calculatePositionScales(&xposSc, &yposSc);

    //now get the slider position
    x += knob->width*xposSc*sc/2;
    wd -= knob->width*sc;

    prevSliderX = sliderX = x + (((*val) + 1)/2)*wd*xposSc;

    scale = .8f*sc;
    // skip mouse collisions if slider set to not editable
    if (type != -1 )
    {
        F32 delta = 0.f;

        // handle mouse collisions
        BuildScaledCBox( &box, sliderX - (knob->width*xposSc*sc/2), cy - knob->height*yposSc*sc/2, knob->width*xposSc*sc, knob->height*yposSc*sc );

        if( mouseCollision(&box) || dragged == type  )
        {    
            scale = .85f*sc;
            alpha = 0xffffffff;
        }

        if( mouseDownHit( &box, MS_LEFT ) )
        {
            F32 mcx = gMouseInpCur.x;
            F32 mcy;

            if( shell_menu() && !is_scrn_scaling_disabled())
                calc_scaled_point( &mcx, &mcy );

            dragged = type;
            dragOffX = sliderX - mcx*xposSc;
        }
        
        if( dragged == type )
        {
            F32 mcx = gMouseInpCur.x;
            F32 mcy;

            if( shell_menu() && !is_scrn_scaling_disabled() )
            {
                calc_scaled_point( &mcx, &mcy );
            }

            sliderX = mcx*xposSc + dragOffX;
        }
    }

    // keep slider in bounds
    if( sliderX < x  )
        sliderX = x;
    else if( sliderX > x + wd*xposSc )
        sliderX = x + wd*xposSc;

    if  (type != -1 && dragged == type)
    {
        if( !isDown( MS_LEFT ) )
        { 
            //stop the sound the moment we quit dragging
            sndStop(SOUND_UI_ALTERNATE, 0.f);
            dragged = -1;
        }
        else if (ABS(sliderX - prevSliderX) < .005f)
        {
            sndStop(SOUND_UI_ALTERNATE, 0.f);
        }
        else
        {
            // play sound only if there's some change
            sndPlay("N_Slider_loop", SOUND_UI_ALTERNATE);
        }
    }

    display_sprite_positional( knob, sliderX, cy, z+2, scale, scale, (type < 0 ? CLR_H_BACK : CLR_WHITE) & alpha, H_ALIGN_CENTER, V_ALIGN_CENTER );


    // normalize slider value back to between -1 and 1
    *val = ((sliderX - x)/xposSc/wd * 2 )-1;
    
    if((*val)<-1.0f) 
        (*val)=-1.0f;
    if((*val)>1.0f) 
        (*val)=1.0f;

    return sliderX;
}

//common frame between single and triple slider
static void drawHybridSliderFrame( F32 x, F32 y, F32 z, F32 sc, F32 wd, F32 sliderWd, const char * text, int type)
{
    AtlasTex * left[3] = 
    {
        atlasLoadTexture("infosliderbar_tip_0"),
        atlasLoadTexture("infosliderbar_tip_1"),
        atlasLoadTexture("infosliderbar_tip_2")
    };
    AtlasTex * mid1[3] =
    {
        atlasLoadTexture("infosliderbar_mid1_0"),
        atlasLoadTexture("infosliderbar_mid1_1"),
        atlasLoadTexture("infosliderbar_mid1_2")
    };
    AtlasTex * mid2[3] =
    {
        atlasLoadTexture((type == -1) ? "infosliderbar_mid2_sharp_0" : "infosliderbar_mid2_round_0"),
        atlasLoadTexture((type == -1) ? "infosliderbar_mid2_sharp_1" : "infosliderbar_mid2_round_1"),
        atlasLoadTexture((type == -1) ? "infosliderbar_mid2_sharp_2" : "infosliderbar_mid2_round_2")
    };
    AtlasTex * mid3[3] =
    {
        atlasLoadTexture("infosliderbar_mid3_0"),
        atlasLoadTexture("infosliderbar_mid3_1"),
        atlasLoadTexture("infosliderbar_mid3_2")
    };
    AtlasTex * right[3] =
    {
        atlasLoadTexture("infosliderbar_end_0"),
        NULL,  //no white asset for this end of the frame
        atlasLoadTexture("infosliderbar_end_2")
    };
    int i;
    F32 cy, xposSc, yposSc;
    F32 textXSc = 1.f;
    F32 textYSc = 1.f;
    SpriteScalingMode ssm = getSpriteScaleMode();
    calculatePositionScales(&xposSc, &yposSc);
    cy = y + left[0]->height*yposSc*sc/2.f;

    devassert(wd > sliderWd);

    //draw frame
    for (i = 0; i < 3; ++i)
    {
        F32 textFrameWd = wd - sliderWd - left[i]->width*sc - mid2[i]->width*sc - right[0]->width*sc;
        F32 xwalk = x;
        display_sprite_positional(left[i], xwalk, y, z, sc, sc, HYBRID_FRAME_COLOR[i], H_ALIGN_LEFT, V_ALIGN_TOP );
        xwalk += left[i]->width*xposSc*sc;
        display_sprite_positional(mid1[i], xwalk, y, z, textFrameWd/mid1[i]->width, sc, HYBRID_FRAME_COLOR[i], H_ALIGN_LEFT, V_ALIGN_TOP );
        xwalk += textFrameWd*xposSc;
        display_sprite_positional(mid2[i], xwalk, y, z, sc, sc, HYBRID_FRAME_COLOR[i], H_ALIGN_LEFT, V_ALIGN_TOP );
        xwalk += mid2[i]->width*xposSc*sc;
        display_sprite_positional(mid3[i], xwalk, y, z, sliderWd/mid3[i]->width, sc, HYBRID_FRAME_COLOR[i], H_ALIGN_LEFT, V_ALIGN_TOP );
        xwalk += sliderWd*xposSc;
        if (i != 1)
        {
            display_sprite_positional(right[i], xwalk, y, z, sc, sc, HYBRID_FRAME_COLOR[i], H_ALIGN_LEFT, V_ALIGN_TOP );
        }
    }

    // print stat title
    if (isTextScalingRequired())
    {
        calculateSpriteScales(&textXSc, &textYSc);
        setSpriteScaleMode(SPRITE_XY_NO_SCALE);
    }
    font(&title_12);
    font_outl(0);
    font_color(CLR_BLACK&0xffffff99, CLR_BLACK&0xffffff99);
    prnt( x+10.f*xposSc+1.5f*sc, cy+5.f*yposSc*sc+1.5f*sc, z, textXSc*sc, textYSc*sc, text );
    font_color(CLR_WHITE, CLR_WHITE);
    prnt( x+10.f*xposSc, cy+5.f*yposSc*sc, z, textXSc*sc, textYSc*sc, text );
    font_outl(1);
    setSpriteScaleMode(ssm);
}

void drawHybridSliderEmpty( F32 x, F32 y, F32 z, F32 sc, F32 wd, F32 sliderWd, const char * text, int type)
{
    AtlasTex * bleft = atlasLoadTexture((type == -1) ? "infosliderbar_base_tip_sharp_0" : "infosliderbar_base_tip_round_0");
    AtlasTex * bright = atlasLoadTexture("infosliderbar_base_end_0");
    AtlasTex * bmid = atlasLoadTexture("infosliderbar_base_mid_0");
    F32 xposSc, yposSc;
    calculatePositionScales(&xposSc, &yposSc);

    display_sprite_positional(bleft, x + (wd - bright->width*sc - sliderWd)*xposSc, y, z, sc, sc, CLR_H_TAB_BACK&0xffffff7f, H_ALIGN_RIGHT, V_ALIGN_TOP );
    display_sprite_positional(bmid, x + (wd - bright->width*sc)*xposSc, y, z, sliderWd/bmid->width, sc, CLR_H_TAB_BACK&0xffffff7f, H_ALIGN_RIGHT, V_ALIGN_TOP );
    display_sprite_positional(bright, x + wd*xposSc, y, z, sc, sc, CLR_H_TAB_BACK&0xffffff7f, H_ALIGN_RIGHT, V_ALIGN_TOP );

    drawHybridSliderFrame(x, y, z, sc, wd, sliderWd, text, type);
}

// a type of -1 indicates a non-editable slider
// val: range of -1 to 1
F32 drawHybridSlider( F32 x, F32 y, F32 z, F32 sc, F32 wd, F32 sliderWd, F32 val, const char * text, int type, int number )
{
    AtlasTex * knob    = atlasLoadTexture( "sliderbar_node_0" );
    AtlasTex * bleft = atlasLoadTexture((type == -1) ? "infosliderbar_base_tip_sharp_0" : "infosliderbar_base_tip_round_0");
    AtlasTex * bright = atlasLoadTexture("infosliderbar_base_end_0");
    AtlasTex * bmid = atlasLoadTexture("infosliderbar_base_mid_0");

    int alpha = 0xcc;

     F32 sliderX, cy;
    static int dragOffX, dragged = -1;

    F32 knobBlendAmount = 0.2f;
    int leftColor = 0xfc0e01ff, rightColor = 0x64c801ff, backColor = 0x657cd400;
    F32 xposSc, yposSc;
    F32 textXSc = 1.f;
    F32 textYSc = 1.f;
    SpriteScalingMode ssm = getSpriteScaleMode();
    calculatePositionScales(&xposSc, &yposSc);

    //slider bar gradient
    display_sprite_positional(bleft, x + (wd - bright->width*sc - sliderWd)*xposSc, y, z, sc, sc, leftColor, H_ALIGN_RIGHT, V_ALIGN_TOP );
    display_sprite_positional_blend(bmid, x + (wd - bright->width*sc)*xposSc, y, z, sliderWd/bmid->width, sc, leftColor, rightColor, rightColor, leftColor, H_ALIGN_RIGHT, V_ALIGN_TOP );
    display_sprite_positional(bright, x + wd*xposSc, y, z, sc, sc, backColor | 0xff, H_ALIGN_RIGHT, V_ALIGN_TOP );

    //draw knob
    //now get the slider position
    cy = y + bleft->height*yposSc*sc/2.f;
    sliderX = handleSliderKnob( x + (wd - bright->width*sc - sliderWd)*xposSc, cy, z+1.f, sc, sliderWd, &val, type );

    //gradient adjustment
    //replace background to the right of the knob
    display_sprite_positional( bmid, sliderX, y, z, (x/xposSc + wd - bright->width*sc - sliderX/xposSc)/bmid->width, sc, backColor | 0xFF, H_ALIGN_LEFT, V_ALIGN_TOP );
    //blend background to the left of the knob
    display_sprite_positional_blend( bmid, max(x + (wd - bright->width*sc - sliderWd)*xposSc, sliderX - (sliderWd * knobBlendAmount)*xposSc), y, z, (sliderWd * knobBlendAmount)/bmid->width, sc, backColor | 0x00, backColor | 0xFF, backColor | 0xFF, backColor | 0x00, H_ALIGN_LEFT, V_ALIGN_TOP );

    //draw frame & print stat title
    drawHybridSliderFrame(x, y, z, sc, wd, sliderWd, text, type);

    //save
    if (isTextScalingRequired())
    {
        calculateSpriteScales(&textXSc, &textYSc);
        setSpriteScaleMode(SPRITE_XY_NO_SCALE);
    }
    font(&game_12);
    font_color(CLR_WHITE, CLR_WHITE);
    font_outl(0);
    if (number=='?')  //special case
    {
        cprntEx( sliderX, cy, z+3, textXSc, textYSc, CENTER_X|CENTER_Y, "?" );
    }
    else if (number>=0)
    {
        //print number on slider
        cprntEx( sliderX, cy, z+3, textXSc, textYSc, CENTER_X|CENTER_Y, "%i", number );
    }
    font_outl(1);
    setSpriteScaleMode(ssm);

    // set the value
    return val;
}


void drawHybridTripleSlider( F32 x, F32 y, F32 z, F32 sc, F32 wd, F32 sliderWd, Vec3 val, const char * text, int type )
{
    AtlasTex * knob    = atlasLoadTexture( "sliderbar_node_0" );
    AtlasTex * bleft = atlasLoadTexture((type == -1) ? "infosliderbar_base_tip_sharp_0" : "infosliderbar_base_tip_round_0");
    AtlasTex * bright = atlasLoadTexture("infosliderbar_base_end_0");
    AtlasTex * bmid = atlasLoadTexture("infosliderbar_base_mid_0");

    F32 sectionWd = (sliderWd - 2.f)/3.f;
    int i, colorL = 0xa01501ff, colorM = 0x567fa8ff, colorR = 0x84a12aff;
    F32 xwalk, cy, xposSc, yposSc;
    calculatePositionScales(&xposSc, &yposSc);
    cy = y + bleft->height*yposSc*sc/2.f;
    xwalk = x + (wd - bright->width*sc - sliderWd - bleft->width*sc)*xposSc;

    // draw the background
    display_sprite_positional(bleft, xwalk, y, z, sc, sc, colorL, H_ALIGN_LEFT, V_ALIGN_TOP );
    xwalk += bleft->width*xposSc*sc;
    display_sprite_positional(bmid, xwalk, y, z, sectionWd/bmid->width, sc, colorL, H_ALIGN_LEFT, V_ALIGN_TOP );
    xwalk += sectionWd*xposSc;
    display_sprite_positional(bmid, xwalk, y, z, 1.f/bmid->width, sc, CLR_WHITE, H_ALIGN_LEFT, V_ALIGN_TOP );
    xwalk += xposSc;
    display_sprite_positional(bmid, xwalk, y, z, sectionWd/bmid->width, sc, colorM, H_ALIGN_LEFT, V_ALIGN_TOP );
    xwalk += sectionWd*xposSc;
    display_sprite_positional(bmid, xwalk, y, z, 1.f/bmid->width, sc, CLR_WHITE, H_ALIGN_LEFT, V_ALIGN_TOP );
    xwalk += xposSc;
    display_sprite_positional(bmid, xwalk, y, z, sectionWd/bmid->width, sc, colorR, H_ALIGN_LEFT, V_ALIGN_TOP );
    display_sprite_positional(bright, x + (wd - bright->width*sc)*xposSc, y, z, sc, sc, colorR, H_ALIGN_LEFT, V_ALIGN_TOP );

    //draw frame & print stat title
    drawHybridSliderFrame(x, y, z, sc, wd, sliderWd, text, type);

    // now deal with the actual selectors
    xwalk = x + (wd - bright->width*sc - sliderWd)*xposSc;
    for( i = 0; i < 3; i++ )
    {
        handleSliderKnob( xwalk, cy, z+1.f, sc, sectionWd, &val[i], type+i );
        xwalk += (sectionWd + 1.f)*xposSc;
    }
}

void drawGradientEdgedBox( F32 x, F32 y , F32 z, F32 wd, F32 ht, F32 depth, int color )
{
    AtlasTex * w = white_tex_atlas;

    // UL
    display_sprite_blend( w, x, y, z, depth/w->width, depth/w->height, color&0xffffff00, color&0xffffff00, color, color&0xffffff00 );
    
    // Top
    display_sprite_blend( w, x+depth, y, z, (wd-2*depth)/w->width, depth/w->height, color&0xffffff00, color&0xffffff00, color, color );

    // UR
    display_sprite_blend( w, x + wd - depth, y, z, depth/w->width, depth/w->height, color&0xffffff00, color&0xffffff00, color&0xffffff00, color );

    // Left
    display_sprite_blend( w, x, y + depth, z, depth/w->width, (ht - 2*depth)/w->height, color&0xffffff00, color, color, color&0xffffff00 );

    // M
    display_sprite( w, x + depth, y + depth, z, (wd-2*depth)/w->width, (ht - 2*depth)/w->height, color );

    // Right
    display_sprite_blend( w, x + wd - depth, y + depth, z, depth/w->width, (ht - 2*depth)/w->height, color, color&0xffffff00, color&0xffffff00, color );

    // LL
    display_sprite_blend( w, x, y + ht - depth, z, depth/w->width, depth/w->height, color&0xffffff00, color, color&0xffffff00, color&0xffffff00 );

    // BOT
    display_sprite_blend( w, x+depth, y + ht - depth, z, (wd-2*depth)/w->width, depth/w->height, color, color, color&0xffffff00, color&0xffffff00 );

    // LR
    display_sprite_blend( w, x + wd - depth, y + ht - depth, z, depth/w->width, depth/w->height, color, color&0xffffff00, color&0xffffff00, color&0xffffff00 );

}

int drawHybridArrow( F32 cx, F32 cy, F32 z, F32 sc, int flip )
{
    AtlasTex * arrow = atlasLoadTexture( "icon_browsearrow_0" );
    CBox box;
    int clr = 0xffffffff88;

       BuildCBox(&box, cx - arrow->width*sc/2, cy - arrow->height*sc/2, arrow->width*sc, arrow->height*sc);
    //drawBox(&box, 5000, CLR_RED, 0);
    if( mouseCollision(&box))
    {
        clr = CLR_WHITE;
        if( isDown(MS_LEFT) )
            sc *= .95f;
    }

       display_rotated_sprite( arrow, cx - arrow->width*sc/2, cy - arrow->height*sc/2, z, sc, sc, clr, flip?PI/2:-PI/2, 0 );

    return mouseClickHit(&box, MS_LEFT);
}

void drawHybridFade( F32 cx, F32 cy, F32 z, F32 wd, F32 ht, F32 mid, int color )
{
    AtlasTex * w = white_tex_atlas;
    F32 xposSc, yposSc;
    calculatePositionScales(&xposSc, &yposSc);
    
    display_sprite_positional_blend(w, cx - wd*xposSc/2, cy, z, ((wd-mid)/2)/w->width, ht/w->height, color&0xffffff00, color, color, color&0xffffff00, H_ALIGN_LEFT, V_ALIGN_CENTER );
      display_sprite_positional(w, cx, cy, z, mid/w->width, ht/w->height, color, H_ALIGN_CENTER, V_ALIGN_CENTER );
    display_sprite_positional_blend(w, cx + wd*xposSc/2, cy, z, ((wd-mid)/2)/w->width, ht/w->height, color, color&0xffffff00, color&0xffffff00, color, H_ALIGN_RIGHT, V_ALIGN_CENTER );
}

static void drawHybridButtonBacking(F32 cx, F32 cy, F32 z, F32 sc, int alpha, int gray)
{
    AtlasTex *tex0 = atlasLoadTexture("circulartab_0.tga");
    AtlasTex *tex1 = atlasLoadTexture("circulartab_1.tga");
    AtlasTex *tex2 = atlasLoadTexture("circulartab_2.tga");
    
    int color0 = gray ? 0x444444ff : CLR_H_DARK_BACK;
    int color1 = gray ? 0x585858ff : CLR_H_BRIGHT_BACK;
    int color2 = gray ? 0x878787ff : CLR_H_BACK;
    display_sprite_positional(tex0, cx, cy, z, sc, sc, color0&alpha, H_ALIGN_CENTER, V_ALIGN_CENTER );   // blue background
    display_sprite_positional(tex1, cx, cy, z, sc, sc, color1&alpha, H_ALIGN_CENTER, V_ALIGN_CENTER);      // shine
    display_sprite_positional(tex2, cx, cy, z, sc, sc, color2&alpha, H_ALIGN_CENTER, V_ALIGN_CENTER);      // ring
}

static int was_tooltip_set;

void resetHybridToolTipAwareness()
{
    was_tooltip_set = 0;
}

void clearHybridToolTipIfUnused()
{
    if (!was_tooltip_set)
    {
        removeToolTip(&hybridToolTip);
    }
}

int drawHybridButtonWindow( HybridElement * hb, F32 cx, F32 cy, F32 z, F32 sc, int color, int flags, int window )
{
    F32 wd, ht;
    CBox box;
    int ret = 0;
    int alpha = 0xffffff00 | (int)((color&0xff)*0.9f);
    AtlasTex * backing = atlasLoadTexture("circulartab_0.tga");
    F32 originalsc = sc;
    F32 xposSc, yposSc;

    calculatePositionScales(&xposSc, &yposSc);
    if (hb->iconName0)
    {
        //try to get a texture
        hb->icon0 = atlasLoadTexture(hb->iconName0);

        if( hb->icon0 == white_tex_atlas )
        {
            //try again with _0
            char * textbuff = _alloca( (strlen(hb->iconName0) + 5) * sizeof(char) );
            sprintf( textbuff, "%s_0", hb->iconName0 );
            hb->icon0 = atlasLoadTexture(textbuff);
            if (hb->icon0 == white_tex_atlas)
            {
                //no valid texture, return without drawing
                hb->icon0 = NULL;
                return 0;
            }
        }
        wd = hb->icon0->width;
        ht = hb->icon0->height;
    }

    if( flags & HB_LIGHT_UP)
    {
        alpha = 0xffffffaa;
    }

    if (flags & HB_DRAW_BACKING)
    {
        BuildScaledCBox(&box, cx - backing->width*xposSc*sc/2, cy - backing->height*yposSc*sc/2, backing->width*xposSc*sc, backing->height*yposSc*sc );
    }
    else
    {
        BuildScaledCBox(&box, cx - wd*xposSc*sc/2, cy - ht*yposSc*sc/2, wd*xposSc*sc, ht*yposSc*sc );
    }
    if( mouseCollision(&box) && !(flags&HB_DISABLED) )
    {
        if (flags & HB_DO_NOT_EDIT_ELEMENT)
        {
            hb->percent = 1.f;
        }
        else
        {
            if (hb->percent == 0.f)
            {
                sndPlay("N_PopHover", SOUND_GAME);
            }
            hb->percent = stateScale(hb->percent, 1);
        }
        if (flags & HB_LIGHT_UP)
        {
            alpha = 0xffffffff;
        }
        else if (flags & HB_SHRINK_OVER)
        {
            sc *= .9f;
        }
        else
        {
            sc *= 1.05f;
        }
        ret = D_MOUSEOVER;

        if( isDown(MS_LEFT) )
        {
            sc *= .95f;
            ret = D_MOUSEDOWN;
        }
        if(!(flags&HB_DISABLED) && mouseClickHit(&box, MS_LEFT))
        {
            ret = D_MOUSEHIT;
            sndPlay("N_Select", SOUND_GAME);
        }
    }
    else
    {
        if (flags & HB_DO_NOT_EDIT_ELEMENT)
        {
            hb->percent = 0.f;
        }
        else
        {
            hb->percent = stateScale(hb->percent, 0);
        }
    }
    if( (flags&HB_DISABLED) && !(flags&HB_DRAW_NORMAL_ICON))
        color = 0x66666680;

    if (flags&HB_DRAW_BACKING)
    {
        drawHybridButtonBacking(cx, cy, z, sc, alpha, flags&HB_DISABLED);
    }

    sc *= 0.9;
    if( hb->icon0 != white_tex_atlas )
    {
        display_sprite_positional_flip(hb->icon0, cx, cy, z, sc, sc, color&alpha, (flags&HB_FLIP_H)?FLIP_HORIZONTAL:0, H_ALIGN_CENTER, V_ALIGN_CENTER );
    }

    if( hb->desc1 )
    {
        CBox cbox;
        BuildScaledCBox(&cbox, cx - wd*xposSc*sc/2, cy-ht*yposSc*sc/2, wd*xposSc*sc, ht*yposSc*sc);
        if (mouseCollision(&cbox))
        {
            if (window)
            {
                setHybridTooltipWindow(&box, hb->desc1, window);
            } else {
                if (current_menu() == MENU_COMBINE_SPEC || current_menu() == MENU_RESPEC)
                {
                    addToolTip( &hybridToolTip ); 
                    forceDisplayToolTip(&hybridToolTip, &box, hb->desc1, current_menu(), 0);
                    was_tooltip_set = 1;
                } else {
                    setHybridTooltip(&box, hb->desc1);
                }
            }
        }
    }

    if( hb->text )
    {
        F32 ty = 0.f;
        F32 offset = 7.f;
        F32 textXSc = 1.f;
        F32 textYSc = 1.f;
        SpriteScalingMode ssm = getSpriteScaleMode();
        
        if (isTextScalingRequired())
        {
            calculateSpriteScales(&textXSc, &textYSc);
            setSpriteScaleMode(SPRITE_XY_NO_SCALE);
        }
        if ( flags & HB_DRAW_BACKING )
        {
            //draw text below the backing
            ty = cy + (backing->height*originalsc/2.f + offset)*yposSc;
        }
        else if( hb->icon0 != white_tex_atlas )
        {
            //draw text below the icon
            ty = cy + (hb->icon0->height*originalsc/2.f + offset)*yposSc;
        }
        font(&hybridbold_12);
        font_color(CLR_WHITE, CLR_WHITE);
        cprntEx(cx, ty, z, originalsc*textXSc, originalsc*textYSc, hb->text_flags | CENTER_X, hb->text);
        setSpriteScaleMode(ssm);
    }

    return ret;
}

int drawHybridButton( HybridElement * hb, F32 cx, F32 cy, F32 z, F32 sc, int color, int flags )
{
    return drawHybridButtonWindow(hb, cx, cy, z, sc, color, flags, 0 );
}

int drawHybridBarButton( HybridElement * hb, F32 cx, F32 cy, F32 z, F32 wd, F32 sc, int buttonColor, int flags )
{
    static SMFBlock *s_HybridBarButtonSMF = 0;
    AtlasTex * tex = 0;
    AtlasTex * button[3][3];
    int buttonColors[3];
    int i;
    F32 midWd;
    F32 xwalk, ywalk, wdwalk;
    int alpha = 0xffffffff;
    CBox box;
    int ret = D_NONE;
    int iFinal;
    int flipFlag = 0;
    TextAttribs *smfFont;
    F32 xposSc, yposSc;
    F32 textXSc = 1.f;
    F32 textYSc = 1.f;
    SpriteScalingMode ssm = getSpriteScaleMode();
    calculatePositionScales(&xposSc, &yposSc);
    if (isTextScalingRequired())
    {
        calculateSpriteScales(&textXSc, &textYSc);
    }

    if (!s_HybridBarButtonSMF)
    {
        s_HybridBarButtonSMF = smfBlock_Create();
        smf_SetFlags(s_HybridBarButtonSMF, SMFEditMode_Unselectable, SMFLineBreakMode_SingleLine, 
            SMFInputMode_AnyTextNoTagsOrCodes, 0, SMFScrollMode_ExternalOnly,
            SMFOutputMode_StripAllTagsAndCodes, SMFDisplayMode_AllCharacters, SMFContextMenuMode_None,
            SMAlignment_Center, 0, 0, 0);
    }

    if (hb->iconName0)
    {
        hb->icon0 = atlasLoadTexture(hb->iconName0);
    }
    else
    {
        hb->icon0 = NULL;
    }

    if (flags & HYBRID_BAR_BUTTON_ARROW_LEFT)
    {
        button[0][0] = atlasLoadTexture("arrowtab_tip_0");
        button[0][1] = atlasLoadTexture("arrowtab_tip_1");
        button[0][2] = atlasLoadTexture("arrowtab_tip_2");
        button[1][0] = atlasLoadTexture("arrowtab_mid_0");
        button[1][1] = atlasLoadTexture("arrowtab_mid_1");
        button[1][2] = atlasLoadTexture("arrowtab_mid_2");
        button[2][0] = atlasLoadTexture("arrowtab_end_0");
        button[2][1] = atlasLoadTexture("arrowtab_end_1");
        button[2][2] = atlasLoadTexture("arrowtab_end_2");
    }
    else if (flags & HYBRID_BAR_BUTTON_ARROW_RIGHT)
    {
        button[0][0] = atlasLoadTexture("arrowtab_end_0");
        button[0][1] = atlasLoadTexture("arrowtab_end_1");
        button[0][2] = atlasLoadTexture("arrowtab_end_2");
        button[1][0] = atlasLoadTexture("arrowtab_mid_0");
        button[1][1] = atlasLoadTexture("arrowtab_mid_1");
        button[1][2] = atlasLoadTexture("arrowtab_mid_2");
        button[2][0] = atlasLoadTexture("arrowtab_tip_0");
        button[2][1] = atlasLoadTexture("arrowtab_tip_1");
        button[2][2] = atlasLoadTexture("arrowtab_tip_2");
        flipFlag = FLIP_HORIZONTAL;
    }
    else
    {
        button[0][0] = atlasLoadTexture("roundlongtab_tip_0");
        button[0][1] = atlasLoadTexture("roundlongtab_tip_1");
        button[0][2] = atlasLoadTexture("roundlongtab_tip_2");
        button[1][0] = atlasLoadTexture("roundlongtab_mid_0");
        button[1][1] = atlasLoadTexture("roundlongtab_mid_1");
        button[1][2] = atlasLoadTexture("roundlongtab_mid_2");
        button[2][0] = atlasLoadTexture("roundlongtab_end_0");
        button[2][1] = atlasLoadTexture("roundlongtab_end_1");
        button[2][2] = atlasLoadTexture("roundlongtab_end_2");
    }

    if (flags & HYBRID_BAR_BUTTON_DISABLED)
    {
        buttonColors[0] = 0x666666ff;
        buttonColors[1] = 0xffffff66;
        buttonColors[2] = 0xffffff66;
    }
    else if (flags & (HYBRID_BAR_BUTTON_ARROW_LEFT | HYBRID_BAR_BUTTON_ARROW_RIGHT))
    {
        buttonColors[0] = HYBRID_FRAME_COLOR[0];
        buttonColors[1] = HYBRID_FRAME_COLOR[1];
        buttonColors[2] = CLR_H_HIGHLIGHT;
    }
    else if (buttonColor)
    {
        buttonColors[0] = buttonColor;
        buttonColors[1] = 0xffffff66;
        buttonColors[2] = 0xffffff66;
    }
    else
    {
        buttonColors[0] = HYBRID_FRAME_COLOR[0];
        buttonColors[1] = HYBRID_FRAME_COLOR[1];
        buttonColors[2] = HYBRID_FRAME_COLOR[2];
    }

    BuildScaledCBox(&box, cx - wd*xposSc*sc/2, cy - button[0][0]->height*yposSc*sc/2.f, wd*xposSc*sc, button[0][0]->height*yposSc*sc );
    if( mouseCollision(&box))
    {
        if (flags & HYBRID_BAR_DO_NOT_EDIT_ELEMENT)
        {
            hb->percent = 1.f;
        }
        else
        {
            if (hb->percent == 0.f)
            {
                sndPlay("N_PopHover", SOUND_GAME);
            }
            hb->percent = stateScale(hb->percent, 1);
        }
        if (!(flags & HYBRID_BAR_BUTTON_DISABLED))
        {
            sc *= 1.05f;
            ret = D_MOUSEOVER;

            if( isDown(MS_LEFT) )
            {
                sc *= .95f;
                ret = D_MOUSEDOWN;
            }
            if(mouseClickHit(&box, MS_LEFT))
            {
                sndPlay("N_Select", SOUND_GAME);
                ret = D_MOUSEHIT;
            }
        }
        if (hb->desc1)
        {
            setHybridTooltip(&box, hb->desc1);
        }
    }
    else
    {
        if (flags & HYBRID_BAR_DO_NOT_EDIT_ELEMENT)
        {
            hb->percent = 0.f;
        }
        else
        {
            hb->percent = stateScale(hb->percent, 0);
        }
    }

    midWd = (wd - button[0][0]->width - button[2][0]->width)*sc;
    ywalk = cy;

    iFinal = (flags & HYBRID_BAR_BUTTON_NO_OUTLINE) ? 2 : 3;

    for (i = 0; i < iFinal; ++i)
    {
        SpriteScalingMode ssm = getSpriteScaleMode();
        xwalk = cx - midWd*xposSc/2.f;
        display_sprite_positional_flip( button[0][i], xwalk, ywalk, z, sc, sc, buttonColors[i]&alpha, flipFlag, H_ALIGN_RIGHT, V_ALIGN_CENTER );
        xwalk = cx;
        if (ssm != SPRITE_XY_NO_SCALE)
        {
            setSpriteScaleMode(SPRITE_XY_SCALE);
        }
        display_sprite_positional( button[1][i], xwalk, ywalk, z, midWd/button[1][i]->width, sc, buttonColors[i]&alpha, H_ALIGN_CENTER, V_ALIGN_CENTER );
        setSpriteScaleMode(ssm);
        xwalk = cx + midWd*xposSc/2.f;;
        display_sprite_positional_flip( button[2][i], xwalk, ywalk, z, sc, sc, buttonColors[i]&alpha, flipFlag, H_ALIGN_LEFT, V_ALIGN_CENTER );
    }

    smfFont = &gTextAttr_WhiteHybridBold18;
    gTextAttr_WhiteHybridBold18.piScale = (int *)(int)(textYSc*sc*SMF_FONT_SCALE);
    if (tex)
    {
        //draw icon
        xwalk = cx - wd*xposSc*sc/2.f + button[0][0]->width*xposSc*sc/2.f;
        display_sprite_positional( tex, xwalk, ywalk, z, sc, sc, CLR_WHITE&alpha, H_ALIGN_LEFT, V_ALIGN_TOP );
        xwalk = cx + (tex->width-wd*sc)*xposSc/2.0f;
        wdwalk = wd*sc - tex->width/2.0f;
        if (isTextScalingRequired())
        {
            setSpriteScaleMode(SPRITE_XY_NO_SCALE);
        }
        ywalk = cy - smf_ParseAndFormat(s_HybridBarButtonSMF, 0, xwalk, cy, z + 1, wdwalk, 0, 0, 0, 0, smfFont, 0)*yposSc/2.f;
    }
    else
    {
        xwalk = cx - wd*sc/2.0f;
        wdwalk = wd*sc;
        if (isTextScalingRequired())
        {
            setSpriteScaleMode(SPRITE_XY_NO_SCALE);
        }
        ywalk = cy - smf_ParseAndFormat(s_HybridBarButtonSMF, 0, xwalk, cy, z + 1, wdwalk, 0, 0, 0, 0, smfFont, 0)*yposSc/2.f;
    }

    if (hb->text_flags&NO_MSPRINT)
        smf_SetRawText(s_HybridBarButtonSMF, hb->text, 0);
    else
        smf_SetRawText(s_HybridBarButtonSMF, textStd(hb->text), 0);

    smf_Display(s_HybridBarButtonSMF, xwalk, ywalk, z + 1, wdwalk, 0, 0, 0, smfFont, 0);

    //revert font changes
    gTextAttr_WhiteHybridBold18.piScale = (int *)(int)(1.f*SMF_FONT_SCALE);
    setSpriteScaleMode(ssm);

    return ret;
}

static void openStoreWindow(const char * item, int flags)
{
    if (!item)
    {
        return;
    }
    else if (flags & HB_STORE_CATEGORY)
    {
        webStoreOpenCategory(item);
    }
    else
    {
        webStoreOpenProduct(item);
    }
}

static void openStoreAddToCartWindow( const ShoppingCart * cart, U32 first, U32 last, int flags)
{
    webStoreAddMultipleToCart(cart, first, last);
}

static int drawHybridStoreCommon( const char * product, HybridElement * storeAccess, HybridElement * storeSlow, HybridElement * storeDown, F32 cx, F32 cy, F32 z, F32 sc, int color, int storeFlags, int buttonFlags, int window )
{
    int ret;
    AccountServerStatus status = inventoryClient_GetAcctAuthoritativeState();

    switch (status)
    {
    case ACCOUNT_SERVER_UP:
        ret = drawHybridButtonWindow(storeAccess, cx, cy, z, sc, color, buttonFlags, window);
        if (D_MOUSEHIT == ret)
        {
            openStoreWindow(product, storeFlags);
        }
        break;
    case ACCOUNT_SERVER_SLOW:
        ret = drawHybridButtonWindow(storeSlow, cx, cy, z, sc, color, buttonFlags|HB_DISABLED|HB_DRAW_NORMAL_ICON, window);
        break;
    case ACCOUNT_SERVER_DOWN:
        ret = drawHybridButtonWindow(storeDown, cx, cy, z, sc, color, buttonFlags|HB_DISABLED|HB_DRAW_NORMAL_ICON, window);
        break;
    }
    return ret;
}

int drawHybridStoreSmallButton( char * product, F32 cx, F32 cy, F32 z, F32 sc, int color, int storeFlags, int window )
{
    int ret;
    AccountServerStatus status = inventoryClient_GetAcctAuthoritativeState();
    AtlasTex * storeLinkTex = atlasLoadTexture("icon_ig_storeaccess_small");
    const char * storeTooltip = (product == NULL || storeFlags & HB_STORE_CATEGORY) ? "paragonstoreButton" : "paragonstoreitemButton";
    static HybridElement sButtonStoreAccess = {0, NULL, NULL, "icon_ig_storeaccess_small"};
    static HybridElement sButtonStoreSlow = {0, NULL, "paragonstoreSlowButton", "icon_ig_serverslow_small"};
    static HybridElement sButtonStoreDown = {0, NULL, "paragonstoreDownButton", "icon_ig_serverdown_small"};
    sButtonStoreAccess.desc1 = storeTooltip;

    ret = drawHybridStoreCommon( product, &sButtonStoreAccess, &sButtonStoreSlow, &sButtonStoreDown, cx, cy, z, sc, color, storeFlags, HB_LIGHT_UP, window );

    font(&game_12);
    font_color(CLR_WHITE, CLR_WHITE);
    cprnt(cx+storeLinkTex->width*sc*0.1f, cy+storeLinkTex->height/6.f, z+1.f, sc, sc, "ShopString");
    return ret;
}

int drawHybridStoreLargeButton( char * product, F32 cx, F32 cy, F32 z, F32 sc, int color, int storeFlags, int window )
{
    AccountServerStatus status = inventoryClient_GetAcctAuthoritativeState();
    const char * storeTooltip = (product == NULL || storeFlags & HB_STORE_CATEGORY) ? "paragonstoreButton" : "paragonstoreitemButton";
    static HybridElement sButtonStoreAccess = {0, NULL, NULL, "icon_ig_storeaccess"};
    static HybridElement sButtonStoreSlow = {0, NULL, "paragonstoreSlowButton", "icon_ig_serverslow"};
    static HybridElement sButtonStoreDown = {0, NULL, "paragonstoreDownButton", "icon_ig_serverdown"};
    sButtonStoreAccess.desc1 = storeTooltip;

    return drawHybridStoreCommon( product, &sButtonStoreAccess, &sButtonStoreSlow, &sButtonStoreDown, cx, cy, z, sc, color, storeFlags, 0, window );
}

int drawHybridStoreMenuButton( const char * product, F32 cx, F32 cy, F32 z, F32 sc, int color, int storeFlags )
{
    int ret;
    const char * storeTooltip = (product == NULL || storeFlags & HB_STORE_CATEGORY) ? "paragonstoreButton" : "paragonstoreitemButton";
    static HybridElement sButtonStoreAccess = {0, NULL, NULL, "icon_paragonstore"};
    static HybridElement sButtonStoreSlow = {0, NULL, "paragonstoreSlowButton", "icon_star_slow"};
    static HybridElement sButtonStoreDown = {0, NULL, "paragonstoreDownButton", "icon_star_down"};
    sButtonStoreAccess.desc1 = storeTooltip;

    ret = drawHybridStoreCommon( product, &sButtonStoreAccess, &sButtonStoreSlow, &sButtonStoreDown, cx, cy, z, sc, color, storeFlags, HB_DRAW_BACKING|HB_DO_NOT_EDIT_ELEMENT, 0);
    if (D_MOUSEHIT == ret)
    {
        collisions_off_for_rest_of_frame = true;
    }
    return ret;
}

int drawHybridStoreWindowButton( const char * product, const char * storeTex, F32 cx, F32 cy, F32 z, F32 sc, int color, int storeFlags, int buttonFlags, const char * tooltip, int window )
{
    static HybridElement sButtonStoreAccess = {0};
    static HybridElement sButtonStoreSlow = {0, NULL, "paragonstoreSlowButton", "icon_star_slow"};
    static HybridElement sButtonStoreDown = {0, NULL, "paragonstoreDownButton", "icon_star_down"};
    sButtonStoreAccess.desc1 = tooltip;
    sButtonStoreAccess.iconName0 = storeTex;

    return drawHybridStoreCommon( product, &sButtonStoreAccess, &sButtonStoreSlow, &sButtonStoreDown, cx, cy, z, sc, color, storeFlags, buttonFlags, window );
}

int drawHybridStoreAddShoppingCartMenuButton ( const ShoppingCart * cart, U32 first, U32 last, F32 cx, F32 cy, F32 z, F32 sc, int color, int flags)
{
    int ret;
    AccountServerStatus status = inventoryClient_GetAcctAuthoritativeState();
    static HybridElement sButtonStoreAccess = {0, NULL, NULL, "icon_paragonstore"};
    static HybridElement sButtonStoreSlow = {0, NULL, "paragonstoreSlowButton", "icon_star_slow"};
    static HybridElement sButtonStoreDown = {0, NULL, "paragonstoreDownButton", "icon_star_down"};
    sButtonStoreAccess.desc1 = flags & HB_STORE_CATEGORY ? "paragonstoreAddCategoryButton" : "paragonstoreAddAllButton";

    switch (status)
    {
    case ACCOUNT_SERVER_UP:
        ret = drawHybridButton(&sButtonStoreAccess, cx, cy, z, sc, color, HB_DRAW_BACKING | HB_DO_NOT_EDIT_ELEMENT);
        if (D_MOUSEHIT == ret)
        {
            openStoreAddToCartWindow(cart, first, last, flags);
            collisions_off_for_rest_of_frame = true;
        }
        break;
    case ACCOUNT_SERVER_SLOW:
        ret = drawHybridButton(&sButtonStoreSlow, cx, cy, z, sc, color, HB_DRAW_BACKING|HB_DISABLED|HB_DRAW_NORMAL_ICON ); 
        break;
    case ACCOUNT_SERVER_DOWN:
        ret = drawHybridButton(&sButtonStoreDown, cx, cy, z, sc, color, HB_DRAW_BACKING|HB_DISABLED|HB_DRAW_NORMAL_ICON ); 
        break;
    }
    return ret;
}

static void drawHybridStoreLockIconEx ( F32 cx, F32 cy, F32 z, F32 sc, int color, const char *toolTip )
{
    AtlasTex * back[] = 
    {
        atlasLoadTexture("circulartab_0"), 
        atlasLoadTexture("circulartab_1"), 
        atlasLoadTexture("circulartab_2")
    };
    AtlasTex * lock = atlasLoadTexture("icon_character_lock_0");
    int alpha = 0xffffff00 | color;
    int backColor[] = {    0xaa3b19ff&alpha, 0xcc8975ff&alpha, 0xf95809ff&alpha };
    int i;
    CBox box;
    F32 xposSc, yposSc;
    calculatePositionScales(&xposSc, &yposSc);

    BuildScaledCBox(&box, cx-back[0]->width*sc/2.f, cy-back[0]->height*sc/2.f, back[0]->width*xposSc*sc, back[0]->height*yposSc*sc);
    if (mouseCollision(&box))
    {
        if (!toolTip)
        {
            toolTip = "ItemStoreLockedString";
        }
        setHybridTooltip(&box, toolTip);
    }
    for (i = 0; i < 3; ++i)
    {
        display_sprite_positional(back[i], cx, cy, z, sc, sc, backColor[i], H_ALIGN_CENTER, V_ALIGN_CENTER );
    }
    display_sprite_positional(lock, cx, cy, z, sc, sc, color, H_ALIGN_CENTER, V_ALIGN_CENTER );
}

void drawHybridStoreLockIcon( F32 cx, F32 cy, F32 z, F32 sc, int color )
{
    drawHybridStoreLockIconEx ( cx, cy, z, sc, color, 0 );
}

void drawHybridStoreLockIconCustomTooltip( F32 cx, F32 cy, F32 z, F32 sc, int color, const char * tooltip )
{
    drawHybridStoreLockIconEx ( cx, cy, z, sc, color, tooltip );
}

//these match with the current popup frame assets
const F32 HBPOPUPFRAME_TOP_OFFSET = 43.f;
const F32 HBPOPUPFRAME_BOTTOM_OFFSET = 14.f;
const F32 HBPOPUPFRAME_LEFT_OFFSET = 7.f;
const F32 HBPOPUPFRAME_RIGHT_OFFSET = 14.f;

void drawHybridPopupFrame(F32 x, F32 y, F32 z, F32 wd, F32 ht)
{
    AtlasTex * tlcorner[3] = 
    {
        atlasLoadTexture("globalframe_corner1_0"),
        atlasLoadTexture("globalframe_corner1_1"),
        atlasLoadTexture("globalframe_corner1_2")
    };
    AtlasTex * trcorner[3] = 
    {
        atlasLoadTexture("globalframe_corner2_0"),
        atlasLoadTexture("globalframe_corner2_1"),
        atlasLoadTexture("globalframe_corner2_2")
    };
    AtlasTex * thoriz[3] = 
    {
        atlasLoadTexture("globalframe_corner1_horizontal_0"),
        atlasLoadTexture("globalframe_corner1_horizontal_1"),
        atlasLoadTexture("globalframe_corner1_horizontal_2")
    };
    AtlasTex * blcorner = atlasLoadTexture("globalframe_corner3_0");
    AtlasTex * brcorner = atlasLoadTexture("globalframe_corner4_0");
    AtlasTex * bhoriz = atlasLoadTexture("globalframe_corner3_horizontal_0");
    AtlasTex * lvert = atlasLoadTexture("globalframe_corner1_vertical_0");
    AtlasTex * rvert = atlasLoadTexture("globalframe_corner2_vertical_0");

    AtlasTex * tlbasecorner = atlasLoadTexture("globalframe_corner1_base_0");
    AtlasTex * trbasecorner = atlasLoadTexture("globalframe_corner2_base_0");
    AtlasTex * blbasecorner = atlasLoadTexture("globalframe_corner3_base_0");
    AtlasTex * brbasecorner = atlasLoadTexture("globalframe_corner4_base_0");
    AtlasTex * tbasehoriz = atlasLoadTexture("globalframe_corner1_horizontal_base_0");
    AtlasTex * bbasehoriz = atlasLoadTexture("globalframe_corner3_horizontal_base_0");
    AtlasTex * lbasevert = atlasLoadTexture("globalframe_corner1_vertical_base_0");
    AtlasTex * rbasevert = atlasLoadTexture("globalframe_corner2_vertical_base_0");

    int i;
    F32 sc = 1.f;
    F32 horizWd;
    F32 vertHt;
    F32 xwalk, ywalk, zwalk;
    int topColor = 0x424242e6;
    int botColor = 0x000000e6;

    // draw frame
    zwalk = z + 2.f; //frame sits above the base
    horizWd    = wd - tlcorner[0]->width*sc - trcorner[0]->width*sc;
    ywalk = y;
    for (i = 0; i < 3; ++i)
    {
        xwalk = x;
        display_sprite(tlcorner[i], xwalk, ywalk, zwalk, sc, sc, HYBRID_FRAME_COLOR[i]);
        xwalk += tlcorner[0]->width*sc;
        display_sprite(thoriz[i], xwalk, ywalk, zwalk, horizWd/thoriz[i]->width, sc, HYBRID_FRAME_COLOR[i]);
        xwalk += horizWd;
        display_sprite(trcorner[i], xwalk, ywalk, zwalk, sc, sc, HYBRID_FRAME_COLOR[i]);
    }

    vertHt = ht - tlcorner[0]->height*sc - blcorner->height*sc;
    xwalk = x;
    ywalk += tlcorner[0]->height*sc;;
    display_sprite(lvert, xwalk, ywalk, zwalk, sc, vertHt/lvert->height, HYBRID_FRAME_COLOR[0]);
    xwalk += tlcorner[0]->width*sc + horizWd;
    display_sprite(rvert, xwalk, ywalk, zwalk, sc, vertHt/rvert->height, HYBRID_FRAME_COLOR[0]);

    horizWd    = wd - blcorner->width*sc - brcorner->width*sc;
    ywalk += vertHt;
    xwalk = x;
    display_sprite(blcorner, xwalk, ywalk, zwalk, sc, sc, HYBRID_FRAME_COLOR[0]);
    xwalk += blcorner->width*sc;
    display_sprite(bhoriz, xwalk, ywalk, zwalk, horizWd/bhoriz->width, sc, HYBRID_FRAME_COLOR[0]);
    xwalk += horizWd;
    display_sprite(brcorner, xwalk, ywalk, zwalk, sc, sc, HYBRID_FRAME_COLOR[0]);

    
    // draw base
    zwalk = z;
    horizWd    = wd - tlbasecorner->width*sc - trbasecorner->width*sc;
    ywalk = y;
    xwalk = x;
    display_sprite(tlbasecorner, xwalk, ywalk, zwalk, sc, sc, topColor);
    xwalk += tlbasecorner->width*sc;
    display_sprite(tbasehoriz, xwalk, ywalk, zwalk, horizWd/tbasehoriz->width, sc, topColor);
    xwalk += horizWd;
    display_sprite(trbasecorner, xwalk, ywalk, zwalk, sc, sc, topColor);

    vertHt = ht - tlbasecorner->height*sc - blbasecorner->height*sc;
    xwalk = x;
    ywalk += tlbasecorner->height*sc;;
    display_sprite_blend(lbasevert, xwalk, ywalk, zwalk, sc, vertHt/lbasevert->height, topColor, topColor, botColor, botColor);
    xwalk += tlbasecorner->width*sc;
    display_sprite_blend(white_tex_atlas, xwalk, ywalk, zwalk, horizWd/white_tex_atlas->width, vertHt/white_tex_atlas->height, topColor, topColor, botColor, botColor);
    xwalk += horizWd;
    display_sprite_blend(rbasevert, xwalk, ywalk, zwalk, sc, vertHt/rbasevert->height, topColor, topColor, botColor, botColor);

    horizWd    = wd - blbasecorner->width*sc - brbasecorner->width*sc;
    ywalk += vertHt;
    xwalk = x;
    display_sprite(blbasecorner, xwalk, ywalk, zwalk, sc, sc, botColor);
    xwalk += blbasecorner->width*sc;
    display_sprite(bbasehoriz, xwalk, ywalk, zwalk, horizWd/bbasehoriz->width, sc, botColor);
    xwalk += horizWd;
    display_sprite(brbasecorner, xwalk, ywalk, zwalk, sc, sc, botColor);
}

void drawHybridScrollBar(F32 x, F32 y, F32 z, F32 ht, F32 * yOffset, F32 maxOffset, CBox * mouseScrollArea)
{
    if (maxOffset > 0.f)
    {
        AtlasTex * knob    = atlasLoadTexture( "sliderbar_node_0" );
        F32 val, cx, cy, xposSc, yposSc;
        calculatePositionScales(&xposSc, &yposSc);
        cx = x + knob->width*xposSc/2.f;
        cy = y + ht*yposSc/2.f;

        if ( mouseCollision(mouseScrollArea) )
        {
            F32 mouseScroll = mouseScrollingScaled();
            if (mouseScroll != 0.f)
            {
                *yOffset += mouseScroll;
            }
        }

        // put val betweeen -1 and 1
        val = ((*yOffset / maxOffset) * 2.f) - 1.f;

        val = -drawHybridVerticalSlider(cx, cy, z, ht, -val);

        *yOffset = ((val + 1.f)/2.f)*maxOffset;
    }
}

void drawSectionArrow(F32 cx, F32 cy, F32 z, F32 sc, F32 alphaPct, F32 dir)
{
    AtlasTex * arrow[2] = 
    {
        atlasLoadTexture("icon_tabindicator_0"),
        atlasLoadTexture("icon_tabindicator_1"),
    };
    int alpha = 0xffffff00 | (int)(100.f + 155.f*alphaPct);

    if (dir <= 0.f)
    {
        display_sprite_positional(arrow[1], cx, cy, z, sc, sc, CLR_WHITE&alpha, H_ALIGN_CENTER, V_ALIGN_CENTER );
    }
    display_sprite_positional_rotated(arrow[0], cx, cy, z, sc, sc, CLR_WHITE&alpha, dir * 3.14159f, 0, H_ALIGN_CENTER, V_ALIGN_CENTER);
}

static void setHybridTooltipEx(CBox * box, const char * text, int window)
{
    addToolTip( &hybridToolTip );
    setToolTipEx( &hybridToolTip, box, textStd(text), 0, current_menu(), window, HB_TOOLTIP_DELAY, 0);
    was_tooltip_set = 1;
}

void setHybridTooltip(CBox * box, const char * text)
{
    setHybridTooltipEx(box, text, 0);
}

void setHybridTooltipWindow(CBox * box, const char * text, int window)
{
    setHybridTooltipEx(box, text, window);
}

int getLastNavMenuType()
{
    return sLastNavMenuType;
}

void hybridMenuFlash(int incomplete)
{
    sIncomplete = incomplete;
}
