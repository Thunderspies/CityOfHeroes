/***************************************************************************
*     Copyright (c) NCSoft
*     All Rights Reserved
*     Confidential Property of NCSoft Studios
***************************************************************************/
#include <utilitieslib/stdtypes.h>
#include "uiNPCProfile.h"
#include "UI/sprite/sprite_base.h"
#include "UI/sprite/sprite_font.h"
#include "UI/sprite/sprite_text.h"
#include "player/player.h"
#include "UI/uiUtilMenu.h"
#include "UI/uiUtilGame.h"
#include "UI/uiUtil.h"
#include "uiNPCCostume.h"
#include "game.h"
#include "formatter/smf_util.h"
#include "formatter/smf_main.h"
#include "graphics/textureatlas.h"
#include "graphics/seqgraphics.h"
#include "entity/entPlayer.h"
#include "entity/entity.h"
#include <utilitieslib/language/MessageStoreUtil.h>
#include "sound/sound.h"
#include "UI/uiAvatar.h"
#include "UI/uiGame.h"
#include "cmdparse/cmdgame.h"
#include "UI/uiInput.h"
#include <utilitieslib/components/EString.h>
#include "filter/validate_name.h"
#include "filter/profanity.h"
#include "storyarc/playerCreatedStoryarcValidate.h"
#include "UI/uiDialog.h"
#include "gameComm/VillainDef.h"
#include "uiPCCProfile.h"
#include "entity/powers.h"
#include "uiNPCCreationNLM.h"

static char **s_cvg_name = NULL;
static char **s_cvg_displayName = NULL;
static char **s_cvg_description = NULL;
static int current_box = 0;
static SMFBlock *NPCNameEditBlock = NULL;
static SMFBlock *NPCDescEditBlock = NULL;
static SMFBlock *NPCGroupEditBlock = NULL;
static SMFBlock *NPCPowerBlock = NULL;
static CBox original_box[4] = { {  106, 177, 716, 217 }, {  117, 365, 679, 622 }, { 200, 240, 430, 270 }, { 725, 475, 918, 575} };
static int waitingForDialog = 0;
static const VillainDef *s_currentVillainDef = NULL;
static int init = 0;

static TextAttribs s_CritterProfileScreenText =
{
    /* piBold        */  (int *)0,
    /* piItalic      */  (int *)0,
    /* piColor       */  (int *)0x000000ff,
    /* piColor2      */  (int *)0,
    /* piColorHover  */  (int *)0x000000ff,
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
    int i = 0;
    float screenScale = MIN(screenScaleX, screenScaleY);
    font(&profileFont);
    font_color(CLR_MM_TEXT_BACK, CLR_MM_TEXT_BACK);
    prnt( 97*screenScaleX, 260*screenScaleY, 20, screenScale, screenScale, textStd("PCCVillainGroupPrompt"));
    prnt( 97*screenScaleX, 350*screenScaleY, 20, screenScale, screenScale, textStd("PCCBossDecriptionPrompt"));

    if (s_currentVillainDef)
    {
        char *rankString = NULL;
        prnt( 97*screenScaleX, 295*screenScaleY, 20, screenScale, screenScale, textStd("PCCRankSelectionPrompt"));
        switch (s_currentVillainDef->rank)
        {
        case VR_SMALL:
            rankString = "CVGSmallString";
            break;
        case VR_MINION:
            rankString = "CVGMinionString";
            break;
        case VR_SNIPER:        //    intentional fall through
        case VR_LIEUTENANT:
            rankString = "CVGLtString";
            break;
        case VR_BOSS:
            rankString = "CVGBossString";
            break;
        case VR_PET:
            rankString = "CVGPetString";
            break;
        case VR_ARCHVILLAIN:    //    intentional fall through
        case VR_ARCHVILLAIN2:
            rankString = "CVGArchVillainString";
            break;
        default:
            rankString = "noneString";
            break;
        }
        cprnt(820*screenScaleX, 377*screenScaleY, 20, screenScale, screenScale, textStd("NPCAvailableLevels"));
        prnt(780*screenScaleX, 470*screenScaleY, 20, screenScale, screenScale, textStd("PCCKnownPowers"));

        font_color(CLR_MM_TEXT, CLR_MM_TEXT);
        prnt( 300*screenScaleX, 295*screenScaleY, 20, screenScale, screenScale, textStd(rankString));
        prnt(730*screenScaleX, 405*screenScaleY, 20, screenScale, screenScale, "%d", s_currentVillainDef->levels[0]->level);
        prnt(730*screenScaleX, 435*screenScaleY, 20, screenScale, screenScale, "%d", s_currentVillainDef->levels[eaSize(&s_currentVillainDef->levels)-1]->level);
            }
    else
    {
        prnt( 97*screenScaleX, 295*screenScaleY, 20, screenScale, screenScale, textStd("PCCRankSelectionPrompt"));
        cprnt(820*screenScaleX, 377*screenScaleY, 20, screenScale, screenScale, textStd("NPCAvailableLevels"));
        prnt(780*screenScaleX, 470*screenScaleY, 20, screenScale, screenScale, textStd("PCCKnownPowers"));

        font_color(CLR_MM_TEXT, CLR_MM_TEXT);
        prnt( 300*screenScaleX, 295*screenScaleY, 20, screenScale, screenScale, textStd("ContactString"));
        prnt(730*screenScaleX, 405*screenScaleY, 20, screenScale, screenScale, "1");
        prnt(730*screenScaleX, 435*screenScaleY, 20, screenScale, screenScale, "54");
    }

}

