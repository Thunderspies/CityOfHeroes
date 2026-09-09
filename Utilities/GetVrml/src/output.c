#include <utilitieslib/stdtypes.h>
#include "tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <utilitieslib/components/gridpoly.h>
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/utils/utils.h>
#include "gridcoll/grid.h"
#include <utilitieslib/utils/error.h>
#include "texsort.h"
#include "output.h" 
#include "assert.h"
#include "geo.h"
#include <utilitieslib/utils/error.h>
#include <io.h>
#include <sys/stat.h>
#include "seq/animtrackanimate.h"
#include <zlib.h>
#include "seq/tricks.h"
#include <utilitieslib/components/earray.h>
#include <utilitieslib/utils/file.h>


typedef struct
{
    char    *name;
    U8        *data;
    int        used;
    int        allocCount;
} MemBlock;

enum
{
    MEM_STRUCTDATA = 0,
    MEM_TEXNAMES,
    MEM_TEXIDX,
    MEM_MODELS,
    MEM_MODELHEADERS,
    MEM_OBJNAMES,
    MEM_PACKED,
    MEM_SCRATCH,
    MEM_HEADER,
};

static int        anim_header_count;

MemBlock mem_blocks[] =
{
    { "STRUCTDATA"},
    { "TEXNAMES"},
    { "TEXIDX"},
    { "MODELS"},
    { "MODELHEADERS"},
    { "OBJNAMES"},
    { "PACKED"},
    { "SCRATCH"},
    { "HEADER"},
};

static void *mallocBlockMem(int block_num,int amount)
{
    void    *ptr;
    MemBlock    *mblock;

    mblock = &mem_blocks[block_num];
    if (mblock->data) {
        mblock->data = realloc(mblock->data, mblock->used+amount+4);
        memset(mblock->data + mblock->used, 0, amount+4);
    } else {
        assert(!mblock->allocCount && !mblock->used);
        mblock->data = calloc(amount+4,1);
    }
    ptr = &mblock->data[mblock->used];
    mblock->used += amount;
    mblock->allocCount++;
    return ptr;
}

static void *allocBlockMem(int block_num,int amount)
{
    void    *ptr;

    ptr = mallocBlockMem(block_num,amount);
    memset(ptr,0,amount);
    return ptr;
}

static void *allocBlockMemVirt(int block_num,int amount)
{
    void    *ptr;

    ptr = mallocBlockMem(block_num,amount);
    memset(ptr,0,amount);
    return (void*)((U8*)ptr - mem_blocks[block_num].data);
}

static void *dereference(int block_num, void *ptr)
{
    return (void*)(mem_blocks[block_num].data + (int)ptr);
}

static void *reference(int block_num, void *ptr)
{
    return (void*)((U8*)ptr - mem_blocks[block_num].data);
}

//############### End Manage Memeory Blocks ############################################

//############### pack All Nodes and helper functions (make own file?) #####################

static int pack_hist[4];

static F32 quantF32(F32 val,F32 float_scale,F32 inv_float_scale)
{
    int        ival;
    F32        outval;

    ival = val * float_scale;
    ival &= ~1;
    outval = ival * inv_float_scale;
    return outval;
}

static U8 *compressDeltas(void *data,int *length,int stride,int count,PackType pack_type,F32 float_scale)
{
    static U32    *bits;
    static U8    *bytes;
    static int    max_bits,max_bytes;
    static int    byte_count[] = {0,1,2,4};
    int        i,j,k,t,val8,val16,val32,iDelta,val,code,cur_byte=0,cur_bit=0,bit_bytes;
    int        *iPtr = data;
    U16        *siPtr = data;
    F32        *fPtr = data,fDelta=0,inv_float_scale = 1;
    Vec3    fLast = {0,0,0};
    int        iLast[3] = {0,0,0};
    U8        *packed;

    *length = 0;
    if (!data || !count)
        return 0;
    if (float_scale)
        inv_float_scale = 1.f / float_scale;
    bits = calloc((2 * count * stride + 7)/8,1);
    bytes = calloc(count * stride * 4 + 1,1); // Add 1 for the float_scale!

    bytes[cur_byte++] = log2((int)float_scale);
    for(i=0;i<count;i++)
    {
        for(j=0;j<stride;j++)
        {
            if (pack_type == PACK_F32)
            {
                fDelta = quantF32(*fPtr++,float_scale,inv_float_scale) - fLast[j];
                val8 = fDelta * float_scale + 0x7f;
                val16 = fDelta * float_scale + 0x7fff;
                val32 = *((int *)&fDelta);
            }
            else
            {
                if (pack_type == PACK_U32)
                    t = *iPtr++;
                else
                    t = *siPtr++;
                iDelta = t - iLast[j] - 1;
                iLast[j] = t;
                val8 = iDelta + 0x7f;
                val16 = iDelta + 0x7fff;
                val32 = iDelta;
            }
            if (val8 == 0x7f)
            {
                code    = 0;
            }
            else if ((val8 & ~0xff) == 0)
            {
                code    = 1;
                val        = val8;
                fLast[j]= (val8 - 0x7f) * 1.f/float_scale + fLast[j];
            }
            else if ((val16 & ~0xffff) == 0)
            {
                code    = 2;
                val        = val16;
                fLast[j]= (val16 - 0x7fff) * 1.f/float_scale + fLast[j];
            }
            else
            {
                code    = 3;
                val        = val32;
                fLast[j]= fDelta + fLast[j];
            }
            bits[cur_bit >> 5] |= code << (cur_bit & 31);
            for(k=0;k<byte_count[code];k++)
                bytes[cur_byte++] = (val >> k*8) & 255;
            cur_bit+=2;
            pack_hist[code]++;
        }
    }

    bit_bytes = (cur_bit+7)/8;
    packed = malloc(bit_bytes + cur_byte);
    memcpy(packed,bits,bit_bytes);
    memcpy(packed+bit_bytes,bytes,cur_byte);
    free(bits);
    free(bytes);
    *length = bit_bytes + cur_byte;
    //uncompressDeltas(data,packed,stride,count,pack_type,float_scale);
    return packed;
}

