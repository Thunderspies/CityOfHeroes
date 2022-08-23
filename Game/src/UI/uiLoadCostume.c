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

#include "imageServer.h"

#include "UI/uiGame.h"
#include "UI/uiUtil.h"
#include "UI/uiInput.h"
#include "UI/uiAvatar.h"
#include "UI/uiTailor.h"
#include "UI/uiCostume.h"
#include "uiLoadCostume.h"
#include "UI/uiGender.h"
#include "UI/uiUtilGame.h"
#include "UI/uiUtilMenu.h"  
#include "uiWindows.h"
#include "UI/uiWindows.h"
#include "UI/uiSuperRegistration.h"
#include "UI/uiEditText.h"
#include "UI/uiNet.h"
#include "UI/uiSupercostume.h"

#include "formatter/smf_main.h"
#include "graphics/ttFontUtil.h"
#include "UI/sprite/sprite_base.h" 
#include "UI/sprite/sprite_text.h"
#include "UI/sprite/sprite_font.h"
#include "render/tex.h"

#include "uiListView.h"
#include "UI/uiClipper.h"
#include "UI/uiTailor.h"
#include "UIEdit.h"
#include "UI/uiFocus.h"
#include "filter/validate_name.h"
#include <utilitieslib/components/EString.h>
#include "UI/uiDialog.h"
#include "sound/sound.h"
#include <utilitieslib/utils/file.h>
#include <utilitieslib/utils/ConvertUtf.h>
#include "graphics/font.h"
#include <utilitieslib/components/StringCache.h>
#include "graphics/textureatlas.h"    //    for force load button
#include "auth/authUserData.h"
#include "UI/uiUtilMenu.h"
#include "uiPCCCreationNLM.h"
#include "UI/Hybrid/uiHybridMenu.h"

#include <direct.h> //for getcwd()

#define DEFAULT_CLR_BACK 0x00000088
#define COMPARECOSTUMEPART(a,b) if (a != b) {return 0;}

#define BUTTON_HEIGHT_SCALE  .7f
#define BUTTON_HEIGHT 54.f*BUTTON_HEIGHT_SCALE
#define LOAD_LIST_HEIGHT 470.f
#define BUTTON_WIDTH 300.f
#define LIST_START_X 50.f
#define LIST_START_Y 150.f
#define BUTTON_SPACING_Y 3.f*BUTTON_HEIGHT_SCALE
#define LIST_SCROLL_SPEED 10.f

