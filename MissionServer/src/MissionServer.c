#include <utilitieslib/stdtypes.h>
#include "MissionServer.h"
#include "MissionServerArcData.h"
#include "MissionServerSearch.h"
#include "MissionServerJournal.h"
#include <utilitieslib/utils/log.h>

#include <serverlib/serverlib.h>
#include <utilitieslib/UtilsNew/meta.h>
#include <utilitieslib/UtilsNew/Str.h>
#include "mission/missionserver_meta.h"
#include "entity/entVarUpdate.h" // tastes bad, man

#include "comm_backend.h"
#include <utilitieslib/network/netcomp.h>
#include <utilitieslib/network/net_packetutil.h>
#include <utilitieslib/utils/textparser.h>
#include <utilitieslib/utils/tokenstore.h>
#include <utilitieslib/components/StashTable.h>
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/utils/file.h> // isDevelopmentMode
#include <utilitieslib/components/StringCache.h>
#include <utilitieslib/components/EString.h>
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/utils/ConsoleDebug.h>
#include <utilitieslib/utils/MemoryMonitor.h>
#include <utilitieslib/network/crypt.h>
#include <utilitieslib/utils/sysutil.h>
#include <utilitieslib/utils/winutil.h>
#include <utilitieslib/utils/FolderCache.h>
#include <utilitieslib/network/sock.h>
#include <utilitieslib/utils/mathutil.h>
#include "persist/persist.h"
#include <utilitieslib/utils/structDefines.h>
#include <utilitieslib/components/bitfield.h>

#define MISSIONSERVER_PETITIONTAG        "[Mission Architect]" // scripts look for this tag for moving petitions to the appropriate RightNow Web category

#define MISSIONSERVER_PACKETFLUSHTIME    (2.0f)
#define MISSIONSERVER_TICK_FREQ            1        // ticks/sec
#define MISSIONSERVER_PERSISTTIME        (60*60)    // hour
#define MISSIONSERVER_PERSISTFAIL        (60*60)    // hour
#define SHARDRECONNECT_RETRY_TIME        (isDevelopmentMode()?15:120)
#define LAZY_SEARCH_CULLING                0

#define INVENTORY_MAX_TICKETS            9999        // set for max salvage of one type you can hold
#define INVENTORY_MAX_LIFETIME_TICKETS    2000000000    // just to stop it ever wrapping around
#define INVENTORY_MAX_OVERFLOW_TICKETS    2000000000    // also stop this from wrapping around
#define INVENTORY_MAX_COMPLETIONS        2000000000    // this mustn't wrap around either
#define INVENTORY_MAX_STARTS            2000000000    // this mustn't wrap around either

MissionServerState g_missionServerState;

// some shorthand for consistent messages
#define USER_PRINTF_FMT                "%s.%d"
#define USER_PRINTF_EX(id,region)    authregion_toString(region), id
#define USER_PRINTF(user)            USER_PRINTF_EX(user->auth_id, user->auth_region)

ParseTable parse_MissionServerInventory[] =
{
    { "Tickets",            TOK_INT(MissionServerInventory, tickets, 0)                                    },
    { "LifetimeTickets",    TOK_INT(MissionServerInventory, lifetime_tickets, 0)                        },
    { "OverflowTickets",    TOK_INT(MissionServerInventory, overflow_tickets, 0)                        },
    { "BadgeFlags",            TOK_INT(MissionServerInventory, badge_flags, 0)                                },
    { "BestArcStars",        TOK_INT(MissionServerInventory, best_arc_stars, 0)                            },
    { "PublishSlots",        TOK_INT(MissionServerInventory, publish_slots, MISSIONSEARCH_MAX_MISSIONS)    },
    { "UncirculatedSlots",    TOK_INT(MissionServerInventory, uncirculated_slots, MISSIONSEARCH_MAX_UNCIRCULATED)    },
    { "PaidPublishSlots",    TOK_INT(MissionServerInventory, paid_publish_slots, 0)                        },
    { "CreatedCompletions",    TOK_INT(MissionServerInventory, created_completions, 0)                        },
    { "CostumeKeys",        TOK_STRINGARRAY(MissionServerInventory, costume_keys)                        },
    { "End",                TOK_END                                                                        },
    { NULL, 0, 0 }
};

ParseTable parse_MissionServerKeywordVote[]=
{
    { "ArchId",                TOK_INT(MissionServerKeywordVote, arcid, 0)                                },
    { "VoteField",            TOK_INT(MissionServerKeywordVote, vote, 0)                            },
    { "End",                TOK_END                                                                    },
    { NULL, 0, 0 }
};

ParseTable parse_MissionServerUser[] =
{
    { ".",                    TOK_STRUCTPARAM | TOK_PRIMARY_KEY | TOK_AUTOINT(MissionServerUser, auth_id, 0)                                    },
    { ".",                    TOK_STRUCTPARAM | TOK_PRIMARY_KEY | TOK_AUTOINT(MissionServerUser, auth_region, AUTHREGION_DEV), AuthRegionEnum    },
    { "AuthName",            TOK_STRING(MissionServerUser, authname, 0)                                                                        },
    { "AutoBans",            TOK_AUTOINT(MissionServerUser, autobans, 0)                                                                        },
    { "Banned",                TOK_AUTOINT(MissionServerUser, banned_until, 0), NULL, TOK_FORMAT_DATESS2000                                    },
    { "AuthIDsVoted",        TOK_INTARRAY(MissionServerUser, authids_voted)                                                                    },
    { "AuthIDsRegion",        TOK_INTARRAY(MissionServerUser, authids_voted_region)                                                            },
    { "AuthIDsTimestamp",    TOK_INTARRAY(MissionServerUser, authids_voted_timestamp)                                                        },
    { "Votes0",                TOK_INTARRAY(MissionServerUser, arcids_votes[0])                                                                },
    { "Votes1",                TOK_INTARRAY(MissionServerUser, arcids_votes[1])                                                                },
    { "Votes2",                TOK_INTARRAY(MissionServerUser, arcids_votes[2])                                                                },
    { "Votes3",                TOK_INTARRAY(MissionServerUser, arcids_votes[3])                                                                },
    { "Votes4",                TOK_INTARRAY(MissionServerUser, arcids_votes[4])                                                                },
    { "Votes5",                TOK_INTARRAY(MissionServerUser, arcids_votes[5])                                                                },
    { "BestVotes0",            TOK_INTARRAY(MissionServerUser, arcids_best_votes[0])                                                            },
    { "BestVotes1",            TOK_INTARRAY(MissionServerUser, arcids_best_votes[1])                                                            },
    { "BestVotes2",            TOK_INTARRAY(MissionServerUser, arcids_best_votes[2])                                                            },
    { "BestVotes3",            TOK_INTARRAY(MissionServerUser, arcids_best_votes[3])                                                            },
    { "BestVotes4",            TOK_INTARRAY(MissionServerUser, arcids_best_votes[4])                                                            },
    { "BestVotes5",            TOK_INTARRAY(MissionServerUser, arcids_best_votes[5])                                                            },
    { "VotesEarned0",        TOK_INT(MissionServerUser, total_votes_earned[0], 0)                                                            },
    { "VotesEarned1",        TOK_INT(MissionServerUser, total_votes_earned[1], 0)                                                            },
    { "VotesEarned2",        TOK_INT(MissionServerUser, total_votes_earned[2], 0)                                                            },
    { "VotesEarned3",        TOK_INT(MissionServerUser, total_votes_earned[3], 0)                                                            },
    { "VotesEarned4",        TOK_INT(MissionServerUser, total_votes_earned[4], 0)                                                            },
    { "VotesEarned5",        TOK_INT(MissionServerUser, total_votes_earned[5], 0)                                                            },
    { "Started",            TOK_INTARRAY(MissionServerUser, arcids_started)                                                                    },
    { "StartedTimestamp",    TOK_INTARRAY(MissionServerUser, arcids_started_timestamp)                                                        },
    { "Completed",            TOK_INTARRAY(MissionServerUser, arcids_played)                                                                    },
    { "CompletedTimestamp",    TOK_INTARRAY(MissionServerUser, arcids_played_timestamp)                                                        },
    { "Complaints",            TOK_INTARRAY(MissionServerUser, arcids_complaints)                                                                },
    { "Inventory",            TOK_EMBEDDEDSTRUCT(MissionServerUser, inventory, parse_MissionServerInventory)                                    },
    { "KeywordVotes",        TOK_STRUCT(MissionServerUser, ppKeywordVotes, parse_MissionServerKeywordVote )                                    },
    { "GuestAuthorBio",        TOK_IGNORE                                                                                                        },
    { "End",                TOK_END                                                                                                            },
    { "", 0, 0 }
};
STATIC_ASSERT(MISSIONRATING_VOTETYPES == 6);

ParseTable parse_legacy_MissionComplaint[] = // used for 
{
    { ".",                    TOK_STRUCTPARAM | TOK_STRING(MissionComplaint, comment, 0)    },
    { "\n",                    TOK_END                                                            },
    { "", 0, 0 }
};

void missionserver_ForceLoadDb();

static U32 *arcsByRange[MISSIONSEARCH_MAX_LEVEL] = {0};

static U32 disconnectAllShards = 0;

static void s_sendPetitionString(NetLink *link, char *authname, AuthRegion authregion,
                                 char *character, char *summary, char *msg, char *notes);
static void s_updateArcRating(MissionServerArc *arc);
void missionserver_UpdateArcTopKeywords(MissionServerArc *arc, U32 keywords);

MissionServerShard* MissionServer_GetShard(const char *name)
{
    if(devassert(name && g_missionServerState.shardsByName))
    {
        MissionServerShard *shard;
        if(stashFindPointer(g_missionServerState.shardsByName,name,&shard))
            return shard;
    }
    return NULL;
}

static MissionServerUser* s_findCreator(MissionServerArc *arc)
{
    MissionServerUser *user = plFind(MissionServerUser, arc->creator_id, arc->creator_region);
    assert(g_missionServerState.forceLoadDb || user);
    return user;
}

static int s_isArcOwned(MissionServerArc *arc, int auth_id, AuthRegion auth_region)
{
    return arc ? arc->creator_id == auth_id && arc->creator_region == auth_region : 0;
}

static int s_instaBanThreshold(MissionServerArc *arc)
{
    MissionServerVars *vars = g_missionServerState.vars;
    int grace = vars->instaban_grace_min;
    if(arc->total_stars)
        grace = MAX(grace, (int)(vars->instaban_grace_speed*log(arc->total_stars)/log(vars->instaban_grace_base)));
    return grace;
}

static void s_addRemoveArcLevelRange(MissionServerArc *arc, int add)
{
    if(!arc->header.levelRangeLow && !arc->header.levelRangeHigh && add)
    {
        int i;
        arc->header.levelRangeLow = 0;
        arc->header.levelRangeHigh = 54;
        for(i = 0; i < eaSize(&arc->header.missionDesc); i++)
        {
            MissionHeaderDescription *description = arc->header.missionDesc[i];
            if(description)
            {
                arc->header.levelRangeLow = MAX(arc->header.levelRangeLow,  description->min);
                arc->header.levelRangeHigh = MIN(arc->header.levelRangeHigh, description->max);
            }
        }
    }
    
    if(arc->header.levelRangeLow>=1 && arc->header.levelRangeHigh<=MISSIONSEARCH_MAX_LEVEL && arc->header.levelRangeHigh>=arc->header.levelRangeLow)
    {
        int i;
        for(i = arc->header.levelRangeLow-1; i < arc->header.levelRangeHigh; i++)    //automatically excludes arcs that have no consistent level range since high < low
        {
            if(add)
                eaiSortedPushUnique(&arcsByRange[i], arc->id);
            else
                eaiSortedFindAndRemove(&arcsByRange[i], arc->id);
        }
    }
}

static U32 **s_getArcsWithLevelRange(int range)
{
    devassert(range >=1 && range <=MISSIONSEARCH_MAX_LEVEL);
    return &arcsByRange[range-1];
}

static MissionRating s_calculateVotesStarsRating(MissionServerArc *arc, F32 *prating_val)
{
    int i = 0;
    MissionServerUser *creator;
    F32 rating_val = 0.f;

    arc->total_votes = 0;
    arc->total_stars = 0;

    for(i = MISSIONRATING_1_STAR; i < MISSIONRATING_VOTETYPES; i++)
    {
        arc->total_votes += arc->votes[i];
        arc->total_stars += i * arc->votes[i];
    }

    creator = plFind(MissionServerUser, arc->creator_id, arc->creator_region);

    if(creator && creator->inventory.best_arc_stars < (U32)(arc->total_stars))
    {
        creator->inventory.best_arc_stars = arc->total_stars;
        missionserver_journalUserInventory(creator);
        creator->inventory.dirty = true;
    }

    rating_val = arc->total_votes ? (F32)arc->total_stars / (F32)arc->total_votes : 0;

    if(prating_val)
        *prating_val = rating_val;

    if(arc->total_votes < MAX(g_missionServerState.vars->rating_threshold, 1))
        return MISSIONRATING_UNRATED;
    else // Float to int rounds down so adding 0.5 makes it round out.
        return MINMAX((rating_val + 0.5f), ((F32)MISSIONRATING_1_STAR), ((F32)MISSIONRATING_MAX_STARS));
}

static void s_arcsPreLoad(void)
{
    int i;
    char *arc_str = estrTemp();

    MissionServerArc **arcs = plGetAll(MissionServerArc);
    MissionServerArc **arcs_delete = NULL;

    for(i = eaSize(&arcs)-1; i >= 0; --i)
    {
        MissionServerArc *arc = arcs[i];
        if(missionserver_FindArcData(arc))
            break; // other missing data shows some other corruption.

        printf("Warning: Recent arc %d has no data, deleting.\n", arc->id);

        estrClear(&arc_str);
        ParserWriteTextEscaped(&arc_str, parse_MissionServerArc, arc, 0, 0);
        LOG( LOG_DELETED_ARCS, LOG_LEVEL_IMPORTANT, 0, "No Data: %s", arc_str);

// CS really doesn't want to be bothered with this. i'll leave it just in case.
//         if(!Str_len(&g_missionServerState.petition[arc->creator_region]))
//             Str_catf(&g_missionServerState.petition[arc->creator_region],
//                         "Architect: Deleted Partial Publishes\n"
//                         "The listed story arcs have been deleted because they have no data. "
//                         "This is normal after a missionserver crash.\n"
//                         "arcid(authid.authregion) title\n");
//         Str_catf(&g_missionServerState.petition[arc->creator_region],
//                     ARC_PRINTF_FMT" %s\n", ARC_PRINTF(arc), arc->header.title);

        eaPush(&arcs_delete, arc);
    }

    plDeleteList(MissionServerArc, arcs_delete, eaSize(&arcs_delete));

    eaDestroy(&arcs_delete);
    estrDestroy(&arc_str);
}

META(COMMANDLINE("forceLoadDb"))
void missionserver_ForceLoadDb(int force)
{
    g_missionServerState.forceLoadDb = force;
}

META(COMMANDLINE("dumpStats"))
void missionserver_DumpStats(int force)
{
    g_missionServerState.dumpStats = force;
}

static void s_arcsPostLoad(void)
{
    int i;
    U32 lastid = 0;
    U32 unpublish_time = timerSecondsSince2000() - g_missionServerState.vars->days_till_unpublish * SECONDS_PER_DAY;
    U32 delete_time = timerSecondsSince2000() - g_missionServerState.vars->days_till_delete * SECONDS_PER_DAY;
    int number_arcs;
    char *buf = Str_temp();
    char *arc_str = estrTemp();

    MissionServerArc **arcs = plGetAll(MissionServerArc);
    MissionServerArc **arcs_delete = NULL;

    missionserversearch_InitializeSearchCache();

    number_arcs = eaSize(&arcs);
    for(i = 0; i < number_arcs; i++)
    {
        MissionServerArc *arc = arcs[i];
        MissionServerUser *user = s_findCreator(arc);
        static int lastFeedbackUpdate =0;
        int percent = i*100/number_arcs;

        if(!user)
        {
            assert(g_missionServerState.forceLoadDb);
            eaPush(&arcs_delete, arc); // since plDelete() changes the arcs array, do it afterward.
            continue;
        }

        if(percent != lastFeedbackUpdate)
        {
            lastFeedbackUpdate = percent;
            Str_clear(&buf);
            Str_catf(&buf, "%d: MissionServer. Preparing Cache: arc %d (%d). %d %%", _getpid(), i, number_arcs, i*100/number_arcs);
            setConsoleTitle(buf);
        }

        // some sanity checks
        assertmsgf(user, "Arc and User databases inconsistent!  Arc %d is missing user %d.", arc->id, arc->creator_id);
        assertmsg(lastid < arc->id, "Arc database is unsorted!"); // FIXME: persist layer should ensure this
        lastid = arc->id;

        // automatic status changes
        if(!arc->pulled && !missionban_isVisible(arc->ban))
            arc->pulled = timerSecondsSince2000(); // small fixup for old db
        if(arc->invalidated < MISSIONSERVER_INVALIDATIONCOUNT)
            arc->invalidated = 0;
        if(arc->playable_errors < MISSIONSERVER_PLAYABLEINVALIDATIONCOUNT)
            arc->playable_errors = 0;
        if(    (!arc->unpublished && arc->pulled && arc->pulled < unpublish_time && arc->ban != MISSIONBAN_SELFREVIEWED_SUBMITTED) )
        {
            LOG( LOG_MISSIONSERVER, LOG_LEVEL_VERBOSE, 0, "Auto-unpublishing arc "ARC_PRINTF_FMT, ARC_PRINTF(arc));
            arc->unpublished = timerSecondsSince2000();
            missionserver_journalArcUnpublish(arc);
        }
        if(arc->unpublished && arc->unpublished < delete_time)
        {
            

            missionserver_ArchiveArcData(arc);

            estrClear(&arc_str);
            ParserWriteTextEscaped(&arc_str, parse_MissionServerArc, arc, 0, 0);
            LOG( LOG_DELETED_ARCS, LOG_LEVEL_VERBOSE, 0, "Deleting arc %s "ARC_PRINTF_FMT, arc_str, ARC_PRINTF(arc));

            eaPush(&arcs_delete, arc); // since plDelete() changes the arcs array, do it afterward.
            missionserver_UnindexArc(arc);
            s_addRemoveArcLevelRange(arc, 0);
            continue;
        }

        // fixup old data
        if(missionhonors_isPermanent(arc->honors))
            arc->permanent = 1;

        // fixup pointers
        if(arc->unpublished)
            eaiPush(&user->arcids_unpublished, arc->id);
        else if(arc->permanent)
            eaiPush(&user->arcids_permanent, arc->id);
        else
            eaiPush(&user->arcids_published, arc->id);

        // recalculate total votes and stars, and fixup rating
        arc->rating = s_calculateVotesStarsRating(arc, NULL);

        if(missionserverarc_isIndexed(arc))
        {
            missionserver_IndexNewArc(arc);
            s_addRemoveArcLevelRange(arc, 1);
        }

        // allow honors, badges, etc. to change
        s_updateArcRating(arc);
        missionserver_UpdateArcTopKeywords(arc, 0);
    }

    plDeleteList(MissionServerArc, arcs_delete, eaSize(&arcs_delete));
    missionserver_CloseArcDataArchive();

    eaDestroy(&arcs_delete);
    Str_destroy(&buf);
    estrDestroy(&arc_str);

    g_missionServerState.forceLoadDb = 0;

#ifdef CORRECTNESS_TEST 
    //make this only on test
    MissionServerSearchTest_SearchFormatting();
    MissionServerSearchTest_TestAllArcSearchPublish();
    MissionServerSearchTest_TestAllArcSearch();
    MissionServerSearchTest_TestAllArcSearchVote();
#endif
}

static void s_pruneArcids(int **parcids)
{
    int i;
    for(i = eaiSize(parcids)-1; i >= 0; --i)
        if(!plFind(MissionServerArc, (*parcids)[i]))
            eaiRemove(parcids, i);
}

static void s_usersPostLoad(void)
{
    int i, j;
    MissionServerUser **users = plGetAll(MissionServerUser);
    for(i = 0; i < eaSize(&users); i++)
    {
        MissionServerUser *user = users[i];
        U32 stars = user->inventory.best_arc_stars;

        // Make sure we don't cheat old users out of badge credit for arcs
        // which have gone down in ranking or been unpublished.
        if((user->inventory.badge_flags & kBadgeArcTotalStars100) && stars < 100)
            stars = 100;
        if((user->inventory.badge_flags & kBadgeArcTotalStars250) && stars < 250)
            stars = 250;
        if((user->inventory.badge_flags & kBadgeArcTotalStars500) && stars < 500)
            stars = 500;
        if((user->inventory.badge_flags & kBadgeArcTotalStars1000) && stars < 1000)
            stars = 1000;
        if((user->inventory.badge_flags & kBadgeArcTotalStars2000) && stars < 2000)
            stars = 2000;

        user->inventory.best_arc_stars = stars;

        for(j = 0; j < ARRAY_SIZE(user->arcids_votes); j++)
            s_pruneArcids(&user->arcids_votes[j]);
        for(j = 0; j < ARRAY_SIZE(user->arcids_best_votes); j++)
            s_pruneArcids(&user->arcids_best_votes[j]);
        s_pruneArcids(&user->arcids_complaints);
        missionserver_updateLegacyTimeStamps(user, 1);
    }
}

static void s_messageEntityv(NetLink *link, int entid, int channel, const char *fmt, va_list va)
{
    Packet *pak_out = pktCreateEx(link, MISSION_SERVER_MESSAGEENTITY);
    pktSendBitsAuto(pak_out, entid);
    pktSendBitsAuto(pak_out, channel);
    pktSendStringAligned(pak_out, fmt);
    while(fmt = strchr(fmt, '%'))
    {
        switch(*++fmt)
        {
            xcase 's':    pktSendStringAligned(pak_out, va_arg(va, char*));
            xcase 'd':    pktSendBitsAuto(pak_out, va_arg(va, int));
            xcase '%':    // do nothing
            xdefault:    assertmsgf(0, "Unhandled printf type '%c'", *fmt);
        }
    }
    pktSend(&pak_out, link);
}

static void s_messageEntity(NetLink *link, int entid, FORMAT fmt, ...)
{
    VA_START(va, fmt);
    s_messageEntityv(link, entid, INFO_ARCHITECT, fmt, va);
    VA_END();
}

static int s_remoteMetaPrintf(const char *fmt, va_list va)
{
    CallInfo *call = meta_getCallInfo();
    if(call->route_steps == 2) // bit of a hack, 2 steps came from a client (client->map->dbserver)
    {
        int channel = call->callflags & CALLFLAG_CONSOLE ? INFO_CONSOLE : INFO_ARCHITECT;
        s_messageEntityv(call->link, call->route_ids[0], channel, fmt, va);
    }
    return 0;
}

