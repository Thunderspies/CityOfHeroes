#include "IOSocket.h"
#include "Protocol.h"
#include "GlobalAuth.h"
#include "util.h"
#include "buildn.h"
#include "Packet.h"
#include "IOServer.h"
#include "AccountDB.h"
#include "Config.h"
#include "DBConn.h"

#define TIMEOUT    60
#define INT_SOCKET_BUFFER_LEN 256

enum {
    INT_SOCKET_ALL_OK,
    INT_SOCKET_INCORRECT_PACKET,
    INT_SOCKET_INVALID_ACCOUNT,
    INT_SOCKET_INVALID_PWD,
};

BOOL SendSocketEx(SOCKET s, const char *format, ...);

static const char *getToken(char *token, const char *input)
{
    for (int i = 0; i < 255; i++) { // Packet contains at least one NULL character(guaranteed)
        *token = *input++;
        if (*token == NULL) // End of input stream return NULL as next token.
            return NULL;
        if (*token == '\t'|| *token == 13) {
            *token = 0;
            return input;
        }
        token++;
    }
    *token = 0;
    return NULL;
}

static void KickAccount(CSocketInt *mysocket, const char *packet)
{
    char name[INT_SOCKET_BUFFER_LEN];
    int  uid=0;
    packet = getToken( name, packet );
    if ( !packet ){
        mysocket->Send( "0" );
        return;
    }
    packet = getToken( name, packet );
    int len = (int)strlen( name );
    for( int i=0; i < len;i++)
    {
        if( !isdigit( name[i] ))
        {
            mysocket->Send( "0" );
            return;
        }
    }
    uid = atoi( name );
    
    accountdb.KickAccount( uid, S_KICKED_BY_WEB, true );
    mysocket->Send( "1" );

    return;
}


static void GetLoginUserCount( CSocketInt *mysocket, const char *packet )
{
    mysocket->Send("%d", accountdb.GetUserNum() );
    return;
}

static void KickAccountByaccount( CSocketInt *mysocket, const char *packet )
{
    char name[INT_SOCKET_BUFFER_LEN];
    int  uid=0;
    memset( name, 0, 32 );
    packet = getToken( name, packet );
    if ( !packet ){
        mysocket->SendBuffer( "0,incorrect format", 19 );
        return;
    }
    packet = getToken( name, packet );
    name[14]=0;    
    CDBConn dbconn(g_linDB);

    dbconn.Bind( &uid );

    dbconn.Execute( "SELECT uid FROM user_account with (NOLOCK) WHERE account = '%s'", name );
    bool nodata=true;
    if ( dbconn.Fetch(&nodata))
    {
        if ( nodata ){
            mysocket->Send("0,InvalidAccount" );
            return;
        } else {
            accountdb.KickAccount( uid, S_KICKED_BY_WEB, true );
            mysocket->Send("1" );
            return;
        }
    } else {
        mysocket->Send("0,InvalidAccount" );

    }

    return;
}

static void SetGMOnlyMode ( CSocketInt *mysocket, const char *packet )
{
    char buffer[INT_SOCKET_BUFFER_LEN];    
    packet = getToken( buffer, packet );
    if ( !packet ) {
        mysocket->Send( "0,Incorrect Format" );
        return;
    }
    memset( buffer, 0, 32 );
    packet = getToken( buffer, packet );


    if ( buffer[0] == '0' ){
        config.GMCheckMode = false;
        logger.AddLog(LOG_WARN, "GM ONLY MODE OFF" );
        mysocket->Send( "1" );
    }
    else{
        config.GMCheckMode = true;
        logger.AddLog( LOG_WARN, "GM ONLY MODE ON" );
        mysocket->Send( "1" );
    }

    return;
}

static void ChangeLimitUser( CSocketInt *mysocket, const char *packet )
{
    char buffer[INT_SOCKET_BUFFER_LEN];
    packet = getToken( buffer, packet );
    if ( packet ){
        getToken(buffer, packet );
        int limituser = 0;
        limituser = atoi( buffer );
        if ((limituser>0)&&(limituser<12000)){
            config.SocketLimit = limituser;
            logger.AddLog( LOG_WARN, "Change socketlimit, %d", config.SocketLimit );
            mysocket->Send("1");
        }
    }
    
    return;
}

