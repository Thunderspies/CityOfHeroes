#include <utilitieslib/stdtypes.h>
#include "UI/uiMissionSearch.h"
#include "mission/MissionSearch.h"
#include "mission/missionserver_meta.h"

#include <utilitieslib/assert/assert.h>
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/components/Earray.h>
#include <utilitieslib/components/estring.h>
#include "storyarc/playerCreatedStoryarc.h"
#include "storyarc/playerCreatedStoryarcClient.h"
#include <utilitieslib/language/MessageStoreUtil.h>
#include <utilitieslib/components/StashTable.h>
#include <utilitieslib/utils/textparser.h>
#include <utilitieslib/utils/file.h>
#include <utilitieslib/utils/FolderCache.h>
#include "entity/CustomVillainGroup.h"
#include "entity/CustomVillainGroup_Client.h"
#include "entity/PCC_Critter.h"
#include "entity/PCC_Critter_Client.h"
#include "UI/uiCustomVillainGroupWindow.h"
#include "uiPCCRank.h"
#include "UI/uiMissionReview.h"
#include "uiMissionComment.h"

#include "entity/entity.h"
#include "player/player.h" 
#include "entity/EntPlayer.h"
#include "cmdparse/cmdgame.h"
#include "gameData/BodyPart.h"

#include "gameComm/wdwbase.h"
#include "UI/uiWindows.h"
#include "UI/uiUtil.h"
#include "UI/uiUtilGame.h"
#include "UI/uiTabControl.h"
#include "graphics/ttFontUtil.h"
#include "graphics/textureatlas.h"
#include "UI/sprite/sprite_base.h"
#include "UI/sprite/sprite_text.h"
#include "UI/sprite/sprite_font.h"
#include "UI/uiInput.h"
#include "UI/uiNet.h"
#include "UI/uiScrollBar.h"
#include "UI/uiClipper.h"
#include "UIEdit.h"
#include "UI/uidialog.h"
#include "UI/uiComboBox.h"
#include "UI/uiMissionMaker.h"
#include "UI/uiMissionMakerScrollSet.h"
#include "UI/uiGame.h"
#include "UI/uiPictureBrowser.h"
#include "UI/uiOptions.h"

#include "gameComm/villainDef.h"
#include <utilitieslib/components/bitfield.h>
#include "storyarc/playerCreatedStoryarcValidate.h"
#include "formatter/smf_main.h"
#include "win/input.h"
#include "filter/profanity.h"
#include <utilitieslib/language/AppLocale.h>
#include <utilitieslib/utils/osdependent.h>
#include "clientcomm/clientcomm.h"
#include "entity/character_level.h"
#include <utilitieslib/utils/timing.h>

#define MISSIONSEARCH_STARHEIGHT        18.f
#define MISSIONSEARCH_PANEBUTTON_HT        20

#define MISSIONSEARCH_SEARCHREQUESTS        2
#define MISSIONSEARCH_SEARCHPERIOD            1
#define MISSIONSEARCH_PUBLISHREQUESTS        1
#define MISSIONSEARCH_PUBLISHPERIOD            30
#define MISSIONSEARCH_ARCREQUESTTIME    (5*60)
#define MISSIONSEARCH_BUTTONWD            (140*sc)
//TODO: get a better max length
#define MISSIONSEARCH_MAX_LENGTH 300.0    //I'm assuming that 5 hours is the longest mission

#define MISSIONSEARCHLINE_EMPTY            ((MissionSearchLine*)1)
#define MISSIONSEARCHLINE_NEW            ((MissionSearchLine*)2)

static char * s_ratingNames[] = {"Rated?", "RatedPoor", "RatedOk", "RatedGood", "RatedExcellent", "RatedAwesome" };
static char * s_searchSummary = 0;
static char * s_searchTextRequested = 0;    //this is the exact search requested, not the search sent (since we strip substrings out of the sent version

typedef struct SMFBlock SMFBlock;


// FIXME: do a pass on colors, margin scaling, etc
typedef struct MissionSearchLine
{
    int id;                        // for published arcs
    char *filename;                // for local, unpublished arcs
    __time32_t timestamp;        // for local, unpublished arcs
    PlayerCreatedStoryArc *arc;    // for owned  arcs

    MissionRating myVote;        //how I voted on this arc
    
    MissionSearchInfo rating;
    int total_votes;
    int keyword_votes;
    MissionSearchHeader header;
    U32 date_created;

    int owned;
    int played;
    U32 requested;
    int refs;
    int deleteme;
    int show;
    int failed_validation;
    int saveLocally;
    U32 idMatch:1;
    U32 settingGuestAuthor:1;
    U32 villainMatch:1;
    U32 summaryMatch:1;
    U32 authorMatch:1;
    U32 titleMatch:1;

    char *estrHeader;
    char *estrFull;
    char *formattedTitle;
    char *formattedAuthor;
    char *formattedSummary;
    char *formattedVillains;
    

    SMFBlock *pBlock;
    MissionSearchGuestBio bio;
    
    char *section_header;        // this line is not a story arc, used for special cs searches

} MissionSearchLine;

typedef struct MissionSearchComboItem
{
    int ID;
    char *name;
    int accesslevel;
    char *iconName;
} MissionSearchComboItem;

typedef struct MissionSearchTab
{
    // static
    MissionSearchPage category;
    char *name;
    char *icon;

    // runtime
    MissionSearchLine *selected;
    ScrollBar list_scroll;
    ScrollBar info_scroll;

    // published arcs
    MissionSearchLine **lines;
    char *context; // estring
    int page;
    int pages;
    U32 requested;
    U32 requested_last;
    int loaded;
    int presorted;

    // local arcs, used for the 'My Arcs' tab
    MissionSearchLine **local;
    int local_loaded;
} MissionSearchTab;


typedef struct PageNavigator
{
    int page;                        // current page
    int nPages;
    F32 x,y,z, wd,ht;
    int changed;
} PageNavigator;

typedef struct SearchRequestInfo
{
    MissionSearchPage category;
    char *context;
    int page;
    int playedFilter;
    int levelFilter;
    int arc_author;
} SearchRequestInfo;

typedef enum PublishRequestType
{
    PUBREQ_PUBLISH,
    PUBREQ_UNPUBLISH,
} PublishRequestType;


static int s_turnOffRateLimiter = 0;
static U32 s_searchRecentRequests[MISSIONSEARCH_SEARCHREQUESTS];
static int s_searchWaiting =0;
static SearchRequestInfo s_searchRequest = {0};
static U32 s_publishRecentRequests[MISSIONSEARCH_SEARCHREQUESTS];
static int s_publishWaiting = 0;
static int s_publishArcid = 0;
static char *s_publishArcstr = 0;
static PublishRequestType s_publishType;
//for setting guest author stuff
static SMFBlock *smfAuthorName = 0;
static SMFBlock *smfAuthorImage = 0;
static SMFBlock *smfAuthorBio = 0;

static SMFBlock *smfSearchDescription = 0;
void updateMissionSearchLine( MissionSearchLine *pLine );

//this is necessary because we want to send the actual characters, not the smf
static void s_prepareArcForSend(PlayerCreatedStoryArc *arc)
{
    StructReallocString(&arc->pchName, smf_DecodeAllCharactersGet(arc->pchName));
    StructReallocString(&arc->pchDescription, smf_DecodeAllCharactersGet(arc->pchDescription));
    StructReallocString(&arc->pchAuthor, smf_DecodeAllCharactersGet(arc->pchAuthor));
}



static void MissionSearch_requestPublishAction(int arcid, char * publishArcstr, PublishRequestType type)
{
    s_publishArcid= arcid;

    if(!s_publishArcstr)
        estrCreate(&s_publishArcstr);
    else
        estrClear(&s_publishArcstr);

    if(type ==PUBREQ_PUBLISH)
        estrConcatCharString(&s_publishArcstr, publishArcstr);
    s_publishType = type;
    s_publishWaiting = 1;
}

void MissionSearch_requestPublish(int arcid, char * publishArcstr)
{
    MissionSearch_requestPublishAction(arcid, publishArcstr, PUBREQ_PUBLISH);
}
int MissionSearch_publishWait()
{
    int i;
    int now;
    int secondsLeft=0;

    now = timerSecondsSince2000();    
    for(i = 0; i <MISSIONSEARCH_PUBLISHREQUESTS; i++)
    {
        int left = MISSIONSEARCH_PUBLISHPERIOD-(now-s_publishRecentRequests[i]);
        if(!secondsLeft)
            secondsLeft = left;
        MIN1(secondsLeft, left);
    }
    MAX1(secondsLeft,0);
    if(!secondsLeft)
        i+=0;
    return secondsLeft;
}

static void MissionSearch_requestSearch(MissionSearchPage category, char * context, int page, int playedFilter, int levelFilter, int arc_author)
{
    s_searchRequest.category= category;

    if(!s_searchRequest.context)
        estrCreate(&s_searchRequest.context);
    else
        estrClear(&s_searchRequest.context);

    estrConcatCharString(&s_searchRequest.context, context);

    s_searchRequest.page = page;
    s_searchRequest.playedFilter = playedFilter;
    s_searchRequest.levelFilter = levelFilter;
    s_searchRequest.arc_author = arc_author;
    s_searchWaiting = 1;
}

static void MissionSearch_searchRequestUpdate()
{
    //only send if we've got something and it's time.
    if(!s_searchWaiting
        || !playerCreatedStoryArc_rateLimiter(s_searchRecentRequests, 
                                    MISSIONSEARCH_SEARCHREQUESTS,
                                    MISSIONSEARCH_SEARCHPERIOD,
                                    timerSecondsSince2000()))
        return;

    missionserver_game_requestSearchPage(s_searchRequest.category, 
                                            s_searchRequest.context, 
                                            s_searchRequest.page, 
                                            s_searchRequest.playedFilter,
                                            s_searchRequest.levelFilter,
                                            s_searchRequest.arc_author );
    s_searchWaiting = 0;
}

void MissionSearch_publishRequestUpdate()
{
    //only send if we've got something and it's time.
    if(!s_publishWaiting 
        || !playerCreatedStoryArc_rateLimiter(s_publishRecentRequests, 
                                    MISSIONSEARCH_PUBLISHREQUESTS,
                                    MISSIONSEARCH_PUBLISHPERIOD,
                                    timerSecondsSince2000()))
        return;

    if(s_publishType == PUBREQ_PUBLISH)
    {
        missionserver_game_publishArc(s_publishArcid, s_publishArcstr);
        MissionSearch_requestSearch(MISSIONSEARCH_MYARCS, 0, 0, 0, 0, 0);
    }
    else if(s_publishArcid)
    {
        missionserver_game_unpublishArc(s_publishArcid);
    }
    s_publishWaiting = 0;
}

int MissionSearch_pageNavInit(PageNavigator *p, F32 x, F32 y, F32 wd, F32 ht)
{
    if(!p || wd <=0 || ht <=0)
        return 0;

    p->page = p->nPages = 0;
    p->x = x;
    p->y = y;
    p->wd = wd;
    p->ht = ht;
    p->changed = 0;
    return 1;
}

int MissionSearch_pageNavReset(PageNavigator *p, int pages)
{
    if(!p )//|| pages == p->nPages)
        return 0;

    p->nPages = pages;
    p->changed = 1;
    return 1;
}

void MissionSearch_pageNavPosition(PageNavigator *p, F32 x, F32 y, F32 z)
{
    p->x = x;
    p->y = y;
    p->z = z;
}

#define MSSEARCH_PNAV_MAXPAGEDISPLAY 30
F32 MissionSearch_pageNavNumber(int number, F32 x, F32 y, F32 z, CBox *box, F32 sc, F32 wd, F32 ht, int selected)
{
    
    if(selected)
        font_color(CLR_MM_TITLE_OPEN, CLR_MM_TITLE_OPEN);
    else
        font_color(CLR_MM_TITLE_OPEN&0xffffff88, CLR_MM_TITLE_OPEN&0xffffff88);

    cprntEx( x + PIX3*sc, y+ht+2*PIX3, z, sc, sc, 0, "%d", number );

    BuildCBox( box, x, y, wd, ht+2*PIX3);
    return x +wd;
}

#define PAGENAV_PREVTEXT "MissionSearchPrevious"
#define PAGENAV_NEXTTEXT "MissionSearchNext"
#define PAGENAV_PRECALCWIDTHS (13)

// FIRST X-100 X-50 X-10 X-3 X-2 X-1 X X+1 X+2 X+3 X+10 X+50 X+100 LAST
int page_offset[] = { -100, -50, -10, -3, -2, -1, 0, 1, 2, 3, 10, 50, 100 };

int MissionSearch_pageNavDisplay(PageNavigator *p, F32 sc)
{
    static CBox box;
    F32 x = p->x;
    F32 charWidths[PAGENAV_PRECALCWIDTHS];    //this is the most character widths we'll ever need.
    F32 totalCharWidth = 0;
    F32 totalDisplayWidth = 0;
    int i, new_page = p->page;
    if(!p || p->nPages < 2)
        return 0;

     p->changed = 0; 

    font(&game_12);
    font_color(CLR_MM_TITLE_OPEN&0xffffff88, CLR_MM_TITLE_OPEN&0xffffff88);

    //figure out which widths to precalculate
    for(i = 0; i < PAGENAV_PRECALCWIDTHS; i++)
    {
        charWidths[i] = 0;
        if( p->page + page_offset[i] >= 0 && p->page + page_offset[i] < p->nPages )
        {
            charWidths[i] = str_wd(&game_12, sc, sc, "%d", ( p->page + page_offset[i]+1) ) + 2*PIX3;
        }
        totalDisplayWidth+=charWidths[i];
    }
    

    //estimate the number of pages we can show on the screen
    totalDisplayWidth += str_wd(&game_12, sc, sc, "MissionSearchPages") +2*PIX3;

     if( p->page >= 4 )
        totalDisplayWidth += (str_wd(&game_12, sc, sc, PAGENAV_PREVTEXT) +2*PIX3);
    if( p->nPages - p->page > 4 )
        totalDisplayWidth += (str_wd(&game_12, sc, sc, PAGENAV_NEXTTEXT) +2*PIX3);

    x += (p->wd-totalDisplayWidth)*.5;
    cprntEx( x , p->y+p->ht+ 2*PIX3, p->z, sc, sc, 0, "MissionSearchPages" );
    x += str_wd(&game_12, sc, sc, "MissionSearchPages") +2*PIX3;

    if(p->page >= 4) 
    {
        F32 width;
        cprntEx( x , p->y+p->ht+ 2*PIX3, p->z, sc, sc, 0, PAGENAV_PREVTEXT );
        width = str_wd(&game_12, sc, sc, PAGENAV_PREVTEXT) +2*PIX3;
        BuildCBox( &box, x, p->y, width+ 2*PIX3*sc, p->ht+2*PIX3);
        x += width;
        if( mouseClickHit(&box,MS_LEFT))
        {
            new_page = 0;
            p->changed = 1;
        }
        if( mouseCollision(&box) )
        {
            drawBox(&box, p->z, 0, 0xffffff11 );
        }
    }

    
    for(i = 0; i < ARRAY_SIZE(page_offset); i++)
    {
        if( charWidths[i] )
        {
             x = MissionSearch_pageNavNumber(p->page + page_offset[i] + 1, x, p->y, p->z, &box, sc, charWidths[i], p->ht, page_offset[i] == 0);
            if( mouseClickHit(&box,MS_LEFT))
            {
                new_page += page_offset[i];
                p->changed = 1;
            }
            if( mouseCollision(&box) )
            {
                drawBox(&box, p->z, 0, 0xffffff11 );
            }
        }
    }

    if( p->nPages - p->page >= 4 )
    {
        F32 width;
        cprntEx( x , p->y+p->ht+ 2*PIX3, p->z, sc, sc, 0, PAGENAV_NEXTTEXT );
        width= str_wd(&game_12, sc, sc, PAGENAV_NEXTTEXT) +2*PIX3;
        BuildCBox( &box, x, p->y, width+ 2*PIX3*sc, p->ht+2*PIX3);
        x+=width;

        if( mouseClickHit(&box,MS_LEFT))
        {
            new_page = p->nPages-1;
            p->changed = 1;
        }
        if( mouseCollision(&box) )
        {
            drawBox(&box, p->z, 0, 0xffffff11 );
        }
    }

    p->page = new_page;
    return p->changed;
}

static PageNavigator s_pNav = {0};

static MissionSearchTab s_categoriesList[] = {
    { MISSIONSEARCH_ALL,        "MissionSearchSearch",     "BrowsePlayGlobe"    },
    { MISSIONSEARCH_MYARCS,        "MissionSearchMyArcs",     "MyArcsPen"    },
};

static MissionSearchComboItem s_alignmentList[] = {
    { kMorality_Evil,        "MissionSearchEvil"        },
    { kMorality_Rogue,        "MissionSearchRogue"        },
    { kMorality_Neutral,    "MissionSearchNeutral"    },
    { kMorality_Vigilante,    "MissionSearchVigilante"        },
    { kMorality_Good,        "MissionSearchGood"    },    
};

static MissionSearchComboItem s_playedFilterList[] = {
    { kMissionSearchViewedFilter_All,                ""    },
    { kMissionSearchViewedFilter_Played_Show,        "MissionSearchFilterPlayedShow"    },
    { kMissionSearchViewedFilter_Played_NoShow,        "MissionSearchFilterPlayed"    },
    { kMissionSearchViewedFilter_Voted_Show,        "MissionSearchFilterVotedShow"    },
    { kMissionSearchViewedFilter_Voted_NoShow,        "MissionSearchFilterVoted"    },
    { kMissionSearchViewedFilter_Level,                "MissionSearchFilterLevel"    },
    { kMissionSearchViewedFilter_Random,            "MissionSearchRandom"},
};
static MissionSearchViewedFilter s_viewedFilter;
static MissionSearchViewedFilter s_currentPlayOption, s_currentVotedOption;
static char *s_searchByAuthor;
static U32 s_keywordsOpen, s_keywords;

#define MISSIONSEARCH_HONORSOFFSET    256
#define MISSIONSEARCH_BANOFFSET            (MISSIONSEARCH_HONORSOFFSET+256)
#define UI_MSSEARCH_COMBO_W 100
STATIC_ASSERT(MISSIONHONORS_TYPES < 256);
STATIC_ASSERT(MISSIONBAN_TYPES < 256);
static MissionSearchComboItem s_ratingList[] = {
    { MISSIONSEARCH_BANOFFSET + MISSIONBAN_NONE,                    "MissionSearchNeverPulled",    1    },
    { MISSIONSEARCH_BANOFFSET + MISSIONBAN_PULLED,                    "MissionSearchPulledOnce",    1    },
    { MISSIONSEARCH_BANOFFSET + MISSIONBAN_SELFREVIEWED,            "MissionSearchSelfReviewed",1    },
    { MISSIONSEARCH_BANOFFSET + MISSIONBAN_SELFREVIEWED_PULLED,        "MissionSearchPulled",        1    },
    { MISSIONSEARCH_BANOFFSET + MISSIONBAN_SELFREVIEWED_SUBMITTED,    "MissionSearchAppealling",    1    },
    { MISSIONSEARCH_BANOFFSET + MISSIONBAN_CSREVIEWED_BANNED,        "MissionSearchBanned",        1    },
    { MISSIONSEARCH_BANOFFSET + MISSIONBAN_CSREVIEWED_APPROVED,        "MissionSearchApproved",    1    },
    { MISSIONSEARCH_HONORSOFFSET + MISSIONHONORS_DC_AND_HOF,        "MissionSearchDCAndHoF",    1    },
    { MISSIONSEARCH_HONORSOFFSET + MISSIONHONORS_FORMER_DC,            "MissionSearchFormerDev",    10    },
    { MISSIONSEARCH_HONORSOFFSET + MISSIONHONORS_FORMER_DC_HOF,        "MissionSearchHOF_formerDev",10    },
    { MISSIONSEARCH_HONORSOFFSET + MISSIONHONORS_DEVCHOICE,            "MissionSearchDevChoice"        },
    { MISSIONSEARCH_HONORSOFFSET + MISSIONHONORS_CELEBRITY,            "MissionSearchCelebrity",        },
    { MISSIONSEARCH_HONORSOFFSET + MISSIONHONORS_HALLOFFAME,        "MissionSearchHallofFame"        },
    { MISSIONSEARCH_HONORSOFFSET + MISSIONHONORS_NONE,                "MissionSearchNoHonors",    1    },
    { MISSIONRATING_5_STARS,                                        "MissionSearchFiveStars"        },
    { MISSIONRATING_4_STARS,                                        "MissionSearchFourStars"        },
    { MISSIONRATING_3_STARS,                                        "MissionSearchThreeStars"        },
    { MISSIONRATING_2_STARS,                                        "MissionSearchTwoStars"            },
    { MISSIONRATING_1_STAR,                                            "MissionSearchOneStar"            },
    { MISSIONRATING_0_STARS,                                        "MissionSearchNoRating"            },
};
STATIC_ASSERT(ARRAY_SIZE(s_ratingList) == MISSIONRATING_VOTETYPES+MISSIONHONORS_TYPES+MISSIONBAN_TYPES);

static MissionSearchComboItem s_lengthList[] = {
    { kArcLength_VeryShort,    "MissionSearchVeryShort"},
    { kArcLength_Short,        "MissionSearchShort"    },
    { kArcLength_Medium,    "MissionSearchMedium"    },
    { kArcLength_Long,        "MissionSearchLong"        },
    { kArcLength_VeryLong,    "MissionSearchVeryLong"    },
};

static MissionSearchComboItem s_localeList[] = {
    { LOCALE_ID_ENGLISH,    "MissionSearchUSUK",    0,    "USUK_Flag"    },
    { LOCALE_ID_FRENCH,        "MissionSearchFrench",    0,    "France_Flag"    },
    { LOCALE_ID_GERMAN,        "MissionSearchGerman",    0,    "German_Flag"    },
};

static MissionSearchComboItem s_statusList[] = {
    { kArcStatus_WorkInProgress,        "PCS_InProgress"},
    { kArcStatus_LookingForFeedback,    "PCS_LFFeedback"},
    { kArcStatus_Final,                    "PCS_Final"},
};

typedef enum MissionSearchSort
{
    kMissionSearchSort_Name = 1,
    kMissionSearchSort_Author,
    kMissionSearchSort_ArcID,
    kMissionSearchSort_Rating,
    kMissionSearchSort_VG,
    kMissionSearchSort_Length,
    kMissionSearchSort_Detail,
    kMissionSearchSort_Morality,
    kMissionSearchSort_Date,
}MissionSearchSort;

static TextAttribs s_taMissionSearchInput =
{
    /* piBold        */  (int *)0,
    /* piItalic      */  (int *)1,
    /* piColor       */  (int *)CLR_MM_TEXT,
    /* piColor2      */  (int *)0,
    /* piColorHover  */  (int *)CLR_MM_TEXT,
    /* piColorSelect */  (int *)0,
    /* piColorSelectBG*/ (int *)0x333333ff,
    /* piScale       */  (int *)(int)(1.0f*SMF_FONT_SCALE),
    /* piFace        */  (int *)&smfSmall,
    /* piFont        */  (int *)0,
    /* piAnchor      */  (int *)0,
    /* piLink        */  (int *)0x80e080ff,
    /* piLinkBG      */  (int *)0,
    /* piLinkHover   */  (int *)0x66ff66ff,
    /* piLinkHoverBG */  (int *)0,
    /* piLinkSelect  */  (int *)0,
    /* piLinkSelectBG*/  (int *)0x666666ff,
    /* piOutline     */  (int *)0,
    /* piShadow      */  (int *)0,
};

