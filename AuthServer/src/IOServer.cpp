// IOServer.cpp: implementation of the CIOServer class.
//
//////////////////////////////////////////////////////////////////////
#include "GlobalAuth.h"
#include "IOServer.h"
#include "ServerList.h"
#include "Thread.h"
#include "Config.h"
#include "IPList.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//#define LISTEN_BACKLOG 5
#define LISTEN_BACKLOG SOMAXCONN

CServer *server = NULL;
CIOServerEx *serverEx = NULL;
CServerInt *serverInt = NULL;

HANDLE g_hSocketTimer;
long g_AcceptExThread;
long g_Accepts = 0;

CIOServer::CIOServer()
{
    m_hSocket = INVALID_SOCKET;
    m_hAcceptEvent = WSA_INVALID_EVENT;
}

CIOServer::~CIOServer()
{
    Close();
    WSACloseEvent(m_hAcceptEvent);
}

void CIOServer::Close()
{
    SOCKET temp_socket;

    if ( m_hSocket != INVALID_SOCKET ) {
        temp_socket = (SOCKET)InterlockedExchangePointer((PVOID volatile*)&m_hSocket, (PVOID)INVALID_SOCKET);
        closesocket(temp_socket);
    }
}

void CIOServer::Stop()
{
    SOCKET hSocket = m_hSocket;
    m_hSocket = INVALID_SOCKET;
    closesocket(hSocket);
}

BOOL CIOServer::Create( int nPort )
{
    m_hSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_hSocket == INVALID_SOCKET) {
        logger.AddLog(LOG_ERROR, "socket error %d", WSAGetLastError());
        return FALSE;
    }

    struct sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    sin.sin_port = htons(nPort);

    if (bind(m_hSocket, (struct sockaddr*)&sin, sizeof(sin))) {
        logger.AddLog(LOG_ERROR, "bind error %d", WSAGetLastError());
        goto fail;
    }
    if (listen(m_hSocket, LISTEN_BACKLOG)) {
        logger.AddLog(LOG_ERROR, "listen error %d", WSAGetLastError());
        goto fail;
    }

    m_hAcceptEvent = WSACreateEvent();
    WSAEventSelect(m_hSocket, m_hAcceptEvent, FD_ACCEPT);
    if (!RegisterEvent(m_hAcceptEvent)) {
        logger.AddLog(LOG_ERROR, "RegisterWait error on port %d", nPort);
        goto fail;
    }
    
    return TRUE;

fail:
    Close();
    return FALSE;
}

void CIOServer::OnIOCallback(BOOL bSuccess, DWORD dwTransferred, LPOVERLAPPED lpOverlapped)
{
_BEFORE
    struct sockaddr_in clientAddress;
    int clientAddressLength = sizeof(clientAddress);
    SOCKET newSocket = accept(m_hSocket, (struct sockaddr*)&clientAddress, &clientAddressLength);
    if (newSocket == INVALID_SOCKET) {
        if (WSAGetLastError() == WSAEWOULDBLOCK) {
            return;
        }
        else {
            if (m_hSocket != INVALID_SOCKET)
                logger.AddLog(LOG_ERROR, "accept error: %d", WSAGetLastError());
            return;
        }
    }
    InterlockedIncrement( &g_Accepts );
    CIOSocket *pSocket = CreateSocket(newSocket, &clientAddress);
    if (pSocket == NULL) {
        logger.AddLog(LOG_ERROR, "ServerClose:CreateSocket Fail" );
        closesocket(newSocket);
        return;
    }
    pSocket->Initialize( g_hIOCompletionPort );
_AFTER_FIN
    return;
}

void CIOServer::OnEventCallback( void )
{
    WSAResetEvent(m_hAcceptEvent);
    PostQueuedCompletionStatus(g_hIOCompletionPort, 0, (ULONG_PTR)this, NULL);
}



CServer::CServer()
{
    shutdown = false;
    InitializeCriticalSectionAndSpinCount(&sockSect, 4000);
}

