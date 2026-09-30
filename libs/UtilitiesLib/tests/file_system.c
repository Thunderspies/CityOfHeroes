#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <direct.h>
#include <process.h>
#include "utilitieslib/utils/FileSystem.h"
#include "checks.h"

static pg_context *fixture_context;
static unsigned callbacks;
static DWORD callback_thread;
static uint32_t last_change;

static int put(const char *path, const char *data, int64_t mtime)
{
	pg_write_options options;
	pg_writer *writer = NULL;
	size_t accepted;

	pg_write_options_init(&options, strlen(data));
	options.entry.mtime = mtime;
	CHECK(pg_writer_open_native(fixture_context, path, &options,
		PG_OVERWRITE, &writer, NULL) == PG_OK);
	CHECK(pg_writer_write(writer, data, strlen(data), &accepted,
		NULL) == PG_OK);
	CHECK(accepted == strlen(data));
	CHECK(pg_writer_finish(writer, NULL) == PG_OK);
	CHECK(pg_writer_close(&writer, NULL) == PG_OK);
	return 0;
}

static int archive(const char *path, int64_t mtime)
{
	pg_archive_builder *builder = NULL;
	pg_entry_options options = { 0 };

	options.mtime = mtime;
	options.compression = PG_COMPRESS_FORCE;
	CHECK(pg_archive_builder_create(fixture_context, path, PG_PIGG2,
		PG_OVERWRITE, &builder, NULL) == PG_OK);
	CHECK(pg_archive_builder_write_all(builder, "dir/file", "archive", 7,
		&options, NULL) == PG_OK);
	CHECK(pg_archive_builder_finish(builder, NULL) == PG_OK);
	CHECK(pg_archive_builder_close(&builder, NULL) == PG_OK);
	return 0;
}

static void changed(const FileChange *change, void *user)
{
	(void)user;
	callbacks++;
	last_change = change->kind;
	callback_thread = GetCurrentThreadId();
}

static unsigned __stdcall worker(void *data)
{
	for (unsigned repeat = 0; repeat < 64; repeat++) {
		FileSelection *selection = fileSystemSelect(data, "dir/file");
		pg_reader *reader = NULL;
		char bytes[8];
		size_t count;
		uint64_t position;

		CHECK(selection != NULL);
		CHECK(fileSelectionOpen(selection, PG_READ_LOGICAL, &reader) == PG_OK);
		fileSelectionFree(&selection);
		CHECK(selection == NULL);
		CHECK(pg_reader_seek(reader, 2, NULL) == PG_OK);
		CHECK(pg_reader_read(reader, bytes, 3, &count, NULL) == PG_OK);
		CHECK(count == 3 && !memcmp(bytes, "chi", 3));
		CHECK(pg_reader_tell(reader, &position, NULL) == PG_OK && position == 5);
		CHECK(pg_reader_close(&reader, NULL) == PG_OK);
	}
	return 0;
}

static unsigned __stdcall direct_worker(void *data)
{
	FileSystem *system = data;
	CHECK(fileSystemAddSource(system, "loose", 99) == PG_BUSY);
	CHECK(fileSystemPrepareTree(system, "dir", 0) == PG_BUSY);
	for (unsigned repeat = 0; repeat < 64; repeat++) {
		FileSystemEntry metadata;
		CHECK(fileSystemQuery(system, "dir/file", &metadata, NULL, 0)
			== PG_OK);
		FileListing *list = fileSystemList(system, "dir", 0);
		CHECK(list && list->count == 1);
		CHECK(!strcmp(list->entries[0].path, "dir/file"));
		fileListingFree(&list);
		FileSelection *selection = fileSystemSelect(system, "DIR/FILE");
		CHECK(selection);
		pg_reader *reader = NULL;
		CHECK(fileSelectionOpen(selection, PG_READ_LOGICAL, &reader) == PG_OK);
		fileSelectionFree(&selection);
		char bytes[16];
		size_t count;
		CHECK(pg_reader_read(reader, bytes, sizeof(bytes), &count, NULL) == PG_OK);
		CHECK(count == metadata.size);
		CHECK(!memcmp(bytes, metadata.format == PG_LOOSE ?
			"native" : "archive", count));
		CHECK(pg_reader_close(&reader, NULL) == PG_OK);
	}
	return 0;
}

