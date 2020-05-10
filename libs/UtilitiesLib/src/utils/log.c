#include "utilitieslib/utils/error.h"
#include "utilitieslib/components/stringcache.h"
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
#include "zlib/zlib.h"
#include "utilitieslib/components/StashTable.h"
#include "utilitieslib/components/MemoryPool.h"
#include "utilitieslib/utils/FolderCache.h"
#include "utilitieslib/utils/sysutil.h"
#include "utilitieslib/utils/textparser.h"
#include "utilitieslib/components/earray.h"
#include "utilitieslib/utils/strings_opt.h"
#include "utilitieslib/utils/log.h"
#include "utilitieslib/UtilsNew/lock.h"
#include <psapi.h>

/// Provide up to 2 minutes worth of second interval bins that the incoming log lines can be binned into. 
#define LOG_SECONDS (60*2)

/// Wait until until 60 seconds have passed before flushing out any sorted log lines.
/// @note This value needs synchronized with @ref SECS_LOG_RESEND in logcomm.c, but with additional time for TCP delay.
#define LOG_WINDOW (60)

/// Batch up 1 second of logs between writes.
#define LOG_FLUSH_INTERVAL (1)

/// Read from the log queue for up to 4 seconds between flushes
#define LOG_QUEUE_INTERVAL (4)

/// Delay log rotation checks for up to 10 seconds per sort log block
#define LOG_ROTATE_CHECK_INTERVAL (10)

/// Close old handles if they haven't been written to in the last 10 minutes
#define LOG_HANDLE_EXPIRATION (60*10)

/// The per log file I/O buffer size
#define LOG_IO_BUF_SIZE (8192)

/// Continue writing logs until 3 minutes have passed, then forcefully shutdown the log thread
#define LOG_SOFT_TIMEOUT (3*60)

/// If the log thread hasn't forcefully exited withing 5 minutes, abort anyway
#define LOG_HARD_TIMEOUT (5*60)



char log_default_fname[MAX_PATH];
char log_default_dir[MAX_PATH];
static bool logs_are_timestamped = false;

typedef struct LogData
{
    char * name;
    LogLevel level;
}LogData;

static LogData s_log_data[] =
{
    { "crashDetails",    LOG_LEVEL_IMPORTANT },    //LOG_CRASH,             // Crash reports
    { "debug",            LOG_LEVEL_IMPORTANT },    //LOG_DEBUG_LOG,         // Logs for debugging purposes.  Usually temporary, while the bug is being tracked down.
    { "persist",        LOG_LEVEL_IMPORTANT },    //LOG_TEXT_JOURNAL,         // Logs for the text journaling system
    { "error",            LOG_LEVEL_IMPORTANT },    //LOG_ERROR,             // Errors we want to know about sent here
    { "old",            LOG_LEVEL_IMPORTANT },    //LOG_DEPRECATED,         // Log messages that are probably worthless are sent here, on the off-chance we still need them
    { "TestClient",        LOG_LEVEL_IMPORTANT },    //LOG_TEST_CLIENT,          // Used to generate output from test clients
    { "packets",        LOG_LEVEL_IMPORTANT },    //LOG_NET,

    // DBserver
    { "delete",            LOG_LEVEL_IMPORTANT },    //LOG_DELETE,             // Proof they deleted their character
    { "offline",        LOG_LEVEL_IMPORTANT },    //LOG_OFFLINE,             // Players getting moved out of the db into text
    { "SystemSpecs",    LOG_LEVEL_IMPORTANT },    //LOG_SYSTEM_SPECS,         // Log users system specs when the connect to db
    { "OverloadProtection",    LOG_LEVEL_IMPORTANT },    //LOG_OVERLOAD_PROTECTION,// Overload protection messages
    { "CharSlotApply",    LOG_LEVEL_IMPORTANT },    //LOG_CHAR_SLOT_APPLY,     // Proof they commited a character slot to a shard

    // Mapserver
    { "entity",            LOG_LEVEL_IMPORTANT },    //LOG_ENTITY,             // General log of things on entitys (no chat, powers, or rewards related)
    { "powers",            LOG_LEVEL_IMPORTANT },    //LOG_POWERS,             // Power Info (may be redundant since chat records lots of powers information)
    { "supergroup",        LOG_LEVEL_IMPORTANT },    //LOG_SUPERGROUP,         // supergroup specific things
    { "bug",            LOG_LEVEL_IMPORTANT },    //LOG_BUG,                 // Player reported bugs
    { "chat",            LOG_LEVEL_IMPORTANT },    //LOG_CHAT,                 // Chat
    { "cheaters",        LOG_LEVEL_IMPORTANT },    //LOG_CHEATING,             // Egregious cheats sent here
    { "rewards",        LOG_LEVEL_IMPORTANT },    //LOG_REWARDS,             // Anything to do with player inventory should go here
    { "sze_rewards",    LOG_LEVEL_IMPORTANT },    //LOG_SZE_REWARDS,         // Scripted zone event reward logs
    { "survey",            LOG_LEVEL_IMPORTANT },  // LOG_SURVEY
    { "cmds",            LOG_LEVEL_IMPORTANT },  // LOG_ACCESS_CMDS
    { "internalcmds",    LOG_LEVEL_IMPORTANT },  // LOG_INTERNAL_CMDS
    { "MARTY",            LOG_LEVEL_IMPORTANT    },    //    LOG_MARTY
    { "Admin",            LOG_LEVEL_IMPORTANT },    //LOG_ADMIN,             // Logs admin commands
    { "Performance",    LOG_LEVEL_IMPORTANT },    //LOG_PERFORMANCE
    { "DuplicateMessageIDs",    LOG_LEVEL_IMPORTANT }, // LOG_DUPLICATE_MESSAGEIDS
    // Chatserver

    // Account Server
    { "Account",        LOG_LEVEL_IMPORTANT },    //LOG_ACCOUNT,             // Generic account log
    { "Transaction",    LOG_LEVEL_IMPORTANT },    //LOG_TRANSACTION,        // Account transaction log

    // Arena Server
    { "ArenaEvent",    LOG_LEVEL_IMPORTANT },    //LOG_ARENA_EVENT,             // In depth arena event logs.

    // Auction Server
    { "Auction", LOG_LEVEL_IMPORTANT }, //LOG_AUCTION,
    { "Xaction", LOG_LEVEL_IMPORTANT }, //LOG_AUCTION_TRANSACTION,

    // Mission Server
    { "DeletedArcs",    LOG_LEVEL_IMPORTANT },    //LOG_DELETED_ARCS,
    { "missionserver",    LOG_LEVEL_IMPORTANT },    //LOG_MISSIONSERVER, 

    // RAIDSERVER
    { "iop",    LOG_LEVEL_IMPORTANT },    //LOG_IOP,

    // STATSERVER
    { "statserver",    LOG_LEVEL_IMPORTANT }, //LOG_STATSERVER,

    // Turnstile Server
    { "turnstile", LOG_LEVEL_IMPORTANT },
};

