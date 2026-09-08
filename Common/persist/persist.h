#pragma once

#include <utilitieslib/stdtypes.h>

typedef struct ParseTable ParseTable;

AUTO_ENUM;
typedef enum PersistType
{
    PERSIST_INVALID = -1,    EIGNORE

    PERSIST_MEMORY,            ENAMES(Memory)
    // memory-only, does no actual persistence
    // dirty, flush-start, and flush-sync do nothing
    // this can be used as a run-time dictionary

    PERSIST_FLAT,            ENAMES(Flat)
    // dirty does nothing
    // flush-start writes out entire db
    // flush-sync does nothing

    PERSIST_DIFF_BIG,        ENAMES(DiffBig)
    // dirty simply marks structs
    // flush-start journals dirty structs in a background thread and spawns a merge process (unless NOTHREADS is set)
    // flush-sync waits for the background thread to finish

    PERSIST_DIFF_SMALL,        ENAMES(DiffSmall)
    // dirty writes the struct to the journal
    // flush-start spawns a merge process
    // flush-sync waits for the merge process

//     PERSIST_SQL,            ENAMES(SQL)
//     // dirty queues struct for persistence
//     // flush-start does nothing
//     // flush-sync waits for queue to empty

    PERSIST_COUNT,            EIGNORE
} PersistType;

// these can be serialized in a config, and are suitable to have end users change them
AUTO_ENUM;
typedef enum PersistFlags
{
    PERSIST_DEFAULT            = (0),
    PERSIST_CRCDB            = (1<<0),    ENAMES(ChecksumDb)            // for text file dbs, use textcrcdb
    PERSIST_CRCJOURNAL        = (1<<1),    ENAMES(ChecksumJournal)        // not implemented! // for text journals, use textcrcjournal
    PERSIST_NOTHREADS        = (1<<2),    ENAMES(DisableThreads)
    PERSIST_EXPONENTIAL        = (1<<3),    ENAMES(BackupDbExponential)    // exponential backups for text dbs
    PERSIST_KEEPLASTJOURNAL    = (1<<4),    ENAMES(BackupJournalLast)
    PERSIST_KEEPALLJOURNALS    = (1<<5),    ENAMES(BackupJournalAll)
    PERSIST_DONTSYNCMERGES    = (1<<6),    ENAMES(DontSyncMerges)
} PersistFlags;

AUTO_STRUCT AST_STARTTOK("") AST_ENDTOK("end");
typedef struct PersistConfig
{
    char *configfile;            AST(CURRENTFILE)

    PersistType        type;        AST(DEFAULT(PERSIST_INVALID))
    PersistFlags    flags;        AST(FLAGS DEFAULT(PERSIST_DEFAULT))
    int                flush_time;    AST(NAME("flushtime"))
    int                fail_time;    AST(NAME("failtime"))
//    int serve_on_port;

    // PERSIST_FLAT, PERSIST_DIFF_BIG, PERSIST_DIFF_SMALL
    char *fname;                AST(NAME("file"))
    char *structname;            // defaults to struct name

    // PERSIST_DIFF_BIG

    // PERSIST_DIFF_SMALL
    U32 max_journal_size;        AST(DEFAULT(100*1024*1024)) // 0 = don't roll over by size
    int roll_journal_tries;        // 0 = retry forever

    // PERSIST_SQL
    char *server;
    char *user;
    char *pass;
    char *db;
    char *table;

} PersistConfig;

// these aren't serialized, they require code changes to change
typedef enum PersistParams
{
//     PERSIST_LOCKEVERYTHING    = (1<<0),    // code makes no lock calls, persist layer treats everything as locked
//     PERSIST_ALWAYSDIRTY        = (1<<1),    // code makes no dirty/unlock calls, assume all structs are always dirty
//     PERSIST_IMPLICITDIRTY    = (1<<2),    // code makes no dirty/unlock calls, persist layer keeps internal copies of locked structs for diffing
    PERSIST_ORDERED            = (1<<3),    // code expects plGetAll to return a list with consistent order, deletion is slower
    PERSIST_STRICTDEFAULT    = (0),        // this is the fastest, but requires the most code
//     PERSIST_LAZYDEFAULT        = (PERSIST_LOCKEVERYTHING|PERSIST_ALWAYSDIRTY),
//     PERSIST_LAZYDEFAULT        = (PERSIST_LOCKEVERYTHING|PERSIST_IMPLICITDIRTY),    // requires double the memory
} PersistParams;

typedef int (*JournalPlayer)(void *lineptr); // returns true on error

typedef struct PersistSettings
{
    ParseTable *tpi;
    size_t size;
    PersistParams params;

    // custom journal settings (PERSIST_DIFF_SMALL)
    ParseTable *linetpi;        // for use with plJournal, describes the journal line structure
    size_t linesize;
    JournalPlayer journalplayer;
    int assert_on_line_error;

} PersistSettings;


// if you need to have two different databases for the same type, you should probably make a new typedef for that type,
// so you'll be able to keep the type-checking and the lighter interface


