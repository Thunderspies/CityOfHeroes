#include <utilitieslib/stdtypes.h>
#include "UI/Hybrid/uiBody.h"
#include "UI/uiGame.h"                // for start_menu

#include "UI/sprite/sprite_text.h"
#include "UI/sprite/sprite_base.h"
#include "UI/sprite/sprite_font.h"

#include "win/input.h"
#include <utilitieslib/components/earray.h>
#include "win/win_init.h" // for windowClientSize
#include "player/player.h"      // for playerPtr
#include "graphics/font.h"
#include "graphics/ttFontUtil.h"
#include "sound/sound.h"
#include "language/langClientUtil.h"
#include "graphics/textureatlas.h"
#include "cmdparse/cmdgame.h"  // for timestep

#include "formatter/smf_parse.h"
#include "formatter/smf_format.h"
#include "formatter/smf_main.h"

#include "UI/uiInput.h"
#include "UI/uiUtilMenu.h"
#include "UI/uiUtil.h"
#include "UI/uiUtilGame.h"
#include "UI/uiClipper.h"
#include "UI/Hybrid/uiHybridMenu.h"

#include "UI/uiGender.h"
#include "UI/uiAvatar.h"
#include "UI/uiCostume.h"
#include "UI/uiTailor.h"

#include "entity/costume.h"
#include "entity/costume_client.h"
#include "entity/entity.h"
#include "seq/seq.h"
#include "render/tex.h"
#include "UI/uiDialog.h"
#include "account/AccountData.h"
#include "entity/entPlayer.h"
#include "clientcomm/dbclient.h"
#include <utilitieslib/language/AppLocale.h>
#include "UI/uiComboBox.h"

#define HEIGHT_WD    910 // width of the police line up area
#define HEIGHT_X    57  // x coordinate
#define HEIGHT_FOOT_OFFSET        66  // width of a foot in pixels
#define HEIGHT_CM_OFFSET        2.17
#define HEIGHT_LINE_OFFSET        10  // offset from bottom of frame to start the lines


HybridElement s_presets[] = 
{
    { 0, "SlimString",        "slim",        "icon_body_slim_0",        "icon_body_slim_1" ,        "icon_body_slim_2" },
    { 1, "AverageString",    "average",    "icon_body_average_0",    "icon_body_average_1" ,        "icon_body_average_2" },
    { 2, "AthleticString",    "athletic", "icon_body_athletic_0", "icon_body_athletic_1" ,    "icon_body_athletic_2" },
    { 3, "HeavyString",        "heavy",    "icon_body_heavy_0",    "icon_body_heavy_1" ,        "icon_body_heavy_2" },
};

HybridElement * s_selected_preset = 0;

bool bUltraTailor = 0;  //have the ultra tailor token
GenderChangeMode genderEditingMode = 0;
static int lastGender;
int gFakeArachnosSoldier = 0;
int gFakeArachnosWidow = 0;
static Costume *prevCostume = NULL;

void resetBodyMenu()
{
    s_selected_preset = 0;
    gFakeArachnosSoldier = 0;
    gFakeArachnosWidow = 0;
    gCurrentBuild = BUILD_ATHLETIC;  //reset build
    SelectConfig(  buildMatchConfig(), -1 );
    setConfigs();
    sBodyConfig.scales.global = 0; //reset height
}

void bodyMenuExitCode(void)
{
    if (genderEditingMode)
    {
        Entity * e = playerPtr();
        if (lastGender != gCurrentGender)
        {
            costume_destroy(costume_current(e));
            e->pl->costume[e->pl->current_costume] = costume_clone(e->costume);
            genderEditingMode = GENDER_CHANGE_MODE_DIFFERENT_GENDER;
        }
        else
        {
            costume_destroy(costume_current(e));
            e->pl->costume[e->pl->current_costume] = costume_clone_nonconst(prevCostume);
            genderEditingMode = GENDER_CHANGE_MODE_BASIC;
        }
    }

    toggle_3d_game_modes(SHOW_NONE);
    setDefaultCostume(false);
}

