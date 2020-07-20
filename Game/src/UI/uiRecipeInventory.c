/***************************************************************************
 *     Copyright (c) 2005-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/

#include "sound/sound.h"
#include <utilitieslib/utils/file.h>

#include "entity/character_base.h"
#include "entity/character_workshop.h"
#include "entity/character_eval.h"
#include "gameComm/wdwbase.h"
#include "win/win_init.h"
#include "entity/powers.h"
#include "player/player.h"
#include "entity/entity.h"
#include "entity/EntPlayer.h"
#include "character.h"
#include "entity/EntPlayer.h"
#include "entity/entVarUpdate.h"
#include "clientcomm/clientcomm.h"
#include "cmdparse/cmdcommon.h"
#include "entity/entity_power_client.h"
#include "win/input.h"
#include "cmdparse/cmdcommon.h"
#include "cmdparse/cmdgame.h"
#include "UI/sprite/sprite_base.h"
#include "UI/sprite/sprite_text.h"
#include "UI/sprite/sprite_font.h"
#include "graphics/textureatlas.h"
#include <utilitieslib/utils/mathutil.h>
#include "entity/character_inventory.h"
#include "entity/character_workshop.h"
#include "entity/character_level.h"
#include "bases/DetailRecipe.h"
#include "entity/Supergroup.h"
#include <utilitieslib/language/MessageStore.h>

#include <utilitieslib/utils/strings_opt.h>
#include <utilitieslib/components/Earray.h>
#include "formatter/smf_main.h"
#include "graphics/groupThumbnail.h"
#include "bases/bases.h"
#include "bases/basedata.h"
#include "UI/uiTabControl.h"
#include "UI/uiComboBox.h"
#include <utilitieslib/language/MessageStoreUtil.h>
#include <utilitieslib/components/HashFunctions.h>
#include "entity/boostset.h"
#include <utilitieslib/components/StashTable.h>
#include "entity/rewardtoken.h"
#include "storyarc/playerCreatedStoryarc.h"

#include "UI/uiCursor.h"
#include "UI/uidialog.h"
#include "UI/uiUtil.h"
#include "UI/uiUtilGame.h"
#include "UI/uiWindows.h"
#include "UI/uiTray.h"
#include "UI/uiCursor.h"
#include "UI/uiToolTip.h"
#include "UI/uiGame.h"
#include "UI/uiNet.h"
#include "UI/uiContextMenu.h"
#include "UI/uiWindows_init.h"
#include "UI/uiScrollBar.h"
#include "UI/uiInput.h"
#include "UI/uiOptions.h"
#include "uiGift.h"
#include "uiCombineSpec.h"
#include "UI/uiEnhancement.h"
#include "uiTree.h"
#include "UI/uiInfo.h"
#include "uiSlider.h"
#include "UI/uiRecipeInventory.h"
#include "UI/uiIncarnate.h"
#include <utilitieslib/utils/SimpleParser.h>

static bool s_bReparse = 0;

static int s_Hide;
static int s_Empowerment; //Hack for now 
static int s_InventoryMode; 
static int s_useCoupon = false;
static int gSplitPane = false;
static int rewardLevel = 1;

static ContextMenu * s_WorkshopRecipeContext = 0;
static ContextMenu * s_WorkshopRecipeNoGiftContext = 0;

static StashTable haTabNames = NULL;
static StashTable haRecipeNames = NULL;

static TextAttribs s_taDefaults =
{
    /* piBold        */  (int *)0,
    /* piItalic      */  (int *)0,
    /* piColor       */  (int *)0xffffffff,
    /* piColor2      */  (int *)0,
    /* piColorHover  */  (int *)0xffffffff,
    /* piColorSelect */  (int *)0,
    /* piColorSelectBG*/ (int *)0x333333ff,
    /* piScale       */  (int *)(int)(1.0f*SMF_FONT_SCALE),
    /* piFace        */  (int *)&smfSmall,
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

enum {
    HIDE_MISSING_SALVAGE = 1,
    HIDE_WRONG_TABLE = 2,
    HIDE_DETAIL = 4,
    HIDE_COMPONENT = 8,
    HIDE_MISSING_RECIPE = 16,
};

#define SPACE_BETWEEN_NODES        10.0f
#define INDENT_SPACE            10.0f


////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
// R E C I P E     F U N C T I O N S    ( N O R M A L )
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////


uiTreeNode *recipeFindChildNode(uiTreeNode *pRoot, const char *name)
{
    int i, count = eaSize(&pRoot->children);

    for (i = 0; i < count; i++)
    {
        if (stricmp((char *) pRoot->children[i]->pData, name) == 0)
            return pRoot->children[i];
    }
    return NULL;
}


////////////////////////////////////////////////////////////////////////////////////////////
// S O R T I N G   &   F I L T E R I N G
////////////////////////////////////////////////////////////////////////////////////////////
static void updateSort(ComboBox *hideBox)
{
    int newhide = 0;
    newhide |= combobox_idSelected(hideBox, HIDE_MISSING_SALVAGE) * HIDE_MISSING_SALVAGE;
    newhide |= combobox_idSelected(hideBox, HIDE_MISSING_RECIPE) * HIDE_MISSING_RECIPE;

    if (s_InventoryMode)
    {
        optionSet(kUO_RecipeHideMissingParts, (combobox_idSelected(hideBox, HIDE_MISSING_SALVAGE) != 0), 0 );
        optionSet(kUO_RecipeHideUnowned, (combobox_idSelected(hideBox, HIDE_MISSING_RECIPE) != 0), 1 );
    } 
    else 
    {
        optionSet(kUO_RecipeHideMissingPartsBench, (combobox_idSelected(hideBox, HIDE_MISSING_SALVAGE) != 0), 0 );
        optionSet(kUO_RecipeHideUnownedBench, (combobox_idSelected(hideBox, HIDE_MISSING_RECIPE) != 0), 1 );
    }

    newhide |= HIDE_WRONG_TABLE;
    s_Hide = newhide;        

    recipeInventoryReparse();
}

////////////////////////////////////////////////////////////////////////////////////////////
// R E C I P E   M O V I N G  &   D R A G G I N G
////////////////////////////////////////////////////////////////////////////////////////////

void recipe_drag(int index, AtlasTex *icon, AtlasTex *overlay, float scale) 
{
    TrayObj to;
    to.type = kTrayItemType_Recipe;
    to.invIdx = index;
    to.scale = scale;
    trayobj_startDraggingEx(&to, icon, overlay, scale);
}

////////////////////////////////////////////////////////////////////////////////////////////
// R E C I P E   S O R T I N G  &  F I L T E R I N G
////////////////////////////////////////////////////////////////////////////////////////////

static const char *getRecipeTabName(const DetailRecipe *pRec)
{
    StashElement elt;

    if (haTabNames == NULL)
        haTabNames = stashTableCreateWithStringKeys( 128, StashDeepCopyKeys );

    stashFindElement( haTabNames, pRec->pchDisplayTabName, &elt);

    if(!elt)
    {
        char *pTranslated = strdup(textStd(pRec->pchDisplayTabName));
        stashAddPointerAndGetElement(haTabNames, pRec->pchDisplayTabName, pTranslated, false, &elt);
    }

    return stashElementGetPointer(elt);
}


static const char *getRecipeDisplayName(const DetailRecipe *pRec)
{
    StashElement elt;

    if (haRecipeNames == NULL)
        haRecipeNames = stashTableCreateWithStringKeys( 128, StashDeepCopyKeys );

    stashFindElement( haRecipeNames, pRec->ui.pchDisplayName, &elt);

    if(!elt)
    {
        char *pTranslated = strdup(textStd(pRec->ui.pchDisplayName));
        stashAddPointerAndGetElement(haRecipeNames, pRec->ui.pchDisplayName, pTranslated, false, &elt);
    }

    return stashElementGetPointer(elt);
}


static int compareRecipe(const DetailRecipe** a, const DetailRecipe** b)
{  
//    static char temp[256];
//    char temp2[256];
    int result;

    if ((*a)->Type > (*b)->Type)
        return -1;
    if ((*a)->Type < (*b)->Type)
        return 1;

    if (!(*a)->pchDisplayTabName)
        return -1;
    if (!(*b)->pchDisplayTabName)
        return 1;

//    strcpy_s(temp, 256, textStd((*a)->pchDisplayTabName));
//    strcpy_s(temp2, 256,textStd((*b)->pchDisplayTabName));
//    result = stricmp( temp,temp2 );
    result = stricmp( getRecipeTabName(*a), getRecipeTabName(*b) );
    if (result)
        return result;

    /*
    strcpy_s(temp, 256, textStd((*a)->ui.pchDisplayName));
    strcpy_s(temp2, 256,textStd((*b)->ui.pchDisplayName));
    result = stricmp( temp, temp2 );
    if (result)
        return result;

    result = stricmp( (*a)->pchName, (*b)->pchName );
    if (result)
        return result;
    */

    return (*a)->id - (*b)->id;
}

static int recipeIsVisible(const DetailRecipe *pRecipe) 
{
    Entity *e = playerPtr(); 

    // check for level requirements
    if (gSplitPane && ((pRecipe->level > 0 && rewardLevel != pRecipe->level && !(pRecipe->flags & RECIPE_FIXED_LEVEL)) ||
        (pRecipe->levelMin > 0 && rewardLevel < pRecipe->levelMin) ||
        (pRecipe->levelMax > 0 && rewardLevel > pRecipe->levelMax)))
        return false;

    if ((!(s_Hide & HIDE_MISSING_SALVAGE) || character_DetailRecipeCreatable(e->pchar, pRecipe, NULL, false, s_useCoupon, 0)) && 
        (!(s_Hide & HIDE_MISSING_RECIPE) || pRecipe->Type != kRecipeType_Drop || (e->pchar && character_RecipeCount(e->pchar, pRecipe) > 0)))
        return true;

/*
    (!(s_Hide & HIDE_MISSING_SALVAGE) || recipeDisplayStates[i]->have_needed) && 
        (!(s_Hide & HIDE_DETAIL) || !detailRecipes[i]->pDetail) &&
        (!(s_Hide & HIDE_COMPONENT) || detailRecipes[i]->pDetail) &&
        ((!(s_Hide & HIDE_WRONG_TABLE) && (recipeDisplayStates[i]->special || recipeDisplayStates[i]->recipe->Personal)) || recipeDisplayStates[i]->usable_here))
*/
    return false;
}


////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
// R E C I P E    C O N T E X T   M E N U 
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

ContextMenu * s_uiTreeRecipeContext = 0;

/////////////////////////////////////////////////////////////////////////////////

const DetailRecipe *deleteme_recipe;

static DialogCheckbox deleteDCB[] = { { "HideDeleteRecipePrompt", 0, kUO_HideDeleteRecipePrompt } };

static void recipe_delete(void * data)
{
    Entity  * e = playerPtr();
    if(!deleteme_recipe)
        return;

    sendRecipeDelete( deleteme_recipe->pchName, 1 );
}

static void cmrecipe_Delete( void * data )
{
    Entity  * e = playerPtr();
    TrayObj *to = (TrayObj *) data;
    deleteme_recipe = detailrecipedict_RecipeFromId(to->invIdx);

    if (optionGet(kUO_HideDeleteRecipePrompt))
        recipe_delete(0);
    else
        dialog( DIALOG_YES_NO, -1, -1, -1, -1, "ReallyDeleteRecipe", NULL, recipe_delete, NULL, NULL,0,sendOptions,deleteDCB,1,0,0,0 );
    sndPlay( "Reject5", SOUND_GAME );
}

static void cmrecipe_Info( void * data )
{
    TrayObj *to = (TrayObj *) data;
    const DetailRecipe *pRec = detailrecipedict_RecipeFromId(to->invIdx);
    if(pRec)
        info_Recipes( pRec );
}

/////////////////////////////////////////////////////////////////////////////////

static const char *cmsalvage_Name( void * data )
{
    TrayObj *to = (TrayObj *) data;
    const DetailRecipe *pRec = detailrecipedict_RecipeFromId(to->invIdx);
    return pRec ? pRec->ui.pchDisplayName : "Unknown";
}