static TextAttribs s_taMissionSearch =
{
    /* piBold        */  (int *)0,
    /* piItalic      */  (int *)0,
    /* piColor       */  (int *)0xffffffff,
    /* piColor2      */  (int *)0,
    /* piColorHover  */  (int *)0xffffffff,
    /* piColorSelect */  (int *)0,
    /* piColorSelectBG*/ (int *)0x333333ff,
    /* piScale       */  (int *)(int)(1.0f*SMF_FONT_SCALE),
    /* piFace        */  (int *)&smfSmall,
    /* piFont        */  (int *)0,
    /* piAnchor      */  (int *)0,
    /* piLink        */  (int *)0x80e080ff,
    /* piLinkBG      */  (int *)0,
    /* piLinkHover   */  (int *)0x66ff66ff,
    /* piLinkHoverBG */  (int *)0,
    /* piLinkSelect  */  (int *)0,
    /* piLinkSelectBG*/  (int *)0x666666ff,
    /* piOutline     */  (int *)1,
    /* piShadow      */  (int *)0,
};


static UIEdit *s_textsearch = NULL;
static ComboBox s_categoryCombo = {0};
static int s_categoryCurrent = 0;
static ComboBox s_alignmentCombo = {0};
static ComboBox s_ratingCombo = {0};
static ComboBox s_lengthCombo = {0};
static ComboBox s_localeCombo = {0};
static ComboBox s_countCombo = {0};
static ComboBox s_enemyCombo = {0};
static ComboBox s_statusCombo = {0};

static int s_updateRefine = 1;
static int s_sort_idx = -kMissionSearchSort_Rating;

static MissionSearchTab *missionsearch_GetTabByCategory( int category )
{
    int i;

    if(category == MISSIONSEARCH_NOPAGE)
        return NULL;

    for( i = ARRAY_SIZE(s_categoriesList)-1; i>=0; i-- )
    {
        if( s_categoriesList[i].category == category )
            return &s_categoriesList[i];
    }

    assertmsg(category != MISSIONSEARCH_ALL, "no 'all' missionsearch page defined");
    return missionsearch_GetTabByCategory(MISSIONSEARCH_ALL); // default to 'All'
}

void missionsearch_MakeMission(void *notused)
{
    if( ArchitectCanEdit(playerPtr()) )
    {
        int i;
        requestArchitectInventory();
        dialogRemove( 0, missionMakerDialogDummyFunction, 0 );

        window_setMode(WDW_MISSIONSEARCH, WINDOW_GROWING);
        for( i = ARRAY_SIZE(s_categoriesList)-1; i>=0; i-- )
        {
            if( s_categoriesList[i].category == MISSIONSEARCH_MYARCS )
                s_categoryCurrent = i;
        }
    }
}

void missionsearch_PlayMission(void *notused)
{
    if( ArchitectCanEdit(playerPtr()) )
    {
        requestArchitectInventory();
        dialogRemove( 0, missionMakerDialogDummyFunction, 0 );
        window_setMode(WDW_MISSIONSEARCH, WINDOW_GROWING);
        //s_categoryCurrent = 0;
    }
    else
    {
        dialogStd( DIALOG_OK, textStd("ArchitectNoAccess"), 0, 0, missionMakerDialogDummyFunction, NULL, DLGFLAG_ARCHITECT );
    }
}

void missionsearch_IntroChoice()
{
    dialogStd( DIALOG_SMF, textStd("ArchitectIntro"), 0, 0, missionMakerDialogDummyFunction, NULL, DLGFLAG_ARCHITECT );
}

MissionSearchParams s_MissionSearch = {0};
static SMFBlock *smfSearch = 0;

void missionsearch_FindArcIds(char *text, int *arcid)
{
    char *temp;
    char *position;
    int tempId = 0;
    if(!text || !arcid)
        return;

    temp = estrTemp();
    estrPrintCharString(&temp, text);
    position = strtok(temp, " ,.-");
    while(position)
    {
        tempId = atoi(position);
        //Todo: accept a list of ids.
        if(tempId)
            *arcid = tempId;
        position = strtok(NULL, " ,.-");
    }
    estrDestroy(&temp);
}

void missionsearch_FindVillainNames(char *text, U32 *vBits)
{
    int i;

    int seen = 0;
    if(!text || !vBits)
        return;

    for(i = 0; i < VG_MAX; i++)
    {
        char *vgroup = textStd(villainGroupGetPrintName(i));
        char *beginStrstr = strstri(text, vgroup);
        if(beginStrstr)
        {
            BitFieldSet( vBits, MISSIONSEARCH_VG_BITFIELDSIZE, i, 1 );
        }
    }
}

void missionsearch_UpdateSearchParams(int clear)
{
    int i;
    int filtering = 0;
    int defaultLocale = locGetIDInRegistry();

     memset(&s_MissionSearch, 0, sizeof(MissionSearchParams));

    //get the search summary
    if(!s_searchSummary)
        estrCreate(&s_searchSummary);
    else
        estrClear(&s_searchSummary);

    estrConcatf(&s_searchSummary, "%s ", textStd("MissionSearchFilter"));
    if(clear)
    {
        smf_SetRawText(smfSearch, "", false);
        estrClear(&s_MissionSearch.text);// = smfSearch.outputString;
    }
    if(smfSearch->outputString && strlen(smfSearch->outputString))
    {
        filtering++;
        estrConcatf(&s_searchSummary, "%s %s; ", textStd("MissionSearchFilterText"), smf_EncodeAllCharactersGet(smfSearch->outputString));
    }

    if(smfSearch->outputString && strlen(smfSearch->outputString))
    {
        if(clear)
            s_MissionSearch.arcid = 0;
        else
        {
            missionsearch_FindArcIds(smfSearch->outputString, &s_MissionSearch.arcid);
            if(s_MissionSearch.arcid)
            {
                filtering++;
                estrConcatf(&s_searchSummary, "%s %d; ", textStd("MissionSearchID"), s_MissionSearch.arcid);
            }
        }
    }
    if( smfSearch->outputString && strlen(smfSearch->outputString) )
    {
        estrPrintEString(&s_MissionSearch.text, smf_DecodeAllCharactersGet(smfSearch->outputString));
        if(!s_searchTextRequested)
            estrCreate(&s_searchTextRequested);
        estrPrintEString(&s_searchTextRequested, s_MissionSearch.text);
        missionsearch_formatSearchString(&s_MissionSearch.text, MSSEARCH_QUOTED_ALLOWED);
        missionsearch_FindVillainNames(smfSearch->outputString,s_MissionSearch.villainGroups);
    }

    //RATING
    if( !SAFE_MEMBER(s_ratingCombo.elements[0],selected ) )
    {
        if(clear)
        {
            for( i = eaSize(&s_ratingCombo.elements)-1; i > 0; i-- )
            {
                s_ratingCombo.elements[i]->selected =0;
            }
            s_ratingCombo.elements[0]->selected =1;
        }
        else
        {
            int seen = 0;
            filtering++;
            estrConcatf(&s_searchSummary, "%s ", textStd("MissionSearchFilterRating"));
            for( i = eaSize(&s_ratingCombo.elements)-1; i >= 0; i-- )
            {
                ComboCheckboxElement *elem = s_ratingCombo.elements[i];
                if( elem->selected )
                {
                    int star = 0;
                    if(elem->id >= MISSIONSEARCH_BANOFFSET)
                        s_MissionSearch.ban |= (1 << (s_ratingCombo.elements[i]->id - MISSIONSEARCH_BANOFFSET));
                    else if(elem->id >= MISSIONSEARCH_HONORSOFFSET)
                    {
                        s_MissionSearch.honors |= (1 << (s_ratingCombo.elements[i]->id - MISSIONSEARCH_HONORSOFFSET));
                    }
                    else
                    {
                        s_MissionSearch.rating |= (1 << s_ratingCombo.elements[i]->id);
                        star = 1;
                    }

                    if(!star)
                    {
                        if(seen)
                            estrConcatStaticCharArray(&s_searchSummary, ", ");
                        estrConcatCharString(&s_searchSummary, textStd(s_ratingCombo.elements[i]->txt));
                        seen++;
                    }
                }
            }
            estrConcatStaticCharArray(&s_searchSummary, "; ");
            //FIXUP FOR LOGICAL RESULTS FROM MISSION SERVER
            if( s_MissionSearch.honors & (1 << MISSIONHONORS_DEVCHOICE) || s_MissionSearch.honors & (1 << MISSIONHONORS_HALLOFFAME ) ) 
                s_MissionSearch.honors |= (1 << MISSIONHONORS_DC_AND_HOF);
            if(s_MissionSearch.honors & ~(1 << MISSIONHONORS_FORMER_DC))
                s_MissionSearch.rating |= (1 << MISSIONRATING_MAX_STARS_PROMOTED);
            if(s_MissionSearch.rating)
                s_MissionSearch.honors |= (1 << MISSIONHONORS_NONE);

            //FIXUP FOR FORMER DEV CHOICE
            if(s_MissionSearch.honors &(1 << MISSIONHONORS_FORMER_DC))    //if this is selected before the next two lines, we're doing a search for all former DC
            {
                s_MissionSearch.honors |= (1 << MISSIONHONORS_FORMER_DC_HOF);
            }
            if(s_MissionSearch.honors & (1 << MISSIONHONORS_NONE))        //the client is looking for all unhonored, whether former DCs or not.
                s_MissionSearch.honors |= (1 << MISSIONHONORS_FORMER_DC);
            if(s_MissionSearch.honors & (1 << MISSIONHONORS_HALLOFFAME))//The client is looking for all hall of fames, whether former DCs or not.
                s_MissionSearch.honors |= (1 << MISSIONHONORS_FORMER_DC_HOF);
        }
    }

    if(s_MissionSearch.rating)
    {
        if(clear)
            s_MissionSearch.rating = 0;
        else
        {
            int starSeen = 0;
            filtering++;
            estrConcatf(&s_searchSummary, "%s ", textStd("MissionSearchFilterStars"));
            for(i = 1; i < MISSIONRATING_VOTETYPES; i++)
            {
                if(s_MissionSearch.rating&(1<<i))
                {
                    if(starSeen)
                        estrConcatStaticCharArray(&s_searchSummary, ", ");
                    estrConcatf(&s_searchSummary, "%d", i);
                    starSeen++;
                }
            }
            estrConcatStaticCharArray(&s_searchSummary, "; ");
        }
    }

    //LENGTH
    if( !SAFE_MEMBER(s_lengthCombo.elements[0],selected ))    //hack because we have another element with id 0, unfortunately.
    {
        if(clear)
        {
            for( i = eaSize(&s_lengthCombo.elements)-1; i > 0; i-- )
            {
                s_lengthCombo.elements[i]->selected =0;
            }
            s_lengthCombo.elements[0]->selected =1;
        }
        else
        {
            int seen = 0;
            filtering++;
            estrConcatf(&s_searchSummary, "%s ", textStd("MissionSearchFilterLength"));
            for( i = eaSize(&s_lengthCombo.elements)-1; i >= 0; i-- )
            {
                if(  s_lengthCombo.elements[i]->selected )
                {
                    s_MissionSearch.arcLength |= (1<<s_lengthCombo.elements[i]->id);

                    if(seen)
                        estrConcatStaticCharArray(&s_searchSummary, ", ");
                    estrConcatCharString(&s_searchSummary, textStd(s_lengthCombo.elements[i]->txt));
                    seen++;
                }
            }
            estrConcatStaticCharArray(&s_searchSummary, "; ");
        }
    }

    //ALIGNMENT
    if( !SAFE_MEMBER(s_alignmentCombo.elements[0],selected ) )
    {
        if(clear)
        {
            for( i = eaSize(&s_alignmentCombo.elements)-1; i > 0; i-- )
            {
                s_alignmentCombo.elements[i]->selected =0;
            }
            s_alignmentCombo.elements[0]->selected =1;
        }
        else
        {
            int seen = 0;
            filtering++;
            estrConcatf(&s_searchSummary, "%s ", textStd("MissionSearchFilterAlignment"));
            for( i = eaSize(&s_alignmentCombo.elements)-1; i >= 0; i-- )
            {
                if(  s_alignmentCombo.elements[i]->selected )
                {
                    s_MissionSearch.morality |= (1<<s_alignmentCombo.elements[i]->id);

                    if(seen)
                        estrConcatStaticCharArray(&s_searchSummary, ", ");
                    estrConcatCharString(&s_searchSummary, textStd(s_alignmentCombo.elements[i]->txt));
                    seen++;
                }
            }
            estrConcatStaticCharArray(&s_searchSummary, "; ");
        }
    }

    //LOCALE

    if(clear)
    {
        for( i = eaSize(&s_localeCombo.elements)-1; i > 0; i-- )
        {
            s_localeCombo.elements[i]->selected =0;
        }
        combobox_setId( &s_localeCombo, defaultLocale, 1 );
        s_localeCombo.elements[0]->selected =0;
    }
    else
    {
        int isNotDefault = 0;
        int seen = 0;
        for( i = eaSize(&s_localeCombo.elements)-1; i >= 0; i-- )
        {
            if(i==0 || s_localeCombo.elements[i]->id != defaultLocale)
                isNotDefault |= s_localeCombo.elements[i]->selected;
        }
        if(isNotDefault)
        {
            filtering++;
            estrConcatf(&s_searchSummary, "%s ", textStd("MissionSearchFilterLocale"));
            for( i = eaSize(&s_localeCombo.elements)-1; i >= 0; i-- )
            {
                if(  s_localeCombo.elements[i]->selected)
                {
                    if(i == 0)
                        s_MissionSearch.locale_id = 0;
                    else
                        s_MissionSearch.locale_id |= (1<<s_localeCombo.elements[i]->id);

                    if(seen)
                        estrConcatStaticCharArray(&s_searchSummary, ", ");
                    estrConcatCharString(&s_searchSummary, textStd(s_localeCombo.elements[i]->txt));
                    seen++;
                }
            }
            estrConcatStaticCharArray(&s_searchSummary, "; ");
        }
        else
        {
            s_MissionSearch.locale_id |= (1<<defaultLocale);
        }
    }



    //STATUS
    if( !SAFE_MEMBER(s_statusCombo.elements[0],selected ) )
    {
        if(clear)
        {
            for( i = eaSize(&s_statusCombo.elements)-1; i > 0; i-- )
            {
                s_statusCombo.elements[i]->selected =0;
            }
            s_statusCombo.elements[0]->selected =1;
        }
        else
        {
            int seen = 0;
            filtering++;
            estrConcatf(&s_searchSummary, "%s ", textStd("MissionSearchFilterStatus"));
            for( i = eaSize(&s_statusCombo.elements)-1; i >= 0; i-- )
            {
                if(  s_statusCombo.elements[i]->selected )
                {
                    s_MissionSearch.status |= (1<<s_statusCombo.elements[i]->id);

                    if(seen)
                        estrConcatStaticCharArray(&s_searchSummary, ", ");
                    estrConcatCharString(&s_searchSummary, textStd(s_statusCombo.elements[i]->txt));
                    seen++;
                }
            }
            estrConcatStaticCharArray(&s_searchSummary, "; ");
        }
    }


    if(s_viewedFilter) 
    {
        if(clear)
            s_viewedFilter = 0;
        else
        {
            int idx;
            int firstTime = 1;
            U32 filter = s_viewedFilter;
            filtering++;
            idx =BitFieldGetFirst(&filter, 1);
            while(idx != -1)
            {
                if(idx !=0)
                {
                    if(firstTime)
                    {
                        estrConcatf(&s_searchSummary, "%s ", textStd("MissionSearchFilterPlayCategory"));
                        firstTime=0;
                    }
                    else
                        estrConcatStaticCharArray(&s_searchSummary, ", ");
                    estrConcatCharString(&s_searchSummary, textStd(s_playedFilterList[idx].name));
                }
                idx = BitFieldGetNext(&filter, 1, idx);
            }
            if(!firstTime)
                estrConcatStaticCharArray(&s_searchSummary, "; ");
        }
    }

    if(s_keywordsOpen && clear)
        s_keywordsOpen = 0;

    if(s_keywords)
    {
        if(clear)
        {
            s_keywords = 0;
        }
        else if(!s_keywordsOpen)
        {
            s_MissionSearch.keywords = 0;
        }
        else
        {
            int idx;
            int firstTime = 1;
            U32 filter = s_keywords;
            filtering++;
            idx =BitFieldGetFirst(&filter, 1);
            while(idx != -1)
            {
                if(firstTime)
                {
                    estrConcatf(&s_searchSummary, "%s: ", textStd("MissionSearchFilterKeywords"));
                    firstTime=0;
                }
                else
                    estrConcatStaticCharArray(&s_searchSummary, ", ");
                estrConcatCharString(&s_searchSummary, textStd(missionReviewGetKeywordKeyFromNumber( idx)));
                idx = BitFieldGetNext(&filter, 1, idx);
            }
            if(!firstTime)
                estrConcatStaticCharArray(&s_searchSummary, "; ");

            s_MissionSearch.keywords = s_keywords;
        }
    }

       if( s_searchByAuthor )
    {
        filtering++;
        estrConcatf(&s_searchSummary, "%s %s", textStd("MissionSearchFilterAuthor"), s_searchByAuthor);
        SAFE_FREE(s_searchByAuthor);
    }

    if(!filtering || clear)
        estrClear(&s_searchSummary);
    smf_SetRawText(smfSearchDescription, s_searchSummary, false);

}

// Update local list and build search to send to server
#define SEARCH_HIGHLIGHTED_SMF_START "</font><font outline=0 color=Orange>"
#define SEARCH_HIGHLIGHTED_SMF_END "</font><font outline=0 color=architect>"
#define SEARCH_HIGHLIGHTED_ENCODED_START 254
#define SEARCH_HIGHLIGHTED_ENCODED_END 255
#define SEARCH_HIGHLIGHTED_MARK 253

static int s_buildVillainList(U32 *bitfield, char **estr, U32 *highlightBitfield, char *startHighlight, char *stopHighlight)
{
    int bit_cursor = BitFieldGetNext(bitfield, MISSIONSEARCH_VG_BITFIELDSIZE, -1 );
    int first = 1;
    int foundMatch = 0;
    while( bit_cursor >= 0 )
    {
        if( first )
        {
            if(highlightBitfield && BitFieldGet(highlightBitfield, MISSIONSEARCH_VG_BITFIELDSIZE, bit_cursor))
            {
                estrConcatf( estr, "%s%s%s", startHighlight, textStd(villainGroupGetPrintName(bit_cursor)), stopHighlight);
                foundMatch++;
            }
            else
                estrConcatCharString(estr, textStd(villainGroupGetPrintName(bit_cursor)));
            first = 0;
        }
        else
        {
            if(highlightBitfield && BitFieldGet(highlightBitfield, MISSIONSEARCH_VG_BITFIELDSIZE, bit_cursor))
            {
                estrConcatf( estr, ", %s%s%s", startHighlight, textStd(villainGroupGetPrintName(bit_cursor)),stopHighlight);
                foundMatch++;
            }
            else
                estrConcatf( estr, ", %s", textStd(villainGroupGetPrintName(bit_cursor)));
        }

        bit_cursor = BitFieldGetNext(bitfield, MISSIONSEARCH_VG_BITFIELDSIZE, bit_cursor );
    }
    return !!foundMatch;
}
static int s_replaceCharWithWord(char **estr, char charIn, char *wordOut)
{
    char *position = strchr(*estr, charIn);
    int found = !!position;
    /*if(strlen(word)==0)
        return 0;*/
    while(position)
    {
        int currentLetterIndex = (position-*estr);
        int nextLetterIndex = currentLetterIndex+ strlen(wordOut) ;
        estrRemove(estr, currentLetterIndex, 1);
        estrInsert(estr, currentLetterIndex, wordOut, strlen(wordOut));
        
        position = strchr(&((*estr)[nextLetterIndex]), charIn);
    }
    return found;
}

static int s_markHighlightPositions(char *text, char *positions, char *word, char highlight)
{
    char *position = strstri(text, word);
    int found = !!position;
    int i;
    int wordLength = strlen(word);

    if(strlen(word)==0)
        return 0;
    while(position)
    {
        int currentLetterIndex = (position-text);
        int nextLetterIndex = currentLetterIndex+strlen(word);
        for(i = 0; i < wordLength; i++)
        {
            positions[currentLetterIndex+i] = highlight;
        }
        position = strstri(&((text)[nextLetterIndex]), word);
    }
    return found;
}

static void s_highlightWord(char **estr, char * positions, char mark, char *start, char * end)
{
    int length = strlen(positions);
    int i;
    int charactersInserted = 0;
    int startLength = strlen(start);
    int endLength = strlen(end);

    for(i = 0; i < length; i++)
    {
        //hit the first highlight
        if(positions[i] ==mark)
        {
            estrInsert(estr, i+charactersInserted, start, startLength);
            charactersInserted += startLength;
            while(positions[i] ==mark && i < length)
            {
                i++;
                
            }
            estrInsert(estr, i+charactersInserted, end, endLength);
            charactersInserted += endLength;
        }
    }
}