static int drawGenderButton(AtlasTex * icon, AtlasTex * iconGlow, F32 cx, F32 cy, F32 z, F32 sc, int selected, const char * tooltip)
{
    AtlasTex * tabBack[3];
    AtlasTex * tabFrame = atlasLoadTexture("genderselecttab_frame_0");
    F32 sizeSc = (sc*.2f+1.f);
    int alphafront = 0xffffff00 | (int)(201 * sc);
    int alphaback = 0xffffff00 | (int)((100 + 104.f*sc));
    CBox box;
    int ret = 0;
    int tabColor[3];
    int i;
    F32 xposSc, yposSc;
    calculatePositionScales(&xposSc, &yposSc);

    tabBack[0] = atlasLoadTexture("genderselecttab_base_0");
    tabBack[1] = atlasLoadTexture("genderselecttab_base_1");
    tabBack[2] = atlasLoadTexture("genderselecttab_base_2");

    BuildScaledCBox(&box, cx-tabBack[0]->width*xposSc*sizeSc/2.f, cy-tabBack[0]->height*yposSc*sizeSc/2.f, tabBack[0]->width*xposSc*sizeSc, tabBack[0]->height*yposSc*sizeSc );
    if( mouseCollision(&box) )
    {
        if( isDown(MS_LEFT))
        {
            sizeSc *= 0.95f;
        }
        if (mouseClickHit(&box, MS_LEFT))
        {
            ret = 1;
        }
        alphaback = 0xffffffc9;
        setHybridTooltip(&box, tooltip);
    }

    if (selected)
    {
        tabColor[0] = HYBRID_SELECTED_COLOR[0];
        tabColor[1] = HYBRID_SELECTED_COLOR[1];
        tabColor[2] = HYBRID_SELECTED_COLOR[2];
    }
    else
    {
        tabColor[0] = HYBRID_FRAME_COLOR[0]&alphaback;
        tabColor[1] = HYBRID_FRAME_COLOR[1]&alphaback;
        tabColor[2] = HYBRID_FRAME_COLOR[2]&alphaback;
    }

    for (i = 0; i < 3; ++i)
    {
        display_sprite_positional(tabBack[i], cx, cy, z, sizeSc, sizeSc, tabColor[i], H_ALIGN_CENTER, V_ALIGN_CENTER );
    }
    display_sprite_positional(tabFrame, cx, cy, z, sizeSc, sizeSc, CLR_H_HIGHLIGHT, H_ALIGN_CENTER, V_ALIGN_CENTER );

    if (selected)
    {
        display_sprite_positional(iconGlow, cx, cy, z, sizeSc, sizeSc, CLR_H_GLOW, H_ALIGN_CENTER, V_ALIGN_CENTER );
    }
    display_sprite_positional(icon, cx, cy, z, sizeSc, sizeSc, CLR_WHITE, H_ALIGN_CENTER, V_ALIGN_CENTER );

    return ret;
}

