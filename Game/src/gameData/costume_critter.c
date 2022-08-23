
#include "entity/costume.h"
#include "gameComm/VillainDef.h"
#include "gameData/costume_data.h"
#include "gameComm/Npc.h"
#include <utilitieslib/components/earray.h>
#include "seq/seq.h"
#include "seq/seqtype.h"
#include "entity/entclient.h"
#include "player/player.h"
#include "gameData/BodyPart.h"
#include "entity/costume_client.h"
#include <utilitieslib/utils/utils.h>
#include "UI/uiCostume.h"
#include "entity/entity.h"
#include "UI/uiGame.h"
#include "UI/uiKeybind.h"
#include "UI/uiAvatar.h"
#include "UI/uiGender.h"
#include "UI/Hybrid/uiHybridMenu.h"
#include "cmdparse/cmdgame.h"

#include "graphics/textureatlas.h"
#include <utilitieslib/utils/timing.h>
#include "entity/entDebugPrivate.h"
#include "entity/entDebug.h"
#include <utilitieslib/language/MessageStoreUtil.h>


CostumeMasterList    gCostumePlayer = {0};
const ColorPalette        **bodyColors;
const ColorPalette        **skinColors;

int gCostumeCritter;
char * prefix;
int gNpcIndx;
const char * gSkeleton;

void addPartToGeo( const CostumeGeoSet *gset, const CostumePart *part, const char * file_prefix );


void createEmptyGeoSet( CostumeBoneSet *bset, char * name, const cCostume * costume )
{
    CostumeGeoSet * gset = StructAllocRaw(sizeof(*gset));
    int found = 0;

    gset->name = StructAllocString(name);
    gset->displayName = StructAllocString(textStd(name));
    gset->bodyPartName = StructAllocString(name);
    gset->bsetParent = bset;

    ////for( i = 0; i < eaSize(&costume->parts); i++ )
    ////{
    ////    if( stricmp( costume->parts[i]->pchName, name ) == 0 )
    ////    {
    ////        // copy exisiting
    ////        addPartToGeo( gset, costume->parts[i], (char*)costume->appearance.costumeFilePrefix );
    ////    }
    ////}

    // make an empty one
    if( !found )
    {
        CostumeTexSet *info = StructAllocRaw(sizeof(*info));
        info->displayName = StructAllocString("None");
        info->geoName = StructAllocString("None");
        info->texName1 = StructAllocString("None");
        info->texName2 = StructAllocString("None");
        info->fxName = StructAllocString("None");
        info->gsetParent = gset;

        eaPushConst(&gset->info, info );
        gset->infoCount++;
    }

    eaPushConst(&bset->geo, gset);
    bset->geoCount++;
}

void createEmptyRegions(CostumeOriginSet *oset, const cCostume * costume)
{
    char *region_name[] = { "head", "upperBody", "lowerBody" };
    char *head_bones[] = { "head", "hair", "face", "eyedetail", "neck", "collar", "cranium", "jaw" };
    char *upper_bones[] = { "chest", "gloves", "belt", "chestdetail", "shoulders", "back", "wepr", "uarmr", "top", "sleeves" };
    char *lower_bones[] = { "pants", "boots", "broach", "cape", "aura", "skirt", };

    int i;
    for( i = 0; i < ARRAY_SIZE(region_name); i++ )
    {
        int j;

        CostumeRegionSet *rset = StructAllocRaw(sizeof(*rset));
        CostumeBoneSet *bset = StructAllocRaw(sizeof(*bset));

        rset->name = StructAllocString(region_name[i]);
        rset->displayName = StructAllocString(textStd(region_name[i]) );

        bset->name = StructAllocString("All");
        bset->displayName = StructAllocString("All");
        bset->rsetParent = rset;

        if( !i )
        {
            for( j = 0; j < ARRAY_SIZE(head_bones); j++ )
                createEmptyGeoSet( bset, head_bones[j], costume );
        }
        else if( i == 1 )
        {
            for( j = 0; j < ARRAY_SIZE(upper_bones); j++ )
                createEmptyGeoSet( bset, upper_bones[j], costume );
        }
        else
        {
            for( j = 0; j < ARRAY_SIZE(lower_bones); j++ )
                createEmptyGeoSet( bset, lower_bones[j], costume );
        }

        eaPushConst(&rset->boneSet, bset);
        rset->boneCount++;

        eaPushConst(&oset->regionSet, rset);
    }
}

