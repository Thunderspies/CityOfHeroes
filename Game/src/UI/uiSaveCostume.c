/***************************************************************************
*     Copyright (c) 2000-2005, Cryptic Studios
*     All Rights Reserved
*     Confidential Property of Cryptic Studios
*
* $Workfile: uiLoadCostume.c $
* 
***************************************************************************/

#include <utilitieslib/stdtypes.h>
#include <utilitieslib/utils/StringUtil.h>
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/components/earray.h>
#include "entity/entity.h"
#include "player/player.h"
#include "seq/seq.h"
#include "seq/seqstate.h"

#include <utilitieslib/language/MessageStoreUtil.h>
#include "cmdparse/cmdgame.h"
#include "entity/costume.h"
#include "entity/entPlayer.h"
#include "entity/entclient.h"
#include "entity/costume_client.h"
#include "entity/character_base.h"
#include "gameData/costume_data.h"
#include "language/langClientUtil.h"
#include <utilitieslib/utils/Cbox.h>

#include "UI/uiGame.h"
#include "UI/uiUtil.h"
#include "UI/uiInput.h"
#include "UI/uiAvatar.h"
#include "UI/uiTailor.h"
#include "UI/uiCostume.h"
#include "UI/uiGender.h"
#include "UI/uiUtilGame.h"
#include "UI/uiUtilMenu.h"  
#include "UI/uiWindows.h"
#include "UI/uiSupercostume.h"
#include "UI/uiSuperRegistration.h"
#include "UI/uiEditText.h"
#include "UI/uiNet.h"

#include "graphics/ttFontUtil.h"
#include "UI/sprite/sprite_base.h" 
#include "UI/sprite/sprite_text.h"
#include "UI/sprite/sprite_font.h"
#include "render/tex.h"

#include "uiListView.h"
#include "UI/uiClipper.h"
#include "UI/uiTailor.h"
#include "UI/uiFocus.h"
#include "uiSaveCostume.h"
#include "filter/validate_name.h"
#include <utilitieslib/components/EString.h>
#include "UI/uiDialog.h"
#include "sound/sound.h"
#include <utilitieslib/utils/file.h>
#include <utilitieslib/utils/ConvertUtf.h>
#include "graphics/font.h"
#include "entity/power_customization.h"
#include "UI/uiPowerCust.h"
#include "entity/powers.h"
#include "UI/uiUtilMenu.h"
#include "uiPCCCreationNLM.h"
#include <direct.h> //for getcwd()
#include "formatter/smf_main.h"
#include "formatter/smf_parse.h"
#include "formatter/smf_format.h"
#include "UI/sprite/sprite_base.h"
#include "graphics/textureatlas.h"

#include "UI/Hybrid/uiHybridMenu.h"

#define DEFAULT_CLR_BACK 0x00000088
#define DEFAULT_CLR_BORD CLR_WHITE

char gCostumeSaveFileName[256];

static int init= 0;
static CBox box = {  50, 135, 350, 175 };
static CBox out_path_box = {  50, 650, 660, 690 };
static int prevMenu = MENU_COSTUME;
static Entity *saveEntity = 0;
static enum saveMethod
{
    SAVE_METHOD_COSTUME,
    SAVE_METHOD_POWER_CUST,
    SAVE_METHOD_PRIMARY_POWER_CUST,
    SAVE_METHOD_SECONDARY_POWER_CUST,
    SAVE_METHOD_COUNT
}saveMethod;