static void drawGenders( F32 cx, F32 cy, F32 z )
{
    AtlasTex * male = atlasLoadTexture("icon_body_male_0");
     AtlasTex * female = atlasLoadTexture("icon_body_female_0");
    AtlasTex * huge = atlasLoadTexture("icon_body_huge_0");
    AtlasTex * maleGlow = atlasLoadTexture("icon_body_male_1");
    AtlasTex * femaleGlow = atlasLoadTexture("icon_body_female_1");
    AtlasTex * hugeGlow = atlasLoadTexture("icon_body_huge_1");
    static F32 male_sc = 0;
    static F32 female_sc = 0;
    static F32 huge_sc = 0;
    F32 base_alpha = 64, mouseover = 1.f;
    Entity * pe = playerPtrForShell(0);
    F32 xposSc, yposSc;
    calculatePositionScales(&xposSc, &yposSc);

     male_sc = stateScale(male_sc,gCurrentGender==AVATAR_GS_MALE?1:0);
    female_sc = stateScale(female_sc,gCurrentGender==AVATAR_GS_FEMALE?1:0);
    huge_sc = stateScale(huge_sc,gCurrentGender==AVATAR_GS_HUGE?1:0);

    // Male
    if(    drawGenderButton(male, maleGlow, cx-135.f*xposSc, cy, z, male_sc, gCurrentGender == AVATAR_GS_MALE, "male") )
    {
        clearCostumeStack();
        costume_init( entGetMutableCostume(pe) );
        gCurrentGender = AVATAR_GS_MALE;
        setGender( playerPtr(), "male" );
        entSetSeq(pe, seqLoadInst( "male_body", 0, SEQ_LOAD_FULL, pe->randSeed, pe ));
        s_selected_preset = 0;  //reset selection
        gCurrentBuild = BUILD_ATHLETIC;
        SelectConfig(  buildMatchConfig(), -1 );
        if (male_sc == 0.f)
        {
            sndPlay("N_GenderZoom", SOUND_GAME);
        }
    }

    // Female
    if(    drawGenderButton(female, femaleGlow, cx, cy, z, female_sc, gCurrentGender == AVATAR_GS_FEMALE, "female") )
    {
        clearCostumeStack();
        costume_init( entGetMutableCostume(pe) );
        gCurrentGender = AVATAR_GS_FEMALE;
        setGender( playerPtr(), "fem" );
        entSetSeq(pe, seqLoadInst( "fem_body", 0, SEQ_LOAD_FULL, pe->randSeed, pe ));
        s_selected_preset = 0;  //reset selection
        gCurrentBuild = BUILD_ATHLETIC;
        SelectConfig(  buildMatchConfig(), -1 );
        if (female_sc == 0.f)
        {
            sndPlay("N_GenderZoom", SOUND_GAME);
        }
    }

    // Huge
    if(    drawGenderButton(huge, hugeGlow, cx+135.f*xposSc, cy, z, huge_sc, gCurrentGender == AVATAR_GS_HUGE, "huge") )
    {
        // check loyalty/vip status
        Entity * e = playerPtr();        
        int tierLocked = e->pl != NULL && !AccountCanCreateHugeBody(ent_GetProductInventory( e ), db_info.loyaltyPointsEarned, db_info.account_inventory.accountStatusFlags );

        if (tierLocked)
        {
            dialogStd(DIALOG_OK, "UnlockToCreateHugeText", 0, 0, 0, 0, 0);
        }
        else
        {
            clearCostumeStack();
            costume_init( entGetMutableCostume(pe) );
            gCurrentGender = AVATAR_GS_HUGE;
            setGender( playerPtr(), "huge" );
            entSetSeq(pe, seqLoadInst( "huge_body", 0, SEQ_LOAD_FULL, pe->randSeed, pe ));
            s_selected_preset = 0;  //reset selection
            gCurrentBuild = BUILD_ATHLETIC;
            SelectConfig(  buildMatchConfig(), -1 );
            if (huge_sc == 0.f)
            {
                sndPlay("N_GenderZoom", SOUND_GAME);
            }
        }
    }
}

void drawPresetButton(HybridElement * he, F32 cx, F32 cy, F32 z)
{
    CBox box;
    F32 sc;
    int alpha;
    AtlasTex * buttonBack[3];
    AtlasTex * buttonFrame = atlasLoadTexture("bodycirculartab_frame_0");
    int i;
    int mouseCollide = 0;
    int selected;
    F32 xposSc, yposSc;
    calculatePositionScales(&xposSc, &yposSc);

    buttonBack[0] = atlasLoadTexture("bodycirculartab_base_0");
    buttonBack[1] = atlasLoadTexture("bodycirculartab_base_1");
    buttonBack[2] = atlasLoadTexture("bodycirculartab_base_2");

    if(!he->icon0)
        return;

    BuildScaledCBox(&box, cx - he->icon0->width*xposSc/2, cy - he->icon0->height*yposSc/2, he->icon0->width*xposSc, he->icon0->height*yposSc );
    if( mouseClickHit(&box, MS_LEFT ) )
    {
        s_selected_preset = he;
        gCurrentBuild = he->index;
        SelectConfig(buildMatchConfig(), -1);        
        sndPlay( "N_Select", SOUND_GAME );
    }
    mouseCollide = mouseCollision(&box);
    selected = s_selected_preset == he;
    if (mouseCollide)
    {
        setHybridTooltip(&box, he->text);
    }
    if( mouseCollide || selected )
    {
        if (he->percent == 0.f)
        {
            sndPlay( "N_MouseExit", SOUND_GAME );
        }
        he->percent = stateScale(he->percent,1);
    }
    else
    {
        if (he->percent == 1.f)
        {
            sndPlayNoInterrupt( "N_MouseExit", SOUND_GAME );
        }
        he->percent = stateScale(he->percent,0);
    }

    sc = 1.f + .1f*he->percent;
    if( mouseCollide && isDown(MS_LEFT) )
        sc *= .98f;

    alpha = 0xffffff00 | (int)(128 + 127.f*he->percent);
    
    for (i = 0; i < 3; ++i)
    {
        display_sprite_positional(buttonBack[i], cx, cy, z, sc, sc, (selected?HYBRID_SELECTED_COLOR[i]:HYBRID_FRAME_COLOR[i])&alpha, H_ALIGN_CENTER, V_ALIGN_CENTER );
    }
    display_sprite_positional(buttonFrame, cx, cy, z, sc, sc, CLR_H_HIGHLIGHT&alpha, H_ALIGN_CENTER, V_ALIGN_CENTER );

      display_sprite_positional(he->icon0, cx, cy, z, sc, sc, CLR_H_WHITE&alpha, H_ALIGN_CENTER, V_ALIGN_CENTER );

    if( selected && he->icon1)
    {
        display_sprite_positional(he->icon1, cx, cy, z, sc, sc, CLR_H_GLOW&alpha, H_ALIGN_CENTER, V_ALIGN_CENTER );
    }
}

