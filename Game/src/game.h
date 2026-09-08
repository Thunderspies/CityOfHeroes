// support functions for main.c

#ifndef _GAME_H_
#define _GAME_H_

#include <utilitieslib/stdtypes.h>

typedef enum
{
    PROGRESSDIALOGTYPE_NONE = -1,
    PROGRESSDIALOGTYPE_OK,                // Prompts OK
    PROGRESSDIALOGTYPE_SAFEMODE,        // Prompts YES/NO for safemode
} PROGRESSDIALOGTYPE;

void parseArgs(int argc,char **argv);

// these are in the order they will typically be called
void changeShellCamera( float x, float y, float z );
void parseArgsForCovFlag(int argc, char **argv);
int getProjectKey(int argc, char **argv);
S32 game_runningCohClientCount(void);
void game_beforeFolderCacheIgnore(int timer, int argc, char **argv);
void parseArgs0(int argc, char **argv);
void game_beforeParseArgs(int doLogging);
void game_reapplyGfxSettings(void);
int game_loadSoundsTricksFonts(int argc, char **argv);
void game_initWindow(int maximize);
void game_networkStart(void);
void game_loadData(int isCostumeCreator);
void game_beforeLoop(int isCostumeCreator, int timer);
int game_mainLoop(int timer);
void updateGameTimers(void);
int game_shutdown(void);
bool game_validateChecksums(bool bForceFullVerify, bool *pCancelled);

// The userMessageID is the message ID of the string to show if the game crashes.  Set to NULL to clear it.
void game_setProgressString(const char *progressString, const char *userMessageID, PROGRESSDIALOGTYPE type);

void game_setAssertSystemExtraInfo(void);

#endif//_GAME_H_