CServer::~CServer()
{
    DeleteCriticalSection(&sockSect);
//    CloseHandle(g_hIOCompletionPort);    
}

CIOSocket *CServer::CreateSocket( SOCKET newSocket, LPSOCKADDR_IN pAddress )
{

    logger.AddLog(LOG_NORMAL, "*new world server connection from %d.%d.%d.%d",             
            pAddress->sin_addr.S_un.S_un_b.s_b1,
            pAddress->sin_addr.S_un.S_un_b.s_b2,
            pAddress->sin_addr.S_un.S_un_b.s_b3,
            pAddress->sin_addr.S_un.S_un_b.s_b4);

    ServerId serverid;
    
    CSocketServer* mysocket = (*allocator)(newSocket);
    
    // CThurow 2/06 --- Allow the game server to specify its own server id.  If the
    // gameServerSpecifiesId config option is set, don't assign the server id right now.
    // Wait for the ServerVersionEx packet, and assign the server id then.
    if (!config.gameServerSpecifiesId)
    {
        serverid = g_ServerList.SetServerSocketByAddress( pAddress->sin_addr, mysocket );
        if ( !serverid.IsValid() ){        
            logger.AddLog(LOG_ERROR, "Non-registered world server %d.%d.%d.%d", 
                            pAddress->sin_addr.S_un.S_un_b.s_b1,
                            pAddress->sin_addr.S_un.S_un_b.s_b2,
                            pAddress->sin_addr.S_un.S_un_b.s_b3,
                            pAddress->sin_addr.S_un.S_un_b.s_b4);
            delete mysocket;
            return NULL;
        }
        else
        {
            //TBROWN
            logger.AddLog(LOG_ERROR, "Registered world server: ServerId: %d   IP: %d.%d.%d.%d", 
                            serverid,
                            pAddress->sin_addr.S_un.S_un_b.s_b1,
                            pAddress->sin_addr.S_un.S_un_b.s_b2,
                            pAddress->sin_addr.S_un.S_un_b.s_b3,
                            pAddress->sin_addr.S_un.S_un_b.s_b4);
        }
    }

    mysocket->serverid = serverid;    
    mysocket->SetAddress(pAddress->sin_addr);
    EnterCriticalSection(&sockSect);
    socketmap.insert(SocketMap::value_type( (pAddress->sin_addr.S_un.S_addr), mysocket));
    LeaveCriticalSection(&sockSect);


    return mysocket;
}

bool CServer::InShutdown()
{
    return listener == 0;
}

void CServer::Run(int port, CSocketServer* (*anAllocator)(SOCKET s), bool aRestrict)
{
    allocator = anAllocator;
    restrict = aRestrict;
    if (Create(port))
        logger.AddLog(LOG_NORMAL, "server ready on port %d", port);
    listener = m_hSocket;
}

CSocketServer *CServer::FindSocket( in_addr s )
{
    CSocketServer *pSocket = NULL;
    EnterCriticalSection(&sockSect);
    SocketMap::iterator i = socketmap.find(s.S_un.S_addr);
    if (i != socketmap.end()) {
        pSocket = i->second;
        pSocket->AddRef();
    }
    LeaveCriticalSection(&sockSect);
    return pSocket;
}

bool CServer::GetServerStatus( in_addr s )
{
    bool result=false;

    EnterCriticalSection( &sockSect );
    SocketMap::iterator i = socketmap.find( s.S_un.S_addr );
    if ( i != socketmap.end()) {
        result = true;
    }
    LeaveCriticalSection( &sockSect);
    return result;
}

void CServer::RemoveSocket(in_addr s)
{
    EnterCriticalSection(&sockSect);
    SocketMap::iterator i = socketmap.find(s.S_un.S_addr);
    if (i != socketmap.end()) {
        socketmap.erase(i);
    }
    LeaveCriticalSection(&sockSect);
}

