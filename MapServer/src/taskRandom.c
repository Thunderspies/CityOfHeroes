#include <utilitieslib/utils/textparser.h>
#include "gameComm/VillainDef.h"
#include "storyarc/missionset.h"
#include "taskRandom.h"
#include <utilitieslib/components/earray.h>
#include "time.h"
#include "entity/entity.h"
#include "entity/character_base.h"
#include "storyarc/storyarcprivate.h"
#include "entity/character_level.h"
#include "storyarc/missionMapCommon.h"
// *********************************************************************************
//  Parse definitions
// *********************************************************************************

StaticDefineInt ParseVillainType[] =
{
    DEFINE_INT
    {"Any",        VGT_ANY},
    {"Arcane",    VGT_ARCANE},
    {"Tech",    VGT_TECH},
    DEFINE_END
};

typedef struct GroupSelection {
    VillainGroupEnum villainGroup;                //ID of the villain group
    int minLevel;                    //minimum level a player can be and see this villain group
    int maxLevel;                    //maximum level a player can be and see this villain group
    VillainGroupType villainType;    //arcane, tech, etc...
    char * bossName;                //possible names of bosses of this villain group
    SCRIPTVARS_STD_FIELD
} GroupSelection;

typedef struct GroupSelectionList {
    GroupSelection ** selections;
} GroupSelectionList;

GroupSelectionList g_GroupSelectionList;
GroupSelectionList g_villainGroupSelectionList;

TokenizerParseInfo ParseGroupSelection[] = {
    {"{",                TOK_START,    0},
    {"VillainGroup",    TOK_INT(GroupSelection,villainGroup,0),    ParseVillainGroupEnum},
    {"VillainType",        TOK_INT(GroupSelection,villainType,0),    ParseVillainType},
    {"MinLevel",        TOK_INT(GroupSelection,minLevel,0)},
    {"MaxLevel",        TOK_INT(GroupSelection,maxLevel,0)},
    SCRIPTVARS_STD_PARSE(GroupSelection)
    {"}",                TOK_END,        0},
    {"",0,0}
};

TokenizerParseInfo ParseGroupSelectionList[] = {
    {"GroupSelection",    TOK_STRUCT(GroupSelectionList,selections,ParseGroupSelection) },
    {"",0,0}
};


typedef struct MapSetSelection {
    int *        villainGroups;    //IDs of possible villain groups
    int            minLevel;        //minimum level a player can be and see this map
    int            maxLevel;        //maximum level a player can be and see this map
    MapSetEnum    mapSet;            //type of map (caves,office,etc...)
    int            doorType;        //type of door that will lead to this map
    int            mapType;        //kind of map, i don't know how this differs from mapset
    SCRIPTVARS_STD_FIELD
} MapSetSelection;

typedef struct MapSetSelectionList {
    MapSetSelection ** selections;
} MapSetSelectionList;

MapSetSelectionList g_MapSetSelectionList;
MapSetSelectionList g_villainMapSetSelectionList;

TokenizerParseInfo ParseMapSelection[] = {
    {"{",                TOK_START,    0},
    {"VillainGroups",    TOK_INTARRAY(MapSetSelection,villainGroups),    ParseVillainGroupEnum},
    {"MinLevel",        TOK_INT(MapSetSelection,minLevel,0)},
    {"MaxLevel",        TOK_INT(MapSetSelection,maxLevel,0)},
    {"MapSet",            TOK_INT(MapSetSelection,mapSet,0),    ParseMapSetEnum},
    SCRIPTVARS_STD_PARSE(MapSetSelection)
    {"}",                TOK_END,        0},
    {"",0,0}
};

TokenizerParseInfo ParseMapSelectionList[] = {
    {"MapSelection",    TOK_STRUCT(MapSetSelectionList,selections,ParseMapSelection) },
    {"",0,0}
};

void loadRandomDefFiles() {
    // Load the villain version of the random group selection
    ParserLoadFiles(0,"defs/generic/villainrandomgroupselection.def","villainrandomgroupselection.bin",PARSER_SERVERONLY,
        ParseGroupSelectionList,&g_villainGroupSelectionList,NULL,NULL,NULL);
    ParserLoadFiles(0,"defs/generic/villainrandommapselection.def","villainrandommapselection.bin",PARSER_SERVERONLY,
        ParseMapSelectionList,&g_villainMapSetSelectionList,NULL,NULL,NULL);

    // Load the hero version of the random group selection
    ParserLoadFiles(0,"defs/generic/randomgroupselection.def","randomgroupselection.bin",PARSER_SERVERONLY,
        ParseGroupSelectionList,&g_GroupSelectionList,NULL,NULL,NULL);
    ParserLoadFiles(0,"defs/generic/randommapselection.def","randommapselection.bin",PARSER_SERVERONLY,
        ParseMapSelectionList,&g_MapSetSelectionList,NULL,NULL,NULL);
}

