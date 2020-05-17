#include <utilitieslib/stdtypes.h>
#include "MissionServerSearch.h"
#include "MissionServer.h"

#include "storyarc/playerCreatedStoryarc.h"

#include <utilitieslib/utils/textparser.h>
#include <utilitieslib/utils/error.h>
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/components/EString.h>
#include <utilitieslib/utils/mathutil.h>
#include "persist/persist.h"
#include <utilitieslib/components/bitfield.h>
#include <utilitieslib/components/MemoryPool.h>
#include <utilitieslib/components/StashTable.h>
#include <utilitieslib/utils/log.h>

#include <utilitieslib/UtilsNew/Str.h>
#include <utilitieslib/UtilsNew/ncstd.h>

#include "AutoGen/missionserversearch_h_ast.h"
#include <utilitieslib/utils/sysutil.h>

#define MSSEARCH_LETTER_COMBOS(wordLength) ((int)((wordLength+1)*wordLength*.5))

//used for the mission search function
typedef enum MissionServerSearchCombine {
    MSSEARCH_COMBINE_AND = 0,
    MSSEARCH_COMBINE_OR,
    MSSEARCH_COMBINE_ORAND,        //or this component, and with rest.
    MSSEARCH_COMBINE_ANDOR,        //and this component, or with rest of arcs
    MSSEARCH_COMBINE_COUNT,
} MissionServerSearchCombine;

//s_performComponentCacheByType is defined below.
static void s_performComponentCacheByType(MissionSearchParams *searchParameters, 
                                       MissionServerSearchType type, MissionServerSearchArcAction add, int subsortInfo);
static void s_updateAllCompositeSearches(MissionServerArc *arc, MissionServerSearchArcAction add);
static void s_updateAllSubStrs(char *incoming, int arcid, MissionServerSearchArcAction add);
static void s_updateAllQuoted(char *incoming, int arcid, MissionServerSearchArcAction add);

static int s_searchesPerMinute = 0;
static int s_publishesPerMinute = 0;
static int s_newTokenSearchesPerMinute = 0;
static int s_ratingChangesPerMinute = 0;
static int s_searchStrStrs = 0;
static int s_searchStrStrCaches = 0;
static int s_publishStrStrs = 0;
static int s_componentSearches = 0;
static int s_uncachedSearches = 0;
static int s_resultsReturned = 0;
static int s_returningResults = 0;
static __time64_t s_lastMinuteReport = 0;

//used for search caching
static S64 s_memory_total = 0;            //total memory available on the system.
static S64 s_memory_search = 0;            //total used, not available
static S64 s_memory_search_used = 0;


#define MSSEARCH_REPORTRATE 10
void missionserversearch_PrintStats()
{
    __time64_t now;
    U32 d;
    F32 scale;
    
    _time64(&now);

    if(!s_lastMinuteReport)
    {
        _time64(&s_lastMinuteReport);
        return;
    }
    d = _difftime64(now, s_lastMinuteReport);
    if(d<=MSSEARCH_REPORTRATE)
        return;
    
    s_lastMinuteReport = now;
    scale = 60.0f/(F32)d;

    LOG_DEBUG( "----------------------------------------------------");
    LOG_DEBUG( "SearchesPerMinute %d", (int)(scale * s_searchesPerMinute));
    LOG_DEBUG( "PublishesPerMinute %d", (int)(scale * s_publishesPerMinute));
    LOG_DEBUG( "tokenSearchesPerMinute %d", (int)(scale * s_newTokenSearchesPerMinute));
    LOG_DEBUG( "ratingPerMinute %d", (int)(scale * s_ratingChangesPerMinute));
    LOG_DEBUG( "update strstrs from publish %d", (int)(scale * s_publishStrStrs));
    LOG_DEBUG( "Search strstrs performed %d", (int)(scale * s_searchStrStrs));
    LOG_DEBUG( "Search strstrs avoided by cache %d", (int)(scale *s_searchStrStrCaches));
    LOG_DEBUG( "Average component searches per search %d", (s_uncachedSearches)?((int)((F32)s_componentSearches/(F32)s_uncachedSearches)):0);
    LOG_DEBUG( "Total Result arcs returned %d", (int)(scale * s_resultsReturned));
    LOG_DEBUG( "Total Searches returning arcs %d", (int)(scale * s_returningResults));
    s_returningResults = s_resultsReturned = s_uncachedSearches = s_componentSearches = 
        s_searchStrStrCaches = s_publishStrStrs =s_searchStrStrs = 
        s_searchesPerMinute = s_publishesPerMinute = s_newTokenSearchesPerMinute = 
        s_ratingChangesPerMinute = 0;
}

// Search Results Generation and Caching ///////////////////////////////////////

static void s_searchAND(int **dst, int **src)
{
    eaiSortedIntersection(dst, dst, src);
}

static void s_searchOR(int **dst, int **src)
{
    static int *buf = NULL; // not thread safe
    if(!buf)
        eaiCreate(&buf);    //actually necessary since none of the eaiSortedUnion calls create an empty eai
    eaiSortedUnion(&buf, dst, src);
    SWAPP(buf, *dst);
}

static int s_arcsCmp(int *key, MissionServerArc **elem)
{
    return (*key) - (*elem)->id;
}

static MissionSearchParams s_arcToSearch(MissionServerArc *arc)
{
    MissionSearchParams search = {0};
    if(!arc)
        return search;
    search.arcid = arc->id;
    search.arcLength = arc->header.arcLength;//1 <<arc->header.arcLength;
    search.author = arc->header.author;
    search.missionTypes = arc->header.missionType;
    search.morality = arc->header.morality;//1 << arc->header.morality;
    if(arc->rating == MISSIONRATING_UNRATED && arc->honors<=MISSIONHONORS_FORMER_DC )
        search.rating = 1 << MISSIONRATING_0_STARS;
    else
        search.rating = 1 << (arc->honors>MISSIONHONORS_FORMER_DC ? MISSIONRATING_MAX_STARS_PROMOTED : arc->rating);
    search.honors = 1 << arc->honors;
    search.ban = 1 << arc->ban;
    search.locale_id = 1<< arc->header.locale_id;
    search.status = 1<< arc->header.arcStatus;//(arc->header.arcStatus)?1<< arc->header.arcStatus:0;
    estrCreate(&search.text);
    if(arc->header.author && strlen(arc->header.author))
        estrConcatf(&search.text, "%s ", arc->header.author);
    if(arc->header.summary && strlen(arc->header.summary))
        estrConcatf(&search.text, "%s ", arc->header.summary);
    if(arc->header.title && strlen(arc->header.title))
        estrConcatf(&search.text, "%s ", arc->header.title);
    memcpy(search.villainGroups, arc->header.villaingroups, sizeof(U32)*MISSIONSEARCH_VG_BITFIELDSIZE);
    search.keywords = arc->top_keywords;
    return search;
}

static void s_arcDiff(MissionServerArc *oldArc, MissionServerArc *newArc)
{
    int i;
    if(!oldArc || !newArc)
        return;

    if(oldArc->id == newArc->id)
        oldArc->id = newArc->id = 0;

    // rating, honors, and ban handled separately

    if(oldArc->header.arcLength == newArc->header.arcLength)
        oldArc->header.arcLength = newArc->header.arcLength = 0;

    if(stricmp(oldArc->header.author, newArc->header.author)==0)
        oldArc->header.author[0] = newArc->header.author[0] = 0;

    if(oldArc->header.missionType == newArc->header.missionType)
        oldArc->header.missionType = newArc->header.missionType = 0;

    if(oldArc->header.morality == newArc->header.morality)
        oldArc->header.morality = newArc->header.morality = 0;

    if(stricmp(oldArc->header.author_shard,newArc->header.author_shard)==0)
        oldArc->header.author_shard[0] = newArc->header.author_shard[0] = 0;

    if(oldArc->header.date_created == newArc->header.date_created)
        oldArc->header.date_created = newArc->header.date_created = 0;

    if(stricmp(oldArc->header.summary, newArc->header.summary)==0)
        oldArc->header.summary[0] = newArc->header.summary[0] = 0;

    if(stricmp(oldArc->header.title, newArc->header.title))
        oldArc->header.title[0] = newArc->header.title[0] = 0;

    for(i = 0; i < MISSIONSEARCH_VG_BITFIELDSIZE; i++)
    {
        oldArc->header.villaingroups[i] &= 
            (oldArc->header.villaingroups[i]^newArc->header.villaingroups[i]);
        newArc->header.villaingroups[i] &= 
            (newArc->header.villaingroups[i]^oldArc->header.villaingroups[i]);
    }

}


//////////////////////////////////////////////////////////////////
// Search Cache Management
//////////////////////////////////////////////////////////////////
typedef enum SearchCacheRefresh
{
    CACHEREFRESH_VERYRARE,
    CACHEREFRESH_RARE,
    CACHEREFRESH_NORMAL,
    CACHEREFRESH_OFTEN,
    CACHEREFRESH_VERYOFTEN,
    CACHEREFRESH_COUNT,
    CACHEREFRESH_ALWAYS = CACHEREFRESH_COUNT
} SearchCacheRefresh;

static SearchCacheRefresh s_lowestSubstringRefreshRecycled = CACHEREFRESH_ALWAYS;    //start at the highest-->nothing has been invalidated
int missionserver_makeSpaceInCache(S64 memory);

static int composedCacheRefreshCategories[CACHEREFRESH_COUNT-1] = {100000, 10000, 1000, 100};    //number of hits to get into each of the following categories
static MissionServerCSearch *composed_head[CACHEREFRESH_COUNT] = {0};
static MissionServerCSearch *composed_tail[CACHEREFRESH_COUNT] = {0};
static int substrCacheRefreshCategories[CACHEREFRESH_COUNT-1] = {2, 4, 6, 8};
static MissionServerSubstringSearch *substrs_head[CACHEREFRESH_COUNT] = {0};
static MissionServerSubstringSearch *substrs_tail[CACHEREFRESH_COUNT] = {0};
static int quotedCacheRefreshCategories[CACHEREFRESH_COUNT-1] = {500, 50, 10, 2};    //number of hits to get into each of the following categories
static MissionServerQuotedSearch *quoted_head[CACHEREFRESH_COUNT] = {0};
static MissionServerQuotedSearch *quoted_tail[CACHEREFRESH_COUNT] = {0};
void s_performSubstrSearch(char *s, int **tokenArcIds);
#define CACHE_COMPOSED 1 
#define CACHE_SUBSTR 2
#define CACHE_QUOTED 3




FORCEINLINE static SearchCacheRefresh s_getCacheRefreshId(int *boundaries, int descending, int data)
{
    int i;
    for(i = 0; i < CACHEREFRESH_VERYOFTEN; i++)
    {
        if((descending && data >= boundaries[i]) ||
            (!descending && data <= boundaries[i]))
        {
            return i;
        }
    }
    return CACHEREFRESH_VERYOFTEN;
}

#define SEARCH_PUSH_FRONT(element, addMe) s_pushFrontNode((MissionServerSearchNode *)element, (MissionServerSearchNode *)addMe)
FORCEINLINE static void s_pushFrontNode(MissionServerSearchNode *element, MissionServerSearchNode *addMe)
{
    addMe->forward = element->forward;
    (element->forward)?(element->forward->backward = addMe):0;
    element->forward = addMe;
    addMe->backward = element;
    s_memory_search_used+=addMe->size;
}

#define SEARCH_PUSH_BEHIND(element, addMe) s_pushBehindNode((MissionServerSearchNode *)element, (MissionServerSearchNode *)addMe)
FORCEINLINE static void s_pushBehindNode(MissionServerSearchNode *element,  MissionServerSearchNode *addMe)
{
    addMe->backward = element->backward;
    (element->backward)?(element->backward->forward = addMe):0;
    element->backward = addMe;
    addMe->forward = element;
    missionserver_makeSpaceInCache(addMe->size);
    s_memory_search_used+=addMe->size;
}

#define SEARCH_REMOVE(element, headPtrPtr, tailPtrPtr) s_removeNode((MissionServerSearchNode *)element, (MissionServerSearchNode **)headPtrPtr, (MissionServerSearchNode **)tailPtrPtr)
FORCEINLINE static void s_removeNode(MissionServerSearchNode *element, MissionServerSearchNode **headPtrPtr, MissionServerSearchNode **tailPtrPtr)
{
    int attached = element->backward || element->forward || (element==*tailPtrPtr) || (element==*headPtrPtr);
    (element->backward)?element->backward->forward = element->forward:0;
    (element->forward)?element->forward->backward = element->backward:0;
    (element==*tailPtrPtr)?(*tailPtrPtr)=element->forward:0;
    (element==*headPtrPtr)?(*headPtrPtr)=element->backward:0;
    element->forward = 0;
    element->backward = 0;
    if(attached)
    {
        s_memory_search_used-=element->size;
    }
}

#define SEARCH_PUSH_HEAD(headPtrPtr, tailPtrPtr, element) s_pushHeadNode((MissionServerSearchNode **)headPtrPtr, (MissionServerSearchNode **)tailPtrPtr, (MissionServerSearchNode *)element)
FORCEINLINE static void s_pushHeadNode(MissionServerSearchNode **headPtrPtr, MissionServerSearchNode **tailPtrPtr, MissionServerSearchNode *pushedElement)
{
    missionserver_makeSpaceInCache(pushedElement->size);
    if(*headPtrPtr)
    {
        s_pushFrontNode(*headPtrPtr, pushedElement);
    }
    else
    {
        s_memory_search_used+=pushedElement->size;
        *tailPtrPtr = pushedElement;
    }
    *headPtrPtr = pushedElement;
}

