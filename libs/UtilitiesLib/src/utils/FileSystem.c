#include "utilitieslib/utils/FileSystem.h"
#include "utilitieslib/components/SharedMemory.h"
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#undef strcpy
#undef strcat

#define POLICY_BUCKETS 1024
typedef struct Source Source;
typedef struct Scope Scope;
typedef struct Policy Policy;
typedef struct Subscription Subscription;
typedef struct Pending Pending;
typedef struct Retired Retired;

/* Coverage and tie history contain no namespace metadata or file handles. */
struct Scope {
	Scope *next;
	char *prefix;
	uint32_t depth;
	uint64_t order;
	unsigned phase;
};
struct Source {
	Source *next;
	pg_source *handle;
	pg_tree *tree;
	pg_source_info info;
	int priority;
	uint64_t order;
	Scope *scopes;
};
struct Policy {
	Policy *next;
	char *path;
	Source *source;
	uint64_t order, signature;
	pg_id exact;
	int seen;
};
struct FileSystem {
	FileSystem *next;
	pg_context *context;
	Source *sources;
	SRWLOCK lock;
	DWORD control_thread;
	Policy *policy[POLICY_BUCKETS];
	unsigned source_kinds;
	uint64_t order;
	int resume;
	unsigned phase;
};
struct FileSelection {
	FileSystemEntry entry;
	pg_file_info info;
	pg_file *file;
};
typedef struct Choice {
	int owned;
	Source *source;
	FileSystemEntry entry;
	pg_file *file;
	uint64_t order;
} Choice;
struct Subscription {
	Subscription *next;
	FileSystem *system;
	char *pattern;
	uint32_t flags;
	FileChangeHandler callback;
	void *user;
};
struct Pending {
	Pending *next;
	Subscription *subscription;
	FileChange change;
};
struct Retired {
	Retired *next;
	pg_context *context;
};

/* Policy locks never enclose Piggle traversal, discovery, or callbacks.
 * Source descriptors remain live until exclusive, quiescent group teardown.
 * The process list and subscriptions are owned by the first control thread.
 */
static SRWLOCK config_lock = SRWLOCK_INIT;
static SRWLOCK retired_lock = SRWLOCK_INIT;
static volatile LONG current_mode = FILE_MODE_LOOSE;
static volatile LONG callbacks_enabled = 1;
#define mode ((FileSystemMode)InterlockedCompareExchange(&current_mode, 0, 0))
static DWORD main_thread;
static int dispatching;
static FileSystem *systems;
static Subscription *subscriptions;
static Pending *pending;
static Pending *dispatch_events;
static Retired *retired;
static int overrides_disabled, exclude_only;
static char *excluded;
static char **ignored;
static size_t ignored_count;
const char *const file_standard_prefixes[] = {"v_", "d_", "", NULL};
#ifndef FINAL
extern int fileIsUsingDevData(void);
#else
static int fileIsUsingDevData(void) { return 0; }
#endif

static int control(FileSystem *system)
{ return system && GetCurrentThreadId() == system->control_thread; }
static unsigned metadata_hash(const char *path)
{
	unsigned hash = 2166136261u;
	while (*path) hash = (hash ^ (unsigned char)*path++) * 16777619u;
	return hash & (POLICY_BUCKETS - 1);
}
static void entry_clear(FileSystemEntry *entry)
{
	free((void *)entry->path);
	memset(entry, 0, sizeof(*entry));
}

static int entry_copy(FileSystemEntry *out, const FileSystemEntry *in)
{
	size_t path = strlen(in->path) + 1;
	size_t native = strlen(in->native_path) + 1;
	char *storage = malloc(path + native);
	if (!storage) return 0;
	*out = *in;
	memcpy(storage, in->path, path);
	memcpy(storage + path, in->native_path, native);
	out->path = storage;
	out->native_path = storage + path;
	out->name = strrchr(out->path, '/');
	out->name = out->name ? out->name + 1 : out->path;
	return 1;
}

static int make_entry(
	FileSystemEntry *out, Source *source, const pg_entry_info *in)
{
	size_t path = strlen(in->canonical_name) + 1;
	size_t native =
		strlen(source->info.native_path) + strlen(in->original_name) + 3;
	char *storage = malloc(path + native);
	if (!storage) return 0;
	memcpy(storage, in->canonical_name, path);
	char *filename = storage + path;
	strcpy(filename, source->info.native_path);
	strcat(filename, source->info.format == PG_LOOSE ? "/" : ":/");
	strcat(filename, in->original_name);
	memset(out, 0, sizeof(*out));
	out->path = storage;
	out->native_path = filename;
	out->name = strrchr(storage, '/');
	out->name = out->name ? out->name + 1 : storage;
	out->source_id = in->source_id;
	out->size = in->size;
	out->mtime = in->mtime;
	out->kind = in->kind;
	out->attributes = in->attributes;
	out->format = source->info.format;
	return 1;
}

static char *normalize(pg_context *context, const char *path)
{
	char *copy, *out;
	size_t required;
	if (!path) path = "";
	while (*path == '/' || *path == '\\' ||
		(path[0] == '.' && (path[1] == '/' || path[1] == '\\'))) {
		path += *path == '.' ? 2 : 1;
	}
	copy = _strdup(path);
	if (!copy) return NULL;
	for (out = copy; *out; out++)
		if (*out == '\\') *out = '/';
	while (out > copy && out[-1] == '/') *--out = 0;
	if (!*copy) return copy;
	if (pg_name_normalize(context, copy, NULL, 0, &required, NULL) !=
		PG_CAPACITY) {
		free(copy);
		return NULL;
	}
	out = malloc(required);
	if (!out ||
		pg_name_normalize(context, copy, out, required, &required, NULL) !=
			PG_OK) {
		free(out);
		out = NULL;
	}
	free(copy);
	return out;
}

static int ignored_unlocked(const char *path)
{
	if (!path) return 0;
	for (;;) {
		for (size_t i = 0; i < ignored_count; i++)
			if (!_strnicmp(path, ignored[i], strlen(ignored[i]))) return 1;
		path += strcspn(path, "/\\");
		if (!*path) return 0;
		path++;
	}
}

static int component(const char *path, const char *part)
{
	size_t n = strlen(part);
	for (;;) {
		if (!_strnicmp(path, part, n) && (!path[n] || path[n] == '/')) return 1;
		path = strchr(path, '/');
		if (!path) return 0;
		path++;
	}
}

static int loose_unlocked(const char *path)
{
	if (excluded) {
		if (exclude_only) {
			size_t n = strlen(excluded);
			if (_strnicmp(path, excluded, n) || (path[n] && path[n] != '/'))
				return 0;
		} else if (component(path, excluded))
			return 0;
	}
	if (mode != FILE_MODE_HYBRID || fileIsUsingDevData()) return 1;
	return !overrides_disabled && !component(path, "scenes") &&
		!component(path, "ent_types") && !component(path, "object_library") &&
		!component(path, "geobin");
}

static void choice_clear(Choice *choice)
{
	pg_file_close(&choice->file, NULL);
	if (choice->owned) entry_clear(&choice->entry);
	memset(choice, 0, sizeof(*choice));
}

