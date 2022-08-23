#include "UI/Hybrid/uiArchetype.h"
#include "UI/uiGame.h"                // for start_menu
#include "UI/uiClipper.h"

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
#include "player/inventory_client.h"

#include "formatter/smf_parse.h"
#include "formatter/smf_format.h"
#include "formatter/smf_main.h"

#include "UI/uiInput.h"
#include "UI/uiUtilMenu.h"
#include "UI/uiUtil.h"
#include "UI/uiUtilGame.h"
#include "UI/uiClipper.h"
#include "UI/Hybrid/uiHybridMenu.h"
#include "UI/Hybrid/uiPlaystyle.h"
#include "UI/Hybrid/uiPower.h"
#include "UI/Hybrid/uiOrigin.h"
#include "UI/Hybrid/uiRedirect.h"
#include "UI/uiDialog.h"
#include "entity/origins.h"
#include "entity/character_base.h"
#include "entity/entity.h"
#include "entity/entPlayer.h"

#include "entity/classes.h"
#include "clientcomm/authclient.h"
#include "clientcomm/dbclient.h"
#include "UI/Hybrid/uiWebStoreFrame.h"
#include "account/AccountData.h"

#define MAX_ARCHETYPES_VISIBLE 7
#define NUM_ARCHETYPE_IMAGES 3 //how many images we will cycle through - this may be dynamic later (hardcoded for now)

static HybridElement **ppClasses;
static HybridElement * selected_class ;
static const CharacterClass * prev_class ;
static SMFBlock *smDesc=0;
static int selectedClassLocked;

static void clearClassList()
{
    //free and remove hybridElements allocated for this list of classes
    eaClearEx(&ppClasses,NULL);
}

void resetArchetypeMenu()
{
    selected_class  = 0;
    prev_class = 0;
    if (smDesc)
    {
        smf_SetRawText(smDesc, "", 0);
    }
    clearClassList();
    eaDestroy(&ppClasses);
    selectedClassLocked = 0;
}

const CharacterClass *getSelectedCharacterClass()
{
    if( selected_class && !selectedClassLocked )
    {
        return selected_class->pData;
    }
    else
    {
        return 0;
    }
    
}

// validate this class with special restrictions
static int isValidClass(const CharacterClass * pClass, int inPraetoria)
{
    if (pClass)
    {
        if (inPraetoria &&
            (class_MatchesSpecialRestriction(pClass, "Kheldian") ||
             class_MatchesSpecialRestriction(pClass, "ArachnosSoldier") ||
             class_MatchesSpecialRestriction(pClass, "ArachnosWidow")))
        {
            // no epic AT's in praetoria
            return 0;
        }
        else
        {
            // check for unpublished product
            if (pClass->pchProductCode != NULL && !AccountHasStoreProductOrIsPublished(inventoryClient_GetAcctInventorySet(), skuIdFromString(pClass->pchProductCode)))
                return 0;
            else
                // currently no restrictions in primal earth    
                return 1;
        }
    }
    else
    {
        return 0;
    }
}

typedef struct ArchetypeStats
{
    char * name;
    int stats[6];

}ArchetypeStats; 

//hardcoded stats
char * statTypes[] = {"ArchetypeSurvivabilityString", "ArchetypeMeleeDamageString", "ArchetypeRangedDamageString", "ArchetypeCrowdControlString", "ArchetypeSupportString", "ArchetypePetsString"};
static ArchetypeStats aStats[] = 
{
    {"",                    {0,    2,    4,    6,    8,    10}}, //no case
    {"Class_Blaster",                {4,    8,    10,    4,    2,    2}},
    {"Class_Controller",            {4,    2,    3,    10,    7,    5}},
    {"Class_Defender",            {4,    2,    6,    4,    10,    2}},
    {"Class_Scrapper",            {7,    10,    2,    4,    2,    2}},
    {"Class_Tanker",                {10, 7,    2,    5,    3,    2}},
    {"Class_Peacebringer",    {5,    5,    5,    5,    5,    5}},
    {"Class_Warshade",    {5,    5,    5,    5,    5,    5}},
    {"Class_Brute",                {8,    9,    2,    5,    2,    2}},
    {"Class_Corruptor",            {4,    2,    7,    4,    8,    2}},
    {"Class_Dominator",            {3,    6,    6,    10,    2,    5}},
    {"Class_Mastermind",            {7,    3,    4,    3,    5,    10}},
    {"Class_Stalker",                {7,    10,    2,    4,    2,    2}},
    {"Class_Arachnos_Soldier",    {7,    6,    7,    3,    5,    3}},
    {"Class_Arachnos_Widow",    {7,    8,    5,    2,    5,    2}},
    {"Class_Primalist",    {8,    8,    2,    5,    8,    5}},
};

