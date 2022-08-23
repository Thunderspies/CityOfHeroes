#include <utilitieslib/components/earray.h>
#include <utilitieslib/utils/mathutil.h>
#include "clientcomm/clientcomm.h"
#include <utilitieslib/utils/textparser.h>
#include "storyarc/playerCreatedStoryarc.h"
#include "storyarc/playerCreatedStoryarcValidate.h"
#include "player/player.h"
#include <utilitieslib/components/EString.h>
#include "entity/entity.h"
#include "gameComm/VillainDef.h"
#include <utilitieslib/components/bitfield.h>
#include "gameData/randomName.h"
#include "storyarc/missionMapCommon.h"
#include <utilitieslib/utils/structDefines.h>
#include "storyarc/pnpcCommon.h"
#include "clientcomm/clientcomm.h"
#include <utilitieslib/utils/utils.h>



#include "UI/uiNet.h"

#define testRandInt2(max) (rand()*max-1)/RAND_MAX
#define MAX_MISSION_TIME 300
#define MAX_VILLAINS 100
#define PPS_REPORT_TIME 60
#define MAX_RESULTS 25
#define SEARCH_RETRY 20

char *s_wordList[] = {
#include "test_dict.txt"
};


typedef struct MissionSearchTestResult
{
    int id;
    MissionSearchInfo rating;
    MissionSearchHeader header;
} MissionSearchTestResult;

int missionserver_isInSearch(MissionSearchTestResult *arc, MissionSearchParams *searchParameters, MissionServerSearchType type);



static int testMissionServerLoadedVillains = 0;
static int *s_receivedArcIds = 0;
static int s_receivedPages = 0;
static int s_receivedPage = 0;
static int s_currentHeader = 0;
static int *s_allArcIds = 0;
static int s_requestingAllArcIds = 0;
static int s_allArcIdsPages = 0;
static int s_allArcIdsPage = 0;
static int s_nVotesRequested = 0;
static int s_searchBaseLoading = 0;
static int s_searchVerifying = 0;
static int s_verifyIdPresent = 0;
static int s_verifyAllGoodResults = 0;
static MissionSearchParams s_search = {0};
static MissionSearchTestResult s_searchBase = {0};
static MissionSearchTestResult s_results[MAX_RESULTS];
static U32 s_base_timer = 0;
static U32 s_verify_timer = 0; 
static int s_noValidation = 0;

int testMissionSearch_hasAllIds()
{
    return (!s_requestingAllArcIds &&!!eaiSize(&s_allArcIds));
}

int *testMissionSearch_getAllPublishedArcIds()
{
    if(testMissionSearch_hasAllIds())
        return s_allArcIds;
    else
        return 0;
}

void testMissionSearch_searchAll(int page)
{
    MissionSearchParams search = {0};
    char *estr =estrTemp();
    search.text="";
    missionban_setVisible(&search.ban);
    ParserWriteText(&estr, parse_MissionSearchParams, &search, 0, 0);
    missionserver_game_requestSearchPage(MISSIONSEARCH_ALL, estr, page, 0, 0,0);
    estrDestroy(&estr);
}

void testMissionSearch_registerPages(int page, int pages)
{
    if(s_noValidation)
        return;
    if(s_requestingAllArcIds)
    {
        s_allArcIdsPages =pages;
        //either this is the last page we asked for or we're out of sync
        //it's possible this could happen because of unpublishes.  
        //I'm assuming that we don't ask for this while doing
        //unpublishes, since that's not part of my test.
        assert(s_allArcIdsPage==page);
        s_allArcIdsPage = page+1;
        if(s_allArcIdsPage>=s_allArcIdsPages)
            s_requestingAllArcIds=0;
        else
            testMissionSearch_searchAll(s_allArcIdsPage);
    }
    else if(s_searchBaseLoading)
    {
        assert(!page && pages == 1);    //we should only ever get one search result back when we're getting the base.
        if(!s_searchBase.id)
            s_search.arcid = 0;
        else
            s_searchBaseLoading =0;
        timerFree(s_base_timer);
        s_base_timer = 0;
    }
    else if(s_searchVerifying)
    {
        //if we're at the last page, assert that the test was successful
        if(page == pages-1)
        {
            if(!s_verifyAllGoodResults)
                printf("Warning: search for %s, sort %d had at least one bad result\n", s_search.text, s_search.sort);
            if(!s_verifyIdPresent)
                printf("Warning: search for %s, sort %d did not find arc %d\n", s_search.text, s_search.sort, s_searchBase.id);
            //assert(s_verifyAllGoodResults && s_verifyIdPresent);
            s_verifyIdPresent = 0;
            s_verifyAllGoodResults = 1;
            s_searchVerifying = 0;
            timerFree(s_verify_timer);
            s_verify_timer = 0;
            eaiPopAll(&s_receivedArcIds);
        }
        else
        {
            static char *estr = 0;
            if(!estr)
                estrCreate(&estr);
            estrClear(&estr);
            ParserWriteText(&estr, parse_MissionSearchParams, &s_search, 0, 0);
            missionserver_game_requestSearchPage(MISSIONSEARCH_ALL, estr, page+1, 0, 0,0);
            timerStart(s_verify_timer);
        }
    }
    else
    {

        s_receivedPages = pages;
        s_receivedPage = page;
    }

}