static AuthRegion s_getRemoteAuthRegion(void)
{
    // this isn't really how I want this to work, but it'll do
    CallInfo *call = meta_getCallInfo();
    if(call && call->link)
    {
        MissionServerShard *shard = call->link->userData;
        return shard->auth_region;
    }
    else
    {
        return -1;
    }
}

static void s_sendPetitionString(NetLink *link, char *authname, AuthRegion authregion,
                                 char *character, char *summary, char *msg, char *notes)
{
    if(authname && authname[0])
    {
        if(link)
        {
            MissionServerShard *shard = link->userData;
            Packet *pak_out = pktCreateEx(link, MISSION_SERVER_SENDPETITION);
            pktSendStringf(pak_out, authname);                        // authname
            pktSendString(pak_out, character ? character : "N/A");    // name
            pktSendBitsAuto(pak_out, timerSecondsSince2000());        // date
            pktSendString(pak_out, summary);                        // summary
            pktSendString(pak_out, msg);                            // msg
            pktSendBitsAuto(pak_out, PETITIONCATEGORY_CONDUCT);        // category
            pktSendString(pak_out, notes);
            pktSend(&pak_out, link);

            LOG( LOG_MISSIONSERVER, LOG_LEVEL_VERBOSE, 0, "Sent petition to %s.%s", authregion_toString(shard->auth_region), shard->name);
        }
        else if(authregion != AUTHREGION_DEV)
        {
            MissionServerPetition *petition = as_push(&g_missionServerState.petitions[authregion]);
            Str_catf(&petition->authname, authname);
            if(character)
                Str_catf(&petition->character, character);
            Str_catf(&petition->summary, summary);
            Str_catf(&petition->message, msg);
            Str_catf(&petition->notes, notes);
        }
        else
        {
            LOG( LOG_MISSIONSERVER, LOG_LEVEL_VERBOSE, LOG_CONSOLE, "Unsent Petition: %s\n%s", summary, msg);
        }
    }
    else
    {
        LOG( LOG_MISSIONSERVER, LOG_LEVEL_VERBOSE, LOG_CONSOLE, "Unsent Petition: %s\n%s", summary, msg);
    }
}

static void s_sendArcPetition(NetLink *link, MissionServerArc *arc)
{
    int i;
    char summary[128];
    char msg[1024];
    char *buf = Str_temp();
    MissionServerUser *user = s_findCreator(arc);

    sprintf(summary, MISSIONSERVER_PETITIONTAG" Arc %d received", arc->id);

    sprintf(msg,"Hello, This is an auto-generated email created for you to "
                "alert In-game Support that your Architect mission [%s] has "
                "been submitted for review. A Game Master will contact you "
                "once they have a chance to review your account and your "
                "missions. Take Care.", arc->header.title);

    Str_catf(&buf, "Arc for review: %d\n", arc->id);
    Str_catf(&buf, "Title: %s\n", arc->header.title);
    Str_catf(&buf, "Author: %s [%s]\n", arc->header.author, arc->header.author_shard);
    Str_catf(&buf, "Auth Id: "USER_PRINTF_FMT"\n", USER_PRINTF_EX(arc->creator_id, arc->creator_region));
    if(user->authname)
        Str_catf(&buf, "Auth Name: %s\n", user->authname);
    Str_catf(&buf, "Comments or Complaints:\n");
    for(i = 0; i < eaSize(&arc->complaints); i++)
    {
        MissionComplaint *complaint = arc->complaints[i];
        if(  complaint->type == kMissionComplaint_Complaint ||complaint->type == kMissionComplaint_BanMessage )
        {
            if( complaint->globalName )
                Str_catf(&buf, " * "USER_PRINTF_FMT" %s %s\n", USER_PRINTF_EX(complaint->complainer_id, complaint->complainer_region), complaint->globalName, complaint->comment);
            else
                Str_catf(&buf, " * "USER_PRINTF_FMT" %s\n", USER_PRINTF_EX(complaint->complainer_id, complaint->complainer_region), complaint->comment);
        }
    }
    Str_catf(&buf, "\n");
    Str_catf(&buf, "The in-game search and the following commands can be used for more information:\n");
    Str_catf(&buf, "/missionserver_arcinfo %d\n", arc->id);
    Str_catf(&buf, "/architect_get_file %d\n", arc->id);
    Str_catf(&buf, "/missionserver_userinfo %d %s", arc->creator_id, authregion_toString(arc->creator_region));
    Str_catf(&buf, "The in-game search or one of the following commands should be used to resolve this submission:\n");
    Str_catf(&buf, "/missionserver_arcban %d\n", arc->id);
    Str_catf(&buf, "/missionserver_arcapprove %d\n", arc->id);

    s_sendPetitionString(link, user->authname, user->auth_region, arc->header.author, summary, msg, buf);

    Str_destroy(&buf);
}

static void s_Str_catPulledArcs(char **buf, int *arcids, const char *note)
{
    int i;
    for(i = 0; i < eaiSize(&arcids); i++)
    {
        MissionServerArc *arc = plFind(MissionServerArc, arcids[i]);
        if(!missionban_isVisible(arc->ban))
            Str_catf(buf, " * %d%s%s: %s\n", arc->id, note, arc->invalidated ? " (invalidated)" : "", arc->header.title);
    }
}

static void s_sendUserPetition(NetLink *link, MissionServerUser *user)
{
    char summary[128];
    char msg[1024];
    char *buf = Str_temp();

    sprintf(summary, MISSIONSERVER_PETITIONTAG" Player %s suspended", user->authname ? user->authname : "<unknown>");

    sprintf(msg,"Hello, This is an auto-generated email created for you to "
                "alert In-game Support that your Mission Architect access "
                "has been automatically suspended ["USER_PRINTF_FMT"]. A Game "
                "Master will contact you once they have a chance to review your "
                "account and your missions. Take Care.", USER_PRINTF(user));

    Str_catf(&buf, "Auth Id: "USER_PRINTF_FMT"\n", USER_PRINTF(user));
    if(user->authname)
        Str_catf(&buf, "Auth Name: %s\n", user->authname);
    Str_catf(&buf, "Pulled Arcs:\n");
    s_Str_catPulledArcs(&buf, user->arcids_permanent, " (permanent)");
    s_Str_catPulledArcs(&buf, user->arcids_published, "");
    s_Str_catPulledArcs(&buf, user->arcids_unpublished, " (unpublished)");
    Str_catf(&buf, "\n");
    Str_catf(&buf, "The in-game search and the following commands can be used for more information:\n");
    Str_catf(&buf, "/missionserver_userinfo %d %s\n", user->auth_id, authregion_toString(user->auth_region));
    Str_catf(&buf, "/architect_search_authored %d %s\n", user->auth_id, authregion_toString(user->auth_region));
    Str_catf(&buf, "The following command can be used to resolve this issue (0 days unbans):\n");
    Str_catf(&buf, "/missionserver_userban %d %s <days>\n", user->auth_id, authregion_toString(user->auth_region));

    s_sendPetitionString(user->shard_link, user->authname, user->auth_region,
                            NULL, summary, msg, buf);

    Str_destroy(&buf);
}

META(ACCESSLEVEL("3") CONSOLE("set") CONSOLE_DESC("modifies the missionserver's live config. call with no parameters for more info.")
     CONSOLE_REQPARAMS("0") )
void missionserver_setLiveConfig(char *var, char *val)
{
    int i;
    char *buf = Str_temp();
    if(!var || !var[0] || !val || !val[0])
    {
        Str_catf(&buf, "Syntax: missionserver_set <variable> <value>\n");
        Str_catf(&buf, "You may modify the following variables:\n");
        FORALL_PARSETABLE(parse_MissionServerVars, i)
        {
            ParseTable *line = &parse_MissionServerVars[i];
            if(line->type & TOK_REDUNDANTNAME)
                continue;
            switch(TOK_GET_TYPE(line->type))
            {
                xcase TOK_INT_X:
                    Str_catf(&buf, "   %5d %s\n", TokenStoreGetInt(parse_MissionServerVars, i, g_missionServerState.vars, 0), line->name);
                xcase TOK_F32_X:
                    Str_catf(&buf, "   %5f %s\n", TokenStoreGetF32(parse_MissionServerVars, i, g_missionServerState.vars, 0), line->name);
                xcase TOK_IGNORE:
                    ; // do nothing
            }
        }
    }
    else
    {
        FORALL_PARSETABLE(parse_MissionServerVars, i)
        {
            ParseTable *line = &parse_MissionServerVars[i];
            if(TOK_GET_TYPE(line->type) == TOK_IGNORE)
                continue;
            if(stricmp(line->name, var) == 0)
            {
                switch(TOK_GET_TYPE(line->type))
                {
                    xcase TOK_INT_X:
                        TokenStoreSetInt(parse_MissionServerVars, i, g_missionServerState.vars, 0, atoi(val));
                    xcase TOK_F32_X:
                        TokenStoreSetF32(parse_MissionServerVars, i, g_missionServerState.vars, 0, atof(val));
                }
                Str_catf(&buf, "%s set.", line->name);
                plDirty(MissionServerVars, g_missionServerState.vars);
                plFlushStart(MissionServerVars, 0); // this db is small, write it now.
                break;
            }
        }
        if(!parse_MissionServerVars[i].name || !parse_MissionServerVars[i].name[0])
            Str_catf(&buf, "Unknown variable.");
    }
    meta_printf("%s", buf);
    Str_destroy(&buf);
}

META(ACCESSLEVEL("11") REMOTE)
void missionserver_unpublishArc(int arcid, int auth_id, int accesslevel)
{
    AuthRegion auth_region = s_getRemoteAuthRegion();
    MissionServerArc *arc = plFind(MissionServerArc, arcid);
    if(!arc)
    {
        LOG( LOG_MISSIONSERVER, LOG_LEVEL_VERBOSE, 0, "Arc %d requested for unpublishing, but it does not exist", arcid);
        meta_printf("MissionServerUnpublishFailNonexistant");
    }
    else if(!accesslevel &&
            (!s_isArcOwned(arc, auth_id, auth_region) || missionban_locksSlot(arc->ban)) )
    {
        LOG( LOG_MISSIONSERVER, LOG_LEVEL_VERBOSE, 0, "Arc %d requested for unpublishing, but user does not have access ("USER_PRINTF_FMT" %d %s)",
                        arcid, USER_PRINTF_EX(auth_id, auth_region), accesslevel, missionban_toString(arc->ban));
        meta_printf("MissionServerUnpublishFailAccess");
    }
    else if(arc->unpublished)
    {
        LOG( LOG_MISSIONSERVER, LOG_LEVEL_VERBOSE, 0, "Arc "ARC_PRINTF_FMT" by "USER_PRINTF_FMT" requested for unpublishing, but it's already unpublished",
                        ARC_PRINTF(arc), USER_PRINTF_EX(auth_id, auth_region));
        meta_printf("MissionServerUnpublishFailAccess");
    }
    else
    {
        MissionServerUser *creator = s_findCreator(arc);
        assert(    eaiSortedFindAndRemove(&creator->arcids_published, arc->id) >= 0 ||
                eaiSortedFindAndRemove(&creator->arcids_permanent, arc->id) >= 0 );
        eaiSortedPushAssertUnique(&creator->arcids_unpublished, arc->id);
        missionserver_UnindexArc(arc);
        s_addRemoveArcLevelRange(arc, 0);
        estrDestroy(&arc->header_estr);

        arc->unpublished = timerSecondsSince2000();
        missionserver_journalArcUnpublish(arc);

        meta_printf("MissionServerUnpublishSuccess%s", arc->header.title);
        creator->inventory.dirty = true;    //we have to resend the inventory since the # publishes has changed.
        if((!creator->shard_dbid || !creator->shard_link) && s_isArcOwned(arc, auth_id, auth_region))
        {
            CallInfo *caller = meta_getCallInfo();
            if(caller->calltype != CALLTYPE_NULL)
            {
                //todo: switch this so that it checks either the length of the route or the origin to make sure the id is a client's
                creator->shard_dbid = caller->route_ids[0];
                creator->shard_link = caller->link;
            }
        }
        LOG( LOG_MISSIONSERVER, LOG_LEVEL_VERBOSE, LOG_CONSOLE, "Arc %d unpublished by "USER_PRINTF_FMT, arcid, USER_PRINTF_EX(auth_id, auth_region));
    }
}

META(ACCESSLEVEL("11") REMOTE)
void missionserver_updateLevelRange(int arcid, int low, int high)
{
    MissionServerArc *arc = plFind(MissionServerArc, arcid);
    if(!arc)
    {
        LOG( LOG_MISSIONSERVER, LOG_LEVEL_VERBOSE, 0, "Arc %d requested for updateLevelRange, but it does not exist", arcid);
    }
    else
    {
        s_addRemoveArcLevelRange(arc, 0);
        arc->header.levelRangeHigh = high;
        arc->header.levelRangeLow = low;
        s_addRemoveArcLevelRange(arc, 1);
        plDirty(MissionServerArc, arc);
        LOG( LOG_MISSIONSERVER, LOG_LEVEL_VERBOSE, 0, "Arc %d had its level range updated to (%d-%d) ",arcid, low, high);
    }
}


META(ACCESSLEVEL("9") REMOTE CONSOLE("arc_invalidate") CONSOLE_DESC("arc_invalidate <arcid> sends an invalidation command to the mission server"))
void missionserver_invalidateArc(int arcid, int unableToPlay, char *error)
{
    AuthRegion auth_region = s_getRemoteAuthRegion();
    MissionServerArc *arc = plFind(MissionServerArc, arcid);
    if(!arc)
    {
        LOG( LOG_MISSIONSERVER, LOG_LEVEL_VERBOSE, 0, "Arc %d requested to be invalidated, but it does not exist", arcid);
    }
    else if(arc->invalidated >= MISSIONSERVER_INVALIDATIONCOUNT)
    {
        LOG( LOG_MISSIONSERVER, LOG_LEVEL_VERBOSE, 0, "Arc "ARC_PRINTF_FMT" requested to be invalidated, but it already is", ARC_PRINTF(arc));
    }
    else
    {
        MissionServerUser *creator = s_findCreator(arc);
        int updatePlayer = 0;

        if(unableToPlay)
        {
            if(++arc->invalidated < MISSIONSERVER_INVALIDATIONCOUNT)
            {
                LOG( LOG_MISSIONSERVER, LOG_LEVEL_VERBOSE, 0, "Arc "ARC_PRINTF_FMT" invalidation %d Reason %s", ARC_PRINTF(arc), arc->invalidated, error);
            }
            else
            {
                missionserver_UnindexArc(arc);
                s_addRemoveArcLevelRange(arc, 0);
                estrDestroy(&arc->header_estr);
                arc->invalidated = timerSecondsSince2000();
                LOG( LOG_MISSIONSERVER, LOG_LEVEL_VERBOSE, 0, "Arc "ARC_PRINTF_FMT" invalidated Reason %s", ARC_PRINTF(arc), error);
                updatePlayer = 1;
            }
            missionserver_journalArcInvalidate(arc);
        }
        else
        {
            if(++arc->playable_errors < MISSIONSERVER_PLAYABLEINVALIDATIONCOUNT)
            {
                LOG( LOG_MISSIONSERVER, LOG_LEVEL_VERBOSE, 0, "Arc "ARC_PRINTF_FMT" playable invalidation %d Reason %s", ARC_PRINTF(arc), arc->playable_errors, error);
            }
            else
            {
                missionserver_UnindexArc(arc);
                s_addRemoveArcLevelRange(arc, 0);
                estrDestroy(&arc->header_estr);
                arc->playable_errors = timerSecondsSince2000();
                LOG( LOG_MISSIONSERVER, LOG_LEVEL_VERBOSE, 0, "Arc "ARC_PRINTF_FMT" playable invalidated Reason %s", ARC_PRINTF(arc), error);
                updatePlayer = 1;
            }
            missionserver_journalArcPlayableInvalidate(arc);
        }
        if(updatePlayer)
        {
            creator->inventory.dirty = true;    //we have to resend the inventory since the # publishes has changed.
            //if we don't have enough info to send to the player, we can't do much about it from this call.
            /*if((!creator->shard_dbid || !creator->shard_link))
            {
            }*/
        }
    }
}

static void s_restoreArc(MissionServerArc *arc)
{
    MissionServerUser *creator = s_findCreator(arc);
    assert(arc->unpublished);
    assert(eaiSortedFindAndRemove(&creator->arcids_unpublished, arc->id) >= 0);
    if(arc->permanent)
        eaiSortedPushAssertUnique(&creator->arcids_permanent, arc->id);
    else
        eaiSortedPushAssertUnique(&creator->arcids_published, arc->id);

    arc->unpublished = 0;
    if(missionserverarc_isIndexed(arc))
    {
        missionserver_IndexNewArc(arc);
        s_addRemoveArcLevelRange(arc, 1);
    }

    missionserver_journalArcUnpublish(arc);
    missionserver_RestoreArcData(arc);
}

META(ACCESSLEVEL("1") REMOTE CONSOLE("arc_restore") CONSOLE_DESC("Restores unpublished arc <arcid>"))
void missionserver_restoreArc(int arcid)
{
    MissionServerArc *arc = plFind(MissionServerArc, arcid);
    if(!arc || !arc->unpublished)
    {
        LOG( LOG_MISSIONSERVER, LOG_LEVEL_VERBOSE, 0, "Arc %d requested for restoration, but it does not exist", arcid);
        meta_printf("MissionServerCommandFailNonexistant");
    }
    else
    {
        s_restoreArc(arc);
        LOG( LOG_MISSIONSERVER, LOG_LEVEL_VERBOSE, 0, "Arc "ARC_PRINTF_FMT" restored via command", ARC_PRINTF(arc));
        meta_printf("MissionServerCommandSuccess");
    }
}

static void s_updateUserHonors(MissionServerUser *creator, MissionHonors honors)
{
    if(creator)
    {
        U32 oldbadges = creator->inventory.badge_flags;

        if(honors == MISSIONHONORS_DEVCHOICE || honors == MISSIONHONORS_DC_AND_HOF)
            creator->inventory.badge_flags |= kBadgeArcDevChoice;
        else if(honors == MISSIONHONORS_HALLOFFAME || honors == MISSIONHONORS_DC_AND_HOF)
            creator->inventory.badge_flags |= kBadgeArcHallOfFame;

        if(creator->inventory.badge_flags != oldbadges)
        {
            LOG( LOG_MISSIONSERVER, LOG_LEVEL_VERBOSE, 0, "User "USER_PRINTF_FMT" badge flags %x", USER_PRINTF(creator), creator->inventory.badge_flags);
            missionserver_journalUserInventory(creator);
            creator->inventory.dirty = true;
        }
    }
}
//TODO: remove this function and make honors a bitfield.  It should work in search just like villain groups.
static int s_translate_DC_honors(int oldHonors, int newHonors)
{
    if((oldHonors == MISSIONHONORS_DEVCHOICE && newHonors == MISSIONHONORS_HALLOFFAME)||
        (oldHonors == MISSIONHONORS_HALLOFFAME && newHonors == MISSIONHONORS_DEVCHOICE) ||
        (oldHonors == MISSIONHONORS_FORMER_DC_HOF && newHonors == MISSIONHONORS_DEVCHOICE))
        newHonors = MISSIONHONORS_DC_AND_HOF;
    else if(oldHonors == MISSIONHONORS_DEVCHOICE || 
        oldHonors == MISSIONHONORS_FORMER_DC || 
        oldHonors == MISSIONHONORS_FORMER_DC_HOF)
    {
        if(newHonors == MISSIONHONORS_NONE)
            newHonors = MISSIONHONORS_FORMER_DC;
        else if(newHonors == MISSIONHONORS_HALLOFFAME)
            newHonors = MISSIONHONORS_FORMER_DC_HOF;
    }
    return newHonors;
}

META(ACCESSLEVEL("1") REMOTE)
void missionserver_changeHonors(int arcid, int ihonors)
{
    MissionServerArc *arc = plFind(MissionServerArc, arcid);
    MissionHonors honors = ihonors;
    MissionHonors oldhonors;

    if(!arc)
    {
        LOG( LOG_MISSIONSERVER, LOG_LEVEL_VERBOSE, 0, "Arc %d requested for honors change, but it does not exist", arcid);
        meta_printf("MissionServerCommandFailNonexistant");
    }
    else if(!INRANGE0(honors, MISSIONHONORS_TYPES))
    {
        LOG( LOG_MISSIONSERVER, LOG_LEVEL_VERBOSE, 0, "Arc %d requested for honors change to invalid type %d", arcid, honors);
        meta_printf("MissionServerCommandFailInvalidStatus");
    }
    else
    {
        MissionServerUser *user = s_findCreator(arc);
        assertmsg(user, "Arc lost its creator!");

        oldhonors = arc->honors;
        honors = s_translate_DC_honors(oldhonors, honors);

        if(arc->honors != honors)
        {
            missionserver_UpdateArcHonors(arc, honors);

            if(missionhonors_isPermanent(honors))
                arc->permanent = 1;

            if(arc->permanent)
            {
                // Going from ordinary to permanent so we need to free up
                // the slot in the ordinary list.
                eaiSortedFindAndRemove(&user->arcids_published, arcid);
                eaiSortedPushUnique(&user->arcids_permanent, arcid);
            }
            else
            {
                // Has it been demoted from permanent status and therefore
                // is moving back to the ordinary list?
                if(eaiSortedFindAndRemove(&user->arcids_permanent, arcid) >= 0)
                    devassertmsg(0, "arc lost permanence?");
                eaiSortedPushUnique(&user->arcids_published, arcid);
            }

            missionserver_journalArcHonors(arc);
            s_updateUserHonors(user, honors);
        }

        meta_printf("MissionServerChangeStatus%s", arc->header.title);
    }
}

static void missionserverarc_changeBan(MissionServerArc *arc, MissionBan ban)
{
    int first_change = arc->ban == MISSIONBAN_NONE;

    if(missionban_isVisible(ban)) // not pulled-for-content
        arc->pulled = 0;
    else if(missionban_isVisible(arc->ban)) // becoming pulled-for-content
        arc->pulled = timerSecondsSince2000();
    // else // still pulled-for-content

    missionserver_UpdateArcBan(arc, ban); // do this first so it'll show up in a user petition

    if(first_change && !missionban_isVisible(arc->ban))
    {
        MissionServerUser *creator = s_findCreator(arc);
        creator->autobans++;
        if(creator->autobans > g_missionServerState.vars->autobans_till_userban)
        {
            creator->banned_until = U32_MAX;
            s_sendUserPetition(meta_getCallInfo()->link, creator);
            LOG( LOG_MISSIONSERVER, LOG_LEVEL_IMPORTANT, 0, "User "USER_PRINTF_FMT" autobanned (sent ticket).", USER_PRINTF(creator));
        }
        missionserver_journalUserBan(creator);
    }

    missionserver_journalArcBanPulled(arc);
}

