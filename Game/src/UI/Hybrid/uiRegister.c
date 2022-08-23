

#include "UI/Hybrid/uiRegister.h"
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
#include <utilitieslib/language/MessageStoreUtil.h>

#include "formatter/smf_parse.h"
#include "formatter/smf_format.h"
#include "formatter/smf_main.h"

#include "entity/origins.h"
#include "entity/classes.h"

#include "UI/uiInput.h"
#include "UI/uiUtilMenu.h"
#include "UI/uiUtil.h"
#include "UI/uiUtilGame.h"
#include "UI/uiClipper.h"
#include "UI/Hybrid/uiHybridMenu.h"
#include "UI/Hybrid/uiOrigin.h"
#include "UI/Hybrid/uiArchetype.h"
#include "UI/uiFocus.h"
#include "entity/character_base.h"
#include "entity/powers.h"
#include "gameComm/trayCommon.h"
#include "gameComm/initClient.h"
#include "UI/uiPowerInventory.h"
#include "graphics/seqgraphics.h"
#include "UI/uiDialog.h"
#include "filter/validate_name.h"
#include "UI/uiWindows.h"
#include "entity/entVarUpdate.h"
#include <utilitieslib/components/EString.h>
#include "entity/entPlayer.h"
#include "clientcomm/dbclient.h"
#include <utilitieslib/utils/file.h>
#include <utilitieslib/language/AppLocale.h>
#include <utilitieslib/utils/StringUtil.h>
#include "UI/uiGender.h"
#include "UI/uiCostume.h"
#include "UI/Hybrid/uiPower.h"
#include "UI/uiNet.h"
#include "UI/uiAvatar.h"
#include "entity/costume_client.h"
#include "entity/Supergroup.h"
#include "entity/character_level.h"
#include "player/inventory_client.h"
#include "UI/uiPet.h"

#include "LWC.h"

#define TMPSIZE 2048

static SMFBlock*         descEdit = NULL;
static SMFBlock*         bcEdit = NULL;
static SMFBlock*         knownPowers = NULL;
static int descEditTouched = 0;
static int descEditSize = 0;
static int bcEditTouched = 0;
static int bcEditSize = 0;
static int creationIncomplete = 1;
static int init = 0;
int gWaitingToEnterGame = FALSE;
int gLoggingIn = FALSE;
static int spinningAvatar = 1;

extern TextAttribs gHybridTextAttr;

static char *trim(char *pch)
{
    int len = strlen(pch);

    // Get rid of leading spaces, inefficiently
    while(*pch==' ')
    {
        strcpy(pch, pch+1);
        len--;
    }
    // Get rid of trailing spaces, inefficiently
    while(len>0 && pch[len-1]==' ')
    {
        pch[len-1]='\0';
        len--;
    }

    return pch;
}

static char * HTMLizeEntersAndTabs( char * text )
{
    char * tmp, *ret;
    tmp = strdup(strchrInsert( text, "<br>", '\n' ));
    ret = strchrInsert( tmp, "&nbsp;&nbsp;&nbsp;&nbsp;", '\t' );
    free(tmp);
    return ret;
}

static int getSizeWithoutBR(char * original)
{
    int len = 0;

    while (*original != 0)
    {
        ++len;  //increment length count
        if (strncmp(original, "<br>", 4) == 0)
        {
            original += 4;  //skip BR
        }
        else
        {
            ++original;
        }
    }
    return len;
}

static void enterGame(void * data)
{
    Entity * e = playerPtr();

    gLoggingIn = TRUE;
    uiGetFocus(NULL, FALSE);

    clearCostumeStack();

    trim(e->name);
    trim(e->strings->motto);

    uiRegisterCopyToEntity(e, 1);

    if( !e->db_id )
    {
        // Total special-case hack to get the preferred tray power order
        // Gets MM macros
        // Everything else is on the server
        int count = 5; // Put the MM macros in slots 6, 7, 8
        int j;

        if (playerIsMasterMind())
        {
            initnewbMasterMindMacros();

            for (j = 0; j < ARRAY_SIZE(newbMastermindMacros); j++)
            {
                // three of these
                buildMacroTrayObj(getTrayObj(e->pl->tray, kTrayCategory_PlayerControlled, 0, count, e->pchar->iCurBuild, true), newbMastermindMacros[j].pchDisplayHelp, textStd(newbMastermindMacros[j].pchName), newbMastermindMacros[j].pchIconName, 1);
                count++;
            }
        }
    }

    updateSeqHeadshotUniqueID();
    gWaitingToEnterGame = TRUE;
    start_menu( MENU_GAME );
}

static void returnToGameFromRegister()
{
    Entity * e = playerPtr();
    uiRegisterCopyToEntity(e, 0);
    sendDescription(e->strings->playerDescription, e->strings->motto);

    start_menu( MENU_GAME );

    resetHybridCommon();
    resetRegisterMenu();
}