/////////////////////////////////////////////////////////////////////////////////
static void recipe_sell(void * data)
{
    Entity  * e = playerPtr();
    if(!deleteme_recipe)
        return;

    sendRecipeSell( deleteme_recipe->pchName, 1 );
}

static void cmrecipe_Sell( void * data )
{
    Entity  * e = playerPtr();
    TrayObj *to = (TrayObj *) data;
    deleteme_recipe = detailrecipedict_RecipeFromId(to->invIdx);

    dialog( DIALOG_YES_NO, -1, -1, -1, -1, "ReallySellRecipe", NULL, recipe_sell, NULL, NULL,0,sendOptions,NULL,1,0,0,0 );
    sndPlay( "Reject5", SOUND_GAME );
}

static int cmrecipe_canSell( void * data )
{
    TrayObj *to = (TrayObj *) data;
    deleteme_recipe = detailrecipedict_RecipeFromId(to->invIdx);

    if (deleteme_recipe == NULL )
        return CM_HIDE;

    if (deleteme_recipe->SellToVendor > 0)
        return CM_AVAILABLE;
    else 
        return CM_HIDE;
} 


/////////////////////////////////////////////////////////////////////////////////

static void initUiTreeRecipeContext()
{
    s_uiTreeRecipeContext = contextMenu_Create(NULL);

    contextMenu_addVariableTitle( s_uiTreeRecipeContext, cmsalvage_Name, 0);
    contextMenu_addCode( s_uiTreeRecipeContext, alwaysAvailable, 0, cmrecipe_Info, 0, "CMInfoString", 0  );
    contextMenu_addCode( s_uiTreeRecipeContext, alwaysAvailable, 0, cmrecipe_Delete, 0, "CMRemoveRecipe", 0  );
//    contextMenu_addCode( s_uiTreeRecipeContext, cmrecipe_canSell, 0, cmrecipe_Sell, 0, "CMSellRecipe", 0  );
    gift_addToContextMenu(s_uiTreeRecipeContext);
}

void uiTreeRecipeContextMenu(treeRecipeDisplayState *rd, int width)
{
    int mx, my;
    static TrayObj to;

    if(!s_uiTreeRecipeContext)
        initUiTreeRecipeContext();

    to.type = kTrayItemType_Recipe;
    to.invIdx = rd->recipe->id;

    rightClickCoords(&mx,&my);
    contextMenu_setAlign( s_uiTreeRecipeContext, mx, my, CM_LEFT, CM_TOP, &to);
}

////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
// R E C I P E     D I S P L A Y    ( N O R M A L )
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

ToolTip recipeTip;
static void recipeSetReformat(uiTreeNode *pNode);

AtlasTex * uiRecipeDrawIconEx(const DetailRecipe *pRec, int level, float x, float y, float z, float scale, int window, int showTooltip, int rgba, float tooltipOffsetY, 
                              int bDrawEnhancementsAsEnhancements)
{
    float iconSize = 60.0f*scale;
    AtlasTex *pIcon = NULL;
    static uiEnhancement *pTooltipEnh = NULL;
    uiEnhancement *pEnh = NULL;

    if (pRec->pchEnhancementReward != NULL || pRec->pchRecipeReward != NULL)
    {
        // draw enhancement here!
        pEnh = uiEnhancementCreateFromRecipe(pRec, level, bDrawEnhancementsAsEnhancements);
        if (pEnh)
        {
            uiEnhancementSetColor(pEnh, rgba);

            // magic code to reformat recipes if a frame is going to be displayed
            if (scale*0.75f > 1.0f)
                pIcon = uiEnhancementDrawEx(pEnh, x + 35.f*scale, y + 35.f*scale, z, scale*0.75f, scale*0.75f, MENU_GAME, window, showTooltip, false, tooltipOffsetY);
            else 
                pIcon = uiEnhancementDrawEx(pEnh, x + 35.f*scale, y + 35.f*scale, z, scale*0.75f, scale*1.2f, MENU_GAME, window, showTooltip, false, tooltipOffsetY);

            if (pEnh->isDisplayingToolTip)
            {
                uiEnhancementFree(&pTooltipEnh);
                pTooltipEnh = pEnh;
            }
            else
            {
                uiEnhancementFree(&pEnh);
            }
        } else {
            if(pRec->ui.pchIcon)
            {
                pIcon = atlasLoadTexture(pRec->ui.pchIcon);
                if (pIcon)
                    display_sprite(pIcon, x, y, z, iconSize*scale/pIcon->width, iconSize*scale/pIcon->height, rgba );
            }
        }
    }
    else 
    { 
        if(pRec->ui.pchIcon)
        {
            pIcon = atlasLoadTexture(pRec->ui.pchIcon);
                        
            if (pIcon)
            {
                float yOffset = 0.f; 
                float xOffset = 0.f; 
                CBox box;        
                if (stricmp(pRec->ui.pchIcon, "e_icon_gen_inv_power.tga") == 0 ||
                    stricmp(pRec->ui.pchIcon, "e_icon_gen_inv_costume.tga") == 0 ||
                    stricmp(pRec->ui.pchIcon, "temporary_raiddeploypylon.tga") == 0
                    )
                {
                    AtlasTex *frameL = atlasLoadTexture( "E_origin_recipe_schematic.tga" );
                    
                    scale = scale*0.75f;
                    yOffset = 14.5f*scale; 
                    xOffset = 14.5f*scale; 

                    display_sprite( frameL, x + xOffset, y + yOffset,  z,   scale, scale, rgba );

                    if (stricmp(pRec->ui.pchIcon, "temporary_raiddeploypylon.tga") == 0)
                        scale = scale/0.75f;

                     display_sprite(pIcon, x + xOffset + 8*scale, y + yOffset + 8*scale, z+0.1f, scale, scale, rgba );
                      BuildCBox(&box, x + 8*scale, y + 8*scale, pIcon->width*scale*2, pIcon->height*scale*1.5);
                } 
                else 
                { 
                    display_sprite(pIcon, x + 8*scale, y + 8*scale, z, scale, scale, rgba );
                    BuildCBox(&box, x + 8*scale, y + 8*scale, pIcon->width*scale, pIcon->height*scale);
                }

                // hmmm
                if( mouseCollision(&box))
                 {
                    StuffBuff tooltiptext;
                    initStuffBuff(&tooltiptext, 1024);

                    // name
                    addStringToStuffBuff(&tooltiptext,"<color InfoBlue><b>%s</b></color><p>", textStd(pRec->ui.pchDisplayName)); 

                    // short help
                    if( pRec->ui.pchDisplayHelp )
                        addStringToStuffBuff(&tooltiptext,"<color paragon>%s</color><p>", textStd(pRec->ui.pchDisplayHelp));

                    if( pRec->pchPowerReward )
                    {
                        const BasePower *pPower = powerdict_GetBasePowerByFullName(&g_PowerDictionary, pRec->pchPowerReward);
                        if( pPower && pPower->pchDisplayHelp)
                            addStringToStuffBuff(&tooltiptext,"<color white>%s</color><p>", textStd(pPower->pchDisplayHelp) );
                    }

                    {
                        SalvageInventoryItem    **components;
                        int have_needed;

                        eaCreate(&components);
                        have_needed = character_DetailRecipeCreatable(playerPtr()->pchar, pRec, &components, false, false, -1);
                        uiRecipeGetRequirementText(&tooltiptext, pRec, 1, components, &have_needed, "Paragon", "White", "Red");
                        eaDestroy(&components);
                    }

                    addStringToStuffBuff(&tooltiptext, "<br>");
                    addToolTip( &recipeTip );
                    forceDisplayToolTip( &recipeTip, &box, tooltiptext.buff, MENU_GAME, window );
                }
            }
        }
    }

    return pIcon;
}

void uiRecipeGetRequirementText(StuffBuff *text, const DetailRecipe *dr, int count, SalvageInventoryItem **components, 
                                int *have_needed, char *blueColor, char *whiteColor, char *redColor)
{
    Entity *e = playerPtr();
    int i;
    int sizeSalReq = eaSize(&dr->ppSalvagesRequired);
    int powerComponentCount = eaSize(&dr->ppPowersRequired);
    int cost = 0;

    if (!text || !dr || !have_needed || !components || !whiteColor || !redColor || !e || !e->pchar)
        return;

    if (dr->CreationCost != NULL)
        cost = chareval_Eval(e->pchar, dr->CreationCost, dr->pchSourceFile);

    if (s_useCoupon)
        cost = (int) ((float) cost * INVENTION_COUPON_DISCOUNT);

    // list requirements
    //        addStringToStuffBuff(&pState->text,"<br><table><tr border=0><td><b>%s</b></td>",textStd("UiInventoryTypeSalvage"));
    addStringToStuffBuff(text,"<br><table><tr border=0><td></td>");
    addStringToStuffBuff(text,"<td width=25%% align=right><color %s><b>%s</b></color></td>", whiteColor, textStd("NeedString"));
    addStringToStuffBuff(text,"<td width=25%% align=right><color %s><b>%s</b></color></td></tr>",whiteColor, textStd("HaveString"));

    // check for influence cost
    if (cost > 0)
    {
        char costCommaStr[50] = { 0 };
        char infCommaStr[50] = { 0 };
        char *currencyStr = NULL;

        sprintf(costCommaStr, "%s", textStd( "InfluenceWithCommas", cost));
        sprintf(infCommaStr, "%s", textStd( "InfluenceWithCommas", e->pchar->iInfluencePoints));

        switch (skinFromMapAndEnt(e))
        {
            case UISKIN_HEROES:
            default:
                currencyStr = textStdEx(TRANSLATE_NOPREPEND, "InfluenceString");
                break;
            case UISKIN_VILLAINS:
                currencyStr = textStdEx(TRANSLATE_NOPREPEND, "V_InfluenceString");
                break;
            case UISKIN_PRAETORIANS:
                currencyStr = textStdEx(TRANSLATE_NOPREPEND, "InfluencePraetoria");
                break;
        }

        if (cost > e->pchar->iInfluencePoints) {
            addStringToStuffBuff(text,"<color %s><tr border=0><td>%s</td><td width=20%% align=right>%s</td><td width=20%% align=right>%s</td></tr></color>", redColor, currencyStr, costCommaStr, infCommaStr);
        } else {
            addStringToStuffBuff(text,"<color %s><tr border=0><td>%s</td><td width=20%% align=right>%s</td><td width=20%% align=right>%s</td></tr></color>", whiteColor, currencyStr, costCommaStr, infCommaStr);
        }
    }

    for (i = 0; i < powerComponentCount; i++)
    {
        unsigned int curcount = 0;
        const PowerRequired *powReq = dr->ppPowersRequired[i];
        // ignores powReq->amount
        if (!powReq || !powReq->pPower)
        {
            continue;
        }

        if (character_OwnsPower(e->pchar, powReq->pPower))
        {
            curcount = 1;
        }

        if (curcount < 1)
        {
            addStringToStuffBuff(text,"<color %s><tr border=0><td><a href='cmd:link_info %s'><b><color %s>[%s]</color></b></a></td><td width=20%% align=right>%d</td><td width=20%% align=right>%d</td></tr></color>", redColor, textStd(powReq->pPower->pchDisplayName), redColor, textStd(powReq->pPower->pchDisplayName), 1, curcount);
        }
        else 
        {
            addStringToStuffBuff(text,"<color %s><tr border=0><td><a href='cmd:link_info %s'><b><color %s>[%s]</color></b></a></td><td width=20%% align=right>%d</td><td width=20%% align=right>%d</td></tr></color>", blueColor, textStd(powReq->pPower->pchDisplayName), blueColor, textStd(powReq->pPower->pchDisplayName), 1, curcount);
        }
    }

    for(i = 0; i < sizeSalReq; i++)
    {
        unsigned int curcount = 0;
        const SalvageRequired *s = dr->ppSalvagesRequired[i];
        if (!s || !s->pSalvage) {
            continue;
        }
        if (components[i]) 
            curcount = components[i]->amount;

        if (curcount < s->amount) 
        {
            addStringToStuffBuff(text,"<color %s><tr border=0><td>%s</td><td width=20%% align=right>%d</td><td width=20%% align=right>%d</td></tr></color>", redColor, textStd(s->pSalvage->ui.pchDisplayName),s->amount,curcount);
        }
        else 
            addStringToStuffBuff(text,"<color %s><tr border=0><td>%s</td><td width=20%% align=right>%d</td><td width=20%% align=right>%d</td></tr></color>", whiteColor, textStd(s->pSalvage->ui.pchDisplayName),s->amount,curcount);
    }

    // add additional requirements here
    for (i = 0; i < eaSize(&dr->pchAdditionalComponents); i++)
    {
        if (stricmp(dr->pchAdditionalComponents[i], "raidpoints") == 0)
        {
            int amount = atoi(dr->pchAdditionalComponents[++i]);

            if (e->supergroup != NULL) 
            {
                int idx = rewardtoken_IdxFromName( &e->supergroup->rewardTokens, "raidpoints" );

                if (EAINRANGE(idx, e->supergroup->rewardTokens))
                {
                    if (e->supergroup->rewardTokens[idx]->val >= amount && sgroup_hasPermissionByName( e, "SgPermissionAllowedToSpendRaidPoints"))
                    {
                        addStringToStuffBuff(text,"<color %s><tr border=0><td>%s</td><td width=20%% align=right>%d</td><td width=20%% align=right>%d</td></tr></color>", whiteColor, textStd("RaidPointsString"),amount, e->supergroup->rewardTokens[idx]->val);
                    } else {
                        addStringToStuffBuff(text,"<color %s><tr border=0><td>%s</td><td width=20%% align=right>%d</td><td width=20%% align=right>%d</td></tr></color>", redColor, textStd("RaidPointsString"),amount, e->supergroup->rewardTokens[idx]->val);
                    }
                } else {
                    addStringToStuffBuff(text,"<color %s><tr border=0><td>%s</td><td width=20%% align=right>%d</td><td width=20%% align=right>%d</td></tr></color>", redColor, textStd("RaidPointsString"),amount,0);
                }
            } else {
                addStringToStuffBuff(text,"<color %s><tr border=0><td>%s</td><td width=20%% align=right>%d</td><td width=20%% align=right>%d</td></tr></color>", redColor, textStd("RaidPointsString"),amount,0);
            }
        } 
        else if (stricmp(dr->pchAdditionalComponents[i], "prestige") == 0)
        {
            int amount = atoi(dr->pchAdditionalComponents[++i]);

            if (e->supergroup != NULL) 
            {
                if (e->supergroup->prestige >= amount && sgroup_hasPermissionByName( e, "SgPermissionAllowedToSpendRaidPoints"))
                {
                    addStringToStuffBuff(text,"<color %s><tr border=0><td>%s</td><td width=20%% align=right>%d</td><td width=20%% align=right>%d</td></tr></color>", whiteColor, textStd("PrestigeString"),amount, e->supergroup->prestige);
                } else {
                    addStringToStuffBuff(text,"<color %s><tr border=0><td>%s</td><td width=20%% align=right>%d</td><td width=20%% align=right>%d</td></tr></color>", redColor, textStd("PrestigeString"),amount, e->supergroup->prestige);
                }
            } else {
                addStringToStuffBuff(text,"<color %s><tr border=0><td>%s</td><td width=20%% align=right>%d</td><td width=20%% align=right>%d</td></tr></color>", redColor, textStd("PrestigeString"),amount, 0);
            }
        } else {
            // unknown
        }
    }
    

    addStringToStuffBuff(text,"</table>");

}