const CostumeBodySet *addOrFindBodySet( const char * name, const cCostume * costume )
{
    CostumeBodySet * bodyset;
    CostumeOriginSet * originset;
    int i;

    if(!gCostumeMaster.bodySet)
        eaCreateConst(&gCostumeMaster.bodySet);

    for( i = eaSize(&gCostumeMaster.bodySet)-1; i >= 0; i-- )
    {
        if( stricmp( name, gCostumeMaster.bodySet[i]->name ) == 0 )
            return gCostumeMaster.bodySet[i];
    }

    bodyset = StructAllocRaw(sizeof(*bodyset));
    originset = StructAllocRaw(sizeof(*originset));

    bodyset->name = StructAllocString(name);
    originset->name = StructAllocString("All");

    originset->bodyColors = bodyColors;
    originset->skinColors = skinColors;
    createEmptyRegions(originset, costume);

    eaPushConst(&bodyset->originSet, originset);
    bodyset->originCount++;

    eaPushConst(&gCostumeMaster.bodySet, bodyset );
    gCostumeMaster.bodyCount++;

    return bodyset;
}

void addPartToGeo( const CostumeGeoSet *gset, const CostumePart *part, const char * prefix_file )
{
    int i;
    CostumeTexSet * info;
    char geo_name[2000];
    char base_geo_name[2000];
    const BodyPart * bp = bpGetBodyPartFromName(gset->bodyPartName);
    CostumeGeoSet* mutable_gset;

    // first look for match
    for(i = 0; i < eaSize(&gset->info); i++ )
    {
        if( (!part->pchGeom || strstriConst(gset->info[i]->geoName, part->pchGeom)) &&
            (!part->pchTex1 || stricmp(gset->info[i]->texName1, part->pchTex1) == 0) &&
            (!part->pchTex2 || stricmp(gset->info[i]->texName2, part->pchTex2) == 0) &&
            (!part->pchFxName || stricmp(gset->info[i]->fxName, part->pchFxName) == 0) )
        {
            return;
        }
    }

    info = StructAllocRaw(sizeof(CostumeTexSet));

    if( part->pchFxName )
    {
        info->displayName = StructAllocString(part->pchFxName);
        info->fxName = StructAllocString(part->pchFxName);
    }
    else
    {
        info->fxName = StructAllocString("none");
    }

    if( part->pchTex2 && !stricmp("none",part->pchTex2)==0 )
    {
        info->displayName = StructAllocString(part->pchTex2);
        info->texName2 = StructAllocString(part->pchTex2);
    }
    else
    {
        info->texName2 = StructAllocString("none");
    }

    if( part->pchTex1 && !stricmp("none",part->pchTex1)==0  )
    {
        info->displayName = StructAllocString(part->pchTex1);
        info->texName1 = StructAllocString(part->pchTex1);
    }
    else
    {
        info->texName1 = StructAllocString("none");
    }

    if( part->pchGeom && !stricmp("none",part->pchGeom)==0  )
    {
        if( strstri( (char*)part->pchGeom, ".geo/" ) )
        {
            info->geoName = StructAllocString(part->pchGeom);
            info->displayName = StructAllocString(part->pchGeom);
        }
        else
        {
            if( stricmp(bp->name_geo, "LArm") == 0 )
                sprintf( base_geo_name, "%s_%s.geo/GEO_Larm*", prefix_file, bp->base_var);
            else if( stricmp(bp->name_geo, "LLeg") == 0 )
                sprintf( base_geo_name, "%s_%s.geo/GEO_Lleg*", prefix_file, bp->base_var );
            else
                sprintf( base_geo_name, "%s_%s.geo/GEO_%s", prefix_file, bp->base_var, bp->name_geo );

            if( bodyPartIsLinkedToChest( bp ) )
                sprintf( geo_name, "%s", part->pchGeom );
            else
                sprintf( geo_name, "%s_%s", base_geo_name, part->pchGeom );

            info->geoName = StructAllocString(geo_name);
            info->displayName = StructAllocString(geo_name);
            
        }
    }
    else
    {
        info->geoName = StructAllocString("none");
    }

    info->gsetParent = gset;

    mutable_gset = cpp_const_cast(CostumeGeoSet*)(gset);
#if CLIENT
    if (!mutable_gset->hb)
        mutable_gset->hb = calloc(1,sizeof(HybridElement));
#endif
    eaPushConst(&gset->info, cpp_const_cast(CostumeTexSet*)(info));
    mutable_gset->infoCount++;
}