static int under(const char *path, const char *prefix)
{
	size_t length = strlen(prefix);
	return !length ||
		(!strncmp(path, prefix, length) &&
			(!path[length] || path[length] == '/'));
}

static void scopes_clear(Source *source)
{
	while (source->scopes) {
		Scope *scope = source->scopes;
		source->scopes = scope->next;
		free(scope->prefix);
		free(scope);
	}
}

static int scope_covers(const Scope *scope, const char *path, int depth)
{
	if (!under(path, scope->prefix)) return 0;
	if (scope->depth == PG_DISCOVER_RECURSIVE) return 1;
	return depth == PG_DISCOVER_CHILDREN && !strcmp(path, scope->prefix);
}

static int is_ignored(const char *path)
{
	AcquireSRWLockShared(&config_lock);
	int result = ignored_unlocked(path);
	ReleaseSRWLockShared(&config_lock);
	return result;
}
static int loose_allowed(const char *path)
{
	AcquireSRWLockShared(&config_lock);
	int result = loose_unlocked(path);
	ReleaseSRWLockShared(&config_lock);
	return result;
}
static Source **sources_copy(FileSystem *system, Source **local, size_t *count)
{
	AcquireSRWLockShared(&system->lock);
	*count = 0;
	for (Source *s = system->sources; s; s = s->next) (*count)++;
	Source **items = *count > 8 ? malloc(*count * sizeof(*items)) : local;
	if (items) {
		size_t i = 0;
		for (Source *s = system->sources; s; s = s->next) items[i++] = s;
	}
	ReleaseSRWLockShared(&system->lock);
	return items;
}
static pg_status discover(
	FileSystem *system, Source *source, const char *prefix, uint32_t depth)
{
	if (!prefix) prefix = "";
	AcquireSRWLockShared(&system->lock);
	int covered = 0, fresh = 0;
	for (Scope *s = source->scopes; s; s = s->next)
		if (scope_covers(s, prefix, depth)) {
			covered = 1;
			fresh |= s->phase == system->phase;
		}
	ReleaseSRWLockShared(&system->lock);
	if (covered && (callbacks_enabled || fresh || !control(system)))
		return PG_OK;
	if (!control(system)) {
		OutputDebugStringA("FileSystem: unprepared worker enumeration\n");
		return PG_BUSY;
	}
	pg_status status = callbacks_enabled
		? pg_tree_discover(source->tree, prefix, depth, NULL)
		: pg_source_discover(source->handle, prefix, depth, NULL);
	if (status != PG_OK) return status;
	AcquireSRWLockExclusive(&system->lock);
	for (Scope *s = source->scopes; s; s = s->next)
		if (s->depth == depth && !strcmp(s->prefix, prefix)) {
			s->phase = system->phase;
			ReleaseSRWLockExclusive(&system->lock);
			return PG_OK;
		}
	ReleaseSRWLockExclusive(&system->lock);
	Scope *scope = calloc(1, sizeof(*scope));
	if (!scope) return PG_NOMEM;
	scope->prefix = _strdup(prefix);
	if (!scope->prefix) {
		free(scope);
		return PG_NOMEM;
	}
	scope->depth = depth;
	AcquireSRWLockExclusive(&system->lock);
	scope->phase = system->phase;
	scope->order = ++system->order;
	scope->next = source->scopes;
	source->scopes = scope;
	ReleaseSRWLockExclusive(&system->lock);
	return PG_OK;
}
static Policy *policy_find(FileSystem *system, const char *path, Source *source)
{
	for (Policy *item = system->policy[metadata_hash(path)]; item;
		item = item->next)
		if (item->source == source && !strcmp(item->path, path)) return item;
	return NULL;
}

static Policy *policy_add(FileSystem *system, const char *path, Source *source)
{
	Policy *item = policy_find(system, path, source);
	if (item) return item;
	item = calloc(1, sizeof(*item));
	if (!item) return NULL;
	item->path = _strdup(path);
	if (!item->path) {
		free(item);
		return NULL;
	}
	item->source = source;
	unsigned bucket = metadata_hash(path);
	item->next = system->policy[bucket];
	system->policy[bucket] = item;
	return item;
}

static uint64_t order_unlocked(
	FileSystem *system, Source *source, const char *path)
{
	if (system->source_kinds != 3) return source->order;
	Policy *item = policy_find(system, path, source);
	if (item && item->order) return item->order;
	uint64_t order = source->order;
	if (source->info.format != PG_LOOSE || mode == FILE_MODE_DEVELOPMENT ||
		mode == FILE_MODE_HYBRID)
		return order;
	order = system->order + 1;
	for (Scope *scope = source->scopes; scope; scope = scope->next) {
		if (!under(path, scope->prefix)) continue;
		const char *tail = path + strlen(scope->prefix);
		if (*tail == '/') tail++;
		if (scope->depth == PG_DISCOVER_RECURSIVE || !strchr(tail, '/'))
			if (scope->order < order) order = scope->order;
	}
	return order;
}

static uint64_t source_order(
	FileSystem *system, Source *source, const char *path)
{
	AcquireSRWLockShared(&system->lock);
	uint64_t result = order_unlocked(system, source, path);
	ReleaseSRWLockShared(&system->lock);
	return result;
}
static int replaces(const Choice *best, const Choice *copy)
{
	if (!best->source) return 1;
	int native = copy->entry.format == PG_LOOSE;
	if (copy->entry.kind == PG_ENTRY_DIRECTORY)
		return best->entry.kind != PG_ENTRY_DIRECTORY ||
			(native &&
				(copy->source->priority == 0 ||
					best->entry.format != PG_LOOSE ||
					(best->source->priority != 0 &&
						copy->source->priority > best->source->priority)));
	if (best->entry.kind == PG_ENTRY_DIRECTORY) return 0;
	if (native == (best->entry.format == PG_LOOSE))
		return copy->source->priority >= best->source->priority;
	/* Subtract only after checking the direction to avoid signed overflow. */
	int equal_hour = copy->entry.mtime >= best->entry.mtime &&
		(uint64_t)copy->entry.mtime - (uint64_t)best->entry.mtime == 3600;
	int equal = copy->entry.mtime == best->entry.mtime || equal_hour;
	return native ? !equal : equal;
}

static int choice_order(const void *left, const void *right)
{
	const Choice *a = left, *b = right;
	return a->order < b->order ? -1 : a->order > b->order;
}

static int eligible(Source *source, const char *path)
{
	return source->info.format == PG_LOOSE
		? mode != FILE_MODE_ARCHIVES && loose_allowed(path)
		: mode != FILE_MODE_LOOSE;
}

static uint64_t policy_signature(const FileSystemEntry *entry)
{
	/* This scalar fingerprint detects metadata refreshes that affect arrival
	 * ordering. It cannot supply metadata, content or namespace membership. */
	uint64_t hash = 14695981039346656037ULL;
	uint64_t fields[] = {entry->size, (uint64_t)entry->mtime, entry->kind};
	for (size_t i = 0; i < sizeof(fields) / sizeof(*fields); i++)
		for (unsigned shift = 0; shift < 64; shift += 8)
			hash = (hash ^ ((fields[i] >> shift) & 255)) * 1099511628211ULL;
	return hash;
}