#define SEARCH_POP_TAIL(tailPtrPtr, headPtrPtr,element) s_popTailNode((MissionServerSearchNode **)tailPtrPtr, (MissionServerSearchNode **)headPtrPtr, (MissionServerSearchNode **)element) 
FORCEINLINE static void s_popTailNode(MissionServerSearchNode **tailPtrPtr, MissionServerSearchNode **headPtrPtr, MissionServerSearchNode **poppedElement) 
{
    *poppedElement = (*tailPtrPtr);
    assert(!(*tailPtrPtr)->backward);
    (*poppedElement)?s_removeNode(*poppedElement, headPtrPtr, tailPtrPtr):(void)0;
}

#define SEARCH_REFRESH(search, headPtrPtrOld, tailPtrPtrOld, \
                        headPtrPtrNew, tailPtrPtrNew, \
                        newSize)\
                            s_refreshNode((MissionServerSearchNode *)search,\
                                        (MissionServerSearchNode **)headPtrPtrOld, (MissionServerSearchNode **)tailPtrPtrOld,\
                                        (MissionServerSearchNode **)headPtrPtrNew, (MissionServerSearchNode **)tailPtrPtrNew,\
                                        newSize) 

FORCEINLINE static void s_refreshNode(MissionServerSearchNode *search, 
                                      MissionServerSearchNode **headPtrPtrOld, MissionServerSearchNode **tailPtrPtrOld,
                                      MissionServerSearchNode **headPtrPtrNew, MissionServerSearchNode **tailPtrPtrNew, 
                                      S64 newSize) 
{
    s_removeNode(search, headPtrPtrOld, tailPtrPtrOld);
    search->size = newSize;
    s_pushHeadNode(headPtrPtrNew, tailPtrPtrNew, search);
    _time64(&search->timestamp);
}


FORCEINLINE static S64 s_getSSize(MissionServerSubstringSearch *search)
{
    S64 size = sizeof(MissionServerSubstringSearch);
    size += strlen(search->substring);
    size += eaiMemUsage(&search->arcIds);
    return size;
}

FORCEINLINE static void s_refreshSubstringSearch(MissionServerSubstringSearch *search)
{
    SearchCacheRefresh arrayIdx = s_getCacheRefreshId(substrCacheRefreshCategories, 0, strlen(search->substring));
    SEARCH_REFRESH(search, &substrs_head[search->node.cacheRefreshId], &substrs_tail[search->node.cacheRefreshId], \
        &substrs_head[arrayIdx], &substrs_tail[arrayIdx], \
        s_getSSize(search));
    search->node.cacheRefreshId = arrayIdx;
}

FORCEINLINE static void s_enforceComposedLimit()
{
    int count = plCount(MissionServerCSearch);

    SearchCacheRefresh cacheLevel = CACHEREFRESH_VERYOFTEN;
    static MissionServerCSearch **composed_delete = 0;
    eaSetSize(&composed_delete, 0);
    while(count > g_missionServerState.cfg.maxComposedCaches && cacheLevel>=0)
    {
        static MissionServerCSearch *cSearchTemp =0;
        while(!composed_tail[cacheLevel] && cacheLevel>=0)
            cacheLevel--;
        if(composed_tail[cacheLevel])
        {
            int i;
            SEARCH_POP_TAIL(&composed_tail[cacheLevel], &composed_head[cacheLevel], &cSearchTemp);
            eaPush(&composed_delete,cSearchTemp);
            estrDestroy(&cSearchTemp->params.text);
            eaiDestroy(&cSearchTemp->arcids);
            for(i = 0; i < MSSEARCH_MAX_SORTBUCKETS; i++)
            {
                if(cSearchTemp->arcidsBySort[i])
                    eaiDestroy(&cSearchTemp->arcidsBySort[i]);
            }
            count--;
        }
    }
    if(eaSize(&composed_delete))
    {
        plDeleteList(MissionServerCSearch, composed_delete, eaSize(&composed_delete));
    }
    devassert(count <= g_missionServerState.cfg.maxComposedCaches);
}

FORCEINLINE static S64 s_getCSize(MissionServerCSearch *search)
{
    S64 size = 0;
    int i;
    size = search->node.size += sizeof(MissionServerCSearch);
    size = search->node.size += (search->str)?strlen(search->str):0;
    size = search->node.size += (search->params.author)?strlen(search->params.author):0;
    size = search->node.size += (search->params.text)?strlen(search->params.text):0;
    size = search->node.size += (search->params.text)?strlen(search->params.text):0;

    if(search->params.sort)
    {
        for(i = 0; i < MSSEARCH_MAX_SORTBUCKETS; i++)
        {
            if(search->arcidsBySort[i])
                size += eaiMemUsage(&search->arcidsBySort[i]);
        }
    }
    else
    {
        size += eaiMemUsage(&search->arcids);
    }
    return size;
}

FORCEINLINE static void s_refreshComposedSearch(MissionServerCSearch *search)
{
    SearchCacheRefresh arrayIdx = s_getCacheRefreshId(composedCacheRefreshCategories, 1, search->hits);
    SEARCH_REFRESH(search, &composed_head[search->node.cacheRefreshId], &composed_tail[search->node.cacheRefreshId], \
        &composed_head[arrayIdx], &composed_tail[arrayIdx], \
        s_getCSize(search));
    search->node.cacheRefreshId = arrayIdx;
}

FORCEINLINE static S64 s_getQSize(MissionServerQuotedSearch *search)
{
    S64 size = sizeof(MissionServerQuotedSearch);
    size += (search->str)?strlen(search->str):0;
    size += eaiMemUsage(&search->arcids);
    return size;
}

FORCEINLINE static void s_refreshQuotedSearch(MissionServerQuotedSearch *search)
{
    SearchCacheRefresh arrayIdx = s_getCacheRefreshId(quotedCacheRefreshCategories, 1, search->hits);
    SEARCH_REFRESH(search, &quoted_head[search->node.cacheRefreshId], &quoted_tail[search->node.cacheRefreshId], \
        &quoted_head[arrayIdx], &quoted_tail[arrayIdx], \
        s_getQSize(search));
    search->node.cacheRefreshId = arrayIdx;
}



int missionserver_makeSpaceInCache(S64 memory)
{
    static MissionServerCSearch **composed_delete = NULL;
    static MissionServerSubstringSearch **substrs_delete = NULL;
    static MissionServerQuotedSearch **quoted_delete = NULL;
    static int hasMadeSpace = 0;
    S64 memoryToClear = (memory < (s_memory_search-s_memory_search_used))?0:memory -(s_memory_search-s_memory_search_used);
    int cachesToDelete = 1;
    int needsDeletion = 0;
    int i;
    if(!memoryToClear)
        return 1;
    else if(!hasMadeSpace)
    {
        hasMadeSpace =1;
        LOG( LOG_MISSIONSERVER, LOG_LEVEL_VERBOSE, LOG_CONSOLE_ALWAYS, "Warning: low memory forced recycling of cache.  This will reduce Mission Server performance.\n");
    }

    eaSetSize(&composed_delete, 0);
    eaSetSize(&substrs_delete, 0);
    eaSetSize(&quoted_delete, 0);
    for(i = CACHEREFRESH_VERYOFTEN; i >= CACHEREFRESH_VERYRARE && memoryToClear >0; i--)
    {
        while((composed_tail[i] || substrs_tail[i] || quoted_tail[i]) && memoryToClear >0)
        {
            __time64_t oldest = 0;
            int oldestID = 0;
            S64 size = 0;
            MissionServerCSearch *cSearchTemp = 0;
            MissionServerSubstringSearch *sSearchTemp = 0;
            MissionServerQuotedSearch *qSearchTemp = 0;
            int found = 1;
            int j;

            //always recycle composed searches first.
            if(composed_tail[i])
            {
                oldest = composed_tail[i]->node.timestamp;
                oldestID = CACHE_COMPOSED;
            } else
            {
                //otherwise, recycle either quoted or substrings depending on timestamp.
                if(substrs_tail[i] && (!oldestID || substrs_tail[i]->node.timestamp <oldest))
                {
                    oldest = substrs_tail[i]->node.timestamp;
                    oldestID = CACHE_SUBSTR;
                }
                if(quoted_tail[i] && (!oldestID || quoted_tail[i]->node.timestamp <oldest))
                {
                    oldest = quoted_tail[i]->node.timestamp;
                    oldestID = CACHE_QUOTED;
                }
            }

            //mark the smallest for removal.
            switch(oldestID)
            {
                xcase CACHE_COMPOSED:
                    SEARCH_POP_TAIL(&composed_tail[i], &composed_head[i], &cSearchTemp);
                    eaPush(&composed_delete,cSearchTemp);
                    size = cSearchTemp->node.size;
                    estrDestroy(&cSearchTemp->params.text);
                    eaiDestroy(&cSearchTemp->arcids);
                    for(j = 0; j < MSSEARCH_MAX_SORTBUCKETS; j++)
                    {
                        if(cSearchTemp->arcidsBySort[j])
                            eaiDestroy(&cSearchTemp->arcidsBySort[j]);
                    }
                xcase CACHE_SUBSTR:
                    SEARCH_POP_TAIL(&substrs_tail[i], &substrs_head[i], &sSearchTemp);
                    eaPush(&substrs_delete,sSearchTemp);
                    eaiDestroy(&sSearchTemp->arcIds);
                    size = sSearchTemp->node.size;
                    if(i < s_lowestSubstringRefreshRecycled)
                    {
                        LOG( LOG_MISSIONSERVER, LOG_LEVEL_VERBOSE, LOG_CONSOLE_ALWAYS, "Warning: low memory forced recycling substring cache level %d\n", i);
                        s_lowestSubstringRefreshRecycled = i;
                    }
                xcase CACHE_QUOTED:
                    SEARCH_POP_TAIL(&quoted_tail[i], &quoted_head[i], &qSearchTemp);
                    eaPush(&quoted_delete,qSearchTemp);
                    eaiDestroy(&qSearchTemp->arcids);
                    size = qSearchTemp->node.size;
                xdefault:
                    found = 0;
            }

            //reduce the amount of memory we're looking to free.
            if(memoryToClear < size)
                memoryToClear = 0;
            else
                memoryToClear-=size;
            if(found)
                needsDeletion |=1;
        }
    }
    if(needsDeletion)
    {
        plDeleteList(MissionServerCSearch, composed_delete, eaSize(&composed_delete));
        plDeleteList(MissionServerSubstringSearch, substrs_delete, eaSize(&substrs_delete));
        plDeleteList(MissionServerQuotedSearch, quoted_delete, eaSize(&quoted_delete));
    }
    if(devassert(memoryToClear ==0))
    {
        return 1;
    }
    else return 0;
}



//warn if there's less than 1 gig for search cache
#define MIN_SEARCH_CACHE_NO_WARN (1<<30)
void missionserversearch_InitializeSearchCache()
{
    //unsigned long s_memory_total, s_memory_used;
    unsigned long long mem_available = 0, temp_total;
    S64 mem_current_used;
    getPhysicalMemory(&temp_total, &mem_available);
    s_memory_total =(S64)temp_total;
    s_memory_search = (S64)((S64)s_memory_total-g_missionServerState.cfg.memory_reserved);
    mem_current_used = s_memory_total-(S64)mem_available;
    /*if(mem_current_used > g_missionServerState.cfg.memory_reserved)
        //printf("Warning: %u memory already in use, but only %u reserved in config.  Consider increasing ReserveMemory in cfg file.\n", s_memory_total-mem_available, g_missionServerState.cfg.memory_reserved);
    else if(mem_available-g_missionServerState.cfg.memory_reserved<MIN_SEARCH_CACHE_NO_WARN)
        printf("Warning: %u memory available, %u reserved in config leaving %u for the search cache.  Try lowering ReserveMemory in cfg file.\n", mem_available, g_missionServerState.cfg.memory_reserved, s_memory_search);
        */
    //these messages have proven to cause confusion and not have a significant impact on search.  We will know if we need to make changes based on whether or not
    //we start destroying the cache.
}



//////////////////////////////////////////////////////////////////
// Dealing with finding substrings in tokens
//////////////////////////////////////////////////////////////////

static MissionServerSearch **s_tokenList = {0};
static U32 s_tokenCount = 0;
#define MSSEARCH_TOKEN_INIT_SIZE 100000
//all the characters
#define CHARACTER_COUNT (1<<(8*sizeof(char)))

//before '0' we can safely overload chars with other useful char combinations
#define CHARACTER_COMBOMAX ('0')

//this needs to be ordered by longest combo to shortest
static char s_comboChars[][MAX_TRACKED_CHARSIZE] = 
{
    //Most common 3 letter combos
    "THE", "AND", "HAT", "ENT", "ION", "FOR", "TIO", "HAS", "TIS",
    //Most common 2 letter combos
    "TH", "HE", "AN", "IN", "ER", "RE", "ES", "ON", "TI", "AT",
    //most common 2 letter reversals
    /*"ER", "RE","ES",*/ "SE", /*"AN",*/ "NA", /*"ON",*/ "NO", /*"TI", */"IT", "EN", "NE", "TO", "OT", "ED", "DE",
};
#define CHARACTER_COMBOCOUNT (ARRAY_SIZE(s_comboChars))
//this array can only fit into the first block of unused chars.
STATIC_ASSERT(CHARACTER_COMBOCOUNT <= CHARACTER_COMBOMAX);

static MissionServerTokenList s_tokenListsByCharacter[CHARACTER_COUNT] = {0};



