#include <stdio.h>
#include <utilitieslib/utils/timing.h>
#include "auth/auth.h"
#include <utilitieslib/stdtypes.h>
#include <utilitieslib/network/sock.h>
#include <utilitieslib/network/netio.h>
#include <utilitieslib/network/net_socket.h>
#include <utilitieslib/utils/error.h>
#include <utilitieslib/utils/log.h>

typedef struct
{
    int            fake_it;
    __int64        send_key;
    __int64        recv_key;
    AuthPacket    send_pak;
    int            socket;
#if CLIENT
    int            connecting;
#endif
    U32            one_time_key;
    BLOWFISH_CTX    blowfish_ctx;
    U32            got_first_packet : 1;
    U32            got_bad_packet : 1;
    U32            global_auth : 1;
    int            packet_size_offset;
} AuthState;

int g_auth_always_check_first_packet;

AuthState auth_state;

#define PACKET_HEADER_SIZE 2

static void dumpData(const char *msg,const void *datav,int count)
{
#if 0
    static FILE    *file;
    int        i;
    unsigned char    *data = (unsigned char*)datav;

    if (!file)
        file = fopen("c:/temp/gameclient.txt","wt");

    fprintf(file,"%s%d:",msg,count);
    for(i=0;i<count;i++)
        fprintf(file," %d",data[i]);
    fprintf(file,"\n");
    fflush(file);
#endif
}


static void decrypt(unsigned char *buf, __int64 *key, int length)
{
    int i;

    char *strKey = (char *)key;
    char source;
    char next_source;

    source = buf[0];
    buf[0] = buf[0] ^ strKey[0];
    for (i = 1; i < length; i++) {
        next_source = buf[i];
        buf[i] = buf[i] ^ source ^ strKey[i & 7];
        source = next_source;
    }

    (*key)+=length;
}

static void encrypt(unsigned char *buf, __int64 *key, int length)
{
    int    i;
    char *strKey = (char *)key;

    buf[0] = buf[0] ^ strKey[0];
    for (i = 1; i < length; i++) {
        buf[i] = buf[i] ^ buf[i - 1] ^ strKey[i & 7];
    }

    (*key)+=length;
}

void BlowfishEncryptPacket(unsigned char *buf, int *lenp)
{
    int        i,len = *lenp;
    int blockLen;
    unsigned int checkSum = 0;
    unsigned int *intPtr = (unsigned int *)buf;

    len = (len + 7) & 0xfffffff8;   // Makes it multiple of 8.
    blockLen = len >> 2;
// As for the checksum, simple xor is proabably enough...
    for (i = 0; i < blockLen; ++i) {
        checkSum ^= intPtr[i];
    }
    intPtr[i] = checkSum;
    len += 8;
    cryptBlowfishEncrypt(&auth_state.blowfish_ctx,(unsigned char *)buf, len);
    *lenp = len;
}

