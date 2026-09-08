#include <utilitieslib/utils/wininclude.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <utilitieslib/assert/assert.h>
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/utils/file.h>
#include <utilitieslib/utils/error.h>
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/components/SharedHeap.h>
#include <utilitieslib/utils/textparser.h>
#ifndef GETVRML
#include "NovodeX/NwWrapper.h"
#endif
#ifdef CLIENT
#include "render/model.h"
#include "group/groupfileload.h"
#include "render/renderprim.h"
#endif
#include "seq/anim.h"
#include "gridcoll/ctri.h"
#include <utilitieslib/components/genericlist.h>
#include <utilitieslib/components/StashTable.h>
#include <zlib/zlib.h>
#include <utilitieslib/utils/memcheck.h>
#include "render/tex.h"
#include "seq/tricks.h"
#include "seq/AutoLOD.h"

#ifdef CLIENT
#include "cmdparse/cmdgame.h"
#include "graphics/gfx.h"
#include "graphics/gfxLoadScreens.h"
#include "render/model_cache.h"
#include "graphics/zOcclusion.h"
#include "render/thread/rt_init.h"
#include "graphics/groupdrawutil.h"
#include "group/groupfilelib.h"
#endif

#include <utilitieslib/utils/strings_opt.h>
#include <utilitieslib/utils/timing.h>

#ifdef SERVER 
#include "cmdparse/cmdserver.h"
#include "group/groupfilelib.h"
#include "dbcomm/dbcomm.h"
#endif

StashTable    glds_ht = 0;
static int number_of_glds_in_background_loader = 0;
static GeoLoadData * glds_ready_for_final_processing;
static int gld_num_bytes_loaded=0;
static int number_of_lods_in_background_loader = 0;

#if CLIENT
MP_DEFINE(LodModel);

typedef struct QueuedLod
{
    LodModel *lodmodel;
    Model *srcmodel;
    ReductionMethod method;
    F32 shrink_amount;
    U32 just_free_reductions : 1;
} QueuedLod;
#endif

#define MAX_GEOS 1000
#define DISABLE_PARALLEL_GEO_THREAD_LOADER 0
#define DISABLE_PARALLEL_LOD_THREAD 1

//to do put this for texture and anim in better joint place
CRITICAL_SECTION CriticalSectionTexLoadQueues;
CRITICAL_SECTION CriticalSectionTexLoadData; // blocked whenever texLoadData is running (to allow it to be called from both threads)
CRITICAL_SECTION CriticalSectionGeoLoadQueues; //for the thread linked list communication 
CRITICAL_SECTION heyThreadLoadAGeo_CriticalSection; //for the actual function (making sure it doesn't get called in both the thread and the main thread at the same time
CRITICAL_SECTION heyThreadLODAModel_CriticalSection; //for the actual function (making sure it doesn't get called in both the thread and the main thread at the same time
CRITICAL_SECTION CriticalSectionGeoUncompress;

static int initedBackgroundLoader;

HANDLE background_loader_handle = NULL;
unsigned background_loader_threadID = 0;

static void modelSetTexOptCtriFlags(Model *model, Vec3 *verts, int *tris);

typedef struct ModelSearchData {
    const char*    name;
    int        namelen;
} ModelSearchData;

static int compareModelNamesAndLengths(const char* name1, int len1, char* name2, int len2)
{
    int ret;
    
    ret = strnicmp(name1, name2, min(len1, len2));
    
    if(!ret && len1 != len2)
    {
        if(len1 < len2)
        {
            ret = -1;
        }
        else
        {
            ret = 1;
        }
    }
    
    return ret;    
}

int __cdecl compareModelBSearch(const ModelSearchData* search, const Model** model)
{
    int ret = compareModelNamesAndLengths(search->name, search->namelen, (*model)->name, (*model)->namelen_notrick);
    return ret;
}

GeoLoadData* useThisGeoLoadData;

int __cdecl compareModelBSearch(const ModelSearchData* search, const Model** model);
static int compareModelBSearchCallback(const void* arg0, const void* arg1)
{
    return compareModelBSearch((const ModelSearchData*)arg0, (const Model**)arg1);
}

Model * modelFind( const char *name, const char * filename, int load_type, int use_type )
{

    GeoLoadData * gld;
    ModelHeader * header;
    Model * model = 0;
    int len;
    char * s;

    PERFINFO_AUTO_START("modelFind", 1);
    
        assert(use_type & GEO_USE_MASK);
        if(!name || !filename || !name[0] || !filename[0] ) //debug
        {
            char buf[256] = {0};
            sprintf(buf, "Bad geometry request");
            if (filename)
            {
                sprintf(buf, " (Model: '%s')", name);
            }
            if (name)
            {
                sprintf(buf, " (File: '%s')", filename);
            }
            writeConsole(OUTPUT_VERBOSE, "%s", buf);
            PERFINFO_AUTO_STOP();
            return 0; 
        }

        if(useThisGeoLoadData){
            gld = useThisGeoLoadData;
        }else{
            gld = geoLoad(filename,load_type,use_type); //will be assumed in_background?
        }

        PERFINFO_AUTO_START("findmodel", 1);

            if(gld)  //find Model in ModelHeader
            {
                gld->geo_use_type |= use_type & GEO_USE_MASK;
                header = &gld->modelheader;

                s = strstr(name,"__"); //hack off trick file suffix
                len = s ? (int)(s - name) : (int)strlen(name); 

                #if 1
                {
                    ModelSearchData search;
                    Model** ppModel;
                
                    search.name = name;
                    search.namelen = len;
                    ppModel = bsearch(&search, header->models, header->model_count, sizeof(header->models[0]), compareModelBSearchCallback);
                    
                    if(ppModel)
                    {
                        model = *ppModel;
                    }
                }
                #else
                {
                    int i;
                    for( i = 0 ; i < header->model_count ; i++ )
                    {    
                        Model* curModel = header->models + i;
                        
                        // TO DO: uppercase everything / faster search?
                        
                        if(curModel->namelen_notrick == len)
                        {
                            s = curModel->name;
                            
                            if (s && strnicmp(s,name,len)==0 )
                            {
                                model = curModel;
                                break;
                            }
                        }
                    }
                }
                #endif
            }
            
        PERFINFO_AUTO_STOP();

    PERFINFO_AUTO_STOP();                         
    return model;
}
    
static void unpackNames(void *mem,PackNames *names)
{
int        i,count;
int        *imem,*idxs;
char    **idxs_ptr,*base_offset;

    imem = (void *)mem;
    count = imem[0];
    names->count = count;
    idxs = &imem[1];
    idxs_ptr = (void *)idxs;
    names->strings = idxs_ptr;
    base_offset = (char *)(&idxs[count]);
    for(i=0;i<count;i++)
        idxs_ptr[i] = idxs[i] + base_offset;
}

int uncompressDeltas(void *dst,U8 *src,int stride,int count,PackType pack_type)
{
    int        i,j,code,iDelta,cur_bit=0;
    Vec3    fLast = {0,0,0};
    int        iLast[3] = {0,0,0};
    F32        inv_float_scale = 1.f;
    F32        *fPtr = dst;
    int        *iPtr = dst;
    U16        *siPtr = dst;
    U32        *bits = (U32 *)src;
    U8        *bytes = src + (2 * count * stride + 7)/8;
    F32        float_scale;

    if (!count)
        return 0;

    float_scale = 1 << *bytes++;
    if (float_scale)
        inv_float_scale = 1.f / float_scale;
    for(i=0;i<count;i++)
    {
        for(j=0;j<stride;j++)
        {
            code = (bits[cur_bit >> 5] >> (cur_bit & 31)) & 3;
            cur_bit+=2;
            switch(code)
            {
                xcase 0:
                    iDelta = 0;
                xcase 1:
                    iDelta = *bytes++ - 0x7f;
                xcase 2:
                    iDelta = (bytes[0] | (bytes[1] << 8)) - 0x7fff;
                    bytes += 2;
                xcase 3:
                    iDelta = bytes[0] | (bytes[1] << 8) | (bytes[2] << 16) | (bytes[3] << 24);
                    bytes += 4;
            }
            switch(pack_type)
            {
                xcase PACK_F32:
                {
                    F32        fDelta;

                    if (code != 3)
                        fDelta = iDelta * inv_float_scale;
                    else
                        fDelta = *((F32 *)&iDelta);
                    *fPtr++ = fLast[j] = fLast[j] + fDelta;
                }
                xcase PACK_U32:
                    *iPtr++ = iLast[j] = iLast[j] + iDelta + 1;
                xcase PACK_U16:
                    *siPtr++ = iLast[j] = iLast[j] + iDelta + 1;
            }
        }
    }
    return bytes - src;
}


#define IDX2PTR(idx,base) ((void *)((uintptr_t)idx + (uintptr_t)base))
#define PTR2IDX(ptr,base) ((void *)((uintptr_t)ptr - (uintptr_t)base))


PolyCell *polyCellUnpack(Model* model,PolyCell *cell,void *base_offset)
{
    int        i;//,t;
    int        model_tri_count;
    int        cell_tri_count;
    U16        *tri_idxs;

    if (!cell)
        return 0;
    if (!base_offset)
        base_offset = cell;
    if (cell != base_offset)
        cell = IDX2PTR(cell,base_offset);
    if (cell->children)
    {
        cell->children = IDX2PTR(cell->children,base_offset);
        for(i=0;i<8/*NUMCELLS_CUBE*/;i++)
            cell->children[i] = polyCellUnpack(model,cell->children[i],base_offset);
    }
    if (cell->tri_idxs)
        cell->tri_idxs = IDX2PTR(cell->tri_idxs,base_offset);
    //compressDeltas(cell->tri_idxs,1,cell->tri_count,PACK_U16);
    //t = sizeof(*cell) + cell->tri_count * 2 + (cell->children ? 32 : 0);

    model_tri_count = model->tri_count;
    cell_tri_count = cell->tri_count;
    tri_idxs = cell->tri_idxs;
    
    for(i=0;i<cell_tri_count;i++)
    {
        assert(tri_idxs[i] < model_tri_count);
    }
    return cell;
}

PolyCell *polyCellPack(Model* model, PolyCell *cell, void *base_offset, PolyCell* dest, void* dest_base_offset)
{
    int        i;

    if (!cell)
        return 0;
        
    if(!base_offset)
    {
        base_offset = cell;
    }
        
    if(!dest)
    {
        dest = cell;
    }
    
    if(!dest_base_offset)
    {
        dest_base_offset = dest;
    }
    
    assert((U32)(uintptr_t)PTR2IDX(dest, dest_base_offset) + sizeof(*dest) <= model->pack.grid.unpacksize);
        
    *dest = *cell;
    
    if (cell->children)
    {
        if(dest != cell)
        {
            dest->children = IDX2PTR(PTR2IDX(cell->children,base_offset), dest_base_offset);
        }
        
        for(i=0;i<8;i++)
        {
            if(dest != cell && cell->children[i])
            {
                dest->children[i] = IDX2PTR(PTR2IDX(cell->children[i],base_offset), dest_base_offset);
            }
            
            dest->children[i] = polyCellPack(model, cell->children[i], base_offset, dest->children[i], dest_base_offset);
        }
        
        dest->children = PTR2IDX(cell->children,base_offset);
    }
    
    if (cell->tri_idxs)
    {
        dest->tri_idxs = PTR2IDX(cell->tri_idxs,base_offset);
        
        assert((U32)(uintptr_t)dest->tri_idxs + sizeof(cell->tri_idxs[0]) * cell->tri_count <= model->pack.grid.unpacksize);
        
        if(dest != cell)
        {
            memcpy(IDX2PTR(dest->tri_idxs, dest_base_offset), cell->tri_idxs, sizeof(cell->tri_idxs[0]) * cell->tri_count);
        }
    }

    if (cell != base_offset)
        cell = PTR2IDX(cell,base_offset);
        
    return cell;
}

