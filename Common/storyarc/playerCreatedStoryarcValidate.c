

#include "storyarc/playerCreatedStoryarc.h"
#include "storyarc/playerCreatedStoryarcValidate.h"

#include "entity/PCC_Critter.h"
#include "entity/CustomVillainGroup.h"
#include "storyarc/pnpcCommon.h"
#include "gameComm/Npc.h"
#include "storyarc/missionMapCommon.h"
#include "entity/entity.h"
#include "seq/seq.h"
#include "entity/entPlayer.h"
#include <utilitieslib/utils/textparser.h>
#include <utilitieslib/components/StashTable.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/utils/error.h>
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/components/EString.h>
#include <utilitieslib/utils/utils.h>
#include "entity/RewardToken.h"
#include <zlib.h>
#include "entity/character_eval.h"

#ifndef TEST_CLIENT
#include "filter/profanity.h"
#endif
#include <utilitieslib/language/MessageStoreUtil.h>
#include "graphics/imageCapture.h"
#include <utilitieslib/utils/SimpleParser.h>
#include "seq/AnimBitList.h"
#include <utilitieslib/utils/file.h>
#include <utilitieslib/language/AppLocale.h>
#include <utilitieslib/utils/tokenstore.h>
#include "AutoGen/playerCreatedStoryarcValidate_h_ast.h"
#include "AutoGen/playerCreatedStoryarcTypes_h_ast.h"
#include "AutoGen/playerCreatedStoryarcTypes_h_ast.c"
#include "AutoGen/playerCreatedStoryarcValidate_h_ast.c"
#include "LWC_common.h"
#ifdef CLIENT
#include "cmdparse/cmdgame.h"
#include "UI/uiDialog.h"
#endif

#define DEFAULT_ANIMATION "At_Ease"
#define DEFAULT_VILLAIN_GROUP "5thColumn"
#define DEFAULT_CRITTER "5thColumn_Fifth_Mek_Man"
#define DEFAULT_BOSS "5thColumn_Fifth_Wolfpack_Boss"
#define DEFAULT_MAP "maps/Missions/Tech/Tech_60/Tech_60_Layout_01_01.txt"
#define DEFAULT_MAP_SET "Tech"
#define DEFAULT_MAP_TIME 60
#define DEFAULT_OBJECT "Objects_Barrel_Group"
#define DEFAULT_OBJECT_SUFFIX "_MA"

static char *PCC_ReplacementCritters[3][4]    = {    {    "PCCInvalidCritterReplacement",    "Hydra_Man",    "Hydra_Slug_Minion", "PCCInvalidCritterDesc"    },
{    "PCCInvalidCritterReplacement",    "Hydra_Lieutenant_Man",    "Hydra_Slug_Lieutenant", "PCCInvalidCritterDesc"    },
{    "PCCInvalidCritterReplacement",    "Hydra_Boss_Man",    "Hydra_Slug_Boss", "PCCInvalidCritterDesc"}    };
// -----------------------------------------------------------------------------------------------------------------------
// Data Structs
//------------------------------------------------------------------------------------------------------------------------

TokenizerParseInfo ParseMMContact[] = 
{
    { "NPC",            TOK_STRUCTPARAM|TOK_STRING(MMContact,pchNPCDef,0) },
    { "DisplayName",    TOK_STRUCTPARAM|TOK_IGNORE },
    { "\n",                TOK_STRUCTPARAM|TOK_END },
    {0},
};

TokenizerParseInfo ParseMMContactCetegory[] = 
{
    { "",            TOK_STRUCTPARAM|TOK_STRING(MMContactCategory,pchCategory,0) },
    { "Contact",    TOK_STRUCT(MMContactCategory,ppContact, ParseMMContact) },
    { "EndCategory",TOK_STRUCTPARAM|TOK_END },
    {0},
};

TokenizerParseInfo ParseMMContactCategoryList[] = 
{
    { "Category",     TOK_STRUCT(MMContactCategoryList,ppContactCategories,ParseMMContactCetegory) },
    {0},
};

TokenizerParseInfo ParseMMVillainGroup[] = 
{
    { "",        TOK_STRUCTPARAM|TOK_STRING(MMVillainGroup,pchName,0) },
    { "",        TOK_STRUCTPARAM|TOK_INT(MMVillainGroup,minLevel,0) },
    { "",        TOK_STRUCTPARAM|TOK_INT(MMVillainGroup,maxLevel,0) },
    { "",        TOK_STRUCTPARAM|TOK_INT(MMVillainGroup,minGroupLevel,0) },
    { "",        TOK_STRUCTPARAM|TOK_INT(MMVillainGroup,maxGroupLevel,0) },
    { "",        TOK_STRUCTPARAM|TOK_STRING(MMVillainGroup, pchExclude, 0) },
    { "\n",        TOK_STRUCTPARAM|TOK_END },
    {0},
};

TokenizerParseInfo ParseMMVillainGroupList[] =
{
    { "VG", TOK_STRUCT(MMVillainGroupList, group, ParseMMVillainGroup) },
    { "Embrace", TOK_STRINGARRAY(MMVillainGroupList, ppIncluded) },
    { "CantMove", TOK_STRINGARRAY(MMVillainGroupList, ppCantMoveNames) },
    {0},
};


TokenizerParseInfo ParseMMAnim[] = 
{
    { "",    TOK_STRUCTPARAM|TOK_STRING(MMAnim,pchName,0) },
    { "",    TOK_STRUCTPARAM|TOK_STRING(MMAnim,pchDisplayName,0) },
    { "\n",    TOK_STRUCTPARAM|TOK_END },
    {0},
};

TokenizerParseInfo ParseMMAnimList[] =
{
    { "AnimList:", TOK_STRUCT(MMAnimList, anim, ParseMMAnim) },
    {0},
};

TokenizerParseInfo ParseMMObject[] = 
{
    { "",    TOK_STRUCTPARAM|TOK_STRING(MMObject,pchName,0) },
    { "",    TOK_STRUCTPARAM|TOK_STRING(MMObject,pchDisplayName,0) },
    { "\n",    TOK_STRUCTPARAM|TOK_END },
    {0},
};

TokenizerParseInfo ParseMMObjectPlacement[] =
{
    { "",    TOK_STRUCTPARAM|TOK_STRING(MMObjectPlacement,pchName,0) },
    { "",    TOK_STRUCTPARAM|TOK_STRING(MMObjectPlacement,pchDisplayName,0) },
    { "Object:", TOK_STRUCT(MMObjectPlacement, object, ParseMMObject) },
    { "EndLocation",    TOK_END },
    {0},
};

TokenizerParseInfo ParseMMObjectList[] =
{
    { "Location", TOK_STRUCT(MMObjectList, placement, ParseMMObjectPlacement) },
    {0},
};


TokenizerParseInfo ParseMMDestructableObject[] = 
{
    { "",    TOK_STRUCTPARAM|TOK_STRING(MMDestructableObject,pchName,0) },
    { "\n",    TOK_STRUCTPARAM|TOK_END },
    {0},
};

TokenizerParseInfo ParseMMDestructableObjectList[] =
{
    { "Object:", TOK_STRUCT(MMDestructableObjectList, object, ParseMMDestructableObject) },
    {0},
};

TokenizerParseInfo ParseMMMapSet[] = 
{
    { "",    TOK_STRUCTPARAM|TOK_STRING(MMMapSet,pchName,0) },
    { "",    TOK_STRUCTPARAM|TOK_STRING(MMMapSet,pchDisplayName,0) },
    { "\n",    TOK_STRUCTPARAM|TOK_END },
    {0},
};

TokenizerParseInfo ParseMMMapSetList[] =
{
    { "MapSet", TOK_STRUCT(MMMapSetList, mapset, ParseMMMapSet) },
    {0},
};

TokenizerParseInfo ParseMMUniqueMap[] = 
{
    { "",    TOK_STRUCTPARAM|TOK_STRING(MMUniqueMap,pchName,0) },
    { "",    TOK_STRUCTPARAM|TOK_STRING(MMUniqueMap,pchMapFile,0) },
    { "\n",    TOK_STRUCTPARAM|TOK_END },
    {0},
};

TokenizerParseInfo ParseMMUniqueMapCategory[] = 
{
    { "",    TOK_STRUCTPARAM|TOK_STRING(MMUniqueMapCategory,pchName,0) },
    { "Map", TOK_STRUCT(MMUniqueMapCategory, maplist, ParseMMUniqueMap) },
    { "EndCategory", TOK_END },
    {0},
};

TokenizerParseInfo ParseMMUniqueMapCategoryList[] =
{
    { "UniqueMapCategory", TOK_STRUCT(MMUniqueMapCategoryList, mapcategorylist, ParseMMUniqueMapCategory) },
    {0},
};


TokenizerParseInfo ParseMMUnlockableContent[] = 
{
    { "",    TOK_STRUCTPARAM|TOK_STRING(MMUnlockableContent,pchName,0) },
    { "",    TOK_STRUCTPARAM|TOK_STRING(MMUnlockableContent,pchTokenName,0) },
    { "\n",    TOK_STRUCTPARAM|TOK_END },
    {0},
};

TokenizerParseInfo ParseMMUnlockableContentList[] =
{
    { "UnlockableContent", TOK_STRUCT(MMUnlockableContentList, ppUnlockableContent, ParseMMUnlockableContent) },
    {0},
};


TokenizerParseInfo ParseMMNonSelectableEntities[] =
{
    { "NonSelectableEntities", TOK_STRINGARRAY(MMNonSelectableEntities, ppNames) },
    {0},
};


TokenizerParseInfo ParseMMSequencerAnimList[] =
{
    { "{",    TOK_START },
    { "Seqs", TOK_STRINGARRAY(MMSequencerAnimList, ppSeq) },
    { "Anims", TOK_STRINGARRAY(MMSequencerAnimList, ppValidAnimLists) },
    { "}",    TOK_END },
    {0},
};

TokenizerParseInfo ParseMMSequencerAnimLists[] =
{
    { "SeqAnimList", TOK_STRUCT(MMSequencerAnimLists,ppLists,ParseMMSequencerAnimList) },
    {0},
};

TokenizerParseInfo ParseMMRewardRank[] =
{
    { "{",    TOK_START },
    { "Ranks",    TOK_STRINGARRAY(MMRewardRank, pchRanks) },
    { "DropValue",    TOK_INTARRAY(MMRewardRank, values) },
    { "DropChance",    TOK_F32ARRAY(MMRewardRank, chances) },
    { "}",    TOK_END },
    {0},
};

TokenizerParseInfo ParseMMRewards[] =
{
    { "MissionCompleteBonus",    TOK_F32ARRAY(MMRewards, MissionCompleteBonus) },
    { "BonusRequires",            TOK_STRINGARRAY( MMRewards, ppBonusRequires)  },
    { "MissionCompleteBonus1",    TOK_F32ARRAY(MMRewards, MissionCompleteBonus1) },
    { "Bonus1Requires",            TOK_STRINGARRAY( MMRewards, ppBonus1Requires)  },
    { "MissionCompleteBonus2",    TOK_F32ARRAY(MMRewards, MissionCompleteBonus2) },
    { "Bonus2Requires",            TOK_STRINGARRAY( MMRewards, ppBonus2Requires)  },
    { "MissionCap",                TOK_INT(MMRewards,MissionCap,840) },
    { "TicketsPerSpawnCap",        TOK_F32(MMRewards,TicketsPerSpawnCap,84) },
    { "TicketCap",                TOK_INT(MMRewards,TicketCap,9999) },
    { "LevelPenalty",            TOK_F32ARRAY(MMRewards, lowLevelPenalty) },
    { "LevelBonus",                TOK_F32ARRAY(MMRewards, highLevelBonus) },
    { "RankReward",                TOK_STRUCT(MMRewards, ppRewardRank, ParseMMRewardRank) },
    {0},
};
int MMdoppelFlagSets[] = {    
VDF_INVERSE | VDF_SHADOW | VDF_BLACKANDWHITE,
VDF_DEMON | VDF_ANGEL,
VDF_REVERSE,
VDF_RANDOMPOWER,
VDF_GHOST,        
0    //    to mark the end
};
MMContactCategoryList gMMContactCategoryList;
MMVillainGroupList gMMVillainGroupList;
MMAnimList gMMAnimList;
MMObjectList gMMObjectList;
MMDestructableObjectList gMMDestructableObjectList;
MMMapSetList gMMMapSetList;
MMUniqueMapCategoryList gMMUniqueMapCategoryList;
MMUnlockableContentList gMMUnlockableContentList;
MMUnlockableCostumeContentList gMMCostumeUnlockableContentList;
MMNonSelectableEntities gMMNonSelectableEntities;
MMSequencerAnimLists gMMSeqAnimLists;
MMRewards gMMRewards;


MMScrollSet_Loader mmScrollSet_template;

static void validateRewards()
{
    int i, j;

    for( i = 0; i < eaSize(&gMMRewards.ppRewardRank); i++ )
    {
        F32 fTally=0;
        MMRewardRank *pRank = gMMRewards.ppRewardRank[i];
        if( eafSize(&pRank->chances) != eaiSize(&pRank->values) )
            ErrorFilenamef( "PC_Rewards.txt", "Architect: Reward rank has chance/value mismatch!" );
        for( j = 0; j < eafSize(&pRank->chances); j++ )
            fTally += pRank->chances[j];
        if(fTally > 1.f + EPSILON)
            ErrorFilenamef( "PC_Rewards.txt", "Architect: Reward rank has chance sum greater than 1.0" );
    }
}

typedef struct PlayerCreatedStoryArcRestriction
{
    int bRequired;
    int iCharLimit;
    int iMin;
    int iMax;
    int bAllowHTML;
    int bCopyrightCheck;
    StaticDefineInt *pSDI;
    StashTable st_ValidEntries;

}PlayerCreatedStoryArcRestriction;

static StashTable st_StoryArcRestrictions;
static StashTable st_MissionRestrictions;
static StashTable st_DetailRestrictions[kDetail_Count];

static void addRestrictionFromElementList( StashTable stash, MMElementList *pList, int root )
{
    PlayerCreatedStoryArcRestriction * pRestrict;
    int i, j;

    for( i = 0; i < eaSize(&pList->ppElement); i++ )
    {
        for( j = 0; j < eaSize(&pList->ppElement[i]->ppElementList); j++ )
            addRestrictionFromElementList(stash, pList->ppElement[i]->ppElementList[j], root );
    }

    if( !pList->pchField )
        return;

    pRestrict = calloc(1,sizeof(PlayerCreatedStoryArcRestriction));

    for( i = 0; i < eaSize(&pList->ppElement); i++ )
    {
        if( pList->ppElement[i]->pchText )
        {
            if(!pRestrict->st_ValidEntries)
                pRestrict->st_ValidEntries = stashTableCreateWithStringKeys( 8, StashDefault );
            stashAddPointer( pRestrict->st_ValidEntries, pList->ppElement[i]->pchText, pList->ppElement[i]->pchText, 0);
        }
    }

    pRestrict->bRequired = root;
    pRestrict->iCharLimit = pList->CharLimit;
    pRestrict->iMin = pList->ValMin;
    pRestrict->iMax = pList->ValMax;
    pRestrict->bAllowHTML = pList->AllowHTML;
    pRestrict->bCopyrightCheck = pList->bCopyrightCheck;
    pRestrict->pSDI = playerCreatedSDIgetByName(pList->eSpecialAction);

    if (stashAddPointer( stash, pList->pchField, pRestrict, 0))
    {
        devassertmsg(!pList->bFieldRestrictionAlreadyDefined, "The field restriction was not already defined, and you want it to be.");
    }
    else if (!devassertmsg(pList->bFieldRestrictionAlreadyDefined, "The field restriction was already defined, and you didn't want it to be."))
    {
        if (pRestrict->st_ValidEntries)
        {
            stashTableDestroy(pRestrict->st_ValidEntries);
        }
        free(pRestrict);
    }
}

static void addRestrictionFromRegion( StashTable stash, MMRegion * ppRegion )
{
    int i, j;

    for( i = 0; i < eaSize(&ppRegion->ppElementList); i++ )
        addRestrictionFromElementList( stash, ppRegion->ppElementList[i], 1 );
    for( i = 0; i < eaSize(&ppRegion->ppButton); i++ )
    {
        for( j = 0; j < eaSize(&ppRegion->ppButton[i]->ppElementList); j++ )
            addRestrictionFromElementList( stash, ppRegion->ppButton[i]->ppElementList[j], 0 );
    }
}

static void setupLookUpRestrictions(void)
{
    int i;

    st_StoryArcRestrictions = stashTableCreateWithStringKeys( 32, StashDefault );
    st_MissionRestrictions = stashTableCreateWithStringKeys( 32, StashDefault );
    for(i = 0; i < kDetail_Count; i++)
        st_DetailRestrictions[i] = stashTableCreateWithStringKeys( 8, StashDefault );

    for( i = 0; i < eaSize(&mmScrollSet_template.ppRegion); i++ )
    {
        if( stricmp( mmScrollSet_template.ppRegion[i]->pchName, "StoryArcTemplate" )==0 )
            addRestrictionFromRegion(st_StoryArcRestrictions, mmScrollSet_template.ppRegion[i]);

        if( mmScrollSet_template.ppRegion[i]->pchStruct )
        {
            if( stricmp( mmScrollSet_template.ppRegion[i]->pchStruct, "Mission" )==0 )
                addRestrictionFromRegion( st_MissionRestrictions, mmScrollSet_template.ppRegion[i] ); 

            if( stricmp( mmScrollSet_template.ppRegion[i]->pchStruct, "Detail" )==0 )
                addRestrictionFromRegion( st_DetailRestrictions[mmScrollSet_template.ppRegion[i]->detailType], mmScrollSet_template.ppRegion[i] ); 
        }
    }
}



