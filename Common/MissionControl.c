#if CLIENT

#include "MissionControl.h"
#include "group/group.h"
#include "group/groupProperties.h"
#include <utilitieslib/components/StashTable.h>
#include "group/grouptrack.h"
#include <utilitieslib/utils/mathutil.h>
#include "render/renderprim.h"
#include "graphics/camera.h"
#include "cmdparse/cmdgame.h"
#include <utilitieslib/utils/Color.h>
#include "edit/edit_cmd.h"
#include <utilitieslib/utils/cmdoldparse.h>
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/network/netcomp.h>

int MissionControlOn;
extern GroupInfo group_info;
int receivedResponse;

typedef enum RouteType {
    RouteTypePingPong,
    RouteTypeCircle,
    RouteTypeOneWay,
} RouteType;

typedef struct RouteNode {
    GroupEnt * ent;
    Vec3 pos;
} RouteNode;

typedef struct PatrolRoute {
    RouteNode ** route;
    RouteType type;
    int firstBeacon;        //the first few points in route are spawn points, this is the 
                            //index of the first patrol beacon
} PatrolRoute;

typedef struct TriggerVolume {
    Vec3 corner[8];            // vertices of a trigger volume
    Vec3 source;            // the center of that volume
    Vec3 ** targets;        // the spawn to which we will draw a line
} TriggerVolume;

typedef struct NPCBeacon {
    Mat4 mat;
    int * connections;
} NPCBeacon;

PatrolRoute ** routes;
NPCBeacon    ** beacons;
TriggerVolume ** volumes;
int NPCBeaconRadius;
double maxLength;
int needsReprocessing;

void destroyNPCBeacon(void* bData) {
    NPCBeacon * b = (NPCBeacon *)bData;
    eaiDestroy(&b->connections);
    free(b);
}

void destroyTriggerVolume(void* vData) {
    TriggerVolume * v = (TriggerVolume *)vData;
    eaDestroyEx((void ***)(&v->targets),NULL);
}

int MissionControlMode() {
    return game_state.missioncontrol;
}

int alphaAtDistance(double distance,int nearDist,int farDist) {
    nearDist*=game_state.lod_scale;
    farDist*=game_state.lod_scale;
    if (distance<=nearDist)
        return 0xff;
    if (distance>=farDist)
        return 0x00;
    return 0xff-(int)((double)0xff*((double)(distance-nearDist))/(double)(farDist-nearDist));
}

int alphaOnObject(Vec3 cam,Vec3 target,int nearDist,int farDist) {
    return alphaAtDistance(distance3(cam,target),nearDist,farDist);
}

void drawColorArrow(Vec3 start,Vec3 end,U32 colorStart,U32 colorEnd) {
    Vec3 side1,side2;
    Vec3 up,side;
    double length;
    up[0]=0;
    up[1]=1;
    up[2]=0;
    subVec3(end,start,side1);
    crossVec3(side1,up,side2);
    length=sqrt(side2[0]*side2[0]+side2[1]*side2[1]+side2[2]*side2[2]);
    if (length==0)
        length=1;
    if ((colorStart&0xff000000)==0 && (colorEnd&0xff000000)==0)
        return;
    scaleVec3(side2,1.0/length,side2);
    copyVec3(side2,side);
    subVec3(end,start,side1);
    scaleVec3(side1,.3,side1);
    subVec3(end,side1,side1);
    copyVec3(side1,side2);
    addVec3(side1,side,side1);
    subVec3(side2,side,side2);
    drawLine3dColorWidth(start,colorStart,end,colorEnd,2,1);
    drawLine3dColorWidth(side1,colorEnd,end,colorEnd,2,1);
    drawLine3dColorWidth(side2,colorEnd,end,colorEnd,2,1);
}

void drawColorArrowPath(Vec3 start,Vec3 end,U32 colorStart,U32 colorEnd) {
    double dist=distance3(end,start);
    Vec3 dif;
    int i;
    Vec3 curStart,curEnd;
    int numArrows;
    double s;
    Vec3 separator;
    numArrows=(int)(dist/12.0+.5);
    s=dist/(double)numArrows-10.0;
    copyVec3(start,curStart);
    copyVec3(end,curEnd);
    subVec3(end,start,dif);
    scaleVec3(dif,10.0/dist,dif);
    copyVec3(curStart,curEnd);
    scaleVec3(dif,s/10.0,separator);
    for (i=0;i<numArrows;i++) {
        extern CameraInfo cam_info;
        unsigned int alpha1, alpha2;
        double percent=(double)(numArrows-i)/(double)numArrows;
        addVec3(curStart,dif,curEnd);
        alpha1=alphaOnObject(cam_info.cammat[3],curEnd,125,200);
        alpha2=alphaOnObject(cam_info.cammat[3],curStart,125,200);
        drawColorArrow(curStart,curEnd,(alpha2<<24) | (colorStart&0x00ffffff),(alpha1<<24) | (colorStart&0x00ffffff));
        addVec3(curEnd,separator,curStart);
    }
}

