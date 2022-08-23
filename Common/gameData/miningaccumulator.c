#include <utilitieslib/stdtypes.h>
#include "miningaccumulator.h"
#include <utilitieslib/components/earray.h>
#include <utilitieslib/components/StashTable.h>
#include "comm_backend.h"
#include "container/dbcontainerpack.h"
#include <utilitieslib/components/EString.h>
#include <utilitieslib/components/StringCache.h>
#include <utilitieslib/network/netio.h>
#include <utilitieslib/utils/timing.h>


#define MINEACC_STASH_DEF        128
#define MINEACC_DATASTASH_DEF    128
#define SECONDS_PER_WEEK        (SECONDS_PER_DAY*7)


typedef struct MiningAccumulatorEntry
{
    const char *field; // string cached
    int ever;

    int thisweek;
    int lastweek;
} MiningAccumulatorEntry;

typedef struct MiningAccumulator
{
    U32        dbid;
    char    *name; // estring
    U32        week_start;

    MiningAccumulatorEntry **data;

    StashTable        data_stash;        // not persisted
    eaiHandle    merged_dbids;    // not persisted
    bool            persist_me;        // not persisted
} MiningAccumulator;


static MiningAccumulatorEntry* MiningAccumulatorEntryCreate();

LineDesc MiningAccumulatorEntry_line_desc[] =
{
    {{    PACKTYPE_STR_ASCII_CACHED,    MINEACC_FIELD_LEN,    "Field",    OFFSET(MiningAccumulatorEntry,field),    INOUT(0,0),    LINEDESCFLAG_INDEXEDCOLUMN    },
    "data being mined"},
    {{    PACKTYPE_INT,            SIZE_INT32,            "Ever",        OFFSET(MiningAccumulatorEntry,ever)                                    },
    "total ever accumulated"},
    {{    PACKTYPE_INT,            SIZE_INT32,            "ThisWeek",    OFFSET(MiningAccumulatorEntry,thisweek)                                },
    "total accumulated this week"},
    {{    PACKTYPE_INT,            SIZE_INT32,            "LastWeek",    OFFSET(MiningAccumulatorEntry,lastweek)                                },
    "total accumulated last week"},
    { 0 },
};

StructDesc MiningAccumulatorEntry_desc[] =
{
    sizeof(MiningAccumulatorEntry),
    {AT_EARRAY, OFFSET(MiningAccumulator,data)},
    MiningAccumulatorEntry_line_desc,
    "mined data"
};

LineDesc MiningAccumulator_line_desc[] =
{
    {{    PACKTYPE_ESTRING_ASCII,    MINEACC_NAME_LEN,                    "Name",            OFFSET(MiningAccumulator,name),    INOUT(0,0),    LINEDESCFLAG_INDEXEDCOLUMN    },
    "name of the item for which data is being accumulated"},
    {{    PACKTYPE_DATE,        0,                            "WeekStart",    OFFSET(MiningAccumulator,week_start)                        },
    "when 'ThisWeek' data started being accumulated"},
    {{    PACKTYPE_EARRAY,    (intptr_t)MiningAccumulatorEntryCreate,    "MinedData",    (intptr_t)MiningAccumulatorEntry_desc                            },
    "data accumulated over various time frames"},
    { 0 },
};

StructDesc MiningAccumulator_desc[] =
{
    sizeof(MiningAccumulator),
    {AT_NOT_ARRAY,{0}},
    MiningAccumulator_line_desc,
    "various data collected on a particular thing"
};


static StashTable s_MiningAccumulators;
static StashTableIterator s_MiningAccumulatorsIterator;
static U32 s_ThisWeek;
static U32 s_NextWeek;

static void MiningAccumulatorSetWeeks()
{
    if(!s_ThisWeek)
    {
        struct tm time = {0};
        U32 now = timerSecondsSince2000();
        int weekday;
        
        timerMakeTimeStructFromSecondsSince2000(now,&time);
        // tm_wday is 0-based where 0 is Sunday,
        // wrap around to Monday for week start.
        weekday = (time.tm_wday + 6) % 7;

        timerMakeTimeStructFromSecondsSince2000(now - weekday*SECONDS_PER_DAY,&time);
        time.tm_hour = time.tm_min = time.tm_sec = 0;
        s_ThisWeek = timerGetSecondsSince2000FromTimeStruct(&time);

        timerMakeTimeStructFromSecondsSince2000(now + (7 - weekday)*SECONDS_PER_DAY,&time);
        time.tm_hour = time.tm_min = time.tm_sec = 0;
        s_NextWeek = timerGetSecondsSince2000FromTimeStruct(&time);
    }
}