void playerCreatedStoryArc_GenerateData()
{
    Entity *ent = entAllocCommon();
    int i, j;
    char * file = "scripts/Player_Created/PC_Def_NonSelectable_Entities.txt";
    char * file2 = "scripts/Player_Created/PC_Def_Sequencer_Anims.txt";

    ParserLoadFiles(NULL, "scripts/Player_Created/PC_Def_Entities.txt", "PC_Def_Entities.bin", 0, ParseMMVillainGroupList, &gMMVillainGroupList, NULL, NULL, NULL);
    gMMVillainGroupList.st_Included = stashTableCreateWithStringKeys( 3000, StashDefault );
    gMMVillainGroupList.st_GroupNames = stashTableCreateWithStringKeys( 32, StashDefault );

    for( i = 0; i < eaSize(&gMMVillainGroupList.ppIncluded); i++ )
        stashAddPointer( gMMVillainGroupList.st_Included, gMMVillainGroupList.ppIncluded[i], gMMVillainGroupList.ppIncluded[i], 0);

    for( i = eaSize(&gMMVillainGroupList.group)-1; i>=0; i-- )
    {
        gMMVillainGroupList.group[i]->vg = villainGroupGetEnum(gMMVillainGroupList.group[i]->pchName);
        if( gMMVillainGroupList.group[i]->vg == VG_NONE )
        {
            ErrorFilenamef( "scripts/Player_Created/PC_Def_Entities.txt", "Invalid Villain Group %s", gMMVillainGroupList.group[i]->pchName );
            eaRemove(&gMMVillainGroupList.group, i);
        }
        gMMVillainGroupList.group[i]->pchDisplayName = villainGroupGetPrintNameSingular(gMMVillainGroupList.group[i]->vg);
    }

    // Add deliberate invalid group now
    {
        MMVillainGroup * pGroup = StructAllocRaw(sizeof(MMVillainGroup));
        pGroup->vg = VG_NONE;
        pGroup->pchName = StructAllocString("Empty");
        pGroup->pchDisplayName = StructAllocString("NoVillainsString");
        pGroup->minLevel = 1;
        pGroup->maxLevel = 54;
        stashAddPointer( gMMVillainGroupList.st_GroupNames, pGroup->pchName, pGroup, 0);
    }

    for( i = 0; i < eaSize(&gMMVillainGroupList.group)-1; i++ )
    {
        int count = 1;
        for( j = i+1; j < eaSize(&gMMVillainGroupList.group); j++ )
        {
            if( gMMVillainGroupList.group[i]->vg == gMMVillainGroupList.group[j]->vg  )
            {
                eaPush( &gMMVillainGroupList.group[i]->ppGroup, gMMVillainGroupList.group[j] );
                eaRemove(&gMMVillainGroupList.group, j);
            }    
        }
    }

    for( i = 0; i < eaSize(&gMMVillainGroupList.group); i++ )
    {
        stashAddPointer( gMMVillainGroupList.st_GroupNames, gMMVillainGroupList.group[i]->pchName, gMMVillainGroupList.group[i], 0);
        if( !gMMVillainGroupList.group[i]->maxGroupLevel )
            gMMVillainGroupList.group[i]->maxGroupLevel = gMMVillainGroupList.group[i]->maxLevel;
        if( !gMMVillainGroupList.group[i]->minGroupLevel )
            gMMVillainGroupList.group[i]->minGroupLevel = gMMVillainGroupList.group[i]->minLevel;
    }

    gMMSeqAnimLists.st_SeqAnimLists = stashTableCreateWithStringKeys( 32, StashDefault );

    ParserLoadFiles(NULL, "scripts/Player_Created/PC_Def_List_Animations.txt", "PC_Def_Animation.bin", 0, ParseMMAnimList, &gMMAnimList, NULL, NULL, NULL);

    for( i = 0; i < eaSize(&villainDefList.villainDefs); i++ )
    {
        for(j = 0; j < eaSize(&villainDefList.villainDefs[i]->levels); j++)
        {
            const VillainDef *pDef = villainDefList.villainDefs[i];
            const NPCDef *npcDef = npcFindByName( villainDefList.villainDefs[i]->levels[j]->costumes[0], 0);
            SeqInst * seq;
            MMSequencerAnimList * pSeqAnimList = 0;

            if(npcDef && npcDef->costumes)
                seq = seqLoadInst( npcDef->costumes[0]->appearance.entTypeFile, 0, SEQ_LOAD_HEADER_ONLY, 0, ent );
            else
                seq = seqLoadInst( villainDefList.villainDefs[i]->levels[0]->costumes[0], 0, SEQ_LOAD_HEADER_ONLY, 0, ent );
            entSetSeq(ent, seq);

            if( !ent->seq || ent->seq->type->notselectable 
#ifndef TEST_CLIENT
                ||!seqIsMoveSelectable(ent->seq) 
#endif
                )
            {
                eaPushConst(&gMMNonSelectableEntities.ppNames, villainDefList.villainDefs[i]->name);
                break;
            }

            if( !playerCreatedStoryArc_isValidVillain( villainGroupGetName(villainDefList.villainDefs[i]->group), villainDefList.villainDefs[i]->name, 0)  )
                break;

            if(!stashFindPointer(gMMSeqAnimLists.st_SeqAnimLists, ent->seq->type->name, &pSeqAnimList))
            {
                int k;

                char **ppAnims = 0;

                for( k = 0; k < eaSize(&gMMAnimList.anim); k++ )
                {
#ifndef TEST_CLIENT
                    const SeqMove *move = getFirstSeqMoveByName(ent->seq, "READY");
                    if(move)
                        ent->seq->animation.move = move;

                    seqClearState(ent->aiAnimListState);
                    seqClearState(ent->seq->state);
                    alSetAIAnimListOnEntity( ent, gMMAnimList.anim[k]->pchName );
                    seqSetOutputs( ent->seq->state, ent->aiAnimListState );
                    move = seqProcessInst( ent->seq, 30 );
                    if( move )
                        eaPush(&ppAnims, gMMAnimList.anim[k]->pchName);
#endif
                }

                if( eaSize(&ppAnims) )
                {
                    int x;
                    for( k = 0; !pSeqAnimList && k < eaSize(&gMMSeqAnimLists.ppLists); k++ )
                    {
                        if( eaSize(&gMMSeqAnimLists.ppLists[k]->ppValidAnimLists) == eaSize(&ppAnims) )
                        {
                            int mismatch = 0;
                            for( x = 0; x < eaSize(&gMMSeqAnimLists.ppLists[k]->ppValidAnimLists); x++ )
                                mismatch = stricmp(gMMSeqAnimLists.ppLists[k]->ppValidAnimLists[x], ppAnims[x] ) != 0;
                            if( !mismatch )
                            {
                                pSeqAnimList = gMMSeqAnimLists.ppLists[k];
                                eaPush( &pSeqAnimList->ppSeq, StructAllocString(ent->seq->type->name));
                            }
                        }
                    }


                    if( !pSeqAnimList )
                    {
                        pSeqAnimList = StructAllocRaw(sizeof(MMSequencerAnimList));
                        eaPush( &pSeqAnimList->ppSeq, StructAllocString(ent->seq->type->name));
                        for(x = 0; x < eaSize(&ppAnims); x++ )
                            eaPush( &pSeqAnimList->ppValidAnimLists, ppAnims[x]);
                        eaPush( &gMMSeqAnimLists.ppLists, pSeqAnimList);
                    }
                }

                stashAddPointer(gMMSeqAnimLists.st_SeqAnimLists, seq->type->name, pSeqAnimList,0);
            }
        }
    }

    for( i = 0; i < eaSize(&gMMVillainGroupList.group); i++ )
    {
        int min = 54, max = 0;
        int k;

        if( gMMVillainGroupList.group[i]->pchExclude )
            continue;

        // for each villain group validate that all the mission spawnable villains fall in the range of the group
        for( j = 0; j < eaSize(&villainDefList.villainDefs); j++ )
        {
            if( villainDefList.villainDefs[j]->group != gMMVillainGroupList.group[i]->vg )
                continue;
            if( villainDefList.villainDefs[j]->spawnlimit == 0 )
                continue;
            if( !playerCreatedStoryArc_isValidVillain( villainGroupGetName(villainDefList.villainDefs[j]->group), villainDefList.villainDefs[j]->name, 0)  )
                continue;

            for(k = 0; k < eaSize(&villainDefList.villainDefs[j]->levels); k++)
            {
                min = MIN(min, villainDefList.villainDefs[j]->levels[k]->level);
                max = MAX(max, villainDefList.villainDefs[j]->levels[k]->level);
            }
        }

        if( min > gMMVillainGroupList.group[i]->minGroupLevel  ) 
            Errorf( "Villain Group %s will only spawn DOWN to level %i (not level %i)", gMMVillainGroupList.group[i]->pchName, min, gMMVillainGroupList.group[i]->minGroupLevel );
        if( max < gMMVillainGroupList.group[i]->maxGroupLevel  )
            Errorf( "Villain Group %s will only spawn UP to level %i (not level %i)", gMMVillainGroupList.group[i]->pchName, max, gMMVillainGroupList.group[i]->maxGroupLevel );


        
    }

    entFreeCommon(ent);

    //ParserWriteTextFile("scripts/Player_Created/PC_Def_New_Ents.txt", ParseMMVillainGroupList2, &gMMVillainGroupList, 0, 0);

}

bool MMRewardsPostProcess(ParseTable* pti, void* structptr)
{
    MMRewards * rewards = (MMRewards *)structptr;
    if (rewards->ppBonus1Requires)
        chareval_Validate(rewards->ppBonus1Requires, "PC_Rewards.txt");
    if (rewards->ppBonus2Requires)
        chareval_Validate(rewards->ppBonus2Requires, "PC_Rewards.txt");

    return true;
}

void playerCreatedStoryarc_LoadData(int create_bins)
{
    int i, j;
    char * dir;

    if( !create_bins && isDevelopmentMode() )
        dir = strdup("scripts/Player_Created/");
    else
        dir = strdup("scripts.loc/Player_Created");

    ParserLoadFiles(dir, "PC_Rewards.txt", "PC_Rewards.bin", PARSER_SERVERONLY, ParseMMRewards, &gMMRewards, NULL, NULL, MMRewardsPostProcess);
    validateRewards();


    ParserLoadFiles(dir, "PC_Def_Entities.txt", "PC_Def_Entities.bin", 0, ParseMMVillainGroupList, &gMMVillainGroupList, NULL, NULL, NULL);
    gMMVillainGroupList.st_Included = stashTableCreateWithStringKeys( 32, StashDefault );
    gMMVillainGroupList.st_GroupNames = stashTableCreateWithStringKeys( 32, StashDefault );
    gMMVillainGroupList.st_CantMoveEnts = stashTableCreateWithStringKeys( 32, StashDefault );

    for( i = 0; i < eaSize(&gMMVillainGroupList.ppIncluded); i++ )
        stashAddPointer( gMMVillainGroupList.st_Included, gMMVillainGroupList.ppIncluded[i], gMMVillainGroupList.ppIncluded[i], 0);
    for( i = 0; i < eaSize(&gMMVillainGroupList.ppCantMoveNames); i++ )
        stashAddPointerConst( gMMVillainGroupList.st_CantMoveEnts, gMMVillainGroupList.ppCantMoveNames[i], gMMVillainGroupList.ppCantMoveNames[i], 0);

    ParserLoadFiles(dir, "PC_Def_NonSelectable_Entities.txt", "PC_Def_NonSelectable_Entities.bin", 0, ParseMMNonSelectableEntities, &gMMNonSelectableEntities, NULL, NULL, NULL);
    for( i = 0; i < eaSize(&gMMNonSelectableEntities.ppNames); i++ )
        stashAddPointerConst( gMMVillainGroupList.st_CantMoveEnts, gMMNonSelectableEntities.ppNames[i], gMMNonSelectableEntities.ppNames[i], 0);

    for( i = eaSize(&gMMVillainGroupList.group)-1; i>=0; i-- )
    {
        gMMVillainGroupList.group[i]->vg = villainGroupGetEnum(gMMVillainGroupList.group[i]->pchName);
        if( gMMVillainGroupList.group[i]->vg == VG_NONE )
        {
            ErrorFilenamef( "scripts/Player_Created/PC_Def_Entities.txt", "Invalid Villain Group %s", gMMVillainGroupList.group[i]->pchName );
            eaRemove(&gMMVillainGroupList.group, i);
        }
        gMMVillainGroupList.group[i]->pchDisplayName = villainGroupGetPrintNameSingular(gMMVillainGroupList.group[i]->vg);
        gMMVillainGroupList.group[i]->pchDescription = villainGroupGetDescription( gMMVillainGroupList.group[i]->vg);
        if( !gMMVillainGroupList.group[i]->maxGroupLevel )
            gMMVillainGroupList.group[i]->maxGroupLevel = gMMVillainGroupList.group[i]->maxLevel;
        if( !gMMVillainGroupList.group[i]->minGroupLevel )
            gMMVillainGroupList.group[i]->minGroupLevel = gMMVillainGroupList.group[i]->minLevel;
    }

    // Add deliberate invalid group now
    {
        MMVillainGroup * pGroup = StructAllocRaw(sizeof(MMVillainGroup));
        pGroup->vg = VG_NONE;
        pGroup->pchName = StructAllocString("Empty");
        pGroup->pchDisplayName = StructAllocString("NoVillainsString");
        pGroup->minGroupLevel = pGroup->minLevel = 1;
        pGroup->maxGroupLevel = pGroup->maxLevel = 54;
        stashAddPointer( gMMVillainGroupList.st_GroupNames, pGroup->pchName, pGroup, 0);
    }

    for( i = 0; i < eaSize(&gMMVillainGroupList.group)-1; i++ )
    {
        int count = 1;
        for( j = i+1; j < eaSize(&gMMVillainGroupList.group); j++ )
        {
            if( gMMVillainGroupList.group[i]->vg == gMMVillainGroupList.group[j]->vg  )
            {
                eaPush( &gMMVillainGroupList.group[i]->ppGroup, gMMVillainGroupList.group[j] );
                eaRemove(&gMMVillainGroupList.group, j);
            }    
        }
    }

    for( i = 0; i < eaSize(&gMMVillainGroupList.group); i++ )
        stashAddPointer( gMMVillainGroupList.st_GroupNames, gMMVillainGroupList.group[i]->pchName, gMMVillainGroupList.group[i], 0);
    ParserLoadFiles(dir, "PC_Def_List_Contacts.txt", "PC_Def_Contacts.bin", 0, ParseMMContactCategoryList, &gMMContactCategoryList, NULL, NULL, NULL);
    gMMContactCategoryList.st_ContactCategories = stashTableCreateWithStringKeys( 8, StashDefault );
    gMMContactCategoryList.st_Contacts = stashTableCreateWithStringKeys( 32, StashDefault );
    for( i = 0; i < eaSize(&gMMContactCategoryList.ppContactCategories); i++ )
    {
        stashAddPointer( gMMContactCategoryList.st_ContactCategories, gMMContactCategoryList.ppContactCategories[i]->pchCategory, gMMContactCategoryList.ppContactCategories[i], 0);
        for( j = eaSize(&gMMContactCategoryList.ppContactCategories[i]->ppContact)-1; j>=0; j-- )
        {
            MMContact *pContact = gMMContactCategoryList.ppContactCategories[i]->ppContact[j];
            pContact->pDef = PNPCFindDef(pContact->pchNPCDef);
            if( !pContact->pDef )
            {
                ErrorFilenamef( "scripts/Player_Created/PC_Def_List_Contacts.txt", "Invalid Contact %s", pContact->pchNPCDef);
                eaRemove(&gMMContactCategoryList.ppContactCategories[i]->ppContact, j);
            }
            else
                stashAddPointer( gMMContactCategoryList.st_Contacts, pContact->pDef->model, pContact, 0);
        }
    }

    ParserLoadFiles(dir, "PC_Def_List_Objects.txt", "PC_Def_Objects.bin", 0, ParseMMObjectList, &gMMObjectList, NULL, NULL, NULL);
    gMMObjectList.st_Object = stashTableCreateWithStringKeys( 32, StashDefault );
    for( i = 0; i < eaSize(&gMMObjectList.placement); i++ )
    {
        for(j = 0; j < eaSize(&gMMObjectList.placement[i]->object); j++)
        {
            stashAddPointer( gMMObjectList.st_Object, gMMObjectList.placement[i]->object[j]->pchName, gMMObjectList.placement[i]->object[j], 0);
        }
    }

    ParserLoadFiles(dir, "PC_Def_List_Map_Sets.txt", "PC_Def_MapSets.bin", 0, ParseMMMapSetList, &gMMMapSetList, NULL, NULL, NULL);
    gMMMapSetList.st_MapSets = stashTableCreateWithStringKeys( 32, StashDefault );
    for( i = 0; i < eaSize(&gMMMapSetList.mapset); i++ )
        stashAddPointer( gMMMapSetList.st_MapSets, gMMMapSetList.mapset[i]->pchName, gMMMapSetList.mapset[i], 0);

    ParserLoadFiles(dir, "PC_Def_List_Map_Unique.txt", "PC_Def_MapUnique.bin", 0, ParseMMUniqueMapCategoryList, &gMMUniqueMapCategoryList, NULL, NULL, NULL);
    gMMUniqueMapCategoryList.st_UniqueMaps = stashTableCreateWithStringKeys( 32, StashDefault );
    for( i = 0; i < eaSize(&gMMUniqueMapCategoryList.mapcategorylist); i++ )
    {
        for(j = 0; j < eaSize(&gMMUniqueMapCategoryList.mapcategorylist[i]->maplist); j++ )
        {
            if( gMMUniqueMapCategoryList.mapcategorylist[i]->maplist[j]->pchMapFile)
                stashAddPointer( gMMUniqueMapCategoryList.st_UniqueMaps, gMMUniqueMapCategoryList.mapcategorylist[i]->maplist[j]->pchMapFile, gMMUniqueMapCategoryList.mapcategorylist[i]->maplist[j], 0);
        }
    }

    ParserLoadFiles(dir, "PC_Def_List_Animations.txt", "PC_Def_Animation.bin", 0, ParseMMAnimList, &gMMAnimList, NULL, NULL, NULL);
    gMMAnimList.st_Anim = stashTableCreateWithStringKeys( 32, StashDefault );
    for( i = 0; i < eaSize(&gMMAnimList.anim); i++ )
        stashAddPointer( gMMAnimList.st_Anim, gMMAnimList.anim[i]->pchName, gMMAnimList.anim[i], 0);

    ParserLoadFiles(dir, "PC_Def_List_Destructible_Objects.txt", "PC_Def_DestructObject.bin", 0, ParseMMDestructableObjectList, &gMMDestructableObjectList, NULL, NULL, NULL);
    gMMDestructableObjectList.st_Object = stashTableCreateWithStringKeys( 32, StashDefault );
    for( i = 0; i < eaSize(&gMMDestructableObjectList.object); i++ )
    {
        if(!gMMDestructableObjectList.object[i]->pchName )
            ErrorFilenamef("PC_Def_List_Destructible_Objects.txt", "Error Loading Data" );
        else
            stashAddPointer( gMMDestructableObjectList.st_Object, gMMDestructableObjectList.object[i]->pchName, gMMDestructableObjectList.object[i], 0);
    }

    ParserLoadFiles(dir, "PC_Def_Unlockable_Content.txt", "PC_Def_Unlockable_Content.bin", 0, ParseMMUnlockableContentList, &gMMUnlockableContentList, NULL, NULL, NULL);
    gMMUnlockableContentList.st_UnlockableContent = stashTableCreateWithStringKeys( 32, StashDefault );
    gMMCostumeUnlockableContentList.st_UnlockableContent = stashTableCreateWithStringKeys( 32, StashDefault );
    {
        MMUnlockableCostumeSetContent *costumeSet = NULL;
        char *lastStr = NULL;
        char *costumeStrPtr;
        for( i = 0; i < eaSize(&gMMUnlockableContentList.ppUnlockableContent); i++ )
        {

            if( !gMMUnlockableContentList.ppUnlockableContent[i]->pchName || !gMMUnlockableContentList.ppUnlockableContent[i]->pchTokenName)
            {
                if( gMMUnlockableContentList.ppUnlockableContent[i]->pchName )
                    ErrorFilenamef("scripts/Player_Created/PC_Def_Unlockable_Content.txt", "Bad Element Loaded: %i.  %s missing a token", i, gMMUnlockableContentList.ppUnlockableContent[i]->pchName );
                else if ( gMMUnlockableContentList.ppUnlockableContent[i]->pchTokenName )
                    ErrorFilenamef("scripts/Player_Created/PC_Def_Unlockable_Content.txt", "Bad Element Loaded: %i. %s missing a name", i, gMMUnlockableContentList.ppUnlockableContent[i]->pchTokenName );
                else
                {
                    if( i > 0 && gMMUnlockableContentList.ppUnlockableContent[i-1]->pchName )
                        ErrorFilenamef("scripts/Player_Created/PC_Def_Unlockable_Content.txt", "Bad Element Loaded: %i. Prior Element: %s", i, gMMUnlockableContentList.ppUnlockableContent[i-1]->pchName );
                    ErrorFilenamef("scripts/Player_Created/PC_Def_Unlockable_Content.txt", "Bad Element Loaded: %i", i );
                }
            }
            else
            {
                stashAddPointer( gMMUnlockableContentList.st_UnlockableContent, gMMUnlockableContentList.ppUnlockableContent[i]->pchName, gMMUnlockableContentList.ppUnlockableContent[i], 0);
                //    if the last string does not equal the current one
                //    push the created costume set
                if (lastStr && ( stricmp( lastStr, gMMUnlockableContentList.ppUnlockableContent[i]->pchTokenName) != 0 ) )
                {
                    stashAddPointer( gMMCostumeUnlockableContentList.st_UnlockableContent, lastStr, costumeSet, 0 );
                    lastStr = NULL;
                    costumeSet = NULL;
                }
                //    if the key is a costume part string
                costumeStrPtr = strstr( gMMUnlockableContentList.ppUnlockableContent[i]->pchTokenName, "Costume" );
                if ( costumeStrPtr == gMMUnlockableContentList.ppUnlockableContent[i]->pchTokenName )
                {
                    //    if this a new set, alloc space for it
                    if (!costumeSet)
                    {
                        costumeSet = StructAllocRaw(sizeof(MMUnlockableCostumeSetContent));
                        costumeSet->pchTokenName = gMMUnlockableContentList.ppUnlockableContent[i]->pchTokenName;
                    }
                    lastStr = gMMUnlockableContentList.ppUnlockableContent[i]->pchTokenName;
                    //    push the key into the set
                    eaPush(&costumeSet->costume_keys, gMMUnlockableContentList.ppUnlockableContent[i]->pchName );
                }
            }
        }
        if ( lastStr )
        {
            stashAddPointer( gMMCostumeUnlockableContentList.st_UnlockableContent, lastStr, costumeSet, 0 );
            lastStr = NULL;
            costumeSet = NULL;
        }
    }
    ParserLoadFiles(dir, "PC_Def_Sequencer_Anims.txt", "PC_Def_Sequencer_Anims.bin", 0, ParseMMSequencerAnimLists, &gMMSeqAnimLists, NULL, NULL, NULL);
    gMMSeqAnimLists.st_SeqAnimLists = stashTableCreateWithStringKeys( 512, StashDefault );

    for( i = 0; i < eaSize(&gMMSeqAnimLists.ppLists); i++ )
    {
        gMMSeqAnimLists.ppLists[i]->index = i;
        for( j = 0; j < eaSize(&gMMSeqAnimLists.ppLists[i]->ppSeq); j++)
            stashAddPointer( gMMSeqAnimLists.st_SeqAnimLists, gMMSeqAnimLists.ppLists[i]->ppSeq[j], gMMSeqAnimLists.ppLists[i], 0);

        gMMSeqAnimLists.ppLists[i]->st_Anim = stashTableCreateWithStringKeys( 8, StashDefault );
        for(j = 0; j < eaSize(&gMMSeqAnimLists.ppLists[i]->ppValidAnimLists); j++)
            stashAddPointer( gMMSeqAnimLists.ppLists[i]->st_Anim, gMMSeqAnimLists.ppLists[i]->ppValidAnimLists[j], gMMSeqAnimLists.ppLists[i]->ppValidAnimLists[j], 0);
    }


    ParserLoadFiles(dir, "uiTemplate.scrollset", "PC_Def_UI.bin", 0, parse_MMScrollSet_Loader, &mmScrollSet_template, NULL, NULL, NULL);
    setupLookUpRestrictions();

    SAFE_FREE(dir);
}

