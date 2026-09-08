#include "ChannelMonitor.h"
#include "ChatAdminNet.h"
#include <utilitieslib/components/earray.h>
#include "resource.h"
#include "CustomDialogs.h"
#include "chatdb.h"
#include <utilitieslib/utils/winutil.h>
#include <utilitieslib/utils/WinTabUtil.h>

static CAChanMon ** gChanMons;

// for storing channel message send history
static int gSendHistoryIdx = -1;    
static char **gSendHistory = 0;


CAChanMon * GetChanMon(CAChannel * channel);

void MemberListFilter(CAChanMon * cm)
{
    vListViewFilter(cm->memberList);
}

CAChanMember * CAChanMemberCreate(CAUser * user, int flags, CAChanMon * monitor)
{
    CAChanMember * member = calloc(sizeof(CAChanMember),1);

    member->user = user;
    member->flags = flags;
    member->monitor = monitor;

    return member;
}

void CAChanMemberDestroy(CAChanMember * member)
{
    free(member);
}



static int compareMemberHandle(const void * a, const void * b)
{
    CAChanMember * memberA = *((CAChanMember**)a);
    CAChanMember * memberB = *((CAChanMember**)b);

    return stricmp(memberA->user->handle, memberB->user->handle);
}

static int compareMemberFlags(const void * a, const void * b)
{
    CAChanMember * memberA = *((CAChanMember**)a);
    CAChanMember * memberB = *((CAChanMember**)b);

    if(memberA->flags < memberB->flags)
        return 1;
    else if(memberA->flags > memberB->flags)
        return -1;
    else 
        return stricmp(memberA->user->handle, memberB->user->handle);
}

char * displayMemberHandle(void* memberData)
{
    CAChanMember * member = (CAChanMember *)memberData;
    return member->user->handle;
}

char * displayMemberFlags(void* memberData)
{
    CAChanMember * member = (CAChanMember *)memberData;
    return flagStr(member->flags);
}

VirtualListViewEntry CAChanMemberInfo[] = 
{
    { "CAColumnHandle",            83,        compareMemberHandle,    displayMemberHandle},
    { "CAColumnChannelStatus",    90,        compareMemberFlags,        displayMemberFlags},
    { 0 }
};


void CAChanMemberColor(void* memberData, COLORREF* pTextColor, COLORREF* pBkColor)
{
    CAChanMember * member = (CAChanMember *)memberData;
    if(member->flags & CHANFLAGS_ADMIN)
    {
        *pTextColor = RGB(255,0,0);
    }
    else if(member->flags & CHANFLAGS_OPERATOR)
    {
        *pTextColor = RGB(0,100,200);
    }
    else
    {
        *pTextColor = RGB(0,0,0);
    }

    *pBkColor = RGB(255,255,255);
}


CAChanMon * CAChanMonCreate(CAChannel * channel)
{
    CAChanMon * cm = calloc(sizeof(CAChanMon),1);

    cm->channel = channel;
    eaCreate(&cm->members);

    return cm;
}

int CAChanMemberFilter(CAChanMember * member)
{
    CAChanMon * cm = member->monitor;
    return strncmp(member->user->filterHandle, cm->memberFilter, cm->memberFilterLength);
}

void CAChanMonInit(CAChanMon * cm, HWND hDlg)
{
    cm->mv = MessageViewCreate(GetDlgItem(hDlg, IDC_EDIT_CHANMON_MESSAGES), 1000);
    cm->hDlg = hDlg;

    cm->memberList = vListViewCreate();
    vListViewInit(cm->memberList, CAChanMemberInfo, hDlg, GetDlgItem(hDlg, IDC_LST_CHANMON_MEMBERS), CAChanMemberFilter,  CAChanMemberColor);

    MessageViewShowTime(cm->mv, g_bShowChatTimestamps);
}