static void s_changeBan(int arcid, MissionBan ban)
{
    MissionServerArc *arc = plFind(MissionServerArc, arcid);

    if(!arc)
    {
        LOG( LOG_MISSIONSERVER, LOG_LEVEL_VERBOSE, 0, "Arc %d requested for ban change, but it does not exist", arcid);
        meta_printf("MissionServerCommandFailNonexistant");
    }
    else if(!INRANGE0(ban, MISSIONBAN_TYPES))
    {
        LOG( LOG_MISSIONSERVER, LOG_LEVEL_VERBOSE, 0, "Arc %d requested for ban change to invalid ban %d", arcid, ban);
        meta_printf("MissionServerCommandFailInvalidStatus");
    }
    else
    {
        if(arc->ban != ban)
            missionserverarc_changeBan(arc, ban);
        meta_printf("MissionServerChangeStatus%s", arc->header.title);
        if( ban == MISSIONBAN_CSREVIEWED_BANNED && arc && arc->unpublished )
            s_restoreArc(arc);
    }
}

META(ACCESSLEVEL("1") REMOTE CONSOLE("arc_ban") CONSOLE_DESC("Ban arc <arcid>"))
void missionserver_banArc(int arcid)
{
    s_changeBan(arcid, MISSIONBAN_CSREVIEWED_BANNED);
}

META(ACCESSLEVEL("1") REMOTE CONSOLE("arc_approve") CONSOLE_DESC("Make arc <arcid> unbannable") )
void missionserver_unbannableArc(int arcid)
{
    s_changeBan(arcid, MISSIONBAN_CSREVIEWED_APPROVED);
}

META(ACCESSLEVEL("1") CONSOLE("arc_info") CONSOLE_DESC("Get exhaustive info on <arcid>"))
void missionserver_printArcInfo(int arcid)
{
    MissionServerArc *arc = plFind(MissionServerArc, arcid);
    if(!arc)
    {
        LOG( LOG_MISSIONSERVER, LOG_LEVEL_VERBOSE, 0, "Arc %d requested for info, but it does not exist", arcid);
        meta_printf("MissionServerCommandFailNonexistant");
    }
    else
    {
        int i;
        char datebuf[20];
        char *buf = Str_temp();

        Str_catf(&buf, "Arc Id: %d\n", arc->id);
        Str_catf(&buf, "Creator Id: "USER_PRINTF_FMT"\n", USER_PRINTF_EX(arc->creator_id, arc->creator_region));
        Str_catf(&buf, "Author: %s (%s)\n", arc->header.author, arc->header.author_shard);
        Str_catf(&buf, "Title: %s\n", arc->header.title);
        Str_catf(&buf, "Created: %s\n", timerMakeDateStringFromSecondsSince2000(datebuf, arc->created));
        if(arc->updated != arc->created)
            Str_catf(&buf, "Updated: %s\n", timerMakeDateStringFromSecondsSince2000(datebuf, arc->updated));
        if(arc->pulled)
            Str_catf(&buf, "Pulled: %s\n", timerMakeDateStringFromSecondsSince2000(datebuf, arc->pulled));
        if(arc->unpublished)
            Str_catf(&buf, "Unpublished: %s (not searchable)\n", timerMakeDateStringFromSecondsSince2000(datebuf, arc->unpublished));
        if(arc->invalidated < MISSIONSERVER_INVALIDATIONCOUNT)
            Str_catf(&buf, "Invalidations: %d (still searchable)\n", arc->invalidated);
        else
            Str_catf(&buf, "Invalidated: %s (not searchable)\n", timerMakeDateStringFromSecondsSince2000(datebuf, arc->invalidated));
        if(arc->playable_errors < MISSIONSERVER_PLAYABLEINVALIDATIONCOUNT)
            Str_catf(&buf, "Playable Error Reports: %d (haven't informed author)\n", arc->playable_errors);
        else
            Str_catf(&buf, "Playable Errors: %s (player informed)\n", timerMakeDateStringFromSecondsSince2000(datebuf, arc->playable_errors));
        Str_catf(&buf, "Ban: %s\n", missionban_toString(arc->ban));
        Str_catf(&buf, "Honors: %s\n", missionhonors_toString(arc->honors));
        if(arc->permanent)
            Str_catf(&buf, "Permanent (does not count against slots)\n");
        if(arc->rating == MISSIONRATING_UNRATED)
            Str_catf(&buf, "Rating: Unrated");
        else
            Str_catf(&buf, "Rating: %s", missionrating_toString(arc->rating));
        if(!missionban_isVisible(arc->ban))
            Str_catf(&buf, " (not visible due to ban)\n");
        else
            Str_catf(&buf, "\n");
        for(i = 0; i < ARRAY_SIZE(arc->votes); i++)
            Str_catf(&buf, "Votes %s: %d\n", missionrating_toString(i), arc->votes[i]);
        Str_catf(&buf, "Starts: (%d) (Note: may have less starts than completes since this stat was added after completes)\n", arc->total_starts);
        Str_catf(&buf, "Completions: (%d)\n", arc->total_completions);
        Str_catf(&buf, "Complaints (%d):\n", eaSize(&arc->complaints));
        for(i = 0; i < eaSize(&arc->complaints); i++)
        {
            MissionComplaint *complaint = arc->complaints[i];
            if( complaint->globalName )    
                Str_catf(&buf, "    "USER_PRINTF_FMT" %s: ", USER_PRINTF_EX(complaint->complainer_id, complaint->complainer_region), complaint->globalName );
            else
                Str_catf(&buf, "    "USER_PRINTF_FMT" : ", USER_PRINTF_EX(complaint->complainer_id, complaint->complainer_region) );
            if(complaint->comment)    //this can be NULL somehow
                Str_catprintable(&buf, complaint->comment);
            Str_catf(&buf, "\n");
        }
        Str_catf(&buf, "Data Size (uncompressed): %d\n", arc->size);
        Str_catf(&buf, "Data Size (compressed): %d\n", arc->zsize);
        Str_catf(&buf, "\n");

        meta_printf("%s", buf);
        Str_destroy(&buf);
    }
}

static void s_Str_catarclist(char **buf, const char *header, int *arcids)
{
    int i;
    Str_catf(buf, "%s: %d\n", header, eaiSize(&arcids));
    for(i = 0; i < eaiSize(&arcids); i++)
    {
        MissionServerArc *arc = plFind(MissionServerArc, arcids[i]);
        Str_catf(buf, "  %d %s\n", arc->id, arc->header.title);
    }
}

static U32 s_howManyTicketsForVote(MissionRating rating)
{
    U32 retval = 0;

    switch (rating)
    {
        case MISSIONRATING_0_STARS:
            retval = g_missionServerState.vars->tickets_0_stars;
            break;

        case MISSIONRATING_1_STAR:
            retval = g_missionServerState.vars->tickets_1_star;
            break;

        case MISSIONRATING_2_STARS:
            retval = g_missionServerState.vars->tickets_2_stars;
            break;

        case MISSIONRATING_3_STARS:
            retval = g_missionServerState.vars->tickets_3_stars;
            break;

        case MISSIONRATING_4_STARS:
            retval = g_missionServerState.vars->tickets_4_stars;
            break;

        case MISSIONRATING_5_STARS:
            retval = g_missionServerState.vars->tickets_5_stars;
            break;

        default:
            break;
    }

    return retval;
}

static void s_updateArcRating(MissionServerArc *arc)
{
    if(arc)
    {
        MissionServerUser *creator = s_findCreator(arc);

        F32 rating_val;
        MissionRating rating = s_calculateVotesStarsRating(arc, &rating_val);

        if(rating != arc->rating)
        {
            LOG( LOG_MISSIONSERVER, LOG_LEVEL_VERBOSE, 0, "Arc "ARC_PRINTF_FMT" new rating %s (%d stars, %d votes)", ARC_PRINTF(arc), missionrating_toString(rating), arc->total_stars, arc->total_votes);
            missionserver_UpdateArcRating(arc, rating);
            // this doesn't need to be journaled, rating is calculated on startup
            // if live vars change the rating, the rating won't be reflected in the db until the arc gets a vote
        }

        if(    arc->total_votes >= g_missionServerState.vars->halloffame_votes &&
            rating_val >= g_missionServerState.vars->halloffame_rating )
        {
            s_updateUserHonors(creator, MISSIONHONORS_HALLOFFAME);
            if(arc->honors == MISSIONHONORS_NONE) // only upgrade
                missionserver_changeHonors(arc->id, MISSIONHONORS_HALLOFFAME);
        }
        else
        {
             // don't downgrade from devchoice or celebrity
            if(arc->honors == MISSIONHONORS_HALLOFFAME)
                missionserver_changeHonors(arc->id, MISSIONHONORS_NONE);
            else if(arc->honors == MISSIONHONORS_FORMER_DC_HOF)
                missionserver_changeHonors(arc->id, MISSIONHONORS_FORMER_DC);
        }

        if(creator)
        {
            U32 oldbadges = creator->inventory.badge_flags;

            if(arc->total_votes >= 5 && rating_val >= 3.f)
                creator->inventory.badge_flags |= kBadgeArcVoteRating1;
            if(arc->total_votes >= 10 && rating_val >= 3.25f)
                creator->inventory.badge_flags |= kBadgeArcVoteRating2;
            if(arc->total_votes >= 25 && rating_val >= 3.50f)
                creator->inventory.badge_flags |= kBadgeArcVoteRating3;
            if(arc->total_votes >= 50 && rating_val >= 3.75f)
                creator->inventory.badge_flags |= kBadgeArcVoteRating4;
            if(arc->total_votes >= 100 && rating_val >= 4.0f)
                creator->inventory.badge_flags |= kBadgeArcVoteRating5;

            if(creator->inventory.badge_flags != oldbadges)
            {
                LOG( LOG_MISSIONSERVER, LOG_LEVEL_VERBOSE, 0, "User "USER_PRINTF_FMT" badge flags %x", USER_PRINTF(creator), creator->inventory.badge_flags);
                missionserver_journalUserInventory(creator);
                creator->inventory.dirty = true;
            }
        }
    }
}

static void s_awardUserBadge(MissionServerUser *user, MissionServerBadgeFlags flag)
{
    if(user)
    {
        U32 oldflags = user->inventory.badge_flags;

        user->inventory.badge_flags |= flag;

        if(user->inventory.badge_flags != oldflags)
        {
            missionserver_journalUserInventory(user);
            user->inventory.dirty = true;
        }
    }
}

static void s_grantTicketsToUser(MissionServerUser *user, int tickets)
{
    if(user)
    {
        LOG( LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "Granted %d tickets (old total %d, %d overflow) to "USER_PRINTF_FMT, tickets, user->inventory.tickets, user->inventory.overflow_tickets, USER_PRINTF(user));

        user->inventory.tickets += tickets;

        // Tickets wind up on a character as pieces of salvage and there's
        // only so much of any type they can hold, so we need to cap their
        // count here as well.
        if(user->inventory.tickets > INVENTORY_MAX_TICKETS)
        {
            user->inventory.overflow_tickets += (user->inventory.tickets - INVENTORY_MAX_TICKETS);

            if(user->inventory.overflow_tickets > INVENTORY_MAX_OVERFLOW_TICKETS)
            {
                user->inventory.overflow_tickets = INVENTORY_MAX_OVERFLOW_TICKETS;
            }

            user->inventory.tickets = INVENTORY_MAX_TICKETS;
        }

        user->inventory.lifetime_tickets += tickets;

        // The player can't claim from this so the limit is much higher.
        // We don't want it to wrap around, that's all.
        if(user->inventory.lifetime_tickets > INVENTORY_MAX_LIFETIME_TICKETS)
        {
            user->inventory.lifetime_tickets = INVENTORY_MAX_LIFETIME_TICKETS;
        }

        missionserver_journalUserInventory(user);
        user->inventory.dirty = true;

        if(user->shard_link && user->shard_dbid)
        {
            s_messageEntity(user->shard_link, user->shard_dbid,
                "MissionServerTicketsEarned%d", tickets);
        }
    }
}

static bool s_grantPublishSlotsToUser(MissionServerUser *user, int publish_slots)
{
    bool retval = false;

    if(user && publish_slots > 0)
    {
        LOG( LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "Granting %d extra publish slots to "USER_PRINTF_FMT, publish_slots, USER_PRINTF(user));

        retval = true;

        user->inventory.publish_slots += publish_slots;

        missionserver_journalUserInventory(user);
        user->inventory.dirty = true;
    }

    return retval;
}

static bool s_grantUnlockKeyToUser(MissionServerUser *user, char *unlock_key)
{
    bool retval = 1;
    int i;

    if(user && unlock_key && strlen(unlock_key) > 0)
    {
        LOG( LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "Granting unlock key '%s' to "USER_PRINTF_FMT, unlock_key, USER_PRINTF(user));

        for( i = eaSize(&user->inventory.costume_keys)-1; i>=0; i-- )
        {
            if( stricmp(user->inventory.costume_keys[i], unlock_key)==0)
                retval = 0;
        }

        if( retval )
        {
            eaPush(&user->inventory.costume_keys, strdup(unlock_key));
            missionserver_journalUserInventory(user);
            user->inventory.dirty = true;
        }
    }
    else 
        retval = 0;

    return retval;
}

META(ACCESSLEVEL("11") REMOTE)
void missionserver_grantTickets(int tickets, int db_id, int auth_id)
{
    MissionServerUser *user = plFindOrAdd(MissionServerUser, auth_id, s_getRemoteAuthRegion());

    if(user)
    {
        if(db_id)
        {
            CallInfo *call = meta_getCallInfo();

            user->shard_link = call->link;
            user->shard_dbid = db_id;
        }

        s_grantTicketsToUser(user, tickets);

        meta_printf("MissionServerAwardedTickets%d", tickets);
    }
    else
    {
        meta_printf("MissionServerNoSuchUser");
    }
}

META(ACCESSLEVEL("11") REMOTE)
void missionserver_grantOverflowTickets(int tickets, int db_id, int auth_id)
{
    MissionServerUser *user = plFindOrAdd(MissionServerUser, auth_id, s_getRemoteAuthRegion());

    if(user)
    {
        Packet *pak_out;
        CallInfo *call = meta_getCallInfo();

        if(db_id)
        {
            user->shard_link = call->link;
            user->shard_dbid = db_id;
        }

        user->inventory.overflow_tickets += tickets;

        if(user->inventory.overflow_tickets < 0)
            user->inventory.overflow_tickets = 0;

        missionserver_journalUserInventory(user);

        if(user->shard_link)
        {
            pak_out = pktCreateEx(user->shard_link, MISSION_SERVER_OVERFLOW_GRANTED);
            pktSendBitsAuto(pak_out, user->shard_dbid);
            pktSendBitsAuto(pak_out, user->auth_id);
            pktSendBitsAuto(pak_out, tickets);
            pktSendBitsAuto(pak_out, user->inventory.overflow_tickets);
            pktSend(&pak_out, user->shard_link);
        }
    }
}

META(ACCESSLEVEL("11") REMOTE)
void missionserver_setPaidPublishSlots(int paid_slots, int db_id, int auth_id)
{
    MissionServerUser *user = plFindOrAdd(MissionServerUser, auth_id, s_getRemoteAuthRegion());

    if(user)
    {
        CallInfo *call = meta_getCallInfo();

        if(db_id)
        {
            user->shard_link = call->link;
            user->shard_dbid = db_id;
        }

        // No need to journal or send back inventory if it didn't change.
        if(paid_slots != user->inventory.paid_publish_slots)
        {
            user->inventory.paid_publish_slots = paid_slots;
            missionserver_journalUserInventory(user);
            user->inventory.dirty = true;
        }
    }
}

static void s_giveTicketsForVote(MissionServerArc *arc, MissionRating prevrating, MissionRating newrating)
{
    MissionServerUser *creator;
    U32 prevtickets;
    U32 newtickets;

    assert(arc && newrating < MISSIONRATING_VOTETYPES);

    creator = s_findCreator(arc);

    if (creator)
    {
        prevtickets = s_howManyTicketsForVote(prevrating);
        newtickets = s_howManyTicketsForVote(newrating);

        // We give tickets for the best rating they've ever given the storyarc.
        if (newtickets > prevtickets)
        {
            s_grantTicketsToUser(creator, newtickets - prevtickets);
        }
        else
        {
            if (creator->shard_link && creator->shard_dbid)
            {
                s_messageEntity(creator->shard_link, creator->shard_dbid, "MissionServerVote%s", arc->header.title);
            }
        }
    }
}

static bool s_recordCreatorAndExpireVotes(MissionServerUser *voter, U32 arc_creator_id, AuthRegion arc_creator_region)
{
    int count;
    U32 timestamp = timerSecondsSince2000();
    bool expired = true;
    bool found = false;

    for(count = 0; count < eaiSize(&voter->authids_voted); count++)
    {
        if(voter->authids_voted[count] == arc_creator_id && voter->authids_voted_region[count] == arc_creator_region)
        {
            found = true;

            // Not enough time has expired to wipe away previous votes.
            if(voter->authids_voted_timestamp[count] >= (int)timestamp - (g_missionServerState.vars->days_till_revote * 86400))
            {
                expired = false;
            }

            break;
        }
    }

    if(expired)
    {
        // Clear out any previous votes for any arcs by the creator of this
        // particular arc. We're starting a new 'lockout' period for votes
        // for them by this auth so they get full credit for however many of
        // their arcs this voter can vote for.
        // where we 
        int votes;
        MissionServerArc *arc;

        if(found)
        {
            LOG( LOG_MISSIONSERVER, LOG_LEVEL_VERBOSE, 0, "Old timestamp being removed from "USER_PRINTF_FMT" - was "USER_PRINTF_FMT" at %d", USER_PRINTF(voter), USER_PRINTF_EX(voter->authids_voted[count], voter->authids_voted_region[count]), voter->authids_voted_timestamp[count]);
            eaiRemove(&voter->authids_voted, count);
            eaiRemove((int **)(&voter->authids_voted_region), count);
            eaiRemove(&voter->authids_voted_timestamp, count);
            missionserver_journalUserDeleteUserVote(voter, arc_creator_id, arc_creator_region);
        }

        LOG( LOG_MISSIONSERVER, LOG_LEVEL_VERBOSE, 0, "New timestamp added to "USER_PRINTF_FMT" - is for "USER_PRINTF_FMT" at %d", USER_PRINTF(voter), USER_PRINTF_EX(arc_creator_id, arc_creator_region), timestamp);

        eaiPush(&voter->authids_voted, arc_creator_id);
        eaiPush((int **)(&voter->authids_voted_region), arc_creator_region);
        eaiPush(&voter->authids_voted_timestamp, timestamp);
        missionserver_journalUserRecentUserVote(voter, arc_creator_id, arc_creator_region, timestamp);

        // We do this even if they were not found so that journalling will
        // always work correctly no matter where we get interrupted.
        for(votes = MISSIONRATING_0_STARS; votes <= MISSIONRATING_MAX_STARS; votes++)
        {
            // Work backwards so deleted votes don't need the loop counter
            // adjusted.
            for(count = eaiSize(&voter->arcids_best_votes[votes]) - 1; count >= 0; count--)
            {
                arc = plFind(MissionServerArc, voter->arcids_best_votes[votes][count]);

                if(arc && arc->creator_id == arc_creator_id && arc->creator_region == arc_creator_region)
                {
                    eaiRemove(&voter->arcids_best_votes[votes], count);
                    missionserver_journalUserDeleteArcBestVote(voter, arc->id);
                }
            }
        }
    }

    // Callers want to know if we already found the user in the list (and
    // thus shouldn't award stuff).
    return !expired;
}

static void s_setArcPlayed(MissionServerUser *user, int arcId)
{
    if(user && arcId)
    {
        int index = eaiSortedFind(&user->arcids_played, arcId);
        int timestamp = timerSecondsSince2000();
        
        if (index == -1)
        {
            index = eaiSortedPushUnique(&user->arcids_played, arcId);
            eaiInsert(&user->arcids_played_timestamp, timestamp, index);
            missionserver_journalUserArcCompleted(user, arcId, timestamp);
        }
        else
        {
            user->arcids_played_timestamp[index] = MAX(user->arcids_played_timestamp[index], timestamp);
            missionserver_journalUserArcCompletedUpdate(user, arcId, timestamp);
        }
        
    }
}

META(ACCESSLEVEL("11") REMOTE)
void missionserver_recordArcCompleted(int arcid, int authid)
{
    MissionServerArc *arc = plFind(MissionServerArc, arcid);

    if(arc)
    {
        AuthRegion finisher_region = s_getRemoteAuthRegion();
        MissionServerUser *creator = plFind(MissionServerUser, arc->creator_id, arc->creator_region);

        MissionServerUser *finisher = plFindOrAdd(MissionServerUser, authid, finisher_region);

        arc->total_completions++;

        if(arc->total_completions > INVENTORY_MAX_COMPLETIONS)
        {
            arc->total_completions = INVENTORY_MAX_COMPLETIONS;
        }

        missionserver_journalArcTotalCompletions(arc);

        creator->inventory.created_completions++;

        if(creator->inventory.created_completions > INVENTORY_MAX_COMPLETIONS)
        {
            creator->inventory.created_completions = INVENTORY_MAX_COMPLETIONS;
        }

        missionserver_journalUserInventory(creator);

        creator->inventory.dirty = true;

        LOG( LOG_MISSIONSERVER, LOG_LEVEL_VERBOSE, 0, "Arc "ARC_PRINTF_FMT" completed by "USER_PRINTF_FMT", total completions now %d", ARC_PRINTF(arc), USER_PRINTF_EX(authid, finisher_region), arc->total_completions);

        s_setArcPlayed(finisher, arcid);

        if(eaSize(&arc->first_to_complete) < MISSION_MAX_UNIQUE_PLAYERS)
        {
            int count;
            bool new_finisher = true;

            for(count = 0; count < eaSize(&arc->first_to_complete); count++)
            {
                if(arc->first_to_complete[count]->id == authid && arc->first_to_complete[count]->region == finisher_region)
                {
                    new_finisher = false;
                    break;
                }
            }

            if(new_finisher)
            {
                MissionAuth *finisher_auth = StructAllocRaw(sizeof(MissionAuth));

                finisher_auth->id = authid;
                finisher_auth->region = finisher_region;
                eaPush(&arc->first_to_complete, finisher_auth);

                // By waiting we only create a user for people who have
                // something to store and not for every random person.
                if(!finisher)
                {
                    finisher = plAdd(MissionServerUser, authid, finisher_region);
                }

                if(!(finisher->inventory.badge_flags & kBadgeArcFirstXPlayers))
                {
                    finisher->inventory.badge_flags |= kBadgeArcFirstXPlayers;
                    missionserver_journalUserInventory(creator);
                    finisher->inventory.dirty = true;
                    LOG( LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "Early bird badge awarded to authid "USER_PRINTF_FMT, USER_PRINTF_EX(authid, finisher_region));
                }
            }
        }
    }
}

