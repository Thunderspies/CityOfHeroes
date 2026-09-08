/***************************************************************************
 *     Copyright (c) 2004-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include <utilitieslib/utils/error.h>
#include <utilitieslib/utils/textparser.h>
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/eval.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/components/StashTable.h>
#include <utilitieslib/language/MessageStore.h>
#include <utilitieslib/components/bitfield.h>
#include "entity/entity.h"    
#include "entity/entPlayer.h"
#include "entity/origins.h"
#include "entity/classes.h"
#include "entity/character_base.h"
#include "player/badges.h"
#include <utilitieslib/utils/file.h>
#include <utilitieslib/utils/mathutil.h>
#include "entity/LoadDefCommon.h"
#include "language/commonLangUtil.h"
#include "entity/Supergroup.h"

#if SERVER || STATSERVER
#include "Reward.h"
#endif

#define BADGE_MONITOR_IDX_SGRP_FLAG        0x00010000

StaticDefineInt CollectionTypeEnum[] =
{
    DEFINE_INT
    { "kBadge",                    kCollectionType_Badge                },
    { "kMarket",                kCollectionType_Market                },
    { "kSuperPack",                kCollectionType_SuperPack            },
    { "kIncarnate",                kCollectionType_Incarnate            },
    { "kSupergroup",            kCollectionType_Supergroup            },
    DEFINE_END
};

StaticDefineInt BadgeTypeEnum[] =
{
    DEFINE_INT
    { "kNone",                    kBadgeType_None           },
    { "kInternal",                kBadgeType_Internal       },
    { "kTourism",                kBadgeType_Tourism        },
    { "kHistory",                kBadgeType_History        },
    { "kAccomplishment",        kBadgeType_Accomplishment },
    { "kAchievement",            kBadgeType_Achievement    },
    { "kPerk",                    kBadgeType_Perk           },
    { "kGladiator",                kBadgeType_Gladiator      },
    { "kVeteran",                kBadgeType_Veteran        },

    { "kPVP",                    kBadgeType_PVP            },
    { "kInvention",                kBadgeType_Invention    },
    { "kDefeat",                kBadgeType_Defeat        },
    { "kEvent",                    kBadgeType_Event        },
    { "kFlashback",                kBadgeType_Flashback    },
    { "kAuction",                kBadgeType_Auction        },
    { "kDayJob",                kBadgeType_DayJob        },
    { "kArchitect",                kBadgeType_Architect    },

    { "kContent",                kMarketType_Content    },
    { "kSignatureStoryArc1",    kMarketType_SignatureStoryArc1    },
    { "kSignatureStoryArc2",    kMarketType_SignatureStoryArc2    },

    { "kHeroesAndVillains",        kSuperPackType_HeroesAndVillains    },
    { "kRoguesAndVigilantes",    kSuperPackType_RoguesAndVigilantes    },

    { "kEmpyrean",                kIncarnateType_Empyrean    },
    { "kAstral",                kIncarnateType_Astral    },

    DEFINE_END
};

TokenizerParseInfo ParseBadgeDef[] = {
    { "{",                    TOK_START,                                        0},
    { "",                    TOK_CURRENTFILE(BadgeDef, filename)                },
    { "Name",                TOK_STRUCTPARAM | TOK_STRING(BadgeDef, pchName,0)                },
    { "Index",                TOK_INT(BadgeDef, iIdx,0)                        },
    { "Collection",            TOK_INT(BadgeDef, eCollection, kCollectionType_Badge), CollectionTypeEnum    },
    { "Category",            TOK_INT(BadgeDef, eCategory, kBadgeType_None), BadgeTypeEnum    },
    { "SteamExport",        TOK_STRING(BadgeDef, steamExport,0)                },
    { "DisplayHint",        TOK_STRING(BadgeDef, pchDisplayHint[0],0)        },
    { "DisplayText",        TOK_STRING(BadgeDef, pchDisplayText[0],0)        },
    { "DisplayTitle",        TOK_STRING(BadgeDef, pchDisplayTitle[0],0)        },
    { "Icon",                TOK_STRING(BadgeDef, pchIcon[0],0)                },
    { "DisplayHintVillain",    TOK_STRING(BadgeDef, pchDisplayHint[1],0)        },
    { "DisplayTextVillain",    TOK_STRING(BadgeDef, pchDisplayText[1],0)        },
    { "DisplayTitleVillain",    TOK_STRING(BadgeDef, pchDisplayTitle[1],0)    },
    { "VillainIcon",        TOK_STRING(BadgeDef, pchIcon[1],0)                },
    { "Reward",                TOK_STRINGARRAY(BadgeDef, ppchReward)            },
    { "Visible",            TOK_STRINGARRAY(BadgeDef, ppchVisible)            },
    { "Hinted",                TOK_STRINGARRAY(BadgeDef, ppchKnown)            },
    { "Known",                TOK_REDUNDANTNAME|TOK_STRINGARRAY(BadgeDef, ppchKnown)            },
    { "Requires",            TOK_STRINGARRAY(BadgeDef, ppchRequires)            },
    { "Meter",                TOK_STRINGARRAY(BadgeDef, ppchMeter)            },
    { "Revoke",                TOK_STRINGARRAY(BadgeDef, ppchRevoke)            },
    { "DisplayButton",        TOK_STRING(BadgeDef, pchDisplayButton,0)        },
    { "ButtonReward",        TOK_STRINGARRAY(BadgeDef, ppchButtonReward)        },
    { "DoNotCount",            TOK_BOOL(BadgeDef, bDoNotCount, false)            },
    { "DisplayPopup",        TOK_STRING(BadgeDef, pchDisplayPopup, 0)        },
    { "AwardText",            TOK_STRING(BadgeDef, pchAwardText, 0)            },
    { "AwardTextColor",        TOK_RGBA(BadgeDef, rgbaAwardText)                },
    { "Contacts",            TOK_STRINGARRAY(BadgeDef, ppchContacts)            },
    { "BadgeValues",        TOK_INT(BadgeDef, iProgressMaxValue, 0)            },
    { "}",                    TOK_END,                                        0},
    { "", 0, 0 }
};

TokenizerParseInfo ParseBadgeDefs[] = {
    { "Badge",    TOK_STRUCT(BadgeDefs, ppBadges, ParseBadgeDef) },
    { "", 0, 0 }
};

typedef struct BadgeTabSetup
{
    CollectionType collect;
    BadgeType type;
    char *name[2];            // 0 for heroes, 1 for villains
} BadgeTabSetup;

// If the villain string is left empty the hero string will be used for
// both sides.
static BadgeTabSetup s_TabSetup[] =
{

    { kCollectionType_Badge, -1, "MostRecentString" },
    { kCollectionType_Badge, -2, "NearCompletionString" },

    { kCollectionType_Badge, kBadgeType_Tourism, "TourismString" },
    { kCollectionType_Badge, kBadgeType_History, "HistoryString" },
    { kCollectionType_Badge, kBadgeType_Accomplishment, "AccomplishmentString" },
    { kCollectionType_Badge, kBadgeType_Achievement, "AchievementString" },
    { kCollectionType_Badge, kBadgeType_Perk, "AccoladeString" },
    { kCollectionType_Badge, kBadgeType_Gladiator, "GladiatorString" },
    { kCollectionType_Badge, kBadgeType_Veteran, "VeteranString" },
    { kCollectionType_Badge, kBadgeType_PVP, "PVPString" },
    { kCollectionType_Badge, kBadgeType_Invention, "InventionString" },
    { kCollectionType_Badge, kBadgeType_Defeat, "DefeatString" },
    { kCollectionType_Badge, kBadgeType_Event, "EventString" },
    { kCollectionType_Badge, kBadgeType_Flashback, "FlashbackString" },
    { kCollectionType_Badge, kBadgeType_Auction, "WentworthsString", "BlackMarketString" },
    { kCollectionType_Badge, kBadgeType_DayJob, "DayJobString" },
    { kCollectionType_Badge, kBadgeType_Architect, "ArchitectString" },

    { kCollectionType_Market, kMarketType_Content, "CollectionMarketContentString" },
    { kCollectionType_Market, kMarketType_SignatureStoryArc1, "CollectionMarketSSA1String" },
    { kCollectionType_Market, kMarketType_SignatureStoryArc2, "CollectionMarketSSA2String" },

    { kCollectionType_SuperPack, kSuperPackType_HeroesAndVillains, "CollectionSuperPackHeroesAndVillainsString" },
    { kCollectionType_SuperPack, kSuperPackType_RoguesAndVigilantes, "CollectionSuperPackRoguesAndVigilantesString" },

    { kCollectionType_Incarnate, kIncarnateType_Empyrean, "CollectionIncarnateEmpyreanString" },
    { kCollectionType_Incarnate, kIncarnateType_Astral, "CollectionIncarnateAstralString" },

    { kCollectionType_Supergroup, kBadgeType_Tourism, "TourismString" },
    { kCollectionType_Supergroup, kBadgeType_History, "HistoryString" },
    { kCollectionType_Supergroup, kBadgeType_Accomplishment, "AccomplishmentString" },
    { kCollectionType_Supergroup, kBadgeType_Achievement, "AchievementString" },
    { kCollectionType_Supergroup, kBadgeType_Perk, "AccoladeString" },
    { kCollectionType_Supergroup, kBadgeType_Gladiator, "GladiatorString" },
    { kCollectionType_Supergroup, kBadgeType_Veteran, "VeteranString" },
    { kCollectionType_Supergroup, kBadgeType_PVP, "PVPString" },
    { kCollectionType_Supergroup, kBadgeType_Invention, "InventionString" },
    { kCollectionType_Supergroup, kBadgeType_Defeat, "DefeatString" },
    { kCollectionType_Supergroup, kBadgeType_Event, "EventString" },
    { kCollectionType_Supergroup, kBadgeType_Flashback, "FlashbackString" },
    { kCollectionType_Supergroup, kBadgeType_Auction, "WentworthsString", "BlackMarketString" },
    { kCollectionType_Supergroup, kBadgeType_DayJob, "DayJobString" },
    { kCollectionType_Supergroup, kBadgeType_Architect, "ArchitectString" },


    { 0, 0, NULL, NULL }
};

static char *collectionNames[kCollectionType_Count] =
{
    "CollectionBadgeString",
    "CollectionMarketString",
    "CollectionSuperPackString",
    "CollectionIncarnateString",
    "CollectionSupergroupString",
};


SHARED_MEMORY BadgeDefs g_BadgeDefs;
SHARED_MEMORY BadgeDefs g_SgroupBadges;

/**********************************************************************func*
 * badge_LoadNames
 *
 */