static unsigned __stdcall unprepared_worker(void *data)
{
	FileListing *list = fileSystemList(data, "dir", 0);
	CHECK(!list);
	/* Exact lookup is permitted even without a requested subtree. */
	FileSelection *selection = fileSystemSelect(data, "dir/file");
	CHECK(selection);
	fileSelectionFree(&selection);
	FileListing *native = fileSystemListNative("loose/dir");
	CHECK(native && native->count == 1);
	fileListingFree(&native);
	return 0;
}

static int direct_workers(void)
{
	CHECK(!put("loose/dir/file", "native", 10000));
	CHECK(!archive("direct.pigg", 10000));
	for (int mode = FILE_MODE_DEVELOPMENT; mode <= FILE_MODE_LOOSE; mode++) {
		fileSystemSetMode(mode);
		FileSystem *system = fileSystemCreate();
		CHECK(system);
		CHECK(fileSystemAddSource(system, "loose", 0) == PG_OK);
		CHECK(fileSystemAddSource(system, "direct.pigg", 0) == PG_OK);
		if (mode == FILE_MODE_DYNAMIC || mode == FILE_MODE_LOOSE) {
			HANDLE thread = (HANDLE)_beginthreadex(NULL, 0,
				unprepared_worker, system, 0, NULL);
			CHECK(thread && WaitForSingleObject(thread, 10000) == WAIT_OBJECT_0);
			DWORD result;
			CHECK(GetExitCodeThread(thread, &result) && !result);
			CloseHandle(thread);
		}
		CHECK(fileSystemPrepareTree(system, "dir", 0) == PG_OK);
		HANDLE threads[4];
		for (unsigned i = 0; i < 4; i++) {
			threads[i] = (HANDLE)_beginthreadex(NULL, 0,
				direct_worker, system, 0, NULL);
			CHECK(threads[i]);
		}
		/* Main waits exactly as rdrQueueFlush does, without a poll/RPC pump. */
		CHECK(WaitForMultipleObjects(4, threads, TRUE, 10000) == WAIT_OBJECT_0);
		for (unsigned i = 0; i < 4; i++) {
			DWORD result;
			CHECK(GetExitCodeThread(threads[i], &result) && !result);
			CloseHandle(threads[i]);
		}
		fileSystemDestroy(&system);
	}
	return 0;
}

static int selection_cases(void)
{
	const int64_t loose_times[] = { 10000, 10000, 10000, 10000 };
	const int64_t archive_times[] = { 10000, 13600, 6400, 10001 };
	const uint32_t formats[] = { PG_PIGG2, PG_PIGG2, PG_LOOSE, PG_LOOSE };

	_mkdir("loose");
	_mkdir("loose/dir");
	_mkdir("loose/empty");
	for (unsigned i = 0; i < 4; i++) {
		CHECK(!put("loose/dir/file", "native", loose_times[i]));
		CHECK(!archive("base.pigg", archive_times[i]));
		fileSystemSetMode(FILE_MODE_HYBRID);
		FileSystem *system = fileSystemCreate();

		CHECK(system != NULL);
		CHECK(fileSystemAddSource(system, "loose", 0) == PG_OK);
		CHECK(fileSystemAddSource(system, "base.pigg", 0) == PG_OK);
		FileSelection *selection = fileSystemSelect(system, "/DIR/FILE");

		CHECK(selection != NULL);
		CHECK(fileSelectionEntry(selection)->format == formats[i]);
		fileSelectionFree(&selection);
		FileListing *list = fileSystemList(system, NULL, 0);

		CHECK(list && list->count == 2);
		CHECK(!strcmp(list->entries[0].path, "dir"));
		CHECK(!strcmp(list->entries[1].path, "empty"));
		CHECK(list->entries[1].kind == PG_ENTRY_DIRECTORY);
		fileListingFree(&list);
		CHECK(list == NULL);
		if (!i) {
			HANDLE threads[4];
			fileSystemCallbacksEnabled(0);
			for (unsigned t = 0; t < 4; t++) {
				threads[t] = (HANDLE)_beginthreadex(NULL, 0,
					worker, system, 0, NULL);
				CHECK(threads[t] != NULL);
			}
			CHECK(WaitForMultipleObjects(4, threads, TRUE, 10000) == WAIT_OBJECT_0);
			for (unsigned t = 0; t < 4; t++) {
				DWORD result;
				CHECK(GetExitCodeThread(threads[t], &result) && result == 0);
				CloseHandle(threads[t]);
			}
			fileSystemCallbacksEnabled(1);
		}
		fileSystemDestroy(&system);
		CHECK(system == NULL);
	}
	return 0;
}

