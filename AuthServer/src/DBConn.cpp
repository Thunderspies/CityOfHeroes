// DBConn.cpp: implementation of the CDBConn class.
//
//////////////////////////////////////////////////////////////////////

#include "DBConn.h"
#include "resource.h"
#include "Config.h"

#define RECOVERY_INTERVAL       30000
#define GLOBALAUTH_REG_ENTRY        "Software\\NCSoft\\GLOBALAUTH"

extern HINSTANCE g_instance;

DBEnv *g_linDB;

DBEnv::DBEnv() : m_lock(eSystemSpinLock, 4000)
{
    m_henv = SQL_NULL_HENV;
    m_pSqlPool = NULL;
    m_pFreeSqlPool = NULL;
    m_pFreeSqlPoolEnd = NULL;
    m_connCount = 0;    
}

DBEnv::~DBEnv()
{
    if(m_henv != SQL_NULL_HENV) {
        Destroy();
    }
    if(m_pSqlPool != NULL) {
        delete [] m_pSqlPool;
    }
}

const char* DBEnv::GameIdToRegistryKey(int gameId)
{
    if ( config.gameId == 0 || config.gameId    == 8 || config.gameId == 16 || config.gameId == 32) 
        return "AuthDB";
    else if ( config.gameId == 4 )
        return "sldb";
    else
    {
        return NULL;
    }
}

// Return TRUE on successful connection.  Otherwise return FALSE.
bool DBEnv::Init(int connCount)
{
    m_connCount = connCount;
    m_pSqlPool = new SQLPool[m_connCount];
    SQLSetEnvAttr(NULL, SQL_ATTR_CONNECTION_POOLING, (SQLPOINTER)SQL_CP_ONE_PER_DRIVER,    SQL_IS_INTEGER);

    // Allocate environment handle
    SQLRETURN retcode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_henv);
    if(retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
        // Set the ODBC version environment attribute
        retcode = SQLSetEnvAttr(m_henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0);
        if(retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
            if(Login()) {
                AllocSQLPool();
                return TRUE;
            } else {
                logger.AddLog(LOG_ERROR, "db login failed");
                SQLFreeHandle(SQL_HANDLE_ENV, m_henv);
                m_henv = SQL_NULL_HENV;
                //TBROWN - delete our pool
                delete[] m_pSqlPool;
                return FALSE;
            }
        }
    }
    logger.AddLog(LOG_ERROR, "db env allocation failed");
    return FALSE;
}

bool DBEnv::Login(bool reset)
{
    SQLHDBC hDbc;
    bool loadFromConfig = true;

    if (!LoadConnStrFromConfig()) {
        loadFromConfig = false;
        if (!LoadConnStrFromReg()) {
            DialogBoxParam(g_instance, MAKEINTRESOURCE(IDD_LOGIN), NULL, (DLGPROC)LoginDlgProc, (LPARAM)this);
        }
    }
    SQLRETURN ret = SQLAllocHandle(SQL_HANDLE_DBC, m_henv, &hDbc);
    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
        if (reset == true ){
            SQLSetConnectAttr(hDbc, SQL_ATTR_LOGIN_TIMEOUT, (SQLPOINTER)20, 0);
        } else {
            SQLSetConnectAttr(hDbc, SQL_ATTR_LOGIN_TIMEOUT, (SQLPOINTER)60, 0);
        }
        SQLSetConnectAttr(hDbc, SQL_ATTR_CONNECTION_TIMEOUT, (SQLPOINTER)60, 0);
        SQLCHAR complStr[1024];
        SQLSMALLINT complLen;
        while (1) {
            ret = SQLDriverConnectA(hDbc, NULL, m_connStr, (SQLSMALLINT)strlen((const char*)m_connStr),
                complStr, sizeof(complStr), &complLen, SQL_DRIVER_NOPROMPT);
            if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
                break;
            if (!reset && (!config.connectionString || !config.connectionString[0])) {
                DialogBoxParam(g_instance, MAKEINTRESOURCE(IDD_LOGIN), NULL, (DLGPROC)LoginDlgProc, (LPARAM)this);
            }
            else if (config.connectionString) {
                if (loadFromConfig) {
                    logger.AddLog(LOG_ERROR, "Connection string in etc\\config.txt is invalid!");
                } else {
                    logger.AddLog(LOG_ERROR, "Connection string in registry is invalid!");
                }
                return false;
            }
        }
        SQLDisconnect(hDbc);
        SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
        return true;
    } else {
        // Allocation failed.
        AS_LOG_VERBOSE( "hdbc allocation failed");
        return false;
    }
}