//------------------------------------------------------------------------------------------------------------------------------
// Data Validation Functions
//------------------------------------------------------------------------------------------------------------------------------

static int ArchitectCanEditCheckLWC( Entity * e )
{
#ifndef TEST_CLIENT
    if (LWC_GetCurrentStage(e) == LWC_STAGE_INVALID || LWC_GetCurrentStage(e) == LWC_STAGE_LAST)
        return 1;
    else
    {
#ifdef CLIENT
        dialogStd( DIALOG_OK, textStd("LWCDataNotReadyAE"), 0, 0, NULL, NULL, DLGFLAG_ARCHITECT );
#endif
        return 0;
    }
#endif

    return 1;
}

int ArchitectCanEdit( Entity * e )
{
    int idx;
    int clientOnMissionMap = 0;

    if( !e || !e->pl )
        return 0;

    if( e->access_level )
        return ArchitectCanEditCheckLWC(e);

    if( entIsTrial(e) )
        return 0;

#ifdef CLIENT
    clientOnMissionMap = game_state.mission_map;
#endif

    if( badge_GetIdxFromName( "ArchitectAccolade", &idx ) && BADGE_IS_OWNED( e->pl->aiBadges[idx] ) && !clientOnMissionMap)
        return ArchitectCanEditCheckLWC(e);

    if (e->pl->inArchitectArea > 0)
        return ArchitectCanEditCheckLWC(e);

    return 0;
}

void ArchitectEnteredVolume( Entity * e )
{
    if( e && e->pl )
        e->pl->inArchitectArea++;
}

void ArchitectExitedVolume( Entity * e )
{
    if( e && e->pl )
        e->pl->inArchitectArea--;
}

int playerCreatedStoryarc_isAnimAllowedOnSeq( char * pchSeq, char *pchAnim )
{
    MMSequencerAnimList * pSeqAnimList;

    if( !stashFindPointer(gMMSeqAnimLists.st_SeqAnimLists, pchSeq, &pSeqAnimList) )
        return 0;
    if( !pSeqAnimList || !stashFindPointer(pSeqAnimList->st_Anim, pchAnim, 0) )
        return 0;

    return 1;
}

int playerCreatedStoryArc_CheckUnlockKey(Entity *e, const char *pchUnlockKey)
{
    int retval = false;

    if( e && e->mission_inventory && pchUnlockKey && strlen( pchUnlockKey ) > 0 )
    {
        if( stashFindPointer(e->mission_inventory->stKeys, pchUnlockKey, 0 ) )
            return 1;
    }

    return 0;
}

int playerCreatedStoryArc_IsCostumeKey( const char *unlockKey, MMUnlockableCostumeSetContent **pCostumeSet)
{
    return ( unlockKey && stashFindPointer( gMMCostumeUnlockableContentList.st_UnlockableContent, unlockKey, pCostumeSet ) );
}
int playerCreatedStoryarc_isUnlockedContent( const char * pchName, Entity * e )
{
    MMUnlockableContent * pUnlock = 0;

    if( !e || !stashFindPointer( gMMUnlockableContentList.st_UnlockableContent, pchName, &pUnlock ) )
        return 1;

    // the entity must have it
    if( playerCreatedStoryArc_CheckUnlockKey(e, pUnlock->pchTokenName) || getRewardToken( e, pUnlock->pchTokenName ) )
        return 1;

    return 0;
}

int playerCreatedStoryarc_isCantMoveVillainDefName( const char * pchName )
{
    if( stashFindPointerConst( gMMVillainGroupList.st_CantMoveEnts, pchName, 0 ) )
        return 1;
    return 0;
}

int playerCreatedStoryarc_isValidVillainDefName( const char * pchName, Entity * e )
{
    if( stashFindPointer( gMMVillainGroupList.st_Included, pchName, 0 ) )
        return playerCreatedStoryarc_isUnlockedContent(pchName, e);

    return 0;
}

int playerCreatedStoryarc_isValidVillainGroup( const char * pchName, Entity * e )
{
    if( stashFindPointer( gMMVillainGroupList.st_GroupNames, pchName, 0 ) )
        return playerCreatedStoryarc_isUnlockedContent(pchName, e);

    return 0;
}



MMVillainGroup* playerCreatedStoryArc_GetVillainGroup( const char * pchName, int set )
{
    static MMVillainGroup *pGroup=0;

    if( !pchName )
        return 0;

    if( !stashFindPointer(gMMVillainGroupList.st_GroupNames, pchName, &pGroup) )
        return 0;

    if( !set )
        return pGroup;

    if( !EAINRANGE(set-1, pGroup->ppGroup ) )
        return 0;

    return pGroup->ppGroup[set-1];
}

int playerCreatedStoryArc_isValidVillain( const char * pchVillainGroup, const char * pchName,  Entity *e )
{
    if(!pchName || !pchVillainGroup)
        return 0;
    if( stricmp(pchName, "Random") == 0 )
    {
        return playerCreatedStoryarc_isValidVillainGroup(pchVillainGroup, e);
    }
    else if (strnicmp(pchName, DOPPEL_NAME, strlen(DOPPEL_NAME)) == 0)
    {
        const char *doppelFlags = pchName + strlen(DOPPEL_NAME);
        int activeFlags = 0;
        while (doppelFlags && doppelFlags[0])
        {
            int i;
            for (i = 1; i < VDF_COUNT; i = i << 1)
            {
                const char *flagName = StaticDefineIntRevLookup(ParseVillainDoppelFlags, i);
                int stringlen = (int)strlen(flagName);
                if (strnicmp(doppelFlags, flagName, stringlen) == 0)
                {
                    doppelFlags += stringlen;
                    activeFlags |= i;
                    break;
                }
            }

            //    there is a flag present that we don't understand
            if (i == VDF_COUNT)
            {
                return 0;
            }
        }

        //    make sure they don't have two flags active in a mutually exclusive set
        if (activeFlags)
        {
            int i = 0;
            while (MMdoppelFlagSets[i])
            {
                int j;
                int found = 0;
                for (j = 1; j < VDF_COUNT; j = j << 1)
                {
                    if ((MMdoppelFlagSets[i] & j) && (activeFlags & j))
                    {
                        //    there was 2 flags in this set that are active
                        if (found)
                            return 0;
                        else
                        {
                            found = 1;
                            activeFlags &= ~j;        //    reset this so we know that we've parsed it
                        }
                    }
                }
                i++;
            }

            //    this flag was not found in the flagsets (tall/short in the future i presume)
            if (activeFlags)
                return 0;
        }
        return 1;
    }
    else
    {
        const VillainDef * pDef = villainFindByName(pchName);
        const char * pchVGName;

        if( !pDef )
            return 0;

        if( !playerCreatedStoryarc_isValidVillainDefName(pchName, e) ||    !playerCreatedStoryarc_isValidVillainGroup(pchVillainGroup, e) )
            return 0;

        // make sure they didn't mess with the text file to trick us
        pchVGName = villainGroupGetName( pDef->group );
        if( !pchVGName || stricmp(pchVGName, pchVillainGroup) != 0 )
            return 0;

        return 1;
    }
}

int playerCreatedStoryArc_IsValidMap( char * pchMapSet, char * pchMapName )
{
    if( !pchMapSet || stricmp( pchMapSet, "Unique" ) == 0 )
    {
        if(stashFindPointer(gMMUniqueMapCategoryList.st_UniqueMaps, pchMapName, 0))
            return 1;
    }
    else if( stashFindPointer( gMMMapSetList.st_MapSets, pchMapSet, 0 ) )
        return 1;

    return 0;
}

char * playerCreatedStoryarc_getMapSetDisplayName( const char * pchMapSet )
{
    MMMapSet *pSet;
    if( stashFindPointer( gMMMapSetList.st_MapSets, pchMapSet, &pSet ) )
        return pSet->pchDisplayName;
    return 0;

}

int playerCreatedStoryArc_isValidContact( char *pchCategory, char* pchContact,  Entity *e )
{
    int categoryOK = !!stashFindPointer(gMMContactCategoryList.st_ContactCategories, pchCategory, 0);
    if( (!stashFindPointer(gMMContactCategoryList.st_ContactCategories, pchCategory, 0) && !isDevelopmentMode())|| 
        !stashFindPointer(gMMContactCategoryList.st_Contacts, pchContact, 0))
    {
        return 0;
    }

    return playerCreatedStoryarc_isUnlockedContent(pchContact, e);
}

int playerCreatedStoryArc_isValidObject( char *pchName, Entity *e )
{
    if( !stashFindPointer( gMMObjectList.st_Object, pchName, 0 ) )
    {
        return 0;
    }
    return playerCreatedStoryarc_isUnlockedContent(pchName, e);
}

int playerCreatedStoryArc_isValidDestructibleObject( char *pchName, Entity *e )
{
    if( !stashFindPointer( gMMDestructableObjectList.st_Object, pchName, 0 ) )
    {
        return 0;
    }
    return playerCreatedStoryarc_isUnlockedContent(pchName, e);
}

int playerCreatedStoryArc_isValidAnim( char *pchName, Entity *e )
{
    if( pchName && stricmp( pchName, "None") != 0 && !stashFindPointer( gMMAnimList.st_Anim, pchName, 0 ) )
    {
        return 0;
    }

    return playerCreatedStoryarc_isUnlockedContent( pchName, e );
}
int playerCreatedStoryArc_isValidPCC( PCC_Critter *pcc, PlayerCreatedStoryArc *pArc, Entity *e, int fixup, int *playableErrorCodes)
{
    if(!pcc)
        return 0;

    if (pcc->pccCostume)
    {
        return isPCCCritterValid(e, pcc, fixup, playableErrorCodes);
    }
    else
    {
        int i;
        int retVal;
        CostumeDiff *diffCostume;
        PCC_Critter *derivedPCC;
        derivedPCC = clone_PCC_Critter(pcc);
        diffCostume = derivedPCC->compressedCostume;
        derivedPCC->pccCostume = costume_clone(pArc->ppBaseCostumes[diffCostume->costumeBaseNum]);
        derivedPCC->pccCostume->appearance = diffCostume->appearance;
        for (i = 0; i < eaSize(&diffCostume->differentParts); i++)
        {
            StructCopy(ParseCostumePart, diffCostume->differentParts[i]->part, derivedPCC->pccCostume->parts[diffCostume->differentParts[i]->index], 0, 0);
        }
        retVal = isPCCCritterValid(e, derivedPCC, fixup, playableErrorCodes);
        StructDestroy(parse_PCC_Critter, derivedPCC);
        return retVal;
    }
}

