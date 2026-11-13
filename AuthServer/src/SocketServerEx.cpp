#include "GlobalAuth.h"
#include "Packet.h"
#include "IOSocket.h"
#include "buildn.h"
#include "Protocol.h"
#include "DBConn.h"
#include "Account.h"
#include "util.h"
#include "Des.h"
#include "ServerList.h"
#include "Thread.h"
#include "AccountDB.h"
#include "IOServer.h"
#include "Config.h"
#include "IPSessionDB.h"
#include "IPList.h"
#include "auth/account_creation.h"
#include <memory>
#include <exception>
#include <utility>
AccountDB accountdb;

BOOL SendSocket(in_addr , const char *format, ...);

#include <osrng.h>
#include <rng.h>
#include <rsa.h>

using namespace CryptoPP;

static std::unique_ptr<InvertibleRSAFunction> rsaParams;

bool InitRSAParams()
{
    if (rsaParams)
        return true;
    try
    {
        AutoSeededRandomPool rng;
        std::unique_ptr<InvertibleRSAFunction> key(new InvertibleRSAFunction);
        // Preserve the established modulus size and Crypto++ public exponent.
        key->Initialize(rng, 1152);
        rsaParams = std::move(key);
        return true;
    }
    catch (const std::exception &)
    {
        return false;
    }
}

static bool DecryptRSAText(const unsigned char *ciphertext, size_t length, unsigned char *plain, size_t expectedLength)
{
    accountCreationClear(plain, expectedLength);
    if (!rsaParams || !ciphertext || length != rsaParams->GetModulus().MinEncodedSize())
        return false;
    try
    {
        struct RsaWorker
        {
            AutoSeededRandomPool rng;
            RSAES_OAEP_SHA_Decryptor decryptor;

            RsaWorker() : decryptor(*rsaParams)
            {
            }
        };
        // Each worker owns its RNG and private-operation state for its lifetime.
        static thread_local RsaWorker worker;
        if (length != worker.decryptor.FixedCiphertextLength())
            return false;
        SecByteBlock recovered(worker.decryptor.MaxPlaintextLength(length));
        if (expectedLength > recovered.size())
            return false;
        DecodingResult result = worker.decryptor.Decrypt(worker.rng, ciphertext, length, recovered);
        if (!result.isValidCoding || result.messageLength != expectedLength)
            return false;
        memcpy(plain, recovered.data(), expectedLength);
        return true;
    }
    catch (const std::exception &)
    {
        return false;
    }
}

static bool CreateAccountPacket(CSocketServerEx *socket, const unsigned char *packet, size_t length)
{
    unsigned char plain[ACCOUNT_CREATE_PLAIN_BYTES] = {0};
    unsigned char hash[128] = {0};
    AccountCreationResult result;
    if (!config.AllowAccountCreation)
        result = ACCOUNT_CREATE_DISABLED;
    else if (socket->um_mode != UM_PRE_LOGIN || socket->uid != 0)
        result = ACCOUNT_CREATE_INVALID_STATE;
    else if (config.useForbiddenIPList && forbiddenIPList.IpExists(socket->getaddr()))
        result = ACCOUNT_CREATE_BLOCKED_IP;
    else if (!rsaParams || !accountCreationFrame(packet, length, rsaParams->GetModulus().MinEncodedSize(),
        config.ProtocolVer >= GLOBAL_AUTH_PROTOCOL_VERSION) ||
        !DecryptRSAText(packet + 4, rsaParams->GetModulus().MinEncodedSize(), plain, sizeof(plain)))
        result = ACCOUNT_CREATE_INVALID_REQUEST;
    else
    {
        result = accountCreationValidateBlock(plain);
        if (result == ACCOUNT_CREATE_SUCCESS)
        {
            if (!accountCreationHash((char *)plain, (char *)plain + ACCOUNT_CREATE_NAME_BYTES, hash))
                result = ACCOUNT_CREATE_DATABASE_FAILURE;
            else
            {
                CDBConn conn(g_linDB);
                result = conn.CreateAccount((char *)plain, hash);
            }
        }
    }
    accountCreationClear(plain, sizeof(plain));
    accountCreationClear(hash, sizeof(hash));
    socket->Send("cc", AC_CREATE_ACCOUNT_RESULT, result);
    return false;
}

