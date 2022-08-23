// IPList.cpp: implementation of the CIPList class.
//
//////////////////////////////////////////////////////////////////////

#include "IPList.h"
#include "Config.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CIPAccessLimit IPaccessLimit;

CIPList::CIPList()
{
}

CIPAccessLimit::CIPAccessLimit()
{
    for ( int i=0; i < 16; i++ )
        InitializeCriticalSection( &(lock[i]));
}

CIPAccessLimit::~CIPAccessLimit()
{
    for ( int i=0; i < 16; i++ )
        DeleteCriticalSection( &(lock[i]));
}

int CIPAccessLimit::GetIPLockValue( in_addr ip )
{
    UINT value = ip.S_un.S_un_b.s_b1 + ip.S_un.S_un_b.s_b2 + ip.S_un.S_un_b.s_b3 + ip.S_un.S_un_b.s_b4;
    UINT test = value &0x0000000f;

    if ( test >= 16 )
        test =15;

    return test;
}


bool CIPAccessLimit::SetAccessIP( in_addr ip )
{
    if ( config.IPAccessLimit <= 0 )
        return true;

    bool result = false;
    
    ACCESSMAP::iterator it;
    int index = GetIPLockValue( ip );
    int conn_num = 1;
    EnterCriticalSection( &(lock[index]));
    it = accessmap[index].find( ip.S_un.S_addr );
    if ( it != accessmap[index].end()){
        if ( (it->second) < config.IPAccessLimit ) {
            (it->second)++;
            result = true;
        }
    }else{
        std::pair< ACCESSMAP::iterator, bool> r = accessmap[index].insert( ACCESSMAP::value_type( ip.S_un.S_addr, conn_num ));
        result = true;
    }
    LeaveCriticalSection( &(lock[index]));
    
    return result;
}

bool CIPAccessLimit::DelAccessIP( in_addr ip )
{
    if ( config.IPAccessLimit <= 0 )
        return true;

    bool result = false;
    ACCESSMAP::iterator it;
    int index = GetIPLockValue( ip );
    EnterCriticalSection( &(lock[index]));
    it = accessmap[index].find( ip.S_un.S_addr );
    if ( it != accessmap[index].end()){
        if ( (it->second) > 1 ){
            (it->second)--;
        } else {
            accessmap[index].erase(it);
        }
        result = true;
    }
    LeaveCriticalSection( &(lock[index]));

    return result;
}

CIPList forbiddenIPList;
CIPList allowedIPList;


BOOL CIPList::Load(const char *fileName)
{
    std::ifstream inp(fileName);
    int line = 0;
    if (inp.fail())
        return FALSE;
    char buf[1024];
    lock.WriteLock();
    table.clear();
    for ( ; ; ) {
        inp.getline(buf, sizeof(buf));
        line++;
        if (inp.fail())
            break;
        unsigned c1, c2, c3, c4;
        IPRecord record;
        char *ptr = buf;
        c1 = strtol(ptr, &ptr, 10);
        if (*ptr != '.')
            continue;    // ignore
        ptr++;
        c2 = strtol(ptr, &ptr, 10);
        if (*ptr != '.')
            continue;    // ignore
        ptr++;
        c3 = strtol(ptr, &ptr, 10);
        if (*ptr != '.')
            continue;    // ignore
        ptr++;
        c4 = strtol(ptr, &ptr, 10);
        record.begin = (c1 << 24) | (c2 << 16) | c3 << 8 | c4;
        ptr += strspn(ptr, " \t");
        if (*ptr == '-') {
            ptr++;
            ptr += strspn(ptr, " \t");
            c1 = strtol(ptr, &ptr, 10);
            if (*ptr != '.')
                continue;    // ignore
            ptr++;
            c2 = strtol(ptr, &ptr, 10);
            if (*ptr != '.')
                continue;    // ignore
            ptr++;
            c3 = strtol(ptr, &ptr, 10);
            if (*ptr != '.')
                continue;    // ignore
            ptr++;
            c4 = strtol(ptr, &ptr, 10);
            record.end = ((c1 << 24) | (c2 << 16) | c3 << 8 | c4) + 1;
        }
        else {
            record.end = (c4 == 0) ? (c3 == 0) ? record.begin + 0x10000 : 
                record.begin + 0x100 : record.begin  + 1;
        }

        if (record.begin >= record.end) {
            logger.AddLog(LOG_ERROR, "IPList: start address address is greater than end address at line %d", line);
            continue;
        }

        std::vector<IPRecord>::iterator it;
        it = std::upper_bound(table.begin(), table.end(), record.begin-1);
        if (it != table.end() && it->begin <= record.begin-1) {
            if (it->end != record.begin)
                logger.AddLog(LOG_ERROR, "IPList: address is merged with %d.%d.%d.%d-%d.%d.%d.%d at line %d",
                    (it->begin>>24)&255, (it->begin>>16)&255, (it->begin>>8)&255, it->begin&255,
                    ((it->end-1)>>24)&255, ((it->end-1)>>16)&255, ((it->end-1)>>8)&255, (it->end-1)&255,
                    line);
            if (it->end >= record.end)
                continue;
            record.begin = it->begin;
            it = table.erase(it);
        }
        while (it != table.end() && it->begin <= record.end) {
            if (it->begin != record.end)
                logger.AddLog(LOG_ERROR, "IPList: address is merged with %d.%d.%d.%d-%d.%d.%d.%d at line %d",
                    (it->begin>>24)&255, (it->begin>>16)&255, (it->begin>>8)&255, it->begin&255,
                    ((it->end-1)>>24)&255, ((it->end-1)>>16)&255, ((it->end-1)>>8)&255, (it->end-1)&255,
                    line);
            if (record.end < it->end)
                record.end = it->end;
            it = table.erase(it);
        }
        table.insert(it, record);        
    }
    lock.WriteUnlock();
    logger.AddLog(LOG_NORMAL, "IPList loaded from %s", fileName);
    return TRUE;
}

BOOL CIPList::IpExists(in_addr ipAddr)
{
    lock.ReadLock();
    unsigned addr = (ipAddr.S_un.S_un_b.s_b1 << 24) | (ipAddr.S_un.S_un_b.s_b2 << 16)
        | (ipAddr.S_un.S_un_b.s_b3 << 8) | ipAddr.S_un.S_un_b.s_b4;
    std::vector<IPRecord>::iterator it = std::upper_bound(table.begin(), table.end(), addr);
    BOOL b = it != table.end() && it->begin <= addr;
    lock.ReadUnlock();
    return b;
}