static void drawPresetFrame(F32 x, F32 y, F32 z )
{
     F32 space = 105.f;
    AtlasTex * topBack = atlasLoadTexture("bodytypeframe_top_-1");
    AtlasTex * topFrame[3];
    AtlasTex * topBase = atlasLoadTexture("bodytypebase_top_0");
    AtlasTex * midFrame = atlasLoadTexture("bodytypeframe_mid_0");
    AtlasTex * midBase = atlasLoadTexture("bodytypebase_mid_0");

    int i;
    F32 xposSc, yposSc;
    calculatePositionScales(&xposSc, &yposSc);

    topFrame[0] = atlasLoadTexture("bodytypeframe_top_0");
    topFrame[1] = atlasLoadTexture("bodytypeframe_top_1");
    topFrame[2] = atlasLoadTexture("bodytypeframe_top_2");

    display_sprite_positional(topBack, x, y, z, 1.f, 1.f, CLR_H_DARK_BACK&0xffffff33, H_ALIGN_LEFT, V_ALIGN_TOP );
    display_sprite_positional(topBase, x, y, z, 1.f, 1.f, CLR_H_TAB_BACK&0xffffff7f, H_ALIGN_LEFT, V_ALIGN_TOP );
    for (i = 0; i < 3; ++i)
    {
        display_sprite_positional(topFrame[i], x, y, z, 1.f, 1.f, HYBRID_FRAME_COLOR[i], H_ALIGN_LEFT, V_ALIGN_TOP );
    }
    display_sprite_positional(midBase, x, y+topBase->height*yposSc, z, 1.f, 100.f/midBase->height, CLR_H_TAB_BACK&0xffffff7f, H_ALIGN_LEFT, V_ALIGN_TOP );
    display_sprite_positional(midFrame, x, y+topBase->height*yposSc, z, 1.f, 100.f/midFrame->height, CLR_H_DARK_BACK, H_ALIGN_LEFT, V_ALIGN_TOP );
    display_sprite_positional_blend(midBase, x, y+(topBase->height+100.f)*yposSc, z, 1.f, 100.f/midBase->height, CLR_H_TAB_BACK&0xffffff7f, CLR_H_TAB_BACK&0xffffff7f, CLR_H_TAB_BACK&0xffffff00, CLR_H_TAB_BACK&0xffffff00, H_ALIGN_LEFT, V_ALIGN_TOP );
    display_sprite_positional_blend(midFrame, x, y+(topBase->height+100.f)*yposSc, z, 1.f, 100.f/midFrame->height, CLR_H_DARK_BACK, CLR_H_DARK_BACK, CLR_H_DARK_BACK&0xffffff00, CLR_H_DARK_BACK&0xffffff00, H_ALIGN_LEFT, V_ALIGN_TOP );

     if( !s_presets[0].icon0 )
    {
        for( i = 0; i < 4; i ++ )
            hybridElementInit(&s_presets[i]);
    }

    for( i = 0; i < 4; i ++ )
    {
        drawPresetButton(&s_presets[i], x + (63.5f+i*space)*xposSc, y+25.f*yposSc, z);
    }
}

