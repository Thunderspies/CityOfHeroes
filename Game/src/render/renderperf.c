#define RT_PRIVATE
#include "render/model_cache.h"
#include <stdio.h>
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/error.h>
#include "assert.h" 
#include <utilitieslib/utils/mathutil.h>
#include "render/render.h"
#include "cmdparse/cmdgame.h"
#include "group/groupfileload.h"
#include "UI/uiConsole.h"
#include "render/thread/rt_perf.h"
#include "render/renderperf.h"
#include "group/group.h"
#include "render/thread/ogl.h"
#include "graphics/sun.h"
#include "graphics/gfxDebug.h"
#include <utilitieslib/utils/file.h>

#define MILLION 1000000.f

typedef struct
{
    int trisPerModel;
    float cost;
    float avgTimePerCall;
} PerfTestResult2;

typedef struct
{
    PerfTestResultsHeader header;
    PerfTestResult2 **testResults;
} PerfTestResults2;


void freePerfTestResults(PerfTestResults *results)
{
    if (!results)
        return;
    eaClearEx(&results->testResults, 0);
    free(results);
}

static void setupResults(PerfTestResults *results, int numTests)
{
    if (!results)
        return;

    if (results->testResults)
        eaClearEx(&results->testResults, 0);
    eaCreate(&results->testResults);
    results->header.maxThroughput = 0;
    results->header.numTests = numTests;
}

static void setupResults2(PerfTestResults2 *results, int numTests)
{
    if (!results)
        return;

    if (results->testResults)
        eaClearEx(&results->testResults, 0);
    eaCreate(&results->testResults);
    results->header.maxThroughput = 0;
    results->header.numTests = numTests;
}

static PerfTestResult *createPerfTestResult(PerfTestResults *results)
{
    PerfTestResult *test;
    if (!results)
        return 0;

    test = (PerfTestResult *)calloc(1, sizeof(PerfTestResult));
    eaPush(&results->testResults, test);

    return test;
}

static PerfTestResult2 *createPerfTestResult2(PerfTestResults2 *results)
{
    PerfTestResult2 *test;
    if (!results)
        return 0;

    test = (PerfTestResult2 *)calloc(1, sizeof(PerfTestResult2));
    eaPush(&results->testResults, test);

    return test;
}

typedef struct 
{
    char *modelName;
    Model *model;
} PerfModelStruct;

static char *PerfModelPrefixes[] = 
{
    "PerfModel_rock_gray_",
    "PerfModel2_rock_gray_",
};

static struct
{
    int extNum;
    Model *models[5];
} PerfModels[] = 
{
    {18, {0}},
    {32, {0}},
    {50, {0}},
    {72, {0}},
    {98, {0}},
    {200, {0}},
    {392, {0}},
    {450, {0}},
    {578, {0}},
    {968, {0}},
    //{1682, {0}},
    //{2450, {0}},
    //{3362, {0}},
    //{4418, {0}},
    //{5618, {0}},
    {7200, {0}},
};

static void preloadModels()
{
    GroupDef *def;
    int i, j;
    char buffer[1024];

    for (i = 0; i < ARRAY_SIZE(PerfModels); i++)
    {
        for (j = 0; j < ARRAY_SIZE(PerfModelPrefixes); j++)
        {
            if (PerfModels[i].models[j])
                return;

            sprintf(buffer, "%s%d", PerfModelPrefixes[j], PerfModels[i].extNum);
            groupFileLoadFromName(buffer);
            def = groupDefFind(buffer);
            modelSetupVertexObject(def->model, VBOS_USE, BlendMode(0,0));
            PerfModels[i].models[j] = def->model;
        }
    }
}

