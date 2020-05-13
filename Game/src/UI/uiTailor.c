
#include <utilitieslib/utils/utils.h>    // for strstri
#include <utilitieslib/components/Earray.h>
#include "player/player.h"
#include "cmdparse/cmdgame.h"
#include "entity/costume.h"
#include "entity/EntPlayer.h"
#include "entity/entclient.h"    // for entSetAlpha
#include "clientcomm/clientcomm.h"
#include "entity/costume_client.h"
#include "entity/character_base.h"

#include "gameData/costume_data.h"
#include "language/langClientUtil.h"

#include "UI/uiNet.h"
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
#include "uiScrollSelector.h"
#include "UI/uiPowerCust.h"

#include "graphics/ttFontUtil.h"
#include "UI/sprite/sprite_base.h" 
#include "UI/sprite/sprite_text.h"
#include "UI/sprite/sprite_font.h"
#include "render/tex.h"
#include "seq/seq.h"
#include "seq/seqstate.h"
#include "entity/entity.h"
#include "entity/Supergroup.h"
#include "auth/authUserData.h"
#include "clientcomm/dbclient.h"
#include <utilitieslib/language/MessageStoreUtil.h>
#include "entity/character_inventory.h"
#include "entity/character_base.h"
#include "graphics/textureatlas.h"
#include "graphics/FX/fx.h"

#include "uiLoadCostume.h"
#include "uiSaveCostume.h"

#include "UI/Hybrid/uiHybridMenu.h"
#include "UI/Hybrid/uiBody.h"
#include "UI/uiOptions.h"

Costume *gTailoredCostume = 0;
int FreeTailorCount = 0;
bool bFreeTailor = 0;
bool bUsingDayJobCoupon = 0;
int discountPercentage;
bool bVetRewardTailor = 0;
int loadedCostume = 0;
static int revertedCostumePart = 0;
int gTailorStanceBits = 0;
extern GenderChangeMode genderEditingMode;
static void tailor_AttemptToMatch(Entity *e);
static void tailor_MarkOrigins(void);
void tailor_RevertAll(Entity *e);
static int tailor_GetDiscountedTotalCost();
static int init = 0;
static ShoppingCart sStorePartsContained;

// These z values was previously hard coded to 8000, putting the costs in front of scroll selectors
#define TAILOR_COST_Z 500

void tailor_capeFixup(Costume *costume)
{
    int j;
    for (j = 15; j <= 18; ++j)
    {
        if( !costume->parts[j]->regionName )
        {
            int i;
            const CostumeOriginSet    *cset = costume_get_current_origin_set();

            for( i = 0; i < eaSize(&cset->regionSet); i++ )
            {
                CostumeRegionSet *reg = cpp_const_cast(CostumeRegionSet*)(cset->regionSet[i]);
                CostumeBoneSet *bset = cpp_const_cast(CostumeBoneSet*)(reg->boneSet[0]);
                CostumeGeoSet *gset = 0;

                if( bset->geo ) 
                    gset = cpp_const_cast(CostumeGeoSet*)(bset->geo[0]);

                if( stricmp(reg->name, (j == 18) ? "Special" : "Capes" ) == 0 )
                {
                    reg->currentBoneSet = 0;
                    bset->currentSet = 0;
                    if(gset)gset->currentInfo = 0;
                    costume->parts[j]->regionName = reg->name;
                }
            }
        }
    }
}

void tailor_exit(int doNotReturnToGame)
{
    Entity *e = playerPtr();

    if(green_blobby_fx_id)
    {
        // handle edge case where this was created due to uninitialized costume
        fxDelete(green_blobby_fx_id, HARD_KILL);
        green_blobby_fx_id = 0;
    }

     texEnableThreadedLoading();
    toggle_3d_game_modes(SHOW_NONE);
    {
        Costume* costume_to_use = e->pl->supergroup_mode ? e->pl->superCostume : costume_current(e);
        costume_to_use->appearance.costumeFilePrefix = NULL;  //reset this so no assumptions are made about the body type
        entSetMutableCostumeUnowned( e, costume_to_use );
//        e->costume = e->pl->supergroup_mode ? costume_as_const(e->pl->superCostume) : costume_as_const(e->pl->costume[e->pl->current_costume]);
    }

    costume_Apply( e );

    if( gTailoredCostume )
        costume_destroy( gTailoredCostume );
    gTailoredCostume = NULL;

    if( gSuperCostume )
        costume_destroy( gSuperCostume );
    gSuperCostume = NULL;

    assert (gCopyOfPowerCustList);
    powerCustList_destroy(e->powerCust);
    e->powerCust = gCopyOfPowerCustList;
    gCopyOfPowerCustList = NULL;

    resetPowerCustMenu();

    genderEditingMode = 0;
    init = 0;

    //update checkbox setting
    optionSet(kUO_HideStorePiecesState, getHideStorePiecesState(), 1);

    if (!doNotReturnToGame)
    {
        start_menu( MENU_GAME );
    }
}

static int tailor_applyDiscount(int fullCost)
{
    return (fullCost * ((100 - discountPercentage) / 100.f));
}

static void tailor_Finalize(Entity *e)
{
    int total = tailor_GetDiscountedTotalCost();
    PowerCustomizationList *newPowerCustList = e->powerCust;
    int powerCustCost = powerCust_GetTotalCost(e, gCopyOfPowerCustList, newPowerCustList);

    powerCustCost = tailor_applyDiscount(powerCustCost);

    total += powerCustCost;

    if( !e->pl->npc_costume )
    {
        const char *costumeFilePrefixPtr = gTailoredCostume->appearance.costumeFilePrefix;
        gTailoredCostume->appearance.costumeFilePrefix = NULL;
         sendTailorTransaction( genderEditingMode, gTailoredCostume, newPowerCustList );
        gTailoredCostume->appearance.costumeFilePrefix = costumeFilePrefixPtr;
        if (genderEditingMode == GENDER_CHANGE_MODE_DIFFERENT_GENDER)
        {
            if( e->costume->appearance.bodytype == kBodyType_Female || e->costume->appearance.bodytype == kBodyType_BasicFemale )
                e->name_gender = e->gender = GENDER_FEMALE;
            else
                e->name_gender = e->gender = GENDER_MALE;
        }
        if(!bFreeTailor)
        {
            ent_AdjInfluence(e, -total, NULL);
        }
    }
}

//-----------------------------------------------------------------------------------------------------------------
// tailor functions ///////////////////////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------------------------------------------

// Essentially a reset function on a per-region basis
//
void tailor_RevertRegionToOriginal( const CostumeOriginSet *cset, const CostumeRegionSet *region )
{
    Entity * e = playerPtr();
    int i, j;
    Costume * costume =  costume_current(e);
    const CostumeGeoSet* selectedGeoSet = getSelectedGeoSet(COSTUME_EDIT_MODE_TAILOR);

       for( i = 0; i < e->costume->appearance.iNumParts; i++ )
    {
         if( costume->parts[i]->regionName && stricmp( region->name, costume->parts[i]->regionName ) == 0  )
        {
             gColorsLinked = FALSE;
            costume_MatchPart( e, costume->parts[i], cset, 0, bodyPartList.bodyParts[i]->name );

            for (j=0; j<ARRAY_SIZE(e->costume->parts[i]->color); j++) 
                costume_PartSetColor( costume_as_mutable_cast(e->costume), i, j, e->costume->parts[i]->colorTmp[j] );

            if( strstriConst( e->costume->parts[i]->pchTex1, "skin" ) )
                costume_SetSkinColor( costume_as_mutable_cast(e->costume), costume->appearance.colorSkin );
        }
    }
    updateCostume();
    revertedCostumePart = 1;
}

void tailor_RevertCostumePieces(Entity *e)
{
    int i;

    CostumeOriginSet *cset = cpp_const_cast(CostumeOriginSet*)(costume_get_current_origin_set());

    for( i = 0; i < e->costume->appearance.iNumParts; i++ )
    {
        if (!isNullOrNone(e->costume->parts[i]->pchGeom) || !isNullOrNone(e->costume->parts[i]->pchFxName))
            costume_MatchPart( e, costume_current(e)->parts[i], cset, 0, bodyPartList.bodyParts[i]->name );
    }

    updateCostume();
}