void uiTreeRecipeReformat(treeRecipeDisplayState *pState, int bExpanded, float scale)
{
    Entity                    *e = playerPtr();
    const DetailRecipe        *dr = pState->recipe;
    int                        canAfford = (e->pchar->iInfluencePoints < dr->BuyFromVendor);
    char                    *whiteColor = "White";
    char                    *redColor = "Red";
    char                    *blueColor = "Paragon";
    SalvageInventoryItem    **components = NULL;

    if (dr == NULL) // This should not happen
        return;

    pState->count = character_RecipeCount(e->pchar, dr);
 
    if (pState->count <= 0 && 
        dr->Type != kRecipeType_Workshop && 
        dr->Type != kRecipeType_Memorized)
    {
        whiteColor = "Gray";
        redColor = "#FF3030";
    }

    eaCreate(&components);
    pState->have_needed = character_DetailRecipeCreatable(e->pchar, dr, &components, false, s_useCoupon, -1);

    // BUY HACK
    if (!pState->have_needed)
    {
        whiteColor = "Gray";
        redColor = "#FF3030";
    }

    // free old text 
    if (pState->text.size > 0) 
        freeStuffBuff(&pState->text);
    initStuffBuff(&pState->text, 1024);

//    if (dr->BuyFromVendor > 0 && (e->pchar->iInfluencePoints > dr->BuyFromVendor))
//        pState->have_needed &= true;

    switch (pState->recipe->Rarity) { 
            case kRecipeRarity_Common:
                if (pState->have_needed)
                    addStringToStuffBuff(&pState->text,"<font color=White scale=%f>", 1.1*scale);
                else
                    addStringToStuffBuff(&pState->text,"<font color=Gray scale=%f>", 1.1*scale);
                break;
            case kRecipeRarity_Uncommon:
                if (pState->have_needed)
                    addStringToStuffBuff(&pState->text,"<font color=#FFFF00 scale=%f>", 1.1*scale);
                else
                    addStringToStuffBuff(&pState->text,"<font color=#909000 scale=%f>", 1.1*scale);
                break;
            case kRecipeRarity_Rare:
                if (pState->have_needed)
                    addStringToStuffBuff(&pState->text,"<font color=#FF5500 scale=%f>", 1.1*scale);
                else
                    addStringToStuffBuff(&pState->text,"<font color=#774400 scale=%f>", 1.1*scale);
                break;
            case kRecipeRarity_VeryRare:
                if (pState->have_needed)
                    addStringToStuffBuff(&pState->text,"<font color=#A082DC scale=%f>", 1.1*scale);
                else
                    addStringToStuffBuff(&pState->text,"<font color=#70429C scale=%f>", 1.1*scale);
                break;
    }

    if (dr->level > 0 && !(dr->flags & RECIPE_NO_LEVEL_DISPLAYED)) 
        addStringToStuffBuff(&pState->text,"%s [%s %d]</font>",textStd(dr->ui.pchDisplayName), textStd("PowerLevel"), dr->level);
    else 
        addStringToStuffBuff(&pState->text,"%s</font>",textStd(dr->ui.pchDisplayName));

    if (bExpanded) 
    {

        if (!s_InventoryMode && dr->BuyFromVendor > 0)
        {
            if (e->pchar->iInfluencePoints < dr->BuyFromVendor)
                addStringToStuffBuff(&pState->text, "<color red>");

            addStringToStuffBuff(&pState->text, "<br>%s %s", textStd("PriceString"), textStd("InfluenceWithCommas", dr->BuyFromVendor));

            if (e->pchar->iInfluencePoints < dr->BuyFromVendor)
                addStringToStuffBuff(&pState->text, "</color>");
        }

        // if not an enhancement, display the help text for the recipe to let the players know what they are getting
        if (dr->pchEnhancementReward == NULL && dr->pchRecipeReward == NULL && dr->ui.pchDisplayHelp)
        {
            addStringToStuffBuff(&pState->text, "<br><color %s>%s</color>", whiteColor, textStd(dr->ui.pchDisplayHelp));
        }

        // add requirements here
        uiRecipeGetRequirementText(&pState->text, dr, pState->count, components, &pState->have_needed, blueColor, whiteColor, redColor);

        // check for full inventories
        if (dr->creates_salvage)
        {
            if (e->pchar->salvageInvCurrentCount + dr->creates_salvage > character_GetInvTotalSize(e->pchar, kInventoryType_Salvage))
            {
                addStringToStuffBuff(&pState->text,"<br><color %s>%s</color>", redColor, textStd("UiInventoryNoInventorySpaceSalvage"));
            }
        }
        if (dr->creates_enhancement)
        {
            if (dr->creates_enhancement > character_BoostInventoryEmptySlots(e->pchar))
            {
                addStringToStuffBuff(&pState->text,"<br><color %s>%s</color>", redColor, textStd("UiInventoryNoInventorySpaceEnhancement"));
            }
        }
        if (dr->creates_recipe)
        {
            if (e->pchar->recipeInvCurrentCount + dr->creates_recipe > character_GetInvTotalSize(e->pchar, kInventoryType_Recipe))
            {
                addStringToStuffBuff(&pState->text,"<br><color %s>%s</color>", redColor, textStd("UiInventoryNoInventorySpaceRecipe"));
            }
        }
        if (dr->creates_inspiration)
        {
            if (dr->creates_inspiration > character_InspirationInventoryEmptySlots(e->pchar))
            {
                addStringToStuffBuff(&pState->text,"<br><color %s>%s</color>", redColor, textStd("UiInventoryNoInventorySpaceInspiration"));
            }
        }
        if (dr->ppchCreateRequires != NULL && e->pchar != NULL)
        {
            if (!chareval_Eval(e->pchar, dr->ppchCreateRequires, dr->pchSourceFile))
            {
                addStringToStuffBuff(&pState->text,"<br><color %s>%s</color>", redColor, textStd(dr->pchDisplayCreateRequiresFail));
            }
        }
        // check for license
        if (dr->flags & RECIPE_INVENTION_LIC_REQUIRED)
        {
            // HYBRID : Verify invention license product code
            if (!AccountHasStoreProduct(ent_GetProductInventory( e ), SKU("coliinvn")))
                addStringToStuffBuff(&pState->text,"<br><color %s>%s</color>", redColor, textStd("UiInventoryNoInventionLicense"));
        }
    }

    eaDestroy(&components);
    pState->reformat = false;

}

#define ICON_WD 48

