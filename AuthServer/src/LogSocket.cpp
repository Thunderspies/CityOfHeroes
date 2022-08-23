// LogSocket.cpp: implementation of the CLogSocket class.
//
//////////////////////////////////////////////////////////////////////

#include "LogSocket.h"
#include "Config.h"
#include "Log.h"
#include "util.h"
#include "Lock.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CRWLock gLogLock;

bool LogDReconnect = false;
bool g_opFlag = false;
HANDLE g_hLogDTimer = NULL;

LONG CLogPacketServer::g_nPendingPacket;
extern bool g_bTerminating;


VOID CALLBACK LOGDTimerRoutine(PVOID lpParam, BYTE TimerOrWaitFired)
{
    if ( g_hLogDTimer )
        DeleteTimerQueueTimer( NULL, g_hLogDTimer, NULL );
    g_hLogDTimer = NULL;

    if ( LogDReconnect == true ) {
        SOCKET LOGSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        sockaddr_in Destination;
        Destination.sin_family = AF_INET;
        Destination.sin_addr   = config.LogDIP;
        Destination.sin_port   = htons( (u_short)config.LogDPort );

        int ErrorCode = connect( LOGSock, ( sockaddr *)&Destination, sizeof( sockaddr ));
        CLogSocket *tempLogSocket = CLogSocket::Allocate(LOGSock);
        tempLogSocket->SetAddress( config.LogDIP );
        if ( ErrorCode == SOCKET_ERROR ){
            tempLogSocket->CloseSocket();
            tempLogSocket->ReleaseRef();
        } else {
            gLogLock.WriteLock();
            CLogSocket *tmpLogSocket = pLogSocket;
            pLogSocket = tempLogSocket;
            LogDReconnect = false;
            config.UseLogD = true;
            pLogSocket->Initialize( g_hIOCompletionPortInt );
            tmpLogSocket->ReleaseRef();
            gLogLock.WriteUnlock();
        }    
    } 
}
class CLOGPacketServerPool
{
public:
    class CSlot
    {
    public:
        CLogPacketServer*   m_pPacket;
        CLock m_lock;
        CSlot() : m_pPacket(NULL),m_lock(eCustomSpinLock) {}
    };
    static CSlot g_slot[16];
    static long    g_nAlloc;
    static long    g_nFree;
    ~CLOGPacketServerPool() { CLogPacketServer::FreeAll(); }
};

CLOGPacketServerPool::CSlot    CLOGPacketServerPool::g_slot[16];
long    CLOGPacketServerPool::g_nAlloc = -1;
long    CLOGPacketServerPool::g_nFree = 0;

CLOGPacketServerPool theLOGPacketPool;

CLogPacketServer * CLogPacketServer::Alloc()
{
    CLogPacketServer *newPacket;

    CLOGPacketServerPool::CSlot *pSlot =
        &CLOGPacketServerPool::g_slot[InterlockedIncrement(&CLOGPacketServerPool::g_nAlloc) & 15];
    pSlot->m_lock.Enter();
    if ((newPacket = pSlot->m_pPacket) != NULL) {
        pSlot->m_pPacket = reinterpret_cast<CLogPacketServer *> (newPacket->m_pSocket);
        pSlot->m_lock.Leave();
    }
    else {
        pSlot->m_lock.Leave();
        newPacket = new CLogPacketServer;
    }

    return newPacket;
}

void CLogPacketServer::FreeAll()
{
    for (int i = 0 ; i < 16; i++) {
        CLOGPacketServerPool::CSlot *pSlot = &CLOGPacketServerPool::g_slot[i];
        pSlot->m_lock.Enter();
        CLogPacketServer *pPacket;
        while ((pPacket = pSlot->m_pPacket) != NULL) {
            pSlot->m_pPacket = reinterpret_cast<CLogPacketServer *> (pPacket->m_pSocket);
            delete pPacket;
        }
        pSlot->m_lock.Leave();
    }
}

void CLogPacketServer::Free()
{
    CLOGPacketServerPool::CSlot *pSlot =
        &CLOGPacketServerPool::g_slot[InterlockedDecrement(&CLOGPacketServerPool::g_nFree) & 15];
    pSlot->m_lock.Enter();
    m_pSocket = reinterpret_cast<CIOSocket *>(pSlot->m_pPacket);
    pSlot->m_pPacket = this;
    pSlot->m_lock.Leave();
}

