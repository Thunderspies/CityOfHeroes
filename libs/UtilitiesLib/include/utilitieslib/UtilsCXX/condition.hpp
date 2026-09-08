#ifndef _CONDITION_HPP
#define _CONDITION_HPP

#include "criticalsection.hpp"

#if _WIN32_WINNT < 0x0600
#error "Condition Variables require Vista or newer"
#endif

class Condition {
    CONDITION_VARIABLE m_cond;

public:
    Condition() {
        InitializeConditionVariable(&m_cond);
    }

    void wait(CriticalSection * lock) {
        SleepConditionVariableCS(&m_cond, &lock->m_sect, INFINITE);
    }

    void signal() {
        WakeConditionVariable(&m_cond);
    }

    void broadcast() {
        WakeAllConditionVariable(&m_cond);
    }
};

#endif
