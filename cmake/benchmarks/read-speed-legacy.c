#include "utilitieslib/utils/wininclude.h"
#include <stdio.h>
#include <stdlib.h>
#include "utilitieslib/stdtypes.h"
#include "utilitieslib/utils/FolderCache.h"
#include "utilitieslib/utils/memcheck.h"
#include "utilitieslib/utils/SuperAssert.h"
#undef fopen
#undef malloc
#undef free
#undef strcpy

static double elapsed(LARGE_INTEGER start, LARGE_INTEGER frequency)
{
	LARGE_INTEGER end;
	QueryPerformanceCounter(&end);
	return (end.QuadPart - start.QuadPart) * 1000.0 / frequency.QuadPart;
}
int main(int argc, char **argv)
{
	const unsigned count = 2000;
	char *files[1024], line[MAX_PATH];
	size_t file_count = 0;
	LARGE_INTEGER frequency, start;
	QueryPerformanceFrequency(&frequency);
	memCheckInit();
	setGuiDisable(1);
	if (argc != 3)
		return 1;
	FILE *manifest = fopen(argv[2], "rb");
	if (!manifest)
		return 2;
	while (fgets(line, sizeof(line), manifest)) {
		line[strcspn(line, "\r\n")] = 0;
		if (!*line || file_count == 1024)
			return 3;
		files[file_count++] = _strdup(line);
	}
	fclose(manifest);
	FolderCacheSetMode(FOLDER_CACHE_MODE_DEVELOPMENT_DYNAMIC);
	FolderCacheEnableCallbacks(0);
	FolderCache *cache = FolderCacheCreate();
	FolderCacheAddFolder(cache, argv[1], 0);
	for (size_t i = 0; i < file_count; i++)
		if (!FolderCacheQuery(cache, files[i]))
			return 4;
	printf("%zu files; %u operations per pass; pre-Piggle FolderCache; "
		   "callbacks disabled\n",
		file_count, count);
	for (unsigned pass = 0; pass < 5; pass++) {
		QueryPerformanceCounter(&start);
		for (unsigned i = 0; i < count; i++) {
			if (!FolderCacheQuery(cache, files[i % file_count]))
				return 5;
		}
		printf("select %.2f ms; ", elapsed(start, frequency));
		unsigned long long total = 0, checksum = 0;
		QueryPerformanceCounter(&start);
		for (unsigned i = 0; i < count; i++) {
			char bytes[32768], path[MAX_PATH];
			size_t got;
			FolderNode *node = FolderCacheQuery(cache, files[i % file_count]);
			if (!node ||
				!FolderCacheGetRealPath(cache, node, path, sizeof(path)))
				return 6;
			FILE *native = fopen(path, "rb");
			if (!native)
				return 7;
			do {
				got = fread(bytes, 1, sizeof(bytes), native);
				if (ferror(native))
					return 8;
				total += got;
				for (size_t j = 0; j < got; j++)
					checksum += (unsigned char)bytes[j];
			} while (got);
			fclose(native);
		}
		printf("native reader %.2f ms (%llu bytes, checksum %llu)\n",
			elapsed(start, frequency), total, checksum);
		fflush(stdout);
	}
	FolderCacheDestroy(cache);
	for (size_t i = 0; i < file_count; i++)
		free(files[i]);
	return 0;
}
