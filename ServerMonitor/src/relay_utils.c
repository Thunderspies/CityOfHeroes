#include <utilitieslib/network/netio.h>
#include <utilitieslib/utils/file.h>

#include <direct.h>

#include "relay_utils.h"
#include "relay_types.h"

char g_updateServerAddr[512];
char g_customCmd[1024];

BOOL OpenAndAllocFile(char * title, char * pattern, FileAllocInfo * file)
{
    char filename[2000] = "";

    if(!OpenFileDlg(title, pattern, filename))
    {
        return FALSE;
    }

    file->data = fileAlloc(filename, &(file->size));


    if(file->data)
        return TRUE;
    else
    {
        MessageBoxA(NULL, "Failed to read file", "ERROR", MB_ICONERROR);
        return FALSE;
    }

}


void sendBatchFileToClient(ListView* lv, void* conData, void* fileData)
{
    CmdRelayCon * con = (CmdRelayCon *)conData;
    FileAllocInfo * file = (FileAllocInfo *)fileData;
    NetLink *link = con->link;
    if(link)
    {
        // send the file to each relay
        Packet * pak = pktCreate();
        pktSendBitsPack(pak, 1,CMDRELAY_REQUEST_RUN_BATCH_FILE);
        pktSendBitsPack(pak, 1, file->size);
        pktSendBitsArray(pak, (file->size * 8), file->data);
        pktSend(&pak,link);
        lnkFlush(link);
    }
}









char *OpenFileDlg(char * title, char *fileMask,char *fileName)
{
    OPENFILENAMEA theFileInfo;
    //char filterStrs[256];
    int        ret;
    char    base[_MAX_PATH];

    _getcwd(base,_MAX_PATH);
    memset(&theFileInfo,0,sizeof(theFileInfo));
    theFileInfo.lStructSize = sizeof(OPENFILENAME);
    theFileInfo.lpstrTitle = title;
    theFileInfo.hwndOwner = NULL;
    theFileInfo.hInstance = g_hInst;
    theFileInfo.lpstrFilter = fileMask;
    theFileInfo.lpstrCustomFilter = NULL;
    theFileInfo.lpstrFile = fileName;
    theFileInfo.nMaxFile = 255;
    theFileInfo.nMaxFileTitle = 0;
    theFileInfo.lpstrFileTitle = NULL;
    theFileInfo.lpstrInitialDir = NULL;
    theFileInfo.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    theFileInfo.lpstrDefExt = NULL;

    ret = GetOpenFileNameA(&theFileInfo);

    _chdir(base);

    //    inpClear();
    if (ret)
        return fileName;
    else
        return NULL;
}