int playerCreatedStoryArc_isValidCVG( CompressedCVG *cvg, PlayerCreatedStoryArc *pArc, Entity *e, int *error, int fixup, int *playableErrorCode)
{
    int i;
    CustomVG *derivedCVG;
    int retVal = CVG_LE_VALID;
    derivedCVG = StructAllocRaw(sizeof(CustomVG));
    derivedCVG->displayName = cvg->displayName;
    derivedCVG->existingVillainList = cvg->existingVillainList;
    derivedCVG->dontAutoSpawnPCCs = cvg->dontAutoSpawnPCCs;
    derivedCVG->dontAutoSpawnEVs = cvg->dontAutoSpawnEVs;
    for (i = (eaiSize(&cvg->customCritterIdx)-1); i>= 0; i--)
    {
        int critter_id;
        critter_id = cvg->customCritterIdx[i]-1;
        if (EAINRANGE(critter_id, pArc->ppCustomCritters) && pArc->ppCustomCritters[critter_id])
        {
            int index;
            index = eaPush(&derivedCVG->customVillainList, pArc->ppCustomCritters[critter_id]);
            playerCreatedStoryArc_PCCSetCritterCostume( derivedCVG->customVillainList[index], pArc );
        }
        else
        {
            retVal |= CVG_LE_INVALID_CRITTER;
            if (fixup)    eaiRemove(&cvg->customCritterIdx, i);
        }
    }
    retVal |= CVG_isValidCVG(derivedCVG, e, fixup, playableErrorCode);
    for (i = 0; i < eaSize(&derivedCVG->customVillainList); ++i)
    {
        //    if it has a compressed costume, it doesnt need it's uncompressed costume
        if ( derivedCVG->customVillainList[i]->pccCostume && derivedCVG->customVillainList[i]->compressedCostume )
        {
            costume_destroy(derivedCVG->customVillainList[i]->pccCostume);
            derivedCVG->customVillainList[i]->pccCostume = NULL;
        }
    }
    if (fixup)
    {
        int matchFound = 0;
#if CLIENT
        assertmsg(0, "Fixup was assumed to always be on the server. Need to add code to handle fixup on the clientside. Specifically removing the critter from g_CustomGroups");
#endif
        if (retVal & CVG_LE_PROFANITY)        StructReallocString(&cvg->displayName, textStd("InvalidVillainGroupName"));
        for (i = 0; i < eaiSize(&derivedCVG->badCritters); ++i)
        {
            int j;
            for (j = (eaiSize(&cvg->customCritterIdx)-1); j >= 0; --j)
            {
                int critter_id = cvg->customCritterIdx[j]-1;
                if (derivedCVG->customVillainList[derivedCVG->badCritters[i]] == pArc->ppCustomCritters[critter_id])
                {
                    int k;
                    int rankIdx = 0;
                    int foundReplacementStandardCritter = 0;
                    switch (derivedCVG->customVillainList[derivedCVG->badCritters[i]]->rankIdx)
                    {
                    case PCC_RANK_ARCH_VILLAIN:        //    intentional fallthrough
                    case PCC_RANK_ELITE_BOSS:        //    intentional    fallthrough
                    case PCC_RANK_BOSS:
                        rankIdx = 2;
                        break;
                    case PCC_RANK_LT:
                        rankIdx = 1;
                        break;
                    default:
                        break;
                    }
                    for (k = 0; k < eaSize(&cvg->existingVillainList); ++k)
                    {
                        if (cvg->existingVillainList[k]->pchName && (stricmp(cvg->existingVillainList[k]->pchName, PCC_ReplacementCritters[rankIdx][1]) == 0) && 
                            cvg->existingVillainList[k]->displayName && (stricmp(cvg->existingVillainList[k]->displayName, textStd(PCC_ReplacementCritters[rankIdx][0])) == 0) )
                        {
                            foundReplacementStandardCritter = 1;
                        }
                    }
                    if (!foundReplacementStandardCritter)
                    {
                        CVG_ExistingVillain *ev;
                        ev = StructAllocRaw(sizeof(CVG_ExistingVillain));
                        ev->pchName = StructAllocString(PCC_ReplacementCritters[rankIdx][1]);
                        ev->costumeIdx = StructAllocString(PCC_ReplacementCritters[rankIdx][2]);
                        ev->displayName = StructAllocString(textStd(PCC_ReplacementCritters[rankIdx][0]));
                        ev->description = StructAllocString(PCC_ReplacementCritters[rankIdx][3]);
                        ev->setNum = 0;
                        ev->levelIdx = 53;
                        eaPush(&cvg->existingVillainList, ev);
                    }
                    //    Only remove it from this list. In fact, it has to be persistant because the arc will continue to use it.
                    //    It will still be on the arc list so it should be safe (memory-leak wise)
                    eaiRemove(&cvg->customCritterIdx, j);
                    break;
                }
            }
        }
        for (i = 0; i < eaiSize(&derivedCVG->badStandardCritters); ++i)
        {
            CVG_ExistingVillain *ev;
            ev = cvg->existingVillainList[derivedCVG->badStandardCritters[i]];
            if (ev->npcCostume)
            {
                StructDestroy(ParseCustomNPCCostume, ev->npcCostume);
                ev->npcCostume = 0;
            }
            StructReallocString(&ev->pchName, PCC_ReplacementCritters[0][1]);
            StructReallocString(&ev->costumeIdx, PCC_ReplacementCritters[0][2]);
            StructReallocString(&ev->displayName, textStd(PCC_ReplacementCritters[0][0]));
            StructReallocString(&ev->description, PCC_ReplacementCritters[0][3]);
            ev->setNum = 0;
            ev->levelIdx = 53;
        }

        //    remove duplicates
        for (i = (eaSize(&cvg->existingVillainList)-1); i >= 0; --i)
        {
            CVG_ExistingVillain *ev = cvg->existingVillainList[i];
            if (ev->pchName && ev->displayName)
            {
                if ( (stricmp(ev->pchName, PCC_ReplacementCritters[0][1]) == 0) && (stricmp(ev->displayName, PCC_ReplacementCritters[0][0]) == 0) )
                {
                    if (matchFound)
                    {
                        StructDestroy(parse_ExistingVillain, ev);
                        eaRemove(&cvg->existingVillainList, i);
                    }
                    else
                    {
                        matchFound = 1;
                    }
                }
            }
        }
        eaDestroy(&derivedCVG->customVillainList);
        StructFree(derivedCVG);
        //////////////    Revalidate again
        return playerCreatedStoryArc_isValidCVG(cvg, pArc, e, error, 0, playableErrorCode);
    }
    else
    {
        eaDestroy(&derivedCVG->customVillainList);
        StructFree(derivedCVG);
    }
    if(error)
        *error = retVal;
    return (retVal == CVG_LE_VALID);
}
static void playerCreatedStoryArc_AddError(char** estr, char const* pchError, char const* pchName)
{
#if CLIENT
    if( !estr )
        return;
#else
    if(!estr || estrLength(estr))//we only care about the first error
        return;
#endif

    estrConcatCharString(estr, textStd(pchError, pchName));
    estrConcatStaticCharArray(estr, "<br>");
}


// we are going to allow rigid HTML formating only
// These are the valid tags
// <br>
// <b></b>
// <i></i>
// <scale #.#></scale> (.8-1.2)
// <color #######></color>
char * playerCreatedStoryarc_basicSMFValidate(const char *pchName, const char * pchText, int maxCharacters)
{
    int characterCount = 0;
    int bold=0, italic=0, scale=0, color=0;
    const char *pCursor = pchText;

    while( *pCursor )
    {
        if( *pCursor == '<' )
        {
            if( strnicmp(pCursor, "<b>", 3)==0 )
            {
                if( bold )
                    return "MMSMFAlreadyBold";

                bold++;
                pCursor += 3;
            }
            else if(  strnicmp(pCursor, "<i>", 3)==0 )
            {
                if( italic )
                    return "MMSMFAlreadyItalic";

                italic++;
                pCursor += 3;
            }
            else if( strnicmp(pCursor, "<br>", 4)==0 )
            {
                pCursor += 4;
            }    
            else if( strnicmp(pCursor, "</b>", 4)==0 )
            {
                if( !bold )
                    return "MMSMFEndBoldNoStart";

                bold--;
                pCursor += 4;
            }    
            else if( strnicmp(pCursor, "</i>", 4)==0 )
            {
                if( !italic )
                    return "MMSMFEndItalicNoStart";

                italic--;
                pCursor += 4;
            }
            else if( strnicmp(pCursor, "</scale>", 8)==0 ) 
            {
                if( !scale )
                    return "MMSMFEndScaleNoStart";

                scale--;
                pCursor += 8;    
            }
            else if( strnicmp(pCursor, "</color>", 8)==0 )
            {
                if( !color )
                    return "MMSMFEndColorNoStart";

                color--;
                pCursor += 8;
            }
            else if( strnicmp(pCursor, "<scale ", 7 )==0 ) // Format is <scale #.#>  Range is .8-1.2
            {
                F32 fVal;
                char val[4]={0};
                int count;

                pCursor += 7;

                strncpy( val, pCursor, 3 );
                fVal = atof(val);
                if( fVal < .8f || fVal > 1.2f )
                    return "MMSMFScaleOutofBounds";

                count = 3;
                while( *pCursor && count )
                {
                    pCursor++;
                    count--;
                }

                if( count || *pCursor != '>' ) // we ran off the end of the string
                    return "MMSMFBadScaleSyntax";

                scale++;
                pCursor++;

            }
            else if( strnicmp(pCursor, "<color #", 8)==0 )
            {
                int count = 6;
                pCursor += 8;

                while( *pCursor && count )
                {
                    // bounds check the hex value
                    if( !((*pCursor >= '0' && *pCursor <= '9') ||
                        (*pCursor >= 'a' && *pCursor <= 'f') ||
                        (*pCursor >= 'A' && *pCursor <= 'F')) )
                    {
                        return "MMSMFBadColorValue";
                    }
                    pCursor++;
                    count--;
                }

                if( count || *pCursor != '>' ) // we ran off the end of the string
                    return "MMSMFBadColorSyntax";

                color++;
                pCursor++;
            }
            else
            {
                return "MMSMFInvalidorImproperlyFormated";
            }
        }
        else
        {
            pCursor++;
            characterCount++;
            if( maxCharacters && characterCount > maxCharacters )
                return "MMSMFTooLong";
        }
    }

    if( scale)
        return "MMSMFUnclosedScale";
    if( bold)
        return "MMSMFUnclosedBold";
    if( italic)
        return "MMSMFUnclosedItalic";
    if( color)
        return "MMSMFUnclosedColor";

    return 0;
}

char *smf_EncodeAllCharactersGet(const char *str)
{
    const char * pCursor = str, *pStart = str;
    static char * estr;

    if(!str)
        return 0;

    if(!estr)
        estrCreate(&estr);
    estrClear(&estr);

    while( *pCursor )
    {
        if (*pCursor == ' ')
        {
            estrConcatFixedWidth(&estr, pStart, pCursor-pStart);
            estrConcatStaticCharArray(&estr, "&nbsp;");
            pStart = pCursor+1;
        }
        else if (*pCursor == '<')
        {
            estrConcatFixedWidth(&estr, pStart, pCursor-pStart);
            estrConcatStaticCharArray(&estr, "&lt;");
            pStart = pCursor+1;
        }
        else if (*pCursor == '>')
        {
            estrConcatFixedWidth(&estr, pStart, pCursor-pStart);
            estrConcatStaticCharArray(&estr, "&gt;");
            pStart = pCursor+1;
        }
        else if (*pCursor == '&')
        {
            if (strnicmp(pCursor, "&nbsp;", 6) &&
                strnicmp(pCursor, "&lt;", 4) &&
                strnicmp(pCursor, "&gt;", 4) &&
                strnicmp(pCursor, "&amp;", 5) &&
                strnicmp(pCursor, "&#37;", 5))
            {
                estrConcatFixedWidth(&estr, pStart, pCursor-pStart);
                estrConcatStaticCharArray(&estr, "&amp;");
                pStart = pCursor+1;
            }
        }
        else if (*pCursor == '%')
        {
            estrConcatFixedWidth(&estr, pStart, pCursor-pStart);
            estrConcatStaticCharArray(&estr, "&#37;");
            pStart = pCursor+1;
        }
        pCursor++;
    }
    estrConcatFixedWidth(&estr, pStart, pCursor-pStart);
    return estr;
}


char *smf_DecodeAllCharactersGet(const char *str)
{
    static char * estr;
    const char * pStart = str, *pCursor = str;

    if(!str)
        return 0;

    if(!estr)
        estrCreate(&estr);
    estrClear(&estr);

    while( *pCursor )
    {
        if (!strnicmp(pCursor, "&nbsp;", 6))
        {
            estrConcatFixedWidth(&estr, pStart, pCursor-pStart);
            estrConcatChar(&estr, ' ');
            pCursor += 6;
            pStart = pCursor;
        }
        else if (!strnicmp(pCursor, "&lt;", 4))
        {
            estrConcatFixedWidth(&estr, pStart, pCursor-pStart);
            estrConcatChar(&estr, '<');
            pCursor += 4;
            pStart = pCursor;
        }
        else if (!strnicmp(pCursor, "&gt;", 4))
        {
            estrConcatFixedWidth(&estr, pStart, pCursor-pStart);
            estrConcatChar(&estr, '>');
            pCursor += 4;
            pStart = pCursor;
        }
        else if (!strnicmp(pCursor, "&amp;", 5))
        {
            estrConcatFixedWidth(&estr, pStart, pCursor-pStart);
            estrConcatChar(&estr, '&');
            pCursor += 5;
            pStart = pCursor;
        }
        else if (!strnicmp(pCursor, "&#37;", 5))
        {
            estrConcatFixedWidth(&estr, pStart, pCursor-pStart);
            estrConcatCharString(&estr, "%%");
            pCursor += 5; 
            pStart = pCursor;
        }
        else
            pCursor++;
    }
    estrConcatFixedWidth(&estr, pStart, pCursor-pStart);

    return estr;
}



static int tokenFieldsAreValid( TokenizerParseInfo *tpi, StashTable stash, void * structptr, char **estr, int locale_match, int fixup )
{
    int i;
    FORALL_PARSEINFO( tpi, i )    
    {
        PlayerCreatedStoryArcRestriction *pRestrict;
        if( stashFindPointer( stash, tpi[i].name, &pRestrict ) )
        {
            const char * token = SimpleTokenGetSingle( tpi, i, structptr, pRestrict->pSDI );
            void *tokenAddr = (void *)((intptr_t)structptr + tpi[i].storeoffset);
            U32 tst = TokenStoreGetStorageType(tpi[i].type);
            if( !token && pRestrict->bRequired )
            {
                playerCreatedStoryArc_AddError( estr, "ArchitectRequiredFieldMissing", tpi[i].name );
                return 0;
            }

            if( token )
            {
                char * pchProfane = 0;
                // NonHTML fields will be completely escaped
                if( pRestrict->bAllowHTML )
                {
                    char *error;
                    error = playerCreatedStoryarc_basicSMFValidate( tpi[i].name, token, pRestrict->iCharLimit);
                    if(error)
                    {
                        if (stricmp(error, "MMSMFTooLong") == 0)
                        {
                            //    If the description is too long, we assume that it is because it is an old arc where encoding was done instead of allowing HTML
                            //    We give them one more chance and validate it as if it was an old arc.
                            if( strchr( token, '<' ) )
                            {
                                playerCreatedStoryArc_AddError( estr, "ArchitectTextInvalidHTML", tpi[i].name );
                                if (fixup)
                                {
                                    assert(TokenStoreGetStorageType(tpi[i].type) == TOK_STORAGE_INDIRECT_SINGLE);
                                    StructReallocString((char **)tokenAddr, textStd("InvalidStringReplacement"));
                                }
                                return 0;
                            }

                            if( pRestrict->iCharLimit && (int)strlen(smf_DecodeAllCharactersGet(token)) > pRestrict->iCharLimit )
                            {
                                playerCreatedStoryArc_AddError( estr, "ArchitectTextLimitExceeded", tpi[i].name );
                                if (fixup)
                                {
                                    assert(TokenStoreGetStorageType(tpi[i].type) == TOK_STORAGE_INDIRECT_SINGLE);
                                    StructReallocString((char **)tokenAddr, textStd("InvalidStringReplacement"));
                                }
                                return 0;
                            }
                        }
                        else
                        {
                            playerCreatedStoryArc_AddError( estr, "ArchitectTextInvalidHTML", tpi[i].name );
                            if (fixup)
                            {
                                assert(TokenStoreGetStorageType(tpi[i].type) == TOK_STORAGE_INDIRECT_SINGLE);
                                StructReallocString((char **)tokenAddr, textStd("InvalidStringReplacement"));
                            }
                            return 0;
                        }
                    }
                }
                else
                {
                    // ensure fields are escaped
                    if( strchr( token, '<' ) )
                    {
                        playerCreatedStoryArc_AddError( estr, "ArchitectTextInvalidHTML", tpi[i].name );
                        if (fixup)
                        {
                            assert(TokenStoreGetStorageType(tpi[i].type) == TOK_STORAGE_INDIRECT_SINGLE);
                            StructReallocString((char **)tokenAddr, textStd("InvalidStringReplacement"));
                        }
                        return 0;
                    }

                    if( pRestrict->iCharLimit && (int)strlen(smf_DecodeAllCharactersGet(token)) > pRestrict->iCharLimit )
                    {
                        playerCreatedStoryArc_AddError( estr, "ArchitectTextLimitExceeded", tpi[i].name );
                        if (fixup)
                        {
                            assert(TokenStoreGetStorageType(tpi[i].type) == TOK_STORAGE_INDIRECT_SINGLE);
                            StructReallocString((char **)tokenAddr, textStd("InvalidStringReplacement"));
                        }
                        return 0;
                    }
                }

                if( characterIsSpace(*token) )
                {
                    playerCreatedStoryArc_AddError( estr, "ArchitectTextWhiteSpace", tpi[i].name );
                    if (fixup)
                    {
                        assert(TokenStoreGetStorageType(tpi[i].type) == TOK_STORAGE_INDIRECT_SINGLE);
                        StructReallocString((char **)tokenAddr, textStd("InvalidStringReplacement"));
                    }
                    return 0;
                }

                if (pRestrict->iMax)
                {
                    if( pRestrict->iMax && ( atoi(token) > pRestrict->iMax || atoi(token) < pRestrict->iMin) )
                    {
                        if (fixup)
                        {
                            int *intAddr = (int*)tokenAddr;
                            *intAddr = (atoi(token) > pRestrict->iMax) ? pRestrict->iMax : pRestrict->iMin;
                        }
                        playerCreatedStoryArc_AddError( estr, "ArchitectValueOutofBounds", tpi[i].name );
                        return 0;
                    }
                }

                // This check is to allow retrofit story arcs to pass validation. It can be removed once we go live. - CW
                if( !(stricmp( token, "Standard Enemy Groups") == 0 || stricmp( token, "Custom Group") == 0  || stricmp( token, "Custom Groups") == 0) )
                {
                    if( pRestrict->st_ValidEntries && !stashFindPointer(pRestrict->st_ValidEntries, token, 0 ) )
                    {
                        playerCreatedStoryArc_AddError( estr, "ArchitectInvalidField", tpi[i].name );
                        if (fixup)
                        {
                            assert(TokenStoreGetStorageType(tpi[i].type) == TOK_STORAGE_INDIRECT_SINGLE);
                            StructReallocString((char **)tokenAddr, textStd("InvalidStringReplacement"));
                        }
                        return 0;
                    }
                }
#ifndef TEST_CLIENT

                if( locale_match )
                {
                    if( pRestrict->bCopyrightCheck )
                    {
                        if( IsAnyWordProfaneOrCopyright(token) )
                        {
                            playerCreatedStoryArc_AddError( estr, "MMProfaneTextServer", tpi[i].name );
                            if (fixup)
                            {
                                assert(TokenStoreGetStorageType(tpi[i].type) == TOK_STORAGE_INDIRECT_SINGLE);
                                StructReallocString((char **)tokenAddr, textStd("InvalidStringReplacement"));
                            }
                            return 0;
                        }
                    }
                    else if( IsAnyWordProfane(token) )
                    {
                        playerCreatedStoryArc_AddError( estr, "MMProfaneTextServer", tpi[i].name );
                        if (fixup)
                        {
                            assert(TokenStoreGetStorageType(tpi[i].type) == TOK_STORAGE_INDIRECT_SINGLE);
                            StructReallocString((char **)tokenAddr, textStd("InvalidStringReplacement"));
                        }
                        return 0;
                    }
                }
#endif
            }
        }
    }

    return 1;
}

