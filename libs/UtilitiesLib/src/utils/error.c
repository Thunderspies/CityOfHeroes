#include "utilitieslib/utils/error.h"
#include "utilitieslib/components/StringCache.h"
#include "utilitieslib/utils/log.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include "utilitieslib/stdtypes.h"
#include "utilitieslib/assert/assert.h"
#include "utilitieslib/utils/wininclude.h"
#include "utilitieslib/utils/timing.h"
#include "utilitieslib/utils/utils.h"
#include "utilitieslib/utils/file.h"
#include "utilitieslib/components/HashFunctions.h"
#include "utilitieslib/components/EString.h"
#include "utilitieslib/utils/mathutil.h"
#include <zlib.h>
#include "utilitieslib/components/StashTable.h"
#include "utilitieslib/components/MemoryPool.h"
#include "utilitieslib/utils/FolderCache.h"
#include "utilitieslib/utils/sysutil.h"
#include "utilitieslib/utils/textparser.h"
#include "utilitieslib/components/earray.h"
#include "utilitieslib/utils/strings_opt.h"


#include <psapi.h>
#pragma comment(lib, "psapi")

ErrorCallback fatalErrorfCallback = NULL;
ErrorCallback errorfCallback = NULL;

void FatalErrorfSetCallback(ErrorCallback func)
{
    fatalErrorfCallback = func;
}

void ErrorfSetCallback(ErrorCallback func)
{
    errorfCallback = func;
}

static ErrorCallback errorfCallbackStack[1];
static int errorfCallbackStackCount=0;
void ErrorfPushCallback(ErrorCallback func)
{
    assert(errorfCallbackStackCount < ARRAY_SIZE(errorfCallbackStack));
    errorfCallbackStack[errorfCallbackStackCount++] = errorfCallback;
    errorfCallback = func;
}

void ErrorfPopCallback(void)
{
    assert(errorfCallbackStackCount>0);
    errorfCallback = errorfCallbackStack[--errorfCallbackStackCount];
}

static int verbose_level,temp_verbose_level;
static int load_line_pos;

static void loadLineClear()
{
    if (load_line_pos)
    {
        load_line_pos = 0;
        printf("\n");
    }
}

void printWinErr(char* functionName, char* filename, int lineno, int err){
    char buffer[1024];

    FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, 0, err, 0, buffer, 1024, 0);
    printf_stderr("%s, %s, %i: %s", functionName, filename, lineno, buffer);
}

char *lastWinErr()
{
    static char buf[2000];
    FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), 0, buf, ARRAY_SIZE( buf ) , NULL);
    return buf;
}

//----------------------------------------------------------------
// FatalErrorf
//----------------------------------------------------------------
NORETURN FatalErrorf(char const *fmt, ...)
{
    char str[1000];
    va_list ap;

    va_start(ap, fmt);
    vsprintf(str, fmt, ap);
    va_end(ap);

    if(fatalErrorfCallback)
    {
        fatalErrorfCallback(str);
    } 
    else 
    {
        char buffer[1024];
        printf_stderr("\n\n%s\n", str);
        printf_stderr("hit return to exit..\n");
        gets(buffer);
        assertmsg(0, str);
        LOG(LOG_CRASH, LOG_LEVEL_ALERT, 0, "Shutting down from fatal error \"%s\"", str);
        logShutdownAndWait();
        exit(-1);
    }
}

NORETURN FatalErrorFilenamef(const char *filename, char const *fmt, ...) {
    const char *user = "UNKNOWN";
    char str[1024];
    char str2[1024];
    va_list ap;
    char buf[1024];

    if (strcmp(user, "Not in database")==0) {
        sprintf_s(SAFESTR(buf), "SCRIPTS.LOC/%s", filename);
        user = "UNKNOWN";
        if (strcmp(user, "Not in database")!=0) {
            // Found something better!
            filename = buf;
        }
    }
    va_start(ap, fmt);
    vsprintf(str, fmt, ap);
    va_end(ap);

    if (filename==NULL) {
        strcpy(str2, str);
    } else {
        sprintf_s(SAFESTR(str2), "File: %s\n%s%s\n\n%s", filename, g_lastAuthorIntro, user, str);
    }
    FatalErrorf("%s", str2);
}



//----------------------------------------------------------------
// Errorf
//----------------------------------------------------------------

static bool errorf_forceshow = false;
static void forceShowThisError(bool show)
{
    errorf_forceshow = show;
}
int errorWasForceShown(void)
{
    return errorf_forceshow? 1: 0;
}