static void drawBodySlider(int index, const char * name, F32 x, F32 y, F32 z, F32 sc, F32 wd, F32 sliderWd, F32 *fCurrent, F32 *rangeMin, F32 *rangeMax, F32 * fSeqDest, F32 * fAppDest )
{
    F32 adjusted;

     adjusted = drawHybridSlider( x, y, z, sc, wd, sliderWd, *fCurrent, name, index, -1 );
    if( sc > .1f)
          *fCurrent = adjusted; // if scale gets to small, stop setting actual scale

    if(rangeMin && rangeMax)
    {
        if((*fCurrent) >= 0.0)
            adjusted = (*fCurrent) * ABS((*rangeMax));
        else
            adjusted = (*fCurrent) * ABS((*rangeMin));
    }
    else if(rangeMin)
        adjusted = adjustScale(*fCurrent, *rangeMin);

    if( fSeqDest )
        *fSeqDest = adjusted;
    if( fAppDest )
        *fAppDest = adjusted;
}

void adjustAvatarHeight()
{
    F32 fActual, fScale = sBodyConfig.scales.global;
    Entity *e = playerPtr();
    Entity * pe = playerPtrForShell(0);

    // pre-built offsets for each character type 
    if( gCurrentGender == AVATAR_GS_FEMALE ) // she is scaled up to male size, but is allowed to be shorter
    {
        if( fScale > 0 )
            fActual = 36*fScale;
        else
            fActual = 27*fScale;
    }
    else if( gCurrentGender == AVATAR_GS_MALE )
    {
        if ( fScale > 0 )
            fActual = 25*fScale;
        else
            fActual = 33*fScale;
    }
    else if( gCurrentGender == AVATAR_GS_HUGE ) // he is scaled to be initially taller, he can be taller overall, but not as short
    {
        if ( fScale > 0 )
            fActual = 25*fScale;
        else
            fActual = 33*fScale;
    }
    else
        fActual = fScale*25;

    //if( gCostumeCritter )
    //    fActual = 100*fScale;

    entGetMutableCostume(e)->appearance.fScale = fActual;
    scaleHero( e, fActual );
    scaleHero( pe, fActual );
}

void drawScaleSliders(F32 x, F32 y, F32 z, F32 sc, F32 wd, F32 sliderWd )
{
    Entity *e = playerPtr();
    Entity * pe = playerPtrForShell(0);
    F32 tmp = 2;
    Costume* mutable_costume = entGetMutableCostume(e);
    F32 space, xposSc, yposSc;
    calculatePositionScales(&xposSc, &yposSc);
    space = 25*sc*yposSc;

    if (current_menu() == MENU_BODY)
    {
        F32 min = -1.f, max = 1.f;
        drawBodySlider(1, "Height", x, y, z, sc, wd, sliderWd, &sBodyConfig.scales.global, &min, &max, 0, 0 );
        adjustAvatarHeight(sBodyConfig.scales.global);
        y += space;
    }

    drawBodySlider(2, "Physique", x, y, z, sc, wd, sliderWd, &sBodyConfig.scales.bone, &tmp, 0, 0, &mutable_costume->appearance.fBoneScale );
    changeBoneScale( e->seq, sBodyConfig.scales.bone );
    y += space;

     drawBodySlider(3, "Shoulders", x, y, z, sc, wd, sliderWd, &sBodyConfig.scales.shoulder, (F32*)&e->seq->type->shoulderScaleRange, 0, &e->seq->shoulderScale, &mutable_costume->appearance.fShoulderScale );
    y += space;

    if(game_state.editnpc)
    {
        F32 min = .9f, max = 5.f;
        drawBodySlider(4, "Arms", x, y, z, sc, wd, sliderWd, &sBodyConfig.scales.arm, &min, &max, &e->seq->armScale, &mutable_costume->appearance.fArmScale );
        y += space;
    }

     drawBodySlider(5, "Chest", x, y, z, sc, wd, sliderWd, &sBodyConfig.scales.chest, (F32*)&e->seq->type->chestScaleRange, 0, 0, &mutable_costume->appearance.fChestScale );
    y += space;

    drawBodySlider(6, "Waist", x, y, z, sc, wd, sliderWd, &sBodyConfig.scales.waist, (F32*)&e->seq->type->waistScaleRange, 0, 0, &mutable_costume->appearance.fWaistScale );
    y += space;

    drawBodySlider(7, "Hips", x, y, z, sc, wd, sliderWd, &sBodyConfig.scales.hip, (F32*)&e->seq->type->hipScaleRange, 0, 0, &mutable_costume->appearance.fHipScale );
    y += space;

    drawBodySlider(8, "Legs", x, y, z, sc, wd, sliderWd, &sBodyConfig.scales.leg, (F32*)&e->seq->type->legScaleMin, (F32*)&e->seq->type->legScaleMax,
        &e->seq->shoulderScale, &mutable_costume->appearance.fLegScale );
    y += space;

     changeBoneScaleEx(e->seq, &e->costume->appearance);
    changeBoneScaleEx(pe->seq, &e->costume->appearance);
}