STATIC_ASSERT(ARRAY_SIZE(s_log_data)==LOG_COUNT);

StashTable stLogOnce;

/// A table of all of the log files being written to
static StashTable log_file_structs = NULL;  
/// Statistic tracking the amount of log data being written.
static size_t log_bytes_written = 0;

char* g_lastAuthorIntro = "Last Author/Status:";
int g_disableLastAuthor=0;
int disable_sort_log=0;

static int log_background_sleep_ms = 1;
static volatile int log_background_should_exit = 0;
static volatile int log_background_should_abort = 0;



typedef struct
{
    char    *filename;            // 4 bytes
    char    *delim; // end of line delimiter // 4 bytes
    U32        num_blocks : 29;    // 4 bytes
    U32        zipped : 1;            // ...
    U32        zip_file : 1;        // ...
    U32        text_file : 1;        // slower and will use .txt instead of .log
#ifdef _WIN64
    char    str[4+8];    // two pointers above are actually +8 bytes
#else
    char    str[4+16];
#endif
    // don't put any members after this. 
    // this acts as a start for the contiguous memory in the msg_queue
    // and any structs put after this will get overwritten by log msgs
    // longer than the array size.
} MsgEntry;
STATIC_ASSERT(POW_OF_2(sizeof(MsgEntry)));

static int msg_queue_size=4096,max_realloc_msgs = 65536; // about 1 meg of buffer

static MsgEntry            *msg_queue = NULL;
// used when the reader is accessing queue memory. prevents
// problems in realloc.




/** 
 * Store persistent information about the log files being 
 * written.
 */
typedef struct logFileStruct {
    /// Store the current roll over log name string via @ref allocAddString
    const char * roll_name;
    /// Store the current file handle when the log file is open
    FILE * cached_handle;
    /// A write buffer of @ref LOG_IO_BUF_SIZE bytes for write combining
    char * write_buffer;
    /// The current count of bytes in the @ref write_buffer
    int write_buffer_bytes;
    /// Indicate whether the stream data requies a flush to guarantee that the contents are pushed out to the disk
    int stream_uncommitted;
    /// The last time this log was written to
    U32 time_of_last_write;
} logFileStruct;

typedef struct SortLogBlock SortLogBlock;

typedef struct SortLogBlock
{
    logFileStruct * log_file;
    char    *fname; // owned by this
    char    *base; // owned by this
    size_t  baseSize;
    int        ref_count;
    unsigned short last_log_rotate_sec;
    unsigned char is_timestamped;
} SortLogBlock;

typedef struct
{
    SortLogBlock    *block;
    char            *str;
    int                len;
}  SortLogLine;

typedef struct
{
    int            count;
    int            max;
    SortLogLine    *lines;
} SortLogSlot;

static SortLogSlot    sort_slots[LOG_SECONDS];

static size_t sortMemoryCap = 1 * 1024 * 1024 * 1024; // 1GB by default
static size_t sortMemoryInUse = 0;

static volatile U32        msg_queue_start,msg_queue_end,reader_is_processing;
static volatile int        background_writer_running = 0;



static char*            g_dbserverprependmsg = "_UNKNOWN_          \"_UNKNOWN_\"        0 [DBServer:Start]\n";


int logShouldWrite( LogType type, LogLevel level)
{
#if defined(DISABLE_CHAT_LOGGING)
    if(type == LOG_CHAT) {
        return 0;
    }
#endif

    return INRANGE0(type, LOG_COUNT) && level <= s_log_data[type].level;
}

void logSetLevel( LogType type, LogLevel level )
{
    if( INRANGE0( type, LOG_COUNT) )
        s_log_data[type].level = level;
}

int logLevel( LogType type )
{
    if( INRANGE0( type, LOG_COUNT) )
        return s_log_data[type].level;
    return 0;
}


char *logGetTypeName(LogType type)
{
    return s_log_data[type].name;
}

int logGetTypeFromName( char * name ) // if this gets called frequently, switch to hashtable
{
    int i;
    for( i = 0; i < LOG_COUNT; i++ )
    {
        if( stricmp( name, s_log_data[i].name) == 0 )
            return i;
    }

    return -1;
}

