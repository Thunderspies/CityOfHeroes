#include <stdlib.h>
#include "Cloth/Cloth.h"
#include "Cloth/ClothPrivate.h"
#include "cmdparse/cmdgame.h"

//////////////////////////////////////////////////////////////////////////////
// This module contains utility functions for creating Cloth structure
// NOTE: Currently, all "Cloth" structures are grid based in nature.
// This is not strictly necessary, but is a convenient assumption for
// writing the code. A significant rewrite would be required to change this.
//
// This module also has code for attaching a "harness" to a Cloth
//
//////////////////////////////////////////////////////////////////////////////

//============================================================================
// GENERAL PURPOSE BUILD FUNCTIONS
//============================================================================

//////////////////////////////////////////////////////////////////////////////
// Generates a cloth from an array of positions and an array of masses/
// Input: A two dimentional array of points with optional masses
void ClothSetPointsMasses(Cloth *cloth, Vec3 *points, F32 *masses)
{
    int p;
    int npoints = cloth->commonData.NumParticles;
    for (p=0; p<npoints; p++)
    {
        if (points)
        {
            copyVec3(points[p],cloth->OrigPos[p]);
            copyVec3(points[p],cloth->OldPos[p]);
            copyVec3(points[p],cloth->CurPos[p]);
        }
        if (masses)
            cloth->InvMasses[p] = masses[p] > 0.0f ? 1.0f / masses[p] : 0.0f; // Use 0 for infinite mass (fixed)
    }
}

//////////////////////////////////////////////////////////////////////////////
// Generates a trapezoid shaped cloth
// If circle=1 then the bottom edge of the cloths forms an arc instead of a line
void ClothBuildGrid(Cloth *cloth, int width, int height, Vec3 dxyz, Vec3 origin, F32 xscale, int circle)
{
    int i,j;
    F32 yzlen;    

    if (!ClothCreateParticles(cloth, width, height))
    {
        ClothErrorf("Unable to create cloth particles");
        return;
    }
    
    yzlen = sqrtf(dxyz[1]*dxyz[1] + dxyz[2]*dxyz[2]);    
    for (i=0; i<height; i++)
    {
        F32 xs = 1.0f + ((F32)i/(F32)(height-1))*(xscale - 1.0f);
        for (j=0; j<width; j++)
        {
            int p = i*width + j;
            Vec3 pt,off;
            if (dxyz[0] == 0.0f) {
                setVec3(off, 0.0f, j*dxyz[1], i*dxyz[2]);
                addVec3(origin,off,pt);
                pt[1] *= xs;
            } else {
                setVec3(off,j*dxyz[0], i*dxyz[1], i*dxyz[2]);
                addVec3(origin,off,pt);
                pt[0] *= xs;
            }
            if (circle && i > 0)
            {
                Vec3 pt0,dvec;
                copyVec3(cloth->OrigPos[p-width], pt0);
                subVec3(pt,pt0,dvec);
                normalVec3(dvec);
                scaleAddVec3(dvec, yzlen, pt0, pt);
            }
            copyVec3(pt, cloth->OrigPos[p]);
            copyVec3(pt, cloth->OldPos[p]);
            copyVec3(pt, cloth->CurPos[p]);
            cloth->InvMasses[p] = 1.0f;
        }
    }
}

//============================================================================
// MESH CONVERSION FUNCTIONS
//============================================================================

//////////////////////////////////////////////////////////////////////////////
// Utility functions to turn an arbitrary triangle mesh into a regular grid

typedef struct {
    Vec3 pos;
    int nconnect;
    int connect[6];
    // "diagonal" connections
    int nconnect2;
    int connect2[6];
} ConnectInfo;

static int connect_idx(ConnectInfo *cinfo, int idx2)
{
    int i;
    for (i=0; i<cinfo->nconnect; i++)
    {
        if (cinfo->connect[i] == idx2)
            return i;
    }
    return -1;
}

