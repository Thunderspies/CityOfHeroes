#include <stdio.h>
#include "utilitieslib/utils/timing.h"
#include "utilitieslib/utils/error.h"
#include "utilitieslib/components/EArray.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "utilitieslib/utils/utils.h"
//#include "Common.h"

#include <share.h>
#include <io.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <direct.h>
#include <errno.h>
#include "utilitieslib/assert/assert.h"
#include "utilitieslib/utils/fileutil.h"
#include <fcntl.h>
#include "utilitieslib/utils/wininclude.h"
#include <process.h>

#ifndef _XBOX
    #include <ShlObj.h>
#endif

#include "utilitieslib/utils/mathutil.h"
#include "utilitieslib/components/EString.h"
#include "utilitieslib/components/StashTable.h"
#include "utilitieslib/network/netio.h"
#include "zlib/zlib.h"
#include "utilitieslib/utils/sysutil.h"
#include "utilitieslib/utils/strings_opt.h"
#include "utilitieslib/utils/osdependent.h"
#include "utilitieslib/utils/log.h"

#undef _beginthreadex

#ifdef ENABLE_LEAK_DETECTION
// Copied from gc.h
      GC_API uintptr_t GC_CALL GC_beginthreadex(
                        void * /* security */, unsigned /* stack_size */,
                        unsigned (__stdcall *)(void *),
                        void * /* arglist */, unsigned /* initflag */,
                        unsigned * /* thrdaddr */);

#define _beginthreadex GC_beginthreadex
#endif

static bool g_isConsoleOpen = false;

char *loadCmdline(char *cmdFname,char *buf,int bufsize)
{
    FILE    *file;
    int        len;

    memset(buf,0,bufsize);
    file = fopen(cmdFname,"rt");
    if (file)
    {
        fgets(buf, bufsize-1, file);
        fclose(file);
        len = (int)strlen(buf);
        if (len && buf[len-1] == '\n')
            len--;
        buf[len] = ' ';
        buf[len+1] = 0;
    }
    return buf;
}

void makefullpath_s(const char *dir,char *full,size_t full_size)
{
    char    base[_MAX_PATH];

    fileGetcwd(base, _MAX_PATH );
    concatpath_s(base,dir,full,full_size);
    forwardSlashes(full);
}

#ifdef _XBOX
int mkdir( const char * path )
{
    int nRetCode;

    nRetCode = 0;
    if ( !CreateDirectory( path, NULL ) )
    {
        switch ( GetLastError() )
        {
            xcase ERROR_ALREADY_EXISTS:
                nRetCode = EEXIST;

            xcase ERROR_PATH_NOT_FOUND:
            xcase ERROR_CANNOT_MAKE:
                nRetCode = ENOENT;

            xcase ERROR_ACCESS_DENIED:
                nRetCode = EACCES;
                break;

            default:
                nRetCode = EACCES;
                break;
        }
    }
    return nRetCode;
}
#endif

// Given a path to a file, this function makes sure that all directories
// specified in the path exists.
void mkdirtree(char const* in_path)
{
    char path[MAX_PATH];
    char    *s;
    strcpy(path,in_path);
#ifdef _XBOX
    backSlashes(path);
#else
    forwardSlashes(path);
#endif

    s = path;
    for(;;)
    {
#ifdef _XBOX
        s = strchr(s,'\\');
#else
        s = strchr(s,'/');
#endif
        if (!s)
            break;
        *s = 0;
        mkdir(path);
#ifdef _XBOX
        *s = '\\';
#else
        *s = '/';
#endif
        s++;
    }
}

void rmdirtree(char *path)
{
    char    buf[1000],*s;
    struct _finddata32_t finddata;
    long hfile;
    forwardSlashes(path);

    strcpy(buf,path);
    for(;;)
    {
        int empty=1;
        s = strrchr(buf,'/');
        if (!s || strlen(s)<=3)
            break;
        *s = 0;
        // Verify the directory is empty (it won't be if this is a mount point
        strcat(buf, "/*.*");
        hfile = _findfirst32(buf, &finddata);
        *strrchr(buf, '/')=0;
        if (hfile!=-1L) {
            do {
                if (!(strcmp(finddata.name, ".")==0||strcmp(finddata.name, "..")==0)) {
                    empty=0;
                }
            } while( _findnext32( hfile, &finddata) == 0 );
            _findclose(hfile);
        }
        if (empty) {
            _rmdir(buf);
        } else {
            break;
        }
    }
}

/* Function rmdirtreeExInternal()
 *    The worker function of rmdirtreeExInternal().  This function recursively
 *    deletes contents of the given path.
 */
static void rmdirtreeExInternal(char* path, int forceRemove)
{
    struct stat status;

    // If the specified path doesn't exist, do nothing.
    if(stat(path, &status))
    {
        return;
    }

    if(forceRemove)
    {
        _chmod(path, _S_IREAD | _S_IWRITE);
    }
    
    // If the path is a directory, enumerate all items in the directory.
    // Recursively process all items in the directory.
    if(status.st_mode & _S_IFDIR)
    {
        char buffer[MAX_PATH];
        int handle;
        struct _finddata32_t finddata;


        concatpath(path, "*.*", buffer);

        handle = _findfirst32(buffer, &finddata);
        if(handle != -1) 
        {
            do
            {
                if (strcmp(finddata.name, ".")==0 || strcmp(finddata.name, "..")==0)
                    continue;
                concatpath(path, finddata.name, buffer);
                rmdirtreeExInternal(buffer, forceRemove);
            } while(_findnext32(handle, &finddata) == 0);
            _findclose(handle);
        }

        _rmdir(path);
    }
    // If the specified path is a file, remove it.
    else
    {
        remove(path);
    }
}

#ifndef _XBOX

/* Function rmdirtreeEx()
 *    Recursively delete all directories and files starting at the specified path.
 *    This function will not perform a recursive delete if the operation will cause
 *    important windows system directories to be deleted.    
 */
unsigned int systemDirectories[] = 
{
    CSIDL_WINDOWS,
    CSIDL_SYSTEM,
    CSIDL_PROGRAM_FILES,
};
void rmdirtreeEx(char* path, int forceRemove)
{
    char systemPath[MAX_PATH];
    int i;

    // Check the path against important system directories.
    // Do not delete anything in those directories or anything that will
    // result in the deletion of those directories.
    for(i = 0; i < ARRAY_SIZE(systemDirectories); i++)
    {
        if(SHGetSpecialFolderPathA(NULL, systemPath, systemDirectories[i], 0))
        {
            if(filePathBeginsWith(systemPath, path) == 0)
                return;
        }
    }

    rmdirtreeExInternal(path, forceRemove);
}

#endif 

int isFullPath(char *dir)
{
    if (strncmp(dir,"//",2)==0 || strncmp(&(dir[1]),":/",2)==0 || strncmp(&(dir[1]),":\\",2)==0)
        return 1;
    return 0;
}

// Given a path to a directory, this function makes sure that all directories
// specified in the path exists.
int makeDirectories(const char* dirPath)
{
    char    buffer[MAX_PATH];
    char    *s;
    
    Strncpyt(buffer, dirPath);

    s = buffer;

    forwardSlashes(buffer);
    // Look for the first slash that seperates a drive letter from the rest of the
    // path.
    if (isFullPath(buffer))
        s = strchr(s+2,'/');
    else
        s = strchr(s,'/');
    if(!s)
        return 0;
    s++;

    for(;;)
    {
        // Locate the next slash.
        s = strchr(s,'/');
        if (!s){
            if(0 != mkdir(buffer) && EEXIST != errno)
                return 0;
            else
                return 1;
        }

        *s = 0;

        // Try to make the directory.  If the operation didn't succeed and the
        // directory doesn't already exist, the required directory still doesn't
        // exist.  Return an error code.
        if(0 != mkdir(buffer) && EEXIST != errno)
            return 0;

        // Otherwise, restore the string and continue processing it.
        *s = '/';
        s++;
    }
}

// Makes all the directories for given file path.

int makeDirectoriesForFile(const char* filePath)
{
    char buffer[MAX_PATH];
    
    Strncpyt(buffer, filePath);
    
    return makeDirectories(getDirectoryName(buffer));
}