void addOrFindCostumePart( const CostumeBodySet *bset, const CostumePart *part, const char * file_prefix )
{
    int i, j;

    for( i = 0; i < eaSize(&bset->originSet[0]->regionSet); i++ )
    {
        const CostumeRegionSet * rset = bset->originSet[0]->regionSet[i];
        
        for( j = 0; j < eaSize(&rset->boneSet[0]->geo); j++ )
        {
            const CostumeGeoSet * gset = rset->boneSet[0]->geo[j];
            if( stricmp( part->pchName, gset->bodyPartName ) == 0 )
            {
                addPartToGeo( gset, part, file_prefix );
            }
        }
    }
}

const char * getBaseSkeletonName( const NPCDef * npcdef )
{
    SeqType *seqType = seqTypeFind( npcdef->costumes[0]->appearance.entTypeFile);
    if( seqType )
    {
        const SeqInfo *info = seqGetSequencer( seqType->seqname,SEQ_LOAD_HEADER_ONLY,0);
        if( info )
        {
            const TypeP * typeP = seqGetTypeP( info, seqType->seqTypeName );
            if( typeP )
                return typeP->baseSkeleton;
        }
    }

    return NULL;
}


void addCritterCostume( const cCostume * costume, const char * name )
{
    const CostumeBodySet * bodyset = addOrFindBodySet( name, costume );
    if( bodyset )
    {
        int i;
        for( i = eaSize( &costume->parts)-1; i>=0; i-- )
            addOrFindCostumePart( bodyset, costume->parts[i], costume->appearance.costumeFilePrefix );
    }
}

void addGeoSetToGeoSet(const CostumeGeoSet * src, CostumeGeoSet * dest )
{
    int x, i;

    for( x = 0; x < eaSize( &src->info ); x++ )
    {
        int match = 0;
        for(i = 0; i < eaSize(&dest->info); i++ )
        {
            if( (!src->info[x]->geoName || stricmp(dest->info[i]->geoName,  src->info[x]->geoName ) == 0) &&
                (!src->info[x]->texName1|| stricmp(dest->info[i]->texName1, src->info[x]->texName1) == 0) &&
                (!src->info[x]->texName2|| stricmp(dest->info[i]->texName2, src->info[x]->texName2) == 0) &&
                (!src->info[x]->fxName  || stricmp((dest->info[i]->fxName?dest->info[i]->fxName:""),   src->info[x]->fxName  ) == 0) )
            {
                match = 1;
            }
        }

        if(!match)
        {
            eaPushConst(&dest->info, src->info[x]);
            dest->infoCount++;
        }
    }

    for( x = 0; x < eaSize( &src->mask ); x++ )
    {
        int match = 0;
        for(i = 0; i < eaSize(&dest->mask); i++ )
        {
            if( stricmp(src->mask[x]->name, dest->mask[i]->name) == 0) 
                match = 1;
        }

        if(!match)
        {
            eaPushConst(&dest->mask, src->mask[x]);
            dest->maskCount++;
        }
    }


}

void addGeoSet( const CostumeGeoSet * src )
{
    int i,j;

    for( i = 0; i < eaSize(&gCostumeMaster.bodySet[0]->originSet[0]->regionSet); i++ )
    {
        const CostumeRegionSet * rset = gCostumeMaster.bodySet[0]->originSet[0]->regionSet[i];
        for( j = 0; j < eaSize(&rset->boneSet[0]->geo); j++ )
        {
            CostumeGeoSet * gset = cpp_const_cast(CostumeGeoSet*)( rset->boneSet[0]->geo[j] );
            if( src->bodyPartName && stricmp(src->bodyPartName, gset->bodyPartName ) == 0 )
            {
                 addGeoSetToGeoSet( src, gset );
                if( stricmp( gset->bodyPartName, "cape" ) == 0 )
                    gset->numColor = 4;
            }
        }
    }


}


void addGenderCostume( const CostumeBodySet *  bodyset )
{
    int i,j,k;

    for( i = 0; i < eaSize(&bodyset->originSet[0]->regionSet); i++ )
    {
        for( j = 0; j < eaSize(&bodyset->originSet[0]->regionSet[i]->boneSet); j++ )
        {
            for( k = 0; k < eaSize(&bodyset->originSet[0]->regionSet[i]->boneSet[j]->geo); k++ )
            {
                addGeoSet( bodyset->originSet[0]->regionSet[i]->boneSet[j]->geo[k] );
            }
        }
    }
}

