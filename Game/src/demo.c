#include <stdarg.h>
#include <utilitieslib/stdtypes.h>
#include "entity/entrecv.h"
#include "graphics/sun.h"
#include "bases/baseclientrecv.h"
#include "bases/basetogroup.h"
#include "bases/basedata.h"
#include "bases/baseedit.h"
#include <utilitieslib/components/EString.h>
#include <stdio.h>
#include <string.h>
#include "demo.h"
#include <utilitieslib/utils/file.h>
#include <utilitieslib/utils/utils.h>
#include "group/groupnetrecv.h"
#include "entity/entity.h"
#include "entity/entclient.h"
#include "entity/costume_client.h"
#include "gameComm/Npc.h"
#include "varutils.h"
#include <utilitieslib/utils/timing.h>
#include "seq/seqsequence.h"
#include <utilitieslib/components/StringCache.h>
#include "entity/entrecv.h"
#include "win/win_init.h"
#include "cmdparse/cmdcommon.h"
#include "UI/uiAutomap.h"
#include "graphics/camera.h"
#include "graphics/font.h"
#include "cmdparse/cmdgame.h"
#include "win/input.h"
#include "UI/uiGame.h"
#include "player/player.h"
#include "gameComm/Npc.h"
#include <utilitieslib/components/earray.h>
#include "graphics/gfx.h"
#include "graphics/gfxDebug.h"
#include "UI/uiChat.h"
#include "clientcomm/clientcommreceive.h"
#include "player/playerState.h"
#include "render/renderUtil.h"
#include "graphics/gfx.h"
#include "graphics/gfxSettings.h"
#include "UI/uiConsole.h"
#include <utilitieslib/utils/FolderCache.h>
#include "entity/netfx.h"
#include "render/tex.h"
#include "render/texUnload.h"
#include "entity/character_base.h"
#include "seq/anim.h"
#include "edit/edit_cmd.h"
#include "graphics/gfxLoadScreens.h"
#include "entity/origins.h"
#include "entity/entDebug.h"
#include "entity/entDebugPrivate.h"
#include "UI/uiInput.h"
#include "gameComm/itemselect.h"
#include <utilitieslib/utils/cpu_count.h>
#include "group/groupdyn.h"
#include "UI/uiOptions.h"

#ifndef TEST_CLIENT
#include "bases/bases.h"
#include "bases/baseparse.h"
#endif

extern Entity* current_target;

static FILE *demo_file;
static int record_ent_idx;
static int demo_is_playing = 0;
static U32 record_abs_time,offset_abs_time;
static U32 abstime_start,abstime_end,play_abs_time,last_play_abs_time;
static S32 noTimeAdvance;

DemoState demo_state;

#define DEMO_CAMERA_ID -2
#define DYN_GROUP_UPDATE -3
#define SKY_FILE_UPDATE -4
#define DEMO_FILE_VERSION 2

// We can't be using ALL the bits, so this is how we signify backwards
// compatibility.
#define DEMO_MOVE_BITS_UNUSED 0xffffffff

typedef struct
{
    char    *geom;
    char    *texnames[2];
    char    *fxname;
    int        colors[4];
} DemoCostumePart;

typedef struct
{
    // Player
    int            bodytype;
    int            color_skin;
    F32            scales[MAX_BODY_SCALES];
    int            num_parts;
    DemoCostumePart    *parts;
    // NPC
    int            npc_index;
    int            costume_index;
    char*        seq_name;
} DemoCostume;

DemoChatType demochattype_Validate( DemoChatType e )
{
    return INRANGE0(e, kDemoChatType_Count) ? e : kDemoChatType_Normal;
}


typedef struct DemoChat
{
    char            *msg;
    INFO_BOX_MSGS    type;
    int                number;
    U32                floater : 1;
} DemoChat;

typedef struct
{
    int        receiver;
    int        amount;
    char    *msg;
} DemoDmg;

typedef struct
{
    Vec3        pos;
    Vec3        pyr;
    U32            created : 1;
    U32            deleted : 1;
    U32            has_costume : 1;
    U32            has_pos : 1;
    U32            has_pyr : 1;
    U32            xlucency : 8;
    char        *move;
    U32            move_bits;
    char        *name;
    U32            abs_time;
    DemoCostume    *costume;
    NetFx        *fx_list;
    int            fx_count;
    DemoChat    *chat_list;
    int            chat_count;
    DemoDmg        *dmg_list;
    int            dmg_count;
    int            unique_id;
    F32            hp;
    F32            hpMax;
    char        *costumeWorldGroup;
    char        *dynGroupUpdate; //not for entities
    int            sky1;
    int            sky2;
    F32            skyWeight;
    int            num_bones; // ragdoll
    int            *ragdollCompressed;
    U32            ragdoll_svr_time;
    U32            ragdoll_client_time;
    U32            ragdoll_svr_dt;
} EntFrame;

typedef struct
{
    int            count;
    int            max;
    EntFrame    *frames;
    int            ent_idx;
    int            last_frame;
    Vec3        last_pyr;
    Vec3        last_pos;
} EntTrack;

typedef struct
{
    U32        abs_time;
    U32        ent_id;
    U32        str_idx;
} DemoRecord;

typedef struct
{
    int        first_frame;
    int        player_id;
    F32        clock_time;
    char    map_name[MAX_PATH];
} Demo;

typedef struct EntityDemoRecordInfo
{
    struct{
        F32            fHitPoints;
        F32            fMaxHitPoints;
    } previous;
} EntityDemoRecordInfo;

DemoRecord    *demo_records;
char        *demo_text;
int            demo_record_count,demo_record_max,demo_text_count,demo_text_max;

static int        enttrack_count,enttrack_max;
static EntTrack    *enttracks;

static Demo demo;

MP_DEFINE(EntityDemoRecordInfo);

static EntityDemoRecordInfo* demoGetEntityRecordInfo(Entity* e)
{
    if(!e->demoRecordInfo)
    {
        MP_CREATE(EntityDemoRecordInfo, 100);
        
        e->demoRecordInfo = MP_ALLOC(EntityDemoRecordInfo);
        
        e->demoRecordInfo->previous.fHitPoints = -1;
        e->demoRecordInfo->previous.fMaxHitPoints = -1;
    }
    
    return e->demoRecordInfo;
}

void demoFreeEntityRecordInfo(Entity* e)
{
    if(e && e->demoRecordInfo)
    {
        MP_FREE(EntityDemoRecordInfo, e->demoRecordInfo);
        e->demoRecordInfo = NULL;
        
        if(mpGetAllocatedCount(MP_NAME(EntityDemoRecordInfo)) == 0)
        {
            mpFreeAllocatedMemory(MP_NAME(EntityDemoRecordInfo));
        }
    }
}

int demoIsPlaying()
{
    return demo_is_playing;
}

int demoIsRecording()
{
    return demo_file ? 1 : 0;
}

void demoRecord(char const *fmt, ...)
{
    char        *str = NULL;
    char        *final = NULL;
    char *s,*args[100];
    va_list        ap;
    DemoRecord    *record;
    int            i,count;

    if (!demo_file)
        return;
    estrCreate(&str);

    va_start(ap, fmt);
    estrConcatfv(&str, fmt, ap);
    va_end(ap);

    estrReserveCapacity(&final,estrLength(&str));

    count = tokenize_line(str,args,0);
    for(i=0;i<count;i++)
    {
        if (strchr(args[i],' ') || strlen(args[i])==0)
            estrConcatf(&final,"\"%s\"",args[i]);
        else
            estrConcatCharString(&final, args[i]);
        if (i!=count-1) {
            estrConcatChar(&final, ' ');
        }
    }
    record = dynArrayAdd(&demo_records,sizeof(demo_records[0]),&demo_record_count,&demo_record_max,1);
    s = dynArrayAdd(&demo_text,1,&demo_text_count,&demo_text_max,strlen(final)+1);
    strcpy_unsafe(s,final);
    record->str_idx = s - demo_text;
    record->abs_time = record_abs_time + offset_abs_time;
    record->ent_id = record_ent_idx;
 
    estrDestroy(&final);
    estrDestroy(&str);
}

static int __cdecl cmpDemoRecords(const void* aData, const void* bData)
{
    const DemoRecord * a = (const DemoRecord *)aData;
    const DemoRecord * b = (const DemoRecord *)bData;
    int        a_id,b_id;

    if (a->abs_time != b->abs_time)
        return a->abs_time - b->abs_time;
    a_id = a->ent_id;
    b_id = b->ent_id;
    if (a_id == DEMO_CAMERA_ID)
        a_id = 0;
    if (b_id == DEMO_CAMERA_ID)
        b_id = 0;
    if (a_id == DYN_GROUP_UPDATE)
        a_id = 0;
    if (b_id == DYN_GROUP_UPDATE)
        b_id = 0;
    if (a_id == SKY_FILE_UPDATE)
        a_id = 0;
    if (b_id == SKY_FILE_UPDATE)
        b_id = 0;
    if (a_id != b_id)
        return a_id - b_id;
    return a->str_idx - b->str_idx;
}