typedef struct ClassImages
{
    char * name;
    char * imageName[NUM_ARCHETYPE_IMAGES];
    AtlasTex * imagePtr[NUM_ARCHETYPE_IMAGES];
    const CharacterClass * pClass;
}ClassImages;

static ClassImages sImages[] =
{
    {"Class_Blaster",                {"archetypeshots_blaster_0", "archetypeshots_blaster_1", "archetypeshots_blaster_2"}},
    {"Class_Controller",            {"archetypeshots_controller_0", "archetypeshots_controller_1", "archetypeshots_controller_2"}},
    {"Class_Defender",                {"archetypeshots_defender_0", "archetypeshots_defender_1", "archetypeshots_defender_2"}},
    {"Class_Scrapper",                {"archetypeshots_scrapper_0", "archetypeshots_scrapper_1", "archetypeshots_scrapper_2"}},
    {"Class_Tanker",                {"archetypeshots_tanker_0", "archetypeshots_tanker_1", "archetypeshots_tanker_2"}},
    {"Class_Peacebringer",            {"archetypeshots_peacebringer_0", "archetypeshots_peacebringer_1", "archetypeshots_peacebringer_2"}},
    {"Class_Warshade",                {"archetypeshots_warshade_0", "archetypeshots_warshade_1", "archetypeshots_warshade_2"}},
    {"Class_Brute",                    {"archetypeshots_brute_0", "archetypeshots_brute_1", "archetypeshots_brute_2"}},
    {"Class_Corruptor",                {"archetypeshots_corruptor_0", "archetypeshots_corruptor_1", "archetypeshots_corruptor_2"}},
    {"Class_Dominator",                {"archetypeshots_dominator_0", "archetypeshots_dominator_1", "archetypeshots_dominator_2"}},
    {"Class_Mastermind",            {"archetypeshots_mastermind_0", "archetypeshots_mastermind_1", "archetypeshots_mastermind_2"}},
    {"Class_Stalker",                {"archetypeshots_stalker_0", "archetypeshots_stalker_1", "archetypeshots_stalker_2"}},
    {"Class_Arachnos_Soldier",        {"archetypeshots_arachnos_soldier_0", "archetypeshots_arachnos_soldier_1", "archetypeshots_arachnos_soldier_2"}},
    {"Class_Arachnos_Widow",        {"archetypeshots_arachnos_widow_0", "archetypeshots_arachnos_widow_1", "archetypeshots_arachnos_widow_2"}},
    {"Class_Primalist",                {"archetypeshots_primalist_0", "archetypeshots_primalist_1", "archetypeshots_primalist_2"}},
};

//indices of special cases to display "?" for their stats
#define Q_PEACEBRINGER 6
#define Q_WARSHADE 7

F32 lastStat[] = {0,0,0,0,0,0};
F32 fSpeed[] = {0,0,0,0,0,0};
ArchetypeStats * currentArchetype = aStats;

// search by name for the archetype
static void setArchetypeStats(HybridElement * pHeClass)
{
    int i, j;
    F32 boneSpeed = 0.05f;

    for (j = 0; j < 6; ++j)
    {
        currentArchetype = aStats;
        for (i = 1; i < ARRAY_SIZE(aStats); ++i)
        {
            if (strcmp(aStats[i].name, pHeClass->text) == 0)
            {
                currentArchetype = aStats + i;
                break;
            }
        }
        fSpeed[j] = (currentArchetype->stats[j]/5.f -1.f - lastStat[j]) * boneSpeed;
    }
    return;
}

