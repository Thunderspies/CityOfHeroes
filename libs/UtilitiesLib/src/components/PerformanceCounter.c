#ifndef _XBOX // xbox has no pdh stuff
#include "utilitieslib/utils/wininclude.h"
#include <pdhmsg.h>
#include <conio.h>
#include <stdio.h>
#include <pdh.h>
#include "utilitieslib/assert/assert.h"
#include "utilitieslib/components/earray.h"
#include "utilitieslib/utils/error.h"

#pragma comment(lib,"pdh.lib")


typedef struct CounterRef {
    HCOUNTER hCounter;
    LONG *storage;
} CounterRef;

typedef struct PerformanceCounter {
    HQUERY hQuery;
    char ***eaInstances;
    char **eaInstances_data;
    CounterRef ***eaCounters;
    CounterRef **eaCounters_data;
    char *instanceList;
    char *interfaceName;
} PerformanceCounter;

// Walks a string of strings
static void printList(char *s) {
    char *walk;
    for (walk=s; *walk;) {
        printf("\t%s\n", walk);
        walk += strlen(walk)+1;
    }
}

void performanceCounterDestroy(PerformanceCounter *counter)
{
    int i;
    if (!counter)
        return;
    for (i=0; i<eaSize(counter->eaCounters); i++) {
        PdhRemoveCounter(counter->eaCounters_data[i]->hCounter);
    }
    eaDestroyEx(counter->eaCounters, NULL);
    eaDestroy(counter->eaInstances);
    if (counter->hQuery) {
        PdhCloseQuery(counter->hQuery);
        counter->hQuery = NULL;
    }
    if (counter->interfaceName) {
        free(counter->interfaceName);
        counter->interfaceName = NULL;
    }
    if (counter->instanceList) {
        free(counter->instanceList);
        counter->instanceList = NULL;
    }
}

PerformanceCounter *performanceCounterCreate(const char *interfaceName)
{
    PDH_STATUS           pdhStatus = ERROR_SUCCESS;
    LPSTR               szCounterListBuffer = NULL;
    DWORD                dwCounterListSize  = 0;
    LPSTR               szInstanceListBuffer = NULL;
    DWORD                dwInstanceListSize  = 0;
    PerformanceCounter   *counter = calloc(sizeof(PerformanceCounter),1);
    counter->eaInstances = &counter->eaInstances_data;
    counter->eaCounters = &counter->eaCounters_data;
    counter->interfaceName = strdup(interfaceName);

    // query for the sizes of the counter and instance list buffers
    // needed to enumerate
    pdhStatus = PdhEnumObjectItemsA (NULL,NULL,
        counter->interfaceName,
        szCounterListBuffer,
        &dwCounterListSize,
        szInstanceListBuffer,
        &dwInstanceListSize,
        PERF_DETAIL_WIZARD,
        0);

    if(    pdhStatus != ERROR_SUCCESS &&
        pdhStatus != PDH_MORE_DATA )
    {
        Errorf("Unable to query performance counter '%s' (Error: 0x%x)", interfaceName, pdhStatus);
        return NULL;
    }

    // counter and instance list sizes can be 0 in which case NULL should be supplied
    if (dwCounterListSize)
    {
        szCounterListBuffer = (LPSTR)malloc ((dwCounterListSize * sizeof (char)));
        if(!szCounterListBuffer)
        {
            printf ("unable to allocate performance counter list buffer\n");
            performanceCounterDestroy(counter);
            return NULL;
        }
    }

    // not all counters have instances (e.g. "Memory" counters)
    if (dwInstanceListSize)
    {
        szInstanceListBuffer = (LPSTR)malloc ((dwInstanceListSize * sizeof (char)));
        if(!szInstanceListBuffer)
        {
            free(szCounterListBuffer);
            printf ("unable to allocate performance counter instance buffer\n");
            performanceCounterDestroy(counter);
            return NULL;
        }
    }

    pdhStatus = PdhEnumObjectItemsA (NULL,NULL,
        counter->interfaceName,
        szCounterListBuffer,
        &dwCounterListSize,
        szInstanceListBuffer,
        &dwInstanceListSize,
        PERF_DETAIL_WIZARD,
        0);

    if(pdhStatus == ERROR_SUCCESS) 
    {
        char *walk;
        counter->instanceList = szInstanceListBuffer;
        eaCreate(counter->eaInstances);
        if (szInstanceListBuffer)                // not all counters have instances (e.g. "Memory" counters)
        {
            for (walk=szInstanceListBuffer; *walk;) {
                if (stricmp(walk, "MS TCP Loopback interface")==0) {
                    // Don't add the loopback adapter for networking counters
                } else if (stricmp(walk, "_Total")==0) {
                    // There's a total, just use it!
                    eaSetSize(counter->eaInstances, 0);
                    eaPush(counter->eaInstances, walk);
                    break;
                } else {
                    eaPush(counter->eaInstances, walk);
                }
                walk += strlen(walk) + 1;
            }
        }
    }
    else
    {
        free(szCounterListBuffer);
        free(szInstanceListBuffer);
        performanceCounterDestroy(counter);
        printf ("unable to allocate performance buffer\n");
        return NULL;
    }

    free(szCounterListBuffer);

    if( PdhOpenQuery(NULL ,0 ,&counter->hQuery))
    {
        printf("PdhOpenQuery failed\n");
        performanceCounterDestroy(counter);
        return NULL;
    }

    return counter;
}

