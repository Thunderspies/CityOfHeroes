/***************************************************************************
 *     Copyright (c) 2004-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#ifndef BADGES_H__
#define BADGES_H__

#include <utilitieslib/utils/textparser_types.h>
#include <utilitieslib/utils/mathutil.h> // MAX()

typedef struct StuffBuff StuffBuff;
typedef struct StashTableImp *StashTable;
typedef const struct StashTableImp *cStashTable;
typedef struct RewardItemSet RewardItemSet;
typedef struct Entity Entity;
typedef struct Packet Packet;

// Pretty much just used for arrays.  Incrementing these by 1000 should mesh nicely with the DB.
#define BADGE_ENT_MAX_STATS        2862
#define BADGE_SG_MAX_STATS        2000

#define BADGE_ENT_MAX_BADGES            4096
#define BADGE_ENT_BITFIELD_SIZE            MAX(((BADGE_ENT_MAX_BADGES+31)/32), 263)    // for a U32 array, previously calculated incorrectly, hence the MAX()
#define BADGE_ENT_BITFIELD_BYTE_SIZE    (BADGE_ENT_BITFIELD_SIZE*4)                    // 4 bytes per U32
STATIC_ASSERT(BADGE_ENT_BITFIELD_BYTE_SIZE < (2303)); // that's 18,000+ badges, good luck
// any bigger than this and you'll overflow the SQL rows in the Badges table
// find somewhere else to put this data!

// This is how many badges we store in the most-recently-earned array.
#define BADGE_ENT_RECENT_BADGES        50

// This is how many badges a player can monitor at a time.
#define MAX_BADGE_MONITOR_ENTRIES 10

#define BADGE_SG_MAX_BADGES            1050                            // make sure the Supergroups table has enough room when you change this
#define BADGE_SG_BITFIELD_SIZE        ((BADGE_SG_MAX_BADGES+31)/32)    // for a U32 array
#define BADGE_SG_BITFIELD_BYTE_SIZE    (BADGE_SG_BITFIELD_SIZE*4)        // 4 bytes per U32

// String buffer size for badge progress string
#define BADGE_PROGRESS_STRING_BUFFER_SIZE 128

typedef struct SgrpBadges
{
    U32 *eaiStates;                            // size is BADGE_MAX_BADGES, comes from StatServer
    U32 abitsOwned[BADGE_SG_BITFIELD_SIZE];    // bitfield: badge 'i' is owned if set. aiBadgesOwned
} SgrpBadges;

typedef struct SgrpBadgeStats
{
    int aiStats[BADGE_SG_MAX_STATS];
} SgrpBadgeStats;

#define sgrpbadges_IsOwned(psgrpbadges, idx) BitFieldGet((psgrpbadges)->abitsOwned, BADGE_SG_BITFIELD_SIZE, (idx))
#define sgrpbadges_SetOwned(psgrpbadges, idx, set) BitFieldSet((psgrpbadges)->abitsOwned, BADGE_SG_BITFIELD_SIZE, (idx), !!(set))

typedef enum CollectionType
{
    kCollectionType_Badge = 0,
    kCollectionType_Market,
    kCollectionType_SuperPack,
    kCollectionType_Incarnate,
    kCollectionType_Supergroup,
    kCollectionType_Count,

} CollectionType;

typedef enum BadgeType
{
    kBadgeType_None = 0,
#define kBadgeType_Internal kBadgeType_None

    // Old categories, some of which may be going away after the re-org.
    kBadgeType_Tourism,
    kBadgeType_History,
    kBadgeType_Accomplishment,
    kBadgeType_Achievement,
    kBadgeType_Perk,
    kBadgeType_Gladiator,
    kBadgeType_Veteran,

    // New categories, for badges to be re-orged into.
    kBadgeType_PVP,
    kBadgeType_Invention,
    kBadgeType_Defeat,
    kBadgeType_Event,
    kBadgeType_Flashback,
    kBadgeType_Auction,
    kBadgeType_DayJob,
    kBadgeType_Architect,                                
    
    // if you add more badge categories, add them before this
    kBadgeType_LastBadgeCategory,

    kMarketType_Content,
    kMarketType_SignatureStoryArc1,
    kMarketType_SignatureStoryArc2,

    kSuperPackType_HeroesAndVillains,
    kSuperPackType_RoguesAndVigilantes,

    kIncarnateType_Empyrean,
    kIncarnateType_Astral,

    // supergroup badges use the regular badge types

    kBadgeType_Count,
} BadgeType;

// This is a fake category used to denote the array of most recently awarded
// badges.
#define kBadgeType_MostRecent        -1
#define kBadgeType_NearCompletion    -2

typedef struct BadgeDef
{
    int iIdx;

    const char *pchName;
        // The internal name of the badge. This is how it is referenced in
        //   code and other data files.

    const char* filename;
        // Filename of the badge used for click to source


    CollectionType eCollection;

    BadgeType eCategory;
        // The category the badge should be placed in. Right now,
        //   we expect "Tourism", "History", "Accomplishment", "Achievement",
        //   and "Perk"


    const char *pchDisplayTitle[2];
        // The title that is conferred onto the player once they earn the
        //   badge. (Heroes get the first, villains the second.)

    const char *pchDisplayHint[2];
        // If the badge is is "known", then this hint is displayed as the
        //   text of the badge. (Heroes get the first, villains the second.)

    const char *pchDisplayText[2];
        // Once the badge is "known" and "visible", or is earned, this text
        //   is displayed for it. (Heroes get the first, villains the second.)

    const char *pchIcon[2];
        // The icon for the badge with is displayed if the badge is "known"
        //   and "visible", or the badge has been earned. Otherwise, if the
        //   badge is "visible", then a generic ? is displayed with the
        //   hint text. (Heroes get the first, villains the second.)

    const char **ppchReward;
        // The reward def to grant to the player when the get this badge.

    const char **ppchKnown;
        // An expression which describes the conditions under which the
        //   badge is known by the player. Once a badge is known (and
        //   visible), the DisplayHint and a paritally hidden icon are
        //   displayed. Once a player earns a badge, it is always known.

    const char **ppchVisible;
        // An expression which describes the conditions under which the
        //   badge is visible to the player. If this expression is false,
        //   and the player doesn't own the badge, then there is no
        //   indication that the badge exists. Once a player earns a badge,
        //   it is always visible.

    const char **ppchRequires;
        // An expression which describes the conditions under which the
        //   player earns the badge. Once the player earns a badge, it is
        //   always known and visible. Once a badge is earned, it can't
        //   be taken away, even if this expression becomes untrue.

    const char **ppchMeter;
        // An expression which returns a value from 0 to 100 designating
        //   how close to completion a particular badge is. (100 meaning
        //   that it's complete or nigh-complete.) An empty expression
        //   means no meter.

    const char **ppchRevoke;
        // An expression which describes the conditions under which the
        //   badge is revoke. Once revoked, all the other expressions are
        //   re-evaluated immediately. Make sure that the revoke and the
        //   ppchRequires cannot be simultaneously true.
    
    const char *pchDisplayButton;
        // The text on a button used to claim rewards that are not granted
        //    immediately, but when the player clicks the button.  An
        //    empty string indicates no such reward.

    const char **ppchButtonReward;
        // The reward table for the button described above.  Note that the
        //    badge will NOT automatically be revoked.

    bool bDoNotCount;
        // Do not count in badgecount evals.
        // Do not show in player info.

    const char *pchDisplayPopup;
        // The SMF text to show when the badge is earned.

    const char *pchAwardText;
        // Custom floater text to be shown instead of the default message.

    U8 rgbaAwardText[4];
        // Color to use for floater text

    const char **ppchContacts;
        // List of contacts to be issued when the badge is awarded.

    U32 iProgressMaxValue;
        // Real value (instead of scaled 0 to 100 result of ppchMeter) that is
        // the maximum quantity for the progress meter. This'll be used in the
        // tooltip.

    const char *steamExport;
        // Internal name of what badge to export this to Steam as when awarded
        // Intended for when multiple badges link to a single Steam achievement (e.g. Level10 and P_Level_10)

} BadgeDef;

typedef struct BadgeDefs
{
    const BadgeDef **ppBadges;
    int idxMax;    // the max valid idx, inclusive.
    cStashTable hashByName;
    cStashTable hashByIdx;
} BadgeDefs;

extern TokenizerParseInfo ParseBadgeDef[];
extern TokenizerParseInfo ParseBadgeDefs[];
extern SHARED_MEMORY BadgeDefs g_BadgeDefs;
extern SHARED_MEMORY BadgeDefs g_SgroupBadges;

typedef struct RecentBadge
{
    int idx;                // index as stored in badge defs array
    U32 timeAwarded;        // seconds since 2000
} RecentBadge;

typedef struct BadgeMonitorInfo
{
    int iIdx;                // index as stored in badge defs array
}BadgeMonitorInfo;

int badge_LoadNames(StashTable hashNames, const char *fileName);

char *badge_CategoryGetName(Entity *e, CollectionType collection, BadgeType category);
char *badge_CollectionGetName(CollectionType collection);

/**********************************************************************
* badge_getProgressString: Get a string representing the entity
*
* Arg [buffer]: The buffer to put the progress string into. Size should be at least BADGE_PROGRESS_STRING_BUFFER_SIZE.
* Arg [player]: The entity whose progress you want to check
* Arg [badge]: The badge definition
*
* Return: The same buffer that was given in [buffer], to ease usage.
**********************************************************************/
const char* badge_getProgressString(char *buffer, const Entity* player, const BadgeDef* badge);


