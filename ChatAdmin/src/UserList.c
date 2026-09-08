#include "UserList.h"
#include "ChatAdmin.h"
#include "ChatAdminNet.h"
#include <utilitieslib/components/earray.h>
#include "resource.h"
#include <utilitieslib/utils/ListView.h>
#include "VirtualListView.h"
#include <utilitieslib/utils/textparser.h>
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/utils/mathutil.h>
#include "CustomDialogs.h"
#include "chatdb.h"
#include "ChannelMonitor.h"
#include <stdlib.h>
#include <utilitieslib/utils/utils.h>

typedef enum FilterStatusType{

    FILTER_ALL = 0,
    FILTER_ONLINE,
    FILTER_OFFLINE,

} FilterStatusType;

StashTable gHandleToUserHash;

char             gUserListFilter[200];
int                 gUserListFilterLength;    // cached for faster filtering

FilterStatusType gUserListStatus;

BOOL    gUserListVisible;

HWND g_hWndUserList;

VListView * lvUserList;


void UserListUpdateMatchCount()
{
    static int s_last = -1;
    int count = vListViewGetFilteredSize(lvUserList);
    if(count != s_last)
    {
        if(count != vListViewGetSize(lvUserList))
        {
            char buf[100];
            itoa(count, buf, 10);
            SetDlgItemText(g_hWndUserList, IDC_STATIC_USERLIST_MATCH_COUNT, buf);
            SetDlgItemText(g_hWndUserList, IDC_STATIC_USERLIST_MATCH_CAPTION, localizedPrintf("MatchesCaption"));
        }
        else
        {
            SetDlgItemText(g_hWndUserList, IDC_STATIC_USERLIST_MATCH_COUNT, "");
            SetDlgItemText(g_hWndUserList, IDC_STATIC_USERLIST_MATCH_CAPTION, "");
        }
    }
    s_last = count;
}



static int compareUserHandle(const void * a, const void * b)
{
    CAUser * userA = *((CAUser**)a);
    CAUser * userB = *((CAUser**)b);

    return stricmp(userA->handle, userB->handle);
}

static int compareUserOnline(const void * a, const void * b)
{
    CAUser * userA = *((CAUser**)a);
    CAUser * userB = *((CAUser**)b);

    if(userA->online < userB->online)
        return 1;
    else if(userA->online > userB->online)
        return -1;
    else 
        return stricmp(userA->handle, userB->handle);
}

static int compareUserSilenced(const void * a, const void * b)
{
    CAUser * userA = *((CAUser**)a);
    CAUser * userB = *((CAUser**)b);

    if(userA->silenced < userB->silenced)
        return 1;
    else if(userA->silenced > userB->silenced)
        return -1;
    else 
        return stricmp(userA->handle, userB->handle);
}

char * displayUserHandle(void* userData)
{
    CAUser * user = (CAUser *)userData;
    return user->handle;
}

char * displayUserOnline(void* userData)
{
    CAUser * user = (CAUser *)userData;
    static char s_buf[100];
    if(user->online)
        sprintf(s_buf, localizedPrintf("CAStatusOnline"));
    else
        s_buf[0] = 0;
    return s_buf;
}

char * displayUserSilenced(void* userData)
{
    CAUser * user = (CAUser *)userData;
    static char s_time[100];
    
    U32 time = timerSecondsSince2000();

    if(user->silenced <= time)
    {
        user->silenced = 0;
        return "";
    }
    else
    {
        int mins = (user->silenced - time) / 60;
        if(mins >= 60)
            sprintf(s_time, "%d:%02d", mins/60, mins % 60);
        else
            sprintf(s_time, "0:%02d", mins);
        return s_time;
    }
}

VirtualListViewEntry CAUserInfo[] = 
{
    { "CAColumnHandle",        100,    compareUserHandle,        displayUserHandle},
    { "CAColumnStatus",        50,        compareUserOnline,        displayUserOnline},
    { "CAColumnSilenced",    100,    compareUserSilenced,    displayUserSilenced},
    { 0 }
};