int badge_LoadNames(StashTable hashNames, const char *fileName)
{
    int max_idx=0;
    
    if( verify( fileName ))
    {
        char *s,*mem,*args[10];
        int idx,count;
        
        mem = fileAlloc(fileName, 0);
        
        if(!mem)
        {
            printf("ERROR: Can't open %s\n", fileName);
            return 0;
        }
        
        s = mem;
        while(s && (count=tokenize_line(s,args,&s)))
        {
            if (count != 2)
                continue;
            idx = atoi(args[0]);
            if(idx>max_idx)
                max_idx=idx;
            
            stashAddInt(hashNames, args[1], idx, false);
        }
        free(mem);
    }
    
    return max_idx;
}

static const char * s_FinalBadgesFilename;
static int s_FinalBadgesIdxHardMax;

/**********************************************************************func*
 * badge_CreateHashes
 *
 */
static bool badge_FinalProcess(ParseTable* pti, void* structptr, bool shared_memory)
{
    BadgeDefs * pdefs = (BadgeDefs *)structptr;
    bool ret = true;
    int i;
    int n = eaSize(&pdefs->ppBadges);

    assert(!pdefs->hashByIdx);
    pdefs->hashByIdx = stashTableCreate(stashOptimalSize(n), stashShared(shared_memory), StashKeyTypeInts, sizeof(int));

    assert(!pdefs->hashByName);
    pdefs->hashByName = stashTableCreateWithStringKeys(stashOptimalSize(n), stashShared(shared_memory));

    for(i=0; i<n; i++)
    {
        BadgeDef *pBadge = (BadgeDef*)pdefs->ppBadges[i];

        if(pBadge->iIdx < s_FinalBadgesIdxHardMax)
        {
            stashAddPointerConst(pdefs->hashByName, pBadge->pchName, pBadge, false);
            stashIntAddPointerConst(pdefs->hashByIdx, pBadge->iIdx, pBadge, false);
#if SERVER
            // NOTE: both sgrp and ent badges go in here. its a reward pool
            // not that its ever used for anything as far as I can tell.
            rewardAddBadgeToHashTable(pBadge->pchName, s_FinalBadgesFilename);
#endif

            // track the largest index
            pdefs->idxMax = MAX( pdefs->idxMax, pBadge->iIdx );
        }
        else
        {
            static bool once = false;
            if( !once )
            {
                ErrorFilenamef(s_FinalBadgesFilename, "max id %d exceeded by %s. Bug a programmer.", s_FinalBadgesIdxHardMax, pBadge->pchName);
                once = true;
                ret = false;
            }
        }

    }

    return ret;
}

