#ifndef _PACKET_H
#define _PACKET_H

#include "GlobalAuth.h"
#include "IOObject.h"
#include "IOSocket.h"

class CPacketServer : public CIOObject
{
public:
    typedef bool (*ServerPacketFunc)(CIOSocket* , const unsigned char* packet);

    CIOSocket *m_pSocket;
    CIOBuffer   *m_pBuf;
    ServerPacketFunc    m_pFunc;
    static LONG g_nPendingPacket;
    static CPacketServer* Alloc();
    static void FreeAll();
    void Free();
    virtual void OnIOCallback(BOOL bSuccess, DWORD dwTransferred, LPOVERLAPPED lpOverlapped);
};


class CPacketServerEx : public CIOObject
{
public:
    typedef bool (*ServerPacketFuncEx)(CSocketServerEx* , const unsigned char* packet, size_t length);
    size_t m_packetLength; // Immutable body length, excluding opcode

    CIOSocket *m_pSocket;
    CIOBuffer   *m_pBuf;
    ServerPacketFuncEx    m_pFunc;
    static LONG g_nPendingPacket;
    static CPacketServerEx* Alloc();
    static void FreeAll();
    void Free();
    virtual void OnIOCallback(BOOL bSuccess, DWORD dwTransferred, LPOVERLAPPED lpOverlapped);
};

#endif