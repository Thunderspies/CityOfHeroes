#include "UI/uiCustomVillainGroupWindow.h"
#include "gameComm/wdwbase.h"    //    for window defines
#include "UI/uiUtil.h"        //    for constants like PIX3 and R10
#include "UI/uiWindows.h"    //    for window functions
#include "UI/uiUtilGame.h"    //    for drawFrame
#include "UI/uiUtilMenu.h"    //    for textWithDot
#include "UI/uidialog.h"    //    for dialog windows
#include "uiHelpButton.h"
#include "cmdparse/cmdcommon.h"

#include "storyarc/playerCreatedStoryarcValidate.h"    //    for villain group list
#include "uiScrollSelector.h"                //    for scroll selectors            
#include <utilitieslib/components/Earray.h>                            //    earray functions
#include <utilitieslib/language/MessageStoreUtil.h>                //    for textStd
#include <utilitieslib/utils/error.h>
#include "UI/uiClipper.h"                        //    for clipping
#include "UI/uiBox.h"                            //    for uiBox
#include "UI/sprite/sprite_text.h"
#include "UI/sprite/sprite_font.h"
#include "graphics/ttFontUtil.h"
#include "gameComm/villainDef.h"                        //    for villainDef
#include <utilitieslib/components/estring.h>                        //    for estrings
#include "UI/uiPictureBrowser.h"                //    for picture browser
#include "formatter/smf_main.h"                        //    for smf
#include <utilitieslib/utils/SuperAssert.h>                    //    for assert
#include "entity/PCC_Critter.h"                    //    for custom critters
#include "entity/PCC_Critter_Client.h"                //    for custom critter creation/edit
#include "uiPCCRank.h"
#include "UI/sprite/sprite_base.h"                    //    for display_sprite
#include "entity/powers.h"                            //    for villain powers
#include "entity/CustomVillainGroup.h"
#include "entity/CustomVillainGroup_Client.h"
#include "UI/uiMissionMakerScrollSet.h"        //    for updateCustomCritterList
#include "player/player.h"                            //    for player ptr to validate
#include "filter/validate_name.h"                    //    for filename/displayname validation
#include <utilitieslib/utils/file.h>                        //    for open test
#include <utilitieslib/stdtypes.h>                    //    for open test
#include "UI/uiInput.h"                    //    for mouse Collision
#include "UI/uiTabControl.h"                //    for tabs
#include "UI/uiContextMenu.h"                //    for right clickies
#include "uiNPCCostume.h"

static int init = 0;
static int PADDING = 10;
static int BUTTON_SPACING = 0;
extern MMVillainGroupList gMMVillainGroupList;
extern CustomVG **g_CustomVillainGroups;
extern MMScrollSet missionMaker;
typedef struct VillainListData
{
    int villainGroup;
    int minLevel;
    int maxLevel;
    int villainIndex;
    int villainRank;
    int setNum;
    char *internalName;
    char *name;
    char *cvg_displayName;
    char *cvg_description;
    int added;
    int dontAutoSpawn;
    int refreshPB;
    PCC_Critter *pCritter;
    CustomNPCCostume *npcCostume;
}VillainListData;
typedef struct CustomVillainGroupList
{
    VillainListData **items;
    ScrollBar sb;
    SMFBlock smfBlock;
}CustomVillainGroupList;
typedef enum VillainGroupFilterEnum
{
    VG_FILTER_ALLOW_MINIONS = 1,
    VG_FILTER_ALLOW_LT = 1 << 1,
    VG_FILTER_ALLOW_BOSS = 1 << 2,
    VG_FILTER_ALLOW_ELITE = 1 << 4,
    VG_FILTER_ALLOW_ARCHVILLAIN = 1 << 5,
    VG_FILTER_COUNT = 1 << 6,
    VG_FILTER_ALLOW_VALID_CRITTERS_ONLY = 1 << 7,
    VG_FILTER_ALLOW_CONTACTS = 1 << 8,
}VillainGroupFilterEnum;
typedef enum CVGUIControlEnum
{
    VG_UI_SAVE,
    VG_UI_SAVE_AS,
    VG_UI_SAVE_EXIT,
    VG_UI_ABANDON,
    VG_UI_COUNT,
}CVGUIControlEnum;
typedef enum CVGSortingOptions
{
    VG_SORT_NAME,
    VG_SORT_MIN,
    VG_SORT_MAX,
}CVGSortingOptions;
static int vGroup = -2;
static int selectionOnLeft = 0;
static CustomVillainGroupList availableVillainList;

#define CVGLIST_RANKS    5
static CustomVillainGroupList rightCreatedVillainList[CVGLIST_RANKS];
static MMElementList *pVillainElementList = NULL;
static MMElementList *pCVGOriginalElementList = NULL;
static MMElementList *pCVGCreatedElementList = NULL;
static int levelRange[54];
static VillainGroupFilterEnum vg_Filter = VG_FILTER_COUNT-1;
int CVGroupIndex = -1;
static SMFBlock saveFileName;
static SMFBlock errorMessages;
static int showStandardCritters = 1;
static char s_overwriteFileName[MAX_PATH];
static char s_overwriteDisplayName[MAX_PATH];
static char *MM_selectedCVGName = NULL;
static int selectionInfoMode = WINDOW_SHRINKING;
static float si_time = 0.0f;
static ContextMenu *CVGContextMenu = NULL;
static int rightClickLocation = -2;
static VillainListData *s_currentlySelectedPtr = NULL;
static VillainListData *s_editingVillainPtr = NULL;
static char *sortOptions[3] = { "NameString", "MinLevel", "MaxLevel" };
static int sortOptionStatus = 0;
static int currentSortingOption = -1;
static TextAttribs textAttr_CVGList =
{
    /* piBold        */  (int *)0,
    /* piItalic      */  (int *)0,
    /* piColor       */  (int *)CLR_MM_TEXT,
    /* piColor2      */  (int *)0,
    /* piColorHover  */  (int *)CLR_MM_TEXT,
    /* piColorSelect */  (int *)0,
    /* piColorSelectBG*/ (int *)CLR_MM_TEXT,
    /* piScale       */  (int *)(int)(1.1f*SMF_FONT_SCALE),
    /* piFace        */  (int *)&game_12,
    /* piFont        */  (int *)0,
    /* piAnchor      */  (int *)0,
    /* piLink        */  (int *)0,
    /* piLinkBG      */  (int *)0,
    /* piLinkHover   */  (int *)0,
    /* piLinkHoverBG */  (int *)0,
    /* piLinkSelect  */  (int *)0,
    /* piLinkSelectBG*/  (int *)CLR_MM_TEXT,
    /* piOutline     */  (int *)0,
    /* piShadow      */  (int *)0,
};
static enum CVG_insertionMethod
{
    CVG_IM_TESTING_UNIQUENESS,
    CVG_IM_INSERT,
    CVG_IM_INSERT_AUTO_ALIGN,
}CVG_insertionMethod;
static void updateLevelRanges()
{
    int i,j,k;
    for (i = 0; i < 54; i++)
    {
        levelRange[i] = 0;
    }
    for (i = 0; i < CVGLIST_RANKS; i++)
    {
        for (j = 0; j < eaSize(&(rightCreatedVillainList[i].items)); ++j)
        {
            VillainListData *vgData;
            vgData = rightCreatedVillainList[i].items[j];
            if (!vgData->dontAutoSpawn)
            {
                for (k = (vgData->minLevel-1); k < vgData->maxLevel; k++)
                {
                    levelRange[k] = 1;
                }
            }
        }
    }
}

int sortCVGNames(const MMElement **a, const MMElement **b)
{
    if( !(*a)->pchText || !(*a)->pchDisplayName || stricmp( (*a)->pchText, "NewString" ) == 0 || stricmp( (*a)->pchDisplayName, textStd("NewString") ) == 0 )
        return -1;
    if( !(*b)->pchText || !(*b)->pchDisplayName || stricmp( (*b)->pchText, "NewString" ) == 0 || stricmp( (*b)->pchDisplayName, textStd("NewString") ) == 0 )
        return 1;
    if( !(*a)->pchDisplayName || stricmp( (*a)->pchDisplayName, textStd("AllCritterListText") ) == 0 )
        return -1;
    if( !(*b)->pchDisplayName || stricmp( (*b)->pchDisplayName, textStd("AllCritterListText") ) == 0 )
        return 1;
    //    if they are equal, don't move them,
    //    otherwise, treat normally
    return ( ( ( stricmp( textStd( (*a)->pchDisplayName ), textStd( (*b)->pchDisplayName ) ) ) >= 0 ) ? 1 : -1 );
}
static int sortCVGData(const VillainListData **a_ptr, const VillainListData **b_ptr)
{
    const VillainListData *a, *b;
    if (sortOptionStatus & (1 << currentSortingOption))
    {
        a = *a_ptr;
        b = *b_ptr;
    }
    else
    {
        a = *b_ptr;
        b = *a_ptr;
    }
    switch (currentSortingOption)
    {
    case VG_SORT_NAME:
        return stricmp(a->name, b->name);
    case VG_SORT_MIN:
        return (a->minLevel > b->minLevel) ? 1 : -1;
    case VG_SORT_MAX:
        return (a->maxLevel > b->maxLevel) ? 1 : -1;
    }
    return 0;
}
static char *getVGTextFromRank(int rank)
{
    switch (rank)
    {
    case VR_SMALL:
        return "CVGSmallString";
    case VR_MINION:
        return "CVGMinionString";
    case VR_SNIPER:        //    intentional fall through
    case VR_LIEUTENANT:
        return "CVGLtString";
    case VR_BOSS:
        return "CVGBossString";
    case VR_ELITE:
        return "CVGEliteString";
    case VR_PET:
        return "CVGPetString";
    case VR_ARCHVILLAIN:    //    intentional fall through
    case VR_ARCHVILLAIN2:
        return "CVGArchVillainString";
    default:
        return "noneString";
    }
}


void editCVG_setup(char *cvgName)
{
    //    set selection to cvgName
    showStandardCritters = 0;
    if (MM_selectedCVGName)
    {
        estrDestroy(&MM_selectedCVGName);
    }
    estrCreate(&MM_selectedCVGName);
    if (cvgName && (stricmp(cvgName, "none") != 0) )
    {
        estrConcatCharString(&MM_selectedCVGName, cvgName);
    }
    else
    {
        estrConcatCharString(&MM_selectedCVGName, textStd("NewString"));
    }
    if (window_getMode(WDW_CUSTOMVILLAINGROUP) != WINDOW_DISPLAYING )
    {
        window_setMode(WDW_CUSTOMVILLAINGROUP, WINDOW_GROWING);
    }
    else
    {
        window_bringToFront(WDW_CUSTOMVILLAINGROUP);
    }
}
void freeVillainGroupData( VillainListData *VLData)
{
    if (VLData)
    {
        estrDestroy(&VLData->name);
        estrDestroy(&VLData->internalName);
        if (VLData->cvg_displayName)
        {
            estrDestroy(&VLData->cvg_displayName);
        }
        if (VLData->cvg_description)
        {
            estrDestroy(&VLData->cvg_description);
        }
        if (VLData->npcCostume)
        {
            StructDestroy(ParseCustomNPCCostume, VLData->npcCostume);
        }
        free(VLData);
    }    
}
static int passedFilter(int rank, PCC_Critter *pcc)
{
    if (pcc)
    {
        if ( vg_Filter & VG_FILTER_ALLOW_VALID_CRITTERS_ONLY )
        {
            if ( isPCCCritterValid(playerPtr(), pcc,0,0) != PCC_LE_VALID )
            {
                return 0;
            }
        }
        if ( (vg_Filter & VG_FILTER_ALLOW_ARCHVILLAIN ) && (rank == PCC_RANK_ARCH_VILLAIN) )
        {
            return 1;
        }
        if ( (vg_Filter & VG_FILTER_ALLOW_ELITE ) && (rank == PCC_RANK_ELITE_BOSS) )
        {
            return 1;
        }
        if ( (vg_Filter & VG_FILTER_ALLOW_BOSS) && (rank == PCC_RANK_BOSS) )
        {
            return 1;
        }
        if ( (vg_Filter & VG_FILTER_ALLOW_LT) && (rank == PCC_RANK_LT))
        {
            return 1;
        }
        if ( (vg_Filter & VG_FILTER_ALLOW_MINIONS) && (rank == PCC_RANK_MINION) )
        {
            return 1;
        }
        if ( (vg_Filter & VG_FILTER_ALLOW_CONTACTS) && (rank == PCC_RANK_CONTACT) )
        {
            return 1;
        }
    }
    else
    {
        if ( (vg_Filter & VG_FILTER_ALLOW_ARCHVILLAIN) && ( (rank == VR_ARCHVILLAIN) || (rank == VR_ARCHVILLAIN2) ) )
        {
            return 1;
        }
        if ( (vg_Filter & VG_FILTER_ALLOW_ELITE) && ( (rank == VR_ELITE) ) )
        {
            return 1;
        }
        if ( (vg_Filter & VG_FILTER_ALLOW_BOSS) && (rank >= VR_BOSS) && (rank != VR_ARCHVILLAIN) && (rank != VR_ARCHVILLAIN2) && (rank != VR_ELITE) )
        {
            return 1;
        }
        if ( (vg_Filter & VG_FILTER_ALLOW_LT) && ((rank == VR_SNIPER) || (rank == VR_LIEUTENANT) ) )
        {
            return 1;
        }
        if ( (vg_Filter & VG_FILTER_ALLOW_MINIONS) && ( (rank == VR_MINION) || (rank == VR_SMALL) ) )
        {
            return 1;
        }
    }
    return 0;
}
static void addVillainGroup( MMVillainGroup *pGroup, MMElementList *pList, int set )
{
    MMElement * pElement;
    char tmp[256];

    if( pGroup->pchExclude && ( ( stricmp(pGroup->pchExclude, "NotAGroup") == 0 ) || 
                                    ( stricmp( pGroup->pchExclude,  "ContactAndPersonOnly" ) == 0 ) ) )
        return;

    pElement = StructAllocRaw(sizeof(MMElement));

    sprintf( tmp, "%s (%i-%i)", textStd(pGroup->pchDisplayName), pGroup->minLevel, pGroup->maxLevel);
    pElement->pchText = StructAllocString(pGroup->pchName );
    pElement->pchDisplayName = StructAllocString(tmp);

    pElement->minLevel = pGroup->minLevel;
    pElement->maxLevel = pGroup->maxLevel;
    pElement->setnum = set;
    pElement->npcIndex = pGroup->vg;

    eaPush(&pList->ppElement, pElement);
    pList->current_element = 0;
}