static pg_status choose_unlocked(FileSystem *system, const char *path,
	Choice *copies, size_t count, int fast, int exact, uint64_t prior,
	Choice *out)
{
	int loose = 0, archive = 0, seen = 0;
	pg_id pinned = 0;
	for (Policy *p = system->policy[metadata_hash(path)]; p; p = p->next)
		if (!strcmp(p->path, path)) {
			seen |= p->seen;
			pinned = p->exact;
		}
	for (size_t i = 0; i < count; i++) {
		if (!eligible(copies[i].source, path)) continue;
		if (copies[i].entry.format == PG_LOOSE) {
			loose = 1;
			if (copies[i].order <= prior) seen = 1;
		} else
			archive = 1;
	}
	if (is_ignored(path) ||
		(!loose &&
			(mode == FILE_MODE_DEVELOPMENT ||
				(mode == FILE_MODE_DYNAMIC && !fast && !seen))))
		return PG_NOT_FOUND;
	if (loose && archive)
		for (size_t i = 0; i < count; i++) {
			Choice *copy = &copies[i];
			Policy *p = policy_add(system, path, copy->source);
			if (!p) return PG_NOMEM;
			uint64_t signature = policy_signature(&copy->entry);
			if (p->signature && p->signature != signature) {
				p->order = ++system->order;
				pinned = 0;
			}
			if (!p->order) p->order = copy->order;
			p->signature = signature;
			copy->order = p->order;
		}
	qsort(copies, count, sizeof(*copies), choice_order);
	size_t best = SIZE_MAX, best_loose = SIZE_MAX, best_archive = SIZE_MAX;
	for (size_t i = 0; i < count; i++) {
		Choice *copy = &copies[i];
		if (!eligible(copy->source, path)) continue;
		if (best == SIZE_MAX || replaces(&copies[best], copy)) best = i;
		size_t *kind =
			copy->entry.format == PG_LOOSE ? &best_loose : &best_archive;
		if (*kind == SIZE_MAX || replaces(&copies[*kind], copy)) *kind = i;
	}
	if (best == SIZE_MAX) return PG_NOT_FOUND;
	if (mode == FILE_MODE_DYNAMIC && loose && archive) {
		if (exact && !seen && copies[best].entry.kind == PG_ENTRY_FILE &&
			best_loose != SIZE_MAX && best_archive != SIZE_MAX) {
			best = copies[best_loose].entry.mtime ==
					copies[best_archive].entry.mtime
				? best_archive
				: best_loose;
			pinned = copies[best].entry.source_id;
		}
		if (pinned)
			for (size_t i = 0; i < count; i++)
				if (copies[i].entry.source_id == pinned &&
					eligible(copies[i].source, path)) {
					best = i;
					break;
				}
	}
	/* Store history only for mixed names, not a discovery index. */
	if (loose && archive)
		for (size_t i = 0; i < count; i++) {
			Policy *p = policy_add(system, path, copies[i].source);
			if (!p) return PG_NOMEM;
			if (!p->order) p->order = copies[i].order;
			p->seen = 1;
			p->exact = pinned;
		}
	*out = copies[best];
	memset(&copies[best], 0, sizeof(copies[best]));
	return PG_OK;
}

static pg_status choose(FileSystem *system, const char *path, Choice *copies,
	size_t count, int fast, int exact, uint64_t prior, Choice *out)
{
	AcquireSRWLockExclusive(&system->lock);
	pg_status result =
		choose_unlocked(system, path, copies, count, fast, exact, prior, out);
	ReleaseSRWLockExclusive(&system->lock);
	return result;
}
static pg_status source_choice(
	FileSystem *system, Source *source, const char *path, Choice *out)
{
	pg_file *file = NULL;
	pg_status status = !callbacks_enabled
		? pg_source_find(source->handle, path, &file, NULL)
		: pg_tree_find(source->tree, path, &file, NULL);
	pg_entry_info entry = {0};
	if (status == PG_OK) {
		pg_file_info info;
		status = pg_file_inspect(file, &info, NULL);
		if (status == PG_OK) {
			entry.kind = PG_ENTRY_FILE;
			entry.source_id = info.source_id;
			entry.canonical_name = info.canonical_name;
			entry.original_name = info.original_name;
			entry.size = info.logical_size;
			entry.mtime = info.mtime;
			if (source->info.format != PG_LOOSE)
				entry.attributes = PG_ENTRY_READ_ONLY;
		}
	} else if (status == PG_CONFLICT || !*path) {
		char *parent = _strdup(path);
		if (!parent) return PG_NOMEM;
		char *slash = strrchr(parent, '/');
		if (slash)
			*slash = 0;
		else
			*parent = 0;
		status = discover(system, source, parent, PG_DISCOVER_CHILDREN);
		pg_entry_cursor *cursor = NULL;
		if (status == PG_OK)
			status =
				pg_source_entries(source->handle, parent, 0, &cursor, NULL);
		free(parent);
		int found = 0;
		while (status == PG_OK &&
			(status = pg_entry_cursor_next(cursor, &entry, &file, NULL)) ==
				PG_OK) {
			if (!strcmp(entry.canonical_name, path)) {
				found = 1;
				break;
			}
			pg_file_close(&file, NULL);
		}
		if (found) {
			out->owned = 1;
			if (!make_entry(&out->entry, source, &entry)) status = PG_NOMEM;
		}
		pg_entry_cursor_close(&cursor, NULL);
		if (!found && status == PG_END) status = PG_NOT_FOUND;
		if (status == PG_OK) goto captured;
	}
	if (status == PG_OK) {
		out->entry.path = entry.canonical_name;
		out->entry.name = strrchr(entry.canonical_name, '/');
		out->entry.name =
			out->entry.name ? out->entry.name + 1 : entry.canonical_name;
		out->entry.source_id = entry.source_id;
		out->entry.size = entry.size;
		out->entry.mtime = entry.mtime;
		out->entry.kind = entry.kind;
		out->entry.attributes = entry.attributes;
		out->entry.format = source->info.format;
	}
captured:
	if (status != PG_OK) {
		pg_file_close(&file, NULL);
		return status;
	}
	out->source = source;
	out->file = file;
	out->order = source_order(system, source, path);
	return PG_OK;
}

