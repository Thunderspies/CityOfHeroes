#include "Thread.h"
#include "Job.h"
#include "IOServer.h"
#include "config.h"
#include "DBConn.h"
#include "ServerList.h"
#include "IPSessionDB.h"
#include "logsocket.h"
#include "logprotocol.h"
#include "util.h"


HANDLE g_hIOCompletionPort;    // For Accept ( Used for World Server Connection )
HANDLE g_hIOCompletionPortInt; // For New Line Terminate Protocol ( Used for Interactive Socket )


HANDLE *g_hThreadInt;

bool  g_bTerminating = false;
long  g_nRunningThread = 0;
int   g_updateKey=0, g_updateKey2 = 0;


#ifdef DEBUG_MEMORY
#undef new
void* __cdecl operator new(size_t size, LPCSTR fileName, int line)
{
    return _malloc_dbg(size, _NORMAL_BLOCK, fileName, line);
}
void operator delete(void *ptr, LPCSTR fileName, int lineNumber)
{
    _free_dbg(ptr, _NORMAL_BLOCK);
}
#endif

BOOL CreateIOThread();

unsigned __stdcall TimerThread(void *)
{

    job.RunTimer();
    logger.AddLog(LOG_DEBUG, "Timer thread terminated");

    return 0;
}

unsigned __stdcall IOThreadServer( void *)
{
_BEFORE
    srand( (unsigned)time( NULL ) );
    for ( ; ; ) {
        
        if (g_bTerminating) {
            logger.AddLog(LOG_DEBUG, "terminate IOThreadServer");
            break;
        }

        DWORD dwTransferred = 0;
        CIOObject *pObject = NULL;
        LPOVERLAPPED lpOverlapped = NULL;
        BOOL bSuccess = GetQueuedCompletionStatus(g_hIOCompletionPort, &dwTransferred, (PULONG_PTR)&pObject, &lpOverlapped, INFINITE);

        if (pObject) {
            InterlockedIncrement( &g_nRunningThread);
            pObject->OnIOCallback(bSuccess, dwTransferred, lpOverlapped);
            InterlockedDecrement( &g_nRunningThread);
        }
    }

    logger.AddLog(LOG_DEBUG, "IOThread Server Exit");
_AFTER_FIN
    return 0;
}

unsigned __stdcall IOThreadInt( void * )
{
_BEFORE
    srand( (unsigned)time( NULL ) );
    for ( ; ; ) {

        if (g_bTerminating) {
            logger.AddLog(LOG_DEBUG, "terminate IOThreadInt");
            break;
        }

        DWORD dwTransferred = 0;
        CIOObject *pObject = NULL;
        LPOVERLAPPED lpOverlapped = NULL;
        BOOL bSuccess = GetQueuedCompletionStatus(g_hIOCompletionPortInt, &dwTransferred, (PULONG_PTR)&pObject, &lpOverlapped, INFINITE);

        if (pObject)
            pObject->OnIOCallback(bSuccess, dwTransferred, lpOverlapped);

    }
_AFTER_FIN
    return 0;
}


HANDLE     *g_hServerThread, 
         *g_hServerThreadInt, 
         *g_hServerThreadClient;

unsigned *g_nServerThreadId, 
         *g_nServerThreadIntId, 
         *g_nServerThreadClientId;

CIPSocket *pIPSocket = NULL;
CLogSocket *pLogSocket = NULL;

unsigned __stdcall ListenThread( void *param )
{
_BEFORE

    server->Run( config.serverPort, CSocketServer::Allocate,  false );
    serverEx->Run( config.serverExPort, CSocketServerEx::Allocate );
    serverInt->Run( config.serverIntPort, CSocketInt::Allocate);
    
    job.RunEvent();
    
    server->Close();
    serverEx->Stop();
    serverInt->Close();


    if ( pIPSocket ){
        gIPLock.WriteLock();
        if ( IPServerReconnect == false )
            pIPSocket->CloseSocket();
        pIPSocket->ReleaseRef();
        gIPLock.WriteUnlock();
    }

    if ( pLogSocket){
        gLogLock.WriteLock();
        if ( LogDReconnect == false )
            pLogSocket->CloseSocket();
        pLogSocket->ReleaseRef();
        gLogLock.WriteUnlock();
    }
    
    free(g_hServerThread);
    free(g_nServerThreadId); 

    
    free(g_hServerThreadInt);
    free(g_nServerThreadIntId);

    globalTeminateEvent = true;

_AFTER_FIN
    return 0;
}

BOOL CreateIOThread( void  )
{
_BEFORE
    unsigned int timerThreadId;
    ULONG_PTR i=0;
    _beginthreadex( NULL, 0, TimerThread, (void *)i, 0, &timerThreadId );

    g_hIOCompletionPort    = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, config.numServerThread);
    g_hIOCompletionPortInt = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, config.numServerIntThread);

    g_hServerThread = (HANDLE *) calloc(config.numServerThread, sizeof(HANDLE));
    g_nServerThreadId = (unsigned *) calloc(config.numServerThread, sizeof(unsigned));
    g_hServerThreadInt = (HANDLE *) calloc(config.numServerIntThread, sizeof(HANDLE));
    g_nServerThreadIntId = (unsigned *) calloc(config.numServerIntThread, sizeof(unsigned));
    
    
    for (i = 0; i < (ULONG_PTR)config.numServerThread; i++) {
        g_hServerThread[i] = (HANDLE)_beginthreadex(NULL, 0, IOThreadServer, (void *) i, 0, &g_nServerThreadId[i]);
    }
    
    for ( i = 0; i < (ULONG_PTR)config.numServerIntThread; i++) {
        g_hServerThreadInt[i] = (HANDLE)_beginthreadex(NULL, 0, IOThreadInt, (void *) i, 0, &g_nServerThreadIntId[i]);
    }

_AFTER_FIN
    return TRUE;
}