#define VG_LINE_HEIGHT 30
static int addToVillainGroupListUnique( CustomVillainGroupList *list, VillainListData *data, int uniquenessTest)
{
    int i = 0;
    int matchCritters;
    matchCritters = data->pCritter ? 1 : 0;    
    for (i = 0; i < eaSize(&list->items); i++)
    {
        VillainListData *listData;
        listData = list->items[i];
        //    if matching critters, ignore items without critters
        if ( matchCritters && (!listData->pCritter) )
        {
            continue;
        }
        if (!matchCritters && (listData->pCritter))
        {
            continue;
        }
        if  ( (stricmp( listData->internalName, data->internalName ) == 0) && (listData->minLevel == data->minLevel)
            && (listData->maxLevel == data->maxLevel) && (listData->villainGroup == data->villainGroup) &&
                ( (listData->cvg_displayName == data->cvg_displayName) || (listData->cvg_displayName && data->cvg_displayName && (stricmp(listData->cvg_displayName, data->cvg_displayName) == 0 )))    
                )
        {
            break;
        }
    }
    if ( (list != &availableVillainList) && (uniquenessTest==CVG_IM_INSERT_AUTO_ALIGN) )
    {
        float sc;
        window_getDims( WDW_CUSTOMVILLAINGROUP, 0, 0,0, 0,0, &sc, 0, 0 );
        list->sb.offset = MAX(((i)*18*sc), 0);
    }
    if (i >= eaSize(&list->items))
    {
        if (uniquenessTest != CVG_IM_TESTING_UNIQUENESS)
            eaPush(&list->items, data);
        return 1;
    }
    return 0;
}
static int insertIntoRightVillainList(VillainListData *vlData, int uniquenessTest)
{
    int isUnique = 0;
    STATIC_INFUNC_ASSERT(CVGLIST_RANKS == 5);
    if (vlData->pCritter)
    {
        switch (vlData->villainRank)
        {
        case PCC_RANK_MINION:
            isUnique = addToVillainGroupListUnique(&rightCreatedVillainList[0], vlData, uniquenessTest);
            break;
        case PCC_RANK_LT:
            isUnique = addToVillainGroupListUnique(&rightCreatedVillainList[1], vlData, uniquenessTest);
            break;
        case PCC_RANK_BOSS:
            isUnique = addToVillainGroupListUnique(&rightCreatedVillainList[2], vlData, uniquenessTest);
            break;
        case PCC_RANK_ELITE_BOSS:
            isUnique = addToVillainGroupListUnique(&rightCreatedVillainList[3], vlData, uniquenessTest);
            break;
        case PCC_RANK_ARCH_VILLAIN:
            isUnique = addToVillainGroupListUnique(&rightCreatedVillainList[4], vlData, uniquenessTest);
            break;
        }
    }
    else
    {
        switch (vlData->villainRank)
        {
        case VR_SMALL:
        case VR_MINION:
            isUnique = addToVillainGroupListUnique(&rightCreatedVillainList[0], vlData, uniquenessTest);
            break;
        case VR_LIEUTENANT:
        case VR_SNIPER:        //    intentional fallthrough
            isUnique = addToVillainGroupListUnique(&rightCreatedVillainList[1], vlData, uniquenessTest);
            break;
        case VR_PET:
        case VR_BOSS:
            isUnique = addToVillainGroupListUnique(&rightCreatedVillainList[2], vlData, uniquenessTest);
            break;
        case VR_ELITE:
            isUnique = addToVillainGroupListUnique(&rightCreatedVillainList[3], vlData, uniquenessTest);
            break;
        case VR_ARCHVILLAIN:
        case VR_ARCHVILLAIN2:    //    intentional fallthrough
            isUnique = addToVillainGroupListUnique(&rightCreatedVillainList[4], vlData, uniquenessTest);
            break;
        }
    }
    return isUnique;
}
static void transferVillainGroup(VillainListData *existingVG)
{
    VillainListData *newVG;
    newVG = calloc(1, sizeof(VillainListData));
    newVG->villainGroup = existingVG->villainGroup;
    newVG->villainIndex = existingVG->villainIndex;
    newVG->villainRank = existingVG->villainRank;
    newVG->pCritter = existingVG->pCritter;                //    copy pointer
    estrCreate(&newVG->internalName);
    estrConcatCharString(&newVG->internalName, existingVG->internalName);
    newVG->minLevel = existingVG->minLevel;
    newVG->maxLevel = existingVG->maxLevel;
    newVG->setNum = existingVG->setNum;
    newVG->dontAutoSpawn = existingVG->dontAutoSpawn;
    if (existingVG->cvg_displayName)
    {
        estrCreate(&newVG->cvg_displayName);
        estrConcatCharString(&newVG->cvg_displayName, existingVG->cvg_displayName);
    }
    else
    {
        newVG->cvg_displayName = NULL;
    }
    if (existingVG->cvg_description)
    {
        estrCreate(&newVG->cvg_description);
        estrConcatCharString(&newVG->cvg_description, existingVG->cvg_description);
    }
    else
    {
        newVG->cvg_description = NULL;
    }
    if (existingVG->npcCostume)
    {
        newVG->npcCostume = StructAllocRaw(sizeof(CustomNPCCostume));
        StructCopy(ParseCustomNPCCostume, existingVG->npcCostume, newVG->npcCostume, 0 , 0);
    }
    else
    {
        newVG->npcCostume = NULL;
    }
    newVG->added = 0;
    estrCreate(&newVG->name);

    estrConcatCharString(&newVG->name, existingVG->name );

    if (!insertIntoRightVillainList(newVG, CVG_IM_INSERT_AUTO_ALIGN))
    {
        //    delete since it wasn't added
        freeVillainGroupData(newVG);
    }
    else
    {
        existingVG->added = 1;
    }
}
static int removeSelectedCritter(CustomVillainGroupList *rightList, VillainListData*villainToRemove)
{
    int j;
    if ( (villainToRemove->pCritter) && 
        ( (stricmp(pCVGCreatedElementList->ppElement[pCVGCreatedElementList->current_element]->pchText, 
        villainToRemove->pCritter->villainGroup) == 0 ) ||
        ( stricmp(pCVGCreatedElementList->ppElement[pCVGCreatedElementList->current_element]->pchText, textStd("AllCritterListText")) == 0 ) ) )
    {
        //    don't remove the critter from his home group
        //    or the all group
        return 0;
    }
    for (j = 0; j < eaSize(&rightList->items); j++)
    {
        VillainListData* listVillain;
        listVillain = (VillainListData*)(rightList->items[j]);
        //    there should only be one instance of the custom critter and everything should point there
        //     so their pointers should be equal and their name should be the same
        //    standard villains should all have NULL pointers
        //    when matching names, make sure that they are both standard critters by making sure that both ptr's are NULL -EL
        if ( listVillain->internalName && villainToRemove->internalName && (stricmp(listVillain->internalName, villainToRemove->internalName) == 0 ) &&    
            (listVillain->pCritter == villainToRemove->pCritter) && (listVillain->minLevel == villainToRemove->minLevel) &&
            (listVillain->maxLevel == villainToRemove->maxLevel) )
        {
            break;
        }
    }
    if (j >= eaSize(&rightList->items))
    {
        //    something went wrong
        //     we couldn't find the critter currently selected
        return 0;
    }
    else
    {
        VillainListData *removedVillain;
        removedVillain = eaRemove(&(rightList->items), j);
        assert(removedVillain);
        //    if we are drawing this one, stop drawing it
        //    UI style preference. Determines whether or not removal is immediate or only applies on save. When commented, 
        //    it only applies on save, which I think works a bit more intuitively. But leaving it in here, in case that isn't
        //    the desired direction. -EL
        //if (removedVillain->pCritter)
        //{
        //    if (stricmp(g_CustomVillainGroups[removedVillain->villainGroup]->displayName, textStd("AllCritterListText")) != 0 )
        //    {
        //        eaRemove(&g_CustomVillainGroups[removedVillain->villainGroup]->customVillainList, removedVillain->villainIndex);
        //        updateCustomCritterList(&missionMaker);
        //        CVGroupIndex = -1;
        //        vGroup = -1;
        //    }
        //}
        freeVillainGroupData(removedVillain);
        return 1;
    }
}
void clearVillainGroupListItems( CustomVillainGroupList *villainList)
{
    int i;
    for ( i = 0; i < eaSize(&villainList->items); ++i )
    {
        if (villainList->items[i] == s_currentlySelectedPtr)    s_currentlySelectedPtr = NULL;
        freeVillainGroupData(villainList->items[i]);
    }
    dialogClearQueue(0);
    eaDestroy(&villainList->items);
}
static int drawVillainGroupListItems(CustomVillainGroupList *villainList, float x, float startY, float z, float width, float sc, int isCreatedSide, float minClipY, float maxClipY)
{
    int i;
    float endY;
    int newlySelected = 0;
    int *piScale = textAttr_CVGList.piScale;
    char *dontSpawnText;
    endY = startY;
    textAttr_CVGList.piScale = (int *)(int)(1.1f*SMF_FONT_SCALE*sc);
    smf_SetFlags(&villainList->smfBlock, SMFEditMode_Unselectable, SMFLineBreakMode_MultiLineBreakOnWhitespace, 
        SMFInputMode_AnyTextNoTagsOrCodes, -1, SMFScrollMode_ExternalOnly, 
        SMFOutputMode_StripAllTags, SMFDisplayMode_AllCharacters, SMFContextMenuMode_None,
        SMAlignment_Left, 0, 0, 0);
    estrCreate(&dontSpawnText);
    estrConcatf(&dontSpawnText, "<span align=center><color Gray>%s</color></span>", textStd("CVGCMAutoSpawnPrompt"));
    for (i = 0; i < eaSize(&villainList->items); ++i)
    {
        float newH = 0;
        CBox box;
        if(  ( (endY+(18*sc)) > minClipY) && ( ( endY - (18*sc) ) < maxClipY ) )
        {
            char *nameString;
            estrCreate(&nameString);
            estrConcatf(&nameString, "<color %s>%s</color>", villainList->items[i]->dontAutoSpawn ? "Gray": "Architect",villainList->items[i]->name);
            smf_SetRawText(&villainList->smfBlock, nameString, 0);
            newH = MAX(smf_Display(&villainList->smfBlock,x+PIX3+R10,endY, z, width, 18*sc, 0, 0, &textAttr_CVGList, NULL), (18*sc));
            estrDestroy(&nameString);
            if (villainList->items[i]->added)
            {
                smf_SetFlags(&villainList->smfBlock, SMFEditMode_Unselectable, 0, SMFInputMode_None, 30, SMFScrollMode_ExternalOnly, 
                    SMFOutputMode_StripAllTags, SMFDisplayMode_AllCharacters, SMFContextMenuMode_None, SMAlignment_Right, 0, "CVGMenu", -1);
                smf_SetRawText(&villainList->smfBlock, textStd("CVGAddedString"), 0);
                smf_Display(&villainList->smfBlock, x, endY, z, width-(20*sc), 20*sc, 0, 0, &textAttr_CVGList, NULL);
                smf_SetFlags(&villainList->smfBlock, SMFEditMode_Unselectable, SMFLineBreakMode_MultiLineBreakOnWhitespace, 
                    SMFInputMode_AnyTextNoTagsOrCodes, -1, SMFScrollMode_ExternalOnly, 
                    SMFOutputMode_StripAllTags, SMFDisplayMode_AllCharacters, SMFContextMenuMode_None,
                    SMAlignment_Left, 0, 0, 0);
            }
            BuildCBox(&box, x, endY+(2*sc), width-(5*sc), newH-(1*sc) );
            if (villainList->items[i]->dontAutoSpawn && (width > (650.0f*sc)) )
            {
                smf_SetRawText(&villainList->smfBlock, dontSpawnText, 0);
                smf_Display(&villainList->smfBlock, x, endY, z, width, 20*sc, 0,0, &textAttr_CVGList, NULL);
            }
            if (s_currentlySelectedPtr == villainList->items[i])
            {
                //                drawFlatFrame(PIX2, R10, x+PIX3, endY+1, z, width-2*PIX3, newH+2, 1.0f, 0x77777777, 0x48796C77);
                drawBox(&box, z+1, 0, 0x48796C77 );
                if (selectionOnLeft)
                {
                    if (!villainList->items[i]->added)
                    {
                        drawListButton("CVGAddQuestionPrompt", x + width - (60*sc), endY-1, z, 50*sc, 15*sc, sc);
                    }
                }
                else
                {
                    if ( (villainList->items[i]->pCritter) && (villainList->items[i]->pCritter->villainGroup) && 
                        (stricmp(pCVGCreatedElementList->ppElement[pCVGCreatedElementList->current_element]->pchText, 
                        villainList->items[i]->pCritter->villainGroup) == 0) )
                    {
                        smf_SetFlags(&villainList->smfBlock, SMFEditMode_Unselectable, 0, SMFInputMode_None, 30, SMFScrollMode_ExternalOnly, 
                            SMFOutputMode_StripAllTags, SMFDisplayMode_AllCharacters, SMFContextMenuMode_None, SMAlignment_Right, 0, "CVGMenu", -1);
                        smf_SetRawText(&villainList->smfBlock, textStd("CVGCantRemoveFromOriginalGroupPrompt"), 0);
                        smf_Display(&villainList->smfBlock, x, endY, z, width-(20*sc), 20*sc, 0, 0, &textAttr_CVGList, NULL);
                        smf_SetFlags(&villainList->smfBlock, SMFEditMode_Unselectable, SMFLineBreakMode_MultiLineBreakOnWhitespace, 
                            SMFInputMode_AnyTextNoTagsOrCodes, -1, SMFScrollMode_ExternalOnly, 
                            SMFOutputMode_StripAllTags, SMFDisplayMode_AllCharacters, SMFContextMenuMode_None,
                            SMAlignment_Left, 0, 0, 0);
                    }
                    else
                    {
                        drawListButton("CVGRemoveQuestionPrompt", x + width - (80*sc), endY-1, z, (70*sc), (15*sc), sc);
                    }

                }
            }
            if (mouseCollision(&box))
            {
                if (s_currentlySelectedPtr != villainList->items[i])
                    drawBox(&box, z+1, 0, 0x1c2f2e77 );
                if (!s_editingVillainPtr)
                {
                    s_editingVillainPtr = villainList->items[i];
                }
            }

            if (mouseClickHit(&box, MS_LEFT))
            {
                villainList->items[i]->refreshPB = 1;
                if ( s_currentlySelectedPtr == villainList->items[i] )
                {
                    if (isCreatedSide)
                    {
                        int success;
                        success = removeSelectedCritter(villainList, villainList->items[i]);
                        if (success)
                        {
                            dialogClearQueue(0);
                            s_currentlySelectedPtr = NULL;
                            updateLevelRanges();
                            vGroup = -1;
                        }
                    }
                    else
                    {
                        transferVillainGroup(villainList->items[i]);
                        updateLevelRanges();
                    }
                }
                else
                {
                    dialogClearQueue(0);
                    s_currentlySelectedPtr = villainList->items[i];
                    newlySelected = 1;
                }
            }
            if (mouseDoubleClickHit(&box, MS_LEFT))
            {
                if (isCreatedSide)
                {
                    int success;
                    success = removeSelectedCritter(villainList, villainList->items[i]);
                    if (success)
                    {
                        dialogClearQueue(0);
                        s_currentlySelectedPtr = NULL;
                        updateLevelRanges();
                        vGroup = -1;
                    }
                }
                else
                {
                    transferVillainGroup(villainList->items[i]);
                    updateLevelRanges();
                }
            }
        }
        else
        {
            newH = 18*sc;
        }
        endY += newH;
    }
    if (newlySelected)
    {
        if (villainList != &availableVillainList)    
        {
            selectionOnLeft = 0;
        }
        else
        {
            selectionOnLeft = 1;
            selectionInfoMode = WINDOW_GROWING;
        }
    }

    textAttr_CVGList.piScale = piScale;
    estrDestroy(&dontSpawnText);
    return (endY - startY);
}
static void drawVillainGroupList(CustomVillainGroupList *villainList, float x, float y, float z, float width, float height, 
                                 float sc, int isCreatedSide)
{
    CBox box;
    UIBox listViewDrawArea;
    int listHeight;
    uiBoxDefine(&listViewDrawArea, x, y, width, height-2);
    BuildCBox(&box, x, y, width, height -2);
    clipperPush(&listViewDrawArea);
    if ( mouseClickHit( &box, MS_RIGHT) )
    {
        int rx, ry;
        rightClickCoords( &rx, &ry );
        contextMenu_set( CVGContextMenu, rx, ry, 0 );
        if ( isCreatedSide )
        {
            int i;
            for (i = 0; i < CVGLIST_RANKS; ++i)
            {
                if (villainList == (&rightCreatedVillainList[i]))
                {
                    rightClickLocation = i;
                    s_editingVillainPtr = NULL;        //    set it to NULL before the villain groups are drawn to make sure that they are over something
                }
            }
        }
        else
        {
            rightClickLocation = -1;
        }
    }
    listHeight = drawVillainGroupListItems(villainList, x+5, y-villainList->sb.offset, z, width-5, sc, isCreatedSide, y, y+height);
    clipperPop();
    doScrollBar(&villainList->sb, height-30, listHeight, x+width, y+15, z+10, &box, &listViewDrawArea );
}
static void addRandomVillains(CustomVillainGroupList *villainList, int vgIndex, int minLevel, int maxLevel, int setNum, int missingFilter)
{
    int i;
    for (i = 2; i >=0; --i)
    {
        if ((vg_Filter&missingFilter) & (1 << i) )
        {
            VillainListData *VLData;
            VLData = calloc(1, sizeof(VillainListData));
            VLData->villainGroup = vgIndex;
            VLData->villainIndex = -(i+1);
            VLData->cvg_displayName = NULL;
            VLData->cvg_description = NULL;
            estrCreate(&VLData->internalName);
            switch (i)
            {
            case 0:
                VLData->villainRank = VR_MINION;
                estrConcatStaticCharArray(&VLData->internalName, "RandomMinion");
                break;
            case 1:
                VLData->villainRank = VR_LIEUTENANT;
                estrConcatStaticCharArray(&VLData->internalName, "RandomLt");
                break;
            case 2:
                VLData->villainRank = VR_BOSS;
                estrConcatStaticCharArray(&VLData->internalName, "RandomBoss");
                break;
            }
            VLData->pCritter = NULL;

            VLData->minLevel = minLevel;
            VLData->maxLevel = maxLevel;
            VLData->setNum = setNum;
            VLData->dontAutoSpawn = 0;
            VLData->refreshPB = 1;
            VLData->npcCostume = NULL;
            estrCreate(&VLData->name);

            estrConcatf(&VLData->name, "%s - %s (%d - %d)", textStd(villainGroupGetPrintName(vgIndex)), textStd(VLData->internalName), minLevel, maxLevel );
            VLData->added = !insertIntoRightVillainList(VLData, CVG_IM_TESTING_UNIQUENESS);
            eaPushFront(&availableVillainList.items, VLData);
        }
    }
}
static void drawLeftStandardVillainList(int npcIndex, int minAllowedLevel, int maxAllowedLevel, int setNum, float x, float y, float z, float width, float height, float sc, int refreshVillainList)
{
    if (npcIndex != -1)
    {

        drawVillainGroupList(&availableVillainList, x, y, z, width, height, sc, 0);
        if (refreshVillainList 
            && &availableVillainList )
        {
            int i,j;
            int missingFilter = ~(VG_FILTER_ALLOW_MINIONS | VG_FILTER_ALLOW_LT | VG_FILTER_ALLOW_BOSS);
            clearVillainGroupListItems(&availableVillainList);
            for (i = 0; i < eaSize(&villainDefList.villainDefs); i++)
            {
                if ( (npcIndex == villainDefList.villainDefs[i]->group) && 
                    CVG_isValidRank( villainDefList.villainDefs[i]->rank, 0) &&
                    eaSize(&villainDefList.villainDefs[i]->levels) &&
                    ((villainDefList.villainDefs[i]->levels[0]->level <= maxAllowedLevel) &&
                    (villainDefList.villainDefs[i]->levels[eaSize(&villainDefList.villainDefs[i]->levels)-1]->level >= minAllowedLevel ))&&
                    playerCreatedStoryarc_isValidVillainDefName(villainDefList.villainDefs[i]->name, playerPtr() ) )
                {
                    const char *prevName = NULL;
                    int minLevel;
                    int maxLevel;
                    if (!passedFilter(villainDefList.villainDefs[i]->rank, NULL) )
                    {
                        continue;
                    }
                    switch (cvg_randomVillainDefRankMatch(villainDefList.villainDefs[i]->rank))
                    {
                    case 1:
                        missingFilter |= VG_FILTER_ALLOW_MINIONS;
                        break;
                    case 2:
                        missingFilter |= VG_FILTER_ALLOW_LT;
                        break;
                    case 3:
                        missingFilter |= VG_FILTER_ALLOW_BOSS;
                        break;
                    }
                    minLevel = 54;
                    maxLevel = 0;
                    if (eaSize(&villainDefList.villainDefs[i]->levels) > 0)
                    {
                        prevName = villainDefList.villainDefs[i]->levels[0]->costumes[0];
                    }                    
                    for (j = 0; j < eaSize(&villainDefList.villainDefs[i]->levels); j++)
                    {
                        if ( prevName && villainDefList.villainDefs[i]->levels[j]->costumes[0] && 
                            ( stricmp( textStd(prevName), textStd(villainDefList.villainDefs[i]->levels[j]->costumes[0]) ) == 0 ) )
                        {
                            minLevel = (villainDefList.villainDefs[i]->levels[j]->level < minLevel) ? villainDefList.villainDefs[i]->levels[j]->level : minLevel;
                            maxLevel = (villainDefList.villainDefs[i]->levels[j]->level > maxLevel) ? villainDefList.villainDefs[i]->levels[j]->level : maxLevel;
                        }
                        else
                        {
                            VillainListData *VLData;
                            VLData = calloc(1, sizeof(VillainListData));
                            VLData->villainGroup = i;
                            VLData->villainIndex = j-1;
                            VLData->villainRank = villainDefList.villainDefs[i]->rank;
                            VLData->cvg_displayName = NULL;
                            VLData->cvg_description = NULL;
                            estrCreate(&VLData->internalName);
                            estrConcatCharString(&VLData->internalName, villainDefList.villainDefs[i]->name);
                            VLData->minLevel = MAX(minLevel, minAllowedLevel);
                            VLData->maxLevel = MIN(maxLevel, maxAllowedLevel);
                            VLData->pCritter = NULL;
                            VLData->dontAutoSpawn = 0;
                            VLData->npcCostume = NULL;
                            VLData->refreshPB = 1;
                            VLData->setNum = setNum;
                            estrCreate(&VLData->name);

                            estrConcatf(&VLData->name, "%s - %s (%i - %i)", textStd(villainDefList.villainDefs[i]->levels[j-1]->displayNames[0]), 
                                textStd(getVGTextFromRank(VLData->villainRank)), VLData->minLevel, VLData->maxLevel );
                            VLData->added = !insertIntoRightVillainList(VLData, CVG_IM_TESTING_UNIQUENESS);                            
                            eaPush(&availableVillainList.items, VLData);
                            //    reset
                            minLevel = 54;
                            maxLevel = 0;
                        }
                        if (villainDefList.villainDefs[i]->levels[j]->costumes[0])    prevName = villainDefList.villainDefs[i]->levels[j]->costumes[0];
                    }
                    if (prevName)
                    {
                        VillainListData *VLData;
                        VLData = calloc(1, sizeof(VillainListData));
                        VLData->villainGroup = i;
                        VLData->villainIndex = j-1;
                        VLData->villainRank = villainDefList.villainDefs[i]->rank;
                        VLData->pCritter = NULL;
                        VLData->cvg_displayName = NULL;
                        VLData->cvg_description = NULL;
                        estrCreate(&VLData->internalName);
                        estrConcatCharString(&VLData->internalName, villainDefList.villainDefs[i]->name);
                        VLData->minLevel = MAX(minLevel, minAllowedLevel);
                        VLData->maxLevel = MIN(maxLevel, maxAllowedLevel);
                        VLData->dontAutoSpawn = 0;
                        VLData->npcCostume = NULL;
                        VLData->refreshPB = 1;
                        VLData->setNum = setNum;

                        estrCreate(&VLData->name);

                        estrConcatf(&VLData->name, "%s - %s (%i - %i)", textStd(villainDefList.villainDefs[i]->levels[j-1]->displayNames[0]), 
                            textStd(getVGTextFromRank(VLData->villainRank)), VLData->minLevel, VLData->maxLevel );
                        VLData->added = !insertIntoRightVillainList(VLData, CVG_IM_TESTING_UNIQUENESS);
                        eaPush(&availableVillainList.items, VLData);
                    }
                }
            }
            eaQSort(availableVillainList.items, sortCVGData);
            addRandomVillains(&availableVillainList, npcIndex, minAllowedLevel, maxAllowedLevel, setNum, missingFilter);
        }
    }
}