static void dumpDemoToFile()
{
    int            i,msecs,last_msec=0,dt;
    DemoRecord    *record;

    qsort(demo_records,demo_record_count,sizeof(demo_records[0]),cmpDemoRecords);
    for(i=0;i<demo_record_count;i++)
    {
        record = &demo_records[i];
        msecs = ABSTIME_TO_MSECS(record->abs_time - demo_records[0].abs_time)+1;
        dt = msecs - last_msec;
        last_msec = msecs;
        if (record->ent_id == DEMO_CAMERA_ID)
            fprintf(demo_file,"%-3d CAM %s\n",dt,record->str_idx + demo_text);
        else if (record->ent_id == DYN_GROUP_UPDATE)
            fprintf(demo_file,"%-3d DYNGROUPS %s\n",dt,record->str_idx + demo_text);
        else if (record->ent_id == SKY_FILE_UPDATE)
            fprintf(demo_file,"%-3d SKYFILE %s\n",dt,record->str_idx + demo_text);
        else
            fprintf(demo_file,"%-3d %-3d %s\n",dt,record->ent_id,record->str_idx + demo_text);
    }
    free(demo_records);
    demo_records = 0;
    demo_record_count = demo_record_max = 0;
    free(demo_text);
    demo_text = 0;
    demo_text_count = demo_text_max = 0;
}

#define bufVec3(v,buf) bufVec3_s(v, buf, ARRAY_SIZE_CHECKED(buf))
static char *bufVec3_s(const Vec3 v, char *buf, int buf_size)
{
    char    a[100],b[100],c[100];

    sprintf_s(buf, buf_size, "%s %s %s",safe_ftoa(v[0],a),safe_ftoa(v[1],b),safe_ftoa(v[2],c));
    return buf;
}

static char *printColor(U32 color,char *buf)
{
    if ((color >> 24) != 0xff)
        sprintf(buf,"%08x",color);
    else
        sprintf(buf,"%06x",color & 0xffffff);
    return buf;
}

static U32 getColor(char *s)
{
    U32    color = strtoul(s,0,16);
    if (!(color >> 24))
        color |= 0xff000000;
    return color;
}

void demoSetEntIdx(int ent_idx)
{
    record_ent_idx = ent_idx;
}

void demoSetAbsTime(U32 abs_time)
{
    record_abs_time = abs_time;
}

U32 demoGetAbsTime()
{
    return record_abs_time;
}

void demoSetAbsTimeOffset(int abs_time_offset)
{
    offset_abs_time = abs_time_offset;
}

void demoRecordAppearance(Entity *e)
{
    if (!demo_file)
        return;
    if (e->npcIndex)
    {
        const NPCDef *def = eaGetConst( &npcDefList.npcDefs, e->npcIndex );

        if (!e->costumeIndex)
            demoRecord( "NPC \"%s\"", def->name );
        else
            demoRecord( "NPC \"%s\" %d", def->name,e->costumeIndex );
    }
    else if (e->costume && e->costume->appearance.iNumParts)
    {
        const cCostume *costume = e->costume;
        int        i;
        char    cbuf1[100],cbuf2[100];
        char    buf[1000];
        char*    pos = buf;

        pos += sprintf(    pos,
                        "COSTUME %d %s",
                        costume->appearance.bodytype,
                        printColor(costume->appearance.colorSkin.integer,cbuf1));
        for(i=0;i<MAX_BODY_SCALES;i++)
            pos += sprintf(pos, " %f", costume->appearance.fScales[i]);
        demoRecord("%s", buf);
        for(i=0; i < costume->appearance.iNumParts; i++)
        {
            const CostumePart *part = costume->parts[i];
            sprintf(buf, " PARTSNAME \"%s\" \"%s\" \"%s\" %s %s",
                part->pchGeom,part->pchTex1,part->pchTex2,
                printColor(part->color[0].integer,cbuf1),printColor(part->color[1].integer,cbuf2));
            if (!part->pchFxName || !part->pchFxName[0] || stricmp(part->pchFxName, "none")==0) {
                // Only 2 colors, no FX
            } else {
                strcatf(buf, " %s %s \"%s\"",
                    printColor(part->color[2].integer,cbuf1), printColor(part->color[3].integer,cbuf2), part->pchFxName);
            }
            demoRecord("%s", buf);
        }
    }
    else
    {
        demoRecord("SEQ %s",e->seq->type->name);
    }

    if( e->costumeWorldGroup )
    {
        demoRecord("DYNLIB %s",e->costumeWorldGroup);  
    }

    if (e->xlucency != 1.f)
        demoRecord("XLU %f",e->xlucency);
}

void demoRecordStatChanges(Entity* e)
{
    EntityDemoRecordInfo* info;
    Character* pchar;
    
    if(!demo_file || !e || !e->pchar)
    {
        return;
    }
    
    info = demoGetEntityRecordInfo(e);
    pchar = e->pchar;
    
    if(pchar->attrCur.fHitPoints != info->previous.fHitPoints)
    {
        info->previous.fHitPoints = pchar->attrCur.fHitPoints;
        demoRecord("HP %1.2f", info->previous.fHitPoints);
    }

    if(pchar->attrMax.fHitPoints != info->previous.fMaxHitPoints)
    {
        info->previous.fMaxHitPoints = pchar->attrMax.fHitPoints;
        demoRecord("HPMAX %1.2f", info->previous.fMaxHitPoints);
    }
}

void demoRecordFx(NetFx *netfx)
{
    if(demo_file)
    {
        char    *cmdname="";

        if (netfx->command == CREATE_ONESHOT_FX)
            cmdname = "OneShot";
        else if (netfx->command == CREATE_MAINTAINED_FX)
            cmdname = "Maintained";
        if (netfx->command == DESTROY_FX)
            demoRecord("FXDESTROY %d",netfx->net_id);
        else
        {
            demoRecord("FX %s %d %s %d",
                cmdname,netfx->net_id,stringFromReference(netfx->handle),netfx->clientTriggerFx);
            if (netfx->radius || netfx->power)
                demoRecord(" FXSCALE %f %d",netfx->radius,netfx->power);
            if (netfx->debris)
                demoRecord(" FXDEBRIS %d",netfx->debris);

            if( netfx->hasColorTint )
                demoRecord(" FXTINT %d %d",netfx->colorTint.primary.integer,netfx->colorTint.secondary.integer);

            if( netfx->originType == NETFX_POSITION )
                demoRecord(" ORIGIN POS %f %f %f",netfx->originPos[0],netfx->originPos[1],netfx->originPos[2]);
            else if ( netfx->originType == NETFX_ENTITY )
                demoRecord(" ORIGIN ENT %d %d",netfx->originEnt,netfx->bone_id);

            if( netfx->targetType == NETFX_POSITION )
                demoRecord(" TARGET POS %f %f %f",netfx->targetPos[0],netfx->targetPos[1],netfx->targetPos[2]);
            else if( netfx->targetType == NETFX_ENTITY )
                demoRecord(" TARGET ENT %d %d",netfx->targetEnt,0);

        }
    }
}

void demoRecordMove(Entity *e,int move_idx,int move_bits)
{
    if(!demo_file)
        return;
    if (!e || !e->seq || !e->seq->info  || !e->seq->info->moves || move_idx >= eaSize(&e->seq->info->moves))
        return;
    demoRecord("MOV \"%s\" %d",e->seq->info->moves[move_idx]->name, move_bits);
}

void demoRecordPos(const Vec3 pos)
{
    if(demo_file)
    {
        char        buf[1000];
        Entity *e = playerPtr();
        if (control_state.predict && e && e->svr_idx == record_ent_idx)
            return;
        demoRecord("POS %s",bufVec3(pos,buf));
    }
}

void demoRecordPyr(Vec3 pyr)
{
    if(demo_file)
    {
        char        buf[1000];
        Entity *e = playerPtr();

        if (control_state.predict && e && e->svr_idx == record_ent_idx)
            return;
        demoRecord("PYR %s",bufVec3(pyr,buf));
    }
}

static Vec3 last_cam_pyr,last_cam_pos,last_plr_pyr,last_plr_pos;

void demoRecordClientInfo()
{
    Entity    *e = playerPtr();
    Vec3    pyr;
    const F32    *pos;
    static    int timer;
    char    buf[1000];
    extern F32     auto_delay;

    if(!demo_file)
        return;
    if (!e)
        return;
    if (!timer)
        timer=timerAlloc();
    if (timerElapsed(timer) < 1/60.f)
        return;
    timerStart(timer);
    getMat3YPR(ENTMAT(e),pyr);
    pos = ENTPOS(e);

    if (control_state.predict)
    {
        if (!demo.first_frame)
        {
            //xyprintf(10,10,"latency %d",global_state.client_abs_latency);
            demoSetAbsTime(global_state.client_abs);
            demoSetAbsTimeOffset(global_state.client_abs_latency + auto_delay * ABSTIME_SECOND);
        }
        demoSetEntIdx(e->svr_idx);
        if (!nearSameVec3(last_plr_pos,pos))
        {
            demoRecord("POS %s",bufVec3(pos,buf));
            copyVec3(pos,last_plr_pos);
        }
        if (!nearSameVec3Tol(last_plr_pyr,pyr,0))
        {
            demoRecord("PYR %s",bufVec3(pyr,buf));
            copyVec3(pyr,last_plr_pyr);
        }
    }
    demoRecordCamera(cam_info.cammat);
    //demoRecordDynGroups();

    demoSetAbsTimeOffset(0);
}

