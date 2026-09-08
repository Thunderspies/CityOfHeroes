#ifndef NET_SOCKET_H
#define NET_SOCKET_H

#include "../utils/wininclude.h"
#include "../stdtypes.h"
#include "../network/net_typedefs.h"
#include "../network/net_structdefs.h"

int simpleSelect(FD_SET* readSet, FD_SET* writeSet, FD_SET* errorSet, F32 timeout);

AsyncOpContext* createAsyncOpContext();
void destroyAsyncOpContext(AsyncOpContext* context);
int simpleWSARecv(SOCKET sock, char* buf, int len, int useless, AsyncOpContext* context);
int simpleWsaSend(SOCKET sock, char* buf, int length, int flags, AsyncOpContext* context);

int simpleWSARecvFrom(SOCKET sock, char* buf, int len, int flags, struct sockaddr* addr, int *addrLen, AsyncOpContext* context);

int SendToSock(SOCKET fd, void* message, int len, struct sockaddr_in* addr);

void FD_AddLinkList(FD_SET* readSet, FD_SET* writeSet, FD_SET* errorSet, NetLinkList* linklist);
void FD_AddLink(FD_SET* readSet, FD_SET* writeSet, FD_SET* errorSet, NetLink* link);

void socketSetBufferSize(SOCKET socket, NetLinkBufferType type, int size);

#endif