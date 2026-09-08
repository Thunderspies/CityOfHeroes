/***************************************************************************
 *     Copyright (c) 2000-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/

#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/wininclude.h>

#include "UI/uiGame.h"
#include "player/player.h"
#include "entity/costume_client.h"
#include "UI/uiUtilGame.h"
#include "UI/uiUtil.h"
#include <utilitieslib/utils/sysutil.h>
#include "UI/sprite/sprite_base.h" // only for interp_rgba
#include "UI/uiUtilMenu.h"  // only for do_gbut and doGbutScaled
#include "uiPCCCreationNLM.h"
#include "cmdparse/cmdcommon.h"   // only for TIMESTEP
#include "UI/uiGender.h"
#include "graphics/FX/fx.h"
#include "UI/uiAvatar.h"
#include "UI/uiCostume.h"
#include "uiLoadCostume.h"
#include "uiSaveCostume.h"
#include "UI/uiTailor.h"
#include "UI/uiInput.h"
#include "UI/uiSupercostume.h"
#include <utilitieslib/components/earray.h>
#include <utilitieslib/utils/timing.h>
#include "UI/sprite/sprite_text.h"
#include "UI/sprite/sprite_font.h"
#include "graphics/textureatlas.h"
#include "render/tex.h"
#include "graphics/font.h"
#include "gameComm/initClient.h"
#include "seq/animtrack.h"  //for MAX_BONES
#include "gameData/costume_data.h"
#include "cmdparse/cmdgame.h"
#include <utilitieslib/utils/Color.h>
#include "language/langClientUtil.h"
#include "sound/sound.h"
#include "entity/character_base.h"
#include "entity/entPlayer.h"
#include "uiSlider.h"
#include "graphics/ttFontUtil.h"
#include "UI/uiComboBox.h"
#include "seq/seqstate.h"
#include <utilitieslib/utils/file.h>
#include "entity/entity.h"
#include "uiScrollSelector.h"
#include "gameComm/Npc.h"
#include "UI/uiScrollBar.h"
#include "UI/uiNet.h"
#include "entity/power_customization.h"
#include "gameData/costume_critter.h"
#include "win/input.h"
#include "entity/entclient.h"
#include "auth/authUserData.h"
#include "clientcomm/dbclient.h"
#include <utilitieslib/language/AppLocale.h>
#include <utilitieslib/language/MessageStoreUtil.h>
#include "UI/uiPowerCust.h"
#include "entity/powers.h" // Power types
#include "win/win_init.h"
#include "UI/Hybrid/uiHybridMenu.h"
#include "malloc.h"
#include "UI/Hybrid/uiOrigin.h"
#include "UI/Hybrid/uiArchetype.h"
#include "UI/Hybrid/uiPower.h"
#include "UI/uiCostume.h"
#include "UI/Hybrid/uiBody.h"
#include "player/inventory_client.h"
#include "account/AccountData.h"
#include "account/AccountCatalog.h"
#include "UI/uiDialog.h"

#ifdef TEST_CLIENT
Costume *pccFixupCostume = NULL;
void *rdrGetTex(int texid, int *width, int *height, int get_color, int get_alpha, int floating_point) {    return NULL;}
#else
extern Costume *pccFixupCostume;
#include "render/renderprim.h"
#endif
int gLoadRandomPresetCostume = 1;
static CostumeGeoSet *s_selectedGeoSet = NULL;
void initAnimComboBox(int rebuild);
static int colorTintablePartsBitField = 0;

#ifdef TEST_CLIENT
//////////////////////////////////////////////////////////////////////////
/// Begin hack for test client
#pragma warning (disable:4002)
#pragma warning (disable:4101)
#define drawMenuBarSquished() 0
#define fxDelete()
#define drawNextButton() 0
#define mouseDownHit() 0
#define mouseCollision() 0
#define str_dims()
#define drawMenuFrame()
#define drawNonLinearMenu() 0
#define xyprintf()
#define display_sprite()
#define display_spriteFlip()
#define isDown() 0
#define build_cboxxy()
#define zoomAvatar()
#define mouseUp() 0
#define BuildCBox()
#define brightenColor()
#define mouseUpHit() 0
#define mouseClickHit() 0
#define display_rotated_sprite()
#define prnt()
#define drawFrame()
#define drawBackground()
#define tailor_displayCost()
#define tailor_DisplayRegionTotal()
#define tailor_RevertRegionToOriginal()
#define selectableText() 0
#define str_wd() 0
#define str_sc() 0
#define drawHelpOverlay()
#define genderScale()
#define cycleCritterCostumes()
#define inpEdge()
#define inpLevel()
#define genderArmScale()
#define genderLegScale()
#define genderHipScale()
#define genderWaistScale()
#define genderShoulderScale()
#define genderChestScale()
#define genderHeadScale()
#define genderBoneScale()
#define combobox_display()
#define setAnim()
#define setAnims()
#define changeGeoSet()
#define inpEdgeNoTextEntry(INP_ENTER) 0
#define powerCustCantEnterCode() 1
#define calc_scrn_scaling()
#define drawNLMIconButton() 0
#define is_scrn_scaling_disabled() 0
#define set_scrn_scaling_disabled()
#define stateScale() 0 
#define drawScaleSliders() 0
#define drawFaceSlider() 0
#define hybridElementInit() 0
#define drawHybridTab() 0
#define drawHybridDescFrame() 0
#define drawSectionArrow() 0
#define getSelectedCharacterClass() 0
#define drawHybridStoreMenuButton() 0
#define drawHybridStoreLockIcon() 0
#define drawHybridStoreLockIconCustomTooltip() 0
#define drawHybridStoreAddShoppingCartMenuButton() 0
#define clearSS() 0
#define setSpriteScaleMode() 0
#define calculateSpriteScales() 0
#define setScalingModeAndOption() 0
#define applyPointToScreenScalingFactorf() 0
#define isTextScalingRequired() 0
#define getSpriteScaleMode() 0
#define BuildScaledCBox() 0
#define applyCostumeSet() 0
const int HYBRID_FRAME_COLOR[3];
void selectCostumeSet(const CostumeOriginSet *oset, float x, float y, float z, float sc, float wd, F32 alpha) {}
void selectBoneset( const CostumeRegionSet * region, float x, float y, float z, float sc, float wd, int mode, F32 alpha ){}
void selectGeoSet( CostumeGeoSet * gset, int type, float x, float y, float z, float sc, float wd, int mode, F32 alpha ){}
int updateScrollSelectors(F32 min, F32 max){ return 0;}
void powerCustMenuStart(int prevMenu, int nextMenu) { start_menu(MENU_REGISTER);    }

/// End hack for test client
//////////////////////////////////////////////////////////////////////////
#endif

//----------------------------------------------------------------------------------------------------------------
// Globals - Constants ///////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------------

#define BUTTON_COLOR            0   // standard part
#define BUTTON_COLOR_HAIR       1   // not used atm
#define BUTTON_COLOR_SKIN       2   // skin part

CostumeSave ** gCostStack = 0; //stack of random costumes created

int gCurrentBodyType      = 0;
int gCurrentOriginSet     = 0;
int gCurrentRegion = REGION_SETS;

int gSelectedCostumeSet = -1;
int gActiveColor = 0;
int gColorsLinked = TRUE;
int gRandSeed;
int gConvergingFace;
Color gLinkedColor1;
Color gLinkedColor2;
const defaultFaceColorIndex = 7;  //light flesh tone
static const Color defaultCostumeColor1 = {0x1f, 0x1f, 0x1f,0xff}; //gray tone for default costume color
static const Color defaultCostumeColor2 = {0xe3, 0xe3, 0xe3,0xff}; //gray tone for default costume color

static F32 compressSc = 1.f;
static F32 globalAlpha = 0.f;
 
float LEFT_COLUMN_SQUISH = 1.f; // deprecated

static int costumeButtonColor, costumeTextColor1, costumeTextColor2, costumeFrameBackColor1, costumeFrameBackColor2;

Vec3 gFaceDest[NUM_3D_BODY_SCALES];
Vec3 gFaceDestSpeed[NUM_3D_BODY_SCALES];

static int init = FALSE;
static int * initRandomCostumeSet[3];
static int repositioningCamera = 1;
static int cc_transactionID = 0;
static int pcc_transactionID = 0;
static int currentBackground = 0;

static F32 rStoreScale[REGION_TOTAL_UI];

static ShoppingCart sStorePartsContained;
static int hideStorePiecesState = HIDE_STORE_PIECES_UNITIALIZED;

const CostumeOriginSet*    costume_get_current_origin_set(void)
{
    return gCostumeMaster.bodySet[gCurrentBodyType]->originSet[gCurrentOriginSet];
}

int costume_bonesetHasFlag( const CostumeBoneSet *bset, const char *flag)
{
    int l;

    if (bset == NULL)
        return 0;

    for (l = 0; l < eaSize(&bset->flags); l++)
    {
        if (stricmp(flag, bset->flags[l]) == 0)
            return 1;
    }
    return 0;
}

int costume_geosetHasFlag( const CostumeGeoSet *gset, const char *flag)
{
    int l;

    if (gset == NULL)
        return 0;

    for (l = 0; l < eaSize(&gset->flags); l++)
    {
        if (stricmp(flag, gset->flags[l]) == 0)
            return 1;
    }
    return 0;
}

int costume_partHasFlag( const CostumeTexSet *tset, const char *flag)
{
    int l;

    if (tset == NULL)
        return 0;

    for (l = 0; l < eaSize(&tset->flags); l++)
    {
        if (stricmp(flag, tset->flags[l]) == 0)
            return 1;
    }
    return 0;
}

int costume_hasFlag(Entity *e, const char *flag)
{
    int i, j;
    int region_total = REGION_TOTAL_DEF;
    const CostumeOriginSet    *cset = costume_get_current_origin_set();
    const CostumeBoneSet      *bset;

    if (e == NULL || flag == NULL)
        return 0;

    if (region_total > eaSize(&cset->regionSet))
        region_total=eaSize(&cset->regionSet);

    for( j = 0; j < region_total; j++ )
    {
        bset = cset->regionSet[j]->boneSet[cset->regionSet[j]->currentBoneSet];

        if (costume_bonesetHasFlag(bset, flag))
            return true;

        for( i = eaSize( &bset->geo )-1; i >= 0; i-- )
        {
            const CostumeGeoSet *gset = bset->geo[i];
            int bpID = -1;

            if( gset->bodyPartName )
            {
                if (costume_geosetHasFlag(gset, flag))
                    return 1;

                if (costume_partHasFlag(gset->info[gset->currentInfo], flag))
                    return 1;
            }
        }
    }
    return 0;
}

static int costume_hasFlagWithException(Entity *e, const char *flag, const char *excludeName, const CostumeRegionSet * excludeRSet)
{
    int i, j;
    int region_total = REGION_TOTAL_DEF;
    const CostumeOriginSet    *cset = costume_get_current_origin_set();
    const CostumeBoneSet      *bset;

    if (e == NULL || flag == NULL || excludeName == NULL)
        return 0;

    if (region_total > eaSize(&cset->regionSet))
        region_total=eaSize(&cset->regionSet);

    for( j = 0; j < region_total; j++ )
    {
        if (excludeRSet == cset->regionSet[j])
        {
            continue;
        }

        bset = cset->regionSet[j]->boneSet[cset->regionSet[j]->currentBoneSet];

        if (excludeName && 
            stricmp(excludeName, bset->name) != 0 && 
            costume_bonesetHasFlag(bset, flag))
            return true;

        for( i = eaSize( &bset->geo )-1; i >= 0; i-- )
        {
            const CostumeGeoSet *gset = bset->geo[i];
            int bpID = -1;

            if( gset->bodyPartName )
            {
                if (excludeName && bset->name && 
                    stricmp(excludeName, bset->name) != 0 && 
                    costume_geosetHasFlag(gset, flag))
                    return 1;

                if (excludeName && 
                    gset->info[gset->currentInfo] != NULL && 
                    bset->name &&
                    stricmp(excludeName, bset->name) != 0 && 
                    costume_partHasFlag(gset->info[gset->currentInfo], flag))
                    return 1;
            }
        }
    }
    return 0;
}

int costume_partHasBadDependencyPair(const CostumeTexSet *tset)
{
    if (costume_partHasFlag(tset, "EyeAura") && costume_hasFlag(playerPtr(), "AnimalHead"))
        return 1;

    // Add additional bad dependency pairs here:

    return 0;
}

static Color getColor( int i, int type ); //fwd declaration

void resetColorsToDefault(Entity * e)
{
    Color sc = getColor( defaultFaceColorIndex, COLOR_SKIN );
    Costume* mutable_costume = costume_as_mutable_cast(e->costume);
    costume_SetSkinColor( mutable_costume, sc );

    gLinkedColor1 = defaultCostumeColor1;
    gLinkedColor2 = defaultCostumeColor2;
}

static void resetRandomCostumes(void)
{
    int i;
    for (i = 0; i < 3; ++i)
    {
        if (initRandomCostumeSet[i])
        {
            eaiDestroy(&initRandomCostumeSet[i]);
        }
    }
}

void resetCostumeMenu()
{
    gCurrentBodyType = 0;
    gCurrentOriginSet = 0;
    gCurrentRegion = REGION_SETS;
    gSelectedCostumeSet = -1;
    gActiveColor = 0;
    gColorsLinked = TRUE;
    init = FALSE;
    resetRandomCostumes();
    repositioningCamera = 1;
    currentBackground = 0;
    sStorePartsContained.itemCount = 0;
    gLinkedColor1 = defaultCostumeColor1;
    gLinkedColor2 = defaultCostumeColor2;
    hideStorePiecesState = HIDE_STORE_PIECES_UNITIALIZED;

    clearCostumeStack();
}

void clearSelectedGeoSet()
{
    s_selectedGeoSet = NULL;
}
const CostumeGeoSet* getSelectedGeoSet(int mode)
{
    const CostumeRegionSet* region;
    const CostumeBoneSet* boneSet;
    int numBones;
    int i;

    if(gCurrentRegion >= eaSize(&gCostumeMaster.bodySet[gCurrentBodyType]->originSet[gCurrentOriginSet]->regionSet) || gCurrentRegion < 0)
        return 0;
    
    if ( (mode == COSTUME_EDIT_MODE_NPC) || (mode == COSTUME_EDIT_MODE_NPC_NO_SKIN) )
    {
        return s_selectedGeoSet;
    }
    region = gCostumeMaster.bodySet[gCurrentBodyType]->originSet[gCurrentOriginSet]->regionSet[gCurrentRegion];
    boneSet = region->boneSet[region->currentBoneSet];

    numBones = eaSize(&boneSet->geo);
    for (i = 0; i < numBones; ++i)
    {
        if (boneSet->geo[i]->isOpen)
            return boneSet->geo[i];
    }
    
    return 0;
}

//----------------------------------------------------------------------------------------------------------------
// Costume UI drawing functions //////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------------
// the dumb functions that just draw stuff go here

#define REGION_X        50
#define REGION_Y        100
#define REGION_WD       306
#define REGION_CENTER (REGION_X + (REGION_WD/2) - 10)
#define REGION_SPEED    40
#define REGION_DEFAULT_SPACE 50
#define MAX_HT 500

#define ELEMENT_HT 50
#define DEFAULT_BAR_HT  56.f

static F32 reg_ht[REGION_TOTAL_UI] = {0};

#define CLR_DIM         0xccccccff
#define BAR_OFF         27

int drawCostumeRegionButton( const char * txt, F32 cx, F32 cy, F32 z, F32 wd, F32 ht, int selected, int valid, HybridElement *hb, int flip )
{
    // Determine scale based off, ht given
    F32 scale = MINMAX( ht/DEFAULT_BAR_HT, 0.f, 1.f );

    if( hb )
    {
        int flags = HB_TAIL_RIGHT_ROUND;

        if( txt )
            hb->text = txt;

        if( flip )
            flags = HB_TAIL_LEFT_ROUND;
        if( txt && strlen(textStd(txt)) > 14)
        {
            flags |= HB_SCALE_DOWN_FONT;
        }

        if( D_MOUSEHIT == drawHybridBar(hb, selected, cx, cy, z, wd, scale, flags, (1.f-globalAlpha/2), H_ALIGN_LEFT, V_ALIGN_CENTER ) )
        {
            return 1;
        }
    }

    return 0;
}

#define SPIN_BUTTON_ZPOS_DEFAULT    50.f
#define SPIN_BUTTON_XPOS_CENTER_LOGIN    640.f
#define SPIN_BUTTON_XPOS_OFFSET_LOGIN    180.f
#define SPIN_BUTTON_YPOS_LOGIN        580.f
#define SPIN_BUTTON_XPOS_CENTER_DEFAULT 815.f
#define SPIN_BUTTON_XPOS_OFFSET_DEFAULT 120.f

void drawSpinButtons(float screenScaleX, float screenScaleY)
{
    AtlasTex * spinL;
    AtlasTex * spinR;
    AtlasTex * ospinL;
    AtlasTex * ospinR;
    AtlasTex * aspinL;
    AtlasTex * aspinR;
    CBox box;
    float x[2] = {0, 0};
    float y = 645.f * screenScaleY;
    float z = SPIN_BUTTON_ZPOS_DEFAULT;
    int colorNormal = CLR_WHITE;

    if (game_state.skin != UISKIN_HEROES)
    {
        spinL     = atlasLoadTexture( "costume_button_spin_L_tintable.tga" );
        spinR     = atlasLoadTexture( "costume_button_spin_R_tintable.tga" );
        ospinL    = atlasLoadTexture( "costume_button_spin_over_L_tintable.tga" );
        ospinR    = atlasLoadTexture( "costume_button_spin_over_R_tintable.tga" );
        aspinL    = atlasLoadTexture( "costume_button_spin_activate_L_grey.tga" );
        aspinR    = atlasLoadTexture( "costume_button_spin_activate_R_grey.tga" );
        //colorNormal = uiColors.standard.base;
    }
    else
    {
        spinL     = atlasLoadTexture( "costume_button_spin_L.tga" );
        spinR     = atlasLoadTexture( "costume_button_spin_R.tga" );
        ospinL    = atlasLoadTexture( "costume_button_spin_over_L.tga" );
        ospinR    = atlasLoadTexture( "costume_button_spin_over_R.tga" );
        aspinL    = atlasLoadTexture( "costume_button_spin_activate_L.tga" );
        aspinR    = atlasLoadTexture( "costume_button_spin_activate_R.tga" );
    }

       if( isMenu(MENU_LOGIN) )
    {
        x[0] = SPIN_BUTTON_XPOS_CENTER_LOGIN - (((spinL->width/2.f) + SPIN_BUTTON_XPOS_OFFSET_LOGIN));
        x[1] = SPIN_BUTTON_XPOS_CENTER_LOGIN - (((spinR->width/2.f) - SPIN_BUTTON_XPOS_OFFSET_LOGIN));
        y = SPIN_BUTTON_YPOS_LOGIN; 
    }
    else
    {
        x[0] = (SPIN_BUTTON_XPOS_CENTER_DEFAULT * screenScaleX) - ( ( (spinL->width/2.f) + (SPIN_BUTTON_XPOS_OFFSET_DEFAULT * screenScaleX) ) );
        x[1] = (SPIN_BUTTON_XPOS_CENTER_DEFAULT * screenScaleX) - ( ( (spinR->width/2.f) - (SPIN_BUTTON_XPOS_OFFSET_DEFAULT * screenScaleX) ) );
    }

    build_cboxxy( spinL, &box, 1.5f, 1.5f, x[0] - (20), y - (20) );
    if( mouseCollision(&box) )
    {
        if( isDown( MS_LEFT ))
        {
            display_sprite( aspinL, x[0], y, z, 1.f, 1.f, CLR_WHITE );
            playerTurn( -5 * (TIMESTEP) );
        }
        else
            display_sprite( ospinL, x[0], y, z,  1.f, 1.f, colorNormal );
    }
    else
        display_sprite( spinL, x[0], y, z,  1.f, 1.f, colorNormal );

    build_cboxxy( spinR, &box, 1.5f, 1.5f, x[1] - (20), y - (20) );

    if( mouseCollision(&box) )
    {
        if( isDown( MS_LEFT ))
        {
            display_sprite( aspinR, x[1], y, z,  1.f, 1.f, CLR_WHITE );
            playerTurn( 5 * (TIMESTEP) );
        }
        else
            display_sprite( ospinR, x[1], y, z,  1.f, 1.f, colorNormal );
    }
    else
        display_sprite( spinR, x[1], y, z,  1.f, 1.f, colorNormal );
}

#define RESET_WD                50
#define COLOR_SPIN_SPEED        0.05f
#define PALETTE_SCALE            0.92f
//
//
static int drawColorRing( float x, float y, float z, int selected, int color, float xsc, float ysc, int clickable )
{
    F32 xscale = 0.8f*xsc*PALETTE_SCALE;
    F32 yscale = 0.8f*ysc*PALETTE_SCALE;

    CBox box;

     AtlasTex * back      = atlasLoadTexture("costume_color_ring_background_square");
    AtlasTex * swirl2    = atlasLoadTexture("costume_color_selected_square");
    AtlasTex * shadow    = atlasLoadTexture("costume_color_selected_3d_square");
    int ret = D_NONE;
    F32 xposSc, yposSc;
    calculatePositionScales(&xposSc, &yposSc);

       BuildScaledCBox( &box, x - back->width*xposSc*xscale/2, y - back->height*yposSc*yscale/2, back->width*xposSc*xscale, back->height*yposSc*yscale);

    if( mouseCollision( &box ) && clickable )
    {
        ret = D_MOUSEOVER;
         xscale = 1.0f*xsc*PALETTE_SCALE;
        yscale = 1.0f*ysc*PALETTE_SCALE;
        if( mouseClickHit( &box, MS_LEFT ) && clickable )
        {
            ret = D_MOUSEHIT;
        }
    }
      display_sprite_positional( back, x, y, z, xscale, yscale, color, H_ALIGN_CENTER, V_ALIGN_CENTER );

      if( selected )
    {
         display_sprite_positional( shadow, x, y, z+1, xscale, yscale, CLR_H_GLOW, H_ALIGN_CENTER, V_ALIGN_CENTER );

        if( !gHelpOverlay )
            display_sprite_positional( swirl2, x, y, z+30, xscale, yscale, CLR_H_GLOW, H_ALIGN_CENTER, V_ALIGN_CENTER );
    }

    return ret;
}

//
//
static ColorBarResult drawColorTab(HybridElement *hb, F32 x, F32 y, F32 z, F32 scale, int selected,
                                   int colorL, int colorR, int twoColors, int cselected, bool grey, int baseColorIndex, int * hovered )
{
     int retValue = 0;
    F32 xColorBump = 15.f;
    F32 yColorBump = 15.f;
    int colorInteract;
    F32 xposSc, yposSc;
    calculatePositionScales(&xposSc, &yposSc);

     if (drawHybridTab(hb, x, y, z, 1.f, scale, selected, (0.5f + hb->percent*0.5f)*scale))
    {
        retValue = CB_HIT;
    }

    if( !twoColors )
    {
        // now draw the rings on top
        colorInteract = ( drawColorRing( x + xColorBump*xposSc, y+yColorBump*yposSc*scale, z, cselected, (colorL&0xffffff00)|(int)(255*scale), 0.75f, scale*.75, 1 ) && !grey);
        if( D_MOUSEOVER <= colorInteract )
        {
            *hovered = baseColorIndex + 0;
        }
        if (D_MOUSEHIT == colorInteract)
            retValue = CB_HIT_ONE;
    }
    else // account for the second color if there was one
    {
        // now draw the rings on top
         colorInteract = ( drawColorRing( x+xColorBump*xposSc, y+yColorBump*yposSc*scale, z, !grey && (cselected == 1 || cselected == 3), (colorL&0xffffff00)|(int)(255*scale), 0.75f, scale*0.75f, 1 ) && !grey );
        if( D_MOUSEOVER <= colorInteract )
        {
            *hovered = baseColorIndex + 0;
        }
        if (D_MOUSEHIT == colorInteract)
            retValue = CB_HIT_ONE;

        colorInteract = ( drawColorRing( x+(xColorBump+17.f)*xposSc, y+yColorBump*yposSc*scale, z, !grey && (cselected == 2 || cselected == 4), (colorR&0xffffff00)|(int)(255*scale), 0.75f, scale*0.75f, 1 ) && !grey );
        if( D_MOUSEOVER <= colorInteract )
        {
            *hovered = baseColorIndex + 1;
        }
        if (D_MOUSEHIT == colorInteract)
            retValue = CB_HIT_TWO;
    }

    return retValue;
}


#define LINK_Z 5000
//
//
static bool drawColorsLinkedSelector( float x, float y, float scale, int* value, char* target, int clickable)
{
    AtlasTex *check, *checkD;
    AtlasTex * glow   = atlasLoadTexture("costume_button_link_glow.tga");
    int checkColor = CLR_WHITE;
    int checkColorD = CLR_WHITE;
    bool result = false;

    CBox box;

    if (*value)
    {
        if (game_state.skin != UISKIN_HEROES)
        {
            check  = atlasLoadTexture("costume_button_link_check_grey.tga");
            checkD = atlasLoadTexture("costume_button_link_check_press_grey.tga");
        }
        else
        {
            check  = atlasLoadTexture("costume_button_link_check.tga");
            checkD = atlasLoadTexture("costume_button_link_check_press.tga");
        }
    }
    else
    {
        if (game_state.skin != UISKIN_HEROES)
        {
            check   = atlasLoadTexture("costume_button_link_off_tintable.tga");
            checkD  = atlasLoadTexture("costume_button_link_off_press_tintable.tga");
        }
        else
        {
            check   = atlasLoadTexture("costume_button_link_off.tga");
            checkD  = atlasLoadTexture("costume_button_link_off_press.tga");
        }
    }

    BuildCBox( &box, x - check->width*scale/2, y - check->width*scale/2, check->width*scale, check->height*scale );

    if( mouseCollision( &box ) && clickable )
    {
        display_sprite( glow, x - glow->width/2, y - glow->height/2, LINK_Z-1, 1.f, 1.f, CLR_WHITE );

        if( isDown( MS_LEFT ) )
            display_sprite( checkD, x - checkD->width/2, y - checkD->height/2, LINK_Z, 1.f, 1.f, checkColorD );
        else
            display_sprite( check, x - check->width/2, y - check->height/2, LINK_Z, 1.f, 1.f, checkColor );

        if( mouseClickHit( &box, MS_LEFT) )
        {
            *value = !*value;
            result = true;
        }
    }
    else
        display_sprite( check, x - check->width*scale/2, y - check->height*scale/2, LINK_Z, scale, scale, checkColor );

    if (getCurrentLocale() == 3)
    {
        char concat[128];
        sprintf(concat,"%s %s",textStd("CopyCurrentColors"),textStd(target));
        font( &game_12 );
        font_color( CLR_WHITE, CLR_WHITE );
        prnt( x + (check->width/2 + 5)*scale, y + 5*scale, LINK_Z, scale, scale, concat);

    }
    else
    {
        font( &game_9 );
        font_color( CLR_WHITE, CLR_WHITE );
        prnt( x + (check->width/2 + 5)*scale, y, LINK_Z, scale, scale, "CopyCurrentColors");
        prnt( x + (check->width/2 + 5)*scale, y + 10*scale, LINK_Z, scale, scale, target);
    }

    return result;
}

#define ZOOM_X SPIN_BUTTON_XPOS_CENTER_DEFAULT
#define ZOOM_Y 682

void drawZoomButton(int* isZoomedIn)
{
    AtlasTex *zoom, *zoomD;
    AtlasTex *glow = atlasLoadTexture("costume_button_link_glow.tga");

    CBox box;

    if( *isZoomedIn )
    {
        if (game_state.skin != UISKIN_HEROES)
        {
            zoom  = atlasLoadTexture("costume_button_zoom_out_tintable.tga");
            zoomD = atlasLoadTexture("costume_button_zoom_out_press_tintable.tga");
        }
        else
        {
            zoom  = atlasLoadTexture("costume_button_zoom_out.tga");
            zoomD = atlasLoadTexture("costume_button_zoom_out_press.tga");
        }
    }
    else
    {
        if (game_state.skin != UISKIN_HEROES)
        {
            zoom  = atlasLoadTexture("costume_button_zoom_in_tintable.tga");
            zoomD = atlasLoadTexture("costume_button_zoom_in_press_tintable.tga");
        }
        else
        {
            zoom  = atlasLoadTexture("costume_button_zoom_in.tga");
            zoomD = atlasLoadTexture("costume_button_zoom_in_press.tga");
        }
    }

    BuildCBox( &box, (ZOOM_X) - (zoom->width/2), (ZOOM_Y) - (zoom->height/2), zoom->width, zoom->height );

    if( mouseCollision( &box ) )
     {
        display_sprite( glow, (ZOOM_X) - (glow->width/2), (ZOOM_Y) - (glow->height/2), ZOOM_Z-10, 1.f, 1.f, CLR_WHITE );

        if( isDown( MS_LEFT ) )
            display_sprite( zoomD, (ZOOM_X) - (zoomD->width/2), (ZOOM_Y) - (zoomD->height/2), ZOOM_Z, 1.f, 1.f,CLR_WHITE );
        else
            display_sprite( zoom, (ZOOM_X) - (zoom->width/2), (ZOOM_Y) - (zoom->height/2), ZOOM_Z, 1.f, 1.f, CLR_WHITE );

        if( mouseClickHit( &box, MS_LEFT) )
        {
            if( *isZoomedIn )
                sndPlay( "minus", SOUND_GAME );
            else
                sndPlay( "plus", SOUND_GAME );

            *isZoomedIn = !*isZoomedIn;
        }
    }
    else
        display_sprite( zoom, (ZOOM_X) - (zoom->width/2), (ZOOM_Y) - (zoom->height/2), ZOOM_Z, 1.f, 1.f, CLR_WHITE );
}

//
void drawZoomButtonHybrid(F32 cx, F32 cy, int* isZoomedIn )
{
    const char * icon = *isZoomedIn ? "icon_costume_zoomout" : "icon_costume_zoomin";
    static HybridElement sButtonIcon = {0, NULL, "ZoomButton", NULL};
    sButtonIcon.iconName0 = icon;

    if( D_MOUSEHIT == drawHybridButton(&sButtonIcon, cx, cy, ZOOM_Z, 1.f, CLR_WHITE, HB_DRAW_BACKING ) )
    {
        if( *isZoomedIn )
            sndPlay( "N_ZoomOut", SOUND_GAME );
        else
            sndPlay( "N_ZoomIn", SOUND_GAME );

        *isZoomedIn = !*isZoomedIn;
    }
}

static int drawBackgroundAdjustButton(float startx, float starty, float z, float wd, float ht, float xscale, float yscale, int isDarker, int buttonColor)
{
    CBox box;
    AtlasTex *bgAdjustBtnBaseEnd = atlasLoadTexture("CC_BGA_Button_Base_End");
    AtlasTex *bgAdjustBtnGrad = atlasLoadTexture("CC_BGA_Button_Grad");
    AtlasTex *bgAdjustBtnHighlight = atlasLoadTexture("CC_BGA_Button_Highlight");
    AtlasTex *bgAdjustBtnHighlightEnd = atlasLoadTexture("CC_BGA_Button_Highlight_End");
    AtlasTex *bgAdjustBtnShadowBottom = atlasLoadTexture("CC_BGA_Button_Shadow_Bottom");
    AtlasTex *bgAdjustBtnShadowEnd = atlasLoadTexture("CC_BGA_Button_Shadow_End");
    AtlasTex *bgAdjustBtnEndBlackGlow = atlasLoadTexture("CC_BGA_Button_Base_End_BlackGlow");
    float x = startx;
    float y = starty;
    float directionModifier = isDarker ? -1.0f : 1.0f;

    int flipModifier = isDarker ? 0 : FLIP_HORIZONTAL;
    int mouseOver = 0;
    float x_scr_scale = 1.0f, y_scr_scale = 1.0f;

    calc_scrn_scaling(&x_scr_scale, &y_scr_scale);

    yscale = ht/ bgAdjustBtnBaseEnd->height;
    xscale = yscale*y_scr_scale/x_scr_scale;
    

    BuildCBox(&box, x-wd/2, y-ht/2, wd, ht);

    display_spriteFlip(bgAdjustBtnEndBlackGlow, x-(xscale*bgAdjustBtnEndBlackGlow->width/2.0f), y-(ht/2.0f),z+1, xscale, yscale, CLR_BLACK, flipModifier);

    display_spriteFlip(bgAdjustBtnBaseEnd, x-(xscale*bgAdjustBtnBaseEnd->width/2.0f), y-(ht/2.0f),z+2, xscale, yscale, buttonColor, flipModifier);

    display_spriteFlip(bgAdjustBtnGrad, x-(xscale*bgAdjustBtnGrad->width/2.0f), y-(ht/2.0f),z+3, xscale, yscale, buttonColor, flipModifier);


    display_spriteFlip(bgAdjustBtnShadowEnd, x-(xscale*bgAdjustBtnShadowEnd->width/2.0f), y-(ht/2.0f),z+4, xscale, yscale, buttonColor, flipModifier);
    display_spriteFlip(bgAdjustBtnHighlightEnd, x-(xscale*bgAdjustBtnHighlightEnd->width/2.0f), y-(ht/2.0f),z+5, xscale, yscale, CLR_WHITE, flipModifier);

    display_spriteFlip(bgAdjustBtnShadowBottom, x-(xscale*bgAdjustBtnShadowBottom->width/2.0f), y-(ht/2.0f),z+6, xscale, yscale, buttonColor, flipModifier);

    display_spriteFlip(bgAdjustBtnHighlight, x-(xscale*bgAdjustBtnHighlight->width/2.0f), y-(ht/2.0f),z+7, xscale, yscale, CLR_WHITE, flipModifier);

    if (mouseCollision(&box))
    {
        mouseOver = 1;
        xscale += 0.1f;
        yscale += 0.1f;
    }

    if (isDarker)
    {
        AtlasTex *bgAdjustIconDark = atlasLoadTexture("CC_BGA_Icon_Dark");
        display_spriteFlip(bgAdjustIconDark, x+4-(xscale * bgAdjustIconDark->width/2.0f), y-(yscale*bgAdjustIconDark->height/2.0f),z+8, xscale, yscale, CLR_WHITE, flipModifier);
    }
    else
    {
        AtlasTex *bgAdjustIconBright = atlasLoadTexture("CC_BGA_Icon_Bright");
        display_spriteFlip(bgAdjustIconBright, x-4-(xscale *bgAdjustIconBright->width/2.0f), y-(yscale*bgAdjustIconBright->height/2.0f),z+8, xscale, yscale, CLR_WHITE, flipModifier);
    }

    return mouseClickHit(&box, MS_LEFT);
}
int drawBackgroundAndTintSelect(F32 cx, F32 cy)
{
    int color;
    static HybridElement sButtonSun = {0, NULL, "brightenBackgroundButton", "icon_costume_sun_0"};
    static HybridElement sButtonMoon = {0, NULL, "dimBackgroundButton", "icon_costume_moon_0"};
    F32 xposSc, yposSc;
    SpriteScalingMode ssm = getSpriteScaleMode();
    calculatePositionScales(&xposSc, &yposSc);

       if (D_MOUSEHIT == drawHybridButton( &sButtonSun, cx + 60.f*xposSc, cy, ZOOM_Z, 1.f, CLR_WHITE, (currentBackground >= 4) | HB_DRAW_BACKING ))
    {
        sndPlay("N_SelectSmall", SOUND_GAME);
        currentBackground++;
    }

    if (D_MOUSEHIT == drawHybridButton( &sButtonMoon, cx - 60.f*xposSc, cy, ZOOM_Z, 1.f, CLR_WHITE, (currentBackground <= -4) | HB_DRAW_BACKING ))
    {
        sndPlay("N_SelectSmall", SOUND_GAME);
        currentBackground--;
    }

    currentBackground = CLAMP(currentBackground, -4, 4);

    if( currentBackground < 0 )
        color = ABS(currentBackground)*50;
        else
        color = 0xffffff00 + currentBackground*50;

    setSpriteScaleMode(SPRITE_XY_SCALE);
    display_sprite( white_tex_atlas, 0, 0, 3, DEFAULT_SCRN_WD/white_tex_atlas->width, DEFAULT_SCRN_HT/white_tex_atlas->height, color );
    setSpriteScaleMode(ssm);

    return currentBackground;
    }
    
//returns whether player was moved/rotated
int drawAvatarControls(F32 cx, F32 cy, int * zoomedIn, int drawArrowsOnly)
{
    int rotating = 0;
    static HybridElement sButtonArrow0 = {0, NULL, "rotateAvatarButton", "icon_arrow"};
    static HybridElement sButtonArrow1 = {0, NULL, "rotateAvatarButton", "icon_arrow"};
    F32 xposSc, yposSc;
    calculatePositionScales(&xposSc, &yposSc);
    if( drawHybridButton(&sButtonArrow0, cx + 130.f*xposSc, cy, ZOOM_Z, 1.f, CLR_H_WHITE, HB_FLIP_H | HB_DRAW_BACKING ) >= D_MOUSEDOWN  )
    {
        sndStop(SOUND_GAME, 0.f); //don't play default sound
        playerTurn( 5 * (TIMESTEP) );
        rotating = 1;
    }
    if( drawHybridButton(&sButtonArrow1, cx - 130.f*xposSc, cy, ZOOM_Z, 1.f, CLR_H_WHITE, HB_DRAW_BACKING ) >= D_MOUSEDOWN  )
    {
        sndStop(SOUND_GAME, 0.f); //don't play default sound
        playerTurn( -5 * (TIMESTEP) );
        rotating = 1;
    }

    if (rotating)
    {
        sndPlay("N_Rotate_Loop", SOUND_UI_REPEATING);
    }
    else
    {
        sndStop(SOUND_UI_REPEATING, 0.f);
    }

    if (!drawArrowsOnly)
    {
        drawZoomButtonHybrid(cx, cy, zoomedIn );
        drawBackgroundAndTintSelect(cx, cy);
    }

    return rotating;
}


//----------------------------------------------------------------------------------------------------------------
// Costume Color Functions /////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------------
// the functions that mess with colors go here
//
#define DEFAULT_GREY CreateColor( 255, 255, 255, 255 )

void costume_SetStartColors()
{
    gColorsLinked = FALSE;
}

//
//
int compareColors( const Vec3 c1, const Color c2)
{
    return (c1[0] == c2.r && c1[1] == c2.g && c1[2] == c2.b /* && c2.a == 0xff */);
}

