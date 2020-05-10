/************************************************************************
* SharedMemory module
* 
* This is designed so that multiple processes that all need access to the
* same static data can share it.  The procedure is for one process to
* acquire a lock on the memory and then fill it out, and any subsequent
* requests from other processes for the shared memory block will return
* a pointer into the data filled in by the first process.  Writing to
* this data after it is unlocked and shared between processes should be
* possible, but has not been thoroughly tested.
*
* The sharedMemoryChunk() functions operate on a given chunk, while the
* sharedMemory() functions go through the management layer that keeps
* track of where things are between different processes.
* 
/************************************************************************/
#include "utilitieslib/components/SharedMemory.h"
#include "utilitieslib/components/SharedHeap.h"
#include "utilitieslib/utils/wininclude.h"
#include "utilitieslib/assert/assert.h"
#include <stdio.h>
#include <conio.h>
#include "utilitieslib/utils/MemoryMonitor.h"
#include "utilitieslib/utils/strings_opt.h"
#include "utilitieslib/utils/sysutil.h"
#include "utilitieslib/utils/file.h"
#include "utilitieslib/utils/osdependent.h"
#include "utilitieslib/utils/error.h"
#include "utilitieslib/utils/log.h"

typedef enum SharedMemoryState {
    SMS_READY, // Has been filled in by another process or this process
    SMS_LOCKED, // Has been locked by this process and needs to be allocated
    SMS_ALLOCATED, // Has had it's size set and allocated, and the caller needs to fill it in
    SMS_ERROR, // General error flag
    SMS_MANUALLOCK, // Has been manually locked by a caller for read/write
} SharedMemoryState;

typedef struct SharedMemoryHandle {
    void *data;
    size_t size;
    size_t bytes_alloced;
    char name[SHARED_HANDLE_NAME_LEN];
    SharedMemoryState state;
    unsigned long page_access;
    void* hMutex;
    void* hMapFile;
} SharedMemoryHandle;

#pragma warning(disable:4028) // parameter differs from declaration

#if _MSC_VER >= 1400
    #pragma warning(disable:4133)
#endif


static int print_shared_memory_messages=0;

static void *shared_memory_base_address = ((void*)(intptr_t)0x80000000);

static SharedMemoryMode shared_memory_mode = SMM_UNINITED;
static SYSTEM_INFO sysinfo; // After inited, it stores the allocation granularity

void sharedMemorySetMode(SharedMemoryMode mode)
{
    shared_memory_mode = mode;
}

SharedMemoryMode sharedMemoryGetMode()
{
    return shared_memory_mode;
}

void sharedMemoryMutexAbandoned(const char * name)
{
    static bool once = true;
    LOG(LOG_ERROR, once ? LOG_LEVEL_IMPORTANT : LOG_LEVEL_VERBOSE, LOG_CONSOLE_ALWAYS, "A process crashed while holding the shared memory mutex \"%s\", incorrect behavior may occur", name);
    devassertmsg(!once, "A process crashed while holding the shared memory mutex \"%s\", incorrect behavior may occur", name);
    once = false;
}

void sharedMemorySetBaseAddress(void *base_address)
{
    shared_memory_base_address = base_address;
    sharedMemorySetMode(SMM_UNINITED);
}

// Makes sure the system supports shared memory, if not, disables it
void sharedMemoryInit(void) {
    MEMORY_BASIC_INFORMATION mbi;

    if (shared_memory_mode != SMM_UNINITED)
        return;

    if (!IsUsingWin7orLater()) {
        sharedMemorySetMode(SMM_DISABLED);
        return;
    }

    GetSystemInfo(&sysinfo);
    if (!(sysinfo.lpMinimumApplicationAddress <= shared_memory_base_address &&
        sysinfo.lpMaximumApplicationAddress > shared_memory_base_address))
    {
        printf("Program address space does not include our desired address!  Running in non-SharedMemory mode.\n");
        sharedMemorySetMode(SMM_DISABLED);
        return;
    }

    VirtualQuery((void*)shared_memory_base_address, &mbi, sizeof(mbi));

    if (mbi.State != MEM_FREE) {
        printf("Our desired address is not free in this process space!  Running in non-SharedMemory mode\n");
        sharedMemorySetMode(SMM_DISABLED);
        return;
    }

    sharedMemorySetMode(SMM_ENABLED);
}


