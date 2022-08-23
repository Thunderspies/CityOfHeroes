/***************************************************************************
 *     Copyright (c) 2008-2008, NCSoft
 *     All Rights Reserved
 *     Confidential Property of NCSoft
 ***************************************************************************/
#pragma once
#include "ncstd.h"
NCHEADER_BEGIN

// Interface ///////////////////////////////////////////////////////////////////
// LazyLocks are safe to use from any thread and don't require initialization

typedef struct LazyLock LazyLock;

NCINLINE void lazyLock(LazyLock *lock);        // waits indefinitely for the lock
NCINLINE int lazyLockTry(LazyLock *lock);    // doesn't wait, returns success
NCINLINE void lazyUnlock(LazyLock *lock);    // releases the lock

////////////////////////////////////////////////////////////////////////////////

#include <winbase.h>

struct LazyLock
{
    CRITICAL_SECTION cs;
    LONG initializing;

    S32 initialized;
};

void lazyLockInit(volatile LONG *initializing, volatile S32 *initialized, CRITICAL_SECTION *cs);

NCINLINE void lazyLock(LazyLock *lock)
{
    if(!lock->initialized)
        lazyLockInit(&lock->initializing, &lock->initialized, &lock->cs);
    EnterCriticalSection(&lock->cs);
}

NCINLINE int lazyLockTry(LazyLock *lock)
{
    if(!lock->initialized)
        lazyLockInit(&lock->initializing, &lock->initialized, &lock->cs);
    return !!TryEnterCriticalSection(&lock->cs);
}

NCINLINE void lazyUnlock(LazyLock *lock)
{
    LeaveCriticalSection(&lock->cs);
}

NCHEADER_END
