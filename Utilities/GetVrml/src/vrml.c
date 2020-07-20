#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <utilitieslib/stdtypes.h>
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/utils/error.h>
#include <utilitieslib/utils/file.h>
#include <utilitieslib/assert/assert.h>
#include "texsort.h" //Does get vrml need this?  //JE: Yes!  Only #ifdef GETVRML
#include "vrml.h"
#include "tree.h"
#include "geo.h"

typedef struct
{
    int        vert_idxs[3];
    int        norm_idxs[3];
    int        st_idxs[3];
    int        tex_idx;
} TriIdx;

typedef struct
{
    Vec3        *verts;
    Vec3        *norms;
    Vec2        *sts;

    TriIdx        *tris;

    int            st_count;
    int            vert_count;
    int            norm_count;
    int            tri_count;
    int            tex_idx;

    int            numbones;
    char        bonelist[MAX_OBJBONES][15];  //array of all bones affecting this Shape (15 chars per bone name)
    F32            *bone_idx[MAX_OBJBONES];

} VrmlShape;

static int addVert(const Vec3 v,const Vec2 st,const Vec3 norm,VrmlShape *shape,F32 weight[],int numbones)
{
    int i,j;

    i = shape->vert_count;
    if (!st)
        st = zerovec3;
    if (!norm)
        norm = zerovec3;
    if (!v)
        v = zerovec3;

    if (shape->verts)
        copyVec3(v,shape->verts[i]);
    if (shape->sts)
        copyVec2(st,shape->sts[i]);
    if (shape->norms)
        copyVec3(norm,shape->norms[i]);

    for (j = 0; j < numbones ; j++)
    {
        if (shape->bone_idx[j])
            (shape->bone_idx[j])[i] = weight[j];  
    }

    shape->vert_count++;
    shape->st_count = shape->vert_count;
    shape->norm_count = shape->vert_count;

    return i;
}

static void simplifyMergeVrmlShapes(VrmlShape *shape,VrmlShape *simple)
{
    int        i,j,maxverts,tc;
    F32        *v=0,*n=0;
    F32        *st=0;
    F32     weight[BONES_ON_DISK];
    int     k;

    tc = simple->tri_count;
    maxverts = simple->vert_count + shape->tri_count * 3;
    if (shape->norms || simple->norms)
        simple->norms = realloc(simple->norms,maxverts * sizeof(Vec3));
    if (shape->verts || simple->verts)
        simple->verts = realloc(simple->verts,maxverts * sizeof(Vec3));

    if (shape->sts || simple->sts)
    {
        st = calloc(maxverts, sizeof(Vec2));
        if (simple->sts)
        {
            memcpy(st,simple->sts,simple->st_count * sizeof(Vec2));
            free(simple->sts);
        }
        simple->sts = (void *)st;
    }

    simple->tris = realloc(simple->tris,(tc + shape->tri_count) * sizeof(TriIdx));

    for (i = 0 ; i < shape->numbones ; i++)
        simple->bone_idx[i] = realloc(simple->bone_idx[i] , maxverts * sizeof(F32));

    for (i=0;i<shape->tri_count;i++)
    {
        for (j=0;j<3;j++)
        {
            if (shape->verts)
                v =    shape->verts[shape->tris[i].vert_idxs[j]];
            if (shape->norms)
                n =    shape->norms[shape->tris[i].norm_idxs[j]];
            if (shape->sts)
                st = shape->sts[shape->tris[i].st_idxs[j]];

            for (k = 0; k < shape->numbones; k++)
                weight[k] = (shape->bone_idx[k])[shape->tris[i].vert_idxs[j]];

            simple->tris[tc + i].vert_idxs[j]    = addVert(v,st,n,simple,weight,shape->numbones); 
            simple->tris[tc + i].st_idxs[j]        = simple->tris[tc + i].vert_idxs[j];
            simple->tris[tc + i].norm_idxs[j]    = simple->tris[tc + i].vert_idxs[j];
        }
        simple->tris[tc + i].tex_idx = shape->tris[i].tex_idx;
    }

    simple->tri_count    += shape->tri_count;
    assert(simple->tri_count < 1000000 && simple->tri_count >= 0);
    simple->tex_idx        = shape->tex_idx;

    simple->numbones = shape->numbones; 
    for (i = 0; i < shape->numbones; i++)
        strcpy(simple->bonelist[i], shape->bonelist[i]); 
}