static void *zipBlock(PackData *pack,void *data,int len)
{
    int        ziplen;
    U8        *zip_buf,*packed;

    ziplen    = len*2 + 128;
    zip_buf    = malloc(ziplen);
    compress((U8 *)zip_buf,&ziplen,data,len);
    if (ziplen < len * 0.8)
    {
        packed = allocBlockMem(MEM_PACKED,ziplen);
        memcpy(packed,zip_buf,ziplen);

    }
    else
    {
        packed = allocBlockMem(MEM_PACKED,len);
        memcpy(packed,data,len);
        ziplen = 0;
    }
    free(zip_buf);
    free(data);
    pack->data            = (U8*)((U8*)packed - (U8*)mem_blocks[MEM_PACKED].data);
    pack->packsize        = ziplen;
    pack->unpacksize    = len;

    return packed;
}

static void *packTexIdxs(GTriIdx *tris,int count,int *tex_count)
{
    int        last_id=-1,i,numtex=0,last_i=0;
    TexID    *tex_idxs;
    int        old_count = count;

    if (!tris)
        return 0;
    if (!count)
    {
        // Pack single dummy texidx to stop game from crashing
        numtex=1;
        count=1;
    } else {
        // Count unique IDs
        for(i=0;i<count;i++)
        {
            if (tris[i].tex_id != last_id)
            {
                last_id = tris[i].tex_id;
                numtex++;
            }
        }
    }

    // Alloc and fill in data
    tex_idxs = mallocBlockMem(MEM_TEXIDX,numtex * sizeof(TexID));
    last_id = -1;
    last_i = 0;
    numtex=0;
    for(i=0;i<old_count;i++)
    {
        if (tris[i].tex_id != last_id)
        {
            tex_idxs[numtex].id = tris[i].tex_id;
            if (numtex)
                tex_idxs[numtex-1].count = i - last_i;
            last_i = i;
            last_id = tris[i].tex_id;
            numtex++;
        }
    }

    if (!old_count)
    {
        tex_idxs[0].id = 0;
        tex_idxs[0].count = 0;
        numtex = 1;
    }
    else
    {
        tex_idxs[numtex-1].count = i - last_i;
    }

    *tex_count = numtex;
    return reference(MEM_TEXIDX, tex_idxs);
}

// optimized build crashes in this function, disabling opts for this function only until track down
#pragma optimize("", off)
static PolyCell *packPolyGridCells(PolyCell *cell)
{
    PolyCell    *pcell;
    int            i;

    assert(cell);
    pcell = allocBlockMemVirt(MEM_SCRATCH,sizeof(PolyCell));
    ((PolyCell*)dereference(MEM_SCRATCH, pcell))->tri_count = cell->tri_count;
    if (cell->children)
    {
        PolyCell **children = allocBlockMemVirt(MEM_SCRATCH,NUMCELLS_CUBE * sizeof(void *));
        ((PolyCell*)dereference(MEM_SCRATCH, pcell))->children = children;
        for(i=0;i<NUMCELLS_CUBE;i++) {
            if (cell->children[i])
                ((PolyCell**)dereference(MEM_SCRATCH, children))[i] = packPolyGridCells(cell->children[i]);
            else 
                ((PolyCell**)dereference(MEM_SCRATCH, children))[i] = 0;
        }
    }
    return pcell;
}
#pragma optimize("", on)