#if defined(_WIN32) && !defined(_XBOX)
int system_detach(char *cmd, int minimized)
{
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    ZeroStruct(&si);
    si.cb = sizeof(si);
    if(minimized)
    {
        si.dwFlags |= STARTF_USESHOWWINDOW;
        si.wShowWindow = SW_MINIMIZE;
    }
    ZeroStruct(&pi);

    //printf("system_detach('%s');\n", cmd);

    if (!CreateProcessA(NULL, cmd,
        NULL, // process security attributes, cannot be inherited
        NULL, // thread security attributes, cannot be inherited
        FALSE, // do NOT let this child inhert handles
        CREATE_NEW_CONSOLE | CREATE_NEW_PROCESS_GROUP,
        NULL, // inherit environment
        NULL, // inherit current directory
        &si,
        &pi))
    {
        // This is sorta a hack, because the dbserver says "run ./mapserver.exe", and
        //  spawnvp will end up finding it in the path if it's not in the current directory
        //  so this functionality is duplicated here.
        if (cmd[0]=='.' && (cmd[1]=='/' ||cmd[1]=='\\')) {
            return system_detach(cmd+2, minimized);
        }
        //printf("Error creating process '%s'\n", cmd);
        return 0;
    } else {
        int pid = (int)pi.dwProcessId;
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        return pid;
    }
}
#elif !defined(_XBOX)
// Warning: this method of spawning (on Windows anyway) makes the child process inherit
//  all of the handles of the parent process, meaning that if the parent process
//  terminates unexpectedly, the TCP ports are still open!
int system_detach(char *cmd, int minimized/*not used*/)
{
    char    *args[50],buf[1000];

    assert(strlen(cmd) < ARRAY_SIZE(buf)-1);
    strcpy(buf,cmd);
    tokenize_line(buf,args,0);
    return _spawnvp( _P_DETACH , args[0], args );
}
#endif

bool gui_disabled = false;
void setGuiDisable(bool disable)
{
    gui_disabled = disable;
}

bool isGuiDisabled()
{
    return gui_disabled;
}

int systemf(const char *cmd,...)
{
    char *tmp = NULL;
    va_list arg;
    int res = 0;

    va_start(arg, cmd);
    estrConcatfv(&tmp,cmd,arg);
    res = system(tmp);
    va_end(arg);
    estrDestroy(&tmp);
    return res;
}


bool isConsoleOpen()
{
    return g_isConsoleOpen;
}


#ifndef _XBOX
#ifdef setvbuf
#undef setvbuf
#endif
#undef fclose
static void fixStdio(int fh, DWORD std_handle)
{
    intptr_t os_handle = (intptr_t)GetStdHandle(std_handle);

    if (_isatty(fh) && os_handle != _get_osfhandle(fh))
    {
        switch (fh)
        {
        case 0:
            *stdin = *_fdopen(fh, "r");
            break;
        case 1:
            *stdout = *_fdopen(fh, "w");
            break;
        case 2:
            *stderr = *_fdopen(fh, "w");
            break;
        }
    }
}

void newConsoleWindow()
{
    g_isConsoleOpen = true;

    if(isGuiDisabled() || IsUsingCider()) return;

    if (AllocConsole())
    {
        fixStdio(0, STD_INPUT_HANDLE);
        fixStdio(1, STD_OUTPUT_HANDLE);
        fixStdio(2, STD_ERROR_HANDLE);

        freopen("CONIN$", "r", stdin);
        freopen("CONOUT$", "w", stdout);
        freopen("CONOUT$", "w", stderr);
    }

    setvbuf( stdout, NULL, _IONBF, 0 );
    setvbuf( stderr, NULL, _IONBF, 0 );
}

void consoleBringToTop(void)
{
    if(isGuiDisabled() ||IsUsingCider()) return;
    BringWindowToTop(compatibleGetConsoleWindow());
}

void setConsoleTitle(char *msg)
{
    static    char    last_buf[1000];

    if(isGuiDisabled() ||IsUsingCider()) return;
    if (!msg)
        return;
    if (strcmp(last_buf,msg)==0)
        return;
    Strncpyt(last_buf,msg);
    SetConsoleTitleA(msg);
}


#define FG_MASK 0x0F
#define BG_MASK 0xF0
#define FG_DEFAULT_COLOR (COLOR_RED | COLOR_GREEN | COLOR_BLUE)

void consoleSetColor(ConsoleColor fg, ConsoleColor bg) {
    static WORD oldcolor = FG_DEFAULT_COLOR;
    static HANDLE console_handle = NULL;

    if(isGuiDisabled() ||IsUsingCider()) return;
    if (console_handle==NULL) {
        console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
        if (console_handle==NULL) return;
    }

    if (bg==COLOR_LEAVE) {
        bg = oldcolor & BG_MASK;
    } else {
        bg <<= 4;
    }
    if (fg==COLOR_LEAVE) {
        fg = oldcolor & FG_MASK;
    }
    oldcolor = fg | bg;
    SetConsoleTextAttribute(console_handle, oldcolor);
}

void consoleSetDefaultColor()
{
    consoleSetColor(FG_DEFAULT_COLOR, 0);
}


#define MAX_SIZE    16*1024
CHAR_INFO buffer[MAX_SIZE];
COORD bufferSize;
CHAR_INFO *consoleGetCapturedData(void) {
    return buffer;
}
void consoleGetCapturedSize(COORD* coord) {
    if(coord){
        *coord = bufferSize;
    }
}

void consoleCapture(void)
{
    HANDLE hConsole;
    COORD bufferCoord = {0,0};
    SMALL_RECT rect = {0, 0, 20, 20};
    CONSOLE_SCREEN_BUFFER_INFO consoleScreenBufferInfo;
    BOOL b;

    if(isGuiDisabled() ||IsUsingCider()) return;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    GetConsoleScreenBufferInfo(hConsole, &consoleScreenBufferInfo);
    bufferSize = consoleScreenBufferInfo.dwSize;
    bufferSize.X = MIN(bufferSize.X, 110);
    bufferSize.Y = MIN(bufferSize.Y, MAX_SIZE / bufferSize.X);
    do {
        rect.Right = bufferSize.X;
        rect.Top = 0;
        rect.Bottom = bufferSize.Y;
        if (rect.Bottom > consoleScreenBufferInfo.dwCursorPosition.Y+1) {
            rect.Bottom = consoleScreenBufferInfo.dwCursorPosition.Y+1;
        } else if (rect.Bottom < consoleScreenBufferInfo.dwCursorPosition.Y) {
            int dy = consoleScreenBufferInfo.dwCursorPosition.Y - rect.Bottom + 1;
            rect.Top += dy;
            rect.Bottom += dy;
        }

        b = ReadConsoleOutput(
            hConsole,
            buffer,
            bufferSize,
            bufferCoord,
            &rect);
        if (!b) {
            bufferSize.Y /= 2;
        }

    } while (!b && bufferSize.Y);
    if (!bufferSize.Y)
        return;
    bufferSize.Y = rect.Bottom - rect.Top;
//    printf("read : %dx%d characters\n", bufferSize.X, bufferSize.Y);
//    for (i=0; i<bufferSize.Y; i++) {
//        for (j=0; j<bufferSize.X; j++) {
//            printf("%c", buffer[i*bufferSize.X + j]);
//        }
//    }
}

void consoleGetDims(COORD* coord)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO consoleScreenBufferInfo;
    COORD ret;

    if(isGuiDisabled()) return;

    GetConsoleScreenBufferInfo(hConsole, &consoleScreenBufferInfo);
    ret.X = consoleScreenBufferInfo.srWindow.Right - consoleScreenBufferInfo.srWindow.Left + 1;
    ret.Y = consoleScreenBufferInfo.srWindow.Bottom - consoleScreenBufferInfo.srWindow.Top + 1;
    
    *coord = ret;
}

