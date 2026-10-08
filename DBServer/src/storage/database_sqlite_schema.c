#include "database_sqlite_schema.h"
#include <utilitieslib/components/EString.h>

static bool incompatible(TableInfo *table, DbStorageError *error)
{
    if (error) {
        memset(error, 0, sizeof(*error));
        snprintf(error->message, sizeof(error->message),
            "Incompatible SQLite schema for %s. Stop the local servers and reset the disposable database (including -wal/-shm files).", table->name);
    }
    return false;
}

static void appendColumn(char **sql, TableInfo *table, int index, const DbStorageForeignKey *keys, int keyCount, bool creating)
{
    ColumnInfo *column = &table->columns[index];
    int k;
    estrConcatf(sql, "\"%s\" %s", column->name, column->data_type_name);
    if (column->reserved_word && !stricmp(column->name, "ContainerId")) {
        if (table->table_type == TT_CONTAINER) estrConcatCharString(sql, " PRIMARY KEY AUTOINCREMENT");
        else estrConcatCharString(sql, " NOT NULL");
    } else if (column->reserved_word && !stricmp(column->name, "Id")) estrConcatCharString(sql, " PRIMARY KEY");
    else if (column->is_sub_id_field) estrConcatCharString(sql, " NOT NULL");
    /* SQLite can add a nullable REFERENCES column without rebuilding a table. */
    if (!creating) for (k = 0; k < keyCount; ++k) if (!stricmp(keys[k].column, column->name))
        estrConcatf(sql, " REFERENCES \"%s\"(\"%s\") DEFERRABLE INITIALLY DEFERRED", keys[k].table, keys[k].target);
}

bool dbStorageSqliteEnsureTable(DbStorageConnection *connection, TableInfo *table,
    const DbStorageForeignKey *keys, int keyCount, DbStorageError *error)
{
    DbStorageStatement *statement;
    DbStorageResult result;
    int existing = 0, i;
    char *sql = NULL;
    bool *matched = calloc(keyCount ? keyCount : 1, sizeof(bool));
    if (!matched) {
        if (error) snprintf(error->message, sizeof(error->message), "SQLite schema allocation failed");
        return false;
    }
    estrPrintf(&sql, "PRAGMA table_info(\"%s\")", table->name);
    statement = connection->ops->prepare(connection, sql, estrLength(&sql), error);
    if (!statement) goto failure;
    while ((result = connection->ops->step(statement, error)) == DB_STORAGE_ROW) {
        ContainerValue name = dbStorageColumn(statement, 1), type = dbStorageColumn(statement, 2), primary = dbStorageColumn(statement, 5);
        int pk = primary.type == CONTAINER_VALUE_INT ? (int)*(const S64 *)primary.data : 0;
        int expectedPk = existing == 0 ? 1 : table->table_type == TT_SUBCONTAINER && existing == 1 ? 2 : 0;
        if (existing >= table->num_columns || stricmp(name.data, table->columns[existing].name) ||
            stricmp(type.data, table->columns[existing].data_type_name) || pk != expectedPk) {
            connection->ops->finalize(statement); incompatible(table, error); goto failure;
        }
        ++existing;
    }
    connection->ops->finalize(statement);
    if (result != DB_STORAGE_DONE) goto failure;
    if (existing) {
        estrPrintf(&sql, "PRAGMA foreign_key_list(\"%s\")", table->name);
        statement = connection->ops->prepare(connection, sql, estrLength(&sql), error);
        if (!statement) goto failure;
        while ((result = connection->ops->step(statement, error)) == DB_STORAGE_ROW) {
            ContainerValue targetTable = dbStorageColumn(statement, 2), column = dbStorageColumn(statement, 3), target = dbStorageColumn(statement, 4);
            for (i = 0; i < keyCount; ++i) if (!matched[i] && !stricmp(keys[i].table, targetTable.data) &&
                !stricmp(keys[i].column, column.data) && !stricmp(keys[i].target, target.data)) break;
            if (i == keyCount) { connection->ops->finalize(statement); incompatible(table, error); goto failure; }
            matched[i] = true;
        }
        connection->ops->finalize(statement);
        if (result != DB_STORAGE_DONE) goto failure;
        for (i = 0; i < keyCount; ++i) if (!matched[i]) {
            int col;
            for (col = existing; col < table->num_columns; ++col) if (!stricmp(table->columns[col].name, keys[i].column)) break;
            if (col == table->num_columns) { incompatible(table, error); goto failure; }
        }
    }
    if (connection->ops->transaction(connection, true, false, error) != DB_STORAGE_OK) goto failure;
    if (!existing) {
        estrPrintf(&sql, "CREATE TABLE \"%s\" (", table->name);
        for (i = 0; i < table->num_columns; ++i) {
            if (i) estrConcatChar(&sql, ',');
            appendColumn(&sql, table, i, keys, keyCount, true);
        }
        if (table->table_type == TT_SUBCONTAINER) estrConcatCharString(&sql, ", PRIMARY KEY(ContainerId,SubId)");
        for (i = 0; i < keyCount; ++i) estrConcatf(&sql,
            ", FOREIGN KEY(\"%s\") REFERENCES \"%s\"(\"%s\") DEFERRABLE INITIALLY DEFERRED", keys[i].column, keys[i].table, keys[i].target);
        estrConcatChar(&sql, ')');
        if (dbStorageExecuteNative(connection, sql, error) != DB_STORAGE_OK) goto rollback;
    } else for (i = existing; i < table->num_columns; ++i) {
        estrPrintf(&sql, "ALTER TABLE \"%s\" ADD COLUMN ", table->name);
        appendColumn(&sql, table, i, keys, keyCount, false);
        if (dbStorageExecuteNative(connection, sql, error) != DB_STORAGE_OK) goto rollback;
    }
    if (connection->ops->transaction(connection, false, true, error) != DB_STORAGE_OK) goto rollback;
    free(matched); estrDestroy(&sql); return true;
rollback:
    connection->ops->transaction(connection, false, false, NULL);
failure:
    free(matched); estrDestroy(&sql); return false;
}