static void packPolyGridIdxs(PolyCell *cell,PolyCell *pcell)
{
    int            i,dsize;

    assert(cell->tri_count == ((PolyCell*)dereference(MEM_SCRATCH,pcell))->tri_count);
    if (cell->tri_count)
    {
        U16 *tri_idxs;
        dsize = sizeof(pcell->tri_idxs[0]) * cell->tri_count;
        tri_idxs = allocBlockMem(MEM_SCRATCH,dsize);
        ((PolyCell*)dereference(MEM_SCRATCH,pcell))->tri_idxs = reference(MEM_SCRATCH, tri_idxs);
        memcpy(tri_idxs,cell->tri_idxs,dsize);
    }
    if (cell->children)
    {
        for(i=0;i<NUMCELLS_CUBE;i++) {
            if (cell->children[i]) {
                PolyCell **children = ((PolyCell*)dereference(MEM_SCRATCH,pcell))->children;
                PolyCell *child = ((PolyCell**)dereference(MEM_SCRATCH, children))[i];
                assert(child);
                packPolyGridIdxs(cell->children[i],child);
            }
        }
    }
}

void packPolyGrids(PackData *pack,PolyCell *cell)
{
    PolyCell    *pcell;
    U8            *zipped,*packed;
    int            ziplen;

    mem_blocks[MEM_SCRATCH].used = 0;
    pcell = packPolyGridCells(cell);
    packPolyGridIdxs(cell,pcell);
    ziplen = mem_blocks[MEM_SCRATCH].used*2 + 128;
    zipped = malloc(ziplen);
    compress((U8 *)zipped,&ziplen,mem_blocks[MEM_SCRATCH].data,mem_blocks[MEM_SCRATCH].used);
    packed = allocBlockMem(MEM_PACKED,ziplen);
    memcpy(packed,zipped,ziplen);
    free(zipped);

    pack->unpacksize = mem_blocks[MEM_SCRATCH].used;
    pack->packsize = ziplen;
    pack->data = (U8*)(packed - mem_blocks[MEM_PACKED].data);
}

/*This does more processing than it should.  The processing should be done in geoAddFile. Should be 
rationalized some too.
*/

#define STRUCTDATA_SKEW 1 // Value to add to boneInfo pointers since they can be "0"

static BoneInfo * packSkin(GMesh *mesh, BoneData *bones, PackData *pack_weights, PackData *pack_matidxs)
{ 
    int         bn; 
    BoneInfo    *bi;

    if(!bones->numbones)
        return 0;
    bi = allocBlockMem(MEM_STRUCTDATA, sizeof(BoneInfo));
    assert(bi);
    bi->numbones = bones->numbones;    
    for (bn = 0; bn < bones->numbones; bn++)
    {    
        bi->bone_ID[bn]  = bones->bone_ID[bn];
        assert(bi->bone_ID[bn] != -1); 
    }
    
    {
        int i;
        U8        *u8Weights,*u8Matidxs;

        u8Weights = malloc(mesh->vert_count);
        u8Matidxs = malloc(mesh->vert_count*2);

        for(i=0;i<mesh->vert_count;i++)
        {
            u8Weights[i] = mesh->boneweights[i][0] * 255;
            u8Matidxs[i*2] = mesh->bonemats[i][0];
            u8Matidxs[i*2+1] = mesh->bonemats[i][1];
        }
        zipBlock(pack_weights,u8Weights,mesh->vert_count);
        zipBlock(pack_matidxs,u8Matidxs,mesh->vert_count*2);
    }
    //###################################################################

    bi->weights = 0;
    bi->matidxs = 0;
    return (void*)(((U8*)reference(MEM_STRUCTDATA, bi)) + STRUCTDATA_SKEW);
}

static AltPivotInfo * packAltPivotInfo(Mat4 altpivots[], int altpivotcount)
{
    AltPivotInfo *api;
    int i;

    if(!altpivotcount)
        return 0;

    api = allocBlockMem(MEM_STRUCTDATA, sizeof(AltPivotInfo));
    memset(api, 0, sizeof(AltPivotInfo));
    for(i = 0; i < altpivotcount ; i++)
    {
        copyMat4(altpivots[i], api->altpivot[i]);
    }
    api->altpivotcount = altpivotcount;

    return (void*)((U8*)reference(MEM_STRUCTDATA,api)+STRUCTDATA_SKEW);
}