static bool LoginPacketSecure(CSocketServerEx *mysocket, const unsigned char *packet, size_t length, bool useMD5)
{
    CAccount account;
    unsigned char RSAblock[94] = {0};
    int  subscription;
    char name[15];
    char passwordLineage2[MAX_PWD_LEN+1];
    char passwordSHA512[ENC_PWD_LEN+1];
    char  err_msg = 0;
    unsigned short cdkind=0;
    
    in_addr cip = mysocket->getaddr();
    
    if ( config.useForbiddenIPList ) {
        if ( forbiddenIPList.IpExists( cip ) ){
            mysocket->Send( "cc", AC_LOGIN_FAIL, S_BLOCKED_IP );
            return false;
        }
    }

    mysocket->m_lastIO = GetTickCount();

    memset( name, 0, 15 );
    memset( passwordLineage2, 0, MAX_PWD_LEN+1 );
    memset( passwordSHA512, 0, ENC_PWD_LEN+1 );

    if ( config.DesApply ){
        if (length < 4)
            return true;
        int size = GetIntFromPacket( packet );
        if (!rsaParams || size != (int)rsaParams->GetModulus().MinEncodedSize() || length < 4 + (size_t)size + 6 ||
            !DecryptRSAText(packet, size, RSAblock, sizeof(RSAblock)))
        {
            logger.AddLog(LOG_ERROR, "SND: AC_LOGIN_FAIL,invalid_key_size" );
            mysocket->Send( "cc", AC_LOGIN_FAIL, S_INCORRECT_MD5Key );
            return false;
        }
        strncpy( name, (char *)RSAblock, 14);
        memcpy( passwordLineage2, (char *)RSAblock+14, 16);
        memcpy( passwordSHA512, (char *)RSAblock+30, 64);
        accountCreationClear(RSAblock, sizeof(RSAblock));

        packet += size;
    } 
    else
    {
        return true;    //    not supported anymore
    }
    subscription = GetIntFromPacket( packet );
    cdkind         = GetShortFromPacket( packet );
    
    logger.AddLog(LOG_VERBOSE, "RCV: AQ_LOGIN,account:%s,ip:%d.%d.%d.%d,subscription:%d,cdkind:%d", name, cip.S_un.S_un_b.s_b1, cip.S_un.S_un_b.s_b2, cip.S_un.S_un_b.s_b3, cip.S_un.S_un_b.s_b4, subscription, cdkind);
    logger.AddLog(LOG_DEBUG, "RCV: AQ_LOGIN,useMD5:%i, MD5Key %i", useMD5, mysocket->GetMd5Key());

    err_msg = account.CheckPassword( name, passwordLineage2, passwordSHA512, mysocket->GetMd5Key(), useMD5 );
    accountCreationClear(passwordLineage2, sizeof(passwordLineage2));
    accountCreationClear(passwordSHA512, sizeof(passwordSHA512));

    if ( err_msg != S_ALL_OK )
    {
#ifdef _DEBUG
        logger.AddLog(LOG_ERROR, "SND: AC_LOGIN_FAIL,Error Msg: %d", err_msg );
#endif
        mysocket->Send( "cc", AC_LOGIN_FAIL, err_msg );
        return false;
    }

    if ( account.block_flag2 != 0 ){
        AS_LOG_VERBOSE( "SND: AC_BLOCKED_ACCOUNT,reason1:%d", account.block_flag2 );
        mysocket->Send( "cdd", AC_BLOCKED_ACCOUNT,account.block_flag2, account.block_end_date );        
        return false;
    }

    if ( account.block_flag != 0 ){
        char bmsg[4096];
        memset(bmsg, 0, 4096);
        int len=account.MakeBlockInfo( bmsg+3 );
        int len2 = len+config.PacketSizeType;
        bmsg[0] = static_cast<char>(len2);
        bmsg[1] = len2>>8;
        bmsg[2] = AC_BLOCKED_ACCOUNT_WITH_MSG;
        AS_LOG_VERBOSE( "SND: AC_BLOCKED_ACCOUNT_WITH_MSG;,block_flag:%d",account.block_flag);
        mysocket->Send( bmsg, 3+len);
        return false;
    }
    
    if ( account.login_flag & 16 ) {
        if ( config.RestrictGMIP ) {
            if( (mysocket->getaddr()).S_un.S_addr != (config.GMIP).S_un.S_addr ){
                in_addr loginaddr = mysocket->getaddr();
                CDBConn conn(g_linDB);
                conn.Execute( "INSERT gm_illegal_login ( account, ip ) VALUES ( '%s', '%d.%d.%d.%d' )", 
                    account.account, loginaddr.S_un.S_un_b.s_b1, loginaddr.S_un.S_un_b.s_b2, loginaddr.S_un.S_un_b.s_b3, loginaddr.S_un.S_un_b.s_b4 );
                mysocket->Send( "cc", AC_LOGIN_FAIL, S_SYSYTEM_ERROR );
                return false;
            }
        }
    }

    if ( config.FreeServer ) {
        if ( account.pay_stat != 0 )
            account.pay_stat = 1002;
    }

    int isRetail = 0;
    if (config.UserData)
    {
        unsigned char userdata[MAX_USERDATA];
        
        memset( userdata, 0, MAX_USERDATA );

        {
        CDBConn dbconn(g_linDB);
        SQLLEN UserInd=0;
        SQLBindCol( dbconn.m_stmt, 1, SQL_C_BINARY, (char *)(userdata), MAX_USERDATA_ORIG, &UserInd );

        SQLLEN UserIndNew=0;
        SQLBindCol( dbconn.m_stmt, 2, SQL_C_BINARY, (char *)(&userdata[MAX_USERDATA_ORIG]), MAX_USERDATA_NEW, &UserIndNew );

        SQLLEN cbUid=0;
        SQLBindParameter( dbconn.m_stmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, (SQLPOINTER)(&account.uid), 0, &cbUid );

        dbconn.Execute( "SELECT user_data, user_data_new FROM user_data WHERE uid = ?" );
        dbconn.Fetch();
        }

        isRetail = userdata[12] & 0x10;
    }

    LoginUser *loginuser = new LoginUser;
    loginuser->loginIp   = mysocket->getaddr();
    loginuser->md5key    = mysocket->GetMd5Key();
    loginuser->stat      = isRetail && config.IsReactivationActive() ? config.ReactivationValue : account.pay_stat;
    loginuser->s         = mysocket->GetSocket();
    loginuser->gender    = account.gender;
    loginuser->timerHandle = NULL;
    loginuser->um_mode   = UM_LOGIN;
    loginuser->ssn       = account.nSSN;
    loginuser->ssn2      = account.ssn2;
    loginuser->loginflag = account.login_flag;
    loginuser->warnflag  = account.warn_flag;
    loginuser->age       = account.age;
    loginuser->cdkind     = cdkind;
    strncpy( loginuser->account, account.account, MAX_ACCOUNT_LEN+1 );
    loginuser->account[MAX_ACCOUNT_LEN]=0;
    loginuser->lastworld = account.lastworld;
    loginuser->logintime = time(NULL);
    loginuser->queuetime = time(NULL);
    loginuser->serverid.SetInvalid();
    loginuser->selectedServerid.SetInvalid();
    loginuser->queueLevel = account.queueLevel;
    loginuser->loyalty = account.loyalty;
    loginuser->loyaltyLegacy = account.loyaltyLegacy;
    memcpy(loginuser->regions, account.regions, sizeof(account.regions));
    
    mysocket->uid = account.uid;
    int OperationCode = (int)(( account.pay_stat% 1000 ) / 100 );

    if ( config.UseIPServer){

        int result=S_ALL_OK;
    
        if ( config.PCCafeFirst )
        {
            result = ipsessionDB.AcquireSessionRequest( loginuser, account.uid );
            switch ( result){
            case IP_ALREADY_WAIT:
                mysocket->Send( "cc", AC_LOGIN_FAIL, S_ALREADY_LOGIN );
                delete loginuser;
                break;
            case IP_ALL_OK:
                break;
            default:
                accountdb.CheckPersonalPayStat( mysocket, loginuser, account.uid );
                delete loginuser;
                break;
            }

            return false;
        }
        
        if ( account.pay_stat == 0 || OperationCode == PERSONAL_SPECIFIC){
            result= ipsessionDB.AcquireSessionRequest( loginuser, account.uid ) ;
            switch ( result){
            case IP_ALREADY_WAIT:
                mysocket->Send( "cc", AC_LOGIN_FAIL, S_ALREADY_USED_IP );
                delete loginuser;
                break;
            case IP_ALL_OK:
                break;
            default:
                accountdb.CheckPersonalPayStat( mysocket, loginuser, account.uid );
                delete loginuser;
                break;
            }

            return false;
        }

    };
    
    accountdb.CheckPersonalPayStat( mysocket, loginuser, account.uid );

    delete loginuser;
    return false;
}