void setupNPCProfileTextBlocks(char **defaultName, char **defaultDesc, char **vg_name, int vg_locked)
{
    if (!NPCNameEditBlock)
    {
        NPCNameEditBlock = smfBlock_Create();
        smf_SetFlags(NPCNameEditBlock, SMFEditMode_ReadWrite, SMFLineBreakMode_SingleLine, SMFInputMode_PlayerNames, MAX_PLAYER_NAME_LEN(e), SMFScrollMode_ExternalOnly, 
            SMFOutputMode_StripAllTags, SMFDisplayMode_AllCharacters, SMFContextMenuMode_None, SMAlignment_Left, 0, "CritterNameEdit", -1);
    }
    if (!NPCDescEditBlock)
    {
        NPCDescEditBlock = smfBlock_Create();
        smf_SetFlags(NPCDescEditBlock, SMFEditMode_ReadWrite, 0, SMFInputMode_AnyTextNoTagsOrCodes, 1010, SMFScrollMode_InternalScrolling, 
            SMFOutputMode_StripFlaggedTags, SMFDisplayMode_AllCharacters, SMFContextMenuMode_CutCopyPasteFormatColor, SMAlignment_Left, 0, "CritterNameEdit", -1);
    }
    if (!NPCGroupEditBlock)
    {
        NPCGroupEditBlock = smfBlock_Create();
        smf_SetFlags(NPCGroupEditBlock, vg_locked ? SMFEditMode_Unselectable : SMFEditMode_ReadWrite, SMFLineBreakMode_SingleLine, SMFInputMode_PlayerNames, 32, SMFScrollMode_ExternalOnly, 
            SMFOutputMode_StripAllTags, SMFDisplayMode_AllCharacters, SMFContextMenuMode_None, SMAlignment_Left, 0, vg_locked ? 0 : "CritterNameEdit", -1);
    }
    if (!NPCPowerBlock)
    {
        int i;
        char *powerStr;
        NPCPowerBlock = smfBlock_Create();
        smf_SetFlags(NPCPowerBlock, SMFEditMode_Unselectable, 0, SMFInputMode_None, 1010, SMFScrollMode_InternalScrolling, 
            SMFOutputMode_StripFlaggedTags, SMFDisplayMode_AllCharacters, SMFContextMenuMode_CutCopyPasteFormatColor, SMAlignment_Left, 0, 0, -1);
        estrCreate(&powerStr);
        estrConcatStaticCharArray(&powerStr, "<color Architect>");
        if (s_currentVillainDef)
        {
            for( i = 0; i < eaSize(&s_currentVillainDef->powers); i++ )
            {
                const PowerNameRef * pRef = s_currentVillainDef->powers[i];

                if( pRef->power && pRef->power[0] == '*')
                {
                    const BasePowerSet * pSetBase = powerdict_GetBasePowerSetByName(&g_PowerDictionary, pRef->powerCategory, pRef->powerSet);

                    if( pSetBase )
                    {
                        int j;
                        for( j = 0; j < eaSize(&pSetBase->ppPowers); j++ )
                            estrConcatf(&powerStr, "%s, ", textStd(pSetBase->ppPowers[j]->pchDisplayName) );
                    }
                }
                else
                {
                    const BasePower * pPowBase = powerdict_GetBasePowerByName(&g_PowerDictionary, pRef->powerCategory, pRef->powerSet, pRef->power);
                    if(pPowBase)
                        estrConcatf(&powerStr, "%s, ", textStd(pPowBase->pchDisplayName) );
                }
            }
            if( powerStr && strlen(powerStr) )
            {
                powerStr[strlen(powerStr)-2] = '\0';
            }
        }
        else
        {
            estrConcatCharString(&powerStr, textStd("Unknown"));
        }
        estrConcatStaticCharArray(&powerStr, "</color>");
        smf_SetRawText(NPCPowerBlock, powerStr, 0);
        estrDestroy(&powerStr);
    }
    smf_SetCursor(NPCNameEditBlock, 0);
    current_box = 0;
    s_cvg_displayName = defaultName;
    s_cvg_description = defaultDesc;
    s_cvg_name = vg_name;
    if (defaultName && *defaultName)    smf_SetRawText(NPCNameEditBlock, *defaultName, 0);
    if (defaultDesc && *defaultDesc)    smf_SetRawText(NPCDescEditBlock, *defaultDesc, 0);
    if (vg_name && *vg_name)            smf_SetRawText(NPCGroupEditBlock, *vg_name, 0);
    else                                smf_SetRawText(NPCGroupEditBlock, "", 0);
}
void NPCProfileMenu_setCurrentVDef(const VillainDef *vDef)
{
    s_currentVillainDef = vDef;
}
void destroyNPCProfileTextBlocks()
{
    smfBlock_Destroy(NPCNameEditBlock);
    smfBlock_Destroy(NPCDescEditBlock);
    smfBlock_Destroy(NPCGroupEditBlock);
    smfBlock_Destroy(NPCPowerBlock);
    NPCPowerBlock = NPCGroupEditBlock = NPCNameEditBlock = NPCDescEditBlock = NULL;
}
static void handleTextBoxes(float screenScaleX, float screenScaleY)
{
    Entity *e = playerPtr();
    static float t = 0;
    int i;        
    CBox box[4];
    float screenScale = MIN(screenScaleX, screenScaleY);
    t+= TIMESTEP * 0.1;

    font( &profileFont );
    font_color( CLR_MM_TEXT, CLR_MM_TEXT );

    //    enter text into current box
    for (i = 0; i < ARRAY_SIZE(box); ++i)
    {
        BuildCBox(&box[i], original_box[i].lx *screenScaleX, original_box[i].ly*screenScaleY, (original_box[i].hx-original_box[i].lx)*screenScaleX, 
            (original_box[i].hy-original_box[i].ly)*screenScaleY);
    }
    smf_SetScissorsBox(NPCDescEditBlock, box[1].lx, box[1].ly, box[1].hx - box[1].lx,
        box[1].hy - box[1].ly);
    smf_SetScissorsBox(NPCPowerBlock, box[3].lx, box[3].ly, box[3].hx - box[3].lx,
        box[3].hy - box[3].ly);

    for (i = 0; i < ARRAY_SIZE(box)-1; ++i)
    {
        int color = CLR_MM_TEXT_BACK_MOUSEOVER, colorBack = CLR_MM_TEXT_BACK;
        if ( ( i == 2 ) && NPCGroupEditBlock->editMode == SMFEditMode_Unselectable )
        {

        }
        else
        {
            if ( ( mouseCollision( &box[i] ) ) )
            {
                colorBack    = CLR_MM_TEXT_DARK;
            }
            if ( mouseDownHit( &box[i], MS_LEFT ) )
            {
                // SMF handles its own mouse events
                // This call still sets current_box
                t = PI/2;
                current_box = i;
            }
            if ( i == current_box )
            {
                colorBack = CLR_MM_TEXT_DARK;
            }
        }
        drawFlatFrame( PIX3, R6, box[i].lx-(5*screenScaleX), box[i].ly-(2*screenScaleY), 20, box[i].hx - box[i].lx, 
            box[i].hy - box[i].ly, screenScale, color, colorBack );
    }

    s_CritterProfileScreenText.piScale = (int *)(int)(1.f*SMF_FONT_SCALE*screenScale);
    smf_Display(NPCNameEditBlock, box[0].lx+(5*screenScaleX), box[0].ly+(5*screenScaleY), 30, (box[0].hx-box[0].lx)-(10*screenScaleX), 
        (box[0].hy-box[0].ly)-(10*screenScaleY), 0, 0, &s_CritterProfileScreenText, 0);
    smf_Display(NPCDescEditBlock, box[1].lx+(5*screenScaleX), box[1].ly+(5*screenScaleY), 30, (box[1].hx-box[1].lx)-(15*screenScaleX), 
        (box[1].hy-box[1].ly)-(10*screenScaleY), 0, 0, &s_CritterProfileScreenText, 0);
    smf_Display(NPCGroupEditBlock, box[2].lx+(5*screenScaleX), box[2].ly+(3*screenScaleY), 30, (box[2].hx-box[2].lx)-(10*screenScaleX), 
        (box[2].hy-box[2].ly)-(10*screenScaleY), 0, 0, &s_CritterProfileScreenText, 0);
    smf_Display(NPCPowerBlock, box[3].lx+(5*screenScaleX), box[3].ly+(5*screenScaleY), 30, (box[3].hx-box[3].lx)-(10*screenScaleX), 
        (box[3].hy-box[3].ly)-(10*screenScaleY), 0, 0, &s_CritterProfileScreenText, 0);

    if (NPCNameEditBlock->displayStringLength == 0 )
    {
        font( &profileFont );
        font_color( 120+120*cosf(t), 120+120*cosf(t) );
        prnt(box[0].lx+(5*screenScaleX), box[0].ly+(23*screenScaleY), 30, screenScale, screenScale, "EnterNameHere");
    }
    Profile_DrawCritterGroupComboBox(NPCGroupEditBlock, NPCGroupEditBlock->editMode == SMFEditMode_Unselectable ? 1 : 0, screenScaleX, screenScaleY);
}
static void NPCsaveAndExit()
{
    updateSeqHeadshotUniqueID();
    if (*s_cvg_displayName)
        estrClear(s_cvg_displayName);
    else
        estrCreate(s_cvg_displayName);
    estrConcatCharString(s_cvg_displayName, NPCNameEditBlock->outputString);
    if (*s_cvg_description)
        estrClear(s_cvg_description);
    else
        estrCreate(s_cvg_description);
    estrConcatCharString(s_cvg_description, NPCDescEditBlock->outputString);
    if (s_cvg_name && NPCGroupEditBlock->editMode != SMFEditMode_Unselectable)
    {
        if (*s_cvg_name)
            estrClear(s_cvg_name);
        else
            estrCreate(s_cvg_name);
        estrConcatCharString(s_cvg_name, NPCGroupEditBlock->outputString);
    }
    CustomNPCEdit_exit(1);
}
static void NPCdummyClearDialog(void *data)
{
    waitingForDialog = 0;
}
static void NPCtrySave()
{
    int saveInValid = 0;
    if (NPCNameEditBlock->outputString && NPCNameEditBlock->outputString[0])
    {
        if (ValidateCustomObjectName(NPCNameEditBlock->outputString, 50, 1) != VNR_Valid)
        {
            saveInValid |= 1;
        }
    }
    if (NPCGroupEditBlock->editMode != SMFEditMode_Unselectable)
    {
        if (NPCGroupEditBlock->outputString && NPCGroupEditBlock->outputString[0] && ValidateCustomObjectName(NPCGroupEditBlock->outputString, 50, 0) != VNR_Valid)
        {
            saveInValid |= 4;
        }
    }
    if (NPCDescEditBlock->outputString && NPCDescEditBlock->outputString[0])
    {
        if (playerCreatedStoryarc_basicSMFValidate(NULL, NPCDescEditBlock->outputString, 1010) || (IsAnyWordProfane(NPCDescEditBlock->outputString)) )
        {
            saveInValid |= 2;
        }
    }
    if (!saveInValid)
    {
        init = 0;
        NPCsaveAndExit();
    }
    else
    {
        char *errorMessage;
        estrCreate(&errorMessage);
        estrConcatCharString(&errorMessage, textStd("NPCErrorString"));
        if (saveInValid & 1)    estrConcatf(&errorMessage, " %s", textStd("NPCInvalidNameString"));
        if (saveInValid & 2)    estrConcatf(&errorMessage, " %s", textStd("NPCInvalidDescString"));
        if (saveInValid & 4)    estrConcatf(&errorMessage, " %s", textStd("NPCINvalidGroupString"));
        dialogStd(DIALOG_OK, errorMessage, 0, 0, NPCdummyClearDialog, 0, DLGFLAG_ARCHITECT);
        estrDestroy(&errorMessage);
        waitingForDialog = 1;
    }
}
static void NPCprofileMenuExitCode(void)
{
    init = false;
    updateSeqHeadshotUniqueID();
}
NonLinearMenuElement NPC_profile_NLME = { MENU_NPC_PROFILE, { "CC_ProfileString", 0, 0 },
                0, 0, 0, 0, NPCprofileMenuExitCode };

