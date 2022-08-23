/***************************************************************************
*     Copyright (c) NCSoft
*     All Rights Reserved
*     Confidential Property of NCSoft Studios
***************************************************************************/
#include <string.h>
#include "graphics/truetype/ttFontCore.h"
#include <utilitieslib/utils/StringUtil.h>
#include <utilitieslib/utils/file.h>        //for fopen

#include "filter/profanity.h"
#include "UI/uiGame.h"   // for start_menu
#include "player/player.h"
#include "graphics/FX/fx.h"
#include "entity/costume_client.h"   // for setGender
#include "UI/sprite/sprite_base.h"
#include "UI/sprite/sprite_font.h"
#include "UI/sprite/sprite_text.h"
#include "UI/uiUtilMenu.h"
#include "UI/uiAvatar.h"
#include "UI/uiUtil.h"
#include "UI/uiInput.h"
#include "graphics/seqgraphics.h"
#include "graphics/ttFont.h"
#include "entity/entPlayer.h"
#include "entity/powers.h"
#include "entity/origins.h"
#include "filter/titles.h"
#include "UI/uiUtilGame.h"
#include "entity/character_base.h"
#include "entity/character_level.h"
#include "cmdparse/cmdgame.h"    // for editnpc
#include "cmdparse/cmdcommon.h"  // for timestep
#include "entity/entclient.h"
#include <utilitieslib/components/earray.h>
#include "win/input.h"
#include "UI/uiDialog.h"
#include "graphics/font.h"
#include "sound/sound.h"
#include "UI/uiNet.h"
#include "uiEmote.h"
#include "UI/uiReticle.h"
#include "entity/entVarUpdate.h"
#include "entity/classes.h"
#include "gameComm/initClient.h"
#include "language/langClientUtil.h"
#include "filter/validate_name.h"
#include "UI/uiFocus.h"
#include <utilitieslib/utils/utils.h>
#include "UI/uiComboBox.h"
#include <utilitieslib/components/EString.h>
#include "formatter/smf_main.h"
#include "graphics/ttFontUtil.h"
#include "graphics/textureatlas.h"
#include "gameData/costume_data.h"
#include "gameComm/trayCommon.h"
#include "seq/seqstate.h"
#include "entity/entity.h"
#include <utilitieslib/utils/utils.h>
#include "UI/uiCostume.h"
#include "entity/Supergroup.h"
#include "UI/uiPowerInventory.h"
#include <utilitieslib/language/AppLocale.h>
#include <utilitieslib/language/MessageStoreUtil.h>
#include "UI/uiChat.h"
#include "uiPCCProfile.h"
#include "entity/PCC_Critter.h"
#include "entity/PCC_Critter_Client.h"
#include "UI/uiMissionMakerScrollSet.h"
#include <utilitieslib/utils/sysutil.h>
#include "UI/uiMissionMaker.h"
#include "UI/uiCustomVillainGroupWindow.h"
#include "UI/uiUtil.h"
#include "entity/CustomVillainGroup.h"
#include "uiPCCCreationNLM.h"

#define SAVE_NAME_COLOR (int *)0x000000ff
#define SAVE_NAME_HOVER (int *)0x000000ff
#define SAVE_PATH_COLOR (int *)0x696969ff
#define SAVE_PATH_HOVER (int *)0xbbbbbbff

static TextAttribs s_CritterProfileScreenText =
{
    /* piBold        */  (int *)0,
    /* piItalic      */  (int *)0,
    /* piColor       */  SAVE_NAME_COLOR,
    /* piColor2      */  (int *)0,
    /* piColorHover  */  SAVE_NAME_HOVER,
    /* piColorSelect */  (int *)0xbbbbbbff,
    /* piColorSelectBG*/ (int *)0x333333ff,
    /* piScale       */  (int *)(int)(1.f*SMF_FONT_SCALE),
    /* piFace        */  (int *)&profileFont,
    /* piFont        */  (int *)0,
    /* piAnchor      */  (int *)0,
    /* piLink        */  (int *)0,
    /* piLinkBG      */  (int *)0,
    /* piLinkHover   */  (int *)0,
    /* piLinkHoverBG */  (int *)0,
    /* piLinkSelect  */  (int *)0,
    /* piLinkSelectBG*/  (int *)0x666666ff,
    /* piOutline     */  (int *)0,
    /* piShadow      */  (int *)0,
};

static int isNameSaveNameLinked = 1;
static int init = 0;
static int waitingForDialogClose = 0;
typedef enum
{    
    PROFILE_NAME,           // 10 digit short name
    PROFILE_VILLAINGROUP,    // Villain Group
    PROFILE_DESCRIPTION,    // long ass description
    PROFILE_SAVENAME,        // save name for enemy character
    PROFILE_SAVEPATH,        // save path for enemy character
    PROFILE_TOTAL_BLOCKS,
} ProfileFields;

