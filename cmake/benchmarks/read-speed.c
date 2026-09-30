#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "utilitieslib/utils/FileSystem.h"

static double elapsed(LARGE_INTEGER start, LARGE_INTEGER frequency)
{
	LARGE_INTEGER end;
	QueryPerformanceCounter(&end);
	return (end.QuadPart - start.QuadPart) * 1000.0 / frequency.QuadPart;
}
int main(int argc, char **argv)
{
	const unsigned count = 2000;
	LARGE_INTEGER frequency, start;
	QueryPerformanceFrequency(&frequency);
	FileSystem *system = fileSystemCreate();
	fileSystemSetMode(FILE_MODE_DYNAMIC);
	fileSystemCallbacksEnabled(0);
	if (argc != 3 || fileSystemAddSource(system, argv[1], 0) != PG_OK)
		return 1;
	char *files[1024], line[MAX_PATH];
	size_t file_count = 0;
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
	if (!file_count || fileSystemPrepareTree(system, NULL, 0) != PG_OK)
		return 4;
	printf("%zu files; %u selections/whole-file reads per pass; callbacks "
		   "disabled\n",
		file_count, count);
	for (unsigned pass = 0; pass < 5; pass++) {
		QueryPerformanceCounter(&start);
		for (unsigned i = 0; i < count; i++) {
			FileSelection *selection =
				fileSystemSelect(system, files[i % file_count]);
			if (!selection)
				return 3;
			fileSelectionFree(&selection);
		}
		printf("select %.2f ms; ", elapsed(start, frequency));
		QueryPerformanceCounter(&start);
		for (unsigned i = 0; i < count; i++) {
			FileSystemEntry entry;
			if (fileSystemQuery(
					system, files[i % file_count], &entry, NULL, 0) != PG_OK)
				return 10;
		}
		printf("metadata %.2f ms; ", elapsed(start, frequency));
		for (unsigned piggle = 0; piggle < 2; piggle++) {
			unsigned long long total = 0, checksum = 0;
			QueryPerformanceCounter(&start);
			for (unsigned i = 0; i < count; i++) {
				char bytes[32768];
				size_t got;
				FileSelection *selection =
					fileSystemSelect(system, files[i % file_count]);
				if (!selection)
					return 5;
				FILE *native = NULL;
				pg_reader *reader = NULL;
				if (piggle) {
					if (fileSelectionOpen(
							selection, PG_READ_LOGICAL, &reader) != PG_OK)
						return 6;
				} else {
					native =
						fopen(fileSelectionEntry(selection)->native_path, "rb");
					if (!native)
						return 7;
				}
				fileSelectionFree(&selection);
				do {
					if (piggle) {
						pg_status status = pg_reader_read(
							reader, bytes, sizeof(bytes), &got, NULL);
						if (status != PG_OK && status != PG_END)
							return 8;
					} else {
						got = fread(bytes, 1, sizeof(bytes), native);
						if (ferror(native))
							return 9;
					}
					total += got;
					for (size_t j = 0; j < got; j++)
						checksum += (unsigned char)bytes[j];
				} while (got);
				if (piggle)
					pg_reader_close(&reader, NULL);
				else
					fclose(native);
			}
			printf("%s %.2f ms (%llu bytes, checksum %llu); ",
				piggle ? "Piggle reader" : "native reader",
				elapsed(start, frequency), total, checksum);
		}
		printf("\n");
		fflush(stdout);
	}
	for (size_t i = 0; i < file_count; i++)
		free(files[i]);
	fileSystemDestroy(&system);
	return 0;
}