static bool LoginPacket(CSocketServerEx *mysocket, const unsigned char *packet, size_t length)
{
    return LoginPacketSecure(mysocket, packet, length, false);
}

static bool SelectServerPacket( CSocketServerEx *pSocket, const unsigned char *packet, size_t length)
{
    if (length < 9)
        return true;
    int uid, md5key, loginflag=0, warnflag=0;
    char account[MAX_ACCOUNT_LEN+1];
_BEFORE
    memset(account, 0, MAX_ACCOUNT_LEN+1);

    uid    = GetIntFromPacket( packet );
    md5key = GetIntFromPacket( packet );
    unsigned char temp = GetCharFromPacket( packet );
    ServerId serverid(temp);

    AS_LOG_VERBOSE( "RCV: AQ_ABOUT_TO_PLAY,uid:%d,serverid:%d", uid, serverid.GetValueChar() );

    int pay_stat=0;
    int pKey=0;
    int queueLevel=0;
    int loyalty = 0;
    int loyaltyLegacy = 0;
    pSocket->m_lastIO = GetTickCount();
    
    if ( !accountdb.FindAccount( uid , account, &loginflag, &warnflag, &pay_stat, &pKey, &queueLevel, &loyalty, &loyaltyLegacy ) ) {

        AS_LOG_VERBOSE( "SND: AC_PLAY_FAIL,NO Login Info");
        pSocket->Send( "cc", AC_PLAY_FAIL, S_NO_LOGININFO );        
        return false;        
    }
    if ( pKey != md5key ){
        pSocket->Send( "cc", AC_PLAY_FAIL, S_NO_LOGININFO );        
        logger.AddLog(LOG_ERROR, "md5key does not matching" );
        return false;
    }

    StdAccount( account );

    if ( !g_ServerList.IsServerUp(serverid))
    {
        AS_LOG_VERBOSE( "SND: AC_PLAY_FAIL,server down : invalid serverid", S_SERVER_DOWN );
        pSocket->Send( "cc", AC_PLAY_FAIL, S_SERVER_DOWN );        
        return false;
    }
    
    int OperationCode = (int)(( pay_stat% 1000 ) / 100 );    
    int RemainTime=0;
    char RetCode = S_ALL_OK;

    if ( OperationCode== 0 ){
    } else if ( OperationCode == PERSONAL_SPECIFIC ) {
        RetCode= accountdb.CheckUserTime( uid ,&RemainTime );
    }

    if ( RetCode != S_ALL_OK ){
        pSocket->Send( "cc", AC_PLAY_FAIL, RetCode );
        return false;
    }

    if ( g_ServerList.IsServerVIPonly(serverid))
    {
        int payment_plan_id = pay_stat % 1000;
        if (payment_plan_id == 0 || payment_plan_id == 11 || payment_plan_id == 15 || payment_plan_id == 16)
        {
            if (loyalty <= 1)
            {
                pSocket->Send( "cc", AC_PLAY_FAIL, S_VIP_ONLY );
                return false;
            }
        }
    }

    pSocket->AddRef();
    accountdb.AboutToPlay( uid, account, RemainTime, loginflag, warnflag, md5key, pSocket, serverid, pay_stat, queueLevel, loyalty, loyaltyLegacy);
    pSocket->ReleaseRef();
_AFTER_FIN
    return false;
}
static bool LogoutPacket( CSocketServerEx *mysocket, const unsigned char *packet, size_t length)
{
    if (length < 8)
        return true;
    int uid = GetIntFromPacket( packet );
    int md5key = GetIntFromPacket( packet );

    if ( !accountdb.logoutAccount( uid, md5key ) )
    {
        AS_LOG_DEBUG( "md5key does not match is it hack??");
        return false;
    }
    AS_LOG_VERBOSE( "AQ_LOGOUT, uid %d, md5key %d", uid, md5key);
    return true;

}