void missionserversearch_initTokenLists()
{
    int i;
    for(i = 0; i < CHARACTER_COUNT; i++)
    {
        int j;
        if(i < CHARACTER_COMBOCOUNT)
            strcpy_s(s_tokenListsByCharacter[i].letter, MAX_TRACKED_CHARSIZE, s_comboChars[i]);
        else
            s_tokenListsByCharacter[i].letter[0] = i;
        for(j =0; j < MAX_TRACKED_LETTER_OCCURENCES; j++)
        {
            int k;
            for(k = 0; k < MAX_TRACKED_WORD_LENGTH; k++)
                eaiCreate(&s_tokenListsByCharacter[i].tokenids[j][k]);
        }
    }

}

/*#define CHARINDEX_LETTER_START (0)
#define CHARINDEX_DIGIT_START ('z'-'a' + 1)
#define CHARINDEX_SPECIAL_START (CHARINDEX_DIGIT_START+10)
#define CHARINDEX_SPECIAL_SIZE (MMSEARCH_MAXCHARS-CHARINDEX_SPECIAL_START)
static int s_getCharIndex(char c)
{
    int valid = iswalnum(c);
    int idx;
    //TODO: make a better lookup function, because this is horrid.
    if(!valid)
        return -1;
    //0-25 letters
    if(iswalpha(c))
        return CHARINDEX_LETTER_START+(towlower(c)-'a');
    //26-35 digits
    else if(isalpha(c))
        return CHARINDEX_DIGIT_START+(c-'0');
    //36-64 international or other sorting
    else
        return CHARINDEX_SPECIAL_START + (c%CHARINDEX_SPECIAL_SIZE);
}*/


static void s_cacheToken(char * token, int size, int tokenId, MissionServerSearchArcAction add)
{
    U32 count[CHARACTER_COUNT] = {0};
    int i;
    unsigned char letter;


    //get all the individual letters
    for(i = 0; i < size; i++)
    {
        int j;
        letter = (unsigned char)tolower(token[i]);
        count[letter]++;

        //if we end up having space to keep every id in its word size or larger, then we should do this.
        //we might also do this if we're having trouble with the eaiSortedIntersection time. just reduce
        //MAX_TRACKED_WORD_LENGTH to a more reasonable number
        for(j = MIN(size,MAX_TRACKED_WORD_LENGTH-1); j >= 0; j--)
        {
            if(add)
                eaiSortedPushUnique(&MISSIONSERVER_TOKENLIST_IDS(s_tokenListsByCharacter[letter], count[letter], j),
                                        tokenId);
            else
                eaiSortedFindAndRemove(&MISSIONSERVER_TOKENLIST_IDS(s_tokenListsByCharacter[letter], count[letter], j),
                                        tokenId);
        }
    }

    //TODO: add all the combinations
    /*//get all the combinations
    for(i = 0; i < CHARACTER_COMBOCOUNT; i++)
    {
        
    }*/
}

//this only helps if smaller sparse lists are faster to eaisortedintersection than big ones
FORCEINLINE static void s_insertTokenList(int **lists, int *list, int oldListSize)
{
    int i;
    int placed = 0;
    int lSize = eaiSize(&list);
    int *last = list;
    int *temp;
    //this should probably be optimized with a binary search for the right starting position
    //such small lists, though...
    for(i = 0; i < oldListSize; i++)
    {
        int iSize = eaiSize(&lists[i]);
        if(lSize < iSize || placed)
        {
            placed=1;
            temp = lists[i];
            lists[i] = last;
            last = temp;
        }
    }
    lists[oldListSize] = last;
}

static void s_findTokens(char * substr, int size, int **tokenIds)
{
    int letterPresence[CHARACTER_COUNT] = {0};
    int uniqueLetters = 0;
    int * lists[CHARACTER_COUNT] = {0};
    int i;
    int found = 0;
    int empty = !eaiSize(tokenIds);
    U32 count[CHARACTER_COUNT] = {0};


    //setup the list of lists

    //TODO: find all the combinations and remove them from the string
    /*//get all the combinations
    for(i = 0; i < CHARACTER_COMBOCOUNT; i++)
    {
        
    }*/

    //then get all the individual letters and their counts
    for(i = 0; i < size; i++)
    {
        //int j;
        unsigned char letter = (unsigned char)tolower(substr[i]);
        if(!count[letter])
        {
            letterPresence[uniqueLetters] = letter;
            uniqueLetters++;
        }
        count[letter]++;
    }

    for(i = 0; i < uniqueLetters; i++)
    {
        unsigned char letter =(unsigned char)letterPresence[i];
        //we might have to use this if we can't store the arcs ids at every size.
        /*for(j = (MIN(size, MAX_TRACKED_WORD_LENGTH)-1); j < MAX_TRACKED_WORD_LENGTH; j++)
        {*/

            s_insertTokenList(lists, 
                MISSIONSERVER_TOKENLIST_IDS(s_tokenListsByCharacter[letter], count[letter], MIN(size, MAX_TRACKED_WORD_LENGTH)),
                found);
            found++;
        //}
    }

    //now get the intersection of all lists.
    for(i = 0; i < found && (empty || eaiSize(tokenIds)); i++)
    {
        if(empty)
        {
            if(found <= i+1)
                eaiCopy(tokenIds, &lists[i]);
            else
            {
                eaiSortedIntersection(tokenIds, &lists[i], &lists[i+1]);
                i++;    //because we're already dealing with the next list in the above line.
            }
            empty = 0;
        }
        else
            eaiSortedIntersection(tokenIds, tokenIds, &lists[i]);
    }
}


static int s_getSubstringMatches(char *substr, int length, int **tokenMatches, int **arcIds)
{
    //we assume that the string coming in is already lower case w/o punctuation, quotes, etc
    int i;
    static int *tokenIds = 0;
    static int *sub_substringIds = 0;
    MissionServerSearch *token = 0;
    int tokensToSearch;
    eaiPopAll(&tokenIds);
    eaiPopAll(tokenMatches);
    eaiPopAll(&sub_substringIds);


    s_findTokens(substr, length, &tokenIds);
    tokensToSearch = eaiSize(&tokenIds);
    s_searchStrStrs += tokensToSearch;

    for(i = 0; i < tokensToSearch; i++)
    {
        token = s_tokenList[tokenIds[i]];

        if(length == 1 ||strstri(token->str, substr))
            s_searchOR(tokenMatches, &token->arcids);
    }
    if(arcIds)
    {
        if(eaiSize(arcIds))
            s_searchAND(arcIds, tokenMatches);
        else
            eaiCopy(arcIds, tokenMatches);
    }

    return (token)?eaiSize(&token->arcids):0;
}

static U32 s_voteSortBuckets[] = {1, 2, 5, 10, 20, 100, 1000};
FORCEINLINE static int s_getVoteNumBucket(int votes, int honors)
{
    static int s_bucketLimits[MISSIONSEARCH_NUMVOTES_BUCKETS-1];
    static int inited = 0;
    int i;
    if(honors && honors >MISSIONHONORS_FORMER_DC)
    {
        if(honors == MISSIONHONORS_FORMER_DC_HOF)
            honors = MISSIONHONORS_HALLOFFAME;

        return honors;
    }

    if(!inited)
    {
        s_bucketLimits[0] = s_voteSortBuckets[0];
        for(i = 1; i < (MISSIONSEARCH_NUMVOTES_BUCKETS-1); i++)
        {
            if(i<ARRAY_SIZE(s_voteSortBuckets))
                s_bucketLimits[i] = s_voteSortBuckets[i];
            else
                s_bucketLimits[i] = 2 * s_bucketLimits[i-1];
        }
        inited=1;
    }
    for(i = 0; i < (MISSIONSEARCH_NUMVOTES_BUCKETS-1); i++)
    {
        if(votes < s_bucketLimits[i])
            return i;
    }
    return (MISSIONSEARCH_NUMVOTES_BUCKETS-1);
}

FORCEINLINE int s_makeComponentKey(char **dst, MissionServerSearchType type, MissionSearchParams *searchParameters, int subsort)
{
    int temp;
    if(!dst || !searchParameters)
        return 0;


    if(type !=MSSEARCH_TEXT)
        estrPrintf(dst, "%d ", type);
    switch(type)
    {
        xcase    MSSEARCH_ALL_ARC_IDS:
            return 0;    //we don't need any specifier
        xcase    MSSEARCH_MORALITY:
            temp = searchParameters->morality;
        xcase    MSSEARCH_LOCALE:
            temp = searchParameters->locale_id;
        xcase    MSSEARCH_RATING:
            temp = searchParameters->rating;
        xcase    MSSEARCH_HONORS:
            temp = searchParameters->honors;
        xcase    MSSEARCH_BAN:
            temp = searchParameters->ban;
        xcase    MSSEARCH_MISSION_TYPE:
            temp = searchParameters->missionTypes;
        xcase    MSSEARCH_ARC_LENGTH:
            temp = searchParameters->arcLength;
        xcase    MSSEARCH_STATUS:
            temp = searchParameters->status;
        xcase    MSSEARCH_KEYWORDS:
            temp = searchParameters->keywords;
        xcase    MSSEARCH_VILLAIN_GROUPS:
            //component searches can only ever have one villain group in them.
            temp = BitFieldGetFirst(searchParameters->villainGroups, MISSIONSEARCH_VG_BITFIELDSIZE);
        xcase    MSSEARCH_TEXT:
            estrConcatCharString(dst, searchParameters->text);
            return -1;
        xcase    MSSEARCH_AUTHOR:
            estrConcatCharString(dst, searchParameters->author);
            return -1;
        xdefault:
            return -1;
    }
    estrConcatf(dst, "%d %d", temp, subsort);
    //Villain groups already have the correct id;  This is because vgroups is bigger than 1
    if(type !=MSSEARCH_VILLAIN_GROUPS && temp)
        temp = BitFieldGetFirst(&temp, 1);

    //special case because we removed 0 star votes and set them all to unrated.
    if(type == MSSEARCH_RATING)
    {
        if(temp == MISSIONRATING_UNRATED)
            temp = MISSIONRATING_0_STARS;
        return ((temp*MISSIONSEARCH_NUMVOTES_BUCKETS)+subsort);
    }
    return temp;
}

void missionserver_UpdateArcRating(MissionServerArc *arc, MissionRating rating)
{
    MissionSearchParams cache = {0};
    MissionRating rating_old = (arc->rating ==MISSIONRATING_UNRATED)?MISSIONRATING_0_STARS:arc->rating;
    int subsortInfo;

    LOG( LOG_MISSIONSERVER, LOG_LEVEL_VERBOSE, 0, "Changing rating of arc "ARC_PRINTF_FMT" from %s to %s",
                    ARC_PRINTF(arc), missionrating_toString(arc->rating), missionrating_toString(rating) );

    if(    !missionserverarc_isIndexed(arc) ||
        arc->honors ) // special case: arcs with honors all always have the rating MISSIONRATING_MAX_STARS_PROMOTED for the purpose of search
    {
        arc->rating = rating;
        return;
    }

    s_updateAllCompositeSearches(arc, MSSEARCHACT_DELETE);
    arc->rating = rating;
    cache.arcid = arc->id;

    // remove old
    cache.rating = 1 << rating_old;
    subsortInfo = s_getVoteNumBucket(arc->total_votes, arc->honors);//this is pretty useless for removal anyway.
    s_performComponentCacheByType(&cache, MSSEARCH_RATING, MSSEARCHACT_DELETE, subsortInfo);

    // add new
    cache.rating = 1 << rating;
    subsortInfo = s_getVoteNumBucket(arc->total_votes, arc->honors);
    s_performComponentCacheByType(&cache, MSSEARCH_RATING, MSSEARCHACT_ADD, subsortInfo);

    s_updateAllCompositeSearches(arc, MSSEARCHACT_ADD);
    s_ratingChangesPerMinute++;
}

void missionserver_UpdateArcHonors(MissionServerArc *arc, MissionHonors honors)
{
    // Note that this must modify searches based on rating as well
    MissionSearchParams cache = {0};
    MissionHonors honors_old = arc->honors;
    int subsortInfo;

    LOG( LOG_MISSIONSERVER, LOG_LEVEL_VERBOSE, 0, "Changing honors of arc "ARC_PRINTF_FMT" from %s to %s",
                    ARC_PRINTF(arc), missionhonors_toString(arc->honors), missionhonors_toString(honors) );

    if(!missionserverarc_isIndexed(arc))
    {
        arc->honors = honors;
        return;
    }

    s_updateAllCompositeSearches(arc, MSSEARCHACT_DELETE);
    arc->honors = honors;
    cache.arcid = arc->id;

    // remove old
    cache.honors = 1 << honors_old;
    cache.rating = 1 << (honors_old>MISSIONHONORS_FORMER_DC ? MISSIONRATING_MAX_STARS_PROMOTED : arc->rating);
    subsortInfo = s_getVoteNumBucket(arc->total_votes, arc->honors);
    s_performComponentCacheByType(&cache, MSSEARCH_HONORS, MSSEARCHACT_DELETE, 0);
    s_performComponentCacheByType(&cache, MSSEARCH_RATING, MSSEARCHACT_DELETE, subsortInfo);

    // add new
    cache.honors = 1 << honors;
    cache.rating = 1 << (honors> MISSIONHONORS_FORMER_DC? MISSIONRATING_MAX_STARS_PROMOTED : arc->rating);
    subsortInfo = s_getVoteNumBucket(arc->total_votes, honors);
    s_performComponentCacheByType(&cache, MSSEARCH_HONORS, MSSEARCHACT_ADD, 0);
    s_performComponentCacheByType(&cache, MSSEARCH_RATING, MSSEARCHACT_ADD, subsortInfo);

    s_updateAllCompositeSearches(arc, MSSEARCHACT_ADD);
    s_ratingChangesPerMinute++;
}

