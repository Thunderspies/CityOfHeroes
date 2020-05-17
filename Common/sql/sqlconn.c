#include <utilitieslib/stdtypes.h>
#include "sqlconn.h"
#include "sqlinclude.h"
#include <utilitieslib/utils/SuperAssert.h>
#include <utilitieslib/utils/stackdump.h>
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/stringutil.h>
#include <utilitieslib/components/estring.h>
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/utils/log.h>
#include <utilitieslib/utils/endian.h>

/// Enable to stress test the retry code
//#define STRESS_RETRY_CODE

/// Retry for up to 2 minutes
#define MAX_RETRY_SECONDS (2*60)

STATIC_ASSERT(sizeof(size_t) == sizeof(SQLULEN));
STATIC_ASSERT(sizeof(ssize_t) == sizeof(SQLLEN));

typedef __declspec(align(EXPECTED_WORST_CACHE_LINE_SIZE)) struct {
    volatile unsigned long worst_usec;
    volatile unsigned long running_usec;
    volatile unsigned long idle_usec;
    volatile unsigned long total_count;
} sqlTimes;

struct sqlGlobals {
    U32 connection_count;
    sqlTimes times[SQLCONN_MAX];
    int timers[SQLCONN_MAX];
    HDBC connections[SQLCONN_MAX];

    HENV env;

    char database_string[256];
    char login_string[SHORT_SQL_STRING];

#ifdef FULLDEBUG
    LONG stmt_allocs;
#endif
} sql;

static const char* sqlTypeName(SQLSMALLINT type)
{
    THREADSAFE_STATIC char *buf = NULL;
    THREADSAFE_STATIC_MARK(buf);

    switch (type) {
        #define SQL_TYPE_NAME(_type) case _type: return #_type;

        // Standard types
        SQL_TYPE_NAME(SQL_UNKNOWN_TYPE);
        SQL_TYPE_NAME(SQL_CHAR);
        SQL_TYPE_NAME(SQL_NUMERIC);
        SQL_TYPE_NAME(SQL_DECIMAL);
        SQL_TYPE_NAME(SQL_INTEGER);
        SQL_TYPE_NAME(SQL_SMALLINT);
        SQL_TYPE_NAME(SQL_FLOAT);
        SQL_TYPE_NAME(SQL_REAL);
        SQL_TYPE_NAME(SQL_DOUBLE);
        SQL_TYPE_NAME(SQL_DATETIME);
        SQL_TYPE_NAME(SQL_TIME);
        SQL_TYPE_NAME(SQL_TIMESTAMP);
        SQL_TYPE_NAME(SQL_VARCHAR);

        // Shortcuts
        SQL_TYPE_NAME(SQL_TYPE_DATE);
        SQL_TYPE_NAME(SQL_TYPE_TIME);
        SQL_TYPE_NAME(SQL_TYPE_TIMESTAMP);
        SQL_TYPE_NAME(SQL_C_DEFAULT);

        // Extended types
        SQL_TYPE_NAME(SQL_LONGVARCHAR);
        SQL_TYPE_NAME(SQL_BINARY);
        SQL_TYPE_NAME(SQL_VARBINARY);
        SQL_TYPE_NAME(SQL_LONGVARBINARY);
        SQL_TYPE_NAME(SQL_BIGINT);
        SQL_TYPE_NAME(SQL_TINYINT);
        SQL_TYPE_NAME(SQL_BIT);
        SQL_TYPE_NAME(SQL_WCHAR);
        SQL_TYPE_NAME(SQL_WVARCHAR);
        SQL_TYPE_NAME(SQL_WLONGVARCHAR);
        SQL_TYPE_NAME(SQL_GUID);

        // Native Client types
        SQL_TYPE_NAME(SQL_SS_TABLE);
    }

    estrPrintf(&buf, "<type %d unsupported by sqlTypeName>", type);
    return buf;
}