bool BlowfishDecryptPacket(unsigned char *buf, int len)
{
    int i,blockLen = (len - 8) >> 2;
    unsigned int checkSum = 0;
    unsigned int *intPtr = (unsigned int *)buf;

    if (len >= 16 && (len & 0x7) == 0) {    // It's not a multiple of 8
        cryptBlowfishDecrypt(&auth_state.blowfish_ctx,(unsigned char *)buf, len);
        for (i = 0; i < blockLen; ++i) {
            checkSum ^= intPtr[i];
        }
        if (intPtr[i] == checkSum) {
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}

static void setupPacketSizeOffset(int len,int reference)
{
    // Hard-coding this to ensure that we can use variable-size handshake packets. 
    // TODO: Refactor the change, assuming that we get a sign-off from NOC.
    auth_state.packet_size_offset = 2;

/*    auth_state.got_first_packet = 1;
    if (len == reference+3)
    {
        auth_state.packet_size_offset = 2;
    }
    else if (len == reference)
        auth_state.packet_size_offset = -1;
    else
    {
        auth_state.got_bad_packet = 1;
    }
*/}

#if CLIENT

int authGetPacket(AuthPacket *pak)
{
    int        amt,sock = auth_state.socket;

    if (!pak->length)
    {
        amt = recv(sock, (char*)(pak->data + pak->bytesRead), PACKET_HEADER_SIZE - pak->bytesRead, 0);

        // If nothing can be retrieved from the socket, nothing else can be done.
        
        if (amt < 0 && WSAGetLastError() != WSAEWOULDBLOCK)
            return -1;
        else if (amt == 0)
            return -1;
        else if (amt < 0)
            return 0;

        // Otherwise, some data has been retrieved.
        pak->bytesRead += amt;

        // Did we get the full header?
        if(pak->bytesRead < PACKET_HEADER_SIZE)
            return 0;

        if (!auth_state.got_first_packet)
            setupPacketSizeOffset(*((U16 *) pak->data),8);
        pak->length = *((U16 *) pak->data) - auth_state.packet_size_offset;
        if (!pak->length || pak->length > sizeof(pak->data))
        {
            auth_state.got_bad_packet = 1;
            return -1;
        }
        pak->bytesRead = 0;
    }

    // Attempt to read the body of the data now.
    amt = recv(sock, (char*)(pak->data + pak->bytesRead), pak->length - pak->bytesRead,0);

    // Didn't get any data?
    // The packet is still only partially read.
    // Report nothing has been read yet.
    if (amt < 0 && WSAGetLastError() != WSAEWOULDBLOCK)
        return -1;
    else if (amt == 0)
        return -1;
    else if (amt < 0)
        return 0;

    pak->bytesRead += amt;

    // Did we get all the data for this packet?
    // If not, report nothing has been read yet.
    if (pak->bytesRead < pak->length)
        return 0;

    pak->bytesRead = 0;
    if (auth_state.global_auth)
    {
        if (!BlowfishDecryptPacket(pak->data,pak->length))
        {
            auth_state.got_bad_packet = 1;
            return -1;
        }
    }
    else
    {
        if(auth_state.recv_key)
            decrypt(pak->data,&auth_state.recv_key,pak->length);
    }
    return 1;
}

#else

enum{
    AARS_None,
    AARS_WaitingToStart,
    AARS_ExtractHeader,
    AARS_ExtractHeaderComplete,
    AARS_ExtractBody,
    AARS_ExtractBodyComplete,
} authAsyncReceiveState;

static AsyncOpContext authAsyncReceiveContext;
static AsyncOpContext authAsyncSendContext;

int authGetPacket(AuthPacket *pak)
{
    int        amt,sock = auth_state.socket;

    // Are we waiting to initiate some 
    if(AARS_None == authAsyncReceiveState || AARS_WaitingToStart == authAsyncReceiveState){
        authAsyncReceiveState = AARS_ExtractHeader;
    }
    
    // Did we retrieve the packet header yet?
    // If not, retrieve it now.
    if(AARS_ExtractHeader == authAsyncReceiveState){
        pak->bytesRead += authAsyncReceiveContext.bytesTransferred;
        authAsyncReceiveContext.bytesTransferred = 0;

        if(PACKET_HEADER_SIZE == pak->bytesRead){
            authAsyncReceiveState++;
            goto ExtractHeaderComplete;
        }
        amt = simpleWSARecv(sock, pak->data + pak->bytesRead, PACKET_HEADER_SIZE - pak->bytesRead, 0, &authAsyncReceiveContext);


        // If nothing can be retrieved from the socket, nothing else can be done.
        if(amt <= 0){
            int errVal = WSAGetLastError();
            return 0;
        }

        return 0;
    }


    if(AARS_ExtractHeaderComplete == authAsyncReceiveState){
ExtractHeaderComplete:
        // The entire packet header should have been read.
        // Otherwise, there is no reason for the async read operation to become "completed".
        //assert(pak->bytesRead == PACKET_HEADER_SIZE);
        //pak->bytesRead = 0;
        // Expected initial payload size is 12: 3 DWORDS, as defined in CSocketServer::OnCreate
        // in the auth server.
        if (!auth_state.got_first_packet)
            setupPacketSizeOffset(*((U16 *) pak->data),12);
        if(auth_state.got_bad_packet)
            return 0;

        pak->length = *((U16 *) pak->data) - auth_state.packet_size_offset;
        pak->bytesRead = 0;        // Allows the next read to read into the first byte of the packet data buffer.

        // Goto the next state now.
        authAsyncReceiveState++;
    }

    if(AARS_ExtractBody == authAsyncReceiveState){
        // Attempt to read the body of the data now.
        //assert((link->bytesToRead - link->bytesRead) > 0);
        pak->bytesRead += authAsyncReceiveContext.bytesTransferred;
        authAsyncReceiveContext.bytesTransferred = 0;

        if(pak->bytesRead == pak->length){
            authAsyncReceiveState++;
            goto ExtractBodyComplete;
        }

        amt = simpleWSARecv(sock, pak->data + pak->bytesRead, pak->length - pak->bytesRead, 0, &authAsyncReceiveContext);

        // Didn't get any data?
        // The packet is still only partially read.
        // Report nothing has been read yet.
        if(amt <= 0){
            int errVal = WSAGetLastError();
            return 0;
        }

        
        return 0;
    }


    if(AARS_ExtractBodyComplete == authAsyncReceiveState){
ExtractBodyComplete:
        // All data for this packet has been read.
        pak->bytesRead = 0;
        

        // Report that one packet has been read.
        authAsyncReceiveState = AARS_WaitingToStart;
        return 1;
    }

    // Execution never reaches this point.
    return 0;
}
#endif

int authSendPacket(AuthPacket *pak)
{
    int        amt,len = pak->length - PACKET_HEADER_SIZE,sock=auth_state.socket;

    if (auth_state.fake_it || !sock)
        return pak->length;

    if (auth_state.send_key)
    {
        if (auth_state.global_auth)
        {
            //memset(pak->data + pak->length,0,50);
            dumpData(" preblow ",pak->data + PACKET_HEADER_SIZE,48);
            BlowfishEncryptPacket(pak->data + PACKET_HEADER_SIZE,&len);
        }
        else
        {
            encrypt(pak->data + PACKET_HEADER_SIZE, &auth_state.send_key, len);
        }
    }
    pak->data[0] = (len+auth_state.packet_size_offset) & 255;
    pak->data[1] = ((len+auth_state.packet_size_offset) >> 8) & 255;
    len += PACKET_HEADER_SIZE;
    dumpData("postblow ",pak->data,len);
    amt = send(sock, (char*)pak->data, len, 0);
    if (amt != len)
    {
        LOG_OLD_ERR("Packet send failed to authserver");
        return 0;
    }
    return amt;
}


int authGetU08(AuthPacket *pak)
{
    if (pak->bytesRead < 0 || pak->bytesRead >= pak->length)
    {
        auth_state.got_bad_packet = 1;
        return 0;
    }
    return pak->data[pak->bytesRead++];
}

int authGetU16(AuthPacket *pak)
{
    U8        *ptr;
    U16        val;

    if (pak->bytesRead < 0 || pak->bytesRead + 2 > pak->length)
    {
        auth_state.got_bad_packet = 1;
        return 0;
    }
    ptr = &pak->data[pak->bytesRead];
    pak->bytesRead+=2;
    val = ptr[0] | (ptr[1] << 8);
    return val;
}

int authGetU32(AuthPacket *pak)
{
    U8        *ptr;
    U32        val;

    if (pak->bytesRead < 0 || pak->bytesRead + 4 > pak->length)
    {
        auth_state.got_bad_packet = 1;
        return 0;
    }
    ptr = &pak->data[pak->bytesRead];
    pak->bytesRead+=4;
    val = (U32)ptr[0] | ((U32)ptr[1] << 8) | ((U32)ptr[2] << 16) | ((U32)ptr[3] << 24);
    return val;
}

char *authGetStr(AuthPacket *pak)
{
    char    *s;

    if (pak->bytesRead < 0 || pak->bytesRead >= pak->length ||
        !memchr(pak->data + pak->bytesRead, 0, pak->length - pak->bytesRead))
    {
        auth_state.got_bad_packet = 1;
        return "";
    }
    s = (char*)&pak->data[pak->bytesRead];
    pak->bytesRead += (int)strlen(s) + 1;
    return s;
}

void *authGetArray(AuthPacket *pak,void *data,int len)
{
    if (len < 0 || pak->bytesRead < 0 || len > pak->length - pak->bytesRead)
    {
        auth_state.got_bad_packet = 1;
        return NULL;
    }
    memcpy(data,&pak->data[pak->bytesRead],len);
    pak->bytesRead += len;
    return data;
}

void authPutArray(AuthPacket *pak,void *data,int len)
{
    memcpy(&pak->data[pak->length],data,len);
    pak->length += len;
}

void authPutU08(AuthPacket *pak,U8 val)
{
    pak->data[pak->length++] = val;
}

void authPutU16(AuthPacket *pak,U16 val)
{
    authPutU08(pak,val & 255);
    authPutU08(pak,(val >> 8)& 255);
}

void authPutU32(AuthPacket *pak,U32 val)
{
    authPutU08(pak,val & 255);
    authPutU08(pak,(val >> 8)& 255);
    authPutU08(pak,(val >> 16)& 255);
    authPutU08(pak,(val >> 24)& 255);
}

void authPutStr(AuthPacket *pak,char *str)
{
    do
    {
        authPutU08(pak,*str);
    } while(*str++);
}

int authSocket()
{
    return auth_state.socket;
}

int authSocketOk()
{
    return !auth_state.got_bad_packet;
}

AuthPacket *authAllocPacket(int type)
{
    AuthPacket    *pak = &auth_state.send_pak;

    memset(pak,0,sizeof(*pak));
    pak->length = PACKET_HEADER_SIZE;
    authPutU08(pak,type);
    return pak;
}

void authDisconnect()
{
    int        got_first_packet = auth_state.got_first_packet,packet_size_offset = auth_state.packet_size_offset;

    if (auth_state.socket)
        closesocket(auth_state.socket);
    ZeroStruct(&auth_state);
#if !CLIENT
    authAsyncReceiveState = AARS_None;
    ZeroStruct(&authAsyncReceiveContext);
    ZeroStruct(&authAsyncSendContext);
#endif

    if (!g_auth_always_check_first_packet)
    {
        auth_state.got_first_packet = got_first_packet;
        auth_state.packet_size_offset = packet_size_offset;
    }
}

#if CLIENT
int authConnectStart(U32 address, int port)
{
    struct sockaddr_in destination;
    unsigned long nonblocking = 1;
    int error;

    if (auth_state.socket || !address || address == INADDR_NONE || port <= 0 || port > 65535)
        return -1;

    authDisconnect();
    auth_state.socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
    if (auth_state.socket == (int)INVALID_SOCKET)
    {
        auth_state.socket = 0;
        return -1;
    }
    if (ioctlsocket(auth_state.socket, FIONBIO, &nonblocking) != 0)
    {
        authDisconnect();
        return -1;
    }
    sockSetAddr(&destination, address, port);
    if (connect(auth_state.socket, (struct sockaddr *)&destination, sizeof(destination)) == 0)
        return 1;

    error = WSAGetLastError();
    if (error == WSAEWOULDBLOCK || error == WSAEINPROGRESS || error == WSAEALREADY)
    {
        auth_state.connecting = 1;
        return 0;
    }
    authDisconnect();
    return -1;
}

int authConnectPoll(void)
{
    fd_set writable, failed;
    struct timeval timeout = {0, 0};
    int ready, error = 0;
#ifdef _WIN32
    int errorBytes = sizeof(error);
#else
    socklen_t errorBytes = sizeof(error);
#endif

    if (!auth_state.socket)
        return -1;
    if (!auth_state.connecting)
        return 1;

    FD_ZERO(&writable);
    FD_ZERO(&failed);
    FD_SET(auth_state.socket, &writable);
    FD_SET(auth_state.socket, &failed);
    ready = select(auth_state.socket + 1, NULL, &writable, &failed, &timeout);
    if (ready == 0)
        return 0;
    if (ready < 0 || getsockopt(auth_state.socket, SOL_SOCKET, SO_ERROR, (char *)&error, &errorBytes) != 0 || error != 0)
    {
        authDisconnect();
        return -1;
    }
    auth_state.connecting = 0;
    return 1;
}
#endif

int authConnect(char *ip_name,int port)
{
    struct sockaddr_in addr;
    int        ret;
    static int timer;
    if (!timer)
        timer = timerAlloc();
    authDisconnect();
    auth_state.socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);

    if (strcmp(ip_name,"127.0.0.1")==0)
        sockSetAddr(&addr,inet_addr(ip_name),port);
    else
        sockSetAddr(&addr,ipFromString(ip_name),port);

    timerStart(timer);
    //    retry in a couple seconds
    while(1)
    {
        ret = connect(auth_state.socket,(struct sockaddr *)&addr,sizeof(addr));
        if (!ret)
            break;

        if (timerElapsed(timer) > 10)
            break;

        Sleep(250);
    }
    if (ret)
    {
        closesocket(auth_state.socket);
        auth_state.socket = 0;
        return 0;
    }
    sockSetBlocking(auth_state.socket,0);
#if SERVER|DBSERVER
    socketSetBufferSize(auth_state.socket, SO_SNDBUF, 1<<20);
    socketSetBufferSize(auth_state.socket, SO_RCVBUF, 1<<20);
#endif
    return 1;
}

void authSetKey(U32 key,int set_global_auth)
{
    auth_state.global_auth = set_global_auth;
    if (set_global_auth)
    {
        U8 blowFishKey[] = {
            0xa4, 0xde, 0x6b, 0x64, 0xff, 0x24, 0xad, 0x74, 0x52, 0xa0,
            0x6a, 0x35, 0xaf, 0xf5, 0x37, 0x11, 0xd3, 0x5a, 0xc8, 0x42
        };

        cryptBlowfishInit(&auth_state.blowfish_ctx,blowFishKey,sizeof(blowFishKey));
    }
    auth_state.one_time_key = key;
    if (key)
        auth_state.send_key = auth_state.recv_key = 0x87546CA100000000 | (__int64)key;
}

void authFakeIt()
{
    auth_state.fake_it = 1;
}

void authEncryptLineage2(const char * original, char * encoded, unsigned int encodeBlock)
{
    char passKey[16],pad=0;
    unsigned int len = (unsigned int)strlen(original);
    unsigned int i;

    strcpy(encoded, original);
    for(i = len; i < encodeBlock; ++i)
    {
        encoded[i] = '\0';
    }

    // TODO: conversion from char * to int * is possibly bad
    // depending on platform
    *((int *)passKey) = *((int *)encoded)*213119 + 2529077;
    *(((int *)passKey) + 1) = *(((int *)encoded) + 1)*213247 + 2529089;
    *(((int *)passKey) + 2) = *(((int *)encoded) + 2)*213203 + 2529589;
    *(((int *)passKey) + 3) = *(((int *)encoded) + 3)*213821 + 2529997;

    // the actual encryption
    for(i = 0; i < encodeBlock; ++i)
    {
        encoded[i] = encoded[i] ^ pad ^ passKey[i & 15];
        pad = encoded[i];
        if(encoded[i] == 0)
        {
            encoded[i] = 0x66;
        }
    }
}

U32 authGetKey()
{
    return auth_state.one_time_key;
}

int authIsGlobalAuth()
{
    return auth_state.global_auth;
}

BOOL g_authdbgprintf_enabled = 0;

int authdbg_printf(const char *format, ...)
{
    if(g_authdbgprintf_enabled)
    {
        char    date_buf[100];
        int result;
        va_list argptr;
        
        timerMakeDateString(date_buf);
        printf("%s ", date_buf);
        
        va_start(argptr, format);    
        result = vprintf(format, argptr); 
        va_end(argptr);
        
        return result;
    }
    else
    {       
        return 0;
    }
}

//If you add new protocol versions that also request specific versions, add them here
bool authRequestSpecificGroups(int version)
{
    if (version == OUROBOROS_PROTOCOL_VERSION_2) { return true; }
    return false;
}