static void s_setArcStarted(MissionServerUser *user, int arcId)
{
    if(user && arcId)
    {
        int index = eaiSortedFind(&user->arcids_started, arcId);
        int timestamp = timerSecondsSince2000();

        if (index == -1)
        {
            index = eaiSortedPushUnique(&user->arcids_started, arcId);
            eaiInsert(&user->arcids_started_timestamp, timestamp, index);
            missionserver_journalUserArcStarted(user, arcId, timestamp);
        }
        else
        {
            user->arcids_started_timestamp[index] = MAX(user->arcids_started_timestamp[index], timestamp);
            missionserver_journalUserArcStartedUpdate(user, arcId, timestamp);
        }

    }
}

META(ACCESSLEVEL("11") REMOTE)
void missionserver_recordArcStarted(int arcid, int authid)
{
    MissionServerArc *arc = plFind(MissionServerArc, arcid);

    if(arc)
    {
        AuthRegion starter_region = s_getRemoteAuthRegion();
        // Deliberately don't auto-add because we don't know if we'll need
        // it yet.
        MissionServerUser *starter = plFindOrAdd(MissionServerUser, authid, starter_region);

        arc->total_starts++;

        if(arc->total_starts > INVENTORY_MAX_STARTS)
        {
            arc->total_starts = INVENTORY_MAX_STARTS;
        }

        missionserver_journalArcTotalStarts(arc);

        LOG( LOG_MISSIONSERVER, LOG_LEVEL_VERBOSE, 0, "Arc "ARC_PRINTF_FMT" started by "USER_PRINTF_FMT", total starts now %d", 
            ARC_PRINTF(arc), USER_PRINTF_EX(authid, starter_region), arc->total_starts);

        s_setArcStarted(starter, arcid);

    }
}

META(ACCESSLEVEL("1") REMOTE CONSOLE("user_info") CONSOLE_DESC("Get exhaustive info on <authid> <authregion>"))
void missionserver_printUserInfo(int auth_id, char *auth_region_str) // empty region string for current region
{
    AuthRegion auth_region = auth_region_str && auth_region_str[0]
                            ? StaticDefineIntLookupInt(AuthRegionEnum, auth_region_str)
                            : s_getRemoteAuthRegion();
    MissionServerUser *user = plFind(MissionServerUser, auth_id, auth_region);
    if(!user)
    {
        LOG( LOG_MISSIONSERVER, LOG_LEVEL_VERBOSE, 0, "User "USER_PRINTF_FMT" requested for info, but they do not exist", auth_region_str, auth_id);
        meta_printf("MissionServerCommandFailNonexistant");
    }
    else
    {
        char datebuf[20];
        char *buf = Str_temp();

        Str_catf(&buf, "Auth Id: "USER_PRINTF_FMT"\n", USER_PRINTF(user));
        Str_catf(&buf, "Auth Name: %s\n", user->authname ? user->authname : "N/A");
        if(user->banned_until < timerSecondsSince2000())
            user->banned_until = 0;
        if(user->banned_until)
            Str_catf(&buf, "User Banned Until: %s\n", timerMakeDateStringFromSecondsSince2000(datebuf, user->banned_until));
        else
            Str_catf(&buf, "User Not Banned (%d/%d pulled arcs to auto-ban)\n", user->autobans, g_missionServerState.vars->autobans_till_userban);

        s_Str_catarclist(&buf, "Published Arcs", user->arcids_published);
        s_Str_catarclist(&buf, "Permanent Arcs", user->arcids_permanent);
        s_Str_catarclist(&buf, "Unpublished Arcs", user->arcids_unpublished);
//         s_Str_catarclist(&buf, "Played Arcs", user->arcids_votes);
        s_Str_catarclist(&buf, "Reported Arcs", user->arcids_complaints);
        Str_catf(&buf, "\n");

        Str_catf(&buf, "\nStarted Arcs: %d\n", eaiSize(&user->arcids_started));
        Str_catf(&buf, "\nCompleted Arcs: %d\n", eaiSize(&user->arcids_played));
        Str_catf(&buf, "\nPublish slots: %d\n", user->inventory.publish_slots);
        Str_catf(&buf, "\nUncirculated slots: %d\n", user->inventory.uncirculated_slots);
        Str_catf(&buf, "\nPaid publish slots: %d\n", user->inventory.paid_publish_slots);
        Str_catf(&buf, "Completed arcs: %d\n", user->inventory.created_completions);

        Str_catf(&buf, "\nCurrent tickets: %d\n", user->inventory.tickets);
        Str_catf(&buf, "Lifetime tickets: %d\n", user->inventory.lifetime_tickets);
        Str_catf(&buf, "Overflow tickets: %d\n", user->inventory.overflow_tickets);
        Str_catf(&buf, "Badge flags: %08lx\n", user->inventory.badge_flags);

        if(eaSize(&user->inventory.costume_keys))
        {
            Str_catf(&buf, "\n%d total costume keys\n", eaSize(&user->inventory.costume_keys));
        }
        else
        {
            Str_catf(&buf, "\nNo costume keys unlocked\n");
        }
        meta_printf("%s", buf);
        Str_destroy(&buf);
    }
}

META(ACCESSLEVEL("1") CONSOLE("user_ban") CONSOLE_DESC("Ban user <authid> <authregion> for <days> days (0 days to unban)"))
void missionserver_banUser(int auth_id, char *auth_region_str, int days)
{
    AuthRegion auth_region = StaticDefineIntLookupInt(AuthRegionEnum, auth_region_str);
    MissionServerUser *user = plFind(MissionServerUser, auth_id, auth_region);
    if(!user)
    {
        LOG( LOG_MISSIONSERVER, LOG_LEVEL_VERBOSE, 0, "User "USER_PRINTF_FMT" requested for banning, but they do not exist", auth_region_str, auth_id);
        meta_printf("MissionServerCommandFailNonexistant");
    }
    else
    {
        if(days)
        {
            char datebuf[20];
            user->banned_until = timerSecondsSince2000() + days*24*60*60;
            if(user->banned_until < timerSecondsSince2000()) // overflow
                user->banned_until = U32_MAX;
            timerMakeDateStringFromSecondsSince2000(datebuf, user->banned_until);
            LOG( LOG_MISSIONSERVER, LOG_LEVEL_VERBOSE, 0, "User "USER_PRINTF_FMT" banned %d days (until %s)", USER_PRINTF(user), days, datebuf);
            meta_printf("MissionServerUserBan%s", datebuf);
        }
        else
        {
            user->banned_until = 0;
            user->autobans = 0;
            LOG( LOG_MISSIONSERVER, LOG_LEVEL_VERBOSE, 0, "User "USER_PRINTF_FMT" unbanned", USER_PRINTF(user));
            meta_printf("MissionServerUserUnban");
        }
        missionserver_journalUserBan(user);
    }
}

META(REMOTE ACCESSLEVEL("11"))
void missionserver_setBestArcStars(int stars, int db_id, int auth_id)
{
    AuthRegion auth_region = s_getRemoteAuthRegion();
    MissionServerUser *user = plFind(MissionServerUser, auth_id, auth_region);

    if(stars > 0 && user)
    {
        if(db_id)
        {
            CallInfo *call = meta_getCallInfo();

            user->shard_link = call->link;
            user->shard_dbid = db_id;
        }

        user->inventory.best_arc_stars = stars;
        missionserver_journalUserInventory(user);
        user->inventory.dirty = true;
    }
    else
    {
        meta_printf("MissionServerNoSuchUser");
    }
}

META(REMOTE ACCESSLEVEL("11")) // only valid from a mapserver
void missionserver_reportArc(int arcid, int auth_id, int type, const char *comment, const char * globalName)
{
    AuthRegion auth_region = s_getRemoteAuthRegion();
    MissionServerArc *arc = plFind(MissionServerArc, arcid);
    if(!arc)
    {
        LOG( LOG_MISSIONSERVER, LOG_LEVEL_VERBOSE, 0, "Arc %d requested for comment (%s), but it does not exist", arcid, missioncomplaint_toString(type));
        meta_printf("MissionServerVotedFailNonexistant");
    }
    else if(!INRANGE0(auth_region, AUTHREGION_COUNT))
    {
        LOG( LOG_MISSIONSERVER, LOG_LEVEL_VERBOSE, 0, "Arc %d requested for comment (%s), but voter auth region is invalid (%s)", arcid, missioncomplaint_toString(type), authregion_toString(auth_region));
        meta_printf("MissionServerVotedFailNonexistant");
    }
    else if(!INRANGE0(type, kMissionComplaint_Count))
    {
        LOG( LOG_MISSIONSERVER, LOG_LEVEL_VERBOSE, 0, "Arc %d requested for comment (%s), but comment type is invalid", arcid, missioncomplaint_toString(type));
        meta_printf("MissionServerVotedFailNonexistant");
    }
    else
    {
        int i = -1;
        char *buf = Str_temp();
        MissionServerUser *voter = plFindOrAdd(MissionServerUser, auth_id, auth_region);

        LOG( LOG_MISSIONSERVER, LOG_LEVEL_VERBOSE, 0, "User "USER_PRINTF_FMT" commenting (%s) on "ARC_PRINTF_FMT, USER_PRINTF(voter), missioncomplaint_toString(type), ARC_PRINTF(arc));

        if(type == kMissionComplaint_Complaint)
        {
            // record the complaint
            eaiSortedPushUnique(&voter->arcids_complaints, arc->id);
            missionserver_journalUserComplaint(voter, arc->id);

            // do some consolidation
            for(i = eaSize(&arc->complaints)-1; i >= 0; --i)
            {
                MissionComplaint *complaint = arc->complaints[i];
                if(    complaint->complainer_id == voter->auth_id &&
                    complaint->complainer_region == voter->auth_region &&
                    complaint->type == type )
                {
                    if(complaint->comment && complaint->comment[0])
                        Str_printf(&buf, "%s // %s", complaint->comment, comment);
                    else
                        Str_cp_raw(&buf, comment);
                    StructReallocString(&complaint->globalName, globalName);
                    StructReallocString(&complaint->comment, buf);
                    complaint->bSent = 0;
                    break;
                }
            }
        }

        if(i < 0) // new comment
        {
            MissionComplaint *complaint = StructAllocRaw(sizeof(*complaint));
            complaint->complainer_id = voter->auth_id;
            complaint->complainer_region = voter->auth_region;
            complaint->comment = StructAllocString(comment);
            complaint->globalName = StructAllocString(globalName);
            complaint->type = type;
            eaPush(&arc->complaints, complaint);

            if(type == kMissionComplaint_Complaint)
            {
                arc->new_complaints++;
                if(arc->new_complaints >= s_instaBanThreshold(arc))
                {
                    if(arc->ban == MISSIONBAN_NONE)
                        missionserverarc_changeBan(arc, MISSIONBAN_PULLED);
                    else if(arc->ban == MISSIONBAN_SELFREVIEWED)
                        missionserverarc_changeBan(arc, MISSIONBAN_SELFREVIEWED_PULLED);
                    // else arc can't be pulled
                }
            }
        }

        missionserver_journalArc(arc); // just journal all changes
        if(type == kMissionComplaint_Comment)
            meta_printf("MissionServerCommented%s", arc->header.title);
        else
            meta_printf("MissionServerVotedContent%s", arc->header.title);
        

        Str_destroy(&buf);
    }
}

META(ACCESSLEVEL("1") CONSOLE("removecomplaint") CONSOLE_DESC("Remove complaint on <arcid> made by <authid> <authregion>"))
void missionserver_removecomplaint(int arcid, int complainer_id, char *complainer_region_str)
{
    AuthRegion complainer_region = StaticDefineIntLookupInt(AuthRegionEnum, complainer_region_str);
    MissionServerArc *arc = plFind(MissionServerArc, arcid);
    if(arc)
    {
        int i;
        for(i = 0; i < eaSize(&arc->complaints); i++)
        {
            MissionComplaint *complaint = arc->complaints[i];
            if(complaint->complainer_id == complainer_id && complaint->complainer_region == complainer_region)
                break;
        }
        if(i < eaSize(&arc->complaints))
        {
            eaRemove(&arc->complaints, i);
            meta_printf("MissionServerCommandSuccess");
        }
        else
        {
            meta_printf("MissionServerCommentFailNonexistant");
        }
    }
    else
    {
        LOG( LOG_MISSIONSERVER, LOG_LEVEL_VERBOSE, 0, "Arc %d requested for complaint hiding, but it does not exist", arcid);
        meta_printf("MissionServerCommentFailNonexistant");
    }
}

META(REMOTE ACCESSLEVEL("11")) // only valid from a mapserver
void missionserver_comment(int arcid, int auth_id, const char *comment)
{
    AuthRegion auth_region = s_getRemoteAuthRegion();
    MissionServerArc *arc = plFind(MissionServerArc, arcid);

    if(arc)
    {
        MissionServerShard * shard = MissionServer_GetShard(arc->header.author_shard);
        if( shard )
        {
            Packet *pak = pktCreateEx(&shard->link,MISSION_SERVER_COMMENT); 

            pktSendString(pak,arc->header.author);
            pktSendString(pak,arc->header.title);
            pktSendBitsAuto(pak, arc->creator_id);
            pktSendBitsAuto(pak, auth_id);
            pktSendString(pak,comment);
            pktSend(&pak,&shard->link);
            meta_printf("MissionServerCommented%s", arc->header.author);
        }
        else
        {
            // non-existant shards?
        }
    }
    else
    {
        LOG( LOG_MISSIONSERVER, LOG_LEVEL_VERBOSE, 0, "Arc %d requested for comment, but it does not exist", arcid);
        meta_printf("MissionServerCommentFailNonexistant");
    }
}

void missionserver_UpdateArcTopKeywords(MissionServerArc *arc, U32 keywords)
{
    int i, top[3] = {-1,-1,-1};
    int newTop = 0;
    for( i = 0; i < MISSIONSERVER_MAX_KEYWORDS; i++  )
    {
        if( keywords & (1<<i) )
            arc->keyword_votes[i]++;

        // used to update top 3 list
        if( top[0] == -1 || arc->keyword_votes[i] > arc->keyword_votes[top[0]] )
        {
            top[2] = top[1];
            top[1] = top[0];
            top[0] = i;
        }
        else if( top[1] == -1 || arc->keyword_votes[i] > arc->keyword_votes[top[1]] )
        {
            top[2] = top[1];
            top[1] = i;
        }
        else if( top[2] == -1 || arc->keyword_votes[i] > arc->keyword_votes[top[2]] )
        {
            top[2] = i;
        }
    }

    for( i = 0; i < 3; i++ )
    {
        if( top[i] >= 0  && arc->keyword_votes[top[i]]>0)
            newTop |= (1 << top[i] );
    }

    if(newTop != arc->top_keywords)
        missionserver_UpdateArcKeywords(arc, newTop); // call to change arc->ban
}

META(REMOTE ACCESSLEVEL("11")) // only valid from a mapserver
void missionserver_setKeywordsForArc( int accesslevel, int arcid, int auth_id, int keywords )
{
    MissionServerArc *arc = plFind(MissionServerArc, arcid);
    AuthRegion auth_region = s_getRemoteAuthRegion();

    // you can only vote for your own arcs if you have accesslevel (that's
    // allowed for testing)
    if(arc && INRANGE0(auth_region, AUTHREGION_COUNT) && (accesslevel || arc->creator_id != auth_id || arc->creator_region != auth_region))
    {
        MissionServerUser *voter = plFindOrAdd(MissionServerUser, auth_id, auth_region);
        MissionServerKeywordVote *pVote = 0;
        int i;

        for( i = 0; i < eaSize(&voter->ppKeywordVotes); i++ )
        {
            if( arcid == voter->ppKeywordVotes[i]->arcid )
            {
                pVote = voter->ppKeywordVotes[i];
                break;
            }
        }

        if( pVote ) // remove prior vote
        {
            for( i = 0; i < MISSIONSERVER_MAX_KEYWORDS; i++  )
            {
                if( pVote->vote & (1<<i) )
                    arc->keyword_votes[i]--;
            }
        }
        else
        {
            pVote = StructAllocRaw(sizeof(MissionServerKeywordVote));
            pVote->arcid = arcid;
        }
        pVote->vote = keywords;

        missionserver_UpdateArcTopKeywords(arc, keywords);

        // Update search buckets here?
        missionserver_journalArcKeywords(arc);
        missionserver_journalUserArcKeywordVote(voter, arc->id, keywords);

        if (voter->shard_link && voter->shard_dbid)
        {
            s_messageEntity(voter->shard_link, voter->shard_dbid, "MissionServerKeyword%s", arc->header.title);
        }
    }
    else
    {
        if(arc && auth_id == arc->creator_id && auth_region == arc->creator_region)
        {
            meta_printf("MissionServerCantVoteOwnArc");
        }
        else
        {
            LOG( LOG_MISSIONSERVER, LOG_LEVEL_VERBOSE, 0, "Arc %d requested for voting, but it does not exist or the voter is invalid", arcid);
            meta_printf("MissionServerVotedFailNonexistant");
        }
    }
}

META(ACCESSLEVEL("10") CONSOLE("setCirculationStatus") CONSOLE_DESC("Set the circulation status of arc <arcid> made by <authid> <authregion> to <on/off>"))
void missionserver_setCirculationStatus(int accesslevel, int arcid, int auth_id, int circulate)
{
    MissionServerArc *arc = plFind(MissionServerArc, arcid);
    AuthRegion auth_region = s_getRemoteAuthRegion();
    MissionServerUser *user =  plFindOrAdd(MissionServerUser, auth_id, auth_region);
    int publisedArcs, uncirculatedArcs, serverArcsAllowed, publishedArcsAllowed, totalArcs;

    if(user)
    {
        //these are just to make the below if clauses readable.
        publisedArcs = eaiSize(&user->arcids_published);
        uncirculatedArcs = eaiSize(&user->arcids_uncirculated);
        totalArcs = publisedArcs + uncirculatedArcs;
        publishedArcsAllowed = user->inventory.publish_slots + user->inventory.paid_publish_slots;
        serverArcsAllowed = publishedArcsAllowed +user->inventory.uncirculated_slots;
    }

    

    if(!arc)
    {
        LOG( LOG_MISSIONSERVER, LOG_LEVEL_VERBOSE, 0, "Arc %d requested for circulation status change, but it does not exist", arcid);
        meta_printf("MissionServerCirculationFailNonexistant");
    }
    else if(!user)
    {
        LOG( LOG_MISSIONSERVER, LOG_LEVEL_VERBOSE, 0, "Arc %d requested for circulation status change, but user does not exist", arcid);
        meta_printf("MissionServerCirculationFailNonexistantUser");
    }
    else if(!s_isArcOwned(arc, auth_id, auth_region) && !accesslevel)
    {
        LOG( LOG_MISSIONSERVER, LOG_LEVEL_VERBOSE, 0, "Arc %d requested for circulation status change, but user does not have access ("USER_PRINTF_FMT" %d)",
            arcid, USER_PRINTF_EX(auth_id, auth_region), accesslevel);
        meta_printf("MissionServerCirculationFailAccess");
    }
    else if(arc->uncirculated && !circulate)
    {
        LOG( LOG_MISSIONSERVER, LOG_LEVEL_VERBOSE, 0, "Arc "ARC_PRINTF_FMT" by "USER_PRINTF_FMT" requested for removal from circulation, but it's already out of circulation",
            ARC_PRINTF(arc), USER_PRINTF_EX(auth_id, auth_region));
        meta_printf("MissionServerAlreadyUncirculated");
    }
    else if(!arc->uncirculated && circulate)
    {
        LOG( LOG_MISSIONSERVER, LOG_LEVEL_VERBOSE, 0, "Arc "ARC_PRINTF_FMT" by "USER_PRINTF_FMT" requested for circulation, but it's already circulated",
            ARC_PRINTF(arc), USER_PRINTF_EX(auth_id, auth_region));
        meta_printf("MissionServerAlreadyCirculated");
    }
    else if(publisedArcs >= publishedArcsAllowed)
    {
        meta_printf("MissionServerTooManyPublishedArcs%d", user->inventory.publish_slots + user->inventory.paid_publish_slots);
    }
    else if(!accesslevel && !circulate &&    //should never happen, but don't allow uncirculation if we've already maxed out.
        uncirculatedArcs >= serverArcsAllowed)
    {
        meta_printf("MissionServerTooManyUncirculatedArcs%d", user->inventory.publish_slots + user->inventory.paid_publish_slots);
    }
    else if(arc->permanent)
    {
        meta_printf("MissionServerNoPermanentUncirculation");
    }
    else
    {
        if(!circulate)
        {
            missionserver_UnindexArc(arc);
            s_addRemoveArcLevelRange(arc, 0);
            arc->uncirculated = timerSecondsSince2000();
            eaiSortedPushUnique(&user->arcids_uncirculated, arc->id);
            eaiSortedFindAndRemove(&user->arcids_published, arc->id);
        }
        else
        {
            missionserver_IndexNewArc(arc);
            s_addRemoveArcLevelRange(arc, 1);
            arc->uncirculated = 0;
            eaiSortedPushUnique(&user->arcids_published, arc->id);
            eaiSortedFindAndRemove(&user->arcids_uncirculated, arc->id);
        }

        meta_printf("MissionServerCirculationSuccess%s", arc->header.title);
        LOG( LOG_MISSIONSERVER, LOG_LEVEL_VERBOSE, 0, "Arc %d circulated by "USER_PRINTF_FMT, arcid, USER_PRINTF_EX(auth_id, auth_region));
    }
}