static int lazy_callbacks(void)
{
	callbacks = 0;
	_mkdir("lazy");
	DeleteFileA("lazy/new.txt");
	fileSystemSetMode(FILE_MODE_DYNAMIC);
	FileSystem *system = fileSystemCreate();

	CHECK(system != NULL);
	CHECK(fileSystemAddSource(system, "lazy", 0) == PG_OK);
	CHECK(fileSystemSubscribe(system, "*.txt", FILE_CHANGE_UPDATE |
		FILE_CHANGE_DELETE, changed, NULL) == PG_OK);
	fileSystemCallbacksEnabled(0);
	CHECK(!put("lazy/new.txt", "first", 12000));
	CHECK(!put("lazy/new.txt", "second version", 12001));
	CHECK(fileSystemDispatch() == PG_OK && callbacks == 0);
	fileSystemCallbacksEnabled(1);
	for (int i = 0; i < 100 && !callbacks; i++) {
		Sleep(10);
		CHECK(fileSystemDispatch() == PG_OK);
	}
	CHECK(callbacks == 1 && last_change == FILE_CHANGE_UPDATE);
	CHECK(callback_thread == GetCurrentThreadId());
	FileSelection *selected = fileSystemSelect(system, "new.txt");

	CHECK(selected && fileSelectionEntry(selected)->size == 14);
	fileSelectionFree(&selected);
	CHECK(DeleteFileA("lazy/new.txt"));
	for (int i = 0; i < 100 && callbacks < 2; i++) {
		Sleep(10);
		CHECK(fileSystemDispatch() == PG_OK);
	}
	CHECK(callbacks == 2 && last_change == FILE_CHANGE_DELETE);
	fileSystemDestroy(&system);
	return 0;
}

static int dynamic_selection(void)
{
	const int64_t times[] = { 10000, 6400, 13600 };
	_mkdir("dynamic");
	_mkdir("dynamic/dir");
	for (unsigned i = 0; i < 3; i++) {
		CHECK(!put("dynamic/dir/file", "native", times[i]));
		CHECK(!archive("dynamic.pigg", 10000));
		fileSystemSetMode(FILE_MODE_DYNAMIC);
		FileSystem *system = fileSystemCreate();
		CHECK(system);
		CHECK(fileSystemAddSource(system, "dynamic", 0) == PG_OK);
		CHECK(fileSystemAddSource(system, "dynamic.pigg", 0) == PG_OK);
		for (unsigned repeat = 0; repeat < 2; repeat++) {
			FileSelection *selected = fileSystemSelect(system, "dir/file");
			CHECK(selected);
			CHECK(fileSelectionEntry(selected)->format ==
				(i ? PG_LOOSE : PG_PIGG2));
			fileSelectionFree(&selected);
		}
		fileSystemDestroy(&system);
	}
	return 0;
}

static int listing_refresh(void)
{
	_mkdir("refresh");
	CHECK(!put("refresh/old.txt", "old", 15000));
	fileSystemSetMode(FILE_MODE_LOOSE);
	FileSystem *system = fileSystemCreate();
	CHECK(system && fileSystemAddSource(system, "refresh", 0) == PG_OK);
	FileListing *before = fileSystemList(system, NULL, 0);
	CHECK(before && before->count == 1);
	CHECK(DeleteFileA("refresh/old.txt"));
	FileListing *after = fileSystemList(system, NULL, 0);
	CHECK(after && after->count == 0);
	CHECK(before->count == 1 && !strcmp(before->entries[0].path, "old.txt"));
	fileListingFree(&after);
	fileListingFree(&before);
	fileSystemDestroy(&system);
	return 0;
}

static int explicit_archive_identity(void)
{
	pg_archive_builder *builder = NULL;
	pg_reader *reader = NULL;
	char bytes[8];
	size_t count;
	CHECK(!archive("identity.pigg", 10000));
	CHECK(pg_archive_builder_create(fixture_context, "identity.pigx",
		PG_PIGG2, PG_OVERWRITE, &builder, NULL) == PG_OK);
	CHECK(pg_archive_builder_write_all(builder, "dir/file", "other", 5,
		NULL, NULL) == PG_OK);
	CHECK(pg_archive_builder_finish(builder, NULL) == PG_OK);
	CHECK(pg_archive_builder_close(&builder, NULL) == PG_OK);
	fileSystemSetMode(FILE_MODE_ARCHIVES);
	FileSystem *system = fileSystemCreate();
	CHECK(system);
	CHECK(fileSystemAddSource(system, "identity.pigg", 0) == PG_OK);
	CHECK(fileSystemOpenPath("identity.pigx:/dir/file", &reader) == PG_OK);
	CHECK(pg_reader_read(reader, bytes, sizeof(bytes), &count, NULL) == PG_OK);
	CHECK(count == 5 && !memcmp(bytes, "other", 5));
	CHECK(pg_reader_close(&reader, NULL) == PG_OK);
	fileSystemDestroy(&system);
	return 0;
}