char *badge_CategoryGetName(Entity *e, CollectionType collection, BadgeType category)
{
    char *retval = NULL;
    U32 count = 0;

    while (!retval && s_TabSetup[count].type != 0)
    {
        if (s_TabSetup[count].type == category && s_TabSetup[count].collect == collection)
        {
            retval = s_TabSetup[count].name[0];

            // only use a villain-specific name for the category if there
            // is one provided
            if (ENT_IS_VILLAIN(e) && s_TabSetup[count].name[1])
            {
                retval = s_TabSetup[count].name[1];
            }
        }

        count++;
    }

    return retval;
}

char *badge_CollectionGetName(CollectionType collection)
{
    if (collection >= kCollectionType_Badge && collection < kCollectionType_Count )
        return collectionNames[collection];
    else 
        return "Unknown";
}

/**********************************************************************func*
 * badge_GetIdxFromName
 *
 */
bool badge_GetIdxFromName(const char *pch, int *pi)
{
    const BadgeDef *pdef;
    if(stashFindPointerConst(g_BadgeDefs.hashByName, pch, &pdef))
    {
        *pi = pdef->iIdx;
        return true;
    }

    return false;
}

/**********************************************************************func*
 * badge_GetBadgeByName
 *
 */
const BadgeDef *badge_GetBadgeByName(const char *pch)
{
    const BadgeDef *pdef;
    if(stashFindPointerConst(g_BadgeDefs.hashByName, pch, &pdef))
    {
        return pdef;
    }

    return NULL;
}