//
//
int getColorFromIndex( int i, int type )
{
    if( type == COLOR_SKIN )
    {
        return getColorFromVec3( *costume_get_current_origin_set()->skinColors[0]->color[i]);
    }
    else
    {
        return getColorFromVec3( *costume_get_current_origin_set()->
            bodyColors[0]->color[i]);
    }
}

//
//
int getIndexFromColor( Color color )
{
    int i;

    const ColorPalette **palette = costume_get_current_origin_set()->bodyColors;

    for( i = eaSize( (F32***)&palette[0]->color )-1; i >= 0; i-- )
    {
        if( compareColors(*palette[0]->color[i], color ) )
            return i;
    }

    return 0;
}

int getSkinIndexFromColor( Color color )
{
    int i;

    const ColorPalette **palette = costume_get_current_origin_set()->skinColors;

    for( i = eaSize( (F32***)&palette[0]->color )-1; i >= 0; i-- )
    {
        if( compareColors(*palette[0]->color[i], color ) )
            return i;
    }

    return 0;
}

//
//
static Color getColor( int i, int type )
{
    int clr = getColorFromIndex( i, type );
    int r = (clr & 0xff000000) >> 24;
    int g = (clr & 0x00ff0000) >> 16;
    int b = (clr & 0x0000ff00) >> 8;

       return CreateColorRGB( r, g, b );
}

static Color getSelectedColor(int colorType, int mode)
{
    Entity* e = playerPtr();
    const CostumeGeoSet* selectedGeoSet;

    if (colorType == COLOR_SKIN)
        return e->costume->appearance.colorSkin;

    selectedGeoSet = getSelectedGeoSet(mode);

    if (selectedGeoSet && selectedGeoSet->bodyPartName)
    {
        if (colorType == COLOR_1 || colorType == COLOR_2 ||
            colorType == COLOR_3 || colorType == COLOR_4)
        {
                int bpID = 
                    costume_PartIndexFromName(e, selectedGeoSet->bodyPartName);
                return e->costume->parts[bpID]->color[colorType - COLOR_1];
            }
        }
    else if (gColorsLinked)
    {
        if (colorType == COLOR_1)
            return gLinkedColor1;
        if (colorType == COLOR_2)
            return gLinkedColor2;
    }

    return ColorNull;
}

Color forceToPalette(Color color, int colorType)
{
    if (colorType == COLOR_SKIN)
        return getColor(getSkinIndexFromColor(color), colorType);

    return getColor(getIndexFromColor(color), colorType);
}

static void setSelectedColor(int colorType, Color color, int mode)
{
    Entity* e = playerPtr();
    Costume* mutable_costume = costume_as_mutable_cast(e->costume);
    const CostumeGeoSet* selectedGeoSet;

    if (colorType == COLOR_SKIN)
        mutable_costume->appearance.colorSkin = color;

    selectedGeoSet = getSelectedGeoSet(mode);

    if (gColorsLinked)
    {
        if (colorType == COLOR_1)
            gLinkedColor1 = color;
        else if (colorType == COLOR_2)
            gLinkedColor2 = color;
    }
    else if (selectedGeoSet)
    {
        int bpID;
        switch (colorType)
        {
        case COLOR_1: case COLOR_2: case COLOR_3: case COLOR_4:
            if (selectedGeoSet->bodyPartName)
            {
                bpID = costume_PartIndexFromName(e, selectedGeoSet->bodyPartName);
                mutable_costume->parts[bpID]->color[colorType - COLOR_1] = color;
                
                if (selectedGeoSet->colorLinked)
                {
                    bpID = costume_PartIndexFromName(e, selectedGeoSet->colorLinked);
                    mutable_costume->parts[bpID]->color[colorType - COLOR_1] = color;
                }
            }
        }
        if ((mode == COSTUME_EDIT_MODE_NPC) ||  (mode == COSTUME_EDIT_MODE_NPC_NO_SKIN))
        {
            switch (colorType)
            {
            case COLOR_1:
                bpID = costume_PartIndexFromName(e, selectedGeoSet->bodyPartName);
                mutable_costume->parts[bpID]->color[COLOR_3 - COLOR_1] = color;
                break;
            case COLOR_2:
                bpID = costume_PartIndexFromName(e, selectedGeoSet->bodyPartName);
                mutable_costume->parts[bpID]->color[COLOR_4 - COLOR_1] = color;
                break;
            }
        }
    }
}

//
//
static void copyLinkedColorIntoAllOthers( Entity *e )
{
    int i;
    Costume* mutable_costume = costume_as_mutable_cast(e->costume);

    // Copy the current bone's colors into all the rest
    for(i=0; i < e->costume->appearance.iNumParts; i++)
    {
        costume_PartSetColor(mutable_costume, i, 0, gLinkedColor1);
        costume_PartSetColor(mutable_costume, i, 1, gLinkedColor2);
        costume_PartSetColor(mutable_costume, i, 2, gLinkedColor1);
        costume_PartSetColor(mutable_costume, i, 3, gLinkedColor2);
    }

}

static void updateColors()
{
    Entity *e = playerPtr();

    if (gColorsLinked)
        copyLinkedColorIntoAllOthers( e );

    costume_Apply( e );
}

enum
{
    CP_DOWN,
    CP_UP,
};

#define PALETTE_X       450
#define PALETTE_WD      10
#define PALETTE_SPACEX   25
#define PALETTE_SPACEY   25
//
//

static bool equalPalette(const ColorPalette* p1, const ColorPalette* p2) {
    int i, paletteSize;

    if (p1 == p2)
        return true;

    if (!p1 || !p2)
        return false;

    paletteSize = eaSize((F32***)&p1->color);

    if (paletteSize != eaSize((F32***)&p2->color))
        return false;

    for (i = 0; i < paletteSize; ++i) {
        if (getColorFromVec3(*p1->color[i]) != getColorFromVec3(*p2->color[i]))
            return false;
    }

    return true;
}

Color drawColorPalette( F32 cx, F32 y, F32 z, float scale, const ColorPalette* newPalette, Color selectedColor, bool anyColorSelected )
{
    int i, size;
    static const ColorPalette *palette;
    static const ColorPalette *last_palette;
    static int mode = CP_DOWN;
    static F32 counter = 0;
    Color result = ColorNull;
    static lastHoveredColor = -1;
    F32 xposSc, yposSc;
    calculatePositionScales(&xposSc, &yposSc);

    if(mode != CP_DOWN && !equalPalette(last_palette, newPalette))
    {
        last_palette = newPalette;
        mode = CP_DOWN;
    }

    if( mode == CP_DOWN )
    {
        counter = stateScale(counter, 0);
        if( counter <= 0.f )
        {
            palette = newPalette;
            mode = CP_UP;
        }
    }

    size = palette?eaSize((F32***)&palette->color):0;

    if( mode == CP_UP )
    {
        counter = stateScale(counter, 1);
    }

    if ( palette )
    {
        int hovered = -1;
         for( i = 0; i < size; i++ )
        {
              float wd = PALETTE_SPACEX*(PALETTE_WD-1);
             int clr = getColorFromVec3( *palette->color[i] ) & (int)( 0xffffff00 | (int)(255*counter*scale));
               F32 tx  = cx - (wd/2 - (i%PALETTE_WD)*PALETTE_SPACEX*PALETTE_SCALE)*xposSc;
             F32 ty  = y + ((i/PALETTE_WD)*PALETTE_SPACEY*scale*PALETTE_SCALE)*yposSc;

            int selected = anyColorSelected &&     compareColors(*palette->color[i], selectedColor);
            int colorInteract = drawColorRing( tx, ty, 5000, selected, clr, 1.f, scale, scale >= 1.f);
            if( D_MOUSEOVER <= colorInteract )
            {
                hovered = i;
            }
            if( D_MOUSEHIT == colorInteract )
            {
                if (mode != CP_DOWN)
                {
                    result.integer = clr;
                    result = colorFlip(result);
                    sndPlay( "N_SelectSmall", SOUND_GAME );
                }
            }
        }
        if (lastHoveredColor != hovered)
        {
            lastHoveredColor = hovered;
            if (hovered > -1)
            {
                //new hovered color, play sound
                sndPlay("N_PopHover", SOUND_GAME);
            }
        }
    }
    return result;
}

static bool preserveSpecialCostumePart(Entity *e, const cCostume *costume, int idx)
{
    bool retval = false;
    const BasePowerSet *crab_primary = NULL;
    const BasePowerSet *crab_secondary = NULL;

    crab_primary = powerdict_GetBasePowerSetByName( &g_PowerDictionary,
        "Arachnos_Soldiers", "Crab_Spider_Soldier" );
    crab_secondary = powerdict_GetBasePowerSetByName( &g_PowerDictionary,
        "Training_Gadgets", "Crab_Spider_Training" );

    // People with crab spider powersets have the backpack locked in place.
    if( ( crab_primary && character_OwnsPowerSetInAnyBuild( e->pchar, crab_primary ) ) ||
        ( crab_secondary && character_OwnsPowerSetInAnyBuild( e->pchar, crab_secondary ) ) )
    {
        // Whatever's placed on the back gets to stay there - that's where
        // the backpack is locked.
        if( idx >= 0 && idx < GetBodyPartCount() &&
            stricmp( bodyPartList.bodyParts[idx]->name, "Back" ) == 0 )
        {
            retval = true;
        }
    }

    return retval;
}

//----------------------------------------------------------------------------------------------------------------
// Costume Control ///////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------------
// The functions that actually change the costume go here

void setHideStorePiecesState(HideStorePiecesStates state)
{
    hideStorePiecesState = state;
}

HideStorePiecesStates getHideStorePiecesState(void)
{
    return hideStorePiecesState;
}

static void confirmHideStorePiecesState(void * data)
{
    resetCostume(1);
    hideStorePiecesState = HIDE_STORE_PIECES_CHECKED;
}

//determine whether this part is visible in the character creator/tailor for a specific character
static int costumeCanSeeThisPart( const char ** costumeKeys, const char * productCode, const char ** internalProducts, bool mustHaveStoreProduct )
{
    int hasCostumeKey = costumeKeys != NULL;
    int hasProductCode = productCode != NULL;
    int hasInternalProducts = internalProducts != NULL;

    if (hasCostumeKey || hasProductCode || hasInternalProducts)
    {
        AccountInventorySet *invSet = inventoryClient_GetAcctInventorySet();
        int playerMissingKey = !costumereward_findall(costumeKeys, getPCCEditingMode());
        int playerMissingCode;
        int playerMissingInternalCode = internalProducts ? !AccountHasInternalProductCode(internalProducts, invSet) : 0;
        
        if (productCode)
        {
            playerMissingCode = !AccountHasStoreProduct(invSet, skuIdFromString(productCode));
            // player who doesn't have the code can still be shown store pieces if
            // 1) is it not required by the function call
            // 2) hide store pieces is not checked
            // 3) it is published
            if (!mustHaveStoreProduct && hideStorePiecesState == HIDE_STORE_PIECES_UNCHECKED)
            {
                playerMissingCode &= !accountCatalogIsSkuPublished(skuIdFromString(productCode));
            }
        }
        else
        {
            playerMissingCode = 0;
        }
        
        

        return (hasCostumeKey && !playerMissingKey) || (hasProductCode && !playerMissingCode) || (hasInternalProducts && !playerMissingInternalCode);
    }
    else
    {
        return true;
    }
}