static int refreshed_selection(void)
{
	_mkdir("selected");
	CHECK(!put("selected/file", "before", 16000));
	fileSystemSetMode(FILE_MODE_HYBRID);
	FileSystem *system = fileSystemCreate();
	CHECK(system && fileSystemAddSource(system, "selected", 0) == PG_OK);
	FileSelection *before = fileSystemSelect(system, "file");
	CHECK(before && fileSelectionEntry(before)->size == 6);
	CHECK(!put("selected/file", "after-change", 16001));
	Sleep(100);
	FileSelection *after = fileSystemSelect(system, "file");
	CHECK(after && fileSelectionEntry(after)->size == 12);
	CHECK(fileSelectionInfo(after)->logical_size == 12);
	CHECK(fileSelectionEntry(before)->size == 6);
	fileSelectionFree(&after);
	fileSelectionFree(&before);
	fileSystemDestroy(&system);
	return 0;
}

static int directory_and_priority_cases(void)
{
	_mkdir("priority-low");
	_mkdir("priority-low/empty");
	_mkdir("priority-high");
	CHECK(!put("priority-low/file", "low", 17000));
	CHECK(!put("priority-high/file", "higher", 17001));
	fileSystemSetMode(FILE_MODE_HYBRID);
	FileSystem *system = fileSystemCreate();
	CHECK(system);
	CHECK(fileSystemAddSource(system, "priority-low", 0) == PG_OK);
	CHECK(fileSystemAddSource(system, "priority-high", 1) == PG_OK);
	CHECK(fileSystemAddSource(system, "priority-low", 0) == PG_EXISTS);
	FileSelection *selected = fileSystemSelect(system, "file");
	CHECK(selected && fileSelectionEntry(selected)->size == 6);
	fileSelectionFree(&selected);
	callbacks = 0;
	CHECK(fileSystemSubscribe(system, "file", FILE_CHANGE_UPDATE |
		FILE_CHANGE_DELETE, changed, NULL) == PG_OK);
	CHECK(!put("priority-low/file", "low2", 17002));
	for (int i = 0; i < 10; i++) {
		Sleep(10);
		CHECK(fileSystemDispatch() == PG_OK);
	}
	CHECK(callbacks == 0);
	CHECK(DeleteFileA("priority-high/file"));
	for (int i = 0; i < 100 && !callbacks; i++) {
		Sleep(10);
		CHECK(fileSystemDispatch() == PG_OK);
	}
	CHECK(callbacks == 1 && last_change == FILE_CHANGE_UPDATE);
	selected = fileSystemSelect(system, "file");
	CHECK(selected && fileSelectionEntry(selected)->size == 4);
	fileSelectionFree(&selected);
	selected = fileSystemSelect(system, "empty");
	CHECK(selected && fileSelectionEntry(selected)->kind == PG_ENTRY_DIRECTORY);
	fileSelectionFree(&selected);
	FileListing *listing = fileSystemList(system, "absent", 0);
	CHECK(listing && listing->count == 0);
	fileListingFree(&listing);
	CHECK(!put("priority-low/d_hidden.txt", "hidden", 17000));
	selected = fileSystemSelect(system, "d_hidden.txt");
	CHECK(selected == NULL);
	fileSystemDestroy(&system);
	return 0;
}