static z_stream* zStream;

static void* geoZAlloc(void* opaque, U32 items, U32 size)
{
    return malloc(items * size);
}

static void geoZFree(void* opaque, void* address)
{
    free(address);
}

static void geoInitZStream()
{
    if(!zStream)
    {
        zStream    = calloc(1, sizeof(*zStream));
        
        zStream->zalloc    = geoZAlloc;
        zStream->zfree    = geoZFree;
    }
    else
    {
        inflateEnd(zStream);
    }
    
    inflateInit(zStream);
}

static void geoUncompress(void* outbuf, U32* outsize, void* inbuf, U32 insize, char *modelname, char *filename)
{
    int ret;

    if(initedBackgroundLoader){
        // Not used for BeaconClients.
        
        EnterCriticalSection(&CriticalSectionGeoUncompress);
    }

    geoInitZStream();

    zStream->avail_out        = *outsize;
    zStream->next_out        = outbuf;
    zStream->next_in        = inbuf;
    zStream->avail_in        = insize;
    ret = inflate(zStream, Z_FINISH);
    if (!((ret == Z_OK || ret == Z_STREAM_END) && (zStream->avail_out == 0)))
    {
        char buffer[1024];
        if (modelname)
            sprintf(buffer, "geoUncompress failed for model %s in file %s", modelname, filename?filename:"unknown");
        else
            sprintf(buffer, "geoUncompress failed for file %s", filename?filename:"unknown");
        assertmsg(0, buffer);
    }

    if(initedBackgroundLoader){
        LeaveCriticalSection(&CriticalSectionGeoUncompress);
    }
}

void geoUnpackDeltas(PackData *pack,void *data,int stride,int count,int type,char *modelname,char *filename)
{
    if (!pack->unpacksize)
        return;
        
    PERFINFO_AUTO_START("geoUnpackDeltas", 1);
    
    if (pack->packsize)
    {
        U8*            unzip_buf;
        int            len_unpack;
        U8            temp_buffer[10000];

        len_unpack = pack->unpacksize;
        if(len_unpack <= sizeof(temp_buffer))
            unzip_buf = temp_buffer;
        else
            unzip_buf = malloc(len_unpack);
            
        geoUncompress(unzip_buf,&len_unpack,pack->data,pack->packsize, modelname, filename);
        uncompressDeltas(data,unzip_buf,stride,count,type);
        if(unzip_buf != temp_buffer)
            free(unzip_buf);
    }
    else
    {
        uncompressDeltas(data,pack->data,stride,count,type);
    }
    
    PERFINFO_AUTO_STOP();
}

void geoUnpack(PackData *pack,void *data,char *modelname,char *filename)
{
    if (!pack->unpacksize)
        return;

    PERFINFO_AUTO_START("geoUnpack", 1);

    if (pack->packsize)
        geoUncompress(data,&pack->unpacksize,pack->data,pack->packsize, modelname, filename);
    else
        memcpy(data,pack->data,pack->unpacksize);

    PERFINFO_AUTO_STOP();
}

#ifndef GETVRMLxx

#include "gridcoll/gridcoll.h"

int ctri_total;

void modelFreeCtris(Model *model)
{
    if (!model->grid.cell)
        return;
    if ( model->pSharedHeapHandle )
    {
        sharedHeapMemoryManagerLock();
        sharedHeapRelease(model->pSharedHeapHandle);
        model->pSharedHeapHandle = NULL;
        sharedHeapMemoryManagerUnlock();
    }
    else
    {
        SAFE_FREE(model->grid.cell);
    }
    //SAFE_FREE(model->tags);
    //SAFE_FREE(model->ctris);
    //SAFE_FREE(model->extra);
    SAFE_FREE(model->tags);

    model->grid.cell = NULL;
    model->ctris = NULL;
    model->extra = NULL;
    model->tags = NULL;

    model->grid.size = model->grid_size_orig;
}

static void modelSetNoColl(Model *model, int trick_flags, int *set_flags, int *clear_flags)
{
    // If this thing isn't a region marker, we never need to collide with it. (except for editing)
    // Setting the grid size to 0 allows the collision code to skip these objects.

    #if SERVER
    if (!(trick_flags & (GROUP_REGION_MARKER|GROUP_VOLUME_TRIGGER)))
    {
        model->grid.size = 0;
        // TRICK RELOAD TODO: how does this get undone?
    }
//    else
    #endif
    {
        *set_flags |= COLL_EDITONLY;
        *clear_flags &= ~COLL_EDITONLY;

        if (trick_flags & (GROUP_REGION_MARKER|GROUP_VOLUME_TRIGGER)) {
            *set_flags |= COLL_PORTAL;
            *clear_flags &=~ COLL_PORTAL;
        }
        *clear_flags |= COLL_NORMALTRI;
        *set_flags &=~ COLL_NORMALTRI;
    }
}

void modelSetTrickCtriFlags(Model *model)
{
    int model_nocoll=0;
    int set_flags=0;
    int clear_flags=0;
    TrickInfo *trick = model->trick?model->trick->info:NULL;
    // GEO RELOAD TODO: clear Ctri flags if the model lost it's trick?
    if (trick && model)
    {
        model_nocoll = trick->lod_near > 0 || trick->tnode.flags1 & TRICK_NOCOLL;
        if (model_nocoll)
            modelSetNoColl(model, trick->group_flags, &set_flags, &clear_flags);
        if (trick->tnode.flags1 & TRICK_PLAYERSELECT) {
            set_flags |= COLL_PLAYERSELECT;
            clear_flags &=~ COLL_PLAYERSELECT;
        } else {
            clear_flags |= COLL_PLAYERSELECT;
            set_flags &=~ COLL_PLAYERSELECT;
        }

        if( trick->tnode.flags1 & TRICK_NOCAMERACOLLIDE )
        {
            clear_flags &= ~COLL_NOCAMERACOLLIDE;
            set_flags |= COLL_NOCAMERACOLLIDE;
        }

        if (trick->tnode.flags1 & TRICK_NOT_SELECTABLE)
        {
#            if SERVER
                // MS: Doesn't matter if we turn on COLL_NOTSELECTABLE server-side for TRICK_NOCOLL objects because
                //     they've already been set to have no collision size.
                
                // Set COLL_NOTSELECTABLE, clear COLL_NORMALTRI
                set_flags |= COLL_NOTSELECTABLE;
                clear_flags &=~ COLL_NOTSELECTABLE;
                clear_flags |= COLL_NORMALTRI;
                set_flags &=~ COLL_NORMALTRI;

#            else
                // MS: This is real nasty, but it's for (TRICK_NOT_SELECTABLE|TRICK_NOCOLL) trick.
                //     For this special case, the TRICK_NOT_SELECTABLE flag is handled in editCollNodeCallback.
                //     The explanation is this:
                //     1. You don't want it to have collision (because of TRICK_NOCOLL),
                //        so you can't turn on COLL_NOTSELECTABLE, but...
                //     2. You need it to be not selectable in the editor, so have that
                //        special callback (editCollNodeCallback) to check for that condition directly on the trick file.

                // Set something, clear COLL_NORMALTRI
                set_flags |= model_nocoll ? COLL_EDITONLY : COLL_NOTSELECTABLE;
                clear_flags &=~ model_nocoll ? COLL_EDITONLY : COLL_NOTSELECTABLE;
                clear_flags |= COLL_NORMALTRI;
                set_flags &=~ COLL_NORMALTRI;

#            endif
        }
    }
    modelSetCtriFlags(model,0,model->tri_count,set_flags,clear_flags);
}

#if CLIENT 
// Debug function used to look at individual triangles in a model and compare actual geometry with the collision geometry
void modelDisplayOneTri(Model * model, Mat4 node, int idx)
{
    static Vec3 *verts = 0;
    static int *tris = 0;
    int    i, *tri;
    Vec3 tverts[3];
    static int maxvcount = 0, maxtcount = 0;
    static int stupidcount = 0;

    stupidcount++;

    if( stupidcount > 60 )
        stupidcount = 0;

    if (model->vert_count > maxvcount)
    {
        SAFE_FREE(verts);
        maxvcount = pow2(model->vert_count);
        verts = malloc(maxvcount * sizeof(Vec3));
    }

    if (model->tri_count > maxtcount)
    {
        SAFE_FREE(tris);
        maxtcount = pow2(model->tri_count);
        tris = malloc(maxtcount * 3 * sizeof(int));
    }


    modelGetTris(tris, model);
    modelGetVerts(verts, model);

    while( idx > model->tri_count )
        idx -= model->tri_count;

    tri = &tris[idx * 3];

    for( i = 0; i < 3; i++)
        mulVecMat4(verts[tri[i]],node,tverts[i]);

    if( stupidcount < 30 )
        drawTriangle3D(tverts[0],tverts[1],tverts[2],0xffffffff);

    expandCtriVerts(&model->ctris[idx],tverts);
    for( i = 0; i < 3; i++)
        mulVecMat4(tverts[i],node,tverts[i]);

    if( stupidcount > 30 )
        drawTriangle3D(tverts[0],tverts[1],tverts[2],0xff00ffff);
}
#endif