void updateCostumeEx(bool resetSets)
{
    Entity *e = playerPtr();
    int i, j, k, keyed = 0, boneId[MAX_BODYPARTS] = {0};

    const CostumeOriginSet    *cset = costume_get_current_origin_set();
    const CostumeBoneSet      *bset;
    Costume* mutable_costume = costume_as_mutable_cast(e->costume);
    int region_total = REGION_TOTAL_DEF;
    const char *hasCloth = NULL;

    mutable_costume->containsRestrictedStoreParts = false;

    if (region_total > eaSize(&gCostumeMaster.bodySet[gCurrentBodyType]->originSet[gCurrentOriginSet]->regionSet))
        region_total=eaSize(&gCostumeMaster.bodySet[gCurrentBodyType]->originSet[gCurrentOriginSet]->regionSet);

    // first mark all the parts the player has
    assert(e->costume->appearance.iNumParts <= GetBodyPartCount());
    for( i = 0; i < e->costume->appearance.iNumParts; i++ )
        boneId[i] = 1;

    // now for each region, see what is using those parts
    for( j = 0; j < region_total; j++ )
    {
        // skip regions we can't access
        if( !game_state.editnpc )
        {
//            if( !e->pl->glowieUnlocked && stricmp( cset->regionSet[j]->name, "Special" ) == 0 )
//            {
//                continue;
//            }

            if(!costumereward_findall(cset->regionSet[j]->keys, getPCCEditingMode() ))
                continue;

            for(i = eaSize(&cset->regionSet[j]->boneSet)-1; i >= 0; --i)
            {
                bset = cset->regionSet[j]->boneSet[i];
                if(costumereward_findall(bset->keys, getPCCEditingMode()))
                {
                    for(k = eaSize(&bset->geo)-1; k >= 0; --k)
                        if(costumereward_findall(bset->geo[k]->keys, getPCCEditingMode()))
                            break;
                    if(k >= 0)
                        break;
                }
            }
            if(i < 0) // all bonesets are keyed, ignore this region
            {
                continue;
            }
        }

        bset = cset->regionSet[j]->boneSet[cset->regionSet[j]->currentBoneSet];
        keyed |= isInvalidPart(NULL, getPCCEditingMode(), true, true, inventoryClient_GetAcctInventorySet(), COSTUME_LOCKS(bset));

        for( i = eaSize( &bset->geo )-1; i >= 0; i-- )
        {
            const CostumeGeoSet *gset = bset->geo[i];
            int bpID = -1;

            // HYBRID: Don't need to check for validity of store here, costume should have been validated elsewhere before getting to here.
            if(!game_state.editnpc && !costumereward_findall(gset->keys, getPCCEditingMode()))
            {
                continue;
            }

            // Crab Spiders cannot wear things on these body locations if
            // they are not Crab Spider-specific
            if(    !game_state.editnpc &&
                costumereward_find("CrabBackpack", getPCCEditingMode() ) && 
                ( !eaSize(&gset->keys) || StringArrayFind(gset->keys, "CrabBackpack") < 0 ) &&
                gset->bodyPartName &&
                (    stricmp( gset->bodyPartName, "Back" ) == 0 ||
                    stricmp( gset->bodyPartName, "Cape" ) == 0 ||
                    stricmp( gset->bodyPartName, "Broach" ) == 0 ||
                    stricmp( gset->bodyPartName, "Collar" ) == 0 )
                )
            {
                continue;
            }

            if( gset->bodyPartName )
            {
                 bpID = costume_PartIndexFromName( e, gset->bodyPartName );
            }
            if( bpID >= 0 )
            {
                int current = gset->currentInfo;

                // if we've already set this piece to something meaningful, and it's not the currently selected piece, skip it
                if( !gset->isOpen && !boneId[bpID] &&
                    !costume_isPartEmpty(e->costume->parts[bpID]) )
                {
                    continue;
                }

                boneId[bpID] = FALSE;

                // don't bother updating the supergroup emblem in the supergroups screen, because its
                // super hacked
             //    if( (isMenu(MENU_SUPER_REG) || isMenu(MENU_SUPERCOSTUME)) && 
                //    stricmp( gset->bodyPartName, "ChestDetail" ) == 0 )
                //{
                //    continue;
                //}

                // check for cloth constraints
                if (costume_isRestrictedPiece(bset->flags, "cloth") || 
                    costume_isRestrictedPiece(gset->flags, "cloth") || 
                    costume_isRestrictedPiece(gset->info[current]->flags, "cloth"))
                {
                    if (hasCloth && stricmp(hasCloth, bset->name) != 0)
                    {
                        boneId[bpID] = TRUE;
                    } else {
                        hasCloth = bset->name;
                    }
                }

                keyed |= !costumeCanSeeThisPart(COSTUME_LOCKS(gset->info[current]), false);

                if (!skuIdIsNull(SELLABLE_PRODUCT(gset->info[current])))
                    mutable_costume->containsRestrictedStoreParts = true;

                costume_PartSetPath( mutable_costume, bpID, gset->displayName, cset->regionSet[j]->name, bset->name );
                costume_PartSetGeometry( mutable_costume, bpID, gset->info[current]->geoName);
                costume_PartSetTexture1( mutable_costume, bpID, gset->info[current]->texName1);
                costume_PartSetFx( mutable_costume, bpID, gset->info[current]->fxName);
                if(gset->numColor == 2)
                    costume_PartForceTwoColor(mutable_costume, bpID);

                if( !gset->info[current]->texName2 || gset->info[current]->texMasked || stricmp( gset->info[current]->texName2, "Mask" ) == 0 )
                {
                    if( eaSize( &gset->mask ) )
                      {
                        int mask = gset->currentMask;
                        if( mask >= 0 )    
                        {
                            costume_PartSetTexture2( mutable_costume, bpID, gset->mask[mask]->name );
                            if (!skuIdIsNull(SELLABLE_PRODUCT(gset->mask[mask])))
                                mutable_costume->containsRestrictedStoreParts = true;

                            keyed |= isInvalidPart(NULL, getPCCEditingMode(), true, true, inventoryClient_GetAcctInventorySet(), COSTUME_LOCKS(gset->mask[mask]));
                        }
                        else
                            costume_PartSetTexture2( mutable_costume, bpID, gset->info[current]->texName2 );
                    }
                    else if( gset->info[current]->texName2 )
                        costume_PartSetTexture2( mutable_costume, bpID, gset->info[current]->texName2 );
                }
                else
                    costume_PartSetTexture2( mutable_costume, bpID, gset->info[current]->texName2 );
            }
        }
    }

    // ok now go clear anything that is not being used
     for( i = 0; i < ARRAY_SIZE(boneId); i++ )
    {
        if( boneId[i] && !preserveSpecialCostumePart( e, e->costume, i ) )
        {
            Costume* mutable_costume = costume_as_mutable_cast(e->costume);

            costume_PartSetPath( mutable_costume, i, NULL, NULL, NULL );
            costume_PartSetGeometry( mutable_costume, i, "None" );
            costume_PartSetTexture1( mutable_costume, i, "None" );
            costume_PartSetTexture2( mutable_costume, i, "None" );
            costume_PartSetFx( mutable_costume, i, "None" );
        }
    }

    updateColors();
    costume_Apply( e );

     if( keyed && isMenu(MENU_COSTUME) && !game_state.editnpc )
        resetCostume(0);

    if(resetSets)
        gSelectedCostumeSet = -1;
}

void updateCostumeFromSelectionSet( StashTable costumeSetSelections )
{
    Entity *e = playerPtr();
    int i, j, k, keyed = 0, boneId[MAX_BODYPARTS] = {0};

    const CostumeOriginSet    *cset = costume_get_current_origin_set();
    const CostumeBoneSet      *bset;
    int region_total = REGION_TOTAL_DEF;

    if (region_total > eaSize(&cset->regionSet))
        region_total=eaSize(&cset->regionSet);

    // first mark all the parts the player has
    assert(e->costume->appearance.iNumParts <= GetBodyPartCount());
    for( i = 0; i < e->costume->appearance.iNumParts; i++ )
        boneId[i] = 1;

    // now for each region, see what is using those parts
    for( j = 0; j < region_total; j++ )
    {
        const CostumeRegionSet* rset = cset->regionSet[j];
        int selected_bone_set = 0;

        // skip regions we can't access
        if( !game_state.editnpc )
        {
            if(!costumereward_findall(rset->keys, getPCCEditingMode() ))
                continue;

            for(i = eaSize(&rset->boneSet)-1; i >= 0; --i)
            {
                bset = rset->boneSet[i];
                if(costumereward_findall(bset->keys, getPCCEditingMode()))
                {
                    for(k = eaSize(&bset->geo)-1; k >= 0; --k)
                        if(costumereward_findall(bset->geo[k]->keys, getPCCEditingMode()))
                            break;
                    if(k >= 0)
                        break;
                }
            }
            if(i < 0) // all bonesets are keyed, ignore this region
            {
                continue;
            }
        }

        if (!stashFindInt(costumeSetSelections, rset, &selected_bone_set))    // retrieve selection for this set
        {
            selected_bone_set = 0;
        }

        bset = rset->boneSet[selected_bone_set];
        keyed |= !costumereward_findall(bset->keys, getPCCEditingMode());

        for( i = eaSize( &bset->geo )-1; i >= 0; i-- )
        {
            const CostumeGeoSet *gset = bset->geo[i];
            int bpID = -1;

            if(!game_state.editnpc && !costumereward_findall(gset->keys, getPCCEditingMode()))
            {
                continue;
            }

            // Crab Spiders cannot wear things on these body locations if
            // they are not Crab Spider-specific
            if(    !game_state.editnpc &&
                costumereward_find("CrabBackpack", getPCCEditingMode() ) && 
                ( !eaSize(&gset->keys) || StringArrayFind(gset->keys, "CrabBackpack") < 0 ) &&
                gset->bodyPartName &&
                (    stricmp( gset->bodyPartName, "Back" ) == 0 ||
                stricmp( gset->bodyPartName, "Cape" ) == 0 ||
                stricmp( gset->bodyPartName, "Broach" ) == 0 ||
                stricmp( gset->bodyPartName, "Collar" ) == 0 )
                )
            {
                continue;
            }

            if( gset->bodyPartName )
            {
                bpID = costume_PartIndexFromName( e, gset->bodyPartName );
            }
            if( bpID >= 0 )
            {
                Costume* mutable_costume = costume_as_mutable_cast(e->costume);
                int selected_gset_info = 0;
                int selected_gset_mask = 0;

                if (stashFindInt(costumeSetSelections, gset, &selected_gset_info))    // retrieve selection for this set
                {
                    selected_gset_mask = selected_gset_info >> 16;
                    selected_gset_info &= 0xFFFF;
                }

                // if we've already set this piece to something meaningful, and it's not the currently selected piece, skip it
                if( !gset->isOpen && !boneId[bpID] &&
                    !costume_isPartEmpty(e->costume->parts[bpID]) )
                {
                    continue;
                }

                boneId[bpID] = FALSE;

                // don't bother updating the supergroup emblem in the supergroups screen, because its
                // super hacked
                //    if( (isMenu(MENU_SUPER_REG) || isMenu(MENU_SUPERCOSTUME)) && 
                //    stricmp( gset->bodyPartName, "ChestDetail" ) == 0 )
                //{
                //    continue;
                //}

                keyed |= !costumereward_findall(gset->info[selected_gset_info]->keys, getPCCEditingMode());


                costume_PartSetPath( mutable_costume, bpID, gset->displayName, rset->name, bset->name );
                costume_PartSetGeometry( mutable_costume, bpID, gset->info[selected_gset_info]->geoName);
                costume_PartSetTexture1( mutable_costume, bpID, gset->info[selected_gset_info]->texName1);
                costume_PartSetFx( mutable_costume, bpID, gset->info[selected_gset_info]->fxName);
                if(gset->numColor == 2)
                    costume_PartForceTwoColor(mutable_costume, bpID);

                if( !gset->info[selected_gset_info]->texName2 || gset->info[selected_gset_info]->texMasked || stricmp( gset->info[selected_gset_info]->texName2, "Mask" ) == 0 )
                {
                    if( eaSize( &gset->mask ) )
                    {
                        int mask = selected_gset_mask;
                        if( mask >= 0 )    
                            costume_PartSetTexture2( mutable_costume, bpID, gset->mask[mask]->name );
                        else
                            costume_PartSetTexture2( mutable_costume, bpID, gset->info[selected_gset_info]->texName2 );
                    }
                    else if( gset->info[selected_gset_info]->texName2 )
                        costume_PartSetTexture2( mutable_costume, bpID, gset->info[selected_gset_info]->texName2 );
                }
                else
                    costume_PartSetTexture2( mutable_costume, bpID, gset->info[selected_gset_info]->texName2 );
            }
        }
    }

    // ok now go clear anything that is not being used
    for( i = 0; i < ARRAY_SIZE(boneId); i++ )
    {
        if( boneId[i] && !preserveSpecialCostumePart( e, e->costume, i ) )
        {
            Costume* mutable_costume = costume_as_mutable_cast(e->costume);

            costume_PartSetPath( mutable_costume, i, NULL, NULL, NULL );
            costume_PartSetGeometry( mutable_costume, i, "None" );
            costume_PartSetTexture1( mutable_costume, i, "None" );
            costume_PartSetTexture2( mutable_costume, i, "None" );
            costume_PartSetFx( mutable_costume, i, "None" );
        }
    }

    costume_Apply( e );
}

static void pickRandomCostumeSets()
{
    int i, k;
    //verify array is empty
    resetRandomCostumes();

    for (i = 0; i < 3; ++i)  //3 covers male/female/huge
    {
        for (k = 0; k < eaSize(&gCostumeMaster.bodySet[i]->originSet); ++k)
        {
            const CostumeOriginSet *oset = gCostumeMaster.bodySet[i]->originSet[k];
            int * validCostumeSets;
            int j, numValidCostumeSets;

            eaiCreate(&validCostumeSets);
            //create a list of valid costume sets
            for (j = eaSize(&oset->costumeSets) - 1; j >= 0; --j)
            {
                if (!(costumesetIsRestricted(oset->costumeSets[j]) || costumesetIsLocked(oset->costumeSets[j])))
                {
                    eaiPush(&validCostumeSets, j);
                }
            }

            numValidCostumeSets = eaiSize(&validCostumeSets);

            if (numValidCostumeSets > 0)
            {
                eaiPush(&initRandomCostumeSet[i], validCostumeSets[rand()%numValidCostumeSets]);
            }
            else
            {
                eaiPush(&initRandomCostumeSet[i], -1);
            }

            eaiDestroy(&validCostumeSets);
        }
    }
}

//forcePickNewRandomCostumeSets - will pick new costume sets even if some have been selected already
static void applyRandomCostumeSet(int forcePickNewRandomCostumeSets)
{
    //check if random costumes have been initialized
    if (!initRandomCostumeSet[gCurrentBodyType] || forcePickNewRandomCostumeSets)
    {
        //initialize them
        pickRandomCostumeSets();
    }

    //use random costume set if this is the first time using this body type/ origin set
    if (initRandomCostumeSet[gCurrentBodyType][gCurrentOriginSet] >= 0)
    {
        const CostumeOriginSet *oset = gCostumeMaster.bodySet[gCurrentBodyType]->originSet[gCurrentOriginSet];
        applyCostumeSet(oset, initRandomCostumeSet[gCurrentBodyType][gCurrentOriginSet]);
        initRandomCostumeSet[gCurrentBodyType][gCurrentOriginSet] = -1;
    }
}

//Sets the default costume if one hasn't been selected already.
//useRandomCostumeSetAsDefault - will use a random costume set if it hasn't been used already
//
void setDefaultCostume(int useRandomCostumeSetAsDefault)
{
    int i, j;
    Entity *e = playerPtr();

    char * genderName[] = { "Female", "Male", "Huge", "BasicMale", "BasicFemale", "Villain", "Villain2", "Villain3"  };
    const CostumeOriginSet *cSet = 0;

    // set the bodyType index and origin index
    for( i = eaSize( &gCostumeMaster.bodySet )-1; i >= 0; i-- )
    {
        if ( stricmp( gCostumeMaster.bodySet[i]->name, genderName[gCurrentGender] ) == 0 )
        {
            int found = 0;
            gCurrentBodyType = i;
            
            if (e->pchar->pclass && (gFakeArachnosSoldier || class_MatchesSpecialRestriction(e->pchar->pclass, "ArachnosSoldier")) && !e->pl->current_costume)
            {
                // look for arachnos soldier first
                for( j = eaSize( &gCostumeMaster.bodySet[i]->originSet )-1; j >= 0 ; j-- )
                {
                    if(stricmp( gCostumeMaster.bodySet[i]->originSet[j]->name, "ArachnosSoldier" ) == 0)
                    {
                        gCurrentOriginSet = j;
                        found = 1;
                    }
                }
            }

            // look for arachnos widow next
            if(!found)
            {
                if (e->pchar->pclass && (gFakeArachnosWidow || class_MatchesSpecialRestriction(e->pchar->pclass, "ArachnosWidow")) && !e->pl->current_costume)
                {
                    for( j = eaSize( &gCostumeMaster.bodySet[i]->originSet )-1; j >= 0 ; j-- )
                    {
                        if( stricmp( gCostumeMaster.bodySet[i]->originSet[j]->name, "ArachnosWidow" ) == 0)
                        {
                            gCurrentOriginSet = j;
                            found = 1;
                        }
                    }
                }
            }

            // didn't find
            if(!found)
            {
                for( j = eaSize( &gCostumeMaster.bodySet[i]->originSet )-1; j >= 0; j-- )
                {
                    if( stricmp( gCostumeMaster.bodySet[i]->originSet[j]->name, "All" ) == 0 )
                    {
                        gCurrentOriginSet = j;
                        break;
                    }
                }
            }
            break;
        }
    }

    if (useRandomCostumeSetAsDefault)
    {
        applyRandomCostumeSet(false);
    }

    //ok now load defaults
    //--------------------------------------------------------------------------
    updateCostume();
}

bool bonesetIsLegacy( const CostumeBoneSet *bset )
{
    return bset->legacy;
}

bool geosetIsLegacy( const CostumeGeoSet *gset )
{
    return gset->legacy;
}

static bool addGeosetStoreParts(const CostumeGeoSet *gset, ShoppingCart * storePartsContained)
{
    bool retval = false;
    if (gset && EAINRANGE(gset->currentInfo, gset->info) && gset->bodyPartName)
    {
        SkuId product = SELLABLE_PRODUCT(gset->info[gset->currentInfo]);
        if (!skuIdIsNull(product))
        {
            storePartsContained->items[storePartsContained->itemCount++] = product;
            retval = true;
        }
        if (EAINRANGE(gset->currentMask, gset->mask))
        {
            SkuId product = SELLABLE_PRODUCT(gset->mask[gset->currentMask]);
            if (!skuIdIsNull(product))
            {
                storePartsContained->items[storePartsContained->itemCount++] = product;
                retval = true;
            }
        }
    }
    return retval;
}

static bool addBonesetStoreParts(const CostumeBoneSet *bset, ShoppingCart * storePartsContained)
{
    bool retval = false;
    if (bset)
    {
        SkuId  product = SELLABLE_PRODUCT(bset);
        if (!skuIdIsNull(product))
        {
            storePartsContained->items[storePartsContained->itemCount++] = product;
            retval = true;
        }
    }
    return retval;
}

bool partIsLegacy( const CostumeTexSet *tset )
{
    return tset->legacy;
}

bool maskIsLegacy( const CostumeMaskSet *mask )
{
    return mask->legacy;
}

bool gIgnoreLegacyForSet;
bool costume_weddingPartSelected();

// outputs the number of not-restricted bsets in *good_bsets
// when *good_bsets is not NULL, if the only not-restricted bset is "None", the region is considered restricted
bool regionsetIsRestricted( const CostumeRegionSet *rset, int *good_bsets)
{
    if(game_state.editnpc) // || playerPtr()->access_level)
    {
        if(good_bsets)
            *good_bsets = eaSize(&rset->boneSet);
        return false;
    }

    if(!costumereward_findall(rset->keys, getPCCEditingMode()))
        return true;

    if(eaSize(&rset->boneSet))
    {
        int i;
        int found_something = 0;
        if(good_bsets)
            *good_bsets = 0;
        for(i = eaSize(&rset->boneSet)-1; i >= 0; --i) // if all of a regionset's bonesets are restricted, then so is the regionset
        {
            if(!bonesetIsRestricted(rset->boneSet[i]))
            {
                if(!good_bsets)
                    break;

                (*good_bsets)++;
                if(stricmp(rset->boneSet[i]->name, "None") != 0)
                    found_something = 1;
            }
        }
        if(good_bsets ? !found_something : i < 0)
            return true;
    }

    return false;
}

static bool costume_isOtherRestrictedPieceSelectedExcludingRegionset(const char *currentPart, const char *restriction, const CostumeRegionSet * rset);

static bool bonesetIsRestrictedEx( const CostumeBoneSet *bset, int bSkipChecksAgainstCurrentRegionset, int mustHaveStoreProduct )
{
    int tailor = isMenu(MENU_TAILOR);

    if(game_state.editnpc) // || playerPtr()->access_level )
        return false;

    if(!costumeCanSeeThisPart(COSTUME_LOCKS(bset), mustHaveStoreProduct))
    {
        return true;
    }

    // check if this category has a single valid item
    if (hideStorePiecesState == HIDE_STORE_PIECES_CHECKED)
    {
        if (bset->geoCount)
        {
            int i;
            int found_something = 0;
            for (i = 0; i < bset->geoCount; ++i)
            {
                //In general, if 1 of the items of the geoset has a key/product, we hide this boneset completely.
                //However, for weapons, since the system is driven by costume keys, we have this exception.
                //Weapons have costume keys, but no store product code, I hope.
                int keyOnly = bset->geo[i]->keys && !bset->geo[i]->storeProduct;
                if (geosetIsRestricted(bset->geo[i]))
                {
                    if (!keyOnly)
                    {
                        return true;
                    }
                }
                else
                {
                    found_something = 1;
                }
            }
            if (!found_something)
            {
                return true;
            }
        }
    }

    if( !gIgnoreLegacyForSet && bonesetIsLegacy(bset) &&
        (!tailor || bset->rsetParent->origBoneSet != bset->idx) ) // legacy piece we don't own
    {
        return true;
    }

    if(bset->restricted) // artist screwup
        return true;

    if (costume_isRestrictedPiece(bset->flags, "cloth"))
    {
        if (bSkipChecksAgainstCurrentRegionset) 
        {
            if (costume_isOtherRestrictedPieceSelectedExcludingRegionset( bset->name, "cloth", bset->rsetParent ))
            {
                return true;
            }
        }
        else
        {
            if (costume_isOtherRestrictedPieceSelected( bset->name, "cloth" ))
            {
                return true;
            }
        }
    }

    // Any boneset that has Crab Spider Backpacks in it is valid for us if we
    // have crab backpacks enabled, and invalid if not
    if(eaSize(&bset->keys) && StringArrayFind(bset->keys, "CrabBackpack") >= 0)
        return false; // (would have returned true above if the key wasn't found)


    if(eaSize(&bset->geo))
    {
        int i;
        for(i = eaSize(&bset->geo)-1; i >= 0; --i) // if all of a boneset's geosets are restricted, then so is the boneset
            if(!geosetIsRestricted(bset->geo[i])) // TODO: we may want to ignore "None" bonesets
                break;
        if(i < 0)
            return true;
    }
    //special case for crab backpack so "none" won't show up for back details
    else if(strcmp(bset->rsetParent->name, "Capes") == 0 && costumereward_find("CrabBackpack", getPCCEditingMode()) && strcmp(bset->name, "None") == 0)
    {
        return true;
    }

    return false;
}

bool bonesetIsRestricted( const CostumeBoneSet *bset )
{
    return bonesetIsRestrictedEx(bset, false, false);
}

bool bonesetIsRestricted_SkipChecksAgainstCurrentRegionSet( const CostumeBoneSet *bset )
{
    return bonesetIsRestrictedEx(bset, true, false);
}

static bool bonesetIsRestricted_MustHaveStoreProduct( const CostumeBoneSet *bset )
{
    return bonesetIsRestrictedEx(bset, false, true);
}

bool geosetIsRestricted( const CostumeGeoSet *gset )
{
     int tailor = isMenu(MENU_TAILOR);

    if(game_state.editnpc) // || playerPtr()->access_level )
        return false;

    if(gset->isHidden) // intentionally hidden to make things work (cape harness)
        return true;

    if(!costumeCanSeeThisPart(COSTUME_LOCKS(gset), false))
    {
        return true;
    }

    // check if this category has a single valid item
    if (hideStorePiecesState == HIDE_STORE_PIECES_CHECKED && gset->infoCount)
    {
        int i;
        int found_something = 0;
        for (i = 0; i < gset->infoCount; ++i)
        {
            if (!partIsRestricted(gset->info[i]))
            {
                found_something = 1;
                break;
            }
        }
        if (!found_something)
        {
            return true;
        }
    }

    if( !gIgnoreLegacyForSet && geosetIsLegacy(gset) &&
        (!tailor || gset->bsetParent->origSet != gset->idx) ) // legacy piece I don't own
    {
        return true;
    }

    if (costume_isRestrictedPiece(gset->flags, "cloth") && 
        gset->bsetParent && 
        costume_isOtherRestrictedPieceSelected( gset->bsetParent->name, "cloth" ))
        return true;

    if(gset->restricted) // there are no valid textsets or mask sets for this geoset
        return true;


    // Anything that gets in the way of a crab backpack is blocked
    if(    costumereward_find("CrabBackpack", getPCCEditingMode()) && 
        ( !eaSize(&gset->keys) || StringArrayFind(gset->keys, "CrabBackpack") < 0 ) &&
        gset->bodyPartName &&
        (
            stricmp(gset->bodyPartName, "Back") == 0 ||
            stricmp(gset->bodyPartName, "Cape") == 0 ||
            stricmp(gset->bodyPartName, "Broach") == 0 ||
            stricmp(gset->bodyPartName, "Collar") == 0 )
        )
    {
        return true;
    }

    return false;
}
//
//
static bool partIsRestrictedEx( const CostumeTexSet *tset, const char *error_on, int bSkipChecksAgainstCurrentCostume, int mustHaveStoreProduct )
{
//    Entity * e = playerPtr();
    int tailor = isMenu(MENU_TAILOR);

    if( game_state.editnpc ) // || e->access_level )
        return false;

    if(isProductionMode())
        error_on = NULL;

    if( tset->devOnly )
    {
        if(error_on)
            ErrorFilenamef(error_on, "%s->%s->%s->%s is marked devOnly", tset->gsetParent->bsetParent->rsetParent->displayName, tset->gsetParent->bsetParent->displayName, tset->gsetParent->displayName, tset->displayName);
        return true; 
    }

    if(!costumeCanSeeThisPart(COSTUME_LOCKS(tset), mustHaveStoreProduct))
    {
        if(error_on)
        {
            ErrorFilenamef(error_on, "%s->%s->%s->%s is keyed or is store product that is not available", tset->gsetParent->bsetParent->rsetParent->displayName, tset->gsetParent->bsetParent->displayName, tset->gsetParent->displayName, tset->displayName);
        }
        return true;
    }

    if (!bSkipChecksAgainstCurrentCostume &&
        costume_isRestrictedPiece(tset->flags, "cloth") && 
        tset->gsetParent && 
        tset->gsetParent->bsetParent &&
        costume_isOtherRestrictedPieceSelected( tset->gsetParent->bsetParent->name, "cloth" ))
        return true;

    if(!gIgnoreLegacyForSet &&  partIsLegacy(tset) && (!tailor || tset->gsetParent->origInfo != tset->idx ) )
    {
        if(error_on)
            ErrorFilenamef(error_on, "%s->%s->%s->%s is marked legacy", tset->gsetParent->bsetParent->rsetParent->displayName, tset->gsetParent->bsetParent->displayName, tset->gsetParent->displayName, tset->displayName);
        return true;
    }

    if (costume_partHasBadDependencyPair(tset))
    {
        return true;
    }

    if( tset->levelReq )
    {
        if( playerPtr()->pchar->iLevel < tset->levelReq )
        {
            if(error_on)
                ErrorFilenamef(error_on, "%s->%s->%s->%s is min level %d", tset->gsetParent->bsetParent->rsetParent->displayName, tset->gsetParent->bsetParent->displayName, tset->gsetParent->displayName, tset->displayName, tset->levelReq);
            return true;
        }
    }

    return false;
}

