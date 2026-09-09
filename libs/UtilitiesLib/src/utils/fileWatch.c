#include <process.h>
#include <sys/stat.h>
#include "utilitieslib/utils/fileWatch.h"
#include "utilitieslib/assert/assert.h"
#include "utilitieslib/utils/sysutil.h"
#include "utilitieslib/utils/utils.h"
#include "utilitieslib/utils/file.h"
#include "utilitieslib/utils/strings_opt.h"
#include "utilitieslib/utils/timing.h"

static struct {
    CRITICAL_SECTION    cs;
} fileWatchClient;

static S32 acquireMutex(HANDLE hMutex, S32 wait){
    switch(WaitForSingleObject(hMutex, wait))
    {
    case WAIT_OBJECT_0:
    case WAIT_ABANDONED:
        {
            return 1;
        }
    }

    return 0;
}

static void releaseAndCloseMutex(HANDLE hMutex){
    ReleaseMutex(hMutex);
    CloseHandle(hMutex);
}

static void atomicInitializeCriticalSectionFunction(volatile S32* init, CRITICAL_SECTION* cs){
    char name[100];
    HANDLE hMutex;

    STR_COMBINE_SD(name, "Global\\InitCritSec:", _getpid());

    hMutex = CreateMutexA(NULL, FALSE, name);

    assertmsg(hMutex, "Can't create mutex.");

    if(!acquireMutex(hMutex, INFINITE)){
        // This should never happen.

        assert(0);
    }

    if(!*init){
        InitializeCriticalSection(cs);
        *init = 1;
    }

    ReleaseMutex(hMutex);
    CloseHandle(hMutex);
}

#define atomicInitializeCriticalSection(init, cs) (*(init) ? 1 : (atomicInitializeCriticalSectionFunction((init), (cs)), 1))

static void EnterCS(void){
    static S32 init;

    atomicInitializeCriticalSection(&init, &fileWatchClient.cs);

    EnterCriticalSection(&fileWatchClient.cs);
}

static void LeaveCS(void){
    LeaveCriticalSection(&fileWatchClient.cs);
}





static S32 isSlash(char c){
    return c == '/' || c == '\\';
}

static S32 isNullOrSlash(char c){
    return !c || isSlash(c);
}

static void adjustFileName(const char* fileName, char* fileNameOut){
    const char* cur;
    char* curOut = NULL;

    // Check for an absolute filename (c:/something, OR \\server\share\filename).
    if(    isalpha(fileName[0]) &&
        fileName[1] == ':'
        ||
        isSlash(fileName[0]) &&
        isSlash(fileName[1]))
    {
        memcpy(fileNameOut, fileName, 2);
        fileNameOut[2] = '/';
        curOut = fileNameOut + 3;
        fileName += 3;
    }else{
        GetCurrentDirectoryA(MAX_PATH, fileNameOut);
        forwardSlashes(fileNameOut);

        curOut = fileNameOut + strlen(fileNameOut);

    }

    // Remove trailing slashes.

    while(    curOut - fileNameOut > 3 &&
        curOut[-1] == '/')
    {
        *--curOut = 0;
    }

    for(cur = fileName; *cur;){
        const char* delim;
        S32 len;

        for(; isSlash(*cur); cur++);

        for(delim = cur; !isNullOrSlash(*delim); delim++);

        len = delim - cur;

        if(    len <= 2 &&
            cur[0] == '.' &&
            (    isNullOrSlash(cur[1]) ||
            cur[1] == '.'))
        {
            if(cur[1] == '.'){
                // cur == ".."
                // fileNameOut: "c:/game/data/thing/whatever"
                //                                    curOut^  (null terminator)

                while(curOut - fileNameOut >= 3){
                    char c = *--curOut;

                    *curOut = 0;

                    if(isSlash(c)){
                        break;
                    }
                }
            }
            else if(!cur[0]){
                // cur == ".", so don't add anything.
            }
        }else{
            // Not a "." or ".."

            if(curOut - fileNameOut > 3){
                *curOut++ = '/';
            }
            memcpy(curOut, cur, len);
            curOut += len;
        }

        cur = delim + (*delim ? 1 : 0);
    }

    *curOut = 0;

    removeLeadingAndFollowingSpaces(fileNameOut);
    moveFromCRoot(fileNameOut);
}