/**********************************************************************
* badge_GetAnyBadgeByIdx: Get any badge by index
*
* Arg [iIdx]: The index of the badge
*
* Return: BadgeDef of a badge by index. The index can be an index for a
*         supergroup badge or a player badge.
**********************************************************************/
const BadgeDef* badge_GetAnyBadgeByIdx(int iIdx);

bool badge_GetIdxFromName(const char *pch, int *pi);
const BadgeDef *badge_GetBadgeByName(const char *pch);
const BadgeDef *badge_GetBadgeByIdx(int iIdx);
const BadgeDef *badge_GetSgroupBadgeByIdx(int iIdx);
bool badge_FindName(const char * pchName);
const BadgeDef *badgedefs_DefFromName(const BadgeDefs *defs, char const *name);
void MarkModifiedBadges(StashTable hashBadgeStatUsage, U32 aiBadges[], const char *pchStat);
void badge_ClearUpdatedAll(Entity *e);
void badgeMonitor_SendInfo( Entity *e, Packet *pak );
void badgeMonitor_ReceiveInfo( Entity *e, Packet *pak );
int badgeMonitor_CanInfoBeAdded( Entity *e, int idx, bool is_supergroup );
void badgeMonitor_CheckAndSortInfo( Entity *e );
int badgeMonitor_AddInfo( Entity *e, int idx, bool is_supergroup );
int entity_OwnsBadge( Entity *e, const char * badgename );