static CAUser * CAUserCreate(char * handle)
{
    CAUser * user = calloc(sizeof(CAUser),1);
    
    user->handle        = strdup(handle);
    user->filterHandle    = strdup(handle);    

    stripNonAlphaNumeric(user->filterHandle);

    return user;
}

static void CAUserDestroy(CAUser * user)
{
    free(user->filterHandle);
    free(user->handle);
    free(user);
}



CAUser * CAUserAdd(char * handle, bool online, U32 silenced, bool sortAndFilter)
{
    CAUser * user = GetUserByHandle(handle);
    
    if(user)
        CAUserRemove(user);

    user = CAUserCreate(handle);

    user->online = online;
    user->silenced = timerSecondsSince2000() + (silenced * 60);

    stashAddPointer(gHandleToUserHash, handle, user, false);

    vListViewInsert(lvUserList, user, sortAndFilter);

    if(sortAndFilter)
    {
        UserListUpdateCount();
        UserListUpdateMatchCount();
    }

    return user;
}


void CAUserRemove(CAUser * user)
{
    if(user)
    {
        stashRemovePointer(gHandleToUserHash, user->handle, NULL);

        if ( vListViewRemove(lvUserList, user) != -1 )
            CAUserDestroy(user);

        UserListUpdateCount();
        UserListUpdateMatchCount();
    }
}

static void CAUserDestroyAdapter(void* arg0)
{
    CAUserDestroy((CAUser *)arg0);
}

void CAUserRemoveAll()
{
    stashTableClearEx(gHandleToUserHash, NULL, CAUserDestroyAdapter);
    vListViewRemoveAll(lvUserList, 0);    
    UserListUpdateCount();
    UserListUpdateMatchCount();
}


void CAUserRename(CAUser * user, char * handle)
{    
    int len = (int)strlen(handle) + 1;

    vListViewRemove(lvUserList, user);
    stashRemovePointer(gHandleToUserHash, user->handle, NULL);

    user->handle = realloc(user->handle, len);
    user->filterHandle = realloc(user->filterHandle, len);
    strcpy(user->handle, handle);
    strcpy(user->filterHandle, handle);

    stripNonAlphaNumeric(user->filterHandle);

    stashAddPointer(gHandleToUserHash, handle, user, false);
    vListViewInsert(lvUserList, user, 1);

    ChanMonUpdateUser(user);
}

// notify list view that we've changed
void CAUserUpdate(CAUser * user)
{
    vListViewRefreshItem(lvUserList, user);
}


void CAUserColor(void* userData, COLORREF* pTextColor, COLORREF* pBkColor)
{
    CAUser * user = (CAUser *)userData;
    if(user == gAdminClient.user)
    {
        *pTextColor = RGB(255,0,0);//255,255);
        *pBkColor = RGB(255,255,255);
    }
    else
    {
        *pTextColor = RGB(0,0,0);
        *pBkColor = RGB(255,255,255);
    }
}

int CAUserFilter(void* userData)
{
    CAUser * user = (CAUser *)userData;
     if(gUserListStatus)
    {
        if(gUserListStatus == FILTER_OFFLINE)
        {
            if(user->online)
                return -1;
        }
        else if(gUserListStatus == FILTER_ONLINE)
        {
            if(! user->online)
                return 1;
        }
    }
    if(gUserListFilterLength)
        return strncmp(user->filterHandle, gUserListFilter, gUserListFilterLength);
    else
        return 0; // match
}



CAUser * GetUserByHandle(char * handle)
{
    CAUser* pResult;
    if (stashFindPointer(gHandleToUserHash, handle, &pResult))
        return pResult;
    return NULL;
}


//-------------------------------------------------------------
// User Status List View
//-------------------------------------------------------------