//print out static stats
static void drawArchetypeStats( F32 x, F32 y, F32 z)
{
    F32 sliderWidth = 210.f;
    F32 sliderX = 275.f;
    F32 spacingY = 28.f;
    F32 textOffsetY = 6.f;
    F32 fullWd = 415.f;

    int i;
    int playSliderSound = 0;
    F32 xposSc, yposSc;
    calculatePositionScales(&xposSc, &yposSc);

    for (i = 0; i < 6; ++i)
    {
        F32 desiredStat;

        //increment
        desiredStat = currentArchetype->stats[i]/5.f-1.f;
        if (lastStat[i] != desiredStat && fSpeed[i] != 0.f)
        {
            lastStat[i] += fSpeed[i];
            if (fSpeed[i] > 0)  //positive sliding
            {
                if (lastStat[i] > desiredStat)
                {
                    lastStat[i] = desiredStat;
                }
            }
            else  //negative sliding
            {
                if (lastStat[i] < desiredStat)
                {
                    lastStat[i] = desiredStat;
                }
            }
            playSliderSound = 1;
        }

        if (currentArchetype == aStats+Q_PEACEBRINGER || currentArchetype == aStats+Q_WARSHADE)
        {
            drawHybridSlider( x, y + i*spacingY*yposSc, z, 1.f, fullWd, sliderWidth, lastStat[i], statTypes[i], -1, '?');
        }
        else
        {
            drawHybridSlider( x, y + i*spacingY*yposSc, z, 1.f, fullWd, sliderWidth, lastStat[i], statTypes[i], -1, currentArchetype->stats[i]);
        }
    }
    if (playSliderSound)
    {
        sndPlay("N_Slider_Loop", SOUND_UI_REPEATING);
    }
    else
    {
        sndStop(SOUND_UI_REPEATING, 0.f);
    }
}

static ClassImages * getClassImages(const CharacterClass * cclass)
{
    if (cclass)
    {
        int i;
        //first see if we can find the pointer
        for (i = 0; i < ARRAY_SIZE(sImages); ++i)
        {
            if (sImages[i].pClass == cclass)
            {
                return &sImages[i];
            }
        }
        //if we can't find the pointer, find the class and initialize
        for (i = 0; i < ARRAY_SIZE(sImages); ++i)
        {
            if (strcmp(sImages[i].name, cclass->pchName) == 0)
            {
                int j;
                // save pointer
                sImages[i].pClass = cclass;

                // get textures
                for(j = 0; j < NUM_ARCHETYPE_IMAGES; ++j)
                {
                    sImages[i].imagePtr[j] = atlasLoadTexture(sImages[i].imageName[j]);
                }
            }
        }
    }
    return NULL;
}

static HybridElement * createArchetypeHybridBar( const CharacterClass *pClass )
{
    HybridElement * hb = calloc(1, sizeof(HybridElement) );
    hb->text = pClass->pchName;
    hb->desc1 = pClass->pchDisplayHelp;

    hb->iconName0 = pClass->pchIcon;

    hb->pData = pClass;

    if (pClass->pchStoreRestrictions)
    {
        hb->storeLocked = !accountEval(inventoryClient_GetAcctInventorySet(), db_info.loyaltyStatus, db_info.loyaltyPointsEarned, (U32 *) db_info.auth_user_data, pClass->pchStoreRestrictions);
    }

    hybridElementInit(hb);

    return hb;
}

