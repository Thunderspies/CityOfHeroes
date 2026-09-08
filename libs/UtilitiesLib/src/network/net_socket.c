
#include <stdio.h>
#include "utilitieslib/assert/assert.h"

#include "utilitieslib/network/net_socket.h"
#include "utilitieslib/network/net_link.h"
#include "utilitieslib/network/net_linklist.h"

#include "utilitieslib/utils/log.h"
#include "utilitieslib/utils/timing.h"
#include "utilitieslib/components/MemoryPool.h"
#include "utilitieslib/utils/error.h"
#include "utilitieslib/network/sock.h"

int simpleSelect(FD_SET* readSet, FD_SET* writeSet, FD_SET* errorSet, F32 timeout){
    struct timeval interval;
    int selectResult;
    int socketError;

    interval.tv_sec = (long)timeout;
    interval.tv_usec = (long)((timeout - interval.tv_sec) * 1000000);

    selectResult = select(0, readSet, writeSet, errorSet, &interval);

    // Did I mess up the call to select somehow?
    if(SOCKET_ERROR == selectResult){
        socketError = WSAGetLastError();
        assert(SOCKET_ERROR != selectResult);
    }

    return selectResult;    
}


static MemoryPool AsyncOpContextMemoryPool = 0;
AsyncOpContext* createAsyncOpContext(){
    AsyncOpContext* context;
    if(!AsyncOpContextMemoryPool){
        AsyncOpContextMemoryPool = createMemoryPool();
        initMemoryPool(AsyncOpContextMemoryPool, sizeof(AsyncOpContext), 1024);
    }

    context = mpAlloc(AsyncOpContextMemoryPool);
    context->time = timerCpuTicks();
    context->inProgress = 0;
    return context;
}

void destroyAsyncOpContext(AsyncOpContext* context){
    //printf("%i bytes transferred in %f seconds asynchronously, %f bytes per second\n", context->bytesTransferred, timerSeconds(timerCpuTicks() - context->time), (float)context->bytesTransferred/seconds);
    mpFree(AsyncOpContextMemoryPool, context);
}

int simpleWSARecv(SOCKET sock, char* buf, int len, int useless, AsyncOpContext* context){
    DWORD    ulFlags = MSG_PARTIAL;
    DWORD      received = 0;
    WSABUF    bufferArray;
    int        result;
    context->type = AOT_Receive;
    ZeroStruct(&context->ol);
    bufferArray.len = len;
    bufferArray.buf = buf;

    assert(!context->inProgress);
    context->inProgress = 1;

    result = WSARecv(sock, 
        &bufferArray,
        1,
        &received, 
        &ulFlags,
        &context->ol, 
        NULL);

    if(result == SOCKET_ERROR)
    {
        int wsaError = WSAGetLastError();
        
        if(wsaError != WSA_IO_PENDING)
        {
            if(wsaError != WSAENOBUFS && !isDisconnectError(wsaError)) 
            {
                printWinErr("WSARecv", __FILE__, __LINE__, wsaError);
            }

            context->inProgress = 0;
        }
    }

    return result == 0 ? (int)received : 0;
}

int wsanobufs_hit=0;

int simpleWsaSend(SOCKET sock, char* buf, int length, int flags, AsyncOpContext* context){
    WSABUF sendBuf;
    DWORD bytesTransferred;

    //printf("Queuing async send op\n");
    sendBuf.buf = buf;
    sendBuf.len = length;
    context->type = AOT_Send;
    memset(&context->ol, 0, sizeof(OVERLAPPED));

    assert(!context->inProgress);
    context->inProgress = 1;

    LOG(LOG_NET, LOG_LEVEL_DEBUG, 0, "Async context at %x\n", context);

    if(0 != WSASend(sock, &sendBuf, 1, &bytesTransferred, 0, &context->ol, NULL)){
        int errVal = WSAGetLastError();

        // If the peer has disconnected, marked the socket to be destroyed.
        if(ERROR_IO_PENDING != errVal && !isDisconnectError(errVal)){
            if (errVal != WSAENOBUFS) { // This error has been known to happen on the UpdateServer
                printf("Unexpected send error %i\n", errVal);
            } else {
                wsanobufs_hit = 1;
            }
            //printf("AsyncOpContext at 0x%x\n", context);
        }
        if (errVal!= ERROR_IO_PENDING) {
            context->inProgress = 0;
        }

        return 0;
    }

    context->bytesTransferred = bytesTransferred;
    return (int)bytesTransferred;
}