static int shallow_lookup(void)
{
	pg_context *context = NULL;
	pg_source *source = NULL;
	pg_file *file = NULL;
	pg_file_info info;
	_mkdir("shallow");
	_mkdir("shallow/nested");
	DeleteFileA("shallow/new");
	CHECK(!put("shallow/file", "indexed", 18000));
	CHECK(!put("shallow/nested/file", "nested", 18000));
	CHECK(pg_context_open(&context, NULL) == PG_OK);
	CHECK(pg_source_open(context, "shallow", NULL, &source, NULL) == PG_OK);
	CHECK(pg_source_discover(source, NULL, PG_DISCOVER_CHILDREN, NULL) == PG_OK);
	CHECK(!put("shallow/file", "replaced", 18001));
	CHECK(!put("shallow/new", "new", 18001));
	/* A discovered child's metadata and absence stay indexed until refresh.
	 * Descendants of a shallow directory still require an exact native probe. */
	CHECK(pg_source_find(source, "file", &file, NULL) == PG_OK);
	CHECK(pg_file_inspect(file, &info, NULL) == PG_OK);
	CHECK(info.logical_size == 7 && info.mtime == 18000);
	CHECK(pg_file_close(&file, NULL) == PG_OK);
	CHECK(pg_source_find(source, "new", &file, NULL) == PG_NOT_FOUND);
	CHECK(pg_source_find(source, "nested/file", &file, NULL) == PG_OK);
	CHECK(pg_file_close(&file, NULL) == PG_OK);
	CHECK(pg_source_discover(source, NULL, PG_DISCOVER_CHILDREN, NULL) == PG_OK);
	CHECK(pg_source_find(source, "file", &file, NULL) == PG_OK);
	CHECK(pg_file_inspect(file, &info, NULL) == PG_OK);
	CHECK(info.logical_size == 8 && info.mtime == 18001);
	CHECK(pg_file_close(&file, NULL) == PG_OK);
	CHECK(pg_source_close(&source, NULL) == PG_OK);
	CHECK(pg_context_close(&context, NULL) == PG_OK);
	return 0;
}

static int standard_ignores(void)
{
	fileSystemIgnoreStandard();
	CHECK(fileSystemIgnored("d_hidden/file"));
	CHECK(fileSystemIgnored("dir/D_hidden.txt"));
	CHECK(!fileSystemIgnored("v_assets/file"));
	CHECK(!fileSystemIgnored("dir/V_variant.txt"));
	_mkdir("standard");
	_mkdir("standard/v_assets");
	CHECK(!put("standard/v_assets/file", "visible", 19000));
	fileSystemSetMode(FILE_MODE_DYNAMIC);
	FileSystem *system = fileSystemCreate();
	CHECK(system && fileSystemAddSource(system, "standard", 0) == PG_OK);
	FileSelection *selection = fileSystemSelect(system, "V_ASSETS/FILE");
	CHECK(selection && fileSelectionEntry(selection)->size == 7);
	fileSelectionFree(&selection);
	fileSystemDestroy(&system);
	return 0;
}

static int captured_loose_selection(void)
{
	char path[128], bytes[32];
	size_t count;
	pg_reader *reader = NULL;
	fileSystemSetMode(FILE_MODE_DYNAMIC);
	FileSystem *system = fileSystemCreate();
	CHECK(system);
	/* Exercise the heap fallback for many sources and release every losing
	 * capture, while keeping the selected file's identity across a change. */
	for (unsigned i = 0; i < 9; i++) {
		sprintf(path, "capture-%u", i);
		_mkdir(path);
		strcat(path, "/file");
		CHECK(!put(path, i == 8 ? "winner" : "shadow", 20000));
		sprintf(path, "capture-%u", i);
		CHECK(fileSystemAddSource(system, path, i) == PG_OK);
	}
	FileSelection *before = fileSystemSelect(system, "file");
	CHECK(before && fileSelectionEntry(before)->size == 6);
	CHECK(strstr(fileSelectionEntry(before)->native_path, "capture-8/file"));
	CHECK(fileSelectionOpen(before, PG_READ_LOGICAL, &reader) == PG_OK);
	CHECK(pg_reader_read(reader, bytes, sizeof(bytes), &count, NULL) == PG_OK);
	CHECK(count == 6 && !memcmp(bytes, "winner", 6));
	CHECK(pg_reader_close(&reader, NULL) == PG_OK);
	CHECK(!put("capture-8/file", "replacement", 20001));
	Sleep(100);
	FileSelection *after = fileSystemSelect(system, "file");
	CHECK(after && fileSelectionEntry(after)->size == 11);
	CHECK(fileSelectionEntry(before)->size == 6);
	CHECK(fileSelectionOpen(before, PG_READ_LOGICAL, &reader) == PG_STALE);
	CHECK(reader == NULL);
	CHECK(fileSelectionOpen(after, PG_READ_LOGICAL, &reader) == PG_OK);
	fileSelectionFree(&after);
	fileSelectionFree(&before);
	fileSystemDestroy(&system);
	CHECK(pg_reader_read(reader, bytes, sizeof(bytes), &count, NULL) == PG_OK);
	CHECK(count == 11 && !memcmp(bytes, "replacement", 11));
	CHECK(pg_reader_close(&reader, NULL) == PG_OK);
	return 0;
}