static char* sqlTypeDataString(SQLSMALLINT type, SQLSMALLINT ctype, SQLPOINTER data, SQLLEN length)
{
    THREADSAFE_STATIC char *buf = NULL;
    THREADSAFE_STATIC_MARK(buf);

    if (!data || length == SQL_NULL_DATA)
        return "<null>";

    estrClear(&buf);

    switch (ctype) {
        xcase SQL_TINYINT:
        case SQL_BIT:
            estrPrintf(&buf, "%hd", *(char*)data);
        xcase SQL_SMALLINT:
            estrPrintf(&buf, "%hd", *(short*)data);
        xcase SQL_INTEGER:
            estrPrintf(&buf, "%d", *(long*)data);
        xcase SQL_REAL:
            estrPrintf(&buf, "%f", *(float*)data);
        xcase SQL_DOUBLE:
            estrPrintf(&buf, "%f", *(double*)data);
        xcase SQL_CHAR:
        case SQL_VARCHAR:
        {
            if (length != SQL_NTS)
                estrConcatFixedWidth(&buf, data, length);
            else
                estrConcatCharString(&buf, data);
        }
        xcase SQL_WCHAR:
        case SQL_WVARCHAR:
            estrConvertToNarrow(&buf, (wchar_t*)data);
        xcase SQL_DATETIME:
        {
            SQL_TIMESTAMP_STRUCT *t = (void*)(data);
            estrPrintf(&buf, "%04d-%02d-%02d %02d:%02d:%02d.%09d", t->year, t->month, t->day, t->hour, t->minute, t->second, t->fraction);
        }
        xcase SQL_GUID:
        {
            U16 *s = (U16*)(data);
            estrPrintf(&buf, "%04hx%04hx-%04hx-%04hx-%04hx-%04hx%04hx%04hx", s[1], s[0], s[2], s[3], endianSwapU16(s[4]), endianSwapU16(s[5]), endianSwapU16(s[7]), endianSwapU16(s[6]));
        }
        xcase SQL_BINARY:
        case SQL_VARBINARY:
        {
            int i;
            U8 *u = (U8*)data;
            if (!u)
                break;
            if (ctype == SQL_C_CHAR) {
                for(i=0; i<MIN(32, length); i++) {
                    if (isprint(u[i]))
                        estrConcatChar(&buf, u[i]);
                    else
                        estrConcatf(&buf, "0x%02x", u[i]);
                }
            } else {
                estrConcatStaticCharArray(&buf, "0x");
                for(i=0; i<MIN(32, length); i++) {
                    estrConcatf(&buf, "%02x", u[i]);
                }
            }
            if (i<length)
                estrConcatStaticCharArray(&buf, "...?");
        }

        xdefault:
            estrPrintf(&buf, "<type %d unsupported by sqlTypeDataString>", type);
    }

    return buf;
}

void sqlConnStmtPrintBinds(HSTMT stmt)
{
    SQLHDESC apd = NULL;
    SQLHDESC ipd = NULL;
    SQLINTEGER num_binds = 0;
    size_t array_size = 0;
    int i;

    if(!SQL_SUCCEEDED(SQLGetStmtAttr(stmt, SQL_ATTR_APP_PARAM_DESC, &apd, 0, NULL))) {
        LOG(LOG_ERROR, LOG_LEVEL_IMPORTANT, LOG_CONSOLE_ALWAYS, "SQLGetStmtAttr failed to retrieve the APD");
        return;
    }

    if(!SQL_SUCCEEDED(SQLGetStmtAttr(stmt, SQL_ATTR_IMP_PARAM_DESC, &ipd, 0, NULL))) {
        LOG(LOG_ERROR, LOG_LEVEL_IMPORTANT, LOG_CONSOLE_ALWAYS, "SQLGetStmtAttr failed to retrieve the IPD");
        return;
    }

    if(!SQL_SUCCEEDED(SQLGetDescField(apd, 0, SQL_DESC_COUNT, &num_binds, 0, NULL))) {
        LOG(LOG_ERROR, LOG_LEVEL_IMPORTANT, LOG_CONSOLE_ALWAYS, "SQLGetDescField failed to retrieve the APD descriptor count");
        return;
    }

    if(!SQL_SUCCEEDED(SQLGetDescField(apd, 0, SQL_DESC_ARRAY_SIZE, &array_size, 0, NULL))) {
        LOG(LOG_ERROR, LOG_LEVEL_IMPORTANT, LOG_CONSOLE_ALWAYS, "SQLGetDescField failed to retrieve the APD array size");
        return;
    }

    if (array_size > 1) {
        LOG(LOG_ERROR, LOG_LEVEL_IMPORTANT, LOG_CONSOLE_ALWAYS, "BIND showing 1 of %d entires in the array", array_size);
    }

    for (i=1; i<=num_binds; i++) {
        SQLINTEGER type = 0;
        SQLINTEGER ctype = 0;
        SQLPOINTER data = NULL;
        SQLPOINTER indicator = 0;

        if(!SQL_SUCCEEDED(SQLGetDescField(apd, i, SQL_DESC_TYPE, &type, 0, NULL))) {
            LOG(LOG_ERROR, LOG_LEVEL_IMPORTANT, LOG_CONSOLE_ALWAYS, "SQLGetDescField failed to retrieve APD %d's type", i);
            return;
        }

        if(!SQL_SUCCEEDED(SQLGetDescField(ipd, i, SQL_DESC_TYPE, &ctype, 0, NULL))) {
            LOG(LOG_ERROR, LOG_LEVEL_IMPORTANT, LOG_CONSOLE_ALWAYS, "SQLGetDescField failed to retrieve IPD %d's type", i);
            return;
        }

        if(!SQL_SUCCEEDED(SQLGetDescField(apd, i, SQL_DESC_DATA_PTR, &data, 0, NULL))) {
            LOG(LOG_ERROR, LOG_LEVEL_IMPORTANT, LOG_CONSOLE_ALWAYS, "SQLGetDescField failed to retrieve APD %d's address", i);
            return;
        }

        if(!SQL_SUCCEEDED(SQLGetDescField(apd, i, SQL_DESC_INDICATOR_PTR, &indicator, 0, NULL))) {
            LOG(LOG_ERROR, LOG_LEVEL_IMPORTANT, LOG_CONSOLE_ALWAYS, "SQLGetDescField failed to retrieve APD %d's indicator", i);
            return;
        }

        LOG(LOG_ERROR, LOG_LEVEL_IMPORTANT, LOG_CONSOLE_ALWAYS, "BIND #%d %s @ %p: %s", i, sqlTypeName(type), data, sqlTypeDataString(type, ctype, data, indicator ? *(SQLLEN*)indicator : SQL_NTS));
    }    
}