//A little bit of dementia to allow Errorf from different parts of the code
//to get their own number of displays 
#define MAX_DIALOG_IDS 10
static int currErrorfLocationCount;
static int dialogsDone[MAX_DIALOG_IDS][2];
static int dialogsDoneCount;
static int giveup;

void ErrorfResetCounts(void)
{
    memset(dialogsDone, 0, sizeof(dialogsDone));
    dialogsDoneCount = 0;
    giveup = 0;
}
void ErrorfFL( const char * file, int line )
{
    int spot = -1, hash, i;

    if (errorf_forceshow) {
        // This is the user's own error, don't increment any counts, he will still see 4 errors of other peoples, but all of his own
        currErrorfLocationCount = 0;
        return;
    }

    if( giveup )
    {
        currErrorfLocationCount = 1000;
        return;
    }

    if( dialogsDoneCount >= MAX_DIALOG_IDS )
    {
        ErrorfInternal("Holy Crap that's a lot of errors.  I give up.");
        giveup = currErrorfLocationCount = 1000;
        return;
    }

    hash = hashString(file, false) + line;
    for( i = 0 ; i < dialogsDoneCount; i++ )
    {
        if( hash == dialogsDone[i][0] )
            spot = i;
    }

    if( spot == -1 )
    {
        spot = dialogsDoneCount++;
        dialogsDone[spot][0] = hash;
    }
    currErrorfLocationCount = ++(dialogsDone[spot][1]);
}

void ErrorfInternal(char const *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    ErrorvInternal(fmt, ap);
    va_end(ap);
}

static int queueing_errors = 0;
static char *error_log_queue = 0;


void ErrorvBeginQueueing()
{
    queueing_errors = 1;
}

void ErrorvEndQueueing()
{
    queueing_errors = 0;
}

void ErrorvInternal(char const *fmt, va_list ap)
{
    char **str;
    STATIC_THREAD_ALLOC(str);

    estrClear(str);
    estrConcatfv(str, fmt, ap);
    loadLineClear();

    if(errorfCallback) 
    {
        errorfCallback(*str);
    } 
    else 
    {
        printf_stderr("%s\n", *str);
    }
}

void ErrorFilenamevInternal(const char *filename, char const *fmt, va_list ap) {
    const char *user;

    char buf[1024];
    bool mine=false;
    char **str;
    STATIC_THREAD_ALLOC(str);
    char **str2;
    STATIC_THREAD_ALLOC(str2);

    estrClear(str);
    estrClear(str2);
    estrConcatfv(str, fmt, ap);

    if (filename==NULL || !filename[0]) 
    {
        estrPrintCharString(str2, *str);
        mine = true;
    }
    else 
    {
        user = "UNKNOWN";
        if (strcmp(user, "Not in database")==0) 
        {
            sprintf_s(SAFESTR(buf), "SCRIPTS.LOC/%s", filename);
            user = "UNKNOWN";
            if (strcmp(user, "Not in database")!=0) 
            {
                // Found something better!
                filename = buf;
            }
        }
        estrConcatf(str2, "File: %s\nLast Author/Status:%s\n%s", filename, user, *str);
        errorLogFileHasError(filename);
    }
    if (mine)
        forceShowThisError(true);
    ErrorfInternal("%s", *str2);
    if (mine)
        forceShowThisError(false);
}

void ErrorFilenamefInternal(const char *filename, char const *fmt, ...) {
    va_list ap;

    va_start(ap, fmt);
    ErrorFilenamevInternal(filename, fmt, ap);
    va_end(ap);
}

void ErrorFilenameDupInternal(const char *filename1, const char *filename2, const char *key, const char *type)
{
    const char *newest_file;

    const char *user1 = "UNKNOWN";
    const char *user2 = "UNKNOWN";

    if (fileNewer(filename1, filename2))
        newest_file = filename1;
    else
        newest_file = filename2;
        
    errorLogFileHasError(filename1);
    errorLogFileHasError(filename2);

    ErrorFilenamefInternal(newest_file, "Duplicate %s %s found in:\n%s (%s) and\n%s (%s)", type, key, filename1, user1, filename2, user2);
}

