#include "Packet.h"

LONG CPacketServerEx::g_nPendingPacket;

class CPacketServerPoolEx
{
public:
    class CSlot
    {
    public:
        CPacketServerEx *   m_pPacket;
        CLock m_lock;
        CSlot() : m_pPacket(NULL),m_lock(eCustomSpinLock) {}
    };
    static CSlot g_slot[16];
    static long    g_nAlloc;
    static long    g_nFree;
    ~CPacketServerPoolEx() { CPacketServerEx::FreeAll(); }
};

CPacketServerPoolEx::CSlot    CPacketServerPoolEx::g_slot[16];
long    CPacketServerPoolEx::g_nAlloc = -1;
long    CPacketServerPoolEx::g_nFree = 0;

CPacketServerPoolEx thePacketPoolEx;

CPacketServerEx * CPacketServerEx::Alloc()
{
    CPacketServerEx *newPacket;
    CPacketServerPoolEx::CSlot *pSlot =
        &CPacketServerPoolEx::g_slot[InterlockedIncrement(&CPacketServerPoolEx::g_nAlloc) & 15];
    pSlot->m_lock.Enter();
    if ((newPacket = pSlot->m_pPacket) != NULL) {
        pSlot->m_pPacket = reinterpret_cast<CPacketServerEx *> (newPacket->m_pSocket);
        newPacket->m_pSocket = NULL;
        pSlot->m_lock.Leave();
    }
    else {
        pSlot->m_lock.Leave();
        newPacket = new CPacketServerEx;
    }
    return newPacket;
}

void CPacketServerEx::Free()
{
    CPacketServerPoolEx::CSlot *pSlot =
        &CPacketServerPoolEx::g_slot[InterlockedDecrement(&CPacketServerPoolEx::g_nFree) & 15];
    pSlot->m_lock.Enter();
    m_pSocket = reinterpret_cast<CIOSocket *>(pSlot->m_pPacket);
    pSlot->m_pPacket = this;
    pSlot->m_lock.Leave();
}

void CPacketServerEx::FreeAll()
{
    for (int i = 0 ; i < 16; i++) {
        CPacketServerPoolEx::CSlot *pSlot = &CPacketServerPoolEx::g_slot[i];
        pSlot->m_lock.Enter();
        CPacketServerEx *pPacket;
        while ((pPacket = pSlot->m_pPacket) != NULL) {
            pSlot->m_pPacket = reinterpret_cast<CPacketServerEx *> (pPacket->m_pSocket);
            pPacket->ReleaseRef();
        }
        pSlot->m_lock.Leave();
    }
}

void CPacketServerEx::OnIOCallback(BOOL bSuccess, DWORD dwTransferred, LPOVERLAPPED lpOverlapped)
{
    unsigned char *packet = (unsigned char *) m_pBuf->m_buffer + dwTransferred;

    if ((*m_pFunc)(m_pSocket, packet + 1)) {
        m_pSocket->CIOSocket::CloseSocket();
    }

    m_pSocket->ReleaseRef();
    m_pBuf->Release();
    InterlockedDecrement(&g_nPendingPacket);
    Free();
    return;
}