static bool sqlConnPrintError(SQLSMALLINT handle_type, SQLHANDLE handle, const char *original_command, char *sql_state_out)
{
    SQLSMALLINT rec_num = 1;
    SQLINTEGER error;
    char msg[SQL_MAX_MESSAGE_LENGTH];
    char sql_state[SQL_SQLSTATE_SIZE + 1];
    SQLSMALLINT msg_len;
    SQLRETURN ret;

    // Get the status records.
    while ((ret = SQLGetDiagRecA(handle_type, handle, rec_num, sql_state, &error, msg, sizeof(msg), &msg_len)) != SQL_NO_DATA)
    {
        if (rec_num == 1) {
            if (sql_state_out)
                memcpy(sql_state_out, sql_state, sizeof(sql_state));
            else
                sdDumpStack();

            if (handle_type == SQL_HANDLE_STMT)
                sqlConnStmtPrintBinds(handle);                

            LOG(LOG_ERROR, LOG_LEVEL_ALERT, LOG_CONSOLE_ALWAYS, "COMMAND: %s\n", original_command);
        }

        assertmsgf(ret != SQL_INVALID_HANDLE, "SQLGetDiagRec got an invalid handle 0x%x of type %d", handle, handle_type);
        assertmsg(SQL_SUCCEEDED(ret), "SQLGetDiagRec failed");

        LOG(LOG_ERROR, LOG_LEVEL_ALERT, LOG_CONSOLE_ALWAYS, "SQLERROR: %d %s %s\n", error, sql_state, msg);

        assertmsg(strcmp(sql_state, "08007")!=0, "Connection failure during transaction");
        assertmsg(strcmp(sql_state, "08S01")!=0, "Communication link failure");
        assertmsg(strcmp(sql_state, "HYT01")!=0, "Connection timeout expired");

         rec_num++;
    }

     return rec_num != 1;
}

static bool sqlConnPrintErrorf(SQLSMALLINT handle_type, SQLHANDLE handle, char *fmt, ...)
{
    char buf[4096];
    va_list va;
    bool ret;

    va_start(va, fmt);
    vsnprintf(buf, sizeof(buf), fmt, va);
    ret = sqlConnPrintError(handle_type, handle, buf, NULL);
    va_end(va);

    return ret;
}

bool sqlConnStmtPrintErrorAndGetState(HSTMT stmt, const char *original_command, char *sqlstate)
{
    return sqlConnPrintError(SQL_HANDLE_STMT, stmt, original_command, sqlstate);
}

bool sqlConnStmtPrintError(HSTMT stmt, const char *original_command)
{
    return sqlConnPrintError(SQL_HANDLE_STMT, stmt, original_command, NULL);
}

bool sqlConnStmtPrintErrorf(HSTMT stmt, char *fmt, ...)
{
    char buf[4096];
    va_list va;
    bool ret;

    va_start(va, fmt);
    vsnprintf(buf, sizeof(buf), fmt, va);
    ret = sqlConnPrintError(SQL_HANDLE_STMT, stmt, buf, NULL);
    va_end(va);

    return ret;
}

bool sqlConnInit(U32 connection_count)
{
    unsigned i;
    sql.connection_count = connection_count;

    if(!SQL_SUCCEEDED(SQLAllocHandle(SQL_HANDLE_ENV, NULL, &sql.env)))
        return false;

    // Require ODBC 3
    if(!SQL_SUCCEEDED(SQLSetEnvAttr(sql.env, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)(uintptr_t)SQL_OV_ODBC3, SQL_IS_UINTEGER)))
       return false;

    // Optionally support ODBC 3.8
    #define SQL_OV_ODBC3_80                     380UL
    //SQLSetEnvAttr(sql.env, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3_80, SQL_IS_UINTEGER);

    // Preallocate DBC handles
    assert(connection_count <= SQLCONN_MAX);
    for(i=0; i<connection_count; i++)
    {
        if(!(SQL_SUCCEEDED(SQLAllocHandle(SQL_HANDLE_DBC, sql.env, &sql.connections[i])))) {
            return false;
        }

        sql.timers[i] = timerAlloc();
    }

    return true;
}

