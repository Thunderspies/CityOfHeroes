#ifndef COH_COMPILER_H
#define COH_COMPILER_H

/* Compiler syntax shared by C and C++ headers.
 * FORCEINLINE does not specify linkage: use static for header-local functions.
 * THREAD_LOCAL objects have a distinct instance in each thread.
 * SECTION places an object in a named section; it does not share its storage.
 * NOP emits a breakpoint location without reading or writing program data.
 * ASSUME supplies MSVC's unevaluated optimizer hint. Other compilers omit it
 * rather than evaluating expressions that may have side effects.
 */
#ifdef _MSC_VER
#define COH_FORCEINLINE __forceinline
#define COH_THREAD_LOCAL __declspec(thread)
#define COH_SECTION(name) __declspec(allocate(name))
#define COH_NOP() __nop()
#define COH_ASSUME(expr) __assume(expr)
#else
#define COH_FORCEINLINE __inline__ __attribute__((always_inline))
#define COH_THREAD_LOCAL __thread
#define COH_SECTION(name) __attribute__((section(name)))
#define COH_NOP() __asm__ __volatile__("nop")
#define COH_ASSUME(expr) ((void)0)
#endif

#endif
