

#include "entity/costume.h"
#include "gameData/costume_data.h"

#include <utilitieslib/components/earray.h>
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/file.h>
#include <utilitieslib/utils/error.h>
#include "UI/uiCostume.h"

CostumeMasterList    gCostumeOriginal = {0};

const char *bodyname;
const char *origname;
const char *regname;
const char *bonename;
const char *geoname;

int safe_stricmp( const char * str1, const char * str2 )
{
    if( !str1 && !str2 )
        return 0;

    if( str1 && str2 )
        return stricmp( str1, str2 );
    else
        return 1;
}

static int matchInfo( const CostumeTexSet * info1, const CostumeTexSet * info2 )
{
    if( safe_stricmp( info1->geoName, info2->geoName ) == 0 &&
        safe_stricmp( info1->fxName, info2->fxName ) == 0 &&
        safe_stricmp( info1->texName1, info2->texName1 ) == 0 &&
         safe_stricmp( info1->texName2, info2->texName2 ) == 0 )
    {
        char str[1024] = {0};

        if( info1->geoName )
            strcatf( str, "Geo: %s ", info1->geoName );
        if( info1->fxName )
            strcatf( str, "Fx: %s ", info1->fxName );        
        if( info1->texName1 )
            strcatf( str, "Tex1: %s ", info1->texName1 );
        if( info1->texName2 )
            strcatf( str, "Tex2: %s", info1->texName2 );

        if( !info1->cohOnly && info2->cohOnly )
            return 0;
        if( !info1->covOnly && info2->covOnly )
            return 0;
        if( !info1->devOnly && info2->devOnly )
            return 0;
        if( !partIsLegacy(info1) && partIsLegacy(info2) )
            return 0;

        return 1;
    }
    return 0;
}
static int matchMask( const CostumeMaskSet * mask1, const CostumeMaskSet * mask2 )
{
    if( safe_stricmp( mask1->name, mask2->name ) == 0 )
    {

        if( !mask1->cohOnly && mask2->cohOnly )
            return 0;
        if( !mask1->covOnly && mask2->covOnly )
            return 0;
        if( !mask1->devOnly && mask2->devOnly )
            return 0;
        if( !maskIsLegacy(mask1) && maskIsLegacy(mask2) )
            return 0;

        return 1;
    }
    return 0;
}

static int findMatching( const CostumeTexSet * info, const CostumeMaskSet * mask, const CostumeBodySet *body, const CostumeOriginSet *orig, 
                             const CostumeRegionSet *reg, const CostumeBoneSet *bone, const CostumeGeoSet *geo )
{
    int i, j, k, x, y, z, bFoundMatch = 0, bFoundBoneSet = 0;
    const CostumeBodySet *bodyset;
    const CostumeOriginSet *oset;
    const CostumeRegionSet *rset;
    const CostumeBoneSet *bset;
    const CostumeGeoSet *gset;

    //if( info && (info->devOnly||info->covOnly) )
    //    return;
    //if( mask && (mask->devOnly||mask->covOnly) )
    //    return;

      for( i = 0; i < gCostumeMaster.bodyCount; i++ )
    {
        bodyset = gCostumeMaster.bodySet[i];
        if ( safe_stricmp( body->name, bodyset->name ) != 0 )
            continue;

        for( j = 0; j < bodyset->originCount; j++ )
        {
            oset = bodyset->originSet[j];
            if ( safe_stricmp( orig->name, oset->name ) != 0 )
                continue;

            for( k = 0; k < oset->regionCount; k++ )
            {
                rset = oset->regionSet[k];
                if ( safe_stricmp( reg->name, rset->name ) != 0 )
                    continue;

                for( x = 0; x < rset->boneCount; x++ )
                {
                    bset = rset->boneSet[x];
                    if ( safe_stricmp( bone->name, bset->name ) != 0 )
                        continue;

                    bFoundBoneSet = 1;
                    for( y = 0; y < bset->geoCount; y++)
                    {
                        gset = bset->geo[y];
                        if ( safe_stricmp( geo->bodyPartName, gset->bodyPartName ) != 0 )
                            continue;

                        if( info )
                        {
                            for( z = 0; z < gset->infoCount; z++ )
                                bFoundMatch |= matchInfo( info, gset->info[z] );
                        }
                        
                        if( mask )
                        {
                            for( z = 0; z < gset->maskCount; z++ )
                                bFoundMatch |= matchMask( mask, gset->mask[z] );
                        }
                    }
                }
            }
        }
    }

    if(bFoundBoneSet && !bFoundMatch)
    {
        if( info )
        {
            char str[1024] = {0};
    
            if( info->geoName )
                strcatf( str, "Geo: %s ", info->geoName );
            if( info->fxName )
                strcatf( str, "Fx: %s ", info->fxName );        
            if( info->texName1 )
                strcatf( str, "Tex1: %s ", info->texName1 );
            if( info->texName2 )
                strcatf( str, "Tex2: %s", info->texName2 );

            printf( "Info (%s): %s->%s->%s->%s->%s\n", str, bodyname, origname, regname, bonename, geoname );
        }

        if( mask )
        {
            printf( "Mask (Tex: %s): %s->%s->%s->%s->%s\n", mask->name, bodyname, origname, regname, bonename, geoname );
        }

        return 0;
    }

    return 1;
}