void tailor_RevertAll(Entity *e)
{
    int i, j;

    CostumeOriginSet *cset = cpp_const_cast(CostumeOriginSet*)(costume_get_current_origin_set());

    for( i = 0; i < e->costume->appearance.iNumParts; i++ )
    {
        if (!isNullOrNone(e->costume->parts[i]->pchGeom) || !isNullOrNone(e->costume->parts[i]->pchFxName))
        {
            for (j=0; j<ARRAY_SIZE(e->costume->parts[i]->color); j++) 
                costume_PartSetColor( costume_as_mutable_cast(e->costume), i, j, e->costume->parts[i]->colorTmp[j] );
            costume_MatchPart( e, costume_current(e)->parts[i], cset, 0, bodyPartList.bodyParts[i]->name );
        }
    }
    updateCostume();
    revertedCostumePart = 1;
}

// should only be called after costume is reset and cleared
void tailor_MarkOrigins(void)
{
    int i, j, k;
    const CostumeOriginSet *cset = costume_get_current_origin_set();

    for( i = eaSize(&cset->regionSet)-1; i >= 0; i-- )
    {
        CostumeRegionSet *rset = cpp_const_cast(CostumeRegionSet*)(cset->regionSet[i]);
        rset->origBoneSet = rset->currentBoneSet;

        for( j = eaSize(&rset->boneSet)-1; j >= 0; j-- )
        {
            CostumeBoneSet * bset = cpp_const_cast(CostumeBoneSet*)(rset->boneSet[j]);
            bset->origSet = bset->currentSet;

            for( k = eaSize(&bset->geo)-1; k >= 0; k-- )
            {
                CostumeGeoSet * gset = cpp_const_cast(CostumeGeoSet*)(bset->geo[k]);
                gset->origInfo = gset->currentInfo;
                gset->origMask = gset->currentMask;
            }
        }
    }
}

//
//
static int costume_AttemptMatchPart( Entity *e, CostumePart *cpart, CostumeOriginSet *cset, const CostumeBoneSet *currentBoneSet, int supergroup, int type, int id )
{
    int i, j, k, m, n, match = 0;
    const CostumeGeoSet* selectedGeoSet = getSelectedGeoSet(COSTUME_EDIT_MODE_TAILOR);

       for( i = 0; i < eaSize(&cset->regionSet); i++ )
    {
        if( cpart->regionName && stricmp( cpart->regionName, cset->regionSet[i]->name ) == 0 )
        {
            for( j = 0; j < eaSize( &cset->regionSet[i]->boneSet); j++ )
            {    
                CostumeBoneSet *bset = cpp_const_cast(CostumeBoneSet*)(cset->regionSet[i]->boneSet[j]);

                if( !currentBoneSet )
                {
                    if( !cpart->bodySetName || stricmp( cpart->bodySetName, bset->name ) != 0 )
                        continue;
                }
                else
                {
                    if( !cpart->bodySetName || stricmp(currentBoneSet->name, cpart->bodySetName) != 0 )
                        continue;
                }

                 for( k = 0; k < eaSize(&bset->geo); k++ )
                {
                    CostumeGeoSet *gset = cpp_const_cast(CostumeGeoSet*)(bset->geo[k]);

                    if( gset->isHidden || !gset->bodyPartName )
                        continue;

                     if( stricmp( bodyPartList.bodyParts[id]->name, gset->bodyPartName ) == 0 ) // match geoset
                    {
                        for( m = 0; m < eaSize(&gset->info); m++ )
                        {

                            if( !game_state.editnpc && gset->info[m]->devOnly )
                                continue;

                            if( type == TAILOR_FX )
                            {
                                if( stricmp( gset->info[m]->fxName, cpart->pchFxName ) == 0 )
                                {
                                    bset->currentSet = k;
                                    gset->currentInfo = m;
                                    return true;
                                }
                            }
                            if( type == TAILOR_GEO )
                            {
                                if( selectedGeoSet != gset )
                                    continue;

                                if( stricmp( gset->info[m]->geoName, cpart->pchGeom ) == 0 )
                                {
                                    int tmp = gset ? gset->currentInfo : m;
                                    bset->currentSet = k;
                                    gset->currentInfo = m;
                                    match = 1;
                                    while( m < gset->infoCount && stricmp( gset->info[m]->geoName, cpart->pchGeom ) == 0 )
                                    {
                                        if( gset && stricmp( gset->info[m]->texName1, gset->info[tmp]->texName1) == 0)
                                        {
                                            gset->currentInfo = m;
                                            return match;
                                        }
                                        m++;
                                    }
                                    
                                    return match;
                                }
                            }
                            else if( type == TAILOR_TEX)
                            {
                                if( strnicmp( cpart->pchTex2, "miniskirt", 9 ) == 0 
                                    && cpart->pchTex2[strlen(cpart->pchTex2)-1] != 'b' 
                                    &&  strnicmp( gset->info[m]->texName2, cpart->pchTex2, 11 ) == 0  ) 
                                {
                                    cpart->pchTex2 = gset->info[m]->texName2;
                                }                

                                if( !gset->info[m]->texMasked && cpart->pchTex2[strlen(cpart->pchTex2)-1] == 'b' &&
                                    stricmp( gset->info[m]->texName2, cpart->pchTex2 ) != 0 )
                                {
                                    continue;
                                }

                                if( m-1>0 && gset->info[m-1]->texMasked )
                                    continue;

                                if( stricmp( gset->info[m]->texName1, cpart->pchTex1 ) == 0 &&
                                       ( selectedGeoSet && stricmp( gset->info[m]->geoName, selectedGeoSet->info[selectedGeoSet->currentInfo]->geoName) == 0) )
                                {
                                    if( stricmp( cpart->pchTex1, "base" ) == 0 )
                                    {
                                        if(  stricmp( gset->info[m]->texName2, cpart->pchTex2 ) == 0  )
                                        {
                                            bset->currentSet = k;
                                            gset->currentInfo = m;
                                            match = 1;
                                        }
                                    }
                                    else
                                    {
                                        bset->currentSet = k;
                                        gset->currentInfo = m;
                                        match = 1;
                                    }
                                }
                            }
                            else if( type == TAILOR_GEOTEX )
                            {
                                // hackis fix for texture name change
                                if( strnicmp( cpart->pchTex2, "miniskirt", 9 ) == 0 
                                    && cpart->pchTex2[strlen(cpart->pchTex2)-1] != 'b' 
                                    &&  strnicmp( gset->info[m]->texName2, cpart->pchTex2, 11 ) == 0  ) 
                                {
                                    cpart->pchTex2 = gset->info[m]->texName2;
                                }

                                if( stricmp( gset->info[m]->texName1, cpart->pchTex1 ) == 0 && // match GeoTex Set
                                    ( isNullOrNone(cpart->pchFxName) || (gset->info[m]->fxName && stricmp( gset->info[m]->fxName, cpart->pchFxName ) == 0 ) ) &&
                                    (stricmp( gset->info[m]->geoName, cpart->pchGeom ) == 0 ) &&
                                    (stricmp( bodyPartList.bodyParts[id]->name, "ChestDetail" ) != 0 ||          // it is also a flaming hack to give the character the supergroup emblem >:|
                                    (( supergroup && stricmp( gset->info[m]->texName2, e->supergroup->emblem ) == 0 ) ||
                                    (!supergroup && stricmp( gset->info[m]->texName2, cpart->pchTex2 ) == 0 )) )
                                    )
                                {
                                    // found a match
                                    if( cpart->pchTex2[strlen(cpart->pchTex2)-1] == 'b' &&
                                        stricmp( gset->info[m]->texName2, cpart->pchTex2 ) != 0 )
                                    {
                                        continue;
                                    }

                                     bset->currentSet = k;
                                    gset->currentInfo = m;

                                    if(  stricmp( gset->info[m]->texName2, cpart->pchTex2 ) == 0  )
                                    {
                                        if(gset->info[m]->texMasked)
                                            gset->currentMask = -1;
                                        match = 1;
                                    }
    
                                    //now get the texture
                                    else if ( gset->mask ) // check the nefangled mask array first
                                    {
                                        for( n = 0; n < eaSize(&gset->mask); n++ )
                                        {
                                            if( stricmp( gset->mask[n]->name, cpart->pchTex2 ) == 0 )
                                            {
                                                gset->currentMask = n;
                                                match = 1;
                                            }
                                        }
                                    }
                                    else // ok try the old one now
                                    {
                                        for( n = 0; n < eaSize(&gset->masks); n++ )
                                        {
                                            if( stricmp( gset->masks[n], cpart->pchTex2 ) == 0 )
                                            {
                                                gset->currentMask = n;
                                                match = 1;
                                            }
                                        }
                                    }

                                } // end if found texset
                            }
                            else if ( type == TAILOR_MASK )
                            {
                                if ( gset->mask ) // check the nefangled mask array first
                                {
                                    for( n = 0; n < eaSize(&gset->mask); n++ )
                                    {
                                        if( stricmp( gset->mask[n]->name, cpart->pchTex2 ) == 0 )
                                        {
                                            gset->currentMask = n;
                                            match = 1;
                                        }
                                    }
                                }
                                else // ok try the old one now
                                {
                                    for( n = 0; n < eaSize(&gset->masks); n++ )
                                    {
                                        if( stricmp( gset->masks[n], cpart->pchTex2 ) == 0 )
                                        {
                                            gset->currentMask = n;
                                            match = 1;
                                        }
                                    }
                                }
                            }
                        } // end for every texset
                    } 
                }// end for every geoset
                
            } // end for every bone set
        }
    } // end for every region set

    return match;
}