META(REMOTE ACCESSLEVEL("11")) // only valid from a mapserver
void missionserver_voteForArc(int accesslevel, int arcid, int auth_id, int ivote)
{
    MissionServerArc *arc = plFind(MissionServerArc, arcid);
    AuthRegion auth_region = s_getRemoteAuthRegion();

    // you can only vote for your own arcs if you have accesslevel (that's
    // allowed for testing)
    if(arc && INRANGE0(auth_region, AUTHREGION_COUNT) && (accesslevel || arc->creator_id != auth_id || arc->creator_region != auth_region))
    {
        MissionServerUser *voter = plFindOrAdd(MissionServerUser, auth_id, auth_region);
        MissionServerUser *creator = plFind(MissionServerUser, arc->creator_id, arc->creator_region);
        MissionRating vote = ivote;
        MissionRating oldvote = MISSIONRATING_0_STARS;
        MissionRating bestvote = MISSIONRATING_UNRATED;

        if(!creator)
        {
            LOG( LOG_MISSIONSERVER, LOG_LEVEL_VERBOSE, 0, "Could not find creator of arc %d (auth=%d,region=%d)", arcid, arc->creator_id, arc->creator_region);
        }

        if(INRANGE0(vote, MISSIONRATING_VOTETYPES))
        {
            int i;
            bool recentvoter = false;

            LOG( LOG_MISSIONSERVER, LOG_LEVEL_VERBOSE, 0, "Arc %d getting %d vote (%d,%d,%d,%d,%d,%d, total %d) from "USER_PRINTF_FMT, arc->id, vote, arc->votes[0], arc->votes[1], arc->votes[2], arc->votes[3], arc->votes[4], arc->votes[5], arc->total_votes, USER_PRINTF(voter));

            recentvoter = s_recordCreatorAndExpireVotes(voter, arc->creator_id, arc->creator_region);

            for(i = 0; i < MISSIONRATING_VOTETYPES; i++)
            {
                if(eaiSortedFindAndRemove(&voter->arcids_votes[i], arc->id) >= 0)
                {
                    // changing their vote
                    arc->votes[i]--;
                    arc->total_votes--;
                    oldvote = i;

                    if(creator)
                    {
                        creator->total_votes_earned[i]--;
                    }

                }

                if(eaiSortedFindAndRemove(&voter->arcids_best_votes[i], arc->id) >= 0)
                {
                    // new vote might be higher than the best so far
                    bestvote = i;
                }
            }

            eaiSortedPush(&voter->arcids_votes[vote], arc->id);
            arc->votes[vote]++;
            arc->total_votes++;

            if(creator)
            {
                creator->total_votes_earned[vote]++;
                missionserver_journalUserTotalVotesEarned(creator);
            }

            if(recentvoter)
            {
                if(creator && creator->shard_link && creator->shard_dbid)
                {
                    // Need to limit how often this gets sent otherwise
                    // creators can get spammed.
//                    s_messageEntity(creator->shard_link, creator->shard_dbid, "MissionServerRevoted%s", arc->header.title);
                }
            }
            else
            {
                s_giveTicketsForVote(arc, bestvote, vote);
            }

            if(bestvote > MISSIONRATING_MAX_STARS || vote > bestvote)
                bestvote = vote;

            LOG( LOG_MISSIONSERVER, LOG_LEVEL_VERBOSE, 0, USER_PRINTF_FMT" changed vote from %d to %d (best %d)", USER_PRINTF(voter), oldvote, vote, bestvote);

            eaiSortedPush(&voter->arcids_best_votes[bestvote], arc->id);

            s_updateArcRating(arc);

            missionserver_journalArcVotesRating(arc);
            missionserver_journalUserArcVote(voter, arc->id, vote);
            missionserver_journalUserArcBestVote(voter, arc->id, bestvote);
        }
        else
        {
            LOG( LOG_MISSIONSERVER, LOG_LEVEL_VERBOSE, 0, "Unknown vote %d made for arc %d", vote, arcid);
        }
        meta_printf("MissionServerVoted%s", arc->header.title);
    }
    else
    {
        if(arc && auth_id == arc->creator_id && auth_region == arc->creator_region)
        {
            meta_printf("MissionServerCantVoteOwnArc");
        }
        else
        {
            LOG( LOG_MISSIONSERVER, LOG_LEVEL_VERBOSE, 0, "Arc %d requested for voting, but it does not exist or the voter is invalid", arcid);
            meta_printf("MissionServerVotedFailNonexistant");
        }
    }
}

META(ACCESSLEVEL("3") REMOTE CONSOLE("arc_author_bio") CONSOLE_DESC("arc_author_bio <arcid> <author display name> <biographical info> <image name>"))
void missionserver_setGuestBio(int arcid, char *author, char *bio, char *image)
{
    MissionServerArc *arc = plFind(MissionServerArc, arcid);
    MissionSearchGuestBio guestbio = {0};
    char *tempBioString = 0;

    if(!arc)
    {
        LOG( LOG_MISSIONSERVER, LOG_LEVEL_VERBOSE, 0, "Arc id %d requested for set bio, but it does not exist", arcid);
        meta_printf("MissionServerCommandFailNonexistant");
    }
    else if(strlen(author) >=MISSIONSEARCH_AUTHORLEN)
    {
        meta_printf("MissionServerBioNameLength");
    }
    else
    {
        guestbio.imageName = image;
        guestbio.bio = bio;
        ParserWriteText(&tempBioString, parse_MissionSearchGuestBio, &guestbio, 0, 0);


        missionserver_UnindexArc(arc);
        s_addRemoveArcLevelRange(arc, 0);
        StructReallocString(&arc->header.guestAuthorBio, tempBioString);
        strcpy(arc->header.author, author);
        estrClear(&arc->header_estr);
        ParserWriteText(&arc->header_estr, parse_MissionSearchHeader, &arc->header, 0, 0);
        missionserver_IndexNewArc(arc);
        s_addRemoveArcLevelRange(arc, 1);
        missionserver_changeHonors(arcid, MISSIONHONORS_CELEBRITY);
        missionserver_journalArc(arc); // this rarely happens, just journal the whole thing

//        missionserver_journalUser(user); // this rarely happens, just journal the whole thing
        LOG( LOG_MISSIONSERVER, LOG_LEVEL_VERBOSE, 0, "%s guest bio for arc %d", bio ? "Set" : "Cleared", arcid);
        meta_printf("MissionServerCommandSuccess");
        estrDestroy(&tempBioString);
    }
}

static int s_getUserArcVote(MissionServerUser *user, U32 arcId)
{
    int i;
    if(!user)
        return MISSIONRATING_UNRATED;
    for(i = 0; i < MISSIONRATING_VOTETYPES; i++)
    {
        if(eaiSortedFind(&user->arcids_votes[i], arcId)>=0)
            return i;
    }
    return MISSIONRATING_UNRATED;
}

static int s_getPackedRating(MissionServerArc *arc)
{
    MissionSearchInfo rating = {0};

    if(!arc)
        return 0;

    if(arc->honors> MISSIONHONORS_FORMER_DC)
        rating.rating = MISSIONRATING_5_STARS;
    else if(arc->rating==MISSIONRATING_UNRATED)
        rating.rating = MISSIONRATING_0_STARS;
    else
        rating.rating = arc->rating;
    rating.honors = arc->honors;
    rating.ban = arc->ban;
    rating.permanent = !!arc->permanent;
    rating.unpublished = !!arc->unpublished;
    rating.invalidated = (arc->invalidated>= MISSIONSERVER_INVALIDATIONCOUNT);
    rating.playableErrors = (arc->playable_errors>= MISSIONSERVER_INVALIDATIONCOUNT);

    return rating.i;
}

static void s_pktSendArcInfo(Packet *pak_out, MissionServerArc *arc,int auth_id, AuthRegion auth_region, MissionServerUser *user, int accesslevel)
{
    bool owned = s_isArcOwned(arc, auth_id, auth_region);
    bool played = (user && arc)?eaiSortedFind(&user->arcids_played, arc->id)!=-1:0;
    int voted = s_getUserArcVote(user, arc->id);

    assert(arc);

    pktSendBool(pak_out, owned);
    pktSendBool(pak_out, played);
    pktSendBitsAuto(pak_out, voted);
    pktSendBitsAuto(pak_out, s_getPackedRating(arc));
    pktSendBitsAuto(pak_out, arc->total_votes);
    pktSendBitsAuto(pak_out, arc->top_keywords);
    pktSendBitsAuto(pak_out, arc->created);

    if(!arc->header_estr) // cache serialized header
        assert(ParserWriteText(&arc->header_estr, parse_MissionSearchHeader, &arc->header, 0, 0));
    pktSendString(pak_out, arc->header_estr);
    if(missionserverarc_isIndexed(arc)) // don't cache unsearchable headers :P
        estrDestroy(&arc->header_estr);

    // only send complaints to the owner and accesslevel users
    if(owned)
    {
        int i, count = 0; 

        for(i = 0; i < eaSize(&arc->complaints); i++) // count unsent comments
        {
            if(!arc->complaints[i]->bSent)
                count++;
        }
    
        pktSendBitsAuto(pak_out, count);
        if( count )
        {
            for(i = 0; i < eaSize(&arc->complaints); i++)
            {
                if( !arc->complaints[i]->bSent )
                {
                    pktSendBitsAuto(pak_out, arc->complaints[i]->complainer_id);
                    pktSendBitsAuto(pak_out, arc->complaints[i]->complainer_region);
                    pktSendBitsAuto(pak_out, arc->complaints[i]->type );
                    pktSendString(pak_out, arc->complaints[i]->globalName );
                    pktSendString(pak_out, arc->complaints[i]->comment);
                    arc->complaints[i]->bSent = 1;

                    // delete comments as they are sent to the owner, no need to preserve these.
                    if( arc->complaints[i]->type == kMissionComplaint_Comment )
                    {
                        MissionComplaint * pComplaint = eaRemove(&arc->complaints, i);
                        StructDestroy( parse_MissionComplaint, pComplaint);
                        i--;
                    }
                }
            }

            missionserver_journalArc(arc); // sent flag was updated
        }
    }
    else if(accesslevel)
    {
        int i;
        pktSendBitsAuto(pak_out, eaSize(&arc->complaints));
        for(i = 0; i < eaSize(&arc->complaints); i++)
        {
            pktSendBitsAuto(pak_out, arc->complaints[i]->complainer_id);
            pktSendBitsAuto(pak_out, arc->complaints[i]->complainer_region);
            pktSendBitsAuto(pak_out, arc->complaints[i]->type );
            pktSendString(pak_out, arc->complaints[i]->globalName );
            pktSendString(pak_out, arc->complaints[i]->comment);
        }
    }
    else
    {
        pktSendBitsAuto(pak_out, 0);
    }
}

static void s_sendUserInventory(MissionServerInventory *inventory, U32 banned_until, int entid, int authid, NetLink *link, int published, int permanent, int invalid, int playableErrors)
{
    Packet *pak_out = pktCreateEx(link, MISSION_SERVER_INVENTORY);
    MissionServerInventory toSend = {0};
    int count;
    pktSendBitsAuto(pak_out, entid);
    pktSendBitsAuto(pak_out, authid);
    pktSendBitsAuto(pak_out, banned_until);

    if(inventory)
        toSend = *inventory;

        
    pktSendBitsAuto(pak_out, toSend.tickets);
    pktSendBitsAuto(pak_out, toSend.lifetime_tickets);
    pktSendBitsAuto(pak_out, toSend.overflow_tickets);
    pktSendBitsAuto(pak_out, toSend.badge_flags);
    pktSendBitsAuto(pak_out, toSend.best_arc_stars);
    // The architect itself doesn't care what kind of slots, just how many you have in total.
    pktSendBitsAuto(pak_out, toSend.publish_slots + toSend.paid_publish_slots);
    pktSendBitsAuto(pak_out, published);
    pktSendBitsAuto(pak_out, permanent);
    pktSendBitsAuto(pak_out, invalid);
    pktSendBitsAuto(pak_out, playableErrors);
    pktSendBitsAuto(pak_out, toSend.uncirculated_slots);
    pktSendBitsAuto(pak_out, toSend.created_completions);
    pktSendBitsAuto(pak_out, eaSize(&toSend.costume_keys));

    for(count = 0; count < eaSize(&toSend.costume_keys); count++)
    {
        pktSendString(pak_out, toSend.costume_keys[count]);
    }

    pktSend(&pak_out, link);
}

static MissionServerUser* s_userFromString(const char *context)
{
    int auth_id;
    AuthRegion auth_region;

    if(!context || !context[0])
        return NULL;
    if(!(auth_id = atoi(context)))
        return NULL;
    if(!(context = strchr(context, ' ')))
        return NULL;

    auth_region = StaticDefineIntLookupInt(AuthRegionEnum, (char*)context+1);
    if(!INRANGE0(auth_region, AUTHREGION_COUNT))
        return NULL;

    return plFind(MissionServerUser, auth_id, auth_region);
}