#define is_connected(a,b) (connect_idx(a,b)>=0)

static int add_connection(ConnectInfo *cinfo, int idx2)
{
    if (!is_connected(cinfo, idx2))
    {
        if (cinfo->nconnect>=6)
            return 0;
        cinfo->connect[cinfo->nconnect] = idx2;
        cinfo->nconnect++;
    }
    return 1;
}

static int del_connection(ConnectInfo *cinfo, int idx2)
{
    int i;
    int idx = connect_idx(cinfo, idx2);
    if (idx < 0)
        return 0; // error
    for (i=idx; i<cinfo->nconnect-1; i++)
        cinfo->connect[i] = cinfo->connect[i+1];
    cinfo->nconnect--;
    return 1;
}

static int add_connections(ConnectInfo *connections, int idx1, int idx2)
{
    if (idx1 == idx2)
        return 1;
    if (!add_connection(connections + idx1, idx2))
        return 0;
    if (!add_connection(connections + idx2, idx1))
        return 0;
    return 1;
}

static int del_connections(ConnectInfo *connections, int idx1, int idx2)
{
    if (idx1 == idx2)
        return 1;
    if (!del_connection(connections + idx1, idx2))
        return 0;
    if (!del_connection(connections + idx2, idx1))
        return 0;
    return 1;
}

static int calc_diagonals(ConnectInfo *connections, int idx1)
{
    ConnectInfo *cinfo1 = connections + idx1;
    int i,j,k;
    cinfo1->nconnect2 = 0;
    for (i=0; i<cinfo1->nconnect; i++)
    {
        int idx2 = cinfo1->connect[i];
        ConnectInfo *cinfo2 = connections + idx2;
        int nc2 = 0;
        for (j=0; j<cinfo2->nconnect; j++)
        {
            int idx3 = cinfo2->connect[j];
            if (idx3==idx1)
                continue;
            if (is_connected(cinfo1, idx3))
                nc2++;
        }
        if (nc2 >= 2)
        {
            if (cinfo1->nconnect2>=6)
                return 0;
            cinfo1->connect2[cinfo1->nconnect2] = idx2;
            cinfo1->nconnect2++;
        }
    }
    return 1;
}

static int remove_diagonal(ConnectInfo *connections, int idx1)
{
    int i,idx2,c2idx;
    ConnectInfo *cinfo1,*cinfo2;

    // Remove only entry from cinfo1
    cinfo1 = connections + idx1;
    if (cinfo1->nconnect2 != 1)
        return 0; // error
    idx2 = cinfo1->connect2[0];
    cinfo1->nconnect2 = 0;
    if (!del_connection(cinfo1, idx2))
        return 0; // error

    // Recalculate diagonals for idx2
    cinfo2 = connections + idx2;
    if (!del_connection(cinfo2, idx1))
        return 0; // error
    if (!calc_diagonals(connections, idx2))
        return 0; // error

    // Recalculate diagonals for neighbors
    for (i=0; i<cinfo1->nconnect; i++)
    {
        if (!calc_diagonals(connections, cinfo1->connect[i]))
            return 0; // error
    }
    for (i=0; i<cinfo2->nconnect; i++)
    {
        if (!is_connected(cinfo1, cinfo2->connect[i]))
        {
            if (!calc_diagonals(connections, cinfo2->connect[i]))
                return 0; // error
        }
    }
    return 1;
}

static int remove_diagonals(ConnectInfo *connections, int npoints)
{
    int i,j;

    // Calculate diagonals
    for (i=0; i<npoints; i++)
        calc_diagonals(connections, i);
    // Traverse the connections, removing diagonals from points
    // with only one diagonal connection. Stop when there are no entries
    // with one diagonal. If any points have > 0 diagonals we failed.
    while(1)
    {
        int changed = 0;
        int connect2 = 0;
        for (i=0; i<npoints; i++)
        {
            ConnectInfo *cinfo = connections + i;
            if (cinfo->nconnect2 == 1)
            {
                if (!remove_diagonal(connections, i))
                    return 0; // error
                changed++;
            }
            else if (cinfo->nconnect2 > 0)
            {
                connect2++;
            }
        }
        if (!changed)
        {
            if (connect2)
                return 0;
            break;
        }
    }
    return 1;
};