void missionsearch_CheckLineAgainstSearch(MissionSearchLine *line)
{
    U32 tempVGroups[MISSIONSEARCH_VG_BITFIELDSIZE] = {0};
    line->show = 1;
    if(!line->estrFull)
        estrCreate(&line->estrFull); 
    estrClear(&line->estrFull); 
    line->idMatch= line->authorMatch= line->titleMatch= line->villainMatch= line->summaryMatch= 0;


    if( s_MissionSearch.text && (line->header.title || line->header.summary))
    {
        int quote = 0;
        char *n, *s;
        char *temp = estrTemp();
        char start[] = {SEARCH_HIGHLIGHTED_ENCODED_START,0};
        char stop[] = {SEARCH_HIGHLIGHTED_ENCODED_END,0};
        char *tempTitle = estrTemp();
        char *tempAuthor = estrTemp();
        char *tempSummary = estrTemp();
        if(!line->formattedTitle)
            estrCreate(&line->formattedTitle);
        else
            estrClear(&line->formattedTitle); 
        if(!line->formattedAuthor)
            estrCreate(&line->formattedAuthor);
        else
            estrClear(&line->formattedAuthor); 
        if(!line->formattedSummary)
            estrCreate(&line->formattedSummary);
        else
            estrClear(&line->formattedSummary); 
        if(!line->formattedVillains)
            estrCreate(&line->formattedVillains);
        else
            estrClear(&line->formattedVillains); 

        estrPrintEString(&temp, s_searchTextRequested ? s_searchTextRequested : smf_DecodeAllCharactersGet(smfSearch->outputString));
        n = temp; 
        estrPrintEString(&line->formattedAuthor, smf_DecodeAllCharactersGet(line->header.author));
        estrPrintEString(&tempAuthor, line->formattedAuthor);
        estrPrintEString(&line->formattedTitle, smf_DecodeAllCharactersGet(line->header.title));
        estrPrintEString(&tempTitle, line->formattedTitle);
        estrPrintEString(&line->formattedSummary, smf_DecodeAllCharactersGet(line->header.summary));
        estrPrintEString(&tempSummary, line->formattedSummary);
        line->villainMatch = s_buildVillainList(line->header.villaingroups, &line->formattedVillains, s_MissionSearch.villainGroups, start, stop);

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

            if( !(strstri(line->header.title, s) || strstri(line->header.summary, s) || strstri(line->header.author, s) || strstri(line->formattedVillains, s)) )
                line->show = 0;
            else if(s)
            {
                //find all examples of this word in the search and mark them a different color.
                if(line->formattedTitle)
                {
                    if( s_markHighlightPositions(line->formattedTitle, tempTitle, s, SEARCH_HIGHLIGHTED_MARK))
                        line->titleMatch = 1;
                }
                if(line->formattedSummary)
                {
                    if( s_markHighlightPositions(line->formattedSummary, tempSummary, s, SEARCH_HIGHLIGHTED_MARK))
                        line->summaryMatch = 1;
                }
                if(line->formattedAuthor)
                {
                    if( s_markHighlightPositions(line->formattedAuthor, tempAuthor, s, SEARCH_HIGHLIGHTED_MARK))
                        line->authorMatch = 1;
                }
            }
        }
        s_highlightWord(&line->formattedTitle, tempTitle, SEARCH_HIGHLIGHTED_MARK, start, stop);
        s_highlightWord(&line->formattedSummary, tempSummary, SEARCH_HIGHLIGHTED_MARK, SEARCH_HIGHLIGHTED_SMF_START, SEARCH_HIGHLIGHTED_SMF_END);
        s_highlightWord(&line->formattedAuthor, tempAuthor, SEARCH_HIGHLIGHTED_MARK, start, stop);

        //s_replaceCharWithWord(&line->formattedSummary, SEARCH_HIGHLIGHTED_ENCODED_START, SEARCH_HIGHLIGHTED_SMF_START);
        //s_replaceCharWithWord(&line->formattedSummary, SEARCH_HIGHLIGHTED_ENCODED_END, SEARCH_HIGHLIGHTED_SMF_END);
        s_replaceCharWithWord(&line->formattedVillains, SEARCH_HIGHLIGHTED_ENCODED_START, SEARCH_HIGHLIGHTED_SMF_START);
        s_replaceCharWithWord(&line->formattedVillains, SEARCH_HIGHLIGHTED_ENCODED_END, SEARCH_HIGHLIGHTED_SMF_END);

        estrDestroy(&tempTitle);
        estrDestroy(&tempAuthor);
        estrDestroy(&tempSummary);
        estrDestroy(&temp);
    }
    if(BitFieldAnyAnd(s_MissionSearch.villainGroups, line->header.villaingroups, MISSIONSEARCH_VG_BITFIELDSIZE))
    {
        line->show = 1;
    }

    if( s_MissionSearch.morality && 
        !(line->header.morality&s_MissionSearch.morality) )
    {
        line->show = 0;
    }        

    if( s_MissionSearch.rating && 
        !(s_MissionSearch.rating & (1 << line->rating.rating)) )
    {
        line->show = 0;
    }

    if( s_MissionSearch.honors && 
        !(s_MissionSearch.honors & (1 << line->rating.honors)) )
    {
        line->show = 0;
    }

    if( s_MissionSearch.ban && !(s_MissionSearch.ban & (1 << line->rating.ban)) )
    {
        line->show = 0;
        //continue;
    }

    if( s_MissionSearch.arcLength && 
        !BitFieldAnyAnd( &s_MissionSearch.arcLength, &line->header.arcLength, 1))
    {
        line->show = 0;
    }

    if( s_MissionSearch.missionTypes && 
        !BitFieldAnyAnd( &s_MissionSearch.missionTypes, &line->header.missionType, 1))
    {
        line->show = 0;
    }

    if( BitFieldCount( s_MissionSearch.villainGroups , MISSIONSEARCH_VG_BITFIELDSIZE, 1 ) && 
        BitFieldAnyAnd( s_MissionSearch.villainGroups, line->header.villaingroups, MISSIONSEARCH_VG_BITFIELDSIZE) )
    {
        line->show = 1;
    }

    if(s_MissionSearch.arcid &&  line->id == s_MissionSearch.arcid)
    {
        line->show = 1;
        line->idMatch = 1;
    }
}
void missionsearch_RefineSearch(MissionSearchLine **lines)
{
    int i;
    MissionSearchLine *line;
    int size = eaSize(&lines);
    

    for(i = 0; i < size; i++)
    {
        line = lines[i];
        missionsearch_CheckLineAgainstSearch(line);
        updateMissionSearchLine(line);
    }
}

static StashTable s_linecache;

static MissionSearchLine* s_createCachedSearchLine(int id)
{
    MissionSearchLine *line = NULL;

    if(!s_linecache)
        s_linecache = stashTableCreateInt(0);

    if(!id || !stashIntFindPointer(s_linecache, id, &line))
    {
        // FIXME: these are never destroyed
        line = calloc(1, sizeof(*line));
        line->id = id;
        line->rating.rating = MISSIONRATING_UNRATED;
        line->show = 1;
        line->myVote = MISSIONRATING_UNRATED;

        if(id)
            assert(stashIntAddPointer(s_linecache, id, line, false));
    }

    line->refs++;
    return line;
}

static void s_destroyCachedSearchLine(MissionSearchLine *line)
{
    if(!--line->refs)
    {
        if(line->id)
            assert(stashIntRemovePointer(s_linecache, line->id, &line));
        StructClear(parse_MissionSearchHeader, &line->header);
        playerCreatedStoryArc_Destroy(line->arc); // FIXME: this may not be safe if uiMissionMaker needs it...
        estrDestroy(&line->estrHeader);
        estrDestroy(&line->estrFull);
        if(line->formattedAuthor)
            estrDestroy(&line->formattedAuthor);
        if(line->formattedTitle)
            estrDestroy(&line->formattedTitle);
        if(line->formattedSummary)
            estrDestroy(&line->formattedSummary);
        if(line->formattedAuthor)
            estrDestroy(&line->formattedVillains);
        smfBlock_Destroy(line->pBlock);
        SAFE_FREE(line->filename);
        SAFE_FREE(line->section_header);
        SAFE_FREE(line);
    }
    else
    {
        assert(line->refs > 0);
        assert(line->id); // multiple refs to a local arc?
    }
}

static MissionSearchLine* s_findCachedSearchLine(int id)
{
    MissionSearchLine *line;
    if(id && stashIntFindPointer(s_linecache, id, &line))
        return line;
    return NULL;
}

static void updateMissionSearchLine( MissionSearchLine *pLine )
{
    int i, bit_cursor = -1, first=1;
    char * pchText; 

    if(!pLine->estrHeader)
        estrCreate(&pLine->estrHeader);
    if(!pLine->estrFull)
        estrCreate(&pLine->estrFull);
    estrClear(&pLine->estrFull);

    estrClear(&pLine->estrHeader);
    estrClear(&pLine->estrFull);

    if(pLine->header.guestAuthorBio)
    {
        StructClear(parse_MissionSearchGuestBio,&pLine->bio);
        if(!ParserReadText(pLine->header.guestAuthorBio,-1,parse_MissionSearchGuestBio,&pLine->bio))
        {
            pLine->bio.bio = pLine->header.guestAuthorBio;
            pLine->bio.imageName = 0;
        }
        estrConcatf( &pLine->estrHeader, "<font outline=1 color=ArchitectTitle>%s </font>", textStd("MissionSearchGuestAuthorBio"));
        estrConcatf(&pLine->estrHeader, "<font outline=0 color=Architect>%s</font>", pLine->bio.bio);
    }

    if( pLine->id )
    {
        //you rated
        if(pLine->myVote != MISSIONRATING_UNRATED && pLine->myVote < MISSIONRATING_MAX_STARS && pLine->myVote >= MISSIONRATING_1_STAR)
        {
            estrConcatf( &pLine->estrFull, "<font outline=1 color=ArchitectTitle>%s </font>", textStd("MissionSearchYouRated"));
            for( i = 0; i < 5; i++ )
            {
                if( i >= (int)pLine->myVote )
                    estrConcatStaticCharArray(&pLine->estrFull, "<img src=Star_Selected_small.tga border=1 color=#aaaaaa>");
                else
                    estrConcatStaticCharArray(&pLine->estrFull, "<img src=Star_Selected_small.tga border=1 color=ArchitectTitle>");
            }
            estrConcatf( &pLine->estrFull, "<font outline=0 color=Architect>%s</font><br>", textStd(s_ratingNames[pLine->myVote]));
        }
        //arc id
        estrConcatf(&pLine->estrFull, "<font outline=1 color=ArchitectTitle>%s: </font><font outline=0 color=architect>%i</font><br>", textStd("MissionSearchArcId"), pLine->id );
    }

    // keywords
    if( pLine->keyword_votes )
    {
        char * word[3];
        int num = 0;
        
        for( i = 0; i < 32 && num < 3; i++ )
        {
            if( pLine->keyword_votes & (1<<i) )
            {
                char * wordName = missionReviewGetKeywordKeyFromNumber(i);
                if(!wordName)
                    continue;
                word[num] = estrTemp();
                if(s_MissionSearch.keywords & (1<<i))
                    estrPrintf(&word[num], "%s%s%s", SEARCH_HIGHLIGHTED_SMF_START, textStd(wordName), SEARCH_HIGHLIGHTED_SMF_END);
                else if(word)
                    estrPrintCharString(&word[num], textStd(wordName));

                num++;
            }
        }

        if( num==3 )
            estrConcatf(&pLine->estrFull, "<font outline=1 color=ArchitectTitle>%s: </font><font outline=0 color=architect>%s, %s, %s</font><br>", textStd("MissionSearchKeywords"), word[0], word[1], word[2] );
        else if( num==2 )
            estrConcatf(&pLine->estrFull, "<font outline=1 color=ArchitectTitle>%s: </font><font outline=0 color=architect>%s, %s</font><br>", textStd("MissionSearchKeywords"), word[0], word[1] );
        else if( num==1 )
            estrConcatf(&pLine->estrFull, "<font outline=1 color=ArchitectTitle>%s: </font><font outline=0 color=architect>%s</font><br>", textStd("MissionSearchKeywords"), word[0] );
        for(i = 0; i < num; i++)
            estrDestroy(&word[i]);

    }

    //length
    pchText = textStd("MissionSearchNA");
    for( i = ARRAY_SIZE(s_lengthList)-1; i >=0; i-- )
    {
        if( pLine->header.arcLength & (1<<s_lengthList[i].ID) )
            pchText = textStd(s_lengthList[i].name);
    }
    estrConcatf(&pLine->estrFull,
        "<font outline=1 color=ArchitectTitle>%s: </font><font outline=0 color=architect>%s</font><br>",
        textStd("MissionSearchLengthOnly"), pchText);

    if(pLine->id && pLine->date_created)
    {
        //first published
        estrConcatf(&pLine->estrFull, 
            "<font outline=1 color=ArchitectTitle>%s: </font><font outline=0 color=architect>%s</font><br>", 
            textStd("MissionSearchDate"),
            timerMakeDateStringFromSecondsSince2000NoSeconds(pLine->date_created));
    }

    //morality
    pchText = textStd("MissionSearchNA");
    for( i = ARRAY_SIZE(s_alignmentList)-1; i >=0; i-- )
    {
        if( pLine->header.morality & (1<<s_alignmentList[i].ID) )
            pchText = textStd(s_alignmentList[i].name);
    }
    estrConcatf( &pLine->estrFull, "<font outline=1 color=ArchitectTitle>%s: </font><font outline=0 color=architect>%s</font><br>", textStd("MissionSearchAlignment"), pchText  );

    //missions
    for( i = 0; i < eaSize(&pLine->header.missionDesc); i++ ) 
    {
        estrConcatf( &pLine->estrFull, "<font outline=1 color=ArchitectTitle>%s: </font>",textStd("MissionSearchMissionNum", i+1) );
        if( !pLine->header.missionDesc[i]->size )
        {
            estrConcatf( &pLine->estrFull, "<font outline=0 color=architect>%s </font>", textStd("MissionSearchUniqueDesc",  pLine->header.missionDesc[i]->min, pLine->header.missionDesc[i]->max  )  );
        }
        else
        {
            estrConcatf( &pLine->estrFull, "<font outline=0 color=architect>%s </font>", 
                textStd("MissionSearchDesc", StaticDefineIntRevLookupDisplay(MapLengthEnum, pLine->header.missionDesc[i]->size), pLine->header.missionDesc[i]->min, pLine->header.missionDesc[i]->max  )  );
        }
        if( pLine->header.missionDesc[i]->detailTypes )
        {
            int j, missionType = pLine->header.missionDesc[i]->detailTypes;
            first = 1;
            estrConcatf( &pLine->estrFull, "<font outline=0 color=Architect>%s", textStd("MissionSearchContains"));
            for( j = 0; j < kDetail_Count; j++ )
            {
                if( missionType&(1<<j) )
                {
                    MMRegion * pRegion=0;
                    int k;
                    for( k = 0; k < eaSize(&ppDetailTemplates); k++ )
                    {
                        if( j == ppDetailTemplates[k]->detailType )
                            pRegion = ppDetailTemplates[k];
                    }
                    if(!pRegion)
                        continue;

                    if( first )
                    {
                        estrConcatCharString(&pLine->estrFull, textStd(pRegion->pchDisplayName));
                        first = 0;
                    }
                    else
                        estrConcatf(&pLine->estrFull, ", %s", textStd(pRegion->pchDisplayName));
                }
            }
            estrConcatStaticCharArray(&pLine->estrFull, ".</font><br>");
        }
        else
            estrConcatStaticCharArray(&pLine->estrFull, "<br>");
    }
    //enemy groups
    estrConcatf( &pLine->estrFull, "<font outline=1 color=ArchitectTitle>%s: </font><font outline=0 color=architect>", textStd("MissionSearchVillainGroups") );
    if(pLine->formattedVillains)
        estrConcatCharString(&pLine->estrFull, pLine->formattedVillains);
    else
    {
        s_buildVillainList(pLine->header.villaingroups, &pLine->estrFull, 0,0,0);
    }
     estrConcatStaticCharArray(&pLine->estrFull, "</font><br>");

    //description
     estrConcatf( &pLine->estrFull, "<font outline=1 color=ArchitectTitle>%s: </font><font outline=0 color=architect>%s</font>", textStd("MissionSearchSummary"), (pLine->formattedSummary)?(pLine->formattedSummary):smf_EncodeAllCharactersGet(pLine->header.summary) );
        
    // Status 
     estrConcatf(&pLine->estrFull, "<br><font outline=1 color=ArchitectTitle>%s: </font><font outline=0 color=architect>%s</font>", textStd("MissionSearchStatus"), textStd(StaticDefineIntRevLookupDisplay(ArcStatusEnum, pLine->header.arcStatus)) );

    // Warnings
    if( pLine->header.hazards ) 
    {
        int comma = 0;

        estrConcatf( &pLine->estrFull, "<br><font outline=1 color=ArchitectError>%s:</font><font outline=0 color=ArchitectError> ", textStd("MissionSearchHazard") );
        if( pLine->header.hazards & (1<<HAZARD_GIANT_MONSTER) )
        {
            estrConcatf( &pLine->estrFull, " %s", textStd("MissionSearchHazardGiantMonster"));
            comma = 1;
        }
        if( pLine->header.hazards & (1<<HAZARD_ARCHVILLAIN) )
        {
            estrConcatf( &pLine->estrFull, " %s", textStd("MissionSearchHazardAV"));
            comma = 1;
        }
        if( pLine->header.hazards & (1<<HAZARD_ELITEBOSS) )
        {
            if( comma )
                estrConcatStaticCharArray(&pLine->estrFull, ", ");
            estrConcatf( &pLine->estrFull, "%s", textStd("MissionSearchHazardEB"));
            comma = 1;
        }
        if( pLine->header.hazards & (1<<HAZARD_EXTREME_ARCHVILLAIN) )
        {
            if( comma )
                estrConcatStaticCharArray(&pLine->estrFull, ", ");
            estrConcatf( &pLine->estrFull, "%s", textStd("MissionSearchHazardExtremeAV"));
            comma = 1;
        }
        if( pLine->header.hazards & (1<<HAZARD_EXTREME_ELITEBOSS) )
        {
            if( comma )
                estrConcatStaticCharArray(&pLine->estrFull, ", ");
            estrConcatf( &pLine->estrFull, "%s", textStd("MissionSearchHazardExtremeEB"));
            comma = 1;
        }
        if( pLine->header.hazards & (1<<HAZARD_EXTREME_BOSS) )
        {
            if( comma )
                estrConcatStaticCharArray(&pLine->estrFull, ", ");
            estrConcatf( &pLine->estrFull, "%s", textStd("MissionSearchHazardExtremeBoss"));
            comma = 1;
        }
        if( pLine->header.hazards & (1<<HAZARD_EXTREME_LT) )
        {
            if( comma )
                estrConcatStaticCharArray(&pLine->estrFull, ", ");
            estrConcatf( &pLine->estrFull, "%s", textStd("MissionSearchHazardExtremeLt"));
            comma = 1;
        }
        if( pLine->header.hazards & (1<<HAZARD_EXTREME_MINIONS) )
        {
            if( comma )
                estrConcatStaticCharArray(&pLine->estrFull, ", ");
            estrConcatf( &pLine->estrFull, "%s", textStd("MissionSearchHazardExtremeMinion"));
            comma = 1;
        }
        if( pLine->header.hazards & (1<<HAZARD_HIGHLEVEL_DOWN) )
        {
            if( comma )
                estrConcatStaticCharArray(&pLine->estrFull, ", ");
            estrConcatf( &pLine->estrFull, "%s", textStd("MissionSearchHazardHighLevelDown"));
        }
        if( pLine->header.hazards & (1<<HAZARD_CUSTOM_POWERS) )
        {
            if( comma )
                estrConcatStaticCharArray(&pLine->estrFull, ", ");
            estrConcatf( &pLine->estrFull, "%s", textStd("MissionSearchHazardCustomPower"));
        }
        estrConcatStaticCharArray(&pLine->estrFull, "!</font><br>");
    }

    if( !pLine->pBlock )
        pLine->pBlock = smfBlock_Create();
}

// Local Arcs //////////////////////////////////////////////////////////////////

static MissionSearchTab* s_myArcsTab(void)
{
    int i;
     for( i = ARRAY_SIZE(s_categoriesList)-1; i>=0; i-- )
    {
        if( s_categoriesList[i].category == MISSIONSEARCH_MYARCS )
            return &s_categoriesList[i];
    }
    return 0; //!!
}

int missionsearch_sort( const MissionSearchLine ** p1, const MissionSearchLine **p2 )
{ 
    int reverser = 1;
    int sort_type = ABS(s_sort_idx);

    if( s_sort_idx < 0 )
        reverser = -1;
    //if this matches arc id
    if((*p1)->id == s_MissionSearch.arcid)
        return -1;
    else if((*p2)->id == s_MissionSearch.arcid)
        return 1;

    switch( sort_type)
    {
        xcase kMissionSearchSort_Name:    
    {
        int val = strcmp( (*p1)->header.title, (*p2)->header.title );
        if( val )
            return reverser*val;
    }
    xcase kMissionSearchSort_ArcID:    
    {
        if( (*p1)->id != (*p2)->id )
            return (-1*reverser*( (*p2)->id - (*p1)->id ) );
    }
    xcase kMissionSearchSort_Rating:
    {
        if((*p1)->rating.honors != (*p2)->rating.honors)
            return -reverser*((*p2)->rating.honors - (*p1)->rating.honors);
        else if((*p1)->rating.rating != (*p2)->rating.rating)
            return -reverser*missionrating_cmp((*p2)->rating.rating, (*p1)->rating.rating);
        else    //equal, check the number of votes
            return -reverser*((*p2)->total_votes - (*p1)->total_votes);
    }
    xcase kMissionSearchSort_Length:
    {
        if( (*p1)->header.arcLength != (*p2)->header.arcLength )
            return (-1*reverser*( (*p2)->header.arcLength - (*p1)->header.arcLength ) );
    }
    xcase kMissionSearchSort_Morality:
    {
        if( (*p1)->header.morality != (*p2)->header.morality )
            return (reverser*( (*p2)->header.morality - (*p1)->header.morality ) );
    }
    xcase kMissionSearchSort_Detail:
    {
        if( (*p1)->header.missionType != (*p2)->header.missionType )
            return (reverser*( (*p2)->header.missionType - (*p1)->header.missionType ) );
    }
    case kMissionSearchSort_Date:    
    default:
        {
            if( (*p1)->id != (*p2)->id )
                return (-1*reverser*( (*p2)->id - (*p1)->id ) );
            else if( (*p1)->date_created != (*p2)->date_created )
                return (-1*reverser*(((*p2)->date_created > (*p1)->date_created)?1:-1) );
            else if((*p1)->timestamp != (*p2)->timestamp )
                return (-1*reverser*(((*p2)->timestamp > (*p1)->timestamp)?1:-1) );
        }
    }

    // Name is default fallback, its also the secondary search
    return reverser*stricmp( (*p1)->header.title, (*p2)->header.title );
}


static FileScanAction s_CheckAndAddArc(char* dir, struct _finddata32_t* data)
{
    char filename[MAX_PATH];
    sprintf(filename, "%s/%s", dir, data->name);

    if(strEndsWith(filename, missionMakerExt()) && !strstri(filename, ".backup.") && fileExists(filename)) // an actual file, not a directory
    {
        MissionSearchTab *tab = s_myArcsTab();
        MissionSearchLine *line = NULL;
        __time32_t timestamp = fileLastChanged(filename);
        char shortname[MAX_PATH];
        int i;

        // get the shortened filename used in the editor (no path, no ext)
        strcpy(shortname, data->name);
        shortname[strlen(data->name) - strlen(missionMakerExt())] = '\0';

        // find the old entry, or make a new one
        for(i = 0; i < eaSize(&tab->local); i++)
        {
            line = tab->local[i];
            if(0==stricmp(line->filename, shortname))
                break;
        }
        if(i >= eaSize(&tab->local))
        {
            line = s_createCachedSearchLine(0);
            line->filename = strdup(shortname);
            eaPush(&tab->local, line);
        }

        // don't bother reloading if the file hasn't changed
        if(line->timestamp != timestamp)
        {
            playerCreatedStoryArc_Destroy(line->arc);
            line->arc = playerCreatedStoryArc_Load(filename); // this can return null, which is fine
            compressArcPCCCostumes(line->arc);

            if( !line->arc )
                line->failed_validation = 1;
            else
                line->failed_validation = playerCreatedStoryarc_GetValid(line->arc, 0, 0, 0, playerPtr(), 0)!=kPCS_Validation_OK;

            playerCreatedStoryArc_FillSearchHeader(line->arc, &line->header, NULL);

            line->timestamp = timestamp;
        }
        line->deleteme = 0;

        missionsearch_CheckLineAgainstSearch(line);
        line->show = 1;
        updateMissionSearchLine( line );
    }

    return FSA_NO_EXPLORE_DIRECTORY;
}

static void missionsearch_LoadMyArcs(void)
{
    MissionSearchTab *tab = s_myArcsTab();
    if(!tab->local_loaded)
    {
        int i;
        for(i = 0; i < eaSize(&tab->local); i++)
            tab->local[i]->deleteme = 1;
        fileScanDirRecurseEx(missionMakerPath(), s_CheckAndAddArc);
        tab->local_loaded = 1;
        if(tab && tab->local)
            eaQSort( tab->local, missionsearch_sort );
    }
}

void missionsearch_myArcsChanged(const char *relpath, int when)
{
    MissionSearchTab *tab = s_myArcsTab();
    tab->local_loaded = 0; // do a reload.  we could reload the changed file here, but this is easier
}

static void missionsearch_InitMyArcs(void)
{
    char reloadpath[MAX_PATH];
    FolderCache *fc;

    sprintf(reloadpath, "%s/", missionMakerPath());
    mkdirtree(reloadpath);

    fc = FolderCacheCreate();
    FolderCacheAddFolder(fc, missionMakerPath(), 0);
    FolderCacheQuery(fc, NULL);
    sprintf(reloadpath, "*%s", missionMakerExt());
    FolderCacheSetCallback(FOLDER_CACHE_CALLBACK_UPDATE_AND_DELETE, reloadpath, missionsearch_myArcsChanged);
}

static void s_deleteArcDialog(MissionSearchLine *line)
{
    char filename[MAX_PATH];
    sprintf(filename, "%s/%s%s", missionMakerPath(), line->filename, missionMakerExt());
    fileMoveToRecycleBin(filename);
    line->deleteme = 1;
}

