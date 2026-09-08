#ifndef _FUTEX_HPP
#define _FUTEX_HPP

#include "../UtilsNew/barrier.h"
#include "../UtilsNew/ntapiext.h"

static inline unsigned char futexBitTestAndSet(volatile LONG *value, LONG bit)
{
#if defined(__MINGW32__)
    return InterlockedBitTestAndSet(value, bit);
#else
    return _interlockedbittestandset(value, bit);
#endif
}

/* Allows up to 2^23-1 waiters */
#define FUTEX_WAKE 256
#define FUTEX_WAIT 512

__declspec(align(EXPECTED_WORST_CACHE_LINE_SIZE)) class Futex {
    union {
        unsigned long m_waiters;
        unsigned char m_owned;
    };

public:
    Futex() {
        init_ntapiext();

        m_waiters = 0;
        m_owned = 0;
    }

    bool trylock() {
        if (!m_owned && !futexBitTestAndSet((volatile LONG*)&m_waiters, 0))
            return true;
        return false;
    }

    void lock() {
        /* Try to take lock if not owned */
        while (m_owned || futexBitTestAndSet((volatile LONG*)&m_waiters, 0))
        {
            LONG waiters = m_waiters | 1;
            
            /* Otherwise, say we are sleeping */
            if (_InterlockedCompareExchange((volatile long *)&m_waiters, waiters + FUTEX_WAIT, waiters) == waiters)
            {
                /* Sleep */
                NtWaitForKeyedEvent(g_keyed_event_handle, &m_waiters, 0, NULL);
                
                /* No longer in the process of waking up */
                _InterlockedExchangeAdd((volatile long *)&m_waiters, -FUTEX_WAKE);
            }
        }
    }

    void unlock() {
        /* Atomically unlock without a bus-locked instruction */
        _ReadWriteBarrier();
        m_owned = 0;
        _ReadWriteBarrier();
        
        /* While we need to wake someone up */
        while (1)
        {
            LONG waiters = m_waiters;
            
            if (waiters < FUTEX_WAIT)
                return;
            
            /* Has someone else taken the lock? */
            if (waiters & 1)
                return;
            
            /* Someone else is waking up */
            if (waiters & FUTEX_WAKE)
                return;
            
            /* Try to decrease wake count */
            if (_InterlockedCompareExchange((volatile long *)&m_waiters, waiters - FUTEX_WAIT + FUTEX_WAKE, waiters) == waiters)
            {
                /* Wake one up */
                NtReleaseKeyedEvent(g_keyed_event_handle, &m_waiters, 0, NULL);
                return;
            }
            
            YieldProcessor();
        }
    }
};

#endif

