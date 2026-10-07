#include "database.h"
#include "sql/sqlinclude.h"
#include <limits.h>

typedef struct OdbcConnection { DbStorageConnection base; SqlConn index; } OdbcConnection;
typedef struct OdbcStorage { DbStorage base; OdbcConnection connections[SQLCONN_MAX]; } OdbcStorage;
typedef struct OdbcBind {
    struct OdbcBind *next;
    int index;
    void *data;
    ssize_t length;
} OdbcBind;
typedef struct OdbcColumn { ContainerValue value; void *data; bool loaded; } OdbcColumn;
typedef struct OdbcStatement {
    DbStorageStatement base;
    HSTMT handle;
    SqlConn connection;
    OdbcBind *binds;
    OdbcColumn *columns;
    SQLSMALLINT columnCount;
    bool executed;
} OdbcStatement;

static void odbcError(DbStorageError *error, HSTMT statement, const char *operation)
{
    if (!error) return;
    memset(error, 0, sizeof(*error));
    snprintf(error->message, sizeof(error->message), "ODBC %s failed; see DBServer log", operation);
    if (statement) sqlConnStmtPrintErrorAndGetState(statement, operation, error->state);
}

static void odbcClearColumns(OdbcStatement *statement)
{
    int i;
    for (i = 0; i < statement->columnCount; ++i) free(statement->columns[i].data);
    free(statement->columns);
    statement->columns = NULL;
    statement->columnCount = 0;
}

static void odbcDestroy(DbStorage *storage) { free(storage); }
static DbStorageConnection *odbcConnection(DbStorage *storage, SqlConn index)
{
    return index < SQLCONN_MAX ? &((OdbcStorage *)storage)->connections[index].base : NULL;
}

static DbStorageStatement *odbcPrepare(DbStorageConnection *connection, const char *sql, size_t length, DbStorageError *error)
{
    OdbcConnection *conn = (OdbcConnection *)connection;
    OdbcStatement *statement;
    if (length > INT_MAX) { odbcError(error, NULL, "SQL length"); return NULL; }
    statement = calloc(1, sizeof(*statement));
    if (!statement) { odbcError(error, NULL, "allocation"); return NULL; }
    statement->base.ops = connection->ops;
    statement->connection = conn->index;
    statement->handle = sqlConnStmtAlloc(conn->index);
    if (!SQL_SUCCEEDED(sqlConnStmtPrepare(statement->handle, sql, (int)length, conn->index))) {
        odbcError(error, statement->handle, "prepare");
        sqlConnStmtFree(statement->handle); free(statement); return NULL;
    }
    return &statement->base;
}

static DbStorageResult odbcExecute(DbStorageStatement *opaque, DbStorageError *error)
{
    OdbcStatement *statement = (OdbcStatement *)opaque;
    if (!SQL_SUCCEEDED(_sqlConnStmtExecute(statement->handle, statement->connection))) {
        odbcError(error, statement->handle, "execute"); return DB_STORAGE_ERROR;
    }
    statement->executed = true;
    if (!SQL_SUCCEEDED(SQLNumResultCols(statement->handle, &statement->columnCount))) {
        odbcError(error, statement->handle, "result metadata"); return DB_STORAGE_ERROR;
    }
    statement->columns = calloc(statement->columnCount, sizeof(*statement->columns));
    if (statement->columnCount && !statement->columns) {
        statement->columnCount = 0;
        odbcError(error, NULL, "allocation"); return DB_STORAGE_ERROR;
    }
    return statement->columnCount ? DB_STORAGE_OK : DB_STORAGE_DONE;
}

static DbStorageResult odbcStep(DbStorageStatement *opaque, DbStorageError *error)
{
    OdbcStatement *statement = (OdbcStatement *)opaque;
    int i, result;
    if (!statement->executed && odbcExecute(opaque, error) == DB_STORAGE_ERROR) return DB_STORAGE_ERROR;
    if (!statement->columnCount) return DB_STORAGE_DONE;
    for (i = 0; i < statement->columnCount; ++i) {
        free(statement->columns[i].data); memset(&statement->columns[i], 0, sizeof(statement->columns[i]));
    }
    result = _sqlConnStmtFetch(statement->handle, statement->connection);
    if (result == SQL_NO_DATA) return DB_STORAGE_DONE;
    if (SQL_SUCCEEDED(result)) return DB_STORAGE_ROW;
    odbcError(error, statement->handle, "fetch"); return DB_STORAGE_ERROR;
}

