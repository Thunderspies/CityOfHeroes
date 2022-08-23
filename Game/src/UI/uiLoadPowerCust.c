#include "uiLoadPowerCust.h"
#include "UI/uiGame.h"                //    for start_menu
#include "UI/uiUtilMenu.h"            //    for drawBackground
#include "UI/uiUtil.h"                //    for colors/others
#include <utilitieslib/language/MessageStoreUtil.h>    //    for textStd
#include "sound/sound.h"
#include "cmdparse/cmdgame.h"
#include "UI/uiCostume.h"            //    for spin button
#include "entity/power_customization.h"
#include "player/player.h"
#include "entity/entity.h"
#include <utilitieslib/utils/textparser.h>
#include <utilitieslib/components/EString.h>
#include <utilitieslib/utils/file.h>
#include <utilitieslib/utils/FolderCache.h>
#include "UI/uiPowerCust.h"
#include <utilitieslib/components/earray.h>
#include "formatter/smf_util.h"
#include "formatter/smf_main.h"
#include "UI/uiInput.h"
#include "UI/uiUtilGame.h"
#include "entity/powers.h"
#include "graphics/textureatlas.h"
#include "UI/sprite/sprite_base.h"
#include "UI/uiScrollBar.h"
#include "UI/uiClipper.h"
#include "UI/uiUtilMenu.h"
#include "graphics/ttFontUtil.h"
#include "UI/sprite/sprite_text.h"
#include "UI/sprite/sprite_font.h"
#include "entity/character_base.h"
#include "UI/uiDialog.h"
#include "UI/uiAvatar.h"
#include "UI/Hybrid/uiHybridMenu.h"

static SMFBlock SM_fileListItem;
static SMFBlock SM_powerCustListItem;
static const BasePowerSet * sSelectedPowerSet;
static const BasePower * sSelectedPower;
extern const Vec3 ZOOMED_OUT_POSITION;
extern const Vec3 ZOOMED_IN_POSITION;

typedef enum PowerCustFilerEnum
{
    PCF_NONE = 0,
    PCF_HIDE_NON_APPLICABLE = 1,
} PowerCustFilerEnum;

