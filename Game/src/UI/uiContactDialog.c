/***************************************************************************
 *     Copyright (c) 2003-2008, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include <stdio.h>

#include <utilitieslib/components/earray.h>
#include <utilitieslib/components/EString.h>

#include "storyarc/contactCommon.h"

#include "graphics/truetype/ttFontDraw.h"
#include "entity/entity.h"
#include "player/player.h"
#include "entity/entPlayer.h"
#include "entity/entclient.h"
#include "UI/uiUtil.h"
#include "UI/uiUtilGame.h"
#include "UI/uiWindows.h"
#include "UI/uiCursor.h"
#include "UI/uiInput.h"
#include "UI/uiStore.h"
#include "UI/uiScrollBar.h"
#include "UI/uiTree.h"
#include "UI/uiDialog.h"
#include "UI/uiComboBox.h"
#include "formatter/smf_interact.h" // calls smf_Navigate directly
#include "formatter/smf_main.h"
#include "UI/sprite/sprite_base.h"
#include "UI/sprite/sprite_text.h"
#include "UI/sprite/sprite_font.h"
#include "graphics/textureatlas.h"
#include <utilitieslib/language/MessageStoreUtil.h>
#include "entity/character_level.h"
#include "entity/character_eval.h"
#include "entity/teamCommon.h"
#include "UI/uiStatus.h"
#include "UI/uiBox.h"
#include "UI/uiToolTip.h"
#include "UI/uiClipper.h"
#include "UI/uiGame.h"
#include "graphics/ttFontUtil.h"
#include "UI/uiContactDialog.h"
#include "storyarc/playerCreatedStoryarcValidate.h"
#include "language/commonLangUtil.h"

// #define DEBUG_FLASHBACK_DISPLAY 1

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

ComboSetting limitedLivesTypes[] = 
{
    {TFDEATHS_INFINITE,            "NoLimitString",                "NoLimitString",                },
    {TFDEATHS_NONE,                "ZeroLifeString",                "ZeroLifeString",                },
    {TFDEATHS_ONE,                "OneLifeString",                "OneLifeString",                },
    {TFDEATHS_THREE,            "ThreeLifeString",                "ThreeLifeString",                },
    {TFDEATHS_FIVE,                "FiveLifeString",                "FiveLifeString",                },
    // these have been removed until they are implemented as designed
//    {TFDEATHS_ONE_PER_MEMBER,    "OneLifePerTeammateString",        "OneLifePerTeammateString",        },
//    {TFDEATHS_THREE_PER_MEMBER,    "ThreeLifePerTeammateString",    "ThreeLifePerTeammateString",    },
//    {TFDEATHS_FIVE_PER_MEMBER,    "FiveLifePerTeammateString",    "FiveLifePerTeammateString",    },
};

ComboSetting timeLimitTypes[] = 
{
    {TFTIMELIMITS_NONE,        NULL,            NULL,            },
    {TFTIMELIMITS_GOLD,        NULL,            NULL,            },
    {TFTIMELIMITS_SILVER,    NULL,            NULL,            },
    {TFTIMELIMITS_BRONZE,    NULL,            NULL,            },
};

ComboSetting powerLimitTypes[] = 
{
    {TFPOWERS_ALL,            "NoLimitString",                "NoLimitString",                },
    {TFPOWERS_NO_TRAVEL,    "NoTravelString",                "NoTravelString",                },
    {TFPOWERS_ONLY_AT,        "OnlyATString",                    "OnlyATString",                    },
    {TFPOWERS_NO_TEMP,        "NoTemporaryString",            "NoTemporaryString",            },
};

ComboSetting powerLimitTypesSL8Plus[] = 
{
    {TFPOWERS_ALL,            "NoLimitString",                "NoLimitString",                },
    {TFPOWERS_NO_TRAVEL,    "NoTravelString",                "NoTravelString",                },
    {TFPOWERS_NO_EPIC_POOL,    "NoEpicPoolString",                "NoEpicPoolString",                },
    {TFPOWERS_ONLY_AT,        "OnlyATString",                    "OnlyATString",                    },
    {TFPOWERS_NO_TEMP,        "NoTemporaryString",            "NoTemporaryString",            },
};

static char* timeLimitMessages[] =
{
    "NoLimitString",
    "TimeLimitGoldString",
    "TimeLimitSilverString",
    "TimeLimitBronzeString"
};

typedef struct ContactDialog
{
    int x;       // x position
    int y;       // y position
    int z;
    int w;       // width
    int h;       // height
    int type;

    float scale; // scale

    int parsed;
    int selected;
    int SL8Plus;

    char *txt;   // original string
    void *data;  // arbitrary data structure to pass to responseCode
    void (*responseCode)(int, void*);

    ContactResponseOption **responses; // buttons at bottom of dialog

    char        *pendingTaskforce;                // taskforce pending
    ComboBox    *comboTimeLimits;
    ComboBox    *comboLimitedLives;
    ComboBox    *comboPowerLimits;
    int            flashbackDebuff;
    int            flashbackBuff;
    int            flashbackNoEnhancements;
    int            flashbackNoInspirations;
    U32            bronzeTime;
    U32            silverTime;
    U32            goldTime;

    char**        timeLimitStrings;
} ContactDialog;

static ContactDialog *cdMaster;

StoryarcFlashbackHeader **storyarcFlashbackList = NULL;
int flashbackListRequested = false;

#define LINK_COLOR                    0x00ff00ff
#define LINK_COLOR_HIT                0xffff88ff
#define CD_BUTTON_SPACING            20
#define CD_FRAME_OFFSET                20
#define CD_CHALLENGE_TOP_OFFSET        33
#define CD_CHALLENGE_LINE_OFFSET    26
#define CD_CHALLENGE_TITLE_OFFSET    33

///////////////////////////////////////////////////////////////////////////

void ContactSendFlashbackListRequest(void);
void ContactSendFlashbackDetailRequest(char *fileID);
void ContactSendFlashbackEligibilityCheckRequest(char *fileID);
void ContactSendFlashbackAcceptRequest(char *fileID, int timeLimits, int limitedLives, int powerLimits, int debuff, int buff, int noEnhancements, int noInspirations);
void ContactSendTaskforceAcceptRequest(int timeLimits, int limitedLives, int powerLimits, int debuff, int buff, int noEnhancements, int noInspirations);

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
// Flashback Display Structure
static uiTreeNode *pAllNode = NULL;

///////////////////////////////////////////////////////////////////////////

typedef struct FlashbackSLBuckets {
    int            minLevel;
    int            maxLevel;
    char        *heroName;
    char        *villainName;
    uiTreeNode    *pNode;
} FlashbackSLBuckets;

#define BUCKET_COUNT        8
static FlashbackSLBuckets bucketsDef[BUCKET_COUNT] =
{
//    { 1,  4,    "SL1StringHero",        "SL1StringVillain",        NULL },
//    { 5,  9,    "SL2StringHero",        "SL2StringVillain",        NULL },
    { 1,  15,    "SL25StringHero",        "SL25StringVillain",    NULL },
    { 16, 19,    "SL3StringHero",        "SL3StringVillain",        NULL },
    { 20, 24,    "SL4StringHero",        "SL4StringVillain",        NULL },
    { 25, 29,    "SL5StringHero",        "SL5StringVillain",        NULL },
    { 30, 34,    "SL6StringHero",        "SL6StringVillain",        NULL },
    { 35, 39,    "SL7StringHero",        "SL7StringVillain",        NULL },
    { 40, 45,    "SL8StringHero",        "SL8StringVillain",        NULL },
    { 46, 50,    "SL9StringHero",        "SL9StringVillain",        NULL },
};

///////////////////////////////////////////////////////////////////////////

typedef struct FlashbackSADetails {
    SMFBlock                    *textFormat;
    char                        *text;
    char                        *acceptString;
    StoryarcFlashbackHeader        *pHeader;
    int                            requested;
    int                            loaded : 1;
    int                            loadFormatted : 1;
} FlashbackSADetails;

///////////////////////////////////////////////////////////////////////////
// F L A S H B A C K   T R E E
///////////////////////////////////////////////////////////////////////////

#define SPACE_BETWEEN_NODES        10.0f
#define INDENT_SPACE            10.0f

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
// H E L P E R   F U N C T I O N S

#define FORMAT_START    '<'
#define FORMAT_END        '>'
static char *stripLinks(char *pOrig)
{
    char *pFinal = NULL;
    char *pTemp = NULL;
    char *pIdx, *pLast;
    int size;

    if (pOrig == NULL)
        return NULL;

    size = strlen(pOrig) + 1;
    pTemp = malloc(size);
    memset(pTemp, 0, size);
    pLast = pOrig;

    while ((pIdx = strchr(pLast, FORMAT_START)) != NULL)
    {
        if (*(pIdx+1) == 'a' || *(pIdx+1) == 'A' || 
            (*(pIdx+1) == '/' && (*(pIdx+2) == 'a' || *(pIdx+2) == 'A')))
        {
            if (pIdx > pLast)
            {
                strncat(pTemp, pLast, (pIdx - pLast));
                pLast = pIdx + 1;
            } else {
                pLast++;
            }
            pIdx = strchr(pLast, FORMAT_END);
            if (pIdx == NULL)
                break;
            pLast = pIdx + 1;
        } else {
            pIdx = strchr(pLast, FORMAT_END);
            if (pIdx == NULL)
                break;
            strncat(pTemp, pLast, (pIdx - pLast + 1));
            pLast = pIdx + 1;
        }
    }

    size = strlen(pLast);
    if (size > 0)
        strcat(pTemp, pLast);

    size = strlen(pTemp) + 1;
    pFinal = malloc(size);
    strcpy_s(pFinal, size, pTemp);

    SAFE_FREE(pTemp);

    return (pFinal);
}


static FlashbackSLBuckets *findBucket(int level)
{
    int i;

    for (i = 0; i < BUCKET_COUNT; i++)
    {
        if (level >= bucketsDef[i].minLevel && level <= bucketsDef[i].maxLevel)
            return &(bucketsDef[i]);
    }
    return NULL;
}


static int uiContactDialogIsValidSide(StoryarcFlashbackHeader *pHeader)
{
    Entity  * e = playerPtr();

    if (!pHeader)
        return false;

    switch (pHeader->alliance)
    {
        case SA_ALLIANCE_HERO:
            return (ENT_IS_HERO(e));
            break;
        case SA_ALLIANCE_VILLAIN:
            return (ENT_IS_VILLAIN(e));
            break;
        case SA_ALLIANCE_BOTH:
        default:
            return true;
            break;
    }

    return false;

}

static int uiContactDialogIsValidLevel(StoryarcFlashbackHeader *pHeader)
{
    Entity  * e = playerPtr();
    FlashbackSLBuckets *pBucket = NULL;
        
    if (!pHeader)
        return false;

    pBucket = findBucket(pHeader->maxLevel);

    if (e && e->pchar && pBucket)
    {
        int level = character_CalcExperienceLevel(e->pchar) + 1;

        // handle strange case where level 50 can take missions in highest level bucket
        if (level == 50)
            level = 51;

        return (level > pBucket->maxLevel);
    } else {
        return false;
    }
}

static int uiContactDialogIsComplete(StoryarcFlashbackHeader *pHeader)
{
    if (pHeader)
        return pHeader->complete;
    else 
        return false;
}

static int uiContactDialogGetCost(StoryarcFlashbackHeader *pHeader)
{
    Entity  * e = playerPtr();

    if (e && e->pchar && pHeader)
    {
        int level = character_CalcExperienceLevel(e->pchar);
        FlashbackSLBuckets *pBucket = findBucket(pHeader->maxLevel);
        int cost = 0;
        
        if (pBucket)
            cost = ((.1 * level) + 3) * ((100 + (4 * level)) * level * (pBucket->maxLevel / 5));

        return cost * pHeader->costMultiplier;
    }

    return 0;
}

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

void uiContactDialogAddTaskDetail(char *pID, char *pText, char *pAccept)
{
    int count, i, j;
    FlashbackSADetails* pState = NULL;
    int found = false;

    // Search through all buckets for the storyarc
    for (i = 0; i < BUCKET_COUNT && !found; i++)
    {
        uiTreeNode *pNode = bucketsDef[i].pNode;

        if (pNode)
        {
            count = eaSize(&pNode->children);
            for (j = 0; j < count && !found; j++)
            {
                pState = (FlashbackSADetails *) pNode->children[j]->pData;

                if (pState)
                {
                    if (!stricmp(pID, pState->pHeader->fileID))
                    {
                        found = true;
                        continue;
                    }
                }
            }
        }
    }

    // Did we find it?
    if (!found)
        return;

    pState->text = stripLinks(pText);
    pState->acceptString = strdup(pAccept);
    pState->loaded = true;
    pState->loadFormatted = false;
}

void uiContactDialogSetChallengeTimeLimits(U32 bronze, U32 silver, U32 gold)
{
    int count;
    char* timestr;

    if (cdMaster)
    {
        cdMaster->bronzeTime = bronze;
        cdMaster->silverTime = silver;
        cdMaster->goldTime = gold;

        if (cdMaster->timeLimitStrings)
        {
            for (count = 0; count < ARRAY_SIZE_CHECKED(timeLimitMessages); count++)
            {
                SAFE_FREE(cdMaster->timeLimitStrings[count]);
            }
        }
        else
        {
            cdMaster->timeLimitStrings = malloc(sizeof(char*) * ARRAY_SIZE_CHECKED(timeLimitMessages));
        }

        cdMaster->timeLimitStrings[0] = strdup(textStd(timeLimitMessages[0]));
        timestr = calcTimeRemainingString(cdMaster->goldTime, true);
        cdMaster->timeLimitStrings[1] = strdup(textStd(timeLimitMessages[1], timestr));
        timestr = calcTimeRemainingString(cdMaster->silverTime, true);
        cdMaster->timeLimitStrings[2] = strdup(textStd(timeLimitMessages[2], timestr));
        timestr = calcTimeRemainingString(cdMaster->bronzeTime, true);
        cdMaster->timeLimitStrings[3] = strdup(textStd(timeLimitMessages[3], timestr));

        for (count = 0; count < ARRAY_SIZE_CHECKED(timeLimitMessages); count++)
        {
            timeLimitTypes[count].text = cdMaster->timeLimitStrings[count];
            timeLimitTypes[count].title = cdMaster->timeLimitStrings[count];
        }
    }
}

FlashbackSADetails *s_currentFlashbackState = 0;

void uiContactAdvanceToChallengeWindow()
{
    cdMaster->pendingTaskforce = s_currentFlashbackState->pHeader->fileID;
    uiContactDialogSetChallengeTimeLimits(
        s_currentFlashbackState->pHeader->bronzeTime,
        s_currentFlashbackState->pHeader->silverTime,
        s_currentFlashbackState->pHeader->goldTime);
    cdMaster->SL8Plus = 0;

    if( s_currentFlashbackState->pHeader->maxLevel >= 41 )
        cdMaster->SL8Plus = 1;

    cdMaster->type = CD_CHALLENGE_SETTINGS;

    // make sure the power limit challenge choices are
    // re-initialised
    SAFE_FREE(cdMaster->comboPowerLimits);
}

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
// C O N F I R M    D I A L O G
/*
static char *fbName;

static void cd_takeFlashback()
{
    Entity  * e = playerPtr();
    if(!fbName)
        return;

    // selected
    ContactSendFlashbackAcceptRequest(fbName);

    // shutdown window
    cd_exit();
}

*/
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