void logSetDir(const char *dir)
{
    char    buf[1000],*s;

    if (!log_default_fname[0])
        sprintf_s(SAFESTR(log_default_fname),"%s.log",dir);
    if (!fileDataDir())
        return;
    if (strncmp(dir,"\\\\",2)==0 || strchr(dir,':'))
    {
        strcpy(log_default_dir,dir);
        if (!strEndsWith(dir,"/") || !strEndsWith(dir,"\\"))
            strcat(log_default_dir,"/");
    }
    else
    {
        strcpy(buf,fileDataDir());
        s = strrchr(buf,'/');
        if (s)
            *s = 0;
        sprintf_s(SAFESTR(log_default_dir),"%s/logs/%s/",buf,dir);
    }

    mkdirtree(log_default_dir);
}

char *getLogDir()
{
    if (!log_default_dir[0])
        logSetDir("");

    return log_default_dir;
}

void logSetUseTimestamped(bool enabled)
{
    logs_are_timestamped = enabled;
}

void logSetDefaultFile(char *fname)
{
    strcpy(log_default_fname,fname);
}


void logSetMsgQueueSize(int max_bytes)
{
    msg_queue_size = (1 << log2i(max_bytes)) / (int)sizeof(MsgEntry);
    max_realloc_msgs = (1 << log2i(max_bytes)) / (int)sizeof(MsgEntry);
}

static SendMsgToLogServerFunc sendMessageToLogServer;
void setSendMsgToLogServerFunc( SendMsgToLogServerFunc func )
{
    sendMessageToLogServer = func;
}

void printv_stderr(const char * msg, va_list va)
{
    vfprintf(fileWrap(stderr), msg, va);
}

void printf_stderr(const char * msg, ...)
{
    va_list va;
    va_start(va, msg);
    printv_stderr(msg, va);
    va_end(va);
}

// either write message locally or send to logserver for writing, do not call directly
// use a macro that checks the log level
void log_msg( LogType type, LogLevel level, int flags, char const* msg  )
{
    char date_buf[100];

    if(!logShouldWrite(type,level)) // redundant if used correctly
    {
        if(flags&LOG_CONSOLE_ALWAYS)
            fputs(msg, stderr);
        return; 
    }

    if( flags & LOG_ONCE )
    {
        int hash = burtlehash3(msg, strlen(msg), DEFAULT_HASH_SEED);
        int time = timerSecondsSince2000();
        if(!stLogOnce)
            stLogOnce = stashTableCreateInt(128);

        // storing the time so we can clean it up later, not storing the string key because I don't really care about collisions
        if( !stashIntAddInt(stLogOnce, hash, time, 0) ) 
        {
            estrDestroy((char**)&msg);
            return;
        }
    }

    if( flags&LOG_CONSOLE || flags&LOG_CONSOLE_ALWAYS )
        fputs(msg, stderr);

    if( flags & LOG_LOCAL || !sendMessageToLogServer )
    {
        char * fname = logGetTypeName( type );
        char * estr = estrTemp();

        timerMakeLogDateStringFromSecondsSince2000(date_buf,timerSecondsSince2000());
        estrConcatFixedWidth(&estr, date_buf, 15);
        estrConcatf(&estr, " %i %s", level, msg);

        // Make sure there is some default directory to output the log to.
        if (!log_default_dir[0])
            logSetDir("");

        logPutMsg(estr,fname);
        estrDestroy(&estr);
    }

    if(sendMessageToLogServer)
        sendMessageToLogServer(type, msg);
}
// all this does is gather arguments for log_msg do not call directly, use macro LOG
void log_va( LogType type, LogLevel level, int flags, char const* fmt, va_list ap  )
{
    char * str = estrTemp();
    int result = 0;

    estrConcatfv(&str, fmt, ap);

    log_msg( type, level, flags, str );

    estrDestroy(&str);
}

// all this does is gather arguments for log_msg do not call directly, use macro LOG
void log_f( LogType type, LogLevel level, int flags, char const* fmt, ...  )
{
    char * str = estrTemp();
    va_list ap;
    int result = 0;

    va_start(ap, fmt);
    estrConcatfv(&str, fmt, ap);
    va_end(ap);

    if (!estrLength(&str) || str[estrLength(&str) - 1] != '\n')
        estrConcatChar(&str, '\n');

    log_msg( type, level, flags, str );

    estrDestroy(&str);
}



static bool logServerStart(char *ftimestamp)
{
    int i;
    FILE *logfile=NULL;
    char fname[MAX_PATH];
    char *exename = getExecutableName();

    // Don't do this if this is not the dbserver or logserver
    if(!(strstr(exename,"logserver") || strstr(exename,"dbserver")))
    {
        return 1;
    }

    sprintf_s(SAFESTR(fname),"%s%s%s%s",log_default_dir,"db",ftimestamp,".begin");
    for(i=0;i<6;i++)
    {
        // Open the log file to append the new log entry.
        logfile = fopen(fname,"a+t");
        if (!logfile)
        {
            // Wait for a second then try again. because another process may be writing to it
            Sleep(1);
            continue;
        }

        break;
    }
    if(!logfile)
    {
        printf("Failed to write to log file (%s): %s\n",fname,fname);
        return 0;
    }
    fwrite(fname, 1, strlen(fname), logfile);
    fclose(logfile);
    return 1;
}

