// ServerList.cpp: implementation of the CServerList class.
//
//////////////////////////////////////////////////////////////////////
#include "ServerList.h"
#include "DBConn.h"
#include "Config.h"
#include "util.h"
#include "IOSocket.h"

CServerList g_ServerList;

static void CALLBACK UpdateDBHelper(PVOID /* param */, BYTE /* TimerOrWaitFired */)
{
    g_ServerList.UpdateDB();
}

CServerList::CServerList() :
    m_serverList(),
    m_timer(),
    mylock()
{
    CreateTimerQueueTimer( &m_timer, NULL, UpdateDBHelper, NULL , 300000, 300000, 0);    
}

CServerList::~CServerList()
{
    DeleteTimerQueueTimer( NULL, m_timer, NULL );
}

void CServerList::Load()
{
    int port = config.worldPort;
    
    CDBConn conn(g_linDB);

    WorldServer worldserver;
    unsigned int localServerId;
    std::map<ServerId, WorldServer> newServerList;

    conn.Bind( &localServerId );
    conn.Bind( worldserver.name, 26 );
    conn.Bind( worldserver.ip, 256 );
    conn.Bind( worldserver.inner_ip, 256 );
    conn.Bind( &worldserver.ageLimit );
    conn.Bind( &worldserver.pkflag );
    conn.Bind( &worldserver.region_id );
    if (config.dbHasClientPorts) { conn.Bind(&worldserver.outer_port); }

    char command[255];
    if (config.dbHasClientPorts) {
        strncpy(command, "Select id, name, ip, inner_ip, ageLimit, pk_flag, server_group_id, client_port From server Order by id", sizeof(command));
    }
    else {
        strncpy(command, "Select id, name, ip, inner_ip, ageLimit, pk_flag, server_group_id From server Order by id", sizeof(command));
    }

    if ( conn.Execute(command) )
     {
        bool nodata;
        conn.Fetch( &nodata );
        while( !nodata )
        {
            worldserver.serverid.SetValue(localServerId);
            worldserver.inner_addr.S_un.S_addr = *(ULONG*)gethostbyname(worldserver.inner_ip)->h_addr_list[0];
            worldserver.outer_addr.S_un.S_addr = *(ULONG*)gethostbyname(worldserver.ip)->h_addr_list[0];
            if ( !config.dbHasClientPorts || worldserver.outer_port <= 0 ) { worldserver.outer_port=config.worldPort; }
            worldserver.s = NULL;
            worldserver.UserNum = 0;
            worldserver.maxUsers = 0;
            worldserver.status = 0;

            newServerList.insert(std::make_pair(worldserver.serverid, worldserver ));
            
            logger.AddLog(LOG_NORMAL, "Server %d loaded, innerip:%s,outerip:%s,port:%d,agelimit:%d,pk:%d",  
                static_cast<int>(worldserver.serverid.GetValueChar()),
                worldserver.inner_ip,
                worldserver.ip,
                worldserver.outer_port,
                worldserver.ageLimit, 
                worldserver.pkflag);

            conn.Fetch( &nodata );
        }

        mylock.WriteLock();
        m_serverList.swap(newServerList);
        mylock.WriteUnlock();
    }
}

void CServerList::SetServerStatus(ServerId id, ServerStatus status)
{
    mylock.WriteLock();
    ServerListType::iterator i=m_serverList.find(id);
    if (i!=m_serverList.end())
    {
        i->second.status = status;
    }
    mylock.WriteUnlock();
}

void CServerList::SetServerVIPStatus(ServerId id, int isVIP)
{
    mylock.WriteLock();
    ServerListType::iterator i=m_serverList.find(id);
    if (i!=m_serverList.end())
    {
        i->second.isVIP = isVIP;
    }
    mylock.WriteUnlock();
}

