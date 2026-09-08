#ifndef _SOCK_H
#define _SOCK_H

#ifdef _WIN32
//#include <winsock2.h>
#include "../utils/wininclude.h"
#define s_addr S_un.S_addr
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <unistd.h>
typedef int SOCKET;
#define closesocket(x) close(x)
#define ioctlsocket(x,y,z) ioctl(x,y,(U32)z)
#define Sleep(x) usleep(x*1000)
#endif
#include "../stdtypes.h"

C_DECLARATIONS_BEGIN

void sockSetAddr(struct sockaddr_in *addr,unsigned int ip,int port);
int sockBind(SOCKET sock,const struct sockaddr_in *name);
void sockSetBlocking(SOCKET fd, int block);
void sockSetDelay(SOCKET fd, int delay);
int sockCheckWriteConnection(SOCKET fd);
void sockStart(void);
void sockStop(void);
int sockGetError(void);
char *makeIpStr(U32 ip);
char *makeHostNameStr(U32 ip);
int isLocalIp(U32 ip);
int setHostIpList(U32 ip_list[2]);
U32 getHostLocalIp(void);
U32 ipFromString(const char *s);
#define ipToString makeIpStr
int isDisconnectError(int errVal);
char* stringFromAddr(struct sockaddr_in *addr);

C_DECLARATIONS_END

#endif
