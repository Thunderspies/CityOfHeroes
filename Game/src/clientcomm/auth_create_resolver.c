#include "clientcomm/auth_create_account_private.h"
#include <stdlib.h>
#include <string.h>
#include <utilitieslib/network/sock.h>
#include <utilitieslib/utils/utils.h>
#include <ws2tcpip.h>

typedef struct AuthCreateResolverJob
{
    volatile LONG references;
    volatile LONG completed;
    U32 address;
    char host[1];
} AuthCreateResolverJob;

static AuthCreateResolverJob *resolverJob;

static void releaseResolverJob(AuthCreateResolverJob *job)
{
    if (InterlockedDecrement(&job->references) == 0)
        free(job);
}

static unsigned __stdcall resolveAuthCreateHost(void *context)
{
    AuthCreateResolverJob *job = context;
    struct addrinfo hints = {0};
    struct addrinfo *addresses = NULL;
    struct addrinfo *entry;

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    if (getaddrinfo(job->host, NULL, &hints, &addresses) == 0)
    {
        for (entry = addresses; entry; entry = entry->ai_next)
        {
            if (entry->ai_family == AF_INET && entry->ai_addrlen >= sizeof(struct sockaddr_in))
            {
                U32 address = ((struct sockaddr_in *)entry->ai_addr)->sin_addr.s_addr;
                if (address && address != INADDR_NONE)
                {
                    job->address = address;
                    break;
                }
            }
        }
        freeaddrinfo(addresses);
    }
    /* Publish the result before releasing the worker's reference. Cancellation
     * only releases the main thread's reference, so a blocked lookup stays safe.
     */
    InterlockedExchange(&job->completed, 1);
    releaseResolverJob(job);
    return 0;
}

int authCreateResolveBegin(const char *host)
{
    AuthCreateResolverJob *job;
    uintptr_t thread;
    size_t length;

    if (resolverJob || !host || !host[0])
        return 0;
    length = strlen(host);
    job = calloc(1, sizeof(*job) + length);
    if (!job)
        return 0;
    memcpy(job->host, host, length + 1);
    sockStart();
    job->address = inet_addr(host);
    if (job->address && job->address != INADDR_NONE)
    {
        job->references = 1;
        job->completed = 1;
        resolverJob = job;
        return 1;
    }
    job->address = 0;
    job->references = 2;
    thread = _beginthreadex(NULL, 0, resolveAuthCreateHost, job, 0, NULL);
    if (!thread)
    {
        free(job);
        return 0;
    }
    CloseHandle((HANDLE)thread);
    resolverJob = job;
    return 1;
}

int authCreateResolvePoll(U32 *address)
{
    if (!resolverJob || !address)
        return -1;
    if (!InterlockedCompareExchange(&resolverJob->completed, 0, 0))
        return 0;
    *address = resolverJob->address;
    authCreateResolveCancel();
    return *address ? 1 : -1;
}

void authCreateResolveCancel(void)
{
    if (resolverJob)
    {
        AuthCreateResolverJob *job = resolverJob;
        resolverJob = NULL;
        releaseResolverJob(job);
    }
}
