#include "utilitieslib/utils/wininclude.h"
#include "utilitieslib/components/WorkerThread.h"
#include "utilitieslib/assert/assert.h"
#include "utilitieslib/utils/file.h"
#include "utilitieslib/components/Queue.h"
#include "utilitieslib/utils/utils.h"

#define SAFETY_CHECKS_ON 0

static void wtDispatchMessages(WorkerThread *wt);

#ifdef _M_X64
#    pragma warning(push)
#    pragma warning(disable:4324)
#endif
typedef struct __declspec(align(16)) QueuedCmd
{
    int            type;
    U32            num_blocks;
    U32            size;
    void        *data;
} QueuedCmd;
#ifdef _M_X64
#    pragma warning(pop)
#endif

typedef struct CmdQueue
{
    QueuedCmd        *queue;
    U32                max_queued;
    volatile U32    start, end;
    U32                next_end;
} CmdQueue;

typedef enum
{
    THREAD_NOTSTARTED=0,
    THREAD_RUNNING,
    THREAD_TOLDTOEXIT,
    THREAD_EXITED,
} ThreadState;


typedef struct WorkerThread
{
    U32 run_threaded : 1;
    U32 flush_requested : 1;
    U32 debug : 1;

    CmdQueue cmd_queue;
    CmdQueue msg_queue;

    volatile int    thread_asleep; // thread sleeps when it has no data queued
    HANDLE            data_queued; // event signalling that data is queued, wakes up sleeping thread

    // thread info
    HANDLE thread_handle;
    DWORD thread_id;
    volatile ThreadState thread_state;

    // for wtQueueAllocCmd/wtQueueSendCmd
    int curr_active_cmdtype;

    // dispatch functions and user data
    void *user_data;
    WTDispatchCallback cmd_dispatch, msg_dispatch;
    WTStallCallback stall_callback;

    // for queueing messages (which can be queued from in and out of the thread)
    CRITICAL_SECTION msg_criticalsection;

    int processor_idx;

} WorkerThread;


bool wtInWorkerThread(WorkerThread *wt)
{
    if (wt && wt->thread_id)
        return (wt->thread_id == (int)GetCurrentThreadId());
    return false;
}

void wtAssertWorkerThread(WorkerThread *wt)
{
    if (wt && wt->thread_id)
        assert(wt->thread_id == (int)GetCurrentThreadId());
}

void wtAssertNotWorkerThread(WorkerThread *wt)
{
    if (wt && wt->thread_id)
        assert(wt->thread_id != (int)GetCurrentThreadId());
}

void wtSetFlushRequested(WorkerThread *wt, int flush_requested)
{
    wt->flush_requested = !!flush_requested;
}

static INLINEDBG void wtReqFlush(WorkerThread *wt, int sleep)
{
    wt->flush_requested = 1;
    if (wt->data_queued)
        SetEvent(wt->data_queued);
    if (sleep)
        Sleep(1);
    wt->flush_requested = 0;
}

static INLINEDBG void getStoredCmd(CmdQueue *cmd_queue)
{
    QueuedCmd *entry;
    if (cmd_queue->start == cmd_queue->end)
        return;
    entry = &cmd_queue->queue[cmd_queue->start];
    assert(entry->num_blocks); // Must be *before* the following line, lest the other thread use it right away
    cmd_queue->start = (cmd_queue->start + entry->num_blocks) & (cmd_queue->max_queued-1);
}

static INLINEDBG QueuedCmd *peekStoredCmd(CmdQueue *cmd_queue)
{
    if (cmd_queue->start == cmd_queue->end)
        return 0;
    assert(cmd_queue->queue[cmd_queue->start].num_blocks);
    return &cmd_queue->queue[cmd_queue->start];
}

static INLINEDBG void handleQueuedCmd(QueuedCmd *cmd, WTDispatchCallback dispatch, void *user_data)
{
    void *data;

    if (cmd->data || cmd->size==0) // Size 0 -> data == NULL
        data = cmd->data;
    else
        data = (U8*)(cmd+1);

    if (cmd->type >= WT_CMD_USER_START)
        dispatch(user_data,cmd->type,data);
    else if (cmd->type == WT_CMD_DEBUGCALLBACK && data)
        (*((WTDebugCallback*)data))((char*)data + sizeof(WTDebugCallback));

    if (cmd->data)
        free(cmd->data);
}

