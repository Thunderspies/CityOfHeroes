#include "edit/MapGenerator.h"
#include "stdio.h"
#include <utilitieslib/components/Earray.h>
#include "string.h"
#include "edit/edit_select.h"
#include "seq/anim.h"
#include <utilitieslib/utils/mathutil.h>
#include "graphics/groupdraw.h"
#include "seq/anim.h"
#include <utilitieslib/utils/utils.h>
#include "group/groupfilelib.h"
#include "edit/edit_net.h"
#include "cmdparse/cmdgame.h"
#include "menu.h"
#include "win/win_init.h"
#include "edit/edit_cmd_file.h"

// the zlib version of fscanf is not working, so this file specifically does not include file.h
#undef fopen

MapPiece * loadPiece(FILE * fin) {
    MapPiece * mp=malloc(sizeof(MapPiece));
    char token[64];
    memset(mp,0,sizeof(MapPiece));
    fscanf(fin,"%s",mp->path);
    strcpy(mp->name,strrchr(mp->path,'/')+1);
    fscanf(fin,"%d",&mp->portals);
    fscanf(fin,"%s",token);
    if (stricmp(token,"start")==0)
        mp->type=MG_Start;
    else if (stricmp(token,"end")==0)
        mp->type=MG_End;
    else if (stricmp(token,"hall")==0)
        mp->type=MG_Hall;
    else if (stricmp(token,"room")==0)
        mp->type=MG_Room;
    else if (stricmp(token,"door")==0)
        mp->type=MG_Door;
    else if (stricmp(token,"elevator")==0)
        mp->type=MG_Elevator;
    fscanf(fin,"%s",token);
    if (stricmp(token,"small")==0)
        mp->size=MG_Small;
    else if (stricmp(token,"medium")==0)
        mp->size=MG_Medium;
    else if (stricmp(token,"large")==0)
        mp->size=MG_Large;
    return mp;
}

MapSetGenerator * loadGenerator(FILE * fin) {
    MapSetGenerator * msg=malloc(sizeof(MapSetGenerator));
    char token[64];
    memset(msg,0,sizeof(MapSetGenerator));
    fscanf(fin,"%s",msg->mapSet);
    fscanf(fin,"%s",token);
    while (stricmp(token,"piece")==0) {
        MapPiece * mp=loadPiece(fin);
        eaPush(&msg->pieces,mp);
        fscanf(fin,"%s",token);
    }
    return msg;
}

void initGenerators() {
    FILE * fin=fopen("C:\\game\\data\\Defs\\MapGeneration.def","r");
    char token[64];
    fscanf(fin,"%s",token);
    while (stricmp(token,"begin")==0) {
        MapSetGenerator * msg=loadGenerator(fin);
        eaPush(&g_generators,msg);
        if (fscanf(fin,"%s",token)==EOF)
            break;
    }
}

MissionSetRules * loadRuleSet(FILE * fin) {
    MissionSetRules * msr=calloc(1, sizeof(MissionSetRules));
    char token[128];
    fscanf(fin,"%s",msr->missionSet);
    fscanf(fin,"%s",token);
    msr->minArea=0;
    msr->maxArea=1000;
    msr->branchFactor=.5;
    msr->minHalls=0;
    msr->maxHalls=100;
    msr->mapSet[0]=0;
    msr->roomsPerFloor=0;
    msr->ambientSound[0]=0;
    msr->sceneFile[0]=0;
    while (stricmp(token,"end")!=0) {
        if (stricmp(token,"minarea")==0)
            fscanf(fin,"%f",&msr->minArea);
        if (stricmp(token,"maxarea")==0)
            fscanf(fin,"%f",&msr->maxArea);
        if (stricmp(token,"branch")==0)
            fscanf(fin,"%d",&msr->branchFactor);
        if (stricmp(token,"minhalls")==0)
            fscanf(fin,"%d",&msr->minHalls);
        if (stricmp(token,"maxhalls")==0)
            fscanf(fin,"%d",&msr->maxHalls);
        if (stricmp(token,"mapset")==0)
            fscanf(fin,"%s",msr->mapSet);
        if (stricmp(token,"maxDepth")==0)
            fscanf(fin,"%d",&msr->maxDepth);
        if (stricmp(token,"minDepth")==0)
            fscanf(fin,"%d",&msr->minDepth);
        if (stricmp(token,"objectivedepth")==0)
            fscanf(fin,"%d",&msr->minObjectiveDepth);
        if (stricmp(token,"objectives")==0)
            fscanf(fin,"%d",&msr->numObjectives);
        if (stricmp(token,"rooms")==0)
            fscanf(fin,"%d",&msr->numRooms);
        if (stricmp(token,"roomsperfloor")==0)
            fscanf(fin,"%d",&msr->roomsPerFloor);
        if (stricmp(token,"scenefile")==0)
            fscanf(fin,"%s",msr->sceneFile);
        if (stricmp(token,"scenefilevillains")==0)
            fscanf(fin,"%s",msr->villainsSceneFile);
        if (stricmp(token,"ambientsound")==0)
            fscanf(fin,"%s",msr->ambientSound);
        fscanf(fin,"%s",token);
    }
    return msr;
}

void loadRules() {
    FILE * fin=fopen("C:\\game\\data\\Defs\\MapGeneration.rules","r");
    char token[128];
    int i,j;
    fscanf(fin,"%s",token);
    eaDestroyEx(&g_rules,NULL);
    while (stricmp(token,"begin")==0) {
        MissionSetRules * msr=loadRuleSet(fin);
        eaPush(&g_rules,msr);
        if (fscanf(fin,"%s",token)==EOF)
            break;
    }
    fclose(fin);
    for (i=0;i<eaSize(&g_rules);i++) {
        for (j=0;j<eaSize(&g_generators);j++) {
            if (strcmp(g_generators[j]->mapSet,g_rules[i]->mapSet)==0) {
                g_rules[i]->msg=g_generators[i];
                break;
            }
        }
    }
}

void initGenerators2() {
    //objectLibraryCount
    int count=objectLibraryCount();
    int i;
    g_generators=0;    //should have some sort of destroy function here
    for (i=0;i<count;i++)
    {
        MapSetGenerator* msg = NULL;
        char path[512];
        strcpy(path,objectLibraryPathFromIdx(i));
        strcpy(strrchr(path,'/')+1,objectLibraryNameFromIdx(i));
        if (objectLibraryNameFromIdx(i)[0]==0 || objectLibraryNameFromIdx(i)[0]=='_')
            continue;
        if (strstri(path,"unique"))
            continue;
        if (strstri(path,"generic"))
            continue;
        if (strstri(path,"_spcl_"))
            continue;
        if (strstri(path,"stc_"))    //these are the antiquated pieces with no spawns
            continue;
        if (strstri(path,"door") && !strstri(path,"frame"))    //don't take locked or closed doors quite yet
            continue;
        if (strstri(path,"swr_elevator"))
            continue;
        if (strstri(path,"cav_h_bnd_02"))
            continue;
        if (strstri(path,"cav_lr_str_02"))
            continue;
        if (strstri(path,"ofc1_sr_bnd_01"))
            continue;
        if (strstri(path,"_tyrant_"))
            continue;
        //not looking for the whole Villain_Lairs string because they misspelled Villians!!!
        if (strstr(path,"_Lairs")!=NULL && (strstr(path,"trays")!=NULL || strstr(path,"anoid_lrg_caves")!=NULL || strstr(path,"arach_lrg_caves")!=NULL) &&
           (strstr(path,"Arachnos/trays")==NULL || strstr(path,"Arach_")!=NULL)) {
            MapPiece * mp=malloc(sizeof(MapPiece));
            char mapset[256];
            int j;
            strcpy(mapset,strstr(path,"_Lairs")+7);
            if (strstri(path,"anoid_lrg_caves")!=NULL)
                strcpy(mapset,"anoidcaves/");
            if (strstri(path,"arach_lrg_caves")!=NULL)
                strcpy(mapset,"arachcaves/");
            (*strchr(mapset,'/'))=0;
            for (j=eaSize(&g_generators)-1;j>=0;j--) {
                if (strcmp(g_generators[j]->mapSet,mapset)==0) {
                    msg=g_generators[j];
                    break;
                }
            }
            if (j==-1) {
                msg=malloc(sizeof(MapSetGenerator));
                strcpy(msg->mapSet,mapset);
                msg->pieces=0;
                eaPush(&g_generators,msg);
            }
            memset(mp,0,sizeof(MapPiece));
            strcpy(mp->name,objectLibraryNameFromIdx(i));
            strcpy(mp->path,path);
            if (strstri(path,"anoid_lrg_caves")!=NULL || strstri(path,"arach_lrg_caves")!=NULL) {
                if (strstri(path,"3wy")) {
                    mp->type=MG_Room;
                    mp->portals=3;
                }
                if (strstri(path,"4wy")) {
                    mp->type=MG_Room;
                    mp->portals=4;
                }
                if (strstri(path,"jct")) {
                    mp->type=MG_Hall;
                    mp->portals=2;
                }
                if (strstri(path,"elv")) {
                    mp->type=MG_Elevator;
                }
                if (strstri(path,"_end_")) {
                    mp->type=MG_End;
                    mp->portals=1;
                }
                if (strstri(path,"strt")) {
                    mp->type=MG_Start;
                    mp->portals=1;
                }
            } else
            if (strstri(path,"doors")!=NULL) {
                mp->portals=2;
                mp->type=MG_Door;
                mp->size=MG_Small;
            } else
            if (strstri(path,"start")!=NULL) {
                mp->portals=1;
                mp->type=MG_Start;
                mp->size=MG_Small;
            } else
            if (strstri(path,"elev")!=NULL || strstri(path,"hall")!=NULL || strstri(path,"_h_")!=NULL) {
                if (strstri(path,"_3wy_")!=NULL || strstri(path,"Tsection")!=NULL)
                    mp->portals=3;
                else
                if (strstri(path,"_4wy_")!=NULL)
                    mp->portals=4;
                else
                if (strstri(path,"_bnd_")!=NULL || strstri(path,"_str_")!=NULL)
                    mp->portals=2;
                else
                if (strstri(path,"_end")!=NULL)
                    mp->portals=1;
                else
                    mp->portals=2;
                if (strstri(path,"_elv_")!=NULL || strstri(path,"elevator")!=NULL)
                    mp->type=MG_Elevator;
                else
                if (strstri(path,"_h_")!=NULL || strstri(path,"_sh_")!=NULL || strstri(path,"hall")!=NULL)
                    mp->type=MG_Hall;
                else
                    mp->type=MG_Room;
                if (strstri(path,"_end")!=NULL)
                    mp->type=MG_End;
                mp->size=MG_Small;
            } else
            if (strstri(path,"rooms")!=NULL || strstri(path,"rms")) {
                mp->type=MG_Room;
                mp->size=MG_Small;
                if (strstri(path,"_3wy_")!=NULL)
                    mp->portals=3;
                else
                if (strstri(path,"_4wy_")!=NULL)
                    mp->portals=4;
                else
                if (strstri(path,"_bnd")!=NULL || strstri(path,"_str")!=NULL || strstri(path,"_90")!=NULL || strstri(path,"corner")!=NULL)
                    mp->portals=2;
                else
                if (strstri(path,"_end")!=NULL || strstri(path,"deadend")!=NULL) {
                    mp->portals=1;
                    mp->type=MG_End;
                }
                else
                if (strstri(path,"Arach_rm")!=NULL || strstri(path,"Arach_Tech_Lab_01")) {
                    mp->portals=1;
                    mp->type=MG_Room;
                }
            }
            if (mp->portals!=0)
                eaPush(&msg->pieces,mp);
            else {
                printf("%s has no portals.\n",path);
                free(mp);
            }
        }
    }
}