void loadCritterCostume( const NPCDef *npcDef)
{
    if (npcDef)
    {
        int i,j;
        const char * villain_skel = getBaseSkeletonName( npcDef );
        i = j = 0;
        // Create the NPC with the correct body.
        changeBody(playerPtrForShell(1), npcDef->costumes[0]->appearance.entTypeFile);

        bodyColors = gCostumeMaster.bodySet[0]->originSet[0]->bodyColors;
        skinColors = gCostumeMaster.bodySet[0]->originSet[0]->skinColors;

        // Don't free data, we leave it around (leak like crazy)
        // @todo SHAREDMEM really????
        memset(&gCostumeMaster, 0, sizeof(gCostumeMaster));

        addCritterCostume( npcDef->costumes[0], villain_skel );

        gCostumeCritter = 1;
        gCurrentBodyType = 0;

        for( i = 0; i < eaSize(&gCostumeMaster.bodySet[0]->originSet[0]->regionSet); i++ )
        {
            const CostumeBoneSet *bset = gCostumeMaster.bodySet[0]->originSet[0]->regionSet[i]->boneSet[0];
            j = 0;
            while( j < eaSize(&bset->geo) )
            {
                if( eaSize(&bset->geo[j]->info)<=1 )
                    eaRemoveConst(&bset->geo, j);
                else
                    j++;
            }
        }
    }
}

void loadCritterCostumes( const char * npc_name )
{
    int i, j;

    //VillainDef *villain = villainFindByName(villain_name);
    //if( villain )
    //{
    //    VillainLevelDef *levelDef = GetVillainLevelDef( villain, level );
    //    if( levelDef )
    //    {
    const NPCDef* npcDef = npcFindByName(npc_name, NULL);
    if (npcDef)
    {
        const char * villain_skel = getBaseSkeletonName( npcDef );

        // Create the NPC with the correct body.
        changeBody(playerPtrForShell(1), npcDef->costumes[0]->appearance.entTypeFile);


        if(!gCostumePlayer.bodySet)
            loadCostume(&gCostumePlayer, NULL);
            

        bodyColors = gCostumeMaster.bodySet[0]->originSet[0]->bodyColors;
        skinColors = gCostumeMaster.bodySet[0]->originSet[0]->skinColors;

        // Don't free data, we leave it around (leak like crazy)
        memset(&gCostumeMaster, 0, sizeof(gCostumeMaster));

        for( i = eaSize( &npcDefList.npcDefs )-1; i >= 0; i-- )
        {
            for( j = eaSize( &npcDefList.npcDefs[i]->costumes )-1; j >= 0; j-- )
            {
                const char * npc_skel = getBaseSkeletonName(npcDefList.npcDefs[i]);
                if( npc_skel && stricmp( villain_skel, npc_skel  ) == 0 )
                    addCritterCostume( npcDefList.npcDefs[i]->costumes[j], villain_skel );
            }
        }

        // if its a match, add all of the hero items now too
        if( stricmp( villain_skel, "male/skel_ready2" ) == 0 )
        {
            addGenderCostume( gCostumePlayer.bodySet[0] );
            setGender( playerPtrForShell(0), "male" );
            gCurrentGender = AVATAR_GS_MALE;
        }

        if( stricmp( villain_skel, "fem/skel_ready2" ) == 0 )
        {
            addGenderCostume( gCostumePlayer.bodySet[1] );
            setGender( playerPtrForShell(0), "fem" );
            gCurrentGender = AVATAR_GS_FEMALE;
        }

        if( stricmp( villain_skel, "huge/skel_ready" ) == 0 )
        {
            addGenderCostume( gCostumePlayer.bodySet[2] );
            setGender( playerPtrForShell(0), "huge" );
            gCurrentGender = AVATAR_GS_HUGE;
        }

        gCostumeCritter = true;
        gCurrentBodyType = 0;

        for( i = 0; i < eaSize(&gCostumeMaster.bodySet[0]->originSet[0]->regionSet); i++ )
        {
            const CostumeBoneSet *bset = gCostumeMaster.bodySet[0]->originSet[0]->regionSet[i]->boneSet[0];
            j = 0;
            while( j < eaSize(&bset->geo) )
            {
                if( eaSize(&bset->geo[j]->info)<=1 )
                    eaRemoveConst(&bset->geo, j);
                else
                    j++;
            }
        }
    }
}