static bool DeprecatedPacket(CSocketServerEx * /*mysocket*/, const unsigned char * /*packet*/, size_t /*length*/)
{
    logger.AddLog(LOG_ERROR, "Received a deprecated packet from client connection.");
    return true;
}

static bool ServerListExtPacket(CSocketServerEx *mysocket, const unsigned char *packet, size_t length)
{
    if (length < (config.useClientSpecifiedRegions ? 10u : 9u))
        return true;
    int  uid    = GetIntFromPacket( packet );
    UINT md5key = GetIntFromPacket( packet );
    char ListKind = GetCharFromPacket( packet );
    
    int clientRegion[MAX_REGIONS];
    int numRegions = 0;

    if (config.useClientSpecifiedRegions)
    {
        numRegions = (unsigned char)GetCharFromPacket(packet);
        if (length < 10u + 4u * numRegions)
            return true;
        if (numRegions > MAX_REGIONS)
            printf("Packet requested %d regions, max allowed is %d\n", numRegions, MAX_REGIONS);

        for (int r = 0; r < MAX_REGIONS; r++)
        {
            clientRegion[r] = 0;
        }

        //We read all the regions in now, even if there are too many, so we stay aligned with the packet
        for (int r = 0; r < numRegions; r++)
        {
            if (r < MAX_REGIONS)
            {
                clientRegion[r] = GetIntFromPacket(packet);
            }
            else
            {
                GetIntFromPacket(packet); // Throw it away
            }
        }
        numRegions = min(numRegions, MAX_REGIONS);
    }

   if ( ListKind !=1)
    {
        mysocket->Send( "cc", AC_SEND_SERVER_FAIL, S_NO_SERVERLIST );
        return false;
    }

    char account[MAX_ACCOUNT_LEN+1];

    AS_LOG_VERBOSE( "RCV: AQ_SERVER_LIST_EXT,uid:%d, md5key:%d,ListKind:%d",uid, md5key, ListKind );
    
    mysocket->uid = uid;
    ServerId serverid;
    int regions[MAX_REGIONS];
    bool result = accountdb.FindAccount( uid , account, serverid, regions );    
    if ( !result)
    {
        return true;
    }
    mysocket->m_lastIO = GetTickCount();

    std::vector<char> message;
    message.reserve(2048);
    if (config.useClientSpecifiedRegions && numRegions > 0)
    {
        g_ServerList.MakeServerListPacket(message, serverid, clientRegion);
    }
    else
    {
        g_ServerList.MakeServerListPacket(message, serverid, regions);
    }

    mysocket->Send( &(message[0]), (int)message.size() );

    if (config.useQueue)
    {
        message.clear();
        g_ServerList.MakeQueueSizePacket(message);
        mysocket->Send( &(message[0]), (int)message.size() );
    }

    return false;
}