SharedMemoryHandle *sharedMemoryCreate(void) {
    return tls_zero_malloc(sizeof(SharedMemoryHandle));
}


// General flow:
// First, lock the Mutex.
//  then, determine if mmapped file exists
//   If so, it's already been created, and we're good to go, release the Mutex
//   If not, we need to create it!  Create the mmapped file, return control to the caller
//      so they can fill it in, and then release the mutex when we're done
static SM_AcquireResult sharedMemoryChunkAcquire(SharedMemoryHandle **phandle, const char *name, void *desired_address)
{
    char buf[SHARED_HANDLE_NAME_LEN+16];
    DWORD dwWaitResult;
    void *lpMapAddress;
    SharedMemoryHandle *handle;

    sharedMemoryInit();

    assert(shared_memory_mode!=SMM_DISABLED);

    assert(phandle);
    if (*phandle == NULL) {
        *phandle = (SharedMemoryHandle*)sharedMemoryCreate();
    }

    handle = *phandle;

    assert(strlen(name) < SHARED_HANDLE_NAME_LEN);

    strcpy(handle->name, name);
    //sprintf(buf, "Mutex_%s", handle->name);
    STR_COMBINE_BEGIN(buf);
    STR_COMBINE_CAT("Global\\Mutex_");
    STR_COMBINE_CAT(handle->name);
    STR_COMBINE_END();
    
    handle->hMutex = CreateMutex( 
        NULL,  // no security attributes
        FALSE, // initially not owned
        buf);  // name of mutex
    if (handle->hMutex == NULL) {
        handle->hMutex=INVALID_HANDLE_VALUE;
        MessageBox(NULL, "Error creating mutex", "Error", MB_ICONINFORMATION);
        handle->state = SMS_ERROR;
        return SMAR_Error;
    }
    // Grab the mutex (release it when we want the app to end)
    dwWaitResult = WaitForSingleObject( 
        handle->hMutex,   // handle to mutex
        INFINITE);   // Wait for ever
    devassert(dwWaitResult == WAIT_OBJECT_0 || dwWaitResult == WAIT_ABANDONED);
    if (dwWaitResult == WAIT_ABANDONED)
        sharedMemoryMutexAbandoned(handle->name);

    // We now have the mutex because either
    //  a) it was abandoned by another thread, or
    //  b) the other thread finished creating the shared memory, or
    //  c) we initially created the mutex

    //sprintf(buf, "MemMapFile_%s", handle->name);
    STR_COMBINE_BEGIN(buf);
    STR_COMBINE_CAT("MemMapFile_");
    STR_COMBINE_CAT(handle->name);
    STR_COMBINE_END();
    // Now, let's check to see if the memory mapped file exists
    handle->hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, // read/write permission 
        FALSE,        // Do not inherit the name
        buf);        // of the mapping object. 

    if (handle->hMapFile == NULL) 
    { 
        // The file doesn't exist, we need to create it (but only after we know how much data there is)!
        handle->state = SMS_LOCKED;
        handle->data = desired_address; // Save the desired address

        // Tell the caller to fill this in
        return SMAR_FirstCaller;
    } 

    // We acquired the mutex lock, and the file is already there, we can just unlock it and use the data!
    handle->size = 0; // This doesn't work: GetFileSize(handle->hMapFile, NULL);

    lpMapAddress = MapViewOfFileExSafe(handle->hMapFile, buf, desired_address, 0);

    if (lpMapAddress == NULL) { 
        goto fail;
    }
    if (desired_address != NULL && lpMapAddress != desired_address) {
        goto fail;
    }

    handle->data = lpMapAddress;
    handle->state = SMS_READY;
    devassert(ReleaseMutex(handle->hMutex));

    return SMAR_DataAcquired;