void testMissionSearch_requestAllPublishedArcIds()
{
    eaiPopAll(&s_allArcIds);
    s_requestingAllArcIds = 1;
    s_allArcIdsPage=0;
    testMissionSearch_searchAll(s_allArcIdsPage);
}

void testMissionSearch_testVote(int arcId)
{
    int randomVote = testRandInt2(MISSIONRATING_MAX_STARS);
    
    missionserver_game_voteForArc(arcId, randomVote);
    s_nVotesRequested++;
    //NOTE: this could change locally held arcs expected rating.
}

void testMissionSearch_testVoteRandom(int nVotes)
{
    int *arcIds = testMissionSearch_getAllPublishedArcIds();
    int nArcs = eaiSize(&arcIds);
    int randomIdx;
    int i;
    if(!arcIds)    //will be NULL unless fully loaded
    {
        if(!s_requestingAllArcIds)
            testMissionSearch_requestAllPublishedArcIds();
        return;
    }
    for(i = 0; i < nVotes; i++)
    {
        randomIdx = testRandInt2(nArcs);
        testMissionSearch_testVote(arcIds[randomIdx]);
    }
}

void testMissionSearch_testPlayRandom()
{
    int size = eaiSize(&s_receivedArcIds);
    int *arcIds = testMissionSearch_getAllPublishedArcIds();
    int nArcs = eaiSize(&arcIds);
    int randomIdx;
    if(!arcIds) //will be NULL unless fully loaded
    {
        if(!s_requestingAllArcIds)
            testMissionSearch_requestAllPublishedArcIds();
        return;
    }

    randomIdx = testRandInt2(nArcs);
    //TODO play non-test.
    commAddInput("/taskforcedestroy");
    missionserver_game_requestArcData_playArc(randomIdx, 1, 0);
}

void testMissionSearch_forcePlay()
{
    int randomIdx;

    randomIdx = testRandInt2(1000);
    //TODO play non-test.
    commAddInput("/taskforcedestroy");
    missionserver_game_requestArcData_playArc(randomIdx, 1, 0);
}

