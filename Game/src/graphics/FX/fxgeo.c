#define RT_ALLOW_VBO    // RDRFIX
#include "graphics/FX/fxgeo.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <utilitieslib/stdtypes.h>
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/utils/error.h>
#include "render/model.h"
#include "render/model_cache.h"
#include <utilitieslib/utils/memcheck.h>
#include "assert.h" 
#include <utilitieslib/utils/utils.h>
#include "seq/gfxtree.h"
#include "graphics/FX/particle.h" 
#include "cmdparse/cmdcommon.h" //TIMESTEP
#include "graphics/camera.h" 
#include "assert.h"   
#include "player/player.h" 
#include <utilitieslib/utils/file.h>
#include "graphics/font.h"
#include "fxutil.h"
#include "graphics/FX/fxlists.h"
#include "group/groupfilelib.h"
#include "render/rendertricks.h"
#include "render/rendertree.h"
#include "graphics/FX/fx.h"
#include <utilitieslib/components/genericlist.h>
#include "graphics/light.h"
#include "seq/seqanimate.h"
#include "sound/sound.h"
#include <utilitieslib/components/MemoryPool.h>
#include "cmdparse/cmdgame.h"
#include <utilitieslib/components/earray.h>
#include "gridcoll/gridcoll.h"
#include <utilitieslib/utils/strings_opt.h>
#include "graphics/gfx.h" //for gfxsetviewmat
#include "graphics/groupdraw.h"
#include "render/rendershadow.h"
#include "graphics/FX/fxcapes.h"
#include "render/renderbonedmodel.h"
#include "graphics/clothnode.h"
#include <utilitieslib/utils/rgb_hsv.h>
#include "seq/gfxtree.h"
#include "seq/seqsequence.h"
#include "seq/tricks.h"
#include "render/texWords.h"
#include "render/tex.h"
#include "entity/entity.h"
#include "seq/seqskeleton.h"
#include "graphics/seqgraphics.h"
#include "group/grouptrack.h"
#include "group/groupfileload.h"
#include "graphics/groupMiniTrackers.h"
#include "graphics/FX/fxdebris.h"
#include <utilitieslib/utils/textparserUtils.h>
#include "UI/uiOptions.h"
#include <utilitieslib/components/StashTable.h>
#include "gameData/BodyPart.h"

#ifdef NOVODEX_FLUIDS
#include "render/renderprim.h"
#include "render/renderparticles.h"
#endif

#include <utilitieslib/utils/timing.h>

//##############################################################################
// Fx Geos##################################################
int debug_fxgeo_count;
extern FxEngine fx_engine;

void fxGeoUpdateAnimationColoring(FxGeo *fxgeo, F32 age);

extern TokenizerParseInfo ParseFxBhvr[];

#define MIN_SHAKE_TO_BOTHER_WITH 0.05
#define MIN_BLUR_TO_BOTHER_WITH 0.05

static void applyTransformFlags( const Mat4 mIn, Mat4 mOut, Vec3 scaleMin, Vec3 scaleMax, FxTransformFlags flags )
{
    switch (flags) // 3 bits means 8 possibilities
    {
    case 0: // 0 0 0
        {
            // don't do nothing
        }
        break;
    case FX_SCALE: // 0 0 1 
        {
            // scale only
            Vec3 vScale;
            getScale(mIn, vScale);
            if( scaleMin[0] )
                MAXVEC3(scaleMin, vScale, vScale);
            if( scaleMax[0] )
                MINVEC3(scaleMax, vScale, vScale);
            scaleMat3Vec3(mOut, vScale);
        }
        break;
    case FX_ROTATION: // 0 1 0 
        {
            // rot only
            copyMat3(mIn, mOut);
            normalMat3(mOut);
        }
        break;
    case FX_SCALE | FX_ROTATION: // 0 1 1
        {
            // scale and rotation, no pos
            Vec3 vScale;
            getScale(mIn, vScale);
            copyMat3(mIn, mOut);
            normalMat3(mOut);
            if( scaleMin[0] )
                MAXVEC3(scaleMin, vScale, vScale);
            if( scaleMax[0] )
                MINVEC3(scaleMax, vScale, vScale);
            scaleMat3Vec3(mOut, vScale);
        }
        break;
    case FX_POSITION: // 1 0 0 
        {
            // pos only
            copyVec3(mIn[3], mOut[3]);
        }
        break;
    case FX_POSITION | FX_SCALE: // 1 0 1
        {
            // pos and scale, no rotation
            Vec3 vScale;
            getScale(mIn, vScale);
            if( scaleMin[0] )
                MAXVEC3(scaleMin, vScale, vScale);
            if( scaleMax[0] )
                MINVEC3(scaleMax, vScale, vScale);
            scaleMat3Vec3(mOut, vScale);
            copyVec3(mIn[3], mOut[3]);
        }
        break;
    case FX_POSITION | FX_ROTATION: // 1 1 0
        {
            // pos and rotation, no scale
            copyMat4(mIn, mOut);
            normalMat3(mOut);
        }
        break;
    case FX_POSITION | FX_SCALE | FX_ROTATION: // 1 1 1
        {
            // all 3
            Vec3 vScale;
            getScale(mIn, vScale);
            copyMat4(mIn, mOut);
            normalMat3(mOut);
            if( scaleMin[0] )
                MAXVEC3(scaleMin, vScale, vScale);
            if( scaleMax[0] )
                MINVEC3(scaleMax, vScale, vScale);
            scaleMat3Vec3(mOut, vScale);
        }
        break;
    }
}

static bool fxGeoWillBecomeDebris(FxGeo* fxgeo)
{
#if NOVODEX
    return ( fxgeo->bhvr->physDebris && fxgeo->nxEmissary );
#else
    return 0;
#endif
}

Mat4Ptr fxGeoGetWorldSpot(int fxid)
{
    FxGeo * fxgeo;

    fxgeo = hdlGetPtrFromHandle(fxid);
    if(fxgeo)
    {    
        if( (fxgeo->age != fxgeo->world_spot_age && gfxTreeNodeIsValid(fxgeo->gfx_node, fxgeo->gfx_node_id)) )
        {
            gfxTreeFindWorldSpaceMat( fxgeo->world_spot, fxgeo->gfx_node );
            fxgeo->world_spot_age = fxgeo->age;
        }
        return (Mat4Ptr)fxgeo->world_spot;
    }
    return 0; //maybe a unitmat?
}

int fxGeoAttachWorldGroup( FxGeo * fxgeo, char * worldGroup, GroupDef * externalWorldGroupPtr )
{
    int success = 0;

    if( worldGroup )
    {
        if( 0 == stricmp( worldGroup, "External" ) && externalWorldGroupPtr )
        {
            fxgeo->worldGroupPtr = externalWorldGroupPtr;
            fxgeo->worldGroupPtrVersion = game_state.groupDefVersion; //poor mans handle
            fxgeo->worldGroupName = strdup( fxgeo->worldGroupPtr->name );
            if( !externalWorldGroupPtr )
                printToScreenLog( 1, "External World Group Specified, but no valid external world group given" );
            else
                success = 1;
        }
        else
        {
            //TO DO : when Bruce allows arbitrary loading of Library Pieces, then give the artists this
            printToScreenLog( 1, "Not Yet Supported" );
            //fxgeo->worldGroupPtr = groupDefFind( worldGroup );
        }
    }

    return success;
}


int fxGeoAttachSeq(FxGeo * fxgeo, char * anim)
{
    SeqInst * seq;

    if( anim && anim[0]  )
    {
        seq = seqLoadInst( anim, fxgeo->gfx_node, SEQ_LOAD_FULL, 0, NULL ); //The seed was going to be 0 75% of the time before
    
        if(seq)
        {
            fxgeo->seq_handle = seq->handle;
            seq->curranimscale = fxgeo->bhvr->animscale; 
            seq->move_predict_seed = qrand();
        }
        else
        {
            printToScreenLog(1, "failed to create anim %s", anim);
        }
    }

    return 1;
}

int fxGeoAttachCape(FxGeo * fxgeo, TokenizerParams ** capeList, int seqHandle, FxParams *fxp )
{
    int    capeCount;
    char *capeName;

    //Choose geometry randomly if several are given
    capeCount = eaSize( &capeList );
    if( capeCount > 0 )
    {
        TokenizerParams * cape;
        cape = capeList[ rand() % capeCount ];
        assert( cape && eaSize( &cape->params ) );
        capeName = cape->params[0];
        assert( capeName && capeName[0] );
    }
    else
        capeName = 0;

    if( capeName )
    {
        fxgeo->capeInst = createFxCapeInst();

        fxCapeInstInit(fxgeo->capeInst, capeName, seqHandle, fxp);
    }

    return 1;
}

static F32 oneOver255 = 1.0/255.;
int fxGeoAttachParticles(FxGeo * fxgeo, TokenizerParams ** part, int kickstart, int power, FxParams *fxp, FxInfo *fxInfo, F32 animScale )
{
    int cnt;
    int success = 1;
    int i;
    char * part_name;
    int part_count;
    int num_toks;
    int high_power, low_power, power_level_ok;

    part_count = eaSize( &part );
    if (part_count > MAX_PSYSPERFXGEO ) {
        // Too many!
        char filename[MAX_PATH];
        sprintf(filename, "FX/%s", fxInfo->name);
        ErrorFilenamef(filename, "Error: too many particle systems, can have at most %d, found %d", MAX_PSYSPERFXGEO, part_count);
        part_count = MAX_PSYSPERFXGEO;
    }

    for( i = 0 ; i < part_count; i++ ) 
    {
        //Check to see if the power level is right
        num_toks = eaSize( &part[i]->params );
        assert( num_toks > 0 && num_toks <= 3 );
        if( num_toks > 1 ) //if there are power levels attached
        {
            low_power = atoi( part[i]->params[1] );
            if( num_toks > 2 )
                high_power = atoi( part[i]->params[2] );
            else
                high_power = FX_POWER_RANGE; //max power
            assert( low_power >= 0 && low_power <= FX_POWER_RANGE && high_power >= 0 && high_power <= FX_POWER_RANGE );
        
            if( power >= low_power && power <= high_power )
                power_level_ok = 1;
            else
                power_level_ok = 0;
        }
        else
        {
            power_level_ok = 1;
        }

        //If power level is right, go ahead and create this particle system
        if( power_level_ok )
        {
            part_name = part[i]->params[0];

            cnt = fxgeo->psys_count;

            fxgeo->psys[cnt] = partCreateSystem(part_name, &fxgeo->psys_id[cnt], kickstart, power, fxp->fxtype, animScale);

            if( fxgeo->psys[cnt] ) 
            {
                fxgeo->psys[cnt]->geo_handle = fxgeo->handle;
                fxgeo->psys_count++;
            }
            else
            {
                //too many particles debug
                if( isDevelopmentMode() )
                {
                    //char * info = fxPrintFxDebug();
                    //Errorf( "Failed to create particle system %s. \n Please file a bug with this info:\n %s", part_name, info );
                }
                //end debug
                success = 0;
            }
        }
    }

    fxGeoParticlesChangeParams(fxgeo, fxp, fxInfo);

    return success;
}

static void dualTintParticleSystems(FxGeo* fxgeo, FxParams* fxp)
{
    int i;
    ColorPair customColors;
    customColors.primary = colorFromU8(fxp->colors[0]);
    customColors.secondary = colorFromU8(fxp->colors[1]);

    for (i=0; i<fxgeo->psys_count; i++)
    {
        ParticleSystem* system = fxgeo->psys[i];
        bool invertTints = system->sysInfo->blend_mode == PARTICLE_SUBTRACTIVE_INVERSE;
        partBuildColorPath(&system->colorPathCopy, system->sysInfo->colornavpoint, &customColors, invertTints);
        system->effectiveColorPath = &system->colorPathCopy;
    }
}

static void hueShiftParticleSystems(FxGeo * fxgeo, FxParams * fxp, FxInfo *fxInfo)
{
    int i;
    // Get HSV shift from color 0
    Vec3 rgb = {fxp->colors[0][0]*oneOver255, fxp->colors[0][1]*oneOver255, fxp->colors[0][2]*oneOver255};
    F32 effectiveShift;
    Vec3 hsvShift;

    rgbToHsv(rgb, hsvShift);
    // Since low Values on Additive FX just won't work right, let's scale V to [0.1..1.0]
    hsvShift[2] = hsvShift[2]*0.9+0.1;

    if (hsvShift[0] < 0 || fxInfo->effectiveHue < 0)
        effectiveShift = -1;
    else
        effectiveShift = fmod(hsvShift[0] - fxInfo->effectiveHue + 360, 360);

    hsvShift[0] = effectiveShift;

    for (i=0; i<fxgeo->psys_count; i++)
    {
        if (fxgeo->psys[i] && fxgeo->psys[i]->sysInfo && 
            !(fxgeo->psys[i]->sysInfo->flags & PART_IGNORE_FX_TINT))
        {
            ParticleSystem* system = fxgeo->psys[i];
            partCreateHueShiftedPath(system->sysInfo, hsvShift, &system->colorPathCopy);
            system->effectiveColorPath = &system->colorPathCopy;
        }
    }
}

void fxGeoParticlesChangeParams(FxGeo * fxgeo, FxParams * fxp, FxInfo *fxInfo)
{
    if (fxp && fxp->numColors == 2)
        dualTintParticleSystems(fxgeo, fxp);
    else if (fxp && fxp->numColors > 0)
        hueShiftParticleSystems(fxgeo, fxp, fxInfo);
}


