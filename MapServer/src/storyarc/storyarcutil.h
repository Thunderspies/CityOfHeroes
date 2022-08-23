/*\
 *
 *    storyarcutil.h/c - Copyright 2003, 2004 Cryptic Studios
 *        All Rights Reserved
 *        Confidential property of Cryptic Studios
 *
 *    this file provides several utility functions for
 *  areas related to story systems
 *
 */

#ifndef __STORYARCUTIL_H
#define __STORYARCUTIL_H

#include <stdlib.h>
#include "storyarcinterface.h"
#include <utilitieslib/language/MultiMessageStore.h>
#include <utilitieslib/utils/file.h>
#include "entity/entPlayer.h"

typedef struct ScriptVarsTable ScriptVarsTable;
typedef struct ParseTable ParseTable;
#define TokenizerParseInfo ParseTable

// Give the correct villain prefix for text localization
#define VILLAIN_PREFIX(player) (ENT_IS_VILLAIN(player) ? "v_" : "")

// Most of the storyarc related systems place files in the data/scripts.loc directory tree.
// These functions simplify handling of references within this directory
void saUtilCleanFileName(char buffer[MAX_PATH], const char* source);    // change to a relative path within scripts dir
bool saUtilVerifyCleanFileName(const char* source);
char* saUtilDirtyFileName(const char* source); // change back to an absolute path
void saUtilFilePrefix(char buffer[MAX_PATH], const char* source); // just the bare filename with no extension or path
#define SCRIPTS_DIR "SCRIPTS.LOC"

// TODO: clean this up
#include "group/groupnetsend.h"
#define saUtilCurrentMapFile currentMapFile 

// NOTE: all saUtilLocalize functions might return pointer to static buffer, so result must be used before calling again
const char* saUtilLocalize(const Entity *e, const char* string, ...);
    // just for static localized strings, uses story arc static string message store
const char* saUtilScriptLocalize(const char* string);
    // localize a string that is given in a script file, without any variable lookups
const char* saUtilTableLocalize(const char* string, ScriptVarsTable* table);
    // the newest localization function.  deals exclusively with vartables and supercedes xxString and xxContactString

char *saUtilEntityVariableReplacement( const char * in_str, Entity * e );
    // this was added for the mission architect, it does basic variable replacement
    // $name
    // $class
    // $origin
    // $supergroup
    // $level

const char * saUtilTableAndEntityLocalize( const char * string, ScriptVarsTable* table, Entity * e );
    // combines saUtilTableLocalize and saUtilEntityVariableReplacement

// This function will process all filenames through saUtilCleanFileName and will
// check vars syntax (must use SCRIPTVARS_STD_FIELD)
bool saUtilPreprocessScripts(TokenizerParseInfo pti[], void* structptr); // THIS MAY ASSERT

// verify that story arc strings don't have any of the following auto-parameters
void saUtilVerifyContactParams(const char* string, const char* filename, int allowGeneral, int allowSuperGroupName, int allowPlayerInfo, int allowAccept, int allowLastContactName, int allowRescuer);

// generally useful stuff
#define ENT_FIND_LIMIT 10000
#define SA_SAY_DISTANCE    150    // all dialog from NPC's and villians is heard within this distance
int saUtilPlayerWithinDistance(const Mat4 pos, F32 distance, Entity** result, int velpredict);
int saUtilVillainWithinDistance(const Mat4 pos, F32 distance, Entity** result, int velpredict);

typedef enum EntSayFlags
{
    kEntSay_NoFlag,            // nothing special
    kEntSay_NPC,            // is an NPC so use that channel
    kEntSay_PetCommand,        // A pet saying standard command
    kEntSay_PetSay,            // A pet saying something its owner told it to
    kEntSay_NPCDirect,        // NPC says only to target
}EntSayFlags;

void saUtilEntSays(Entity* ent, int flags, const char* dialog);
void saUtilCaption(const char* caption);
void saUtilReplaceChars(char* string, char from, char to); // replaces all instances of <from>

// make random rolls
void saSeed(unsigned int seed); // just seed it
unsigned int saSeedRoll(unsigned int seed, unsigned int mod);
    // seed the random generator, then return a number from [0 - (mod-1)]
unsigned int saRoll(unsigned int mod);
    // as above, without reseeding the generator

// These stubs are for traversing group trees in the standard fashion
// (noop stubs for tree traversal)
void* noopContextCreate();
void noopContextCopy(void** src, void** dst);
void noopContextDestroy(void* context);

////////////////////////////////////////////////////////////// TextParams
// MAK 6/28/4 - TextParams used to exist to work around the message store not using
// ScriptVars directly.  This has been changed, and TextParams removed

////////////////////////////////////////////////////////////////////////////
// private to story arc system
void saUtilPreload();

char* saUtilGetPowerName(PowerNameRef* power);
char* saUtilGetBasePowerName(const BasePower* basePower);

extern MultiMessageStore* g_storyarcMessages;

// *********************************************************************************
//  Random Fame Strings - a place to put strings on a player, may be said by npcs later
// *********************************************************************************

void RandomFameAdd(Entity* player, const char* famestring);
void RandomFameTick(Entity* player);
void RandomFameMoved(Entity* player);
void RandomFameWipe(Entity* player);

//perhaps this is in the wrong file
void handleFutureChatQueue( Entity * e );

int parseDialogForBubbleTags( const char * dialog, char * animation, const char ** dialogToSayNowPtr, const char ** dialogToSayLaterPtr, F32 * timePtr, 
                             F32 *durationPtr, char * face, int * shout, int *isCaption, F32 *captionPosition, char * gotoLoc, char *animList);

void saBuildScriptVarsTable(ScriptVarsTable        *svt,                    // Table* to add our scopes to - should be not NULL
                            const Entity        *ent,                    // Entity* to draw name, gender, alignment from - can be NULL, in which case ent_db_id is used instead
                            int                    ent_db_id,                // db_id to draw name, gender, alignment from if ent == NULL, unless 0 in which case they are not filled
                            ContactHandle        contactHandle,            // ContactHandle to draw contact name and var scope from - can be 0 (treated as NULL)
                            const StoryTaskInfo *sti,                    // StoryTaskInfo* to draw arc/parent/task/mission var scope from, as well as taskRandom fields and seed - can be NULL
                                                                        //   the seed here supercedes seed field (for convenience's sake) - can be NULL
                            const StoryTask        *st,                    // StoryTask* to draw arc/parent/task/mission var scope from if sti == NULL - can be NULL
                            const StoryTaskHandle *sahandle,             // StoryTaskHandle* to draw arc/parent/task/mission var scope from if sti == NULL && st == NULL - can be NULL
                            const StoryArcInfo    *sai,                    // StoryArcInfo* to draw arc var scope and seed from if there is no current assigned task - can be NULL - sti supercedes
                            const ContactTaskPickInfo    *pickInfo,        // ContactTaskPickInfo* to draw arc/parent/task/mission var scope and random villainGroup from - trumps st & sahandle - can be NULL
                            VillainGroupEnum    villainGroup,            // VillainGroupEnum to draw random villainGroup from if no other source available
                            unsigned int        seed,                    // seed to use if sti == NULL - can be 0
                            bool                permanent);                // true if this table is intended for permanent use (use SV_COPYVALUE) and false otherwise

#endif // __STORYARCUTIL_H