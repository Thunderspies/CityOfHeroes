#include "utilitieslib/utils/SuperAssert.h"
#include "utilitieslib/utils/threadedFileCopy.h"
#include <sys/stat.h>
#include "utilitieslib/components/MemoryPool.h"
#include "utilitieslib/utils/wininclude.h"
#include "utilitieslib/utils/utils.h"
#include "utilitieslib/utils/mathutil.h"
#include "utilitieslib/components/earray.h"
#include "utilitieslib/utils/file.h"
#include "utilitieslib/utils/timing.h"

#define MAX_THREADS        16
#define NON_THREAD_MAX_QUEUE_SIZE    (256*1024) // Max total files that can be queued up
#define MAX_QUEUES_PER_THREAD 5 // Max to be allowed queued up per thread
#define THREAD_MAX_QUEUE_SIZE 64 // Max files that can be queued per thread, and max results queued per thread

typedef struct BufferedCriticalSection {
    int buffer0[4];
    CRITICAL_SECTION critSect;
    int buffer1[4];
} BufferedCriticalSection;

static int tfc_max_threads=1;
static int tfc_num_threads=0;
static BufferedCriticalSection critSectFileWrite = {
    {0x12345678,0x12345678,0x12345678,0x12345678},{0},{0x12345678,0x12345678,0x12345678,0x12345678}
};
static volatile int critSectFileWrite_init=false;
static BufferedCriticalSection critSectMallocTrack = {
    {0x12345678,0x12345678,0x12345678,0x12345678},{0},{0x12345678,0x12345678,0x12345678,0x12345678}
};

typedef enum TFCType {
    TFC_COPY, // request to copy
    TFC_PROGRESS, // response of progress
    TFC_COMPLETION, // response of completion
} TFCType;

typedef struct TFCRequestQueueEntry {
    TFCType type;
    TFCRequest *request;

    // For TFC_PROGRESS:
    int bytes; // bytes copied
    int total; // total file size
} TFCRequestQueueEntry;

typedef struct TFCRequestQueue {
    TFCRequestQueueEntry *queue;
    int size;
    int start, end;
} TFCRequestQueue;

TFCRequestQueue **queue_requests; // EArray
TFCRequestQueue **queue_results; // EArray
TFCRequestQueue *queue_notYetQueued;
DWORD threads[MAX_THREADS];


MP_DEFINE(TFCRequest);
TFCRequest *createTFCRequest(void)
{
    MP_CREATE(TFCRequest, 8);
    return MP_ALLOC(TFCRequest);
}

TFCRequest *createTFCRequestEx(char *src, char *dst, TFCRequestFlags flags, ProgressCallback progressCallback, FinishCallback finishCallback, void *userData)
{
    TFCRequest *ret;
    MP_CREATE(TFCRequest, 8);
    ret = MP_ALLOC(TFCRequest);
    Strncpyt(ret->src, src);
    Strncpyt(ret->dst, dst);
    ret->progressCallback = progressCallback;
    ret->finishCallback = finishCallback;
    ret->userData = userData;
    ret->flags = flags;
    return ret;
}

void destroyTFCRequest(TFCRequest *request)
{
    MP_FREE(TFCRequest, request);
}

static void tfcInitCriticalSections(void)
{
    if (!critSectFileWrite_init) {
        // Lazy init critical sections, but safely
        static volatile int num_people_initing=0;
        int result = InterlockedIncrement(&num_people_initing);
        if (result != 1) {
            while (!critSectFileWrite_init)
                Sleep(1);
        } else {
            // First person trying to init this
            InitializeCriticalSection(&critSectFileWrite.critSect);
            InitializeCriticalSection(&critSectMallocTrack.critSect);
            critSectFileWrite_init = true;
        }
    }
}

static U64 total_allocated = 0;
#define MAX_ALLOWED_MEM_USAGE (450*1024*1024)
static void tfcRecordMalloc(U64 size)
{
    while (total_allocated > MAX_ALLOWED_MEM_USAGE ||
        total_allocated > MAX_ALLOWED_MEM_USAGE/2 && (total_allocated + size) > MAX_ALLOWED_MEM_USAGE)
    {
        Sleep(100);
    }
    EnterCriticalSection(&critSectMallocTrack.critSect);
    total_allocated += size;
    //printf("TFC: +%10I64d = %10I64d              \n", size, total_allocated);
    LeaveCriticalSection(&critSectMallocTrack.critSect);
}