static int opened_native_object(void)
{
	const char *path = "opened-object/file.txt";
	const char *moved = "opened-object/moved.txt";
	char bytes[32];
	size_t count;
	pg_reader *reader = NULL, *fresh = NULL;
	_mkdir("opened-object");
	DeleteFileA(moved);
	CHECK(!put(path, "original", 24000));
	fileSystemSetMode(FILE_MODE_DYNAMIC);
	fileSystemCallbacksEnabled(0);
	FileSystem *system = fileSystemCreate();
	CHECK(system && fileSystemAddSource(system, "opened-object", 0) == PG_OK);
	FileSelection *before = fileSystemSelect(system, "file.txt");
	CHECK(before);
	CHECK(fileSelectionOpen(before, PG_READ_LOGICAL, &reader) == PG_OK);
	CHECK(pg_reader_read(reader, bytes, 2, &count, NULL) == PG_OK);
	CHECK(count == 2 && !memcmp(bytes, "or", 2));
	CHECK(MoveFileExA(path, moved, 0));
	CHECK(!put(path, "replacement", 24001));
	CHECK(fileSelectionOpen(before, PG_READ_LOGICAL, &fresh) == PG_STALE);
	CHECK(!fresh);
	fileSelectionFree(&before);
	fileSystemDestroy(&system);
	system = fileSystemCreate();
	CHECK(system && fileSystemAddSource(system, "opened-object", 0) == PG_OK);
	FileSelection *after = fileSystemSelect(system, "file.txt");
	CHECK(after);
	CHECK(fileSelectionOpen(after, PG_READ_LOGICAL, &fresh) == PG_OK);
	fileSelectionFree(&after);
	CHECK(pg_reader_read(fresh, bytes, sizeof(bytes), &count, NULL) == PG_OK);
	CHECK(count == 11 && !memcmp(bytes, "replacement", 11));
	CHECK(pg_reader_close(&fresh, NULL) == PG_OK);
	CHECK(DeleteFileA(moved));
	fileSystemDestroy(&system);
	CHECK(!system);
	CHECK(pg_reader_read(reader, bytes, sizeof(bytes), &count, NULL) == PG_OK);
	CHECK(count == 6 && !memcmp(bytes, "iginal", 6));
	CHECK(pg_reader_read(reader, bytes, sizeof(bytes), &count, NULL) == PG_END);
	CHECK(count == 0);
	CHECK(pg_reader_seek(reader, 0, NULL) == PG_OK);
	CHECK(pg_reader_read(reader, bytes, sizeof(bytes), &count, NULL) == PG_OK);
	CHECK(count == 8 && !memcmp(bytes, "original", 8));
	CHECK(pg_reader_close(&reader, NULL) == PG_OK);
	CHECK(DeleteFileA(path));
	fileSystemCallbacksEnabled(1);
	return 0;
}

static int paused_cache(void)
{
	FileSystemEntry entry;
	char native[MAX_PATH], tiny_buffer[1];
	pg_reader *reader = NULL;
	_mkdir("paused");
	DeleteFileA("paused/new.txt");
	DeleteFileA("paused/third.txt");
	CHECK(!put("paused/file.txt", "before", 21000));
	fileSystemSetMode(FILE_MODE_DYNAMIC);
	FileSystem *system = fileSystemCreate();
	CHECK(system && fileSystemAddSource(system, "paused", 0) == PG_OK);
	fileSystemCallbacksEnabled(0);
	FileListing *list = fileSystemList(system, NULL, 0);
	CHECK(list && list->count == 1);
	CHECK(fileSystemQuery(system, "./FILE.TXT", &entry, native, sizeof(native)) == PG_OK);
	CHECK(entry.size == 6 && !entry.path && !entry.name);
	CHECK(entry.native_path == native && strstr(native, "paused/file.txt"));
	CHECK(fileSystemQuery(system, "file.txt", &entry, tiny_buffer, sizeof(tiny_buffer)) == PG_CAPACITY);
	CHECK(!entry.size && !entry.native_path && !tiny_buffer[0]);
	FileSelection *before = fileSystemSelect(system, "file.txt");
	CHECK(before && fileSelectionInfo(before)->logical_size == 6);
	CHECK(!put("paused/file.txt", "after-change", 21001));
	CHECK(!put("paused/new.txt", "new", 21001));
	Sleep(100);
	/* Loading uses the same cached metadata until observation resumes. */
	CHECK(fileSystemQuery(system, "file.txt", &entry, NULL, 0) == PG_OK);
	CHECK(entry.size == 6);
	FileSelection *cached = fileSystemSelect(system, "file.txt");
	CHECK(cached && fileSelectionEntry(cached)->size == 6);
	CHECK(fileSelectionOpen(cached, PG_READ_LOGICAL, &reader) == PG_STALE);
	CHECK(!reader);
	fileSelectionFree(&cached);
	FileListing *paused = fileSystemList(system, NULL, 0);
	CHECK(paused && paused->count == 1);
	fileListingFree(&paused);
	fileSystemCallbacksEnabled(1);
	CHECK(fileSystemDispatch() == PG_OK);
	CHECK(fileSystemQuery(system, "file.txt", &entry, NULL, 0) == PG_OK);
	CHECK(entry.size == 12);
	FileListing *resumed = fileSystemList(system, NULL, 0);
	CHECK(resumed && resumed->count == 2 && list->count == 1);
	fileListingFree(&resumed);
	CHECK(!put("paused/third.txt", "third", 21002));
	Sleep(100);
	fileSystemCallbacksEnabled(0);
	FileListing *next_phase = fileSystemList(system, NULL, 0);
	CHECK(next_phase && next_phase->count == 3);
	fileListingFree(&next_phase);
	fileSystemCallbacksEnabled(1);
	fileListingFree(&list);
	fileSystemDestroy(&system);
	/* Last captured-file release is queued to its owner after group teardown. */
	fileSelectionFree(&before);
	CHECK(fileSystemDispatch() == PG_OK);
	return 0;
}