void loadDefByName(char * name) {
    sel.fake_ref.def = groupDefFind(name);
    if (!sel.fake_ref.def)// && !sel.lib_load)
    {
        if (!sel.lib_load)
        {
            sel.lib_load = 1;
            strcpy(sel.lib_name,name);
            editDefLoad(name);
        }
    }
    else
    {
        sel.lib_load = 0;
        copyMat4(unitmat,sel.fake_ref.mat);
        selCopy(NULL);
    }
}

extern void modelSetupZOTris(Model * m);

void getModelPoints(Model * m,const Mat4 mat,Vec2 *** v) {
    int i;
    if (m==NULL)
        return;
    modelSetupZOTris(m);
    for (i=0;i<m->vert_count;i++) {
        Vec3 vec;
        Vec2 * v2=malloc(sizeof(Vec2));
        mulVecMat4(m->unpack.verts[i],mat,vec);
        (*v2)[0]=vec[0];
        (*v2)[1]=vec[2];
        eaPush((F32***)v,v2);
    }
}

void getDefPoints(GroupDef * def,const Mat4 mat,Vec2 *** v) {
    int i;
    if (def->model)
        getModelPoints(def->model,mat,v);
    for (i=0;i<def->count;i++) {
        Mat4 m;
        if (def->entries[i].flags_cache & ONLY_EDITORVISIBLE)
            continue;
        mulMat4(mat,def->entries[i].mat,m);
        getDefPoints(def->entries[i].def,m,v);
    }
}

Vec2 grahamCenterPoint;

int grahamScanComparator(const Vec2 ** v1,const Vec2 ** v2) {
    double d=    atan2((**v2)[1]-grahamCenterPoint[1],(**v2)[0]-grahamCenterPoint[0])-
        atan2((**v1)[1]-grahamCenterPoint[1],(**v1)[0]-grahamCenterPoint[0]);
    if (d<0) return 1;
    if (d>0) return -1;
    return 0;
}

double dGrahamScanComparator(const Vec2 ** v1,const Vec2 ** v2) {
    return    atan2((**v1)[1]-grahamCenterPoint[1],(**v1)[0]-grahamCenterPoint[0])-
            atan2((**v2)[1]-grahamCenterPoint[1],(**v2)[0]-grahamCenterPoint[0]);
}

double grahamComp(Vec2 v) {
    return atan2(v[1]-grahamCenterPoint[1],v[0]-grahamCenterPoint[0]);
}

double distanceSquaredVec2(Vec2 v1,Vec2 v2) {
    return ( (v2[0]-v1[0])*(v2[0]-v1[0])+(v2[1]-v1[1])*(v2[1]-v1[1]));
}

//returns true iff the points make a left turn
int leftTurn(Vec2 V1,Vec2 V2,Vec2 V3) {
    double r[2];
    double n[2];
    double p[2];
    double v1[2];
    double v2[2];
    double v3[2];
    double buf[2];
    double length;
    copyVec2(V1,v1);
    copyVec2(V2,v2);
    copyVec2(V3,v3);
    subVec2(v2,v1,r);
    length=lengthVec2(r);
    scaleVec2(r,1.0/length,buf);
    copyVec2(buf,r);
    n[0]=r[1];
    n[1]=-r[0];
    subVec2(v3,v2,p);
    length=lengthVec2(p);
    scaleVec2(p,1.0/length,buf);
    copyVec2(buf,p);
    return ((n[0]*p[0]+n[1]*p[1])<=0);
}

double turnVal(Vec2 V1,Vec2 V2,Vec2 V3) {
    double r[2];
    double n[2];
    double p[2];
    double v1[2];
    double v2[2];
    double v3[2];
    double buf[2];
    double length;
    copyVec2(V1,v1);
    copyVec2(V2,v2);
    copyVec2(V3,v3);
    subVec2(v2,v1,r);
    length=lengthVec2(r);
    scaleVec2(r,1.0/length,buf);
    copyVec2(buf,r);
    n[0]=r[1];
    n[1]=-r[0];
    subVec2(v3,v2,p);
    length=lengthVec2(p);
    scaleVec2(p,1.0/length,buf);
    copyVec2(buf,p);
    return ((n[0]*p[0]+n[1]*p[1]));
}

void getCirclePoints(Vec2 *** v) {
    int i;
    for (i=0;i<100;i++) {
        Vec2 * vec=malloc(sizeof(Vec2));
        (*vec)[0]=((double)(rand()%200))-100;
        (*vec)[1]=((double)(rand()%200))-100;
        if ((*vec)[0]*(*vec)[0]+(*vec)[1]*(*vec)[1]<10000)
            eaPush((F32***)v,vec);
    }
}

void showVec2(Vec2 v) {
    printf("(%f,%f)",v[0],v[1]);
}

double watan2(double y,double x) {
    return atan2(y,x);
}

double dabs(double d) {
    if (d<0)
        return -d;
    return d;
}

void findBoundingBox(DefTracker * tracker,Vec2 *** v) {
    Vec2 ** allPoints=0;
    int i;
    F32 minx,miny,maxx,maxy;
    minx=miny=1e9;
    maxx=maxy=1e-9;

    getDefPoints(tracker->def,unitmat,&allPoints);
    //    getCirclePoints(&allPoints);
    if (eaSize((F32 ***)&allPoints)==0)
        return;
    copyVec2(*(allPoints[0]),grahamCenterPoint);
    for (i=0;i<eaSize((F32 ***)&allPoints);i++) {
        if ((*(allPoints[i]))[0]<minx)
            minx=(*(allPoints[i]))[0];
        if ((*(allPoints[i]))[1]<miny)
            miny=(*(allPoints[i]))[1];
        if ((*(allPoints[i]))[0]>maxx)
            maxx=(*(allPoints[i]))[0];
        if ((*(allPoints[i]))[1]>maxy)
            maxy=(*(allPoints[i]))[1];
        free(allPoints[i]);
    }
    for (i=0;i<4;i++) {
        Vec2 * corner=calloc(1, sizeof(Vec2));
        eaPush((F32***)v,corner);
    }
    (*(*v)[0])[0]=minx;
    (*(*v)[0])[1]=miny;
    (*(*v)[1])[0]=maxx;
    (*(*v)[1])[1]=miny;
    (*(*v)[2])[0]=maxx;
    (*(*v)[2])[1]=maxy;
    (*(*v)[3])[0]=minx;
    (*(*v)[3])[1]=maxy;
    eaDestroy((F32 ***)&allPoints);
}