//recreates class list dependent upon the playstyle chosen and current origin location
//return 1: list updated
//return 0: no updating necessary
static int recreateClassList()
{
    static int                prev_startinglocation = -1;
    static HybridElement *    prev_playstyle = 0;
    HybridElement **ppClassesLocked;
    HybridElement * playstyle = g_selected_playstyle;
    int originLocation = getPraetorianChoice();
    const CharacterClass *** ppClassesNew = getCurrentClassList();
    int i;
    int change = (prev_playstyle != playstyle) || (prev_startinglocation != originLocation);

    //remember archetype selected
    const CharacterClass * pcc = selected_class ? selected_class->pData : NULL;

    if( ppClasses && change)
    {
        //clear
        clearClassList();
    }

    if (!ppClasses || change)
    {
        int updatedSelectedArchetype = 0;
        eaCreate(&ppClassesLocked);
        
        //rebuild
        for(i = 0; i < eaSize(ppClassesNew); ++i )
        {
            if (isValidClass((*ppClassesNew)[i], originLocation))
            {
                HybridElement * hb = createArchetypeHybridBar((*ppClassesNew)[i]);
                if (!hb->storeLocked)
                {
                    eaPush(&ppClasses, hb);
                }
                else
                {
                    eaPush(&ppClassesLocked, hb);
                }
                if (pcc == (*ppClassesNew)[i])
                {
                    selected_class = hb;
                    updatedSelectedArchetype = 1;
                }
            }
        }

        if (pcc && !updatedSelectedArchetype)
        {
            dialogStd( DIALOG_OK, "ArchetypeDeselectedNonexistentText", 0, 0, 0, 0, 0 );
            deselectArchetype();
        }

        //copy store locked to the end of the list
        for (i = 0; i < eaSize(&ppClassesLocked); ++i)
        {
            eaPush(&ppClasses, ppClassesLocked[i]);
        }
        //clear store locked list
        eaDestroy(&ppClassesLocked);

        prev_playstyle = playstyle;
        prev_startinglocation = originLocation;

        return 1;
    }

    return 0;
}

static void refreshClassList()
{
    int i;

    for( i = eaSize(&ppClasses)-1; i >= 0; --i )
    {
        const CharacterClass * pClass = ppClasses[i]->pData;

        //lock check
        devassert(pClass);
        if (pClass->pchStoreRestrictions)
        {
            ppClasses[i]->storeLocked = !accountEval(inventoryClient_GetAcctInventorySet(), db_info.loyaltyStatus, db_info.loyaltyPointsEarned, (U32 *) db_info.auth_user_data, pClass->pchStoreRestrictions);
        }
    }
}

