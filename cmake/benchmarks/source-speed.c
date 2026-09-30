#include <windows.h>
#include <stdio.h>
#include <piggle/piggle.h>
static double elapsed(LARGE_INTEGER start, LARGE_INTEGER frequency)
{
	LARGE_INTEGER end;
	QueryPerformanceCounter(&end);
	return (end.QuadPart - start.QuadPart) * 1000.0 / frequency.QuadPart;
}
int main(int argc, char **argv)
{
	pg_context *context = NULL;
	pg_source *source = NULL;
	LARGE_INTEGER frequency, start;
	QueryPerformanceFrequency(&frequency);
	if (argc != 4 || pg_context_open(&context, NULL) != PG_OK ||
		pg_source_open(context, argv[1], NULL, &source, NULL) != PG_OK)
		return 1;
	QueryPerformanceCounter(&start);
	if (pg_source_discover(source, NULL, PG_DISCOVER_RECURSIVE, NULL) != PG_OK)
		return 2;
	printf("recursive discovery %.2f ms\n", elapsed(start, frequency));
	fflush(stdout);
	for (unsigned pass = 0; pass < 5; pass++) {
		QueryPerformanceCounter(&start);
		for (unsigned i = 0; i < 2000; i++) {
			pg_file *file = NULL;
			if (pg_source_find(source, argv[2], &file, NULL) != PG_OK)
				return 3;
			pg_file_close(&file, NULL);
		}
		printf("2000 lookups %.2f ms; ", elapsed(start, frequency));
		QueryPerformanceCounter(&start);
		size_t files = 0, dirs = 0;
		for (unsigned i = 0; i < 20; i++) {
			pg_entry_cursor *cursor = NULL;
			pg_file *file = NULL;
			pg_entry_info info;
			pg_status status;
			if (pg_source_entries(source, argv[3], 0, &cursor, NULL) != PG_OK)
				return 4;
			while ((status = pg_entry_cursor_next(
						cursor, &info, &file, NULL)) == PG_OK) {
				if (info.kind == PG_ENTRY_FILE)
					files++;
				else
					dirs++;
				if (file)
					pg_file_close(&file, NULL);
			}
			if (status != PG_END)
				return 5;
			pg_entry_cursor_close(&cursor, NULL);
		}
		printf("20 scoped listings %.2f ms (%zu files, %zu dirs)\n",
			elapsed(start, frequency), files, dirs);
		fflush(stdout);
	}
	pg_source_close(&source, NULL);
	pg_context_close(&context, NULL);
	return 0;
}