ListView * lvUserStatus;

typedef struct { 

    char * category;
    char * desc;

}UserStatusEntry;

UserStatusEntry * UserStatusEntryCreate(char * category, const char * desc)
{
    UserStatusEntry * e = calloc(sizeof(UserStatusEntry), 1);

    e->category = strdup(category);
    e->desc = strdup(desc);

    return e;
}

void UserStatusEntryDestroy(UserStatusEntry * e)
{
    free(e->category);
    free(e->desc);
    free(e);
}

TokenizerParseInfo ParseUserStatus[] =
{
    { "",                        TOK_STRING(UserStatusEntry, category, 0), 0,    TOK_FORMAT_LVWIDTH(60)},
    { "Value",                    TOK_STRING(UserStatusEntry, desc, 0), 0,        TOK_FORMAT_LVWIDTH(120)},
    { 0, }
};


static void UserStatusEntryDestroyAdapter(void* arg0)
{
    UserStatusEntryDestroy((UserStatusEntry *)arg0);
}

void UserListStatusUpdate(char * handle, char * auth_id, char * shard, char * silencedMins, char *args[], int count, char *channels[], int channel_count)
{
    int i;
    int mins = atoi(silencedMins);

    listViewDelAllItems(lvUserStatus, UserStatusEntryDestroyAdapter);

    listViewAddItem(lvUserStatus, UserStatusEntryCreate("Handle", handle));

    listViewAddItem(lvUserStatus, UserStatusEntryCreate("Auth ID", auth_id));

    if(mins > 0)
    {
        char time[100];
        if(mins >= 60)
            sprintf(time, "%d hours, %d mins", mins/60, mins % 60);
        else
            sprintf(time, "%d mins", mins);
        listViewAddItem(lvUserStatus, UserStatusEntryCreate("Silenced", time));
    }

    if(! count)
    {
        listViewAddItem(lvUserStatus, UserStatusEntryCreate("Status", "Offline"));
    }
    else if(count >= 7)
    {
        listViewAddItem(lvUserStatus, UserStatusEntryCreate("Player", args[0]));
        listViewAddItem(lvUserStatus, UserStatusEntryCreate("Shard", shard));
        listViewAddItem(lvUserStatus, UserStatusEntryCreate("Map", args[2]));
        listViewAddItem(lvUserStatus, UserStatusEntryCreate("XP Level", args[6]));
        listViewAddItem(lvUserStatus, UserStatusEntryCreate("Team Size", args[5]));
        listViewAddItem(lvUserStatus, UserStatusEntryCreate("DB ID", args[1]));
    }
//    else if(count == 1 && !stricmp(args[0], "Online"))
    else
    {
        listViewAddItem(lvUserStatus, UserStatusEntryCreate("Status", "Online"));
    }

    for (i = 0; i < channel_count; i++)
    {
        listViewAddItem(lvUserStatus, UserStatusEntryCreate("Channel", unescapeString(channels[i])));
    }
}




void UserListInit()
{
    if(gHandleToUserHash)
        stashTableDestroy(gHandleToUserHash);
    gHandleToUserHash = stashTableCreateWithStringKeys(10000, StashDeepCopyKeys);
}




void onAcceptRenameUser(char * oldHandle, char * newHandle)
{
    chatCmdSendf("CsrName", "%s %s", oldHandle, newHandle);
    free(oldHandle);
}




void onSilenceUserAccept(void* oldHandleData, int minutes)
{
    char * oldHandle = (char *)oldHandleData;
    chatCmdSendf("CsrSilence", "%s %d", oldHandle, minutes);
    free(oldHandle);
}


void UserListUpdateCount()
{
    if(gUserListVisible && gHandleToUserHash)
    {
        int size = stashGetValidElementCount(gHandleToUserHash);
        if(size)
            setStatusBar(3, localizedPrintf("CAUserCount", size));
        else
            setStatusBar(3, "");
    }
}