// Operations on a single persist type
#define plRegisterQuick(type, tpi, config, params) \
                                                persist_RegisterQuick(#type, sizeof(type), tpi, config, params)
#define plRegisterJournaled(type, tpi, linetpi, linesize, journalplayer, config, params) \
                                                persist_RegisterJournaled(#type, sizeof(type), tpi, linesize, linetpi, journalplayer, config, params)
#define plRegister(type, settings, config)        persist_RegisterType(#type, settings, config)
#define plLoad(type)                            persist_Load(#type, 0, NULL, NULL)
#define plCount(type)                            persist_Count(#type)
#define plCountSafe(type)                        persist_CountSafe(#type)
#define plGetAll(type)                            (type**)persist_GetAll(#type)
#define plFind(type, ...)                        (type*)persist_Find(#type, __VA_ARGS__)
#define plFindOrAdd(type, ...)                    (type*)persist_FindOrAdd(#type, __VA_ARGS__)
#define plAdd(type, ...)                        (type*)persist_Add(#type, __VA_ARGS__)
#define plNew(type, ...)                        (type*)persist_New(#type, __VA_ARGS__)
#define plFindAndDelete(type, ...)                persist_FindAndDestroy(#type, __VA_ARGS__)
#define plDelete(type, structptr)                persist_RemoveAndDestroy(#type, 0 ? (type*)(0) : (structptr))
#define plDeleteList(type, structptrs, count)    persist_RemoveAndDestroyList(#type, 0 ? (type**)(0) : (structptrs), count)
#define plDirty(type, structptr)                persist_Dirty(#type, 0 ? (type*)(0) : (structptr), NULL)
#define plResetAutoInrcrement(type)                persist_ResetAutoIncrement(#type)
#define plJournal(type, structptr, lineptr)        persist_Dirty(#type, 0 ? (type*)(0) : (structptr), lineptr)
#define plFlushStart(type, quitting)            persist_FlushStart(#type, quitting)
#define plIsFlushing(type)                        persist_IsFlushing(#type)
#define plFlushSync(type)                        persist_FlushSync(#type)


// Operations on all registered persist types
#define plLoadAll()                                persist_LoadAll()
#define plTick()                                persist_FlushAll(PERSIST_TICK)
#define plMergeAllSync()                        persist_FlushAll(PERSIST_MERGEALLSYNC)
#define plShutdown()                            persist_FlushAll(PERSIST_SHUTDOWN)
#define plPanic()                                persist_Panic()


// Special operations

#define plMergeToDisk(type)                        persist_Load(#type, 1, NULL, NULL)
#define plMergeByName(typeName)                    persist_Load(typeName, 1, NULL, NULL)
// as plLoad, but also flushes any changes from journals to disk

#define plConvertFromConfig(type, config_was)    persist_Load(#type, 0, config_was, NULL)
// as plLoad, but loads according to the configuration 'config_was' and immediately writes to disk according to the registered 'config'
// currently, this is a little leaky.  if we ever need to use this without exiting immediately after, the leak will need to be fixed.

#define plConvertFromStructs(type, structptrs)    persist_Load(#type, 0, NULL, structptrs)
// as plLoad, but uses the supplied object list 'structptrs' and immediately writes to disk according to the registered 'config'


// you probably don't want to call these directly...
typedef enum PersistFlushAll
{
    PERSIST_TICK,
    PERSIST_MERGEALLSYNC,
    PERSIST_SHUTDOWN,
    PERSIST_FLUSHALLTYPES
} PersistFlushAll;

int        persist_RegisterType(const char *type, PersistSettings *settings, PersistConfig *config); // settings is shallow copied, config is deep copied
int        persist_RegisterQuick(const char *type, size_t size, ParseTable *tpi, PersistConfig *config, PersistParams params); // shorthand
int        persist_RegisterJournaled(const char *type, size_t size, ParseTable *tpi, size_t linesize, ParseTable *linetpi, JournalPlayer journalplayer, PersistConfig *config, PersistParams params); // shorthand for custom journaling
int        persist_Load(const char *type, int merging, PersistConfig *config_was, void ***structptrs);
int        persist_Count(const char *type);
int        persist_CountSafe(const char *type);
void**    persist_GetAll(const char *type);
void*    persist_Find(const char *type, ...);                            // returns NULL if not exists
void*    persist_FindOrAdd(const char *type, ...);                        // for non-auto-incrementing keys, always returns something
void*    persist_Add(const char *type, ...);                                // for non-auto-incrementing keys, returns NULL if already exists
void*    persist_New(const char *type, ...);                                // for auto-incrementing keys, pass in non-autoincrement fields
int        persist_FindAndDestroy(const char *type, ...);                    // returns 1 if found
void    persist_RemoveAndDestroy(const char *type, void *structptr);
void    persist_RemoveAndDestroyList(const char *type, void **structptrs, int count);
void    persist_Dirty(const char *type, void *structptr, void *lineptr);// specifying lineptr will do custom journaling for backends that support it
int        persist_ResetAutoIncrement(const char *type);                    // returns the highest current auto-incrementing key
void    persist_FlushStart(const char *type, int quitting);                // quitting does a fast flush, safe for exit but not while running
int        persist_IsFlushing(const char *type);
void    persist_FlushSync(const char *type);
int        persist_LoadAll(void);
void    persist_FlushAll(PersistFlushAll type);
int        persist_Panic(void);                                            // dumps all dbs to disk and does anything else necessary for a catastrophic event
void    persist_http_page(char **str, char *path, char **keys, char **values, int count, char *fragment);
void    persist_http_summary(char **str, char **keys, char **values, int count);

#if !defined(NO_TEXT_PARSER) && !defined(UNITTEST)
#include "AutoGen/persist_h_ast.h"
#endif

