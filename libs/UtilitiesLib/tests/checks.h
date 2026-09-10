#ifndef UTILITIES_TEST_CHECKS_H
#define UTILITIES_TEST_CHECKS_H

/* Evaluate expr once, report failures, and return 1 from the current check.
 * Unlike assert, checks remain active in Release builds.
 */
#define CHECK(expr) do { \
	if (!(expr)) { \
		fprintf(stderr, "%s:%d: %s\n", __FILE__, __LINE__, #expr); \
		return 1; \
	} \
} while (0)

#ifdef __cplusplus
extern "C" {
#endif
/* Check C++ allocation and typed containers; return 0 on success. */
int check_cpp_allocation(void);
/* Check ncHash key ownership and container behavior; return 0 on success. */
int check_nchash(void);
#ifdef __cplusplus
}
#endif

#endif