extern CustomVG **g_CustomVillainGroups;
extern int pccCritterIsRanged;
extern int pccCritterRank;
extern int pccCritterDifficulty[2];
extern int PCCselectedPowers[2];
int pccTimeAge = 0;
extern MMElementList *pcc_prevCritterList;
extern MMScrollSet templateSet;
int gPCCProfileInit = false;
SMFBlock *critterSaveNameEdit = NULL;
SMFBlock *critterNameEdit = NULL;
SMFBlock *critterGroupEdit = NULL;
SMFBlock *critterDescEdit = NULL;
SMFBlock *critterSavePathField = NULL;
static int last_result = -1;
extern int PCCselectedPowerSets[3];

static CBox original_box[PROFILE_TOTAL_BLOCKS] = {   
    { 106, 177, 716, 217 },
    { 200, 240, 430, 270 },
    { 117, 365, 679, 622 },
    { 280, 680, 495, 705 },
    { 600, 677, 960, 705 },
};    
static char saveCritterFileName[256];
static CBox out_path_box = { 600, 675, 960, 705 };
static CBox villainGroupComboBox = { 457, 239, 677, 269 };
#define VGCB_EXPAND_HEIGHT 90
static int showSavePath = 0;
static char **critter_groups = NULL;
static void generateErrorText(int retVal)
{
    char *errorText = 0;
    estrCreate(&errorText);
    estrConcatf(&errorText, "%s ", textStd("PCCSaveFailed"));
    generatePCCErrorText_ex(retVal, &errorText);
    dialogStd( DIALOG_OK, errorText, NULL, NULL, NULL, NULL, DLGFLAG_ARCHITECT);
    estrDestroy(&errorText);
}
static void drawIdentityCard()
{
    Entity * e = playerPtr();

    AtlasTex * card      = atlasLoadTexture("Architect_identity_card_background.tga");
    AtlasTex * headshot  = seqGetHeadshot(e->costume, getSeqHeadshotUniqueID() );

    // the bid ID card
    display_sprite( card, 0, 0, 10, 1.f, 1.f, CLR_WHITE );

    // the colored backing behind the player
    display_sprite( headshot, 724, 160, 20, 199.f/headshot->width, 201.f/headshot->height, CLR_WHITE );
}
static void printStaticText(float screenScaleX, float screenScaleY)
{
    Entity *e = playerPtr();
    char tmp[2048] = {0}, tmp2[2048] = {0};
    int i = 0;
    float screenScale = MIN(screenScaleX, screenScaleY);
    font(&profileFont);
    font_color(CLR_MM_TEXT_BACK, CLR_MM_TEXT_BACK);

    prnt(780*screenScaleX, 377*screenScaleY, 20, screenScale, screenScale, textStd("PCCKnownPowers"));
    prnt( 97*screenScaleX, 260*screenScaleY, 20, screenScale, screenScale, textStd("PCCVillainGroupPrompt"));
    prnt( 97*screenScaleX, 350*screenScaleY, 20, screenScale, screenScale, textStd("PCCBossDecriptionPrompt"));
    prnt( 97*screenScaleX, 295*screenScaleY, 20, screenScale, screenScale, textStd("PCCRankSelectionPrompt"));
    if (pccCritterRank != PCC_RANK_CONTACT)
    {
        prnt( 97*screenScaleX, 323*screenScaleY, 20, screenScale, screenScale, textStd("PCCFightingPreferencePrompt") );
    }
    font_color(CLR_MM_TEXT, CLR_MM_TEXT);
    for (i = 0; i < 3; i++)
    {
        if (PCCselectedPowerSets[i] == -1)
        {
            if ((pccCritterRank == PCC_RANK_CONTACT) && (i == 0))
            {
                prnt(730*screenScaleX, (405+(i*30))*screenScaleY, 20, screenScale, screenScale, textStd("Brawl") );
            }
        }
        else
        {
            if (i < 2)
            {
                float textWidth;
                char *powerString;
                estrCreate(&powerString);
                estrConcatf(&powerString, "%s (%s)",    textStd(missionMakerPowerSet[i]->ppPowerSets[PCCselectedPowerSets[i]]->pchDisplayName), 
                    getPCCDifficultySelectionTitleText(pccCritterDifficulty[i]) );
                textWidth = str_wd(&game_12, screenScale,screenScale, powerString);
                prnt(730*screenScaleX, (405+(i*30))*screenScaleY, 20, (textWidth > (185*screenScaleX)) ? (185.0f*screenScaleX/textWidth) : screenScale, screenScale, powerString);
                estrDestroy(&powerString);
            }
            else
            {
                prnt(730*screenScaleX, (405+(i*30))*screenScaleY, 20, screenScale, screenScale, textStd(missionMakerPowerSet[i]->ppPowerSets[PCCselectedPowerSets[i]]->pchDisplayName));
            }
        }
    }

    prnt( 300*screenScaleX, 295*screenScaleY, 20, screenScale, screenScale, getPCCRankSelectionTitleText(pccCritterRank));
    if (pccCritterRank != PCC_RANK_CONTACT)
    {
        prnt( 300*screenScaleX, 323*screenScaleY, 20, screenScale, screenScale, pccCritterIsRanged ? textStd("PCCRanged") : textStd("PCCMelee"));
    }
    if (showSavePath)
    {
        font_color(0xBBBBBBFF, 0xBBBBBBFF);
        prnt( 195*screenScaleX, 697*screenScaleY, 20, screenScale, screenScale, textStd("PCCSaveNamePrompt") );
        prnt( 525*screenScaleX, 697*screenScaleY, 20, screenScale, screenScale, textStd("PCCSavePathPrompt") );
    }
}

