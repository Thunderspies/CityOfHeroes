#include "entity/PCC_Critter.h"
#include "entity/PCC_Critter_Client.h"
#include "uiPCCProfile.h"
#include "uiPCCRank.h"
#include "UI/uiGame.h"
#include "UI/uiUtil.h"
#include "UI/uiUtilGame.h"
#include "UI/uiUtilMenu.h"
#include "UI/uiClipper.h"
#include "UI/uiBox.h"                    //    for uiBox

#include "formatter/smf_main.h"                //    text editing
#include "UI/sprite/sprite_base.h"            //    display things
#include "UI/sprite/sprite_text.h"
#include "UI/sprite/sprite_font.h"
#include "graphics/textureatlas.h"
#include <utilitieslib/components/EString.h>                //    estrings

#include "cmdparse/cmdgame.h"                //    game commands
#include "sound/sound.h"                    //    to play sounds
#include <utilitieslib/language/MessageStoreUtil.h>        //    for    textStd
#include "player/player.h"                    //    for playerPtr
#include "entity/entity.h"                    //    for pchar
#include "entity/character_base.h"            //    for pclass
#include "UI/uiAvatar.h"                //    AVATAR genders
#include "entity/entPlayer.h"                //    for pl
#include "entity/powers.h"
#include "uiPCCCreationNLM.h"        //    for pcc creation menu
#include "entity/costume_client.h"
#include "UI/uiCostume.h"

extern int PCCselectedPowerSets[3];
extern int PCCselectedPowers[2];
extern SMFBlock *critterDescEdit;
extern SMFBlock *critterNameEdit;
extern SMFBlock *critterGroupEdit;
extern SMFBlock *critterSaveNameEdit;
extern int gPCCProfileInit;
static int mouseOver;
int pccCritterRank = 0;
extern int pccTimeAge;
int pccCritterIsRanged = 0;
int creationEditing = 0;
extern int pccCritterDifficulty[2];
int pccCritterFirstGenderPass = 0;
Costume *pccFixupCostume = NULL;
static int pccFixup = 0;
extern int pcc_powerSelection;
extern int gCurrentGender;
MMElementList *pcc_prevCritterList;