int shardMsgCallback(Packet *pak_in, int cmd, NetLink *link)
{
    MissionServerState *state = &g_missionServerState;
    int cursor = bsGetCursorBitPosition(&pak_in->stream); // for debugging
    MissionServerShard *shard = link->userData;

    g_missionServerState.packet_count++;

    // This forces data to go back to the dbserver a little more smoothly
    // when there's a huge number of incoming packets.
    if(timerElapsed(g_missionServerState.packet_timer) > MISSIONSERVER_PACKETFLUSHTIME)
    {
        lnkFlushAll();
        timerStart(g_missionServerState.packet_timer);
    }

    if(cmd == MISSION_CLIENT_ERROR)
    {
        netSendDisconnect(link,0);
        shard->failed_connect = true;
        FatalErrorf("Error connecting to shard %s:\n%s", shard->name, pktGetString(pak_in));
        return 1;
    }

    if(!shard->name && cmd != MISSION_CLIENT_CONNECT)
    {
        printf("received cmd (%i) before shard connect\n",cmd);
        shard->failed_connect = true;
        return 1;
    }

    switch(cmd)
    {
        xcase MISSION_CLIENT_CONNECT:
        {
            int version = pktGetBitsAuto(pak_in);
            char *shardname = pktGetString(pak_in);
            if(version != DBSERVER_MISSION_PROTOCOL_VERSION)
            {
                printf("Info: dbserver at %s needs to be upgraded, rechecking version in %is. (got %i, need %i)\n",
                                shard->ip, SHARDRECONNECT_RETRY_TIME, version, DBSERVER_MISSION_PROTOCOL_VERSION);
                shard->failed_connect = true;
                netSendDisconnect(link,0);
                break;
            }

            if(shard->name)
            {
                if(strcmp(shard->name,shardname))
                {
                    printf("Error: dbserver at %s gave a name that doesn't match config (got %s, have %s).\n",
                                    shard->ip, shardname, shard->name);
                    shard->failed_connect = true;
                    netSendDisconnect(link,0);
                    break;
                }
            }
            else if(stashFindPointer(state->shardsByName,shardname,NULL))
            {
                printf("Error: dbserver at %s has the same name as a shard that is already connected (%s), dropping the new connection.\n",
                                shard->ip, shardname);
                shard->failed_connect = true;
                netSendDisconnect(link,0);
                break;
            }
            else
            {
                shard->name = allocAddString(shardname);
            }
            shard->data_ok = 1;
            stashAddPointer(state->shardsByName,shard->name,shard,true);
        }

        xcase MISSION_CLIENT_DATAOK:
            shard->data_ok = 1;

        xcase MISSION_CLIENT_COMMAND:
            meta_handleRemote(&shard->link, pak_in, 0, 11); // this could pass in the shard idx instead of 0

        xcase MISSION_CLIENT_PUBLISHARC:
        {
            int entid = pktGetBitsAuto(pak_in);
            int arcid = pktGetBitsAuto(pak_in);
            int auth_id = pktGetBitsAuto(pak_in);
            AuthRegion auth_region = shard->auth_region;
            char *authname = pktGetStringTemp(pak_in);
            int accesslevel = pktGetBitsAuto(pak_in);
            int keywords = pktGetBitsAuto(pak_in);
            MissionServerPublishFlags publishflags = pktGetBitsAuto(pak_in);
            char *headerstr = pktGetStringTemp(pak_in);
            int i;

            MissionServerUser *user = plFindOrAdd(MissionServerUser, auth_id, auth_region);
            MissionSearchHeader header = {0};

            LOG( LOG_MISSIONSERVER, LOG_LEVEL_VERBOSE, 0, "publish for ent %d, arc %d, auth %d, access level %d", entid, arcid, auth_id, accesslevel);

            if(!user->authname)
            {
                user->authname = StructAllocString(authname);
                missionserver_journalUser(user);
            }
            else if(!streq(user->authname, authname))
            {
                LOG( LOG_MISSIONSERVER, LOG_LEVEL_VERBOSE, 0,"Warning: User "USER_PRINTF_FMT" authname changed from %s to %s", USER_PRINTF(user), user->authname, authname);
                StructReallocString(&user->authname, authname);
                missionserver_journalUser(user);
            }

            if(user->banned_until && user->banned_until < timerSecondsSince2000())
            {
                user->banned_until = 0;
                user->autobans = 0;
                missionserver_journalUserBan(user);
            }

            if(!accesslevel && user->banned_until)
            {
                LOG( LOG_MISSIONSERVER, LOG_LEVEL_VERBOSE, 0, "Publish requested for arc %d by banned user "USER_PRINTF_FMT,
                                    arcid, USER_PRINTF(user));
                s_messageEntity(link, entid, "MissionServerYouAreBanned");
            }
            else if(arcid) // republish
            {
                MissionServerArc *arc = plFind(MissionServerArc, arcid);
                int owned = s_isArcOwned(arc, auth_id, auth_region);

                if(!arc)
                {
                    LOG( LOG_MISSIONSERVER, LOG_LEVEL_VERBOSE, 0,"Unknown arc %d requested for republish", arcid);
                }
                else if(!accesslevel && !owned)
                {
                    LOG( LOG_MISSIONSERVER, LOG_LEVEL_VERBOSE, 0, "Republish requested for arc %d by non-owner "USER_PRINTF_FMT" (owner "USER_PRINTF_FMT")",
                                        arcid, USER_PRINTF_EX(auth_id, auth_region), USER_PRINTF_EX(arc->creator_id, arc->creator_region) );
                }
                else if(!accesslevel && // owned &&
                        eaiSize(&user->arcids_published) > (int)(user->inventory.publish_slots + user->inventory.paid_publish_slots) )
                {
                    s_messageEntity(link, entid, "MissionServerTooManyPublishedArcs%d", user->inventory.publish_slots + user->inventory.paid_publish_slots);
                }
                else if(!accesslevel && // owned &&
                    (eaiSize(&user->arcids_uncirculated)+eaiSize(&user->arcids_published) > (int)(user->inventory.uncirculated_slots + user->inventory.publish_slots + user->inventory.paid_publish_slots) ))
                {
                    s_messageEntity(link, entid, "MissionServerTooManyUncirculatedArcs%d", user->inventory.uncirculated_slots + user->inventory.paid_publish_slots);
                }
                else if(!ParserReadText(headerstr, -1, parse_MissionSearchHeader, &header))
                {
                    s_messageEntity(link, entid, "MissionServerTryAgain");
                }
                else
                {
                    char *guestBio = 0;
                    char *guestName = 0;
                    
                    if(missionserverarc_isIndexed(arc)) // remove the old arc from search
                    {
                        missionserver_UnindexArc(arc);
                        s_addRemoveArcLevelRange(arc, 0);
                    }
                    arc->updated = timerSecondsSince2000();

                    if(owned)
                    {
                        strcpy(header.author_shard, shard->name);
                    }
                    else
                    {
                        // accesslevel republish, preserve author information
                        strcpy(header.author, arc->header.author);
                        strcpy(header.author_shard, arc->header.author_shard);
                    }
                    guestBio = arc->header.guestAuthorBio;
                    if(guestBio)
                    {
                        guestName = estrTemp();
                        estrPrintCharString(&guestName, arc->header.author);
                    }
                    StructClear(parse_MissionSearchHeader, &arc->header);
                    arc->header = header; // struct copy
                    estrDestroy(&arc->header_estr);
                    arc->header.guestAuthorBio = guestBio;
                    if(guestBio)
                    {
                        Strncpyt( arc->header.author, guestName);
                        estrDestroy(&guestName);
                    }

                    missionserver_UpdateArcData(arc, pktGetZippedInfo(pak_in, &arc->zsize, &arc->size)); // pktGetZippedInfo mallocs, s_updateArcData owns

                    switch(arc->ban)
                    {
                        xcase MISSIONBAN_NONE:
                         case MISSIONBAN_SELFREVIEWED:
                            // leave the arc in the same ban state

                        xcase MISSIONBAN_SELFREVIEWED_SUBMITTED:
                            // don't resubmit, but reset the auto-unpublish timer
                            arc->pulled = timerSecondsSince2000();

                        xcase MISSIONBAN_PULLED:
                         case MISSIONBAN_CSREVIEWED_APPROVED:
                            missionserver_UpdateArcBan(arc, MISSIONBAN_SELFREVIEWED);
                            arc->pulled = 0;
                            arc->new_complaints = 0;

                        xcase MISSIONBAN_SELFREVIEWED_PULLED:
                         case MISSIONBAN_CSREVIEWED_BANNED:
                            // send a petition to csr
                            missionserver_UpdateArcBan(arc, MISSIONBAN_SELFREVIEWED_SUBMITTED);
                            s_sendArcPetition(link, arc);

                        xdefault:
                            assertmsg(0, "unhandled arc ban type");
                    }

                    arc->invalidated = 0; // validated by mapserver, okay to reindex
                    arc->playable_errors = 0; // validated by mapserver, okay to reindex
                    if(arc->unpublished) // restore
                    {
                        s_restoreArc(arc); // calls missionserver_IndexNewArc()
                        LOG( LOG_MISSIONSERVER, LOG_LEVEL_VERBOSE, 0, "Arc "ARC_PRINTF_FMT" restored via publish by"USER_PRINTF_FMT,
                                    ARC_PRINTF(arc), USER_PRINTF_EX(auth_id, auth_region));
                    }
                    else if(!arc->uncirculated)//otherwise, we still have to update search.
                    {
                        assert(missionserverarc_isIndexed(arc));
                        missionserver_IndexNewArc(arc);
                        s_addRemoveArcLevelRange(arc, 1);
                    }
                    if(arc->honors == MISSIONHONORS_DEVCHOICE)
                    {
                        missionserver_changeHonors(arcid, MISSIONHONORS_FORMER_DC);
                    }
                    else if(arc->honors == MISSIONHONORS_DC_AND_HOF)
                    {
                        missionserver_changeHonors(arcid, MISSIONHONORS_FORMER_DC_HOF);
                    }

                    if(publishflags & kPublishCustomBoss)
                    {
                        s_awardUserBadge(user, kBadgeArcCustomBossPublish);
                    }

                    if( arc->initial_keywords != keywords )
                    {
                        int top[3]={-1,-1,-1}, newTop = 0;

                        // update keyword choices
                        for( i = 0; i < MISSIONSERVER_MAX_KEYWORDS; i++  )
                        {
                            if( arc->initial_keywords & (1<<i) )
                                arc->keyword_votes[i] -= 4;
                            if( keywords & (1<<i) )
                                arc->keyword_votes[i] += 4;
                        }

                        // see if top 3 list changed
                        for( i = 0; i < MISSIONSERVER_MAX_KEYWORDS; i++  )
                        {
                            // used to update top 3 list
                            if( arc->keyword_votes[i] > arc->keyword_votes[top[0]] )
                            {
                                top[2] = top[1];
                                top[1] = top[0];
                                top[0] = i;
                            }
                            else if( arc->keyword_votes[i] > arc->keyword_votes[top[1]] )
                            {
                                top[2] = top[1];
                                top[1] = i;
                            }
                            else if( arc->keyword_votes[i] > arc->keyword_votes[top[2]] )
                            {
                                top[2] = i;
                            }
                        }

                        for( i = 0; i < 3; i++ )
                        {
                            if( top[i] >= 0 )
                                newTop |= (1 << top[i] );
                        }

                        if(newTop != arc->top_keywords)
                            missionserver_UpdateArcKeywords(arc, newTop); // call to change arc->ban
                        arc->initial_keywords = keywords;    //if we don't reset initial keywords, you can artifically inflate your keyword ranking by republish
                    }

                    missionserver_journalArc(arc); // big changes, journal everything

                    LOG( LOG_MISSIONSERVER, LOG_LEVEL_VERBOSE, 0, "Arc %d republished by "USER_PRINTF_FMT"%s (ban %s)",
                                    arcid, USER_PRINTF_EX(auth_id, auth_region),
                                    owned ? "" : " (non-owner)", missionban_toString(arc->ban));
                    if(arc->ban == MISSIONBAN_SELFREVIEWED_SUBMITTED)
                        s_messageEntity(link, entid, "MissionServerAppealSuccess%s", arc->header.title);
                    else
                        s_messageEntity(link, entid, "MissionServerRepublishSuccess%s", arc->header.title);
                }
            }
            else // normal publish
            {
                if(    !accesslevel &&
                    eaiSize(&user->arcids_published) >= (int)(user->inventory.publish_slots + user->inventory.paid_publish_slots) )
                {
                    s_messageEntity(link, entid, "MissionServerTooManyPublishedArcs%d", user->inventory.publish_slots + user->inventory.paid_publish_slots);
                }
                else if(!ParserReadText(headerstr, -1, parse_MissionSearchHeader, &header))
                {
                    s_messageEntity(link, entid, "MissionServerTryAgain");
                }
                else
                {
                    MissionServerArc *arc = plNew(MissionServerArc);
                    arc->creator_id = auth_id;
                    arc->creator_region = auth_region;
                    arc->created = arc->updated = timerSecondsSince2000();
                    arc->header = header; // struct copy
                    arc->top_keywords = keywords;
                    arc->initial_keywords = keywords;

                    for( i = 0; i < MISSIONSERVER_MAX_KEYWORDS; i++  )
                    {
                        if( keywords & (1<<i) )
                            arc->keyword_votes[i] += 4;
                    }

                    Strncpyt(arc->header.author_shard, shard->name);
                    missionserver_UpdateArcData(arc, pktGetZippedInfo(pak_in, &arc->zsize, &arc->size)); // pktGetZippedInfo mallocs, s_updateArcData owns

                    eaiPush(&user->arcids_published, arc->id);
                    missionserver_IndexNewArc(arc);
                    s_addRemoveArcLevelRange(arc, 1);

                    if(publishflags & kPublishCustomBoss)
                    {
                        s_awardUserBadge(user, kBadgeArcCustomBossPublish);
                    }

                    user->inventory.dirty = true;    //we have to resend the inventory since the # publishes has changed.
                    if(!user->shard_dbid || !user->shard_link)
                    {
                        user->shard_dbid = entid;
                        user->shard_link = link;
                    }
                    missionserver_journalArc(arc); // big changes, journal everything

                    LOG( LOG_MISSIONSERVER, LOG_LEVEL_VERBOSE, 0, "Arc %d published by "USER_PRINTF_FMT, arc->id, USER_PRINTF(user));
                    s_messageEntity(link, entid, "MissionServerPublishSuccess%s", arc->header.title);
                }
            }
        }

        xcase MISSION_CLIENT_SEARCHPAGE:
        {
            typedef struct MissionSearchSection
            {
                const char *header;    // translated on the client
                int *arcids;        // don't destroy these
            } MissionSearchSection;

            static MissionSearchSection *s_searchsections = NULL; // not thread safe
            static int **s_arcids_buffer = NULL;
            static int *s_arcids_temp = NULL;
            static int *s_arcids_byArc = NULL;

            const char *error = NULL;
            int page_size = 20; // default for user searches, 0 to send everything
            int count, pages, start, i, sect;
            int firstArc = 0;    //specifies the first arc of the first page, regardless of sort.

            int entid = pktGetBitsAuto(pak_in);
            int auth_id = pktGetBitsAuto(pak_in);
            AuthRegion auth_region = shard->auth_region;
            int accesslevel = pktGetBitsAuto(pak_in);
            int category = pktGetBitsAuto(pak_in);
            char *context = pktGetString(pak_in);
            int page = pktGetBitsAuto(pak_in);
            int playedFilter = pktGetBitsAuto(pak_in);
            int levelFilter = pktGetBitsAuto(pak_in);
            int arc_author_id = pktGetBitsAuto(pak_in);
            int randomArc = !!(playedFilter&(1<<kMissionSearchViewedFilter_Random));


            int disconnectingFromError = 0;

            MissionServerUser *user = plFind(MissionServerUser, auth_id, auth_region);

            // clear previous results
            as_popall(&s_searchsections);
            eaiPopAll(&s_arcids_byArc);
            eaSetSize(&s_arcids_buffer, 0);    //don't put anything in here that isn't freed elsewhere.
        
             switch(category)
            {
                xcase MISSIONSEARCH_ALL:
                    //apply search parameters
                    if(strlen(context) && !missionserver_GetSearchResults(&s_arcids_buffer, context, &firstArc))
                    {
                        error = "Invalid search parameters";
                    }
                    else
                    {
                        int arraySize = eaSize(&s_arcids_buffer);
                        //remove the voted or played
                        if(page >=0 && firstArc)    //arc by ID goes in the front.
                        {
                            eaiPush(&s_arcids_byArc, firstArc);
                            as_push(&s_searchsections)->arcids = s_arcids_byArc;
                        }
                        for(i = 0; i < arraySize; i++)
                        {
                            int *eaiArray = s_arcids_buffer[i];
                            int j;
                            if(user && playedFilter&(1<<kMissionSearchViewedFilter_Voted_NoShow))
                            {
                                for(j=MISSIONRATING_1_STAR; j<MISSIONRATING_VOTETYPES; j++)
                                {
                                    eaiSortedDifference(&eaiArray, &eaiArray, &user->arcids_votes[j]);
                                }
                            }
                            else if(playedFilter&(1<<kMissionSearchViewedFilter_Voted_Show))
                            {
                                if(user)
                                {
                                    static U32 *tempArray = 0;
                                    eaiPopAll(&tempArray);
                                    eaiPopAll(&s_arcids_temp);
                                    for(j=MISSIONRATING_1_STAR; j<MISSIONRATING_VOTETYPES; j++)
                                    {
                                        eaiSortedUnion(&tempArray, &s_arcids_temp, &user->arcids_votes[j]);
                                    }
                                    eaiSortedIntersection(&eaiArray, &eaiArray, &tempArray);
                                }
                                else
                                    eaiPopAll(&eaiArray);
                            }
                            if(user && playedFilter&(1<<kMissionSearchViewedFilter_Played_NoShow))
                            {
                                eaiSortedDifference(&eaiArray, &eaiArray, &user->arcids_played);
                            }
                            else if(playedFilter&(1<<kMissionSearchViewedFilter_Played_Show))
                            {
                                if(user)
                                    eaiSortedIntersection(&eaiArray, &eaiArray, &user->arcids_played);
                                else
                                    eaiPopAll(&eaiArray);
                            }
                            if(levelFilter)
                            {
                                U32 **arcsInRange = s_getArcsWithLevelRange(levelFilter);
                                eaiSortedIntersection(&eaiArray, &eaiArray, arcsInRange);
                            }
                            as_push(&s_searchsections)->arcids = eaiArray;
                        }

                        if(page <0 && firstArc)    //arc by ID goes in the back.
                        {
                            eaiPush(&s_arcids_byArc, firstArc);
                            as_push(&s_searchsections)->arcids = s_arcids_byArc;
                        }
                    }

                xcase MISSIONSEARCH_MYARCS:
                    page_size = 0; // send everything
                    if(user)
                    {
                        MissionSearchSection *section;
                        section =as_push(&s_searchsections);
                        section->arcids = user->arcids_permanent;
                        //section->header = "MissionSearchPermanent";

                        section =as_push(&s_searchsections);
                        section->arcids = user->arcids_published;
                        //section->header = "MissionSearchPublished";

                        section = as_push(&s_searchsections);
                        section->arcids = user->arcids_uncirculated;
                        section->header = "MissionSearchUncirculated";

                    }

                xcase MISSIONSEARCH_ARC_AUTHOR:
                    {
                        MissionServerArc *arc = plFind(MissionServerArc, arc_author_id);
                        
                        if( arc )
                        {
                            MissionServerUser *author = plFind(MissionServerUser, arc->creator_id, arc->creator_region);
                            MissionSearchSection *section;
                            section = as_push(&s_searchsections);
                            section->arcids = author->arcids_published;
                        }
                        page_size = 0; // send everything
                    }


                xcase MISSIONSEARCH_CSR_AUTHORED:
                {
                    MissionServerUser *target = s_userFromString(context);
                    MissionSearchSection *section;

                    section = as_push(&s_searchsections);
                    section->header = "MissionSearchPermanent";
                    section->arcids = target ? target->arcids_permanent : NULL;

                    section = as_push(&s_searchsections);
                    section->header = "MissionSearchPublished";
                    section->arcids = target ? target->arcids_published : NULL;

                    section = as_push(&s_searchsections);
                    section->header = "MissionSearchUnpublished";
                    section->arcids = target ? target->arcids_unpublished : NULL;

                    page_size = 0; // send everything
                }

                xcase MISSIONSEARCH_CSR_COMPLAINED:
                {
                    MissionServerUser *target = s_userFromString(context);

                    MissionSearchSection *section = as_push(&s_searchsections);
                    section->header = "MissionSearchComplained";
                    section->arcids = target ? target->arcids_complaints : NULL;

                    page_size = 100;
                }

                xcase MISSIONSEARCH_CSR_VOTED:
                {
                    MissionServerUser *target = s_userFromString(context);

                    for(i = 0; i < MISSIONRATING_VOTETYPES; i++)
                    {
                        MissionSearchSection *section = as_push(&s_searchsections);
                        section->header = missionrating_toString(i); // bit of a hack
                        section->arcids = target ? target->arcids_votes[i] : NULL;
                    }

                    page_size = 100;
                }


                xcase MISSIONSEARCH_CSR_PLAYED:
                {
                    MissionServerUser *target = s_userFromString(context);

                    MissionSearchSection *section = as_push(&s_searchsections);
                    section->header = "MissionSearchPlayed";
                    section->arcids = target ? target->arcids_played : NULL;

                    page_size = 100;
                }

                xdefault:
                {
                    error = "Invalid search category.  Disconnecting from shard.";
                    netSendDisconnect(link,0);
                    disconnectingFromError = 1;
                }
            }

            if(error)
            {
                LOG( LOG_MISSIONSERVER, LOG_LEVEL_VERBOSE, 0, "%s: page %d (context %s) requested by %s.%d", error, category, context, shard->name, entid);

                count = 0;
                pages = 1;
                page = 0;
                start = 0;
            }
            else
            {
                count = 0;
                for(sect = 0; sect < as_size(&s_searchsections); sect++)
                    count += eaiSize(&s_searchsections[sect].arcids);
                    

                if(!page_size)
                {
                    // one big page
                    pages = 1;
                    page = 0;
                    start = 0;
                }
                else if (randomArc && count)
                {
                    pages = 1;
                    page = 0;
                    start = rand()%count;
                    count = 1;
                }
                else
                {
                    // pagination
                    pages = count ? (count + page_size-1) / page_size : 1;
                    if(page < -pages)
                        page = -pages;
                    if( page >= pages)
                        page = pages - 1;
                    if(page >= 0)
                    {
                        start = page * page_size;
                        count = MINMAX(count - start, 0, page_size);
                    }
                    else    //reverse index.  We still read from low idx to high, so
                            //we use the fact that page 0 of the reverse index = -1 to
                            //choose the left (low idx) side of the selection
                    {
                        int lastPageOffset = 0;;
                        start = (count)+(page * page_size);
                        if(start < 0)
                            lastPageOffset = start;
                        start = MAX(0, start);
                        count = MINMAX(count-start, 0, page_size+lastPageOffset);
                    }
                }
            }

            if(!disconnectingFromError)
            {
                Packet *pak_out = pktCreateEx(link, MISSION_SERVER_SEARCHPAGE);
                pktSendBitsAuto(pak_out, entid);
                pktSendBitsAuto(pak_out, category);
                pktSendString(pak_out, context);
                pktSendBitsAuto(pak_out, page);
                pktSendBitsAuto(pak_out, pages);
                for(sect = 0, i = 0; sect < as_size(&s_searchsections) && i < count; sect++)
                {
                    MissionSearchSection *section = &s_searchsections[sect];
                    if(section->header && start + i < eaiSize(&section->arcids))
                    {
                        pktSendBitsAuto(pak_out, -1); // arcid (section header)
                        pktSendString(pak_out, section->header);
                    }
                    for(; i < count && start + i < eaiSize(&section->arcids); i++)
                    {
                        MissionServerArc *arc = plFind(MissionServerArc, section->arcids[start+i]);
                        if(arc)
                        {
                            pktSendBitsAuto(pak_out, arc->id);
                            s_pktSendArcInfo(pak_out, arc, auth_id, auth_region, user, accesslevel);
                        }
                    }
                    start -= eaiSize(&section->arcids);
                }
                devassert(i == count);
                pktSendBitsAuto(pak_out, 0); // arcid (finished)
                pktSend(&pak_out, link);
            }
        }

        xcase MISSION_CLIENT_REQUESTALL:
        {
            MissionServerArc **allArcs = plGetAll(MissionServerArc);
            int nArcs = eaSize(&allArcs);
            int i;
            int mapId = pktGetBitsAuto(pak_in);
            int sendUnpublished = pktGetBool(pak_in);
            int sendInvalid = pktGetBool(pak_in);
            int *sendIds =0;
            int nUnpublished = 0;
            int nInvalid = 0;
            int nFound = 0;
            int nSend;
            Packet *pak_out;

            for(i = 0; i < nArcs; i++)
            {
                MissionServerArc *arc = allArcs[i];
                if(arc)
                {
                    nFound++;
                    if(arc->invalidated>=MISSIONSERVER_INVALIDATIONCOUNT)
                    {
                        if(sendInvalid)
                            eaiSortedPushUnique(&sendIds, arc->id);
                        nInvalid++;
                    }

                    if(arc->unpublished)
                    {
                        if(sendUnpublished)
                            eaiSortedPushUnique(&sendIds, arc->id);
                        
                        nUnpublished++;
                    }

                    if(!arc->data)
                        missionserver_RestoreArcData(arc);

                    else if(arc->invalidated<MISSIONSERVER_INVALIDATIONCOUNT && !arc->unpublished)
                    {
                        eaiSortedPushUnique(&sendIds, arc->id);
                    }
                }
            }
            pak_out = pktCreateEx(link, MISSION_SERVER_ALLARCS);
            pktSendBitsAuto(pak_out, mapId);
            pktSendBitsAuto(pak_out, nFound);    //total arcs
            pktSendBitsAuto(pak_out, nInvalid);
            pktSendBitsAuto(pak_out, nUnpublished);
            nSend = eaiSize(&sendIds);
            for(i = 0; i < nSend; i++)
            {
                pktSendBitsAuto(pak_out, sendIds[i]);
            }
            pktSendBitsAuto(pak_out, 0);    //no more arcs.
            pktSend(&pak_out, link);
        }

        xcase MISSION_CLIENT_ARCINFO:
        {
            int arcid;
            int entid = pktGetBitsAuto(pak_in);
            int auth_id = pktGetBitsAuto(pak_in);
            AuthRegion auth_region = shard->auth_region;
            int accesslevel = pktGetBitsAuto(pak_in);

            Packet *pak_out = pktCreateEx(link, MISSION_SERVER_ARCINFO);
            pktSendBitsAuto(pak_out, entid);
            while(arcid = pktSendGetBitsAuto(pak_out, pak_in))
            {
                MissionServerArc *arc = plFind(MissionServerArc, arcid);
                if(pktSendBool(pak_out, arc))    //NULL below because we didn't load the auth.
                    s_pktSendArcInfo(pak_out, arc, auth_id, auth_region, NULL, accesslevel);
            }
            pktSend(&pak_out, link);
        }

        xcase MISSION_CLIENT_BANSTATUS:
        {
            int mapid = pktGetBitsAuto(pak_in);
            int arcid = pktGetBitsAuto(pak_in);
            Packet *pak_out = pktCreateEx(link, MISSION_SERVER_BANSTATUS);
            MissionServerArc *arc = plFind(MissionServerArc, arcid);
            pktSendBitsAuto(pak_out, mapid);    //mapid
            pktSendBitsAuto(pak_out, arcid);
            if(!arc)
            {
                pktSendBitsAuto(pak_out, 0);    //ban
                pktSendBitsAuto(pak_out, 0);    //honors
            }
            else
            {
                pktSendBitsAuto(pak_out, arc->ban);
                pktSendBitsAuto(pak_out, arc->honors);
            }
            pktSend(&pak_out, link);
        }

        xcase MISSION_CLIENT_ARCDATA:
        {
            int entid = pktGetBitsAuto(pak_in);
            int arcid = pktGetBitsAuto(pak_in);
            int auth_id = pktGetBitsAuto(pak_in);
            int map_id = pktGetBitsAuto(pak_in);
            AuthRegion auth_region = shard->auth_region;
            int accesslevel = pktGetBitsAuto(pak_in);
            int test = pktGetBitsAuto(pak_in);
            int devchoice = pktGetBitsAuto(pak_in);
            MissionServerArc *arc = plFind(MissionServerArc, arcid);
            Packet *pak_out;
            int show;

            if(arc &&test == MISSIONSEARCH_ARCDATA_MAPTEST)
                show = 1;
            else if(!arc || arc->unpublished) // these will crash, unpublished arcs must be restored first
                show = 0;
            else if(accesslevel)
                show = 1;
            else if(test) // 1: testing, 2: viewing
                show = s_isArcOwned(arc, auth_id, auth_region);
            else // 0: playing
                show = !arc->invalidated && missionban_isVisible(arc->ban);

            pak_out = pktCreateEx(link, MISSION_SERVER_ARCDATA);
            pktSendBitsAuto(pak_out, entid);
            pktSendBitsAuto(pak_out, map_id);
            pktSendBitsAuto(pak_out, arcid);
            pktSendBitsAuto(pak_out, test);
            pktSendBitsAuto(pak_out, devchoice);
            if(pktSendBool(pak_out, show))
            {
                // currently we are preloading all arc data (MISSIONSERVER_LOADALL_HOGGS)
                // if we switch back to lazy loading, note that missionserver_getArcData() could be slow
                pktSendBitsAuto(pak_out, auth_region == arc->creator_region ? arc->creator_id : 0);
                pktSendBitsAuto(pak_out, s_getPackedRating(arc));
                pktSendZippedAlready(pak_out, arc->size, arc->zsize, missionserver_GetArcData(arc));
                if(test == 1)
                {
                    LOG( LOG_MISSIONSERVER, LOG_LEVEL_VERBOSE, 0, "Testing arc "ARC_PRINTF_FMT, ARC_PRINTF(arc))
                }
                else if(!test)
                {
                    LOG( LOG_MISSIONSERVER, LOG_LEVEL_VERBOSE, 0, "Playing arc "ARC_PRINTF_FMT, ARC_PRINTF(arc))
                }
            }
            else
            {
                if(arc)
                {
                    LOG( LOG_MISSIONSERVER, LOG_LEVEL_VERBOSE, 0, "Arc %d requested, but it is not visible to requester", arcid)
                }
                else
                {
                    LOG( LOG_MISSIONSERVER, LOG_LEVEL_VERBOSE, 0, "Arc %d requested, but it does not exist", arcid)
                }
            }
            pktSend(&pak_out, link);
        }

        xcase MISSION_CLIENT_ARCDATA_OTHERUSER:
        {
            int entid = pktGetBitsAuto(pak_in);
            int arcid = pktGetBitsAuto(pak_in);
            int map_id = pktGetBitsAuto(pak_in);
            MissionServerArc *arc = plFind(MissionServerArc, arcid);
            Packet *pak_out;
            void* arcdata;

            pak_out = pktCreateEx(link, MISSION_SERVER_ARCDATA_OTHERUSER);
            pktSendBitsAuto(pak_out, entid);
            pktSendBitsAuto(pak_out, map_id);
            pktSendBitsAuto(pak_out, arcid);

            if(arc)
            {
                arcdata = missionserver_GetArcData(arc);

                if(arcdata)
                {
                    pktSendBitsPack(pak_out, 1, 1);
                    pktSendZippedAlready(pak_out, arc->size, arc->zsize, arcdata);
                }
                else
                {
                    pktSendBitsPack(pak_out, 1, 0);
                }
            }
            else
            {
                pktSendBitsPack(pak_out, 1, 0);
            }
            pktSend(&pak_out, link);
        }

        xcase MISSION_CLIENT_INVENTORY:
        {
            int entid = pktGetBitsAuto(pak_in);
            int authid = pktGetBitsAuto(pak_in);

            MissionServerUser *user = plFind(MissionServerUser, authid, shard->auth_region);

            if(user)
            {
                s_sendUserInventory(&user->inventory, user->banned_until, entid, authid, link, eaiSize(&user->arcids_published), eaiSize(&user->arcids_permanent),
                    eaiSize(&user->arcids_invalid), eaiSize(&user->arcids_playableErrors));

                user->shard_dbid = entid;
                user->shard_link = link;
            }
            else
            {
                s_sendUserInventory(NULL, 0, entid, authid, link, 0, 0, 0, 0);
            }
        }

        xcase MISSION_CLIENT_CLAIM_TICKETS:
        {
            int entid = pktGetBitsAuto(pak_in);
            int authid = pktGetBitsAuto(pak_in);
            U32 tickets = pktGetBitsAuto(pak_in);

            MissionServerUser *user = plFind(MissionServerUser, authid, shard->auth_region);

            Packet *pak_out = pktCreateEx(link, MISSION_SERVER_CLAIM_TICKETS);

            pktSendBitsAuto(pak_out, entid);
            pktSendBitsAuto(pak_out, authid);

            if(user)
            {
                user->shard_dbid = entid;
                user->shard_link = link;

                if(!tickets)
                {
                    pktSendBitsAuto(pak_out, 0);
                    pktSendBitsAuto(pak_out, user->inventory.tickets);
                    pktSendString(pak_out, "MissionServerZeroTickets");
                }
                else if(tickets > user->inventory.tickets)
                {
                    pktSendBitsAuto(pak_out, 0);
                    pktSendBitsAuto(pak_out, user->inventory.tickets);
                    pktSendString(pak_out, "MissionServerNotEnoughTickets");
                }
                else
                {
                    user->inventory.tickets -= tickets;
                    missionserver_journalUserInventory(user);

                    pktSendBitsAuto(pak_out, tickets);
                    pktSendBitsAuto(pak_out, user->inventory.tickets);
                    pktSendString(pak_out, "MissionServerTicketsClaimed");
                }
            }
            else
            {
                pktSendBitsAuto(pak_out, 0);        // no tickets claimed
                pktSendBitsAuto(pak_out, 0);        // no tickets in inventory
                pktSendString(pak_out, "MissionServerNoSuchUser");
            }

            pktSend(&pak_out, link);
        }

        xcase MISSION_CLIENT_NOT_PRESENT:
        {
            int authid = pktGetBitsAuto(pak_in);
            int count;

            for(count = 0; count < g_missionServerState.shard_count; count++)
            {
                if(&(g_missionServerState.shards[count]->link) == link)
                {
                    MissionServerUser *user = plFind(MissionServerUser, authid, g_missionServerState.shards[count]->auth_region);

                    if(user)
                    {
                        user->shard_dbid = 0;
                        user->shard_link = NULL;
                    }

                    break;
                }
            }
        }

        xcase MISSION_CLIENT_BUY_ITEM:
        {
            Packet *pak_out;
            int entid = pktGetBitsAuto(pak_in);
            int authid = pktGetBitsAuto(pak_in);
            int cost = pktGetBitsAuto(pak_in);
            int flags = pktGetBitsAuto(pak_in);
            MissionServerUser *user = plFindOrAdd(MissionServerUser, authid, shard->auth_region);

            if(flags & kPurchaseUnlockKey)
            {
                char *key = pktGetString(pak_in);
                if( s_grantUnlockKeyToUser(user, key) )
                    s_messageEntity(link, entid, "MissionServerUnlockKeyBought%s", key);
                else
                {
                    s_messageEntity(link, entid, "MissionServerUnlockKeyBuyFailed%s", key);
                    flags = kPurchaseFail;
                }
            }

            if(flags & kPurchasePublishSlots)
            {
                int slots = pktGetBitsAuto(pak_in);
                if( s_grantPublishSlotsToUser(user, slots) )
                    s_messageEntity(link, entid,"MissionServerPublishSlotsBought%d", slots);
                else
                {
                    s_messageEntity(link, entid,"MissionServerPublishSlotBuyFailed%d", slots);
                    flags = kPurchaseFail;
                }
            }

            // send an inventory refresh, if they got the new item or not.  If they got here then they are out of synch
            s_sendUserInventory(&user->inventory, 0, entid, authid, link, eaiSize(&user->arcids_published), eaiSize(&user->arcids_permanent),
                eaiSize(&user->arcids_invalid), eaiSize(&user->arcids_playableErrors));
            user->inventory.dirty = 0;

            // The traffic stays in order so we don't need to reply with
            // the actual items. In theory we don't even need the cost
            // and flags but they're useful for debugging.
            pak_out = pktCreateEx(link, MISSION_SERVER_ITEM_BOUGHT);
            pktSendBitsAuto(pak_out, entid);
            pktSendBitsAuto(pak_out, authid);
            pktSendBitsAuto(pak_out, cost);
            pktSendBitsAuto(pak_out, flags);
            pktSend(&pak_out, link);
        }

        xcase MISSION_CLIENT_SET_LOG_LEVELS:
        {
            int i;
            for( i = 0; i < LOG_COUNT; i++ )
            {
                int log_level = pktGetBitsPack(pak_in,1);
                logSetLevel(i,log_level);
            }
        }
        xcase MISSION_CLIENT_TEST_LOG_LEVELS:
        {
            LOG( LOG_DEPRECATED, LOG_LEVEL_ALERT, LOG_CONSOLE, "MissionServer Log Line Test: Alert" );
            LOG( LOG_DEPRECATED, LOG_LEVEL_IMPORTANT, LOG_CONSOLE, "MissionServer Log Line Test: Important" );
            LOG( LOG_DEPRECATED, LOG_LEVEL_VERBOSE, LOG_CONSOLE, "MissionServer Log Line Test: Verbose" );
            LOG( LOG_DEPRECATED, LOG_LEVEL_DEPRECATED, LOG_CONSOLE, "MissionServer Log Line Test: Deprecated" );
            LOG( LOG_DEPRECATED, LOG_LEVEL_DEBUG, LOG_CONSOLE, "MissionServer Log Line Test: Debug" );
        }

        xdefault:
        {
            LOG( LOG_MISSIONSERVER, LOG_LEVEL_VERBOSE, 0, "invalid shard msg %i from %s.  Disconnecting.", cmd, shard->name);
            netSendDisconnect(link,0);
        }
    };
    return 1;
}