int modelCreateCtris(Model *model)
{
    static Vec3 *verts = 0;
    static int *tris = 0;
    static int maxvcount = 0, maxtcount = 0;

    int        i,*tri;
    U32        uiGridAllocSize;
    U32        uiCTrisAllocSize;
    U32        uiTotalAllocSize;
    void    *pTotalAlloc;
    SharedHeapAcquireResult ret = SHAR_Error;
    char    cCtrisHandleName[512];
    bool    bAddExtra = false;
    bool    bFoundInvalidCTri = false;
    int        iCtriCount=0;


    if (!model || !model->tri_count || model->tags || !model->pack.grid.unpacksize)
        return 0;

    //if(    !stricmp(model->name, "GEO_Collision") &&
    //    !stricmp(model->filename, "player_library/G_Door_sewer2.geo"))
    //{
    //    int x = 10;
    //}

    model->pSharedHeapHandle = NULL;

    STR_COMBINE_BEGIN(cCtrisHandleName);
    STR_COMBINE_CAT("Models-Ctris-");
    STR_COMBINE_CAT(model->filename);
    STR_COMBINE_CAT("-");
    STR_COMBINE_CAT(model->name);
    STR_COMBINE_END();

    uiGridAllocSize = model->pack.grid.unpacksize;
    uiCTrisAllocSize = model->tri_count * sizeof(CTri);

    uiTotalAllocSize = uiGridAllocSize + uiCTrisAllocSize + sizeof(int)*2;


    // If there is a model->extra, add that to shared memory (for clients)
    for(i=0;i<model->tex_count;i++)
    {
        TexOptFlags texopt_flags=0;
        char    *texname = model->gld->texnames.strings[model->tex_idx[i].id];
        if (strstri(texname,"PORTAL"))
        {
            bAddExtra = true;
        }
    }

    if ( bAddExtra )
    {
        uiTotalAllocSize += sizeof(ModelExtra);
    }

    

#ifdef SERVER // shared memory for client -> problems (see texFind below)
    if( !db_state.map_supergroupid && !db_state.map_userid )
        ret = sharedHeapAcquire(&(model->pSharedHeapHandle), cCtrisHandleName);
#endif

    if ( ret == SHAR_FirstCaller )
    {
        bool bSuccess = sharedHeapAlloc(model->pSharedHeapHandle, uiTotalAllocSize);
        if ( !bSuccess )
        {
            pTotalAlloc = calloc( uiTotalAllocSize, 1 );
            model->pSharedHeapHandle = NULL;
            sharedHeapMemoryManagerUnlock();
            ret = SHAR_Error;
        }
        else
        {
            pTotalAlloc = model->pSharedHeapHandle->data;
            // clear the allocation
            memset(pTotalAlloc, 0, uiTotalAllocSize);
        }
    }
    else if ( ret == SHAR_Error )
    {
        pTotalAlloc = calloc( uiTotalAllocSize, 1 );

    }
    else // ret == SHAR_DataAcquired
    {
        assert( model->pSharedHeapHandle && model->pSharedHeapHandle->data && getHandleAllocatedSize(model->pSharedHeapHandle) == uiTotalAllocSize);
        pTotalAlloc = model->pSharedHeapHandle->data;
        model->grid.cell = pTotalAlloc; //malloc(model->pack.grid.unpacksize);
        model->ctris = (void*)((char*)pTotalAlloc + uiGridAllocSize);//calloc(sizeof(model->tags[0]) * model->tri_count,1);
        memcpy(&model->ctriflags_setonsome, (char*)pTotalAlloc + uiGridAllocSize + uiCTrisAllocSize, sizeof(int));
        memcpy(&model->ctriflags_setonall, (char*)pTotalAlloc + uiGridAllocSize + uiCTrisAllocSize + sizeof(int), sizeof(int));
        model->tags = calloc(sizeof(model->tags[0]) * model->tri_count,1);
        if ( bAddExtra )
            model->extra = (void*)((char*)pTotalAlloc + uiTotalAllocSize - sizeof(ModelExtra));

        {
            TrickInfo *trick = model->trick?model->trick->info:NULL;
            int set_flags, clear_flags;
            // GEO RELOAD TODO: clear Ctri flags if the model lost it's trick?
            if (trick && model)
            {
                
                if (trick->lod_near > 0 || trick->tnode.flags1 & TRICK_NOCOLL)
                    modelSetNoColl(model, trick->group_flags, &set_flags, &clear_flags);
            }
        }

        /*

        bSuccess = animCreateNovodexStream(model, true, NULL, 0, NULL, 0);

        if ( bSuccess )
        {
            return 1;
        }
        else
        {
            // unpack verts and tris, create the nxstream
            if (!model->gld->geo_data)
            {
                geoLoadData(model->gld);
            }

            assert(model->gld->geo_data);
            verts = _alloca(model->vert_count * sizeof(Vec3));
            tris = _alloca(model->tri_count * 3 * sizeof(int));

            modelGetTris(tris, model);
            modelGetVerts(verts, model);
            animCreateNovodexStream(model, false, verts, model->vert_count, tris, model->tri_count);

        }
        */


        return 1;
    }

    if ( bAddExtra )
        model->extra = (void*)((char*)pTotalAlloc + uiTotalAllocSize - sizeof(ModelExtra));

    PERFINFO_AUTO_START("modelCreateCtris", 1);



    assert(model->gld);
    

    if (!model->gld->geo_data)
    {
        geoLoadData(model->gld);
    }

    assert(model->gld->geo_data);


    ctri_total += model->pack.grid.unpacksize;
    model->grid.cell = pTotalAlloc; //malloc(model->pack.grid.unpacksize);
    geoUncompress((U8*)model->grid.cell,&model->pack.grid.unpacksize,model->pack.grid.data,model->pack.grid.packsize,model->name,model->filename);
    model->grid.cell = polyCellUnpack(model,model->grid.cell,(void*)model->grid.cell);

    if (model->vert_count > maxvcount)
    {
        SAFE_FREE(verts);
        maxvcount = pow2(model->vert_count);
        verts = malloc(maxvcount * sizeof(Vec3));
    }

    if (model->tri_count > maxtcount)
    {
        SAFE_FREE(tris);
        maxtcount = pow2(model->tri_count);
        tris = malloc(maxtcount * 3 * sizeof(int));
    }

    modelGetTris(tris, model);
    modelGetVerts(verts, model);
    
    model->tags = calloc(sizeof(model->tags[0]) * model->tri_count,1);
    model->ctris = (void*)((char*)pTotalAlloc + model->pack.grid.unpacksize);//calloc(sizeof(model->tags[0]) * model->tri_count,1);
    for(i=0;i<model->tri_count;i++)
    {
        
        tri = &tris[i * 3];
        model->ctris[i].flags = 0;
#ifndef GETVRML
        if (createCTri(&model->ctris[i],verts[tri[0]],verts[tri[1]],verts[tri[2]]) 
            && model->ctris[i].scale)
        {
            model->ctris[i].flags |= COLL_NORMALTRI;
            {
                Vec3 tverts[3];
                int        j;
                F32        d;

                expandCtriVerts(&model->ctris[i],tverts);
                for(j=0;j<3;j++)
                {
                    d = distance3(tverts[j],verts[tri[j]]);
                }
            }
        }
        else
            bFoundInvalidCTri = true;
#endif
    }
    if ( bFoundInvalidCTri )
        model->ctriflags_setonall = 0;
    else
        model->ctriflags_setonall = COLL_NORMALTRI;
    model->ctriflags_setonsome = COLL_NORMALTRI;


    modelSetTexOptCtriFlags(model, verts, tris);
    modelSetTrickCtriFlags(model);



    if ( ret == SHAR_FirstCaller )
    { 
        memcpy((char*)pTotalAlloc + uiGridAllocSize + uiCTrisAllocSize, &model->ctriflags_setonsome, sizeof(int));
        memcpy((char*)pTotalAlloc + uiGridAllocSize + uiCTrisAllocSize + sizeof(int), &model->ctriflags_setonall, sizeof(int));

        sharedHeapMemoryManagerUnlock();
    }

    // Now, given the verts and tris, create a novodex stream

    //animCreateNovodexStream(model, false, verts, model->vert_count, tris, model->tri_count);

    PERFINFO_AUTO_STOP();
    
    return 1;
}



static void modelSetTexOptCtriFlags(Model *model, Vec3 *verts, int *tris)
{
    int        i,base=0;
    Vec3    *normals = 0;
    char    **texnames;

    texnames = model->gld->texnames.strings;

    for(i=0;i<model->tex_count;i++)
    {
        TexOptFlags texopt_flags=0;
        char    *texname = texnames[model->tex_idx[i].id];

#ifdef CLIENT
        BasicTexture *surfTex = texFind(texname);
        if (!surfTex) {
            TexBind *bind = texFindComposite(texname);
            if (bind) {
                surfTex = bind->tex_layers[TEXLAYER_BASE];
            }
        }

        if (surfTex )
        {
            int        j;

            for(j=base;j<base + model->tex_idx[i].count;j++)
                model->ctris[j].surfTex = surfTex;
        }
#endif

        trickFromTextureName(texname, &texopt_flags);
        if (texopt_flags & TEXOPT_NOCOLL)
            modelSetCtriFlags(model,base,model->tex_idx[i].count,COLL_EDITONLY,COLL_NORMALTRI);

        //Slipperiness Flags
        if (texopt_flags & TEXOPT_SURFACESLICK)
            modelSetCtriFlags(model,base,model->tex_idx[i].count,COLL_SURFACESLICK,0);
        if (texopt_flags & TEXOPT_SURFACEICY)
            modelSetCtriFlags(model,base,model->tex_idx[i].count,COLL_SURFACEICY,0);
        if (texopt_flags & TEXOPT_SURFACEBOUNCY)
            modelSetCtriFlags(model,base,model->tex_idx[i].count,COLL_SURFACEBOUNCY,0);
        //End slipperiness

        if (strstri(texname,"ENTBLOCKER"))
            modelSetCtriFlags(model,0,model->tri_count,COLL_ENTBLOCKER,COLL_NORMALTRI);
        if (strstri(texname,"PORTAL"))
        {
            int *tri;
            int        j,k;
            F32        *vp;
            Vec3    dv;

            Portal * portal;
            ModelExtra * extra;
            Vec3    min = {8e16,8e16,8e16}, max = {-8e16,-8e16,-8e16};
            double magnitude;

            //Get the model extra and the next portal
            if (!model->extra)
                model->extra = calloc(sizeof(ModelExtra),1);

            extra = model->extra;
            portal= &extra->portals[extra->portal_count];
            memset( portal, 0, sizeof(Portal) );
            extra->portal_count++;
            assert( extra->portal_count <= ARRAY_SIZE(extra->portals) );

            for(j=base;j<base + model->tex_idx[i].count;j++)
            {
                tri = &tris[j * 3];
                for(k=0;k<3;k++)
                {
                    vp = verts[tri[k]];
                    MINVEC3(vp,min,min);
                    MAXVEC3(vp,max,max);
                }
            }
            {
                Vec3 v1,v2;
                subVec3(verts[tris[base*3+2]],verts[tris[base*3+1]],v1);
                subVec3(verts[tris[base*3+1]],verts[tris[base*3+0]],v2);
                crossVec3(v1,v2,portal->normal);
                magnitude=lengthVec3(portal->normal);
                scaleVec3(portal->normal,1.0/magnitude,portal->normal);
            }

            copyVec3(min, portal->min);
            copyVec3(max, portal->max);

            subVec3(max,min,dv);
            scaleVec3(dv,0.5f,dv);

            addVec3(dv, min, portal->pos);

            modelSetCtriFlags(model,base,model->tex_idx[i].count,COLL_PORTAL,COLL_NORMALTRI);

        }
        base += model->tex_idx[i].count;
    }
}

void modelSetCtriFlags(Model *model,int base,int count,U32 set_flag,U32 clear_flag)
{
    int        j;
    int        all;
    CTri *ctris=NULL;

    if (!set_flag && !model->ctris) {// We're only clearing flags and we haven't created the ctris yet
        return;
    }
    if (modelCreateCtris(model))
        assert(count == model->tri_count && base==0);

    ctris = &model->ctris[base];
    assert(count && ctris);

    // Optimization for re-setting existing flags
    set_flag &= ~model->ctriflags_setonall; // Remove flags that are already set on all tris
    clear_flag &= model->ctriflags_setonsome; // Only need to clear flags that are set on some tris
    if (!(set_flag || clear_flag)) {// Nothing to do
        return;
    }
    all = ctris == model->ctris && count == model->tri_count;
    if (all) {
        model->ctriflags_setonsome &= ~clear_flag;
        model->ctriflags_setonall |= set_flag;
    }
    model->ctriflags_setonsome |= set_flag;
    model->ctriflags_setonall &= ~clear_flag;

    for(j=0;j<count;j++)
    {
        if (ctris[j].flags)
        {
            ctris[j].flags &= ~clear_flag;
            ctris[j].flags |= set_flag;
        }
    }
}

#endif


/*Has this geo already been loaded, if yes, return it...
*/
static GeoLoadData *geoFindPreload(char *name)
{
    if (glds_ht)
        return stashFindPointerReturnPointer(glds_ht, name);
    return NULL;
}


static void testGeoUnpack(Model *model)
{
    Vec3    *norms,*verts;
    Vec2    *sts;
    int        *tris;
    U8        *weights,*matidxs;

    verts = malloc(model->vert_count * sizeof(verts[0]));
    tris = malloc(model->tri_count * sizeof(int)*3);
    norms = malloc(model->vert_count * sizeof(norms[0]));
    sts = malloc(model->vert_count * sizeof(sts[0]));
    weights = malloc(model->vert_count);
    matidxs = malloc(model->vert_count * 2);

    modelGetTris(tris, model);
    modelGetVerts(verts, model);
    modelGetNorms(norms, model);
    modelGetSts(sts, model);
    modelGetWeights(weights, model);
    modelGetMatidxs(matidxs, model);

    {
        int i;
        printf("Verts:\n");
        for (i=0; i<model->vert_count; i++) {
            printf(" %3d: (%f,%f,%f) N:(%f,%f,%f) ST:(%f,%f)  W:%d MI:(%d,%d)\n", i, verts[i][0], verts[i][1], verts[i][2],
                norms[i][0], norms[i][1], norms[i][2], sts[i][0], sts[i][1], weights[i], matidxs[i*2], matidxs[i*2+1]);
        }
        printf("Tris:\n");
        for (i=0; i<model->tri_count; i++) {
            printf(" %3d: (%d,%d,%d)\n", i, tris[i*3+0], tris[i*3+1], tris[i*3+2]);
        }
    }

    modelCreateCtris(model);
    free(tris);
    free(verts);
    free(norms);
    free(sts);
    free(weights);
    free(matidxs);
}

