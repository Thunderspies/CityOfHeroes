#include "utilitieslib/stdtypes.h"
#include "utilitieslib/utils/error.h"
#include "utilitieslib/network/sock.h"
#include "utilitieslib/utils/timing.h"
#include <string.h>
#include <stdio.h>
#include <ws2tcpip.h>
#include "utilitieslib/utils/SuperAssert.h"
#include "utilitieslib/utils/utils.h"
#include "utilitieslib/utils/endian.h"
#include "utilitieslib/utils/strings_opt.h"
#include "utilitieslib/utils/log.h"

#ifdef _WIN32
#include "utilitieslib/utils/wininclude.h"
#endif

void    sockSetAddr(struct sockaddr_in *addr,unsigned int ip,int port)
{
     memset(addr,0,sizeof(struct sockaddr_in));
     addr->sin_family=AF_INET;
     addr->sin_addr.s_addr=ip;
     addr->sin_port = htons((u_short)port);
}

int    sockBind(SOCKET sock,const struct sockaddr_in *name)
{
    if (bind (sock,(struct sockaddr *) name, sizeof(struct sockaddr_in)) >= 0) return 1;

    return 0;
}

void    sockSetBlocking(SOCKET fd,int block)
{
u_long     noblock;

    noblock = !block;
    ioctlsocket (fd, FIONBIO, &noblock);
}

void sockSetDelay(SOCKET fd, int delay)
{
    int noDelay;

    noDelay = !delay;

    setsockopt(fd,IPPROTO_TCP,TCP_NODELAY,(void *)&noDelay,sizeof(noDelay));
}

// Poll a non-blocking socket to see if it's writeable.  This is used by the asynchronous connect tech in net_link.c
int sockCheckWriteConnection(SOCKET fd)
{
    // This is total boilerplate code .....
    int ready;
    fd_set fds;
    struct timeval timeout;

    // init the fd set
    FD_ZERO(&fds);
    // and add the one socket (fd in "Berkeley-speak") we care about
    FD_SET(fd, &fds);

    // Timeout zero - this is an insta-poll
    timeout.tv_sec = 0;
    timeout.tv_usec = 0;

    // Check via writable fds for a successful connection.  ready tells us how many sockets were writeable.
    ready = select(1, NULL, &fds, NULL, &timeout);
    if (ready == 1)
    {
        // FD_ISSET should never fail
        return FD_ISSET(fd, &fds) ? 1 : -1;
    }

    // Otherwise reset everything
    FD_ZERO(&fds);
    FD_SET(fd, &fds);

    timeout.tv_sec = 0;
    timeout.tv_usec = 0;

    // Check via error fds for an aborted connection.
    ready = select(1, NULL, NULL, &fds, &timeout);
    if (ready == 1)
    {
        return -1;
    }

    return 0;
}

void    sockStart(void)
{
#if defined(_WIN32)
    static bool bAlreadyCalled;

    WORD wVersionRequested;  
    WSADATA wsaData; 
    int err; 

    if ( bAlreadyCalled )
        return;
    wVersionRequested = MAKEWORD(2, 2); 
 
    err = WSAStartup(wVersionRequested, &wsaData); 

    if (err)
        printf_stderr("Winsock error..");
    else
        bAlreadyCalled = true;
#endif
}


void sockStop(void){
#if defined(_WIN32)
    WSACleanup();
#endif
}

char *makeIpStr(U32 ip)
{
    // Looping buffer of 4 entries since this might get called in another thread
    // Yes, not "thread-safe", but since it's only used in debugging logging functions,
    //  it's as good as we need.
    static    char    buf[5][64],idx;
    char            *s;

    s = buf[idx++];
    if (idx >= ARRAY_SIZE(buf)-1)
        idx = 0;
    if (isBigEndian())
        ip = endianSwapU32(ip);
    sprintf_s(s, ARRAY_SIZE(buf[idx]),"%d.%d.%d.%d",ip&255,(ip>>8)&255,(ip>>16)&255,ip>>24);
    return s;
}

char *makeHostNameStr(U32 ip)
{
    struct hostent    *host_ent;
    U32        num_ip;
    char    *s;
    U32        start=0;
    static    int state=0;

    if (!state) {
        start = timerSecondsSince2000();
    }

    s = makeIpStr(ip);
    if (state==-1)
        return s;
    num_ip = inet_addr(s);

    host_ent = gethostbyaddr((char*)&num_ip,4,AF_INET);

    if (!state) {
        if (timerSecondsSince2000() - start > 5) {
            // More than 5 seconds to do a host lookup!  Too slow!
            state = -1;
        } else {
            state = 1;
        }
    }

    if (!host_ent)
        return s;
    return host_ent->h_name;
}

// sorry for the Windows-specific code, future code porters -Pazaz
#ifdef _WIN32
static PMIB_IPADDRTABLE pIPAddrTable;
#endif