int fxGeoAttachChildFx(FxGeo * fxgeo, char * childfxname, FxParams *parentFxp, int seq_handle, int target_seq_handle)
{    
    FxParams    fxp;
//    FxGeo * fxgeo2;

    if( childfxname && childfxname[0] && childfxname[0] != '0' )
    {        
        fxInitFxParams(&fxp);
        fxp.keys[fxp.keycount].gfxnode    = fxgeo->gfx_node;
        fxp.keys[fxp.keycount].seq = hdlGetPtrFromHandle( seq_handle );
// This fixed V_COV\Powers\Necromacy\SummonHit.fx but broke WORLD\City\RunwayLights.fx.
// Made change to fxCreate instead
//        if (!seq_handle && fxgeo->gfx_node) {
//            // Target is probably a location
//            gfxTreeFindWorldSpaceMat(fxp.keys[fxp.keycount].offset, fxgeo->gfx_node);
//        }
        fxp.keycount++;

        fxp.keys[fxp.keycount].gfxnode    = NULL;
        fxp.keys[fxp.keycount].seq = hdlGetPtrFromHandle( target_seq_handle );
        fxp.keycount++;

//JE: Woomer says these don't do anything anyway =)
//        fxgeo2 = hdlGetPtrFromHandle(fxgeo->magnetid);
//        if(fxgeo2)
//        {
//            fxp.keys[fxp.keycount].gfxnode = fxgeo2->gfx_node;
//            fxp.keycount++;
//        }
//        
//        fxgeo2 = hdlGetPtrFromHandle(fxgeo->lookatid);
//        if(fxgeo2)
//        {
//            fxp.keys[fxp.keycount].gfxnode = fxgeo2->gfx_node;
//            fxp.keycount++;
//        }

        if (parentFxp && parentFxp->numColors) {
            fxp.numColors = parentFxp->numColors;
            memcpy(fxp.colors, parentFxp->colors, sizeof(fxp.colors));
        }

        fxp.isUserColored = parentFxp->isUserColored;

        fxgeo->childfxid = fxCreate( childfxname, &fxp );
        if(!fxgeo->childfxid)
        {
            #if NOVODEX
                Errorf("FXGEO: Failed to alloc a child fx %s", childfxname);
            #endif
            return 0;
        }
    }
    return 1;
}

    ///////////////////////////////////////////////////////

#ifdef NOVODEX_FLUIDS
void fxGeoCreateFluid(FxGeo* fxgeo)
{
    FxFluid* fluid = fxgeo->fluidEmitter.fluid;

    if ( fluid )
    {
        // Create emitter
        fxgeo->fluidEmitter.geoOwner = fxgeo;
        fxgeo->fluidEmitter.particlePositions = malloc(fluid->maxParticles * sizeof(Vec3));
        fxgeo->fluidEmitter.particleVelocities = malloc(fluid->maxParticles * sizeof(Vec3));
        fxgeo->fluidEmitter.particleAges = malloc(fluid->maxParticles * sizeof(F32));
        fxgeo->fluidEmitter.particleDensities = malloc(fluid->maxParticles * sizeof(F32));
        fxgeo->fluidEmitter.nxFluid = nwCreateFluidEmitter( &fxgeo->fluidEmitter, NX_CLIENT_SCENE );
    }
}
#endif

static Model * fxGeoGetModelToUse( GfxNode * key,  TokenizerParams ** geomList )
{
    Model * model;
    int    geomCount;

    if(!geomList)
    {
        return NULL;
    }
    
    model = NULL;

    //Choose geometry randomly if several are given
    geomCount = eaSize( &geomList );
    if( geomCount > 0 )
    {
        TokenizerParams * geom;
        char *path;
        char * geometryName;
        
        geom = geomList[ rand() % geomCount ];
        assert( geom && eaSize( &geom->params ) );
        geometryName = geom->params[0];
        assert( geometryName && geometryName[0] );


        //Special case "Parent" use the geometry from this guys At Node (needed because geometry can be random)
        //Used, for example, by random thrown items.  
        if( 0 == stricmp( geometryName, "Parent" ) )
        {
            if( key )
                model = key->model;
        }
        else //Otherwise just dig up the name
        {
            if( (path = objectLibraryPathFromObj( geometryName ) ) )
            {        
                model = modelFind( geometryName, path, LOAD_NOW, GEO_INIT_FOR_DRAWING | GEO_USED_BY_GFXTREE);
                if(!model)
                    printToScreenLog( 1, "FX: Failed to get %s in %s", geometryName, path );
            }
            else
            {
                printToScreenLog( 1, "FX: Bad geo name %s ", geometryName );
            }
        }
    }
    return model;
}


static GfxNode * fxGeoAddGfxNode(GfxNode * parent, Mat4 offset, Model * newmodel)
{
    GfxNode    * gfxnode;
    
    gfxnode      = gfxTreeInsert(parent); //rem 0 = root 
    gfxnode->model = newmodel;
    
    if(newmodel)
    {
        *(gfxTreeAssignTrick( gfxnode )) = *(allocModelTrick( newmodel )); //causes the facing to screw up
        gfxnode->trick_from_where = TRICKFROM_MODEL;
    }

    gfxnode->seqGfxData = 0;  //This will be dynamically set every frame, any thing else is too iffy

    gfxNodeSetFxUseFlag( gfxnode->parent );

    if( offset )
        copyMat4( offset, gfxnode->mat );
    else
        copyMat4( unitmat, gfxnode->mat );

    gfxNodeBuildMiniTracker(gfxnode);
    
    return gfxnode;
}

//#######################################################################################
//FxGeo Create

int fxGeoCreate(void ** fxgeolist, int * fxgeo_count, GfxNode * key, Mat4 offset, FxEvent * fxevent, int iskey, FxInfo *fxinfo, DefTracker * lighttracker )
{
    Mat4        realoffset;
    Mat4        result;
    FxGeo     * fxgeo;
    GfxNode   * parent = NULL;
    Model     * model;
    const FxBhvr * bhvr;
    static int fxgeo_id_pool = SOUND_FX_BASE;
    Vec3 minScale, maxScale;

    PERFINFO_AUTO_START("fxGeoCreate", 1);

    //###1. Set up basic FxGeo from params
    fxgeo = (FxGeo*)mpAlloc(fx_engine.fxgeo_mp);
    listAddForeignMember(fxgeolist, fxgeo);
    if(!fxgeo)
    {
        printToScreenLog( 1, "FXGEO: !!Cant alloc fxgeo " );
        PERFINFO_AUTO_STOP();
        return 0; 
    }
    debug_fxgeo_count++;

    fxgeo->handle = hdlAssignHandle(fxgeo);
    if (!fxgeo->handle) // Out of handles
    {
        PERFINFO_AUTO_STOP();
        return 0;
    }

    fxgeo->world_spot_age = -1;
    fxgeo->unique_id = fxgeo_id_pool++; // used by sound system
    if(fxgeo_id_pool >= SOUND_FX_LAST )
        fxgeo_id_pool = SOUND_FX_BASE; // rollover, don't go into off limits id range

    if (fxevent->name) 
        Strcpy( fxgeo->name, fxevent->name ); //Redundant, but useful
    else 
        fxgeo->name[0] = 0;

    fxgeo->isRoot = !stricmp(&(fxgeo->name[strlen(fxgeo->name)-4]), "ROOT");

    if(!iskey)
    {
        fxgeo->event = fxevent; //otherwise leave event zero //TO DO is this OK?
        fxgeo->lifeStage = FXGEO_ACTIVE;
    }
    else
    {
        fxgeo->event = 0;
        fxgeo->lifeStage = FXGEO_KEY;
    }

    copyVec3(fxinfo->clampMinScale, minScale);
    copyVec3(fxinfo->clampMaxScale, maxScale);

    //###2. Set Gfx_node
    if ( 
        ( fxevent->type == FxTypeLocal )
        || ( fxevent->type == FxTypeStart )
        || ( fxevent->type == FxTypeStartPositOnly )
        || ( fxevent->type == FxTypePosit )
        || ( fxevent->type == FxTypePositOnly )
        )
    {
        Errorf("Invalid fxevent->type in event %s.", fxevent->name);
    }
    if ( fxevent->type == FxTypeCreate )
    {
        Mat4 mKey;
        gfxTreeFindWorldSpaceMat( mKey, key ); 
        copyMat4(unitmat, result);

        // Check to see if it needs anything in the first frame
        applyTransformFlags(mKey, result, minScale, maxScale, fxevent->inherit);

        if ( fxevent->update == ( FX_POSITION | FX_SCALE | FX_ROTATION ) )
        {
            // This is "local", just parent it since it inherits everything
            fxgeo->origin    = 0;
            fxgeo->originid = -1;
            parent            = key;
        }
        else if ( fxevent->update ) // not local, but some stuff
        {
            fxgeo->origin    = key;
            fxgeo->originid = key->unique_id;
            parent            = 0;
        }
        else // no update stuff
        {
            fxgeo->origin   = 0;
            fxgeo->originid = -1;
            parent            = 0;
        }
    }

    if( offset )
        mulMat4( result, offset, realoffset ); 
    else
        copyMat4( result, realoffset );

    ////////////////////////////////////////////////////////
    {
        Model * model;
        model = fxGeoGetModelToUse( key, fxevent->geom );
        //if(model) printf("fxgeocreate: adding model %s\n", model->name);
        fxgeo->gfx_node        = fxGeoAddGfxNode( parent, realoffset, model);
        fxgeo->gfx_node_id    = fxgeo->gfx_node->unique_id; 
        if(fxinfo->fxinfo_flags & FX_INHERIT_GEO_SCALE && model && parent && parent->parent) { // tbd, revisit use of parent->parent here, does this work for all cases?
            fxgeo->gfx_node->flags |= GFXNODE_APPLYBONESCALE;
        }
    }

#ifdef NOVODEX_FLUIDS
    fxgeo->fluidEmitter.fluid = NULL;
    fxgeo->fluidEmitter.particlePositions = NULL;
    if ( fxevent->fluid && fxevent->fluid[0] )
        fxgeo->fluidEmitter.fluid = fxGetFxFluid( fxevent->fluid );
#endif


    //###3. Get bhvr file and init it
    fxgeo->bhvr = 0;
    if( fxevent->bhvr && fxevent->bhvr[0] )
        fxgeo->bhvr = fxGetFxBhvr( fxevent->bhvr ) ; 
    if(!fxgeo->bhvr)
        fxgeo->bhvr = fxGetFxBhvr( "behaviors/null.bhvr" ) ;
    assert(fxgeo->bhvr);


    // Check to see if we have an override copy, and if we do override the appropirate structures
    if ( fxevent->bhvrOverride )
    {
        FxBhvr *newbhvr = malloc(sizeof(FxBhvr));
        if ( !(*fxevent->bhvrOverride)->initialized )
        {
            if( !fxInitFxBhvr( *fxevent->bhvrOverride ) )
            {
                Errorf( "Error in initializing bhvroverride for file %s\n", fxevent->name);
            }
            (*fxevent->bhvrOverride)->initialized = 1;
        }
        *newbhvr = *fxgeo->bhvr;
        ParserOverrideStruct(ParseFxBhvr, newbhvr, *fxevent->bhvrOverride, 0);
        fxgeo->bhvr = newbhvr;
        fxgeo->bhvrOverride = 1;
    }

    bhvr = fxgeo->bhvr;

    copyVec3(bhvr->initspin, fxgeo->bhvr_spin);
    partVectorJitter(fxgeo->bhvr_spin, bhvr->spinjitter, fxgeo->bhvr_spin);


    //Add in the position changes from the behavior file
    //This is redundant and cumulative with the offset value, but added to make life easier for artists so 
    //they can specify this stuff in a behavior file instead of doing simple offsets in altpivs 
    if(!vec3IsZero(bhvr->pyrRotate) || !vec3IsZero(bhvr->pyrRotateJitter))
    {
        Vec3 vPyrRotate;
        partVectorJitter(bhvr->pyrRotate, bhvr->pyrRotateJitter, vPyrRotate);
        rotateMat3( vPyrRotate, fxgeo->gfx_node->mat);
    }
    if(bhvr->useShieldOffset && SAFE_MEMBER2(parent, parent, seqGfxData) && !vec3IsZero(parent->parent->seqGfxData->shieldpyr))
        rotateMat3(parent->parent->seqGfxData->shieldpyr, fxgeo->gfx_node->mat);

    if ( bhvr->initialvelocityangle )
    {
        copyVec3(bhvr->initialvelocity, fxgeo->velocity);
        partRadialJitter(fxgeo->velocity, RAD(bhvr->initialvelocityangle));
    }
    else
    {
        partVectorJitter(bhvr->initialvelocity, bhvr->initialvelocityjitter, fxgeo->velocity);
    }

    {
        Vec3 vStartingOffset;
        partVectorJitter(bhvr->positionOffset, bhvr->startjitter, vStartingOffset);
        if(bhvr->useShieldOffset && SAFE_MEMBER2(parent, parent, seqGfxData))
            addVec3(vStartingOffset, parent->parent->seqGfxData->shieldpos, vStartingOffset);

        // Rotate the offset by the orientation of the parent, if we inherit rotation

        if ( fxevent->inherit & FX_ROTATION && fxevent->inherit & FX_ROTATE_ALL )
        {
            Vec3 vTemp;
            Mat4 mKey;
            gfxTreeFindWorldSpaceMat( mKey, key ); 
            normalMat3(mKey);
            mulVecMat3(vStartingOffset, mKey, vTemp);
            copyVec3(vTemp, vStartingOffset);
            // also, impart rotation to velocity
            mulVecMat3(fxgeo->velocity, mKey, vTemp);
            copyVec3(vTemp, fxgeo->velocity);
        }

        addVec3(vStartingOffset, fxgeo->gfx_node->mat[3], fxgeo->gfx_node->mat[3]);

    }

    //###4. Initialize fxgeo params from it's bhvr file    
    //## Initialize scaling
    copyVec3( minScale, fxgeo->clampMinScale);
    copyVec3( maxScale, fxgeo->clampMaxScale);
    {
        Vec3 scaleVec;
        copyVec3(bhvr->scale, scaleVec );

        if( fxgeo->clampMinScale[0] )
            MAXVEC3(fxgeo->clampMinScale, scaleVec, scaleVec);
        if( fxgeo->clampMaxScale[0] )
            MINVEC3(fxgeo->clampMaxScale, scaleVec, scaleVec);
    
        scaleMat3Vec3( fxgeo->gfx_node->mat, scaleVec );
        copyVec3( scaleVec, fxgeo->cumulativescale ); 
    }
    fxgeo->is_scaling = ( bhvr->scaleTime[0] || bhvr->scaleTime[1] || bhvr->scaleTime[2] );

    
    //### 5. Light fxgeo (is the mat fully correct at this point?)
    model = fxgeo->gfx_node->model;
    if(model && (model->flags & OBJ_STATICFX) && lighttracker)
    {
        // JE: There are not any FX files that will currently hit this code, and up until 5/5/05, this
        //  would have just crashed.  Remove it?
        Mat4Ptr mat;
        fxgeo->gfx_node->rgbs = malloc(model->vert_count * 4);
        mat = fxGeoGetWorldSpot(fxgeo->handle);
        lightModel(model, mat, fxgeo->gfx_node->rgbs, lighttracker, 1.f);
        fxgeo->gfx_node->rgbs = fxgeo->rgbs; 
    }

#ifdef NOVODEX
    fxgeo->nxEmissary = 0;
    if ( bhvr->physicsEnabled && fxgeo->event && nwEnabled() && nwCanAddDynamicActor() )
    {

        PERFINFO_AUTO_START("Push actor",1);
        if ( model )
        {
            int iVert=0;
            char cHashName[256];
            char cScaleBuf[64];

            Vec3*    verts;
            Mat4    mUnscaledMat;
            NxGroupType group;
            switch (bhvr->physDebris)
            {
            case eNxPhysDebrisType_None:
            case eNxPhysDebrisType_Small:
                group = nxGroupDebris;
                break;
            case eNxPhysDebrisType_Large:
                group = nxGroupLargeDebris;
                break;
            case eNxPhysDebrisType_LargeIfHardware:
                if ( nx_state.hardware )
                    group = nxGroupLargeDebris;
                else
                    group = nxGroupDebris;
                break;
            default:
                group = nxGroupDebris;
                break;
            }

            // Generate a name with the scale and model->name, so that
            // We have a different shape for different scales
            // We don't care too much about scale resolution, either
            strcpy(cHashName, model->name);
            sprintf(cScaleBuf, "Scale:%.2f,%.2f,%.2f", bhvr->scale[0], bhvr->scale[1], bhvr->scale[2] );
            strcat(cHashName, cScaleBuf);

            // Check to see if the shape is already cached
            if ( !nwFindConvexShapeDesc(cHashName, NULL) )
            {
                Vec3 vScale;
                fxGeoGetWorldSpot(fxgeo->handle);
                getScale(fxgeo->world_spot, vScale);
                
                // Also increase physics scale
                if ( !vec3IsZero(bhvr->physScale) )
                    mulVecVec3(vScale, bhvr->physScale, vScale);
                //mulVecVec3(bhvr->scale, vScale, vScale);

                verts = _alloca(model->vert_count * sizeof(*verts));
                modelGetVerts(verts, model);
                for(iVert=0; iVert<model->vert_count; ++iVert)
                {
                    mulVecVec3(vScale,verts[iVert], verts[iVert]);
                }
                nwCookAndStoreConvexShape(cHashName, (float*)verts, model->vert_count, group, fxgeo, NX_CLIENT_SCENE);
            }

            // Create the actor (scale must be removed and then readded
            copyMat4(fxgeo->gfx_node->mat, mUnscaledMat);
            normalMat3(mUnscaledMat);
            fxgeo->nxEmissary = nwPushConvexActor(cHashName, mUnscaledMat, fxgeo->velocity, fxgeo->bhvr_spin, bhvr->physDensity, fxgeo, NX_CLIENT_SCENE);
            if ( bhvr->physJointDOFs )
            {
                NwEmissaryData* pEmissary = nwGetNwEmissaryDataFromGuid(fxgeo->nxEmissary);
                if ( pEmissary) {
                    nwAddJoint(pEmissary, bhvr->physJointDOFs, bhvr->physJointAnchor, mUnscaledMat, bhvr->physJointLinLimit, bhvr->physJointAngLimit, bhvr->physJointBreakForce, bhvr->physJointBreakTorque, bhvr->physJointLinSpring, bhvr->physJointLinSpringDamp, bhvr->physJointAngSpring, bhvr->physJointAngSpringDamp);
                    pEmissary->bJointCollidesWorld = bhvr->physJointCollidesWorld;
                }
            }
        }
        else
        {
            fxgeo->nxEmissary = nwPushSphereActor(fxgeo->gfx_node->mat[3], fxgeo->velocity, fxgeo->bhvr_spin, bhvr->physDensity, bhvr->physRadius, nxGroupDebris, fxgeo, false, NX_CLIENT_SCENE);
        }

        if ( fxgeo->nxEmissary )
        {
            NwEmissaryData* pData = nwGetNwEmissaryDataFromGuid(fxgeo->nxEmissary);
            if ( bhvr->physGravity != 1.0f )
            {
                // The idea here is that the physGravity tag is a multiplier times the default gravity
                // In order to accomplish this we apply an external force every frame to the actor in order to acheieve this "gravity"
                Vec3 vCounterGravity;
                zeroVec3( vCounterGravity );
                vCounterGravity[1] = nwGetDefaultGravity() * (bhvr->physGravity - 1.0f);
                addVec3( pData->vAccelToAdd, vCounterGravity, pData->vAccelToAdd );
            }

            pData->fDrag = bhvr->drag;
            pData->fJointDrag = bhvr->physJointDrag;
        }
        PERFINFO_AUTO_STOP();
    }
#endif

    // Calculate our lifespan from the event lifespan and the jitter
    fxgeo->lifespan = fxevent->lifespan;
    if ( fxevent->lifespanjitter > 0 )
    {
        fxgeo->lifespan += qfrand() * fxevent->lifespanjitter;
        if ( fxgeo->lifespan <= 0)
            fxgeo->lifespan = 0.001;
    }

    // If the event says it's in camera space, copy that to the geo.
    if( fxevent->bCameraSpace )
    {
        fxgeo->fxgeo_flags |= FXGEO_AT_CAMERA;
    }

    //### 6. Yucky little hack that I hope will never be used
    //hack so UI can have a toggle to make power looping sounds loop forever, if you are so inclined
    //FXGEO_POWER_LOOPING_SOUND is a one time flags Mike Henry is putting on fx like targeting drone when he fixes them to
    //fade away and not be annoying if we want people to be able to set them back to annoying.
    if( game_state.powerLoopingSoundsGoForever && fxevent->fxevent_flags & FXGEO_POWER_LOOPING_SOUND )
    {
        fxevent->lifespan = 0;
        fxgeo->lifespan = 0;
    }

    assert(fxgeo->handle);
    (*fxgeo_count)++; 
    
    PERFINFO_AUTO_STOP();

    return fxgeo->handle;
}