void demoRecordDelete()
{
    if(!demo_file)
        return;
        
    demoRecord("DEL");
}

void demoRecordCreate(char *name)
{
    if(!demo_file)
        return;
        
    demoRecord("NEW \"%s\"",name ? name : "");
}


static void invalidateLastCameraPos()
{
    setVec3(last_cam_pyr,9999,9999,9999);
    setVec3(last_cam_pos,9999,9999,9999);
    setVec3(last_plr_pos,9999,9999,9999);
    setVec3(last_plr_pyr,9999,9999,9999);
}

void demoRecordDynGroups( void )
{
    extern int            dyn_group_count;
    extern DynGroupStatus    *dyn_group_status; 
    int i;
    static char * str;
    int strSize = 50 + dyn_group_count * 10; //arbitrarily plenty big
    
    if( dyn_group_count )
    {
        str = realloc( str, strSize ); 
        memset( str, 0, strSize );

        demoSetEntIdx( DYN_GROUP_UPDATE );
        strcat_s( str, strSize, "DYNARRAY " );
        for( i = 0 ; i < dyn_group_count ; i++ )
        {
            char buf[100];
            sprintf( buf, "|%d,%d", dyn_group_status[i].hp, dyn_group_status[i].repair );
            strcat_s( str, strSize, buf );
        }
        demoRecord( str );
    }
}

void demoRecordSkyFile(int sky1, int sky2, F32 weight)
{
    char str[100];
    int strSize = 100; //arbitrarily plenty big
    
    demoSetEntIdx( SKY_FILE_UPDATE );
    sprintf( str, "SKY %d %d %f", sky1, sky2, weight );
    demoRecord( str );
}

void demoRecordCamera(Mat4 mat)
{
    Vec3    pyr;
    F32        *pos = mat[3];

    demoSetEntIdx(DEMO_CAMERA_ID);
    getMat3YPR(mat,pyr);
    if (!nearSameVec3(last_cam_pos,pos))
    {
        demoRecordPos(pos);
        copyVec3(pos,last_cam_pos);
    }
    if (!nearSameVec3Tol(last_cam_pyr,pyr,0))
    {
        demoRecordPyr(pyr);
        copyVec3(pyr,last_cam_pyr);
    }
}

static EntTrack *findTrack(int ent_idx)
{
    int        i;

    for(i=0;i<enttrack_count;i++)
    {
        if (enttracks[i].ent_idx == ent_idx)
        {
            return &enttracks[i];
        }
    }
    return 0;
}

static EntFrame *entFindInTracks(int ent_idx,U32 abs_time,EntTrack** trackOut)
{
    EntTrack    *track = 0;
    EntFrame    *ent;

    track = findTrack(ent_idx);
    if (!track)
    {
        track = dynArrayAdd(&enttracks,sizeof(enttracks[0]),&enttrack_count,&enttrack_max,1);
        ZeroStruct(track);
        track->ent_idx = ent_idx;
    }
    if(trackOut)
    {
        *trackOut = track;
    }
    if (track->frames && track->frames[track->count-1].abs_time == abs_time)
        return &track->frames[track->count-1];
    ent = dynArrayAdd(&track->frames,sizeof(track->frames[0]),&track->count,&track->max,1);
    ZeroStruct(ent);
    ent->abs_time = abs_time;
    ent->hp = -1;
    ent->hpMax = -1;
    return ent;
}

static int isFloat(const char* s)
{
    for(; s && *s; s++)
    {
        if(*s != '.' && *s < '0' && *s > '9')
        {
            return 0;
        }
    }
    
    return 1;
}