// go through the entire costume and try to match values where possible
//
static void tailor_AttemptToMatch(Entity *e)
{
    int i;
    CostumeOriginSet *cset = cpp_const_cast(CostumeOriginSet*)(costume_get_current_origin_set());
    for( i = 0; i < e->costume->appearance.iNumParts; i++ )
    {    
        costume_AttemptMatchPart( e, costume_as_mutable_cast(e->costume)->parts[i], cset, NULL, 0, TAILOR_GEOTEX, i );
    }

}

int tailor_RevertBone( int boneID, const CostumeBoneSet *bset, int type )
{
    Entity *e = playerPtr();
    CostumeOriginSet *cset = cpp_const_cast(CostumeOriginSet*)(costume_get_current_origin_set());
    int j;
    const CostumeGeoSet* selectedGeoSet = getSelectedGeoSet(COSTUME_EDIT_MODE_TAILOR);

       int result = costume_AttemptMatchPart( e, costume_current(e)->parts[boneID], cset, bset, 0, type, boneID  );

    for (j=0; j<ARRAY_SIZE(e->costume->parts[boneID]->colorTmp); j++) 
          costume_PartSetColor( costume_as_mutable_cast(e->costume), boneID, j, e->costume->parts[boneID]->colorTmp[j] );

    if( strstriConst( e->costume->parts[boneID]->pchTex1, "skin" ) )
        costume_SetSkinColor( costume_as_mutable_cast(e->costume), costume_current(e)->appearance.colorSkin );

    revertedCostumePart = 1;
    return result;
}


// displays the cost for a given geoset or boneset
//
int tailor_displayCost( const CostumeBoneSet *bset, const CostumeGeoSet *gset, float x, float y, float z, float scale, int grey, int display )
{
    Entity *e = playerPtr();
    Costume * costume = costume_current(e);
 
    //first get the base cost
    int bsetChanged = 0, bought = 0, totalCost=0;
    int bpID=0;

    if( !gset && bset )
        gset = cpp_const_cast(CostumeGeoSet*)(bset->geo[0]);

     if( gset->isHidden )
        return 0;

    if(gset->faces)
        return 0;

    bpID = costume_PartIndexFromName( e, gset->bodyPartName );

    if( costume->parts[bpID]->regionName )
        totalCost = tailor_getSubItemCost( e, costume->parts[bpID] );

    if (!costume->parts[bpID]->bodySetName ||
        (stricmp( bset->name, costume->parts[bpID]->bodySetName ) != 0 &&
        (stricmp(costume->parts[bpID]->bodySetName, "arachnos weapons") != 0 ||    stricmp(bset->name, "weapons") != 0))) //HACK Previously we had a seperate body part name for ararchnos
    {
        bsetChanged = 1;
    }

    // now figure out if they have actually changed something
    if( !gset->info[gset->currentInfo]->texMasked &&
        (stricmp(gset->info[gset->currentInfo]->texName2, "mask" ) != 0 || (costume->parts[bpID]->pchTex2[strlen(costume->parts[bpID]->pchTex2)-1] == 'b')) &&
          (strnicmp(gset->info[gset->currentInfo]->texName2, costume->parts[bpID]->pchTex2, strlen(gset->info[gset->currentInfo]->texName2)-1 ) != 0 ))
             bought = 1; 
    if( stricmp(gset->info[gset->currentInfo]->geoName, costume->parts[bpID]->pchGeom ) != 0 )
            bought = 1;
    if( stricmp(gset->info[gset->currentInfo]->texName1, costume->parts[bpID]->pchTex1 ) != 0 )
            bought = 1;
     if( gset->info[gset->currentInfo]->fxName && costume->parts[bpID]->pchFxName && stricmp(gset->info[gset->currentInfo]->fxName, costume->parts[bpID]->pchFxName) != 0 )
            bought = 1;

      if( gset->currentMask >= 0 && (gset->info[gset->currentInfo]->texMasked || stricmp(gset->info[gset->currentInfo]->texName2, "mask" ) == 0)) 
    {
        if( eaSize( &gset->mask ) )
        {
            if( (gset->info[gset->currentInfo]->texMasked || costume->parts[bpID]->pchTex2[strlen(costume->parts[bpID]->pchTex2)-1] != 'b') && 
                stricmp(gset->mask[gset->currentMask]->name, costume->parts[bpID]->pchTex2 ) != 0 )
                bought = 1;
        }
    }
    else if(  gset->currentMask < 0 )
    {
        if( stricmp(gset->info[gset->currentInfo]->texName2, costume->parts[bpID]->pchTex2 ) != 0 )
            bought = 1;
    }

    if( gset->currentMask < 0 && gset->info[gset->currentInfo]->texMasked && 
        !strstriConst(gset->info[gset->currentInfo]->texName2, costume->parts[bpID]->pchTex2 ))
    {
        bought = 1;
    }

    if( display && !bsetChanged )
    {
        F32 xposSc, yposSc;
        F32 textXSc = 1.f;
        F32 textYSc = 1.f;
        SpriteScalingMode ssm = getSpriteScaleMode();
        calculatePositionScales(&xposSc, &yposSc);
        if (isTextScalingRequired())
        {
            calculateSpriteScales(&textXSc, &textYSc);
            setSpriteScaleMode(SPRITE_XY_NO_SCALE);
        }
          if( bought )
        {
              font( &game_12 );
            font_color( CLR_WHITE, CLR_RED );
            cprntEx( x - 40*xposSc, y, z, textXSc*scale*LEFT_COLUMN_SQUISH, textYSc*scale*LEFT_COLUMN_SQUISH, CENTER_X|CENTER_Y, "(+%i)", totalCost );
        }
        else
        {
            font( &game_12 );

            if( grey )
                font_color( CLR_GREY, CLR_GREY );
            else
                font_color( CLR_WHITE, CLR_WHITE );

               cprntEx( x - 40*xposSc, y, z, textXSc*scale*LEFT_COLUMN_SQUISH, textYSc*scale*LEFT_COLUMN_SQUISH, CENTER_X|CENTER_Y, "(+0)" );
        }
        setSpriteScaleMode(ssm);
    }

    if( bought )
        return totalCost;
    else
        return 0;
}

// displays total cost for a region
//
void tailor_DisplayRegionTotal(const char *regionName, float x, float y, float z )
{
    int totalCost = tailor_RegionCost( playerPtr(), regionName, playerPtr()->costume, 0 );
    F32 textXSc = 1.f;
    F32 textYSc = 1.f;
    SpriteScalingMode ssm = getSpriteScaleMode();
    if (isTextScalingRequired())
    {
        calculateSpriteScales(&textXSc, &textYSc);
        setSpriteScaleMode(SPRITE_XY_NO_SCALE);
    }
    font(&game_12);

    if( totalCost > 0 )
        font_color( CLR_WHITE, CLR_RED );
    else
        font_color( CLR_WHITE, CLR_WHITE );

     prnt( x, y, z, LEFT_COLUMN_SQUISH*textXSc, LEFT_COLUMN_SQUISH*textYSc, "(+%i)", totalCost );
    setSpriteScaleMode(ssm);
}