void CLogPacketServer::OnIOCallback(BOOL bSuccess, DWORD dwTransferred, LPOVERLAPPED lpOverlapped)
{
_BEFORE
    unsigned char *packet = (unsigned char *) m_pBuf->m_buffer + dwTransferred;

    if ((*m_pFunc)(m_pSocket, packet + 1)) {
        m_pSocket->CIOSocket::CloseSocket();
    }

    m_pSocket->ReleaseRef();
    m_pBuf->Release();
    InterlockedDecrement(&g_nPendingPacket);
    Free();
_AFTER_FIN
    return;
}

static bool LOGDummyPacket( CLogSocket *s, const unsigned char *packet )
{
    logger.AddLog(LOG_WARN, "Call DummyPacket What What What" );
    return false;
}

static LOGPacketFunc LOGPacketFuncTable[] = {
    LOGDummyPacket,
};

CLogSocket::~CLogSocket()
{
    AS_LOG_VERBOSE( "DELETE LogSocket 0x%x", this );
}

CLogSocket::CLogSocket( SOCKET sock )
: CIOSocket( sock )
{
    addr = config.LogDIP;
    host = 0;
    mode = SM_READ_LEN;
    LogDAddress.sin_family = AF_INET;
    LogDAddress.sin_addr   = config.LogDIP;
    LogDAddress.sin_port   = htons( (u_short)config.LogDPort );
    packetTable =LOGPacketFuncTable;
}

CLogSocket* CLogSocket::Allocate(SOCKET s)
{
    return new CLogSocket( s );
}

void CLogSocket::OnRead( void ) {
    int pi = 0;
    int ri = m_pReadBuf->m_size;
    int errorNum = 0;
    unsigned char *inBuf = (unsigned char *)m_pReadBuf->m_buffer;
    if (mode == SM_CLOSE) {
        CloseSocket();
        return;
    }

    for  ( ; ; ) {
        if (pi >= ri) {
            pi = 0;
            Read(0);
            return;
        }
        if (mode == SM_READ_LEN) {
            if (pi + 3 <= ri) {
                packetLen = inBuf[pi] + (inBuf[pi + 1] << 8) + 1 - config.PacketSizeType;
                if (packetLen <= 0 || packetLen > BUFFER_SIZE) {
                    errorNum = 1;
                    logger.AddLog(LOG_ERROR, "%d: bad packet size %d", m_hSocket, packetLen);
                    break;
                } else {
                    pi += 2;
                    mode = SM_READ_BODY;
                }
            } else {
                Read(ri - pi);
                return;
            }
        } else if (mode == SM_READ_BODY) {
            if (pi + packetLen <= ri) {

                if (inBuf[pi] >= 0) {
                    logger.AddLog(LOG_ERROR, "unknown protocol %d", inBuf[pi]);
                    errorNum = 2;
                    break;
                } else {
                    CLogPacketServer *pPacket = CLogPacketServer::Alloc();
                    pPacket->m_pSocket = this;
                    pPacket->m_pBuf = m_pReadBuf;
                    pPacket->m_pFunc = (CLogPacketServer::LOGPacketFunc) packetTable[inBuf[pi]];
                    CIOSocket::AddRef();
                    m_pReadBuf->AddRef();
                    InterlockedIncrement(&CLogPacketServer::g_nPendingPacket);
                    pPacket->PostObject(pi, g_hIOCompletionPortInt);
                    pi += packetLen;
                    mode = SM_READ_LEN;
                }
            } else {
                Read(ri - pi);
                return;
            }
        }
        else{
            errorNum = 3;
            break;
        }
    }
    

    errlog.AddLog(LOG_NORMAL, "logd server connection close. invalid status and protocol %s, errorNum :%d", IP(), errorNum);
    CIOSocket::CloseSocket();
}

void CLogSocket::OnCreate()
{
_BEFORE
    Send2("cd", RQ_SET_CHECK_STATUS, 0 );
    Send2("cdd", RQ_SERVER_STARTED, LOG_AUTH_WORLDID, SERVER_TYPE);
    g_opFlag = 1;
    OnRead();
_AFTER_FIN
}

void CLogSocket::Send(const char* format, ...)
{
_BEFORE
    AddRef();
    if ( mode == SM_CLOSE || LogDReconnect == true || m_hSocket == INVALID_SOCKET || g_opFlag == 0)
    {
        AS_LOG_VERBOSE( "logdsocket Call invalid" );
        ReleaseRef();
        return ;
    }

    CIOBuffer *pBuffer = CIOBuffer::Alloc();
    char *buffer = pBuffer->m_buffer;
    va_list ap;
    va_start(ap, format);
    int len = Assemble(buffer + 2, BUFFER_SIZE - 2, format, ap);
    va_end(ap);
    if (len == 0) {
        logger.AddLog(LOG_ERROR, "%d: assemble too large packet. format %s", m_hSocket, format);
    } else {
        len -= 1;
        len = len + 3;
        //len = len + config.PacketSizeType;
        buffer[0] = len;
        buffer[1] = len >> 8;

    }
    pBuffer->m_size = len;
    Write(pBuffer);
    ReleaseRef();
_AFTER_FIN
}

