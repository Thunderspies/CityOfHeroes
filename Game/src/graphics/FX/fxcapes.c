#include "graphics/FX/fxcapes.h"
#include <utilitieslib/utils/textparser.h>
#include <utilitieslib/components/MemoryPool.h>
#include "seq/anim.h"
#include "render/tex.h"
#include "render/model_cache.h"
#include "seq/gfxtree.h"
#include "graphics/clothnode.h"
#include "fxutil.h"
#include "graphics/FX/fx.h"
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/fileutil.h>
#include "graphics/font.h"
#include <utilitieslib/components/genericlist.h>
#include <utilitieslib/utils/error.h>
#include "group/groupfilelib.h"
#include "cmdparse/cmdcommon.h"
#include "player/player.h"
#include "Cloth/Cloth.h"
#include "entity/entity.h"
#include "seq/tricks.h"
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/utils/strings_opt.h>
#include "graphics/FX/fxlists.h"
#include "graphics/FX/particle.h"

// This file mirrors fxbhvr.c identically in how the files are loaded/stored in .bin/etc

typedef struct FxCapeList {
    FxCape **capes;
} FxCapeList;

FxCapeList fx_capelist;

extern FxEngine fx_engine;

/*
Cape
    GeoName        Geo_Chest_Cape_01
    GeoFile        MaleCape.geo
    HarnessName Geo_Chest_CapeHarness_01
    HarnessFile MaleCape.geo

    Tex1    Chest_leather_01
    Tex2    chest_Celtic
    Color1    10 10 10
    Color2    255 10 10

    InnerTex1    Cape_01
    InnerTex2    Cape_Liner_02 #Atom_Short
    InnerColor1    160 10 10
    InnerColor2    20 10 10
End
 */

TokenizerParseInfo ParseFxCape[] = {
    { "Cape",                    TOK_IGNORE,    0}, // hack, so we can use the old list system for a bit
    { "Name",                    TOK_CURRENTFILE(FxCape,name)                },
    { "Timestamp",                TOK_TIMESTAMP(FxCape,fileAge)},
    { "GeoName",                TOK_STRING(FxCape,geoName,0)                },
    { "GeoFile",                TOK_STRING(FxCape,geoFile,0)                },
    { "HarnessName",            TOK_STRING(FxCape,harnessName,0)            },
    { "HarnessFile",            TOK_STRING(FxCape,harnessFile,0)            },
    { "Trick",                    TOK_STRING(FxCape,trickName,0)                },
    { "Color1",                    TOK_RGB(FxCape,rgb[0])                    },
    { "Color2",                    TOK_RGB(FxCape,rgb[1])                    },
    { "Color3",                    TOK_RGB(FxCape,rgb[2])                    },
    { "Color4",                    TOK_RGB(FxCape,rgb[3])                    },
    { "InnerColor1",            TOK_REDUNDANTNAME|TOK_RGB(FxCape,rgb[2])    },
    { "InnerColor2",            TOK_REDUNDANTNAME|TOK_RGB(FxCape,rgb[3])    },
    { "Tex1",                    TOK_STRING(FxCape,texture[0],0)                },
    { "Tex2",                    TOK_STRING(FxCape,texture[1],0)                },
    { "Tex3",                    TOK_STRING(FxCape,texture[2],0)                },
    { "Tex4",                    TOK_STRING(FxCape,texture[3],0)                },
    { "InnerTex1",                TOK_REDUNDANTNAME|TOK_STRING(FxCape,texture[2],0)    },
    { "InnerTex2",                TOK_REDUNDANTNAME|TOK_STRING(FxCape,texture[3],0)    },
    { "Stiffness",                TOK_F32(FxCape,stiffness,0)                },
    { "Drag",                    TOK_F32(FxCape,drag,0)                    },
    { "Scale",                    TOK_F32(FxCape,scale,0)                    },
    { "ScaleXY",                TOK_VEC2(FxCape,scaleXY)                },
    { "LODBias",                TOK_F32(FxCape,cape_lod_bias,0)            },
    { "PointYScale",            TOK_F32(FxCape,point_y_scale,0)            },
    { "ColRad",                    TOK_F32(FxCape,colrad,0)                    },

    { "IsFlag",                    TOK_BOOLFLAG(FxCape,isflag,0),                },
    { "Wind",                    TOK_STRING(FxCape,windInfoName,0)            },

    { "Collision",                TOK_STRING(FxCape,colInfoName,0)            },

    { "End",                    TOK_END,        0                                        },
    { "", 0, 0 }
};

