#include "logcomm.h"
#include "cmdparse/cmdserver.h"
#include <utilitieslib/network/net_masterlist.h>
#include <utilitieslib/components/StashTable.h>
#include <utilitieslib/network/netio.h>
#include "comm_backend.h"
#include <utilitieslib/components/EString.h>
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/strings_opt.h>
#include "dbcomm.h"
#include "entity/entity.h"
#include "entity/character_base.h"
#include "dbghelper.h"
#include <utilitieslib/network/netcomp.h>
#include "shardcomm.h"
#include "shardcommtest.h"
#include <utilitieslib/utils/log.h>
#include "entity/Supergroup.h"
#include <utilitieslib/version/AppVersion.h>
#include "entity/incarnate.h"

#if SERVER
#include "dbnamecache.h"
#include "entity/character_level.h"
#endif

static char *db_log_msgs = NULL;
static char *db_bug_log = NULL;
static char *db_log_file = NULL;
static StashTable s_logmsgs_from_fname = 0; // for log_printf

typedef struct NamedLogMsg
{
    char *msgs;
    U32 last_sent_time;
} NamedLogMsg;
static NamedLogMsg s_logs_by_type[LOG_COUNT] = {0};

extern NetLink    log_comm_link;

static char* s_getInstNameForLogging(char *resMapname, int len)
{
    char *s;

    strncpyt(resMapname,getFileName(db_state.map_name), len-10);
    s = strrchr(resMapname,'.');
    if (s)
        *s = 0;

    strcatf_s(resMapname, len, "_%d:%s:%s", db_state.instance_id, makeIpStr(getHostLocalIp()), db_state.server_name);
    
    return resMapname;
}

void dbLog(char const *cmdname,Entity *e,char const *fmt, ...)
{
    char * str = estrTemp();
    va_list ap;

    if(e) // pre-pend entity name and team id
    {
        estrConcatf(&str, "%s %i %s", e->name?e->name:"(no name)", e->teamup_id, cmdname );
    }
    else
        estrConcatf(&str, "%s ", cmdname );
    
    va_start(ap, fmt);
    estrConcatfv(&str, fmt, ap);
    va_end(ap);

    log_msg( LOG_DEPRECATED, LOG_LEVEL_DEPRECATED, 0, str );

    estrDestroy(&str);
}


void dbLogBug(char const *fmt, ...)
{
    va_list    ap;
    NetLink        *link = &log_comm_link;

    PERFINFO_AUTO_START("dbLogBug",1);

    va_start(ap, fmt);
    estrConcatfv(&db_bug_log, (char*)fmt, ap);
    va_end(ap);

    if (link->connected)
    {
        Packet *pak;
        // send the bug logs
        pak = pktCreateEx(link,LOGCLIENT_LOGBUG);
        pktSendString(pak,db_bug_log);
        pktSend(&pak,link);

        lnkBatchSend(link);
        lnkBatchReceive(link);
    }
    estrClear(&db_bug_log);

    PERFINFO_AUTO_STOP();
    
}

static U32    send_when_greater_than_size=200000; // Send at 200k (should compress to ~20K)

static void sendNamedLogMsg( NamedLogMsg * log, const char * name, int force, int resend_time )
{
    U32    curr_time = dbSecondsSince2000();
    NetLink    *link = &log_comm_link;

    if( log && (force || curr_time > log->last_sent_time + resend_time || estrLength(&log->msgs) >= send_when_greater_than_size) )
    {
        log->last_sent_time = curr_time;
        if( estrLength( &log->msgs ) )
        {
            if (!link->connected && link == &log_comm_link)
            {
                netConnect(link,db_state.log_server,DEFAULT_LOG_PORT,NLT_TCP,1,NULL);
                shardCommReconnect();
            }
            if (link->connected)
            {
                Packet    *pak;
                
                // send the db log messages
                pak = pktCreateEx(link, LOGCLIENT_LOGPRINTF);
                pktSendString(pak, name);
                pktSendZipped(pak,estrLength(&log->msgs),log->msgs);
                pktSend(&pak,link);
                estrClear(&log->msgs);

                lnkBatchSend(link);
                lnkBatchReceive(link);
            }
        }
    }
}