void findConvexHull(DefTracker * tracker,Vec2 *** v) {
    Vec2 ** allPoints=0;
    int i,j,k;
    getDefPoints(tracker->def,unitmat,&allPoints);
//    getCirclePoints(&allPoints);
    if (eaSize((F32 ***)&allPoints)==0)
        return;
    copyVec2(*(allPoints[0]),grahamCenterPoint);
    for (i=0;i<eaSize((F32 ***)&allPoints);i++) {
        if ((*(allPoints[i]))[1]<grahamCenterPoint[1])
            copyVec2((*(allPoints[i])),grahamCenterPoint);
    }
    eaQSort((F32 **)allPoints,grahamScanComparator);
    //march i and j through the array, when one is strictly better than the other, copy
    //it to where k is.  this will take out redundancies in allPoints
    for (i=0,j=1,k=0;j<eaSize((F32 ***)&allPoints)-1;) {
        if (dabs(dGrahamScanComparator(&allPoints[i],&allPoints[j]))<1e-5) {
            if (distanceSquaredVec2(*(allPoints[i]),grahamCenterPoint)<
                distanceSquaredVec2(*(allPoints[j]),grahamCenterPoint)) {
                free(allPoints[j]);
                allPoints[j]=NULL;
                j++;
            } else {
                free(allPoints[i]);
                allPoints[i]=NULL;
                i++;
                while (allPoints[i]==NULL && i<eaSize((F32 ***)&allPoints)-1)
                    i++;
            }
            if (i==j)
                j++;
        } else if (distanceSquaredVec2(*(allPoints[i]),*(allPoints[j]))<1e-3) {
            free(allPoints[j]);
            allPoints[j]=NULL;
            j++;
        } else {
            allPoints[k++]=allPoints[i];
            i++;
            while (allPoints[i]==NULL && i<eaSize((F32 ***)&allPoints)-1)
                i++;
            if (i==j)
                j++;
        }
    }
    eaSetSize((F32***)&allPoints,k);
    eaPush((F32***)v,allPoints[0]);
    eaPush((F32***)v,allPoints[1]);
    eaPush((F32***)v,allPoints[2]);
    for (i=3;i<eaSize((F32 ***)&allPoints);i++) {
        double val=turnVal(*((*v)[eaSize((F32 ***)v)-2]),*((*v)[eaSize((F32 ***)v)-1]),*(allPoints[i]));
        while(val>0) {
            eaPop((F32***)v);
            val=turnVal(*((*v)[eaSize((F32 ***)v)-2]),*((*v)[eaSize((F32 ***)v)-1]),*(allPoints[i]));
        }
        eaPush((F32***)v,allPoints[i]);
    }
    eaDestroy((F32 ***)&allPoints);
}

double getArea(GroupDef * def) {
    int i,j;
    double area;
    for (i=0;i<eaSize(&g_generators);i++) {
        for (j=0;j<eaSize(&g_generators[i]->pieces);j++) {
            if (stricmp(g_generators[i]->pieces[j]->name,def->name)==0)
                return g_generators[i]->pieces[j]->area;
        }
    }
    area=0;
    for (i=0;i<def->count;i++)
        area+=getArea(def->entries[i].def);
    return area;
}

void findPointInside(GroupDef * def,const Mat4 mat,MapPiece * mp) {
    Mat4 buf;
    int i;
    if (strstriConst(def->name,"encounter")) {
        copyVec3(mat[3],mp->inside);
        mp->pointInside=1;
    } else
    if (strstriConst(def->name,"objective_spawn") || strstriConst(def->name,"objctv_spwn"))
        mp->hasObjective=1;
    if (mp->pointInside && mp->hasObjective)    //break out early if we've already found everything we could want
        return;
    for (i=0;i<def->count;i++) {
        mulMat4(mat,def->entries[i].mat,buf);
        findPointInside(def->entries[i].def,buf,mp);
    }
}

void createAllCtris(GroupDef * def) {
    int i;
    for (i=0;i<def->count;i++) {
        createAllCtris(def->entries[i].def);
    }
    if (def->model)
        modelCreateCtris(def->model);
}

void addAllPortals(GroupDef * def,MapPiece * mp) {
    int i;
    for (i=0;i<def->count;i++) {
        addAllPortals(def->entries[i].def,mp);
    }
    if (def->model && def->model->extra) {
        int k;
        for (k=0;k<def->model->extra->portal_count;k++) {
            copyVec3(def->model->extra->portals[k+mp->portals].pos,mp->portal[k+mp->portals]);
            copyVec3(def->model->extra->portals[k+mp->portals].normal,mp->normal[k+mp->portals]);
        }
        mp->portals+=def->model->extra->portal_count;
    }
    
}

int preprocessGenerators() {
    int i,j,k;
    static int rewrite=0;
    static int read=0;
    if (!read) {
        FILE * fin=fopen("C:\\game\\data\\Defs\\MapGeneration.pieces","r");
        char mapset[128];
        char piece[128];
        MapPiece * mp;
        MapPiece dummyPiece;
        int hullsize,portals;
        float area;
        Vec2 * v;
        Vec3 a,b;
        eaDestroyEx(&g_excludes,NULL);
        while (1) {
            if (fscanf(fin,"%s %s",mapset,piece)==EOF)
                break;
            if (stricmp(mapset,"exclude")==0) {
                eaPush(&g_excludes,strdup(piece));
                continue;
            }
//            if (stricmp(mapset,"eof")==0 && stricmp(piece,"eof")==0)
//                break;
            mp=NULL;
            for (i=0;i<eaSize(&g_generators);i++) {
                if (stricmp(g_generators[i]->mapSet,mapset)!=0)
                    continue;
                for (j=0;j<eaSize(&g_generators[i]->pieces);j++) {
                    if (stricmp(piece,g_generators[i]->pieces[j]->name)!=0)
                        continue;
                    else {
                        mp=g_generators[i]->pieces[j];
                        break;
                    }
                }
                if (mp!=NULL)
                    break;
            }
            if (mp==NULL) {
                mp=&dummyPiece;
                memset(mp,0,sizeof(*mp));
            }
            fscanf(fin,"%d",&hullsize);
            v=calloc(hullsize, sizeof(Vec2));
            for (i=0;i<hullsize;i++) {
                fscanf(fin,"%f %f",&v[i][0],&v[i][1]);
                eaPush((F32***)&mp->convexHull,&v[i]);
            }
            fscanf(fin,"\n");
            fscanf(fin,"%f",&area);
            mp->area=area;
            fscanf(fin,"%d",&portals);
            mp->portals=portals;
            for (i=0;i<mp->portals;i++) {
                fscanf(fin,"%f %f %f %f %f %f",&a[0],&a[1],&a[2],&b[0],&b[1],&b[2]);
                copyVec3(a,mp->portal[i]);
                copyVec3(b,mp->normal[i]);
            }
            //this is a clever little hack so that elevators work just like normal pieces
            if (mp->type==MG_Elevator) {
                copyVec3(mp->portal[0],mp->portal[1]);
                copyVec3(mp->normal[0],mp->normal[1]);
            }
            fscanf(fin,"%d",&mp->pointInside);
            if (mp->pointInside)
                fscanf(fin,"%f %f %f",&mp->inside[0],&mp->inside[1],&mp->inside[2]);
            fscanf(fin,"%d",&mp->hasObjective);
        }
        read=1;
    }
    for (i=0;i<eaSize(&g_generators);i++) {
        for (j=0;j<eaSize(&g_generators[i]->pieces);j++) {
            MapPiece * mp=g_generators[i]->pieces[j];
            if (eaSize((F32 ***)&mp->convexHull)>0)
                continue;
            if (sel.lib_load)
                return 1;
            if (!sel.fake_ref.def || stricmp(sel.fake_ref.def->name,mp->name)!=0) {
                loadDefByName(mp->name);
                return 1;
            }
            {
                int k;
                for (k=0;k<eaSize(&g_excludes);k++) {
                    if (stricmp(g_generators[i]->pieces[j]->name,g_excludes[k])==0) {
                        break;
                    }
                }
                if (k<eaSize(&g_excludes)) {
                    eaRemove(&g_generators[i]->pieces,k);
                    j--;    // since we're screwing with the array that we're looping over, fix the loop variable
                    continue;
                }
            }
            //do my stuff
            {
                int portals;
                int k;
                double area=0;
                if (strstriConst(sel.fake_ref.def->name,"ofc"))
                    findBoundingBox(&sel.fake_ref,&mp->convexHull);
                else
                    findConvexHull(&sel.fake_ref,&mp->convexHull);
                for (k=0;k<eaSize((F32 ***)&mp->convexHull);k++) {
                    int k2=(k+1)%eaSize((F32 ***)&mp->convexHull);
                    area+=    (*mp->convexHull[k])[0]*(*mp->convexHull[k2])[1]-
                            (*mp->convexHull[k2])[0]*(*mp->convexHull[k])[1];
                }
                mp->area=area;
                createAllCtris(sel.fake_ref.def);
                findPointInside(sel.fake_ref.def,unitmat,mp);
                mp->inside[1]+=2;    //just to be safe, bump it up two feet
                portals=mp->portals;
                mp->portals=0;
                addAllPortals(sel.fake_ref.def,mp);
                if ( (mp->type!=MG_Door && mp->type!=MG_Elevator && mp->portals!=portals) || 
                    (mp->type==MG_Elevator && mp->portals!=portals-1) ) {
                    printf("%s: %d portals defined, but %d on the geometry!\n",mp->name,portals,mp->portals);
                }
                mp->portals=portals;
                rewrite=1;
            }
            return 1;
        }
    }
    if (rewrite) {
        FILE * fout=fopen("C:\\game\\data\\Defs\\MapGeneration.pieces","w");
        if (!fout)
            return 0;
        for (i=0;i<eaSize(&g_excludes);i++)
            fprintf(fout,"exclude %s\n",g_excludes[i]);
        fprintf(fout,"\n");
        for (i=0;i<eaSize(&g_generators);i++) {
            for (j=0;j<eaSize(&g_generators[i]->pieces);j++) {
                MapPiece * mp=g_generators[i]->pieces[j];
                fprintf(fout,"%s %s\n",g_generators[i]->mapSet,mp->name);
                fprintf(fout,"\t%d\n",eaSize((F32 ***)&mp->convexHull));
                for (k=0;k<eaSize((F32 ***)&mp->convexHull);k++)
                    fprintf(fout,"\t\t%f %f\n",(*(mp->convexHull[k]))[0],(*(mp->convexHull[k]))[1]);
                fprintf(fout," %f ",mp->area);
                fprintf(fout,"\t%d\n",mp->portals);
                for (k=0;k<mp->portals;k++)
                    fprintf(fout,"\t\t%f %f %f    %f %f %f\n",
                    mp->portal[k][0],
                    mp->portal[k][1],
                    mp->portal[k][2],
                    mp->normal[k][0],
                    mp->normal[k][1],
                    mp->normal[k][2]);
                fprintf(fout,"\t%d\n",mp->pointInside);
                if (mp->pointInside)
                    fprintf(fout,"\t%f %f %f\n",mp->inside[0],mp->inside[1],mp->inside[2]);
                fprintf(fout,"\t%d\n",mp->hasObjective);
            }
        }
        fprintf(fout,"eof eof");    //eof marker
        fclose(fout);
        rewrite=0;
    }
    return 0;
}