static void chooseEnterGame(void * data)
{
    dialogStd( DIALOG_YES_NO, "ExitCharacterCreation", NULL, NULL, enterGame, 0, DLGFLAG_LIMIT_MOUSE_TO_DIALOG );
}

static void chooseHero(void *data)
{
    // Atlas Park and Mercy Island are not available before stage 3
    if (!LWC_CheckStageAndShowMessageIfNotReady(LWC_STAGE_3))
    {
        return;
    }

    pickHero();
    chooseEnterGame(0);
}

static void chooseVillain(void *data)
{
    // Atlas Park and Mercy Island are not available before stage 3
    if (!LWC_CheckStageAndShowMessageIfNotReady(LWC_STAGE_3))
    {
        return;
    }

    pickVillain();
    chooseEnterGame(0);
}

static void chooseHeroTutorial(void *data)
{
	pickHeroTutorial();
	chooseEnterGame(0);
}

static void chooseVillainTutorial(void *data)
{
	pickVillainTutorial();
	chooseEnterGame(0);
}

static void chooseNeutralTutorial(void *data)
{
	pickPrimalTutorial();
	chooseEnterGame(0);
}

static void chooseLoyalist(void *data)
{
    // Praetoria not available before stage 4
    if (!LWC_CheckStageAndShowMessageIfNotReady(LWC_STAGE_4))
    {
        return;
    }

    pickLoyalist();
    chooseEnterGame(0);
}

static void chooseResistance(void *data)
{
    // Praetoria not available before stage 4
    if (!LWC_CheckStageAndShowMessageIfNotReady(LWC_STAGE_4))
    {
        return;
    }

    pickResistance();
    chooseEnterGame(0);
}

static void choosePraetorianTutorial(void *data)
{
    // Praetoria not available before stage 4
    if (!LWC_CheckStageAndShowMessageIfNotReady(LWC_STAGE_4))
    {
        return;
    }

    pickPraetorianTutorial();
    chooseEnterGame(0);
}

static void choosePraetorianAlignment(void *data)
{
    // Praetoria not available before stage 4
    if (!LWC_CheckStageAndShowMessageIfNotReady(LWC_STAGE_4))
    {
        return;
    }

    dialogStd(DIALOG_TWO_RESPONSE, "LoyalistOrResistance", "LoyalistStr", "ResistanceStr", chooseLoyalist, chooseResistance, DLGFLAG_LIMIT_MOUSE_TO_DIALOG);
}

static void choosePrimalTutorial(void *data)
{
    // Primal Tutorial not available before stage 2
    if (!LWC_CheckStageAndShowMessageIfNotReady(LWC_STAGE_2))
    {
        return;
    }
	dialogStd3(DIALOG_THREE_RESPONSE,  textStd("TutorialSelect"), "TutorialHero", "TutorialVillain", "TutorialNeutral", chooseHeroTutorial, chooseVillainTutorial, chooseNeutralTutorial, 0);
}

static void chooseHV(void *data)
{
    // Atlas Park and Mercy Island are not available before stage 3
    if (!LWC_CheckStageAndShowMessageIfNotReady(LWC_STAGE_3))
    {
        return;
    }

    dialogStd( DIALOG_TWO_RESPONSE, "HeroOrVillainString", "HeroAlignment" , "VillainAlignment", chooseHero, chooseVillain, DLGFLAG_LIMIT_MOUSE_TO_DIALOG);
}

void uiRegisterCopyToEntity(Entity* e, int copyName)
{
    char* text;
    char* BR_replacedStr;

    if (copyName)
    {
        text = getNameBarName();
        if( text )
        {        
            strncpyt(e->name,text,MAX_PLAYER_NAME_LEN(e)+1);
        }
    }

    text = bcEdit->outputString;
    if( text && bcEditTouched )
    {
        char * tmpptr;
        tmpptr = strchr( text, '\n' );
        while( tmpptr )
        {
            strcpy( tmpptr, tmpptr+1 );
            tmpptr = strchr( text, '\n' );
        }

        strncpyt(e->strings->motto, text, 33);
    }
    else
    {
        strcpy( e->strings->motto, "" );
    }

    if (descEdit->outputString && descEditTouched)
    {
        estrCreate(&BR_replacedStr);
        estrConcatCharString(&BR_replacedStr, descEdit->outputString);
        //    replace <br> with \n
        if (BR_replacedStr)
        {
            char *br_ptr = NULL;
            while ((br_ptr = strstr(BR_replacedStr, "<br>")) != NULL)
            {
                strcpy_s(br_ptr, strlen(br_ptr), br_ptr+3);
                br_ptr[0] = '\n';
            }
        }

        if( BR_replacedStr )
        {
            strncpyt(e->strings->playerDescription, BR_replacedStr, ARRAY_SIZE(e->strings->playerDescription));
        }
        else
        {
            strcpy( e->strings->playerDescription, "" );
        }
        estrDestroy(&BR_replacedStr);
    }
    else
    {
        strcpy( e->strings->playerDescription, "" );
    }

}