static pg_status resolve(
	FileSystem *system, const char *path, int fast, int exact, Choice *out)
{
	AcquireSRWLockShared(&system->lock);
	uint64_t prior = system->order;
	ReleaseSRWLockShared(&system->lock);
	if (is_ignored(path)) return PG_NOT_FOUND;
	size_t count;
	Source *source_local[8];
	Source **sources = sources_copy(system, source_local, &count);
	if (!sources) return PG_NOMEM;
	Choice local[8] = {0};
	Choice *copies = count > 8 ? calloc(count, sizeof(*copies)) : local;
	if (!copies) {
		if (sources != source_local) free(sources);
		return PG_NOMEM;
	}
	size_t used = 0;
	pg_status status = PG_OK;
	for (size_t i = 0; i < count; i++) {
		Source *s = sources[i];
		if (!eligible(s, path)) continue;
		if (!callbacks_enabled && control(system) &&
			s->info.format == PG_LOOSE) {
			char buffer[1024];
			char *prefix = strlen(path) < sizeof(buffer) ? strcpy(buffer, path)
														 : _strdup(path);
			if (!prefix) {
				status = PG_NOMEM;
				break;
			}
			char *slash = strchr(prefix, '/');
			if (slash)
				*slash = 0;
			else
				*prefix = 0;
			status = discover(system, s, prefix,
				slash ? PG_DISCOVER_RECURSIVE : PG_DISCOVER_CHILDREN);
			if (prefix != buffer) free(prefix);
			if (status != PG_OK && status != PG_CONFLICT) break;
		}
		status = source_choice(system, s, path, &copies[used]);
		if (status == PG_OK)
			used++;
		else if (status != PG_NOT_FOUND && status != PG_CONFLICT)
			break;
		status = PG_OK;
	}
	if (status == PG_OK)
		status = choose(system, path, copies, used, fast, exact, prior, out);
	for (size_t i = 0; i < used; i++) choice_clear(&copies[i]);
	if (copies != local) free(copies);
	if (sources != source_local) free(sources);
	return status;
}

static void retire_context(pg_context **context)
{
	if (!*context) return;
	AcquireSRWLockExclusive(&retired_lock);
	if (pg_context_close(context, NULL) != PG_OK) {
		Retired *item = malloc(sizeof(*item));
		if (item) {
			item->context = *context;
			item->next = retired;
			retired = item;
			*context = NULL;
		}
	}
	ReleaseSRWLockExclusive(&retired_lock);
}
static void collect_retired(void)
{
	AcquireSRWLockExclusive(&retired_lock);
	for (Retired **p = &retired; *p;) {
		Retired *item = *p;
		if (pg_context_close(&item->context, NULL) == PG_OK) {
			*p = item->next;
			free(item);
		} else
			p = &item->next;
	}
	ReleaseSRWLockExclusive(&retired_lock);
}
FileSystem *fileSystemCreate(void)
{
	FileSystem *system = calloc(1, sizeof(*system));
	if (!system) return NULL;
	if (pg_context_open(&system->context, NULL) != PG_OK) {
		free(system);
		return NULL;
	}
	system->control_thread = GetCurrentThreadId();
	if (!main_thread) main_thread = system->control_thread;
	if (main_thread != system->control_thread) {
		pg_context_close(&system->context, NULL);
		free(system);
		return NULL;
	}
	AcquireSRWLockExclusive(&config_lock);
	system->next = systems;
	systems = system;
	ReleaseSRWLockExclusive(&config_lock);
	return system;
}
void fileSystemDestroy(FileSystem **input)
{
	if (!input || !*input || !control(*input)) return;
	FileSystem *system = *input;
	AcquireSRWLockExclusive(&config_lock);
	for (FileSystem **p = &systems; *p; p = &(*p)->next)
		if (*p == system) {
			*p = system->next;
			break;
		}
	ReleaseSRWLockExclusive(&config_lock);
	for (Pending *event = dispatch_events; event; event = event->next)
		if (event->change.system == system) event->change.system = NULL;
	for (Subscription *s = subscriptions; s; s = s->next)
		if (s->system == system) s->flags = 0;
	for (Pending **p = &pending; *p;) {
		Pending *event = *p;
		if (event->change.system != system) {
			p = &event->next;
			continue;
		}
		*p = event->next;
		entry_clear(&event->change.entry);
		free(event);
	}
	for (size_t i = 0; i < POLICY_BUCKETS; i++)
		while (system->policy[i]) {
			Policy *p = system->policy[i];
			system->policy[i] = p->next;
			free(p->path);
			free(p);
		}
	while (system->sources) {
		Source *s = system->sources;
		system->sources = s->next;
		pg_tree_close(&s->tree, NULL);
		pg_source_close(&s->handle, NULL);
		scopes_clear(s);
		free(s);
	}
	retire_context(&system->context);
	free(system);
	*input = NULL;
	collect_retired();
}
static int same_native_path(const char *left, const char *right);
pg_status fileSystemAddSource(
	FileSystem *system, const char *path, int priority)
{
	if (!system || !path || priority < 0) return PG_INVALID;
	if (!control(system)) return PG_BUSY;
	for (Source *known = system->sources; known; known = known->next)
		if (known->priority == priority &&
			same_native_path(path, known->info.native_path))
			return PG_EXISTS;
	Source *s = calloc(1, sizeof(*s));
	if (!s) return PG_NOMEM;
	pg_status status =
		pg_source_open(system->context, path, NULL, &s->handle, NULL);
	if (status != PG_OK) goto done;
	pg_source_inspect(s->handle, &s->info, NULL);
	s->priority = priority;
	for (Source *known = system->sources; known; known = known->next)
		if ((known->info.format == PG_LOOSE) == (s->info.format == PG_LOOSE) &&
			known->priority == priority) {
			status = PG_EXISTS;
			goto done;
		}
	status = pg_tree_create(system->context, &s->tree, NULL);
	if (status == PG_OK) status = pg_tree_attach(s->tree, s->handle, NULL);
	if (status == PG_OK)
		status = pg_tree_manage(s->tree, NULL, PG_DISCOVER_RECURSIVE, NULL);
	if (status == PG_OK) status = pg_tree_watch(s->tree, PG_WATCH_NATIVE, NULL);
	AcquireSRWLockExclusive(&system->lock);
	s->order = ++system->order;
	ReleaseSRWLockExclusive(&system->lock);
	if (status == PG_OK &&
		(s->info.format != PG_LOOSE || mode == FILE_MODE_DEVELOPMENT ||
			mode == FILE_MODE_HYBRID))
		status = discover(system, s, NULL, PG_DISCOVER_RECURSIVE);
	if (status == PG_OK) {
		AcquireSRWLockExclusive(&system->lock);
		AcquireSRWLockExclusive(&config_lock);
		Source **tail = &system->sources;
		while (*tail) tail = &(*tail)->next;
		*tail = s;
		system->source_kinds |= s->info.format == PG_LOOSE ? 1 : 2;
		ReleaseSRWLockExclusive(&config_lock);
		ReleaseSRWLockExclusive(&system->lock);
		return PG_OK;
	}
done:
	pg_tree_close(&s->tree, NULL);
	pg_source_close(&s->handle, NULL);
	scopes_clear(s);
	free(s);
	return status;
}
/* Common metadata queries borrow their operation's captured Piggle names.
 * Only returned selections/listings allocate wrapper-owned path storage.
 */
