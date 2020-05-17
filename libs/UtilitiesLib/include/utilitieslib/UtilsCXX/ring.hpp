#ifndef _RING_HPP
#define _RING_HPP

#include "../assert/assert.h"
#include "../UtilsNew/barrier.h"

#if _M_IX86
    #define _InterlockedCompareExchangePointer(_destination, _exchange, _comparand) (void*)_InterlockedCompareExchange((volatile long*)_destination, (long)_exchange, (long)_comparand)
#endif

/**
* Pointer Ring Buffer Base
*/
template<typename T>
class PointerRingBase {
public:
    T m_read;
    T m_write;

    void ** m_base;
    unsigned long m_size;

public:
    PointerRingBase() : m_base(NULL), m_read(0), m_write(0), m_size(0) {}

    void set_buffer(void ** buf, size_t size) {
        assert(!m_base);
        assert(!(size & (size-1)));

        m_read = 0;
        m_write = 0;

        m_base = buf;
        m_size = (unsigned long)size;
    }

    bool empty() {
        return m_base[m_read]==NULL;
    }

    bool full() {
        return m_base[m_write]!=NULL;
    }

    bool push(void * data) {
        assert(data);

        unsigned write = m_write;
        if(!m_base[write]) {
            _WriteBarrier();
            m_base[write] = data;
            m_write = (write + 1) & (m_size - 1); // wrap around with no divide
            return true;
        }

        return false;
    }

    void * pop() {
        unsigned read = m_read;
        void * ret = m_base[read];
        if (ret) {
            m_base[read] = NULL;
            m_read = (read + 1) & (m_size - 1); // wrap around with no divide
            _ReadBarrier();
            return ret;
        }
        return NULL;
    }
};

template<typename T>
class AtomicPointerRingBase : public PointerRingBase<T> {
    #define ATOMIC_HAZARD ~0UL
public:
    bool empty() {
        while (true) {
            unsigned long read = static_cast<T*>(this)->m_read;
            if (read != ATOMIC_HAZARD)
                return static_cast<T*>(this)->m_base[read]==NULL;

            _mm_pause();
        }
    }

    bool full() {
        while (true) {
            unsigned long write = static_cast<PointerRingBase<T>*>(this)->m_write;
            if (write != ATOMIC_HAZARD)
                return static_cast<PointerRingBase<T>*>(this)->m_base[write]!=NULL;

            _mm_pause();
        }
    }

    bool push(void * data) {
        assert(data);

        while (true) {
            unsigned long write = static_cast<PointerRingBase<T>*>(this)->m_write;
            if ((write != ATOMIC_HAZARD) && ((unsigned long)_InterlockedCompareExchange((volatile long*)& static_cast<PointerRingBase<T>*>(this)->m_write, ATOMIC_HAZARD, write) == write)) {
                if (!static_cast<PointerRingBase<T>*>(this)->m_base[write]) {
                    static_cast<PointerRingBase<T>*>(this)->m_base[write] = data;
                    _WriteBarrier();
                    static_cast<PointerRingBase<T>*>(this)->m_write = (write + 1) & (static_cast<PointerRingBase<T>*>(this)->m_size - 1); // wrap around with no divide
                    return true;
                }

                static_cast<PointerRingBase<T>*>(this)->m_write = write;

                if (write == static_cast<PointerRingBase<T>*>(this)->m_read)
                    break;
            }
            _mm_pause();
        }
        return false;
    }

    void * pop() {
        while (true) {
            unsigned long read = static_cast<PointerRingBase<T>*>(this)->m_read;
            if ((read != ATOMIC_HAZARD) && ((unsigned long)_InterlockedCompareExchange((volatile long*)& static_cast<PointerRingBase<T>*>(this)->m_read, ATOMIC_HAZARD, read) == read)) {
                if (void * ret = static_cast<PointerRingBase<T>*>(this)->m_base[read]) {
                    static_cast<PointerRingBase<T>*>(this)->m_base[read] = NULL;
                    _WriteBarrier();
                    static_cast<PointerRingBase<T>*>(this)->m_read = (read + 1) & (static_cast<PointerRingBase<T>*>(this)->m_size - 1); // wrap around with no divide
                    return ret;
                }

                static_cast<PointerRingBase<T>*>(this)->m_read = read;

                if (read == static_cast<PointerRingBase<T>*>(this)->m_write)
                    break;
            }
            _mm_pause();
        }
        return NULL;
    }
};

template<typename T>
class PointerRing : public PointerRingBase<T> {
public:
    PointerRing(size_t size = 0) {
        if (size)
            construct(size);
    }

    ~PointerRing() {
        destruct();
    }

    void construct(size_t size) {
        static_cast<PointerRingBase<T>*>(this)->set_buffer((void**)calloc(size, sizeof(void*)), size);
    }

    void destruct() {
        if (static_cast<T*>(this)->m_base) {
            free(static_cast<T*>(this)->m_base);
            static_cast<T*>(this)->m_base = NULL;
        }
    }
};

template<typename T>
class AtomicPointerRing : public AtomicPointerRingBase<T> {
public:
    AtomicPointerRing(size_t size = 0) {
        if (size)
            construct(size);
    }

    ~AtomicPointerRing() {
        destruct();
    }

    void construct(size_t size) {
        static_cast<PointerRingBase<T>*>(this)->set_buffer((void**)calloc(size, sizeof(void*)), size);
    }

    void destruct() {
        if (static_cast<PointerRingBase<T>*>(this)->m_base) {
            free(static_cast<PointerRingBase<T>*>(this)->m_base);
            static_cast<PointerRingBase<T>*>(this)->m_base = NULL;
        }
    }
};

/// Single threaded
typedef PointerRingBase<unsigned> UnthreadedPointerRingBase;
typedef PointerRing<unsigned> UnthreadedPointerRing;

/// One reader thread / One writer thread
typedef PointerRingBase<__declspec(align(EXPECTED_WORST_CACHE_LINE_SIZE)) unsigned> TwoThreadedPointerRingBase;
typedef PointerRing<__declspec(align(EXPECTED_WORST_CACHE_LINE_SIZE)) unsigned> TwoThreadedPointerRing;

/// Multiple readers / writers
typedef AtomicPointerRingBase<volatile unsigned long> MultiThreadedPointerRingBase;
typedef AtomicPointerRing<volatile unsigned long> MultiThreadedPointerRing;

#endif