////////////////////////////////////////////////////////////////////////////////

void missionserver_init(void);
void missionserver_load(void);
void missionserver_tick(void);
void missionserver_stop(void);
void missionserver_http(char**);
ConsoleDebugMenu s_debugmenu[];

ServerLibConfig g_serverlibconfig =
{
    "MissionServer",    // name
    'M',                // icon
    0xffcc00,            // color
    missionserver_init,
    missionserver_load,
    missionserver_tick,
    missionserver_stop,
    missionserver_http,
    s_debugmenu,
    kServerLibDefault,
};

static void shardNetConnectTick(MissionServerState *state)
{
    MissionServerCfg *cfg = &state->cfg;
    U32 retry_time = SHARDRECONNECT_RETRY_TIME;
    bool retry_failed_ok = state->last_failed_time + retry_time < timerSecondsSince2000();
    int next_retry_shard = state->next_retry_shard;
    U32 now = timerSecondsSince2000();
    int i;

    // first alloc shards/links if need be
    if(!state->shards)
    {
        state->shards = cfg->shards;
        state->shard_count = eaSize(&state->shards);
        assert(state->shard_count); // this should have already failed during config load
        state->shardsByName = stashTableCreateWithStringKeys(32,0);
        for(i = 0; i < state->shard_count; ++i)
            initNetLink(&state->shards[i]->link);
    }

    // connect to any disconnected shards
    for(i = 0; i < state->shard_count; ++i)
    {
        int shard_idx = (next_retry_shard + i) % state->shard_count;
        MissionServerShard *shard = state->shards[shard_idx];
        NetLink *link = &shard->link;

        if(disconnectAllShards)
        {
            netSendDisconnect(link,0);
        }
        // if the link is down, try to reconnect if the last attempt on that shard was good or
        // the last failed attempt was a while ago
        else if( !link->connected && shard->ip && (!shard->failed_connect || retry_failed_ok) )
        {
            char *ipstr;
            loadstart_printf("connecting to %s...",shard->ip);
            ipstr = makeIpStr(ipFromString(shard->ip));
            if(!netConnect(link,ipstr, DEFAULT_MISSIONSERVER_PORT,NLT_TCP,1.f,NULL))
            {
                link->last_update_time = timerCpuSeconds();

                shard->failed_connect = true;
                state->next_retry_shard = shard_idx + 1; // this will wrap
                state->last_failed_time = timerSecondsSince2000(); // don't try a known failure again for while
                retry_failed_ok = false;

                loadend_printf("failed, retry in %is",retry_time);
            }
            else
            {
                Packet *pak = pktCreateEx(link,MISSION_SERVER_CONNECT); 
                pktSend(&pak,link);

                link->compressionDefault = 1;
                link->userData = shard;
                shard->failed_connect = false;

                loadend_printf("done");
            }
        }
    }
    disconnectAllShards = 0;
}

static void shardNetProcessTick(MissionServerState *state)
{
    int i;
    for(i = 0; i < state->shard_count; ++i)
    {
        MissionServerShard *shard = state->shards[i];
        NetLink *link = &shard->link;
        if(link->connected)
        {
            netLinkMonitor(link, 0, shardMsgCallback);

            if(    !shard->failed_connect && shard->data_ok &&
                as_size(&g_missionServerState.petitions[shard->auth_region]) )
            {
                int j;
                for(j = 0; j < as_size(&g_missionServerState.petitions[shard->auth_region]); j++)
                {
                    MissionServerPetition *petition = &g_missionServerState.petitions[shard->auth_region][j];
                    // send authregion DEV to prevent requeueing
                    s_sendPetitionString(link, petition->authname, AUTHREGION_DEV, petition->character, petition->summary, petition->message, petition->notes);
                    Str_destroy(&petition->authname);
                    Str_destroy(&petition->character);
                    Str_destroy(&petition->summary);
                    Str_destroy(&petition->message);
                    Str_destroy(&petition->notes);
                }
                as_popall(&g_missionServerState.petitions[shard->auth_region]);
            }

            lnkBatchSend(link);
            netIdle(link, 0, 10.f);
            link->last_update_time = timerCpuSeconds(); // reset on disconnect in netLinkMonitor
        }
    }
}

static void conReset(void)
{
    disconnectAllShards = 1;
    printf("Resetting all shard connections.\n");
}

static void conShowShards(void)
{
    int i;
    U32 now = timerCpuSeconds();

    for(i = 0; i < g_missionServerState.shard_count; ++i)
    {
        MissionServerShard *shard = g_missionServerState.shards[i];
        if(!devassert(shard))
            continue;

        if(shard->link.connected)
        {
            printf("%s Shard %s(%s): connected, silent %is\n",
                            authregion_toString(shard->auth_region), shard->name,
                            shard->ip, now - shard->link.last_update_time);
        }
        else
        {
            printf( "%s Shard %s(%s): disconnected\n",
                            authregion_toString(shard->auth_region), shard->name,
                            shard->ip);
        }
    }
}

static void conPersist(void)
{
    loadstart_printf("Persisting all dbs synchronously...");
    plMergeAllSync();
    loadend_printf("");
}

static ConsoleDebugMenu s_debugmenu[] =
{
    { 'd', "list connected shards",        conShowShards    },
    { 'S', "persist dbs synchronously",    conPersist        },
    { 'R', "Reset links to all shards.  Temporarily Disconnects from shards.",    conReset        },
    { 0 }
};

void s_setPersistDefaults(PersistConfig **pconfig, const char *name, PersistType type)
{
    // fill db configs with any values not specified in the config
    char buf[MAX_PATH];
    PersistConfig *config = *pconfig;

    if(!config)
    {
        config = *pconfig = StructAllocRaw(sizeof(*config));
        StructInit(config, sizeof(*config), parse_PersistConfig);
    }

    if(config->type == PERSIST_INVALID)
    {
        config->type = type;
        config->flags = PERSIST_CRCDB | PERSIST_EXPONENTIAL | PERSIST_KEEPLASTJOURNAL;
    }

    if(!config->fname || !config->fname[0])
    {
        sprintf(buf, "%s/db_%s.txt", g_missionServerState.dir, name);
        config->fname = StructAllocString(buf);
    }

    if(!config->flush_time)
    {
        config->flush_time = MISSIONSERVER_PERSISTTIME;
        config->fail_time = MISSIONSERVER_PERSISTFAIL;
    }
}


void missionserver_init(void)
{
    int i;
    char *s;
    char buf[MAX_PATH];
    MissionServerCfg *cfg = &g_missionServerState.cfg;

    strcpy(g_missionServerState.dir, fileDataDir());
    s = strrchr(g_missionServerState.dir, '/');
    if(s)
        s[1] = '\0';
    else
        s = g_missionServerState.dir;
    strcat(g_missionServerState.dir, "missionserver");

    fileDisableWinIO(1);

    loadstart_printf("Loading configuration...");
    StructInit(cfg, sizeof(*cfg), parse_MissionServerCfg);
    if(    !fileLocateRead("server/db/mission_server.cfg", buf) ||
        !ParserLoadFiles(NULL, buf, NULL, 0, parse_MissionServerCfg, cfg, NULL, NULL, NULL) )
    {
        if(isProductionMode())
        {
            FatalErrorf("Error: could not load mission_server.cfg");
            exit(1);
        }
        else
        {
            MissionServerShard *shard;
            loadstart_printf("Adding local shard (devmode only)...");
            shard = StructAllocRaw(sizeof(MissionServerShard));
            shard->ip = "localhost";
            eaPush(&cfg->shards,shard);
            loadend_printf("");
        }
    }
    if(isProductionMode())
    {
        if(!eaSize(&cfg->shards))
        {
            FatalErrorf("Error: no shards specified in mission_server.cfg");
            exit(2);
        }

        for(i = 0; i < eaSize(&cfg->shards); i++)
        {
            MissionServerShard *shard = cfg->shards[i];
            if(    shard->auth_region != AUTHREGION_WW)
            {
                FatalErrorf("Error: shard %s in mission_server.cfg has invalid region %s",
                            shard->name, authregion_toString(shard->auth_region));
                exit(3);
            }
        }
    }
    s_setPersistDefaults(&cfg->userpersist,        "users",    PERSIST_DIFF_SMALL);
    s_setPersistDefaults(&cfg->arcpersist,        "arcs",        PERSIST_DIFF_SMALL);
    s_setPersistDefaults(&cfg->searchpersist,    "search",    PERSIST_MEMORY);
    s_setPersistDefaults(&cfg->composedpersist,    "composed",    PERSIST_MEMORY);
    s_setPersistDefaults(&cfg->substringpersist,"substring",PERSIST_MEMORY);
    s_setPersistDefaults(&cfg->quotedpersist,    "quoted",    PERSIST_MEMORY);
    s_setPersistDefaults(&cfg->varspersist,        "vars",        PERSIST_FLAT);
    loadend_printf("");
    missionserversearch_initTokenLists();

    loadstart_printf("Registering types..");
    missionserver_registerJournaledUserPersist(cfg->userpersist);
    missionserver_registerJournaledArcPersist(cfg->arcpersist); // SORTED!
    plRegisterQuick(MissionServerSearch,            parse_MissionServerSearch,            cfg->searchpersist,        PERSIST_STRICTDEFAULT);
    plRegisterQuick(MissionServerCSearch,            parse_MissionServerCSearch,            cfg->composedpersist,    PERSIST_STRICTDEFAULT);
    plRegisterQuick(MissionServerQuotedSearch,        parse_MissionServerSearch,            cfg->quotedpersist,        PERSIST_STRICTDEFAULT);
    plRegisterQuick(MissionServerVars,                parse_MissionServerVars,            cfg->varspersist,        PERSIST_STRICTDEFAULT);
    plRegisterQuick(MissionServerSubstringSearch,    parse_MissionServerSubstringSearch,    cfg->substringpersist,    PERSIST_STRICTDEFAULT);
    missionserver_meta_register();

    // FIXME: move these into the serverlib config
    meta_registerPrintfMessager(s_remoteMetaPrintf);
    loadend_printf("");
}

#define REBUILD_DB_FOR_LIVE 0
#if REBUILD_DB_FOR_LIVE

#define FIX_CHAT_HANDLES 0
#define REBUILD_BY_HONORS 0

#if FIX_CHAT_HANDLES
    #include "../Common/chatdb.h"

    ParseTable parse_email[] = {
        { "From",            TOK_INT(Email,from,0)    },
        { "Sent",            TOK_INT(Email,sent,0)    },
        { "Body",            TOK_STRING(Email,body,0)    },
        { "{",                TOK_START,        0                            },
        { "}",                TOK_END,            0                            },
        { "", 0, 0 }
    };

    ParseTable parse_watching[] = {
        { "Name",            TOK_STRING(Watching,name,0)    },
        { "LastRead",        TOK_INT(Watching,last_read,0)    },
        { "Access",            TOK_INT(Watching,access,0)    },
        { "{",                TOK_START,        0                            },
        { "}",                TOK_END,            0                            },
        { "", 0, 0 }
    };

    ParseTable parse_user[] = {
        { "AuthId",            TOK_PRIMARY_KEY|TOK_INT(User,auth_id,0)    },
        { "Name",            TOK_STRING(User,name,0)    },
        { "Access",            TOK_INT(User,access,0)    },
        { "AccessLevel",    TOK_U8(User,access_level,0)    },
        { "Hash0",            TOK_INT(User,hash0,0)    },
        { "Hash1",            TOK_INT(User,hash1,0)    },
        { "Hash2",            TOK_INT(User,hash2,0)    },
        { "Hash3",            TOK_INT(User,hash3,0)    },
        { "LastOnline",        TOK_INT(User,last_online,0)    },
        { "Silenced",        TOK_INT(User,silenced,0)    },
        { "Watching",        TOK_STRUCT(User,watching,parse_watching) },
        { "Friends",        TOK_INTARRAY(User,friends)    },
        { "Ignore",            TOK_INTARRAY(User,ignore)    },
        { "Email",            TOK_STRUCT(User,email,parse_email) },
        { "MessageHash",    TOK_INT(User,message_hash,0)    },
        { "ChatBan",        TOK_INT(User,chat_ban_expire,0)    },
        { "{",                TOK_START,        0                        },
        { "}",                TOK_END,            0                        },
        { "", 0, 0 }
    };
#endif

MissionServerArc* cloneArcForLive(MissionServerArc *arc_in, int id)
{
    MissionServerArc *arc = StructAllocRaw(sizeof(*arc));
    arc->id = id;
    arc->creator_id = arc_in->creator_id;
    arc->creator_region = arc_in->creator_region;
    arc->created = arc_in->created;
    arc->updated = arc_in->updated;

    arc->size = arc_in->size;
    arc->zsize = arc_in->zsize;
    arc->data = malloc(arc->zsize);
    memcpy(arc->data, arc_in->data, arc->zsize);

    arc->rating = MISSIONRATING_UNRATED;
    arc->honors = MISSIONHONORS_DEVCHOICE;
    arc->permanent = 1;
    arc->ban = MISSIONBAN_CSREVIEWED_APPROVED;

    StructCopy(parse_MissionSearchHeader, &arc_in->header, &arc->header, 0, 0);
#if FIX_CHAT_HANDLES
    {
        User *chatter;
        if(arc->creator_region == AUTHREGION_US)
        {
            chatter = plFind(User, arc->creator_id);
            assert(chatter);
            sprintf(arc->header.author, "@%s", chatter->name);
        }
        else
        {
            printf("rebuild","Warning: EU author not fixed:\n");
        }
    }
#endif

    return arc;
}

void rebuildDBForLive(void)
{
    int i;
    int keepers[] = { 1007, 1013, 1022, 1025, 1026, 1030, 1031, 1032, 1036, 1039, 1044, 1050, 1054 };

    MissionServerArc **arcs_in = (MissionServerArc**)ap_temp();
    MissionServerArc **arcs_out = (MissionServerArc**)ap_temp();
    MissionServerUser **users = (MissionServerUser**)ap_temp();

    int id = 1001;
    ap_append(&arcs_in, plGetAll(MissionServerArc), plCount(MissionServerArc));
    ap_append(&users, plGetAll(MissionServerUser), plCount(MissionServerUser));

#if FIX_CHAT_HANDLES
    {
        PersistConfig chatconf = {0};

        printf("Loading chat db\n");
        chatconf.type = PERSIST_FLAT;
        chatconf.flags = PERSIST_CRCDB;
        estrPrintf(&chatconf.fname, "%s/chatdb_users.txt", g_missionServerState.dir);
        plRegisterQuick(User, parse_user, &chatconf, PERSIST_STRICTDEFAULT);
        plLoad(User);
    }
#endif

    printf("Building new arc list\n");
#if REBUILD_BY_HONORS
    for(i = 0; i < ap_size(&arcs_in); i++)
    {
        MissionServerArc *arc = arcs_in[i];
        if(!arc->unpublished && arc->honors == MISSIONHONORS_DEVCHOICE)
        {
            printf("%d: keeping "ARC_PRINTF_FMT" \"%s\"\n", id, ARC_PRINTF(arc), arc->header.title);
            ap_push(&arcs_out, cloneArcForLive(arc, id++));
        }
        missionserver_ArchiveArcData(arc);
    }
#else
    for(i = 0; i < ARRAY_SIZE(keepers); i++)
    {
        MissionServerArc *arc = plFind(MissionServerArc, keepers[i]);
        printf("%d: keeping "ARC_PRINTF_FMT" \"%s\"\n", id, ARC_PRINTF(arc), arc->header.title);
        ap_push(&arcs_out, cloneArcForLive(arc, id++));
    }
    for(i = 0; i < ap_size(&arcs_in); i++)
        missionserver_ArchiveArcData(arcs_in[i]);
#endif

    printf("Destroying old dbs\n");
    plDeleteList(MissionServerArc, arcs_in, ap_size(&arcs_in));
    plResetAutoInrcrement(MissionServerArc);
    plDeleteList(MissionServerUser, users, ap_size(&users));

    printf("Building new dbs\n");
    for(i = 0; i < ap_size(&arcs_out); i++)
    {
        MissionServerUser *user;
        MissionServerArc *arc = plAdd(MissionServerArc, arcs_out[i]->id);
        StructCopy(parse_MissionServerArc, arcs_out[i], arc, 0, 0);
        missionserver_UpdateArcData(arc, arcs_out[i]->data);
        plDirty(MissionServerArc, arc);

        user = plFindOrAdd(MissionServerUser, arc->creator_id, arc->creator_region);
        user->inventory.badge_flags |= kBadgeArcDevChoice;
        plDirty(MissionServerUser, user);
    }

    printf("Flushing changes\n");
    missionserver_FlushAllArcData();
    plMergeAllSync();

    printf("Done\n");
    exit(0);
}
#endif

static void dump_playsvsvotes(void)
{
    int i, j;
    MissionServerUser **users = plGetAll(MissionServerUser);

    StashTable stash = stashTableCreateInt(0);
    StashTableIterator iter;
    StashElement elem;

    for(i = 0; i < eaSize(&users); i++)
    {
        U32 idx, val = 0;
        MissionServerUser *user = users[i];
        int plays = eaiSize(&user->arcids_played);
        int votes = 0;
        for(j = 0; j < MISSIONRATING_VOTETYPES; j++)
            votes += eaiSize(&user->arcids_votes[j]);
        assert(votes < (1 << 16) && plays < (1 << 16));
        idx = (plays << 16) | votes;

        if(idx) // only count those who have played or voted
        {
            stashIntFindInt(stash, idx, &val);
            stashIntAddInt(stash, idx, val+1, 1);
        }
    }

    {
        FILE *fout = fopenf("%s/plays_vs_votes.csv", "wb", g_missionServerState.dir);
        fprintf(fout, "Plays,Ratings,Count\n");
        for(stashGetIterator(stash, &iter); stashGetNextElement(&iter, &elem);)
        {
            U32 idx = stashElementGetIntKey(elem);
            U32 val = stashElementGetInt(elem);
            fprintf(fout, "%d,%d,%d\n", idx>>16, idx&((1<<16)-1), val);
        }
        fclose(fout);
    }
}