static int choice_materialize(Choice *choice)
{
	if (choice->owned) return 1;
	pg_file_info info;
	if (pg_file_inspect(choice->file, &info, NULL) != PG_OK) return 0;
	pg_entry_info entry = {0};
	entry.canonical_name = info.canonical_name;
	entry.original_name = info.original_name;
	entry.kind = choice->entry.kind;
	entry.attributes = choice->entry.attributes;
	entry.source_id = choice->entry.source_id;
	entry.size = choice->entry.size;
	entry.mtime = choice->entry.mtime;
	if (!make_entry(&choice->entry, choice->source, &entry)) return 0;
	choice->owned = 1;
	return 1;
}
static char *normalize_into(FileSystem *system, const char *path, char *local,
	size_t capacity, char **heap)
{
	if (!path) path = "";
	while (*path == '/' || *path == '\\' ||
		(path[0] == '.' && (path[1] == '/' || path[1] == '\\')))
		path += *path == '.' ? 2 : 1;
	if (!*path) {
		*local = 0;
		return local;
	}
	size_t required;
	pg_status status = pg_name_normalize(
		system->context, path, local, capacity, &required, NULL);
	if (status == PG_OK) return local;
	if (status != PG_CAPACITY) return NULL;
	*heap = normalize(system->context, path);
	return *heap;
}
FileSelection *fileSystemSelect(FileSystem *system, const char *name)
{
	if (!system || !name) return NULL;
	char local[1024], *heap = NULL;
	char *path = normalize_into(system, name, local, sizeof(local), &heap);
	if (!path) return NULL;
	Choice choice = {0};
	pg_status status = resolve(system, path, 0, 1, &choice);
	free(heap);
	if (status != PG_OK) return NULL;
	FileSelection *selection = calloc(1, sizeof(*selection));
	if (!selection || !choice_materialize(&choice)) {
		free(selection);
		choice_clear(&choice);
		return NULL;
	}
	selection->entry = choice.entry;
	selection->file = choice.file;
	if (selection->file)
		pg_file_inspect(selection->file, &selection->info, NULL);
	return selection;
}
const FileSystemEntry *fileSelectionEntry(const FileSelection *selection)
{ return selection ? &selection->entry : NULL; }

const pg_file_info *fileSelectionInfo(const FileSelection *selection)
{ return selection && selection->file ? &selection->info : NULL; }

static void selection_close(FileSelection *selection)
{
	pg_file_close(&selection->file, NULL);
	entry_clear(&selection->entry);
	free(selection);
}

void fileSelectionFree(FileSelection **selection)
{
	if (!selection || !*selection) return;
	selection_close(*selection);
	*selection = NULL;
}
pg_status fileSystemQuery(FileSystem *system, const char *name,
	FileSystemEntry *out, char *native_path, size_t capacity)
{
	if (!out) return PG_INVALID;
	memset(out, 0, sizeof(*out));
	if (!system || !name || (native_path && !capacity) ||
		(!native_path && capacity))
		return PG_INVALID;
	if (native_path) *native_path = 0;
	char local[1024], *heap = NULL;
	char *path = normalize_into(system, name, local, sizeof(local), &heap);
	if (!path) return PG_INVALID;
	Choice choice = {0};
	pg_status status = resolve(system, path, 0, 1, &choice);
	free(heap);
	if (status != PG_OK) return status;
	if (native_path) {
		if (choice.owned) {
			if (strlen(choice.entry.native_path) >= capacity)
				status = PG_CAPACITY;
			else
				strcpy(native_path, choice.entry.native_path);
		} else {
			pg_file_info info;
			pg_file_inspect(choice.file, &info, NULL);
			const char *root = choice.source->info.native_path;
			const char *separator =
				choice.entry.format == PG_LOOSE ? "/" : ":/";
			if (strlen(root) + strlen(separator) + strlen(info.original_name) >=
				capacity)
				status = PG_CAPACITY;
			else {
				strcpy(native_path, root);
				strcat(native_path, separator);
				strcat(native_path, info.original_name);
			}
		}
	}
	if (status == PG_OK) {
		*out = choice.entry;
		out->path = out->name = NULL;
		out->native_path = native_path;
	}
	choice_clear(&choice);
	return status;
}
pg_status fileSelectionOpen(
	const FileSelection *selection, uint32_t representation, pg_reader **out)
{
	if (!out) return PG_INVALID;
	*out = NULL;
	if (!selection) return PG_INVALID;
	return selection->file
		? pg_reader_open(selection->file, representation, out, NULL)
		: PG_CONFLICT;
}
static int same_native_path(const char *left, const char *right)
{
	DWORD size = GetFullPathNameA(left, 0, NULL, NULL);
	char *absolute;
	int equal = 0;
	if (!size) return 0;
	absolute = malloc(size);
	if (!absolute) return 0;
	if (GetFullPathNameA(left, size, absolute, NULL)) {
		char *cursor = absolute;
		while (*cursor && *right) {
			int a = (unsigned char)*cursor++ == '\\'
				? '/'
				: (unsigned char)cursor[-1];
			int b = (unsigned char)*right++ == '\\' ? '/'
													: (unsigned char)right[-1];
			if (tolower(a) != tolower(b)) {
				free(absolute);
				return 0;
			}
		}
		equal = !*cursor && !*right;
	}
	free(absolute);
	return equal;
}

pg_status fileSystemOpenPath(const char *path, pg_reader **out)
{
	if (!path || !out) return PG_INVALID;
	*out = NULL;
	const char *split = strchr(path, ':');
	pg_context *context = NULL;
	pg_source *source = NULL;
	pg_status status;
	char *archive = NULL;
	if (split == path + 1) split = strchr(split + 1, ':');
	status = pg_context_open(&context, NULL);
	if (status != PG_OK) return status;
	if (split) {
		archive = _strdup(path);
		if (!archive) {
			status = PG_NOMEM;
			goto done;
		}
		archive[split - path] = 0;
		split++;
		while (*split == '/' || *split == '\\') split++;
		AcquireSRWLockShared(&config_lock);
		for (FileSystem *group = systems; group; group = group->next) {
			for (Source *known = group->sources; known; known = known->next) {
				if (known->info.format == PG_LOOSE ||
					!same_native_path(archive, known->info.native_path))
					continue;
				/* Retain an immediate source reference before group teardown.
				 */
				status = pg_tree_source(known->tree, 0, &source, NULL);
				ReleaseSRWLockShared(&config_lock);
				if (status == PG_OK)
					status = pg_reader_open_source(
						source, split, PG_READ_LOGICAL, out, NULL);
				goto done;
			}
		}
		ReleaseSRWLockShared(&config_lock);
		status = pg_source_open(context, archive, NULL, &source, NULL);
		if (status == PG_OK) {
			status = pg_reader_open_source(
				source, split, PG_READ_LOGICAL, out, NULL);
		}
	} else
		status = pg_reader_open_native(context, path, out, NULL);
done:
	if (source) pg_source_close(&source, NULL);
	retire_context(&context);
	free(archive);
	return status;
}