// Draws the overall total at the bottom of the screen
//
static float tailorTotalXPositions[13][3] =
{    //    normal        free tailor    discounted
    {    300.0f,        300.0f,        250.0f,    },        //0  Tailor Fee / Tailor
    {    350.0f,        350.0f,        290.0f,    },        //1  '+'
    {    400.0f,        400.0f,        330.0f,    },        //2  Changes / Powers Fee
    {    450.0f,        450.0f,        370.0f,    },        //3  '+'
    {    500.0f,        500.0f,        410.0f,    },        //4  Powers / Changes
    {    550.0f,        550.0f,        450.0f,    },        //5  '='
    {    600.0f,        640.0f,        490.0f,    },        //6  Total
    {    0.0f,        0.0f,        520.0f,    },        //7  '-'
    {    0.0f,        0.0f,        550.0f,    },        //8  Discount
    {    0.0f,        0.0f,        580.0f,    },        //9  '='
    {    0.0f,        0.0f,        620.0f,    },        //10 Total
    {    640.0f,        0.0f,        650.0f,    },        //11 |
    {    695.0f,        0.0f,        705.0f,    },        //12 Available Influence
};

static SGColorBits sgColorsU[NUM_SG_COLOR_SLOTS + 1][6];
static unsigned int currentCostumeColors[MAX_COSTUME_PARTS][4];
static int s_anyChanges;
static int hasDiscountSalvage;

int tailor_drawTotal( Entity *e, int genderChange, float screenScaleX, float screenScaleY )
{
    int tTotal = 0;
    int pTotal = 0;
    int total = 0;
    int fee = 0;
    int tFee, pFee;
    int change = 0;
    int mode;
    float xsc, ysc;
       float y = 740.0f;
    float screenScale = MIN(screenScaleX, screenScaleY);
    char *currency = NULL;
    float z = 5000.f;

    int isTailor = current_menu() == MENU_TAILOR;
    unsigned int saved_fpu_control;

    //we have to force the floating point calculation to be the same as the mapserver since the web browser requires different precision
    _controlfp_s(&saved_fpu_control, 0, 0);    // retrieve current control word
    _controlfp_s(NULL, _PC_24, _MCW_PC);    // set the fpu to 24 bit precision
    tTotal = tailor_CalcTotalCost( e, e->costume, genderChange );
    if (gCopyOfPowerCustList)
    {
        pTotal = getPowerCustCost();
    }
    tFee = tTotal ? tailor_getFee(e) : 0;
    pFee = powerCust_GetFee(e);
    _controlfp_s(NULL, saved_fpu_control, _MCW_PC);    // restore fpu precision

    //check for power cust changes (pTotal will be 0 if there are no changes)
    if (!pTotal)
    {
        pFee = 0;
    }
    if (!tTotal && !pTotal)
    {
        s_anyChanges = 0;
        discountPercentage = 0;
        bFreeTailor = 0;
    }
    else
    {
        s_anyChanges = 1;
    }

    total = tTotal + pTotal;
    fee = isTailor ? tFee : pFee;
    change = (isTailor ? tTotal : pTotal) - fee;

    if (bFreeTailor)
    {
        mode = 1;
    }
    else if (discountPercentage)
    {
        mode = 2;
    }
    else
    {
        mode = 0;
    }

      getToScreenScalingFactor(&xsc, &ysc);
 
    if (ysc * 2.0f < xsc)
    {
        xsc = ysc * 2.0f;
    }
    if (xsc < 1.3f)
    {
        xsc = xsc / 1.3f;
    }
    else
    {
        xsc = 1.0f;
    }

      font( &game_12 );
    font_color( CLR_WHITE, CLR_WHITE );
    xsc *= screenScale;
    cprnt( tailorTotalXPositions[0][mode]*screenScaleX, y*screenScaleY, z, xsc, xsc, textStd(isTailor ? "FeeString" : "TailorString") );
    cprntEx( tailorTotalXPositions[0][mode]*screenScaleX, (y+15)*screenScaleY, z, xsc, xsc, CENTER_X | NO_MSPRINT, textStd("InfluenceWithCommas", isTailor ? fee : tTotal) );

    cprntEx( tailorTotalXPositions[1][mode]*screenScaleX, (y+7)*screenScaleY, z, xsc * 2.0f, xsc * 2.0f, CENTER_X|CENTER_Y, "+" );

    cprnt( tailorTotalXPositions[2][mode]*screenScaleX, y*screenScaleY, z, xsc, xsc, textStd(isTailor ? "ChangeCost" : "PowerCustFeeString") );
    cprntEx( tailorTotalXPositions[2][mode]*screenScaleX, (y+15)*screenScaleY, z, xsc, xsc, CENTER_X | NO_MSPRINT, textStd("InfluenceWithCommas", isTailor ? change : fee) );

     cprntEx( tailorTotalXPositions[3][mode]*screenScaleX, (y+7)*screenScaleY, z, xsc * 2.0f, xsc * 2.0f, CENTER_X|CENTER_Y, "+" );

    cprnt( tailorTotalXPositions[4][mode]*screenScaleX, y*screenScaleY, z, xsc, xsc, textStd(isTailor ? "PowerCustFeeString" : "ChangeCost") );
    cprntEx( tailorTotalXPositions[4][mode]*screenScaleX, (y+15)*screenScaleY, z, xsc, xsc, CENTER_X | NO_MSPRINT, textStd("InfluenceWithCommas", isTailor ? pTotal : change) );

    cprntEx( tailorTotalXPositions[5][mode]*screenScaleX, (y+7)*screenScaleY, z, xsc * 2.0f, xsc * 2.0f, CENTER_X|CENTER_Y, "=" );

    if(bFreeTailor)
    {
           cprnt( tailorTotalXPositions[6][mode]*screenScaleX, y*screenScaleY, z, xsc, xsc, textStd("UsingFreeTailorSession", FreeTailorCount ) );
         cprnt( tailorTotalXPositions[6][mode]*screenScaleX, (y+15)*screenScaleY, z, xsc, xsc, "FeeWaived");
    }
    else
    {
        if (discountPercentage)
        {
             cprnt( tailorTotalXPositions[6][mode]*screenScaleX, y*screenScaleY, z, xsc, xsc, textStd("TotalTailorCost") );
            cprntEx( tailorTotalXPositions[6][mode]*screenScaleX, (y+15)*screenScaleY, z, xsc, xsc, CENTER_X | NO_MSPRINT, textStd("InfluenceWithCommas", total) );

            cprntEx( tailorTotalXPositions[7][mode]*screenScaleX, (y+7)*screenScaleY, z, xsc * 2.0f, xsc * 2.0f, CENTER_X|CENTER_Y, "-" );

             cprnt( tailorTotalXPositions[8][mode]*screenScaleX, y*screenScaleY, z, xsc, xsc, textStd("DiscountAmount") );
            cprntEx( tailorTotalXPositions[8][mode]*screenScaleX, (y+15)*screenScaleY, z, xsc, xsc, CENTER_X | NO_MSPRINT, "%s%%", textStd("InfluenceWithCommas", discountPercentage) );

             cprntEx( tailorTotalXPositions[9][mode]*screenScaleX, (y+7)*screenScaleY, z, xsc * 2.0f, xsc * 2.0f, CENTER_X|CENTER_Y, "=" );

            total = tailor_applyDiscount(total);
             cprnt( tailorTotalXPositions[10][mode]*screenScaleX, y*screenScaleY, z, xsc, xsc, textStd("FinalCost") );
            cprntEx( tailorTotalXPositions[10][mode]*screenScaleX, (y+15)*screenScaleY, z, xsc, xsc, CENTER_X | NO_MSPRINT, textStd("InfluenceWithCommas", total) );
        }
        else
        {
             cprnt( tailorTotalXPositions[6][mode]*screenScaleX, y*screenScaleY, z, xsc, xsc, textStd("TotalTailorCost") );
            cprntEx( tailorTotalXPositions[6][mode]*screenScaleX, (y+15)*screenScaleY, z, xsc, xsc, CENTER_X | NO_MSPRINT, textStd("InfluenceWithCommas", total) );
        }

           drawVerticalLine( tailorTotalXPositions[11][mode]*screenScaleX, (y-15)*screenScaleY, 30*screenScaleY, z, 1.f, CLR_WHITE );

         if( total > e->pchar->iInfluencePoints )
             font_color( CLR_RED, CLR_RED );

        switch( skinFromMapAndEnt(e) )
        {
            case UISKIN_HEROES:
            default:
                currency = "AvaialableInfluence";
                break;
            case UISKIN_VILLAINS:
                currency = "V_AvaialableInfluence";
                break;
            case UISKIN_PRAETORIANS:
                currency = "P_AvaialableInfluence";
                break;
        }

        cprntEx( tailorTotalXPositions[12][mode]*screenScaleX, y*screenScaleY, z, xsc, xsc, CENTER_X | NO_PREPEND, currency );
        cprntEx( tailorTotalXPositions[12][mode]*screenScaleX, (y+15)*screenScaleY, z, xsc, xsc, CENTER_X | NO_MSPRINT, textStd("InfluenceWithCommas", e->pchar->iInfluencePoints) );
    }
    return total;
}