// disable the close button, set the window size, also set the buffer size
void consoleInit(int minWidth, int minHeight, int bufferLines)
{
    COORD coord;
    COORD desiredBufferSize;
    COORD maxWindowSize;
    HANDLE console_handle;
    CONSOLE_SCREEN_BUFFER_INFO consoleScreenBufferInfo;
    SMALL_RECT rect;
    SMALL_RECT desiredWindowSize;
    int retries = 3;
    BOOL isMinimized;
    HWND hDlg = GetConsoleWindow();
    HANDLE hStdIn = GetStdHandle(STD_INPUT_HANDLE);
    BOOL ret;
    DWORD mode;

    // Disable close button because it does an unavoidably dirty close on Windows 7.
    // Use Ctrl+C instead.
    DeleteMenu(GetSystemMenu(hDlg, FALSE), SC_CLOSE, MF_BYCOMMAND);

    // print time
    printf_stderr("started %s\n", timerGetTimeString());

    // Disable QuickEdit because an accidental mouse click could pause a server indefinately.
    ret = GetConsoleMode(hStdIn, &mode);
    if (ret && mode & ENABLE_QUICK_EDIT_MODE)
    {
        printf_stderr("detected QuickEdit, disabling...\n");
        mode &= ~ENABLE_QUICK_EDIT_MODE;
        mode |= ENABLE_EXTENDED_FLAGS; // MSDN for SetConsoleMode says you need this
        ret = SetConsoleMode(hStdIn, mode);
        devassert(ret);
    }

    if(isGuiDisabled() ||IsUsingCider()) return;

    // See if we are minimized
    isMinimized = IsIconic(hDlg);

    // If we are minimzed, restore the window so SetConsoleWindowInfo() will work
    if (isMinimized)
    {
        ShowWindow(hDlg, SW_RESTORE);
    }

    console_handle = GetStdHandle(STD_OUTPUT_HANDLE);

    GetConsoleScreenBufferInfo(console_handle, &consoleScreenBufferInfo);
    maxWindowSize = GetLargestConsoleWindowSize(console_handle);

    // desired buffer size
    if (isProductionMode()) {
        desiredBufferSize.X = minWidth;
    } else {
        desiredBufferSize.X = max(consoleScreenBufferInfo.dwSize.X, minWidth);
    }
    
    desiredBufferSize.Y = bufferLines ? bufferLines : 9999; // ab: always maximize this.

    // desired window size
    desiredWindowSize.Top = 0;
    desiredWindowSize.Left = 0;
    if (isProductionMode()) {
        desiredWindowSize.Right = minWidth-1;
        desiredWindowSize.Bottom = minHeight-1;
    } else {
        desiredWindowSize.Right = max(consoleScreenBufferInfo.srWindow.Right, minWidth-1);
        desiredWindowSize.Bottom = max(consoleScreenBufferInfo.srWindow.Bottom, minHeight-1);
    }

    desiredWindowSize.Right = min(maxWindowSize.X - 1, desiredWindowSize.Right);
    // JP: height of GetLargestConsoleWindowSize is too tall, 75% of that is more reasonable
    desiredWindowSize.Bottom = min(3 * (maxWindowSize.Y - 1) / 4, desiredWindowSize.Bottom);

    while ((consoleScreenBufferInfo.dwSize.X != desiredBufferSize.X ||
            consoleScreenBufferInfo.dwSize.Y != desiredBufferSize.Y ||
            (consoleScreenBufferInfo.srWindow.Right - consoleScreenBufferInfo.srWindow.Left) != desiredWindowSize.Right ||
            (consoleScreenBufferInfo.srWindow.Bottom - consoleScreenBufferInfo.srWindow.Top) != desiredWindowSize.Bottom) && retries)
    {
        coord = desiredBufferSize;
        SetConsoleScreenBufferSize(console_handle, coord);

        rect = desiredWindowSize;
        if (desiredWindowSize.Bottom < consoleScreenBufferInfo.srWindow.Bottom)
        {
            rect.Top = consoleScreenBufferInfo.srWindow.Bottom - desiredWindowSize.Bottom;
            rect.Bottom = consoleScreenBufferInfo.srWindow.Bottom;
        }

        SetConsoleWindowInfo(console_handle, TRUE, &rect);

        GetConsoleScreenBufferInfo(console_handle, &consoleScreenBufferInfo);
        retries--;
    }

    // Re-minimize the window if we started out minimized
    if (isMinimized)
    {
        ShowWindow(hDlg, SW_MINIMIZE);
    }
}

int consoleYesNo(void) {
    char ch;

    if(isGuiDisabled() ||IsUsingCider())
    {
        printf("Console input not supported under Cider or when running with -nogui.  Returning false\n");
        return 0;
    }

    do {
        if (tolower(ch=(char)_getch())=='n' || ch=='\n' || ch=='\r') { printf("\n"); return 0; }
    } while (ch!='y');
    printf("\n");
    return 1;
}

void printfColor(int color, const char *str, ...)
{
    va_list arg;
    consoleSetColor(color, 0);
    va_start(arg, str);
    vprintf(str, arg);
    va_end(arg);
    consoleSetColor(COLOR_RED|COLOR_GREEN|COLOR_BLUE, 0);
}

char *getUserName()
{
    static int gotUserName = 0;
    static char    name[1000];
    int        name_len = sizeof(name);

    if(!gotUserName)
    {
        if(!GetUserNameA(name,&name_len))
            name[0] = '\0';
        gotUserName = 1;
    }
    return name;
}

char *getHostName() {
    static char hostname[80];
    if (gethostname(hostname, sizeof(hostname)) == SOCKET_ERROR)
        return "Error getting hostname";
    return hostname;
}

#else
void printfColor(int color, const char *str, ...)
{
    // Just do a normal printf
    va_list arg;
    va_start(arg, str);
    vprintf(str, arg);
    va_end(arg);    
}

void consoleSetColor(ConsoleColor fg, ConsoleColor bg) {
}


void consoleSetDefaultColor()
{
}

char *getUserName()
{
    static char name[1024];
    strcpy(name,"FAKEUSER");
    return name;
}

#endif

static char lastmatch[MAX_PATH];
char *getLastMatch() { // Returns the portion that matched in the last call
    return lastmatch;
}

static bool eq(const char c0, const char c1) {
    return c0==c1 || (c0=='/' && c1=='\\') || (c0=='\\' && c1=='/') || toupper((unsigned char)c0)==toupper((unsigned char)c1);
}

// This version of simpleMatch doesn't do the weird prefix matching thing, so "the" does not match "then"
bool simpleMatchExact(const char *exp, const char *tomatch)
{
    int l1, l2, i;

    if (strchr(exp, '*'))
        return simpleMatch(exp, tomatch);
    
    l1 = (int)strlen(exp);
    l2 = (int)strlen(tomatch);

    if (l1 != l2)
        return 0;

    for (i = 0; i < l1; i++)
    {
        if (!eq(exp[i], tomatch[i]))
            return 0;
    }

    return 1;
}

// simpleMatch assumes that exp is a prefix, so simpleMatch("the", "then") is true
bool simpleMatch(const char *exp, const char *tomatch) {
    char exp2Buffer[1000];
    char *exp2;
    char *pre;
    char *post = 0;
    char *star;
    bool matches = true;
    if(strlen(exp) < 1000) {
        Strcpy(exp2Buffer, exp);
        exp2 = exp2Buffer;
    } else {
        exp2 = strdup(exp);
    }
    pre = exp2;
    star = strchr(exp2, '*');
    if (star) {
        post = star+1;
        *star=0;
        if (strchr(post, '*')) {
            static bool printed_error=false;
            if (!printed_error) {
                printed_error=true;
                printf("Error: filespec \"%s\" contains more than one wildcard\n", exp);
            }
        }
    } else {
        post = exp2+strlen(exp2); // point to a null string
    }
    if (eq(*tomatch, '/') && !eq(*pre, '/')) tomatch++;
    while (*pre && *tomatch && eq(*pre, *tomatch)) {
        pre++;
        tomatch++;
    }
    if (*pre) { // could not match prefix
        matches=false;
    } else { // check postfix
        int postidx=(int)strlen(post);
        int tomatchidx=(int)strlen(tomatch);
    
        strcpy(lastmatch, tomatch);
        if (postidx>tomatchidx) { // post is longer than the remaining tomatch
            matches=false;
        } else {
            while (postidx>=0 && eq(post[--postidx], tomatch[--tomatchidx]));
            if (postidx!=-1) {
                matches=false;
            } else {
                lastmatch[tomatchidx+1]=0;
            }
        }
    }
    if(exp2 != exp2Buffer){
        free(exp2);
    }
    return matches;
}


