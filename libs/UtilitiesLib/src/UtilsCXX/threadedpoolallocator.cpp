#include "utilitieslib/UtilsCXX/threadedpoolallocator.hpp"
#include "utilitieslib/UtilsNew/barrier.h"
#include "utilitieslib/components/MemoryPool.h"
#include "utilitieslib/UtilsCXX/criticalsection.hpp"
#include "utilitieslib/UtilsCXX/futex.hpp"

ThreadedPoolAllocator::ThreadedPoolAllocator(unsigned structSize, bool zero, unsigned initialPoolSize, unsigned threadFreeListSize) :
    m_zero(zero), m_threadFreeLists(NULL), m_threadFreeListSize(threadFreeListSize)
{
    assert(structSize);

    // TODO replace this with a lock free container
    m_globalFreeList.construct(initialPoolSize);

    m_lock = new CriticalSection;

    m_pool = createMemoryPool();
    initMemoryPool(m_pool, structSize, initialPoolSize);
    mpSetMode(m_pool, zero ? ZeroMemoryBit : TurnOffAllFeatures);

    m_threadLocalStorageIndex = TlsAlloc();
}

ThreadedPoolAllocator::~ThreadedPoolAllocator() {
    empty_all_free_lists();
    TlsFree(m_threadLocalStorageIndex);
    destroyMemoryPool(m_pool);
    delete m_lock;
}

ThreadedFreeList * ThreadedPoolAllocator::new_thread_free_list() {
    ThreadedFreeList * ret = (ThreadedFreeList*)calloc(1, sizeof(ThreadedFreeList));
    ret->construct(m_threadFreeListSize);
    ret->thread = GetCurrentThreadId();

    m_lock->lock();
    ret->next = m_threadFreeLists;
    _WriteBarrier();
    m_threadFreeLists = ret;
    m_lock->unlock();

    return ret;
}

ThreadedFreeList * ThreadedPoolAllocator::get_thread_free_list() {
    ThreadedFreeList * ret = (ThreadedFreeList*)TlsGetValue(m_threadLocalStorageIndex);
    if (ret)
        return ret;
    
    ret = new_thread_free_list();
    TlsSetValue(m_threadLocalStorageIndex, ret);
    return ret;
}

void ThreadedPoolAllocator::empty_global_free_list() {
    m_lock->lock();
    while (void * ptr = m_globalFreeList.pop()) {
        mpFree(m_pool, ptr);
    }
    m_lock->unlock();
}

void ThreadedPoolAllocator::empty_free_list(ThreadedFreeList * freeList) {
    while (void * ptr = freeList->pop()) {
        if (!m_globalFreeList.push(ptr)) {
            m_lock->lock();
            mpFree(m_pool, ptr);
            m_lock->unlock();
        }
    }
}

void ThreadedPoolAllocator::empty_thread_free_list() {
    empty_free_list(get_thread_free_list());
}

void ThreadedPoolAllocator::empty_all_free_lists() {
    for (ThreadedFreeList * freeList = m_threadFreeLists; freeList; freeList = freeList->next) {
        empty_free_list(freeList);
    }

    empty_global_free_list();
}

void * ThreadedPoolAllocator::talloc() {
    void * ret;

    // Check for an entry in the thread local pool
    if (ret = get_thread_free_list()->pop())
        return ret;

    // Return a new entry from the global pool or allocate one
    if (!(ret = m_globalFreeList.pop())) {
        m_lock->lock();
        ret = mpAlloc(m_pool);
        m_lock->unlock();
    }
    return ret;
}

void ThreadedPoolAllocator::tfreenozero(void * ptr) {
    ThreadedFreeList * thread_free_list = get_thread_free_list();

    // Try to put the entry in the thread local pool
    if (thread_free_list->push(ptr))
        return;

    // Empty the thread local pool
    empty_free_list(thread_free_list);

    // Put entry in the thread local pool
    assert(thread_free_list->push(ptr));
}

void ThreadedPoolAllocator::tfreezero(void * ptr) {
    ThreadedFreeList * thread_free_list = get_thread_free_list();

    // Try to put the entry in the thread local pool
    if (!thread_free_list->full()) {
        // Thread free pool pointers are pre-zeroed
        memset(ptr, 0, mpStructSize(m_pool));
        if (thread_free_list->push(ptr))
            return;
    }

    if (!m_globalFreeList.full()) {
        // Global free pool pointers need to be zeroed before insertion
        memset(ptr, 0, mpStructSize(m_pool));
        if (m_globalFreeList.push(ptr))
            return;
    }

    // Hand it back to the memory pool
    m_lock->lock();
    mpFree(m_pool, ptr);
    m_lock->unlock();
}