void DBEnv::AllocSQLPool(void)
{
    for (int i = 0; i < m_connCount; i++) {
        m_pSqlPool[i].pNext = NULL;
        // Allocate connection.
        SQLRETURN ret = SQLAllocHandle(SQL_HANDLE_DBC, m_henv, &m_pSqlPool[i].hdbc);
        if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
            // Set login timeout to 1 minutes.
            SQLSetConnectAttr(m_pSqlPool[i].hdbc, SQL_ATTR_LOGIN_TIMEOUT, (SQLPOINTER)60, 0);
            // Connection timeout to 1 minutes.
            SQLSetConnectAttr(m_pSqlPool[i].hdbc, SQL_ATTR_CONNECTION_TIMEOUT, (SQLPOINTER)60, 0);
            // Connect to data source
            SQLCHAR complStr[1024];
            SQLSMALLINT complLen;
            ret = SQLDriverConnectA(m_pSqlPool[i].hdbc, NULL, m_connStr, (SQLSMALLINT)strlen((const char*)m_connStr),
                complStr, sizeof(complStr), &complLen, SQL_DRIVER_NOPROMPT);
            if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
                ret = SQLAllocHandle(SQL_HANDLE_STMT, m_pSqlPool[i].hdbc, &m_pSqlPool[i].stmt);
                if (!(ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)) {
                    logger.AddLog(LOG_ERROR, "stmt allocation failed");
                    SQLFreeHandle(SQL_HANDLE_DBC, m_pSqlPool[i].hdbc);
                    m_pSqlPool[i].stmt = 0;
                    m_pSqlPool[i].hdbc = 0;
                    m_pSqlPool[i].reset = FALSE;
                } else {
                    m_pSqlPool[i].reset = TRUE;
                }
                
            } else {
                // Connection failed.
                logger.AddLog(LOG_ERROR, "hdbc connection failed");
                SQLFreeHandle(SQL_HANDLE_DBC, m_pSqlPool[i].hdbc);
                m_pSqlPool[i].stmt = 0;
                m_pSqlPool[i].hdbc = 0;
                m_pSqlPool[i].reset = FALSE;
            }
        } else {
            // Allocation failed.
            logger.AddLog(LOG_ERROR, "hdbc allocation failed");
            m_pSqlPool[i].stmt = 0;
            m_pSqlPool[i].hdbc = 0;
            m_pSqlPool[i].reset = FALSE;
        }
    }
// Let's salvage valid statement handles. (Actually all handles should be valid.)
    for (int i = 0; i < m_connCount; i++) {
        if (m_pSqlPool[i].stmt) {
            if (m_pFreeSqlPool) {
                m_pSqlPool[i].pNext = m_pFreeSqlPool;
                m_pFreeSqlPool = &m_pSqlPool[i];
            } else {
                m_pFreeSqlPool = &m_pSqlPool[i];
                m_pFreeSqlPoolEnd = &m_pSqlPool[i];
            }
        }
    }
}

void DBEnv::Destroy()
{
    for(int i = 0; i < m_connCount; i++) {
        if(m_pSqlPool[i].stmt) {
            SQLFreeHandle(SQL_HANDLE_STMT, m_pSqlPool[i].stmt);
//            m_pSqlPool[i].stmt = 0;
        }
        if(m_pSqlPool[i].hdbc) {
            SQLDisconnect(m_pSqlPool[i].hdbc);
            SQLFreeHandle(SQL_HANDLE_DBC, m_pSqlPool[i].hdbc);
//            m_pSqlPool[i].hdbc = 0;
        }
    }

    SQLFreeHandle(SQL_HANDLE_ENV, m_henv);
    m_henv = SQL_NULL_HENV;
}

bool DBEnv::LoadConnStrFromConfig() {
    if (!config.connectionString || !config.connectionString[0]) {
        return false;
    }

    strcpy((char *)m_connStr, config.connectionString);
    return true;
}