// From RFC 1918
//   The Internet Assigned Numbers Authority (IANA) has reserved the
//   following three blocks of the IP address space for private internets:
//
//     10.0.0.0        -   10.255.255.255  (10/8 prefix)
//     172.16.0.0      -   172.31.255.255  (172.16/12 prefix)
//     192.168.0.0     -   192.168.255.255 (192.168/16 prefix)
//     127.0.0.1                           (localhost)
int isLocalIp(U32 ip)
{
    U8    a,b,c,d;
#ifdef _WIN32
    int i = 0;
    DWORD dwSize = 0;
    DWORD dwRetVal = 0;
    LPVOID lpMsgBuf;
#endif

    a = ip & 255;
    b = (ip >> 8) & 255;
    c = (ip >> 16) & 255;
    d = (ip >> 24) & 255;
    if (a == 10)
        return 1;
    if (a == 172 && (b >= 16 && b <= 31))
        return 1;
    if (a == 192 && b == 168)
        return 1;
    if (a == 127 && b == 0 && c == 0 && d == 1)
        return 1;

#ifdef _WIN32
    if (!pIPAddrTable) {
        if (GetIpAddrTable(pIPAddrTable, &dwSize, 0) == ERROR_INSUFFICIENT_BUFFER) {
            free(pIPAddrTable);
            pIPAddrTable = (MIB_IPADDRTABLE*)malloc(dwSize);
        }
        if (pIPAddrTable == NULL) {
            printf("Memory allocation failed for GetIpAddrTable\n");
            return 0;
        }

        if ((dwRetVal = GetIpAddrTable(pIPAddrTable, &dwSize, 0)) != NO_ERROR) {
            printf("GetIpAddrTable failed with error %d\n", dwRetVal);
            if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, dwRetVal, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                (LPTSTR)& lpMsgBuf, 0, NULL)) {
                printf("\tError: %s", lpMsgBuf);
                LocalFree(lpMsgBuf);
            }
            return 0;
        }
    }

    for (i = 0; i < (int)pIPAddrTable->dwNumEntries; i++) {
        if (pIPAddrTable->table[i].dwAddr == ip) {
            return 1;
        }
    }
    
    /* if memory leaks occur, add this in and make sure it gets run
    if (pIPAddrTable) {
        free(pIPAddrTable);
        pIPAddrTable = NULL;
    }*/
#endif

    return 0;
}

int setHostIpList(U32 ip_list[2])
{
    struct addrinfo hints;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;

    struct addrinfo *addr;
    int result = getaddrinfo("localhost", NULL, &hints, &addr);
    if (result != 0)
    {
        printf("setHostIpList, %s, %i: %d", __FILE__, __LINE__, result);
        return 0;
    }

    //
    devassert(addr->ai_family == AF_INET);
    struct sockaddr_in* addr41 = (struct sockaddr_in*)(addr->ai_addr);
    int ip_remote = addr41->sin_addr.S_un.S_addr;
    
    char str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(addr41->sin_addr), str, INET_ADDRSTRLEN);

    //
    addr = addr->ai_next;
    int ip_local = ip_remote;
    if (addr)
    {
        devassert(addr->ai_family == AF_INET);
        struct sockaddr_in* addr42 = (struct sockaddr_in*)(addr->ai_addr);
        ip_local = addr42->sin_addr.S_un.S_addr;

        char str2[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(addr42->sin_addr), str2, INET_ADDRSTRLEN);

        if (isLocalIp(ip_remote))
        {
            int tmp = ip_remote;
            ip_remote = ip_local;
            ip_local = tmp;
        }
    }

    ip_list[0] = ip_local;
    ip_list[0] = ip_remote;

    //
    freeaddrinfo(addr);

    return 1;
}

U32 getHostLocalIp()
{
    const U32 kSecBeforeRefresh = 60;
    static U32 ip_list[2];
    static U32 cachedAt = 0;

    if(!cachedAt || (timerSecondsSince2000() - cachedAt) > kSecBeforeRefresh) {
        if (!setHostIpList(ip_list)) {
            cachedAt = 0;
            return 0;
        }
    }

    cachedAt = timerSecondsSince2000();
    return ip_list[0];
}

U32 ipFromString(const char *s)
{
    char    ip_str[100];

    strcpy(ip_str,s);
    if (stricmp(s,"localhost")==0 || stricmp(s,"127.0.0.1")==0)
        strcpy(ip_str,makeIpStr(getHostLocalIp()));
    else if (!isdigit((unsigned char)s[0]))
    {
        struct hostent *hostent=0;
        hostent = gethostbyname(s);
        if (!hostent)
            printWinErr("ipFromString", __FILE__, __LINE__, WSAGetLastError());
        else
            sprintf_s(SAFESTR(ip_str),"%d.%d.%d.%d",(U8)hostent->h_addr_list[0][0],(U8)hostent->h_addr_list[0][1],
                                        (U8)hostent->h_addr_list[0][2],(U8)hostent->h_addr_list[0][3]);
    }
    return inet_addr(ip_str);
}

int isDisconnectError(int errVal){ 
    if(WSAECONNRESET == errVal || WSAECONNABORTED == errVal || WSAENETRESET == errVal){
        LOG(LOG_NET, LOG_LEVEL_DEBUG, 0, "disconnect error %i:%s", errVal, lastWinErr());
        return 1;
    } else if (errVal == 10040) {
        // "A message sent on a datagram socket was larger than the internal message
        // buffer or some other network limit, or the buffer used to receive a
        // datagram into was smaller than the datagram itself."

        // The other end sent us a packet bigger than our network layer accepts, must be spoofed or garbage!
        // Just disconnect the link!
        return 1;
    } else
        return 0;
}

char* stringFromAddr(struct sockaddr_in *addr){
    static char buffer[128];
    sprintf_s(SAFESTR(buffer), "%i.%i.%i.%i:%i", 
            addr->sin_addr.S_un.S_un_b.s_b1,
            addr->sin_addr.S_un.S_un_b.s_b2,
            addr->sin_addr.S_un.S_un_b.s_b3,
            addr->sin_addr.S_un.S_un_b.s_b4,
            addr->sin_port);
    return buffer;
}

int sockGetError(void){
    return WSAGetLastError();
}