TokenizerParseInfo ParseFxCapeList[] = {
    { "Cape",                    TOK_STRUCT(FxCapeList,capes, ParseFxCape) },
    { "", 0, 0 }
};

MP_DEFINE(FxCapeInst);

FxCapeInst *createFxCapeInst(void)
{
    MP_CREATE(FxCapeInst, 4);
    return MP_ALLOC(FxCapeInst);
}
void destroyFxCapeInst(FxCapeInst *fxCapeInst)
{
    if (!fxCapeInst)
        return;
    gfxTreeDelete(fxCapeInst->clothnode);
    MP_FREE(FxCapeInst, fxCapeInst);
}

static FxCape * fxLoadFxCape( char fname[] )
{
    FxCape * fxCape = 0;
    int         fileisgood = 0;
    TokenizerHandle tok;
    char buf[1000];

    errorLogFileIsBeingReloaded(fname);

    tok = TokenizerCreate(fname);
    if (tok) 
    {
        fxCape = (FxCape*)listAddNewMember(&fx_engine.fxCapes, sizeof(FxCape));
        assert(fxCape);
        listScrubMember(fxCape, sizeof(FxCape));
        fileisgood = TokenizerParseList(tok, ParseFxCape, fxCape, TokenizerErrorCallback);
        if( !fileisgood )
        {
            listFreeMember( fxCape, &fx_engine.fxCapes );
            fxCape = 0;
        }
        else
        {
            fxCleanFileName(buf, fxCape->name); //(buf2 will always be shorter than info->name)
            strcpy(fxCape->name, buf);
            fx_engine.fxCapecount++;
        }
    }
    TokenizerDestroy(tok);

    return fxCape;
}

static int fxCapeNameCmp(const FxCape ** info1, const FxCape ** info2 )
{
    return stricmp( (*info1)->name, (*info2)->name );
}

static int fxCapeNameCmp2(const FxCape * info1, const FxCape ** info2 )
{
    return stricmp( info1->name, (*info2)->name );
}


FxCape * fxGetFxCape( char cape_name[] )
{
    FxCape * fxCape = 0;
    char fx_name_cleaned_up[FX_MAX_PATH];
    int numcapes;
    FxCape dummy = {0};
    FxCape * * dptr;

    fxCleanFileName(fx_name_cleaned_up, cape_name);

    // Search binary of loaded fxCapes. 
    dummy.name = fx_name_cleaned_up;
    numcapes = eaSize(&fx_capelist.capes);
    dptr = bsearch(&dummy, fx_capelist.capes, numcapes, sizeof(FxCape*), fxCapeNameCmp2);
    if( dptr )
    {
        fxCape = *dptr;
        assert(fxCape);
    }

    //Failure in production mode == very bad data.

    // ## Development only 
    if (!global_state.no_file_change_check && isDevelopmentMode()) 
    {
        // always prefer the debug version to the binary version 
        {
            FxCape * dev_fxCape;
            for( dev_fxCape = fx_engine.fxCapes ; dev_fxCape ; dev_fxCape = dev_fxCape->next )
            {
                if( !_stricmp(fx_name_cleaned_up, dev_fxCape->name ) )
                {
                    fxCape = dev_fxCape;
                    break;    
                }
            }
        }

        {
            char buf[500];
            sprintf(buf, "%s%s", "fx/", fx_name_cleaned_up);

            //Reload the sysinfo from the file under these conditions
            if( !fxCape || //it's brand new fxCape
                (fxCape && fileHasBeenUpdated( buf, &fxCape->fileAge )) )  //the fxCape file itself has changed
            {
                fxCape = fxLoadFxCape(buf);
            }
        }
    }
    // ## End development only

    // ## Developnment only: if fxCape fails for any reason, try to remove it from the run time load list
    if( !fxCape )
    {
        FxCape * checkerCape;
        FxCape * checkerCape_next;

        for( checkerCape = fx_engine.fxCapes ; checkerCape ; checkerCape = checkerCape_next )
        {
            checkerCape_next = checkerCape->next;
            if( !checkerCape->name || !_stricmp( fx_name_cleaned_up, checkerCape->name ) )
            {
                listFreeMember( checkerCape, &fx_engine.fxCapes );
                fx_engine.fxCapecount--;
                assert( fx_engine.fxCapecount >= 0 );
            }
        }
    }
    //## end development only

    if (fxCape) {
        if (fxCape->windInfoName && fxCape->windInfoName[0]) {
            fxCape->windInfo = getWindInfo(fxCape->windInfoName);
        }
        if (fxCape->point_y_scale == 0 && fxCape->isflag) {
            fxCape->point_y_scale = 1.0;
        }
    }

    return fxCape;
}