MP_DEFINE(MiningAccumulatorEntry);

static MiningAccumulatorEntry* MiningAccumulatorEntryCreate()
{
    MP_CREATE(MiningAccumulatorEntry, 1024);
    return MP_ALLOC(MiningAccumulatorEntry);
}

static MiningAccumulatorEntry* MiningAccumulatorEntryCreateEx(const char *field)
{
    MiningAccumulatorEntry *pmae = MiningAccumulatorEntryCreate();
    pmae->field = allocAddString(field);
    return pmae;
}

static void MiningAccumulatorEntryDestroy(MiningAccumulatorEntry* pmae)
{
    // fields are left in the cache
    MP_FREE(MiningAccumulatorEntry, pmae);
}

MP_DEFINE(MiningAccumulator);

static MiningAccumulator* MiningAccumulatorCreateNull() // not indexed!, no week_start
{
    MP_CREATE(MiningAccumulator, 1024);
    return MP_ALLOC(MiningAccumulator);
}

static MiningAccumulator* MiningAccumulatorCreateEx(const char *name)
{
    MiningAccumulator *pma = MiningAccumulatorCreateNull();
    estrPrintCharString(&pma->name,name);
    pma->data_stash = stashTableCreateWithStringKeys(MINEACC_DATASTASH_DEF,StashDefault);
    MiningAccumulatorSetWeeks();
    pma->week_start = s_ThisWeek;
    return pma;
}

static void MiningAccumulatorDestroy(MiningAccumulator* pma)
{
    estrDestroy(&pma->name);
    eaDestroyEx(&pma->data,MiningAccumulatorEntryDestroy);
    stashTableDestroy(pma->data_stash);
    eaiDestroy(&pma->merged_dbids);
    MP_FREE(MiningAccumulator, pma);
}


static MiningAccumulator* MiningAccumulatorGet(const char *name)
{
    MiningAccumulator *pma;

    if(!s_MiningAccumulators)
        s_MiningAccumulators = stashTableCreateWithStringKeys(MINEACC_STASH_DEF,StashDefault);

    if(!stashFindPointer(s_MiningAccumulators, name, &pma))
    {
        pma = MiningAccumulatorCreateEx(name);
        stashAddPointer(s_MiningAccumulators, pma->name, pma, true);
    }

    return pma;
}

static MiningAccumulatorEntry* MiningAccumulatorGetEntry(MiningAccumulator *pma, const char *field)
{
    MiningAccumulatorEntry *pmae;

    if(!stashFindPointer(pma->data_stash, field, &pmae))
    {
        pmae = MiningAccumulatorEntryCreateEx(field);
        eaPush(&pma->data,pmae);
        stashAddPointer(pma->data_stash, pmae->field, pmae, true);
    }

    return pmae;
}

static void MiningAccumulatorIndex(MiningAccumulator *pma)
{
    int i;

    if(pma->data_stash)
        stashTableClear(pma->data_stash);
    else
        pma->data_stash = stashTableCreateWithStringKeys(eaSize(&pma->data),StashDefault); // FIXME: shared heap?

    // duplicate fields should never happen, we control all the data
    for(i = 0; i < eaSize(&pma->data); ++i)
    {
        if(pma->data[i] && pma->data[i]->field && pma->data[i]->field[0])
        {
            stashAddPointer(pma->data_stash, pma->data[i]->field, (void*)pma->data[i], false);
#ifdef STATSERVER
            if(pma->data[i]->ever < 0)
            {
                pma->data[i]->ever = INT_MAX;
                pma->persist_me = true;
            }
#endif
        }
        else
        {
            // this should never happen, but has internally
            // i've put in devasserts to help locate the issue
            eaRemoveFast(&pma->data,i); // avoid causing lots of db rows to change
#ifdef STATSERVER
            pma->persist_me = true;
#endif
            --i;
        }
    }
}


int MiningAccumulatorCount()
{
    return s_MiningAccumulators ? stashGetValidElementCount(s_MiningAccumulators) : 0;
}