bool matchExact(const char *exp, const char *tomatch)
{
    bool ret;
    const char *pExp, *pStr;
    pExp=exp;
    pStr=tomatch;
    // Match up until first star or things don't match
    for (; *pExp && *pStr && *pExp!='*' && eq(*pExp, *pStr); pExp++, pStr++);
    if (*pExp=='*') {
        // Recursively try and match
        pExp++; // Now points at first character to match
        if (!*pExp) {
            // Matches!
            ret = true;
        } else {
            ret = false;
            for (; *pStr; pStr++) {
                if (eq(*pExp, *pStr) && matchExact(pExp, pStr)) {
                    ret = true;
                    break;
                }
            }
        }
    } else {
        // EOS or didn't match
        if (eq(*pExp, *pStr))
            ret = true;
        else
            ret = false;
    }
    return ret;
}

// Slow O(n^m) where n is number of characters in tomatch and m is the number of wildcards + 1
bool match(const char *exp, const char *tomatch)
{
    bool ret;
    if (!strchr(exp, '*')) {
        size_t len = strlen(exp)+2;
        char *exp2=malloc(len);
        strcpy_s(exp2, len, exp);
        if (!strchr(exp2, '*'))
            strcat_s(exp2, len, "*");
        ret = matchExact(exp2, tomatch);
        free(exp2);
    } else {
        ret = matchExact(exp, tomatch);
    }
    return ret;
}


void OutputDebugStringf(const char *fmt, ... ) { // Outputs a string to the debug console (useful for remote debugging, and outputing stuff we don't want users to see)
    va_list va;
    char buf[4096]={0};

    va_start(va, fmt);
    _vsnprintf(buf, ARRAY_SIZE(buf)-1, fmt, va);
    va_end(va);

    if (IsDebuggerPresent()) {
        OutputDebugStringA(buf);
    } 
    else
    {
        printf("%s", buf);
    }
}

static void cleanFileName(char buffer[], size_t buffer_size, const char* source, const char* dir, int* dirLenParam)
{
    int dirLen;
    char* pos;

    // dir is of the format: "/DIR/"
    // dir must be uppercase.

    if(!*dirLenParam)
    {
        // Initialize length.
        
        *dirLenParam = (int)strlen(dir);
    }
    
    dirLen = *dirLenParam;

    strcpy_s(buffer, buffer_size, source);
    _strupr_s(buffer, buffer_size);
    forwardSlashes(buffer);
    pos = strstri(buffer, dir);
    if(pos)
    {
        // Found dir somewhere in the string.
        pos += dirLen;
    }
    else if( !strncmp( buffer, dir + 1, dirLen - 1 ) )
    {
        // Found dir (without leading slash) at beginning of string.
        pos = buffer + dirLen - 1;
    }
    if (pos)
        memmove(buffer, pos, strlen(pos)+1);
}

//Not, strictly speaking, a generic utility, but lots of things need it, so I put it here
// clean up forward slashes, uppercase, remove up to and including /FX/
// gets a relative path to requested FX object
void fxCleanFileName_s(char buffer[], size_t buffer_size, const char* source)
{
    static int dirLen;
    char cTempBuffer[512];
    char* pcFrontOfBuffer = &cTempBuffer[0];
    
    cleanFileName(cTempBuffer, ARRAY_SIZE(cTempBuffer), source, "/FX/", &dirLen);

    // Somehow, a lot (hundreds?) of leading slashes got sprinkled throughout ChildFx fields
    // in the fx data, on the CoV cd, so rather than try to patch everyone we're just going
    // to remove leading slashes when we clean up the filename...

    while ( *pcFrontOfBuffer == '/')
        ++pcFrontOfBuffer;

    strcpy_s(buffer, buffer_size, pcFrontOfBuffer);
}

//TO DO merge this function and above function -- pass in string
//Not, strictly speaking, a generic utility, but lots of things need it, so I put it here
// clean up forward slashes, uppercase, remove up to and including /FX/
// gets a relative path to requested FX object
void seqCleanSeqFileName(char buffer[], size_t buffer_size, const char* source)
{
    static int dirLen;
    
    cleanFileName(buffer, buffer_size, source, "/SEQUENCERS/", &dirLen);
}


/* Function encodeStringn()
*  Escapes a limited number of characters in a string (limits output string length)
*/
const char *escapeStringn(const char *instring, unsigned int len)
{
    char *ret = (char*)escapeString(instring);
    assert(len > 0);
    if (strlen(ret) >= len) {
        // Need to destroy the 'character' at position len-1, this might be the second character of an escape sequence, though!
        char *delme = &ret[len-1];
        bool dellast = true;
        char *cursor = delme-1;
        while (*cursor == '\\' && cursor >= ret) {
            dellast = !dellast;
            cursor--;
        }
        if (dellast) {
            *delme = 0;
        } else {
            *(delme-1)=0;
        }
    }
    return ret;
}


/* Function decodeString()
*    Decodes a string, replacing \'ed codes with their characters
*  Warning: not thread safe at all.
*  Returns a pointer to some static memory where the temporary result is stored
*/
char* unescapeDataToBuffer(char *dst, const char *src, int *outlen)
{
    const char *c;
    char *out;
    for(c = src, out = dst; *c; c++)
    {
        if(*c == '\\')
        {
            c++;
            switch(*c)
            {
                xcase 'n':    *out++='\n';
                xcase 'r':    *out++='\r';
                xcase 't':    *out++='\t';
                xcase 'q':    *out++='\"';
                xcase 's':    *out++='\'';
                xcase '\\':    *out++='\\';
                xcase 'p':    *out++='%';
                xcase '0':    *out++='\0';
                xcase '\0':    *out++=*c; // End of string in an escape!  Probably bad, maybe truncation happened, add nothing
                xcase 'd':    *out++='$';

                xdefault:
                    printf("Warning while decoding string '%s': contains invalid escape code '%c'!\n", src, *c);
                    // just copy verbatim
                    *out++='\\';
                    *out++=*c;
            }
        }
        else
        {
            *out++=*c;
        }
    }
    *out = '\0';

    if(outlen)
        *outlen = out - dst;

    return dst;
}

char* unescapeStringToBuffer(char *dst, const char *src)
{
    return unescapeDataToBuffer(dst, src, NULL);
}

const char *unescapeData(const char *instring,int *outlen)
{
    static char *ret=NULL;
    static int retlen=0;
    int deslen;

    if(!instring)
    {
        if (outlen)
            *outlen = 0;
        return NULL;
    }
    deslen = (int)strlen(instring)+1;
    if (retlen < deslen) { // We need a bigger buffer to return the data
        if (ret)
            free(ret);
        if (deslen<256) deslen = 256; // Allocate at least 256 bytes the first time
        ret = malloc(deslen);
        retlen = deslen;
    }

    return unescapeDataToBuffer(ret, instring, outlen);
}

const char *unescapeString(const char *instring)
{
    if(!instring)
    {
        return NULL;
    }
    return unescapeData(instring,0);
}

int strIsAlpha(const unsigned char* str)
{
    if(!str)
        return 0;

    while(*str != '\0')
    {
        if(!isalpha(*str))
            return 0;
        str++;
    }

    return 1;
}

int strIsNumeric(const unsigned char* str)
{
    if(!str)
        return 0;

    while(*str != '\0')
    {
        if(!isdigit(*str))
            return 0;
        str++;
    }

    return 1;
}

int strHasSQLEscapeChars(const unsigned char* str)
{
    if(str)
    {
        while(*str != '\0')
        {
            // Newlines, carriage returns, tabs, single and double quotes,
            // percentage signs and dollar signs all need to be quoted
            // no matter where they appear in SQL statements.
            if(*str == '\n' || *str == '\r' || *str == '\t' ||
                *str == '\"' || *str == '\'' || *str == '%' || *str == '$')
            {
                return 1;
            }

            str++;
        }
    }

    return 0;
}

int strEndsWith(const char* str, const char* ending)
{
    int strLength;
    int endingLength;
    if(!str || !ending)
        return 0;

    strLength = (int)strlen(str);
    endingLength = (int)strlen(ending);

    if(endingLength > strLength)
        return 0;

    if(stricmp(str + strLength - endingLength, ending) == 0)
        return 1;
    else
        return 0;
}