void sqlConnShutdown(void)
{
    unsigned i;

    if (!sql.env)
        return;

    for (i=0; i<sql.connection_count; i++) {
        sqlConnEndTransaction(true, true, i);

        if(!SQL_SUCCEEDED(SQLDisconnect(sql.connections[i])))
            sqlConnPrintError(SQL_HANDLE_DBC, sql.connections[i], "SQLFreeHandle", NULL);

        if (!(SQL_SUCCEEDED(SQLFreeHandle(SQL_HANDLE_DBC, sql.connections[i]))))
            sqlConnPrintError(SQL_HANDLE_DBC, sql.connections[i], "SQLFreeHandle", NULL);

        timerFree(sql.timers[i]);
    }

    if (!SQL_SUCCEEDED(SQLFreeHandle(SQL_HANDLE_ENV, sql.env)))
        sqlConnPrintError(SQL_HANDLE_ENV, sql.env, "SQLFreeHandle", NULL);

    sql.env = 0;
}

void sqlConnSetLogin(char *s)
{
    strcpy(sql.login_string, s);
}

HSTMT sqlConnStmtAlloc(SqlConn conn)
{
    HSTMT stmt;

    assert(SQL_SUCCEEDED(SQLAllocHandle(SQL_HANDLE_STMT, sql.connections[conn], &stmt)));

#ifdef FULLDEBUG
    {
        LONG allocs = InterlockedIncrement(&sql.stmt_allocs);
        assert(allocs >=0 && allocs < 0x100000);
    }
#endif

    return stmt;
}

void sqlConnStmtFree(HSTMT stmt)
{
    // If this asserts we could get data loss on live because MS SQL will not
    // necessarily execute the entire command.
    onlydevassert(SQLMoreResults(stmt) == SQL_NO_DATA);

    assert(SQL_SUCCEEDED(SQLFreeHandle(SQL_HANDLE_STMT, stmt)));

#ifdef FULLDEBUG
    {
        LONG allocs = InterlockedDecrement(&sql.stmt_allocs);
        assert(allocs >=0 && allocs < 0x1000000);
    }
#endif
}

void sqlConnGetTimes(int * count, long * avg_usec, long * worst_usec, float * fore_idle_ratio, float * back_idle_ratio)
{
    unsigned i;
    sqlTimes aggregate = {0,};
    long back_total_usec = 0;
    long back_idle_usec = 0;

    for(i=0; i<sql.connection_count; i++) {
        sqlTimes current = {0,};
        long total_usec;

        current.worst_usec = InterlockedExchange(&sql.times[i].worst_usec, 0LL);
        current.running_usec = InterlockedExchange(&sql.times[i].running_usec, 0LL);
        current.idle_usec = InterlockedExchange(&sql.times[i].idle_usec, 0LL);
        current.total_count = InterlockedExchange(&sql.times[i].total_count, 0LL);

        // Calculate both foreground and background idle ratios
        total_usec = current.running_usec + current.idle_usec;
        if(i == 0) {
            STATIC_INFUNC_ASSERT(SQLCONN_FOREGROUND == 0);
            if(fore_idle_ratio) *fore_idle_ratio = total_usec ? (float)current.idle_usec / (float)total_usec : 1;
        } else {
            back_total_usec += total_usec;
            back_idle_usec += current.idle_usec;
        }

        aggregate.worst_usec += current.worst_usec;
        aggregate.running_usec += current.running_usec;
        aggregate.idle_usec += current.idle_usec;
        aggregate.total_count += current.total_count;
    }

    if(count) *count = aggregate.total_count;
    if(worst_usec) *worst_usec = aggregate.worst_usec;
    if(avg_usec) *avg_usec = aggregate.total_count ? aggregate.running_usec / aggregate.total_count : 0;
    if(back_idle_ratio) *back_idle_ratio = back_total_usec ? (float)back_idle_usec / (float)back_total_usec : 1;
}

static void sqlConnAddTimeInternal(SqlConn conn, long long dt_usec, long long idle_usec)
{
    // Keep track of the longest time seen
    register ULONG cur_worst = sql.times[conn].worst_usec;
    while((ULONG)dt_usec > cur_worst) {
        InterlockedCompareExchange(&sql.times[conn].worst_usec, dt_usec, cur_worst);
        cur_worst = sql.times[conn].worst_usec;
    }

    // Keep track of the total time, idle time and number issued
    InterlockedExchangeAdd(&sql.times[conn].running_usec, (ULONG)dt_usec);
    InterlockedExchangeAdd(&sql.times[conn].idle_usec, (ULONG)idle_usec);
    InterlockedIncrement(&sql.times[conn].total_count);
}

static void sqlConnTimerAddTime(SqlConn conn, float since_last)
{
    float   dt;

    // clamp extremely idle periods to prevent inaccuracy when the sql thread
    // is sleeping and not issuing any commands
    if(since_last > 10.0f)
        since_last = 10.0f;

    dt = timerElapsedAndStart(sql.timers[conn]);
    sqlConnAddTimeInternal(conn, dt * 1000000LL, since_last * 1000000LL);
}