void missionserver_UpdateArcBan(MissionServerArc *arc, MissionBan ban)
{
    MissionSearchParams cache = {0};
    MissionBan ban_old = arc->ban;

    LOG( LOG_MISSIONSERVER, LOG_LEVEL_VERBOSE, 0, "Changing ban status of arc "ARC_PRINTF_FMT" from %s to %s",
                    ARC_PRINTF(arc), missionban_toString(arc->ban), missionban_toString(ban) );

    if(!missionserverarc_isIndexed(arc))
    {
        arc->ban = ban;
        return;
    }

    s_updateAllCompositeSearches(arc, MSSEARCHACT_DELETE);
    arc->ban = ban;
    cache.arcid = arc->id;

    // remove old
    cache.ban = 1 << ban_old;
    s_performComponentCacheByType(&cache, MSSEARCH_BAN, MSSEARCHACT_DELETE, 0);

    // update, add new
    cache.ban = 1 << ban;
    s_performComponentCacheByType(&cache, MSSEARCH_BAN, MSSEARCHACT_ADD, 0);

    s_updateAllCompositeSearches(arc, MSSEARCHACT_ADD);
}

void missionserver_UpdateArcKeywords(MissionServerArc *arc, int keywordsBitfield)
{
    MissionSearchParams cache = {0};
    int keyword_old = arc->top_keywords;
    int remove = keyword_old & ~keywordsBitfield;
    int add =  keywordsBitfield;

    LOG( LOG_MISSIONSERVER, LOG_LEVEL_VERBOSE, 0, "Changing keyword status of arc "ARC_PRINTF_FMT" from %d to %d",
        ARC_PRINTF(arc), arc->top_keywords, keywordsBitfield);

    if(!missionserverarc_isIndexed(arc))
    {
        arc->top_keywords = keywordsBitfield;
        return;
    }

    s_updateAllCompositeSearches(arc, MSSEARCHACT_DELETE);
    arc->top_keywords = keywordsBitfield;
    cache.arcid = arc->id;

    // remove old
    cache.keywords = remove;
    s_performComponentCacheByType(&cache, MSSEARCH_KEYWORDS, MSSEARCHACT_DELETE, 0);

    // update, add new
    cache.keywords = add;
    s_performComponentCacheByType(&cache, MSSEARCH_KEYWORDS, MSSEARCHACT_ADD, 0);

    s_updateAllCompositeSearches(arc, MSSEARCHACT_ADD);
}

void missionserver_IndexNewArc(MissionServerArc *arc)
{
    assert(missionserverarc_isIndexed(arc));
    missionserversearch_AddRemoveArcToCache(arc, MSSEARCHACT_ADD);
    s_publishesPerMinute++;
}

void missionserver_UnindexArc(MissionServerArc *arc)
{
    missionserversearch_AddRemoveArcToCache(arc, MSSEARCHACT_DELETE);
}

static int s_matchTokens(char **matchTo, int matchToSize, char *match)
{
    char *temp = estrTemp();
    char *position;
    int i;
    int allMatches = 1;
    estrPrintCharString(&temp, match);
    position = strtok_quoted(temp, " ", " ");
    while(position && allMatches)
    {
        int found = 0;
        for(i = 0; i < matchToSize && !found; i++)
        {
            found |= !!(strstri(matchTo[i], position));
        }
        position = strtok_quoted(NULL, " ", " ");
        allMatches &=found;
    }

    //recreate the string
    estrDestroy(&temp);
    return allMatches;
}

FORCEINLINE int missionserver_isInSearch(MissionServerArc *arc, MissionSearchParams *searchParameters, MissionServerSearchType type)
{
    char *textComponents[3] = {0};
    switch(type)
    {
        xcase    MSSEARCH_ALL_ARC_IDS:
            return 1;
        xcase    MSSEARCH_MORALITY:
            return (!searchParameters->morality) || arc->header.morality&searchParameters->morality;    //fix if we switch arc morality to non bitfield
        xcase    MSSEARCH_BAN:
            return (!searchParameters->ban) || (1<<arc->ban)&searchParameters->ban;
        xcase    MSSEARCH_HONORS:
            return !searchParameters->honors || (1<<arc->honors)&searchParameters->honors;
        xcase    MSSEARCH_LOCALE:
            return !searchParameters->locale_id || searchParameters->locale_id & (1 << arc->header.locale_id);
        xcase    MSSEARCH_RATING:
            return !searchParameters->rating || searchParameters->rating & (1 << (arc->honors ? MISSIONRATING_MAX_STARS_PROMOTED : arc->rating)) ||
                (arc->rating == MISSIONRATING_UNRATED && searchParameters->rating & (1<<MISSIONRATING_0_STARS));
        xcase    MSSEARCH_MISSION_TYPE:
            return !searchParameters->missionTypes || searchParameters->missionTypes&arc->header.missionType;
        xcase    MSSEARCH_STATUS:
            return !searchParameters->status || searchParameters->status&arc->header.arcStatus;
        xcase    MSSEARCH_KEYWORDS:
            return !searchParameters->keywords || !(~searchParameters->keywords&arc->header.arcStatus);
        xcase    MSSEARCH_ARC_LENGTH:
            return !searchParameters->arcLength || searchParameters->arcLength&arc->header.arcLength;    //fix if we switch arc arclength to non bitfield
        xcase    MSSEARCH_VILLAIN_GROUPS:
            return BitFieldGetFirst(searchParameters->villainGroups, MISSIONSEARCH_VG_BITFIELDSIZE) == -1 || 
                BitFieldAnyAnd( searchParameters->villainGroups, arc->header.villaingroups, MISSIONSEARCH_VG_BITFIELDSIZE);
        xcase    MSSEARCH_TEXT:
            if(!searchParameters->text)
                return(!searchParameters->arcid || arc->id == searchParameters->arcid);
            else 
            {
                textComponents[0] = arc->header.title;
                textComponents[1] = arc->header.summary;
                textComponents[2] = arc->header.author;
                
                return (arc->id == searchParameters->arcid ||
                    s_matchTokens(textComponents, 3, searchParameters->text));
            }
        xcase    MSSEARCH_AUTHOR:
            return (!searchParameters->author || strstri(arc->header.author, searchParameters->author))?1:0;
        xdefault:
            return 0;
    }
}

/*FORCEINLINE static int s_searchIsTooOld(MissionServerSearch *search)
{
    __time64_t now;
    _time64(&now);
    return _difftime64(now, search->timestamp)>MISSIONSERVER_SEARCHCLEAR;
}*/









#define MISSIONSERVER_MAX_SEARCH_ARGS MAX(MISSIONSEARCH_MAX_VILLAINGROUPS, MISSIONSEARCH_TOTALTEXTPARTLEN)
static MissionSearchParams s_componentSearchParametersArray[MISSIONSERVER_MAX_SEARCH_ARGS];
static int s_componentSearchParametersLength = 0;

FORCEINLINE static void s_resetComponentSearchParametersArray()
{
    //this function may be expanded if s_componentSearchParametersArray needs
    //special maintenance again.
    memset(s_componentSearchParametersArray, 0, s_componentSearchParametersLength*sizeof(MissionSearchParams));
    s_componentSearchParametersLength = 0;
}


//NOTE: This function destroys the original text.
FORCEINLINE static void s_getTextParameters(char *text,
                                MissionServerSearchType type,
                                int thisIsACache)
{
    int i;
    MissionSearchParams *searchPart;
    if(MSSEARCH_CHARACTER_RULE == MSSEARCHCHARS_ALL && text && thisIsACache)
    {
        searchPart = &s_componentSearchParametersArray[s_componentSearchParametersLength];
        if(type == MSSEARCH_TEXT)
        {
            searchPart->text = text;
        }
        else if(type == MSSEARCH_AUTHOR)
        {
            searchPart->author = text;
        }
        s_componentSearchParametersLength++;
    }
    else if(text)
    {
        char * s;
        s = strtok_quoted(text, " ", " ");
        for (i=0;i<MISSIONSERVER_MAX_SEARCH_ARGS && s;i++)
        {
            if (i < MISSIONSERVER_MAX_SEARCH_ARGS && strlen(s))
            {
                searchPart = &s_componentSearchParametersArray[s_componentSearchParametersLength];
                s_componentSearchParametersLength++;
                if(type == MSSEARCH_TEXT)
                {
                    searchPart->text = s;
                }
                else if(type == MSSEARCH_AUTHOR)
                {
                    searchPart->author = s;
                }
            }

            s = strtok_quoted(0, " ", " ");
        }
    }
}


FORCEINLINE static void s_getBitfieldParameters(U32 *groups, int size, MissionServerSearchType type)
{
    MissionSearchParams *searchPart;

    if(groups)
    {
        int id;
        for(id = BitFieldGetFirst(groups, size);
            id != -1;
            id = BitFieldGetNext(groups, size, id))
        {
            if (s_componentSearchParametersLength < MISSIONSERVER_MAX_SEARCH_ARGS)
            {
                searchPart = &s_componentSearchParametersArray[s_componentSearchParametersLength];
                s_componentSearchParametersLength++;
                switch(type)
                {
                    xcase    MSSEARCH_ALL_ARC_IDS:
                //don't copy anything over since it doesn't matter.
                    xcase    MSSEARCH_MORALITY:
                        BitFieldSet(&searchPart->morality, size, id, 1);
                    xcase    MSSEARCH_LOCALE:
                        BitFieldSet(&searchPart->locale_id, size, id, 1);
                    xcase    MSSEARCH_RATING:
                        BitFieldSet(&searchPart->rating, size, id, 1);
                    xcase    MSSEARCH_HONORS:
                        BitFieldSet(&searchPart->honors, size, id, 1);
                    xcase    MSSEARCH_BAN:
                        BitFieldSet(&searchPart->ban, size, id, 1);
                    xcase    MSSEARCH_MISSION_TYPE:
                        BitFieldSet(&searchPart->missionTypes, size, id, 1);
                    xcase    MSSEARCH_ARC_LENGTH:
                        BitFieldSet(&searchPart->arcLength, size, id, 1);
                    xcase    MSSEARCH_VILLAIN_GROUPS:
                        BitFieldSet(searchPart->villainGroups, size, id, 1);
                    xcase    MSSEARCH_STATUS:
                        BitFieldSet(&searchPart->status, size, id, 1);
                    xcase    MSSEARCH_KEYWORDS:
                        BitFieldSet(&searchPart->keywords, size, id, 1);
                    xcase    MSSEARCH_TEXT: //do nothing
                    xcase    MSSEARCH_AUTHOR: //dp nothing
                    xdefault:
                        return;
                }
            }
        }
    }
}

FORCEINLINE static void s_initComponentSearchParametersArray(MissionSearchParams *searchParameters, 
                                            MissionServerSearchType type, char *searchStrStorage,
                                            int force, int isACache)
{
    MissionSearchParams *searchPart = NULL;

    if(type !=MSSEARCH_TEXT && type !=MSSEARCH_AUTHOR)
    {
        searchPart = &s_componentSearchParametersArray[s_componentSearchParametersLength];
        s_componentSearchParametersLength=0;
    }

    switch(type)
    {
        xcase    MSSEARCH_ALL_ARC_IDS:
        s_componentSearchParametersLength++;
            //don't copy anything over since it doesn't matter.
        xcase    MSSEARCH_MORALITY:
            {
                U32 morality = searchParameters->morality;
                if(!morality)
                {
                    if(!force)
                        return;
                    morality = MORALITYTYPE_ALL;
                }
                s_getBitfieldParameters(&morality, 1, type);
            }
        xcase    MSSEARCH_LOCALE:
            {
                U32 locale = searchParameters->locale_id;
                if(!locale)
                {
                    if(!force)
                        return;
                    locale = AUTHREGION_ALL;
                }
                s_getBitfieldParameters(&locale, 1, type);
            }
        xcase    MSSEARCH_RATING:
            {
                U32 rating = searchParameters->rating;
                if(!rating)
                {
                    if(!force)
                        return;
                    rating = MISSIONRATING_ALL_SEARCHED;
                }
                s_getBitfieldParameters(&rating, 1, type);
            }
        xcase    MSSEARCH_HONORS:
            {
                U32 honors = searchParameters->honors;
                if(!honors)
                {
                    if(!force)
                        return;
                    honors = MISSIONHONORS_ALL;
                }
                s_getBitfieldParameters(&honors, 1, type);
            }
        xcase    MSSEARCH_BAN:
            {
                U32 ban = searchParameters->ban;
                if(!ban)
                {
                    if(!force)
                        return;
                    ban = MISSIONBAN_ALL;
                }
                s_getBitfieldParameters(&ban, 1, type);
            }
        xcase    MSSEARCH_MISSION_TYPE:
            {
                //TODO: can we do this?
                U32 missionTypes = searchParameters->missionTypes;
                if(!missionTypes)
                {
                    if(!force)
                        return;
                    missionTypes = ~0;
                }
                s_getBitfieldParameters(&missionTypes, 1, type);
            }
        xcase    MSSEARCH_ARC_LENGTH:
            {
                U32 arcLength = searchParameters->arcLength;
                if(!arcLength)
                {
                    if(!force)
                        return;
                    arcLength = MISSIONSEARCH_ARCLENGTH_ALL;
                }
                s_getBitfieldParameters(&arcLength, 1, type);
            }
        xcase    MSSEARCH_VILLAIN_GROUPS:
            {
                int i;
                int content = 0;

                U32 searchGroups[MISSIONSEARCH_VG_BITFIELDSIZE];
                for(i = 0; i < MISSIONSEARCH_VG_BITFIELDSIZE; i++)
                {
                    if(searchParameters->villainGroups[i])
                        content++;
                    searchGroups[i] = ~0;
                }
                if(!content)
                {
                    if(!force)
                        return;
                    s_getBitfieldParameters(searchGroups, MISSIONSEARCH_VG_BITFIELDSIZE,type);
                }
                s_getBitfieldParameters(searchParameters->villainGroups, MISSIONSEARCH_VG_BITFIELDSIZE, type);
            }
        xcase    MSSEARCH_STATUS:
            {
                U32 arcStatus = searchParameters->status;
                if(!arcStatus)
                {
                    if(!force)
                        return;
                    arcStatus = MISSIONSEARCH_ARCSTATUS_ALL;
                }
                s_getBitfieldParameters(&arcStatus, 1, type);
            }
        xcase    MSSEARCH_KEYWORDS:
            {
                U32 keywords = searchParameters->keywords;
                if(!keywords)
                {
                    if(!force)
                        return;
                    keywords = ~0;
                }
                s_getBitfieldParameters(&keywords, 1, type);
            }
        xcase    MSSEARCH_TEXT:
            if((!searchParameters->text || !strlen(searchParameters->text)))
                s_componentSearchParametersLength = 0; 
            else
            {
                strcpy_s(searchStrStorage, MISSIONSEARCH_TOTALTEXTPARTLEN, searchParameters->text);
                s_getTextParameters(searchStrStorage, type, isACache);
            }
        xcase    MSSEARCH_AUTHOR:
            if((!searchParameters->author || !strlen(searchParameters->author)))
                s_componentSearchParametersLength = 0; 
            else
            {
                strcpy_s(searchStrStorage, MISSIONSEARCH_TOTALTEXTPARTLEN, searchParameters->author);
                s_getTextParameters(searchStrStorage, type, isACache);
            }
        xdefault:
            s_componentSearchParametersLength = 0; 
    }
}