//passing -1 for portals, type, or size indicates that that any value is acceptable for that field
//exception: will not return MG_Door types when passed -1 for type
MapPiece * getRandomPiece(MapSetGenerator * msg,int minPortals,int maxPortals,MGRoomType type,MGRoomSize size,double area) {
    int i;
    int total=0;
    for (i=0;i<eaSize(&msg->pieces);i++) {
        if ( msg->pieces[i]->portals>0 &&
            (minPortals    ==-1 || msg->pieces[i]->portals    >=minPortals) &&
            (maxPortals    ==-1 || msg->pieces[i]->portals    <=maxPortals) &&
            ((type        ==-1 && msg->pieces[i]->type!=MG_Door && msg->pieces[i]->type!=MG_Start) || msg->pieces[i]->type    ==type        ) &&
            (size        ==-1 || msg->pieces[i]->size    ==size        )    &&
            (area==-1         || msg->pieces[i]->area<area || msg->pieces[i]->portals==1) )
            total++;
    }
    if (total==0) return NULL;
    for (i=0;i<eaSize(&msg->pieces);i++) {
        if ( msg->pieces[i]->portals>0 &&
            (minPortals    ==-1 || msg->pieces[i]->portals    >=minPortals) &&
            (maxPortals    ==-1 || msg->pieces[i]->portals    <=maxPortals) &&
            ((type        ==-1 && msg->pieces[i]->type!=MG_Door && msg->pieces[i]->type!=MG_Start) || msg->pieces[i]->type    ==type        ) &&
            (size        ==-1 || msg->pieces[i]->size    ==size        )    &&
            (area==-1         || msg->pieces[i]->area<area || msg->pieces[i]->portals==1) )
            if ((rand()%total)==0)
                return msg->pieces[i];
            else
                total--;
    }
    return NULL;    //should never reach here
}

//source is true iff we want the 'A' side elevator, false iff we want the 'B' side
//floor is the floor number of the elevator
MapPiece * getElevator(MapSetGenerator * msg,int source,int floor) {
    int i;
    int total=0;
    char buf[3];
    sprintf(buf,"%d%c",source?floor+1:floor,source?'A':'B');
    for (i=0;i<eaSize(&msg->pieces);i++) {
        if (msg->pieces[i]->type==MG_Elevator && 
            strstriConst(msg->pieces[i]->name,buf)==msg->pieces[i]->name+strlen(msg->pieces[i]->name)-2)
            total++;
    }
    if (total==0)
        return NULL;
    for (i=0;i<eaSize(&msg->pieces);i++) {
        if (msg->pieces[i]->type==MG_Elevator && 
            strstriConst(msg->pieces[i]->name,buf)==msg->pieces[i]->name+strlen(msg->pieces[i]->name)-2)
            if ((rand()%total)==0)
                return msg->pieces[i];
            else
                total--;
    }
    return NULL;    //should never reach here
}

int segmentsIntersect(Vec3 a1,Vec3 a2,Vec3 b1,Vec3 b2) {
    Vec3 av,bv,w;
    float D;
    float s;
    subVec3(a2,a1,av);
    subVec3(b2,b1,bv);
    D=av[0]*bv[2]-av[2]*bv[0];
    if (fabs(D)<1e-5)
        return 0;
    subVec3(a1,b1,w);
    s=av[0]*w[2]-av[2]*w[0];
    if (s<0 || s>D)
        return 0;
    s=bv[0]*w[2]-bv[2]*w[0];
    if (s<0 || s>D)
        return 0;
    return 1;
}

int piecesOverlapHelper(MGNode * a,MGNode * b) {
    Vec3 va,vb,va2,buf,buf1,buf2;
    int i,j;
    int inside;
    for (i=0;i<eaSize((F32 ***)&a->piece->convexHull);i++) {
        int i2=(i+1)%eaSize((F32 ***)&a->piece->convexHull);
        buf[0]=(*(a->piece->convexHull[i]))[0];
        buf[1]=0;
        buf[2]=(*(a->piece->convexHull[i]))[1];
        mulVecMat4(buf,a->mat,va);
        buf[0]=(*(a->piece->convexHull[i2]))[0];
        buf[1]=0;
        buf[2]=(*(a->piece->convexHull[i2]))[1];
        mulVecMat4(buf,a->mat,va2);
        inside=1;
        for (j=0;j<eaSize((F32 ***)&b->piece->convexHull);j++) {
            int j2=(j+1)%eaSize((F32 ***)&b->piece->convexHull);
            buf[0]=(*(b->piece->convexHull[j]))[0];
            buf[1]=0;
            buf[2]=(*(b->piece->convexHull[j]))[1];
            mulVecMat4(buf,b->mat,buf1);
            buf[0]=(*(b->piece->convexHull[j2]))[0];
            buf[1]=0;
            buf[2]=(*(b->piece->convexHull[j2]))[1];
            mulVecMat4(buf,b->mat,buf2);
            subVec3(buf2,buf1,vb);
            subVec3(va,buf1,buf);
            if (buf[0]*vb[2]-buf[2]*vb[0]>0) {
                inside=0;
                break;
            }
            if (segmentsIntersect(va,va2,buf1,buf2))
                return 1;
        }
        if (inside)
            return 1;
    }
    return 0;
}

int piecesOverlap(MGNode * a,MGNode * b) {
    return (piecesOverlapHelper(a,b) || piecesOverlapHelper(b,a));
}

