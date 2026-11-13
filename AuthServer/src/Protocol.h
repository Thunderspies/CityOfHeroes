#ifndef _PROTOCOL_H
#define _PROTOCOL_H
#include "GlobalAuth.h"
// when client send to auth
#define PROTOCOL_VERSION 1

// This must match AUTH_BYTES in auth.h
#define MAX_USERDATA 128
// This must match AUTH_BYTES_ORIG in auth.h
#define MAX_USERDATA_ORIG 16
#define MAX_USERDATA_NEW (MAX_USERDATA - MAX_USERDATA_ORIG)

enum {
    AQ_LOGIN,
    AQ_SERVER_LIST,
    AQ_ABOUT_TO_PLAY,
    AQ_LOGOUT,
    AQ_LOGIN_MD5,
    AQ_SERVER_LIST_EXT, // Server List kind
    AQ_LOGIN_MD5_MD5 = 6, // Reserved legacy request
    AQ_CREATE_ACCOUNT = 7,
    AQ_MAX,
};

// when auth send to server
enum {
    SQ_ABOUT_TO_PLAY,
    SQ_KICK_ACCOUNT,
    SQ_SERVER_NUM,
    SQ_VERSION,
    SQ_PING,
    SQ_COMPLETE_USERLIST,
    SQ_USER_DATA,
    SQ_GAME_DATA, // ajackson - to send game data about a UID
    SQ_MAX
};

// when auth send to client
enum{
    AC_PROTOCOL_VER,
    AC_LOGIN_FAIL,
    AC_BLOCKED_ACCOUNT,
    AC_LOGIN_OK,
    AC_SEND_SERVERLIST,
    AC_SEND_SERVER_FAIL,
    AC_PLAY_FAIL,
    AC_PLAY_OK,
    AC_ACCOUNT_KICKED, // account kick : char(error_code)
    AC_BLOCKED_ACCOUNT_WITH_MSG,
    AC_SC_CHECK_REQ,
    AC_QUEUE_SIZE,
    AC_HANDOFF_TO_QUEUE,
    AC_POSITION_IN_QUEUE,
    AC_HANDOFF_TO_GAME = 14, // Reserved legacy response
    AC_CREATE_ACCOUNT_RESULT = 15,
    AC_MAX
};

// return value to client ( error_reason_code ) AC_LOGIN_FAIL
enum {
    S_IP_ALL_OK = 0,
};
enum {
    S_ALL_OK,             // no error  //0
    S_DATABASE_FAIL,     // fail to fetch password data or something bad situation takes place at auth database server // 1
    S_INVALID_ACCOUNT,   // no account //2
    S_INCORRECT_PWD,     // incorrect password //3
    S_ACCOUNT_LOAD_FAIL, // This account exist in user_auth Table , but not user_account Table //4
    S_LOAD_SSN_ERROR,    // fail to load ssn //5
    S_NO_SERVERLIST,     // somthing wrong happens on server table in lin2db database //6
    S_ALREADY_LOGIN,
    S_SERVER_DOWN,
    S_INCORRECT_MD5Key,
    S_NO_LOGININFO,
    S_KICKED_BY_WEB,
    S_UNDER_AGE,
    S_KICKED_DOUBLE_LOGIN,
    S_ALREADY_PLAY_GAME,
    S_LIMIT_EXCEED,
    S_SERVER_CHECK,
    S_MODIFY_PASSWORD,
    S_NOT_PAID,
    S_NO_SPECIFICTIME,
    S_SYSYTEM_ERROR,
    S_ALREADY_USED_IP,
    S_BLOCKED_IP,
    S_VIP_ONLY,
};

// AS_QUIT_GAME Reason
enum {
    S_QUIT_NORMAL=0,
};

// when world server to auth server
enum{
    AS_PLAY_OK,
    AS_PLAY_FAIL,
    AS_PLAY_GAME,
    AS_QUIT_GAME,
    AS_KICK_ACCOUNT,
    AS_SERVER_USERNUM,
    AS_BAN_USER,
    AS_VERSION,
    AS_PING,
    AS_WRITE_USERDATA,
    AS_SET_CONNECT,
    AS_PLAY_USER_LIST,
    AS_SET_SERVER_ID,
    AS_SERVER_USER_NUM_BY_QUEUE_LEVEL,
    AS_FINISHED_QUEUE,
    AS_SET_LOGIN_FREQUENCY,
    AS_QUEUE_SIZES,
    AS_READ_USERDATA,  // ajackson Sept. 30, 2008 - a read user data -- This is AS_READ_USER_DATA in the dbserver
    AS_WRITE_GAMEDATA, // ajackson Sept. 30, 2008 - a write game data -- This is AS_WRITE_GAME_DATA in the dbserver
    AS_READ_GAMEDATA,  // ajackson Sept. 30, 2008 - a read game data -- This is AS_READ_GAME_DATA in the dbserver
    AS_SHARD_TRANSFER, // DGNOTE - request from "departure" dbserver for a shard transfer
    AS_MAX
};

enum COUNTRY {
    CC_KOREA,
    CC_JAPAN,
};

enum {
    IA_SERVER_VERSION, //0
    IA_IP_KIND,
    IA_IP_USE,    // return_code, uid, ip, 
    IA_IP_START_OK,
    IA_IP_START_FAIL,
    IA_IP_USE_FAIL,
    IA_IP_SESSIONKEY, //6
    IA_IP_INSTANTLOGIN_OK,
    IA_IP_INSTANTLOGIN_FAIL,
    IA_IP_KICK,
    IA_IP_READY_FAIL,
    IA_IP_READY_OK,
    IA_IP_SET_STARTTIME_OK,
    IA_IP_SET_STARTTIME_FAIL,
    IA_MAX,
};

enum {
    AI_SERVER_VERSION,
    AI_IP_KIND,
    AI_IP_ACQUIRE,
    AI_IP_RELEASE,
    AI_IP_START_CHARGE,
    AI_IP_STOP_CHARGE,
    AI_IP_INSTANT_START_GAME,
    AI_IP_INSTANT_STOP_GAME,
    AI_IP_KICKED,
    AI_IP_READY_GAME,
    AI_IP_SET_START_TIME,
    AI_MAX,
};

// IA_IP_USE_FAIL, AI_IP_START_GAME
enum {
    IP_ALL_OK,             //0 no error 
    IP_DB_ERROR,             //1
    IP_ALREADY_USE,         //2
    IP_LIMIT_OVER,         //3
    IP_TIME_OUT,
    IP_NOT_EXIST,
    IP_NOT_SUBSCRIBED,
    IP_SESSION_NOT_EXIST,
    IP_UNKNOWN_KIND,
    IP_SESSION_CREATE_FAIL,
    IP_SERVER_SOCKET_FAIL,
    IP_ALREADY_WAIT,
};

enum {
    AW_START,
    AW_QUIT,
    AW_MAX,
};

enum {
    WA_VERSION,
    WA_SEND_OK,
    WA_SEND_FAIL,
    WA_MAX,
};

#endif