static void PCC_generateErrorText(int pcc_return_code, char **estr, char *listName, char *critterName)
{
    if (pcc_return_code & PCC_LE_NULL)            {    playerCreatedStoryArc_AddError(estr, "PCC_LE_NULL", listName);    }
    if (pcc_return_code & PCC_LE_NO_FILE)        {    playerCreatedStoryArc_AddError(estr, "PCC_LE_NO_FILE", listName);    }
    if (pcc_return_code & PCC_LE_BAD_PARSE)        {    playerCreatedStoryArc_AddError(estr, "PCC_LE_BAD_PARSE", listName);    }
    if (pcc_return_code & PCC_LE_INVALID_NAME)    {    playerCreatedStoryArc_AddError(estr, "PCC_LE_INVALID_NAME", listName);    }
    else
    {
        if (pcc_return_code & PCC_LE_INVALID_VILLAIN_GROUP)    {    playerCreatedStoryArc_AddError(estr, "PCC_LE_INVALID_VILLAIN_GROUP",critterName );    }
        if (pcc_return_code & PCC_LE_INVALID_RANGED)    {    playerCreatedStoryArc_AddError(estr, "PCC_LE_INVALID_RANGED", critterName);    }
        if (pcc_return_code & PCC_LE_INVALID_PRIMARY_POWER) {    playerCreatedStoryArc_AddError(estr, "PCC_LE_INVALID_PRIMARY_POWER", critterName);    }
        if (pcc_return_code & PCC_LE_INVALID_DIFFICULTY)    {    playerCreatedStoryArc_AddError(estr, "PCC_LE_INVALID_DIFFICULTY",critterName );    }
        if (pcc_return_code & PCC_LE_INVALID_SELECTED_PRIMARY_POWERS)    {    playerCreatedStoryArc_AddError(estr, "PCC_LE_INVALID_SELECTED_PRIMARY_POWERS",critterName );    }
        if (pcc_return_code & PCC_LE_INVALID_SECONDARY_POWER)    {    playerCreatedStoryArc_AddError(estr, "PCC_LE_INVALID_SECONDARY_POWER", critterName);    }
        if (pcc_return_code & PCC_LE_INVALID_DIFFICULTY2)    {    playerCreatedStoryArc_AddError(estr, "PCC_LE_INVALID_DIFFICULTY2",critterName );    }
        if (pcc_return_code & PCC_LE_INVALID_SELECTED_SECONDARY_POWERS)    {    playerCreatedStoryArc_AddError(estr, "PCC_LE_INVALID_SELECTED_SECONDARY_POWERS",critterName );    }
        if (pcc_return_code & PCC_LE_INVALID_TRAVEL_POWER)    {    playerCreatedStoryArc_AddError(estr, "PCC_LE_INVALID_TRAVEL_POWER", critterName);    }
        if (pcc_return_code & PCC_LE_CONFLICTING_POWERSETS)    {    playerCreatedStoryArc_AddError(estr, "PCC_LE_CONFLICTING_POWERSETS", critterName);    }
        if (pcc_return_code & PCC_LE_BAD_COSTUME)    {    playerCreatedStoryArc_AddError(estr, "PCC_LE_BAD_COSTUME", critterName);    }
        if (pcc_return_code & PCC_LE_INACCESSIBLE_COSTUME)    {    playerCreatedStoryArc_AddError(estr, "PCC_LE_INACCESSIBLE_COSTUME", critterName);    }
        if (pcc_return_code & PCC_LE_INVALID_RANK)    {    playerCreatedStoryArc_AddError(estr, "PCC_LE_INVALID_RANK", critterName);    }
        if (pcc_return_code & PCC_LE_FAILED_TO_REMOVE)    {    playerCreatedStoryArc_AddError(estr, "PCC_LE_FAILED_TO_REMOVE", critterName);    }
        if (pcc_return_code & PCC_LE_PROFANITY_NAME)            {    playerCreatedStoryArc_AddError(estr, "PCC_LE_PROFANITY_NAME", critterName);    }
        if (pcc_return_code & PCC_LE_PROFANITY_VG)            {    playerCreatedStoryArc_AddError(estr, "PCC_LE_PROFANITY_VG", critterName);    }
        if (pcc_return_code & PCC_LE_PROFANITY_DESC)            {    playerCreatedStoryArc_AddError(estr, "PCC_LE_PROFANITY_DESC", critterName);    }
        if (pcc_return_code & PCC_LE_INVALID_DESC)            {    playerCreatedStoryArc_AddError(estr, "PCC_LE_INVALID_DESC", critterName);    }
    }
}

void playerCreatedStoryarcValidate_setDefaultEnemy(char **group, char **enemy, int boss)
{
    char *critter = (boss)?DEFAULT_BOSS:DEFAULT_CRITTER;
    if(group)
    {
        if(*group)
            StructReallocString(group, DEFAULT_VILLAIN_GROUP);
        else
            *group = StructAllocString(DEFAULT_VILLAIN_GROUP);
    }
    if(enemy)
    {
        if(*enemy)
            StructReallocString(enemy, critter);
        else
            *enemy = StructAllocString(critter);
    }
}