BOOL SendSocket( in_addr ina, const char *format, ...)
{
_BEFORE
    CSocketServer *pSocket = server->FindSocket(ina);
    if (!pSocket)
        return FALSE;
    CIOBuffer *pBuffer = CIOBuffer::Alloc();
    char *buffer = pBuffer->m_buffer;
    va_list ap;
    va_start(ap, format);
    int len = Assemble(buffer + 2, BUFFER_SIZE - 2, format, ap);
    va_end(ap);
    if (len == 0) {
        logger.AddLog(LOG_ERROR, "%d: assemble too large packet. format %s", pSocket->GetSocket(), format);
    } else {
        len -= 1;
        len = len + config.PacketSizeType;
        buffer[0] = static_cast<char>(len);
        buffer[1] = static_cast<char>(len >> 8);
    }
    pBuffer->m_size = len+3-config.PacketSizeType;
    pSocket->Write(pBuffer);
    pSocket->ReleaseRef();
_AFTER_FIN
    return TRUE;
}

static bool LoginPacketMd5Key( CSocketServerEx *pSocket, const unsigned char *packet, size_t length)
{
    return LoginPacketSecure(pSocket, packet, length, true);
}

static_assert(AQ_CREATE_ACCOUNT == 7 && AC_CREATE_ACCOUNT_RESULT == 15, "Account creation wire IDs changed");
static ServerPacketFuncEx ServerPacketEx[] = {
    LoginPacket,
    DeprecatedPacket,
    SelectServerPacket,
    LogoutPacket,
    LoginPacketMd5Key,
    ServerListExtPacket,
    DeprecatedPacket, // Reserved request 6
    CreateAccountPacket,
};