U8 * checkForDuplicates(U8 * newdata, int newdata_amt, U8 * olddata, int olddata_amt)
{
    int i;
    if(newdata_amt == 0 || olddata_amt == 0)
        return 0;

    //i+2 instead? (size of the short we use as compressed anim data?
    for(i = 0 ; i <= olddata_amt - newdata_amt ; i++)
    {
        if(memcmp(&(newdata[0]), &(olddata[i]), newdata_amt) == 0)
            return &(olddata[i]);
    }
    return 0;

}
    

int matches_found;
int match_bytes_found;
int total_tracks;

static void packTris(PackData *pack,GTriIdx *tris,int tri_count)
{
    int        i,j;
    int        *mem,delta_tri_len;
    void    *delta_tris;

    if (!tris || !tri_count)
        return;
    mem = malloc(sizeof(int) * tri_count * 3);
    for (i = 0; i < tri_count; i++)
    {
        for (j = 0; j < 3; j++)
            mem[i*3 + j] = tris[i].idx[j];
    }
    delta_tris    = compressDeltas(mem,&delta_tri_len,3,tri_count,PACK_U32,0);
    zipBlock(pack,delta_tris,delta_tri_len);
    free(mem);
}

static void zipDeltas(PackData *pack,void *data,int stride,int vert_count,F32 float_scale)
{
    void    *deltas;
    int        delta_len;

    deltas    = compressDeltas(data,&delta_len,stride,vert_count,PACK_F32,float_scale);
    zipBlock(pack,deltas,delta_len);
}

__forceinline static writeFloatBlockMem(int block_num, float f)
{
    float *fmem = allocBlockMem(block_num, sizeof(float));
    *fmem = f;
}

__forceinline static writeIntBlockMem(int block_num, int i)
{
    int *imem = allocBlockMem(block_num, sizeof(int));
    *imem = i;
}

__forceinline static writeStringBlockMem(int block_num, char *str)
{
    if (str)
    {
        int slen = strlen(str) + 1;
        char *smem = allocBlockMem(block_num, slen);
        memcpy(smem, str, slen);
    }
    else
    {
        char *smem = allocBlockMem(block_num, 1);
        *smem = 0;
    }
}

static void dynAddInts(void **data, int *count, int *max_count, int num_ints, void *ints)
{
    void *ptr = dynArrayAdd(data, 1, count, max_count, sizeof(int) * num_ints);
    memcpy(ptr, ints, sizeof(int) * num_ints);
}

static void dynAddData(void **data, int *count, int *max_count, int datasize, void *newdata)
{
    void *ptr = dynArrayAdd(data, 1, count, max_count, datasize);
    memcpy(ptr, newdata, datasize);
}

static void packReductions(GMeshReductions *reductions, PackData *pack)
{
    void    *data=0;
    int        count=0, max_count=0;
    void    *deltas;
    int        delta_len;

    dynAddInts(&data, &count, &max_count, 1, &reductions->num_reductions);
    dynAddInts(&data, &count, &max_count, reductions->num_reductions, reductions->num_tris_left);
    dynAddInts(&data, &count, &max_count, reductions->num_reductions, reductions->error_values);
    dynAddInts(&data, &count, &max_count, reductions->num_reductions, reductions->remaps_counts);
    dynAddInts(&data, &count, &max_count, reductions->num_reductions, reductions->changes_counts);

    dynAddInts(&data, &count, &max_count, 1, &reductions->total_remaps);
    dynAddInts(&data, &count, &max_count, reductions->total_remaps * 3, reductions->remaps);
    dynAddInts(&data, &count, &max_count, 1, &reductions->total_remap_tris);
    dynAddInts(&data, &count, &max_count, reductions->total_remap_tris, reductions->remap_tris);

    dynAddInts(&data, &count, &max_count, 1, &reductions->total_changes);
    dynAddInts(&data, &count, &max_count, reductions->total_changes, reductions->changes);

    deltas = compressDeltas(reductions->positions,&delta_len,3,reductions->total_changes,PACK_F32,32768.f);
    dynAddInts(&data, &count, &max_count, 1, &delta_len);
    dynAddData(&data, &count, &max_count, delta_len, deltas);
    free(deltas);

    deltas = compressDeltas(reductions->tex1s,&delta_len,2,reductions->total_changes,PACK_F32,4096.f);
    dynAddInts(&data, &count, &max_count, 1, &delta_len);
    dynAddData(&data, &count, &max_count, delta_len, deltas);
    free(deltas);

    zipBlock(pack, data, count);
}

static bool isPlanarReflectionName(const char *name)
{
    // look for "__prquad"
    int nameLen = strlen(name);

    if (nameLen < 8 || (strcmp(name + nameLen - 8, "__prquad") != 0))
    {
        return false;
    }

    return true;
}

typedef struct PRQuadNode PRQuadNode;