// Gets a group selection list depending on the player
static GroupSelectionList getGroupSelectionList(const Entity * player)
{
    if (ENT_IS_ON_RED_SIDE(player))
        return g_villainGroupSelectionList;
    else
        return g_GroupSelectionList;
}

// Gets a map selection list depending on the player
static MapSetSelectionList getMapSetSelectionList(const Entity * player)
{
    if (ENT_IS_ON_RED_SIDE(player))
        return g_villainMapSetSelectionList;
    else
        return g_MapSetSelectionList;
}

//gets allowable villain groups for a given level and villaingrouptype
//level==-1 will allow all levels
static void getPossibleVillainGroups(const Entity * player,GroupSelection *** groups,int level,VillainGroupType type) {
    int i;
    GroupSelectionList selectionList = getGroupSelectionList(player);
    for (i=0;i<eaSize(&selectionList.selections);i++) {
        GroupSelection * gs=selectionList.selections[i];
        if ((level==-1 || (level>=gs->minLevel && level<=gs->maxLevel)) && 
            (type==VGT_ANY || gs->villainType==VGT_ANY || type==gs->villainType) )
            eaPush(groups,gs);
    }
}

static void getPossibleMapSets(const Entity * player,MapSetSelection *** maps,int level) {
    int i;
    MapSetSelectionList selectionList = getMapSetSelectionList(player);
    for (i=0;i<eaSize(&selectionList.selections);i++) {
        MapSetSelection * ms=selectionList.selections[i];
        if (level>=ms->minLevel && level<=ms->maxLevel)
            eaPush(maps,ms);
    }
}

static void getPossibleMapSetsGivenVillainGroup(const Entity * player,MapSetSelection *** maps,int level,VillainGroupEnum vge) {
    int i;
    MapSetSelectionList selectionList = getMapSetSelectionList(player);
    for (i=0;i<eaSize(&selectionList.selections);i++) {
        MapSetSelection * ms=selectionList.selections[i];
        int j;
        for (j=0;j<eaiSize(&ms->villainGroups);j++) {
            if (ms->villainGroups[j]==vge && level>=ms->minLevel && level<=ms->maxLevel) {
                eaPush(maps,ms);
                break;
            }
        }
    }
}

//  calling this function resets the saSeed
static void getRandomVillainGroup(const Entity * player,VillainGroupEnum * vge,int level,VillainGroupType type,int seed) {
    GroupSelection ** groups=0;
    if (vge==NULL)
        return;
    eaCreate(&groups);
    getPossibleVillainGroups(player,&groups,level,type);
    if (eaSize(&groups)==0)
        return;
    *vge=groups[saSeedRoll(seed,eaSize(&groups))]->villainGroup,seed;
    eaDestroy(&groups);
}

//  calling this function resets the saSeed
static void getRandomMapSet(const Entity * player,MapSetEnum * mse,char ** filename,int level,int seed) {
    MapSetSelection ** maps=0;
    int roll;
    if (mse==NULL && filename==NULL)
        return;
    eaCreate(&maps);
    getPossibleMapSets(player,&maps,level);
    if (eaSize(&maps)==0)
        return;
    roll=saSeedRoll(seed,eaSize(&maps));
    if (mse!=NULL)
        *mse=maps[roll]->mapSet;
    eaDestroy(&maps);
}

//  calling this function resets the saSeed
void getRandomMapSetForVillainGroup(const Entity * player,MapSetEnum * mse,char ** filename,int level,VillainGroupEnum vge,int seed) {
    MapSetSelection ** maps=0;
    int roll;
    if (mse==NULL && filename==NULL)
        return;
    eaCreate(&maps);
    getPossibleMapSetsGivenVillainGroup(player,&maps,level,vge);
    if (eaSize(&maps)==0)
        return;
    roll=saSeedRoll(seed,eaSize(&maps));
    if (mse!=NULL)
        *mse=maps[roll]->mapSet;
    eaDestroy(&maps);
}

