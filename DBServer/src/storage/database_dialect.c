#include "database.h"
#include <utilitieslib/components/EString.h>
#include <stdio.h>

const char *dbStorageQualifiedTable(DatabaseProvider provider, const char *table, char *buffer, size_t bufferSize)
{
    _snprintf(buffer, bufferSize, "dbo.%s", table);
    (void)provider;
    return buffer;
}

const char *dbStorageTypeName(DatabaseProvider provider, ContainerFieldType type)
{
    static const char *mssql[CFTYPE_COUNT] = {NULL, "tinyint", "smallint", "int", "real", "nvarchar", "varchar", "datetime", "varbinary(max)", "nvarchar(max)", "varchar(max)", "text", "image"};
    static const char *postgresql[CFTYPE_COUNT] = {NULL, "int2", "int2", "int4", "float4", "varchar", "varchar", "timestamp", "bytea", "text", "text", "text", "bytea"};
    if (type <= CFTYPE_NULL || type >= CFTYPE_COUNT) return NULL;
    return provider == DBPROV_POSTGRESQL ? postgresql[type] : mssql[type];
}

void dbStorageAppendLimit(DatabaseProvider provider, char **sql, int limit)
{
    if (limit > 0 && provider != DBPROV_MSSQL) estrConcatf(sql, " LIMIT %d", limit);
}

const char *dbStorageCoalesceFunction(DatabaseProvider provider)
{
    return provider == DBPROV_MSSQL ? "ISNULL" : "COALESCE";
}