PlayerCreatedStoryarcValidation playerCreatedStoryarc_DetailIsValid( PlayerCreatedStoryArc *pArc, PlayerCreatedDetail *pDetail, char ** estr, char **errors2, int *pMin, int *pMax, int iMin, int iMax, Entity *creator, int fixup )
{
    PlayerCreatedStoryarcValidation retval = kPCS_Validation_OK;
    MMVillainGroup *pGroup;
    int pass = 0;

    if( pDetail->pchName )
    {
        char * newstr = StructAllocString(smf_DecodeAllCharactersGet(pDetail->pchName));
        StructFreeString( pDetail->pchName);
        pDetail->pchName = newstr;
    }

    //pchVillainGroupName
    pass = playerCreatedStoryarc_isUnlockedContent( pDetail->pchVillainGroupName, creator );
    if(fixup && !pass)
    {
        StructReallocString(&pDetail->pchVillainGroupName, DEFAULT_VILLAIN_GROUP);
        pDetail->iVillainSet = 0;
        pass = playerCreatedStoryarc_isUnlockedContent( pDetail->pchVillainGroupName, creator );
        if(pass)
        {
            playerCreatedStoryArc_AddError( errors2, "ArchitectLockedContent", pDetail->pchVillainGroupName );
            MAX1(retval, kPCS_Validation_ERRORS);
        }
        
    }
    if( !pass )
    {
        playerCreatedStoryArc_AddError( estr, "ArchitectLockedContent", pDetail->pchVillainGroupName );
        retval = kPCS_Validation_FAIL;
    }


    //pchEntityGroupName
    pass = playerCreatedStoryarc_isUnlockedContent( pDetail->pchEntityGroupName, creator );
    if(fixup && !pass)
    {
        StructReallocString(&pDetail->pchEntityGroupName, DEFAULT_VILLAIN_GROUP);
        pDetail->iVillainSet = 0;
        pass = playerCreatedStoryarc_isUnlockedContent( pDetail->pchEntityGroupName, creator );
        if(pass)
        {
            playerCreatedStoryArc_AddError( errors2, "ArchitectLockedContent", pDetail->pchEntityGroupName );
            MAX1(retval, kPCS_Validation_ERRORS);
        }
    }
    if( !pass )
    {
        playerCreatedStoryArc_AddError( estr, "ArchitectLockedContent", pDetail->pchEntityGroupName );
        retval = kPCS_Validation_FAIL;
    }

    //pchEntityGroupName
    pass = playerCreatedStoryarc_isUnlockedContent( pDetail->pchVillainGroup2Name, creator );
    if(fixup && !pass)
    {
        StructReallocString(&pDetail->pchVillainGroup2Name, DEFAULT_VILLAIN_GROUP);
        pDetail->iVillainSet = 0;
        pass = playerCreatedStoryarc_isUnlockedContent( pDetail->pchVillainGroup2Name, creator );
        if(pass)
        {
            playerCreatedStoryArc_AddError( errors2, "ArchitectLockedContent", pDetail->pchVillainGroup2Name );
            MAX1(retval, kPCS_Validation_ERRORS);
        }
    }
    if( !pass )
    {
        playerCreatedStoryArc_AddError( estr, "ArchitectLockedContent", pDetail->pchVillainGroup2Name );
        retval = kPCS_Validation_FAIL;
    }

    if (pDetail->CVGIdx) 
    {
        int errorCode = 0;
        int playableErrors = 0;
        if ((!EAINRANGE(pDetail->CVGIdx-1, pArc->ppCustomVGs)) || !playerCreatedStoryArc_isValidCVG(pArc->ppCustomVGs[pDetail->CVGIdx-1], pArc, creator, &errorCode, fixup, &playableErrors))
        {
            retval = (errorCode&~PCC_LE_INVALID_DESC)?kPCS_Validation_FAIL:kPCS_Validation_ERRORS;

            if(!fixup || retval == kPCS_Validation_FAIL)
            {
                generatePCCErrorText_ex(errorCode, estr);
                playerCreatedStoryArc_AddError( estr, "ArchitectInvalidVillainGroup", "CustomVillainGroupError" );
            }
            else 
            {
                generatePCCErrorText_ex(errorCode, errors2);
            }
        }
        else
        {
            int low, high;
            if(playableErrors)
            {
                generatePCCErrorText_ex(playableErrors, errors2);
            }
            CVG_getLevelsFromCompressedVG( pArc->ppCustomVGs[pDetail->CVGIdx-1], &low, &high, pArc->ppCustomCritters );
            if( low > *pMin )
                *pMin = low;
            if( high < *pMax )
                *pMax = high;
        }
    }
    else
    {
        pGroup = playerCreatedStoryArc_GetVillainGroup( pDetail->pchVillainGroupName, pDetail->iVillainSet );
        if(fixup && !pGroup)
        {
            //    None is okay on some details (bosses, escort/rescue/ally)
            if ((pDetail->eDetailType == kDetail_Boss || pDetail->eDetailType == kDetail_GiantMonster || pDetail->eDetailType == kDetail_Escort || pDetail->eDetailType == kDetail_Rescue || pDetail->eDetailType == kDetail_Ally) && 
                (pDetail->pchVillainGroupName && !(stricmp(pDetail->pchVillainGroupName, "None") == 0)) )
            {
                StructReallocString(&pDetail->pchVillainGroupName, DEFAULT_VILLAIN_GROUP);
                pDetail->iVillainSet = 0;
                pGroup = playerCreatedStoryArc_GetVillainGroup( pDetail->pchVillainGroupName, pDetail->iVillainSet );
                if(pGroup)
                {
                    playerCreatedStoryArc_AddError( errors2, "ArchitectInvalidVillainGroup", pDetail->pchVillainGroupName );
                    MAX1(retval, kPCS_Validation_ERRORS);
                }
            }
        }
        if(!pGroup )
        {
            // boss and person spawns can get their group from the entity
            if( (pDetail->eDetailType == kDetail_Boss || pDetail->eDetailType == kDetail_GiantMonster || pDetail->eDetailType == kDetail_Escort || pDetail->eDetailType == kDetail_Rescue || pDetail->eDetailType == kDetail_Ally) && 
                (pDetail->pchVillainGroupName && !(stricmp(pDetail->pchVillainGroupName, "None") == 0)) )
            {
                playerCreatedStoryArc_AddError( estr, "ArchitectInvalidVillainGroup", pDetail->pchVillainGroupName );
                retval = kPCS_Validation_FAIL;
            }
        }
        else
        {
            if( (!iMin && pGroup->maxGroupLevel < *pMin) || (!iMax && pGroup->minGroupLevel > *pMax) )
            {
                playerCreatedStoryArc_AddError( estr, "ArchitectInvalidLevelRange", pDetail->pchName );
                retval = kPCS_Validation_FAIL;
            }

            if( pGroup->minGroupLevel > *pMin )
                *pMin = pGroup->minGroupLevel;
            if( pGroup->maxGroupLevel < *pMax )
                *pMax = pGroup->maxGroupLevel;
        }

        pGroup = playerCreatedStoryArc_GetVillainGroup( pDetail->pchEntityGroupName, pDetail->iEntityVillainSet );
        if( pGroup && pDetail->pchVillainDef && stricmp( "random", pDetail->pchVillainDef ) == 0 )
        {
            if( (!iMin && pGroup->maxGroupLevel < *pMin) || (!iMax && pGroup->minGroupLevel > *pMax) )
            {
                playerCreatedStoryArc_AddError( estr, "ArchitectInvalidLevelRange", pDetail->pchName );
                retval = kPCS_Validation_FAIL;
            }

            if( pGroup->minGroupLevel > *pMin )
                *pMin = pGroup->minGroupLevel;
            if( pGroup->maxGroupLevel < *pMax )
                *pMax = pGroup->maxGroupLevel;
        }

    }
    if( pDetail->eDetailType == kDetail_Rumble ) // only rumbles require second villain group
    {
        if (pDetail->CVGIdx2)
        {
            int errorCode = 0;
            int playableErrors;
            if ((!EAINRANGE(pDetail->CVGIdx2-1, pArc->ppCustomVGs)) || !playerCreatedStoryArc_isValidCVG(pArc->ppCustomVGs[pDetail->CVGIdx2-1], pArc, creator, &errorCode, fixup, &playableErrors))
            {
                MAX1(retval, (errorCode&~PCC_LE_INVALID_DESC)?kPCS_Validation_FAIL:kPCS_Validation_ERRORS);

                if(!fixup || retval == kPCS_Validation_FAIL)
                {
                    generatePCCErrorText_ex(errorCode, estr);
                    playerCreatedStoryArc_AddError( estr, "ArchitectInvalidVillainGroup", "CustomVillainGroupError" );
                }
                else
                {
                    generatePCCErrorText_ex(errorCode, errors2);
                }
            }
            else
            {
                int low, high;
                if(playableErrors)
                    generatePCCErrorText_ex(playableErrors, errors2);
                CVG_getLevelsFromCompressedVG( pArc->ppCustomVGs[pDetail->CVGIdx2-1], &low, &high, pArc->ppCustomCritters );
                if( low > *pMin )
                    *pMin = low;
                if( high < *pMax )
                    *pMax = high;
            }
        }
        else
        {
            pGroup = playerCreatedStoryArc_GetVillainGroup( pDetail->pchVillainGroup2Name, pDetail->iVillainSet2 );
            if(fixup && !pGroup)
            {
                StructReallocString(&pDetail->pchVillainGroup2Name, DEFAULT_VILLAIN_GROUP);
                pGroup = playerCreatedStoryArc_GetVillainGroup( pDetail->pchVillainGroup2Name, pDetail->iVillainSet2 );
                if(pGroup)
                {
                    playerCreatedStoryArc_AddError( errors2, "ArchitectInvalidVillainGroup", pDetail->pchVillainGroup2Name );
                    MAX1(retval, kPCS_Validation_ERRORS);
                }
            }
            if(!pGroup )
            {
                playerCreatedStoryArc_AddError( estr, "ArchitectInvalidVillainGroup", pDetail->pchVillainGroup2Name );
                retval = kPCS_Validation_FAIL;
            }
            else
            {
                if( (!iMin && pGroup->maxGroupLevel < *pMin) || (!iMax && pGroup->minGroupLevel > *pMax) )
                {
                    playerCreatedStoryArc_AddError( estr, "ArchitectInvalidLevelRange", pDetail->pchName );
                    retval = kPCS_Validation_FAIL;
                }

                if( pGroup->minGroupLevel > *pMin )
                    *pMin = pGroup->minGroupLevel;
                if( pGroup->maxGroupLevel < *pMax )
                    *pMax = pGroup->maxGroupLevel;
            }
        }
    }

    if( pDetail->ePlacement < 0 || pDetail->ePlacement >= kPlacement_Count )
    {
        playerCreatedStoryArc_AddError( estr, "ArchitectInvalidPlacement", pDetail->pchName );
        retval = kPCS_Validation_FAIL;
    }

    if( pDetail->eAlignment < 0 || pDetail->eAlignment >= kAlignment_Count )
    {
        playerCreatedStoryArc_AddError( estr, "ArchitectInvalidAlignment", pDetail->pchName );
        retval = kPCS_Validation_FAIL;
    }

    if( pDetail->eDetailType == kDetail_Rescue ||  pDetail->eDetailType == kDetail_Escort || pDetail->eDetailType == kDetail_Ally || pDetail->eDetailType == kDetail_DestructObject )
    {
        if( pDetail->eDifficulty < 0 || pDetail->eDifficulty >= kDifficulty_Count )
        {
            playerCreatedStoryArc_AddError( estr, "ArchitectInvalidDifficulty", pDetail->pchName );
            retval = kPCS_Validation_FAIL;
        }
    }
    else if( pDetail->eDifficulty < 0 || pDetail->eDifficulty >= kDifficulty_Single )
    {
        playerCreatedStoryArc_AddError( estr, "ArchitectInvalidDifficulty", pDetail->pchName );
        retval = kPCS_Validation_FAIL;
    }

    if( pDetail->eDetailType < 0 || pDetail->eDetailType >= kDetail_Count )
    {
        playerCreatedStoryArc_AddError( estr, "ArchitectInvalidDetailType", pDetail->pchName );
        return  kPCS_Validation_FAIL;
    }

    if( !playerCreatedStoryArc_isValidAnim( pDetail->pchDefaultStartingAnim, creator ) )
    {

        if(!fixup)
        {
            retval = kPCS_Validation_FAIL;
            playerCreatedStoryArc_AddError( estr, "ArchitectInvalidAnim", pDetail->pchDefaultStartingAnim );
        }
        else
        {
            MAX1(retval, kPCS_Validation_ERRORS);
            playerCreatedStoryArc_AddError( errors2, "ArchitectInvalidAnim", pDetail->pchDefaultStartingAnim );
            StructReallocString(&pDetail->pchDefaultStartingAnim, DEFAULT_ANIMATION);
        }
        
    }

    if( pDetail->eDetailType == kDetail_Boss || pDetail->eDetailType == kDetail_GiantMonster || pDetail->eDetailType == kDetail_Escort || 
        pDetail->eDetailType == kDetail_Rescue || pDetail->eDetailType == kDetail_Ally )
    {
        if( pDetail->customCritterId )
        {
            PCC_Critter * pcc;
            int pcc_error;
            if( !EAINRANGE(pDetail->customCritterId-1, pArc->ppCustomCritters) )
            {
                if(!fixup)
                {
                    playerCreatedStoryArc_AddError( estr, "ArchitectCustomCritterIDInvalid", pDetail->pchName );
                    retval = kPCS_Validation_FAIL;
                }
                else
                {
                    playerCreatedStoryArc_AddError( errors2, "ArchitectCustomCritterIDInvalid", pDetail->pchName );
                    MAX1(retval, kPCS_Validation_ERRORS);
                    pDetail->customCritterId = 0;
                    StructReallocString(&pDetail->pchEntityType, "Standard");
                    playerCreatedStoryarcValidate_setDefaultEnemy(&pDetail->pchEntityGroupName, &pDetail->pchVillainDef, pDetail->eDetailType == kDetail_Boss);
                }
            }
            else
            {
                int replaced = 0;
                int playableErrors = 0;
                pcc = pArc->ppCustomCritters[pDetail->customCritterId-1];
                pcc_error = playerCreatedStoryArc_isValidPCC( pcc, pArc, creator, fixup, &playableErrors );
                if (pcc_error!= PCC_LE_VALID)
                {
                    if(!fixup)
                    {
                        PCC_generateErrorText(pcc_error, estr, pDetail->pchName, pcc ? pcc->name : NULL);
                        retval = kPCS_Validation_FAIL;
                    }
                    else
                    {
                        PCC_generateErrorText(pcc_error, errors2, pDetail->pchName, pcc ? pcc->name : NULL);
                        MAX1(retval, kPCS_Validation_ERRORS);
                        pDetail->customCritterId = 0;
                        StructReallocString(&pDetail->pchEntityType, "Standard");
                        playerCreatedStoryarcValidate_setDefaultEnemy(&pDetail->pchEntityGroupName, &pDetail->pchVillainDef, pDetail->eDetailType == kDetail_Boss);
                        replaced = 1;
                    }
                }
                if(playableErrors)
                    PCC_generateErrorText(pcc_error, errors2, pDetail->pchName, pcc ? pcc->name : NULL);
                if     ((pDetail->eDetailType == kDetail_Boss || pDetail->eDetailType == kDetail_GiantMonster ) && (pcc->rankIdx == PCC_RANK_CONTACT))
                    //    Bosses cannot be contacts
                {
                    if(!fixup)
                    {
                        playerCreatedStoryArc_AddError( estr, "ArchitectContactAsBoss", pDetail->pchName );
                        retval = kPCS_Validation_FAIL;
                    }
                    if (fixup && !replaced)
                    {
                        playerCreatedStoryArc_AddError( errors2, "ArchitectContactAsBoss", pDetail->pchName );
                        MAX1(retval, kPCS_Validation_ERRORS);
                        pDetail->customCritterId = 0;
                        StructReallocString(&pDetail->pchEntityType, "Standard");
                        playerCreatedStoryarcValidate_setDefaultEnemy(&pDetail->pchEntityGroupName, &pDetail->pchVillainDef, pDetail->eDetailType == kDetail_Boss);
                    }
                }
            }
        }
        else
        {
            if( !playerCreatedStoryarc_isUnlockedContent( pDetail->pchVillainDef, creator ) )
            {
                if(fixup)
                {
                    playerCreatedStoryArc_AddError( estr, "ArchitectLockedContent", pDetail->pchVillainDef );
                    playerCreatedStoryarcValidate_setDefaultEnemy(&pDetail->pchEntityGroupName, &pDetail->pchVillainDef, pDetail->eDetailType == kDetail_Boss);
                    MAX1(retval, kPCS_Validation_ERRORS);
                }
                else
                {
                    playerCreatedStoryArc_AddError( estr, "ArchitectLockedContent", pDetail->pchVillainDef );
                    retval = kPCS_Validation_FAIL;
                }
            }

            if( !playerCreatedStoryArc_isValidVillain(pDetail->pchEntityGroupName, pDetail->pchVillainDef, creator) )
            {
                if(fixup)
                {
                    playerCreatedStoryArc_AddError( errors2, "ArchitectInvalidCritter", pDetail->pchName );
                    playerCreatedStoryarcValidate_setDefaultEnemy(&pDetail->pchEntityGroupName, &pDetail->pchVillainDef, pDetail->eDetailType == kDetail_Boss);
                    MAX1(retval, kPCS_Validation_ERRORS);
                }
                else
                {
                    playerCreatedStoryArc_AddError( estr, "ArchitectInvalidCritter", pDetail->pchName );
                    retval = kPCS_Validation_FAIL;
                }
            }
            else if ( ( stricmp(pDetail->pchVillainDef, "Random") != 0 ) && ( strnicmp(pDetail->pchVillainDef, DOPPEL_NAME, strlen(DOPPEL_NAME)) != 0 ))
            {
                const VillainDef *pDef = villainFindByName(pDetail->pchVillainDef);

                if(!pDef )
                {
                    if(fixup)
                    {
                        playerCreatedStoryArc_AddError( errors2, "ArchitectInvalidCritter", pDetail->pchName );
                        StructReallocString(&pDetail->pchVillainDef, DEFAULT_VILLAIN_GROUP);
                        MAX1(retval, kPCS_Validation_ERRORS);
                    }
                    else
                    {
                        playerCreatedStoryArc_AddError( estr, "ArchitectInvalidCritter", pDetail->pchName );
                        retval = kPCS_Validation_FAIL;
                    }
                }
                else
                {
                    int minLevel = pDef->levels[0]->level;
                    int maxLevel = pDef->levels[eaSize(&pDef->levels)-1]->level;
                    int fail= 0;
                    int levelRangeFail = 0;

                    if( (!iMin && maxLevel < *pMin) || (!iMax && minLevel > *pMax) )
                    {
                        levelRangeFail = fail = 1;
                    }

                    if( minLevel > *pMin )
                        *pMin = minLevel;
                    if( maxLevel < *pMax )
                        *pMax = maxLevel;

                    if( (pDetail->eDetailType == kDetail_Boss) && pDef->rank < VR_SNIPER  && pDef->rank < VR_BIGMONSTER )
                    {
                        fail = 1;
                    }
                    else if( pDetail->eDetailType == kDetail_GiantMonster && pDef->rank < VR_BIGMONSTER )
                    {
                        fail = 1;
                    }
                    else if( (pDetail->eDetailType == kDetail_Rescue ||  pDetail->eDetailType == kDetail_Escort || pDetail->eDetailType == kDetail_Ally ) )
                    {    
                        if(  pDef->rank == VR_SNIPER || playerCreatedStoryarc_isCantMoveVillainDefName(pDetail->pchVillainDef) )
                        {
                            fail = 1;
                        }
                    }

                    if(fixup && fail)
                    {
                        if(levelRangeFail)
                            playerCreatedStoryArc_AddError( errors2, "ArchitectInvalidLevelRange", pDetail->pchName );
                        else
                            playerCreatedStoryArc_AddError( errors2, "ArchitectInvalidCritter", pDetail->pchName );

                        playerCreatedStoryarcValidate_setDefaultEnemy(&pDetail->pchEntityGroupName, &pDetail->pchVillainDef, pDetail->eDetailType == kDetail_Boss);
                        MAX1(retval, kPCS_Validation_ERRORS);
                    }
                    else if (fail)
                    {
                        if(levelRangeFail)
                            playerCreatedStoryArc_AddError( estr, "ArchitectInvalidLevelRange", pDetail->pchName );
                        else
                            playerCreatedStoryArc_AddError( estr, "ArchitectInvalidCritter", pDetail->pchName );

                        retval = kPCS_Validation_FAIL;
                    }
                }
            }
        }

        if( !playerCreatedStoryArc_isValidAnim(pDetail->pchBossStartingAnim, creator) )
        {
            
            if(!fixup)
            {
                playerCreatedStoryArc_AddError( estr, "ArchitectInvalidAnim", pDetail->pchBossStartingAnim );
                retval = kPCS_Validation_FAIL;
            }
            else
            {
                playerCreatedStoryArc_AddError( errors2, "ArchitectInvalidAnim", pDetail->pchBossStartingAnim );
                StructReallocString(&pDetail->pchBossStartingAnim, "None");
                MAX1(retval, kPCS_Validation_ERRORS);
            }
        }

        if( pDetail->eDetailType == kDetail_Escort || pDetail->eDetailType == kDetail_Rescue || pDetail->eDetailType == kDetail_Ally )
        {
            if( pDetail->ePersonCombatType < 0 || pDetail->ePersonCombatType >= kPersonCombat_Count  )
            {
                playerCreatedStoryArc_AddError( estr, "ArchitectInvalidPersonCombat", pDetail->pchName );
                retval = kPCS_Validation_FAIL;
            }
            if( pDetail->ePersonRescuedBehavior < 0 || pDetail->ePersonRescuedBehavior >= kPersonBehavior_Count  )
            {
                playerCreatedStoryArc_AddError( estr, "ArchitectInvalidPersonBehavior", pDetail->pchName );
                retval = kPCS_Validation_FAIL;
            }
            if( pDetail->ePersonArrivedBehavior < 0 || pDetail->ePersonArrivedBehavior >= kPersonBehavior_Count  )
            {
                playerCreatedStoryArc_AddError( estr, "ArchitectInvalidPersonBehavior", pDetail->pchName );
                retval = kPCS_Validation_FAIL;
            }

            if( !playerCreatedStoryArc_isValidAnim(pDetail->pchPersonStartingAnimation, creator) )
            {
                
                if(!fixup || retval == kPCS_Validation_FAIL)
                {
                    retval = kPCS_Validation_FAIL;
                    playerCreatedStoryArc_AddError( estr, "ArchitectInvalidAnim", pDetail->pchPersonStartingAnimation );
                }
                else
                {
                    MAX1(retval, kPCS_Validation_ERRORS);
                    playerCreatedStoryArc_AddError( errors2, "ArchitectInvalidAnim", pDetail->pchPersonStartingAnimation );
                    StructReallocString(&pDetail->pchPersonStartingAnimation, DEFAULT_ANIMATION);
                }
            }

            if( !playerCreatedStoryArc_isValidAnim(pDetail->pchRescueAnimation, creator) )
            {
                if(!fixup || retval == kPCS_Validation_FAIL)
                {
                    retval = kPCS_Validation_FAIL;
                    playerCreatedStoryArc_AddError( estr, "ArchitectInvalidAnim", pDetail->pchRescueAnimation );
                }
                else
                {
                    MAX1(retval, kPCS_Validation_ERRORS);
                    playerCreatedStoryArc_AddError( errors2, "ArchitectInvalidAnim", pDetail->pchRescueAnimation );
                    StructReallocString(&pDetail->pchRescueAnimation, DEFAULT_ANIMATION);
                }
                
            }

            if( !playerCreatedStoryArc_isValidAnim(pDetail->pchReRescueAnimation, creator) )
            {
                if(!fixup || retval == kPCS_Validation_FAIL)
                {
                    playerCreatedStoryArc_AddError( estr, "ArchitectInvalidAnim", pDetail->pchReRescueAnimation );
                    retval = kPCS_Validation_FAIL;
                }
                else
                {
                    playerCreatedStoryArc_AddError( errors2, "ArchitectInvalidAnim", pDetail->pchReRescueAnimation );
                    StructReallocString(&pDetail->pchReRescueAnimation, DEFAULT_ANIMATION);
                    MAX1(retval, kPCS_Validation_ERRORS);
                }
            }

            if( !playerCreatedStoryArc_isValidAnim(pDetail->pchArrivalAnimation, creator) )
            {
                
                if(!fixup || retval == kPCS_Validation_FAIL)
                {
                    playerCreatedStoryArc_AddError( estr, "ArchitectInvalidAnim", pDetail->pchArrivalAnimation );
                    retval = kPCS_Validation_FAIL;
                }
                else
                {
                    playerCreatedStoryArc_AddError( errors2, "ArchitectInvalidAnim", pDetail->pchArrivalAnimation );
                    StructReallocString(&pDetail->pchArrivalAnimation, DEFAULT_ANIMATION);
                    MAX1(retval, kPCS_Validation_ERRORS);
                }
            }

            if( !playerCreatedStoryArc_isValidAnim(pDetail->pchStrandedAnimation, creator) )
            {
                
                if(!fixup || retval == kPCS_Validation_FAIL)
                {
                    playerCreatedStoryArc_AddError( estr, "ArchitectInvalidAnim", pDetail->pchStrandedAnimation );
                    retval = kPCS_Validation_FAIL;
                }
                else
                {
                    playerCreatedStoryArc_AddError( errors2, "ArchitectInvalidAnim", pDetail->pchStrandedAnimation );
                    StructReallocString(&pDetail->pchStrandedAnimation, DEFAULT_ANIMATION);
                    MAX1(retval, kPCS_Validation_ERRORS);
                }
            }
        }
    }
    else if(pDetail->eDetailType == kDetail_Collection)
    {
        if( !playerCreatedStoryArc_isValidObject(pDetail->pchVillainDef, creator)  )
        {
            if(fixup)
            {
                playerCreatedStoryArc_AddError( errors2, "ArchitectInvalidCollectionObject", pDetail->pchVillainDef );
                StructReallocString(&pDetail->pchVillainDef,DEFAULT_OBJECT);
                MAX1(retval, kPCS_Validation_ERRORS);
            }
            else
            {
                playerCreatedStoryArc_AddError( estr, "ArchitectInvalidCollectionObject", pDetail->pchVillainDef );
                retval = kPCS_Validation_FAIL;    
            }
            
        }
    }
    else if( pDetail->eDetailType == kDetail_DestructObject || pDetail->eDetailType ==  kDetail_DefendObject )
    {
        if( pDetail->eObjectLocation < 0 || pDetail->eObjectLocation >= kObjectLocationType_Count || !stashFindPointer(gMMDestructableObjectList.st_Object, pDetail->pchVillainDef, 0) )
        {
            if(fixup)
            {
                char *temp = estrTemp();
                playerCreatedStoryArc_AddError( errors2, "ArchitectInvalidDestructibleObject", pDetail->pchVillainDef );
                estrPrintf(&temp, "%s%s", pDetail->pchVillainDef, DEFAULT_OBJECT_SUFFIX);
                if(playerCreatedStoryArc_isValidDestructibleObject(temp, creator))
                    StructReallocString(&pDetail->pchVillainDef,temp);
                else
                {
                    estrPrintf(&temp, "%s%s", DEFAULT_OBJECT, DEFAULT_OBJECT_SUFFIX);
                    StructReallocString(&pDetail->pchVillainDef,temp);
                }
                MAX1(retval, kPCS_Validation_ERRORS);
                estrDestroy(&temp);
            }
            else
            {
                playerCreatedStoryArc_AddError( estr, "ArchitectInvalidDestructibleObject", pDetail->pchVillainDef );
                retval = kPCS_Validation_FAIL;    
            }
        }
    }

    MAX1(retval,tokenFieldsAreValid( ParsePlayerCreatedDetail, st_DetailRestrictions[pDetail->eDetailType], pDetail, estr, pArc->locale_id==getCurrentLocale(), fixup )?kPCS_Validation_OK:kPCS_Validation_ERRORS);    //todo, fix this so that it is ACTUALLY fixed up, and then switch to fail

    return retval;
}

