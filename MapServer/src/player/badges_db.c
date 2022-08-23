/***************************************************************************
 *     Copyright (c) 2004-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include <utilitieslib/components/earray.h>
#include <utilitieslib/components/StringCache.h>
#include <utilitieslib/components/EString.h>
#include "entity/teamCommon.h"
#include <utilitieslib/utils/file.h>

#include "container/dbcontainerpack.h"
#include "entity/entPlayer.h"
#include "entity/entity.h"

#include "player/badges.h"
#include "badges_server.h"
#include "BadgeStats.h"
#include <utilitieslib/components/StashTable.h>
#include "gameData/badgestats_db.h"

#include <utilitieslib/utils/structDefines.h>
#include "gameComm/VillainDef.h"

// --------------------
// player

static LineDesc s_owned[] = {{ PACKTYPE_BIN_STR, BADGE_ENT_BITFIELD_BYTE_SIZE, "Badges[0].Owned", OFFSET(EntPlayer, aiBadgesOwned), }, {0}};

static StructDesc s_badges_desc[] =
{
    sizeof(EntPlayer),
    {AT_NOT_ARRAY,{0}},
    s_owned
};

static char const *g_fixup_group_stats[] = 
{
    "time",
    "time.pvp",
    "xp",
    "xpdebt",
    "xpdebt.removed",
    "prestige",
    "kills",
    "kills.pvp",
    "kills.arena",
    "deaths.pvp",
    "deaths.arena",
    "damage.given",
    "damage.taken",
    "task.complete",
    "task.success",
    "task.failed",
};

static StashTable g_fixup_group_stats_stash = NULL;
bool badge_IsFixupGroupStat(const char *pchStat)
{
    if(!g_fixup_group_stats_stash)
    {
        int i;
        g_fixup_group_stats_stash = stashTableCreateWithStringKeys(ARRAY_SIZE(g_fixup_group_stats)*1.5f, StashDefault);
        for( i = 0; i < ARRAY_SIZE( g_fixup_group_stats ); ++i ) 
        {
            char const *s = g_fixup_group_stats[i];
            stashAddInt(g_fixup_group_stats_stash,s,i,true);
        }
    }
    return stashFindInt(g_fixup_group_stats_stash,pchStat,NULL);
}
/**********************************************************************func*
 * badge_StatNames
 *
 */
char *badge_StatNames(StashTable hashBadgeStatUsage)
{
    StuffBuff sb;
    StashElement elem;
    StashTableIterator iter;
    StashTable dupe_checker = stashTableCreateWithStringKeys(ARRAY_SIZE(g_fixup_group_stats)*1.5f, StashDeepCopyKeys);
    int i;
    char *eTmp = NULL;
    
    initStuffBuff(&sb, 256);
    
    stashGetIterator(hashBadgeStatUsage, &iter);
    while(stashGetNextElement(&iter, &elem))
    {
        const char *pchName = stashElementGetStringKey(elem);
        if(*pchName!='*') // Skip internal names
        {
            addStringToStuffBuff(&sb, "%s\n", pchName);
        }
    }
    
    // ----------
    // stats only get added to the badgestats.attribute file
    // if they are 'used' by a badge requires statement
    // 
    // badge stats are now doing double duty to report information
    // about player behavior. subsequently we want to force
    // some stats to get saved regardless of their use in the
    // badges.
    //
    // there are also stats which we want to take permutations of
    // for example:
    // - g_fixup_group_stats : what size group are you in.
    
    // helper macro
#define ADD_STAT(STATNAME)                                                  \
    if(!stashFindPointer(hashBadgeStatUsage,STATNAME,NULL)                  \
       && !stashFindInt(dupe_checker,STATNAME,NULL))                        \
    {stashAddInt(dupe_checker,STATNAME,1,FALSE);addStringToStuffBuff(&sb,"%s\n",STATNAME);}
    
    // check the group stats.
    for( i = 0; i < ARRAY_SIZE( g_fixup_group_stats ); ++i )
    {
        static const char *tails[] = 
            {
                "group1",
                "group2",
                "group3_4",
                "group5_8",
            };
        char const *stem = g_fixup_group_stats[i];
        int j;
        STATIC_INFUNC_ASSERT(MAX_TEAM_MEMBERS == 8);
        
        // add all the base stat and all permutations
        ADD_STAT(stem);
        for( j = 0; j < ARRAY_SIZE( tails ); ++j ) 
        {
            char const *tail = tails[j];
            char const *s;
            estrPrintf(&eTmp,"%s.%s",stem,tail);
            s = allocAddString(eTmp); 
            ADD_STAT(s);
        }
    }

    // for each villain rank
    ADD_STAT("kills");
    for( i = 0; i < VR_MAX; ++i )
    {
        const char *rank = StaticDefineIntRevLookup(ParseVillainRankEnum, i);
        if(rank)
        {
            estrPrintCharString(&eTmp,"kills.");
            estrConcatCharString(&eTmp,rank);
            ADD_STAT(eTmp);    //does this work?  Seems like you're adding the same estring to the hash over and over...
        }
    }

    // ----------
    // cleanup

    estrDestroy( &eTmp );
    stashTableDestroy(dupe_checker);
    
    // This function is only called in -template mode, so we're letting
    //    this allocation dangle. (Same thing is done for attribs.)
    return sb.buff;
}

