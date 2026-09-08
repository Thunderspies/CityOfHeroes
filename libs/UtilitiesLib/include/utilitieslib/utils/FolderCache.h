/***************************************************************************
*     Copyright (c) 2003-2006, Cryptic Studios
*     All Rights Reserved
*     Confidential Property of Cryptic Studios
*
* Module Description:
*   System to keep track of a directory in memory that is scanned from
*   both a set of Pig files and from a set of game data dirs. Relies on
*   dirMonitor for real-time updating (WinNT only)
* 
***************************************************************************/

#pragma once
#ifndef _FOLDERMONITOR_H
#define _FOLDERMONITOR_H

#include <time.h>
#include "piglib.h"
#include "wininclude.h"

C_DECLARATIONS_BEGIN

typedef struct StashTableImp *StashTable;
typedef const struct StashTableImp *cStashTable;

#define MAX_IGNORE_PREFIXES 50

typedef struct FolderNode    FolderNode;
typedef struct FolderCache    FolderCache;

typedef struct FolderNode {
    // next and prev must be first because our standard link lists are used.
    FolderNode*    next;
    FolderNode*    prev;
    
    S32            virtual_location:16;    // <0 -> Piggs, >=0 -> a game data dir // larger ABS is higher virtual_location
    U32            is_dir:1;                // Is a folder/directory.
    U32            writeable:1;            // Is writeable (i.e. not read-only).
    U32            hidden:1;                // Is hidden.
    U32            system:1;                // System flag is set.
    
    U32            seen_in_fs:1;            // Used for development_dynamic hack to add all pigs and verify they're in the FS later
    U32            in_hash_table:1;        // Set when the node is added to htAllFiles.
    U32            delete_if_absent:1;        // Set when scanning for nonexistent nodes.
    int         file_index;             // index to the file in the pigfile specified by the virtual_location
    FolderNode*    contents;                // NULL if !is_dir;  linked list of all of the children nodes
    FolderNode*    contents_tail;            // NULL if !is_dir;  tail end of the linked list of children
    FolderNode*    parent;                    // NULL if root
    char*        name;                    // name, relative to parent node
    size_t        size;
    __time32_t    timestamp;
} FolderNode;

//#define MAX_DIRS 95 // This limits how many different game data dirs and how many different pig files //TODO: make this dynamic!
//#define NUM_PRIORITIES (MAX_DIRS*2)
//#define GDD_TO_INDEX(x) (x + MAX_DIRS)


typedef void (*FolderCacheNodeCreateDeleteCallback)(FolderCache* fc, FolderNode* node);

typedef struct FolderCache {
    FolderNode *root;
    FolderNode *root_tail;
    CRITICAL_SECTION critSecAllFiles; // Critical section around hashtable calls
    StashTable htAllFiles; // Hash table of pointers to FolderNodes
    char **gamedatadirs; // Map of "priorities" to the game data dir that is related
    char **piggs; // Map of "priorities" to the pig file that is related
    
    FolderCacheNodeCreateDeleteCallback nodeCreateCallback;
    FolderCacheNodeCreateDeleteCallback nodeDeleteCallback;
    
    S32    needsRescan;

    S32 scanCreatedFolders;
} FolderCache;

#define PIG_INDEX_TO_VIRTUAL_LOCATION(pi)  (-(pi+1))
#define VIRTUAL_LOCATION_TO_PIG_INDEX(vl)  ((-vl)-1)

// Maps "priorities" to the game data dir/pig file that is related
char *FolderCache_GetFromVirtualLocation(FolderCache *fc, int virtual_location);
void FolderCache_SetFromVirtualLocation(FolderCache *fc, int virtual_location, char *path);
int FolderCache_GetNumGDDs(FolderCache *fc);
int FolderCache_GetNumPiggs(FolderCache *fc);

typedef enum {
    FOLDER_CACHE_MODE_DEVELOPMENT, // Filesystem is "master", only load from a Pig if the file exists in the file system and it's the same timestamp
    FOLDER_CACHE_MODE_DEVELOPMENT_DYNAMIC, // Same as above, but don't initially scan the tree, only do so as needed...  this is sorta a hack.  For folders that are not scanned with fileScanAllDataDirs, this won't do anything better than FILESYSTEM_ONLY
    FOLDER_CACHE_MODE_I_LIKE_PIGS, // Equal rights for pigs.  Use files from the pigs even if they don't exist in the File System (hybride mode)
    FOLDER_CACHE_MODE_PIGS_ONLY, // Pigs are master, do not load from file system in any circumstances (in fact, don't even scan the file system)
    FOLDER_CACHE_MODE_FILESYSTEM_ONLY, // Do not cache the tree, do not load pigs
} FolderCacheMode;

typedef enum {
    FOLDER_CACHE_ALL_FOLDERS,
    FOLDER_CACHE_EXCLUDE_FOLDER,
    FOLDER_CACHE_ONLY_FOLDER,
} FolderCacheExclusion;

FolderCache *FolderCacheCreate(void);
void FolderCacheDestroy(FolderCache *fc);

void FolderCacheHashTableAdd(FolderCache* fc, const char* name, FolderNode* node);
FolderNode* FolderCacheHashTableRemove(FolderCache* fc, const char* name);

void FolderCacheSetMode(FolderCacheMode mode);
FolderCacheMode FolderCacheGetMode(void);
void FolderCacheChooseMode(void); // Auto-chooses the appropriate mode, based on existence of ./piggs

