#include <utilitieslib/stdtypes.h>
#include "playerCreatedStoryarcServer.h"
#include "missionServerMapTest.h"
#include <utilitieslib/network/netio.h>
#include "dbcomm/dbcomm.h"
#include "comm_backend.h"
#include <utilitieslib/components/earray.h>
#include <utilitieslib/components/EString.h>
#include "storyarc/playerCreatedStoryarc.h"
#include "storyarc/playerCreatedStoryarcValidate.h"
#include "cmdparse/cmdserver.h"
#include <utilitieslib/UtilsNew/meta.h>
#include "mission/missionserver_meta.h"
#include <utilitieslib/utils/error.h>

#define TEST_START_WAIT_TIME 10

static MissionserverMapDatabaseStats s_stats = {0};
static int s_idx = 0;

void missionServerMapTest_getStatus(int *checked, int *total, int *invalids, int *uniques)
{
    *checked = s_stats.nValidated;
    *total = s_stats.nValidated + eaiSize(&s_stats.arcIdsToRequest);
    *invalids = s_stats.nNewInvalidated;
    *uniques = s_stats.uniqueInvalidations;
}
void missionServerMapTest_RequestAllArcs(int requestInvalid, int requestUnpublished)
{
    Packet *pak_out = pktCreateEx(&db_comm_link, DBCLIENT_MISSIONSERVER_ALLARCS);
    pktSendBool(pak_out, requestUnpublished);
    pktSendBool(pak_out, requestInvalid);
    pktSend(&pak_out, &db_comm_link);
}
void missionServerMapTest_ReceiveAllArcs(Packet *pak_in)
{
    int arcId;
    if(s_stats.testStarted)
    {
        //just consume the packet, we're already started the test, we don't want more results.
        pktGetBitsAuto(pak_in);    //total arcs
        pktGetBitsAuto(pak_in);    //nInvalid
        pktGetBitsAuto(pak_in);    //nUnpublished
        while(pktGetBitsAuto(pak_in));
        return;
    }
    s_stats.nTotal = pktGetBitsAuto(pak_in);    //total arcs
    s_stats.nOldInvalidated = pktGetBitsAuto(pak_in);    //nInvalid
    s_stats.nPublished = s_stats.nTotal-pktGetBitsAuto(pak_in);    //nUnpublished
    while(arcId = pktGetBitsAuto(pak_in))
        eaiSortedPushUnique(&s_stats.arcIdsToRequest, arcId);

    s_stats.testStarted = 1;
}

MissionserverMapInvalidationRecord **recordList = 0;

static int s_stickInList(StashElement element)
{
    MissionserverMapInvalidationRecord *record = (MissionserverMapInvalidationRecord*)stashElementGetPointer(element);

    if(record)
    {
        eaPush(&recordList, (MissionserverMapInvalidationRecord*)stashElementGetPointer(element));
    }
    return 1;
}
static int show_errors(MissionserverMapInvalidationRecord *record, char ** string, int errorId)
{
    char filename[128] = {0};
    int i;
    if(record)
    {
        char *temp = estrTemp();
        int nArcs = eaiSize(&record->arcids);
        for(i = 0; i < nArcs; i++)
        {
            if(i!=0)
                estrConcatStaticCharArray(&temp, ", ");
            estrConcatf(&temp, "%d", record->arcids[i]);
        }

        estrConcatf(string, "%d: INVALIDATION (%d arcs): %s; affecting arcs {%s};\n", errorId, nArcs, record->error, temp);
        sprintf(filename, "%d- %d arcs", errorId, i);
        playerCreatedStoryArc_Save( record->sampleArc, getMissionPath(filename) );
        estrDestroy(&temp);
        StructFree(record->sampleArc);
        estrDestroy(&record->error);
        eaiDestroy(&record->arcids);
        free(record);
    }
    return 1;
}

static int compare_reports(const void* r1Data, const void* r2Data)
{
    const MissionserverMapInvalidationRecord ** r1 = (const MissionserverMapInvalidationRecord **)r1Data;
    const MissionserverMapInvalidationRecord ** r2 = (const MissionserverMapInvalidationRecord **)r2Data;
    int size1 = eaiSize(&((*r1)->arcids));
    int size2 = eaiSize(&((*r2)->arcids));
    return  size2 - size1; 
}

void missionServerMapTest_FinishValidation()
{
    int i;
    char * string = 0;
    s_stats.testStarted = 0;
    log_printf("MissionServerMapValidation", "total arcs: %d", s_stats.nTotal);
    log_printf("MissionServerMapValidation", "old invalidated: %d", s_stats.nOldInvalidated);
    log_printf("MissionServerMapValidation", "new invalidated: %d", s_stats.nNewInvalidated);
    log_printf("MissionServerMapValidation", "");
    if(s_stats.invalidations && stashGetValidElementCount(s_stats.invalidations))
        stashForEachElement(s_stats.invalidations, s_stickInList);
    eaQSort(recordList, compare_reports);
    for(i = 0; i < eaSize(&recordList); i++)
        show_errors(recordList[i], &string, i);
    estrConcatStaticCharArray(&string, "------------------Invalid, but fixable--------------------\n");

    eaSetSize(&recordList, 0);
    if(s_stats.invalidations && stashGetValidElementCount(s_stats.fixups))
        stashForEachElement(s_stats.fixups, s_stickInList);
    eaQSort(recordList, compare_reports);
    for(i = 0; i < eaSize(&recordList); i++)
        show_errors(recordList[i], &string, i);

    log_printf("MissionServerMapValidation", "%s", string);

    estrDestroy(&string);
    exit(0);
}