typedef struct PRQuadNode
{

    int          num_quads;
    PRQuadNode    *next;
    Vec3         translate;
    const char    *name;
    const Node    *parent_node;
    Vec3         vert_array;    // first enty of array
} PRQuadNode;

PRQuadNode *PRQuadList = NULL;

static void saveReflectionQuads(Node **list, int count)
{
    int i;
    PRQuadNode *PRQNode = NULL;

    for (i = 0; i < count; i++)
    {
        const Node *node = list[i];
        int num_quads;
        PRQuadNode *newNode = NULL;

        if (!isPlanarReflectionName(node->name))
            continue;

        // number of verts should be a multiple of 6
        // The quads are in node->mesh as a triangle list, two triangles per quad, 6 verts per quad
        if ((node->mesh.vert_count % 6) != 0)
        {
            Errorf("Planar Reflection mesh %s does not have a vertex count (%d) which is a multiple of 6. Skipping this mesh.\n", node->name, node->mesh.vert_count);
            continue;
        }

        num_quads = node->mesh.vert_count / 6;

        newNode = (PRQuadNode *) malloc(sizeof(PRQuadNode) + sizeof(Vec3) * (num_quads * 4 - 1));
        
        if (!newNode)
        {
            Errorf("Memory allocation failed for saving %d vertices in saveReflectionQuads() for mesh %s\n", node->mesh.vert_count, node->name);
            continue;
        }
        else
        {
            int j;

            newNode->num_quads = num_quads;
            newNode->name = node->name;
            copyVec3(node->translate, newNode->translate);
            newNode->parent_node = NULL;

            for (j = 0; j < num_quads; j++)
            {
                copyVec3(node->mesh.positions[j * 6 + 0], (&newNode->vert_array)[j * 4 + 0]);
                copyVec3(node->mesh.positions[j * 6 + 1], (&newNode->vert_array)[j * 4 + 1]);
                copyVec3(node->mesh.positions[j * 6 + 2], (&newNode->vert_array)[j * 4 + 2]);
                copyVec3(node->mesh.positions[j * 6 + 4], (&newNode->vert_array)[j * 4 + 3]);
            }

            newNode->next = PRQuadList;
            PRQuadList = newNode;
        }
    }

    // For each reflection quad node, find its parent
    for (PRQNode = PRQuadList; PRQNode != NULL; PRQNode = PRQNode->next)
    {
        const Node *parent_node = NULL;
        float bestdistsqr = FLT_MAX;

        for (i = 0; i < count; i++)
        {
            const Node *node = list[i];
            float xdiff = PRQNode->translate[0] - node->translate[0];
            float zdiff = PRQNode->translate[2] - node->translate[2];
            float distsqr = xdiff * xdiff + zdiff * zdiff;

            if (isPlanarReflectionName(node->name))
                continue;

            if (distsqr < bestdistsqr)
            {
                bestdistsqr = distsqr;
                parent_node = node;
            }
        }

        PRQNode->parent_node = parent_node;
    }

    // NOTE: Doesn't handle the case where two prquad nodes has the same parent
    // To handle this properly, consolidate PRQuadNode's in the PRQuadList which
    // have the same parents.
    for (PRQNode = PRQuadList; PRQNode != NULL; PRQNode = PRQNode->next)
    {
        PRQuadNode *later_PRQNode = NULL;
        for (later_PRQNode = PRQNode->next; later_PRQNode != NULL; later_PRQNode = later_PRQNode->next)
        {
            if (later_PRQNode->parent_node == PRQNode->parent_node)
            {
                Errorf("Multiple prquad meshes found for model %s\n", PRQNode->parent_node->name);
                break;
            }
        }
    }
}

static PRQuadNode *getReflectionQuads(const Node *node)
{
    PRQuadNode **prev_pointer = &PRQuadList;
    PRQuadNode *current_node = PRQuadList;
    const char *name = node->name;
    int name_len = strlen(node->name);

    if (isPlanarReflectionName(node->name))
        return NULL;

    while(current_node != NULL && (current_node->parent_node != node))
    {
        prev_pointer = &current_node->next;
        current_node = current_node->next;
    }

    if (current_node)
    {
        // Take this node out of the list
        *prev_pointer = current_node->next;
        current_node->next = NULL;
    }

    return current_node;
}