/*Used all over to find a node in the fx to do something with*/
FxGeo * findFxGeo( FxGeo * firstfxgeo, char * name )
{
    FxGeo * fxgeo;
    
    fxgeo = firstfxgeo;
    while(fxgeo && fxgeo->next) //screwy //why do I care what order I'm evaluating it in??
        fxgeo = fxgeo->next;

    for( ; fxgeo ; fxgeo = fxgeo->prev)
    {
        if( !_stricmp( name, fxgeo->name ) )
        {
            return fxgeo;
        }
    }
    printToScreenLog(0, "GEO: Failed to find geo %s for attaching", name); 
    return 0;
}

void fxGeoInitialize(FxGeo * fxgeo, FxGeo * fxgeolist)
{
    FxEvent * fxevent;
    FxGeo * fxgeo2;
    
    fxevent = fxgeo->event;
    if(fxevent)
    {        
        if( fxevent->magnet && fxevent->magnet[0] && fxevent->magnet[0] != '0')
        {
            fxgeo2 = findFxGeo( fxgeolist, fxevent->magnet );
            if(fxgeo2)
                fxgeo->magnetid    = hdlGetHandleFromPtr( fxgeo2, fxgeo2->handle);
        }
        
        if( fxevent->lookat && fxevent->lookat[0] && fxevent->lookat[0] != '0')
        {
            if( !_stricmp( "Camera", fxevent->lookat) ) //TO DO: does this work?
            {
                fxgeo->fxgeo_flags |= FXGEO_LOOKAT_CAMERA;
                //(gfxTreeAssignTrick( fxgeo->gfx_node ))->flags |= TRICK_FRONTFACE;
            }
            else
            {
                fxgeo2 = findFxGeo( fxgeolist, fxevent->lookat );
                if(fxgeo2)
                    fxgeo->lookatid    = hdlGetHandleFromPtr( fxgeo2, fxgeo2->handle);
            }
        }

        if( fxevent->pmagnet && fxevent->pmagnet[0] && fxevent->pmagnet[0] != '0')
        {
            fxgeo2 = findFxGeo( fxgeolist, fxevent->pmagnet );
            if(fxgeo2)
                fxgeo->pmagnetid = hdlGetHandleFromPtr( fxgeo2, fxgeo2->handle);
        }

        if( fxevent->pother && fxevent->pother[0] && fxevent->pother[0] != '0')
        {
            fxgeo2 = findFxGeo( fxgeolist, fxevent->pother );
            if(fxgeo2)
                fxgeo->potherid = hdlGetHandleFromPtr( fxgeo2, fxgeo2->handle);
        }
    }        
}

void fxGeoUpdateAnimation( FxGeo *fxgeo, int seq_handle, int parent_seq_handle, int * state, EntLight * light, U8 alpha, F32 animScale, char * additionalStates )
{    
    SeqInst * seq, *parent_seq;

    seq = hdlGetPtrFromHandle(seq_handle);
    parent_seq = hdlGetPtrFromHandle(parent_seq_handle);

    if( seq ) 
    {
        F32 timestep;
        PERFINFO_AUTO_START("animation",1);

        
        //seqSynchCycles(seq, e->owner); maybe add this later

        //make the fx's state the sequencer's (is this tha right place for this?
        seqSetOutputs(seq->state, state); 

        if( additionalStates )
            seqSetStateFromString( seq->state, additionalStates );

        //TO DO consolidate repeats in entClientUpdate...
        if ( parent_seq )
        {
            timestep = parent_seq->moveRateAnimScale * TIMESTEP * animScale;
            seq->moveRateAnimScale = parent_seq->moveRateAnimScale;
        }
        else
            timestep = TIMESTEP * animScale;
        if(    seqProcessInst( seq, timestep ) ) {//TO DO fix this  
            seq->updated_appearance = 1;
        }
 
//xyprintf(10, 10, "%s", seq->animation.move->name);

        //Manage Fx that are played as a result of the sequencer move
        seqManageSeqTriggeredFx( seq, seq->updated_appearance, NULL );

        // if parent seq move has hideweapon, and triggeredFx has isweapon, hide it.  Note that any nodes that are hidden here
        //    will get shown next from in fxGeoUpdateAll, so we rely on the fact that anim triggered fx come after the fx
        //    which trigger them in the order of fx getting serviced each frame.
        if(getMoveFlags(parent_seq, SEQMOVE_HIDEWEAPON))
        {
            int i;
            for(i=0; i<MAX_SEQFX; i++)
            {
                int fxid = seq->const_seqfx[i];
                if(fxid)
                {
                    FxObject * childfx = hdlGetPtrFromHandle(fxid);
                    if(childfx && childfx->fxinfo->fxinfo_flags & FX_IS_WEAPON)
                    {
                        FxGeo *fxgeo;
                        for( fxgeo = childfx->fxgeos; fxgeo ; fxgeo = fxgeo->next) 
                            fxgeo->gfx_node->flags |= GFXNODE_HIDE;
                    }
                }
            }
        }

        // TO DO maybe use this to not draw dudes till they are fully loaded into the world, or hold entry till you are fully loaded
        if( seq->loadingObjects == OBJECT_LOADING_IN_PROGRESS )
        {
            seq->loadingObjects = animCheckForLoadingObjects( seq->gfx_root->child, seq->handle );
            if( seq->loadingObjects == OBJECT_LOADING_COMPLETE )
                seq->updated_appearance = 1;
        }

        if(seq->updated_appearance)
        {
            animSetHeader( seq, 0 );
            seq->updated_appearance = 0;
            // Apply custom colors and textures
            fxgeo->needSetCustomTextures = 1;
            fxGeoUpdateAnimationColoring(fxgeo, fxgeo->age);
        }
        assert( gfxTreeNodeIsValid(seq->gfx_root, seq->gfx_root_unique_id ) );
        copyVec3( seq->gfx_root->mat[3], seq->posLastFrame );
    
        seqClearState( seq->state );
        gfxNodeSetAlpha(seq->gfx_root, alpha, 1); //FXFADE check if this works
        animPlayInst( seq );
        seq->seqGfxData.alpha = alpha;
        if( seq->static_light_done != STATIC_LIGHT_DONE  )
        {
            seqSetStaticLight(seq, 0);
            seq->static_light_done = STATIC_LIGHT_DONE;
        }

        //Propagate fx's lightng to the child animation
        if( light )
            seq->seqGfxData.light = *light;//copy the struct
        else
            seq->seqGfxData.light.use = ENTLIGHT_DONT_USE; 

        PERFINFO_AUTO_STOP();
    }
}

static void trackerClearDefs(DefTracker *tracker)
{
    int i;
    if (!tracker)
        return;
    tracker->def = 0;
    for (i = 0; i < tracker->count; i++)
        trackerClearDefs(&tracker->entries[i]);
}