bool DBEnv::LoadConnStrFromReg()
{
    HKEY hKey;
    unsigned char buffer[MAX_CONN_STR];
    bool strExists = false;
    LONG e;
    DWORD dwType;
    DWORD dwSize;

    const char *keyStr = GameIdToRegistryKey(config.gameId);
    if (keyStr == NULL)
    {
        //TBROWN - returning false here instead of exceptioning below
        logger.AddLog(LOG_WARN, "Invalid gameId set in config file. Will manually prompt instead.");
        return false;
    }

    e = RegOpenKeyExA(HKEY_CURRENT_USER, GLOBALAUTH_REG_ENTRY, 0, KEY_READ, &hKey);
    if (e == ERROR_SUCCESS) {
        dwSize = MAX_CONN_STR;
        e = RegQueryValueEx(hKey, (LPTSTR)keyStr, NULL, &dwType, buffer, &dwSize);
        if ((e == ERROR_SUCCESS) && (dwType == REG_BINARY)) {
            strExists = true;
        }
        RegCloseKey(hKey);
    }

    if (strExists) {
        DesReadBlock(buffer, MAX_CONN_STR);
        strcpy((char *)m_connStr, (const char *)buffer);
    }

    return strExists;
}

void DBEnv::SaveConnStrToReg()
{
    HKEY hKey;
    DWORD dwResult;
    unsigned char buffer[MAX_CONN_STR];
    //char *keyStr;

    //TBROWN - adding the gameId == 0 here because that's the default, and it wasn't handled before
    const char *keyStr = GameIdToRegistryKey(config.gameId);
    if (keyStr == NULL)
    {
        const char *error_message = "Invalid gameId set in config file. Don't know how to store the connection in the registry.";
        logger.AddLog(LOG_ERROR, error_message);
        MessageBoxA( NULL, error_message, "Error", MB_ICONERROR | MB_OK );
        exit(0);
    }


    strcpy((char *)buffer, (const char *)m_connStr);
    DesWriteBlock(buffer, MAX_CONN_STR);
    LONG e = RegCreateKeyExA(HKEY_CURRENT_USER, GLOBALAUTH_REG_ENTRY, 0, NULL, 
      REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, &dwResult);
    if (e == ERROR_SUCCESS) {
        DWORD dwType = REG_BINARY;
        DWORD dwData = 0;
        RegSetValueEx(hKey, (LPTSTR)keyStr, NULL, REG_BINARY, buffer, MAX_CONN_STR);
        RegCloseKey(hKey);
    }
}

void DBEnv::OnTimerCallback()
{
    if(Login(true)) {
        m_lock.Enter();
        Destroy();
        SQLSetEnvAttr(NULL, SQL_ATTR_CONNECTION_POOLING, 
          (SQLPOINTER)SQL_CP_ONE_PER_DRIVER, SQL_IS_INTEGER);
        SQLRETURN retcode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_henv);
        if(retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
            retcode = SQLSetEnvAttr(m_henv, SQL_ATTR_ODBC_VERSION, 
              (SQLPOINTER)SQL_OV_ODBC3, 0);
            if(retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
                AllocSQLPool();
                m_recoveryNeeded = FALSE;
                m_lock.Leave();
                logger.AddLog(LOG_NORMAL, "db recovery succeeded");
            } else {
                m_lock.Leave();
                RegisterTimer(RECOVERY_INTERVAL);
                logger.AddLog(LOG_ERROR, "db recovery failed: env allocation");
            }
        } else
            m_lock.Leave();
    } else {
        logger.AddLog(LOG_ERROR, "db recovery failed: login impossible");
        RegisterTimer(RECOVERY_INTERVAL);
    }
    ReleaseRef();
    return;
}

CDBConn::CDBConn(DBEnv *env)
{
    m_pEnv = env;
    m_pCurSql = NULL;
    while (1) {
        m_pEnv->m_lock.Enter();
        // Take one off the front of m_pEnv->m_pFreeSqlPool
        if (m_pEnv->m_pFreeSqlPool)
        {
            m_pCurSql = m_pEnv->m_pFreeSqlPool;
            m_pCurSql->reset = FALSE;

            m_pEnv->m_pFreeSqlPool = m_pEnv->m_pFreeSqlPool->pNext;

            if (!m_pEnv->m_pFreeSqlPool) 
            {
                // Must be the last item in the pool
                // assert(m_pEnv->m_pFreeSqlPool == m_pEnv->m_pFreeSqlPoolEnd);
                m_pEnv->m_pFreeSqlPoolEnd = NULL;
            }
        }
        
        m_pEnv->m_lock.Leave();
        if (m_pCurSql) {
            break;
        } else {
            Sleep(100);            // Wait 100 ms and check again
        }
    }
    m_stmt = m_pCurSql->stmt;
    m_colNum = 1;
    m_paramNum = 1;
}