static int growing_cache(void)
{
	pg_archive_builder *builder = NULL;
	pg_entry_options options = { 0 };
	char name[1024];
	CHECK(pg_archive_builder_create(fixture_context, "growth.pigg", PG_PIGG2,
		PG_OVERWRITE, &builder, NULL) == PG_OK);
	/* Many implied parents exercise rehashing during recursive insertion,
	 * without creating thousands of native fixture files. */
	for (unsigned i = 0; i < 160; i++) {
		sprintf(name, "tree%03u", i);
		for (unsigned depth = 0; depth < 64; depth++) strcat(name, "/branch");
		strcat(name, "/file");
		CHECK(pg_archive_builder_write_all(builder, name, NULL, 0,
			&options, NULL) == PG_OK);
	}
	CHECK(pg_archive_builder_finish(builder, NULL) == PG_OK);
	CHECK(pg_archive_builder_close(&builder, NULL) == PG_OK);
	fileSystemSetMode(FILE_MODE_ARCHIVES);
	FileSystem *system = fileSystemCreate();
	CHECK(system && fileSystemAddSource(system, "base.pigg", 0) == PG_OK);
	FileSelection *before = fileSystemSelect(system, "dir/file");
	CHECK(before);
	fileSystemCallbacksEnabled(0);
	CHECK(fileSystemAddSource(system, "growth.pigg", 1) == PG_OK);
	for (unsigned i = 0; i < 160; i++) {
		FileSystemEntry entry;
		sprintf(name, "tree%03u", i);
		for (unsigned depth = 0; depth < 64; depth++) strcat(name, "/branch");
		strcat(name, "/file");
		CHECK(fileSystemQuery(system, name, &entry, NULL, 0) == PG_OK);
		CHECK(entry.kind == PG_ENTRY_FILE && entry.size == 0);
	}
	FileListing *list = fileSystemList(system, NULL, 0);
	CHECK(list && list->count == 161 && !strcmp(list->entries[0].path, "dir"));
	CHECK(!strcmp(list->entries[160].path, "tree159"));
	fileListingFree(&list);
	CHECK(fileSelectionEntry(before)->size == 7);
	fileSelectionFree(&before);
	fileSystemDestroy(&system);
	fileSystemCallbacksEnabled(1);
	return 0;
}