/***********************************************************************
* badge_getProgressString
***********************************************************************/
const char* badge_getProgressString(char* buffer, const Entity *entity, const BadgeDef* badge)
{
    assert(buffer);
    assert(entity);
    assert(badge);
    if (!buffer || !entity || !badge)
    {
        return buffer;
    }

    // U32* badgeFields;
    int isSG = badge->iIdx & BADGE_MONITOR_IDX_SGRP_FLAG;
    U32* badgeFields = isSG ? (entity->supergroup->badgeStates.eaiStates) : (entity->pl->aiBadges);
    int realBadgeIdx = badge->iIdx;
    if (isSG)
    {
        realBadgeIdx &= ~BADGE_MONITOR_IDX_SGRP_FLAG;
    }
    int completedValue = BADGE_COMPLETION(badgeFields[realBadgeIdx]); // value is in millionths

    if (badge->iProgressMaxValue > 0)
    {
        int value = (int)(0.5f + (((float)(completedValue) * (float)(badge->iProgressMaxValue)) / 1000000.0f));
        int pct = completedValue / 10000.0f;
        int maxValue = badge->iProgressMaxValue;

        // 35/70 (50%)
        sprintf_s(buffer, BADGE_PROGRESS_STRING_BUFFER_SIZE, "%d/%d (%d%%)", value, maxValue, pct);
    }
    else
    {
        // 0%
        int pct = completedValue / 10000.0f;
        sprintf_s(buffer, BADGE_PROGRESS_STRING_BUFFER_SIZE, "%d%%", pct);
    }

    return buffer;
}