CServerInt::CServerInt()
{
    shutdown = false;
    InitializeCriticalSectionAndSpinCount(&sockSect, 4000);
}

CServerInt::~CServerInt()
{
    DeleteCriticalSection(&sockSect);
    CloseHandle(g_hIOCompletionPortInt);    
}

void CServerInt::OnIOCallback(BOOL bSuccess, DWORD dwTransferred, LPOVERLAPPED lpOverlapped)
{
    struct sockaddr_in clientAddress;
    int clientAddressLength = sizeof(clientAddress);
    SOCKET newSocket = accept(m_hSocket, (struct sockaddr*)&clientAddress, &clientAddressLength);
    if (newSocket == INVALID_SOCKET) {
        if (WSAGetLastError() == WSAEWOULDBLOCK) {
            return;
        }
        else {
            if (m_hSocket != INVALID_SOCKET)
                logger.AddLog(LOG_ERROR, "accept error: %d", WSAGetLastError());
            return;
        }
    }
    InterlockedIncrement( &g_Accepts );
    CIOSocket *pSocket = CreateSocket(newSocket, &clientAddress);
    if (pSocket == NULL) {
        closesocket(newSocket);
        return;
    }
    pSocket->Initialize( g_hIOCompletionPortInt );

    return;
}

CIOSocket *CServerInt::CreateSocket( SOCKET newSocket, LPSOCKADDR_IN pAddress )
{
    CSocketInt* mysocket = (*allocator)(newSocket);
    mysocket->SetAddress(pAddress->sin_addr);
    logger.AddLog(LOG_NORMAL, "*new interactive socket connection from %d.%d.%d.%d",             
            pAddress->sin_addr.S_un.S_un_b.s_b1,
            pAddress->sin_addr.S_un.S_un_b.s_b2,
            pAddress->sin_addr.S_un.S_un_b.s_b3,
            pAddress->sin_addr.S_un.S_un_b.s_b4);

    return mysocket;
}

bool CServerInt::InShutdown()
{
    return listener == 0;
}

void CServerInt::Run(int port, CSocketInt* (*anAllocator)(SOCKET s))
{
    allocator = anAllocator;
    if (Create(port))
        logger.AddLog(LOG_NORMAL, "interactive server ready on port %d", port);
    listener = m_hSocket;
}

OverlappedPool::Slot OverlappedPool::m_slot[16];
long OverlappedPool::m_alloc = -1;
long OverlappedPool::m_free = 0;

OverlappedPool g_overlappedPool;

OverlappedAccept *OverlappedPool::Alloc()
{
    OverlappedPool::Slot *pSlot = 
        &OverlappedPool::m_slot[InterlockedIncrement(&OverlappedPool::m_alloc) & 15];
    OverlappedAccept *pNewOverlapped;
    pSlot->m_lock.Enter();
    if((pNewOverlapped = pSlot->m_pOverlapped) != NULL) {
        pSlot->m_pOverlapped = pNewOverlapped->next;
        pSlot->m_lock.Leave();
    } else {
        pSlot->m_lock.Leave();
        pNewOverlapped = new OverlappedAccept;
    }
    memset(&(pNewOverlapped->ol), 0, sizeof(OVERLAPPED));
    pNewOverlapped->sock = INVALID_SOCKET;
    pNewOverlapped->next = NULL;

    return pNewOverlapped;
}

void OverlappedPool::Free(OverlappedAccept *pOverlapped)
{
    OverlappedPool::Slot *pSlot = 
        &OverlappedPool::m_slot[InterlockedIncrement(&OverlappedPool::m_free) & 15];
    pSlot->m_lock.Enter();
    pOverlapped->next = pSlot->m_pOverlapped;
    pSlot->m_pOverlapped = pOverlapped;
    pSlot->m_lock.Leave();
}