void CLogSocket::Send2(const char* format, ...)
{
_BEFORE
    AddRef();
    if ( mode == SM_CLOSE || LogDReconnect == true || m_hSocket == INVALID_SOCKET)
    {
        AS_LOG_VERBOSE( "logdsocket Call invalid send2" );
        ReleaseRef();
        return ;
    }

    CIOBuffer *pBuffer = CIOBuffer::Alloc();
    char *buffer = pBuffer->m_buffer;
    va_list ap;
    va_start(ap, format);
    int len = Assemble(buffer + 2, BUFFER_SIZE - 2, format, ap);
    va_end(ap);
    if (len == 0) {
        logger.AddLog(LOG_ERROR, "%d: assemble too large packet. format %s", m_hSocket, format);
    } else {
        len -= 1;
        len = len + 3;
        // len= len + config.PacketSizeType
        buffer[0] = len;
        buffer[1] = len >> 8;

    }
    pBuffer->m_size = len+3 - 3;
    //pBuffer->m_size = len+3 - config.PacketSizeType;
    Write(pBuffer);
    //AS_LOG_VERBOSE( "pending:%d byte", pLogSocket->PendingWrite() );
    ReleaseRef();
_AFTER_FIN
}

const char *CLogSocket::IP()
{
    return inet_ntoa(addr);
}

void CLogSocket::OnTimerCallback( void )
{
/*
    logger.AddLog(LOG_WARN, "Timer Callback Reconnect LOGDTimer Timer Called" );
_BEFORE
    if ( g_bTerminating )
    {
        ReleaseRef();
        return ;
    }

    EnterCriticalSection( &m_cs );
    if ( !Reconnect ){
        LeaveCriticalSection( &m_cs );
        return ;
    }
    if ( m_hSocket != INVALID_SOCKET ){
        closesocket( m_hSocket );
        m_hSocket = INVALID_SOCKET;
    }
    m_hSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_hSocket == INVALID_SOCKET) {
        logger.AddLog(LOG_ERROR, "log socket error %d", WSAGetLastError());
        RegisterTimer( config.LogDReconnectInterval, true );
        LeaveCriticalSection( &m_cs );
        ReleaseRef();
        return ;
    }

    int ErrorCode = connect( m_hSocket, ( sockaddr *)(&LogDAddress), sizeof( sockaddr ));
    
    if ( ErrorCode == SOCKET_ERROR ){
        closesocket( m_hSocket );
        m_hSocket = INVALID_SOCKET;
        RegisterTimer( config.LogDReconnectInterval, true );
        LeaveCriticalSection( &m_cs );
        ReleaseRef();
        return ;
    } else {
        Reconnect = false;
        config.UseLogD = true;
        LeaveCriticalSection( &m_cs );
        mode = SM_READ_LEN;
        Initialize( g_hIOCompletionPortInt );
        ReleaseRef();
        logger.AddLog(LOG_WARN, "LOGD Socket Connected" );
        return ;
    }
_AFTER_FIN
*/
}

void CLogSocket::OnClose(SOCKET closedSocket)
{
    // Must not use the GetSocket() function from within
     // this function!  Instead, use the socket argument
     // passed into the function.

    LogDReconnect = true;
    config.UseLogD = false;
    mode = SM_CLOSE;
    time_t ActionTime;
    struct tm ActionTm;

    ActionTime = time(0);
    ActionTm = *localtime(&ActionTime);

    logger.AddLog(LOG_ERROR, "*close logd connection from %s, %x(%x)", IP(), closedSocket, this);
    errlog.AddLog(LOG_NORMAL, "%d-%d-%d %d:%d:%d,main server connection close from %s, 0x%x\r\n",        
                ActionTm.tm_year + 1900, ActionTm.tm_mon + 1, ActionTm.tm_mday,
                ActionTm.tm_hour, ActionTm.tm_min, ActionTm.tm_sec, IP(), closedSocket );    
    
/*
    if ( !g_bTerminating ) {
        EnterCriticalSection( &m_cs );
        RegisterTimer( config.LogDReconnectInterval, true );
        if ( !Reconnect ){
            Reconnect = true;
            config.UseLogD = false;
            opFlag = 0;
        }
        LeaveCriticalSection( &m_cs );
    }
*/
    AddRef();
    CreateTimerQueueTimer( &g_hLogDTimer, NULL, LOGDTimerRoutine, this, config.LogDReconnectInterval, 0, 0 );
}