fail:
    //MessageBox(NULL, "Error mapping view of file!", "Error", MB_ICONINFORMATION);
    devassert(ReleaseMutex(handle->hMutex));
    handle->state = SMS_ERROR;
    return SMAR_Error;

}




// Allocates the required amount of shared memory and returns a pointer to it.
//   This can only be called after sharedMemoryChunkAcquire and only if
//   sharedMemoryChunkAquire returned 1
void *sharedMemoryChunkSetSize(SharedMemoryHandle *handle, uintptr_t size)
{
    char buf[SHARED_HANDLE_NAME_LEN+16];
    void *lpMapAddress;
    assert(handle && handle->state == SMS_LOCKED && handle->hMapFile == NULL);

    //sprintf(buf, "MemMapFile_%s", handle->name);
    STR_COMBINE_BEGIN(buf);
    STR_COMBINE_CAT("MemMapFile_");
    STR_COMBINE_CAT(handle->name);
    STR_COMBINE_END();

    // Create the Mem mapped file
    handle->hMapFile = CreateFileMappingSafe(PAGE_READWRITE, size, buf, 0);

    if (handle->hMapFile == NULL) {
        goto fail;
    }

    lpMapAddress = MapViewOfFileExSafe(handle->hMapFile, buf, handle->data, 0);

    if (lpMapAddress == NULL) { 
        goto fail;
    }

    if (handle->data != NULL && lpMapAddress != handle->data) {
        goto fail;
    }

    handle->data = lpMapAddress;
    handle->size = size;
    handle->state = SMS_ALLOCATED;
    handle->page_access = PAGE_READONLY;
    handle->bytes_alloced = 0;

    return handle->data;

fail:
    //MessageBox(NULL, "Error creating/mapping view of file!", "Error", MB_ICONINFORMATION);
    handle->state = SMS_ERROR;
    devassert(ReleaseMutex(handle->hMutex));
    return NULL;

}

// Unlocks a block of shared memory, so that other processes can now access it
//   This can only be called after sharedMemoryChunkAcquire and only if
//   sharedMemoryChunkAquire returned 1, and after sharedMemoryChunkSetSize has
//   been called, or after sharedMemoryChunkLock has been called
void sharedMemoryChunkUnlock(SharedMemoryHandle *handle)
{
    assert(handle->state == SMS_ALLOCATED || handle->state == SMS_MANUALLOCK);

    devassert(ReleaseMutex(handle->hMutex));
    handle->state = SMS_READY;
}

void sharedMemoryChunkLock(SharedMemoryHandle *handle)
{
    DWORD dwWaitResult;

    assert(handle->state == SMS_READY);

    dwWaitResult = WaitForSingleObject( 
        handle->hMutex,   // handle to mutex
        INFINITE);   // Wait for ever
    devassert(dwWaitResult == WAIT_OBJECT_0 || dwWaitResult == WAIT_ABANDONED);
    if (dwWaitResult == WAIT_ABANDONED)
        sharedMemoryMutexAbandoned(handle->name);

    handle->state = SMS_MANUALLOCK;
}


// Frees all handles to the shared memory, but leaves it around for the OS to clean up
// or other processes to use
void sharedMemoryChunkDestroy(SharedMemoryHandle *handle)
{
#define SAFE_CLOSE(x)    \
    if (x) {            \
        CloseHandle(x);    \
        x = NULL;        \
    }
    SAFE_CLOSE(handle->hMapFile);
    SAFE_CLOSE(handle->hMutex);
    free(handle);
}


static uintptr_t roundUpToMemoryBlock(uintptr_t origsize) {
    return (((origsize + sysinfo.dwAllocationGranularity - 1) / 
        sysinfo.dwAllocationGranularity) * sysinfo.dwAllocationGranularity);
}


//////////////////////////////////////////////////////////////////////////
// Shared Memory Management (wrapper)

typedef struct SharedMemoryAllocList {
    struct SharedMemoryAllocList *next;
    uintptr_t size;
    void *base_address;
    int reference_count;
    char name[MAX_PATH]; // In memory, this is actually a variable length, null terminated string
} SharedMemoryAllocList;

