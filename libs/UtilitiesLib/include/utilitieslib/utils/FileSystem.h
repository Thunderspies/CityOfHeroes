#ifndef COX_FILE_SYSTEM_H
#define COX_FILE_SYSTEM_H

#include <piggle/piggle.h>

#ifdef __cplusplus
extern "C" {
#endif

/* The creating thread owns source attachment, discovery, polling and tree
 * teardown. Lookup, prepared enumeration, reads and owned-result cleanup may
 * run directly on workers. Stop tree users before destroying their group;
 * retained selections, listings and readers survive group destruction.
 */
typedef struct FileSystem FileSystem;
typedef struct FileSelection FileSelection;

/* CoX resolution modes. Development requires a loose counterpart; dynamic
 * discovers on demand. Hybrid permits archive-only files. Archive/loose modes
 * exclude the other storage kind. Initial process mode is LOOSE.
 */
typedef enum FileSystemMode {
	FILE_MODE_DEVELOPMENT,
	FILE_MODE_DYNAMIC,
	FILE_MODE_HYBRID,
	FILE_MODE_ARCHIVES,
	FILE_MODE_LOOSE
} FileSystemMode;

/* Owned snapshot metadata. Strings borrow the selection or listing. path is
 * canonical relative to the group, name its basename; native_path is a loose
 * filename or "archive:/entry". size is logical bytes, mtime Unix seconds.
 * kind/attributes/format use Piggle constants; directories have size zero.
 */
typedef struct FileSystemEntry {
	const char *path;
	const char *name;
	const char *native_path;
	pg_id source_id;
	uint64_t size;
	int64_t mtime;
	uint32_t kind;
	uint32_t attributes;
	uint32_t format;
} FileSystemEntry;

/* Immutable owned listing in canonical lexical order. Safe to read on any
 * thread; release it with fileListingFree. Strings borrow this listing.
 */
typedef struct FileListing {
	size_t count;
	const FileSystemEntry *entries;
} FileListing;

/* Create an empty group; NULL on allocation failure. First create
 * establishes the main callback thread and must run there. Destroy requires
 * the creating thread and exclusive caller ownership, releases sources and
 * queued changes, and clears *system. NULL addresses/handles are accepted.
 * Open readers survive destroy.
 */
FileSystem *fileSystemCreate(void);
void fileSystemDestroy(FileSystem **system);

/* Add an existing directory/archive. Higher nonnegative priority wins within
 * each kind. Equal priorities within a kind -> EXISTS. Add loose roots before
 * archives; mixed ties preserve CoX timestamp rules. Copies path, no initial
 * notifications. Returns Piggle status; no partial attachment on failure.
 */
pg_status fileSystemAddSource(FileSystem *system, const char *path, int priority);

/* Select a file or directory using the process mode and ignore rules. Accept
 * leading separators and ./ for engine-relative asset names. NULL on absence
 * or error. The owned selection retains its exact copy; free independently.
 * entry/info borrow it; info is NULL for directories. Neither accessor does I/O.
 */
FileSelection *fileSystemSelect(FileSystem *system, const char *path);
/* Copy metadata without capturing content. out is required; path/name in out
 * are NULL. Optional native_path storage receives the resolved name and is
 * borrowed by out->native_path. CAPACITY leaves out empty. Requested indexes
 * remain stable during a callback pause.
 */
pg_status fileSystemQuery(FileSystem *system, const char *path,
	FileSystemEntry *out, char *native_path, size_t capacity);
const FileSystemEntry *fileSelectionEntry(const FileSelection *selection);
const pg_file_info *fileSelectionInfo(const FileSelection *selection);
void fileSelectionFree(FileSelection **selection);

/* Open selected content once without retargeting, returning an owned reader.
 * representation is PG_READ_LOGICAL/STORED. out required, NULL on failure.
 * Directory -> CONFLICT; stale content -> STALE. Release with pg_reader_close.
 * Native readers keep their opened object across rename, unlink or replacement;
 * later selections see namespace changes. Observable object mutation is STALE.
 */
pg_status fileSelectionOpen(const FileSelection *selection,
	uint32_t representation, pg_reader **out);

/* Open an explicit native or archive-qualified path independently of group
 * resolution. Same ownership as selection open; always logical bytes. Native
 * source identity is captured once. path/out required; output NULL on failure.
 */
pg_status fileSystemOpenPath(const char *path, pg_reader **out);

/* On the control thread, discover immediate children; workers require a
 * prepared covering subtree. Capture effective entries, including empty
 * directories. NULL/empty prefix means root, excluded from result. Ignored
 * components are omitted. NULL on failure; absent prefix -> empty listing.
 * archive_fast permits archive-only results in DYNAMIC mode for quickload.
 * Caller can prune by choosing which child directories to list next.
 */
FileListing *fileSystemList(FileSystem *system, const char *prefix,
	int archive_fast);

/* Prepare recursive coverage before queuing worker enumeration. Control
 * thread only; does not change archive_fast resolution policy. NULL/empty
 * prefix means root. Returns Piggle status (BUSY on an incorrect thread).
 * Repeated preparation during a loading pause reuses the retained index.
 */
pg_status fileSystemPrepareTree(FileSystem *system, const char *prefix,
	int archive_fast);

/* During a loading pause, capture a recursive subtree once for subsequent
 * queries and walks. Outside a pause this is a no-op; normal observation and
 * listing refresh remain active. archive_fast uses the same policy as List.
 * NULL/empty prefix means root. Returns Piggle status.
 */
pg_status fileSystemCacheTree(FileSystem *system, const char *prefix,
	int archive_fast);

/* Independent native-directory snapshot, without asset policy/ignore rules.
 * Returns immediate children. Same ownership as group listings. Input path
 * must exist; NULL on failure. Release on any thread; NULL addresses accepted.
 */
FileListing *fileSystemListNative(const char *path);
void fileListingFree(FileListing **listing);

/* Global policy configuration; choose DYNAMIC for development data and HYBRID
 * otherwise. Set mode before attaching sources. Get is thread-safe.
 */
void fileSystemSetMode(FileSystemMode mode);
FileSystemMode fileSystemGetMode(void);
void fileSystemChooseMode(void);

/* Parser's additional prefixes, terminated by NULL, process lifetime. Ignore
 * rules compare each component's prefix without case; initial ignore is d_.
 * Add copies a nonempty string; remove is idempotent. Standard adds d_ only;
 * v_ remains a valid parser variant and asset path. Query accepts NULL (not
 * ignored), does not mutate state.
 */
extern const char *const file_standard_prefixes[];
void fileSystemIgnoreAdd(const char *prefix);
void fileSystemIgnoreRemove(const char *prefix);
void fileSystemIgnoreStandard(void);
int fileSystemIgnored(const char *path);

/* Disable loose overrides for packaged assets for the remainder of the
 * process. Exclude skips matching loose directory components; only_root
 * instead includes just the named top-level directory. NULL clears exclusion.
 */
void fileSystemDisallowOverrides(void);
void fileSystemExclude(const char *directory, int only_root);

/* UPDATE includes creation; DELETE provides the previous metadata. SHARED is
 * a subscription flag permitting registration while shared memory is enabled.
 */
enum {
	FILE_CHANGE_UPDATE = 1u,
	FILE_CHANGE_DELETE = 2u,
	FILE_CHANGE_SHARED = 4u
};
typedef struct FileChange {
	FileSystem *system;
	uint32_t kind;
	FileSystemEntry entry;
} FileChange;
/* Main-thread callback; change and spans borrow through return. File reads
 * and subscriptions are allowed; recursive dispatch is a no-op.
 */
typedef void (*FileChangeHandler)(const FileChange *change, void *user);

/* Subscribe to copied case-insensitive pattern (at most one '*'). NULL group
 * observes all groups; user may be NULL and borrows until process exit.
 * Requires UPDATE and/or DELETE, non-NULL callback. Returns Piggle status.
 * Suppressed shared-memory subscriptions succeed without registering.
 */
pg_status fileSystemSubscribe(FileSystem *system, const char *pattern,
	uint32_t flags, FileChangeHandler callback, void *user);

/* Pause observation/dispatch and reuse cached metadata, content identities,
 * and discovered listings during loading. Resume processes accumulated
 * notices. Opening captured content still checks for stale identity.
 */
void fileSystemCallbacksEnabled(int enabled);

/* Poll group-owned watches and dispatch coalesced transitions on the first
 * creator's thread. Other threads and recursive calls are no-ops. Last event
 * per subscription/name wins. Returns first observation error. Loss rescans
 * affected known scopes; undiscovered scopes stay lazy until requested.
 */
pg_status fileSystemDispatch(void);

#ifdef __cplusplus
}
#endif
#endif
