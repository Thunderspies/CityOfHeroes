#include "database.h"
#include "sqlite3.h"
#include <ctype.h>
#include <limits.h>

typedef struct SqliteConnection {
    DbStorageConnection base;
    sqlite3 *handle;
    unsigned transactionDepth;
    bool transactionControl;
} SqliteConnection;

static int sqliteAuthorize(void *context, int action, const char *a, const char *b, const char *database, const char *trigger)
{
    SqliteConnection *connection = context;
    (void)a; (void)b; (void)database; (void)trigger;
    if ((action == SQLITE_TRANSACTION || action == SQLITE_SAVEPOINT) && !connection->transactionControl) return SQLITE_DENY;
    return SQLITE_OK;
}
typedef struct SqliteStorage { DbStorage base; SqliteConnection connection; } SqliteStorage;
typedef struct SqliteStatement {
    DbStorageStatement base;
    sqlite3_stmt *handle;
    S64 *integers;
    double *floats;
} SqliteStatement;

static void sqliteError(DbStorageError *error, sqlite3 *database, int result)
{
    if (!error) return;
    memset(error, 0, sizeof(*error));
    error->nativeCode = result;
    error->retryable = (result & 0xff) == SQLITE_BUSY;
    snprintf(error->message, sizeof(error->message), "%s", database ? sqlite3_errmsg(database) : sqlite3_errstr(result));
}

/* Normalize only identifier tokens. Quoted strings/identifiers and comments are
 * copied intact so user data such as 'dbo.Name' cannot be changed by execution. */
char *dbStorageNormalizeSqlite(const char *sql)
{
    size_t length = strlen(sql), i = 0, out = 0;
    char *result = malloc(length * 2 + 1);
    if (!result) return NULL;
    while (i < length) {
        size_t start = i;
        char quote = sql[i];
        if (quote == '\'' || quote == '"' || quote == '`' || quote == '[') {
            char end = quote == '[' ? ']' : quote;
            ++i;
            while (i < length) {
                if (sql[i++] == end) {
                    if (i < length && sql[i] == end) ++i;
                    else break;
                }
            }
        } else if (sql[i] == '-' && sql[i + 1] == '-') {
            while (i < length && sql[i] != '\n') ++i;
        } else if (sql[i] == '/' && sql[i + 1] == '*') {
            i += 2;
            while (i < length && !(sql[i - 1] == '*' && sql[i] == '/')) ++i;
            if (i < length) ++i;
        } else if (isalpha((unsigned char)sql[i]) || sql[i] == '_') {
            size_t next;
            while (isalnum((unsigned char)sql[i]) || sql[i] == '_') ++i;
            next = i;
            while (isspace((unsigned char)sql[next])) ++next;
            /* SQL Server's Unicode literal introducer is redundant for UTF-8 SQLite text. */
            if (i - start == 1 && (sql[start] == 'N' || sql[start] == 'n') && sql[i] == '\'')
                continue;
            if (i - start == 3 && !_strnicmp(sql + start, "dbo", 3) && sql[next] == '.') {
                i = next + 1; continue;
            }
            if (i - start == 6 && !_strnicmp(sql + start, "isnull", 6) && sql[next] == '(') {
                memcpy(result + out, "COALESCE", 8); out += 8; continue;
            }
        } else ++i;
        memcpy(result + out, sql + start, i - start); out += i - start;
    }
    result[out] = 0;
    return result;
}

static void sqliteDestroy(DbStorage *opaque)
{
    SqliteStorage *storage = (SqliteStorage *)opaque;
    sqlite3_close(storage->connection.handle);
    free(storage);
}

static DbStorageConnection *sqliteConnection(DbStorage *storage, SqlConn index)
{
    (void)index;
    return &((SqliteStorage *)storage)->connection.base;
}