void connectPieces(MGNode * target,MGNode * source) {
    Mat4 targetMat,sourceMat;
    Mat4 mbuf;
    Vec3 vbuf;
    double magnitude;
    int i;
    int sourceIndex,targetIndex;

    if (target==NULL || source==NULL)
        return;
    for (i=0;i<target->piece->portals;i++)
        if (target->adjacent[i]==source)
            targetIndex=i;
    if (target->piece->type==MG_Door) {
        for (i=0;i<source->piece->portals;i++)
            if (source->doors[i]==target)
                sourceIndex=i;
    } else {
        for (i=0;i<source->piece->portals;i++)
            if (source->adjacent[i]==target)
                sourceIndex=i;
    }

    memset(targetMat,0,sizeof(Mat4));
    copyVec3(target->piece->normal[targetIndex],vbuf);                //set up the target matrix
    vbuf[0]=-vbuf[0];                                                //we want to invert the normal so we know
    vbuf[2]=-vbuf[2];                                                //what to rotate the source normal to
    mulVecMat3(vbuf,unitmat,targetMat[0]);                            //set the forward vector
    targetMat[1][1]=1;                                                //set the up vector
    crossVec3(targetMat[0],targetMat[1],targetMat[2]);                //set the side vector
    magnitude=lengthVec3(targetMat[2]);                                //now make sure that it is normalized
    scaleVec3(targetMat[2],1.0/magnitude,targetMat[2]);
    mulVecMat4(target->piece->portal[targetIndex],unitmat,targetMat[3]);    //now set position

    memset(sourceMat,0,sizeof(Mat4));
    copyVec3(source->piece->normal[sourceIndex],vbuf);                //set up the source matrix
    mulVecMat3(vbuf,source->mat,sourceMat[0]);                        //set the forward vector
    sourceMat[1][1]=1;                                                //set the up vector
    crossVec3(sourceMat[0],sourceMat[1],sourceMat[2]);                //set the side vector
    magnitude=lengthVec3(sourceMat[2]);                                //now make sure that it is normalized
    scaleVec3(sourceMat[2],1.0/magnitude,sourceMat[2]);
    mulVecMat4(source->piece->portal[sourceIndex],source->mat,sourceMat[3]);    //now set position

    invertMat4Copy(targetMat,mbuf);
    mulMat4(sourceMat,mbuf,target->mat);

    //now we have to fudge the position so it sits nicely on the grid
    for (i=0;i<3;i++) {
        int fudgeFactor=16;
        int sign=(target->mat[3][i]<0)?-1:1;
        int cur=((int)(target->mat[3][i]+(double)sign/2.0));
        cur=abs(cur);
        if (i==1)        //don't fudge the verticle
            continue;
        if (cur%fudgeFactor<(fudgeFactor/2))
            target->mat[3][i]=(cur-(cur%fudgeFactor))*sign;
        else
            target->mat[3][i]=(cur+fudgeFactor-(cur%fudgeFactor))*sign;
    }
}

void connectElevators(MGNode * target,MGNode * source) {
    copyMat4(source->mat,target->mat);        //to connect elevators, we just set their locations the same
    target->mat[3][0]+=4096;                //then translate one far enough away that we don't risk the
    target->mat[3][2]+=4096;                //two floors running into each other
}

void fitPieceToPiece(MGNode * target,MGNode * source) {
    if (target->piece->type==MG_Elevator && source->piece->type==MG_Elevator)
        connectElevators(target,source);
    else
        connectPieces(target,source);
}

int mapGeneratorClock;

MGNode * generateMapRecursive(MapSetGenerator * msg,MissionSetRules * msr,CurrentMapStatus * cms,MGNode * sourceNode,int sourceIndex) {
    MGNode * node=calloc(1, sizeof(MGNode));
    MGNode * temp;
    static int lowestElevator;        //we need this so floor X doesn't get two elevators to floor X+1
    int branchPath;                    //portal to which the main branch goes
    int i;

    if (cms->maxArea<0 || ((clock()-mapGeneratorClock)/CLK_TCK>5)) {
        free(node);
        if (sourceNode!=NULL)
            sourceNode->adjacent[sourceIndex]=NULL;
        return NULL;
    }

    if (sourceNode==NULL) {
        copyMat4(unitmat,node->mat);
        node->startNode=node;
        node->floor=0;
        node->hasDoors=0;
        node->depth=0;
        node->roomsThisFloor=0;
        lowestElevator=-1;
    } else {
        node->startNode=sourceNode->startNode;
        node->floor=sourceNode->floor;
        node->depth=sourceNode->depth+1;
        node->roomsThisFloor=0;
        if (sourceNode->piece->type==MG_Elevator && sourceNode->adjacent[sourceNode->source]->piece->type!=MG_Elevator)
            node->floor++;    //this room will have to be an elevator, so it will be one floor higher
        node->hasDoors=!sourceNode->hasDoors;
    }

    if (cms->depth==0) {
        node->piece=getRandomPiece(msg,1,1,MG_Start,-1,-1);
    } else if (cms->depth==msr->maxDepth) {
        node->piece=getRandomPiece(msg,1,1,MG_End,-1,-1);
    } else if (sourceNode->piece->type==MG_Elevator && sourceNode->adjacent[sourceNode->source]->piece->type!=MG_Elevator) {
        node->piece=getElevator(msg,0,node->floor);
        node->roomsThisFloor=1;
    } else if (cms->halls<msr->minHalls) {
        node->piece=getRandomPiece(msg,-1,-1,MG_Hall,-1,-1);
    } else {
        node->piece=getRandomPiece(msg,-1,-1,-1,-1,-1);
        if (node->piece && node->piece->type==MG_Elevator)
            if (node->floor>lowestElevator) {
                node->piece=getElevator(msg,1,node->floor);
                lowestElevator=node->floor;
            } else
                node->piece=NULL;
    }
    
    //if for any reason this piece is not valid, quit out of this branch now
    if (node->piece==NULL    || (sourceNode && sourceNode->piece->type==MG_Elevator && 
                                ((    sourceNode->adjacent[sourceNode->source]->piece->type!=MG_Elevator &&
                                    node->piece->type!=MG_Elevator) ||
                                (    sourceNode->adjacent[sourceNode->source]->piece->type==MG_Elevator &&
                                    node->piece->type==MG_Elevator)))
                            ||    (cms->halls==msr->maxHalls && node->piece->type==MG_Hall)
                            ||    (node->piece->area>cms->maxArea)
                            ||    (cms->depth>msr->maxDepth)
                            ||    (node->piece->portals==1 && cms->depth<msr->minDepth+1 && cms->depth>0)
                            ||    (sourceNode && node->piece==sourceNode->piece)) {
        if (node->piece!=NULL && node->piece->type==MG_Elevator && sourceNode!=NULL && sourceNode->piece->type!=MG_Elevator && lowestElevator==node->floor)
            lowestElevator--;
        free(node);
        if (sourceNode!=NULL)
            sourceNode->adjacent[sourceIndex]=NULL;
        return NULL;
    }
    if (node->piece->type!=MG_Elevator && node->piece->type!=MG_Start)
        node->roomsThisFloor=1;
    if (sourceNode!=NULL) {
        node->source=rand()%node->piece->portals;
        node->adjacent[node->source]=sourceNode;
    } else
        node->source=-1;
    if (sourceIndex!=-1) {
        sourceNode->adjacent[sourceIndex]=node;
        fitPieceToPiece(node,sourceNode);
    }
    i=0;
    temp=getNthNode(node->startNode,i);
    while (sourceNode!=NULL && temp!=node) {
        i++;
        if (temp!=sourceNode && piecesOverlap(node,temp)) {
            if (node->piece!=NULL && node->piece->type==MG_Elevator && sourceNode!=NULL && sourceNode->piece->type!=MG_Elevator && lowestElevator==node->floor)
                lowestElevator--;
            free(node);
            sourceNode->adjacent[sourceIndex]=NULL;
            return NULL;
        }
        temp=getNthNode(node->startNode,i);
    }
    assert(temp!=NULL);
    node->area=node->piece->area;
    do {
        branchPath=rand()%node->piece->portals;
    } while (node->piece->portals>1 && branchPath==node->source);
    for (i=0;i<node->piece->portals;i++) {
        int tries=5;
        if (i==node->source)
            continue;
        do {
            CurrentMapStatus cmsBuffer;
            memcpy(&cmsBuffer,cms,sizeof(CurrentMapStatus));
            if (node->piece->type==MG_Elevator)
                cms->depth--;                    //this prevents elevators from making maps too small
            if (i==branchPath)
                cms->depth++;
            else
                cms->depth+=msr->branchFactor;
            if (node->piece->type==MG_Hall)
                cms->halls++;
            else
            if (node->piece->type==MG_Start)
                cms->halls=msr->maxHalls-msr->minHalls-1;    //to avoid starting with a long boring hallway
            else
                cms->halls=0;
            cms->maxArea=(cms->maxArea-node->area);
            cms->minArea=0;
            generateMapRecursive(msg,msr,cms,node,i);
            memcpy(cms,&cmsBuffer,sizeof(CurrentMapStatus));
            tries--;
        } while (node->adjacent[i]==NULL && tries>0);
        
        if (node->adjacent[i]==NULL) {
            int j;
            for (j=0;j<i;j++) {
                if (j==node->source)
                    continue;
                destroyMGNode(node->adjacent[j]);
                node->adjacent[j]=NULL;
            }
            if (node->piece!=NULL && node->piece->type==MG_Elevator && sourceNode!=NULL && sourceNode->piece->type!=MG_Elevator && lowestElevator==node->floor)
                lowestElevator--;
            free(node);
            if (sourceNode!=NULL)
                sourceNode->adjacent[sourceIndex]=NULL;
            return NULL;
        }
        node->area+=node->adjacent[i]->area;
        if (node->piece->type!=MG_Elevator || node->adjacent[i]->piece->type!=MG_Elevator)
            node->roomsThisFloor+=node->adjacent[i]->roomsThisFloor;
    }
    //if we are going to use this piece, go ahead and throw some doors on it
    if (node->hasDoors)
        for (i=0;i<node->piece->portals;i++) {
            MGNode * door=calloc(1, sizeof(MGNode));
            node->doors[i]=door;
            door->piece=getRandomPiece(msg,-1,-1,MG_Door,-1,-1);
            if (door->piece==NULL) {
                node->hasDoors=0;
                break;                    //some mapsets don't have doors, so don't worry if we can't find one
            }
            door->adjacent[0]=node;
            fitPieceToPiece(door,node);
        }
    //last check here to make sure this is a valid node
    if (node->area<cms->minArea || node->area>cms->maxArea || ((node->piece->type==MG_Start || (node->piece->type==MG_Elevator && sourceNode->piece->type==MG_Elevator)) && node->roomsThisFloor<msr->roomsPerFloor)) {
        destroyMGNode(node);
        if (sourceNode!=NULL)
            sourceNode->adjacent[sourceIndex]=NULL;
        return NULL;
    }
    return node;
}