//locally verify name
static int handleNamePreValidation(char * text)
{
    if(text)
    {
        ValidateNameResult vnr = ValidateName(text, NULL, true, MAX_PLAYER_NAME_LEN(e) );

        switch(vnr)
        {
        case VNR_Malformed:
        case VNR_Profane:
        case VNR_Reserved:
        case VNR_WrongLang:
        case VNR_TooLong:
        default:
            dialogStd( DIALOG_OK, "InvalidName", NULL, NULL, NULL, NULL, 0 );
            break;

        case VNR_Titled:
            dialogStd( DIALOG_OK, "InvalidNameTitled", NULL, NULL, NULL, NULL, 0 );
            break;
        case VNR_Valid:
            // ----------
            // special Korea hack: filter any non-displayable characters from
            // the name
            // ab: it turns out this is taken care of by UIEdit if something can't be drawn.

            if( getCurrentLocale() == 3 )
            {
                wchar_t wtxt[128] = {0};
                wchar_t *pwtxt = NULL;
                int iEnd;

                iEnd = UTF8ToWideStrConvert(text, wtxt, ARRAY_SIZE( wtxt ));
            }

            // ----------
            // 

            if( multipleNonAciiCharacters(text) )
            {
                dialogStd( DIALOG_OK, "MultipleNonAsciiChars", NULL, NULL, NULL, NULL,0 );
            }
            else
            {
                return 1;
            }
            break;
        }
    }
    else
    {
        dialogStd( DIALOG_OK, "InvalidName", NULL, NULL, NULL, NULL, 0 );
    }
    return 0;
}

static void handleGameEntry(Entity *e, char *text)
{
    int nonAscii;

    nonAscii = multipleNonAciiCharacters(text);

    // This way we only clear the focus if the name is valid no matter what
    // other dialogs etc we're going to show.
    if( !nonAscii )
    {
        uiGetFocus(NULL, FALSE);

        window_closeAlways();
    }

    if( nonAscii )
    {
        dialogStd( DIALOG_OK, "MultipleNonAsciiChars", NULL, NULL, NULL, NULL,0 );
    }
    else
    {
        int count = 0;
        if( getPraetorianChoice() )
        {
            // Praetoria not available before stage 4
            if (!LWC_CheckStageAndShowMessageIfNotReady(LWC_STAGE_4))
            {
                return;
            }

            //praetoria
                dialogStd( DIALOG_YES_NO, "EnterTutorial", NULL, NULL, choosePraetorianTutorial, choosePraetorianAlignment, DLGFLAG_LIMIT_MOUSE_TO_DIALOG );
            }
            else
            {
            //primal
            if (class_MatchesSpecialRestriction(e->pchar->pclass, "Kheldian"))
            {
                chooseHero(NULL);
            }
            else if (class_MatchesSpecialRestriction(e->pchar->pclass, "ArachnosSoldier") || class_MatchesSpecialRestriction(e->pchar->pclass, "ArachnosWidow"))
            {
                chooseVillain(NULL);
            }
            else
            {
				dialogStd( DIALOG_YES_NO, "EnterTutorial", NULL, NULL, choosePrimalTutorial, chooseHV, DLGFLAG_LIMIT_MOUSE_TO_DIALOG );
            }
        }
    }
}

void resetRegisterMenu()
{
    if (descEdit)
    {
        smfBlock_Destroy(descEdit);
        descEdit = NULL;
    }
    if (bcEdit)
    {
        smfBlock_Destroy(bcEdit);
        bcEdit = NULL;
    }
    if (knownPowers)
    {
        smfBlock_Destroy(knownPowers);
        knownPowers = NULL;
    }

    descEditTouched = 0;
    bcEditTouched = 0;
    descEditSize = 0;
    bcEditSize = 0;
    init = 0;
    spinningAvatar = 1;
    gWaitingToEnterGame = FALSE;
}

