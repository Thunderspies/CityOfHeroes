#include "UI/uiConvertEnhancement.h"
#include "UI/uiWindows.h"
#include "gameComm/wdwbase.h"
#include "UI/uiUtil.h"
#include "UI/uiUtilGame.h"
#include "UI/uiEnhancement.h"
#include "UI/uiCursor.h"
#include "UI/uiInput.h"
#include "UI/uiTray.h"
#include "UI/uiGame.h"
#include "UI/Hybrid/uiHybridMenu.h"
#include "UI/uiComboBox.h"
#include "uiHelpButton.h"
#include "UI/uiBox.h"
#include "UI/uiDialog.h"
#include "UI/uiClipper.h"
#include "player/player.h"
#include "character.h"
#include "entity/character_base.h"
#include "entity/boost.h"
#include "entity/boostset.h"
#include "entity/entity.h"
#include "UI/sprite/sprite_text.h"
#include "UI/sprite/sprite_font.h"
#include "entity/powers.h"
#include <utilitieslib/components/earray.h>
#include <utilitieslib/components/EString.h>
#include "cmdparse/cmdgame.h"
#include "graphics/textureatlas.h"
#include "UI/sprite/sprite_base.h"
#include "uiOptions_Type.h"
#include <utilitieslib/language/MessageStoreUtil.h>
#include "UI/uiNet.h"
#include "UI/uiOptions.h"

#define SPEC_SCALE 0.75f
static int sEnhancementIdx = -1;
static int sWaitingForOutput = 0;
static Boost * sInputBoost;
static Boost * sOutputBoost;
static ComboBox sOutsetTypes;
static const char ** sOutsetList;
static int sInputChanged;
static HelpButton * sInHelp;
static HelpButton * sOutHelp;

static void clearCachedBoosts(void)
{
    sEnhancementIdx = -1;
    if (sInputBoost)
    {
        boost_Destroy(sInputBoost);
    }
    sInputBoost = NULL;
    sWaitingForOutput = 0;
    if (sOutputBoost)
    {
        boost_Destroy(sOutputBoost);
    }
    sOutputBoost = NULL;

    sInputChanged = 1;
}

static void sendBoostConversion(int index, const char * conversionSet)
{
    char * str;
    estrCreate(&str);
    estrConcatf( &str, "boost_convert %d %s", index, conversionSet );
    cmdParse(str);
    estrDestroy(&str);
}

void receiveConvertEnhancement(Packet *pak)
{
    if (pak)
    {
        int crcOldEnhancementName = pktGetBitsAuto(pak);
        int crcNewEnhancementName = pktGetBitsAuto(pak);

        if (crcOldEnhancementName && crcNewEnhancementName)
        {
            if (sWaitingForOutput && sInputBoost->ppowBase->crcFullName == crcOldEnhancementName)
            {
                Entity * e = playerPtr();
                int newEnhancementIndex;

                //find new enhancement
                for (newEnhancementIndex = 0; newEnhancementIndex < CHAR_BOOST_MAX; ++newEnhancementIndex)
                {
                    if (e->pchar->aBoosts[newEnhancementIndex] && e->pchar->aBoosts[newEnhancementIndex]->ppowBase->crcFullName == crcNewEnhancementName)
                    {
                        sOutputBoost = boost_Clone(e->pchar->aBoosts[newEnhancementIndex]);
                        sWaitingForOutput = 0;
                        break;
                    }
                }
                
                if (newEnhancementIndex == CHAR_BOOST_MAX)
                {
                    // can't find the new enhancement
                    clearCachedBoosts();
                }
            }
            //else
                // ui has already been updated with something else, don't replace
        }
        else
        {
            // bad old or new name, error message sent from mapserver
            clearCachedBoosts();
        }
    }
}

static char * sConvertConfirmStr;
static DialogCheckbox confirmDCB[] = { { "HideConvertConfirmPrompt", 0, kUO_HideConvertConfirmPrompt}, };

typedef struct ConvertData
{
    int index;
    const char * conversionSet;
} ConvertData;

static void convertConfirm(ConvertData * data)
{
    sendBoostConversion(data->index, data->conversionSet);
}

static void convertReject(void * data)
{
    sWaitingForOutput = 0;
}

static ConvertData sConvertData;

static void convertConfirmAdapter(void * arg0)
{
    convertConfirm((ConvertData *)arg0);
}