float uiTreeFlashbackDrawCallback(uiTreeNode *pNode, float x, float y, float z, float width, 
                                   float scale, int display)
{ 
    float spaceBetweenNodes = SPACE_BETWEEN_NODES;
    int textHeight;
    CBox box, parameterButtonBox, acceptButtonBox;
    int box_color = CLR_NORMAL_BACKGROUND;
    int forground_color = CLR_NORMAL_FOREGROUND;
    FlashbackSADetails *pState = (FlashbackSADetails *) pNode->pData;
    int xOffset = 15;
    char *pTitle = NULL;
    char *pTitleText = NULL;
    int reformat = false;
    AtlasTex *line = white_tex_atlas;
    int color = LINK_COLOR;
    float linkScale;
    char *pColor = "white";
    static ToolTip toolTip;


    s_taDefaults.piScale = (int*)((int)(0.85f*scale*SMF_FONT_SCALE));

    estrCreate(&pTitle);
    if (pState->pHeader->name && strlen(pState->pHeader->name) > 0)
        pTitleText = pState->pHeader->name;
    else
        pTitleText = pState->pHeader->fileID;

    // check for validity
    if (!uiContactDialogIsValidSide(pState->pHeader))
    {
#ifndef DEBUG_FLASHBACK_DISPLAY
        return 0;
#else
        pColor = "red";
#endif
    } else if (!uiContactDialogIsValidLevel(pState->pHeader)) {
#ifndef DEBUG_FLASHBACK_DISPLAY
        return 0;
#else
        pColor = "blue";
#endif
    }
    else if (uiContactDialogIsComplete(pState->pHeader))
    {
        AtlasTex * leftArrow  = atlasLoadTexture( "MissionPicker_icon_star_16x16.tga");    
        UIBox ubox;
        BuildCBox( &box, x+3.0*scale, y+3.0*scale, leftArrow->width*scale, leftArrow->height*scale );
        uiBoxFromCBox(&ubox, &box);
    
        pColor = "gold";

        display_sprite( leftArrow, x+3.0*scale, y+3.0*scale, z, scale, scale, CLR_WHITE); 

        if (clipperIntersects(&ubox) && mouseCollision(&box))
        {
            box.lowerRight.y += (-20.0f)*scale;
            box.upperLeft.y += (-20.0f)*scale;
            addToolTip( &toolTip );
            forceDisplayToolTip( &toolTip, &box, textStd("StoryArcCompleteString"), MENU_GAME, WDW_CONTACT_DIALOG );
        }
    }

    if (pNode->state & UITREENODE_EXPANDED)
        estrPrintf(&pTitle, "<color %s><b>%s</b></color>", pColor, pTitleText);
    else
        estrPrintf(&pTitle, "<color %s>%s</color>", pColor, pTitleText);

    s_taDefaults.piScale = (int*)((int)(SMF_FONT_SCALE*(scale*1.0f)));

    if (display) 
    {
        textHeight = smf_ParseAndDisplay(pState->textFormat, pTitle, x+19*scale, 
                                            y+2*scale, z, width-19*scale, 0,    
                                            false, false, &s_taDefaults, NULL, 
                                            0, true);

    } else {
        textHeight = smf_ParseAndFormat(pState->textFormat, pTitle, x+19*scale, 
                                            y+2*scale, z, width-19*scale, 0,    
                                            false, false, false, &s_taDefaults, 0);
    }
    estrDestroy(&pTitle);

    if (pNode->state & UITREENODE_EXPANDED)
    {
        if (pState->loaded)
        {
            int titleSpacing = 8;
            float buttonWidth = 200.0f;

            if (display && !pState->loadFormatted)
            {
                uiTreeRecalculateNodeSize(pNode, width, scale);
                pState->loadFormatted = true;
            }

            textHeight += titleSpacing*scale;

            if (display)  
            {
                textHeight += smf_ParseAndDisplay(pState->textFormat, pState->text, x+19*scale, 
                                                    y+titleSpacing*scale+textHeight, z, width-30*scale, 0,    
                                                    false, false, &s_taDefaults, NULL, 0, true);
            } else {
                textHeight += smf_ParseAndFormat(pState->textFormat, pState->text, x+19*scale, 
                                                    y+titleSpacing*scale+textHeight, z, width-30*scale, 0,    
                                                    false, false, false, &s_taDefaults, 0);
            }

            textHeight += titleSpacing*2*scale; 

            // special case for arcs that have requires that aren't met
            if (stricmp(pState->acceptString, "Leave") != 0)
            {
                // line
                display_sprite( line, x + 19*scale, y + textHeight, (float) z + 0.5f, 65.0f*scale, 1.5f*scale/line->height, CLR_WHITE );

                textHeight += titleSpacing*3*scale; 

                font( &game_12 );

                //////////////////////////////////////////////////////
                // accept
                BuildCBox( &acceptButtonBox, x + 19*scale, y + textHeight, 0, 0);
     
                linkScale = 1.f;
                str_dims( &game_12, linkScale*scale, linkScale*scale, false, &acceptButtonBox, pState->acceptString );
                if( mouseCollision( &acceptButtonBox ) )
                {
                    setCursor( "cursor_point.tga", NULL, false, 2, 2);
                    font_color( CLR_WHITE, LINK_COLOR_HIT );

                    if( isDown( MS_LEFT ) )
                        linkScale = .95f;

                    if( mouseClickHit( &acceptButtonBox, MS_LEFT) )
                    {
                        s_currentFlashbackState = pState;
                        ContactSendFlashbackEligibilityCheckRequest(pState->pHeader->fileID);
                    }
                }
                else
                    font_color( CLR_WHITE, LINK_COLOR );

                prnt( x + 19*scale, y + textHeight, z, linkScale*scale, linkScale*scale, pState->acceptString );
            }
            textHeight += titleSpacing*1*scale; 

        } else {
            if (pState->requested == 0)
            {
                (pState->requested)++;

                // request the detail information from the server.
                ContactSendFlashbackDetailRequest(pState->pHeader->fileID);

            } else {
                (pState->requested)++;

                // if we haven't heard back in a LOOOOOooong time, re-request the info
                if (pState->requested > 10000)
                    pState->requested = 0;
            }
 
            if (display) 
            {
                textHeight += smf_ParseAndDisplay(NULL, textStd("FlashbackLoadingData"), x+19*scale, 
                                                    y+2*scale+textHeight, z, width-19*scale, 0,    
                                                    false, false, &s_taDefaults, NULL, 0, true);
            } else {
                textHeight += smf_ParseAndFormat(NULL, textStd("FlashbackLoadingData"), x+19*scale, 
                                                    y+2*scale+textHeight, z, width-19*scale, 0,    
                                                    false, false, false, &s_taDefaults, 0);
            }
        }
    }

    if (display)
    {

        // check for mouse over
        BuildCBox( &box, x, y, width, textHeight+5*scale);
        if (mouseCollision(&box) && !mouseCollision(&parameterButtonBox) && !mouseCollision(&acceptButtonBox))
        {
            box_color = CLR_MOUSEOVER_BACKGROUND;
            forground_color = CLR_MOUSEOVER_FOREGROUND;
        }

        // draw frame
        drawFlatFrame(PIX2, R10, x, y, z-0.5, width, textHeight+5*scale, scale, forground_color, 
            box_color);

        if (pNode->state & UITREENODE_EXPANDED)
        {
            xOffset = 20;
        } else {

        }

        // check for click
        if( mouseClickHit( &box, MS_LEFT ) && !mouseClickHit( &parameterButtonBox, MS_LEFT ) && 
            !mouseClickHit( &acceptButtonBox, MS_LEFT ))
        {
            pNode->state ^= UITREENODE_EXPANDED;

            ////////////////////////////////////////////
            // if this node has changed state recalculate size
            uiTreeRecalculateNodeSize(pNode, width, scale);
        } 
    }    

    pNode->heightThisNode = textHeight + spaceBetweenNodes*scale;

    return pNode->heightThisNode;
}

