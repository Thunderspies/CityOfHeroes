#include <stdio.h>
#include <string.h>
#include "utilitieslib/stdtypes.h"
#include "utilitieslib/utils/wininclude.h"
#include "utilitieslib/utils/memcheck.h"
#include "utilitieslib/utils/SuperAssert.h"
#include "utilitieslib/utils/cpu_count.h"
#include "utilitieslib/utils/utils.h"
#include "utilitieslib/utils/fileWatch.h"
#include "utilitieslib/components/earray.h"
#include "utilitieslib/components/EString.h"
#include "utilitieslib/components/StashTable.h"
#include "utilitieslib/components/BloomFilter.h"
#include "utilitieslib/UtilsNew/Array.h"
#include "utilitieslib/UtilsNew/Str.h"
#include "checks.h"

/* Fault probes also exercise threads outside the project wrapper. */
#undef CreateThread

static COH_THREAD_LOCAL int thread_value = 7;

static LONG WINAPI previous_filter(EXCEPTION_POINTERS *info)
{
	ExitProcess(info->ExceptionRecord->ExceptionCode == 0xe0000042 ? 1 : 3);
	return EXCEPTION_CONTINUE_SEARCH;
}

static DWORD WINAPI fault_thread(void *unused)
{
	RaiseException(0xe0000042, EXCEPTION_NONCONTINUABLE, 0, NULL);
	return 2;
}

static unsigned __stdcall check_thread(void *unused)
{
	CHECK(thread_value == 7);
	thread_value = 42;
	return 0;
}

static int check_tls(void)
{
	thread_value = 19;
	unsigned id;
	HANDLE thread = (HANDLE)x_beginthreadex(NULL, 0, check_thread, NULL,
		CREATE_SUSPENDED, &id, "UtilitiesLibTests", 32);
	CHECK(thread != NULL);
#ifdef __MINGW32__
	typedef HRESULT (WINAPI *GetDescription)(HANDLE, PWSTR *);
	GetDescription get_description = (GetDescription)GetProcAddress(
		GetModuleHandleA("kernel32.dll"), "GetThreadDescription");
	if (get_description) {
		PWSTR name = NULL;
		CHECK(SUCCEEDED(get_description(thread, &name)));
		CHECK(name != NULL);
		int matches = wcscmp(name, L"UtilitiesLibTests(32)") == 0;
		LocalFree(name);
		CHECK(matches);
	}
#endif
	CHECK(ResumeThread(thread) != (DWORD)-1);
	CHECK(WaitForSingleObject(thread, 10000) == WAIT_OBJECT_0);
	DWORD result;
	CHECK(GetExitCodeThread(thread, &result));
	CloseHandle(thread);
	CHECK(result == 0 && thread_value == 19);
	return 0;
}

static int check_arrays(void)
{
	int entries[128];
	int **array = NULL, **copy = NULL;
	const int **constant = NULL;
	int *integers = NULL;
	U32 *unsigneds = NULL;
	F32 *floats = NULL;
	for (int i = 0; i < 128; ++i) {
		entries[i] = i;
		eaPush(&array, &entries[i]);
		eaPushConst(&constant, &entries[i]);
		eaiPush(&integers, -i);
		ea32Push(&unsigneds, 0x80000000u + i);
		eafPush(&floats, i + 0.5f);
	}
	CHECK(eaSize(&array) == 128 && eaSize(&constant) == 128);
	CHECK(eaCapacity(&array) >= 128);
	eaCopy(&copy, &array);
	eaReverse(&copy);
	CHECK(eaGet(&copy, 0) == &entries[127]);
	CHECK(eaGetConst(&constant, 127) == &entries[127]);
	CHECK(eaRemove(&copy, 3) == &entries[124]);
	CHECK(eaSize(&copy) == 127);
	CHECK(eaiSize(&integers) == 128 && eaiPop(&integers) == -127);
	CHECK(ea32Size(&unsigneds) == 128);
	CHECK(eaiSize(&unsigneds) == 128 && unsigneds[127] == 0x8000007fu);
	CHECK(eafSize(&floats) == 128 && floats[127] == 127.5f);
	eaDestroy(&array);
	eaDestroy(&copy);
	eaDestroyConst(&constant);
	eaiDestroy(&integers);
	ea32Destroy(&unsigneds);
	eafDestroy(&floats);
	CHECK(!array && !copy && !constant && !integers && !floats);

	int **pointers = NULL;
	int *numbers = NULL;
	for (int i = 0; i < 128; ++i) {
		ap_push(&pointers, &entries[i]);
		aint_push(&numbers, i);
	}
	CHECK(ap_size(&pointers) == 128 && aint_size(&numbers) == 128);
	CHECK(*ap_pop(&pointers) == &entries[127]);
	CHECK(*aint_pop(&numbers) == 127);
	ap_destroy(&pointers, NULL);
	aint_destroy(&numbers);
	return 0;
}