float uiTreeRecipeDrawCallback(uiTreeNode *pNode, float x, float y, float z, float width, 
                                    float scale, int display)
{ 
    float spaceBetweenNodes = SPACE_BETWEEN_NODES;
    int textHeight;
    CBox box, buttonBox, buyButtonBox;
    int box_color = CLR_NORMAL_BACKGROUND;
    int forground_color = CLR_NORMAL_FOREGROUND;
    treeRecipeDisplayState *pState = (treeRecipeDisplayState *) pNode->pData;
    float iconSize = 60.0f*scale;

    if ( !(pNode->state&UITREENODE_EXPANDED) )
        iconSize = 0.0f;

    s_taDefaults.piScale = (int*)((int)(0.85f*scale*SMF_FONT_SCALE));

    if (pState->reformat)
    {
        uiTreeRecipeReformat(pState, pNode->state&UITREENODE_EXPANDED, scale);
        if (pState->pEnhancement)
            uiEnhancementRefresh(pState->pEnhancement);
    }

    if (!pState->have_needed)
    {
        if (0)
        {
            forground_color = CLR_NORMAL_BACKGROUND;
        } else {
//            box_color = CLR_CONSTRUCT_EX(255, 255, 255, 0.12); 
            forground_color = CLR_CONSTRUCT_EX(255, 255, 255, 0.04); 
        }
    }

    if (display) 
    {
        int xOffset = 15;

        if (pNode->state&UITREENODE_EXPANDED)
            xOffset = 20;

        textHeight = smf_ParseAndDisplay(pState->recipeFormat, pState->text.buff, 
                                            x+(10+xOffset)*scale+iconSize, y+1*scale, z, width-(20+xOffset)*scale-iconSize, 0,    
                                            false, false, &s_taDefaults, NULL, 0, 0);
        if (pState->count > 0)
        {
            font( &game_9 );
            font_color(0x00deffff, 0x00deffff);
            prnt( x+10*scale, y+16*scale, z+0.1f, scale, scale, "%d", pState->count);  
        } 
        else if (pNode->state&UITREENODE_EXPANDED && pState->count <= 0 && pState->recipe->BuyFromVendor > 0) // BUY HACK
        {
            font( &game_9 );
            font_color(0xcc0000ff, 0xcc0000ff);
            prnt( x+10*scale, y+16*scale, z+0.1f, scale, scale, "%d", pState->count);  
        }

        // create button if opened
        if (pNode->state&UITREENODE_EXPANDED && !s_InventoryMode && 
            ((pState->recipe->Type == kRecipeType_Drop && pState->count > 0) ||
            (pState->recipe->Type == kRecipeType_Workshop) ||
            (pState->recipe->Type == kRecipeType_Memorized))) 
        {
            if(D_MOUSEHIT == drawStdButton(x + 42*scale, y + textHeight-10*scale, (float)z, 60*scale, 20*scale, 
                window_GetColor(WDW_RECIPEINVENTORY), "CreateString", scale, 
                !pState->have_needed)) 
            {
                    Entity *e = playerPtr();
                    int iLevel = DETAILRECIPE_USE_RECIPE_LEVEL;                // add code to check for Merit style interface here
                    character_WorkshopSendDetailRecipeBuild(e->pchar, pState->recipe->pchName, iLevel, s_useCoupon );
            }
            BuildCBox( &buttonBox, x + 17*scale, y + textHeight-20*scale, 60*scale, 20*scale);
        } else {
            BuildCBox( &buttonBox, 0, 0, 0, 0);
        }

        /////////////////////////////////////////////////////////////////////////////////////
        // BUY HACK
        // check for buy button
        if (pNode->state&UITREENODE_EXPANDED && !s_InventoryMode && pState->recipe->BuyFromVendor > 0)
        {
            Entity *e = playerPtr();
            int isLicenseLocked = pState->recipe->flags & RECIPE_INVENTION_LIC_REQUIRED && !AccountHasStoreProduct(ent_GetProductInventory( e ), SKU("coliinvn"));

            if(D_MOUSEHIT == drawStdButton(x + 42*scale, y + textHeight-30*scale, (float)z, 60*scale, 20*scale, 
                window_GetColor(WDW_RECIPEINVENTORY), "BuyString", scale, 
                (e->pchar->iInfluencePoints < pState->recipe->BuyFromVendor) || isLicenseLocked)) 
            {
                    if (!character_WorkshopSendDetailRecipeBuy(e->pchar, pState->recipe->pchName ))
                    {
                        // Tell player not enough inventory space
                    } else {
                        recipeSetReformat(pNode);
                    }
            }
            BuildCBox( &buyButtonBox, x + 17*scale, y + textHeight-40*scale, 60*scale, 20*scale);
        } else {
            BuildCBox( &buyButtonBox, 0, 0, 0, 0);
        }
        //
        /////////////////////////////////////////////////////////////////////////////////////


        // check for mouse over
        BuildCBox( &box, x, y, width, textHeight+5*scale);
        if (mouseCollision(&box) && !mouseCollision(&buttonBox) && !mouseCollision(&buyButtonBox))
//        if (mouseCollision(&buttonBox))
        {
            box_color = CLR_MOUSEOVER_BACKGROUND;
            forground_color = CLR_MOUSEOVER_FOREGROUND;
        }

        // draw frame
        drawFlatFrame(PIX2, R10, x, y, z-0.5, width, textHeight+5*scale, scale, forground_color, 
                        box_color);


        // draw enhancement if needed
        if (pState->pEnhancement)
        {
            if (pNode->state&UITREENODE_EXPANDED)
            {
                pState->icon = uiRecipeDrawIcon(pState->recipe, pState->recipe->level, x, y, z, scale, WDW_RECIPEINVENTORY, true, CLR_WHITE);
//                pState->icon = uiEnhancementDraw(pState->pEnhancement, x + 35.f*scale, y + 35.f*scale, z, scale*0.75f, scale*1.25f, MENU_GAME, WDW_RECIPEINVENTORY, true);
                pState->icon2 = uiEnhancementGetTexture(pState->pEnhancement);
            }
            else
            {
                uiEnhancementDontDraw(pState->pEnhancement);
                pState->icon = uiEnhancementGetPog(pState->pEnhancement);
                pState->icon2 = uiEnhancementGetTexture(pState->pEnhancement);
//                drawEnhancementOriginLevel( pState->pBoostBase, pState->recipe->level, 0, false, x, y, z, scale/3.0, scale/3.0, CLR_WHITE );
            }
        } else {
            if (pNode->state & UITREENODE_EXPANDED)
            {
                pState->icon = uiRecipeDrawIcon(pState->recipe, pState->recipe->level, x, y, z, scale, WDW_RECIPEINVENTORY, true, CLR_WHITE);
                pState->icon2 = NULL;
            } else {
                if (pState->recipe->ui.pchIcon)
                    pState->icon = atlasLoadTexture(pState->recipe->ui.pchIcon);
                else 
                    pState->icon = NULL;
                pState->icon2 = NULL;
            }
            /*
            // draw regular icon
            if(pState->recipe->ui.pchIcon && !pState->icon)
            {
                pState->icon = atlasLoadTexture(pState->recipe->ui.pchIcon);
            }
            if (pState->icon)
//                display_sprite(pState->icon, x + 35.f*scale, y + 35.f*scale, z, iconSize*scale/pState->icon->width, iconSize*scale/pState->icon->height, CLR_WHITE );
                display_sprite(pState->icon, x, y, z, iconSize*scale/pState->icon->width, iconSize*scale/pState->icon->height, CLR_WHITE );
            */
        }

        // check for click
        if( mouseClickHit( &box, MS_LEFT ) && !mouseClickHit( &buttonBox, MS_LEFT ) && 
            !mouseClickHit( &buyButtonBox, MS_LEFT ))
        {
            pNode->state ^= UITREENODE_EXPANDED;
 
            // flag all children as needing a reset to clear tooltips
            recipeSetReformat(pNode);

            // force a reformat of this node
            uiTreeRecipeReformat(pState, pNode->state&UITREENODE_EXPANDED, scale);

            ////////////////////////////////////////////
            // if this node has changed state recalculate size
            uiTreeRecalculateNodeSize(pNode, width, scale);

        } 

        BuildCBox( &box, x, y, width-10*scale, textHeight+5*scale);

        if( mouseLeftDrag( &box ) && pState->icon && pState->recipe->Type == kRecipeType_Drop)
        { 
            float cursorScale = optionGetf(kUO_CursorScale);
            if (pState->icon2)
                recipe_drag(pState->recipe->id, pState->icon2, pState->icon, cursorScale ? scale / cursorScale : scale);
            else
                recipe_drag(pState->recipe->id, pState->icon, NULL, cursorScale ? scale / cursorScale : scale);
        } 
        else if( pState->recipe->Type == kRecipeType_Drop && mouseClickHit( &box, MS_RIGHT ) )
        {
            uiTreeRecipeContextMenu(pState, width);
        }

    } else {
        textHeight = smf_ParseAndFormat(pState->recipeFormat, pState->text.buff, 
            x+25*scale+iconSize, y+1*scale, z, width-35*scale-iconSize, 0, false, false, false, &s_taDefaults, 0);
    }

    pNode->heightThisNode = textHeight + spaceBetweenNodes*scale;

    return pNode->heightThisNode;
}

static void recipeSetReformat(uiTreeNode *pNode)
{
    int i, count = eaSize(&pNode->children);
    uiTreeNode *pParent = NULL;

    if (pNode->fpDraw == uiTreeRecipeDrawCallback) 
    {
        treeRecipeDisplayState *pDisplay = (treeRecipeDisplayState *)pNode->pData;
        pDisplay->reformat = true;

        // clear old tool tips
        if (pDisplay->pEnhancement)
            uiEnhancementDontDraw(pDisplay->pEnhancement); 
    }

    
    // remove old size from parent
    pParent = pNode->pParent;
    while (pParent)
    {
        if (pNode->height > 0.0f)
            pParent->height -= pNode->height;
        pParent = pParent->pParent;
    }
    pNode->height = -1.0f;

    // check children
    for (i = 0; i < count; i++)
    {
        recipeSetReformat(pNode->children[i]);
    }
}

static uiTreeNode *recipeTreeFind(uiTreeNode *pNode, U32 nameHash)
{
    uiTreeNode *retval = NULL;

    if (pNode->fpDraw == uiTreeRecipeDrawCallback) 
    {
        treeRecipeDisplayState *pDisplay = (treeRecipeDisplayState *)pNode->pData;
        if (pDisplay->hash == nameHash)
            return pNode;
    } else {
        // check children
        int i, count = eaSize(&pNode->children);
        for (i = 0; i < count; i++)
        {
            retval = recipeTreeFind(pNode->children[i], nameHash);
            if (retval != NULL)
                return retval;
        }
    }
    return retval;
}

void recipeTreeFreeCallback(uiTreeNode *pNode)
{
    if (pNode->fpDraw == uiTreeRecipeDrawCallback && pNode->pData != NULL)
    {
        treeRecipeDisplayState *pDisplay = (treeRecipeDisplayState *)pNode->pData;
        if (pDisplay->pEnhancement)
            uiEnhancementFree(&pDisplay->pEnhancement);

        smfBlock_Destroy(pDisplay->recipeFormat);
        freeStuffBuff(&pDisplay->text);

        free(pNode->pData);
    }
}

/////////////////////////////////////////////////////////////////////////////////

static int compareTreeNodes(const uiTreeNode **a, const uiTreeNode **b)
{  
    static char temp[128];
    static char temp2[128];
    int result;

    if ((*a)->fpDraw == uiTreeRecipeDrawCallback)
    {
        if ((*b)->fpDraw != uiTreeRecipeDrawCallback)
        {
            return -1;
        } else {
            treeRecipeDisplayState *pADisplay = (treeRecipeDisplayState *)(*a)->pData;
            treeRecipeDisplayState *pBDisplay = (treeRecipeDisplayState *)(*b)->pData;

            result = stricmp( getRecipeDisplayName(pADisplay->recipe), 
                                getRecipeDisplayName(pBDisplay->recipe) );
            
            if (result)
                return result; 

            return pADisplay->recipe->id - pBDisplay->recipe->id;

        }
    } else {
        if ((*b)->fpDraw != uiTreeRecipeDrawCallback)
        {
            strcpy(temp,textStd((char *) (*a)->pData));
            if (stricmp(temp, "MemorizedString") == 0)
                return 1;

            strcpy(temp2,textStd((char *) (*b)->pData));
            if (stricmp(temp2, "MemorizedString") == 0)
                return -1; 

            result = stricmp( temp,temp2 );
            if (result)
                return result;
        } else {
            return 1;
        }
    }
    return 1;
}

// This function attempts to find the leaves of the tree and then sort them accordingly.
static void recipeTreeSort(uiTreeNode *pNode)
{
    // sort children's children
    int i, count = eaSize(&pNode->children);
    for (i = 0; i < count; i++)
    {
        recipeTreeSort(pNode->children[i]);
    }

    // sort children
    eaQSort(pNode->children, compareTreeNodes);
}

/////////////////////////////////////////////////////////////////////////////////

// This function removes all entires in the tree whose recipe pointer has been reset to NULL.  
//        This can happen if the recipe entry that was in the tree has since been removed.
static int recipeTreePrune(uiTreeNode *pNode)
{
    // check children
    int i, count = eaSize(&pNode->children);
    for (i = 0; i < count; i++)
    {
        if (recipeTreePrune(pNode->children[i]))
        {
            uiTreeNode *pChild = pNode->children[i];
            uiTreeFree(pChild); // removes child from parent
            i--;
            count--;
        }
    }

    // check this node
    if (pNode->fpDraw == uiTreeRecipeDrawCallback) 
    {
        treeRecipeDisplayState *pDisplay = (treeRecipeDisplayState *)pNode->pData;
        if (pDisplay->recipe == NULL)
            return true;
    } else {
        if (eaSize(&pNode->children) < 1)
            return true;
    }
    return false;
}