bool partIsRestricted( const CostumeTexSet *tset )
{
    return partIsRestrictedEx(tset, NULL, false, false);
}
bool partIsRestricted_SkipChecksAgainstCurrentCostume( const CostumeTexSet *tset )
{
    return partIsRestrictedEx(tset, NULL, true, false);
}
static bool partIsRestricted_MustHaveStoreProduct( const CostumeTexSet *tset )
{
    return partIsRestrictedEx(tset, NULL, false, true);
}

// this should return NULL if the player owns the product or if the item is not store locked.
SkuId isSellableProduct( const char ** costumeKeys, const char * productCode, const char ** internalProductCodes )
{
    int playerMissingKey = !costumereward_findall(costumeKeys, getPCCEditingMode());
    int playerMissingInternalProductCode = !AccountHasInternalProductCode(internalProductCodes, inventoryClient_GetAcctInventorySet());

    if ( productCode != NULL && (costumeKeys == NULL || playerMissingKey) && (internalProductCodes == NULL || playerMissingInternalProductCode))
    {
        SkuId productId = skuIdFromString(productCode);
        return ( AccountHasStoreProduct( inventoryClient_GetAcctInventorySet(), productId ) ) ? kSkuIdInvalid : productId;
    }
    return kSkuIdInvalid;
}

static bool maskIsRestrictedEx( const CostumeMaskSet *mask, int mustHaveStoreProduct )
{
//    Entity * e = playerPtr();
    int tailor = isMenu(MENU_TAILOR);

    if(!mask)
        return false;

    if( game_state.editnpc ) // || e->access_level  )
        return false;

    if( mask->devOnly )
        return true;

    if(!costumeCanSeeThisPart(COSTUME_LOCKS(mask), mustHaveStoreProduct))
    {
        return true;
    }

    if(!gIgnoreLegacyForSet && maskIsLegacy(mask) && (!tailor || mask->gsetParent->origMask != mask->idx ) )
        return true;

    return false;
}

bool maskIsRestricted( const CostumeMaskSet *mask )
{
    return maskIsRestrictedEx(mask, false);
}
static bool maskIsRestricted_MustHaveStoreProduct( const CostumeMaskSet *mask )
{
    return maskIsRestrictedEx(mask, true);
}

bool costumesetIsRestricted( const CostumeSetSet *set)
{
    int i;
    const NPCDef *npc = set ? npcFindByName(set->npcName,NULL) : NULL;

    if(!npc)
        return true;

    if( game_state.editnpc )
    {
        return false;
    }

    // set can't be completely restricted if it can be bought
    if (set->storeProductList != NULL)
    {
        for(i = eaSize(&set->storeProductList)-1; i >= 0; --i)
        {
            //if they own this product, move on to the next code
            if (!AccountHasStoreProduct(inventoryClient_GetAcctInventorySet(), skuIdFromString(set->storeProductList[i])))
            {
                //check if this product can be bought now, otherwise it's restricted
                if (!accountCatalogIsSkuPublished(skuIdFromString(set->storeProductList[i])) || hideStorePiecesState == HIDE_STORE_PIECES_CHECKED)
                {
                    return true;
                }
            }
        }
        return false;
    }

    for(i = eaSize(&set->keys)-1; i >= 0; --i)
        if(!costumereward_find(set->keys[i], getPCCEditingMode() ))
            return true;

    return false;
}


bool costumesetIsLocked( const CostumeSetSet *set)
{
    int i;
    const NPCDef *npc = set ? npcFindByName(set->npcName,NULL) : NULL;
    AccountInventorySet *invSet = NULL;
    int isLocked = false;

    if(!npc)
        return isLocked;

    for(i = eaSize(&set->keys)-1; i >= 0 && !isLocked; --i)
    {
        if(!costumereward_find(set->keys[i], getPCCEditingMode() ))
        {
            isLocked = true;
        }
    }

    if (isLocked && set->storeProductList != NULL)
    {
        isLocked = false;
        invSet = inventoryClient_GetAcctInventorySet();
        for(i = eaSize(&set->storeProductList)-1; i >= 0 && !isLocked; --i)
        {
            if (!AccountHasStoreProduct(invSet, skuIdFromString(set->storeProductList[i])))
            {
                isLocked = true;
            }
        }
    }

    return isLocked;
}


static void changeTexInfo( CostumeGeoSet *gset, int forward );

static void changeFaceScaleInfo( CostumeGeoSet *gset, int forward )
{
    if( forward )
    {
        gset->currentInfo++;
        if( gset->currentInfo >= eaSize( &gset->faces ) )
            gset->currentInfo = 0;
    }
    else
    {
        gset->currentInfo--;
        if( gset->currentInfo < 0 )
            gset->currentInfo = eaSize(&gset->faces)-1;
    }

}

//----------------------------------------------------------------------------------------------------------------
// Costume UI Conrol Functions ///////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------------
//
#define RSET_SPEED      20
#define RSET_ENDSPACE   10
#define BS_SELECTOR_WD  280
#define BS_SELECTOR_Y   (32*LEFT_COLUMN_SQUISH)
//
#define GSET_MAX_WD     300
#define GSET_MIN_WD     250
#define GSET_SPEED      10
#define GSET_ENDSPACE   20

#define LINK_X            500
#define LINK_Y            456
#define PALETTE_Y       74
#define COLOR_BAR2_Y    40

static HybridElement colorTabHB[2] =
{
    { 0, 0, 0, 0, 0, 0, "colorframe_tab1_0", "colorframe_tab1_1", "colorframe_tab1_2"},
    { 1, 0, 0, 0, 0, 0, "colorframe_tab2_0", "colorframe_tab2_1", "colorframe_tab2_2" },
};

static void drawCostumeFrame(F32 x, F32 y, F32 z, F32 ysc, F32 ht, int selectedTab)
{
    AtlasTex * topFrame[] =
    {
        atlasLoadTexture("colorframe_top_0"),
        atlasLoadTexture("colorframe_top_1"),
        atlasLoadTexture("colorframe_top_2"),
    };
    AtlasTex * tabFrame[2][3] = 
    {
        {
            atlasLoadTexture("colorframe_tab1_0"),
            atlasLoadTexture("colorframe_tab1_1"),
            atlasLoadTexture("colorframe_tab1_2")
        },
        {
            atlasLoadTexture("colorframe_tab2_0"),
            atlasLoadTexture("colorframe_tab2_1"),
            atlasLoadTexture("colorframe_tab2_2")
        }
    };
    AtlasTex * vertFrame = atlasLoadTexture("colorframe_mid_0");
    AtlasTex * topBase = atlasLoadTexture("colorbase_top_0");
    AtlasTex * vertBase = atlasLoadTexture("colorbase_mid_0");    
    int alpha[2] = 
    {
        0xffffff00 | (int)((128.f + colorTabHB[0].percent*127.f)*ysc),
        0xffffff00 | (int)((128.f + colorTabHB[1].percent*127.f)*ysc),
    };
    int i;//, j;
    F32 vertHt = ht - topFrame[0]->height*ysc;
    F32 fadePct = 0.35f;
    int baseAlpha = 0xffffff00 | (int)(0x7f*ysc);
    int scaleAlpha = 0xffffff00 | (int)(255*ysc);  //for fading with scaling
    F32 xposSc, yposSc;
    calculatePositionScales(&xposSc, &yposSc);

    //draw base
    display_sprite_positional(topBase, x, y, z, 1.f, ysc, CLR_H_TAB_BACK&baseAlpha, H_ALIGN_LEFT, V_ALIGN_TOP );
    display_sprite_positional(vertBase, x, y+topBase->height*yposSc*ysc, z, 1.f, vertHt*(1.f-fadePct)/vertBase->height, CLR_H_TAB_BACK&baseAlpha, H_ALIGN_LEFT, V_ALIGN_TOP );
    display_sprite_positional_blend(vertBase, x, y+(topBase->height*ysc+vertHt*(1.f-fadePct))*yposSc, z, 1.f, vertHt*fadePct/vertBase->height, CLR_H_TAB_BACK&baseAlpha, CLR_H_TAB_BACK&baseAlpha, CLR_H_TAB_BACK&0xffffff00, CLR_H_TAB_BACK&0xffffff00, H_ALIGN_LEFT, V_ALIGN_TOP );
    //draw frame
    for (i = 0; i < 1; ++i)
    {
        display_sprite_positional(topFrame[i], x, y, z, 1.f, ysc, HYBRID_FRAME_COLOR[i]&scaleAlpha, H_ALIGN_LEFT, V_ALIGN_TOP );
    }
    display_sprite_positional(vertFrame, x, y+topFrame[0]->height*yposSc*ysc, z, 1.f, vertHt*(1.f-fadePct)/vertFrame->height, HYBRID_FRAME_COLOR[0]&scaleAlpha, H_ALIGN_LEFT, V_ALIGN_TOP );
    display_sprite_positional_blend(vertFrame, x, y+(topFrame[0]->height*ysc+vertHt*(1.f-fadePct))*yposSc, z, 1.f, vertHt*fadePct/vertFrame->height, HYBRID_FRAME_COLOR[0]&scaleAlpha, HYBRID_FRAME_COLOR[0]&scaleAlpha, HYBRID_FRAME_COLOR[0]&0xffffff00, HYBRID_FRAME_COLOR[0]&0xffffff00, H_ALIGN_LEFT, V_ALIGN_TOP );
}

//
//
static void costume_manageColors( int mode, F32 x, F32 y, F32 z, float scale )
{
    int result, drawTwo = FALSE, selected, clrSkin, color1, color2, color3, color4, fourColor = FALSE, whichColor;
    const CostumeTexSet * tset = NULL;
    const CostumeGeoSet* selectedGeoSet;
    Color newSelectedColor;
    const ColorPalette* palette;
    Color selectedColor;
    bool anyColorSelected;
     F32 bar_sc = MINMAX( (ELEMENT_HT*compressSc)/DEFAULT_BAR_HT , 0.f, 1.f );
    F32 ht = 400.f;
    F32 tabXBump = 207.f;
    int alphafont[2] = 
    {
        0xffffff00 | (int)((50.f  + colorTabHB[0].percent*205.f)*scale),
        0xffffff00 | (int)((50.f  + colorTabHB[1].percent*205.f)*scale),
    };
    int hovered = -1;
    static int lastHoveredColor = -1;
    F32 xposSc, yposSc;
    F32 textXSc = 1.f;
    F32 textYSc = 1.f;
    SpriteScalingMode ssm = getSpriteScaleMode();
    calculatePositionScales(&xposSc, &yposSc);

    if (!colorTabHB[0].icon0)
        {
        //init textures once
        hybridElementInit(&colorTabHB[0]);
        hybridElementInit(&colorTabHB[1]);
    }

    //draw frame
    drawCostumeFrame(x, y, z, scale, ht, 0);

    if( gActiveColor == COLOR_SKIN )
        palette = costume_get_current_origin_set()->skinColors[0];
    else
        palette = costume_get_current_origin_set()->bodyColors[0];

    selectedColor = getSelectedColor(gActiveColor, mode);

    selectedGeoSet = getSelectedGeoSet(mode);
    result = (selectedGeoSet && selectedGeoSet->bodyPartName);  //check to see if this part has colors

    // Make sure we differentiate between ColorNull and ordinary black.
    anyColorSelected =    (selectedColor.integer != ColorNull.integer) ||    result || 
                        (gColorsLinked && (gActiveColor == COLOR_1 || gActiveColor == COLOR_2));  //always show color picker when linked

    newSelectedColor = drawColorPalette( x + 175.f*xposSc, y + 50.f*yposSc*scale, z, scale, palette, selectedColor, anyColorSelected);    //210
    
    if (newSelectedColor.integer != ColorNull.integer)
        setSelectedColor(gActiveColor, newSelectedColor, mode);

    clrSkin = colorFlip(
        forceToPalette(getSelectedColor(COLOR_SKIN, mode), COLOR_SKIN)).integer;
    color1  = 
        colorFlip(forceToPalette(getSelectedColor(COLOR_1, mode), COLOR_1)).integer;
     color2  = 
        colorFlip(forceToPalette(getSelectedColor(COLOR_2, mode), COLOR_2)).integer;
     color3  = 
        colorFlip(forceToPalette(getSelectedColor(COLOR_3, mode), COLOR_3)).integer;
    color4  = 
        colorFlip(forceToPalette(getSelectedColor(COLOR_4, mode), COLOR_4)).integer;

    // skin color bar
    //----------------------------------------------------------------------------------
     if( !gColorsLinked && selectedGeoSet && selectedGeoSet->numColor == 4 )
    {
        fourColor = TRUE;

         if( gActiveColor == COLOR_SKIN )
            gActiveColor = COLOR_3;

        tset = selectedGeoSet->info[selectedGeoSet->currentInfo];

        if( gActiveColor == COLOR_1 || gActiveColor == COLOR_2 )
        {
            selected = FALSE;
            whichColor = 0;
        }
        else
        {
            selected = TRUE;
            if( gActiveColor == COLOR_3 )
                whichColor = 3;
            else
                whichColor = 4;
        }

        colorTabHB[1].text = textStd( "Color34" );
        result = drawColorTab( &colorTabHB[1], x + tabXBump*xposSc, y, z, scale, selected, color3, color4, 1, whichColor, false, 2, &hovered );
        tabXBump += 18.f;

        if( result )
            sndPlay( "color", SOUND_GAME );

        if( result == CB_HIT )
        {
            if( gActiveColor == COLOR_3 )
                gActiveColor = COLOR_4;
            else
                gActiveColor = COLOR_3;
        }
        else if ( result == CB_HIT_ONE )
            gActiveColor = COLOR_3;
        else if ( result == CB_HIT_TWO )
            gActiveColor = COLOR_4;
    }
    else
    {
        if( gActiveColor == COLOR_3 || gActiveColor == COLOR_4 )
            gActiveColor = COLOR_SKIN;
        selected = (gActiveColor == COLOR_SKIN);

        colorTabHB[1].text = "SkinColor";
        if( drawColorTab(&colorTabHB[1], x + tabXBump*xposSc, y, z, scale, selected, clrSkin, 0, FALSE, selected, false, 2, &hovered) )
            {
                gActiveColor = COLOR_SKIN;
                clearSelectedGeoSet();
            }
        }

    // other color bar
    //----------------------------------------------------------------------------------

    // first find out about our selected part
    if( selectedGeoSet && selectedGeoSet->info || gColorsLinked )
    {
        if( selectedGeoSet && selectedGeoSet->info )
            tset = selectedGeoSet->info[selectedGeoSet->currentInfo];
        else
        {
            // this geo has no colors, just display linked colors
            color1 = colorFlip(gLinkedColor1).integer;
            color2 = colorFlip(gLinkedColor2).integer;
        }

        if( gColorsLinked )
            colorTabHB[0].text = textStd( "ColorsLinked" );
        else if ( fourColor )
            colorTabHB[0].text = textStd( "Color12" );
        else if (selectedGeoSet)
            colorTabHB[0].text = textStd( "Color", selectedGeoSet->displayName );
        else
            colorTabHB[0].text = textStd( "Color", "InvalidString" );

        if( gColorsLinked ||
            selectedGeoSet && 
                (tset->texName1 && !strstriConst(tset->texName1, "skin" )) && 
                selectedGeoSet->numColor != 1 )
        {
            drawTwo = TRUE;
        }
        else if( gActiveColor == COLOR_2 )
            gActiveColor = COLOR_1;

        if( gActiveColor == COLOR_SKIN || gActiveColor == COLOR_3 || gActiveColor == COLOR_4 )
        {
            selected = FALSE;
            whichColor = 0;
        }
        else
        {
            selected = TRUE;
            if( gActiveColor == COLOR_1 )
                whichColor = 1;
            else
                whichColor = 2;
        }

          result = drawColorTab( &colorTabHB[0], x, y, z, scale, selected, color1, color2, drawTwo, whichColor, false, 0, &hovered );

        if( result )
            sndPlay( "color", SOUND_GAME );

        if( result == CB_HIT )
        {
            if( drawTwo )
            {
                if( gActiveColor == COLOR_1 )
                    gActiveColor = COLOR_2;
                else
                    gActiveColor = COLOR_1;
            }
            else
                gActiveColor = COLOR_1;
        }
        else if ( result == CB_HIT_ONE )
            gActiveColor = COLOR_1;
        else if ( result == CB_HIT_TWO )
            gActiveColor = COLOR_2;
    }
    else
    {
        colorTabHB[0].text = "NothingSelected";
        drawHybridTab(&colorTabHB[0], x, y, z, 1.f, scale, selected == colorTabHB[0].index, 0.5f + colorTabHB[0].percent*0.5f);
    }

    if (lastHoveredColor != hovered)
    {
        lastHoveredColor = hovered;
        if (hovered > -1)
        {
            //new hovered color, play sound
            sndPlay("N_PopHover", SOUND_GAME);
        }
    }

    //draw tab text
    if (isTextScalingRequired())
    {
        calculateSpriteScales(&textXSc, &textYSc);
        setSpriteScaleMode(SPRITE_XY_NO_SCALE);
    }
    font( &hybridbold_12 );
    font_color(CLR_BLACK&alphafont[0], CLR_BLACK&alphafont[0]);
    prnt( x+45.f*xposSc+1.f, y+(21.f+1.f)*yposSc*scale, z, textXSc, textYSc*scale, colorTabHB[0].text );
    font_color(CLR_WHITE&alphafont[0], CLR_WHITE&alphafont[0]);
    prnt( x+45.f*xposSc, y+21.f*yposSc*scale, z, textXSc, textYSc*scale, colorTabHB[0].text );

    font_color(CLR_BLACK&alphafont[1], CLR_BLACK&alphafont[1]);
    prnt( x+(27.f+tabXBump)*xposSc+1.f, y+(21.f+1.f)*yposSc*scale, z, textXSc, textYSc*scale, colorTabHB[1].text );
    font_color(CLR_WHITE&alphafont[1], CLR_WHITE&alphafont[1]);
    prnt( x+(27.f+tabXBump)*xposSc, y+21.f*yposSc*scale, z, textXSc, textYSc*scale, colorTabHB[1].text );
    setSpriteScaleMode(ssm);
}


F32 costumeDrawSlider(float x, float y, float scale, int sliderType, char * title,  F32 value, F32 range, float uiScale, float screenScaleX, float screenScaleY )
{
    float screenScale = MIN(screenScaleX, screenScaleY);
    font( &game_12 );
    font_color( uiColors.standard.text, uiColors.standard.text2 );

       prnt(x - (140*uiScale*screenScaleX), y+(5*uiScale*screenScaleY), 5000, uiScale*screenScale, uiScale*screenScale, title);

       return doSlider( x+(20*screenScaleX), y, 5000, 200*screenScaleX/screenScale, uiScale*screenScale, uiScale*screenScale, scale, sliderType, uiColors.standard.scrollbar, 0);
}

F32 costumeDrawSliderTriple(float x, float y, float wd, float scale, int sliderType, F32 value, F32 range, float uiscale )
{
    if( game_state.editnpc)
          prnt(x + 180, y+5*uiscale, 5000, uiscale, uiscale, "%.2f", scale);

    return doSlider( x, y, 5000, wd, uiscale, uiscale, scale, sliderType, uiColors.standard.scrollbar, 0 );
}

static void costume_manageScale(int tailor, F32 x, F32 y, F32 ht)
{
    int i = 0;

    Entity *e = playerPtr();

    F32 scale = ht / 412.5f;//(DEFAULT_BAR_HT*compressSc*2 + NUM_3D_BODY_SCALES*divider + 6*30 );
    F32 bar_sc = MINMAX( (ELEMENT_HT*compressSc)/DEFAULT_BAR_HT , 0.f, 1.f );
    F32 z = ZOOM_Z;
    F32 wd = 320.f;
    F32 faceHt = 190.f;
    F32 bodyHt = 175.f;
    F32 space = 25.f;
    F32 ySliderOffset = 40.f;
    int alpha = 0xffffff00 | (int)(255*scale);
    static HybridElement sButtonRandom = {0, NULL, "randomFaceButton", "icon_costume_random"};
    F32 xposSc, yposSc;
    F32 textXSc = 1.f;
    F32 textYSc = 1.f;
    SpriteScalingMode ssm = getSpriteScaleMode();
    calculatePositionScales(&xposSc, &yposSc);

    drawHybridDescFrame(x, y, z, wd, faceHt*scale, HB_DESCFRAME_FADEDOWN, 0.25f, scale, 0.5f, H_ALIGN_LEFT, V_ALIGN_TOP);
    if (isTextScalingRequired())
    {
        calculateSpriteScales(&textXSc, &textYSc);
        setSpriteScaleMode(SPRITE_XY_NO_SCALE);
    }
    font(&title_12);
    font_outl(0);
    font_color(CLR_H_DESC_TEXT_DS, CLR_H_DESC_TEXT_DS);
    cprnt(x+wd*xposSc/2.f+2.f, y+24.f*yposSc+2.f, z, textXSc, textYSc*scale, "Face");
    font_color(CLR_WHITE, CLR_WHITE);
    cprnt(x+wd*xposSc/2.f, y+24.f*yposSc, z, textXSc, textYSc*scale, "Face");
    font_outl(1);
    setSpriteScaleMode(ssm);
 
       if( D_MOUSEHIT == drawHybridButton( &sButtonRandom, x + (wd - 20.f)*xposSc, y+20.f*yposSc, z, bar_sc*scale, CLR_WHITE&alpha, HB_DRAW_BACKING ) )
    {    
        int j;
        const Appearance *app = &e->costume->appearance;
        srand( timerCpuTicks64() );

        for(j=0;j<NUM_3D_BODY_SCALES;j++)
        {
            gFaceDest[j][0] = ((float)(rand()%200)/100.f) - 1;
            gFaceDest[j][1] = ((float)(rand()%200)/100.f) - 1; 
            gFaceDest[j][2] = ((float)(rand()%200)/100.f) - 1; 

            gFaceDestSpeed[j][0] = ABS(gFaceDest[j][0]-app->f3DScales[j][0])*.1f/2;
            gFaceDestSpeed[j][1] = ABS(gFaceDest[j][1]-app->f3DScales[j][1])*.1f/2;
            gFaceDestSpeed[j][2] = ABS(gFaceDest[j][2]-app->f3DScales[j][2])*.1f/2;
        }

        gConvergingFace = true;
    }

     y += ySliderOffset*yposSc*scale;

    if (isTextScalingRequired())
    {
        setSpriteScaleMode(SPRITE_XY_NO_SCALE);
    }
    font(&hybridbold_12);
    font_color(CLR_WHITE, CLR_WHITE);
    cprnt(x+162.f*xposSc, y+10.f*yposSc, z, 0.8f*textXSc, 0.8f*textYSc*scale, "WidthString");
    cprnt(x+212.f*xposSc, y+10.f*yposSc, z, 0.8f*textXSc, 0.8f*textYSc*scale, "HeightString");
    cprnt(x+270.f*xposSc, y+10.f*yposSc, z, 0.8f*textXSc, 0.8f*textYSc*scale, "DepthString");
    setSpriteScaleMode(ssm);

    for(i=0;i<NUM_3D_BODY_SCALES;i++)
           drawFaceSlider( x+20.f*xposSc, y+(15.f+i*space*scale*.75f)*yposSc, z, scale*.75f, BS_SELECTOR_WD, i);
    
     y += (faceHt-30.f)*yposSc*scale;

    drawHybridDescFrame(x, y, z, wd, bodyHt*scale, HB_DESCFRAME_FADEDOWN, 0.25f, scale, 0.5f, H_ALIGN_LEFT, V_ALIGN_TOP);
    if (isTextScalingRequired())
    {
        setSpriteScaleMode(SPRITE_XY_NO_SCALE);
    }
    font(&title_12);
    font_outl(0);
    font_color(CLR_H_DESC_TEXT_DS, CLR_H_DESC_TEXT_DS);
    cprnt(x+wd*xposSc/2.f+2.f, y+24.f*yposSc+2.f, z, textXSc, textYSc*scale, "Body");
    font_color(CLR_WHITE, CLR_WHITE);
    cprnt(x+wd*xposSc/2.f, y+24.f*yposSc, z, textXSc, textYSc*scale, "Body");
    font_outl(1);
    setSpriteScaleMode(ssm);

       y += ySliderOffset*yposSc*scale;

       drawScaleSliders(x + 20.f*xposSc, y, z, scale*.75f, BS_SELECTOR_WD, BS_SELECTOR_WD-130.f);

    changeBoneScaleEx(e->seq, &e->costume->appearance);
}