int demoLoad(char *fname)
{
    char            *mem,*s,*args[100],*cmd;
    int                i,count,ent_idx,unique_id=1;
    U32                abs_time=1;
    EntFrame        *ent=0;
    DemoCostume* costume = NULL;
    NetFx* fx = NULL;
    int                lineNum = 0;
    int                wasCostume = 0;
    #define            ID_OFFSET 0
    #define            FX_ID_OFFSET 0

    enttrack_count = enttrack_max = 0;
    for(i=0;i<enttrack_count;i++)
        free(enttracks[i].frames);
    free(enttracks);
    enttracks = 0;
    s = mem = fileAlloc(fname,0);
    if (!mem)
        return 0;
    count=tokenize_line(s,args,&s);
    if (count != 4 || stricmp(args[2],"Version")!=0 || atoi(args[3]) != DEMO_FILE_VERSION)
        FatalErrorf("Don't know how to load this demo file.");
    for(; s; lineNum++)
    {
        EntTrack* track;
        count=tokenize_line(s,args,&s);
        if (count < 3)
            continue;
        for(i = count; i < ARRAY_SIZE(args); i++)
        {
            args[i] = "";
        }        
        if (stricmp(args[1],"CAM")==0)
            ent_idx = DEMO_CAMERA_ID;
        else if (stricmp(args[1],"DYNGROUPS")==0)
            ent_idx = DYN_GROUP_UPDATE;
        else if (stricmp(args[1],"SKYFILE")==0)
            ent_idx = SKY_FILE_UPDATE;
        else
            ent_idx = atoi(args[1]);
        cmd = args[2];
        abs_time += MSECS_TO_ABSTIME(atoi(args[0]));

        // for data not related to entities ...
        if ( ( ent_idx <= 0 && ent_idx != DEMO_CAMERA_ID && ent_idx != DYN_GROUP_UPDATE && ent_idx != SKY_FILE_UPDATE ) ||
            ent_idx >= MAX_ENTITIES_PRIVATE)
        {
            if (stricmp(cmd,"Map")==0)
            {
                Strncpyt(demo.map_name,args[3]);
            }
            else if(stricmp(cmd,"Base")==0)
            {
                char *strBase = (char *)unescapeAndUnpack(args[3]);
                if(!baseFromStr(strBase,&g_base))
                {
                    printf("ERROR: couldn't load base\n");
                }
                else
                {
//                     int i;
//                     baseResetIds(g_base);
//                     base->curr_id++;
                      // cribbed from baseReceive
                    if(!baseAfterLoad(0, NULL, 0))
                        printf("ERROR: failed to init base\n");
                }
            }
            else if (stricmp(cmd,"Time")==0)
            {
                demo.clock_time = atof(args[3]);
            }
            else if (stricmp(cmd,"Del")==0) // special "delete all" command for concatenating demos
            {
                for(i=0;i<enttrack_count;i++)
                {
                    ent = entFindInTracks(enttracks[i].ent_idx,abs_time,NULL);
                    ent->deleted = 1;
                }
            }
            else if (stricmp(cmd,"Chat")==0)
            {
                // "You have defeated blah blah" messages. just ignore em for now
            }
            else if (stricmp(cmd,"EntRagdoll")==0)
            {
                // ragdoll data.
            }
            else if(stricmp(cmd,"GlobalAbsTime")==0)
            {
            }
            else if(ent_idx <= 0)
            {
                printf("Unknown command: %s\n",cmd);
            }
            
            continue;
        }
        ent_idx += ID_OFFSET;
        
        if (stricmp(cmd,"Player")==0)
        {
            demo.player_id = ent_idx;
            continue;
        }
        if (wasCostume)
        {
            if(count == 3 && isFloat(args[2]))
            {
                if(wasCostume <= ARRAY_SIZE(costume->scales)){
                    costume->scales[wasCostume++ - 1] = atof(args[2]);
                }
                continue;
            }
            else
            {
                wasCostume = 0;
            }
        }
        ent = entFindInTracks(ent_idx,abs_time,&track);

        #define BEGIN_IF()    if(0);
        #define IF(x)        else if(!stricmp(cmd, x))

        BEGIN_IF()
        IF("POS")
        {
            for(i=0;i<3;i++)
                ent->pos[i] = atof(args[3+i]);
            ent->has_pos = 1;
            
            // Add the previous PYR if it hasn't been updated.
            
            if(!ent->has_pyr)
            {
                copyVec3(track->last_pyr, ent->pyr);
                ent->has_pyr = 1;
            }
            copyVec3(ent->pos, track->last_pos);
        }
        IF("PYR")
        {
            for(i=0;i<3;i++)
                ent->pyr[i] = atof(args[3+i]);
            ent->has_pyr = 1;
            
            //if(!ent->has_pos)
            //{
            //    copyVec3(track->last_pos, ent->pos);
            //    ent->has_pos = 1;
            //}
            copyVec3(ent->pyr, track->last_pyr);
        }
        IF("NEW")
        {
            ent->created = 1;
            ent->name = args[3];
        }
        IF("DYNARRAY")
        {
            ent->dynGroupUpdate = args[3];
        }
        IF("SKY")
        {
            ent->sky1 = atoi( args[3] );
            ent->sky2 = atoi( args[4] );
            ent->skyWeight = atof( args[5] );
        }
        IF("MOV")
        {
            ent->move = args[3];
            if (count == 5)
            {
                ent->move_bits = atoi(args[4]);
            }
            else
            {
                // Hack to mark it unused for backwards compatibility.
                ent->move_bits = DEMO_MOVE_BITS_UNUSED;
            }
        }
        IF("HP")
        {
            ent->hp = atof(args[3]);
        }
        IF("HPMAX")
        {
            ent->hpMax = atof(args[3]);
        }
        IF("DEL")
        {
            ent->deleted = 1;
        }
        IF("NPC")
        {
            ent->has_costume = 1;
            costume = ent->costume = calloc(sizeof(*costume),1);
            {
                int idx;
                npcFindByName(args[3], &idx);
                costume->npc_index = idx;
            }
            costume->costume_index = 0;
            if (count > 4)
                costume->costume_index    = atoi(args[3+1]);
        }
        IF("SEQ")
        {
            ent->has_costume = 1;
            costume = ent->costume = calloc(sizeof(*costume),1);
            costume->seq_name = args[3];
        }
        IF("DYNLIB") //The bases record the library piece they use outside the costume
        {
            ent->costumeWorldGroup = args[3];
        }
        IF("COSTUME")
        {
            if(ent)
            {
                int        k;

                wasCostume = 1;
                ent->has_costume = 1;
                costume = ent->costume    = calloc(sizeof(*costume),1);
                costume->bodytype        = atoi(args[3]);
                costume->color_skin        = getColor(args[3+1]);
                for(k=0;k<count-5;k++)
                    costume->scales[k] = atof(args[5 + k]);
            }
        }
        IF("PARTSNAME")
        {
            if(ent && costume && ent->costume)
            {
                DemoCostumePart    *part;

                costume->num_parts++;
                costume->parts = realloc(ent->costume->parts,sizeof(*part) * costume->num_parts);
                part = &costume->parts[costume->num_parts-1];
                part->geom = args[3];
                for(i=0;i<2;i++)
                    part->texnames[i] = args[4+i];
                for(i=0;i<4;i++)
                {
                    if (6+i < count)
                        part->colors[i] = getColor(args[6+i]);
                    else
                        break;
                }
                if ( 10 < count )
                    part->fxname = args[10];
                else
                    part->fxname = NULL;
            }
        }
        IF("FX")
        {
            if(ent)
            {
                ent->fx_list = realloc(ent->fx_list,sizeof(*fx) * ++ent->fx_count);
                fx = &ent->fx_list[ent->fx_count-1];
                ZeroStruct(fx);
                if (stricmp(args[3],"OneShot")==0)
                    fx->command = CREATE_ONESHOT_FX;
                if (stricmp(args[3],"Maintained")==0)
                    fx->command = CREATE_MAINTAINED_FX;
                fx->net_id            = atoi(args[4]);
                if (fx->net_id)
                    fx->net_id += FX_ID_OFFSET;
                fx->handle            = stringToReference(args[5]);
                if(!fx->handle)
                {
                    ent->fx_count--;
                }
                else
                {
                    fx->clientTimer        = 0;
                    fx->clientTriggerFx    = atoi(args[6]);
                    if (fx->clientTriggerFx)
                        fx->clientTriggerFx += FX_ID_OFFSET;
                }
            }
        }
        IF("FXDESTROY")
        {
            if(ent)
            {
                ent->fx_list = realloc(ent->fx_list,sizeof(*fx) * ++ent->fx_count);
                fx = &ent->fx_list[ent->fx_count-1];
                fx->command = DESTROY_FX;
                fx->net_id    = atoi(args[3]);
                if (fx->net_id)
                    fx->net_id += FX_ID_OFFSET;;
            }
        }
        IF("FXSCALE")
        {
            if(fx)
            {
                fx->radius    = atof(args[3]);
                fx->power    = atoi(args[4]);
            }
        }
        IF("FXDEBRIS")
        {
            if(fx)
            {
                fx->debris    = atoi(args[3]);
            }
        }
        IF("FXTINT")
        {
            if(fx)
            {
                fx->colorTint.primary.integer = atoi(args[3]);
                fx->colorTint.secondary.integer = atoi(args[4]);
                fx->hasColorTint = 1;
            }
        }
        IF("ORIGIN")
        {
            if(fx)
            {
                if (stricmp(args[3],"pos")==0)
                {
                    fx->originType = NETFX_POSITION;
                    for(i=0;i<3;i++)
                        fx->originPos[i] = atoi(args[4+i]);
                }
                else
                {
                    fx->originType    = NETFX_ENTITY;
                    fx->originEnt    = atoi(args[4]);
                    fx->bone_id        = atoi(args[5]);
                    if (fx->originEnt)
                        fx->originEnt += ID_OFFSET;
                }
            }
        }
        IF("TARGET")
        {
            if(fx)
            {
                if (stricmp(args[3],"pos")==0)
                {
                    fx->targetType = NETFX_POSITION;
                    for(i=0;i<3;i++)
                        fx->targetPos[i] = atoi(args[4+i]);
                }
                else
                {
                    fx->targetType    = NETFX_ENTITY;
                    fx->targetEnt    = atoi(args[4]);
                    //fx->bone_id        = atoi(args[5]);
                    if (fx->targetEnt)
                        fx->targetEnt += ID_OFFSET;
                }
            }
        }
        IF("Chat")
        {
            if(ent)
            {
                DemoChat    *chat;

                ent->chat_list = realloc(ent->chat_list,sizeof(*chat) * ++ent->chat_count);
                chat = &ent->chat_list[ent->chat_count-1];
                chat->type = atoi(args[3]);

                chat->floater = 0;

                chat->number = atoi(args[4]);
                chat->msg = args[5];
            }
        }
        IF("EntRagdoll")
        {
            if(ent)
            {
                ent->num_bones = atoi(args[3]);
                ent->ragdoll_svr_time = atoi(args[4]);
                ent->ragdoll_client_time = atoi(args[5]);
                if(ent->num_bones)
                {
                    /*
                    U32 uiHash = hashString(args[6], true);
                    printf("Hash = 0x%x\n", uiHash);
                    */
                    ent->ragdollCompressed = intListFromHexString(args[6], ent->num_bones * 3);
                }
            }
        }
        IF("Float")
        {
            if(ent)
            {
                DemoChat    *chat;

                ent->chat_list = realloc(ent->chat_list,sizeof(*chat) * ++ent->chat_count);
                chat = &ent->chat_list[ent->chat_count-1];
                chat->type = atoi(args[3]);
                chat->number = 0;
                chat->msg = args[4];
                chat->floater = 1;
            }
        }
        IF("FloatDmg")
        {
            if(ent)
            {
                DemoDmg    *dmg;

                ent->dmg_list = realloc(ent->dmg_list,sizeof(*dmg) * ++ent->dmg_count);
                dmg = &ent->dmg_list[ent->dmg_count-1];
                dmg->receiver = atoi(args[3]);
                dmg->amount = atoi(args[4]);
                if(count>5)
                    dmg->msg = args[5];
                else
                    dmg->msg = NULL;
            }
        }
        IF("XLU")
        {
            if(ent)
            {
                ent->xlucency = atof(args[3]) * 255;
            }
        }
        else
        {
            printf("Unknown command: %s\n", cmd);
        }
    }
    abstime_end = abstime_start = enttracks[0].frames[0].abs_time;
    for(i=0;i<enttrack_count;i++)
    {
        int            j,k;
        EntFrame    *frame,*next;
        Vec3        pos = {0,0,0},pyr={0,0,0};
        U32            last_pos_time=0,last_pyr_time=0;
        DemoCostume    *costume = 0;
        F32            ratio;

        for(j=0;j<enttracks[i].count;j++)
        {
            frame = &enttracks[i].frames[j];
            if (frame->created)
                unique_id++;
            frame->unique_id = unique_id;
            if (frame->deleted)
                unique_id++;
            if (frame->has_costume)
                costume = frame->costume;
            else
                frame->costume = costume;
            if (frame->has_pos)
            {
                copyVec3(frame->pos,pos);
                last_pos_time = frame->abs_time;
            }
            else
            {
                for(next=0,k=j+1;k<enttracks[i].count;k++)
                {
                    if (enttracks[i].frames[k].has_pos)
                    {
                        next = &enttracks[i].frames[k];
                        break;
                    }
                }
                if (next && (frame->abs_time - last_pos_time < ABSTIME_SECOND))
                {
                    ratio = (frame->abs_time - last_pos_time) / (F32) MAX((next->abs_time - last_pos_time),1);
                    posInterp(ratio,pos,next->pos,frame->pos);
                }
                else
                    copyVec3(pos,frame->pos);
            }
            if (frame->has_pyr)
            {
                copyVec3(frame->pyr,pyr);
                last_pyr_time = frame->abs_time;
            }
            else
            {
                for(next=0,k=j+1;k<enttracks[i].count;k++)
                {
                    if (enttracks[i].frames[k].has_pyr)
                    {
                        next = &enttracks[i].frames[k];
                        break;
                    }
                }
                if (next && (frame->abs_time - last_pos_time < ABSTIME_SECOND))
                {
                    ratio = (frame->abs_time - last_pyr_time) / (F32) MAX((next->abs_time - last_pyr_time),1);
                    interpPYR(ratio,pyr,next->pyr,frame->pyr);
                }
                else
                    copyVec3(pyr,frame->pyr);
            }
            if(frame->ragdoll_svr_time)
            {
                int i_ragprev;
                for(i_ragprev = j - 1; i_ragprev >= 0; --i_ragprev)
                {
                    if(enttracks[i].frames[i_ragprev].ragdoll_svr_time)
                    {
                        frame->ragdoll_svr_dt = frame->ragdoll_svr_time - enttracks[i].frames[i_ragprev].ragdoll_svr_time;
                        break;
                    }
                }
                if( i_ragprev == 0 ) // not found
                    frame->ragdoll_svr_dt = frame->ragdoll_svr_time - frame->ragdoll_client_time;
            }
        }
        abstime_start    = MIN(abstime_start,enttracks[i].frames[0].abs_time);
        abstime_end        = MAX(abstime_end,enttracks[i].frames[enttracks[i].count-1].abs_time);
    }
    return 1;
}