/***********************************************************************
* badge_GetAnyBadgeByIdx
***********************************************************************/
const BadgeDef* badge_GetAnyBadgeByIdx(int iIdx)
{
    if (iIdx & BADGE_MONITOR_IDX_SGRP_FLAG)
    {
        int realIdx = iIdx & (!BADGE_MONITOR_IDX_SGRP_FLAG);
        return badge_GetSgroupBadgeByIdx(realIdx);
    }
    return badge_GetBadgeByIdx(iIdx);
}


/**********************************************************************func*
* badge_GetBadgeByIdx
*
*/
const BadgeDef *badge_GetBadgeByIdx(int iIdx)
{
    const BadgeDef* pResult;

    if ( g_BadgeDefs.hashByIdx && stashIntFindPointerConst(g_BadgeDefs.hashByIdx, iIdx, &pResult))
        return pResult;

    return NULL;
}

/**********************************************************************func*
* badge_GetSgroupBadgeByIdx
*
*/
const BadgeDef *badge_GetSgroupBadgeByIdx(int iIdx)
{
    const BadgeDef* pResult;

    if ( stashIntFindPointerConst(g_SgroupBadges.hashByIdx, iIdx, &pResult))
        return pResult;

    return NULL;
}



#if (CLIENT | SERVER) && !defined(TEST_CLIENT)
static char * setTitleInternal( Entity * e, const BadgeDef * badge )
{
    char * retval = NULL;
    int iBadgeField;

    if( badge && badge->eCollection == kCollectionType_Badge)
    {
        iBadgeField = e->pl->aiBadges[badge->iIdx];

        if( BADGE_IS_OWNED(iBadgeField) )
        {
            e->pl->titleBadge = badge->iIdx;
            return printLocalizedEnt(badge->pchDisplayTitle[ENT_IS_VILLAIN(e)?1:0], e);
        }
    }
    else
        e->pl->titleBadge = 0;

    return retval;
}

char * badge_setTitle( Entity * e, int idx )
{
    char * retval = NULL;

    if( idx >= 0 && idx < eaSize(&g_BadgeDefs.ppBadges) )
    {
        retval = setTitleInternal( e, g_BadgeDefs.ppBadges[idx] );
    }

    return retval;
}

char * badge_setTitleId( Entity * e, int idx )
{
    const BadgeDef *badge = badge_GetBadgeByIdx( idx );

    return setTitleInternal( e, badge );
}
#endif

/**********************************************************************func*
* badge_FindName
*
*/
bool badge_FindName(const char * pchName)
{
    return stashFindElementConst(g_BadgeDefs.hashByName, pchName, NULL);
}


//------------------------------------------------------------
// 
//----------------------------------------------------------
const BadgeDef *badgedefs_DefFromName(const BadgeDefs *defs, const char *name)
{    
    const BadgeDef *pdef = NULL;
    stashFindPointerConst(defs->hashByName, name, &pdef);
    return pdef;
}


/**********************************************************************func*
 * MarkModifiedBadges
 *
 */
void MarkModifiedBadges(StashTable hashBadgeStatUsage, U32 aiBadges[], const char *pchStat)
{
    int *piBadges;

    if( verify( aiBadges ))
    {
        if(stashFindInt(hashBadgeStatUsage, pchStat, (int *)&piBadges))
        {
            int n = eaiSize(&piBadges);
            int i;
            for(i=0; i<n; i++)
            {
                BADGE_CLEAR_UPDATED(aiBadges[piBadges[i]]);
            }
        }
    }    
}