bool fxCapeInitWindInfo(TokenizerParseInfo *tpi, void *structptr)
{
    FxCape * fxCape;
    char capeFilePath[MAX_PATH];
    FxCapeList *capelist = structptr?((FxCapeList *)structptr):&fx_capelist;
    bool ret = true;

    for( fxCape = fx_engine.fxCapes ; fxCape ; fxCape = fxCape->next )
    {
        if (fxCape->windInfoName && fxCape->windInfoName[0]) {
            fxCape->windInfo = getWindInfo(fxCape->windInfoName);
            if (!fxCape->windInfo) {
                sprintf(capeFilePath, "FX/%s", fxCape->name);
                ErrorFilenamef(capeFilePath, "Error finding WindInfo %s", fxCape->windInfoName);
                ret = false;
            }
        }
    }
    {
        int i;
        int numcapes = eaSize(&capelist->capes);
        for (i=0; i<numcapes; i++) {
            fxCape = capelist->capes[i];
            if (fxCape->windInfoName && fxCape->windInfoName[0]) {
                fxCape->windInfo = getWindInfo(fxCape->windInfoName);
                if (!fxCape->windInfo) {
                    sprintf(capeFilePath, "FX/%s", fxCape->name);
                    ErrorFilenamef(capeFilePath, "Error finding WindInfo %s", fxCape->windInfoName);
                    ret = false;
                }
            }
        }
    }
    return ret;
}


void fxPreloadCapeInfo()
{
    ParserLoadFiles("fx", ".cape", "capes.bin", 0, ParseFxCapeList, &fx_capelist, NULL, NULL, fxCapeInitWindInfo);

    {
        char buf2[FX_MAX_PATH];
        int num_structs, i;
        FxCape * Cape;

        //Clean Up Cape names, and sort them
        num_structs = eaSize(&fx_capelist.capes);
        for (i = 0; i < num_structs; i++)
        {
            Cape = fx_capelist.capes[i];
            fxCleanFileName(buf2, Cape->name); //(buf2 will always be shorter than info->name)
            strcpy(Cape->name, buf2);
        }
        qsort(fx_capelist.capes, num_structs, sizeof(void*), fxCapeNameCmp);
    }
}

static Model *getCapeGeometry(const char *geoname, const char *filename)
{
    char filepath[256],*filepath_ptr;
    Model *model=NULL;
    // Try player library first
    if (filename && filename[0] && !strStartsWith(filename, "object_library")) {
        sprintf( filepath, "player_library/%s", filename); 
        model = modelFind( geoname, filepath, LOAD_NOW, GEO_INIT_FOR_DRAWING | GEO_USED_BY_GFXTREE);
        if (model)
            return model;
    }

    // Try object library
    if ((filepath_ptr=objectLibraryPathFromObj( geoname ))) {
        model = modelFind( geoname, filepath_ptr, LOAD_NOW, GEO_INIT_FOR_DRAWING | GEO_USED_BY_GFXTREE);
    }
    return model;
}