static_assert(sizeof(ServerPacketEx) / sizeof(ServerPacketEx[0]) == AQ_MAX, "Client dispatch table mismatch");


VOID CALLBACK SocketExTimerCallback(PVOID lpParam, unsigned char TimerOrWaitFired)
{
    SOCKET s = (SOCKET)lpParam;
    CSocketServerEx *mysocket = serverEx->FindSocket( s );
    if ( mysocket )
        mysocket->OnTimerCallback( );
}

CSocketServerEx::CSocketServerEx( SOCKET aSoc )
: CIOSocket( aSoc )
{
    addr.s_addr = 0;
    host = 0;
    mode = SM_READ_LEN;
    packetTable =ServerPacketEx;
    um_mode = UM_PRE_LOGIN;
    uid = 0;
    m_TimerHandle = NULL;
    int zero = 0;
    
    m_lastIO = GetTickCount();
    InterlockedIncrement( &(reporter.m_SocketCount ));
    int ret = CreateTimerQueueTimer( &m_TimerHandle, g_hSocketTimer,SocketExTimerCallback, (LPVOID)GetSocket(), config.SocketTimeOut, config.SocketTimeOut , 0 );
//    int ret = RegisterTimer( (config.SocketTimeOut), TRUE );
    if ( ret == 0 ) {
        logger.AddLog(LOG_ERROR, "create socket timer error" );
    }
    if ( config.ProtocolVer == USA_AUTH_PROTOCOL_VERSION )
    {
        EncPacket = Encrypt;
        DecPacket = Decrypt;
    } else if ( config.ProtocolVer >= GLOBAL_AUTH_PROTOCOL_VERSION ) {
        EncPacket = BlowFishEncryptPacket;
        DecPacket = BlowFishDecryptPacket;
    }
}

CSocketServerEx::~CSocketServerEx()
{
//    InterlockedDecrement( &(reporter.m_SocketEXObjectCount ));
}

CSocketServerEx *CSocketServerEx::Allocate( SOCKET s )
{
    return new CSocketServerEx( s );
}