void tfcRecordMemoryUsed(U64 size)
{
    tfcInitCriticalSections();
    EnterCriticalSection(&critSectMallocTrack.critSect);
    total_allocated += size;
    //printf("TFCExternal: +%10I64d = %10I64d              \n", size, total_allocated);
    LeaveCriticalSection(&critSectMallocTrack.critSect);
}

static void tfcRecordFree(U64 size)
{
    EnterCriticalSection(&critSectMallocTrack.critSect);
    total_allocated -= size;
    //printf("TFC: -%10I64d = %10I64d              \n", size, total_allocated);
    LeaveCriticalSection(&critSectMallocTrack.critSect);
}

void tfcRecordMemoryFreed(U64 size)
{
    tfcInitCriticalSections();
    //printf("TFCExternal: -%10I64d = %10I64d              \n", size, total_allocated);
    tfcRecordFree(size);
}

static void queueInit(TFCRequestQueue *queue, int size)
{
    queue->size = size;
    queue->queue = calloc(size, sizeof(*queue->queue));
}

void threadedFileCopyInit(int maxThreads)
{
    tfc_max_threads = MIN(MAX_THREADS, maxThreads);
    tfcInitCriticalSections();

    if (queue_notYetQueued) {
        free(queue_notYetQueued->queue);
        free(queue_notYetQueued);
    }
    queue_notYetQueued = calloc(sizeof(*queue_notYetQueued), 1);
    queueInit(queue_notYetQueued, NON_THREAD_MAX_QUEUE_SIZE);
}

void enterFileWriteCriticalSection(void)
{
    if (!critSectFileWrite_init) {
        critSectFileWrite_init = true;
        InitializeCriticalSection(&critSectFileWrite.critSect);
    }
    EnterCriticalSection(&critSectFileWrite.critSect);
}

void leaveFileWriteCriticalSection(void)
{
    LeaveCriticalSection(&critSectFileWrite.critSect);
}


static CRITICAL_SECTION tfc_bytes_transferred_critsect;
static bool tfc_bytes_transferred_critsect_inited=false;
static volatile U64 tfc_bytes_transferred;
static U64 tfc_last_bytes_transferred;

U32 threadedFileCopyBPS(void)
{
    static int timer = -1;
    static U32 cached=0;
    F32 elapsed;
    if (timer==-1)
        timer = timerAlloc();
    if (!tfc_bytes_transferred_critsect_inited) {
        InitializeCriticalSection(&tfc_bytes_transferred_critsect);
        tfc_bytes_transferred_critsect_inited = true;
    }
    if ((elapsed = timerElapsed(timer)) > 3.f) {
        U64 value = tfc_bytes_transferred;
        U64 delta = value - tfc_last_bytes_transferred;
        tfc_last_bytes_transferred = value;
        timerStart(timer);
        cached = (U32)((F32)delta / elapsed);
    }
    return cached;
}

static void recordBytesCopied(U32 bytes)
{
    EnterCriticalSection(&tfc_bytes_transferred_critsect);
    tfc_bytes_transferred+=bytes;
    LeaveCriticalSection(&tfc_bytes_transferred_critsect);
}

static void queueRequest(TFCRequestQueue *queue, TFCRequestQueueEntry *request)
{
    while (queue->end == (queue->start + queue->size - 1)%queue->size) {
//        printf("Queue full, stalling...\n");
        Sleep(1);
    }
    queue->queue[queue->end] = *request;
    queue->end = (queue->end + 1) % queue->size;
}

static TFCRequestQueueEntry *peekRequest(TFCRequestQueue *queue)
{
    TFCRequestQueueEntry *ret;
    if (queue->start == queue->end)
        return NULL;
    ret = &queue->queue[queue->start];
    return ret;
}

// Return is guaranteeded good until the next dequeueRequest call on the same queue
static TFCRequestQueueEntry *dequeueRequest(TFCRequestQueue *queue)
{
    TFCRequestQueueEntry *ret = peekRequest(queue);
    if (ret)
        queue->start = (queue->start + 1) % queue->size;
    return ret;
}

static int queueSize(TFCRequestQueue *queue)
{
    return (queue->end - queue->start + queue->size) % queue->size;
}

static int queueSizeSafe(TFCRequestQueue **queues, int queueNum)
{
    if (queueNum >= eaSize(&queues))
        return 0;
    if (queues[queueNum]==NULL)
        return 0;
    return queueSize(queues[queueNum]);
}