static int check_stash(void)
{
	int value = 37;
	StashTable table = stashTableCreateWithStringKeys(4, StashDefault);
	CHECK(table != NULL);
	CHECK(stashAddPointer(table, "entry", &value, false));
	int *found = NULL;
	CHECK(stashFindPointer(table, "entry", &found) && found == &value);
	const int *readonly = NULL;
	CHECK(stashFindPointerConst(table, "entry", &readonly));
	CHECK(readonly == &value);
	StashTableIterator mutable_iter;
	StashElement mutable_element;
	stashGetIterator(table, &mutable_iter);
	CHECK(stashGetNextElement(&mutable_iter, &mutable_element));
	CHECK(stashElementGetPointer(mutable_element) == &value);
	cStashTableIterator iter;
	cStashElement element;
	stashGetIteratorConst(table, &iter);
	CHECK(stashGetNextElementConst(&iter, &element));
	CHECK(stashElementGetPointerConst(element) == &value);
	CHECK(!stashGetNextElementConst(&iter, &element));
	CHECK(stashRemovePointer(table, "entry", &found) && found == &value);
	CHECK(!stashFindPointer(table, "entry", &found));
	stashTableDestroy(table);
	table = stashTableCreateInt(4);
	CHECK(stashIntAddPointer(table, 0x1234567, &value, false));
	CHECK(stashIntFindPointer(table, 0x1234567, &found));
	CHECK(found == &value);
	stashTableDestroy(table);
	BloomFilter *filter = bloomFilterCreate(128, 0.01f);
	CHECK(filter != NULL);
	for (U32 i = 0; i < 128; ++i)
		bloomAddIntKey(filter, i);
	for (U32 i = 0; i < 128; ++i)
		CHECK(bloomFindIntKey(filter, i));
	bloomFilterDestroy(filter);
	return 0;
}

static int check_strings(void)
{
	char *text = NULL, *packed = NULL, *unpacked = NULL;
	estrPrintf(&text, "%s %d", "MinGW", 32);
	estrConcatf(&text, " %.2f", 1.25);
	CHECK(strcmp(text, "MinGW 32 1.25") == 0);
	CHECK(estrLength(&text) == strlen(text));
	estrPackData(&packed, text, estrLength(&text));
	estrUnpackStr(&unpacked, (const char *const *)&packed);
	CHECK(estrLength(&unpacked) == estrLength(&text));
	CHECK(memcmp(text, unpacked, estrLength(&text)) == 0);
	estrDestroy(&text);
	estrDestroy(&packed);
	estrDestroy(&unpacked);
	Str string = NULL;
	Str_printf(&string, "literal");
	Str_catf(&string, " suffix");
	CHECK(strcmp(string, "literal suffix") == 0);
	Str_printf(&string, "%s %d", "value", 42);
	Str_catf(&string, " %.1f", 1.5);
	CHECK(strcmp(string, "value 42 1.5") == 0);
	Str_destroy(&string);
	CHECK(string == NULL);
	return 0;
}

static int check_allocation(void)
{
#if _CRTDBG_MAP_ALLOC
	char *buffer = malloc_timed(8, _NORMAL_BLOCK, __FILE__, __LINE__);
	CHECK(buffer != NULL);
	memcpy(buffer, "payload", 8);
	buffer = realloc_timed(buffer, 1024, _NORMAL_BLOCK, __FILE__, __LINE__);
	CHECK(buffer != NULL && strcmp(buffer, "payload") == 0);
	free_timed(buffer, _NORMAL_BLOCK);
#endif
	CHECK(check_cpp_allocation() == 0);
	return 0;
}