//applies discount and will remove the fee if there is no changes
static int tailor_GetDiscountedTotalCost()
{
    Entity *e = playerPtr();
    int total = tailor_CalcTotalCost( e, e->costume, genderEditingMode );
    return tailor_applyDiscount(total);
}

int tailor_hasStoreCostumeParts()
{
    return sStorePartsContained.itemCount != 0;
}

static void drawSaveCostume()
{
    AtlasTex *saveButton = atlasLoadTexture(SELECT_FROM_UISKIN("CC_Save_H", "CC_Save_V", "CC_Save_P"));
    if (drawNLMIconButton(790, 48, 5000, 65, 20, "SaveCostume", saveButton, -5, 0))
    {
        saveCostume_start(MENU_TAILOR);
    }
}
static void drawLoadCostume()
{
    AtlasTex *loadButton = atlasLoadTexture(SELECT_FROM_UISKIN("CC_Load_H", "CC_Load_V", "CC_Load_P"));
    if (drawNLMIconButton(880, 48, 5000, 65, 20,"LoadCostume", loadButton, -5, 0))
    {
        loadCostume_start(MENU_TAILOR);
    }
}

// Nasty hack to get SG colors where we need them for tailor screen.
void tailor_pushSGColors(Costume *costume)
{
    int i;

    for (i = 0; i < NUM_SG_COLOR_SLOTS; i++)
    {
        sgColorsU[i + 1][0] = costume->appearance.superColorsPrimaryU[i];
        sgColorsU[i + 1][1] = costume->appearance.superColorsSecondaryU[i];
        sgColorsU[i + 1][2] = costume->appearance.superColorsPrimary2U[i];
        sgColorsU[i + 1][3] = costume->appearance.superColorsSecondary2U[i];
        sgColorsU[i + 1][4] = costume->appearance.superColorsTertiaryU[i];
        sgColorsU[i + 1][5] = costume->appearance.superColorsQuaternaryU[i];
    }
}

void tailor_SGColorSetSwitch(int colorSet)
{
    Entity *e = playerPtr();
    assert(colorSet >= 0 && colorSet < NUM_SG_COLOR_SLOTS + 1);
    if (gTailorColorGroup != 0)
    {
        costume_SGColorsExtract(e, gSuperCostume, &sgColorsU[gTailorColorGroup][0], &sgColorsU[gTailorColorGroup][1], &sgColorsU[gTailorColorGroup][2], 
                                                     &sgColorsU[gTailorColorGroup][3], &sgColorsU[gTailorColorGroup][4], &sgColorsU[gTailorColorGroup][5]);
        if (colorSet == 0)
        {
            e->costume = costume_as_const(gTailoredCostume);
            if (gSuperCostume)
            {
                costume_destroy(gSuperCostume);
                gSuperCostume = 0;
            }
        }
    }
    else if (colorSet != 0)
    {
        gSuperCostume = costume_clone(costume_as_const(gTailoredCostume));
        e->costume = costume_as_const(gSuperCostume);
        gTailorStanceBits = TAILOR_STANCE_PROFILE | TAILOR_STANCE_HIGH;
    }

    gTailorColorGroup = colorSet;
    costume_baseColorsApplyToCostume(gTailoredCostume, currentCostumeColors);
    if (gTailorColorGroup != 0)
    {
        costume_baseColorsApplyToCostume(gSuperCostume, currentCostumeColors);
        costume_SGColorsApplyToCostume(e, gSuperCostume, sgColorsU[gTailorColorGroup][0], sgColorsU[gTailorColorGroup][1], sgColorsU[gTailorColorGroup][2],
                                                            sgColorsU[gTailorColorGroup][3], sgColorsU[gTailorColorGroup][4], sgColorsU[gTailorColorGroup][5]);
    }
}

static void tailor_CopyColorsToTemp(int mode)
{
    int i;

    if (mode == 0)
    {
           for( i = 0; i < gTailoredCostume->appearance.iNumParts; i++ )
        {
            gTailoredCostume->parts[i]->colorTmp[0].integer = gTailoredCostume->parts[i]->color[0].integer | 0xff000000;
            gTailoredCostume->parts[i]->colorTmp[1].integer = gTailoredCostume->parts[i]->color[1].integer | 0xff000000;
            gTailoredCostume->parts[i]->colorTmp[2].integer = gTailoredCostume->parts[i]->color[2].integer | 0xff000000;
            gTailoredCostume->parts[i]->colorTmp[3].integer = gTailoredCostume->parts[i]->color[3].integer | 0xff000000;
        }
    }
    else if (gSuperCostume)
    {
           for( i = 0; i < gSuperCostume->appearance.iNumParts; i++ )
        {
            gSuperCostume->parts[i]->colorTmp[0] = gTailoredCostume->parts[i]->colorTmp[0];
            gSuperCostume->parts[i]->colorTmp[1] = gTailoredCostume->parts[i]->colorTmp[1];
            gSuperCostume->parts[i]->colorTmp[2] = gTailoredCostume->parts[i]->colorTmp[2];
            gSuperCostume->parts[i]->colorTmp[3] = gTailoredCostume->parts[i]->colorTmp[3];
        }
    }
}

// DGNOTE 9/16/2008
// Figuring these options and prices has become a totally gnarly mess.  Rather than a bunch of mixed up if tests, it has become cleaner to use a state table.
// The state table is sparse, in that not all theoretically possible rows are present, only those that either make sense, or can be legally achieved.
//    L    Vet reward    Salvage    (using)        Free token    (using)        Button text            button click line        select button state        next button state
//    0       0            0       0            0           0          <absent>                0                    <absent>                grey if rawcost > inf

//    1       0            0       0            1           0        use free token            2                    active                    grey if rawcost > inf
//    2       0            0       0            1           1        pay with inf            1                    grey if rawcost > inf    active

//    3       0            1       0            0           0        use salvage                4                    grey if salvcost > inf    grey if rawcost > inf
//    4       0            1       1            0           0        pay full cost            3                    grey if rawcost > inf    grey if salvcost > inf

//    5       0            1       0            1           0        use salvage                6                    grey if rawcost > inf    grey if rawcost > inf    
//    6       0            1       1            1           0        use free token            7                    active                    grey if salvcost > inf
//    7       0            1       0            1           1        pay full cost            5                    active                    active

//    8       1            0       0            0           0          <absent>                8                    <absent>                grey if vetcost > inf

//    9       1            0       0            1           0        use free token            10                    active                    grey if vetcost > inf
//    10       1            0       0            1           1        pay with inf            9                    grey if vetcost > inf    active

// These control the state of the next button.  I *COULD* control the state of the select button, but that would introduce even more nastiness into this.
typedef enum ButtonStateCheck
{
    kButtonState_Always,
    kButtonState_CheckRawCost,
    kButtonState_CheckSalvageDiscountCost,
    kButtonState_CheckVetDiscountCost,
} ButtonStateCheck;