void UserListReset()
{
    CAUserRemoveAll();
}

void onUserListSelected(CAUser * user)
{
    if(user)
        chatCmdSendf("CsrStatus", "%s", user->handle);
}

VOID CALLBACK UpdateUserStatusProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
    if(!vListViewGetSelected(lvUserList))
        listViewDelAllItems(lvUserStatus, UserStatusEntryDestroyAdapter);
}

LRESULT CALLBACK DlgUserListProc (HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    static RECT s_listViewOffsets;
     switch (iMsg)
    {
    case WM_INITDIALOG:
        {
            RECT rect, rect2;
            GetWindowRect(hDlg, &rect);
            GetWindowRect(GetDlgItem(hDlg, IDC_LST_ALLUSERS), &rect2);
            SetRect(&s_listViewOffsets, 
                rect2.left - rect.left,
                rect2.top - rect.top,
                rect2.right - rect.right,
                rect2.bottom - rect.bottom);

            if (lvUserList) vListViewDestroy(lvUserList);
            lvUserList = vListViewCreate();
            vListViewInit(lvUserList, CAUserInfo, hDlg, GetDlgItem(hDlg, IDC_LST_ALLUSERS), CAUserFilter, CAUserColor);

            if(lvUserStatus) listViewDestroy(lvUserStatus);
            lvUserStatus = listViewCreate();
            listViewInit(lvUserStatus, ParseUserStatus, hDlg, GetDlgItem(hDlg, IDC_LST_USER_STATUS));
            listViewEnableColor(lvUserStatus, false);

            SetTimer(hDlg, 0, (30*1000), NULL);    // update user list every 30 seconds (to update "silenced" times)
            SetTimer(hDlg, 1, 500, UpdateUserStatusProc);

            CheckRadioButton(hDlg, IDC_RADIO_ONLINE, IDC_RADIO_ALL, IDC_RADIO_ALL);

            g_hWndUserList = hDlg;

            LocalizeControls(hDlg);

            UserListUpdateMatchCount();
        }
        return FALSE;

    xcase WM_TIMER:
        vListViewRefresh(lvUserList);    // this will update "silenced" times periodically
        //CAUserRemove(GetUserByHandle("abcd"));
    xcase WM_COMMAND:
        {
            switch (LOWORD(wParam))
            {
            case IDCANCEL:
                EndDialog(hDlg, 0);
                PostQuitMessage(0);
                return TRUE;
            xcase IDC_EDIT_USERLIST_FILTER:
                 if(EN_CHANGE == HIWORD(wParam))
                {
                    GetDlgItemText(hDlg, IDC_EDIT_USERLIST_FILTER, gUserListFilter, sizeof(gUserListFilter));
                    stripNonAlphaNumeric(gUserListFilter);
                    gUserListFilterLength = (int)strlen(gUserListFilter);
                //    if(gUserListFilterLength != strlen(gUserListFilter))
                //        SetDlgItemText(hDlg, IDC_EDIT_USERLIST_FILTER, gUserListFilter);    // will trigger another EN_CHANGE msg
                //    else
                        UserListFilter();
                }
            xcase IDC_BUTTON_USERLIST_ADD:
                {
                    static int i = 100000;
                    char buf[10000];
                    GetDlgItemText(hDlg, IDC_EDIT_USERLIST_ADD, buf, sizeof(buf));
                    CAUserAdd(buf, i++, 0, 1);
                }
            xcase IDC_BUTTON_USERLIST_REMOVE:
                {
                    char buf[10000];
                    GetDlgItemText(hDlg, IDC_EDIT_USERLIST_ADD, buf, sizeof(buf));
                    CAUserRemove(GetUserByHandle(buf));
                }
            xcase IDC_BUTTON_USERLIST_INFO:
                {
                    CAUser * user = (CAUser*) vListViewGetSelected(lvUserList);
                    if(user)
                        chatCmdSendf("CsrStatus", "%s", user->handle);
                }
            xcase IDB_USERLIST_RENAME:
                {
                    CAUser * user = (CAUser*) vListViewGetSelected(lvUserList);
                    if(user)
                    {
                        char title[1000];
                        strcpy(title, localizedPrintf("CARenameUserTitle", user->handle));
                        CreateDialogPrompt(hDlg, title, localizedPrintf("CARenameUserPrompt"), (dialogHandler2) onAcceptRenameUser, freeData, strdup(user->handle));  
                    }
                }
            xcase IDB_USERLIST_CHATBAN:
                {
                    CAUser * user = (CAUser*) vListViewGetSelected(lvUserList);
                    if(user)
                    {    
                        char title[1000];
                        strcpy(title, localizedPrintf("CASilenceUserTitle", user->handle));
                        CreateDialogPromptTime(hDlg, title, localizedPrintf("CASilenceUserPrompt"), onSilenceUserAccept, freeData, strdup(user->handle));
                    }
                }
            xcase IDB_USERLIST_PRIVATECHAT:
                {
                    CAUser * user = (CAUser*) vListViewGetSelected(lvUserList);
                    if(user && user->online)
                        chatCmdSendf("CsrInvite", "%s %d %s", localizedPrintf("Private"), kChatId_Handle, user->handle);
                    else    
                    {
                        // TODO: inform person requesting private chat that the user does not exist or is not online.
                        conPrintf( MSG_ERROR, localizedPrintf( "playerNotFound", user->handle ) );
                    }
                }
            xcase IDC_RADIO_ALL:
                if(BN_CLICKED == HIWORD(wParam))
                    gUserListStatus = FILTER_ALL;
                vListViewSetSortColumn(lvUserList, 0);
                UserListFilter();
            xcase IDC_RADIO_ONLINE:
                if(BN_CLICKED == HIWORD(wParam))
                    gUserListStatus = FILTER_ONLINE;
                vListViewSetSortColumn(lvUserList, 1);
                UserListFilter();
            xcase IDC_RADIO_OFFLINE:
                if(BN_CLICKED == HIWORD(wParam))
                    gUserListStatus = FILTER_OFFLINE;
                vListViewSetSortColumn(lvUserList, 1);
                UserListFilter();
            }


        }    
    xcase WM_SIZE:
        {
            WORD w = LOWORD(lParam);
            WORD h = HIWORD(lParam);
            RECT rect, rect2;

            GetClientRect(hDlg, &rect);
            GetWindowRect(GetDlgItem(hDlg, IDC_LST_ALLUSERS), &rect2);
            MoveWindow(GetDlgItem(hDlg, IDC_LST_ALLUSERS), 
                       s_listViewOffsets.left, 
                       s_listViewOffsets.top,
                       rect2.right - rect2.left,    // fixed width
                       (rect.bottom - rect.top) - s_listViewOffsets.top,    
                         true);
        }

    xcase WM_NOTIFY:
        {
            if(   vListViewOnNotify(lvUserList, wParam, lParam, onUserListSelected)
               || listViewOnNotify(lvUserStatus, wParam, lParam, NULL))
            {
                return TRUE;    // signal that we processed the message
            }
        }
    xcase WM_SHOWWINDOW:
        {
            gUserListVisible = (BOOL) wParam;
            if(wParam)
                UserListUpdateCount();
        }
        break;
    }

    return FALSE;
}



void UserListFilter()
{
    vListViewFilter(lvUserList);
    UserListUpdateMatchCount();
    listViewDelAllItems(lvUserStatus, UserStatusEntryDestroyAdapter);
    onUserListSelected(vListViewGetSelected(lvUserList));
}

// Might need to speed up by setting a bool & calling ListView_() function 1 time
// every tick (if necessary)
void UserListUpdate()
{
    vListViewRefresh(lvUserList);
}