typedef struct SharedMemoryAllocHeader {
    SharedMemoryAllocList *head;
    void *next_free_node; // The next place we can safely cast to an AllocList node
    void *next_free_block; // The next address in process space we can use to allocated shared memory
} SharedMemoryAllocHeader;

#ifdef _M_X64
static char manager_name[] = "SharedMemoryManager_x64";
#else
static char manager_name[] = "SharedMemoryManager";
#endif
static size_t manager_size = 128*4*1024;
static SharedMemoryHandle *pManager = NULL;
static int pManagerLockCount = 0;
static SharedMemoryAllocHeader *pHeader = NULL; // Should always be equal to pManager->data;
static SharedMemoryAllocList fake_shared_memory; // Up to one element might be in "fake" shared memory mode

// Adds a node to the alloc list (only call this *after* the node has been filled in, otherwise
// we can get wacky issues between processes thinking it's filled in when it's not)
static void addNodeToAllocList(void *base_address, size_t size, const char *name)
{
    size_t len;
    SharedMemoryAllocList *newnode;
    assert(pHeader);

    // Find the new node
    newnode = (SharedMemoryAllocList*)pHeader->next_free_node;
    
    // Update next_free_node
    len = (int)strlen(name) + 1;
    pHeader->next_free_node = &newnode->name[0] + len;

    if ((char*)pHeader->next_free_node < (char*)pHeader + manager_size) {

        // Fill in the node
        newnode->base_address = base_address;
        newnode->size = size;
        newnode->next = pHeader->head;
        newnode->reference_count = 1;
        strcpy_s(newnode->name, len, name);

        pHeader->head = newnode;

    } else {
        assert(!"Ran out of memory in the shared memory manager!");
        sharedMemorySetMode(SMM_DISABLED);
    }

    if (print_shared_memory_messages)
        printf("Added new node at 0x%Ix, named '%s', used %Id/%Id bytes in smm\n", base_address, name, (char*)pHeader->next_free_node - (char*)pHeader, manager_size);
    // Update next_free_block
    pHeader->next_free_block = (char*)base_address + size;

    // Report to MemMonitor
    // skip shared heap pages
    if (memMonitorActive() && strncmp(name, "sharedHeapAllocPage", 19)) {
        ModuleMemOperationStats* stats;
        stats = mmGetModuleOperationStats(&MemMonitor, (char*)name, 0);
        assert(stats);
        stats->bShared = true;
        if (stats->stats->count.value==0) {
            mmoUpdateStats(stats, MOT_ALLOC, 0, roundUpToMemoryBlock(size));
            memMonitorTrackSharedMemory(roundUpToMemoryBlock(size));
        }
    }
}

static SharedMemoryAllocList *findNodeInAllocList(const char *name)
{
    SharedMemoryAllocList *walk;
    assert(pHeader);

    for (walk = pHeader->head; walk; walk = walk->next) {
        if (strcmp(walk->name, name)==0) {
            return walk;
        }
    }
    return NULL;    
}

static void removeNodeFromAllocList(SharedMemoryAllocList *node)
{
    assert(pHeader);
    if (node == pHeader->head) {
        pHeader->head = node->next;
    } else {
        SharedMemoryAllocList *walk;

        for (walk = pHeader->head; walk; walk = walk->next) {
            if (walk->next == node) {
                walk->next = node->next;
                break;
            }
        }
    }
    node->next = NULL;
    strcpy_s(node->name, 6, "freed");
}

static void *findNextFreeBlock(void)
{
    assert(pHeader);

    pHeader->next_free_block = (void*)roundUpToMemoryBlock((uintptr_t)pHeader->next_free_block);
    
    return pHeader->next_free_block;
}