#if CLIENT | SERVER
char* badge_setTitle( Entity * e, int idx );
char* badge_setTitleId( Entity * e, int idx );
#endif

//
// These are used for storing the badge info in Entity.pl.aiBadges
//
#define BADGE_OWNS_MASK        (0x80000000)
#define BADGE_VISIBLE_MASK     (0x40000000)
#define BADGE_KNOWN_MASK       (0x20000000)
#define BADGE_CHANGED_MASK     (0x10000000)
#define BADGE_UPDATED_MASK     (0x08000000)
#define BADGE_REWARD_MASK      (0x04000000)

#define BADGE_COMPLETION_MASK  (0x000fffff)
#define BADGE_FULL_OWNERSHIP   (BADGE_KNOWN_MASK|BADGE_VISIBLE_MASK|BADGE_OWNS_MASK|BADGE_COMPLETION_MASK)

#define BADGE_IS_OWNED(b)      ((b)&BADGE_OWNS_MASK)

#define BADGE_SET_CHANGED(b)   ((b)|=BADGE_CHANGED_MASK)
#define BADGE_CLEAR_CHANGED(b) ((b)&=~BADGE_CHANGED_MASK)
#define BADGE_IS_CHANGED(b)    ((b)&BADGE_CHANGED_MASK)

#define BADGE_SET_VISIBLE(b)   ((b)|=BADGE_VISIBLE_MASK)
#define BADGE_IS_VISIBLE(b)    ((b)&BADGE_VISIBLE_MASK)

#define BADGE_SET_KNOWN(b)     ((b)|=BADGE_KNOWN_MASK)
#define BADGE_CLEAR_KNOWN(b)   ((b)&=~BADGE_KNOWN_MASK)
#define BADGE_IS_KNOWN(b)      ((b)&BADGE_KNOWN_MASK)

#define BADGE_SET_UPDATED(b)   ((b)|=BADGE_UPDATED_MASK)
#define BADGE_CLEAR_UPDATED(b) ((b)&=~BADGE_UPDATED_MASK)
#define BADGE_IS_UPDATED(b)    ((b)&BADGE_UPDATED_MASK)

#define BADGE_SET_REWARD(b)    ((b)|=BADGE_REWARD_MASK)
#define BADGE_CLEAR_REWARD(b)  ((b)&=~BADGE_REWARD_MASK)
#define BADGE_IS_REWARDING(b)  ((b)&BADGE_REWARD_MASK)

#define BADGE_SET_COMPLETION(b, v) ((b)|=(BADGE_COMPLETION_MASK&(v)))
#define BADGE_CLEAR_COMPLETION(b)  ((b)&=~BADGE_COMPLETION_MASK)
#define BADGE_COMPLETION(b)        ((b)&BADGE_COMPLETION_MASK)

void load_Badges(SHARED_MEMORY_PARAM BadgeDefs *p, const char *pchFilename, int* bNewAttribs, const char *badgesAttribFilename, int idxHardMax);

//pop help "badges"
// each tutorial/pop-up help window has four bits of state- didn't happen, unread, read, hidden, and dismissed. (three unused possibilities)
#define MAX_POP_HELP 1024
#define BITS_PER_POP_HELP 4
//The number of U32s need to store the pop help status, 64 u32s, 256 bytes
#define POP_HELP_STATUS_SIZE (MAX_POP_HELP*BITS_PER_POP_HELP/(sizeof(U32)*8))
//The size of the pop help "history"
#define POP_HELP_LATEST_SIZE 5


#endif /* #ifndef BADGES_H__ */

/* End of File */