bool DelayExpired( const char *filename, int days )
{
    __time32_t changedTime, systemTime, daysToSeconds;
    
    if (days == -1)
        return false;

    // this returns the number of seconds elapsed since midnight of january 1, 1970,
    // the single most useful measure of time in the history of the universe...
    changedTime = fileLastChanged( filename );
    systemTime = _time32(NULL);

    systemTime -= changedTime;

    // the number of seconds in the delay
    daysToSeconds = (__time32_t)(days * 86400);

    if ( systemTime >= daysToSeconds )
        return true;

    return false;
}


int ErrorfCount()
{
    return currErrorfLocationCount;
}

int filelog_vprintf(char *fname,char const *fmt, va_list ap)
{
    char    buf_base[2048],*buf = buf_base;
    char    date_buf[100];
    char    *newFormat;
    size_t    newFormat_size;
    static    int uid=0; // UID for tracking relative order of separate log entries in separate logs
    int        uid_local = ++uid,buf_size = sizeof(buf_base);
    static    int pid=0;

    // Make sure there is some default directory to output the log to.
    if (!log_default_dir[0])
        logSetDir("");

#ifndef _XBOX
    if (!pid) {
        pid = _getpid();
    }
#endif

    // Dump the entry to a default log file if one hasn't been specified.
    if (!fname)
        fname = log_default_fname;

    timerMakeDateString(date_buf);
    newFormat_size = strlen(fmt)+256;
    newFormat = _alloca(newFormat_size);
    sprintf_s(newFormat, newFormat_size, "%s %5d %6d %s", date_buf, pid, uid_local, fmt);
    if (!strchr(fmt,'\n'))
        strcat_s(newFormat,newFormat_size,"\n");
    for(;;)
    {
        if (_vsnprintf_s(buf,buf_size,buf_size-1,newFormat,ap) >= 0) {
            buf[buf_size-1]='\0';
            break;
        }
        if (buf_size != sizeof(buf_base))
            free(buf);
        buf_size *= 2;
        buf = malloc(buf_size);
    }
    logPutMsg(buf,fname);
    if (errorGetVerboseLevel() == 2)
        printf("%s",buf);
    if (buf_size != sizeof(buf_base))
        free(buf);
    return 1;
}

int filelog_printf(char *fname,char const *fmt, ...)
{
    int result;
    va_list ap;

    va_start(ap, fmt);
    result = filelog_vprintf(fname, fmt, ap);
    va_end(ap);
    return result;
}


static int (*s_log_vprintf_fptr)(char *fname,char const *fmt, va_list ap) = NULL;

LogVprintfFptr set_log_vprintf(LogVprintfFptr log_vprintf_fptr)
{
    LogVprintfFptr orig = s_log_vprintf_fptr;
    s_log_vprintf_fptr = log_vprintf_fptr;
    return orig;
}

int log_vprintf(char *fname,char const *fmt, va_list ap)
{
    int result;
    if(s_log_vprintf_fptr)
        s_log_vprintf_fptr(fname?fname:log_default_fname,fmt,ap);
    result = filelog_vprintf(fname, fmt, ap); // always print locally
    return result;    
}

int log_printf(char *fname,char const *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    LOG_AP(LOG_DEPRECATED, LOG_LEVEL_DEPRECATED, 0, (char*)fmt, ap)
    va_end(ap);
    return 0;
}



void verbose_printf(char const *fmt, ...)
{
    char str[1000];
    va_list ap;

    if (!verbose_level)
        return;

    loadLineClear();
    va_start(ap, fmt);
    vsprintf(str, fmt, ap);
    va_end(ap);

    printf("%s",str);
}

void errorSetVerboseLevel(int v)
{
    verbose_level = v;
}

void errorSetTempVerboseLevel(int v)
{
    temp_verbose_level = v;
}

int errorGetVerboseLevel()
{
    return MAX(verbose_level,temp_verbose_level);
}

#define MAX_DEPTH    10
static int depth=0;
static int loadstart_children=0;
static int load_timer[MAX_DEPTH];
int g_loadstart_depth=0;

void loadstart_printf(const char* fmt, ...)
{
    va_list args;
    int i;

    devassert(g_loadstart_depth < 15);

    if (!load_timer[depth])
        load_timer[depth] = timerAlloc();
    timerStart(load_timer[depth]);
    loadLineClear();
    load_line_pos += depth*2;
    for (i=0; i<load_line_pos; i++) 
        printf(" ");
    va_start(args, fmt);
    load_line_pos += vfprintf(fileGetStdout(),fmt,args);
    va_end(args);
    fflush(fileGetStdout());
    g_loadstart_depth++;
    depth++;
    if (depth==MAX_DEPTH) depth--;
    loadstart_children = 0;
}