//each patrol starts from it's spawn point and then goes to the first patrol beacon
//this is the same for each type
void drawStartingPaths(PatrolRoute * pr,int color) {
    int i;
    for (i=0;i<pr->firstBeacon;i++) {
        pr->route[i]->pos[1]+=.1;
        pr->route[i+1]->pos[1]+=.1;
        drawColorArrowPath(pr->route[i]->pos,pr->route[pr->firstBeacon]->pos,color,color);
        pr->route[i]->pos[1]-=.1;
        pr->route[i+1]->pos[1]-=.1;
    }
}

void drawPingPong(PatrolRoute * pr) {
    int i;
    drawStartingPaths(pr,0x0000ff);
    for (i=pr->firstBeacon;i<eaSize(&pr->route)-1;i++) {
        pr->route[i]->pos[1]+=.1;
        pr->route[i+1]->pos[1]+=.1;
        drawColorArrowPath(pr->route[i]->pos,pr->route[i+1]->pos,0x0000ff,0x0000ff);
        pr->route[i]->pos[1]-=.1;
        pr->route[i+1]->pos[1]-=.1;
    }
}

void drawCircle(PatrolRoute * pr) {
    int i;
    drawStartingPaths(pr,0x00ff00);
    for (i=pr->firstBeacon;i<eaSize(&pr->route);i++) {
        int next=(i+1)%eaSize(&pr->route);
        if (next==0) next=pr->firstBeacon;
        pr->route[i]->pos[1]+=.1;
        pr->route[next]->pos[1]+=.1;
        drawColorArrowPath(pr->route[i]->pos,pr->route[next]->pos,0x00ff00,0x00ff00);
        pr->route[i]->pos[1]-=.1;
        pr->route[next]->pos[1]-=.1;
    }
}

void drawOneWay(PatrolRoute * pr) {
    int i;
    drawStartingPaths(pr,0xff0000);
    for (i=pr->firstBeacon;i<eaSize(&pr->route)-1;i++) {
        pr->route[i]->pos[1]+=.1;
        pr->route[i+1]->pos[1]+=.1;
        drawColorArrowPath(pr->route[i]->pos,pr->route[i+1]->pos,0xff0000,0xff0000);
        pr->route[i]->pos[1]-=.1;
        pr->route[i+1]->pos[1]-=.1;
    }
}

void drawPatrolRoute(PatrolRoute * pr) {
    if (pr->type==RouteTypePingPong)
        drawPingPong(pr);
    else
    if (pr->type==RouteTypeCircle)
        drawCircle(pr);
    else
    if (pr->type==RouteTypeOneWay)
        drawOneWay(pr);

}