static void patchPackPtr(PackData *pack,int offset)
{
    if (pack->unpacksize)
    {
        pack->data += offset;
    }
}

static void unpatchPackPtr(PackData *pack,int offset)
{

    if (pack->unpacksize)
    {
        pack->data -= offset;
    }
}


static void restoreStubOffsets(GeoLoadData* gld)
{
    U32 base_offset = (U32)(uintptr_t)(gld->geo_data);
    int j;

    for( j=0 ; j < gld->modelheader.model_count ; j++ )
    {
        Model * model = gld->modelheader.models[j];
        unpatchPackPtr(&model->pack.tris,base_offset);
        unpatchPackPtr(&model->pack.verts,base_offset);
        unpatchPackPtr(&model->pack.norms,base_offset);
        unpatchPackPtr(&model->pack.sts,base_offset);
        if (gld->file_format_version >= 3)
            unpatchPackPtr(&model->pack.sts3,base_offset);
        unpatchPackPtr(&model->pack.grid,base_offset);
        unpatchPackPtr(&model->pack.weights,base_offset);
        unpatchPackPtr(&model->pack.matidxs,base_offset);
        if (gld->file_format_version >= 7)
            unpatchPackPtr(&model->pack.reductions,base_offset);
        if (gld->file_format_version >= 8)
            unpatchPackPtr(&model->pack.reflection_quads,base_offset);
        if (model->api)
            model->api            = (void *)((uintptr_t)model->api - base_offset);
        if(model->boneinfo)
            model->boneinfo        = (void *)((uintptr_t)model->boneinfo - base_offset);
    }
}


void geoLoadData(GeoLoadData* gld)
{
    U8 * mem;
    int j;
    U32 base_offset;

    if(!gld->file)
        gld->file = fileOpen(gld->name, "rb"); //if failure, I need to do something
    if(!gld->file)
        FatalErrorf("Could not open GeoLoadData file %s", gld->name);

    //////////////////////////////////////////////////////////////////////////
    
    fseek(gld->file,0,SEEK_SET);
    fseek(gld->file, gld->data_offset, SEEK_SET);

    gld->geo_data = mem = malloc(gld->datasize);
    fread(mem, 1, gld->datasize, gld->file);
    assert((U32)gld->headersize < (uintptr_t)mem);
    base_offset = (U32)(uintptr_t)mem;

    for( j=0 ; j < gld->modelheader.model_count ; j++ )
    {
        Model *model = gld->modelheader.models[j];
        patchPackPtr(&model->pack.tris,base_offset);
        patchPackPtr(&model->pack.verts,base_offset);
        patchPackPtr(&model->pack.norms,base_offset);
        patchPackPtr(&model->pack.sts,base_offset);
        if (gld->file_format_version >= 3)
            patchPackPtr(&model->pack.sts3,base_offset);
        patchPackPtr(&model->pack.grid,base_offset);
        patchPackPtr(&model->pack.weights,base_offset);
        patchPackPtr(&model->pack.matidxs,base_offset);
        if (gld->file_format_version >= 7)
            patchPackPtr(&model->pack.reductions,base_offset);
        if (gld->file_format_version >= 8)
            patchPackPtr(&model->pack.reflection_quads,base_offset);

        if (model->api)
            model->api            = (void *)(base_offset + (uintptr_t)model->api);
        if(model->boneinfo)
        {
            model->boneinfo    = (void *) (base_offset + (uintptr_t)model->boneinfo);
            model->boneinfo->weights    = 0;
            model->boneinfo->matidxs    = 0;
        }
        //testGeoUnpack(model);
    }

    //////////////////////////////////////////////////////////////////////////
    
    fclose(gld->file);
    gld->file = 0;
}

typedef struct
{
    PackData        tris;
    PackData        verts;
    PackData        norms;
    PackData        sts;
    PackData        weights;
    PackData        matidxs;
    PackData        grid;
} PackBlockOnDisk;

typedef struct ModelFormatOnDisk_v2 // Do not change this structure
{
    // frequently used data keep in same cache block
    U32                flags;
    F32                radius;
    VBO                *vbo;
    int                tex_count;        //number of tex_idxs (sum of all tex_idx->counts == tri_count)
    S16                id;            //I am this bone 
    U8                blend_mode;
    U8                loadstate; 
    BoneInfo        *boneinfo; //if I am skinned, everything about that
    TrickNode        *trick;
    int                vert_count;
    int                tri_count;
    TexID            *tex_idx;        //array of (textures + number of tris that have it)

    // collision
    PolyGrid        grid;
    CTri            *ctris;
    int                *tags;

    // Less frequently used data
    char            *name;
    AltPivotInfo    *api;    // if I have alternate pivot points defined for fx, everything about that
    ModelExtra        *extra;    // Portals
    Vec3            scale;    // hardly used at all, but dont remove, jeremy scaling files
    Vec3            min,max;
    GeoLoadData        *gld;

    PackBlockOnDisk    pack;
} ModelFormatOnDisk_v2;


static int readModel(Model *dst, char *data, int version_num, U8 *objname_base, U8 *texidx_base)
{

    int size = 0;

    if (version_num < 3)
    {
        ModelFormatOnDisk_v2 *src = (void *)data;
        #define COPY(fld) dst->fld = src->fld
            COPY(flags);
            COPY(radius);
            COPY(tex_count);
            COPY(id);
            COPY(loadstate);
            COPY(boneinfo);
            assert(!src->trick);
            COPY(vert_count);
            COPY(tri_count);
            dst->tex_idx = (TexID*)(texidx_base + (uintptr_t)src->tex_idx);

            COPY(grid);
            assert(!src->ctris);
            assert(!src->tags);

            dst->name = objname_base + (uintptr_t)src->name;
            COPY(api);
            assert(!src->extra);    // Portals
            copyVec3(src->scale, dst->scale);
            copyVec3(src->min, dst->min);
            copyVec3(src->max, dst->max);

            COPY(pack.tris);
            COPY(pack.verts);
            COPY(pack.norms);
            COPY(pack.sts);
            memset(&dst->pack.sts3, 0, sizeof(dst->pack.sts3));
            COPY(pack.weights);
            COPY(pack.matidxs);
            COPY(pack.grid);
        #undef COPY

        size = sizeof(*src);
    }
    else
    {
        #define COPY(fld,bytes) { memcpy(&(dst->fld), data, bytes); data += bytes; }
        #define COPY2(fld,bytes) { memcpy((dst->fld), data, bytes); data += bytes; }

            memcpy(&size, data, 4); data += 4;
            
            COPY(radius, 4);
            COPY(tex_count, 4);
            COPY(boneinfo, 4);
            COPY(vert_count, 4);
            COPY(tri_count, 4);
            if (version_num >= 8)
            {
                COPY(reflection_quad_count, 4);
            }
            COPY(tex_idx, 4);
            dst->tex_idx = (TexID*)(texidx_base + (intptr_t)dst->tex_idx);
            COPY(grid, sizeof(PolyGrid));
            COPY(name, 4);
            dst->name = objname_base + (intptr_t)dst->name;
            COPY(api, 4);
            COPY2(scale, sizeof(Vec3));
            COPY2(min, sizeof(Vec3));
            COPY2(max, sizeof(Vec3));
            COPY(pack.tris, sizeof(PackData));
            COPY(pack.verts, sizeof(PackData));
            COPY(pack.norms, sizeof(PackData));
            COPY(pack.sts, sizeof(PackData));
            COPY(pack.sts3, sizeof(PackData));
            COPY(pack.weights, sizeof(PackData));
            COPY(pack.matidxs, sizeof(PackData));
            COPY(pack.grid, sizeof(PackData));
            if (version_num == 4)
            {
                data += sizeof(PackData); // used to be pack.lmap_utransforms
                data += sizeof(PackData); // used to be pack.lmap_vtransforms
            }
            if (version_num >= 7)
            {
                COPY(pack.reductions, sizeof(PackData));
            }
            if (version_num >= 8)
            {
                COPY(pack.reflection_quads, sizeof(PackData));
            }
#if 0
            if (version_num < 8)
            {
                //Skip unused lightmap_size
                data += sizeof(F32);
            }
#endif
            if (version_num >= 7)
            {
#if CLIENT
                COPY(autolod_dists, sizeof(F32)*3);
#else
                data += sizeof(F32)*3;
#endif
            }
#if CLIENT
            else
            {
                setVec3(dst->autolod_dists, -1, -1, -1);
            }
#endif
            COPY(id, 2);

        #undef COPY
        #undef COPY2
    }

    assert(!dst->grid.cell);
    return size;

}

/*called when setting up the modelect only.  Gets the right TrickInfo,
then does the trick of copying the TrickInfo's TrickNode into the given TrickNode
before assigning the TrickInfo to that TrickInfo.  This is just a shortcut to initialize
the TrickNode*/
void modelInitTrickNodeFromName(char *name,Model *model)
{
    TrickInfo    *trickinfo;

    trickinfo = trickFromObjectName(name,model->gld->name);
    if (trickinfo)
    {
        if (!model->trick)
            model->trick = malloc(sizeof(TrickNode));
        *model->trick = trickinfo->tnode;
        model->trick->info = trickinfo;
    } else if (model->trick) {
        memset(model->trick, 0, sizeof(*model->trick));
        setVec3(model->trick->trick_rgba, 255, 255, 255);
        setVec3(model->trick->trick_rgba2, 255, 255, 255);
    }
}

// Allocate one chunk of memory, and return an EArray of pointers (so that when we reload data we can add to this list)
static Model **allocModelList(int count)
{
    Model **ret=0;
    Model *rawData;
    int i;

    if (!count)
        return NULL;
    eaCreate(&ret);
    rawData = calloc(sizeof(Model)*count, 1);
    for (i=0; i<count; i++) 
        eaPush(&ret, &rawData[i]);
    return ret;
}

static int __cdecl compareModelNames(const Model** m1, const Model** m2)
{
    return compareModelNamesAndLengths((*m1)->name, (*m1)->namelen_notrick, (*m2)->name, (*m2)->namelen_notrick);
}

static int __cdecl compareModelNames(const Model** m1, const Model** m2);
static int compareModelNamesCallback(const void* arg0, const void* arg1)
{
    return compareModelNames((const Model**)arg0, (const Model**)arg1);
}