ServerId CServerList::SetServerSocketByAddress(in_addr address, CSocketServer *socket)
{
    ServerId id;

    mylock.WriteLock();
    for (ServerListType::iterator i=m_serverList.begin(); i!= m_serverList.end(); ++i)
    {
        if (i->second.inner_addr.s_addr == address.s_addr)
        {
            if (i->second.s)
            {
                // Two connections from the same server.  Drop the old connection
                logger.AddLog(LOG_ERROR, "Got second connection from Server %d.  Dropping first connection.", static_cast<int>(id.GetValueChar()));
                CSocketServer *oldSocket = i->second.s;
                i->second.s=NULL;

                mylock.WriteUnlock(); //CloseSocket() calls back into CServerList::RemoveServer, so the lock must be released first
                oldSocket->CloseSocket();
                mylock.WriteLock();
            }
            i->second.s = socket;
            id = i->first;
        }
    }

    if (config.allowUnknownServers && !id.IsValid())
    {
        id = GetFreeServerId();

        WorldServer newServer;
        _snprintf(newServer.name,sizeof(newServer.name),"Server %d",static_cast<int>(id.GetValueChar()));
        newServer.s = socket;
        newServer.inner_addr=address;
        newServer.outer_addr=address;
        newServer.outer_port=config.worldPort;
        newServer.ageLimit = 0;
        newServer.UserNum = 0;
        newServer.maxUsers = 0;
        newServer.pkflag = 0;
        
        m_serverList.insert(std::make_pair(id,newServer));

        logger.AddLog(LOG_NORMAL, "Added Server %d which was not in the database (allowUnkownServers is enabled)",
                static_cast<int>(id.GetValueChar()));
    }
    mylock.WriteUnlock();

    return id;
}

void CServerList::RemoveSocket(CSocketServer *socket)
{
    mylock.WriteLock();
    for (ServerListType::iterator i=m_serverList.begin(); i!= m_serverList.end(); ++i)
    {
        if (i->second.s == socket)
        {
            i->second.status = 0;
            i->second.s = NULL;
        }
    }
    mylock.WriteUnlock();
}

bool CServerList::SetServerSocketById(ServerId id, CSocketServer * socket, in_addr actualAddress, short int actualPort)
{
    bool found = false;

    mylock.WriteLock();
    ServerListType::iterator i=m_serverList.find(id);
    if (i!=m_serverList.end())
    {
        if (i->second.s)
        {
            // Two connections from the same server.  Drop the old connection
            logger.AddLog(LOG_ERROR, "Got second connection from Server %d.  Dropping first connection.", static_cast<int>(id.GetValueChar()));
            CSocketServer *oldSocket = i->second.s;
            i->second.s=NULL;

            mylock.WriteUnlock();
            oldSocket->CloseSocket();
            mylock.WriteLock();
        }
        i->second.s = socket;
        i->second.inner_addr=actualAddress;
        i->second.outer_addr=actualAddress;
        i->second.outer_port=actualPort;
        found = true;
    }

    if (config.allowUnknownServers && !found)
    {
        WorldServer newServer;
        _snprintf(newServer.name,sizeof(newServer.name),"Server %d",static_cast<int>(id.GetValueChar()));
        newServer.s = socket;
        newServer.inner_addr=actualAddress;
        newServer.outer_addr=actualAddress;
        newServer.outer_port=actualPort;
        newServer.serverid=id;
        newServer.ageLimit = 0;
        newServer.UserNum = 0;
        newServer.maxUsers = 0;
        newServer.pkflag = 0;
        newServer.region_id = 0;
        
        m_serverList.insert(std::make_pair(id,newServer));

        logger.AddLog(LOG_NORMAL, "Added Server %d which was not in the database (allowUnkownServers is enabled)",
                static_cast<int>(id.GetValueChar()));

        found = true;
    }

    mylock.WriteUnlock();

    return found;
}

in_addr CServerList::GetInternalAddress(ServerId id) const
{
    in_addr result;
    result.S_un.S_addr=0;

    mylock.ReadLock();
    ServerListType::const_iterator i=m_serverList.find(id);
    if (i!=m_serverList.end())
    {
        result = i->second.inner_addr;
    }
    mylock.ReadUnlock();

    return result;
}

bool CServerList::IsServerUp(ServerId id) const
{
    bool result=false;

    mylock.ReadLock();
    ServerListType::const_iterator i=m_serverList.find(id);
    if (i!=m_serverList.end())
    {
        result = i->second.status != 0; 
    }
    mylock.ReadUnlock();

    return result;
}

bool CServerList::IsServerVIPonly(ServerId id) const
{
    bool result=false;

    mylock.ReadLock();
    ServerListType::const_iterator i=m_serverList.find(id);
    if (i!=m_serverList.end())
    {
        result = !!i->second.isVIP; 
    }
    mylock.ReadUnlock();

    return result;
}
void CServerList::RequestUserCounts() const
{
    mylock.ReadLock();

    for(ServerListType::const_iterator i=m_serverList.begin(); i!=m_serverList.end(); ++i)
    {
        SendSocket( i->second.inner_addr, "c", SQ_SERVER_NUM);
    }

    mylock.ReadUnlock();
}

