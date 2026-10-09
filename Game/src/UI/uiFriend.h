#ifndef UIFRIEND_H
#define UIFRIEND_H

void friendListPrepareUpdate();
void friendListReset();

void addToFriendsList(void *foo);
void removeFromFriendsList(void *foo);
int isFriend(void *foo);
int isNotFriend(void *foo);


void displayFriendsButton( float sx, float sy, float sz );
int  friendWindow();

void AddGlobalIgnore(char * handle, int db_id);
void UpdateGlobalIgnore(char * oldHandle, char * newHandle);
void RemoveGlobalIgnore(char * handle);
void RemoveAllGlobalIgnores();

void removeFromGlobalIgnoreList(void* useless);
void addToGlobalIgnoreList(void* useless);
int isNotGlobalIgnore(void* foo);
int isGlobalIgnore(void* foo);

// NEW GLOBAL FRIEND STUFF...

typedef enum {
    GFStatus_Offline,
    GFStatus_Online,
    GFStatus_Playing,
}GFStatus;

typedef struct GlobalFriend
{
    char        name[128];
    char        handle[128];
    char        shard[128];
    char        map[128];
    char        archetype[128];
    char        origin[128];
    char        powerset1[128];
    char        powerset2[128];
    int            xpLevel;
    int            teamSize;
    int            dbid;
    int            relativeDbId;    // if on same shard as player, == db_id, else == 0

    bool        onSameShard;    // cached value true if shard name matches client's shard name

    GFStatus    status;
}GlobalFriend;

extern GlobalFriend ** gGlobalFriends;

GlobalFriend * GetGlobalFriend(const char * handle);
GlobalFriend * GetGlobalFriendOnShardByPlayerName(const char * name);
GlobalFriend * GlobalFriendCreate(const char * handle, int db_id);
void GlobalFriendDestroy(GlobalFriend * gf);
void GlobalFriendDestroyAll();
void globalFriendListRemove(GlobalFriend * gf);

int isNotGlobalFriend(void *foo);
int isGlobalFriend(void* foo);
void removeFromGlobalFriendsList(void* useless);
void addToGlobalFriendsList(void* useless);

void channelWindowOpen( void * unused );
void selectChannelWindow(char * channel);


#define FRIEND_NAME_COLUMN            "FriendName"
#define FRIEND_STATUS_COLUMN        "Status"
#define FRIEND_MAP_COLUMN            "Map"
#define FRIEND_HANDLE_COLUMN        "Handle"
#define FRIEND_SHARD_COLUMN            "Shard"
#define FRIEND_LEVEL_COLUMN            "Level"
#define FRIEND_ARCHETYPE_COLUMN        "Archetype"
#define FRIEND_ORIGIN_COLUMN        "Origin"
#define FRIEND_PRIMARY_COLUMN        "Primary"
#define FRIEND_SECONDARY_COLUMN        "Secondary"
#define FRIEND_TEAMSIZE_COLUMN        "TeamSize"

#endif