void drawBeacons() {
    int i;
    int sides;
    Vec3 t,temp,start,end,buf;
    int color;
    extern CameraInfo cam_info;
     for (i=0;i<eaSize(&beacons);i++) {
        int j;
        int alpha=alphaOnObject(cam_info.cammat[3],beacons[i]->mat[3],115,159+NPCBeaconRadius);
        sides=alpha/10+10;
        if (alpha>0) {
             for (j=0;j<sides;j++) {
                int alpha1,alpha2;
        //        scaleVec3(beacons[i]->mat[0],beacons[i]->radius,t);
                copyVec3(beacons[i]->mat[3],t);
                copyVec3(beacons[i]->mat[3],start);
                copyVec3(beacons[i]->mat[3],end);
                start[0] += NPCBeaconRadius*cos(((double)j/(double)sides)*2*3.1415926535);
                start[2] += NPCBeaconRadius*sin(((double)j/(double)sides)*2*3.1415926535);
                end[0] += NPCBeaconRadius*cos(((double)(j+1)/(double)sides)*2*3.1415926535);
                end[2] += NPCBeaconRadius*sin(((double)(j+1)/(double)sides)*2*3.1415926535);
                alpha1=alphaOnObject(cam_info.cammat[3],start,115,159+NPCBeaconRadius);
                alpha2=alphaOnObject(cam_info.cammat[3],end,115,159+NPCBeaconRadius);

                copyVec3(start,buf);
                subVec3(start, t, temp);
                mulVecMat3(temp, beacons[i]->mat,start);
                addVec3(start, t,start);

                copyVec3(end,buf);
                subVec3(end, t, temp);
                mulVecMat3(temp, beacons[i]->mat,end);
                addVec3(end, t,end);

                if (beacons[i]->connections!=NULL)
                    color=0x000000ff;
                else
                    color=0x00ff0000;
                drawLine3dColorWidth(start,color | (alpha1 << 24),end,color | (alpha2 << 24),2,1);
            }
        }
        if (alphaAtDistance(distance3(cam_info.cammat[3],beacons[i]->mat[3])-maxLength,115,159+NPCBeaconRadius)>0) {
            for (j=0;j<eaiSize(&beacons[i]->connections);j++) {
                int alpha1=alphaOnObject(cam_info.cammat[3],beacons[i]->mat[3],115,159+NPCBeaconRadius);
                int alpha2=alphaOnObject(cam_info.cammat[3],beacons[beacons[i]->connections[j]]->mat[3],115,159+NPCBeaconRadius);
                drawLine3dColorWidth(beacons[i]->mat[3],0x7f00ff+(alpha1<<24),beacons[beacons[i]->connections[j]]->mat[3],0x7f00ff+(alpha2<<24),2,1);
            }
        }
    }
    for (i=0;i<eaSize(&volumes);i++) {
        static int p[8];    // pop count
        int j,k;
        int alpha1=alphaAtDistance(distance3(cam_info.cammat[3],volumes[i]->source),95,259);
        int alpha2;
        int a[8];
        if (alphaAtDistance(distance3(cam_info.cammat[3],volumes[i]->source)-25,95,259)==0)
            continue;
        if (p[1]==0) {
            for (i=0;i<8;i++)
                p[i]=((i&1)?1:0)+((i&2)?1:0)+((i&4)?1:0);
        }
        if (eaSizeUnsafe(&volumes[i]->targets)) {
            alpha2=alphaAtDistance(distance3(cam_info.cammat[3],*volumes[i]->targets[0]),95,259);
            drawLine3dColorWidth(volumes[i]->source,0xff000f | (alpha1<<24),*(volumes[i]->targets[0]),0xff000f | (alpha2<<24),2,1);
        }
        for (j=0;j<=0/*eaSize((&volumes[i]->targets))*/;j++) {
        }
        for (j=0;j<8;j++)
            a[j]=alphaAtDistance(distance3(cam_info.cammat[3],volumes[i]->corner[j]),95,259);
        for (j=0;j<8;j++)
        for (k=j+1;k<8;k++) {
            if (p[(j^k)]==1)
                drawLine3dColorWidth(volumes[i]->corner[j],0xffffff | (a[j]<<24),volumes[i]->corner[k],0xffffff | (a[k]<<24),2,1);
        }
    }
}