static char workingDirectory[256];
static char filename[256];
int waitingForLoad;
static int makingVillainsMaps;
static int mapIsValid;

void writeMap(MGNode * mgn,MissionSetRules * msr,int seed) {
    MGNode * node;
    MGNode * temp;
    Vec3 pyr;
    int i=0;
    int j;
    FILE * fout;
    int firstRoom=0;
//    Vec3 lastRoomInside;
    int furthestInside=0;
    int currentObjective=9;
    char * extentsNames[5];
    MGNode ** nodes=0;
    int currentRoom=1;
    eaPush(&nodes,mgn);
    extentsNames[0]="Extents_Marker.txt";
    extentsNames[1]="extents_marker_01";
    extentsNames[2]="extents_marker_02";
    extentsNames[3]="extents_marker_03";
    extentsNames[4]="extents_marker_04";

    if (mgn==NULL)
        return;
    if (mapIsValid)
        sprintf(filename,"%s/Valid_R_%s_%d.txt",workingDirectory,msr->missionSet,seed);
    else
        sprintf(filename,"%s/_R_%s_%d.txt",workingDirectory,msr->missionSet,seed);
    fout=fopen(filename,"w");
    fprintf(fout,"Version 2\n");
    if (makingVillainsMaps && msr->villainsSceneFile[0]) {        //if no villain scene file is specified
        fprintf(fout,"Scenefile %s\n",msr->villainsSceneFile);    //then just use the heroes one as default
    } else {
        if (msr->sceneFile[0])
            fprintf(fout,"Scenefile %s\n",msr->sceneFile);
    }
    fprintf(fout,"Def grp1\n");
    while (i<eaSize(&nodes)) {
        temp=nodes[i++];
        for (j=0;j<(temp->hasDoors?temp->piece->portals+1:1)-((temp->hasDoors && temp->piece->type==MG_Elevator)?1:0);j++) {
            if (j==0)                        //this is sorta ugly, but in the case that the room
                node=temp;                    //has doors, we write all of those to the file, but
            else                            //only after we've written the room itself
                node=temp->doors[j-1];        //hence the j-1 here
            fprintf(fout,"\tGroup %s\n",node->piece->path);
            fprintf(fout,"\t\tpos %f %f %f\n",node->mat[3][0],node->mat[3][1],node->mat[3][2]);
            getMat3PYR(node->mat,pyr);
            fprintf(fout,"\t\trot %f %f %f\n",pyr[0],pyr[1],pyr[2]);
            fprintf(fout,"\tEnd\n");
    
            //only run this stuff for rooms, not doors
            if (node==temp) {
                int k,l;
                int used[4];
                for (l=0;l<node->piece->portals;l++)
                    used[l]=0;
                for (l=0;l<node->piece->portals-(node->source==-1?0:1);l++) {
                    int min=1000000000;
                    int bestIndex=-1;
                    for (k=0;k<node->piece->portals;k++) {
                        if (k==node->source)
                            continue;
                        if (node->adjacent[k]->area<min && !used[k]) {
                            min=node->adjacent[k]->area;
                            bestIndex=k;
                        }
                    }
                    eaPush(&nodes,node->adjacent[bestIndex]);
                    used[bestIndex]=1;
                }

                //put additional conditions here for determining which rooms get room markers
/*                if (node->number!=-1 && node->piece->pointInside) {
                    Vec3 buf;
                    mulVecMat4(node->piece->inside,node->mat,buf);
                    fprintf(fout,"\tGroup Omni/MissionLabels/Mission_Room%s%d\n",currentRoom<10?"_0":"_",currentRoom);
                    fprintf(fout,"\t\tpos %f %f %f\n\tEnd\n",buf[0],buf[1],buf[2]);
                    currentRoom++;
                    if (!firstRoom) {
                        fprintf(fout,"\tGroup Omni/MissionLabels/Mission_Start\n");
                        fprintf(fout,"\t\tpos %f %f %f\n\tEnd\n",buf[0],buf[1],buf[2]);
                        firstRoom=1;
                    }
*/                    
                    //rate this node for objective placement
                    if (node->piece->hasObjective)
                        node->objectiveRating=(int)((node->piece->area))*sqrt((double)(node->depth*12/node->piece->portals));
                    else
                        node->objectiveRating=0;
                    if (node->piece->pointInside)
                        node->roomRating=(int)(sqrt(node->piece->area))*node->depth*12/node->piece->portals;
                    else
                        node->roomRating=0;
/*                    if (node->depth>=msr->minObjectiveDepth && node->piece->hasObjective && currentObjective>0) {
                        fprintf(fout,"\tGroup Omni/MissionLabels/Mission_Objective_0%d\n",currentObjective--);
                        fprintf(fout,"\t\tpos %f %f %f\n\tEnd\n",buf[0],buf[1],buf[2]);
                    }
*/
//                    if (node->depth>furthestInside) {
//                        copyVec3(buf,lastRoomInside);
///                        furthestInside=node->depth;
//                    }
    //            }
                if (node->hasExtents) {
                    fprintf(fout,"\tGroup Omni/%s\n",extentsNames[node->hasExtents-1]);
                    fprintf(fout,"\t\tpos %f 0 %f\n",node->min[0]-25,node->min[1]-25);
                    fprintf(fout,"\t\trot 0 3.1415926 0\n\tEnd\n");
                    fprintf(fout,"\tGroup Omni/%s\n",extentsNames[node->hasExtents-1]);
                    fprintf(fout,"\t\tpos %f 0 %f\n\tEnd\n",node->max[0]+25,node->max[1]+25);
                }
            }
        }
    }
    {
        int objectivesLeft=0;
        int currentRoom=1;
        Vec3 buf;
        MGNode * endRoom=NULL;
        double endRoomBestArea=0;
        for (i=0;i<msr->numObjectives;i++) {
            MGNode * bestNode=NULL;
            MGNode * temp;
            int bestRating=0;
            int ratingFactor;
            for (j=0;j<eaSize(&nodes);j++)
                if (nodes[j]->objectiveRating>bestRating) {
                    bestRating=nodes[j]->objectiveRating;
                    bestNode=nodes[j];
                }
            if (bestNode!=NULL) {
                bestNode->objectiveRating=-1;
                bestNode->roomRating=-1;
                temp=bestNode->adjacent[bestNode->source];
                ratingFactor=bestNode->depth;
                objectivesLeft++;
                while (temp!=NULL && ratingFactor>0) {
                    if (temp->objectiveRating>0)        //don't want to divide anything we've set to -1
                        temp->objectiveRating/=ratingFactor;
                    ratingFactor-=temp->piece->portals-1;
                    if (temp->source==-1)
                        break;
                    temp=temp->adjacent[temp->source];
                }
            }
        }
        //we want the end room to be valid, and this is surprisingly hard, so we choose the largest
        //objective room with no objective room after it and make this the end room
        for (i=0;i<eaSize(&nodes);i++) {
            if (nodes[i]->objectiveRating==-1) {
                MGNode * temp=nodes[i]->adjacent[nodes[i]->source];
                if (nodes[i]->source==-1)
                    continue;
                while (temp!=NULL) {
                    if (temp->objectiveRating==-1)
                        temp->objectiveRating=-2;
                    if (temp->source==-1)
                        break;
                    temp=temp->adjacent[temp->source];
                }
            }
        }
        //weight end rooms 2 times as much, this should generally make the 'end' room be in something that
        //will have sufficient spawns and such
        for (i=0;i<eaSize(&nodes);i++) {
            if (nodes[i]->objectiveRating==-1 && nodes[i]->area*(nodes[i]->piece->type==MG_End?2:1)>endRoomBestArea) {
                endRoomBestArea=nodes[i]->area*(nodes[i]->piece->type==MG_End?2:1);
                endRoom=nodes[i];
            }
            if (nodes[i]->objectiveRating==-2)
                nodes[i]->objectiveRating=-1;
        }
        for (i=0;i<eaSize(&nodes);i++)
            if (nodes[i]->objectiveRating==-1) {
                mulVecMat4(nodes[i]->piece->inside,nodes[i]->mat,buf);
                if (nodes[i]==endRoom) {
                    fprintf(fout,"\tGroup Omni/MissionLabels/Mission_Objective_01\n");
                    fprintf(fout,"\t\tpos %f %f %f\n\tEnd\n",buf[0],buf[1],buf[2]);
                    fprintf(fout,"\tGroup Omni/MissionLabels/Mission_End\n");
                    fprintf(fout,"\t\tpos %f %f %f\n\tEnd\n",buf[0],buf[1],buf[2]);
                } else {
                    fprintf(fout,"\tGroup Omni/MissionLabels/Mission_Objective_0%d\n",objectivesLeft);
                    fprintf(fout,"\t\tpos %f %f %f\n\tEnd\n",buf[0],buf[1],buf[2]);
                    objectivesLeft--;
                }
            }
         for (i=0;i<msr->numRooms-msr->numObjectives;i++) {
            MGNode * bestNode=NULL;
            MGNode * temp;
            int bestRating=0;
            int ratingFactor;
            for (j=0;j<eaSize(&nodes);j++) {
                if (nodes[j]->roomRating>bestRating) {
                    bestRating=nodes[j]->roomRating;
                    bestNode=nodes[j];
                }
            }
            if (bestNode!=NULL) {
                bestNode->roomRating=-1;
                temp=bestNode->adjacent[bestNode->source];
                ratingFactor=bestNode->depth;
                while (temp!=NULL && ratingFactor>0) {
                    if (temp->roomRating>0)
                        temp->roomRating/=ratingFactor;
                    ratingFactor-=temp->piece->portals-1;
                    if (temp->source==-1)
                        break;
                    temp=temp->adjacent[temp->source];
                }
            }
        }
        for (i=0;i<eaSize(&nodes);i++)
            if (nodes[i]->roomRating==-1) {
                mulVecMat4(nodes[i]->piece->inside,nodes[i]->mat,buf);
                if (currentRoom>1 || nodes[i]->piece->type==MG_Room || nodes[i]->objectiveRating<0) {
                    fprintf(fout,"\tGroup Omni/MissionLabels/Mission_Room%s%d\n",currentRoom<10?"_0":"_",currentRoom);
                    fprintf(fout,"\t\tpos %f %f %f\n\tEnd\n",buf[0],buf[1],buf[2]);
                    if (currentRoom==1) {
                        fprintf(fout,"\tGroup Omni/MissionLabels/Mission_Start\n");
                        fprintf(fout,"\t\tpos %f %f %f\n\tEnd\n",buf[0],buf[1],buf[2]);
                    }
                    currentRoom++;
                }
            }
    }
    //put in the last room marker

    fprintf(fout,"End\n");
    if (msr->ambientSound[0])
        fprintf(fout,"Def grpsnd2\n\tGroup object_library/Omni/_sound\n\tEnd\n\tSound %s .4 1000000000 30\n\tFlags Ungroupable\nEnd\n\n",msr->ambientSound);
    fprintf(fout,"Ref grp1\nEnd\n");
    if (msr->ambientSound[0])
        fprintf(fout,"Ref grpsnd2\nEnd");
    fclose(fout);
    eaDestroy(&nodes);
    editLoad(filename,0);
    waitingForLoad=1;
}