// Looks at the first timestamp in the message to build a timestamp for the file
static bool lognameTimestamp(const char *msg, char *timestamp, size_t timestamp_size, bool exact)
{
    char *p;
    if(!timestamp || !msg || strlen(msg) < 19)
        return false;

    timestamp[0] = 0;
    strcat_s(timestamp,timestamp_size,"_");

    if(msg[2] == '-')
    {
        strncat_s(timestamp,timestamp_size,&(msg[6]),4);
        strcat_s(timestamp,timestamp_size,"-");
        strncat_s(timestamp,timestamp_size,msg,6);
        strncat_s(timestamp,timestamp_size,&(msg[11]),2);
        if(exact)
        {
            strcat_s(timestamp,timestamp_size,"-");
            strncat_s(timestamp,timestamp_size,&(msg[14]),2);
            strcat_s(timestamp,timestamp_size,"-");
            strncat_s(timestamp,timestamp_size,&(msg[17]),2);
        }
        else
        {
            strcat_s(timestamp,timestamp_size,"-00-00");
        }
    }
    else
    {
        strcat_s(timestamp,timestamp_size,"20");
        strncat_s(timestamp,timestamp_size,msg,2);
        strcat_s(timestamp,timestamp_size,"-");
        strncat_s(timestamp,timestamp_size,&(msg[2]),2);
        strcat_s(timestamp,timestamp_size,"-");
        strncat_s(timestamp,timestamp_size,&(msg[4]),2);
        strcat_s(timestamp,timestamp_size,"-");
        strncat_s(timestamp,timestamp_size,&(msg[7]),2);
        if(exact)
        {
            strcat_s(timestamp,timestamp_size,"-");
            strncat_s(timestamp,timestamp_size,&(msg[10]),2);
            strcat_s(timestamp,timestamp_size,"-");
            strncat_s(timestamp,timestamp_size,&(msg[13]),2);
        }
        else
        {
            strcat_s(timestamp,timestamp_size,"-00-00");
        }
    }

    // verify
    p = &(timestamp[1]);
    while(*p)
    {
        if(*p != '-' && (*p < '0' || *p > '9'))
            return false;
        p++;
    }

    return true;
}

// Returns true if the filename is a valid target for the given log name timestamp
static bool isAppropriateLogname(const char *timestamp, const char *fname)
{
    char shortstamp[MAX_PATH];
    strcpy(shortstamp,&(timestamp[1]));
    shortstamp[13] = 0;
    return (NULL != strstr(fname,shortstamp));
}

/**
 * Find the correct open log file for this filename, possibly rotating if it exceeds the size threshold.
 */
static logFileStruct * openAndRotateLogFile(const char *fname_noext, const char *msg, size_t bytes, int compress, int isTimestamped, int isTextFile)
{    
    const char * ext = compress ? ".log.gz" : ".log";

    static bool firstmsg = true;

    int MAX_LOG_BYTES = 500000000;
    bool prepend_startup = false;
    bool validFilename = false;

    int i;
    logFileStruct * log_file;
    char fname_timestamp[MAX_PATH];

    if (isTextFile)
        ext = ".txt";

    if (compress)
        MAX_LOG_BYTES /= 8;

    if(isTimestamped)
        lognameTimestamp(msg,SAFESTR(fname_timestamp),false);

    if (!log_file_structs)
        log_file_structs = stashTableCreateWithStringKeys(100,StashDeepCopyKeys);
    stashFindPointer( log_file_structs, fname_noext, &log_file );
    if (!log_file)
    {
        char fname_new[MAX_PATH];
        strcpy(fname_new, fname_noext);

        if(isTimestamped)
        {
            // Check if we need to go exact because of a restart
            char fname_test[MAX_PATH];
            strcpy(fname_test,fname_new);
            strcat(fname_test,fname_timestamp);
            strcat(fname_test,ext);

            if(fileExists(fname_test))
            {
                isTimestamped = lognameTimestamp(msg,SAFESTR(fname_timestamp),true);
            }

            strcat(fname_new,fname_timestamp);
            if(firstmsg)
            {
                // save this timestamp and drop a .begin file
                if(logServerStart(fname_timestamp))
                {
                    firstmsg = false;
                }
            }
        }
        strcat(fname_new,ext);

        // tag the object to add an initial message
        if(strstr(fname_new, "db_"))
        {
            prepend_startup = true;
        }

        // add the new entry
        log_file = malloc(sizeof(logFileStruct));
        log_file->roll_name = allocAddString(fname_new);
        log_file->cached_handle = NULL;
        log_file->write_buffer = calloc(1, LOG_IO_BUF_SIZE);
        log_file->write_buffer_bytes = 0;
        log_file->stream_uncommitted = 0;
        log_file->time_of_last_write = 0;
        stashAddPointer(log_file_structs, fname_noext, log_file, false);
    }

    validFilename = isTimestamped && isAppropriateLogname(fname_timestamp, log_file->roll_name);

    // Try to open the log file.
    for(i=0;i<6;i++)
    {
        if(!log_file->cached_handle) {
            // Open the log file to append the new log entry.
            // JE: calling AllocAddString here so that we don't thrash memory by strduping the
            //   name of the log file over and over again (which is what the file layer
            //   will do on paths to unknown files (files not in the string cache).
            if (isTextFile)
                log_file->cached_handle = fopen(log_file->roll_name,"a+t");
            else if (compress)
                log_file->cached_handle = fopen(log_file->roll_name,"a+bz3");
            else
                log_file->cached_handle = fopen(log_file->roll_name,"a+b");

            if(log_file->cached_handle) {
                // we do our own buffering
                x_setvbuf(log_file->cached_handle, NULL, _IONBF, 0);
            }
        }

        if (!log_file->cached_handle)
        {
            // Wait for a second then try again. because another process may be writing to it
            Sleep(1);
            continue;
        }

        if (fileGetSize(log_file->cached_handle) < MAX_LOG_BYTES && // too big or not a valid name
            (!isTimestamped || validFilename))
        {
            break;
        }
        
        {
            int        filenum = 1;
            char    *s,roll_name[MAX_PATH],buf[MAX_PATH];
            
            strcpy(buf, log_file->roll_name);
            buf[strlen(buf) - strlen(ext)] = 0;
            s = strrchr(buf,'_');

            if(s)
            {
                // try to redo the timestamp, exact timestamp if the old name was valid 
                if(isTimestamped && lognameTimestamp(msg,SAFESTR(fname_timestamp),validFilename))
                {
                    *s = 0;
                    sprintf_s(SAFESTR(roll_name),"%s%s%s",buf,fname_timestamp,ext);
                }
                else
                {
                    if(strspn(s+1,"0123456789") == strlen(s+1))
                    {
                        filenum = opt_atol(s+1) + 1;
                        *s = 0;
                    }
                    sprintf_s(SAFESTR(roll_name),"%s_%d%s",buf,filenum,ext);
                }
            }
            else
            {
                sprintf_s(SAFESTR(roll_name),"%s_%d%s",buf,filenum,ext);
            }
            
            if(0==strcmp(log_file->roll_name, roll_name)) // Ended up with same name, so go ahead and write
            {
                break;
            }
            
            fclose(log_file->cached_handle);
            log_file->cached_handle = NULL;                
            log_file->roll_name = allocAddString(roll_name); // this will eventually leak enough strings to run out of memory

            i=0;
            validFilename = 1;
        }
    }

    // insert opening line
    if(prepend_startup && log_file->cached_handle)
    {
        fwrite(msg, 1, 16, log_file->cached_handle);
        fwrite(g_dbserverprependmsg, 1, strlen(g_dbserverprependmsg), log_file->cached_handle);
    }

    return log_file;
}