SM_AcquireResult sharedMemoryAcquire(SharedMemoryHandle **phandle, const char *name)
{
    SharedMemoryHandle *handle;
    SharedMemoryAllocList *plist;
    SM_AcquireResult ret;
    DWORD dwDummy;
#ifdef _M_X64
    char namebuf_x64[SHARED_HANDLE_NAME_LEN];
    sprintf_s(SAFESTR(namebuf_x64), "%s_x64", name);
    name = namebuf_x64;
#endif

    sharedMemoryInit();

    if (shared_memory_mode == SMM_DISABLED) {
        return SMAR_Error;
    }

    assert(!sharedHeapLockHeld());

    assert(phandle);
    if (*phandle == NULL) {
        *phandle = (SharedMemoryHandle*)sharedMemoryCreate();
    }

    handle = *phandle;

    // First acquire (and init) the memory manager
    if (pManager==NULL) {
        assert(!sharedHeapLockHeld());
        ret = sharedMemoryChunkAcquire(&pManager, manager_name, shared_memory_base_address);
        if (ret == SMAR_Error) {
            printf("Error acquiring shared memory for the memory manager!\n");
            sharedMemorySetMode(SMM_DISABLED);
            return SMAR_Error;
        }
    } else {
        ret = SMAR_DataAcquired; // Pretend we just got a view of it (since we already have one!)
    }
    if (ret == SMAR_FirstCaller) {
        // Needs to be filled in!
        char *data = sharedMemoryChunkSetSize(pManager, manager_size);

        if (data == NULL) {
            sharedMemorySetMode(SMM_DISABLED);
            return SMAR_Error;
        }

        // Fill it in (with 1 structure describing this entry)
        pHeader = (SharedMemoryAllocHeader*)data;
        pHeader->head = NULL;
        pHeader->next_free_node = data + sizeof(SharedMemoryAllocHeader);
        pHeader->next_free_block = data + manager_size; // Needs to be rounded up to granularity

        addNodeToAllocList(data, manager_size, manager_name);

        // We have now created the list, but we want to leave it locked until
        //  the caller of this function fills in his data and we can save it
        pManagerLockCount = 1;
    } else {
        // The memory is already there, just lock it!
        if (!pManagerLockCount++)
            sharedMemoryChunkLock(pManager);
        pHeader = (SharedMemoryAllocHeader*)pManager->data;
    }

    assert(pManagerLockCount > 0);

    // We now have the memory manager locked, let's get some shared memory at the appropriate
    //   address and return it to the caller, or create it if it doesn't exist
    if (plist = findNodeInAllocList(name)) {
        // Already in the list, great!
        // Acquire a view of this shared memory
        ret = sharedMemoryChunkAcquire(&handle, name, plist->base_address);
        if (ret==SMAR_Error) {
            // could not acquire it
            //assert(!"Could not acquire view of memory, perhaps it's being mapped twice?");
            if (!--pManagerLockCount)
                sharedMemoryChunkUnlock(pManager);
            // Disable shared memory from here on out, because we don't want to grab a chunk of shared memory
            // that has pointers into this chunk that was never acquired.
            sharedMemorySetMode(SMM_DISABLED);
            return ret; // SMAR_Error
        } else if (ret==SMAR_DataAcquired) {
            plist->reference_count++;
            // Acquired it, and it's already filled in, excellent!
            // Verify size value if its there, and set it if it's not
            assert(handle->size==0 || handle->size==plist->size);
            if (handle->size==0)
                handle->size = plist->size;

            // Make the memory read-only
            VirtualProtect(handle->data, handle->size, handle->page_access, &dwDummy);

            // Just unlock things and send it back to the client
            if (!--pManagerLockCount)
                sharedMemoryChunkUnlock(pManager);
            // Report to MemMonitor
            // skip shared heap pages
            if (memMonitorActive() && strncmp(name, "sharedHeapAllocPage", 19)) {
                ModuleMemOperationStats* stats;
                stats = mmGetModuleOperationStats(&MemMonitor, (char*)name, 0);
                assert(stats);
                stats->bShared = true;
                if (stats->stats->count.value==0) {
                    mmoUpdateStats(stats, MOT_ALLOC, 0, roundUpToMemoryBlock(handle->size));
                    memMonitorTrackSharedMemory(roundUpToMemoryBlock(handle->size));
                }
            }
            return ret; // SMAR_DataAcquired
        } else {
            plist->reference_count = 1;
            // The entry is in the allocation list, but it's not in memory anymore, all
            // processes referencing it must have exited... this new process can put it
            // wherever it wants
            assert(ret==SMAR_FirstCaller);
            // The client needs to fill stuff in, let's leave it all locked and return!
            return ret; // SMAR_Firstcaller
        }
    } else {
        // The memory is not currently in the list, that means we will need to 
        //  add it to the list after it's filled in!
        void *base_address = findNextFreeBlock();

        // Acquire a view of this shared memory
        ret = sharedMemoryChunkAcquire(&handle, name, base_address);
        if (ret==SMAR_Error) {
            // could not acquire it
            // This sometimes happens with "Attempt to access an invalid address." as the return
            //   perhaps some DLL grabs the memory from under us...
            //assert(!"Could not acquire view of memory, perhaps it's being mapped twice?");
            if (!--pManagerLockCount)
                sharedMemoryChunkUnlock(pManager);
            return ret;
        } else if (ret==SMAR_DataAcquired) {
            char name2[SHARED_HANDLE_NAME_LEN];
            // Acquired it, and it's already filled in, but the
            //  manager doesn't know about it, so it is either something
            //  we removed from the list, or shared memory from a different
            //  app.  Let's generate a new name, and use that instead...
            if (!--pManagerLockCount)
                sharedMemoryChunkUnlock(pManager);
            sprintf_s(SAFESTR(name2), "%s_1", name);
            return sharedMemoryAcquire((SharedMemoryHandle**)phandle, name2);
        } else {
            assert(ret==SMAR_FirstCaller);
            // The entry is not in memory and not in the list (expected)
            // The client needs to fill stuff in, let's leave it all locked and return!
            return ret;
        }
    }
    assert(0);
}