void handlePatrolsOrBeaconsInDef(GroupDef * def,Mat4 mat) {
    int i;
    int opened=0;
    extern CameraInfo cam_info;
    Mat4 camera;
    copyMat4(cam_info.cammat,camera);
    if (0 && (strcmp(def->name,"NAV")==0 || strcmp(def->name,"_NAV")==0)) {
        NPCBeacon * npcb=malloc(sizeof(NPCBeacon));
        copyMat4(mat,npcb->mat);
        eaPush(&beacons,npcb);
    }
    for (i=0;i<def->count;i++) {
        Mat4 childMat;
        Mat4 tempMat;
        if (strstriConst(def->entries[i].def->name,"vol_trigger_spawn")) {
            char buf[256];
            int a,b,c,j;
            float width,height;
            TriggerVolume * tv=malloc(sizeof(TriggerVolume));
            Mat4 m;
            tv->targets=NULL;
            sprintf(buf,"%s",def->entries[i].def->name+strlen("vol_trigger_spawn"));
            for (j=0;j<(int)strlen(def->entries[i].def->name);j++)
                if (buf[j]=='_')
                    buf[j]=' ';
            sscanf(buf,"%f %f",&width,&height);
            width=width;
            j=0;
            mulMat4(mat,def->entries[i].mat,m);
            for (a=0;a<2;a++)
            for (b=0;b<2;b++)
            for (c=0;c<2;c++,j++) {
                Vec3 v;
                v[0]=(a?width:-width)/2.0;
                v[1]=(b?height:0);
                v[2]=(c?width:-width)/2.0;
                mulVecMat4(v,m,tv->corner[j]);
                eaPush(&volumes,tv);
            }
            copyVec3(m[3],tv->source);
            tv->source[1]+=height/2;
            for (j=0;j<def->count;j++) {
                Vec3 * v;
                if (j==i) continue;
                v=malloc(sizeof(Vec3));
                mulMat4(mat,def->entries[j].mat,m);
                copyVec3(m[3],*v);
                if (strstriConst(def->entries[j].def->name,"Encounter"))
                    eaPush((void ***)(&tv->targets),*v);
            }
        }
        if (def->entries[i].def->has_properties) {
            PropertyEnt * prop;
            stashFindPointer( def->entries[i].def->properties,"RouteType", &prop );
            if (prop!=NULL) {
                //here we have a patrol of some kind, so we want to find all the
                //markers for this patrol, and then draw the appropriate arrows
                //between them
                int j;
                int spawnPoint=0;
                RouteNode* rn = NULL;
                PatrolRoute* pr = malloc(sizeof(PatrolRoute));
                if (strcmp(prop->value_str,"PingPong")==0)
                    pr->type=RouteTypePingPong;
                else
                if (strcmp(prop->value_str,"Circle")==0)
                    pr->type=RouteTypeCircle;
                else
                if (strcmp(prop->value_str,"OneWay")==0)
                    pr->type=RouteTypeOneWay;
                else {    //not a known patrol type
                    free(rn);
                    free(pr);
                    break;
                }
                pr->route=NULL;
                eaCreate(&pr->route);
                for (j=0;j<def->count;j++) {
                    if (stashFindPointerReturnPointer(def->entries[j].def->properties,"EncounterPosition")) {
                        rn=malloc(sizeof(RouteNode));
                        rn->ent=&def->entries[j];
                        mulMat4(mat,def->entries[j].mat,tempMat);
                        copyVec3(tempMat[3],rn->pos);
                        eaPush(&pr->route,rn);
                        spawnPoint++;
                    }
                }
                pr->firstBeacon=spawnPoint;
                rn=malloc(sizeof(RouteNode));
                rn->ent=&def->entries[i];
                mulMat4(mat,def->entries[i].mat,tempMat);
                copyVec3(tempMat[3],rn->pos);
                eaPush(&pr->route,rn);
                //we already have the first patrol point, so start checking
                //for the second one
                for (j=2;j<=def->count;j++) {
                    int k;
                    for (k=0;k<def->count;k++) {
                        int value;
                        stashFindPointer( def->entries[k].def->properties,"PatrolPoint", &prop );
                        if (prop==NULL)
                            continue;
                        sscanf(prop->value_str,"%d",&value);
                        if (value==j) {
                            RouteNode * rn=malloc(sizeof(RouteNode));
                            rn->ent=&def->entries[k];
                            mulMat4(mat,def->entries[k].mat,tempMat);
                            copyVec3(tempMat[3],rn->pos);
                            eaPush(&pr->route,rn);
                        }
                    }
                }
                eaPush(&routes,pr);
            }
        }
        mulMat4(mat,def->entries[i].mat,childMat);
        handlePatrolsOrBeaconsInDef(def->entries[i].def,childMat);
    }
}

void handlePatrolsAndBeacons() {
    int i;

    if (routes==NULL) {
        eaCreate(&routes);
//        eaCreate(&beacons);
        for (i=0;i<group_info.ref_count;i++) {
            if (group_info.refs[i]==NULL || group_info.refs[i]->def==NULL)
                continue;
            handlePatrolsOrBeaconsInDef(group_info.refs[i]->def,group_info.refs[i]->mat);
        }
    }

    for (i=0;i<eaSize(&routes);i++)
        drawPatrolRoute(routes[i]);
    drawBeacons();
}

//this needs to get called if things have changed that mission control deals with,
//so it can update paths and whatnot
void MissionControlReprocess() {
    //we need to ask the mapserver for some stuff first, so do that,
    //when we hear back we will reprocess things
    if (!game_state.missioncontrol) {
        needsReprocessing=1;
        return;
    }
    needsReprocessing=0;
    cmdParse("beaconprocessnpc");
    cmdParse("servermissioncontrol 1");
}

void MissionControlMain() {
    if (!game_state.missioncontrol)
        return;
    if (needsReprocessing)
        MissionControlReprocess();
    handlePatrolsAndBeacons();
}

