#ifndef MISSIONSERVERMAPTEST_H
#define MISSIONSERVERMAPTEST_H
#include "storyarc/playerCreatedStoryarc.h"

//the following are used for verifying/updating the missionserver database.
typedef struct MissionserverMapInvalidationRecord
{
    int *arcids;
    char *error;
    PlayerCreatedStoryArc *sampleArc;
} MissionserverMapInvalidationRecord;

typedef enum MissionServerMapTestType
{
    MISSIONSERVERMAP_TEST_VALIDATION = 0,
    MISSIONSERVERMAP_TEST_FIXUP,
    MISSIONSERVERMAP_TEST_COUNT,
} MissionServerMapTestType;

typedef struct MissionserverMapDatabaseStats
{
    int *arcIdsToRequest;                                //ids are removed as they are received
    U32 testStarted:1;                                //have the stats already been collected?
    int nArcsToRequest;
    int nTotal;
    int nValidated;
    int nPublished;
    int nOldInvalidated;
    int nNewInvalidated;
    int uniqueInvalidations;
    StashTable invalidations;
    StashTable fixups;
    MissionServerMapTestType type;
} MissionserverMapDatabaseStats;

void missionServerMapTest_RequestAllArcs();
void missionServerMapTest_ReceiveAllArcs(Packet *pak_in);
void missionServerMapTest_tick();
void missionServerMapTest_receiveArcs(int id, char *arc);

void missionServerMapTest_getStatus(int *checked, int *total, int *invalids, int *uniques);

#endif