void flashbackTreeFreeCallback(uiTreeNode *pNode)
{
    if (pNode->fpDraw == uiTreeFlashbackDrawCallback && pNode->pData != NULL)
    {
        FlashbackSADetails *pDisplay = (FlashbackSADetails *)pNode->pData;

        smfBlock_Destroy(pDisplay->textFormat);

        free(pNode->pData);
    }
}


static void flashbackTreeAdd(uiTreeNode *pNode, StoryarcFlashbackHeader *pHeader, Entity *e)
{
    uiTreeNode *pATNode;
    FlashbackSADetails *pDisplay = NULL;

    if (pNode == NULL)
        return;

    pDisplay = (FlashbackSADetails *) malloc(sizeof(FlashbackSADetails));
    pATNode = uiTreeNewNode();
    memset(pDisplay, 0, sizeof(FlashbackSADetails));
    pDisplay->textFormat = smfBlock_Create();
    pDisplay->loaded = false;
    pDisplay->requested = 0;
    pDisplay->pHeader = pHeader;

    // set up the node information
    pATNode->pParent = pNode;
    pATNode->fpDraw = uiTreeFlashbackDrawCallback;        
    pATNode->fpFree = flashbackTreeFreeCallback;
    pATNode->pData = (void *) pDisplay;

    eaPush(&pNode->children, pATNode);
}

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