void *sharedMemorySetSize(SharedMemoryHandle *handle, uintptr_t size)
{
    return sharedMemorySetSizeEx(handle, size, 1);
}
void *sharedMemorySetSizeEx(SharedMemoryHandle *handle, uintptr_t size, int allowFakeSharing)
{
    void *ret;
    SharedMemoryAllocList *plist;

    assert(shared_memory_mode == SMM_ENABLED);
    assert(pManagerLockCount > 0);

    /* old code - could have SMM_DISABLED always pretend they're the first caller
    if (shared_memory_mode == SMM_DISABLED) {
        handle->size = size;
        handle->data = malloc(size);
        handle->bytes_alloced = 0;
        return handle->data;
    }*/
    // Make sure that this new size isn't larger than the old size!
    if (plist = findNodeInAllocList(handle->name)) {
        assert(plist->base_address == handle->data);
        if (size > roundUpToMemoryBlock(plist->size)) {
            // We need a new address, this is too big for the old address!
            handle->data = findNextFreeBlock();
            removeNodeFromAllocList(plist);
        } else {
            // Just use the new size
            plist->size = size;
        }
    }

    // Just pass through
    ret = sharedMemoryChunkSetSize(handle, size);
    if (ret == NULL) { // unlock on error

        // fpe - dont allow fake sharing, just abort here
        FatalErrorf("Failed to allocate shared memory, size %d.  Try running with -nosharedmemory.", size);

        sharedMemoryChunkUnlock(pManager);
        if(allowFakeSharing) 
        {
            // Just allocate memory for it, pretend it's shared
            handle->data = calloc(size, 1);
            handle->size = size;
            sharedMemorySetMode(SMM_DISABLED);
            //assert(!fake_shared_memory.base_address);
            fake_shared_memory.base_address = handle->data;
            fake_shared_memory.size = size;
            fake_shared_memory.reference_count = 1;
            strcpy(fake_shared_memory.name, "FakeSharedMemory");
        }
    } else {
        // Succeeded, but...
        // Do this so isSharedMemory tests while loading behave right
        fake_shared_memory.base_address = handle->data;
        fake_shared_memory.size = handle->size;
    }
    return ret;
}


size_t sharedMemoryGetSize(SharedMemoryHandle *handle)
{
    if ( handle )
        return handle->size;
    return 0;
}