void geoSortModels(GeoLoadData* gld)
{
    PERFINFO_AUTO_START("qsort", 1);
    
        qsort(    gld->modelheader.models,
                gld->modelheader.model_count,
                sizeof(gld->modelheader.models[0]),
                compareModelNamesCallback);
                
    PERFINFO_AUTO_STOP_START("verify", 1);

        if(0){
            int i;
            char dupeNames[100000] = "";
            char* pos = dupeNames;
            for(i = 0; i < gld->modelheader.model_count - 1; i++)
            {
                int j;
                for(j = i + 1; j < gld->modelheader.model_count; j++)
                {
                    Model* m1 = gld->modelheader.models[i];
                    Model* m2 = gld->modelheader.models[j];
                    ModelSearchData search;
                    
                    if(0)
                    {
                        if(!compareModelNames(&m1, &m2))
                        {
                            //ErrorFilenamef(    file->name,
                            //                "Duplicate geo name:\nGeo1: %s\nGeo2: %s\n",
                            //                m1->name,
                            //                m2->name);
                            assert(pos - dupeNames < ARRAY_SIZE(dupeNames) - 1000);
                            pos += sprintf(pos, "    %s", m1->name);
                            if(stricmp(m1->name, m2->name))
                            {
                                pos += sprintf(pos, ", %s <--------- ***** different trick file!!! *****", m2->name);
                            }
                            pos += sprintf(pos, "\n");
                        }
                    }

                    if(0)
                    {
                        // Verify that the models are ordered properly.
                        
                        assert(compareModelNames(&m1, &m2) <= 0);
                        assert(compareModelNames(&m2, &m1) >= 0);
                        
                        search.name = m1->name;
                        search.namelen = m1->namelen_notrick;
                        
                        assert(compareModelBSearch(&search, &m2) <= 0);

                        search.name = m2->name;
                        search.namelen = m2->namelen_notrick;
                        
                        assert(compareModelBSearch(&search, &m1) >= 0);
                    }
                }
            }
            
            if(dupeNames[0])
            {
                ErrorFilenamef(gld->name, "Duplicate geo names.\n%s", dupeNames);
            }
        }

    PERFINFO_AUTO_STOP();
}

typedef struct AutoLODOnDisk_v5
{
    float        max_error;
    F32            lod_near, lod_far, lod_nearfade, lod_farfade;
    int            flags;
} AutoLODOnDisk_v5;

typedef struct ModelLODInfoOnDisk_v5
{
    int num_lods;
    AutoLODOnDisk_v5 lods[6];
} ModelLODInfoOnDisk_v5;

static ModelLODInfo *unpackLODInfo(char *data, int memsize, int model_count, int version_num)
{
    ModelLODInfo *lod_infos = calloc(sizeof(*lod_infos), model_count);
    int i, j, bytes_read = 0;

    if (version_num >= 6)
    {
        #define COPY(dst,bytes) { memcpy(&dst, data + bytes_read, bytes); bytes_read += bytes; }
        for (j = 0; j < model_count; j++)
        {
            ModelLODInfo *lod_info = &lod_infos[j];
            int num_lods;
            COPY(num_lods, sizeof(int));
            for (i = 0; i < num_lods; i++)
            {
                AutoLOD *lod = allocAutoLOD();
                eaPush(&lod_info->lods, lod);

                COPY(lod->flags, sizeof(int));
                COPY(lod->max_error, sizeof(float));
                COPY(lod->lod_far, sizeof(float));
                COPY(lod->lod_farfade, sizeof(float));
                COPY(lod->lod_near, sizeof(float));
                COPY(lod->lod_nearfade, sizeof(float));

                lod->lod_modelname = ParserAllocString(data + bytes_read);
                bytes_read += (int)strlen(lod->lod_modelname) + 1;
                if (!lod->lod_modelname[0])
                {
                    ParserFreeString(lod->lod_modelname);
                    lod->lod_modelname = 0;
                }

                lod->lod_filename = ParserAllocString(data + bytes_read);
                bytes_read += (int)strlen(lod->lod_filename) + 1;
                if (!lod->lod_filename[0])
                {
                    ParserFreeString(lod->lod_filename);
                    lod->lod_filename = 0;
                }
            }
        }
        #undef COPY
    }
    else if (version_num >= 2 && version_num <= 5)
    {
        ModelLODInfoOnDisk_v5 *lod_data = (void *)data;
        for (j = 0; j < model_count; j++)
        {
            ModelLODInfo *lod_info = &lod_infos[j];
            bytes_read += sizeof(*lod_data);
            for (i = 0; i < lod_data[j].num_lods; i++)
            {
                AutoLOD *lod = allocAutoLOD();
                eaPush(&lod_info->lods, lod);

                lod->flags = lod_data[j].lods[i].flags;
                lod->lod_far = lod_data[j].lods[i].lod_far;
                lod->lod_farfade = lod_data[j].lods[i].lod_farfade;
                lod->lod_near = lod_data[j].lods[i].lod_near;
                lod->lod_nearfade = lod_data[j].lods[i].lod_nearfade;
                lod->max_error = lod_data[j].lods[i].max_error;
            }
        }
    }
    else
    {
        assert(0);
    }

    assert(bytes_read <= memsize);

    return lod_infos;
}

#define MAX_VERSION 8
static GeoLoadData *geoLoadStubs(FILE * file, GeoLoadData * gld,GeoUseType type)
{
    int        i=0,j,ziplen;
    U8        *mem,*zipmem,*texidx_offset;
    char    *aps;
    Model    *model;
    int        texname_blocksize,objname_blocksize,texidx_blocksize,lodinfo_blocksize=0;
    char    *objnames;
    int        version_num = 0;
    int        mem_pos, oziplen;
    int        totalSize;
    void    *lod_data = 0;

    PERFINFO_AUTO_START("top", 1);
    
        //read the size of the header, then read the header into memory
        if(!fread(&ziplen,1,4,file))
        {
            ErrorFilenamef(gld->name, "Bad file size.");
            PERFINFO_AUTO_STOP();
            return NULL;
        }
        oziplen = ziplen;

        if(!fread(&gld->headersize,1,4,file))
        {
            ErrorFilenamef(gld->name, "Bad file size.");
            PERFINFO_AUTO_STOP();
            return NULL;
        }
        if (gld->headersize == 0)
        {
            // new format
            fread(&version_num,4,1,file);
            if (version_num >= 2 && version_num <= MAX_VERSION && version_num != 6)
            {
                ziplen -= 0; // was biased to include sizeof(new header) so pigg system would cache it
            }
            else
            {
                ErrorFilenamef(gld->name, "Bad version: %d", version_num);
                PERFINFO_AUTO_STOP();
                return NULL;
            }

            ziplen -= 12; // was biased to include 2 * sizeof(gld->headersize) + sizeof(version_num) so pigg system would cache it
            fread(&gld->headersize,1,4,file);
        }
        else
        {
            // old format
            ziplen -= 4; // was biased to include sizeof(gld->headersize) so pigg system would cache it
            oziplen += 4; // because of an old bug in getVRML
        }

    PERFINFO_AUTO_STOP_START("middle", 1);

        gld->file_format_version = version_num;

        gld->header_data = mem = malloc(gld->headersize);
        assert(mem);
        zipmem = _alloca(ziplen);
        assert(zipmem);
        fread(zipmem,1,ziplen,file);
        geoUncompress(mem,&gld->headersize,zipmem,ziplen,"",gld->name);

        gld->data_offset = oziplen + 4;

        #define MEM_NEXT(add)    (mem_pos + (int)add <= gld->headersize ? mem_pos += add, &mem[mem_pos - add] : (devassert(0), NULL))
        #define MEM_INT()        (*(int*)MEM_NEXT(sizeof(int)));

        //2 ####### Unpack the animlist
        mem_pos = 0;

        gld->datasize            = MEM_INT();

        texname_blocksize        = MEM_INT();

        objname_blocksize        = MEM_INT();

        texidx_blocksize        = MEM_INT();

        if (version_num >= 2 && version_num <= 6)
        {
            lodinfo_blocksize    = MEM_INT();
        }

        totalSize =    mem_pos +    
                    texname_blocksize +
                    objname_blocksize +
                    texidx_blocksize  +
                    lodinfo_blocksize;

        totalSize += sizeof(ModelHeader);
        
        mem = malloc(totalSize);
        
        memcpy(mem, gld->header_data, totalSize);

        unpackNames(MEM_NEXT(texname_blocksize), &gld->texnames);

        if (gld->texnames.count == 0)
        {
            assert( texname_blocksize >=8 && gld->texnames.strings);
            gld->texnames.count = 1;
            gld->texnames.strings[0] = "white";
        }

        objnames = (void *)MEM_NEXT(objname_blocksize);

        texidx_offset = MEM_NEXT(texidx_blocksize);

        if (version_num >= 2 && version_num <= 6)
        {
            lod_data = (void *)MEM_NEXT(lodinfo_blocksize);
        }

        MEM_NEXT(sizeof(ModelHeader));
        
        assert(mem_pos == totalSize);
        
        aps = (void*)((U8*)gld->header_data + mem_pos);

        memcpy(&gld->modelheader, (void*)((U8*)mem + mem_pos - sizeof(gld->modelheader)), sizeof(gld->modelheader));
        gld->modelheader.models = allocModelList(gld->modelheader.model_count);
        gld->modelheader.model_data = gld->modelheader.models?gld->modelheader.models[0]:NULL;

#if CLIENT | GETVRML
        gld->lod_infos = 0;
        if (lod_data)
            gld->lod_infos = unpackLODInfo(lod_data, lodinfo_blocksize, gld->modelheader.model_count, version_num);
#endif

    PERFINFO_AUTO_STOP_START("middle2", 1);

        for( j=0 ; j < gld->modelheader.model_count ; j++ )
        {
            char* trick;
            PERFINFO_AUTO_START("initModel", 1);
                model = gld->modelheader.models[j]; 
                aps += readModel(model, aps, version_num, (U8*)objnames, (U8*)texidx_offset);

                model->gld = gld;
                model->namelen = (int)strlen(model->name);
                trick = strstr(model->name, "__");
                model->namelen_notrick = trick ? trick - model->name : model->namelen;
                model->filename = gld->name;
                model->grid_size_orig = model->grid.size;
                assert(!(model->flags&~1)); // Sometimes this had OBJ_ALPHASORT, but the current GetVrml cannot make one of these anymore

                if( !(type & GEO_GETVRML_FASTLOAD)) // for fast loading when getvrml is just loading .geo to print stats
                {
                    PERFINFO_AUTO_START("modelInitTrickNodeFromName", 1);
                        modelInitTrickNodeFromName(model->name,model);     
                    PERFINFO_AUTO_STOP();
                }

#if CLIENT | GETVRML
                if (gld->lod_infos && eaSize(&gld->lod_infos[j].lods) && getAutoLodNum(model->name) == 0 && !(gld->lod_infos[j].lods[0]->flags & LOD_LEGACY))
                {
                    model->lod_info = &gld->lod_infos[j];
                }
#endif
#if CLIENT
                else if (model->pack.reductions.unpacksize)
                {
                    // THIS SHOULD NOT HAPPEN IN GETVRML UNDER ANY CIRCUMSTANCES
                    model->lod_info = lodinfoFromObjectName(0, model->name, model->filename, model->radius, 0, model->tri_count, model->autolod_dists);
                }

                if (model->lod_info && !model->pack.reductions.unpacksize)
                {
                    for (i = 0; i < eaSize(&model->lod_info->lods); i++)
                    {
                        AutoLOD *lod = model->lod_info->lods[i];
                        lod->modelname_specified = !!lod->lod_modelname;

                        if (!lod->lod_modelname)
                        {
                            lod->lod_modelname = getAutoLodModelName(model->name, i);
                            if (lod->lod_modelname)
                                lod->lod_modelname = ParserAllocString(lod->lod_modelname);
                            lod->lod_filename = ParserAllocString(model->filename);
                        }
                        if (!lod->lod_filename && lod->lod_modelname)
                        {
                            lod->lod_filename = objectLibraryPathFromObj(lod->lod_modelname);
                            if (lod->lod_filename)
                                lod->lod_filename = ParserAllocString(lod->lod_filename);
                        }
                    }
                }
#endif

            PERFINFO_AUTO_STOP();
        }

        // Discard ModelFormatOnDisk data as it's been copied into newly alloced structures

        free(gld->header_data);
        gld->header_data = mem;

        gld->type |= type;

        #ifdef CLIENT
            if( (type & GEO_INIT_FOR_DRAWING) && gld->modelheader.model_count )
                addModelData(gld);
        #endif

        if(!(type & GEO_GETVRML_FASTLOAD)) {
            // Sort models by name for quick lookup.
            PERFINFO_AUTO_STOP_START("geoSortModels", 1);
            geoSortModels(gld);
            PERFINFO_AUTO_STOP();
        }

    return gld;
}