void CServerList::SetServerUserCount( ServerId id, short userCount, short userLimit)
{
    mylock.WriteLock();

    ServerListType::iterator i=m_serverList.find(id);
    if (i!=m_serverList.end())
    {
        i->second.UserNum=userCount;
        i->second.maxUsers=userLimit;
    }

    mylock.WriteUnlock();
}

static void PushInt(std::vector<char> & buffer, int value)
{
    buffer.push_back(value);
    buffer.push_back(value >> 8);
    buffer.push_back(value >> 16);
    buffer.push_back(value >> 24);
}

static void PushShort(std::vector<char> & buffer, int value)
{
    buffer.push_back(value);
    buffer.push_back(value >> 8);    
}

void CServerList::MakeServerListPacket(std::vector<char> & buffer, ServerId lastServer, int regions[MAX_REGIONS]) const
{
    mylock.ReadLock();

    buffer.push_back(0);  // Reserving space for packet size
    buffer.push_back(0);
    buffer.push_back(AC_SEND_SERVERLIST);
    buffer.push_back(char(0)); // static_cast<char>(m_serverList.size())); // reserving space for serverlist size
    buffer.push_back(lastServer.GetValueChar());
    
    int serverCount=0;
    for (ServerListType::const_iterator i=m_serverList.begin(); i!=m_serverList.end(); ++i)
    {        
        for (int r=0; r<MAX_REGIONS; ++r)
        {
            if (i->second.region_id == 0 || i->second.region_id == regions[r])
            {
                ++serverCount;

                 // These add up to 14 bytes:
                buffer.push_back(i->second.serverid.GetValueChar());
                PushInt(buffer,i->second.outer_addr.S_un.S_addr);
                PushInt(buffer,i->second.outer_port);
                buffer.push_back(i->second.ageLimit);
                buffer.push_back(i->second.pkflag);
                PushShort(buffer,i->second.UserNum);
                PushShort(buffer,i->second.maxUsers);
                buffer.push_back(i->second.status);
                buffer.push_back(i->second.isVIP);

                // Send shard name
                if (config.ProtocolVer >= OUROBOROS_PROTOCOL_VERSION_1)
                {
                    // Format: one byte of size and then a non-nullterminated string
                    buffer.push_back(static_cast<char>(strnlen(i->second.name, MAX_SERVER_NAME)));
                    const char * s = i->second.name;
                    for (int j = 0; j < MAX_SERVER_NAME && *s; ++j, ++s)
                        buffer.push_back(*s);
                }

                break;
            }
        }
    }
        
    buffer[3]=static_cast<char>(serverCount);
    mylock.ReadUnlock();
}

void CServerList::MakeQueueSizePacket(std::vector<char> & buffer) const
{
    mylock.ReadLock();

    buffer.push_back(0);  // Reserving space for packet size
    buffer.push_back(0);
    buffer.push_back(AC_QUEUE_SIZE);
    buffer.push_back(0);  // Reserving space for queue list size

    char count=0;
        
    for (QueueSizesType::const_iterator i=m_queueSizes.begin(); i!=m_queueSizes.end(); ++i)
    {
        char queueLevel = 0;
        for (std::vector<std::pair<int, int> >::const_iterator j=i->second.begin(); j!=i->second.end(); ++j)
        {
            buffer.push_back(i->first.GetValueChar());
            buffer.push_back(queueLevel); // queue level
            PushInt(buffer,j->first); // queue size
            PushInt(buffer,j->second); // wait time

            ++count;
            ++queueLevel;
        }
    }

    buffer[3]=count;
        
    mylock.ReadUnlock();
}

/**
 * Record the number of users on each server to the database
 */