static void freeVrmlShape(VrmlShape *shape)
{
    int i;
    SAFE_FREE(shape->norms);
    SAFE_FREE(shape->verts);
    SAFE_FREE(shape->sts);
    SAFE_FREE(shape->tris);

    for (i = 0; i < shape->numbones; i++)
    {
        SAFE_FREE(shape->bone_idx[i]);
    }

    shape->numbones = 0;

    shape->norm_count = 0;
    shape->vert_count = 0;
    shape->st_count = 0;
    shape->tri_count = 0;
}

static void convertVrmlShapeToGMesh(BoneData *bones, GMesh *mesh, const VrmlShape *shape)
{
    Vec2 *bone_weights = 0;
    GBoneMats *bone_matidxs = 0;
    int i, j, usage = 0;

    gmeshFreeData(mesh);

    if (shape->vert_count)
        usage |= USE_POSITIONS;
    if (shape->norm_count)
        usage |= USE_NORMALS;
    if (shape->st_count)
        usage |= USE_TEX1S;
    if (shape->numbones)
        usage |= USE_BONEWEIGHTS;
    gmeshSetUsageBits(mesh, usage);

    // calculate bone weights and matrix indices
    if (shape->numbones)
    {
        F32 total;
        int bone;

        bone_weights = calloc(shape->vert_count, sizeof(*bone_weights));
        bone_matidxs = calloc(shape->vert_count, sizeof(*bone_matidxs));
        for (i = 0; i < shape->vert_count; i++)
        {
            bone = 0;
            for (j = 0; j < shape->numbones; j++)
            {
                if (shape->bone_idx[j][i] != 0.0)
                {
                    bone_matidxs[i][bone] = j * 3;
                    bone_weights[i][bone] = shape->bone_idx[j][i];
                    bone++;
                }
                if (bone >= 2)
                    break;
            }

            total = bone_weights[i][0] + bone_weights[i][1];
            if (total != 1.0)
                bone_weights[i][0] += 1.0 - total;
        }
    }

    // add tris and verts to generic mesh
    for (i = 0; i < shape->tri_count; i++)
    {
        TriIdx *tri = &shape->tris[i];
        int idx0, idx1, idx2;
        idx0 = gmeshAddVert(mesh, 
            shape->verts?shape->verts[tri->vert_idxs[0]]:0,
            shape->norms?shape->norms[tri->norm_idxs[0]]:0,
            shape->sts?shape->sts[tri->st_idxs[0]]:0,
            0,
            shape->numbones?bone_weights[tri->vert_idxs[0]]:0,
            shape->numbones?bone_matidxs[tri->vert_idxs[0]]:0,
            0);
        idx1 = gmeshAddVert(mesh, 
            shape->verts?shape->verts[tri->vert_idxs[1]]:0,
            shape->norms?shape->norms[tri->norm_idxs[1]]:0,
            shape->sts?shape->sts[tri->st_idxs[1]]:0,
            0,
            shape->numbones?bone_weights[tri->vert_idxs[1]]:0,
            shape->numbones?bone_matidxs[tri->vert_idxs[1]]:0,
            0);
        idx2 = gmeshAddVert(mesh, 
            shape->verts?shape->verts[tri->vert_idxs[2]]:0,
            shape->norms?shape->norms[tri->norm_idxs[2]]:0,
            shape->sts?shape->sts[tri->st_idxs[2]]:0,
            0,
            shape->numbones?bone_weights[tri->vert_idxs[2]]:0,
            shape->numbones?bone_matidxs[tri->vert_idxs[2]]:0,
            0);
        gmeshAddTri(mesh, idx0, idx1, idx2, tri->tex_idx, 0);
    }

    // fill bone data
    bones->numbones = shape->numbones; 
    for(i = 0; i < shape->numbones; i++)
        bones->bone_ID[i] = bone_IdFromText(shape->bonelist[i]);

    // cleanup
    SAFE_FREE(bone_weights);
    SAFE_FREE(bone_matidxs);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#define MEMCHUNK (2 << 20)

typedef struct
{
    char *name;
    int value;
} VrmlToken;

enum
{
    VRML_DEF = 1,
    VRML_USE,
    VRML_TRANSFORM,
    VRML_TRANSLATION,
    VRML_PIVOT,
    VRML_ROTATION,
    VRML_SCALE,
    VRML_SCALEORIENTATION,
    VRML_CENTER,
    VRML_CHILDREN,
    VRML_TIMESENSOR,
    VRML_POSINTERP,
    VRML_KEY,
    VRML_KEYVALUE,
    VRML_ROTINTERP,
    VRML_SHAPE,
    VRML_APPEARANCE,
    VRML_MATERIAL,
    VRML_TEXTURE,
    VRML_URL,
    VRML_DIFFUSECOLOR,
    VRML_GEOMETRY,
    VRML_INDEXEDFACESET,
    VRML_CCW,
    VRML_SOLID,
    VRML_CONVEX,
    VRML_COLORPERVERTEX,
    VRML_COLORINDEX,
    VRML_COORD,
    VRML_COORDINDEX,
    VRML_TEXCOORD,
    VRML_TEXCOORDINDEX,
    VRML_NODESTART,
    VRML_NODEEND,
    VRML_ARRAYSTART,
    VRML_ARRAYEND,
    VRML_POINTLIGHT,
    VRML_SPOTLIGHT,
    VRML_INTENSITY,
    VRML_COLOR,
    VRML_LOCATION,
    VRML_RADIUS,
    VRML_ATTENUATION,
    VRML_ON,
    VRML_FALSE,
    VRML_TRUE,
    VRML_SKIN, 
    VRML_NORMAL, 
    VRML_NORMALINDEX,
    VRML_NORMALPERVERTEX, 
    VRML_TEXTURETRANSFORM,
};    
    
VrmlToken vrml_tokens[] =
{
    { "DEF",                    VRML_DEF },
    { "USE",                    VRML_USE },
    { "Transform",                VRML_TRANSFORM },
    { "translation",            VRML_TRANSLATION },
    { "pivot",                    VRML_PIVOT },
    { "rotation",                VRML_ROTATION },
    { "scale",                    VRML_SCALE },
    { "scaleOrientation",        VRML_SCALEORIENTATION },
    { "center",                    VRML_CENTER },
    { "children",                VRML_CHILDREN },
    { "TimeSensor",                VRML_TIMESENSOR },
    { "PositionInterpolator",    VRML_POSINTERP },
    { "key",                    VRML_KEY },
    { "keyvalue",                VRML_KEYVALUE },
    { "OrientationInterpolator",VRML_ROTINTERP },
    { "Shape",                    VRML_SHAPE },
    { "appearance",                VRML_APPEARANCE },
    { "material",                VRML_MATERIAL },
    { "texture",                VRML_TEXTURE },
    { "url",                    VRML_URL },
    { "DiffuseColor",            VRML_DIFFUSECOLOR },
    { "geometry",                VRML_GEOMETRY },
    { "ccw",                    VRML_CCW },
    { "convex",                    VRML_CONVEX },
    { "IndexedFaceSet",            VRML_INDEXEDFACESET },
    { "solid",                    VRML_SOLID },
    { "coord",                    VRML_COORD },
    { "coordindex",                VRML_COORDINDEX },
    { "texcoord",                VRML_TEXCOORD },
    { "texcoordindex",            VRML_TEXCOORDINDEX },
    { "PointLight",                VRML_POINTLIGHT },
    { "SpotLight",                VRML_SPOTLIGHT },
    { "intensity",                VRML_INTENSITY },
    { "color",                    VRML_COLOR },
    { "ColorPerVertex",            VRML_COLORPERVERTEX },
    { "ColorIndex",                VRML_COLORINDEX },
    { "location",                VRML_LOCATION },
    { "radius",                    VRML_RADIUS },
    { "attenuation",            VRML_ATTENUATION },
    { "on",                        VRML_ON },
    { "true",                    VRML_TRUE },
    { "false",                    VRML_FALSE },
    { "{",                        VRML_NODESTART },
    { "}",                        VRML_NODEEND },
    { "[",                        VRML_ARRAYSTART },
    { "]",                        VRML_ARRAYEND },
    { "skin",                   VRML_SKIN }, 
    { "normal",                 VRML_NORMAL },
    { "normalIndex",            VRML_NORMALINDEX },
    { "normalPerVertex",        VRML_NORMALPERVERTEX }, 
    { "textureTransform",        VRML_TEXTURETRANSFORM },
    { 0,                        0 },
};

char last_tokname[1000];

typedef struct
{
    char    *text;
    int        idx;
    char    token[1024];
    int        cmd;
} TokenInfo;

TokenInfo vrml_token_info;

static void setVrmlText(char *text)
{
    memset(&vrml_token_info,0,sizeof(vrml_token_info));
    vrml_token_info.text = text;
}

static char *getTok()
{
char        *s;
int            idx,quote=0;
TokenInfo    *tok;

    tok = &vrml_token_info;
    s = tok->text + tok->idx;
    for(;;)
    {
        if (*s == 0)
            return 0;

        // remove any leading whitespace
        idx = (int)strspn(s,"\n\r \t,");
        s += idx;

        // remove any comments (we only handle comments at start of non-whitespace)
        if (*s == '#')
        {
            idx = (int)strcspn(s,"\n\r");
            s += idx;
            idx = (int)strspn(s,"\n\r");
            s += idx;
            continue;
        }

        if (*s == '[' || *s == ']' || *s == '{' || *s == '}')
            idx = 1;
        else if (*s == '"')
        {
            s++;
            idx = (int)strcspn(s,"\n\r\"");
            quote = 1;
        }
        else
            idx = (int)strcspn(s,"\n\r \t,]}[{");
        strncpy(tok->token,s,idx);
        tok->token[idx] = 0;
        if (quote) {
            idx += (int)strcspn(s + idx,"\n\r \t,]}[{");
        }
        tok->idx = s - tok->text + idx;
        if (tok->token[0] == 0)
            return 0;
        return tok->token;
    }
}

static int tokNameCmp (const VrmlToken *cmd1, const VrmlToken *cmd2)
{
    if (!cmd1 || !cmd2)
        return -1;
    return stricmp(cmd1->name,cmd2->name);
}

static int cmdOldSort(VrmlToken *tokens)
{
int        count;

    for(count=0;tokens[count].name;count++)
        ;
    qsort(tokens,count,sizeof(VrmlToken),
          (int (*) (const void *, const void *)) tokNameCmp);
    return count;
}

static int cmdOldNum(char *s)
{
VrmlToken    search,*match;
static    int vrml_token_count;

    if (!vrml_token_count)
        vrml_token_count = cmdOldSort(vrml_tokens);
    if (!s)
        return 0;
    strcpy(last_tokname,s);

    search.name = s;

    match = bsearch(&search, vrml_tokens, vrml_token_count,
                sizeof(VrmlToken),(int (*) (const void *, const void *))tokNameCmp);

    if (match)
        return match->value;
    return -1;
#if 0
int        i;

    for(i=0;vrml_tokens[i].name;i++)
    {
        if (stricmp(s,vrml_tokens[i].name) == 0)
            return vrml_tokens[i].value;
    }
    return -1;
#endif
}

static int getCmd()
{
    return cmdOldNum(getTok());
}

static void skipBlock()
{
int        depth = 0,cmd;

    for(;;)
    {
        cmd = getCmd();
        if (!cmd)
            FatalErrorf("Got lost in skipblock, depth = %d\n",depth);
        
        if (cmd == VRML_ARRAYSTART || cmd == VRML_NODESTART)
            depth++;
        if (cmd == VRML_ARRAYEND || cmd == VRML_NODEEND)
            depth--;
        if (!depth && (cmd == VRML_NODEEND || cmd == VRML_ARRAYEND))
            return;
    }
    printf("");
}

static F32 getF32()
{
    return atof(getTok());
}

static void getVec3(Vec3 v)
{
int        i;

    for(i=0;i<3;i++)
        v[i] = atof(getTok());
}

static void getVec4(Vec4 v)
{
int        i;

    for(i=0;i<4;i++)
        v[i] = atof(getTok());
}

static F32 *getF32s(int *count)
{
char    *s;
static F32    *list = NULL;
static int list_len = 0;
F32        *ptr;
int        i;

    getCmd();
    for(i=0;;i++)
    {
        s = getTok();
        if (*s == ']')
            break;
        if (i >= list_len)
        {
            list_len+=1000;
            list = realloc(list,sizeof(list[0]) * list_len);
        }
        list[i] = atof(s);
    }
    ptr = malloc(i * sizeof(F32));
    memcpy(ptr,list,i * sizeof(F32));
    //free(list);
    *count = i;
    return ptr;
}

static S32 *getS32s(int *count)
{
char    *s;
static S32    *list = NULL;
static int list_len = 0;
S32        *ptr;
int        i;

    getCmd();
    for(i=0;;i++)
    {
        s = getTok();
        if (*s == ']')
            break;
        if (i >= list_len)
        {
            list_len+=1000;
            list = realloc(list,sizeof(list[0]) * list_len);
        }
        list[i] = atoi(s);
    }
    ptr = malloc(i * sizeof(S32));
    memcpy(ptr,list,i * sizeof(S32));
    //free(list);
    *count = i;
    return ptr;
}

static void getAnimKeys(AnimKeys *keys)
{
int        found = 0,cmd,spam;

    while(found != 2)
    {
        cmd = getCmd();
        if (cmd == VRML_KEY)
        {
            keys->times = getF32s(&keys->count);
            found++;
        }
        if (cmd == VRML_KEYVALUE)
        {
            keys->vals = getF32s(&spam);
            found++;
        }
    }
    getCmd();
}

static void getAppearance(VrmlShape *shape)
{
int        cmd;
char    *s;

    for(;;)
    {
        cmd = getCmd();
        if (cmd == VRML_MATERIAL || cmd == VRML_TEXTURETRANSFORM)
            skipBlock();
        else if (cmd == VRML_TEXTURE)
        {
            getTok();
            getCmd();
            getTok();
            s = getTok();
#ifdef GETVRML
            shape->tex_idx = texNameAdd(s); //to do, extricate this from vrml file reading
#else
            shape->tex_idx = 0; 
#endif
            getTok();
        }
        else if (cmd == VRML_NODEEND)
            break;
    
    }
}

static void getLight(char *name)
{
int        depth = 0;
Vec3    garbage;
F32        junk;

    for(;;)
    {
        switch(getCmd())
        {
            case VRML_INTENSITY:
                junk = getF32();
            xcase VRML_COLOR:
                getVec3(garbage);
            xcase VRML_LOCATION:
                getVec3(garbage);
            xcase VRML_RADIUS:
                junk = getF32();
            xcase VRML_ATTENUATION:
                getVec3(garbage);
            xcase VRML_ON:
                getTok(); // should check it, but what the hey
            xcase VRML_NODESTART:
                depth++;
            xcase VRML_NODEEND:
                if (--depth <= 0)
                    return;
        }
    }
}

static F32 *getCoords(int *count)
{
F32        *coords;

    getTok();
    getTok();
    getTok();
    getTok();
    getTok();
    coords = getF32s(count);
    getTok();
    return coords;
}

static F32 *getNormals(int *count)
{
F32        *coords;

    getTok();
    getTok();
    getTok();
    coords = getF32s(count);
    getTok();
    return coords;
}

//mm Assumes that the vertex coordinate listing came before the bone listing in the VRML file.  
static void loadBoneWeights(VrmlShape * vrml_shape, int numverts)
{
    char Token[1024];
    char bone[1024];
    float weight;
    int anothervertex;
    int anotherbone;
    int vertexindex;
    int numbones; 
    int position;
    int idx;

    anothervertex = 1;
    vertexindex   = 0;    
    numbones = 0;

    strcpy(Token, getTok()); //get first "["

    while (anothervertex) 
    {
        if(vertexindex > numverts)
            FatalErrorf("There are more bones in this shape than vertices.");
        
        strcpy(Token, getTok()); //get rid of first "("
        if(strcmp(Token, "]")) //if not end of list, process the next vertex
        {
            anotherbone = 1;
            while (anotherbone)
            {
                strcpy(bone, getTok());
                if(strcmp(bone, ")"))
                { 
                    strcpy(Token, getTok());
                    weight = atof(Token);
                    
                    //now I have a bone and a weight and a vertex index.
                    position = -1;
                    for(idx = 0 ; idx < numbones ; idx++)
                    {
                        if(!strcmp(bone, vrml_shape->bonelist[idx]))
                        {
                            position = idx;
                            break;
                        }
                    }
                    if(position == -1)
                    {    
                        vrml_shape->bone_idx[numbones] = malloc(numverts * sizeof(F32));
                        memset(vrml_shape->bone_idx[numbones], 0, numverts * sizeof(F32));
                        
                        strcpy(vrml_shape->bonelist[numbones], bone);
                        
                        position = numbones; 
                        numbones++;
                    }
                    (vrml_shape->bone_idx[position])[vertexindex] = weight;
                }
                else { anotherbone = 0; }
            }
            vertexindex++;
        }
        else{ anothervertex = 0;}
    }

    if (numbones > MAX_OBJBONES)
    {
        FatalErrorf("This shape contains %d bones.  Only %d bones supported.",
                    numbones, MAX_OBJBONES);
    }
    vrml_shape->numbones = numbones;

    if(vertexindex != numverts)
        FatalErrorf("vertex count != boneweight count.  Might fixed by reset xform.");

    //mm Order the bones here.  When I get around to it.  Heirarchical?
    //You now have numbones bone_idx  * [numverts]
    //             numbones bone_list 
}
//end mm*/

static TriIdx *setTriIdxs(TriIdx *tris,int *idxs,int count,int tex_idx,int type,int ccw)
{
int        i,j,idx;

    count /= 4;
    if (!tris)
        tris = calloc(count,sizeof(TriIdx));
    for(i=0;i<count;i++)
    {
        if (idxs[i*4+3] != -1)
        {
            FatalErrorf("ERROR! Non-triangle polys in file! Giving up.\n");
            exit(-1);
        }
        for(j=0;j<3;j++)
        {
            idx = j;
            if (ccw)
                idx = 2-idx;
            if (type == VRML_COORDINDEX)
                tris[i].vert_idxs[idx] = idxs[i*4+j];
            else if (type == VRML_TEXCOORDINDEX)
                tris[i].st_idxs[idx] = idxs[i*4+j];
            else if (type == VRML_NORMALINDEX)
                tris[i].norm_idxs[idx] = idxs[i*4+j]; 
        }
        tris[i].tex_idx = tex_idx;
    }
    return tris;
}

static void getGeometry(VrmlShape *out_shape, char * name) 
{
    int            cmd,count,*idxs,i,ccw=1;
    int            normcount;
    int            normidxcount;
    VrmlShape    vrml_shape;
    char        *tok, *tok2;

    tok = getTok();
    if (stricmp(tok, "DEF")!=0) {
        printf("can only read geometry DEFs, found '%s' instead - skipping (object %s)\n", 
                    tok, ( *name ? name : "name unknown" ));
        skipBlock();
    } else {
        tok2 = getTok();
        cmd = getCmd();
        if (cmd != VRML_INDEXEDFACESET)
        {
            printf("can only read indexedfaceset - skipping (object %s)\n", name);
            skipBlock();
        }
        else
        {
            memset(&vrml_shape,0,sizeof(vrml_shape));
            vrml_shape.tex_idx = out_shape->tex_idx;
            getCmd();
            for(;;)
            {
                cmd = getCmd();
                switch(cmd)
                {
                    case VRML_SOLID:
                        getTok();
                    xcase VRML_CCW:
                        ccw = getCmd() == VRML_TRUE;
                    xcase VRML_CONVEX:
                        getCmd();
                    xcase VRML_COLORPERVERTEX:
                        getCmd();
                    xcase VRML_COLOR:
                        getCoords(&count);
                    xcase VRML_COLORINDEX:
                        getS32s(&count);
                    xcase VRML_COORD:
                        vrml_shape.verts = (void *)getCoords(&count);
                        vrml_shape.vert_count = count/3;
                        for(i=0;i<vrml_shape.vert_count;i++)
                        {
                            orientPos(vrml_shape.verts[i]);
                        }
                    xcase VRML_SKIN:
                        loadBoneWeights(&vrml_shape, vrml_shape.vert_count);
                    xcase VRML_NORMAL: 
                        vrml_shape.norms = (void *)getNormals(&normcount);
                        vrml_shape.norm_count = normcount/3;
                        for(i=0;i<vrml_shape.norm_count;i++)
                        {
                            orientPos(vrml_shape.norms[i]);
                        }
                    xcase VRML_NORMALINDEX:
                        idxs = getS32s(&normidxcount);
                        if(normidxcount/4 == vrml_shape.tri_count) //if it's not screwy, get the normals...
                            vrml_shape.tris = setTriIdxs(vrml_shape.tris,idxs,count,vrml_shape.tex_idx,VRML_NORMALINDEX,ccw);
                        else
                        {
                            printf("Geometry %s normals != tris*3. (%d normals, %d triangles) Skipping...\n", name, normidxcount, vrml_shape.tri_count);
                            free(vrml_shape.norms);
                            vrml_shape.norms = 0;
                            vrml_shape.norm_count = 0;
                        }
                        free(idxs);
                        idxs = 0;
                    xcase VRML_NORMALPERVERTEX:
                        getCmd();
                    xcase VRML_TEXCOORD:
                        vrml_shape.sts = (void *)getCoords(&count);
                        vrml_shape.st_count = count/2;
                    xcase VRML_COORDINDEX:
                        idxs = getS32s(&count);
                        vrml_shape.tris = setTriIdxs(vrml_shape.tris,idxs,count,vrml_shape.tex_idx,VRML_COORDINDEX,ccw);
                        vrml_shape.tri_count = count / 4;
                        assert(vrml_shape.tri_count < 1000000 && vrml_shape.tri_count >= 0);
                        free(idxs);
                    xcase VRML_TEXCOORDINDEX:
                        idxs = getS32s(&count);
                        vrml_shape.tris = setTriIdxs(vrml_shape.tris,idxs,count,vrml_shape.tex_idx,VRML_TEXCOORDINDEX,ccw);
                        free(idxs);
                    xcase VRML_NODEEND:
                        if (!vrml_shape.tris) 
                        {
                            vrml_shape.verts = 0;
                            vrml_shape.vert_count = vrml_shape.tri_count = 0;
                        }
                        simplifyMergeVrmlShapes(&vrml_shape,out_shape); 
                        freeVrmlShape(&vrml_shape);
                        return;
                    xcase 0:
                        printf("Warning: possible bad .wrl file!\n");
                        printf("Press any key to continue...\n");
                        _getch();
                        return;
                }
            }
        }
    }
}

static void getShape(VrmlShape *shape, char * name)
{
    int        cmd;

    cmd = getCmd();
    for(;;)
    {
        char *tok = getTok();
        cmd = cmdOldNum(tok);
        if (cmd == VRML_APPEARANCE)
            getAppearance(shape);
        else if (cmd == VRML_GEOMETRY)
            getGeometry(shape, name); 
        else if (cmd == VRML_NODEEND)
            break;
        else printf("Skipping unknown block: %s (%d)\n", tok, cmd);
    }
}

static void fixName(char *name)
{
char    *s;

    s = strstr(name,"-ROOT");
    if (s)
        *s = 0;
}

static Node *getNodes(Node *node)
{
    int            cmd,i;
    char        name[1000];
    Node        *child=0;
    VrmlShape    vrml_shape = {0};

    static int node_depth; //not used

    name[0] = '\0';

    for(i=0;i<3;i++)
    {
        if (node)
            node->scale[i] = 1;
    }

    for(;;)
    {
        cmd = getCmd();
        if (!cmd)
            return getTreeRoot();

        switch(cmd)
        {
            case VRML_DEF:
            case VRML_USE:
                strcpy_s(name,ARRAY_SIZE(name),getTok());
            xcase VRML_TRANSFORM:
                if (treeFind(name))
                {
                    printf("Skipping duplicate: %s (child of %s)\n",name,node?node->name:"root");
                    skipBlock();
                }
                else
                {
                    //Insert new node and init 
                    child = treeInsert(node);
                    assert(child);
                    ZeroStruct(&child->mesh);
                    strcpy_s(child->name,ARRAY_SIZE(child->name),name);
                    fixName(child->name);
                    child->rotate[0] = 1.0;        //set default rotation
                    child->scale[0] = child->scale[0] = child->scale[0] = 1.0; //set default scale
                    orientAngle(child->rotate);
                }
            xcase VRML_POINTLIGHT:
            case VRML_SPOTLIGHT:
                getLight(name);
            xcase VRML_TRANSLATION:
                getVec3(node->translate);
                orientPos(node->translate);
                copyVec3(node->translate,node->pivot);
            xcase VRML_PIVOT: // Not real VRML, but who cares?
                getVec3(node->pivot);
                orientPos(node->pivot);
            xcase VRML_ROTATION:
                getVec4(node->rotate);
                orientAngle(node->rotate);
            xcase VRML_CENTER:
                getVec3(node->center);
                orientPos(node->center);
            xcase VRML_SCALE:
                getVec3(node->scale);
            xcase VRML_SCALEORIENTATION:
                getVec4(node->scaleOrient);
                orientAngle(node->scaleOrient);
            xcase VRML_CHILDREN:
                getTok();
            xcase VRML_TIMESENSOR:
                skipBlock();
            xcase VRML_POSINTERP:
                if (!node || strstr(name,"-SCALE-"))
                    skipBlock();
                else
                    getAnimKeys(&node->poskeys);
            xcase VRML_ROTINTERP:
                if (!node || strstr(name,"-SCALE-"))
                    skipBlock();
                else
                    getAnimKeys(&node->rotkeys);
            xcase VRML_SHAPE:
                {
                    getShape(&vrml_shape, node->name);
                    convertVrmlShapeToGMesh(&node->bones, &node->mesh, &vrml_shape);
                }
            xcase VRML_NODESTART:
                node_depth++;
                getNodes(child);
            xcase VRML_NODEEND:
                node_depth--;
                freeVrmlShape(&vrml_shape);
                return getTreeRoot();
            break;
        }
    }

    freeVrmlShape(&vrml_shape);
    return getTreeRoot();
}
    

static char *expandVrml()
{
    int        depth,cmd,last_cmd=-1,idx,maxbytes=0,len,blocklen;
    char    *mem=0,*s;
    int        max_mem = 0,i;
    int        *def_idxs = 0;
    int        def_count = 0;
    char    *s2;

    idx = 0;
    for(;;)
    {
        s = getTok();
        // since this routine removes quotes, we need to fixup the words with spaces so the parser won't get confused.
        // fixing up the words will cause errors later on, but they should at least be understandable
        // don't use spaces in your texture names!
        if (s)
        {
            for(s2=s;*s2;s2++)
                if (*s2 == ' ')
                    *s2 = '_';
        }
        cmd = cmdOldNum(s);
        if (!cmd)
        {
            free(def_idxs);
            def_count = 0;
            return mem;
        }
        len = (int)strlen(s);
        if (idx + len + 2 >= maxbytes)
        {
            maxbytes += MEMCHUNK;
            mem = realloc(mem,maxbytes);
        }
        strcpy(mem + idx,s);
        idx += (int)strlen(s);
        mem[idx++] = ' ';
        mem[idx] = 0;
        if (cmd == VRML_DEF)
        {
            def_idxs = realloc(def_idxs,sizeof(def_idxs[0]) * (def_count+1));
            def_idxs[def_count] = idx;
            def_count++;
        }
        if (last_cmd == VRML_USE)
        {
            len = (int)strlen(s);
            for(i=0;i<def_count;i++)
                if (strncmp(mem + def_idxs[i],s,len)==0)
                    break;
            if (i >= def_count)
            {
                FatalErrorf("Unmatched USE: %s\n",s);
            }
            depth = 0;
            for(s = mem + def_idxs[i];;s++)
            {
                if (*s == '[' || *s == '{')
                    depth++;
                if (*s == ']' || *s == '}')
                {
                    depth--;
                    if (!depth)
                        break;
                }
            }
            blocklen = s - (mem + def_idxs[i]) + 1 - len;
            if (idx + blocklen + 2 >= maxbytes)
            {
                maxbytes += blocklen;
                mem = realloc(mem,maxbytes);
            }
            memcpy(mem + idx,mem + def_idxs[i] + len,blocklen);
            idx += blocklen;
            mem[idx] = 0;
        }
        last_cmd = cmd;
    }
}

// Helper function to look for max file reference in vrml text comment.  
//    We do this so that we can make sure the max file is added to perforce.
//    We rely on there being a line as follows in the VRML comment header:
//        # MAX File: AP_CityHall_Makeover_01.max
static void ParseVrmlForMaxFile(const char * vrml_path, const char * vrml_text)
{
    const char * start;
    char buf[256];

    #define MAX_FILE_PREFIX "MAX File: "
    start = strstr(vrml_text, MAX_FILE_PREFIX);
    if(start)
    {
        char buf2[256], *c;

        // advance to start of file name
        start += strlen(MAX_FILE_PREFIX);

        // prep full path of max file (assumed to be in same dir as WRL file)
        strcpy(buf, vrml_path);
        c = strrchr(buf, '\\');
        if(!c) c = strrchr(buf, '/');
        assert(c);
        if(!c) return;
        c++; *c = 0;

        strncpy(buf2, start, sizeof(buf2)); buf2[255] = 0;
        c = strstri(buf2, ".max");
        assert(c);
        if(!c) return;
        c += 4; *c = 0;
        strcat(buf, buf2);
    }
}

/*Only entry point for vrml.c 
Reads in a Vrml file, expands it, and converts it into a tree of Nodes, each
of which corresponds to a "DEF whatever Transform{}, Shape{}" in the VRML file
(Note: the tree and lights are global.) Returns the 
root of the tree.
*/
Node *readVrmlFile(char *name)
{
Node *root;
char *vrml_text,*vrml_exp_text;
int fileLen=0;

    vrml_text = fileAlloc(name, &fileLen);
    if(!vrml_text)
        return 0;

    assert(fileLen != 0);
    setVrmlText(vrml_text);
    ParseVrmlForMaxFile(name, vrml_text);
    vrml_exp_text = expandVrml();
    free(vrml_text);
    setVrmlText(vrml_exp_text);

    treeFree();
    root = getNodes(0);
    free(vrml_exp_text);

    return root;
}