void drawFaceSlider(F32 x, F32 y, F32 z, F32 sc, F32 wd, int part)
{

    Entity *e = playerPtr();
    char* titles[] = {"Head", "Brow", "Cheek", "Chin", "Cranium", "Jaw", "Nose"};
    int type = 20 + (part * 3);
    Costume* mutable_costume = entGetMutableCostume(e);
      drawHybridTripleSlider( x, y, z, sc, wd, wd - 130.f, mutable_costume->appearance.f3DScales[part], titles[part], type );
}

void genderChangeMenuStart()
{
    genderEditingMode = GENDER_CHANGE_MODE_BASIC;

    start_menu(MENU_BODY);
}

void ultraTailorInit()
{
    Entity * e = playerPtr();

    //select the correct gender
    switch (e->pl->costume[e->pl->current_costume]->appearance.bodytype)
    {
    case kBodyType_Male:
        gCurrentGender = AVATAR_GS_MALE;
        break;
    case kBodyType_Female:
        gCurrentGender = AVATAR_GS_FEMALE;
        break;
    case kBodyType_Huge:
        gCurrentGender = AVATAR_GS_HUGE;
        break;
    }

    if (prevCostume)
    {
        costume_destroy(prevCostume);
    }
    prevCostume = costume_current(e);
    e->pl->costume[e->pl->current_costume] = costume_clone_nonconst(prevCostume);

    lastGender = gCurrentGender;

    //initialize tailor (makes copies of costume/powerset)
    tailor_init(0);

    //run the entrance code since it hasn't been run yet
    bodyMenuEnterCode();

    genderEditingMode = GENDER_CHANGE_MODE_BASIC;

    tailorGenderInit(e);
}

static void drawHeightLines(float screenScaleX, float screenScaleY)
{
    AtlasTex * lineL    = atlasLoadTexture( "bodytype_heightline_end_L.tga" );
    AtlasTex * line    = atlasLoadTexture( "bodytype_heightline_mid.tga" );
    AtlasTex * lineR    = atlasLoadTexture( "bodytype_heightline_end_R.tga" );
    AtlasTex * floorpattern_0 = atlasLoadTexture("floorpattern_0");
    float wd = (HEIGHT_WD - HEIGHT_LINE_OFFSET*2 - 600) * screenScaleX;
    float x = (600 + HEIGHT_X + HEIGHT_LINE_OFFSET) * screenScaleX;
    float w = lineL->width;
    int i;

    font( &game_9 );
    font_color( CLR_WHITE, CLR_WHITE );

    // draw 8ft of lines
    if( !getCurrentLocale() )
    {
        for( i = 0; i < 9; i++ )
        {

            float y = (620 - (i*HEIGHT_FOOT_OFFSET))* screenScaleY;

            display_sprite( lineL, x,            y, 21, 1.f, 1.f, CLR_WHITE );
            display_sprite( line,  x + w,        y, 21, (wd - w*2)/line->width, 1.f, CLR_WHITE );
            display_sprite( lineR, x + wd - w,    y, 21, 1.f, 1.f, CLR_WHITE );

            cprntEx( x,                y + 2, 22, 1.f, 1.f, CENTER_Y, "%i'", i );
            cprntEx( x + wd - 10,    y + 2, 22, 1.f, 1.f, CENTER_Y, "%i'", i );
        }
    }
    else
    {
        for( i = 0; i <= 240; i+=20 )
        {
            float y = (620 - (i*HEIGHT_CM_OFFSET))*screenScaleY;

            display_sprite( lineL, x,            y, 21, 1.f, 1.f, CLR_WHITE );
            display_sprite( line,  x + w,        y, 21, (wd - w*2)/line->width, 1.f, CLR_WHITE );
            display_sprite( lineR, x + wd - w,    y, 21, 1.f, 1.f, CLR_WHITE );

            cprntEx( x - 20,                y + 2, 22, 1.f, 1.f, CENTER_Y, "%i cm", i );
            cprntEx( x + wd - 10,    y + 2, 22, 1.f, 1.f, CENTER_Y, "%i cm", i );
        }    
    }

    //draw floor pattern
    display_sprite(floorpattern_0, 815.f-floorpattern_0->width/2.f, 640.f-floorpattern_0->height/2.f, 5.f, 1.f, 1.f, CLR_H_BACK&0xffff7f);
}