static void s_deleteArc(MissionSearchLine *line)
{
    assert(!line->id && line->filename);
    dialog(DIALOG_YES_NO, -1, -1, -1, -1, "MissionSearchReallyDelete", NULL, s_deleteArcDialog, NULL, NULL, DLGFLAG_ARCHITECT, 0, 0, 1, 0, 0, line);
}
static void s_deleteCustomCritterDialog(PCC_Critter *pCritter)
{
    removeCustomCritter(pCritter->sourceFile, 1);
    updateCustomCritterList(&missionMaker);
    populateCVGSS();
}
static void s_deleteCustomCritter(PCC_Critter *pCritter)
{
    dialog(DIALOG_YES_NO, -1, -1, -1, -1, "CustomCritterReallyDelete", NULL, s_deleteCustomCritterDialog, NULL, NULL, DLGFLAG_ARCHITECT, 0, 0, 1, 0, 0, pCritter);
}
static void s_deleteCVGDialog(CustomVG *cvg)
{
    CVG_removeCustomVillainGroup(cvg->displayName, 1);
    updateCustomCritterList(&missionMaker);
}
static void s_deleteCustomVillainGroup(CustomVG *cvg)
{
    dialog(DIALOG_YES_NO, -1, -1, -1, -1, "CVGReallyDelete", NULL, s_deleteCVGDialog, NULL, NULL, DLGFLAG_ARCHITECT, 0, 0, 1, 0, 0, cvg);
}
void missionsearch_deleteArc(const char *filename)
{
    MissionSearchTab *tab = s_myArcsTab();
    int i;
    for(i = 0; i < eaSize(&tab->local); i++)
        if(tab->local[i]->filename && stricmp(tab->local[i]->filename, filename) == 0)
            s_deleteArc(tab->local[i]);
}

// uiNet Interface /////////////////////////////////////////////////////////////


void missionsearch_TabClear(MissionSearchPage category, const char *context, int page, int pages)
{
    MissionSearchTab *tab = missionsearch_GetTabByCategory( category );

    if(tab)
    {
        eaClearEx(&tab->lines, s_destroyCachedSearchLine);
        if(context[0] && !devassert(tab->context))
            estrPrintCharString(&tab->context, context);
        tab->page = page;
        if(tab->page < 0)
            tab->page = (page *-1) -1;
        tab->pages = pages;
        MissionSearch_pageNavReset(&s_pNav, pages);
        s_pNav.page = tab->page;
        tab->loaded = 1;
        tab->list_scroll.offset = 0;

        if(tab->category != category) // if category defaulted, then it's a csr command, open the tab for them
        {
            s_categoryCurrent = tab - &s_categoriesList[0];
            window_setMode(WDW_MISSIONSEARCH, WINDOW_GROWING);
            tab->presorted = 1;
        }
        else
        {
            tab->presorted = 0;
        }
    }
}

void missionsearch_SortResults(MissionSearchPage category)
{
    MissionSearchTab *tab = missionsearch_GetTabByCategory( category );
    if(tab && !tab->presorted)
        eaQSort( tab->lines, missionsearch_sort );
}

void missionsearch_TabAddLine(MissionSearchPage category, int arcid, int mine, int played, int myVote, int ratingi, int total_votes, int keyword_votes, U32 date, const char *header)
{
    if(devassert(arcid))
    {
        MissionSearchLine *line;
        MissionSearchTab *tab = missionsearch_GetTabByCategory( category );

        if(tab)
        {
            line = s_createCachedSearchLine(arcid);
            eaPush(&tab->lines, line);
        }
        else
        {
            line = s_findCachedSearchLine(arcid);
        }

        if(line)
        {
            line->owned = mine;
            line->myVote = myVote;
            line->rating.i = ratingi;
            line->total_votes = total_votes;
            line->keyword_votes = keyword_votes;
            line->date_created = date;
            line->played = played;
            StructClear(parse_MissionSearchHeader, &line->header);
            ZeroStruct(&line->header);
            ParserReadText(header, -1, parse_MissionSearchHeader, &line->header);

            missionsearch_CheckLineAgainstSearch(line);
            line->show = 1;
            updateMissionSearchLine( line );
            s_updateRefine = 1;
        }
    }
}

void missionsearch_TabAddSection(MissionSearchPage category, const char *section)
{
    if(devassert(section && section[0] && category != MISSIONSEARCH_NOPAGE))
    {
        MissionSearchTab *tab = missionsearch_GetTabByCategory(category);
        MissionSearchLine *line = s_createCachedSearchLine(0);
        line->section_header = strdup(section);
        eaPush(&tab->lines, line);
    }
}

void missionsearch_ArcDeleted(int arcid)
{
    MissionSearchLine *line = s_findCachedSearchLine(arcid);
    if(line)
        line->deleteme = 1;
}

void missionsearch_SetArcData(int arcid, const char *arcdata)
{
    MissionSearchLine *line = s_findCachedSearchLine(arcid);
    if(line && (line->owned || playerPtr()->access_level))
    {
        playerCreatedStoryArc_Destroy(line->arc);
        line->arc = playerCreatedStoryArc_FromString((char*)arcdata, NULL, 0, 0, 0, 0);
        if(    !line->owned && playerPtr()->access_level && // a bit of a hack
            window_getMode(WDW_MISSIONSEARCH) == WINDOW_DISPLAYING )
        {
            if (line->saveLocally)
            {
                //    first pass at dev local saves
                //    will overwrite existing arcs w/o warning and will always start at 0
                //    but I dont think this will be used very often. -EL
                static int numLocalSaveFiles = 0;
                char fileNameBuffer[50];
                char textBuffer[100];
                sprintf(fileNameBuffer, "DevLocalSave%d", numLocalSaveFiles++);
                playerCreatedStoryArc_Save(line->arc, getMissionPath(fileNameBuffer)) ;
                sprintf(textBuffer, "File %s%s saved locally", fileNameBuffer, missionMakerExt());
                dialogStd(DIALOG_OK, textBuffer, 0, 0, 0, 0, DLGFLAG_ARCHITECT);
                line->saveLocally = 0;
            }
            else
            {
                MMtoggleArchitectLocalValidation((line->id && !line->owned && playerPtr()->access_level) ? 1 : 0);
                missionMakerOpenWithArc(line->arc, line->filename, line->id, line->owned);
                window_setMode(WDW_MISSIONSEARCH, WINDOW_SHRINKING);
            }
            playerCreatedStoryArc_Destroy(line->arc); // clear, so we'll get an update if they republish
            line->arc = NULL;
        }

        // this function doesn't update the header, so we shouldn't have to regenerate smf
        // updateMissionSearchLine( line );
    }
}

// Published Arcs //////////////////////////////////////////////////////////////

static void s_playPublishedArc_accept(int arcid, int devchoice)
{
    if(arcid)
    {
        missionserver_game_requestArcData_playArc(arcid,0,devchoice);
        window_setMode(WDW_CUSTOMVILLAINGROUP, WINDOW_SHRINKING);
        window_setMode(WDW_MISSIONMAKER, WINDOW_SHRINKING);
        window_setMode(WDW_MISSIONSEARCH, WINDOW_SHRINKING);
    }
}

static void s_playPublishedArc_AcceptDefault(void * p)
{
    int arcid = PTR_TO_U32(p);
    s_playPublishedArc_accept( arcid, 0 );
}

static void s_playPublishedArc_AcceptDevChoice(void *p)
{
    int arcid = PTR_TO_U32(p);
    s_playPublishedArc_accept( arcid, 1 );
}

static void s_testPublishedArc_accept(void *p)
{
    int arcid = PTR_TO_U32(p);
    if(arcid)
    {
        missionserver_game_requestArcData_playArc(arcid,1,0);
        window_setMode(WDW_CUSTOMVILLAINGROUP, WINDOW_SHRINKING);
        window_setMode(WDW_MISSIONMAKER, WINDOW_SHRINKING);
        window_setMode(WDW_MISSIONSEARCH, WINDOW_SHRINKING);
    }
}