void CServerList::UpdateDB() const
{
    mylock.ReadLock();

    CDBConn conn(g_linDB);

    char buffer[256];
    sprintf( buffer, "{CALL dbo.sp_LogUserNumbers (?,?,?,?,?,?) }" );

    // Convert the current time to a DB-compatible format, and use it for all rows recorded
    time_t log_time;
    struct tm log_timeTM;
    TIMESTAMP_STRUCT dblogtime;

    log_time = time(0);
    log_timeTM = *localtime(&log_time);

    dblogtime.year     = log_timeTM.tm_year + 1900;
    dblogtime.month    = log_timeTM.tm_mon + 1;
    dblogtime.day      = log_timeTM.tm_mday;
    dblogtime.hour     = log_timeTM.tm_hour;
    dblogtime.minute   = log_timeTM.tm_min;
    dblogtime.second   = log_timeTM.tm_sec;
    dblogtime.fraction = 0;

    // Record "server 0," which is actually the total number of connected users
    {
        SQLLEN logtimeLen=0;
        SQLBindParameter( conn.m_stmt, 1, SQL_PARAM_INPUT, SQL_C_TYPE_TIMESTAMP,SQL_TIMESTAMP, 19, 0, (SQLPOINTER)&dblogtime, 0, &logtimeLen );
      
        SQLLEN serverIdLen = 4;
        unsigned long serverid = 0;
        SQLBindParameter( conn.m_stmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, (SQLPOINTER)&serverid, 0, &serverIdLen);

        SQLLEN worlduserLen = 4;
        SQLBindParameter( conn.m_stmt, 3, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, (SQLPOINTER)&(reporter.m_UserCount), 0, &worlduserLen);

        SQLLEN limituserLen = 4;
        SQLBindParameter( conn.m_stmt, 4, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, (SQLPOINTER)&(reporter.m_UserCount), 0, &limituserLen);

        SQLLEN authuserLen = 4;
        SQLBindParameter( conn.m_stmt, 5, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, (SQLPOINTER)&(reporter.m_UserCount), 0, &authuserLen);
 
        SQLLEN waituserLen = 4;
        SQLBindParameter( conn.m_stmt, 6, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, (SQLPOINTER)&reporter.m_SocketCount, 0, &waituserLen);

        RETCODE RetCode= SQLExecDirectA( conn.m_stmt, (SQLCHAR*)buffer, SQL_NTS );
        
        if ( RetCode == SQL_SUCCESS ) {
            conn.ResetHtmt();
        } else {
            conn.Error(SQL_HANDLE_STMT, conn.m_stmt, buffer);
            conn.ResetHtmt();
        }
    }

    // Record the count for all the real servers
    for (ServerListType::const_iterator i=m_serverList.begin(); i!=m_serverList.end(); ++i)
    {
        SQLLEN logtimeLen=0;
        SQLBindParameter( conn.m_stmt, 1, SQL_PARAM_INPUT, SQL_C_TYPE_TIMESTAMP,SQL_TIMESTAMP, 19, 0, (SQLPOINTER)&dblogtime, 0, &logtimeLen );
      
        SQLLEN serverIdLen = 4;
        unsigned long serverid = i->first.GetValueChar();
        SQLBindParameter( conn.m_stmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, (SQLPOINTER)&serverid, 0, &serverIdLen);

        SQLLEN worlduserLen = 4;
        SQLBindParameter( conn.m_stmt, 3, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, (SQLPOINTER)&(i->second.UserNum), 0, &worlduserLen);

        SQLLEN limituserLen = 4;
        SQLBindParameter( conn.m_stmt, 4, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, (SQLPOINTER)&(i->second.maxUsers), 0, &limituserLen);

        SQLLEN authuserLen = 4;
        SQLBindParameter( conn.m_stmt, 5, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, (SQLPOINTER)&(reporter.m_UserCount), 0, &authuserLen);
 
        SQLLEN waituserLen = 4;
        SQLBindParameter( conn.m_stmt, 6, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, (SQLPOINTER)&reporter.m_SocketCount, 0, &waituserLen);

        RETCODE RetCode= SQLExecDirectA( conn.m_stmt, (SQLCHAR*)buffer, SQL_NTS );
        
        if ( RetCode == SQL_SUCCESS ) {
            conn.ResetHtmt();
        } else {
            conn.Error(SQL_HANDLE_STMT, conn.m_stmt, buffer);
            conn.ResetHtmt();
        }
    
    }

    mylock.ReadUnlock();
}

/**
 * Gets an arbitrary server id that is not in use.
 */
ServerId CServerList::GetFreeServerId() const
{
    if (m_serverList.empty())
    {
        return ServerId(1);
    }
    else
    {
        ServerId maxId = m_serverList.rbegin()->first;
        return ServerId(maxId.GetValueChar() + 1);
    }
}

void CServerList::SetServerQueueSize( ServerId id, int queueLevel, int queueSize, int queueTime)
{
    if (queueLevel < 0)
        return;

    std::vector<std::pair<int, int> > & queueSizes = m_queueSizes[id];
    if (queueSizes.size() < static_cast<size_t>(queueLevel+1))
    {
        queueSizes.resize(static_cast<size_t>(queueLevel+1),std::make_pair(0,0));
    }
    queueSizes[static_cast<size_t>(queueLevel)]=std::make_pair(queueSize, queueTime);
}