void tailor_RevertScales( Entity * e )
{
    int i;
    Costume * costume = costume_current(e);
    Costume* mutable_e_costume = costume_as_mutable_cast(e->costume);

     for( i = 0; i < MAX_BODY_SCALES; i++ )
    {
        mutable_e_costume->appearance.fScales[i] = costume->appearance.fScales[i];
    }
    genderSetScalesFromApperance(e);

    // constrain scales
    genderBoneScale(    0, 0, 1, 1.f, 1.f, 1);
    genderChestScale(    0, 0, 1, 1.f, 1.f, 1);
    genderShoulderScale(0, 0, 1, 1.f, 1.f, 1);
    genderWaistScale(    0, 0, 1, 1.f, 1.f, 1);
    genderHipScale(        0, 0, 1, 1.f, 1.f, 1);
    genderLegScale(        0, 0, 1, 1.f, 1.f, 1);
    genderArmScale(        0, 0, 1, 1.f, 1.f, 1);
}

int gTailorColorGroup = 0;
int gTailorShowingSGColors = 0;
int gTailorShowingCostumeScreen = 0;
int gTailorShowingSGScreen = 0;
float gTailorSGCostumeScale = 1.0f;

static float sgcolor_ht;
static int color_selected = 1;
static int sSGMode;
static char *colorOpts[] = 
{
    "CostumeStr",
    "SuperGroupStr",
};
static GenericTextList sTextList = { colorOpts, 2, 0 };
static int sCurrentColorSet = 0;
static F32 sOverall_ht;

void costume_reset(int sgMode)
{
    Entity *e = playerPtr();

    sSGMode = sgMode;
    if (sgMode)
    {
        sgcolor_ht = MAX_HT;
        sOverall_ht = 0.0f;
        color_selected = 1;
        gTailorColorGroup = e->pl->costume[e->pl->current_costume]->appearance.currentSuperColorSet + 1;
        sCurrentColorSet = gTailorColorGroup;
        gTailorShowingSGColors = 1;
        gTailorShowingCostumeScreen = 0;
        sTextList.textItems = &colorOpts[1];
        sTextList.count = 1; // costume_getNumSGColorSlot(e);
        sTextList.current = 0; // gTailorColorGroup - 1;
    }
    else
    {
        sgcolor_ht = 0.0f;
        sOverall_ht = MAX_HT;
        color_selected = 1;
        gTailorColorGroup = 0;
        sCurrentColorSet = 0;
        gTailorShowingSGColors = 0;
        gTailorShowingCostumeScreen = 1;
        sTextList.textItems = &colorOpts[0];
        sTextList.count = e && e->supergroup ? /*costume_getNumSGColorSlot(e) + 1*/2 : 1;
        sTextList.current = 0;
    }
}

// TODO handle height management for both main display *AND* sg color picker
// What's drawing is set by gTailorShowingSGColors
void costume_drawColorsScales(CostumeEditMode mode)
{
    static float color_ht, scale_ht;
    static HybridElement hb[2] = 
    {
        {0,"ColorHeading", 0, "icon_costume_colors_0", "icon_costume_colors_1"}, 
        {0,"Scales", 0, "icon_costume_scales_0", "icon_costume_scales_1"}
    };
     float y = REGION_Y;
    float maxht = MAX_HT - REGION_DEFAULT_SPACE*1.75f;
    float color_sc, scale_sc;
    Entity * e = playerPtr();
    float colorButtonX;
    static int pendingColorSet = -1;
    int prev_TailorShowingSGColors;
    F32 z = ZOOM_Z;
    F32 xposSc, yposSc, xp, yp;
    F32 textXSc = 1.f;
    F32 textYSc = 1.f;
    SpriteScalingMode ssm = getSpriteScaleMode();
    if (isTextScalingRequired())
    {
        calculateSpriteScales(&textXSc, &textYSc);
    }
    calculatePositionScales(&xposSc, &yposSc);

    if (!hb[0].icon0)
    {
        hybridElementInit(&hb[0]);
        hybridElementInit(&hb[1]);
    }

    // This is completely in the wrong place.  Originally it only handled the growth and shrinkage of the color picker and the scales.
    // But when the "Colors" button grew the selector, the overall_ht and sgcolor_ht scale factors were added.  The problem is that these
    // are used elsewhere in the system, hence they get passed around as globals.
    // TODO move the second block of code (the one that sets overall_ht and sgcolor_ht) elsewhere in the function hierarchy, possibly tailormenu(),
    // and pass the two resultant values around as parameters.
    // TODO I really need three variables that flag: scales / colors fully open,
    // sg color picker fully open, and which side of the "watershed" we're on.  These need to be set here
    // and made generally available to the whole uiTailor / uiSuperCostume subsystem.
    // Do this if any weird bugs show up here.
       if( color_selected )
    {
         if( color_ht < maxht )
            color_ht += TIMESTEP*RSET_SPEED*2;
        if( color_ht > maxht )
            color_ht = maxht;

        if( scale_ht > 0 )
            scale_ht -= TIMESTEP*RSET_SPEED*2;
        if( scale_ht < 0 )
            scale_ht = 0;
    }
    else
    {
        if( scale_ht < maxht )
            scale_ht += TIMESTEP*RSET_SPEED*2;
        if( scale_ht > maxht )
            scale_ht = maxht;

         if( color_ht > 0 )
            color_ht -= TIMESTEP*RSET_SPEED*2;
        if( color_ht < 0 )
            color_ht = 0;
    }

    if (sCurrentColorSet == 0 || !(mode == COSTUME_EDIT_MODE_TAILOR))
    {
         if( sgcolor_ht > 0 )
            sgcolor_ht -= TIMESTEP*RSET_SPEED*2;
        if( sgcolor_ht < 0 )
            sgcolor_ht = 0;
        if( sgcolor_ht == 0.0f)
        {
             if( sOverall_ht < maxht )
                sOverall_ht += TIMESTEP*RSET_SPEED*2;
            if( sOverall_ht > maxht )
                sOverall_ht = maxht;
        }
    }
    else
    {
        if( sOverall_ht > 0 )
            sOverall_ht -= TIMESTEP*RSET_SPEED*2;
        if( sOverall_ht < 0 )
            sOverall_ht = 0;
        if( sOverall_ht == 0.0f)
        {
             if( sgcolor_ht < maxht )
                sgcolor_ht += TIMESTEP*RSET_SPEED*2;
            if( sgcolor_ht > maxht )
                sgcolor_ht = maxht;
        }
    }
    prev_TailorShowingSGColors = gTailorShowingSGColors;
    gTailorShowingSGColors = sgcolor_ht != 0.0f;
    gTailorShowingCostumeScreen = sOverall_ht == MAX_HT;
    gTailorShowingSGScreen = sgcolor_ht == MAX_HT;

    if (prev_TailorShowingSGColors != gTailorShowingSGColors && pendingColorSet != -1)
    {
#ifndef TEST_CLIENT
        tailor_SGColorSetSwitch(pendingColorSet); 
        if (pendingColorSet)
        {
            sendHideEmblemChange(e->pl->hide_supergroup_emblem);
        }
#endif
        pendingColorSet = -1;
    }

    // Disable the slide
    //colorButtonX = gTailorShowingSGColors ? 220 - (int) (170.0f * sgcolor_ht / area_ht) : 220 + (int) (170.0f * sOverall_ht / area_ht);
       colorButtonX = 1024 - REGION_X - REGION_WD/2;
    color_sc = MAX(color_ht, sOverall_ht)/ maxht;

    if (gTailorColorGroup != 0)
    {
        color_sc = 1;
    }
    if ((mode != COSTUME_EDIT_MODE_TAILOR) || !color_selected)
    {
        color_sc = 0;
    }
    scale_sc = MIN(scale_ht, sOverall_ht) / maxht;
    gTailorSGCostumeScale = sgcolor_ht / maxht;

    xp = DEFAULT_SCRN_WD;
    yp = 0.f;
    applyPointToScreenScalingFactorf(&xp, &yp); //use real pixel location
    if( drawCostumeRegionButton( 0, xp - 20.f*xposSc, (y + REGION_DEFAULT_SPACE/2.f)*yposSc, z, REGION_WD, REGION_DEFAULT_SPACE, color_selected, 1, &hb[0], 1 ))
    {
        if (!color_selected)
        {
            sndPlay( "N_MenuExpand", SOUND_UI_ALTERNATE );
        }
        color_selected = true;
    }

    y += REGION_DEFAULT_SPACE;

    if ((mode == COSTUME_EDIT_MODE_TAILOR) && (color_selected || gTailorColorGroup != 0) && (color_sc >= 1))
    {
#ifndef TEST_CLIENT
        static unsigned int nextChangeAt = 0;
        scrollSet_TextListSelector(&sTextList, colorButtonX + ((BS_SELECTOR_WD+40) / 2.0f), y - BS_SELECTOR_Y - 23, 5200, 
            1.f, (BS_SELECTOR_WD*1.f), timerSecondsSince2000() > nextChangeAt);
        if (gTailorColorGroup != sTextList.current + (sSGMode ? 1 : 0))
        {
            sCurrentColorSet = sTextList.current + (sSGMode ? 1 : 0);
            pendingColorSet = sCurrentColorSet;
            nextChangeAt = timerSecondsSince2000() + 3;
        }
#endif
    }
    else
    {
        gTailorColorGroup = 0;
    }

    color_sc = MIN(color_ht, sOverall_ht) / maxht;
    color_sc = color_sc > 0.95f ? 1.f : color_sc; //fix for some really strange FP error

     if( color_sc > .05f )
    {
        costume_manageColors(mode, xp - 334.f*xposSc, y*yposSc, z, color_sc);
    }

    y += color_ht;

    {
        //draw link colors button
        const char * colorLinkTextureName = gColorsLinked ? "icon_costume_colorbody_linked" : "icon_costume_colorbody";
        const char * colorLinkTooltip = gColorsLinked ? "unlinkColorsButton" : "linkColorsButton";
        static HybridElement sButtonLink = {0};
        SpriteScalingMode ssm = getSpriteScaleMode();
        sButtonLink.iconName0 = colorLinkTextureName;
        sButtonLink.desc1 = colorLinkTooltip;
        if( D_MOUSEHIT == drawHybridButton(&sButtonLink, xp - 299.f*xposSc, (y+10.f)*yposSc, z, 0.6f, (0xffffff00|(int)(255*color_sc)), HB_DRAW_BACKING ))
        {
            gColorsLinked = !gColorsLinked;  //toggle
            if (gColorsLinked)
            {
                gLinkedColor1 = getSelectedColor(COLOR_1, mode);
                gLinkedColor2 = getSelectedColor(COLOR_2, mode);
            }
        }
        updateColors();
        
        if (isTextScalingRequired())
        {
            setSpriteScaleMode(SPRITE_XY_NO_SCALE);
        }
        font(&hybridbold_12);
        font_color(CLR_WHITE, CLR_WHITE);
        prnt(xp - 274.f*xposSc, (y+15.f)*yposSc, z, textXSc, textYSc*color_sc, gColorsLinked ? "LinkCostumeColorsString" : "UnlinkCostumeColorsString");
        y += color_sc*30.f;  //add space for the link buttons
        setSpriteScaleMode(ssm);
    }


     if( (mode == COSTUME_EDIT_MODE_TAILOR) && !gTailorShowingSGColors && sOverall_ht == maxht)
    {
        int retVal, cost = 0;
        static HybridElement sButtonReset = {0, NULL, "ResetString", "icon_reset_0"};
        font( &game_12 );
        if( costume_changedScale( costume_as_const(costume_current(e)), e->costume ) )
        {
            font_color( CLR_WHITE, CLR_RED );
            cost = 2*tailor_getRegionCost( e, "Upper Body" );;
        }
        else
            font_color( CLR_WHITE, CLR_WHITE );

        setSpriteScaleMode(SPRITE_XY_NO_SCALE);
        cprntEx( xp-84.f*xposSc, (y+24.f)*yposSc, 5101, textXSc, textYSc, CENTER_Y, "(+%i)", cost );
        setSpriteScaleMode(ssm);

        retVal = drawHybridButton(&sButtonReset, xp - (264.f+hb[1].percent*30.f)*xposSc, (y+25.f)*yposSc, 5100, 0.9f, CLR_WHITE & (0xffffff00 | (int)(0xff*scale_sc)), HB_DRAW_BACKING | HB_DO_NOT_EDIT_ELEMENT );
        if( retVal >= D_MOUSEOVER )
        {
            if( retVal == D_MOUSEHIT )
                tailor_RevertScales(e);
        }

    }

      if ( (!gTailorShowingSGColors && sOverall_ht == maxht) && (mode != COSTUME_EDIT_MODE_NPC) && (mode != COSTUME_EDIT_MODE_NPC_NO_SKIN ))
    {
        //color_selected = false;
           if( drawCostumeRegionButton( 0, xp - 20.f*xposSc, (y + REGION_DEFAULT_SPACE/2)*yposSc, z, REGION_WD, REGION_DEFAULT_SPACE, !color_selected, 1, &hb[1], 1 ))
        {
            if (color_selected)
        {
                sndPlay( "N_MenuExpand", SOUND_UI_ALTERNATE );
            }
            color_selected = false;
        }
    }

    y += REGION_DEFAULT_SPACE;
    if(!gTailorShowingSGColors && sOverall_ht == maxht && scale_sc > .05f )
        costume_manageScale(mode, xp - 334.f*xposSc, y*yposSc, scale_ht);
}


//
//
static F32 drawGeoSet( CostumeGeoSet * gset, const CostumeBoneSet * bset, float x, float y, float z, float scale, int regionOpen, int mode, const char * storeCategory, ShoppingCart * storePartsContained)
{
    int i, bpID=0;
    const char *error_on = NULL;
    bool categoryHasStoreParts = false;

    F32 curr_sc, maxHt, element_sc = MINMAX( (ELEMENT_HT*compressSc*scale)/DEFAULT_BAR_HT, 0, 1.f );
    int flags = HB_ROUND_ENDS | HB_SMALL_FONT;
    int prevCartIndex = storePartsContained->itemCount;
    F32 xposSc, yposSc;
    calculatePositionScales(&xposSc, &yposSc);
    
     maxHt = gset->ht;
     gset->wd = BS_SELECTOR_WD - 20;

    // HYBRID : add invalid check for details

    categoryHasStoreParts = addGeosetStoreParts(gset, storePartsContained);

    if( element_sc <= 0 )
        return 0;

    // This draws the Part Names: Face, Hair, Ears, Detail 1
    y += ELEMENT_HT*compressSc*scale/2;
      (cpp_const_cast(HybridElement*)(gset->hb))->text = gset->displayName;

    //draw locks
    if ((gset->ht > 0.f || !gset->isOpen) && categoryHasStoreParts)
    {
        F32 locksc = (0.9f + gset->hb->percent*0.1f);  //scaling
        if (gset->ht < ELEMENT_HT)
        {
            int alpha = (int) ((1.f-gset->ht/ELEMENT_HT) * 0xff);

            //draw lock
            drawHybridStoreLockIcon((x-90*(1.f+gset->hb->percent*0.1f))*xposSc, y*yposSc, z+3.f, locksc*element_sc, 0xffffff00 | alpha);

            //draw store icon (geo)
            drawHybridStoreAddShoppingCartMenuButton(storePartsContained, prevCartIndex, storePartsContained->itemCount, (x+90*(1.f+gset->hb->percent*0.1f))*xposSc, y*yposSc, z+3.f, element_sc, 0xffffff00 | alpha, HB_STORE_CATEGORY); //PSID_Costume
        }
    }

    if ( (mode == COSTUME_EDIT_MODE_NPC) || (mode == COSTUME_EDIT_MODE_NPC_NO_SKIN ) )
    {
        if( D_MOUSEHIT == drawHybridBar( cpp_const_cast(HybridElement*)(gset->hb), s_selectedGeoSet == gset, x*xposSc, y*yposSc, z+2, gset->wd, element_sc, flags, (1.f-globalAlpha/2), H_ALIGN_LEFT, V_ALIGN_CENTER ) )
        {
            s_selectedGeoSet = gset;
            gColorsLinked = 0;
            if (gActiveColor == COLOR_SKIN)
            {
                gActiveColor = 0;
            }
        }
    }
    else
    {
        if( D_MOUSEHIT == drawHybridBar( cpp_const_cast(HybridElement*)(gset->hb), gset->isOpen, x*xposSc, y*yposSc, z+2, gset->wd, element_sc, flags, (1.f-globalAlpha/2), H_ALIGN_LEFT, V_ALIGN_CENTER ) )
        {
            int tmp = gset->isOpen;
            for( i = eaSize( &bset->geo )-1; i >= 0; i-- )
            {
                CostumeGeoSet* mutable_gset = cpp_const_cast(CostumeGeoSet*)(bset->geo[i]);
                mutable_gset->isOpen = FALSE;
            }

            gset->isOpen = !tmp;
        }
        drawSectionArrow( (x + 105.f*(0.9f + 0.1f*gset->hb->percent))*xposSc, y*yposSc, z+2.f, compressSc, gset->hb->percent, (1.f - gset->openScale));
    }
     y += ELEMENT_HT*compressSc;
    curr_sc = MINMAX( MIN(maxHt,ELEMENT_HT*compressSc)/DEFAULT_BAR_HT, 0.f, 1.f);

        if( gset->faces ) // this is a face scale selector
        {
             selectGeoSet( gset, kScrollType_Face, x, y, z, curr_sc, gset->wd, mode, (1.f-globalAlpha/2) );
             return (gset->ht + ELEMENT_HT)*compressSc;
        }
        else if( gset->type )
        {
            selectGeoSet( gset, kScrollType_Geo, x, y, z, curr_sc, gset->wd, mode, (1.f-globalAlpha/2));
        }
        else
        {
            selectGeoSet( gset, kScrollType_GeoTex, x, y, z, curr_sc, gset->wd, mode, (1.f-globalAlpha/2));
        }

    maxHt -= ELEMENT_HT;
    y += ELEMENT_HT*compressSc;
    curr_sc = MINMAX(MIN(maxHt,ELEMENT_HT*compressSc)/DEFAULT_BAR_HT, 0.f, 1.f);

    if(gSelectedCostumeSet >= 0)
        error_on = costume_get_current_origin_set()->costumeSets[gSelectedCostumeSet]->filename;

    if( gset->info && partIsRestrictedEx(gset->info[gset->currentInfo], error_on, false, false))
    {
        if( gset->faces )
            changeGeoSet(gset, kScrollType_Face, 1 );
        else if ( gset->type  )
            changeGeoSet(gset, kScrollType_Geo, 1 );
        else
            changeGeoSet(gset, kScrollType_GeoTex, 1 );
    }


    // tex set
     if( eaSize( &gset->mask ) || gset->type )
    {
            if( gset->type )
            selectGeoSet( gset, kScrollType_Tex1, x, y, z, curr_sc, gset->wd, mode, (1.f-globalAlpha/2) );
            else
            selectGeoSet( gset, kScrollType_Tex2, x, y, z, curr_sc, gset->wd, mode, (1.f-globalAlpha/2) );

        maxHt -= ELEMENT_HT;
        y += ELEMENT_HT*compressSc;
        curr_sc = MINMAX( MIN(maxHt,ELEMENT_HT*compressSc)/DEFAULT_BAR_HT, 0.f, 1.f);

        if( gset->info && partIsRestrictedEx(gset->info[gset->currentInfo], error_on, false, false))
        {
            Errorf("Reverting %s->%s->%s", bset->name, gset->name, gset->info[gset->currentInfo]->displayName);
            if( gset->faces )
                changeGeoSet(gset, kScrollType_Tex1, 1 );
            else if ( gset->type  )
                changeGeoSet(gset, kScrollType_Tex2, 1 );
        }
    }
    
    // mask
    if( gset->type )
    {
        selectGeoSet( gset, kScrollType_Tex2, x, y, z,curr_sc, gset->wd, mode, (1.f-globalAlpha/2) );
    }

       return (gset->ht + ELEMENT_HT)*compressSc;
}

//
//
static int drawRegionSet( F32 y, F32 z, int regNum, int mode, ShoppingCart * storePartsContained )
{
    F32 curr_sc;
    F32 element_ht = ELEMENT_HT*compressSc;
    F32 max_ht = 0; // tracks non-scaled total height
    F32 ht_remaining = reg_ht[regNum]*compressSc; // track actual height remaining to draw
    int geo_drawn = 0;
    F32 xposSc, yposSc;
    calculatePositionScales(&xposSc, &yposSc);

    // first take care of the bone set selector
    //-----------------------------------------------------------------------------------------------------------
    curr_sc = MINMAX( MIN(ht_remaining, element_ht)/DEFAULT_BAR_HT , 0.f, 1.f );

     if(regNum == REGION_SETS)
    {
        const CostumeOriginSet *oset = gCostumeMaster.bodySet[gCurrentBodyType]->originSet[gCurrentOriginSet];
           selectCostumeSet(oset, REGION_CENTER, y + element_ht/2, z, curr_sc, BS_SELECTOR_WD, (1.f-globalAlpha/2));
        ht_remaining -= element_ht;
        max_ht += ELEMENT_HT;
        y += element_ht;
    }
    else
    {
        const CostumeRegionSet *region = gCostumeMaster.bodySet[gCurrentBodyType]->originSet[gCurrentOriginSet]->regionSet[regNum];
        const CostumeBoneSet *bset = NULL;
        int i, geo_count;
        bool categoryHasStoreParts = false;

         if(eaSize(&region->boneSet) > 1 )
        {
            selectBoneset( region, REGION_CENTER, (y + element_ht/2), z,    curr_sc, BS_SELECTOR_WD, mode, (1.f-globalAlpha/2) );
            ht_remaining -= element_ht;
            max_ht += ELEMENT_HT;
            y += element_ht;


            bset = region->boneSet[region->currentBoneSet];
            categoryHasStoreParts = addBonesetStoreParts(bset, storePartsContained);

            if (element_ht > 0.f && categoryHasStoreParts)
            {
                F32 locksc = (0.9f + curr_sc*0.1f);  //scaling temporary asset
                int alpha = (int) ((1.f-globalAlpha/2) * 0xff);
                F32 locky = y - (element_ht)/2;

                //draw lock
                drawHybridStoreLockIcon((REGION_CENTER-100.f)*xposSc, locky*yposSc, z+3.f, locksc*curr_sc, 0xffffff00 | alpha);

                //draw store icon
                drawHybridStoreMenuButton(bset->storeProduct, (REGION_CENTER+100.f)*xposSc, locky*yposSc, z+3.f, curr_sc, 0xffffff00 | alpha, HB_STORE_PRODUCT);
            }
        }

        // now the GeoSet Buttons
        //-----------------------------------------------------------------------------------------------------
        bset = region->boneSet[region->currentBoneSet];
        geo_count = eaSize( &bset->geo );
        for( i = 0; i < geo_count; i++ )
        {
            int num_elements = 1;
            F32 geo_ht;
            CostumeGeoSet* mutable_gset = cpp_const_cast(CostumeGeoSet*)(bset->geo[i]);

            if(  geosetIsRestricted( bset->geo[i] ) )
            {
                mutable_gset->isOpen = 0;
                continue;
            }

            if( bset->geo[i]->isOpen )
            {
                num_elements++;
                if( eaSize( &bset->geo[i]->mask ) )
                    num_elements++;
                if( bset->geo[i]->type )
                    num_elements++;
            }

            geo_ht = ELEMENT_HT*num_elements*compressSc;
            curr_sc = MINMAX( ht_remaining/geo_ht, 0.f, 1.f );

            if( (mode == COSTUME_EDIT_MODE_TAILOR) )
                tailor_displayCost( bset, bset->geo[i], (REGION_CENTER + curr_sc*bset->geo[i]->wd/2)*xposSc, (y+24.f*compressSc)*yposSc, z+5.f, curr_sc, 0, 1 );

            if( ((mode == COSTUME_EDIT_MODE_NPC) || (mode == COSTUME_EDIT_MODE_NPC_NO_SKIN) ) && 
                !(colorTintablePartsBitField & (1 << (bpGetIndexFromName(bset->geo[i]->bodyPartName)))) )
            {
                if (isDevelopmentMode())
                {
                    geo_ht = drawGeoSet( cpp_const_cast(CostumeGeoSet*)(bset->geo[i]), bset, REGION_CENTER, y, z, curr_sc, gCurrentRegion == regNum, mode, region->storeCategory, storePartsContained )*curr_sc;
                    ht_remaining -= geo_ht;
                    max_ht += ELEMENT_HT*num_elements;     
                    y += geo_ht;
                    geo_drawn++;
                }
            }
            else
            {
                geo_ht = drawGeoSet( cpp_const_cast(CostumeGeoSet*)(bset->geo[i]), bset, REGION_CENTER, y, z, curr_sc, gCurrentRegion == regNum, mode, region->storeCategory, storePartsContained )*curr_sc;
                ht_remaining -= geo_ht;
                max_ht += ELEMENT_HT*num_elements;     
                y += geo_ht;
                geo_drawn++;
            }

        }
    }

    if( reg_ht[regNum] > max_ht )
         reg_ht[regNum] = max_ht; 

    rStoreScale[regNum] = 1.f - reg_ht[regNum] / max_ht;

    if ( ( (mode == COSTUME_EDIT_MODE_NPC) || (mode == COSTUME_EDIT_MODE_NPC_NO_SKIN) ) && ( geo_drawn == 0 ) )
    {
        return -1;
    }
    return 0;
}