#define GEO_VERSION_NUM 8
typedef struct _ModelFormatOnDisk_v8
{
    int                struct_size;
    F32                radius;
    int                tex_count;
    BoneInfo        *boneinfo;
    int                vert_count;
    int                tri_count;
    int                reflection_quad_count;    // Added with version 8
    TexID            *tex_idx;
    PolyGrid        grid;
    char            *name;
    AltPivotInfo    *api;
    Vec3            scale;
    Vec3            min,max;

    struct
    {
        PackData        tris;
        PackData        verts;
        PackData        norms;
        PackData        sts;
        PackData        st3s;
        PackData        weights;
        PackData        matidxs;
        PackData        grid;
        PackData        reductions;
        PackData        reflection_quads;    // Added with version 8
    } pack;

    //F32                lightmap_size;        // Removed with version 8

    F32                lod_distances[3];

    S16                id;

} ModelFormatOnDisk_v8;

/*Packs an node tree into an modelheader and its modelss 
note that the name of the header is the full path of the .wrl file minus ".wrl"
*/
void outputPackAllNodes(char *name, Node **list, int count)
{
    int                        i,j;
    Node                    *node;
    ModelFormatOnDisk_v8    *model;
    ModelHeader                *header;
    AnimKeys                *keys;
    F32                        longest = 0;

    saveReflectionQuads(list, count);

#define GROUPINFO 1

    anim_header_count++;

    header        = allocBlockMem(MEM_MODELHEADERS,sizeof(ModelHeader));
    model        = allocBlockMem(MEM_MODELS,count * sizeof(*model));

    strcpy(header->name,name);
    header->models    = (Model**)-1; // Not used here, and re-generated on the client
    header->model_count    = count;

    for (i = 0; i < count; i++, model++)
    {
        PRQuadNode *reflection_quad_node;

        node = list[i];

        if (!node->mesh.tri_count)
            printf("   WARNING: model %s has no triangles!\n", node->name);

        model->struct_size = sizeof(*model);

        for(j=0;j<3;j++)
            model->scale[j] = node->scale[j];
        model->name = allocBlockMem(MEM_OBJNAMES,strlen(node->name)+1);
        strcpy(model->name,node->name);
        model->name -= (int)mem_blocks[MEM_OBJNAMES].data;

        packTris(&model->pack.tris,node->mesh.tris,node->mesh.tri_count);
        zipDeltas(&model->pack.verts,node->mesh.positions,3,node->mesh.vert_count,32768.f);
        zipDeltas(&model->pack.norms,node->mesh.normals,3,node->mesh.vert_count,256.f);
        zipDeltas(&model->pack.sts,node->mesh.tex1s,2,node->mesh.vert_count,4096.f);
        zipDeltas(&model->pack.st3s,node->mesh.tex2s,2,node->mesh.vert_count,32768.f);

        copyVec3(node->lod_distances, model->lod_distances);

        if (node->reductions)
            packReductions(node->reductions,&model->pack.reductions);

        // See if any of the groups of reflection quads should go with this model
        reflection_quad_node = getReflectionQuads(node);

        // planar reflection quads are packed in the first model
        if (reflection_quad_node)
        {
            model->reflection_quad_count = reflection_quad_node->num_quads;
            zipDeltas(&model->pack.reflection_quads,&reflection_quad_node->vert_array,3,reflection_quad_node->num_quads * 4,32768.f);
            free(reflection_quad_node);
        }
        else
        {
            model->reflection_quad_count = 0;
        }

        if (!node->mesh.tri_count)
            gridPolys(&node->mesh.grid, &node->mesh);
        model->grid = node->mesh.grid;
        packPolyGrids(&model->pack.grid,model->grid.cell);

        model->tex_idx        = packTexIdxs(node->mesh.tris,node->mesh.tri_count,&model->tex_count);
        model->boneinfo        = packSkin(&node->mesh,&node->bones,&model->pack.weights,&model->pack.matidxs);
        model->api            = packAltPivotInfo(node->altMat, node->altMatCount);
        model->tri_count    = node->mesh.tri_count;
        model->vert_count    = node->mesh.vert_count;
        model->radius        = node->radius;
        model->id            = node->anim_id; 
        subVec3(node->min,node->mat[3],model->min);
        subVec3(node->max,node->mat[3],model->max);


        /*this is to figure out how long the animation track is by picking the longest of the 
        rotation track and xlation track.  Don't get %100
        */
        keys = &node->rotkeys;  
        if (keys->count && keys->times[keys->count-1] > longest)
            longest = keys->times[keys->count-1];
        keys = &node->poskeys;
        if (keys->count && keys->times[keys->count-1] > longest)
            longest = keys->times[keys->count-1];
    }
    header->length = longest;

    if (PRQuadList != NULL)
    {
        Errorf("Some Planar Reflection mesh names did not match any model names:\n");
        while(PRQuadList)
        {
            PRQuadNode *current = PRQuadList;
            PRQuadList = current->next;

            Errorf("    %s\n", current->name);
            free(current);
        }
    }
}

///################### End Pack Node ##########################################

