#include <memory.h>
#include "Cloth/Cloth.h"
#include "Cloth/ClothMesh.h"
#include "Cloth/ClothPrivate.h"

//////////////////////////////////////////////////////////////////////////////
// ClothCreateMeshIndices() builds a list of vertex indices describing a
//   Triangle List or Mesh.
// A single Cloth structure may have multiple ClothMeshes associated with it
//   representing different SubLODs.
// The Cloth and associated ClothMeshes are grouped together in the ClothLOD
//   structure.
// NOTE: ClothMesh->TextureData points to game specific texture information
//   This data is neither allocated nor destroyed by this code.

#define TRIANGLE_LISTS 1

// Creates mesh index data
ClothMesh *ClothCreateMeshIndices(Cloth *cloth, int lod)
{
    int Width = cloth->commonData.Width;
    int Height = cloth->commonData.Height;
    
    int i,j;
    int idx = 0;
    int numtris,numidx;
    int nump1 = Width*Height; // centers (W-1 x H-1)
    int nump2 = nump1 + (Width-1)*(Height-1);     // horiz edge midpoints (W-1 x H)
    int nump3 = nump2 + (Width-1)*(Height);     // vert edge midpoints  (W x H-1)
    int nump4 = nump3 + (Width)*(Height-1);     // horiz edge border points (2 x W-1)
    int nump5 = nump4 + 4*(Width-1);             // vert edge border points    (2 x H-1)
    //int nump6 = nump2 + 4*(Height-1); // end
    
    if (Height < 2 || Width < 2)
        return 0;
    if (lod < cloth->MinSubLOD)
        lod = cloth->MinSubLOD;

    {
    ClothMesh *mesh = ClothMeshCreate();
    int nump = ClothNumRenderedParticles(&cloth->commonData, lod);
    ClothMeshSetPoints(mesh, nump, cloth->renderData.RenderPos, cloth->renderData.Normals, cloth->renderData.TexCoords, cloth->renderData.BiNormals, cloth->renderData.Tangents);
    
    switch(lod)
    {
      case 0:
      {
          // Special case for edges,
#if TRIANGLE_LISTS
          int s = 0;
          ClothMeshCreateStrips(mesh, 1, CLOTHMESH_TRILIST);
          numtris = (Height-1)*(Width-1)*8;
          numtris += 2*(Width-1)*2;
          numtris += 2*(Height-1)*2;
          numidx = numtris * 3;
          idx = 0;
          ClothStripCreateIndices(&mesh->Strips[s], numidx, -1);
          for (i=0; i<Height-1; i++)
          {
              for (j=0; j<Width-1; j++)
              {
                  S16 cidx  = (S16)(nump1 + i*(Width-1) + j);
                  S16 ulidx = (S16)(i*Width + j);
                  S16 uridx = ulidx + 1;
                  S16 llidx = ulidx + Width;
                  S16 lridx = llidx + 1;
                  S16 hmid1 = (S16)(nump2 + i*(Width-1) + j);
                  S16 hmid2 = hmid1 + (Width-1);
                  S16 vmid1 = (S16)(nump3 + i*(Width) + j);
                  S16 vmid2 = vmid1 + 1;
                  // Tri 1
                  mesh->Strips[s].IndicesCCW[idx++] = ulidx;
                  mesh->Strips[s].IndicesCCW[idx++] = cidx;
                  if (i==0) {
                      S16 hmid1a = (S16)(nump4 + j*2 + 0);
                      mesh->Strips[s].IndicesCCW[idx++] = hmid1a;
                      // Tri 1b
                      mesh->Strips[s].IndicesCCW[idx++] = hmid1a;
                      mesh->Strips[s].IndicesCCW[idx++] = cidx;
                      mesh->Strips[s].IndicesCCW[idx++] = hmid1;
                  } else {
                      mesh->Strips[s].IndicesCCW[idx++] = hmid1;
                  }
                  // Tri 2
                  mesh->Strips[s].IndicesCCW[idx++] = hmid1;
                  mesh->Strips[s].IndicesCCW[idx++] = cidx;
                  if (i==0) {
                      S16 hmid1b = (S16)(nump4 + j*2 + 1);
                      mesh->Strips[s].IndicesCCW[idx++] = hmid1b;
                      // Tri 2b
                      mesh->Strips[s].IndicesCCW[idx++] = hmid1b;
                      mesh->Strips[s].IndicesCCW[idx++] = cidx;
                      mesh->Strips[s].IndicesCCW[idx++] = uridx;
                  } else {
                      mesh->Strips[s].IndicesCCW[idx++] = uridx;
                  }
                  
                  // Tri 3
                  mesh->Strips[s].IndicesCCW[idx++] = uridx;
                  mesh->Strips[s].IndicesCCW[idx++] = cidx;
                  if (j==Width-2) {
                      S16 vmid2a = (S16)(nump5 + (Height-1)*2 + i*2 + 0);
                      mesh->Strips[s].IndicesCCW[idx++] = vmid2a;
                      // Tri 1b
                      mesh->Strips[s].IndicesCCW[idx++] = vmid2a;
                      mesh->Strips[s].IndicesCCW[idx++] = cidx;
                      mesh->Strips[s].IndicesCCW[idx++] = vmid2;
                  } else {
                      mesh->Strips[s].IndicesCCW[idx++] = vmid2;
                  }
                  // Tri 4
                  mesh->Strips[s].IndicesCCW[idx++] = vmid2;
                  mesh->Strips[s].IndicesCCW[idx++] = cidx;
                  if (j==Width-2) {
                      S16 vmid2b = (S16)(nump5 + (Height-1)*2 + i*2 + 1);
                      mesh->Strips[s].IndicesCCW[idx++] = vmid2b;
                      // Tri 2b
                      mesh->Strips[s].IndicesCCW[idx++] = vmid2b;
                      mesh->Strips[s].IndicesCCW[idx++] = cidx;
                      mesh->Strips[s].IndicesCCW[idx++] = lridx;
                  } else {
                      mesh->Strips[s].IndicesCCW[idx++] = lridx;
                  }
                  
                  // Tri 5
                  mesh->Strips[s].IndicesCCW[idx++] = lridx;
                  mesh->Strips[s].IndicesCCW[idx++] = cidx;
                  if (i==Height-2) {
                      S16 hmid2b = (S16)(nump4 + (Width-1)*2 + j*2 + 1);
                      mesh->Strips[s].IndicesCCW[idx++] = hmid2b;
                      // Tri 1b
                      mesh->Strips[s].IndicesCCW[idx++] = hmid2b;
                      mesh->Strips[s].IndicesCCW[idx++] = cidx;
                      mesh->Strips[s].IndicesCCW[idx++] = hmid2;
                  } else {
                      mesh->Strips[s].IndicesCCW[idx++] = hmid2;
                  }
                  // Tri 6
                  mesh->Strips[s].IndicesCCW[idx++] = hmid2;
                  mesh->Strips[s].IndicesCCW[idx++] = cidx;
                  if (i==Height-2) {
                      S16 hmid2a = (S16)(nump4 + (Width-1)*2 + j*2 + 0);
                      mesh->Strips[s].IndicesCCW[idx++] = hmid2a;
                      // Tri 2b
                      mesh->Strips[s].IndicesCCW[idx++] = hmid2a;
                      mesh->Strips[s].IndicesCCW[idx++] = cidx;
                      mesh->Strips[s].IndicesCCW[idx++] = llidx;
                  } else {
                      mesh->Strips[s].IndicesCCW[idx++] = llidx;
                  }
                  
                  // Tri 7
                  mesh->Strips[s].IndicesCCW[idx++] = llidx;
                  mesh->Strips[s].IndicesCCW[idx++] = cidx;
                  if (j==0) {
                      S16 vmid1b = (S16)(nump5 + i*2 + 1);
                      mesh->Strips[s].IndicesCCW[idx++] = vmid1b;
                      // Tri 1b
                      mesh->Strips[s].IndicesCCW[idx++] = vmid1b;
                      mesh->Strips[s].IndicesCCW[idx++] = cidx;
                      mesh->Strips[s].IndicesCCW[idx++] = vmid1;
                  } else {
                      mesh->Strips[s].IndicesCCW[idx++] = vmid1;
                  }
                  // Tri 8
                  mesh->Strips[s].IndicesCCW[idx++] = vmid1;
                  mesh->Strips[s].IndicesCCW[idx++] = cidx;
                  if (j==0) {
                      S16 vmid1a = (S16)(nump5 + i*2 + 0);
                      mesh->Strips[s].IndicesCCW[idx++] = vmid1a;
                      // Tri 2b
                      mesh->Strips[s].IndicesCCW[idx++] = vmid1a;
                      mesh->Strips[s].IndicesCCW[idx++] = cidx;
                      mesh->Strips[s].IndicesCCW[idx++] = ulidx;
                  } else {
                      mesh->Strips[s].IndicesCCW[idx++] = ulidx;
                  }
                  
              }
          }
#else
          // One fan per grid (i.e. ~1/particle!)
          ClothMeshCreateStrips(mesh, (Height-1)*(Width-1), CLOTHMESH_TRIFAN);
          for (i=0; i<Height-1; i++)
          {
              for (j=0; j<Width-1; j++)
              {
                  int s = i*(Width-1) + j;
                  idx = 0;
                  numidx = 10;
                  if (i==0 || i==Height-2) numidx += 2;
                  if (j==0 || j==Width-2)  numidx += 2;
                  ClothStripCreateIndices(&mesh->Strips[s], numidx, -1);

                  mesh->Strips[s].IndicesCCW[idx++] = (S16)(nump1 + i*(Width-1) + j); // center
                
                  mesh->Strips[s].IndicesCCW[idx++] = (S16)(i*Width + j); // UL
                  if (j==0)
                      mesh->Strips[s].IndicesCCW[idx++] = (S16)(nump5 + i*2 + 0); // vert border1a
                  mesh->Strips[s].IndicesCCW[idx++] = (S16)(nump3 + i*(Width) + j); // vert mid1
                  if (j==0)
                      mesh->Strips[s].IndicesCCW[idx++] = (S16)(nump5 + i*2 + 1); // vert border1b

                  mesh->Strips[s].IndicesCCW[idx++] = (S16)((i+1)*Width + j); // LL
                  if (i==Height-2)
                      mesh->Strips[s].IndicesCCW[idx++] = (S16)(nump4 + (Width-1)*2 + j*2 + 0); // horiz border2a
                  mesh->Strips[s].IndicesCCW[idx++] = (S16)(nump2 + (i+1)*(Width-1) + j); // horiz mid2
                  if (i==Height-2)
                      mesh->Strips[s].IndicesCCW[idx++] = (S16)(nump4 + (Width-1)*2 + j*2 + 1); // horiz border2b

                  mesh->Strips[s].IndicesCCW[idx++] = (S16)((i+1)*Width + j+1); // LR
                  if (j==Width-2)
                      mesh->Strips[s].IndicesCCW[idx++] = (S16)(nump5 + (Height-1)*2 + i*2 + 1); // vert border2b
                  mesh->Strips[s].IndicesCCW[idx++] = (S16)(nump3 + i*(Width) + j+1); // vert mid2
                  if (j==Width-2)
                      mesh->Strips[s].IndicesCCW[idx++] = (S16)(nump5 + (Height-1)*2 + i*2 + 0); // vert border2a

                  mesh->Strips[s].IndicesCCW[idx++] = (S16)(i*Width + j+1); // UR
                  if (i==0)
                      mesh->Strips[s].IndicesCCW[idx++] = (S16)(nump4 + j*2 + 1); // horiz border1b
                  mesh->Strips[s].IndicesCCW[idx++] = (S16)(nump2 + i*(Width-1) + j); // horiz mid1
                  if (i==0)
                      mesh->Strips[s].IndicesCCW[idx++] = (S16)(nump4 + j*2 + 0); // horiz border1a

                  mesh->Strips[s].IndicesCCW[idx++] = (S16)(i*Width + j); // UL
              }
          }
#endif
      }
      break;
      case 1:
      {
#if TRIANGLE_LISTS
          int s = 0;
          ClothMeshCreateStrips(mesh, 1, CLOTHMESH_TRILIST);
          numtris = (Height-1)*(Width-1)*8;
          numidx = numtris * 3;
          idx = 0;
          ClothStripCreateIndices(&mesh->Strips[s], numidx, -1);
          for (i=0; i<Height-1; i++)
          {
              for (j=0; j<Width-1; j++)
              {
                  S16 cidx  = (S16)(nump1 + i*(Width-1) + j);
                  S16 ulidx = (S16)(i*Width + j);
                  S16 uridx = ulidx + 1;
                  S16 llidx = ulidx + Width;
                  S16 lridx = llidx + 1;
                  S16 hmid1 = (S16)(nump2 + i*(Width-1) + j);
                  S16 hmid2 = hmid1 + (Width-1);
                  S16 vmid1 = (S16)(nump3 + i*(Width) + j);
                  S16 vmid2 = vmid1 + 1;
                  // Tri 1
                  mesh->Strips[s].IndicesCCW[idx++] = ulidx;
                  mesh->Strips[s].IndicesCCW[idx++] = cidx;
                  mesh->Strips[s].IndicesCCW[idx++] = hmid1;
                  // Tri 2
                  mesh->Strips[s].IndicesCCW[idx++] = hmid1;
                  mesh->Strips[s].IndicesCCW[idx++] = cidx;
                  mesh->Strips[s].IndicesCCW[idx++] = uridx;
                  // Tri 3
                  mesh->Strips[s].IndicesCCW[idx++] = uridx;
                  mesh->Strips[s].IndicesCCW[idx++] = cidx;
                  mesh->Strips[s].IndicesCCW[idx++] = vmid2;
                  // Tri 4
                  mesh->Strips[s].IndicesCCW[idx++] = vmid2;
                  mesh->Strips[s].IndicesCCW[idx++] = cidx;
                  mesh->Strips[s].IndicesCCW[idx++] = lridx;
                  // Tri 5
                  mesh->Strips[s].IndicesCCW[idx++] = lridx;
                  mesh->Strips[s].IndicesCCW[idx++] = cidx;
                  mesh->Strips[s].IndicesCCW[idx++] = hmid2;
                  // Tri 6
                  mesh->Strips[s].IndicesCCW[idx++] = hmid2;
                  mesh->Strips[s].IndicesCCW[idx++] = cidx;
                  mesh->Strips[s].IndicesCCW[idx++] = llidx;
                  // Tri 7
                  mesh->Strips[s].IndicesCCW[idx++] = llidx;
                  mesh->Strips[s].IndicesCCW[idx++] = cidx;
                  mesh->Strips[s].IndicesCCW[idx++] = vmid1;
                  // Tri 8
                  mesh->Strips[s].IndicesCCW[idx++] = vmid1;
                  mesh->Strips[s].IndicesCCW[idx++] = cidx;
                  mesh->Strips[s].IndicesCCW[idx++] = ulidx;
              }
          }
#else
          // 8 triangle fan PER GRID
          // Use half-height strips, 2 per row (combined)
          ClothMeshCreateStrips(mesh, (Height-1), CLOTHMESH_TRISTRIP);
          for (i=0; i<Height-1; i++)
          {
              numidx = 2*(Width*4-2) + 2;
              ClothStripCreateIndices(&mesh->Strips[i], numidx, -1);
              idx = 0;
              // top half
              for (j=0; j<Width; j++)
              {
                  mesh->Strips[i].IndicesCCW[idx++] = (S16)(i*Width + j);
                  mesh->Strips[i].IndicesCCW[idx++] = (S16)(nump3 + i*Width + j);
                  if (j < Width-1)
                  {
                      mesh->Strips[i].IndicesCCW[idx++] = (S16)(nump2 + i*(Width-1) + j);
                      mesh->Strips[i].IndicesCCW[idx++] = (S16)(nump1 + i*(Width-1) + j);
                  }
              }
              mesh->Strips[i].IndicesCCW[idx++] = (S16)(nump3 + i*Width + Width-1);
              mesh->Strips[i].IndicesCCW[idx++] = (S16)(nump3 + i*Width + 0);
              // bottom half
              for (j=0; j<Width; j++)
              {
                  mesh->Strips[i].IndicesCCW[idx++] = (S16)(nump3 + i*Width + j);
                  mesh->Strips[i].IndicesCCW[idx++] = (S16)((i+1)*Width + j);
                  if (j < Width-1)
                  {
                      mesh->Strips[i].IndicesCCW[idx++] = (S16)(nump1 + i*(Width-1) + j);
                      mesh->Strips[i].IndicesCCW[idx++] = (S16)(nump2 + (i+1)*(Width-1) + j);
                  }
              }
          }
#endif
      }
      break;
      case 2:
      case 3:
      default:
      {
#if 1 && TRIANGLE_LISTS
          int s = 0;
          ClothMeshCreateStrips(mesh, 1, CLOTHMESH_TRILIST);
          numtris = (Height-1)*(Width-1)*2;
          numidx = numtris * 3;
          idx = 0;
          ClothStripCreateIndices(&mesh->Strips[s], numidx, -1);
          for (i=0; i<Height-1; i++)
          {
              for (j=0; j<Width-1; j++)
              {
                  S16 ulidx = (S16)(i*Width + j);
                  S16 uridx = ulidx + 1;
                  S16 llidx = ulidx + Width;
                  S16 lridx = llidx + 1;
                  // Tri 1
                  mesh->Strips[s].IndicesCCW[idx++] = uridx;
                  mesh->Strips[s].IndicesCCW[idx++] = ulidx;
                  mesh->Strips[s].IndicesCCW[idx++] = lridx;
                  // Tri 2
                  mesh->Strips[s].IndicesCCW[idx++] = lridx;
                  mesh->Strips[s].IndicesCCW[idx++] = ulidx;
                  mesh->Strips[s].IndicesCCW[idx++] = llidx;
              }
          }
#else
          // One strip based on widt/height for lod point set
          numidx = (Height-1)*(Width*2+2)-2;
          ClothMeshCreateStrips(mesh, 1, CLOTHMESH_TRISTRIP);
          ClothStripCreateIndices(&mesh->Strips[0], numidx, -1);
          for (i=0; i<Height-1; i++)
          {
              if (i!=0)
                  mesh->Strips[0].IndicesCCW[idx++] = (S16)(i*Width); // dup first vertex
              for (j=0; j<Width; j++)
              {
                  mesh->Strips[0].IndicesCCW[idx++] = (S16)(i*Width + j);
                  mesh->Strips[0].IndicesCCW[idx++] = (S16)((i+1)*Width + j);
              }
              if (i!=Height-2)
                  mesh->Strips[0].IndicesCCW[idx++] = (S16)((i+2)*Width-1); // dup last vertex
          }
          break;
#endif
      }
    }
    ClothMeshCalcMinMax(mesh);
    return mesh;
    }
}