static int doFileCopy(TFCRequest* request)
{
    struct stat status;
    FILE    *file;
    int        bytes_read=0, last_read, total;
    char    *mem=0;
    int        chunk_size=65536;

    request->data = NULL;

    // Get file size and check for existence
    if(0!=stat(request->src, &status)){
        return -1;
    }
    if(!(status.st_mode & _S_IFREG))
        return -1;
    total = status.st_size;

    // Open file
    file = fopen(request->src,"rb");
    if (!file)
        return -1;

    // Allocate memory
    tfcRecordMalloc(total);
    mem = malloc(total);
    request->progressCallback(request, 0, total);
    // Start reading

    while (bytes_read < total) {
        if (total - bytes_read < chunk_size) {
            chunk_size = total - bytes_read;
        }
        last_read = fread(mem+bytes_read,1,chunk_size,file);
        if (last_read!=chunk_size) {
            // This should not happen.  Ever.  Well, unless a file is modified during the read
            //   process, or pigs are modifed while the game is running, or a file was deleted?
            tfcRecordFree(total);
            free(mem);
            return -1;
        } else if (last_read == -1) {
            // Error!
            tfcRecordFree(total);
            free(mem);
            return -1;
        }
        if (last_read > 0) {
            bytes_read+=last_read;
            request->progressCallback(request, bytes_read, total);
            recordBytesCopied(last_read);
        }
    }
    assert(bytes_read==total);
    fclose(file);

    // Write to destination file (unless flagged otherwise)
    if (!(request->flags & TFC_DO_NOT_WRITE)) {
        mkdirtree(request->dst);
        _chmod(request->dst, _S_IREAD | _S_IWRITE);
        file = fopen(request->dst, "wb");
        if (!file) {
            if (strEndsWith(request->dst, ".exe") || strEndsWith(request->dst, ".pdb") || strEndsWith(request->dst, ".dll")) {
                fileRenameToBak(request->dst);
                file = fopen(request->dst, "wb");
            }
        }
        if (!file) {
            tfcRecordFree(total);
            free(mem);
            return -1;
        }
        enterFileWriteCriticalSection();
        fwrite(mem, 1, total, file);
        leaveFileWriteCriticalSection();
        fclose(file);
    }
    // Send data back to main thread (for callback to use)
    request->data = mem;
    request->data_size = total;
    return bytes_read;
}

typedef struct ThreadStartParam {
    TFCRequestQueue *queue_in;
    TFCRequestQueue *queue_out;
} ThreadStartParam;

static DWORD WINAPI threadedFileCopyThread( LPVOID lpParam )
{
    EXCEPTION_HANDLER_BEGIN
    ThreadStartParam *param = (ThreadStartParam*)lpParam;
    TFCRequestQueue *queue_in = param->queue_in;
    TFCRequestQueue *queue_out = param->queue_out;

    for(;;)
    {
        TFCRequestQueueEntry *entry;
        while(entry = peekRequest(queue_in))
        {
            int ret = doFileCopy(entry->request);
            entry->type = TFC_COMPLETION;
            if (ret==-1) {
                // Failure
                entry->bytes=0;
                entry->total=1;
            } else {
                // success
                entry->bytes=ret;
                entry->total=ret;
            }
            queueRequest(queue_out, entry); // struct copy happens in here
            dequeueRequest(queue_in);
        }
        Sleep(1);
    }
    EXCEPTION_HANDLER_END
}


void allocThreadQueues(int num)
{
    if (queue_results==NULL)
        eaCreate(&queue_results);
    if (queue_requests==NULL)
        eaCreate(&queue_requests);
    while(eaSize(&queue_results)<num+1)
        eaPush(&queue_results, NULL);
    while(eaSize(&queue_requests)<num+1)
        eaPush(&queue_requests, NULL);
    if (queue_results[num]==NULL) {
        queue_results[num] = calloc(sizeof(*queue_results[num]), 1);
        queueInit(queue_results[num], THREAD_MAX_QUEUE_SIZE);
    }
    if (queue_requests[num]==NULL) {
        queue_requests[num] = calloc(sizeof(*queue_requests[num]), 1);
        queueInit(queue_requests[num], THREAD_MAX_QUEUE_SIZE);
    }
}