void MiningAccumulatorAdd(const char *name, const char *field, int val)
// note that on the statserver, all existing accumulators are preloaded, new accumulators are created when persisting
{
    if(devassert(name && name[0]) && devassert(field && field[0]))
    {
        MiningAccumulator *pma = MiningAccumulatorGet(name);
        MiningAccumulatorEntry *pmae = MiningAccumulatorGetEntry(pma,field);

        if(val < 0)
            val = 0;
        if(pmae->ever > INT_MAX - val)
            pmae->ever = INT_MAX;
        else
            pmae->ever += val;
#ifdef STATSERVER
        if(pmae->thisweek > INT_MAX - val)
            pmae->thisweek = INT_MAX;
        else
            pmae->thisweek += val;
        pma->persist_me = true;
//        return !pma->dbid;    // this was previously used to indicate that the container had to be created
#endif
    }
//    return false;
}

#ifdef STATSERVER
void MiningAccumulatorSetDBID(const char *name, U32 dbid)
{
    if(devassert(name && name[0]))
    {
        MiningAccumulator *pma = MiningAccumulatorGet(name);
        if(devassert(!pma->dbid))
        {
            pma->dbid = dbid;
            eaiPush(&pma->merged_dbids, dbid);
        }
        else; // made a second container for the same data!
    }
}
#endif // #ifdef STATSERVER

#ifdef STATSERVER
int MiningAccumulatorUnpackAndMerge(char *container_data, U32 dbid, char **name)
// returns the dbid of any container that needs to be destroyed
// returns -1 if the container was already handled
// returns 0 when all goes well (the vast majority of the time)
// returns the name of the container in *name
{
    int i;
    MiningAccumulatorEntry **data;
    MiningAccumulator maTemp = {0}, *pmaDst;

    dbContainerUnpack(MiningAccumulator_desc, container_data, &maTemp);
    if(!maTemp.name || !maTemp.name[0]) // a bad sql write can cause this, happened on cryptic shard
        return dbid; // delete the bad container

    // adjust the data to match the current week, if it doesn't already
    MiningAccumulatorSetWeeks();
    if(maTemp.week_start < s_ThisWeek - SECONDS_PER_WEEK - SECONDS_PER_DAY) // more than a week old
    {
        int i;
        for(i = eaSize(&maTemp.data)-1; i >= 0; i--)
        {
            maTemp.data[i]->lastweek = 0;
            maTemp.data[i]->thisweek = 0;
        }
    }
    else if(maTemp.week_start < s_ThisWeek - SECONDS_PER_DAY) // one week old
    {
        int i;
        for(i = eaSize(&maTemp.data)-1; i >= 0; i--)
        {
            maTemp.data[i]->lastweek = maTemp.data[i]->thisweek;
            maTemp.data[i]->thisweek = 0;
        }
    }
    // else just right

    maTemp.week_start = s_ThisWeek; // always do this, just to keep everything matched


    pmaDst = MiningAccumulatorGet(maTemp.name);

    if(*name)
        *name = pmaDst->name;

    // record what dbids we've gotten to prevent doubling up on persisted data
    if(dbid)
    {
        // this collision should basically never happen
        // and the list should theoretically never have more than one element
        if(eaiFind(&pmaDst->merged_dbids,dbid) >= 0)
            return -1; // we've already merged in this container
        eaiPush(&pmaDst->merged_dbids, dbid);
    }

    // if the incoming accumulator has a dbid and the existing one doesn't,
    // or if the incoming accumulator has a lower dbid,
    // use the incoming accumulator as the primary one, to minimize data changes in the container from ordering
    if((!pmaDst->dbid && dbid) || dbid < pmaDst->dbid)
    {
        U32 tmp = pmaDst->dbid;
        data = pmaDst->data;

        pmaDst->dbid = dbid;
        pmaDst->data = maTemp.data;
        MiningAccumulatorIndex(pmaDst);
        
        // if pmaSrc had a lower dbid, we want to delete the old container
        // if there was no old container, dbid == 0, i.e. don't delete anything
        dbid = tmp;
    }
    else
    {
        data = maTemp.data;
        // we want to delete the incoming data, it had a higher dbid
    }

    for(i = eaSize(&data)-1; i >= 0; i--)
    {
        if(data[i] && data[i]->field && data[i]->field[0])
        {
            MiningAccumulatorEntry *pmae = MiningAccumulatorGetEntry(pmaDst,data[i]->field);

            pmae->ever += data[i]->ever;
            pmae->thisweek += data[i]->thisweek;
            pmae->lastweek += data[i]->lastweek;

            MiningAccumulatorEntryDestroy(data[i]);
        }
    }

    eaDestroy(&data);
    return dbid;
}
#endif // #ifdef STATSERVER