//  calling this function resets the saSeed
static void getRandomVillainGroupAndMapSet(const Entity * player,VillainGroupEnum * vge,MapSetEnum * mse,char ** filename,int level,VillainGroupType type,int seed) {
    GroupSelection ** groups=0;
    MapSetSelection ** maps=0;
    int numPossible;
    int i;
    eaCreate(&groups);
    getPossibleVillainGroups(player,&groups,level,type);
    eaCreate(&maps);
    getPossibleMapSets(player,&maps,level);
    numPossible=eaSize(&maps);
    saSeed(seed);
    for (i=0;i<eaSize(&maps);i++) {
        int j;
        int good=false;
        for (j=0;j<eaiSize(&maps[i]->villainGroups);j++) {
            int k;
            for (k=0;k<eaSize(&groups);k++) {
                if (maps[i]->villainGroups[j]==groups[k]->villainGroup) {
                    good=true;
                    break;
                }
            }
            if (good)
                break;
        }
        if (!good) {
            maps[i]=0;
            numPossible--;
        }
    }
    if (numPossible==0) {
        //handle the case when there is no overlap of villain groups and maps
    }
    for (i=0;i<eaSize(&maps);i++) {
        if (maps[i]==NULL)
            continue;
        if (saRoll(numPossible)==0) {
            int j;
            int numPossibleGroups=0;
            if (mse!=NULL)
                *mse=maps[i]->mapSet;
            for (j=0;j<eaSize(&groups);j++) {
                int k;
                for (k=0;k<eaiSize(&maps[i]->villainGroups);k++) {
                    if (groups[j]->villainGroup==maps[i]->villainGroups[k])
                        numPossibleGroups++;
                }
            }
            assert(numPossibleGroups);
            for (j=0;j<eaSize(&groups);j++) {
                int k;
                for (k=0;k<eaiSize(&maps[i]->villainGroups);k++) {
                    if (groups[j]->villainGroup==maps[i]->villainGroups[k]){
                        if (saRoll(numPossibleGroups)==0) {
                            if (vge!=NULL)
                                *vge=groups[j]->villainGroup;
                            return;
                        } else
                            numPossibleGroups--;
                    }
                }
            }
        } else 
            numPossible--;
    }
    eaDestroy(&groups);
    eaDestroy(&maps);
}

// Given a particular StoryTask and a seed, what Random fields would show up?
//  calling this function resets the saSeed
void getRandomFields(const Entity * player,VillainGroupEnum * vge,MapSetEnum * mse,char ** filename,int level,const StoryTask * task,int seed) {
    if (getVillainGroupFromTask(task)==villainGroupGetEnum("Random") && 
    getMapSetFromTask(task)==MAPSET_RANDOM) {
        getRandomVillainGroupAndMapSet(player,vge,mse,filename,level,getVillainGroupTypeFromTask(task),seed);
    } else {
        if (getVillainGroupFromTask(task)==villainGroupGetEnum("Random"))
            getRandomVillainGroup(player,vge,level,getVillainGroupTypeFromTask(task),seed);
        if (getMapSetFromTask(task)==MAPSET_RANDOM)
            getRandomMapSet(player,mse,filename,level,seed);
    }
}

// Used only when the task is assigned (as in, inside the player's StoryInfo)
//  - the seed here is already assumed as the processing of the StoryTask into the unassigned StoryTaskInfo should have affixed the values properly
//  - this effectively acts as the authoritative source for the random results once the task was assigned
void randomAddVarsScopeFromStoryTaskInfo(ScriptVarsTable * svt, const Entity * ent, const StoryTaskInfo * sti) {
    if (!sti)
        return;
    randomAddGroupVarsScope(svt,ent,sti->villainGroup ? sti->villainGroup : getVillainGroupFromTask(sti->def));
    randomAddMapVarsScope(svt,ent,sti->mapSet ? sti->mapSet : getMapSetFromTask(sti->def));
}

// Used when displaying unassigned but rolled for Newspaper missions
//  - the roll for villain group has already been made and saved into the pickInfo, but the MapSet is undetermined
void randomAddVarsScopeFromContactTaskPickInfo(ScriptVarsTable * svt, const Entity * ent, const ContactTaskPickInfo * pickInfo) {
    if (!pickInfo)
        return;
    
    if (getVillainGroupFromTask(pickInfo->taskDef)!=villainGroupGetEnum("Random"))
        randomAddGroupVarsScope(svt,ent,pickInfo->villainGroup ? pickInfo->villainGroup : getVillainGroupFromTask(pickInfo->taskDef));
    else
    {
        // not sure this should ever happen
        devassert(pickInfo->villainGroup);
        randomAddGroupVarsScope(svt,ent,pickInfo->villainGroup ? pickInfo->villainGroup : getVillainGroupFromTask(pickInfo->taskDef));
    }

    // mapSet choice not yet determined at pickInfo stage
    randomAddMapVarsScope(svt,ent,getMapSetFromTask(pickInfo->taskDef));
}