void OverlappedPool::FreeAll()
{
    for(int i = 0 ; i < 16; i++) {
        OverlappedPool::Slot *pSlot = &OverlappedPool::m_slot[i];
        OverlappedAccept *pOverlapped;
        pSlot->m_lock.Enter();
        while ((pOverlapped = pSlot->m_pOverlapped) != NULL) {
            pSlot->m_pOverlapped = pOverlapped->next;
            delete pOverlapped;
        }
        pSlot->m_lock.Leave();
    }
}

CIOServerEx::CIOServerEx()
{
    m_hSocket        = INVALID_SOCKET;
    m_acceptEvent    = WSA_INVALID_EVENT;
    m_hAcceptSocket = INVALID_SOCKET;
    g_hSocketTimer    = CreateTimerQueue();
    
    if ( g_hSocketTimer == NULL )
        logger.AddLog(LOG_ERROR, "CIOServerEx Constructor create socket timer fails" );

    InitializeCriticalSectionAndSpinCount(&sockSect, 4000);
}

CIOServerEx::~CIOServerEx()
{
    Close();
    DeleteCriticalSection(&sockSect);
    DeleteTimerQueueEx( g_hSocketTimer, NULL );

    if ( m_acceptEvent )
        WSACloseEvent( m_acceptEvent );
}

void CIOServerEx::OnIOCallback(BOOL bSuccess, DWORD dwTransferred, LPOVERLAPPED lpOverlapped)
{
_BEFORE    
#ifndef _USE_ACCEPTEX

    struct sockaddr_in clientAddress;
    int clientAddressLength = sizeof(clientAddress);
    SOCKET newSocket = accept(m_hSocket, (struct sockaddr*)&clientAddress, &clientAddressLength);
    if (newSocket == INVALID_SOCKET) {
        if (WSAGetLastError() == WSAEWOULDBLOCK) {
            return;
        }
        else {
            if (m_hSocket != INVALID_SOCKET)
                logger.AddLog(LOG_ERROR, "accept error: %d", WSAGetLastError());
            return;
        }
    }
    InterlockedIncrement( &g_Accepts );
    CSocketServerEx *pSocket = CreateSocket(newSocket, &clientAddress);
    if (pSocket == NULL) {
        logger.AddLog(LOG_ERROR, "ServerClose:CreateSocketEx Fail" );
        closesocket(newSocket);
        return;
    }
    pSocket->Initialize( g_hIOCompletionPort );
    logger.AddLog(LOG_DEBUG, "SocketInit" );
#else

    int clientAddressLength = sizeof(sockaddr_in);
    CSocketServerEx *pSocket;
    OverlappedAccept *pAccept = NULL;
    if(!bSuccess) {
        if(lpOverlapped) {
            pAccept = CONTAINING_RECORD(lpOverlapped, OverlappedAccept, ol);
            closesocket(pAccept->sock);
            delete pAccept;
            QueueUserWorkItem(OnAcceptExCallback, (PVOID)this, WT_EXECUTEDEFAULT);
        }
        return;
    }

    QueueUserWorkItem(OnAcceptExCallback, (PVOID)this, WT_EXECUTEDEFAULT);
    struct sockaddr_in *pClientAddress;
    struct sockaddr_in *pServerAddress;
    int serverAddressLength;
    pAccept = CONTAINING_RECORD(lpOverlapped, OverlappedAccept, ol);
    
    GetAcceptExSockaddrs(pAccept->buffer, dwTransferred, 
      sizeof(struct sockaddr_in) + 16, sizeof(struct sockaddr_in) + 16,
      (LPSOCKADDR *)&pServerAddress, &serverAddressLength, 
      (LPSOCKADDR *)&pClientAddress, &clientAddressLength);
    
    pSocket = CreateSocket(pAccept->sock, pClientAddress);

    if(pSocket) {
        pSocket->Initialize( g_hIOCompletionPort );
    } else {
        closesocket(pAccept->sock);
    }
   
    OverlappedPool::Free(pAccept);

#endif
_AFTER_FIN
    return;
}