static DbStorageStatement *sqlitePrepare(DbStorageConnection *opaque, const char *sql, size_t length, DbStorageError *error)
{
    SqliteConnection *connection = (SqliteConnection *)opaque;
    SqliteStatement *statement;
    const char *tail = NULL;
    char *input, *normalized;
    int result, columns;
    if (!sql || length > INT_MAX) { sqliteError(error, NULL, SQLITE_MISUSE); return NULL; }
    input = malloc(length + 1);
    if (!input) { sqliteError(error, NULL, SQLITE_NOMEM); return NULL; }
    memcpy(input, sql, length); input[length] = 0;
    normalized = dbStorageNormalizeSqlite(input); free(input);
    if (!normalized) { sqliteError(error, NULL, SQLITE_NOMEM); return NULL; }
    statement = calloc(1, sizeof(*statement));
    if (!statement) { free(normalized); sqliteError(error, NULL, SQLITE_NOMEM); return NULL; }
    statement->base.ops = opaque->ops;
    result = sqlite3_prepare_v2(connection->handle, normalized, -1, &statement->handle, &tail);
    if (result == SQLITE_OK) while (isspace((unsigned char)*tail) || *tail == ';') ++tail;
    if (result != SQLITE_OK || !statement->handle || (tail && *tail)) {
        sqliteError(error, connection->handle, result == SQLITE_OK ? SQLITE_MISUSE : result);
        if (result == SQLITE_OK && error) snprintf(error->message, sizeof(error->message), "Expected one prepared SQLite statement");
        sqlite3_finalize(statement->handle); free(statement); free(normalized); return NULL;
    }
    free(normalized);
    columns = sqlite3_column_count(statement->handle);
    if (columns) {
        statement->integers = calloc(columns, sizeof(*statement->integers));
        statement->floats = calloc(columns, sizeof(*statement->floats));
        if (!statement->integers || !statement->floats) {
            sqlite3_finalize(statement->handle); free(statement->integers); free(statement->floats); free(statement);
            sqliteError(error, NULL, SQLITE_NOMEM); return NULL;
        }
    }
    return &statement->base;
}

static DbStorageResult sqliteStep(DbStorageStatement *opaque, DbStorageError *error)
{
    SqliteStatement *statement = (SqliteStatement *)opaque;
    int result = sqlite3_step(statement->handle);
    if (result == SQLITE_ROW) return DB_STORAGE_ROW;
    if (result == SQLITE_DONE) return DB_STORAGE_DONE;
    sqliteError(error, sqlite3_db_handle(statement->handle), result);
    return (result & 0xff) == SQLITE_BUSY ? DB_STORAGE_RETRY : DB_STORAGE_ERROR;
}

static void sqliteReset(DbStorageStatement *statement) { sqlite3_reset(((SqliteStatement *)statement)->handle); }
static void sqliteFinalize(DbStorageStatement *opaque)
{
    SqliteStatement *statement = (SqliteStatement *)opaque;
    sqlite3_finalize(statement->handle); free(statement->integers); free(statement->floats); free(statement);
}

static DbStorageResult sqliteNative(DbStorageConnection *opaque, const char *sql, DbStorageError *error)
{
    SqliteConnection *connection = (SqliteConnection *)opaque;
    char *normalized = dbStorageNormalizeSqlite(sql);
    int result;
    if (!normalized) { sqliteError(error, NULL, SQLITE_NOMEM); return DB_STORAGE_ERROR; }
    result = sqlite3_exec(connection->handle, normalized, NULL, NULL, NULL);
    free(normalized);
    if (result == SQLITE_OK) return DB_STORAGE_OK;
    sqliteError(error, connection->handle, result);
    return (result & 0xff) == SQLITE_BUSY ? DB_STORAGE_RETRY : DB_STORAGE_ERROR;
}

static DbStorageResult sqliteTransaction(DbStorageConnection *opaque, bool begin, bool commit, DbStorageError *error)
{
    SqliteConnection *connection = (SqliteConnection *)opaque;
    char command[128];
    DbStorageResult result;
    unsigned depth = connection->transactionDepth;
    if (!begin && !depth) { sqliteError(error, NULL, SQLITE_MISUSE); return DB_STORAGE_ERROR; }
    if (!depth) strcpy(command, "BEGIN IMMEDIATE");
    else if (begin) sprintf(command, "SAVEPOINT cox_%u", depth);
    else if (depth == 1) strcpy(command, commit ? "COMMIT" : "ROLLBACK");
    else if (commit) sprintf(command, "RELEASE cox_%u", depth - 1);
    else sprintf(command, "ROLLBACK TO cox_%u; RELEASE cox_%u", depth - 1, depth - 1);
    connection->transactionControl = true;
    result = sqliteNative(opaque, command, error);
    connection->transactionControl = false;
    if (result == DB_STORAGE_OK) connection->transactionDepth = begin ? depth + 1 : depth - 1;
    return result;
}

static DbStorageCapabilities sqliteCapabilities(DbStorage *storage)
{
    DbStorageCapabilities capabilities = {0, 32766, false, false, false, false};
    (void)storage; return capabilities;
}