static int findInterpFrame(EntTrack *track,U32 abs_time,int *before,int *after)
{
    int        i,count,start;
    EntFrame *frames;

    frames    = track->frames;
    count    = track->count;
    start    = track->last_frame;
    *before    = 0;
    *after    = 0;
    if (abs_time < frames[0].abs_time)
        return 0;
    if (start < 0 || start >= count || abs_time < frames[start].abs_time)
        start = 0;
    for(i=start;i<count;i++)
    {
        *before = i;
        *after = i;
        if (abs_time >= frames[i].abs_time && abs_time <= frames[i+1].abs_time)
        {
            *after = i+1;
            break;
        }
    }
    i = MIN(i,count-1);
    if (frames[i].deleted)
        return 0;
    return 1;
}



static void setCostume(Entity *e,DemoCostume *dcostume)
{
    int changeCostume = 1;

    if (dcostume->seq_name)
    {
        entSetSeq(e, seqLoadInst( dcostume->seq_name, 0, SEQ_LOAD_FULL, e->randSeed, e  ));
    }
    else if (dcostume->npc_index)
    {
        e->npcIndex = dcostume->npc_index;
        e->costumeIndex = dcostume->costume_index;
        entSetImmutableCostumeUnowned( e, npcDefsGetCostume(e->npcIndex, e->costumeIndex) ); // e->costume = npcDefsGetCostume(e->npcIndex, e->costumeIndex);
    }
    else if(e->costume && e->costume->parts)
    {
        int        i, j;
        Costume *new_costume;

        new_costume = entGetMutableCostume(e);    // ask for a costume we can modify (may cause a clone if costume was shared)

        new_costume->appearance.bodytype            = dcostume->bodytype;
        new_costume->appearance.colorSkin.integer    = dcostume->color_skin;

        for(i=0;i<MAX_BODY_SCALES;i++)
            new_costume->appearance.fScales[i] = dcostume->scales[i];

        new_costume->appearance.iNumParts            = dcostume->num_parts;

        for(i=0;i<dcostume->num_parts;i++)
        {
            new_costume->parts[i]->pchGeom = allocAddString_checked(dcostume->parts[i].geom);
            new_costume->parts[i]->pchTex1 = allocAddString_checked(dcostume->parts[i].texnames[0]);
            new_costume->parts[i]->pchTex2 = allocAddString_checked(dcostume->parts[i].texnames[1]);
            new_costume->parts[i]->pchFxName = allocAddString_checked(dcostume->parts[i].fxname);

            for (j=0; j<ARRAY_SIZE(new_costume->parts[i]->color); j++) 
                new_costume->parts[i]->color[j].integer  = dcostume->parts[i].colors[j];
        }
    }
    else
    {
        printf(    "DEMO ERROR: Costume with parts being applied to non-player entity (%d).\n",
                e->svr_idx);
        changeCostume = 0;
    }
    
    if(e->costume && changeCostume)
    {
        costume_Apply(e);
    }
}

static void makeInterpFrame(EntTrack *track,U32 abs_time,int before,int after,EntFrame *interp)
{
    EntFrame    *a,*b;
    F32            ratio;
    int            dt;

    a = &track->frames[before];
    b = &track->frames[after];
    dt = b->abs_time - a->abs_time;
    if (dt <= 0 || dt > ABSTIME_SECOND)
        ratio = 0;
    else
        ratio = (abs_time - a->abs_time) / (F32)(b->abs_time - a->abs_time);
    *interp = *a;
    interpPYR(ratio,a->pyr,b->pyr,interp->pyr);
    posInterp(ratio,a->pos,b->pos,interp->pos);
}

static void fixFxTrack(EntTrack *track,int j,int k)
{
    int        net_id;
    NetFx    *destroy,*create;

    destroy = &track->frames[j].fx_list[k];
    net_id = destroy->net_id;
    destroy->command = 0;

    for(;j<track->count;j++)
    {
        for(k=0;k<track->frames[j].fx_count;k++)
        {
            create = &track->frames[j].fx_list[k];
            if (create->net_id == net_id && create != destroy)
            {
                create->command = 0;
                return;
            }
        }
    }
}

void demoPlayFrame(U32 abs_time)
{
    int            i,before,after;
    EntTrack    *track;
    EntFrame    interp;
    Entity        *e;

    for(i=0;i<enttrack_count;i++)
    {
        int        del_ent=0,update_ent=0,update_costume=0;

        track = &enttracks[i];

        if (!findInterpFrame(track,abs_time,&before,&after))
        {
            if (track->ent_idx != DEMO_CAMERA_ID && track->ent_idx != DYN_GROUP_UPDATE && track->ent_idx != SKY_FILE_UPDATE )
                del_ent = 1;
        }
        else
        {
            update_ent = 1;
            makeInterpFrame(track,abs_time,before,after,&interp);
        }
        if (update_ent && track->ent_idx == SKY_FILE_UPDATE)
        {
            sunSetSkyFadeClient(interp.sky1, interp.sky2, interp.skyWeight);
        }
        e = ent_xlat[track->ent_idx];
        if (update_ent && e && (e->demo_id != interp.unique_id || interp.deleted))
            del_ent = 1; 
        if (del_ent && e)
        {
            ent_xlat[track->ent_idx] = 0;
            if (track->ent_idx == demo.player_id)
                playerSetEnt(0);
            entFree(e);
        }
        if(update_ent)
        {
            int        ragdoll_prev_time = 0; // ab: hack for testing, stick in frame
            int        j;
            Mat3    newmat;

            if (!interp.costume)
                continue;
            if (!(e=ent_xlat[track->ent_idx]))
            {
                const CharacterClass  *pclass = NULL;
                const CharacterOrigin *porigin = NULL;

                e = ent_xlat[track->ent_idx] = entCreate(0);
                e->svr_idx = track->ent_idx;
                e->demo_id = interp.unique_id;
                if (interp.costume->num_parts || track->ent_idx == demo.player_id)
                    SET_ENTTYPE(e) = ENTTYPE_PLAYER;
                else
                    SET_ENTTYPE(e) = ENTTYPE_CRITTER;
                playerVarAlloc(e, ENTTYPE(e));
                e->pchar->entParent = e;
                porigin = origins_GetPtrFromName(&g_CharacterOrigins, "Natural");
                pclass = classes_GetPtrFromName(&g_CharacterClasses, "Class_Scrapper");
                character_Create(e->pchar, e, porigin, pclass);
                update_costume = 1;
                track->last_frame = before-1;
                if (track->ent_idx == demo.player_id)
                    playerSetEnt(e);
                if (interp.name)
                    Strcpy(e->name,interp.name);
            }
            entUpdatePosInterpolated(e, interp.pos);
            createMat3YPR(newmat,interp.pyr);
            entSetMat3(e, newmat);

            for(j=track->last_frame+1;j<=before;j++)
            {
                EntFrame    *curr = &track->frames[j];
                int            k;

                if (update_costume || curr->has_costume)
                {
                    if (curr->costume)
                        setCostume(e,curr->costume);
                    if (curr->costumeWorldGroup) //3.23.06 MW 
                        e->costumeWorldGroup = strdup( curr->costumeWorldGroup );
                }
                if(e->pchar)
                {
                    if (curr->hp >= 0)
                    {
                        e->pchar->attrCur.fHitPoints = curr->hp;
                    }
                    if (curr->hpMax >= 0)
                    {
                        e->pchar->attrMax.fHitPoints = curr->hpMax;
                    }
                }
                if (curr->move && e->seq && e->seq->info)
                {
                    U16 uiIdx;
                    if( seqGetMoveIdxFromName( curr->move, e->seq->info, &uiIdx ) )
                        e->next_move_idx = (int)uiIdx;
                    else
                        e->next_move_idx = 0;

                    e->move_change_timer = 0;

                    if (curr->move_bits != DEMO_MOVE_BITS_UNUSED)
                    {
                        e->next_move_bits = curr->move_bits;
                    }
                }
                for(k=0;k<curr->fx_count;k++)
                {
                    int        already_started=0;
                    NetFx    *fx,*alloc_fx;

                    fx = &curr->fx_list[k];
                    if (!fx->command)
                        continue;

                    //I think this is all cray now.  entrecvAllocNetFx(e);
                    //should be all you need, I think the rest is just leftovers?

                    {
                        int        j;

                        for(j=0;j<e->fxtrackersAllocedCount;j++)
                        {
                            if(e->fxtrackers[j].netfx.net_id == fx->net_id)
                            {
                                already_started = 1;
                                break;
                            }
                        }
                        for(j=0;j<e->netfx_count;j++)
                        {
                            if(e->netfx_list[j].net_id == fx->net_id)
                            {
                                already_started = 1;
                                break;
                            }
                        }
                    }
                    if (!already_started && fx->command==DESTROY_FX)
                        fixFxTrack(track,j,k);
                    else if ((!already_started && fx->command!=DESTROY_FX)
                        || (already_started && fx->command==DESTROY_FX))
                    {
                        alloc_fx = entrecvAllocNetFx(e);
                        *alloc_fx = *fx;
                    }
                }
                for(k=0;k<curr->chat_count;k++)
                {
                    DemoChat    *chat = &curr->chat_list[k];

                    if (chat->floater)
                        handleFloatingInfo(track->ent_idx,chat->msg,chat->type,0,0);
                    else
                    {
                        // The more detailed means of identifying what source
                        // the chat came from are stripped when the demo is
                        // recorded, so here we rely that all battle spam is
                        // marked as type 2 (infobox) and everything else type 0
                        // (normal). This lets us mark battle spam as non-floater
                        // and thus hidden but the rest of the text will come
                        // through.
                        int floats = 0;

                        if (chat->number == kDemoChatType_Normal)
                        {
                            floats = 1;
                        }

                        addChatMsgsFloat(chat->msg, chat->type, DEFAULT_BUBBLE_DURATION, track->ent_idx, 0);
                    }
                }
                for(k=0;k<curr->dmg_count;k++)
                {
                    DemoDmg    *dmg = &curr->dmg_list[k];
                    Entity    *hit_ent = entFromId(dmg->receiver);

                    // TODO: Fix this to track absorbs
                    if (hit_ent)
                        addFloatingDamage( hit_ent, e, -dmg->amount, dmg->msg, 0, 0);
                }

                if(!control_state.no_ragdoll
                    && curr->ragdoll_svr_time ) // its a valid ragdoll frame
                {
                    EntFrame *before_frame = &track->frames[before];
                    S32 frame_dt = abs_time - before_frame->abs_time;
                    S32 svr_client_dt = curr->ragdoll_svr_time - curr->ragdoll_client_time;
                    
                    entGetRagdoll_ForDemoPlayback(e,curr->num_bones,curr->ragdollCompressed,curr->ragdoll_svr_dt, svr_client_dt, frame_dt);
                }
            }
        }
        track->last_frame = before;
    }
}