/**********************************************************************func*
 * badge_Names
 *
 */
char *badge_Names(const BadgeDefs *badgeDefs)
{
    StuffBuff sb = {0};
    
    if( verify( badgeDefs ))
    {
        
        int i;
        
        int n = eaSize(&badgeDefs->ppBadges);
        
        initStuffBuff(&sb, 256);
        
        for(i=0; i<n; i++)
        {
            addStringToStuffBuff(&sb, "%s\n", badgeDefs->ppBadges[i]->pchName);
        }
    }
    
    // This function is only called in -template mode, so we're letting
    //    this allocation dangle. (Same thing is done for attribs.)
    return sb.buff;
}

/**********************************************************************func*
 * badge_Template
 *
 */
void badge_Template(StuffBuff *psb) 
{
    structToLineTemplate(psb,0,s_owned,1);
    badgestats_Subtable(BADGESTATSTYPE_ENTITY,psb);
}

/**********************************************************************func*
 * badge_UnpackOwned
 *
 */
void badge_UnpackOwned(Entity *e, char *val)
{
    lineToStruct((char *)e->pl,s_badges_desc,s_owned[0].name,"Owned",val,0);
}

/**********************************************************************func*
 * badge_UnpackLine
 *
 */
void badge_UnpackLine(Entity *e, char *table, char *field, char *val, int idx)
{
    if(stricmp(field, "Owned")==0)
    {
        // HACK
        badge_UnpackOwned(e, val);
    }
    else
    {
        int i = badgestats_GetIndex(BADGESTATSTYPE_ENTITY,table,field);
        if(i > 0 && i <= g_iMaxBadgeStatIdx)
            e->pl->aiBadgeStats[i] = atoi(val);
        // if(i < 0), then table and field aren't a badgestat field
    }
}

/**********************************************************************func*
 * badge_PackageOwned
 *
 */

void badge_PackageOwned(Entity *e, StuffBuff *psb)
{
    structToLine(psb,(char *)e->pl,0,s_badges_desc,&s_owned[0],0);
}

/**********************************************************************func*
 * badge_Package
 *
 */
void badge_Package(Entity *e, StuffBuff *psb)
{
    int i;

    // Write out Owned
    badge_PackageOwned(e,psb);

    for(i=1; i <= g_iMaxBadgeStatIdx; i++)
        addStringToStuffBuff(psb, "%s %d\n", badgestats_ColumnString(BADGESTATSTYPE_ENTITY,i), e->pl->aiBadgeStats[i]);
}


/* End of File */