//U8 inheritedAlpha is currently unused.  Maybe use it later
void fxGeoUpdateWorldGroup( FxGeo * fxgeo, int drawstatus, EntLight * entLight, U8 inheritedAlpha, int useStaticLighting )
{
    Mat4Ptr mat;
    Mat4 matx;
    EntLight * light;
    char newVersion = 0;

    if( fxgeo->worldGroupPtr )
    {
        PERFINFO_AUTO_START("updateWorldGroup",1);

        //Recover from Edits
        if( fxgeo->worldGroupPtrVersion != game_state.groupDefVersion )
        {
            fxgeo->worldGroupPtr = groupDefFind( fxgeo->worldGroupName );
            fxgeo->worldGroupPtrVersion = game_state.groupDefVersion;
            newVersion = 1;
        }

        if( fxgeo->worldGroupPtr )
        {
            int uid = -1; // this will always get set, unless you remove my hack -GG
            DefTracker *lighttracker=0;
            mat = fxGeoGetWorldSpot(fxgeo->handle);
            if (useStaticLighting)
                lighttracker = groupFindLightTracker(mat[3]);
            mulMat4(cam_info.viewmat,mat,matx);

            if( !fxgeo->fxMiniTracker )
                fxgeo->fxMiniTracker = calloc( 1, sizeof( FxMiniTracker ) );

            fxgeo->fxMiniTracker->count = 0;
            fxgeo->fxMiniTracker->alpha = inheritedAlpha;

            if (fxgeo->fxMiniTracker->lighttracker != lighttracker || newVersion)
            {
                if (fxgeo->fxMiniTracker->tracker)
                {
                    trackerClearDefs(fxgeo->fxMiniTracker->tracker);
                    trackerClose(fxgeo->fxMiniTracker->tracker);
                }
                else
                {
                    fxgeo->fxMiniTracker->tracker = malloc(sizeof(DefTracker));
                    ZeroStruct(fxgeo->fxMiniTracker->tracker);
                    groupAddAuxTracker(fxgeo->fxMiniTracker->tracker);
                }
                fxgeo->fxMiniTracker->tracker->def = fxgeo->worldGroupPtr;
                trackerUpdate(fxgeo->fxMiniTracker->tracker, fxgeo->fxMiniTracker->tracker->def, 1);

                // JW fx end up using fxMiniTracker->tracker->mat to draw, rather than matx, so copy mat in here
                copyMat4(mat,fxgeo->fxMiniTracker->tracker->mat);

                fxgeo->fxMiniTracker->lighttracker = lighttracker;
            }

            // This is a hack to get uid information out of groupMiniTrackers.c and groupdrawutil.c to build
            // lasting group-MiniTrackers for getting texture swapping information to drawModel() through the
            // rabbit hole of groupDrawDefTracker().
            // These have to be rebuilt any time group-MiniTrackers are rebuilt (i.e. mapmoves or editing).
            // Alternatively, one could write a new system for getting texture swap information to the draw system.
            {
                static StashTable worldGroupPtr_uids;

                extern int g_worlduids_reset;
                extern int g_worlduids_last;
                extern int glob_last_uid;

                if(g_worlduids_reset)
                {
                    if(worldGroupPtr_uids) stashTableClear(worldGroupPtr_uids);
                    g_worlduids_reset = 0;
                }

                if( !(worldGroupPtr_uids && stashFindInt(worldGroupPtr_uids, fxgeo->worldGroupPtr->name, &uid)) )
                {
                    uid = g_worlduids_last;
                    groupDrawBuildMiniTrackersForDef(fxgeo->worldGroupPtr, uid);
                    g_worlduids_last = glob_last_uid;
                    if(!worldGroupPtr_uids)
                        worldGroupPtr_uids = stashTableCreateWithStringKeys(0, StashDeepCopyKeys);
                    assert(stashAddInt(worldGroupPtr_uids, fxgeo->worldGroupPtr->name, uid, false));
                }
            }

            if( entLight && entLight->use )
                light = entLight;
            else
                light = 0;
            //3.0 == amount of extra distance to put on the thing, just to be safe.
            groupDrawDefTracker( fxgeo->worldGroupPtr, fxgeo->fxMiniTracker->tracker, matx, light, fxgeo->fxMiniTracker, 3.0, lighttracker, uid); //TO DO, is this the right place to call this?
        }

        PERFINFO_AUTO_STOP_CHECKED("updateWorldGroup");
    }
}



void fxGeoUpdateParticleSystems( FxGeo * fxgeo, int drawstatus, int teleported, U8 inheritedAlpha, F32 animScale )
{
    ParticleSystem * psys;
    int i;
    Mat4Ptr mat;
    int part_count;

    if( fxgeo->event )
        part_count = eaSize( &fxgeo->event->part );
    else 
        part_count = 0;
        
    if(!part_count)
        return;
        
    PERFINFO_AUTO_START("particles",1);

    for( i = 0 ; i < part_count ; i++ )
    {
        fxgeo->psys[i] = psys = partConfirmValid( fxgeo->psys[i], fxgeo->psys_id[i] );
        if( psys ) 
        {    
            psys->animScale = animScale;

            mat = fxGeoGetWorldSpot(fxgeo->handle);
            partSetOriginMat( psys, fxgeo->psys_id[i], mat );

            if( hdlGetPtrFromHandle(fxgeo->pmagnetid) )
                mat = fxGeoGetWorldSpot(fxgeo->pmagnetid);
            else
                mat = fxGeoGetWorldSpot(fxgeo->handle);
    
            partSetMagnetPoint( psys, fxgeo->psys_id[i], mat[3] );

            if( hdlGetPtrFromHandle(fxgeo->potherid) )
                mat = fxGeoGetWorldSpot(fxgeo->potherid);
            else
                mat = fxGeoGetWorldSpot(fxgeo->handle);

            partSetOtherPoint( psys, fxgeo->psys_id[i], mat[3] );

            partToggleDrawing( psys, fxgeo->psys_id[i], drawstatus ); 

            if( teleported )  
                partSetTeleported( psys, fxgeo->psys_id[i], teleported );

            //if( inheritedAlpha != 255 ) 
                partSetInheritedAlpha( psys, fxgeo->psys_id[i], inheritedAlpha );
        }
    }

    PERFINFO_AUTO_STOP();
}

static SoundEvent* pickNewSound(FxEvent* fxevent)
{
    SoundEvent* result = NULL;
    int soundCount = eaSize(&fxevent->sound);
    int historyCount = eaiSize(&fxevent->soundHistory);
    int soundOffset;
    int soundIndex = -1;

    if (soundCount == 0)
        return NULL;

    assert(fxevent->soundNoRepeat <= soundCount);

    soundOffset = (rand() * (soundCount - historyCount)) / (RAND_MAX + 1);

    while (soundOffset >= 0)
    {
        bool indexInHistory = false;
        int i;

        ++soundIndex;

        for (i = 0; i < historyCount; ++i)
        {
            if (fxevent->soundHistory[i] == soundIndex)
            {
                indexInHistory = true;
                break;
            }
        }

        if (!indexInHistory)
            --soundOffset;
    }

    if (fxevent->soundNoRepeat > 0)
    {
        if (historyCount < fxevent->soundNoRepeat)
            eaiPush(&fxevent->soundHistory, soundIndex);
        else
        {
            int i;
            for (i = 0; i < historyCount - 1; ++i)
                fxevent->soundHistory[i] = fxevent->soundHistory[i + 1];
            fxevent->soundHistory[historyCount - 1] = soundIndex;
        }
    }

    return fxevent->sound[soundIndex];
}

void fxGeoUpdateSound( FxGeo* fxgeo, int seqHandle )
{
    Mat4Ptr mat;
    int soundCount;   
    SoundEvent * sound;
    F32 volume;
    SeqInst * seq=0;

    if( !fxgeo->event || !fxgeo->event->sound )
        return;

    soundCount = eaSize( &fxgeo->event->sound );
    if( !soundCount )
        return;

    //Only looping samples should get processed more than once.
    sound = fxgeo->sound_triggered;

    if( sound && !sound->isLoopingSample )
        return;

    //Don't play sounds on guys who aren't perceptible
    if( seqHandle )
    {
        seq = hdlGetPtrFromHandle(seqHandle);
        if( seq && seq->notPerceptible )
            return;

    }

    PERFINFO_AUTO_START("sound prep",1);

        if ( !sound )
            sound = pickNewSound(fxgeo->event);

        assert( sound );
    
        volume = sound->volume;

        mat = fxGeoGetWorldSpot(fxgeo->handle);   

        //Little hack to make looping sounds fade as the entity fades away
        if( seqHandle )
        {
            seq = hdlGetPtrFromHandle(seqHandle);
            if( seq && seq->alphafade != 255 )
            {
                if(    sound->isLoopingSample )
                    volume *= (F32)seq->alphafade / 255.0;
            }
        }
        if( fxgeo->alpha != 255 )
        {
            if(    sound->isLoopingSample )
                volume *= (F32)fxgeo->alpha / 255.0;
        }
    
    PERFINFO_AUTO_STOP_START("sndPlaySpatial",1);
    
    {
        int flags = SOUND_PLAY_FLAGS_INTERRUPT;
        if (seq && playerPtr() && seq==playerPtr()->seq)
            flags |= SOUND_PLAY_FLAGS_ONPLAYER;

        sndPlaySpatial( sound->soundName,
                        fxgeo->unique_id,
                        0,
                        mat[3],
                        MAX((sound->radius-sound->fade), 0.0),
                        sound->radius,
                        volume,
                        sound->volume,
                        flags,
                        0,
                        0.0f);
    }
    
    PERFINFO_AUTO_STOP(); 
    
    fxgeo->sound_triggered = sound;
}

//debug: int decision[100];
int gfxTreeDrawNodeJustToFillSeqGfxDataDecide( GfxNode *node, int root, BoneInfo *bones, int *decisionIndex, int *decisionBits, int seqHandle )
{
    int ret=0;
    int i;
    for(;node;node = node->next)
    {
        bool goodnode=false;

        if (!root && node->seqHandle != seqHandle)
            continue;

        if (*decisionIndex == SIZEOF2(FxCapeInst,pathToSkin)*8)
            break;

//        assert(*decisionIndex < SIZEOF2(FxCapeInst,pathToSkin)*8); // that's how many bits I've got defined in fxCapes at the moment

        if( ( node->flags & GFXNODE_HIDE )) {// || ( node->seqHandle && !node->useFlags ) ) {
            if( root )
                break;
            continue;
        }

        // Check to see if this is a worthwhile node
        for (i=0; i<bones->numbones; i++) {
            if (node->anim_id == bones->bone_ID[i]) {
                goodnode = true;
                break;
            }
        }
        ret |= goodnode;
        //printf("Node %d: %d\n", node->anim_id, goodnode);

        if ( !node->child ) { 
            if ( goodnode ) {
                //printf(" no child, but good node\n");
                SETB(decisionBits, *decisionIndex);
                //decision[*decisionIndex] = 1;
                ++(*decisionIndex);
            } else {
                //printf(" no child, not good bone\n");
                CLRB(decisionBits, *decisionIndex);
                //decision[*decisionIndex] = 0;
                ++(*decisionIndex);
                continue;
            }
        }

        //If you are just a bone for skinning, you are done.
        if( !node->child )
            continue;

        if (node->child) {
            int tempDecisionIndex = *decisionIndex+2;
            int ret2;
            //printf(" has child\n");

            ret2 = gfxTreeDrawNodeJustToFillSeqGfxDataDecide(node->child, 0, bones, &tempDecisionIndex, decisionBits, seqHandle);
            //printf("Node %d: child decide: %d (index %d)\n", node->anim_id, ret2, *decisionIndex, ret2);
            if (ret2) {
                // something valid beneath
                ret = 1;
                // Do me
                SETB(decisionBits, *decisionIndex);
                //decision[*decisionIndex] = 1;
                ++(*decisionIndex);
                // Do children
                SETB(decisionBits, *decisionIndex);
                //decision[*decisionIndex] = 1;
                *decisionIndex = tempDecisionIndex;
            } else {
                if (goodnode) {
                    // Do me
                    SETB(decisionBits, *decisionIndex);
                    //decision[*decisionIndex] = 1;
                    ++(*decisionIndex);
                    // Skip children
                    CLRB(decisionBits, *decisionIndex);
                    //decision[*decisionIndex] = 0;
                    ++(*decisionIndex);
                } else {
                    // Skip me and children
                    CLRB(decisionBits, *decisionIndex);
                    //decision[*decisionIndex] = 0;
                    ++(*decisionIndex);
                }
            }
        }

        if( root )
            break;
    }
    //assert(*decisionIndex < SIZEOF2(FxCapeInst,pathToSkin)*8); // that's how many bits I've got defined in fxCapes at the moment
    return ret;
}

int gfxTreeDrawNodeJustToFillSeqGfxDataExecute( GfxNode *node, const Mat4 parent_mat, int seqHandle, SeqGfxData * seqGfxDataDst, SeqGfxData * seqGfxDataSrc, int root, int decisionIndex, int *decisionBits )
{
    Mat4 viewspace;
    for(;node;node = node->next)
    {
        int d;

        if (!root && node->seqHandle != seqHandle)
            continue;

        if( ( node->flags & GFXNODE_HIDE ) ) { // || ( node->seqHandle && !node->useFlags ) ) {
            if( root )
                break;
            continue;
        }

        // either the decision to look at this node because it's childless and a good bone, or childful and either good or has good children
        d = TSTB(decisionBits, decisionIndex);
        //d = decision[decisionIndex];
        ++decisionIndex;
        //printf("node %d decision %d = %d\n", node->anim_id, decisionIndex-1, d);

        if (!d)
            continue;

        //gfxGetAnimMatrices
        if( bone_IdIsValid(node->anim_id) && node->seqHandle == seqHandle ) 
        {    
            Mat4Ptr viewspace_scaled; 
            Vec3 xlate_to_hips;
            Mat4 scaled_xform;
            Mat4 scale_mat;

            viewspace_scaled = seqGfxDataDst->bpt[node->anim_id];

            copyMat4(unitmat, scale_mat); 
            scaleMat3Vec3(scale_mat,node->bonescale); //I think just Mat4 transfer will fix??
            mulMat4(node->mat, scale_mat, scaled_xform);    

            mulMat4(parent_mat, scaled_xform, viewspace_scaled );
            scaleVec3(seqGfxDataSrc->btt[node->anim_id], -1, xlate_to_hips);   //Prexlate this back to home
            mulVecMat4( xlate_to_hips, viewspace_scaled, viewspace_scaled[3] );//is this the right function
            //printf("did math on %d\n", node->anim_id);
        }

        //If you are just a bone for skinning, you are done.
        if( !node->child )
            continue;

        mulMat4( parent_mat, node->mat, viewspace );

        if (node->child) {
            // whether or not to recurse from here
            d = TSTB(decisionBits, decisionIndex);
            //d = decision[decisionIndex];
            ++decisionIndex;
            //printf("node %d decision %d = %d (child choice)\n", node->anim_id, decisionIndex-1, d);
            if (d) {
                decisionIndex = gfxTreeDrawNodeJustToFillSeqGfxDataExecute(node->child, viewspace, seqHandle, seqGfxDataDst, seqGfxDataSrc, 0, decisionIndex, decisionBits);
            } else {
                // skipped it!
            }
        }

        if( root )
            break;
    }
    return decisionIndex;
}