static void odbcReset(DbStorageStatement *opaque)
{
    OdbcStatement *statement = (OdbcStatement *)opaque;
    sqlConnStmtCloseCursor(statement->handle);
    odbcClearColumns(statement);
    statement->executed = false;
}

static void odbcFinalize(DbStorageStatement *opaque)
{
    OdbcStatement *statement = (OdbcStatement *)opaque;
    OdbcBind *binding = statement->binds;
    sqlConnStmtFree(statement->handle);
    while (binding) { OdbcBind *next = binding->next; free(binding->data); free(binding); binding = next; }
    odbcClearColumns(statement); free(statement);
}

static DbStorageResult odbcTransaction(DbStorageConnection *opaque, bool begin, bool commit, DbStorageError *error)
{
    OdbcConnection *connection = (OdbcConnection *)opaque;
    bool success = begin ? sqlConnEnableAutoTransactions(false, connection->index) : SQL_SUCCEEDED(sqlConnEndTransaction(commit, true, connection->index));
    if (!begin && success) success = sqlConnEnableAutoTransactions(true, connection->index);
    if (!success) odbcError(error, NULL, "transaction");
    return success ? DB_STORAGE_OK : DB_STORAGE_ERROR;
}

static DbStorageCapabilities odbcCapabilities(DbStorage *storage)
{
    DbStorageCapabilities capabilities = {SQLCONN_MAX - 1, 1500, true, true, true, true};
    (void)storage; return capabilities;
}

static DbStorageResult odbcBind(DbStorageStatement *opaque, int index, ContainerValue value, DbStorageError *error)
{
    OdbcStatement *statement = (OdbcStatement *)opaque;
    OdbcBind *binding;
    int ctype = SQL_C_CHAR, sqltype = SQL_VARCHAR;
    size_t bytes = value.size;
    if (index < 1 || bytes > INT_MAX || (bytes && !value.data)) { odbcError(error, NULL, "bind value"); return DB_STORAGE_ERROR; }
    if ((value.type == CONTAINER_VALUE_INT && bytes != sizeof(S64)) ||
        (value.type == CONTAINER_VALUE_FLOAT && bytes != sizeof(double))) {
        odbcError(error, NULL, "numeric bind width"); return DB_STORAGE_ERROR;
    }
    for (binding = statement->binds; binding && binding->index != index; binding = binding->next) {}
    if (!binding) {
        binding = calloc(1, sizeof(*binding));
        if (!binding) { odbcError(error, NULL, "allocation"); return DB_STORAGE_ERROR; }
        binding->index = index; binding->next = statement->binds; statement->binds = binding;
    }
    free(binding->data); binding->data = NULL;
    switch (value.type) {
        case CONTAINER_VALUE_INT: ctype = SQL_C_SBIGINT; sqltype = SQL_BIGINT; break;
        case CONTAINER_VALUE_FLOAT: ctype = SQL_C_DOUBLE; sqltype = SQL_DOUBLE; break;
        case CONTAINER_VALUE_BLOB: ctype = SQL_C_BINARY; sqltype = SQL_VARBINARY; break;
        case CONTAINER_VALUE_DATETIME: sqltype = SQL_TYPE_TIMESTAMP; break;
        case CONTAINER_VALUE_TEXT: {
            int chars = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, value.data, (int)bytes, NULL, 0);
            if (bytes && !chars) { odbcError(error, NULL, "UTF-8 bind"); return DB_STORAGE_ERROR; }
            bytes = chars * sizeof(wchar_t);
            binding->data = calloc(chars + 1, sizeof(wchar_t));
            if (!binding->data) { odbcError(error, NULL, "allocation"); return DB_STORAGE_ERROR; }
            if (chars) MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, value.data, (int)value.size, binding->data, chars);
            ctype = SQL_C_WCHAR; sqltype = SQL_WVARCHAR; break;
        }
        case CONTAINER_VALUE_NULL: bytes = 0; break;
        default: odbcError(error, NULL, "value type"); return DB_STORAGE_ERROR;
    }
    if (!binding->data) {
        binding->data = calloc(1, bytes + 1);
        if (!binding->data) { odbcError(error, NULL, "allocation"); return DB_STORAGE_ERROR; }
        if (bytes) memcpy(binding->data, value.data, bytes);
    }
    binding->length = value.type == CONTAINER_VALUE_NULL ? SQL_NULL_DATA : (ssize_t)bytes;
    if (!SQL_SUCCEEDED(sqlConnStmtBindParam(statement->handle, index, SQL_PARAM_INPUT, ctype, sqltype,
            ctype == SQL_C_SBIGINT ? 19 : ctype == SQL_C_DOUBLE ? 15 : ctype == SQL_C_WCHAR ? bytes / 2 : bytes, 0, binding->data, bytes, &binding->length))) {
        odbcError(error, statement->handle, "bind"); return DB_STORAGE_ERROR;
    }
    return DB_STORAGE_OK;
}