void archetypeMenu()
{
    static SMFBlock *smDesc=0;
    static F32 yoffset;
    static F32 imageRotationTime;

    F32 alphasc = .5f + (getRedirectBlinkScale(BLINK_ARCHETYPE)*.5f);
    int i, bReparse = 0;
    UIBox uibox;
    CBox box;

    int drawUp = 1, drawDn = 1;
    F32 topXAdjusted;
    F32 topY = 103.f;
    F32 barWd = 400.f;
    F32 barHt = 54.f;

    F32 boneSpeed = 0.05f;
    F32 xposSc, yposSc, textXSc, textYSc, xp, yp;

    if(drawHybridCommon(HYBRID_MENU_CREATION))
        return;

    setScalingModeAndOption(SPRITE_Y_UNIFORM_SCALE, SSO_SCALE_TEXTURE);
    calculatePositionScales(&xposSc, &yposSc);
    calculateSpriteScales(&textXSc, &textYSc);
    xp = DEFAULT_SCRN_WD / 2.f;
    yp = 0.f;
    applyPointToScreenScalingFactorf(&xp, &yp);
    topXAdjusted = xp - 446.f*xposSc;
    if( !smDesc )
    {
        smDesc = smfBlock_Create();
    }

    // update class list
     if( recreateClassList() )
    {
        //reset y
        yoffset = 0.f;
    }
    else
    {
        //refresh archtype list in case something was bought
        refreshClassList();
    }

    if( eaSize(&ppClasses) > MAX_ARCHETYPES_VISIBLE ) // too many classes to fit, need arrow
    {
         AtlasTex * arrow = atlasLoadTexture( "chat_separator_arrow_down.tga" );
        int clr = 0xffffff88;
        F32 sc = 2.f;
        int onArrow = 0;

        F32 midX = topXAdjusted + (barWd/2.f-20.f)*xposSc;
        F32 topMidY = topY + barHt/2.f;

        BuildScaledCBox(&box, topXAdjusted, topY*yposSc, barWd*xposSc, MAX_ARCHETYPES_VISIBLE * barHt*yposSc);
        //check for mouse scroll
        if (mouseCollision(&box))
        {
            yoffset -= mouseScrollingScaled();
        }

        //out of bounds checks
        if (yoffset >= 0.f)
        {
            yoffset = 0.f;
            drawUp = 0;
        }
        else if (yoffset <= (MAX_ARCHETYPES_VISIBLE - eaSize(&ppClasses)) * barHt)
        {
            yoffset = (MAX_ARCHETYPES_VISIBLE - eaSize(&ppClasses)) * barHt;
            drawDn = 0;
        }

        //remove arrows if there's no where to go
        if (drawUp)
        {
            BuildScaledCBox( &box, topXAdjusted, topY*yposSc, barWd*xposSc, barHt*yposSc );
            if( mouseCollision(&box))
            {
                if (yoffset < 0.f)
                {
                    yoffset += TIMESTEP*10;
                    clr = CLR_WHITE;
                    sc = 2.1;
                    onArrow = 1;
                }
            }
            display_sprite_positional_flip( arrow, midX, topMidY*yposSc, 5, sc, sc, clr, FLIP_VERTICAL, H_ALIGN_CENTER, V_ALIGN_CENTER );
        }

        sc = 2.f;
        if (drawDn)
        {
            clr = 0xffffff88;
            BuildScaledCBox(&box, topXAdjusted, (topY+barHt*(MAX_ARCHETYPES_VISIBLE-1))*yposSc, barWd*xposSc, barHt*yposSc );
            if( mouseCollision(&box))
            {
                if (yoffset > (MAX_ARCHETYPES_VISIBLE - eaSize(&ppClasses)) * barHt)
                {
                    yoffset -= TIMESTEP*10;
                    clr = CLR_WHITE;
                    sc = 2.1;
                    onArrow = 1;
                }
            }
            display_sprite_positional( arrow, midX, (topMidY + barHt*(MAX_ARCHETYPES_VISIBLE-1))*yposSc, 5.f, sc, sc, clr, H_ALIGN_CENTER, V_ALIGN_CENTER );
        }

        if (onArrow)
        {
            sndPlay("N_Scroll_Loop", SOUND_UI_REPEATING_SCROLL);
        }
        else
        {
            sndStop(SOUND_UI_REPEATING_SCROLL, 0.f);
        }

        uiBoxDefineScaled(&uibox, 0, (topY + drawUp*barHt)*yposSc, 5000*xposSc, barHt*(MAX_ARCHETYPES_VISIBLE-drawUp-drawDn)*yposSc);
        clipperPush(&uibox);
    }
    
    // draw class list
    for( i = 0; i < eaSize(&ppClasses); ++i )
    {
        int flags = HB_TAIL_RIGHT_ROUND | HB_FLASH;
        F32 alpha = alphasc;

        //lock check
        if (ppClasses[i]->storeLocked)
        {
            const CharacterClass * pClass = ppClasses[i]->pData;

            flags |= HB_GREY;

            //draw lock
            drawHybridStoreLockIcon(topXAdjusted+40.f*xposSc, (topY + yoffset + barHt/2.f + i * barHt)*yposSc, 6.f, 1.f, CLR_WHITE);

            //draw store icon
            drawHybridStoreMenuButton(pClass->pchProductCode, topXAdjusted+325.f*xposSc, (topY + yoffset + barHt/2.f + i * barHt)*yposSc, 6.f, 1.f, CLR_WHITE, HB_STORE_PRODUCT);
        }

        //fade edges for scrolling only
        if ((eaSize(&ppClasses) > MAX_ARCHETYPES_VISIBLE) &&
            ((drawUp && (barHt * i + yoffset) < (barHt*1.5f)) ||
            (drawDn && (barHt * i + yoffset) > (barHt*(MAX_ARCHETYPES_VISIBLE-2.5f)))))
        {
            alpha *= 0.25f;
        }

         if( D_MOUSEHIT == drawHybridBar( ppClasses[i], ppClasses[i] == selected_class , topXAdjusted, (topY + yoffset + barHt/2.f + i * barHt)*yposSc, 5, 354.f, 1.f, flags, alpha, H_ALIGN_LEFT, V_ALIGN_CENTER ))
        {
            const CharacterClass * pClass = ppClasses[i]->pData;
            char archetypeSound[64] = "N_Archetype_";
            strcat(archetypeSound, pClass->pchName);
            sndPlay(archetypeSound, SOUND_UI_ALTERNATE);
            bReparse = 1;
            if( ppClasses[i] != selected_class  )
            {
                selected_class  = ppClasses[i];
                setArchetypeStats(selected_class);
                selectedClassLocked = ppClasses[i]->storeLocked;
                imageRotationTime = 0.f;
            }
        }
    }

    if( eaSize(&ppClasses) > MAX_ARCHETYPES_VISIBLE )
    {
        clipperPop();
    }

    drawHybridDescFrame(xp, 480.f*yposSc, 5.f, 950.f, 210.f, HB_DESCFRAME_FADEDOWN, 0.5f, 1.f, 0.5f, H_ALIGN_CENTER, V_ALIGN_TOP);

    if( selected_class )
    {
        // draw class picture
        AtlasTex * dsCorner = atlasLoadTexture("archetypeshots_dropshadow_corner1");
        AtlasTex * dsHoriz = atlasLoadTexture("archetypeshots_dropshadow_corner1_horizontal");
        AtlasTex * dsVert = atlasLoadTexture("archetypeshots_dropshadow_corner1_vertical");
        ClassImages * cImage = getClassImages(selected_class->pData);
        if (cImage)
        {
            int currentImageIdx, nextImageIdx, alpha;
            F32 sc = 1.f;
            F32 xAdjusted = xp - 62.f*xposSc;
            F32 y = 115.f;
            F32 z = 5.f;
            F32 dsOffset = 10.f*sc;
            F32 cHt = dsCorner->height*sc;
            F32 cWd = dsCorner->width*sc;
            F32 vHt, hWd;
            AtlasTex * imgPtr, * imgPtrNext;

            //draw image + fade animation
            imageRotationTime += TIMESTEP * 0.01f;
            imageRotationTime = fmodf(imageRotationTime, NUM_ARCHETYPE_IMAGES);
            alpha = MAX((fmodf(imageRotationTime, 1.f)-0.75f)/0.25f, 0.f) * 0xff; //transition only the last 25%
            currentImageIdx = imageRotationTime;
            nextImageIdx = (currentImageIdx+1)%NUM_ARCHETYPE_IMAGES;
            imgPtr = cImage->imagePtr[currentImageIdx];
            imgPtrNext = cImage->imagePtr[nextImageIdx];
            vHt = imgPtr->height*sc - cHt*2.f;
            hWd = imgPtr->width*sc - cWd*2.f;
            display_sprite_positional(imgPtr, xAdjusted, y*yposSc, z+0.1f, sc, sc, CLR_WHITE, H_ALIGN_LEFT, V_ALIGN_TOP );
            display_sprite_positional(imgPtrNext, xAdjusted, y*yposSc, z+0.2f, sc, sc, 0xffffff00 | alpha, H_ALIGN_LEFT, V_ALIGN_TOP );

            xAdjusted += dsOffset*xposSc;
            y += dsOffset;
            display_sprite_positional_flip(dsCorner, xAdjusted+(cWd+hWd)*xposSc, y*yposSc, z, sc, sc, CLR_BLACK, FLIP_HORIZONTAL, H_ALIGN_LEFT, V_ALIGN_TOP );
            display_sprite_positional_flip(dsVert, xAdjusted+(cWd+hWd)*xposSc, (y+cHt)*yposSc, z, sc, vHt/dsVert->height, CLR_BLACK, FLIP_HORIZONTAL, H_ALIGN_LEFT, V_ALIGN_TOP );
            display_sprite_positional_flip(dsCorner, xAdjusted+(cWd+hWd)*xposSc, (y+cHt+vHt)*yposSc, z, sc, sc, CLR_BLACK, FLIP_BOTH, H_ALIGN_LEFT, V_ALIGN_TOP );
            display_sprite_positional_flip(dsCorner, xAdjusted, (y+cHt+vHt)*yposSc, z, sc, sc, CLR_BLACK, FLIP_VERTICAL, H_ALIGN_LEFT, V_ALIGN_TOP );
            display_sprite_positional_flip(dsHoriz, xAdjusted+(cWd)*xposSc, (y+cHt+vHt)*yposSc, z, hWd/dsHoriz->width, sc, CLR_BLACK, FLIP_VERTICAL, H_ALIGN_LEFT, V_ALIGN_TOP );
        }
    
        // draw stats
        drawArchetypeStats(xp+38.f*xposSc, 525*yposSc, 5);
        drawHybridDesc(smDesc, topXAdjusted+16.f*xposSc, 520*yposSc, 5, 1.f, 450, 300, selected_class->desc1, 0, 0xff);

        // draw class text
        setSpriteScaleMode(SPRITE_XY_NO_SCALE);
        font(&title_12);
        font_outl(0);
        font_color(CLR_H_DESC_TEXT_DS, CLR_H_DESC_TEXT_DS);
        prnt(xp-450.f*xposSc+2.f, 504.f*yposSc+2.f, 5.f, textXSc, textYSc, selected_class->text);
        font_color(CLR_WHITE, CLR_WHITE);
        prnt(xp-450.f*xposSc , 504.f*yposSc, 5.f, textXSc, textYSc, selected_class->text);
        font_outl(1);
    }
    else
    {
        //default text
        setSpriteScaleMode(SPRITE_XY_NO_SCALE);
        font(&title_12);
        font_outl(0);
        font_color(CLR_H_DESC_TEXT_DS, CLR_H_DESC_TEXT_DS);
        prnt(xp-450.f*xposSc+2.f, 504.f*yposSc+2.f, 5.f, textXSc, textYSc, "ArchetypeSelectString");
        font_color(CLR_WHITE, CLR_WHITE);
        prnt(xp-450.f*xposSc , 504.f*yposSc, 5.f, textXSc, textYSc, "ArchetypeSelectString");
        font_outl(1);
    }

    setScalingModeAndOption(SPRITE_XY_SCALE, SSO_SCALE_ALL);
    drawHybridNext( DIR_LEFT, 0, "BackString" );
    
    if (game_state.editnpc)
    {
        //choose at if there isn't one
        if (!selected_class)
        {
            assert(eaSize(&ppClasses));
            selected_class = ppClasses[0];
        }
        goHybridNext(DIR_RIGHT);
    }
    else
    {
        drawHybridNext( DIR_RIGHT, 0, "NextString" );
    }

}