void logFlush(int force)
{
// ab: before you change this, this value is very tightly coupled with
// UtilitiesLib/utils/error.c:sort_slots and LOG_SECONDS defined there
#define SECS_LOG_RESEND (30)

    // new logging
    {
        int i;
        for( i = 0; i < LOG_COUNT; i++ )
        {
            sendNamedLogMsg( &s_logs_by_type[i], logGetTypeName(i), force, SECS_LOG_RESEND);
        }
    }

    // old logging
    {
        static U32    last_sent_time;
        U32     curr_time = dbSecondsSince2000();
        NetLink        *link = &log_comm_link;

         if ((force || curr_time > last_sent_time + SECS_LOG_RESEND || estrLength(&db_log_msgs) >= send_when_greater_than_size))
        {
            PERFINFO_AUTO_START("dbFlushLogs",1);
            last_sent_time = curr_time;            
            if ( estrLength(&db_log_msgs) || gChatServerTestClients)
            {
                if (!link->connected && link == &log_comm_link)
                {
                    netConnect(link,db_state.log_server,DEFAULT_LOG_PORT,NLT_TCP,1,NULL);
                    shardCommReconnect();
                }
                if (link->connected)
                {
                    Packet    *pak;

                    // send the db log messages
                    pak = pktCreateEx(link,LOGCLIENT_LOG);
                    pktSendZipped(pak,estrLength(&db_log_msgs),db_log_msgs);
                    pktSend(&pak,link);
                    estrClear(&db_log_msgs);

                    lnkBatchSend(link);
                    lnkBatchReceive(link);
                }
            }
            PERFINFO_AUTO_STOP();
        }

        // old logs
        if (s_logmsgs_from_fname)
        {
            StashTableIterator is;
            StashElement elem;
            for(stashGetIterator( s_logmsgs_from_fname, &is);stashGetNextElement(&is,&elem);)
            {
                NamedLogMsg *log = stashElementGetPointer( elem );
                const char *fname = stashElementGetStringKey(elem);
                sendNamedLogMsg( log, fname, force, SECS_LOG_RESEND );
            }
        }
    }
}

void logSendDisconnect(F32 timeout)
{
    logFlush(1);
    netSendDisconnect(&log_comm_link, timeout);
}

void logDisconnectFlush()
{
    // Flush db_log_msgs to disk (we've already been disconnected from the DbServer
    if (estrLength(&db_log_msgs))
    {
        static char logname[256];
        sprintf(logname, "dbLogUnflushed_pid_%d.log", getpid());
        filelog_printf(logname, "Unflushed dbLog messages:\n%s", db_log_msgs);
    }

    if (estrLength(&db_bug_log))
    {
        filelog_printf("bug", "%s", db_bug_log);
    }

    if (s_logmsgs_from_fname)
    {
        StashTableIterator is;
        StashElement elem;
        for(stashGetIterator( s_logmsgs_from_fname, &is);stashGetNextElement(&is,&elem);)
        {
            NamedLogMsg *log = stashElementGetPointer( elem );
            const char *fname = stashElementGetStringKey(elem);

            if( log && estrLength(&log->msgs) )
            {
                static char logname[256];
                sprintf(logname, "%sLogUnflushed_pid_%d.log", fname, getpid());
                filelog_printf(logname, "%s", log->msgs);
            }
        }
    }
}

int Logserver_NetPacketCallback(Packet *pak, int cmd, NetLink *link)
{
    if( link == &log_comm_link )
    {
        printf("received unexpected message from the logserver!?!\n");
    }

    return 0; // nothing handled
}

static void logserverLogCallback( LogType type, char const* msg );
int logNetInit()
{
    getAutoDbName(db_state.log_server);
    if (!netConnect(&log_comm_link,db_state.log_server,DEFAULT_LOG_PORT,NLT_TCP,NO_TIMEOUT,NULL))
        return 0;
    
    // the callback can safely be NULL, but I foresee needing this functionality some day
    //NMAddLink(&log_comm_link, Logserver_NetPacketCallback);

    // no need to copy strings, assume they're all static constants.
    if(!s_logmsgs_from_fname)
        s_logmsgs_from_fname = stashTableCreateWithStringKeys(128, StashDefault);

    // set the utils logger
    setSendMsgToLogServerFunc(logserverLogCallback);
    return 1;
}

void logCommMonitor()
{
    shardCommMonitor();
}

// *********************************************************************************
//  log_printf section
// *********************************************************************************

MP_DEFINE(NamedLogMsg);
static NamedLogMsg* namedlogmsg_Create(void)
{
    NamedLogMsg *res = NULL;

    // create the pool, arbitrary number
    MP_CREATE(NamedLogMsg, 64);
    res = MP_ALLOC( NamedLogMsg );
    if( verify( res ))
    {
    }

    // --------------------
    // finally, return

    return res;
}

static void namedlogmsg_Destroy( NamedLogMsg *hItem )
{
    if(hItem)
    {
        estrDestroy(&hItem->msgs);
        MP_FREE(NamedLogMsg, hItem);
    }
}