static ContainerValue odbcColumn(DbStorageStatement *opaque, int index)
{
    OdbcStatement *statement = (OdbcStatement *)opaque;
    ContainerValue empty = {CONTAINER_VALUE_NULL, NULL, 0};
    OdbcColumn *column;
    SQLSMALLINT type, decimals, nullable;
    SQLULEN size;
    SQLCHAR name[128];
    SQLSMALLINT nameLength;
    ssize_t length;
    void *data;
    int ctype;
    if (index < 0 || index >= statement->columnCount) return empty;
    column = &statement->columns[index];
    if (column->loaded) return column->value;
    column->loaded = true;
    if (!SQL_SUCCEEDED(SQLDescribeColA(statement->handle, index + 1, name, sizeof(name), &nameLength, &type, &size, &decimals, &nullable))) return empty;
    if (type == SQL_TINYINT || type == SQL_SMALLINT || type == SQL_INTEGER || type == SQL_BIGINT || type == SQL_BIT) {
        ctype = SQL_C_SBIGINT; column->value.type = CONTAINER_VALUE_INT;
    } else if (type == SQL_REAL || type == SQL_FLOAT || type == SQL_DOUBLE || type == SQL_DECIMAL || type == SQL_NUMERIC) {
        ctype = SQL_C_DOUBLE; column->value.type = CONTAINER_VALUE_FLOAT;
    } else if (type == SQL_BINARY || type == SQL_VARBINARY || type == SQL_LONGVARBINARY) {
        ctype = SQL_C_BINARY; column->value.type = CONTAINER_VALUE_BLOB;
    } else {
        ctype = SQL_C_WCHAR; column->value.type = CONTAINER_VALUE_TEXT;
    }
    data = sqlConnStmtGetData(statement->handle, index + 1, ctype, &length, "storage column", statement->connection);
    if (!data || length == SQL_NULL_DATA || length < 0) { column->value = empty; return empty; }
    if (ctype == SQL_C_WCHAR) {
        int bytes = WideCharToMultiByte(CP_UTF8, 0, data, (int)(length / sizeof(wchar_t)), NULL, 0, NULL, NULL);
        column->data = calloc(1, bytes + 1);
        if (column->data && bytes) WideCharToMultiByte(CP_UTF8, 0, data, (int)(length / sizeof(wchar_t)), column->data, bytes, NULL, NULL);
        column->value.size = bytes;
    } else {
        column->data = calloc(1, length + 1);
        if (column->data && length) memcpy(column->data, data, length);
        column->value.size = length;
    }
    column->value.data = column->data;
    return column->data ? column->value : empty;
}

static DbStorageResult odbcNative(DbStorageConnection *opaque, const char *sql, DbStorageError *error)
{
    OdbcConnection *connection = (OdbcConnection *)opaque;
    if (SQL_SUCCEEDED(sqlConnExecDirectMany((char *)sql, (int)strlen(sql), connection->index, true))) return DB_STORAGE_OK;
    odbcError(error, NULL, "native SQL"); return DB_STORAGE_ERROR;
}

static const DbStorageOps odbcOps = {odbcDestroy, odbcConnection, odbcPrepare, odbcExecute, odbcStep,
    odbcReset, odbcFinalize, odbcTransaction, odbcCapabilities, odbcBind, odbcColumn, odbcNative};

DbStorage *dbStorageCreateOdbc(void)
{
    OdbcStorage *storage = calloc(1, sizeof(*storage));
    unsigned i;
    if (!storage) return NULL;
    storage->base.ops = &odbcOps; storage->base.provider = gDatabaseProvider;
    for (i = 0; i < SQLCONN_MAX; ++i) { storage->connections[i].base.ops = &odbcOps; storage->connections[i].index = i; }
    return &storage->base;
}