static void recipeTreeAdd(uiTreeNode *pNode, const DetailRecipe *pRecipe, Entity *e)
{
    uiTreeNode *pATNode;
    U32 nameHash;
    
    if (pNode == NULL)
        return;

    nameHash = hashString(pRecipe->pchName, false);

    // see if it already exists
    pATNode = recipeTreeFind(pNode, nameHash);
    if (pATNode == NULL)
    {
        treeRecipeDisplayState *pDisplay = (treeRecipeDisplayState *) calloc(1, sizeof(treeRecipeDisplayState));
        pATNode = uiTreeNewNode();
        memset(pDisplay, 0, sizeof(treeRecipeDisplayState));
        pDisplay->recipeFormat = smfBlock_Create();
        pDisplay->recipe = pRecipe;
        pDisplay->count = character_RecipeCount(e->pchar, pRecipe);
        pDisplay->reformat = true;
        pDisplay->hash = nameHash;
        // see if this is a boost recipe
        if (pRecipe->pchEnhancementReward)
        {
            pDisplay->pEnhancement = uiEnhancementCreateFromRecipe(pRecipe, pRecipe->level, false);
        }

        // set up the node information
        pATNode->pParent = pNode;
        pATNode->fpDraw = uiTreeRecipeDrawCallback;        
        pATNode->fpFree = recipeTreeFreeCallback;
        pATNode->pData = (void *) pDisplay;

        eaPush(&pNode->children, pATNode);
    } else {
        treeRecipeDisplayState *pDisplay = (treeRecipeDisplayState *)pATNode->pData;
        pDisplay->recipe = pRecipe;
        pDisplay->reformat = true;
    }
}

static void recipeTreeRemoveRecipes(uiTreeNode *pNode)
{
    treeRecipeDisplayState *pDisplay = (treeRecipeDisplayState *)pNode->pData;
    int i, count = eaSize(&pNode->children);

    // clear recipe entry if it has one
    if (pNode->fpDraw == uiTreeRecipeDrawCallback)
    {
        pDisplay->recipe = NULL;
    }

    // check the children
    for (i = 0; i < count; i++)
    {
        recipeTreeRemoveRecipes(pNode->children[i]);
    }
}


////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
// R E C I P E     D I S P L A Y    ( S P L I T   S C R E E N )
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

uiTreeNode *g_pCurrentSelection = NULL;

typedef struct uiMeritNodeData {
    char *name;
    treeRecipeDisplayState **ppRecipes;
} uiMeritNodeData;

void uiMeritRecipeFreeCallback(void *pData)
{
    treeRecipeDisplayState *pRecipe = (treeRecipeDisplayState *) pData;

    if (pData)
    {
        if (pRecipe->pEnhancement)
            uiEnhancementFree(&pRecipe->pEnhancement);    

        smfBlock_Destroy(pRecipe->recipeFormat);
        freeStuffBuff(&pRecipe->text);

        SAFE_FREE(pData);
    }
}

void uiMeritTreeNodeFreeCallback(uiTreeNode *pNode)
{
    uiMeritNodeData *pNodeData = NULL;

    if (!pNode)
        return;

    pNodeData = (uiMeritNodeData *) pNode->pData;

    // clean up name
    SAFE_FREE(pNodeData->name);

    // clean recipe list
    eaDestroyEx(&pNodeData->ppRecipes, uiMeritRecipeFreeCallback);

    // clean up node data
    SAFE_FREE(pNodeData);

}

static void recipeMeritTreeAdd(uiTreeNode *pNode, const DetailRecipe *pRecipe)
{
    uiMeritNodeData *pNodeData = NULL;

    if (!pNode)
        return;

    pNodeData = (uiMeritNodeData *) pNode->pData;

    {
        treeRecipeDisplayState *pDisplay = (treeRecipeDisplayState *) calloc(1, sizeof(treeRecipeDisplayState));
        memset(pDisplay, 0, sizeof(treeRecipeDisplayState));
        pDisplay->recipeFormat = smfBlock_Create();
        pDisplay->recipe = pRecipe;
        pDisplay->count = 0;
        pDisplay->reformat = true;
        if (pRecipe->pchEnhancementReward || pRecipe->pchRecipeReward)
        {
            pDisplay->pEnhancement = uiEnhancementCreateFromRecipe(pRecipe, rewardLevel, true);
        }
        eaPush(&pNodeData->ppRecipes, pDisplay);
    }
}

float uiMeritTextDrawCallback(uiTreeNode *pNode, float x, float y, float z, float width, 
                              float scale, int display)
{
    static SMFBlock *block = 0;
    float spaceBetweenNodes = SPACE_BETWEEN_NODES;
    int textHeight;
    CBox box;
    int box_color = CLR_NORMAL_BACKGROUND;
    int forground_color = CLR_NORMAL_FOREGROUND;
    uiMeritNodeData *pNodeData = NULL;

    if (!pNode)
        return 0.0;

    if (!block)
    {
        block = smfBlock_Create();
    }

    pNodeData = (uiMeritNodeData *) pNode->pData;

    if (display)
    {
        s_taDefaults.piScale = (int*)((int)(SMF_FONT_SCALE*(scale*1.0f)));
        textHeight = smf_ParseAndDisplay(block, textStd(pNodeData->name), x+19*scale,
            y+2*scale, z, width-19*scale, 0,    false, false, 
            &s_taDefaults, NULL, 0, 0);

        // check for mouse over
        BuildCBox( &box, x, y, width, textHeight+5*scale);
        if (mouseCollision(&box) || pNode == g_pCurrentSelection)
        {
            box_color = CLR_MOUSEOVER_BACKGROUND;
            forground_color = CLR_MOUSEOVER_FOREGROUND;
        }

        // draw frame
        drawFlatFrame(PIX2, R10, x, y, z-0.5, width, textHeight+5*scale, scale, forground_color, 
            box_color);

        // check for click
        if( mouseClickHit( &box, MS_LEFT ) )
        {
            pNode->state ^= UITREENODE_EXPANDED;

            ////////////////////////////////////////////
            // if this node has changed state recalculate size
            uiTreeRecalculateNodeSize(pNode, width, scale);

            g_pCurrentSelection = pNode;
        }

        // expanded arrow
        if (eaSize(&pNode->children) > 0)
        {
            if (pNode->state & UITREENODE_EXPANDED)
            {
                AtlasTex * leftArrow  = atlasLoadTexture( "Jelly_arrow_down.tga");    

                display_sprite( leftArrow, x+6.0*scale, y+8.0*scale, z, 0.5*scale, scale, CLR_WHITE);
            } else {
                AtlasTex * leftArrow  = atlasLoadTexture( "teamup_arrow_expand.tga");    // uses same arrows as uiTray.c

                display_sprite( leftArrow, x+10.0*scale, y+5.0*scale, z, scale, 0.4*scale, CLR_WHITE);
            }
        }

    } else {
        s_taDefaults.piScale = (int*)((int)(SMF_FONT_SCALE*(scale*1.0f)));
        textHeight = smf_ParseAndFormat(block, textStd(pNodeData->name), 0.0f, 0.0f,
            0.1f, width-19*scale, 0, false, false, false, &s_taDefaults, 0);
    }

    pNode->heightThisNode = textHeight+spaceBetweenNodes*scale;

    return pNode->heightThisNode;
}


uiTreeNode *recipeFindOrAddChildNode(uiTreeNode *pRoot, char *name)
{
    int i, count = eaSize(&pRoot->children);
    char *nameRemains = NULL;
    uiTreeNode *pCurrentTree = NULL;
    uiMeritNodeData *pNodeData = NULL;

    if (!name || !pRoot)
        return NULL;

    nameRemains = strchr(name, '|');
    removeTrailingWhiteSpaces(&name);
    if (nameRemains)
    {
        *nameRemains = 0;
        nameRemains++;
        nameRemains = (char*)removeLeadingWhiteSpaces(nameRemains);
    }

    // see if this already exists
    for (i = 0; i < count; i++)
    {
        uiMeritNodeData *pNodeData = NULL;

        pNodeData = (uiMeritNodeData *) pRoot->children[i]->pData;

        if (stricmp(pNodeData->name, name) == 0)
        {
            if (nameRemains != NULL)
            {
                return recipeFindOrAddChildNode(pRoot->children[i], nameRemains);
            } else {
                return pRoot->children[i];
            }
        }
    }

    // doesn't exist - make one!
    pCurrentTree = uiTreeNewNode();
    pNodeData = (uiMeritNodeData *) malloc(sizeof(uiMeritNodeData));
    pNodeData->name = (char *) strdup(name);
    pNodeData->ppRecipes = NULL;
    pCurrentTree->pData = pNodeData;
    pCurrentTree->fpFree = uiMeritTreeNodeFreeCallback;
    pCurrentTree->state = UITREENODE_NONE;
    pCurrentTree->fpDraw = uiMeritTextDrawCallback;
    pCurrentTree->pParent = pRoot;
    eaPush(&pRoot->children, pCurrentTree);

    if (nameRemains != NULL)
    {
        return recipeFindOrAddChildNode(pCurrentTree, nameRemains);
    } else {
        return pCurrentTree;
    }
}

#define MIN_RECIPE_HEIGHT 85

float uiMeritRecipeDraw(treeRecipeDisplayState *pState, float x, float y, float z, float width, float scale)
{ 
    float spaceBetweenNodes = SPACE_BETWEEN_NODES;
    int textHeight;
    CBox box, buttonBox;
    int box_color = CLR_NORMAL_BACKGROUND;
    int forground_color = CLR_NORMAL_FOREGROUND;
    float iconSize = 60.0f*scale;
    int xOffset = 20;
    Entity * e = playerPtr();
    int i, bArchitectInventory = 0;
    int bCanBuy = pState->have_needed;

    // try not to hijack entire recipe system for this
    for( i = 0; i < eaSize(&pState->recipe->pchInventReward); i++ )
    {
        if( strnicmp( pState->recipe->pchInventReward[i], "architect.", 10 ) == 0 )
            bArchitectInventory = 1;
    }

    // checking for required license
    if (pState->recipe->flags & RECIPE_INVENTION_LIC_REQUIRED)
    {
        if (!AccountHasStoreProduct(ent_GetProductInventory( e ), SKU("coliinvn")))
            bCanBuy = false;
    }


    if( bArchitectInventory )
        bCanBuy &= (!e->mission_inventory || !e->mission_inventory->dirty);

    s_taDefaults.piScale = (int*)((int)(0.85f*scale*SMF_FONT_SCALE));

    if (pState->reformat)
    {
        uiTreeRecipeReformat(pState, true, scale);
        if (pState->pEnhancement)
            uiEnhancementRefresh(pState->pEnhancement);
    }


    if (!bCanBuy)
    {
        forground_color = CLR_CONSTRUCT_EX(255, 255, 255, 0.04); 
    }

    textHeight = smf_ParseAndDisplay(pState->recipeFormat, pState->text.buff, 
        x+(10+xOffset)*scale+iconSize, y+1*scale, z, width-(20+xOffset)*scale-iconSize, 0,    
        false, false, &s_taDefaults, NULL, 0, 1);


    if (textHeight < MIN_RECIPE_HEIGHT*scale)
        textHeight = MIN_RECIPE_HEIGHT*scale;

    // buy button 
    if(D_MOUSEHIT == drawStdButton(x + 42*scale, y + textHeight-10*scale, (float)z, 60*scale, 20*scale,    window_GetColor(WDW_RECIPEINVENTORY), "BuyString", scale, !bCanBuy)) 
    {
        int iLevel = rewardLevel;
        if( bArchitectInventory && e->mission_inventory )
            e->mission_inventory->dirty = 1;
        character_WorkshopSendDetailRecipeBuild(e->pchar, pState->recipe->pchName, iLevel, s_useCoupon );
    }
    BuildCBox( &buttonBox, x + 17*scale, y + textHeight-20*scale, 60*scale, 20*scale);

    // check for mouse over
    BuildCBox( &box, x, y, width, textHeight+5*scale);
    if (mouseCollision(&box) && !mouseCollision(&buttonBox) )
    {
        box_color = CLR_MOUSEOVER_BACKGROUND;
        forground_color = CLR_MOUSEOVER_FOREGROUND;
    }

    // draw frame
    drawFlatFrame(PIX2, R10, x, y, z-0.5, width, textHeight+5*scale, scale, forground_color, box_color);

    // draw enhancement if needed
    if (pState->pEnhancement)
    {
        pState->icon = uiRecipeDrawIconEx(pState->recipe, rewardLevel, x, y, z, scale, WDW_RECIPEINVENTORY, true, CLR_WHITE, 0.0f, true);
        pState->icon2 = uiEnhancementGetTexture(pState->pEnhancement);
    } 
    else
    {
        pState->icon = uiRecipeDrawIconEx(pState->recipe, rewardLevel, x, y, z, scale, WDW_RECIPEINVENTORY, true, CLR_WHITE, 0.0f, false);
        pState->icon2 = NULL;
    }

    BuildCBox( &box, x, y, width-10*scale, textHeight+5*scale);

    return textHeight + spaceBetweenNodes*scale;;
}