FORCEINLINE static void s_getLetterCombo(char **estring, char *string, int stringSize, int sizeIdx, int iteration)
{
    int newWordSize = stringSize-sizeIdx;
    estrPrintCharString(estring, &string[iteration]);
    (*estring)[newWordSize] = 0;
}

static void s_updateAllQuoted(char *incoming, int arcid, MissionServerSearchArcAction add)
{
    MissionServerQuotedSearch **quoteds = plGetAll(MissionServerQuotedSearch);
    int i;
    int size = eaSize(&quoteds);
    s_publishStrStrs+=size;
    for(i = 0; i < size; i++)
    {
        MissionServerQuotedSearch *search = quoteds[i];
        if(strstri(incoming, search->str))
        {
            if(add)
                eaiSortedPushUnique(&search->arcids, arcid);
            else
                eaiSortedFindAndRemove(&search->arcids, arcid);
            plDirty(MissionServerQuotedSearch, search);
        }
    }
}

static void s_getBiggestTokenSubstrIds(char *token, int **arcids)
{
    static char *searchKey = 0;
    int i;
    int found = 0;
    int letters = strlen(token);
    if(!searchKey)
        estrCreate(&searchKey);
    else
        estrClear(&searchKey);
    estrSetLength(&searchKey, letters+1);
    //only substring up to MAX_TRACKED_WORD_LENGTH characters.  After that, just rely on dynamic search
    for(i = MAX(letters-MAX_TRACKED_WORD_LENGTH, 0); i < letters &&!found; i++)
    {
        int j;
        for(j = 0; j <= i; j++)
        {
            MissionServerSubstringSearch *search;
            s_getLetterCombo(&searchKey, token, letters, i, j);
            search = plFind(MissionServerSubstringSearch, searchKey);
            if(search)
            {
                eaiCopy(arcids, &search->arcIds);
                found = 1;
            }
        }
    }
}

static void s_updateTokenSubStrs(char *incoming, int arcid, MissionServerSearchArcAction add)
{
    static char *searchKey = 0;
    int i;
    int letters = strlen(incoming);
    if(!searchKey)
        estrCreate(&searchKey);
    else
        estrClear(&searchKey);
    estrSetLength(&searchKey, letters+1);
    //only substring up to MAX_TRACKED_WORD_LENGTH characters.  After that, just rely on dynamic search
    for(i = MAX(letters-MAX_TRACKED_WORD_LENGTH, 0); i < letters; i++)
    {
        int j;
        for(j = 0; j <= i; j++)
        {
            MissionServerSubstringSearch *search;
            s_getLetterCombo(&searchKey, incoming, letters, i, j);
            search = plFind(MissionServerSubstringSearch, searchKey);
            if(!search)
            {
                /*if(s_lowestSubstringRefreshRecycled < CACHEREFRESH_ALWAYS && 
                    substrCacheRefreshCategories[s_lowestSubstringRefreshRecycled] < letters)*/
                s_performSubstrSearch(searchKey, 0);
                search = plFind(MissionServerSubstringSearch, searchKey);
            }
            if(search)
            {
                if(add)
                    eaiSortedPushUnique(&search->arcIds, arcid);
                else
                    eaiSortedFindAndRemove(&search->arcIds, arcid);
                plDirty(MissionServerSubstringSearch, search);
            }
        }
    }
}

static void s_updateAllSubStrs(char *incoming, int arcid, MissionServerSearchArcAction add)
{
    static char **tokenList = 0;
    static char *temp = 0;
    int i, listSize;
    
    estrPrintCharString(&temp, incoming);
    eaSetSize(&tokenList, 0);
    missionsearch_getUniqueTokens(&temp, &tokenList, 0, 0);
    listSize = eaSize(&tokenList);
    for(i = 0; i < listSize; i++)
    {
        s_updateTokenSubStrs(tokenList[i], arcid, add);
    }
}


FORCEINLINE static int s_calculateRatingSortIdx(int voteNum, int rating, int honor)
{
    return MISSIONSEARCH_NUMVOTES_BUCKETS * (rating == MISSIONRATING_UNRATED ? 0 : rating) + s_getVoteNumBucket(voteNum, honor);
}

FORCEINLINE static int s_getSortIdx(MissionServerArc *arc, MissionServerSearchType type)
{
    switch(type)
    {
        xcase    MSSEARCH_MORALITY:
            return BitFieldGetFirst(&arc->header.morality,1);    //this is because we treat morality and arc length differently, for no reason.
        xcase    MSSEARCH_LOCALE:
            return arc->header.locale_id;
        xcase    MSSEARCH_RATING:
            return s_calculateRatingSortIdx(arc->total_votes, arc->rating, arc->honors);
        xcase    MSSEARCH_HONORS:
            return arc->honors;
        xcase    MSSEARCH_BAN:
            return arc->ban;
        xcase    MSSEARCH_ARC_LENGTH:
            return BitFieldGetFirst(&arc->header.arcLength,1);
        xcase    MSSEARCH_STATUS:
            return arc->header.arcStatus;
        xcase    MSSEARCH_KEYWORDS:
            return arc->top_keywords;
        case MSSEARCH_ALL_ARC_IDS:
        case MSSEARCH_MISSION_TYPE:
        case    MSSEARCH_VILLAIN_GROUPS:
        case    MSSEARCH_TEXT:
        case    MSSEARCH_AUTHOR:
        default:
            return -1;    //doesn't work with sort;
    }
}

static void s_cSearchAddRemoveSorted(MissionServerCSearch *search, MissionServerArc *arc,MissionServerSearchArcAction add)
{
    int found = 0;
    if(search->params.sort == MSSEARCH_RATING && add == MSSEARCHACT_DELETE)
    {
        int i, buckets = 1, start = s_calculateRatingSortIdx(0, arc->rating, 0);    //start at bucket zero for rating, go through all buckets.
        buckets =MISSIONSEARCH_NUMVOTES_BUCKETS;
        for(i = 0; i < buckets &&!found; i++)
            found |=(-1!=eaiSortedFindAndRemove(&search->arcidsBySort[start+i], arc->id));
    }
    else
    {
        int sortIdx = s_getSortIdx(arc, search->params.sort);
        if(!devassert(sortIdx >= 0 && sortIdx<MSSEARCH_MAX_SORTBUCKETS))
        {
            LOG( LOG_MISSIONSERVER, LOG_LEVEL_VERBOSE, 0, "Error: attempted to remove from sort bucket %d for search %s on update from arc %d", sortIdx, search->str, arc->id);
            return;
        }
        if(add)
            eaiSortedPushUnique(&search->arcidsBySort[sortIdx], arc->id);
        else
            found |=(-1!=eaiSortedFindAndRemove(&search->arcidsBySort[sortIdx], arc->id));
    }
    
    //for removal, we should check all existing buckets until we successfully remove the arc.
}

FORCEINLINE static int s_isArcInCompositeSearch(MissionServerArc *arc, MissionServerCSearch *search)
{
    int i;
    int isIn = 1;
    for(i = 1; i < MSSEARCH_COUNT && isIn; i++)
    {
        if(i==MSSEARCH_VILLAIN_GROUPS)
            continue;
        else if(i==MSSEARCH_TEXT)
            isIn &= ((BitFieldGetFirst(search->params.villainGroups, MISSIONSEARCH_VG_BITFIELDSIZE) != -1 &&
            missionserver_isInSearch(arc, &search->params, MSSEARCH_VILLAIN_GROUPS)) 
            || missionserver_isInSearch(arc, &search->params, MSSEARCH_TEXT));
        else
            isIn &= missionserver_isInSearch(arc, &search->params, i);
    }
    return isIn;
}

FORCEINLINE static int s_updateCompositeSearch(MissionServerArc *arc, MissionServerCSearch *search, MissionServerSearchArcAction add)
{
    int isIn = s_isArcInCompositeSearch(arc, search);
    if(isIn)
    {
        if(search->params.sort)
            s_cSearchAddRemoveSorted(search, arc,add);
        else if(add)
            eaiSortedPushUnique(&search->arcids, arc->id);
        else
            eaiSortedFindAndRemove(&search->arcids, arc->id);
    }
    return isIn;
}

static void s_updateAllCompositeSearches(MissionServerArc *arc, MissionServerSearchArcAction add)
{
    MissionServerCSearch **csearches = plGetAll(MissionServerCSearch);
    int i, nSearches, actualRating = arc->rating;
    if(arc->honors)
        arc->rating = MISSIONRATING_MAX_STARS_PROMOTED;
    nSearches = eaSize(&csearches);
    for(i = 0; i < nSearches; i++)
    {
        if(s_updateCompositeSearch(arc, csearches[i], add))
            plDirty(MissionServerCSearch, csearches[i]);
    }
    arc->rating = actualRating;
}

static void s_performComponentCacheByType(MissionSearchParams *searchParameters, MissionServerSearchType type, MissionServerSearchArcAction add, int subsort)
{
    char * key = estrTemp();
    int i, j;
    MissionServerSearch *searchPart;
    char tempSearchString[MISSIONSEARCH_TOTALTEXTPARTLEN];
    int buckets = 1;
    if(add==MSSEARCHACT_DELETE && type ==MSSEARCH_RATING)
    {
        buckets = MISSIONSEARCH_NUMVOTES_BUCKETS;
        subsort = 0;
    }

    s_initComponentSearchParametersArray(searchParameters, type, tempSearchString, 0, 1);
    if(!s_componentSearchParametersLength)    //this means the field was empty.
        return;

    for(i=0; i<s_componentSearchParametersLength; i++)
    {
        int found = 0;
        MissionSearchParams *paramPart = &s_componentSearchParametersArray[i];
        for(j = 0; j < buckets &&!found;j++)
        {
            estrClear(&key);
            s_makeComponentKey(&key, type, paramPart,subsort+j);
            searchPart = plFind(MissionServerSearch, key);

            if(add)
            {
                if(!searchPart)
                {
                    searchPart = plAdd(MissionServerSearch, key);
                    //if this is a word/token, register it for substring searches.
                    if(type == MSSEARCH_TEXT)
                    {
                        if(!s_tokenList)
                            eaCreate(&s_tokenList);
                        eaInsert(&s_tokenList, searchPart, s_tokenCount);

                        s_cacheToken(paramPart->text, strlen(paramPart->text), s_tokenCount, MSSEARCHACT_ADD);
                        s_tokenCount++;
                    }
                }
                eaiSortedPushUnique(&searchPart->arcids, searchParameters->arcid);
            }
            else
            {
                //assert(searchParameters->arcid !=1011);
                if(searchPart) 
                    found |=(-1 !=eaiSortedFindAndRemove(&searchPart->arcids, searchParameters->arcid));
            }
        }
        //devassert(add!=MSSEARCHACT_DELETE||found);
    }
    s_resetComponentSearchParametersArray();
    estrDestroy(&key);
}

