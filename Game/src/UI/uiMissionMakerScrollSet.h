
#ifndef UIMISSIONMAKERSCROLLSET_H
#define UIMISSIONMAKERSCROLLSET_H


#include "UI/uiScrollBar.h"
#include "storyarc/playerCreatedStoryarcValidate.h"
#include <utilitieslib/utils/Cbox.h>

typedef struct MMElementList MMElementList;

AUTO_STRUCT;
typedef struct LevelRestrict
{
    int min;
    int max;
}LevelRestrict;

AUTO_STRUCT;
typedef struct MMScrollSet_Mission
{
    int minLevel; 
    int maxLevel; 

    int minRestrictedLevel;
    int maxRestrictedLevel;

    LevelRestrict **ppRestrict; 

    MapLimit mapCount; 
    MapLimit *pMapLimit; NO_AST

    MMRegion **ppMissionRegion;
    MMRegion **ppDetailRegion;
    MMRegionButton **ppDetailButton;

    MMScrollSet *pSetParent; NO_AST
    int bReportedLevelRange; NO_AST
    int    bFailable; NO_AST

}MMScrollSet_Mission;

typedef struct MMScrollSet
{
    // Per Story Arc Info
    int dirty;
    F32 wd; 
    F32 ht; 
    F32 page_wd; 


    char *pchFilename;
    int arc_id;
    int owned; // editing an arc that the player has published, only relevant with arc_id

    int bShowTips; 

    int current_page;
    int current_mission;
    int current_region;

    int bShowErrors;
    int bShowWrongness;
    int bWarnings;
    F32 fErrorBlinkTimer;
    F32 filesize;
    int keywordpicks;

    F32 fSaveMessageTimer;

    int scrollsetOpen;

    ScrollBar leftSb;    
    ScrollBar rightSb;
    ScrollBar errorSb;

    MMElementList *pTargetList; 
    
    F32 fBlinkTimer; 
    F32 fTargetY; 
    int converge; 

    MMRegion **ppStoryRegion;
    MMScrollSet_Mission **ppMission;
    CBox bounds;

} MMScrollSet;

extern MMRegion **ppDetailTemplates;


void MMScrollSet_Load(void);
void MMScrollSet_New(MMScrollSet *pSet);

MMScrollSet_Mission* MMScrollSet_AddMission(MMScrollSet *pSet);
void MMScrollSet_deleteMission(MMScrollSet_Mission * pMission);
MMRegion* MMScrollSet_AddDetailRegion(MMScrollSet_Mission *pMission, int type);

char * MMScrollSet_getValueFromRegion( MMRegion * pRegion, char * pchKey, int displayName);
void MMScrollSet_updateAmbushTrigger( MMScrollSet_Mission *pMission );
void MMScrollSet_SetCrossLinks( MMRegion * pRegion );
void MMScrollSet_NameSet( MMRegion *pRegion );
void MMScrollSet_mapViewerInit(MMElementList * pList);

int MMScrollSet_ValidLevel( MMScrollSet_Mission *pMission, int min, int max, int removemin, int removemax );
int MMScrollSet_isStoryArcValid( MMScrollSet *pSet, int log_errs );
int MMScrollSet_isMissionValid( MMScrollSet_Mission *pMission, int page, int log_errs );
int MMScrollSet_isRegionValid( MMScrollSet_Mission * pSet, MMRegion * pRegion, int log_errs );
int MMScrollSet_isElementListValid( MMScrollSet_Mission * pMission, MMElementList * pList, int recurse, int log_errs, int * only_level_wrong, int *sub_has_level );
int MMScrollSet_IsReady( MMScrollSet *pSet );
void MMScrollSet_GetMissionLevelRange(MMScrollSet_Mission * pMission, int *outmin, int *outmax );

void updateCustomCritterList(MMScrollSet *pSet);
void updateCustomCrittersElementList( MMElementList *pList );
typedef struct PCC_Critter PCC_Critter;
void setSelectionToCritter(MMElementList *pList, PCC_Critter* returnCritter);
F32 MMScrollSet_Draw( MMScrollSet * pSet, F32 x, F32 y, F32 z, F32 ht, F32 sc, int clr );
void MMtoggleArchitectLocalValidation(int mode);
typedef enum MMIgnoreValidationFlags
{
    MM_IGNORE_OFF = 0,
    MM_IGNORE_ON = 1,
    MM_IGNORE_FROM_CONSOLE = 1 << 1,
}MMIgnoreValidationFlags;
#endif
