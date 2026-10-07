#include "database.h"
#include <utilitieslib/components/EString.h>
#include <stdio.h>

const char *dbStorageQualifiedTable(DatabaseProvider provider, const char *table, char *buffer, size_t bufferSize)
{
    _snprintf(buffer, bufferSize, provider == DBPROV_SQLITE ? "%s" : "dbo.%s", table);
    return buffer;
}

const char *dbStorageTypeName(DatabaseProvider provider, ContainerFieldType type)
{
    static const char *mssql[CFTYPE_COUNT] = {NULL, "tinyint", "smallint", "int", "real", "nvarchar", "varchar", "datetime", "varbinary(max)", "nvarchar(max)", "varchar(max)", "text", "image"};
    static const char *postgresql[CFTYPE_COUNT] = {NULL, "int2", "int2", "int4", "float4", "varchar", "varchar", "timestamp", "bytea", "text", "text", "text", "bytea"};
    static const char *sqlite[CFTYPE_COUNT] = {NULL, "INT1", "INT2", "INTEGER", "REAL", "UTEXT", "TEXT", "DTEXT", "BLOB", "UTEXT", "TEXT", "TEXT", "BLOB"};
    if (type <= CFTYPE_NULL || type >= CFTYPE_COUNT) return NULL;
    if (provider == DBPROV_SQLITE) return sqlite[type];
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

void dbStorageAppendDaysAgo(DatabaseProvider provider, char **sql, int days)
{
    if (provider == DBPROV_SQLITE)
        estrConcatf(sql, "datetime('now', 'localtime', '%lld days')", -(S64)days);
    else if (provider == DBPROV_POSTGRESQL)
        estrConcatf(sql, "(LOCALTIMESTAMP - INTERVAL '%d days')", days);
    else
        estrConcatf(sql, "DATEADD(d, %lld, GETDATE())", -(S64)days);
}