static void printStates()
{
    int i;

#define printBlendState(mode) if (setBlendStateCalls[mode]) conPrintf(" %s: %d/%d", blend_mode_names[mode], setBlendStateChanges[mode],setBlendStateCalls[mode]);
#define printDrawState(mode) if (setDrawStateCalls[mode]) conPrintf(" " #mode ": %d/%d", setDrawStateChanges[mode],setDrawStateCalls[mode]);

    for (i=0; i<BLENDMODE_NUMENTRIES; i++) 
        printBlendState(i);

    printDrawState(DRAWMODE_SPRITE);
    printDrawState(DRAWMODE_DUALTEX);
    printDrawState(DRAWMODE_COLORONLY);
    printDrawState(DRAWMODE_DUALTEX_NORMALS);
    printDrawState(DRAWMODE_FILL);
    printDrawState(DRAWMODE_BUMPMAP_SKINNED);
    printDrawState(DRAWMODE_BUMPMAP_SKINNED_MULTITEX);
    printDrawState(DRAWMODE_HW_SKINNED);
    printDrawState(DRAWMODE_BUMPMAP_NORMALS);
    printDrawState(DRAWMODE_BUMPMAP_DUALTEX);
    printDrawState(DRAWMODE_BUMPMAP_RGBS);
    printDrawState(DRAWMODE_BUMPMAP_MULTITEX);
    printDrawState(DRAWMODE_BUMPMAP_MULTITEX_RGBS);

#undef printBlendState
#undef printDrawState
}

static int addModelToPerfInfo(RdrPerfInfo *perfinfo, Model *model)
{
    RdrTexList    *texlist;
    int i;

    if (!model)
        return 0;

    i = perfinfo->num_models;

    perfinfo->models[i].model.vbo = model->vbo;
    perfinfo->models[i].model.alpha = 255;
    copyVec4(g_sun.ambient, perfinfo->models[i].model.ambient );
    copyVec4(g_sun.diffuse, perfinfo->models[i].model.diffuse );

    texlist = modelDemandLoadTextures(model,0,1,-1,0,true,0);
    memcpy(&(perfinfo->models[i].texlists),texlist,sizeof(RdrTexList) * model->tex_count);

    perfinfo->num_models++;

    return model->vbo->tri_count;
}

static void doPerfTest(RdrPerfInfo *testinfo, int test_num, int num_runs, int loop_count, float *avgThroughput, float *avgCalls, double *timeToDraw, U64 *trisDrawn)
{
    RdrPerfIO    perfio, *perfiop = &perfio;
    RdrPerfInfo *perfinfo;
    U64 totalTris = 0;
    U64 totalCalls = 0;
    double totalTime = 0;
    int i;

    perfio.test_num    = test_num;
    perfio.loop_count = loop_count;

    for (i = 0; i < num_runs; i++)
    {
        rdrQueue(DRAWCMD_PERFSET,&perfiop,sizeof(RdrPerfIO*));

        perfinfo = rdrQueueAlloc(DRAWCMD_PERFTEST,sizeof(RdrPerfInfo));
        memcpy(perfinfo,testinfo,sizeof(RdrPerfInfo));

        rdrQueueSend();
        rdrQueueFlush();

        totalTris += perfio.trisDrawn;
        totalCalls += perfio.repeats * perfio.loop_count;
        totalTime += perfio.elapsed;
    }

    if (avgCalls)
        *avgCalls = (float)(totalCalls / totalTime);
    if (avgThroughput)
        *avgThroughput = (float)(totalTris / totalTime);
    if (timeToDraw)
        *timeToDraw = totalTime;
    if (trisDrawn)
        *trisDrawn = totalTris;
}

static int setupPerfInfo(RdrPerfInfo *perfinfo, int test_num, int modelIndex)
{
    int trisPerModel = 0;
    memset(perfinfo, 0, sizeof(RdrPerfInfo));
    trisPerModel = addModelToPerfInfo(perfinfo,PerfModels[modelIndex].models[0]);
    if (test_num)
        trisPerModel = addModelToPerfInfo(perfinfo,PerfModels[modelIndex].models[1]);

    return trisPerModel;
}