void badge_ClearUpdatedAll(Entity *e)
{
    if( e && e->pl )
    {
        int i;
        int n = eaSize(&g_BadgeDefs.ppBadges);
        
        ZeroArray(e->pl->aiBadges);
        for(i=0; i<n; i++)
        {
            int iBadgeIdx = g_BadgeDefs.ppBadges[i]->iIdx;
            if(AINRANGE(iBadgeIdx,e->pl->aiBadges))
                BADGE_SET_CHANGED(e->pl->aiBadges[iBadgeIdx]);
        }
    }
}

// ------------------------------------------------------------
// load defs

static const char *s_PruneBadgesAttribFilename = NULL;

/**********************************************************************func*
 * load_PruneBadges
 *
 */
static bool load_PruneBadges(ParseTable* pti, void* structptr)
{
    BadgeDefs * pdefs = (BadgeDefs *)structptr;
    StashTable hashBadgeNames = stashTableCreateWithStringKeys(128, StashDeepCopyKeys);
    int maxIdx = badge_LoadNames(hashBadgeNames, s_PruneBadgesAttribFilename);

    if(pti==ParseBadgeDefs)
    {
        int i;
        int n = eaSize(&pdefs->ppBadges);

        // --------------------
        // process the badges

        for(i=0; i<n; i++)
        {
            BadgeDef *pBadge = (BadgeDef*)pdefs->ppBadges[i];

            // This puts the official index of the badge into the data
            //   structure. This structure is the saved to a bin file,
            //   which is the only thing a real client has. (This code
            //   shouldn't be called on a real client.)
            if( !stashFindInt(hashBadgeNames, pBadge->pchName, &pBadge->iIdx) )
            {
                pBadge->iIdx = ++maxIdx;
            }    

            if(!pBadge->pchIcon[1] && pBadge->pchIcon[0])
                pBadge->pchIcon[1] = ParserAllocString(pBadge->pchIcon[0]);

            if(!pBadge->pchDisplayTitle[1] && pBadge->pchDisplayTitle[0])
                pBadge->pchDisplayTitle[1] = ParserAllocString(pBadge->pchDisplayTitle[0]);

#if SERVER
            if(pBadge->pchDisplayHint[0])
            {
                ParserFreeString(cpp_const_cast(char*)(pBadge->pchDisplayHint[0]));
                pBadge->pchDisplayHint[0] = NULL;
            }
            if(pBadge->pchDisplayHint[1])
            {
                ParserFreeString(cpp_const_cast(char*)(pBadge->pchDisplayHint[1]));
                pBadge->pchDisplayHint[1] = NULL;
            }
            if(pBadge->pchDisplayText[0])
            {
                ParserFreeString(cpp_const_cast(char*)(pBadge->pchDisplayText[0]));
                pBadge->pchDisplayText[0] = NULL;
            }
            if(pBadge->pchDisplayText[1])
            {
                ParserFreeString(cpp_const_cast(char*)(pBadge->pchDisplayText[1]));
                pBadge->pchDisplayText[1] = NULL;
            }
            if(pBadge->pchDisplayButton)
            {
                ParserFreeString(cpp_const_cast(char*)(pBadge->pchDisplayButton));
                pBadge->pchDisplayButton = NULL;
            }
#endif
#if CLIENT
            if(!pBadge->pchDisplayHint[1] && pBadge->pchDisplayHint[0])
                pBadge->pchDisplayHint[1] = ParserAllocString(pBadge->pchDisplayHint[0]);

            if(!pBadge->pchDisplayText[1] && pBadge->pchDisplayText[0])
                pBadge->pchDisplayText[1] = ParserAllocString(pBadge->pchDisplayText[0]);

            // Clip out text describing the badges from the client.
            if(eaSize(&pBadge->ppchReward))
                eaDestroyConst(&pBadge->ppchReward);

            // Clip out all of the "how to get this" info from the client.
            eaDestroyConst(&pBadge->ppchKnown);
            eaDestroyConst(&pBadge->ppchVisible);
            eaDestroyConst(&pBadge->ppchRequires);
            eaDestroyConst(&pBadge->ppchMeter);
            eaDestroyConst(&pBadge->ppchRevoke);

            if(eaSize(&pBadge->ppchButtonReward))
                eaDestroyConst(&pBadge->ppchButtonReward);
            if(pBadge->pchDisplayPopup)
            {
                ParserFreeString(cpp_const_cast(char*)(pBadge->pchDisplayPopup));
                pBadge->pchDisplayPopup = NULL;
            }
            if(pBadge->pchAwardText)
            {
                ParserFreeString(cpp_const_cast(char*)(pBadge->pchAwardText));
                pBadge->pchAwardText = NULL;
            }
            eaDestroyConst(&pBadge->ppchContacts);
#endif
        }
    }

    stashTableDestroy(hashBadgeNames);

    return true;
}