static void s_getSubstringById(char **estringOut, char * incoming, int id)
{
    int letters = strlen(incoming);
    int startingLetter = -1, endingLetter, i, j;
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

static void s_makeSearchVariation(MissionSearchParams *search,
                                    int author_i,
                                    int summary_i,
                                    int title_i,
                                    int sort_i)
{
    static char*authorPart = 0;
    static char*summaryPart = 0;
    static char*titlePart = 0;

    
    if(!authorPart)
        estrCreate(&authorPart);
    if(!summaryPart)
        estrCreate(&summaryPart);
    if(!titlePart)
        estrCreate(&titlePart);
    
    s_getSubstringById(&authorPart, s_searchBase.header.author, author_i);
    s_getSubstringById(&summaryPart, s_searchBase.header.summary, summary_i);
    s_getSubstringById(&titlePart, s_searchBase.header.title, title_i);
    estrPrintf(&search->text, "%s %s %s", authorPart, summaryPart, titlePart);
    missionsearch_formatSearchString(&search->text, MSSEARCH_QUOTED_ALLOWED);


    if(!sort_i)
        search->sort = MSSEARCH_RATING;
    else if(sort_i == 1)
        search->sort = MSSEARCH_ARC_LENGTH;
    else
        search->sort = MSSEARCH_ALL_ARC_IDS;
};

#define STRING_COMBOS(length) ((length+1)*(length/2))
#define STRING_VARIATIONS(length) (MIN(5, STRING_COMBOS(length)))
#define SORT_VARIATIONS 3
void testMissionSearch_testSearch()
{
    static int currentIdx = -1;
    static int iteration = 0;
    static int iterations = 0;
    static int author_i, title_i, summary_i, sort_i;
    static int author_is, title_is, summary_is;
    static int firstTime = 0;
    static char *estr =0;
    int refresh = 0;
    


    char *tempStr = 0;

    int *arcIds = testMissionSearch_getAllPublishedArcIds();
    int nArcs = eaiSize(&arcIds);
    if(!estr)
        estrCreate(&estr);
    if(!arcIds)    //will be NULL unless fully loaded
    {
        if(!s_requestingAllArcIds)
        {
            firstTime = 1;
            testMissionSearch_requestAllPublishedArcIds();
        }
        return;
    }
    if(!s_searchVerifying &&((!s_searchBaseLoading && firstTime)||(iteration >= iterations) 
        || (s_base_timer &&(timerElapsed(s_base_timer) > SEARCH_RETRY))))    //first time through, request an arc
    {
        if(!s_base_timer)
            s_base_timer = timerAlloc();
        timerStart(s_base_timer);
        firstTime = 0;
        if(!s_base_timer || !(timerElapsed(s_base_timer) > SEARCH_RETRY))
        {
            if(currentIdx < 0)
                currentIdx = testRandInt2(nArcs);
            else
                currentIdx++;
            currentIdx %= nArcs;
        }
        iteration = -1;
        iterations = 0;
        memset(&s_search, 0, sizeof(MissionSearchParams));//clear out the last search
        memset(&s_searchBase, 0, sizeof(MissionSearchTestResult));//clear out the last search
        s_search.arcid = arcIds[currentIdx];
        s_searchBaseLoading = 1;
        ParserWriteText(&estr, parse_MissionSearchParams, &s_search, 0, 0);
        missionserver_game_requestSearchPage(MISSIONSEARCH_ALL, estr, 0, 0, 0,0);
        return;
    }
    else if(s_searchBaseLoading || (s_searchVerifying && (!s_verify_timer || !(timerElapsed(s_verify_timer) > SEARCH_RETRY))))
        return;

    //we have the arc in s_searchBase
    iteration++;
    if(iterations == 0)
    {
        //recalculate iterations
        author_is=STRING_VARIATIONS(strlen(s_searchBase.header.author));//STRING_COMBOS(strlen(s_searchBase.header.author))+1;
        summary_is=STRING_VARIATIONS(strlen(s_searchBase.header.summary));//STRING_COMBOS(strlen(s_searchBase.header.summary))+1;
        title_is=STRING_VARIATIONS(strlen(s_searchBase.header.title));//STRING_COMBOS(strlen(s_searchBase.header.title))+1;
        iterations = author_is*summary_is*title_is*SORT_VARIATIONS;
        sort_i=author_i=title_i=summary_i=0;
    }

    if(author_i >=author_is)
    {
        author_i = 0;
        summary_i++;
        if(summary_i>=summary_is)
        {
            summary_i = 0;
            title_i++;
            if(title_i >=title_is)
            {
                title_i = 0;
                sort_i++;
                if(sort_i>=SORT_VARIATIONS)
                {
                    devassert(iterations ==iteration);
                    return;
                }
            }
        }

    }
    tempStr = s_search.text;
    memset(&s_search, 0, sizeof(MissionSearchParams));//clear out the last search
    s_search.text = tempStr;
    s_searchVerifying = 1;
    s_verifyIdPresent = 0;
    s_verifyAllGoodResults = 1;
    s_makeSearchVariation(&s_search, author_i, summary_i, title_i, sort_i);
    ParserWriteText(&estr, parse_MissionSearchParams, &s_search, 0, 0);
    if(!s_verify_timer)
        s_verify_timer = timerAlloc();
    timerStart(s_verify_timer);
    missionserver_game_requestSearchPage(MISSIONSEARCH_ALL, estr, 0, 0, 0,0);
    estrDestroy(&estr);
    author_i++;
}


void testMissionSearch_registerHeader(int id, char *header, int rating)
{
    MissionSearchHeader *h = 0;
    if(s_currentHeader >=MAX_RESULTS)
        return;
    ZeroStruct(&s_results[s_currentHeader].header);

    ParserReadText(header, -1, parse_MissionSearchHeader, &s_results[s_currentHeader].header);
    s_results[s_currentHeader].id = id;
    s_results[s_currentHeader].rating.i = rating;
    s_currentHeader++;
}

static MissionSearchHeader *s_getHeader()
{
    MissionSearchHeader *h= &s_results[s_currentHeader-1].header;
    s_currentHeader = MAX(s_currentHeader-1, 0);
    return h;
}

static char *s_getRandomWord()
{
    int idx = testRandInt2(ARRAY_SIZE_CHECKED(s_wordList));
    return s_wordList[idx];
}

static void s_makeRandomString(char**string, unsigned int size)
{
    estrCreate(string);
    while(estrLength(string)<size)
        estrConcatf(string, "%s ", s_getRandomWord());
    estrSetLength(string, size);//TODO: set length to size of clue name.
}

void testMissionSearch_receiveArc(int arcId, char *headerStr, int rating)
{
    if(s_noValidation)
        return;
    if(s_requestingAllArcIds)
    {
        if(!s_allArcIds)
            eaiCreate(&s_allArcIds);
        //if we get mutliple ids, we're either testing incorrectly or (more likely) the search is broken.
        if(eaiSortedFind(&s_allArcIds, arcId)>=0)
            printf("Warning: search for %s, sort %d had duplicate ID %d\n", s_search.text, s_search.sort, arcId);
        else
            eaiSortedPushAssertUnique(&s_allArcIds, arcId);
    }
    else if(s_searchBaseLoading)
    {
        assert(arcId == s_search.arcid);    //this is the only result we should get.
        assert(!s_searchBase.id);            //otherwise, we're getting multiple ids back.
        s_searchBase.id = arcId;
        s_searchBase.rating.i = rating;
        StructClear(parse_MissionSearchHeader, &s_searchBase.header);
        ParserReadText(headerStr, -1, parse_MissionSearchHeader, &s_searchBase.header);
    }
    else if(s_searchVerifying)
    {
        int i;
        if(arcId == s_searchBase.id)
            s_verifyIdPresent = 1;
        else
        {
            MissionSearchTestResult results = {0};
            results.id = arcId;
            results.rating.i = rating;
            ParserReadText(headerStr, -1, parse_MissionSearchHeader, &results.header);
            for(i = 1; i < MSSEARCH_COUNT; i++)
                if(!(s_verifyAllGoodResults &=missionserver_isInSearch(&results, &s_search, i)))
                {
                    printf( "WARNING: search %s sort %d does not match result.\n", s_search.text, s_search.sort);
                    printf( "        --returned result arc %d with text %s %s %s\n",results.id, results.header.author, results.header.summary, results.header.title);
                }
            StructClear(parse_MissionSearchHeader, &results.header);
        }
        if(!s_receivedArcIds)
            eaiCreate(&s_receivedArcIds);
        if(eaiSortedFind(&s_receivedArcIds, arcId)>=0)
            printf( "Warning: search for %s, sort %d had duplicate ID %d\n", s_search.text, s_search.sort, arcId);
        else
            eaiSortedPushAssertUnique(&s_receivedArcIds, arcId);
    }
    else
    {
        if(!s_receivedArcIds)
            eaiCreate(&s_receivedArcIds);
        eaiSortedPushUnique(&s_receivedArcIds, arcId);
        testMissionSearch_registerHeader(arcId, headerStr, rating);
    }
}



void testMissionSearchDelete()
{
    int size = eaiSize(&s_receivedArcIds);
    int choice;
    if(!size)
        return;

    choice = testRandInt2(size);
    eaiRemove(&s_receivedArcIds, choice);
    missionserver_game_unpublishArc(choice);
}

static void s_makeRandomContact(char **contactCategory, char **contact)
{
    int randIdx = testRandInt2(eaSize(&gMMContactCategoryList.ppContactCategories));
    MMContactCategory *pContactCat = gMMContactCategoryList.ppContactCategories[randIdx];
    MMContact *pContact;
    estrCreate(contactCategory);
    estrConcatCharString(contactCategory, pContactCat->pchCategory);
    randIdx = eaSize(&pContactCat->ppContact);
    pContact = pContactCat->ppContact[randIdx];
    estrCreate(contact);
    estrConcatCharString(contact, (PNPCFindDef(pContact->pchNPCDef))->model);
}

#define TEST_MAX_ENT_ATTEMPTS 200
static int s_makeRandomEntity(char **group, char **entity, int *minLevel, int *maxLevel, int *groupIdx)
{
    int randIdx;
    int attempts = 0;
    const VillainDef *vDef = 0;
    while(!vDef)
    {
        MMVillainGroup *pGroup;
        if(attempts>=TEST_MAX_ENT_ATTEMPTS)
            return 0;

        randIdx = testRandInt2(eaSize(&villainDefList.villainDefs));
        if( !playerCreatedStoryarc_isValidVillainDefName( villainDefList.villainDefs[randIdx]->name, 0 ) )
            continue;

        /*if( villainDefs[randIdx]->rank == VR_SNIPER || 
            playerCreatedStoryarc_isCantMoveVillainDefName( villainDefList.villainDefs[randIdx]->name ) )
            continue;*/

        pGroup = playerCreatedStoryArc_GetVillainGroup( villainGroupGetName(villainDefList.villainDefs[randIdx]->group), 0 );
        if(pGroup &&  pGroup->maxLevel >= *minLevel && pGroup->minLevel <= *maxLevel  && villainDefList.villainDefs[randIdx]->rank >=VR_SNIPER)
        {
            vDef = villainDefList.villainDefs[randIdx];
            if( pGroup->minLevel > *minLevel )
                *minLevel = pGroup->minLevel;
            if( pGroup->maxLevel < *maxLevel )
                *maxLevel = pGroup->maxLevel;
        }

        if(vDef && !playerCreatedStoryarc_isValidVillainGroup( villainGroupGetName(vDef->group), NULL))
            vDef = NULL;
        
        attempts++;
    }
    if(group)
    {
        estrCreate(group);
        estrConcatCharString(group, villainGroupGetName(vDef->group));
    }
    if(entity)
    {
        estrCreate(entity);
        estrConcatCharString(entity, vDef->name);
    }
    if(groupIdx)
        *groupIdx = vDef->group;
    return 1;
}

static int s_fillOutFields( TokenizerParseInfo *tpi, void * structptr, int (*charlen)(const char *, int), int detailType)
{
    int i;
    FORALL_PARSETABLE( tpi, i )    
    {
        int charLength = (*charlen)(tpi[i].name, detailType);
        if(TOK_GET_TYPE(tpi[i].type) == TOK_STRING_X && charLength)
        {
            char *temp = 0;
            s_makeRandomString(&temp, charLength);
            SimpleTokenSetSingle( temp, tpi, i, structptr, 0 );
            estrDestroy(&temp);
        }
    }

    return 1;
}

static const MapLimit*s_makeMap(char **mapName, char **mapSet, int mapLength)
{
    int i;

    for(i = 0; i < TEST_MAX_ENT_ATTEMPTS; i++)
    {
        int randIdx = testRandInt2(eaSize(&g_missionmaplist.missionmapsets));
        const MissionMapSet *pMapSet = g_missionmaplist.missionmapsets[randIdx];
        const MapLimit* limit = missionMapStat_GetRandom( StaticDefineIntRevLookup(ParseMapSetEnum,pMapSet->mapset), mapLength);
        if(limit)
        {
            *mapName = "Random";
            estrCreate(mapSet);
            estrConcatCharString(mapSet, StaticDefineIntRevLookup(ParseMapSetEnum,pMapSet->mapset));
            return limit;
        }
    }
    return 0;
}

static int s_setQuantity(ObjectLocationType location, MapLimit *limit, int *qty)
{
    int max;
    if(!qty || !limit)
        return 0;
    switch(location)
    {
        xcase kObjectLocationType_Person:
            max = limit->regular_only[0];
        xcase kObjectLocationType_Wall:
            max = limit->wall[0];
        xcase kObjectLocationType_Floor:
            max = limit->floor[0];
        xcase kObjectLocationType_Roof:
            max = limit->around_only[0];
        xdefault:
            max = 0;
    }
    if(!max)
    {
        *qty = 0;
        return 0;
    }
    else
        *qty = testRandInt2(max)+1;

    return 1;
}


static PlayerCreatedDetail *s_makeDetail(int *minLevel, int *maxLevel, MapLimit *limit)
{
    PlayerCreatedDetail *eDetail = StructAllocRaw(sizeof(PlayerCreatedDetail));
    eDetail->eDetailType = kDetail_Boss;//testRandInt2(kDetail_Count-1);
    s_fillOutFields( ParsePlayerCreatedDetail, eDetail, &playerCreatedStoryArc_getDetailFieldCharLimit, eDetail->eDetailType);
    if(!s_makeRandomEntity(&eDetail->pchEntityGroupName, &eDetail->pchVillainDef, minLevel, maxLevel, 0) ||
        !s_makeRandomEntity(&eDetail->pchVillainGroupName, &eDetail->pchSupportVillainDef, minLevel, maxLevel, 0) ||
        ((eDetail->eDetailType ==kDetail_Rumble) && !s_makeRandomEntity(&eDetail->pchVillainGroup2Name, NULL, minLevel, maxLevel, 0))
        )
    {
        StructFree(eDetail);
        return 0;
    }
    eDetail->pchEntityType = eDetail->pchSupportVillainType = "Standard";
    
    eDetail->ePlacement = testRandInt2(kPlacement_Count);


    eDetail->eAlignment = testRandInt2(kAlignment_Count);
    if(eDetail->eAlignment == kAlignment_Ally &&
        (eDetail->eDetailType == kDetail_DestructObject ||    
        eDetail->eDetailType == kDetail_DefendObject ||
        eDetail->eDetailType == kDetail_Boss ||
        eDetail->eDetailType == kDetail_DefendObject) )
        eDetail->eAlignment--;

    eDetail->eDifficulty = testRandInt2(kDifficulty_Single);//(kDifficulty_Count);

    
    

    // Collection
    eDetail->iInteractTime = testRandInt2(50)+10;
    //if(eDetail->eDetailType == kDetail_Collection)
    //{
    /*    if(testRandInt2(2) && limit->wall>0)
        {
            eDetail->eObjectLocation =kObjectLocationType_Wall;
        }
        else
        {*/
            eDetail->eObjectLocation =kObjectLocationType_Floor;
        /*}
    }
    else
    {
        eDetail->eObjectLocation = testRandInt2(kObjectLocationType_Count);
    }*/
    eDetail->iQty = 1;
    /*if(!s_setQuantity(eDetail->eObjectLocation, limit, &eDetail->iQty))
    {
        StructFree(eDetail);
        return 0;
    }*/
    eDetail->bObjectiveRemove = testRandInt2(2);
    

    // Ambush
    //char *pchAmbushTrigger;



    // Person
    eDetail->ePersonCombatType = testRandInt2(kPersonCombat_Count);
    //char *pchEscortDestination;
    
    eDetail->pchPersonStartingAnimation = gMMAnimList.anim[testRandInt2(eaSize(&gMMAnimList.anim))]->pchName;
    eDetail->pchRescueAnimation = gMMAnimList.anim[testRandInt2(eaSize(&gMMAnimList.anim))]->pchName;
    eDetail->pchReRescueAnimation = gMMAnimList.anim[testRandInt2(eaSize(&gMMAnimList.anim))]->pchName;
    eDetail->pchArrivalAnimation = gMMAnimList.anim[testRandInt2(eaSize(&gMMAnimList.anim))]->pchName;
    eDetail->pchStrandedAnimation = gMMAnimList.anim[testRandInt2(eaSize(&gMMAnimList.anim))]->pchName;

    eDetail->ePersonRescuedBehavior = testRandInt2(kPersonBehavior_Count);
    eDetail->ePersonArrivedBehavior = testRandInt2(kPersonBehavior_Count);
    return eDetail;
}

static PlayerCreatedMission *s_makeMission(PlayerCreatedStoryArc *arc)
{
    PlayerCreatedMission *pMission = StructAllocRaw(sizeof(PlayerCreatedMission));
    int details, i;
    int missionGroupIdx = testRandInt2(eaSize(&gMMVillainGroupList.group));
    MMVillainGroup *group = gMMVillainGroupList.group[missionGroupIdx];
    MapLimit limit = {0};
    const MapLimit *tempLimit;
    pMission->fTimeToComplete = MAX_MISSION_TIME*(testRandInt2(100)/(F32)100);
    pMission->ePacing = testRandInt2(kPacing_Count);
    pMission->iVillainSet = 0;
    pMission->pchVillainGroupName = group->pchName;
    pMission->iMinLevel = group->minLevel;
    pMission->iMaxLevel = group->maxLevel;
    pMission->pParentArc = arc;
    pMission->ppDetail = 0;
    pMission->eMapLength =(testRandInt2(4)+1)*kMapLength_Tiny;
    tempLimit = s_makeMap(&pMission->pchMapName, &pMission->pchMapSet, pMission->eMapLength);
    if(!tempLimit)
    {
        StructFree(pMission);
        return NULL;
    }
    memcpy(&limit, tempLimit, sizeof(MapLimit));
    
    

    //do text.
    s_fillOutFields( ParsePlayerCreatedMission, pMission, &playerCreatedStoryArc_getMissionFieldCharLimit, 0);

    details = randInt(15)+1;

    for( i = 0; i < details; i++ )
    {

        PlayerCreatedDetail *pDetail = s_makeDetail(&pMission->iMinLevel, &pMission->iMaxLevel, &limit);
        if(!pDetail)
            continue;
        if( i < tempLimit->regular_only[0] + tempLimit->around_or_regular[0] )
        {
            eaPush(&pMission->ppDetail, pDetail);
            if( pDetail->eDetailType != kDetail_Ambush &&    pDetail->eDetailType != kDetail_Patrol )
                eaPush(&pMission->ppMissionCompleteDetails, pDetail->pchName);
        }
        else
            i+=0;
        
        //lets see if we can get away with just this.
    }

    /*s_makeRandomString(&pMission->pchTitle, 75);
    s_makeRandomString(&pMission->pchSubTitle, 75);
    char *pchSubTitle;

    char *pchClueName;
    char *pchClueDesc;

    U32 uID;

    int iMinLevel;
    int iMaxLevel;

    char *pchMapName;    // if mapname is random or not set, use the set and length to determine the misison
    char *pchMapSet; // Type of map to use barring actual file
    char *pchUniqueMapCat; // just saved for the UI
    int eMapLength;    // In Minutes

    char *pchVillainGroupName;
    int iVillainSet;
    PacingType ePacing;

    F32 fTimeToComplete;

    char *pchIntroDialog;
    char *pchAcceptDialog;
    char *pchDeclineDialog;
    char *pchSendOff;
    char *pchDeclineSendOff;
    char *pchActiveTask;
    char *pchEnterMissionDialog;
    char *pchStillBusyDialog;

    char *pchTaskSuccess;
    char *pchTaskFailed;

    char *pchTaskReturnSuccess;
    char *pchTaskReturnFail;

    char **ppMissionCompleteDetails;
    char **ppMissionFailDetails;*/

    return pMission;
}

PlayerCreatedStoryArc * missionMakerTestCreateRandomArc(PlayerCreatedStoryArc *pArc)
{
    static int loaded = 0;
    //PlayerCreatedStoryArc * pArc = StructAllocRaw(sizeof(PlayerCreatedStoryArc));
//    int i;
//    int missions;
//    int filesize;
    if(!pArc)
        pArc = StructAllocRaw(sizeof(PlayerCreatedStoryArc));

    if(!testMissionServerLoadedVillains || VG_MAX == 0)
    {
        villainReadDefFiles();
        playerCreatedStoryarc_LoadData(0);
        PNPCPreload();
        MissionMapPreload();

        testMissionServerLoadedVillains++;
    }

    pArc->eContactType = testRandInt2(kContactType_Count);
    pArc->eMorality = testRandInt2(kMorality_Count);
    pArc->writer_dbid = playerPtr()->db_id;
    //pArc->pchName = playerPtr()->auth_name; 
    pArc->uID = 0;

    /*s_makeRandomString(&pArc->pchName, MISSIONSEARCH_TITLELEN);


    s_makeRandomString(&pArc->pchDescription, MISSIONSEARCH_SUMMARYLEN);


    s_makeRandomString(&pArc->pchSouvenirClueName, 75);
    

    s_makeRandomString(&pArc->pchSouvenirClueDesc, 3000);


    s_makeRandomString(&pArc->pchContactDisplayName, 32);*/
    s_fillOutFields( ParsePlayerStoryArc, pArc, &playerCreatedStoryArc_getArcFieldCharLimit, 0);


    //for now we always choose a typical contact.  Always costume idx 0.
    pArc->eContactType = kContactType_Critter;
    pArc->pchContactCategory = "Arachnos";
    pArc->pchContactCategory = "Arachnos_Arachnobot";
    //s_makeRandomContact(&pArc->pchContactCategory, &pArc->pchContact);



    //missions = testRandInt2(MISSIONSEARCH_MAX_MISSIONS)+1;

    //for( i = 0; i <missions; i++ )
    while( StructGetMemoryUsage(ParsePlayerStoryArc, pArc, 0 ) < 10000.f)
    {
        eaPush(&pArc->ppMissions, s_makeMission(pArc));
    }

    return pArc;

};
static void clearArcForDeletion(PlayerCreatedStoryArc *pArc)
{
    if(!pArc)
        return;

    while(eaSize(&pArc->ppMissions))
    {
        PlayerCreatedMission *mission = eaPop(&pArc->ppMissions);
        while(mission &&eaSize(&mission->ppDetail))
        {
            PlayerCreatedDetail *pDetail = eaPop(&mission->ppDetail);
            free(pDetail);
        }
        free(mission);
    }
}

void missionMakerTestPublish()
{
    PlayerCreatedStoryArc *pArc = StructAllocRaw(sizeof(PlayerCreatedStoryArc));
    char *estr =0;
    estrCreate(&estr);
    ParserWriteTextEscaped(&estr, ParsePlayerStoryArc, missionMakerTestCreateRandomArc(pArc), 0, 0);
    missionserver_game_publishArc(0, estr);
    estrDestroy(&estr);
    {
        int i;
        for(i = 0; i < eaSize(&pArc->ppMissions); i++)
        {
            int j;
            for(j = 0; j < eaSize(&pArc->ppMissions[i]->ppDetail); j++)
                StructFree(pArc->ppMissions[i]->ppDetail[j]);
            eaDestroy(&pArc->ppMissions[i]->ppDetail);
            StructFree(pArc->ppMissions[i]);
        }
        eaDestroy(&pArc->ppMissions);
    }
    StructFree(pArc);
}



static int s_oddTest = 0;
void testMissionSearchTestPublish()
{
    static U32 history_timer = 0;
    static int ticksPerSecond = 0; 


    if(!history_timer)
    {
        history_timer = timerAlloc();
    }
    
    missionMakerTestPublish();
    ticksPerSecond++;
    if(timerElapsed(history_timer) > PPS_REPORT_TIME)
    {
        F32 pubsPerSec = ticksPerSecond/timerElapsed(history_timer);
        timerStart(history_timer);
        
        log_printf("MissionServerTest.txt", "<missionSearchTestPublish publishes per second> %f", pubsPerSec);
        ticksPerSecond = 0;
    }
    //if(s_oddTest%2)
    //    missionserver_game_unpublishArc(s_oddTest/2);
    s_oddTest++;
}

static int s_currentSearchPage = 0;
#define testGetRandomBits(maxOptions) testRandInt2((1 << maxOptions))
#define TEST_MAX_RANDOM_SEARCH_LENGTH 5
MissionSearchParams *MissionMakerTestCreateRandomSearch(MissionSearchParams *search)
{
    //int i;
    unsigned int fieldLength = 2+testRandInt2(TEST_MAX_RANDOM_SEARCH_LENGTH);
    MissionSearchHeader *header;
    static char buffer[512] = {0};
    int categoriesToSearch = testRandInt2(4);
    if(!search)
        search = StructAllocRaw(sizeof(MissionSearchParams));

    if(!testMissionServerLoadedVillains || VG_MAX == 0)
    {
        villainReadDefFiles();
        playerCreatedStoryarc_LoadData(0);
        PNPCPreload();
        MissionMapPreload();
        testMissionServerLoadedVillains++;
    }

    header = s_getHeader();
    search->arcid = 0;
    search->text = buffer;
    search->text[0] = 0;
    {
        int length = strlen(header->summary);
        int start =testRandInt2(MAX(length - fieldLength,0));
        strcat_s(search->text, 512, &header->summary[start]);
        search->text[fieldLength] = ' ';
        search->text[fieldLength+1] = 0;
    }
    search->morality = (categoriesToSearch%2)?header->morality:0;
    search->arcLength = (!(categoriesToSearch%2))?header->arcLength:0;
    //search->villainGroups = (!categoriesToSearch)?header->villaingroups:0;
    if(categoriesToSearch==1)
        strcat(search->text, header->title);

    search->text[fieldLength + 10] = 0;    //at most, ten more characters from title.

    return search;
}




/*void testMissionSearchSearch()
{
    static U32 history_timer = 0;
    static int ticksPerSecond = 0; 


    if(!history_timer)
    {
        history_timer = timerAlloc();
    }

    missionMakerTestSearch(MISSIONSEARCH_ALL);
    
    ticksPerSecond++;
    if(timerElapsed(history_timer) > PPS_REPORT_TIME)
    {
        F32 searchesPerSec = ticksPerSecond/timerElapsed(history_timer);
        timerStart(history_timer);

        log_printf("MissionServerTest.txt", "<missionSearchSearch searches per second> %f", searchesPerSec);
        ticksPerSecond = 0;
    }
}*/



int missionserver_isInSearch(MissionSearchTestResult *arc, MissionSearchParams *searchParameters, MissionServerSearchType type)
{
    if(!arc || arc->id == searchParameters->arcid)    //we don't ever want to put a search by arc's arc into the search.  That happens last.
        return 0;
    switch(type)
    {
        xcase    MSSEARCH_ALL_ARC_IDS:
    return 1;
    xcase    MSSEARCH_MORALITY:
    return (!searchParameters->morality) || arc->header.morality&searchParameters->morality;    //fix if we switch arc morality to non bitfield
    xcase    MSSEARCH_BAN:
    return (!searchParameters->ban) || (1<<arc->rating.ban)&searchParameters->ban;
    xcase    MSSEARCH_HONORS:
    return !searchParameters->honors || (1<<arc->rating.honors)&searchParameters->honors;
    xcase    MSSEARCH_LOCALE:
    return !searchParameters->locale_id || searchParameters->locale_id & (1 << arc->header.locale_id);
    xcase    MSSEARCH_RATING:
    return !searchParameters->rating || searchParameters->rating & (1 << arc->rating.rating);
    xcase    MSSEARCH_MISSION_TYPE:
    return !searchParameters->missionTypes || searchParameters->missionTypes&arc->header.missionType;
    xcase    MSSEARCH_ARC_LENGTH:
    return !searchParameters->arcLength || searchParameters->arcLength&arc->header.arcLength;    //fix if we switch arc arclength to non bitfield
    xcase    MSSEARCH_VILLAIN_GROUPS:
    return BitFieldGetFirst(searchParameters->villainGroups, MISSIONSEARCH_VG_BITFIELDSIZE) == -1 || 
        BitFieldAnyAnd( searchParameters->villainGroups, arc->header.villaingroups, MISSIONSEARCH_VG_BITFIELDSIZE);
    xcase    MSSEARCH_TEXT:
    {
        int inSearch = 1;
        char *temp = estrTemp();
        char *n, *s = 0;
        int quote = 0;
        estrPrintCharString(&temp, searchParameters->text);
        n = temp;
        while(n) // what is wrong with using strtok_quoted?
        {
            if(!quote)
                while(n[0] == ' ')
                    n++;
            if(n[0] == '"')
            {
                n++;
                quote = 1;
            }
            s = n;
            while(n[0] && (quote || n[0] != ' ') && n[0] != '"')
                n++;
            if(!n)
                break;
            if(n[0] == '"')
            {
                quote = !quote;
                if(!n[1])
                {
                    n = NULL; 
                    break;
                }
            }
            if(n[0])
                (n++)[0] = '\0';
            else
                n = NULL;
            if(s)
            {
                inSearch &=(strstri(arc->header.title, s) || 
                    strstri(arc->header.summary,s) ||
                    strstri(arc->header.author, s)) ;
            }
        }
        estrDestroy(&temp);
        return !searchParameters->text || inSearch;
    }
    xcase    MSSEARCH_AUTHOR:
    return (!searchParameters->author || strstri(arc->header.author, searchParameters->author))?1:0;
xdefault:
    return 0;
    }
}

void testMissionSearch_forceSearch()
{
    static int author_i = 0;
    static int summary_i = 0;
    static int title_i = 0;
    static int sort_i = 0;
    char *estr = estrTemp();

    s_noValidation = 1;

    if(!author_i && !summary_i && !title_i && !sort_i)
    {
        Strncpyt(s_searchBase.header.author, s_getRandomWord());
        author_i = MIN(strlen(s_searchBase.header.author), 5);
        Strncpyt(s_searchBase.header.title, s_getRandomWord());
        title_i = MIN(strlen(s_searchBase.header.title), 5);
        Strncpyt(s_searchBase.header.summary, s_getRandomWord());
        summary_i = MIN(strlen(s_searchBase.header.summary), 5);
        sort_i = 2;
    }


    s_makeSearchVariation(&s_search,
        author_i,
        summary_i,
        title_i,
        sort_i);

    ParserWriteText(&estr, parse_MissionSearchParams, &s_search, 0, 0);
    missionserver_game_requestSearchPage(MISSIONSEARCH_ALL, estr, 0, 0, 0,0);

    sort_i--;
    if(sort_i < 0)
    {
        sort_i = 2;
        title_i--;
        if(title_i < 0)
        {
            title_i = MIN(strlen(s_searchBase.header.title), 5);
            summary_i--;
            if(summary_i < 0)
            {
                summary_i = MIN(strlen(s_searchBase.header.summary), 5);
                author_i--;
                if(author_i < 0)
                {
                    devassert(0);//should never get here.  We should restart first.
                    author_i = summary_i = title_i = sort_i = 0;    //should force a restart.
                }
            }
        }
    }
}
/*#define GUARANTEED_ARC_IDS_PUBLISH    300
void testMissionSearch_forcePubUnpubRepub()
{
    missionserver_game_unpublishArc();
    missionserver_game_republishArc
}*/

#define GUARANTEED_ARC_IDS    5000
void testMissionSearch_forceVote()
{
    int randomIdx = testRandInt2(GUARANTEED_ARC_IDS)+1;
    testMissionSearch_testVote(randomIdx);
}

void testMissionSearch_registerData(char *arc){};