void FolderCacheEnableCallbacks(int enable); // Disables/enables checking for changes in the filesystem (turn these off when loading)

void FolderCacheExclude(FolderCacheExclusion mode, const char *folder);

void FolderCacheSetFullScanCreatedFolders(FolderCache* fc, int enabled);

enum {
    FOLDER_CACHE_CALLBACK_UPDATE    = (1<<0), // Called on add or update
    FOLDER_CACHE_CALLBACK_DELETE    = (1<<1), // Called on delete
    FOLDER_CACHE_CALLBACK_UPDATE_AND_DELETE = (1<<2)-1,
    FOLDER_CACHE_CALLBACK_CAN_USE_SHARED_MEM    = (1<<2), // if this is not set, callback is disabled when sharedmemory enabled
};
typedef void (*FolderCacheCallback)(const char *relpath, int when);
typedef void (*FolderCacheCallbackEx)(FolderCache* fc, FolderNode* node, const char *relpath, int when);
// Sets it up so that a callback will be called when a file with a given filespec
// matches  (uses simpleMatch, i.e. only 1 '*' allowed)
void FolderCacheSetCallback(int when, const char *filespec, FolderCacheCallback callback);
void FolderCacheSetCallbackEx(int when, const char *filespec, FolderCacheCallbackEx callback);
void printFilenameCallback(const char *relpath, int when); // example/test callback
void FolderCacheDoCallbacks(void);
void FolderCacheSetManualCallbackMode(int on); // Call this if you're calling FolderCacheDoCallbacks each tick


// Add a folder from the filesystem into the virtual tree
void FolderCacheAddFolder(FolderCache *fc, const char *basepath, int virtual_location);
int FolderCacheFindFolder(FolderCache *fc, const char *basepath );    // returns the virtual location or -1 if not found
// Add a Pig's files into the virtual tree.  This must be called *AFTER* all calls to 
//   FolderCacheAddFolder, because Pigged files can only be added if they already exist
//   in the file system (unless we're in production mode, then Pig files are used
//   exclusively).  Returns non-zero upon error
int FolderCacheAddAllPigs(FolderCache *fc);

// Add new piggs (called when piggs are downloaded DURING gamelay)
int FolderCacheAddNewPigs(FolderCache *fc);

// Dynamic hack functions:
void FolderCacheRequestTree(FolderCache *fc, const char *relpath); // If we're in dynamic mode, this will load the tree!
void FolderCacheModePush(FolderCacheMode newmode); // Another hack for dynamic mode
void FolderCacheModePop(void); // Another hack for dynamic mode

int FolderCacheSetDirMonTimeout(int timeout);
void FolderCacheSetNoDisableOnBufferOverruns(int value);

FolderNode *FolderCacheQuery(FolderCache *fc, const char *relpath);
char *FolderCacheGetRealPath(FolderCache *fc, FolderNode *node, char *buffer, size_t buffer_size); // pass a buffer to fill

extern const char* g_StdAdditionalFilePrefixes[];    // standard known prefixes for the textparser to additionally load
void FolderCacheIgnoreStdPrefixes(void);
void FolderCacheAddIgnorePrefix(const char *prefix);
void FolderCacheRemoveIgnorePrefix(const char *prefix);
bool FolderCacheMatchesIgnorePrefixStart(const char *path); // only checks for prefix at the beginning of path
bool FolderCacheMatchesIgnorePrefixAnywhere(const char *path); // checks for prefix at any directory in path
char ** FolderCacheGetIgnoredPrefixes(int *num);

void FolderCacheDisallowOverrides(void); // Disallows any override files

FolderNode *FolderNodeCreate(void);
void FolderNodeDestroy(FolderCache* fc, FolderNode *node);
FolderNode *FolderNodeAdd(FolderNode **head, FolderNode **tail, FolderNode *parent, const char *relpath, __time32_t timestamp, size_t size, int virtual_location, int file_index, int* createdOut); // returns the newly added node // orignode is a sibling that the new node should be attatched to, parent is what the parent field should be set to (the parent is not modified, unless, of course, &parent->contents is passed in as orignode)
char *FolderNodeGetFullPath_s(FolderNode *node, char *buf, size_t buf_size); // Includes prefixed slash, pass a buffer to fill
#define FolderNodeGetFullPath(node, buf) FolderNodeGetFullPath_s(node, buf, ARRAY_SIZE_CHECKED(buf))
FolderNode *FolderNodeFind(FolderNode *base, const char *relpath);
void FolderNodeDeleteFromTree(FolderCache* fc, FolderNode *node); // Only on leaves
int FolderNodePigIndex(FolderNode *node);

// Return 0 to prune a tree, 1 to keep parsing, and 2 for a complete stop
typedef int (*FolderNodeOp)(FolderNode *node);
int FolderNodeRecurse(FolderNode *node, FolderNodeOp op);

// Return 0 to prune a tree, 1 to keep parsing, and 2 for a complete stop
typedef int (*FolderNodeOpEx)(const char *dir, FolderNode *node, void *userdata);
int FolderNodeRecurseEx(FolderNode *node, FolderNodeOpEx op, void *userdata, const char *base); // Pass NULL to base if we're operating on the root, or you don't care about getting the path up to the current node

// Debug/performance info
void FolderNodeGetCounts(int *discard, int *update);
int FolderNodeVerifyIntegrity(FolderNode *node, FolderNode *parent);

extern FolderCache *folder_cache;

C_DECLARATIONS_END

#endif
