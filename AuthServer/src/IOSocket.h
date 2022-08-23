// IOSocket.h: interface for the CIOSocket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IOSOCKET_H__5C32C777_24A9_4987_B86E_8F185F221438__INCLUDED_)
#define AFX_IOSOCKET_H__5C32C777_24A9_4987_B86E_8F185F221438__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IOBuffer.h"
#include "Lock.h"
#include "IOObject.h"
#include "Blowfish.h"

#define INBUFSIZE   32768
#define    OUTBUFSIZE  16384


typedef std::set< int > UIDSET;

class CServerKickList {

public:
    CServerKickList() {
    };
    ~CServerKickList() {
    };
    bool AddKickUser( int uid );
    bool PopKickUser();

protected:
    UIDSET KickList;
};

enum SocketMode {
    SM_READ_LEN,
    SM_READ_BODY,
    SM_CLOSE
};

enum CloseReason {
    CR_NORMAL,
    CR_ABNORMAL,
    CR_BROKEN_PIPE,
    CR_NETNAME,
    CR_CLOSED,
    CR_MAX
};

class CSocketServer;
class CSocketInt;
class CSocketServerEx;

typedef bool (*ServerPacketFunc)(CSocketServer* , const unsigned char* packet);
typedef void (*InteractivePacketFunc)(CSocketInt* , const char* packet);
typedef bool (*ServerPacketFuncEx)(CSocketServerEx* , const unsigned char* packet);
typedef void (*EncPacketType)(unsigned char *buf, __int64 &key, int &length);
typedef bool (*DecPacketType)(unsigned char *buf, __int64 &key, int length);

class CIOSocket 
: public CIOObject
{
protected:
    CRITICAL_SECTION m_cs;

    OVERLAPPED m_overlappedRead;
    OVERLAPPED m_overlappedWrite;

    CIOBuffer    *m_pReadBuf;
    CIOBuffer    *m_pFirstBuf;
    CIOBuffer    *m_pLastBuf;

    long    m_nPendingWrite;

    SOCKET m_hSocket;

public:
    CIOSocket(SOCKET s);
    virtual ~CIOSocket();
    virtual void OnClose(SOCKET closedSocket);
    virtual void OnCreate( void );
    virtual void OnRead( void )=0;
    virtual void OnIOCallback( BOOL bSuccess, DWORD dwTransferred, LPOVERLAPPED lpOverlapped );
    virtual void OnReadCallback( DWORD dwTransferred );
    virtual void OnWriteCallback( DWORD dwTransferred );

    void CloseSocket( void );
    void Initialize( HANDLE hIOCompletionPort );
    void Read(DWORD size);
    long PendingWrite() { return m_nPendingWrite; }
    void Write(CIOBuffer *pBuffer);
    void Write(char *buf, DWORD size);

    SOCKET GetSocket() const { return m_hSocket; }
};

class CSocketServer 
: public CIOSocket
{
public:
    CSocketServer(SOCKET sock);
    virtual ~CSocketServer();
    static CSocketServer* Allocate(SOCKET s);
    virtual void OnCreate();
    virtual void OnRead();
    virtual void OnClose(SOCKET closedSocket);
    friend class CPacketServer;
    
    in_addr getaddr( void ) { return addr; };
    void SetAddress( in_addr in_Addr );
    void Send(const char* format, ...);
    const char * IP();

    ServerId serverid;
    char GameCode;
    int opFlag;
    bool bSetActiveServer;

protected:
    int packetLen;

    SocketMode mode;
    ServerPacketFunc *packetTable;
    in_addr addr;
    char* host;

    // KICK USER LIST

};

enum IntSocketMode {
    ISM_READ_LEN,
    ISM_READ_BODY,
    ISM_CLOSE
};

class CSocketInt
: public CIOSocket
{
public:
    CSocketInt(SOCKET sock);
    virtual ~CSocketInt();
    static CSocketInt* Allocate(SOCKET s);
    virtual void OnCreate();
    virtual void OnRead();
    virtual void OnClose(SOCKET closedSocket);
    friend class CPacketServer;
    
    void SetAddress( in_addr in_Addr );
    void Send(const char* format, ...);
    const char * IP();
    void Process( char *);
    void SendBuffer(const char* buf, int len);

protected:
    int packetLen;
    IntSocketMode mode;
    InteractivePacketFunc *packetTable;
    in_addr addr;
    char* host;
    int    m_nTimeout;
};

class CSocketServerEx
: public CIOSocket
{
public:
    CSocketServerEx(SOCKET socket);
    virtual ~CSocketServerEx();
    static CSocketServerEx* Allocate(SOCKET s);
    virtual void OnCreate();
    virtual void OnRead();
    virtual void OnClose(SOCKET closedSocket);
    virtual void OnTimerCallback(void);    
    
    friend class CPacketServerEx;
    
    void SetAddress( in_addr in_Addr );
    void Send(const char* format, ...);
    void NonEncSend( const char* fromat, ... );
    void Send(const char* sendmsg, int msglen );
    int GetMd5Key(){ return oneTimeKey;}
    const char * IP();
    in_addr getaddr( void ) { return addr; };
    UserMode um_mode;
    int uid;
    __int64 EncOneTimeKey;
    __int64 DecOneTimeKey;

    int m_lastIO;
    EncPacketType EncPacket;
    DecPacketType DecPacket;

protected:
    
    int  packetLen;
    char serverlist;

    HANDLE m_TimerHandle;
    SocketMode mode;
    ServerPacketFuncEx *packetTable;
    in_addr addr;
    char* host;
    int oneTimeKey;
    bool EncryptFlag;
    Blowfish_matrix blf_mtx;
};


#endif // !defined(AFX_IOSOCKET_H__5C32C777_24A9_4987_B86E_8F185F221438__INCLUDED_)