#ifdef FULLDEBUG
static void checkQuery(char const* str, size_t str_len)
{
    if (str_len == SQL_NTS)
        str_len = strlen(str);

    // Make sure the command is terminated
    assert(str[str_len-1] == ';' || (str[0] == '{' && str[str_len-1] == '}'));
}
#endif

static SQLRETURN _sqlConnStmtExecDirectUtf8(HSTMT stmt, char const* str, int str_len, SqlConn conn)
{
    SQLRETURN ret;
    float since_last;

    static U16 *wstrs[SQLCONN_MAX];
    static int wstr_maxs[SQLCONN_MAX];

#ifdef FULLDEBUG
    checkQuery(str, str_len);
#endif

    // Resize static buffer per connection to fit new string
    if (str_len == SQL_NTS)
        str_len = (int)strlen(str);
    dynArrayFit(&wstrs[conn], 2, &wstr_maxs[conn], str_len+1);

    // Do a UTF8 to UTF16 conversion
    str_len = MultiByteToWideChar(CP_UTF8, 0, str, str_len, wstrs[conn], str_len+1);

    since_last = timerElapsedAndStart(sql.timers[conn]);
    ret = SQLExecDirectW(stmt, wstrs[conn], str_len);
    sqlConnTimerAddTime(conn, since_last);

    return ret;
}

/** 
* @note You should always call @ref sqlExecDirectTimedUtf8 on if
* the query supplies user created data. 
*/
static SQLRETURN _sqlConnStmtExecDirectAscii(HSTMT stmt, char const* str, int str_len, SqlConn conn)
{
    SQLRETURN ret;
    float since_last;

#ifdef FULLDEBUG
    checkQuery(str, str_len);
#endif

    since_last = timerElapsedAndStart(sql.timers[conn]);
    ret = SQLExecDirectA(stmt, (char*)str, str_len);
    sqlConnTimerAddTime(conn, since_last);

    return ret;
}

INLINEDBG int sqlConnStmtExecDirect(HSTMT stmt, char const* str, int str_len, SqlConn conn, bool utf8)
{
    SQLRETURN ret;
    U32 firstFailTime = 0;

    for (;;) {
        U32 now;
        char sql_state[SQL_SQLSTATE_SIZE + 1] = {0};

        if (utf8)
            ret = _sqlConnStmtExecDirectUtf8(stmt, str, str_len, conn);
        else
            ret = _sqlConnStmtExecDirectAscii(stmt, str, str_len, conn);

        if (SQL_SHOW_ERROR(ret))
            sqlConnStmtPrintErrorAndGetState(stmt, str, sql_state);

#ifdef STRESS_RETRY_CODE
        {
            SQLUINTEGER autoCommit = SQL_AUTOCOMMIT_DEFAULT;
            SQLGetConnectAttr(sql.connections[conn], SQL_ATTR_AUTOCOMMIT, &autoCommit, 0, NULL);

            if ((firstFailTime || (autoCommit != SQL_AUTOCOMMIT_OFF)) && !SQL_NEEDS_RETRY(ret))
                break;
        }
#else
        // Do not retry on non error conditions or on the foreground thread
        if (!SQL_NEEDS_RETRY(ret) || SQLSTATE_SKIP_RETRY(sql_state) || (conn == SQLCONN_FOREGROUND))
            break;
#endif

        // Retry until MAX_RETRY_SECONDS has passed
        now = timerSecondsSince2000();
        if (!firstFailTime)
            firstFailTime = now;
        else if ((now - firstFailTime) > MAX_RETRY_SECONDS) {
            LOG(LOG_ERROR, LOG_LEVEL_ALERT, LOG_CONSOLE_ALWAYS, "Giving up on SQL statement for connection %d", conn);
            break;
        }

        LOG(LOG_ERROR, LOG_LEVEL_ALERT, LOG_CONSOLE_ALWAYS, "Rolling back transaction for connection %d", conn);
        SQLCancel(stmt);
        sqlConnEndTransaction(false, true, conn);
        Sleep(randInt(1000) + 1000);
        LOG(LOG_ERROR, LOG_LEVEL_ALERT, LOG_CONSOLE_ALWAYS, "Retrying SQL statement for connection %d", conn);
    }

    return ret;
}

/**
* @note You should always call this command when you pass in 
* more than one sql command at a time. 
*/
INLINEDBG int sqlConnStmtExecDirectMany(HSTMT stmt, char const* str, int str_len, SqlConn conn, bool utf8)
{
    SQLRETURN ret = SQL_SUCCESS;
    SQLRETURN next;

    next = sqlConnStmtExecDirect(stmt, str, str_len, conn, utf8);
    if (SQL_SHOW_ERROR(next))
        sqlConnStmtPrintError(stmt, str);

    if (next < 0)
        ret = next;

#ifdef FULLDEBUG
    // SQLMoreResults does not function properly if SQL_NEED_DATA was returned
    assert(next != SQL_NEED_DATA);
#endif

    while ((next = SQLMoreResults(stmt)) != SQL_NO_DATA) {
        if (SQL_SHOW_ERROR(next))
            sqlConnStmtPrintError(stmt, str);
        if (next < 0)
            ret = next;
        if (ret == SQL_ERROR)
            break;
    }

    return ret;
}