int g_fxgeo_cape_timestamp=0;
void fxGeoResetAllCapes(void)
{
    g_fxgeo_cape_timestamp++;
}

void fxGeoUpdateCape( FxGeo* fxgeo, FxObject *fx )
{
    static Vec3 weightedverts[100]={0};
    static Vec3 weightednorms[100]={0};
    FxCapeInst *capeInst;
    SeqInst * seq=NULL;
    Vec3 gfx_pos;                    //Position of this entities graphics (can be different from entity position)
    Vec3 gfx_pos_vs_camera;            //pos of this ent's gfx relative to camera
    F32 gfx_dist_from_camera;        //distance from ent's gfx to camera
    Mat4Ptr mat=NULL;
    GfxNode *gfx_root;

    if( !fxgeo->capeInst )
        return;

    PERFINFO_AUTO_START("capes", 1);

    capeInst = fxgeo->capeInst;

    if (capeInst->updateTimestamp != g_fxgeo_cape_timestamp) {
        // reset cape!
        gfxTreeDelete(capeInst->clothnode);
        capeInst->clothnode = NULL;
        fxCapeInstInit(capeInst, capeInst->cape->name, fx->handle_of_parent_seq, &fx->fxp);
        return;
    }

    if( !fx->handle_of_parent_seq ) {
        if (capeInst->type == CLOTH_TYPE_FLAG) {
            mat = fxGeoGetWorldSpot(fxgeo->handle);
            gfx_root = fxgeo->gfx_node;
        } else {
            PERFINFO_AUTO_STOP();
            return;
        }
    } else {
        seq = hdlGetPtrFromHandle(fx->handle_of_parent_seq);
        if (!seq)
        {
            PERFINFO_AUTO_STOP();
            return;
        }
        gfx_root = seq->gfx_root;
    }

    if (!capeInst->clothnode)
    {
        PERFINFO_AUTO_STOP();
        // What happened here?
        return;
    }

    // Fixes issue when going back to login screen from char select and other madness
    if (gfx_root->flags & GFXNODE_HIDE) {
        PERFINFO_AUTO_STOP();
        // Hide the cape as well!
        hideCloth(capeInst->clothnode);
        return;
    }

    if (mat)
        copyVec3( mat[3], gfx_pos );
    else if ( gfx_root->child )
        mulVecMat4( gfx_root->child->mat[3], gfx_root->mat, gfx_pos );
    else
        copyVec3( gfx_root->mat[3], gfx_pos );
    mulVecMat4( gfx_pos, cam_info.viewmat, gfx_pos_vs_camera );
    gfx_dist_from_camera = lengthVec3( gfx_pos_vs_camera );
    if (capeInst->cape->cape_lod_bias!=0) {
        gfx_dist_from_camera /= capeInst->cape->cape_lod_bias;
    }

    if( capeInst->capeHarness && capeInst->type == CLOTH_TYPE_CAPE)
    {
        SeqGfxData globSeqGfxData;
        Model * model;
        GfxNode * node;
        Vec3 positionOffset;
        
        copyVec3(seq->gfx_root->mat[3], positionOffset);

        model = capeInst->capeHarness;

        if (!capeInst->capeHarness->vbo || !capeInst->capeHarness->vbo->verts) {
            // VBO gone?  happens on loadmap and reload
            modelSetupVertexObject( capeInst->capeHarness, VBOS_DONTUSE, BlendMode(0,0));
        }

        node = seqFindGfxNodeGivenBoneNum(seq, BONEID_CHEST); //In the future have system to figure out which bone harness is rooted to, for now, it's just the chest

        //Crazy cooked up thing to get xformed verts in global space, not viewspace
        memset(globSeqGfxData.bpt, 0, sizeof(globSeqGfxData.bpt));
        // Fill in bpt
        // Do transformations in "cape space"  Is this too hacky?
        copyVec3(zerovec3, seq->gfx_root->mat[3]);
        {
            int di=0;
            if (!capeInst->pathToSkinCalculated) {
                gfxTreeDrawNodeJustToFillSeqGfxDataDecide( seq->gfx_root, 1, model->boneinfo, &di, capeInst->pathToSkin, seq->handle);
                capeInst->pathToSkinCalculated = 1;
            }
            gfxTreeDrawNodeJustToFillSeqGfxDataExecute( seq->gfx_root, unitmat, seq->handle, &globSeqGfxData, &seq->seqGfxData, 1, 0, capeInst->pathToSkin);
        }
        copyVec3(positionOffset, seq->gfx_root->mat[3]); 
 
        // Get one used btt value  
        copyVec3(seq->seqGfxData.btt[node->anim_id], globSeqGfxData.btt[node->anim_id]);
        //Function to transform the harness.  
        {
            Mat4 bones[ARRAY_SIZE(globSeqGfxData.bpt)];
            int i;  
 
            for(i = 0 ; i < model->boneinfo->numbones ; i++)      
            {
                Vec3 * bonemat = globSeqGfxData.bpt[model->boneinfo->bone_ID[i]]; 
                mulVecMat4( globSeqGfxData.btt[node->anim_id], bonemat, bones[i][3] );  
                copyMat3(bonemat,bones[i]);
            }
            DeformObject(bones, model->vbo, model->vert_count, weightedverts, weightednorms, 0);
        }

        //DeformObject2(node, model->vbo, model->vert_count, model->vbo->verts, model->vbo->norms, weightedverts, weightednorms, model->boneinfo, &globSeqGfxData);
        if (0)
        {
            printf( "My pos:  %f %f %f \n", seq->gfx_root->mat[3][0], seq->gfx_root->mat[3][1], seq->gfx_root->mat[3][2] ); 
            printf("Verts\n"); 
            {
                int x; 
                for ( x = 0 ; x < model->vert_count ; x++ )
                {
                    printf( "%d:  %f %f %f \n", x, weightedverts[x][0], weightedverts[x][1], weightedverts[x][2] ); 
                }
                for(x = 0 ; x < model->boneinfo->numbones ; x++)
                {
                    printf("%d, bid: %d, %f %f %f\n", x, model->boneinfo->bone_ID[x], globSeqGfxData.bpt[model->boneinfo->bone_ID[x]][3][0], globSeqGfxData.bpt[model->boneinfo->bone_ID[x]][3][1], globSeqGfxData.bpt[model->boneinfo->bone_ID[x]][3][2]);
                }
            }
            printf("\n");
            if (!(weightedverts[0][0]!=0.0 || weightedverts[0][1]!=0.0 || weightedverts[0][2]!=0.0)) {
                printf("BAD weightedverts\n");
                if (0) {
                    assert(0);
                }
            }
        }
/*        for (i=0; i<model->vert_count; i++) {
            if (weightedverts[i][0]==0.0 && weightedverts[i][1]==0.0 && weightedverts[i][2]==0.0) {
                // seqGfxData is not initialized right, gfxNode->useFlags == 0 causes this, but I'm not
                //  sure why it's 0 for a frame.  Seems to only happen at the login screen (while the
                //    model is loading?), and only if the first character does not have a cape and the
                //  second does, so I'm just going to ignore it.

                // Instead I've removed the useFlags check in the fillInSeqGfxData function, and it seems
                //  to fix it, but does 25 decisions instead of 16
                capeInst->pathToSkinCalculated = 0;
                return;
            }
        }*/

        //Update the cape
        updateCloth(seq, capeInst->clothnode, NULL, gfx_dist_from_camera, model->vert_count, weightedverts, weightednorms, positionOffset, capeInst->cape->windInfo);
    }
    else if (capeInst->type == CLOTH_TYPE_FLAG)
    {
        Mat4 chestmat;
        int i;
        F32 scale_origin_x;

        if (!capeInst->capeHarness->vbo || !capeInst->capeHarness->vbo->verts) {
            // VBO gone.  happens on loadmap and reload
            modelSetupVertexObject( capeInst->capeHarness, VBOS_DONTUSE, BlendMode(0,0));
        }

        scale_origin_x = capeInst->capeHarness->vbo->verts[0][0];

        if (seq) {
            gfxTreeFindWorldSpaceMat(chestmat, seq->gfx_root->child);
        } else {
            copyMat4(mat, chestmat);
        }

        // Transform harness verts to current location
        for (i=0; i<capeInst->capeHarness->vert_count; i++) {
            //F32 offset[] = {0, 0.1, 0.2, 0.1, 0};
            Vec3 pos;
            copyVec3(capeInst->capeHarness->vbo->verts[i], pos);
            if (capeInst->cape->scale!=0.0 && capeInst->cape->scale!=1.0) {
                // Scale all harness points from midpoint
                pos[0] = (pos[0] - scale_origin_x) * capeInst->cape->scale + scale_origin_x;
            }
            if (capeInst->cape->scaleXY[0]!=0.0 && capeInst->cape->scaleXY[0]!=1.0) {
                // Scale all harness points from midpoint
                pos[0] = (pos[0] - scale_origin_x) * capeInst->cape->scaleXY[0] + scale_origin_x;
            }
            //pos[2] += offset[i%ARRAY_SIZE(offset)];
            mulVecMat4(pos, chestmat, weightedverts[i]);
        }

        updateCloth(seq, capeInst->clothnode, mat, gfx_dist_from_camera, capeInst->capeHarness->vert_count, weightedverts, NULL, zerovec3, capeInst->cape->windInfo);
        copyVec3(chestmat[3], capeInst->clothnode->mat[3]);
    } else {
        updateClothHack(seq, capeInst->clothnode, mat, gfx_dist_from_camera);
    }

    PERFINFO_AUTO_STOP();
}

static void getColorByAge(const FxBhvr * bhvr, const ColorPair* customTint,
                          F32 age, U8* pRGB)
{
    Color c = sampleColorPath(bhvr->colornavpoint, customTint, age);
    pRGB[0] = c.rgb[0];
    pRGB[1] = c.rgb[1];
    pRGB[2] = c.rgb[2];
}

void fxGeoAnimationChangeParams(FxGeo * fxgeo, FxParams * fxp)
{
    if (fxp->numColors > 0  && !fxp->isUserColored) {
        copyVec3(fxp->colors[0], fxgeo->seq_tint_color[0]);
    } else {
        setVec3(fxgeo->seq_tint_color[0], 255, 255, 255);
    }
    if (fxp->numColors > 1  && !fxp->isUserColored) {
        copyVec3(fxp->colors[1], fxgeo->seq_tint_color[1]);
    } else {
        setVec3(fxgeo->seq_tint_color[1], 255, 255, 255);
    }
    fxgeo->seq_textures.base = fxp->textures[0]?texFindComposite(fxp->textures[0]):NULL;
    fxgeo->seq_textures.generic = fxp->textures[1]?texFind(fxp->textures[1]):NULL;
    fxgeo->needSetCustomTextures = 1;
}

static void fxGeoUpdateAnimationColoring(FxGeo *fxgeo, F32 age)
{
    SeqInst        *seq;
    U8            rgb[3];
    U8            rgb2[3] = {255, 255, 255};

    // Simultaneous time- and texture-based tinting is not supported.
    if (!fxgeo->bhvr->bTintGeom) 
        getColorByAge( fxgeo->bhvr, &fxgeo->customTint, age, rgb);
    else
    {
        rgb[0] = fxgeo->customTint.primary.rgb[0];
        rgb[1] = fxgeo->customTint.primary.rgb[1];
        rgb[2] = fxgeo->customTint.primary.rgb[2];
        rgb2[0] = fxgeo->customTint.secondary.rgb[0];
        rgb2[1] = fxgeo->customTint.secondary.rgb[1];
        rgb2[2] = fxgeo->customTint.secondary.rgb[2];
    }

    if ( fxgeo->bhvr->bInheritGroupTint )
    {
        U32 uiTempColor = (rgb[0] * fxgeo->groupTint[0]) >> 8;
        rgb[0] = uiTempColor;
        uiTempColor = (rgb[1] * fxgeo->groupTint[1]) >> 8;
        rgb[1] = uiTempColor;
        uiTempColor = (rgb[2] * fxgeo->groupTint[2]) >> 8;
        rgb[2] = uiTempColor;
    }

    rgb[0] = ((U32)(rgb[0]+1) * fxgeo->seq_tint_color[0][0]) >> 8;
    rgb[1] = ((U32)(rgb[1]+1) * fxgeo->seq_tint_color[0][1]) >> 8;
    rgb[2] = ((U32)(rgb[2]+1) * fxgeo->seq_tint_color[0][2]) >> 8;

    rgb2[0] = ((U32)(rgb2[0]+1) * fxgeo->seq_tint_color[1][0]) >> 8;
    rgb2[1] = ((U32)(rgb2[1]+1) * fxgeo->seq_tint_color[1][1]) >> 8;
    rgb2[2] = ((U32)(rgb2[2]+1) * fxgeo->seq_tint_color[1][2]) >> 8;


    seq = hdlGetPtrFromHandle(fxgeo->seq_handle);

    if( seq ) 
    {
        if (gfxTreeNodeIsValid(seq->gfx_root, seq->gfx_root_unique_id)) {
            gfxTreeSetCustomColorsRecur( seq->gfx_root, rgb, rgb2, GFXNODE_CUSTOMCOLOR );
            if (fxgeo->needSetCustomTextures) {
                fxgeo->needSetCustomTextures = 0;
                gfxTreeSetCustomTexturesRecur( seq->gfx_root, &fxgeo->seq_textures );
                animCalculateSortFlags(seq->gfx_root->child); // this may have changed, because of custom texturing
            }
        }
    }
}