//todo: void threadedFileCopyChangeMaxThreads(int maxThreads);
void threadedFileCopyStartCopy(TFCRequest *request)
{
    TFCRequestQueueEntry entry={0};
    int ret;
    threadedFileCopyBPS(); // Init the critical section before creating threads
    if (tfc_max_threads==0) {
        allocThreadQueues(0);
        // Just copy syncronously
        ret = doFileCopy(request);
        entry.request = request;
        entry.type = TFC_COMPLETION;
        if (ret == -1) {
            entry.bytes = 0;
            entry.total = 1;
        } else {
            entry.total = ret;
            entry.bytes = ret;
        }
        queueRequest(queue_results[0], &entry);
    } else {
        // Queue async request
        int best = 0;
        int bestval = queueSizeSafe(queue_requests, 0);
        int i;
        for (i=1; i<tfc_max_threads; i++) {
            int val = queueSizeSafe(queue_requests, i);
            if (val < bestval) {
                best = i;
                bestval = val;
            }
        }
        if (threads[best]==0) {
            HANDLE threadHandle;
            ThreadStartParam *param = calloc(sizeof(ThreadStartParam),1);
            allocThreadQueues(best);
            param->queue_in = queue_requests[best];
            param->queue_out = queue_results[best];
            threadHandle = CreateThread( NULL, 0,
                threadedFileCopyThread, // thread function 
                (void*)param,           // argument to thread function 
                0,               // use default creation flags 
                &threads[best]); // returns the thread identifier 
            assert(tfc_num_threads == best); // Remove this if we allow dynamically killing/starting thredas
            tfc_num_threads = best+1;
        }
        entry.type = TFC_COPY;
        entry.request = request;
        if (bestval >= MAX_QUEUES_PER_THREAD) {
            // Too many queued in all threads!  Keep this one around until some queues clean out
            queueRequest(queue_notYetQueued, &entry);
        } else {
            queueRequest(queue_requests[best], &entry);
        }
    }
}

// Checks for progress/completetion
void threadedFileCopyPoll(ThreadStatusCallback statusCallback)
{
    int i;
    int left=0;
    int threadCounts[MAX_THREADS+1];
    int qs;
    int best=0;
    int bestval;
    if (queue_requests==NULL)
        return;
    bestval = queueSizeSafe(queue_requests, 0); // Find emptiest request queue
    for (i=0; i<MAX(tfc_num_threads, 1); i++) {
        TFCRequestQueueEntry *entry; // Deal with results
        while (entry = dequeueRequest(queue_results[i])) {
            switch(entry->type) {
            case TFC_COMPLETION:
                entry->request->finishCallback(entry->request, entry->bytes==entry->total, entry->total);
                tfcRecordFree(entry->request->data_size);
                SAFE_FREE(entry->request->data);
                destroyTFCRequest(entry->request);
                break;
            case TFC_PROGRESS:
                entry->request->progressCallback(entry->request, entry->bytes, entry->total);
                break;
            default:
                assert(0);
            }
        }
        qs = queueSize(queue_requests[i]); // Find emptiest request queue
        if (qs < bestval) {
            best = i;
            bestval = qs;
        }
        left += qs;
        threadCounts[i] = qs;
    }
    left += queueSize(queue_notYetQueued);
    while (bestval <5 && queueSize(queue_notYetQueued)) {
        TFCRequestQueueEntry *entry = dequeueRequest(queue_notYetQueued);
        queueRequest(queue_requests[best], entry);
        threadCounts[best]++;
        if (queueSize(queue_notYetQueued)) {
            // Look for new best
            bestval++;
            for (i=0; i<tfc_num_threads; i++) {
                qs = queueSizeSafe(queue_requests, i);
                if (qs < bestval) {
                    best = i;
                    bestval = qs;
                }
            }
        }
    }
    threadCounts[tfc_num_threads]=queueSize(queue_notYetQueued);

    if (statusCallback)
        statusCallback(left, tfc_num_threads+1, threadCounts);
}

void threadedFileCopyWaitForCompletion(ThreadStatusCallback statusCallback) // Waits for all pending copies to complete
{
    int i;
    int left;

    do {
        threadedFileCopyPoll(statusCallback);

        left = 0;
        for (i=0; i<tfc_num_threads; i++) {
            left += queueSize(queue_requests[i]);
            left += queueSize(queue_results[i]);
        }
        left += queueSize(queue_notYetQueued);
        if (!left)
            break;
        Sleep(1);
    } while (true);
}

int threadedFileCopyNumPending(void)
{
    int i;
    int left=0;

    for (i=0; i<tfc_num_threads; i++) {
        left += queueSize(queue_requests[i]);
        left += queueSize(queue_results[i]);
    }
    left += queueSize(queue_notYetQueued);
    return left;
}


// TODO: void threadedFileCopyShutdown(void) // Stops all copies