bool CIOServerEx::WrapAcceptEx( void )
{
    DWORD read;

    OverlappedAccept * pOverlapped = OverlappedPool::Alloc();
    pOverlapped->sock = socket(AF_INET, SOCK_STREAM, 0);
    if(pOverlapped->sock == INVALID_SOCKET) {
        logger.AddLog(LOG_ERROR, "socket error %d", WSAGetLastError());
        goto fail;
    }
    if(!AcceptEx(m_hSocket, pOverlapped->sock, pOverlapped->buffer, 0, 
      sizeof(struct sockaddr_in) + 16, sizeof(struct sockaddr_in) + 16, &read, 
      (LPOVERLAPPED)pOverlapped) && GetLastError() != ERROR_IO_PENDING) {
        logger.AddLog(LOG_ERROR, "AcceptEx error %d", WSAGetLastError());
        goto fail;
    }
    
    return true;

fail:
    if(pOverlapped->sock != INVALID_SOCKET) {
        closesocket(pOverlapped->sock);
    }
    OverlappedPool::Free(pOverlapped);
    return false;
}
DWORD WINAPI CIOServerEx::OnAcceptExCallback(LPVOID pParam)
{
    CIOServerEx *pThis = (CIOServerEx *)pParam;
    pThis->WrapAcceptEx();
    return 0;
}

void CIOServerEx::OnEventCallback()
{
#ifndef _USE_ACCEPTEX
    WSAResetEvent(m_acceptEvent);
    PostQueuedCompletionStatus(g_hIOCompletionPort, 0, (ULONG_PTR)this, NULL);
#else
    WSAResetEvent(m_acceptEvent);
    
    if ( WrapAcceptEx())
    {
        InterlockedIncrement( &g_AcceptExThread );
        logger.AddLog(LOG_NORMAL, "AcceptEx Thread Added %d", g_AcceptExThread );
    }

#endif
    return;
}

BOOL CIOServerEx::Create( int nPort )
{
#ifndef _USE_ACCEPTEX
    m_hSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_hSocket == INVALID_SOCKET) {
        logger.AddLog(LOG_ERROR, "socket error %d", WSAGetLastError());
        return FALSE;
    }

    struct sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    sin.sin_port = htons(nPort);

    if (bind(m_hSocket, (struct sockaddr*)&sin, sizeof(sin))) {
        logger.AddLog(LOG_ERROR, "bind error %d", WSAGetLastError());
        goto fail;
    }
    if (listen(m_hSocket, LISTEN_BACKLOG)) {
        logger.AddLog(LOG_ERROR, "listen error %d", WSAGetLastError());
        goto fail;
    }

    m_acceptEvent = WSACreateEvent();
    WSAEventSelect(m_hSocket, m_acceptEvent, FD_ACCEPT);
    if (!RegisterEvent(m_acceptEvent)) {
        logger.AddLog(LOG_ERROR, "RegisterWait error on port %d", nPort);
        goto fail;
    }
    
#endif

    return TRUE;

#ifndef _USE_ACCEPTEX
fail:
    Close();
    return FALSE;
#endif
}
void CIOServerEx::Stop()
{
    Close();
}
void CIOServerEx::Close()
{
    SOCKET temp_socket;

    if ( m_hSocket != INVALID_SOCKET ) {
        temp_socket = (SOCKET)InterlockedExchangePointer((PVOID volatile*)&m_hSocket, (PVOID)INVALID_SOCKET);
        closesocket(temp_socket);
    }
    
    if(m_acceptEvent != WSA_INVALID_EVENT) {
        WSACloseEvent(m_acceptEvent);
        m_acceptEvent = WSA_INVALID_EVENT;
    }
}