//----------------------------------------------------------------------------------------------
// new logging
//----------------------------------------------------------------------------------------------

// this is very similar to log_f, but prepends entity name and info
void log_ent( Entity * e, LogType type, LogLevel level, int flags, int sg, char * fmt, ...  )
{
    char * str = estrTemp();
    va_list ap;
    int result = 0;

    if(e) // pre-pend entity name and team id
    {
        if( sg )
            estrConcatf(&str, "\"%s:%s\" SG:%s %i ", e->name?e->name:"(no name)", e->auth_name?e->auth_name:"(no authname)", e->supergroup->name, e->supergroup_id );
        else
            estrConcatf(&str, "\"%s:%s\" %i ", e->name?e->name:"(no name)", e->auth_name?e->auth_name:"(no authname)", e->teamup_id );
    }

    va_start(ap, fmt);
    estrConcatfv(&str, fmt, ap);
    va_end(ap);

#if SERVER

    //append entity level (based on experience points), alignment, archetype, and incarnate status
    //to end of the log message so that it doesn't break how CSR parses these logs for their SQL database
    if(e && e->pl && e->pchar && e->pchar->pclass)
    {
        estrConcatf(&str, " ExpLevel:%d, AlignmentNum:%d, Archetype:%s, Incarnate:%d", 
                            character_GetExperienceLevelExtern(e->pchar),
                            getEntAlignment(e), //requires e->pl, otherwise function will assert
                            e->pchar->pclass->pchName,
                            IsEntityIncarnate(e));
    }
#endif
    if( str[estrLength(&str)-1] != '\n' )
        estrConcatChar(&str, '\n');

    log_msg( type, level, flags, str );

    estrDestroy(&str);
}

// this is very similar to log_f, but prepends entity name
void log_dbid( int dbid, LogType type, LogLevel level, int flags, char * fmt, ...  )
{
    char * str = estrTemp();
    va_list ap;
    int result = 0;

#if STATSERVER
    estrConcatf(&str, "%i", dbid);
#elif SERVER
    estrConcatCharString(&str, dbPlayerNameFromId(dbid));
#endif

    va_start(ap, fmt);
    estrConcatfv(&str, fmt, ap);
    va_end(ap);

    if( str[estrLength(&str)-1] != '\n' )
        estrConcatChar(&str, '\n');

    log_msg( type, level, flags, str );

    estrDestroy(&str);
}

// this is very similar to log_f, but prepends entity name and info
void log_sg( char * name, int id, LogLevel level, int flags, char * fmt, ...  )
{
    char * str = estrTemp();
    va_list ap;
    int result = 0;

    estrConcatf(&str, "SG:%s %i ",name,id);

    va_start(ap, fmt);
    estrConcatfv(&str, fmt, ap);
    va_end(ap);

    if( str[estrLength(&str)-1] != '\n' )
        estrConcatChar(&str, '\n');

    log_msg( LOG_SUPERGROUP, level, flags, str );

    estrDestroy(&str);
}


// DO NOT CALL DIRECTLY (this is invoked by log_f)
static void logserverLogCallback( LogType type, char const* msg )
{
    int len;
    NamedLogMsg *log = &s_logs_by_type[type];
    char datestr[200],mapname[356];

    // construct the message
    timerMakeLogDateStringFromSecondsSince2000(datestr,dbSecondsSince2000());
    s_getInstNameForLogging(mapname, ARRAY_SIZE( mapname )); 
    estrConcatMinimumWidth(&log->msgs, datestr, 15);
    estrConcatChar(&log->msgs, ' ');
    estrConcatMinimumWidth(&log->msgs, mapname, 18);  // FIXME? not sure I want to include this on every line
    estrConcatChar(&log->msgs, ' ');
    estrConcatCharString(&log->msgs, msg);

    // ----------
    // terminate the message
    len = estrLength(&log->msgs);
    if(log->msgs[len-1] == '\n')
    {
        log->msgs[len-1] = ' ';
    }

    //add buildnumber to end of log message before adding '\t\t\n' terminator
    //I was looking for another instance of a mapserver file that called the function to obtain the build number.
    //I found that svr_bug.c does so with getExecutablePatchVersion("CoH Server"), so I used that here.
    estrConcatf(&log->msgs, "BuildNumber: %s", getExecutablePatchVersion("CoH Server"));
    
    estrConcatMinimumWidth(&log->msgs, "\t\t\n", 3); // message terminator ( this is weird )
    // ----------
    // finally
}