FORCEINLINE static int s_performComponentSort(MissionServerCSearch *cSearch, 
                                                      int **parcids)
{
    char * key = estrTemp();
    int i, j;
    static int *temp = 0;
    static int*catTemp = 0;
    MissionServerSearch *searchPart;
    int empty = !eaiSize(parcids);
    char tempSearchString[MISSIONSEARCH_MAX_SEARCH_STRING+1];
    int subsorts = (cSearch->params.sort == MSSEARCH_RATING)?MISSIONSEARCH_NUMVOTES_BUCKETS:1;


    s_initComponentSearchParametersArray(&cSearch->params, cSearch->params.sort, tempSearchString, 1, 0);
    if(!s_componentSearchParametersLength)    //this means the field was empty.
        return 0;

    eaiPopAll(&temp);
    eaiPopAll(&catTemp);

    s_componentSearches += s_componentSearchParametersLength;

    for(i=0; i<s_componentSearchParametersLength; i++)
    {
        for(j = 0; j < subsorts; j++)
        {
            MissionSearchParams *paramPart = &s_componentSearchParametersArray[i];
            int sortIdx = s_makeComponentKey(&key, cSearch->params.sort, paramPart, j);
            assert(sortIdx <MSSEARCH_MAX_SORTBUCKETS);
            searchPart = plFind(MissionServerSearch, key);
            if(!searchPart)
            {
                //searching for an element that doesn't exist.  Don't return any ids.
                continue;
            }
            else if(empty)
            {
                s_searchOR(&cSearch->arcidsBySort[sortIdx], &searchPart->arcids);
            }
            else
                eaiSortedIntersection(&cSearch->arcidsBySort[sortIdx], &searchPart->arcids, parcids);
            if(j > 0)
                sortIdx +=0;

            searchPart->hits++;
        }
    }

    s_resetComponentSearchParametersArray();
    estrDestroy(&key);
    return 1;
}

static int s_performComponentSearchByType(MissionSearchParams *searchParameters, 
                                                       MissionServerSearchType type, 
                                                       int **parcids,
                                                       MissionServerSearchCombine combine)
{
    char * key = estrTemp();
    int i, j;
    static int *temp = 0;
    MissionServerSearch *searchPart;
    int noMoreResults = 0;
    int empty = !eaiSize(parcids);
    char tempSearchString[MISSIONSEARCH_MAX_SEARCH_STRING];
    int subsorts = (type==MSSEARCH_RATING)?MISSIONSEARCH_NUMVOTES_BUCKETS:1;


    s_initComponentSearchParametersArray(searchParameters, type, tempSearchString, 0, 0);
    if(!s_componentSearchParametersLength)    //this means the field was empty.
        return 0;

    eaiPopAll(&temp);

    s_componentSearches += s_componentSearchParametersLength;

    for(i=0; i<s_componentSearchParametersLength && !noMoreResults; i++)
    {
        for(j = 0; j < subsorts; j++)
        {
            MissionSearchParams *paramPart = &s_componentSearchParametersArray[i];
            s_makeComponentKey(&key, type, paramPart, j);
            searchPart = plFind(MissionServerSearch, key);
            if(!searchPart)
            {
                //searching for an element that doesn't exist.  Don't return any ids.
                if(combine == MSSEARCH_COMBINE_AND)
                    eaiPopAll(parcids);
                estrDestroy(&key);
                continue;

            }
            if(combine == MSSEARCH_COMBINE_OR)
            {
                s_searchOR(parcids, &searchPart->arcids);
            }
            else if(combine == MSSEARCH_COMBINE_AND)
            {
                if(!eaiSize(&searchPart->arcids))
                {
                    eaiPopAll(parcids);
                    noMoreResults = 1;
                }
                else
                    s_searchAND(parcids, &searchPart->arcids);
            }
            else if(combine == MSSEARCH_COMBINE_ORAND)
            {
                if(!temp)
                    eaiCreate(&temp);
                s_searchOR(&temp, &searchPart->arcids);
            }
            else if(combine == MSSEARCH_COMBINE_ANDOR)
            {
                if(!eaiSize(&searchPart->arcids))
                {
                    noMoreResults = 1;
                }
                else if(!eaiSize(&temp))
                {
                    eaiCopy(&temp, &searchPart->arcids);
                }
                else
                    s_searchAND(&temp, &searchPart->arcids);
            }
            searchPart->hits++;
        }
    }
    estrDestroy(&key);

    if(combine == MSSEARCH_COMBINE_ORAND)
    {
        s_searchAND(parcids, &temp);
    }
    if(combine == MSSEARCH_COMBINE_ANDOR)
    {
        s_searchOR(parcids, &temp);
    }

    if(temp)
        eaiPopAll(&temp);
    
    s_resetComponentSearchParametersArray();
    estrDestroy(&key);
    return 1;
}

void missionserversearch_AddRemoveArcToCache(MissionServerArc *arc, MissionServerSearchArcAction action)
{
    MissionSearchParams search;
    int i;
    int subsort = (arc->honors ? arc->honors : s_getVoteNumBucket(arc->total_votes, arc->honors));

    search=s_arcToSearch(arc);
    
    //assert(!strchr(arc->header.summary, '\''));

    missionsearch_stripNonSearchCharacters(&search.text, strlen(search.text), MSSEARCH_CHARACTER_RULE, 0);

    s_updateAllCompositeSearches(arc, action);
    s_updateAllQuoted(search.text,arc->id, action);
    if(MSSEARCH_CHARACTER_RULE == MSSEARCHCHARS_ALL)
    {
        char *temp = estrTemp();
        //if we're caching spaces, then each title, author and summary needs to be its own word.
        estrPrintCharString(&temp, arc->header.author);
        missionsearch_stripNonSearchCharacters(&temp, strlen(temp), MSSEARCH_CHARACTER_RULE, 0);
        s_updateTokenSubStrs(temp, arc->id, action);

        estrPrintCharString(&temp, arc->header.title);
        missionsearch_stripNonSearchCharacters(&temp, strlen(temp), MSSEARCH_CHARACTER_RULE, 0);
        s_updateTokenSubStrs(temp, arc->id, action);

        estrPrintCharString(&temp, arc->header.summary);
        missionsearch_stripNonSearchCharacters(&temp, strlen(temp), MSSEARCH_CHARACTER_RULE, 0);
        s_updateTokenSubStrs(temp, arc->id, action);
        estrDestroy(&temp);
    }
    else
        s_updateAllSubStrs(search.text,arc->id, action);
    for(i = 0; i < MSSEARCH_COUNT; i++)
    {
        s_performComponentCacheByType(&search, i, action, (i!=MSSEARCH_RATING)?0:subsort);
    }

    estrDestroy(&search.text);
}

void missionserversearch_UpdateArcInCache(MissionServerArc *oldArc, MissionServerArc *newArc)
{
    MissionServerArc oldArcDiff, newArcDiff;
    memcpy(&oldArcDiff, oldArc, sizeof(MissionServerArc));
    memcpy(&newArcDiff, newArc, sizeof(MissionServerArc));


    if(oldArc->id == newArc->id)    //selective replacement only works if id is the same.
        s_arcDiff(&oldArcDiff, &newArcDiff);

    missionserversearch_AddRemoveArcToCache(&oldArcDiff, MSSEARCHACT_DELETE);
    missionserversearch_AddRemoveArcToCache(&newArcDiff, MSSEARCHACT_ADD);
}

static void s_removeArcsWithoutText(int **arcids, char *text)
{
    int i;
    s_searchStrStrs += 3*eaiSize(arcids);
    for(i = 0; i < eaiSize(arcids); i++)
    {
        int id = (*arcids)[i];
        MissionServerArc *arc = plFind(MissionServerArc, id);
        if(!strstri(arc->header.author, text) 
            && !strstri(arc->header.title, text)
            && !strstri(arc->header.summary, text))
        {
            eaiRemove(arcids, i);
            i--;
        }
    }
}

static void s_performSubstrSearch(char *s, int **tokenArcIds)
{
    MissionServerSubstringSearch * substrSearch;
    int strlength= strlen(s);
    
    substrSearch = plFind(MissionServerSubstringSearch, s);
    
    if(tokenArcIds)
        eaiPopAll(tokenArcIds);
    //special case where we compute the longest words every time.  This is because we don't want to have to cache every element of a really 
    //really long word.
    if(strlength > MAX_TRACKED_WORD_LENGTH)
    {
        static int *tempIds = 0;
        if(tempIds)
            eaiPopAll(&tempIds);
        s_newTokenSearchesPerMinute++;
        s_getSubstringMatches(s, strlength, tokenArcIds, &tempIds);
        return;
    }
    if(!substrSearch)
    {
        //if this is a cache or we don't guarantee the existence of the string in the cache, make it
        if(s_lowestSubstringRefreshRecycled == CACHEREFRESH_VERYRARE ||
            (s_lowestSubstringRefreshRecycled <CACHEREFRESH_ALWAYS && 
                strlength > substrCacheRefreshCategories[s_lowestSubstringRefreshRecycled-1])
            )
        {
            s_newTokenSearchesPerMinute++;
            substrSearch = plAdd(MissionServerSubstringSearch, s);
            substrSearch->arcIds = 0;
            eaiCreate(&substrSearch->arcIds);
            s_getSubstringMatches(s, strlength, &substrSearch->arcIds, tokenArcIds);
            plDirty(MissionServerSubstringSearch,substrSearch);
        }
        else if(!tokenArcIds)
        {
            substrSearch = plAdd(MissionServerSubstringSearch, s);
            substrSearch->arcIds = 0;
            eaiCreate(&substrSearch->arcIds);
        }
        else
        {
            //we guarantee that substrings of this size are in the cache.  Since it's not there, searching is useless.
            return;
        }
    }
    else
    {
            eaiCopy(tokenArcIds, &substrSearch->arcIds);
    }
        //s_searchOR(tokenArcIds, &substrSearch->arcIds);

    s_refreshSubstringSearch(substrSearch);
    //SEARCH_REFRESH(substrSearch, &substrs_head, &substrs_tail, s_getSSize(substrSearch));
    //_time64(&substrSearch->timestamp);
    substrSearch->hits++;

    s_searchStrStrCaches ++;
}

static void s_performQuotedSearch(char *s, int **tokenArcIds, MissionSearchParams *tempParams)
{
    MissionServerQuotedSearch * search;
    search = plFind(MissionServerQuotedSearch, s);

    if(!search)
    {
        static char **tempStrings=0;
        static int *results = 0;
        int nResults;
        int nStrings;
        int i;
        char *estr = estrTemp();
        eaiPopAll(&results);
        estrPrintCharString(&estr, s);
        strarray_quoted(&estr, " ", " ", &tempStrings, 0);
        nStrings = eaSize(&tempStrings);
        for(i=1; i<nStrings; i++)
        {
            int j;
            int duplicate = 0;
            for(j=0; !duplicate && j<i; j++)
            {
                if(!strcmp(tempStrings[i], tempStrings[j]))
                    duplicate=1;
            }

            if(duplicate)
                continue;
            else
            {
                tempParams->text = tempStrings[i];
                s_performComponentSearchByType(tempParams, MSSEARCH_TEXT, &results, MSSEARCH_COMBINE_ANDOR);
            }
        }

        nResults = eaiSize(&results);
        //We may want to limit the total number of arcs we're willing to search.
        /*if(nResults < 100)
        {*/
            search = plAdd(MissionServerQuotedSearch, s);
            s_removeArcsWithoutText(&results, s);
            eaiCopy(&search->arcids, &results);
            plDirty(MissionServerQuotedSearch,search);
        //}
            estrDestroy(&estr);
    }

    if(search)
    {
        s_refreshQuotedSearch(search);
        search->hits++;
        eaiCopy(tokenArcIds, &search->arcids);
    }
}

static int s_searchByTypeSwitch(MissionServerSearchType type, MissionSearchParams *searchParameters, int ** arcids)
{
    int empty = !eaiSize(arcids);
    int found = 0;
    static int *results = 0;
    MissionServerSearchCombine combine = MSSEARCH_COMBINE_OR;

    if(type ==MSSEARCH_VILLAIN_GROUPS)
        return 0;    //villain groups are handled with text.
    
    //fixme, only mark empty if it's never had anything in it.
    if(empty)
    {
        combine = MSSEARCH_COMBINE_OR;
    }
    else
    {
        switch(type)
        {
            xcase MSSEARCH_ALL_ARC_IDS:
                combine = MSSEARCH_COMBINE_OR;
            xcase MSSEARCH_MORALITY:
            case MSSEARCH_LOCALE:
            case MSSEARCH_RATING:
            case MSSEARCH_HONORS:
            case MSSEARCH_BAN:
            case MSSEARCH_MISSION_TYPE:
            case MSSEARCH_STATUS:
            case MSSEARCH_ARC_LENGTH:
                combine = MSSEARCH_COMBINE_ORAND;
            xcase MSSEARCH_VILLAIN_GROUPS:
                return 0;    //we deal with this specially.
            xcase MSSEARCH_TEXT:
                combine = MSSEARCH_COMBINE_ANDOR;    //ignoring this anyway
            xcase MSSEARCH_AUTHOR:
            case MSSEARCH_KEYWORDS:
                combine = MSSEARCH_COMBINE_AND;
            xdefault:
                assert(0);
        }
    }

    if(type != MSSEARCH_TEXT)
        found = !!s_performComponentSearchByType(searchParameters, type, arcids, combine);
    else //(type == MSSEARCH_TEXT)
    {
        static char **tokens = 0;
        int quoted;
        int first = 1;
        static int *tokenArcIds = 0;
        char * text = estrTemp();
        MissionSearchParams tempParams = {0};
        eaiPopAll(&tokenArcIds);
        if(searchParameters->text)
        {
            estrConcatCharString(&text, searchParameters->text);
            {
                int numberTokens, i;
                eaSetSize(&tokens, 0);
                strarray_quoted(&text, " ", " ", &tokens, 0);
                numberTokens = eaSize(&tokens);
                for(i = 0; i < numberTokens; i++)
                {
                    char *s = tokens[i];
                    found++;
                    quoted = (MSSEARCH_CHARACTER_RULE !=MSSEARCHCHARS_ALL)?!!strchr(s, ' '):0;    //only things that were in quotes should have spaces.
                    if(quoted)
                        s_performQuotedSearch(s, &tokenArcIds, &tempParams);
                    else
                        s_performSubstrSearch(s, &tokenArcIds);

                    if(first)
                        eaiCopy(&results, &tokenArcIds);
                    else
                        s_searchAND(&results, &tokenArcIds);

                    first=0;
                }
            }
        }
        s_performComponentSearchByType(searchParameters, MSSEARCH_VILLAIN_GROUPS, &results, MSSEARCH_COMBINE_OR);
        if(searchParameters->arcid > 0)
            eaiSortedPushUnique(&results, searchParameters->arcid);    //removed later if not part of arcids.
        if(eaiSize(&results))
        {
            if(eaiSize(arcids))
                s_searchAND(arcids, &results);
            else
                s_searchOR(arcids, &results);
            found = 1;
        }
        else if(found)
            eaiPopAll(arcids);
        eaiPopAll(&tokenArcIds);
        eaiPopAll(&results);
        estrDestroy(&text);
    }
    return found;
}