void NPCProfileMenu(void)
{
    int screenScalingDisabled = is_scrn_scaling_disabled();
    float screenScaleX = 1.f, screenScaleY = 1.f, screenScale;

    calc_scrn_scaling(&screenScaleX, &screenScaleY);
    screenScale = MIN(screenScaleX, screenScaleY);

    if (getPCCEditingMode() < 0)
        setPCCEditingMode(getPCCEditingMode()*-1);

    toggle_3d_game_modes(SHOW_NONE);
    drawBackground( NULL );
    drawIdentityCard();
    //    draw menu frame
    set_scrn_scaling_disabled(1);
    if (!init)
    {
        init = 1;
        Profile_CritterComboBoxClear();
    }
    handleTextBoxes(screenScaleX, screenScaleY);
    printStaticText(screenScaleX, screenScaleY);
    //    draw back string
    if (drawNextButton(40*screenScaleX, 740*screenScaleY, 1001, screenScale, 0, waitingForDialog, 0, "BackString"))
    {
        sndPlay( "back", SOUND_GAME);
        NPCprofileMenuExitCode();
        if (getNPCEditCostumeMode())
        {
            start_menu( MENU_NPC_COSTUME );
        }
        else
        {
            CustomNPCEdit_exit(0);
        }
    }
    //    draw save (as) button
    if (drawNextButton(980*screenScaleX, 740*screenScaleY, 1001, screenScale, 1, waitingForDialog, 0, "PCCSaveAndContinuePrompt"))
    {
        NPCtrySave();
    }

    set_scrn_scaling_disabled(0);
    drawNonLinearMenu( NLM_NPCCreationMenu(), 20, 10, 20, 1000,20, 1.0f, NPC_profile_NLME.listId, NPC_profile_NLME.listId, 0 );

    set_scrn_scaling_disabled(screenScalingDisabled);
    if (getPCCEditingMode() > 0)
        setPCCEditingMode(getPCCEditingMode()*-1);

}