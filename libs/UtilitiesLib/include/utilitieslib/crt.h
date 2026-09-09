#ifndef COH_CRT_H
#define COH_CRT_H

/* MinGW links the redistributable CRT, which has no Microsoft debug heap.
 * Select its release CRT interface while retaining project Debug assertions,
 * memory pools, timed allocation wrappers and Windows heap validation.
 * Include this instead of crtdbg.h so header order cannot select debug DLL
 * imports or remap malloc recursively through the project allocator.
 */
#if defined(__MINGW32__) && defined(_DEBUG)
#pragma push_macro("_DEBUG")
#undef _DEBUG
#include <crtdbg.h>
#pragma pop_macro("_DEBUG")
#else
#include <crtdbg.h>
#endif

#endif