void demoStop()
{
    int        i;
    Entity    *e;

    play_abs_time = 0;
    if (demo_file)
    {
        dumpDemoToFile();
        fclose(demo_file);
        demo_file = 0;
    }
    for(i=1;i<entities_max;i++)
    {
        if (!(entity_state[i] & ENTITY_IN_USE ))
            continue;
        e = entities[i];
        if (e->demo_id)
        {
            ent_xlat[e->svr_idx] = 0;
            if (e->svr_idx == demo.player_id)
                playerSetEnt(0);
            entFree(e);
        }
    }
}

static char *demoDir(char *fname,char *demoname, size_t demoname_size)
{
    char    *s,buf[1000];

    if (strncmp(fname,"\\\\",2)==0 || strchr(fname,':') || fname[0]=='.')
    {
        strcpy_s(demoname,demoname_size,fname);
        return demoname;
    }
    Strcpy(buf,fileDataDir());
    s = strrchr(buf,'/');
    if (s)
        *s = 0;
    if (strchr(fname,'.'))
        sprintf(demoname,"%s/client_demos/%s",buf,fname);
    else
        sprintf(demoname,"%s/client_demos/%s.cohdemo",buf,fname);
    mkdirtree(demoname);
    return demoname;
}

void demoStartPlay(char *fname)
{
    char    buf[1000];

    demoStop();
    sprintf(buf,"demos/%s.cohdemo",fname);
    if (!demoLoad(buf))
        demoLoad(demoDir(fname,SAFESTR(buf)));
    play_abs_time = abstime_start;
}


void demoStartRecord(char *fname)
{
    char    buf[1000],errmsg[1000];
    int        i,j;
    Entity    *e;

    demoStop();
    demo.first_frame = 1;
    demo_file = fileOpen(demoDir(fname,SAFESTR(buf)),"wt");
    if (!demo_file)
    {
        sprintf(errmsg,"can't open %s for writing!",buf);
        winMsgAlert(errmsg);
    }
    invalidateLastCameraPos();
    demoSetAbsTime(global_state.client_abs);
    demoSetEntIdx(0);
    demoRecord("Version %d",DEMO_FILE_VERSION);
    demoRecord("Map %s",gMapName);
    if( g_base.rooms )
    {
        char *baseStr = NULL;
        if( baseToStr(&baseStr, &g_base) )
        {
            char const *escaped = packAndEscape(baseStr);
            demoRecord("Base \"%s\"", escaped);
            if(isDevelopmentMode())
                demoRecord("BaseDebug \"%s\"", escapeString(baseStr));
            estrDestroy(&baseStr);
        }
        else
        {
            sprintf(errmsg,"couldn't convert %i base to string", g_base.supergroup_id );
            winMsgAlert(errmsg);
        }
    }
    demoRecord("Time %f",server_visible_state.time);
    e = playerPtr();
    if (e)
    {
        demoSetEntIdx(e->svr_idx);
        demoRecord("Player");
        demoSetEntIdx(0);
    }
    for(i=1;i<entities_max;i++)
    {
        Vec3    pyr;

        e = entities[i];
        if (!(entity_state[i] & ENTITY_IN_USE ) || e->svr_idx <= 0)
            continue;
        demoSetEntIdx(e->svr_idx);
        demoRecordCreate(e->name);
        demoRecordAppearance(e);
        demoRecordPos(ENTPOS(e));
        getMat3YPR(ENTMAT(e),pyr);
        demoRecordPyr(pyr);
        demoRecordMove(e,e->next_move_idx,e->next_move_bits);
        for(j=0;j<e->fxtrackersAllocedCount;j++)
        {
            if(e->fxtrackers[j].netfx.net_id)
                demoRecordFx(&e->fxtrackers[j].netfx);
        }
        demoRecordStatChanges(e);
    }

    demoRecordDynGroups();

    demoRecordClientInfo();
    demo.first_frame = 0;
}

#ifndef TEST_CLIENT

static char *getCpuName(char *cpu_name,int cpu_name_size)
{
    int result,namesize = cpu_name_size;
    HKEY hKey;

    strcpy_s(cpu_name, cpu_name_size, "Unknown");
    result = RegOpenKeyExA(HKEY_LOCAL_MACHINE,"Hardware\\Description\\System\\CentralProcessor\\0", 0, KEY_QUERY_VALUE, &hKey);
    if (result != ERROR_SUCCESS)
        return cpu_name;
    result = RegQueryValueExA(hKey, "ProcessorNameString", NULL, NULL, (LPBYTE)cpu_name, &namesize);
    RegCloseKey(hKey);
    if (result != ERROR_SUCCESS)
        return cpu_name;
    return cpu_name;
}

static void writeSystemInfo(char *str, size_t strSize)
{
    char        namebuf[256];
    GfxSettings gfxSettings = {0};

    rdrGetSystemSpecs( &systemSpecs );
    gfxGetSettings( &gfxSettings );

    strcatf_s(str,strSize,"Demo:\t\t%s\n",demo_state.demoname);
    strcatf_s(str,strSize,"Screen:\t\t");
    if( gfxSettings.maximized && !gfxSettings.fullScreen)
    {
        strcatf_s(str,strSize,"Windowed, Maximized\n");
    }
    else
    {
        strcatf_s(str,strSize,"%dx%d (%d Hz) %s\n",gfxSettings.screenX, gfxSettings.screenY, gfxSettings.refreshRate, gfxSettings.fullScreen ? "Fullscreen" : "Windowed");
    }

    if( systemSpecs.CPUSpeed )
    {
        strcatf_s(str,strSize,"CPU:\t\t%d x %0.0f Mhz %s\n",getNumRealCpus(), (systemSpecs.CPUSpeed / 1000000.0),getCpuName(namebuf,sizeof(namebuf)) );
    }
    else
        strcatf_s(str,strSize,"CPU:\t\tUnable to determine\n");

    if( systemSpecs.maxPhysicalMemory )
        strcatf_s(str,strSize,"Memory:\t%u MB\n",  ( systemSpecs.maxPhysicalMemory / (1024*1024) ) );
    else
        strcatf_s(str,strSize,"Memory:\tUnable to determine\n" );

    if( systemSpecs.videoCardName[0] )
        strcatf_s(str,strSize,"Video Card:\t%s\n", systemSpecs.videoCardName );
    else
        strcatf_s(str,strSize,"Video Card:\tUnable to identify your video card.\n" );

    {
        static char *texqual_names[] = { "Very High", "High", "Medium", "Low", "Very Low" };

        if (gfxSettings.advanced.mipLevel+1 < ARRAY_SIZE(texqual_names))
            strcatf_s(str,strSize,"Texture:\t%s",texqual_names[gfxSettings.advanced.mipLevel+1]);
        else
            strcatf_s(str,strSize,"Texture:\t%d",gfxSettings.advanced.mipLevel+1);
    }
    {
        static char *shadowmode_names[] = { "None", "Stencil", "Shadow Maps"};

        if (gfxSettings.advanced.shadowMode+1 < ARRAY_SIZE(shadowmode_names))
            strcatf_s(str,strSize,"Shadows:\t%s",shadowmode_names[gfxSettings.advanced.shadowMode]);
        else
            strcatf_s(str,strSize,"Shadows:\t%d",gfxSettings.advanced.shadowMode);
    }
    {
        static char *cubemapmode_names[] = { "None", "Low Quality", "Medium Quality", "High Quality", "Ultra Quality"};

        if (gfxSettings.advanced.cubemapMode+1 < ARRAY_SIZE(cubemapmode_names))
            strcatf_s(str,strSize,"Cubemaps:\t%s",cubemapmode_names[gfxSettings.advanced.cubemapMode]);
        else
            strcatf_s(str,strSize,"Cubemaps:\t%d",gfxSettings.advanced.cubemapMode);
    }
    {
        static char *physqual_names[] = { "None", "Low", "Medium", "High", "VeryHigh" };

        if (gfxSettings.advanced.physicsQuality+1 < ARRAY_SIZE(physqual_names))
            strcatf_s(str,strSize,"Physics:\t%s",physqual_names[gfxSettings.advanced.physicsQuality+1]);
        else
            strcatf_s(str,strSize,"Physics:\t%d",gfxSettings.advanced.physicsQuality+1);
    }


    strcatf_s(str,strSize,"WorldDetail:\t%1.2f\t\tEntityDetail:\t%1.2f\n",gfxSettings.advanced.worldDetailLevel,gfxSettings.advanced.entityDetailLevel);
    strcatf_s(str,strSize,"MaxParticles:\t%d\t\tParticleFill:\t%1.2f\n",gfxSettings.advanced.maxParticles,gfxSettings.advanced.maxParticleFill);
    strcatf_s(str,strSize,"Render features:\t%s\n",rdrFeaturesString(true));
    strcatf_s(str,strSize,"\nRun\tAvg\tMin\tMax\tFrames\tTime\n" );
}

