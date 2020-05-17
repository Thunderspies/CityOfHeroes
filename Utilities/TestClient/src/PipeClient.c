#include <utilitieslib/stdtypes.h>
#include "PipeClient.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <utilitieslib/utils/error.h>
#include <utilitieslib/utils/memcheck.h>

PipeClient PipeClientCreate(const char *pipe_name, const char *pipe_server) {
    char full_pipe_name[MAX_PATH];
    PipeClient pc = calloc(1, sizeof(PipeClient_t));
    DWORD dwMode;
    BOOL fSuccess;

    sprintf(full_pipe_name, "\\\\%s\\pipe\\%s", pipe_server, pipe_name);
    while (1) {
        pc->hPipe = CreateFileA( 
            full_pipe_name,   // pipe name 
            GENERIC_READ |  // read and write access 
            GENERIC_WRITE, 
            0,              // no sharing 
            NULL,           // no security attributes
            OPEN_EXISTING,  // opens existing pipe 
            0,              // default attributes 
            NULL);          // no template file 

        // Break if the pipe handle is valid. 
        if (pc->hPipe != INVALID_HANDLE_VALUE) 
            break; 

        // Exit if an error other than ERROR_PIPE_BUSY occurs. 
        if (GetLastError() != ERROR_PIPE_BUSY) {
            //FatalErrorf("Could not open pipe %s", full_pipe_name); 
            free(pc);
            return NULL;
        }

        // All pipe instances are busy, so wait for 60 seconds. 
        if (!WaitNamedPipeA(full_pipe_name, 60000) ) {
            printf("\nCould not wait to open pipe %s\n", full_pipe_name);
            free(pc);
            return NULL;
        }
    } 

    // The pipe connected; change to message-read mode. 
    dwMode = PIPE_READMODE_MESSAGE; 
    fSuccess = SetNamedPipeHandleState( 
        pc->hPipe,    // pipe handle 
        &dwMode,  // new pipe mode 
        NULL,     // don't set maximum bytes 
        NULL);    // don't set maximum time 
    if (!fSuccess) 
        FatalErrorf("SetNamedPipeHandleState"); 
    pc->connected = PCS_CONNECTED;
    return pc;
}

void PipeClientDestroy(PipeClient p) {
    if (p && p->hPipe) {
        CloseHandle(p->hPipe); 
        p->hPipe = NULL;
    }
    free(p);
}

void PipeClientSendMessage(PipeClient pc, const char *fmt, ...) {
    DWORD cbWritten;
    BOOL fSuccess;
    char str[100000];
    va_list ap;

    if (pc==NULL) return;

    va_start(ap, fmt);
    vsprintf(str, fmt, ap);
    va_end(ap);

    // Send a message to the pipe server. 
    fSuccess = WriteFile( 
        pc->hPipe,                  // pipe handle 
        str,             // message 
        strlen(str) + 1, // message length 
        &cbWritten,             // bytes written 
        NULL);                  // not overlapped 
    if (! fSuccess) {
        pc->connected=PCS_DISCONNECTED;
        if (pc->hPipe)
            CloseHandle(pc->hPipe);
        pc->hPipe=NULL;
    }
}

char *PipeClientGetMessage(PipeClient pc) {
    DWORD numread, junk1, junk2;
    BOOL ret;

    if (pc==NULL) return NULL;

    ret = PeekNamedPipe(pc->hPipe, pc->buf, 1, &numread, &junk1, &junk2);
    if (!ret)
        pc->connected=PCS_DISCONNECTED;
    if (!ret || numread==0)
        return NULL;

    
    ret = ReadFile( 
        pc->hPipe,    // pipe handle 
        pc->buf,    // buffer to receive reply 
        ARRAY_SIZE(pc->buf),      // size of buffer 
        &numread,  // number of bytes read 
        NULL);    // not overlapped 

    if (! ret && GetLastError() != ERROR_MORE_DATA) 
        return NULL; 
    return pc->buf;
}