int sqlConnStmtPrepare(HSTMT stmt, char const* str, int str_len, SqlConn conn)
{
    SQLRETURN ret;
    float since_last;

#ifdef FULLDEBUG
    checkQuery(str, str_len);
#endif

    since_last = timerElapsedAndStart(sql.timers[conn]);
    ret = SQLPrepareA(stmt, (char*)str, str_len);
    sqlConnTimerAddTime(conn, since_last);

    if (ret != SQL_SUCCESS)
        sqlConnPrintError(SQL_HANDLE_DBC, sql.connections[conn], str, NULL);

    return ret;
}

int _sqlConnStmtExecute(HSTMT stmt, SqlConn conn)
{
    SQLRETURN ret;
    float since_last;

    since_last = timerElapsedAndStart(sql.timers[conn]);
    ret = SQLExecute(stmt);
    sqlConnTimerAddTime(conn, since_last);

    return ret;
}

int _sqlConnStmtFetch(HSTMT stmt, SqlConn conn) {
    SQLRETURN ret;
    float since_last;

    since_last = timerElapsedAndStart(sql.timers[conn]);
    ret = SQLFetch(stmt);
    sqlConnTimerAddTime(conn, since_last);

    return ret;
}

int sqlConnEndTransaction(bool commit, bool wait, SqlConn conn)
{
    SQLRETURN ret;
    float since_last;

    since_last = timerElapsedAndStart(sql.timers[conn]);

    do {
        ret = SQLEndTran(SQL_HANDLE_DBC, sql.connections[conn], commit ? SQL_COMMIT : SQL_ROLLBACK);
        if(ret != SQL_STILL_EXECUTING) break;
        Sleep(0);
    } while(wait);

    sqlConnTimerAddTime(conn, since_last);

    if (ret != SQL_SUCCESS)
        sqlConnPrintError(SQL_HANDLE_DBC, sql.connections[conn], "SQLEndTran", NULL);

    if (!SQL_SUCCEEDED(ret))
    {
        LOG(LOG_ERROR, LOG_LEVEL_ALERT, LOG_CONSOLE_ALWAYS, "Manual end transaction (commit %d) failed\n", commit);
        devassert(0);
    }

    return ret;
}

int sqlConnGetInfo(unsigned info_type, bool ignore_error, SqlConn conn)
{
    SQLRETURN ret;
    SQLUINTEGER data = 0;

    ret = SQLGetInfo(sql.connections[conn], info_type, &data, sizeof(data), NULL);
    
    if (ret != SQL_SUCCESS && !ignore_error)
        sqlConnPrintErrorf(SQL_HANDLE_DBC, sql.connections[conn], "SQLGetInfo <%d>", info_type);

    if (!SQL_SUCCEEDED(ret))
        return SQL_ERROR;

    return data;
}

static bool sqlConnSetConnectAttr(SQLINTEGER Attribute, SQLPOINTER Value, SqlConn conn)
{
    SQLRETURN ret;

    ret = SQLSetConnectAttr(sql.connections[conn], Attribute, Value, 0);
    if (ret != SQL_SUCCESS)
        sqlConnPrintErrorf(SQL_HANDLE_DBC, sql.connections[conn], "SQLSetConnectAttr <%d=%d>", Attribute, Value);

    return SQL_SUCCEEDED(ret);
}

bool sqlConnEnableAsynchDBC(bool enable, SqlConn conn)
{
    #define SQL_ASYNC_DBC_FUNCTIONS                 10023
    #define SQL_ASYNC_DBC_NOT_CAPABLE               0x00000000L
    #define SQL_ASYNC_DBC_CAPABLE                   0x00000001L

    if(sqlConnGetInfo(SQL_ASYNC_DBC_FUNCTIONS, true, conn) != SQL_ASYNC_DBC_CAPABLE)
        return false;

    #define SQL_ATTR_ASYNC_DBC_FUNCTIONS_ENABLE    117
    #define SQL_ASYNC_DBC_ENABLE_ON         1UL
    #define SQL_ASYNC_DBC_ENABLE_OFF        0UL
    #define SQL_ASYNC_DBC_ENABLE_DEFAULT    SQL_ASYNC_DBC_ENABLE_OFF

    return sqlConnSetConnectAttr(SQL_ATTR_ASYNC_DBC_FUNCTIONS_ENABLE, (SQLPOINTER)(uintptr_t)(enable ? SQL_ASYNC_DBC_ENABLE_ON : SQL_ASYNC_DBC_ENABLE_OFF), conn);
}