float uiIncarnateRecipeDraw(treeRecipeDisplayState *pState, float x, float y, float z, float width, float scale)
{ 
    float spaceBetweenNodes = SPACE_BETWEEN_NODES;
    int textHeight;
    CBox box, buttonBox;
    int box_color = CLR_CONSTRUCT_EX(255, 255, 255, 0.4);
    int forground_color = CLR_NORMAL_FOREGROUND;
    float iconSize = pState->icon ?  pState->icon->width*scale : 60.0f*scale;
    Entity * e = playerPtr();
    int i = 0;
    int borderOffset = 10;
    int bCanBuy = pState->have_needed;
    int *incarnateScale = (int*)((int)(0.85f*scale*SMF_FONT_SCALE));
    int oldUseCoupon = s_useCoupon;
    s_useCoupon = 0;        //    incarnates don't use the discount coupon ever
    if (s_taDefaults.piScale != incarnateScale)
    {
        uiIncarnateReparse();
        s_taDefaults.piScale = incarnateScale;
    }
    if (pState->reformat)
    {
        uiTreeRecipeReformat(pState, true, scale);
        if (pState->pEnhancement)
            uiEnhancementRefresh(pState->pEnhancement);
    }
    

    if (!bCanBuy)
    {
        forground_color = CLR_CONSTRUCT_EX(255, 255, 255, 0.04); 
    }

    textHeight = smf_ParseAndDisplay(pState->recipeFormat, pState->text.buff, 
        x+(10+borderOffset)*scale+iconSize, y+(borderOffset)*scale, z, width-(20+2*borderOffset)*scale-iconSize, 0,    
        false, false, &s_taDefaults, NULL, 0, 1);


    if (textHeight < MIN_RECIPE_HEIGHT*scale)
        textHeight = MIN_RECIPE_HEIGHT*scale;

    textHeight += 2*borderOffset*scale;

    // buy button 
    if(D_MOUSEHIT == drawStdButton(x + width/2, y + textHeight+10*scale, (float)z, 60*scale, 20*scale,    window_GetColor(WDW_RECIPEINVENTORY), "CreateString", scale, !bCanBuy)) 
    {
        Character *p = e->pchar;

        if(p)
        {
            int iLevel = rewardLevel;
            const char *recipeName = pState->recipe->pchName;
            const DetailRecipe *r =  detailrecipedict_RecipeFromName(recipeName);
            if( r && character_DetailRecipeCreatable( p, r, NULL, true, 0, 0 ))
            {
                START_INPUT_PACKET(pak, CLIENTINP_INCARNATE_CREATE);
                pktSendString(pak, recipeName);
                pktSendBitsAuto(pak, iLevel);
                END_INPUT_PACKET;
            }
        }
    }
    BuildCBox( &buttonBox, x + width/2 -30*scale, y + textHeight+10*scale, 60*scale, 20*scale);

    // check for mouse over
    BuildCBox( &box, x, y, width, textHeight+5*scale);
    if (mouseCollision(&box) && !mouseCollision(&buttonBox) )
    {
        box_color = CLR_CONSTRUCT_EX(255, 255, 255, 0.5);
        forground_color = CLR_MOUSEOVER_FOREGROUND;
    }

    // draw frame
    drawFlatFrame(PIX2, R10, x, y, z-0.5, width, textHeight+35*scale, scale, forground_color, box_color);

    // draw enhancement if needed
    if (pState->pEnhancement)
    {
        pState->icon = uiRecipeDrawIconEx(pState->recipe, rewardLevel, x, y, z, scale, WDW_RECIPEINVENTORY, true, CLR_WHITE, 0.0f, true);
        pState->icon2 = uiEnhancementGetTexture(pState->pEnhancement);
    } 
    else
    {
        pState->icon = uiRecipeDrawIconEx(pState->recipe, rewardLevel, x, y, z, scale, WDW_RECIPEINVENTORY, true, CLR_WHITE, 0.0f, false);
        pState->icon2 = NULL;
    }

    BuildCBox( &box, x, y, width-10*scale, textHeight+35*scale);

    s_useCoupon = oldUseCoupon;
    return textHeight + spaceBetweenNodes*scale + 30*scale;
}

float uiMeritDrawSelectedRecipes(uiTreeNode *pNode, float x, float *startY, float windowTop, float z, float width, float height, float scale)
{
    int i, count;
    float totalHeight = 0.0f;

    if (pNode)
    {
        uiMeritNodeData *pNodeData = NULL;

        pNodeData = (uiMeritNodeData *) pNode->pData;

        // draw this nodes recipes
        count = eaSize(&pNodeData->ppRecipes);
        for (i = 0; i < count; i++)
        {
            treeRecipeDisplayState *pRecipe = pNodeData->ppRecipes[i];
            if (pRecipe)
            {
                if (pRecipe->height == 0 || (*startY +  pRecipe->height > windowTop && *startY <= windowTop + height))
                    pRecipe->height = uiMeritRecipeDraw(pRecipe, x, *startY, z, width, scale);
                *startY += pRecipe->height;
                totalHeight += pRecipe->height;
            }
        }
        
        // then draw children's
        count = eaSize(&pNode->children);
        for (i = 0; i < count; i++)
        {
            totalHeight += uiMeritDrawSelectedRecipes(pNode->children[i], x, startY, windowTop, z, width, height, scale);
        }
    }

    return totalHeight;
}

/////////////////////////////////////////////////////////////////////////////////
// This function removes all entires in the tree whose recipe pointer has been reset to NULL.  
//        This can happen if the recipe entry that was in the tree has since been removed.
static int recipeMeritTreePrune(uiTreeNode *pNode)
{
    int i, count;

    if (!pNode)
        return false;

    // check children
    count = eaSize(&pNode->children);
    for (i = count - 1; i >= 0; i--)
    {
        if (recipeMeritTreePrune(pNode->children[i]))
        {
            uiTreeNode *pChild = pNode->children[i];

            if (g_pCurrentSelection == pChild)
                g_pCurrentSelection = NULL;

            uiTreeFree(pChild); // removes child from parent
        }
    }

    // check this node
    {
        uiMeritNodeData *pNodeData = (uiMeritNodeData *) pNode->pData;

        if (pNodeData->ppRecipes == NULL && eaSize(&pNode->children) < 1)
        {
            return true;
        } 
    }
    return false;
}


static void recipeTreeRemoveMeritRecipes(uiTreeNode *pNode)
{
    int i, count;
    uiMeritNodeData *pNodeData = NULL;

    if (!pNode)
        return;

    pNodeData = (uiMeritNodeData *) pNode->pData;
    
    eaDestroyEx(&pNodeData->ppRecipes, uiMeritRecipeFreeCallback);

    // check the children
    count = eaSize(&pNode->children);
    for (i = 0; i < count; i++)
    {
        recipeTreeRemoveMeritRecipes(pNode->children[i]);
    }
}

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

static int compareMeritTreeNodes(const uiTreeNode **a, const uiTreeNode **b)
{  
    static char temp[128];
    static char temp2[128];
    int result;

    if ((*a)->fpDraw == uiMeritTextDrawCallback)
    {
        uiMeritNodeData *pNodeData = (uiMeritNodeData *) (*a)->pData;

        strcpy(temp,textStd(pNodeData->name));
    } else {
        strcpy(temp,textStd((char *) (*a)->pData));
    }

    if ((*b)->fpDraw == uiMeritTextDrawCallback)
    {
        uiMeritNodeData *pNodeData = (uiMeritNodeData *) (*b)->pData;

        strcpy(temp2,textStd(pNodeData->name));
    } else {
        strcpy(temp2,textStd((char *) (*b)->pData));
    }

    result = stricmp( temp,temp2 );
    if (result)
        return result;
    else
        return 1;
}


// This function attempts to find the leaves of the tree and then sort them accordingly.
static void recipeMeritTreeNodeSort(uiTreeNode *pNode)
{
    // sort children's children
    int i, count = eaSize(&pNode->children);
    for (i = 0; i < count; i++)
    {
        recipeMeritTreeNodeSort(pNode->children[i]);
    }

    // sort children
    eaQSort(pNode->children, compareMeritTreeNodes);
}

static int compareTreeRecipes(const treeRecipeDisplayState **a, const treeRecipeDisplayState **b)
{  
    static char temp[128];
    static char temp2[128];
    int result;

    if ((*a) == NULL)
        return -1;
    if ((*b) == NULL)
        return 1;

    strcpy(temp, textStd((char *) (*a)->recipe->ui.pchDisplayName));
    strcpy(temp2, textStd((char *) (*b)->recipe->ui.pchDisplayName));
    
    result = stricmp( temp,temp2 );
    if (result)
        return result;
    else
        return 1;
}

/////////////////////////////////////////////////////////////////////////////////
// This function sorts all the nodes in the tree and all the recipes that each node contains
static int recipeMeritTreeSort(uiTreeNode *pNode)
{
    uiMeritNodeData *pNodeData = NULL;

    if (!pNode)
        return false;

    // sort the nodes
    recipeMeritTreeNodeSort(pNode);

    // sort the recipes
    if (pNode->fpDraw == uiMeritTextDrawCallback)
    {
        pNodeData = (uiMeritNodeData *) pNode->pData;

        if (pNodeData->ppRecipes != NULL)
        {
            eaQSort(pNodeData->ppRecipes, compareTreeRecipes);
        } 
    }
    return false;
}

////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

static const DetailRecipe **detailRecipes = NULL;

void recipeInventoryReparse(void) {
    s_bReparse = 1; 
}