/* 
    if cloth
        no cloth or back
    else if back
        no cloth
*/
static bool costume_isOtherRestrictedPieceSelectedEx(const char *currentPart, const char *restriction, const CostumeRegionSet * rset)
{
    int isCloth = false;

    if (!currentPart || !restriction)
        return false;

    isCloth = costume_hasFlagWithException(playerPtr(), restriction, currentPart, rset);

    return isCloth;
}

static bool costume_isOtherRestrictedPieceSelectedExcludingRegionset(const char *currentPart, const char *restriction, const CostumeRegionSet * rset)
{
    return costume_isOtherRestrictedPieceSelectedEx( currentPart, restriction, rset );
}

bool costume_isOtherRestrictedPieceSelected(const char *currentPart, const char *restriction)
{
    return costume_isOtherRestrictedPieceSelectedEx( currentPart, restriction, NULL );
}


const char *costume_isRestrictedPiece(const char **flags, const char *restriction)
{
    int i, count = eaSize(&flags);

    if (!restriction)
        return NULL;

    for (i = 0; i < count; i++)
    {
        if (stricmp(flags[i], restriction) == 0)
            return restriction;
    }

    return NULL;
}

// These funcs tell everything else what to do


F32 gatherRegionsHeight() // this gets the total size, unscaled
{
    F32 y = 0;
    int i, j;
     int region_total = REGION_TOTAL_DEF;
    if (region_total > eaSize(&gCostumeMaster.bodySet[gCurrentBodyType]->originSet[gCurrentOriginSet]->regionSet))
        region_total = eaSize(&gCostumeMaster.bodySet[gCurrentBodyType]->originSet[gCurrentOriginSet]->regionSet);

    // costume sets hack /////////////////////////////////////////////////////////
    for(i = eaSize(&gCostumeMaster.bodySet[gCurrentBodyType]->originSet[gCurrentOriginSet]->costumeSets)-1; i >= 0; --i)
    {
        // make sure there's at least one good set before trying to display the region button
        if(!costumesetIsRestricted(gCostumeMaster.bodySet[gCurrentBodyType]->originSet[gCurrentOriginSet]->costumeSets[i]))
        {
            y += REGION_DEFAULT_SPACE;
            if( gCurrentRegion == REGION_DEFAULT_SPACE)
                y += ELEMENT_HT;

            if( REGION_SETS == gCurrentRegion)
            {
                reg_ht[REGION_SETS] += TIMESTEP*REGION_SPEED;
            }
            else
            {
                if( reg_ht[REGION_SETS] > 0 )
                    reg_ht[REGION_SETS] -= TIMESTEP*REGION_SPEED;
                if( reg_ht[REGION_SETS] < 0 )
                    reg_ht[REGION_SETS] = 0;
            }

            break;
        }
    }

    for( i = 0; i < region_total; i++)
    {    
        int bsets_found = 0, bsets_good = 0;
        const CostumeRegionSet *rset = gCostumeMaster.bodySet[gCurrentBodyType]->originSet[gCurrentOriginSet]->regionSet[i];
        const CostumeBoneSet *bset = rset->boneSet[rset->currentBoneSet];
        int iSize = eaSize( &bset->geo );

        if(regionsetIsRestricted(rset, &bsets_found))
            continue;

        y += REGION_DEFAULT_SPACE;

        if( gCurrentRegion == i )
        {
            reg_ht[i] += TIMESTEP*REGION_SPEED;

            if( eaSize(&rset->boneSet) > 1 )
                y += ELEMENT_HT;
        }
        else
        {
            if( reg_ht[i] > 0 )
                reg_ht[i] -= TIMESTEP*REGION_SPEED;
            if( reg_ht[i] < 0 )
                reg_ht[i] = 0;
        }

        for( j = 0; j < iSize; j++ )
        {
            int cats = 1;
            F32 maxHt;
            CostumeGeoSet * gset =  cpp_const_cast(CostumeGeoSet*)(bset->geo[j]);
            if(  geosetIsRestricted( gset ) )
            {
                gset->isOpen = 0;
                continue;
            }

            if( gset->isOpen )
            {
                cats++;
                if( eaSize( &gset->mask ) )
                    cats++;
                if( gset->type )
                    cats++;
            }

            if( gCurrentRegion == i )
                y += cats * ELEMENT_HT;

            maxHt = ELEMENT_HT*(cats-1);
            if( gset->isOpen )
            {
                gset->ht += TIMESTEP*(REGION_SPEED+5);
                if ( gset->ht > maxHt )
                    gset->ht = maxHt;
            }
            else
            {
                gset->ht -= TIMESTEP*(REGION_SPEED+5);
                if ( gset->ht < 0 )
                    gset->ht = 0;
            }
            gset->openScale = maxHt ? gset->ht / maxHt : 0.f;
        }
    }

    return y;
}

//
void costume_drawRegions( CostumeEditMode mode, ShoppingCart * storePartsContained )
{
    // state variables for the region tabs
    static HybridElement hb[REGION_TOTAL_UI] = 
    {
        {REGION_HEAD, 0, 0, "icon_costume_head_0", "icon_costume_head_1",},
        {REGION_UPPER, 0, 0, "icon_costume_upperbody_0", "icon_costume_upperbody_1",},
        {REGION_LOWER, 0, 0, "icon_costume_lowerbody_0", "icon_costume_lowerbody_1",},
        {REGION_WEAPON, 0, 0, "icon_costume_weapons_0", "icon_costume_weapons_1",},
        {REGION_SHIELD, 0, 0, "icon_costume_shields_0", "icon_costume_shields_1",},
        {REGION_CAPE, 0, 0, "icon_costume_bakdetails_0", "icon_costume_bakdetails_1",},
        {REGION_GLOWIE, 0, 0, "icon_costume_auras_0", "icon_costume_auras_1",},
        {REGION_SETS, 0, 0, "icon_costume_sets_0", "icon_costume_sets_1",}
    };
    static HybridElement hb_arachnos[REGION_ARACHNOS_TOTAL_UI] =   //special case for arachnos so the icons match up
    {
        {REGION_ARACHNOS_EPIC_ARCHETYPE, 0, 0, "icon_costume_sets_0", "icon_costume_sets_1",},
        {REGION_ARACHNOS_WEAPON, 0, 0, "icon_costume_weapons_0", "icon_costume_weapons_1",},
        {REGION_ARACHNOS_AURA, 0, 0, "icon_costume_auras_0", "icon_costume_auras_1",}
    };
    static ScrollBar sb;
    static F32 targetCompressSc;
    const CostumeRegionSet *rset;
    const CostumeGeoSet* selectedGeoSet;
     int region_total = REGION_TOTAL_DEF;
     float y = REGION_Y;    
    int i;
    F32 area_ht = gatherRegionsHeight();
    F32 z = ZOOM_Z;
    int arachnosIcons = 0;
    Entity * e = playerPtr();
    const CharacterClass * cclass = e->db_id > 0 ? e->pchar->pclass : getSelectedCharacterClass();
    F32 xposSc, yposSc;

    calculatePositionScales(&xposSc, &yposSc);

    if (hb[0].icon0 == NULL)
    {
        for (i = 0; i < REGION_TOTAL_UI; ++i)
        {
            hybridElementInit(&hb[i]);
        }
        for (i = 0; i < REGION_ARACHNOS_TOTAL_UI; ++i)
        {
            hybridElementInit(&hb_arachnos[i]);
        }
    }

    if (game_state.editnpc)
    {
        //no compression in editnpc, list is too big
        targetCompressSc = 1.f;
            }
            else
            {
        targetCompressSc = MIN( 1.f, MAX_HT/area_ht ); 
            }

    // prevent popping by gradually transitioning the global compress scale
      if( compressSc < targetCompressSc )
    {
        compressSc += TIMESTEP*.03;
        if( compressSc > targetCompressSc )
            compressSc = targetCompressSc;
    }
    if( compressSc > targetCompressSc )
            {
        compressSc -= TIMESTEP*.03;
        if( compressSc < targetCompressSc )
            compressSc = targetCompressSc;
            }

    if (!storePartsContained->items)
    {
        storePartsContained->items = calloc(20, sizeof(SkuId));
    }
    else
    {
        storePartsContained->itemCount = 0;
    }

    selectedGeoSet = getSelectedGeoSet(mode);

    if (region_total > eaSize(&gCostumeMaster.bodySet[gCurrentBodyType]->originSet[gCurrentOriginSet]->regionSet))
        region_total = eaSize(&gCostumeMaster.bodySet[gCurrentBodyType]->originSet[gCurrentOriginSet]->regionSet);

    // costume sets hack /////////////////////////////////////////////////////////
    for(i = eaSize(&gCostumeMaster.bodySet[gCurrentBodyType]->originSet[gCurrentOriginSet]->costumeSets)-1; i >= 0; --i)
    {
        // make sure there's at least one good set before trying to display the region button
        if(!costumesetIsRestricted(gCostumeMaster.bodySet[gCurrentBodyType]->originSet[gCurrentOriginSet]->costumeSets[i]))
        {
            F32 setScale;
             if(drawCostumeRegionButton("CostumeSets", REGION_X*xposSc, (y + REGION_DEFAULT_SPACE*compressSc/2 -sb.offset)*yposSc, z, REGION_WD, REGION_DEFAULT_SPACE*compressSc, gCurrentRegion == REGION_SETS, 1, &hb[REGION_SETS], 0))
            {
                if( gCurrentRegion == REGION_SETS)
                    gCurrentRegion = -1;
                else
                    gCurrentRegion = REGION_SETS;
            }
            drawSectionArrow( (REGION_X + 280.f*(0.9f + 0.1f*hb[REGION_SETS].percent))*xposSc, (y + REGION_DEFAULT_SPACE*compressSc/2 -sb.offset)*yposSc, z, compressSc, hb[REGION_SETS].percent, rStoreScale[REGION_SETS]);
            y += REGION_DEFAULT_SPACE*compressSc;

            drawRegionSet( y-sb.offset, z, REGION_SETS, mode, NULL );
            setScale = 1.f - reg_ht[REGION_SETS]/ELEMENT_HT;
            if ( setScale > 0.f && costumesetIsLocked(gCostumeMaster.bodySet[gCurrentBodyType]->originSet[gCurrentOriginSet]->costumeSets[gSelectedCostumeSet]))
            {
                F32 locksc = (0.95f + hb[REGION_SETS].percent*0.05f);  //scaling temporary asset
                int alpha = (int) (setScale * 0xff);
                F32 cx = REGION_X;
                F32 cy = (y - REGION_DEFAULT_SPACE*compressSc/2 - sb.offset);
                F32 ht = REGION_DEFAULT_SPACE*compressSc;
                F32 scale = MINMAX( ht/DEFAULT_BAR_HT, 0.f, 1.f );

                //draw lock
                drawHybridStoreLockIconCustomTooltip((cx+(35.f*(0.95f+hb[REGION_SETS].percent*0.05f)))*xposSc, cy*yposSc, z+1.f, locksc*scale, 0xffffff00 | alpha, "CostumeSetLockedString");
            }
            y += reg_ht[REGION_SETS]*compressSc;
            break;
        }
    }
//////////////////////////////////////////////////////////////////////////////
    ///arachnos special case
    if( gFakeArachnosSoldier || gFakeArachnosWidow || (cclass && !e->pl->current_costume &&
        (class_MatchesSpecialRestriction(cclass, "ArachnosSoldier") ||
        class_MatchesSpecialRestriction(cclass, "ArachnosWidow"))) )
    {
        arachnosIcons = 1;
    }

    // manage the button expanding
       for( i = 0; i < region_total; i++)
    {
        int result;
        int bsets_found = 0, bsets_good = 0;
        int prevCartIndex = storePartsContained->itemCount;
        rset = costume_get_current_origin_set()->regionSet[i];

        if(regionsetIsRestricted(rset, &bsets_found))
            continue;

        if( (mode == COSTUME_EDIT_MODE_TAILOR))
        {
             float tx = (REGION_CENTER) - 150.f + RESET_WD/2.f;
               float ty = y + (BAR_OFF*LEFT_COLUMN_SQUISH + 4)*compressSc - sb.offset;
            int retVal;
            static HybridElement sButtonReset = {0, NULL, "ResetString", "icon_reset_0"};

             font( &game_12 );
            font_color( CLR_WHITE, CLR_RED );

            retVal = drawHybridButton(&sButtonReset, (tx-40.f)*xposSc, (ty - 5.f*compressSc)*yposSc, z+10.f, compressSc*0.9f, CLR_WHITE, HB_DRAW_BACKING | HB_DO_NOT_EDIT_ELEMENT);
            if( retVal >= D_MOUSEOVER )
            {
                collisions_off_for_rest_of_frame = 1;

                if( retVal == D_MOUSEHIT )
                {
                    tailor_RevertRegionToOriginal( costume_get_current_origin_set(), rset );
                    updateCostume();
                }
            }
            
        }

        //    Strange hack.    drawRegionSet will return -1 if the mode is NPC edit mode and the region is empty
        //    when it is -1, the if statement will be false and the region will not be drawn
        //    when it isn't -1, it will draw the region set ahead of where it should be, and then the rest of the pieces are drawn after.
          result = drawRegionSet(y + REGION_DEFAULT_SPACE*compressSc -sb.offset, z, i, mode, storePartsContained);

         if ( !( ( (mode == COSTUME_EDIT_MODE_NPC) || (mode == COSTUME_EDIT_MODE_NPC_NO_SKIN) ) && ( result == -1 ) ) )
        {    
            F32 cx = REGION_X;
            F32 cy = (y + REGION_DEFAULT_SPACE*compressSc/2 - sb.offset);
            int selected = gCurrentRegion == i;
            int valid = storePartsContained->itemCount == prevCartIndex;
            // Determine scale based off, ht given
            F32 ht = REGION_DEFAULT_SPACE*compressSc;
            F32 scale = MINMAX( ht/DEFAULT_BAR_HT, 0.f, 1.f );
            HybridElement * hb_current = arachnosIcons ? &hb_arachnos[i] : &hb[i];

            if ((rStoreScale[i] > 0.f || !selected) && !valid)
            {
                F32 locksc = (0.95f + hb[i].percent*0.05f);  //scaling temporary asset
                int alpha = (int) (rStoreScale[i] * 0xff);

                //draw lock
                drawHybridStoreLockIcon((cx+(35.f*(0.95f+hb[i].percent*0.05f)))*xposSc, cy*yposSc, z+1.f, locksc*scale, 0xffffff00 | alpha);

                //draw store icon (region)
                drawHybridStoreAddShoppingCartMenuButton(storePartsContained, prevCartIndex, storePartsContained->itemCount, (cx+280.f*(0.9f+hb[i].percent*0.1f))*xposSc, cy*yposSc, z+1.f, scale, 0xffffff00 | alpha, HB_STORE_CATEGORY);
            }

            if ( drawCostumeRegionButton( rset->displayName, cx*xposSc, cy*yposSc, z, REGION_WD, ht,    selected, valid, hb_current, 0 ) )
            {
                if( gCurrentRegion == i )
                    gCurrentRegion = -1;
                else
                {
                    gCurrentRegion = i;
                    sndPlay( "N_MenuExpand", SOUND_UI_ALTERNATE );
                }
            }
            drawSectionArrow( (cx + 280.f*(0.9f + 0.1f*hb_current->percent))*xposSc, cy*yposSc, z, compressSc, hb_current->percent, rStoreScale[i]);

            if((mode == COSTUME_EDIT_MODE_TAILOR))
                tailor_DisplayRegionTotal( rset->name, (REGION_X+220)*xposSc, (y+34*compressSc-sb.offset)*yposSc, z+5.f );

            y += REGION_DEFAULT_SPACE*compressSc;
            y += reg_ht[i]*compressSc;
        }
    }

    if( game_state.editnpc ) 
    {
        CBox box;
        compressSc = 1.f;
        BuildCBox( &box, 0, 0, 2000, 2000 );
         doScrollBarEx(&sb, 660, (y-REGION_Y), 20, REGION_Y, 5000, &box, 0, 1.f, 1.f );
    }
    // ----------
    // hacky section
    /*
    if (costume_isOtherClothPieceSelected(NULL))
    {
        // go through all the costume parts and reset all but the first found

        int selectedClothPieces = costume_isOtherClothPieceSelected(CCP_NONE);
        // if player has trenchcoat that means they can't have a cape (or any 'cape' location things)
        // trenchcoat supercedes any capes, so check for that:
        if (selectedClothPieces & CCP_WEDDING_VEIL)
        {
            if ( (selectedClothPieces & CCP_MAGIC_BOLERO) || (selectedClothPieces & CCP_TRENCH_COAT) )
            {

                if( EAINRANGE(REGION_UPPER, costume_get_current_origin_set()->regionSet) )
                {
                    CostumeRegionSet *mutable_upper_body = cpp_const_cast(CostumeRegionSet*)( costume_get_current_origin_set()->regionSet[REGION_UPPER] );
                    mutable_upper_body->currentBoneSet = 0;
                    updateCostume();
                }
            }
            if( selectedClothPieces & CCP_CAPE )
            {
                // player has trenchcoat selected. see if they have any capes
                if( EAINRANGE(REGION_CAPE,costume_get_current_origin_set()->regionSet))
                {
                    CostumeRegionSet *mutable_capes = cpp_const_cast(CostumeRegionSet*)( costume_get_current_origin_set()->regionSet[REGION_CAPE] );
                    if( mutable_capes && mutable_capes->currentBoneSet )
                    {
                        mutable_capes->currentBoneSet = 0;
                        updateCostume();
                    }
                }
            }
        }
        else if ( (selectedClothPieces & CCP_MAGIC_BOLERO) || (selectedClothPieces & CCP_TRENCH_COAT) )
        {
            if( selectedClothPieces & CCP_CAPE )
            {
                // player has trenchcoat selected. see if they have any capes
                if( EAINRANGE(REGION_CAPE,costume_get_current_origin_set()->regionSet))
                {
                    CostumeRegionSet *mutable_capes = cpp_const_cast(CostumeRegionSet*)( costume_get_current_origin_set()->regionSet[REGION_CAPE] );
                    if( mutable_capes && mutable_capes->currentBoneSet )
                    {
                        mutable_capes->currentBoneSet = 0;
                        updateCostume();
                    }
                }
            }
        }
    }
    */
}

//-----------------------------------------------------------------------------------------------------------------
// Random/Reset Costume ///////////////////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------------------------------------------
// if the mask name is found return the index, unmodified if not found
static bool findMask( const CostumeGeoSet* gset, char * tex, int* match_index )
{
    int i;
    devassert(match_index);

    if( eaSize( &gset->mask ) > 0 )
    {
        for( i = eaSize( &gset->mask )-1; i >= 0; i-- )
        {
            if( stricmp( gset->mask[i]->name, tex ) == 0 )
            {
                *match_index = i;
                return true;
            }
        }
    }
    return false;
}

void randomCostume()
{
    int offset;
    int i, k;
    Entity *e = playerPtr();
     char tex1[128] = {0};

    const CostumeOriginSet*    cset = costume_get_current_origin_set();
    int region_total = REGION_TOTAL_DEF;

    // @todo Selection and UI status fields should be removed from the otherwise static CTM data so that it can be shared read only (and gets smaller).
    // Those manipulations should be handled in mutable memory by the clients.
#ifdef TEST_CLIENT
    // We use 'out of band' selection stash so that TestClients can store otherwise static
    // CTM data in read only shared memory

    // Stash table of costume set sub object address (e.g. geo set) as key and the selected index (e.g. current info and mask) as the value
    // Used in place of the embedded 'current' indices in the Costume Set structures which cannot be written to if CTM data is in shared memory.
    // Those values are not appropriate to be stored with the otherwise static CTM data in any case.
    // @todo Currently this is used specifically for TestClients that put the CTM data in shared memory yet still want to
    // generate random costumes.

        // for each region, 1 selected boneset with a geo set selected index pair (info, mask) for each part
    StashTable costumeSetSelections = stashTableCreateAddress(GetBodyPartCount() + REGION_TOTAL_DEF);
#endif // TEST CLIENT

    srand( timerCpuTicks64() );

     offset = 1;

    if (region_total > eaSize(&costume_get_current_origin_set()->regionSet))
        region_total=eaSize(&costume_get_current_origin_set()->regionSet);

    for( k = 0; k < region_total; k++ )
    {
        const CostumeBoneSet*    bset;
        const CostumeRegionSet*    rset;
        int j = k;
        int count;
        int selected_bone_set = 0;

        j += offset;
        if( j >= region_total )
            j -= region_total;

        rset = cset->regionSet[j];

        if(regionsetIsRestricted(rset, NULL))
            continue;

GET_BSET:    // choose a random boneSet
        do
        {
            selected_bone_set = rand()%eaSize(&rset->boneSet);
            bset = rset->boneSet[selected_bone_set];
        } while(bonesetIsRestricted(bset) || !skuIdIsNull(SELLABLE_PRODUCT(bset)));
        
        count = eaSize( &bset->geo );
        for( i = 0; i < count; i++ )
        {
            const CostumeGeoSet* gset = bset->geo[i];
            int selected_gset_info = 0;
            int selected_gset_mask = 0;

            int found = FALSE;
            int j;
            int restrictedInfoCount = 0;
            gset = bset->geo[i];

            if( !(geosetIsRestricted(gset) || !skuIdIsNull(SELLABLE_PRODUCT(gset))) )
            {
                // we must go through all parts to see if there's even 1 valid piece
                for (j = 0; j < eaSize(&gset->info); ++j)
                {
                    if( (partIsRestricted(gset->info[j]) || !skuIdIsNull(SELLABLE_PRODUCT(gset->info[j]))) && 
                        !gset->restrict_info )
                    {
                        if (++restrictedInfoCount == eaSize(&gset->info))
                        {
                            goto GET_BSET;  // not ideal, but this is how we've been doing it
                        }
                    }
                    else
                    {
                        // there is something legal, stop searching
                        break;
                    }
                }

                if( gset->bodyPartName && costume_PartIndexFromName( e, gset->bodyPartName ) >= 0 )
                {
                    do
                    {
                        selected_gset_info = rand()%eaSize( &gset->info );    // randomly choose info index
                    }while( (partIsRestricted(gset->info[selected_gset_info]) || !skuIdIsNull(SELLABLE_PRODUCT(gset->info[selected_gset_info]))) && 
                            !gset->restrict_info );

                     if( !gset->info[selected_gset_info]->texName2 || stricmp( gset->info[selected_gset_info]->texName2, "Mask" ) == 0 )
                    {
                        int num_masks = eaSize( &gset->mask );
                        if(  num_masks && !gset->restrict_mask )
                        {
                            selected_gset_mask = rand()%num_masks;    // randomly choose mask index
                            while( gset->mask && (maskIsRestricted(gset->mask[selected_gset_mask]) ||
                                !skuIdIsNull(SELLABLE_PRODUCT(gset->mask[selected_gset_mask]))))
                            {
                                selected_gset_mask = rand()%num_masks;    // randomly choose mask index
                            }
                        }

                        // @todo why?
                        if( num_masks && !gset->restrict_mask )
                        {
                            if(  tex1[0] == 0 )
                                    strcpy( tex1, gset->mask[selected_gset_mask]->name );
                        }
                    }

                    // Can potentially replace the selected mask index here...
                    // @todo why?
                     if( tex1[0] != 0 )
                        findMask( gset, tex1, &selected_gset_mask );
                    else
                        strcpy( tex1, "none" );
                }

                   if ( gset->bodyPartName && stricmp( gset->bodyPartName, "Neck") == 0 )
                {
                    if( rand()%10 != 0 )
                    {
                        selected_gset_info = 0;
                        while( gset->info && !gset->restrict_info && 
                                (partIsRestricted(gset->info[selected_gset_info]) || !skuIdIsNull(SELLABLE_PRODUCT(gset->info[selected_gset_info]))) )
                            selected_gset_info++;
                    }
                }

                if ( gset->bodyPartName && stricmp( gset->bodyPartName, "EyeDetail" ) == 0 )
                {
                    if( rand()%10 != 0 )
                    {
                        selected_gset_info = 0;
                        while( gset->info && !gset->restrict_info && 
                                (partIsRestricted(gset->info[selected_gset_info]) || !skuIdIsNull(SELLABLE_PRODUCT(gset->info[selected_gset_info]))) )
                            selected_gset_info++;
                    }
                }

                if( gset->bodyPartName && stricmp( gset->bodyPartName, "Shoulders" ) == 0 )
                {
                    if( rand()%4 != 0 )
                    {
                        selected_gset_info = 0;
                        while( gset->info && !gset->restrict_info && partIsRestricted(gset->info[selected_gset_info]) )
                            selected_gset_info++;
                    }
                }
            }

            // Track the current selection for the geo set info and mask
            #ifdef TEST_CLIENT
            if (selected_gset_info || selected_gset_mask)
                {
                    stashAddressAddInt(costumeSetSelections, gset, selected_gset_info | selected_gset_mask<<16, true);    // encode both indices into one int
                }
            #else
                (cpp_const_cast(CostumeGeoSet*)(gset))->currentInfo = selected_gset_info;
                (cpp_const_cast(CostumeGeoSet*)(gset))->currentMask = selected_gset_mask;
            #endif
        }

        // Track the current selected bone set index
        #ifdef TEST_CLIENT
            if (selected_bone_set)    // to the selection stash for this region
                stashAddressAddInt(costumeSetSelections, rset, selected_bone_set, true);
        #else
            (cpp_const_cast(CostumeRegionSet*)(rset))->currentBoneSet = selected_bone_set;
        #endif
    }

    gColorsLinked = TRUE;

    // apply our selections to the current costume
#ifdef TEST_CLIENT
    updateCostumeFromSelectionSet(costumeSetSelections);
    stashTableDestroy(costumeSetSelections); costumeSetSelections = NULL;
#else
    updateCostume();    // apply selections embedded in the CTM 'current' fields to make a costume
#endif

    // set colors
    {
        Costume* mutable_costume = entGetMutableCostume(e);
        int colorIndices[COLOR_TOTAL];
        Color sc;

        colorIndices[COLOR_SKIN] = rand()%eaSize( (F32***)&cset->skinColors[0]->color );
        colorIndices[COLOR_1] = rand()%eaSize( (F32***)&cset->bodyColors[0]->color );
        colorIndices[COLOR_2] = rand()%eaSize( (F32***)&cset->bodyColors[0]->color );
        colorIndices[COLOR_3] = rand()%eaSize( (F32***)&cset->bodyColors[0]->color );
        colorIndices[COLOR_4] = rand()%eaSize( (F32***)&cset->bodyColors[0]->color );

        sc = getColor( colorIndices[COLOR_SKIN], COLOR_SKIN );

        costume_SetSkinColor( mutable_costume, sc );
        gLinkedColor1 = getColor(colorIndices[COLOR_1], COLOR_1);
        gLinkedColor2 = getColor(colorIndices[COLOR_2], COLOR_2);
        copyLinkedColorIntoAllOthers( e );
        updateColors();
    }

    costume_Apply( e );
}

