
#ifndef PLAYERCREATEDSTORYARCVALIDATE_H
#define PLAYERCREATEDSTORYARCVALIDATE_H

#include "storyarc/playerCreatedStoryarcTypes.h"
#include "gameComm/VillainDef.h"
#include <utilitieslib/components/StashTable.h>

typedef struct PictureBrowser PictureBrowser;
typedef struct ComboBox ComboBox;
typedef struct ToolTip ToolTip;
typedef struct MMElementList MMElementList;
typedef struct SMFBlock SMFBlock;
typedef struct Costume Costume;
typedef struct MissionMapStats MissionMapStats;
typedef struct PCC_Critter PCC_Critter;
typedef struct PlayerCreatedStoryArc PlayerCreatedStoryArc;
typedef struct HelpButton HelpButton;
typedef struct CompressedCVG CompressedCVG;
typedef struct CustomNPCCostume CustomNPCCostume;

typedef enum MapLimitType
{
    kMapLimit_All  = 0,
    kMapLimit_Front,
    kMapLimit_Middle,
    kMapLimit_Back,
}MapLimitType;

typedef enum PlayerCreatedStoryarcValidation
{
    kPCS_Validation_OK = 0,
    kPCS_Validation_ERRORS,        //this has errors, but we can play it.
    kPCS_Validation_FAIL,
    kPCS_Validation_COUNT,        
}PlayerCreatedStoryarcValidation;

AUTO_STRUCT;
typedef struct MapLimit
{
    int regular_only[4];
    int around_only[4];
    int around_or_regular[4];
    int giant_monster;
    int wall[4];
    int floor[4];
    int defeat_all;
}MapLimit;

AUTO_STRUCT;
typedef struct MMElement
{
    char *pchDisplayName;

    char *pchText; // This is the actual value sent to conversion
    char *pchToolTip;
    int bActive;
    int minLevel;
    int maxLevel;

    MMElementList **ppElementList; // sub list ownership

    // Other info?
    char * pchDescription;
    char * pchVGDescription;
    char * pchPowerDescription;
    char * pchAlias;
    char * pchSeq;

    int npcIndex;
    int *npcIndexList;
    int animationUpdate;
    int setnum;
    int bColorable;

    SMFBlock * pBlock; NO_AST

    char *mapImageName; 
    MapLimit maplimit; 

    PCC_Critter *pCritter;
    int doppelFlags;
    CustomNPCCostume *pNPCCostume; NO_AST
    CustomNPCCostume *plastNPCCostume; NO_AST
    Costume *tempCostume; NO_AST

}MMElement;


AUTO_STRUCT;
typedef struct MMElementList
{
    char *pchName;
    char *pchTemplate;
    SpecialAction eSpecialAction;

    MMElement **ppElement;

    int current_element;
    int updated; AST(DEFAULT(1))

    char *pchToolTip;
    char *pchDefault; // Storage for the actual text
    char *pchField;
    char *pchAmbushLoad; // load saved ambush value into here so we can pick it out of list later
    char *pchDestinationLoad;
    char *pchReplaceNoneWith; // Change the text on a none element

    int bCheckBoxList;
    int bButton;
    int bCopyrightCheck;
    int bTabbedSelection;
    int bKeywordSelector;
    int bRequired;
    int bFieldRestrictionAlreadyDefined;

    int bDontUnlockCheck; // this is confusing name

    int AllowHTML;
    int CharLimit; // -1 means number only
    int ValMin;
    int ValMax;

    MMElementList **ppMultiList;
    int current_list;
    int previous_list;

    PCC_Critter *SelectedCritter;
    int reasonForInvalid;
    int doneCritterValidation;

    HelpButton *pHelp; NO_AST
    SMFBlock *pBlock; NO_AST
    void *pCrossLinkPointer; NO_AST
    void *pNameLinkPtr; NO_AST
    void *pGroupLinkPtr; NO_AST
}MMElementList;


AUTO_STRUCT;
typedef struct MMRegionButton
{
    char *pchName;
    char *pchSpecialAction;

    MMElementList **ppElementList;

    char *pchPageName;
    char *pchToolTip;
    int isOpen;
    int bOptional;

    int bFirstUse;

    F32 wd;
    F32 ht;

    HelpButton *pHelp; NO_AST

}MMRegionButton;

typedef struct MMRegion MMRegion;
typedef struct MMScrollSet MMScrollSet;
typedef struct MMScrollSet_Mission MMScrollSet_Mission;