// void backgroundLoaderSetThreadPriority(U32 priority)
// {
//     if(background_loader_handle)
//         SetThreadPriority(background_loader_handle, priority);
// }

/*Geo Loader thread: All I do is sleep, waiting to be given work by QueueUserAPC*/
static unsigned __stdcall backgroundLoadingThread( LPVOID lpParam )
{
    EXCEPTION_HANDLER_BEGIN
        PERFINFO_AUTO_START("backgroundLoadingThread", 1);
            //if(fileIsUsingDevData())
            //{
            //    SetThreadPriority(GetCurrentThread(),THREAD_PRIORITY_BELOW_NORMAL - 1);
            //}
            for(;;)
            {
                SleepEx(INFINITE, TRUE);
            }
        PERFINFO_AUTO_STOP();
        return 0; 
    EXCEPTION_HANDLER_END
    return 0;
} 

void initBackgroundLoader()
{
    
    if(!initedBackgroundLoader)
    {
        initedBackgroundLoader = 1;

        InitializeCriticalSection(&CriticalSectionTexLoadQueues);
        InitializeCriticalSection(&CriticalSectionTexLoadData);
        InitializeCriticalSection(&CriticalSectionGeoLoadQueues);
        InitializeCriticalSection(&heyThreadLoadAGeo_CriticalSection);
        InitializeCriticalSection(&heyThreadLODAModel_CriticalSection);
        InitializeCriticalSection(&CriticalSectionGeoUncompress);

#if CLIENT
        modelCacheInitUnpackCS();
#endif

        if(background_loader_handle == NULL )
        {
            DWORD dwThrdParam = 1; 

            background_loader_handle = (HANDLE)_beginthreadex( 
                NULL,                        // no security attributes 
                0,                           // use default stack size  
                backgroundLoadingThread,             // thread function 
                &dwThrdParam,                // argument to thread function 
                0,                           // use default creation flags 
                &background_loader_threadID); // returns the thread identifier 

            //_ASSERTE(heapValidateAll());
            assert(background_loader_handle != NULL);
        }
    }
}

/*Callback that the background anim loader is given to do*/
static VOID CALLBACK heyThreadLoadAGeo(ULONG_PTR dwParam)
{
    GeoLoadData * gld;

    PERFINFO_AUTO_START("heyThreadLoadAGeo", 1);
    EnterCriticalSection(&heyThreadLoadAGeo_CriticalSection);
        gld = (GeoLoadData *)dwParam;

        geoLoadData(gld);

        EnterCriticalSection(&CriticalSectionGeoLoadQueues); 
        listAddForeignMember(&glds_ready_for_final_processing, gld);
        LeaveCriticalSection(&CriticalSectionGeoLoadQueues);
        LeaveCriticalSection(&heyThreadLoadAGeo_CriticalSection);
    PERFINFO_AUTO_STOP();
};

static void geoRequestForegroundLoad( GeoLoadData * gld, FILE * file )
{
    gld->file = file; //this might be hacky, perhaps make a package of list, file and next
#ifdef CLIENT
    gld->tex_load_style = TEX_LOAD_DONT_ACTUALLY_LOAD; //TEX_LOAD_NOW_CALLED_FROM_MAIN_THREAD;
#endif

    heyThreadLoadAGeo( (ULONG_PTR)gld );
    number_of_glds_in_background_loader++;
}

static void geoRequestBackgroundLoad( GeoLoadData * gld, FILE * file )
{
    gld->file = file; //this might be hacky, perhaps make a package of list, file and next
#ifdef CLIENT

    gld->tex_load_style = TEX_LOAD_DONT_ACTUALLY_LOAD; //TEX_LOAD_NOW_CALLED_FROM_LOAD_THREAD;
#endif

    QueueUserAPC(heyThreadLoadAGeo, background_loader_handle, (ULONG_PTR)gld );
    number_of_glds_in_background_loader++;
}

static void geoSetLoadState(GeoLoadData *gld,int loadstate)
{
    int i;

    gld->loadstate = loadstate;
    for(i=0;i<gld->modelheader.model_count;i++)
        gld->modelheader.models[i]->loadstate = loadstate;
}

void geoCheckThreadLoader()
{
    GeoLoadData * gld;
    GeoLoadData * next_gld;  

    if (!glds_ready_for_final_processing)
        return;
    EnterCriticalSection(&CriticalSectionGeoLoadQueues); 
    for(gld = glds_ready_for_final_processing; gld ; gld = next_gld)
    {
        next_gld = gld->next;
        listRemoveMember(gld, &glds_ready_for_final_processing);
        
        geoSetLoadState(gld,LOADED);
        gld_num_bytes_loaded += gld->datasize;
    
        number_of_glds_in_background_loader--;
    }
    LeaveCriticalSection(&CriticalSectionGeoLoadQueues);
#if CLIENT
    if (gld_num_bytes_loaded)
        loadUpdate("bg_anims",gld_num_bytes_loaded);
    gld_num_bytes_loaded = 0;
#endif
}

void forceGeoLoaderToComplete()
{
    geoCheckThreadLoader();
    while(number_of_glds_in_background_loader || number_of_lods_in_background_loader)
    {
        Sleep(1);
        geoCheckThreadLoader();
    }
    geoCheckThreadLoader();
}

static void waitForGeoToFinishLoading(GeoLoadData * gld)
{
    PERFINFO_AUTO_START("waitForGeoToFinishLoading", 1);
        while(1)
        {
            PERFINFO_AUTO_START("geoCheckThreadLoader", 1);
                geoCheckThreadLoader();
            PERFINFO_AUTO_STOP();

            if(gld->loadstate == LOADED)
            {
                break;
            }
            
            Sleep(1);
        }
    PERFINFO_AUTO_STOP();
}


static StashTable ht_nonexistent = 0;
void geoLoadResetNonExistent(void)
{
    if(ht_nonexistent)
        stashTableClear(ht_nonexistent);
}

static int anim_report_exist_errors = 1;
void geoSetExistenceErrorReporting(int report_errors)
{
    anim_report_exist_errors = report_errors;
}

//load_type:
//LOAD_BACKGROUND = get headers and aps, then load data in background
//LOAD_NOW      = get headers and aps, then load data immediately in this thread
//LOAD_HEADER = get headers and aps, and dont load data

//use_type
//DONT_GEO_INIT_FOR_DRAWING        = just get the models
//GEO_INIT_FOR_DRAWING    = also do the model convert to make objectinfos

GeoLoadData * geoLoad(const char * name_old, GeoLoadType load_type, GeoUseType use_type)
{
    GeoLoadData    *gld = 0;
    FILE        *file = 0;
    char        *s,name[1000];
    
    PERFINFO_AUTO_START("geoLoad", 1);

    assert(use_type & GEO_USE_MASK);

    strcpy(name,name_old);
    s = strrchr(name,'.');
    if (stricmp(s,".anm")==0)
        strcpy(s,".geo");
    
    if(load_type & LOAD_FASTLOAD)
        load_type |= LOAD_HEADER;

#if DISABLE_PARALLEL_GEO_THREAD_LOADER
    if(load_type == LOAD_BACKGROUND)
        load_type = LOAD_NOW;
#endif

#ifdef CLIENT
    if( game_state.disableGeometryBackgroundLoader && load_type == LOAD_BACKGROUND)
        load_type = LOAD_NOW;
#endif

    //load_type = LOAD_NOW;
 
    /*if you've looked for this model before, and you know it doesn't exist, stop trying, or if it's junk*/
    if( !name || !name[0] )
        printf( "ANIM: anim Load called with no anim name\n"  );

    if ( ht_nonexistent && stashFindElement(ht_nonexistent, name, NULL))
    {
        PERFINFO_AUTO_STOP();
        return 0;
    }
    
    /*get what has already been loaded: none,headers,loading,or all*/
    if (load_type & LOAD_RELOAD) {
        // Force a load, the calling function will deal with freeing the old stuff
        gld = NULL;
    } else {
        gld = geoFindPreload(name);
    }
    if (gld)
    {
        if(gld->loadstate == LOADED)
        {
            PERFINFO_AUTO_STOP();
            return gld;
        }
        else if(gld->loadstate == NOT_LOADED)
        {
            if(load_type & LOAD_HEADER)
            {
                PERFINFO_AUTO_STOP();
                return gld;
            }
        }
        else if(gld->loadstate == LOADING)
        {
            if(load_type & (LOAD_NOW|LOAD_RELOAD)) 
            {
                waitForGeoToFinishLoading(gld);
            }
            PERFINFO_AUTO_STOP();
            return gld;
        }    
    }
    

    /*now either we have no gld, or an gld with no data, so take care of first case and get gld*/
    if(!gld)
    {
        PERFINFO_AUTO_START("loadAnimList", 1);
            //printf("loading gld: %s\n", name);
            file = fileOpen(name, "rb");
                
            /*if file doesn't exist, add it to the list of nonexistent file names (necessary because 
            player code often says "I don't know if this object exists, but if it does, I want to use it")*/
            if(!file)
            {
                if(anim_report_exist_errors)
                {
                    char filename[256];
                    PERFINFO_AUTO_START("addToNonExistent", 1);
                        changeFileExt(name,".rootnames",filename);
                        if (fileExists(filename)) {
                            printf( "GEO: Missing .geo file %s (but rootnames exists)\n", name );
                        } else {
                            // this is a normal case
                            //printf( "GEO: Missing .geo file %s\n", name );
                        }
                        if(!ht_nonexistent)
                            ht_nonexistent = stashTableCreateWithStringKeys(5000,StashDeepCopyKeys);
                        stashAddPointer(ht_nonexistent, name, 0 , false);
                    PERFINFO_AUTO_STOP();
                }
            }
            /*otherwise read the anim_headers/models but not the data into an animlist*/
            else
            {
                PERFINFO_AUTO_START("geoLoadStubs", 1);
                    //OPT gld = &anim_lists[anim_count++];
                    //OPT assert(anim_count < MAX_GEOS);
                    if (glds_ht==0 && !(load_type & LOAD_FASTLOAD) ) {
                        glds_ht = stashTableCreateWithStringKeys(MAX_GEOS, StashDeepCopyKeys);
                    }
                    gld = calloc(sizeof(GeoLoadData), 1);

                    assert( strlen(name) < ARRAY_SIZE(gld->name) - 1 );
                    strcpy(gld->name, name);

                    if(!geoLoadStubs(file, gld, use_type))
                    {
                        SAFE_FREE(gld);
                        fclose(file);
                        file = NULL;
                    }
                    else if(glds_ht)
                    {
                        gld->geo_use_type = use_type & GEO_USE_MASK;
                        gld->loadstate = NOT_LOADED;
                        gld->type = use_type;
                        fseek( file, 0L, SEEK_SET );
                        stashAddPointer(glds_ht, gld->name, gld, false);
                    }
                PERFINFO_AUTO_STOP();
            }
        PERFINFO_AUTO_STOP();
    }
    
    /*Now if we have animlist, decide whether we need the background loader to ever load the data*/
    if(gld)
    {
        if(load_type & LOAD_HEADER)
        {
            if(file)
                fclose( file );
        }
        else if(load_type & LOAD_BACKGROUND)  //check not necessary
        {
            geoRequestBackgroundLoad( gld, file );
            geoSetLoadState(gld,LOADING);
        }
        else if(load_type & (LOAD_NOW|LOAD_RELOAD))
        {
            geoRequestForegroundLoad( gld, file );
            geoSetLoadState(gld,LOADING);
        }
        else
            assert(0);

        if(load_type & (LOAD_NOW|LOAD_RELOAD) && !(load_type & LOAD_HEADER))
        {
            waitForGeoToFinishLoading(gld);
#ifdef CLIENT
            texCheckThreadLoader(); //inside if? //can I take this out now?
#endif
        }
    }
        
    PERFINFO_AUTO_STOP();

    return gld;
}