// yz = 1 (Y) or 2 (Z)
static int left_max_yz(ConnectInfo *connections, int idx, int yz)
{
    int i;
    int maxyzidx = -1;
    F32 maxyz;
    ConnectInfo *cinfo = connections + idx;
    
    // Find the connected point left (smaller X) of this point
    //   with the max Y/Z value
    for(i=0; i<cinfo->nconnect; i++)
    {
        ConnectInfo *cinfo2 = connections + cinfo->connect[i];
        if (cinfo2->pos[0] < cinfo->pos[0])
        {
            if (maxyzidx < 0 || cinfo2->pos[yz] > maxyz)
            {
                maxyzidx = cinfo->connect[i];
                maxyz = cinfo2->pos[yz];
            }
        }
    }
    return maxyzidx;
}

// yz = 1 (Y) or 2 (Z)
static int right_max_yz(ConnectInfo *connections, int idx, int yz)
{
    int i;
    int maxyzidx = -1;
    F32 maxyz;
    ConnectInfo *cinfo = connections + idx;
    
    // Find the connected point right (larger X) of this point
    //   with the max Y/Z value
    for(i=0; i<cinfo->nconnect; i++)
    {
        ConnectInfo *cinfo2 = connections + cinfo->connect[i];
        if (cinfo2->pos[0] > cinfo->pos[0])
        {
            if (maxyzidx < 0 || cinfo2->pos[yz] > maxyz)
            {
                maxyzidx = cinfo->connect[i];
                maxyz = cinfo2->pos[yz];
            }
        }
    }
    return maxyzidx;
}

// returns width, -1 if error
static int order_grid(ConnectInfo *connections, int npoints, int *neworder)
{
    Vec3 min,max,delta;
    int i, first, yz;
    int corneridx=0, corners[4];
    int width, newidx;

    // Find top-left corner
    /// Find top point
    copyVec3(connections[0].pos, min);
    copyVec3(connections[0].pos, max);
    for (i=0; i<npoints; i++)
    {
        if (connections[i].nconnect == 2)
        {
            if (corneridx >= 4)
                return -1; // error
            corners[corneridx++] = i;
        }
        MINVEC3(min, connections[i].pos, min);
        MAXVEC3(max, connections[i].pos, max);
    }
    subVec3(max, min, delta);
    if (delta[0] <= 0.001f) {
        ClothErrorf("Cloth lays on the the Y-Z plane, it must lay on either X-Y or X-Z");
        return -1; // Y-Z plane = error
    }

    /// Chooze X-Y or X-Z plane
    if (delta[1] >= delta[2])
        yz = 1;  // X-Y plane
    else
        yz = 2;  // X-Z plane

    ///  find top two corners
    if (connections[corners[2]].pos[yz] > connections[corners[0]].pos[yz] ||
        connections[corners[2]].pos[yz] > connections[corners[1]].pos[yz])
    {
        if (connections[corners[0]].pos[yz] > connections[corners[1]].pos[yz])
            corners[1] = corners[2];
        else
            corners[0] = corners[2];
    }
    if (connections[corners[3]].pos[yz] > connections[corners[0]].pos[yz] ||
        connections[corners[3]].pos[yz] > connections[corners[1]].pos[yz])
    {
        if (connections[corners[0]].pos[yz] > connections[corners[1]].pos[yz])
            corners[1] = corners[3];
        else
            corners[0] = corners[3];
    }
    /// use the left-most of the top two corners
    if (connections[corners[0]].pos[0] <= connections[corners[1]].pos[0])
        first = corners[0];
    else
        first = corners[1];
        
    // Starting with first, traverse the grid, filling in neworder
    // Destroy connections as we traverse to make finding the next
    // logical connection easier.
    width = 0;
    newidx = 0;
    while(1)
    {
        int w = 1;
        int next,pnext,pfirst;
        ConnectInfo *cinfo = connections + first;
        next = right_max_yz(connections, first, yz);
        neworder[newidx++] = first;
        if (next >= 0)
            if (!del_connections(connections, first, next))
            return -1; // error
        while(next >= 0)
        {
            pnext = next;
            w++;
            neworder[newidx++] = next;
            cinfo = connections + next;
            if ((w == width) || (!width && cinfo->nconnect == 1))
            {
                // hit corner, done with row
                if (cinfo->nconnect > 1)
                    return -1;
                if (cinfo->nconnect == 1)
                    if (!del_connections(connections, pnext, cinfo->connect[0]))
                        return -1; // err
                break;
            }
            else
            {
                next = right_max_yz(connections, next, yz);
                if (next >= 0)
                    if (!del_connections(connections, pnext, next))
                        return -1; // err
                if (cinfo->nconnect > 1)
                    return -1;
                if (cinfo->nconnect == 1)
                    if (!del_connections(connections, pnext, cinfo->connect[0]))
                        return -1; // err
            }
        }
        if (width == 0)
            width = w;
        else if (w != width)
            return -1; // error
        cinfo = connections + first;
        if (cinfo->nconnect == 0)
            break; // done
        if (cinfo->nconnect > 1)
            return -1; // error
        pfirst = first;
        first = cinfo->connect[0];
        if (!del_connections(connections, pfirst, first))
            return -1; // err
    }
    return width;
}