static INLINEDBG void *allocStoredCmd(WorkerThread *wt, CmdQueue *cmd_queue, int cmd_type, U32 size)
{
    int            free_blocks,num_blocks,filler_blocks,contiguous_blocks,malloc_size;
    QueuedCmd    *cmd;

    if (!wt || (size > (sizeof(*cmd) * cmd_queue->max_queued/2)))
    {
        malloc_size = size;
        size = 0;
    }
    else
        malloc_size = 0;
    num_blocks = 1 + (size + sizeof(QueuedCmd) - 1) / sizeof(QueuedCmd);

    contiguous_blocks = cmd_queue->max_queued - cmd_queue->end;
    if (num_blocks > contiguous_blocks)
        filler_blocks = contiguous_blocks;
    else
        filler_blocks = 0;
    for(;;)
    {
        free_blocks = (cmd_queue->start - (cmd_queue->end+1)) & (cmd_queue->max_queued-1);
        if (num_blocks+filler_blocks <= free_blocks)
            break;

        assert(wt);
#if SAFETY_CHECKS_ON
        wtAssertNotWorkerThread(wt);
#endif

        // wait for worker thread to free some blocks
        wtReqFlush(wt, 1);
        wtDispatchMessages(wt);

        // call the users callback to indicate a stall occured
        if(wt->stall_callback) {
            wt->stall_callback();
        }
    }
    cmd_queue->next_end = (cmd_queue->end + num_blocks + filler_blocks) & (cmd_queue->max_queued-1);

    cmd = &cmd_queue->queue[cmd_queue->end];
    if (filler_blocks)
    {
        memset(cmd,0,sizeof(*cmd));
        cmd->type = WT_CMD_NOP;
        cmd->num_blocks = filler_blocks;
        cmd = &cmd_queue->queue[(cmd_queue->end + filler_blocks) & (cmd_queue->max_queued-1)];
    }
    cmd->type        = cmd_type;
    cmd->num_blocks = num_blocks;
    if (!malloc_size)
    {
        cmd->size = size;
        cmd->data = 0;
        return cmd+1;
    }
    else
    {
        cmd->size = malloc_size;
        cmd->data = calloc(malloc_size, 1);
        return cmd->data;
    }
}

static void wtDispatchMessages(WorkerThread *wt)
{
    QueuedCmd *cmd;

#if SAFETY_CHECKS_ON
    wtAssertNotWorkerThread(wt);
#endif

    while (cmd = peekStoredCmd(&wt->msg_queue))
    {
        handleQueuedCmd(cmd, wt->msg_dispatch, wt->user_data);
        getStoredCmd(&wt->msg_queue);
    }
}

void wtFlushMessages(WorkerThread *wt)
{
    while (wt->run_threaded && wt->thread_handle)
    {
        if (wt->msg_queue.start == wt->msg_queue.end)
            break;
        Sleep(1);
    }
}

void wtFlush(WorkerThread *wt)
{
#if SAFETY_CHECKS_ON
    wtAssertNotWorkerThread(wt);
#endif
    while (wt->run_threaded && wt->thread_handle)
    {
        if (wt->cmd_queue.start == wt->cmd_queue.end)
            break;
        wtReqFlush(wt, 1);
        wtDispatchMessages(wt);
    }
}

void wtMonitor(WorkerThread *wt)
{
#if SAFETY_CHECKS_ON
    wtAssertNotWorkerThread(wt);
#endif
    wtReqFlush(wt, 0);
    wtDispatchMessages(wt);
}

static INLINEDBG void initCmdQueue(CmdQueue *cmd_queue, int max_cmds)
{
    cmd_queue->queue = calloc(max_cmds, sizeof(QueuedCmd));
    cmd_queue->max_queued = max_cmds;
}

WorkerThread *wtCreate(WTDispatchCallback cmd_dispatch, int max_cmds, WTDispatchCallback msg_dispatch, WTStallCallback stall_callback, int max_msgs, void *user_data)
{
    WorkerThread *wt = calloc(1, sizeof(*wt));
    wt->cmd_dispatch = cmd_dispatch;
    wt->msg_dispatch = msg_dispatch;
    wt->stall_callback = stall_callback;
    wt->user_data = user_data;
    wt->processor_idx = -1;
    initCmdQueue(&wt->cmd_queue, max_cmds);
    initCmdQueue(&wt->msg_queue, max_msgs);
    InitializeCriticalSectionAndSpinCount(&wt->msg_criticalsection, 4000);
    return wt;
}

void wtSetProcessor(WorkerThread *wt, int processor_idx)
{
    // right now this only does something on xbox
    wt->processor_idx = processor_idx;

#ifdef _XBOX
    if (wt->thread_handle && processor_idx >= 0)
        XSetThreadProcessor(wt->thread_handle, wt->processor_idx);
#endif
}

void wtDestroy(WorkerThread *wt)
{
    wtFlush(wt);
    if (wt->thread_handle && wt->thread_state != THREAD_EXITED)
    {
        wt->thread_state = THREAD_TOLDTOEXIT;
        while (wt->thread_state != THREAD_EXITED)
            wtReqFlush(wt, 1);
        CloseHandle(wt->data_queued);
        CloseHandle(wt->thread_handle);
    }
    DeleteCriticalSection(&wt->msg_criticalsection);
    free(wt->cmd_queue.queue);
    free(wt->msg_queue.queue);
    ZeroStruct(wt);
    free(wt);
}

void wtSetThreaded(WorkerThread *wt,int run_threaded)
{
    wt->run_threaded = !!run_threaded;
}