static int pccCritterCanFly = 0;
static TextAttribs s_TextAttr_PCC_rank =
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
#define PCCSET_WIDTH 240
#define PCC_RANK_X 95
#define PCC_DIFFICULTY_X ( PCCSET_WIDTH + 60 + PCC_RANK_X )
#define PCC_FLIGHT_X ( PCCSET_WIDTH + PCC_DIFFICULTY_X + 60 )
#define PCC_FIGHT_X PCC_DIFFICULTY_X
#define PCC_RANK_SPACING 45
#define PCC_RANK_START_Y 140
#define PCC_RANK_HEIGHT 560
#define PCC_RANK_RANK_HEIGHT ((PCC_RANK_COUNT*PCC_RANK_SPACING)+30)
#define PCC_RANK_AI_START_Y (PCC_RANK_RANK_HEIGHT+PCC_RANK_START_Y+20)
#define PCC_RANK_AI_HEIGHT ( (2 * PCC_RANK_SPACING ) + 20 )
#define PCC_RANK_FLIGHT_START_Y PCC_RANK_START_Y/*( PCC_RANK_START_Y + PCC_RANK_AI_HEIGHT+35 )*/
#define PCC_RANK_FLIGHT_HEIGHT ( ( 3 * PCC_RANK_SPACING ) + 20)
void editPCC_setup(MMElementList *pList, int editingMode, PCC_Critter *pcc, int flags, char *overrideName)
{
    pcc_prevCritterList = pList;
    if (editingMode == PCC_EDITING_MODE_EDIT)
    {
        char *saveName = estrTemp();
        pccFixup = 1;
        pccCritterRank = pcc->rankIdx;
        pccCritterDifficulty[0] = pcc->difficulty[0];
        pccCritterDifficulty[1] = pcc->difficulty[1];
        PCCselectedPowers[0] = pcc->selectedPowers[0];
        PCCselectedPowers[1] = pcc->selectedPowers[1];
        pccCritterIsRanged = pcc->isRanged;
        pccFixupCostume = pcc->pccCostume;
        pccTimeAge = pcc->timeAge;
        pccCritterFirstGenderPass = 1;
        switch (pccFixupCostume->appearance.bodytype)
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
        PCCselectedPowerSets[0] = getPCCPowerIndexFromPowersetName(pcc->primaryPower, 0);
        PCCselectedPowerSets[1] = getPCCPowerIndexFromPowersetName(pcc->secondaryPower, 1);
        PCCselectedPowerSets[2] = getPCCPowerIndexFromPowersetName(pcc->travelPower, 2);
        if (PCCselectedPowerSets[2] == getPCCPowerIndexFromPowersetName("Mission_Maker_Movement.Flight",2) )
        {
            pccCritterCanFly = 1;
        }
        else if (PCCselectedPowerSets[2] == getPCCPowerIndexFromPowersetName("Mission_Maker_Movement.Reflections_Effects", 2) )
        {
            pccCritterCanFly = 2;
        }
        else
        {
            pccCritterCanFly = 0;
            PCCselectedPowerSets[2] = -1;
        }
        createPCCProfileTextBoxes();
        smf_SetRawText(critterNameEdit, pcc->name, false);                //    names should be valid by validations
        smf_SetRawText(critterGroupEdit, pcc->villainGroup, false);        //    villain groups should be valid by validation
        if (pcc->description)
            smf_SetRawText(critterDescEdit, pcc->description, false);        //    villain groups should be valid by validation

    
        estrConcatCharString(&saveName, pcc->sourceFile);
        if ( saveName)
        {
            saveName = MAX(strrchr(saveName, '/' ), strrchr(saveName, '\\'));    //    find the last slash
            if (saveName)
            {
                saveName++;
                if (saveName)
                {
                    char *killExtension;
                    killExtension = strchr(saveName, '.');
                    //    Work in progress. what to do if the file was not a critter file (i.e. a storyArc)
                    //    right now, kill the name, and make the player enter a new one.
                    if (!killExtension || stricmp(killExtension, ".critter") != 0)
                    {
                        estrPrintCharString(&saveName, pcc->name);
                        pccFixup = 0;
                    }
                    else
                    {
                        *killExtension = 0;
                    }
                }
                else 
                {    
                    estrPrintCharString(&saveName, pcc->name);    
                }
            }
            else 
            {    
                estrPrintCharString(&saveName, pcc->name);    
            }
        }
        else 
        {    
            estrPrintCharString(&saveName, pcc->name);    
        }
        //    save name will either be a valid filename or just the critter name is some kind of corruption happens
        smf_SetRawText(critterSaveNameEdit, saveName, 0);                    
        estrDestroy(&saveName);
    }
    else
    {
        if (flags & PCC_SETUP_FLAGS_ISCONTACT)
            pccCritterRank = PCC_RANK_CONTACT;
        else if (flags & PCC_SETUP_FLAGS_SEED_MINION)
            pccCritterRank = PCC_RANK_MINION;
        else if (flags & PCC_SETUP_FLAGS_SEED_LT)
            pccCritterRank = PCC_RANK_LT;
        else
            pccCritterRank = PCC_RANK_BOSS;

        pccCritterDifficulty[0] = pccCritterDifficulty[1] = PCC_DIFF_STANDARD;
        PCCselectedPowers[0] = PCCselectedPowers[1] = 0;

        if (overrideName)
        {
            if (!critterNameEdit)
            {
                createPCCProfileTextBoxes();
            }
            smf_SetRawText(critterNameEdit, overrideName, 0);

            //    to make sure that critterNameEdit->outputString is correct, call smf_ParseAndFormat
            //    critterSaveNameEdit will be formatted later
            smf_ParseAndFormat(critterNameEdit, critterNameEdit->rawString, 0,0,0,1,1,0,0,0,0,0);

            smf_SetRawText(critterSaveNameEdit, overrideName, 0);
        }
        NLM_incrementTransactionID(NLM_PCCCreationMenu());
        pccCritterIsRanged = 0;
        pccCritterCanFly = 0;
        pccTimeAge = 0;
        PCCselectedPowerSets[0] = PCCselectedPowerSets[1] = PCCselectedPowerSets[2] = -1;
        resetCostumeMenu();
    }
    start_menu(MENU_PCC_RANK);
}
static void drawPCCRankFrame(float leftX, float leftY, float width, float height, float z, int color, int back_color, float screenScaleX, float screenScaleY)
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
    width *= screenScaleX;
    leftY *= screenScaleY;
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
    }
    drawFlatFrame(PIX3, R10, leftX, leftY, 5, width, height, 1.0, 0x00000000, 0x00000088);

}
static char *getDifficultySelectionDescriptionText( int difficulty)
{
    switch (difficulty)
    {
    case PCC_DIFF_STANDARD:
        return textStd("PCCStandardDescription");
        break;
    case PCC_DIFF_HARD:
        return textStd("PCCHardDescription");
        break;
    case PCC_DIFF_EXTREME:
        return textStd("PCCExtremeDescription");
        break;
    case PCC_DIFF_CUSTOM:
        return textStd("PCCCustomDescription");
        break;
    default:
        return ("BadSelection");
    }
}
static char *getRankSelectionDescriptionText(int rank)
{
    switch (rank)
    {
    case PCC_RANK_MINION:
        return textStd("PCCMinionDescription");
        break;
    case PCC_RANK_LT:
        return textStd("PCCLtDescription");
        break;
    case PCC_RANK_BOSS:
        return textStd("PCCBossDescription");
        break;
    case PCC_RANK_ELITE_BOSS:
        return textStd("PCCEliteBossDescription");
        break;
    case PCC_RANK_ARCH_VILLAIN:
        return textStd("PCCArchVillaiDescription");
        break;
    case PCC_RANK_CONTACT:
        return textStd("PCCContactDescription");
        break;
    default:
        return ("BadSelection");
    }
}
static void drawPCC_RankSelection(int leftX, int y, float screenScaleX, float screenScaleY)
{
    int mouse,i;
    float screenScale = MIN(screenScaleX, screenScaleY);
    font( &gamebold_12 );
    font_color( CLR_WHITE, SELECT_FROM_UISKIN( 0x88b6ffff, 0xaaaaaaff, 0x8888ccff ) );
    cprnt((leftX+(PCCSET_WIDTH/2))*screenScaleX, (y+7.5-PIX3)*screenScaleY,60, screenScale, screenScale, "PCCRankSelection" );

    for (i = 0; i < PCC_RANK_COUNT; i++)
    {
        mouse = drawCheckBar(PCC_RANK_X*screenScaleX, (y*screenScaleY)+((PCC_RANK_SPACING+(i*PCC_RANK_SPACING))*screenScale), 60,screenScale,PCCSET_WIDTH*screenScaleX, getPCCRankSelectionTitleText(i), pccCritterRank==i, 1,0,0, NULL);
        if (mouse)
        {
            mouseOver = i;
            if (mouse == D_MOUSEHIT)
            {
                if (i != pccCritterRank)
                {
                    sndPlay("Select", SOUND_GAME);
                    pccCritterRank = i;
                    if (PCCselectedPowers[0] > 0)    PCCselectedPowers[0] *= -1;
                    if (PCCselectedPowers[1] > 0)    PCCselectedPowers[1] *= -1;
                }
            }
        }
    }
}