static void claimTicketsCallback(void *data)
{
    int tickets = atoi(dialogGetTextEntry());
    Entity *e = playerPtr();
    char cmdstr[100];

    if (tickets && e && !e->mission_inventory->dirty)
    {
        e->mission_inventory->dirty = true;
        sprintf(cmdstr, "architect_claim_tickets %d", tickets);
        cmdParse(cmdstr);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////
// M A I N   F U N C T I O N
////////////////////////////////////////////////////////////////////////////////////////////

static uiTreeNode *pAllNode = NULL;
static uiTreeNode *pMeritAllNode = NULL;
//static uiTreeNode **recipeTreeList = NULL;
static uiTabControl *workshopCategories = NULL;

#define CBOXHEIGHT 21

int recipeInventoryWindow()
{
    float x, y, z, wd, ht, sc;
    int color, bcolor;
    int i, docHeight;
    int tabCount = 0;
    float oldy, yoffset, height;
    Entity *e = playerPtr();
    static int init = 1;
    char *curTab;
    static ScrollBar sb = { WDW_RECIPEINVENTORY, 0 };
    static ScrollBar splitLeft = { -1, 0 };
    static ScrollBar splitRight = { -1, 0 };
    static ComboBox hideBox = {0};
    static float oldScale = 0.0f;
    static float fLevel = 1.0f;
    static int characterLevel = 1;
    static int closeTimer = 0;
    uiTreeNode *pCurrentTree = NULL;
    uiTreeNode *pMemTree = NULL;
    RoomDetail *pdetail = baseGetDetail(&g_base, e->pl->detailInteracting, NULL);
    static char *pMemorizedString = "MemorizedString";
    char *pSelectedTab = NULL;
    char *pWorkshopType = e->pl->workshopInteracting;
    float yRecipes;



    if(!window_getDims(WDW_RECIPEINVENTORY, &x, &y, &z, &wd, &ht, &sc, &color, &bcolor))
    {
        init = 1;
        recipeInventoryReparse();

        // pointer into tree - Tree is gone so...
        g_pCurrentSelection = NULL;

        // clean away any old information
        if (pAllNode)
        {
            int i, count = eaSize(&pAllNode->children);

            // clean up tree here! 
            for (i = 0; i < count; i++)
            {
                recipeTreeRemoveRecipes(pAllNode->children[i]);
            }

            uiTreeFree(pAllNode);
            pAllNode = NULL;

        }

        if (pMeritAllNode)
        {
            int i, count = eaSize(&pMeritAllNode->children);

            // clean up tree here! 
            for (i = 0; i < count; i++)
            {
                recipeTreeRemoveMeritRecipes(pMeritAllNode->children[i]);
            }

            uiTreeFree(pMeritAllNode);
            pMeritAllNode = NULL;

            // pointer into tree - Tree is gone so...
            g_pCurrentSelection = NULL;
        }

        if ((window_getMode( WDW_RECIPEINVENTORY ) == WINDOW_SHRINKING ||
                window_getMode( WDW_RECIPEINVENTORY ) == WINDOW_DOCKED) && 
            e && e->pl && e->pl->workshopInteracting[0] != 0 && closeTimer < 1)
        {
            workshop_SendClose();
            closeTimer = 20;
        } 
        else if (closeTimer > 0) 
        {
            closeTimer--;
        }
        return 0;
    }

    closeTimer = 0;

    // it's a workshop window if the player is interacting with a detail in the workshop category or
    //        they are interacting with an object with the workshop tag
    if (entity_IsAtWorkShop(e, pdetail))
        s_InventoryMode = false;
    else
        s_InventoryMode = true;

    // is this a split pane inventory?
    if (pWorkshopType && (strstri(pWorkshopType, "merit") != NULL 
                            || strstri(pWorkshopType, "ticket") != NULL 
                            || strstri(pWorkshopType, "incarnate") != NULL
                            || strstri(pWorkshopType, "HAM") != NULL
                            || strstri(pWorkshopType, "VAM") != NULL
                            || strstri(pWorkshopType, "split") != NULL))
    {
        gSplitPane = true;
        if (window_getMode( WDW_RECIPEINVENTORY ) == WINDOW_SHRINKING)
        {
            if (!s_InventoryMode) 
                cmdParse("emote none");
            return 0;
        } 
        else if (init || (s_bReparse && window_getMode(WDW_RECIPEINVENTORY) == WINDOW_DISPLAYING ))
        {

            // setup filter
            if (init)
            {    
                s_Hide = 0; 
                comboboxClear(&hideBox);
                combocheckbox_init(&hideBox,0,0,0,200,200,CBOXHEIGHT,400,"HideTitle",WDW_RECIPEINVENTORY,1,0);
                combocheckbox_add(&hideBox,0,NULL,"HideMissingSalvage",HIDE_MISSING_SALVAGE);

                if (s_InventoryMode) 
                {
                    if (optionGet(kUO_RecipeHideMissingParts))
                        combobox_setId(&hideBox, HIDE_MISSING_SALVAGE, true);
                    if (optionGet(kUO_RecipeHideUnowned))
                        combobox_setId(&hideBox, HIDE_MISSING_RECIPE, true);
                } 
                else 
                {
                    if (optionGet(kUO_RecipeHideMissingPartsBench))
                        combobox_setId(&hideBox, HIDE_MISSING_SALVAGE, true);
                    if (optionGet(kUO_RecipeHideUnownedBench))
                        combobox_setId(&hideBox, HIDE_MISSING_RECIPE, true);
                }

                characterLevel = character_CalcExperienceLevel(e->pchar) + 1;
                if (characterLevel + 3 > MAX_PLAYER_SECURITY_LEVEL)
                    characterLevel = MAX_PLAYER_SECURITY_LEVEL - 3;

                fLevel = 1.0f;
                updateSort(&hideBox);
            }

            // create unfiltered list of all recipes to be viewed 
            if (detailRecipes)
            {
                eaDestroyConst(&detailRecipes);
            }
            detailRecipes = entity_GetValidDetailRecipes(e, pdetail, e->pl->workshopInteracting); 

            if (pMeritAllNode)
            {
                int i, count = eaSize(&pMeritAllNode->children);

                // clean up tree here! 
                for (i = 0; i < count; i++)
                {
                    recipeTreeRemoveMeritRecipes(pMeritAllNode->children[i]);
                }
            } 
            else 
            {
                pMeritAllNode = uiTreeNewNode();
                pMeritAllNode->pData = (char *) "AllString";
                pMeritAllNode->state |= UITREENODE_EXPANDED;
            }
            
            for (i = 0; i < eaSize(&detailRecipes);i++)
            {
                const DetailRecipe *pRecipe = detailRecipes[i]; 
                char *pNodeName = NULL;

                if (!recipeIsVisible(pRecipe))
                    continue; 

                pNodeName = strdup(textStd(pRecipe->pchDisplayTabName));
                pCurrentTree = recipeFindOrAddChildNode(pMeritAllNode, pNodeName);
                recipeMeritTreeAdd(pCurrentTree, pRecipe);
                SAFE_FREE(pNodeName);
            }

            // prune nodes that have no recipes
            {
                int i, count = eaSize(&pMeritAllNode->children);

                for (i = count - 1; i >= 0; i--)
                {
                    uiTreeNode *pTree = pMeritAllNode->children[i];
                    if (recipeMeritTreePrune(pTree))
                    {
                        if (g_pCurrentSelection == pTree)
                            g_pCurrentSelection = NULL;

                        uiTreeFree(pTree);
                    }
                }
            }

            // sort nodes & recipes
            recipeMeritTreeSort(pMeritAllNode);

            recipeSetReformat(pMeritAllNode);

            init = 0;
            s_bReparse = 0;
        } 
        else
        {
            if (sc != oldScale)
            {
                recipeInventoryReparse();
                oldScale = sc;
            }
        }

        drawFrame( PIX3, R10, x, y, z, wd, ht, sc, color, bcolor );
        drawVerticalLine(x+wd/2, y, ht-(2*sc), z, sc, color);

        set_scissor( 1 ); 

        if (window_getMode(WDW_RECIPEINVENTORY) == WINDOW_DISPLAYING)
        {
            CBox box;
            int windowTop = y;
            int recipeTop = y + (PIX3*4)*sc;
            static int iNewLevel;
            static int changeTicks = 0;
            static const SalvageItem *ticketsalvage;
            bool ticketview = false;
            int ticketoffset = PIX3;

            yRecipes = recipeTop - splitRight.offset;

            if( e && e->pl && e->pl->workshopInteracting && stricmp( e->pl->workshopInteracting, "Worktable_Ticket" ) == 0 )
            {
                int tickets = 0;
                int claim = 0;
                bool canClaim = false;
                AtlasTex *icon;
                int textxoffset = 0;
                int claim_wd;
                int hand_wd;

                ticketview = true;

                if( !ticketsalvage )
                    ticketsalvage = salvage_GetItem("s_ArchitectTicket");
                else
                    tickets = character_SalvageCount( playerPtr()->pchar, ticketsalvage );

                if( e && e->mission_inventory )
                {
                    claim = e->mission_inventory->claimable_tickets;

                    if( !e->mission_inventory->dirty )
                        canClaim = true;
                }

                // Top-left summary
                scissor_dims( x + (PIX3)*sc, y + (PIX3*2)*sc, wd, ht - (PIX3*2)*sc );

                icon = atlasLoadTexture("salvage_ArchitectTicket.tga");
                if( icon )
                {
                    display_sprite( icon, x + (PIX3)*sc, y + (PIX3)*sc, z + 0.1f, sc, sc, CLR_WHITE );
                    ticketoffset += icon->height;
                    textxoffset = icon->width;
                }

                // Make sure we have enough room for the text if the icon happens to be extra-small.
                if( PIX3*19 > ticketoffset )
                    ticketoffset = PIX3*19;

                font( &game_12 );
                font_color(CLR_WHITE, CLR_WHITE);

                claim_wd = str_wd( &game_12, sc, sc, "TicketsOnHandString" );
                hand_wd = str_wd( &game_12, sc, sc, "TicketsToClaimString" );

                prnt( x + (textxoffset+PIX3*2)*sc, y + (PIX3*6+ticketoffset*3/14)*sc, z + 0.2f, sc, sc, "TicketsOnHandString" );

                if( !canClaim )
                    font_color( CLR_GREYED, CLR_GREYED );

                prnt( x + (textxoffset+PIX3*2)*sc, y + (PIX3*6+ticketoffset*7/14)*sc, z + 0.2f, sc, sc, "TicketsToClaimString" );

                font_color( 0xddffddff, 0xddffddff );
                prnt( x + MAX( claim_wd, hand_wd ) + (textxoffset+PIX3*3)*sc, y + (PIX3*6+ticketoffset*3/14)*sc, z + 0.2f, sc, sc, "%d", tickets );

                if( !canClaim )
                    font_color( 0x88aa88ff, 0x88aa88ff );

                prnt( x + MAX( claim_wd, hand_wd ) + (textxoffset+PIX3*3)*sc, y + (PIX3*6+ticketoffset*7/14)*sc, z + 0.2f, sc, sc, "%d", claim );

                if( drawStdButton( x + (wd/2 - wd/18 - PIX3*2*sc), y + (ticketoffset/2)*sc, z, (wd/9), (PIX3*7)*sc, color, "ClaimString", sc, canClaim ? 0 : BUTTON_LOCKED ) == D_MOUSEHIT )
                {
                    char claimStr[20];

                    sprintf( claimStr, "%d", claim );
                    dialogSetTextEntry( claimStr );
                    dialogStd( DIALOG_OK_CANCEL_TEXT_ENTRY, "ClaimTicketsDialogMsg", "ClaimString", "CancelString", claimTicketsCallback, NULL, 0 );
                }

                drawHorizontalLine(x+(PIX2)*sc, y+(ticketoffset)*sc, wd/2-(PIX2*sc), z, sc, color);

                ticketoffset = ticketoffset*sc;
                y += ticketoffset;
            }

            // Level Slider
            set_scissor( 0 );
            fLevel = doSlider( x + (wd/6) + (5*PIX3)*sc, y + (PIX3*8)*sc, z, (wd/3)/sc, sc, sc, fLevel, 0, color, 0);

            rewardLevel = round(((fLevel + 1.0)* 0.5 * (characterLevel + 2)) + 1);
            if (rewardLevel > MAX_PLAYER_SECURITY_LEVEL)
                rewardLevel = MAX_PLAYER_SECURITY_LEVEL; 
            fLevel = (((float) (rewardLevel - 1) / (float) (characterLevel + 2)) * 2.0f) - 1.0f;
            font( &game_12 );
            font_color(CLR_WHITE, CLR_WHITE);
            prnt( x + (wd/3) + (8*PIX3)*sc, y + (PIX3*10)*sc, z+0.1f, sc, sc, "%s %d", textStd("LevelString"), rewardLevel);
            drawHorizontalLine(x+(PIX2)*sc, y + (PIX3*15)*sc, wd/2-(PIX2*sc), z, sc, color);
            set_scissor( 1 );

            // Category Tree
            scissor_dims( x + (PIX3)*sc, y + (PIX3*18)*sc, (wd ), ht - ticketoffset - (PIX3*20)*sc );
            y += (PIX3*18)*sc;
            yoffset = y - splitLeft.offset;
            height = ht - ticketoffset - (PIX3*20)*sc;
            docHeight = uiTreeDisplay(pMeritAllNode, 0, x + PIX3*3*sc, &yoffset, y, z+1, 
                                        (wd / 2) - PIX3*6*sc, &height, sc, true);
            splitLeft.use_color = 1;
            splitLeft.color = color;
            BuildCBox( &box, x, y+PIX3*sc, (wd/2), ht - (PIX3)*sc );
               doScrollBar( &splitLeft, ht - ticketoffset - (PIX3*28)*sc, docHeight + 2*SPACE_BETWEEN_NODES*sc, x + (wd/2) + 3, 
                        windowTop + ticketoffset + (PIX3*21)*sc, z, &box, 0 );
 
            // Recipe List
            scissor_dims( x + (wd / 2) + PIX3*3*sc, recipeTop, (wd / 2) + PIX3*2*sc, ht - (PIX3*8 + CBOXHEIGHT)*sc );
            uiMeritDrawSelectedRecipes(g_pCurrentSelection, x + (wd / 2) + PIX3*4*sc, &yRecipes, windowTop + ticketoffset, z+1, (wd/2) - PIX3*8*sc, ht - ticketoffset, sc);

            splitRight.use_color = 1;
            splitRight.color = color;
            BuildCBox( &box, x+(wd/2), windowTop+(PIX3)*sc, (wd/2), ht - (PIX3)*sc );
            doScrollBar( &splitRight, ht - (PIX3*14 + CBOXHEIGHT)*sc, yRecipes - recipeTop + splitRight.offset + SPACE_BETWEEN_NODES*sc, 
                        x + wd, recipeTop + (PIX3*3)*sc, z, &box, 0 );


            if (iNewLevel != rewardLevel)
            { 
                int i, count = eaSize(&pMeritAllNode->children);
                changeTicks = 50;
                iNewLevel = rewardLevel;

                // clean up tree here! 
                for (i = 0; i < count; i++)
                {
                    recipeTreeRemoveMeritRecipes(pMeritAllNode->children[i]);
                }

            } else {
                if (changeTicks > 1)
                    changeTicks--;
                else if (changeTicks == 1)
                {
                    changeTicks--;
                    recipeInventoryReparse();
                }
            }

        }
        else 
            docHeight = 0.0f;

        set_scissor( 0 );

        hideBox.x = wd/sc - 200;
        hideBox.y = ht/sc - CBOXHEIGHT - PIX3;
        hideBox.z = 0;
        combobox_display(&hideBox);
        if (hideBox.changed)
        {
            updateSort(&hideBox);
            sendOptions(0);
        }


        set_scissor( 0 ); 

    } else {    
        drawHorizontalLine(x+(PIX2)*sc, y+(PIX3*15)*sc, wd/2-(PIX2*sc), z, sc, color);
        gSplitPane = false;

        if (window_getMode( WDW_RECIPEINVENTORY ) == WINDOW_SHRINKING)
        {
            if (!s_InventoryMode) 
                cmdParse("emote none");
//            e->pl->detailInteracting = 0;
//            e->pl->workshopInteracting[0] = 0;
            return 0;
        } 
        else if (init || !workshopCategories ||  (s_bReparse && window_getMode(WDW_RECIPEINVENTORY) == WINDOW_DISPLAYING ))
        {

            // Set up data - only first time.
            if (init) 
            {
                oldScale = sc;
            }

            // create unfiltered list of all recipes to be viewed
            if (detailRecipes)
            {
                eaDestroyConst(&detailRecipes);
            }
            detailRecipes = entity_GetValidDetailRecipes(e, pdetail, e->pl->workshopInteracting); 
            eaQSortConst(detailRecipes, compareRecipe);

            // clean up tree list
            if (pAllNode)
            {
                int i, count = eaSize(&pAllNode->children);

                // clean up trees here! 
                for (i = 0; i < count; i++)
                {
                    recipeTreeRemoveRecipes(pAllNode->children[i]);
                }

                pMemTree = recipeFindChildNode(pAllNode, pMemorizedString);
                if (pMemTree == NULL)
                {
                    pMemTree = uiTreeNewNode();
                    pMemTree->pData = pMemorizedString;
                    pMemTree->state |= UITREENODE_EXPANDED;
                    eaPush(&pAllNode->children, pMemTree);
                    pMemTree->pParent = pAllNode;
                }
            } else {
                // create All tab
                pAllNode = uiTreeNewNode();
                pAllNode->pData = (char *) "AllString";
                pAllNode->state |= UITREENODE_EXPANDED;

                pMemTree = uiTreeNewNode();
                pMemTree->pData = pMemorizedString;
                pMemTree->state |= UITREENODE_EXPANDED;
                eaPush(&pAllNode->children, pMemTree);
                pMemTree->pParent = pAllNode;
            }

            // update tab list
            if (!workshopCategories)
            {
                workshopCategories = uiTabControlCreate(TabType_Undraggable, 0, 0, 0, 0, 0);
            } else {
                pSelectedTab = (char *) uiTabControlGetSelectedData(workshopCategories);
            }

            uiTabControlRemoveAll(workshopCategories);
            uiTabControlAdd(workshopCategories,"AllString", pAllNode->pData);
            for (i = 0; i < eaSize(&detailRecipes);i++)
            {
                const DetailRecipe *pRecipe = detailRecipes[i]; 

                if (!recipeIsVisible(pRecipe))
                    continue;

                // push recipe to correct list
                if (pRecipe->Type == kRecipeType_Memorized)
                {
                    pCurrentTree = recipeFindChildNode(pMemTree, pRecipe->pchDisplayTabName);
                    if (pCurrentTree == NULL)
                    {
                        // create new sub MemTree
                        pCurrentTree = uiTreeNewNode();
                        pCurrentTree->pData = (char *) pRecipe->pchDisplayTabName;
                        eaPush(&pMemTree->children, pCurrentTree);
                        pCurrentTree->pParent = pMemTree;
                    }

                    // push recipe to current list
                    recipeTreeAdd(pCurrentTree, pRecipe, e);
                }
                else 
                {
                    pCurrentTree = recipeFindChildNode(pAllNode, pRecipe->pchDisplayTabName);

                    if (!pCurrentTree)
                    {
                        pCurrentTree = uiTreeNewNode();
                        pCurrentTree->pData = (char *) pRecipe->pchDisplayTabName;
                        eaPush(&pAllNode->children, pCurrentTree);
                        if (eaSize(&pAllNode->children) < 2)
                            pCurrentTree->state |= UITREENODE_EXPANDED;
                        pCurrentTree->pParent = pAllNode;
                    }

                    // push recipe to current list
                    recipeTreeAdd(pCurrentTree, pRecipe, e);
                }
            }

            // prune any unneeded recipe entries from the list
            {
                int i, count = eaSize(&pAllNode->children);

                for (i = count - 1; i >= 0; i--)
                {
                    uiTreeNode *pTree = pAllNode->children[i];
                    if (recipeTreePrune(pTree))
                        uiTreeFree(pTree);
                }
            }

            recipeSetReformat(pAllNode);

            // recreate the tabs
            tabCount = eaSize(&pAllNode->children);
            for (i = 0; i < tabCount; i++)
            {
                uiTabControlAdd(workshopCategories, pAllNode->children[i]->pData, pAllNode->children[i]->pData);
            }

            if (pSelectedTab)
            {
                uiTabControlSelect(workshopCategories, pSelectedTab);
            }

            // sort the lists
            recipeTreeSort(pAllNode);


            ////////////////////////////////////////////////////////////////////
            // not used currently
            // update needed workshop list

            /*
            if (neededWorkshops)
            {
                eaDestroy(&neededWorkshops);
            }
            eaCreate(&neededWorkshops);
            for( i = 0; i < eaSize(&recipeDisplayStates); ++i )
            {
                recipeDisplayStates[i]->recipe = NULL; //make sure we update it
            }
            */


            if (pdetail && strstriConst(pdetail->info->pchName,"empower"))
                s_Empowerment = 1;
            else
                s_Empowerment = 0;

            if (init)
            {    
                s_Hide = 0; 
                comboboxClear(&hideBox);
                combocheckbox_init(&hideBox,0,0,0,200,200,CBOXHEIGHT,400,"HideTitle",WDW_RECIPEINVENTORY,1,0);
                combocheckbox_add(&hideBox,0,NULL,"HideMissingSalvage",HIDE_MISSING_SALVAGE);
                combocheckbox_add(&hideBox,0,NULL,"HideMissingRecipe",HIDE_MISSING_RECIPE);
                
                if (s_InventoryMode) 
                {
                    if (optionGet(kUO_RecipeHideMissingParts))
                        combobox_setId(&hideBox, HIDE_MISSING_SALVAGE, true);
                    if (optionGet(kUO_RecipeHideUnowned))
                        combobox_setId(&hideBox, HIDE_MISSING_RECIPE, true);
                } 
                else 
                {
                    if (optionGet(kUO_RecipeHideMissingPartsBench))
                        combobox_setId(&hideBox, HIDE_MISSING_SALVAGE, true);
                    if (optionGet(kUO_RecipeHideUnownedBench))
                        combobox_setId(&hideBox, HIDE_MISSING_RECIPE, true);
                }

                updateSort(&hideBox);

                ////////////////////////////////////////////////////////////////////
                ////////////////////////////////////////////////////////////////////

                if (!s_InventoryMode)
                    cmdParse("emote invent");
            }
            init = 0;
            s_bReparse = 0;
        } 
        else 
        {
            if (sc != oldScale)
            {
                recipeInventoryReparse();
                oldScale = sc;
            }
        }
        
        oldy = y;
        drawFrame( PIX3, R10, x, y, z, wd, ht, sc, color, bcolor );
        curTab = drawTabControl(workshopCategories, x+R10*sc, y, z, wd - 2*R10*sc, TAB_HEIGHT, sc, color, color, TabDirection_Horizontal );
        set_scissor( 1 ); 

        scissor_dims( x + PIX3*sc, y + (PIX3+TAB_HEIGHT)*sc, wd - PIX3*2*sc, ht - (PIX3*3 + TAB_HEIGHT + CBOXHEIGHT)*sc );

        y += (PIX3*2+TAB_HEIGHT*sc);
        yoffset = y - sb.offset;
        height = ht - (PIX3*3 + TAB_HEIGHT + CBOXHEIGHT)*sc;

        if (window_getMode(WDW_RECIPEINVENTORY) == WINDOW_DISPLAYING)
        {
            char *pSelectedTab = (char *) uiTabControlGetSelectedData(workshopCategories);
            if (pAllNode && pAllNode->pData == pSelectedTab)
            {
                docHeight = uiTreeDisplay(pAllNode, 0, x + PIX3*3*sc, &yoffset, y, z+1, 
                                            wd - PIX3*6*sc, &height, sc, true);
            }
            else if(pAllNode)
            {
                uiTreeNode *pTree = recipeFindChildNode(pAllNode, pSelectedTab);
                if (pTree)
                    docHeight = uiTreeDisplay(pTree, 0, x + PIX3*3*sc, &yoffset, y, z+1, wd - PIX3*6*sc, &height, sc, true);
            }
            else
            {
                docHeight = 0;
            }
        }
        else 
            docHeight = 0.0f;

        set_scissor( 0 );

        // drawn inventory count
        if (e->pchar) 
        {
            int countX = (x + 30.f*sc);
            int countY = (y + ht - 33.f*sc);
            font( &game_9 );
            if (e->pchar->recipeInvCurrentCount < character_GetInvTotalSize(e->pchar, kInventoryType_Recipe))
                font_color(CLR_WHITE, CLR_WHITE);
            else
                font_color(CLR_RED, CLR_RED);
            prnt( countX, countY, z+1, sc, sc, "%d/%d", e->pchar->recipeInvCurrentCount, 
                    character_GetInvTotalSize(e->pchar, kInventoryType_Recipe) );
        }

        // check to see if player has coupons!
        if (!s_InventoryMode)
        {
            int couponX = (90.f*sc);
            int couponY = (ht - 14.f*sc);
            SalvageInventoryItem *sii = NULL;
            sii = character_FindSalvageInv(e->pchar, INVENTION_COUPON_SALVAGE);
            if( sii && sii->amount > 0)
            {
                if (drawSmallCheckboxEx( WDW_RECIPEINVENTORY, couponX, couponY, z+1, sc,
                                            "UseCouponString", &s_useCoupon, NULL, true, NULL, NULL, true, 0 ))
                    recipeInventoryReparse();
            } else {
                s_useCoupon = false;
            }
        } else {
            s_useCoupon = false;
        }

        // draw filter selection
        set_scissor( 0 );
        hideBox.x = wd/sc - 200;
        hideBox.y = ht/sc - CBOXHEIGHT - PIX3;
        hideBox.z = 0;
        combobox_display(&hideBox);
        if (hideBox.changed)
        {
            updateSort(&hideBox);
            sendOptions(0);
        }

        set_scissor( 0 ); 

        doScrollBar( &sb, ht - (PIX3*3 + TAB_HEIGHT + CBOXHEIGHT)*sc, docHeight + SPACE_BETWEEN_NODES*sc, wd, (PIX3+R10)*sc, z, 0, 0 );
    }

    return 0;
}

/* End of File */
