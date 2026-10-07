#include "database.h"

DbStorageResult dbStorageBind(DbStorageStatement *statement, int index, ContainerValue value, DbStorageError *error)
{
    return statement->ops->bind(statement, index, value, error);
}

ContainerValue dbStorageColumn(DbStorageStatement *statement, int index)
{
    return statement->ops->column(statement, index);
}

DbStorageResult dbStorageExecuteNative(DbStorageConnection *connection, const char *sql, DbStorageError *error)
{
    return connection->ops->executeNative(connection, sql, error);
}
