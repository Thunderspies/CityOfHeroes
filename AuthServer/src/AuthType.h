#ifndef _AUTHTYPE_H
#define _AUTHTYPE_H

#include "GlobalAuth.h"
#include "ServerId.h"
#include "Account.h"

#define MAX_IP_LEN 15
// In theory, the game client can accept up to 255 characters
// of shard name, but there's no reason to give it that much
// as it cannot render that much anyway.
#define MAX_SERVER_NAME 25

#define FREE_DURATION      0
#define PERSONAL_DURATION 1
#define PERSONAL_SPECIFIC 2
#define PERSONAL_POINT      3

enum UserMode {
    UM_PRE_LOGIN,
    UM_LOGIN,
    UM_PLAY_REQUEST,
    UM_PLAY_OK,
    UM_IN_GAME
};

typedef char ServerStatus;

class CSocketServer;

struct WorldServer
{
    ServerId serverid;
    char ip[255+1];
    char inner_ip[255+1];
    char name[MAX_SERVER_NAME+1];
    in_addr outer_addr;
    short int outer_port;
    in_addr inner_addr;
    char  ageLimit;
    unsigned char  pkflag;
    int   UserNum;
    int   maxUsers;
    CSocketServer *s;
    ServerStatus status;
    int region_id;
    int isVIP;
};

typedef struct LoginUser
{
    time_t  queuetime;
    time_t  logintime; 
    int        md5key;       
    in_addr loginIp;   
    int        stat;      
    ServerId serverid;  
    SOCKET  s;           
    UserMode um_mode;  
    char    account[MAX_ACCOUNT_LEN+1]; 
    HANDLE  timerHandle;
    char    gender;
    int        ssn; 
    int        ssn2;
    int     loginflag;
    int        warnflag; 
    char    age; 
    short int    cdkind;
    ServerId lastworld;
    ServerId selectedServerid;
    int     queueLevel;
    int     loyalty;
    int     loyaltyLegacy;
    int regions[MAX_REGIONS]; // Would much rather make this a std::vector, but this struct is used in functions that can't allow object unwinding.  Grrr.
} LoginUser;

class iless {
public:
    bool operator()(const std::string& a, const std::string& b) const {
        return _stricmp(a.c_str(), b.c_str()) < 0;
    }
    bool operator()(const char *a, const std::string& b) const {
        return _stricmp(a, b.c_str()) < 0;
    }
    bool operator()(const std::string& a, const char *b) const {
        return _stricmp(a.c_str(), b) < 0;
    }
};

typedef struct world_user_num {
    
    char serverid;
    int    limituser;
    int    playuser;

} WorldUserNum;

typedef std::map<int, LoginUser> UserMap;
typedef std::map<int, LoginUser * > UserPointerMap;
typedef std::map<int, int > SESSIONMAP;
typedef void (*EncPwdType)(char *);

extern EncPwdType EncPwd;

#endif