//////////////////////////////////////////////////////////////////////////////

void ClothStripCreateIndices(ClothStrip *strip, int num, int newtype)
{
    if (newtype >= 0)
        strip->Type = newtype;
    strip->NumIndices = num;
    strip->IndicesCCW = CLOTH_CALLOC(S16, num);
}

//////////////////////////////////////////////////////////////////////////////

ClothMesh *ClothMeshCreate()
{
    ClothMesh *mesh = CLOTH_MALLOC(ClothMesh, 1);
    memset(mesh, 0, sizeof(*mesh));

    // Rendering
    setVec3(mesh->Color,1.f,1.f,1.f);
    mesh->Alpha = 1.0f;

    return mesh;
}

void ClothMeshDelete(ClothMesh *mesh)
{
    int i;
    for (i=0; i<mesh->NumStrips; i++)
    {
        ClothStrip *strip = mesh->Strips + i;
        CLOTH_FREE(strip->IndicesCCW);
    }
    CLOTH_FREE(mesh->Strips);
    if (mesh->Allocate)
    {
        CLOTH_FREE(mesh->Points);
        CLOTH_FREE(mesh->Normals);
        CLOTH_FREE(mesh->BiNormals);
        CLOTH_FREE(mesh->Tangents);
        CLOTH_FREE(mesh->TexCoords);
    }
    CLOTH_FREE(mesh);
}