void flashbackTreeBucketFreeCallback(uiTreeNode *pNode)
{
    SAFE_FREE((char *) pNode->pData)
}

static void contactWindowCreateFlashbackTree(int isHero)
{
    int i, count;
    uiTreeNode *pNode;
    Entity *e = playerPtr(); 
    int level = character_CalcExperienceLevel(e->pchar) + 1;
 
#ifdef DEBUG_FLASHBACK_DISPLAY
    level = 50;
#endif

    if (storyarcFlashbackList == NULL)
        return;

    if (pAllNode != NULL)
        return;

    // handle strange case where level 50 can take missions in highest level bucket
    if (level == 50)
        level = 51;

    pAllNode = uiTreeNewNode();
    pAllNode->pData = (char *) "AllString";
    pAllNode->state |= UITREENODE_EXPANDED;

    // create all buckets
    for (i = 0; i < BUCKET_COUNT; i++)
    {
        if (level > bucketsDef[i].maxLevel)
        {
            pNode = uiTreeNewNode();
            if (isHero)
                pNode->pData = strdup(printLocalizedEnt(bucketsDef[i].heroName, playerPtr()));
            else 
                pNode->pData = strdup(printLocalizedEnt(bucketsDef[i].villainName, playerPtr()));
            //        pNode->state |= UITREENODE_EXPANDED;
            bucketsDef[i].pNode = pNode;
            pNode->pParent = pAllNode;
            pNode->fpFree = flashbackTreeBucketFreeCallback;
            eaPush(&pAllNode->children, pNode);
        } else {
            bucketsDef[i].pNode = NULL;
        }
    }


    count = eaSize(&storyarcFlashbackList);
    for (i = 0; i < count; i++)
    {
        StoryarcFlashbackHeader *pHeader = storyarcFlashbackList[i];
        FlashbackSLBuckets *pBucket = findBucket(pHeader->maxLevel);

        if (!pBucket)
            pBucket = &bucketsDef[0];

        if (pBucket != NULL && pBucket->pNode != NULL)
        {
            flashbackTreeAdd(pBucket->pNode, pHeader, e);
        }
    }
}