void bodyMenu()
{
    int tailorBody = bodyCanBeEdited(0);
    F32 xposSc, yposSc, xp, yp;
    if( tailorBody && !gTailoredCostume )
    {
        ultraTailorInit();
    }

    if(drawHybridCommon(tailorBody ? HYBRID_MENU_TAILOR : HYBRID_MENU_CREATION))
        return;

    setScalingModeAndOption(SPRITE_Y_UNIFORM_SCALE, SSO_SCALE_TEXTURE);
    calculatePositionScales(&xposSc, &yposSc);
    xp = DEFAULT_SCRN_WD;
    yp = 0.f;
    applyPointToScreenScalingFactorf(&xp, &yp);
    setAnim( comboAnims.currChoice, &comboAnims.newlySelectedItem );  //keeps the same animation

     drawGenders( 240.f*xposSc, 250.f*yposSc, 5.f );
     drawPresetFrame( 15.f*xposSc, 420.f*yposSc, 5.f );
    convergeConfigs(); // migrate slider positions toward preset
       drawScaleSliders(25.f*xposSc, 490*yposSc, 5.f, 1.f, 170.f + 260.f, 260.f);

    setScreenScaleOption(SSO_SCALE_ALL);
    drawAvatarControls(DEFAULT_SCRN_WD-199.f, 660.f, 0, 1);
    setScalingModeAndOption(SPRITE_XY_SCALE, SSO_SCALE_ALL);
    moveAvatar( gCurrentGender );
    drawHeightLines(1.f, 1.f);


    if( game_state.editnpc )
    {
        drawSmallCheckboxEx( 0, 200.f, 735.f, 12.f, 1.f,  "Fake Arachnos Soldier", &gFakeArachnosSoldier, 0, 1, 0, 0, 0, 0 );
        drawSmallCheckboxEx( 0, 200.f, 750.f, 12.f, 1.f,  "Fake Arachnos Widow", &gFakeArachnosWidow, 0, 1, 0, 0, 0, 0 );
    }

    if (drawHybridNext( DIR_LEFT, 0, "BackString" ) && tailorBody)
    {
        if(prevCostume && genderEditingMode)
        {
            Entity * e = playerPtr();
            costume_destroy(costume_current(e));
            e->pl->costume[e->pl->current_costume] = prevCostume;
            prevCostume = NULL;
        }

        tailor_exit(0);
    }
    drawHybridNext( DIR_RIGHT, 0, "NextString" );
}

void bodyMenuEnterCode(void)
{
    Entity *pe = playerPtrForShell(1);

    moveAvatar( gCurrentGender );

    //this will remove costumes from the avatar if we are going back
    costume_init( entGetMutableCostume(pe) );
    //remove any lingering fx pieces... weapons/shields
    doDelAllCostumeFx(pe, 0);

    switch (gCurrentGender)
    {
    case AVATAR_GS_MALE:
        entSetSeq(pe, seqLoadInst( "male_body", 0, SEQ_LOAD_FULL, pe->randSeed, pe ));
        break;
    case AVATAR_GS_FEMALE:
        entSetSeq(pe, seqLoadInst( "fem_body", 0, SEQ_LOAD_FULL, pe->randSeed, pe ));
        break;
    case AVATAR_GS_HUGE:
        entSetSeq(pe, seqLoadInst( "huge_body", 0, SEQ_LOAD_FULL, pe->randSeed, pe ));
        break;
    default:
        break;
    }
}

int bodyCanBeEdited(int redirect)
{
    return bUltraTailor || genderEditingMode;
}