/**
 * Attempt to shutdown the logging thread cleanly, but 
 * eventually give up and shutdown uncleanly if it refuses to do 
 * so. 
 */
int logShutdownAndWait()
{
    U32 timerStart;
    if(!background_writer_running)
        return 0;

    timerStart = timerSecondsSince2000();

    // Wait for the log thread to exit cleanly
    log_background_should_exit = 1;    
    do {
        if(!background_writer_running) {
            return 0;
        }
        Sleep(1);
    } while((timerSecondsSince2000() - timerStart) < LOG_SOFT_TIMEOUT);

    printf("Aborting the log thread (throwing any new log data out)\n");

    // Tell the log thread to get out asap and drop incoming log data on the floor
    log_background_should_abort = 1;
    do {
        if(!background_writer_running) {
            return -1;
        }
        Sleep(1);
    } while((timerSecondsSince2000() - timerStart) < LOG_HARD_TIMEOUT);

    // Just bail as the log thread is probably dead, log files may be corrupted if this occurs
    printf("Log thread failed to exit, bailing anyway (may result in corrupted log files)\n");
    return -2;
}



static void __cdecl s_atexit(void)
{
    logShutdownAndWait();
}

static BOOL WINAPI s_handler_routine(DWORD type)
{
    printf("Shutting down log thread due to interrupt\n");
    logShutdownAndWait();
    return FALSE;
}

/**
 * Build the full log file path and strip off the .log extension.
 */
static void makeAbsLogNameNoExt(char * dst, size_t dst_size, const char *src, char isTextFile)
{
    const char * ext = isTextFile ? ".txt" : ".log";
    size_t len;

    if (fileIsAbsolutePath(src)) {
        strcpy_s(dst, dst_size, src);
    } else {
        strcpy_s(dst, dst_size, log_default_dir);
        strcat_s(dst, dst_size, src);
    }

    len = strlen(dst);
    if(len >= 4 && stricmp(dst + len - 4, ext)==0) {
        dst[len-4] = 0;
    }
}

/** 
 * Flush any pending writes to the operating system to be placed 
 * in the file via an atomic append operation.
 */ 
static int flushLogWriteBuffer(logFileStruct * log_file)
{
    int fwriteStatus;

    assert(log_file->cached_handle);
    assert(log_file->write_buffer);
    if(log_file->write_buffer_bytes) {
        verify(fwriteStatus = fwrite(log_file->write_buffer, log_file->write_buffer_bytes, 1, log_file->cached_handle)==1);
        if (fwriteStatus)
        {
            log_bytes_written += log_file->write_buffer_bytes;
            log_file->write_buffer_bytes = 0;
            log_file->stream_uncommitted = 1;
            return 1;
        }
        else
        {
            // if the write failed, assume the file is closed so we can re-open and try again later
            // Closing the file here can cause a crash if the file does not exist anymore
            log_file->cached_handle = NULL;
            log_file->time_of_last_write = 0;
            printf("Failed to flush log file (%s)\n", log_file->roll_name);
            return 0;
        }
    }

    return 1;
}

/**
 * Write to an already open log file.
 */
static int writeToLogFile(logFileStruct *log_file, const char *msg, size_t bytes)
{
    int fwriteStatus;

    if (!log_file->cached_handle)
    {
        printf("Failed to write to log file (%s): %s\n", log_file->roll_name, msg);
        return 0;
    }

    if(log_file->write_buffer_bytes + bytes > LOG_IO_BUF_SIZE) {
        flushLogWriteBuffer(log_file);
    }

    if(log_file->write_buffer_bytes + bytes > LOG_IO_BUF_SIZE) {
        // this message is too big to cache, just submit it directly
        if (flushLogWriteBuffer(log_file))
        {
            assert(!log_file->write_buffer_bytes);
            verify(fwriteStatus = fwrite(msg, bytes, 1, log_file->cached_handle)==1);
            if (fwriteStatus)
            {
                log_bytes_written += bytes;
                log_file->stream_uncommitted = 1;
            }
            else
            {
                // if the write failed, assume the file is closed so we can re-open and try again later
                // Closing the file here can cause a crash if the file does not exist anymore
                log_file->cached_handle = NULL;
                log_file->time_of_last_write = 0;
                return 0;
            }
        }
    } else {
        memcpy(log_file->write_buffer + log_file->write_buffer_bytes, msg, bytes);
        log_file->write_buffer_bytes += bytes;
    }
    log_file->time_of_last_write = timerSecondsSince2000();
    return 1;
}