//////////////////////////////////////////////////////////////////////////////
// ClothBuildGridFromTriList()
//   This funcion only works if the input is a list of triangles that form a
//   rectangular grid.
//   The order of the points and the slope of the diagonals does not matter.
//   The points should be (at least mostly) coplaner in either the X-Y or X-Z plane.

int ClothBuildGridFromTriList(Cloth *cloth,
                              int npoints, Vec3 *points, Vec2 *texcoords,
                              F32 *masses, F32 mass_y_scale, F32 stiffness,
                              int ntris, int *tris,
                              Vec3 scale)
{
    int i;
    int first,res,width,height;
    int *neworder;
    Vec3 *newpoints;
    F32 *newmasses;
    
    ConnectInfo *connections = CLOTH_CALLOC(ConnectInfo, npoints);

    // Create Connection Info
    for (i=0; i<npoints; i++)
    {
        mulVecVec3(points[i], scale, connections[i].pos);
        connections[i].nconnect = 0;
    }
    for (i=0; i<ntris*3; i+=3)
    {
        int err = 0;
        if (!add_connections(connections, tris[i], tris[i+1]))
            err++;
        if (!add_connections(connections, tris[i+1], tris[i+2]))
            err++;
        if (!add_connections(connections, tris[i+2], tris[i]))
            err++;
        if (err)
        {
            CLOTH_FREE(connections);
            return ClothErrorf("ClothBuildGridFromTriList: > 6 connections");
        }
    }
    // Remove diagonal connections
    res = remove_diagonals(connections, npoints);
    if (!res)
    {
        CLOTH_FREE(connections);
        return ClothErrorf("ClothBuildGridFromTriList: Error removing diagonals");
    }
        
    // Order grid
    neworder = CLOTH_CALLOC(int, npoints);

    width = order_grid(connections, npoints, neworder);
    CLOTH_FREE(connections);
    if (width < 0)
    {
        CLOTH_FREE(neworder);
        return ClothErrorf("ClothBuildGridFromTriList: Can not order grid");
    }

    if (masses[neworder[0]]!=0 && masses[neworder[npoints-1]]==0) {
        // HACK!  Fixes the symptom not the problem!
        // For some reason it's upside down (eyelets on the bottom)
        // Flip!
        for (i=0; i<npoints/2; i++) {
            int t = neworder[i];
            neworder[i] = neworder[npoints-1-i];
            neworder[npoints-1-i] = t;
        }
    }

    newpoints = CLOTH_MALLOC(Vec3, npoints);
    newmasses = CLOTH_MALLOC(F32, npoints);
    height = npoints/width;

    if (game_state.clothDebug)
        printf("Cloth masses:\n");
    {
        F32 inv_width = 1.0f / (F32)width;
        F32 inv_height = 1.0f / (F32)(height-1);
        for (i=0; i<npoints; i++)
        {
            if (game_state.clothDebug)
                if ((i % width) == 0)
                    printf("\t");
            mulVecVec3(points[neworder[i]], scale, newpoints[i]);
            if (masses)
            {
                newmasses[i] = masses[neworder[i]];
            }
            else
            {
                // Debug/no masses specified
                F32 dh0 = (F32)((int)((F32)i*inv_width))*inv_height;
                F32 mass_scale = 1.0f + dh0*(mass_y_scale - 1.f);
                if (i<width)
                    newmasses[i] = 0.0f;
                else
                    newmasses[i] = 1.0f * mass_scale;
            }
            if (game_state.clothDebug)
                printf("%4.2f ", newmasses[i]);
            if (game_state.clothDebug)
                if ((i % width) == width-1)
                    printf("\n");
        }
    }
    
    // Create Particles
    if (!ClothCreateParticles(cloth, width, height))
    {
        CLOTH_FREE(newpoints);
        CLOTH_FREE(newmasses);
        CLOTH_FREE(neworder);
        return ClothErrorf("Unable to create cloth particles");
    }

    // Copy texture coordinates
    if (game_state.clothDebug)
        printf("Cloth TextureCoordinates:\n");
    for (i=0; i<npoints; i++)
    {
        if (game_state.clothDebug)
            if ((i % width) == 0)
                printf("\t");
        copyVec2(texcoords[neworder[i]], cloth->renderData.TexCoords[i]);
        if (game_state.clothDebug)
            printf("%4.2f,%4.2f ", cloth->renderData.TexCoords[i][0], cloth->renderData.TexCoords[i][1]);
        if (game_state.clothDebug)
            if ((i % width) == width-1)
                printf("\n");
    }

    // Copy points and masses
    ClothSetPointsMasses(cloth, newpoints, newmasses);

    CLOTH_FREE(newpoints);
    CLOTH_FREE(newmasses);
    CLOTH_FREE(neworder);
        
    // constraints
    {
        int clothflags = CLOTH_FLAGS_CONNECTIONS(2);
        F32 constraint_scale[1];
        constraint_scale[0] = stiffness; // only one secondary constraint used
        ClothCalcLengthConstraints(cloth, clothflags, 1, constraint_scale);
    }
    
    // Calculate stuff
    ClothCalcConstants(cloth, 0);
    ClothCopyToRenderData(cloth, NULL, CCTR_COPY_ALL);
    ClothUpdateNormals(&cloth->renderData);

    return 0; // no error
}

