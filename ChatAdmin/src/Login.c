#include "Login.h"
#include "ChatAdminNet.h"
#include "ChatAdmin.h"
#include "resource.h"
#include "ChatAdminUtils.h"



static void fixButtons(HWND hDlg)
{
    if(    SendMessage(GetDlgItem(hDlg, IDC_COMBO_LOGIN_SERVER),  WM_GETTEXTLENGTH, 0, 0)
        && SendMessage(GetDlgItem(hDlg, IDC_COMBO_LOGIN_HANDLE),  WM_GETTEXTLENGTH, 0, 0)
        && SendMessage(GetDlgItem(hDlg, IDC_EDIT_LOGIN_PASSWORD), WM_GETTEXTLENGTH, 0, 0))
    {
        EnableWindow(GetDlgItem(hDlg, IDC_LOGIN_OK), TRUE);
    }
    else
        EnableWindow(GetDlgItem(hDlg, IDC_LOGIN_OK), FALSE);
}

INT_PTR CALLBACK DlgLoginProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    switch (iMsg) 
    {
        case WM_INITDIALOG:
        {
            int i;
            RECT rect, rect2;
            int wd, wd2, ht, ht2;

            getNameList("ServerList");
            for (i=0; i<name_count; i++) 
                SendMessage(GetDlgItem(hDlg, IDC_COMBO_LOGIN_SERVER), CB_ADDSTRING, 0, (LPARAM)namelist[i]);
            SendMessage(GetDlgItem(hDlg, IDC_COMBO_LOGIN_SERVER), CB_SELECTSTRING, (WPARAM) -1, (LPARAM)regGetString("LastServer", ""));
                

            getNameList("HandleList");
            for (i=0; i<name_count; i++) 
                SendMessage(GetDlgItem(hDlg, IDC_COMBO_LOGIN_HANDLE), CB_ADDSTRING, 0, (LPARAM)namelist[i]);
            SendMessage(GetDlgItem(hDlg, IDC_COMBO_LOGIN_HANDLE), CB_SELECTSTRING, (WPARAM) -1, (LPARAM)regGetString("LastHandle", ""));

            fixButtons(hDlg);
            SetFocus(GetDlgItem(hDlg, IDC_EDIT_LOGIN_PASSWORD));

            SetWindowText(hDlg, localizedPrintf("ChatAdminLogin"));

            // center in dialog
            GetWindowRect(GetParent(hDlg), &rect);
            GetWindowRect(hDlg, &rect2);
            wd    = rect.right    - rect.left;
            ht    = rect.bottom    - rect.top;
            wd2 = rect2.right    - rect2.left;
            ht2 = rect2.bottom    - rect2.top;
            MoveWindow(hDlg, (rect.left + (wd/2)-(wd2/2)), (rect.top + (ht/2)-(ht2/2)), wd2, ht2, TRUE);
        
            LocalizeControls(hDlg);    
        }
        xcase WM_COMMAND:
        {
            switch (LOWORD (wParam))
            {
                case IDC_LOGIN_OK: // Syncronize Data
                {
                        char server[1000], handle[1000], password[1000];
                        GetDlgItemText(hDlg, IDC_COMBO_LOGIN_SERVER, server, sizeof(server));
                        GetDlgItemText(hDlg, IDC_COMBO_LOGIN_HANDLE, handle, sizeof(handle));
                        GetDlgItemText(hDlg, IDC_EDIT_LOGIN_PASSWORD, password, sizeof(password));
                            
                        addNameToList("ServerList", server);
                        addNameToList("HandleList", handle);

                        regPutString("LastServer", server);
                        regPutString("LastHandle", handle);

                        setLoginInfo(server, handle, password);

                        EndDialog(hDlg, LOWORD (wParam));
                        return TRUE;
                }
                xcase IDCANCEL:
                {
                    EndDialog(hDlg, LOWORD (wParam));
                    return TRUE;
                }
                xcase IDC_COMBO_LOGIN_SERVER: 
                case  IDC_COMBO_LOGIN_HANDLE:    //    fall through
                case  IDC_EDIT_LOGIN_PASSWORD:    //    fall through
                {
                    fixButtons(hDlg);
                }

            }
        }
        break;

    }
    return FALSE;
}


int LoginDlg(HWND hDlg)
{
    static int s_lock = 0;
    int res = 0;

    if(!s_lock)
    {
        s_lock = 1;
        res = DialogBox (g_hInst, MAKEINTRESOURCE (IDD_LOGIN), hDlg, DlgLoginProc);
        if(res == IDC_LOGIN_OK)
        {
            chatAdminConnect();
        }
        s_lock = 0;
    }
    return res;
}