/**
 * Determine whether this logfile is timestamped.
 */
static int isLogTimestamped(char *fname_noext)
{
#if 0
    static StashTable timestamp_whitelist;
    char *fname_nodir_noext;
    int isTimestamped = 0;

    if(!timestamp_whitelist)
    {
        StashTable timestamp_whitelist = stashTableCreateWithStringKeys(20,StashDeepCopyKeys);
        // list the extentionless, directoryless file names of logfiles to apply the new timestamp/rollover code to
        stashAddInt(timestamp_whitelist, "db", 1, true);
        stashAddInt(timestamp_whitelist, "bug", 1, true);
    }

    // Figure out if the log is timestamped
    fname_nodir_noext = strrchr(fname_noext,'\\');
    if(!fname_nodir_noext)
        fname_nodir_noext = strrchr(fname_noext,'/');
    stashFindInt(timestamp_whitelist, fname_nodir_noext ? fname_nodir_noext + 1 : fname_noext, &isTimestamped);

    return isTimestamped;
#else
    return logs_are_timestamped;
#endif
}

/**
 * Log the msg to the filename.
 */
static int logToFileName(const char *fname_ptr, const char *msg, size_t bytes, int compress, int isTextFile)
{
    char fname_orig[MAX_PATH];
    int isTimestamped;
    logFileStruct * log_file;

    makeAbsLogNameNoExt(SAFESTR(fname_orig), fname_ptr, isTextFile);
    isTimestamped = isLogTimestamped(fname_orig);
    compress = 0; // TODO: DIRTY HACK! writing compressed logs is broken right now, force uncompressed
    log_file = openAndRotateLogFile(fname_orig, msg, bytes, compress, isTimestamped, isTextFile);
    return writeToLogFile(log_file, msg, bytes);
}



/**
 * Empty the contents of the write buffer and the application level queue
 * streams so that the most of the log data has been pushed to the
 * operating system in case of an unexpected crash.
 */
static void flushCachedLogHandles()
{
    StashElement element;
    StashTableIterator it;

    if(!log_file_structs) return;

    stashGetIterator(log_file_structs, &it);
    while(stashGetNextElement(&it, &element))
    {
        logFileStruct * log_file = stashElementGetPointer(element);
        if(log_file->cached_handle) {
            // empty the write buffer
            if (flushLogWriteBuffer(log_file))
            {

                // check if the stream also needs to be flushed
                if(log_file->stream_uncommitted) {
                    verify(fflush(log_file->cached_handle)==0);
                    log_file->stream_uncommitted = 0;
                }

                // check to see if this handle is older than the expiration time
                if((timerSecondsSince2000() - log_file->time_of_last_write) > LOG_HANDLE_EXPIRATION) {
                    verify(x_fsync(log_file->cached_handle)==0);
                    verify(fclose(log_file->cached_handle)==0);
                    log_file->cached_handle = NULL;
                    log_file->time_of_last_write = 0;
                }
            }
        }
    }
}

/**
 * Force the contents of the log files to be committed to the disk
 * and terminate any gzip streams.
 */
static void commitAndCloseCachedLogHandles()
{
    StashElement element;
    StashTableIterator it;

    stashGetIterator(log_file_structs, &it);
    while(stashGetNextElement(&it, &element))
    {
        logFileStruct * log_file = stashElementGetPointer(element);
        if(log_file->cached_handle) {
            // empty the write buffer
            if (flushLogWriteBuffer(log_file))
            {
                verify(fflush(log_file->cached_handle)==0);
                verify(x_fsync(log_file->cached_handle)==0);
                verify(fclose(log_file->cached_handle)==0);
            }
            log_file->cached_handle = NULL;
            log_file->time_of_last_write = 0;
        }
    }
}




MP_DEFINE(SortLogBlock);
static SortLogBlock* sortlogblock_Create(char *base, size_t baseSize, char *fname)
{
    SortLogBlock *res = NULL;

    // create the pool, arbitrary number
    MP_CREATE(SortLogBlock, 64);
    res = MP_ALLOC( SortLogBlock );
    assert( res );

    res->baseSize = baseSize;
    res->base = malloc(baseSize+1);
    memcpy(res->base, base, baseSize);
    res->base[baseSize] = 0;
    res->fname = strdup(fname);
    res->ref_count = 0;
    res->log_file = NULL;
    res->is_timestamped = 0;
    res->last_log_rotate_sec = 0xFFFF;

    sortMemoryInUse += res->baseSize;
    return res;
}

static void sortlogblock_Release( SortLogBlock *block )
{
    if(block && --block->ref_count <= 0)
    {
        sortMemoryInUse -= block->baseSize;
        assert(sortMemoryInUse >= 0);
        free(block->base);
        free(block->fname);
        MP_FREE(SortLogBlock, block);
    }
}