int simpleWSARecvFrom(SOCKET sock, char* buf, int len, int flags, struct sockaddr* addr, int *addrLen, AsyncOpContext* context){
    DWORD      received = 0;
    WSABUF    bufferArray;
    int     result;
    DWORD   recvFlags = (DWORD)flags;

    assert(!context->inProgress);
    context->inProgress = 1;

    context->type = AOT_Receive;
    memset(&context->ol, 0, sizeof(OVERLAPPED));
    bufferArray.len = len;
    bufferArray.buf = buf;

    memset(addr, 0, *addrLen);

    PERFINFO_AUTO_START("WSARecvFrom", 1);
        result = WSARecvFrom(
                    sock,
                    &bufferArray,
                    1,
                    &received,
                    &recvFlags,
                    addr,
                    addrLen,
                    &context->ol,
                    NULL
        );
    PERFINFO_AUTO_STOP();

    if(result == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING) 
    {
        context->inProgress = 0;
        if(isDisconnectError(WSAGetLastError()))
            return SOCKET_ERROR;
        
        printWinErr("WSARecvFrom", __FILE__, __LINE__, WSAGetLastError());
        assert(0);
    }

    return result == 0 ? (int)received : 0;
}


/* Function SendToSock()
 *    A slightly simplified form of sendto().
 *
 */
int SendToSock(SOCKET fd, void* message, int len, struct sockaddr_in* addr){
    int    len_sent;

    len_sent = sendto(fd,message,len,0,(struct sockaddr *)addr,sizeof(struct sockaddr_in));
    return len_sent;
}

void FD_AddLink(FD_SET* readSet, FD_SET* writeSet, FD_SET* errorSet, NetLink* link){
    if(readSet)
        FD_SET(link->socket, readSet);

    if(writeSet && link->transmitCallback)
        FD_SET(link->socket, writeSet);    

    if(errorSet)
        FD_SET(link->socket, errorSet);    
}

void FD_AddLinkList(FD_SET* readSet, FD_SET* writeSet, FD_SET* errorSet, NetLinkList* linklist){
    int i;

    // Always wait for data on the udp socket.
    if(readSet && linklist->udp_sock)
        FD_SET(linklist->udp_sock, readSet);
    if(errorSet && linklist->udp_sock)
        FD_SET(linklist->udp_sock, errorSet);

    if(readSet && linklist->listen_sock)
        FD_SET(linklist->listen_sock, readSet);
    if(errorSet && linklist->listen_sock)
        FD_SET(linklist->listen_sock, errorSet);

    for(i = 0; i < linklist->links->size; i++){
        NetLink* link = linklist->links->storage[i];

        // Are we looking at the UDP socket for the NetLinkList?
        // Don't add UDP sockets more than once.
        if(link->socket == linklist->udp_sock){
            continue;
        }

        if(readSet)
            FD_SET(link->socket, readSet);
        if(errorSet)
            FD_SET(link->socket, errorSet);

        // Only add sockets to the write set if the user requests to have data transmission
        // as a priority.
        if(writeSet && link->transmitCallback){
            if(link->socket == linklist->udp_sock)
                continue;
            FD_SET(link->socket, writeSet);
        }
    }
}


void socketSetBufferSize(SOCKET socket, NetLinkBufferType type, int size){
    int ret, sizeof_size = sizeof(size);
    int result_size;

    if (type & SendBuffer) {
        //ret = getsockopt(socket, SOL_SOCKET,SO_SNDBUF, (char*)&result_size,&sizeof_size);
        //printf("    Old send buffer size on %d: %d\n", socket, result_size);
        ret = setsockopt(socket, SOL_SOCKET,SO_SNDBUF, (char*)&size,sizeof(size));
        if (ret!=0) goto fail;
        ret = getsockopt(socket, SOL_SOCKET,SO_SNDBUF, (char*)&result_size,&sizeof_size);
        if (ret!=0) goto fail;
        if (result_size!=size) {
            printf("Error calling setsockopt, ending socket buffer size is not what we told it! (%d!=%d)\n", size, result_size);
        }
        //ret = getsockopt(socket, SOL_SOCKET,SO_SNDBUF, (char*)&result_size,&sizeof_size);
        //printf("    New send buffer size on %d: %d\n", socket, result_size);
    }

    if (type & ReceiveBuffer) {
        //ret = getsockopt(socket, SOL_SOCKET,SO_RCVBUF, (char*)&result_size,&sizeof_size);
        //printf("    Old recv buffer size on %d: %d\n", socket, result_size);
        ret = setsockopt(socket, SOL_SOCKET,SO_RCVBUF, (char*)&size,sizeof(size));
        if (ret!=0) goto fail;
        ret = getsockopt(socket, SOL_SOCKET,SO_RCVBUF, (char*)&result_size,&sizeof_size);
        if (ret!=0) goto fail;
        if (result_size!=size) {
            printf("Error calling setsockopt, ending socket buffer size is not what we told it! (%d!=%d)\n", size, result_size);
        }
        //ret = getsockopt(socket, SOL_SOCKET,SO_RCVBUF, (char*)&result_size,&sizeof_size);
        //printf("    New recv buffer size on %d: %d\n", socket, result_size);
    }
/*
    if(SendBuffer == type)
        printf("New send buffer size = ");
    else
        printf("New receive buffer size = ");

    printf("%d\n",size);
*/
    return;

fail:

    printf("Socket error: ");
    printWinErr("socketSetBufferSize", __FILE__, __LINE__, WSAGetLastError());
    assert(0);
}