// Used when the StoryTaskInfo is unavailable - such as when initing a Mission.
//  - VillainGroup was added to the mission info string directly
void randomAddGroupVarsScope(ScriptVarsTable * svt, const Entity * player, VillainGroupEnum vge) {
    int i;
    GroupSelectionList selectionList = getGroupSelectionList(player);
    if (vge==0)
        return;
    for (i=0;i<eaSize(&selectionList.selections);i++) {
        GroupSelection * gs=selectionList.selections[i];
        if (gs->villainGroup==vge) {
            ScriptVarsTablePushScope(svt,&gs->vs);
            return;
        }
    }
}

static void randomAddMapVarsScope(ScriptVarsTable * svt, const Entity * player, MapSetEnum mse) {
    int i;
    MapSetSelectionList selectionList = getMapSetSelectionList(player);
    if (mse==0)
        return;
    for (i=0;i<eaSize(&selectionList.selections);i++) {
        MapSetSelection * ms=selectionList.selections[i];
        if (ms->mapSet==mse) {
            ScriptVarsTablePushScope(svt,&ms->vs);
            return;
        }
    }
}

int isVillainGroupAcceptableGivenMapSet(const Entity * player,VillainGroupEnum vge,MapSetEnum mse,int level) {
    MapSetSelection ** ms=0;
    int i;
    int ret=0;
    if (mse==MAPSET_RANDOM)
        getPossibleMapSets(player,&ms,level);
    else
    {
        MapSetSelectionList selectionList = getMapSetSelectionList(player);
        ms=selectionList.selections;
    }
    for (i=0;i<eaSize(&ms);i++) {
        int j;
        if (mse==MAPSET_RANDOM || mse==ms[i]->mapSet) {
            for (j=0;j<eaiSize(&ms[i]->villainGroups);j++) {
                if (vge==ms[i]->villainGroups[j]) {
                    ret=1;
                    break;
                }
            }
        }
        if (ret)
            break;
    }
    if (mse==MAPSET_RANDOM)
        eaDestroy(&ms);
    return ret;
}

int isVillainGroupAcceptableGivenLevel(const Entity * player,VillainGroupEnum vge,int level) {
    GroupSelection ** gs=0;
    int i;
    int ret=0;
    getPossibleVillainGroups(player,&gs,level,VGT_ANY);
    for (i=0;i<eaSize(&gs);i++) {
        if (gs[i]->villainGroup==vge) {
            ret=1;
            break;
        }
    }
    eaDestroy(&gs);
    return ret;
}

int isVillainGroupAcceptableGivenVillainGroupType(const Entity * player,VillainGroupEnum vge,VillainGroupType vgt) {
    GroupSelection ** gs=0;
    int i;
    int ret=0;
    getPossibleVillainGroups(player,&gs,-1,vgt);
    for (i=0;i<eaSize(&gs);i++) {
        if (gs[i]->villainGroup==vge) {
            ret=1;
            break;
        }
    }
    eaDestroy(&gs);
    return ret;
}

VillainGroupEnum getVillainGroupFromTask(const StoryTask * task) {
    if (!task)
        return 0;
    if (task->villainGroup)
        return task->villainGroup;
    if (eaSize(&task->missiondef))
        return villainGroupGetEnum(task->missiondef[0]->villaingroup);
    if (task->missionref)
        return villainGroupGetEnum(task->missionref->villaingroup);
    return 0;
}

VillainGroupType getVillainGroupTypeFromTask(const StoryTask * task) {
    if (!task)
        return 0;
    if (task->villainGroupType)
        return task->villainGroupType;
    if (eaSize(&task->missiondef))
        return task->missiondef[0]->villainGroupType;
    if (task->missionref)
        return task->missionref->villainGroupType;
    return 0;
}

MapSetEnum getMapSetFromTask(const StoryTask * task) {
    if (!task)
        return 0;
    if (eaSize(&task->missiondef))
        return task->missiondef[0]->mapset;
    if (task->missionref)
        return task->missionref->mapset;
    return 0;
}

