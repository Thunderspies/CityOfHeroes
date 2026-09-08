#include "ChannelList.h"
#include <utilitieslib/components/earray.h>
#include "VirtualListView.h"
#include "resource.h"
#include "ChatAdminNet.h"
#include "CustomDialogs.h"
#include <stdlib.h>

char gChannelListFilter[200];
int     gChannelListFilterLength;    // cached for faster filtering

StashTable gNameToChannelHash;

BOOL    gChannelListVisible;

HWND g_hWndChannelListView;
HWND g_hWndChannelList;
VListView * lvChannelList;

void ChannelListUpdateMatchCount()
{
    static int s_last = -1;
    int count = vListViewGetFilteredSize(lvChannelList);
    if(count != s_last)
    {
        if(count != vListViewGetSize(lvChannelList))
        {
            char buf[100];
            itoa(count, buf, 10);
            SetDlgItemText(g_hWndChannelList, IDC_STATIC_CHANNELLIST_MATCH_COUNT, buf);
            SetDlgItemText(g_hWndChannelList, IDC_STATIC_CHANNELLIST_MATCH_CAPTION, localizedPrintf("MatchesCaption"));
        }
        else
        {
            SetDlgItemText(g_hWndChannelList, IDC_STATIC_CHANNELLIST_MATCH_COUNT, "");
            SetDlgItemText(g_hWndChannelList, IDC_STATIC_CHANNELLIST_MATCH_CAPTION, "");
        }
    }
    s_last = count;
}

static int compareChannelName(const void * a, const void * b)
{
    CAChannel * channelA = *((CAChannel**)a);
    CAChannel * channelB = *((CAChannel**)b);

    return stricmp(channelA->name, channelB->name);
}


char * displayChannelName(CAChannel * channel)
{
    return channel->name;
}

VirtualListViewEntry CAChannelInfo[] = 
{
    { "CAColumnChannelName",    160,    compareChannelName,    displayChannelName},
    { 0 }
};

CAChannel * CAChannelCreate(char * name)
{
    CAChannel * channel = calloc(sizeof(CAChannel),1);

    channel->name = strdup(name);
    channel->filterName = strdup(name);

    stripNonAlphaNumeric(channel->filterName);

    return channel;

}

void CAChannelDestroy(CAChannel * channel)
{
    eaDestroy(&channel->members);
    free(channel->filterName);
    free(channel->name);
    free(channel);
}

CAChannel * CAChannelAdd(char * name, bool sortAndFilter)
{
    CAChannel * channel = CAChannelCreate(name);

    stashAddPointer(gNameToChannelHash, name, channel, false);

    vListViewInsert(lvChannelList, channel, sortAndFilter);

    if(sortAndFilter)
    {
        ChannelListUpdateCount();
        ChannelListUpdateMatchCount();
    }

    return channel;
}

void CAChannelRemove(CAChannel * channel)
{
    stashRemovePointer(gNameToChannelHash, channel->name, NULL);
    vListViewRemove(lvChannelList, channel);

    CAChannelDestroy(channel);

    ChannelListUpdateCount();
    ChannelListUpdateMatchCount();
}


static void CAChannelDestroyAdapter(void* arg0)
{
    CAChannelDestroy((CAChannel *)arg0);
}

void CAChannelRemoveAll()
{    
    vListViewRemoveAll(lvChannelList, 0);
    stashTableClearEx(gNameToChannelHash, NULL, CAChannelDestroyAdapter);
    ChannelListUpdateCount();
    ChannelListUpdateMatchCount();}

void CAChannelRename(CAChannel * channel, char * newName)
{
    size_t len = strlen(newName) + 1;

    vListViewRemove(lvChannelList, channel);
    stashRemovePointer(gNameToChannelHash, channel->name, NULL);

    channel->name = realloc(channel->name, len);
    channel->filterName = realloc(channel->filterName, len);
    strcpy(channel->name, newName);
    strcpy(channel->filterName, newName);

    stripNonAlphaNumeric(channel->filterName);

    stashAddPointer(gNameToChannelHash, newName, channel, false);
    vListViewInsert(lvChannelList, channel, 1);
}


CAChannel * GetChannelByName(char * name)
{
    CAChannel* pResult;
    if (stashFindPointer(gNameToChannelHash, name, &pResult))
        return pResult;
    return NULL;
}

//---------------------------------------------------------------------------------------------
// CAChannelMember
//---------------------------------------------------------------------------------------------

CAChannelMember * CAChannelMemberCreate(CAUser * user)
{
    CAChannelMember * member = calloc(sizeof(CAChannelMember), 1);

    member->user = user;

    return member;
}

void CAChannelMemberDestroy(CAChannelMember * member)
{
    free(member);
}

CAChannelMember * CAChannelGetMember(CAChannel * channel, CAUser * user)
{
    int i;
    for(i=eaSize(&channel->members)-1;i>=0;--i)
    {
        if(channel->members[i]->user == user)
            return channel->members[i];
    }

    return 0;
}

void CAChannelAddMember(CAChannel * channel, CAUser * user, int flags)
{
    CAChannelMember * member = CAChannelGetMember(channel, user);
    if(!member)
    {
        member = CAChannelMemberCreate(user);
        eaPush(&channel->members, member);
    }

    member->flags = flags;
}