void fxCapeInstInit(FxCapeInst *fxCapeInst, char *capeFile, int seqHandle, FxParams *fxp)
{
    const char *capeTextures[4];
    char capeFilePath[MAX_PATH];
    int i, j;
    extern int g_fxgeo_cape_timestamp;

    PERFINFO_AUTO_START("fxCapeInstInit", 1);
    
    fxCapeInst->updateTimestamp = g_fxgeo_cape_timestamp;

    //sprintf(capeFilePath, "FX/%s", capeFile);
    STR_COMBINE_BEGIN(capeFilePath);
    STR_COMBINE_CAT("FX/");
    STR_COMBINE_CAT(capeFile);
    STR_COMBINE_END();
    fxCapeInst->cape = fxGetFxCape(capeFile);
    if (!fxCapeInst->cape) {
        ErrorFilenamef(capeFilePath, "Could not find cape file %s", capeFilePath);
        PERFINFO_AUTO_STOP();
        return;
    }

    fxCapeInst->type = fxCapeInst->cape->isflag?CLOTH_TYPE_FLAG:CLOTH_TYPE_CAPE;

    if (fxCapeInst->cape->geoFile) {
        fxCapeInst->capeGeom = getCapeGeometry(fxCapeInst->cape->geoName, fxCapeInst->cape->geoFile);

        if( fxCapeInst->capeGeom ) {
            modelSetupVertexObject( fxCapeInst->capeGeom, VBOS_DONTUSE, BlendMode(0,0));
            if (!fxCapeInst->capeGeom->boneinfo) {
                ErrorFilenamef(capeFilePath, "Cape '%s/%s' is missing weights!", fxCapeInst->cape->geoFile, fxCapeInst->cape->geoName);
                fxCapeInst->capeGeom = NULL;
            }
        } else
            ErrorFilenamef(capeFilePath, "Could not find cape geometry '%s/%s'", fxCapeInst->cape->geoFile, fxCapeInst->cape->geoName);
    }

    if (fxCapeInst->cape->harnessFile) {
        const char *geoname=fxCapeInst->cape->harnessName;
        if (fxp->geometry && fxp->geometry[0] && stricmp(fxp->geometry, "NONE")!=0) {
            geoname = fxp->geometry;
        }
        fxCapeInst->capeHarness = getCapeGeometry(geoname, fxCapeInst->cape->harnessFile);
        if (!fxCapeInst->capeHarness && geoname!=fxCapeInst->cape->harnessName) {
            // Try harness in cape file
            geoname = fxCapeInst->cape->harnessName;
            fxCapeInst->capeHarness = getCapeGeometry(geoname, fxCapeInst->cape->harnessFile);
        }

        if( fxCapeInst->capeHarness ) {
            if (!fxCapeInst->capeHarness->boneinfo && fxCapeInst->type == CLOTH_TYPE_CAPE && isDevelopmentMode()) {
                ErrorFilenamef(capeFilePath, "Cape harness '%s/%s' is not skinned geometry!  Capes need skinned geometry (flags do not).", fxCapeInst->cape->harnessFile, geoname);
                fxCapeInst->capeHarness = NULL;
            } else {
                modelSetupVertexObject( fxCapeInst->capeHarness, VBOS_DONTUSE, BlendMode(0,0));
            }
        } else {
            if (fxp->geometry && fxp->geometry[0] && stricmp(fxp->geometry, "NONE")!=0) {
                ErrorFilenamef(capeFilePath, "Could not find cape harness '%s/%s' or '%s'", fxCapeInst->cape->harnessFile, geoname, fxp->geometry);
            } else {
                ErrorFilenamef(capeFilePath, "Could not find cape harness '%s/%s'", fxCapeInst->cape->harnessFile, geoname);
            }
        }
    }

    // Override from FxParams
    STATIC_INFUNC_ASSERT(ARRAY_SIZE(capeTextures)==ARRAY_SIZE(fxCapeInst->cape->texture));
    for (i=0; i<ARRAY_SIZE(fxCapeInst->cape->texture); i++) {
        if (fxp->textures[i] && fxp->textures[i][0] && stricmp(fxp->textures[i], "NONE")!=0) {
            capeTextures[i] = fxp->textures[i];
        } else {
            capeTextures[i] = fxCapeInst->cape->texture[i];
        }
    }

    fxCapeInst->capeFileAge = fxCapeInst->cape->fileAge;

    // Init colors
    for (i=0; i<4; i++) {
        if (i<fxp->numColors) {
            for (j=0; j<3; j++) {
                fxCapeInst->rgba[i][j] = fxp->colors[i][j];
            }
        } else {
            for (j=0; j<3; j++) {
                fxCapeInst->rgba[i][j] = fxCapeInst->cape->rgb[i][j];
            }
        }
        fxCapeInst->rgba[i][3] = 255;
    }

    //fxCapeInst->type == CLOTH_TYPE_CAPE && 
    if( ((fxCapeInst->capeGeom && fxCapeInst->capeHarness))
        && !gfxTreeNodeIsValid(fxCapeInst->clothnode, fxCapeInst->clothnodeUniqueId ) )
    {
        Vec3 capescale;
        SeqInst * seq;
        const char *seqType;
        seq = hdlGetPtrFromHandle(seqHandle);
        if (seq) {
            copyVec3(seq->currgeomscale, capescale);
#if 1
            capescale[1] *= .75f; // CLOTHFIX: Temp Hack
#endif
            seqType = seq->type->seqTypeName; // Use this so that all the things that use "fem" are treated as such
        } else {
            // No sequencer?
            // This will happen with flags, etc
            copyVec3(onevec3, capescale);
            seqType = "none";
        }
        if (fxCapeInst->cape->scale) {
            scaleVec3(capescale, fxCapeInst->cape->scale, capescale);
        }
        if (fxCapeInst->cape->scaleXY[0])
            capescale[0]*=fxCapeInst->cape->scaleXY[0];
        if (fxCapeInst->cape->scaleXY[1])
            capescale[1]*=fxCapeInst->cape->scaleXY[1];
        if (fxCapeInst->cape->colInfoName && fxCapeInst->cape->colInfoName[0]) {
            seqType = fxCapeInst->cape->colInfoName;
        }
        fxCapeInst->clothnode = initClothCapeNode( &fxCapeInst->clothnodeUniqueId, capeTextures, fxCapeInst->rgba, fxCapeInst->capeGeom, fxCapeInst->capeHarness, capescale, fxCapeInst->cape->stiffness, fxCapeInst->cape->drag, fxCapeInst->cape->point_y_scale, fxCapeInst->cape->colrad, fxCapeInst->type, seqType, fxCapeInst->cape->trickName);
        assert( gfxTreeNodeIsValid( fxCapeInst->clothnode, fxCapeInst->clothnodeUniqueId) );
        /*
        if (playerPtr() && seq == playerPtr()->seq && fxCapeInst->clothnode->flags & GFXNODE_ALPHASORT)
        {
            // Hack to draw a cape when we draw particle systems!
            ParticleSystem * system;
            extern ParticleSystem * partCreateDummySystem();
            system = partCreateDummySystem();
            system->gfxnode = fxCapeInst->clothnode;
            fxCapeInst->clothnode->clothobj->GameData->system = system;
        }
        */
        fxCapeInst->clothnode->anim_id = 198; //Says I'm a cloth node
        // TODO: need to figure out where this comes from and how it gets used
        //fxCapeInst->clothnode->seqGfxData = &seq->seqGfxData;
        copyMat4( unitmat, fxCapeInst->clothnode->mat );
    }
/*    else if ( fxCapeInst->type == CLOTH_TYPE_FLAG ) {
        fxCapeInst->clothnode = initClothFlagNode( &fxCapeInst->clothnodeUniqueId, capeTexures, fxCapeInst->rgba, onevec3, fxCapeInst->cape->stiffness, fxCapeInst->cape->drag);
        fxCapeInst->clothnode->anim_id = 198; //Says I'm a cloth node?
    }*/
    
    PERFINFO_AUTO_STOP();
}