// SMF handles its own mouse events, so this function shouldn't do so anymore.
// This function is still needed to set current_box, which determines the background color.
static int current_box = PROFILE_NAME;
static void selectTextBox(ProfileFields field)
{
    int success = 0;
    switch (field)
    {
        case PROFILE_NAME:
            success = true;//smf_SetCursor( critterNameEdit, smfBlock_GetDisplayLength(critterNameEdit) );
            break;
        case PROFILE_VILLAINGROUP:
            success = true;//smf_SetCursor( critterGroupEdit, smfBlock_GetDisplayLength(critterGroupEdit) );
            break;
        case PROFILE_DESCRIPTION:
            success = true;//smf_SetCursor( critterDescEdit, smfBlock_GetDisplayLength(critterDescEdit) );
            break;
        case PROFILE_SAVENAME:
            success = true;//smf_SetCursor( critterSaveNameEdit, smfBlock_GetDisplayLength(critterSaveNameEdit) );
            break;
        case PROFILE_SAVEPATH:
            success = true;
            break;
    }
    if (success)
    {
        current_box = field;
    }
}
#define DEFAULT_CLR_BACK 0x00000011
#define DEFAULT_CLR_BORD 0x8d8d8dff
void createPCCProfileTextBoxes()
{
    if( !gPCCProfileInit )
    {
        //    init the UI boxes for the text boxes
        isNameSaveNameLinked = 1;
        if (!critterNameEdit)
        {
            critterNameEdit = smfBlock_Create();
            smf_SetFlags(critterNameEdit, SMFEditMode_ReadWrite, SMFLineBreakMode_SingleLine, SMFInputMode_PlayerNames, MAX_PLAYER_NAME_LEN(e), SMFScrollMode_ExternalOnly, 
                SMFOutputMode_StripAllTags, SMFDisplayMode_AllCharacters, SMFContextMenuMode_None, SMAlignment_Left, 0, "CritterNameEdit", -1);
        }

        if (!critterGroupEdit)
        {
            critterGroupEdit = smfBlock_Create();
            smf_SetFlags(critterGroupEdit, SMFEditMode_ReadWrite, SMFLineBreakMode_SingleLine, SMFInputMode_PlayerNames, 32, SMFScrollMode_ExternalOnly, 
                SMFOutputMode_StripAllTags, SMFDisplayMode_AllCharacters, SMFContextMenuMode_None, SMAlignment_Left, 0, "CritterNameEdit", -1);
        }

        if (!critterDescEdit)
        {
            critterDescEdit = smfBlock_Create();
            smf_SetFlags(critterDescEdit, SMFEditMode_ReadWrite, 0, SMFInputMode_AnyTextNoTagsOrCodes, 1010, SMFScrollMode_InternalScrolling, 
                SMFOutputMode_StripFlaggedTags, SMFDisplayMode_AllCharacters, SMFContextMenuMode_CutCopyPasteFormatColor, SMAlignment_Left, 0, "CritterNameEdit", -1);
        }

        if (!critterSaveNameEdit)
        {
            critterSaveNameEdit = smfBlock_Create();
            smf_SetFlags(critterSaveNameEdit, SMFEditMode_ReadWrite, SMFLineBreakMode_SingleLine, SMFInputMode_PlayerNames, MAX_PLAYER_NAME_LEN(e), SMFScrollMode_ExternalOnly, 
                SMFOutputMode_StripAllTags, SMFDisplayMode_AllCharacters, SMFContextMenuMode_None, SMAlignment_Left, 0, "CritterNameEdit", -1);
        }

        if (!critterSavePathField)
        {
            critterSavePathField = smfBlock_Create();
            smf_SetFlags(critterSavePathField, SMFEditMode_ReadOnly, SMFLineBreakMode_SingleLine, SMFInputMode_PlayerNames, MAX_PLAYER_NAME_LEN(e), SMFScrollMode_InternalScrolling, 
                SMFOutputMode_StripAllTags, SMFDisplayMode_AllCharacters, SMFContextMenuMode_None, SMAlignment_Left, 0, "CritterNameEdit", -1);
        }
        smf_SetCursor(critterNameEdit, 0);
        selectTextBox(PROFILE_NAME); // Sets current_box
        gPCCProfileInit = TRUE;
    }
}
static int getVillainGroups(char ***groupList, char *matchGroup )
{
    int matchedIndex = 0;
    int i;
    eaPush(groupList, "NewString");        //    this gets textStd somewhere else
    for (i = 0; i < eaSize(&g_CustomVillainGroups); ++i)
    {
        if (stricmp(g_CustomVillainGroups[i]->displayName, textStd("AllCritterListText")) != 0)
        {
            eaPush(groupList, g_CustomVillainGroups[i]->displayName);
        }
        if (matchGroup && (stricmp(g_CustomVillainGroups[i]->displayName, matchGroup) == 0))
        {
            matchedIndex = eaSize(groupList) - 1;
        }
    }
    return matchedIndex;
}
void Profile_CritterComboBoxClear()
{
    eaDestroy(&critter_groups);
    critter_groups = NULL;
}
void Profile_DrawCritterGroupComboBox(SMFBlock *groupEditBlock, int locked, float screenScaleX, float screenScaleY)
{
    static ComboBox cb[1] = {0};
    static int result = 0;
    int oldResult;
    if (!critter_groups)
    {    
        char *matchVillainGroup = NULL;
        if (groupEditBlock->outputString)
        {
            matchVillainGroup = groupEditBlock->outputString;
        }
        oldResult = result = getVillainGroups( &critter_groups, matchVillainGroup);
        combobox_init( &cb[0], villainGroupComboBox.lx, villainGroupComboBox.ly, 60, 
            (villainGroupComboBox.hx - villainGroupComboBox.lx) + 2*R10 + 2*PIX3, 
            (villainGroupComboBox.hy - villainGroupComboBox.ly), VGCB_EXPAND_HEIGHT, critter_groups, 0 );
        cb[0].currChoice = result;
        cb[0].sb->architect = 1;
    }
    oldResult = result;
    result = combobox_displayRegister( &cb[0], R6, CLR_MM_TEXT_BACK_MOUSEOVER, CLR_MM_TEXT_BACK_LIT, CLR_BLACK, textStd("PCCExistingVillainGroupsPrompt"), locked, screenScaleX, screenScaleY, 0, 0 );
    if ((result != oldResult) && (result != 0))
    {
        smf_SetRawText(groupEditBlock, cb[0].strings[result], 0);
    }
}
static void handleTextBoxes(float screenScaleX, float screenScaleY)
{
    Entity *e = playerPtr();
    static float t = 0;
    float tx = 94;    
    int i;        
    CBox box[PROFILE_TOTAL_BLOCKS];
    float screenScale = MIN(screenScaleX, screenScaleY);
    t+= TIMESTEP * 0.1;

    font( &profileFont );
    font_color( CLR_MM_TEXT, CLR_MM_TEXT );

    createPCCProfileTextBoxes();
    for (i = 0; i < PROFILE_TOTAL_BLOCKS; ++i)
    {
        BuildCBox(&box[i], original_box[i].lx*screenScaleX, original_box[i].ly*screenScaleY, (original_box[i].hx-original_box[i].lx)*screenScaleX, 
            (original_box[i].hy-original_box[i].ly)*screenScaleY);
    }
    smf_SetScissorsBox(critterDescEdit, box[PROFILE_DESCRIPTION].lx, box[PROFILE_DESCRIPTION].ly, box[PROFILE_DESCRIPTION].hx - box[PROFILE_DESCRIPTION].lx,
        box[PROFILE_DESCRIPTION].hy - box[PROFILE_DESCRIPTION].ly);

    //    enter text into current box
    for ( i = 0; i < (showSavePath ? PROFILE_TOTAL_BLOCKS : PROFILE_SAVENAME); i++)
    {
        int color = CLR_MM_TEXT_BACK_MOUSEOVER, colorBack = CLR_MM_TEXT_BACK;
        if (i == PROFILE_SAVENAME)
        {
            colorBack = 0xffffff22;
        }
        if ( ( mouseCollision( &box[i] ) ) )
        {
            colorBack    = CLR_MM_TEXT_DARK;
        }
        if ( mouseDownHit( &box[i], MS_LEFT ) )
        {
            // SMF handles its own mouse events
            // This call still sets current_box
            selectTextBox(i);
            t = PI/2;
        }
        if ( i == current_box )
        {
            colorBack = CLR_MM_TEXT_DARK;
        }
        drawFlatFrame( PIX3, R6, box[i].lx-(5*screenScaleX), box[i].ly-(2*screenScaleY), 20, (box[i].hx - box[i].lx), 
            box[i].hy - box[i].ly, screenScale, color, colorBack );
    }

    //    Check if they edit the save name. If it is untouched in edit mode, overwrite away.
    //    If it does get touched, switched to creation editing mode.
    s_CritterProfileScreenText.piColor = SAVE_NAME_COLOR;
    s_CritterProfileScreenText.piColorHover = SAVE_NAME_HOVER;
    s_CritterProfileScreenText.piScale = (int *)(int)(1.f*SMF_FONT_SCALE*screenScale);
    if (showSavePath)
    {
        smf_Display(critterSaveNameEdit, box[PROFILE_SAVENAME].lx+(7*screenScaleX), box[PROFILE_SAVENAME].ly, 1010, (box[PROFILE_SAVENAME].hx-box[PROFILE_SAVENAME].lx)-(12*screenScaleX), 
            (box[PROFILE_SAVENAME].hy-box[PROFILE_SAVENAME].ly)-(10*screenScaleY), 0, 0, &s_CritterProfileScreenText, 0);
        if (getPCCEditingMode() == PCC_EDITING_MODE_EDIT)
        {
            char *newSaveNameText;
            char *saveNameText;
            strdup_alloca(saveNameText, critterSaveNameEdit->outputString);
            newSaveNameText = critterSaveNameEdit->outputString;
            if (newSaveNameText && saveNameText)
            {
                if (stricmp(saveNameText, newSaveNameText) != 0)
                {
                    setPCCEditingMode(PCC_EDITING_MODE_CREATION);
                }
            }
        }
    }
    else
    {
        smf_ParseAndFormat(critterSaveNameEdit, critterSaveNameEdit->rawString, box[PROFILE_SAVENAME].lx, box[PROFILE_SAVENAME].ly, 30, box[PROFILE_SAVENAME].hx-box[PROFILE_SAVENAME].lx, 
            box[PROFILE_SAVENAME].hy-box[PROFILE_SAVENAME].ly, true, 0, 0, &s_CritterProfileScreenText, 0);
    }
    if (isNameSaveNameLinked && (critterNameEdit->editedThisFrame || critterSaveNameEdit->editedThisFrame))
    {
        char *nameText = critterNameEdit->outputString;
        char *saveNameText = critterSaveNameEdit->outputString;
        if (nameText && saveNameText)
        {
            if (stricmp(nameText, saveNameText) != 0)
            {
                isNameSaveNameLinked = 0;
            }        
        }
    }
    smf_Display(critterNameEdit, box[PROFILE_NAME].lx+(5*screenScaleX), box[PROFILE_NAME].ly+(5*screenScaleY), 30, (box[PROFILE_NAME].hx-box[PROFILE_NAME].lx)-(10*screenScaleX), 
        (box[PROFILE_NAME].hy-box[PROFILE_NAME].ly)-(10*screenScaleY), 0, 0, &s_CritterProfileScreenText, 0);
    smf_Display(critterGroupEdit, box[PROFILE_VILLAINGROUP].lx+(5*screenScaleX), box[PROFILE_VILLAINGROUP].ly+(3*screenScaleY), 30, (box[PROFILE_VILLAINGROUP].hx-box[PROFILE_VILLAINGROUP].lx)-(10*screenScaleX), 
        (box[PROFILE_VILLAINGROUP].hy-box[PROFILE_VILLAINGROUP].ly)-10, 0, 0, &s_CritterProfileScreenText, 0);
    smf_Display(critterDescEdit, box[PROFILE_DESCRIPTION].lx+(5*screenScaleX), box[PROFILE_DESCRIPTION].ly+(5*screenScaleY), 30, (box[PROFILE_DESCRIPTION].hx-box[PROFILE_DESCRIPTION].lx)-(15*screenScaleX), 
        (box[PROFILE_DESCRIPTION].hy-box[PROFILE_DESCRIPTION].ly)-(10*screenScaleY), 0, 0, &s_CritterProfileScreenText, 0);

    s_CritterProfileScreenText.piColor = SAVE_PATH_COLOR;
    s_CritterProfileScreenText.piColorHover = SAVE_PATH_HOVER;
    if (showSavePath)
    {
        //display save path window
        smf_SetScissorsBox(critterSavePathField, box[PROFILE_SAVEPATH].lx + (2*screenScaleX), box[PROFILE_SAVEPATH].ly+(1*screenScaleY),
            (box[PROFILE_SAVEPATH].hx-box[PROFILE_SAVEPATH].lx) - (18*screenScaleX), (box[PROFILE_SAVEPATH].hy-box[PROFILE_SAVEPATH].ly) -(4*screenScaleY));

        smf_Display(critterSavePathField, box[PROFILE_SAVEPATH].lx + (2*screenScaleX), box[PROFILE_SAVEPATH].ly+(1*screenScaleY), 1010,
            (box[PROFILE_SAVEPATH].hx-box[PROFILE_SAVEPATH].lx)-(12*screenScaleX), (box[PROFILE_SAVEPATH].hy-box[PROFILE_SAVEPATH].ly)-(4*screenScaleY), 
              0, 0, &s_CritterProfileScreenText, 0);

        if(critterSaveNameEdit->displayStringLength == 0)
        {
            font_color( CLR_CONSTRUCT(128, 128, 128, (int)(120+120*sinf(t))), CLR_CONSTRUCT(128, 128, 128, (int)(120+120*sinf(t))) );
            prnt( box[PROFILE_SAVENAME].lx + (10*screenScaleX), box[PROFILE_SAVENAME].ly +(15*screenScaleY), 30, screenScale, screenScale, textStd("PCCSaveNamePrompt"));

            smf_SetRawText(critterSavePathField, "", false);
            smf_OnLostFocus(critterSavePathField, NULL);
        }
        else
        {
            char buf[MAX_PATH] = {0,};
            char *text = critterSaveNameEdit->outputString;
            sprintf(buf, "%s/%s.critter", getCustomCritterDir(), text);
            smf_SetRawText(critterSavePathField, buf, 0);
        }
    }

    if (critterNameEdit->displayStringLength == 0 )
    {
        font( &profileFont );
        font_color( 120+120*cosf(t), 120+120*cosf(t) );
        prnt(box[PROFILE_NAME].lx+(5*screenScaleX), box[PROFILE_NAME].ly+(23*screenScaleY), 30, screenScale, screenScale, "EnterNameHere");
    }
    if (critterGroupEdit->displayStringLength == 0 )
    {
        font( &profileFont );
        font_color( 120+120*cosf(t), 120+120*cosf(t) );
        prnt(box[PROFILE_VILLAINGROUP].lx+(5*screenScaleX), box[PROFILE_VILLAINGROUP].ly+(20*screenScaleY), 30, screenScale, screenScale, "EnterCritterGroupHere");
    }

    //    When in creation mode, and the name is linked, copy over the name into the save filename
    //    If in edit mode, the save filename is not linked (since it is an edit)
    if (critterNameEdit->editedThisFrame && isNameSaveNameLinked && (getPCCEditingMode() != PCC_EDITING_MODE_EDIT) )
    {
        char *text = critterNameEdit->outputString;
        if (text)
        {
            smf_SetRawText(critterSaveNameEdit, text, 0);
        }
    }
    //    If the save name remains blank, let the player know (but only if they have actually entered stuff into the name field)
    if ( (critterSaveNameEdit->displayStringLength == 0) && ( critterNameEdit->displayStringLength != 0 ) )
    {
        //showSavePath = true;
    }

    //    This must be after the smf_parseAndDisplay or else the critterGroupEdit->outputstring will be empty
    Profile_DrawCritterGroupComboBox(critterGroupEdit, 0, screenScaleX, screenScaleY);
}
static int savePCC(char *fname)
{
    int retVal = 0;
    PCC_Critter pccCritter;
    int i;
    pccCritter.name = critterNameEdit->outputString;
    pccCritter.description = critterDescEdit->outputString;
    pccCritter.villainGroup = critterGroupEdit->outputString;
    pccCritter.pccCostume = costume_as_mutable_cast(playerPtr()->costume);
    pccCritter.legacyRankText = 0;
    pccCritter.rankIdx = pccCritterRank;
    for (i = 0; i < 2; ++i)
    {
        pccCritter.difficulty[i] = pccCritterDifficulty[i];
        pccCritter.selectedPowers[i] = PCCselectedPowers[i];
    }
    pccCritter.primaryPower = (pccCritterRank == PCC_RANK_CONTACT) ?  "Brawl" : missionMakerPowerSet[0]->ppPowerSets[PCCselectedPowerSets[0]]->pchFullName;        
    pccCritter.secondaryPower = (pccCritterRank == PCC_RANK_CONTACT) ? "None" : missionMakerPowerSet[1]->ppPowerSets[PCCselectedPowerSets[1]]->pchFullName;
    if (PCCselectedPowerSets[2] == -1)
    {
        pccCritter.travelPower = "None";
    }
    else
    {
        pccCritter.travelPower = missionMakerPowerSet[2]->ppPowerSets[PCCselectedPowerSets[2]]->pchFullName;
    }
    pccCritter.isRanged = pccCritterIsRanged;
    pccCritter.sourceFile = fname;
    pccCritter.referenceFile = NULL;
    pccCritter.badCostumeParts = NULL;
    pccCritter.compressedCostume = NULL;
    retVal = isPCCCritterValid(playerPtr(), &pccCritter,0,0);
    if ( retVal == PCC_LE_VALID)
    {
        PCC_Critter *returnCritter;
        ParserWriteTextFile(fname, parse_PCC_Critter, &pccCritter,0,0 );
        pccCritter.timeAge = pccTimeAge+1;
        removeCustomCritter(fname, 0);        //    don't need to delete the source since we just overwrote it. 
        //    do need to get rid of the old reference to it.
        returnCritter = addCustomCritter(&pccCritter, playerPtr());
        if (!returnCritter)
        {
            //    spit out error
            retVal = PCC_LE_FAILED_TO_REMOVE;
        }
        else
        {
            PCC_reinsertMissing(returnCritter);
            updateCustomCritterList(&missionMaker);
            populateCVGSS();
            setSelectionToCritter(pcc_prevCritterList, returnCritter);
        }
    }
    return retVal;
}
static char overwrite_critter_save_file_name[256];
static void exitCleanup()
{
    Entity *critter;
    Entity *player;
    init = false;
    //    sanity check
    if (getPCCEditingMode() < 0)
    {
        Errorf("Critter Editing Mode should be set to critter but is set to player. Check callstack.");
        setPCCEditingMode(getPCCEditingMode()*-1);
    }
    critter = playerPtr();
    destroyPCC_Entity(critter);
    setPCCEditingMode(PCC_EDITING_MODE_NONE);
    player = playerPtr();
    costumeAwardPowersetParts(player, 0, 1);
    smfBlock_Destroy(critterNameEdit);
    smfBlock_Destroy(critterGroupEdit);
    smfBlock_Destroy(critterDescEdit);
    smfBlock_Destroy(critterSaveNameEdit);
    smfBlock_Destroy(critterSavePathField);
    critterNameEdit = 0;
    critterGroupEdit = 0;
    critterDescEdit = 0;
    critterSaveNameEdit = 0;
    critterSavePathField = 0;
    gPCCProfileInit = 0;
    dialogClearQueue(0);
    sndPlay( "playgame", SOUND_GAME);
    start_menu( MENU_GAME );
}
static void OverWriteCritterFile(void *data)
{
    int retVal;
    if (getPCCEditingMode() < 0)
        setPCCEditingMode(getPCCEditingMode() * -1);
    retVal = savePCC(overwrite_critter_save_file_name);    
    waitingForDialogClose = 0;
    if (retVal == PCC_LE_VALID)
    {        
        exitCleanup();
    }
    else
    {
        generateErrorText(retVal);
        //        dialogStd( DIALOG_OK, textStd("PCCSaveFailed"), NULL, NULL, NULL, NULL, 0);
    }
    if (getPCCEditingMode() > 0)
        setPCCEditingMode(getPCCEditingMode() * -1);
}
static void DeclineOverwrite(void *data)
{
    waitingForDialogClose = 0;
    showSavePath = 1;
}
static void savePCC_check()
{
    char *text = critterSaveNameEdit->outputString;
    if (text)
    {
        ValidateNameResult vnr = ValidateCustomObjectName(text, 50, 1);
        if (vnr != VNR_Valid)
        {
            dialogStd( DIALOG_OK, "InvalidName", NULL, NULL, NULL, NULL, DLGFLAG_ARCHITECT);
        }
        else
        {
            if (isInvalidFilename(text))
            {
                dialogStd( DIALOG_OK, "InvalidFile", NULL, NULL, NULL, NULL, DLGFLAG_ARCHITECT );
            }
            else
            {
                char fname[MAX_PATH];
                FILE * open_test = NULL;
                makeDirectories(getCustomCritterDir());
                sprintf(fname, "%s/%s.critter", getCustomCritterDir(), text);
                open_test = fopen( fname, "rb" );
                if (open_test && (getPCCEditingMode() != PCC_EDITING_MODE_EDIT))
                {
                    waitingForDialogClose = 1;
                    strcpy(overwrite_critter_save_file_name, fname);
                    dialogStd(DIALOG_YES_NO, "FileExists", NULL, NULL, OverWriteCritterFile, DeclineOverwrite, DLGFLAG_ARCHITECT);
                    fclose(open_test);
                }
                else
                {
                    int retVal;
                    retVal = savePCC(fname);
                    if (retVal == PCC_LE_VALID)
                    {                            
                        exitCleanup();
                    }
                    else
                    {
                        //    create error message based on retVal?
                        generateErrorText(retVal);
                        //                        dialogStd( DIALOG_OK, textStd("PCCSaveFailed"), NULL, NULL, NULL, NULL, 0);
                    }
                }
            }
        }
    }    
    else
    {
        dialogStd( DIALOG_OK, "InvalidName", NULL, NULL, NULL, NULL, DLGFLAG_ARCHITECT );
    }
}
static void PCCprofileMenuExitCode(void)
{
    init = false;
    dialogClearQueue(0);
    updateSeqHeadshotUniqueID();
}
extern NonLinearMenuElement PCC_powers_primary_NLME;
extern NonLinearMenuElement PCC_powers_secondary_NLME;
extern NonLinearMenuElement PCC_costume_NLME;
NonLinearMenuElement PCC_profile_NLME = { MENU_PCC_PROFILE, { "CC_ProfileString", 0, 0 },
                                                    0, 0, 0, 0, PCCprofileMenuExitCode };

