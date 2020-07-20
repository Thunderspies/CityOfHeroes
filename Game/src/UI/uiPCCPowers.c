#include "uiPCCPowers.h"
#include "UI/uiGame.h"
#include "UI/uiUtil.h"
#include "UI/uiInput.h"
#include "UI/uiUtilMenu.h"
#include "UI/uiScrollBar.h"
#include "UI/uiClipper.h"
#include "UI/uiUtilGame.h"                //    for drawFrame
#include "UI/uiPowerInfo.h"
#include "uiSlider.h"
#include "uiHelpButton.h"

#include "formatter/smf_main.h"                //    text editing
#include "UI/sprite/sprite_base.h"            //    display things
#include "UI/sprite/sprite_text.h"
#include "UI/sprite/sprite_font.h"
#include "graphics/textureatlas.h"

#include "win/win_init.h"
#include "cmdparse/cmdgame.h"
#include "entity/character_base.h"            // character base
#include "sound/sound.h"                    //    sounds
#include "entity/powers.h"                    //    basepowersets
#include <utilitieslib/components/Earray.h>                    //    earraygetsize
#include "entity/entity.h"                    //    entity def
#include "entity/EntPlayer.h"                //    for PL
#include "entity/costume.h"                //    for costumeunawardparts
#include "graphics/ttFontUtil.h"                //    for CENTER_Y
#include <utilitieslib/language/MessageStoreUtil.h>        //    for textStd
#include "player/player.h"                    //    for playerPtr
#include "entity/entclient.h"                //    for entcreate
#include <utilitieslib/components/StashTable.h>                //    for stashTable
#include "gameComm/villainDef.h"                //    for villain rank enum
#include <utilitieslib/components/estring.h>
#include "entity/PCC_Critter.h"
#include "entity/PCC_Critter_Client.h"
#include "uiPCCCreationNLM.h"

#define PCC_POWER_MENU_Y 140
#define PCC_POWER_MENU_HEIGHT 560
#define PCC_POWER_LC_X 60
#define PCC_POWER_LC_W 245
#define PCC_POWER_MC_W 275
#define PCC_POWER_COLUMN_SPACING 15
#define PCCSET_MIN_SPACING 30
#define PCCSET_SPACING 45
#define PCCSET_X (PCC_POWER_LC_X+40)
#define PCCSET_WIDTH 170
#define PCCSET_MAJOR_WIDTH (960 - (PCC_POWER_LC_X+PCC_POWER_LC_W+PCC_POWER_COLUMN_SPACING))
#define PCCSET_DIFFICULTYWIDTH ( PCCSET_MAJOR_WIDTH / PCC_DIFF_COUNT )
static const BasePower * s_pMouseoverPower;
static int s_mouseOverPowerSet = -1;
static int s_mouseOverPowerIdx = -1;
int pcc_powerSelection = pcc_MenuPrimary;
static int nextMenu = MENU_GENDER;
static int *PCCSortedPowers[2] = { NULL, NULL };
int PCCselectedPowerSets[pcc_MenuCount] = {-1, -1, -1};
int currentlyBoughtCritterPowers[pcc_MenuCount] = {-1,-1,-1};
int pccCritterDifficulty[2]  = { 0, 0 };

static int *primaryPowerSetMatches = NULL;
static int *secondaryPowerSetMatches = NULL;
static int resetOpenPowersets = 1;
static int includeMiscPowers = 0;
int PCCselectedPowers[2] = { 0, 0 };
extern int pccCritterRank;
extern void toggle_3d_game_modes(int);