int noautomapcheckstats;
int checkingmapstats;
Menu * MGMenu;
int mapsAtATime;
int massProduction;
int massProductionCurentSet;
int massProductionNumEach;
int massProductionMapCheckStatsBuffer;    //it's funny how long this silly variable name is
int massProductionNumToGo;
int massProductionNumValidToGo;
int massProductionOnlySet;
int waitingForMapcheckstats;

void initGenerators2();
MGNode * generateMap(const char * mapSet,int randomseed);
void destroyMGNode(MGNode * node);
MGNode * getNthNode(MGNode * node,int n);
double getArea(GroupDef * def);

void generateMapWrapper(MenuEntry * me,ClickInfo * ci) {
    if (mapsAtATime==1) {
        generateMap(me->info,-1);
        return;
    }
    for (massProductionCurentSet=0;massProductionCurentSet<eaSize(&g_rules);massProductionCurentSet++) {
        if (g_rules[massProductionCurentSet]->missionSet==me->info)
            break;
    }
    if (massProductionCurentSet==eaSize(&g_rules))
        return;
    massProduction=1;
    massProductionOnlySet=1;
    massProductionNumEach=mapsAtATime;
    massProductionNumToGo=mapsAtATime*2;
    massProductionNumValidToGo=mapsAtATime;
    massProductionMapCheckStatsBuffer=noautomapcheckstats;
    noautomapcheckstats=0;
}

void cancelMapGenerationMenu(MenuEntry * me,ClickInfo * ci) {
    *((int *)me->info)=0;
}

void changeWorkingDirectory(MenuEntry * me,ClickInfo * ci) {
    char * t;
    strcat(workingDirectory,"/*");
    backSlashes(workingDirectory);
    t=winGetFolderName(workingDirectory,"Choose a new working directory.");
    if (t==NULL)
        return;
    strcpy(workingDirectory,t);
    forwardSlashes(workingDirectory);
    if (strrchr(workingDirectory,'/')!=NULL && *(strrchr(workingDirectory,'/')+1)==0)
        *(strrchr(workingDirectory,'/'))=0;
    estrPrintf(&me->name, "Working Directory: %s (Click to Change)", workingDirectory);
}

void toggleAutoMapCheckStats(MenuEntry * me,ClickInfo * ci) {
    noautomapcheckstats=!noautomapcheckstats;
    if (noautomapcheckstats)
        estrPrintCharString(&me->name, "Don't automatically mapcheckstats new maps (Click to Change)");
    else
        estrPrintCharString(&me->name, "Automatically mapcheckstats new maps (Click to Change)");
}

void toggleMakingVillainsMaps(MenuEntry * me,ClickInfo * ci) {
    makingVillainsMaps=!makingVillainsMaps;
    if (makingVillainsMaps)
        estrPrintCharString(&me->name, "Making City of Villains maps (Click to Change)");
    else
        estrPrintCharString(&me->name, "Making City of Heroes maps (Click to Change)");
}

void sendMapCheckStats(MenuEntry * me,ClickInfo * ci) {
    cmdParse("mapcheckstats 1");
    checkingmapstats=1;
}

void startMassProduction(MenuEntry * me,ClickInfo * ci) {
    static char num[TEXT_DIALOG_MAX_STRLEN];
    int n;
    if( !winGetString("How many of each mapset to make?",num) )
        return;
    n=atoi(num);
    if (n<=0) return;
    massProduction=1;
    massProductionCurentSet=0;
    massProductionNumEach=n;
    massProductionNumToGo=n*2;
    massProductionNumValidToGo=n;
    massProductionMapCheckStatsBuffer=noautomapcheckstats;
    noautomapcheckstats=0;
}

void changeMapsAtATime(MenuEntry * me,ClickInfo * ci) {
    static char num[TEXT_DIALOG_MAX_STRLEN];
    int n;
    if( !winGetString("How many maps to make at a time?",num) )
        return; 
    n=atoi(num);
    if (n<=0) return;
    mapsAtATime=n;
    estrPrintf(&me->name, "Produce %d map%sat a time (Click to Change)",mapsAtATime,mapsAtATime==1?" ":"s ");
}

extern void commandMenuClickFunc(MenuEntry * me,ClickInfo * ci);