typedef struct FileWatchFindFileStruct {
    S32                allocIndex;
    HANDLE        windowsHandle;
} FileWatchFindFileStruct;

static struct {
    FileWatchFindFileStruct*        finds;
    S32                                count;
    S32                                maxCount;
    S32                                firstFree;
} finds;

static FileWatchFindFileStruct* getNewFind(S32* handleOut){
    FileWatchFindFileStruct* find;

    if(!finds.firstFree){
        find = dynArrayAddStruct(&finds.finds, &finds.count, &finds.maxCount);

        find->allocIndex = finds.count;
    }else{
        S32 index = finds.firstFree - 1;

        assert(index >= 0 && index < finds.count);

        find = finds.finds + index;

        finds.firstFree = find->allocIndex;

        find->allocIndex = index + 1;
    }

    *handleOut = find->allocIndex;

    return find;
}

static void freeFind(FileWatchFindFileStruct* find){
    S32 index = find->allocIndex;

    assert(index > 0 && index <= finds.count);
    assert(finds.finds + index - 1 == find);
    assert(finds.firstFree != index);

    find->allocIndex = finds.firstFree;

    finds.firstFree = index;
}

static FileWatchFindFileStruct* getFindByIndex(S32 index){
    FileWatchFindFileStruct* find;

    if(index <= 0 || index > finds.count){
        return NULL;
    }

    find = finds.finds + index - 1;

    if(find->allocIndex != index){
        return NULL;
    }

    return find;
}

// Function: fwFindFirstFile.
//
// ------------------------------------------------------------------------
// return | handleOut | Meaning
// -------+-----------+----------------------------------------------------
// 0      | any       | Folder isn't watched.
// 1      | 0         | Folder is watched, but no files match.
// 1      | !0        | Here's the first matching file.

S32 fwFindFirstFile(U32* handleOut, const char* fileSpec, WIN32_FIND_DATAA* wfd){
    char                fileNameAdjusted[MAX_PATH];
    S32                    tryCount = 0;
    S32                    retVal = 0;

    assert(handleOut);

    adjustFileName(fileSpec, fileNameAdjusted);

    EnterCS();

    while(1){
        HANDLE handle = FindFirstFileA(fileSpec, wfd);

        if(handle != INVALID_HANDLE_VALUE){
            FileWatchFindFileStruct* find = getNewFind(handleOut);

            find->windowsHandle = handle;

            retVal = 1;

            break;
        }
        else if(GetLastError() == ERROR_SHARING_VIOLATION){
            if(++tryCount == 5){
                printfColor(COLOR_RED, "Sharing violation doing FindFirstFile: %s\n", fileSpec);
                break;
            }
            Sleep(300);
        }
        else{
            U32 error = GetLastError();
            UNUSED(error);
            //printf("unhandled error: %d\n", error);
            break;
        }
    }

    LeaveCS();

    return retVal;
}

S32 fwFindNextFile(U32 handle, WIN32_FIND_DATAA* wfd){
    FileWatchFindFileStruct*    find;
    S32                            retVal = 1;

    EnterCS();

    find = getFindByIndex(handle);

    if(!find){
        retVal = 0;
    }
    else{
        retVal = FindNextFileA(find->windowsHandle, wfd);
    }

    LeaveCS();

    return retVal;
}

S32 fwFindClose(U32 handle){
    FileWatchFindFileStruct*    find;
    S32                            retVal = 1;

    EnterCS();

    find = getFindByIndex(handle);

    if(!find){
        // Invalid handle, user error.

        retVal = 0;
    }else{
        retVal = FindClose(find->windowsHandle);

        freeFind(find);
    }

    LeaveCS();

    return retVal;
}

S32 fwStat(const char* fileName, struct _stat32* statInfo){
    struct _stat32 tempStatInfo;
    char fileNameAdjusted[MAX_PATH];

    if(!statInfo){
        statInfo = &tempStatInfo;
    }

    adjustFileName(fileName, fileNameAdjusted);

    // Folder isn't tracked by FileWatcher, or FileWatcher isn't running.
#if defined(_MSC_VER) && _MSC_VER < 1400
    return _stat(fileNameAdjusted, (struct _stat*)statInfo);
#else
    return _stat32(fileNameAdjusted, statInfo);
#endif
}