static int listing_append(
	FileListing *listing, size_t *capacity, FileSystemEntry *entry, int take)
{
	FileSystemEntry *entries = (FileSystemEntry *)listing->entries;
	if (listing->count == *capacity) {
		size_t next = *capacity ? *capacity * 2 : 32;
		if (next < *capacity || next > SIZE_MAX / sizeof(*entries)) return 0;
		entries = realloc(entries, next * sizeof(*entries));
		if (!entries) return 0;
		listing->entries = entries;
		*capacity = next;
	}
	if (take) {
		entries[listing->count] = *entry;
		memset(entry, 0, sizeof(*entry));
	} else if (!entry_copy(&entries[listing->count], entry))
		return 0;
	listing->count++;
	return 1;
}

typedef struct EntryStream {
	Source *source;
	pg_entry_cursor *cursor;
	Choice choice;
	pg_status status;
} EntryStream;
static pg_status stream_next(FileSystem *system, EntryStream *stream)
{
	choice_clear(&stream->choice);
	pg_entry_info entry;
	pg_file *file = NULL;
	pg_status status =
		pg_entry_cursor_next(stream->cursor, &entry, &file, NULL);
	if (status == PG_OK) {
		if (!make_entry(&stream->choice.entry, stream->source, &entry)) {
			pg_file_close(&file, NULL);
			return PG_NOMEM;
		}
		stream->choice.owned = 1;
		stream->choice.source = stream->source;
		stream->choice.file = file;
		stream->choice.order =
			source_order(system, stream->source, entry.canonical_name);
	}
	stream->status = status;
	return status;
}

static FileListing *listing(FileSystem *system, const char *prefix, int fast,
	int recursive, int refresh)
{
	size_t count, capacity = 0;
	Source *local[8];
	Source **sources = sources_copy(system, local, &count);
	if (!sources) return NULL;
	EntryStream *streams = calloc(count ? count : 1, sizeof(*streams));
	Choice *copies = calloc(count ? count : 1, sizeof(*copies));
	FileListing *result = calloc(1, sizeof(*result));
	if (!streams || !copies || !result) goto fail;
	size_t used = 0;
	for (size_t si = 0; si < count; si++) {
		Source *s = sources[si];
		if ((s->info.format == PG_LOOSE &&
				(mode == FILE_MODE_ARCHIVES ||
					(fast && mode == FILE_MODE_DYNAMIC))) ||
			(s->info.format != PG_LOOSE && mode == FILE_MODE_LOOSE))
			continue;
		pg_status status = refresh
			? discover(system, s, prefix,
				  recursive ? PG_DISCOVER_RECURSIVE : PG_DISCOVER_CHILDREN)
			: PG_OK;
		if (status == PG_CONFLICT) continue;
		if (status != PG_OK) goto fail;
		EntryStream *stream = &streams[used++];
		stream->source = s;
		/* Discovery above has reconciled the tree when observation is live.
		 * Source cursors themselves never consume hints during a pause. */
		status = callbacks_enabled && !system->resume
			? pg_tree_entries(s->tree, prefix,
				  recursive ? PG_ENTRIES_RECURSIVE : 0, &stream->cursor, NULL)
			: pg_source_entries(s->handle, prefix,
				  recursive ? PG_ENTRIES_RECURSIVE : 0, &stream->cursor, NULL);
		if (status == PG_INVALID && !refresh) {
			used--;
			continue;
		}
		if (status != PG_OK) goto fail;
		status = stream_next(system, stream);
		if (status != PG_OK && status != PG_END) goto fail;
	}
	for (;;) {
		const char *path = NULL;
		for (size_t i = 0; i < used; i++)
			if (streams[i].status == PG_OK &&
				(!path || strcmp(streams[i].choice.entry.path, path) < 0))
				path = streams[i].choice.entry.path;
		if (!path) break;
		/* Moving the stream choice keeps its name alive through resolution. */
		const char *name = path;
		size_t n = 0;
		pg_status status = PG_OK;
		for (size_t i = 0; i < used; i++) {
			EntryStream *stream = &streams[i];
			if (stream->status != PG_OK ||
				strcmp(stream->choice.entry.path, name))
				continue;
			copies[n++] = stream->choice;
			memset(&stream->choice, 0, sizeof(stream->choice));
			status = stream_next(system, stream);
			if (status != PG_OK && status != PG_END) break;
		}
		Choice selected = {0};
		if (status == PG_OK || status == PG_END)
			status = choose(system, name, copies, n, fast, 0, 0, &selected);
		if (status == PG_OK) {
			if (!listing_append(result, &capacity, &selected.entry, 1))
				status = PG_NOMEM;
		}
		choice_clear(&selected);
		for (size_t i = 0; i < n; i++) choice_clear(&copies[i]);
		if (status != PG_OK && status != PG_NOT_FOUND) goto fail;
	}
	for (size_t i = 0; i < count; i++) {
		choice_clear(&streams[i].choice);
		pg_entry_cursor_close(&streams[i].cursor, NULL);
	}
	free(streams);
	free(copies);
	if (sources != local) free(sources);
	return result;
fail:
	if (streams)
		for (size_t i = 0; i < count; i++) {
			choice_clear(&streams[i].choice);
			pg_entry_cursor_close(&streams[i].cursor, NULL);
		}
	free(streams);
	free(copies);
	fileListingFree(&result);
	if (sources != local) free(sources);
	return NULL;
}

FileListing *fileSystemList(FileSystem *system, const char *name, int fast)
{
	if (!system) return NULL;
	char *prefix = normalize(system->context, name);
	if (!prefix) return NULL;
	FileListing *result = listing(system, prefix, fast, 0, 1);
	free(prefix);
	return result;
}
pg_status fileSystemPrepareTree(FileSystem *system, const char *name, int fast)
{
	if (!system) return PG_INVALID;
	if (!control(system)) return PG_BUSY;
	char *prefix = normalize(system->context, name);
	if (!prefix) return PG_INVALID;
	pg_status status = PG_OK;
	for (Source *s = system->sources; s; s = s->next) {
		if (s->info.format == PG_LOOSE &&
			(mode == FILE_MODE_ARCHIVES || (fast && mode == FILE_MODE_DYNAMIC)))
			continue;
		status = discover(system, s, prefix, PG_DISCOVER_RECURSIVE);
		if (status == PG_CONFLICT) status = PG_OK;
		if (status != PG_OK) break;
	}
	free(prefix);
	return status;
}
pg_status fileSystemCacheTree(FileSystem *system, const char *prefix, int fast)
{
	if (!system) return PG_INVALID;
	return callbacks_enabled ? PG_OK
							 : fileSystemPrepareTree(system, prefix, fast);
}
FileListing *fileSystemListNative(const char *path)
{
	if (!path) return NULL;
	pg_context *context = NULL;
	Source source = {0};
	pg_entry_cursor *cursor = NULL;
	pg_file *file = NULL;
	pg_entry_info info;
	FileListing *listing = calloc(1, sizeof(*listing));
	size_t capacity = 0;
	pg_status status = PG_NOMEM;
	if (!listing || pg_context_open(&context, NULL) != PG_OK) goto done;
	status = pg_source_open(context, path, NULL, &source.handle, NULL);
	if (status != PG_OK) goto done;
	pg_source_inspect(source.handle, &source.info, NULL);
	if (source.info.format != PG_LOOSE) {
		status = PG_INVALID;
		goto done;
	}
	status =
		pg_source_discover(source.handle, NULL, PG_DISCOVER_CHILDREN, NULL);
	if (status != PG_OK) goto done;
	status = pg_source_entries(source.handle, NULL, 0, &cursor, NULL);
	if (status != PG_OK) goto done;
	while (
		(status = pg_entry_cursor_next(cursor, &info, &file, NULL)) == PG_OK) {
		FileSystemEntry entry;
		pg_file_close(&file, NULL);
		if (!make_entry(&entry, &source, &info)) {
			status = PG_NOMEM;
			break;
		}
		if (!listing_append(listing, &capacity, &entry, 1)) status = PG_NOMEM;
		entry_clear(&entry);
		if (status != PG_OK) break;
	}
done:
	if (cursor) pg_entry_cursor_close(&cursor, NULL);
	if (source.handle) pg_source_close(&source.handle, NULL);
	if (context) pg_context_close(&context, NULL);
	if (status != PG_END) fileListingFree(&listing);
	return listing;
}