AUTO_STRUCT;
typedef struct MMRegion
{
    char *pchName;
    char *pchDisplayName;
    char *pchUserName;
    char *pchActualDisplay;

    MMElementList ** ppElementList;
    MMRegionButton ** ppButton;

    char *pchStruct;
    char *pchToolTip; 
    char *pchIcon;
    char *pchIconGlow;
    PictureBrowser **pb; NO_AST

    F32 outer_ht;
    F32 inner_ht;
    F32 inner_wd;
    int isOpen;
    int bUndeleteable;
    PlayerCreatedDetailType detailType;
    int limit; 

    // Back Pointers
    MMScrollSet *pParentSet; NO_AST
    MMScrollSet_Mission *pParentMission; NO_AST

    HelpButton *pHelp; NO_AST
    SMFBlock * pBlock; NO_AST
    F32 fOffset; NO_AST // static for expanding contracting bar

}MMRegion;


AUTO_STRUCT;
typedef struct MMScrollSet_Loader
{
    MMRegion ** ppRegion;
    MMRegionButton ** ppButton;

}MMScrollSet_Loader;

extern MMScrollSet_Loader mmScrollSet_template;
// --- Data validation Files ----------------------------------------------------------------------------------------

typedef struct PNPCDef PNPCDef;
typedef struct MMContact
{
    char * pchNPCDef;
    const PNPCDef* pDef;
}MMContact;

typedef struct MMContactCategory
{
    char *pchCategory;
    MMContact ** ppContact;
}MMContactCategory;

typedef struct MMContactCategoryList
{
    MMContactCategory ** ppContactCategories;
    StashTable st_ContactCategories;
    StashTable st_Contacts;
}MMContactCategoryList;

extern MMContactCategoryList gMMContactCategoryList;
extern int MMdoppelFlagSets[];

typedef struct MMVillainGroup MMVillainGroup;
typedef struct MMVillainGroup
{
    char *pchName;
    const char *pchDisplayName;
    int minLevel;
    int maxLevel;
    int minGroupLevel;
    int maxGroupLevel;
    VillainGroupEnum vg;
    const char *pchDescription;
    char *pchExclude;
    MMVillainGroup ** ppGroup;
}MMVillainGroup;

typedef struct MMVillainGroupList
{
    MMVillainGroup ** group;
    char ** ppIncluded;
    char ** ppCantMoveNames;
    StashTable st_Included;
    StashTable st_GroupNames;
    cStashTable st_CantMoveEnts;
}MMVillainGroupList;

extern MMVillainGroupList gMMVillainGroupList;

typedef struct MMAnim
{
    char *pchName;
    char *pchDisplayName;
}MMAnim;

typedef struct MMAnimList
{
    MMAnim ** anim;
    StashTable st_Anim;
}MMAnimList;

extern MMAnimList gMMAnimList;

typedef struct MMObject
{
    char *pchName;
    char *pchDisplayName;
}MMObject;

typedef struct MMObjectPlacement
{
    char *pchName;
    char *pchDisplayName;
    MMObject **object;
}MMObjectPlacement;


typedef struct MMObjectList
{
    MMObjectPlacement ** placement;
    StashTable st_Object;
}MMObjectList;

extern MMObjectList gMMObjectList;

typedef struct MMDestructableObject
{
    char *pchName;
}MMDestructableObject;

typedef struct MMDestructableObjectList
{
    MMDestructableObject** object;
    StashTable st_Object;
}MMDestructableObjectList;

extern MMDestructableObjectList gMMDestructableObjectList;

typedef struct MMMapSet
{
    char * pchName;
    char * pchDisplayName;
}MMMapSet;

typedef struct MMMapSetList
{
    MMMapSet** mapset;
    StashTable st_MapSets;
}MMMapSetList;

extern MMMapSetList gMMMapSetList;

typedef struct MMUniqueMap
{
    char * pchName;
    char * pchMapFile;
}MMUniqueMap;

typedef struct MMUniqueMapCategory
{
    char* pchName;
    MMUniqueMap** maplist;
}MMUniqueMapCategory;

typedef struct MMUniqueMapCategoryList
{
    MMUniqueMapCategory** mapcategorylist;
    StashTable st_UniqueMaps;
}MMUniqueMapCategoryList;

extern MMUniqueMapCategoryList gMMUniqueMapCategoryList;

typedef struct MMUnlockableContent
{
    char * pchName;
    char * pchTokenName;
}MMUnlockableContent;

