#include "utilitieslib/utils/wininclude.h"
#include "utilitieslib/utils/utils.h"
#include "utilitieslib/utils/file.h"
#include "utilitieslib/utils/memcheck.h"
#include "utilitieslib/utils/SuperAssert.h"
#include "checks.h"
#undef fprintf

static int make_archive(pg_context *context, const char *path, uint32_t format,
	const char *text)
{
	pg_archive_builder *builder = NULL;
	pg_entry_options entry = { 0 };
	entry.compression = PG_COMPRESS_FORCE;
	CHECK(pg_archive_builder_create(context, path, format, PG_OVERWRITE,
		&builder, NULL) == PG_OK);
	CHECK(pg_archive_builder_write_all(builder, "dir/file", text, strlen(text),
		&entry, NULL) == PG_OK);
	CHECK(pg_archive_builder_finish(builder, NULL) == PG_OK);
	CHECK(pg_archive_builder_close(&builder, NULL) == PG_OK);
	return 0;
}

static unsigned visited_files, visited_directories, stopped;

static FileScanAction visit_native(char *directory, struct _finddata32_t *entry)
{
	(void)directory;
	if (stopped) {
		visited_files++;
		return FSA_STOP;
	}
	if (entry->attrib & _A_SUBDIR) {
		visited_directories++;
		return !strcmp(entry->name, "skip") ? FSA_NO_EXPLORE_DIRECTORY :
			FSA_EXPLORE_DIRECTORY;
	}
	visited_files++;
	return FSA_NO_EXPLORE_DIRECTORY;
}

static int native_walk(void)
{
	char root[MAX_PATH], path[MAX_PATH];
	CHECK(GetFullPathNameA("walk", sizeof(root), root, NULL));
	sprintf(path, "%s/keep/file", root);
	mkdirtree(path);
	FILE *file = fopen(path, "wb");
	CHECK(file && fclose(file) == 0);
	sprintf(path, "%s/skip/hidden", root);
	mkdirtree(path);
	file = fopen(path, "wb");
	CHECK(file && fclose(file) == 0);
	fileScanDirRecurseEx(root, visit_native);
	CHECK(visited_files == 1 && visited_directories == 2);
	visited_files = 0;
	stopped = 1;
	fileScanDirRecurseEx(root, visit_native);
	CHECK(visited_files == 1);
	rmdirtreeEx(root, 1);
	CHECK(GetFileAttributesA(root) == INVALID_FILE_ATTRIBUTES);
	return 0;
}

int main(void)
{
	pg_context *context = NULL;
	char bytes[32];
	memCheckInit();
	setGuiDisable(true);
	setAssertUnitTesting(true);
	setAssertMode(ASSERTMODE_EXIT | ASSERTMODE_STDERR);
	CHECK(native_walk() == 0);
	CHECK(pg_context_open(&context, NULL) == PG_OK);
	CHECK(!make_archive(context, "explicit.pigg", PG_PIGG2, "first\r\nlast"));
	CHECK(!make_archive(context, "explicit.hogg", PG_HOGG10, "second"));
	FILE *first = fopen("./explicit.pigg:/dir/file", "rb");
	FILE *second = fopen("./explicit.hogg:/dir/file", "rb");
	CHECK(first && second);
	CHECK(fileGetSize(first) == 11 && fileGetSize(second) == 6);
	CHECK(fread(bytes, 1, 6, second) == 6 && !memcmp(bytes, "second", 6));
	CHECK(fclose(second) == 0);
	CHECK(fgets(bytes, sizeof(bytes), first) && !strcmp(bytes, "first\n"));
	CHECK(ftell(first) == 7);
	CHECK(fseek(first, -4, SEEK_END) == 0 && fgetc(first) == 'l');
	CHECK(fseek(first, 0, SEEK_SET) == 0 && fgetc(first) == 'f');
	CHECK(fclose(first) == 0);
	CHECK(pg_context_close(&context, NULL) == PG_OK);
	puts("Piggle FileWrapper checks passed");
	return 0;
}