typedef struct DetailNode DetailNode;
typedef struct DetailNode
{
    void * pDetail;
    void ** ppNext;
    int visitStatus;
}DetailNode;
typedef enum DN_VisitStatus
{
    DNVS_UNVISITED,
    DNVS_VISITED,
    DNVS_CLEAR,
}DN_VisitStatus;
static DetailNode **s_ppDetailNodeList;

void playerCreatedStoryarc_clearDetailNodeList()
{
    int i;
    for (i = 0; i < eaSize(&s_ppDetailNodeList); ++i)
    {
        eaDestroy(&s_ppDetailNodeList[i]->ppNext);
    }
    eaDestroy(&s_ppDetailNodeList);
}
static int performDFS(DetailNode *pDet)
{
    assert(pDet);
    if (pDet)
    {
        int i;
        int isCycle = 0;
        switch (pDet->visitStatus)
        {
        case DNVS_VISITED:    return 1;
        case DNVS_CLEAR: return 0;
        default:
            pDet->visitStatus = DNVS_VISITED;
        }

        for (i = 0; i < eaSize(&pDet->ppNext); ++i)
        {
            int j;
            for (j = 0; j < eaSize(&s_ppDetailNodeList); ++j)
            {
                if (s_ppDetailNodeList[j]->pDetail == pDet->ppNext[i])
                {
                    if (performDFS(s_ppDetailNodeList[j]))
                    {
                        return 1;
                    }
                }
            }
        }
        pDet->visitStatus = DNVS_CLEAR;
        return 0;
    }
    return 1;
}
int playerCreatedStoryArc_addDetailNodeLink(void * pDetail, void *pDetailNext, void *pMission)
{
    int i, found = 0, valid = 1;
    int detail_required=0, parent_required=0;

    if( pMission ) // required elements can't be triggered by non-required elements
    {
        PlayerCreatedMission *pMish = (PlayerCreatedMission*)pMission;
        PlayerCreatedDetail * p1 = (PlayerCreatedDetail *)pDetail;
        PlayerCreatedDetail * p2 = (PlayerCreatedDetail *)pDetailNext;

        for( i = 0; i < eaSize(&pMish->ppMissionCompleteDetails); i++ )
        {
            if( p1->pchName && stricmp( pMish->ppMissionCompleteDetails[i], p1->pchName ) == 0 )
                parent_required = 1;
            if( p2->pchName && stricmp( pMish->ppMissionCompleteDetails[i], p2->pchName ) == 0 )
                detail_required = 1;
        }

        if( detail_required && !parent_required )
            return 0;
    }

    //    add to adjacency list
    for( i = 0; i < eaSize(&s_ppDetailNodeList); i++ )
    {
        if (s_ppDetailNodeList[i]->pDetail == pDetail)
        {
            eaPushUnique(&s_ppDetailNodeList[i]->ppNext, pDetailNext);        
        }
        s_ppDetailNodeList[i]->visitStatus = 0;
    }

    if(!found)
    {
        DetailNode * pNew = calloc(1, sizeof(DetailNode));
        pNew->pDetail = pDetail; 
        eaPush(&pNew->ppNext, pDetailNext);
        pNew->visitStatus = DNVS_UNVISITED;
        eaPush(&s_ppDetailNodeList, pNew);
    }

    //    using http://en.wikipedia.org/wiki/Topological_sorting#Algorithms
    //    the second algorithm using DFS
    for (i = 0; i < eaSize(&s_ppDetailNodeList); ++i)
    {
        if (performDFS(s_ppDetailNodeList[i]))
        {
            return 0;
        }
    }

    return 1;

}


PlayerCreatedStoryarcValidation playerCreatedStoryarc_MissionIsValid( PlayerCreatedStoryArc * pArc, PlayerCreatedMission *pMission, char **estr, char **errors2, Entity *creator, int fixup )
{
    PlayerCreatedStoryarcValidation retval = kPCS_Validation_OK;
    int i, j, obj_cnt = 0;
    MapLimit mapLimit = {0};
    int wall_cnt = 0, floor_cnt = 0, around_cnt = 0, regular_cnt = 0, ambush_cnt = 0, defeat_all = 0, giant_cnt = 0;
    int iMinLevel = 0, iMaxLevel = 53;
    int possibleFail = pMission->fTimeToComplete>0;
    MMVillainGroup *pGroup = 0;

    // check the basics
    MAX1(retval,tokenFieldsAreValid( ParsePlayerCreatedMission, st_MissionRestrictions, pMission, estr, pArc->locale_id==getCurrentLocale(), fixup )?kPCS_Validation_OK:kPCS_Validation_FAIL);

    if (pMission->CVGIdx)
    {
        int errorCode = 0;
        int playableErrors = 0;
        if ((!EAINRANGE(pMission->CVGIdx-1, pArc->ppCustomVGs)) || !playerCreatedStoryArc_isValidCVG(pArc->ppCustomVGs[pMission->CVGIdx-1], pArc, creator, &errorCode,fixup, &playableErrors ))
        {
            retval = (errorCode&~PCC_LE_INVALID_DESC)?kPCS_Validation_FAIL:kPCS_Validation_ERRORS;
            if(!fixup || retval == kPCS_Validation_FAIL)
            {
                generatePCCErrorText_ex(errorCode, estr);
                playerCreatedStoryArc_AddError( estr, "ArchitectInvalidVillainGroup", "CustomVillainGroupString" );
            }
            else
            {
                generatePCCErrorText_ex(errorCode, errors2);
            }
        }
        else
        {
            if(playableErrors)
                generatePCCErrorText_ex(playableErrors, errors2);
            CVG_getLevelsFromCompressedVG(pArc->ppCustomVGs[pMission->CVGIdx-1], &iMinLevel, &iMaxLevel, pArc->ppCustomCritters);
        }
    }
    else
    {
        pGroup = playerCreatedStoryArc_GetVillainGroup( pMission->pchVillainGroupName, pMission->iVillainSet );
        if(!pGroup && fixup)
        {
            playerCreatedStoryArc_AddError( errors2, "ArchitectInvalidVillainGroup", pMission->pchVillainGroupName );
            MAX1(retval, kPCS_Validation_ERRORS);
            StructReallocString(&pMission->pchVillainGroupName, DEFAULT_VILLAIN_GROUP);
            pMission->iVillainSet = 0;
            pGroup = playerCreatedStoryArc_GetVillainGroup( pMission->pchVillainGroupName, pMission->iVillainSet );
        }
        if(!pGroup )
        {
            if (!pMission->pchVillainGroupName)
            {
                pMission->pchVillainGroupName = StructAllocString("None");
            }
            playerCreatedStoryArc_AddError( estr, "ArchitectInvalidVillainGroup", pMission->pchVillainGroupName );
            retval = kPCS_Validation_FAIL;
        }
        else
        {
            iMinLevel =    pGroup->minLevel;
            iMaxLevel =    pGroup->maxLevel;
        }
    }
    // Map is valid and everything fits on it
    if( !pMission->pchMapName )
    {
        if(fixup)
        {
            playerCreatedStoryArc_AddError( errors2, "ArchitectNoMap", 0 );
            StructReallocString(&pMission->pchMapName, DEFAULT_MAP);
            StructReallocString(&pMission->pchMapSet, DEFAULT_MAP_SET);
            pMission->eMapLength = DEFAULT_MAP_TIME;
            MAX1(retval, kPCS_Validation_ERRORS);
        }
        else
        {
            playerCreatedStoryArc_AddError( estr, "ArchitectNoMap", 0 );
            return kPCS_Validation_FAIL;
        }
    }

    if( !playerCreatedStoryarc_isUnlockedContent( pMission->pchMapName, creator ) )
    {
        if(fixup)
        {
            playerCreatedStoryArc_AddError( errors2, "ArchitectLockedContent", pMission->pchMapName );
            StructReallocString(&pMission->pchMapName, DEFAULT_MAP);
            StructReallocString(&pMission->pchMapSet, DEFAULT_MAP_SET);
            pMission->eMapLength = DEFAULT_MAP_TIME;
            MAX1(retval, kPCS_Validation_ERRORS);
        }
        else
        {
            playerCreatedStoryArc_AddError( estr, "ArchitectLockedContent", pMission->pchMapName );
            return kPCS_Validation_FAIL;
        }
    }

    if( !playerCreatedStoryArc_IsValidMap( pMission->pchMapSet, pMission->pchMapName ) )
    {
        if(!fixup)
        {
            playerCreatedStoryArc_AddError( estr, "ArchitectInvalidMap", pMission->pchMapName );
            return kPCS_Validation_FAIL;
        }
        else
        {
            playerCreatedStoryArc_AddError( errors2, "ArchitectInvalidMap", pMission->pchMapName );
            StructReallocString(&pMission->pchMapName, DEFAULT_MAP);
            StructReallocString(&pMission->pchMapSet, DEFAULT_MAP_SET);
            pMission->eMapLength = DEFAULT_MAP_TIME;
            MAX1(retval, kPCS_Validation_ERRORS);
        }
        
    }

    // map time needs to be validated too
    if( stricmp( "Unique", pMission->pchMapSet )!=0 && stricmp( "Random", pMission->pchMapName ) != 0 && !missionMapStat_isFileValid(pMission->pchMapName, pMission->pchMapSet, pMission->eMapLength) )
    {
        if(!fixup)
        {
            playerCreatedStoryArc_AddError( estr, "ArchitectInvalidMap", pMission->pchMapName );
            return kPCS_Validation_FAIL;
        }
        else
        {
            playerCreatedStoryArc_AddError( errors2, "ArchitectInvalidMap", pMission->pchMapName );
            StructReallocString(&pMission->pchMapName, DEFAULT_MAP);
            StructReallocString(&pMission->pchMapSet, DEFAULT_MAP_SET);
            pMission->eMapLength = DEFAULT_MAP_TIME;
            MAX1(retval, kPCS_Validation_ERRORS);
        }
    }

    if( stricmp( "Random", pMission->pchMapName ) == 0 || stricmp( "RandomString", pMission->pchMapName ) == 0 )
    {
        const MapLimit *pMapLimit = missionMapStat_GetRandom( pMission->pchMapSet, pMission->eMapLength);
        if( !pMapLimit )
        {
            playerCreatedStoryArc_AddError( estr, "ArchitectInvalidRandomMapSet", pMission->pchMapSet );
            return kPCS_Validation_FAIL;
        }
        memcpy(&mapLimit,pMapLimit,sizeof(MapLimit));
    }
    else
    {
        MissionMapStats *pStat = missionMapStat_Get(pMission->pchMapName);
        if( !pStat )
        {
            if(!fixup)
            {
                retval = kPCS_Validation_FAIL;
                playerCreatedStoryArc_AddError( estr, "ArchitectInvalidMap", pMission->pchMapName );
            }
            else
            {
                playerCreatedStoryArc_AddError( errors2, "ArchitectInvalidMap", pMission->pchMapName );
                StructReallocString(&pMission->pchMapName, DEFAULT_MAP);
                StructReallocString(&pMission->pchMapSet, DEFAULT_MAP_SET);
                pMission->eMapLength = DEFAULT_MAP_TIME;
                pStat = missionMapStat_Get(pMission->pchMapName);
                MAX1(retval, kPCS_Validation_ERRORS);
            }
            
        }
            
        if( !pStat )
        {
            retval = kPCS_Validation_FAIL;
            if(fixup)
                devassert(0);    //is the default map screwed up!?!?
        }
        else
        {
            MissionPlaceStats * pPlace = pStat->places[0];
            int x;

            mapLimit.regular_only[0] = pPlace->genericEncounterOnlyCount;
            for( x = 0; x < eaSize(&pPlace->customEncounters); x++ )
            {
                if( stricmp( "Around", pPlace->customEncounters[x]->type ) == 0)
                {
                    mapLimit.around_only[0] = pPlace->customEncounters[x]->countOnly;
                    mapLimit.around_or_regular[0] = pPlace->customEncounters[x]->countGrouped;
                }
                if( stricmp( "GiantMonster", pPlace->customEncounters[x]->type ) == 0)
                    mapLimit.giant_monster = pPlace->customEncounters[x]->countOnly;
            }
            mapLimit.wall[0] = pPlace->wallObjectiveCount;
            mapLimit.floor[0] = pPlace->floorObjectiveCount;
        }
    }

    if(  pMission->ePacing < 0 || pMission->ePacing >= kPacing_Count)
    {
        playerCreatedStoryArc_AddError( estr, "ArchitectInvalidPacing", 0 );
        retval = kPCS_Validation_FAIL;
    }

    // delete the fail detail list because we are going to rebuild it
    while( eaSize(&pMission->ppMissionFailDetails) )
        StructFreeString(eaRemove(&pMission->ppMissionFailDetails,0));

    for( i = 0; i < eaSize(&pMission->ppMissionCompleteDetails); i++ )
    {
        int found_objective = 0;
        int objective_is_failable = 0;

        if( stricmp( pMission->ppMissionCompleteDetails[i], "DefeatAllVillains") == 0 ||
            stricmp( pMission->ppMissionCompleteDetails[i], "DefeatAllVillainsInEndRoom") == 0 )
        {
            obj_cnt++;
            defeat_all = 1;
            continue;
        }

        for(j = 0; j < eaSize(&pMission->ppDetail); j++ )
        {
            if( pMission->ppDetail[j]->pchName && stricmp( smf_DecodeAllCharactersGet(pMission->ppDetail[j]->pchName), pMission->ppMissionCompleteDetails[i] ) == 0)
            {
                if( pMission->ppDetail[j]->eDetailType == kDetail_Ambush ||    pMission->ppDetail[j]->eDetailType == kDetail_Patrol )
                {
                    playerCreatedStoryArc_AddError( estr, "ArchitectMissionObjectiveNotCompleteable", pMission->ppMissionCompleteDetails[i] );
                    retval = kPCS_Validation_FAIL;
                }
                else if(  pMission->ppDetail[j]->eAlignment == kAlignment_Ally &&
                    (pMission->ppDetail[j]->eDetailType == kDetail_DestructObject ||    
                    pMission->ppDetail[j]->eDetailType == kDetail_DefendObject ||
                    pMission->ppDetail[j]->eDetailType == kDetail_Boss ||
                    pMission->ppDetail[j]->eDetailType == kDetail_GiantMonster ||
                    pMission->ppDetail[j]->eDetailType == kDetail_DefendObject) )
                {
                    playerCreatedStoryArc_AddError( estr, "ArchitectMissionObjectiveBadAlignment", pMission->ppMissionCompleteDetails[i] );
                    retval = kPCS_Validation_FAIL;
                }
                found_objective = 1;

                // Defend, Escorts and Bosses that Run away can fail
                if( (pMission->ppDetail[j]->eDetailType == kDetail_DefendObject) || 
                    (pMission->ppDetail[j]->eDetailType == kDetail_Escort) ||
                    ( pMission->ppDetail[j]->eDetailType == kDetail_Boss &&
                    ((pMission->ppDetail[j]->pchRunAwayHealthThreshold && *pMission->ppDetail[j]->pchRunAwayHealthThreshold != '0' ) ||
                    (pMission->ppDetail[j]->pchRunAwayTeamThreshold && *pMission->ppDetail[j]->pchRunAwayTeamThreshold  != '0' )) ))
                {
                    objective_is_failable = 1;
                }
                possibleFail |= objective_is_failable;
            }
        }

        if(!found_objective)
        {
            playerCreatedStoryArc_AddError( estr, "ArchitectInvalidMissionObjective", pMission->ppMissionCompleteDetails[i] );
            retval = kPCS_Validation_FAIL;
        }
        else
        {
            obj_cnt++;
            if(objective_is_failable) // Ally objects should succeed when ally is rescued, but never cause the mission to fail if they die
                eaPush(&pMission->ppMissionFailDetails, StructAllocString(smf_DecodeAllCharactersGet(pMission->ppMissionCompleteDetails[i])));
        }
    }

    if(!obj_cnt)
    {
        playerCreatedStoryArc_AddError( estr, "ArchitectNoValidMissionObjectives", 0 );
        retval = kPCS_Validation_FAIL;
    }

    if( obj_cnt == 1 && defeat_all && pGroup && pGroup->vg == VG_NONE )
    {
        playerCreatedStoryArc_AddError( estr, "ArchitectNoValidMissionObjectives", 0 );
        retval = kPCS_Validation_FAIL;
    }


    if(possibleFail && (!pMission->pchTaskReturnFail || strlen(pMission->pchTaskReturnFail)==0) )
    {
        if(!fixup)
            playerCreatedStoryArc_AddError( estr, "ArchitectFailPossible", 0 );
        else
            playerCreatedStoryArc_AddError( errors2, "ArchitectFailPossible", 0 );
        //we don't want to invalidate arcs because of this
        MAX1(retval, kPCS_Validation_ERRORS);
        
    }

    if( eaSize(&pMission->ppDetail) > 25 )
    {
        playerCreatedStoryArc_AddError( estr, "ArchitectTooManyDetails", 0 );
        retval = kPCS_Validation_FAIL;
    }

    playerCreatedStoryarc_clearDetailNodeList();

    for( i = 0; i < eaSize(&pMission->ppDetail); i++ )
    {
        PlayerCreatedDetail *pDetail = pMission->ppDetail[i];

        for( j = i+1; j < eaSize(&pMission->ppDetail); j++ )
        {
            if( pDetail->pchName && pMission->ppDetail[j]->pchName && stricmp(pDetail->pchName, pMission->ppDetail[j]->pchName) == 0 )
            {
                playerCreatedStoryArc_AddError( estr, "ArchitectDupliacteNames", 0 );
                retval = kPCS_Validation_FAIL;
            }
        }

        if( pDetail->iQty <= 0 ) // this should never be zero, but players figured out a way
            pDetail->iQty = 1;

        if( pDetail->eDetailType == kDetail_GiantMonster )
            giant_cnt += pDetail->iQty;
        if( pDetail->eDetailType  == kDetail_Collection )
        {
            if( pDetail->eObjectLocation == kObjectLocationType_Wall )
                wall_cnt += pDetail->iQty;
            else if( pDetail->eObjectLocation == kObjectLocationType_Floor )
                floor_cnt += pDetail->iQty;
            else
            {
                playerCreatedStoryArc_AddError( estr, "ArchitectObjectLocation", 0 );
                retval = kPCS_Validation_FAIL;
            }
        }
        else if( pDetail->eDetailType == kDetail_Escort || pDetail->eDetailType == kDetail_Rescue || pDetail->eDetailType == kDetail_Ally || 
            pDetail->eDetailType == kDetail_DefendObject || pDetail->eDetailType == kDetail_Rumble )
            around_cnt += pDetail->iQty;
        else
            regular_cnt += pDetail->iQty;

        if( pDetail->eDetailType == kDetail_DefendObject )
            ambush_cnt += pDetail->iQty;

        if( pDetail->eDetailType == kDetail_Ambush ) 
        {
            ambush_cnt++;
            if(!pDetail->pchDetailTrigger ) 
            {
                playerCreatedStoryArc_AddError( estr, "ArchitectInvalidAmbushTrigger", 0 );
                retval = kPCS_Validation_FAIL;
            }
        }

        if( pDetail->pchEscortDestination )
        {
            int found = 0;

            for(j = 0; j < eaSize(&pMission->ppDetail); j++)
            {
                PlayerCreatedDetail * pDet = pMission->ppDetail[j];

                if( pDet->pchName )
                {
                    char * pchName = smf_DecodeAllCharactersGet( pDet->pchName );
                    if( strcmp(pDetail->pchEscortDestination, pchName ) == 0 )
                        found++;
                }
            }

            if(found == 0)
            {
                playerCreatedStoryArc_AddError( estr, "ArchitectInvalidAmbushTrigger", 0 );
                retval = kPCS_Validation_FAIL;
            }
        }

        if( pDetail->pchDetailTrigger )
        {
            int found = 0;

            for(j = 0; j < eaSize(&pMission->ppDetail); j++)
            {
                PlayerCreatedDetail * pDet = pMission->ppDetail[j];

                if( i != j &&  pDetail->eDetailType == kDetail_Ambush && pDet->pchDetailTrigger && stricmp( pDet->pchDetailTrigger, pDetail->pchDetailTrigger ) == 0 )
                {
                    playerCreatedStoryArc_AddError( estr, "ArchitectInvalidAmbushTriggerDuplicate", pDetail->pchDetailTrigger );
                    retval = kPCS_Validation_FAIL;
                }

                if( pDet->pchName )
                {
                    char * pchName = smf_DecodeAllCharactersGet( pDet->pchName );

                    if( strcmp(pDetail->pchDetailTrigger, pchName ) == 0 )
                    {
                        found++;
                        if( !playerCreatedStoryArc_addDetailNodeLink( pDet, pDetail, pMission ) )
                        {
                            playerCreatedStoryArc_AddError( estr, "ArchitectInvalidCircularDependency", pDetail->pchDetailTrigger );
                            retval = kPCS_Validation_FAIL;
                        }
                        continue;
                    }

                    if( pDet->eDetailType == kDetail_Boss ||  pDet->eDetailType == kDetail_GiantMonster || pDet->eDetailType == kDetail_DestructObject )
                    {
                        char * trigger;
                        estrCreate(&trigger);
                        estrConcatf(&trigger, "%s 75_HEALTH", pchName);
                        if( strcmp(pDetail->pchDetailTrigger, trigger ) == 0 )
                        {
                            found++;
                            if( !playerCreatedStoryArc_addDetailNodeLink( pDet, pDetail, pMission ) )
                            {
                                playerCreatedStoryArc_AddError( estr, "ArchitectInvalidCircularDependency", pDetail->pchDetailTrigger );
                                retval = kPCS_Validation_FAIL;
                            }
                        }

                        estrClear(&trigger);
                        estrConcatf(&trigger, "%s 50_HEALTH", pchName );
                        if( strcmp(pDetail->pchDetailTrigger, trigger ) == 0 )
                        {
                            if( !playerCreatedStoryArc_addDetailNodeLink( pDet, pDetail, pMission ) )
                            {
                                playerCreatedStoryArc_AddError( estr, "ArchitectInvalidCircularDependency", pDetail->pchDetailTrigger );
                                retval = kPCS_Validation_FAIL;
                            }
                            found++;
                        }

                        estrClear(&trigger);
                        estrConcatf(&trigger, "%s 25_HEALTH", pchName );
                        if( strcmp(pDetail->pchDetailTrigger, trigger ) == 0 )
                        {
                            if( !playerCreatedStoryArc_addDetailNodeLink( pDet, pDetail, pMission ) )
                            {
                                playerCreatedStoryArc_AddError( estr, "ArchitectInvalidCircularDependency", pDetail->pchDetailTrigger );
                                retval = kPCS_Validation_FAIL;
                            }
                            found++;
                        }

                        estrDestroy(&trigger);
                    }

                    if( pDet->eDetailType == kDetail_Escort )
                    {
                        char * trigger;
                        estrCreate(&trigger);
                        estrConcatf(&trigger, "%s Rescue", pchName );
                        if( strcmp(pDetail->pchDetailTrigger, trigger ) == 0 )
                        {
                            found++;
                            if( !playerCreatedStoryArc_addDetailNodeLink( pDet, pDetail, pMission ) )
                            {
                                playerCreatedStoryArc_AddError( estr, "ArchitectInvalidCircularDependency", pDetail->pchDetailTrigger );
                                retval = kPCS_Validation_FAIL;
                            }
                        }
                        estrDestroy(&trigger);
                    }

                }
            }

            if(found > 1)
            {
                playerCreatedStoryArc_AddError( estr, "ArchitectInvalidAmbushTriggerDuplicate", pDetail->pchDetailTrigger );
                retval = kPCS_Validation_FAIL;
            }

            if(found == 0)
            {
                playerCreatedStoryArc_AddError( estr, "ArchitectInvalidAmbushTrigger", 0 );
                retval = kPCS_Validation_FAIL;
            }
        }

        MAX1(retval, playerCreatedStoryarc_DetailIsValid( pArc, pDetail, estr, errors2, &iMinLevel, &iMaxLevel, pMission->iMinLevel, pMission->iMaxLevel, creator, fixup ));
    }

    if(pMission->iMinLevel && pMission->iMaxLevel && pMission->iMinLevel > pMission->iMaxLevel )
    {
        playerCreatedStoryArc_AddError( estr, "ArchitectLevelRange", 0 );
        retval = kPCS_Validation_FAIL;
    }
    if( !(pMission->iMinLevel && pMission->iMaxLevel) && iMinLevel > iMaxLevel )
    {
        playerCreatedStoryArc_AddError( estr, "ArchitectLevelRange", 0 );
        retval = kPCS_Validation_FAIL;
    }
    pMission->iMinLevelAuto = iMinLevel;
    pMission->iMaxLevelAuto = iMaxLevel;

    if( ambush_cnt > ARCHITECT_AMBUSH_LIMIT )
    {
        playerCreatedStoryArc_AddError( estr, "ArchitectTooManyAmbushes", 0 );
        retval = kPCS_Validation_FAIL;
    }
    if( floor_cnt > mapLimit.floor[0] )
    {
        playerCreatedStoryArc_AddError( estr, "ArchitectTooManyFloorObjects", 0 );
        retval = kPCS_Validation_FAIL;
    }
    if( wall_cnt > mapLimit.wall[0] )
    {
        playerCreatedStoryArc_AddError( estr, "ArchitectTooManyWallObjects", 0 );
        retval = kPCS_Validation_FAIL;
    }
    if( regular_cnt > mapLimit.regular_only[0] + mapLimit.around_or_regular[0] - MIN(0, around_cnt - mapLimit.around_only[0])   )
    {
        playerCreatedStoryArc_AddError( estr, "ArchitectTooManyRegularSpawns", 0 );
        retval = kPCS_Validation_FAIL;
    }
    if( around_cnt > mapLimit.around_only[0] + mapLimit.around_or_regular[0] - MIN(0, regular_cnt - mapLimit.regular_only[0]) )
    {
        playerCreatedStoryArc_AddError( estr, "ArchitectTooManyAroundSpawns", 0 );
        retval = kPCS_Validation_FAIL;
    }
    if( giant_cnt > mapLimit.giant_monster )
    {
        playerCreatedStoryArc_AddError( estr, "ArchitectTooManyGiantMonster", 0 );
        retval = kPCS_Validation_FAIL;
    }

    return retval;
}