void PCCProfileMenu(void)
{
    int screenScalingDisabled = is_scrn_scaling_disabled();
    float screenScaleX = 1.f, screenScaleY = 1.f, screenScale;
    int badPowerChoice = 0;
    Entity *e;
    if (getPCCEditingMode() < 0)
        setPCCEditingMode(getPCCEditingMode()*-1);

    e = playerPtr();
    calc_scrn_scaling(&screenScaleX, &screenScaleY);
    screenScale = MIN(screenScaleX, screenScaleY);

    drawBackground( NULL );
    drawIdentityCard();


    toggle_3d_game_modes(SHOW_NONE);
    //    draw menu frame

    set_scrn_scaling_disabled(1);
    //    drawMenuTitle( 10*screenScaleX, 30*screenScaleY, 1001, textStd("PCCProfileTitle"), screenScale, !init );
    if (!init)
    {
        init = true;
        Profile_CritterComboBoxClear();
        showSavePath = 0;
        if (pccCritterRank == PCC_RANK_CONTACT)
        {
            pccCritterIsRanged = 0;
            PCCselectedPowerSets[0] = PCCselectedPowerSets[1] =PCCselectedPowerSets[2] = -1;
        }
    }

    handleTextBoxes(screenScaleX, screenScaleY);

    printStaticText(screenScaleX, screenScaleY);

    if (drawMMButton(textStd(showSavePath ? "PCCHideSavePathPrompt" : "PCCShowSavePathPrompt"), 
        NULL, NULL,105*screenScaleX, 690*screenScaleY, 1001, 105*screenScaleX, 0.8f*screenScale, 0, 0  ))
    {
        showSavePath = !showSavePath;
        sndPlay("next", SOUND_GAME);
    }

    //    draw back string
    if (drawNextButton(40*screenScaleX, 740*screenScaleY, 1001, screenScale, 0, 0, 0, "BackString"))
    {
        PCCprofileMenuExitCode();
        sndPlay( "back", SOUND_GAME);
        costumeMenuEnterCode();
        start_menu( MENU_COSTUME );
    }
    //    draw save (as) button
    if (pccCritterRank == PCC_RANK_CONTACT)
    {
        if ((PCCselectedPowerSets[0] >= 0) || (PCCselectedPowerSets[1] >= 0))
            badPowerChoice = 1;
    }
    else
    {
        if ((PCCselectedPowerSets[0] < 0) || (PCCselectedPowerSets[1] < 0))
            badPowerChoice = 1;
    }
    if (    (critterNameEdit->displayStringLength == 0) || (critterSaveNameEdit->displayStringLength == 0) || 
            (critterGroupEdit->displayStringLength == 0) || waitingForDialogClose || !e->costume || badPowerChoice)
    {
        CBox box;
        if (drawNextButton(980*screenScaleX, 740*screenScaleY, 1001, screenScale, 1, 1, 0, "PCCSaveAndContinuePrompt"))
        {
            //    mouse clicks are handled below
        }
        BuildCBox(&box, 800*screenScaleX, 720*screenScaleY, 200*screenScaleX, 40*screenScaleY);
        if (mouseClickHit(&box, MS_LEFT))
        {
            NonLinearMenu *nlm = NLM_PCCCreationMenu();
            if (nlm)
            {
                if (pccCritterRank != PCC_RANK_CONTACT)
                {
                    if (PCCselectedPowerSets[0] < 0)        nlm->elementList[PCC_powers_primary_NLME.listId]->flashingTimer = 1.0f;    //    powers menu
                    else if (PCCselectedPowerSets[1] < 1)    nlm->elementList[PCC_powers_secondary_NLME.listId]->flashingTimer = 1.0f;    //    powers menu
                }
                if (!e->costume)                    nlm->elementList[PCC_costume_NLME.listId]->flashingTimer = 1.0f;    //    costume menu
            }
        }
    }
    else
    {
        if (drawNextButton(980*screenScaleX, 740*screenScaleY, 1001, screenScale, 1, 0, 0, "PCCSaveAndContinuePrompt"))
        {
            updateSeqHeadshotUniqueID();
            savePCC_check();
        }
    }

    set_scrn_scaling_disabled(0);
    if (drawNonLinearMenu( NLM_PCCCreationMenu(), 20, 10, 20, 1000, 20, 1.0f, PCC_profile_NLME.listId, PCC_profile_NLME.listId, 0 ))
    {
        //    if anything needs to be done
    }

    if (getPCCEditingMode() > 0)
        setPCCEditingMode(getPCCEditingMode()*-1);

    set_scrn_scaling_disabled(screenScalingDisabled);

}