static DbStorageResult sqliteBind(DbStorageStatement *opaque, int index, ContainerValue value, DbStorageError *error)
{
    SqliteStatement *statement = (SqliteStatement *)opaque;
    int result = SQLITE_MISUSE;
    if (value.size > INT_MAX || (value.size && !value.data)) {
        sqliteError(error, NULL, SQLITE_MISUSE); return DB_STORAGE_ERROR;
    }
    switch (value.type) {
        case CONTAINER_VALUE_NULL: result = sqlite3_bind_null(statement->handle, index); break;
        case CONTAINER_VALUE_INT:
            if (value.data && value.size == sizeof(S64)) result = sqlite3_bind_int64(statement->handle, index, *(const S64 *)value.data);
            break;
        case CONTAINER_VALUE_FLOAT:
            if (value.data && value.size == sizeof(double)) result = sqlite3_bind_double(statement->handle, index, *(const double *)value.data);
            break;
        case CONTAINER_VALUE_TEXT:
        case CONTAINER_VALUE_DATETIME: result = sqlite3_bind_text(statement->handle, index, value.data ? value.data : "", (int)value.size, SQLITE_TRANSIENT); break;
        case CONTAINER_VALUE_BLOB: result = sqlite3_bind_blob(statement->handle, index, value.data ? value.data : "", (int)value.size, SQLITE_TRANSIENT); break;
    }
    if (result != SQLITE_OK) sqliteError(error, sqlite3_db_handle(statement->handle), result);
    return result == SQLITE_OK ? DB_STORAGE_OK : DB_STORAGE_ERROR;
}

static ContainerValue sqliteColumn(DbStorageStatement *opaque, int index)
{
    SqliteStatement *statement = (SqliteStatement *)opaque;
    ContainerValue value = {CONTAINER_VALUE_NULL, NULL, 0};
    int type;
    if (index < 0 || index >= sqlite3_column_count(statement->handle)) return value;
    type = sqlite3_column_type(statement->handle, index);
    if (type == SQLITE_INTEGER) {
        statement->integers[index] = sqlite3_column_int64(statement->handle, index);
        value.type = CONTAINER_VALUE_INT; value.data = &statement->integers[index]; value.size = sizeof(S64);
    } else if (type == SQLITE_FLOAT) {
        statement->floats[index] = sqlite3_column_double(statement->handle, index);
        value.type = CONTAINER_VALUE_FLOAT; value.data = &statement->floats[index]; value.size = sizeof(double);
    } else if (type == SQLITE_TEXT || type == SQLITE_BLOB) {
        value.type = type == SQLITE_TEXT ? CONTAINER_VALUE_TEXT : CONTAINER_VALUE_BLOB;
        value.data = type == SQLITE_TEXT ? (const void *)sqlite3_column_text(statement->handle, index) : sqlite3_column_blob(statement->handle, index);
        value.size = sqlite3_column_bytes(statement->handle, index);
    }
    return value;
}

static const DbStorageOps sqliteOps = {sqliteDestroy, sqliteConnection, sqlitePrepare, sqliteStep, sqliteStep,
    sqliteReset, sqliteFinalize, sqliteTransaction, sqliteCapabilities, sqliteBind, sqliteColumn, sqliteNative};

DbStorage *dbStorageCreateSqlite(const char *path, DbStorageError *error)
{
    SqliteStorage *storage;
    int result;
    if (!path || !*path || !strcmp(path, ":memory:")) { sqliteError(error, NULL, SQLITE_MISUSE); return NULL; }
    storage = calloc(1, sizeof(*storage));
    if (!storage) { sqliteError(error, NULL, SQLITE_NOMEM); return NULL; }
    storage->base.ops = &sqliteOps; storage->base.provider = DBPROV_SQLITE;
    storage->connection.base.ops = &sqliteOps;
    result = sqlite3_open_v2(path, &storage->connection.handle, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_FULLMUTEX, NULL);
    if (result != SQLITE_OK) { sqliteError(error, storage->connection.handle, result); sqliteDestroy(&storage->base); return NULL; }
    sqlite3_extended_result_codes(storage->connection.handle, 1);
    sqlite3_set_authorizer(storage->connection.handle, sqliteAuthorize, &storage->connection);
    sqlite3_busy_timeout(storage->connection.handle, 5000);
    if (sqliteNative(&storage->connection.base, "PRAGMA foreign_keys=ON; PRAGMA journal_mode=WAL; PRAGMA synchronous=FULL", error) != DB_STORAGE_OK) {
        sqliteDestroy(&storage->base); return NULL;
    }
    {
        sqlite3_stmt *check = NULL;
        bool wal;
        result = sqlite3_prepare_v2(storage->connection.handle, "PRAGMA journal_mode", -1, &check, NULL);
        wal = result == SQLITE_OK && sqlite3_step(check) == SQLITE_ROW &&
            !stricmp((const char *)sqlite3_column_text(check, 0), "wal");
        sqlite3_finalize(check);
        if (!wal) {
            if (error) { memset(error, 0, sizeof(*error)); snprintf(error->message, sizeof(error->message), "SQLite storage requires a file that supports WAL journaling"); }
            sqliteDestroy(&storage->base); return NULL;
        }
    }
    return &storage->base;
}

int dbStorageSqliteVersion(void) { return sqlite3_libversion_number(); }
