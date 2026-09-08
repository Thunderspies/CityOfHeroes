#ifndef ASSERT_DUMP_H
#define ASSERT_DUMP_H

#include "SuperAssert.h"
#include "wininclude.h"
#include <dbghelp.h>

C_DECLARATIONS_BEGIN

#ifndef DISABLE_ASSERTIONS
void assertWriteFullDumpSimpleSetFlags(MINIDUMP_TYPE flags);
#else
#define assertWriteFullDumpSimpleSetFlags(flags)                                                                                                               \
    do                                                                                                                                                         \
    {                                                                                                                                                          \
    } while (0)
#endif

C_DECLARATIONS_END

#endif
