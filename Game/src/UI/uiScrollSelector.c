

#include "UI/uiUtil.h"
#include "UI/uiUtilGame.h"
#include "UI/uiUtilMenu.h"
#include "uiScrollSelector.h"
#include "UI/uiClipper.h"
#include "UI/uiInput.h"
#include "UI/uiBox.h"
#include "UI/uiCostume.h"
#include "UI/uiGame.h"  
#include "UI/uiWindows.h"
#include "UI/uiScrollBar.h"
#include "UI/uidialog.h"
#include "UI/Hybrid/uiHybridMenu.h"

#include "UI/uiMissionMakerScrollSet.h"
#include "storyarc/playerCreatedStoryarcValidate.h"

#include "entity/costume.h"
#include "UI/uiTailor.h"
#include "entity/power_customization.h"
#include "UI/uiPowerCust.h"
#include "gameData/costume_data.h"
#include "player/player.h"
#include <utilitieslib/components/Earray.h>
#include "cmdparse/cmdgame.h"
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/utils/file.h>
#include "win/win_init.h"
#include "entity/entity.h"
#include "sound/sound.h"

#include "UI/sprite/sprite_text.h"
#include "UI/sprite/sprite_base.h"
#include "UI/sprite/sprite_font.h"
#include "graphics/textureatlas.h"
#include "entity/costume_client.h"
#include "graphics/ttFontUtil.h"

#include "gameComm/npc.h" // costume sets
#include "entity/powers.h" // customizable powers

//---------------------------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------------------------

enum
{
    SELECT_BACK,
    SELECT_NOCHANGE,
    SELECT_FORWARD,
    SELECT_RESET,
    SELECT_CLICK,
    SELECT_BUY,
};

enum
{
    SS_CLOSED,
    SS_OPEN,
    SS_CLOSING,
};

#define OPEN_SPEED            .5f
#define SCROLL_MIN            5
#define SCROLL_ACC            3
#define SCROLL_MAX            30
#define RESET_WD            50


typedef struct ScrollElement
{
    int idx;
    const char *displayName;

    int coh;
    int cov;
    int dev;
    int key;
    int legacy;
    int masked;
    int invalid;
    int architect;
    int back_color;

    int storeProduct;

}ScrollElement;

typedef struct ScrollSelector
{
    int mode;
    int type;

    int cur_idx;

    int count;
    int blocked_collision;
    int architect;

    float x, y, topY, z, wd, ht, offset, sc, ratio;
    float scroll_up, scroll_down;
    F32 scale;
    const void * parent;
    HybridElement hb;

    ScrollElement ** elements;

} ScrollSelector;

ScrollSelector **scrollSelectors;

//---------------------------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------------------------

static ScrollSelector * matchSS( const void * parent, int type )
{
    int i;

    if( !scrollSelectors )
        eaCreate(&scrollSelectors);

    for( i = eaSize(&scrollSelectors)-1; i>= 0; i-- )
    {
        if( parent == scrollSelectors[i]->parent &&    type == scrollSelectors[i]->type )
        {
            return scrollSelectors[i];
        }
    }

    return NULL;
}

static void addScrollElement( ScrollSelector *ss, int idx, const char * name, int coh, int cov, int dev, int legacy, int key, int masked, 
                                int invalid, int architect, int store, char * color )
{
    ScrollElement * se = calloc(1, sizeof(ScrollElement));
    se->idx = idx;
    se->displayName = name;

    se->coh = coh;
    se->cov = cov;
    se->dev = dev;
    se->key = key;
    se->legacy = legacy;
    se->masked = masked;
    se->invalid = invalid;
    se->architect = architect;
    se->back_color = 0;
    se->storeProduct = store;

    if( color )
    {
        char * color_copy = strdup( color );
        se->back_color |= atoi(strtok( color_copy, " " )) << 24; // red
        se->back_color |= atoi(strtok( NULL, " " )) << 16; // green
        se->back_color |= atoi(strtok( NULL, " " )) << 8; // blue
        se->back_color |= 0xff; // alpha
        SAFE_FREE( color_copy );
    }

    eaPush(&ss->elements, se);
    ss->count++;
}

static void changeGeoInfo( CostumeGeoSet *gset, int forward, int debug );
static void changeTexInfo( CostumeGeoSet *gset, int forward );
static void resetGeoTex( CostumeGeoSet *gset, int tex);

static const char* getThemeTranslation(const char* str, const char* suffix, cStashTable* table) {
    const char* result;

    if (!*table)
        *table = stashTableCreateWithStringKeys(30, StashDeepCopyKeys);

    if (!stashFindPointerConst(*table, str, &result)) {
        static char buf[256];
        char* in;
        char* out;
        sprintf_s(buf, sizeof(buf), "%s%s", str, suffix);
        in = out = buf;
        while (*in) {
            if (*in == ' ')
                ++in;
            else
                *out++ = *in++;
        }
        *out = '\0';
        result = strdup(buf);
        stashAddPointerConst(*table, str, result, true);
    }

    return result;
}

static const char* getPowerThemeTranslation(const char* powerTheme) {
    static cStashTable powerThemeTranslations;
    return getThemeTranslation(powerTheme, "PowerTheme", &powerThemeTranslations);
}

static const char* getPowersetThemeTranslation(const char* powerTheme) {
    static StashTable powersetThemeTranslations;
    return getThemeTranslation(powerTheme, "PowersetTheme", &powersetThemeTranslations);
}