static char UseFreeToken[] = "UseFreeToken";
static char PayWithInf[] = "PayInfluence";
// If anyone complains about the "Use Free Coupon" text in the button, kindly direct them to line 9733 (or thereabouts) in c:\game\data\texts\English\menuMessages.ms
static char UseCoupon[] = "UseCouponString";
static char PayFullCost[] = "PayFullCost";

typedef struct TailorPriceOption
{
    int usingSalvage;
    int usingFreeToken;
    char *buttonText;
    int newSelectIndex;
    ButtonStateCheck nextButtonStateCheck;
} TailorPriceOption;

static TailorPriceOption priceOptions[] =
{
    { 0, 0, NULL,            0,    kButtonState_CheckRawCost                },

    { 0, 0, UseFreeToken,    2,    kButtonState_CheckRawCost                },
    { 0, 1, PayWithInf,        1,    kButtonState_Always                        },

    { 0, 0, UseCoupon,        4,    kButtonState_CheckRawCost                },
    { 1, 0, PayFullCost,    3,    kButtonState_CheckSalvageDiscountCost    },

    { 0, 0, UseCoupon,        6,    kButtonState_CheckRawCost                },
    { 1, 0, UseFreeToken,    7,    kButtonState_CheckSalvageDiscountCost    },
    { 0, 1, PayFullCost,    5,    kButtonState_Always                        },


    { 0, 0, NULL,            8,    kButtonState_CheckVetDiscountCost        },

    { 0, 0, UseFreeToken,    10,    kButtonState_CheckVetDiscountCost        },
    { 0, 1, PayWithInf,        9,    kButtonState_Always                        },
};

// Starting lines determines the initial index into the above, given the presence or absence of the following three: vet reward, dayjob coupon, free token.
// Form a bitfield: vet_reward << 2 | dayjob_coupon << 1 | free_token << 0, and then use that as an index into this array.
static int startingLines[8] =
{
    0,
    1,
    3,
    5,
    8,
    9,
    8,
    9,
};