/*fucked up a little bit*/
static int calcNameTableSize(char *names,int step,int count)
{
    int        i,bytes,idx_bytes;

    idx_bytes = (count + 1) * 4;
    bytes = idx_bytes;
    for(i=0;i<count;i++)
        bytes += strlen(&names[i * step]) + 1;
    bytes = (bytes + 7) & ~7;

    return bytes;
}

static void *nameTablePack(int mblock,char *names,int step,int count)
{
    int        i,bytes,idx_bytes,idx;
    char    *mem,*str;
    int        *idxs;

    idx_bytes = (count + 1) * 4;
    bytes = idx_bytes;
    for(i=0;i<count;i++)
        bytes += strlen(&names[i * step]) + 1;
    bytes = (bytes + 7) & ~7;

    mem = allocBlockMem(mblock,bytes);
    idxs = (void *)mem;
    idxs[0] = count;
    idxs++;
    str = mem + idx_bytes;
    for(idx=i=0;i<count;i++)
    {
        idxs[i] = idx;
        strcpy(&str[idx],&names[i * step]);
        idx += strlen(str + idx) + 1;
    }
    return mem;
}

#define MAX_GROUPS_IN_FILE 3000

int        def_seen[MAX_GROUPS_IN_FILE];
char    lines[MAX_GROUPS_IN_FILE][400];
int        line_count;

static char *fgets_nocr(char *buf,int len,FILE *file)
{
char    *ret;

    ret = fgets(buf,len,file);
    if (!ret)
        return 0;
    buf[strlen(buf)-1] = 0;
    return ret;
}

char *stripGeoName(char *s,char *buf)
{
    strcpy(buf,s);
    s = strstr(buf,"__");
    if (s)
        *s = 0;
    return buf;
}

/*reset the globals (called after outputData and outputGroupInfo are done) So this 
*/
void outputResetVars()
{
    int        i;

    for(i=0;i<ARRAY_SIZE(mem_blocks);i++)
    {
        SAFE_FREE(mem_blocks[i].data);
        mem_blocks[i].used = 0;
        mem_blocks[i].allocCount = 0;
    }
    anim_header_count = 0;
    texNameClear(1);
}

/*
*/
void outputGroupInfo(char *fname)
{
    int            i,size,count;
    ModelFormatOnDisk_v8    *model;
    FILE        *file;
    char        stripname[1000];
    extern        int no_check_out;

    size = mem_blocks[MEM_MODELS].used;
    count = size / sizeof(*model);
    model = (void *)mem_blocks[MEM_MODELS].data;

    _chmod(fname, _S_IREAD | _S_IWRITE);
    
    file = fopen(fname,"wt");
    if (!file)
        FatalErrorf("can't open %s for writing",fname);
    for(i=0;i<count;i++,model++)
    {
        stripGeoName(model->name,stripname);
        fprintf(file,"Def %s\n Obj %s\nDefEnd\n\n",stripname,stripname);
    }
    fclose(file);

}

int foundDuplicateNames(char *fname)
{
    int            i,size,count;
    ModelFormatOnDisk_v8    *model;
    char        stripname[1000];
    extern int addGroupName(char *fname, char *objname, int fatal);

    size = mem_blocks[MEM_MODELS].used;
    count = size / sizeof(*model);
    model = (void *)mem_blocks[MEM_MODELS].data;

    for(i=0;i<count;i++,model++)
    {
        stripGeoName(dereference(MEM_OBJNAMES, model->name),stripname);
        if (addGroupName(fname, stripname, 0))
            return 1;
    }
    return 0;
}

void appendHeader(int blocknum)
{
    void    *ptr;

    ptr    = allocBlockMem(MEM_HEADER,mem_blocks[blocknum].used);
    memcpy(ptr,mem_blocks[blocknum].data,mem_blocks[blocknum].used);
}

void *compressHeader(int *ziplen,int *header_len,int data_size)
{
    U8        *zipped;
    int        header_size, ret;
    int        *texname_blocksize_ptr,*objname_blocksize_ptr,*data_size_ptr,*texidx_blocksize_ptr;

    assert(mem_blocks[MEM_HEADER].used == 0);

    data_size_ptr            = allocBlockMem(MEM_HEADER,4);
    *data_size_ptr            = data_size;
    texname_blocksize_ptr    = allocBlockMem(MEM_HEADER,4);
    *texname_blocksize_ptr    = mem_blocks[MEM_TEXNAMES].used;
    objname_blocksize_ptr    = allocBlockMem(MEM_HEADER,4);
    *objname_blocksize_ptr    = mem_blocks[MEM_OBJNAMES].used;
    texidx_blocksize_ptr    = allocBlockMem(MEM_HEADER,4);
    *texidx_blocksize_ptr    = mem_blocks[MEM_TEXIDX].used;

    appendHeader(MEM_TEXNAMES);
    appendHeader(MEM_OBJNAMES);
    appendHeader(MEM_TEXIDX);
    appendHeader(MEM_MODELHEADERS);
    appendHeader(MEM_MODELS);
    header_size = mem_blocks[MEM_HEADER].used;

    *ziplen = header_size*2 + 128;
    zipped = malloc(*ziplen);
    ret = compress((U8 *)zipped,ziplen,mem_blocks[MEM_HEADER].data,header_size);
    assert(ret == Z_OK);
    *header_len = header_size;
    return zipped;
}