//////////////////////////////////////////////////////////////////////////////
// ClothBuildLOD() creates a new Cloth with half the height and width of the original.
// 'newcloth' must already be created but must be empty (using ClothCreateEmpty())
// NOTE: The best input is a 9x9 as that will scale nicely to a 5x5 and a 3x3
//   Other inputs will work, but the "Eyelets" will not match exactly to hooks
int ClothBuildLOD(Cloth *newcloth, Cloth *incloth, int lod, F32 point_y_scale, F32 stiffness)
{
    int width,height;
    int i,p;

    if (!lod)
        return 1; // can't build lod
    width = incloth->commonData.Width;
    height = incloth->commonData.Height;
    for (i=0; i<lod; i++)
    {
        width = (width + 1)/2;
        height = (height + 1)/2;
    }
    if (width < 2 && height < 2)
        return 1; // can't build lod
    width = MAX(width, 2);
    height = MAX(height, 2);
    
    if (!ClothCreateParticles(newcloth, width, height))
        return ClothErrorf("Unable to create cloth particles");

    ClothBuildCopy(newcloth, incloth, point_y_scale);

    // constraints
    {
        int clothflags = CLOTH_FLAGS_CONNECTIONS(2);
        F32 constraint_scale[1];
        constraint_scale[0] = stiffness; // only one secondary constraint used
        ClothCalcLengthConstraints(newcloth, clothflags, 1, constraint_scale);
    }

    ClothCalcConstants(newcloth, 0);
    ClothCopyToRenderData(newcloth, NULL, CCTR_COPY_ALL);
    ClothUpdateNormals(&newcloth->renderData);
    ClothUpdateIntermediatePoints(&newcloth->renderData);
    
    return 0; // no error
}