///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////


/**********************************************************************func*
 * cd_exit
 *
 */
void cd_exit(void)
{
    int count;

    if(!cdMaster)
        return;

    if (cdMaster->timeLimitStrings)
    {
        for (count = 0; count < ARRAY_SIZE_CHECKED(timeLimitMessages); count++)
        {
            SAFE_FREE(cdMaster->timeLimitStrings[count]);
        }

        SAFE_FREE(cdMaster->timeLimitStrings);
    }

    cdMaster->responseCode(CONTACTLINK_BYE, cdMaster->data);
}

/**********************************************************************func*
 * cd_clear
 *
 */
void cd_clear(void)
{
    if(!cdMaster)
        return;

    SAFE_FREE(cdMaster->comboLimitedLives);
    SAFE_FREE(cdMaster->comboPowerLimits);
    SAFE_FREE(cdMaster->comboTimeLimits);

    if (windowUp(WDW_CONTACT_DIALOG))
    {
        // If the contact dialog is actually up, close it.
        window_setMode(WDW_CONTACT_DIALOG, WINDOW_SHRINKING);
    }
    else
    {
        // Otherwise assume the store is open, and tell the store code not to reopen the contact dialog
        storeDisableRestoreContact();
    }
}

/**********************************************************************func*
 * cd_SetBasic
 *
 */
void cd_SetBasic(CDtype type, char *body, ContactResponseOption **rep,
    void ( *fp)( int, void* ), void* voidptr)
{
    cd_clear();

    if(cdMaster==NULL)
        cdMaster = malloc(sizeof(ContactDialog));

    memset(cdMaster, 0, sizeof(ContactDialog));

    cdMaster->selected = false;
    cdMaster->type = type;

    cdMaster->responses = rep;

    cdMaster->responseCode = fp;
    cdMaster->data = voidptr;
    cdMaster->txt = body;

    if( !windowUp(WDW_STORE) && !windowUp(WDW_TITLE_SELECT) )
        window_setMode( WDW_CONTACT_DIALOG, WINDOW_GROWING );

}

/**********************************************************************func*
 * cd_DisplayButtons
 *
 */