//    This is in the tailor, but the load costume will also use this
void setSeqFromStanceBits(int stance)
{
    if (stance & TAILOR_STANCE_PROFILE)
    {
        seqSetState(playerPtrForShell(1)->seq->state, TRUE, STATE_PROFILE);
    }
    if (stance & TAILOR_STANCE_HIGH)
    {
        seqSetState(playerPtrForShell(1)->seq->state, TRUE, STATE_HIGH);
    }
    if (stance & TAILOR_STANCE_COMBAT)
    {
        seqSetState(playerPtrForShell(1)->seq->state, TRUE, STATE_COMBAT);
    }
    if (stance & TAILOR_STANCE_WEAPON)
    {
        seqSetState(playerPtrForShell(1)->seq->state, TRUE, TAILOR_STATE_WEAPON);
    }
    if (stance & TAILOR_STANCE_SHIELD)
    {
        seqSetState(playerPtrForShell(1)->seq->state, TRUE, STATE_SHIELD);
    }
    if (stance & TAILOR_STANCE_RIGHTHAND)
    {
        seqSetState(playerPtrForShell(1)->seq->state, TRUE, STATE_RIGHTHAND);
    }
    if (stance & TAILOR_STANCE_EPICRIGHTHAND)
    {
        seqSetState(playerPtrForShell(1)->seq->state, TRUE, STATE_EPICRIGHTHAND);
    }
    if (stance & TAILOR_STANCE_LEFTHAND)
    {
        seqSetState(playerPtrForShell(1)->seq->state, TRUE, STATE_LEFTHAND);
    }
    if (stance & TAILOR_STANCE_TWOHAND_LARGE)
    {
        seqSetState(playerPtrForShell(1)->seq->state, TRUE, TAILOR_STATE_TWOHAND_LARGE);
    }
    if (stance & TAILOR_STANCE_TWOHAND)
    {
        seqSetState(playerPtrForShell(1)->seq->state, TRUE, STATE_TWOHAND);
    }
    if (stance & TAILOR_STANCE_DUALHAND)
    {
        seqSetState(playerPtrForShell(1)->seq->state, TRUE, STATE_DUALHAND);
    }
    if (stance & TAILOR_STANCE_GUN)
    {
        seqSetState(playerPtrForShell(1)->seq->state, TRUE, TAILOR_STATE_GUN);
    }
    if (stance & TAILOR_STANCE_KATANA)
    {
        seqSetState(playerPtrForShell(1)->seq->state, TRUE, TAILOR_STATE_KATANA);
    }
}
static int tailorPriceSelectIndex = 0;
int tailorPriceOptions_getStartingState()
{
    return startingLines[(bVetRewardTailor ? 4 : 0) | (hasDiscountSalvage ? 2 : 0) | (FreeTailorCount ? 1 : 0)];
}
int tailorPriceOptions_getNextState()
{
    return priceOptions[tailorPriceSelectIndex].newSelectIndex;
}
char *tailorPriceOptions_getButtonText()
{
    return priceOptions[tailorPriceSelectIndex].buttonText;
}
void tailorPriceOptionsSelectionUpdate()
{
    bFreeTailor = priceOptions[tailorPriceSelectIndex].usingFreeToken;;
    // I ought to make up my damn mind as to whether it's salvage or a coupon. ;)
    bUsingDayJobCoupon = priceOptions[tailorPriceSelectIndex].usingSalvage;
    discountPercentage = bVetRewardTailor ? TAILOR_VET_REWARD_DISCOUNT_PERCENTAGE : (priceOptions[tailorPriceSelectIndex].usingSalvage ? TAILOR_SALVAGE_DISCOUNT_PERCENTAGE : 0);
}
char * tailorPriceOptions_GreyPay(int cost, int influencePoints)
{
    char * greyPay = NULL;

    switch (priceOptions[tailorPriceSelectIndex].nextButtonStateCheck)
    {
    default:
        assert(0);    // This should never happen
        break;
    case kButtonState_Always:
        greyPay = 0;
        break;
    case kButtonState_CheckRawCost:
    case kButtonState_CheckSalvageDiscountCost:
    case kButtonState_CheckVetDiscountCost:
        if (cost > influencePoints)
        {
            switch( skinFromMapAndEnt(playerPtr()))
            {
            case UISKIN_HEROES:
            default:
                greyPay = "CostumeNotEnoughInfluence";
                break;
            case UISKIN_VILLAINS:
                greyPay = "CostumeNotEnoughInfamy";
                break;
            case UISKIN_PRAETORIANS:
                greyPay = "CostumeNotEnoughInformation";
                break;
            }
        }
        else
        {
            greyPay = 0;
        }
        break;
    }
    if (tailor_hasStoreCostumeParts())
    {
        greyPay = "CostumeIncomplete";
    }

    return greyPay;
}
void tailor_drawPayOptions(int rawCost, int changes, float screenScaleX, float screenScaleY)
{
    Entity *e = playerPtr();
    float screenScale = MIN(screenScaleX, screenScaleY);
    static HybridElement sButtonCoupon;
    if (tailorPriceOptions_getButtonText() && changes)
    {
        // Unilaterally enable the switch button, even if it allows a "bGreyPay" condition to be selected.  If someone bitches about this, 
        // it can be fixed.  It'll involve an extra field in the priceOptions array to control the switch button, and some additional tests
        // in the logic that sets the start index.
        sButtonCoupon.text = tailorPriceOptions_getButtonText();
        if (D_MOUSEHIT == drawHybridBarButton(&sButtonCoupon, 840.f, 745.f, 5000.f, 270.f, 0.6f, 0, HYBRID_BAR_BUTTON_NO_OUTLINE))
        {
            tailorPriceSelectIndex = tailorPriceOptions_getNextState();
        }
    }
}
static int tailorHiddenCode(void)
{
    return !gTailoredCostume;
}
static void tailorExitCode(void)
{
    const CostumeGeoSet *gset = getSelectedGeoSet(COSTUME_EDIT_MODE_TAILOR);
    init = 0;
    if (gset)
    {
        CostumeGeoSet* mutable_gset = cpp_const_cast(CostumeGeoSet*)(gset);
        mutable_gset->isOpen = false;
    }
}
NonLinearMenuElement tailor_costume_NLME = { MENU_TAILOR, { "TailorString", 0, 0 }, tailorHiddenCode, 0, 0, 0, tailorExitCode };
//-----------------------------------------------------------------------------------------------------------------
// King of the tailor functions ///////////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------------------------------------------
void tailorMenuEx(int sgMode)
{
    Entity *e = playerPtr();
    const CostumeGeoSet* selectedGeoSet = NULL;
    static int repositioningCamera = 0;
    SpriteScalingMode ssm = getSpriteScaleMode();
    float screenScaleX = 1.f, screenScaleY = 1.f, screenScale;
    F32 buttonY = 700.f;
    F32 xposSc, yposSc, xp, yp, textXSc, textYSc;

    calc_scrn_scaling(&screenScaleX, &screenScaleY);
    screenScale = MIN(screenScaleX, screenScaleY);

    // 
    //drawBackgroundAndTintSelect();

     if( !gTailoredCostume )
    {
         tailor_init(sgMode);
    }

    tailorPriceOptionsSelectionUpdate();
       toggle_3d_game_modes(SHOW_TEMPLATE);

    // TODO - bang on this a bit
    if (sgMode)
    {
        setSpriteScaleMode(SPRITE_XY_NO_SCALE);
        drawMenuTitle( 10*screenScaleX, 30*screenScaleY, 20, "ChooseSuperGroupColors", screenScale, !init );
        setSpriteScaleMode(SPRITE_XY_SCALE);
        if(drawHybridCommon(HYBRID_MENU_NO_NAVIGATION))
            return;
        setSpriteScaleMode(ssm);
    }
    else
    {
        setScalingModeAndOption(SPRITE_Y_UNIFORM_SCALE, SSO_SCALE_TEXTURE);
        calculatePositionScales(&xposSc, &yposSc);
        calculateSpriteScales(&textXSc, &textYSc);
        xp = DEFAULT_SCRN_WD / 2.f;
        yp = 0.f;
        applyPointToScreenScalingFactorf(&xp, &yp);
        updateScrollSelectors( 32, 712 );
        setScalingModeAndOption(SPRITE_XY_SCALE, SSO_SCALE_ALL);
        if(drawHybridCommon(HYBRID_MENU_TAILOR))
            return;
        setSpriteScaleMode(ssm);
    }
    if (!init)
    {
        int oldRotationAngle = playerTurn_GetTicks();
        repositioningCamera = 1;
        playerTurn(0);
        playerTurn(oldRotationAngle);
        init = 1;
    }
    if (!gTailorShowingSGColors)
    {
        setScalingModeAndOption(SPRITE_Y_UNIFORM_SCALE, SSO_SCALE_TEXTURE);
          costume_drawRegions( genderEditingMode != GENDER_CHANGE_MODE_DIFFERENT_GENDER, &sStorePartsContained );
        setScalingModeAndOption(ssm, SSO_SCALE_ALL);
    }
    else
    {
        setSpriteScaleMode(SPRITE_XY_NO_SCALE);
        font( &title_18 );
        font_color( CLR_YELLOW, CLR_ORANGE );

        if (!sgMode)
        {
            prnt( 70*screenScaleX, 95*screenScaleY, 20, 1.5f*screenScale, 1.5f*screenScale, textStd("SuperGroupStr") );
            prnt( 100*screenScaleX, 135*screenScaleY, 20, 1.5f*screenScale, 1.5f*screenScale, textStd("ColorHeading") );
        }
        
        tailor_CopyColorsToTemp(1);
        supercostumeTailorMenu(sgMode ? 1.0f : gTailorSGCostumeScale, sgMode, screenScaleX, screenScaleY);
        costume_SGColorsExtract(e, gSuperCostume, &sgColorsU[gTailorColorGroup][0], &sgColorsU[gTailorColorGroup][1], &sgColorsU[gTailorColorGroup][2], 
                                                     &sgColorsU[gTailorColorGroup][3], &sgColorsU[gTailorColorGroup][4], &sgColorsU[gTailorColorGroup][5]);
        setSpriteScaleMode(ssm);
    }
    if (loadedCostume)
    {
        costume_baseColorsExtract(costume_as_const(gTailoredCostume), currentCostumeColors, NULL);
        loadedCostume = 0;
    }
    else if (revertedCostumePart)
    {
        //    if the costume part gets reverted, dont apply the base colors or else the current colors will override 
        //    their color before the reverted color can be applied to the current colors. -EL
        revertedCostumePart = 0;
    }
    else
    {
        costume_baseColorsApplyToCostume(gTailoredCostume, currentCostumeColors);
    }
    if (gTailorColorGroup != 0)
    {
        costume_baseColorsApplyToCostume(gSuperCostume, currentCostumeColors);
        costume_SGColorsApplyToCostume(e, gSuperCostume, sgColorsU[gTailorColorGroup][0], sgColorsU[gTailorColorGroup][1], sgColorsU[gTailorColorGroup][2],
                                                            sgColorsU[gTailorColorGroup][3], sgColorsU[gTailorColorGroup][4], sgColorsU[gTailorColorGroup][5]);
    }
     costume_Apply(e);

    if (sgMode)
    {
        gTailorColorGroup = 1;
        selectedGeoSet = getSelectedGeoSet(COSTUME_EDIT_MODE_TAILOR);
    }
    else if( playerPtrForShell(0) )
    {
        setScalingModeAndOption(SPRITE_Y_UNIFORM_SCALE, SSO_SCALE_TEXTURE);
        costume_drawColorsScales( COSTUME_EDIT_MODE_TAILOR);
        if (gTailorColorGroup == 0)
        {
            tailor_CopyColorsToTemp(0);
            costume_baseColorsExtract(costume_as_const(gTailoredCostume), currentCostumeColors, NULL);
        }
        selectedGeoSet = getSelectedGeoSet(COSTUME_EDIT_MODE_TAILOR);
        setScalingModeAndOption(ssm, SSO_SCALE_ALL);
    }


    if (gTailorShowingSGColors)
    {
        setSeqFromStanceBits(gTailorStanceBits);
    }
    else
    {
        if (!gZoomed_in && selectedGeoSet && eaSize(&selectedGeoSet->bitnames))
        {
            setAnims(selectedGeoSet->bitnames);
        }
        else if (gZoomed_in && selectedGeoSet && eaSize(&selectedGeoSet->zoombitnames))
        {
            setAnims(selectedGeoSet->zoombitnames);
        }
        else
        {
             seqSetState( playerPtrForShell(1)->seq->state, TRUE, STATE_PROFILE ); // freeze the player model
            seqSetState( playerPtrForShell(1)->seq->state, TRUE, STATE_HIGH ); // freeze the player model
        }
    }

    if (repositioningCamera)
    {
        int turnedClock = 0;
        int desiredAngle = 10;
        int currentAngle = playerTurn_GetTicks();
        int diffAngle = (desiredAngle - currentAngle) % 360;
        if (diffAngle < 0)
        {
            diffAngle += 360;
        }
        if (diffAngle <= 180)
        {
            playerTurn( 5 * (TIMESTEP) );    //    clockwise 
            turnedClock = 1;
        }
        else
        {
            playerTurn( -5 * (TIMESTEP) );    //    ccw
        }

        //    If we overshot, just stop spinning
        currentAngle = playerTurn_GetTicks();
        diffAngle = (desiredAngle - currentAngle) % 360;
        if (diffAngle < 0)
        {
            diffAngle += 360;
        }

        if ((diffAngle < 180) && !turnedClock)
        {
            repositioningCamera = 0;
        }
        else if ((diffAngle >= 180) && turnedClock)
        {
            repositioningCamera = 0;
        }
    }

    if (sgMode)
    {
        setScalingModeAndOption(SPRITE_Y_UNIFORM_SCALE, SSO_SCALE_ALL);
        drawAvatarControls(870.f, 670.f, &gZoomed_in, 0);
        zoomAvatar(false, gZoomed_in ? avatar_getZoomedInPosition(0,0) : avatar_getDefaultPosition(0,0));
    }
    else
    {
        setScalingModeAndOption(SPRITE_Y_UNIFORM_SCALE, SSO_SCALE_TEXTURE);
        drawAvatarControls(xp, buttonY*yposSc, &gZoomed_in, 0);
        zoomAvatar(false, gZoomed_in ? avatar_getZoomedInPosition(0,1) : avatar_getDefaultPosition(0,1));
    }
    setScalingModeAndOption(ssm, SSO_SCALE_ALL);
    
    convergeFaceScale();
    scaleAvatar();
    
    if (sgMode)
    {
        ACButtonResult result;

        setSpriteScaleMode(SPRITE_XY_NO_SCALE);
        result = drawAcceptCancelButton(0, screenScaleX, screenScaleY);
        if (result & ACB_CANCEL)
        {
            init = 0;
            tailor_exit(1);
            srEnterRegistrationScreen();
        }
        else if (result & ACB_ACCEPT)
        {
            init = 0;
            sendSuperCostumeData(sgColorsU, e->pl->hide_supergroup_emblem, costume_getNumSGColorSlot(e));
            tailor_exit(0);
        }
        setSpriteScaleMode(ssm);
    }
    else
    {
        int rawCost;
        static HybridElement sButtonSave = {0, "SaveString", "saveCostumeButton", "icon_costume_save"};
        static HybridElement sButtonLoad = {0, "LoadString", "loadCostumeButton", "icon_costume_load"};
        int hideStorePiecesState = getHideStorePiecesState();
        const char * hideStoreTextureName = hideStorePiecesState == HIDE_STORE_PIECES_CHECKED ?  "loginscreen_checkfield_selected" : "loginscreen_checkfield";
        const char * hideStoreTooltip = hideStorePiecesState == HIDE_STORE_PIECES_CHECKED ? "showStorePiecesTooltip" : "hideStorePiecesTooltip";
        static HybridElement sButtonHideStore = {0};

        setSpriteScaleMode(SPRITE_XY_NO_SCALE);
        rawCost = tailor_drawTotal(e, genderEditingMode, screenScaleX, screenScaleY);
        setScalingModeAndOption(SPRITE_Y_UNIFORM_SCALE, SSO_SCALE_ALL);
        tailor_drawPayOptions(rawCost, s_anyChanges, screenScaleX, screenScaleY);
        if ( drawHybridButton(&sButtonSave, 820, buttonY, 5000.f, 1.f, CLR_WHITE, HB_DRAW_BACKING) == D_MOUSEHIT)
        {
            repositioningCamera = 1;
            saveCostume_start(MENU_TAILOR);
        }

        if ( drawHybridButton(&sButtonLoad, 890, buttonY, 5000.f, 1.f, CLR_WHITE, HB_DRAW_BACKING) == D_MOUSEHIT)
        {
            repositioningCamera = 1;
            loadCostume_start(MENU_TAILOR);
        }

        setScalingModeAndOption(SPRITE_Y_UNIFORM_SCALE, SSO_SCALE_TEXTURE);
        sButtonHideStore.iconName0 = hideStoreTextureName;
        sButtonHideStore.desc1 = hideStoreTooltip;
        if( D_MOUSEHIT == drawHybridButton(&sButtonHideStore, 55.f*xposSc, 640.f*yposSc, ZOOM_Z, 1.f, CLR_WHITE, 0 ))
        {
            costumeHideStorePiecesButtonPress();
        }
        //text
        setSpriteScaleMode(SPRITE_XY_NO_SCALE);
        font( &hybridbold_12 );
        font_color(CLR_WHITE, CLR_WHITE);
        prnt( 75.f*xposSc, 645.f*yposSc, ZOOM_Z, textXSc, textYSc, "hideStorePiecesButton" );

        setScalingModeAndOption(SPRITE_XY_SCALE, SSO_SCALE_ALL);
        if (drawHybridNext( DIR_LEFT, 0, "BackString" ))
        {
            if (!bodyCanBeEdited(0))
            {
                tailor_exit(0);
            }
        }
        if (tailor_hasStoreCostumeParts())
        {
            setScalingModeAndOption(SPRITE_Y_UNIFORM_SCALE, SSO_SCALE_ALL);
            drawHybridStoreAddShoppingCartMenuButton(&sStorePartsContained, 0, sStorePartsContained.itemCount, 979.f, 741.f, 5000.f, 1.f, CLR_WHITE, 0);
            setScalingModeAndOption(SPRITE_XY_SCALE, SSO_SCALE_ALL);
        }
        else
        {
            drawHybridNext( DIR_RIGHT, 0, "NextString" );
        }

        setScalingModeAndOption(ssm, SSO_SCALE_ALL);
    }
}
void tailorMenu_applyChanges()
{
    Entity *e = playerPtr();
    sendSuperCostumeData( sgColorsU, e->pl->hide_supergroup_emblem, costume_getNumSGColorSlot(e) );
    costume_baseColorsApplyToCostume(gTailoredCostume, currentCostumeColors);
    if (s_anyChanges)
    {
        tailor_Finalize(e);
    }
}
void tailorMenu()
{
    tailorMenuEx(0);
}

