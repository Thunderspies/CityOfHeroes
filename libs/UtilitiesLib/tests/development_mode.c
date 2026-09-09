#include <stdio.h>
#include <string.h>
#include "utilitieslib/stdtypes.h"
#include "utilitieslib/utils/file.h"
#include "utilitieslib/utils/SuperAssert.h"
#include "utilitieslib/utils/utils.h"
#include "checks.h"

/* CHECK reports through the CRT's stderr, not an engine FileWrapper. */
#undef fprintf

#if COX_EXPECT_OPTDEBUG
#if !defined(_OPTDEBUG) || !defined(NDEBUG) || defined(FINAL) || defined(_DEBUG) || defined(FULLDEBUG) || defined(_FULLDEBUG) || defined(_CRTDBG_MAP_ALLOC)
#error OptDebug must use optimized development definitions and the release CRT
#endif
#endif

#if COX_EXPECT_DEV_FEATURES && defined(DISABLE_ASSERTIONS)
#error Developer configurations must retain engine assertions
#endif

#ifndef FINAL
extern int g_force_production_mode;
extern int g_force_qa_mode;
#endif

int main(int argc, char **argv)
{
	CHECK(argc == 2);
	int loose = strcmp(argv[1], "loose") == 0;
	CHECK(loose || strcmp(argv[1], "packaged") == 0);
	CHECK(fileIsUsingDevData() == (COX_EXPECT_DEV_FEATURES && loose));
	CHECK(isDevelopmentMode() == (COX_EXPECT_DEV_FEATURES && loose));
	CHECK(isProductionMode() == !(COX_EXPECT_DEV_FEATURES && loose));

#ifndef FINAL
	g_force_production_mode = 1;
	CHECK(isDevelopmentMode() == 0 && isProductionMode() == 1);
	g_force_qa_mode = 1;
	CHECK(isDevelopmentOrQAMode() == 0);
	g_force_production_mode = 0;
	CHECK(isDevelopmentOrQAMode() == 1);
	g_force_qa_mode = 0;
	CHECK(isDevelopmentOrQAMode() == loose);
#endif

	/* A successful assertion proves evaluation without opening a dialog. */
	int evaluated = 0;
	assert(++evaluated);
	/* The engine preserves expression side effects even in FINAL builds. */
	CHECK(evaluated == 1);
	writeConsole(OUTPUT_DEBUG, "COX_DEVELOPMENT_LOG");
	puts("Development mode checks passed");
	return 0;
}