void sharedMemoryCommit(SharedMemoryHandle *handle)
{
    if (shared_memory_mode == SMM_DISABLED)
        return;

    if (handle->state == SMS_ERROR) {
        // If the handle is in an erroneous state, just ignore the request
        return;
    } else {
        SharedMemoryAllocList *plist;
        assert(handle->state == SMS_ALLOCATED);
        assert(pManagerLockCount > 0);

        // The client just finished filling in the data, it is now safe to add it to the
        // allocation list!
        if (plist = findNodeInAllocList(handle->name)) {
            if (plist->base_address != handle->data ||
                plist->size != handle->size ||
                strcmp(plist->name, handle->name) != 0)
            {
                // Remove old entry from the list, and create a new one!
                removeNodeFromAllocList(plist);
                addNodeToAllocList(handle->data, handle->size, handle->name);
            }
        } else {
            // It's a new one, add it to the list!
            addNodeToAllocList(handle->data, handle->size, handle->name);
            // Then unlock everything
        }
    }
}

void sharedMemoryUnlock(SharedMemoryHandle *handle)
{
    DWORD dwDummy;
    if (shared_memory_mode == SMM_DISABLED)
        return;

    if (handle->state == SMS_MANUALLOCK) {
        // Just a normal unlock, pass it through and let it go
        sharedMemoryChunkUnlock(handle);
        // Make the memory writeable
        VirtualProtect(handle->data, handle->size, handle->page_access, &dwDummy);
        return;
    } else {
        sharedMemoryCommit(handle);
        sharedMemoryChunkUnlock(handle);

        // Make the memory read-only
        VirtualProtect(handle->data, handle->size, handle->page_access, &dwDummy);

        if (!--pManagerLockCount)
            sharedMemoryChunkUnlock(pManager);
    }
}

void sharedMemoryLock(SharedMemoryHandle *handle)
{
    DWORD dwDummy;
    assert(shared_memory_mode == SMM_ENABLED);
    if (shared_memory_mode == SMM_DISABLED)
        return;

    // Just pass through
    sharedMemoryChunkLock(handle);
    // Make the memory writeable
    VirtualProtect(handle->data, handle->size, PAGE_READWRITE, &dwDummy);
}

void sharedMemoryUnprotect(SharedMemoryHandle *handle, unsigned long page_access)
{
    DWORD dwDummy;
    assert(shared_memory_mode == SMM_ENABLED);
    if (shared_memory_mode == SMM_DISABLED)
        return;
    handle->page_access = page_access;
    VirtualProtect(handle->data, handle->size, page_access, &dwDummy);
}

void sharedMemoryDestroy(SharedMemoryHandle *handle)
{
    if (shared_memory_mode == SMM_DISABLED) {
        if (handle && handle->data)
            free(handle->data);
        if (handle)
            free(handle);
        return;
    }
    if (handle->size) {
        // skip shared heap pages
        if (memMonitorActive() && strncmp(handle->name, "sharedHeapAllocPage", 19)) {
            ModuleMemOperationStats* stats;
            stats = mmGetModuleOperationStats(&MemMonitor, (char*)handle->name, 0);
            assert(stats);
            stats->bShared = true;
            if (stats->stats->count.value!=0) {
                mmoUpdateStats(stats, MOT_FREE, 0, roundUpToMemoryBlock(handle->size));
                memMonitorTrackSharedMemory(-(int)roundUpToMemoryBlock(handle->size));
            }
        }
    }

    // Just pass through
    sharedMemoryChunkDestroy(handle);
}

void *sharedMemoryAlloc(SharedMemoryHandle *handle, size_t size)
{
    void *ret;
    //assert(shared_memory_mode == SMM_ENABLED);
    //if (shared_memory_mode == SMM_DISABLED) {
    //    return malloc(size);
    //}
    if (handle->bytes_alloced + size > handle->size) {
        assert(!"Trying to alloc more memory from a shared memory block than was originally inited!");
        return NULL;
    }
    ret = (char*)handle->data + handle->bytes_alloced;
    handle->bytes_alloced += size;
    return ret;
}