void matchPartByName(char * region, char *boneset, char *name, char * fx, char * geo, char * texture1, char *texture2)
{
    Entity *e = playerPtr();
    const CostumeOriginSet    *cset = costume_get_current_origin_set();
    CostumePart cpart;
    ZeroStruct(&cpart);

    cpart.regionName = region;
    cpart.bodySetName = boneset;
    cpart.pchName = name;
    cpart.pchFxName = fx;
    cpart.pchGeom = geo;
    cpart.pchTex1 = texture1;
    cpart.pchTex2 = texture2;

    costume_MatchPart(e, &cpart, cset, 0, name);
}

void costume_ZeroStruct( CostumeOriginSet *cset );

void updateCostumeWeapons()
{
    int x, y, i, j, k;

    for( x = 0; x < gCostumeMaster.bodyCount; x++ )
    {
        for( y = 0; y < gCostumeMaster.bodySet[x]->originCount; y++ )
        {    
            const CostumeOriginSet *cset = gCostumeMaster.bodySet[x]->originSet[y];
            int regionMin = REGION_WEAPON;
            int regionMax = REGION_SHIELD;

            // HACK to deal with different number of region types
            if (strcmp(cset->name, "ArachnosSoldier") == 0 || strcmp(cset->name, "ArachnosWidow") == 0)
            {
                regionMin = regionMax = REGION_ARACHNOS_WEAPON;
            }

             for( i = regionMin; i <= regionMax && i < cset->regionCount; i++ )
            {
                CostumeRegionSet* mutable_rset = cpp_const_cast(CostumeRegionSet*)(cset->regionSet[i]);
                int lastBoneSet = mutable_rset->currentBoneSet;

                // find the next bone set that will work this costume
                while( bonesetIsRestricted_MustHaveStoreProduct( mutable_rset->boneSet[mutable_rset->currentBoneSet]))
                {
                    ++mutable_rset->currentBoneSet;
                    mutable_rset->currentBoneSet %= mutable_rset->boneCount;
                    if( lastBoneSet == mutable_rset->currentBoneSet )
                    {
                        // no piece found, set to zero
                        mutable_rset->currentBoneSet = 0;
                        break;
                    }
                }

                for( j = eaSize(&mutable_rset->boneSet)-1; j >= 0; j-- )
                {
                    const CostumeBoneSet *bset = mutable_rset->boneSet[j];

                    if(bonesetIsRestricted_MustHaveStoreProduct(bset))
                        continue;

                    for( k = eaSize(&bset->geo)-1; k >= 0; k-- )
                    {
                        CostumeGeoSet* mutable_gset = cpp_const_cast(CostumeGeoSet*)(bset->geo[k]);

                        if( mutable_gset->info && !mutable_gset->restrict_info )
                        {
                            int lastInfo = mutable_gset->currentInfo;

                            // find the next part that will work with this costume
                            while( partIsRestricted_MustHaveStoreProduct(mutable_gset->info[mutable_gset->currentInfo]) )
                            {
                                ++mutable_gset->currentInfo;
                                mutable_gset->currentInfo %= mutable_gset->infoCount;
                                if( lastInfo == mutable_gset->currentInfo )
                                {
                                    //no piece found, set to zero
                                    mutable_gset->currentInfo = 0;
                                    break;
                                }
                            }
                        }
                        else
                        {
                            mutable_gset->currentInfo = 0;
                        }

                        if( mutable_gset->mask && !mutable_gset->restrict_mask )
                        {
                            int lastMask = mutable_gset->currentMask;
                            // find the next mask that will work with this costume
                            while( maskIsRestricted_MustHaveStoreProduct(mutable_gset->mask[mutable_gset->currentMask]) )
                            {
                                ++mutable_gset->currentMask;
                                mutable_gset->currentMask %= mutable_gset->maskCount;
                                if ( lastMask == mutable_gset->currentMask )
                                {
                                    //no piece found, set to zero
                                    mutable_gset->currentMask = 0;
                                    break;
                                }
                            }
                        }
                        else
                        {
                            mutable_gset->currentMask = 0;
                        }
                    }
                }
            }
        }
    }
}