static void fxGeoUpdateObject(FxGeo * fxgeo, F32 age )
{
    U8            rgb[3] = {255, 255, 255};
    U8            rgb2[3] = {255, 255, 255};

    // Simultaneous time- and texture-based tinting is not supported.
    if (!fxgeo->bhvr->bTintGeom) 
        getColorByAge( fxgeo->bhvr, &fxgeo->customTint, age, rgb);
    else
    {
        rgb[0] = fxgeo->customTint.primary.rgb[0];
        rgb[1] = fxgeo->customTint.primary.rgb[1];
        rgb[2] = fxgeo->customTint.primary.rgb[2];
        rgb2[0] = fxgeo->customTint.secondary.rgb[0];
        rgb2[1] = fxgeo->customTint.secondary.rgb[1];
        rgb2[2] = fxgeo->customTint.secondary.rgb[2];

        // fpe 2/22/11 -- HACK to fix case where white palette entry cause some fx to not get tinted:
        //    Don't allow pure white for either tint color, because this is 
        //    interpreted as a flag of sorts to distinguish between old style fx tinting and
        //    new style dual color tinting.  This check for white exists in 2 places in
        //    rendertree.c (where avsn_params.has_fx_tint is set/checked:
        //        1. initViewSortModelInline2
        //        2. drawNodeAsWorldModel
        if(rgb[0]== 255 && rgb[1]== 255 && rgb[2]== 255) rgb[2] = 254;
        if(rgb2[0]== 255 && rgb2[1]== 255 && rgb2[2]== 255) rgb2[2] = 254;
    }

    if ( fxgeo->bhvr->bInheritGroupTint )
    {
        U32 uiTempColor = (rgb[0] * fxgeo->groupTint[0]) >> 8;
        rgb[0] = uiTempColor;
        uiTempColor = (rgb[1] * fxgeo->groupTint[1]) >> 8;
        rgb[1] = uiTempColor;
        uiTempColor = (rgb[2] * fxgeo->groupTint[2]) >> 8;
        rgb[2] = uiTempColor;
    }

    gfxTreeSetCustomColors( fxgeo->gfx_node, rgb, rgb2, GFXNODE_TINTCOLOR );
}


//Tells Fxgeo to begin fading out its geometry and when it's invisible, to destroy itself.  
int fxGeoFadeOut( FxGeo * fxgeo )
{
    int i;

    assert( fxgeo->lifeStage == FXGEO_ACTIVE );

    if( !gfxTreeNodeIsValid(fxgeo->gfx_node, fxgeo->gfx_node_id) )
    {
        printToScreenLog( 0, "GEO: Bad node for %s", fxgeo->name );
        return 0; //error check becomes necessary functional element..//ONFADE TODO delete child fx anyway??? ANd in other places??
    }

    if( fxgeo->childfxid )
        fxgeo->childfxid = fxDelete( fxgeo->childfxid, SOFT_KILL );

    //Note anims automatically get the alpha of the fxgeo 

    //Kill all particle systems
    {
        int part_count;
        if( fxgeo->event )
            part_count = eaSize( &fxgeo->event->part );
        else 
            part_count = 0;
        for( i = 0 ; i < part_count ; i++ )
        {
            partSoftKillSystem(fxgeo->psys[i], fxgeo->psys_id[i]);
        }
    }

    fxgeo->lifeStage = FXGEO_FADINGOUT;
    return 1;
}

int fxGeoCameraShakeAndBlur( FxGeo * fxgeo )
{
    F32 shake;
    F32 blur;
    Vec3 cam_pos_dv;
    F32    cam_dist_sqr;
    F32 shake_dist_sqr;
    F32 blur_dist_sqr;
    F32 part;
    const FxBhvr * bhvr;
    Mat4Ptr mat;
    F32 * feelerPos;
    int ret=0;

    bhvr = fxgeo->bhvr;

    mat = fxGeoGetWorldSpot( fxgeo->handle );

    if( game_state.viewCutScene )
        feelerPos = game_state.cutSceneCameraMat[3];
    else
        feelerPos = cam_info.mat[3];

    subVec3(  mat[3], feelerPos, cam_pos_dv );
    cam_dist_sqr = lengthVec3Squared( cam_pos_dv );
    shake_dist_sqr = bhvr->shakeRadius * bhvr->shakeRadius;
    blur_dist_sqr = bhvr->blurRadius * bhvr->blurRadius;

    if( cam_dist_sqr < shake_dist_sqr )
    {
        //note inverted square fall off, not linear
        part = (shake_dist_sqr - cam_dist_sqr) / shake_dist_sqr;
        shake = bhvr->shake * part;
        if( !optionGet(kUO_DisableCameraShake) && shake > MIN_SHAKE_TO_BOTHER_WITH )
        {
            ///xyprintf(5,18,"######### %0.0f ###########", shake);
            game_state.camera_shake = shake;
            game_state.camera_shake_falloff = bhvr->shakeFallOff;
            ret = 1;
        }
    }
    if( cam_dist_sqr < blur_dist_sqr )
    {
        //note inverted square fall off, not linear
        part = (blur_dist_sqr - cam_dist_sqr) / blur_dist_sqr;
        blur = bhvr->blur * part;
        if( blur > MIN_BLUR_TO_BOTHER_WITH )
        {
            ///xyprintf(5,18,"######### %0.0f ###########", blur);
            game_state.camera_blur = MAX(blur, game_state.camera_blur);
            game_state.camera_blur_falloff = MAX(bhvr->blurFallOff, game_state.camera_blur_falloff);
            ret = 1;
        }
    }
    return ret;
}


//Quick cut and paste from my player turn stuff.  
void fxGeoLayFlat( FxGeo * fxgeo )
{
    Vec3 destpyr;
    Vec3 oldpyr;
    Vec3 newpyr;
    F32 percent_new,precalcnewpyr, progress;

    //#### What is the destination pyr?
    destpyr[0] = 0;
    destpyr[1] = 0;
    destpyr[2] = 0;

    //#### What is the entity's current pyr?
    getMat3YPR(fxgeo->gfx_node->mat, oldpyr);

    //#### how far should I go?
    //    if( TIMESTEP < player->time_to_arrive_at_correct_facing )
    //        percent_new = TIMESTEP / player->time_to_arrive_at_correct_facing;//true linear
    //    else
    //        percent_new = 1.0;
    percent_new = 0.05; 

    //#### if you don't go a certain minimum turn speed, boost the percent new to get the minimum turn speed
    //I don't know if this is worth having...
    precalcnewpyr = interpAngle( percent_new, oldpyr[1], destpyr[1] );
    progress = subAngle(precalcnewpyr, oldpyr[1]);
    if(progress < 0)
        progress *= -1;
    if(progress < 0.02 * TIMESTEP) 
        percent_new = MIN( 1.0, percent_new * (0.02 * TIMESTEP)/progress);

    //#### Interpolate between the two pyrs
    //if flying include yaw, add to rotate vec
    newpyr[0] = interpAngle( percent_new, oldpyr[0], destpyr[0] );
    newpyr[1] = oldpyr[1];  //newpyr[1] = oldpyr[1] * (1-percent_new) + destpyr[1] * percent_new;
    newpyr[2] = interpAngle( percent_new, oldpyr[2], destpyr[2]  ); 

    createMat3YPR(fxgeo->gfx_node->mat, newpyr);
}