void findCritterCostume( const NPCDef *npcdef )
{
    int a,x,y,i,j,k;
    Entity * e = playerPtrForShell(1);

    for( x = 0; x < eaSize(&bodyPartList.bodyParts); x++ )
    {
        const BodyPart * bp = bodyPartList.bodyParts[x];

        for( i = 0; i < eaSize(&gCostumeMaster.bodySet[0]->originSet[0]->regionSet); i++ )
        {
            const CostumeRegionSet * rset = gCostumeMaster.bodySet[0]->originSet[0]->regionSet[i];
            for( j = 0; j < eaSize(&rset->boneSet[0]->geo); j++ )
            {
                const CostumeGeoSet * gset = rset->boneSet[0]->geo[j];
                if( stricmp( bp->name, gset->bodyPartName ) == 0 )
                {
                    int exists = 0;
                      for( y = 0; y < eaSize(&npcdef->costumes[0]->parts); y++ )
                    {
                        const CostumePart *part = npcdef->costumes[0]->parts[y];
                        if(stricmp(bp->name, part->pchName ) == 0)
                        {
                            for( a = 0; a < eaSize(&gset->info); a++ )
                            {
                                const CostumeTexSet *info = gset->info[a];

                                if( (!part->pchGeom || strstriConst(info->geoName, part->pchGeom)) &&
                                    (!part->pchTex1 || stricmp(info->texName1, part->pchTex1)==0) &&
                                     (!part->pchTex2 || stricmp(info->texName2, part->pchTex2)==0) &&
                                    ( 
                                      (!part->pchFxName && (!info->fxName||stricmp(info->fxName,"none")==0)) || 
                                      (part->pchFxName && info->fxName && stricmp(info->fxName, part->pchFxName)==0) 
                                    )
                                  )
                                {
                                    (cpp_const_cast(CostumeGeoSet*)(gset))->currentInfo = a;
                                    exists = true;

                                    for( k = 0; k < 4; k++ )
                                        costume_PartSetColor(costume_as_mutable_cast(e->costume), x, k, part->color[k] );
                                }
                            }
                        }
                    }
                    if( !exists )
                        (cpp_const_cast(CostumeGeoSet*)(gset))->currentInfo = 0;
                }
            }
        }
    }

     gColorsLinked = 0;
     costume_SetSkinColor(costume_as_mutable_cast(e->costume), npcdef->costumes[0]->appearance.colorSkin );
    changeBody(e, npcdef->costumes[0]->appearance.entTypeFile);
}


void setCritterCostume( int index )
{
    const char * skel_name;

    if( index == gNpcIndx )
        return;

    if (debug_state.menuItem) {
        freeDebugMenuItem(debug_state.menuItem);
        debug_state.menuItem = NULL;
        unbindKeyProfile(&ent_debug_binds_profile);
    }

    start_menu( MENU_COSTUME );
    skel_name = getBaseSkeletonName( npcDefList.npcDefs[index] );

    if( !gCostumeCritter || !gSkeleton || stricmp(gSkeleton,skel_name) != 0 )
        loadCritterCostumes( npcDefList.npcDefs[index]->name );

    findCritterCostume( npcDefList.npcDefs[index] );

    gSkeleton = skel_name;
    gNpcIndx = index;
    updateCostume();
}

void cycleCritterCostumes(int forward, int next_skel)
{
    const char * skel_name;
    
     if( forward )
    {
        do
        {
            if( ++gNpcIndx >= eaSize(&npcDefList.npcDefs) )
                gNpcIndx = 0;
            skel_name = getBaseSkeletonName( npcDefList.npcDefs[gNpcIndx] );
            
        }while( !npcDefList.npcDefs[gNpcIndx]->costumes[0]->parts || (gSkeleton && (!skel_name || ((!next_skel && stricmp(gSkeleton,skel_name) != 0) || (next_skel && stricmp(gSkeleton,skel_name) == 0) )) ) );
    }
    else
    {
        do
        {
            if( --gNpcIndx < 0 )
                gNpcIndx = eaSize(&npcDefList.npcDefs)-1;
            skel_name = getBaseSkeletonName( npcDefList.npcDefs[gNpcIndx] );
        }while( !npcDefList.npcDefs[gNpcIndx]->costumes[0]->parts || (gSkeleton && (!skel_name || ((!next_skel && stricmp(gSkeleton,skel_name) != 0) || (next_skel && stricmp(gSkeleton,skel_name) == 0) ) )) );
    }

    skel_name = getBaseSkeletonName( npcDefList.npcDefs[gNpcIndx] );
     
    if( !gCostumeCritter || !gSkeleton || stricmp(gSkeleton,skel_name) != 0 )
        loadCritterCostumes( npcDefList.npcDefs[gNpcIndx]->name );

    findCritterCostume( npcDefList.npcDefs[gNpcIndx] );

     gSkeleton = skel_name;
    updateCostume();

}