CDBConn::~CDBConn()
{
    m_pEnv->m_lock.Enter();
    if(!m_pCurSql->reset) {
        SQLFreeStmt(m_stmt, SQL_UNBIND);
        SQLFreeStmt(m_stmt, SQL_CLOSE);
        SQLFreeStmt(m_stmt, SQL_RESET_PARAMS);

        // Add it back to the end of the free pool
        m_pCurSql->pNext = NULL;
        if (m_pEnv->m_pFreeSqlPool)
        {
            m_pEnv->m_pFreeSqlPoolEnd->pNext = m_pCurSql;
        }
        else
        {
            // assert(!m_pEnv->m_pFreeSqlPoolEnd);
            m_pEnv->m_pFreeSqlPool = m_pCurSql;
        }
        m_pEnv->m_pFreeSqlPoolEnd = m_pCurSql;
    }
    m_pEnv->m_lock.Leave();
}

BOOL CALLBACK LoginDlgProc(HWND hDlg, DWORD dwMessage, DWORD wParam, LPARAM lParam)
{
    static DBEnv *pEnv;

    switch (dwMessage) {
    case WM_INITDIALOG:
        pEnv = (DBEnv *)(LONG_PTR)lParam;
        const char *pDefault;
        const char *pTitle;
        pDefault = "AuthDB";
        pTitle = "ODBC Connection Info";
        SendDlgItemMessageA(hDlg, IDC_FILE, WM_SETTEXT, 0, (LPARAM)pDefault);
        SetWindowTextA(hDlg, pTitle);
        return 0;
    case WM_COMMAND:
        switch (wParam) {
        case IDOK:
        {
                char buffer[64] = {0};
                char *pTempConnStr;
                pTempConnStr = (char *)pEnv->m_connStr;

                SendDlgItemMessage(hDlg, IDC_FILE, WM_GETTEXT, 64, (LPARAM) buffer);
                strcpy(pTempConnStr, "FILEDSN=");
                strcat(pTempConnStr, buffer);
                SendDlgItemMessage(hDlg, IDC_USER, WM_GETTEXT, 64, (LPARAM) buffer);
                strcat(pTempConnStr, ";UID=");
                strcat(pTempConnStr, buffer);
                SendDlgItemMessage(hDlg, IDC_PASS, WM_GETTEXT, 64, (LPARAM) buffer);
                strcat(pTempConnStr, ";PWD=");
                strcat(pTempConnStr, buffer);
                pEnv->SaveConnStrToReg();
                EndDialog(hDlg, 0);
                break;
            }
        }
        break;
    }
    return 0;
}

bool CDBConn::Execute(const char *format, ...)
{
    char buffer[8192];
    va_list ap;
    va_start(ap, format);
    int len = vsprintf(buffer, format, ap);
    va_end(ap);
    SQLRETURN r = SQL_ERROR;
    if (len > 0) {
        r = SQLExecDirectA(m_stmt, (SQLCHAR*)buffer, len);
    }
    if (r == SQL_SUCCESS ) {
        return true;
    } else {
        Error(SQL_HANDLE_STMT, m_stmt, buffer);
        return false;
    }
}

bool CDBConn::ExecuteIndirect(const char *format, ...)
{
    char buffer[4096];
    va_list ap;
    va_start(ap, format);
    int len = vsprintf(buffer, format, ap);
    va_end(ap);
    SQLRETURN r = SQL_ERROR;
    if (len > 0) {
        SQLPrepareA(m_stmt, (SQLCHAR*)buffer, SQL_NTS);
        r = SQLExecute(m_stmt);
    }
    if (r == SQL_SUCCESS || r == SQL_SUCCESS_WITH_INFO) {
        return true;
    } else {
        Error(SQL_HANDLE_STMT, m_stmt, buffer);
        return false;
    }
}