// statserver only
bool MiningAccumulatorWeekendUpdate()
{
    MiningAccumulatorSetWeeks();

    if(timerSecondsSince2000() > s_NextWeek)
    {
        StashTableIterator si;
        StashElement se;

        for(stashGetIterator(s_MiningAccumulators,&si); stashGetNextElement(&si,&se);)
        {
            MiningAccumulator *pma = stashElementGetPointer(se);
            if(devassert(pma->week_start > s_ThisWeek - SECONDS_PER_DAY))
            {
                int i;
                for(i = eaSize(&pma->data)-1; i >= 0; i--)
                {
                    pma->data[i]->lastweek = pma->data[i]->thisweek;
                    pma->data[i]->thisweek = 0;
                }
            }
            else
            {
                // something weird happened... thisweek's data is probably actually for lastweek
                // the data would be getting bumped now, or it's bad, either way we should drop it
                int i;
                for(i = eaSize(&pma->data)-1; i >= 0; i--)
                {
                    pma->data[i]->lastweek = 0;
                    pma->data[i]->thisweek = 0;
                }
            }
            pma->week_start = s_NextWeek;
            pma->persist_me = true;
        }
        s_ThisWeek = s_NextWeek = 0;
        return true;
    }
    return false;
}

// statserver only
char* MiningAccumulatorPackageNext(const char **name, int *dbid)
{
    StashElement se;
    MiningAccumulator *pma;

    if(!s_MiningAccumulatorsIterator.pTable || !stashGetNextElement(&s_MiningAccumulatorsIterator,&se)) // end of the list
    {
        stashGetIterator(s_MiningAccumulators,&s_MiningAccumulatorsIterator);
        if(!stashGetNextElement(&s_MiningAccumulatorsIterator,&se)) // empty list
            return NULL;
    }
    
    pma = stashElementGetPointer(se);
    if(!pma || !pma->persist_me)
        return NULL;

    pma->persist_me = false;

    *name = pma->name;
    *dbid = pma->dbid ? (int)pma->dbid : -1;
    return dbContainerPackage(MiningAccumulator_desc, pma);
}

// statserver only
void miningdata_Receive(Packet *pak)
{
    int i, count = pktGetBitsPack(pak,1);

    for(i = 0; i < count; i++)
    {
        char *name = strdup(pktGetString(pak));
        int j, fields = pktGetBitsPack(pak,1);

        for(j = 0; j < fields; j++)
        {
            char *field = pktGetString(pak);
            int val = pktGetBitsPack(pak,1);

            MiningAccumulatorAdd(name,field,val);
        }
        free(name);
    }
}

// statserver clients only (mapserver)
void MiningAccumulatorSendAndClear(NetLink *link)
{
    Packet *pak;
    int count = MiningAccumulatorCount();
    StashTableIterator si;
    StashElement se;

    if(!link || !count)
        return;

    pak = pktCreateEx(link,DBCLIENT_MININGDATA_RELAY);

    if(!pak)
        return;


    pktSendBitsPack(pak,1,count);
    for(stashGetIterator(s_MiningAccumulators,&si); stashGetNextElement(&si,&se); count--)
    {
        int i;
        MiningAccumulator *pma = stashElementGetPointer(se);
        pktSendString(pak,pma->name);
        pktSendBitsPack(pak,1,eaSize(&pma->data));

        for(i = eaSize(&pma->data)-1; i >= 0; i--)
        {
            MiningAccumulatorEntry *pmae = pma->data[i];
            pktSendString(pak,pmae->field);
            pktSendBitsPack(pak,1,pmae->ever);
        }
        MiningAccumulatorDestroy(pma);
    }
    stashTableClear(s_MiningAccumulators);

    if(devassert(!count))
        pktSend(&pak,link);
    else
        pktFree(pak);
}

// statserver clients only (mapserver)
void MiningAccumulatorSendImmediate(const char *name, const char *field, int val, NetLink *link)
{
    if(devassert(name && name[0]) && devassert(field && field[0]))
    {
        Packet *pak;

        if(!link || !(pak = pktCreateEx(link,DBCLIENT_MININGDATA_RELAY)))
            return;

        pktSendBitsPack(pak,1,1); // accumulator count
        pktSendString(pak,name);
        pktSendBitsPack(pak,1,1); // field count
        pktSendString(pak,field);
        pktSendBitsPack(pak,1,val);
        pktSend(&pak,link);
    }
}

// mapserver only
char* MiningAccumulatorTemplate()
{
    return dbContainerTemplate(MiningAccumulator_desc);
}

// mapserver only
char* MiningAccumulatorSchema()
{
    return dbContainerSchema(MiningAccumulator_desc,"MiningAccumulator");
}