//gets origin name if there's only 1
const char * getOnlyAllowedOriginName(const CharacterClass * pclass)
{

    if (pclass)
    {
        if (eaSize(&pclass->pchAllowedOriginNames) == 1)
        {
            return pclass->pchAllowedOriginNames[0];
        }
    }

    return 0;
}

void archetypeMenuExitCode()
{
    const CharacterClass * cur_class = getSelectedCharacterClass();

    if (invalidOriginArchetype())
    {
        const char * validOrigin = getOnlyAllowedOriginName(cur_class);
        char buf[32];

        devassert(validOrigin);  //If there's more than one allowed origin, this logic needs to change.

        //change the origin
        setSelectedOrigin(validOrigin);

        //and popup a message to tell the player
        devassert(strlen(validOrigin) <= 10);
        sprintf(buf, "OriginChangedto%sText", validOrigin);
        dialogStd( DIALOG_OK, buf, 0, 0, 0, 0, 0 );
    }
    
    if (prev_class != cur_class)
    {
        //new archetype selected, reset powers
        if (prev_class)
        {
            resetPowerMenu();
        }

        //create new character on exit
        if (cur_class)
        {
            Entity * e = playerPtr();
            // Don't ever grant auto-issue powers on the client during creation.
            //   Let the server do it.
            character_CreatePartOne(e->pchar, e, getSelectedOrigin(), cur_class );
        }
    }
    prev_class = cur_class;
}

void deselectArchetype()
{
    selected_class  = 0;
    selectedClassLocked = 0;
    resetPowerMenu();
}
