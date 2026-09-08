#ifndef STORYARCCOMMON_H
#define STORYARCCOMMON_H

#include <utilitieslib/utils/structDefines.h>


typedef enum MissionPlaceEnum {
    MISSION_NONE,
    MISSION_FRONT,
    MISSION_BACK,
    MISSION_MIDDLE,
    MISSION_OBJECTIVE,
    MISSION_ANY,
    MISSION_SCRIPTCONTROLLED,                // not actually a place on the map, these are for scripts to refer to
    MISSION_REPLACEGENERIC,            // not actually a place on the map, these spawns are used instead of usual generics
    MISSION_LOBBY,
} MissionPlaceEnum;



extern StaticDefineInt ParseMissionPlaceEnum[];
//------------------------------------------------------------------
//------------------------------------------------------------------
// StoryClue
//------------------------------------------------------------------
//------------------------------------------------------------------
typedef struct StoryClue {
    char* filename;
    char* name;
    char* displayname;
    char* introstring;
    char* detailtext;
    char* iconfile;
} StoryClue;

//-------------------------------------------------------------
// Constructor/Destructors
//-------------------------------------------------------------
StoryClue* StoryClueCreate();
void StoryClueDestroy(StoryClue* clue);


//------------------------------------------------------------------
//------------------------------------------------------------------
// SouvenirClue
//------------------------------------------------------------------
//------------------------------------------------------------------
typedef struct SouvenirClue 
{
    int uid;
    const char* filename;
    const char* name;

    // "Header" info
    const char* displayName;
    const char* icon;

    const char* description;    // Complete text associated with the clue.  May be long.
} SouvenirClue;

//-------------------------------------------------------------
// Constructor/Destructors
//-------------------------------------------------------------
SouvenirClue* scCreate();



// *********************************************************************************
//  Difficulty
// *********************************************************************************

typedef struct StoryDifficulty
{
    int levelAdjust; // ( -1 to +4 )
    int teamSize;         // (1 to 8 )
    int alwaysAV;  // it true, don't downgrade AV to EB, otherwise always do
    int dontReduceBoss; // if true, don't downgrade bosses to Lt while solo
}StoryDifficulty;

#endif