static TextAttribs textAttr_Error =
{
    /* piBold        */  (int *)0,
    /* piItalic      */  (int *)0,
    /* piColor       */  (int *)0xff2222ff,
    /* piColor2      */  (int *)0,
    /* piColorHover  */  (int *)0xff2222ff,
    /* piColorSelect */  (int *)0,
    /* piColorSelectBG*/ (int *)0x333333ff,
    /* piScale       */  (int *)(int)(1.1f*SMF_FONT_SCALE),
    /* piFace        */  (int *)&game_12,
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
static int prevMenu = MENU_COSTUME;
static const cCostume *prevCostume;
static int prevBodyType;
static int prevOriginSet;
static int prevGender;
extern int loadedCostume;
static int stanceBits = 0;
static int enableForceLoad = 0;
static int reloadCostume = 0;
extern int gColorsLinked;
static int init = 0;
extern GenderChangeMode genderEditingMode;

HybridElement **ppCostumeList;
static int selectedIndex = -1;
static int lastHideStoreItems;

void addCostumeToList(char * name)
{
    HybridElement * he = calloc(1,sizeof(HybridElement));
    he->text = strdup(name);
    he->text_flags = NO_MSPRINT;    // Don't translate the costume file name
    eaPush(&ppCostumeList, he);
}

static void doScaling()
{
    Entity *e = playerPtr();

       switch( e->costume->appearance.bodytype )
    {
    case kBodyType_Huge:
        scaleHero( e, -10.f );
        break;
    case kBodyType_Female:
        scaleHero( e, 0.f );
        break;

    case kBodyType_Male:
    default:
        scaleHero( e, -10.f );
        break;
    }
}

void loadCostume_start(int previous_menu)
{
    Entity *e = playerPtr();

    texDisableThreadedLoading();
    entSetAlpha( e, 255, SET_BY_CAMERA );    
    enableForceLoad = 0;
    prevMenu = previous_menu;
    prevCostume = e->costume;    
    e->costume = costume_as_const(costume_clone(e->costume));

    prevGender = gCurrentGender;
    prevBodyType = gCurrentBodyType;
    prevOriginSet = gCurrentOriginSet;
    lastHideStoreItems = getHideStorePiecesState();
    setHideStorePiecesState(HIDE_STORE_PIECES_UNCHECKED);  // this is required so the player can see store unpurchased pieces, even if the costume menu is currently hiding them
    start_menu(MENU_LOADCOSTUME);
}

static void loadCostume_cancel(void)
{
    Entity *e = playerPtr();
    setHideStorePiecesState(lastHideStoreItems);
    texEnableThreadedLoading();
    costume_destroy(costume_as_mutable_cast(e->costume));
    e->costume = prevCostume;

    if (prevMenu == MENU_COSTUME)
    {
        e->pl->costume[0] = costume_as_mutable_cast(e->costume);
    }

    gCurrentGender = prevGender;
    gCurrentBodyType = prevBodyType;
    gCurrentOriginSet = prevOriginSet;
    costume_setCurrentCostume(e,0);
    updateCostume();
    costume_Apply(e);
    init = 0;
    toggle_3d_game_modes(SHOW_TEMPLATE);
}

#define COSTUME_CHAR_BUFFER_SIZE 300
static int costume_CorrectText(Costume * costume, int **badPartList)
{
    int i;
    int isLoadable = 1;
     eaiDestroy(badPartList);
    if (costume->appearance.costumeFilePrefix && strlen(costume->appearance.costumeFilePrefix) > COSTUME_CHAR_BUFFER_SIZE)
    {
        //    This stuff isn't actually important but it does imply that the person is messing with the costume file and trying to overflow things
        costume->appearance.costumeFilePrefix = NULL;
        isLoadable = 0;
    }
    if (costume->appearance.entTypeFile && strlen(costume->appearance.entTypeFile) > COSTUME_CHAR_BUFFER_SIZE)
    {
        //    This stuff isn't actually important but it does imply that the person is messing with the costume file and trying to overflow things
        costume->appearance.entTypeFile = NULL;
        isLoadable = 0;
    }
    for (i = 0; i < costume->appearance.iNumParts; i++)
    {
        //    Problem parts here aren't irrecoverable, 
        //    but it does imply that the person is messing with the costume file and trying to overflow things.
        //    I guess we should allow them to recover ...
        CostumePart *srcPart;
        int badPart = -1;
        srcPart = eaGet(&costume->parts, i);
        if (srcPart->bodySetName && (strlen(srcPart->bodySetName) > COSTUME_CHAR_BUFFER_SIZE))
        {
            srcPart->bodySetName = NULL;
            badPart = i;
        }
        if (srcPart->displayName && (strlen(srcPart->displayName) > COSTUME_CHAR_BUFFER_SIZE))    
        {    
            srcPart->displayName = NULL;
            badPart = i;
        }
        if (srcPart->pchFxName && (strlen(srcPart->pchFxName) > COSTUME_CHAR_BUFFER_SIZE))        
        {
            srcPart->pchFxName = allocAddString("none");
            badPart = i;
        }
        if (srcPart->pchGeom && (strlen(srcPart->pchGeom) > COSTUME_CHAR_BUFFER_SIZE))        
        {
            srcPart->pchGeom = allocAddString("none");
            badPart = i;
        }
        if (srcPart->pchName && (strlen(srcPart->pchName) > COSTUME_CHAR_BUFFER_SIZE))        
        {
            srcPart->pchName = NULL;
            badPart = i;
        }
        if (srcPart->pchTex1 && (strlen(srcPart->pchTex1) > COSTUME_CHAR_BUFFER_SIZE))    
        {
            srcPart->pchTex1 = allocAddString("none");
            badPart = i;
        }
        if (srcPart->pchTex2 && (strlen(srcPart->pchTex2) > COSTUME_CHAR_BUFFER_SIZE))    
        {
            srcPart->pchTex2 = allocAddString("none");
            badPart = i;
        }
        if (srcPart->sourceFile && (strlen(srcPart->sourceFile) > COSTUME_CHAR_BUFFER_SIZE))
        {
            srcPart->sourceFile = NULL;
            badPart = i;
        }
        if (badPart != -1)
        {
            eaiPush(badPartList, badPart);
        }
    }    
    return isLoadable;
}

static void correctOptionalRegions( const cCostume *originalCostume, Costume *loadCostume)
{
    int i, j, region_total, numParts;
    const CostumeOriginSet *cset = costume_get_current_origin_set();
    region_total = eaSize(&cset->regionSet);
    region_total = min(region_total, REGION_SHIELD+1);
    numParts = max(originalCostume->appearance.iNumParts, loadCostume->appearance.iNumParts);
    numParts = min(numParts, eaSize(&bodyPartList.bodyParts));
    stanceBits = 0;
    //    for all optional regions
    for (i = REGION_WEAPON; i < region_total; i++)
    {
        const CostumeRegionSet *rset = cset->regionSet[i];
        //    check all the parts
        for (j = 0; j < numParts; j++)
        {
            //    if the costume used to have a weapon/shield
            if (prevCostume->parts[j]->regionName && (stricmp(rset->name, prevCostume->parts[j]->regionName)==0))
            {
                //    but the new one doesn't
                if(!(loadCostume->parts[j]->regionName && (stricmp(rset->name, loadCostume->parts[j]->regionName) ==0) ))
                {
                    //    use the previous one
                    StructCopy(ParseCostumePart, prevCostume->parts[j], loadCostume->parts[j], 0,0);
                }    
                //    if it does, show the new shield as an example
                else
                {
                    if (prevCostume->appearance.bodytype == loadCostume->appearance.bodytype)
                    {
                        if (prevCostume->parts[j]->pchFxName)
                        {
                            //    the format of pchFxName will be...
                            //    WEAPONS/Custom_WeaponType/WeaponName.weapon
                            //    make sure that it matches the same weapon type
                            //    if not, set it back to the prev weapon
                            char tempStr[COSTUME_CHAR_BUFFER_SIZE+1];
                            char *result;
                            strcpy_s(tempStr, COSTUME_CHAR_BUFFER_SIZE,prevCostume->parts[j]->pchFxName);
                            result = strtok(tempStr, "/\\");
                            if (result)
                            {
                                result = strtok(NULL, "/\\");
                                if (loadCostume->parts[j]->pchFxName && result)
                                {
                                    char tempStr2[COSTUME_CHAR_BUFFER_SIZE+1];
                                    char *result2;
                                    strcpy_s(tempStr2, COSTUME_CHAR_BUFFER_SIZE, loadCostume->parts[j]->pchFxName);
                                    result2= strtok(tempStr2, "/\\");
                                    if (result2)
                                    {
                                        result2= strtok(NULL, "/\\");
                                        if (result2 && strcmpi(result, result2) == 0)
                                        {
                                            int stanceForThis = 0;
                                            costume_SetStanceFor(prevCostume->parts[j], &stanceForThis);
                                            stanceBits |= stanceForThis;
                                            continue;
                                        }                            
                                    }
                                }
                            }
                        }
                    }
                    //    that's strange. weapons/shields always should have a pchFxName
                    StructCopy(ParseCostumePart, prevCostume->parts[j], loadCostume->parts[j], 0,0);
                }
            }
            else if(loadCostume->parts[j]->regionName && (stricmp(rset->name, loadCostume->parts[j]->regionName) ==0) )
            {
                //    use the previous one
                StructCopy(ParseCostumePart, prevCostume->parts[j], loadCostume->parts[j], 0,0);
            }    
        }
    }
}
//    loadCostume_Validate checks for valid input
static int loadCostume_validate(Costume *costume, int slotid, int **badPartList, int **badColorList)
{        
    int retVal = COSTUME_ERROR_NOERROR;
    if (!costume->parts)
    {
        return COSTUME_ERROR_INVALID_NUM_PARTS;
    }
    if (eaSize(&costume->parts) != MAX_COSTUME_PARTS)
    {
        return COSTUME_ERROR_INVALID_NUM_PARTS;
    }

    //    Check for valid text (no buffer overflows)
    if (!costume_CorrectText(costume, badPartList))
    {
        retVal |= COSTUME_ERROR_INVALIDFILE;
    }
    else
    {
        if (eaiSize(badPartList) > 0)
        {
            retVal |= COSTUME_ERROR_BADPART;
        }        
    }

    //    Validate the costume
    getCurrentBodyTypeAndOrigin(playerPtr(), costume, &gCurrentBodyType, NULL);
    correctOptionalRegions(prevCostume, costume);
    retVal |= costume_Validate(playerPtr(), costume, slotid, badPartList, badColorList, getPCCEditingMode(), true );
    return retVal;
}
int fixCostumeParts(const cCostume *originalCostume, Costume *loadCostume, int slotid, int **badPartList, int **badColorList)
{
    Entity *e = playerPtr();
    int i, numParts;
    numParts = max(originalCostume->appearance.iNumParts, loadCostume->appearance.iNumParts);
    numParts = min(numParts, eaSize(&bodyPartList.bodyParts));
    eaiDestroy(badPartList);
    eaiDestroy(badColorList);
    for (i = 0; i < numParts; i++)
    {
        if (!costume_validatePart(loadCostume->appearance.bodytype, i, loadCostume->parts[i], e, getPCCEditingMode(), true, slotid ))
        {
            StructCopy(ParseCostumePart, originalCostume->parts[i], loadCostume->parts[i], 0,0);
        }
    }
    costumeCorrectColors(e, loadCostume, badColorList);
    return costume_Validate(e, loadCostume, slotid, badPartList, badColorList, getPCCEditingMode(), true );
}
static Costume LoadCostumeForParser ={0};

static int loadCostumeFromFile(char * filename, int *costumeValid, int **badPartList, int **badColorList, int loadSelf)
{    
    Entity *e = playerPtr();
    Costume *plCostume = NULL;
    int i;
    int *diffPartList = NULL;
    *costumeValid = COSTUME_ERROR_NOERROR;    
    
    resetCostume(1);
    gColorsLinked = false;
    StructClear(ParseCostume, &LoadCostumeForParser);
    StructInitFields(ParseCostume, &LoadCostumeForParser);
    if (loadSelf)
    {    
        int oldPCC;
        oldPCC = getPCCEditingMode();
        setPCCEditingMode(0);
        e = playerPtr();
        if (e->costume)
        {
            StructCopy(ParseCostume, e->costume, &LoadCostumeForParser, 0,0);
        }
        else
        {
            Errorf(textStd("NoCostumeString") );
            //    error message about no costume to load
        }
        setPCCEditingMode(oldPCC);
        e = playerPtr();
    }
    else
    {
        assert(filename);
        if (!ParserLoadFiles(NULL, filename, NULL, 0, ParseCostume, &LoadCostumeForParser, 0, 0, NULL ) )
        {
            *costumeValid = COSTUME_ERROR_INVALIDFILE;
            return false;    
        }
        if ( e->db_id && !genderEditingMode)
        {
            //    If this is a player who isn't editing from the gender screen (i.e. no gender change permissions)
            //    lock in the height so it can pass validation..
            LoadCostumeForParser.appearance.fScale = prevCostume->appearance.fScale;
        }
    }    

    LoadCostumeForParser.appearance.convertedScale = prevCostume->appearance.convertedScale;
    //    we force their fscale to their original fscale
    if (LoadCostumeForParser.appearance.iNumParts < MAX_COSTUME_PARTS)
    {
        Costume *costume = &LoadCostumeForParser;
        int i = eaSize(&costume->parts);
        costume->appearance.iNumParts = MAX_COSTUME_PARTS;  // Temporarily set this, or the debug client asserts in a few lines

        // BW - For technical reasons, we need GetBodyPartCount() in appearance->iNumParts, but actually need MAX_COSTUME_PARTS entries in ->parts to pass validation checks later
        while (i < MAX_COSTUME_PARTS)
        {
            eaPush(&costume->parts, StructAllocRaw(sizeof(CostumePart)));
            costume_PartSetGeometry(costume, i, NULL);
            costume_PartSetTexture1(costume, i, NULL);
            costume_PartSetTexture2(costume, i, NULL);
            costume_PartSetFx(costume, i, NULL);
            i++;
        }
        costume->appearance.iNumParts = GetBodyPartCount(); // Set this back to the correct value
    }

    for(i=1;i<MAX_BODY_SCALES;i++)
    {
        LoadCostumeForParser.appearance.fScales[i] = CLAMPF32(LoadCostumeForParser.appearance.fScales[i],-1,1);
    }
    //    loadCostume_Validate checks/corrects for valid input
    *costumeValid =loadCostume_validate(&LoadCostumeForParser, e->pl->current_costume, badPartList, badColorList);    
    if (*costumeValid > COSTUME_ERROR_MUST_EXIT)
    {
        return false;
    }

    //    Now we check to see if the costume can be worn/applied
    if (prevCostume->appearance.bodytype != LoadCostumeForParser.appearance.bodytype)
    {
        *costumeValid |= COSTUME_ERROR_GENDERCHANGE;
    }
    if (enableForceLoad && (*costumeValid < COSTUME_ERROR_CAN_FORCE_LOAD)  && (*costumeValid != COSTUME_ERROR_NOERROR))
    {
        *costumeValid = fixCostumeParts(prevCostume, &LoadCostumeForParser, e->pl->current_costume, badPartList, badColorList);
    }
    costume_destroy(costume_as_mutable_cast(e->costume));
    e->costume = costume_as_const( costume_clone( costume_as_const(&LoadCostumeForParser) ) );
    plCostume = e->pl->costume[e->pl->current_costume];
    e->pl->costume[e->pl->current_costume] = costume_as_mutable_cast(e->costume);
    costume_setCurrentCostume( e, 0 );
    genderSetScalesFromApperance(e);
    updateCostume();
    e->pl->costume[e->pl->current_costume] = plCostume;
    eaiCreate(&diffPartList);
    costume_getDiffParts(costume_as_const(&LoadCostumeForParser), e->costume, &diffPartList);
    if (enableForceLoad)
    {
        int itr;
        for (itr = 0; itr < eaiSize(&diffPartList); itr++)
        {
            StructCopy(ParseCostumePart, prevCostume->parts[diffPartList[itr]], LoadCostumeForParser.parts[diffPartList[itr]], 0, 0 );
        }
    }
    else
    {
        int itr;
        eaiDestroy(badPartList);
        for (itr = 0; itr < eaiSize(&diffPartList); itr++)
        {
            eaiPush(badPartList, diffPartList[itr]);
        }
        if (eaiSize(&diffPartList))
        {
            *costumeValid |= COSTUME_ERROR_NOTAWARDED;
        }
    }
    eaiDestroy(&diffPartList);
    if ((*costumeValid) != COSTUME_ERROR_NOERROR)
    {
        costume_destroy(costume_as_mutable_cast(e->costume));    
        e->costume = costume_as_const( costume_clone( costume_as_const(&LoadCostumeForParser) ) );
    }
    return 1;
}

void loadCostume_next()
{
    Entity *e = playerPtr();
    Costume *plCostume = NULL;
    texEnableThreadedLoading();
    costume_destroy( costume_as_mutable_cast(prevCostume) );
    prevCostume = 0;

    if (prevMenu == MENU_TAILOR)
    {
        //    There was some concern that this was leaking memory because gTailoredCostume wasn't getting destroyed. However,
        //    it is getting destroyed by costume_destroy(prevCostume) which points to e->costume which points to gTailoredCostume. -EL
        gTailoredCostume = costume_as_mutable_cast(e->costume);
        loadedCostume = 1;
        plCostume = e->pl->costume[e->pl->current_costume];
    }
    if (game_state.editnpc)
    {
        switch (e->costume->appearance.bodytype)
        {
        case kBodyType_Male:
            gCurrentGender = AVATAR_GS_MALE;
            break;
        case kBodyType_Female:
            gCurrentGender = AVATAR_GS_FEMALE;
                break;
        case kBodyType_BasicMale:
            gCurrentGender = AVATAR_GS_MALE;
                break;
        case kBodyType_BasicFemale:
            gCurrentGender = AVATAR_GS_FEMALE;
                break;
        case kBodyType_Huge:
            gCurrentGender = AVATAR_GS_HUGE;
                break;
        case kBodyType_Enemy:
            gCurrentGender = AVATAR_GS_ENEMY;
                break;
        case kBodyType_Villain:
            gCurrentGender = AVATAR_GS_BM;
                break;
        case kBodyType_Enemy2:
            gCurrentGender = AVATAR_GS_ENEMY2;
                break;
        case kBodyType_Enemy3:
            gCurrentGender = AVATAR_GS_ENEMY3;
                break;
        default:
            break;
            //    warning?
        }
    }
    else
    {
        assert(gCurrentBodyType == prevBodyType);
        assert(gCurrentOriginSet == prevOriginSet);
        assert(gCurrentGender == prevGender);
    }
    e->pl->costume[e->pl->current_costume] = costume_as_mutable_cast(e->costume);
    genderSetScalesFromApperance(e);
    updateCostume();

    if (prevMenu == MENU_TAILOR)
    {
        e->pl->costume[e->pl->current_costume] = plCostume;
    }

    costume_setCurrentCostume( e, 0 );
    costume_Apply( e );
    doScaling();
    init = 0;
    start_menu( prevMenu );
    toggle_3d_game_modes(SHOW_TEMPLATE);
}

UIBox costumesListViewDisplayItem(UIListView* list, PointFloatXYZ rowOrigin, void* userSettings, char* item, int itemIndex)
{
    UIBox box;
    PointFloatXYZ pen = rowOrigin;
    UIColumnHeaderIterator columnIterator; 
    int color;

    box.origin.x = rowOrigin.x;
    box.origin.y = rowOrigin.y;

    uiCHIGetIterator(list->header, &columnIterator);

    rowOrigin.z+=1;

    if(item == list->selectedItem)
    {
        color = CLR_WHITE;
    }
    else
    {
        color = CLR_GREY;
    }

    font_color(color, color);

    // Iterate through each of the columns.
    for(;uiCHIGetNextColumn(&columnIterator, list->scale);)
    {
        UIColumnHeader* column = columnIterator.column;
        UIBox clipBox;
        int rgba[4] = {color, color, color, color };

        if(item)
        {
            pen.x = rowOrigin.x + columnIterator.columnStartOffset*list->scale;

            clipBox.origin.x = pen.x;
            clipBox.origin.y = pen.y;
            clipBox.height = 20*list->scale;

            if( columnIterator.columnIndex >= eaSize(&columnIterator.header->columns )-1 )
                clipBox.width = 1000;
            else
                clipBox.width = columnIterator.currentWidth;

            clipperPushRestrict(&clipBox);

            if(str_wd_notranslate(&game_12, list->scale , list->scale, item) <= clipBox.width)
                font(&game_12);
            else
                font(&game_9);

            printBasic(font_grp, pen.x, pen.y+18*list->scale, rowOrigin.z, list->scale, list->scale, 0, item, strlen(item), rgba );

            clipperPop();
        }
    }

    box.height = 0;
    box.width = list->header->width;

    // Returning the bounding box of the item does not do anything right now.
    // See uiListView.c for more details.
    return box;
}

static UIListView *lvCostumes = 0;

#include <stdio.h>
#include <mbstring.h>

void freeCostumeStr(char *costumeStr)
{
    if (costumeStr)
        free(costumeStr);
}

static int refreshList = 0;
static void deleteCurrentCostume( void* data )
{
    if(EAINRANGE(selectedIndex, ppCostumeList) )
    {
        char fname[MAX_PATH];
        sprintf_s(fname, sizeof(fname), "%s/%s", getCustomCostumeDir(), ppCostumeList[selectedIndex]->text);

        char *p = strchr(fname, '.');
        if (p == NULL)
        {
            strcat_s(fname, sizeof(fname), ".v2costume");
        }
        else
        {
            // Costume ends in '.', otherwise this can't be here, due to other code
            *p = 0;
            strcat_s(fname, sizeof(fname), ".costume");
        }
        
        remove(fname);
        refreshList = 1;
        selectedIndex = -1;
    }
}
static FileScanAction costumeInfoProcessor(char *dir, struct _finddata32_t *data)
{
    char fullpath[MAX_PATH];
    sprintf_s(fullpath, sizeof(fullpath), "%s/%s", dir, data->name);

    // Standard (new-format) costume
    if (simpleMatch("*.v2costume", fullpath))
    {
        size_t size = strlen(data->name) + 2;   // zero terminator  one char for the end . if needed
        char* name = (char*)_alloca(size);
        strcpy_s(name, size, data->name);

        char *p = strrchr(name, '.');
        assert(p);
        if ( p != NULL )
        {
            *p = 0;
        }

        addCostumeToList(name);
    }
    // Legacy (old-format) costume
    else if (simpleMatch("*.costume", fullpath))
    {
        size_t size = strlen(data->name) + 2;   // zero terminator  one char for the end . if needed
        char* name = (char*)_alloca(size);
        strcpy_s(name, size, data->name);

        char *p = strrchr(name, '.');
        assert(p);
        if ( p != NULL )
        {
            *p = 0;
        }

        //The . here lets us know this is a .costume not a .v2costume later
        strcat_s(name, size, ".");

        addCostumeToList(name);
    }
    return FSA_NO_EXPLORE_DIRECTORY;
}

#define LINK_Z 5000
static int drawForceLoadSelector( float x, float y)
{
    AtlasTex *check, *checkD;
    AtlasTex * glow   = atlasLoadTexture("costume_button_link_glow.tga");
    int checkColor = CLR_WHITE;
    int checkColorD = CLR_WHITE;
    int retVal = false;
    CBox box;
    F32 xposSc, yposSc, textXSc, textYSc;
    calculatePositionScales(&xposSc, &yposSc);
    calculateSpriteScales(&textXSc, &textYSc);

    if( enableForceLoad )
    {
        check  = atlasLoadTexture("costume_button_link_check.tga");
        checkD = atlasLoadTexture("costume_button_link_check_press.tga");
    }
    else
    {
        check   = atlasLoadTexture("costume_button_link_off.tga");
        checkD  = atlasLoadTexture("costume_button_link_off_press.tga");
    }

    BuildScaledCBox( &box, x - check->width*xposSc/2, y - check->width*yposSc/2, check->width, check->height );

    if( mouseCollision( &box ) )
    {
        display_sprite_positional( glow, x, y, LINK_Z-1, 1.f, 1.f, CLR_WHITE, H_ALIGN_CENTER, V_ALIGN_CENTER );

        if( isDown( MS_LEFT ) )
            display_sprite_positional( checkD, x, y, LINK_Z, 1.f, 1.f, checkColorD, H_ALIGN_CENTER, V_ALIGN_CENTER );
        else
            display_sprite_positional( check, x, y, LINK_Z, 1.f, 1.f, checkColor, H_ALIGN_CENTER, V_ALIGN_CENTER );

        if( mouseClickHit( &box, MS_LEFT) )
        {
            enableForceLoad = !enableForceLoad;
            retVal = 1;
        }
    }
    else
        display_sprite_positional( check, x, y, LINK_Z, 1.f, 1.f, checkColor, H_ALIGN_CENTER, V_ALIGN_CENTER );

    setSpriteScaleMode(SPRITE_XY_NO_SCALE);
    font( &game_9 );
    font_color( CLR_WHITE, CLR_WHITE );
    prnt( x + (check->width/2 + 5)*xposSc, y+5*yposSc, LINK_Z, textXSc, textYSc, "ForceLoadString");
    setSpriteScaleMode(SPRITE_Y_UNIFORM_SCALE);

    return retVal;
}

static int loadCostume_hiddenCode(void)
{
    return 1;
}

#define MAX_NUM_SIMULTANEOUS_ERRORS 1



void loadCostume_menu()
{
    static int loadCost = 1;
    static int notReady = 1;
    static int costumeStatus = COSTUME_ERROR_NOERROR;
    static int *badColorList;
    static int *badPartList;

    Entity *e;
    int i;
    static F32 yoffset = 0.f;
    UIBox uibox;
    int flags = HB_TAIL_RIGHT_ROUND | HB_FLASH;
    int listSize;
    F32 maxOffset;
    CBox box;
    int newCostumeClicked = 0;
    static HybridElement sButtonDelete = {0, "DeleteString", NULL, "icon_loadsave_delete_0"};
    F32 xposSc, yposSc, textXSc, textYSc, xp, yp;

    if( (window_getMode(WDW_DIALOG) != WINDOW_DOCKED) )
        flags |= HB_UNSELECTABLE;

    if(drawHybridCommon(HYBRID_MENU_NO_NAVIGATION))
        return;

    if (getPCCEditingMode() < 0)
        setPCCEditingMode(getPCCEditingMode()*-1);
    e = playerPtr();

    if( !init )
    {
        init = 1;
        notReady = 1;
        costumeStatus = COSTUME_ERROR_NOERROR;
        refreshList = 1;
    }

    if (refreshList)
    {
        refreshList = 0;
        while(eaSize(&ppCostumeList))
        {
            HybridElement *h = eaRemove(&ppCostumeList, 0);
            SAFE_FREE(h->text);
            SAFE_FREE(h);
        }
        fileScanDirRecurseEx(getCustomCostumeDir(), costumeInfoProcessor);
    }

    setScalingModeAndOption(SPRITE_Y_UNIFORM_SCALE, SSO_SCALE_TEXTURE);
    calculatePositionScales(&xposSc, &yposSc);
    calculateSpriteScales(&textXSc, &textYSc);
    xp = 512.f;
    yp = 0.f;
    applyPointToScreenScalingFactorf(&xp, &yp);
    setSpriteScaleMode(SPRITE_XY_NO_SCALE);
    font(&title_18);
    font_outl(0);
     font_color(CLR_WHITE, CLR_WHITE);
    prnt( 50.f*xposSc, 120.f*yposSc, ZOOM_Z, 1.3f*textXSc, 1.3f*textYSc, "LoadingCostume" );
    font_outl(1);
    setSpriteScaleMode(SPRITE_Y_UNIFORM_SCALE);

    uiBoxDefineScaled(&uibox, 0, LIST_START_Y*yposSc, 500.f, LOAD_LIST_HEIGHT*yposSc );

    listSize = eaSize(&ppCostumeList);
    maxOffset = listSize * (BUTTON_HEIGHT+BUTTON_SPACING_Y) -  LOAD_LIST_HEIGHT;
    BuildScaledCBox(&box, LIST_START_X*xposSc, LIST_START_Y*yposSc, BUTTON_WIDTH*xposSc, LOAD_LIST_HEIGHT*yposSc );
    drawHybridScrollBar((LIST_START_X + BUTTON_WIDTH - 10.f)*xposSc, LIST_START_Y*yposSc, 5000.f, LOAD_LIST_HEIGHT, &yoffset, maxOffset, &box);
    
    clipperPush(&uibox);
    for( i = 0; i < listSize; i++ )
    {
          if( D_MOUSEHIT == drawHybridBar( ppCostumeList[i], i == selectedIndex, LIST_START_X*xposSc, (LIST_START_Y - yoffset + BUTTON_HEIGHT/2.f + i*(BUTTON_HEIGHT+BUTTON_SPACING_Y))*yposSc, 5000.f, BUTTON_WIDTH, BUTTON_HEIGHT_SCALE, flags, 1.f, H_ALIGN_LEFT, V_ALIGN_CENTER ))
        {
            selectedIndex = i;
            reloadCostume = 1;
            newCostumeClicked = 1;
        }
    }
    clipperPop();

    if (reloadCostume && selectedIndex >= 0)
    {
        //turn off "costume fix" after choosing a new costume
        if (newCostumeClicked)
        {
            enableForceLoad = 0;
        }
        loadCost = 0;
        reloadCostume = 0;

        // BW: There are two possible filenames here, if it ends in '.' we are a legacy, otherwise we are standard
        char fname[MAX_PATH];
        sprintf_s(fname, sizeof(fname), "%s/%s", getCustomCostumeDir(), ppCostumeList[selectedIndex]->text);
        char *p = strchr(fname, '.');
        if (p == NULL)
        {
            strcat_s(fname, sizeof(fname), ".v2costume");
        }
        else
        {
            // Costume ends in '.', otherwise this can't be here, due to other code
            *p = 0;
            strcat_s(fname, sizeof(fname), ".costume");
        }

        if (fname)
        {
            eaiDestroy(&badColorList);
            eaiDestroy(&badPartList);
            notReady = !loadCostumeFromFile(fname, &costumeStatus, &badPartList, &badColorList, 0);
        }
    }

    if (getPCCEditingMode())  // this will need to be fixed later
    {
        if (drawStdButton( 500, 680, 5000, 60, 20, CLR_H_BACK, "LoadSelfString",  1.f, 0))
        {
            notReady = !loadCostumeFromFile(NULL, &costumeStatus, &badPartList, &badColorList, 1);
        }
    }

    if (costumeStatus != COSTUME_ERROR_NOERROR)
    {
        static SMFBlock *errorText = 0;
        int x,y,z,width,height, errorCount;
         x = 730; y = 100; z = 30; width = 250; height = 25;
        errorCount = 0;
        textAttr_Error.piScale = (int *)(int)(1.1f*SMF_FONT_SCALE);
        if ( costumeStatus & COSTUME_ERROR_INVALIDFILE )
        {
            if (errorCount < MAX_NUM_SIMULTANEOUS_ERRORS)
            {
                y += smf_ParseAndDisplay(errorText, textStd("InvalidCostumeErrorString"), x, y, z, width, height, 0,0, &textAttr_Error, NULL, 0, false);
            }
            errorCount++;
        }
        if ( costumeStatus & COSTUME_ERROR_INVALIDSCALES )
        {
            if (errorCount < MAX_NUM_SIMULTANEOUS_ERRORS)
            {
                y += smf_ParseAndDisplay(errorText, textStd("CostumeScalesErrorString"), x, y, z, width, height, 0,0, &textAttr_Error, NULL, 0, false);
            }
            errorCount++;
        }
        if ( costumeStatus & COSTUME_ERROR_INVALID_NUM_PARTS)
        {
            if (errorCount < MAX_NUM_SIMULTANEOUS_ERRORS)
            {
                y += smf_ParseAndDisplay(errorText, textStd("CostumeInvalidNumPartString"), x, y, z, width, height, 0,0, &textAttr_Error, NULL, 0, false);            
            }
            errorCount++;
        }
        if ( costumeStatus & COSTUME_ERROR_GENDERCHANGE )
        {
            if (errorCount < MAX_NUM_SIMULTANEOUS_ERRORS)
            {
                y += smf_ParseAndDisplay(errorText, textStd("CostumeGenderErrorString"), x, y, z, width, height, 0,0, &textAttr_Error, NULL, 0, false);
            }
            errorCount++;
        }
        if ( costumeStatus & COSTUME_ERROR_BADPART )
        {
            if (errorCount < MAX_NUM_SIMULTANEOUS_ERRORS)
            {
                int i;
                char *text = NULL;
                for (i = 0; i < eaiSize(&badPartList); i++ )
                {
                    estrConcatf(&text, "%s ", textStd("CostumeBadPartPair", badPartList[i]+1, 
                        textStd(bodyPartList.bodyParts[badPartList[i]]->name ? bodyPartList.bodyParts[badPartList[i]]->name : "unknown")));
                }
                y += smf_ParseAndDisplay(errorText, textStd("CostumeBadPartString", text), x, y, z, width, height, 0,0, &textAttr_Error, NULL, 0, false);
                estrDestroy(&text);
            }
            errorCount++;
        }
        if ( costumeStatus & COSTUME_ERROR_NOTAWARDED )
        {
            if (errorCount < MAX_NUM_SIMULTANEOUS_ERRORS)
            {
                int i;
                char *text = NULL;
                for (i = 0; i < eaiSize(&badPartList); i++ )
                {
                    estrConcatf(&text, "%s ", textStd("CostumeBadPartPair", badPartList[i]+1, 
                        textStd(bodyPartList.bodyParts[badPartList[i]]->name ? bodyPartList.bodyParts[badPartList[i]]->name : "unknown")));
                }
                y += smf_ParseAndDisplay(errorText, textStd("CostumeMatchingErrorString", text), x, y, z, width, height, 0,0, &textAttr_Error, NULL, 0, false);
                estrDestroy(&text);
            }
            errorCount++;
        }    
        if ( costumeStatus & COSTUME_ERROR_INVALIDCOLORS )
        {
            if (errorCount < MAX_NUM_SIMULTANEOUS_ERRORS)
            {                    
                int i;
                char *text = NULL;
                for (i = 0; i < eaiSize(&badColorList); i++ )
                {
                    const char *output;
                    if (badColorList[i] == 0)
                    {
                        output = "SkinColor";
                    }
                    else
                    {
                        output = bodyPartList.bodyParts[badColorList[i]]->name ? bodyPartList.bodyParts[badColorList[i]]->name:"unknown";
                    }
                    estrConcatf(&text, "%s ", textStd("CostumeBadPartPair", badColorList[i], 
                        textStd(output)));
                }
                y += smf_ParseAndDisplay(errorText, textStd("CostumeColorErrorString", text), x, y, z, width, height, 0,0, &textAttr_Error, NULL, 0, false);
                estrDestroy(&text);
            }
            errorCount++;
        }
    }

       drawAvatarControls(xp, 710.f*yposSc, &gZoomed_in, 0 );
    zoomAvatar(false, gZoomed_in ? avatar_getZoomedInPosition(0,1) : avatar_getDefaultPosition(0,1));


    if (enableForceLoad || ((costumeStatus < COSTUME_ERROR_CAN_FORCE_LOAD)  && (costumeStatus != COSTUME_ERROR_NOERROR)))
    {
         reloadCostume = drawForceLoadSelector(xp + 308.f*xposSc,630.f*yposSc);
    }

      

    if (!notReady)
        costume_Apply(e);

    doScaling();

     if (stanceBits)
    {
        setSeqFromStanceBits(stanceBits);
    }

      if (D_MOUSEHIT == drawHybridButton(&sButtonDelete, xp+288.f*xposSc, 710.f*yposSc, 5000, 1.f, CLR_H_WHITE, (selectedIndex < 0) | HB_DRAW_BACKING) )
    {
        dialogStd( DIALOG_YES_NO, "DeleteCostumeConfirmString",NULL,NULL, deleteCurrentCostume, 0, 0 );
        sndPlay("N_Error", SOUND_GAME);
    }

    setScalingModeAndOption(SPRITE_XY_SCALE, SSO_SCALE_ALL);

      if ( drawHybridNext( DIR_LEFT_NO_NAV, 0, "CancelString" ) )
    {
        loadCostume_cancel();
        start_menu( prevMenu );
    }

    if ( drawHybridNext( DIR_RIGHT_NO_NAV,  (!game_state.editnpc) && (notReady || (costumeStatus != COSTUME_ERROR_NOERROR)), "LoadString" ) )
    {
        loadCostume_next();
    }

    if (getPCCEditingMode() > 0)
        setPCCEditingMode(getPCCEditingMode()*-1);
}