void CSocketServerEx::OnClose(SOCKET closedSocket)
{
    // Must not use the GetSocket() function from within
     // this function!  Instead, use the socket argument
     // passed into the function.
    HANDLE timerhandle = NULL;
    #pragma warning( push )
    #pragma warning( disable: 4312 )
    timerhandle = InterlockedExchangePointer( &m_TimerHandle, NULL );
    #pragma warning( pop )
    
    if ( timerhandle )
        DeleteTimerQueueTimer( g_hSocketTimer, timerhandle, NULL );
    m_TimerHandle = NULL;
    mode = SM_CLOSE;
    InterlockedDecrement( &(reporter.m_SocketCount ));
    serverEx->RemoveSocket(closedSocket);
    logger.AddLog(LOG_DEBUG, "*close connection from %s, %x(%x)", IP(), closedSocket, this);
    if ( um_mode != UM_PLAY_OK && uid != 0 )
        accountdb.removeAccountPreLogIn( uid, closedSocket );
    else
        accountdb.UpdateSocket( uid, INVALID_SOCKET, oneTimeKey, ServerId::s_invalid );

    IPaccessLimit.DelAccessIP( getaddr() );
}

void CSocketServerEx::OnRead()
{
    int pi = 0;
    int ri = m_pReadBuf->m_size;
    bool result;
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
                if (packetLen <= 0 || packetLen > BUFFER_SIZE - 2) {
                    logger.AddLog(LOG_ERROR, "%d: bad packet size %d", m_hSocket, packetLen);
                    break;
                } else {
/////////////////////////////
//decode routine need
/////////////////////////////
                    logger.AddLog(LOG_DEBUG, "Received %d bytes DecKey 0x87546CA1, 0x%x", packetLen+2, DecOneTimeKey);
                    pi += 2;
                    mode = SM_READ_BODY;
                }
            } else {
                Read(ri - pi);
                return;
            }
        } else if (mode == SM_READ_BODY) {
            if (pi + packetLen <= ri) {
                
                result = DecPacket(inBuf + pi, DecOneTimeKey, packetLen);
                if ( !result ){
#ifdef _DEBUG
                    logger.AddLog(LOG_ERROR, "unknown protocol,decrypt error" );
#endif
                    break;
                }
                // Credential packets must never be dumped to logs.
                if (inBuf[pi] >= AQ_MAX) {
                    logger.AddLog(LOG_ERROR, "unknown protocol %d", inBuf[pi]);
                    break;
                } else {
                    CPacketServerEx *pPacket = CPacketServerEx::Alloc();
                    pPacket->m_pSocket = this;
                    pPacket->m_pBuf = m_pReadBuf;
                    pPacket->m_packetLength = packetLen - 1;
                    pPacket->m_pFunc = ServerPacketEx[inBuf[pi]];
                    CIOSocket::AddRef();
                    m_pReadBuf->AddRef();
                    InterlockedIncrement(&CPacketServerEx::g_nPendingPacket);
                    pPacket->PostObject(pi, g_hIOCompletionPort);
                    pi += packetLen;
                    mode = SM_READ_LEN;
                    m_lastIO = GetTickCount();
                }
            } else {
                Read(ri - pi);
                return;
            }
        }
        else
            break;
    }
    CIOSocket::CloseSocket();
}

const char *CSocketServerEx::IP()
{
    return inet_ntoa(addr);
}