static int cd_DisplayButtons(ContactDialog *cd, float scale)
{
    int i = eaSize( &cd->responses ) - 1;
    int y = cd->y + cd->h;
    int x = cd->x;
    int color = LINK_COLOR;
    float sc;
    AtlasTex *line = white_tex_atlas;

    font( &game_12 );

    for(; i >= 0; i-- )
    {
        CBox box = { x, y, 0, 0 };
        float boxWidth, boxHeight;

        // display left responses

        sc = 1.f;

         if( cd->type == CD_ARCHITECT )
            str_dims_notranslate( &game_12, sc*scale, sc*scale, false, &box, smf_DecodeAllCharactersGet(cd->responses[i]->responsetext) );
        else
            str_dims( &game_12, sc*scale, sc*scale, false, &box, cd->responses[i]->responsetext );

        //drawBox(&box, 0, 0xff0000ff, 0);

        if( mouseCollision( &box ) )
        {
            setCursor( "cursor_point.tga", NULL, false, 2, 2);
            font_color( CLR_WHITE, LINK_COLOR_HIT );

            if( isDown( MS_LEFT ) )
                sc = .9f;

            if( mouseClickHit( &box, MS_LEFT) )
            {
                cd->responseCode( cd->responses[i]->link, cd->data );
                cdMaster->selected = true;
            }
        }
        else
            font_color( CLR_WHITE, LINK_COLOR );

        if( cd->type == CD_ARCHITECT )
            cprntEx( x, y, cd->z, sc*scale, sc*scale, NO_MSPRINT, smf_DecodeAllCharactersGet(cd->responses[i]->responsetext) );
        else
            prnt( x, y, cd->z, sc*scale, sc*scale, cd->responses[i]->responsetext );

        // display right responses

        sc = 1.f;

        if( cd->type == CD_ARCHITECT )
            str_dims_notranslate( &game_12, sc*scale, sc*scale, false, &box, smf_DecodeAllCharactersGet(cd->responses[i]->rightResponseText) );
        else
            str_dims( &game_12, sc*scale, sc*scale, false, &box, cd->responses[i]->rightResponseText );

        boxWidth = box.lowerRight.x - box.upperLeft.x;
        boxHeight = box.lowerRight.y - box.upperLeft.y;

        box.upperLeft.x = x + cdMaster->w - boxWidth;
        box.upperLeft.y = y - boxHeight;
        box.lowerRight.x = x + cdMaster->w;
        box.lowerRight.y = y;

        //drawBox(&box, 0, 0xff0000ff, 0);

        if( mouseCollision( &box ) )
        {
            setCursor( "cursor_point.tga", NULL, false, 2, 2);
            font_color( CLR_WHITE, LINK_COLOR_HIT );

            if( isDown( MS_LEFT ) )
                sc = .9f;

            if( mouseClickHit( &box, MS_LEFT) )
            {
                cd->responseCode( cd->responses[i]->rightLink, cd->data );
                cdMaster->selected = true;
            }
        }
        else
            font_color( CLR_WHITE, LINK_COLOR );

        if( cd->type == CD_ARCHITECT )
            cprntEx( x + cdMaster->w - boxWidth, y, cd->z, sc*scale, sc*scale, NO_MSPRINT, smf_DecodeAllCharactersGet(cd->responses[i]->rightResponseText) );
        else
            prnt( x + cdMaster->w - boxWidth, y, cd->z, sc*scale, sc*scale, cd->responses[i]->rightResponseText );

        y -= CD_BUTTON_SPACING*scale;
    }

    display_sprite( line, x, y, cd->z, cd->w/line->width, 1.f*scale/line->height, CLR_WHITE );

     return cd->y+cd->h-y+CD_BUTTON_SPACING*scale;
}

/**********************************************************************func*
* cd_DisplayButtons
*
*/
static int cd_DisplayChallengeButtons(ContactDialog *cd, float scale)
{
    int i = eaSize( &cd->responses ) - 1;
    int y = cd->y + cd->h;
    int x = cd->x;
    int color = LINK_COLOR;
    float sc;
    AtlasTex *line = white_tex_atlas;
    CBox box = { x, y, 0, 0 };

    font( &game_12 );
    sc = 1.f;

    // Back
    str_dims( &game_12, sc*scale, sc*scale, false, &box, "BackString" );

    if( mouseCollision( &box ) )
    {
        setCursor( "cursor_point.tga", NULL, false, 2, 2);
        font_color( CLR_WHITE, LINK_COLOR_HIT );

        if( isDown( MS_LEFT ) )
            sc = .9f;

        if( mouseClickHit( &box, MS_LEFT) )
        {
            // is this a flashback or a taskforce?
            if (cdMaster->type == CD_CHALLENGE_SETTINGS)
            {
                cdMaster->type = CD_FLASHBACK_LIST;        
            }
            else
            {
                if (cdMaster->responseCode && cdMaster->data)
                    cdMaster->responseCode(CONTACTLINK_MAIN, cdMaster->data);
                else 
                    cd_exit();
            }
        }
    }
    else
        font_color( CLR_WHITE, LINK_COLOR );
    prnt( x, y, cd->z, sc*scale, sc*scale, "BackString" );

    y -= CD_BUTTON_SPACING*scale;

    // Help
    BuildCBox(&box, x, y, 0, 0);
    sc = 1.f;
    str_dims( &game_12, sc*scale, sc*scale, false, &box, "HelpString" );

    if( mouseCollision( &box ) )
    {
        setCursor( "cursor_point.tga", NULL, false, 2, 2);
        font_color( CLR_WHITE, LINK_COLOR_HIT );

        if( isDown( MS_LEFT ) )
            sc = .9f;

        if( mouseClickHit( &box, MS_LEFT) )
        {
            // display help dialog
            dialogStdWidth( DIALOG_OK, textStd("ChallengeSetttingsHelpString"), NULL, NULL, NULL, NULL, 0, 500 * scale );
        }
    }
    else
        font_color( CLR_WHITE, LINK_COLOR );
    prnt( x, y, cd->z, sc*scale, sc*scale, "HelpString" );

    y -= CD_BUTTON_SPACING*scale;


    BuildCBox(&box, x, y, 0, 0);
    sc = 1.f;
    str_dims( &game_12, sc*scale, sc*scale, false, &box, "AcceptChallengeSettingsString" );

    if( mouseCollision( &box ) ) 
    {
        setCursor( "cursor_point.tga", NULL, false, 2, 2);
        font_color( CLR_WHITE, LINK_COLOR_HIT );

        if( isDown( MS_LEFT ) )
            sc = .9f;

        if( mouseClickHit( &box, MS_LEFT) )
        {
            // is this a flashback or a taskforce?
            if (cdMaster->type == CD_CHALLENGE_SETTINGS)
            {
                ContactSendFlashbackAcceptRequest(cdMaster->pendingTaskforce, 
                    cdMaster->comboTimeLimits->elements[cdMaster->comboTimeLimits->currChoice]->id,
                    cdMaster->comboLimitedLives->elements[cdMaster->comboLimitedLives->currChoice]->id,
                    cdMaster->comboPowerLimits->elements[cdMaster->comboPowerLimits->currChoice]->id,
                    cdMaster->flashbackDebuff,
                    cdMaster->flashbackBuff,
                    cdMaster->flashbackNoEnhancements,
                    cdMaster->flashbackNoInspirations);        
            } 
            else 
            {
                ContactSendTaskforceAcceptRequest(
                    cdMaster->comboTimeLimits->elements[cdMaster->comboTimeLimits->currChoice]->id,
                    cdMaster->comboLimitedLives->elements[cdMaster->comboLimitedLives->currChoice]->id,
                    cdMaster->comboPowerLimits->elements[cdMaster->comboPowerLimits->currChoice]->id,
                    cdMaster->flashbackDebuff,
                    cdMaster->flashbackBuff,
                    cdMaster->flashbackNoEnhancements,
                    cdMaster->flashbackNoInspirations);        
            }
        }
    }
    else
        font_color( CLR_WHITE, LINK_COLOR );

    prnt( x, y, cd->z, sc*scale, sc*scale, "AcceptChallengeSettingsString" );

    y -= CD_BUTTON_SPACING*scale;

    display_sprite( line, x, y, cd->z, cd->w/line->width, 1.f*scale/line->height, CLR_WHITE );

    return cd->y+cd->h-y+CD_BUTTON_SPACING*scale;
}