/**********************************************************************func*
 * load_Badges
 *
 */
void load_Badges(SHARED_MEMORY_PARAM BadgeDefs *p, const char *pchFilename, int* bNewAttribs, const char *badgesAttribFilename, int idxHardMax)
{
#if SERVER || STATSERVER
    int flags = PARSER_SERVERONLY;
#else
    int flags = 0;
#endif

    const char *pchBinFilename = MakeBinFilename(pchFilename);

    // indirect parameter to load_pruneBadges (yuck!)
    s_PruneBadgesAttribFilename = badgesAttribFilename;
    s_FinalBadgesFilename = pchFilename;
    s_FinalBadgesIdxHardMax = idxHardMax;
    {
        ParserLoadFilesShared(MakeSharedMemoryName(pchBinFilename),NULL,pchFilename,pchBinFilename,flags,
            ParseBadgeDefs,p,sizeof(*p),NULL,NULL,load_PruneBadges,NULL,badge_FinalProcess);
    }
    s_PruneBadgesAttribFilename = NULL;
    s_FinalBadgesFilename = NULL;
    s_FinalBadgesIdxHardMax = 0;
}

void badgeMonitor_SendInfo( Entity *e, Packet *pak )
{
    int count;

    for( count = 0; count < MAX_BADGE_MONITOR_ENTRIES; count++ )
    {
        pktSendIfSetBitsPack( pak, 8, e->pl->badgeMonitorInfo[count].iIdx );
    }
}

void badgeMonitor_ReceiveInfo( Entity *e, Packet *pak )
{
    int count;

    for( count = 0; count < MAX_BADGE_MONITOR_ENTRIES; count++ )
    {
        e->pl->badgeMonitorInfo[count].iIdx = pktGetIfSetBitsPack( pak, 8 );
    }

    badgeMonitor_CheckAndSortInfo( e );
}

int badgeMonitor_CanInfoBeAdded( Entity *e, int idx, bool is_supergroup )
{
    int retval = 1;
    int count = 0;
    int idx_to_add;
    bool full = true;

    idx_to_add = idx;

    if( is_supergroup )
    {
        idx_to_add |= BADGE_MONITOR_IDX_SGRP_FLAG;
    }

    // Don't rely on the used entries being contiguous or ordered. The badge
    // has to have an empty slot to occupy and not already be in the list.
    while( count < MAX_BADGE_MONITOR_ENTRIES )
    {
        if( e->pl->badgeMonitorInfo[count].iIdx == 0 )
        {
            full = false;
        }
        else if( e->pl->badgeMonitorInfo[count].iIdx == idx_to_add )
        {
            retval = 0;
        }

        count++;
    }

    // Even if this isn't a duplicate badge we can't add it if there's
    // nowhere to store it.
    if( full )
    {
        retval = 0;
    }

    return retval;
}

static bool badgePersonalIsMonitorable( Entity *e, int idx )
{
    bool retval = false;

    // Has to be a badge known to the player but not yet earned.
    if( e && e->pl && idx < BADGE_ENT_MAX_BADGES &&
        !BADGE_IS_OWNED( e->pl->aiBadges[idx] ) &&
        BADGE_IS_VISIBLE( e->pl->aiBadges[idx] ) )
    {
        retval = true;
    }

    return retval;
}

