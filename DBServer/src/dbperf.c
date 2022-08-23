#include <stdio.h>
#include "container.h"
#include "dbinit.h"
#include "container_flatfile.h"
#include <utilitieslib/utils/timing.h>
#include "container_sql.h"
#include <utilitieslib/utils/MemoryMonitor.h>
#include "comm_backend.h"
#include "dbperf.h"
#include <utilitieslib/components/EString.h>
#include <utilitieslib/utils/mathutil.h>
#include "dbdispatch.h"
#include <utilitieslib/utils/file.h>
#include "sql_fifo.h"

#define LOOPNUM 50000
//#define LOOPNUM 10

typedef struct
{
    U32        bytes;
    U32        count;
} DbCmdStat;

DbCmdStat    cmds_in[DBCLIENT_NUMCMDS];
DbCmdStat    cmds_out[DBSERVER_NUMCMDS];
DbCmdStat    cmds_get[MAX_CONTAINER_TYPES];
DbCmdStat    cmds_set[MAX_CONTAINER_TYPES];
DbCmdStat    cmds_special[DBPERF_NUMCMDS];

static int perf_timer;

void logSpecialCmd(int idx,int size)
{
    cmds_special[idx].bytes += size;
    cmds_special[idx].count++;
}

void logStatCmdIn(int type,int size)
{
    cmds_in[type].bytes += size;
    cmds_in[type].count++;
    logSpecialCmd(DBPERF_CMDS_IN,size);
}

void logStatGetContainer(int type,int size)
{
    cmds_get[type].bytes += size;
    cmds_get[type].count++;
}

void logStatSetContainer(int type,int size)
{
    cmds_set[type].bytes += size;
    cmds_set[type].count++;
}

void logStatCmdOut(int type,int size)
{
    cmds_out[type].bytes += size;
    cmds_out[type].count++;
}

#define CMDLOGTIME 60.0

static void resetPerfLog()
{
    memset(cmds_special,0,sizeof(cmds_special));
    memset(cmds_set,0,sizeof(cmds_set));
    memset(cmds_get,0,sizeof(cmds_get));
    memset(cmds_out,0,sizeof(cmds_out));
    memset(cmds_in,0,sizeof(cmds_in));
    timerStart(perf_timer);
}

#define CMDFUN(x)    x[i].count/timerElapsed(perf_timer),(x[i].count ? x[i].bytes / ((F32)x[i].count) : 0)

static void printStatsLine(char *msg,DbCmdStat *stats,int count)
{
    int        i;

    printf("\n");
    for(i=0;i<count;i++)
    {
        printf("%s %2d %-8.1f %-8.1f\n",msg,i,stats[i].count/timerElapsed(perf_timer),
            (stats[i].count ? stats[i].bytes / ((F32)stats[i].count) : 0));
    }
}

void logStatPrint(int type)
{
    printf("\n\n");
    switch(type)
    {
        xcase 'i':
            printStatsLine("in ",cmds_in,DBCLIENT_NUMCMDS);
        xcase 'g':
            printStatsLine("get",cmds_get,MAX_CONTAINER_TYPES);
        xcase 's':
            printStatsLine("set",cmds_set,MAX_CONTAINER_TYPES);
        xcase 'a':
            printStatsLine("agg",cmds_special,DBPERF_NUMCMDS);
    }
    printf("Sampled for %.1f seconds\n",timerElapsed(perf_timer));
    resetPerfLog();
}

void flatToSql()
{
    containerListLoadFile(ent_list);
    containerListLoadFile(supergroups_list);
    containerListLoadFile(teamups_list);
    containerListLoadFile(league_list);
}

void sqlToFlat()
{
    int        i,count=0,num_containers;
    EntCon    *ent_con;
    char    *mem;
    char    cmd_buf[1000];

    int buf_len = sprintf(cmd_buf, "SELECT COUNT(ContainerId) FROM %s", ent_list->tplt->tables->name);
    num_containers = sqlGetSingleValue(cmd_buf, buf_len, NULL, SQLCONN_FOREGROUND);
    for(i=0;count < num_containers;i++)
    {
        mem = sqlContainerRead(ent_list->tplt,i,SQLCONN_FOREGROUND);
        if (mem)
        {
            free(mem);
            ent_con = containerLoad(ent_list,i);
            count++;
        }
    }
    containerListSave(ent_list);
    containerListSave(supergroups_list);
    containerListSave(teamups_list);
    containerListSave(league_list);
}

static int * ilist = NULL;
static int ilist_max = 0;

