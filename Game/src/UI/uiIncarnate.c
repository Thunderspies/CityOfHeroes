#include "UI/uiIncarnate.h"

#include "entity/incarnate.h"
#include "UI/uiWindows.h"
#include "UI/uiUtil.h"
#include "UI/uiUtilGame.h"
#include "UI/uiInput.h"
#include "UI/uiScrollBar.h"
#include "UI/uiClipper.h"
#include "gameComm/wdwbase.h"
#include "formatter/smf_main.h"

#include "entity/entity.h"
#include "entity/EntPlayer.h"
#include "player/player.h"
#include "entity/character_base.h"
#include "player/badges.h"
#include <utilitieslib/components/Earray.h>
#include "cmdparse/cmdgame.h"
#include "clientcomm/clientcomm.h"
#include <utilitieslib/language/MessageStoreUtil.h>
#include "auth/authUserData.h"
#include "UI/sprite/sprite_base.h"
#include "graphics/textureatlas.h"
#include "UI/uiTabControl.h"
#include "UI/uiRecipeInventory.h"
#include "bases/DetailRecipe.h"
#include "entity/Proficiency.h"
#include "entity/character_workshop.h"
#include "UI/sprite/sprite_text.h"
#include "UI/sprite/sprite_font.h"
#include "UI/uiTray.h"
#include "UI/uiContextMenu.h"
#include "gameComm/trayCommon.h"
#include "UI/uiInfo.h"
#include <utilitieslib/utils/SimpleParser.h>

typedef struct RecipeTreeNode RecipeTreeNode;
static IncarnateSlot currentlySelectedSlot = kIncarnateSlot_Count; // nothing selected

static SMFBlock *slotNames[kIncarnateSlot_Count];
static SMFBlock *slotActivated[kIncarnateSlot_Count];
static SMFBlock *slotContains[kIncarnateSlot_Count];
static RecipeTreeNode *s_ppIncarnateRecipeList = NULL;
static RecipeTreeNode *s_ppIncarnateConversionNode = NULL;
static SMFBlock *inventoryHeader;
static SMFBlock **abilityNames;
static ContextMenu *sPowerContext = 0;
static int s_incarnateMouseOverIndex = -1;