static void drawRightVillainList(float x, float y, float z, float width, float height, float sc, const int *visibleFrames)
{
    int visibleCount = 0;
    int i;
    for (i = 0; i < CVGLIST_RANKS; i++)
    {
        if (visibleFrames[i] == WINDOW_DISPLAYING)
        {
            visibleCount++;
        }
    }
    if (visibleCount)
    {
        float nextY = 0;
        height -= (CVGLIST_RANKS-visibleCount)*30*sc;
        
        for(i = 0; i < CVGLIST_RANKS; i++)
        {
            if (visibleFrames[i] == WINDOW_DISPLAYING)
            {
                drawVillainGroupList(&rightCreatedVillainList[i], x, y+(15*sc)+nextY, z, width, (height/visibleCount)-(47*sc), sc, 1);
                nextY += height/visibleCount;
            }
            else
            {
                nextY += 30*sc;
            }
        }
    }
}
#define CVG_COVERAGE_GOOD_COLOR 0x58913cFF
#define CVG_COVERAGE_HOLE_COLOR 0xcf3d00ff
static void drawLevelRanges(float x, float y, float z, float width, float sc, int example)
{
    AtlasTex * end = atlasLoadTexture( "VillianGroupLevelCoverageBar_end.tga" );
    AtlasTex * highlight_end = atlasLoadTexture( "VillianGroupLevelCoverageBar_highlight_end.tga" );
    AtlasTex * highlight_mid = atlasLoadTexture( "VillianGroupLevelCoverageBar_highlight_mid.tga" );
    AtlasTex * mid = atlasLoadTexture( "VillianGroupLevelCoverageBar_mid.tga" );
    AtlasTex * notch_large = atlasLoadTexture( "VillianGroupLevelCoverageBar_notch_large.tga" );
    AtlasTex * notch_small = atlasLoadTexture( "VillianGroupLevelCoverageBar_notch_small.tga" );
    AtlasTex * shadow_end = atlasLoadTexture( "VillianGroupLevelCoverageBar_shadow_end.tga" );
    AtlasTex * shadow_mid = atlasLoadTexture( "VillianGroupLevelCoverageBar_shadow_mid.tga" );
    int i;
    float sizeOfOneLevel;
    sizeOfOneLevel = (width-(2*end->width*sc))/54.0f;
    if (example)
    {
        display_sprite( end, x+(sizeOfOneLevel)-(end->width*sc), y, z, sc, sc, CVG_COVERAGE_HOLE_COLOR);
    }
    else
    {
        display_sprite( end, x+(sizeOfOneLevel)-(end->width*sc), y, z, sc, sc, levelRange[0] ? CVG_COVERAGE_GOOD_COLOR : CVG_COVERAGE_HOLE_COLOR);
    }

    display_sprite( shadow_end, x+(sizeOfOneLevel)-(end->width*sc), y-1, z, sc, sc, 0x000000FF);
    display_sprite( highlight_end, x+(sizeOfOneLevel)-(end->width*sc), y-1, z, sc, sc, CLR_WHITE );

    for (i = 0; i < 54; ++i)
    {
        if (!example)
        {
            if ((i+1) % 10)
            {
                display_sprite(notch_small, x+((i+0)*sizeOfOneLevel), y+(end->height*sc), z, sc, sc, CLR_MM_TEXT & 0xFFFFFFAA);
            }
            else
            {
                char *number;
                estrCreate(&number);
                display_sprite(notch_large, x+((i+0)*sizeOfOneLevel), y+(end->height*sc), z, sc, sc, CLR_MM_TEXT & 0xFFFFFFAA);
                estrConcatf(&number, "%d", i+1);
                cprntEx(x+((i+0)*sizeOfOneLevel),y+((end->height+notch_large->height + 13)*sc),z,sc, sc, CENTER_X, number);
                estrDestroy(&number);
            }
        }
        if (i < 53)
        {
            if (example)
            {
                display_sprite(mid, x+((i+0)*sizeOfOneLevel), y+(sc*end->height/2)-(sc*mid->height/2), z, sizeOfOneLevel/(mid->width), sc*end->height/(mid->height), (i >= 27) ? CVG_COVERAGE_GOOD_COLOR : CVG_COVERAGE_HOLE_COLOR );
            }
            else
            {
                display_sprite(mid, x+((i+0)*sizeOfOneLevel), y+(sc*end->height/2)-(sc*mid->height/2), z, sizeOfOneLevel/(mid->width), sc*end->height/(mid->height), levelRange[i] ? CVG_COVERAGE_GOOD_COLOR : CVG_COVERAGE_HOLE_COLOR );
            }
            display_sprite(highlight_mid, x+((i+0)*sizeOfOneLevel), y+(sc*end->height/2)-((sc*mid->height/2) + 1), z, sizeOfOneLevel/(mid->width), sc*end->height/(mid->height), CLR_WHITE );
            display_sprite(shadow_mid, x+((i+0)*sizeOfOneLevel), y+(sc*end->height/2)-((sc*mid->height/2)+1), z, sizeOfOneLevel/(mid->width), sc*end->height/(mid->height), 0x000000FF );
        }
    }
    if (example)
    {
        display_spriteFlip( end, x+width-(2*((end->width*sc)+sizeOfOneLevel)), y, z, sc, sc, CVG_COVERAGE_GOOD_COLOR, FLIP_HORIZONTAL);
    }
    else
    {
        display_spriteFlip( end, x+width-(2*((end->width*sc)+sizeOfOneLevel)), y, z, sc, sc, levelRange[53] ? CVG_COVERAGE_GOOD_COLOR : CVG_COVERAGE_HOLE_COLOR, FLIP_HORIZONTAL);
    }
    display_spriteFlip( shadow_end, x+width-(2*((end->width*sc)+sizeOfOneLevel)), y-1, z, sc, sc,  0x000000FF, FLIP_HORIZONTAL);
    display_spriteFlip( highlight_end, x+width-(2*((end->width*sc)+sizeOfOneLevel)), y-1, z, sc, sc, CLR_WHITE, FLIP_HORIZONTAL);
}