// creates a temporary costume, fixes world alpha and shadows
void tailor_init(int sgMode)
{
    const SalvageItem *salvageItem;
    Entity *e = playerPtr();
    int SGColorIndex = e->pl->costume[e->pl->current_costume]->appearance.currentSuperColorSet + 1;

    costumeInitHideStorePiecesState(optionGet(kUO_HideStorePiecesState));
    costume_baseColorsExtract(costume_as_const(e->pl->costume[e->pl->current_costume]), currentCostumeColors, NULL);

    costume_fillExtraData( NULL, &gCostumeMaster, false );

    texDisableThreadedLoading();
    tailor_capeFixup(e->pl->costume[e->pl->current_costume]);
    gTailoredCostume = entSetCostumeAsUnownedCopy(e, e->pl->costume[e->pl->current_costume]);

    entSetAlpha( e, 255, SET_BY_CAMERA );    
    costume_SetStartColors();

    gIgnoreLegacyForSet = 1;
    costume_setCurrentCostume( e, 0 );
    gIgnoreLegacyForSet = 0;

    gZoomed_in = 0;

    tailor_RevertAll(e);
    tailor_MarkOrigins();

    tailor_RevertScales(e);
    updateCostume();

    if (gSuperCostume != NULL)
    {
        costume_SGColorsExtract(e, gSuperCostume, &sgColorsU[SGColorIndex][0], &sgColorsU[SGColorIndex][1], &sgColorsU[SGColorIndex][2],
            &sgColorsU[SGColorIndex][3], &sgColorsU[SGColorIndex][4], &sgColorsU[SGColorIndex][5]);
        costume_destroy(gSuperCostume);
        gSuperCostume = 0;
    }
    else
    {
        memset(&sgColorsU[SGColorIndex][0], 0, 6 * sizeof(SGColorBits));
    }

    if (sgMode)
    {
        gSuperCostume = entSetCostumeAsUnownedCopy(e, gTailoredCostume);
        gTailorStanceBits = TAILOR_STANCE_PROFILE | TAILOR_STANCE_HIGH;
        sendHideEmblemChange(e->pl->hide_supergroup_emblem);
    }
    costume_reset(sgMode);
    salvageItem = salvage_GetItem(TAILOR_SALVAGE_TOKEN_NAME);
    hasDiscountSalvage = character_CanAdjustSalvage(e->pchar, salvageItem, -1);

    costume_baseColorsApplyToCostume(gTailoredCostume, currentCostumeColors);
    if (sgMode)
    {
        costume_baseColorsApplyToCostume(gSuperCostume, currentCostumeColors);
        costume_SGColorsApplyToCostume(e, gSuperCostume, sgColorsU[SGColorIndex][0], sgColorsU[SGColorIndex][1], sgColorsU[SGColorIndex][2],
            sgColorsU[SGColorIndex][3], sgColorsU[SGColorIndex][4], sgColorsU[SGColorIndex][5]);
        costume_Apply(e);
    }
    else
    {
        //add weapons if they haven't been added yet
        updateCostumeWeapons();
    }
    tailorPriceSelectIndex = tailorPriceOptions_getStartingState();

    powerCustList_tailorInit(e);
}

void tailorMenuEnterCode()
{
    //snap avatar to the correct place
    zoomAvatar(true, gZoomed_in ? avatar_getZoomedInPosition(0,1) : avatar_getDefaultPosition(0,1));

    clearSS();
}