// Same as Execute() except that primary key violation error is not shown.
bool CDBConn::ExecuteInsert(const char *format, ...)
{
//    if (!stmt) {
//        return false;
//    }
    char buffer[4096];
    va_list ap;
    va_start(ap, format);
    int len = vsprintf(buffer, format, ap);
    va_end(ap);
    SQLRETURN r = SQL_ERROR;
    if (len > 0) {
        r = SQLExecDirectA(m_stmt, (SQLCHAR*)buffer, len);
    }
    if (r == SQL_SUCCESS || r == SQL_SUCCESS_WITH_INFO) {
        return true;
    } else {
        ErrorExceptInsert(SQL_HANDLE_STMT, m_stmt, buffer);
        return false;
    }
}

bool CDBConn::ExecuteInsertIndirect(const char *format, ...)
{
    char buffer[4096];
    va_list ap;
    va_start(ap, format);
    int len = vsprintf(buffer, format, ap);
    va_end(ap);
    SQLRETURN r = SQL_ERROR;
    if (len > 0) {
        SQLPrepareA(m_stmt, (SQLCHAR*)buffer, SQL_NTS);
        r = SQLExecute(m_stmt);
    }
    if (r == SQL_SUCCESS || r == SQL_SUCCESS_WITH_INFO) {
        return true;
    } else {
        ErrorExceptInsert(SQL_HANDLE_STMT, m_stmt, buffer);
        return false;
    }
}

// Same as Execute() except that SQL_NO_DATA is not error.
bool CDBConn::ExecuteDelete(const char *format, ...)
{
    char buffer[4096];
    va_list ap;
    va_start(ap, format);
    int len = vsprintf(buffer, format, ap);
    va_end(ap);
    SQLRETURN r = SQL_ERROR;
    if (len > 0) {
        r = SQLExecDirectA(m_stmt, (SQLCHAR*)buffer, len);
    }
    if (r == SQL_SUCCESS || r == SQL_SUCCESS_WITH_INFO || r == SQL_NO_DATA) {
        return true;
    } else {
        Error(SQL_HANDLE_STMT, m_stmt, buffer);
        return false;
    }
}

bool CDBConn::Fetch(bool *nodata)
{
    SQLRETURN r = SQLFetch(m_stmt);
    if(nodata) {
        *nodata = (r == SQL_NO_DATA);
        return true;
    }

    if(r == SQL_SUCCESS || r == SQL_SUCCESS_WITH_INFO) {
        return true;
    } else {
        if (r != SQL_NO_DATA) {
            logger.AddLog(LOG_ERROR, "Fetch error = %d", r);
        }
        return false;
    }
}

bool CDBConn::SetAutoCommit(bool autoCommit)
{
    SQLRETURN ret;
    if (autoCommit) {
        // Turn on auto commit mode.
        ret = SQLSetConnectAttr(m_pCurSql->hdbc, SQL_ATTR_AUTOCOMMIT, (SQLPOINTER)SQL_AUTOCOMMIT_ON, 4);
    } else {
        // Turn off auto commit mode.
        ret = SQLSetConnectAttr(m_pCurSql->hdbc, SQL_ATTR_AUTOCOMMIT, (SQLPOINTER)SQL_AUTOCOMMIT_OFF, 4);
    }
    if (ret == SQL_ERROR || ret == SQL_INVALID_HANDLE) {
        Error(SQL_HANDLE_DBC, m_pCurSql->hdbc, 0);
        return false;
    } else {
        return true;
    }
}

bool CDBConn::EndTran(SQLSMALLINT compType)
{
    SQLRETURN r = SQLEndTran(SQL_HANDLE_DBC, m_pCurSql->hdbc, compType);
    if (r == SQL_SUCCESS || r == SQL_SUCCESS_WITH_INFO) {
        return true;
    } else {
        Error(SQL_HANDLE_STMT, m_stmt, 0);
        return false;
    }
}