static void populateVillainListFromCVG(MMElementList *pList, int insertIntoRight)
{
    //    clear lists
    int i;
    if (insertIntoRight)
    {
        for (i = 0; i < CVGLIST_RANKS; ++i)
        {
            clearVillainGroupListItems(&rightCreatedVillainList[i]);
        }
    }
    else
    {
        clearVillainGroupListItems( &availableVillainList );
    }
    if ( (pList->current_element >= 0) && (pList->ppElement[pList->current_element]->pchText) )
    {
        for (i = 0; i < eaSize(&g_CustomVillainGroups); ++i)
        {
            if (g_CustomVillainGroups[i]->displayName && 
                ( stricmp(g_CustomVillainGroups[i]->displayName, pList->ppElement[pList->current_element]->pchText) == 0 ) )
            {
                int j;
                for (j = 0; j < eaSize(&(g_CustomVillainGroups[i]->customVillainList)); ++j)
                {
                    PCC_Critter *pcc;
                    VillainListData *VLData;
                    pcc = g_CustomVillainGroups[i]->customVillainList[j];
                    VLData = calloc(1, sizeof(VillainListData));
                    VLData->villainGroup = i;
                    VLData->villainIndex = j;
                    VLData->villainRank = pcc->rankIdx;
                    VLData->pCritter = pcc;
                    VLData->cvg_displayName = NULL;
                    VLData->cvg_description = NULL;
                    estrCreate(&VLData->internalName);
                    estrConcatCharString(&VLData->internalName, pcc->referenceFile);

                    VLData->minLevel = 1;
                    VLData->maxLevel = 54;
                    VLData->added = 0;
                    VLData->npcCostume = NULL;
                    VLData->refreshPB = 1;
                    VLData->setNum = 0;
                    VLData->dontAutoSpawn = StringArrayFind(g_CustomVillainGroups[i]->dontAutoSpawnPCCs, g_CustomVillainGroups[i]->customVillainList[j]->referenceFile) != -1 ? 1 : 0;
                    estrCreate(&VLData->name);

                    estrConcatf(&VLData->name, "%s - %s (%i - %i)", pcc->name, getPCCRankSelectionTitleText(pcc->rankIdx), 1, 54 );
                    if (insertIntoRight)
                    {
                        if (!insertIntoRightVillainList(VLData, CVG_IM_INSERT))
                        {
                            //    delete since it wasn't added
                            freeVillainGroupData(VLData);
                        }
                    }
                    else
                    {
                        int wasAdded = 0;
                        if (passedFilter(VLData->villainRank, pcc))
                        {
                            if (addToVillainGroupListUnique( &availableVillainList, VLData, CVG_IM_INSERT))
                            {
                                wasAdded = 1;
                            }
                            VLData->added = !insertIntoRightVillainList(VLData, CVG_IM_TESTING_UNIQUENESS);
                        }
                        if (!wasAdded)
                        {
                            freeVillainGroupData(VLData);
                        }
                    }
                }
                for (j = 0; j < eaSize(&(g_CustomVillainGroups[i]->existingVillainList)); ++j)
                {
                    CVG_ExistingVillain *ev;
                    char *internalName;
                    int pListIndex = -1;
                    ev = g_CustomVillainGroups[i]->existingVillainList[j];
                    internalName = ev->costumeIdx;
                    if (    (stricmp( internalName, "RandomBoss" ) == 0)  || 
                        (stricmp( internalName, "RandomLt" ) == 0) || 
                        (stricmp( internalName, "RandomMinion" ) == 0) )
                    {
                        MMVillainGroup *mmGroup = NULL;
                        if (ev->pchName)
                        {
                            mmGroup = playerCreatedStoryArc_GetVillainGroup( ev->pchName, 
                                ev->setNum );
                        }
                        if (mmGroup)
                        {
                            VillainListData *VLData;
                            VLData = calloc(1, sizeof(VillainListData));

                            VLData->villainGroup = mmGroup->vg ;
                            VLData->cvg_displayName = NULL;
                            VLData->cvg_description = NULL;
                            estrCreate(&VLData->internalName);
                            if (stricmp( internalName, "RandomBoss") == 0)
                            {
                                VLData->villainRank = VR_BOSS;
                                VLData->villainIndex = -3;
                            }
                            else if (stricmp(internalName, "RandomLt") == 0)
                            {
                                VLData->villainRank = VR_LIEUTENANT;
                                VLData->villainIndex = -2;
                            }
                            else
                            {
                                VLData->villainRank = VR_MINION;
                                VLData->villainIndex = -1;
                            }
                            estrConcatCharString(&VLData->internalName, internalName);
                            VLData->pCritter = NULL;

                            VLData->minLevel = mmGroup->minLevel;
                            VLData->maxLevel = mmGroup->maxLevel;
                            VLData->setNum = ev->setNum;
                            estrCreate(&VLData->name);

                            VLData->added = !insertIntoRightVillainList(VLData, CVG_IM_TESTING_UNIQUENESS);
                            estrConcatf(&VLData->name, "%s - %s (%d - %d)", textStd(villainGroupGetPrintName(VLData->villainGroup)), textStd(VLData->internalName), VLData->minLevel, VLData->maxLevel );
                            VLData->added = 0;
                            VLData->dontAutoSpawn = 0;
                            VLData->npcCostume = NULL;
                            VLData->refreshPB = 1;
                            if (insertIntoRight)
                            {
                                if (!insertIntoRightVillainList(VLData, CVG_IM_INSERT))
                                {
                                    //    delete since it wasn't added
                                    freeVillainGroupData(VLData);
                                }
                            }
                            else
                            {
                                int wasAdded = 0;

                                if (passedFilter(VLData->villainRank, NULL))
                                {
                                    if (addToVillainGroupListUnique( &availableVillainList, VLData, CVG_IM_INSERT))
                                    {
                                        wasAdded = 1;
                                    }
                                    VLData->added = !insertIntoRightVillainList(VLData, CVG_IM_TESTING_UNIQUENESS);
                                }
                                if (!wasAdded)
                                {
                                    freeVillainGroupData(VLData);
                                }
                            }
                        }
                    }
                    else
                    {
                        const VillainDef *vdef = villainFindByName(ev->pchName);
                        if (vdef )
                        {
                            for (pListIndex = 0; pListIndex < eaSize(&(pVillainElementList->ppElement) ); ++pListIndex)
                            {
                                if (pVillainElementList->ppElement[pListIndex]->pchText &&
                                    ( stricmp(textStd(pVillainElementList->ppElement[pListIndex]->pchText), 
                                    textStd(villainGroupGetPrintName(vdef->group))) == 0 ) )
                                {
                                    break;
                                }
                            }
                            if (pListIndex != -1)
                            {
                                int m;
                                int k;
                                int maxLevel, minLevel;
                                int villainIndex;
                                minLevel = 54;
                                maxLevel = 0;
                                villainIndex = -1;
                                for (m = 0; m < eaSize(&vdef->levels); ++m)
                                {
                                    if (stricmp(textStd(vdef->levels[m]->costumes[0]), ev->costumeIdx) == 0)
                                    {
                                        villainIndex = m;
                                        maxLevel = (maxLevel < vdef->levels[m]->level) ? vdef->levels[m]->level : maxLevel;
                                        minLevel = (minLevel > vdef->levels[m]->level) ? vdef->levels[m]->level : minLevel;
                                    }
                                }
                                for (k = 0; k < eaSize(&villainDefList.villainDefs); ++k)
                                {
                                    if ( stricmp(vdef->name, villainDefList.villainDefs[k]->name) == 0 )
                                    {
                                        break;
                                    }
                                }
                                if ( (villainIndex != -1) && (k < eaSize(&villainDefList.villainDefs) ) )
                                {
                                    MMVillainGroup *mmGroup = NULL;
                                    VillainListData *VLData;
                                    VLData = calloc(1, sizeof(VillainListData));
                                    VLData->villainIndex = villainIndex;
                                    VLData->villainRank = vdef->rank;
                                    VLData->villainGroup = k;
                                    mmGroup = playerCreatedStoryArc_GetVillainGroup(villainGroupGetName(vdef->group), ev->setNum);
                                    if (ev->displayName)
                                    {
                                        estrCreate(&VLData->cvg_displayName);
                                        estrConcatCharString(&VLData->cvg_displayName, ev->displayName);
                                    }
                                    else
                                    {
                                        VLData->cvg_displayName = NULL;
                                    }
                                    if (ev->description)
                                    {
                                        estrCreate(&VLData->cvg_description);
                                        estrConcatCharString(&VLData->cvg_description, ev->description);
                                    }
                                    else
                                    {
                                        VLData->cvg_description = NULL;
                                    }

                                    estrCreate(&VLData->internalName);
                                    estrConcatCharString(&VLData->internalName, vdef->name);

                                    VLData->minLevel = MAX(minLevel, mmGroup->minLevel);
                                    VLData->maxLevel = MIN(maxLevel, mmGroup->maxLevel);
                                    VLData->pCritter = NULL;
                                    VLData->added = 0;
                                    VLData->refreshPB = 1;
                                    VLData->setNum = ev->setNum;
                                    VLData->dontAutoSpawn = StringArrayFind(g_CustomVillainGroups[i]->dontAutoSpawnEVs, ev->pchName) != -1 ? 1 : 0;;

                                    if (ev->npcCostume)
                                    {
                                        VLData->npcCostume = StructAllocRaw(sizeof(CustomNPCCostume));
                                        StructCopy(ParseCustomNPCCostume, ev->npcCostume, VLData->npcCostume, 0,0);
                                    }
                                    else
                                    {
                                        VLData->npcCostume = NULL;
                                    }
                                    estrCreate(&VLData->name);
                                    if (VLData->cvg_displayName)
                                        estrConcatf(&VLData->name, "%s \"%s\" - %s (%i - %i)", textStd(vdef->levels[villainIndex]->displayNames[0]), VLData->cvg_displayName, textStd(getVGTextFromRank(VLData->villainRank)), VLData->minLevel, VLData->maxLevel );
                                    else
                                        estrConcatf(&VLData->name, "%s - %s (%i - %i)", textStd(vdef->levels[villainIndex]->displayNames[0]), textStd(getVGTextFromRank(VLData->villainRank)), VLData->minLevel, VLData->maxLevel );
                                    if (insertIntoRight)
                                    {
                                        if (!insertIntoRightVillainList(VLData, CVG_IM_INSERT))
                                        {
                                            //    delete since it wasn't added
                                            freeVillainGroupData(VLData);
                                        }
                                    }
                                    else
                                    {
                                        int wasAdded = 0;

                                        if (passedFilter(VLData->villainRank, NULL))
                                        {
                                            if (addToVillainGroupListUnique( &availableVillainList, VLData, CVG_IM_INSERT))
                                            {
                                                wasAdded = 1;
                                            }
                                            VLData->added = !insertIntoRightVillainList(VLData, CVG_IM_TESTING_UNIQUENESS);
                                        }
                                        if (!wasAdded)
                                        {
                                            freeVillainGroupData(VLData);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

static void addVillainsToCVG(CustomVG *vg)
{
    int i,j;
    for ( i = 0; i < CVGLIST_RANKS; ++i )
    {
        for ( j = 0; j < eaSize(&(rightCreatedVillainList[i].items)); ++j )
        {
            VillainListData *villain;
            villain = rightCreatedVillainList[i].items[j];
            if (!(villain->pCritter))
            {
                CVG_ExistingVillain *ev_ptr;
                ev_ptr = StructAllocRaw(sizeof(CVG_ExistingVillain));
                if (stricmp(villain->internalName, "RandomMinion") == 0)
                {
                    ev_ptr->pchName = StructAllocString(villainGroupGetName(villain->villainGroup));
                    ev_ptr->costumeIdx = StructAllocString("RandomMinion");
                    ev_ptr->setNum = villain->setNum;
                }
                else if (stricmp(villain->internalName, "RandomLt") == 0)
                {
                    ev_ptr->pchName = StructAllocString(villainGroupGetName(villain->villainGroup));
                    ev_ptr->costumeIdx = StructAllocString("RandomLt");
                    ev_ptr->setNum = villain->setNum;
                }
                else if (stricmp(villain->internalName, "RandomBoss") == 0)
                {
                    ev_ptr->pchName = StructAllocString(villainGroupGetName(villain->villainGroup));
                    ev_ptr->costumeIdx = StructAllocString("RandomBoss");
                    ev_ptr->setNum = villain->setNum;
                }
                else
                {
                    int villainGroupIdx, villainLevelIdx;
                    villainGroupIdx = villain->villainGroup;
                    villainLevelIdx = villain->villainIndex;

                    ev_ptr->costumeIdx = StructAllocString(textStd(villainDefList.villainDefs[villainGroupIdx]->levels[villainLevelIdx]->costumes[0]));
                    ev_ptr->pchName = StructAllocString(textStd(villainDefList.villainDefs[villainGroupIdx]->name));
                    ev_ptr->levelIdx = villainLevelIdx;
                    ev_ptr->setNum = villain->setNum;
                    if (villain->cvg_displayName && villain->cvg_displayName[0])
                    {
                        if (ev_ptr->displayName)
                        {
                            StructFreeString(ev_ptr->displayName);
                        }
                        ev_ptr->displayName = StructAllocString(villain->cvg_displayName);
                    }
                    else
                    {
                        if (ev_ptr->displayName)
                        {
                            StructFreeString(ev_ptr->displayName);
                            ev_ptr->displayName = NULL;
                        }
                    }
                    if (villain->cvg_description && villain->cvg_description[0])
                    {
                        if (ev_ptr->description)
                        {
                            StructFreeString(ev_ptr->description);
                        }
                        ev_ptr->description = StructAllocString(villain->cvg_description);
                    }
                    else
                    {
                        if (ev_ptr->description)
                        {
                            StructFreeString(ev_ptr->description);
                            ev_ptr->description = NULL;
                        }
                    }
                    if (villain->npcCostume)
                    {
                        if (!ev_ptr->npcCostume)
                        {
                            ev_ptr->npcCostume = StructAllocRaw(sizeof(CustomNPCCostume));
                        }
                        StructCopy(ParseCustomNPCCostume, villain->npcCostume, ev_ptr->npcCostume, 0, 0);
                    }
                    else
                    {
                        if (ev_ptr->npcCostume)
                        {
                            StructDestroy(ParseCustomNPCCostume, ev_ptr->npcCostume);
                            ev_ptr->npcCostume = NULL;
                        }
                    }
                }
                if (villain->dontAutoSpawn)
                {
                    eaPush(&vg->dontAutoSpawnEVs, StructAllocString(ev_ptr->pchName));
                }
                eaPush(&(vg->existingVillainList), ev_ptr);

            }
            else
            {
                //    only push the pointer
                if (villain->dontAutoSpawn)
                {
                    eaPush(&vg->dontAutoSpawnPCCs, StructAllocString(villain->pCritter->referenceFile));
                }
                eaPush(&(vg->customVillainList), villain->pCritter);
            }
        }
    }
}
static int saveCVG(char *fileName, char *displayName)
{
    CustomVG *currentCustomVG;
    int i;
    int retVal;
    i = CVG_CVGExists(displayName);
    currentCustomVG = StructAllocRaw(sizeof(CustomVG));
    currentCustomVG->displayName = StructAllocString(displayName);
    addVillainsToCVG(currentCustomVG);
    retVal = CVG_isValidCVG( currentCustomVG, playerPtr(),0,0 );
    if (stricmp(currentCustomVG->displayName, textStd("AllCritterListText")) == 0)
    {
        //    don't let them name the critter list the default all critter list text =/
        retVal |= CVG_LE_INVALID_NAME;
    }
    if (retVal == CVG_LE_VALID)
    {
        int j;
        if (i == -1)
        {
            eaPush( &g_CustomVillainGroups, currentCustomVG );
        }
        else
        {
            CVG_clearVillains(g_CustomVillainGroups[i]);
            StructFreeString(g_CustomVillainGroups[i]->displayName);
            StructFree(g_CustomVillainGroups[i]);
            g_CustomVillainGroups[i] = currentCustomVG;
        }
        ParserWriteTextFile(fileName, parse_CustomVG, currentCustomVG, 0, 0 );
        if (currentCustomVG->sourceFile)
        {
            StructFreeString(currentCustomVG->sourceFile);
        }
        currentCustomVG->sourceFile = StructAllocString(fileName);
        updateCustomCritterList(&missionMaker);
        populateCVGSS();
        for (j = 0; j < eaSize(&(pCVGCreatedElementList->ppElement)); ++j)
        {
            if (stricmp(pCVGCreatedElementList->ppElement[j]->pchText, currentCustomVG->displayName) == 0)
            {
                pCVGCreatedElementList->current_element = j;
                CVGroupIndex = vGroup = -1;
                s_currentlySelectedPtr = NULL;
                dialogClearQueue(0);
            }
        }
    }
    else
    {
        CVG_clearVillains(currentCustomVG);
        StructFreeString(currentCustomVG->displayName);
        StructFree(currentCustomVG);
    }
    init = 0;
    return retVal;
}
static int validate_CVGName(char *text)
{
    ValidateNameResult vnr = ValidateCustomObjectName(text, 50, 0);
    if (vnr != VNR_Valid)
    {
        dialogStd( DIALOG_OK, "InvalidName", NULL, NULL, NULL, NULL, DLGFLAG_ARCHITECT);
        return 0;
    }
    if ( (stricmp(text, textStd("NewString")) == 0) || (stricmp(text, textStd("AllCritterListText") ) == 0 ) || (stricmp(text, textStd("NoneString")) == 0 ) )
    {
        return 0;
    }
    return 1;
}
static void createCVGSaveErrors(int saveValue)
{
    char *errorMessages;
    estrCreate(&errorMessages);
    estrConcatCharString(&errorMessages, textStd("CVGSaveFailed"));
    if (saveValue & CVG_LE_NONAME)
    {
        estrConcatf(&errorMessages, ", %s", textStd("CVGNoNameString") );
    }
    if (saveValue & CVG_LE_INVALID_NAME)
    {
        estrConcatf(&errorMessages, ", %s", textStd("CVGInvalidNameString") );
    }
    if (saveValue & CVG_LE_INVALID_STANDARD)
    {
        estrConcatf(&errorMessages, ", %s", textStd("CVGInvalidStandardCritterString") );
    }
    if (saveValue & CVG_LE_INVALID_CRITTER)
    {
        estrConcatf(&errorMessages, ", %s", textStd("CVGInvalidCustomCritterString") );
    }
    if (saveValue & CVG_LE_NO_CRITTERS)
    {
        estrConcatf(&errorMessages, ", %s", textStd("CVGNoEnemiesString") );
    }
    if (saveValue & CVG_LE_LEVELRANGE_HOLES)
    {
        estrConcatf(&errorMessages, ", %s", textStd("CVGLevelRangeHolesString") );
    }
    if (saveValue & CVG_LE_PROFANITY)
    {
        estrConcatf(&errorMessages, ", %s", textStd("CVGProfanityString") );
    }
    if (saveValue & CVG_LE_NON_UNIQUE_EXISTING_VILLAINS)
    {
        estrConcatf(&errorMessages, ", %s", textStd("CVGNonUniqueNameString") );
    }
    dialogStd( DIALOG_OK, errorMessages, NULL, NULL, NULL, NULL, DLGFLAG_ARCHITECT);
    estrDestroy(&errorMessages);
}
static void OverWriteCVGFile(void *data)
{
    int retVal;
    retVal = saveCVG( s_overwriteFileName, s_overwriteDisplayName );    
    if (retVal != CVG_LE_VALID)
    {
        createCVGSaveErrors(retVal);
    }

}

static void drawRightGroupWindow(float x, float y, float z, float width, float height, float sc)
{
    //    draw frame (list view?)
    int i;
    static int visibleFrames[CVGLIST_RANKS] = {WINDOW_DISPLAYING, WINDOW_DISPLAYING, WINDOW_DISPLAYING, WINDOW_DISPLAYING, WINDOW_DISPLAYING};
    static char* frameText[CVGLIST_RANKS] = {"CVGMinionString", "CVGLtString", "CVGBossString", "CVGEliteString", "CVGArchVillainString"};
    static float visibleRelativeHeight[CVGLIST_RANKS] = {1.0/CVGLIST_RANKS, 1.0/CVGLIST_RANKS, 1.0/CVGLIST_RANKS, 1.0/CVGLIST_RANKS, 1.0/CVGLIST_RANKS};
    static float timer = 1;
    static HelpButton *rightGroupHelpButton = NULL;
    CBox rightHelpGroupBox;
    int visibleCount = 0;
    float widgetHeight;
    float nextY = 0;

    STATIC_INFUNC_ASSERT(CVGLIST_RANKS == 5);

    for (i = 0; i < CVGLIST_RANKS; i++)
    {
        if (visibleFrames[i] != WINDOW_SHRINKING)
        {
            visibleCount++;
        }
    }
    widgetHeight = height- ((60 + CVGLIST_RANKS * 30)*sc);
    drawFlatFrame(PIX2, R10, x,y,z,width, height,sc,CLR_FRAME_GREY,CLR_MM_FRAME_BACK );
    drawMMBar("CVGCreationString", x+(width/2), y, z+10, width-(10*sc), sc, 1, 0);
    BuildCBox(&rightHelpGroupBox, x+width-(600*sc), y-(70*sc), 700*sc, 200*sc);
    helpButton(&rightGroupHelpButton, x+ width - (30*sc), y, z+20, sc, "CVGCreationToolTip", &rightHelpGroupBox );
    
    for (i = 0; i < CVGLIST_RANKS; i++)
    {
        if(i == 0)
            nextY = 60*sc;
        else
            nextY += (30*sc) +(widgetHeight *visibleRelativeHeight[i - 1]);

        visibleRelativeHeight[i] = (( (1- timer) * visibleRelativeHeight[i] ) + ( ( (visibleFrames[i] == WINDOW_SHRINKING) || (!visibleCount) ) ? 0 : (timer/visibleCount)));
        drawFrame(PIX3, R10, x+(15*sc), y + nextY, z, width-(30*sc), (widgetHeight*visibleRelativeHeight[i]), sc,CLR_MM_TEXT_BACK, 0x000000FF);
        if (drawMMBar(frameText[i], x+(width/2), y+nextY, z+10, width-(40*sc), sc, 1, 1))
        {
            visibleFrames[i] = ( visibleFrames[i] == WINDOW_SHRINKING ) ? WINDOW_GROWING : WINDOW_SHRINKING;
            timer = 0.0f;
        }
    }

    //    populate list on right side
    if (timer == 1.0f)
    {
        drawRightVillainList(x+(15*sc),y+(60*sc),z,width-(30*sc), height-(60*sc), sc, visibleFrames);
    }

    //    all critters that have been selected into group
    selectVillainGroup(pCVGCreatedElementList, x+(width/2),y+(30*sc),z+10,sc, width-20, 1);

    //    after right Villain list to make sure that right villain list exists
    if ( (pCVGCreatedElementList->current_element >= 0) && (CVGroupIndex != pCVGCreatedElementList->current_element) )
    {
        populateVillainListFromCVG(pCVGCreatedElementList, 1);
        updateLevelRanges();
        CVGroupIndex = pCVGCreatedElementList->current_element;
        vGroup = -1;
        smf_SetRawText(&saveFileName, pCVGCreatedElementList->ppElement[pCVGCreatedElementList->current_element]->pchDisplayName, 0 );
    }


    //    update timers
    timer += 0.1f;
    for (i = 0; i < CVGLIST_RANKS; ++i)
    {
        if ((visibleFrames[i] == WINDOW_GROWING) && (timer >= 1.0f))
        {
            visibleFrames[i] = WINDOW_DISPLAYING;
        }
    }
    timer = CLAMPF32(timer, 0.0f, 1.0f);
}

void populateCVGSS()
{
    int i;
    MMElement *blank;
    char *oldCreatedElementString = NULL;
    char *oldOriginalElementString = NULL;
    //    clear out old ss
    estrCreate(&oldCreatedElementString);
    estrCreate(&oldOriginalElementString);
    if (pCVGCreatedElementList)
    {
        if ( pCVGCreatedElementList && pCVGCreatedElementList->ppElement && 
            EAINRANGE( pCVGCreatedElementList->current_element, pCVGCreatedElementList->ppElement) &&
            pCVGCreatedElementList->ppElement[pCVGCreatedElementList->current_element]->pchText )
        {
            estrConcatCharString(&oldCreatedElementString, pCVGCreatedElementList->ppElement[pCVGCreatedElementList->current_element]->pchText);
        }
        if ( pCVGOriginalElementList && pCVGOriginalElementList->ppElement && 
            EAINRANGE( pCVGOriginalElementList->current_element, pCVGOriginalElementList->ppElement) &&
            pCVGOriginalElementList->ppElement[pCVGOriginalElementList->current_element]->pchText )
        {
            estrConcatCharString(&oldOriginalElementString, pCVGOriginalElementList->ppElement[pCVGOriginalElementList->current_element]->pchText);
        }

        for (i = 0; i < (eaSize(&pCVGCreatedElementList->ppElement)); ++i)
        {
            StructFreeString(pCVGCreatedElementList->ppElement[i]->pchText);
            StructFreeString(pCVGCreatedElementList->ppElement[i]->pchDisplayName);
            StructFree(pCVGCreatedElementList->ppElement[i]);
        }
        eaDestroy(&pCVGCreatedElementList->ppElement);
        for (i = 0; i < (eaSize(&pCVGOriginalElementList->ppElement)); ++i)
        {
            StructFreeString(pCVGOriginalElementList->ppElement[i]->pchText);
            StructFreeString(pCVGOriginalElementList->ppElement[i]->pchDisplayName);
            StructFree(pCVGOriginalElementList->ppElement[i]);
        }
        eaDestroy(&pCVGOriginalElementList->ppElement);
    }
    else
    {
        pCVGCreatedElementList = StructAllocRaw(sizeof(MMElementList));
        pCVGOriginalElementList = StructAllocRaw(sizeof(MMElementList));
        pCVGOriginalElementList->bDontUnlockCheck = pCVGCreatedElementList->bDontUnlockCheck = 1;
    }

    if (eaSize(&g_CustomVillainGroups) == 0 )
    {
        MMElement *noneElement;
        noneElement = StructAllocRaw(sizeof(MMElement));
        noneElement->pchDisplayName = StructAllocString(textStd("noneString"));
        noneElement->pchText = StructAllocString(textStd("noneString"));
        eaPush(&pCVGOriginalElementList->ppElement,noneElement);
    }
    blank = StructAllocRaw(sizeof(MMElement));
    blank->pchDisplayName = StructAllocString(textStd("NewString"));
    blank->pchText = StructAllocString(textStd("NewString"));
    eaPush(&pCVGCreatedElementList->ppElement, blank);
    //    populate ss
    for ( i = 0; i < eaSize(&g_CustomVillainGroups); ++i)
    {
        MMElement *pElement;
        MMElement *pElement2;
        pElement = StructAllocRaw(sizeof(MMElement));
        pElement->pchDisplayName = StructAllocString(g_CustomVillainGroups[i]->displayName);
        pElement->pchText = StructAllocString(g_CustomVillainGroups[i]->displayName);
        eaPush(&pCVGCreatedElementList->ppElement, pElement);
        pElement2 = StructAllocRaw(sizeof(MMElement));
        pElement2->pchDisplayName = StructAllocString(g_CustomVillainGroups[i]->displayName);
        pElement2->pchText = StructAllocString(g_CustomVillainGroups[i]->displayName);
        eaPush(&pCVGOriginalElementList->ppElement, pElement2);
    }
    eaQSort( pCVGOriginalElementList->ppElement, sortCVGNames);
    eaQSort( pCVGCreatedElementList->ppElement, sortCVGNames);
    for ( i = 0; i < eaSize(&pCVGCreatedElementList->ppElement); ++i)
    {
        if (oldCreatedElementString && stricmp(pCVGCreatedElementList->ppElement[i]->pchText, oldCreatedElementString) == 0)
        {
            pCVGCreatedElementList->current_element = i;
            break;
        }
    }
    if ( !EAINRANGE(pCVGCreatedElementList->current_element, pCVGCreatedElementList->ppElement) )
    {
        pCVGCreatedElementList->current_element = 0;
    }
    for ( i = 0; i < eaSize(&pCVGOriginalElementList->ppElement); ++i)
    {
        if (oldOriginalElementString && stricmp(pCVGOriginalElementList->ppElement[i]->pchText, oldOriginalElementString) == 0)
        {
            pCVGOriginalElementList->current_element = i;
            break;
        }
    }
    if ( !EAINRANGE(pCVGOriginalElementList->current_element, pCVGOriginalElementList->ppElement) )
    {
        pCVGOriginalElementList->current_element = 0;
    }

    estrDestroy(&oldCreatedElementString);
    estrDestroy(&oldOriginalElementString);
    CVGroupIndex = -1;
    vGroup = -1;
    s_currentlySelectedPtr = NULL;
    dialogClearQueue(0);
}

static char * getVGFilterText(int vg_filter_index)
{
    switch (vg_filter_index)
    {
    case VG_FILTER_ALLOW_BOSS:
        return "CVGBossString";
    case VG_FILTER_ALLOW_MINIONS:
        return "CVGMinionString";
    case VG_FILTER_ALLOW_LT:
        return "CVGLtString";
    case VG_FILTER_ALLOW_VALID_CRITTERS_ONLY:
        return "CVGValidCrittersOnlyString";
    default:
        return "noneString";
    }
}

static void drawSelectionInfo(float x, float y, float z, float width, float height, float sc)
{
    int i;
    static HelpButton *previewHelpButton = NULL;
    static PictureBrowser *pb = NULL;
    static SMFBlock sb;
    static SMFBlock villainInfo;
    CBox previewHelpBox;
    char *costumeStr;
    char *textStr;
    char *powerStr;
    UIBox clipBox;
    int selectionValid = 0;
    int textHeight = 0;
    float previewImageWidth = 0;
    drawFlatFrame(PIX2, R10, x,y,z,width, height,sc,CLR_FRAME_GREY,CLR_MM_FRAME_BACK );
    BuildCBox(&previewHelpBox, x+width-(600*sc), y-(20*sc), 700*sc, 200*sc);
    helpButton(&previewHelpButton, x+ width - (30*sc), y, z+20, sc, "CVGPreviewToolTip", &previewHelpBox );
    if (drawMMBar("CVGPreviewString", x+(width/2), y, z+10, width-(10*sc), sc, 1, 1))
    {
        if (selectionInfoMode == WINDOW_SHRINKING)
        {
            selectionInfoMode = WINDOW_GROWING;
        }
        else
        {
            selectionInfoMode = WINDOW_SHRINKING;
        }
    }
    if ((selectionInfoMode != WINDOW_DISPLAYING) || (!s_currentlySelectedPtr) )
    {
        return;
    }
    uiBoxDefine(&clipBox, x,y, width, height);
    clipperPush(&clipBox);
    if(!pb)
    {
        pb = picturebrowser_create();
        picturebrowser_init( pb,0, 0, 0, 256, 256, NULL, 0 );
        picturebrowser_setAnimationAvailable(pb, 0);
    }
    estrCreate(&powerStr);
    if ( (!s_currentlySelectedPtr->pCritter) && (s_currentlySelectedPtr->villainGroup >= 0) && (s_currentlySelectedPtr->villainIndex < 0) && (s_currentlySelectedPtr->villainIndex > -4) )
    {
        AtlasTex *randomEnemy_icon_mouseOver = atlasLoadTexture("Random_Enemy_Icon");
        AtlasTex *randomEnemy_icon = atlasLoadTexture("Random_Enemy_Icon_default");
        CBox iconBox;
        char *rank;
        selectionValid = 1;
        switch (s_currentlySelectedPtr->villainIndex)
        {
        case -1:
            rank = "CVGMinionString";
            break;
        case -2:
            rank = "CVGLtString";
            break;
        case -3:
            rank = "CVGBossString";
            break;
        default:
            rank = "CVGBossString";
            break;
        }

        BuildCBox(&iconBox, x+(6*sc), y+(17*sc), randomEnemy_icon->width*sc, randomEnemy_icon->height*sc);
        if (mouseCollision(&iconBox))
        {
            display_sprite(randomEnemy_icon_mouseOver, x+(6*sc), y+(17*sc), z, sc, sc, CLR_WHITE );    
        }
        else
        {
            display_sprite(randomEnemy_icon, x+(6*sc), y+(17*sc), z, sc, sc, CLR_WHITE );    
        }
        previewImageWidth = randomEnemy_icon->width *sc;
        estrCreate(&textStr);
        estrConcatf(&textStr, "<scale %f>", sc);
        estrConcatf(&textStr, "<font outline=1 color=ArchitectDark>%s:</color><font outline=0> %s <br>", 
            textStd("CVGRankType", textStd(rank)),
            textStd("CVGStandardString"));
        estrConcatf(&textStr, "<font outline=1 color=ArchitectDark>%s:</color><font outline=0> %s <br>", 
            textStd("CVGRankName", textStd(rank)),
            textStd("Random"));
        estrConcatf(&textStr, "<font outline=1 color=ArchitectDark>%s:</color><font outline=0> %s <br>",
            textStd("MMPowerDesc"),
            textStd("Random"));
    }
    else if ( (!s_currentlySelectedPtr->pCritter) && (s_currentlySelectedPtr->villainGroup >= 0) && (s_currentlySelectedPtr->villainIndex >= 0) )
    {
        for (i = 0; i < eaSize(&villainDefList.villainDefs[s_currentlySelectedPtr->villainGroup]->levels[s_currentlySelectedPtr->villainIndex]->costumes); i++)
        {
            if (villainDefList.villainDefs[s_currentlySelectedPtr->villainGroup]->levels[s_currentlySelectedPtr->villainIndex]->costumes[i])
            {
                break;
            }
        }
        if (i < eaSize(&villainDefList.villainDefs[s_currentlySelectedPtr->villainGroup]->levels[s_currentlySelectedPtr->villainIndex]->costumes) )
        {
            int j,k;
            static Costume *prevCostume[2] = { NULL, NULL };
            static int rotater = 0;
            static int npcIsTintable = 0;
            const NPCDef *npc;
            int refresh;
            selectionValid = 1;
            npc = npcFindByName(villainDefList.villainDefs[s_currentlySelectedPtr->villainGroup]->levels[s_currentlySelectedPtr->villainIndex]->costumes[i], NULL);
            refresh = s_currentlySelectedPtr->refreshPB;
            s_currentlySelectedPtr->refreshPB = 0;
            if (refresh)
            {
                npcIsTintable = npc && NPCCostume_canEditCostume(npc->costumes[0]);
            }
            if (s_currentlySelectedPtr->npcCostume)
            {
                static CustomNPCCostume *prevNPCCostume = NULL;
                if (refresh || (prevNPCCostume != s_currentlySelectedPtr->npcCostume))
                {
                    rotater = (rotater + 1) % 2;
                    if (prevCostume[rotater])
                    {
                        costume_destroy(prevCostume[rotater]);
                    }
                    prevCostume[rotater] = costume_clone(npc->costumes[0]);
                    costumeApplyNPCColorsToCostume(prevCostume[rotater], s_currentlySelectedPtr->npcCostume);
                }
                prevNPCCostume = s_currentlySelectedPtr->npcCostume;
                picturebrowser_setMMImageCostume(pb, costume_as_const(prevCostume[rotater]));
            }
            else
            {
                picturebrowser_setMMImageCostume(pb, npc->costumes[0] );
            }
            pb->sc = sc;
            estrCreate(&costumeStr);
            estrConcatf(&costumeStr, "<img src=pictureBrowser:%d align=center>", pb->idx);
            smf_ParseAndDisplay(&sb, costumeStr, x+(2*sc), y+(13*sc), z, 256*sc, height,0,0, 0, 0,0,0);
            estrDestroy(&costumeStr);
            previewImageWidth = pb->wd;
            estrCreate(&textStr);
            estrConcatf(&textStr, "<scale %f>", sc);
            estrConcatf(&textStr, "<font outline=1 color=ArchitectDark>%s:</color><font outline=0> %s <br>",
                textStd("CVGRankType", textStd(getVGTextFromRank(villainDefList.villainDefs[s_currentlySelectedPtr->villainGroup]->rank))),
                textStd("CVGStandardString"));
            estrConcatf(&textStr, "<font outline=1 color=ArchitectDark>%s:</color><font outline=0> %s <br>",
                textStd("CVGRankName", textStd(getVGTextFromRank(villainDefList.villainDefs[s_currentlySelectedPtr->villainGroup]->rank))),
                textStd(villainDefList.villainDefs[s_currentlySelectedPtr->villainGroup]->levels[s_currentlySelectedPtr->villainIndex]->displayNames[0]));


            for( k = 0; k < eaSize(&villainDefList.villainDefs[s_currentlySelectedPtr->villainGroup]->powers); k++ )
            {
                const PowerNameRef * pRef = villainDefList.villainDefs[s_currentlySelectedPtr->villainGroup]->powers[k];

                if( pRef->power && pRef->power[0] == '*')
                {
                    const BasePowerSet * pSetBase = powerdict_GetBasePowerSetByName(&g_PowerDictionary, pRef->powerCategory, pRef->powerSet);

                    if( pSetBase )
                    {
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
            estrConcatf(&textStr, "<font outline=1 color=ArchitectDark>%s:</color><font outline=0> %s<br>", textStd("MMPowerDesc"), powerStr);
            estrConcatf(&textStr, "<font outline=1 color=ArchitectDark>%s:</color><font outline=0> %s<br>", textStd("NPCColorTintable"), npcIsTintable ? textStd("YesString") : textStd("NoString"));
            if (isDevelopmentMode() )
            {
                estrConcatf(&textStr, "Debug Info: group %i level %i <br>", s_currentlySelectedPtr->villainGroup, s_currentlySelectedPtr->villainIndex );
                estrConcatf(&textStr, "Debug Info: groupenum %i <br>", villainDefList.villainDefs[s_currentlySelectedPtr->villainGroup]->group);
                estrConcatf(&textStr, "Debug Info: %s<br>", villainDefList.villainDefs[s_currentlySelectedPtr->villainGroup]->levels[s_currentlySelectedPtr->villainIndex]->costumes[i]);
                estrConcatf(&textStr, "Debug Info: minLevel %i maxLevel %i", villainDefList.villainDefs[s_currentlySelectedPtr->villainGroup]->levels[0]->level, villainDefList.villainDefs[s_currentlySelectedPtr->villainGroup]->levels[eaSize(&villainDefList.villainDefs[s_currentlySelectedPtr->villainGroup]->levels)-1]->level);
            }
        }
    }
    else if ( s_currentlySelectedPtr->pCritter && (s_currentlySelectedPtr->villainGroup >= 0) && (s_currentlySelectedPtr->villainIndex >= 0) )
    {
        PCC_Critter* pCritter;
        pCritter = g_CustomVillainGroups[s_currentlySelectedPtr->villainGroup]->customVillainList[s_currentlySelectedPtr->villainIndex];
        if (pCritter)
        {
            static int reasonForInvalid;
            if ( s_currentlySelectedPtr->refreshPB )
            {
                reasonForInvalid = isPCCCritterValid(playerPtr(), pCritter,0,0);
                s_currentlySelectedPtr->refreshPB = 0;
            }
            selectionValid = 1;
            picturebrowser_setMMImageCostume(pb, costume_as_const(pCritter->pccCostume));
            pb->sc = sc;
            estrCreate(&costumeStr);
            estrConcatf(&costumeStr, "<span align=left><img src=pictureBrowser:%d></span>", pb->idx);
            smf_ParseAndDisplay(&sb, costumeStr, x+(2*sc), y+(13*sc), z, 256*sc, height,0,0, 0, 0,0,0);
            estrDestroy(&costumeStr);
            previewImageWidth = pb->wd;
            estrCreate(&textStr);            
            estrConcatf(&textStr, "<scale %f>", sc);
            PCC_generatePCCHTMLSelectionText(&textStr, pCritter, reasonForInvalid, 0, "ArchitectDark" );
        }
    }
    if (selectionValid)
    {
        estrConcatStaticCharArray(&textStr, "</scale>");
        textHeight = smf_ParseAndDisplay(&villainInfo, textStr, x+(previewImageWidth + (10*sc)), y+(15*sc), z, (width - (pb->wd + (10*sc))), height,0,0, &textAttr_CVGList, 0,0,0);
        estrDestroy(&textStr);
    }
    estrDestroy(&powerStr);
    clipperPop();
}

static int drawSortOption(float startX, float y, float z, float sc, int i, CustomVillainGroupList *list)
{
    CBox cbox;
    int textWidth = 0;
    int x;
    int clr1, clr2;
    textWidth = str_wd(&game_12, sc, sc, textStd(sortOptions[i]));
    x = startX-(textWidth+10*sc);
    BuildCBox(&cbox, x,y, textWidth+(12*sc), 20*sc );
    clr1 = CLR_MM_TEXT_DARK& 0xFFFFFF77;
    clr2 = CLR_MM_TEXT & 0xFFFFFF33;
    if (mouseCollision(&cbox))
    {
        font_color(CLR_MM_TEXT_DARK,CLR_MM_TEXT_DARK);
        if (mouseClickHit(&cbox, MS_LEFT))
        {
            if (currentSortingOption != i)
            {
                currentSortingOption = i;
            }
            else
            {
                sortOptionStatus ^= 1 << i;        //    toggles the sort option
            }
            eaQSort(list->items, sortCVGData);
        }
        clr1 = CLR_MM_TEXT_DARK& 0xFFFFFF77;
        clr2 = CLR_MM_TEXT & 0xFFFFFF77;
    }
    else
    {
        font_color(CLR_MM_TEXT,CLR_MM_TEXT);
    }
    if (currentSortingOption != i)
    {
        drawFlatFrameBox(PIX2, R4, &cbox, z,clr1, clr2);
    }
    if (currentSortingOption == i)
    {
        AtlasTex *arrow;
        if (sortOptionStatus & (1 << currentSortingOption))
        {
            arrow = atlasLoadTexture( "chat_separator_arrow_down.tga" );
        }
        else
        {
            arrow = atlasLoadTexture( "chat_separator_arrow_up.tga" );
        }
        display_sprite(arrow, x-(arrow->width/2), y+5*sc, z, sc, sc, CLR_WHITE);
        drawFlatFrame(PIX2, R4, x-((arrow->width/2)+(2*sc)),cbox.ly, z-1, cbox.hx+(4*sc)+(arrow->width/2)-cbox.lx, cbox.hy-cbox.ly, sc,CLR_MM_TEXT_DARK& 0xFFFFFF55, CLR_MM_TEXT & 0xFFFFFF55);
    }
    font(&game_12);
    prnt(x+(8*sc), y+(16*sc), z, sc, sc, textStd(sortOptions[i]));
    return textWidth+(25*sc);
}
static void drawSortOptions(float x, float y, float z, float sc, CustomVillainGroupList *list)
{
    int i;
    int nextPosition = 0;
    for (i = ARRAY_SIZE(sortOptions)-1; i >= 0; --i)
    {
        nextPosition += drawSortOption(x - nextPosition, y-(2*sc), z, sc, i, list);
    }
    nextPosition += str_wd(&game_12, sc, sc, textStd("MMSortBy"));
    font_color(CLR_MM_TEXT,CLR_MM_TEXT);
    prnt(x-nextPosition, y+(15*sc),z, sc, sc, textStd("MMSortBy"));
}
static void drawFilterButtons(float x, float y, float z, float width, float height, float sc)
{
    if (!showStandardCritters)
    {
        int onlyShowValid;
        onlyShowValid = vg_Filter & VG_FILTER_ALLOW_VALID_CRITTERS_ONLY;
        if (drawMMCheckBox(x+(60*sc), y,z, sc, textStd(getVGFilterText(VG_FILTER_ALLOW_VALID_CRITTERS_ONLY)), 
            &onlyShowValid, CLR_MM_TEXT_DARK, CLR_MM_TEXT_DARK,    CLR_MM_TEXT,CLR_MM_TEXT, 0,0 ))
        {
            if (!onlyShowValid)
            {
                vg_Filter &= (~VG_FILTER_ALLOW_VALID_CRITTERS_ONLY);
                vGroup = -1;
            }
            else
            {
                vg_Filter |= VG_FILTER_ALLOW_VALID_CRITTERS_ONLY; 
                vGroup = -1;
            }
        }
    }
}
static void cvg_selectTab(void *data)
{
    vg_Filter = (int)data;
    vGroup = -1;
}

static void dummyDialogFunction(void *data)
{
    CustomNPCEdit_setup(0, villainDefList.villainDefs[s_currentlySelectedPtr->villainGroup], 0,
        &s_currentlySelectedPtr->npcCostume, 0,
        &s_currentlySelectedPtr->cvg_displayName, &s_currentlySelectedPtr->cvg_description, &pCVGCreatedElementList->ppElement[pCVGCreatedElementList->current_element]->pchText, 1);
    init = 2;
    s_currentlySelectedPtr = NULL;
}
static void drawLeftGroupWindow(float x, float y, float z, float width, float height, float sc)
{    
    int i,j;    
    static uiTabControl *tc = NULL;
    static uiTabControl *tcCustom = NULL;
    static HelpButton *leftGroupHelpButton = NULL;
    CBox leftGroupHelpBox;
    if (!tc)
    {
        tc = uiTabControlCreate(TabType_Undraggable, cvg_selectTab, 0,0,0,0);
        uiTabControlAdd(tc, "AllString", (int*) (VG_FILTER_COUNT-1) );
        uiTabControlAdd(tc, "CVGMinionString", (int*) VG_FILTER_ALLOW_MINIONS );
        uiTabControlAdd(tc, "CVGLtString", (int*) VG_FILTER_ALLOW_LT );
        uiTabControlAdd(tc, "CVGBossString", (int*) VG_FILTER_ALLOW_BOSS );
        uiTabControlAdd(tc, "CVGEliteString", (int*) VG_FILTER_ALLOW_ELITE );
        uiTabControlAdd(tc, "CVGArchVillainString", (int*) VG_FILTER_ALLOW_ARCHVILLAIN );
        uiTabControlSelect(tc, (int *) (VG_FILTER_COUNT-1) );
    }
    if (!tcCustom)
    {
        tcCustom = uiTabControlCreate(TabType_Undraggable, cvg_selectTab, 0,0,0,0);
        uiTabControlAdd(tcCustom, "AllString", (int*) (VG_FILTER_COUNT-1) );
        uiTabControlAdd(tcCustom, "CVGMinionString", (int*) VG_FILTER_ALLOW_MINIONS );
        uiTabControlAdd(tcCustom, "CVGLtString", (int*) VG_FILTER_ALLOW_LT );
        uiTabControlAdd(tcCustom, "CVGBossString", (int*) VG_FILTER_ALLOW_BOSS );
        uiTabControlAdd(tcCustom, "CVGEliteString", (int*) VG_FILTER_ALLOW_ELITE );
        uiTabControlAdd(tcCustom, "CVGArchVillainString", (int*) VG_FILTER_ALLOW_ARCHVILLAIN );
        uiTabControlSelect(tcCustom, (int *) (VG_FILTER_COUNT-1) );
    }
    //    get list of all critters groups from missionMaker
    if (!pVillainElementList)
    {
        pVillainElementList = StructAllocRaw(sizeof(MMElementList));
        pVillainElementList->bDontUnlockCheck = 0;
        //    populate list on left side
        for( i = 0; i < eaSize(&gMMVillainGroupList.group); i++ )
        {
            addVillainGroup( gMMVillainGroupList.group[i], pVillainElementList, 0 );
            for(j = 0; j < eaSize(&gMMVillainGroupList.group[i]->ppGroup); j++ )
                addVillainGroup( gMMVillainGroupList.group[i]->ppGroup[j], pVillainElementList, j+1 );
        }
        eaQSort( pVillainElementList->ppElement, sortCVGNames );
    }

    //    drawFrame (for list selection)
    drawFlatFrame(PIX2, R10, x,y,z,width, height,sc,CLR_FRAME_GREY,CLR_MM_FRAME_BACK );
    drawMMBar( "CVGAvailableEnemiesString",  x + width/2, y, z+5, width-(10*sc), sc, 1, 0 );
    BuildCBox(&leftGroupHelpBox, x+width-(600*sc), y-(20*sc), 700*sc, 200*sc);
    helpButton(&leftGroupHelpButton, x+ width - (30*sc), y, z+20, sc, "CVGAvailableEnemyToolTip", &leftGroupHelpBox );

    {
        char * cvg_tabs[] = { "CVGStandardString" , "CustomString" };
        char **names = 0;
        int oldShow;
        int itr;
        for (itr = 0; itr < ARRAY_SIZE(cvg_tabs); ++itr)
        {
            eaPush(&names, cvg_tabs[itr]);
        }
        oldShow = showStandardCritters;
        showStandardCritters = !drawMMFrameTabs(x+5, y+35, z+10, sc, width-10, height-45, 20*sc, kStyle_Architect, &names, !showStandardCritters);
        if (oldShow != showStandardCritters)
        {
            vGroup = -1;
        }
        eaDestroy(&names);

    }
    //    create a list of all villain groups
    selectVillainGroup(showStandardCritters? pVillainElementList : pCVGOriginalElementList, x+(width/2),y+(65*sc),z+10, sc, width-20, 0);

    //    draw frame (list view?)
    drawFrame(PIX3, R10, x+20, y + (110*sc), z, width-40, height-(205*sc), sc, CLR_MM_TEXT_BACK, 0x000000FF);
    drawTabControl(showStandardCritters? tc : tcCustom, x+30, y + (110*sc),z, width-40, height, sc, CLR_FRAME_GREY, CLR_MM_TEXT_BACK, TabDirection_Horizontal);

    if (showStandardCritters)
    {
        //    for each villain group
        //    create a list of all critters in group
        if ( pVillainElementList->ppElement[pVillainElementList->current_element])
        {
            drawLeftStandardVillainList(    pVillainElementList->ppElement[pVillainElementList->current_element]->npcIndex, 
                pVillainElementList->ppElement[pVillainElementList->current_element]->minLevel,
                pVillainElementList->ppElement[pVillainElementList->current_element]->maxLevel,
                pVillainElementList->ppElement[pVillainElementList->current_element]->setnum,
                x+20, 
                y+(130*sc), z, width-40, height-(226*sc), sc, vGroup != pVillainElementList->current_element);
        }
        vGroup = pVillainElementList->current_element;
    }
    else
    {
        if ( (vGroup != pCVGOriginalElementList->current_element) && (pCVGOriginalElementList->current_element >= 0) )
        {
            populateVillainListFromCVG(pCVGOriginalElementList, 0);
            vGroup = pCVGOriginalElementList->current_element;
            eaQSort(availableVillainList.items, sortCVGData);
        }
        drawVillainGroupList(&availableVillainList,  x+20, y+(130*sc), z, width-40, height-(226*sc), sc, 0);    
    }
    if ( drawMMButton("MMCreateCustomCritter", "Character_Icon", 0, x+(width/2), y+height-(73*sc), z, 300*sc, sc, MMBUTTON_ICONALIGNTOTEXT | MMBUTTON_USEOVERRIDE_COLOR, 0 ))
    {
        int flags = 0;
        if ((vg_Filter & (VG_FILTER_COUNT-1)) == (VG_FILTER_COUNT-1))
            flags = 0;
        else if (vg_Filter & VG_FILTER_ALLOW_MINIONS)
            flags |= PCC_SETUP_FLAGS_SEED_MINION;
        else if (vg_Filter & VG_FILTER_ALLOW_LT)
            flags |= PCC_SETUP_FLAGS_SEED_LT;
        editPCC_setup(pCVGCreatedElementList, PCC_EDITING_MODE_CREATION, NULL, flags, NULL);
        init = 0;
    }

    if ( drawMMButton((s_currentlySelectedPtr && !s_currentlySelectedPtr->pCritter) ? "MMEditNPC" : "MMEditCustomCritter", "Character_Icon", 0, x+(width/2), y+height-(33*sc), z, 300*sc, sc, 
        MMBUTTON_ICONALIGNTOTEXT | MMBUTTON_USEOVERRIDE_COLOR | ( (s_currentlySelectedPtr &&
        ( s_currentlySelectedPtr->pCritter || (!s_currentlySelectedPtr->pCritter && !selectionOnLeft) )) ? 0 : MMBUTTON_GRAYEDOUT ), 0 ))
    {
        if (s_currentlySelectedPtr)
        {
            if (s_currentlySelectedPtr->pCritter)
            {
                init = 0;
                if ( (s_currentlySelectedPtr->villainGroup >= 0) && (s_currentlySelectedPtr->villainIndex >= 0) )
                {
                    static PCC_Critter *tempCritter = NULL;
                    if (tempCritter)
                    {
                        destroy_PCC_Critter(tempCritter);
                    }
                    tempCritter = clone_PCC_Critter(g_CustomVillainGroups[s_currentlySelectedPtr->villainGroup]->customVillainList[s_currentlySelectedPtr->villainIndex]);
                    editPCC_setup(selectionOnLeft ? pCVGOriginalElementList : pCVGCreatedElementList, PCC_EDITING_MODE_EDIT, tempCritter, 0, NULL);
                }
                else
                {
                    editPCC_setup(pCVGCreatedElementList, PCC_EDITING_MODE_CREATION, NULL, 0, NULL);
                }
            }
            else if (!selectionOnLeft)
            {
                int found = 0;
                if ( (s_currentlySelectedPtr->villainGroup >=0) && (s_currentlySelectedPtr->villainIndex >= 0) )
                {
                    const NPCDef *npc = NULL;
                    const char *npc_name = NULL;
                    found = 1;
                    npc_name = villainDefList.villainDefs[s_currentlySelectedPtr->villainGroup]->levels[s_currentlySelectedPtr->villainIndex]->costumes[0];
                    npc = npcFindByName(npc_name, NULL);
                    //    NPCCostume_canEditCostume must be called to setup some state information
                    if (NPCCostume_canEditCostume(npc->costumes[0]))
                    {
                        init = 2;        //    set back to 0 later
                        CustomNPCEdit_setup(0, villainDefList.villainDefs[s_currentlySelectedPtr->villainGroup], 0,
                            &s_currentlySelectedPtr->npcCostume, 1,
                            &s_currentlySelectedPtr->cvg_displayName, &s_currentlySelectedPtr->cvg_description, &pCVGCreatedElementList->ppElement[pCVGCreatedElementList->current_element]->pchText, 1);
                        s_currentlySelectedPtr = NULL;
                    }
                    else
                    {
                        dialogStd(DIALOG_YES_NO, "NPCNoColorsAvailable", NULL, NULL, dummyDialogFunction, 0, DLGFLAG_ARCHITECT);
                    }
                }
            }
            else
            {
                dialogStd(DIALOG_OK, "NPCEditSelectedOnLeft", NULL, NULL, 0, 0, DLGFLAG_ARCHITECT);
            }
        }
        else
        {
            dialogStd(DIALOG_OK, "NPCEditNothingSelected", NULL, NULL, 0, 0, DLGFLAG_ARCHITECT);
        }
    }

}
static int CVG_saveFile(char *text, char *displayName)
{
    char fname[MAX_PATH];
    if (text && displayName)
    {
        if (    (!isInvalidFilename(text)) && validate_CVGName(text) && validate_CVGName(displayName) )
        {
            FILE * open_test;
            makeDirectories(getCustomVillainGroupDir());
            sprintf(fname, "%s/%s.cvg", getCustomVillainGroupDir(), text);
            open_test = fopen( fname, "rb" );
            if (open_test && 
                (stricmp(pCVGCreatedElementList->ppElement[pCVGCreatedElementList->current_element]->pchDisplayName, displayName) != 0) )
            {
                strcpy(s_overwriteFileName, fname);
                strcpy(s_overwriteDisplayName, displayName);
                dialogStd(DIALOG_YES_NO, "FileExists", NULL, NULL, OverWriteCVGFile, 0, DLGFLAG_ARCHITECT);
                fclose(open_test);
            }
            else
            {
                int retVal;
                retVal = saveCVG(fname, displayName);
                if ( retVal != CVG_LE_VALID)
                {
                    createCVGSaveErrors(retVal);
                }
                else
                {
                    return 1;
                }
            }
        }
        else 
        {
            dialogStd( DIALOG_OK, textStd("CVGInvalidNameString"), NULL, NULL, NULL, NULL, DLGFLAG_ARCHITECT);
        }
    }
    return 0;
}
static char *getCVGUIControlText(int control_enum)
{
    switch (control_enum)
    {
    case VG_UI_SAVE:
        return "MMSave";
        break;
    case VG_UI_SAVE_AS:
        return "MMSaveAs";
        break;
    case VG_UI_SAVE_EXIT:
        return "MMSaveAndExit";
        break;
    case VG_UI_ABANDON:
        return "MMExitAndAbandon";
        break;
    }
    return NULL;
}
static void saveDialog(void*data)
{
    int control_enum = (int)data;
    if (CVG_saveFile(dialogGetTextEntry(), dialogGetTextEntry()) && (control_enum == VG_UI_SAVE_EXIT) )
    {
        window_setMode( WDW_CUSTOMVILLAINGROUP, WINDOW_SHRINKING );        
    }
}
static void handleCVGUIControl(int control_enum)
{
    char *sourceFile;
    char *cvg_name;
    int cvg_idx;    
    cvg_name = pCVGCreatedElementList->ppElement[pCVGCreatedElementList->current_element]->pchText;
    cvg_idx = CVG_CVGExists(cvg_name);
    switch (control_enum)
    {
    case VG_UI_SAVE_EXIT:
    case VG_UI_SAVE:
        {
            if ( (cvg_idx != -1) && g_CustomVillainGroups[cvg_idx]->sourceFile )
            {
                int retVal;
                sourceFile = g_CustomVillainGroups[cvg_idx]->sourceFile;
                if (validate_CVGName(cvg_name))
                {
                    retVal = saveCVG(sourceFile, cvg_name);                //    if source_file exists for cvg, save. other wise, pop up a dialog for name
                    if (retVal != CVG_LE_VALID)
                    {
                        createCVGSaveErrors(retVal);
                    }
                    if (control_enum == VG_UI_SAVE_EXIT)
                    {
                        window_setMode( WDW_CUSTOMVILLAINGROUP, WINDOW_SHRINKING );
                    }
                }
                break;
            }
        }
    case VG_UI_SAVE_AS:                                                //    intentional fall through
        {
            dialogSetTextEntry(pCVGCreatedElementList->ppElement[pCVGCreatedElementList->current_element]->pchDisplayName);
            dialogStdCB( DIALOG_NAME_TEXT_ENTRY, "CVGSaveAsNameEntry", 0, 0, saveDialog, NULL, DLGFLAG_ARCHITECT, (void*)control_enum);
        }
        break;
    case VG_UI_ABANDON:
        window_setMode(WDW_CUSTOMVILLAINGROUP, WINDOW_SHRINKING );        //    exit.
        break;
    }
}
static void drawControl(float x, float y, float z, float sc, AtlasTex *icon, AtlasTex *rollover, int control_enum)
{
    CBox cbox;
    static SMFBlock control_text = { 0 };
    BuildCBox(&cbox, x, y, icon->width, icon->height);
    if (mouseCollision(&cbox))
    {
        char *estr;
        display_sprite(rollover, x+(icon->width/2)-(rollover->width/2), y, z, sc, sc, CLR_MM_TITLE_LIT);
        estrCreate(&estr);
        estrConcatf(&estr, "<scale %f><span align=center><font outline=1 color=ArchitectTitle>%s</color></span></scale>", sc, textStd(getCVGUIControlText(control_enum)));
        smf_SetRawText(&control_text, estr, 0);
        smf_Display(&control_text, x-80, y + (icon->height*sc)-(10*sc), z, (icon->width*sc)+160, 0, 0, 0, &textAttr_CVGList, 0);
        estrDestroy(&estr);
        if (mouseClickHit(&cbox, MS_LEFT))
        {
            handleCVGUIControl(control_enum);
        }
    }
    else
    {
        display_sprite(icon, x, y, z, sc, sc, CLR_MM_TITLE_OPEN);
    }
}
static void drawSaveControls(float x, float y, float z, float width, float height, float sc)
{
    AtlasTex * saveCheck = atlasLoadTexture( "SaveCheck" );
    AtlasTex * saveAsCheck = atlasLoadTexture( "SaveAsCheck" );
    AtlasTex * saveExit = atlasLoadTexture( "SaveExit" );
    AtlasTex * saveAbandon = atlasLoadTexture( "SaveAbandon" );
    AtlasTex * saveCheck_glow = atlasLoadTexture( "SaveCheck__rollover_glow" );
    AtlasTex * saveAsCheck_glow = atlasLoadTexture( "SaveAsCheck_rollover_glow" );
    AtlasTex * saveExit_glow = atlasLoadTexture( "SaveExit_glow" );
    AtlasTex * saveAbandon_glow = atlasLoadTexture( "SaveAbandon_glow" );

    float startx = x+(20*sc);

    drawControl(startx, y, z, sc, saveCheck, saveCheck_glow, VG_UI_SAVE);
    startx += saveCheck->width + (15*sc);
    drawControl(startx, y, z, sc, saveAsCheck, saveAsCheck_glow, VG_UI_SAVE_AS);
    startx += saveAsCheck->width + (15*sc);
    drawControl(startx, y, z, sc, saveExit, saveExit_glow, VG_UI_SAVE_EXIT);
    startx += saveExit->width + (15*sc);
    drawControl(startx, y, z, sc, saveAbandon, saveAbandon_glow, VG_UI_ABANDON);
}
static int CVGCM_isAutoSpawning(void *data)
{
    if (s_editingVillainPtr && ( s_editingVillainPtr->villainIndex >= 0 ) &&
        (rightClickLocation >= 0) )
    {
        return s_editingVillainPtr->dontAutoSpawn ? CM_CHECKED : CM_AVAILABLE;
    }
    return CM_VISIBLE;
}

static void CVGCM_setAutoSpawning(void *data)
{
    if (s_editingVillainPtr && ( s_editingVillainPtr->villainIndex >= 0 ) )
    {
        s_editingVillainPtr->dontAutoSpawn = !s_editingVillainPtr->dontAutoSpawn;
        updateLevelRanges();
    }
    return;
}
static int CVGCM_canMoveAll(void *data)
{
    if (rightClickLocation == -1)
    {
        return (eaSize(&availableVillainList.items)) ? CM_AVAILABLE : CM_VISIBLE;
    }
    else if (rightClickLocation >= 0)
    {
        return (eaSize(&rightCreatedVillainList[rightClickLocation].items)) ? CM_AVAILABLE : CM_VISIBLE;
    }
    return CM_VISIBLE;
}
static void CVGCM_moveAll(void *data)
{
    int i;
    if (rightClickLocation == -1)
    {
        for (i = 0; i < eaSize(&availableVillainList.items); ++i)
        {
            transferVillainGroup(availableVillainList.items[i]);
        }
        updateLevelRanges();
    }
    else if (rightClickLocation >= 0 )
    {
        int j;
        int oneRemoved = 0;
        for (j = eaSize(&rightCreatedVillainList[rightClickLocation].items)-1; j >= 0; --j)
        {
            int success = 0;
            int currentlySelected;
            currentlySelected = (s_currentlySelectedPtr == rightCreatedVillainList[rightClickLocation].items[j]);

            success = removeSelectedCritter(&rightCreatedVillainList[rightClickLocation], rightCreatedVillainList[rightClickLocation].items[j]);

            if ( success )
            {
                oneRemoved = 1;
                if (currentlySelected)
                {
                    s_currentlySelectedPtr = NULL;
                    dialogClearQueue(0);
                }
            }
        }
        if (oneRemoved)
        {
            updateLevelRanges();
            vGroup = -1;
        }
    }
}
static void initCVGContextMenus()
{
    static int inited = 0;
    if (!inited)
    {
        CVGContextMenu = contextMenu_Create( NULL );
        contextMenu_addCheckBox( CVGContextMenu, CVGCM_isAutoSpawning, 0, CVGCM_setAutoSpawning, 0, "CVGCMAutoSpawnPrompt" );
        contextMenu_addCode( CVGContextMenu, CVGCM_canMoveAll, 0, CVGCM_moveAll, 0, "CVGMoveAll", 0  );
    }
    inited = 1;
}
int customVillainGroupWindow()
{
    float x,y,z, wd, ht, sc;
    float coverageHeight;
    int clr, bkclr;
    if (!window_getDims( WDW_CUSTOMVILLAINGROUP, &x, &y, &z, &wd, &ht, &sc, &clr, &bkclr ))
    {
        return 0;
    }
    winDefs[WDW_CUSTOMVILLAINGROUP].maximizeable = 1;
    if (window_getMode( WDW_CUSTOMVILLAINGROUP ) == WINDOW_DISPLAYING )
    {
        int yoffset;
        int selectionBoxHeight = 300;
        AtlasTex * white = atlasLoadTexture( "white" );
        static HelpButton *coverageHelpButton = NULL;
        CBox coverageTooltipBox;
        int coverageTextWidth;
        char *helpCoverageText;
        if (!init)
        {
            int i;
            initCVGContextMenus();
            populateCVGSS();
            updateCustomCritterList(&missionMaker);
            smf_SetFlags(&saveFileName, SMFEditMode_ReadWrite, 0, SMFInputMode_PlayerNames, 30, SMFScrollMode_ExternalOnly, 
                SMFOutputMode_StripAllTags, SMFDisplayMode_AllCharacters, SMFContextMenuMode_None, SMAlignment_Left, 0, "CVGMenu", -1);
            for (i = 0; i < CVGLIST_RANKS; ++i)
            {
                rightCreatedVillainList[i].sb.architect = 1;
            }
            availableVillainList.sb.architect = 1;
            init = 1;
            selectionInfoMode = WINDOW_SHRINKING;
            si_time = 0;
            s_currentlySelectedPtr = NULL;
            dialogClearQueue(0);
        }
        if (init == 2)
        {
            handleCVGUIControl(VG_UI_SAVE);
            init = 1;
        }
        if ( MM_selectedCVGName )
        {
            int i = -1;
            for ( i = 0; i < eaSize(&pCVGCreatedElementList->ppElement); ++i)
            {
                if (stricmp(MM_selectedCVGName, pCVGCreatedElementList->ppElement[i]->pchDisplayName) == 0)
                {
                    pCVGCreatedElementList->current_element = i;
                    break;
                }
            }
            pCVGOriginalElementList->current_element = 0;
            vGroup = -1;
            CVGroupIndex = -1;
            s_currentlySelectedPtr = NULL;
            estrDestroy(&MM_selectedCVGName);
            MM_selectedCVGName = NULL;
        }
        if (!window_isFront(WDW_MISSIONMAKER))
        {
            updateScrollSelectors(y+(2*PADDING*sc), (y+ht)-(PADDING*sc));
        }
        if (!EAINRANGE( pCVGOriginalElementList->current_element, pCVGOriginalElementList->ppElement) )
        {
            pCVGOriginalElementList->current_element = 0;
            s_currentlySelectedPtr = NULL;
        }
        if (!EAINRANGE( pCVGCreatedElementList->current_element, pCVGCreatedElementList->ppElement) )
        {
            s_currentlySelectedPtr = NULL;
            pCVGCreatedElementList->current_element = 0;
        }

        //    draw outer frame
        drawWaterMarkFrame(x,y,z,sc, wd, ht, 1);
        font(&game_12);
        font_ital(1);
        font_color(CLR_MM_TITLE_OPEN, CLR_MM_TITLE_OPEN);
        cprntEx( x + R10 + PIX3, y + (PADDING*sc) + PIX3, z, sc*1.5, sc*1.5, CENTER_Y, "CVGCreatorTitleText" );
        display_sprite( white, x+ R10 + PIX3, y+(25*sc), z, (wd- (2*(R10+PIX3)))/(white->width),2.0f/(white->height), 0xbbbbbb44 );
        font_color(CLR_MM_TEXT, CLR_MM_TEXT);
        cprntEx( x + wd/2, y + ht - (55*sc),z, sc, sc, CENTER_X, "CVGVillainGroupLevelCoverageString");
        coverageTextWidth = str_wd(&game_12, sc, sc, textStd("CVGVillainGroupLevelCoverageString"));
        BuildCBox(&coverageTooltipBox, (x+wd/2)+((coverageTextWidth/2)*sc), y+ (ht-270*sc), (600*sc),(300*sc));
        estrCreate(&helpCoverageText);
        estrConcatf(&helpCoverageText, "%s <br><br>", textStd("CVGCoverageTooltip"));
        coverageHeight = helpButtonEx(&coverageHelpButton, (x+wd/2)+(coverageTextWidth/2)+(20*sc), y+ (ht-(65*sc)), z, sc, helpCoverageText, &coverageTooltipBox, HELP_BUTTON_USE_BOX_WIDTH );
        estrDestroy(&helpCoverageText);
        if (coverageHelpButton && coverageHelpButton->state )
        {
            font(&game_12);
            prnt( x + wd/2 + coverageTextWidth/2 + (200*sc) - (str_wd(&game_12, sc, sc, textStd("CVGLevelsNotCovered")) + 30*sc), coverageHeight - 5*sc, z+30, sc, sc, "CVGLevelsNotCovered");
            drawLevelRanges( x+wd/2+(coverageTextWidth/2)+200*sc, coverageHeight-20*sc, z+30, 200*sc, sc, 1 );
            prnt( x + wd/2 + coverageTextWidth/2 + (400*sc), coverageHeight - 5*sc, z+30, sc, sc, "CVGLevelsCovered");
        }
        font_ital(0);
        drawSaveControls( x + R10 + PIX3, y+35*sc, z, wd, 30*sc, sc);
        drawLevelRanges(x+(wd/2)-(240*sc),y+ ht - (50*sc),z,(480*sc), sc, 0);
        y += (80*sc);
        ht -= (150*sc);
        //    draw left side
        drawLeftGroupWindow(x +(PADDING*sc), y + (2*PADDING*sc), z, (wd/2)-((3*PADDING*sc/2)+(sc*BUTTON_SPACING/2)), 
            ht - ((3*PADDING*sc)), sc);

        //    draw right side
        yoffset = si_time*((selectionBoxHeight*sc)+(PADDING*sc))+ ((1-si_time)*((20+PADDING)*sc));
        drawRightGroupWindow(x + (sc*BUTTON_SPACING/2) + (wd/2) + (PADDING*sc/2), y + (2*PADDING*sc)+yoffset, z, 
            (wd/2)-((3*PADDING*sc/2)+(sc*BUTTON_SPACING/2)), ht - (yoffset+(3*PADDING*sc)) , sc);

        //    draw filter buttons
        yoffset = 0;
//        drawFilterButtons(x, y+yoffset + (110*sc),z+20, wd/2, ht,sc);
        drawSortOptions((x+wd/2) -(20*sc), y+yoffset + (108*sc), z+20, sc, &availableVillainList);
        //    draw info about villain that is currently selected
        drawSelectionInfo(x + (sc*BUTTON_SPACING/2) + (wd/2) + (PADDING*sc/2), y + (10*sc) +(PADDING*sc),z, 
            (wd/2)-((3*PADDING*sc/2)+(sc*BUTTON_SPACING/2)), si_time*sc*(selectionBoxHeight-10), sc);

        if (ht < (600*sc))
        {
            selectionInfoMode = WINDOW_SHRINKING;
        }
        switch (selectionInfoMode)
        {
        case WINDOW_SHRINKING:
            si_time -= TIMESTEP * 0.1f;
            si_time = CLAMPF32(si_time, 0.0f, 1.0f);
            break;
        case WINDOW_GROWING:
            si_time += TIMESTEP * 0.1f;
            si_time = CLAMPF32(si_time, 0.0f, 1.0f);
            if (si_time >= 1.0)
            {
                selectionInfoMode = WINDOW_DISPLAYING;
            }
            break;
        }

    }
    else
    {
        init = 0;
    }
    return 0;
}