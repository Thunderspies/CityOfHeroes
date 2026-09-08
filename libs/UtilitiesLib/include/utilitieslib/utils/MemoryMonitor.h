#ifndef MEMORYMONITOR_H
#define MEMORYMONITOR_H

#include "../stdtypes.h"

C_DECLARATIONS_BEGIN

typedef struct StashTableImp *StashTable;
typedef const struct StashTableImp *cStashTable;

//-------------------------------------------------------------------------------------
// Variable history tracking
//-------------------------------------------------------------------------------------
typedef struct ValueHistory ValueHistory;

typedef struct HistoryCollection
{
    __int64 lastSampleTime;
    float* convergenceTimes;
    float* convergenceExponents;
    ValueHistory** collection;

} HistoryCollection;

struct ValueHistory
{
    HistoryCollection* def;
    double value;        // Instantaneous value
    float* history;        // Historic values
};

HistoryCollection* hcCreate();
void hcSample(HistoryCollection* hc);
void hcAddHistory(HistoryCollection* hc, ValueHistory* vh);
void vhDetach(ValueHistory* vh);
void vhDestroyContents(ValueHistory* vh);


//-------------------------------------------------------------------------------------
// Memory Monitor
//-------------------------------------------------------------------------------------
typedef enum
{
    MOT_NONE,
    MOT_ALLOC,
    MOT_REALLOC,
    MOT_FREE,
    MOT_AGGREGATE,
    MOT_BALANCE,
    MOT_COUNT = MOT_BALANCE
} MemOperationTypes;

typedef struct MemOperationStat
{
    ValueHistory    count;            // How many times has this operation been performed?
    ValueHistory    elapsedTime;    // How much time elapsed performing these operations?
    ValueHistory    memTraffic;        // How much memory did the operation operate on?
} MemOperationStat;

typedef struct ModuleMemOperationStats
{
    const char* moduleName;
    MemOperationStat stats[MOT_COUNT];
    bool bShared;
} ModuleMemOperationStats;

typedef struct 
{
    __int64 lastSampleTime;
    HistoryCollection* history;
    StashTable moduleStats;
} MemoryMonitor;

extern MemoryMonitor MemMonitor;

void memMonitorInit(void);
void memMointorUpdate(void);
int memMonitorActive(void);

ModuleMemOperationStats* mmGetModuleOperationStats(MemoryMonitor* statsCollection, char* moduleName, int staticModuleName);
void mmoUpdateStats(ModuleMemOperationStats* stats, MemOperationTypes type, float elapsedTime, intptr_t memTraffic);

void memMonitorTrackUserMemory(const char *moduleName, int staticModuleName, MemOperationTypes type, intptr_t memTraffic);
void memMonitorTrackSharedMemory(int v);

//-------------------------------------------------------------------------------------
// Memory Monitor stat dumping
//-------------------------------------------------------------------------------------
//Superceded by memMonitorDisplayStats()
//void mcPrintStats(StashTable statsCollection);
//void moPrintStats(MemOperationStat* stat);
//void mmPrintStats();

typedef int (__cdecl *MMOSSortCompare)(const void* elem1, const void* elem2);
void memMonitorDisplayStats(void);
void memMonitorLogStats(void);
U64 memMonitorBytesAlloced(void); // Gets the total amount of memory we think is allocated


typedef bool (*memMonitorFilter)(ModuleMemOperationStats* stats);
bool filterAll(ModuleMemOperationStats* stats);
bool filterOneMeg(ModuleMemOperationStats* stats);
const char *memMonitorGetStats(MMOSSortCompare compare, int width, memMonitorFilter filter);

void mmCRTHeapUnlock(void);
void mmCRTHeapLock(void);

#define MEMLOGDUMP_PAGEWIDTH 1024

C_DECLARATIONS_END

#endif