void dupContainer(unsigned runs)
{
    unsigned run;
    int timer = timerAlloc();
    unsigned found;
    unsigned i;
    char * container_texts[LOOPNUM];

    memtrack_mark(0, "perfdup start");

    printf("Finding first %d entities\n", LOOPNUM);
    found = allContainerIds(&ilist, &ilist_max, CONTAINER_ENTS, LOOPNUM);

    // Prefetch the containers
    for (i=0; i < found; i++)
        sqlReadContainerAsync(CONTAINER_ENTS, ilist[i], NULL, NULL, NULL);

    // Extract the container text
    for (i=0; i < found; i++)
    {
        DbContainer *container;

        sqlFifoTick();
        while (sqlIsAsyncReadPending(CONTAINER_ENTS, ilist[i])) {
            Sleep(1);
            sqlFifoTick();
            checkExitRequest();
        }

        if ((container = containerLoad(ent_list, ilist[i])))
            container_texts[i] = strdup(containerGetText(container));
    }
    printf("Found %d entities\n", found);
    sqlFifoFinish();
    assert(found);

    for(run=0; !runs || run < runs; run++)
    {
        memtrack_mark(0, "perfdup tick");

        timerStart(timer);
        for(i=0; i<found; i++)
        {
            DbContainer *container = containerAlloc(ent_list, -1);
            containerUpdate(ent_list, container->id, container_texts[i], 1);
            containerUnload(ent_list, container->id);
            sqlFifoTick();
            checkExitRequest();
        }
        sqlFifoFinish();
        printf("Created %f per second\n", found / timerElapsed(timer));

        // Check for memory leaks
        //memMonitorLogStats();
    }

    // Unload
    for (i=0; i < found; i++) {
        free(container_texts[i]);
        containerUnload(ent_list, ilist[i]);
    }

    free(ilist);
    timerFree(timer);

    memtrack_mark(0, "perfdup end");
}

void perfLoad(unsigned runs)
{
    unsigned run;
    int timer = timerAlloc();
    unsigned found;
    int * ilist = NULL;
    int ilist_max = 0;
    unsigned i;

    memtrack_mark(0, "perfload start");

    printf("Finding first %d entities\n", LOOPNUM);
    found = allContainerIds(&ilist, &ilist_max, CONTAINER_ENTS, LOOPNUM);
    printf("Found %d entities\n", found);
    assert(found);
    sqlFifoFinish();

    for(run=0; !runs || run < runs; run++)
    {
        memtrack_mark(0, "perfload tick");

        timerStart(timer);
        for(i=0; i<found; i++)
        {
            sqlReadContainerAsync(CONTAINER_ENTS, ilist[i], NULL, NULL, NULL);
            sqlFifoTick();
            checkExitRequest();
        }
        sqlFifoFinish();
        printf("Loaded %f per second\n", found / timerElapsed(timer));

        // Unload to repeat
        for(i=0; i<found; i++)
            containerUnload(ent_list, ilist[i]);
        sqlFifoFinish();

        // Check for memory leaks
        //memMonitorLogStats();
    }

    free(ilist);
    timerFree(timer);

    memtrack_mark(0, "perfload end");
}

void perfUpdate(unsigned runs)
{
    unsigned run;
    int timer = timerAlloc();
    unsigned found;
    int * ilist = NULL;
    int ilist_max = 0;
    unsigned i;
    char * cmd = estrTemp();

    memtrack_mark(0, "perfupdate start");

    qsrand(0);

    printf("Finding first %d entities\n", LOOPNUM);
    found = allContainerIds(&ilist, &ilist_max, CONTAINER_ENTS, LOOPNUM);

    // Prefetch the containers
    for (i=0; i < found; i++)
        sqlReadContainerAsync(CONTAINER_ENTS, ilist[i], NULL, NULL, NULL);

    printf("Found %d entities\n", found);
    assert(found);
    sqlFifoFinish();

    for(run=0; !runs || run < runs; run++)
    {
        memtrack_mark(0, "perfupdate tick");
        estrPrintf(&cmd, "MapId %d\nPosX %f\nPosY %f\nPosZ %f\nOrientY %f\n", qrand(), qfrand(), qfrand(), qfrand(), qufrand());

        timerStart(timer);
        for(i=0; i<found; i++)
        {
            DbContainer *container = container = containerLoad(ent_list, ilist[i]);
            containerUpdate(ent_list, container->id, cmd, 1);
            sqlFifoTick();
            checkExitRequest();
        }
        sqlFifoFinish();
        printf("Updated %f per second\n", found / timerElapsed(timer));

        // Check for memory leaks
        //memMonitorLogStats();
    }

    // Unload
    for(i=0; i<found; i++)
        containerUnload(ent_list, ilist[i]);
    sqlFifoFinish();

    free(ilist);
    timerFree(timer);

    memtrack_mark(0, "perfupdate end");
}

void modifyContainersAsync(unsigned containers)
{
    unsigned found;
    unsigned i;
    char * cmd = estrTemp();

    printf("Finding first %d entities\n", containers);
    found = allContainerIds(&ilist, &ilist_max, CONTAINER_ENTS, containers);

    // Prefetch the containers
    for (i=0; i < found; i++)
        sqlReadContainerAsync(CONTAINER_ENTS, ilist[i], NULL, NULL, NULL);

    printf("Found %d entities\n", found);
    assert(found);
    sqlFifoFinish();

    estrPrintf(&cmd, "MapId %d\nPosX %f\nPosY %f\nPosZ %f\nOrientY %f\n", qrand(), qfrand(), qfrand(), qfrand(), qufrand());

    // Generate some load
    for (i=0; i < found; i++)
        sqlReadContainerAsync(CONTAINER_ENTS, ilist[i], NULL, NULL, NULL);

    for(i=0; i<found; i++)
    {
        DbContainer *container = container = containerLoad(ent_list, ilist[i]);
        containerUpdate(ent_list, container->id, cmd, 1);
        checkExitRequest();
    }

    free(ilist);
}