static void renderPerfTest2(int test_num, int num_runs, PerfTestResults2 *results, float maxThroughput)
{
    RdrPerfInfo    perfinfo;
    int i, trisPerModel;
    float avgThroughput, avgCalls;
    PerfTestResult2 *test;

    preloadModels();

    setupResults2(results, ARRAY_SIZE(PerfModels));

    for (i = 0; i < ARRAY_SIZE(PerfModels); i++)
    {
        trisPerModel = setupPerfInfo(&perfinfo, test_num, i);
        if (!trisPerModel)
            continue;

        doPerfTest(&perfinfo, test_num, num_runs, 2048, &avgThroughput, &avgCalls, 0, 0);

        test = createPerfTestResult2(results);
        if (test)
        {
            test->trisPerModel = trisPerModel;
            test->avgTimePerCall = 1.f / avgCalls;
            test->cost = (maxThroughput - avgThroughput) / avgCalls;
        }

        conPrintf("Tris per model: %d", trisPerModel);
        conPrintf(" Cost: %f  tris/call", (maxThroughput - avgThroughput) / avgCalls);
        conPrintf(" Time: %f  microseconds/call", (1.f / avgCalls) * MILLION);
        conPrintf(".");
    }
}

void renderPerfTest(int test_num, int num_runs, PerfTestResults *results)
{
    RdrPerfInfo    perfinfo;
    int i, trisPerModel;
    float avgThroughput, avgCalls, avgTimePerCall, maxThroughput = 0, minTimePerCall;
    PerfTestResult *test;

    preloadModels();

    setupResults(results, ARRAY_SIZE(PerfModels));

    for (i = 0; i < ARRAY_SIZE(PerfModels); i++)
    {
        trisPerModel = setupPerfInfo(&perfinfo, test_num, i);
        if (!trisPerModel)
            continue;

        doPerfTest(&perfinfo, test_num, num_runs, 2048, &avgThroughput, &avgCalls, 0, 0);
        avgTimePerCall = 1.f / avgCalls;

        test = createPerfTestResult(results);
        if (test)
        {
            test->trisPerModel = trisPerModel;
            test->avgTimePerCall = avgTimePerCall;
            test->avgCalls = avgCalls;
            test->avgThroughput = avgThroughput;
        }

        if (avgThroughput > maxThroughput)
            maxThroughput = avgThroughput;

        if (i == 0 || avgTimePerCall < minTimePerCall)
            minTimePerCall = avgTimePerCall;
    }

    if (results)
    {
        results->header.maxThroughput = maxThroughput;
        results->header.minTimePerCall = minTimePerCall;
    }

    conPrintf("Max throughput: %.3f million tris/second", maxThroughput / MILLION);
    conPrintf("Min time: %.3f microseconds/call", minTimePerCall * MILLION);
    conPrintf(".");
}

#define TESTS_TO_RUN 32