//-------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------

int costumeReportDiff( int flags )
{
    int i, j, k, x, y, z, diff_failed = 0;

    const CostumeBodySet *bodyset;
    const CostumeOriginSet *oset;
    const CostumeRegionSet *rset;
    const CostumeBoneSet *bset;
    const CostumeGeoSet *gset;
    char data_dir[1000];
    char valid_data_dir[1000];

    // Don't free data, we leave it around (leak like crazy)
     memset(&gCostumeOriginal, 0, sizeof(gCostumeOriginal));
    strcpy( data_dir, fileDataDir() );
    sprintf( valid_data_dir, "%s/menu/CostumeValid", data_dir );
    fileSetDataDirHack( valid_data_dir );

      if(!loadCostume(&gCostumeOriginal, NULL))
        return 1;

    printf("******BEGIN COSTUME DIFF******\n");

       for( i = 0; i < gCostumeOriginal.bodyCount; i++ )
    {
        bodyset = gCostumeOriginal.bodySet[i];
        bodyname = bodyset->name;

        if( safe_stricmp( bodyname, "Male" ) != 0 &&
            safe_stricmp( bodyname, "Female" ) != 0 &&
            safe_stricmp( bodyname, "Huge" ) != 0 )
        {
            continue;
        }

        printf("\n");

          for( j = 0; j < bodyset->originCount; j++ )
        {
            oset = bodyset->originSet[j];
            origname = oset->name;
            for( k = 0; k < oset->regionCount; k++ )
            {
                rset = oset->regionSet[k];
                regname = rset->name;
                for( x = 0; x < rset->boneCount; x++ )
                {
                    bset = rset->boneSet[x];
                    bonename = bset->name;
                    if( bset->devOnly || bset->covOnly )
                        continue; 

                    for( y = 0; y < bset->geoCount; y++)
                    {
                        gset = bset->geo[y];
                         geoname = gset->bodyPartName;

                        if( gset->devOnly || gset->covOnly )
                            continue; 

                        for( z = 0; z < gset->infoCount; z++ )
                        {
                            if( !findMatching( gset->info[z], NULL, bodyset, oset, rset, bset, gset ) )
                                diff_failed = true;
                        }

                        for( z = 0; z < gset->maskCount; z++ )
                        {
                            if( !findMatching( NULL, gset->mask[z], bodyset, oset, rset, bset, gset ) )
                                diff_failed = true;
                        }
                    }
                }
            }
        }
    }

    printf("******END COSTUME DIFF******\n");

    if( diff_failed )
        FatalErrorf( "Costume Diff Failed.  Artists Must Fix!  Erros displayed on console." );


    if( !diff_failed && flags == 2 ) // build version
    {
        char buffer[1000];

        // delete good version
        sprintf( buffer, "rd %s\\menu\\costume /s /q", backSlashes(valid_data_dir) );
        system( buffer );

        // copy new version to good version
        sprintf( buffer, "xcopy %s\\menu\\costume %s\\menu\\costume /s /i ", backSlashes(data_dir),  backSlashes(valid_data_dir) );
        system( buffer );
    }

    return diff_failed;
}