static void setDefaultContact(PlayerCreatedStoryArc * pArc)
{
    pArc->eContactType = kContactType_Critter;
    StructReallocString(&pArc->pchContactCategory, DEFAULT_VILLAIN_GROUP);
    StructReallocString(&pArc->pchContact, DEFAULT_CRITTER);
}

PlayerCreatedStoryarcValidation playerCreatedStoryarc_GetValid( PlayerCreatedStoryArc * pArc, char * src, char **estr, char **errors2, Entity *creator, int fixup)
{
    int i, key_cnt = 0, zipped_size, size;
    U8    *zip_data;
    PlayerCreatedStoryarcValidation retval = kPCS_Validation_OK;

    if( !src )
        src = playerCreatedStoryArc_ToString(pArc);

    size = (int)strlen(src);
    zipped_size = size*1.0125+12;
    zip_data = malloc(zipped_size);
    if( Z_OK != compress2(zip_data,&zipped_size,src,size,5) || zipped_size >= MAX_ARC_FILESIZE )
    {
        playerCreatedStoryArc_AddError( estr, "ArchitectFileTooLarge", 0 );
        retval = kPCS_Validation_FAIL;
    }
    free(zip_data);

    if(!locIsUserSelectable(pArc->locale_id))
    {
        playerCreatedStoryArc_AddError( estr, "ArchitectInvalidLocale", 0 );
        retval = kPCS_Validation_FAIL;
    }

    if( !eaSize(&pArc->ppMissions) )
    {
        playerCreatedStoryArc_AddError( estr, "ArchitectNoMissions", 0 );
        retval = kPCS_Validation_FAIL;
    }

    // Check Each Individual Field
    MAX1(retval, (tokenFieldsAreValid( ParsePlayerStoryArc, st_StoryArcRestrictions, pArc, estr, pArc->locale_id==getCurrentLocale(), fixup ))?kPCS_Validation_OK:kPCS_Validation_FAIL);

    // Higher Level Checks
    if( pArc->eContactType < 0 || pArc->eContactType >= kContactType_Count )
    {
        if(fixup)
        {
            playerCreatedStoryArc_AddError( errors2, "ArchitectInvalidContactType", 0 );
            MAX1(retval, kPCS_Validation_ERRORS);
            setDefaultContact(pArc);
        }
        else
        {
            playerCreatedStoryArc_AddError( estr, "ArchitectInvalidContactType", 0 );
            retval = kPCS_Validation_FAIL;
        }
    }

    if( pArc->eContactType == kContactType_Contact && !playerCreatedStoryArc_isValidContact(pArc->pchContactCategory, pArc->pchContact, creator) )
    {
        if(fixup)
        {
            playerCreatedStoryArc_AddError( errors2, "ArchitectInvalidContact", pArc->pchContact );
            MAX1(retval, kPCS_Validation_ERRORS);
            setDefaultContact(pArc);
        }
        else
        {
            playerCreatedStoryArc_AddError( estr, "ArchitectInvalidContact", pArc->pchContact );
            retval = kPCS_Validation_FAIL;    
        }
    }

    if( pArc->eContactType == kContactType_Critter && !playerCreatedStoryArc_isValidVillain(pArc->pchContactCategory, pArc->pchContact, creator) )
    {
        if(fixup)
        {
            playerCreatedStoryArc_AddError( errors2, "ArchitectInvalidContact", pArc->pchContact );
            MAX1(retval, kPCS_Validation_ERRORS);
            setDefaultContact(pArc);
        }
        if(!fixup || !playerCreatedStoryArc_isValidVillain(pArc->pchContactCategory, pArc->pchContact, creator))
        {
            playerCreatedStoryArc_AddError( estr, "ArchitectInvalidContact", pArc->pchContact );
            retval = kPCS_Validation_FAIL;    
        }
    }
    if( pArc->eContactType == kContactType_Object && !playerCreatedStoryArc_isValidObject(pArc->pchContact, creator) )
    {
        if(fixup)
        {
            playerCreatedStoryArc_AddError( errors2, "ArchitectInvalidContact", pArc->pchContact );
            MAX1(retval, kPCS_Validation_ERRORS);
            setDefaultContact(pArc);
        }
        else
        {
            playerCreatedStoryArc_AddError( estr, "ArchitectInvalidContact", pArc->pchContact );
            retval = kPCS_Validation_FAIL;    
        }
    }
    //    Temporary. I will eventually pass the entity down from higher up and use that.
    if( pArc->eContactType == kContactType_PCC )
    {
        int acceptibleContactErrors = PCC_LE_VALID | PCC_LE_INVALID_RANGED | PCC_LE_INVALID_PRIMARY_POWER | PCC_LE_INVALID_SECONDARY_POWER | 
            PCC_LE_INVALID_TRAVEL_POWER | PCC_LE_INVALID_RANK | PCC_LE_CONFLICTING_POWERSETS |PCC_LE_INVALID_DIFFICULTY | 
            PCC_LE_INVALID_DIFFICULTY2 | PCC_LE_INVALID_SELECTED_PRIMARY_POWERS | PCC_LE_INVALID_SELECTED_SECONDARY_POWERS;
        int playableError = 0;
        int outOfRange = !EAINRANGE(pArc->customContactId-1, pArc->ppCustomCritters) || !pArc->ppCustomCritters[pArc->customContactId-1];
        int pccError = outOfRange?0:playerCreatedStoryArc_isValidPCC( pArc->ppCustomCritters[pArc->customContactId-1], pArc, creator, fixup, &playableError );
        if(outOfRange)
        {
            if(fixup)
            {
                playerCreatedStoryArc_AddError( errors2, "ArchitectInvalidContact", pArc->pchContact );
                MAX1(retval, kPCS_Validation_ERRORS);
                setDefaultContact(pArc);
            }
            if(!fixup || !playerCreatedStoryArc_isValidVillain(pArc->pchContactCategory, pArc->pchContact, creator))
            {
                playerCreatedStoryArc_AddError( estr, "ArchitectInvalidContact", pArc->pchContact );
                retval = kPCS_Validation_FAIL;    
            }
        }
        else if(pccError &~acceptibleContactErrors)
        {
            retval = (pccError&~PCC_LE_INVALID_DESC)?kPCS_Validation_FAIL:kPCS_Validation_ERRORS;    
            if(!fixup || retval == kPCS_Validation_FAIL)
            {
                generatePCCErrorText_ex(pccError, estr);
                playerCreatedStoryArc_AddError( estr, "ArchitectInvalidContact", pArc->pchContact );    //I find this error very misleading.
            }
            else
            {
                generatePCCErrorText_ex(pccError, errors2);
            }
            if (fixup)
            {
                pArc->customContactId = 0;
                setDefaultContact(pArc);
            }
        }
        if(playableError)
            generatePCCErrorText_ex(playableError, errors2);
    }

    if( pArc->eMorality < 0 || pArc->eMorality >= kMorality_Count )
    {
        if(fixup)
        {
            pArc->eMorality = kMorality_Neutral;
            playerCreatedStoryArc_AddError( errors2, "ArchitectInvalidMorality", 0 );
            MAX1(retval, kPCS_Validation_ERRORS);
        }
        else
        {
            playerCreatedStoryArc_AddError( estr, "ArchitectInvalidMorality", 0 );
            retval = kPCS_Validation_FAIL;
        }
    }

    if( eaSize(&pArc->ppMissions) > 5 )
    {
        playerCreatedStoryArc_AddError( estr, "ArchitectInvalidTooManyMissions", 0 );
        retval = kPCS_Validation_FAIL;
    }

    for( i = 0; i < 32; i ++ )
    {
        if( pArc->keywords & (1<<i) )
            key_cnt++;
    }
    if( key_cnt > 3 )
    {
        if(fixup)
        {
            playerCreatedStoryArc_AddError( errors2, "ArchitectInvalidTooManyKeywords", 0 );
            MAX1(retval, kPCS_Validation_ERRORS);
        }
        else
        {
            playerCreatedStoryArc_AddError( estr, "ArchitectInvalidTooManyKeywords", 0 );
            retval = kPCS_Validation_FAIL;
        }
    }

    // Check all the missions
    for( i = 0; i < eaSize(&pArc->ppMissions); i++ )
        MAX1(retval, playerCreatedStoryarc_MissionIsValid(pArc, pArc->ppMissions[i], estr, errors2, creator, fixup ));

    return retval;
}


#if TEST_CLIENT
static int playerCreatedStoryArc_getFieldCharLimit(const char *fieldname, StashTable stash)
{
    PlayerCreatedStoryArcRestriction *pRestrict;
    if( stashFindPointer( stash, fieldname, &pRestrict ) )
        return pRestrict->iCharLimit;
    return 0;
}

int playerCreatedStoryArc_getArcFieldCharLimit(const char *fieldname, int notused)
{
    return playerCreatedStoryArc_getFieldCharLimit(fieldname, st_StoryArcRestrictions);
}
int playerCreatedStoryArc_getMissionFieldCharLimit(const char *fieldname, int notused)
{
    return playerCreatedStoryArc_getFieldCharLimit(fieldname, st_MissionRestrictions);
}
int playerCreatedStoryArc_getDetailFieldCharLimit(const char *fieldname, int type)
{
    return playerCreatedStoryArc_getFieldCharLimit(fieldname, st_DetailRestrictions[type]);
}

#endif