char *changeFileExt_s(const char *fname,const char *new_extension,char *buf, size_t buf_size)
{
    char    *s;

    strcpy_s(buf,buf_size,fname);
    s = strrchr(buf,'.');
    if (strrchr(buf,'/') < s && strrchr(buf,'\\') < s)
        *s = 0;
    strcat_s(buf,buf_size,new_extension);
    return buf;
}

void changeFileExtEString(const char *fname, const char *new_extension, char **outputBuffer)
{
    char *s;

    estrPrintCharString(outputBuffer, fname);
    s = strrchr(*outputBuffer,'.');
    if (strrchr(*outputBuffer,'/') < s && strrchr(*outputBuffer,'\\') < s)
        estrRemove(outputBuffer, s - *outputBuffer, estrLength(outputBuffer) - (s - *outputBuffer));
    estrConcatCharString(outputBuffer, new_extension);
}


int strStartsWith(const char* str, const char* start)
{
    if(!str || !start)
        return 0;

    return strnicmp(str, start, strlen(start))==0;
}

int printPercentageBar(int filled, int total) {
    int i;
    for (i=0; i<total; i++) {
        printf("%c", (i<filled)?0xdb:0xb0);
    }
    return total;
}

void backSpace(int num, bool clear) {
    int i;
    if (clear) {
        for (i=0; i<num; i++) 
            printf("%c %c", 8, 8);
    } else {
        for (i=0; i<num; i++) 
            printf("%c", 8);
    }

}

char *printDate_s(__time32_t date, char *buf, size_t buf_size)
{
    __time32_t now = _time32(NULL);
    struct tm time;
    struct tm today;

    if (_localtime32_s(&time, &date)) {
        sprintf_s(buf, buf_size, "INVALID");
        return buf;
    }

    _localtime32_s(&today, &now);

    if (today.tm_year == time.tm_year && today.tm_mday==time.tm_mday && today.tm_mon==time.tm_mon) {
        sprintf_s(buf, buf_size, "Today, %02d:%02d:%02d", time.tm_hour, time.tm_min, time.tm_sec);
    } else if (today.tm_year == time.tm_year && today.tm_yday-6<=time.tm_yday && today.tm_yday>=time.tm_yday) {
        int dd = today.tm_yday-time.tm_yday;
        sprintf_s(buf, buf_size, "%d day%s ago, %02d:%02d:%02d", dd, (dd==1)?"":"s", time.tm_hour, time.tm_min, time.tm_sec);
    } else {
        sprintf_s(buf, buf_size, "%04d %02d/%02d %02d:%02d:%02d", time.tm_year+1900, time.tm_mon+1, time.tm_mday, time.tm_hour, time.tm_min, time.tm_sec);
    }
    return buf;
}

char *strncpyta(char **buf, const char *s, int n) // Does a strncpy, and allocates the buffer if null, and null terminates
{
    assert(buf);
    if (*buf==NULL) {
        *buf = malloc(n);
    }
    strncpy_s(*buf, INT_MAX, s, n);
    (*buf)[n-1]=0; // null terminate
    return *buf;
}

//#define STR_OVERRUN_DEBUG

char *strncpyt(char *dst, const char *src, int dst_size) // Does a strncpy, null terminates, truncates without asserting
{
    strncpy_s(dst, dst_size, src, dst_size - 1);
    dst[dst_size-1] = 0;
#ifdef STR_OVERRUN_DEBUG
    {
        // Fills all of the area the caller claims is available with a fill, to catch people passing in too small of buffer sizes
        size_t len = strlen(dst)+1;
        if (dst_size > len) {
            memset(dst + len, 0xf9, dst_size - len - 1);
        }
    }
#endif
    return dst;
}

#if _MSC_VER < 1400
// VS2005 has these functions already
errno_t strcpy_s(char *dst, size_t dst_size, const char *src) // Same as strncpyt
{
    errno_t res;
    if(isDevelopmentMode())
        assert(strlen(src) <= dst_size - 1); // VS2005 asserts on truncating with strcpy_s, devassert for now
    res = strncpy_s(dst, dst_size, src, dst_size - 1);
    dst[dst_size-1] = 0;
#ifdef STR_OVERRUN_DEBUG
    {
        // Fills all of the area the caller claims is available with a fill, to catch people passing in too small of buffer sizes
        size_t len = strlen(dst)+1;
        if (dst_size > len) {
            memset(dst + len, 0xf9, dst_size - len - 1);
        }
    }
#endif
    return res;
}

errno_t strcat_s(char *dst, size_t dst_size, const char *src)
{
    strncat_count(dst, src, (int)dst_size);
    return NO_ERROR;
}

errno_t strncpy_s(char* dst, size_t dst_size, const char* src, size_t count)
{
    assert(count <= dst_size);
    strncpy(dst, src, count);
    assert(dst_size > 0);
    assert(count >= 0);
    if(count < dst_size)
        dst[count]=0; // null terminate
    return NO_ERROR;
}
#else // functions to make VS2005 not a pain to use
int open_cryptic(const char* filename, int oflag)
{
    int fd;

    _sopen_s(&fd, filename, oflag, _SH_DENYNO, _S_IREAD | _S_IWRITE);

    return fd;
}
#endif

char *strcpy_unsafe(char *dst, const char *src)
{
    register char *c=dst;
    register const char *s=0;
    for (s=src; *s; *c++=*s++);
    *c=0;
    return dst;
}

// Just so we can use it in the Command window in visual studio
size_t slen(const char *s)
{
    return strlen(s);
}

int mcmp(const void *a, const void *b, size_t size)
{
    return memcmp(a, b, size);
}

void* loadCrashRptDll()
{
    HMODULE hDll = 0;
    LPCSTR dllName = "CrashRpt.dll";
    return LoadLibraryA(dllName);
}

char *printUnit_s(char *buf, size_t buf_size, S64 val)
{
    char        *units = "";

    if (val < 1024)
    {
        sprintf_s(buf, buf_size, "%I64dB",val);
        return buf;
    }
    else if (val < 1048576)
    {
        val /= 1024 / 10;
        units = "K";
    }
    else if (val < 1073741824)
    {
        val /= 1048576 / 10;
        units = "M";
    }
    else if (val < 1099511627776)
    {
        val /= 1073741824 / 10;
        units = "G";
    }
    else if (val < 1125899906842624)
    {
        val /= 1099511627776 / 10;
        units = "T";
    }
    else if (val < 1152921504606846976)
    {
        val /= 1125899906842624 / 10;
        units = "P";
    }
    if (val >= 1024)
        sprintf_s(buf, buf_size, "%d%s",(int)(val/10),units);
    else
        sprintf_s(buf, buf_size, "%d.%d%s",(int)(val/10),(int)(val%10),units);
    return buf;
}

char *printTimeUnit_s(char *buf, size_t buf_size, U32 val)
{
    if (val < 3600)
    {
        sprintf_s(buf, buf_size, "%d:%02d",val/60,val%60);
    }
    else
    {
        sprintf_s(buf, buf_size, "%d:%02d:%02d",val/3600,(val/60)%60,val%60);
    }
    return buf;
}

char* getCommaSeparatedInt(S64 x){
    static int curBuffer = 0;
    // 27+'\0' is the max length of a 64bit value with commas.
    static char bufferArray[10][30]; 
    
    char* buffer = bufferArray[curBuffer = (curBuffer + 1) % 10];
    int j = 0;
    int addSign = 0;

    buffer += ARRAY_SIZE(bufferArray[0]) - 1;

    *buffer-- = 0;
    
    if(x < 0){
        x = -x;
        addSign = 1;
    }

    do{
        *buffer-- = '0' + (char)(x % 10);

        x = x / 10;

        if(x && ++j == 3){
            j = 0;
            *buffer-- = ',';
        }
    }while(x);
    
    if(addSign){
        *buffer-- = '-';
    }

    return buffer + 1;
}

// Moved from contextQsort
void swap (void *vpa, void *vpb, size_t width)
{
    char *a = (char *)vpa;
    char *b = (char *)vpb;
    char tmp;

    if ( a != b )
        /* Do the swap one character at a time to avoid potential alignment
        problems. */
        while ( width-- ) {
            tmp = *a;
            *a++ = *b;
            *b++ = tmp;
        }
}