static void saveFile( char * filename, int savingPowerCust)
{    
    if( filename )
    {
        Entity *e = playerPtr();
        static PowerCustomizationList tempList = { 0};
        switch (savingPowerCust)
        {
        case SAVE_METHOD_COSTUME:
            ParserWriteTextFile(filename, ParseCostume, e->costume,0,0 );
            break;
        case SAVE_METHOD_POWER_CUST:
            ParserWriteTextFile(filename, ParsePowerCustomizationList, e->powerCust,0,0 );
            break;
        case SAVE_METHOD_PRIMARY_POWER_CUST:
            {
                int i;
                PowerCustomizationList *currentList = e->powerCust;
                for (i = 0; i < eaSize(&currentList->powerCustomizations); ++i)
                {
                    const PowerCategory *pCat = e->pchar->pclass->pcat[kCategory_Primary];
                    if (currentList->powerCustomizations[i]->power
                        && pCat == currentList->powerCustomizations[i]->power->psetParent->pcatParent)
                    {
                        eaPush(&tempList.powerCustomizations, currentList->powerCustomizations[i]);
                    }
                }
                ParserWriteTextFile(filename, ParsePowerCustomizationList, &tempList,0,0 );
                eaDestroy(&tempList.powerCustomizations);
            }
            break;
        case SAVE_METHOD_SECONDARY_POWER_CUST:
            {
                int i;
                PowerCustomizationList *currentList = e->powerCust;
                for (i = 0; i < eaSize(&currentList->powerCustomizations); ++i)
                {
                    const PowerCategory *pCat = e->pchar->pclass->pcat[kCategory_Secondary];
                    if (currentList->powerCustomizations[i]->power
                        && pCat == currentList->powerCustomizations[i]->power->psetParent->pcatParent)
                    {
                        eaPush(&tempList.powerCustomizations, currentList->powerCustomizations[i]);
                    }
                }
                ParserWriteTextFile(filename, ParsePowerCustomizationList, &tempList,0,0 );
                eaDestroy(&tempList.powerCustomizations);
            }
            break;
        default:
            break;
        }
    }
}
void OverWriteCustomFile(int saveFileMethod)
{
    if (getPCCEditingMode() < 0)
        setPCCEditingMode(getPCCEditingMode()*-1);
    saveFile(gCostumeSaveFileName, saveFileMethod);
    if (saveFileMethod < SAVE_METHOD_PRIMARY_POWER_CUST)
    {
        dialogStd( DIALOG_OK, "CostumeSaved", NULL, NULL, NULL, NULL, 0 );
        start_menu(prevMenu );
    }

    if (getPCCEditingMode() > 0)
        setPCCEditingMode(getPCCEditingMode()*-1);
}

void OverWriteCostumeFile(void *data)    {    OverWriteCustomFile(SAVE_METHOD_COSTUME);    }
void OverWritePowerCustFile(void *data)    {    OverWriteCustomFile(SAVE_METHOD_POWER_CUST);    }
void OverWritePrimaryPowerCustFile(void *data)    {    OverWriteCustomFile(SAVE_METHOD_PRIMARY_POWER_CUST);    }
void OverWriteSecondaryPowerCustFile(void *data)    {    OverWriteCustomFile(SAVE_METHOD_SECONDARY_POWER_CUST);    }

void saveCostume_start(int previous_menu)
{

    entSetAlpha( saveEntity, 255, SET_BY_CAMERA );    

    prevMenu = previous_menu;
    init = 0;
    start_menu(MENU_SAVECOSTUME);
}