typedef struct
{
    F32        render_time;
    U32        abs_time;
} FrameTime;

static FrameTime    *frame_times;
static int frame_count,frame_max;
static int demo_count,frame_total_timer;
static char stat_buf[10000];
static int    total_frames;
static F32    total_avg,total_min,total_max,total_time;

static void demoShowStats(void)
{
    winErrorDialog(stat_buf, "Performance Results", 0, 0);
}

void demotestprint()
{
    writeSystemInfo(SAFESTR(stat_buf));
    demoShowStats();
    exit(0);
}

static void dumpPerfStats()
{
    static FILE    *file;
    char        fname[1000],fullname[1000];
    int            i,idx=0;
    F32            total,min_tick=1000,max_tick=0;

    if (!demo_state.demo_loop)
        return;
    if (!demo_count++)
    {
        for(i=1;;i++)
        {
            sprintf(fname,"results/%s_%d.cohstats",getFileName(demo_state.demoname),i);
            demoDir(fname,SAFESTR(fullname));
            if (!(file = fileOpen(fullname,"r")))
            {
                file = fileOpen(fullname,"wt");
                break;
            }
            fclose(file);
        }
        if (!file)
            return;
        writeSystemInfo(SAFESTR(stat_buf));
        fprintf(file,"%s",stat_buf);
    }
    for(i=0;i<frame_count;i++)
    {
        min_tick = MIN(min_tick,frame_times[i].render_time);
        max_tick = MAX(max_tick,frame_times[i].render_time);
    }
    total = timerElapsed(frame_total_timer);

    idx = strlen(stat_buf);
    strcatf(stat_buf,"%1d\t%1.1f\t%1.1f\t%1.1f\t%d\t%1.2f\n",demo_count,
        frame_count / total,1 / max_tick,1 / min_tick,
        frame_count,total);
    fprintf(file,"%s",stat_buf + idx);
    if (demo_count!=1)
    {
        total_avg += frame_count / total;
        total_min += 1 / max_tick;
        total_max += 1 / min_tick;
        total_frames += frame_count;
        total_time += total;
    }
    if (demo_state.demo_framestats)
    {
        for(i=0;i<frame_count;i++)
        {
            fprintf(file,"%d %d %f\n",i,ABSTIME_TO_MSECS(frame_times[i].abs_time),1/frame_times[i].render_time);
        }
    }

    fflush(file);
    free(frame_times);
    frame_times = 0;
    frame_count = frame_max = 0;
    timerStart(frame_total_timer);
    if (demo_count >= demo_state.demo_loop)
    {
        F32        div = demo_count-1;

        if (div > 0)
        {
            strcatf(stat_buf,"-----\n");
            strcatf(stat_buf,"Avg*\t%1.1f\t%1.1f\t%1.1f\t%d\t%1.2f\n",
                total_avg / div,total_min / div,total_max / div,
                (int)(total_frames / div),total_time / div);
            strcatf(stat_buf,"\n* Note that first run is not used to compute average.");
        }
        atexit(demoShowStats);
        windowExit(0);
    }
}

void updateDynGroupStatus( char * updateString )
{
    extern int                dyn_group_count;
    extern DynGroupStatus    *dyn_group_status;
    char * dStr;
    int i;
    char * tok;

    if( !updateString )
        return;

    dStr = strdup( updateString );

    tok = strtok( dStr, "|" ); 

    //expecting "|12,1|200,0|30,1" exactly equal to the dynGroupCount
    for( i = 0; tok && i < dyn_group_count ; i++ )
    {
        char * comma = strchr( tok, ',' );
        if( comma )
        {
            comma[0] = 0;
            dyn_group_status[i].hp = atoi( tok );
            dyn_group_status[i].repair = atoi( &(comma[1]) );
            comma[0] = ','; //so strtok will keep working
        }
        else 
            break; //bad string

        tok = strtok( NULL, "|" );
    }

    free(dStr );
}



//global to emphasisze that there's only one DynGroup track
static int gNextUnplayedDynGroupFrame = 0;
static void updateDynGroups()
{
    EntTrack    *dynGroupTrack;
    EntFrame    *frame;
    int            i;

    dynGroupTrack = findTrack(DYN_GROUP_UPDATE);
    if (!dynGroupTrack)
        return;

    for( i = gNextUnplayedDynGroupFrame ; i < dynGroupTrack->count ; i++ )
    {
        frame = &dynGroupTrack->frames[i];
        if( frame->abs_time < play_abs_time )
        {
            updateDynGroupStatus( frame->dynGroupUpdate );
            gNextUnplayedDynGroupFrame = i+1;
        }
        else
        {
            break;
        }
    }
}

//global to emphasisze that there's only one DynGroup track
static int gNextUnplayedSkyFileFrame = 0;
static void updateSkyFile()
{
    EntTrack    *dynGroupTrack;
    EntFrame    *frame;
    int            i;

    dynGroupTrack = findTrack(DYN_GROUP_UPDATE);
    if (!dynGroupTrack)
        return;

    for( i = gNextUnplayedSkyFileFrame ; i < dynGroupTrack->count ; i++ )
    {
        frame = &dynGroupTrack->frames[i];
        if( frame->abs_time < play_abs_time )
        {
            sunSetSkyFadeClient( dynGroupTrack->frames[i].sky1, dynGroupTrack->frames[i].sky2, dynGroupTrack->frames[i].skyWeight );
            gNextUnplayedSkyFileFrame = i+1;
        }
        else
        {
            break;
        }
    }
}

static void updateCamera()
{
    EntTrack    *cam_track,*plr_track;
    EntFrame    interp;
    int            before=0,after=0,abs_time = play_abs_time;
    static int    init=1;

    if (demo_state.detached_camera)
        return;

    if(editMode())
    {
        camLockCamera( false );
        camUpdate();
        return;
    }
    cam_track = findTrack(DEMO_CAMERA_ID);
    control_state.canlook = control_state.cam_rotate = 0;
    if (!cam_track)
        return;
    camLockCamera( true );
    findInterpFrame(cam_track,abs_time,&before,&after);
    makeInterpFrame(cam_track,abs_time,before,after,&interp);
    if(debug_state.debugCamera.editMode)
    {
        ItemSelect item_sel = {0};
        Vec3 end;
        bool hitWorld;
        Mat4 matWorld;
        
        cursorFind((F32)gMouseInpCur.x, (F32)gMouseInpCur.y, 1000, &item_sel, end, matWorld, &hitWorld, 0, NULL);
        
        if(item_sel.ent_id > 0 && inpEdge(INP_LBUTTON))
        {
            current_target = entFromId(item_sel.ent_id);
        }
    }
    else
    {
        current_target = NULL;
    }
    if(!getDebugCameraPosition(&cam_info))
    {
        createMat3RYP(cam_info.cammat,interp.pyr);
        copyVec3(interp.pos,cam_info.cammat[3]);
    }
    // if camera looks like it's in first person, stop drawing player
    {
        Entity        *e;

        e = ent_xlat[demo.player_id];
        if (e)
        {
            Vec3    head_pos;

            copyVec3(ENTPOS(e),head_pos);
            head_pos[1] += 5.5;
            if (distance3(cam_info.cammat[3],head_pos) < 2)
                SET_ENTHIDE(e) = 1;
            else
                SET_ENTHIDE(e) = 0;
        }
    }
    plr_track = findTrack(demo.player_id);
    if (plr_track && findInterpFrame(plr_track,abs_time,&before,&after))
    {
        makeInterpFrame(plr_track,abs_time,before,after,&interp);
        createMat3RYP(cam_info.mat,interp.pyr);
        copyVec3(interp.pos,cam_info.mat[3]);
    }
    init = 1;
}