void costumeResetCostumePiecesToDefault( int do_all, int init_costume )
{
    int i, j, k , x, y;


    for( x = 0; x < gCostumeMaster.bodyCount; x++ )
    {
        for( y = 0; y < gCostumeMaster.bodySet[x]->originCount; y++ )
        {    
            const CostumeOriginSet * cset = gCostumeMaster.bodySet[x]->originSet[y];

            if( !do_all && ( x != gCurrentBodyType || y != gCurrentOriginSet ) )
                continue;

            for( i = 0; i < cset->regionCount; i++ )
            {
                const CostumeRegionSet* rset = cset->regionSet[i];
                CostumeRegionSet* mutable_rset = cpp_const_cast(CostumeRegionSet*)(rset);
                //special case since weapons are not always initialized at the same time as the costume
                //Weapons must be defaulted to a non-store item always, while some geosets ONLY have store items.
                int isWeapon = i == REGION_WEAPON || i == REGION_SHIELD;
                if (init_costume)
                {
                    mutable_rset->currentBoneSet = 0;
                }
                while( bonesetIsRestrictedEx( rset->boneSet[rset->currentBoneSet], false, isWeapon))
                {
                    mutable_rset->currentBoneSet++;
                    if(rset->currentBoneSet >= rset->boneCount)
                    {
                        mutable_rset->currentBoneSet = 0;
                        break;
                    }
                }

                for( j = eaSize(&rset->boneSet)-1; j >= 0; j-- )
                {
                    const CostumeBoneSet* bset = rset->boneSet[j];

                    if(bonesetIsRestrictedEx(bset, false, isWeapon))
                        continue;

                    for( k = eaSize(&bset->geo)-1; k >= 0; k-- )
                    {
                        CostumeGeoSet* gset = cpp_const_cast(CostumeGeoSet*)(bset->geo[k]);    // mutable_gset
                        if (init_costume)
                        {
                            gset->currentInfo = 0;
                        }
                        while( gset->info && !gset->restrict_info && partIsRestrictedEx(gset->info[gset->currentInfo], NULL, false, isWeapon) )
                        {
                            gset->currentInfo++;
                            if( gset->currentInfo >= eaSize(&gset->info) )
                            {
                                gset->currentInfo = 0;
                                break;
                            }
                        }
                        if (init_costume)
                        {
                            gset->currentMask = 0;
                        }
                        while( gset->mask && !gset->restrict_mask && maskIsRestrictedEx(gset->mask[gset->currentMask], isWeapon) )
                        {
                            gset->currentMask++;
                            if( gset->currentMask == eaSize(&gset->mask) )
                            {
                                gset->currentMask = 0;
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
}

void resetCostume( int do_all )
{
    Entity *e = playerPtr();
    int i;

    gCurrentRegion = REGION_SETS;
    gSelectedCostumeSet = -1;
    gActiveColor = 0;
    gColorsLinked = TRUE;

    if( !e )
        return;

    costumeResetCostumePiecesToDefault(do_all, 1);

    resetColorsToDefault(e);

    for( i = 0; i < NUM_3D_BODY_SCALES; i++ )
    {
        int j;
        for( j = 0; j < 3; j++ )
        {
            Costume* mutable_costume = costume_as_mutable_cast(e->costume);
            mutable_costume->appearance.f3DScales[i][j] = 0.f;
        }
    }    

    changeBoneScaleEx(e->seq, &e->costume->appearance);
    copyLinkedColorIntoAllOthers( e );
    updateColors();
    costume_Apply( e );

    updateCostume();
}

static int drawLoadCostume()
{
    AtlasTex *loadButton;
    loadButton = atlasLoadTexture(SELECT_FROM_UISKIN("CC_Load_H", "CC_Load_V", "CC_Load_P"));
    return drawNLMIconButton(880, 48, 5000, 65, 20, "LoadCostume", loadButton, -5, 0);
}
static void drawBackCostume(F32 cx, F32 cy)
{
    int flag = !(eaSize(&gCostStack));
    static HybridElement sButtonPrevious = {0, "PreviousString", "previousCostumeButton", "icon_costume_previous"};
    if( D_MOUSEHIT == drawHybridButton(&sButtonPrevious, cx, cy, ZOOM_Z, 1.f, CLR_WHITE, flag | HB_DRAW_BACKING ) )
    {
        CostumeSave *save = (CostumeSave *)eaPop(&gCostStack);
        sndPlay("N_Undo", SOUND_GAME);
        costumeApplySave(save);
        costumeClearSave(save);
        free(save);
    }
}

static void drawRandomCostume(F32 cx, F32 cy)
{
    static HybridElement sButtonRandom = {0, "RandomString", "randomCostumeButton", "icon_costume_random"};
    if( D_MOUSEHIT == drawHybridButton(&sButtonRandom, cx, cy, ZOOM_Z, 1.f, CLR_WHITE, HB_DRAW_BACKING ) )
    {
        sndPlay("N_RandomCostume", SOUND_GAME);
        eaPush(&gCostStack,costumeBuildSave(NULL));
        randomCostume();
    } 
} 
static void drawResetCostume(F32 cx, F32 cy)
{
    static HybridElement sButtonClear = {0, "ClearString", "clearCostumeButton", "icon_costume_clearall"};
    if( D_MOUSEHIT == drawHybridButton(&sButtonClear, cx, cy, ZOOM_Z, 1.f, CLR_WHITE, HB_DRAW_BACKING ) )
    {
        sndPlay("N_CloseCostume", SOUND_GAME);
        resetCostume(0);
    }
}

//-----------------------------------------------------------------------------------------------------------------
// Save Costume ///////////////////////////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------------------------------------------

void regionBuildSave( const CostumeRegionSet *rset, RegionSave * rsave )
{
     const CostumeBoneSet    *bset = rset->boneSet[rset->currentBoneSet];
    int i;

     if( bset->name)
        rsave->boneset = bset->name;
    
    for( i = 0; i < eaSize(&bset->geo); i++ )
    {
        const CostumeGeoSet *gset = bset->geo[i];
        if (gset->faces) { 
        } else {
            GeoSave *gsave = calloc(sizeof(GeoSave),1);
            const CostumeTexSet *tset = gset->info[gset->currentInfo];

            gsave->setName = gset->bodyPartName;
            gsave->geoName = tset->geoName;
            gsave->texName1 = tset->texName1;
            gsave->texName2 = tset->texName2;

            if (!tset->texName2)
            {
                Errorf("File %s, Body part %s, geometry part %s is missing a tex2 name", bset->filename, gset->bodyPartName, tset->geoName);
            }
            if( tset->texName2 && stricmp(tset->texName2, "mask") == 0 )
            {
                if( gset->mask )
                    gsave->maskName = (gset->currentMask>=0)?gset->mask[gset->currentMask]->name:gset->info[gset->currentInfo]->texName2;
            }
            if( tset->fxName )
                gsave->fxName = tset->fxName;    
            eaPush(&rsave->geoSave, gsave );        
        }
    }
}

void costumeClearSave( CostumeSave *cs )
{
    eaDestroy(&cs->head.geoSave);
    eaDestroy(&cs->upper.geoSave);
    eaDestroy(&cs->lower.geoSave);
    eaDestroy(&cs->weapon.geoSave);
    eaDestroy(&cs->shield.geoSave);
    eaDestroy(&cs->cape.geoSave);
    eaDestroy(&cs->glowie.geoSave);

    memset(cs, 0, sizeof(CostumeSave) );
}


CostumeSave *costumeBuildSave(CostumeSave * cs) 
{
    Entity                *e        = playerPtr();
    const CostumeOriginSet    *cset    = costume_get_current_origin_set();

    if (!cs)
    {
        cs        = (CostumeSave *)malloc(sizeof(CostumeSave));
    }
    else 
    {
        costumeClearSave(cs);
    }

    cs->gender            = gCurrentBodyType;
    cs->fScale            = e->costume->appearance.fScale;
    cs->fBoneScale        = e->costume->appearance.fBoneScale;
    cs->fHeadScale        = e->costume->appearance.fHeadScale;
    cs->fShoulderScale    = e->costume->appearance.fShoulderScale;
    cs->fChestScale        = e->costume->appearance.fChestScale;
    cs->fWaistScale        = e->costume->appearance.fWaistScale;
    cs->fHipScale        = e->costume->appearance.fHipScale;
    cs->fLegScale        = e->costume->appearance.fLegScale;
    cs->fArmScale        = e->costume->appearance.fArmScale;

    eaCreate(&cs->head.geoSave);
    if( EAINRANGE(REGION_HEAD, cset->regionSet ))
        regionBuildSave( cset->regionSet[REGION_HEAD], &cs->head );
    eaCreate(&cs->upper.geoSave);
    if( EAINRANGE(REGION_UPPER, cset->regionSet ))
        regionBuildSave( cset->regionSet[REGION_UPPER], &cs->upper );
    eaCreate(&cs->lower.geoSave);
    if( EAINRANGE(REGION_LOWER, cset->regionSet ))
        regionBuildSave( cset->regionSet[REGION_LOWER], &cs->lower);
    eaCreate(&cs->weapon.geoSave);
    eaCreate(&cs->shield.geoSave);
    eaCreate(&cs->cape.geoSave);
    eaCreate(&cs->glowie.geoSave);

    if( !gCostumeCritter )
    {
        if( EAINRANGE(REGION_WEAPON, cset->regionSet ))
            regionBuildSave( cset->regionSet[REGION_WEAPON], &cs->weapon );
        if( EAINRANGE(REGION_SHIELD, cset->regionSet ))
            regionBuildSave( cset->regionSet[REGION_SHIELD], &cs->shield );
        if( EAINRANGE(REGION_CAPE, cset->regionSet ))
            regionBuildSave( cset->regionSet[REGION_CAPE], &cs->cape );
        if( EAINRANGE(REGION_GLOWIE, cset->regionSet ))
            regionBuildSave( cset->regionSet[REGION_GLOWIE], &cs->glowie );
    }

    cs->color1 = getIndexFromColor(getSelectedColor(COLOR_1, COSTUME_EDIT_MODE_STANDARD));
    cs->color2 = getIndexFromColor(getSelectedColor(COLOR_2, COSTUME_EDIT_MODE_STANDARD));
    cs->color3 = getIndexFromColor(getSelectedColor(COLOR_3, COSTUME_EDIT_MODE_STANDARD));
    cs->color4 = getIndexFromColor(getSelectedColor(COLOR_4, COSTUME_EDIT_MODE_STANDARD));
    cs->colorSkin = getSkinIndexFromColor(getSelectedColor(COLOR_SKIN, COSTUME_EDIT_MODE_STANDARD));
    return cs;
}

static bool stringsmatch(const char *a, const char *b)
{
    return a == b || a && b && !stricmp(a,b);
}

bool costumeApplyGeoSave( GeoSave *gs, CostumeGeoSet *gset )
{
    int i,j;
    for( i = 0; i < eaSize(&gset->info); i++ )
    {
        const CostumeTexSet *tset = gset->info[i];
        if( partIsRestrictedEx(tset, NULL, false, false) )
            continue;

        if( stringsmatch(gs->geoName, tset->geoName) && stringsmatch(gs->texName1, tset->texName1) && 
            stringsmatch(gs->texName2, tset->texName2) && stringsmatch(gs->fxName, tset->fxName) )
        {
            if(stringsmatch(tset->texName2, "mask"))
            {
                if( gset->mask )
                {
                    for( j = 0; j < eaSize(&gset->mask); j++ )
                    {
                        if(stringsmatch(gs->maskName, gset->mask[j]->name) && !maskIsRestricted(gset->mask[j]))
                        {
                            gset->currentInfo = i;
                            gset->currentMask = j;
                        }
                    }
                }
                else if (gset->masks)
                {
                    for( j = 0; j < eaSize(&gset->masks); j++ )
                    {
                        if(stringsmatch(gs->maskName, gset->masks[j]))
                        {
                            gset->currentInfo = i;
                            gset->currentMask = j;
                        }
                    }
                }
            }
            else gset->currentInfo = i;
            return true;
        }
    }
    return false;
}

void costumeApplyRegionSave( RegionSave *rs, CostumeRegionSet *rset  )
{
    int i,j,k;

    if(regionsetIsRestricted(rset, NULL))
        return;

    for( i = 0; i < eaSize(&rset->boneSet); i++ )
    {
        const CostumeBoneSet *bset = rset->boneSet[i];
        if( stricmp( rs->boneset, bset->name ) == 0 && !bonesetIsRestricted(bset) )
        {
            if(!eaSize(&rs->geoSave) && !eaSize(&bset->geo))
                rset->currentBoneSet = i;

            for( j = 0; j < eaSize(&rs->geoSave); j++ )
            {
                for( k = 0; k < eaSize(&bset->geo); k++ )
                {
                    if(bset->geo[k]->faces) continue;
                    if( stricmp( rs->geoSave[j]->setName, bset->geo[k]->bodyPartName ) == 0  && !geosetIsRestricted(bset->geo[k]))
                    {
                        CostumeGeoSet* mutable_gset = cpp_const_cast(CostumeGeoSet*)(bset->geo[k]);
                        if (costumeApplyGeoSave( rs->geoSave[j], mutable_gset ))
                        {
                            (cpp_const_cast(CostumeRegionSet*)(rset))->currentBoneSet = i;
                            (cpp_const_cast(CostumeBoneSet*)(bset))->currentSet = k;
                        }
                    }
                }
            }
        }
    }
}

int    costumeApplySave( CostumeSave *cs )
{
    Entity *e = playerPtr();
    const CostumeOriginSet *cset;
    int colorIndexes[COLOR_TOTAL];

    if(    cs->gender!=AVATAR_GS_FEMALE&&
        cs->gender!=AVATAR_GS_MALE&&
        cs->gender!=AVATAR_GS_HUGE )
        return false;

    if( gCurrentGender == AVATAR_GS_FEMALE && ( cs->fScale > 36 || cs->fScale < -27 ) )
        return false;
    else if( gCurrentGender != AVATAR_GS_FEMALE && (cs->fScale > 33 || cs->fScale < -33))
        return false;

    if( !costume_ValidateScales(e->seq, cs->fBoneScale, cs->fShoulderScale, cs->fChestScale, cs->fWaistScale, cs->fHipScale, cs->fLegScale, cs->fArmScale) )
        return false;

    cset = gCostumeMaster.bodySet[cs->gender]->originSet[gCurrentOriginSet];

    if( EAINRANGE(REGION_HEAD, cset->regionSet ))
        costumeApplyRegionSave( &cs->head, cpp_const_cast(CostumeRegionSet*)( cset->regionSet[REGION_HEAD] ) );
    if( EAINRANGE(REGION_UPPER, cset->regionSet ))
        costumeApplyRegionSave( &cs->upper, cpp_const_cast(CostumeRegionSet*)( cset->regionSet[REGION_UPPER] ) );
    if( EAINRANGE(REGION_LOWER, cset->regionSet ))
        costumeApplyRegionSave( &cs->lower, cpp_const_cast(CostumeRegionSet*)( cset->regionSet[REGION_LOWER] ) );
    if( EAINRANGE(REGION_WEAPON, cset->regionSet ))
        costumeApplyRegionSave( &cs->weapon, cpp_const_cast(CostumeRegionSet*)( cset->regionSet[REGION_WEAPON] ) );
    if( EAINRANGE(REGION_SHIELD, cset->regionSet ))
        costumeApplyRegionSave( &cs->weapon, cpp_const_cast(CostumeRegionSet*)( cset->regionSet[REGION_SHIELD] ) );
    if( EAINRANGE(REGION_CAPE, cset->regionSet ))
        costumeApplyRegionSave( &cs->cape, cpp_const_cast(CostumeRegionSet*)( cset->regionSet[REGION_CAPE] ) );
    if( EAINRANGE(REGION_GLOWIE, cset->regionSet ))
        costumeApplyRegionSave( &cs->glowie, cpp_const_cast(CostumeRegionSet*)( cset->regionSet[REGION_GLOWIE] ) );

    gColorsLinked = TRUE;

    updateCostume();
    

    colorIndexes[COLOR_SKIN] = cs->colorSkin;
    colorIndexes[COLOR_1] = cs->color1;
    colorIndexes[COLOR_2] = cs->color2;
    colorIndexes[COLOR_3] = cs->color3;
    colorIndexes[COLOR_4] = cs->color4;
    {
        Color sc = getColor( colorIndexes[COLOR_SKIN], COLOR_SKIN );
        Costume* mutable_costume = costume_as_mutable_cast(e->costume);
        int i;

        costume_SetSkinColor( mutable_costume, sc );
        for (i=0; i<4; i++) 
            costume_PartSetColor( mutable_costume, 0, i, getColor( colorIndexes[COLOR_1+i],    COLOR_1+i ) );
    }

    updateColors();
    costume_Apply( e );
    return true;
}

void convergeFaceScale()
{
    int i,j;
    bool changed = false;
    Appearance *app = cpp_const_cast(Appearance*)( &playerPtr()->costume->appearance );


    if( !gConvergingFace )
        return;

    for( i = 0; i < NUM_3D_BODY_SCALES; i++ )
    {
        for( j = 0; j < 3; j++ )
        {
            if( app->f3DScales[i][j] - gFaceDest[i][j] > .001f)
            {
                changed = true;
                app->f3DScales[i][j] -= gFaceDestSpeed[i][j]*TIMESTEP;
                if( app->f3DScales[i][j] < gFaceDest[i][j] )
                    app->f3DScales[i][j] = gFaceDest[i][j];
            }
            else if( gFaceDest[i][j] - app->f3DScales[i][j] > .001f)
            {
                changed = true;
                app->f3DScales[i][j] += gFaceDestSpeed[i][j]*TIMESTEP;
                if( app->f3DScales[i][j] > gFaceDest[i][j] )
                    app->f3DScales[i][j] = gFaceDest[i][j];
            }
        }
    }

    if( !changed )
    {
        gConvergingFace = false;
        sndStop(SOUND_UI_ALTERNATE, 0.f);
    }
    else
    {
        sndPlay("N_Slider_loop", SOUND_UI_ALTERNATE);
    }

}

void clearCostumeStack() {
    if (!gCostStack) return;
    while (eaSize(&gCostStack) > 0) { // Clear the random costume stack
        CostumeSave *s = eaPop(&gCostStack);
        costumeClearSave(s);
        free(s);
    }
}

static void pccFixCostume(Entity *e)
{
    if ( pccFixupCostume )
    {
        int genderMatch = 0;
        switch (pccFixupCostume->appearance.bodytype)
        {
        case kBodyType_Male:
            if ( gCurrentGender == AVATAR_GS_MALE )
            {
                genderMatch = 1;
            }
            break;
        case kBodyType_Female:
            if ( gCurrentGender == AVATAR_GS_FEMALE )
            {
                genderMatch = 1;
            }
            break;
        case kBodyType_Huge:
            if ( gCurrentGender == AVATAR_GS_HUGE )
            {
                genderMatch = 1;
            }
            break;
        }
        if ( genderMatch )
        {
            Appearance genderAppearance;
            Costume* mutable_costume;
            int i;
            resetCostume(0);
            gColorsLinked = false;
            genderAppearance = e->costume->appearance;
            e->pl->costume[0] = costume_clone( costume_as_const(pccFixupCostume) );
            mutable_costume = e->pl->costume[0];
            e->costume = costume_as_const(mutable_costume);
            //    the only thing we really want to copy is the scales from the gender screen... (not stuff like color and so on)
            for (i = 0; i < ARRAY_SIZE(genderAppearance.fScales); i++)
            {
                mutable_costume->appearance.fScales[i] = genderAppearance.fScales[i];
            }

            //    resetCostume() destroys the face scales... sad face =/ EL. so we re-init them
            for( i = 0; i < NUM_3D_BODY_SCALES; i++ )
            {
                int j;
                for( j = 0; j < 3; j++ )
                {
                    mutable_costume->appearance.f3DScales[i][j] = pccFixupCostume->appearance.f3DScales[i][j];
                }
            }    
            costume_setCurrentCostume(e,0);
            updateCostume();
            costume_Apply(e);
        }
        else
        {
            resetCostume(0);
        }
        pccFixupCostume = NULL;
    }
}

void costumeMenuExitCode(void)
{
    Entity *e = playerPtr();
    const CostumeGeoSet *gset = getSelectedGeoSet(COSTUME_EDIT_MODE_STANDARD);
    repositioningCamera = 1;
    texEnableThreadedLoading();
    if (e && e->costume)
    {
        Costume* mutable_costume = costume_as_mutable_cast(e->costume);
        mutable_costume->appearance.convertedScale = true;
    }
    toggle_3d_game_modes(SHOW_NONE);
    if (gset)
    {
        CostumeGeoSet* mutable_gset = cpp_const_cast(CostumeGeoSet*)(gset);
        mutable_gset->isOpen = false;
    }
}

void costumeMenuEnterCode(void)
{
    Entity * e = playerPtr();
    int oldRotationAngle = 0;

    toggle_3d_game_modes(SHOW_TEMPLATE);
#ifndef TEST_CLIENT
    seqClearState( playerPtrForShell(1)->seq->state );
#endif
    seqSetState( playerPtrForShell(1)->seq->state, TRUE, STATE_PROFILE ); // freeze the player model
    seqSetState( playerPtrForShell(1)->seq->state, TRUE, STATE_HIGH ); // freeze the player model

    costume_fillExtraData( NULL, &gCostumeMaster, false );    // @todo SHAREDMEM review
    initAnimComboBox(0);
    comboAnims.currChoice = 0;

    costumeInit();

    texDisableThreadedLoading();

    oldRotationAngle = playerTurn_GetTicks();
    playerTurn(oldRotationAngle);

    gZoomed_in = 0;
    zoomAvatar(true, avatar_getDefaultPosition(0,1));

    setDefaultCostume(true);

    game_state.fxkill = 1; // a bit extreme, only way I could fix weird stacking error though - CW

    pccFixCostume(e);
    clearSS();
}

void costumeInitHideStorePiecesState(HideStorePiecesStates state)
{
    //initialize checkbox once per account logged in.  Status should stick until the client is closed or account changes.
    if (state == HIDE_STORE_PIECES_UNITIALIZED) //uninitialized
    {
        if (inventoryClient_GetAcctAuthoritativeState() == ACCOUNT_SERVER_UP && AccountIsReallyVIPAfterServerSelected())
        {
            hideStorePiecesState = HIDE_STORE_PIECES_UNCHECKED;
        }
        else
        {
            hideStorePiecesState = HIDE_STORE_PIECES_CHECKED;
        }
    }
    else
    {
        hideStorePiecesState = state;
    }
}

void costumeInit()
{
    if( !init )
    {
        resetCostume(1);
        costumeInitHideStorePiecesState(hideStorePiecesState);
        init = 1;
    }
}

int hasStoreCostumeParts()
{
    return sStorePartsContained.itemCount != 0;
}

static void costumeMenuPassThroughCode(void)
{
    Entity *e = playerPtr();
    int cc_id = 0;
    int newTransaction = 0;
#if !TEST_CLIENT
    if (getPCCEditingMode() > 0)
    {
        cc_id = NLM_PCCCreationMenu()->transactionId;
        newTransaction = cc_id != pcc_transactionID;
    }
#endif
    toggle_3d_game_modes(SHOW_TEMPLATE);
    if (!checkCostumeMatchesBody(e->costume->appearance.bodytype, gCurrentGender))
    {
        setDefaultCostume(false);
    }

    if (newTransaction)
    {
        int i;
        const CostumeOriginSet* cset = costume_get_current_origin_set();
        Costume* mutable_costume = costume_as_mutable_cast(e->costume);

        costume_SetSkinColor( mutable_costume, getColor( rand()%eaSize( (F32***)&cset->skinColors[0]->color ), COLOR_SKIN ) );
        for (i=0; i<4; i++) 
            costume_PartSetColor( mutable_costume, 0, i, getColor( rand()%eaSize( (F32***)&cset->bodyColors[0]->color ),    COLOR_1+i ) );

        if (getPCCEditingMode() > 0)
            pcc_transactionID = cc_id;
        else
            cc_transactionID = cc_id;

        eaCreate(&gCostStack);
        if ( !gCostumeCritter )
        {
            costumeInit();
        }
    }
    setDefaultCostume(false);
    costume_Apply(e);
    pccFixCostume(e);
    toggle_3d_game_modes(SHOW_NONE);
}
NonLinearMenuElement cc_costume_NLME = { MENU_COSTUME, { "CC_CostumeString", 0, 0 },
                                                0, 0, costumeMenuEnterCode, costumeMenuPassThroughCode,  costumeMenuExitCode };

NonLinearMenuElement PCC_costume_NLME = { MENU_COSTUME, { "CC_CostumeString", 0, 0 },
                                                0, 0, costumeMenuEnterCode, costumeMenuPassThroughCode,  costumeMenuExitCode };

//-----------------------------------------------------------------------------------------------------------------
// King of the costume functions //////////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------------------------------------------

int repositionCamera(int doReposition)
{
    int continueRepositioning = 0;
    if (doReposition)
    {
        int turnedClock = 0;
        int desiredAngle = 10;
        int currentAngle = playerTurn_GetTicks();
        int diffAngle = (desiredAngle - currentAngle) % 360;
        continueRepositioning = 1;
        if (diffAngle < 0)
        {
            diffAngle += 360;
        }
        if (diffAngle <= 180)
        {
            playerTurn( 5 * (TIMESTEP) );    //    clockwise 
            turnedClock = 1;
        }
        else
        {
            playerTurn( -5 * (TIMESTEP) );    //    ccw
        }

        //    If we overshot, just stop spinning
        currentAngle = playerTurn_GetTicks();
        diffAngle = (desiredAngle - currentAngle) % 360;
        if (diffAngle < 0)
        {
            diffAngle += 360;
        }

        if ((diffAngle < 180) && !turnedClock)
        {
            continueRepositioning = 0;
        }
        else if ((diffAngle >= 180) && turnedClock)
        {
            continueRepositioning = 0;
        }
    }
    return continueRepositioning;
}

void costumeHideStorePiecesButtonPress(void)
{
    if (hideStorePiecesState == HIDE_STORE_PIECES_CHECKED)
    {
        dialogStd( DIALOG_OK, "showStorePiecePopupText", NULL, NULL, NULL, NULL, 0 );
        hideStorePiecesState = HIDE_STORE_PIECES_UNCHECKED;
        costumeResetCostumePiecesToDefault(1, 0);
    }
    else
    {
        dialogStd( DIALOG_YES_NO, "hideStorePiecePopupText", NULL, NULL, confirmHideStorePiecesState, NULL, 0 );
    }
}

void costumeMenu()
{
    Entity *e, *pe;
    const CostumeGeoSet* selectedGeoSet;
    int cc_id = 0;
    int w, h;
    float ht = 0;
    int newTransaction = 0;
    F32 bottomButtonY = 710.f;
    static HybridElement sButtonSave = {0, "SaveString", "saveCostumeButton", "icon_costume_save"};
    static HybridElement sButtonLoad = {0, "LoadString", "loadCostumeButton", "icon_costume_load"};
    const char * hideStoreTextureName = hideStorePiecesState == HIDE_STORE_PIECES_CHECKED ?  "loginscreen_checkfield_selected" : "loginscreen_checkfield";
    const char * hideStoreTooltip = hideStorePiecesState == HIDE_STORE_PIECES_CHECKED ? "showStorePiecesTooltip" : "hideStorePiecesTooltip";
    static HybridElement sButtonHideStore = {0};
    F32 xposSc, yposSc, textXSc, textYSc, xp, yp;

    windowClientSizeThisFrame( &w, &h );

    pe = playerPtr();
    setScalingModeAndOption(SPRITE_Y_UNIFORM_SCALE, SSO_SCALE_TEXTURE);
    calculatePositionScales(&xposSc, &yposSc);
    calculateSpriteScales(&textXSc, &textYSc);

    if (getPCCEditingMode() < 0)
        setPCCEditingMode(getPCCEditingMode()*-1);

    e = playerPtr();

     if( updateScrollSelectors( 32, 712 ) )
        globalAlpha = stateScale(globalAlpha, 1);
    else
        globalAlpha = stateScale(globalAlpha, 0);

    if (!getPCCEditingMode())
    {
        setScalingModeAndOption(SPRITE_XY_SCALE, SSO_SCALE_ALL);
        if(drawHybridCommon(HYBRID_MENU_CREATION))
        {
            return;
        }
        setScalingModeAndOption(SPRITE_Y_UNIFORM_SCALE, SSO_SCALE_TEXTURE);
    }
    else
    {
        setScalingModeAndOption(SPRITE_XY_SCALE, SSO_SCALE_ALL);
        if(drawHybridCommon(HYBRID_MENU_NO_NAVIGATION))
        {
            return;
        }
        drawNonLinearMenu( NLM_PCCCreationMenu(), 20, 10, 20, 1000,20, 1.0f, PCC_costume_NLME.listId, PCC_costume_NLME.listId, 0 );
        bottomButtonY = 675.f;
        setScalingModeAndOption(SPRITE_Y_UNIFORM_SCALE, SSO_SCALE_TEXTURE);
    }

    xp = DEFAULT_SCRN_WD / 2.f;
    yp = 0.f;
    applyPointToScreenScalingFactorf(&xp, &yp); //use real pixel location

    selectedGeoSet = getSelectedGeoSet(COSTUME_EDIT_MODE_STANDARD);

    if(!gZoomed_in && selectedGeoSet && eaSize(&selectedGeoSet->bitnames))
    {
        setAnims(selectedGeoSet->bitnames);
    }
    else if(gZoomed_in && selectedGeoSet && eaSize(&selectedGeoSet->zoombitnames))
    {
        setAnims(selectedGeoSet->zoombitnames);
    }
    else if(game_state.editnpc)
    {
        setAnim( comboAnims.currChoice, &comboAnims.newlySelectedItem );
    }
    else
    {
        seqSetState( playerPtrForShell(1)->seq->state, TRUE, STATE_PROFILE ); // freeze the player model
        seqSetState( playerPtrForShell(1)->seq->state, TRUE, STATE_HIGH ); // freeze the player model
    }

    repositioningCamera = repositionCamera(repositioningCamera);

    drawAvatarControls(xp, 675.f*yposSc, &gZoomed_in, 0);
    zoomAvatar(false, gZoomed_in ? avatar_getZoomedInPosition(0,1) : avatar_getDefaultPosition(0,1));

    //--------------------------//
    costume_drawRegions(COSTUME_EDIT_MODE_STANDARD, &sStorePartsContained );     // Most of the work done here
    costume_drawColorsScales(COSTUME_EDIT_MODE_STANDARD );//
    //--------------------------//

    sButtonHideStore.iconName0 = hideStoreTextureName;
    sButtonHideStore.desc1 = hideStoreTooltip;
    if( D_MOUSEHIT == drawHybridButton(&sButtonHideStore, 55.f*xposSc, 640.f*yposSc, ZOOM_Z, 1.f, CLR_WHITE, 0 ))
    {
        costumeHideStorePiecesButtonPress();
    }
    //text
    setSpriteScaleMode(SPRITE_XY_NO_SCALE);
    font( &hybridbold_12 );
    font_color(CLR_WHITE, CLR_WHITE);
    prnt( 75.f*xposSc, 645.f*yposSc, ZOOM_Z, textXSc, textYSc, "hideStorePiecesButton" );

    setScalingModeAndOption(SPRITE_Y_UNIFORM_SCALE, SSO_SCALE_ALL);
    if ( drawHybridButton(&sButtonSave, 820.f, bottomButtonY, ZOOM_Z, 1.f, CLR_WHITE, HB_DRAW_BACKING ) == D_MOUSEHIT)
    {
        repositioningCamera = 1;
        saveCostume_start(MENU_COSTUME);
    }

    if ( drawHybridButton(&sButtonLoad, 890.f, bottomButtonY, ZOOM_Z, 1.f, CLR_WHITE, HB_DRAW_BACKING ) == D_MOUSEHIT)
    {
        repositioningCamera = 1;
        loadCostume_start(MENU_COSTUME);
    }

       drawBackCostume( 140.f, bottomButtonY);
     drawRandomCostume( 205.f, bottomButtonY);
    drawResetCostume(270.f, bottomButtonY);
    setSpriteScaleMode(SSO_SCALE_TEXTURE);
    
    convergeFaceScale();

     if( game_state.editnpc || gCostumeCritter )
    {
        CBox txtbox = { DEFAULT_SCRN_WD/2, DEFAULT_SCRN_HT-5, 0, 0 };
        char *saveCostume = "Save Costume";

        font( &game_12 );
        str_dims( &game_12, 1.f, 1.f, TRUE, &txtbox, saveCostume );

        if( mouseCollision(&txtbox) )
            font_color( CLR_RED, CLR_GREEN );
        else
            font_color( CLR_GREY, CLR_WHITE );

        cprnt( DEFAULT_SCRN_WD/2, DEFAULT_SCRN_HT - 5, 5000, 1.f, 1.f, saveCostume );

        if( mouseDownHit( &txtbox, MS_LEFT ) )
            saveVisualTextFile();
    }

    setScalingModeAndOption(SPRITE_XY_SCALE, SSO_SCALE_ALL);

    if( !gCostumeCritter )
    {
        scaleAvatar();
    }

    if (!getPCCEditingMode())
        {
        drawHybridNext( DIR_LEFT, 0, "BackString" );
        if (hasStoreCostumeParts())
        {
            setScalingModeAndOption(SPRITE_Y_UNIFORM_SCALE, SSO_SCALE_ALL);
            drawHybridStoreAddShoppingCartMenuButton(&sStorePartsContained, 0, sStorePartsContained.itemCount, 979.f, 741.f, 5000.f, 1.f, CLR_WHITE, 0);
            setScalingModeAndOption(SPRITE_XY_SCALE, SSO_SCALE_ALL);
        }
        else
        {
            drawHybridNext( DIR_RIGHT, 0, "NextString" );
        }
    }
    else
    {
        float screenScaleX = 1.f, screenScaleY = 1.f, screenScale;
        int ssd = is_scrn_scaling_disabled();

        calc_scrn_scaling(&screenScaleX, &screenScaleY);
        screenScale = MIN(screenScaleX, screenScaleY);
        set_scrn_scaling_disabled(1);

        if (drawNextButton( 40*screenScaleX, 740*screenScaleY, 5000.f, screenScale, 0, 0, 0, "BackString" ))
        {
            costumeMenuExitCode();
            start_menu( MENU_GENDER );
    }

    if (!gCostumeCritter &&
            (drawNextButton( 980*screenScaleX, 740*screenScaleY, 5000.f, screenScale, 1, 0, 0, "NextString" ) || inpEdgeNoTextEntry(INP_RETURN) ) )
    {
        costumeMenuExitCode();
            start_menu(MENU_PCC_PROFILE);
        }
        set_scrn_scaling_disabled(ssd);
    }

    if( game_state.editnpc || gCostumeCritter )
        combobox_display( &comboAnims );

     if( gCostumeCritter )
    {
        prnt( 512, 20, 5000, 1.f, 1.f, npcDefList.npcDefs[gNpcIndx]->name );

          if( drawStdButton( 790, 740, 5000, 50, 30, CLR_WHITE, "<<", 1.f, 0) == D_MOUSEDOWN )
        {
            cycleCritterCostumes(0,0);
        }
        if( drawStdButton( 850, 740, 5000, 50, 30, CLR_WHITE, "<|", 1.f, 0) == D_MOUSEHIT )
        {
            cycleCritterCostumes(0,0);
        }
        if( drawStdButton( 910, 740, 5000, 50, 30, CLR_WHITE, "|>", 1.f, 0) == D_MOUSEHIT )
        {
            cycleCritterCostumes(1,0);
        }
        if( drawStdButton( 970, 740, 5000, 50, 30, CLR_WHITE, ">>", 1.f, 0) == D_MOUSEDOWN )
        {
            cycleCritterCostumes(1,0);
        }
    }

    if (getPCCEditingMode() > 0)
        setPCCEditingMode(getPCCEditingMode()*-1);
    }


char *getCustomCostumeDir()
{
    static char path[MAX_PATH];
    if(!path[0])
    {
        if(isDevelopmentMode())
        {
            char *s;
            strcpy(path, fileDataDir());
            if(s = strrchr(path, '/'))
                *s = '\0';
        }
        else
        {
            getExecutableDir(path);
        }
        strcat(path, "/costumes");
    }
    return path;
}

const char* getCustomCostumeExt() { return ".v2costume"; }

int costume_isThereSkinEffect(const cCostume *costume)
{
    //    does the skin color effect any piece
    int skinEffect = 0;
    int i;
    for ( i = 0; i < eaSize(&costume->parts); ++i)
    {
        if (costume->parts[i]->pchTex1 && strstriConst(costume->parts[i]->pchTex1, "SKIN_"))
        {
            skinEffect = 1;
        }
    }
    return skinEffect;
}

static int doesTexHaveAnotherColor(int id, U8 color, int alpha)
{
    //    run through the entire pix data, and check if there is another color besides the specified color
    int i, j;
    int wd,ht;
    U8 *pix;
    pix = rdrGetTex(id, &wd, &ht, !alpha, alpha, 0);
    if (pix)
    {
        for (i = 0; i < ht; ++i)
        {
            for (j = 0; j < wd; ++j)
            {
                if (pix[(wd*i)+j] != color)
                {
                    return 1;
                }
            }
        }
        free(pix);
    }
    return 0;
}
int costume_setColorTintablePartsBitField(const cCostume *costume)
{
    int i;
    colorTintablePartsBitField = 0;
    if (!costume)    return 0;
    for (i = 0; i < eaSize(&costume->parts); ++i)
    {
        //    if the part is an fx, it is "most likely" tintable
        if (!isNullOrNone(costume->parts[i]->pchFxName) )
        {
            int bpId = bpGetIndexFromName(costume->parts[i]->pchName);
            if (bpId >= 0)
                colorTintablePartsBitField |= (1 << bpId);
            else
                assertmsgf(0, "bodyPart %s could not be match to an id", costume->parts[i]->pchName);
        }
        else if (isNullOrNone(costume->parts[i]->pchTex1))
        {
            //    for sure not color tintable
        }
        else if ( costume->parts[i]->pchTex1 && strstriConst(costume->parts[i]->pchTex1, "skin_") )
        {
            //    if the texture is a skin texture, it will only be tintable if there is a tex2, and the tex2 has some non-white in it
            if (!isNullOrNone(costume->parts[i]->pchTex2))
            {
                BasicTexture *btex = NULL;
                TexBind *tb = NULL;
                int bpId = bpGetIndexFromName(costume->parts[i]->pchName);
                if (bpId >= 0)
                {
                    if (costume->parts[i]->pchTex2[0] == '!')
                    {
                        btex = texFind(costume->parts[i]->pchTex2);
                        if (!btex)
                        {
                            tb = texFindComposite(costume->parts[i]->pchTex2);
                            if (tb)
                                btex = tb->tex_layers[TEXLAYER_BASE];
                        }
                    }
                    else
                    {
                        char texName2[MAX_NAME_LEN];
                        sprintf_s(texName2, MAX_NAME_LEN, "%s_%s", bodyPartList.bodyParts[bpId]->name_tex, costume->parts[i]->pchTex2);
                        btex = texFind(texName2);
                        if (!btex)
                        {
                            tb = texFindComposite(texName2);
                            if (tb)
                                btex = tb->tex_layers[TEXLAYER_BASE];
                        }

                    }
                    if (btex)
                    {
                        if (!btex->id)
                            texLoadBasic(btex->name, TEX_LOAD_NOW_CALLED_FROM_MAIN_THREAD, TEX_FOR_ENTITY);
                        colorTintablePartsBitField |= (doesTexHaveAnotherColor(btex->id, 0xff, 0) ? (1 << bpId) : 0);
                    }
                }
                else
                    assertmsgf(0, "bodyPart %s could not be match to an id", costume->parts[i]->pchName);
            }
        }
        else
        {
            //    if there is a tex1, it will be tintable iff there is a non-white in the tex1
            BasicTexture *btex = NULL;
            TexBind *tb = NULL;
            int bpId = bpGetIndexFromName(costume->parts[i]->pchName);
            if (bpId >= 0)
            {
                if (costume->parts[i]->pchTex1[0] == '!')
                {
                    btex = texFind(costume->parts[i]->pchTex1);
                    if (!btex)
                    {
                        tb = texFindComposite(costume->parts[i]->pchTex1);
                        if (tb)
                            btex = tb->tex_layers[TEXLAYER_BASE];
                    }
                }
                else
                {
                    char texName1[MAX_NAME_LEN];
                    sprintf_s(texName1, MAX_NAME_LEN, "%s_%s", bodyPartList.bodyParts[bpId]->name_tex, costume->parts[i]->pchTex1);
                    btex = texFind(texName1);
                    if (!btex)
                    {
                        tb = texFindComposite(texName1);
                        if (tb)
                            btex = tb->tex_layers[TEXLAYER_BASE];
                    }
                }
                if (btex)
                {
                    if (!btex->id)
                        texLoadBasic(btex->name, TEX_LOAD_NOW_CALLED_FROM_MAIN_THREAD, TEX_FOR_ENTITY);
                    colorTintablePartsBitField |= (doesTexHaveAnotherColor(btex->id, 0xff, 1) ? (1 << bpId) : 0);
                }
            }
            else
                assertmsgf(0, "bodyPart %s could not be match to an id", costume->parts[i]->pchName);
        }
    }
    return colorTintablePartsBitField;
}
void costume_NPCCostumeSave(const cCostume *originalCostume, const char *npcName, CustomNPCCostume **npcCostume)
{
    Entity *e = playerPtr();
    int i;

    if (originalCostume->appearance.colorSkin.integer != e->costume->appearance.colorSkin.integer)
    {
        if (!(*npcCostume))
        {
            *npcCostume = StructAllocRaw(sizeof(CustomNPCCostume));
        }
        (*npcCostume)->skinColor.integer = e->costume->appearance.colorSkin.integer;
    }

    //    loop through all the valid regions
    for (i = 0; i < eaSize(&costume_get_current_origin_set()->regionSet); ++i)
    {
        const CostumeRegionSet *rset = costume_get_current_origin_set()->regionSet[i];
        if (!regionsetIsRestricted(rset, NULL))
        {
            int j;
            //    loop through all the valid bonesets
            for (j = 0; j < rset->boneCount; ++j)
            {
                const CostumeBoneSet *bset = rset->boneSet[j];
                if (!bonesetIsRestricted(bset))
                {
                    int k;
                    //    loop through all the geometry
                    const char *lastGeoPartName = NULL;
                    for (k = 0; k < bset->geoCount; ++k)
                    {
                        const CostumeGeoSet *gset = bset->geo[k];
                        if (!geosetIsRestricted(gset) && ((!lastGeoPartName) || (lastGeoPartName && gset->bodyPartName && (stricmp(lastGeoPartName, gset->bodyPartName) != 0) )))
                        {
                            int m;
                            int playerBPID = costume_PartIndexFromName( playerPtr(), gset->bodyPartName );    //    player body part id
                            if (colorTintablePartsBitField & (1 <<playerBPID))
                            for (m = 0; m < eaSize(&originalCostume->parts); ++m)
                            {
                                //    if it is color tintable and the color doesnt match the npc color
                                if (stricmp(originalCostume->parts[m]->pchName, gset->bodyPartName) == 0)
                                {
                                    if (((e->costume->parts[playerBPID]->color[0].integer != originalCostume->parts[m]->color[0].integer) ||
                                        (e->costume->parts[playerBPID]->color[1].integer != originalCostume->parts[m]->color[1].integer) ) )
                                    {
                                        CustomNPCCostumePart *part = StructAllocRaw(sizeof(CustomNPCCostumePart));
                                        if (!(*npcCostume))
                                        {
                                            *npcCostume = StructAllocRaw(sizeof(CustomNPCCostume));
                                            (*npcCostume)->skinColor.integer = e->costume->appearance.colorSkin.integer;
                                        }
                                        part->regionName = gset->bodyPartName;
                                        part->color[0].integer = e->costume->parts[playerBPID]->color[0].integer;
                                        part->color[1].integer = e->costume->parts[playerBPID]->color[1].integer;
                                        eaPush(&((*npcCostume)->parts), part);
                                        lastGeoPartName = gset->bodyPartName;
                                    }
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    if (*npcCostume)
    {
        (*npcCostume)->originalNPCName = StructAllocString(npcName);
    }
}
int checkCostumeMatchesBody(BodyType costume, AvatarMode avatarBodyType)
{
    switch (costume)
    {
    case kBodyType_Male:
        if (avatarBodyType == AVATAR_GS_MALE)
        {
            return 1;
        }
        break;
    case kBodyType_Female:
        if (avatarBodyType == AVATAR_GS_FEMALE)
        {
            return 1;
        }
        break;
    case kBodyType_Huge:
        if (avatarBodyType == AVATAR_GS_HUGE)
        {
            return 1;
        }
        break;
    default:
        break;
    }

    return 0;
}