static const char * getExtension(int saveFileMethod)
{
    if (saveFileMethod == SAVE_METHOD_COSTUME)    return getCustomCostumeExt();
    else                                        return powerCustExt();

}
static const char * getFolder(int saveFileMethod)
{
    if (saveFileMethod == SAVE_METHOD_COSTUME)    return getCustomCostumeDir();
    else                                        return powerCustPath();
}
static char *extraExt(int saveFileMethod)
{
    switch (saveFileMethod)
    {
    case SAVE_METHOD_PRIMARY_POWER_CUST:
        return "Primary";
    case SAVE_METHOD_SECONDARY_POWER_CUST:
        return "Secondary";
    default:
        return "";
    }
}
static void setupSave(int saveFileMethod, char * text)
{
    sndPlay( "N_Error", SOUND_GAME );

    if(text)
    {
        ValidateNameResult vnr = ValidateName(text, NULL, true, 20);

        if(vnr!=VNR_Valid)
        {
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
            }
        }
        else
        {
            if ( isInvalidFilename(text) )
            {
                dialogStd( DIALOG_OK, "InvalidName", NULL, NULL, NULL, NULL, 0 );
            }
            else
            {
                char fname[MAX_PATH];
                FILE * open_test = NULL;

                sprintf(fname, "%s/%s%s%s", getFolder(saveFileMethod), text, getExtension(saveFileMethod), extraExt(saveFileMethod));

                makeDirectories(getFolder(saveFileMethod));
                strcpy( gCostumeSaveFileName, fname );

                open_test = fopen( fname, "rb" );
                //open_test = _wfopen( fname, L"rb" );
                if ( open_test )
                {
                    switch (saveFileMethod)
                    {
                        case SAVE_METHOD_COSTUME:                dialogStd( DIALOG_YES_NO, "FileExists", NULL, NULL, OverWriteCostumeFile, NULL, 0 ); break;
                        case SAVE_METHOD_POWER_CUST:            dialogStd( DIALOG_YES_NO, "FileExists", NULL, NULL, OverWritePowerCustFile, NULL, 0 ); break;
                        case SAVE_METHOD_PRIMARY_POWER_CUST:    dialogStd( DIALOG_YES_NO, "FileExists", NULL, NULL, OverWritePrimaryPowerCustFile, NULL, 0 ); break;
                        case SAVE_METHOD_SECONDARY_POWER_CUST:    dialogStd( DIALOG_YES_NO, "FileExists", NULL, NULL, OverWriteSecondaryPowerCustFile, NULL, 0 ); break;
                        default:
                            break;
                    }
                    fclose(open_test);
                }
                else
                {
                    //Entity *e = playerPtr();
                    saveFile(fname, saveFileMethod);
                    if (saveFileMethod < SAVE_METHOD_PRIMARY_POWER_CUST)
                    {
                        dialogStd( DIALOG_OK, "CostumeSaved", NULL, NULL, NULL, NULL, 0 );
                        start_menu(prevMenu );
                    }
                }
            }
        }
    }
    else
    {
        dialogStd( DIALOG_OK, "InvalidName", NULL, NULL, NULL, NULL, 0 );
    }
}

static int saveCostume_hiddenCode(void)
{
    return 1;
}
static void saveCostume_exitCode(void)
{
    init = 0;
    toggle_3d_game_modes(SHOW_NONE);
}