static int cached_subtrees(void)
{
	FileSystemEntry entry;
	callbacks = 0;
	_mkdir("subtrees");
	_mkdir("subtrees/one");
	_mkdir("subtrees/one/nested");
	_mkdir("subtrees/one_more");
	DeleteFileA("subtrees/one/nested/later");
	DeleteFileA("subtrees/one_more/new");
	CHECK(!put("subtrees/one/nested/file", "first", 22000));
	fileSystemSetMode(FILE_MODE_DYNAMIC);
	fileSystemCallbacksEnabled(0);
	FileSystem *system = fileSystemCreate();
	CHECK(system && fileSystemAddSource(system, "subtrees", 0) == PG_OK);
	CHECK(fileSystemCacheTree(system, "ONE", 0) == PG_OK);
	CHECK(fileSystemSubscribe(system, "one/nested/file",
		FILE_CHANGE_UPDATE | FILE_CHANGE_DELETE, changed, NULL) == PG_OK);
	CHECK(!put("subtrees/one/nested/file", "updated", 22002));
	CHECK(!put("subtrees/one/nested/later", "late", 22001));
	CHECK(!put("subtrees/one_more/new", "separate", 22001));
	CHECK(fileSystemQuery(system, "one/nested/file", &entry, NULL, 0) == PG_OK);
	CHECK(entry.size == 5);
	FileListing *frozen = fileSystemList(system, "one/nested", 0);
	CHECK(frozen && frozen->count == 1);
	/* A recursive prefix cannot claim either the whole source or a sibling. */
	CHECK(fileSystemQuery(system, "one_more/new", &entry, NULL, 0) == PG_OK);
	CHECK(entry.size == 8);
	fileSystemCallbacksEnabled(1);
	for (int i = 0; i < 100 && !callbacks; i++) {
		Sleep(10);
		CHECK(fileSystemDispatch() == PG_OK);
	}
	CHECK(callbacks == 1 && last_change == FILE_CHANGE_UPDATE);
	CHECK(fileSystemQuery(system, "one/nested/file", &entry, NULL, 0) == PG_OK);
	CHECK(entry.size == 7);
	FileListing *refreshed = fileSystemList(system, "one/nested", 0);
	CHECK(refreshed && refreshed->count == 2 && frozen->count == 1);
	fileListingFree(&refreshed);
	fileListingFree(&frozen);
	CHECK(DeleteFileA("subtrees/one/nested/file"));
	for (int i = 0; i < 100 && callbacks < 2; i++) {
		Sleep(10);
		CHECK(fileSystemDispatch() == PG_OK);
	}
	CHECK(callbacks == 2 && last_change == FILE_CHANGE_DELETE);
	fileSystemCallbacksEnabled(0);
	CHECK(fileSystemCacheTree(system, "one", 0) == PG_OK);
	CHECK(fileSystemQuery(system, "one/nested/later", &entry, NULL, 0) == PG_OK);
	CHECK(entry.size == 4);
	fileSystemDestroy(&system);
	fileSystemCallbacksEnabled(1);
	return 0;
}

typedef struct TeardownCallback {
	FileSystem *system;
	unsigned calls;
} TeardownCallback;

static void teardown_changed(const FileChange *change, void *user)
{
	TeardownCallback *state = user;
	(void)change;
	state->calls++;
	fileSystemDestroy(&state->system);
}

static int callback_teardown(void)
{
	TeardownCallback state = { 0 };
	_mkdir("callback-teardown");
	_mkdir("callback-teardown/destroy");
	CHECK(!put("callback-teardown/destroy/a", "old", 23000));
	CHECK(!put("callback-teardown/destroy/b", "old", 23000));
	fileSystemSetMode(FILE_MODE_LOOSE);
	fileSystemCallbacksEnabled(1);
	state.system = fileSystemCreate();
	CHECK(state.system);
	CHECK(fileSystemAddSource(state.system, "callback-teardown", 0) == PG_OK);
	CHECK(fileSystemPrepareTree(state.system, "destroy", 0) == PG_OK);
	CHECK(fileSystemSubscribe(NULL, "destroy/*", FILE_CHANGE_UPDATE,
		teardown_changed, &state) == PG_OK);
	CHECK(!put("callback-teardown/destroy/a", "changed", 23001));
	CHECK(!put("callback-teardown/destroy/b", "changed", 23001));
	Sleep(30);
	CHECK(fileSystemDispatch() == PG_OK);
	CHECK(state.calls == 1 && !state.system);
	return 0;
}

int main(void)
{
	CHECK(pg_context_open(&fixture_context, NULL) == PG_OK);
	CHECK(standard_ignores() == 0);
	CHECK(captured_loose_selection() == 0);
	CHECK(opened_native_object() == 0);
	CHECK(paused_cache() == 0);
	CHECK(shallow_lookup() == 0);
	CHECK(directory_and_priority_cases() == 0);
	CHECK(explicit_archive_identity() == 0);
	CHECK(refreshed_selection() == 0);
	CHECK(selection_cases() == 0);
	CHECK(direct_workers() == 0);
	CHECK(growing_cache() == 0);
	CHECK(cached_subtrees() == 0);
	CHECK(lazy_callbacks() == 0);
	CHECK(dynamic_selection() == 0);
	CHECK(listing_refresh() == 0);
	CHECK(callback_teardown() == 0);
	CHECK(pg_context_close(&fixture_context, NULL) == PG_OK);
	puts("Piggle filesystem checks passed");
	return 0;
}