static bool badgeSupergroupIsMonitorable( Entity *e, int idx )
{
    bool retval = false;

    // Has to be a badge known to the SG but not yet earned.
    if( e && e->supergroup &&
        idx < eaiSize( &e->supergroup->badgeStates.eaiStates ) &&
        !BADGE_IS_OWNED( e->supergroup->badgeStates.eaiStates[idx] ) &&
        BADGE_IS_VISIBLE( e->supergroup->badgeStates.eaiStates[idx] ) )
    {
        retval = true;
    }

    return retval;
}

static bool badgeMonitorInfoIsMonitorable( Entity *e, int idx )
{
    bool retval = false;

    if( e && e->pl && idx != 0 )
    {
        if( idx & BADGE_MONITOR_IDX_SGRP_FLAG )
        {
            retval = badgeSupergroupIsMonitorable( e, idx & ~BADGE_MONITOR_IDX_SGRP_FLAG );
        }
        else
        {
            retval = badgePersonalIsMonitorable( e, idx );
        }
    }

    return retval;
}

static int badgeMonitorInfoCompare( const void *left, const void *right, const void *context)
{
    int retval = 0;

    BadgeMonitorInfo *bi_left = (BadgeMonitorInfo *)left;
    BadgeMonitorInfo *bi_right = (BadgeMonitorInfo *)right;

    if( bi_left && bi_right )
    {
        if( bi_left->iIdx == 0 )
        {
            retval = 1;
        }
        else if ( bi_right->iIdx == 0 )
        {
            retval = -1;
        }
        else
        {
            retval = 0;
        }
    }

    return retval;
}

void badgeMonitor_CheckAndSortInfo( Entity *e )
{
    int count;

    if( e && e->pl )
    {
        for( count = 0; count < MAX_BADGE_MONITOR_ENTRIES; count++ )
        {
            if( !badgeMonitorInfoIsMonitorable( e, e->pl->badgeMonitorInfo[count].iIdx ) )
            {
                // Blank out this invalid badge's monitoring info.
                e->pl->badgeMonitorInfo[count].iIdx = 0;
            }
        }

        // Non-zero indices and orders take priority and then sort by order.
        stableSort(e->pl->badgeMonitorInfo, MAX_BADGE_MONITOR_ENTRIES, sizeof(BadgeMonitorInfo), NULL, badgeMonitorInfoCompare);
    }
}

int badgeMonitor_AddInfo( Entity *e, int idx, bool is_supergroup )
{
    int retval = 0;
    const BadgeDef *badge;
    int count;
    int pos;
    int max_order;
    int idx_to_add;

    if( e && e->pl )
    {
        idx_to_add = idx;
        badge = NULL;

        if( is_supergroup )
        {
            idx_to_add |= BADGE_MONITOR_IDX_SGRP_FLAG;

            if( badgeSupergroupIsMonitorable( e, idx_to_add ) )
            {
                badge = badge_GetSgroupBadgeByIdx( idx );
            }
        }
        else
        {
            if( badgePersonalIsMonitorable( e, idx_to_add ) )
            {
                badge = badge_GetBadgeByIdx( idx );
            }
        }

        if( badge )
        {
            pos = -1;
            max_order = 0;

            for( count = 0; count < MAX_BADGE_MONITOR_ENTRIES; count++ )
            {
                // Find the first empty slot in the array of entries.
                if( e->pl->badgeMonitorInfo[count].iIdx == 0 )
                {
                    if( pos == -1 )
                    {
                        pos = count;
                    }
                }
            }

            if( pos >= 0 && pos < MAX_BADGE_MONITOR_ENTRIES )
            {
                e->pl->badgeMonitorInfo[pos].iIdx = idx_to_add;

                retval = 1;
            }
        }
    }

    return retval;
}

int entity_OwnsBadge( Entity *e, const char * badgename )
{
    int idx=0;
    if( e && e->pl && badge_GetIdxFromName( badgename, &idx ) )
        return BADGE_IS_OWNED( e->pl->aiBadges[idx] );
    return 0;
}

/* End of File */