/**********************************************************************func*
 * SelectionCallback
 *
 */
int SelectionCallback(char *pch)
{
    // Support cmd: navigation too
    if(smf_Navigate(pch))
        return 0;

    if(!cdMaster->selected)
    {
        int value = StaticDefineIntGetInt(ParseContactLink, pch);
        if (value < 0) 
            value = atoi(pch);

        cdMaster->responseCode(value, cdMaster->data);
        cdMaster->selected = true;
    }

    return 0;
}

/**********************************************************************func*
 * contactWindow
 *
 */
int contactWindow(void)
{
    static ScrollBar sb = { WDW_CONTACT_DIALOG, 0 };
    static SMFBlock *s_block;

    int color, bcolor;
    float x, y, z, w, h, scale;
    int iHeight = 0;
    int iButtonHeight;
    int i;

    float height, yoffset;
    Entity *e = playerPtr(); 

     if(!window_getDims( WDW_CONTACT_DIALOG, &x, &y, &z, &w, &h, &scale, &color, &bcolor))
    {
        sb.offset = 0;
        if (pAllNode)
        {
            uiTreeFree(pAllNode);
            pAllNode = NULL;
        }
        return 0;
    }

    if (!cdMaster) {
        window_setMode(WDW_CONTACT_DIALOG, WINDOW_DOCKED);
        return 0;
    }

    if (!s_block)
    {
        s_block = smfBlock_Create();
    }

    drawFrame(PIX3, R10, x, y, z, w, h, scale, color, bcolor);

    if(window_getMode( WDW_CONTACT_DIALOG ) == WINDOW_DISPLAYING)
    {
         cdMaster->x = x + CD_FRAME_OFFSET*scale;
        cdMaster->y = y + CD_FRAME_OFFSET*scale;
        cdMaster->z = z;
        cdMaster->w = w - CD_FRAME_OFFSET*2*scale;
        cdMaster->h = h - CD_FRAME_OFFSET*2*scale;

        set_scissor(true);
        scissor_dims(x+PIX3*scale, y+(PIX3+1)*scale, w-PIX3*2*scale, h - PIX3*2*scale);
        if (cdMaster->type == CD_CHALLENGE_SETTINGS || cdMaster->type == CD_CHALLENGE_SETTINGS_TASKFORCE)
        {
            iButtonHeight = cd_DisplayChallengeButtons(cdMaster, scale);
        } else {
            iButtonHeight = cd_DisplayButtons(cdMaster, scale);
        }

        scissor_dims(x+PIX3*scale, y+(PIX3+1)*scale, w-PIX3*2*scale, h-PIX3*3*scale-iButtonHeight);

        switch (cdMaster->type)
        {
            case CD_STANDARD:
            case CD_ARCHITECT:
            default:
                {
                     s_taDefaults.piScale = (int*)((int)(scale*SMF_FONT_SCALE));
                    iHeight = smf_ParseAndDisplay(s_block,
                        cdMaster->txt,
                        cdMaster->x, cdMaster->y - sb.offset, cdMaster->z,
                        cdMaster->w, cdMaster->h,
                        !cdMaster->parsed,
                        false,
                        &s_taDefaults, /* use global defaults for text attribs */
                        SelectionCallback,
                        0, true);
                }
                break;
            case CD_FLASHBACK_LIST:
                {
                    if (storyarcFlashbackList == NULL)
                    {
                        // request list
                        if (!flashbackListRequested)
                        {
                            ContactSendFlashbackListRequest();
                            flashbackListRequested = true;

                            // clear any UI we might have had from before
                            if (pAllNode != NULL)
                            {
                                uiTreeFree(pAllNode);
                                pAllNode = NULL;
                            }
                        }
                    } else {
                        // display list
                        if (pAllNode == NULL)
                            contactWindowCreateFlashbackTree(ENT_IS_HERO(e));

                        y += (PIX3*3*scale);

                        yoffset = y - sb.offset;
                        height = h - iButtonHeight - (PIX3*9)*scale;

                        iHeight = uiTreeDisplay(pAllNode, 0, x + PIX3*3*scale, &yoffset, y, z+1, 
                                                    w - PIX3*6*scale, &height, scale, true);

                    }
                }
                break;
            case CD_CHALLENGE_SETTINGS:
            case CD_CHALLENGE_SETTINGS_TASKFORCE:
                {
                    // power limits get refreshed every time the window is
                    // opened because the combo box contents change based on
                    // which story arc was chosen
                    if (cdMaster->comboPowerLimits == NULL)
                    {
                        cdMaster->comboPowerLimits = malloc(sizeof(ComboBox));
                        ZeroStruct(cdMaster->comboPowerLimits);
                        comboboxTitle_init( cdMaster->comboPowerLimits,        
                            100, CD_CHALLENGE_TOP_OFFSET+ R10 + 5*CD_CHALLENGE_LINE_OFFSET,            20, 400, 20, 400, WDW_CONTACT_DIALOG );

                        if (cdMaster->SL8Plus)
                        {
                            for( i = 0; i < ARRAY_SIZE(powerLimitTypesSL8Plus); i++ )
                                comboboxTitle_add( cdMaster->comboPowerLimits, 0, NULL, powerLimitTypesSL8Plus[i].text, powerLimitTypesSL8Plus[i].title, powerLimitTypesSL8Plus[i].id, CLR_WHITE,0,0 );
                        }
                        else
                        {
                            for( i = 0; i < ARRAY_SIZE(powerLimitTypes); i++ )
                                comboboxTitle_add( cdMaster->comboPowerLimits, 0, NULL, powerLimitTypes[i].text, powerLimitTypes[i].title, powerLimitTypes[i].id, CLR_WHITE,0 ,0);
                        }
                    }

                    // check for initialization
                    if (cdMaster->comboLimitedLives == NULL)
                    {
                        cdMaster->comboTimeLimits = malloc(sizeof(ComboBox));
                        cdMaster->comboLimitedLives = malloc(sizeof(ComboBox));
                        ZeroStruct(cdMaster->comboTimeLimits);
                        ZeroStruct(cdMaster->comboLimitedLives);
                        comboboxTitle_init( cdMaster->comboTimeLimits,        
                            100, CD_CHALLENGE_TOP_OFFSET+ R10 + CD_CHALLENGE_LINE_OFFSET,            20, 400, 20, 400, WDW_CONTACT_DIALOG );
                        comboboxTitle_init( cdMaster->comboLimitedLives,    
                            100, CD_CHALLENGE_TOP_OFFSET+ R10 + 3*CD_CHALLENGE_LINE_OFFSET,            20, 400, 20, 400, WDW_CONTACT_DIALOG );

                        for( i = 0; i < ARRAY_SIZE(timeLimitTypes); i++ )
                        {
                            comboboxTitle_add( cdMaster->comboTimeLimits, 0, NULL, timeLimitTypes[i].text, timeLimitTypes[i].title, timeLimitTypes[i].id, CLR_WHITE,0,0 );
                            combobox_setFlags( cdMaster->comboTimeLimits, timeLimitTypes[i].id, CCE_TITLE_NOMSPRINT | CCE_TEXT_NOMSPRINT );
                        }
                        for( i = 0; i < ARRAY_SIZE(limitedLivesTypes); i++ )
                            comboboxTitle_add( cdMaster->comboLimitedLives, 0, NULL, limitedLivesTypes[i].text, limitedLivesTypes[i].title, limitedLivesTypes[i].id, CLR_WHITE,0,0 );
                    }

                    // draw combo boxes
                    font( &game_12 );
                    font_color(0xffffffff, 0xffffffff); 
                    prnt( x+75*scale, y+(CD_CHALLENGE_TOP_OFFSET+CD_CHALLENGE_TITLE_OFFSET)*scale, z+0.1f, scale, scale, "TimeLimitString");  
                    prnt( x+75*scale, y+(CD_CHALLENGE_TOP_OFFSET+2*CD_CHALLENGE_LINE_OFFSET+CD_CHALLENGE_TITLE_OFFSET)*scale, z+0.1f, scale, scale, "LimitedLivesString");  
                    prnt( x+75*scale, y+(CD_CHALLENGE_TOP_OFFSET+4*CD_CHALLENGE_LINE_OFFSET+CD_CHALLENGE_TITLE_OFFSET)*scale, z+0.1f, scale, scale, "PowerLimitsString");  
                    combobox_display( cdMaster->comboTimeLimits );
                    combobox_display( cdMaster->comboLimitedLives );
                    combobox_display( cdMaster->comboPowerLimits );

                    // draw checkboxes
                    drawSmallCheckboxEx( WDW_CONTACT_DIALOG, 75*scale,  
                        (CD_CHALLENGE_TOP_OFFSET+6*CD_CHALLENGE_LINE_OFFSET+CD_CHALLENGE_TITLE_OFFSET)*scale, 0, scale,
                        "DebuffString", &cdMaster->flashbackDebuff, NULL, true, NULL, NULL, true, 0);
                    drawSmallCheckboxEx( WDW_CONTACT_DIALOG, 75, 
                        (CD_CHALLENGE_TOP_OFFSET+7*CD_CHALLENGE_LINE_OFFSET+CD_CHALLENGE_TITLE_OFFSET)*scale, 0, scale,
                        "BuffString", &cdMaster->flashbackBuff, NULL, true, NULL, NULL, true, 0 );
                    drawSmallCheckboxEx( WDW_CONTACT_DIALOG, 75, 
                        (CD_CHALLENGE_TOP_OFFSET+8*CD_CHALLENGE_LINE_OFFSET+CD_CHALLENGE_TITLE_OFFSET)*scale, 0,  scale,
                        "NoEnhancementsString", &cdMaster->flashbackNoEnhancements, NULL, true, NULL, NULL, true, 0 );
                    drawSmallCheckboxEx( WDW_CONTACT_DIALOG, 75, 
                        (CD_CHALLENGE_TOP_OFFSET+9*CD_CHALLENGE_LINE_OFFSET+CD_CHALLENGE_TITLE_OFFSET)*scale, 0,  scale,
                        "NoInspirationsString", &cdMaster->flashbackNoInspirations, NULL, true, NULL, NULL, true, 0 );

                    // draw title
                    font( &title_18 ); 
//                    font_color(0xaadeffff, 0x0000ffff);  
                    font_color(CLR_YELLOW, CLR_RED);
                    prnt( x+40*scale, y+(CD_CHALLENGE_TOP_OFFSET)*scale, z+0.1f, scale*1.2f, scale*1.2f, "SelectChallengeTitle");  

                    // draw buttons?

                    iHeight = 0;
                break;
                }
        }
        set_scissor(false);
        sb.pulseArrow = true;
        if (iHeight > 0)
            doScrollBar(&sb, h-(PIX3*2+R10+4)*scale-iButtonHeight, iHeight+CD_FRAME_OFFSET*scale, w, (PIX3+R10)*scale, z, 0, 0);

        cdMaster->parsed = true;
    }
    return 0;
}


/* End of File */