static void drawPCC_FightingPreference(int leftX, int y, float screenScaleX, float screenScaleY)
{
    int mouse;
    float screenScale = MIN(screenScaleX, screenScaleY);
    font( &gamebold_12 );
    font_color( CLR_WHITE, SELECT_FROM_UISKIN( 0x88b6ffff, 0xaaaaaaff, 0x8888ccff ) );
    cprnt((leftX+(PCCSET_WIDTH/2))*screenScaleX,(y + 7.5-PIX3)*screenScaleY,60, screenScale, screenScale, "PCCFightingPreference" );

    mouse = drawCheckBar(PCC_FIGHT_X*screenScaleX, (y*screenScaleY)+(PCC_RANK_SPACING*screenScale), 60, screenScale, PCCSET_WIDTH*screenScaleX, textStd("PCCMelee"), !pccCritterIsRanged && (pccCritterRank!=PCC_RANK_CONTACT), (pccCritterRank!=PCC_RANK_CONTACT), 0, 0, NULL);
    if (mouse)
    {
        mouseOver = PCC_RANK_COUNT + 1;
        if (mouse == D_MOUSEHIT)
        {
            if (pccCritterIsRanged)
            {
                sndPlay("Select", SOUND_GAME);
            }
            pccCritterIsRanged = 0;
        }
    }
    mouse = drawCheckBar(PCC_FIGHT_X*screenScaleX, (y*screenScaleY)+(2*PCC_RANK_SPACING*screenScale), 60, screenScale, PCCSET_WIDTH*screenScaleX, textStd("PCCRanged"), pccCritterIsRanged && (pccCritterRank!=PCC_RANK_CONTACT), (pccCritterRank!=PCC_RANK_CONTACT), 0, 0, NULL);
    if (mouse)
    {
        mouseOver = PCC_RANK_COUNT + 2;
        if (mouse == D_MOUSEHIT)
        {
            if (!pccCritterIsRanged)
            {
                sndPlay("Select", SOUND_GAME);
            }
            pccCritterIsRanged = 1;
        }
    }

}
static void drawFlightPower(int leftX, int y, float screenScaleX, float screenScaleY)
{
    int mouse;
    static const BasePowerSet *flightBset = NULL;
    static const BasePowerSet *reflectBset = NULL;
    float screenScale = MIN(screenScaleX, screenScaleY);
    if (!flightBset)
    {
        flightBset = powerdict_GetBasePowerSetByFullName(&g_PowerDictionary, "Mission_Maker_Movement.Flight");
    }
    if (!reflectBset)
    {
        reflectBset = powerdict_GetBasePowerSetByFullName(&g_PowerDictionary, "Mission_Maker_Movement.Reflections_Effects");
    }

    font( &gamebold_12 );
    font_color( CLR_WHITE, SELECT_FROM_UISKIN( 0x88b6ffff, 0xaaaaaaff, 0x8888ccff ) );
    cprnt((leftX+(PCCSET_WIDTH/2))*screenScaleX, (y + 7.5-PIX3)*screenScaleY,60, screenScale, screenScale, "PCCFlightPreference" );
    mouse = drawCheckBar(PCC_FLIGHT_X*screenScaleX, (y*screenScaleY)+(PCC_RANK_SPACING*screenScale), 60, screenScale, PCCSET_WIDTH*screenScaleX, textStd("NoneString"), ((pccCritterCanFly == 0) && (pccCritterRank!=PCC_RANK_CONTACT)), (pccCritterRank!=PCC_RANK_CONTACT), 0, 0, NULL);
    if (mouse)
    {
        mouseOver = PCC_RANK_COUNT + 3;
        if (mouse == D_MOUSEHIT)
        {
            sndPlay("Select", SOUND_GAME);
            pccCritterCanFly = 0;
        }
    }
    mouse = drawCheckBar(PCC_FLIGHT_X*screenScaleX, (y*screenScaleY)+(2*PCC_RANK_SPACING*screenScale), 60, screenScale, PCCSET_WIDTH*screenScaleX, textStd(flightBset->pchDisplayName), ((pccCritterCanFly == 1) && (pccCritterRank!=PCC_RANK_CONTACT)), (pccCritterRank!=PCC_RANK_CONTACT), 0, 0, NULL);
    if (mouse)
    {
        mouseOver = PCC_RANK_COUNT + 4;
        if (mouse == D_MOUSEHIT)
        {
            sndPlay("Select", SOUND_GAME);
            pccCritterCanFly = 1;
        }
    }
    mouse = drawCheckBar(PCC_FLIGHT_X*screenScaleX, (y*screenScaleY)+(3*PCC_RANK_SPACING*screenScale), 60, screenScale, PCCSET_WIDTH*screenScaleX, textStd(reflectBset->pchDisplayName), ((pccCritterCanFly == 2) && (pccCritterRank!=PCC_RANK_CONTACT)), (pccCritterRank!=PCC_RANK_CONTACT), 0, 0, NULL);
    if (mouse)
    {
        mouseOver = PCC_RANK_COUNT + 5;
        if (mouse == D_MOUSEHIT)
        {
            sndPlay("Select", SOUND_GAME);
            pccCritterCanFly = 2;
        }
    }
}
static void drawPCCRankSelectionPanel(float screenScaleX, float screenScaleY)
{
    drawPCC_RankSelection(PCC_RANK_X, PCC_RANK_START_Y, screenScaleX, screenScaleY);
    drawPCC_FightingPreference(PCC_DIFFICULTY_X, PCC_RANK_START_Y, screenScaleX, screenScaleY);
    drawFlightPower(PCC_FLIGHT_X, PCC_RANK_FLIGHT_START_Y, screenScaleX, screenScaleY);
}
static void drawPCCRankTextPanel(float x, float y, float z, float width, float height, float screenScaleX, float screenScaleY)
{
    static SMFBlock sm;
    UIBox box;
    float screenScale = MIN(screenScaleX, screenScaleY);
    font( &gamebold_12 );
    font_color( CLR_WHITE, SELECT_FROM_UISKIN( 0x88b6ffff, 0xaaaaaaff, 0x8888ccff ) );
    cprnt((x+(width/2))*screenScaleX, (PCC_RANK_START_Y + PCC_RANK_RANK_HEIGHT  + 50+7.5+PIX3)*screenScaleY,60, screenScale, screenScale, "PCCDetailedInfo" );
    uiBoxDefine(&box, x*screenScaleX, y*screenScaleY, width*screenScaleX, height*screenScaleY);
    clipperPush(&box);
    //    Mouse is over a rank
    font_color( uiColors.standard.text, uiColors.standard.text2 );
    s_TextAttr_PCC_rank.piScale = (int *)(int)(1.0f*SMF_FONT_SCALE*screenScale);
    if (mouseOver < PCC_RANK_COUNT)
    {
        cprntEx(x*screenScaleX,(y+13)*screenScaleY,z,screenScale, screenScale, 1<<1 /* CENTER_Y */,  getPCCRankSelectionTitleText(mouseOver));
        smf_SetRawText(&sm, getRankSelectionDescriptionText(mouseOver), 0 );
        smf_Display(&sm, (x+15)*screenScaleX, (y+18)*screenScaleY, z, (width - 15)*screenScaleX, (height-15)*screenScaleY, 0,0, &s_TextAttr_PCC_rank, NULL);
    }
    else
    {
        switch (mouseOver)
        {
        case PCC_RANK_COUNT + 1:
            {
                //    Melee
                cprntEx(x*screenScaleX,(y+13)*screenScaleY,z,screenScale, screenScale, 1<<1 /* CENTER_Y */,  textStd("PCCMelee"));
                smf_SetRawText(&sm, textStd("PCCMeleeDescription"), 0 );
                smf_Display(&sm, (x+15)*screenScaleX, (y+18)*screenScaleY, z, (width - 15)*screenScaleX, (height-15)*screenScaleY, 0,0, &s_TextAttr_PCC_rank, NULL);
                break;
            }
        case PCC_RANK_COUNT + 2:
            {
            //    Range
                cprntEx(x*screenScaleX,(y+13)*screenScaleY,z,screenScale, screenScale, 1<<1 /* CENTER_Y */,  textStd("PCCRanged"));
                smf_SetRawText(&sm,  textStd("PCCRangedDescription"), 0 );
                smf_Display(&sm, (x+15)*screenScaleX, (y+18)*screenScaleY, z, (width - 15)*screenScaleX, (height-15)*screenScaleY, 0,0, &s_TextAttr_PCC_rank, NULL);
                break;
            }
        case PCC_RANK_COUNT + 3:
            {
                cprntEx(x*screenScaleX,(y+13)*screenScaleY,z,screenScale, screenScale, 1<<1 /* CENTER_Y */,  textStd("NoneString"));
                smf_SetRawText(&sm, textStd("PCCCritterNoFlyText"), 0 );
                smf_Display(&sm, (x+15)*screenScaleX, (y+18)*screenScaleY, z, (width - 15)*screenScaleX, (height-15)*screenScaleY, 0,0, &s_TextAttr_PCC_rank, NULL);
                break;
            }
        case PCC_RANK_COUNT + 4:
            {
                //    flight
                static const BasePowerSet *bset = NULL;
                if (!bset)
                {
                    bset = powerdict_GetBasePowerSetByFullName(&g_PowerDictionary, "Mission_Maker_Movement.Flight");
                }
                cprntEx(x*screenScaleX,(y+13)*screenScaleY,z,screenScale, screenScale, 1<<1 /* CENTER_Y */,  textStd(bset->pchDisplayName));
                smf_SetRawText(&sm,textStd(bset->pchDisplayHelp), 0 );
                smf_Display(&sm, (x+15)*screenScaleX, (y+18)*screenScaleY, z, (width - 15)*screenScaleX, (height-15)*screenScaleY, 0,0, &s_TextAttr_PCC_rank, NULL);
                break;
            }
        case PCC_RANK_COUNT + 5:
            {
                //    flight
                static const BasePowerSet *rset = NULL;
                if (!rset)
                {
                    rset = powerdict_GetBasePowerSetByFullName(&g_PowerDictionary, "Mission_Maker_Movement.Reflections_Effects");
                }
                cprntEx(x*screenScaleX,(y+13)*screenScaleY,z,screenScale, screenScale, 1<<1 /* CENTER_Y */,  textStd(rset->pchDisplayName));
                smf_SetRawText(&sm, textStd(rset->pchDisplayHelp), 0 );
                smf_Display(&sm, (x+15)*screenScaleX, (y+18)*screenScaleY, z, (width - 15)*screenScaleX, (height-15)*screenScaleY, 0,0, &s_TextAttr_PCC_rank, NULL);
                break;
            }
        }
    }
        
    clipperPop();
}
static void drawPCCRankHelp(float x, float y, float z, float width, float height, float screenScaleX, float screenScaleY)
{
    static SMFBlock sm;
    static int oneTimeOnly = 1;
    float screenScale = MIN(screenScaleX, screenScaleY);
    if (oneTimeOnly)
    {
        smf_SetFlags(&sm, SMFEditMode_ReadOnly, 0, SMFInputMode_None, 1010, SMFScrollMode_InternalScrolling, 
            0, SMFDisplayMode_AllCharacters, SMFContextMenuMode_None, SMAlignment_Left, 0, 0, -1);
        smf_SetRawText(&sm, textStd("PCCRankHelpText"), 0);
        oneTimeOnly = 0;
    }
    smf_SetScissorsBox(&sm, x*screenScaleX, (y+PIX3+2)*screenScaleY, width*screenScaleX, (height-(2*(PIX3+2)))*screenScaleY);
    gTextAttr_Chat.piScale = (int *)(int)(1.0f*SMF_FONT_SCALE*screenScale);
    smf_Display(&sm, (x+15)*screenScaleX, (y+PIX3+2) *screenScaleY, z, (width - 15)*screenScaleX, (height-(2*(PIX3+2)))*screenScaleY, 0, 0, &gTextAttr_Chat, NULL);
}

