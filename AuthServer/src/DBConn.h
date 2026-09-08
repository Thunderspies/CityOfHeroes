// DBConn.h: interface for the CDBConn class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DBCONN_H__BE8285EA_B6EF_4AB1_83B4_8FE6EA59F5BA__INCLUDED_)
#define AFX_DBCONN_H__BE8285EA_B6EF_4AB1_83B4_8FE6EA59F5BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GlobalAuth.h"
#include "IOObject.h"
#include "Lock.h"

#define MAX_CONN_STR 256

typedef struct _SQLPool_ 
{
    //TBROWN - adding a constructor to init these values so that it's obvious if they
    //need to be cleaned up or not
    _SQLPool_() : reset(false), hdbc(NULL), stmt(NULL), pNext(NULL)
    {
        bool here = true;
    }
    BOOL reset;
    SQLHDBC hdbc;
    SQLHSTMT stmt;
    struct _SQLPool_ *pNext;
} SQLPool;

class DBEnv : public CIOObject{
protected:
    CLock m_lock;
    SQLHENV m_henv;
    SQLPool *m_pSqlPool;
    SQLPool *m_pFreeSqlPool;
    SQLPool *m_pFreeSqlPoolEnd;
    SQLCHAR m_connStr[MAX_CONN_STR];
    int m_connCount;
    BOOL m_recoveryNeeded;

    static const char* GameIdToRegistryKey(int gameId);

public:
    virtual void OnEventCallback() {}
    virtual void OnTimerCallback();
    virtual void OnIOCallback(BOOL success, DWORD transferred, LPOVERLAPPED pOverlapped) {}

    friend class CDBConn;
    friend INT_PTR CALLBACK LoginDlgProc(HWND hDlg, UINT dwMessage, WPARAM wParam, LPARAM lParam);

    DBEnv();
    ~DBEnv();
    bool Init(int connCount);
    void Destroy();
    bool Login(bool reset=false);
    void AllocSQLPool();
    bool LoadConnStrFromConfig();
    bool LoadConnStrFromReg();
    void SaveConnStrToReg();
};

class CDBConn {
public:
    SQLHSTMT m_stmt;
    int m_colNum;
    int m_paramNum;

    CDBConn(DBEnv *env);
    ~CDBConn();

    bool SetAutoCommit(bool autoCommit);
    bool EndTran(SQLSMALLINT compType);
    bool Execute(const char *format, ...);
    bool ExecuteIndirect(const char *format, ...);
    bool ExecuteInsert(const char *format, ...);
    bool ExecuteInsertIndirect(const char *format, ...);
    bool ExecuteDelete(const char *format, ...);
    bool Fetch(bool *nodata = NULL);
    void Error(SQLSMALLINT handleType, SQLHANDLE handle, const char *command);
    void ErrorExceptInsert(SQLSMALLINT handleType, SQLHANDLE handle, const char *command);

    // column binding for fetch
    void SetColumnNumber(int n);
    void Bind(char *str, int size);
    void Bind(char *n);
    void Bind(unsigned char *n);
    void Bind(unsigned *n);
    void Bind(int *n);
    void Bind(short *n);

    // parameter binding for parameterized call
    void SetParamNumber(int n);
    void BindParam(char *str, int size, SQLSMALLINT type = SQL_PARAM_INPUT);
    void BindParam(char *n, SQLSMALLINT type = SQL_PARAM_INPUT);
    void BindParam(unsigned char *n, SQLSMALLINT type = SQL_PARAM_INPUT);
    void BindParam(unsigned *n, SQLSMALLINT type = SQL_PARAM_INPUT);
    void BindParam(int *n, SQLSMALLINT type = SQL_PARAM_INPUT);
    void BindParam(short *n, SQLSMALLINT type = SQL_PARAM_INPUT);

    void ResetHtmt(void);
    void CloseCursor(void) { SQLCloseCursor(m_stmt); }

   // friend BOOL CALLBACK LoginDlgProc(HWND hDlg, DWORD dwMessage, DWORD wParam, LPARAM lParam);

protected:
    SQLPool *m_pCurSql;
    DBEnv *m_pEnv;

};

extern DBEnv *g_linDB;

INT_PTR CALLBACK LoginDlgProc(HWND hDlg, UINT dwMessage, WPARAM wParam, LPARAM lParam);

#endif // !defined(AFX_DBCONN_H__BE8285EA_B6EF_4AB1_83B4_8FE6EA59F5BA__INCLUDED_)