int wtIsThreaded(WorkerThread *wt)
{
    return wt->run_threaded;
}

HANDLE wtGetThreadHandle(WorkerThread *wt)
{
    if (!wt->run_threaded)
        return NULL;
    return wt->thread_handle;
}

U32 wtGetThreadID(WorkerThread *wt)
{
    if (!wt->run_threaded)
        return 0;
    return wt->thread_id;
}

static DWORD WINAPI wtThread( LPVOID lpParam )
{
    EXCEPTION_HANDLER_BEGIN

    WorkerThread *wt;
    QueuedCmd *cmd,last;

    wt = (WorkerThread *)lpParam;
    wt->thread_id = (int)GetCurrentThreadId();
    wt->thread_state = THREAD_RUNNING;
    wt->data_queued = CreateEvent(0,0,0,0);
#ifdef _XBOX
    if (wt->processor_idx >= 0)
        XSetThreadProcessor(wt->thread_handle, wt->processor_idx);
#endif

    for(;;)
    {
        if (wt->thread_state == THREAD_TOLDTOEXIT)
        {
            wt->thread_state = THREAD_EXITED;
            ExitThread(0);
        }

        if (wt->debug && !wt->flush_requested && wt->run_threaded)
            cmd = 0;
        else
            cmd = peekStoredCmd(&wt->cmd_queue);
        if (!cmd)
        {
            if (wt->data_queued)
            {
                ResetEvent(wt->data_queued);
                cmd = peekStoredCmd(&wt->cmd_queue);
                if (!cmd) {
                    wt->thread_asleep = 1;
                    WaitForSingleObject(wt->data_queued, INFINITE);
                    wt->thread_asleep = 0;
                }
            }
            continue;
        }

        handleQueuedCmd(cmd, wt->cmd_dispatch, wt->user_data);

        last = *cmd;
        getStoredCmd(&wt->cmd_queue);
    }
    return 0; 
    EXCEPTION_HANDLER_END
} 

void wtStart(WorkerThread *wt)
{
    DWORD dwThreadId; 

    if (!wt->run_threaded)
        return;

    wt->thread_handle = CreateThread(0,0,wtThread,wt,0,&dwThreadId);
    wt->thread_id = dwThreadId;
//     if (isDevelopmentMode())
//         SetThreadPriority(wt->thread_handle, THREAD_PRIORITY_LOWEST);
    //ret = SetThreadIdealProcessor(render_thread_handle,1);
    //ret = SetThreadIdealProcessor(GetCurrentThread(),0);
    while (wt->thread_state != THREAD_RUNNING)
        Sleep(1);
}

void wtQueueDebugCmd(WorkerThread *wt,WTDebugCallback callback_func,void *data,int size)
{
    WTDebugCallback *cb_data = _alloca(size+sizeof(WTDebugCallback));
    *cb_data = callback_func;
    memcpy(cb_data+1,data,size);
    wtQueueCmd(wt,WT_CMD_DEBUGCALLBACK,cb_data,size+sizeof(WTDebugCallback));
}

void *wtAllocCmd(WorkerThread *wt, int cmd_type, int size)
{
    assert(!wt->curr_active_cmdtype);
    wt->curr_active_cmdtype = cmd_type;
    return allocStoredCmd(wt, &wt->cmd_queue, cmd_type, size);
}

void wtSendCmd(WorkerThread *wt)
{
    wt->curr_active_cmdtype = 0;
    if (!wt->run_threaded)
    {
        handleQueuedCmd(&wt->cmd_queue.queue[0], wt->cmd_dispatch, wt->user_data);
        return;
    }
    wt->cmd_queue.end = wt->cmd_queue.next_end;
#if SAFETY_CHECKS_ON
    wtAssertNotWorkerThread(wt);
#endif
    if (wt->thread_asleep && wt->data_queued)
        SetEvent(wt->data_queued);
}

void wtCancelCmd(WorkerThread *wt)
{
    QueuedCmd    *cmd = &wt->cmd_queue.queue[wt->cmd_queue.end];

    wt->curr_active_cmdtype = 0;
    if (cmd->data)
    {
        free(cmd->data);
        cmd->data = 0;
    }
}

void wtQueueCmd(WorkerThread *wt,int cmd_type,const void *cmd_data,int size)
{
    void    *mem;
#if SAFETY_CHECKS_ON
    wtAssertNotWorkerThread(wt);
#endif
    mem = wtAllocCmd(wt,cmd_type,size);
    if ( size )
        memcpy(mem,cmd_data,size);
    wtSendCmd(wt);
}

void wtQueueMsg(WorkerThread *wt,int msg_type,void *msg_data,int size)
{
    void *mem;
    EnterCriticalSection(&wt->msg_criticalsection);
    mem = allocStoredCmd(0, &wt->msg_queue, msg_type, size);
    memcpy(mem,msg_data,size);
    wt->msg_queue.end = wt->msg_queue.next_end;
    LeaveCriticalSection(&wt->msg_criticalsection);
}


