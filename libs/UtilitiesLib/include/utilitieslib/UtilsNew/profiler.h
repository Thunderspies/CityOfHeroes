#ifndef PROFILER_H
#define PROFILER_H

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

#if defined(__GNUC__)
// The patchable profiler requires compiler-specific MSVC instrumentation.
#undef ENABLE_PROFILER
#elif !defined(FINAL)
#define ENABLE_PROFILER
#endif

#ifdef ENABLE_PROFILER
void BeginProfile(size_t memory);
void EndProfile(const char * filename);
#endif

#ifdef __cplusplus
}
#endif

#endif
