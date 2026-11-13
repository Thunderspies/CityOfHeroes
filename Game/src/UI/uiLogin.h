#ifndef UI_LOGIN_H
#define UI_LOGIN_H

// Keep these in the order in which they occur
typedef enum LoginStage {
    LOGIN_STAGE_START = 0,
    LOGIN_STAGE_EULA,
    LOGIN_STAGE_REJECT_EULA_NDA,
    LOGIN_STAGE_SERVER_SELECT,
    LOGIN_STAGE_NDA,
    LOGIN_STAGE_IN_QUEUE,
    LOGIN_STAGE_CHARACTER_SELECT,
    LOGIN_STAGE_LEAVING,
    LOGIN_STAGE_SHARD_JUMP_DBCONNECT,
    LOGIN_STAGE_SHARD_JUMP_CHARACTER_SELECT,
} LoginStage;

extern LoginStage s_loggedIn_serverSelected;

// Console entry point: create from current fields and report a result without logging in.
void loginCreateAccount(void);
// Cancel registration on departure/shutdown and clear its temporary credentials.
void loginCancelAccountCreation(void);
void loginMenu(void);
int loginToDbServer(int id,char *err_msg);
void respondToDbServerLogin(int idx, char *err_msg, char *server_name);
int loginToAuthServer(int retry);
void restartLoginScreen(void);
void restartCharacterSelectScreen(void);
void resetCharacterCreationFlag(void);
void createCharacter(void);
void resetVillainHeroDependantStuff(int changeCostume);
void setCharPage(int forward);

void renameCharacterRightAwayIfPossible(void);
void unlockCharacterRightAwayIfPossible(void);

extern bool characterRenameReceiveResult(char* resultMsg);
extern void characterUnlockReceiveResult(void);

extern bool charSelectDialog(char* resultMsg);
extern void charSelectRefreshCharacterList(void);

extern void startGRWebSwitch(void);

extern char g_achAccountName[32];
extern U8 g_achPassword[32]; // NOT A STRING!  This is the encrypted password.
extern int g_iDontSaveName;
extern char g_shardVisitorData[];
extern U32 g_shardVisitorDBID;
extern U32 g_shardVisitorUID;
extern U32 g_shardVisitorCookie;
extern U32 g_shardVisitorAddress;
extern U16 g_shardVisitorPort;

extern int gPlayerNumber;
extern int gServerNumber;
extern int gSelectedDbServer;

char *getAccountDir(char *buff);
void GRNagCancelledLogin(void);

#endif UI_LOGIN_H