void mapGeneratorUpkeep(int * mapGenerator) {
    static int inited=0;
    if (!inited && *mapGenerator) {
        initGenerators2();
        inited=1;
        mapsAtATime=1;
    }
    if (!inited)
        return;
    if (preprocessGenerators())
        return;
    if (*mapGenerator && MGMenu==NULL) {
        int i;
        char buf[256];
        char buf2[256];
        static int thisIsStupidChangeItPlease=1;
        MenuEntry * me;
        MGMenu=newMenu(400,100,500,650,"Mission Generator");
        MGMenu->root->opened=1;
        loadRules();
        sprintf(workingDirectory,game_state.world_name);
        forwardSlashes(workingDirectory);
        if (!strstri(workingDirectory,":/game/data"))
            sprintf(workingDirectory,"C:/game/data/%s",game_state.world_name);
        *(strrchr(workingDirectory,'/'))=0;
        me=addEntryToMenu(MGMenu," ",changeWorkingDirectory,NULL);
        forwardSlashes(workingDirectory);
        estrPrintf(&me->name, "Working Directory: %s (Click to Change)",workingDirectory);
        if (noautomapcheckstats)
            addEntryToMenu(MGMenu,"Don't automatically mapcheckstats new maps (Click to Change)",toggleAutoMapCheckStats,NULL);
        else
            addEntryToMenu(MGMenu,"Automatically mapcheckstats new maps (Click to Change)",toggleAutoMapCheckStats,NULL);
        if (makingVillainsMaps)
            addEntryToMenu(MGMenu,"Making City of Villains maps (Click to Change)",toggleAutoMapCheckStats,NULL);
        else
            addEntryToMenu(MGMenu,"Making City of Heroes maps (Click to Change)",toggleMakingVillainsMaps,NULL);
        addEntryToMenu(MGMenu,"mapcheckstats",sendMapCheckStats,NULL);
        sprintf(buf,"Produce %d map%sat a time (Click to Change)",mapsAtATime,mapsAtATime==1?" ":"s ");
        addEntryToMenu(MGMenu,buf,changeMapsAtATime,NULL);
        addEntryToMenu(MGMenu,"Start Mass Production",startMassProduction,NULL);
        addEntryToMenu(MGMenu,"Done",cancelMapGenerationMenu,mapGenerator);
        me=addEntryToMenu(MGMenu,"Generate:^2Mapset^4Time",NULL,NULL);
        me->opened=1;
        for (i=0;i<eaSize(&g_rules);i++) {
            int j;
            int len=strlen(g_rules[i]->missionSet);
            int writeIndex=0;
            static char missionSets[100][128];    //ugly, but whatever
            buf2[writeIndex++]='^';
            buf2[writeIndex++]='2';
            for (j=0;j<len;j++) {
                buf2[writeIndex++]=g_rules[i]->missionSet[j];
                if (g_rules[i]->missionSet[j+1]>='A' && g_rules[i]->missionSet[j+1]<='Z')
                    buf2[writeIndex++]=' ';
                if ((g_rules[i]->missionSet[j]<'0' || g_rules[i]->missionSet[j]>'9') &&
                    (g_rules[i]->missionSet[j+1]>='0' && g_rules[i]->missionSet[j+1]<='9')) {
                    buf2[writeIndex++]='^';
                    buf2[writeIndex++]='4';
                }
            }
            buf2[writeIndex++]=0;
            sprintf(buf,"Generate:^2Mapset^4Time/%s",buf2);
            strcpy(missionSets[i],g_rules[i]->missionSet);
            addEntryToMenu(MGMenu,buf,generateMapWrapper,missionSets[i]);
        }
    }
    if (!(*mapGenerator) && MGMenu!=NULL) {
        destroyMenu(MGMenu);
        MGMenu=NULL;
    }
    if (noautomapcheckstats) {
        waitingForLoad=0;
        filename[0]=0;
    }
    if (waitingForLoad && filename[0]!=0 && strstri(game_state.world_name,filename) && !editIsWaitingForServer() && !strstri(filename,"valid")) {
        if (massProduction)
            cmdParse("mapcheckstats 0");
        else
            cmdParse("mapcheckstats 1");
        waitingForLoad=0;
        waitingForMapcheckstats=1;
    }
    if (massProduction && waitingForLoad==0 && waitingForMapcheckstats==0) {
        int notvalid=1;
        if (massProductionNumValidToGo==0 || massProductionNumToGo==0) {
            massProductionNumValidToGo=massProductionNumEach;
            massProductionNumToGo=massProductionNumEach*2;
            massProductionCurentSet++;
            if (massProductionCurentSet>=eaSize(&g_rules) || massProductionOnlySet) {
                massProduction=0;
                massProductionOnlySet=0;
                winMsgAlert("Done with mass production.");
                noautomapcheckstats=massProductionMapCheckStatsBuffer;
                return;
            }
        }
        generateMap(g_rules[massProductionCurentSet]->missionSet,-1);
    }
}

//BFS the rooms to number them, so we are guaranteed that higher numbered
//rooms are actually further from the start than lower numbered rooms
void placeRoomMarkers(MGNode * node,MissionSetRules * msr) {
    MGNode ** nodes;
    int i=0;
    eaPush(&nodes,node);
    while (i<eaSize(&nodes)) {
        int j;
        MGNode * temp=nodes[i];
        temp->number=i;
        for (j=0;j<temp->piece->portals;j++) {
            if (j==temp->source)
                continue;
            eaPush(&nodes,temp->adjacent[j]);
        }
        i++;
    }
    eaDestroy(&nodes);
}

void placeExtentsMarkersRecursive(MGNode * node,int * num,Vec2 * min,Vec2 * max) {
    int i;
    for (i=0;i<eaSize((F32 ***)&node->piece->convexHull);i++) {
        Vec3 v;
        Vec3 v2;
        v2[0]=(*(node->piece->convexHull[i]))[0];
        v2[1]=0;
        v2[2]=(*(node->piece->convexHull[i]))[1];
        mulVecMat4(v2,node->mat,v);
        if (v[0]<(*min)[0])
            (*min)[0]=v[0];
        if (v[0]>(*max)[0])
            (*max)[0]=v[0];
        if (v[2]<(*min)[1])
            (*min)[1]=v[2];
        if (v[2]>(*max)[1])
            (*max)[1]=v[2];
    }
    for (i=0;i<node->piece->portals;i++) {
        if (i==node->source)
            continue;
        if (node->piece->type==MG_Elevator && node->adjacent[i]->piece->type==MG_Elevator) {
            node->adjacent[i]->min[0]=node->adjacent[i]->min[1]=1e9;
            node->adjacent[i]->max[0]=node->adjacent[i]->max[1]=1e-9;
            (*num)++;
            node->adjacent[i]->hasExtents=(*num);
            placeExtentsMarkersRecursive(node->adjacent[i],num,&node->adjacent[i]->min,&node->adjacent[i]->max);
            (*num)--;
        } else {
            node->adjacent[i]->hasExtents=0;
            placeExtentsMarkersRecursive(node->adjacent[i],num,min,max);
        }
    }
}

void placeExtentsMarkers(MGNode * node) {
    int num=1;
    node->min[0]=node->min[1]=1e9;
    node->max[0]=node->max[1]=1e-9;
    placeExtentsMarkersRecursive(node,&num,&node->min,&node->max);
    node->hasExtents=1;
}

int mapIsProper(MGNode * node) {
    MGNode * a;
    MGNode * b;
    int i=0;
    int j=1;
    int k;
    b=getNthNode(node,j++);
    while (b!=NULL) {
        int good=1;
        a=getNthNode(node,i++);
        for (k=0;k<a->piece->portals;k++)
            if (a->adjacent[k]==b)
                good=0;
        if (!good) {
            b=getNthNode(node,j++);
            i=0;
            a=getNthNode(node,i);
        }
        if (b!=NULL && piecesOverlap(a,b))
            return 0;
    }
    return 1;
}

MGNode * map;
int lastSeed;
MissionSetRules * lastMsr;

MGNode * generateMap(const char * missionSet,int randomseed) {
    MapSetGenerator * msg=NULL;
    MissionSetRules * msr=NULL;
    CurrentMapStatus cms;
    int i;
    if (map!=NULL)
        destroyMGNode(map);
    map=NULL;
    if (checkingmapstats)
        return NULL;
    if (randomseed==-1)
        randomseed=rand();
    printf("Making a %s map with seed %d\n",missionSet,randomseed);
    srand(randomseed);
    lastSeed=randomseed;
    loadRules();
    for (i=0;i<eaSize(&g_rules);i++) {
        if (stricmp(missionSet,g_rules[i]->missionSet)==0)
            msr=g_rules[i];
    }
    if (msr==NULL)
        return NULL;
    for (i=0;i<eaSize(&g_generators);i++) {
        if (stricmp(msr->mapSet,g_generators[i]->mapSet)==0)
            msg=g_generators[i];
    }
    if (msg==NULL)
        return NULL;
    cms.maxArea=msr->maxArea;
    cms.minArea=msr->minArea;
    cms.depth=0;
    cms.halls=0;
    mapGeneratorClock=clock();
    i=25;
    while (map==NULL && i>0) {
        map=generateMapRecursive(msg,msr,&cms,NULL,-1);
        i--;
    }
    if (map==NULL) {
        printf("No map generated!\n");
        return NULL;
    }
    if (!mapIsProper(map))
        printf("Error!  Pieces overlap!\n");
    printf("Map has area of %f\n",map->area);
//    placeRoomMarkers(map,msr);
    placeExtentsMarkers(map);
    writeMap(map,msr,randomseed);
    lastMsr=msr;
    return map;
}

void destroyMGNode(MGNode * node) {
    int i;
    for (i=0;i<node->piece->portals;i++) {
        if (i==node->source)
            continue;
        destroyMGNode(node->adjacent[i]);
        if (node->hasDoors)
            free(node->doors[i]);    //don't have to destroyMGNode these, they're special cases
    }
    free(node);
}

MGNode * getNthNodeRecursive(MGNode * node,int * n) {
    MGNode * ret=NULL;
    int i;
    if (*n==0)
        return node;
    for (i=0;i<node->piece->portals;i++) {
        if (i==node->source)
            continue;
        (*n)--;
        ret=getNthNodeRecursive(node->adjacent[i],n);
        if (ret!=NULL)
            return ret;
    }
    return NULL;
}

MGNode * getNthNode(MGNode * node,int n) {
    return getNthNodeRecursive(node,&n);
}

void mapGeneratorPacketHandler(Packet * pak) {
    char fullpath[512];
    mapIsValid=pktGetBitsPack(pak,1);
    checkingmapstats=0;
    waitingForMapcheckstats=0;
    sprintf(fullpath,filename);
    remove(fullpath);
    if (massProduction)
        massProductionNumToGo--;
    if (!mapIsValid)
        return;
    if (massProduction)
        massProductionNumValidToGo--;
    sprintf(game_state.world_name,filename);
    if (strrchr(game_state.world_name,'/')!=NULL)
        *(strrchr(game_state.world_name,'/')+1)=0;
    if (makingVillainsMaps)
        strcat(game_state.world_name,"Valid_V");
    else
        strcat(game_state.world_name,"Valid");
    if (!filename || filename[0]==0)
        return;
    strcat(game_state.world_name,strrchr(filename,'/')+1);
    editCmdSaveFile();
    mapIsValid=0;
}
