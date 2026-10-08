#ifndef DBSERVER_SQLITE_SCHEMA_H
#define DBSERVER_SQLITE_SCHEMA_H
#include "database.h"
#include "container_sql.h"

typedef struct DbStorageForeignKey { const char *column, *table, *target; } DbStorageForeignKey;
/* Existing columns and keys must match. Only trailing nullable columns and
 * new tables are added; incompatible changes leave the table intact. */
bool dbStorageSqliteEnsureTable(DbStorageConnection *connection, TableInfo *table,
    const DbStorageForeignKey *keys, int keyCount, DbStorageError *error);
#endif