//This is currently a bit of a mess
int fxGeoUpdate( FxGeo * fxgeo, int * fxobject_flags, U8 inheritedAlpha, F32 animScale )
{
    U8            usedAlpha;
    int            still_going, i;
    int            collision=0;
    F32            len, scale;
    Mat4        newMat, tempmat, tempmat2;
    Vec3        moveto, temp, scalevec;
    const FxBhvr *bhvr;
    int            thisIsMyFirstUpdate;
    F32            timestep = TIMESTEP * animScale;

    PERFINFO_AUTO_START("fxGeoUpdate", 1);
    PERFINFO_AUTO_START("initial", 1);

    //########### Check GfxNode for Validity and set up basic variables
    if( !gfxTreeNodeIsValid(fxgeo->gfx_node, fxgeo->gfx_node_id) ) 
    {
        printToScreenLog( 0, "GEO: Bad node for %s", fxgeo->name );
        PERFINFO_AUTO_STOP_CHECKED("initial");
        PERFINFO_AUTO_STOP_CHECKED("fxGeoUpdate");
        return 0; //error check becomes necessary functional element..
    }

    still_going = 1;
    bhvr = fxgeo->bhvr;
    copyVec3( fxgeo->gfx_node->mat[3], fxgeo->position_last_frame );
    fxGeoGetWorldSpot(fxgeo->handle);
    copyMat4( fxgeo->world_spot, fxgeo->world_spot_last_frame );


    thisIsMyFirstUpdate = fxgeo->age ? 0 : 1; //allows an age zero update (for color and fade in esp)
    fxgeo->age += timestep;  //At top so world spot update is timed right

    //#### Update Camera Shake #################
    if( thisIsMyFirstUpdate && (bhvr->shake || bhvr->blur) )
        fxGeoCameraShakeAndBlur( fxgeo );

    //#### Update Appearacne ####################################
    //Fade In and Fade out
    if( fxgeo->lifeStage == FXGEO_FADINGOUT && !fxGeoWillBecomeDebris(fxgeo))
    {
        int alpha;
        fxgeo->fadingOutTime += ( bhvr->fadeoutrate * timestep );
        alpha = (int) ( 255 * ( 1.0 - (fxgeo->fadingOutTime / bhvr->fadeoutlength) ) );
        alpha = MIN(alpha, bhvr->alpha );
        fxgeo->alpha = MAX(0, alpha );
    }
    else if( fxgeo->alpha < bhvr->alpha ) //If we haven't gotten up to full fade in
    {
        int alpha;
        fxgeo->fadingInTime += ( bhvr->fadeinrate * timestep );
        alpha = (int) ( 255 * ( fxgeo->fadingInTime / bhvr->fadeinlength ) );
        alpha = MIN(alpha, bhvr->alpha );
        fxgeo->alpha = MAX(1, alpha );    
    }
    else
    {
        fxgeo->alpha = bhvr->alpha;
    }

    //Use the lower of the fxgeo's own alpha and the inherited alpha
    usedAlpha = MIN( inheritedAlpha, fxgeo->alpha );

    gfxNodeSetAlpha(fxgeo->gfx_node,usedAlpha, 1);//(TO DO: is that really what I want??, or just for this one?)

    //### Update Object 
    if(fxgeo->gfx_node->model)
        fxGeoUpdateObject( fxgeo,  ( fxgeo->age - timestep ) ); //use the age last frame
    if (fxgeo->seq_handle)
        fxGeoUpdateAnimationColoring( fxgeo,  ( fxgeo->age - timestep ) ); //use the age last frame

    //#### Update physics behavior state ################################################

    PERFINFO_AUTO_STOP_CHECKED("initial");
    PERFINFO_AUTO_START("Geo Physics", 1);
    if( fxgeo->fxgeo_flags != 16 )
    {
#ifdef NOVODEX
        NwEmissaryData* pEmissary = fxgeo->nxEmissary?nwGetNwEmissaryDataFromGuid(fxgeo->nxEmissary):NULL;

        if ( pEmissary )
        {
            if ( pEmissary->pActor )
            {
                Vec3 vNodeScale;
                PERFINFO_AUTO_START("getVelocity", 1);
                nwGetActorLinearVelocity( pEmissary->pActor, fxgeo->velocity );
                nwGetActorAngularVelocity(pEmissary->pActor, fxgeo->spinrate );

                PERFINFO_AUTO_STOP_START("getTransformAndScale", 1);
                extractScale(fxgeo->gfx_node->mat, vNodeScale);
                nwGetActorMat4( pEmissary->pActor, (float*)fxgeo->gfx_node->mat);
                scaleMat3Vec3(fxgeo->gfx_node->mat, vNodeScale);

                PERFINFO_AUTO_STOP_CHECKED("getTransformAndScale");
            }
        }
        else
#endif
        {
            #if NOVODEX
                fxgeo->nxEmissary = 0; // if we can't find it from the guid, it must be gone
            #endif
            
            PERFINFO_AUTO_START("OldPhys", 1);
            fxgeo->velocity[1] -= bhvr->gravity * timestep;

            if ( bhvr->drag )
            {
                Vec3 vDragDV;
                scaleVec3(fxgeo->velocity, (bhvr->drag * timestep), vDragDV);
                subVec3(fxgeo->velocity, vDragDV, fxgeo->velocity);
            }
    

            //## Position ####
            scaleVec3(fxgeo->velocity, timestep, temp);
            addVec3( temp, fxgeo->gfx_node->mat[3], fxgeo->gfx_node->mat[3] );
            PERFINFO_AUTO_STOP_CHECKED("OldPhys");
        }


        //Update position if relevant
        PERFINFO_AUTO_START("Update geo mat4", 1);
        if( gfxTreeNodeIsValid(fxgeo->origin, fxgeo->originid) && fxgeo->event && fxgeo->event->update )
        {
            Mat4 posit;
            gfxTreeFindWorldSpaceMat( posit, fxgeo->origin );

            // figure out from event update flags what to copy over
            applyTransformFlags(posit, fxgeo->gfx_node->mat, fxgeo->clampMinScale, fxgeo->clampMaxScale, fxgeo->event->update );
        }

        PERFINFO_AUTO_STOP_START("transposeMat4", 1);
        if(fxgeo->isRoot && fxgeo->gfx_node->parent)
        {
            // You can only do a transpose if the matrix is an orthogonal matrix, implying no scaling
            //transposeMat4Copy(fxgeo->gfx_node->parent->mat, fxgeo->gfx_node->mat);
            invertMat4Copy(fxgeo->gfx_node->parent->mat, fxgeo->gfx_node->mat);
        }

        PERFINFO_AUTO_STOP_START("update_spin", 1);
#ifdef NOVODEX
        if ( fxgeo->nxEmissary == 0 )
#endif
        {
            copyVec3(fxgeo->bhvr_spin, fxgeo->spinrate);
            scaleVec3( fxgeo->bhvr_spin, timestep, temp );
            rotateMat3( temp, fxgeo->gfx_node->mat );
            addVec3( fxgeo->cumulativespin, temp, fxgeo->cumulativespin );
        }
        PERFINFO_AUTO_STOP_CHECKED("update_spin");
    }

    PERFINFO_AUTO_STOP_CHECKED("Geo Physics");

    PERFINFO_AUTO_START("Process forces", 1);

#if NOVODEX
    if ( nwEnabled() && fxgeo->bhvr->physForceType != eNxForceType_None )
    {
        fxGeoGetWorldSpot(fxgeo->handle);

        if ( fxgeo->bhvr->physForceType == eNxForceType_Drag )
        {
            // "Collision" force type isn't really a force, it's a kinematic sphere
            NwEmissaryData* pData = nwGetNwEmissaryDataFromGuid(fxgeo->nxForceCollisionSphere);

            if ( !pData    )
            {
                // create it
                fxgeo->nxForceCollisionSphere = nwPushSphereActor((float*)fxgeo->world_spot, zerovec3, zerovec3, -1.0f, fxgeo->bhvr->physForceRadius, nxGroupKinematic, NULL, true, NX_CLIENT_SCENE);
                pData = nwGetNwEmissaryDataFromGuid(fxgeo->nxForceCollisionSphere);
                if ( pData )
                    pData->bNotifyOnEndTouch = true;
            }
            else
            {
                // update position
                if ( pData->pActor )
                    nwSetActorMat4(pData->uiActorGuid, (float*)fxgeo->world_spot);
            }
        }
        else
        {
            F32 fPhysForceScalar = 1.0f;
            if ( fxgeo->bhvr->physForceSpeedScaleMax )
            {
                Vec3 vDiff;
                F32 fVelocity;
                subVec3(fxgeo->world_spot[3], fxgeo->world_spot_last_frame[3], vDiff);
                fVelocity = global_state.frame_time_30 > 0.0f ? lengthVec3(vDiff) /  ( global_state.frame_time_30 * (1.0f/30.0f) ) : 0.0f;

                fPhysForceScalar = CLAMP(fVelocity / fxgeo->bhvr->physForceSpeedScaleMax, 0.0f, 1.0f);
            }


            fxApplyForce( fxgeo->world_spot[3], fxgeo->bhvr->physForceRadius, fxgeo->bhvr->physForcePower * fPhysForceScalar, fxgeo->bhvr->physForcePowerJitter, fxgeo->bhvr->physForceCentripetal, fxgeo->bhvr->physForceType, fxgeo->world_spot);
        }
    }
#endif

    PERFINFO_AUTO_STOP_CHECKED("Process forces");

#ifdef NOVODEX_FLUIDS
    if ( fxgeo->fluidEmitter.nxFluid != NULL )
    {
        Mat4 newFluidEmitterMat;
        Vec3 vOffset;
        zeroVec3(vOffset);
        vOffset[1] = 5.0f;
        copyMat4(fxgeo->gfx_node->mat, newFluidEmitterMat);
        pitchMat3(RAD(90.0f), newFluidEmitterMat);

        addVec3(newFluidEmitterMat[3], vOffset, newFluidEmitterMat[3]);
        nwUpdateFluidEmitterTransform( fxgeo->fluidEmitter.nxFluid, newFluidEmitterMat );
        
        // Render for now, lines
        {
            if ( fxgeo->fluidEmitter.currentNumParticles && fxgeo->fluidEmitter.particlePositions )
            {
                //drawFluid( &fxgeo->fluidEmitter );
                /*
                //modelDrawFluidParticles(&fxgeo->fluidEmitter);
                U32 uiCurrentParticle;
                for (uiCurrentParticle=1; uiCurrentParticle<fxgeo->fluidEmitter.currentNumParticles; ++uiCurrentParticle )
                {
                    drawLine3D(fxgeo->fluidEmitter.particlePositions[uiCurrentParticle-1], fxgeo->fluidEmitter.particlePositions[uiCurrentParticle], 0xffffffff );
                }
                */
            }
        }
        
    }
#endif
    PERFINFO_AUTO_START("Scaling", 1);

    //##Scaling ####
    //TO DO : bhvr->scale should be scaled by power
    if( fxgeo->is_scaling )
    {
        F32 percentDone;
        Vec3 newScale;
        Vec3 oldScaleRemover;
        int dimensionsDone;

        dimensionsDone = 0;
        for( i = 0 ; i < 3 ; i++ )
        {
            percentDone = fxgeo->age / bhvr->scaleTime[i];
            if( percentDone > 1.0 )
            {
                dimensionsDone++;
                percentDone = 1.0;
            }
            newScale[i] = bhvr->scale[i] + (percentDone * (bhvr->endscale[i] - bhvr->scale[i]));
        }
        
        oldScaleRemover[0] = 1.0/fxgeo->cumulativescale[0];
        oldScaleRemover[1] = 1.0/fxgeo->cumulativescale[1];
        oldScaleRemover[2] = 1.0/fxgeo->cumulativescale[2];
    
        if( fxgeo->clampMinScale[0] )
            vec3RunningMin(newScale,fxgeo->clampMinScale);
        if( fxgeo->clampMaxScale[0] )
            vec3RunningMax(newScale,fxgeo->clampMaxScale);

        scaleMat3Vec3( fxgeo->gfx_node->mat, oldScaleRemover );
        scaleMat3Vec3( fxgeo->gfx_node->mat, newScale );

        copyVec3( newScale, fxgeo->cumulativescale );

        if( dimensionsDone == 3 )
            fxgeo->is_scaling = 0;
    }

    //4. Update Lookat and Magnetism.  I would clean this up, but I'm afraid even small changes at this point
    //whould cause unexpected changes in existing fx, so it will remain a little ugly
    PERFINFO_AUTO_STOP_START("Magnetism", 1);
    if( !thisIsMyFirstUpdate && fxgeo->fxgeo_flags != 16 && hdlGetPtrFromHandle(fxgeo->magnetid) )
    {
        Mat4Ptr        targetMat;

        targetMat = fxGeoGetWorldSpot(fxgeo->magnetid);

        if( fxgeo->gfx_node->parent ) // is local to something
        {
            Mat4Ptr mat2;
            Mat4    mat1;
            Mat4    inv2;
            mat2 = fxGeoGetWorldSpot(fxgeo->handle);
            invertMat4Copy(mat2, inv2);
            mulMat4(inv2, targetMat, mat1);
            targetMat = (Mat4Ptr)mat1;
        }

        if( fxgeo->hasCollided )
        {
            copyVec3( targetMat[3], fxgeo->gfx_node->mat[3] );
            //TO DO if you have collided with the target, become local to your target in a sense.
        }
        else
        {
            fxLookAt( fxgeo->gfx_node->mat[3], targetMat[3], newMat );        
            copyMat3( newMat, fxgeo->gfx_node->mat );

            scale = timestep * bhvr->trackrate; 
            scaleVec3( fxgeo->gfx_node->mat[2], scale, moveto );
            addVec3( fxgeo->gfx_node->mat[3], moveto, fxgeo->gfx_node->mat[3] );

            //reintroduce scale...
            scaleMat3Vec3( fxgeo->gfx_node->mat, fxgeo->cumulativescale );
            //reintroduce spin...
            rotateMat3( fxgeo->cumulativespin, fxgeo->gfx_node->mat );

            collision = 0;
            if( nearSameVec3Tol( fxgeo->gfx_node->mat[3], targetMat[3], 0.001 ) )
            {
                collision = 1;
            }
            else if( !thisIsMyFirstUpdate ) //if positon position_last_frame is valid, really redundant
            {
// i'm commenting some of the redundancy out to make /analyze happy... this code could be rewritten though -GG
                Vec3 frame1, frame2;
                U32 sign1[3];
                U32 sign2[3];

                subVec3( targetMat[3], fxgeo->position_last_frame, frame1 );
                subVec3( targetMat[3], fxgeo->gfx_node->mat[3],    frame2 );
                sign1[0] =    (frame1[0] > 0.0) ? 1 : 0;
//                 sign1[1] =    (frame1[1] > 0.0) ? 1 : 0;
//                 sign1[2] =    (frame1[2] > 0.0) ? 1 : 0;
                sign2[0] =    (frame2[0] > 0.0) ? 1 : 0;
//                 sign2[1] =    (frame2[1] > 0.0) ? 1 : 0;
//                 sign2[2] =    (frame2[2] > 0.0) ? 1 : 0;
                if( sign1[0] != sign2[0] ) // && sign1[0] != sign2[0] &&  sign1[0] != sign2[0] )
                    collision = 1;
            }

            if( collision )     
            {
                fxgeo->hasCollided = 1;
                copyVec3( targetMat[3], fxgeo->gfx_node->mat[3] );
            }

            if( collision && !_strnicmp( fxgeo->name, "Prime", 5 ) )
            {
                int flag = 0;
                if( !_stricmp( fxgeo->name, "Prime" ) )
                    flag |= FX_STATE_PRIMEHIT;  
                else if( !_stricmp( fxgeo->name, "Prime1" ) )
                    flag |= FX_STATE_PRIME1HIT; //this is a bit clunky, but I'm in a hurry
                else if( !_stricmp( fxgeo->name, "Prime2" ) )
                    flag |= FX_STATE_PRIME2HIT;
                else if( !_stricmp( fxgeo->name, "Prime3" ) )
                    flag |= FX_STATE_PRIME3HIT;
                else if( !_stricmp( fxgeo->name, "Prime4" ) )
                    flag |= FX_STATE_PRIME4HIT;
                else if( !_stricmp( fxgeo->name, "Prime5" ) )
                    flag |= FX_STATE_PRIME5HIT;
                else if( !_stricmp( fxgeo->name, "Prime6" ) )
                    flag |= FX_STATE_PRIME6HIT;
                else if( !_stricmp( fxgeo->name, "Prime7" ) )
                    flag |= FX_STATE_PRIME7HIT;
                else if( !_stricmp( fxgeo->name, "Prime8" ) )
                    flag |= FX_STATE_PRIME8HIT;
                else if( !_stricmp( fxgeo->name, "Prime9" ) )
                    flag |= FX_STATE_PRIME9HIT;
                else if( !_stricmp( fxgeo->name, "Prime10" ) )
                    flag |= FX_STATE_PRIME10HIT;

                if(flag)
                    *fxobject_flags |= flag; 
            }
        }
    }

    PERFINFO_AUTO_STOP_START("Lookat", 1);
    if( !fxgeo->hasCollided && ((hdlGetPtrFromHandle(fxgeo->lookatid) || fxgeo->fxgeo_flags & FXGEO_LOOKAT_CAMERA)) && fxgeo->fxgeo_flags != 16)
    {    
        Mat4Ptr mat = NULL;

        //Maybe change to FxGeos so I can use world spot?
        if( hdlGetPtrFromHandle(fxgeo->lookatid) )
            mat = fxGeoGetWorldSpot(fxgeo->lookatid);
        else if( fxgeo->fxgeo_flags & FXGEO_LOOKAT_CAMERA )
            mat = cam_info.cammat;

        //TO DO : I think this local xform is totally crazy, why did I do it? 
        if( fxgeo->gfx_node->parent ) // is local to something
        {
            Mat4Ptr mat2;
            Mat4    mat1;
            Mat4    inv2;
            mat2 = fxGeoGetWorldSpot(fxgeo->handle);
            invertMat4Copy(mat2, inv2);
            mulMat4(inv2, mat, mat1);
            mat = (Mat4Ptr)mat1;
        }

        len = fxLookAt( fxgeo->gfx_node->mat[3], mat[3],  newMat ); 
        copyMat3( newMat, fxgeo->gfx_node->mat);

        //reintroduce scale...
        copyVec3( fxgeo->cumulativescale, scalevec );

        if( bhvr->stretch >= FX_STRETCH )
        {
            Vec3 dv;
            subVec3( fxgeo->gfx_node->mat[3], mat[3], dv );
            len = lengthVec3( dv );
            scalevec[2] *= len;    
        }
        
        copyMat4( unitmat, tempmat);
        scaleMat3Vec3( tempmat, scalevec );
        mulMat4( fxgeo->gfx_node->mat, tempmat,  tempmat2 );
        copyMat4( tempmat2, fxgeo->gfx_node->mat );        
        
        //reintroduce spin
        rotateMat3( fxgeo->cumulativespin, fxgeo->gfx_node->mat );

        if( bhvr->stretch == FX_STRETCHANDTILE )
        {
            TrickNode * tricks = gfxTreeAssignTrick( fxgeo->gfx_node );
            tricks->flags2 |= TRICK2_STSSCALE;
            tricks->tex_scale_y = scalevec[2];
//            tricks->tex_scale[0] = 1;
//            tricks->tex_scale[1] = scalevec[2];
//            tricks->tex_scale[2] = 1;
        }
    }

    if (( fxgeo->gfx_node->model ) &&
        ( fxgeo->gfx_node->model->trick ) &&
        ( fxgeo->gfx_node->model->trick->info ) &&
        ( fxgeo->gfx_node->model->trick->info->stAnim ))
    {
        // Update or initialize the texture animation state info.
        // < 0.0 means "initialize me"
        fxgeo->gfx_node->model->trick->st_anim_age = 
            ( fxgeo->gfx_node->model->trick->st_anim_age < 0.0f ) ?
                0.0f : ( fxgeo->gfx_node->model->trick->st_anim_age + TIMESTEP );
    }
    
    PERFINFO_AUTO_STOP_START("AtCamera", 1);
    if(fxgeo->fxgeo_flags & FXGEO_AT_CAMERA && fxgeo->fxgeo_flags != 16)
    {    
        Vec3 offset;
        // DGNOTE 4/27/2009 - This vec3 has the near clip distance plus small offset as its 3rd
        // element.  This is set to match the zNear parameter handed to glFrustum over in rt_init.c
        // Do *NOT* mess with this vector, unless (1) you really really understand what you're doing,
        // and (2) you were explicitly sent here by a comment in rt_init.c
        static Vec3 zNear = {0.0f, 0.0f, 4.01f};
        // I meant that.  You didn't touch that vector, did you?

        mulVecMat3(zNear, cam_info.cammat, offset);
        copyMat3(cam_info.cammat, fxgeo->gfx_node->mat);
        subVec3(cam_info.cammat[3], offset, fxgeo->gfx_node->mat[3]);
        // For reasons I can't be bothered to track down, some particles set their world_spot to the
        // fred point of the Entity they're centered on, and totally blow off my carefully crafted
        // gfx_node->mat.  So I just copy it over manually here, overriding any damage that might be done.
        // If particles ever start playing wrong (i.e. centered on the Entity in world space), come here
        // and start looking.  I'd recommend trapping really early in the fxgeo creation process, and
        // sticking a write breakpoint on fxgeo->world_spot[3][0]
        copyMat4(fxgeo->gfx_node->mat, fxgeo->world_spot);
    }

    //Something of a HACK to handle things that are supposed to shoot toward a target while facing the target
    //if( fxgeo->hasCollided && !thisIsMyFirstUpdate && fxgeo->lookatid && fxgeo->magnetid  ) //don't go wheeling all over the place, just hit and stop 
    //    copyMat3(orientation_last_frame, fxgeo->gfx_node->mat );

    PERFINFO_AUTO_STOP_START("collides hack", 1);
    if( bhvr->collides && !fxgeo->fxgeo_flags != 16 )
    {
        CollInfo coll; 
        Entity *player = playerPtr();
        if( collGrid(0, fxgeo->position_last_frame, fxgeo->gfx_node->mat[3], &coll, 0, COLL_DISTFROMSTART) )
        {
            copyVec3(coll.mat[3], fxgeo->gfx_node->mat[3]);
            fxgeo->gfx_node->mat[3][1] += 0.01;
            fxgeo->fxgeo_flags = 16;
            //fxGeoLayFlat( fxgeo );
        }
    }
    //Quick hack, do better later (in order of importance
    //1. Have it lay flat with coll.mat, and only after it hits
    //2. Have it fly through the air with its current spin and velocity (use fxgeo->matprevframe )
    //3. Have it bounce or spin, or whatever
    if( 0 && bhvr->collides )
    {
        fxGeoLayFlat( fxgeo );
    }

    //Check for scaling state change.
    if( fxgeo->cumulativescale[2] >= bhvr->endscale[2] && !_stricmp( fxgeo->name, "Prime" ) ) 
            *fxobject_flags |= FX_STATE_PRIMEBEAMHIT; 

    fxgeo->gfx_node->flags |= GFXNODE_NEEDSVISCHECK;

    PERFINFO_AUTO_STOP_CHECKED("collides hack");
    PERFINFO_AUTO_STOP_CHECKED("fxGeoUpdate");

    if ( fxGeoWillBecomeDebris(fxgeo) && fxgeo->lifeStage == FXGEO_FADINGOUT )
        return 0;

    return ( fxgeo->alpha ? 1 : 0 ); //if the alpha hits zero, then it's dead...
}