void removeLeadingAndFollowingSpaces(char * str)
{
    int i, len = (int)strlen(str);
    for(i = 0; str[i] == ' '; i++)
        len--;
    while(len && str[len+i-1] == ' ')
        len--;
    memmove(str, str+i, len);
    str[len] = '\0';
}

void removeLeadingAndFollowingSpacesUnlessAllSpaces(char * str)
{
    int i = 0;
    while (str[i])
    {
        if (str[i] != ' ')
        {
            removeLeadingAndFollowingSpaces(str);
            return;
        }

        ++i;
    }
}

char* strInsert( char * dest, const char * insert_ptr, const char *insert )
{
    static char  *buffer = NULL;
    static int buffer_size = 0;
    int requiredBufferSize;

    if(!dest || !insert_ptr || !insert)
        return NULL;

    requiredBufferSize = (int)strlen(dest)+(int)strlen(insert);
    if( buffer_size < requiredBufferSize+1 )
    {
        buffer_size = requiredBufferSize+1;
        buffer = realloc( buffer, buffer_size );
    }

    strncpyt( buffer, dest, insert_ptr-dest + 1);
    strcat_s( buffer, buffer_size, insert );
    strcat_s( buffer, buffer_size, insert_ptr );

    return buffer;
}

char *strchrInsert( const char * dest, const char * insert, int character )
{
    static char *str = NULL;
    char *curptr;
    char *dup = strdup(dest);
    char *origdup = dup;
    estrClear(&str);
    
    curptr = strchr(dup,character);

      while(curptr)
    {
        char tmp;
        curptr++;
        tmp = *curptr;
         *curptr = '\0';
        estrConcatMinimumWidth(&str, dup, curptr-dup+1);
        *curptr = tmp;
        estrConcatCharString(&str, insert);
        dup = curptr;
        curptr = strchr(dup,character);
    }
    estrConcatCharString(&str, dup);
    free(origdup);
    return str;
}

char *strstrInsert( const char * src, const char * find, const char * replace )
{
    char *str = NULL;
    char *curptr;
    char *dup = strdup(src);
    char *last_end;

    estrClear(&str);
    curptr = strstri(dup,find);
    last_end = dup;

    while(curptr)
    {
        estrConcatFixedWidth(&str, last_end, curptr-last_end);
        estrConcatCharString(&str, replace);
        curptr += strlen(find);
        last_end = curptr;
        curptr = strstri(last_end, find);
    }

    estrConcatCharString(&str, last_end);
    free(dup);
    return str;
}

void strchrReplace( char *dest, int find, int replace )
{
    char * curptr = strchr(dest, find);
    char * destptr = dest;
    while(curptr)
    {
        *curptr = replace;
        destptr = curptr+1;
        curptr = strchr( destptr, find );
    }
}

char *strchrReplaceStatic(char const *str, char c, char r) // make static copy of string.
{
    THREADSAFE_STATIC char *s = NULL;
    THREADSAFE_STATIC_MARK(s);
    for(;*str;str++)
    {
        if(*str != c)
            estrConcatChar(&s,*str);
        else
            estrConcatChar(&s,r);
    }
    return s;
}

char *strchrRemoveStatic(char const *str, char c)
{
    THREADSAFE_STATIC char *s = NULL;
    THREADSAFE_STATIC_MARK(s);
    if(!s)
        estrCreate(&s);
    estrClear(&s);
    for(;*str;str++)
    {
        if(*str != c)
            estrConcatChar(&s,*str);
    }
    return s;
}

void strReplaceWhitespace( char *dest, int replace )
{
    // not using isspace : detects \r, \n
    strchrReplace(dest,' ',replace);
    strchrReplace(dest,'\t',replace);
}

void strToFileName(char *dest)
{
    strReplaceWhitespace(dest, '_');
    strchrReplace(dest,'\r',0);
    strchrReplace(dest,'\n',0);
    // add new features here.
}


void strstriReplace_s(char *src, size_t src_size, const char *find, const char *replace)
{
    char *sec2;
    char *endsec1 = strstri(src, find);
    if (!endsec1)
        return;

    sec2 = strdup(endsec1 + strlen(find));
    strcpy_s(endsec1, src_size - (endsec1-src), replace);
    strcat_s(src, src_size, sec2);
    free(sec2);
}



// chatserver gets reserved names from mapservers
StashTable receiveHashTable(Packet * pak, int mode)
{        
    StashTable table;
    int i, count;

    count = pktGetBitsPack(pak, 1);

    table = stashTableCreateWithStringKeys(count, mode);

    for(i=0;i<count;i++)
    {
        char * s = pktGetString(pak);
        assert(s);
        stashAddPointer(table, s, NULL, false);
    }

    return table;
}

void sendHashTable(Packet * pak, StashTable table)
{
    StashElement element;
    StashTableIterator it;

    assert(table);

    pktSendBitsPack(pak, 1, stashGetValidElementCount(table));

    stashGetIterator(table, &it);
    while(stashGetNextElement(&it, &element))
    {
        const char * name = stashElementGetStringKey(element);
        assert(name);
        pktSendString(pak, name);
    }
}

char *incrementName(unsigned char *name, unsigned int maxlen)
{
    unsigned int len = (unsigned int)strlen(name);
    if (!isdigit(name[len-1])) {
        // Need to add a digit to the end
        if (len < maxlen) {
            strcat_s(name, maxlen+1, "1");
        } else {
            name[len-1]='1';
        }
    } else {
        // Already ends in a number, increment!
        unsigned char *s = &name[len-1];
        S64 value;
        char valuebuf[33];
        unsigned int valuelen;
        while (isdigit(*s) && s >= name) {
            s--;
            len--;
        }
        s++;
        value = _atoi64(s) + 1;
        name[len]='\0'; // clear out digit for strcat later
        sprintf_s(SAFESTR(valuebuf), "%I64d", value);
        if (strlen(valuebuf) > maxlen) { // Loop
            value = 0;
            sprintf_s(SAFESTR(valuebuf), "%I64d", value);
        }
        valuelen = (int)strlen(valuebuf);
        while (valuelen + len > maxlen) {
            len--;
            name[len]='\0';
        }
        strcat_s(name, maxlen+1, valuebuf);
    }
    return name;
}

void *zipData(const void *src,U32 src_len,U32 *zip_size_p)
{
    U8        *zip_data;
    U32        zip_size;

    zip_size = (U32)(src_len*1.0125+12); // 1% + 12 bigger, so says the zlib docs
    zip_data = malloc(zip_size);
    compress2(zip_data,&zip_size,src,src_len,5);
    zip_data = realloc(zip_data,zip_size);
    *zip_size_p = zip_size;
    return zip_data;
}

void stableSort(void *base, size_t num, size_t width, const void* context,
                int (__cdecl *comp)(const void* item1, const void* item2, const void* context))
{
    char *tmp = (char*)malloc(width);
    char *start = (char*)base;
    char *p = NULL;
    char *q = NULL;
    char *end = start + num*width;
    
    for(p = start + width; p < end; p += width )
    {
        for( q = p; q > start; q -= width ) 
        {
            char *r = q - width;
            if( 0 < comp(r, q, context))
            {
#define SSRT_MOVE(dst, src) memmove(dst,src,width)
                SSRT_MOVE(tmp, r);
                SSRT_MOVE(r,q);
                SSRT_MOVE(q,tmp);
#undef SSRT_MOVE
            }
            else
            {
                break;
            }
        }
    }

    // --------------------
    // finally

    free(tmp);
}

// how bsearch _should_ have been prototyped..
// returns location where this key should be inserted into array (can be equal to n)
size_t bfind(void* key, void* base, size_t num, size_t width, int (__cdecl *compare)(const void*, const void*))
{
    size_t low, high;
    char* arr = (char*)base;

    low = 0;
    high = num;
    while (high > low)
    {
        size_t mid = (high + low) / 2;
        int comp = compare(key, &arr[width*mid]);
        if (comp == 0)
            low = high = mid;
        else if (comp < 0)
            high = mid;
        else if (mid == low)
            low = high;
        else
            low = mid;
    }
    return low;
}