bool sqlConnEnableAutoTransactions(bool enable, SqlConn conn)
{
    if(sqlConnGetInfo(SQL_TXN_CAPABLE, false, conn) == SQL_TC_NONE)
        return false;

    return sqlConnSetConnectAttr(SQL_ATTR_AUTOCOMMIT, (SQLPOINTER)(uintptr_t)(enable ? SQL_AUTOCOMMIT_ON : SQL_AUTOCOMMIT_OFF), conn);
}

bool sqlConnDatabaseConnect(const char * db_name, const char * app_name)
{
    unsigned i;
    char conn_str[SHORT_SQL_STRING];
    int conn_str_len;

    if (db_name) {
        strcpy(sql.database_string, db_name);
        conn_str_len = sprintf(conn_str, "%sDatabase=%s;App=%s", sql.login_string, sql.database_string, app_name);
    } else {
        sql.database_string[0] = 0;
        conn_str_len = sprintf(conn_str, "%s;", sql.login_string);
    }

    for (i=0;i <sql.connection_count; i++)
    {
        // Drop any old connections first
        SQLDisconnect(sql.connections[i]);

        if (!SQL_SUCCEEDED(SQLDriverConnectA(sql.connections[i], 0, (SQLCHAR*)conn_str, conn_str_len, NULL, 0, NULL, SQL_DRIVER_NOPROMPT)))
        {
            sqlConnPrintErrorf(SQL_HANDLE_DBC, sql.connections[i], "SQLDriverConnect: %s", conn_str);
            return false;
        }
    }

    // Display ODBC driver information
    {
        char name[256] = {0};
        char sqlver[256] = {0};
        char odbcver[256] = {0};

        SQLGetInfo(sql.connections[SQLCONN_FOREGROUND], SQL_DRIVER_NAME, name, sizeof(name), NULL);
        SQLGetInfo(sql.connections[SQLCONN_FOREGROUND], SQL_DRIVER_VER, sqlver, sizeof(sqlver), NULL);
        SQLGetInfo(sql.connections[SQLCONN_FOREGROUND], SQL_DRIVER_ODBC_VER, odbcver, sizeof(odbcver), NULL);
        printf_stderr("SQL %s %s / ODBC %s\n", name, sqlver, odbcver);
    }

    return true;
}

int sqlConnExecDirect(char *str, int str_len, SqlConn conn, bool utf8)
{
    HSTMT stmt = sqlConnStmtAlloc(conn);
    SQLRETURN ret = sqlConnStmtExecDirect(stmt, str, str_len, conn, utf8);
    sqlConnStmtFree(stmt);
    return ret;
}

/**
* @note You should always call this command when you pass in 
* more than one sql command at a time. 
*/
int sqlConnExecDirectMany(char *str, int str_len, SqlConn conn, bool utf8)
{
    HSTMT stmt = sqlConnStmtAlloc(conn);
    SQLRETURN ret = sqlConnStmtExecDirectMany(stmt, str, str_len, conn, utf8);
    sqlConnStmtFree(stmt);
    return ret;
}

/**
* Switch to binding an array of values instead of a single value
* @param count The number of entries in the array
* @param row_stride The stride in bytes between rows or the value
*                   @ref SQL_PARAM_BIND_BY_COLUMN to indicate
*                   column binding.
*/
int sqlConnStmtBindParamArray(HSTMT stmt, size_t count, size_t row_stride) {
    SQLRETURN ret;

    ret = SQLSetStmtAttr(stmt, SQL_ATTR_PARAMSET_SIZE, (SQLPOINTER)count, SQL_IS_INTEGER);
    if (SQL_SHOW_ERROR(ret))
        sqlConnStmtPrintError(stmt, "SQLSetStmtAttr <SQL_ATTR_PARAMSET_SIZE>");

    ret = SQLSetStmtAttr(stmt, SQL_ATTR_PARAM_BIND_TYPE, (SQLPOINTER)row_stride, SQL_IS_UINTEGER);
    if (SQL_SHOW_ERROR(ret))
        sqlConnStmtPrintError(stmt, "SQLSetStmtAttr <SQL_ATTR_PARAM_BIND_TYPE>");    
    return ret;
}

/**
* Allow the usage of relative addresses for the data and strlen
* parameters by @ref sqlConnStmtBindParam
*/
int sqlConnStmtSetBindOffset(HSTMT stmt, size_t *offset) {
    SQLRETURN ret = SQLSetStmtAttr(stmt, SQL_ATTR_PARAM_BIND_OFFSET_PTR, offset, SQL_IS_UINTEGER);
    if (SQL_SHOW_ERROR(ret))
        sqlConnStmtPrintError(stmt, "SQLSetStmtAttr <SQL_ATTR_PARAM_BIND_OFFSET_PTR>");    
    return ret;
}

int sqlConnStmtBindParam(HSTMT stmt, int param, int paramtype, int ctype, int sqltype, size_t colsize, int decimals, void* data, size_t buflen, ssize_t* strlen) {
    SQLRETURN ret;    

    ret = SQLBindParameter(stmt, param, paramtype, ctype, sqltype, colsize, decimals, data, buflen, strlen);
    if (ret != SQL_SUCCESS)
        sqlConnStmtPrintError(stmt, "SQLBindParameter");
    return ret;
}