int performanceCounterAdd(PerformanceCounter *counter, const char *counterName, LONG *storage)
{
    int i;
    int num_instances = eaSize(counter->eaInstances);

    if (num_instances>0)
    {
        // counter has multiple instances...
        for (i=0; i<num_instances; i++)
        {
            char szCounter[256];
            CounterRef *ref = calloc(sizeof(CounterRef),1);
            sprintf_s(SAFESTR(szCounter),"\\%s(%s)\\%s",
                counter->interfaceName, counter->eaInstances_data[i], counterName);
            if( PdhAddCounterA(counter->hQuery,szCounter,(DWORD_PTR)NULL,&ref->hCounter))
            {
                printf("PdhAddCounter failed : %s\n", szCounter);
                free(ref);
                return 0;
            }
            ref->storage = storage;
            eaPush(counter->eaCounters, ref);
        }
    }
    else
    {
        // only one instance
        char szCounter[256];
        CounterRef *ref = calloc(sizeof(CounterRef),1);
        sprintf_s(SAFESTR(szCounter),"\\%s\\%s",counter->interfaceName, counterName);
        if( PdhAddCounterA(counter->hQuery,szCounter,(DWORD_PTR)NULL,&ref->hCounter))
        {
            printf("PdhAddCounter failed : %s\n", szCounter);
            free(ref);
            return 0;
        }
        ref->storage = storage;
        eaPush(counter->eaCounters, ref);
    }

    return 1;
}

int performanceCounterQuery(PerformanceCounter *counter)
{
    PDH_FMT_COUNTERVALUE value;
    int i;
    int ret=1;

    if(PdhCollectQueryData(counter->hQuery))
    {
        printf("PdhCollectQueryData failed\n");
        return 0;
    }

    for (i=0; i<eaSize(counter->eaCounters); i++)
    {
        *(counter->eaCounters_data[i]->storage) = 0;
    }

    for (i=0; i<eaSize(counter->eaCounters); i++)
    {
        if( PdhGetFormattedCounterValue(counter->eaCounters_data[i]->hCounter, PDH_FMT_LONG, NULL, &value))
        {
            //printf("PdhGetFormattedCounterValue failed\n");
            ret = 0;
            continue;
        }
        *(counter->eaCounters_data[i]->storage) += value.longValue;
    }
    return ret;
}

int testPerformanceCounter()
{
    LONG bytesSent;
    LONG bytesRead;
    LONG cpuUsage;
    PerformanceCounter *counter;
    PerformanceCounter *counterCPU;

    if ((counter=performanceCounterCreate("Network Interface"))==NULL)
        return 0;

    performanceCounterAdd(counter, "Bytes Sent/sec", &bytesSent);
    performanceCounterAdd(counter, "Bytes Received/sec", &bytesRead);

    if ((counterCPU=performanceCounterCreate("Processor"))==NULL)
        return 0;

    performanceCounterAdd(counterCPU, "% Processor Time", &cpuUsage);

    while(!_kbhit())
    {

        performanceCounterQuery(counter);
        performanceCounterQuery(counterCPU);

        printf("Bytes Sent/sec : %ld\n",bytesSent);
        printf("Bytes Read/sec : %ld\n",bytesRead);
        printf("CPU Usage : %ld\n",cpuUsage);
        Sleep(1000);
    }

    performanceCounterDestroy(counter);

    return 1;
}

#endif