typedef struct MMUnlockableContentList
{
    MMUnlockableContent **ppUnlockableContent;
    StashTable st_UnlockableContent;
}MMUnlockableContentList;
typedef struct MMUnlockableCostumeSetContent
{
    char **costume_keys;
    char *pchTokenName;
}MMUnlockableCostumeSetContent;
typedef struct MMUnlockableCostumeContentList
{
    StashTable st_UnlockableContent;
}MMUnlockableCostumeContentList;

extern MMUnlockableContentList gMMUnlockableContentList;

typedef struct MMNonSelectableEntities
{
    const char **ppNames;
}MMNonSelectableEntities;

typedef struct MMSequencerAnimList
{
    char ** ppSeq;
    char **ppValidAnimLists;
    int index;
    StashTable st_Seq;
    StashTable st_Anim;
}MMSequencerAnimList;

typedef struct MMSequencerAnimLists
{
    MMSequencerAnimList ** ppLists;
    StashTable st_SeqAnimLists;
}MMSequencerAnimLists;

extern MMSequencerAnimLists gMMSeqAnimLists;


#define MAX_ARC_FILESIZE 40000.f
//the following is the maximum size of the string.  
//Otherwise, you could have extremely large string size do to formatting that gets accepted.
#define MAX_ARC_STRINGSIZE (MAX_ARC_FILESIZE*2)

typedef struct MMRewardRank
{
    char **pchRanks;
    int *values;
    F32 *chances;
}MMRewardRank;

typedef struct MMRewards
{
    F32 *MissionCompleteBonus;
    char **ppBonusRequires;

    F32 *MissionCompleteBonus1;
    char **ppBonus1Requires;

    F32 *MissionCompleteBonus2;
    char **ppBonus2Requires;

    F32 *lowLevelPenalty;
    F32 *highLevelBonus;

    MMRewardRank **ppRewardRank;

    int    MissionCap;
    F32 TicketsPerSpawnCap;
    int TicketCap;
}MMRewards;

extern MMRewards gMMRewards;

#define ARCHITECT_AMBUSH_LIMIT 6


void playerCreatedStoryArc_GenerateData(void);
void playerCreatedStoryarc_LoadData(int create_bins);
PlayerCreatedDetailType playerCreatedStoryarc_GetValid( PlayerCreatedStoryArc * pArc, char * src, char **error, char **errors2, Entity *creator, int fixup );


char *playerCreatedStoryarc_getMapSetDisplayName( const char * pchMapSet );
int playerCreatedStoryarc_isValidVillainDefName( const char * pchName, Entity *e );
int playerCreatedStoryarc_isCantMoveVillainDefName(  const char * pchName );
int playerCreatedStoryArc_IsCostumeKey( const char *unlockKey, MMUnlockableCostumeSetContent **pCostumeSet);
int playerCreatedStoryarc_isUnlockedContent( const char * pchName, Entity * e );
char* playerCreatedStoryarc_basicSMFValidate(const char *pchName, const char * pchText, int maxCharacters );
int playerCreatedStoryArc_isValidVillain( const char * pchVillainGroup, const char * pchName,  Entity *e );
int playerCreatedStoryArc_isValidPCC( PCC_Critter *pcc, PlayerCreatedStoryArc *pArc, Entity *e, int fixup, int *playableError);
int playerCreatedStoryArc_isValidCVG( CompressedCVG *cvg, PlayerCreatedStoryArc *pArc, Entity *e, int *error, int fixup, int *playableErrorCode);

void playerCreatedStoryarc_clearDetailNodeList();
int playerCreatedStoryArc_addDetailNodeLink(void * pDetail, void *pDetailNext, void *pMission);
int playerCreatedStoryarc_isAnimAllowedOnSeq( char * pchSeq, char *pchAnim );
int playerCreatedStoryArc_CheckUnlockKey(Entity *e, const char *pchUnlockKey);

char *smf_EncodeAllCharactersGet(const char *str);
char *smf_DecodeAllCharactersGet(const char *str);

MMVillainGroup* playerCreatedStoryArc_GetVillainGroup( const char * pchName, int set );
void playerCreatedStoryarcValidate_setDefaultEnemy(char **group, char **enemy, int boss);

#if TEST_CLIENT
int playerCreatedStoryarc_isValidVillainGroup( const char * pchName, Entity * e );
int playerCreatedStoryArc_getArcFieldCharLimit(const char *fieldname, int notused);
int playerCreatedStoryArc_getMissionFieldCharLimit(const char *fieldname, int notused);
int playerCreatedStoryArc_getDetailFieldCharLimit(const char *fieldname, int type);
#endif

void ArchitectEnteredVolume( Entity * e );
void ArchitectExitedVolume( Entity * e );
int ArchitectCanEdit( Entity * e );
#endif
