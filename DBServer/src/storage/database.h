#ifndef DBSERVER_STORAGE_DATABASE_H
#define DBSERVER_STORAGE_DATABASE_H

#include <utilitieslib/stdtypes.h>
#include "container_tplt_utils.h"
#include "container/container_query.h"
#include "sql/sqlconn.h"

C_DECLARATIONS_BEGIN

typedef struct DbStorage DbStorage;
typedef struct DbStorageConnection DbStorageConnection;
typedef struct DbStorageStatement DbStorageStatement;

/* Prepare/bind errors return ERROR; step returns ROW or DONE. Execute may return
 * OK for an ODBC result cursor or ROW for SQLite's first result row. Native execution
 * and transactions return OK. RETRY means a bounded lock wait expired. */
typedef enum DbStorageResult {
    DB_STORAGE_OK, DB_STORAGE_ROW, DB_STORAGE_DONE, DB_STORAGE_RETRY, DB_STORAGE_ERROR
} DbStorageResult;

typedef struct DbStorageError {
    int nativeCode;
    char state[8];
    char message[512];
    bool retryable;
} DbStorageError;

typedef struct DbStorageCapabilities {
    unsigned workerCount;
    unsigned maxParameters;
    bool asyncTransactions, alterColumn, dropColumn, schemas;
} DbStorageCapabilities;

/* Connections belong to storage. Statements belong to the caller and must be
 * finalized before destroy. Bind indices are one-based, columns zero-based.
 * Binding copies input data. Column views remain valid until step/reset/finalize.
 * A connection and its statements must be used by one execution context at a
 * time. Reset closes the cursor and permits reexecution with retained binds. */
typedef struct DbStorageOps {
    void (*destroy)(DbStorage *storage);
    DbStorageConnection *(*connection)(DbStorage *storage, SqlConn index);
    DbStorageStatement *(*prepare)(DbStorageConnection *connection, const char *sql, size_t length, DbStorageError *error);
    DbStorageResult (*execute)(DbStorageStatement *statement, DbStorageError *error);
    DbStorageResult (*step)(DbStorageStatement *statement, DbStorageError *error);
    void (*reset)(DbStorageStatement *statement);
    void (*finalize)(DbStorageStatement *statement);
    DbStorageResult (*transaction)(DbStorageConnection *connection, bool begin, bool commit, DbStorageError *error);
    DbStorageCapabilities (*capabilities)(DbStorage *storage);
    DbStorageResult (*bind)(DbStorageStatement *statement, int index, ContainerValue value, DbStorageError *error);
    ContainerValue (*column)(DbStorageStatement *statement, int index);
    DbStorageResult (*executeNative)(DbStorageConnection *connection, const char *sql, DbStorageError *error);
} DbStorageOps;

struct DbStorage { const DbStorageOps *ops; DatabaseProvider provider; };
/* Adapter bases; consumers use only the opaque pointers and operations. */
struct DbStorageConnection { const DbStorageOps *ops; };
struct DbStorageStatement { const DbStorageOps *ops; };

/* The ODBC adapter borrows existing sqlConn connections; sqlConn owns shutdown. */
DbStorage *dbStorageCreateOdbc(void);
/* One file-backed connection, serialized by DBServer's main-thread FIFO. */
DbStorage *dbStorageCreateSqlite(const char *path, DbStorageError *error);
int dbStorageSqliteVersion(void);
char *dbStorageNormalizeSqlite(const char *sql);
DbStorageResult dbStorageBind(DbStorageStatement *statement, int index, ContainerValue value, DbStorageError *error);
ContainerValue dbStorageColumn(DbStorageStatement *statement, int index);
/* Native execution does not implicitly create a transaction. */
DbStorageResult dbStorageExecuteNative(DbStorageConnection *connection, const char *sql, DbStorageError *error);
const char *dbStorageQualifiedTable(DatabaseProvider provider, const char *table, char *buffer, size_t bufferSize);
const char *dbStorageTypeName(DatabaseProvider provider, ContainerFieldType type);
void dbStorageAppendLimit(DatabaseProvider provider, char **sql, int limit);
const char *dbStorageCoalesceFunction(DatabaseProvider provider);
/* Append an expression for the database's current local timestamp minus days.
 * The caller owns the EString; signed days are passed through to the provider. */
void dbStorageAppendDaysAgo(DatabaseProvider provider, char **sql, int days);

C_DECLARATIONS_END
#endif