static int check_file_stat(void)
{
	struct {
		struct _stat32 info;
		unsigned char guard[64];
	} result;
	char path[MAX_PATH];
	DWORD length = GetModuleFileNameA(NULL, path, sizeof(path));
	CHECK(length > 0 && length < sizeof(path));
	memset(&result, 0xa5, sizeof(result));
	CHECK(fwStat(path, &result.info) == 0);
	for (size_t i = 0; i < sizeof(result.guard); ++i)
		CHECK(result.guard[i] == 0xa5);
	CHECK((result.info.st_mode & _S_IFREG) != 0);
	CHECK(result.info.st_size > 0);
	CHECK(fwStat(path, NULL) == 0);
	return 0;
}

static int check_dump(void)
{
#ifndef DISABLE_ASSERTIONS
	char path[MAX_PATH + 5];
	CHECK(GetModuleFileNameA(NULL, path, MAX_PATH) < MAX_PATH);
	strcat(path, ".mdmp");
	DeleteFileA(path);
	assertWriteMiniDumpSimple(NULL);
	HANDLE dump = CreateFileA(path, GENERIC_READ, FILE_SHARE_READ,
		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	CHECK(dump != INVALID_HANDLE_VALUE);
	char signature[4];
	DWORD bytes = 0;
	BOOL read = ReadFile(dump, signature, sizeof(signature), &bytes, NULL);
	CloseHandle(dump);
	DeleteFileA(path);
	CHECK(read && bytes == 4 && memcmp(signature, "MDMP", 4) == 0);
#endif
	return 0;
}

static int check_child(const char *mode)
{
	char path[MAX_PATH], command[MAX_PATH + 32];
	CHECK(GetModuleFileNameA(NULL, path, sizeof(path)) < sizeof(path));
	CHECK(snprintf(command, sizeof(command), "\"%s\" %s", path, mode) > 0);
	STARTUPINFOA startup = {0};
	PROCESS_INFORMATION process = {0};
	startup.cb = sizeof(startup);
	CHECK(CreateProcessA(path, command, NULL, NULL, FALSE, 0, NULL, NULL,
		&startup, &process));
	DWORD wait = WaitForSingleObject(process.hProcess, 30000);
	if (wait != WAIT_OBJECT_0)
		TerminateProcess(process.hProcess, 99);
	DWORD code = 99;
	GetExitCodeProcess(process.hProcess, &code);
	CloseHandle(process.hThread);
	CloseHandle(process.hProcess);
	DWORD expected = 1;
#ifndef DISABLE_ASSERTIONS
	/* MSVC's SEH block guards only the calling thread; a raw worker
	 * reaches previous_filter. MinGW installs a process-wide handler.
	 * The Debug reporter terminates through SIGABRT with exit code 3.
	 */
#ifdef __MINGW32__
	expected = 3;
#else
	if (strcmp(mode, "--fault") == 0)
		expected = 3;
#endif
#endif
	CHECK(wait == WAIT_OBJECT_0 && code == expected);
	return 0;
}

int main(int argc, char **argv)
{
	memCheckInit();
	setAssertUnitTesting(true);
	setAssertMode(ASSERTMODE_EXIT | ASSERTMODE_STDERR);
	if (argc == 2) {
		SetUnhandledExceptionFilter(previous_filter);
		EXCEPTION_HANDLER_BEGIN
		if (strcmp(argv[1], "--worker-fault") == 0) {
			HANDLE worker = CreateThread(NULL, 0, fault_thread,
				NULL, 0, NULL);
			CHECK(worker != NULL);
			WaitForSingleObject(worker, 10000);
			CloseHandle(worker);
		} else if (strcmp(argv[1], "--fault") == 0) {
			fault_thread(NULL);
		}
		EXCEPTION_HANDLER_END
		return 2;
	}
	CHECK(sizeof(void *) == 4);
	CHECK(check_file_stat() == 0);
	CHECK(check_tls() == 0);
	CHECK(check_arrays() == 0);
	CHECK(check_stash() == 0);
	CHECK(check_nchash() == 0);
	CHECK(check_strings() == 0);
	CHECK(check_allocation() == 0);
	CHECK(check_dump() == 0);
	CHECK(getNumVirtualCpus() > 0 && getNumRealCpus() > 0);
	CHECK(check_child("--fault") == 0);
	CHECK(check_child("--worker-fault") == 0);
	puts("UtilitiesLib integration checks passed");
	return 0;
}
