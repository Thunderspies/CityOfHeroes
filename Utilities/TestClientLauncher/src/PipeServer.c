
#include <utilitieslib/stdtypes.h>
#include "PipeServer.h"
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/error.h>
#include <utilitieslib/assert/assert.h>
#include <utilitieslib/components/genericlist.h>
#include <utilitieslib/components/earray.h>

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 

#define BUFSIZE 100000
#define PIPE_TIMEOUT (5*60*1000) // 5 minutes?

static unsigned __stdcall WatcherThread(LPVOID);
static unsigned __stdcall InstanceThread(LPVOID);

PipeServer PipeServerCreate(const char *pipe_name) {
    unsigned dwThreadId;
    PipeServer ret = calloc(sizeof(PipeServer_t), 1);

    ret->pipename = strdup(pipe_name);
    InitializeCriticalSection(&(ret->critsect));
    ret->messagelist = NULL;
    ret->eaPipes = &ret->eaPipes_data;

    // Create a thread for this client. 
    ret->hWatcherThread = CreateThread( 
        NULL,              // no security attribute 
        0,                 // default stack size 
        WatcherThread,
        (LPVOID) ret,    // thread parameter 
        0,                 // not suspended 
        &dwThreadId);      // returns thread ID 

    if (ret->hWatcherThread == NULL) 
        FatalErrorf("CreateThread"); 

    // Create a thread for this all clients. 
    ret->hInstanceThread = CreateThread( 
        NULL,              // no security attribute 
        0,                 // default stack size 
        InstanceThread,
        (LPVOID) ret,      // thread parameter 
        0,                 // not suspended 
        &dwThreadId);      // returns thread ID 

    if (ret->hInstanceThread == NULL)
        FatalErrorf("CreateThread");

    return ret;
}

void PipeServerDestroy(PipeServer p) {
    int i;
    if (!p) return;
    EnterCriticalSection(&p->critsect);
    if (p->hWatcherThread) {
        TerminateThread(p->hWatcherThread, 0);
        CloseHandle(p->hWatcherThread);
    }
    if (p->hInstanceThread) {
        TerminateThread(p->hInstanceThread, 0);
        CloseHandle(p->hInstanceThread);
    }
    if (p->pipename) {
        free(p->pipename);
        p->pipename = NULL;
    }
    for (i=0; i<eaSizeUnsafe(p->eaPipes); i++) {
        HANDLE hPipe;
        hPipe = eaGet(p->eaPipes, i);
        if (hPipe) {
            FlushFileBuffers(hPipe); 
            DisconnectNamedPipe(hPipe); 
            CloseHandle(hPipe);
            eaSet(p->eaPipes, NULL, i);
        }
    }
    eaDestroy(p->eaPipes);
    LeaveCriticalSection(&p->critsect);
    DeleteCriticalSection(&p->critsect);
    free(p);
}

void PipeServerQuery(PipeServer p, PipeServerCallback callback) {
    static int count=0;
    if (!p) return;
    EnterCriticalSection(&p->critsect);
    if (count) return; // In case of multiple accesses from the same thread!
    count++;
    if (p->messagelist) {
        // Go to the end, and do a callback starting from the end of the list
        MessageList *ml = p->messagelist;
        while (ml->next) ml = ml->next;
        while (ml) {
            callback(ml->uid, ml->data);
            ml = ml->prev;
        }
    }
    // Delete all entries
    while (p->messagelist) {
        SAFE_FREE(p->messagelist->data);
        listFreeMember(p->messagelist, &p->messagelist);
    }
    count--;
    LeaveCriticalSection(&p->critsect);
}

static unsigned __stdcall WatcherThread(LPVOID param)
{ 
    SECURITY_DESCRIPTOR sd;
    BOOL fConnected; 
    HANDLE hPipe; 
    PipeServer ps = (PipeServer)param;
    char full_pipe_name[MAX_PATH];
    SECURITY_ATTRIBUTES sa;
    static int thread_num = 0;

    sprintf(full_pipe_name, "\\\\.\\pipe\\%s", ps->pipename);

    sa.nLength = sizeof(sa);
    sa.bInheritHandle = FALSE;
    sa.lpSecurityDescriptor = &sd;
    InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION);
    SetSecurityDescriptorDacl(&sd, TRUE, NULL, FALSE);

    // The main loop creates an instance of the named pipe and 
    // then waits for a client to connect to it. When the client 
    // connects, a thread is created to handle communications 
    // with that client, and the loop is repeated. 

    for (;;) 
    { 
        hPipe = CreateNamedPipeA( 
            full_pipe_name,           // pipe name 
            PIPE_ACCESS_DUPLEX,       // read/write access 
            PIPE_TYPE_MESSAGE |       // message type pipe 
            PIPE_READMODE_MESSAGE |   // message-read mode 
            PIPE_WAIT,                // blocking mode 
            PIPE_UNLIMITED_INSTANCES, // max. instances  
            BUFSIZE,                  // output buffer size 
            BUFSIZE,                  // input buffer size 
            PIPE_TIMEOUT,             // client time-out 
            &sa);                    // no security attribute 

        if (hPipe == INVALID_HANDLE_VALUE) {
            LPVOID lpMsgBuf;
            if (!FormatMessage( 
                FORMAT_MESSAGE_ALLOCATE_BUFFER | 
                FORMAT_MESSAGE_FROM_SYSTEM | 
                FORMAT_MESSAGE_IGNORE_INSERTS,
                NULL,
                GetLastError(),
                MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
                (LPTSTR) &lpMsgBuf,
                0,
                NULL ))
            {
            } else {
                // Display the string.
                MessageBox( NULL, (LPCTSTR)lpMsgBuf, TEXT("Error"), MB_OK | MB_ICONINFORMATION );
            }

            // Free the buffer.
            LocalFree( lpMsgBuf );
            FatalErrorf("CreatePipe");
        }

        // Wait for the client to connect; if it succeeds, 
        // the function returns a nonzero value. If the function returns 
        // zero, GetLastError returns ERROR_PIPE_CONNECTED. 

        fConnected = ConnectNamedPipe(hPipe, NULL) ? 
            TRUE : (GetLastError() == ERROR_PIPE_CONNECTED); 

        if (fConnected) 
        { 
            EnterCriticalSection(&ps->critsect);
            eaPush(ps->eaPipes, hPipe);
            LeaveCriticalSection(&ps->critsect);

        } 
        else {
            // The client could not connect, so close the pipe. 
            CloseHandle(hPipe); 
            assert(!"Error, client couldn't connect to pipe!");
        }
    } 
} 