//----------------------------------------
// 1. create a block for this text
// 2. for each line in the text:
//   1. extract the time from the text
//   2. get the log slot for this time
//   3. add an entry to it for this line
//----------------------------------------
static void addBlock(char *text, size_t textSize, char *fname, char *lineDelim)
{
    SortLogLine        *line;
    SortLogSlot        *slot;
    SortLogBlock    *block = NULL; // calloc(1,sizeof(SortLogBlock));
    char            *s;
    int                len,hours=0,seconds=0,last_seconds=0;
    int lenDelim = 1;
    static int last_hour = -1;
    char fname_abs[MAX_PATH];

    makeAbsLogNameNoExt(SAFESTR(fname_abs), fname, 0);

    lineDelim = lineDelim ? lineDelim : "\n";
    lenDelim = (int)strlen(lineDelim);

    block = sortlogblock_Create(text, textSize, fname_abs);

    block->is_timestamped = isLogTimestamped(fname_abs);

    for(s=block->base;*s;s += len+lenDelim)
    {
        char *pDelim = strstr(s,lineDelim);
        len = pDelim ? pDelim-s : 0;
        if(s[len] != '\n') s[len] = '\n';
        if (len > 13)
        {
            seconds = (opt_atol(s+10)*60 + opt_atol(s+13)) % LOG_SECONDS;
            hours = (opt_atol(s+7));
        }
        if (seconds < 0)
            seconds = last_seconds; // Just in case of a mangled string with negative numbers
        if(last_hour < 0)
            last_hour = hours;
        if(hours > last_hour || (hours==0 && last_hour==23)) // looped around
        {
            last_hour = hours;
        }
        last_seconds = seconds;
        slot = &sort_slots[seconds];
        line = dynArrayAdd(&slot->lines,sizeof(slot->lines[0]),&slot->count,&slot->max,1);
        line->str = s;
        line->block = block;
        line->len = len;

        block->ref_count++;
        if (!len) // the loop should typically not be exiting via this break, but through the *s test at the top
            break;
    }
}



static CRITICAL_SECTION multiple_writers;

void logWaitForQueueToEmpty(void)
{
    const U32 timeout = 30; // wait 30 seconds, otherwise, give up, maybe the log thread is crashed
    U32 timerStart;
    BOOL enteredCritical=FALSE;
    if (!background_writer_running)
        return;
    timerStart = timerSecondsSince2000();
    disable_sort_log = 1;
    while(!(enteredCritical=TryEnterCriticalSection(&multiple_writers)) && (timerSecondsSince2000() - timerStart) < timeout)
        Sleep(1);
    if (enteredCritical) {
        while((msg_queue_end != msg_queue_start || reader_is_processing) && (timerSecondsSince2000() - timerStart) < timeout)
            Sleep(1);
        LeaveCriticalSection(&multiple_writers);
    }
    disable_sort_log = 0;
}

void logMsgQueueStats(size_t * bytes_written, int * queue_count, int * queue_max, size_t * sorted_memory, size_t * sorted_memory_limit)
{
    if(bytes_written) {
        *bytes_written = log_bytes_written;
        log_bytes_written = 0;
    }
    if(queue_count) 
        *queue_count = (msg_queue_end - msg_queue_start) & (msg_queue_size-1);
    if(queue_max) 
        *queue_max = msg_queue_size;
    if(sorted_memory)
        *sorted_memory = sortMemoryInUse;
    if(sorted_memory_limit)
        *sorted_memory_limit = sortMemoryCap;
}

//----------------------------------------
//  for each entry in the sort_slots gather the text for the given entry.
//----------------------------------------
static void writeSortLog(bool flush)
{
    static int        old_seconds=-1;
    static int        max,count;
    int                i,j,sec_count,curr_seconds;    
    bool zipLogfile = 0; // isProductionMode(); TODO: DIRTY HACK! writing compressed logs is broken right now, force uncompressed


    count = 0;
    curr_seconds = timerSecondsSince2000() % LOG_SECONDS;
    if (old_seconds < 0)
        old_seconds = curr_seconds;

    sec_count = ((curr_seconds + LOG_SECONDS - old_seconds) % LOG_SECONDS) - LOG_WINDOW;

    if(flush || disable_sort_log)
        sec_count = LOG_SECONDS;

    if((sec_count < LOG_FLUSH_INTERVAL) && (sortMemoryInUse < sortMemoryCap))
        return;

    for(i=0; (i<sec_count) || (sortMemoryInUse>sortMemoryCap); i++)
    {
        SortLogSlot    *slot;
        int idx = (old_seconds + i) % LOG_SECONDS;
        slot = &sort_slots[idx];
        for(j=0;j<slot->count;j++)
        {
            SortLogLine    *line = &slot->lines[j];

            if(!line->block->log_file || ((line->block->last_log_rotate_sec != idx) && !(idx % LOG_ROTATE_CHECK_INTERVAL)))
            {
                line->block->log_file = openAndRotateLogFile(line->block->fname, line->str, line->len+1, zipLogfile, line->block->is_timestamped, 0);
                line->block->last_log_rotate_sec = idx;
            }

            writeToLogFile(line->block->log_file, line->str, line->len+1);
            sortlogblock_Release(line->block);
        }
        if (slot->count > 10000)
        {
            free(slot->lines);
            slot->lines = 0;
            slot->max = 0;
        }
        slot->count = 0;
    }
    old_seconds = (old_seconds + sec_count) % LOG_SECONDS;
}


void logBackgroundWriterSetSleep(int sleep_ms){
    log_background_sleep_ms = max(0, sleep_ms);
}

static MsgEntry *peekMsg()
{
    MsgEntry    *msg;
    if (msg_queue_start == msg_queue_end)
        return 0;
    msg = &msg_queue[msg_queue_start];
    //     printf("writer %i: 0x%x: ", msg_queue_start, msg);
    //     printf(" filename (0x%x)%s zipped:%i, blocks:%i delim:%s\n", msg->filename, msg->filename, msg->zipped,msg->num_blocks, msg->delim);

    return msg;
}

static MsgEntry *getMsg()
{
    MsgEntry    *msg;

    //     EnterCriticalSection(&multiple_writers);
    if (msg_queue_start == msg_queue_end)
        return 0;
    msg = &msg_queue[msg_queue_start];
    msg_queue_start = (msg_queue_start + msg->num_blocks) & (msg_queue_size-1);
    //printf("read %d (start %d   end %d) TID:%d\n",msg->num_blocks,msg_queue_start,msg_queue_end, GetCurrentThreadId());
    //     LeaveCriticalSection(&multiple_writers);
    return msg;
}