void CSocketServerEx::OnCreate()
{
    if (!rsaParams)
    {
        CloseSocket();
        return;
    }
    try
    {
        int modByteCount = static_cast<int>(rsaParams->GetModulus().MinEncodedSize());
        int expByteCount = static_cast<int>(rsaParams->GetPublicExponent().MinEncodedSize());
        if (modByteCount <= 0 || modByteCount > 512 || expByteCount <= 0 || expByteCount > modByteCount)
        {
            CloseSocket();
            return;
        }
        // Export the actual generated exponent rather than assuming its value.
        SecByteBlock modBuffer(modByteCount);
        SecByteBlock expBuffer(expByteCount);
        rsaParams->GetModulus().Encode(modBuffer, modByteCount);
        rsaParams->GetPublicExponent().Encode(expBuffer, expByteCount);

        oneTimeKey = static_cast<int>((static_cast<unsigned int>(rand()) << 16) | static_cast<unsigned int>(rand()));
        EncOneTimeKey = oneTimeKey | PRIVATE_KEY;
        DecOneTimeKey = oneTimeKey | PRIVATE_KEY;

        if (config.encrypt)
            Send("cdddbdb", AC_PROTOCOL_VER, oneTimeKey, config.ProtocolVer,
                expByteCount, expByteCount, expBuffer.data(), modByteCount, modByteCount, modBuffer.data());
        else
        {
            // XXX: Is there a reason to not send the protocol version when doing non-enc?
            NonEncSend("cdddbdb", AC_PROTOCOL_VER, oneTimeKey, config.ProtocolVer,
                expByteCount, expByteCount, expBuffer.data(), modByteCount, modByteCount, modBuffer.data());
        }
    }
    catch (const std::exception &)
    {
        CloseSocket();
        return;
    }
    AS_LOG_VERBOSE("SND: AC_PROTOCOL_VER,onetimekey:%d,buildVersion:%s", oneTimeKey, buildVersion);
    OnRead();
}

void CSocketServerEx::SetAddress(in_addr inADDR )
{
    addr = inADDR;
}

void CSocketServerEx::Send(const char* format, ...)
{
    if (mode == SM_CLOSE) {
        return;
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
        len = len -1 + config.PacketSizeType;
        
        logger.AddLog(LOG_DEBUG, "Sent %d bytes", len+3-config.PacketSizeType);
        int tmplen =     len+1-config.PacketSizeType;
        EncPacket( (unsigned char *)(buffer+2), EncOneTimeKey, tmplen );
#ifdef _DEBUG
        DumpPacket( (unsigned char*)buffer, len+3-config.PacketSizeType );
#endif
        len = tmplen - 1 + config.PacketSizeType;
        buffer[0] = static_cast<char>(len);
        buffer[1] = static_cast<char>(len >> 8);
        
        pBuffer->m_size = tmplen+2;

        Write(pBuffer);
        m_lastIO = GetTickCount();
    }
}

void CSocketServerEx::NonEncSend(const char* format, ...)
{
    if (mode == SM_CLOSE) {
        return;
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
        len = len + config.PacketSizeType;
        buffer[0] = static_cast<char>(len);
        buffer[1] = static_cast<char>(len >> 8);

    }
    pBuffer->m_size = len+3-config.PacketSizeType;
    logger.AddLog(LOG_DEBUG, "Sent %d bytes, Non Encrypt", len+3-config.PacketSizeType);
#ifdef _DEBUG
    DumpPacket( (unsigned char *)buffer, len+3-config.PacketSizeType );
#endif
    Write(pBuffer);
    m_lastIO = GetTickCount();
}

void CSocketServerEx::Send( const char *sendmsg, int msglen )
{
    if (mode == SM_CLOSE) {
        return;
    }

    CIOBuffer *pBuffer = CIOBuffer::Alloc();
    char *buffer = pBuffer->m_buffer;
    memcpy( buffer, sendmsg, msglen);
    logger.AddLog(LOG_DEBUG, "Sent %d bytes, EncKey 0x87546CA1, 0x%x", msglen, EncOneTimeKey);
#ifdef _DEBUG
    DumpPacket( (unsigned char *)buffer, msglen );
#endif
    int tmplen = msglen - 2;
    EncPacket( (unsigned char *)(buffer+2), EncOneTimeKey, tmplen );
    int len = tmplen - 1 + config.PacketSizeType;
    buffer[0] = static_cast<char>(len);
    buffer[1] = static_cast<char>(len >> 8);
    pBuffer->m_size = tmplen+2;
    Write(pBuffer);
}

void CSocketServerEx::OnTimerCallback( void )
{
    if (mode == SM_CLOSE) {
        ReleaseRef();
        return;
    }
    DWORD curTick = GetTickCount();
    if ( curTick - (DWORD)m_lastIO >= (DWORD)config.SocketTimeOut - (DWORD)500 ){
        CIOSocket::CloseSocket();
    } 
    ReleaseRef();
    
    return;
}