static void handleDetachedCameraInput(CameraInfo *ci)
{
    extern int mouse_dx,mouse_dy;
    static struct { int key; int axis; int sign; } DOLLY_CONTROLS[] =
    {
        { INP_D, 0, -1 },
        { INP_A, 0, 1 },
        { INP_X, 1, -1 },
        { INP_SPACE, 1, 1 },
        { INP_W, 2, -1 },
        { INP_S, 2, 1 },
    };

    int i;

    if (gMouseInpCur.right == MS_DOWN)
    {
        ci->pyr[0] = addAngle(ci->pyr[0], mouse_dy * optionGetf(kUO_MouseSpeed) / 500.0F);
        ci->pyr[1] = addAngle(ci->pyr[1], mouse_dx * optionGetf(kUO_MouseSpeed) / 500.0F);
        createMat3YPR(ci->cammat, ci->pyr);
    }

    for (i = 0; i < ARRAY_SIZE(DOLLY_CONTROLS); ++i) 
    {
        if (inpLevel(DOLLY_CONTROLS[i].key))
        {
            F32 *direction = ci->cammat[DOLLY_CONTROLS[i].axis];
            F32 scale = DOLLY_CONTROLS[i].sign * TIMESTEP * control_state.speed_scale;
            scaleAddVec3(direction, scale, ci->cammat[3], ci->cammat[3]);
        }
    }
}

void demoCheckInterface()
{
    if (!conVisible() && demoIsPlaying()) {
        if (inpEdge(INP_P)) {
            demo_state.demo_pause=!demo_state.demo_pause;
        }
        if (inpEdge(INP_EQUALS)) {
            if (!demo_state.demo_speed_scale)
                demo_state.demo_speed_scale = 1.0;
            demo_state.demo_speed_scale *= 2.0;
        }
        if (inpEdge(INP_MINUS)) {
            if (!demo_state.demo_speed_scale)
                demo_state.demo_speed_scale = 1.0;
            demo_state.demo_speed_scale /= 2.0;
        }
        if (inpEdge(INP_F2)) {
            demo_state.detached_camera = !demo_state.detached_camera;
        }

        if (demo_state.detached_camera)
            handleDetachedCameraInput(&cam_info);
    }
}


void demoCheckPlay()
{
    static int timer;
    F32        elapsed;
    int        dt;

    demoCheckInterface();
    if (!play_abs_time)
        return;
    if (!timer)
        timer = timerAlloc();
    elapsed = timerElapsed(timer);
    if(noTimeAdvance)
    {
        elapsed = 0;
        noTimeAdvance = 0;
    }
    timerStart(timer);
    if (demo_state.demo_fps)
        elapsed = 1.f/demo_state.demo_fps;
    if (demo_state.demo_pause && play_abs_time >= (U32)MSECS_TO_ABSTIME(demo_state.demo_pause))
        elapsed = 0;
    if (demo_state.demo_speed_scale)
        elapsed *= demo_state.demo_speed_scale;
    dt = (elapsed * ABSTIME_SECOND) + 0.5;
    if (demo_state.demo_pause && 
        play_abs_time < (U32)MSECS_TO_ABSTIME(demo_state.demo_pause) &&
        play_abs_time + dt >= (U32)MSECS_TO_ABSTIME(demo_state.demo_pause))
    {
        // Going to go past the pause point!
        dt = (U32)MSECS_TO_ABSTIME(demo_state.demo_pause) - play_abs_time;
    }
    play_abs_time += dt;
    game_state.client_loop_timer -= TIMESTEP; // Subtract what was previously added
    TIMESTEP = 30.f * elapsed;
    game_state.client_loop_timer += TIMESTEP; // Add in new value
    global_state.frame_time_30_real = TIMESTEP; 
    if (play_abs_time > abstime_end)
    {
        if (demo_state.demo_dump)
            windowExit(0);
        demoStop();
        play_abs_time = abstime_start;
        gNextUnplayedDynGroupFrame = 0;
        gNextUnplayedSkyFileFrame = 0;
        dumpPerfStats();
        server_visible_state.time = demo.clock_time;
    }
    if(TIMESTEP > 0.0)
    {
        demoPlayFrame(play_abs_time);
    }

    updateDynGroups();
    updateSkyFile();

    updateCamera();
}

void demoPlay()
{
    extern void engine_update();
    int            timer;
    extern int    g_win_ignore_popups;

    //g_win_ignore_popups = 1;
    demo_is_playing = 1;
    gNextUnplayedDynGroupFrame = 0;
    gNextUnplayedSkyFileFrame = 0;
    
//    texDisableThreadedLoading();
    timer = timerAlloc();
    ShowCursor(TRUE);
    uigameDemoForceMenu();
    if (demo_state.demo_dump)
        game_state.showfps = 0;

    loadScreenResetBytesLoaded();
    showBgAdd(2);
    game_state.game_mode = SHOW_LOAD_SCREEN;
    texDynamicUnload(0);
    texLoadQueueStart();

    demoStartPlay(demo_state.demoname);
    demoLoadMap(demo.map_name);
    groupDynBuildBreakableList(); //maybe more robustly groupTrackersHaveBeenReset(); 
    updateDynGroups();
    updateSkyFile();
    updateCamera();
    server_visible_state.time = demo.clock_time;
    gfxLoadRelevantTextures();
    forceGeoLoaderToComplete(); // This doesn't seem to do anything?
    texLoadQueueFinish();
    texDynamicUnload(1);
    showBgAdd(-2);
    game_state.game_mode = SHOW_GAME;

    server_visible_state.time = demo.clock_time;
    frame_total_timer = timerAlloc();
    control_state.predict = 0;
    global_state.client_abs = play_abs_time;
    FolderCacheEnableCallbacks(1);

    engine_update(); 
    last_play_abs_time = play_abs_time = abstime_start;
#if 1
    for(;;)
    {
        FrameTime    *ft;
        
        timerTickBegin();

        FolderCacheDoCallbacks(); // Check for directory changes

        if(inpEdge(INP_ESCAPE))
        {
            if(winMsgYesNo("Are you sure you want to exit demo playback?"))
            {
                windowExit(0);
            }
            
            noTimeAdvance = 1;
        }

        global_state.client_abs = play_abs_time;
         
        timerStart(timer);
        engine_update();

        if (play_abs_time < last_play_abs_time)
        {
            last_play_abs_time = play_abs_time;
            timerTickEnd();
            continue;
        }
        ft = dynArrayAdd(&frame_times,sizeof(frame_times[0]),&frame_count,&frame_max,1);
        ft->abs_time = last_play_abs_time;
        last_play_abs_time = play_abs_time;
        ft->render_time = timerElapsed(timer);
        if (demo_state.demo_dump)
        {
            char    subdir[1000];
            char * s;

            sprintf(subdir,"client_demos/screens/%s",demo_state.demoname);
            s = strrchr( subdir, '.' );
            if(s)
                *s = 0;
            gfxScreenDump(subdir,1,demo_state.demo_dump_tga ? "tga" : "jpg");
        }

        timerTickEnd();
    }
#else
    for(;;)
    {
        void entDebugAdvanceTimerHistory(void);
        
        FrameTime    *ft;
        
        autoTimerTickBegin();

        FolderCacheDoCallbacks(); // Check for directory changes

        if(!editMode() && inpEdge(INP_ESCAPE))
        {
            if(current_target)
            {
                current_target = NULL;
            }
            else
            {
                if(winMsgYesNo("Are you sure you want to exit demo playback?"))
                {
                    windowExit(0);
                }
            
                noTimeAdvance = 1;
            }
        }

        timerStart(timer);
        engine_update();

        if (play_abs_time < last_play_abs_time)
        {
            last_play_abs_time = play_abs_time;

            PERFINFO_AUTO_START("entDebugAdvanceTimerHistory", 1);
                entDebugAdvanceTimerHistory();
            PERFINFO_AUTO_STOP();
            
            autoTimerTickEnd();
            continue;
        }
        ft = dynArrayAdd(&frame_times,sizeof(frame_times[0]),&frame_count,&frame_max,1);
        ft->abs_time = last_play_abs_time;
        last_play_abs_time = play_abs_time;
        ft->render_time = timerElapsed(timer);
        if (demo_state.demo_dump)
        {
            char    subdir[1000];
            char * s;

            sprintf(subdir,"client_demos/screens/%s",demo_state.demoname);
            s = strrchr( subdir, '.' );
            if(s)
                *s = 0;
            gfxScreenDump(subdir,1,demo_state.demo_dump_tga ? "tga" : "jpg");
        }

        PERFINFO_AUTO_START("entDebugAdvanceTimerHistory", 1);
            entDebugAdvanceTimerHistory();
        PERFINFO_AUTO_STOP();

        autoTimerTickEnd();
    }
#endif
}

#endif
