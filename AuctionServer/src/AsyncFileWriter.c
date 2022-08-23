/***************************************************************************
 *     Copyright (c) 2006-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 *
 * $Workfile: $
 * $Revision: 1.36 $
 * $Date: 2006/02/07 06:13:59 $
 *
 * Module Description:
 *
 * Revision History:
 *
 ***************************************************************************/
#include "AsyncFileWriter.h"
#include <utilitieslib/components/EString.h>
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/assert/assert.h>
#include <utilitieslib/utils/error.h>
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/components/MemoryPool.h>
#include <utilitieslib/utils/log.h>


MP_DEFINE(AsyncFileWriter);
// currently: creates a new file, but fails if existing file
AsyncFileWriter* AsyncFileWriter_Create(char *fname,bool concat)
{
    AsyncFileWriter *fw = NULL;
    DWORD create_flag = 0;

    MP_CREATE(AsyncFileWriter, 64);
    fw = MP_ALLOC(AsyncFileWriter);

    // CREATE_NEW : if existing, fail.
    // CREATE_ALWAYS : truncate existing
    // OPEN_ALWAYS : append existing
    // OPEN_EXISTING, TRUNCATE_EXISTING
    create_flag = concat?OPEN_ALWAYS:CREATE_ALWAYS;

    fw->hfile = CreateFileA(fname,GENERIC_WRITE,FILE_SHARE_READ,NULL,create_flag,FILE_FLAG_OVERLAPPED,NULL); // FILE_SHARE_READ : just for debug, so you can see what is being written when open
    if(fw->hfile == INVALID_HANDLE_VALUE)
    {
        LOG( LOG_AUCTION, LOG_LEVEL_IMPORTANT, LOG_CONSOLE_ALWAYS, "couldn't open file %s reason %s",fname,winGetLastErrorStr() );
        AsyncFileWriter_Destroy(fw);
        return NULL;
    }

//     if(concat)
//         devassert(SetFilePointer(fw->hfile,0,0,FILE_END)!=INVALID_SET_FILE_POINTER);
    return fw;
}


void AsyncFileWriter_Destroy( AsyncFileWriter *hItem )
{
    if(hItem)
    {
        estrDestroy(&hItem->buf); 
        estrDestroy((char**)&hItem->output);
        CloseHandle(hItem->hfile);
        MP_FREE(AsyncFileWriter, hItem);
    }
}




// void AsyncFileWriter_Close(AsyncFileWriter *fw)
// {
//     CloseHandle(fw->hfile);
//     ZeroStruct(&fw->ol);
//     fw->writing=false;
//     fw->total_written = 0;
//     fw->last_write = 0;
// }

void AsyncFileWriter_Update(AsyncFileWriter *fw, bool until_done)
{
    if(!fw||fw->hfile==INVALID_HANDLE_VALUE)
        return;
    if(GetOverlappedResult(fw->hfile,&fw->ol,&fw->last_write,until_done))
    {
        fw->total_written += fw->last_write;
        fw->writing = false;
    }
    else if(GetLastError() == ERROR_IO_PENDING)
        fw->writing = true;
}

bool AsyncFileWriter_Write(AsyncFileWriter *fw)
{
    char *tmp;
    if(!fw||fw->hfile == INVALID_HANDLE_VALUE||!fw->buf)
        return false;
    if(fw->writing)
        return false;
    if(!devassert(estrLength(&fw->buf)<ASYNCFILEWRITER_MAX_WRITE))
    {
        // todo: move portions of buf over. too lazy
        LOG( LOG_AUCTION, LOG_LEVEL_IMPORTANT, LOG_CONSOLE_ALWAYS, "AsyncFileWriter file too big %i",estrLength(&fw->buf));
        return false;
    }
    
    // init write to end of file (future optimization: set the offset once on create)
    ZeroStruct(&fw->ol);
    fw->ol.Offset = GetFileSize(fw->hfile,&fw->ol.OffsetHigh);
    
    tmp = (char*)fw->output;
    *((char**)&fw->output) = fw->buf;
    fw->buf = tmp;
    estrClear(&fw->buf);
    
    if(!WriteFileEx(fw->hfile,fw->output,estrLength((char**)&fw->output),&fw->ol,0))
        return false;
    fw->writing = true;
    return true;    
}

void AsyncFileWriter_Stream(AsyncFileWriter *fw, bool until_done)
{
    if(!fw)
        return;
    do
    {
        AsyncFileWriter_Update(fw,until_done);
        if(!fw->writing && estrLength(&fw->buf))
            AsyncFileWriter_Write(fw);
        else
            break;
    } while(until_done);
}