int convertEnhancementWindow()
{
    F32 x, y, z, wd, ht, sc;
    int color, bcolor, inColor, outColor, arrowColor = 0xffee5bff;
    CBox box;
    Entity * e = playerPtr();
    static HybridElement sButtonClear = {0, "ClearString"};
    static HybridElement sButtonConvert = {1, "ConvertEnhancementString"};
    static int inset = 1;
    static int outset = 0;
    static int init = 0;
    int boostMatchesInventory = 0;
    int convertLocked;
    int cost = 0;
    int wallet = character_SalvageCount(e->pchar, salvage_GetItem( "S_EnhancementConverter" ));
    int isDraggingInputValid;
    AtlasTex * converterBar = atlasLoadTexture("EnhancementConverterBar");
    AtlasTex * converterArrow = atlasLoadTexture("EnhancementConverterArrow");
    AtlasTex * converterIcon = atlasLoadTexture("salvage_EnhancementConverter");
    static const char * empty = "";

    if(!window_getDims(WDW_CONVERT_ENHANCEMENT, &x, &y, &z, &wd, &ht, &sc, &color, &bcolor)) 
    {
        return 0;
    }
    drawFrame( PIX3, R10, x, y, z-5, wd, ht, sc, color, bcolor );
    BuildCBox( &box, x, y, wd, ht );

    if (drawSmallCheckboxEx( WDW_CONVERT_ENHANCEMENT, 30.f*sc, 20.f*sc, z, sc, "InSetString", &inset, 0, 1, 0, 0, 1, 0 ))
    {
        inset = 1;
        outset = 0;
        sInputChanged = 1;
    }
    helpButton( &sInHelp, x+180.f*sc, y+20.f*sc, z,sc, "ConvertInSetHelp", &box );

    if (drawSmallCheckboxEx( WDW_CONVERT_ENHANCEMENT, 240.f*sc, 20.f*sc, z, sc, "OutSetString", &outset, 0, 1, 0, 0, 1, 0 ))
    {
        inset = 0;
        outset = 1;
        sInputChanged = 1;
    }
    helpButton( &sOutHelp, x+410.f*sc, y+20.f*sc, z,sc, "ConvertOutSetHelp", &box );

    if (!init)
    {
        init = 1;
        eaCreateConst(&sOutsetList);
        eaPushConst(&sOutsetList, empty);
        combobox_init( &sOutsetTypes, 240.f, 40.f, 0, 175.f, 20.f, 100.f, sOutsetList, WDW_CONVERT_ENHANCEMENT );
    }
    if ((outset == 0) || sOutputBoost || !sInputBoost || sWaitingForOutput)
    {
        sOutsetTypes.state = 0;
        sOutsetTypes.color = CLR_GREY;
        sOutsetTypes.back_color = 0x333333ff;
        sOutsetTypes.highlight_color = CLR_GREY;
    }
    else
    {
        sOutsetTypes.color = CLR_GREEN;
        sOutsetTypes.back_color = CLR_BLACK;
        sOutsetTypes.highlight_color = CLR_GREEN;
    }

    combobox_display( &sOutsetTypes );

    isDraggingInputValid = cursor.dragging &&
        cursor.drag_obj.type == kTrayItemType_SpecializationInventory &&
        cursor.drag_obj.ispec != -1 &&
        e->pchar->aBoosts[cursor.drag_obj.ispec] && // possible null pointer if enhancement disappears before/during drag
        e->pchar->aBoosts[cursor.drag_obj.ispec]->ppowBase->pBoostSetMember &&
        e->pchar->aBoosts[cursor.drag_obj.ispec]->ppowBase->pBoostSetMember->ppchConversionGroups != NULL &&
        e->pchar->aBoosts[cursor.drag_obj.ispec]->ppowBase->pBoostSetMember->ppchConversionGroups[0] != NULL; //check if this boost can be converted

    //check if this is an attuned boost
    if (isDraggingInputValid)
    {
        const ConversionSetCost * pSet = boostset_findConversionSet(e->pchar->aBoosts[cursor.drag_obj.ispec]->ppowBase->pBoostSetMember->ppchConversionGroups[0]);
        isDraggingInputValid = !(e->pchar->aBoosts[cursor.drag_obj.ispec]->ppowBase->bBoostUsePlayerLevel && !pSet->allowAttuned);
    }

    if (isDraggingInputValid)
    {
        if (sWaitingForOutput)
        {
            //prevent movement while waiting
            trayobj_stopDragging();
        }
        else if( mouseCollision(&box) && !isDown(MS_LEFT) )
        {
            clearCachedBoosts();
            sEnhancementIdx = cursor.drag_obj.ispec;
            sInputBoost = boost_Clone(e->pchar->aBoosts[sEnhancementIdx]);
            trayobj_stopDragging();
        }
    }
    if (sInputChanged)
    {
        sInputChanged = 0;
        eaClearConst(&sOutsetList);
        eaSetSizeConst(&sOutsetList, 0);
        sOutsetTypes.currChoice = 0;
        if (sInputBoost && outset)
        {
            int i;
            for (i = 0; i < eaSize(&sInputBoost->ppowBase->pBoostSetMember->ppchConversionGroups); ++i)
            {
                if (boostset_hasValidConversion(e, sInputBoost->ppowBase->pBoostSetMember->ppchConversionGroups[i], sInputBoost->iLevel))
                {
                    eaPushConst(&sOutsetList, sInputBoost->ppowBase->pBoostSetMember->ppchConversionGroups[i]);
                }
            }
            if (!devassertmsg(eaSize(&sOutsetList), "No possible out set conversion available. (Something of at least the same rarity is required.)"))
            {
                //fail gracefully by clearing input and resetting to inset
                clearCachedBoosts();
                eaPushConst(&sOutsetList, empty);
                inset = 1;
                outset = 0;
            }
        }
        else
        {
            eaPushConst(&sOutsetList, empty);
        }
        sOutsetTypes.strings = sOutsetList;
    }
    font(&hybridbold_12);
    font_color(CLR_WHITE, CLR_WHITE);
    prnt(x+70.f*sc, y+160.f*sc, z, sc, sc, "OwnedEnhancementConverters", wallet);
    display_sprite_positional(converterIcon, x+55.f*sc, y+155.f*sc, z, 0.4f*sc, 0.4f*sc, CLR_WHITE, H_ALIGN_CENTER, V_ALIGN_CENTER);

    // update output once we get it
    boostMatchesInventory = sEnhancementIdx > -1 && e->pchar->aBoosts[sEnhancementIdx] && sInputBoost->ppowBase == e->pchar->aBoosts[sEnhancementIdx]->ppowBase;

    //show the previous input.
    //if the conversion has occured, there should be an output boost and in the input boost will no longer exist
    //in the player's inventory.
    if( sInputBoost && (sOutputBoost || boostMatchesInventory ) )
    {
        UIBox uibox;
        if (sInputBoost->pUI == NULL)
            uiEnhancementCreateFromBoost(sInputBoost);

        uiEnhancementDraw( sInputBoost->pUI, x+55.f*sc, y+100.f*sc, z+1.f, sc, sc*2, MENU_GAME, WDW_ENHANCEMENT, true);
        uiBoxDefine(&uibox, x+90.f*sc, y+85.f*sc, 300.f*sc, 80.f*sc);
        font(&hybridbold_12);
        font_color(CLR_WHITE, CLR_WHITE);
        clipperPush(&uibox);
        prnt(x+90.f*sc, y+107.f*sc, z+2.f, sc, sc, sInputBoost->ppowBase->pchDisplayName);
        clipperPop();

        if (inset == 1)
        {
            //find the rarity inset value
            const ConversionSetCost * pConvert = boostset_findConversionSet(sInputBoost->ppowBase->pBoostSetMember->ppchConversionGroups[0]);
            cost = pConvert->tokensInSet;
        }
        else
        {
            const ConversionSetCost * pConvert = boostset_findConversionSet(sOutsetTypes.strings[sOutsetTypes.currChoice]);
            cost = pConvert->tokensOutSet;
        }
        prnt(x+310.f*sc, y+160.f*sc, z, sc, sc, "CostEnhancementConverters", cost);
        display_sprite_positional(converterIcon, x+295.f*sc, y+155.f*sc, z, 0.4f*sc, 0.4f*sc, CLR_WHITE, H_ALIGN_CENTER, V_ALIGN_CENTER);
        inColor = CLR_GREEN;
    }
    else
    {
        if (sInputBoost)
        {
            clearCachedBoosts();
        }
        inColor = CLR_WHITE;
        drawEnhancementFrame( x+55.f*sc, y+100.f*sc, z+5.f, SPEC_SCALE*sc, isDraggingInputValid, 1, inColor );
        arrowColor = CLR_WHITE;
    }

    if( sOutputBoost )
    {
        UIBox uibox;
        if (sOutputBoost->pUI == NULL)
            uiEnhancementCreateFromBoost(sOutputBoost);

        uiEnhancementDraw( sOutputBoost->pUI, x+55.f*sc, y+215.f*sc, z+1.f, sc, sc*2, MENU_GAME, WDW_ENHANCEMENT, true);  
        uiBoxDefine(&uibox, x+90.f*sc, y+200.f*sc, 300.f*sc, 80.f*sc);
        font(&hybridbold_12);
        font_color(CLR_WHITE, CLR_WHITE);
        font_color(CLR_WHITE, CLR_WHITE);
        clipperPush(&uibox);
        prnt(x+90.f*sc, y+222.f*sc, z+2.f, sc, sc, sOutputBoost->ppowBase->pchDisplayName);
        clipperPop();
        outColor = CLR_GREEN;
        inColor = CLR_GREY;
    }
    else
    {
        outColor = CLR_WHITE;
        drawEnhancementFrame( x+55.f*sc, y+215.f*sc, z+5.f, SPEC_SCALE*sc, 0, 1, outColor );
    }
    display_sprite_positional(converterBar, x+wd/2.f+10.f*sc, y+95.f*sc, z, sc, sc, inColor, H_ALIGN_CENTER, V_ALIGN_CENTER);
    display_sprite_positional(converterArrow, x+wd/2.f, y+153.f*sc, z, sc, sc, arrowColor, H_ALIGN_CENTER, V_ALIGN_CENTER);
    display_sprite_positional(converterBar, x+wd/2.f+10.f*sc, y+210.f*sc, z, sc, sc, outColor, H_ALIGN_CENTER, V_ALIGN_CENTER);

    font(&hybridbold_12);
    font_color(CLR_WHITE, CLR_WHITE);
    prnt(x+90.f*sc, y+80.f*sc, z+1.f, sc, sc, "inputString");
    prnt(x+90.f*sc, y+195.f*sc, z+1.f, sc, sc, "outputString");

    if ( D_MOUSEHIT == drawHybridBarButton(&sButtonClear, x+wd/2.f-100.f*sc, y+ht-25.f*sc, z+5.f, 200.f, 0.75f*sc, 0x3399ffff, 0 ))
    {
        clearCachedBoosts();
    }
    convertLocked = sInputBoost && !sOutputBoost && !sWaitingForOutput && cost <= wallet;
    if ( D_MOUSEHIT == drawHybridBarButton(&sButtonConvert, x+wd/2.f+100.f*sc, y+ht-25.f*sc, z+5.f, 200.f, 0.75f*sc, CLR_GREEN, convertLocked ? 0 : HYBRID_BAR_BUTTON_DISABLED ))
    {
        sConvertData.index = sEnhancementIdx;
        if (inset)
        {
            sConvertData.conversionSet = "In";
            estrPrintf(&sConvertConfirmStr, textStd("ConvertConfirmDialogString", textStd("InSetString"), textStd(e->pchar->aBoosts[sEnhancementIdx]->ppowBase->pchDisplayName), cost));
        }
        else
        {
            sConvertData.conversionSet = sOutsetTypes.strings[sOutsetTypes.currChoice];
            estrPrintf(&sConvertConfirmStr, textStd("ConvertConfirmDialogString", textStd("OutSetString"), textStd(e->pchar->aBoosts[sEnhancementIdx]->ppowBase->pchDisplayName), cost));
        }
        sWaitingForOutput = 1;
        if (optionGet(kUO_HideConvertConfirmPrompt))
        {
            convertConfirm(&sConvertData);
        }
        else
        {
            dialog( DIALOG_YES_NO, -1, -1, -1, -1, sConvertConfirmStr, NULL, convertConfirmAdapter, NULL, convertReject, 0 , sendOptions, confirmDCB, 1, 0, 0, &sConvertData );
        }
    }
    // UNCOMMMENT if we are going to sell enhancement converters
    //if (sInputBoost && !sOutputBoost && !sWaitingForOutput && cost > wallet)
    //{
    //    //draw store icon
    //    drawHybridStoreWindowButton( "COBOCONV", "icon_paragonstore", x+wd/2.f+160.f*sc, y+ht-25.f*sc, z+7.f, 0.75f*sc, CLR_WHITE, HB_STORE_PRODUCT, HB_DRAW_BACKING|HB_DO_NOT_EDIT_ELEMENT, "paragonStoreButton", WDW_CONVERT_ENHANCEMENT); //Enhancement Conversion product code
    //}
    return 0;
}