// important note: disabling Wow64 redirection screws up Winsock
void disableWow64Redirection(void)
{
    PVOID dummy;
    typedef BOOL (WINAPI *Wow64DisableWow64FsRedirectionType)(PVOID*);
    Wow64DisableWow64FsRedirectionType Wow64DisableWow64FsRedirection;
        
    Wow64DisableWow64FsRedirection = (Wow64DisableWow64FsRedirectionType)GetProcAddress(LoadLibraryA("kernel32"), "Wow64DisableWow64FsRedirection");
    if (Wow64DisableWow64FsRedirection)
        Wow64DisableWow64FsRedirection(&dummy);
}

void enableWow64Redirection(void)
{
    typedef BOOLEAN (WINAPI *Wow64EnableWow64FsRedirectionType)(BOOLEAN);
    Wow64EnableWow64FsRedirectionType Wow64EnableWow64FsRedirection;

    Wow64EnableWow64FsRedirection = (Wow64EnableWow64FsRedirectionType)GetProcAddress(LoadLibraryA("kernel32"), "Wow64EnableWow64FsRedirection");
    if (Wow64EnableWow64FsRedirection)
        Wow64EnableWow64FsRedirection(TRUE);
}


// Functions to convert between stat time and UTC.
//
// Problem: stat and _stat and _findfirst32 (et al) return time in UTC theoretically, but in
//         real life they return an adjusted value based on whether the current time is in DST.
//
// Known issues: 1. curTime can be checked at a different time than stat, which can straddle the DST switch.
//               2. The one hour overlap when the clock is turned back an hour will cause problems.
//                  Hopefully no one is creating files then?
//
// Resolution: Let's stop using stat, except that I think the "old version control system" relies on it.
//
// The rule for thes function is as follows:
//
// --------------------------------------------------------------------------------
// Currently in DST   |   Stat time In DST   |   Seconds to add to stat time
// --------------------------------------------------------------------------------
// no                 |   no                 |   0
// no                 |   yes                |   3600
// yes                |   no                 |   -3600
// yes                |   yes                |   0
// --------------------------------------------------------------------------------

__time32_t statTimeToUTC(__time32_t statTime){
    __time32_t        curTime = _time32(NULL);
    struct tm    t;
    struct tm    cur;
    S32            statIsDST;
    
    if(_localtime32_s(&t, &statTime)){
        // localtime returns NULL if statTime is invalid somehow.
        // I dunno what an invalid time is, but I had it happen once.
        // _localtime32_s returns an error code, so this is different now
        
        return statTime;
    }
    
    statIsDST = t.tm_isdst;
    
    // I'll assume that curTime is always valid.

    _localtime32_s(&cur, &curTime);
    
    return statTime + ((S32)(statIsDST?1:0) - (S32)(cur.tm_isdst?1:0)) * 3600;
}

__time32_t statTimeFromUTC(__time32_t utcTime){
    __time32_t        curTime = _time32(NULL);
    struct tm    t;
    struct tm    cur;
    S32            utcIsDST;
    
    if(_localtime32_s(&t, &utcTime)){
        // localtime returns NULL if statTime is invalid somehow.
        // I dunno what an invalid time is, but I had it happen once.
        // _localtime32_s returns an error code, so this is different now
        
        return utcTime;
    }
    
    utcIsDST = t.tm_isdst;

    _localtime32_s(&cur, &curTime);

    return utcTime - ((S32)(utcIsDST?1:0) - (S32)(cur.tm_isdst?1:0)) * 3600;
}


// The rule for thes function is as follows:
//
// --------------------------------------------------------------------------------
// Currently in DST   |   Stat time In DST   |   Seconds to add to stat time
// and a non-Samba drive
// --------------------------------------------------------------------------------
// no                 |   N/A                |   0
// yes                |   N/A                |   -3600
// --------------------------------------------------------------------------------
//__time32_t statTimeToPstTime(__time32_t statTime)
//{
//    return statTime + pstGetTimeAdjust();
//}
//
//__time32_t statTimeFromPstTime(__time32_t pstTime)
//{
//    return pstTime - pstGetTimeAdjust();
//}



void getAutoDbName_s(char *server_name, size_t server_name_size) // Modifies buffer
{
    if (stricmp(server_name, "test")==0) {
        char *auto_file = fileAlloc("Which Branch Is This.txt", NULL);
        if (auto_file) {
            char *line = auto_file;
            while (line)
            {
                char *args[3];
                int num_args = tokenize_line_safe(line, args, ARRAY_SIZE(args), &line);
                if (num_args != 2)
                    continue;
                if (stricmp(args[0], "AutoDb:")==0)
                    strcpy_s(server_name, server_name_size, args[1]);
            }
            fileFree(auto_file);
        }
    } 
    else if (stricmp(server_name, "!test")==0) 
    {
        // Override to allow specific access to "test"
        strcpy_s(server_name, server_name_size, "test");
    }
}

#ifndef _XBOX

// for COM objects
#pragma comment ( lib, "ole32.lib" )

// creates a shortcut to {file} at {out}
// file - the full path of the file you are linking to
// out - the full path of the shortcut to make
// icon - the index into the {file} of which icon to use for the shortcut
// working_dir - if not null, the directory {file} will be run from
// args - if not null, command line arguments that are passed when {file} is run
// desc - if not null, a description of {file} (for tool tips)
int createShortcut(char *file, char *out, int icon, char *working_dir, char *args, char *desc)
{
    HRESULT hres;
    IShellLinkA *psl;
    CoInitialize(NULL);
    hres = CoCreateInstance(&CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, 
                                 &IID_IShellLink, (void **) &psl); 
    if (SUCCEEDED(hres)) 
    { 
        IPersistFile* ppf; 

        hres = psl->lpVtbl->QueryInterface(psl,&IID_IPersistFile, (void **) &ppf); 
        if (SUCCEEDED(hres)) 
        { 
            char path[MAX_PATH], *tmp;

            hres = psl->lpVtbl->SetPath(psl,file); 

            if ( working_dir )
                psl->lpVtbl->SetWorkingDirectory(psl,working_dir); 
            else
            {
                strcpy(path, file);
                tmp = strrchr(path, '/');
                if ( !tmp )
                    tmp = strrchr(path, '\\');
                if ( !tmp )
                    return 0;
                *tmp = 0;
                psl->lpVtbl->SetWorkingDirectory(psl,path); 
            }
            psl->lpVtbl->SetShowCmd(psl,SW_SHOWNORMAL);
            if (icon >= 0) psl->lpVtbl->SetIconLocation(psl,file,icon); 
            if (args) psl->lpVtbl->SetArguments(psl,args); 
            if (desc) psl->lpVtbl->SetDescription(psl,desc); 

            if (SUCCEEDED(hres)) 
            { 
                WCHAR wsz[1024];
                wsz[0]=0; 

                if ( !strEndsWith(out, ".lnk") )
                {
                    char newOut[MAX_PATH];
                    sprintf_s(SAFESTR(newOut), "%s.lnk", out);
                    MultiByteToWideChar(CP_ACP, 0, newOut, -1, wsz, 1024); 
                }
                else
                    MultiByteToWideChar(CP_ACP, 0, out, -1, wsz, 1024);

                hres=ppf->lpVtbl->Save(ppf,(const WCHAR*)wsz,TRUE); 
                if ( !SUCCEEDED(hres) )
                    return 0;
            } 
            else
                return 0;
            ppf->lpVtbl->Release(ppf); 
        } 
        psl->lpVtbl->Release(psl); 
    }
    else
        return 0;

    return 1;
}
#endif

#ifndef _XBOX
int spawnProcess(char *cmdLine, int mode)
{
    char *args[100];
    int ret;
    tokenize_line_quoted(cmdLine, args, 0);
    ret = _spawnv(mode, args[0], args);
    if ( ret < 0 )
    {
        switch ( errno )
        {
        case (E2BIG):
            assert("Argument list is too long"==0);
            break;
        case (EINVAL):
            assert("Invalid mode specified"==0);
            break;
        case (ENOENT):
            assert("Could not find process"==0);
            break;
        case (ENOEXEC):
            assert("Not an executable"==0);
            break;
        case (ENOMEM):
            assert("Not enough memory to spawn process"==0);
            break;
        }
    }

    return ret;
}
#endif