static MissionServerCSearch *s_getSearch(const char * key)
{
    MissionServerCSearch *search;
    int i;
    int firstTime = 1;
    int nValidSearchParameters = 0;
    MissionSearchParams searchParameters = {0};
    static int *arcIds = 0;    //allocate this once, then enlarge as needed.  Don't allocate every time
    static MissionServerCSearch uncachedSearch = {0};
    if(!ParserReadText(key, -1, parse_MissionSearchParams, &searchParameters))
        return NULL;
    
    if(MISSIONSEARCH_CACHE_COMPOSED)
    {
        search = plFind(MissionServerCSearch, key);
    }
    else
    {
        memset(&uncachedSearch, 0, sizeof(MissionServerCSearch));
        search = &uncachedSearch;
    }

    if(!search || !MISSIONSEARCH_CACHE_COMPOSED)
    {
        s_uncachedSearches++;
        if(!search || !MISSIONSEARCH_CACHE_COMPOSED)
        {
            if(!search)
                search = plAdd(MissionServerCSearch, key);
            search->params = searchParameters;    //copy struct
            //recreate text
            search->params.text = 0;
            if(searchParameters.text)
            {
                estrPrintCharString(&search->params.text, searchParameters.text);
            }
        }
        if(!arcIds)
            eaiCreate(&arcIds);
        eaiPopAll(&arcIds);
        //compose the search from its elements; start at 1 to skip MSSEARCH_ALL_ARC_IDS
        for(i = 1; i < MSSEARCH_COUNT && (firstTime || eaiSize(&arcIds)); i++)
        {
            if(searchParameters.sort != i)
            {
                nValidSearchParameters += s_searchByTypeSwitch(i, &searchParameters, &arcIds);
                if(nValidSearchParameters)
                    firstTime = 0;
            }
        }

        if(searchParameters.sort > 0 
            && searchParameters.sort < MSSEARCH_COUNT 
            && (!nValidSearchParameters || eaiSize(&arcIds)))
        {
            if(s_performComponentSort(search, &arcIds))
                nValidSearchParameters++;
        }

        if(!nValidSearchParameters && !searchParameters.arcid)
            s_performComponentSearchByType(&searchParameters, MSSEARCH_ALL_ARC_IDS, &arcIds, MSSEARCH_COMBINE_OR);

        if(!searchParameters.sort)
        {
            eaiClear(&search->arcids);
            eaiCopy(&search->arcids, &arcIds);
            if(eaiSize(&search->arcids))
                search->nextid = search->arcids[eaiSize(&search->arcids)-1] + 1;
        }
        //_time64(&search->timestamp);
        if(MISSIONSEARCH_CACHE_COMPOSED)
            plDirty(MissionServerCSearch, search);
    }

    //TODO: allow searches for multiple arcs
    StructClear(parse_MissionSearchParams, &searchParameters);
    search->hits++;
    if(MISSIONSEARCH_CACHE_COMPOSED)
    {
        s_enforceComposedLimit();
        s_refreshComposedSearch(search);
    }
    //SEARCH_REFRESH(search, &composed_head, &composed_tail, s_getCSize(search));
    s_searchesPerMinute++;
    return search;
}

int missionserver_GetSearchResults(int ***parcids, const char *params_str, int *firstArc)
{
    MissionServerCSearch *search;
    static int *userArrays[MSSEARCH_MAX_SORTBUCKETS] = {0};    //this is just so that the arrays we senotd out into the world beyond this function don't effect the search results
    int i;
    int arcIdFound = 0;

    search = s_getSearch(params_str);

    if(search->params.sort>0)
    {
        for(i = 0; i < MSSEARCH_MAX_SORTBUCKETS; i++)
        {
            if(search->arcidsBySort[i])
            {
                s_resultsReturned += MIN(eaiSize(&search->arcidsBySort[i]), 25);
                eaiCopy(&userArrays[i], &search->arcidsBySort[i]);
                if(search->params.arcid)
                    arcIdFound |= (eaiSortedFindAndRemove(&userArrays[i], search->params.arcid)>=0);
                eaPush(parcids, userArrays[i]);
            }
        }
    }
    else
    {
        s_resultsReturned += MIN(eaiSize(&search->arcids), 25);
        eaiCopy(&userArrays[0], &search->arcids);
        if(search->params.arcid)
            arcIdFound |= (eaiSortedFindAndRemove(&userArrays[0], search->params.arcid)>=0);
        eaPush(parcids, userArrays[0]);
    }

    if(arcIdFound)
        *firstArc = search->params.arcid;
    else
        *firstArc =0;

    return 1;
}

#include "autogen/missionserversearch_h_ast.c"

























//////////////////////////////////////////////////////////////////////////////////////////////////////////
// TESTING CODE
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// UNIT TESTS ///////////////////////////
//test s_updateCompositeSearch(MissionServerArc *arc, MissionServerCSearch *search, MissionServerSearchArcAction add)
//s_isArcInCompositeSearch(MissionServerArc *arc, MissionServerCSearch *search)
//s_updateAllCompositeSearches(MissionServerArc *arc, MissionServerSearchArcAction add)
//////////////////// SEARCH FUNCTIONALITY TESTS ///////////////////////////
char * MissionServerSearchTest_isArcInSearch(MissionServerCSearch *search, int arcId)
{
    int i;
    int found = 0;
    static char *error = 0;
    if(!error)
        estrCreate(&error);

    if(search->params.sort>0)
    {
        for(i = 0; i < MSSEARCH_MAX_SORTBUCKETS && !found; i++)
        {
            if(search->arcidsBySort[i])
            {
                found |= (eaiSortedFind(&search->arcidsBySort[i], arcId)!=-1);
            }
        }
    }
    else
    {
        found |= (eaiSortedFind(&search->arcids, arcId)!=-1);
    }
    if (found)
        return 0;
    else
    {
        estrPrintf(&error, "Arc %d not found in search.", arcId);
        return error;
    }
}
char * MissionServerSearchTest_noDuplicateArcs(MissionServerCSearch *search)
{
    int i;
    int lastId;
    int size;
    static int *ids = 0;
    static int *badIds = 0;
    static int *badIdsTemp = 0;
    static char *error = 0;

    eaiPopAll(&ids);
    eaiPopAll(&badIds);
    if(!error)
        estrCreate(&error);

    if(search->params.sort>0)
    {
        for(i = 0; i < MSSEARCH_MAX_SORTBUCKETS; i++)
        {
            if(search->arcidsBySort[i])
            {
                //check to make sure that these lists have nothing in common
                eaiSortedIntersection(&badIdsTemp, &ids, &search->arcidsBySort[i]);
                if(eaiSize(&badIdsTemp))
                {
                    s_searchOR(&badIds, &badIdsTemp);
                }
                s_searchOR(&ids, &search->arcidsBySort[i]);
            }
        }
    }
    else
    {
        eaiCopy(&ids, &search->arcids);
    }

    size = eaiSize(&ids);
    if(!size)
        return 0;

    lastId = ids[0];
    for(i = 1; i < size; i++)
    {
        if(ids[i]<=lastId)
            eaiPush(&badIds, ids[i]);
        lastId = ids[i];
    }

    if(eaiSize(&badIds))
    {
        estrPrintf(&error, "Duplicate ids found in sorted search.  {");
        for(i = 0; i < eaiSize(&badIds); i++)
        {
            estrConcatf(&error, "%d ", badIds[i]);
        }
        estrConcatChar(&error, '}');
        return error;
    }
    return 0;
}
char * MissionServerSearchTest_arcsMatchSearch(MissionServerCSearch *search)
{
    int i;
    int foundMismatch = 0;
    int size;
    static int *ids = 0;
    static char *error = 0;

    eaiPopAll(&ids);
    if(!error)
        estrCreate(&error);

    if(search->params.sort>0)
    {
        for(i = 0; i < MSSEARCH_MAX_SORTBUCKETS; i++)
        {
            if(search->arcidsBySort[i])
            {
                s_searchOR(&ids, &search->arcidsBySort[i]);
            }
        }
    }
    else
    {
        eaiCopy(&ids, &search->arcids);
    }

    size = eaiSize(&ids);
    if(!size)
        return 0;
    estrPrintCharString(&error, "Arc(s) did not belong in search: ");

    for(i = 0; i < size; i++)
    {
        MissionServerArc *arc = plFind(MissionServerArc, ids[i]);
        if(!arc)
        {
            estrConcatf(&error, "%d(does not exist)", ids[i]);
        }
        else if(!s_isArcInCompositeSearch(arc,search))
        {
            foundMismatch =1;
            estrConcatf(&error, "%d ", ids[i]);
        }
    }

    if(foundMismatch)
        return error;
    return 0;
}

#define N_TESTS_PER_ARC 10
typedef enum MissionServerSearchTestParams {
    MSSEARCHTEST_TEXT = 0,
    MSSEARCHTEST_VGROUP,
    MSSEARCHTEST_RATING,
    MSSEARCHTEST_HONORS,
    MSSEARCHTEST_BAN,
    MSSEARCHTEST_MORALITY,
    MSSEARCHTEST_LENGTH,
    MSSEARCHTEST_LOCALE,
    MSSEARCHTEST_COUNT,
} MissionServerSearchTestParams;

static int searchVariationFieldsByIteration[N_TESTS_PER_ARC][MSSEARCHTEST_COUNT] = {    {1,0,0,0,0,0,0,0}, //1 just text
                                                                                        {1,1,0,0,0,0,0,0}, //2
                                                                                        {1,0,1,0,0,0,0,0}, //3
                                                                                        {1,0,0,1,0,0,0,0}, //4
                                                                                        {1,0,0,0,1,0,0,0}, //5
                                                                                        {1,0,0,0,0,1,0,0}, //6
                                                                                        {1,0,0,0,0,0,1,0}, //7
                                                                                        {1,0,0,0,0,0,0,1}, //8
                                                                                        {0,0,1,1,0,1,1,0}, //9
                                                                                        {0,0,0,1,0,0,1,0}}; //10
#define STRING_COMBOS(length) ((length+1)*(length/2))
void MissionServerSearchTest_getSubstringById(char **estringOut, char * incoming, int id)
{
    int letters = strlen(incoming);
    int startingLetter = -1, endingLetter, i, j;
    devassert(id <= STRING_COMBOS(letters));
    estrClear(estringOut);
    estrSetLength(estringOut, letters+1);
    if(id<=0)    //no string case.
        return;
    id--;
    for(i = 0; i < letters && startingLetter < 0; i++)
    {
        if(id < (letters-i))
            startingLetter = i;
        else
            id -=(letters-i);
    }
    assert(startingLetter >=0);    //otherwise we're providing a bad index
    assert(id >=0);
    endingLetter = startingLetter + id;
    estrSetLength(estringOut, endingLetter-startingLetter+1);
    for(i = startingLetter, j=0; i <= endingLetter && j < endingLetter-startingLetter+1; i++, j++)
    {
        (*estringOut)[j] =incoming[i];
    }
}

MissionSearchParams MissionServerSearchTest_makeSearchVariation(MissionSearchParams params, int iteration)
{
    iteration %= N_TESTS_PER_ARC;
    params.author = 0;
    params.arcid = params.missionTypes = 0;
    if(searchVariationFieldsByIteration[iteration][MSSEARCHTEST_TEXT])
    {
        static char *text = 0;
        int maxCombos = STRING_COMBOS(strlen(params.text))-1;
        int randId = (rand()*maxCombos)/RAND_MAX;
        devassert(randId <= maxCombos);
        if(!text)
            estrCreate(&text);
        MissionServerSearchTest_getSubstringById(&text, params.text, randId);
        missionsearch_formatSearchString(&text, MSSEARCH_QUOTED_ALLOWED);
        params.text = StructAllocString(text);
    }
    else
        params.text = 0;
    if(!searchVariationFieldsByIteration[iteration][MSSEARCHTEST_VGROUP])
        memset(params.villainGroups,0, MISSIONSEARCH_VG_BITFIELDSIZE);
    if(iteration < 8)
        params.sort = MSSEARCH_RATING;
    else if(iteration == 8)
        params.sort = MSSEARCH_ARC_LENGTH;
    else
        params.sort =MSSEARCH_ALL_ARC_IDS;

    if(!searchVariationFieldsByIteration[iteration][MSSEARCHTEST_RATING])
        params.rating = 0;
    if(!searchVariationFieldsByIteration[iteration][MSSEARCHTEST_HONORS])
        params.honors = 0;
    if(!searchVariationFieldsByIteration[iteration][MSSEARCHTEST_BAN])
        params.ban = 0;
    if(!searchVariationFieldsByIteration[iteration][MSSEARCHTEST_MORALITY])
        params.morality = 0;
    if(!searchVariationFieldsByIteration[iteration][MSSEARCHTEST_LENGTH])
    params.arcLength = 0;
    if(!searchVariationFieldsByIteration[iteration][MSSEARCHTEST_LOCALE])
        params.locale_id = 0;

    return params;
}