void CIOServerEx::Run( int nPort, SocketExAllocator al )
{
#ifndef _USE_ACCEPTEX
    allocator = al;
    if (Create(nPort))
        logger.AddLog(LOG_NORMAL, "Service ready on port %d", nPort);
#else
    allocator = al;
    int i;
    m_hSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(m_hSocket == INVALID_SOCKET) {
        logger.AddLog(LOG_ERROR, "acceptex socket error %d", WSAGetLastError());
        return;
    }
    struct sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    sin.sin_port = htons(nPort);

    if(bind(m_hSocket, (struct sockaddr*)&sin, sizeof(sin))) {
        logger.AddLog(LOG_ERROR, "bind(port %d) error %d", nPort, WSAGetLastError());
        goto fail;
    }
    if(listen(m_hSocket, LISTEN_BACKLOG)) {
        logger.AddLog(LOG_ERROR, "listen error %d", WSAGetLastError());
        goto fail;
    }
    HANDLE result;
    result = CreateIoCompletionPort((HANDLE)m_hSocket, g_hIOCompletionPort, (ULONG_PTR)this, 0);
    
    if(result == NULL) {
        logger.AddLog(LOG_ERROR, "CreateIoCompletionPort: %d %x %x\n", GetLastError(), m_hSocket, g_hIOCompletionPort);
        goto fail;
    }

    for ( i=0; i < config.AcceptCallNum; i++ ){
        if(!WrapAcceptEx()) {
            goto fail;
        }
    }

    g_AcceptExThread = config.AcceptCallNum;
//    m_acceptEvent = WSACreateEvent();
//    WSAEventSelect(m_hSocket, m_acceptEvent, FD_ACCEPT);
//    if (!RegisterEvent(m_acceptEvent)) {
//        logger.AddLog(LOG_ERROR, "RegisterEvent error on port %d", nPort);
//        goto fail;
//    }


#endif
    logger.AddLog(LOG_NORMAL, "service ready on port %d", nPort);

    return;

//fail:
    Close();
    return;
}

CSocketServerEx *CIOServerEx::CreateSocket(SOCKET s, LPSOCKADDR_IN pAddress)
{
    if ( reporter.m_SocketCount >= config.SocketLimit ){
        return NULL;
    } 

    in_addr cip;
    cip.S_un.S_addr = pAddress->sin_addr.S_un.S_addr;

    if( !IPaccessLimit.SetAccessIP( cip )){
        logger.AddLog(LOG_WARN, "AccessLimit Expire,%d.%d.%d.%d", cip.S_un.S_un_b.s_b1, cip.S_un.S_un_b.s_b2,cip.S_un.S_un_b.s_b3,cip.S_un.S_un_b.s_b4 );
        return NULL;
    }

    CSocketServerEx* mysocket = (*allocator)(s);
    mysocket->SetAddress(pAddress->sin_addr);

    EnterCriticalSection(&sockSect);
    socketmap.insert(SocketExMap::value_type(s, mysocket));
    LeaveCriticalSection(&sockSect);

#ifdef _DEBUG
    logger.AddLog(LOG_NORMAL, "*new connection from %d.%d.%d.%d,0x%x",             
            pAddress->sin_addr.S_un.S_un_b.s_b1,
            pAddress->sin_addr.S_un.S_un_b.s_b2,
            pAddress->sin_addr.S_un.S_un_b.s_b3,
            pAddress->sin_addr.S_un.S_un_b.s_b4, mysocket->GetSocket());
#endif 
    return mysocket;
}

CSocketServerEx *CIOServerEx::FindSocket( SOCKET s )
{
    CSocketServerEx *pSocket = NULL;
    EnterCriticalSection(&sockSect);
    SocketExMap::iterator i = socketmap.find(s);
    if (i != socketmap.end()) {
        pSocket = i->second;
        pSocket->AddRef();
    }
    LeaveCriticalSection(&sockSect);
    return pSocket;
}

bool CIOServerEx::RemoveSocket( SOCKET s )
{
    EnterCriticalSection(&sockSect);
    SocketExMap::iterator i = socketmap.find(s);
    if (i != socketmap.end()) {
        socketmap.erase(i);
    }
    LeaveCriticalSection(&sockSect);

    return true;
}