void CAChannelRemoveMember(CAChannel * channel, CAUser * user)
{
    CAChannelMember * member = CAChannelGetMember(channel, user);
    if(member)
    {
        eaFindAndRemove(&channel->members, member);
        CAChannelMemberDestroy(member);
    }
}



void ChannelListInit()
{
    if(gNameToChannelHash)
        stashTableDestroy(gNameToChannelHash);
    gNameToChannelHash = stashTableCreateWithStringKeys(10000, StashDeepCopyKeys);
}




int CAChannelFilter(CAChannel * channel)
{
    if(gChannelListFilterLength)
        return strncmp(channel->filterName, gChannelListFilter, gChannelListFilterLength);
    else
        return 0; // match
}

void ChannelListUpdateCount()
{
    if(gChannelListVisible && gNameToChannelHash)
    {
        int size = stashGetValidElementCount(gNameToChannelHash);
        if(size)
            setStatusBar(3, localizedPrintf("CAChannelCount", size));
        else
            setStatusBar(3, "");
    }
}

void ChannelListReset()
{
    CAChannelRemoveAll();
}


void onAcceptCreateChannel(void * foo, char * channelName)
{
    chatCmdSendf("create", "%s 0", channelName);
}

LRESULT CALLBACK DlgChannelListProc (HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    static RECT s_listViewOffsets;

    switch (iMsg)
    {
    case WM_INITDIALOG:
        {
            RECT rect, rect2;
            GetWindowRect(hDlg, &rect);
            GetWindowRect(GetDlgItem(hDlg, IDC_LST_ALLCHANNELS), &rect2);
            SetRect(&s_listViewOffsets, 
                rect2.left - rect.left,
                rect2.top - rect.top,
                rect2.right - rect.right,
                rect2.bottom - rect.bottom);

            if (lvChannelList) vListViewDestroy(lvChannelList);
            lvChannelList = vListViewCreate();
            vListViewInit(lvChannelList, CAChannelInfo, hDlg, GetDlgItem(hDlg, IDC_LST_ALLCHANNELS), (FilterFunc)CAChannelFilter, 0);

            SetTimer(hDlg, 0, 5000, NULL);

            g_hWndChannelList = hDlg;

            ChannelListUpdateMatchCount();

            LocalizeControls(hDlg);

        }
        return FALSE;

        xcase WM_TIMER:
            //onTimer();
        xcase WM_COMMAND:
        {
            switch (LOWORD(wParam))
            {
            case IDCANCEL:
                EndDialog(hDlg, 0);
                PostQuitMessage(0);
                return TRUE;
            xcase IDC_EDIT_CHANNELLIST_FILTER:
                if(EN_CHANGE == HIWORD(wParam))
                {
                    GetDlgItemText(hDlg, IDC_EDIT_CHANNELLIST_FILTER, gChannelListFilter, sizeof(gChannelListFilter));
                    stripNonAlphaNumeric(gChannelListFilter);
                    gChannelListFilterLength = (int)strlen(gChannelListFilter);
                    ChannelListFilter();
                }
            xcase IDB_CHANNELLIST_CREATE:
                {
                    char title[1000];
                    strcpy(title, localizedPrintf("CACreateChannelTitle"));
                    CreateDialogPrompt(hDlg, title, localizedPrintf("CACreateChannelPrompt"), onAcceptCreateChannel, NULL, 0); 
                }
            xcase IDB_CHANNELLIST_JOIN:
                {
                    CAChannel * channel = (CAChannel*) vListViewGetSelected(lvChannelList);
                    if(channel)
                        chatCmdSendf("join", "%s 0", channel->name);
                }
            xcase IDB_CHANNELLIST_KILL:
                {
                    CAChannel * channel = (CAChannel*) vListViewGetSelected(lvChannelList);
                    if(channel)
                         chatCmdSendf("CsrChanKill", "%s", channel->name);
                }
            }
        }    
        xcase WM_SIZE:
        {
            WORD w = LOWORD(lParam);
            WORD h = HIWORD(lParam);
            RECT rect, rect2;

            GetClientRect(hDlg, &rect);
            GetWindowRect(GetDlgItem(hDlg, IDC_LST_ALLCHANNELS), &rect2);
            MoveWindow(GetDlgItem(hDlg, IDC_LST_ALLCHANNELS), 
                s_listViewOffsets.left, 
                s_listViewOffsets.top,
                rect2.right - rect2.left,    // fixed width
                (rect.bottom - rect.top) - s_listViewOffsets.top,
                true);
        }

        xcase WM_NOTIFY:
        {
            if(vListViewOnNotify(lvChannelList, wParam, lParam, 0))
            {
                return TRUE;    // signal that we processed the message
            }
        }
        xcase WM_SHOWWINDOW:
        {
            gChannelListVisible = (BOOL) wParam;
            if(wParam)
                ChannelListUpdateCount();
        }
        break;
    }

    return FALSE;
}





void ChannelListFilter()
{
    vListViewFilter(lvChannelList);
    ChannelListUpdateMatchCount();
}

// Might need to speed up by setting a bool & calling ListView_() function 1 time
// every tick (if necessary)
void ChannelListUpdate()
{
    vListViewRefresh(lvChannelList);
}