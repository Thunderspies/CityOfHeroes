#pragma once

#include "persist.h"
#include "utilitieslib/components/StashTable.h" // StashKeyType

typedef struct BackendInfo BackendInfo; // individually defined for each backend

typedef enum KeyType
{
    KEY_VOID,    // no keys, single object
    KEY_INT,
    KEY_STRING,
    KEY_MULTI,    // more than one key
} KeyType;

typedef enum DirtyType
{
    DIRTY_CLEAN,
    DIRTY_DBONLY,
    DIRTY_ROW,
} DirtyType;

AUTO_ENUM;
typedef enum PersistLoadState
{
    PERSISTINFO_NOTLOADED,
    PERSISTINFO_LOADING,
    PERSISTINFO_CLEAN,
    PERSISTINFO_DIRTY,
    PERSISTINFO_QUITTING,

    PERSISTINFO_ERROR,    // this could be multiple states instead...
} PersistLoadState;

AUTO_ENUM;
typedef enum PersistMergeState
{
    PERSISTINFO_NOTMERGING,
    PERSISTINFO_MERGEPENDING,
    PERSISTINFO_MERGING,
} PersistMergeState;

typedef union RowInfo
{
    struct {
        unsigned idx        : 30;
        unsigned dirty        : 1;
        unsigned readonly    : 1;
    };
    int i;
} RowInfo;
STATIC_ASSERT(sizeof(RowInfo) == sizeof(int));

typedef struct PersistInfo
{
    char *typeName;
    PersistSettings settings;    // code-dependent setting... feel free to rename these two
    PersistConfig *config;        // user-tweakable config... feel free to rename these two
    PersistLoadState state;
    PersistMergeState merging;
    int merge_ticked;
    int merge_thread;    // id of the currently-running thread that's merging this db, used to sync with the thread
    int timer;
    int merge_recovery;            // used to check if a previous merge failed and is currently being recovered
    KeyType keytype;
    int *key_idxs;
    int key_autoinc;    // key that requires auto-incrementing
    int key_size;        // size for MULTI key
    int key_flat;        // MULTI key, bytes are contiguous at the beginning of the struct with no indirection
    intptr_t key_last;    // last autoincrement-generated key
    void **structptrs;
    StashTable lookup;    // indexed by keytype
    StashTable rows;
    BackendInfo *backend;
} PersistInfo;

unsigned int persist_hashStruct(PersistInfo *info, const void *structptr, int hashSeed);
int persist_compStruct(PersistInfo *info, const void *struct1, const void *struct2);

intptr_t persist_getKey(PersistInfo *info, void *structptr);
intptr_t persist_dupKey(PersistInfo *info, void *structptr);
void persist_freeKey(PersistInfo *info, intptr_t key);

void persist_startMerge(PersistInfo *info);

// these won't try to do any persistance
void persist_mergeReplace(PersistInfo *info, void *structptr); // takes ownership of structptr
void persist_mergeDelete(PersistInfo *info, intptr_t key);
void persist_mergeLastKey(PersistInfo *info, int last_key);
void persist_mergeRemoveJournal(PersistInfo *info, char *fname);

#include "AutoGen/persist_internal_h_ast.h"