static void GetAccountInfo( CSocketInt *mysocket, const char *packet )
{
}

static void SetGMIP( CSocketInt *mysocket, const char *packet )
{
    char buffer[256];
    in_addr ip;
    packet = getToken( buffer, packet );
    if ( !packet ){
        mysocket->Send("0" );
        return;
    }
    
    getToken( buffer, packet );
    buffer[16]=0;
    ip.S_un.S_addr = inet_addr( buffer );
    if ( ip.S_un.S_addr != INADDR_NONE ){
        config.GMIP = ip;
        logger.AddLog( LOG_WARN, "Change GMIP, %d.%d.%d.%d", ip.S_un.S_un_b.s_b1, ip.S_un.S_un_b.s_b2, ip.S_un.S_un_b.s_b3, ip.S_un.S_un_b.s_b4 );
        mysocket->Send("1" );
        return;
    }

    mysocket->Send( "0" );
    return;
}


static void SetGMAccessLimit( CSocketInt *mysocket, const char *packet )
{
    return;
}

static void SetFreeServer( CSocketInt *mysocket, const char *packet )
{
    char buffer[256];
    packet = getToken( buffer, packet );

    if ( packet ){
        packet = getToken( buffer, packet );
        if ( buffer[0] == '0' ){
            config.FreeServer = false;
            logger.AddLog( LOG_WARN, "Config FreeServer is chaged to false" );
            mysocket->Send("1");
            return;
        } else if ( buffer[0] == '1' ){
            config.FreeServer = true;
            logger.AddLog( LOG_WARN, "Config FreeServer is changed to true" );
            mysocket->Send("1" );
            return;
        }
    }
    mysocket->Send( "0" );
    
    return;
}

static void OCHKRequest( CSocketInt *mysocket, const char *packet )
{
    char buffer[256];
    char account[16];
    char pwd[ENC_PWD_LEN];
    char clientKey[64];

    memset( clientKey, 0, 64 );
    memset( pwd, 0, ENC_PWD_LEN );
    packet = getToken( buffer, packet );

    if ( packet ) {
        packet = getToken( buffer, packet ); // get user id
        if ( packet ) {
            strncpy( account, buffer, 16 );
            packet = getToken( buffer, packet ); // get pwd
            if ( packet ){
                strncpy( pwd, buffer, 20 );
                packet = getToken( buffer, packet ); // get client key
                strncpy( clientKey, buffer, 63 );
                
                CAccount accinfo;
                char result = accinfo.CheckPassword( account, pwd, NULL, 0, false );
                if ( result == S_ACCOUNT_LOAD_FAIL ) {
                    mysocket->Send("-OCHK\t201\tError\t%s\r\n", clientKey);
                    return;
                }
                if ( result == S_INCORRECT_PWD ) {
                    mysocket->Send("-OCHK\t202\tError\t%s\r\n", clientKey);
                    return;
                }
                if ( accinfo.block_flag == 0 && accinfo.block_flag2 == 0 )
                    mysocket->Send("+OCHK\t%d\t%s\t%d\r\n", accinfo.login_flag, clientKey,accinfo.uid );
                else
                    mysocket->Send("+OCHK\t%d\t%s\t%d\r\n", accinfo.login_flag | 0x80000000, clientKey, accinfo.uid );
                return;

            } else {
                mysocket->Send("-OCHK\t003\tError\r\n" );
                return;            
            }
        } else {
            mysocket->Send("-OCHK\t003\tError\r\n" );
            return;
        }
    }
    
    mysocket->Send("-OCHK\t004\t\n" );
    return;
}