static void PCCRankMenuExitCode(void)
{
    Entity *e = playerPtr();
    if (!e)
        return;
    if (!e->pchar)
        return;
    if (!e->pl)
        return;
    switch (pccCritterRank)
    {
    case PCC_RANK_MINION:
        e->pchar->pclass = classes_GetPtrFromName(&g_VillainClasses, "Class_Minion_Grunt");
        break;
    case PCC_RANK_LT:
        e->pchar->pclass = classes_GetPtrFromName(&g_VillainClasses, "Class_Lt_Grunt");
        break;
    case PCC_RANK_BOSS:
        e->pchar->pclass = classes_GetPtrFromName(&g_VillainClasses, "Class_Boss_Grunt");
        break;
    case PCC_RANK_ELITE_BOSS:
        e->pchar->pclass = classes_GetPtrFromName(&g_VillainClasses, "Class_Boss_Elite");
        break;
    case PCC_RANK_ARCH_VILLAIN:
        e->pchar->pclass = classes_GetPtrFromName(&g_VillainClasses, "Class_Boss_Archvillain");
        break;
    case PCC_RANK_CONTACT:
        e->pchar->pclass = classes_GetPtrFromName(&g_VillainClasses, "Class_Boss_Grunt");
        break;
    default:
        //    ERROR
        break;
    }    
    pcc_powerSelection = 0;
    switch (pccCritterCanFly)
    {
    case 1:
        PCCselectedPowerSets[2] = getPCCPowerIndexFromPowersetName("Mission_Maker_Movement.Flight", 2);
        break;
    case 2:
        PCCselectedPowerSets[2] = getPCCPowerIndexFromPowersetName("Mission_Maker_Movement.Reflections_Effects", 2);
        break;
    default:
        PCCselectedPowerSets[2] = -1;
        break;

    }
    e->pl->costume[0] = costume_as_mutable_cast(e->costume);

    if( gCurrentGender == AVATAR_GS_FEMALE || gCurrentGender == AVATAR_GS_BF )
        setGender( e, "fem" );
    else if ( gCurrentGender == AVATAR_GS_HUGE || gCurrentGender == AVATAR_GS_ENEMY || gCurrentGender == AVATAR_GS_ENEMY2 )
        setGender( e, "huge" );
    else
        setGender( e, "male" );
}