static ScrollSelector *addSS( int type, const void * parent, F32 cx, F32 cy, F32 z, F32 wd, F32 sc, void * extra )
{
    ScrollSelector *ss;
    int i;

     if( eaSize(&scrollSelectors) )
        return NULL;

     ss = calloc(1,sizeof(ScrollSelector));

    ss->parent = parent;
    ss->type = type;
    ss->x = cx;
     ss->y = cy;
       ss->z = z+400;
     ss->wd = wd;
    ss->scale = sc;
    ss->mode = SS_OPEN;

    eaCreate(&ss->elements);
 
    switch( type )
    {
        case kScrollType_CostumeSet:
        {
            CostumeOriginSet *oset = (CostumeOriginSet*)parent;
            if(gSelectedCostumeSet < 0)
            {
                ss->cur_idx = ss->count;
                addScrollElement(ss, -1, "CustomString", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
            }
            for(i = 0; i < eaSize(&oset->costumeSets); ++i)
            {
                const CostumeSetSet *set = oset->costumeSets[i];
                if(!costumesetIsRestricted(set))
                {
                    int isStoreProduct = costumesetIsLocked(set);

                    if(gSelectedCostumeSet == i)
                        ss->cur_idx = ss->count;
                    addScrollElement(ss, i, set->displayName, 0, 0, 0, 0, eaSize(&set->keys), 0, 0, 0, isStoreProduct, 0);
                }
            }
        }break;
        case kScrollType_CustomPower:
        {
            const BasePower *power = (const BasePower*)parent;
            int i;
            Entity* e = playerPtr();
            PowerCustomization* cust = powerCust_FindPowerCustomization(e->powerCust, power);

            ss->cur_idx = ss->count;
            addScrollElement(ss, -1, "OriginalString", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

            if (cust) {
                for(i = 0; i < eaSize(&power->customfx); ++i)
                {
                    const PowerCustomFX* customfx = power->customfx[i];
                    const char* label;
                    if (cust->token && stricmp(cust->token, customfx->pchToken) == 0)
                        ss->cur_idx = ss->count;
                    label = customfx->pchDisplayName ? getPowerThemeTranslation(customfx->pchDisplayName) : 
                        getPowersetThemeTranslation(customfx->pchToken);
                    addScrollElement(ss, i, label, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );
                }
            }
        }break;
        case kScrollType_CustomPowerSet:
        {
            BasePowerSet *powerSet = (BasePowerSet*)parent;
            ss->cur_idx = 0;

            if (powerSet->iCustomToken == -2)
                addScrollElement(ss, -2, "CustomString", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

            addScrollElement(ss, -1, "OriginalString", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
            for(i = 0; i < eaSize(&powerSet->ppchCustomTokens); ++i)
            {
                if(powerSet->iCustomToken == i)
                    ss->cur_idx = ss->count;
                addScrollElement(ss, i, getPowersetThemeTranslation(powerSet->ppchCustomTokens[i]), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
            }
        }break;
        case kScrollType_Boneset:
        {
            CostumeRegionSet * reg = (CostumeRegionSet*)parent;
            for( i = 0; i < reg->boneCount; i++ )
            {
                if( !bonesetIsRestricted_SkipChecksAgainstCurrentRegionSet(reg->boneSet[i]) )
                {
                    const CostumeBoneSet * bset = reg->boneSet[i];
                    int isLegacy = bonesetIsLegacy(bset);
                    int isStoreProduct = !skuIdIsNull(SELLABLE_PRODUCT(bset));

                    if( i == reg->currentBoneSet )
                        ss->cur_idx = ss->count;
                    addScrollElement( ss, i, bset->displayName, bset->cohOnly, bset->covOnly, bset->devOnly, (isLegacy&&bset->legacy)?bset->legacy:isLegacy, eaSize(&bset->keys),  0, 0, 0, isStoreProduct, 0 );
                }
            }
        }break;
        case kScrollType_Geo:
        {
            CostumeGeoSet * gset = (CostumeGeoSet*)parent;
            int cursor = 0;
            const char *lastGeo = "";

            do 
            {
                while( cursor < gset->infoCount && partIsRestricted( gset->info[cursor]) )
                    cursor++;

                if( cursor < gset->infoCount )
                {
                    const CostumeTexSet *tset = gset->info[cursor];
                    int isLegacy = partIsLegacy(tset);
                    int isStoreProduct = !skuIdIsNull(SELLABLE_PRODUCT(tset));

                       if( stricmp(tset->geoName, lastGeo ) != 0 )
                        addScrollElement( ss, cursor, tset->geoDisplayName, tset->cohOnly, tset->covOnly, tset->devOnly, (isLegacy&&tset->legacy)?tset->legacy:isLegacy, eaSize(&tset->keys), tset->texMasked, 0, 0, isStoreProduct, 0 );
                    if( cursor == gset->currentInfo )
                        ss->cur_idx = ss->count-1;

                    lastGeo = gset->info[cursor]->geoName;
                    cursor++;
                }
            }while( cursor < gset->infoCount );
        }break;

        case kScrollType_GeoTex:
        {
            CostumeGeoSet * gset = (CostumeGeoSet*)parent;
            for( i = 0; i < gset->infoCount; i++ )
            {
                const CostumeTexSet *tset = gset->info[i];
                if( !partIsRestricted(tset) )
                {
                    int isLegacy = partIsLegacy(tset);
                    int isStoreProduct = !skuIdIsNull(SELLABLE_PRODUCT(tset));
                    
                    if( i == gset->currentInfo )
                        ss->cur_idx = ss->count;

                    addScrollElement( ss, i, tset->displayName, tset->cohOnly, tset->covOnly, tset->devOnly, (isLegacy&&tset->legacy)?tset->legacy:isLegacy, eaSize(&tset->keys), tset->texMasked, 0, 0, isStoreProduct, 0 );
                }
            }
        }break;
        case kScrollType_Tex1:
        {
            int orig, start;
            CostumeGeoSet * gset = (CostumeGeoSet*)parent;
            orig = gset->currentInfo;
            resetGeoTex( gset, 1 );
            start = gset->currentInfo;
            do
            {
                const CostumeTexSet *tset = gset->info[gset->currentInfo];
                int isLegacy = partIsLegacy(tset);
                int isStoreProduct = !skuIdIsNull(SELLABLE_PRODUCT(tset));
                if( gset->currentInfo == orig )
                    ss->cur_idx = ss->count;
                addScrollElement( ss, gset->currentInfo, tset->displayName, tset->cohOnly, tset->covOnly, tset->devOnly, (isLegacy&&tset->legacy)?tset->legacy:isLegacy, eaSize(&tset->keys), tset->texMasked, 0, 0, isStoreProduct, 0 );
                changeTexInfo( gset, 1);
            }while( gset->currentInfo != start );
            gset->currentInfo = orig;
        }break;
        case kScrollType_Tex2:
        {
            CostumeGeoSet * gset = (CostumeGeoSet*)parent;
            const CostumeTexSet *info = gset->info[gset->currentInfo];
             for( i = 0; i < gset->maskCount; i++ )
            {
                if( !maskIsRestricted(gset->mask[i]) )
                {
                    const CostumeMaskSet * mask = gset->mask[i];
                    int isStoreProduct = !skuIdIsNull(SELLABLE_PRODUCT(mask));
                    int isLegacy = maskIsLegacy(mask);

                    if( i == gset->currentMask )
                        ss->cur_idx = ss->count;

                    addScrollElement( ss, i, mask->displayName, mask->cohOnly, mask->covOnly, mask->devOnly, (isLegacy&&mask->legacy)?mask->legacy:isLegacy, eaSize(&mask->keys),  0, 0, 0, isStoreProduct, 0 );
                }    
            }
            if(info->texMasked)
            {
                int isStoreProduct = !skuIdIsNull(SELLABLE_PRODUCT(info));
                int isLegacy = partIsLegacy(info);
                addScrollElement( ss, i+1, info->displayName, info->cohOnly, info->covOnly, info->devOnly, (isLegacy&&info->legacy)?info->legacy:isLegacy, eaSize(&info->keys),  1, 0, 0, isStoreProduct, 0 );
            }

        }break;
        case kScrollType_Face:
        {
            CostumeGeoSet * gset = (CostumeGeoSet*)parent;
            for( i = 0; i < gset->faceCount; i++ )
            {
                if( i == gset->currentInfo )
                    ss->cur_idx = ss->count;
                addScrollElement( ss, i, gset->faces[i]->displayName, gset->faces[i]->cohOnly, gset->faces[i]->covOnly, 0, 0, 0, 0, 0, 0, 0, 0 );
            }
        }break;

        case kScrollType_GenericRegionSet:
            {

            }break;
        case kScrollType_GenericElementList:
            {
                MMElementList * list = (MMElementList*)parent;
                int actual_idx = list->current_element;;
                for( i = 0; i < eaSize(&list->ppElement); i++ )
                {
                    int invalid, only_level_wrong = 0, sub_has_level = 0;
                    MMElement * pElement = list->ppElement[i];

                    if( playerCreatedStoryarc_isUnlockedContent( pElement->pchText, playerPtr()) || list->bDontUnlockCheck )
                    {
                         list->current_element = i;
                        invalid = !MMScrollSet_isElementListValid(extra, list, 0, 0, &only_level_wrong, &sub_has_level);

                        if( only_level_wrong ) // if the only problem is the level
                        {
                            // try level validation assuming we choose this element
                            invalid = MMScrollSet_ValidLevel( extra, list->ppElement[list->current_element]->minLevel, list->ppElement[list->current_element]->maxLevel, 
                                        list->ppElement[actual_idx]->minLevel, list->ppElement[actual_idx]->maxLevel );
                        }

                        if( i == actual_idx )
                            ss->cur_idx = ss->count;
                        addScrollElement( ss, i, pElement->pchDisplayName, 0, 0, 0, 0, 0, 0, invalid, 1, 0, pElement->bColorable?pElement->pchText:0 );
                    }
                }
                list->current_element = actual_idx;

            }break;
        case kScrollType_GenericTextList:
            {
                GenericTextList *list = (GenericTextList *) parent;
                for( i = 0; i < list->count; i++ )
                {
                    addScrollElement( ss, i, list->textItems[i], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0  );
                }
                ss->cur_idx = list->current;
            }break;
        case kScrollType_LeftVillainGroupList:        //    intentional fallthrough
        case kScrollType_RightVillainGroupList:
            {
                MMElementList *list = (MMElementList *) parent;
                for( i = 0; i < eaSize(&list->ppElement); i++ )
                {
                    if( playerCreatedStoryarc_isUnlockedContent( list->ppElement[i]->pchText, playerPtr()) || list->bDontUnlockCheck )
                    {
                        if (i == list->current_element)
                            ss->cur_idx = ss->count;
                        addScrollElement( ss, i, list->ppElement[i]->pchDisplayName, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0 );
                    }
                }
            }break;
    }
    eaPush( &scrollSelectors, ss );
    return ss;
}

void clearSS()
{
    int i;
    for( i = 0; i < eaSize(&scrollSelectors); i++)
        scrollSelectors[i]->mode = SS_CLOSING;
}

void updateCostumeGeoSet( CostumeGeoSet *gset, int currInfo, int currMask )
{
    Entity *e = playerPtr();
    int bpID = 0;

    if( gset->bodyPartName )
    {
        bpID = costume_PartIndexFromName( e, gset->bodyPartName );
    }
    if( bpID >= 0 )
    {
        int current = gset->currentInfo;
        Costume* mutable_costume = costume_as_mutable_cast(e->costume);

        costume_PartSetPath( mutable_costume, bpID, gset->displayName, gset->bsetParent->rsetParent->name, gset->bsetParent->name );
        costume_PartSetGeometry( mutable_costume, bpID, gset->info[currInfo]->geoName);
        costume_PartSetTexture1(mutable_costume, bpID, gset->info[currInfo]->texName1);
        costume_PartSetFx( mutable_costume, bpID, gset->info[currInfo]->fxName);

        if( !gset->info[currInfo]->texName2 || gset->info[currInfo]->texMasked || stricmp( gset->info[currInfo]->texName2, "Mask" ) == 0 )
        {
            if( eaSize( &gset->mask ) )
            {
                if( currMask >= 0 )    
                    costume_PartSetTexture2( mutable_costume, bpID, gset->mask[currMask]->name );
                else
                    costume_PartSetTexture2( mutable_costume, bpID, gset->info[currInfo]->texName2 );
            }
            else if( gset->info[currInfo]->texName2 )
                costume_PartSetTexture2( mutable_costume, bpID, gset->info[currInfo]->texName2 );
        }
        else
            costume_PartSetTexture2( mutable_costume, bpID, gset->info[currInfo]->texName2 );
    }
    costume_Apply(e);
}

//---------------------------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------------------------

static float max_ht = 708.f, elem_ht = 20.f;

//pass empty string to "product" to indicate that we have a locked item, but we don't want to show the store link to a specific product
static int drawSelectorBar( HybridElement *hb, float x, float y, float z, float sc, float wd, const char * txt, int grey, 
                           int hasResetButton, ScrollSelector *ss, const char *product, F32 alpha, const char * customLockTooltip )
{
    AtlasTex * lockIcon = atlasLoadTexture("League_Icon_Lock_Closed.tga");
     int retValue = SELECT_NOCHANGE, clr = CLR_WHITE;
     AtlasTex * arrow = atlasLoadTexture( "icon_browsearrow_rotate_0" );
    F32 xposSc, yposSc;
    calculatePositionScales(&xposSc, &yposSc);

     if( ss )
        z = ss->z+10;

     if( grey )
    {
        clr = 0xaaaaaaff;    
        txt = "NoSelection";
    }

    // left arrow
    if( !ss || !ss->mode )
    {
        static HybridElement sButtonRotate0 = {0, NULL, NULL, "icon_browsearrow_rotate_0"};
        static HybridElement sButtonRotate1 = {0, NULL, NULL, "icon_browsearrow_rotate_0"};
           if( D_MOUSEHIT == drawHybridButton(&sButtonRotate0, x - (wd/2 - arrow->width*sc*0.8f/2)*xposSc, y, z+1, sc*0.8f, CLR_WHITE, HB_DRAW_BACKING | HB_FLIP_H | HB_DO_NOT_EDIT_ELEMENT) )
            retValue = SELECT_BACK;

        // right arrow
         if( D_MOUSEHIT == drawHybridButton(&sButtonRotate1, x + (wd/2 - arrow->width*sc*0.8f/2)*xposSc, y, z+1, sc*0.8f, CLR_WHITE, HB_DRAW_BACKING | HB_DO_NOT_EDIT_ELEMENT) )
            retValue = SELECT_FORWARD;
    }

    if( hasResetButton && sc > 0.f)
     {
        static HybridElement sButtonReset = {0, NULL, "ResetString", "icon_reset_0"};
        if (D_MOUSEHIT == drawHybridButton(&sButtonReset, x - (wd/2 + 20.f)*xposSc, y, z+2.f, sc, CLR_WHITE, HB_DRAW_BACKING | HB_DO_NOT_EDIT_ELEMENT))
        {
            retValue = SELECT_RESET;
        }
    }

       if( !ss || !ss->mode )
    {
        //draw locks
        if (product)
        {

            if (*product)  //draw the store icon if there's actually a product to continue to
            {
                //draw lock
                drawHybridStoreLockIcon(x-95.f*xposSc, y, z+3.f, sc, CLR_WHITE);

                //draw store icon
                drawHybridStoreMenuButton(product, x+95.f*xposSc, y, z+3.f, sc, CLR_WHITE, HB_STORE_PRODUCT);
            }
            else if (customLockTooltip)  //this selector is for a set
            {
                //draw lock
                drawHybridStoreLockIconCustomTooltip(x-95.f*xposSc, y, z+3.f, sc, CLR_WHITE, customLockTooltip);
            }

        }

        hb->text = txt;
           if( (D_MOUSEHIT == drawHybridBar( hb, 0, x, y, z, wd, sc, HB_FADE_ENDS | HB_NO_HOVER_SCALING | HB_FLAT | HB_SMALL_FONT, alpha, H_ALIGN_LEFT, V_ALIGN_CENTER )) && retValue!=SELECT_BACK && retValue!=SELECT_FORWARD )
        {
            retValue = SELECT_CLICK;
        }
    }

    if( grey )
        retValue = SELECT_NOCHANGE;

    if( retValue != SELECT_NOCHANGE )
        sndPlay( "N_HorizSelect", SOUND_GAME );

    return retValue;
}

static int drawSelectorBarMM( float x, float y, float z, float sc, float wd, char * txt, int bad_value, ScrollSelector *ss )
{
    AtlasTex * bar   = atlasLoadTexture("costume_catagory_blackbar.tga");
    AtlasTex * white = atlasLoadTexture("white.tga");
    AtlasTex * arch = atlasLoadTexture("arrowbutton");
    float w;

    int clr = CLR_MM_TEXT;
    int retValue = SELECT_NOCHANGE;

    CBox box;

    w = sc*arch->width*LEFT_COLUMN_SQUISH;

    if( ss )
        z = ss->z+10;

    // left arrow
    if( drawMMArrow( (x - wd/2) + arch->width*sc/2, y, z+1, sc, 0, 0 ))
        retValue = SELECT_BACK;

    // right arrow
    if( drawMMArrow( (x + wd/2) - arch->width*sc/2, y, z+1, sc, 1, 0 ))
        retValue = SELECT_FORWARD;

    // display the current name
    font( &game_12 );

    if( (!ss || ss->sc < 1.f) )
    {
        F32 scale = sc*LEFT_COLUMN_SQUISH;

        font_color(CLR_MM_TEXT, CLR_MM_TEXT);
        if( bad_value )
            font_color(CLR_MM_TEXT_ERROR, CLR_MM_TEXT_ERROR);

        if( str_wd( font_grp, scale, scale, txt ) > wd-arch->width )
            scale = str_sc( font_grp, wd-arch->width, txt );

        cprnt( x, y + scale*5, z+1,scale, scale, txt );
    }

    if( !ss || !ss->mode )
    {
        display_sprite( bar, x - wd/2 + w/2, y - sc*LEFT_COLUMN_SQUISH*bar->height/2, z, (wd-w)/bar->width, sc*LEFT_COLUMN_SQUISH, clr );
    }

    if (ss) 
    {
        display_sprite( white, x - wd/2 + w/2, y - sc*(bar->height - 6)/2, z, (wd-w)/white->width, (float)((bar->height - 6.0f)*sc/white->height), 0x222222cc );
    }

    // and the background bar
    BuildCBox(&box, x - wd/2 + w, y - sc*bar->height/2, (wd-2*w), bar->height*sc );

    if( !ss && mouseCollision(&box) )
    {
        CBox mouseOverBox;
        BuildCBox(&mouseOverBox, x + w -wd/2, y + 2*sc - sc*bar->height/2, (wd-2*w), bar->height*sc-(6*sc));
        drawBox(&mouseOverBox, z+1, 0, 0xffffff44);

        if( mouseClickHit(&box, MS_LEFT) )
            retValue = SELECT_CLICK;
    }

    if( retValue != SELECT_NOCHANGE )
        sndPlay( "Select", SOUND_GAME );

    return retValue;
}

static void drawScrollElement( ScrollSelector *ss, float curY, int i, int *retVal, F32 alpha, int * hovered )
{
    CBox cbox;
    ScrollElement * se = ss->elements[i];
    float z = ss->z + (ss->mode!=SS_CLOSING?11:0); 
    int alphaMask = 0xffffff00|(int)(255*alpha);
    F32 xposSc, yposSc;
    F32 textXSc = 1.f;
    F32 textYSc = 1.f;
    SpriteScalingMode ssm = getSpriteScaleMode();

    calculatePositionScales(&xposSc, &yposSc);
     font(&hybridbold_18);
    font_color( CLR_WHITE&alphaMask, CLR_WHITE&alphaMask );

       if( ss->cur_idx == i )
    {
           drawHybridFade( ss->x*xposSc, curY*yposSc, ss->z, (ss->wd-R10), elem_ht*ss->scale, 60, 0xffaa4499&alphaMask );
    }

    if (se->storeProduct)
    {
        //draw lock
        drawHybridStoreLockIcon((ss->x - 100.f)*xposSc, curY*yposSc, z+10.f, ss->scale*0.7f, CLR_WHITE);
    }

      BuildScaledCBox( &cbox, (ss->x - ss->wd/2 + R10)*xposSc, (curY-elem_ht*ss->scale/2)*yposSc, (ss->wd-2*R10)*xposSc, (elem_ht*ss->scale-1)*yposSc );
     //drawBox(&cbox, 5000, CLR_YELLOW, 0);
    if(mouseCollision(&cbox))
    {
        *hovered = i;
        drawHybridFade( ss->x*xposSc, curY*yposSc, ss->z, ss->wd, elem_ht*ss->scale, 0, 0xffffff44&alphaMask );

        if( ss->sc >= 1.f )
        {
            if( mouseUp(MS_LEFT) )
            {
                sndPlay("N_SelectSmall", SOUND_GAME);
                *retVal = i;
            }

            if( ss->type == kScrollType_Geo ||
                ss->type == kScrollType_GeoTex ||
                ss->type == kScrollType_Tex1 )
            {
                CostumeGeoSet *gset = (CostumeGeoSet *)ss->parent;
                updateCostumeGeoSet( gset, se->idx, gset->currentMask );
            }

            if( ss->type == kScrollType_Tex2 )
            {
                if( se->masked )
                {
                    CostumeGeoSet *gset = (CostumeGeoSet *)ss->parent;
                    updateCostumeGeoSet( gset, gset->currentInfo, -1);
                }
                else
                {
                    CostumeGeoSet *gset = (CostumeGeoSet *)ss->parent;
                    updateCostumeGeoSet( gset, gset->currentInfo, se->idx);
                }
            }
        }

        collisions_off_for_rest_of_frame = true;
    }

    if( game_state.editnpc )
    {
        if( se->coh )
            drawBox( &cbox, z, 0, 0x0000ff88&alphaMask );
         else if( se->cov )
            drawBox( &cbox, z, 0, 0xff000088&alphaMask );
        else
            drawBox( &cbox, z, 0, 0x00ff0088&alphaMask );

        BuildScaledCBox( &cbox, ss->x*xposSc,( (curY-elem_ht*ss->scale/2)*yposSc ), (ss->wd-2*R10)*xposSc/2, (elem_ht*ss->scale-1)*yposSc );

        if( se->key )
            drawBox( &cbox, z, 0, 0xff00ffff&alphaMask );
        if( se->legacy == 1)
            drawBox( &cbox, z, 0, 0xffff00ff&alphaMask );
        if( se->legacy == 2)
            drawBox( &cbox, z, 0, 0x666600ff&alphaMask );
        if( se->legacy > 2)
            drawBox( &cbox, z, 0, 0x333300ff&alphaMask );
        if( se->dev )
            drawBox( &cbox, z, 0, 0x888888ff&alphaMask );
        if( se->masked )
            drawBox( &cbox, z, 0, 0xffffffff&alphaMask );
    }

     if( se->back_color )
    {
        AtlasTex * white = atlasLoadTexture("white");
        display_sprite_positional_blend(white, (ss->x - ss->wd/2 + R10)*xposSc, (curY-elem_ht/2)*yposSc, z, (ss->wd-2*R10)/white->width, (elem_ht*ss->scale-1)/white->height, se->back_color&alphaMask, 0, 0, se->back_color&alphaMask, H_ALIGN_LEFT, V_ALIGN_TOP );
    }

      if( se->invalid )
        font_color( CLR_RED&alphaMask, CLR_RED&alphaMask );

    if (isTextScalingRequired())
    {
        calculateSpriteScales(&textXSc, &textYSc);
        setSpriteScaleMode(SPRITE_XY_NO_SCALE);
    }
    cprntEx( ss->x*xposSc, (curY+1)*yposSc, z, ss->scale*textXSc, ss->scale*textYSc, CENTER_X|CENTER_Y, se->displayName);
    setSpriteScaleMode(ssm);
}


static void drawScrollElementMM( ScrollSelector *ss, float curY, int i, int *retVal )
{
    CBox cbox;
    ScrollElement * se = ss->elements[i];
    float z = ss->z + (ss->mode!=SS_CLOSING?11:0); 

    if( ss->cur_idx == i )
    {
        AtlasTex * white = atlasLoadTexture("white.tga");
        font_color( CLR_WHITE, 0x00ff44ff );

        if( ss->offset )
            drawFlatFrame( PIX2, R6, (ss->x - ss->wd/2 + R10/2), (curY-elem_ht/2), ss->z, (ss->wd-R10), elem_ht, 1.f, 0x222222ff, 0x222222ff );
    }
    else
        font_color( CLR_WHITE, CLR_WHITE );

    BuildCBox( &cbox, (ss->x - ss->wd/2 + R10), (curY-elem_ht/2), (ss->wd-2*R10), (elem_ht-1) );
    if(mouseCollision(&cbox))
    {
        CBox selectionCBox;
        BuildCBox(&selectionCBox, ((ss->x - ss->wd/2) + PIX3), (curY-elem_ht/2), (ss->wd-(2*PIX3)), (elem_ht-1) );
        drawBox( &selectionCBox, z, 0, 0xffffff44 );

        if( ss->sc >= 1.f )
        {
            if( mouseUp(MS_LEFT) )
                *retVal = i;
        }

        collisions_off_for_rest_of_frame = true;
    }

    if( se->back_color )
    {
        AtlasTex * white = atlasLoadTexture("white");
        display_sprite_blend(white, (ss->x - ss->wd/2 + R10), (curY-elem_ht/2), z, (ss->wd-2*R10)/white->width, (elem_ht-1)/white->height, se->back_color, 0, 0, se->back_color );
    }

    if (se->storeProduct)
    {
        AtlasTex * lock = atlasLoadTexture("League_Icon_Lock_Closed.tga");

        drawFlatFrame( PIX2, R6, (ss->x - ss->wd/2 + R10 - 5), (curY-elem_ht/2+1.5f), z, (ss->wd-1*R10), (elem_ht-3), 1.f, 0xff555544, 0xff555544 );
        display_sprite(lock, (ss->x + ss->wd/2) - (1.75*lock->width - 9.0f), (curY+1.5f) -((float)lock->height*0.8)/2, z+10, (float)lock->width*0.8/(float)lock->width, (float)lock->height*0.8/(float)lock->height, 0xffffffff );
    }

    if( se->invalid > 1 )
        font_color( CLR_MM_BUTTON_ERROR_TEXT_LIT, CLR_MM_BUTTON_ERROR_TEXT_LIT );//CLR_MM_TEXT_ERROR_LIT, CLR_MM_TEXT_ERROR_LIT );
    else if( se->invalid)
        font_color( CLR_MM_TEXT_ERROR, CLR_MM_TEXT_ERROR );
    else
        font_color( CLR_MM_TEXT, CLR_MM_TEXT );

    cprnt( ss->x, (curY + 5*LEFT_COLUMN_SQUISH), z, LEFT_COLUMN_SQUISH, LEFT_COLUMN_SQUISH, se->displayName );
}
static int drawScrollSelector( ScrollSelector *ss )
{
    AtlasTex * arrow = atlasLoadTexture( "icon_browsearrow_0" );
    int retVal = -1, i;
    int drawn =0;
    float diff, offset = 0.f;
    UIBoxAlterDirection shrink_dir = UIBAD_NONE;
    UIBox box;
    CBox cbox;
    int scrollUp = 0;
    int scrollDn = 0;
    static int lastHoveredNode = -1;
    int hovered = -1;
    F32 xposSc, yposSc;
    calculatePositionScales(&xposSc, &yposSc);

     if(ss->mode == SS_CLOSED )
        return 0;

    clipperPush(0);

     if( ss->blocked_collision == true )
        collisions_off_for_rest_of_frame = false;

    uiBoxDefine( &box, 0, ss->topY, 10000, (ss->ht*ss->sc-2*PIX3) );

    // off the top
    diff = (ss->cur_idx+1)*elem_ht*ss->scale - (ss->y-ss->topY);
    if(ss->sc >= 1.f && ss->offset < diff-1)
    {
        int clr = 0xffffff44;
        float arrow_sc = 1.f;
        float curY = ss->topY;
           BuildScaledCBox( &cbox, (ss->x - ss->wd/2)*xposSc, curY*yposSc, ss->wd*xposSc, elem_ht*ss->scale*yposSc );
        if( mouseCollision(&cbox) && ss->offset < diff  )
        {
            drawHybridFade( ss->x*xposSc, (curY + elem_ht*ss->scale/2.f)*yposSc, ss->z, ss->wd, elem_ht*ss->scale, 0, 0xffffff44 );
            offset += TIMESTEP*(ss->scroll_up*(SCROLL_MAX-SCROLL_MIN)+SCROLL_MIN);
            if( ss->offset+offset > diff )
                offset = diff - ss->offset;
            ss->scroll_up += TIMESTEP/30.f/SCROLL_ACC;
            if(ss->scroll_up > 1.f)
                ss->scroll_up = 1.f;

            clr = 0x88ff8888;
            arrow_sc *= 1.1f;
        }
        else
        {
            ss->scroll_up = 0;
        }

         display_sprite_positional( arrow, ss->x*xposSc, curY*yposSc, ss->z, arrow_sc, elem_ht*ss->scale/arrow->height, CLR_WHITE, H_ALIGN_CENTER, V_ALIGN_TOP );
        shrink_dir |= UIBAD_TOP;
        scrollUp = 1;
    }

    // off the bottom
    diff = (ss->count-ss->cur_idx)*elem_ht*ss->scale - (ss->ht-(ss->y-ss->topY));
    if(ss->sc >= 1.f && ss->offset > -diff)
    {
        // bottom arrow
         int clr = 0xffffff44;
        float arrow_sc = 1.f;
           float curY = ss->topY + ss->ht - elem_ht*ss->scale;
          BuildScaledCBox( &cbox, (ss->x - ss->wd/2)*xposSc, curY*yposSc, ss->wd*xposSc, elem_ht*ss->scale*yposSc );
         if( mouseCollision(&cbox) && ss->offset > -diff )
        {
            drawHybridFade( ss->x*xposSc, (curY + elem_ht*ss->scale/2.f)*yposSc, ss->z, ss->wd, elem_ht*ss->scale, 0, 0xffffff44 );
            offset -= TIMESTEP*(ss->scroll_down*(SCROLL_MAX-SCROLL_MIN)+SCROLL_MIN);
            if( ss->offset+offset < -diff )
                offset = -diff - ss->offset;
            ss->scroll_down += TIMESTEP/30.f/SCROLL_ACC;
            if(ss->scroll_down > 1.f)
                ss->scroll_down = 1.f;

            if (ss->architect)
                clr = CLR_MM_SS_ARROW;
            else
                clr = 0x88ff8888;
            arrow_sc *= 1.1f;
        }
        else
        {
            ss->scroll_down = 0;
        }

          display_sprite_positional_flip( arrow, ss->x*xposSc, curY*yposSc, ss->z, arrow_sc, elem_ht*ss->scale/arrow->height, CLR_WHITE, FLIP_VERTICAL, H_ALIGN_CENTER, V_ALIGN_TOP );
        shrink_dir |= UIBAD_BOTTOM;
        scrollDn = 1;
    }

    BuildScaledCBox(&cbox, (ss->x - ss->wd/2.f)*xposSc, ss->topY*yposSc, ss->wd*xposSc, (ss->ht*ss->sc-2*PIX3)*yposSc);
    if (mouseCollision(&cbox))
    {
        F32 mouseScroll = -mouseScrollingScaled();
        if ((mouseScroll > 0 && scrollUp) || (mouseScroll < 0 && scrollDn))
        {
            offset += mouseScroll;
        }
    }

    // make room for the arrows
    if(shrink_dir != UIBAD_NONE)
    {
         uiBoxAlter( &box, UIBAT_SHRINK, shrink_dir, elem_ht*ss->scale );
    }

    clipperPush(&box);
      for( i = 0; i < ss->count; i++ )
    {
         if(ss->elements[i]->legacy <=game_state.editnpc)
        {
             F32 cy = ss->y + (drawn-ss->cur_idx)*elem_ht*ss->scale + ss->offset;
             F32 alpha = 1.f;

            // The 10 is just a little fudge to make the fade look nicer
               if( cy < (box.y + (elem_ht*ss->scale)*3/2 - 10) && (shrink_dir&UIBAD_TOP) )
                alpha = MAX( 0, (cy - (box.y + (elem_ht*ss->scale)/2) + 10 )/ (elem_ht*ss->scale));
              if( cy > ((box.y+box.height) - (elem_ht*ss->scale)*3/2 + 10)  && (shrink_dir&UIBAD_BOTTOM) )
                alpha = MAX( 0, ( (box.y+box.height) - (elem_ht*ss->scale)/2 - cy + 10)/(elem_ht*ss->scale));  
        
            drawScrollElement( ss, cy, i, &retVal, alpha, &hovered );
            drawn++;
        }
    }
    clipperPop(&box);

    if (lastHoveredNode != hovered)
    {
        lastHoveredNode = hovered;
        if (hovered > -1)
        {
            //new hovered node, play sound
            sndPlay("N_PopHover", SOUND_GAME);
        }
    }

     BuildScaledCBox( &cbox, (ss->x - ss->wd/2)*xposSc, ss->topY*yposSc, ss->wd*xposSc, ss->ht*yposSc );
     if( mouseCollision(&cbox) )
        collisions_off_for_rest_of_frame = true;
       else if( mouseUp(MS_LEFT) || mouseDown(MS_LEFT) || mouseDragging() ) 
    {
        if( ss->type < kScrollType_GenericRegionSet )
            updateCostume();
        ss->mode = SS_CLOSING;
        if(mouseUp(MS_LEFT))
            collisions_off_for_rest_of_frame = true;
    }

    if( retVal >= 0 )
    {
        if( ss->type < kScrollType_GenericRegionSet )
            updateCostume();
        ss->mode = SS_CLOSING;
        collisions_off_for_rest_of_frame = true;
    }
    clipperPop();
    ss->offset += offset;
    return retVal;
}



static int drawScrollSelectorMM( ScrollSelector *ss )
{
    int retVal = -1, i;
    int drawn =0;
    AtlasTex * up = atlasLoadTexture( "Conning_menuexpander_arrow_up.tga" );
    AtlasTex * down = atlasLoadTexture( "Conning_menuexpander_arrow_down.tga" );
    float diff, offset = 0.f;
    UIBoxAlterDirection shrink_dir = UIBAD_NONE;
    UIBox box;
    CBox cbox;

    if(ss->mode == SS_CLOSED )
        return 0;

    clipperPush(0);
    if( ss->blocked_collision == true )
        collisions_off_for_rest_of_frame = false;

    uiBoxDefine( &box, 0, ss->topY, 10000, (ss->ht*ss->sc-2*PIX3) );

    // off the top
    diff = (ss->cur_idx+1)*elem_ht - (ss->y-ss->topY);
    if(ss->sc >= 1.f && ss->offset < diff-1)
    {
        int clr = 0xffffff44;
        float arrow_sc = 1.f;
        float curY = ss->topY + elem_ht/2;
        BuildCBox( &cbox, (ss->x - ss->wd/2 + R10), curY, (ss->wd-2*R10), (elem_ht-1) );
        if( mouseCollision(&cbox) && ss->offset < diff  )
        {
            offset += TIMESTEP*(ss->scroll_up*(SCROLL_MAX-SCROLL_MIN)+SCROLL_MIN);
            if( ss->offset+offset > diff )
                offset = diff - ss->offset;
            ss->scroll_up += TIMESTEP/30.f/SCROLL_ACC;
            if(ss->scroll_up > 1.f)
                ss->scroll_up = 1.f;
            clr = CLR_MM_SS_ARROW;
            arrow_sc *= 1.1f;
        }
        else
        {
            ss->scroll_up = 0;
        }

        drawFlatFrame( PIX2, R6, (ss->x - ss->wd/2 + R10), curY, ss->z, (ss->wd-2*R10), (elem_ht-2), 1.f, clr, clr );
        display_sprite( up, (ss->x) - (up->width*arrow_sc/2), (curY+(elem_ht-(up->height*arrow_sc))/2), ss->z, arrow_sc, arrow_sc, CLR_WHITE );
        shrink_dir |= UIBAD_TOP;
    }

    // off the bottom
    diff = (ss->count-ss->cur_idx+0)*elem_ht - (ss->ht-(ss->y-ss->topY));
    if(ss->sc >= 1.f && ss->offset > -diff)
    {
        // bottom arrow
        int clr = 0xffffff44;
        float arrow_sc = 1.f;
        float curY = ss->topY + ss->ht - 3*elem_ht/2;
        BuildCBox( &cbox, (ss->x - ss->wd/2 + R10), curY, (ss->wd-2*R10), (elem_ht-1) );
        if( mouseCollision(&cbox) && ss->offset > -diff )
        {
            offset -= TIMESTEP*(ss->scroll_down*(SCROLL_MAX-SCROLL_MIN)+SCROLL_MIN);
            if( ss->offset+offset < -diff )
                offset = -diff - ss->offset;
            ss->scroll_down += TIMESTEP/30.f/SCROLL_ACC;
            if(ss->scroll_down > 1.f)
                ss->scroll_down = 1.f;

            clr = CLR_MM_SS_ARROW;
            arrow_sc *= 1.1f;
        }
        else
        {
            ss->scroll_down = 0;
        }

        drawFlatFrame( PIX2, R6, (ss->x - ss->wd/2 + R10), (curY+2), ss->z, (ss->wd-2*R10), (elem_ht), 1.f, clr, clr );
        display_sprite( down, (ss->x) - (up->width*arrow_sc/2), (curY+2 + ((elem_ht-(up->height*arrow_sc))/2)), ss->z, arrow_sc, arrow_sc, CLR_WHITE );
        shrink_dir |= UIBAD_BOTTOM;
    }

    // make room for the arrows
    if(shrink_dir != UIBAD_NONE)
    {
        uiBoxAlter( &box, UIBAT_SHRINK, shrink_dir, (elem_ht+R6) );
    }

    clipperPush(&box);
    for( i = 0; i < ss->count; i++ )
    {
        if(ss->elements[i]->legacy <=game_state.editnpc)
        {
            drawScrollElementMM( ss, ss->y + (drawn-ss->cur_idx)*elem_ht + ss->offset, i, &retVal );
            drawn++;
        }
    }
    clipperPop(&box);

    BuildCBox( &cbox, (ss->x - ss->wd/2), (ss->topY), ss->wd, ss->ht );
    if( mouseCollision(&cbox) )
        collisions_off_for_rest_of_frame = true;
    else if( mouseUp(MS_LEFT) || mouseDown(MS_LEFT) || mouseDragging() ) 
    {
        if( ss->type < kScrollType_GenericRegionSet )
            updateCostume();
        ss->mode = SS_CLOSING;
        if(mouseUp(MS_LEFT))
            collisions_off_for_rest_of_frame = true;
    }

    if( retVal >= 0 )
    {
        if( ss->type < kScrollType_GenericRegionSet )
            updateCostume();
        ss->mode = SS_CLOSING;
        collisions_off_for_rest_of_frame = true;
    }
    clipperPop();
    ss->offset += offset;
    return retVal;
}
//---------------------------------------------------------------------------------------------
// COSTUME SET ////////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------------------------
static const char* texName_unmangle(const char *name)
{
    // deal with the mangling from determineTextureNames()
    char *p;
    if(name[0] == '!')
    {
        name++;

        if( strStartsWith(name,"mm_") ||
            strStartsWith(name,"sm_") ||
            strStartsWith(name,"sf_") )
        {
            name += 3;
        }

        p = strchr(name,'_');
        if(p)
            name = p+1;
    }
//    name++;
    return name;
}

static bool texName_cmp(const char *a, const char *b)
{
    return stricmp(texName_unmangle(a),texName_unmangle(b));
}

static void applyCostumePieces(const NPCDef *npcdef)
{
    const CostumeOriginSet *oset = costume_get_current_origin_set();
    int rset_idx;
    for(rset_idx = 0; rset_idx < eaSize(&oset->regionSet); rset_idx++)
    {
        const CostumeRegionSet * rset = oset->regionSet[rset_idx];
        int bestbset = -1; // the first valid boneset, or the current boneset if it's valid
        int bestparts = 0; // number of matching geos
        int bestbreakers = 0;
        int bset_idx;
        for(bset_idx = 0; bset_idx < eaSize(&rset->boneSet); bset_idx++)
        {
            const CostumeBoneSet *bset = rset->boneSet[bset_idx];
            int validparts = 0; // number of matching geos
            int tiebreakers = 0; // exact geo matches. needed for sleeved/sleeveless jackets
            int gset_idx;
            for(gset_idx = 0; gset_idx < eaSize(&bset->geo); gset_idx++)
            {
                CostumeGeoSet *gset = cpp_const_cast(CostumeGeoSet*)(bset->geo[gset_idx]);    // mutable_gset
                bool partlessgset = true; // there is no part for this
                bool emptygset = true; // there is no match for this
                int part_idx;
                if(!gset->bodyPartName || bpGetBodyPartFromName(gset->bodyPartName)->dont_clear)
                    continue;
                for(part_idx = 0; part_idx < eaSize(&npcdef->costumes[0]->parts); part_idx++)
                {
                    const CostumePart *part = npcdef->costumes[0]->parts[part_idx];
                    int info_idx;
                    if(costume_isPartEmpty(part) || stricmp(gset->bodyPartName,part->pchName))
                        continue;
                    partlessgset = false;
                    for(info_idx = 0; info_idx < eaSize(&gset->info); info_idx++)
                    {
                        const CostumeTexSet *info = gset->info[info_idx];
                        if( !info->devOnly && !partIsRestricted_SkipChecksAgainstCurrentCostume(info) &&
                            (!part->pchGeom || strstriConst(info->geoName, part->pchGeom)) &&
                            (!part->pchTex1 || texName_cmp(info->texName1, part->pchTex1)==0) &&
                            (
                              (!part->pchFxName && (!info->fxName||stricmp(info->fxName,"none")==0)) || 
                              (part->pchFxName && info->fxName && stricmp(info->fxName, part->pchFxName)==0) 
                            )
                          )
                        {
                            bool validtex2 = false;
                            if(info->texMasked || !stricmp(info->texName2,"mask"))
                            {
                                int mask_idx;
                                for(mask_idx = 0; mask_idx < eaSize(&gset->mask); mask_idx++)
                                {
                                    const CostumeMaskSet *mask = gset->mask[mask_idx];
                                    if(!part->pchTex2 || texName_cmp(mask->name,part->pchTex2)==0)
                                    {
                                        gset->currentMask = mask_idx;
                                        validtex2 = true;
                                        break;
                                    }
                                }
                            }

                            if( !validtex2 && 
                                (!part->pchTex2 || texName_cmp(info->texName2, part->pchTex2)==0) )
                            {
                                gset->currentMask = info->texMasked ? -1 : 0;
                                validtex2 = true;
                            }

                            if(validtex2)
                            {
                                gset->currentInfo = info_idx;
                                emptygset = false;
                                validparts++;
                                if(stricmp(info->geoName, part->pchGeom)==0)
                                    tiebreakers++;
                                break;
                            }
                        }
                    }
                }
                if(emptygset)
                {
                    int info_idx, mask_idx;

                    gset->currentInfo = -1;
                    for(info_idx = 0; info_idx < eaSize(&gset->info); info_idx++)
                    {
                        const CostumeTexSet *info = gset->info[info_idx];
                        if(!info->devOnly && !partIsRestricted_SkipChecksAgainstCurrentCostume(info))
                        {
                            if( isNullOrNone(info->geoName) &&
                                isNullOrNone(info->texName1) &&
                                (isNullOrNone(info->texName2) || info->texName2 && !stricmp(info->texName2,"mask")) &&
                                isNullOrNone(info->fxName) )
                            {
                                gset->currentInfo = info_idx;
                                break;
                            }
                            else if(gset->currentInfo < 0) // in case we can't find anything better
                            {
                                gset->currentInfo = info_idx;
                            }
                        }
                    }
                    if(info_idx >= eaSize(&gset->info))
                    {
                        if(gset->currentInfo < 0)
                            gset->currentInfo = 0;
                        if(partlessgset)
                            validparts--; // it should have been 'none', but we were forced to take a part
                    }

                    for(mask_idx = 0; mask_idx < eaSize(&gset->mask); mask_idx++)
                    {
                        const CostumeMaskSet *mask = gset->mask[mask_idx];
                        if(isNullOrNone(mask->name))
                        {
                            gset->currentMask = mask_idx;
                            break;
                        }
                    }
                    if(mask_idx >= eaSize(&gset->mask))
                        gset->currentMask = 0;
                }
            }
            if( validparts > bestparts || 
                (validparts == bestparts && (bset_idx == rset->currentBoneSet || tiebreakers > bestbreakers)) )
            {
                bestbset = bset_idx;
                bestparts = validparts;
                bestbreakers = tiebreakers;
            }
        }
        if(bestbset >= 0)
            (cpp_const_cast(CostumeRegionSet*)(rset))->currentBoneSet = bestbset;
        else
        {
            for(bset_idx = 0; bset_idx < eaSize(&rset->boneSet); bset_idx++)
            {
                const CostumeBoneSet *bset = rset->boneSet[bset_idx];
                if(stricmp(bset->name,"none")==0)
                {
                    (cpp_const_cast(CostumeRegionSet*)(rset))->currentBoneSet = bset_idx;
                    break;
                }
            }
            if(bset_idx >= eaSize(&rset->boneSet))
                Errorf("No matching boneset for npc costume %s for region %s",npcdef->displayName,rset->name);
        }
    }
}

void applyCostumeSet(const CostumeOriginSet *oset, int selectedCostumeSet)
{
    if(selectedCostumeSet >= 0)
    {
        extern CostumeSave **gCostStack;
        const CostumeSetSet *set = oset->costumeSets[selectedCostumeSet];
        const NPCDef *npc = npcFindByName(set->npcName,NULL);
        if(gSelectedCostumeSet < 0) // custom costume, save it
            eaPush(&gCostStack, costumeBuildSave(NULL));
        if(npc && npc->costumes[0]->parts)
        {
            applyCostumePieces(npc);
            updateCostumeEx(false);
        }
        else
        {
            ErrorFilenamef(set->filename, "Unknown npc %s for costume set %s", set->npcName, set->name);
        }
    }
    gSelectedCostumeSet = selectedCostumeSet;
}

void selectCostumeSet(const CostumeOriginSet *oset, float x, float y, float z, float sc, float wd, F32 alpha)
{
    int result, i, selectedCostumeSet;
    static HybridElement hb;
    ScrollSelector *ss = matchSS(oset, kScrollType_CostumeSet);
    F32 xposSc, yposSc;
    calculatePositionScales(&xposSc, &yposSc);

    if(ss)
    {
        result = drawScrollSelector(ss);
        if(result >= 0)
            applyCostumeSet(oset, ss->elements[result]->idx);
    }

    if (gSelectedCostumeSet < 0)
    {
        result = drawSelectorBar(&hb, x*xposSc, y*yposSc, z, sc, wd, "CustomString", 0, 0, ss, NULL, alpha, NULL);
    }
    else
    {
        bool showLock = costumesetIsLocked(oset->costumeSets[gSelectedCostumeSet]);
        result = drawSelectorBar(&hb, x*xposSc, y*yposSc, z, sc, wd, oset->costumeSets[gSelectedCostumeSet]->displayName, 0, 0, ss, showLock ? "" : NULL, alpha, "CostumeSetLockedString");
    }
    switch(result)
    {
        xcase SELECT_FORWARD:
            selectedCostumeSet = gSelectedCostumeSet;
            for(i = 0; i < eaSize(&oset->costumeSets); ++i)
            {
                selectedCostumeSet++;
                if(selectedCostumeSet >= eaSize(&oset->costumeSets))
                    selectedCostumeSet = 0;
                if(!costumesetIsRestricted(oset->costumeSets[selectedCostumeSet]))
                    break;
            }
            applyCostumeSet(oset, i < eaSize(&oset->costumeSets) ? selectedCostumeSet : -1);

        xcase SELECT_BACK:
            selectedCostumeSet = gSelectedCostumeSet;
            for(i = 0; i < eaSize(&oset->costumeSets); ++i)
            {
                selectedCostumeSet--;
                if(selectedCostumeSet < 0)
                    selectedCostumeSet = eaSize(&oset->costumeSets)-1;
                if(!costumesetIsRestricted(oset->costumeSets[selectedCostumeSet]))
                    break;
            }
            applyCostumeSet(oset, i < eaSize(&oset->costumeSets) ? selectedCostumeSet : -1);

        xcase SELECT_CLICK:
            {
                addSS(kScrollType_CostumeSet, oset, x, y, z, wd, sc, 0);
            }
    }
}

//---------------------------------------------------------------------------------------------
// Power Customization Function ///////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------------------------

int selectCustomPower(const BasePower *power, float x, float y, float z, float sc, float wd, int mode, F32 alpha)
{
    int result;
    ScrollSelector * ss = matchSS(power, kScrollType_CustomPower);
    Entity *e = playerPtr();
    const Appearance *app = &e->costume->appearance;
    const char* selectedLabel;
    int selectedIndex = -1;
    int updated = 0;
    static HybridElement hb;

    int i;
    PowerCustomization* cust = powerCust_FindPowerCustomization(e->powerCust, power);
    F32 xposSc, yposSc;
    calculatePositionScales(&xposSc, &yposSc);

    if (cust && cust->token) 
    {
        for (i = eaSize(&power->customfx) - 1; i >= 0; --i) {
            if (stricmp(power->customfx[i]->pchToken, cust->token) == 0)
                selectedIndex = i;
        }
    }

    //  scroll selector
    if(ss)
    {
        result = drawScrollSelector(ss);
        if(cust && result >= 0)
        {
            if (selectedIndex != ss->elements[result]->idx) 
            {
                (cpp_const_cast(BasePowerSet*)(power->psetParent))->iCustomToken = -2;
                selectedIndex = ss->elements[result]->idx;
                
                if (selectedIndex >= 0)
                    cust->token = power->customfx[selectedIndex]->pchToken;
                else
                    cust->token = NULL;

                updated = 1;

            }
        }
    }

    if (EAINRANGE(selectedIndex, power->customfx))
    {
        const PowerCustomFX* customfx = power->customfx[selectedIndex];
        selectedLabel = customfx->pchDisplayName ? getPowerThemeTranslation(customfx->pchDisplayName) : 
            getPowersetThemeTranslation(customfx->pchToken);
    } else
        selectedLabel = "OriginalString";

    // display the selector bar
    result = drawSelectorBar(&hb, x*xposSc, y*yposSc, z, sc, wd, selectedLabel, 0, 1, ss, NULL, alpha, NULL);
    
    if (eaSize(&power->customfx) == 0)
        return updated;

    switch(result)
    {
        xcase SELECT_FORWARD:
            if(++selectedIndex < eaSize(&power->customfx))
                cust->token = power->customfx[selectedIndex]->pchToken;
            else
                cust->token = NULL;

            (cpp_const_cast(BasePowerSet*)(power->psetParent))->iCustomToken = -2;
            clearSS();
            updated = 1;

        xcase SELECT_BACK:
            if (--selectedIndex < -1)
                selectedIndex = eaSize(&power->customfx) - 1;

            if (selectedIndex < 0)
                cust->token = NULL;
            else
                cust->token = power->customfx[selectedIndex]->pchToken;

            (cpp_const_cast(BasePowerSet*)(power->psetParent))->iCustomToken = -2;
            clearSS();
            updated = 1;

        xcase SELECT_RESET:
            resetCustomPower(power);
            clearSS();
            updated = 1;

        xcase SELECT_CLICK:
            {
                float buttonWidth = ((ss && ss->architect) ? 45:32)*sc;
                float resetWidth = RESET_WD;
                addSS(kScrollType_CustomPower, power, x, y, z, wd, sc, 0 );    
            }
    }
    return updated;
}


int selectPowerSetTheme(BasePowerSet *powerSet, float x, float y, float z, float sc, float wd, F32 alpha)
{
    int result;
    ScrollSelector * ss = matchSS(powerSet, kScrollType_CustomPowerSet);
    Entity *e = playerPtr();
    const Appearance *app = &e->costume->appearance;
    const char* token;
    static HybridElement hb;
    int updated = 0;
    F32 xposSc, yposSc;
    calculatePositionScales(&xposSc, &yposSc);

    //  scroll selector
    if(ss)
    {
        result = drawScrollSelector(ss);
        if(result >= 0)
        {
            powerSet->iCustomToken = ss->elements[result]->idx; // default is at idx 0
            updateCustomPowerSet(powerSet);
            updated = 1;
        }
    }

    // display the selector bar
    if (powerSet->iCustomToken == -2)
        token = "CustomString";
    else if ((powerSet->iCustomToken == -1) || (!powerSet->ppchCustomTokens))
        token = "OriginalString";
    else
        token = getPowersetThemeTranslation(powerSet->ppchCustomTokens[powerSet->iCustomToken]);

    result = drawSelectorBar(&hb, x*xposSc, y*yposSc, z, sc, wd, token, 0, 0, ss, 0, alpha, NULL);

    if (eaSize(&powerSet->ppchCustomTokens) == 0)
        return updated;

    switch(result)
    {
        xcase SELECT_FORWARD:
            if(++powerSet->iCustomToken >= eaSize(&powerSet->ppchCustomTokens))
                powerSet->iCustomToken = -1;
            updateCustomPowerSet(powerSet);
            clearSS();
            updated = 1;

        xcase SELECT_BACK:
            if(--powerSet->iCustomToken < -1)
                powerSet->iCustomToken = eaSize(&powerSet->ppchCustomTokens) - 1;
            updateCustomPowerSet(powerSet);
            clearSS();
            updated = 1;

        xcase SELECT_CLICK:
            {
                addSS(kScrollType_CustomPowerSet, powerSet, x, y, z, wd, sc, 0);
            }
    }
    return updated;
}


void scrollSet_ElementListSelector( MMScrollSet_Mission *pMission, MMElementList * pList, float x, float y, float z, float sc, float wd )
{
    int result, i;
     ScrollSelector *ss = matchSS(pList, kScrollType_GenericElementList);

       if(ss) 
    {
        if( ss->x != x  || ss->y != y ) // These things just do not support being ballyed about
            ss->mode = SS_CLOSED;
        ss->x = x;
        ss->y = y;
        ss->z = z+20;
        result = drawScrollSelectorMM(ss);
        if(result >= 0 && result < eaSize(&ss->elements) && ss->elements[result]->idx != pList->current_element )
        {
            pList->updated = 1;
            pList->doneCritterValidation = 0;
            pList->current_element = ss->elements[result]->idx;
        }
        ss->architect = 1;
    }

     result = drawSelectorBarMM(x, y, z+2, sc, wd, pList->ppElement[pList->current_element]->pchDisplayName, !MMScrollSet_isElementListValid(pMission, pList, 0, 0, 0, 0), ss);
    switch(result)
    {
        xcase SELECT_FORWARD:
        pList->updated = 1;
        pList->doneCritterValidation = 0;
        for(i = 0; i < eaSize(&pList->ppElement); i++)
        {
            pList->current_element++;
            if(pList->current_element >= eaSize(&pList->ppElement))
                pList->current_element = 0;
            if( playerCreatedStoryarc_isUnlockedContent(pList->ppElement[pList->current_element]->pchText, playerPtr()) || pList->bDontUnlockCheck )
                break;
        }
        xcase SELECT_BACK:
        pList->updated = 1;
        pList->doneCritterValidation = 0;
        for(i = 0; i < eaSize(&pList->ppElement); i++)
        {
            pList->current_element--;
            if(pList->current_element < 0)
                pList->current_element = eaSize(&pList->ppElement) - 1;
            if( playerCreatedStoryarc_isUnlockedContent(pList->ppElement[pList->current_element]->pchText, playerPtr()) || pList->bDontUnlockCheck )
                break;
        }
        xcase SELECT_CLICK:
        {
            float buttonWidth = ((ss && ss->architect) ? 45:32)*sc;
              addSS(kScrollType_GenericElementList, pList, x, y, z, wd-(1.5*buttonWidth), sc, pMission);
        }
    }    
}
void selectVillainGroup( MMElementList *pList, float x, float y, float z, float sc, float wd, int rightList)
{
    int result, i;
    ScrollSelector *ss = matchSS(pList, rightList ? kScrollType_RightVillainGroupList : kScrollType_LeftVillainGroupList );

    if(ss)
    {
        if( ss->x != x  || ss->y != y ) // These things just do not support being ballyed about
            ss->mode = SS_CLOSED;
        ss->x = x;
        ss->y = y;
        ss->z = z+20;
        font(&game_12);
        result = drawScrollSelectorMM(ss);
        if(result >= 0 && result != pList->current_element )
        {
            pList->updated = 1;
            pList->current_element = ss->elements[result]->idx;
        }
        ss->architect = 1;
    }

    result = drawSelectorBarMM(x, y, z+2, sc, wd, pList->ppElement[pList->current_element]->pchDisplayName, 0, ss);

    switch(result)
    {
    xcase SELECT_FORWARD:
    pList->updated = 1;
    for(i = 0; i < eaSize(&pList->ppElement); i++)
    {
        pList->current_element++;
        if(pList->current_element >= eaSize(&pList->ppElement))
            pList->current_element = 0;
        if( playerCreatedStoryarc_isUnlockedContent(pList->ppElement[pList->current_element]->pchText, playerPtr()) || pList->bDontUnlockCheck )
            break;
    }
    xcase SELECT_BACK:
    pList->updated = 1;
    for(i = 0; i < eaSize(&pList->ppElement); i++)
    {
        pList->current_element--;
        if(pList->current_element < 0)
            pList->current_element = eaSize(&pList->ppElement) - 1;
        if( playerCreatedStoryarc_isUnlockedContent(pList->ppElement[pList->current_element]->pchText, playerPtr()) || pList->bDontUnlockCheck )
            break;
    }
    xcase SELECT_CLICK:
    {
        float buttonWidth = ((ss && ss->architect) ? 45:32)*sc;
        addSS(rightList ? kScrollType_RightVillainGroupList : kScrollType_LeftVillainGroupList, pList, x, y, z, wd-(1.5*buttonWidth), sc, 0);
    }
    }    
}
void scrollSet_TextListSelector( GenericTextList *pList, float x, float y, float z, float sc, float wd, int active )
{
    int result;//, i, selectedCostumeSet;

    ScrollSelector *ss = matchSS(pList, kScrollType_GenericTextList);

    if(ss)
    {
        result = drawScrollSelector(ss);
        if(result >= 0 && active)
            pList->current = ss->elements[result]->idx;
    }

    switch(result)
    {
        xcase SELECT_FORWARD:
        if(active && ++pList->current >= pList->count)
            pList->current = 0;
        xcase SELECT_BACK:
        if(active && --(pList->current) < 0)
            pList->current = pList->count - 1;
        xcase SELECT_CLICK:
        {
            float buttonWidth = ((ss && ss->architect) ? 45:32)*sc;
             addSS(kScrollType_GenericTextList, pList, x, y, z, wd-(1.5*buttonWidth), 1.f, 0);
        }
    }    
}


//---------------------------------------------------------------------------------------------
// BONE SET ///////////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------------------------
static void changeRegionSet( int regNum, int forward, int mode )
{
    CostumeRegionSet * reg = cpp_const_cast(CostumeRegionSet*)( costume_get_current_origin_set()->regionSet[regNum] );

     if( forward )
    {
        do
        {
            reg->currentBoneSet++;
            if( reg->currentBoneSet >= eaSize( &reg->boneSet ) )
                reg->currentBoneSet = 0;
        } while( bonesetIsRestricted(reg->boneSet[reg->currentBoneSet]));
    }
    else
    {
        do {
            reg->currentBoneSet--;
            if( reg->currentBoneSet < 0 )
                reg->currentBoneSet = eaSize( &reg->boneSet )-1;
        } while( bonesetIsRestricted(reg->boneSet[reg->currentBoneSet]));

    }

    updateCostume();
}

void selectBoneset( const CostumeRegionSet * region, float x, float y, float z, float sc, float wd, int mode, F32 alpha )
{
    int result;
    static HybridElement hb;
     ScrollSelector * ss = matchSS( region, kScrollType_Boneset );
    F32 xposSc, yposSc;
    calculatePositionScales(&xposSc, &yposSc);

     if( ss )
    {
        font(&game_12);
        result = drawScrollSelector( ss );
        if( result >= 0 )
        {
             (cpp_const_cast(CostumeRegionSet*)(region))->currentBoneSet = ss->elements[result]->idx;
            updateCostume();
        }
    }
    
    // extra hacky trench coat hack
    {
        bool grey_out = (costume_isRestrictedPiece(region->boneSet[region->currentBoneSet]->flags, "cloth") && 
                            costume_isOtherRestrictedPieceSelected( region->boneSet[region->currentBoneSet]->name, "cloth" ));
        result = drawSelectorBar( &hb, x*xposSc, y*yposSc, z, sc, wd, region->boneSet[region->currentBoneSet]->displayName, grey_out, 0, ss, NULL, alpha, NULL );
    }
    
      if ( result == SELECT_FORWARD )
    {
        changeRegionSet( region->idx, 1, mode );
        clearSS();
    }
    else if ( result == SELECT_RESET )
    {
        tailor_RevertRegionToOriginal( costume_get_current_origin_set(), region );
        updateCostume();
        clearSS();
    }
    else if ( result == SELECT_BACK )
    {
         changeRegionSet( region->idx, 0, mode );
        clearSS();
    }
    else if( result == SELECT_CLICK )
    {
           addSS( kScrollType_Boneset, region, x, y, z, wd, sc, 0);    
    }
}

//---------------------------------------------------------------------------------------------
// GeoSet Changing Functions //////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------------------------
static void resetTex2(CostumeGeoSet * gset );

static void changeGeoInfo( CostumeGeoSet *gset, int forward, int debug_orig )
{
    char    lastGeoName[128];
    char    lastTexName[128];

    int emblem = stricmp("base",gset->info[gset->currentInfo]->texName1)==0;

    if (debug_orig == -1) // First time in here (non-recursive call to it)
        debug_orig = gset->currentInfo;

    strcpy( lastGeoName, gset->info[gset->currentInfo]->geoName );

    if( emblem )
        strcpy( lastTexName, gset->info[gset->currentInfo]->texName2 );
    else
        strcpy( lastTexName, gset->info[gset->currentInfo]->texName1 );

    if( forward )
    {
        int index;

        // first go to the next geo name
         while( stricmp( lastGeoName, gset->info[gset->currentInfo]->geoName ) == 0 )
        {
            gset->currentInfo++;

            // check for loop
            if (gset->currentInfo == debug_orig)
            {
                gset->currentInfo = debug_orig;
                return;
            }

            if( gset->currentInfo >= eaSize( &gset->info ) )
            {
                gset->currentInfo = 0;
                // check for loop
                if (gset->currentInfo == debug_orig)
                {
                    gset->currentInfo = debug_orig;
                    return;
                }
                break;
            }
        }

        if (gset->currentInfo == debug_orig && isDevelopmentMode() && game_state.showCustomErrors && partIsRestricted(gset->info[gset->currentInfo])) {
            // COVCOSTUMETODO: Verify this in loading phase!
            winMsgAlert("This geo set has no available options!");
            return; // Stop infinite recursion!
        }
        index = gset->currentInfo;
        strcpy( lastGeoName, gset->info[gset->currentInfo]->geoName );

        // now look for the same texture
        while( stricmp( lastTexName, (emblem?gset->info[gset->currentInfo]->texName2:gset->info[gset->currentInfo]->texName1) ) != 0 )
        {
            gset->currentInfo++;

            // check for loop
            if (gset->currentInfo == debug_orig)
            {
                gset->currentInfo = debug_orig;
                return;
            }

             if( gset->currentInfo < gset->infoCount && partIsRestricted(gset->info[index]) && stricmp(lastGeoName, gset->info[gset->currentInfo]->geoName)==0  ) 
            {
                index = gset->currentInfo;
            }

            // went off the end of the array or didn't find it
            if( (gset->currentInfo >= gset->infoCount ||
                stricmp( lastGeoName, gset->info[gset->currentInfo]->geoName ) != 0 ))
            {
                gset->currentInfo = index;
                break;
            }
        }
    }
    else // kinda the same thing but backwards
    {
        // first go to the previous geo name
        while( stricmp( lastGeoName, gset->info[gset->currentInfo]->geoName ) == 0 )
        {
            gset->currentInfo--;
            // check for loop
            if (gset->currentInfo == debug_orig)
            {
                gset->currentInfo = debug_orig;
                return;
            }

            if( gset->currentInfo < 0 )
            {
                gset->currentInfo = eaSize( &gset->info )-1;
                while( partIsRestricted(gset->info[gset->currentInfo]) )
                {
                    gset->currentInfo--;
                    // check for loop
                    if (gset->currentInfo == debug_orig)
                    {
                        gset->currentInfo = debug_orig;
                        return;
                    }
                }

                if( stricmp( lastGeoName, gset->info[gset->currentInfo]->geoName ) == 0 )
                {
                    gset->currentInfo = 0;
                    return;                    
                }

                break;
            }
        }

        if (gset->currentInfo == debug_orig && isDevelopmentMode() && game_state.showCustomErrors && partIsRestricted(gset->info[gset->currentInfo])) {
            // COVCOSTUMETODO: Verify this in loading phase!
            winMsgAlert("This geo set has no available options!");
            return; // Stop infinite recursion!
        }

        strcpy( lastGeoName, gset->info[gset->currentInfo]->geoName );

        // now go back looking for the same texture
        while( stricmp( lastTexName, (emblem?gset->info[gset->currentInfo]->texName2:gset->info[gset->currentInfo]->texName1) ) != 0 )
        {
             gset->currentInfo--;
            // check for loop
            if (gset->currentInfo == debug_orig)
            {
                gset->currentInfo = debug_orig;
                return;
            }

            // went off the end of the array
            if( gset->currentInfo < 0 )
            {
                gset->currentInfo = 0;
                break;
            }

            // didn't find it, go forward one
            if( stricmp( lastGeoName, gset->info[gset->currentInfo]->geoName ) != 0  )
            {
                gset->currentInfo++;
                break;
            }
        }

        if( gset->currentInfo+1 < gset->infoCount && partIsRestricted(gset->info[gset->currentInfo]) && stricmp(lastGeoName, gset->info[gset->currentInfo+1]->geoName)==0  )
            gset->currentInfo++;
    }


    if (gset->currentInfo == debug_orig && isDevelopmentMode() && game_state.showCustomErrors && partIsRestricted(gset->info[gset->currentInfo])) {
        // COVCOSTUMETODO: Verify this in loading phase!
        winMsgAlert("This geo set has no available options!");
    } else {
        while( partIsRestricted(gset->info[gset->currentInfo]) && gset->currentInfo != debug_orig)
            changeGeoInfo( gset, forward, debug_orig);
    }

    if( gset->currentMask < 0 )
        resetTex2( gset );

    updateCostume();
}

//
//
static int countCurrentGeoInfos( CostumeGeoSet * gset )
{

    char    lastGeoName[128];
    int     count = 0, index = gset->currentInfo;
    strcpy( lastGeoName, gset->info[gset->currentInfo]->geoName );

    // first go to the first geo name
    while( stricmp( lastGeoName, gset->info[index]->geoName ) == 0 )
    {
        index--;
        if( index < 0 )
            break;
    }
    index++;

    // now count how many there are
    while( stricmp( lastGeoName, gset->info[index]->geoName ) == 0 )
    {
        if( !partIsRestricted(gset->info[gset->currentInfo]))
            count++;

        index++;
        if( index >= eaSize( &gset->info ) )
            break;
    }

    return count;
}

// Return 1 if all items are store locked
//
static int checkStoreCurrentGeoInfos( CostumeGeoSet * gset )
{
    char    lastGeoName[128];
    int index;
    strcpy( lastGeoName, gset->info[gset->currentInfo]->geoName );

    // first go to the first geo name (minus 1)
    for (index = gset->currentInfo; index >= 0 && (stricmp( lastGeoName, gset->info[index]->geoName ) == 0) ; --index);
    ++index;

    // now check if all are store items
    for(; index < eaSize( &gset->info ) && (stricmp( lastGeoName, gset->info[index]->geoName ) == 0); ++index)
    {
        if( !partIsRestricted(gset->info[index]) && (skuIdIsNull(SELLABLE_PRODUCT(gset->info[index]))))
        {
            return 0;
        }
    }

    return 1; //
}

//
//
static void resetGeoTex( CostumeGeoSet *gset, int tex)
{
    char    lastGeoName[128];
    strcpy( lastGeoName, gset->info[gset->currentInfo]->geoName );

     if( tex )
    {
        while( stricmp( lastGeoName, gset->info[gset->currentInfo]->geoName ) == 0 && gset->currentInfo > 0 )
        {
            gset->currentInfo--;
        }

        if( gset->currentInfo != 0 || stricmp(gset->info[0]->geoName, gset->info[1]->geoName) != 0  )
            gset->currentInfo++;

        while( partIsRestricted(gset->info[gset->currentInfo]))
            gset->currentInfo++;
    }
    else
    {
        char    lastTexName[128];
        strcpy( lastTexName, gset->info[gset->currentInfo]->texName1);

        gset->currentInfo = 0;
        while( partIsRestricted(gset->info[gset->currentInfo]) )
            gset->currentInfo++;

        strcpy( lastGeoName, gset->info[gset->currentInfo]->geoName );

        while( stricmp(lastTexName, gset->info[gset->currentInfo]->texName1 ) != 0 &&
            stricmp( lastGeoName, gset->info[gset->currentInfo]->geoName ) == 0 )
        {
            do
            {
                gset->currentInfo++;
            } while ( partIsRestricted(gset->info[gset->currentInfo]));
        }

        if( stricmp( lastGeoName, gset->info[gset->currentInfo]->geoName ) != 0)
        {
            gset->currentInfo = 0;
            while( partIsRestricted(gset->info[gset->currentInfo]) )
                gset->currentInfo++;
        }
    }
    if( gset->currentMask < 0 )
        resetTex2( gset );

    updateCostume();
}

//
//
static void changeTexInfo( CostumeGeoSet *gset, int forward )
{
    char    lastGeoName[128];
    int start = gset->currentInfo;
    strcpy( lastGeoName, gset->info[gset->currentInfo]->geoName );

    if( forward )
    {
        gset->currentInfo++;

        // go back to the begining of this geometry set
        if( gset->currentInfo >= eaSize( &gset->info ) ||
            stricmp( lastGeoName, gset->info[gset->currentInfo]->geoName ) != 0 )
        {
            gset->currentInfo--; // push back inside original geoset
            while( stricmp( lastGeoName, gset->info[gset->currentInfo]->geoName ) == 0 )
            {
                gset->currentInfo--;
                if( gset->currentInfo < 0 )
                    break;
            }
            gset->currentInfo++; // one last push
        }
    }
    else
    {
        gset->currentInfo--;

        // go to the end of this geometry set
        if( (gset->currentInfo < 0 || stricmp( lastGeoName, gset->info[gset->currentInfo]->geoName ) != 0) )
        {
            gset->currentInfo++; // push back inside geoset
            while( stricmp( lastGeoName, gset->info[gset->currentInfo]->geoName ) == 0 )
            {
                gset->currentInfo++;
                if( gset->currentInfo >= eaSize( &gset->info ) )
                {
                    break;
                }
            }
            gset->currentInfo--; // final push
        }
    }

    while( partIsRestricted(gset->info[gset->currentInfo]))
        changeTexInfo( gset, forward );

    if( gset->currentMask < 0 )
        resetTex2( gset );

    updateCostume();
}

//
//
static void changeGeoTexInfo( CostumeGeoSet *gset, int forward )
{
    int orig = gset->currentInfo;
    if( forward )
    {
        do
        {
            gset->currentInfo++;

            if( gset->currentInfo >= eaSize( &gset->info ) )
                gset->currentInfo = 0;

        } while ( partIsRestricted(gset->info[gset->currentInfo]) && gset->currentInfo != orig);
    }
    else
    {
        do
        {
            gset->currentInfo--;

            if( gset->currentInfo < 0 )
                gset->currentInfo = eaSize( &gset->info ) -1;

        } while ( partIsRestricted(gset->info[gset->currentInfo]) && gset->currentInfo != orig);
    }
    if (gset->currentInfo == orig && isDevelopmentMode() && game_state.showCustomErrors && partIsRestricted(gset->info[gset->currentInfo])) {
        // COVCOSTUMETODO: Verify this in loading phase!
        winMsgAlert("This set has nothing flagged as available!");
    }

    if( gset->currentMask < 0 )
        resetTex2( gset );

    updateCostume();
}

//
//
static void changeTex2Info( CostumeGeoSet *gset, int forward )
{
    int texMasked = gset->info[gset->currentInfo]->texMasked;
    int orig = gset->currentMask;

    if( forward )
    {
        do 
        {
            gset->currentMask++;
            if( eaSize( &gset->mask ) )
            {
                if( gset->currentMask >= eaSize( &gset->mask ) )
                    gset->currentMask = texMasked?-1:0;
            }
            else
                if( gset->currentMask >= eaSize( &gset->masks ) )
                    gset->currentMask = texMasked?-1:0;

        }while( gset->currentMask >= 0 && maskIsRestricted( gset->mask[gset->currentMask] ) && gset->currentMask!=orig);

    }
    else
    {
        do 
        {
            gset->currentMask--;
            if( gset->currentMask < (texMasked?-1:0) )
            {
                if( eaSize( &gset->mask) )
                    gset->currentMask = eaSize( &gset->mask ) -1;
                else
                    gset->currentMask = eaSize( &gset->masks ) -1;
            }
        }while( gset->currentMask >= 0 && maskIsRestricted(gset->mask[gset->currentMask]) && gset->currentMask!=orig);
    }
    if (gset->currentMask == orig && isDevelopmentMode() && game_state.showCustomErrors && gset->currentMask >= 0 &&  maskIsRestricted( gset->mask[gset->currentMask]) )
    {
        // COVCOSTUMETODO: Verify this in loading phase!
        winMsgAlert("This Mask set has nothing flagged as available!");
    }

    updateCostume();
}

static void changeFaceScaleInfo( CostumeGeoSet *gset, int forward )
{
    int j;
    Entity * e = playerPtr();
    const Appearance *app = &e->costume->appearance;

    if( forward )
    {
        gset->currentInfo++;
        if( gset->currentInfo >= eaSize( &gset->faces ) )
            gset->currentInfo = 0;
    }
    else
    {
        gset->currentInfo--;
        if( gset->currentInfo < 0 )
            gset->currentInfo = eaSize(&gset->faces)-1;
    }

    memcpy( &gFaceDest, gset->faces[gset->currentInfo]->face, sizeof(Vec3)*NUM_3D_BODY_SCALES );
    gConvergingFace = true;
    for( j = 0; j < NUM_3D_BODY_SCALES; j++ )
    {
        gFaceDestSpeed[j][0] = ABS(gFaceDest[j][0]-app->f3DScales[j][0])*.1f/2;
        gFaceDestSpeed[j][1] = ABS(gFaceDest[j][1]-app->f3DScales[j][1])*.1f/2;
        gFaceDestSpeed[j][2] = ABS(gFaceDest[j][2]-app->f3DScales[j][2])*.1f/2;
    }
}

void resetFace( CostumeGeoSet *gset, int mode )
{
    int j;
    Entity * e = playerPtr();
    const Appearance *app = &e->costume->appearance;

    gset->currentInfo = 0;
    while( gset->info && partIsRestricted(gset->info[gset->currentInfo]) )
        gset->currentInfo++;

    if( mode )
        memcpy( &gFaceDest, costume_current(e)->appearance.f3DScales, sizeof(Vec3)*NUM_3D_BODY_SCALES );
    else
        memcpy( &gFaceDest, gset->faces[0]->face, sizeof(Vec3)*NUM_3D_BODY_SCALES );
    gConvergingFace = true;
    for( j = 0; j < NUM_3D_BODY_SCALES; j++ )
    {
        gFaceDestSpeed[j][0] = ABS(gFaceDest[j][0]-app->f3DScales[j][0])*.1f/2;
        gFaceDestSpeed[j][1] = ABS(gFaceDest[j][1]-app->f3DScales[j][1])*.1f/2;
        gFaceDestSpeed[j][2] = ABS(gFaceDest[j][2]-app->f3DScales[j][2])*.1f/2;
    }
}

static void resetTex2(CostumeGeoSet * gset )
{
    int match = 0;
    int bpID = costume_PartIndexFromName( playerPtr(), gset->bodyPartName );

    if( isMenu(MENU_TAILOR) )
        match = tailor_RevertBone( bpID, gset->bsetParent, TAILOR_MASK  );

    if( !match )
    {
        gset->currentMask = gset->info[gset->currentInfo]->texMasked?-1:0;
        while( gset->currentMask >= 0 && maskIsRestricted(gset->mask[gset->currentMask]) )
            gset->currentMask++;
    }

    updateCostume();
}

//---------------------------------------------------------------------------------------------
// Selector Functions /////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------------------------

static const char *getGeoSetDisplayName( CostumeGeoSet * gset, int type, int mode, int face_match )
{
    switch( type )
    {
        case kScrollType_Geo:
            {
                if( gset->info[gset->currentInfo]->geoDisplayName )
                    return  gset->info[gset->currentInfo]->geoDisplayName;
                else
                    return  (char*)gset->info[gset->currentInfo]->geoName;
            }
        case kScrollType_Tex1:
        case kScrollType_GeoTex:
             return gset->info[gset->currentInfo]->displayName;
        case kScrollType_Tex2:
            {
                if(gset->mask && gset->currentMask>=0)
                    return gset->mask[gset->currentMask]->displayName;
                else
                    return gset->info[gset->currentInfo]->displayName;
            }
        case kScrollType_Face:
            {
                if( gConvergingFace )
                    return "AdjustingString";
                else if( (mode == COSTUME_EDIT_MODE_TAILOR) && !face_match )
                    return "Custom";
                else
                    return gset->faces[gset->currentInfo]->displayName;
            }
        default:
            return NULL;
    }
}

static int geoSetSelectIsGrey( CostumeGeoSet * gset, int type )
{
     switch( type )
    {
        case kScrollType_Geo:
        case kScrollType_Face:
        case kScrollType_GeoTex:
            return 0;
        case kScrollType_Tex1:
            return (countCurrentGeoInfos( gset ) <= 1); 
        case kScrollType_Tex2:
            return ( !gset->info[gset->currentInfo]->texName2 || (!gset->info[gset->currentInfo]->texMasked && stricmp( gset->info[gset->currentInfo]->texName2, "Mask" ) != 0) );
    }

    return 0;
}

void changeGeoSet( CostumeGeoSet * gset, int type, int forward )
{
    switch( type )
    {
        case kScrollType_Geo:
            {
                changeGeoInfo( gset, forward, -1 );
            }break;
        case kScrollType_Tex1:
            {
                changeTexInfo( gset, forward );
            }break;
        case kScrollType_GeoTex:
            {
                changeGeoTexInfo( gset, forward );
            }break;
        case kScrollType_Tex2:
            {
                changeTex2Info( gset, forward );
            }break;
        case kScrollType_Face:
            {
                changeFaceScaleInfo( gset, forward );
            }break;
    }
}

static void resetGeoSet( CostumeGeoSet * gset, int type, int mode )
{
    int match = 0;
    int bpID = 0;

    if( gset->bodyPartName )
        bpID = costume_PartIndexFromName( playerPtr(), gset->bodyPartName );

    switch( type )
    {
        case kScrollType_Geo:
            {
                if( (mode == COSTUME_EDIT_MODE_TAILOR) )
                {
                    match = tailor_RevertBone( bpID, gset->bsetParent, TAILOR_GEO );
                    updateCostume();
                }

                if( !match )
                    resetGeoTex( gset, FALSE );
            }break;
        case kScrollType_Tex1:
            {
                if( (mode == COSTUME_EDIT_MODE_TAILOR) )
                {
                    match = tailor_RevertBone( bpID, gset->bsetParent, TAILOR_TEX  );
                    updateCostume();
                }

                if( !match )
                    resetGeoTex( gset, TRUE );
            }break;
        case kScrollType_GeoTex:
            {
                if( (mode == COSTUME_EDIT_MODE_TAILOR) )
                {
                    if( !eaSize(&gset->mask) )
                        match = tailor_RevertBone( bpID, gset->bsetParent, TAILOR_GEOTEX );
                    else
                    {
                        if( gset->info[gset->currentInfo]->fxName )
                            match = tailor_RevertBone(bpID, gset->bsetParent, TAILOR_FX );
                        else
                            match = tailor_RevertBone( bpID, gset->bsetParent, TAILOR_TEX  );
                    }
                }

                if( !match )
                {
                    gset->currentInfo = 0;
                    while( gset->info && !gset->restrict_info && partIsRestricted(gset->info[gset->currentInfo]) )
                        gset->currentInfo++;
                }
                if( gset->currentMask < 0 )
                    resetTex2( gset );
                updateCostume();
            }break;
        case kScrollType_Tex2:
            {
                resetTex2(gset);
            }break;
        case kScrollType_Face:
            {
                resetFace( gset, mode );
            }break;
    }
    
}

//---------------------------------------------------------------------------------------------
// Generic Master Fucntion ////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------------------------

void selectGeoSet( CostumeGeoSet * gset, int type, F32 x, F32 y, F32 z, F32 sc, F32 wd, int mode, F32 alpha )
{
    int i, result;
    ScrollSelector * ss = matchSS( gset, type );
    const char * displayName;
    int grey = geoSetSelectIsGrey( gset, type );
    Entity * e = playerPtr();
    const Appearance *app = &e->costume->appearance;
    bool bMatch = false;
    const char *product = NULL;
    static HybridElement hb;
    F32 xposSc, yposSc;
    calculatePositionScales(&xposSc, &yposSc);

    // annoying extra just for face
    if( (mode == COSTUME_EDIT_MODE_TAILOR)  && type == kScrollType_Face ) // if its the tailor we may choose to display custom
    {
        for( i = eaSize(&gset->faces)-1; i >= 0; i-- )
        {
            if( memcmp(&gset->faces[i]->face, app->f3DScales, sizeof(Vec3)*NUM_3D_BODY_SCALES ) == 0 )
            {
                gset->currentInfo = i;
                bMatch = true;
            }
        }
    }

    //  scroll selector
    if( ss )
    {
         result = drawScrollSelector( ss );
        if( result >= 0 )
        {
            if( type == kScrollType_Tex2 )
            {
                 gset->currentMask = ss->elements[result]->idx;
                if( gset->currentMask > eaSize(&gset->mask) )
                    gset->currentMask = -1;
            }
            else
            {
                gset->currentInfo = ss->elements[result]->idx;
                if (gset->currentMask < 0)
                {
                    gset->currentMask = gset->info[gset->currentInfo]->texMasked ? -1 : 0;
                }
            }

            if( type == kScrollType_Face )
            {
                memcpy( &gFaceDest, gset->faces[result]->face, sizeof(Vec3)*NUM_3D_BODY_SCALES );
                gConvergingFace = true;
                for( i = 0; i < NUM_3D_BODY_SCALES; i++ )
                {
                    gFaceDestSpeed[i][0] = ABS(gFaceDest[i][0]-app->f3DScales[i][0])*.1f/2;
                    gFaceDestSpeed[i][1] = ABS(gFaceDest[i][1]-app->f3DScales[i][1])*.1f/2;
                    gFaceDestSpeed[i][2] = ABS(gFaceDest[i][2]-app->f3DScales[i][2])*.1f/2;
                }
            }
            updateCostume();
        }
    }

    // display the selector bar
    displayName = getGeoSetDisplayName( gset, type, mode, bMatch );
    if (gset && EAINRANGE(gset->currentInfo, gset->info) && gset->bodyPartName)
    {
        int drawGeoLock = type == kScrollType_Geo && checkStoreCurrentGeoInfos(gset);
        if (!grey)
        {
            grey = (costume_isRestrictedPiece(gset->flags, "cloth") && 
                        gset->bsetParent &&
                        costume_isOtherRestrictedPieceSelected( gset->bsetParent->name, "cloth" ));
        }
        if (!grey)
        {
            grey = (costume_isRestrictedPiece(gset->info[gset->currentInfo]->flags, "cloth") && 
                        gset->bsetParent &&
                        costume_isOtherRestrictedPieceSelected( gset->bsetParent->name, "cloth" ));
        }

        if ( !grey && type == kScrollType_Tex2 && EAINRANGE(gset->currentMask, gset->mask))
        {
            SkuId productId = SELLABLE_PRODUCT(gset->mask[gset->currentMask]);
            if (!skuIdIsNull(productId))
            {
                product = skuIdAsString(productId);
            }
        }
        else if ( !grey && (type == kScrollType_Tex1 || type == kScrollType_GeoTex || type == kScrollType_Tex2) || drawGeoLock)
        {
            SkuId productId = SELLABLE_PRODUCT(gset->info[gset->currentInfo]);
            if (!skuIdIsNull(productId))
            {
                product = skuIdAsString(productId);
            }
        }
    }
     result = drawSelectorBar( &hb, x*xposSc, y*yposSc, z, sc, wd, displayName, grey, 1, ss, product, alpha, NULL );

    // react to results
    if( result == SELECT_FORWARD )
    {
        changeGeoSet(gset, type, 1 );
        clearSS();
    }
    else if ( result == SELECT_RESET )
    {
        resetGeoSet( gset, type, mode );
        clearSS();
    }
    else if ( result == SELECT_BACK )
    {
        changeGeoSet(gset, type, 0 );
        clearSS();
    }
    else if ( result == SELECT_CLICK )
    {
        addSS( type, gset, x, y, z, wd, sc, 0 );    
    }
    else if (result == SELECT_BUY)
    {
        // HYBRID : Add code to display store here
        dialog( DIALOG_OK, -1, -1, -1, -1, product, NULL, NULL, NULL, NULL, 0, 
            NULL, NULL, 1, 0, 0, 0 );

    }
}



//---------------------------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------------------------

int updateScrollSelector(ScrollSelector * ss, F32 min, F32 max )
{
    CBox box; 
    F32 h = max-min;
    F32 top_ht, bot_ht, total_ht, y_offset;
     int color = SELECT_FROM_UISKIN( 0xaaaaffff, CLR_WHITE, CLR_WHITE );
    F32 xposSc, yposSc;
    calculatePositionScales(&xposSc, &yposSc);

       if( ss->mode == SS_OPEN && !gWindowDragging && !gScrollBarDragging && !mouseDragging() )
    {
        ss->sc += TIMESTEP*OPEN_SPEED;
        if( ss->sc > 1.f )
            ss->sc = 1.f;
    }
    else
    {
        ss->sc -= TIMESTEP*OPEN_SPEED;
        if( ss->sc < 0.f || ss->mode == SS_CLOSED )
        {
            int idx = eaFind(&scrollSelectors,ss);
            eaRemove(&scrollSelectors, idx);
            while( eaSize(&ss->elements) )
            {
                ScrollElement * se = eaRemove(&ss->elements,0);
                SAFE_FREE(se);
            }
            SAFE_FREE(ss);
            return 0;
        }
        ss->mode = SS_CLOSING;
    }

      total_ht = MIN(MIN( h, (ss->count+2)*elem_ht*ss->scale ), max-min); // 4 gives room for the two arrows and a little extra space
    y_offset = (ss->cur_idx + 1) * elem_ht * ss->scale;

     top_ht = MAX( elem_ht, MIN( y_offset, ss->y - min )); 
    bot_ht = MAX( elem_ht, MIN( total_ht-top_ht, max - ss->y ));
    top_ht = MAX( elem_ht, total_ht - bot_ht);
 
     ss->ht = top_ht+bot_ht; 
      ss->topY = ss->y - top_ht;
    
    if( ss->architect )
    {
        color = CLR_MM_TITLE_LIT;
        drawBustedOutFrame( kFrameStyle_3D, PIX3, R10, (ss->x-ss->wd/2), ss->y, ss->z, 1.f, ss->wd, 
            14, top_ht*ss->sc, bot_ht*ss->sc, color,CLR_BLACK, 0 );
    }
    else
    {
        AtlasTex * w = white_tex_atlas;
        F32 fadein = 30;
          int clrBlue = CLR_H_BACK&0xffffffc8;
        int clrWhite = 0x769cffc8;
         
          drawHybridFade( ss->x*xposSc, (ss->y - top_ht*ss->sc)*yposSc, ss->z, ss->wd, 2, 80, CLR_H_HIGHLIGHT);

         display_sprite_positional_blend( w, (ss->x - ss->wd/2)*xposSc, (ss->y - top_ht*ss->sc)*yposSc, ss->z, fadein/w->width, ss->ht*ss->sc/w->height, clrBlue&0xffffff00, clrBlue, clrBlue, clrBlue&0xffffff00, H_ALIGN_LEFT, V_ALIGN_TOP );
         display_sprite_positional_blend( w, (ss->x - ss->wd/2 + fadein)*xposSc, (ss->y - top_ht*ss->sc)*yposSc, ss->z, (ss->wd-(2*fadein))/w->width, ss->ht*ss->sc/w->height, clrBlue, clrWhite, clrWhite, clrBlue, H_ALIGN_LEFT, V_ALIGN_TOP );
        display_sprite_positional_blend( w, (ss->x + ss->wd/2 - fadein)*xposSc, (ss->y - top_ht*ss->sc)*yposSc, ss->z, fadein/w->width, ss->ht*ss->sc/w->height, clrWhite, clrWhite&0xffffff00, clrWhite&0xffffff00, clrWhite, H_ALIGN_LEFT, V_ALIGN_TOP );

        drawHybridFade( ss->x*xposSc, (ss->y + bot_ht*ss->sc)*yposSc, ss->z, ss->wd, 2, 80, CLR_H_HIGHLIGHT);
    }
     BuildScaledCBox( &box, (ss->x - ss->wd/2)*xposSc, (ss->topY+PIX3)*yposSc, ss->wd*xposSc, (ss->ht*ss->sc-2*PIX3)*yposSc );
 
     if( mouseCollision(&box) )
    {
        collisions_off_for_rest_of_frame = true;
        ss->blocked_collision = true;
    }

    if(ss->mode == SS_OPEN)
        return 1;
    else
        return 0;
}



int updateScrollSelectors(F32 min, F32 max )
{
    int i, is_open = 0;
      elem_ht = 30;
    for( i = 0; i < eaSize(&scrollSelectors); i++)
        is_open |= updateScrollSelector(scrollSelectors[i], min, max);

    return is_open;
}