static TextAttribs s_TextAttr_PCC_powers =
{
    /* piBold        */  (int *)0,
    /* piItalic      */  (int *)0,
    /* piColor       */  (int *)0xddddccff,
    /* piColor2      */  (int *)0,
    /* piColorHover  */  (int *)0xddddccff,
    /* piColorSelect */  (int *)0,
    /* piColorSelectBG*/ (int *)0x333333ff,
    /* piScale       */  (int *)(int)(1.0f*SMF_FONT_SCALE),
    /* piFace        */  (int *)&game_12,
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

enum{
    PCC_BLASTER = 1<<1,
    PCC_CLASS_START = PCC_BLASTER,
    PCC_TANKER = 1<<2,
    PCC_SCRAPPER = 1 << 3,
    PCC_DEFENDER = 1 << 4,
    PCC_CONTROLLER = 1 << 5,

    PCC_BRUTE = 1 << 6,
    PCC_STALKER = 1 << 7,
    PCC_CORRUPTOR = 1 << 8,
    PCC_DOMINATOR = 1 << 9,
    PCC_MASTERMIND = 1 << 10,
    PCC_OTHER = 1 << 11,
    PCC_ALL = 1 << 12,
};

static int PCCPower_powerIsAvailable(const BasePower *power, int category, int conning, int difficulty, int selectedPowers, int powerIdx)
{

      if( difficulty == PCC_DIFF_CUSTOM )
        return (1<<powerIdx&selectedPowers); // this seems backwards to me


    if( conning <= basepower_GetAIMaxRankCon(power) &&
        conning >= basepower_GetAIMinRankCon(power)  &&
        difficulty <= basepower_GetMaxDifficulty(power) &&
        difficulty >= basepower_GetMinDifficulty(power) )    
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
static void drawPCC_DifficultyPreference( int leftX, int y, float screenScaleX, float screenScaleY)
{
    int mouse, i;
    float screenScale = MIN(screenScaleX, screenScaleY);
    font( &gamebold_12 );
    font_color( CLR_WHITE, SELECT_FROM_UISKIN( 0x88b6ffff, 0xaaaaaaff, 0x8888ccff ) );
    cprnt( (leftX+((PCCSET_MAJOR_WIDTH-60)/2.0f))*screenScaleX, (y + 7.5-PIX3)*screenScaleY, 60, screenScale, screenScale, "PCCDifficultyPreference" );

     for (i = -1; i < (PCC_DIFF_COUNT-1); ++i)
    {
        mouse = drawCheckBar((leftX + ((i+1)*PCCSET_DIFFICULTYWIDTH))*screenScaleX, (y*screenScaleY) + (37*screenScale), 60, screenScale, 120*screenScaleX, getPCCDifficultySelectionTitleText(i), 
            (pccCritterDifficulty[pcc_powerSelection]==i), 1, 0, 0, NULL);
        if (mouse == D_MOUSEHIT)
        {
            if (i != pccCritterDifficulty[pcc_powerSelection])
            {
                sndPlay("Select", SOUND_GAME);
                pccCritterDifficulty[pcc_powerSelection] = i;
            }
        }
    }
}

static void drawPCCFrame(float leftX, float leftY, float width, float height, float z, int color, int back_color, float screenScaleX, float screenScaleY)
{
    int size = 4;
    float w;
    // load pipe pieces
    AtlasTex *ul        = atlasLoadTexture( "frame_blue_4px_12r_UL.tga" );
     AtlasTex *ll        = atlasLoadTexture( "frame_blue_4px_12r_LL.tga" );
    AtlasTex *lr        = atlasLoadTexture( "frame_blue_4px_12r_LR.tga" );
    AtlasTex *ur        = atlasLoadTexture( "frame_blue_4px_12r_UR.tga" );
    AtlasTex *ypipe    = atlasLoadTexture( "frame_blue_4px_vert.tga" );
    AtlasTex *xpipe    = atlasLoadTexture( "frame_blue_4px_horiz.tga" );

    // load backgrounds
    AtlasTex *bul    = atlasLoadTexture( "frame_blue_background_UL.tga" );
    AtlasTex *bll    = atlasLoadTexture( "frame_blue_background_LL.tga" );
    AtlasTex *blr    = atlasLoadTexture( "frame_blue_background_LR.tga" );
    AtlasTex *bur    = atlasLoadTexture( "frame_blue_background_UR.tga" );
    AtlasTex * back  = white_tex_atlas;

    // load elbows
    AtlasTex *eul    = atlasLoadTexture( "frame_blue_background_elbow_UL.tga" );
    AtlasTex *ell    = atlasLoadTexture( "frame_blue_background_elbow_LL.tga" );
    AtlasTex *elr    = atlasLoadTexture( "frame_blue_background_elbow_LR.tga" );
    AtlasTex *eur    = atlasLoadTexture( "frame_blue_background_elbow_UR.tga" );

    leftX *= screenScaleX;
    leftY *= screenScaleY;
    width *= screenScaleX;
    height *= screenScaleY;
    {
        w = ul->width;
        // clamp the left width
        if (width < (w*2))
        {
            width = w*2;
        }
        if (height < (w*2))
        {
            height = w*2;
        }
        // top left
        display_sprite( ul,  leftX, leftY, z+1, 1.0f, 1.0f, color      );
        display_sprite( bul, leftX , leftY , z, 1.0f, 1.0f, back_color );

        // top
        display_sprite( xpipe, leftX + w, leftY, z+1, (width - 2*w)/xpipe->width, 1.f, color );
        display_sprite( back , leftX + w, leftY + size, z, (width - 2*w)/back->width, (w - size)/back->height, back_color );

        // top right
        display_sprite( ur,  leftX + width - w, leftY, z+1, 1.0f, 1.0f, color      );
        display_sprite( bur, leftX + width - w, leftY ,  z, 1.0f, 1.0f, back_color );

        // lower right
        display_sprite( lr,  leftX + width - w, leftY + height - w, z+1, 1.0f, 1.0f, color      );
        display_sprite( blr, leftX + width - w, leftY + height - w,   z, 1.0f, 1.0f, back_color );

        // bottom
        display_sprite( xpipe, leftX + w, leftY + height - size, z+1, (width - 2*w)/xpipe->width, 1.f, color );
        display_sprite( back,  leftX + w, leftY + height - w, z, (width - 2*w)/back->width, (w - size)/back->height, back_color );

        // lower left
        display_sprite( ll,  leftX, leftY + height - w, z+1, 1.0f, 1.0f, color );
        display_sprite( bll, leftX, leftY + height - w,   z, 1.0f, 1.0f, back_color );

        // left
        display_sprite( ypipe, leftX, leftY + w, z+1, 1.0f, (height - 2*w)/ypipe->height, color );
        display_sprite( back,  leftX + size, leftY + w, z, (w - size)/back->width, (height - 2*w)/back->height, back_color );

        // right
        display_sprite( ypipe, leftX + width - size, leftY + w, z+1, 1.0f, (height - 2*w)/ypipe->height, color );
         display_sprite( back,  leftX + width - w, leftY + w, z, (w - size)/back->width, (height - 2*w)/back->height, back_color );    

         display_sprite( back,  leftX +  w, leftY + w, z, (width - 2*w)/back->width, (height - 2*w)/back->height, back_color );    
    }
    
}
static int displayPowerSetDetails(float x, float y, float z, float width, float height, int category, float screenScaleX, float screenScaleY)
{
    static SMFBlock sm;
    int textBlockHeight=0;
    float screenScale = MIN(screenScaleX, screenScaleY);
    font(&title_12);
    font_color(uiColors.standard.text, uiColors.standard.text2);
    s_TextAttr_PCC_powers.piScale = (int *)(int)(1.0f*SMF_FONT_SCALE*screenScale);
    if (s_mouseOverPowerSet >= 0)
    {    
        const BasePowerSet *pSet;        
        pSet = missionMakerPowerSet[category]->ppPowerSets[s_mouseOverPowerSet];
        if (pSet)
        {
            cprntEx(x*screenScaleX,(y+13)*screenScaleY,z, screenScale,screenScale, CENTER_Y, pSet->pchDisplayName);
            textBlockHeight = smf_ParseAndDisplay(&sm, textStd(pSet->pchDisplayHelp), (x+15)*screenScaleX, (y*screenScaleY)+(18*screenScale), z, (width - 20)*screenScaleX, (height-15)*screenScaleY, 0,0, &s_TextAttr_PCC_powers, NULL, 0, false);
        }
        else
        {
            //    Error Message
        }
    }
    else
    {
        int class_index = -s_mouseOverPowerSet;
        switch (class_index)
        {
        case PCC_BLASTER:
            cprntEx(x*screenScaleX,(y+13)*screenScaleY,z, screenScale,screenScale, CENTER_Y, textStd("Class_Blaster"));
            textBlockHeight = smf_ParseAndDisplay(&sm, textStd("PCC_Powers_Blaster_Desc"), (x+15)*screenScaleX, (y*screenScaleY)+(18*screenScale), z, (width - 20)*screenScaleX, (height-15)*screenScaleY, 0,0, &s_TextAttr_PCC_powers, NULL, 0, false);
            break;
        case PCC_TANKER:
            cprntEx(x*screenScaleX,(y+13)*screenScaleY,z, screenScale,screenScale, CENTER_Y, textStd("Class_Tanker"));
            textBlockHeight = smf_ParseAndDisplay(&sm, textStd("PCC_Powers_Tanker_Desc"), (x+15)*screenScaleX, (y*screenScaleY)+(18*screenScale), z, (width - 20)*screenScaleX, (height-15)*screenScaleY, 0,0, &s_TextAttr_PCC_powers, NULL, 0, false);
            break;
        case PCC_SCRAPPER:
            cprntEx(x*screenScaleX,(y+13)*screenScaleY,z, screenScale,screenScale, CENTER_Y, textStd("Class_Scrapper"));
            textBlockHeight = smf_ParseAndDisplay(&sm, textStd("PCC_Powers_Scrapper_Desc"), (x+15)*screenScaleX, (y*screenScaleY)+(18*screenScale), z, (width - 20)*screenScaleX, (height-15)*screenScaleY, 0,0, &s_TextAttr_PCC_powers, NULL, 0, false);
            break;
        case PCC_DEFENDER:
            cprntEx(x*screenScaleX,(y+13)*screenScaleY,z, screenScale,screenScale, CENTER_Y, textStd("Class_Defender"));
            textBlockHeight = smf_ParseAndDisplay(&sm, textStd("PCC_Powers_Defender_Desc"), (x+15)*screenScaleX, (y*screenScaleY)+(18*screenScale), z, (width - 20)*screenScaleX, (height-15)*screenScaleY, 0,0, &s_TextAttr_PCC_powers, NULL, 0, false);
            break;
        case PCC_CONTROLLER:
            cprntEx(x*screenScaleX,(y+13)*screenScaleY,z, screenScale,screenScale, CENTER_Y, textStd("Class_Controller"));
            textBlockHeight = smf_ParseAndDisplay(&sm, textStd("PCC_Powers_Controller_Desc"), (x+15)*screenScaleX, (y*screenScaleY)+(18*screenScale), z, (width - 20)*screenScaleX, (height-15)*screenScaleY, 0,0, &s_TextAttr_PCC_powers, NULL, 0, false);
            break;
        case PCC_BRUTE:
            cprntEx(x*screenScaleX,(y+13)*screenScaleY,z, screenScale,screenScale, CENTER_Y, textStd("Class_Brute"));
            textBlockHeight = smf_ParseAndDisplay(&sm, textStd("PCC_Powers_Brute_Desc"), (x+15)*screenScaleX, (y*screenScaleY)+(18*screenScale), z, (width - 20)*screenScaleX, (height-15)*screenScaleY, 0,0, &s_TextAttr_PCC_powers, NULL, 0, false);
            break;
        case PCC_CORRUPTOR:
            cprntEx(x*screenScaleX,(y+13)*screenScaleY,z, screenScale,screenScale, CENTER_Y, textStd("Class_Corruptor"));
            textBlockHeight = smf_ParseAndDisplay(&sm, textStd("PCC_Powers_Corruptor_Desc"), (x+15)*screenScaleX, (y*screenScaleY)+(18*screenScale), z, (width - 20)*screenScaleX, (height-15)*screenScaleY, 0,0, &s_TextAttr_PCC_powers, NULL, 0, false);
            break;
        case PCC_DOMINATOR:
            cprntEx(x,y+13,z, 1.0f,1.0f, CENTER_Y, textStd("Class_Dominator"));
            textBlockHeight = smf_ParseAndDisplay(&sm, textStd("PCC_Powers_Dominator_Desc"), (x+15)*screenScaleX, (y*screenScaleY)+(18*screenScale), z, (width - 20)*screenScaleX, (height-15)*screenScaleY, 0,0, &s_TextAttr_PCC_powers, NULL, 0, false);
            break;
        case PCC_MASTERMIND:
            cprntEx(x*screenScaleX,(y+13)*screenScaleY,z, screenScale,screenScale, CENTER_Y, textStd("Class_MasterMind"));
            textBlockHeight = smf_ParseAndDisplay(&sm, textStd("PCC_Powers_MasterMind_Desc"), (x+15)*screenScaleX, (y*screenScaleY)+(18*screenScale), z, (width - 20)*screenScaleX, (height-15)*screenScaleY, 0,0, &s_TextAttr_PCC_powers, NULL, 0, false);
            break;
        case PCC_OTHER:
            cprntEx(x*screenScaleX,(y+13)*screenScaleY,z, screenScale,screenScale, CENTER_Y, textStd("OptionsMisc"));
            textBlockHeight = smf_ParseAndDisplay(&sm, textStd("PCC_Powers_Misc_Desc"), (x+15)*screenScaleX, (y*screenScaleY)+(18*screenScale), z, (width - 20)*screenScaleX, (height-15)*screenScaleY, 0,0, &s_TextAttr_PCC_powers, NULL, 0, false);
            break;
        case PCC_ALL:
            cprntEx(x*screenScaleX,(y+13)*screenScaleY,z, screenScale,screenScale, CENTER_Y, textStd("AllString"));
            textBlockHeight = smf_ParseAndDisplay(&sm, textStd("PCC_Powers_All_Desc"), (x+15)*screenScaleX, (y*screenScaleY)+(18*screenScale), z, (width - 20)*screenScaleX, (height-15)*screenScaleY, 0,0, &s_TextAttr_PCC_powers, NULL, 0, false);
            break;
        }
    }
    return textBlockHeight + 18*screenScale;

}
static int displayMiddlePanel(float x, float y, float z, float width, float height, int category, float screenScaleX, float screenScaleY)
{
    float startY;
    int textBlockHeight;
    F32 scale = 1.f;
    float returnY;
    UIBox box;
    float screenScale = MIN(screenScaleX, screenScaleY);
    startY = returnY = y;

    font( &gamebold_12 );
    font_color( CLR_WHITE, SELECT_FROM_UISKIN( 0x88b6ffff, 0xaaaaaaff, 0x8888ccff ) );
    cprnt( (x+width/2)*screenScaleX, (PCC_POWER_MENU_Y + (2*PCCSET_SPACING) + 7.5-PIX3)*screenScaleY, 60, screenScale, screenScale, "PCCAvailablePowersString" );
    uiBoxDefine(&box, (x+5)*screenScaleX,( PCC_POWER_MENU_Y+PIX3 + ( 2*PCCSET_SPACING))*screenScaleY, (width-9)*screenScaleX, height*screenScaleY);
    clipperPush( &box );

    //    show the title text
    textBlockHeight = displayPowerSetDetails(x+10, y, z, width-(10+(2*PIX3)), 200, category, screenScaleX, screenScaleY);
    if (s_mouseOverPowerSet >= 0)
    {
        //    white dividing line
        AtlasTex * white = atlasLoadTexture("white");
        display_sprite(white, x*screenScaleX+5*scale*screenScale, y*screenScaleY+textBlockHeight+4*scale*screenScale, z, (width-10*scale)*screenScale/white->width, 1.f/white->height, CLR_WHITE );
    }    

    if (s_mouseOverPowerSet >= 0)
    {
        int i, mouse;
        CBox mouseBox;
        static SMFBlock powersText;
        int villainConning;
        returnY = y+(textBlockHeight/screenScale)+PCCSET_MIN_SPACING;
        switch (pccCritterRank)
        {
        case PCC_RANK_MINION:
            villainConning = villainRankConningAdjustment(VR_MINION);
            break;
        case PCC_RANK_LT:
            villainConning = villainRankConningAdjustment(VR_LIEUTENANT);
            break;
        case PCC_RANK_BOSS:
            villainConning = villainRankConningAdjustment(VR_BOSS);
            break;
        case PCC_RANK_ELITE_BOSS:
            villainConning = villainRankConningAdjustment(VR_BOSS);
            break;
        case PCC_RANK_ARCH_VILLAIN:
            villainConning = villainRankConningAdjustment(VR_ARCHVILLAIN);
            break;
        default:
            villainConning = villainRankConningAdjustment(VR_NONE);
            break;
        }
         for (i = 0; i < eaSize(&missionMakerPowerSet[category]->ppPowerSets[s_mouseOverPowerSet]->ppPowers); i++ )
        {
            const BasePower *currentPower;
            currentPower = missionMakerPowerSet[category]->ppPowerSets[s_mouseOverPowerSet]->ppPowers[i];
            BuildCBox(&mouseBox, x*screenScaleX, (returnY - 20)*screenScaleY, PCC_POWER_MC_W*screenScaleX, 28*screenScaleY );
            mouse = mouseCollision( &mouseBox);
            if (mouse)
            {
                drawBox(&mouseBox, z, 0, 0x55555533);
            }
            if ( PCCPower_powerIsAvailable(currentPower, category, villainConning, pccCritterDifficulty[category], 
                 s_mouseOverPowerSet == PCCselectedPowerSets[category] ? PCCselectedPowers[category] : 0, i)    )
            {
                AtlasTex *dot = atlasLoadTexture( "EnhncTray_powertitle_dot.tga" );
                int textHt;
                if (pccCritterDifficulty[category] == PCC_DIFF_CUSTOM)
                {
                    AtlasTex *cmark = atlasLoadTexture( "costume_color_ring_main.tga" );
                    display_sprite( cmark, (x+24)*screenScaleX, returnY*screenScaleY - (dot->height*1.1f*screenScale), 55.0f, dot->width*1.2f*screenScale / cmark->width, dot->height *screenScale*1.2f/cmark->height, 
                        SELECT_FROM_UISKIN( CLR_FRAME_BLUE, CLR_FRAME_RED, CLR_FRAME_ROGUE ) );
                }
                
                display_sprite( dot, (x+25)*screenScaleX, returnY*screenScaleY - dot->height*screenScale, 50.0f, screenScale, screenScale, CLR_WHITE );
                s_TextAttr_PCC_powers.piScale = (int *)(int)(1.0f*SMF_FONT_SCALE*screenScale);
                textHt = smf_ParseAndDisplay(&powersText, textStd(currentPower->pchDisplayName),
                    (x+25 + 5)*screenScaleX+dot->width*screenScale, (returnY-5)*screenScaleY-(dot->height*screenScale), 50.0f, (PCC_POWER_MC_W*screenScaleX)-((25+5)*screenScaleX+dot->width*screenScale), 50, 0,0,&s_TextAttr_PCC_powers,0,0, 0);

                if (mouse)
                {
                    AtlasTex *ring_highlight = atlasLoadTexture("costume_button_link_glow.tga.tga");
                    display_sprite(ring_highlight, (x+18)*screenScaleX, (returnY-18)*screenScaleY, 49.0f, 25.0f*screenScale/ring_highlight->width, 25.0f*screenScale/ring_highlight->height, CLR_WHITE );
                    s_pMouseoverPower = currentPower;
                    s_mouseOverPowerIdx = i;
                    if (mouseClickHit(&mouseBox, MS_LEFT) )
                    {
                        if ( (pccCritterDifficulty[category] != PCC_DIFF_CUSTOM) || 
                            (character_IsAllowedToHavePowerSetHypothetically(playerPtr()->pchar, missionMakerPowerSet[category]->ppPowerSets[s_mouseOverPowerSet]) &&
                            (s_mouseOverPowerSet != PCCselectedPowerSets[category]) ) )
                        {
                            int j;
                            PCCselectedPowers[category] = 0;
                            PCCselectedPowerSets[category] = s_mouseOverPowerSet;
                            for (j=0; j < eaSize(&missionMakerPowerSet[category]->ppPowerSets[s_mouseOverPowerSet]->ppPowers); ++j)
                            {
                                if (PCCPower_powerIsAvailable(missionMakerPowerSet[category]->ppPowerSets[s_mouseOverPowerSet]->ppPowers[j], 
                                                                category, villainConning, pccCritterDifficulty[category], 0, j))
                                    PCCselectedPowers[category] |= 1 << j;
                            }
                            pccCritterDifficulty[category] = PCC_DIFF_CUSTOM;
                        }
                        PCCselectedPowers[category] &= (~(1 << i));
                        pccCritterDifficulty[category] = PCC_DIFF_CUSTOM;
                    }
                }            
                returnY += PCCSET_MIN_SPACING;
            }
            else
            {
                AtlasTex *dot = atlasLoadTexture( "EnhncTray_powertitle_dot.tga" );
                int textHt;
                char *htmlString;
                int selectable;
                selectable = (pccCritterDifficulty[category] == PCC_DIFF_CUSTOM);


                estrCreate(&htmlString);
                if (pccCritterDifficulty[category] == PCC_DIFF_CUSTOM)
                {
                    if (selectable)
                    {
                        AtlasTex *cmark = atlasLoadTexture( "costume_color_ring_main.tga" );
                        display_sprite( cmark, (x+24)*screenScaleX, returnY*screenScaleY - (dot->height*1.1f*screenScale), 55.0f, dot->width*1.2f*screenScale / cmark->width, dot->height *screenScale*1.2f/cmark->height, 
                            SELECT_FROM_UISKIN( CLR_FRAME_BLUE, CLR_FRAME_RED, CLR_FRAME_ROGUE ) );
                    }
                }
                else
                {
                    display_sprite( dot, (x+25)*screenScaleX, returnY*screenScaleY - dot->height*screenScale, 50.0f, screenScale, screenScale, CLR_WHITE );
                }
                estrConcatf(&htmlString, "<color #%s>%s</color>", selectable ? "777777" : "333333",textStd(currentPower->pchDisplayName));
                s_TextAttr_PCC_powers.piScale = (int *)(int)(1.0f*SMF_FONT_SCALE*screenScale);
                textHt = smf_ParseAndDisplay(&powersText, htmlString,
                    (x+25 + 5)*screenScaleX+dot->width*screenScale, (returnY-5)*screenScaleY-(dot->height*screenScale), 50.0f, (PCC_POWER_MC_W*screenScaleX)-((25+5)*screenScaleX+dot->width*screenScale), 50, 0,0,&s_TextAttr_PCC_powers,0,0, 0);
                estrDestroy(&htmlString);

                if (mouse)
                {
                    s_pMouseoverPower = currentPower;
                    s_mouseOverPowerIdx = i;
                    if (mouseClickHit(&mouseBox, MS_LEFT)  )
                    {
                        if ( (pccCritterDifficulty[category] != PCC_DIFF_CUSTOM) || 
                            (character_IsAllowedToHavePowerSetHypothetically(playerPtr()->pchar, missionMakerPowerSet[category]->ppPowerSets[s_mouseOverPowerSet]) &&
                            (s_mouseOverPowerSet != PCCselectedPowerSets[category]) ) )

                        {
                            int j;
                            PCCselectedPowers[category] = 0;
                            PCCselectedPowerSets[category] = s_mouseOverPowerSet;
                            for (j=0; j < eaSize(&missionMakerPowerSet[category]->ppPowerSets[s_mouseOverPowerSet]->ppPowers); ++j)
                            {
                                if (PCCPower_powerIsAvailable(missionMakerPowerSet[category]->ppPowerSets[s_mouseOverPowerSet]->ppPowers[j], 
                                                                category, villainConning, pccCritterDifficulty[category], 0, j))
                                    PCCselectedPowers[category] |= 1 << j;
                            }
                            pccCritterDifficulty[category] = PCC_DIFF_CUSTOM;
                        }
                        PCCselectedPowers[category] |= (1 << i);
                    }
                }
                returnY += PCCSET_MIN_SPACING;
            }
        }
    }
    clipperPop();
    return (returnY-startY);
}
static void displayRightPanel(float x, float y, float z, float width, float height, int category, float screenScaleX, float screenScaleY)
{
    float screenScale = MIN(screenScaleX, screenScaleY);
    font( &gamebold_12 );
    font_color( CLR_WHITE, SELECT_FROM_UISKIN( 0x88b6ffff, 0xaaaaaaff, 0x8888ccff ) );
    cprnt( (x + width/2)*screenScaleX, (PCC_POWER_MENU_Y + (2*PCCSET_SPACING) + 7.5-PIX3)*screenScaleY, 60, screenScale, screenScale, "PCCPowerDetailInfoString" );

    if ( s_pMouseoverPower )
    {
        int villainConning;
        switch (pccCritterRank)
        {
        case PCC_RANK_MINION:
            villainConning = villainRankConningAdjustment(VR_MINION);
            break;
        case PCC_RANK_LT:
            villainConning = villainRankConningAdjustment(VR_LIEUTENANT);
            break;
        case PCC_RANK_BOSS:
            villainConning = villainRankConningAdjustment(VR_BOSS);
            break;
        case PCC_RANK_ELITE_BOSS:
            villainConning = villainRankConningAdjustment(VR_BOSS);
            break;
        case PCC_RANK_ARCH_VILLAIN:
            villainConning = villainRankConningAdjustment(VR_ARCHVILLAIN);
            break;
        default:
            villainConning = villainRankConningAdjustment(VR_NONE);
            break;
        }
        if ( PCCPower_powerIsAvailable(s_pMouseoverPower, category, villainConning, pccCritterDifficulty[category], 
                        s_mouseOverPowerSet == PCCselectedPowerSets[category] ? PCCselectedPowers[category] : 0, s_mouseOverPowerIdx)    
            )
        {
            displayPowerInfo(x*screenScaleX, (y+5)*screenScaleY, z, width*screenScaleX, (height-20)*screenScaleY, s_pMouseoverPower, 0, 0, POWERINFO_BASE, 1 );
        }
        else
        {
            static SMFBlock rightPanel;
            char *translatedPowerName;
            int selectable = 1;
            estrCreate(&translatedPowerName);
            estrConcatf(&translatedPowerName, "<scale 1.2><color white>%s</color></scale>", textStd(s_pMouseoverPower->pchDisplayName) );

            if (selectable)
            {
                smf_SetRawText(&rightPanel, textStd("PCCInvalidPowerDifficulty", translatedPowerName), 0);
            }
            else
            {
                smf_SetRawText(&rightPanel, textStd("PCCInvalidPowerRank", translatedPowerName), 0);
            }
            s_TextAttr_PCC_powers.piScale = (int *)(int)(1.0f*SMF_FONT_SCALE*screenScale);
            smf_Display(&rightPanel, x*screenScaleX, (y+5)*screenScaleY, z, width*screenScaleX, (height-30)*screenScaleY, 0, 0, &s_TextAttr_PCC_powers, 0);
            estrDestroy(&translatedPowerName);
        }
    }
}
//    This function displays all the power sets and the powers
static int classHasPowerSet(const PowerCategory *primary, const PowerCategory *secondary, const PowerCategory *tertiary, const char *powerSetName, const char *displayName)
{
    //    Attempt to find if the class has the powerset.
    //    It is exactly function findSetInCategory, but without the error messages
    int i = 0;
    if (primary) 
    {    
        if (displayName)
        {
            for(i=eaSize(&primary->ppPowerSets)-1; i>=0; i--)
            {
                if(stricmp(textStd(primary->ppPowerSets[i]->pchDisplayName), textStd(displayName) )==0)
                {
                    return 1;
                }
            }
        }
    }
    if (secondary) 
    {    
        if (displayName)
        {
            for(i=eaSize(&secondary->ppPowerSets)-1; i>=0; i--)
            {
                if(stricmp(textStd(secondary->ppPowerSets[i]->pchDisplayName), textStd(displayName) )==0)
                {
                    return 1;
                }
            }
        }
    }
    if (tertiary) 
    {    
        if (displayName)
        {
            for(i=eaSize(&tertiary->ppPowerSets)-1; i>=0; i--)
            {
                if(stricmp(textStd(tertiary->ppPowerSets[i]->pchDisplayName), textStd(displayName) )==0)
                {
                    return 1;
                }
            }
        }
    }
    return 0;
}

////////////
//    createMatchList()
//    1.    use a class index to set up the hard-coded string constants to match for each class
//    2.    use the strings to get the proper power categories
//    3.    for all powersets in missionmaker, check if it is in any of the class power categories.
////////////
static void createMatchList( int *matchList, int category, int class_index )
{
    //    create the matchList
    //    first get the classes powerset categories
    char *power_class[3] = { NULL, NULL, NULL };
    const PowerCategory *pcat[3] = { NULL, NULL, NULL };
    int i;
    switch (class_index)
    {
    case PCC_BLASTER:
        {    power_class[0] = "Blaster_Ranged";        power_class[1] = "Blaster_Support";        }
        break;
    case PCC_TANKER:
        {    power_class[0] = "Tanker_Defense";        power_class[1] = "Tanker_Melee";            }
        break;
    case PCC_SCRAPPER:
        {    power_class[0] = "Scrapper_Melee";        power_class[1] = "Scrapper_Defense";        }
        break;
    case PCC_DEFENDER:
        {    power_class[0] = "Defender_Buff";        power_class[1] = "Defender_Ranged";        }
        break;
    case PCC_CONTROLLER:
        {    power_class[0] = "Controller_Control";    power_class[1] = "Controller_Buff";        }
        break;
    case PCC_BRUTE:
        {    power_class[0] = "Brute_Melee";            power_class[1] = "Brute_Defense";        }
        break;
    case PCC_STALKER:
        {    power_class[0] = "Stalker_Defense";        power_class[1] = "Stalker_Melee";        }
        break;
    case PCC_CORRUPTOR:
        {    power_class[0] = "Corruptor_Ranged";    power_class[1] = "Corruptor_Buff";        }
        break;
    case PCC_DOMINATOR:
        {    power_class[0] = "Dominator_Control";    power_class[1] = "Dominator_Assault";    }
        break;
    case PCC_MASTERMIND:
        {    power_class[0] = "Mastermind_Summon";    power_class[1] = "Mastermind_Buff";        power_class[2] = "Mastermind_Pets";    }
        break;
    }
    for (i = 0; i < 3; i++)
    {
        if (power_class[i])    pcat[i] = powerdict_GetCategoryByName(&g_PowerDictionary, power_class[i]);
        else                pcat[i] = NULL;
    }

    //    if the powerset category has the missionMaker power, set the flag for it in the matchList
    for (i = 0; i < eaSize(&missionMakerPowerSet[category]->ppPowerSets); i++)
    {
        if (missionMakerPowerSet[category]->ppPowerSets[i] && 
            classHasPowerSet(pcat[0], pcat[1], pcat[2], 
            missionMakerPowerSet[category]->ppPowerSets[i]->pchName,
            missionMakerPowerSet[category]->ppPowerSets[i]->pchDisplayName) )
        {
            matchList[i] |= class_index;
        }
    }
}


////////////
//    drawMatchingPowerSets()
//    1.    first create (one-time) a match list (set of flags) for each class
//        a.    for each class, get the powersets associated with that class
//        b.    loop through all the powersets in the missionMaker, and see if the powerset is also in the class' set
//    2.    when displaying, loop through all the powers for each class. If the class has this power, display it
////////////
static int drawMatchingPowerSets(int startY, int category, int class_index, float timer, float screenScaleX, float screenScaleY)
{
    int i;
    int returnHeight, mouse;
    static int *PowerSetMatches[2] = { NULL, NULL };
    //    for all powers
    returnHeight = startY;
    if (PowerSetMatches[category] == NULL)
    {
        int includeMisc = 0;
        PowerSetMatches[category] = calloc(eaSize(&missionMakerPowerSet[category]->ppPowerSets), sizeof(int));
        for (i = 0; i < eaSize(&missionMakerPowerSet[category]->ppPowerSets); i++)
        {
            PowerSetMatches[category][i] = 0;
        }
        for (i = PCC_CLASS_START; i < PCC_OTHER; i = i << 1)
        {
            createMatchList(PowerSetMatches[category], category, i);
        }
        for (i = 0; i < eaSize(&missionMakerPowerSet[category]->ppPowerSets); i++)
        {
            if (PowerSetMatches[category][i] == 0)
            {
                includeMisc = 1;
            }
        }
        if (includeMisc)
        {
            includeMiscPowers |= 1 << (category+1);
        }
        else
        {
            includeMiscPowers &= (~(1 << (category+1)));
        }
    }
    if (timer > 0.0f)
    {
        int spacing = PCCSET_SPACING;
        int actualSpacing = PCCSET_SPACING;
        float screenScale = MIN(screenScaleX, screenScaleY);
        if (timer > 1.0f) timer = 1.0f;
        spacing *= timer;
        returnHeight += PCCSET_SPACING;

        startY = returnHeight;
        for (i = 0; i < eaSize(&missionMakerPowerSet[category]->ppPowerSets); i++)
        {
            //    check to see if this power belongs in the power_class
            if ((class_index == PCC_ALL) || (PowerSetMatches[category][PCCSortedPowers[category][i]] & class_index) ||
                ( ( class_index == PCC_OTHER) && !PowerSetMatches[category][PCCSortedPowers[category][i]])
                )
            {
                if (actualSpacing > PCCSET_MIN_SPACING)
                {
                    Entity *e;
                    int available = 1;
                    const BasePowerSet *loopingPowerSet;
                    actualSpacing = 0;
                    loopingPowerSet = missionMakerPowerSet[category]->ppPowerSets[PCCSortedPowers[category][i]];
                    e = playerPtr();
                    if (!character_IsAllowedToHavePowerSetHypothetically(e->pchar, loopingPowerSet))
                    {
                        available = 0;
                        if (PCCselectedPowerSets[category] == PCCSortedPowers[category][i])
                        {
                            PCCselectedPowerSets[category] = -1;
                        }
                    }
                    mouse = drawCheckBar( PCCSET_X*screenScaleX, returnHeight*screenScaleY, 50, screenScale, PCCSET_WIDTH*screenScaleX, loopingPowerSet->pchDisplayName, PCCselectedPowerSets[category] == PCCSortedPowers[category][i], available, 0,0, NULL );
                    returnHeight += spacing;
                    if (mouse)
                    {
                        //    display detailed info
                        if (i != PCCselectedPowerSets[category])
                        {
                            s_pMouseoverPower = 0;
                        }
                        s_mouseOverPowerSet = PCCSortedPowers[category][i];
                        if ( mouse == D_MOUSEHIT)
                        {
                            if (PCCselectedPowerSets[category] != PCCSortedPowers[category][i])
                            {
                                int j;
                                int villainConning;
                                const BasePower *currentPower;
                                s_pMouseoverPower = 0;
                                PCCselectedPowerSets[category] = PCCSortedPowers[category][i];
                                PCCselectedPowers[category] = 0;
                                switch (pccCritterRank)
                                {
                                case PCC_RANK_MINION:
                                    villainConning = villainRankConningAdjustment(VR_MINION);
                                    break;
                                case PCC_RANK_LT:
                                    villainConning = villainRankConningAdjustment(VR_LIEUTENANT);
                                    break;
                                case PCC_RANK_BOSS:
                                    villainConning = villainRankConningAdjustment(VR_BOSS);
                                    break;
                                case PCC_RANK_ELITE_BOSS:
                                    villainConning = villainRankConningAdjustment(VR_BOSS);
                                    break;
                                case PCC_RANK_ARCH_VILLAIN:
                                    villainConning = villainRankConningAdjustment(VR_ARCHVILLAIN);
                                    break;
                                default:
                                    villainConning = villainRankConningAdjustment(VR_NONE);
                                    break;
                                }
                                for (j=0; j < eaSize(&loopingPowerSet->ppPowers); ++j)
                                {
                                    currentPower = loopingPowerSet->ppPowers[j];
                                    if (    PCCPower_powerIsAvailable(currentPower, category, villainConning, pccCritterDifficulty[category],
                                                    PCCselectedPowers[category], j))
                                    {
                                        PCCselectedPowers[category] |= 1 << j;
                                        s_mouseOverPowerIdx = -1;
                                    }
                                }
                                sndPlay("Select", SOUND_GAME);
                            }
                        }
                    }
                }
                actualSpacing += spacing;
            }
        }
        drawFrame( PIX2, R6, (PCCSET_X+20)*screenScaleX, (startY- PCCSET_SPACING)*screenScaleY, 5, (PCCSET_WIDTH - 40)*screenScaleX,
            ((returnHeight - startY)+PCCSET_SPACING)*screenScaleY, 1.f, 0xffffffaa, SELECT_FROM_UISKIN( 0x0000ff44, 0xff000044, 0x00000066 ) );
    }
    return returnHeight+(PCCSET_SPACING);
}
typedef struct PCC_powerIndex
{
    char *displayName;
    int index;
}PCC_powerIndex;
static int lessThan(const PCC_powerIndex **a, const PCC_powerIndex **b)
{
    return stricmp((*a)->displayName, (*b)->displayName);
}
static void sortPowers(int category)
{
    int i;
    PCC_powerIndex **tempPowerList = NULL;
    if (!PCCSortedPowers[category])    PCCSortedPowers[category] = calloc(eaSize(&missionMakerPowerSet[category]->ppPowerSets), sizeof(int));
    for (i = 0; i < eaSize(&missionMakerPowerSet[category]->ppPowerSets); ++i)
    {
        PCC_powerIndex *powerIndex;
        powerIndex = malloc(sizeof(PCC_powerIndex));
        powerIndex->index = i;
        estrCreate(&(powerIndex->displayName));
        estrConcatCharString(&(powerIndex->displayName), textStd(missionMakerPowerSet[category]->ppPowerSets[i]->pchDisplayName));
        eaPush(&tempPowerList, powerIndex);
    }
    eaQSort(tempPowerList, lessThan);
    for (i = 0; i < eaSize(&missionMakerPowerSet[category]->ppPowerSets); ++i)
    {
        PCCSortedPowers[category][i] = tempPowerList[i]->index;
    }
    for (i = (eaSize(&missionMakerPowerSet[category]->ppPowerSets)-1); i >= 0; --i)
    {
        estrDestroy(&tempPowerList[i]->displayName);
        free(tempPowerList[i]);
    }
    eaDestroy(&tempPowerList);
}
////////////
//    drawPCCPowerSetSelector()
//    For each class (hard-coded), there exists a static timer and toggle
//    These are used for animating separate events. This requires that all the code for displaying each class is basically a copy of the one before
////////////
static int drawPCCPowerSetSelector(int startY, int height, int category, float screenScaleX, float screenScaleY)
{
    int endY;
    UIBox box;
    float screenScale = MIN(screenScaleX, screenScaleY);
    endY = startY;
    font( &gamebold_12 );
    font_color( CLR_WHITE, SELECT_FROM_UISKIN( 0x88b6ffff, 0xaaaaaaff, 0x8888ccff ) );
    cprnt((PCC_POWER_LC_X + (PCC_POWER_LC_W/2))*screenScaleX, (55+PIX3 + 7.5)*screenScaleY,60, screenScale, screenScale, "PCCPowerSetSelectionString" );
    uiBoxDefine(&box, 55*screenScaleX, (PCC_POWER_MENU_Y+PIX3-79)*screenScaleY, 355*screenScaleX, height*screenScaleY);
    clipperPush( &box );

    if (PCCSortedPowers[category] == NULL)
    {
        sortPowers(category);
    }
    //    for each type of filter
    //        draw the button
    //    Note: each section requires its own static variables because the timers need to be unique for each filter. 
    //    That is why there is this gian list of repeated code.
    {
        int mouse2;
        static int showToggle = 0;
        static float timer;
        if (resetOpenPowersets)
        {
            showToggle = 0;
            timer = 0;
        }
        mouse2 = drawMenuBarSquished((PCC_POWER_LC_X + (PCC_POWER_LC_W/2))*screenScaleX, endY*screenScaleY, 10, screenScale, (PCC_POWER_LC_W-25)*screenScaleX/screenScale, textStd("AllString"), 0, 1, 0, 0, 0, 0, 1.0f, CLR_WHITE, &title_12, 1);
        if (mouse2)
        {
            //    show mouse2over text
            if (PCCselectedPowerSets[category] == -1)
            {
                s_mouseOverPowerSet = -PCC_ALL;
                s_pMouseoverPower = NULL;
            }
            if (mouse2 == D_MOUSEHIT)
            {
                showToggle = !showToggle;
            }
        }
        if (showToggle)    {    timer += TIMESTEP * 0.1f;    timer = CLAMP(timer, 0.0f, 1.0f);}
        else            {    timer -= TIMESTEP * 0.1f;    timer = CLAMP(timer, 0.0f, 1.0f);}
        endY = drawMatchingPowerSets(endY, category, PCC_ALL, timer, screenScaleX, screenScaleY);
    }
    {
        int mouse2;
        static int showToggle = 0;
        static float timer = 0.0f;
        if (resetOpenPowersets)
        {
            showToggle = 0;
            timer = 0;
        }
        mouse2 = drawMenuBarSquished((PCC_POWER_LC_X + (PCC_POWER_LC_W/2))*screenScaleX, endY*screenScaleY, 10, screenScale, (PCC_POWER_LC_W-25)*screenScaleX/screenScale,textStd("Class_Blaster"), 0, 1, 0, 0, 0, 0, 1.0f, CLR_WHITE, &title_12, 1);
        if (mouse2)
        {
            //    show mouseover text
            if (PCCselectedPowerSets[category] == -1)
            {
            s_mouseOverPowerSet = -PCC_BLASTER;
            s_pMouseoverPower = NULL;
            }
            if (mouse2 == D_MOUSEHIT)
            {
                sndPlay("Select", SOUND_GAME);
                showToggle = !showToggle;
            }
        }
        if (showToggle)    {    timer += TIMESTEP * 0.1f;    timer = CLAMP(timer, 0.0f, 1.0f);}
        else            {    timer -= TIMESTEP * 0.1f;    timer = CLAMP(timer, 0.0f, 1.0f);}
        endY = drawMatchingPowerSets(endY, category, PCC_BLASTER, timer, screenScaleX, screenScaleY);
    }
    {
        int mouse2;
        static int showToggle = 0;
        static float timer = 0.0f;
        if (resetOpenPowersets)
        {
            showToggle = 0;
            timer = 0;
        }
        mouse2 = drawMenuBarSquished((PCC_POWER_LC_X + (PCC_POWER_LC_W/2))*screenScaleX, endY*screenScaleY, 10, screenScale, (PCC_POWER_LC_W-25)*screenScaleX/screenScale, textStd("Class_Tanker"), 0, 1, 0, 0, 0, 0, 1.0f, CLR_WHITE, &title_12, 1);
        if (mouse2)
        {
            //    show mouse2over text
            if (PCCselectedPowerSets[category] == -1)
            {
            s_mouseOverPowerSet = -PCC_TANKER;
            s_pMouseoverPower = NULL;
            }
            if (mouse2 == D_MOUSEHIT)
            {
                showToggle = !showToggle;
            }
        }
        if (showToggle)    {    timer += TIMESTEP * 0.1f;    timer = CLAMP(timer, 0.0f, 1.0f);}
        else            {    timer -= TIMESTEP * 0.1f;    timer = CLAMP(timer, 0.0f, 1.0f);}
        endY = drawMatchingPowerSets(endY, category, PCC_TANKER,timer, screenScaleX, screenScaleY);
    }
    {
        int mouse2;
        static int showToggle = 0;
        static float timer = 0.0f;
        if (resetOpenPowersets)
        {
            showToggle = 0;
            timer = 0;
        }
        mouse2 = drawMenuBarSquished((PCC_POWER_LC_X + (PCC_POWER_LC_W/2))*screenScaleX, endY*screenScaleY, 10, screenScale, (PCC_POWER_LC_W-25)*screenScaleX/screenScale, textStd("Class_Scrapper"), 0, 1, 0, 0, 0, 0, 1.0f, CLR_WHITE, &title_12, 1);
        if (mouse2)
        {
            //    show mouse2over text
            if (PCCselectedPowerSets[category] == -1)
            {
            s_mouseOverPowerSet = -PCC_SCRAPPER;
            s_pMouseoverPower = NULL;
            }

            if (mouse2 == D_MOUSEHIT)
            {
                showToggle = !showToggle;
            }
        }
        if (showToggle)    {    timer += TIMESTEP * 0.1f;    timer = CLAMP(timer, 0.0f, 1.0f);}
        else            {    timer -= TIMESTEP * 0.1f;    timer = CLAMP(timer, 0.0f, 1.0f);}
        endY = drawMatchingPowerSets(endY, category, PCC_SCRAPPER, timer, screenScaleX, screenScaleY);
    }
    {
        int mouse2;
        static int showToggle = 0;
        static float timer = 0.0f;
        if (resetOpenPowersets)
        {
            showToggle = 0;
            timer = 0;
        }
        mouse2 = drawMenuBarSquished((PCC_POWER_LC_X + (PCC_POWER_LC_W/2))*screenScaleX, endY*screenScaleY, 10, screenScale, (PCC_POWER_LC_W-25)*screenScaleX/screenScale, textStd("Class_Defender"), 0, 1, 0, 0, 0, 0, 1.0f, CLR_WHITE, &title_12, 1);
        if (mouse2)
        {
            //    show mouse2over text
            if (PCCselectedPowerSets[category] == -1)
            {
            s_mouseOverPowerSet = -PCC_DEFENDER;
            s_pMouseoverPower = NULL;
            }

            if (mouse2 == D_MOUSEHIT)
            {
                showToggle = !showToggle;
            }
        }
        if (showToggle)    {    timer += TIMESTEP * 0.1f;    timer = CLAMP(timer, 0.0f, 1.0f);}
        else            {    timer -= TIMESTEP * 0.1f;    timer = CLAMP(timer, 0.0f, 1.0f);}
        endY = drawMatchingPowerSets(endY, category, PCC_DEFENDER, timer, screenScaleX, screenScaleY);
    }
    {
        int mouse2;
        static int showToggle = 0;
        static float timer = 0.0f;
        if (resetOpenPowersets)
        {
            showToggle = 0;
            timer = 0;
        }
        mouse2 = drawMenuBarSquished((PCC_POWER_LC_X + (PCC_POWER_LC_W/2))*screenScaleX, endY*screenScaleY, 10, screenScale, (PCC_POWER_LC_W-25)*screenScaleX/screenScale, textStd("Class_Controller"), 0, 1, 0, 0, 0, 0, 1.0f, CLR_WHITE, &title_12, 1);
        if (mouse2)
        {
            //    show mouse2over text
            if (PCCselectedPowerSets[category] == -1)
            {
            s_mouseOverPowerSet = -PCC_CONTROLLER;
            s_pMouseoverPower = NULL;
            }

            if (mouse2 == D_MOUSEHIT)
            {
                showToggle = !showToggle;
            }
        }
        if (showToggle)    {    timer += TIMESTEP * 0.1f;    timer = CLAMP(timer, 0.0f, 1.0f);}
        else            {    timer -= TIMESTEP * 0.1f;    timer = CLAMP(timer, 0.0f, 1.0f);}
        endY = drawMatchingPowerSets(endY, category, PCC_CONTROLLER, timer, screenScaleX, screenScaleY);
    }
    {
        int mouse2;
        static int showToggle = 0;
        static float timer = 0.0f;
        if (resetOpenPowersets)
        {
            showToggle = 0;
            timer = 0;
        }
        mouse2 = drawMenuBarSquished((PCC_POWER_LC_X + (PCC_POWER_LC_W/2))*screenScaleX, endY*screenScaleY, 10, screenScale, (PCC_POWER_LC_W-25)*screenScaleX/screenScale, textStd("Class_Brute"), 0, 1, 0, 0, 0, 0, 1.0f, CLR_WHITE, &title_12, 1);
        if (mouse2)
        {
            //    show mouse2over text
            if (PCCselectedPowerSets[category] == -1)
            {
            s_mouseOverPowerSet = -PCC_BRUTE;
            s_pMouseoverPower = NULL;
            }

            if (mouse2 == D_MOUSEHIT)
            {
                showToggle = !showToggle;
            }
        }
        if (showToggle)    {    timer += TIMESTEP * 0.1f;    timer = CLAMP(timer, 0.0f, 1.0f);}
        else            {    timer -= TIMESTEP * 0.1f;    timer = CLAMP(timer, 0.0f, 1.0f);}
        endY = drawMatchingPowerSets(endY, category, PCC_BRUTE, timer, screenScaleX, screenScaleY);
    }
    {
        int mouse2;
        static int showToggle = 0;
        static float timer = 0.0f;
        if (resetOpenPowersets)
        {
            showToggle = 0;
            timer = 0;
        }
        mouse2 = drawMenuBarSquished((PCC_POWER_LC_X + (PCC_POWER_LC_W/2))*screenScaleX, endY*screenScaleY, 10, screenScale, (PCC_POWER_LC_W-25)*screenScaleX/screenScale, textStd("Class_Stalker"), 0, 1, 0, 0, 0, 0, 1.0f, CLR_WHITE, &title_12, 1);
        if (mouse2)
        {
            //    show mouse2over text
            if (PCCselectedPowerSets[category] == -1)
            {
            s_mouseOverPowerSet = -PCC_STALKER;
            s_pMouseoverPower = NULL;
            }

            if (mouse2 == D_MOUSEHIT)
            {
                showToggle = !showToggle;
            }
        }
        if (showToggle)    {    timer += TIMESTEP * 0.1f;    timer = CLAMP(timer, 0.0f, 1.0f);}
        else            {    timer -= TIMESTEP * 0.1f;    timer = CLAMP(timer, 0.0f, 1.0f);}
        endY = drawMatchingPowerSets(endY, category, PCC_STALKER, timer, screenScaleX, screenScaleY);
    }
    {
        int mouse2;
        static int showToggle = 0;
        static float timer = 0.0f;
        if (resetOpenPowersets)
        {
            showToggle = 0;
            timer = 0;
        }
        mouse2 = drawMenuBarSquished((PCC_POWER_LC_X + (PCC_POWER_LC_W/2))*screenScaleX, endY*screenScaleY, 10, screenScale, (PCC_POWER_LC_W-25)*screenScaleX/screenScale, textStd("Class_Corruptor"), 0, 1, 0, 0, 0, 0, 1.0f, CLR_WHITE, &title_12, 1);
        if (mouse2)
        {
            //    show mouse2over text
            if (PCCselectedPowerSets[category] == -1)
            {
            s_mouseOverPowerSet = -PCC_CORRUPTOR;
            s_pMouseoverPower = NULL;
            }

            if (mouse2 == D_MOUSEHIT)
            {
                showToggle = !showToggle;
            }
        }
        if (showToggle)    {    timer += TIMESTEP * 0.1f;    timer = CLAMP(timer, 0.0f, 1.0f);}
        else            {    timer -= TIMESTEP * 0.1f;    timer = CLAMP(timer, 0.0f, 1.0f);}
        endY = drawMatchingPowerSets(endY, category, PCC_CORRUPTOR, timer, screenScaleX, screenScaleY);
    }
    {
        int mouse2;
        static int showToggle = 0;
        static float timer = 0.0f;
        if (resetOpenPowersets)
        {
            showToggle = 0;
            timer = 0;
        }
        mouse2 = drawMenuBarSquished((PCC_POWER_LC_X + (PCC_POWER_LC_W/2))*screenScaleX, endY*screenScaleY, 10, screenScale, (PCC_POWER_LC_W-25)*screenScaleX/screenScale, textStd("Class_Dominator"), 0, 1, 0, 0, 0, 0, 1.0f, CLR_WHITE, &title_12, 1);
        if (mouse2)
        {
            //    show mouse2over text
            if (PCCselectedPowerSets[category] == -1)
            {
            s_mouseOverPowerSet = -PCC_DOMINATOR;
            s_pMouseoverPower = NULL;
            }

            if (mouse2 == D_MOUSEHIT)
            {
                showToggle = !showToggle;
            }
        }
        if (showToggle)    {    timer += TIMESTEP * 0.1f;    timer = CLAMP(timer, 0.0f, 1.0f);}
        else            {    timer -= TIMESTEP * 0.1f;    timer = CLAMP(timer, 0.0f, 1.0f);}
        endY = drawMatchingPowerSets(endY, category, PCC_DOMINATOR, timer, screenScaleX, screenScaleY);
    }
    {
        int mouse2;
        static int showToggle = 0;
        static float timer = 0.0f;
        if (resetOpenPowersets)
        {
            showToggle = 0;
            timer = 0;
        }
        mouse2 = drawMenuBarSquished((PCC_POWER_LC_X + (PCC_POWER_LC_W/2))*screenScaleX, endY*screenScaleY, 10, screenScale, (PCC_POWER_LC_W-25)*screenScaleX/screenScale, textStd("Class_MasterMind"), 0, 1, 0, 0, 0, 0, 1.0f, CLR_WHITE, &title_12, 1);
        if (mouse2)
        {
            //    show mouse2over text
            if (PCCselectedPowerSets[category] == -1)
            {
            s_mouseOverPowerSet = -PCC_MASTERMIND;
            s_pMouseoverPower = NULL;
            }

            if (mouse2 == D_MOUSEHIT)
            {
                showToggle = !showToggle;
            }
        }
        if (showToggle)    {    timer += TIMESTEP * 0.1f;    timer = CLAMP(timer, 0.0f, 1.0f);}
        else            {    timer -= TIMESTEP * 0.1f;    timer = CLAMP(timer, 0.0f, 1.0f);}
        endY = drawMatchingPowerSets(endY, category, PCC_MASTERMIND, timer, screenScaleX, screenScaleY);
    }
    {
        if (includeMiscPowers & (1 << (category+1)))
        {
            int mouse2;
            static int showToggle = 0;
            static float timer = 0.0f;
            if (resetOpenPowersets)
            {
                showToggle = 0;
                timer = 0;
            }
            mouse2 = drawMenuBarSquished((PCC_POWER_LC_X + (PCC_POWER_LC_W/2))*screenScaleX, endY*screenScaleY, 10, screenScale, (PCC_POWER_LC_W-25)*screenScaleX/screenScale, textStd("OptionsMisc"), 0, 1, 0, 0, 0, 0, 1.0f, CLR_WHITE, &title_12, 1);;
            if (mouse2)
            {
                //    show mouse2over text
                if (PCCselectedPowerSets[category] == -1)
                {
                s_mouseOverPowerSet = -PCC_OTHER;
                s_pMouseoverPower = NULL;
                }

                if (mouse2 == D_MOUSEHIT)
                {
                    showToggle = !showToggle;
                }
            }
            if (showToggle)    {    timer += TIMESTEP * 0.1f;    timer = CLAMP(timer, 0.0f, 1.0f);}
            else            {    timer -= TIMESTEP * 0.1f;    timer = CLAMP(timer, 0.0f, 1.0f);}
            endY = drawMatchingPowerSets(endY, category, PCC_OTHER, timer, screenScaleX, screenScaleY);
        }
    }
    if (resetOpenPowersets)
    {
        resetOpenPowersets = 0;
    }
    clipperPop();
    //    minus the spacing one time to account for padding space on the last element
    return (endY - startY);
}

static F32 calcCustomPowerXP( int rank, int villainConning, int level )
{
    int category;
    const BasePower **basePowerList = NULL;
    F32 retVal;
    if( pccCritterDifficulty[0] >= 0 && pccCritterDifficulty[1] >= 0 )
        return (g_PCCRewardMods.fDifficulty[pccCritterDifficulty[0]] + g_PCCRewardMods.fDifficulty[pccCritterDifficulty[1]])/2;

    // at least one set is custom, so add up all powers
     for( category = 0; category < 2; category++ )
    {
        int i;
        for (i = 0; PCCselectedPowerSets[category] >= 0 && i < eaSize(&missionMakerPowerSet[category]->ppPowerSets[PCCselectedPowerSets[category]]->ppPowers); i++ )
        {
            const BasePower *currentPower = missionMakerPowerSet[category]->ppPowerSets[PCCselectedPowerSets[category]]->ppPowers[i];
            if( PCCPower_powerIsAvailable(currentPower, category, villainConning, pccCritterDifficulty[category], 0, i) ||
                (pccCritterDifficulty[category] == PCC_DIFF_CUSTOM && PCCselectedPowers[category]&(1<<i)) )
            {
                eaPushConst(&basePowerList, currentPower);
            }
        }
    }
    retVal = pcccritter_tallyPowerValueFromPowers(&basePowerList, rank, level);
    eaDestroyConst(&basePowerList);
    return retVal;
}

static void drawPCC_ExperienceTally(F32 x, F32 y, F32 z, F32 width, F32 height, float screenScaleX, float screenScaleY )
{
    F32 xp_fraction;
    static F32 s_fLevel;
    int w,h,iLevel;
    int villainConning;
    float screenScale = MIN(screenScaleX, screenScaleY);
    Entity *e = playerPtr();
    static HelpButton *help;
    CBox bounds;

 
     drawPCCFrame( x, y, width, height, z, CLR_WHITE, 0xffffff11, screenScaleX, screenScaleY);

     font( &gamebold_12 );
     font_color( CLR_WHITE, SELECT_FROM_UISKIN( 0x88b6ffff, 0xaaaaaaff, 0x8888ccff ) );
    cprnt( (x + width/2)*screenScaleX, (y + 7.5-PIX3)*screenScaleY, 60, screenScale, screenScale, "PCCExperienceValue" );

       s_fLevel = doSliderAll((x + width/2)*screenScaleX, (y+height-15)*screenScaleY, z+10, (width-200)*screenScaleX, screenScale, screenScale, s_fLevel, -1.f, 1.f, SLIDER_CUSTOM_CRITTER_LEVEL, CLR_WHITE, 0, 0, 0);
    iLevel = round((s_fLevel+1)*24.5);

    switch (pccCritterRank)
    {
    case PCC_RANK_MINION:
        villainConning = villainRankConningAdjustment(VR_MINION);
        break;
    case PCC_RANK_LT:
        villainConning = villainRankConningAdjustment(VR_LIEUTENANT);
        break;
    case PCC_RANK_BOSS:
        villainConning = villainRankConningAdjustment(VR_BOSS);
        break;
    case PCC_RANK_ELITE_BOSS:
        villainConning = villainRankConningAdjustment(VR_BOSS);
        break;
    case PCC_RANK_ARCH_VILLAIN:
        villainConning = villainRankConningAdjustment(VR_ARCHVILLAIN);
        break;
    default:
        villainConning = villainRankConningAdjustment(VR_NONE);
        break;
    }

    xp_fraction = calcCustomPowerXP(pccCritterRank, villainConning,  iLevel+1);

     font( &game_14 );
    font_color( CLR_WHITE, CLR_WHITE);
    cprntEx( (x + width/2)*screenScaleX, (y + 25)*screenScaleY, 60, screenScale, screenScale, CENTER_X, "PCCExperienceAtLevel", xp_fraction*100, e->pchar->pclass->pchDisplayName, iLevel+1 );

     windowClientSize(&w, &h);

    BuildCBox(&bounds, 300, 0, w-300, h);
       helpButtonEx( &help, (x + width-25)*screenScaleX, (y + 25)*screenScaleY, z+10, 1.5f, "PCCExperienceHelp", &bounds, HELP_BUTTON_USE_BOX_WIDTH);

}
static void PCCprimaryPowerMenuEnterCode(void)
{
    pcc_powerSelection = pcc_MenuPrimary;
}
static int PCCprimaryPowerMenuCantEnter(void)
{
    return (pccCritterRank == PCC_RANK_CONTACT);
}

static int PCCsecondaryPowerMenuCantEnter(void)
{
    return (pccCritterRank == PCC_RANK_CONTACT) || (PCCselectedPowerSets[pcc_MenuPrimary] == -1);
}
static void PCCsecondaryPowerMenuEnterCode(void)
{
    pcc_powerSelection = pcc_MenuSecondary;
}
static int prev_primary_power = -1;
static void PCCpowerMenuPassThroughCode(void)
{
    int i;
    Entity *e = playerPtr();
    PowerSet *pset;
    //    reset in case they are toggling back and forth
    if (!missionMakerPowerSet[0])
    {
        missionMakerPowerSet[0] = powerdict_GetCategoryByName(&g_PowerDictionary, "Mission_Maker_Attacks");
        missionMakerPowerSet[1] = powerdict_GetCategoryByName(&g_PowerDictionary, "Mission_Maker_Secondary");
        missionMakerPowerSet[2] = powerdict_GetCategoryByName(&g_PowerDictionary, "Mission_Maker_Movement");
    }

    if ((PCCselectedPowerSets[0] >= 0) 
        && character_OwnsPowerSet(e->pchar, missionMakerPowerSet[0]->ppPowerSets[PCCselectedPowerSets[0]]) == NULL)
    {
        int j;
        pset = character_AddNewPowerSet(e->pchar, missionMakerPowerSet[0]->ppPowerSets[PCCselectedPowerSets[0]] );
        for (j = 0; j < eaSize(&missionMakerPowerSet[0]->ppPowerSets[PCCselectedPowerSets[0]]->ppPowers); ++j)
        {
            character_BuyPower(e->pchar, pset, missionMakerPowerSet[0]->ppPowerSets[PCCselectedPowerSets[0]]->ppPowers[j], 0);
        }
    }
    if (PCCselectedPowerSets[pcc_MenuSecondary] != -1)
    {
        if ( !character_IsAllowedToHavePowerSetHypothetically(e->pchar, missionMakerPowerSet[pcc_MenuSecondary]->ppPowerSets[PCCselectedPowerSets[pcc_MenuSecondary]]))
        {
            PCCselectedPowerSets[pcc_MenuSecondary] = -1;
        }
    }
    prev_primary_power = PCCselectedPowerSets[0];

    for (i = 0; i < pcc_MenuCount; i++)
    {
        if (currentlyBoughtCritterPowers[i] >= 0)
        {
            costumeUnawardPowerSetPartsFromPower(e, missionMakerPowerSet[i]->ppPowerSets[currentlyBoughtCritterPowers[i]], 1);
        }
    }
    for (i = 0; i < pcc_MenuCount; i++)
    {
        currentlyBoughtCritterPowers[i] = PCCselectedPowerSets[i];
        if( PCCselectedPowerSets[i] >= 0 )
        {
            //    The parts that are given here, must eventually be removed
            //    For critters, remove at the entrance and exit to the creation process
            //    In rank and profile, call destroy PCC_Entity, which unawards powerset parts
            costumeAwardPowerSetPartsFromPower(e, missionMakerPowerSet[i]->ppPowerSets[PCCselectedPowerSets[i]], 0, 1);                
        }
    }
    e->pl->costume[0] = costume_as_mutable_cast(e->costume);
}

static int init = false;
static void PCCpowerMenuExitCode(void)
{
    Entity *e = playerPtr();
    init = false;
    s_mouseOverPowerSet = -1;
    s_pMouseoverPower = 0;
    
    switch (pcc_powerSelection)
    {
    case pcc_MenuPrimary:
    if (prev_primary_power != -1)
    {
        character_RemovePowerSet(e->pchar, missionMakerPowerSet[0]->ppPowerSets[prev_primary_power], NULL );
        prev_primary_power = -1;
    }
    break;
    case pcc_MenuSecondary:
        PCCpowerMenuPassThroughCode();
        break;
    default:
        assert(0);
    }
}


NonLinearMenuElement PCC_powers_primary_NLME = { MENU_PCC_POWERS, { "CC_PrimaryPowersString", 0, 0 },
0, PCCprimaryPowerMenuCantEnter, PCCprimaryPowerMenuEnterCode, 0, PCCpowerMenuExitCode };
NonLinearMenuElement PCC_powers_secondary_NLME = { MENU_PCC_POWERS, { "CC_SecondaryPowersString", 0, 0 },
0, PCCsecondaryPowerMenuCantEnter, PCCsecondaryPowerMenuEnterCode, PCCpowerMenuPassThroughCode, PCCpowerMenuExitCode};


void PCCPowersMenu(void)
{
    Entity *e;
    AtlasTex *layover = NULL;
    static ScrollBar sb[2] = {0};
    int screenScalingDisabled = is_scrn_scaling_disabled();
    float screenScaleX = 1.f, screenScaleY = 1.f, screenScale;

    calc_scrn_scaling(&screenScaleX, &screenScaleY);
    screenScale = MIN(screenScaleX, screenScaleY);

    if (getPCCEditingMode() < 0)
        setPCCEditingMode(getPCCEditingMode()*-1);

    //    Draw background
     drawBackground( layover );

    set_scrn_scaling_disabled(1);
    //    draw left frame
       drawPCCFrame( PCC_POWER_LC_X, 60 , PCC_POWER_LC_W, 640, 10, CLR_WHITE, 0x00000088, screenScaleX, screenScaleY);
    //    draw center frame
    drawPCCFrame( PCC_POWER_LC_X+PCC_POWER_LC_W+PCC_POWER_COLUMN_SPACING, 60, PCCSET_MAJOR_WIDTH,
         PCCSET_SPACING*1.5, 10, CLR_WHITE, 0x00000088, screenScaleX, screenScaleY );

    drawPCCFrame( PCC_POWER_LC_X+PCC_POWER_LC_W+PCC_POWER_COLUMN_SPACING, PCC_POWER_MENU_Y + (2*PCCSET_SPACING), PCC_POWER_MC_W,
        PCC_POWER_MENU_HEIGHT - (2 *PCCSET_SPACING), 10, CLR_WHITE, 0x00000088, screenScaleX, screenScaleY );
    //    draw right frame
    drawPCCFrame( PCC_POWER_LC_X+PCC_POWER_LC_W+PCC_POWER_MC_W+(2*PCC_POWER_COLUMN_SPACING),PCC_POWER_MENU_Y + (2*PCCSET_SPACING), 
        900-(PCC_POWER_LC_W+PCC_POWER_MC_W+(2*PCC_POWER_COLUMN_SPACING)),PCC_POWER_MENU_HEIGHT - (2 *PCCSET_SPACING), 10, CLR_WHITE, 0x00000088, screenScaleX, screenScaleY );

    //    Get the Entity pointer for the player created critter
    e = playerPtr();

    if (!init)
    {
        init = true;
        resetOpenPowersets = 1;
        if (!missionMakerPowerSet[0])
        {
            missionMakerPowerSet[0] = powerdict_GetCategoryByName(&g_PowerDictionary, "Mission_Maker_Attacks");
            missionMakerPowerSet[1] = powerdict_GetCategoryByName(&g_PowerDictionary, "Mission_Maker_Secondary");
            missionMakerPowerSet[2] = powerdict_GetCategoryByName(&g_PowerDictionary, "Mission_Maker_Movement");
        }

        if (pcc_powerSelection == pcc_MenuSecondary)
        {
            PowerSet *pset;

            if (character_OwnsPowerSet(e->pchar, missionMakerPowerSet[0]->ppPowerSets[PCCselectedPowerSets[0]]) == NULL)
            {
                int j;
                pset = character_AddNewPowerSet(e->pchar, missionMakerPowerSet[0]->ppPowerSets[PCCselectedPowerSets[0]] );
                for (j = 0; j < eaSize(&missionMakerPowerSet[0]->ppPowerSets[PCCselectedPowerSets[0]]->ppPowers); ++j)
                {
                    character_BuyPower(e->pchar, pset, missionMakerPowerSet[0]->ppPowerSets[PCCselectedPowerSets[0]]->ppPowers[j], 0);
                }
            }
            if (PCCselectedPowerSets[pcc_MenuSecondary] != -1)
            {
                if ( !character_IsAllowedToHavePowerSetHypothetically(e->pchar, missionMakerPowerSet[pcc_MenuSecondary]->ppPowerSets[PCCselectedPowerSets[pcc_MenuSecondary]]))
                {
                    PCCselectedPowerSets[pcc_MenuSecondary] = -1;
                }
            }
            prev_primary_power = PCCselectedPowerSets[0];
        }
        if (PCCselectedPowerSets[pcc_powerSelection] != -1)
        {
            s_mouseOverPowerSet = PCCselectedPowerSets[pcc_powerSelection];
        }
        powerInfoSetLevel(e->pchar->iLevel);        
        powerInfoSetClass(e->pchar->pclass);

        if ((pccCritterDifficulty[pcc_powerSelection] == PCC_DIFF_CUSTOM) && (PCCselectedPowers[pcc_powerSelection] < 0) && (s_mouseOverPowerSet >= 0))
        {
            int j;
            int villainConning;
            PCCselectedPowers[pcc_powerSelection] *= -1;
            switch (pccCritterRank)
            {
            case PCC_RANK_MINION:
                villainConning = villainRankConningAdjustment(VR_MINION);
                break;
            case PCC_RANK_LT:
                villainConning = villainRankConningAdjustment(VR_LIEUTENANT);
                break;
            case PCC_RANK_BOSS:
                villainConning = villainRankConningAdjustment(VR_BOSS);
                break;
            case PCC_RANK_ELITE_BOSS:
                villainConning = villainRankConningAdjustment(VR_BOSS);
                break;
            case PCC_RANK_ARCH_VILLAIN:
                villainConning = villainRankConningAdjustment(VR_ARCHVILLAIN);
                break;
            default:
                villainConning = villainRankConningAdjustment(VR_NONE);
                break;
            }
            for (j=0; j < eaSize(&missionMakerPowerSet[pcc_powerSelection]->ppPowerSets[s_mouseOverPowerSet]->ppPowers); ++j)
            {
                const BasePower *power = missionMakerPowerSet[pcc_powerSelection]->ppPowerSets[s_mouseOverPowerSet]->ppPowers[j];
                if ((villainConning <= basepower_GetAIMaxRankCon(power) ) &&
                    (villainConning >= basepower_GetAIMinRankCon(power) )  &&
                    (basepower_GetMinDifficulty(power)== -1))
                    PCCselectedPowers[pcc_powerSelection] |= 1 << j;
            }
        }
    }

    //    Draw power selections on left
    {
        int ht;
        CBox box;
        ht = drawPCCPowerSetSelector(100 - sb[0].offset, (PCC_POWER_MENU_HEIGHT+80)-(3*PIX3), pcc_powerSelection, screenScaleX, screenScaleY);
        BuildCBox(&box, 60*screenScaleX, (PCC_POWER_MENU_Y+PIX3-80)*screenScaleY, 270*screenScaleX, ((PCC_POWER_MENU_HEIGHT+80)-(2*PIX3))*screenScaleY);
        doScrollBarEx(&sb[0], ((PCC_POWER_MENU_HEIGHT+80-(PIX3))-50), ht, (PCC_POWER_LC_X+PCC_POWER_LC_W), 90, 30, &box, 0, screenScaleX, screenScaleY );

    }

    drawPCC_DifficultyPreference( PCC_POWER_LC_X+PCC_POWER_LC_W+PCC_POWER_COLUMN_SPACING +30, 60, screenScaleX, screenScaleY );
     drawPCC_ExperienceTally( PCC_POWER_LC_X+PCC_POWER_LC_W+PCC_POWER_COLUMN_SPACING, 150,30, 960 -  (PCC_POWER_LC_X+PCC_POWER_LC_W+PCC_POWER_COLUMN_SPACING), 50, screenScaleX, screenScaleY);

    {
        int ht;
        CBox box;
        //    Draw the middle panel where the powerList will go
        ht = displayMiddlePanel(PCC_POWER_LC_X+PCC_POWER_LC_W+PCC_POWER_COLUMN_SPACING, 155 + (2*PCCSET_SPACING) - sb[1].offset, 30, PCC_POWER_MC_W, (PCC_POWER_MENU_HEIGHT-((3*PIX3) + ( 2*PCCSET_SPACING))), pcc_powerSelection, screenScaleX, screenScaleY);
        BuildCBox(&box, (PCC_POWER_LC_X+PCC_POWER_LC_W+PCC_POWER_COLUMN_SPACING)*screenScaleX, (PCC_POWER_MENU_Y+PIX3+ ( 2*PCCSET_SPACING))*screenScaleY, PCC_POWER_MC_W*screenScaleX, (PCC_POWER_MENU_HEIGHT-((2*PIX3)+(2*PCCSET_SPACING)))*screenScaleY );
        doScrollBarEx(&sb[1], ((PCC_POWER_MENU_HEIGHT-((2*PIX3)+( 2*PCCSET_SPACING)))-50),ht, (PCC_POWER_LC_X+PCC_POWER_LC_W+PCC_POWER_MC_W+PCC_POWER_COLUMN_SPACING), (170+( 2*PCCSET_SPACING)), 30, &box, 0, screenScaleX, screenScaleY );
    }
    //    Draw the specific power information on the right
    displayRightPanel(PCC_POWER_LC_X+PCC_POWER_LC_W+PCC_POWER_MC_W+(2*PCC_POWER_COLUMN_SPACING)+10, 155+ (2*PCCSET_SPACING), 30, 
        960-(PCC_POWER_LC_X+PCC_POWER_LC_W+PCC_POWER_MC_W+(2*PCC_POWER_COLUMN_SPACING)+(2*10)), (PCC_POWER_MENU_HEIGHT-((3*PIX3) + ( 2*PCCSET_SPACING))), pcc_powerSelection, screenScaleX, screenScaleY);
    
    //    Draw back button
    if ( drawNextButton( 40*screenScaleX, 740*screenScaleY, 10, screenScale, 0, 0, 0, "BackString" ) )
    {
        init = false;
        s_mouseOverPowerSet = -1;
        s_pMouseoverPower = 0;
        sndPlay( "back", SOUND_GAME );
        if (pcc_powerSelection != pcc_MenuPrimary)
        {
            pcc_powerSelection--;
            start_menu( MENU_PCC_POWERS );
        }
        else
        {
            if (prev_primary_power != -1)
            {
                character_RemovePowerSet(e->pchar, missionMakerPowerSet[pcc_powerSelection]->ppPowerSets[prev_primary_power], NULL );
                prev_primary_power = -1;
            }
            start_menu( MENU_PCC_RANK );
        }        
    }
    //    Draw next button
    //    They only have to pick a primary power. Other powers are optional.
    if ( drawNextButton( 980*screenScaleX, 740*screenScaleY, 10, screenScale, 1, (pcc_powerSelection < pcc_MenuTravel) ? (PCCselectedPowerSets[pcc_powerSelection] == -1) : 0, 0, "NextString" ) )
    {
        PCCpowerMenuExitCode();
        sndPlay( "next", SOUND_GAME );
        if (pcc_powerSelection < pcc_MenuSecondary)
        {
            pcc_powerSelection++;
        }        
        else
        {            
            start_menu( nextMenu );
        }
    }

    set_scrn_scaling_disabled(0);

    {
        int currentId = pcc_powerSelection == pcc_MenuPrimary ? PCC_powers_primary_NLME.listId : PCC_powers_secondary_NLME.listId;
        drawNonLinearMenu( NLM_PCCCreationMenu(), 20, 10, 20, 1000,20, 1.0f, 
            currentId, currentId, 0 );
    }

    if (getPCCEditingMode() > 0)
        setPCCEditingMode(getPCCEditingMode()*-1);

    set_scrn_scaling_disabled(screenScalingDisabled);
}