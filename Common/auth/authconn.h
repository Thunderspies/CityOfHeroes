#ifndef _AUTHCONN_H
#define _AUTHCONN_H

// auth->dbserver
typedef enum AuthToDbserverCmd {
    SQ_ABOUT_TO_PLAY,                   // 0
    SQ_KICK_ACCOUNT,                    // 1
    SQ_SERVER_NUM,                      // 2
    SQ_VERSION,                         // 3
    SQ_PING,                            // 4
    SQ_COMPLETE_USERLIST,               // 5
#if 0           // obsolete Korean?
    SQ_PAYMENT_REMAIN_TIME,             // 6 - Korea?
    SQ_PAYMENT_CHANGE_TARGET,           // 7 - Korea?
#endif
    SQ_RECV_USER_DATA = 6,              // 6
    SQ_RECV_GAME_DATA = 7,              // 7
    SQ_MAX
} AuthToDbserverCmd;

// dbserver->auth
typedef enum DbserverToAuthCmd {
    AS_PLAY_OK,                                    // 0
    AS_PLAY_FAIL,                                // 1
    AS_PLAY_GAME,                                // 2
    AS_QUIT_GAME,                                // 3
    AS_KICK_ACCOUNT,                            // 4
    AS_SERVER_USER_NUM,                            // 5
    AS_BAN_USER,                                // 6
    AS_VERSION,                                    // 7
    AS_PING,                                    // 8
    AS_WRITE_USERDATA,                            // 9
    AS_SET_CONNECT,                                // 10
    AS_PLAY_USER_LIST,                            // 11

    // Auth 2
    AS_SET_SERVER_ID,                            // 12
    AS_SERVER_USER_NUM_BY_QUEUE,                // 13
    AS_FINISHED_QUEUE,                            // 14
    AS_SET_LOGIN_FREQUENCY,                        // 15
    AS_UNKNOWN,                                    // 16
    AS_READ_USER_DATA,                            // 17 auth data -- This is AS_READ_USERDATA in the authserver
    AS_WRITE_GAME_DATA,                            // 18 send coh-writable data -- This is AS_WRITE_GAMEDATA in the authserver
    AS_READ_GAME_DATA,                            // 19 req coh-writable data -- This is AS_READ_GAMEDATA in the authserver
    AS_SHARD_TRANSFER,                            // DGNOTE - request from "departure" dbserver for a shard transfer
    AS_MAX
} DbserverToAuthCmd; 

// client->auth
typedef enum ClientToAuthCmd {
    AQ_LOGIN,
    AQ_SERVER_LIST,
    AQ_ABOUT_TO_PLAY,
    AQ_LOGOUT,
    AQ_LOGIN_MD5,
    AQ_SERVER_LIST_EXT,
    AQ_LOGIN_MD5_MD5, // Reserved legacy request 6
    AQ_CREATE_ACCOUNT = 7,
} ClientToAuthCmd;

// auth->client
typedef enum AuthToClientCmd {
    AC_PROTOCOL_VER,                    // 0
    AC_LOGIN_FAIL,                      // 1
    AC_BLOCKED_ACCOUNT,                 // 2
    AC_LOGIN_OK,                        // 3
    AC_SEND_SERVER_LIST,                // 4
    AC_SEND_SERVER_FAIL,                // 5
    AC_PLAY_FAIL,                       // 6
    AC_PLAY_OK,                         // 7
    AC_ACCOUNT_KICKED,                  // 8
    AC_BLOCKED_ACCOUNT_WITH_MSG,        // 9
    AC_LOGIN_OK_WITH_MSG,               // 10 - not in auth2, remove after
//     AC_UNKNOWN_ERROR,                   // 11 - not in auth2, remove after
    
    // auth 2
    AC_SCCHECKREQ                       = 10, // This is specific for asia, whereby users enter there security card number and the system validates that they are old enough to play.  Not currently used or needed for US or Europe.
    AC_QUEUESIZE                        = 11,
    AC_HANDOFFTOQUEUE                   = 12,
    AC_POSITIONINQUEUE                  = 13,
    AC_HANDOFFTOGAME                    = 14,

    AC_CREATE_ACCOUNT_RESULT = 15,
    AuthToClientCmd_Count,
} AuthToClientCmd;


// reason codes for errors from auth
typedef enum AuthErrorCodes {
    S_ALL_OK                =  0,
    S_DATABASE_FAIL            =  1, // PresenceD Down, Database Down
    S_INVALID_ACCOUNT        =  2,
    S_INCORRECT_PWD            =  3,
    S_ACCOUNT_LOAD_FAIL        =  4,
    S_LOAD_SSN_ERROR        =  5,
    S_NO_SERVERLIST            =  6,
    S_ALREADY_LOGIN            =  7,
    S_SERVER_DOWN            =  8,
    S_INCORRECT_MD5Key        =  9,
    S_NO_LOGININFO            = 10, // If SessionId is not exists.
    S_KICKED_BY_WEB            = 11,
    S_UNDER_AGE                = 12,
    S_KICKED_DOUBLE_LOGIN    = 13,
    S_ALREADY_PLAY_GAME        = 14,
    S_LIMIT_EXCEED            = 15,
    S_SERVER_CHECK            = 16,
    S_MODIFY_PASSWORD        = 17,
    S_NOT_PAID                = 18,
    S_NO_SPECIFICTIME        = 19,
    S_SYSTEM_ERROR            = 20,
    S_ALREADY_USED_IP        = 21,
    S_BLOCKED_IP            = 22,
    S_VIP_ONLY                = 23,
    S_DELETE_CHRARACTER_OK    = 24,
    S_CREATE_CHARACTER_OK    = 25,
    S_INVALID_NAME            = 26,
    S_INVALID_GENDER        = 27,
    S_INVALID_CLASS            = 28,
    S_INVALID_ATTR            = 29, // Unknown Protocol ID
    S_MAX_CHAR_NUM_OVER        = 30,
    S_TIME_SERVER_LIMIT_EXCEED = 31,
    S_INVALID_USER_STATUS    = 32,
    S_INCORRECT_AUTHKEY        = 33,
} AuthErrorCodes;

int authdbg_printf(const char *format, ...);

#endif