void missionsearch_playPublishedArc(int arcid, int dev_choice)
{
    Entity *e = playerPtr();
    if(e->pl->taskforce_mode)
    {
        dialogStd( DIALOG_OK, "MustLeaveTaskForce", 0, 0, 0, 0, DLGFLAG_ARCHITECT );
    }
    else
    {
        if (e && e->pl && AccountCanPlayMissionArchitect(ent_GetProductInventory( e ), e->pl->loyaltyPointsEarned, e->pl->account_inventory.accountStatusFlags))
        {
            int allRewards = e && e->pl && AccountCanGainAllMissionArchitectRewards(ent_GetProductInventory( e ), e->pl->loyaltyPointsEarned, e->pl->account_inventory.accountStatusFlags);
            if ( dev_choice && allRewards )
                dialogStdCB(DIALOG_TWO_RESPONSE_CANCEL, "EnteringArchitectDevChoice", "MAChooseStdRewards", "MAChooseTickets", s_playPublishedArc_AcceptDevChoice, s_playPublishedArc_AcceptDefault, DLGFLAG_ARCHITECT, U32_TO_PTR(arcid) );
            else if ( dev_choice && !allRewards )
                dialogStdCB(DIALOG_ACCEPT_CANCEL, "EnteringArchitectDevChoiceForceStandardDrops", 0, 0, s_playPublishedArc_AcceptDevChoice, NULL, DLGFLAG_ARCHITECT, U32_TO_PTR(arcid) );
            else if ( !dev_choice && allRewards )
                dialogStdCB(DIALOG_ACCEPT_CANCEL, "EnteringArchitectTaskForceWarning", 0, 0, s_playPublishedArc_AcceptDefault, NULL, DLGFLAG_ARCHITECT, U32_TO_PTR(arcid));
            else // if ( !dev_choice && !allRewards )
                dialogStdCB(DIALOG_ACCEPT_CANCEL, "EnteringArchitectTaskForceWarningFewerRewards", 0, 0, s_playPublishedArc_AcceptDefault, NULL, DLGFLAG_ARCHITECT, U32_TO_PTR(arcid));
        }
        else
        {
            dialogStd( DIALOG_OK, "InvalidPermissionTier2MALic", 0, 0, 0, 0, DLGFLAG_ARCHITECT );
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

static void s_setSearchSort(MissionSearchParams *search)
{
    MissionServerSearchType sort;
    if(!search)
        return;

    switch(ABS(s_sort_idx))
    {
        //xcase kMissionSearchSort_Name:    //don't sort by this on the server.
        //xcase kMissionSearchSort_Author:// ditto
        xcase kMissionSearchSort_ArcID:
            sort = MSSEARCH_ALL_ARC_IDS;
        xcase kMissionSearchSort_Rating:
            sort = MSSEARCH_RATING;
        xcase kMissionSearchSort_VG:
            sort = MSSEARCH_VILLAIN_GROUPS;
        xcase kMissionSearchSort_Length:
            sort = MSSEARCH_ARC_LENGTH;
        xcase kMissionSearchSort_Detail:
            sort = MSSEARCH_MISSION_TYPE;
        xcase kMissionSearchSort_Morality:
            sort = MSSEARCH_MORALITY;
        xcase kMissionSearchSort_Date:
            sort = MSSEARCH_ALL_ARC_IDS;
        xdefault:
            sort = MSSEARCH_ALL_ARC_IDS;
    }

    search->sort = sort;
}

void missionsearch_clearPages(MissionSearchTab *tab)
{
    if(tab)
    {
        eaClearEx(&tab->lines, s_destroyCachedSearchLine);
        tab->list_scroll.offset = 0;
        tab->loaded = 0;
    }
}

void missionsearch_clearAllPages()
{
    int i;
    for(i = 0; i < ARRAY_SIZE(s_categoriesList); i++)
        missionsearch_clearPages(&s_categoriesList[i]);
}

void missionsearch_requestTab(MissionSearchTab *tab)
{
    int pageIdx = tab->page;    //we may send the idx as a negative if we've flipped the sort
    U32 oldBan;
    estrClear(&tab->context);
    s_setSearchSort(&s_MissionSearch); 
    if(s_sort_idx < 0)
    {
        pageIdx = -1*(tab->page+1);    
    }
    oldBan = s_MissionSearch.ban;
    if(!playerPtr()->access_level)
    {
        missionban_removeInvisible(&s_MissionSearch.ban);
        if(!s_MissionSearch.ban)
            missionban_setVisible(&s_MissionSearch.ban);
    }
    ParserWriteText(&tab->context, parse_MissionSearchParams, &s_MissionSearch, 0, 0);

    missionsearch_clearPages(tab);
    if(tab-&s_categoriesList[0] == s_categoryCurrent)
    {
        missionsearch_UpdateSearchParams(0);
        MissionSearch_requestSearch(tab->category, tab->context, pageIdx, s_viewedFilter, (s_viewedFilter & (1<<kMissionSearchViewedFilter_Level))?character_CalcExperienceLevel(playerPtr()->pchar)+1:0, 0);
        tab->requested = timerSecondsSince2000();
    }
    else
    {
        // we aren't looking at this tab, do the search when/if we do.
        tab->requested = 0;
    }
    tab->list_scroll.offset = 0;
    s_updateRefine = 1;
    s_pNav.nPages = tab->pages;
    s_pNav.page = tab->page;
    s_MissionSearch.ban = oldBan;
}

void missionsearch_SelectLine(MissionSearchTab *tab, MissionSearchLine *line)
{
     if( tab->selected == line )
        tab->selected = 0;
    else
        tab->selected = line;
    //tab->info_scroll.offset = 0;
}





// X - Y are from bottom right
static F32 missionsearch_sortButton( F32 x, F32 y, F32 z, F32 sc, MissionSearchSort eSort, char * pchText, int active)
{
    AtlasTex *arrow = 0;
    MissionSearchTab *tab = &s_categoriesList[s_categoryCurrent];
     F32 ty = y, wd, ht = 26*sc;
    CBox box;
    UIBox uibox;
     int box_clr = 0xfffffff22;

      font(&game_12);
    font_ital(1);
    if(active)
         font_color(CLR_MM_TEXT ,CLR_MM_TEXT);
    else
        font_color(0xaaaaaaff ,0xaaaaaaff);

       wd = str_wd( font_grp, sc, sc, pchText ) + 2*R10*sc + 10*sc;

    if(s_sort_idx == eSort && !active)
        s_sort_idx = kMissionSearchSort_ArcID;

    if( -s_sort_idx == eSort)
    {
        arrow = atlasLoadTexture( "chat_separator_arrow_down.tga" );
        ty = y - 12*sc;
        //wd += 12*sc;
    }
    else if( s_sort_idx == eSort) 
    {
        arrow = atlasLoadTexture( "chat_separator_arrow_up.tga" );
        ty = y - 14*sc;
        //*wd*/ += 12*sc;
    }

    BuildCBox(&box, x - wd, y - ht, wd, ht );
       uiBoxDefine(&uibox, x - wd, y - ht, wd, ht );
    clipperPush(&uibox);

    if(active)
    {
         if( mouseCollision(&box) )
            box_clr = 0xfffffff88;
        if( mouseClickHit(&box, MS_LEFT) )
        {
            box_clr = 0xffffffaa;

            if( ABS(s_sort_idx) == eSort )
                s_sort_idx = -s_sort_idx;
            else
                s_sort_idx = eSort;

            eaQSort( tab->lines, missionsearch_sort );
            eaQSort( tab->local, missionsearch_sort );
            missionsearch_UpdateSearchParams(0);
            missionsearch_requestTab(tab);
        }
    }
    else
    {
        box_clr = CLR_TAB_INACTIVE;
    }

       if(box_clr)
    {
        AtlasTex * end = atlasLoadTexture("Tab_end.tga");
        AtlasTex * mid = atlasLoadTexture("Tab_mid.tga");

          display_sprite( end, x - wd, y - ht + 10*sc, z, sc, sc, box_clr );
        display_spriteFlip( end, x - end->width*sc, y - ht + 10*sc, z, sc, sc, box_clr, FLIP_HORIZONTAL );
        display_sprite( mid, x - wd + end->width*sc, y - ht + 10*sc, z, (wd-2*end->width*sc)/mid->width, sc, box_clr );
    }

       prnt( x-wd+8*sc, y, z+5, sc, sc, pchText );

     if(arrow)
         display_sprite(arrow, x - (R10+8)*sc, ty, z+5, 12*sc/arrow->width, 10*sc/arrow->height, CLR_WHITE );

     clipperPop();
    font_ital(0);

    return wd;
}

int missionsearch_MissionLineBreak(char *title, float x, float y, float z, float wd, float sc, int showSortOptions)
{
    int ret = D_NONE;

    y += 18*sc;
 
    if(title)
    {
         font(&game_18);
        font_color(CLR_MM_TEXT ,CLR_MM_TEXT);
         cprntEx(x + 5*sc, y, z+1, sc, sc, 0, title);
    }

      if(showSortOptions)
    {
         F32 twd = wd - PIX3*sc;
         twd -= PIX3*sc + missionsearch_sortButton( x+twd, y, z, sc, kMissionSearchSort_Date, "MMDate", 1);
        twd -= PIX3*sc + missionsearch_sortButton( x+twd, y, z, sc, kMissionSearchSort_Length, "MMLength",1);
        twd    -= PIX3*sc + missionsearch_sortButton( x+twd, y, z, sc, kMissionSearchSort_Rating, "MMRating",1);

        font(&gamebold_12);
        font_color(CLR_MM_TEXT ,CLR_MM_TEXT);
         cprntEx(x + twd - str_wd(font_grp,sc,sc,"MMSortBy") - 2*PIX3*sc, y, z+1, sc, sc, 0, "MMSortBy" );    
    }



     drawHorizontalLine(x, y, wd, z, sc, CLR_MM_TEXT);
    return ret;
}

static void s_unpublishArc(void *arcid)
{
    MissionSearchLine *line = s_findCachedSearchLine(PTR_TO_U32(arcid));
    if(line)
    {
        missionserver_game_unpublishArc(line->id);
        line->deleteme = 1;
        
    }
}

void missionsearch_allowDevChoiceEditing(void *accept)
{
    missionMaker.owned = 1;
}

float missionsearch_LineButtons(MissionSearchLine *line, F32 x, F32 y, F32 z, F32 wd, F32 ht, F32 sc, int selected)
{
    float buttonht = 40*sc;
    float buttonwd = MISSIONSEARCH_BUTTONWD;
    float starty;
    float origX = x;
    Entity *e = playerPtr();

       MissionSearchTab *tab = &s_categoriesList[s_categoryCurrent];

    z+=1;
    //deal with honors images.
    {
        AtlasTex *honor = 0, *honor2 = 0;
        int found = 0;
        char * text = 0;
        
        if(line->id)
        {
            if(line->rating.invalidated || (line->owned && line->rating.playableErrors))
            {
                text = "MissionSearchInvalidArc";
                honor = atlasLoadTexture("Invalid_Arc_Icon.tga");
            }
            else if( !missionban_isVisible(line->rating.ban) )
            {
                text = "MissionSearchBanned";
                honor = atlasLoadTexture("Banned_Icon.tga");
            }
            else if( line->rating.honors == MISSIONHONORS_CELEBRITY)
            {
                text = "MissionSearchCelebrity";
                honor = atlasLoadTexture("GuestAuthor_ring_32x32");
            }
            else if( line->rating.honors == MISSIONHONORS_DC_AND_HOF )
            {
                // todo, both icons
                text = "MissionSearchDevChoice";
                honor2 = atlasLoadTexture("Hall_of_Fame_Icon.tga");
                honor = atlasLoadTexture("Dev_Choice_Icon.tga");
            }
            else if( line->rating.honors == MISSIONHONORS_HALLOFFAME ||line->rating.honors == MISSIONHONORS_FORMER_DC_HOF )
            {
                text = "MissionSearchHallofFame";
                honor = atlasLoadTexture("Hall_of_Fame_Icon.tga");
            }
            else if( line->rating.honors == MISSIONHONORS_DEVCHOICE )
            {
                text = "MissionSearchDevChoice";
                honor = atlasLoadTexture("Dev_Choice_Icon.tga");
            }
        }
        if(honor)
            found++;
        else
            honor = atlasLoadTexture("Dev_Choice_Icon.tga");
            
         if( honor2 )
        {
            origX = x;
            x += wd - buttonwd - honor->width*sc - PIX3*sc;
            display_sprite( honor2, x, y+2*PIX3*sc , z, sc, sc, 0xffffffff );
            x = origX;
        }
         
         if(honor)
         {
             x +=wd - buttonwd - (honor2?2:1)*honor->width*sc - (honor2?2:1)*PIX3*sc;
            origX = x;
            if(found)
            {
                 CBox box = {0};
                display_sprite( honor, x, y+2*PIX3*sc , z, sc, sc, 0xffffffff );
                BuildCBox(&box, x , y+2*sc*PIX3, (honor2?2:1)*honor->width*sc, honor->height*sc);

                if(mouseCollision(&box))
                {
                    F32 wd = str_wd(&game_12, sc, sc, textStd(text)) + 2*sc*PIX3;
                    font_color(CLR_MM_TEXT,CLR_MM_TEXT);
                    font(&game_12);
                    font_outl(0);
                    cprntEx( x-wd+5*sc, y +honor->height*sc*.5 +PIX2*sc+2*sc, z+3, sc, sc, CENTER_Y, textStd(text));
                    drawFrame( PIX3, R10, x-wd-3*PIX3*sc, y+honor->height*sc*.5-10*sc+sc*PIX3+sc*PIX2, z+2, wd+PIX3*2*sc, 20*sc, sc, CLR_MM_BACK_DARK, CLR_MM_BACK_DARK );
                    font_outl(1);
                }
            }
             x+= 2*PIX3*sc + (honor2?2:1)*honor->width*sc;
        }
        /*
        // we should probably put these back in, with an indicator for unpublished/invalidated as well.
        if(line->rating.ban == MISSIONBAN_NONE && playerPtr()->access_level)
            estrConcatf( &line->estrFull, "<color white>%s</color><br>", textStd("MissionSearchNeverPulled") );
        else if(line->rating.ban == MISSIONBAN_PULLED)
            estrConcatf( &pLine->estrFull, "<color red>%s</color><br>", textStd("MissionSearchPulledOnce") );
        else if(line->rating.ban == MISSIONBAN_SELFREVIEWED && playerPtr()->access_level)
            estrConcatf( &pLine->estrFull, "<color red>%s</color><br>", textStd("MissionSearchSelfReviewed") );
        else if(line->rating.ban == MISSIONBAN_SELFREVIEWED_PULLED)
            estrConcatf( &pLine->estrFull, "<color red>%s</color><br>", textStd("MissionSearchPulled") );
        else if(line->rating.ban == MISSIONBAN_SELFREVIEWED_SUBMITTED)
            estrConcatf( &pLine->estrFull, "<color red>%s</color><br>", textStd("MissionSearchAppealling") );
        else if(line->rating.ban == MISSIONBAN_CSREVIEWED_BANNED)
            estrConcatf( &pLine->estrFull, "<color red>%s</color><br>", textStd("MissionSearchBanned") );
        else if(line->rating.ban == MISSIONBAN_CSREVIEWED_APPROVED && (pLine->owned || playerPtr()->access_level) )
            estrConcatf( &pLine->estrFull, "<color white>%s</color><br>", textStd("MissionSearchApproved") );
        */
    }

    y += buttonht*.5;
    starty = y;
    // owner buttons
       if(selected &&(tab->category == MISSIONSEARCH_MYARCS || line->owned || playerPtr()->access_level >= 9))
    {
        int secondsPublishBan = MissionSearch_publishWait();
        if(line->id) // published
        {
            if( drawMMButton( "MissionSearchUnpublish", "Publish_Button_inside", "Publish_Button_outside", x + buttonwd/2, y, z+1, buttonwd, sc, MMBUTTON_SMALL|(!line->owned||missionban_locksSlot(line->rating.ban)), 0  ))
            {
                char *message = line->rating.permanent
                                ? "ArchitectWarnUnpublishPermanent"
                                : "ArchitectWarnUnpublish";
                dialogStdCB(DIALOG_ACCEPT_CANCEL, message, 0, 0, s_unpublishArc, NULL, DLGFLAG_ARCHITECT, U32_TO_PTR(line->id));
                collisions_off_for_rest_of_frame = 1;
            }
            y += buttonht;
        }
        else // local
        {
            int slotsAvailable = e && e->mission_inventory && (e->mission_inventory->publish_slots > e->mission_inventory->publish_slots_used);
            if( drawMMButton( "MissionSearchPublish", "Publish_Button_inside", "Publish_Button_outside", x + buttonwd/2, y, z+1, buttonwd, sc, (((!slotsAvailable&&!e->access_level) || secondsPublishBan)?MMBUTTON_GRAYEDOUT:0)|MMBUTTON_SMALL|(!line->arc||line->failed_validation), 0 ))
            {
                if(!e || !e->pl || !AccountCanPublishOnMissionArchitect(ent_GetProductInventory( e ), e->pl->loyaltyPointsEarned, e->pl->account_inventory.accountStatusFlags))
                {
                    dialogStd(DIALOG_OK, textStd("InvalidPermissionTier7"), NULL, NULL, NULL, NULL, DLGFLAG_ARCHITECT);
                }
                else if(secondsPublishBan)
                {
                    char *errorText = 0;
                    if(!slotsAvailable)
                        errorText = "MissionSearchNoSlots";
                    else
                        errorText = "MissionSearchCantPublish";
                    dialogStd(DIALOG_OK, textStd(errorText), NULL, NULL, NULL, NULL, 0);
                    collisions_off_for_rest_of_frame = 1;
                }
                else
                {
                    // request publish
                    char *estr = estrTemp();
                    PlayerCreatedStoryArc tempArc = {0};
                    StructCopy(ParsePlayerStoryArc, line->arc, &tempArc, 0, 0);
                    s_prepareArcForSend(&tempArc);
                    ParserWriteTextEscaped(&estr, ParsePlayerStoryArc, &tempArc, 0, 0);
                    MissionSearch_requestPublish(line->id, estr);
                    missionsearch_requestTab(s_myArcsTab());
                    StructClear(ParsePlayerStoryArc, &tempArc);
                    estrDestroy(&estr);
                    collisions_off_for_rest_of_frame = 1;
                }
            }
            y += buttonht;
        }
    }
    if(line->id && line->rating.unpublished && playerPtr()->access_level)
    {
        if( drawMMButton( "MissionSearchRestore", "Publish_Button_inside", "Publish_Button_outside", x + buttonwd/2, y, z+1, buttonwd, sc, MMBUTTON_SMALL|(!!line->failed_validation), 0  ))
        {
            missionserver_restoreArc(line->id);
            missionserver_game_requestArcInfo(line->id);
            collisions_off_for_rest_of_frame = 1;
        }
        y += buttonht;
    }


    if(selected && (tab->category == MISSIONSEARCH_MYARCS || playerPtr()->access_level))
    {
        int disabled = 0;
        
        if(tab->category == MISSIONSEARCH_MYARCS)
            disabled = !((line->arc) || line->id);
        else
            disabled = !playerPtr()->access_level || line->rating.unpublished;


          if( drawMMButton("MissionSearchEdit", "Edit_Button_inside", "Edit_Button_outside", x + buttonwd/2, y, z+1, buttonwd, sc, MMBUTTON_SMALL|disabled, 0 ))
        {
            if(line->arc)
            {
                // launch editor window with this arc
                int allow_republish =    line->owned && !(line->rating.honors == MISSIONHONORS_DEVCHOICE || line->rating.honors == MISSIONHONORS_DC_AND_HOF );
                if(line->owned && (line->rating.honors == MISSIONHONORS_DEVCHOICE || line->rating.honors == MISSIONHONORS_DC_AND_HOF ))
                    dialogStdCB( DIALOG_ACCEPT_CANCEL, textStd("ArchitectEditingDevChoice"), NULL, NULL, missionsearch_allowDevChoiceEditing, NULL, 0, NULL);
                MMtoggleArchitectLocalValidation((line->id && !line->owned && playerPtr()->access_level) ? 1 : 0);
                missionMakerOpenWithArc(line->arc, line->filename, line->id, allow_republish);
                window_setMode(WDW_MISSIONSEARCH, WINDOW_SHRINKING);
                if(line->id)
                {
                    playerCreatedStoryArc_Destroy(line->arc); // clear, so we'll get an update if they republish
                    line->arc = NULL;
                }
            }
            else
            {
                missionserver_game_requestArcData_viewArc(line->id);
            }
            collisions_off_for_rest_of_frame = 1;
        }
        y += buttonht;
    }

    if(selected && (line->owned || playerPtr()->access_level))
    {
        if(missioncomment_Exists(line->id))
        {
            if( drawMMButton( "MissionSearchComplaintsButton", 0, 0, x + buttonwd/2, y, z+1, buttonwd, sc, MMBUTTON_SMALL, 0) )
            {
                missioncomment_Open(line->id);
            }
            y += buttonht;
        }
    }

    if(line->id)
    {
        int disabled = line->rating.unpublished || (!missionban_isVisible(line->rating.ban) && !playerPtr()->access_level);
        if(drawMMButton("MissionSearchPlay", "playarrow_outside_inside", "playarrow_outside", x + buttonwd/2, y, z+1, buttonwd, sc,
                        MMBUTTON_SMALL|disabled, 0 ))
        {
            // request play
            missionsearch_playPublishedArc(line->id, line->rating.honors == MISSIONHONORS_DEVCHOICE || line->rating.honors == MISSIONHONORS_DC_AND_HOF );
            collisions_off_for_rest_of_frame = 1;
        }
        y += buttonht;
    }
     else if(devassert(tab->category == MISSIONSEARCH_MYARCS) && selected)
    {
         if( drawMMButton( "MissionSearchTest", "Test_Button_inside", "Test_Button_outside", x + buttonwd/2, y, z+1, buttonwd, sc, MMBUTTON_SMALL|(!line->arc||line->failed_validation||line->rating.unpublished), 0 ))
        {
            // initiate test
            PlayerCreatedStoryArc *pArc = StructAllocRaw(sizeof(*pArc));
            StructCopyAll(ParsePlayerStoryArc, line->arc, sizeof(*pArc), pArc);
            missionMakerTestStoryArc(pArc);
            collisions_off_for_rest_of_frame = 1;
        }
        y += buttonht;

         if( line->filename && drawMMButton( "DeleteString", "delete_button_inside", "delete_button_outside", x + buttonwd/2, y, z+1, buttonwd, sc, ((!line->arc)|MMBUTTON_ERROR|MMBUTTON_SMALL ), 0 ))
        {
            s_deleteArc(line);
        }
        y += buttonht;
    }

    //finally, draw the rating.
    if(line->id)
    {
        F32 xPos;
        int i;
        char buffer[7] = {0};
        AtlasTex *star = atlasLoadTexture("Star_Selected_small.tga");
        y-=buttonht*.5 -PIX3*sc;

        xPos = x;
        font(&game_12);
        font_color(CLR_MM_TITLE_OPEN,CLR_MM_TITLE_OPEN);
        cprntEx( xPos, y+PIX3*sc, z, sc, sc, CENTER_Y, textStd("MissionSearchRated") );

        xPos += str_wd(&game_12, sc, sc, textStd("MissionSearchRated"))+3*sc*PIX3;
        xPos += star->width*.5*sc;
        for( i = 0; i < MISSIONRATING_5_STARS; i++ )
        {
            int color;    //!line->rating.honors below should be unnecessary, but it seems to be necessary on the QA shard.
            if( line->rating.honors<=MISSIONHONORS_FORMER_DC && (i >= (int)line->rating.rating || line->rating.rating == MISSIONRATING_UNRATED))
                color = 0xaaaaaaff;
            else
                color = CLR_MM_TITLE_LIT;

            display_sprite( star, xPos , y-(star->height)*.5*sc, z, sc, sc, color );
            xPos += star->width*sc;
        }
        if(line->total_votes > 999)
            sprintf(buffer, "(999+)");
        else
            sprintf(buffer, "(%d)", line->total_votes);
        cprntEx( xPos+sc*PIX3, y, z, sc, sc, CENTER_Y, buffer );
        y+= star->height*sc + PIX3*sc;
        y+=buttonht*.5;
    }
    font_outl(1);

    return y - starty;
}

F32 missionsearch_AdminButtons(MissionSearchLine *line, F32 x, F32 y, F32 z, F32 wd, F32 ht, F32 sc)
{
    static SMFBlock *smfBanMessage;

    F32 textwd, buttonwd, buttonht = 20*sc;
    F32 startx = x, starty = y;
    int access_level = playerPtr()->access_level;

    if(!line->id || !access_level)
        return 0.f;

    if(!smfBanMessage)
    {
        smfBanMessage = smfBlock_Create();
        smf_SetFlags(smfBanMessage, SMFEditMode_ReadWrite, SMFLineBreakMode_SingleLine, SMFInputMode_AnyTextNoTagsOrCodes, 512, SMFScrollMode_InternalScrolling,
            SMFOutputMode_StripAllTags, SMFDisplayMode_AllCharacters, SMFContextMenuMode_None, SMAlignment_Left, 0, 0, -1);
    }

    // header
    y += 10*sc;
    missionsearch_MissionLineBreak("Access Level Commands", x + PIX3*sc, y, z, wd - 2*PIX3*sc, sc*0.5f, 0);
    y += 18*sc*0.5f + PIX3*sc;

    // ban / approve text entry and buttons
    buttonwd = 100*sc;
    textwd = wd - 2*(buttonwd+PIX3)*sc;

    smf_SetScissorsBox(smfBanMessage, x, y, textwd, buttonht);
    s_taMissionSearchInput.piScale = (int *)((int)(1.0f*sc*SMF_FONT_SCALE));
    smf_Display(smfBanMessage, x + PIX3*sc, y, z+2, textwd - 2*PIX3*sc, 0, 0, 0, &s_taMissionSearchInput, 0);
    drawTextEntryFrame(x, y, z+1, textwd, buttonht, sc, smfBlock_HasFocus(smfBanMessage));
    x += textwd + PIX3*sc;

    if(D_MOUSEHIT == drawStdButtonTopLeft(x, y, z+1, buttonwd, buttonht, 0x662222ff, "Ban", sc,
        line->rating.ban == MISSIONBAN_CSREVIEWED_BANNED || !SAFE_DEREF(smfBanMessage->outputString)) )
    {
        missionserver_game_reportArc(line->id, kComplaint_BanMessage, smfBanMessage->outputString);
        missionserver_banArc(line->id);
        missionserver_game_requestArcInfo(line->id);
        smf_SetCursor(0,0);
    }
    x += buttonwd + PIX3*sc;

    if(D_MOUSEHIT == drawStdButtonTopLeft(x, y, z+1, buttonwd, buttonht, 0x226622ff, "Approve", sc,
        line->rating.ban == MISSIONBAN_CSREVIEWED_APPROVED) )
    {
        if(smfBanMessage->outputString)
            missionserver_game_reportArc(line->id, kComplaint_BanMessage, smfBanMessage->outputString);
        missionserver_unbannableArc(line->id);
        missionserver_game_requestArcInfo(line->id);
        smf_SetCursor(0,0);
    }
    x += buttonwd + PIX3*sc;

    y += buttonht + PIX3*sc;
    x = startx;

    // honors buttons
    if(access_level >= 9)
    {
        buttonwd = (wd - PIX3*3*sc)/5;

        if(D_MOUSEHIT == drawStdButtonTopLeft(x, y, z+1, buttonwd, buttonht, 0x222222ff, "MissionSearchNoHonors", sc,
            line->rating.honors == MISSIONHONORS_NONE) )
        {
            missionserver_changeHonors(line->id, MISSIONHONORS_NONE);
            missionserver_game_requestArcInfo(line->id);
        }
        x += buttonwd + PIX3*sc;

        if(D_MOUSEHIT == drawStdButtonTopLeft(x, y, z+1, buttonwd, buttonht, 0x222222ff, "MissionSearchHallofFame", sc,
            line->rating.honors == MISSIONHONORS_HALLOFFAME || line->rating.honors == MISSIONHONORS_DC_AND_HOF) )
        {
            missionserver_changeHonors(line->id, MISSIONHONORS_HALLOFFAME );
            missionserver_unbannableArc(line->id);
            missionserver_game_requestArcInfo(line->id);
        }
        x += buttonwd + PIX3*sc;

        if(D_MOUSEHIT == drawStdButtonTopLeft(x, y, z+1, buttonwd, buttonht, 0x222222ff, "MissionSearchCelebrity", sc,!playerPtr() || playerPtr()->access_level < 3) )
        {
            line->settingGuestAuthor = !line->settingGuestAuthor;
            if(!smfAuthorName)
                smfAuthorName = smfBlock_Create();
            if(!smfAuthorImage)
                smfAuthorImage = smfBlock_Create();
            if(!smfAuthorBio)
                smfAuthorBio = smfBlock_Create();
            smf_SetRawText( smfAuthorName, smf_EncodeAllCharactersGet(line->header.author), false );
            smf_SetRawText( smfAuthorBio,(line->bio.bio)?smf_EncodeAllCharactersGet(line->bio.bio): "", false );
            smf_SetRawText( smfAuthorImage, (line->bio.imageName)?line->bio.imageName: "", false );
        }
        x += buttonwd + PIX3*sc;

        if(D_MOUSEHIT == drawStdButtonTopLeft(x, y, z+1, buttonwd, buttonht, 0x222222ff, "MissionSearchDevChoice", sc, 
            line->rating.honors == MISSIONHONORS_DEVCHOICE || line->rating.honors == MISSIONHONORS_DC_AND_HOF ) )
        {
            missionserver_changeHonors(line->id, MISSIONHONORS_DEVCHOICE );
            missionserver_unbannableArc(line->id);
            missionserver_game_requestArcInfo(line->id);
        }
        x += buttonwd + PIX3*sc;
        
        if(D_MOUSEHIT == drawStdButtonTopLeft(x, y, z+1, buttonwd, buttonht, 0x222222ff, "MissionSaveToLocal", sc, 
            0 ) )
        {
            line->saveLocally = 1;
            missionserver_game_requestArcData_viewArc(line->id);
        }
        x += buttonwd + PIX3*sc;

        y += buttonht + PIX3*sc;
        x = startx;
    }

    return y - starty;
}


static struct {
    char *message;
//    char warned[SIZEOF2(Entity, auth_name)];
    int warned;
}
s_banwarnings[] = {
    {NULL},                            // MISSIONBAN_NONE
    {"ArchitectBanWarnSelfReview"},    // MISSIONBAN_PULLED
    {NULL},                            // MISSIONBAN_SELFREVIEWED
    {"ArchitectBanWarnCSRReview"},    // MISSIONBAN_SELFREVIEWED_PULLED
    {NULL},                            // MISSIONBAN_SELFREVIEWED_SUBMITTED
    {"ArchitectBanWarnBanned"},        // MISSIONBAN_CSREVIEWED_BANNED
    {NULL},                            // MISSIONBAN_CSREVIEWED_APPROVED
};
STATIC_ASSERT(ARRAY_SIZE(s_banwarnings) == MISSIONBAN_TYPES);

#define MAX_TITLE_LENGTH 301
#define MAX_NAME_LENGTH 23
#define DATE_POSITION 190
#define LENGTH_POSITION 285
#define FLAG_POSITION 415

static F32 s_drawLineFlags(MissionSearchLine *pLine, F32 x, F32 y, F32 z, F32 wd, F32 sc, int hover)
{
    F32 width = 0;
    F32 flag_height = 0;
    F32 height = 0, lineHeight;
    char buffer[512] = {0};
    U32 alphaMask = 0xffffffff;

    if(!hover)
        alphaMask = 0xffffff99;

    
    //first line: title
    //first line: title 
    font(&game_14);
    font_color(CLR_MM_TITLE_OPEN,CLR_MM_TITLE_OPEN);
    sprintf(buffer, "%s ", pLine->header.title);
    if(strlen(buffer)>MAX_TITLE_LENGTH)
    {
        sprintf(&buffer[MAX_TITLE_LENGTH-3], "...");
    }
    height = str_height(&game_14, sc, sc, 0, buffer)+sc*PIX3;
    //cprntEx( x, y +height, z, sc, sc, 0, buffer );
    font(&game_12);
    width = str_wd(&game_14, sc, sc, buffer);
    //cprntEx( x+width, y +height, z, sc, sc, 0, "[-]" );
    if(pLine->played)
    {
        CBox box = {0};
        AtlasTex *played = atlasLoadTexture("playedmission.tga");
        width += str_wd(&game_14, sc, sc, "[-]");
        display_sprite( played, x + width+played->width*sc*.5, y, z, sc, sc, alphaMask );
        BuildCBox(&box, x + width+played->width*sc*.5, y, played->width, played->height);

        if(mouseCollision(&box))
        {
            font_color(CLR_MM_TEXT,CLR_MM_TEXT);
            font(&game_14);
            font_outl(0);
            cprntEx( x+width+played->width*sc*.5+played->width, y +height, z, sc, sc, 0, textStd("MissionSearchHasPlayed" ));
            font_outl(1);
        }
    }

    lineHeight = str_height(&game_14, sc, sc, 0, buffer)+PIX3;
    flag_height = lineHeight*.5 + height;
    height +=  lineHeight;

    //published specific stuff
    if( pLine->id && pLine->date_created && s_MissionSearch.locale_id != 1<<locGetIDInRegistry())
    {
        AtlasTex *flag = 0;
        if(pLine->header.locale_id == LOCALE_ID_ENGLISH)
            flag = atlasLoadTexture("USUK_Flag");
        else if(pLine->header.locale_id == LOCALE_ID_FRENCH)
            flag = atlasLoadTexture("France_Flag");
        else if(pLine->header.locale_id == LOCALE_ID_GERMAN)
            flag = atlasLoadTexture("German_Flag");
        else
            flag = atlasLoadTexture("USUK_Flag");

        if(flag)
            display_sprite( flag, x + FLAG_POSITION*sc, y +flag_height - flag->height*.5*sc + sc*PIX3, z, sc, sc, alphaMask );
    }
    return lineHeight;
}

static int s_drawMultiColorLine(char *line, F32 x, F32 y, F32 z, F32 sc, int color1, int color2, char start, char stop, TTDrawContext *font)
{
    char *temp = estrTemp();
    char *position = 0;
    F32 origX = x;
    estrPrintCharString(&temp, line);
    position = temp;

    while(position)
    {
        char * startPos = strchr(position, start);
        char *stopPos = (startPos)?strchr(startPos, stop):0;

        if(!startPos)
        {
            font_color(color1,color1);
            cprntEx( x, y , z, sc, sc, 0, position );
            x+=str_wd(font, sc, sc, position);
        }
        else
        {
            startPos[0] = 0;
            if(stopPos)
                stopPos[0] = 0;
            font_color(color1,color1);
            cprntEx( x, y , z, sc, sc, 0, position );
            x += str_wd(font, sc, sc, position);
            position = startPos+1;
            font_color(color2,color2);
            cprntEx( x, y , z, sc, sc, 0, position );
            x += str_wd(font, sc, sc, position);
        }

        position = (stopPos)?stopPos+1:0;
    }
    estrDestroy(&temp);
    return x-origX;
}

static int s_drawLineHeader(MissionSearchLine *pLine, F32 x, F32 y, F32 z, F32 wd, F32 sc, int open)
{
    F32 width = 0;
    F32 height = 0;
    F32 lineHeight;
    F32 byWidth = 0;
    char buffer[512] = {0};
    static SMFBlock *block = 0;
    int i;
    UIBox box;
    CBox author_box;

    if(!block)
        block = smfBlock_Create();
    
    font(&game_14);
    font_color(CLR_MM_TITLE_OPEN,CLR_MM_TITLE_OPEN);

     //first line: title
      sprintf(buffer, "%s ", smf_DecodeAllCharactersGet(pLine->header.title));
    if(strlen(buffer)>MAX_TITLE_LENGTH)
    {
        sprintf(&buffer[MAX_TITLE_LENGTH-3], "...");
    }
    height = str_height(&game_14, sc, sc, 0, buffer)+sc*PIX3;
    width = str_wd(&game_14, sc, sc, buffer);
      uiBoxDefine(&box, x, 0, wd - 40*sc, 50000);
    clipperPushRestrict(&box);
    if(pLine->formattedTitle)
        width = s_drawMultiColorLine(pLine->formattedTitle,  x, y+height , z, sc, CLR_MM_TITLE_OPEN, 0xffa500ff, SEARCH_HIGHLIGHTED_ENCODED_START, SEARCH_HIGHLIGHTED_ENCODED_END, &game_14);
    else
    {
        cprntEx( x, y +height, z, sc, sc, 0, buffer );
    }
    clipperPop();
    
    font(&game_12);
    font_outl(0);
    if(open)
        cprntEx( x+width, y +height-sc*PIX2, z, sc, sc, 0, "[-]" );
    else
        cprntEx( x+width, y +height-sc*PIX2, z, sc, sc, 0, "[+]" );
    font_outl( 1 );

    //second line: author OR filename
    if(pLine->id)    //TODO: switch to different colored fonts.
    {
        font_ital( 1 );
        sprintf(buffer, "%s", textStd("MissionSearchByLine", (pLine->formattedAuthor)?pLine->formattedAuthor:pLine->header.author));
    }
    else
        sprintf(buffer, "%s: %s", textStd("MissionSearchFileLine"), pLine->filename);

    //if open, truncate if necessary.
    if(!open)
    {
        width = str_wd(&game_12, sc, sc, buffer);
        if(width + PIX3*sc> sc*DATE_POSITION)
            sprintf(&buffer[MAX_NAME_LENGTH], "...");
        if(strlen(buffer)>MAX_TITLE_LENGTH)
        {
            sprintf(&buffer[MAX_TITLE_LENGTH-3], "...");
        }
    }
    lineHeight = str_height(&game_12, sc, sc, 0, buffer)+PIX3;
    height += lineHeight +sc*PIX3;

     font_outl(0);
    if(pLine->formattedAuthor && pLine->id)
    {
        int authorBaseColor = (pLine->rating.honors == MISSIONHONORS_CELEBRITY)?CLR_MM_TITLE_LIT:CLR_MM_TITLE_OPEN;
        cprntEx( x, y+height , z, sc, sc, 0,"MissionSearchBy");
        byWidth = str_wd(&game_12, sc, sc, "MissionSearchBy");
        byWidth += s_drawMultiColorLine(pLine->formattedAuthor, byWidth+x, y+height, z+1, sc, authorBaseColor, 0xffa500ff, SEARCH_HIGHLIGHTED_ENCODED_START, SEARCH_HIGHLIGHTED_ENCODED_END, &game_12);
    }
    else
    {
        cprntEx( x, y+height , z, sc, sc, 0, buffer );
        byWidth = str_wd(&game_12, sc, sc, buffer);
    }

       BuildCBox( &author_box, x, y+height-15*sc, byWidth-5*sc, 15*sc ); 
    //drawBox(&author_box, z, CLR_RED, 0 );
    if( mouseCollision(&author_box) && pLine->id )
    {
        // underline
        drawHorizontalLine( x, y+height, byWidth-8*sc, z, sc, 0xffa500ff );
        if( mouseClickHit(&author_box, MS_LEFT) )
        {
            MissionSearch_requestSearch( MISSIONSEARCH_ARC_AUTHOR, 0, 0, 0, 0, pLine->id );
            collisions_off_for_rest_of_frame = 1;
            s_searchByAuthor = strdup(pLine->header.author);
            missionsearch_UpdateSearchParams(0);
        }
    }

    font_outl( 1 );
    font_ital( 0 );

    //published specific stuff
    if( pLine->id && pLine->date_created && !open)
    {
        char *spacePosition = 0;
        char *pchText = 0;
        F32 tempWd = 0;
        AtlasTex *flag = 0;
        
        for( i = ARRAY_SIZE(s_lengthList)-1; i >=0; i-- )
        {
            if( pLine->header.arcLength & (1<<s_lengthList[i].ID) )
                pchText = textStd(s_lengthList[i].name);
        }

        font_color(CLR_MM_TEXT,CLR_MM_TEXT);
        sprintf(buffer, "%s", timerMakeDateStringFromSecondsSince2000NoSeconds(pLine->date_created));
        spacePosition = strchr(buffer, ' ');    //remove the space.
        if(spacePosition)
            spacePosition[0] = 0;

        cprntEx( x+ DATE_POSITION*sc, y +height, z, sc, sc, 0, buffer );

        sprintf(buffer, "%s: ", textStd("MissionSearchLengthOnly"));

        font_color(CLR_MM_TITLE_OPEN,CLR_MM_TITLE_OPEN);
        cprntEx( x+ LENGTH_POSITION*sc, y +height, z, sc, sc, 0, buffer );
        tempWd = str_wd(&game_12, sc, sc, buffer);
        font_outl(0);
        sprintf(buffer, "%s", pchText);
        font_color(CLR_MM_TEXT,CLR_MM_TEXT);
        cprntEx( x+ LENGTH_POSITION*sc + tempWd, y +height, z, sc, sc, 0, buffer );
        height += str_height(&game_12, sc, sc, 0, buffer)+ PIX3*sc;
        font_outl( 1 );
    }

    if(pLine->bio.bio && open)
    {
        static SMFBlock *smfTitle = 0;
        AtlasTex *bioImage = 0;
        if(!smfTitle)
        {
            smfTitle = smfBlock_Create();
        }
        smf_SetFlags(smfTitle, SMFEditMode_ReadWrite, SMFLineBreakMode_MultiLineBreakOnWhitespace, 0, 128, SMFScrollMode_InternalScrolling, 
            SMFOutputMode_StripAllTags, SMFDisplayMode_AllCharacters, SMFContextMenuMode_None, SMAlignment_Left, 0, 0, -1);

        //check for the actual image.  If it's there, use it.  Otherwise...
        if(pLine->bio.imageName)
            bioImage = atlasLoadTexture(pLine->bio.imageName);
        else
            bioImage = atlasLoadTexture("GuestAuthor_64x64_Male");
        display_sprite( bioImage, x, y+height+2*PIX3*sc, z, sc, sc, 0xffffffff );
        if(pLine->estrHeader)
            smf_SetRawText( smfTitle, pLine->estrHeader, false );
        else
            smf_SetRawText( smfTitle,"", false );
        height +=MAX(smf_Display(smfTitle, x+(bioImage->width+2*PIX3)*sc, y+height+PIX3*sc, z, wd-(bioImage->width+PIX3)*sc, 0, 0, 0, &s_taMissionSearch, 0),
                        (bioImage->height+2*PIX3)*sc);
    }
        
    return height+PIX3*sc;
}

static F32 s_drawMatch(MissionSearchLine *line,F32 x,F32 y,F32 z,F32 sc, char *text, int on, int comma)
{
    if(on)
        font_color(0xffa500ff,0xffa500ff);
    else
        font_color(CLR_TAB_INACTIVE,CLR_TAB_INACTIVE);

    cprntEx( x, y , z, sc, sc, 0, text );
    x+= str_wd(&game_9, sc, sc, text);
    if(comma)
    {
        cprntEx( x, y , z, sc, sc, 0, ",");
        x+= str_wd(&game_9, sc, sc, ", ");
    }
    return x+ sc*PIX3;
}

static F32 s_drawMatches(MissionSearchLine *line,F32 x,F32 y,F32 z,F32 sc)
{
    F32 height = str_height(&game_9, sc, sc, 0, textStd("MissionSearchMatches"))+2*sc*PIX3;
    font(&game_9);
    y+=height;

    font_color(CLR_MM_TITLE_LIT,CLR_MM_TITLE_LIT);
    //Search Matches:
    cprntEx( x, y , z, sc, sc, 0, textStd("MissionSearchMatches") );
    x+=str_wd(&game_9, sc, sc, textStd("MissionSearchMatches"))+PIX3*sc;
    //Title,
    x = s_drawMatch(line,x,y, z, sc, textStd("MissionSearchMatchesTitle"), line->titleMatch, 1);
    
    //Author,
    x = s_drawMatch(line,x,y, z, sc, textStd("MissionSearchMatchesAuthor"), line->authorMatch, 1);

    //Arc ID,
    x = s_drawMatch(line,x,y, z, sc, textStd("MissionSearchMatchesId"), line->idMatch, 1);

    //Enemy Group
    x = s_drawMatch(line,x,y, z, sc, textStd("MissionSearchMatchesEnemy"), line->villainMatch, 1);

    //Description
    x = s_drawMatch(line,x,y, z, sc, textStd("MissionSearchMatchesDescription"), line->summaryMatch, 0);

    return height;
}

F32 missionsearch_SetGuestAuthorUi(MissionSearchLine *line, F32 x, F32 y, F32 z, F32 wd, F32 ht, F32 sc)
{
    int readyToUpdate = 0;
    F32 yStart = y;
    int titleWd;

    if(!smfAuthorName)
        smfAuthorName = smfBlock_Create();
    if(!smfAuthorImage)
        smfAuthorImage = smfBlock_Create();
    if(!smfAuthorBio)
        smfAuthorBio = smfBlock_Create();

    font( &game_12 );
    font_color(CLR_MM_TEXT, CLR_MM_TEXT);

    titleWd = str_wd(&game_12, sc, sc, "GuestAuthorName");
    titleWd = MAX(titleWd, str_wd(&game_12, sc, sc, "GuestAuthorImage"));
    titleWd = MAX(titleWd, str_wd(&game_12, sc, sc, "GuestAuthorBio"));
    titleWd += PIX3;
    
    //Name
    cprntEx( x, y+20, z+2, sc, sc, 0, "GuestAuthorName");
    drawTextEntryFrame(titleWd+ x+PIX3, y, z+2, wd-PIX3-titleWd, 20, sc, 0 );
    smf_SetFlags(smfAuthorName, SMFEditMode_ReadWrite, SMFLineBreakMode_SingleLine, 0, 128, SMFScrollMode_InternalScrolling, 
        SMFOutputMode_StripAllTags, SMFDisplayMode_AllCharacters, SMFContextMenuMode_None, SMAlignment_Left, 0, 0, -1); 
    smf_SetScissorsBox(smfAuthorName, titleWd+x+PIX3, y, wd-titleWd-PIX3, 20 );
    smf_Display(smfAuthorName, titleWd+x+PIX3, y, z+2, wd-PIX3-titleWd, 0, 0, 0, &s_taMissionSearchInput, 0);
    smf_ClearScissorsBox(smfAuthorName);
    y+= 20 + PIX3;

    //Image
    
    cprntEx( x, y+20, z+2, sc, sc, 0, "GuestAuthorImage");
    drawTextEntryFrame( x+PIX3+titleWd, y, z+2, wd-PIX3-titleWd, 20, sc, 0 );
    smf_SetFlags(smfAuthorImage, SMFEditMode_ReadWrite, SMFLineBreakMode_SingleLine, 0, 128, SMFScrollMode_InternalScrolling, 
        SMFOutputMode_StripAllTags, SMFDisplayMode_AllCharacters, SMFContextMenuMode_None, SMAlignment_Left, 0, 0, -1); 
    smf_SetScissorsBox(smfAuthorImage, titleWd+x+PIX3, y, wd-titleWd-PIX3, 20 );
    smf_Display(smfAuthorImage, x+PIX3+titleWd, y, z+2, wd-PIX3-titleWd, 0, 0, 0, &s_taMissionSearchInput, 0);
    smf_ClearScissorsBox(smfAuthorImage);
    y+= 20 + PIX3;

    //Bio
    cprntEx( x, y+20, z+2, sc, sc, 0, "GuestAuthorBio");
    drawTextEntryFrame( x+PIX3+titleWd, y, z+2, wd-PIX3-titleWd, 100, sc, 0 );
    smf_SetFlags(smfAuthorBio, SMFEditMode_ReadWrite, SMFLineBreakMode_MultiLineBreakOnWhitespace, 0, 99999999, SMFScrollMode_InternalScrolling, 
        SMFOutputMode_StripAllTags, SMFDisplayMode_AllCharacters, SMFContextMenuMode_None, SMAlignment_Left, 0, 0, -1); 
    smf_SetScissorsBox(smfAuthorBio, titleWd+x+PIX3, y, wd-titleWd-PIX3, 100 );
    smf_Display(smfAuthorBio, x+PIX3+titleWd, y, z+2, wd-PIX3-titleWd, 100, 0, 0, &s_taMissionSearchInput, 0);
    smf_ClearScissorsBox(smfAuthorBio);
    y+= 100 + PIX3;

    if( smfAuthorName->outputString && strlen(smfAuthorName->outputString) &&
        smfAuthorImage->outputString && strlen(smfAuthorImage->outputString) &&
        smfAuthorBio->outputString && strlen(smfAuthorBio->outputString))
    {
        readyToUpdate = 1;
    }

    if(D_MOUSEHIT == drawStdButtonTopLeft(x, y, z+1, 200, 20, 0x222222ff, "GuestAuthorSubmit", sc,
        !readyToUpdate) )
    {
        char *name = estrTemp();
        char *bio = estrTemp();
        char *image = estrTemp();

        estrPrintEString(&name, smf_DecodeAllCharactersGet(smfAuthorName->outputString));
        estrPrintEString(&bio, smf_DecodeAllCharactersGet(smfAuthorBio->outputString));
        estrPrintEString(&image, smfAuthorImage->outputString);
        line->settingGuestAuthor = 0;
        missionserver_setGuestBio(line->id, name, bio, image);
        estrDestroy(&name);
        estrDestroy(&bio);
        estrDestroy(&image);
        missionserver_game_requestArcInfo(line->id);
    }
    y+=20+PIX3;

    return y-yStart;
}

int missionsearch_MissionLine(MissionSearchLine *line, int selected, float x, float *y, float z, float wd, float sc)
{
    int ret = 0;
    float ht = 0.f, htadmin = 0.f;
    int color = 0x224e4eff;
    int borderColor = CLR_MM_TEXT;
    int hover = 0;

     MissionSearchTab *tab = &s_categoriesList[s_categoryCurrent];
    AtlasTex *white = atlasLoadTexture("white");
    CBox box;
    char * str = selected?line->estrFull:0;
    F32 matchHeight = 0;

    if(line->section_header)
    {
        missionsearch_MissionLineBreak(line->section_header, x, *y, z, wd, sc, 0);
        *y += 18*sc;
        return 0;
    }

    if(line->idMatch || line->authorMatch || line->titleMatch || line->villainMatch || line->summaryMatch)
    {
        matchHeight = s_drawMatches(line,x+20*sc,*y,z+2,sc);
        matchHeight+= PIX3*sc;
        //matchHeight+= PIX3*sc;
        ht+= matchHeight;
    }

     s_taMissionSearch.piScale = (int *)((int)(1.0f*sc*SMF_FONT_SCALE));
    if(selected && str)
    {
        F32 yOffset = s_drawLineHeader(line, x+20*sc, matchHeight +*y+PIX3*sc, z+2, wd-220*sc, sc, selected);
         ht += PIX3*sc + yOffset + smf_ParseAndDisplay( line->pBlock, str, x+20*sc, matchHeight+*y+ yOffset+PIX3*sc, z+2, wd-220*sc, 0, 0, 0, &s_taMissionSearch, NULL, 0, 0);
        
    }
    else
        ht += PIX3*sc + s_drawLineHeader(line, x+20*sc, ht+*y+PIX3*sc, z+2, wd-220*sc, sc, selected);
      if(selected)
        MAX1(ht, 180*sc);

    if(selected && line->owned && line->id && !line->arc) // mine, published, and not loaded
    {
        if(line->requested + MISSIONSEARCH_ARCREQUESTTIME < timerSecondsSince2000())
        {
            // request info
            missionserver_game_requestArcData_viewArc(line->id);
            line->requested = timerSecondsSince2000();
        }
        // Loading Progress Bar?
    }

    if(    line->owned && s_banwarnings[line->rating.ban].message && s_banwarnings[line->rating.ban].warned != playerPtr()->db_id )
    {
        dialogStd(DIALOG_OK, s_banwarnings[line->rating.ban].message, 0, 0, 0, 0, DLGFLAG_ARCHITECT);
        s_banwarnings[line->rating.ban].warned = playerPtr()->db_id;
    }

    if(selected || line->id)
    {
        float ht_butt = matchHeight+ missionsearch_LineButtons(line, x + wd - 220*sc, matchHeight+*y, z, 200*sc, ht, sc, selected );
        MAX1(ht, ht_butt);
        if(selected)
            ht += htadmin = missionsearch_AdminButtons( line, x+10*sc, *y+ht, z, wd-2*10*sc, ht, sc ); 
    }
    ht += sc*PIX3;
    if(line->settingGuestAuthor)
    {
        ht+= sc*PIX3 + missionsearch_SetGuestAuthorUi( line, x+10*sc, *y+ht, z, wd-2*10*sc, ht, sc ); 
    }


    if(line->rating.invalidated)
        borderColor = 0x76000088;
    else if(!missionban_isVisible(line->rating.ban))
        borderColor = 0x76000088;
    else if(line->rating.honors == MISSIONHONORS_CELEBRITY)
        borderColor = CLR_MM_GUESTAUTHOR;
    else if(line->rating.honors == MISSIONHONORS_DEVCHOICE || line->rating.honors == MISSIONHONORS_DC_AND_HOF)
        borderColor = CLR_MM_SEARCH_DEVCHOICE;
    else if(line->rating.honors == MISSIONHONORS_HALLOFFAME)
        borderColor = CLR_MM_SEARCH_HALLOFFAME;
    else if(line->rating.unpublished)
        borderColor = 0x00000088;
    

    BuildCBox(&box, x, *y, wd, ht);
    hover = mouseCollision(&box);
    if(selected || hover)
    {
        if( color == 0x224e4eff ) 
            color = 0x2d5f5fff;
        else
            color |= 0x99;
    }
    s_drawLineFlags(line, x+20*sc,  matchHeight+*y+PIX3*sc, z+2, wd-220*sc, sc, hover);

    BuildCBox(&box, x, *y, wd, ht-htadmin);
    if(mouseClickHit(&box, MS_LEFT) && !line->settingGuestAuthor)
        ret = 1;

    if(!line->id && !line->arc)
        color = 0xff4444cc;

    if(matchHeight)
        drawWaterMarkFrameCorner( x, *y, z, sc, wd, ht, matchHeight-sc*PIX3, wd-220*sc, color&0xffffffbb, color&0xffffff00, borderColor, 0);
    else
        drawWaterMarkFrame_ex( x, *y, z, sc, wd, ht, color&0xffffffbb, color&0xffffff00,borderColor, 0);
      //display_sprite_blend(white, x, *y, z, wd/white->width, ht/white->height, color&0xffffff66, color&0xffffff04, color&0xffffff04, color&0xffffff66 );

    *y += ht + PIX3*sc;

    return ret;
}

#define SEARCH_TAB_RESIZE (.9)
#define TAB_WD 20*SEARCH_TAB_RESIZE
#define TAB_HT 40*SEARCH_TAB_RESIZE
#define REFINE_B_WD 180*sc
#define REFINE_B_HT 30*sc
#define COMBOS_WIDTH(nCombos) (UI_MSSEARCH_COMBO_W*nCombos + 7*PIX3*sc)
#define SEARCH_PANE_WIDTH ((REFINE_B_WD)*2.75)

static int missionsearc_NavTab( MissionSearchTab *tab, F32 x, F32 y, F32 z, F32 wd, F32 ht, F32 sc, int selected )
{
    static AtlasTex * corner, *side, *top, *frame, *frame_side, *frame_top, *back;
    static int init;
       int clr = 0x00ffff05, txt_clr = 0x8fc867ff;
     AtlasTex *icon = atlasLoadTexture(tab->icon);
    static SMFBlock *smfTitle = 0;
    CBox box;
    char * estr = estrTemp();

     if(!smfTitle)
    {
        smfTitle = smfBlock_Create(); 
    }
    smf_SetFlags(smfTitle, SMFEditMode_Unselectable, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );

      if(selected)
    {
        txt_clr = CLR_MM_TITLE_OPEN;
        clr = 0x00ffff11;
    }

    BuildCBox(&box, x, y, wd, ht);
    if( mouseCollision(&box) )
    {
        clr = 0x00ffff33;
        txt_clr = CLR_MM_TITLE_LIT;
    }

    if(!init)
    {
        corner = atlasLoadTexture("TabOverlap_36px_corner");
        frame = atlasLoadTexture("TabOverlap_36px_frame_edge");
        frame_side = atlasLoadTexture("TabOverlap_36px_frame_mid_side");
        frame_top = atlasLoadTexture("TabOverlap_36px_frame_mid_top");
        side = atlasLoadTexture("TabOverlap_36px_mid_side");
        top = atlasLoadTexture("TabOverlap_36px_mid_top"); 
        back = atlasLoadTexture("white.tga");
    }

    display_spriteFlip(corner, x, y, z, sc, sc, clr, FLIP_HORIZONTAL);
    display_spriteFlip(frame, x, y, z, sc, sc, clr, FLIP_HORIZONTAL );
    display_sprite(back, x + corner->width*sc, y+ PIX2*sc, z, (wd-2*corner->width*sc)/back->width, (corner->height*sc-PIX2*sc)/back->height, clr );
    display_sprite(back, x+2*sc, y + corner->height*sc, z, (wd-4*sc)/back->width, (1*sc)/back->height, clr );
   //    display_sprite(frame_top, x + frame->width, y, z, (wd-2*frame->width*sc)/frame_top->width, sc, clr );
     display_sprite(corner, x+wd-corner->width*sc, y, z, sc, sc, clr );
    display_sprite(frame, x+wd-frame->width*sc, y, z, sc, sc, clr );

      display_sprite( icon, x + corner->width*sc - icon->width*sc/2, y + ht/2 - icon->height*sc/2+4*sc, z, sc, sc, txt_clr );

      s_taMissionSearch.piScale = (int *)((int)(1.0f*sc*SMF_FONT_SCALE));

    estrConcatf(&estr, "<font face=title color=#%06x><i>%s</i></font>", 0x00ffffff&(txt_clr>>8), textStd(tab->name) );
    smf_SetRawText( smfTitle, estr, false );
     smf_Display(smfTitle, x + corner->width*sc + icon->width*sc/2, y+5*sc, z+2, wd-2*corner->width*sc-icon->width*sc/2, 0, 0, 0, &s_taMissionSearch, 0);
    estrDestroy(&estr);

    return mouseClickHit(&box, MS_LEFT);
}

static int s_allowSearch = 0;
static SMFBlock *smfError = 0;
#define MMSEARCH_TOGGLE_SCALE .8
static F32 s_drawToggleButton(float x, float y, float z, float sc, MissionSearchComboItem *item1, MissionSearchComboItem *item2, U32 *filter, U32 *option)
{
    MissionSearchComboItem *itemChosen;
    U32 toggleMask = (1<<item1->ID)|(1<<item2->ID);
    int selected = ((*filter)&toggleMask);
    float wd, ht, startX = x;
    if(selected)
        (*option) = selected;
    else if(!((*option)&toggleMask))
        (*option) = (1<<item1->ID);

    drawMMCheckBox(x, y, z+1, sc, "", &selected, CLR_MM_TITLE_OPEN, CLR_MM_BUTTON,
        CLR_MM_TITLE_LIT, CLR_MM_BUTTON_LIT, &wd, &ht);
    x+=sc*(20+PIX3);

    if(*option == (1<<item2->ID))
        itemChosen = item2;
    else
        itemChosen = item1;

    if( drawMMButton( itemChosen->name, 0, 0, x+.5*MISSIONSEARCH_BUTTONWD, y +(MMSEARCH_TOGGLE_SCALE*REFINE_B_HT-ht/2)/2, z, MISSIONSEARCH_BUTTONWD, MMSEARCH_TOGGLE_SCALE*sc, MMBUTTON_SMALL|((!selected)?MMBUTTON_GRAYEDOUT:0), 0  ) )
        (*option) = toggleMask&~(1<<itemChosen->ID);

    (*filter)&=~toggleMask;
    if(selected)
    {
        (*filter)|=(*option);
    }
    x+=MISSIONSEARCH_BUTTONWD+sc*PIX3;
    return x-startX;
}

static F32 s_drawToggleCheckBox(float x, float y, float z, float sc, MissionSearchComboItem *item1, MissionSearchComboItem *item2, U32 *filter, U32 *option)
{
    U32 toggleMask = (1<<item1->ID)|(1<<item2->ID);
    int selected1 = !!((*filter)&(1<<item1->ID));
    int selected2 = !!((*filter)&(1<<item2->ID));
    float wd, ht, startX = x, startY = y;


    if(drawMMCheckBox(x, y, z+1, sc, textStd(item1->name), &selected1, CLR_MM_TITLE_OPEN, CLR_MM_BUTTON,
        CLR_MM_TITLE_LIT, CLR_MM_BUTTON_LIT, &wd, &ht))
        selected2=(selected1)?0:selected2;
    y+=ht+sc*PIX3;

    if(drawMMCheckBox(x, y, z+1, sc, textStd(item2->name), &selected2, CLR_MM_TITLE_OPEN, CLR_MM_BUTTON,
        CLR_MM_TITLE_LIT, CLR_MM_BUTTON_LIT, &wd, &ht))
        selected1=(selected2)?0:selected1;
    x+=wd+sc*PIX3;

    if(selected1)
        (*option) = (1<<item1->ID);
    else if(selected2)
        (*option) = (1<<item2->ID);
    else
        (*option) = 0;

    (*filter)&=~toggleMask;
    if((*option))
    {
        (*filter)|=(*option);
    }
    x+=sc*PIX3;

    drawFlatFrame( PIX2, R10, startX-sc*PIX3, startY-sc*PIX2, z, x-startX+sc*PIX2, y-startY + 20*sc, sc, CLR_MM_BACK_ALTERNATE, CLR_MM_BACK_ALTERNATE );
    return x-startX;
}

//this will clear out any other fields set previously.
static F32 s_drawSingleToggleCheckBox(float x, float y, float z, float sc, MissionSearchComboItem *item1, U32 *filter, U32 mask)
{
    int selected = !!((*filter)&(1<<item1->ID));
    float wd, ht, startX = x;


    drawMMCheckBox(x, y, z+1, sc, textStd(item1->name), &selected, CLR_MM_TITLE_OPEN, CLR_MM_BUTTON,
        CLR_MM_TITLE_LIT, CLR_MM_BUTTON_LIT, &wd, &ht);
    x+=wd+sc*PIX3;
    
    (*filter)&=~mask;
    if(selected)
    {
        (*filter)|=(1<<item1->ID);
    }
    return x-startX;
}

static int missionsearch_displayRefinements( float *x, float *y, float z, float wd, float *tx, float *ty, float *expandibleHeight, float *descHt, CBox *box, float sc )
{
    F32 searchInputWd, availableSpace, text_ht = 22*sc, txt_wd = wd/3;
    F32 refinementTabWidth = wd - 2*(PIX3*sc+ R10*sc);
    F32 refinementEntryX;
    F32 refinementEntryEdge = *x +refinementTabWidth - MISSIONSEARCH_BUTTONWD;
    F32 spaceForCombos;
    F32 comboScale = 1;
    F32 buttonX = *x+refinementTabWidth - MISSIONSEARCH_BUTTONWD*.5;
    int update = 0, i;
    static int advancedSearch = 0;

    s_taMissionSearchInput.piScale = (int *)((int)(1.0f*sc*SMF_FONT_SCALE));
    *y += 30*sc;
    *tx = *x;

    searchInputWd = SEARCH_PANE_WIDTH;
    availableSpace = refinementTabWidth-(2*PIX3*sc+ 10*sc);
    if(availableSpace < searchInputWd)
        searchInputWd = availableSpace;
    refinementEntryX = (*box).lx + PIX3*sc + 10*sc;
    smf_SetFlags(smfSearch, SMFEditMode_ReadWrite, SMFLineBreakMode_SingleLine, 0, 128, SMFScrollMode_InternalScrolling, 
        SMFOutputMode_StripAllTags, SMFDisplayMode_AllCharacters, SMFContextMenuMode_None, SMAlignment_Left, 0, 0, -1); 
    smf_SetScissorsBox(smfSearch, refinementEntryX+PIX3, *y+*descHt-REFINE_B_HT/2, searchInputWd, text_ht );
    smf_Display(smfSearch, refinementEntryX+PIX3, *y+*descHt+text_ht-REFINE_B_HT - 2*sc, z+2, searchInputWd, 0, 0, 0, &s_taMissionSearchInput, 0);
    if(smfBlock_HasFocus(smfSearch))
    {
        KeyInput* input;

        // Handle keyboard input.
        input = inpGetKeyBuf();

        while(input )
        {
            if(input->type == KIT_EditKey)
            {
                switch(input->key)
                {
                    xcase INP_ESCAPE:
                //smf_SetRawText(smfSearch, "", false);
                smf_SetCursor(smfSearch, 0);
                xcase INP_RETURN:
                    case INP_NUMPADENTER:
                        update = 1;
                    default:
                        break;
                }
            }
            else
            {
                //uiFormattedEdit_DefaultKeyboardHandler(edit, input);
            }
            inpGetNextKey(&input);
        }
    }

    drawTextEntryFrame( refinementEntryX, *y+*descHt-REFINE_B_HT/2, z, searchInputWd, text_ht, sc, 0 );

    *tx += txt_wd;
    spaceForCombos = refinementEntryEdge - refinementEntryX;

    if( drawMMButton( "MissionSearchUpdate", "Search_Button_inside", "Search_Button_outside", buttonX, *y + 10*sc+*descHt-REFINE_B_HT/2, z, MISSIONSEARCH_BUTTONWD, sc, MMBUTTON_SMALL, 0  ) )
        update = 1;
    if(advancedSearch)
    {
        if( drawMMButton( "MissionSearchRandom", 0,0, buttonX, *y + 45*sc+*descHt-REFINE_B_HT/2, z, MISSIONSEARCH_BUTTONWD, sc, MMBUTTON_SMALL, 0  ) )
        {
            update = 1;
            s_viewedFilter |= 1<<kMissionSearchViewedFilter_Random;
        }
        if( drawMMButton( "MissionSearchClear", 0, 0, buttonX-37.5*sc, *y + 77.5*sc+*descHt-REFINE_B_HT/2, z, MISSIONSEARCH_BUTTONWD*.50, sc*.75, MMBUTTON_SMALL|MMBUTTON_ERROR, 0  ) )
            missionsearch_UpdateSearchParams(1);
        if( drawMMButton( "MissionSearchLess", 0, 0, buttonX+37.5*sc, *y + 77.5*sc+*descHt-REFINE_B_HT/2, z, MISSIONSEARCH_BUTTONWD*.50, sc*.75, MMBUTTON_SMALL, 0  ) )
            advancedSearch = !advancedSearch;

        if(COMBOS_WIDTH(5)*sc > spaceForCombos)
            comboScale =  spaceForCombos/(COMBOS_WIDTH(5)*sc);

        // Comboboxes - I hate these
        //            MS: They feel the same way about us.
        s_ratingCombo.wdw = WDW_MISSIONSEARCH;
        s_ratingCombo.x = 20;
        s_ratingCombo.y = 110+(*descHt-REFINE_B_HT/2)/sc;
        s_ratingCombo.wd = UI_MSSEARCH_COMBO_W*comboScale;
        if(SAFE_MEMBER(s_ratingCombo.elements[0],selected ))
            s_ratingCombo.architect =COMBOBOX_ARCHITECT_UNSELECTED;
        else
            s_ratingCombo.architect =COMBOBOX_ARCHITECT_SELECTED;
        combobox_display(&s_ratingCombo);

        s_lengthCombo.x = s_ratingCombo.wd + s_ratingCombo.x + 2*PIX3;
        s_lengthCombo.y = 110+(*descHt-REFINE_B_HT/2)/sc;
        s_lengthCombo.wd = UI_MSSEARCH_COMBO_W*comboScale;
        if(SAFE_MEMBER(s_lengthCombo.elements[0],selected ))
            s_lengthCombo.architect =COMBOBOX_ARCHITECT_UNSELECTED;
        else
            s_lengthCombo.architect =COMBOBOX_ARCHITECT_SELECTED;
        combobox_display(&s_lengthCombo);

        s_alignmentCombo.x = s_lengthCombo.wd + s_lengthCombo.x + 2*PIX3;
        s_alignmentCombo.y = 110+(*descHt-REFINE_B_HT/2)/sc;
        s_alignmentCombo.wd = UI_MSSEARCH_COMBO_W*comboScale;
        if(SAFE_MEMBER(s_alignmentCombo.elements[0],selected ))
            s_alignmentCombo.architect =COMBOBOX_ARCHITECT_UNSELECTED;
        else
            s_alignmentCombo.architect =COMBOBOX_ARCHITECT_SELECTED;
        combobox_display(&s_alignmentCombo); 

        s_localeCombo.x = s_alignmentCombo.wd + s_alignmentCombo.x + 2*PIX3;
        s_localeCombo.y = 110+(*descHt-REFINE_B_HT/2)/sc;
        s_localeCombo.wd = UI_MSSEARCH_COMBO_W*comboScale;
        {
            int defaultSelection = 1;
            int defaultId = locGetIDInRegistry();
            if(s_localeCombo.elements[0]->selected)
                defaultSelection =0;
            for(i = 1; i < eaSize(&s_localeCombo.elements) &&defaultSelection;i++)
            {
                if(s_localeCombo.elements[i]->selected && s_localeCombo.elements[i]->id!=defaultId)
                    defaultSelection=0;
            }

            if(defaultSelection)
                s_localeCombo.architect =COMBOBOX_ARCHITECT_UNSELECTED;
            else
                s_localeCombo.architect =COMBOBOX_ARCHITECT_SELECTED;
        }
        combobox_display(&s_localeCombo);

        s_statusCombo.x = s_localeCombo.wd + s_localeCombo.x + 2*PIX3;
        s_statusCombo.y = 110+(*descHt-REFINE_B_HT/2)/sc;
        s_statusCombo.wd = UI_MSSEARCH_COMBO_W*comboScale;
        if(SAFE_MEMBER(s_statusCombo.elements[0],selected ))
            s_statusCombo.architect =COMBOBOX_ARCHITECT_UNSELECTED;
        else
            s_statusCombo.architect =COMBOBOX_ARCHITECT_SELECTED;
        combobox_display(&s_statusCombo);


        {
            float tempX = refinementEntryX;
            float tempY = *y + 60*sc+*descHt-REFINE_B_HT/2+PIX3*sc;
            float tempWd, tempHt, textHeight;
            F32 tabWd, keywordsX;
            F32 tempExtend = 0;
            textHeight = str_height(&game_12, sc, sc, 1, "MissionSearchFilterPlayDescription");
            tempX += str_wd(&game_12, sc, sc, "MissionSearchFilterPlayDescription") + PIX3*2;
            //tempX += PIX3*2 + s_drawToggleButton(tempX, tempY, z+1, sc, &s_playedFilterList[1], &s_playedFilterList[2], (U32 *)&s_viewedFilter, (U32 *)&s_currentPlayOption);
            tempX += PIX3*2 + s_drawToggleCheckBox(tempX, tempY, z+1, sc, &s_playedFilterList[1], &s_playedFilterList[2], (U32 *)&s_viewedFilter, (U32 *)&s_currentPlayOption);

            //tempX += PIX3*2 + s_drawToggleButton(tempX, tempY, z+1, sc, &s_playedFilterList[3], &s_playedFilterList[4], (U32 *)&s_viewedFilter, (U32 *)&s_currentVotedOption);
            tempX += PIX3*2 + s_drawToggleCheckBox(tempX, tempY, z+1, sc, &s_playedFilterList[3], &s_playedFilterList[4], (U32 *)&s_viewedFilter, (U32 *)&s_currentVotedOption);

            font( &game_12 );
            font_color(CLR_MM_TEXT, CLR_MM_TEXT);
            font_outl(0);
            cprntEx( refinementEntryX, tempY+textHeight, z+1, sc, sc, CENTER_Y, "MissionSearchFilterPlayDescription" );
            font_outl(1);
            //tempY +=textHeight;

            //search by level range.
            {
                int selected = s_viewedFilter&(1<<kMissionSearchViewedFilter_Level);
                int colorOpen = CLR_MM_TITLE_OPEN;//(selected)?CLR_MM_TITLE_OPEN:0x888888ff;
                int colorLit = CLR_MM_BUTTON_LIT;//(selected)?CLR_MM_BUTTON_LIT:0xaaaaaaff;
                if(drawMMCheckBox( tempX, tempY, z+1, sc, textStd(s_playedFilterList[kMissionSearchViewedFilter_Level].name), &selected, colorOpen, CLR_MM_BUTTON,
                    colorLit, CLR_MM_BUTTON_LIT, &tempWd, &tempHt))
                {
                    if(!selected)
                        s_viewedFilter &= ~(1<<kMissionSearchViewedFilter_Level);
                    else
                        s_viewedFilter |= (1<<kMissionSearchViewedFilter_Level);
                }
                tempY+=tempHt+sc*PIX3;
            }
            keywordsX = tempX;
            tempX += PIX3+drawMMCheckBox( keywordsX, tempY, z+3, sc, textStd("MissionSearchFilterKeywords"), &s_keywordsOpen, CLR_MM_TITLE_OPEN, CLR_MM_BUTTON,
                CLR_MM_BUTTON_LIT, CLR_MM_BUTTON_LIT, &tabWd, &tempHt);

            if(s_keywordsOpen)
            {
                AtlasTex * end = atlasLoadTexture("Tab_end.tga");
                AtlasTex * mid = atlasLoadTexture("Tab_mid.tga");
                UIBox uibox;
                tempExtend = 2*(textHeight+ 3*sc*PIX3);

                uiBoxDefine(&uibox, keywordsX-5*sc, tempY-2*PIX3*sc, tabWd+25*sc, tempExtend+2*PIX3*sc );
                clipperPush(&uibox);

                display_sprite( end, keywordsX-5*sc, tempY, z+2, sc, sc, CLR_MM_BACK_ALTERNATE );
                display_spriteFlip( end, tabWd+keywordsX+15*sc - end->width*sc, tempY, z+2, sc, sc, CLR_MM_BACK_ALTERNATE, FLIP_HORIZONTAL );
                display_sprite( mid, keywordsX-5*sc + end->width*sc, tempY, z+2, ( tabWd+25*sc-2*end->width*sc)/mid->width, sc, CLR_MM_BACK_ALTERNATE );
                clipperPop();
                *expandibleHeight=tempExtend+missionReviewDrawKeywords(refinementEntryX, tempY+tempExtend, z+3, sc, refinementEntryEdge-refinementEntryX, 100, WDW_MISSIONSEARCH, &s_keywords, 3, CLR_MM_BACK_ALTERNATE);
            }
            else
            {
                missionReviewResetKeywordsTooltips();
                *expandibleHeight = 2*sc*PIX3;
            }
        }
        *expandibleHeight += 65*sc;
    }
    else
    {
        if( drawMMButton( "MissionSearchClear", 0, 0, buttonX-37.5*sc, *y + 42.5*sc+*descHt-REFINE_B_HT/2, z, MISSIONSEARCH_BUTTONWD*.50, sc*.75, MMBUTTON_SMALL|MMBUTTON_ERROR, 0  ) )
            missionsearch_UpdateSearchParams(1);
        if( drawMMButton( "MissionSearchMore", 0, 0, buttonX+37.5*sc, *y + 42.5*sc+*descHt-REFINE_B_HT/2, z, MISSIONSEARCH_BUTTONWD*.50, sc*.75, MMBUTTON_SMALL, 0  ) )
            advancedSearch = !advancedSearch;

        if(COMBOS_WIDTH(2)*sc > spaceForCombos)
            comboScale =  spaceForCombos/(COMBOS_WIDTH(2)*sc);

        s_ratingCombo.wdw = WDW_MISSIONSEARCH;
        s_ratingCombo.x = 20;
        s_ratingCombo.y = 110+(*descHt-REFINE_B_HT/2)/sc;
        s_ratingCombo.wd = UI_MSSEARCH_COMBO_W*comboScale;
        if(SAFE_MEMBER(s_ratingCombo.elements[0],selected ))
            s_ratingCombo.architect =COMBOBOX_ARCHITECT_UNSELECTED;
        else
            s_ratingCombo.architect =COMBOBOX_ARCHITECT_SELECTED;
        combobox_display(&s_ratingCombo);

        s_lengthCombo.x = s_ratingCombo.wd + s_ratingCombo.x + 2*PIX3;
        s_lengthCombo.y = 110+(*descHt-REFINE_B_HT/2)/sc;
        s_lengthCombo.wd = UI_MSSEARCH_COMBO_W*comboScale;
        if(SAFE_MEMBER(s_lengthCombo.elements[0],selected ))
            s_lengthCombo.architect =COMBOBOX_ARCHITECT_UNSELECTED;
        else
            s_lengthCombo.architect =COMBOBOX_ARCHITECT_SELECTED;
        combobox_display(&s_lengthCombo);


        {
            float tempX = *x+ s_lengthCombo.wd*sc +s_lengthCombo.x*sc + 2*PIX3*sc;
            float tempY = *y + 31*sc+*descHt-REFINE_B_HT/2+PIX3*sc;
            float tempWd, tempHt;
            F32 tempExtend = 0;
            
            
            tempX += PIX3*2 + s_drawSingleToggleCheckBox(tempX, tempY, z+1, sc, &s_playedFilterList[4], (U32 *)&s_viewedFilter, (1<<s_playedFilterList[4].ID) | (1<<s_playedFilterList[3].ID));
            //tempX += PIX3*2 + s_drawToggleCheckBox(tempX, tempY, z+1, sc, &s_playedFilterList[3], &s_playedFilterList[4], (U32 *)&s_viewedFilter, (U32 *)&s_currentVotedOption);
            //search by level range.
            {
                int selected = s_viewedFilter&(1<<kMissionSearchViewedFilter_Level);
                int colorOpen = CLR_MM_TITLE_OPEN;//(selected)?CLR_MM_TITLE_OPEN:0x888888ff;
                int colorLit = CLR_MM_BUTTON_LIT;//(selected)?CLR_MM_BUTTON_LIT:0xaaaaaaff;
                if(drawMMCheckBox( tempX, tempY, z+1, sc, textStd(s_playedFilterList[kMissionSearchViewedFilter_Level].name), &selected, colorOpen, CLR_MM_BUTTON,
                    colorLit, CLR_MM_BUTTON_LIT, &tempWd, &tempHt))
                {
                    if(!selected)
                        s_viewedFilter &= ~(1<<kMissionSearchViewedFilter_Level);
                    else
                        s_viewedFilter |= (1<<kMissionSearchViewedFilter_Level);
                }
                tempX+= tempWd + 20*sc;
            }

        }
        *expandibleHeight += 30*sc;
    }
    drawFlatSectionFrame( PIX3, R10, (*box).lx-1*sc, (*box).ly, z, refinementTabWidth, 60*sc+*descHt-REFINE_B_HT/2+*expandibleHeight, sc, 0x00ffff11, 
        FRAMEPARTS_ALL & ~(kFramePart_UpL|kFramePart_UpC|kFramePart_UpR));
    *ty= (TAB_HT+70)*sc+(*expandibleHeight) + PIX3*sc;
    return update;
}


static int missionsearch_Nav( float x, float y, float z, float wd, float sc )
{
    static int s_displayRefinements = 0;

     MissionSearchTab *tab = 0;
    CBox box;
    int i;
    static int clicking = 0;
    F32 tx = x + R10*sc;
    F32 descX, descY, descWd, descHt;
    F32 ty, start = y;
      F32 tabwd = 200*sc;
    F32 expandibleHeight = 0;
    static AtlasTex *search_in;
    static AtlasTex * search_out;
    int search_clr = CLR_MM_TITLE_OPEN&0xffffff88;
    int update=0;

    if(!search_in)
        search_in = atlasLoadTexture("Search_Button_inside");
    if(!search_out)
        search_out = atlasLoadTexture("Search_Button_outside");

     z+=1;
    font( &title_18 );   
    
       for(i = 0; i < ARRAY_SIZE(s_categoriesList); i++)
    {
           if( missionsearc_NavTab(&s_categoriesList[i], tx, y + R10*sc, z, tabwd, 36*sc, sc, s_categoryCurrent==i ) )
        {
            update++;
            s_categoryCurrent = i;
        }
         tx += tabwd;
    }

    tab = &s_categoriesList[s_categoryCurrent];

    s_categoryCombo.currChoice = s_categoryCurrent;
      if( drawMMButton( "ArchitectNew", "MissionPen_rollover_glow", "MissionPen", x + wd - 160*sc, y + 30*sc, z, 300*sc, sc, MMBUTTON_CREATE, 0  ) )
    {
        MMtoggleArchitectLocalValidation(0);
        missionMakerOpenWithArc(NULL, NULL, 0, 0);
        window_setMode(WDW_MISSIONSEARCH, WINDOW_SHRINKING); 
    }

     y += 50*sc;

    ///Display Refinements button
    BuildCBox( &box, x + PIX3*sc + R10*sc, y, REFINE_B_WD + search_in->width*sc, REFINE_B_HT);
     
     if( s_allowSearch )  
    {
        if(s_displayRefinements )
            search_clr = CLR_MM_TITLE_OPEN;

        if( mouseCollision(&box) )
            search_clr = CLR_MM_TITLE_LIT;
    }
    else
        search_clr = 0xaaaaaa88;

    font_color(search_clr,search_clr);
      display_sprite( search_in, x + PIX3*sc + 2*R10*sc, y+5*sc, z, sc, sc, search_clr );
    display_sprite( search_out, x + PIX3*sc + 2*R10*sc, y+5*sc, z, sc, sc, CLR_MM_BACK_DARK );
    cprntEx( x+ PIX3*sc + 2*R10*sc + search_in->width*sc, y + REFINE_B_HT/2, z, sc, sc, CENTER_Y, "MissionSearchShowRefineSearch" );

    if(mouseClickHit(&box,MS_LEFT) && !clicking && s_allowSearch)
    {
        s_displayRefinements = !s_displayRefinements;
        clicking = 1;
    }
    else
        clicking = 0;

    descX = x + sc*(PIX3 + 2*R10 + search_in->width)+ REFINE_B_WD;
    descY = y+5*sc;
    descWd = wd -(descX-x) - PIX3*4;

    smf_SetFlags(smfSearchDescription, SMFEditMode_ReadOnly, SMFLineBreakMode_MultiLineBreakOnWhitespace, 0, 128, SMFScrollMode_None, 
        SMFOutputMode_StripAllTags, SMFDisplayMode_AllCharacters, SMFContextMenuMode_None, SMAlignment_Left, 0, 0, -1); 
    //smf_SetScissorsBox(smfSearchDescription, descX, descY, searchInputWd, text_ht );
      descHt = smf_Display(smfSearchDescription, descX, descY, z+2, descWd, 0, 0, 0, &s_taMissionSearchInput, 0);


       if( s_displayRefinements && s_allowSearch )
    {
        update |= missionsearch_displayRefinements( &x, &y, z, wd, &tx, &ty, &expandibleHeight, &descHt, &box, sc );
    }
    else
        ty = TAB_HT*sc + 40*sc;

    if( update)
    {
        s_pNav.page = 0;    //update means clear page.
        tab->page = 0;
        missionsearch_UpdateSearchParams(0);
        missionsearch_requestTab(tab);
    }
    //always reset random
    s_viewedFilter &= ~(1<<kMissionSearchViewedFilter_Random);

    MissionSearch_pageNavPosition(&s_pNav, x +PIX3, start+ty+descHt-REFINE_B_HT/2, z);
    s_pNav.wd = wd-PIX3*2;

    if(MissionSearch_pageNavDisplay(&s_pNav, sc))
    {
        tab->page = s_pNav.page;
        missionsearch_UpdateSearchParams(0);
        missionsearch_requestTab(tab);
    }

    if(s_pNav.nPages>1)
        ty += s_pNav.ht +2*PIX3;
    ty +=descHt-REFINE_B_HT/2;

    return ty;
}

static F32 s_drawPublishInventory(MissionInventory * inventory, F32 x, F32 y, F32 z, F32 wd, F32 ht, F32 sc)
{
    int publishSlots = (inventory->publish_slots)?inventory->publish_slots:3;    //3 is min publish slots a person can have
    F32 displayWd = str_wd(&game_12, sc, sc, "MissionSearchInventory", inventory->publish_slots_used, publishSlots) +2*PIX3*sc;
    F32 displayHt = str_height(&game_12, sc, sc, 0, "MissionSearchInventory", inventory->publish_slots_used, publishSlots);
    F32 extraHonorsWd = 0;

    if(inventory->permanent)
    {
        displayWd+= PIX3*sc;
        extraHonorsWd = str_wd(&game_12, sc, sc, "MissionSearchInventoryPermanent", inventory->permanent) +3*PIX3*sc;
        cprntEx( x+wd-extraHonorsWd-PIX3*sc ,y+displayHt, z, sc, sc, 0, "MissionSearchInventoryPermanent", inventory->permanent);
    }

    cprntEx( x+wd-displayWd-extraHonorsWd-2*PIX3*sc ,y+displayHt, z, sc, sc, 0, "MissionSearchInventory", inventory->publish_slots_used, publishSlots);
    return displayHt + PIX3*sc;
}


void missionsearch_DrawArcLines(MissionSearchTab *tab, int category, F32 x, F32 y, F32 z, F32 wd, F32 ht, F32 sc  )
{
    MissionSearchLine *selectedLine=0;
    float starty = y, ty, docht, view_ht;
    UIBox uibox;
    int i;
    int linesDrawn = 0;
    int linesDeleted = 0;
    Entity *e = playerPtr();

    if(tab->category == MISSIONSEARCH_MYARCS)
    {
        F32 tx = x;
        missionsearch_LoadMyArcs();
        missionsearch_MissionLineBreak( 0, x, y+TAB_HT*sc, z+1, wd, sc, 1);

        y += TAB_HT*sc + 30*sc*SEARCH_TAB_RESIZE;

        if((category == 0 || category == 1) && e && e->mission_inventory)
        {
            y += s_drawPublishInventory(e->mission_inventory, x, y, z, wd, ht, sc);
        }

         if( category == 0 )
        {
            view_ht = ht - (y-starty);
             if( IsUsingCider() )
                view_ht -= 30*sc;
            uiBoxDefine(&uibox, x, y - 5*sc, wd, view_ht-5*sc);
            clipperPush(&uibox);
            starty = y;
            ty = y - tab->list_scroll.offset;

            if(!eaSize(&tab->local))
            {
                y += 5*sc;
                s_taMissionSearch.piScale = (int *)((int)(1.5f*sc*SMF_FONT_SCALE));
                smf_SetRawText( smfError, textStd("MissionSearchNoLocalFiles", "ArchitectNew"), false );
                y += smf_Display(smfError, x + 30*sc, y+30*sc, z+2, wd-60*sc, 0, 0, 0, &s_taMissionSearch, 0);
                linesDrawn = 1;
            }
            for(i = 0; i < eaSize(&tab->local); i++)
            {
                if(!tab->local[i]->id && tab->local[i]->show)
                {
                    if(missionsearch_MissionLine(tab->local[i], tab->local[i] == tab->selected, x, &ty, z+1, wd, sc))
                        selectedLine = tab->local[i];
                    linesDrawn++;
                }
            }
        }
        else
        {
            int invalidSearch = 0;
            view_ht = ht - (y-starty);
             uiBoxDefine(&uibox, x, y - 5*sc, wd, view_ht-5*sc);
            clipperPush(&uibox); 
            starty = y;
            ty = y - tab->list_scroll.offset;

            // FIXME: show actual empty slots
            if( !eaSize(&tab->lines))
            {
                y += 5*sc;
                s_taMissionSearch.piScale = (int *)((int)(1.5f*sc*SMF_FONT_SCALE));
                smf_SetRawText( smfError, textStd("MissionSearchNoArcsPublished"), false );
                y += smf_Display(smfError, x + 30*sc, y+30*sc, z+2, wd-60*sc, 0, 0, 0, &s_taMissionSearch, 0);        
                linesDrawn = 1;
            }
            else
            {
                 for(i = 0; i < eaSize(&tab->lines); i++)
                {
                    if(tab->lines[i]->show)
                    {
                        linesDrawn++;
                        if(missionsearch_MissionLine(tab->lines[i], tab->lines[i] == tab->selected, x, &ty, z+1, wd, sc))
                        {
                            selectedLine = tab->lines[i];    
                        }
                        invalidSearch |= tab->lines[i]->rating.invalidated;
                    }
                }
            }
            if(invalidSearch)
            {
                y += 5*sc;
                s_taMissionSearch.piScale = (int *)((int)(1.5f*sc*SMF_FONT_SCALE));
                if(tab->loaded)
                    smf_SetRawText( smfError, textStd("MissionSearchInvalidArcExplanation"), false );
                ty += smf_Display(smfError, x + 15*sc, ty, z+2, wd-20*sc, 0, 0, 0, &s_taMissionSearch, 0);
            }
            
        }
        if(!linesDrawn)
        {
            smf_SetRawText( smfError, textStd("MissionSearchNoResultsMatch"), false );
            y += smf_Display(smfError, x + 15*sc, y, z+2, wd-20*sc, 0, 0, 0, &s_taMissionSearch, 0);
        }
        clipperPop();
        if( category == 0 && IsUsingCider() )
        {
               if( drawMMButton( "RaidRefreshButton", 0, 0, x +wd/2, starty + view_ht + 10*sc, z, 100*sc, sc, MMBUTTON_SMALL, 0  ))
                missionsearch_myArcsChanged( 0, 0 );
        }
    }
    else
    {
        missionsearch_MissionLineBreak( textStd(tab->name), x, y+5*sc, z+1, wd, sc, tab->pages && !tab->presorted);    //if there is at least 1 page, show sort options
        y += 30*sc;

        view_ht = ht - (y-starty);
        uiBoxDefine(&uibox, x, y - 5*sc, wd, view_ht-5*sc);
        clipperPush(&uibox);
        starty = y;
        ty = y - tab->list_scroll.offset;

        if(!eaSize(&tab->lines))
        {
            y += 5*sc;
            s_taMissionSearch.piScale = (int *)((int)(1.5f*sc*SMF_FONT_SCALE));
            if(tab->loaded)
                smf_SetRawText( smfError, textStd("MissionSearchNoResultsMatch"), false );
            else
                smf_SetRawText( smfError, textStd("MissionSearchSearching"), false );
            y += smf_Display(smfError, x + 15*sc, y, z+2, wd-20*sc, 0, 0, 0, &s_taMissionSearch, 0);

        }
        else
        {
            for(i = 0; i < eaSize(&tab->lines); i++)
            {
                if(tab->lines[i] && tab->lines[i]->show && missionsearch_MissionLine(tab->lines[i], tab->lines[i] == tab->selected, x, &ty, z+1, wd, sc))
                    selectedLine = tab->lines[i];
            }
        }
        clipperPop();
    }

    if( selectedLine ) // move selection to end to prevent flicker when there is a single frame of nothing selected
        missionsearch_SelectLine(tab, selectedLine);

    docht = ty + tab->list_scroll.offset - starty;
    tab->list_scroll.wdw = -1;
    tab->list_scroll.use_color = 1;
    tab->list_scroll.color = 0x2d98ffff;
    tab->list_scroll.xsc = sc;
    tab->list_scroll.architect = 1;
    doScrollBar(&tab->list_scroll, view_ht, docht + 20*sc, x + wd + R10*sc, starty, z+1, 0, &uibox);

    // Cleanup
    for(i = eaSize(&tab->lines)-1; i >= 0; --i)
    {
        MissionSearchLine *line = tab->lines[i];
        if(line->deleteme)
        {
            if(tab->selected == line)
                missionsearch_SelectLine(tab, NULL);
            eaRemove(&tab->lines, i);
            s_destroyCachedSearchLine(line);
            linesDeleted++;
        }
    }
    for(i = eaSize(&tab->local)-1; i >= 0; --i)
    {
        MissionSearchLine *line = tab->local[i];
        if(line->deleteme)
        {
            if(tab->selected == line)
                missionsearch_SelectLine(tab, NULL);
            eaRemove(&tab->local, i);
            s_destroyCachedSearchLine(line);
            linesDeleted++;
        }
    }
}

#define LINE_SELECTED (1<<0)
#define LINE_EDIT      (1<<1)
#define LINE_DELETE      (1<<2)

static int missionsearch_Line(char *pchText, int selected, F32 x, F32 *y, F32 z, F32 wd, F32 sc, int allowDelete)
{
    int ret = 0;
    float ht;
    int color = 0x224e4eff;
    static SMFBlock *block = 0;

    AtlasTex *white = atlasLoadTexture("white");
    CBox box;

    if(!block)
        block = smfBlock_Create();
    
     s_taMissionSearch.piScale = (int *)((int)(1.0f*sc*SMF_FONT_SCALE));
    smf_SetFlags(block, SMFEditMode_Unselectable, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      ht = MAX(selected?100*sc:0, 2*PIX3*sc + smf_ParseAndDisplay( block, pchText, x+20*sc, *y, z+2, wd-220*sc, 0, 0, 0, &s_taMissionSearch, NULL, 0, 0));

     if( selected ) 
    {
        // edit
            if( drawMMButton( "MissionSearchEdit","Edit_Button_inside", "Edit_Button_outside", x + wd - 120*sc, *y + ht/2 - 20*sc, z+1, 150*sc, sc, 0, 0 ) )
        {
            collisions_off_for_rest_of_frame = 1;
            ret |= LINE_EDIT;
        }
        //if(drawCloseButton(x + +12*sc, *y + 12*sc, z, sc, color) ) 
        if (allowDelete)
        {
        if( drawMMButton( "DeleteString", "delete_button_inside", "delete_button_outside",  x + wd - 120*sc, *y + ht/2 + 20*sc, z+1, 110*sc, sc, MMBUTTON_ERROR|MMBUTTON_SMALL, 0  ))
        {
            ret |= LINE_DELETE;
            collisions_off_for_rest_of_frame = 1;
        }
        }
    }

    BuildCBox(&box, x, *y, wd, ht);
    if(selected || mouseCollision(&box))
    {
        if( color == 0x224e4eff ) 
            color = 0x2d5f5fff;
        else
            color |= 0x99;
    }

    if(mouseClickHit(&box, MS_LEFT))
        ret |= LINE_SELECTED;

    display_sprite_blend(white, x, *y, z, wd/white->width, ht/white->height, color, color&0xffffff44, color&0xffffff44, color );
    *y += ht + PIX3*sc;

    return ret;
}

static void missionsearch_DrawVGLines( F32 x, F32 y, F32 z, F32 wd, F32 ht, F32 sc )
{
    static int iSelected = -1;
    int i,j;
    UIBox box;
    static ScrollBar sb = { WDW_MISSIONSEARCH };
    F32 starty = y, ty = y - sb.offset;

     uiBoxDefine(&box, x, y, wd, ht - 90*sc );
    clipperPush(&box);

    if(!eaSize(&g_CustomVillainGroups) )
    {
        y += 5*sc;
        s_taMissionSearch.piScale = (int *)((int)(1.5f*sc*SMF_FONT_SCALE));
        smf_SetRawText( smfError, textStd("MissionSearchNoVillainGroups"), false );
        y += smf_Display(smfError, x + 30*sc, y+30*sc, z+2, wd-60*sc, 0, 0, 0, &s_taMissionSearch, 0);
    }

      for( i = 0; i < eaSize(&g_CustomVillainGroups); i++ )
    {
        char * estr;
        CustomVG *pvg = g_CustomVillainGroups[i];
        int first = 1, retval=0;

        estrCreate(&estr);
        estrConcatf(&estr, "<scale 1.5><font outline=1 color=ArchitectTitle>%s</font></scale><br>", pvg->displayName);

        if( iSelected == i )
        {
            
            for( j = 0; j < eaSize(&pvg->customVillainList); j++ )
            {
                if( first )
                    estrConcatf(&estr, "<font outline=1 color=ArchitectTitle>%s: </font><font outline=0 color=architect>%s", textStd("MMCustomCritters"), pvg->customVillainList[j]->name );
                else
                    estrConcatf(&estr, ", %s", pvg->customVillainList[j]->name );
                first = 0;
            }
            if(!first)
                estrConcatStaticCharArray(&estr, "</font><br>");
            
            first = 1;
            for( j = 0; j < eaSize(&pvg->existingVillainList); j++ )
            {
                const VillainDef *pDef = villainFindByName(pvg->existingVillainList[j]->pchName);
                int idx = pvg->existingVillainList[j]->levelIdx;

                if( pDef && EAINRANGE(idx, pDef->levels) && (pDef->rank == VR_BOSS || pDef->rank == VR_PET) )
                {
                    if( first )
                        estrConcatf( &estr, "<font outline=1 color=ArchitectTitle>%s: </font><font outline=0 color=architect>%s", textStd("MMBosses"), textStd(pDef->levels[idx]->displayNames[0]) );
                    else
                        estrConcatf( &estr, ", %s", textStd(pDef->levels[idx]->displayNames[0]) );
                    first = 0;
                }
            }
            if(!first)
                estrConcatStaticCharArray(&estr, "</font><br>");

            first = 1;
            for( j = 0; j < eaSize(&pvg->existingVillainList); j++ )
            {
                const VillainDef *pDef = villainFindByName(pvg->existingVillainList[j]->pchName);
                int idx = pvg->existingVillainList[j]->levelIdx;

                if( pDef && EAINRANGE(idx, pDef->levels) && (pDef->rank == VR_LIEUTENANT || pDef->rank == VR_SNIPER) )
                {
                    if( first )
                        estrConcatf(&estr, "<font outline=1 color=ArchitectTitle>%s: </font><font outline=0 color=architect>%s", textStd("MMLts"), textStd(pDef->levels[idx]->displayNames[0]) );
                    else
                        estrConcatf(&estr, ", %s", textStd(pDef->levels[idx]->displayNames[0]) );
                    first = 0;
                }
            }
            if(!first)
                estrConcatStaticCharArray(&estr, "</font><br>");

            first = 1;
            for( j = 0; j < eaSize(&pvg->existingVillainList); j++ )
            {
                const VillainDef *pDef = villainFindByName(pvg->existingVillainList[j]->pchName);
                int idx = pvg->existingVillainList[j]->levelIdx;

                if( pDef && EAINRANGE(idx, pDef->levels) && (pDef->rank == VR_SMALL || pDef->rank == VR_MINION) )
                {
                    if( first )
                        estrConcatf(&estr, "<font outline=1 color=ArchitectTitle>%s: </font><font outline=0 color=architect>%s", textStd("MMMinions"), textStd(pDef->levels[idx]->displayNames[0])  );
                    else
                        estrConcatf(&estr, ", %s", textStd(pDef->levels[idx]->displayNames[0]) );
                    first = 0;
                }
            }
            if(!first)
                estrConcatStaticCharArray(&estr, "</font><br>");
        }

        retval = missionsearch_Line( estr, iSelected == i, x, &ty, z, wd, sc, stricmp(pvg->displayName, textStd("AllCritterListText")) != 0 );

        if( retval&LINE_EDIT)
            editCVG_setup(pvg->displayName);
        if( retval&LINE_SELECTED)
            iSelected = i;
        if( retval&LINE_DELETE )
        {
            s_deleteCustomVillainGroup(pvg);
        }

        estrDestroy(&estr);
    }

    clipperPop();

     sb.architect = 1;
       doScrollBar(&sb, ht - 90*sc, (ty+sb.offset-starty) + 20*sc, x + wd + R10*sc, 130*sc, z+1, 0, &box);

    if (drawMMButton("NewCVGString", 0, 0, x+wd/2, starty + ht - 65*sc, z, 250*sc, sc, 0, 0 ))
    {
        editCVG_setup(0);
    }
}

void missionsearch_DrawCharacterLines( F32 x, F32 y, F32 z, F32 wd, F32 ht, F32 sc )
{
    static int iSelected = -1;
    int i;
    PCC_Critter **listOfCritters = NULL;
    UIBox box;
    static ScrollBar sb = { WDW_MISSIONSEARCH };
    F32 starty = y, ty = y - sb.offset;

     uiBoxDefine(&box, x, y, wd, ht - 90*sc );
    clipperPush(&box);
    
    for (i = 0; i < eaSize(&g_CustomVillainGroups); i++)
    {
        if (stricmp(g_CustomVillainGroups[i]->displayName, textStd("AllCritterListText")) == 0)
        {
            listOfCritters = g_CustomVillainGroups[i]->customVillainList;
        }
    }

    if (listOfCritters && eaSize(&listOfCritters))
    {
        for( i = 0; i < eaSize(&listOfCritters); i++ )
        {
            char * estr;
            PCC_Critter *pCritter = listOfCritters[i];
            static int previousSelected = -1;
            int first = 1, retval=0;

            if(!pCritter->name )
                continue;

            estrCreate(&estr);

            if( iSelected == i )
            {
                static PictureBrowser *pb;
                if(!pb)
                {
                    pb = picturebrowser_create();
                    picturebrowser_init( pb,0, 0, 0, 200, 140, NULL, 0 );
                    picturebrowser_setAnimationAvailable(pb, 0);
                }
                picturebrowser_setMMImageCostume(pb, costume_as_const(pCritter->pccCostume));
                estrConcatf(&estr, "<img src=pictureBrowser:%d align=right>", ((pb)->idx) );
            }

            estrConcatf(&estr, "<scale 1.5><font outline=1 color=ArchitectTitle>%s</font></scale><br>", pCritter->name );

            if( iSelected == i )
            {
                static int reasonForInvalid = PCC_LE_VALID;
                if (previousSelected != iSelected)
                {
                    reasonForInvalid = isPCCCritterValid( playerPtr(), pCritter,0,0);
                    previousSelected = iSelected;
                }

                PCC_generatePCCHTMLSelectionText(&estr, pCritter, reasonForInvalid, 0, "ArchitectTitle");
            }
            else
            {
                estrConcatf(&estr, "<font outline=1 color=ArchitectTitle>%s: </font><font outline=0 color=architect>%s</font><br>", textStd("MMVillainGroup"), pCritter->villainGroup );
            }

            retval = missionsearch_Line( estr, iSelected == i, x, &ty, z, wd, sc, 1 );

            if( retval&LINE_EDIT)
            {
                editPCC_setup(0, PCC_EDITING_MODE_EDIT, pCritter, 0, 0);
                previousSelected = -1;
                iSelected = -1;
            }
            if( retval&LINE_SELECTED)
                iSelected = i;
            if( retval&LINE_DELETE )
            {
                //    assume we are only going to delete one per frame (i.e., no selection removal)
                s_deleteCustomCritter(pCritter);
                previousSelected = -1;
            }
            estrDestroy(&estr);
        }        
    } 
    else
    {
        y += 5*sc;
        s_taMissionSearch.piScale = (int *)((int)(1.5f*sc*SMF_FONT_SCALE));
        smf_SetRawText( smfError, textStd("MissionSearchNoCustomCritter"), false );
        y += smf_Display(smfError, x + 30*sc, y+30*sc, z+2, wd-60*sc, 0, 0, 0, &s_taMissionSearch, 0);
    }

    clipperPop();

     sb.architect = 1;
    doScrollBar(&sb, ht - 90*sc, (ty + sb.offset - starty) + 20*sc, x + wd + R10*sc, 130*sc, z+1, 0, &box);

       if (drawMMButton("MMCreateCustomCritter","Character_Icon", 0, x+wd/2, starty + ht - 65*sc, z, 350*sc, sc, MMBUTTON_ICONALIGNTOTEXT  | MMBUTTON_USEOVERRIDE_COLOR, 0 ))
     {
        editPCC_setup(0, PCC_EDITING_MODE_CREATION, NULL, 0, 0);
        iSelected = -1;
     }
}


void missionsearch_DrawTab(float x, float y, float z, float wd, float ht, float sc)
{
    int i;
    MissionSearchTab *tab = &s_categoriesList[s_categoryCurrent];

      if(s_updateRefine && s_categoryCurrent == MISSIONSEARCH_MYARCS)
     {
        missionsearch_RefineSearch(tab->lines);
        missionsearch_RefineSearch(tab->local);
        s_updateRefine = 0;
    }
    // FIXME: handle stalled searches
    if(!tab->loaded && tab->requested + 60 < timerSecondsSince2000())
        missionsearch_requestTab(tab);
    s_allowSearch = 1;
    
       if(tab->category == MISSIONSEARCH_MYARCS)
     {
        static int my_category = 0;
        int last_category = my_category;
        char * myCategories[] = { "MissionSearchLocalFiles", "MissionSearchPublishedArcs", "MissionSearchCustomCharacters", "MissionSearchCustomVillainGroups" };
        char **ppNames=0;

        for(i = 0; i < ARRAY_SIZE(myCategories); i++)
            eaPush( &ppNames, myCategories[i] );
        my_category = drawMMFrameTabs( x, y + 30*sc, z, sc*SEARCH_TAB_RESIZE, wd, ht-30*sc, 20*sc, kStyle_Architect, &ppNames, my_category );
        eaDestroy(&ppNames);

           if ( my_category == 3 )
        {
            missionsearch_DrawVGLines(x,y + 40*sc,z,wd,ht,sc);
            s_allowSearch = 0;
        }
        else if ( my_category == 2 )
        {
            missionsearch_DrawCharacterLines(x,y + 40*sc,z,wd,ht,sc);
            s_allowSearch = 0;
        }
        else if( my_category < 2 )
            missionsearch_DrawArcLines(tab, my_category, x, y, z, wd, ht, sc );
    }
    else
        missionsearch_DrawArcLines(tab, 0, x, y, z, wd, ht, sc );

}

static void missionsearch_initCombo( char * pchName, ComboBox *cb, MissionSearchComboItem *list, int size, int dropWidth)
{
    int i;
    int accesslevel = playerPtr()->access_level;

    comboboxClear(cb);
    combocheckbox_init(cb, 10, 0, 20, UI_MSSEARCH_COMBO_W, dropWidth, 20, 500, pchName, WDW_MISSIONSEARCH, 0, 1 );

    for(i = 0; i < size; i++)
        if(list[i].accesslevel <= accesslevel)
            combocheckbox_add(cb, 0, (list[i].iconName)? atlasLoadTexture( list[i].iconName):NULL, list[i].name, list[i].ID);
}

int missionsearch_Window(void)
{
    static int init = -1;

    float x, y, z, wd, ht, sc, nav_ht;
    int color, bcolor;

    if(!window_getDims(WDW_MISSIONSEARCH, &x, &y, &z, &wd, &ht, &sc, &color, &bcolor) ) 
        return 0;

    PERFINFO_AUTO_START("missionsearch_Window", 1); 

    MissionSearch_searchRequestUpdate();
    MissionSearch_publishRequestUpdate();

    missionReviewLoadKeywordList();

    if(init != playerPtr()->access_level) // some menus depend on access level
    {
        int i;
        int locale;
        initPCCFolderCache();
        initCVGFolderCache();
        missionsearch_InitMyArcs();
        missionsearch_initCombo( "MissionSearchRating",        &s_ratingCombo,        s_ratingList,        ARRAY_SIZE(s_ratingList), 175);
        missionsearch_initCombo( "MissionSearchLength",        &s_lengthCombo,        s_lengthList,        ARRAY_SIZE(s_lengthList), 125);
        missionsearch_initCombo( "MissionSearchMorality",    &s_alignmentCombo,    s_alignmentList,    ARRAY_SIZE(s_alignmentList), 125);
        missionsearch_initCombo( "MissionSearchLocale",        &s_localeCombo,        s_localeList,        ARRAY_SIZE(s_localeList), 125);
        missionsearch_initCombo( "MissionSearchStatusE",        &s_statusCombo,        s_statusList,        ARRAY_SIZE(s_statusList), 175);
        locale = locGetIDInRegistry();
        s_viewedFilter = (1<<kMissionSearchViewedFilter_Voted_NoShow) & (1<<kMissionSearchViewedFilter_Played_NoShow);
        //unselect the all item.
        combobox_setId( &s_localeCombo, locale, 1 );
        s_localeCombo.elements[0]->selected = 0;
        s_MissionSearch.locale_id = 1<<locale;
        

        // this is mostly for QA
        for(i = 0; i < ARRAY_SIZE(s_categoriesList); i++)
            missionsearch_clearPages(&s_categoriesList[i]);

        s_localeCombo.architect = s_ratingCombo.architect =  s_lengthCombo.architect = s_alignmentCombo.architect = 1;
        init = playerPtr()->access_level;
        MissionSearch_pageNavInit(&s_pNav, 0,0, 200, 10);
        if(!smfSearchDescription)
            smfSearchDescription = smfBlock_Create();
        if(!smfSearch)
            smfSearch = smfBlock_Create();
        if(!smfError)
            smfError = smfBlock_Create();
    }

    if( ArchitectCanEdit( playerPtr() ) )
    {
        drawWaterMarkFrame( x ,y, z, sc, wd, ht, 1 );

        nav_ht = missionsearch_Nav( x, y, z, wd, sc );

        missionsearch_DrawTab( x + R10*sc, y + nav_ht, z+1, wd - 2*R10*sc, ht - nav_ht, sc );
    }
    else
    {
        window_setMode(WDW_CUSTOMVILLAINGROUP, WINDOW_SHRINKING);
        window_setMode(WDW_MISSIONMAKER,WINDOW_SHRINKING);
        window_setMode(WDW_MISSIONSEARCH,WINDOW_SHRINKING);
    }

    PERFINFO_AUTO_STOP();
    return 0;
}

#define INVALID_ARC_WARNING_PERIOD (24*60*60)
void missionsearch_InvalidWarning(int invalidArcs, int playableErrors)
{
    Entity *e = playerPtr();
    if(e && e->pl && (invalidArcs || playableErrors))
    {
        U32 now = timerSecondsSince2000();
        if(now > e->pl->timeLastInvalidArcWarning + INVALID_ARC_WARNING_PERIOD)
        {
            e->pl->timeLastInvalidArcWarning = now;
            dialogStd( DIALOG_OK, textStd("MissionArchitectInvalidArcs"), 0, 0, missionMakerDialogDummyFunction, NULL, DLGFLAG_ARCHITECT );
        }
    }
}