void registerMenuInit()
{
    Entity * e = playerPtr();
    int hasBeenInGame = (e->db_id > 0);

    //setup register menu
    if (!descEdit)
    {
        descEdit = smfBlock_Create();
        smf_SetFlags(descEdit, SMFEditMode_ReadWrite, 0, SMFInputMode_AnyTextNoTagsOrCodes, ARRAY_SIZE(e->strings->playerDescription)-1, SMFScrollMode_InternalScrolling, 
            SMFOutputMode_StripFlaggedTags, SMFDisplayMode_AllCharacters, SMFContextMenuMode_None, SMAlignment_Left, 0, "descEdit", -1);
    }

    if (!bcEdit)
    {
        bcEdit = smfBlock_Create();
        smf_SetFlags(bcEdit, SMFEditMode_ReadWrite, 0, SMFInputMode_AnyTextNoTagsOrCodes, 32, SMFScrollMode_InternalScrolling, 
            SMFOutputMode_StripAllTags, SMFDisplayMode_AllCharacters, SMFContextMenuMode_None, SMAlignment_Left, 0, "bcEdit", -1);
    }

    if (hasBeenInGame)
    {
        //in profile mode: set pre-existing description and battlecry text
        smf_SetRawText(descEdit, HTMLizeEntersAndTabs(e->strings->playerDescription),0);
        smf_SetRawText(bcEdit, e->strings->motto,0);
        descEditTouched = 1;
        bcEditTouched = 1;
    }
    else
    {
        //show powers only in character creation
        if (!knownPowers )
        {
            knownPowers = smfBlock_Create();
        }
        smf_SetFlags(knownPowers, SMFEditMode_Unselectable, SMFLineBreakMode_MultiLineBreakOnWhitespace, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    }

    init = 1;
}

static void displayPowers(F32 x, F32 y, F32 z, F32 wd, F32 ht)
{
    Entity * e = playerPtr();
    char tmp[TMPSIZE] = {0}, tmp2[TMPSIZE] = {0};
    int i, j;
    U32    tmpLen = 1;

    // cat all the power names into one string
    // Skip all temp powers
    tmp[0] = 0;
    for( i = 0; i < eaSize( &e->pchar->ppPowerSets); ++i )
    {
        int setCount = eaSize( &e->pchar->ppPowerSets);
        if (stricmp(e->pchar->ppPowerSets[i]->psetBase->pcatParent->pchName,"Temporary_Powers") != 0) //skip all temp powers
        {
            int pwrCount = eaSize( &e->pchar->ppPowerSets[i]->ppPowers);
            for( j = 0; j < pwrCount; ++j )
            {
                Power * pwr = e->pchar->ppPowerSets[i]->ppPowers[j];
                if (pwr && (power_ShowInInventory(e->pchar, pwr)))
                {
                    U32 tmp2Len;
                    msPrintf( menuMessages, SAFESTR(tmp2), pwr->ppowBase->pchDisplayName );
                    tmp2Len = strlen(tmp2);
                    if (tmpLen + tmp2Len < TMPSIZE)
                    {
                        strcat( tmp, tmp2 );
                        tmpLen += tmp2Len;
                    }
                    if( j < pwrCount - 1 || i < setCount - 1  )
                    {
                        if (tmpLen + 2 < TMPSIZE)
                        {
                            strcat( tmp, ", ");
                            tmpLen += 2;
                        }
                    }
                }
            }
        }
    }

    smf_SetScissorsBox(knownPowers, x, y, wd, ht);
    smf_SetRawText(knownPowers, tmp, 0);
    smf_Display(knownPowers, x, y, z, wd, ht, 0, 0, &gHybridTextAttr, 0);
}

void registerMenuEnterCode()
{
    Entity *e = playerPtr();
    int slotid = 0;
    creationIncomplete = 0;

    e->pchar->porigin = getSelectedOrigin();  //set the origin here
    devassert(e->pchar->porigin);

    //check if all necessary choices are made
    devassertmsg(MENU_TOTAL <= sizeof(int)*8, "More menus than int. May need to remove some or use long integer.");
    if (getSelectedCharacterClass() == 0)
    {
        creationIncomplete |= (1<<MENU_ARCHETYPE);
    }
    if (!powersReady(0) || !powersChosen(0))
    {
        creationIncomplete |= (1<<MENU_POWER);
    }
    

    if (e && e->pl)
        slotid = e->pl->current_costume;

    //create costume if there isn't one
    costumeInit();
    setDefaultCostume(true);
    toggle_3d_game_modes(SHOW_TEMPLATE);
    costume_Apply(e);

    if (hasStoreCostumeParts() || costume_Validate(e, costume_current(e), slotid, NULL, NULL, 0, 1) != COSTUME_ERROR_NOERROR) //allowStoreParts set to true, since this validation function doesn't have access to the account inventory
    {
        creationIncomplete |= (1<<MENU_COSTUME);
    }

}

void registerMenuExitCode()
{
    toggle_3d_game_modes(SHOW_NONE);
}

void registerMenu()
{
    //info bars
    AtlasTex * floorIcon = getPraetorianChoice() ? atlasLoadTexture("flooricon_praetorianlogo") : atlasLoadTexture("flooricon_hero_villainlogo");
    int infoTitleColor = (CLR_H_BACK & 0xffffff00) | 0x99; //60% opacity
    int infoInfoColor = 0x42424299;
    int editColor = 0x42424200;
    F32 infoTitleWidth = 200.f;
    F32 infoInfoWidth = 300.f;
    F32 bcWidth = 550.f;

    AtlasTex * typingArrow = atlasLoadTexture("icon_typingarrow");

    F32 x, y, space;
    F32 z = 5000.f;
    F32 borderWd = 15.f;
    Entity *e = playerPtr();
    int hasBeenInGame = (e->db_id > 0);
    //int i;
    F32 xwalk, ywalk, offset;
    CBox box;
    char tmp[TMPSIZE];
    int numChars;
    F32 xposSc, yposSc, textXSc, textYSc, xp, yp;

    // we will stay on this menu while waiting to enter the game.  During this time,
    // we don't want this menu to do anything.
    if (gWaitingToEnterGame)
    {
        collisions_off_for_rest_of_frame = false;  //give the mouse back during loading screen
        return;
    }

    if (!init)
    {
        registerMenuInit();
    }

    // draw common top bar only in creation mode
    if (!hasBeenInGame)
    {
        if(drawHybridCommon(HYBRID_MENU_CREATION))
        {
            //if something's wrong with top bar, just return
            return;
        }
    }
    else
    {
        if(drawHybridCommon(HYBRID_MENU_NO_NAVIGATION))
        {
            return;
        }

        setScalingModeAndOption(SPRITE_Y_UNIFORM_SCALE, SSO_SCALE_TEXTURE);
        drawCheckNameBar(HYBRID_NAME_ID);
        //TODO: draw some kind of profile title
    }

    setScalingModeAndOption(SPRITE_Y_UNIFORM_SCALE, SSO_SCALE_TEXTURE);
    calculatePositionScales(&xposSc, &yposSc);
    calculateSpriteScales(&textXSc, &textYSc);
    xp = DEFAULT_SCRN_WD / 2.f;
    yp = 0.f;
    applyPointToScreenScalingFactorf(&xp, &yp);

    x = xp - 466.f*xposSc;
    y = 150.f*yposSc;
    space = 26.f*yposSc;
    
    ywalk = y;
    xwalk = x;
    drawHybridSliderEmpty(xwalk, ywalk, z, 1.f, 550.f, 250.f, "RegisterOrigin", -1);
    ywalk += space;
    drawHybridSliderEmpty(xwalk, ywalk, z, 1.f, 550.f, 250.f, "RegisterArchetype", -1);
    ywalk += space;
    drawHybridSliderEmpty(xwalk, ywalk, z, 1.f, 550.f, 250.f, "RegisterHitpoints", -1);
    ywalk += space;
    drawHybridSliderEmpty(xwalk, ywalk, z, 1.f, 550.f, 250.f, "RegisterEndurance", -1);
    ywalk += space;
    drawHybridSliderEmpty(xwalk, ywalk, z, 1.f, 550.f, 250.f, "RegisterLevel", -1);
    ywalk += space;
    drawHybridSliderEmpty(xwalk, ywalk, z, 1.f, 550.f, 250.f, "RegisterXPTotal", -1);
    ywalk += space;
    drawHybridSliderEmpty(xwalk, ywalk, z, 1.f, 550.f, 250.f, "RegisterXPNeeded", -1);
    ywalk += space;
    drawHybridSliderEmpty(xwalk, ywalk, z, 1.f, 550.f, 250.f, "RegisterSuperGroupAffiliation", -1);
    ywalk += space;
    if (hasBeenInGame)
    {
        drawHybridSliderEmpty(xwalk, ywalk, z, 1.f, 550.f, 250.f, "InfluenceString", -1);
        ywalk += space;
    }

    setSpriteScaleMode(SPRITE_XY_NO_SCALE);
    font(&hybridbold_14);
    font_color( CLR_H_DESC_TEXT, CLR_H_DESC_TEXT );
    offset = 300.f*xposSc;
    ywalk = y + 19.f*yposSc;

    //origin
    if (!hasBeenInGame)
    {
        sprintf(tmp, "%s", getSelectedOrigin() ? getSelectedOrigin()->pchDisplayName : "RegisterNoneSelected");
    }
    else
    {
        devassert(e->pchar->porigin->pchDisplayName);  //would be strange if this didn't exist
        sprintf(tmp, "%s", e->pchar->porigin->pchDisplayName);
    }
    prnt( xwalk + offset, ywalk, z, textXSc, textYSc, tmp);
    ywalk += space;

    //archetype
    if (!hasBeenInGame)
    {
        sprintf(tmp, "%s", getSelectedCharacterClass() ? getSelectedCharacterClass()->pchDisplayName : "RegisterNoneSelected");
    }
    else
    {
        devassert(e->pchar->pclass->pchDisplayName);  //would be strange if this didn't exist
        sprintf(tmp, "%s", e->pchar->pclass->pchDisplayName);
    }
    prnt( xwalk + offset, ywalk, z, textXSc, textYSc, tmp);
    ywalk += space;

    //hitpoints
    if (!hasBeenInGame && !getSelectedCharacterClass())
    {
        sprintf(tmp, "");
    }
    else
    {
        if( !e->pchar->attrMax.fHitPoints )
            sprintf(tmp, "%s", itoa_with_commas_static((int)e->pchar->pclass->pattrMax[0].fHitPoints));
        else
            sprintf(tmp, "%s", itoa_with_commas_static((int)e->pchar->attrMax.fHitPoints));
    }
    prnt( xwalk + offset, ywalk, z, textXSc, textYSc, tmp);
    ywalk += space;

    //endurance
    if (!hasBeenInGame && !getSelectedCharacterClass())
    {
        sprintf(tmp, "");
    }
    else
    {
        if( !e->pchar->attrMax.fEndurance )
            sprintf(tmp, "%s", itoa_with_commas_static(e->pchar->pclass->pattrMax[0].fEndurance));
        else
            sprintf(tmp, "%s", itoa_with_commas_static(e->pchar->attrMax.fEndurance));
    }
    prnt( xwalk + offset, ywalk, z, textXSc, textYSc, tmp);
    ywalk += space;

    //level
    sprintf(tmp, "%d", e->pchar->iLevel+1);
    prnt( xwalk + offset, ywalk, z, textXSc, textYSc, tmp);
    ywalk += space;

    //xp
    if(e->pchar->iExperienceDebt>0)
    { 
        char exp[128];
        char debt[128];
        sprintf(tmp, "%s (%s %s)", itoa_with_commas(e->pchar->iExperiencePoints,exp), itoa_with_commas(e->pchar->iExperienceDebt,debt), textStd("DebtString"));
    }
    else
        sprintf(tmp, "%s", itoa_with_commas_static(e->pchar->iExperiencePoints));
    prnt( xwalk + offset, ywalk, z, textXSc, textYSc, tmp);
    ywalk += space;

    //xp needed
    sprintf(tmp, "%s", itoa_with_commas_static(character_ExperienceNeeded(e->pchar)));
    prnt( xwalk + offset, ywalk, z, textXSc, textYSc, tmp);
    ywalk += space;

    //super group
    if(e->supergroup)
    {
        sprintf(tmp, "%s", e->supergroup->name);
    }
    else
    {
        // Translate "NoneString"
        sprintf(tmp, textStd("NoneString"));
    }

    // Don't translate the super group name
    prntEx( xwalk + offset, ywalk, z, textXSc, textYSc, NO_MSPRINT, tmp);
    ywalk += space;

    if (hasBeenInGame)
    {
        sprintf(tmp, "%s", itoa_with_commas_static(e->pchar->iInfluencePoints) );
        prnt( xwalk + offset, ywalk, z, textXSc, textYSc, tmp);
        ywalk += space;
    }
    setSpriteScaleMode(SPRITE_Y_UNIFORM_SCALE);
    ywalk -= 20.f*yposSc;
    
    if (!hasBeenInGame)
    {
        //powers 
        drawHybridDescFrame(xwalk, ywalk, z, bcWidth, 80.f, HB_DESCFRAME_FULLFRAME, 0.f, 1.f, 0.5f, H_ALIGN_LEFT, V_ALIGN_TOP);

        setSpriteScaleMode(SPRITE_XY_NO_SCALE);
        font(&title_12);
        font_outl(0);
        font_color( CLR_H_DESC_TEXT_DS, CLR_H_DESC_TEXT_DS );
        prnt( xwalk+15.f*xposSc+2.f, ywalk+24.f*yposSc+2.f, z, textXSc, textYSc, "RegisterPowers");
        font_color( CLR_WHITE, CLR_WHITE );
        prnt( xwalk+15.f*xposSc, ywalk+24.f*yposSc, z, textXSc, textYSc, "RegisterPowers");
        font_outl(1);

        gHybridTextAttr.piScale = (int*)(int)(textYSc*SMF_FONT_SCALE);
        displayPowers(xwalk+borderWd*xposSc, ywalk+33.f*yposSc, z, (bcWidth-borderWd*2.5f)*xposSc, 37.f*yposSc);
        gHybridTextAttr.piScale = (int*)(int)SMF_FONT_SCALE;
        setSpriteScaleMode(SPRITE_Y_UNIFORM_SCALE);
        ywalk += 80.f*yposSc;
    }

    drawHybridDescFrame(xwalk, ywalk, z, bcWidth, 80.f, HB_DESCFRAME_FULLFRAME | HB_DESCFRAME_HOVERBACKGROUND, 0.f, 1.f, 0.5f, H_ALIGN_LEFT, V_ALIGN_TOP);

    // battle cry
    BuildScaledCBox(&box, xwalk+borderWd*xposSc, ywalk+33.f*yposSc, (bcWidth-borderWd*2.5f)*xposSc, 37.f*yposSc);
    if( smfBlock_HasFocus(bcEdit) || mouseCollision(&box))
    {
        if (!bcEditTouched && mouseClickHit(&box, MS_LEFT))
        {
            //first click, clear message
            bcEditTouched = 1;
            smf_SetRawText(bcEdit, "", 0);
        }
    }
    if (!bcEditTouched)
    {
        display_sprite_positional(typingArrow, xwalk+borderWd*xposSc, ywalk+33.f*yposSc, z, 1.f, 1.f, CLR_WHITE, H_ALIGN_LEFT, V_ALIGN_TOP );
    }

    setSpriteScaleMode(SPRITE_XY_NO_SCALE);
    font(&title_12);
    font_outl(0);
    font_color( CLR_H_DESC_TEXT_DS, CLR_H_DESC_TEXT_DS );
    prnt( xwalk+15.f*xposSc+2.f, ywalk+24.f*yposSc+2.f, z, textXSc, textYSc, "RegisterBattleCryTitle");
    font_color( CLR_WHITE, CLR_WHITE );
    prnt( xwalk+15.f*xposSc, ywalk+24.f*yposSc, z, textXSc, textYSc, "RegisterBattleCryTitle");
    font_outl(1);

    if (bcEdit->editedThisFrame)
    {
        bcEditSize = getSizeWithoutBR(bcEdit->rawString);
    }
    font(&hybridbold_12);
    font_color( CLR_H_DESC_TEXT_DS, CLR_H_DESC_TEXT_DS );
    prnt( xwalk+460.f*xposSc+1.f, ywalk+24.f*yposSc+1.f, z, textXSc, textYSc, "(%d/32)", bcEditSize);
    font_color( CLR_WHITE, CLR_WHITE );
    prnt( xwalk+460.f*xposSc, ywalk+24.f*yposSc, z, textXSc, textYSc, "(%d/32)", bcEditSize);

    smf_SetScissorsBox(bcEdit, xwalk+borderWd*xposSc, ywalk+33.f*yposSc, (bcWidth-borderWd*2.5f)*xposSc, 37.f*yposSc);
    gHybridTextAttr.piScale = (int*)(int)(textYSc*SMF_FONT_SCALE);
    smf_Display(bcEdit, xwalk+borderWd*xposSc, ywalk+33.f*yposSc, z, (bcWidth-borderWd*2.5f)*xposSc, 37.f, 0, 0, &gHybridTextAttr, 0);
    gHybridTextAttr.piScale = (int*)(int)(1.f*SMF_FONT_SCALE);
    if (!bcEditTouched)
    {
        font(&hybridbold_14);
        font_color(CLR_H_DESC_TEXT, CLR_H_DESC_TEXT);
        prnt(xwalk+(borderWd + typingArrow->width)*xposSc, ywalk+47.f*yposSc, z, textXSc, textYSc, "RegisterBattleCryText");
    }
    setSpriteScaleMode(SPRITE_Y_UNIFORM_SCALE);

    ywalk += 80.f*yposSc;

    drawHybridDescFrame(xwalk, ywalk, z, bcWidth, 170.f, HB_DESCFRAME_FULLFRAME | HB_DESCFRAME_HOVERBACKGROUND, 0.f, 1.f, 0.5f, H_ALIGN_LEFT, V_ALIGN_TOP);

    BuildScaledCBox(&box, xwalk+borderWd*xposSc, ywalk+33.f*yposSc, (bcWidth-borderWd*2.5f)*xposSc, 127.f*yposSc);
    if( smfBlock_HasFocus(descEdit) || mouseCollision(&box))
    {
        if (!descEditTouched && mouseClickHit(&box, MS_LEFT))
        {
            //first click, clear message
            descEditTouched = 1;
            smf_SetRawText(descEdit, "", 0);
        }
    }
    if (!descEditTouched)
    {
        display_sprite_positional(typingArrow, xwalk+borderWd*xposSc, ywalk+33.f*yposSc, z, 1.f, 1.f, CLR_WHITE, H_ALIGN_LEFT, V_ALIGN_TOP );
    }
    setSpriteScaleMode(SPRITE_XY_NO_SCALE);
    font(&title_12);
    font_outl(0);
    font_color( CLR_H_DESC_TEXT_DS, CLR_H_DESC_TEXT_DS );
    prnt( xwalk+15.f*xposSc+2.f, ywalk+24.f*yposSc+2.f, z, textXSc, textYSc, "RegisterCharacterDescrTitle");
    font_color( CLR_WHITE, CLR_WHITE );
    prnt( xwalk+15.f*xposSc, ywalk+24.f*yposSc, z, textXSc, textYSc, "RegisterCharacterDescrTitle");
    font_outl(1);

    // character description
    if (descEdit->editedThisFrame)
    {
        descEditSize = getSizeWithoutBR(descEdit->rawString);
    }
    numChars = ARRAY_SIZE(e->strings->playerDescription)-1;

    font(&hybridbold_12);
    font_color( CLR_H_DESC_TEXT_DS, CLR_H_DESC_TEXT_DS );
    prnt( xwalk+460.f*xposSc+1.f, ywalk+24.f*yposSc+1.f, z, textXSc, textYSc, "(%d/%d)", descEditSize, numChars);
    font_color( CLR_WHITE, CLR_WHITE );
    prnt( xwalk+460.f*xposSc, ywalk+24.f*yposSc, z, textXSc, textYSc, "(%d/%d)", descEditSize, numChars);

    smf_SetScissorsBox(descEdit, xwalk+borderWd*xposSc, ywalk+33.f*yposSc, (bcWidth-borderWd*2.5f)*xposSc, 127.f*yposSc);
    gHybridTextAttr.piScale = (int*)(int)(textYSc*SMF_FONT_SCALE);
    smf_Display(descEdit, xwalk+borderWd*xposSc, ywalk+33.f*yposSc, z, (bcWidth-borderWd*2.5f)*xposSc, 127.f*yposSc, 0, 0, &gHybridTextAttr, 0);
    gHybridTextAttr.piScale = (int*)(int)(1.f*SMF_FONT_SCALE);
    if (!descEditTouched)
    {
        font(&hybridbold_14);
        font_color(CLR_H_DESC_TEXT, CLR_H_DESC_TEXT);
        prnt(xwalk+(borderWd + typingArrow->width)*xposSc, ywalk+47.f*yposSc, z, textXSc, textYSc, "RegisterCharacterDescrText");
    }

    setScalingModeAndOption(SPRITE_Y_UNIFORM_SCALE, SSO_SCALE_ALL);
    display_sprite_positional(floorIcon, 790.f, 620.f, 5.f, 1.f, 1.f, 0xffffff60, H_ALIGN_CENTER, V_ALIGN_TOP );
    if (drawAvatarControls(790.f, 670.f, 0, 1))
    {
        spinningAvatar = 0;
    }

    setScalingModeAndOption(SPRITE_XY_SCALE, SSO_SCALE_ALL);

    if (hasBeenInGame)
    {
        //profile has no reason for a back button, just "return to game"
        if( drawHybridNext( DIR_RIGHT_NO_NAV, 0, "PlayGameString" ) )
        {
            returnToGameFromRegister();
        }
    }
    else
    {
        char* enterText = NULL;

        //back button
        drawHybridNext( DIR_LEFT, gWaitingToEnterGame, "BackString" ); 


        if (creationIncomplete)
        {
            if (creationIncomplete & (1<<MENU_ARCHETYPE))
            {
                enterText = "ArchetypeIncomplete";
            }
            else if (creationIncomplete & (1<<MENU_POWER))
            {
                enterText = "PowerIncomplete";
            }
            else if (creationIncomplete & (1<<MENU_COSTUME))
            {
                enterText = "CostumeIncomplete";
            }
        }
        else
        {
            if (getPraetorianChoice())
            {
                enterText = "Praetorian_PlayGameString";
            }
            else
            {
                enterText = "Neutral_PlayGameString";
            }
        }

        //next button
        if( drawHybridNext( DIR_RIGHT, creationIncomplete || gWaitingToEnterGame, enterText ) )
        {
            char *text = getNameBarName();
            Entity * e = playerPtr();

            if (handleNamePreValidation(text)) //check locally if the name is valid
            {
                uiGetFocus(NULL, FALSE);
                handleGameEntry( e, text);
            }
        }

        if (creationIncomplete)
        {
            static int playingSound = 0;
            BuildCBox(&box, 923.5f, 680.f,103.5f, 138.f); //normal "next" boundaries
            if (mouseCollision(&box))
            {
                //enable flash
                hybridMenuFlash(creationIncomplete);
                if (!playingSound)
                {
                    sndPlay("N_ErrorTab", SOUND_UI_ALTERNATE);
                    playingSound = 1;
                }
            }
            else
            {
                playingSound = 0;
            }
        }
    }

    //get current costume
    toggle_3d_game_modes(SHOW_TEMPLATE);
    costume_Apply(e);

    if (!spinningAvatar)
    {
        playerTurn( -0.4f * TIMESTEP );  //make an opposed turn to the spinning
    }
    if( gCurrentGender == AVATAR_GS_HUGE  )
        moveAvatar( AVATAR_PROFILE_HUGE );
    else if( gCurrentGender == AVATAR_GS_FEMALE )
        moveAvatar( AVATAR_PROFILE_FEMALE);
    else
        moveAvatar( AVATAR_PROFILE_MALE );

    scaleAvatar();
}