NonLinearMenuElement PCC_Rank_NLME = { MENU_PCC_RANK, { "RankString", 0, 0 },
                                                0, 0, 0, 0, PCCRankMenuExitCode};

void PCCRankMenu(void)
{
    Entity *e = NULL;
    AtlasTex *layover = NULL;
    int largestColumnHeight;
    int screenScalingDisabled = is_scrn_scaling_disabled();
    float screenScaleX = 1.f, screenScaleY = 1.f, screenScale;

    calc_scrn_scaling(&screenScaleX, &screenScaleY);
    screenScale = MIN(screenScaleX, screenScaleY);

    if (getPCCEditingMode() < 0)
        setPCCEditingMode(getPCCEditingMode()*-1);
    //    disable the avatar
    toggle_3d_game_modes(SHOW_NONE);

    //    draw background
    drawBackground( layover );

    set_scrn_scaling_disabled(1);
    //    draw background frame
    drawPCCRankHelp(60,60, 30, 890, 50, screenScaleX, screenScaleY);
    drawPCCRankFrame(60, 60,900, 50, 20, CLR_WHITE, 0, screenScaleX, screenScaleY );
    largestColumnHeight = PCC_RANK_RANK_HEIGHT;
    largestColumnHeight = MAX(largestColumnHeight, PCC_RANK_FLIGHT_HEIGHT + PCC_RANK_AI_HEIGHT + 50);
    drawPCCRankFrame(PCC_RANK_X -35, PCC_RANK_START_Y, PCCSET_WIDTH + 50, largestColumnHeight, 20, CLR_WHITE,0, screenScaleX, screenScaleY );
    drawPCCRankFrame(PCC_FIGHT_X-35, PCC_RANK_START_Y, PCCSET_WIDTH + 50, largestColumnHeight,  20,CLR_WHITE ,0, screenScaleX, screenScaleY );
    drawPCCRankFrame(PCC_FLIGHT_X-35, PCC_RANK_FLIGHT_START_Y, PCCSET_WIDTH + 60, largestColumnHeight,  20,CLR_WHITE ,0, screenScaleX, screenScaleY );
    drawPCCRankFrame(60, PCC_RANK_START_Y + largestColumnHeight  + 40 ,900, PCC_RANK_HEIGHT - (largestColumnHeight + 40),  20,CLR_WHITE ,0, screenScaleX, screenScaleY );

    if (getPCCEditingMode() == PCC_EDITING_MODE_NONE)
    {
        Entity *player = playerPtr();
        costumeUnawardPowersetParts(player, 1);

        if (!pccFixup)
        {
            setPCCEditingMode( PCC_EDITING_MODE_CREATION );
        }
        else
        {
            setPCCEditingMode( PCC_EDITING_MODE_EDIT );
        }
        pccFixup = 0;

        //    If the critter doesn't exist, we will have to allocate space for one
        //    has to be after the mode set
        createPCC_Entity();
    }

    //    This is an ugly hack imo, but the player ptr has this information because many costume things use the player ptr
    e = playerPtr();

    //    draw left panel
    drawPCCRankSelectionPanel(screenScaleX, screenScaleY);

    //    draw right panel
    drawPCCRankTextPanel(80, PCC_RANK_START_Y + PCC_RANK_RANK_HEIGHT  + 70,30, 900 - 25, PCC_RANK_HEIGHT - (largestColumnHeight + 50+40), screenScaleX, screenScaleY);

    //    draw back button
    if (drawNextButton(40*screenScaleX, 740*screenScaleY, 10, screenScale, 0,0,0, "BackString"))
    {
        Entity *player;
        sndPlay("back", SOUND_GAME);
        pccCritterRank = PCC_RANK_MINION;
        pccCritterDifficulty[0] = PCC_DIFF_STANDARD;
        pccCritterDifficulty[1] = PCC_DIFF_STANDARD;
        pccCritterIsRanged = 0;        
        destroyPCC_Entity(e);
        setPCCEditingMode( PCC_EDITING_MODE_NONE );
        player = playerPtr();
        costumeAwardPowersetParts(player, 0, 1);
        smfBlock_Destroy(critterNameEdit);
        smfBlock_Destroy(critterGroupEdit);
        smfBlock_Destroy(critterDescEdit);
        smfBlock_Destroy(critterSaveNameEdit);
        critterGroupEdit = 0;
        critterNameEdit = 0;
        critterDescEdit = 0;
        critterSaveNameEdit = 0;
        gPCCProfileInit = 0;
        pccFixupCostume = NULL;
        start_menu( MENU_GAME );
    }

    //    draw next button
    if (drawNextButton(980*screenScaleX, 740*screenScaleY, 10, screenScale, 1, 0, 0, "NextString"))
    {
        sndPlay("next", SOUND_GAME);
        PCCRankMenuExitCode();
        start_menu( (pccCritterRank == PCC_RANK_CONTACT) ? MENU_GENDER :MENU_PCC_POWERS );
    }

    set_scrn_scaling_disabled(0);
    if (drawNonLinearMenu( NLM_PCCCreationMenu(), 20, 10, 20, 1000, 20, 1.0f, PCC_Rank_NLME.listId, PCC_Rank_NLME.listId, 0 ))
    {
        //    if anything needs to be done
    }

    if (getPCCEditingMode() > 0)
        setPCCEditingMode(getPCCEditingMode()*-1);

    set_scrn_scaling_disabled(screenScalingDisabled);
}