void fileListingFree(FileListing **listing)
{
	if (!listing || !*listing) return;
	for (size_t i = 0; i < (*listing)->count; i++)
		entry_clear((FileSystemEntry *)&(*listing)->entries[i]);
	free((void *)(*listing)->entries);
	free(*listing);
	*listing = NULL;
}

void fileSystemSetMode(FileSystemMode value)
{ InterlockedExchange(&current_mode, value); }
FileSystemMode fileSystemGetMode(void) { return mode; }
void fileSystemChooseMode(void)
{
	fileSystemSetMode(
		fileIsUsingDevData() ? FILE_MODE_DYNAMIC : FILE_MODE_HYBRID);
}
void fileSystemIgnoreAdd(const char *prefix)
{
	if (!prefix || !*prefix) return;
	AcquireSRWLockExclusive(&config_lock);
	for (size_t i = 0; i < ignored_count; i++)
		if (!_stricmp(prefix, ignored[i])) goto done;
	char *copy = _strdup(prefix);
	if (!copy) goto done;
	char **items = realloc(ignored, (ignored_count + 1) * sizeof(*items));
	if (!items) {
		free(copy);
		goto done;
	}
	ignored = items;
	ignored[ignored_count++] = copy;
done:
	ReleaseSRWLockExclusive(&config_lock);
}
void fileSystemIgnoreRemove(const char *prefix)
{
	if (!prefix) return;
	AcquireSRWLockExclusive(&config_lock);
	for (size_t i = 0; i < ignored_count; i++) {
		if (_stricmp(prefix, ignored[i])) continue;
		free(ignored[i]);
		memmove(ignored + i, ignored + i + 1,
			(--ignored_count - i) * sizeof(*ignored));
		break;
	}
	ReleaseSRWLockExclusive(&config_lock);
}
void fileSystemIgnoreStandard(void) { fileSystemIgnoreAdd("d_"); }
int fileSystemIgnored(const char *path) { return is_ignored(path); }
void fileSystemDisallowOverrides(void)
{
	AcquireSRWLockExclusive(&config_lock);
	overrides_disabled = 1;
	ReleaseSRWLockExclusive(&config_lock);
}
void fileSystemExclude(const char *directory, int only_root)
{
	char *copy = directory ? _strdup(directory) : NULL;
	if (directory && !copy) return;
	AcquireSRWLockExclusive(&config_lock);
	free(excluded);
	excluded = copy;
	exclude_only = only_root;
	ReleaseSRWLockExclusive(&config_lock);
}
static int matches(const char *pattern, const char *name)
{
	const char *star = strchr(pattern, '*');
	if (!star) return !_stricmp(pattern, name);
	size_t prefix = star - pattern, suffix = strlen(star + 1);
	size_t length = strlen(name);
	return length >= prefix + suffix && !_strnicmp(pattern, name, prefix) &&
		!_stricmp(star + 1, name + length - suffix);
}
static int subscribed(FileSystem *system, const char *path)
{
	for (Subscription *s = subscriptions; s; s = s->next)
		if (s->flags && (!s->system || s->system == system) &&
			matches(s->pattern, path))
			return 1;
	return 0;
}
pg_status fileSystemSubscribe(FileSystem *system, const char *pattern,
	uint32_t flags, FileChangeHandler callback, void *user)
{
	const char *star = pattern ? strchr(pattern, '*') : NULL;
	if (!pattern || !callback ||
		!(flags & (FILE_CHANGE_UPDATE | FILE_CHANGE_DELETE)) ||
		(star && strchr(star + 1, '*')))
		return PG_INVALID;
	if (GetCurrentThreadId() != main_thread) return PG_BUSY;
	if (!(flags & FILE_CHANGE_SHARED) && sharedMemoryGetMode() == SMM_ENABLED)
		return PG_OK;
	Subscription *s = calloc(1, sizeof(*s));
	if (!s) return PG_NOMEM;
	s->pattern = _strdup(pattern);
	if (!s->pattern) {
		free(s);
		return PG_NOMEM;
	}
	s->system = system;
	s->flags = flags;
	s->callback = callback;
	s->user = user;
	s->next = subscriptions;
	subscriptions = s;
	return PG_OK;
}
void fileSystemCallbacksEnabled(int enabled)
{
	if (main_thread && GetCurrentThreadId() != main_thread) return;
	if (!!enabled == !!callbacks_enabled) return;
	/* Pausing does not disarm watches or discard undelivered reports.
	 * A new loading phase refreshes prepared scopes on their first use.
	 */
	for (FileSystem *system = systems; system; system = system->next) {
		if (enabled)
			system->resume = 1;
		else {
			AcquireSRWLockExclusive(&system->lock);
			system->phase++;
			ReleaseSRWLockExclusive(&system->lock);
		}
	}
	InterlockedExchange(&callbacks_enabled, !!enabled);
}
static void queue_change(
	FileSystem *system, const FileSystemEntry *entry, uint32_t kind)
{
	for (Subscription *s = subscriptions; s; s = s->next) {
		if (!(s->flags & kind) || (s->system && s->system != system) ||
			!matches(s->pattern, entry->path))
			continue;
		Pending *event;
		for (event = pending; event; event = event->next)
			if (event->subscription == s && event->change.system == system &&
				!strcmp(event->change.entry.path, entry->path))
				break;
		FileSystemEntry copy;
		if (!entry_copy(&copy, entry)) continue;
		if (event)
			entry_clear(&event->change.entry);
		else {
			event = calloc(1, sizeof(*event));
			if (!event) {
				entry_clear(&copy);
				continue;
			}
			event->next = pending;
			pending = event;
		}
		event->subscription = s;
		event->change.system = system;
		event->change.kind = kind;
		event->change.entry = copy;
	}
}
static int entry_equal(const FileSystemEntry *a, const FileSystemEntry *b)
{
	return a->source_id == b->source_id && a->kind == b->kind &&
		a->size == b->size && a->mtime == b->mtime &&
		a->attributes == b->attributes;
}
typedef struct Notice {
	struct Notice *next;
	Source *source;
	char *path;
	uint32_t kind;
	FileSystemEntry before, after;
} Notice;
typedef struct Observation {
	Source *source;
	Notice **tail;
	pg_status status;
} Observation;
static void PG_CALL observed(void *user, const pg_visible_change *change)
{
	Observation *batch = user;
	Notice *n = calloc(1, sizeof(*n));
	if (!n) {
		batch->status = PG_NOMEM;
		return;
	}
	n->path =
		_strdup(change->canonical_name ? change->canonical_name
									   : (change->scope ? change->scope : ""));
	n->source = batch->source;
	n->kind = change->kind;
	if (!n->path ||
		(change->before_entry &&
			!make_entry(&n->before, n->source, change->before_entry)) ||
		(change->after_entry &&
			!make_entry(&n->after, n->source, change->after_entry))) {
		entry_clear(&n->before);
		entry_clear(&n->after);
		free(n->path);
		free(n);
		batch->status = PG_NOMEM;
		return;
	}
	*batch->tail = n;
	batch->tail = &n->next;
}
/* Before sides come from Piggle, never a persistent wrapper metadata map. */
static void notify_name(FileSystem *system, Notice *notices, Notice *event)
{
	if (!*event->path || !subscribed(system, event->path)) return;
	size_t count = 0, used = 0;
	for (Source *s = system->sources; s; s = s->next) count++;
	Choice *copies = calloc(count ? count : 1, sizeof(*copies));
	if (!copies) return;
	for (Source *s = system->sources; s; s = s->next) {
		Notice *n;
		for (n = notices; n; n = n->next)
			if (n->source == s && !strcmp(n->path, event->path)) break;
		if (n) {
			if (!n->before.path) continue;
			if (!entry_copy(&copies[used].entry, &n->before)) break;
			copies[used].owned = 1;
			copies[used].source = s;
			copies[used++].order = source_order(system, s, event->path);
		} else if (source_choice(system, s, event->path, &copies[used]) ==
			PG_OK)
			used++;
	}
	Choice before = {0}, after = {0};
	choose(system, event->path, copies, used, 0, 0, 0, &before);
	for (size_t i = 0; i < used; i++) choice_clear(&copies[i]);
	free(copies);
	AcquireSRWLockExclusive(&system->lock);
	for (Policy *p = system->policy[metadata_hash(event->path)]; p;
		p = p->next) {
		if (strcmp(p->path, event->path)) continue;
		p->exact = 0;
		if (p->source == event->source && event->kind == PG_CHANGE_UPDATE)
			p->order = ++system->order;
	}
	ReleaseSRWLockExclusive(&system->lock);
	pg_status status = resolve(system, event->path, 0, 0, &after);
	if (status == PG_OK &&
		(!before.source || !entry_equal(&before.entry, &after.entry) ||
			(event->kind == PG_CHANGE_UPDATE &&
				after.source == event->source))) {
		if (choice_materialize(&after))
			queue_change(system, &after.entry, FILE_CHANGE_UPDATE);
	} else if (status == PG_NOT_FOUND && before.source) {
		if (choice_materialize(&before))
			queue_change(system, &before.entry, FILE_CHANGE_DELETE);
	}
	choice_clear(&before);
	choice_clear(&after);
}
static void diff_listings(
	FileSystem *system, const FileListing *before, const FileListing *after)
{
	if (!before || !after) return;
	size_t i = 0, j = 0;
	while (i < before->count || j < after->count) {
		const FileSystemEntry *a =
			i < before->count ? &before->entries[i] : NULL;
		const FileSystemEntry *b = j < after->count ? &after->entries[j] : NULL;
		int order = !a ? 1 : !b ? -1 : strcmp(a->path, b->path);
		if (order < 0) {
			queue_change(system, a, FILE_CHANGE_DELETE);
			i++;
		} else if (order > 0) {
			queue_change(system, b, FILE_CHANGE_UPDATE);
			j++;
		} else {
			if (!entry_equal(a, b)) queue_change(system, b, FILE_CHANGE_UPDATE);
			i++;
			j++;
		}
	}
}
static pg_status resume_scopes(FileSystem *system)
{
	pg_status first = PG_OK;
	/* Temporary snapshots exist only across an actual refresh. Suppressed
	 * loading watches have no per-file tree baseline to provide before sides.
	 */
	for (Source *s = system->sources; s; s = s->next)
		for (Scope *scope = s->scopes; scope; scope = scope->next) {
			FileListing *before = subscriptions
				? listing(system, scope->prefix, 0,
					  scope->depth == PG_DISCOVER_RECURSIVE, 0)
				: NULL;
			pg_status status =
				pg_tree_discover(s->tree, scope->prefix, scope->depth, NULL);
			if (status != PG_OK && first == PG_OK) first = status;
			FileListing *after = before
				? listing(system, scope->prefix, 0,
					  scope->depth == PG_DISCOVER_RECURSIVE, 0)
				: NULL;
			diff_listings(system, before, after);
			fileListingFree(&before);
			fileListingFree(&after);
		}
	system->resume = 0;
	return first;
}
pg_status fileSystemDispatch(void)
{
	if (GetCurrentThreadId() != main_thread || dispatching) return PG_OK;
	dispatching = 1;
	pg_status first = PG_OK;
	if (callbacks_enabled)
		for (FileSystem *system = systems; system; system = system->next) {
			if (system->resume) {
				pg_status status = resume_scopes(system);
				if (status != PG_OK && first == PG_OK) first = status;
			}
			Notice *notices = NULL;
			Observation batch = {NULL, &notices, PG_OK};
			for (Source *s = system->sources; s; s = s->next) {
				batch.source = s;
				pg_observer observer = {&batch, observed};
				pg_status status = pg_tree_poll(s->tree, &observer, NULL);
				if (status != PG_OK && first == PG_OK) first = status;
			}
			if (batch.status != PG_OK && first == PG_OK) first = batch.status;
			for (Notice *n = notices; n; n = n->next) {
				if (n->kind == PG_CHANGE_INVALIDATE) {
					/* A source-local loading request may predate watch
					 * discovery. */
					for (Scope *scope = n->source->scopes; scope;
						scope = scope->next)
						if (under(n->path, scope->prefix)) {
							pg_tree_discover(n->source->tree, scope->prefix,
								scope->depth, NULL);
							break;
						}
				}
				notify_name(system, notices, n);
				if (n->kind == PG_CHANGE_LOSS) resume_scopes(system);
			}
			while (notices) {
				Notice *n = notices;
				notices = n->next;
				entry_clear(&n->before);
				entry_clear(&n->after);
				free(n->path);
				free(n);
			}
		}
	dispatch_events = callbacks_enabled ? pending : NULL;
	if (callbacks_enabled) pending = NULL;
	while (dispatch_events) {
		Pending *event = dispatch_events;
		dispatch_events = event->next;
		Subscription *s = event->subscription;
		if (s->flags && event->change.system)
			s->callback(&event->change, s->user);
		entry_clear(&event->change.entry);
		free(event);
	}
	collect_retired();
	dispatching = 0;
	return first;
}