#define MAX_PUBLISH_TRACKED 10
static void dump_usercounts(void)
{
    int i, j;
    MissionServerUser **users = plGetAll(MissionServerUser);
    int count_vote[MISSIONRATING_VOTETYPES] = {0};
    int count                = 0;
    int count_players        = 0;
    int count_voters        = 0;
    int count_complainers    = 0;
    int count_creators        = 0;
    int count_play            = 0;
    int count_report        = 0;
    int count_published        = 0;
    int count_unpublished    = 0;
    int count_permanent        = 0;
    int count_users_per_slot[MAX_PUBLISH_TRACKED] = {0};

    for(i = 0; i < eaSize(&users); i++)
    {
        int voted = 0;
        MissionServerUser *user = users[i];
        for(j = 0; j < MISSIONRATING_VOTETYPES; j++)
        {
            count_vote[j] += eaiSize(&user->arcids_votes[j]);
            if(eaiSize(&user->arcids_votes[j]))
                voted = 1;
        }
        for(j = 0; j < MAX_PUBLISH_TRACKED; j++)
        {
            if(eaiSize(&user->arcids_published) == j || (j == MAX_PUBLISH_TRACKED-1 && eaiSize(&user->arcids_published)>j))
                count_users_per_slot[j]++;
        }
        count++;
        if(eaiSize(&user->arcids_played))
            count_players++;
        if(voted)
            count_voters++;
        if(eaiSize(&user->arcids_complaints))
            count_complainers++;
        if(eaiSize(&user->arcids_published) || eaiSize(&user->arcids_unpublished) || eaiSize(&user->arcids_permanent))
            count_creators++;
        count_play            += eaiSize(&user->arcids_played);
        count_report        += eaiSize(&user->arcids_complaints);
        count_published        += eaiSize(&user->arcids_published);
        count_unpublished    += eaiSize(&user->arcids_unpublished);
        count_permanent        += eaiSize(&user->arcids_permanent);
    }

    {
        FILE *fout = fopenf("%s/player_counts.csv", "wb", g_missionServerState.dir);
        fprintf(fout, "Users,%d,(Players who have used Architect in any capacity)\n", count);
        fprintf(fout, "Players,%d,(Players who have gotten credit for playing at least one arc)\n", count_players);
        fprintf(fout, "Voters,%d,(Players who have voted on at least one arc)\n", count_voters);
        fprintf(fout, "Complainers,%d,(Players who have reported at least one arc)\n", count_complainers);
        fprintf(fout, "Creators,%d,(Players who have published at least one arc)\n", count_creators);
        for(j = 0; j < MISSIONRATING_VOTETYPES; j++)
            fprintf(fout, "%d Star Votes,%d,(%d star votes ever given)\n", j, count_vote[j], j);
        for(j = 0; j < MAX_PUBLISH_TRACKED; j++)
            fprintf(fout, "%d Slots used,%d, (users have used %d of their publish slots)\n", j, count_users_per_slot[j], j);
        fprintf(fout, "Plays,%d,(Times players have gotten credit for playing an arc)\n", count_play);
        fprintf(fout, "Reports,%d,(Times players have reported an arc)\n", count_report);
        fprintf(fout, "Publishes,%d,(Times players have published an arc)\n", count_published + count_unpublished + count_permanent);
        fprintf(fout, "Unpublishes,%d,(Times players have unpublished an arc)\n", count_unpublished);
//        fprintf(fout, "Honors,%d\n", count_permanent);
        fclose(fout);
    }
}

static void dump_arccounts(void)
{
    int i, j;
    MissionServerArc **arcs = plGetAll(MissionServerArc);
    int count                = 0;
    int count_reported        = 0;
    int count_ww            = 0;
    int count_republished    = 0;
    int count_vote[MISSIONRATING_RATINGTYPES] = {0};
    int count_honors[MISSIONHONORS_TYPES] = {0};
    int keyword_votes[MISSIONSERVER_MAX_KEYWORDS] = {0};
    int totalCompletions = 0;

    for(i = 0; i < eaSize(&arcs); i++)
    {
        MissionServerArc *arc = arcs[i];
        if(!arc->unpublished)
        {
            count++;
            if(eaSize(&arc->complaints))
                count_reported++;
            if(arc->creator_region == AUTHREGION_WW)
                count_ww++;
            if(arc->updated != arc->created)
                count_republished++;
            if(arc->honors == MISSIONHONORS_NONE)
                count_vote[arc->rating]++;
            count_honors[arc->honors]++;

            //get flags
            {
                int top[3] = {-1};
                totalCompletions+=arc->total_completions;
                for( j = 0; j < MISSIONSERVER_MAX_KEYWORDS; j++  )
                {

                    // used to update top 3 list
                    if( top[0] == -1 || arc->keyword_votes[j] > arc->keyword_votes[top[0]] )
                    {
                        top[2] = top[1];
                        top[1] = top[0];
                        top[0] = j;
                    }
                    else if( top[1] == -1 || arc->keyword_votes[j] > arc->keyword_votes[top[1]] )
                    {
                        top[2] = top[1];
                        top[1] = j;
                    }
                    else if( top[2] == -1 || arc->keyword_votes[j] > arc->keyword_votes[top[2]] )
                    {
                        top[2] = j;
                    }
                }
                for(j = 0; j < 3; j++)
                {
                    int idx= top[j];
                    if(idx>=0)
                    {
                        int seen=0, k;
                        for(k = 0; k < j;k++)
                        {
                            if(top[k] == top[j])
                                seen = 1;
                        }
                        if(!seen)
                            keyword_votes[idx] += arc->total_completions;
                    }
                }
            }
        }
    }
    
    {
        FILE *fout = fopenf("%s/arc_counts.csv", "wb", g_missionServerState.dir);
        fprintf(fout, "Published,%d\n", count);
        fprintf(fout, "Reported,%d\n", count_reported);
        fprintf(fout, "WW,%d\n", count_ww);
        fprintf(fout, "Republished,%d\n", count_republished);
        fprintf(fout, "----rating----\n");
        for(i = 0; i < MISSIONRATING_RATINGTYPES; i++)
        {
            if(i <= MISSIONRATING_MAX_STARS)
                fprintf(fout, "%d star Rated arcs,%d, (arcs with rating of %d )\n", i,count_vote[i], i);
            else if(count_vote[i] && i == MISSIONRATING_MAX_STARS_PROMOTED)
                fprintf(fout, "honorary 5 star arcs,%d, (arcs with rating of %d )\n", count_vote[i], i);
            else if (count_vote[i])
                fprintf(fout, "unrated arcs,%d, (arcs with rating of %d )\n", count_vote[i], i);
        }
        fprintf(fout, "----honors----\n");
        for(i = 0; i < MISSIONHONORS_TYPES; i++)
        {
            char *honor = 0;
            if(i == MISSIONHONORS_NONE)
                fprintf(fout, "unhonored arcs,%d, (total arcs with honor of %d )\n", count_honors[i], i);
            if(i == MISSIONHONORS_FORMER_DC)
                fprintf(fout, "arcs removed from dev choice,%d, (total arcs with honor of %d )\n", count_honors[i], i);
            if(i == MISSIONHONORS_FORMER_DC_HOF)
                fprintf(fout, "arcs removed from dev choice current hall of fame,%d, (total arcs with honor of %d )\n", count_honors[i], i);
            if(i == MISSIONHONORS_HALLOFFAME)
                fprintf(fout, "hall of fame arcs never dev choiced,%d, (total arcs with honor of %d )\n", count_honors[i], i);
            if(i == MISSIONHONORS_DEVCHOICE)
                fprintf(fout, "dev choice arcs without hall of fame,%d, (total arcs with honor of %d )\n", count_honors[i], i);
            if(i == MISSIONHONORS_DC_AND_HOF)
                fprintf(fout, "dev choice and HoF arcs,%d, (total arcs with honor of %d )\n", count_honors[i], i);
            if(i == MISSIONHONORS_CELEBRITY)
                fprintf(fout, "Celebrity arcs,%d, (total arcs with honor of %d )\n", count_honors[i], i);
        }
        fprintf(fout, "----keywords----\n");
        for(i = 0; i < MISSIONSERVER_MAX_KEYWORDS; i++)
        {
            fprintf(fout, "keyword %d: %d completions\n", i, keyword_votes[i]);
        }

        fclose(fout);
    }
}

static void dump_honoredArcData(void)
{
    int i, j;
    MissionServerArc **arcs = plGetAll(MissionServerArc);
    int totalVotes            = 0;
    int totalCompletions    = 0;
    int *honoredIds = 0;
    FILE *fout;

    eaiCreate(&honoredIds);

    for(i = 0; i < eaSize(&arcs); i++)
    {
        MissionServerArc *arc = arcs[i];
        if(!arc->unpublished && arc->honors > MISSIONHONORS_FORMER_DC_HOF && arc->honors != MISSIONHONORS_NONE)
        {
            eaiPush(&honoredIds, i);
        }
    }

    fout = fopenf("%s/honored_arc_data.csv", "wb", g_missionServerState.dir);
    fprintf(fout, "Arc ID, votes, rating, completions, percent voted\n");
    for(i = MISSIONHONORS_HALLOFFAME; i < MISSIONHONORS_TYPES; i++)
    {
        if(i == MISSIONHONORS_HALLOFFAME)
            fprintf(fout, "\nhall of fame arcs never dev choiced:\n");
        if(i == MISSIONHONORS_DEVCHOICE)
            fprintf(fout, "\ndev choice arcs without hall of fame:\n");
        if(i == MISSIONHONORS_DC_AND_HOF)
            fprintf(fout, "\ndev choice and HoF arcs:\n");
        if(i == MISSIONHONORS_CELEBRITY)
            fprintf(fout, "\nCelebrity arcs:\n");
        for(j = 0; j < eaiSize(&honoredIds); j++)
        {
            MissionServerArc *arc = arcs[honoredIds[j]];
            if(arc->honors == i)
            {
                float rating = ((float)arc->total_stars/(float)arc->total_votes);
                float percentVoted = ((float)arc->total_votes/(float)arc->total_completions);
                fprintf(fout, "%d, %d, %f, %d,%f,,,\"%s\"\n", arc->id, arc->total_votes, rating, arc->total_completions, percentVoted, arc->header.title);
            }
        }
    }
    fclose(fout);
    eaiDestroy(&honoredIds);
}

static void dump_publishedbytime(int offset, int radix, const char *name)
{
#define LAST() ((item*)eaLast(&items))
    typedef struct item
    {
        char date[100];
        int current;
        int ever;
    } item;

    int i, j;
    MissionServerArc **arcs = plGetAll(MissionServerArc);
    item **items = NULL;

    U32 first = (eaSize(&arcs) > offset) ? (arcs[offset]->created / radix) : 0;

    for(i = offset; i < eaSize(&arcs); i++)
    {
        MissionServerArc *arc = arcs[i];
        int from = arc->created / radix - first;
        int to = (arc->unpublished ? arc->unpublished / radix - first : from) + 1; // lets be optimistic and say it was unpublished at the very end of that time

        while(eaSize(&items) <= to)
        {
            item *n = malloc(sizeof(item));
            n->current = eaSize(&items) ? LAST()->current : 0;
            n->ever = eaSize(&items) ? LAST()->ever : 0;
            timerMakeDateStringFromSecondsSince2000(n->date, (first + eaSize(&items)) * radix);
            eaPush(&items, n);
        }

        if(!arc->unpublished)
            to = eaSize(&items);

        for(j = from; j < to; j++)
        {
            items[j]->current++;
            items[j]->ever++;
        }
        for(; j < eaSize(&items); j++)
            items[j]->ever++;
    }

    {
        FILE *fout = fopenf("%s/published_by_%s.csv", "wb", g_missionServerState.dir, name);
        fprintf(fout, "Published\n");
        fprintf(fout, "%s,Current,Ever\n", name);
        for(i = 0; i < eaSize(&items); i++)
            fprintf(fout, "%s,%d,%d\n", items[i]->date, items[i]->current, items[i]->ever);
        fclose(fout);
    }

    eaDestroyEx(&items, NULL);
}

static void dump_GuestAuthorData(void)
{
    int i;
    MissionServerArc **arcs = plGetAll(MissionServerArc);
    int guestAuthorCount = 0;
    int completionCount = 0;

    FILE *fout = fopenf("%s/guestAuthor.csv", "wb", g_missionServerState.dir);
    fprintf(fout, "Guest Author Arcs Stats:\n");
    fprintf(fout, "Name,Completions\n");
    for(i = 0; i < eaSize(&arcs); i++)
    {
        MissionServerArc *arc = arcs[i];
        if(!arc->unpublished && (arc->honors == MISSIONHONORS_CELEBRITY))
        {
            fprintf(fout, "\"%s\"", arc->header.title);
            fprintf(fout, ",%d", arc->total_completions);
            fprintf(fout, "\n");
            guestAuthorCount += arc->total_completions;
        }
        completionCount += arc->total_completions;
    }

    fprintf(fout, "\n");
    fprintf(fout, "Total plays stats\n");
    fprintf(fout, "Total completions to date, Total Guest Author completions to date\n");
    fprintf(fout, "%d,%d\n", completionCount, guestAuthorCount);
    fclose(fout);
}

static void dump_StartCompleteData(void)
{
    int i;
    MissionServerArc **arcs = plGetAll(MissionServerArc);
    MissionServerUser **users = plGetAll(MissionServerUser);
    int guestAuthorCount = 0;
    int completionCount = 0;
    int startCount = 0;
    int mostRecentActivity = 0;
    int numWeeks = 4;
    char mostRecentDate[100];

    FILE *fout = fopenf("%s/startComplete.csv", "wb", g_missionServerState.dir);
    fprintf(fout, "Start/Complete stats:\n");
    for(i = 0; i < eaSize(&arcs); i++)
    {
        MissionServerArc *arc = arcs[i];
        startCount += arc->total_starts;
        completionCount += arc->total_completions;
    }

    fprintf(fout, "Total plays stats\n");
    fprintf(fout, "Total arcs to date, Total starts to date, Total completions to date\n");
    fprintf(fout, "%d, %d,%d\n", eaSize(&arcs), startCount, completionCount);
    fprintf(fout, "\n");
    fprintf(fout, "\n");
    
    for (i = 0; i < eaSize(&users); ++i)
    {
        int j;
        for (j = 0; j < eaiSize(&users[i]->arcids_played_timestamp); j++)
        {
            mostRecentActivity = MAX(mostRecentActivity,users[i]->arcids_played_timestamp[j]);
        }
        for (j = 0; j < eaiSize(&users[i]->arcids_started_timestamp); j++)
        {
            mostRecentActivity = MAX(mostRecentActivity,users[i]->arcids_started_timestamp[j]);
        }
    }
    mostRecentActivity += 10;    //    give a small buffer
    
    timerMakeDateStringFromSecondsSince2000(mostRecentDate, mostRecentActivity);
    fprintf(fout, "Stats up until %s\n", mostRecentDate);
    for (i = 0; i < numWeeks; ++i)
    {
        int j;
        int secondsPerWeek = SECONDS_PER_DAY*7;
        int startTime = mostRecentActivity - (secondsPerWeek)*(numWeeks-i);
        int endTime = startTime + secondsPerWeek;
        int numStarts = 0;
        int numCompletes = 0;
        char startDate[100];
        char endDate[100];
        timerMakeDateStringFromSecondsSince2000(startDate, startTime);
        timerMakeDateStringFromSecondsSince2000(endDate, endTime);
        fprintf(fout, "Week %i (%s - %s)\n", i+1, startDate, endDate);
        fprintf(fout, "Users:");
        for (j = 0; j < eaSize(&users); ++j)
        {
            int k;
            int foundUser = 0;
            MissionServerUser *user = users[j];
            for (k = 0; k < eaiSize(&user->arcids_started_timestamp); ++k)
            {
                if (INRANGE(user->arcids_started_timestamp[k], startTime, endTime))
                {
                    numStarts++;
                    foundUser = 1;
                }
            }
            for (k = 0; k < eaiSize(&user->arcids_played_timestamp); ++k)
            {
                if (INRANGE(user->arcids_played_timestamp[k], startTime, endTime))
                {
                    numCompletes++;
                    foundUser = 1;
                }
            }
            if (foundUser)
            {
                fprintf(fout, ",%s:%u",authregion_toString(user->auth_region),user->auth_id);
            }
        }
        fprintf(fout, "\n");
        fprintf(fout, "Total \"Last Started\":,%i,Total \"Last Completed\":,%i\n",numStarts, numCompletes);
    }

    fclose(fout);
}
void missionserver_load(void)
{
    loadstart_printf("loading live config...");
    g_missionServerState.vars = plFind(MissionServerVars);
    if(!g_missionServerState.vars)
    {
        MissionServerVars *vars = plAdd(MissionServerVars);

        // fixup since textparser doesn't like fractions for defaults...
        vars->halloffame_rating = 4.5;

        g_missionServerState.vars = vars;
    }
    loadend_printf("");

    loadstart_printf("removing partial publishes...");
    s_arcsPreLoad();
    loadend_printf("");

#if MISSIONSERVER_LOADALL_HOGGS
    loadstart_printf("loading arc data..");
    missionserver_LoadAllArcData(plGetAll(MissionServerArc), plCount(MissionServerArc));
    if(g_missionServerState.forceLoadDb)
    {
        MissionServerArc **removed = 0;
        MissionServerArc **changed = 0;
        missionserver_getLoadMismatches(&removed, &changed);

        while(eaSize(&changed))
            plDirty(MissionServerArc, eaPop(&changed));

        plDeleteList(MissionServerArc, removed, eaSize(&removed));

        missionserver_clearLoadMismatches();
    }
    loadend_printf("");
#endif

#if REBUILD_DB_FOR_LIVE
    rebuildDBForLive();
#endif

    loadstart_printf("fixup etc..");
    s_arcsPostLoad();
    s_usersPostLoad();
    loadend_printf("");

    if (g_missionServerState.dumpStats)
    {
        // I'm checking these in in case anyone wants to play with them
        // it's safe to run these without data, but make sure you exit()!
        // TODO: make these command line parameters or console debug commands
        s_usersPostLoad();
        loadstart_printf("Writing plays vs votes stats..");
        dump_playsvsvotes();
        loadend_printf("");
        loadstart_printf("Writing user counts stats..");
        dump_usercounts();
        loadend_printf("");
        loadstart_printf("Writing arc counts stats..");
        dump_arccounts();
        loadend_printf("");
        loadstart_printf("Writing honored Arc stats..");
        dump_honoredArcData();
        loadend_printf("");
        loadstart_printf("Writing Published by time stats..");
        dump_publishedbytime(31, SECONDS_PER_HOUR, "Hour");
        loadend_printf("");
        loadstart_printf("Writing Guest Author stats..");
        dump_GuestAuthorData();
        loadend_printf("");
        loadstart_printf("Writing Starts and Completes..");
        dump_StartCompleteData();
        loadend_printf("");

        //    dump_publishedbytime(31, SECONDS_PER_MINUTE, "Minute");
        exit(0);
    }

    g_missionServerState.tick_timer = timerAlloc();
    g_missionServerState.packet_timer = timerAlloc();
}

void missionserver_tick(void)
{
    int count;
    MissionServerUser **users;
    F32 time_elapsed;

    #ifdef TEST
    MissionServerSearchTest_FullRun();
    #endif

    timerStart(g_missionServerState.packet_timer);
    shardNetConnectTick(&g_missionServerState);
    shardNetProcessTick(&g_missionServerState);

    time_elapsed = timerElapsed(g_missionServerState.tick_timer);

    if(time_elapsed > (1.f / MISSIONSERVER_TICK_FREQ))
    {
        char *buf = Str_temp();
        int i, connected = 0;

        for(i = 0; i < g_missionServerState.shard_count; ++i)
            if(g_missionServerState.shards[i]->link.connected)
                connected++;

        Str_catf(&buf, "%d: MissionServer. %d shards (%d). ", _getpid(), connected, (g_missionServerState.tick_count / MISSIONSERVER_TICK_FREQ) % 10);
        setConsoleTitle(buf);

        timerStart(g_missionServerState.tick_timer);
        g_missionServerState.tick_count++;
        Str_destroy(&buf);
    }

    /*if(time_elapsed >MISSIONSERVER_SEARCHCLEAR_INTERVAL)
        missionserver_updateSearchCache();*/

    users = plGetAll(MissionServerUser);
    for(count = 0; count < eaSize(&users); count++)
    {
        MissionServerUser *user = users[count];
        if(user)
        {
            if(user->inventory.dirty && user->shard_dbid && user->shard_link)
                s_sendUserInventory(&user->inventory, user->banned_until, user->shard_dbid, user->auth_id, user->shard_link, eaiSize(&user->arcids_published), eaiSize(&user->arcids_permanent),
                eaiSize(&user->arcids_invalid), eaiSize(&user->arcids_playableErrors));
            user->inventory.dirty = false;
        }
    }
}

void missionserver_stop(void)
{
    int i, j;

    loadstart_printf("flushing arc data..");
    missionserver_FlushAllArcData();
    loadend_printf("");

    for(i = 0; i < ARRAY_SIZE(g_missionServerState.petitions); i++)
    {
        for(j = 0; j < as_size(&g_missionServerState.petitions[i]); j++)
        {
            MissionServerPetition *petition = &g_missionServerState.petitions[i][j];
            LOG( LOG_MISSIONSERVER, LOG_LEVEL_VERBOSE, 0, "Warning: unable to send petition to %s (%s)\n%s\n%s\n%s",
                            petition->authname, petition->character ? petition->character : "N/A",
                            petition->summary, petition->message, petition->notes);
        }
    }
}

void missionserver_http(char **str)
{
#define sendline(fmt, ...)    Str_catf(str, fmt"\n", __VA_ARGS__)
#define rowclass            ((row++)&1)

    int i;
    U32 now = timerCpuSeconds();

    int row = 0;
    sendline("<table>");
    sendline("<tr class=rowt><th colspan=6>MissionServer Summary");
    sendline("<tr class=rowh><th>Shard<th>Region<th>IP Address<th>Connected");
    for(i = 0; i < g_missionServerState.shard_count; ++i)
    {
        MissionServerShard *shard = g_missionServerState.shards[i];
        if(shard)
        {
            sendline("<tr class=row%d><td>%s<td>%s<td>%s<td>%s", rowclass,
                        shard->name, authregion_toString(shard->auth_region),
                        shard->ip, shard->link.connected ? "&#x2713;" : "");
        }
    }
    sendline("</table>");
}

void missionserver_updateLegacyTimeStamps(MissionServerUser *user, int journalChange)
{
    assert(user);
    if (user)
    {
        int start = eaiSize(&user->arcids_played_timestamp);
        int end = eaiSize(&user->arcids_played);
        int diff = end-start;
        if (diff)
        {
            int i;
            assert(diff > 0);                                    //    the timestamp array should not exceed the completion array
            assert(!start);                                        //    This should only be happening if this is the first time that the user completed
                                                                //    an arc, since we added timestamps
            for (i = start; i < end; i++)
            {
                if (journalChange)
                    missionserver_journalUserArcCompletedUpdate(user, user->arcids_played[i], 0);
                eaiPush(&user->arcids_played_timestamp, 0);
            }
        }
    }
}
#include "autogen/missionserver_h_ast.c"