#if CLIENT
Model *modelCopyForModification(const Model *basemodel, int tex_count)
{
    Model *model = malloc(sizeof(Model));
    memcpy(model, basemodel, sizeof(Model));
    
    ZeroStruct(&model->unpack);
    ZeroStruct(&model->pack);

    model->tex_count = tex_count;
    model->tex_idx = calloc(sizeof(TexID), tex_count);

    model->tex_binds = 0;
    model->grid.cell = 0;
    model->grid.size = 0;
    model->ctris = 0;
    model->blend_modes = 0;
    model->lod_models = 0;
    model->lod_info = 0;
    model->vbo = 0;

    model->loadstate = NOT_LOADED;

    return model;
}

void modelToGMesh(GMesh *mesh, Model *srcmodel)
{
    int i, j;
    GTriIdx *meshtri;
    int *modeltri;

    assert(!srcmodel->boneinfo);

    PERFINFO_AUTO_START("modelToGMesh",1);
    modelUnpackAll(srcmodel);

    gmeshSetUsageBits(mesh, USE_POSITIONS|(srcmodel->unpack.norms?USE_NORMALS:0)|(srcmodel->unpack.sts?USE_TEX1S:0)|(srcmodel->unpack.sts3?USE_TEX2S:0));
    gmeshSetVertCount(mesh, srcmodel->vert_count);
    gmeshSetTriCount(mesh, srcmodel->tri_count);

    memcpy(mesh->positions, srcmodel->unpack.verts, mesh->vert_count * sizeof(Vec3));
    if (srcmodel->unpack.norms)
        memcpy(mesh->normals, srcmodel->unpack.norms, mesh->vert_count * sizeof(Vec3));
    if (srcmodel->unpack.sts)
        memcpy(mesh->tex1s, srcmodel->unpack.sts, mesh->vert_count * sizeof(Vec2));
    if (srcmodel->unpack.sts3)
        memcpy(mesh->tex2s, srcmodel->unpack.sts3, mesh->vert_count * sizeof(Vec2));

    meshtri = mesh->tris;
    modeltri = srcmodel->unpack.tris;
    for (i = 0; i < srcmodel->tex_count; i++)
    {
        for (j = 0; j < srcmodel->tex_idx[i].count; j++)
        {
            meshtri->idx[0] = *(modeltri++);
            meshtri->idx[1] = *(modeltri++);
            meshtri->idx[2] = *(modeltri++);
            meshtri->tex_id = srcmodel->tex_idx[i].id;
            meshtri++;
        }
    }

    PERFINFO_AUTO_STOP();
}

void modelFromGMesh(Model *model, const GMesh *mesh, const Model *srcmodel)
{
    int i, j, last_tex;
    GTriIdx *meshtri;
    int *modeltri;

    assert(!mesh->bonemats);
    assert(!mesh->boneweights);

    PERFINFO_AUTO_START("convertGMeshToModel",1);

    model->tri_count = mesh->tri_count;
    model->vert_count = mesh->vert_count;

    model->unpack.verts = malloc(mesh->vert_count * sizeof(Vec3));
    memcpy(model->unpack.verts, mesh->positions, mesh->vert_count * sizeof(Vec3));
    if (mesh->normals)
    {
        model->unpack.norms = malloc(mesh->vert_count * sizeof(Vec3));
        memcpy(model->unpack.norms, mesh->normals, mesh->vert_count * sizeof(Vec3));
    }
    if (mesh->tex1s)
    {
        model->unpack.sts = malloc(mesh->vert_count * sizeof(Vec2));
        memcpy(model->unpack.sts, mesh->tex1s, mesh->vert_count * sizeof(Vec2));
    }
    if (mesh->tex2s)
    {
        model->unpack.sts3 = malloc(mesh->vert_count * sizeof(Vec2));
        memcpy(model->unpack.sts3, mesh->tex2s, mesh->vert_count * sizeof(Vec2));
    }

    meshtri = mesh->tris;
    modeltri = model->unpack.tris = malloc(mesh->tri_count * sizeof(int) * 3);

    last_tex = 0;
    for (i = 0; i < mesh->tri_count; i++)
    {
        for (j = last_tex; j < model->tex_count; j++)
        {
            if (model->tex_idx[j].id == meshtri->tex_id)
            {
                model->tex_idx[j].count++;
                last_tex = j;
                break;
            }
        }
        assert(j < model->tex_count);

        *(modeltri++) = meshtri->idx[0];
        *(modeltri++) = meshtri->idx[1];
        *(modeltri++) = meshtri->idx[2];
        meshtri++;
    }

    if (model->gld->type & GEO_INIT_FOR_DRAWING)
        addModelDataSingle(model);

    PERFINFO_AUTO_STOP();
}

static VOID CALLBACK heyThreadLODAModel(ULONG_PTR dwParam)
{
    QueuedLod *lod = (QueuedLod *)dwParam;

    PERFINFO_AUTO_START("heyThreadLODAModel",1);
    EnterCriticalSection(&heyThreadLODAModel_CriticalSection);

    if (lod->just_free_reductions)
    {
        modelFreeAllUnpacked(lod->srcmodel);
    }
    else
    {
        LodModel *lodmodel = lod->lodmodel;
        Model *srcmodel = lod->srcmodel;
        Model *model = lodmodel->model;
        GMesh mesh={0};
        GMesh reducedmesh={0};
        F32 newerror;

        //////////////////////////////////////////////////////////////////////////
        // convert model to mesh
        modelToGMesh(&mesh, srcmodel);

        //////////////////////////////////////////////////////////////////////////
        // reduce mesh
        PERFINFO_AUTO_START("reduceGMesh",1);
        modelUnpackReductions(srcmodel);
        assert(srcmodel->unpack.reductions);
        newerror = 100.f * gmeshReduce(&reducedmesh, &mesh, srcmodel->unpack.reductions, lodmodel->error*0.01f, lod->method, lod->shrink_amount);
        assert(newerror >= 0);
        lodmodel->error = newerror;
        gmeshFreeData(&mesh);
        PERFINFO_AUTO_STOP();

        //////////////////////////////////////////////////////////////////////////
        // convert mesh to model
        modelFromGMesh(model, &reducedmesh, srcmodel);
        gmeshFreeData(&reducedmesh);

        //////////////////////////////////////////////////////////////////////////
        // final data fixup
        lodmodel->tri_percent = 100.f * ((F32)model->tri_count) / srcmodel->tri_count;
        model->loadstate = LOADED;
    }

    number_of_lods_in_background_loader--;
    free(lod);

    LeaveCriticalSection(&heyThreadLODAModel_CriticalSection);
    PERFINFO_AUTO_STOP();
}

Model *modelFindLOD(Model *srcmodel, F32 error, ReductionMethod method, F32 shrink_amount)
{
    int i;
    LodModel *lodmodel;
    Model *model;
    F32 tri_percent = 100.f - error;
    QueuedLod *queuedlod;

    PERFINFO_AUTO_START("modelFindLOD",1);


    assert(srcmodel->loadstate & LOADED);


    //////////////////////////////////////////////////////////////////////////
    // fix up error values
    if (eaSize(&srcmodel->lod_models))
    {
        if (method == ERROR_RMETHOD)
        {
            if (error < srcmodel->lod_minerror)
                return srcmodel;
            if (srcmodel->lod_maxerror && error > srcmodel->lod_maxerror)
                error = srcmodel->lod_maxerror;
        }
        if (method == TRICOUNT_RMETHOD)
        {
            if (tri_percent < srcmodel->lod_mintripercent)
            {
                tri_percent = srcmodel->lod_mintripercent;
                error = 100.f - tri_percent;
            }
            if (tri_percent > srcmodel->lod_maxtripercent)
                return srcmodel;
        }
    }


    //////////////////////////////////////////////////////////////////////////
    // see if we already have one
    for (i = 0; i < eaSize(&srcmodel->lod_models); i++)
    {
        F32 diff;
        if (method == TRICOUNT_RMETHOD)
            diff = tri_percent - srcmodel->lod_models[i]->tri_percent;
        else
            diff = error - srcmodel->lod_models[i]->error;

        if (fabs(diff) < 1)
        {
            PERFINFO_AUTO_STOP();
            return srcmodel->lod_models[i]->model;
        }
    }


    //////////////////////////////////////////////////////////////////////////
    // unpack reductions and fixup error values (if not done previously)
    if (!eaSize(&srcmodel->lod_models))
    {
        modelUnpackReductions(srcmodel);
        srcmodel->lod_minerror = 100.f * srcmodel->unpack.reductions->error_values[0];
        srcmodel->lod_maxerror = 100.f * srcmodel->unpack.reductions->error_values[0];
        for (i = 1; i < srcmodel->unpack.reductions->num_reductions; i++)
        {
            F32 errval = 100.f * srcmodel->unpack.reductions->error_values[i];
            MIN1(srcmodel->lod_minerror, errval);
            MAX1(srcmodel->lod_maxerror, errval);
        }
        srcmodel->lod_mintripercent = 100.f * ((F32)srcmodel->unpack.reductions->num_tris_left[srcmodel->unpack.reductions->num_reductions-1]) / srcmodel->tri_count;
        srcmodel->lod_maxtripercent = 100.f * ((F32)srcmodel->unpack.reductions->num_tris_left[0]) / srcmodel->tri_count;
        if (method == ERROR_RMETHOD)
        {
            if (error < srcmodel->lod_minerror)
                return srcmodel;
            if (error > srcmodel->lod_maxerror)
                error = srcmodel->lod_maxerror;
        }
        if (method == TRICOUNT_RMETHOD)
        {
            if (tri_percent < srcmodel->lod_mintripercent)
            {
                tri_percent = srcmodel->lod_mintripercent;
                error = 100.f - tri_percent;
            }
            if (tri_percent > srcmodel->lod_maxtripercent)
                return srcmodel;
        }
    }

    model = modelCopyForModification(srcmodel, srcmodel->tex_count);
    for (i = 0; i < model->tex_count; i++)
        model->tex_idx[i].id = srcmodel->tex_idx[i].id;


    //////////////////////////////////////////////////////////////////////////
    // add to lod model list
    MP_CREATE(LodModel, 1024);
    lodmodel = MP_ALLOC(LodModel);
    lodmodel->tri_percent = tri_percent;
    lodmodel->error = error;
    lodmodel->model = model;
    eaPush(&srcmodel->lod_models, lodmodel);
    model->srcmodel = srcmodel;


    //////////////////////////////////////////////////////////////////////////
    // add to background loader queue
    queuedlod = calloc(sizeof(*queuedlod), 1);
    queuedlod->lodmodel = lodmodel;
    queuedlod->srcmodel = srcmodel;
    queuedlod->method = method;
    queuedlod->shrink_amount = shrink_amount;

    EnterCriticalSection(&heyThreadLODAModel_CriticalSection);
    number_of_lods_in_background_loader++;
    LeaveCriticalSection(&heyThreadLODAModel_CriticalSection);

#if DISABLE_PARALLEL_LOD_THREAD
    heyThreadLODAModel((ULONG_PTR)queuedlod);
#else
    QueueUserAPC(heyThreadLODAModel, background_loader_handle, (ULONG_PTR)queuedlod );
#endif

    PERFINFO_AUTO_STOP();

    return model;
}