void renderPerfTestSuite(int num_runs)
{
    char path[MAX_PATH], filename[MAX_PATH], cardName[MAX_PATH];
    PerfTestResults results;
    PerfTestResults2 results2[TESTS_TO_RUN];
    FILE *f;
    int i, j, k, bIsFirstTime;
    char *p;

    memset(&results,0, sizeof(PerfTestResults));
    for (i = 0; i < TESTS_TO_RUN; i++)
        memset(&results2[i],0, sizeof(PerfTestResults2));

    renderPerfTest(0, num_runs, &results);

    // generate the info for results2[0], no need to run the actual test again
    {
        PerfTestResult2 *test;
        setupResults2(&results2[0], results.header.numTests);
        for (i = 0; i < ARRAY_SIZE(PerfModels); i++)
        {
            test = createPerfTestResult2(&results2[0]);
            test->trisPerModel = results.testResults[i]->trisPerModel;
            test->cost = (results.header.maxThroughput - results.testResults[i]->avgThroughput) / results.testResults[i]->avgCalls;
            test->avgTimePerCall = results.testResults[i]->avgTimePerCall;
        }
    }

    for (i = 1; i < TESTS_TO_RUN; i++)
        renderPerfTest2(i, num_runs, &results2[i], results.header.maxThroughput);

    sprintf(path, "C:\\PerfTest\\");
    makeDirectories(path);
    
    strcpy(filename, path);
    strcat(filename, "Throughput.csv");

    bIsFirstTime = 1;
    if (f = fopen(filename, "rt"))
    {
        bIsFirstTime = 0;
        fclose(f);
    }

    if (f = fopen(filename, "at"))
    {
        if (bIsFirstTime)
        {
            fprintf(f, "Tris per Model: ");
            for (i = 0; i < results.header.numTests; i++)
                fprintf(f, ", %d", results.testResults[i]->trisPerModel);
            fprintf(f, "\n");
        }

        fprintf(f, systemSpecs.videoCardName);
        for (i = 0; i < results.header.numTests; i++)
            fprintf(f, ", %f", results.testResults[i]->avgThroughput);
        fprintf(f, "\n");

        fclose(f);
    }

    strcpy(cardName, systemSpecs.videoCardName);
    p = cardName;
    while (p = strchr(p, '/'))
        *(p++) = ' ';
    p = cardName;
    while (p = strchr(p, '\\'))
        *(p++) = ' ';

    for (k = 0; k < 3; k++)
    {
        strcpy(filename, path);
        if (k == 0)
            strcat(filename, "costs for ");
        else if (k == 1)
            strcat(filename, "times for ");
        else
            strcat(filename, "table for ");
        strcat(filename, cardName);
        if (k == 0 || k == 1)
            strcat(filename, ".csv");
        else
            strcat(filename, ".txt");
        
        if (f = fopen(filename, "wt"))
        {
            if (k == 0 || k == 1)
                fprintf(f, "Tris per Model: ,");
            else
                fprintf(f, "{");

            for (i = 0; i < results.header.numTests; i++)
            {
                if (i == 0)
                    fprintf(f, "%d", results.testResults[i]->trisPerModel);
                else
                    fprintf(f, ", %d", results.testResults[i]->trisPerModel);
            }

            if (k == 2)
                fprintf(f, ", -1},\n{");

            fprintf(f, "\n");

            for (j = 0; j < TESTS_TO_RUN; j++)
            {
                if (k == 0)
                    fprintf(f, "Tri cost (DrawElements");
                else if (k == 1)
                    fprintf(f, "Microseconds (Triangles DrawElements");
                else
                    fprintf(f, "{");

                if (k != 2 && j & 1)
                    fprintf(f, " & VBO");

                if (k != 2 && j & 2)
                    fprintf(f, " & FP");

                if (k != 2 && j & 4)
                    fprintf(f, " & VP");

                if (k != 2 && j & 24)
                    fprintf(f, " & %d textures", (j & 24) >> 3);

                if (k != 2)
                    fprintf(f, "): ,");

                for (i = 0; i < results2[j].header.numTests; i++)
                {
                    if (i == 0)
                    {
                        if (k == 0)
                            fprintf(f, "%f", results2[j].testResults[i]->cost);
                        else if (k == 1)
                            fprintf(f, "%f", results2[j].testResults[i]->avgTimePerCall * MILLION);
                        else
                            fprintf(f, "%ff", results2[j].testResults[i]->avgTimePerCall * MILLION);
                    }
                    else if (k == 0)
                        fprintf(f, ", %f", results2[j].testResults[i]->cost);
                    else if (k == 1)
                        fprintf(f, ", %f", results2[j].testResults[i]->avgTimePerCall * MILLION);
                    else
                        fprintf(f, ", %ff", results2[j].testResults[i]->avgTimePerCall * MILLION);
                }
                if (k == 2)
                    fprintf(f, "},");
                fprintf(f, "\n");
            }

            if (k == 2)
                fprintf(f, "},\n");

            fclose(f);
        }
    }

    eaClearEx(&(results.testResults), 0);
    for (j = 0; j < TESTS_TO_RUN; j++)
        eaClearEx(&(results2[j].testResults), 0);
}