void CDBConn::Error(SQLSMALLINT handleType, SQLHANDLE handle, const char *command)
{
    unsigned char message[SQL_MAX_MESSAGE_LENGTH];
    unsigned char state[SQL_SQLSTATE_SIZE + 1];
    SQLINTEGER native;
    SQLSMALLINT len;
    SQLRETURN r;

    r = SQLGetDiagRecA(handleType, handle, 1, state, &native, message, sizeof(message), &len);
    if (r == SQL_SUCCESS) {
        if (command) {
            logger.AddLog(LOG_ERROR, "sql: %s", command);
        }
        logger.AddLog(LOG_ERROR, "%s:%s", state, (char*)message);
        if(strcmp((char *)state, "08S01") == 0) {
            m_pEnv->m_lock.Enter();
            if(!m_pEnv->m_recoveryNeeded && !m_pCurSql->reset) {
                m_pEnv->RegisterTimer(RECOVERY_INTERVAL);
                m_pEnv->m_recoveryNeeded = TRUE;
            }
            m_pEnv->m_lock.Leave();
        }
    }
    
}

void CDBConn::ErrorExceptInsert(SQLSMALLINT handleType, SQLHANDLE handle, const char *command)
{
    SQLSMALLINT len;
    SQLINTEGER native;
    unsigned char state[SQL_SQLSTATE_SIZE + 1];
    unsigned char message[SQL_MAX_MESSAGE_LENGTH];
    if (SQLGetDiagRecA(handleType, handle, 1, state, &native, message, sizeof(message), &len) == SQL_SUCCESS) {
        if (strcmp((char*)state, "23000")) {
            if (command) {
                logger.AddLog(LOG_ERROR, "sql: %s", command);
            }
            logger.AddLog(LOG_ERROR, "%s:%s", state, (char*)message);
        }
    }
}

void CDBConn::SetColumnNumber(int n)
{
    m_colNum = n;
}

void CDBConn::Bind(char *n)
{
    SQLBindCol(m_stmt, m_colNum++, SQL_C_TINYINT, n, 1, 0);
}

void CDBConn::Bind(unsigned char *n)
{
    SQLBindCol(m_stmt, m_colNum++, SQL_C_TINYINT, n, 1, 0);
}

void CDBConn::Bind(char *str, int size)
{
    SQLBindCol(m_stmt, m_colNum++, SQL_C_CHAR, str, size, 0);
}

void CDBConn::Bind(unsigned *n)
{
    SQLBindCol(m_stmt, m_colNum++, SQL_C_LONG, n, 4, 0);
}

void CDBConn::Bind(int *n)
{
    SQLBindCol(m_stmt, m_colNum++, SQL_C_LONG, n, 4, 0);
}

void CDBConn::Bind(short *n)
{
    SQLBindCol(m_stmt, m_colNum++, SQL_C_SHORT, n, 2, 0);
}

void CDBConn::SetParamNumber(int n)
{
    m_paramNum = n;
}

void CDBConn::BindParam(char *n, SQLSMALLINT type)
{
    SQLBindParameter(m_stmt, m_paramNum++, type, SQL_C_TINYINT, SQL_TINYINT, 0, 0, n, 0, 
      NULL);
}

void CDBConn::BindParam(unsigned char *n, SQLSMALLINT type)
{
    SQLBindParameter(m_stmt, m_paramNum++, type, SQL_C_TINYINT, SQL_TINYINT, 0, 0, n, 0, 
      NULL);
}

void CDBConn::BindParam(char *str, int size, SQLSMALLINT type)
{
    SQLBindParameter(m_stmt, m_paramNum++, type, SQL_C_CHAR, SQL_VARCHAR, size, 0, str, 
      size + 1, NULL);
}

void CDBConn::BindParam(unsigned *n, SQLSMALLINT type)
{
    SQLBindParameter(m_stmt, m_paramNum++, type, SQL_C_LONG, SQL_INTEGER, 0, 0, n, 
      0, NULL);
}

void CDBConn::BindParam(int *n, SQLSMALLINT type)
{
    SQLBindParameter(m_stmt, m_paramNum++, type, SQL_C_LONG, SQL_INTEGER, 0, 0, n, 
      0, NULL);
}

void CDBConn::BindParam(short *n, SQLSMALLINT type)
{
    SQLBindParameter(m_stmt, m_paramNum++, type, SQL_C_SHORT, SQL_SMALLINT, 0, 0, n, 
      0, NULL);
}

void CDBConn::ResetHtmt(void)
{
    m_colNum = 1;
    m_paramNum = 1;
    SQLFreeStmt(m_stmt, SQL_UNBIND);
    SQLFreeStmt(m_stmt, SQL_CLOSE);
}