void ClothMeshAllocate(ClothMesh *mesh, int npoints)
{
    mesh->NumPoints = npoints;
    mesh->Allocate = 1;
    mesh->Points = CLOTH_CALLOC(Vec3, npoints);
    mesh->Normals = CLOTH_CALLOC(Vec3, npoints);
    mesh->BiNormals = CLOTH_CALLOC(Vec3, npoints);
    mesh->Tangents = CLOTH_CALLOC(Vec3, npoints);
    mesh->TexCoords = CLOTH_CALLOC(Vec2, npoints);
    assert(mesh->Points);
    assert(mesh->Normals);
    assert(mesh->BiNormals);
    assert(mesh->Tangents);
    assert(mesh->TexCoords);
}

//////////////////////////////////////////////////////////////////////////////

void ClothMeshSetColorAlpha(ClothMesh *mesh, Vec3 c, F32 a)
{
    if (c)
        copyVec3(c, mesh->Color);
    if (a >= 0.0f)
        mesh->Alpha = a;
}

void ClothMeshSetPoints(ClothMesh *mesh, int npoints, Vec3 *pts, Vec3 *norms, Vec2 *tcoords, Vec3 *binorms, Vec3 *tangents)
{
    mesh->NumPoints = npoints;
    mesh->Points = pts;
    if (norms)
        mesh->Normals = norms;
    if (tcoords)
        mesh->TexCoords = tcoords;
    if (binorms)
        mesh->BiNormals = binorms;
    if (tangents)
        mesh->Tangents = tangents;
}

void ClothMeshCreateStrips(ClothMesh *mesh, int num, int type)
{
    int i;
    mesh->NumStrips = num;
    mesh->Strips = CLOTH_MALLOC(ClothStrip, num);
    memset(mesh->Strips, 0, num*sizeof(ClothStrip));
    for (i=0; i<num; i++)
        mesh->Strips[i].Type = type;
}

void ClothMeshCalcMinMax(ClothMesh *mesh)
{
    int i,j;
    ClothStrip *Strips = mesh->Strips;
    for (i=0; i<mesh->NumStrips; i++)
    {
        Strips[i].MinIndex = Strips[i].IndicesCCW[0];
        Strips[i].MaxIndex = Strips[i].IndicesCCW[0];
        for (j=1; j<Strips[i].NumIndices; j++)
        {
            if (Strips[i].MinIndex > Strips[i].IndicesCCW[j])
                Strips[i].MinIndex = Strips[i].IndicesCCW[j];
            if (Strips[i].MaxIndex < Strips[i].IndicesCCW[j])
                Strips[i].MaxIndex = Strips[i].IndicesCCW[j];
        }
    }
}

////////////////////////////////////////


