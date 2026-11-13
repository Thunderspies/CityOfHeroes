#ifndef AUTH_CREATE_ACCOUNT_PRIVATE_H
#define AUTH_CREATE_ACCOUNT_PRIVATE_H

#include <utilitieslib/stdtypes.h>

/* Private ownership check used to prevent normal login from stealing the
 * socket. */
int authCreateAccountIsPending(void);

/* Private, main-thread-only ownership of a worker with an independent host
 * copy. The worker never touches credentials, sockets, or shared authentication
 * state. */
int authCreateResolveBegin(const char *host);
/* Return 1 and the network-order IPv4 address, 0 while pending, or -1 on error.
 */
int authCreateResolvePoll(U32 *address);
/* Release the caller's reference without waiting for a resolver in the OS. */
void authCreateResolveCancel(void);

#endif