//////////////////////////////////////////////////////////////////////////////
// ClothBuildAttachHarness() takes as input an unordered array of Hooks
// and associates each hook with the appropriate eyelet using the X
// value of Hooks and grid order of eyelets to determine association.
// NOTE: If huuknum > N, only N hooks are considered (N = number of eyelets)
// NOTE: This ONLY works if the elyelets are all in the same row.
//       Otherwise a more complicated association will be required.

typedef struct {
    int idx;
    Vec3 pos;
} HookInfo;

int compare_hookinfo(const void *va, const void *vb)
{
    HookInfo *a = (HookInfo*)va;
    HookInfo *b = (HookInfo*)vb;
    F32 dx = a->pos[0] - b->pos[0];
    if (dx < 0) return -1;
    else if (dx == 0) return 0;
    else return 1;
}

int ClothBuildAttachHarness(Cloth *cloth, int hooknum, Vec3 *hooks)
{
    int i,h;
    int hookidx = 0;
    int eyenum = 0;
    HookInfo *hooklist;

    //////////////////////////////////////////////////
    // Hack for Cryptic harness output
    for (i=0; i<cloth->commonData.NumParticles; i++)
        if (cloth->InvMasses[i] == 0.0f)
            eyenum++;
    //////////////////////////////////////////////////
    
    hooklist = CLOTH_CALLOC(HookInfo, hooknum);
    for (i=0, h=0; h<eyenum && i<hooknum; i++)
    {
        int j;
        bool foundit=false;
        // Look for same point already in list
        for (j=0; j<h; j++) 
            if (sameVec3(hooks[i], hooklist[j].pos)) {
                foundit=true;
                break;
            }
        if (foundit)
            continue;
        // New, unique point
        hooklist[h].idx = i;
        copyVec3(hooks[i], hooklist[h].pos);
        h++;
    }
    if (game_state.clothDebug) {
        printf("Found %d hooks on harness, %d eyelets on cape\n", h, eyenum);
    }
    if (h < eyenum)
        return ClothErrorf("ClothBuildAttachHarness: too few unique hooks on harness (%d hooks on harness, %d eyelets on cape)", h, eyenum); // Same as too many eyelets
    hooknum = h;

    if (hooknum > eyenum)
        hooknum = eyenum; // skip extra hooks
    qsort(hooklist, hooknum, sizeof(HookInfo), compare_hookinfo);

    for (i=0; i<cloth->commonData.NumParticles; i++)
    {        
        if (cloth->InvMasses[i] == 0.0f)
        {
            if (hookidx >= hooknum)
                return ClothErrorf("ClothBuildAttachHarness: too many eyelets (%d hooks on harness, %d eyelets on cape)", h, eyenum);
            h = hooklist[hookidx].idx;
            ClothAddAttachment(cloth, i, h, h, 1.0f);
            hookidx++;
        }
    }
    CLOTH_FREE(hooklist);
    return 0; // no error
}

//////////////////////////////////////////////////////////////////////////////