static void queryPipe(PipeServer ps, int uid)
{
    CHAR chRequest[BUFSIZE]; 
    DWORD cbBytesRead; 
    BOOL fSuccess; 
    HANDLE hPipe = eaGet(ps->eaPipes, uid);
    DWORD numread=0, junk1, junk2;
    BOOL ret=0;
    bool disconnect=false;

    if (!hPipe)
        return;

    while (true)
    {
        ret = PeekNamedPipe(hPipe, chRequest, 1, &numread, &junk1, &junk2);
        if (ret==0) {
            // Disconnected from Pipe
            disconnect = true;
            break;
        }
        if (numread==0) {
            // No data ready
            return;
        }

        // Read client requests from the pipe. 
        fSuccess = ReadFile( 
            hPipe,        // handle to pipe 
            chRequest,    // buffer to receive data 
            BUFSIZE,      // size of buffer 
            &cbBytesRead, // number of bytes read 
            NULL);        // not overlapped I/O 

        if (! fSuccess || cbBytesRead == 0) {
            disconnect = true;
            break; 
        }

        // Send the data back to the main thread, through an linked list (we're already inside the CritSect
        {
            MessageList *ml;
            ml = listAddNewMember(&ps->messagelist, sizeof(MessageList));
            ml->uid = uid;
            ml->data = strdup(chRequest);
        }
    }

    // Flush the pipe to allow the client to read the pipe's contents 
    // before disconnecting. Then disconnect the pipe, and close the 
    // handle to this pipe instance. 

    assert(disconnect);

    if (eaGet(ps->eaPipes, uid)==NULL) { // closed via closePipe (in main thread probably)
        assert(0); // Don't think this can happen anymore because of CritSects
        return;
    }

    FlushFileBuffers(hPipe); 
    DisconnectNamedPipe(hPipe); 
    CloseHandle(hPipe); 
    eaSet(ps->eaPipes, NULL, uid);

    // Send the a disconnect back to the main thread, through an interlocked linked list
    {
        MessageList *ml;
        ml = listAddNewMember(&ps->messagelist, sizeof(MessageList));
        ml->uid = uid;
        ml->data = NULL;
    }

    //assert(!"Client disconnected");

}

// One of these is created to handle *all* clients, it needs to parse a message,
// and then get it back to the WinMain thread
static unsigned __stdcall InstanceThread(LPVOID lpvParam)
{ 
    PipeServer ps = (PipeServer)lpvParam;
    int i;

    while (1) 
    { 
        int cnt;
        EnterCriticalSection(&ps->critsect);
        cnt = eaSizeUnsafe(ps->eaPipes);
        for (i=0; i<cnt; i++) {
            // Query all of the pipes.
            queryPipe(ps, i);
        }
        LeaveCriticalSection(&ps->critsect);
        Sleep(250);
    }
} 


void PipeServerSendMessage(PipeServer p, int uid, const char *fmt, ...) {
    DWORD cbWritten;
    BOOL fSuccess;
    HANDLE hPipe;
    char str[20000];
    va_list ap;

    if (p==NULL) return;

    EnterCriticalSection(&p->critsect);
    hPipe = eaGet(p->eaPipes, uid);
    LeaveCriticalSection(&p->critsect);
    if (!hPipe)
        return;

    va_start(ap, fmt);
    vsprintf(str, fmt, ap);
    va_end(ap);

    // Send a message to the pipe client. 
    // This hangs if the client has closed the pipe!  Why doesn't it just error?!?
    fSuccess = WriteFile( 
        hPipe,                  // pipe handle 
        str,             // message 
        (DWORD)(strlen(str) + 1), // message length 
        &cbWritten,             // bytes written 
        NULL);                  // not overlapped 
    if (!fSuccess) 
        FatalErrorf("WriteFile"); 
}

void PipeServerClosePipe(PipeServer p, int uid) {
    HANDLE hPipe;
    if (p==NULL) return;

    EnterCriticalSection(&p->critsect);
    hPipe = eaGet(p->eaPipes, uid);
    if (!hPipe) {
        LeaveCriticalSection(&p->critsect);
        return;
    }

    FlushFileBuffers(hPipe); 
    DisconnectNamedPipe(hPipe); 
    CloseHandle(hPipe); 

    eaSet(p->eaPipes, NULL, uid);
    LeaveCriticalSection(&p->critsect);

}