void saveCostume_menu()
{
    static SMFBlock * textEntry;
    static int zoomstate;

    Entity *e = playerPtr();
    
    int saveFileMethod = (prevMenu == MENU_POWER_CUST) ? SAVE_METHOD_POWER_CUST : SAVE_METHOD_COSTUME;
    int dialog_is_open = (window_getMode(WDW_DIALOG) != WINDOW_DOCKED);

    char * fileStr;
    F32 xposSc, yposSc, textXSc, textYSc, xp, yp;

     AtlasTex * w = atlasLoadTexture("white");
    AtlasTex * end = atlasLoadTexture("checkname_circletip");
    AtlasTex * mid = atlasLoadTexture("checkname_circlemid");
    static HybridElement sButtonSave = {0, "SaveString", NULL, "icon_costume_save"};

    if (getPCCEditingMode() < 0)
        setPCCEditingMode(getPCCEditingMode()*-1);

    if(drawHybridCommon(HYBRID_MENU_NO_NAVIGATION))
        return;

    if( !init )
    {
        init = 1;
        zoomstate = ZOOM_DEFAULT;
        gZoomed_in = 0;
        toggle_3d_game_modes(SHOW_TEMPLATE);

        switch( e->costume->appearance.bodytype )
        {
        case kBodyType_Huge:
            moveAvatar( AVATAR_CENTER_HUGE );
            scaleHero( e, -10.f );
            break;
        case kBodyType_Female:
            moveAvatar( AVATAR_CENTER_FEMALE);
            scaleHero( e, 0.f );
            break;
        case kBodyType_Male:
        default:
            moveAvatar( AVATAR_CENTER_MALE );
            scaleHero( e, -10.f );
            break;
        }

        seqSetState( playerPtrForShell(1)->seq->state, TRUE, STATE_PROFILE ); // freeze the player model
        seqSetState( playerPtrForShell(1)->seq->state, TRUE, STATE_HIGH ); // freeze the player model
    }
    
    if (saveFileMethod != SAVE_METHOD_COSTUME)
    {
        int mouse;
        mouse = drawStdButton( 430, 740, 50000,105, 30, SELECT_FROM_UISKIN( 0x4466ffff, CLR_BASE_RED, CLR_BASE_ROGUE ), "PowerCustSavePrimary", 1.f, 0);
        if (mouse)
        {
            saveFileMethod = SAVE_METHOD_PRIMARY_POWER_CUST;
            if ( D_MOUSEHIT == mouse )
            {
                setupSave(SAVE_METHOD_PRIMARY_POWER_CUST, textEntry->outputString );    
            }
        }
        mouse = drawStdButton( 550, 740, 50000, 105, 30, SELECT_FROM_UISKIN( 0x4466ffff, CLR_BASE_RED, CLR_BASE_GREY ), "PowerCustSaveSecondary", 1.f, 0);
        if (mouse)
        {
            saveFileMethod = SAVE_METHOD_SECONDARY_POWER_CUST;
            if ( D_MOUSEHIT == mouse )
            {
                setupSave(SAVE_METHOD_SECONDARY_POWER_CUST, textEntry->outputString);
            }
        }
    }

    setScalingModeAndOption(SPRITE_Y_UNIFORM_SCALE, SSO_SCALE_TEXTURE);
    calculatePositionScales(&xposSc, &yposSc);
    calculateSpriteScales(&textXSc, &textYSc);
    xp = DEFAULT_SCRN_WD / 2.f;
    yp = 0.f;
    applyPointToScreenScalingFactorf(&xp, &yp);

     drawAvatarControls(xp, 680.f*yposSc, &zoomstate, 0);
    zoomAvatar(false, zoomstate ? avatar_getZoomedInPosition(0,1) : avatar_getDefaultPosition(0,1));

    drawHybridFade(200*xposSc, 195*yposSc, ZOOM_Z, 300, 30, 190, CLR_H_BACK );

    // text entry
    if(!textEntry)
    {
        textEntry = smfBlock_Create();
        smf_SetFlags(textEntry, SMFEditMode_ReadWrite, SMFLineBreakMode_SingleLine, SMFInputMode_AnyTextNoTagsOrCodes, 30, 
            SMFScrollMode_ExternalOnly, SMFOutputMode_StripAllTagsAndCodes, SMFDisplayMode_AllCharacters, SMFContextMenuMode_None, SMAlignment_Left, 0, "costumeFileEdit", -1);
    }

      drawHybridTextEntry( textEntry, 200.f*xposSc, 150.f*yposSc, ZOOM_Z, 300, 30 );

    if( D_MOUSEHIT == drawHybridButton( &sButtonSave, 200.f*xposSc, 255.f*yposSc, ZOOM_Z, 1.f, CLR_WHITE, (dialog_is_open || strlen(textEntry->outputString) == 0) | HB_DRAW_BACKING ) )
        setupSave(saveFileMethod, textEntry->outputString);

    setSpriteScaleMode(SPRITE_XY_NO_SCALE);
    font(&title_18);
    font_outl(0);
    font_color(CLR_WHITE, CLR_WHITE);
    prnt( 50.f*xposSc, 120.f*yposSc, ZOOM_Z, 1.3f*textXSc, 1.3f*textYSc, textStd((saveFileMethod == SAVE_METHOD_COSTUME) ? "SavingCostume" : "SavingPowerCust") );
    font_outl(1);

    fileStr = estrTemp();
    estrConcatf( &fileStr, "%s/", getFolder(saveFileMethod) );
    font(&hybrid_14);
    if(strlen(textEntry->outputString) == 0)
    {
         font(&hybrid_14);
          cprntEx( 50.f*xposSc, 150.f*yposSc, ZOOM_Z, textXSc, textYSc, CENTER_Y, "EnterNameHere" );
    }
    else
        estrConcatf( &fileStr, "%s%s%s", textEntry->outputString, getExtension(saveFileMethod), extraExt(saveFileMethod) );

       prnt( 80.f*xposSc, 200.f*yposSc, ZOOM_Z, textXSc, textYSc, fileStr );
    estrDestroy(&fileStr);

    setScalingModeAndOption(SPRITE_XY_SCALE, SSO_SCALE_ALL);

    if ( drawHybridNext( DIR_LEFT_NO_NAV, 0, "ReturnString" ) )
    {
        start_menu(prevMenu);
    }

    if (getPCCEditingMode() > 0)
        setPCCEditingMode(getPCCEditingMode()*-1);
}