static void CheckPassword(CSocketInt *mysocket, const char *packet)
{
    char buffer[INT_SOCKET_BUFFER_LEN];
    char account[MAX_ACCOUNT_LEN+1];
    char pwd[MAX_PWD_LEN+1];

    memset(account, 0, MAX_ACCOUNT_LEN+1);
    memset(pwd, 0, MAX_PWD_LEN+1);

    packet = getToken( buffer, packet ); // skip packet type
    if (packet) {
        buffer[0]=0;
        packet = getToken(buffer, packet);
        if (packet) {
            buffer[MAX_ACCOUNT_LEN];
            strncpy(account, buffer, MAX_ACCOUNT_LEN);
            account[MAX_ACCOUNT_LEN] = 0;
            if (CheckAccount(account)){
                buffer[0]=0;
                packet = getToken(buffer, packet);
                if (packet) {
                    buffer[MAX_PWD_LEN]=0;
                    strncpy(pwd, buffer, MAX_PWD_LEN);
                    CAccount accinfo;
                    char result = accinfo.CheckPassword( account, pwd, NULL, 0, false );
                    if ( result == S_ALL_OK ) {
                        mysocket->Send( "%d", INT_SOCKET_ALL_OK );
                    } else {
                        mysocket->Send( "%d", INT_SOCKET_INVALID_PWD );    
                    }

                }else{
                    mysocket->Send( "%d", INT_SOCKET_INCORRECT_PACKET );
                }
            } else {
                mysocket->Send( "%d", INT_SOCKET_INVALID_ACCOUNT );
            }
        } else {
            mysocket->Send( "%d", INT_SOCKET_INCORRECT_PACKET );
        }
    } else {
        mysocket->Send( "%d", INT_SOCKET_INCORRECT_PACKET );
    }

    return;
}

static InteractivePacketFunc interactivePacketTable[] ={
    KickAccount,
    ChangeLimitUser,
    GetLoginUserCount,
    KickAccountByaccount,
    SetGMOnlyMode,
    GetAccountInfo,
    SetGMIP,
    SetGMAccessLimit,
    SetFreeServer,
    OCHKRequest, // 9
    CheckPassword, // 10
};

#define    MAX_INTERACTIVE_PACKET    (sizeof(interactivePacketTable)/sizeof(interactivePacketTable[0]))

CSocketInt::CSocketInt( SOCKET aSoc )
: CIOSocket( aSoc )
{
    addr.s_addr = 0;
    host = 0;
    mode = ISM_READ_LEN;
    packetTable = interactivePacketTable;
}

CSocketInt::~CSocketInt()
{
}

CSocketInt *CSocketInt::Allocate( SOCKET s )
{
    return new CSocketInt( s );
}
void CSocketInt::OnClose(SOCKET closedSocket)
{
    // Must not use the GetSocket() function from within
     // this function!  Instead, use the socket argument
     // passed into the function.
    mode = ISM_CLOSE;
    logger.AddLog(LOG_NORMAL, "*close connection from %s, %x(%x)", IP(), closedSocket, this);
}

void CSocketInt::OnRead()
{
    char *buf = m_pReadBuf->m_buffer;
    int ri = 0;
    int len = m_pReadBuf->m_size;
    while (len-- > 0) {
        if (ri >= 255 || buf[ri] == '\n') {
            buf[ri] = 0;
            m_nTimeout = TIMEOUT;
            Process(buf);
            buf += ri + 1;
            ri = 0;
        }
        else
            ri++;
    }
    Read(ri);
}
void CSocketInt::Process(char *buf)
{
    int packetNumber;
    AS_LOG_VERBOSE( "%s", buf );
    sscanf(buf, "%d", &packetNumber);
    if (packetNumber >= 0 && packetNumber < MAX_INTERACTIVE_PACKET) {
        (*packetTable[packetNumber])(this, buf);
    }
}
const char *CSocketInt::IP()
{
    return inet_ntoa(addr);
}

void CSocketInt::OnCreate()
{
    OnRead();
}

void CSocketInt::SetAddress(in_addr inADDR )
{
    addr = inADDR;
}

void CSocketInt::Send(const char* format, ...)
{
    if (mode == ISM_CLOSE)
        return;

    CIOBuffer *pBuffer = CIOBuffer::Alloc();
    va_list ap;
    va_start(ap, format);
    pBuffer->m_size = max(vsprintf(pBuffer->m_buffer, format, ap), 0);
    va_end(ap);
    Write(pBuffer);
}

void CSocketInt::SendBuffer(const char* buf, int len)
{
    if (mode == ISM_CLOSE)
        return;
    if (len > 0) {
        if (len > BUFFER_SIZE)
            return;
        CIOBuffer *pBuffer = CIOBuffer::Alloc();
        memcpy(pBuffer->m_buffer, buf, len);
        pBuffer->m_size = len;
        Write(pBuffer);
    }
}