int sqlConnStmtStartBindParamTableValued(HSTMT stmt, int param, int maxrows, wchar_t const* tabletype, ssize_t *numrows) {
    SQLRETURN ret;
    
    ret = sqlConnStmtBindParam(stmt, param, SQL_PARAM_INPUT, SQL_C_DEFAULT, SQL_SS_TABLE, maxrows, 0, (void*)tabletype, tabletype ? SQL_NTS : 0, numrows);

    ret = SQLSetStmtAttr(stmt, SQL_SOPT_SS_PARAM_FOCUS, (SQLPOINTER)(uintptr_t)param, SQL_IS_UINTEGER);
    if (SQL_SHOW_ERROR(ret))
        sqlConnStmtPrintError(stmt, "SQLSetStmtAttr <SQL_SOPT_SS_PARAM_FOCUS>");

    return ret;
}

int sqlConnStmtEndBindParamTableValued(HSTMT stmt) {
    SQLRETURN ret = SQLSetStmtAttr(stmt, SQL_SOPT_SS_PARAM_FOCUS, (SQLPOINTER)0, SQL_IS_INTEGER);
    if (SQL_SHOW_ERROR(ret))
        sqlConnStmtPrintError(stmt, "SQLSetStmtAttr <SQL_SOPT_SS_PARAM_FOCUS>");    
    return ret;
}

int sqlConnStmtUnbindParams(HSTMT stmt) {
    SQLRETURN ret = SQLFreeStmt(stmt, SQL_RESET_PARAMS);
    if (ret != SQL_SUCCESS)
        sqlConnStmtPrintError(stmt, "SQLFreeStmt <SQL_RESET_PARAMS>");
    return ret;
}

int sqlConnStmtBindCol(HSTMT stmt, int col, int ctype, void* data, size_t buflen, ssize_t* strlen)
{
    SQLRETURN ret;

    ret = SQLBindCol(stmt, col, ctype, data, buflen, strlen);
    if (ret != SQL_SUCCESS)
        sqlConnStmtPrintError(stmt, "SQLBindParameter");
    return ret;
}

int sqlConnStmtUnbindCols(HSTMT stmt) {
    SQLRETURN ret = SQLFreeStmt(stmt, SQL_UNBIND);
    if (ret != SQL_SUCCESS)
        sqlConnStmtPrintError(stmt, "SQLFreeStmt <SQL_UNBIND>");
    return ret;
}

int sqlConnStmtCloseCursor(HSTMT stmt) {
    SQLRETURN ret = SQLCloseCursor(stmt);
    // TODO Errors disabled because we call this in places we are not sure whether
    // there is an active cursor
#ifdef FULLDEBUG
    if (ret != SQL_SUCCESS)
        sqlConnStmtPrintError(stmt, "SQLCloseCursor");
#endif
    return ret;
}

void* sqlConnStmtGetData(HSTMT stmt, int param, int ctype, ssize_t *length, const char *original_command, SqlConn conn)
{
    static StuffBuff sb[SQLCONN_MAX];

    SQLRETURN ret = 0;
    int terminator_size = 0;

    if (!sb[conn].buff)
        initStuffBuff(&sb[conn], 8192);

    clearStuffBuff(&sb[conn]);

    switch (ctype) {
        xcase SQL_C_CHAR: terminator_size = 1;
        xcase SQL_C_WCHAR: terminator_size = 2;
    }

    for(;;)
    {
        SQLLEN got;
        SQLLEN avail;
        int bytes;        

        avail = sb[conn].size - sb[conn].idx;
        assert(avail);

        ret = SQLGetData(stmt, param, ctype, sb[conn].buff + sb[conn].idx, avail, &got);
        if (!SQL_SUCCEEDED(ret))
            break;

        if (got == SQL_NULL_DATA) {
            *length = SQL_NULL_DATA;
            return "SQL_NULL_DATA";
        }

        bytes = (got > avail) || (got == SQL_NO_TOTAL) ? avail - terminator_size : got;
        assert(bytes >= 0);
        sb[conn].idx += bytes;

        // SQL_SUCCESS_WITH_INFO is used to denote more data is available
        if (ret == SQL_SUCCESS)
            break;

        avail = MAX(got+terminator_size, 4096);
        if ((sb[conn].idx + avail) >= sb[conn].size)
            resizeStuffBuff(&sb[conn], sb[conn].size + avail);
    }

    if (SQL_SHOW_ERROR(ret))
        sqlConnStmtPrintError(stmt, "SQLGetData");

    if (ret != SQL_NO_DATA && !SQL_SUCCEEDED(ret))
    {
        *length = SQL_NULL_DATA;
        return NULL;
    }

    *length = sb[conn].idx;
    return sb[conn].buff;
}