void fxCapeInstChangeParams(FxCapeInst *fxCapeInst, FxParams *fxp)
{
    const char *capeTextures[4];
    int i, j;

    // Override from FxParams
    for (i=0; i<ARRAY_SIZE(fxCapeInst->cape->texture); i++) {
        if (fxp->textures[i] && fxp->textures[i][0] && stricmp(fxp->textures[i], "NONE")!=0) {
            capeTextures[i] = fxp->textures[i];
        } else {
            capeTextures[i] = fxCapeInst->cape->texture[i];
        }
    }

    // Init colors
    for (i=0; i<4; i++) {
        if (i<fxp->numColors) {
            for (j=0; j<3; j++) {
                fxCapeInst->rgba[i][j] = fxp->colors[i][j];
            }
        } else {
            for (j=0; j<3; j++) {
                fxCapeInst->rgba[i][j] = fxCapeInst->cape->rgb[i][j];
            }
        }
        fxCapeInst->rgba[i][3] = 255;
    }

    if( gfxTreeNodeIsValid(fxCapeInst->clothnode, fxCapeInst->clothnodeUniqueId) )
    {
        updateClothNodeParams( fxCapeInst->clothnode, capeTextures, fxCapeInst->rgba );
    }
}

int fxCapeGeoExists(const char * harnessName, char cape_name[])
{
    FxCape * cape = fxGetFxCape(cape_name);
    Model * found = cape ? getCapeGeometry(harnessName, cape->harnessFile) : NULL;

    return !!found;
}