const char* unescapeAndUnpack(const char *data)
{
    static char *s_buf;
    static U32 s_buf_len;

    const char *zipped;
    U32 origsize, packsize;
    int ret;

    if (!data || !data[0])
        return "";

    zipped = unescapeData(data, &packsize);
    origsize = *((U32*)zipped);
    if(s_buf_len < origsize)
    {
        if(s_buf)
            free(s_buf);
        s_buf = malloc(origsize);
        s_buf_len = origsize;
    }
    ret = uncompress(s_buf, &origsize, zipped+4, packsize-4);
    return s_buf;
}

const char* packAndEscape(const char *str)
{
    const char *escaped;
    char    *zipped;
    U32        zipsize,origsize;

    origsize = (int)strlen(str)+1;
    zipped = zipData(str,origsize,&zipsize);
    zipped = realloc(zipped,zipsize+4);
    memmove(zipped+4,zipped,zipsize);
    *((U32*)zipped) = origsize;
    escaped = escapeData(zipped,zipsize+4);
    free(zipped);
    return escaped;
}

char* hexStringFromIntList(const int* pIntList, U32 uiNumInts)
{
    U32 uiNumChars = 8 * uiNumInts + 1;
    char* pcResult = calloc(sizeof(char),  uiNumChars);
    U32 uiIntIndex;
    for (uiIntIndex=0; uiIntIndex<uiNumInts; ++uiIntIndex)
    {
        strcatf_s(pcResult, uiNumChars, "%08X", pIntList[uiIntIndex]);
    }
    return pcResult;
}

int* intListFromHexString(const char* pcString, U32 uiNumInts)
{
    int* intList = calloc(sizeof(int), uiNumInts);
    U32 uiIntIndex;
    const char* pcCursor = pcString;
    for (uiIntIndex=0; uiIntIndex<uiNumInts; ++uiIntIndex)
    {
        sscanf(pcCursor,"%8X", &intList[uiIntIndex]);
        pcCursor += 8;
    }
    return intList;
}

static char base64_tbl[] = {
    'A','B','C','D','E','F','G','H',
    'I','J','K','L','M','N','O','P',
    'Q','R','S','T','U','V','W','X',
    'Y','Z','a','b','c','d','e','f',
    'g','h','i','j','k','l','m','n',
    'o','p','q','r','s','t','u','v',
    'w','x','y','z','0','1','2','3',
    '4','5','6','7','8','9','+','/'
};

static char base64_tbl_inv[256] = {0};

// 0: aa aaaa
// 1: aa bbbb
// 2: bb bbcc
// 3: cc cccc
U8 *base64_decode(U8 **pres, int *res_len, char *s)
{
    int n;
    U8 *r,*res;
    int i;
    int tmp;
    int q;

    if(!s)
        return NULL;

    if(!base64_tbl_inv['B'])
    {
        memset(base64_tbl_inv,'=',ARRAY_SIZE(base64_tbl_inv));
        for( i = 0; i < ARRAY_SIZE(base64_tbl); ++i ) 
        {
            char c = base64_tbl[i];
            base64_tbl_inv[c] = i;
        }
    }

    n = (int)strlen(s);
    res = r = *pres = realloc(*pres,n*3/4);
    q = 0;
    for( i = 0; i < n; i++) 
    {
        if(s[i] == '=')
            break;

        tmp = base64_tbl_inv[s[i]];
        switch (i%4)
        {
        case 0:
            q = tmp;
            q <<= 2;            // q = aaaa aa00
            break;      
        case 1:                 // tmp = aa bbbb
            q += (tmp>>4);      // q = aaaa aaaa
            *r++ = q;
            q = (tmp&0xf) << 4;   // q = bbbb 0000
            break;
        case 2:                 // tmp = bb bbcc
            q += (tmp>>2);      // q = bbbb bbbb
            *r++ = q;
            q = (tmp&3)<<6;     // q = cc00 0000
            break;
        case 3:
            q += (tmp&0x3f);    // q = cccc cccc
            *r++ = q;
            break;
        };
    }
    if(res_len)
        *res_len = r - res;
    return res;
}


// a bytes = 1234 5678 bits
// with 64 values, you can represent 6 bits
// so 4 6 bit values = 24 bits
// and 3 8 bit values = 24 bits
char *base64_encode(char **pres, U8 *s, int length)
{
    int i;
    int buffLen;
    U8 *res, *r;

    if(!s)
        return NULL;
    
    buffLen = (( 4 * (length + 2) / 3) + 1);
    res = r = *pres = realloc(*pres,buffLen);
    for (i = 0; i < length; i += 3)
    {
        *r++ = base64_tbl[s[0] >> 2];
        *r++ = base64_tbl[((s[0] & 3) << 4) + (s[1] >> 4)];
        *r++ = base64_tbl[((s[1] & 0xf) << 2) + (s[2] >> 6)];
        *r++ = base64_tbl[s[2] & 0x3f];
        s += 3;
    }
    if (i == length + 1)
        *(r - 1) = '=';
    else if (i == length + 2)
        *(r - 1) = *(r - 2) = '=';
    *r = '\0';
    return res;
}

///////////// Threads ////////////////////////////////////////////////////////

// see http://msdn2.microsoft.com/en-us/library/xcb2z8hs.aspx
#define MS_VC_EXCEPTION 0x406D1388

typedef struct tagTHREADNAME_INFO
{
    DWORD dwType; // Must be 0x1000.
    LPCSTR szName; // Pointer to name (in user addr space).
    DWORD dwThreadID; // Thread ID (-1=caller thread).
    DWORD dwFlags; // Reserved for future use, must be zero.
} THREADNAME_INFO;

void SetThreadName( DWORD dwThreadID, LPCSTR szThreadName)
{
    THREADNAME_INFO info;
    info.dwType = 0x1000;
    info.szName = szThreadName;
    info.dwThreadID = dwThreadID;
    info.dwFlags = 0;

    __try
    {
        RaiseException( MS_VC_EXCEPTION, 0, sizeof(info)/sizeof(DWORD), (ULONG_PTR*)&info );
    }
    __except(EXCEPTION_CONTINUE_EXECUTION)
    {
    }
}

uintptr_t x_beginthreadex(void *security, unsigned stack_size,
                          unsigned ( __stdcall *start_address )( void * ),
                          void *arglist, unsigned initflag, unsigned *thrdaddr,
                          char* filename, int linenumber)
{
    unsigned tid = 0;
    uintptr_t handle;
    if(!thrdaddr)
        thrdaddr = &tid;
    handle = _beginthreadex(security,stack_size,start_address,arglist,initflag,thrdaddr);
    if(handle)
    {
        char threadname[128];
        assertYouMayFreezeThisThread((HANDLE)handle,*thrdaddr);
        sprintf(threadname, "%s(%d)", filename, linenumber);
        SetThreadName(*thrdaddr,threadname);
    }
    return handle;
}

int isHexChar(char h)
{
    return INRANGE(h,'0','9'+1)
        || INRANGE(h,'A','Z'+1)
        || INRANGE(h,'a','z'+1);
}

void writeConsole(OutputLevel level, const char *format, ...) {
#ifndef _DEBUG
    if (level == OUTPUT_DEBUG) {
        return;
    }
#endif // _DEBUG

    if (level == OUTPUT_VERBOSE && errorGetVerboseLevel() == 0) {
        return;
    }

    va_list args = 0;
    SYSTEMTIME systemTime = { 0 };
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

    GetSystemTime(&systemTime);
    va_start(args, format);
    printf("[%02i-%02i-%02i %02i:%02i:%02i.%03i] [", systemTime.wYear, systemTime.wMonth, systemTime.wDay, systemTime.wHour, systemTime.wMinute, systemTime.wSecond, systemTime.wMilliseconds);
    switch (level) {
    case OUTPUT_INFO:
        printf("INFO");
        break;
    case OUTPUT_VERBOSE:
        consoleSetColor(COLOR_BLUE, 0);
        printf("VERBOSE");
        break;
    case OUTPUT_DEBUG:
        consoleSetColor(COLOR_GREEN, 0);
        printf("DEBUG");
        break;
    case OUTPUT_WARNING:
        consoleSetColor(COLOR_YELLOW, 0);
        printf("WARNING");
        break;
    case OUTPUT_ERROR:
        consoleSetColor(COLOR_RED, 0);
        printf("ERROR");
        break;
    }
    consoleSetDefaultColor();
    printf("] ");
    vprintf(format, args);
    printf("\n");
    va_end(args);
}