void fxGeoDestroy(FxGeo * fxgeo, int * fxobject_flags, void ** fxgeolist, int * fxgeo_count, int killtype, int fxobjectid)
{
    int fxid;
    int i;

    bool bMakeDebris = fxGeoWillBecomeDebris(fxgeo);

    #if NOVODEX
    {
        if ( bMakeDebris )
        {
            Vec3 vScale;
            bool bJointed = false;
            NwEmissaryData* pData = nwGetNwEmissaryDataFromGuid(fxgeo->nxEmissary);
            if ( pData )
            {
                bJointed = !!(pData->bCurrentlyJointed);
            }

            fxGeoGetWorldSpot(fxgeo->handle);
            getScale(fxgeo->world_spot, vScale);
            //mulVecVec3(fxgeo->bhvr->scale, vScale, vScale);
            bMakeDebris = fxAddDebris(fxgeo, vScale, bJointed?fxobjectid:0);
        }

        if ( fxgeo->nxForceCollisionSphere && nwEnabled() )
        {
            nwDeleteActor(nwGetActorFromEmissaryGuid(fxgeo->nxForceCollisionSphere), NX_CLIENT_SCENE);
            fxgeo->nxForceCollisionSphere = 0;
        }
    }
    #endif

    fxid = hdlGetHandleFromPtr(fxgeo, fxgeo->handle);
    if( !fxid || !*fxgeolist || !fxobject_flags )
        return;
    #ifdef NOVODEX
    if ( fxgeo->nxEmissary && nwEnabled() && !bMakeDebris)
    {
        nwDeleteActor(nwGetActorFromEmissaryGuid(fxgeo->nxEmissary), NX_CLIENT_SCENE);
        fxgeo->nxEmissary = 0;
    }
    #endif

#ifdef NOVODEX_FLUIDS
    if ( fxgeo->fluidEmitter.nxFluid )
    {
        nwDeleteFluid( fxgeo->fluidEmitter.nxFluid, NX_CLIENT_SCENE );
        fxgeo->fluidEmitter.nxFluid = NULL;
    }
    free( fxgeo->fluidEmitter.particlePositions );
    free( fxgeo->fluidEmitter.particleVelocities );
    free( fxgeo->fluidEmitter.particleAges );
    free( fxgeo->fluidEmitter.particleDensities );

    fxgeo->fluidEmitter.particlePositions = NULL;
    fxgeo->fluidEmitter.particleVelocities = NULL;
    fxgeo->fluidEmitter.particleAges = NULL;
    fxgeo->fluidEmitter.particleDensities = NULL;
    fxgeo->fluidEmitter.currentNumParticles = 0;
#endif
    
    //1. Set flags, I donno if this is the best way
    if( !_stricmp( fxgeo->name, "Prime") )
        *fxobject_flags |= FX_STATE_PRIMEDIED;

    //2. Kill the FxGeo
    if( fxgeo->childfxid )
    {
        fxDestroyForReal( fxgeo->childfxid, SOFT_KILL );
    }

    //3. If you have any world geometry, and that world geometry itself had special fx
    //    then kill them using the miniTracker that you used to emulate a tracker for the world geometry
    if( fxgeo->fxMiniTracker )
    {
        int i;
        for( i = 0 ; i < fxgeo->fxMiniTracker->count ; i++ )
        {
            fxDestroyForReal( fxgeo->fxMiniTracker->fxIds[i], SOFT_KILL );
        }
        if (fxgeo->fxMiniTracker->tracker)
        {
            trackerClearDefs(fxgeo->fxMiniTracker->tracker);
            trackerClose(fxgeo->fxMiniTracker->tracker);
            groupRemoveAuxTracker(fxgeo->fxMiniTracker->tracker);
            free(fxgeo->fxMiniTracker->tracker);
        }
        free( fxgeo->fxMiniTracker );
        fxgeo->fxMiniTracker = 0;
    }

    //4. If there's an attached library piece
    if( fxgeo->worldGroupName )
    {
        free( fxgeo->worldGroupName );
        fxgeo->worldGroupName = 0;
    }

    if( fxgeo->seq_handle)
    {
        SeqInst * seq;
        
        //if the seq is dead for some reason (should never happen)
        seq = hdlGetPtrFromHandle(fxgeo->seq_handle);

        if(seq) 
        {
            //if the seq has had it's gfxnodes blown out from under it, don't be deleting gfxnodes that are unknown...
            if( gfxTreeNodeIsValid( seq->gfx_root, seq->gfx_root_unique_id ) )
            {
                seq->gfx_root = 0;
            }
            seqFreeInst( seq );
            
        }
        fxgeo->seq_handle = 0;
    }

    if( gfxTreeNodeIsValid(fxgeo->gfx_node, fxgeo->gfx_node_id) && !bMakeDebris)
    {
        gfxNodeClearFxUseFlag(fxgeo->gfx_node->parent );
        gfxTreeDelete(fxgeo->gfx_node);
    }

    if( gfxTreeNodeIsValid(fxgeo->splatNode, fxgeo->splatNodeUniqueId) )
    {
        gfxTreeDelete(fxgeo->splatNode);
    }

    //If this Fxgeo got a fade out command, this has already been done...
    {
        int part_count;
        if( fxgeo->event )
            part_count = eaSize( &fxgeo->event->part );
        else 
            part_count = 0;
        for( i = 0 ; i < part_count ; i++ )
        {
            if(killtype == SOFT_KILL)
                partSoftKillSystem(fxgeo->psys[i], fxgeo->psys_id[i]);
            else
                partKillSystem(fxgeo->psys[i], fxgeo->psys_id[i]);
        }
    }
    
    if(fxgeo->rgbs)
        free(fxgeo->rgbs);

    if (fxgeo->capeInst)
        destroyFxCapeInst(fxgeo->capeInst);

    if (fxgeo->bhvrOverride)
        free((FxBhvr *)fxgeo->bhvr);

    debug_fxgeo_count--;
    listRemoveMember(fxgeo, fxgeolist);
    hdlClearHandle(fxid);
    mpFree( fx_engine.fxgeo_mp, fxgeo );
    

    (*fxgeo_count)--;
}


/////////////////////////////////////////////////////////
///////////////

static void getCenter( Vec3 start, Vec3 end, Vec3 center )
{
    subVec3( end, start, center );
    scaleVec3( center, 0.5, center );
    addVec3( start, center, center ); 
}


void fxGeoUpdateSplat( FxGeo * fxgeo, U8 actualAlpha )
{
    static SplatParams sp;
    SplatParams * splatParams;
    Mat4Ptr mat;
    SplatEvent * splatEvent;
    Splat* splat;
    const FxBhvr *bhvr = fxgeo->bhvr;

    PERFINFO_AUTO_START("fxGeoUpdateSplat", 1);


    assert( fxgeo->event->splat[0] && eaSize( &(fxgeo->event->splat ) ) == 1 ); //only one splat per fxgeo

    splatEvent = fxgeo->event->splat[0];

    splatParams = &sp; 
    {
        //Make sure you have a good node   
        if( !gfxTreeNodeIsValid(fxgeo->splatNode, fxgeo->splatNodeUniqueId) )
        { 
            int doInvertedSplat = (bhvr->splatFalloffType == SPLAT_FALLOFF_BOTH) ? 1 : 0;
            fxgeo->splatNode = initSplatNode( &fxgeo->splatNodeUniqueId, splatEvent->texture1, splatEvent->texture2, doInvertedSplat );
        }  
        assert( gfxTreeNodeIsValid(fxgeo->splatNode, fxgeo->splatNodeUniqueId) );
        splatParams->node = fxgeo->splatNode;  
    }

    //node+uniqueID + shadowParams
    //Fill out the shadow node parameters:
    splatParams->shadowSize[0] = 0.5f * MAX(bhvr->scale[0], bhvr->endscale[0]);
    splatParams->shadowSize[2] = 0.5f * MAX(bhvr->scale[2], bhvr->endscale[2]);

    splatParams->shadowSize[1] = 0.5f * MAX(bhvr->scale[1], bhvr->endscale[1]);

    //Get alpha for shadow
    {
        F32 distAlpha;
        F32 drawDist;
        U8 maxAlpha; //maxAlpha possible under the circumstances
        
        drawDist = SHADOW_BASE_DRAW_DIST * ( splatParams->shadowSize[0] / SEQ_DEFAULT_SHADOW_SIZE );
 
        //If you are outside of your comfort zome, fade out over 30 feet
        distAlpha = 255;//(1.0 - (( distFromCamera - drawDist ) / SHADOW_FADE_OUT_DIST )) ; 
        distAlpha = MINMAX( distAlpha, 0, 1.0 );

        //Find MaxAlpha
        maxAlpha = actualAlpha;                                //Node's alpha
        maxAlpha = MIN( maxAlpha, (U8)(distAlpha * 255) );          //Distance from camera 
        
        splatParams->maxAlpha = maxAlpha;
    }
    


    mat = fxGeoGetWorldSpot( fxgeo->handle ); 


        
    //TO DO this won't work with arbitrary projections
    copyVec3( mat[3], splatParams->projectionStart );   //Get Projection Start  

    //TO DO won't work for arbitraries
    //splatSetBack = what percent of the splat should be behind the node, and what in front
    splatParams->projectionStart[1] += ( splatParams->shadowSize[1] * bhvr->splatSetBack );

    splatParams->projectionDirection[0] = 0;
    splatParams->projectionDirection[1] = -1; 
    splatParams->projectionDirection[2] = 0;


    getColorByAge( bhvr, &fxgeo->customTint, fxgeo->age, splatParams->rgb);

    copyMat4( mat, splatParams->mat );

    //Set the Collision Density Rejection Coefficient
    splatParams->max_density = 12.0;//  SPLAT_HIGH_DENSITY_REJECTION;    

    splatParams->flags            = bhvr->splatFlags; 
    splatParams->falloffType    = bhvr->splatFalloffType; 
    splatParams->normalFade        = bhvr->splatNormalFade; 
    splatParams->fadeCenter        = bhvr->splatFadeCenter; 


    if( bhvr->stAnim )
    {
        splatParams->stAnim = bhvr->stAnim[0];
        assert( splatParams->stAnim );
    }
    else
        splatParams->stAnim = 0;

    //set alpha and color based on distance from fadeCenter
    splat = splatParams->node->splat;


    {
        Vec3 vMovement;
        subVec3(splat->previousStart, splatParams->projectionStart, vMovement);
        if ( lengthVec3Squared(vMovement) > 0.001f
            || fabsf(splat->width - splatParams->shadowSize[0]) > 0.001f
            || fabsf(splat->height - splatParams->shadowSize[2]) > 0.001f
            || fabsf(splat->depth - splatParams->shadowSize[1]) > 0.001f
            )
            updateASplat( splatParams ); 
    }

    updateSplatTextureAndColors(splatParams, fxgeo->cumulativescale[0], fxgeo->cumulativescale[2]);
    PERFINFO_AUTO_STOP();

}