static TextAttribs s_textAttr_TreeText =
{
    /* piBold        */  (int *)0,
    /* piItalic      */  (int *)0,
    /* piColor       */  (int *)0xffffffff,
    /* piColor2      */  (int *)0,
    /* piColorHover  */  (int *)0xffffffff,
    /* piColorSelect */  (int *)0,
    /* piColorSelectBG*/ (int *)0x333333ff,
    /* piScale       */  (int *)(int)(1.0f*SMF_FONT_SCALE),
    /* piFace        */  (int *)&game_9,
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

static TextAttribs s_textAttr_HeaderText =
{
    /* piBold        */  (int *)0,
    /* piItalic      */  (int *)0,
    /* piColor       */  (int *)0xffffffff,
    /* piColor2      */  (int *)0,
    /* piColorHover  */  (int *)0xffffffff,
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

static TextAttribs s_textAttr_HeaderFadeText =
{
    /* piBold        */  (int *)0,
    /* piItalic      */  (int *)0,
    /* piColor       */  (int *)0x666666ff,
    /* piColor2      */  (int *)0,
    /* piColorHover  */  (int *)0x666666ff,
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

#define INVENTORY_WD 120

typedef struct RecipeTreeNode
{
    char *name;
    RecipeTreeNode **children;
    treeRecipeDisplayState *recipeDisplay;
    int selectedIndex;
    SMFBlock *smfBlock;
    int classification;
    int childrenClassified;
    int depth;    //    using base 1
}RecipeTreeNode;

typedef enum IncarnateRecipeEnum
{
    IRE_CREATION,
    IRE_CONVERSION,
    IRE_COUNT,
} IncarnateRecipeEnum;

typedef enum IncarnateVisibilityEnum
{
    IVE_OWNED_NEVER,
    IVE_OWNED_PREVIOUSLY,
    IVE_OWNED_CURRENTLY,
    IVE_COUNT,
} IncarnateVisibilityEnum;

typedef enum IncarnateTabEnum
{
    ITE_POWERS,
    ITE_CREATION,
    ITE_CONVERSION,
    ITE_COUNT,
}IncarnateTabEnum;
static void addToRecipeTree(RecipeTreeNode *tree, char *nodeName, const DetailRecipe *recipe)
{
    assert(tree);
    assert(nodeName);
    assert(recipe);
    if (tree && nodeName && recipe)
    {
        int i;
        char *nameRemains = NULL;

        nameRemains = strchr(nodeName, '|');
        removeTrailingWhiteSpaces(&nodeName);
        if (nameRemains)
        {
            *nameRemains = 0;
            nameRemains++;
            nameRemains = (char*)removeLeadingWhiteSpaces(nameRemains);
        }

        //    try to find it in the tree
        //    if we get a match, we haven't hit the bottom
        if (nameRemains)
        {
            for (i = 0; i < eaSize(&tree->children); ++i)
            {
                if (stricmp(tree->children[i]->name, nodeName) == 0)
                {
                    addToRecipeTree(tree->children[i], nameRemains, recipe);
                    tree->depth = MAX(tree->depth, tree->children[i]->depth + 1);
                    return;
                }
            }
        }

        {
            RecipeTreeNode *child = malloc(sizeof(RecipeTreeNode));
            child->name = nodeName;
            child->recipeDisplay = NULL;
            child->children = NULL;
            child->selectedIndex = -1;
            child->smfBlock = smfBlock_Create();
            child->childrenClassified = 0;
            child->classification = -1;
            child->depth = 1;    //    default depth is 1
            smf_SetFlags(child->smfBlock, SMFEditMode_Unselectable, SMFLineBreakMode_SingleLine, 0, 0,
                SMFScrollMode_ExternalOnly, 0, 0, SMFContextMenuMode_None, SMAlignment_Left, 0, 0, 0);
            smf_SetRawText(child->smfBlock, textStd(nodeName), 0);
            eaPush(&tree->children, child);
            if (nameRemains)
            {
                addToRecipeTree(child, nameRemains, recipe);
            }
            else
            {
                if (recipe)
                {
                    child->recipeDisplay = (treeRecipeDisplayState *) malloc(sizeof(treeRecipeDisplayState));
                    memset(child->recipeDisplay, 0, sizeof(treeRecipeDisplayState));
                    child->recipeDisplay->recipeFormat = smfBlock_Create();
                    child->recipeDisplay->recipe = recipe;
                    child->recipeDisplay->count = 0;
                    child->recipeDisplay->reformat = true;
                }
            }
            tree->depth = MAX(tree->depth, child->depth+1);
        }
    }
}
static int RTNcompare(const RecipeTreeNode **a, const RecipeTreeNode **b ) 
{ 
    return stricmp(textStd((*a)->name), textStd((*b)->name));
}
static void sortRecipeTree(RecipeTreeNode *node)
{
    int i;
    eaQSort(node->children, RTNcompare);
    for (i = 0; i < eaSize(&node->children); ++i)
    {
        sortRecipeTree(node->children[i]);
    }
}

//    data has confirmed that they will only use depth 4 for conversions
#define CONVERSION_TREE_DEPTH 4
static void BuildRecipeTree(const DetailRecipe **originalList)
{
    assert(originalList);
    if (originalList)
    {
        int i;
        s_ppIncarnateRecipeList = calloc(1, sizeof(RecipeTreeNode));
        s_ppIncarnateRecipeList->name = "AllString";
        s_ppIncarnateRecipeList->recipeDisplay = NULL;
        s_ppIncarnateRecipeList->children = NULL;
        s_ppIncarnateRecipeList->selectedIndex = -1;
        s_ppIncarnateRecipeList->childrenClassified = 0;
        s_ppIncarnateRecipeList->classification = -1;
        s_ppIncarnateRecipeList->depth = 0;
        for (i = 0; i < eaSize(&originalList); ++i)
        {
            const DetailRecipe *pRecipe = originalList[i];
            char *pNodeName = strdup(textStd(pRecipe->pchDisplayTabName));
            addToRecipeTree(s_ppIncarnateRecipeList, pNodeName, pRecipe);
        }
        //    sortRecipeTree(s_ppIncarnateRecipeList);
        //    init display types for the tree
        if (!s_ppIncarnateRecipeList->childrenClassified)
        {
            for (i = 0; i < eaSize(&s_ppIncarnateRecipeList->children); ++i)
            {
                RecipeTreeNode *child = s_ppIncarnateRecipeList->children[i];
                child->classification = (child->depth == CONVERSION_TREE_DEPTH) ? IRE_CONVERSION : IRE_CREATION;
                if (child->classification == IRE_CONVERSION)
                {
                    assert(s_ppIncarnateConversionNode == NULL);
                    s_ppIncarnateConversionNode = child;
                }
            }
            s_ppIncarnateRecipeList->childrenClassified = 1;
        }
    }
}

static void s_incarnateTreeReparse(RecipeTreeNode *node)
{
    int i;
    for (i = 0; i < eaSize(&node->children); ++i)
    {
        s_incarnateTreeReparse(node->children[i]);
    }
    if (node->recipeDisplay)
    {
        node->recipeDisplay->reformat = 1;
    }
}
void uiIncarnateReparse()
{
    if (s_ppIncarnateRecipeList)
    {
        s_incarnateTreeReparse(s_ppIncarnateRecipeList);
    }
}

static void drawIncarnateSlot(IncarnateSlot slot, float x, float y, float z, float slotWidth, float slotHeight, float sc, int color)
{
    Entity *e = playerPtr();
    Power *slottedPower;
    CBox box;

    float barWidth = slotWidth-90*sc;
    float barHeight = 15*sc;
    float enablePercent = IncarnateSlot_getActivatePercent(e, slot);
    float filledWidth = min((0 + enablePercent) / 100.0f, 1.0f) * barWidth*sc;

    static AtlasTex *power_hole = NULL;
    static AtlasTex *power_ring = NULL;
    int isActivated = IncarnateSlot_isActivated(e, slot);

    if (!power_hole) {
        power_hole = atlasLoadTexture("EnhncTray_RingHole.tga");
        power_ring = atlasLoadTexture("tray_ring_power.tga");
    }

    // draw slot frame
    if (slot == currentlySelectedSlot)
    {
        if (!isActivated)
        {
            if (enablePercent >= 1.0f)
            {
                drawFrame(PIX3, R10, x, y, z, slotWidth, slotHeight, sc, 0x99999999, 0x55555555);
                drawFrame(PIX3, R10, x + 80*sc, y + 5*sc, z, filledWidth, barHeight, sc, 0x00000000, color - 0x66);
                drawFrame(PIX3, R10, x + 80*sc, y + 5*sc, z, barWidth, barHeight, sc, 0x66666666, 0x00000099);
            }
            else
            {
                drawFrame(PIX3, R10, x, y, z, slotWidth, slotHeight, sc, 0x66666666, 0x55555555);
                drawFrame(PIX3, R10, x + 80*sc, y + 5*sc, z+1, barWidth, barHeight, sc, 0x66666666, 0x00000099);
            }
        }
        else
        {
            drawFrame(PIX3, R10, x, y, z, slotWidth, slotHeight, sc, color, color - 0x80);
        }
    }
    else
    {
        if (!isActivated)
        {
            if (enablePercent >= 1.0f)
            {
                drawFrame(PIX3, R10, x, y, z, slotWidth, slotHeight, sc, 0x99999999, 0x00000099);
                drawFrame(PIX3, R10, x + 80*sc, y + 5*sc, z, barWidth, barHeight, sc, 0x66666666, 0x00000099);
                drawFrame(PIX3, R10, x + 80*sc, y + 5*sc, z, filledWidth, barHeight, sc, 0x00000000, color - 0x66);
            }
            else
            {
                drawFrame(PIX3, R10, x, y, z, slotWidth, slotHeight, sc, 0x66666666, 0x00000066);
                drawFrame(PIX3, R10, x + 80*sc, y + 5*sc, z, barWidth, barHeight, sc, 0x66666666, 0x00000066);
            }
        }
        else
        {
            drawFrame(PIX3, R10, x, y, z, slotWidth, slotHeight, sc, color, color - 0xcc);
        }
    }

    // handle clicking on the slot
    BuildCBox(&box, x, y, slotWidth, slotHeight);
    if (mouseClickHit(&box, MS_LEFT))
    {
        currentlySelectedSlot = slot;
    }
    if (mouseClickHit(&box, MS_RIGHT))
    {
        static TrayObj to_tmp;
        Power *ppow = Incarnate_getEquipped(e, slot);
        if (ppow)
        {
            float cx = (box.lx + box.hx)/2;
            float cy = (box.ly + box.hy)/2;
            buildPowerTrayObj( &to_tmp, ppow->psetParent->idx, ppow->idx, ppow->ppowBase->eType == kPowerType_Auto );
            to_tmp.invIdx = slot;
            contextMenu_setAlign( sPowerContext, cx, cy, CM_LEFT, CM_TOP, &to_tmp );
        }
    }
    if( mouseLeftDrag( &box ) )
    {
        Power *ppow;
        currentlySelectedSlot = slot;
        ppow = Incarnate_getEquipped(e, currentlySelectedSlot);
        if (ppow)
        {
            AtlasTex *icon = atlasLoadTexture(ppow->ppowBase->pchIconName);
            tray_dragPower( ppow->psetParent->idx, ppow->idx, icon );
        }
    }

    // draw slot name
    if (!slotNames[slot])
    {
        slotNames[slot] = smfBlock_Create();
        smf_SetFlags(slotNames[slot], SMFEditMode_Unselectable, SMFLineBreakMode_SingleLine, 0, 0, SMFScrollMode_ExternalOnly, 0, 0, SMFContextMenuMode_None, SMAlignment_Left, 0, 0, 0);
        smf_SetRawText(slotNames[slot], textStd(incarnateSlotDisplayNames[slot]), false);
    }
    if (isActivated || enablePercent >= 1.0f)
    {
        smf_Display(slotNames[slot], x + 10*sc, y + 4*sc, z, slotWidth, 0, 0, 0, &s_textAttr_HeaderText, 0);
    }
    else
    {
        smf_Display(slotNames[slot], x + 10*sc, y + 4*sc, z, slotWidth, 0, 0, 0, &s_textAttr_HeaderFadeText, 0);
    }

    // draw activated/xp earned
    if (!slotActivated[slot])
    {
        slotActivated[slot] = smfBlock_Create();
        smf_SetFlags(slotActivated[slot], SMFEditMode_Unselectable, SMFLineBreakMode_SingleLine, 0, 0, SMFScrollMode_ExternalOnly, 0, 0, SMFContextMenuMode_None, SMAlignment_Center, 0, 0, 0);
    }
    if (!isActivated)
    {
        char temp[50];
        sprintf(temp, "%i%%", (int) IncarnateSlot_getActivatePercent(e, slot));
        smf_SetRawText(slotActivated[slot], temp, false);
        if (enablePercent >= 1.0f)
        {
            smf_Display(slotActivated[slot], x + 80*sc, y + 5*sc, z+1, barWidth, 0, 0, 0, &s_textAttr_TreeText, 0);
        }
        else
        {
            smf_Display(slotActivated[slot], x + 80*sc, y + 4*sc, z+1, barWidth, 0, 0, 0, &s_textAttr_HeaderFadeText, 0);
        }
    }

    // draw currently slotted ability
    if (!slotContains[slot])
    {
        slotContains[slot] = smfBlock_Create();
        smf_SetFlags(slotContains[slot], SMFEditMode_Unselectable, SMFLineBreakMode_MultiLineBreakOnWhitespace, 0, 0, SMFScrollMode_ExternalOnly, 0, 0, SMFContextMenuMode_None, SMAlignment_Left, 0, 0, 0);
    }

    {
        float iconPosX = x+slotWidth-30*sc;
        float iconPosY = y+slotHeight/2;
        if (slottedPower = Incarnate_getEquipped(e, slot))
        {
            AtlasTex *icon = basepower_GetIcon(slottedPower->ppowBase);
            if (slot == currentlySelectedSlot)
            {
                AtlasTex *glow = atlasLoadTexture("costume_button_link_glow.tga");
                display_sprite(glow, iconPosX-glow->width*0.5f*sc, iconPosY-glow->height*0.5*sc, z+1, sc, sc, CLR_WHITE);
            }
            smf_SetRawText(slotContains[slot], textStd(slottedPower->ppowBase->pchDisplayName), false);
            smf_Display(slotContains[slot], x + 10*sc, y + slotHeight-28*sc, z, slotWidth - (40+icon->width*0.5)*sc, 1, 1, 0, &s_textAttr_TreeText, 0);
            display_sprite(icon, iconPosX-icon->width*0.5*sc, iconPosY-icon->height*0.5*sc, z+1, 1.0f*sc, 1.0f*sc, 0xffffffff);
        }
        else if (isActivated)
        {
            display_sprite(power_hole, iconPosX-power_hole->width*0.5*0.5*sc, iconPosY-power_hole->height*0.5*0.5*sc, z, 0.5f*sc, 0.5f*sc, 0xffffffff);
            display_sprite(power_ring, iconPosX-power_ring->width*0.5*sc, iconPosY+0.5*sc-power_ring->height*0.5*sc, z, 1.0f*sc, 1.0f*sc, color);
        }
    }
}

static void drawIncarnateAbility(const Power *ppow, int abilityIndex, float x, float y, float z, float wd, float ht, float sc, int color)
{
    CBox box;
    Entity *e = playerPtr();
    const BasePower *ability = ppow->ppowBase;
    AtlasTex *icon = basepower_GetIcon(ability);
    bool isSlotted = Incarnate_isEquippedByBasePower(e, ability);

    // handle clicking on the ability
    BuildCBox(&box, x, y+3*sc, wd, ht-4*sc);
    //drawBox(&box, z, 0xffffffff, 0);
    if (mouseDoubleClickHit(&box, MS_LEFT))
    {
        char temp[100];
        if (isSlotted)
        {
            sprintf(temp, "incarnate_unequip %s %s", StaticDefineIntRevLookup(ParseIncarnateSlotNames,currentlySelectedSlot), ability->pchName);
        }
        else
        {
            sprintf(temp, "incarnate_equip %s %s", StaticDefineIntRevLookup(ParseIncarnateSlotNames,currentlySelectedSlot), ability->pchName);
        }
        cmdParse(temp);
    }
    if (mouseClickHit(&box, MS_RIGHT))
    {
        static TrayObj to_tmp;
        float cx = (box.lx + box.hx)/2;
        float cy = (box.ly + box.hy)/2;
        buildPowerTrayObj( &to_tmp, ppow->psetParent->idx, ppow->idx, ppow->ppowBase->eType == kPowerType_Auto );
        to_tmp.invIdx = currentlySelectedSlot;
        contextMenu_setAlign( sPowerContext, cx, cy, CM_LEFT, CM_TOP, &to_tmp );
    }
    if (isSlotted)
    {
        drawFlatFrameBox(PIX2, R10, &box, z + 1, CLR_SELECTION_FOREGROUND, CLR_SELECTION_BACKGROUND);
    }
    else if (mouseCollision(&box))
    {
        drawFlatFrameBox(PIX2, R10, &box, z + 1, CLR_MOUSEOVER_FOREGROUND, CLR_MOUSEOVER_BACKGROUND);
    }

    // draw ability icon
    display_sprite(icon, x + 5*sc, y + 6*sc, z + 2, 0.5f*sc, 0.5f*sc, isSlotted ? 0xffffffff : 0xffffffaa);

    // draw ability name
    if (!abilityNames)
    {
        eaCreate(&abilityNames);
    }
    while (eaSize(&abilityNames) <= abilityIndex)
    {
        eaInsert(&abilityNames, 0, eaSize(&abilityNames));
    }
    if (!abilityNames[abilityIndex])
    {
        abilityNames[abilityIndex] = smfBlock_Create();
        smf_SetFlags(abilityNames[abilityIndex], SMFEditMode_ReadOnly, SMFLineBreakMode_MultiLineBreakOnWhitespace, 0, 0, SMFScrollMode_ExternalOnly, 0, 0, SMFContextMenuMode_None, SMAlignment_Left, 0, 0, 0);
    }
    smf_SetRawText(abilityNames[abilityIndex], textStd(ability->pchDisplayName), false);
    smf_Display(abilityNames[abilityIndex], x + 23*sc, y + 8*sc, z + 2, wd - 18 * sc, 0, 0, 0, &s_textAttr_TreeText, 0);
}

static void drawIncarnateInventory(float x, float y, float z, float wd, float ht, float sc, int color)
{
    static int abilityTotalHeight = 0;
    static CBox abilityBox;
    int abilityHeight = 25;
    int abilityBuffer = 0;
    //     int headerHeight = 0;
    const BasePowerSet *slotAbilities;
    int abilityIndex;
    Entity *e = playerPtr();
    int displayIndex = 0;
    static ScrollBar inventoryScrollbar;
    CBox backBox;
    if (currentlySelectedSlot < kIncarnateSlot_Count)
    {
        int string_wd = str_wd((TTDrawContext*)s_textAttr_HeaderText.piFace, sc, sc, slotNames[currentlySelectedSlot]->outputString);


        smf_Display(slotNames[currentlySelectedSlot], x + (wd-string_wd)/2, y + 2*sc, z, wd, 0, 0, 0, &s_textAttr_HeaderText, 0);

        // draw abilities in inventory
        BuildCBox(&abilityBox, x + 3*sc, y + 20*sc, wd - 6*sc, ht - 40*sc);
        clipperPushCBox(&abilityBox);
        slotAbilities = IncarnateSlot_getBasePowerSet(currentlySelectedSlot);
        if (slotAbilities)
        {
            float currentY = (y+20*sc) - inventoryScrollbar.offset;
            for (abilityIndex = 0; abilityIndex < eaSize(&slotAbilities->ppPowers); abilityIndex++)
            {
                const BasePower *bpow = slotAbilities->ppPowers[abilityIndex];
                const Power *ppow;
                if (ppow = character_OwnsPower(e->pchar, bpow))
                {
                    BuildCBox(&backBox, x, currentY, wd, abilityHeight*sc);
                    drawBox(&backBox, z, 0, displayIndex % 2 ? 0xffffff55 : 0x00000055);
                    drawIncarnateAbility(ppow, displayIndex, x+5*sc, currentY, z+1, wd-(10*sc), abilityHeight*sc, sc, color);
                    currentY += (abilityHeight + abilityBuffer)*sc;
                    displayIndex++;
                }
            }
            abilityTotalHeight = (displayIndex * abilityHeight) + (displayIndex * abilityBuffer)*sc;
            while(currentY < (y+ht))
            {
                BuildCBox(&backBox, x, currentY, wd, abilityHeight*sc);
                drawBox(&backBox, z, 0, displayIndex % 2 ? 0xffffff55 : 0x00000055);
                currentY += (abilityHeight + abilityBuffer)*sc;
                displayIndex++;
            }
        }
        clipperPop();
    }
    BuildCBox(&backBox, x, y, wd, ht - 10*sc);
    drawFrameBox(PIX2, R10, &backBox, z, 0, CLR_CONSTRUCT_EX(255, 255, 255, 0.1));

    // draw inventory scrollbar
    doScrollBar(&inventoryScrollbar, ht - 40*sc, abilityTotalHeight, x + wd, y + 20*sc, z, &abilityBox, 0);
}
static void drawIncarnatePowers(float x, float y, float z, float wd, float ht, float sc, int color, int backcolor)
{
    int start_xoffset = 20;
    float yoffset = 10*sc;
    int numRows = 6;
    int inventoryWd = 250;
    float slotWidth = ((wd-((start_xoffset*2) + 10 + inventoryWd)*sc)/2);
    float slotHeight = ((ht - ((numRows-1)*5 + yoffset*2)*sc)/numRows);
    float midpoint_xoffset = start_xoffset+slotWidth+5;

    drawIncarnateSlot(kIncarnateSlot_Omega, x + (midpoint_xoffset-(slotWidth/2)), y + yoffset, z, slotWidth, slotHeight, sc, color);
    yoffset += slotHeight+5*sc;
    drawIncarnateSlot(kIncarnateSlot_Mind, x + (start_xoffset*sc), y + yoffset, z, slotWidth, slotHeight, sc, color);
    drawIncarnateSlot(kIncarnateSlot_Vitae, x + midpoint_xoffset+5*sc, y + yoffset, z, slotWidth, slotHeight, sc, color);
    yoffset += slotHeight+5*sc;
    drawIncarnateSlot(kIncarnateSlot_Hybrid, x + (start_xoffset*sc), y + yoffset, z, slotWidth, slotHeight, sc, color);
    drawIncarnateSlot(kIncarnateSlot_Genesis, x + midpoint_xoffset+5*sc, y + yoffset, z, slotWidth, slotHeight, sc, color);
    yoffset += slotHeight+5*sc;
    drawIncarnateSlot(kIncarnateSlot_Lore, x + (start_xoffset*sc), y + yoffset, z, slotWidth, slotHeight, sc, color);
    drawIncarnateSlot(kIncarnateSlot_Destiny, x + midpoint_xoffset+5*sc, y + yoffset, z, slotWidth, slotHeight, sc, color);
    yoffset += slotHeight+5*sc;
    drawIncarnateSlot(kIncarnateSlot_Judgement, x + (start_xoffset*sc), y + yoffset, z, slotWidth, slotHeight, sc, color);
    drawIncarnateSlot(kIncarnateSlot_Interface, x + midpoint_xoffset+5*sc, y + yoffset, z, slotWidth, slotHeight, sc, color);
    yoffset += slotHeight+5*sc;
    drawIncarnateSlot(kIncarnateSlot_Alpha, x + (midpoint_xoffset-(slotWidth/2)), y + yoffset, z, slotWidth, slotHeight, sc, color);

    //    draw powers information
    drawIncarnateInventory(x+wd-((inventoryWd+10)*sc), y, z, inventoryWd*sc, ht, sc, color);

}
static void clearSelectedIndex(RecipeTreeNode *node)
{
    assert(node);
    if (node)
    {
        if (EAINRANGE(node->selectedIndex, node->children))
        {
            clearSelectedIndex(node->children[node->selectedIndex]);
        }
        node->selectedIndex = -1;
    }
}
static void drawTreeText(float x, float y, float z, float wd, float ht, float sc, RecipeTreeNode *parent, int i )
{
    assert(parent);
    assert(EAINRANGE(i, parent->children));
    if (parent && EAINRANGE(i, parent->children))
    {
        CBox cbox;
        RecipeTreeNode *node = parent->children[i];
        BuildCBox(&cbox, x, y, wd-PIX3*sc, (20-PIX3)*sc);

        smf_Display(node->smfBlock, x+20*sc, y+3*sc, z+1, wd-PIX3*sc, 20, 0, 0, &s_textAttr_TreeText, 0);
        if (parent->selectedIndex == i)
        {
            drawFlatFrameBox(PIX2, R10, &cbox, z, CLR_SELECTION_FOREGROUND, CLR_SELECTION_BACKGROUND);
        }
        else if (mouseCollision(&cbox))
        {
            drawFlatFrameBox(PIX2, R10, &cbox, z, CLR_MOUSEOVER_FOREGROUND, CLR_MOUSEOVER_BACKGROUND);
        }
        else
        {
            drawFlatFrameBox(PIX2, R10, &cbox, z, CLR_NORMAL_FOREGROUND, CLR_NORMAL_BACKGROUND);
        }
        if (mouseClickHit(&cbox, MS_LEFT))
        {
            if (parent->selectedIndex != i)
            {
                clearSelectedIndex(parent);
            }
            parent->selectedIndex = i;
        }
    }
}

static void drawIncarnateTree(float x, float y, float z, float wd, float ht, float sc, int classification, RecipeTreeNode *rootNode)
{
    assert(rootNode);
    if (rootNode)
    {
        int i;
        float docHeight = 0;
        static ScrollBar sb;
        float currentHt = ht;
        float startY = y-sb.offset;
        float yoffset = startY;
        CBox cbox;

        BuildCBox(&cbox, x, y-PIX3*sc, wd, ht+2*PIX3*sc);
        clipperPushCBox(&cbox);
        for (i = 0; i < eaSize(&rootNode->children); ++i)
        {
            RecipeTreeNode *child = rootNode->children[i];

            if (child->classification == classification)
            {
                drawTreeText(x, yoffset, z, wd, ht, sc, rootNode, i);
                if (rootNode->selectedIndex == i)
                {
                    AtlasTex * leftArrow  = atlasLoadTexture( "Jelly_arrow_down.tga");    
                    display_sprite( leftArrow, x+8.0*sc, yoffset+5.0*sc, z, 0.5*sc, sc, CLR_WHITE);
                }
                else
                {
                    AtlasTex * leftArrow  = atlasLoadTexture( "teamup_arrow_expand.tga");    // uses same arrows as uiTray.c
                    display_sprite( leftArrow, x+10.0*sc, yoffset+3.0*sc, z, sc, 0.4*sc, CLR_WHITE);
                }
                yoffset += 20*sc;

                //    if currently selected, display the children at an offset
                if (rootNode->selectedIndex == i)
                {
                    int j;
                    for (j = 0; j < eaSize(&child->children); ++j)
                    {
                        drawTreeText(x+15*sc, yoffset, z, wd-15*sc, ht, sc, child, j);
                        yoffset += 20*sc;
                    }
                }
            }
        }
        clipperPop(&cbox);
        doScrollBar( &sb, ht - (10+PIX3)*2*sc, yoffset-(startY+10), x + wd + (5+PIX3)*sc, y+10*sc, z, &cbox, 0 ); 
    }
}
#define INCARNATE_GRAPH_BOX_SIZE 50

static int drawIncarnateGraphIcon(float x, float y, float z, float wd, float sc, int fcolor, int bcolor, int visibility, AtlasTex *pIcon, const BasePower *bpow)
{
    CBox cbox;
    int boxSize = INCARNATE_GRAPH_BOX_SIZE;
    int retVal = D_NONE;
    int alpha;
    BuildCBox(&cbox, x-boxSize*0.5f*sc, y-boxSize*0.5f*sc, boxSize*sc, boxSize*sc);
    switch (visibility)
    {
    case IVE_OWNED_NEVER:
        alpha = 0xffffff55;
        break;
    case IVE_OWNED_PREVIOUSLY:
        alpha = 0xffffff99;
        break;
    case IVE_OWNED_CURRENTLY:
        alpha = 0xffffffff;
        break;
    default:
        assert(0);    //    unhandled type
    }
    if (pIcon)
    {
        if (mouseCollision(&cbox))
        {
            AtlasTex *glow = atlasLoadTexture("costume_button_link_glow.tga");
            display_sprite(glow, x-glow->width*0.5f*sc, y-glow->height*0.5f*sc, z+1, sc, sc, alpha);
            retVal = D_MOUSEOVER;
        }
        if (bpow && mouseClickHit(&cbox, MS_RIGHT))
        {
            info_Power(bpow, 0);
        }
        if (bpow && mouseClickHit(&cbox, MS_LEFT))
        {
            retVal = D_MOUSEHIT;
        }

        display_sprite(pIcon, x-pIcon->width*0.5f*sc, y-pIcon->height*0.5f*sc, z+2, sc, sc, alpha);
    }

    fcolor &= alpha;
    bcolor &= alpha;

    drawFrameBox(PIX2, R10, &cbox, z, fcolor, bcolor);
    return retVal;
}
static void drawIncarnateGraphIcons(float x, float y, float z, float wd, float sc, int fcolor, int bcolor, RecipeTreeNode *node, int rarity, float *iconPos, int baseRarity)
{
    int i;
    int *matchingRarity = NULL;
    Entity *e = playerPtr();
    int numIcons = eafSize(&iconPos);
    for (i = 0; i < eaSize(&node->children); ++i)
    {
        RecipeTreeNode *child = node->children[i];
        const DetailRecipe *recipe = child->recipeDisplay->recipe;
        if (recipe->Rarity == rarity)
        {
            if (eaiFind(&matchingRarity, child->classification) == -1)
            {
                int size = eaiPush(&matchingRarity, child->classification);
                assert(size < numIcons);        //    found more icons than we are going to draw.. what now?
                if (size < numIcons)
                {
                    AtlasTex *pIcon = atlasLoadTexture(recipe->ui.pchIcon);
                    const BasePower *bpow = powerdict_GetBasePowerByFullName(&g_PowerDictionary, recipe->pchIncarnateReward);
                    int currentlyOwned = character_OwnsPower(e->pchar, bpow) ? IVE_OWNED_CURRENTLY : IVE_OWNED_NEVER;
                    int visibility = (baseRarity >= rarity) ? IVE_OWNED_PREVIOUSLY : IVE_OWNED_NEVER;
                    int retVal = D_NONE;
                    visibility = MAX(currentlyOwned, visibility);
                    if (retVal = drawIncarnateGraphIcon(x+wd*iconPos[size], y, z, sc, sc, (node->selectedIndex == child->classification) ? CLR_WHITE : fcolor, bcolor, visibility, pIcon, bpow))
                    {
                        if (retVal == D_MOUSEHIT)
                        {
                            if (node->selectedIndex != child->classification)
                            {
                                clearSelectedIndex(node);
                            }
                            node->selectedIndex = child->classification;
                        }
                        else if (retVal == D_MOUSEOVER)
                        {
                            s_incarnateMouseOverIndex = child->classification;
                        }
                    }
                }
            }
        }
    }
    for (i = eaiSize(&matchingRarity); i < numIcons; ++i)
    {
        drawIncarnateGraphIcon(x+wd*iconPos[i], y, z, sc, sc, fcolor, bcolor, IVE_OWNED_NEVER, NULL, NULL);
    }
    eaiDestroy(&matchingRarity);
}
static int incarnateGraphGetBaseVisibility(RecipeTreeNode *node)
{
    int returnRarity = kProfRarityType_Common-1;
    assert(node);
    if (node)
    {
        int i;
        Entity *e = playerPtr();
        if (e && e->pchar)
        {
            for (i = 0; i < eaSize(&node->children); ++i)
            {
                RecipeTreeNode *child = node->children[i];
                const DetailRecipe *recipe = child->recipeDisplay->recipe;
                if (recipe->Rarity > returnRarity)
                {
                    if (character_OwnsPower(e->pchar,powerdict_GetBasePowerByFullName(&g_PowerDictionary, recipe->pchIncarnateReward)))
                    {
                        returnRarity = recipe->Rarity;
                    }
                }
            }
        }
    }
    return returnRarity;
}
#define TIME_MAX 90
static void drawAnimatedGraphLine(int size, float x1, float y1, float x2, float y2, float z, float sc, int color, int timer)
{
    //    we are making some assumptions here
    //    since we know its a graph, we can always go from the bottom up when we animate
    //    we also assume that the first point is the start, and the next point is the end
    AtlasTex *blip = atlasLoadTexture("costume_button_link_glow.tga");
    float cornerWidth = drawConnectingLineStyle(size, R6, x1, y1, x2, y2, z, 0, sc, color, kFrameStyle_3D);
    float wd = x2-x1;
    float ht = y2-y1;

    if (timer >= 0)
    {
        float t = timer * 1.f/ TIME_MAX;
        float blipScale = 0.1*sc;
        blipScale += (sinf(2*PI*(timer%60)/60.0f)+1)*0.15f*0.5f;
        if (t > 1.0f)    t = 1.f;
        if (wd && ht)
        {
            float sec[2] = { 0.6f, 0.35f };
            if (t > sec[0])
            {
                float t2 = (t-sec[0])/(1.0f - sec[0]);
                display_sprite(blip, x2-(blip->width*blipScale*0.5f), ((y1-cornerWidth)*(1-t2)+y2*t2)-(blip->height*blipScale*0.5f), z+1, blipScale, blipScale, CLR_WHITE);
            }
            else if (t > sec[1])
            {
                float t2 = (t-sec[1])/(sec[0]-sec[1]);
                float x;
                float y;
                if (x2 > x1)
                {
                    x = x2-cornerWidth;
                    x += cosf((t2+3) * PI * 0.5f)*cornerWidth;
                    y = y1-cornerWidth;
                    y -= sinf((t2+3) * PI * 0.5f)*cornerWidth;
                }
                else
                {
                    x = x2+cornerWidth;
                    x += cosf((3-t2) * PI * 0.5f)*cornerWidth;
                    y = y1-cornerWidth;
                    y -= sinf((3-t2) * PI * 0.5f)*cornerWidth;
                }
                display_sprite(blip, x-(blip->width*blipScale*0.5f),y-(blip->height*blipScale*0.5f), z+1, blipScale, blipScale, CLR_WHITE);
            }
            else
            {
                float t2 = t/sec[1];
                if (x2 > x1)
                {
                    x2 -= cornerWidth;
                }
                else
                {
                    x2 += cornerWidth;
                }
                display_sprite(blip, (x1*(1-t2)+x2*t2)-(blip->width*blipScale*0.5f),y1-(blip->height*blipScale*0.5f), z+1, blipScale, blipScale, CLR_WHITE);
            }
        }
        else
        {
            //    interpolate the point
            display_sprite(blip, (x1*(1-t)+x2*t)-(((blip->width*blipScale))*0.5f), (y1*(1-t)+y2*t)-(((blip->height*blipScale))*0.5f), z+1, blipScale, blipScale, CLR_WHITE);
        }
    }
}
static void drawIncarnateCreationGraph(float x, float y, float z, float wd, float ht, float sc, int fcolor, int bcolor, RecipeTreeNode *node)
{
    assert(node);
    if (node)
    {
        static int timeTracker= 0;
        static float *uniquePos = NULL;
        static float *rarePos= NULL;
        static float *uncommonPos= NULL;
        static float *commonPos= NULL;
        int baseVis = incarnateGraphGetBaseVisibility(node);
        int string_wd = str_wd((TTDrawContext*)s_textAttr_HeaderText.piFace, sc, sc, node->smfBlock->outputString);
        int pwd = PIX2;    //    pipe width
        float icon_wds = (wd+(50+10)*sc-(2.0f*wd/5));
        float new_wd = wd *wd/icon_wds;
        int interpTime;
        int fBasecolorNoAlpha = fcolor & 0xffffff00;
        int fColorVis;
        x += (wd-new_wd)/2;
        wd = new_wd;

        if (!uniquePos)
        {
            //    if you add more pieces here, you have to update the lines =/
            eafPush(&uniquePos, 1/3.f);    eafPush(&uniquePos, 2/3.f);
            eafPush(&rarePos, 0.2f);    eafPush(&rarePos, 0.38f); eafPush(&rarePos, 0.62f);    eafPush(&rarePos, 0.8f);
            eafPush(&uncommonPos, 0.38f);    eafPush(&uncommonPos, 0.62f);
            eafPush(&commonPos, 0.5f);
        }
        timeTracker++;
        if (timeTracker > TIME_MAX)
            timeTracker -= TIME_MAX;

        //    init the tree
        if (!node->childrenClassified)
        {
            int i;
            const char **knownIncarnates = NULL;
            for (i = 0; i < eaSize(&node->children); ++i)
            {
                int index;
                const DetailRecipe *recipe = node->children[i]->recipeDisplay->recipe;
                node->children[i]->classification = -1;
                if ((index = StringArrayFind(knownIncarnates, recipe->pchIncarnateReward))==-1)
                {
                    index = eaPushConst(&knownIncarnates, recipe->pchIncarnateReward);
                }
                node->children[i]->classification = index;
            }
            eaDestroyConst(&knownIncarnates);
            node->childrenClassified = 1;
        }

        smf_Display(node->smfBlock, x+(wd-string_wd)/2, y+0.05f*ht, z, wd, 20, 0, 0, &s_textAttr_HeaderText, 0);

        y += 0.2f*ht;
        interpTime = (baseVis >= kProfRarityType_Rare) ? (timeTracker+TIME_MAX-10)%TIME_MAX : -1;
        fColorVis = fBasecolorNoAlpha | ((baseVis >= kProfRarityType_Rare) ? 0x000000ff : 0x00000044);
        drawIncarnateGraphIcons(x,y,z,wd,sc, fcolor, bcolor, node, kProfRarityType_Unique, uniquePos, baseVis);
        //    horizontal line across
        drawAnimatedGraphLine(pwd, x+wd*(uniquePos[0]+uniquePos[1])/2, y, x+wd*uniquePos[0] + (INCARNATE_GRAPH_BOX_SIZE+PIX2)*0.5f*sc, y, 
            z, sc, fColorVis, interpTime);
        drawAnimatedGraphLine(pwd, x+wd*(uniquePos[0]+uniquePos[1])/2, y, x+wd*uniquePos[1] - (INCARNATE_GRAPH_BOX_SIZE+PIX2)*0.5f*sc, y, 
            z, sc, fColorVis, interpTime);
        //    vertical line down
        drawAnimatedGraphLine(pwd, x+wd*(uniquePos[0]+uniquePos[1])/2-pwd*0.5f, y+0.2f*ht-pwd*0.5f*sc, x+wd*(uniquePos[0]+uniquePos[1])/2-pwd*0.5f, y, 
            z, sc, fColorVis, interpTime);
        y += 0.2f*ht;
        drawIncarnateGraphIcons(x,y,z,wd,sc, fcolor, bcolor, node, kProfRarityType_Rare, rarePos, baseVis);
        //    horizontal line across
        drawAnimatedGraphLine(pwd, x+wd*rarePos[0] + (INCARNATE_GRAPH_BOX_SIZE+PIX2)*0.5f*sc, y+pwd*0.5f, 
            x+wd*rarePos[1] - (INCARNATE_GRAPH_BOX_SIZE)*0.5f*sc, y+pwd*0.5f, z, sc, fColorVis, interpTime);
        drawAnimatedGraphLine(pwd, x+wd*rarePos[1] + (INCARNATE_GRAPH_BOX_SIZE+PIX2)*0.5f*sc, y+pwd*0.5f, 
            x+wd*(rarePos[1]+rarePos[2])*0.5, y+pwd*0.5f, z, sc, fColorVis, interpTime);
        drawAnimatedGraphLine(pwd, x+wd*rarePos[2] - (INCARNATE_GRAPH_BOX_SIZE)*0.5f*sc, y+pwd*0.5f, 
            x+wd*(rarePos[1]+rarePos[2])*0.5+pwd*0.5f*sc, y+pwd*0.5f, z, sc, fColorVis, interpTime);
        drawAnimatedGraphLine(pwd, x+wd*rarePos[3] - (INCARNATE_GRAPH_BOX_SIZE)*0.5f*sc, y+pwd*0.5f, 
            x+wd*rarePos[2] + (INCARNATE_GRAPH_BOX_SIZE+PIX2)*0.5f*sc, y+pwd*0.5f, z, sc, fColorVis, interpTime);

        interpTime = (baseVis >= kProfRarityType_Uncommon) ? (timeTracker+TIME_MAX-5)%TIME_MAX : -1;
        fColorVis = fBasecolorNoAlpha | ((baseVis >= kProfRarityType_Uncommon) ? 0x000000ff : 0x00000044);

        //    vertical line down
        drawAnimatedGraphLine(pwd, x+wd*uncommonPos[0]-pwd*0.5f - ((INCARNATE_GRAPH_BOX_SIZE)*0.5f*sc), y+0.2f*ht, x+wd*rarePos[0]-pwd*0.5f, y-pwd*0.5f*sc + ((INCARNATE_GRAPH_BOX_SIZE)*0.5f*sc), 
            z, sc, fColorVis, interpTime);
        drawAnimatedGraphLine(pwd, x+wd*uncommonPos[0]-pwd*0.5f, y+0.2f*ht - ((INCARNATE_GRAPH_BOX_SIZE)*0.5f*sc), x+wd*rarePos[1]-pwd*0.5f, y-pwd*0.5f*sc + ((INCARNATE_GRAPH_BOX_SIZE)*0.5f*sc), 
            z, sc, fColorVis, interpTime);
        drawAnimatedGraphLine(pwd, x+wd*uncommonPos[1]-pwd*0.5f, y+0.2f*ht - ((INCARNATE_GRAPH_BOX_SIZE)*0.5f*sc), x+wd*rarePos[2]-pwd*0.5f, y-pwd*0.5f*sc + ((INCARNATE_GRAPH_BOX_SIZE)*0.5f*sc), 
            z, sc, fColorVis, interpTime);
        drawAnimatedGraphLine(pwd, x+wd*uncommonPos[1]-pwd*0.5f + ((INCARNATE_GRAPH_BOX_SIZE)*0.5f*sc), y+0.2f*ht, x+wd*rarePos[3]-pwd*0.5f, y-pwd*0.5f*sc + ((INCARNATE_GRAPH_BOX_SIZE)*0.5f*sc), 
            z, sc, fColorVis, interpTime);

        y += 0.2f*ht;
        drawIncarnateGraphIcons(x,y,z,wd,sc, fcolor, bcolor, node, kProfRarityType_Uncommon, uncommonPos, baseVis);
        interpTime = (baseVis >= kProfRarityType_Common) ? timeTracker : -1;    
        fColorVis = fBasecolorNoAlpha | ((baseVis >= kProfRarityType_Common) ? 0x000000ff : 0x00000044);

        //    corner line joints down
        drawAnimatedGraphLine(pwd, x+wd*commonPos[0] - (INCARNATE_GRAPH_BOX_SIZE+PIX2)*0.5f*sc, y+0.2f*ht, 
                    x+wd*uncommonPos[0], y+((INCARNATE_GRAPH_BOX_SIZE)*0.5f*sc),    z, sc, fColorVis, interpTime);
        drawAnimatedGraphLine(pwd, x+wd*commonPos[0]+(INCARNATE_GRAPH_BOX_SIZE+PIX2)*0.5f*sc, y+0.2f*ht,
                    x+wd*uncommonPos[1], y+((INCARNATE_GRAPH_BOX_SIZE)*0.5f*sc), z, sc, fColorVis, interpTime);

        y += 0.2f*ht;
        drawIncarnateGraphIcons(x,y,z,wd,sc, fcolor, bcolor, node, kProfRarityType_Common, commonPos, baseVis);
    }
}
#undef INCARNATE_GRAPH_BOX_SIZE
static void drawIncarnateCreationRecipes(float x, float y, float z, float wd, float ht, float sc, RecipeTreeNode *selectedNode)
{
    CBox cbox;
    static ScrollBar sb;
    float startY = y-sb.offset;
    float yrecipeOffset = startY;
    int i;
    BuildCBox(&cbox, x, y, wd, ht-PIX3);
    clipperPushCBox(&cbox);
    if (s_incarnateMouseOverIndex != -1)
    {
        for (i = 0; i < eaSize(&selectedNode->children); ++i)
        {
            RecipeTreeNode *child = selectedNode->children[i];
            if (child->classification == s_incarnateMouseOverIndex)
            {
                if (child->recipeDisplay)
                {
                    yrecipeOffset += uiIncarnateRecipeDraw(child->recipeDisplay, x, yrecipeOffset, z, wd-8*sc, sc);
                }
            }
        }
    }
    else
    {
        for (i = 0; i < eaSize(&selectedNode->children); ++i)
        {
            RecipeTreeNode *child = selectedNode->children[i];
            if (child->classification == selectedNode->selectedIndex)
            {
                if (child->recipeDisplay)
                {
                    yrecipeOffset += uiIncarnateRecipeDraw(child->recipeDisplay, x, yrecipeOffset, z, wd-8*sc, sc);
                }
            }
        }
    }
    clipperPop();
    doScrollBar( &sb, ht - (10+PIX3)*2*sc, yrecipeOffset-(startY+10*sc), x + wd + PIX3, y+10*sc, z, &cbox, 0 ); 
}

static void drawIncarnateCreation(float x, float y, float z, float wd, float ht, float sc, int color, int backcolor)
{
    s_incarnateMouseOverIndex = -1;
    //    display recipe tree
    drawIncarnateTree(x+5*sc, y, z, INVENTORY_WD*sc, ht, sc, IRE_CREATION, s_ppIncarnateRecipeList);
    
    //    divider
    drawVerticalLine(x+(INVENTORY_WD+15)*sc, y, ht-PIX3, z, 1.f, color);

    if (EAINRANGE(s_ppIncarnateRecipeList->selectedIndex, s_ppIncarnateRecipeList->children))
    {
        RecipeTreeNode *powerChild = s_ppIncarnateRecipeList->children[s_ppIncarnateRecipeList->selectedIndex]; //    god I need better names
        if (powerChild->classification == IRE_CREATION)
        {
            if (EAINRANGE(powerChild->selectedIndex, powerChild->children))
            {
                RecipeTreeNode *patternChild = powerChild->children[powerChild->selectedIndex];    //    god I need better names
                int incarnateTreeWd = MAX((wd - (INVENTORY_WD+15)*sc) /2, 300*sc );
                //    draw Dependecy graph
                drawIncarnateCreationGraph(x+(INVENTORY_WD+15)*sc, y, z, incarnateTreeWd, ht, sc, color, backcolor, patternChild);

                if ((patternChild->selectedIndex >= 0) || (s_incarnateMouseOverIndex != -1))
                {
                    //    draw Info
                    drawIncarnateCreationRecipes(x+incarnateTreeWd+(INVENTORY_WD+15)*sc, y, z, wd-((INVENTORY_WD+20-PIX3)*sc+incarnateTreeWd), ht, sc, patternChild);
                }
            }
        }
    }
}
static void drawIncarnateConversion(float x, float y, float z, float wd, float ht, float sc, int color, int backcolor)
{
    assert(s_ppIncarnateConversionNode);
    if (s_ppIncarnateConversionNode)
    {
        static ScrollBar sb;
        float yrecipeOffset = y-sb.offset;
        float startY = y-sb.offset;

        //    display recipe tree
        drawIncarnateTree(x+5*sc, y, z, INVENTORY_WD*sc, ht, sc, -1, s_ppIncarnateConversionNode);

        drawVerticalLine(x+(INVENTORY_WD+15)*sc, y, ht-PIX3, z, 1.f, color);

        if (EAINRANGE(s_ppIncarnateConversionNode->selectedIndex, s_ppIncarnateConversionNode->children))
        {
            RecipeTreeNode *conversionNode = s_ppIncarnateConversionNode->children[s_ppIncarnateConversionNode->selectedIndex];
            if (EAINRANGE(conversionNode->selectedIndex, conversionNode->children))
            {
                RecipeTreeNode *selectedNode = conversionNode->children[conversionNode->selectedIndex];
                CBox cbox;
                int i;
                BuildCBox(&cbox, x + (INVENTORY_WD + PIX3*5)*sc, y, wd -(INVENTORY_WD + PIX3*9)*sc, ht-PIX3*sc);
                clipperPushCBox(&cbox);
                for (i = 0; i < eaSize(&selectedNode->children); ++i)
                {
                    RecipeTreeNode *recipeChild = selectedNode->children[i];
                    if (recipeChild->recipeDisplay)
                    {
                        yrecipeOffset += uiIncarnateRecipeDraw(recipeChild->recipeDisplay, x + (INVENTORY_WD + PIX3*6)*sc, yrecipeOffset, z, wd -(INVENTORY_WD + PIX3*10)*sc, sc);
                    }
                }
                clipperPop();
                doScrollBar( &sb, ht - (10+PIX3)*2*sc, yrecipeOffset-(startY+10), x + wd + PIX3*sc, y+10*sc, z, &cbox, 0 ); 
            }
        }
    }
}
#undef INVENTORY_WD

static void drawIncarnateTab(float x, float y, float z, float wd, float ht, float sc, int color, int backcolor, int curIncTab)
{
    switch (curIncTab)
    {
    case ITE_POWERS:
        drawIncarnatePowers(x, y+5*sc, z, wd, ht-5*sc, sc, color, backcolor);
        break;
    case ITE_CREATION:
        drawIncarnateCreation(x, y+5*sc, z, wd, ht-5*sc, sc, color, backcolor);
        break;
    case ITE_CONVERSION:
        drawIncarnateConversion(x, y+5*sc, z, wd, ht-5*sc, sc, color, backcolor);
        break;
    default:
        assert(0);    //    unhandled incarnate tab
    }
}

static int incarnatecm_CanSlotPower(void *data)
{
    TrayObj *ts = (TrayObj*)data;
    Entity *e = playerPtr();
    int setIdx = ts->iset;
    int powIdx = ts->ipow;
    int slot = ts->invIdx;
    if (e)
    {
        Power *pow = e->pchar->ppPowerSets[setIdx]->ppPowers[powIdx];
        if (pow)
        {
            if (!Incarnate_isEquippedByBasePower(e, pow->ppowBase))
            {
                return CM_AVAILABLE;
            }
        }
    }
    return CM_HIDE;
}
static int incarnatecm_CanDeSlotPower(void *data)
{
    TrayObj *ts = (TrayObj*)data;
    Entity *e = playerPtr();
    int setIdx = ts->iset;
    int powIdx = ts->ipow;
    int slot = ts->invIdx;
    if (e)
    {
        Power *pow = e->pchar->ppPowerSets[setIdx]->ppPowers[powIdx];
        if (pow)
        {
            if (Incarnate_isEquippedByBasePower(e, pow->ppowBase))
            {
                return CM_AVAILABLE;
            }
        }
    }
    return CM_HIDE;
}
static void incarnatecm_SlotPower(void *data)
{
    TrayObj *ts = (TrayObj*)data;
    Entity *e = playerPtr();
    int setIdx = ts->iset;
    int powIdx = ts->ipow;
    int slot = ts->invIdx;
    if (e)
    {
        Power *pow = e->pchar->ppPowerSets[setIdx]->ppPowers[powIdx];
        if (pow)
        {
            char temp[200];
            const BasePower *ability = pow->ppowBase;
            bool isSlotted = Incarnate_isEquipped(e, slot, ability->pchName);
            if (!isSlotted)
            {
                sprintf(temp, "incarnate_equip %s %s", StaticDefineIntRevLookup(ParseIncarnateSlotNames,slot), ability->pchName);
                cmdParse(temp);
            }
        }
    }
}
static void incarnatecm_DeSlotPower(void *data)
{
    TrayObj *ts = (TrayObj*)data;
    Entity *e = playerPtr();
    int setIdx = ts->iset;
    int powIdx = ts->ipow;
    int slot = ts->invIdx;
    if (e)
    {
        Power *pow = e->pchar->ppPowerSets[setIdx]->ppPowers[powIdx];
        if (pow)
        {
            char temp[200];
            const BasePower *ability = pow->ppowBase;
            bool isSlotted = Incarnate_isEquipped(e, slot, ability->pchName);
            if (isSlotted)
            {
                sprintf(temp, "incarnate_unequip %s %s", StaticDefineIntRevLookup(ParseIncarnateSlotNames,slot), ability->pchName);
                cmdParse(temp);
            }
        }
    }
}
int incarnateWindow()
{
    float x, y, z, wd, ht, sc;
    int color, back_color;
    static uiTabControl *incarnateTabs = NULL;
    int currentTab;
    int activeColorTop;

    if (!window_getDims(WDW_INCARNATE, &x, &y, &z, &wd, &ht, &sc, &color, &back_color))
    {
        return 0;
    }

    activeColorTop = (CLR_GREEN&0xFFFFFF00)|(color&0xff); // allow active tab to only fade so much)

    //    build recipe tree
    if (!s_ppIncarnateRecipeList)
    {
        const DetailRecipe **dr = entity_GetValidDetailRecipes(playerPtr(), NULL, "Worktable_Incarnate");
        if (dr)
        {
            BuildRecipeTree(dr);
        }
        else
        {
            Errorf("No Incarnates found");
            return 0;
        }
    }

    if (!incarnateTabs)
    {
        incarnateTabs = uiTabControlCreate(TabType_Undraggable, 0, 0,0,0,0);
        uiTabControlAdd(incarnateTabs, textStd("IncarnatePowersString"), (int*) ITE_POWERS );
        uiTabControlAdd(incarnateTabs, textStd("IncarnateCreationString"), (int*) ITE_CREATION );
        uiTabControlAdd(incarnateTabs, textStd("IncarnateConversionString"), (int*) ITE_CONVERSION );
        uiTabControlSelect(incarnateTabs, (int*)ITE_POWERS );            
    }

    if (!sPowerContext)
    {
        sPowerContext = contextMenu_Create(NULL);
        contextMenu_addCode( sPowerContext, incarnatecm_CanDeSlotPower, 0, traycm_Add,        0, "CMAddToTray", 0  );
        contextMenu_addCode( sPowerContext, alwaysAvailable, 0, traycm_Remove, 0, "CMRemoveFromTray", 0  );
        contextMenu_addCode( sPowerContext, traycm_InfoAvailable, 0, traycm_Info, 0, "CMInfoString", 0  );
        contextMenu_addCode( sPowerContext, incarnatecm_CanSlotPower, 0, incarnatecm_SlotPower, 0, "CMSlotIncarnateString", 0  );
        contextMenu_addCode( sPowerContext, incarnatecm_CanDeSlotPower, 0, incarnatecm_DeSlotPower, 0, "CMDeSlotIncarnateString", 0  );
    }
    s_textAttr_HeaderFadeText.piScale =(int *)(int)(SMF_FONT_SCALE*sc);
    s_textAttr_HeaderText.piScale =(int *)(int)(SMF_FONT_SCALE*sc);
    s_textAttr_TreeText.piScale =(int *)(int)(SMF_FONT_SCALE*sc);
    //    background
    drawFrame(PIX3, R10, x, y, z, wd, ht, sc, color, back_color);

    //    draw tabs
    currentTab = (int)drawTabControl(incarnateTabs, x+R10*sc, y, z, wd-(R10*sc), ht, sc, color, activeColorTop, TabDirection_Horizontal);
    drawIncarnateTab(x, y+(20*sc), z, wd, ht-(20*sc), sc, color, back_color, currentTab);

    return 0;
}