void modelDoneMakingLODs(Model *model)
{
    QueuedLod *queuedlod = calloc(sizeof(*queuedlod), 1);
    queuedlod->srcmodel = model;
    queuedlod->just_free_reductions = 1;

    EnterCriticalSection(&heyThreadLODAModel_CriticalSection);
    number_of_lods_in_background_loader++;
    LeaveCriticalSection(&heyThreadLODAModel_CriticalSection);

#if DISABLE_PARALLEL_LOD_THREAD
    heyThreadLODAModel((ULONG_PTR)queuedlod);
#else
    QueueUserAPC(heyThreadLODAModel, background_loader_handle, (ULONG_PTR)queuedlod );
#endif
}

static void modelFreeLODModel(void* lodmodelData)
{
    LodModel * lodmodel = (LodModel *)lodmodelData;
    if (!(lodmodel->model->loadstate & LOADED))
    {
        forceGeoLoaderToComplete();
        texForceTexLoaderToComplete(0);
    }
    modelFreeCache(lodmodel->model);
    free(lodmodel->model);
    MP_FREE(LodModel, lodmodel);
}

void modelFreeLODs(Model *model, int reset_lodinfos)
{
    eaDestroyEx(&model->lod_models, modelFreeLODModel);
    if (reset_lodinfos && !model->gld->lod_infos && model->pack.reductions.unpacksize)
        model->lod_info = lodinfoFromObjectName(0, model->name, model->filename, model->radius, 0, model->tri_count, model->autolod_dists);
}

void modelFreeAllLODs(int reset_lodinfos)
{
    StashElement element;
    StashTableIterator it;

    PERFINFO_AUTO_START("modelFreeAllLODs", 1);

    forceGeoLoaderToComplete();
    texForceTexLoaderToComplete(0);

    if (!glds_ht)
        return;
    stashGetIterator(glds_ht, &it);
    while(stashGetNextElement(&it, &element))
    {
        GeoLoadData*    gld;
        int            j;

        gld = stashElementGetPointer(element);
        for( j=0 ; j < gld->modelheader.model_count ; j++ )
        {
            Model *model = gld->modelheader.models[j];
            modelFreeLODs(model, reset_lodinfos);
        }
    }

    PERFINFO_AUTO_STOP();
}

#endif

void modelFreeCache(Model *model)
{
    int i=0;

    if ( model->ctris )
        modelFreeCtris(model);
    if (model->loadstate != LOADED)
        return;

    forceGeoLoaderToComplete();

#if CLIENT
    if (model->vbo) {
        modelFreeVertexObject(model->vbo);
        model->vbo = 0;
    }

    modelFreeAllUnpacked(model);

    eaDestroyEx(&model->lod_models, modelFreeLODModel);

    if (model->srcmodel)
    {
        SAFE_FREE(model->tex_idx);
    }

    zoNotifyModelFreed(model);
#endif
}

void modelListFree(GeoLoadData *gld)
{
    int j;

    // JE: This only actually frees the gld if it was fully loaded, if only the headers were
    //     loaded, then it does nothing, which is odd, because it frees everything, including the
    //     headers, if it had loaded everything...

    if (gld->loadstate != LOADED
#ifdef SERVER 
        && (!server_state.map_stats && !server_state.map_minimaps)
#endif
        )
        return;
        
    //printf("freeing gld: %s\n", gld->name);
    
    for( j=0 ; j < gld->modelheader.model_count ; j++ )
    {
        Model* model = gld->modelheader.models[j];
        modelFreeCache(model);
        SAFE_FREE(model->trick);
#if CLIENT
        SAFE_FREE(model->tex_binds);
        assert(!model->vbo); // freed in modelFreeCache
#endif
#if CLIENT | GETVRML
        if (gld->lod_infos)
        {
            freeModelLODInfoData(&gld->lod_infos[j]);
        }
        model->lod_info = 0;
#endif
    }
#if CLIENT | GETVRML
    SAFE_FREE(gld->lod_infos);
#endif
    if (stashFindPointerReturnPointer(glds_ht, gld->name) == gld)
        stashRemovePointer(glds_ht, gld->name, NULL);
    SAFE_FREE(gld->modelheader.model_data);
    eaDestroy(&gld->modelheader.models);
    SAFE_FREE(gld->header_data);
    SAFE_FREE(gld->geo_data);
    SAFE_FREE(gld);
}

#if CLIENT
Model **welded_models = 0;

void geoAddWeldedModel(Model *weldedModel)
{
    if (!welded_models)
        eaCreate(&welded_models);
    addModelDataSingle(weldedModel);
    eaPush(&welded_models, weldedModel);
}

void freeWeldedModels(void)
{
    int i;
    bool did_anything = false;
    for (i = 0; i < eaSize(&welded_models); i++)
    {
        SAFE_FREE(welded_models[i]->tex_idx);
        modelFreeCache(welded_models[i]);
        SAFE_FREE(welded_models[i]);
        did_anything = true;
    }
    if (did_anything) {
        eaDestroy(&welded_models);
        groupClearAllWeldedModels();
        zoClearOccluders();
    }
}
#endif

void modelFreeAllCache(GeoUseType unuse_type)
{
    StashElement element;
    StashTableIterator it;

    PERFINFO_AUTO_START("modelFreeAllCache", 1);

#if CLIENT
    freeWeldedModels();
#endif

    if (!glds_ht)
        return;
    stashGetIterator(glds_ht, &it);
    while(stashGetNextElement(&it, &element))
    {
        GeoLoadData*    gld;
        int            j;

        gld = stashElementGetPointer(element);
        for( j=0 ; j < gld->modelheader.model_count ; j++ )
        {
            modelFreeCache(gld->modelheader.models[j]);
        }
        gld->geo_use_type &= ~unuse_type;
        if (!gld->geo_use_type)
            modelListFree(gld);
    }
#if CLIENT
    zoClearOccluders();
    groupResetAll(RESET_TRICKS); // To free auto-lod objects
#endif

    PERFINFO_AUTO_STOP();
}

#ifdef SERVER
void modelFreeAllGeoData()
{
    StashElement element;
    StashTableIterator it;

    if (!glds_ht)
        return;
    stashGetIterator(glds_ht, &it);
    while(stashGetNextElement(&it, &element))
    {
        GeoLoadData*    gld = stashElementGetPointer(element);
        if ( gld->geo_data )
        {
            restoreStubOffsets(gld);
            SAFE_FREE(gld->geo_data);
        }

    }
}
#endif

void modelResetAllFlags(bool freeModels)
{
    StashElement element;
    StashTableIterator it;

#if CLIENT
    freeWeldedModels();
#endif

    if (!glds_ht)
        return;
    stashGetIterator(glds_ht, &it);
    while(stashGetNextElement(&it, &element))
    {
        GeoLoadData*    gld;
        int            j;

        gld = stashElementGetPointer(element);
        for( j=0 ; j < gld->modelheader.model_count ; j++ )
        {
            Model *model = gld->modelheader.models[j];
            if (model->loadstate != LOADED && !model->ctris) {
                continue;
            }
#if CLIENT
            if (model->tex_binds)
                modelResetFlags(model);
#else
            modelInitTrickNodeFromName(model->name,model);
#endif
            if (freeModels)
                modelFreeCache(model); // Will be auto-generated
        }
    }
}

#if CLIENT
void modelRebuildTextures(void)
{
    int i;
    StashElement element;
    StashTableIterator it;

    for (i = 0; i < eaSize(&welded_models); i++)
        addModelDataSingle(welded_models[i]);

    if (!glds_ht)
        return;

    stashGetIterator(glds_ht, &it);
    while(stashGetNextElement(&it, &element))
    {
        GeoLoadData*    gld;

        gld = stashElementGetPointer(element);
        if (!(gld->type & GEO_INIT_FOR_DRAWING))
            continue;
        addModelData(gld);
    }
}
#endif

static char *fmt1 = "  %3s %3s %3s %4s%4s %5s %s\n";
static char *fmt2 = "  %3d %3d %3d %4d%4d %5.3g %s";
static void modelPrintModelInfo(Model *model)
{
    printf(fmt2, model->vert_count, model->tri_count, model->tex_count, model->flags, model->id, model->radius, model->name);
    if (!sameVec3(model->scale, onevec3))
        printf("      Scale: %1.3f %1.3f %1.3f\n", model->scale[0], model->scale[1], model->scale[2]);
//    printf("      Min: %1.3f %1.3f %1.3f\n", model->min[0], model->min[1], model->min[2]);
//    printf("      Max: %1.3f %1.3f %1.3f\n", model->max[0], model->max[1], model->max[2]);
    if (model->api)
        printf(" (Has AltPivots)");
    if (model->boneinfo)
        printf(" (Has Bones)");
    printf("\n");
//    printf("     Data:\n");
//    printf("       Tris: %d/%d\n", model->pack.tris.packsize, model->pack.tris.unpacksize);
//    printf("       Verts:%d/%d\n", model->pack.verts.packsize, model->pack.verts.unpacksize);
//    printf("       Norms:%d/%d\n", model->pack.norms.packsize, model->pack.norms.unpacksize);
//    printf("       Sts:  %d/%d\n", model->pack.sts.packsize, model->pack.sts.unpacksize);
//    printf("       Wgts: %d/%d\n", model->pack.weights.packsize, model->pack.weights.unpacksize);
//    printf("       Matid:%d/%d\n", model->pack.matidxs.packsize, model->pack.matidxs.unpacksize);
//    printf("       Grid: %d/%d\n", model->pack.grid.packsize, model->pack.grid.unpacksize);
//    testGeoUnpack(model);
}

static void modelPrintModelHeaderInfo(ModelHeader *header)
{
    int i;
    printf("Header Name:\t%s\n", header->name);
    if (header->length)
        printf("Length: %f\n", header->length);
    printf("Models:\t\t%d\n", header->model_count);
    printf(fmt1, " V", " P", "Tex", "Flgs", "Bone", "Rad", "Name");
    for (i=0; i<header->model_count; i++)
        modelPrintModelInfo(header->models[i]);
}

// Called from GetVrml or in the Command window
void modelPrintFileInfo(char *fileName, GeoUseType use_type)
{
    GeoLoadData *gld = geoLoad(fileName, LOAD_NOW |(use_type&GEO_GETVRML_FASTLOAD ? LOAD_FASTLOAD:0), use_type);
    int i, j;
    if (!gld) {
        printf("Error reading file %s\n", fileName);
    } else {
        int *usedTex, usedTexCount, bFirst;
        printf("File %s:\n", gld->name);
        printf("Model version:\t%d\n", gld->file_format_version);
        printf("Header size:\t%d\nData size:\t%d\n", gld->headersize, gld->datasize);
        modelPrintModelHeaderInfo(&gld->modelheader);

        // only show textures which are referenced.  (tbd, update getvrml to only output used textures in this list)
        usedTex = alloca(sizeof(int) * gld->texnames.count);
        memset(usedTex, 0, sizeof(int) * gld->texnames.count);
        usedTexCount = 0;
        for (i=0; i<gld->modelheader.model_count; i++) {
            Model *model = gld->modelheader.models[i];
            for (j=0; j<model->tex_count; j++) {
                int texIdx = model->tex_idx[j].id;
                if(!usedTex[texIdx]) {
                    usedTex[texIdx] = 1;
                    usedTexCount++;
                }
            }
        }
        printf("Texture Names (%d): ", usedTexCount);
        bFirst = 1;
        for (i=0; i<gld->texnames.count; i++) 
        {
            if(usedTex[i])
            {
                printf("%s%s", (bFirst ? "" : ", "), gld->texnames.strings[i]);
                bFirst = 0;
            }
        }
        printf("\n");
    }
}

//////////////////////////////////////////////////////////////////////////