int MissionServerSearchTest_SimpleSearch()
{
    MissionServerArc *arc;
    static int arcIdx = 0;
    static MissionServerArc **allArcs = 0;
    static int size;
    MissionServerSearchType type = 0;
    static char *estr =0;
    MissionServerCSearch *resultSearch;
    MissionSearchParams search;
    int i;

    if(!allArcs)
    {
        allArcs = plGetAll(MissionServerArc);
        size = eaSize(&allArcs);
    }
    if(!size)
        return 0;

    arc = allArcs[arcIdx];
    while(!missionserverarc_isIndexed(arc))
    {
        arcIdx = (arcIdx+1)%size;
        arc = allArcs[arcIdx];
    }
    arcIdx = (arcIdx+1)%size;

    search = s_arcToSearch(arc);


    for(i = 0; i < N_TESTS_PER_ARC; i++)
    {
        char * error = 0;
        int hasErrors = 0;
        MissionSearchParams testParams =MissionServerSearchTest_makeSearchVariation(search, i);
        assert(ParserWriteText(&estr, parse_MissionSearchParams, &testParams, 0, 0));

        resultSearch = s_getSearch(estr);
        estrClear(&estr);
        StructFreeString(testParams.text);
    }
    return 1;
}

int MissionServerSearchTest_SimplePublish()
{
    MissionServerArc *arc;
    static int arcIdx = 0;
    static MissionServerArc **allArcs = 0;
    static int size;

    if(!allArcs)
    {
        allArcs = plGetAll(MissionServerArc);
        size = eaSize(&allArcs);
    }
    if(!size)
        return 0;

    arc = allArcs[arcIdx];
    while(!missionserverarc_isIndexed(arc))
    {
        arcIdx = (arcIdx+1)%size;
        arc = allArcs[arcIdx];
    }
    arcIdx = (arcIdx+1)%size;

    arc->id +=size;
    missionserver_IndexNewArc(arc);
    missionserver_UnindexArc(arc);
    arc->id -=size;
    return 1;
}

int MissionServerSearchTest_SimpleVote()
{
    MissionServerArc *arc;
    static int arcIdx = 0;
    static MissionServerArc **allArcs = 0;
    static int size;
    int randomVote = 0;

    if(!allArcs)
    {
        allArcs = plGetAll(MissionServerArc);
        size = eaSize(&allArcs);
    }
    if(!size)
        return 0;

    arc = allArcs[arcIdx];
    while(!missionserverarc_isIndexed(arc))
    {
        arcIdx = (arcIdx+1)%size;
        arc = allArcs[arcIdx];
    }
    arcIdx = (arcIdx+1)%size;

    randomVote = (rand()%MISSIONRATING_RATINGTYPES);
    missionserver_UpdateArcRating(arc, randomVote);
    randomVote = (rand()%MISSIONHONORS_TYPES);
    missionserver_UpdateArcHonors(arc, randomVote);
    return 1;
}

static void reportResult_test(char **error, char *test, int *result)
{
    if((*error = test))
    {
        LOG_DEBUG("SearchFunctionalityTest %s", *error);
        *result = 1;
    }
}
int MissionServerSearchTest_SearchesForArc(MissionServerArc *arc)
{
    MissionServerSearchType type = 0;
    static char *estr =0;
    MissionServerCSearch *resultSearch;
    MissionSearchParams search = s_arcToSearch(arc);
    int i;
    int failure = 0;

    for(i = 0; i < N_TESTS_PER_ARC; i++)
    {
        char * error = 0;
        int hasErrors = 0;
        MissionSearchParams testParams =MissionServerSearchTest_makeSearchVariation(search, i);
        assert(ParserWriteText(&estr, parse_MissionSearchParams, &testParams, 0, 0));
        
        resultSearch = s_getSearch(estr);
        reportResult_test(&error, MissionServerSearchTest_isArcInSearch(resultSearch, arc->id), &hasErrors);
        reportResult_test(&error, MissionServerSearchTest_noDuplicateArcs(resultSearch), &hasErrors);
        reportResult_test(&error, MissionServerSearchTest_arcsMatchSearch(resultSearch), &hasErrors);
        if(hasErrors)
            LOG_DEBUG("SearchFunctionalityTest FAIL test arc search test %d;  %s", i, estr);
        estrClear(&estr);
        failure |=hasErrors;
        StructFreeString(testParams.text);
    }
    return failure;
}
int MissionServerSearchTest_SearchesAndPublish(MissionServerArc *arc, int newId)
{
    MissionServerSearchType type = 0;
    static char *estr =0;
    MissionServerCSearch *resultSearch;
    MissionSearchParams search = s_arcToSearch(arc);
    static char **searches = 0;
    int i;
    int failure = 0;
    int oldId = arc->id;

    for(i = 0; i < N_TESTS_PER_ARC; i++)
    {
        char * error = 0;
        int hasErrors = 0;
        MissionSearchParams testParams =MissionServerSearchTest_makeSearchVariation(search, i);
        assert(ParserWriteText(&estr, parse_MissionSearchParams, &testParams, 0, 0));

        resultSearch = s_getSearch(estr);
        eaPush(&searches, estrCloneEString(estr));
        reportResult_test(&error, MissionServerSearchTest_isArcInSearch(resultSearch, arc->id), &hasErrors);
        reportResult_test(&error, MissionServerSearchTest_noDuplicateArcs(resultSearch), &hasErrors);
        reportResult_test(&error, MissionServerSearchTest_arcsMatchSearch(resultSearch), &hasErrors);
        if(hasErrors)
            LOG_DEBUG("SearchFunctionalityTest FAIL test arc search test %d;  %s", i, estr);
        assert(!estr || !strchr(estr, (unsigned char)-52));
        estrClear(&estr);
        failure |=hasErrors;
        StructFreeString(testParams.text);
    }
    
    arc->id+=newId;
    missionserver_IndexNewArc(arc);
    
    for(i = 0; i < N_TESTS_PER_ARC; i++)
    {
        char * error = 0;
        int hasErrors = 0;
        char *searchStr = eaPop(&searches);
        resultSearch = s_getSearch(searchStr);
        reportResult_test(&error, MissionServerSearchTest_isArcInSearch(resultSearch, arc->id), &hasErrors);
        reportResult_test(&error, MissionServerSearchTest_isArcInSearch(resultSearch, oldId), &hasErrors);
        reportResult_test(&error, MissionServerSearchTest_noDuplicateArcs(resultSearch), &hasErrors);
        reportResult_test(&error, MissionServerSearchTest_arcsMatchSearch(resultSearch), &hasErrors);
        if(hasErrors)
            LOG_DEBUG("SearchFunctionalityTest FAIL test arc search test %d;  %s", i, searchStr);
        failure |=hasErrors;
        estrDestroy(&searchStr);
    }
    MissionServerSearchTest_SearchesForArc(arc);
    assert(!eaSize(&searches));
    missionserver_UnindexArc(arc);
    arc->id =oldId;
    estrClear(&estr);
    return failure;
}
int MissionServerSearchTest_VoteAndSearch(MissionServerArc *arc)
{
    int failure = 0;
    missionserver_UpdateArcRating(arc, MISSIONRATING_4_STARS);
    failure |=MissionServerSearchTest_SearchesForArc(arc);
    missionserver_UpdateArcRating(arc, MISSIONRATING_1_STARS);
    failure |=MissionServerSearchTest_SearchesForArc(arc);
    missionserver_UpdateArcHonors(arc, MISSIONHONORS_HALLOFFAME);
    failure |=MissionServerSearchTest_SearchesForArc(arc);
    missionserver_UpdateArcHonors(arc, MISSIONHONORS_DEVCHOICE);
    failure |=MissionServerSearchTest_SearchesForArc(arc);
    missionserver_UpdateArcHonors(arc, MISSIONHONORS_HALLOFFAME);
    failure |=MissionServerSearchTest_SearchesForArc(arc);
    missionserver_UpdateArcRating(arc, MISSIONRATING_1_STARS);
    failure |=MissionServerSearchTest_SearchesForArc(arc);
    return failure;
}

int MissionServerSearchTest_TestAllArcSearch()
{
    MissionServerArc **allArcs = plGetAll(MissionServerArc);
    int nArcs = eaSize(&allArcs);
    int i;
    int failure = 0;
    char *buf = Str_temp();
    for(i =0; i < nArcs; i++)
    {
        static int lastFeedbackUpdate =0;
        int percent = i*100/nArcs;

        if(percent != lastFeedbackUpdate)
        {
            lastFeedbackUpdate = percent;
            Str_clear(&buf);
            Str_catf(&buf, "%d: MissionServer. Testing All Arc Search: arc %d (%d). %d %%", _getpid(), i, nArcs, i*100/nArcs);
            setConsoleTitle(buf);
        }
        if(!allArcs[i]->unpublished && !allArcs[i]->invalidated)
            failure |=MissionServerSearchTest_SearchesForArc(allArcs[i]);
    }
    Str_destroy(&buf);
    return failure;
}
int MissionServerSearchTest_TestAllArcSearchVote()
{
    MissionServerArc **allArcs = plGetAll(MissionServerArc);
    int nArcs = eaSize(&allArcs);
    
    int i;
    int failure = 0;
    char *buf = Str_temp();
    for(i =0; i < nArcs; i+=2)
    {
        static int lastFeedbackUpdate =0;
        int percent = i*100/nArcs;

        if(percent != lastFeedbackUpdate)
        {
            lastFeedbackUpdate = percent;
            Str_clear(&buf);
            Str_catf(&buf, "%d: MissionServer. Testing Search and Vote: arc %d (%d). %d %%", _getpid(), i, nArcs, i*100/nArcs);
            setConsoleTitle(buf);
        }
        if(!allArcs[i]->unpublished && !allArcs[i]->invalidated)
            failure |=MissionServerSearchTest_VoteAndSearch(allArcs[i]);
    }
    Str_destroy(&buf);
    return failure;
}
int MissionServerSearchTest_TestAllArcSearchPublish()
{
    MissionServerArc **allArcs = plGetAll(MissionServerArc);
    int nArcs = eaSize(&allArcs);
    int maxId = allArcs[nArcs-1]->id;
    int i;
    int failure = 0;
    char *buf = Str_temp();
    int liveArcs = 0;
    for(i =0; i < nArcs; i+=2)
    {
        static int lastFeedbackUpdate =0;
        int percent = i*100/nArcs;

        if(percent != lastFeedbackUpdate)
        {
            lastFeedbackUpdate = percent;
            Str_clear(&buf);
            Str_catf(&buf, "%d: MissionServer. Testing Search and Publish: arc %d (%d). %d %%", _getpid(), i, nArcs, i*100/nArcs);
            setConsoleTitle(buf);
        }
        if(!allArcs[i]->unpublished && !allArcs[i]->invalidated)
        {
            failure |=MissionServerSearchTest_SearchesAndPublish(allArcs[i], allArcs[i]->id+maxId);
            liveArcs++;
        }
    }
    Str_destroy(&buf);
    printf( "%d total arcs\n", liveArcs);
    return failure;
}

//all rates are per second
#define TEST_SEARCH_RATE (20000/60)
#define TEST_PUBLISH_RATE (1000/60)
#define TEST_VOTE_RATE (66)
int MissionServerSearchTest_SimulateSeconds(int seconds)
{
    int i;
    int repetitions;
    //search
    repetitions = seconds * (TEST_SEARCH_RATE/N_TESTS_PER_ARC);
    for(i = 0; i < repetitions; i++)
    {
        MissionServerSearchTest_SimpleSearch();
    }
    //publish
    repetitions = seconds * (TEST_PUBLISH_RATE/2);
    for(i = 0; i < repetitions; i++)
    {
        MissionServerSearchTest_SimplePublish();
    }
    //vote
    repetitions = seconds * (TEST_VOTE_RATE/2);
    for(i = 0; i < repetitions; i++)
    {
        MissionServerSearchTest_SimpleVote();
    }
    return 1;
}

int MissionServerSearchTest_FullRun()
{
    __time64_t now;
    static __time64_t lastFullRunCall = 0;
    static int multiplier = 2;
    U32 d;

    _time64(&now);

    if(!lastFullRunCall)
    {
        printf("WARNING!!!!  You're running in testing mode.  You're going to run slow!!!!\n");
        _time64(&lastFullRunCall);
        return 0;
    }
    d = _difftime64(now, lastFullRunCall);
    if(d<=0)
        return 0;
    else if(d > 1)
        printf("WARNING!!!!  Tick took longer than a second!!!!\n");

    lastFullRunCall = now;
    MissionServerSearchTest_SimulateSeconds(multiplier*1);
    missionserversearch_PrintStats();
    return 1;
}

static char testRequest[] = "zoos are xu boy yak car where ar zo xu oy ya ere ar";
static char testResult[] =  "are boy car where xu yak zoos";
int MissionServerSearchTest_SearchFormatting()
{
    char * estr = estrTemp();
    estrPrintCharString(&estr, testRequest);
    missionsearch_formatSearchString(&estr, 0);
    assert(!strcmp(testResult, estr));
    estrDestroy(&estr);
    return 1;
}