void missionServerMapTest_FinishFixup()
{
    s_stats.testStarted = 0;
    exit(0);
}

void missionServerMapTest_tick()
{
    static __time64_t lastAttempt;
    static int testHasBeenSet = 0;
    __time64_t now;
    U32 time;
    int startTest = 0;
    int nRequests;
    _time64(&now);
    time = _difftime64(now, lastAttempt);

    if(!testHasBeenSet)    //TODO: this is a total hack.
    {
        s_stats.type = server_state.mission_server_test-1;
        testHasBeenSet++;
    }
    if(!lastAttempt || time >= TEST_START_WAIT_TIME)
    {
        startTest = 1;
    }

    nRequests = eaiSize(&s_stats.arcIdsToRequest);
    if(nRequests)
    {
        s_idx %=nRequests;
            missionserver_map_requestArcData_mapTest(s_stats.arcIdsToRequest[s_idx]);
        s_idx++;
    }
    else if(!s_stats.testStarted && startTest)
    {
        _time64(&lastAttempt);
        if(s_stats.type == MISSIONSERVERMAP_TEST_VALIDATION)
            missionServerMapTest_RequestAllArcs(0, 0);
        else if(s_stats.type == MISSIONSERVERMAP_TEST_FIXUP)
            missionServerMapTest_RequestAllArcs(1, 1);
    }
    else if(s_stats.testStarted)
    {
        if(s_stats.type == MISSIONSERVERMAP_TEST_VALIDATION)
            missionServerMapTest_FinishValidation();
        else if(s_stats.type == MISSIONSERVERMAP_TEST_FIXUP)
            missionServerMapTest_FinishFixup();
        server_state.mission_server_test = 0;
    }
}
void missionServerMapTest_receiveArcs(int id, char *arc)
{
    PlayerCreatedStoryArc *pPlayerArc = playerCreatedStoryArc_FromString(arc, NULL, 0, 0, 0, 0);
    char *error = estrTemp();
    char *errors2 = estrTemp();
    int errorCode = 0;
    s_stats.nValidated++;
    
    if(eaiSortedFindAndRemove(&s_stats.arcIdsToRequest, id)==-1)
        return;    //duplicate arc received;

    if(!pPlayerArc)
    {
        printf("missionServerMap fixup for arc %d unsuccessful because text %s could not be converted into an arc.\n", id, arc);
        return;
    }

    errorCode = playerCreatedStoryarc_GetValid(pPlayerArc, arc, &error, &errors2, 0, 1);
    if(s_stats.type == MISSIONSERVERMAP_TEST_VALIDATION &&
        errorCode != kPCS_Validation_OK)
    {
        MissionserverMapInvalidationRecord *record = 0;
        StashTable *table = (errorCode==kPCS_Validation_FAIL)?&s_stats.invalidations:&s_stats.fixups;
        if(!s_stats.invalidations)
            s_stats.invalidations = stashTableCreateWithStringKeys(100, StashDeepCopyKeys);
        if(!s_stats.fixups)
            s_stats.fixups = stashTableCreateWithStringKeys(100, StashDeepCopyKeys);
        if(errorCode==kPCS_Validation_FAIL)
            stashFindPointer(*table, error, &record);
        else
            stashFindPointer(*table, errors2, &record);
        if(!record)
        {
            record = malloc(sizeof(MissionserverMapInvalidationRecord));
            record->arcids = 0;
            estrCreate(&record->error);
            estrPrintEString(&record->error, error);
            record->sampleArc = pPlayerArc;
            stashAddPointer(*table, error, record, 1);
            if(errorCode==kPCS_Validation_FAIL)
                s_stats.uniqueInvalidations++;
        }
        else
            StructFree(pPlayerArc);
        eaiPushUnique(&record->arcids, id);
        if(errorCode==kPCS_Validation_FAIL)
        {
            s_stats.nNewInvalidated++;
            printf( "New Invalidation Type: %s\n", error);
        }
        else
        {
            printf( "Check validation: %s\n", errors2);
            if(error && strlen(error))
            {
                printf( "Fixup had the following misplaced errors, please fix: %s\n", error);
            }
            estrClear(&error);
            estrClear(&errors2);
            if(playerCreatedStoryarc_GetValid(pPlayerArc, arc, &error, &errors2, 0, 0)==kPCS_Validation_FAIL)
            {
                FatalErrorf("failedSecondValidation Fixup failed second validation pass: (%d) %s; %s\n", id, error, errors2);
            }
        }
    }

    else if(s_stats.type == MISSIONSERVERMAP_TEST_FIXUP)
    {
        MissionSearchHeader header = {0};
        playerCreatedStoryArc_FillSearchHeader(pPlayerArc, &header, "");    //we only care about the level range
        missionserver_updateLevelRange(id, header.levelRangeLow, header.levelRangeHigh);
        StructFree(pPlayerArc);
    }
    estrDestroy(&error);
    estrDestroy(&errors2);
}