typedef struct PowerCustListItem
{
    char *fileName;
    __time32_t timeAge;
    PowerCustomizationList *listData;
} PowerCustListItem;
PowerCustListItem **s_powerCustList = NULL;
static PowerCustomizationList *previousPowerCust = NULL;
static TextAttribs gTextAttr =
{
    /* piBold        */  (int *)0,
    /* piItalic      */  (int *)0,
    /* piColor       */  (int *)0x000000ff,
    /* piColor2      */  (int *)0,
    /* piColorHover  */  (int *)0x000000ff,
    /* piColorSelect */  (int *)0x666666ff,
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
static bool isNullOrNone(const char *str)
{
    return !str || !str[0] || stricmp(str,"None")==0;
}
static void freeLoadPowerCustListItem(PowerCustListItem *item)
{
    if (item)
    {
        estrDestroy(&item->fileName);
        StructDestroy(ParsePowerCustomizationList, item->listData);
        free(item);
    }
}
static PowerCustListItem * allocLoadPowerCustListItem(char *displayName, __time32_t timeWrite, PowerCustomizationList *list )
{
    PowerCustListItem *returnItem = malloc(sizeof(PowerCustListItem));
    assertmsg(returnItem, "Could not alloc enough space");
    if (returnItem)
    {
        estrCreate(&returnItem->fileName);
        estrConcatCharString(&returnItem->fileName, displayName);
        returnItem->timeAge = timeWrite;
        returnItem->listData = list;
    }
    return returnItem;
}
static FileScanAction powerCustFileProcessor(char *dir, struct _finddata32_t *data)
{
    PowerCustomizationList tempPowerCust = {0};
    char filename[MAX_PATH];

    sprintf(filename, "%s/%s", dir, data->name);

    if((strEndsWith(filename, powerCustExt()) || (strEndsWith(filename, ".powerCustPrimary")) || strEndsWith(filename, ".powerCustSecondary") )&& 
        fileExists(filename)) // an actual file, not a directory
    {
        StructClear(ParsePowerCustomizationList, &tempPowerCust);
        if (ParserLoadFiles( NULL, filename, NULL, 0, ParsePowerCustomizationList, &tempPowerCust, 0, 0, NULL))
        {
            int i;

            powerCustList_initializePowerFromFullName(&tempPowerCust);
            eaQSort(tempPowerCust.powerCustomizations, comparePowerCustomizations);

            //    check to see if it exists
            for (i = 0; i < eaSize(&s_powerCustList); ++i)
            {
                if (stricmp(s_powerCustList[i]->fileName, data->name) == 0)
                {
                    break;
                }
            }
            if (i < eaSize(&s_powerCustList))
            {
                if (s_powerCustList[i]->timeAge < data->time_write)
                {
                    StructCopy(ParsePowerCustomizationList, &tempPowerCust, s_powerCustList[i]->listData, 0,0);
                    s_powerCustList[i]->timeAge = data->time_write;
                }
            }
            else
            {
                PowerCustListItem *newItem;
                PowerCustomizationList *newItemData = StructAllocRaw(sizeof(PowerCustomizationList));
                StructCopy(ParsePowerCustomizationList, &tempPowerCust, newItemData, 0,0);
                if (powerCustList_validate(newItemData, playerPtr(), 0))
                {
                    newItem = allocLoadPowerCustListItem(data->name, data->time_write, newItemData);
                    if (newItem)    
                        eaPush(&s_powerCustList, newItem);
                }
            }
        }
    }
    return FSA_NO_EXPLORE_DIRECTORY;
}
void loadPowerCustFromFiles()
{
    fileScanDirRecurseEx(powerCustPath(), powerCustFileProcessor);
}
static void reloadPowerCustFiles( const char *relPath, int reloadWhen)
{
    loadPowerCustFromFiles();
}
void initPowerCustFolderCache()
{
    char reloadpath[MAX_PATH];
    FolderCache *fc;

    sprintf(reloadpath, "%s/", powerCustPath());
    mkdirtree(reloadpath);

    fc = FolderCacheCreate();
    FolderCacheAddFolder(fc, powerCustPath(), 0);
    FolderCacheQuery(fc, NULL);
    sprintf(reloadpath, "*%s", ".powerCust");
    FolderCacheSetCallback(FOLDER_CACHE_CALLBACK_UPDATE_AND_DELETE, reloadpath, reloadPowerCustFiles);
}
static int init = 0;
static void loadPowerCust_exit(int saveChanges)
{
    Entity *e = playerPtr();
    if (saveChanges)
    {
        StructCopy(ParsePowerCustomizationList, e->powerCust, previousPowerCust, 0, 0);
    }
    powerCustList_destroy(e->powerCust);
    e->powerCust = previousPowerCust;
    previousPowerCust = NULL;
    init = 0;
    start_menu(MENU_POWER_CUST);
}

static void powerCustRevert(int index)
{
    Entity *e = playerPtr();
    if (EAINRANGE(index, previousPowerCust->powerCustomizations))        //    sanity check
    {
        StructCopy(ParsePowerCustomization, previousPowerCust->powerCustomizations[index], e->powerCust->powerCustomizations[index], 0, 0);
    }
    else
    {
        assertmsg(0, "Power Customization Application Index was out of range.");
    }
}

static void drawPowerCustomizationFileList(float x, float y, float z, float wd, float ht, int color, int bcolor, int *currentlySelected, float screenScaleX, float screenScaleY)
{
    int i;
    static ScrollBar sb;
    float endY, startY;
    CBox box;
    float screenScale = MIN(screenScaleX, screenScaleY);
    drawMenuFrame(R12, x*screenScaleX, y*screenScaleY, z, wd*screenScaleX, ht*screenScaleY, color, bcolor, FALSE);
    
    BuildCBox(&box, x*screenScaleX, (y+R10)*screenScaleY, wd*screenScaleX, (ht-(R10*2))*screenScaleY);
    clipperPushCBox(&box);
    startY = endY = y+10-sb.offset;
    //    load file list
    for (i = 0; i < eaSize(&s_powerCustList); ++i)
    {
        char *displayName;
        char *color = "white";
        CBox mouseBox;
        BuildCBox(&mouseBox, (x+PIX3)*screenScaleX, (endY+2)*screenScaleY, (wd-(2*PIX3))*screenScaleX, 18*screenScale);
        if (mouseClickHit(&mouseBox, MS_LEFT))
        {
            *currentlySelected = i;
        }
        if (i == *currentlySelected)
        {
            drawBox(&mouseBox, z-1, CLR_WHITE, SELECT_FROM_UISKIN( CLR_FRAME_BLUE, CLR_FRAME_RED, CLR_FRAME_ROGUE ) );
        }
        else if (mouseCollision(&mouseBox))
        {
            color = "white";
            drawBox(&mouseBox, z-1, 0, SELECT_FROM_UISKIN( CLR_FRAME_BLUE, CLR_FRAME_RED, CLR_FRAME_ROGUE ) );
        }
        estrCreate(&displayName);
        estrConcatf(&displayName, "<color %s>%s</color>", color, s_powerCustList[i]->fileName);
        smf_SetRawText(&SM_fileListItem, displayName, 0);
        smf_Display(&SM_fileListItem, (x+10)*screenScaleX, endY*screenScaleY, z, wd*screenScaleX, 20*screenScale, 0, 0, &gTextAttr, NULL);
        estrDestroy(&displayName);
        endY += 20;
    }
    clipperPop();
    doScrollBarEx(&sb, (ht-(4*(R12+PIX3))), (endY-startY), (x+PIX3), (y+(2*(R12+PIX3))), 5030, &box, 0, screenScaleX, screenScaleY );
}

#define GSET_MAX_WD 300
#define BULLET_WD 25
static void drawGradient(float x, float y, float z, float scale, int alpha) {
    AtlasTex* gradient = atlasLoadTexture("Gradient_28px.tga");
    display_sprite(gradient, x + PIX1, y - gradient->height*scale / 2, z, scale * 2 * BULLET_WD / gradient->width, scale*(BULLET_WD+3)/gradient->height, alpha);
}

static void drawBullet(float x, float y, float z, float scale, Color color, int alpha) {
    AtlasTex* border = atlasLoadTexture("Bullet_002_Border.tga");
    AtlasTex* base = atlasLoadTexture("Bullet_002_Fill_Base.tga");
    AtlasTex* outline = atlasLoadTexture("Bullet_002_Fill_Outline.tga");
    AtlasTex* highlight = atlasLoadTexture("Bullet_002_Fill_Highlight.tga");
    display_sprite(border, x - border->width*scale / 2, y - border->height *scale/ 2, z, scale, scale, alpha);
    display_sprite(base, x - base->width *scale/ 2, y - base->height*scale / 2, z, scale, scale, colorFlip(color).integer & alpha);
    display_sprite(outline, x - outline->width *scale/ 2, y - outline->height*scale / 2, z, scale, scale, alpha);
    display_sprite(highlight, x - highlight->width*scale / 2, y - highlight->height*scale / 2, z, scale, scale, alpha);
}

static void drawPowerCustomizationApplicableList(float x, float y, float z, float wd, float ht, int color, int bcolor, int currentlySelectedPowerSet, const BasePower **currentlySelectedPower, float screenScaleX, float screenScaleY)
{
    static ScrollBar sb;
    static int prevSelectedPowerSet = -1;
    int oneTimeOnly = (prevSelectedPowerSet != currentlySelectedPowerSet);
    Entity *e = playerPtr();
    float screenScale = MIN(screenScaleX, screenScaleY);
    prevSelectedPowerSet =currentlySelectedPowerSet;
    drawMenuFrame(R12, x*screenScaleX, y*screenScaleY, z, wd*screenScaleX, ht*screenScaleY, color, bcolor, FALSE);

    //    if the custom list is not NULL
    if (EAINRANGE(currentlySelectedPowerSet, s_powerCustList))
    {
        CBox box;
        static int flags = 0;
        float endY, startY;
        int i;
        int custIndex, setIndex, setCount = 0, classIndex, sortIndex = 0;
        PowerCustomizationList *powerCustList = s_powerCustList[currentlySelectedPowerSet]->listData;
#define BASE_POWER_SET_MAX 40
        const BasePowerSet *basePowerSets[BASE_POWER_SET_MAX]; // mild hack because I couldn't get an EArray to compile because of const shenanigans
        const BasePowerSet *tempPowerSet;

        for (custIndex = 0; custIndex < eaSize(&powerCustList->powerCustomizations); custIndex++)
        {
            if (powerCustList->powerCustomizations[custIndex]->power)
            {
                const BasePowerSet *basePowerSet = powerCustList->powerCustomizations[custIndex]->power->psetParent;
                for (setIndex = 0; setIndex < setCount; setIndex++)
                {
                    if (basePowerSets[setIndex] == basePowerSet)
                        break;
                }
                if (setIndex == setCount)
                {
                    basePowerSets[setCount] = basePowerSet;
                    setCount++;
                    if (setCount == BASE_POWER_SET_MAX)
                        break; // for custIndex
                }
            }
        }
    
        // Put primaries first
        for (setIndex = sortIndex; setIndex < setCount; setIndex++)
        {
            for (classIndex = eaSize(&g_CharacterClasses.ppClasses) - 1; classIndex >= 0; classIndex--)
            {
                if (basePowerSets[setIndex]->pcatParent == g_CharacterClasses.ppClasses[classIndex]->pcat[kCategory_Primary])
                {
                    if (setIndex != sortIndex)
                    {
                        tempPowerSet = basePowerSets[setIndex];
                        basePowerSets[setIndex] = basePowerSets[sortIndex];
                        basePowerSets[sortIndex] = tempPowerSet;
                    }
                    sortIndex++;
                    break;
                }
            }
        }

        // Put secondaries second
        for (setIndex = sortIndex; setIndex < setCount; setIndex++)
        {
            for (classIndex = eaSize(&g_CharacterClasses.ppClasses) - 1; classIndex >= 0; classIndex--)
            {
                if (basePowerSets[setIndex]->pcatParent == g_CharacterClasses.ppClasses[classIndex]->pcat[kCategory_Secondary])
                {
                    if (setIndex != sortIndex)
                    {
                        tempPowerSet = basePowerSets[setIndex];
                        basePowerSets[setIndex] = basePowerSets[sortIndex];
                        basePowerSets[sortIndex] = tempPowerSet;
                    }
                    sortIndex++;
                    break;
                }
            }
        }

        //if (drawStdButton(x+(3*wd/4), y+ht, z+5, 100, 30, CLR_RED, (flags & PCF_HIDE_NON_APPLICABLE) ? textStd("ShowInvalidString") : textStd("HideInvalidString"), 1.0, 0) == D_MOUSEHIT)
        //{
        //    if (flags & PCF_HIDE_NON_APPLICABLE)    flags = flags & (~PCF_HIDE_NON_APPLICABLE);
        //    else                                    flags = flags | PCF_HIDE_NON_APPLICABLE;
        //}

        BuildCBox(&box, x*screenScaleX, (y+PIX3+R10)*screenScaleY, wd*screenScaleX, (ht-(2*(PIX3+R10)))*screenScaleY);
        startY = endY = y+PIX3+R10-sb.offset;
        clipperPushCBox(&box);
        for (setIndex = 0; setIndex < setCount; setIndex++)
        {
            const BasePowerSet *basePowerSet = basePowerSets[setIndex];
            for (i = 0; i < eaSize(&basePowerSet->ppPowers); ++i)
            {
                const BasePower *bPower = basePowerSet->ppPowers[i];
                PowerCustomization *powerCustomization = powerCust_FindPowerCustomization(powerCustList, bPower);
                if (powerCustomization && powerCustomization->power)
                {
                    PowerCustomization *applyTarget = powerCust_FindPowerCustomization(playerPtr()->powerCust, powerCustomization->power);
                    int applicable = 0;
                    char *listString;
                    CBox mouseBox;
                    BuildCBox(&mouseBox, (x+PIX3)*screenScaleX, endY*screenScaleY, (wd-(2*PIX3))*screenScaleX, BULLET_WD*screenScaleY);
                    if (applyTarget)
                    {
                        //    check to see which powers in it are valid and applicable    
                        applicable = 1;
                        //    have a checkmark next to each selectable power
                        if (mouseCollision(&mouseBox))
                        {
                            drawBox(&mouseBox, z-1, 0, SELECT_FROM_UISKIN( CLR_FRAME_BLUE, CLR_FRAME_RED, CLR_FRAME_ROGUE ) );
                        }
                        if ((*currentlySelectedPower) == bPower)
                        {
                            drawBox(&mouseBox, z-1, CLR_WHITE, SELECT_FROM_UISKIN( CLR_FRAME_BLUE, CLR_FRAME_RED, CLR_FRAME_ROGUE ) );
                        }
                        if (mouseClickHit(&mouseBox, MS_LEFT))
                        {
                            (*currentlySelectedPower) = bPower;
                        }
                        if (oneTimeOnly)
                        {
                            StructCopy(ParsePowerCustomization, powerCustomization, applyTarget, 0, 0);
                        }
                    }
                    else
                    {
                        if (mouseClickHit(&mouseBox, MS_LEFT))
                        {
                            dialogStd( DIALOG_OK, 
                                "PowerCustCantApply", 
                                NULL, 
                                NULL, 
                                0, 
                                0, 
                                0 );
                        }
                    }
                    {
                        int alpha = applicable ? CLR_WHITE : 0xFFFFFF77;
                        estrCreate(&listString);

                        //    draw those in a list
                        estrConcatf(&listString, "<color %s>%s->%s->%s</color>", applicable ? "white" : "#77777777", textStd(bPower->psetParent->pcatParent->pchDisplayName), textStd(bPower->psetParent->pchDisplayName), textStd(bPower->pchDisplayName)/*, 
                            isNullOrNone(powerCustomization->token) ? textStd("OriginalString") : textStd(powerCustomization->token)*/);
                        smf_SetRawText(&SM_powerCustListItem, listString, 0);
                        smf_Display(&SM_powerCustListItem, ((x +5 +1)*screenScaleX)+ (2 * BULLET_WD*screenScale), endY*screenScaleY, z, wd*screenScaleX, 20*screenScale, 0, 0, &gTextAttr, NULL);

                        drawGradient((x +1+PIX3)*screenScaleX, (endY*screenScaleY) + (BULLET_WD*screenScale/2), z + 2, screenScale, alpha);

                        drawBullet(((x +1+PIX3)*screenScaleX) + BULLET_WD*screenScale / 2, (endY*screenScaleY)+(BULLET_WD*screenScale/2), z + 2, screenScale, isNullOrNone(powerCustomization->token) ? ARGBToColor(0) : powerCustomization->customTint.primary, alpha);
                        drawBullet(((x +1+PIX3)*screenScaleX) + 3 * BULLET_WD*screenScale / 2, (endY*screenScaleY)+(BULLET_WD*screenScale/2), z + 2, screenScale, isNullOrNone(powerCustomization->token) ? ARGBToColor(0) : powerCustomization->customTint.secondary, alpha);

                        //    the selected power will animate
                        estrDestroy(&listString);
                        endY += (BULLET_WD+3);
                    }
                }
            }
        }
        clipperPop();
        doScrollBarEx(&sb, (ht-(4*(R12+PIX3))), (endY-startY), (x+wd), (y+(2*(R12+PIX3))), 5030, &box, 0, screenScaleX, screenScaleY );
    }
}

static const Vec3 ZOOMED_OUT_POSITION = {0.7015, 2.6, 20.5};
extern int gZoomedInForPowers;
static void drawPowerCustomizationAvatarFrame(float x, float y, const BasePower *currentPower)
{
    int restart = (sSelectedPowerSet != (currentPower ? currentPower->psetParent : 0)
                    || sSelectedPower != currentPower);
    Vec3 avatarPos;
    F32 xp = DEFAULT_SCRN_WD;
    F32 yp = 0.f;
    F32 xposSc, yposSc;
    calculatePositionScales(&xposSc, &yposSc);
    applyPointToScreenScalingFactorf(&xp, &yp);
    sSelectedPowerSet = currentPower ? currentPower->psetParent : 0;
    sSelectedPower = currentPower;
    if (restart)
    {
        restartPowerCustomizationAnimation();
    }
    powerCust_handleAnimation();
    drawAvatarControls(x, y, &gZoomedInForPowers, 0);
    copyVec3(gZoomedInForPowers ? ZOOMED_IN_POSITION : ZOOMED_OUT_POSITION, avatarPos);
    avatarPos[0] = 1.f - (159.f*xposSc)/(xp/2.f);
    zoomAvatar(true, avatarPos);
    //    try to get the handleAnimation in here
}

static int currentlySelectedPowerSet = -1;
static const BasePower *currentlySelectedPower = 0;
static void deleteCurrentPowerCust( void * data)
{
    if (EAINRANGE(currentlySelectedPowerSet,s_powerCustList))
    {
        char fname[MAX_PATH];
        sprintf(fname, "%s/%s", powerCustPath(), s_powerCustList[currentlySelectedPowerSet]->fileName);
        freeLoadPowerCustListItem(s_powerCustList[currentlySelectedPowerSet]);
        eaRemove(&s_powerCustList, currentlySelectedPowerSet);
        remove(fname);
        currentlySelectedPowerSet = -1;
        currentlySelectedPower = 0;
    }
}
void loadPowerCust_menu()
{
    int previouslySelectedPowerSet;
    Entity *e = playerPtr();
    static HybridElement sButtonDelete = {0, "DeleteString", NULL, "icon_loadsave_delete_0"};
    F32 xposSc, yposSc, textXSc, textYSc, xp, yp;

    //commom main menu bar
    if (drawHybridCommon(HYBRID_MENU_NO_NAVIGATION))
    {
        return;
    }

    if (!init)
    {
        static int oneTimeOnly = 1;
        init = 1;
        if (oneTimeOnly)
        {
            initPowerCustFolderCache();
            loadPowerCustFromFiles();
            oneTimeOnly = 0;
        }
        smf_SetFlags(&SM_fileListItem, SMFEditMode_Unselectable, SMFLineBreakMode_None, SMFInputMode_None, 1010, SMFScrollMode_DefaultValue, SMFOutputMode_StripAllTags,
            SMFDisplayMode_AllCharacters, 0, SMAlignment_None, 0, "PowerCustLoad", 0);
        smf_SetFlags(&SM_powerCustListItem, SMFEditMode_Unselectable, SMFLineBreakMode_None, SMFInputMode_None, 1010, SMFScrollMode_DefaultValue, SMFOutputMode_StripAllTags,
            SMFDisplayMode_AllCharacters, 0, SMAlignment_None, 0, "PowerCustLoad", 0);

        if (previousPowerCust)
        {
            powerCustList_destroy(previousPowerCust);
        }

        previousPowerCust = e->powerCust;
        e->powerCust = powerCustList_clone(e->powerCust);
        sSelectedPowerSet = NULL;
        currentlySelectedPowerSet = -1;
        sSelectedPower = NULL;
        currentlySelectedPower = 0;
        restartPowerCustomizationAnimation();
    }

    setScalingModeAndOption(SPRITE_Y_UNIFORM_SCALE, SSO_SCALE_TEXTURE);
    calculatePositionScales(&xposSc, &yposSc);
    calculateSpriteScales(&textXSc, &textYSc);
    xp = DEFAULT_SCRN_WD;
    yp = 0.f;
    applyPointToScreenScalingFactorf(&xp, &yp);

    if (D_MOUSEHIT == drawHybridButton(&sButtonDelete, xp-224.f*xposSc, 710.f*yposSc, 5000, 1.f, CLR_WHITE, (currentlySelectedPowerSet < 0) | HB_DRAW_BACKING) )
    {
        dialogStd( DIALOG_YES_NO, "DeletePowerCustConfirmString", NULL, NULL, deleteCurrentPowerCust, 0, 0 );
        sndPlay("N_Error", SOUND_GAME);
    }
    drawPowerCustomizationAvatarFrame(xp-159.f*xposSc, 640.f*yposSc, currentlySelectedPower );

    //title
    setSpriteScaleMode(SPRITE_XY_NO_SCALE);
    font(&title_18);
    font_outl(0);
    font_color(CLR_WHITE, CLR_WHITE);
    prnt( 50.f*xposSc, 120.f*yposSc, ZOOM_Z, 1.3f*textXSc, 1.3f*textYSc, "LoadingPowerCust" );
    font_outl(1);

    //draw list
    previouslySelectedPowerSet = currentlySelectedPowerSet;
    gTextAttr.piScale = (int *)(int)(1.f*SMF_FONT_SCALE);
    drawPowerCustomizationFileList(37*xposSc, 135*yposSc, 5000, 290*xposSc, 530*yposSc, CLR_WHITE, SELECT_FROM_UISKIN( 0x00000088, 0x50505088, 0x50505088 ), &currentlySelectedPowerSet, 1.f, 1.f);

    //draw contents
    if (previouslySelectedPowerSet != currentlySelectedPowerSet)
    {
        int i;
        for (i = 0; i < eaSize(&previousPowerCust->powerCustomizations); ++i)
        {
            powerCustRevert(i);
        }
        currentlySelectedPower = 0;
    }

    drawPowerCustomizationApplicableList(350*xposSc, 135*yposSc, 5000, 370*xposSc, 530*yposSc, CLR_WHITE, SELECT_FROM_UISKIN( 0x00000088, 0x50505088, 0x50505088 ), currentlySelectedPowerSet, &currentlySelectedPower, 1.f, 1.f);

    setScalingModeAndOption(SPRITE_XY_SCALE, SSO_SCALE_ALL);
    if ( drawHybridNext( DIR_LEFT_NO_NAV, 0, "CancelString" ) )
    {
        loadPowerCust_exit(0);
    }

    if ( drawHybridNext( DIR_RIGHT_NO_NAV,  0, "LoadString" ) )
    {
        loadPowerCust_exit(1);
    }
}