static DWORD WINAPI logBackgroundWriter( LPVOID lpParam )
{
    EXCEPTION_HANDLER_BEGIN
        MsgEntry    *msg;
    static char    *str = 0;
    static int    str_max = 0;
    U32 timer;

    timer = timerAlloc();

    for(;;)
    {
        PERFINFO_AUTO_START("logBackgroundWriter", 1);
        timerStart(timer);
        reader_is_processing = 1;
        while((msg = peekMsg()) && (timerElapsed(timer) < LOG_QUEUE_INTERVAL))
        {
            if (msg->filename)
            {
                char *s = msg->str;
                if (msg->zipped)
                {
                    U32        zip_size = ((U32 *)msg->str)[0];
                    U32        raw_size = ((U32 *)msg->str)[1];

                    dynArrayFit(&str,1,&str_max,raw_size+1);
                    uncompress(str,&raw_size,msg->str+8,zip_size);
                    str[raw_size] = 0;
                    addBlock(str, raw_size, msg->filename, msg->delim);
                    s = str;
                }
                else
                {
                    PERFINFO_AUTO_START("logToFileName", 1);
                    logToFileName(msg->filename, s, strlen(s), msg->zip_file, msg->text_file);
                    PERFINFO_AUTO_STOP();
                }
            }
            getMsg();
        }
        reader_is_processing = 0;

        writeSortLog(false);
        flushCachedLogHandles();
        PERFINFO_AUTO_STOP();

        if(log_background_should_abort || (log_background_should_exit && !peekMsg())) {
            break;
        }

        Sleep(log_background_sleep_ms);
    }

    printf_stderr("Flushing log files to disk\n");
    writeSortLog(true);
    flushCachedLogHandles();
    commitAndCloseCachedLogHandles();
    timerFree(timer);

    EXCEPTION_HANDLER_END

        background_writer_running = 0;
    return 0;
}




void logPutMsgSub(char *msg_str,int len,char *filename,int zip_output,int zipped_input, int text_file, char *msgDelim)
{
    U32            t,free_blocks,num_blocks,contiguous_blocks,filler_blocks=0;
    MsgEntry    *msg;

    // Don't allow zipped output to text file
    assert(!text_file || !zip_output);

    if (!background_writer_running)
    {
        static LazyLock lock = {0};
        lazyLock(&lock);
        if (!background_writer_running)
        {
            if(!msg_queue) {
                msg_queue = calloc(msg_queue_size, sizeof(MsgEntry));
                InitializeCriticalSectionAndSpinCount(&multiple_writers, 4000);
                assert(!atexit(s_atexit));
                assert(SetConsoleCtrlHandler(s_handler_routine, TRUE));
            }
            background_writer_running = 1;
            log_background_should_exit = 0;
            log_background_should_abort = 0;
            _beginthreadex(0,0,logBackgroundWriter,0,0,0);
        }
        lazyUnlock(&lock);
    }
    num_blocks = 1 + (len - 1 - sizeof(msg->str) + sizeof(MsgEntry)) / sizeof(MsgEntry);
    EnterCriticalSection(&multiple_writers);
    for(;;)
    {
        contiguous_blocks = msg_queue_size - msg_queue_end;
        if (num_blocks > contiguous_blocks)
            filler_blocks = contiguous_blocks;
        else
            filler_blocks = 0;
        free_blocks = (msg_queue_start - (msg_queue_end+1)) & (msg_queue_size-1);
        //printf("end %d  start %d = %d free (need %d) TID:%d\n",msg_queue_end,msg_queue_start,free_blocks,num_blocks+filler_blocks, GetCurrentThreadId());
        if (free_blocks >= num_blocks+filler_blocks)
        {
            t = (msg_queue_end + num_blocks + filler_blocks) & (msg_queue_size-1);
            break;
        }
        if (msg_queue_size < max_realloc_msgs || num_blocks+filler_blocks > (U32)msg_queue_size)
        {
            // wait for queue to empty, and other thread processing to complete
            while(msg_queue_end != msg_queue_start || reader_is_processing)
                Sleep(1);
            msg_queue_size *= 2;
            //free(msg_queue);
            //msg_queue = malloc(sizeof(msg_queue[0]) * msg_queue_size);
            msg_queue = realloc(msg_queue,sizeof(msg_queue[0]) * msg_queue_size);
        }
        Sleep(1);
    }
    msg = &msg_queue[msg_queue_end];
    if (filler_blocks)
    {
        memset(msg,0,sizeof(*msg));
        msg->num_blocks = filler_blocks;
        msg = &msg_queue[(msg_queue_end + filler_blocks) & (msg_queue_size-1)];
    }
    msg->zipped = zipped_input;
    msg->zip_file = zip_output;
    msg->text_file = text_file;
    msg->filename = (char*)allocAddString(filename);
    msg->num_blocks = num_blocks;
    msg->delim = msgDelim;
    memcpy(msg->str,msg_str,len);
    msg_queue_end = t;

    //printf("msg %i-%i: 0x%x: filename (0x%x)%s zipped:%i, blocks:%i delim:%s\n", msg_queue_end, msg_queue_end+num_blocks, msg, msg->filename, msg->filename, msg->zipped,msg->num_blocks, msg->delim);
    //printf("wrote %d/%d (start %d   end %d) TID:%d\n",filler_blocks,num_blocks,msg_queue_start,msg_queue_end,GetCurrentThreadId());
    LeaveCriticalSection(&multiple_writers);
}

void logPutMsg(char *msg_str,char *filename)
{
    logPutMsgSub(msg_str,(int)strlen(msg_str)+1,filename,0,0,0, NULL);
}

void logPutMsgTxt(char *msg_str,char *filename)
{
    logPutMsgSub(msg_str,(int)strlen(msg_str)+1,filename,0,0,1, NULL);
}