void CAChanMonDestroy(CAChanMon * cm)
{
    eaDestroy(&cm->members);
    MessageViewDestroy(cm->mv);
    vListViewDestroyEx(cm->memberList, CAChanMemberDestroy);
}

void ChanMonChannelMsg(char * channelName, char * msg, int type)
{
    CAChannel * channel = GetChannelByName(channelName);
    if(channel)
    {
        CAChanMon * cm = GetChanMon(channel);
        if(cm)
            MessageViewAdd(cm->mv, msg, type);
    }
}


bool isMember(void* memberData, void* userData)
{
    CAChanMember * member = (CAChanMember *)memberData;
    CAUser * user = (CAUser *)userData;
    return (member->user == user);
}


void ChanMonUpdateUser(CAUser * user)
{
    int i =0;
    for(i=eaSize(&gChanMons)-1;i>=0;--i)
    {
        VListView * lv = gChanMons[i]->memberList;
        CAChanMember * member = (CAChanMember*) vListViewFind(lv, user, isMember);
        if(member)
        {
            vListViewRefreshItem(lv, member);
            MemberListFilter(member->monitor);
        }
    }
}


LRESULT CALLBACK DlgChannelMonitorProc (HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

void ChanMonChannelMsg(char * channelName, char * msg, int type); 

void ChanMonTabCreate(CAChannel * channel)
{    
    DLGHDR *pHdr = (DLGHDR *)GetWindowLongPtr(g_hDlgMain, GWLP_USERDATA);

    CAChanMon * cm = CAChanMonCreate(channel);

    TabDlgInsertTab(g_hInst, pHdr, IDD_DLG_CHANNELMONITOR, DlgChannelMonitorProc, localizedPrintf("CAChannelMonitorTab", channel->name), 0, cm);

    // have main dialog resize all tabs
    {
        int height, width;
        RECT rect;

        GetWindowRect(g_hDlgMain, &rect);
        height = rect.bottom - rect.top;
        width = rect.right - rect.left;
        SendMessage(g_hDlgMain, WM_SIZE, 0, MAKELPARAM(width, height));
    }
}

CAChanMon * GetChanMon(CAChannel * channel)
{
    int i =0;
    for(i=eaSize(&gChanMons)-1;i>=0;--i)
    {
        if(gChanMons[i]->channel == channel)
            return gChanMons[i];
    }
    return 0;
}

void ChanMonTabRemove(CAChannel * channel)
{
    CAChanMon * cm = GetChanMon(channel);
    if(cm)
    {
        DLGHDR *pHdr = (DLGHDR *)GetWindowLongPtr(g_hDlgMain, GWLP_USERDATA);
        TabDlgRemoveTab(pHdr, cm->hDlg);
        eaFindAndRemove(&gChanMons, cm);
        CAChanMonDestroy(cm);
    }
}



void ChanMonJoin(CAChannel * channel, CAUser * user, int flags)
{
    CAChanMon * cm = GetChanMon(channel);
    if(cm)
    {
        CAChanMember * member = vListViewFind(cm->memberList, user, isMember);
        if(member)
        {
            member->flags = flags;
            vListViewRefreshItem(cm->memberList, member);
        }
        else
        {    
            member = CAChanMemberCreate(user, flags, cm);
            vListViewInsert(cm->memberList, member, 1);
            ChanMonUpdateCount(cm);
            MemberListFilter(cm);
        }
    }
}

void ChanMonLeave(CAChannel * channel, CAUser * user)
{
    CAChanMon * cm = GetChanMon(channel);
    if(cm)
    {
        if(user == gAdminClient.user)
            ChanMonTabRemove(channel);
        else
        {
            vListViewFindAndRemove(cm->memberList, user, isMember);
            ChanMonUpdateCount(cm);
        }
    }
}

void ChanMonUpdateCount(CAChanMon * cm)
{
    if(cm->isVisible && cm->memberList)
    {
        int size = vListViewGetSize(cm->memberList);
        if(size)
            setStatusBar(3, localizedPrintf("CAMemberCount", size));
        else
            setStatusBar(3, "");
    }
}

void ChanMonReset()
{
    int i;
    CAChanMon **list = 0;
    eaCopy(&list, &gChanMons);
    for(i=eaSize(&list)-1;i>=0;--i)
    {
        ChanMonTabRemove(list[i]->channel);
    }
    eaDestroy(&list);
}


void setUserMode(CAChanMon * cm, CAChanMember * member, char * mode)
{
    chatCmdSendf("UserMode", "%s %s %s", cm->channel->name, member->user->handle, mode);
}


void sendChannelMsg(HWND hDlg, CAChanMon * cm)
{
    if(SendMessage(GetDlgItem(hDlg, IDC_EDIT_CHANMON_SEND),  WM_GETTEXTLENGTH, 0, 0))
    {
        char msg[1000];
        GetDlgItemText(hDlg, IDC_EDIT_CHANMON_SEND, msg, sizeof(msg));
        SetDlgItemText(hDlg, IDC_EDIT_CHANMON_SEND, "");
        EnableWindow(GetDlgItem(hDlg, IDB_CHANMON_SEND), FALSE);
        chatCmdSendf("Send", "%s %s", cm->channel->name, msg);

        // update history
        if(eaSize(&gSendHistory) >= 10)
        {
            char * str = eaPop(&gSendHistory);
            free(str);
        }
        eaInsert(&gSendHistory, strdup(msg), 0);
        gSendHistoryIdx = -1;
    }
}


void setChanMsg(HWND hDlg, int step)
{
    int newIdx = gSendHistoryIdx + step;
    if(newIdx >= 0 && newIdx < eaSize(&gSendHistory))
    {
        size_t cursorPos = strlen(gSendHistory[newIdx]);
        gSendHistoryIdx = newIdx;
        SetDlgItemText(hDlg, IDC_EDIT_CHANMON_SEND, gSendHistory[newIdx]);
        SendMessage(GetDlgItem(hDlg, IDC_EDIT_CHANMON_SEND), EM_SETSEL, (WPARAM)cursorPos, (LPARAM)cursorPos);
    }
    else if(newIdx == -1)
    {
        gSendHistoryIdx = -1;
        SetDlgItemText(hDlg, IDC_EDIT_CHANMON_SEND, "");
    }
}


void ChanMonShowTimestamps(bool show)
{
    int i;
    for(i=0;i<eaSize(&gChanMons);i++)
    {
        MessageViewShowTime(gChanMons[i]->mv, show);
    }
}


LRESULT FAR PASCAL chanMsgSubClassFunc(HWND hWnd, UINT wMsg, WPARAM wParam, LONG lParam)
{
    WNDPROC oldProc = (WNDPROC)GetWindowLongPtr(hWnd, GWLP_USERDATA);
    HWND hDlg = GetParent(hWnd);
    CAChanMon * cm = (CAChanMon*)GetWindowLongPtr(hDlg, GWLP_USERDATA);

    switch (wMsg)
    {
    case WM_GETDLGCODE:
        return (DLGC_WANTALLKEYS |
            CallWindowProc(oldProc, hWnd, wMsg, wParam, lParam));

    xcase WM_CHAR:
        //Process this message to avoid message beeps.
        if ((wParam == VK_RETURN) || (wParam == VK_TAB))
            return 0;
        else
            return (CallWindowProc(oldProc, hWnd, wMsg, wParam, lParam));

    xcase WM_KEYDOWN:
        {
            switch(wParam)
            {
                case VK_RETURN:
                    sendChannelMsg(hDlg, cm);    
                    return FALSE;
                xcase VK_UP:
                    setChanMsg(hDlg, 1);
                    return FALSE;
                xcase VK_DOWN: 
                    setChanMsg(hDlg, -1);
                    return FALSE;
            }
        }            

        return CallWindowProc(oldProc, hWnd, wMsg, wParam, lParam);
    
    break ;
    default:
        return CallWindowProc(oldProc, hWnd, wMsg, wParam, lParam);

    } /* end switch */
}


void MoveControl(HWND hDlg, int id, int deltaX, int deltaY, int deltaWidth, int deltaHeight)
{
    RECT rMain, rButton;
    HWND hButton = GetDlgItem(hDlg, id);

    GetWindowRect(hDlg, &rMain);
    GetWindowRect(hButton, &rButton);

    MoveWindow( hButton, 
                rButton.left - rMain.left + deltaX,
                rButton.top  - rMain.top  + deltaY,
                (rButton.right - rButton.left) + deltaWidth,
                (rButton.bottom - rButton.top) + deltaHeight,
                TRUE);

}


static void fixButtons(CAChanMon * cm)
{
    CAChanMember * member = vListViewGetSelected(cm->memberList);
    char *operatorStr, *silenceStr;

    if(member)
    {
        silenceStr  = (member->flags & CHANFLAGS_SEND) ? "CAControlChannelMonitorSilence" : "CAControlChannelMonitorUnsilence";
        operatorStr = (member->flags & CHANFLAGS_OPERATOR) ? "CAControlChannelMonitorRevokeOperator" : "CAControlChannelMonitorMakeOperator";
    }
    else
    {
        silenceStr  = "CAControlChannelMonitorSilence";
        operatorStr = "CAControlChannelMonitorMakeOperator";
    }

    if(cm->lastSilence != silenceStr)
    {
        SetDlgItemText(cm->hDlg, IDB_CHANMON_SILENCE, localizedPrintf(silenceStr));
        cm->lastSilence = silenceStr;
    }

    if(cm->lastOperator != operatorStr)
    {
        SetDlgItemText(cm->hDlg, IDB_CHANMON_OPERATOR, localizedPrintf(operatorStr));
        cm->lastOperator = operatorStr;
    }
}


void onSelectMember(CAChanMember * member)
{
    if(member)
        fixButtons(member->monitor);
}


void silenceMemberAction( void *member, void * params )
{
    if ( strcmp( gAdminClient.handle, ((CAChanMember*)member)->user->handle ) != 0 )
        setUserMode( (CAChanMon*)params, (CAChanMember*)member, "-send");
}

void onAcceptInviteUser( void *channelname, char * user )
{
    chatCmdSendf( "Invite", "%s %d %s", channelname, kChatId_Handle, user );
}

LRESULT CALLBACK DlgChannelMonitorProc (HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    static RECT s_listViewOffsets;
    CAChanMon * cm = (CAChanMon*)GetWindowLongPtr(hDlg, GWLP_USERDATA);

    if (iMsg != WM_INITDIALOG && cm == NULL) {
        return FALSE;
    }

    switch (iMsg)
    {
    case WM_INITDIALOG:
        {
            static int s_init = 0;

            FARPROC proc;
            RECT rect, rect2;

            CAChanMon * cm = (CAChanMon*) (lParam);
            CAChanMonInit(cm, hDlg);
            SetWindowLongPtr(hDlg, GWLP_USERDATA, (LONG_PTR) cm);
            eaPush(&gChanMons, cm); 
            EnableControls(hDlg, TRUE);
            

            if(!gSendHistory)
                eaCreate(&gSendHistory);

            // subclass "send channel msg" edit box
            proc = (FARPROC) SetWindowLongPtr(GetDlgItem(hDlg, IDC_EDIT_CHANMON_SEND), GWLP_USERDATA, (LONG_PTR)chanMsgSubClassFunc);
            SetWindowLongPtr(GetDlgItem(hDlg, IDC_EDIT_CHANMON_SEND), GWLP_USERDATA, (LONG_PTR)proc);
        
            EnableWindow(GetDlgItem(hDlg, IDB_CHANMON_SEND), FALSE);

            SetTimer(hDlg, 0, 100, NULL);

            if( ! s_init)
            {
                s_init = 1;

                GetWindowRect(hDlg, &rect);
                GetWindowRect(GetDlgItem(hDlg, IDC_EDIT_CHANMON_MESSAGES), &rect2);
                SetRect(&s_listViewOffsets, 
                    rect2.left - rect.left,
                    rect2.top - rect.top,
                    rect2.right - rect.right,
                    rect2.bottom - rect.bottom);
            }

            EnableWindow(GetDlgItem(hDlg, IDB_CHANMON_LEAVE), TRUE);

            LocalizeControls(hDlg);

        }
        return FALSE;
    xcase WM_TIMER:
        fixButtons(cm);
    xcase WM_COMMAND:
        {
            switch (LOWORD(wParam))
            {
                case IDCANCEL:
                    EndDialog(hDlg, 0);
                    PostQuitMessage(0);
                    return TRUE;
                xcase IDB_CHANMON_SEND:
                    sendChannelMsg(hDlg, cm);
                xcase IDB_CHANMON_LEAVE:
                    chatCmdSendf("Leave", "%s", cm->channel->name);
                xcase IDC_EDIT_CHANMON_SEND:
                {
                    if(EN_CHANGE == HIWORD(wParam))
                    {
                        int len = SendMessage(GetDlgItem(hDlg, IDC_EDIT_CHANMON_SEND),  WM_GETTEXTLENGTH, 0, 0);
                        EnableWindow(GetDlgItem(hDlg, IDB_CHANMON_SEND), (len ? TRUE : FALSE));
                    }
                }
                xcase IDB_CHANMON_KICK:
                {
                    CAChanMember * member = vListViewGetSelected(cm->memberList);
                    if(member)
                    {
                        setUserMode(cm, member, "-join");
                    }
                }
                xcase IDB_CHANMON_OPERATOR:
                {
                    CAChanMember * member = vListViewGetSelected(cm->memberList);
                    if(member)
                    {
                        if(member->flags & CHANFLAGS_OPERATOR)
                            setUserMode(cm, member, "-operator");
                        else
                            setUserMode(cm, member, "+operator");
                    }
                }
                xcase IDB_CHANMON_PRIVATECHAT:
                {
                    CAChanMember * member = vListViewGetSelected(cm->memberList);
                    if(member)
                        chatCmdSendf("CsrInvite", "%s %d %s", localizedPrintf("Private"), kChatId_Handle, member->user->handle);
                }
                xcase IDB_CHANMON_SILENCE:
                {
                    CAChanMember * member = vListViewGetSelected(cm->memberList);
                    if(member)
                    {
                        if(member->flags & CHANFLAGS_SEND)
                            setUserMode(cm, member, "-send");
                        else
                            setUserMode(cm, member, "+send");
                    }
                }
                xcase IDB_CHANMON_RENAME:
                {
                    CAChanMember * member = (CAChanMember*) vListViewGetSelected(cm->memberList);
                    if(member && member->user)
                    {
                        char title[1000];
                        strcpy(title, localizedPrintf("CARenameUserTitle", member->user->handle));
                        CreateDialogPrompt(hDlg, title, localizedPrintf("CARenameUserPrompt"), (dialogHandler2) onAcceptRenameUser, freeData, strdup(member->user->handle));  
                    }
                }
                xcase IDB_CHANMON_KILL:
                {
                    if(cm->channel)
                         chatCmdSendf("CsrChanKill", "%s", cm->channel->name);
                }
                xcase IDB_CHANMON_SILENCEALL:
                {
                    vListViewDoToAllItems( cm->memberList, silenceMemberAction, cm );
                }
                xcase IDB_CHANMON_INVITE:
                {
                    CreateDialogPrompt(hDlg, "Invite User", 
                        "User Name", onAcceptInviteUser, NULL, cm->channel->name);
                }
                xcase IDC_EDIT_MEMBERLIST_FILTER:
                    if(EN_CHANGE == HIWORD(wParam))
                    {
                        GetDlgItemText(hDlg, IDC_EDIT_MEMBERLIST_FILTER, cm->memberFilter, SIZEOF2(CAChanMon, memberFilter));
                        stripNonAlphaNumeric(cm->memberFilter);
                        cm->memberFilterLength = (int)strlen(cm->memberFilter);
                        MemberListFilter(cm);
                    }
            }
        }    
        xcase WM_SIZE:
        {
            WORD w = LOWORD(lParam);
            WORD h = HIWORD(lParam);
            RECT r;
            int width, height;
            int deltaX, deltaY;

            GetWindowRect(GetDlgItem(hDlg, IDC_EDIT_CHANMON_MESSAGES), &r);
            
            width = w - s_listViewOffsets.left + s_listViewOffsets.right;
            height = h - s_listViewOffsets.top + s_listViewOffsets.bottom;

            if( width <= 0 || height <= 0)    // deal with minimized window
                break;


            deltaX = width - (r.right - r.left);
            deltaY = height - (r.bottom - r.top);    

            MoveControl(hDlg, IDC_EDIT_CHANMON_MESSAGES, 0, 0, deltaX, deltaY);
            MoveControl(hDlg, IDC_LST_CHANMON_MEMBERS,     0, 0,        0, deltaY);

            MoveControl(hDlg, IDB_CHANMON_RENAME,        0, deltaY, 0, 0);
            MoveControl(hDlg, IDB_CHANMON_KICK,            0, deltaY, 0, 0);
            MoveControl(hDlg, IDB_CHANMON_OPERATOR,        0, deltaY, 0, 0);
            MoveControl(hDlg, IDB_CHANMON_PRIVATECHAT,    0, deltaY, 0, 0);
             MoveControl(hDlg, IDB_CHANMON_SILENCE,        0, deltaY, 0, 0);

            MoveControl(hDlg, IDB_CHANMON_KILL,            0, deltaY, 0, 0);
            MoveControl(hDlg, IDB_CHANMON_LEAVE,        0, deltaY, 0, 0);
            MoveControl(hDlg, IDB_CHANMON_SILENCEALL,    0, deltaY, 0, 0);
            MoveControl(hDlg, IDB_CHANMON_INVITE,        0, deltaY, 0, 0);

             MoveControl(hDlg, IDB_CHANMON_SEND,            deltaX, deltaY,      0, 0);
             MoveControl(hDlg, IDC_EDIT_CHANMON_SEND,         0, deltaY, deltaX, 0);
        }
        xcase WM_NOTIFY:
        {
            if(vListViewOnNotify(cm->memberList, wParam, lParam, onSelectMember))
            {
                return TRUE;    // signal that we processed the message
            }
        }
        xcase WM_SHOWWINDOW:
        {
            cm->isVisible = (BOOL) wParam;
            if(wParam)
            {
                ChanMonUpdateCount(cm);
                gSendHistoryIdx = -1;
                SetFocus(GetDlgItem(hDlg, IDC_EDIT_CHANMON_SEND));
            }
        }
        xcase WM_KEYDOWN:
        {
            switch(wParam)
            {
                case VK_RETURN:
                    sendChannelMsg(hDlg, cm);
                break;
            }
        }
        xcase WM_CTLCOLORSTATIC:
            {
                if(((HWND)lParam) == GetDlgItem(hDlg, IDC_EDIT_CHANMON_MESSAGES))
                {
                    // make read-only edit boxes a lighter shade of gray
                    HDC hdcStatic = (HDC)wParam;
                    COLORREF color = RGB(230,230,230);
                    SetTextColor(hdcStatic, RGB(0, 0, 0));
                    SetBkColor(hdcStatic, color);
                    return (LRESULT)CreateSolidBrush(color);
                }
            }
        break;

        break;
    }

    return FALSE;
}