void loadupdate_printf(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    load_line_pos += vfprintf(fileGetStdout(),fmt,args);
    va_end(args);
    fflush(fileGetStdout());
}

// Feel free to twiddle with these numbers to get a rainbow of timing colors =)
static struct {
    F32 elapsed;
    int color;
} scale[] = {
    { 0.005    , COLOR_BLACK                    },
    { 0.095    , COLOR_GREEN                    },
    { 0.995    , COLOR_BRIGHT | COLOR_GREEN    },
    { 9.995    , COLOR_BRIGHT | COLOR_YELLOW    },
    { 0.    , COLOR_BRIGHT | COLOR_CYAN        },
//    { 0.    , COLOR_BRIGHT | COLOR_RED        }, // Bright red isn't a good idea for non-errors
};

static char endfmt[16]="(%0.2f)";
void setLoadTimingPrecistion(int digits)
{
    sprintf_s(SAFESTR(endfmt), "(%%0.%df)", digits);
}

#ifndef FINAL
typedef struct PROCESS_MEMORY_COUNTERS_with_EX {
    PROCESS_MEMORY_COUNTERS base;
    SIZE_T PrivateUsage;
} PROCESS_MEMORY_COUNTERS_with_EX;

static void printMemUsage() {
    HANDLE pHandle = GetCurrentProcess();
    PROCESS_MEMORY_COUNTERS_with_EX pmc;
    if (GetProcessMemoryInfo(pHandle, (PROCESS_MEMORY_COUNTERS*)(&pmc), sizeof(pmc)))
    {
        static int lastWorkingSize;
        static int lastPrivateSize;
        int workingSize = pmc.base.WorkingSetSize;
        int privateSize = pmc.PrivateUsage;
        printf("    %dMB (%+dMB)   %dMB (%+dMB)\n", (workingSize >> 20), ((workingSize - lastWorkingSize) >> 20),
            (privateSize >> 20), ((privateSize - lastPrivateSize) >> 20));
        lastWorkingSize = workingSize;
        lastPrivateSize = privateSize;
    }

    CloseHandle(pHandle);
}
#endif

static bool isLoadMemUsageShown = false;
void setShowLoadMemUsage(bool flag)
{
    isLoadMemUsageShown = flag;
}

void loadend_printf(const char* fmt, ...)
{
    int        i;
    va_list args;
    F32        elapsed;

    g_loadstart_depth--;
    depth--;
    if (depth<0) depth=0;
    if(loadstart_children)
    {
        load_line_pos += (depth+1)*2;
        for (i=0; i<load_line_pos; i++) 
            printf(" ");
    }
    va_start(args, fmt);
    load_line_pos += vfprintf(fileGetStdout(),fmt,args);
    va_end(args);
    for(i=load_line_pos;i<70;i++)
        printf(" ");
    elapsed = timerElapsed(load_timer[depth]);
    for (i=0; i<ARRAY_SIZE(scale)-1 && elapsed >= scale[i].elapsed; i++);
    consoleSetFGColor(loadstart_children ? (COLOR_BRIGHT|COLOR_WHITE) : scale[i].color);
    printf(endfmt,elapsed);
    consoleSetFGColor(COLOR_WHITE);
    load_line_pos=0;
    if (depth > 1) {
        timerFree(load_timer[depth]);
        load_timer[depth] = 0;
    }
    loadstart_children=1;

#ifndef FINAL
    if (isLoadMemUsageShown)
        printMemUsage();
    else
#endif
        printf("\n");
}



static char *errorLogFile = "errorLogLastRun";
void errorLogStart(void)
{
    int num;
    char str[128] = "";
    char **ignored;
    char fullpath[MAX_PATH];
    sprintf_s(SAFESTR(fullpath), "%s%s.log", getLogDir(), errorLogFile);
    if (fileSize(fullpath) > 200000) {
        // Reset the log every 200K
        fileForceRemove(fullpath);
    }
    ignored = FolderCacheGetIgnoredPrefixes(&num);
    for (; num; num--) {
        strcat(str, ignored[num-1]);
        if (num>1)
            strcat(str, ",");
    }
    writeConsole(OUTPUT_INFO, "Initialized error log %s", fullpath);
}

void errorLogFileHasError(const char *file)
{
    printf("ERRORLOG FILEERROR: %s\n", file);
}

void errorLogFileIsBeingReloaded(const char *file)
{
    printf("ERRORLOG FILERELOAD: %s\n", file);
}