//delete everything, it will get reprocessed the next time we run through
//the main loop
void MissionControlHandleResponse(Packet * pak) {
    //delete patrol routes so they get reprocessed
    int i;
    int numBeacons;
    NPCBeaconRadius=pktGetBitsPack(pak,1);
    numBeacons=pktGetBitsPack(pak,1);
    if (beacons) {
        eaDestroyEx(&beacons,destroyNPCBeacon);
        eaDestroyEx(&volumes,destroyTriggerVolume);
        beacons=NULL;
        volumes=NULL;
    }
    eaCreate(&beacons);
    for (i=0;i<numBeacons;i++) {
        int j,k;
        NPCBeacon * b=malloc(sizeof(NPCBeacon));
        int index;
        for (j=0;j<4;j++)
            for (k=0;k<4;k++)
                b->mat[j][k]=pktGetF32(pak);
        b->mat[0][0]=1;
        b->mat[0][1]=0;
        b->mat[0][2]=0;
        b->mat[1][0]=0;
        b->mat[1][1]=1;
        b->mat[1][2]=0;
        b->mat[2][0]=0;
        b->mat[2][1]=0;
        b->mat[2][2]=1;
        index=pktGetBitsPack(pak,1);
        b->connections=NULL;
        if (index)
            eaiCreate(&b->connections);
        else
            b->connections=NULL;
        while (index) {
            eaiPush(&b->connections,index);
            index=pktGetBitsPack(pak,1);
        }
        eaPush(&beacons,b);
    }

    //a little bit of post-processing, now that we have all of our information
    maxLength=0;
    for (i=0;i<eaSize(&beacons);i++) {
        int j;
        for (j=0;j<eaiSize(&beacons[i]->connections);j++) {
            double t=distance3(beacons[i]->mat[3],beacons[beacons[i]->connections[j]]->mat[3]);
            if (t>maxLength)
                maxLength=t;

            //this is so we know that a beacon has connections, even if it doesn't
            //know about it, so we can mark orphaned beacons
            if (beacons[beacons[i]->connections[j]]->connections==NULL)
                eaiCreate(&beacons[beacons[i]->connections[j]]->connections);
        }
    }
    for (i=0;i<eaSize(&routes);i++)
        eaClearEx(&routes[i]->route,NULL);
    eaDestroy(&(void **)routes);
    routes=NULL;
}

#endif

/******************************************************************/

#if SERVER

#include "svr/svr_base.h"
#include "entity/entity.h"
#include "comm_game.h"
#include "dbcomm/staticMapInfo.h"
#include "dbcomm/dbcomm.h"
#include <utilitieslib/components/ArrayOld.h>
#include <utilitieslib/network/netcomp.h>
#include "beacon/beacon.h"
#include "beacon/beaconPrivate.h"

extern Array basicBeaconArray;

void MissionControlHandleRequest(ClientLink* client, Entity* player, int sendpopup) {
    int radius=0;
    const MapSpec * spec;
    int i;
    START_PACKET(pak,player,SERVER_MISSION_CONTROL);
        spec=MapSpecFromMapId(db_state.base_map_id);
        if(!spec)
            spec=MapSpecFromMapName(db_state.map_name);
        if (spec)
            radius=spec->NPCBeaconRadius;
        if(!radius)
            radius=3;
        pktSendBitsPack(pak,1,radius);
        pktSendBitsPack(pak,1,basicBeaconArray.size);
        for (i=0;i<basicBeaconArray.size;i++) {
            int j,k;
            //first send the position of the beacon
            for (j=0;j<4;j++)
                for (k=0;k<4;k++)
                    pktSendF32(pak,((Beacon *)basicBeaconArray.storage[i])->mat[j][k]);
            //now for each of its connection beacons...
            for (j=0;j<((Beacon *)basicBeaconArray.storage[i])->gbConns.size;j++) {
                int k;
            //find out what it's index is in the main array and send that index
                for (k=i+1;k<basicBeaconArray.size;k++) {
                    if (((BeaconConnection *)((Beacon *)basicBeaconArray.storage[i])->gbConns.storage[j])->destBeacon==((Beacon *)basicBeaconArray.storage[k])) {
                        pktSendBitsPack(pak,1,k);
                        break;
                    }
                }
            }
            //since we only send indices greater than i (current beacon's index) we can send 0 as
            //the stopping value.  This also prevents us from duplicating all of the connections
            pktSendBitsPack(pak,1,0);
        }
    END_PACKET;
}

#endif

