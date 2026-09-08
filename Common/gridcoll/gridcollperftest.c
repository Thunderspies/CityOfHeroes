#include "gridcoll.h"
#include "gridcollperftest.h"
#include <utilitieslib/utils/mathutil.h>
#include "cmdparse/cmdserver.h"
#include <utilitieslib/utils/utils.h>
#include "gridcache.h"
#include <utilitieslib/utils/file.h>
#include "group/groupgrid.h"

typedef struct
{
    Vec3    start,end,hitpos;
    int        flags;
    F32        radius;
    int        got_hit;
} CollRecord;

CollRecord *coll_rec_list;
int            coll_rec_count,coll_rec_max;
int            length_counts[500];
int            coll_count,total_hit_count,total_hit2_count,findinside_count;
extern grid_cache_find,reuse_obj_count,full_search_count,obj_nocache_count;
extern find_inside_count,find_inside_cache_count,objcache_cant_fit;

static int coll_recording;

void gridCollRecordStart()
{
    coll_recording = 1;
    dynArrayFit(&coll_rec_list,sizeof(coll_rec_list[0]),&coll_rec_max,1000000);
    coll_rec_count = 0;
}

void gridCollRecordStop()
{
    FILE    *file;

    coll_recording = 0;
    file = fopen("c:/temp/gridcoll.bin","wb");
    fwrite(coll_rec_list,sizeof(coll_rec_list[0]),coll_rec_count,file);
    fclose(file);
    free(coll_rec_list);
    coll_rec_max = 0;
}

void saveCollRecX(const Vec3 start,const Vec3 end,F32 radius, U32 flags, int got_hit, const Vec3 hit_pos)
{
    CollRecord *coll_rec;

    if (!coll_recording)
        return;

    coll_rec = dynArrayAdd(&coll_rec_list,sizeof(coll_rec_list[0]),&coll_rec_count,&coll_rec_max,1);
    copyVec3(start,coll_rec->start);
    copyVec3(end,coll_rec->end);
    copyVec3(hit_pos,coll_rec->hitpos);
    coll_rec->radius = radius;
    coll_rec->flags = flags;
    coll_rec->got_hit = got_hit;
}

void loadCollRec()
{
    int        bytes;

    coll_rec_list = fileAlloc("c:/temp/gridcoll.bin",&bytes);
    coll_rec_count = bytes / sizeof(CollRecord);
}

int coll_hit_err_count,coll_hitpos_err_count,bothsides_count;

int has_rad_count,hit_diff_count;

void coll_err(int pos_err)
{
    if (pos_err)
        coll_hitpos_err_count++;
    coll_hit_err_count++;
}

int glob_timer;

#include <utilitieslib/utils/timing.h>


static U8 *dup_checker;
static int dup_count;

static void dupCount()
{
    int        i,j;
    CollRecord    *c1,*c2;

    dup_checker = calloc(coll_rec_count,1);
    for(i=0;i<coll_rec_count;i++)
    {
        c1 = &coll_rec_list[i];
        for(j=i+1;j<coll_rec_count;j++)
        {
            c2 = &coll_rec_list[j];
            if (!dup_checker[j] && c1->radius == c2->radius && nearSameVec3(c1->start,c2->start) && nearSameVec3(c1->end,c2->end))
            {
                dup_checker[j]=1;
                dup_count++;
            }
        }
    }
}

void quantizeVec3(Vec3 v)
{
    int        ivec[3];
    #define FOOTFRAC 2.f

    scaleVec3(v,FOOTFRAC,v);
    qtruncVec3(v,ivec);
    copyVec3(ivec,v);
    scaleVec3(v,1.f/FOOTFRAC,v);
}

void runCollRec()
{
    int            i,hit;
    CollRecord    *collrec;
    CollInfo    coll = {0};
    Vec3        dv;
    F32            l;
    int            timer;
    int            checkfoot_count=0,slide_count=0,over_one_foot_count=0;

    //dupCount();
    glob_timer = timer = timerAlloc();
    timerStart(timer);
    coll_count=total_hit_count=total_hit2_count=findinside_count=0;
    coll_hit_err_count=coll_hitpos_err_count=bothsides_count=0;
    grid_cache_find=0;
    reuse_obj_count=full_search_count=obj_nocache_count=0;
    find_inside_count=find_inside_cache_count=0;
    objcache_cant_fit=0;

    for(i=0;i<coll_rec_count;i++)
    {
        collrec = &coll_rec_list[i];
        if (collrec->radius)
            has_rad_count++;
        if (collrec->flags & (COLL_BOTHSIDES|COLL_DISTFROMCENTER))
            bothsides_count++;
        subVec3(collrec->end,collrec->start,dv);
        l = lengthVec3(dv);
        if (0 && l > 8)
            continue;
        coll_count++;
        if (l > ARRAY_SIZE(length_counts))
            l = ARRAY_SIZE(length_counts)-1;
        length_counts[(int)l]++;
#if 0
        quantizeVec3(collrec->start);
        quantizeVec3(collrec->end);
#endif

        if (collrec->flags & COLL_DISTFROMCENTER)
            slide_count++;
        if (collrec->radius > 0.7 && collrec->radius < 0.8)
            over_one_foot_count++;
        if (collrec->radius==1.f && fabs(collrec->start[1] - collrec->end[1]) < 4)
        {
            checkfoot_count++;
        }
        if (collrec->flags & COLL_FINDINSIDE)
        {
            extern FindInsideOpts glob_find_type;
            extern int findInsideTest(void *tracker, int backside);

            glob_find_type = FINDINSIDE_VOLUMETRIGGER;
            coll.node_callback = findInsideTest;
            findinside_count++;
            if (0)
                continue;
        }
        hit = collGrid(0,collrec->start,collrec->end,&coll,collrec->radius,collrec->flags);
        total_hit_count += hit;
        if (hit != collrec->got_hit)
        {
#if 0
        DefTracker    *tracker;

            safe_mode = 1;
            hit = collGrid(0,collrec->start,collrec->end,&coll,collrec->radius,collrec->flags);
            tracker = coll.node;

            safe_mode = 0;
            hit = collGrid(0,collrec->start,collrec->end,&coll,collrec->radius,collrec->flags);
#endif
            coll_err(0);
        }
        if (hit)
        {
            if (!nearSameVec3(coll.mat[3],collrec->hitpos) && !(collrec->flags & COLL_HITANY))
                coll_err(1);
        }
    }

    printf("tested %d collisions in %f seconds:  %f coll/sec\n",coll_rec_count,timerElapsed(timer),(coll_rec_count) / timerElapsed(timer));
    printf("err count: %d   hitpos err count %d\n",coll_hit_err_count,coll_hitpos_err_count);
    printf("slide %d   checkfoot %d   over_one_foot %d\n",slide_count,checkfoot_count,over_one_foot_count);
    printf("find_inside %d   find_inside_cache_hit %d\n",find_inside_count,find_inside_cache_count);
    printf("coll cachehits: %d\n",grid_cache_find);
    printf("total searches: %d   obj_reuse: %d  uncached %d  cant_fit %d\n",full_search_count,reuse_obj_count,obj_nocache_count,objcache_cant_fit);
    printf("\n");
    timerFree(timer);
    //checkCubeHash();
}

void gridCollPerfTest()
{
    printf("\n");
    loadCollRec();
    //VTResume();
    //memCheckDumpAllocs();
    gridCacheSetSize(14);
    runCollRec();
    runCollRec();
    for(;;)
        runCollRec();

    //memCheckDumpAllocs();
    //runCollRec();
    //VTPause();
    exit(0);
}