void outputData(char *fname)
{
    FILE        *file;
    U8            *header;
    int            oziplen, ziplen,header_len,i,count,datasize,pos,structdata_offset,packdata_offset;
    ModelFormatOnDisk_v8    *model;


    nameTablePack(MEM_TEXNAMES,tex_names[0].name,sizeof(tex_names[0]),tex_name_count);
    mem_blocks[MEM_OBJNAMES].used = (mem_blocks[MEM_OBJNAMES].used + 3) & ~3;
    mem_blocks[MEM_PACKED].used = (mem_blocks[MEM_PACKED].used + 3) & ~3;

    model            = (void *)mem_blocks[MEM_MODELS].data;
    count        = mem_blocks[MEM_MODELS].used / sizeof(*model);

    structdata_offset = (int)(mem_blocks[MEM_STRUCTDATA].data - mem_blocks[MEM_PACKED].used);
    packdata_offset = (int)mem_blocks[MEM_PACKED].data;
    for( i = 0 ; i < count ; i++, model++ )
    {
        assert((int)model->name < mem_blocks[MEM_OBJNAMES].used); // Should already be packed
        assert((int)model->pack.tris.data < mem_blocks[MEM_PACKED].used); // Should already be packed
        assert((int)model->pack.verts.data < mem_blocks[MEM_PACKED].used); // Should already be packed
        assert((int)model->pack.norms.data < mem_blocks[MEM_PACKED].used); // Should already be packed
        assert((int)model->pack.sts.data < mem_blocks[MEM_PACKED].used); // Should already be packed
        assert((int)model->pack.st3s.data < mem_blocks[MEM_PACKED].used); // Should already be packed
        assert((int)model->pack.grid.data < mem_blocks[MEM_PACKED].used); // Should already be packed
        assert((int)model->pack.weights.data < mem_blocks[MEM_PACKED].used); // Should already be packed
        assert((int)model->pack.matidxs.data < mem_blocks[MEM_PACKED].used); // Should already be packed
        assert((int)model->pack.reductions.data < mem_blocks[MEM_PACKED].used); // Should already be packed
        model->grid.cell = 0;

        if(model->api)
            model->api        = (void *)((U32)dereference(MEM_STRUCTDATA, ((U8*)model->api) - STRUCTDATA_SKEW) - structdata_offset);
        if (model->boneinfo)
            model->boneinfo = (void *)((U32)dereference(MEM_STRUCTDATA, ((U8*)model->boneinfo) - STRUCTDATA_SKEW) - structdata_offset);
    }

    // open file
    file = fopen(fname,"wb");
    if (!file)
        FatalErrorf("Can't open %s for writing! (Look for Checkout errors above)\n",fname);

    datasize = mem_blocks[MEM_PACKED].used + mem_blocks[MEM_STRUCTDATA].used;
    //pack MEM_TEXNAMES mem block [one int as tex_name count + (tex_name_count *(one ptr to the tex name in MEM_TEXNAMES * size of that texname string)]
    header = compressHeader(&ziplen,&header_len,datasize);
    oziplen = ziplen;

    {
        int version = GEO_VERSION_NUM;
        int zero = 0;

        ziplen += 12; // compensate for 2 * sizeof(header_len) + sizeof(version) + sizeof(new header) so pig system will cache everything
        fwrite(&ziplen,4,1,file);
        fwrite(&zero,4,1,file);
        fwrite(&version,4,1,file);
    }

    fwrite(&header_len,4,1,file);
    fwrite(header,oziplen,1,file);
    pos = ftell(file);
    fwrite(mem_blocks[MEM_PACKED].data,1,mem_blocks[MEM_PACKED].used,file);
    fwrite(mem_blocks[MEM_STRUCTDATA].data,1,mem_blocks[MEM_STRUCTDATA].used,file);
    fclose(file);
    free(header);
    //testme(fname);

    model            = (void *)mem_blocks[MEM_MODELS].data;
    for( i = 0 ; i < count ; i++, model++ )
    {
        model->name = dereference(MEM_OBJNAMES, model->name);
    }
}