void printSharedMemoryUsage(void)
{
    SharedMemoryAllocList *walk;
    long long total_size=0;
    long long actual_size=0;
    SharedMemoryHandle *handle=NULL;
    if (SMAR_FirstCaller==sharedMemoryAcquire(&handle, "Dummy")) {
        sharedMemorySetSize(handle, 1);
        sharedMemoryUnlock(handle);
    }
    if (!pHeader) {
        printf("Shared memory not accessible by this process or an error has occured\n");
        return;
    }
    walk = pHeader->head;
    printf("Shared memory usage:\n");
    while (walk) {
        printf("  0x%Ix:%8Id bytes, %2d refs, \"%s\"\n", walk->base_address, walk->size, walk->reference_count, walk->name);
        total_size += walk->size;
        actual_size += roundUpToMemoryBlock(walk->size);
        walk = walk->next;
    }
    printf("Total: %I64d bytes, %I64d actual\n", total_size, actual_size);
}

// Tests to see if a pointer is in a range that is from shared memory
bool isSharedMemory(const void *ptr)
{
    SharedMemoryAllocList *walk;

    if (fake_shared_memory.base_address) {
        if (ptr >= fake_shared_memory.base_address && ((char*)ptr <= (char*)fake_shared_memory.base_address + fake_shared_memory.size))
            return true;
    }

    if (!pHeader) {
        // No shared memory!
        return false;
    }
    if (ptr < (void*)pHeader) {
        // It's earlier than the base address (this is dependent on the current implementation)
        return false;
    }
    walk = pHeader->head;
    while (walk) {
        if (ptr >= walk->base_address && ((char*)ptr <= (char*)walk->base_address + walk->size))
            return true;
        walk = walk->next;
    }
    return false;
}

// Takes a pointer, and it it's part of shared memory removes the shared memory block from
//  the shared list so other instances will not be able to access it
void sharedMemoryUnshare(void *ptr)
{
    SharedMemoryAllocList *walk;
    if (!pHeader) {
        // No shared memory!
        return;
    }
    if (ptr < (void*)pHeader) {
        // It's earlier than the base address (this is dependent on the current implementation)
        return;
    }
    walk = pHeader->head;
    while (walk) {
        if (ptr >= walk->base_address && ((char*)ptr <= (char*)walk->base_address + walk->size)) {
            // found it!
            removeNodeFromAllocList(walk);
            return;
        }
        walk = walk->next;
    }
}
    




//////////////////////////////////////////////////////////////////////////
// Test function


void testSharedMemory(void)
{
    SharedMemoryHandle *handle=NULL;
    int ret;
    char *data=NULL;
    char *c;
    char c2;
#define TEST_SIZE 1024*1024*50+1 // 50MB and a byte
#define TEST_STRING "This is our test string!"

    printf("Acquiring memory...\n");
    ret = sharedMemoryAcquire(&handle, "SMTEST");
    assert(ret!=-1);
    if (ret == 1) { // We need to fill in the data!
        printf("We got it first, fill it in...");
        data = sharedMemorySetSize(handle, TEST_SIZE);
        assert(data);
        strcpy_s(data, TEST_SIZE, TEST_STRING);
        for (c=data + strlen(TEST_STRING) + 1; c < data +TEST_SIZE; c++) {
            *c = (char)c;
        }
        printf("press any key to unlock.\n");
        _getch();
        sharedMemoryUnlock(handle);
    } else {
        printf("Somone else created it, let's just use it.\n");
        _getch();
        data = handle->data;
        for (c=data; c < data +TEST_SIZE; c++) {
            c2 = *c;
        }
    }
    printf("Shared memory is at 0x%Ix and contains \"%s\"\n", data, data);
    assert(strcmp(data, TEST_STRING)==0);
    printf("Press any key to destroy.\n");
    _getch();
    sharedMemoryDestroy(handle);

}

void *sharedMemoryGetDataPtr(SharedMemoryHandle *handle)
{
    return handle->data;
}
