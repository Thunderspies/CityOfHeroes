#include "database.h"
#include <utilitieslib/components/EString.h>
#include <stdio.h>

#define CHECK(x) do { if (!(x)) { fprintf(stderr, "Failed at line %d: %s\n", __LINE__, #x); return 1; } } while (0)

int main(void)
{
    char table[128];
    char *query = NULL;
    CHECK(!strcmp(dbStorageQualifiedTable(DBPROV_MSSQL, "Ents", table, sizeof(table)), "dbo.Ents"));
    CHECK(!strcmp(dbStorageQualifiedTable(DBPROV_POSTGRESQL, "Ents", table, sizeof(table)), "dbo.Ents"));
    CHECK(!strcmp(dbStorageTypeName(DBPROV_MSSQL, CFTYPE_BYTE), "tinyint"));
    CHECK(!strcmp(dbStorageTypeName(DBPROV_POSTGRESQL, CFTYPE_BYTE), "int2"));
    CHECK(!strcmp(dbStorageQualifiedTable(DBPROV_SQLITE, "Ents", table, sizeof(table)), "Ents"));
    CHECK(!strcmp(dbStorageTypeName(DBPROV_SQLITE, CFTYPE_BYTE), "INT1"));
    CHECK(!strcmp(dbStorageTypeName(DBPROV_SQLITE, CFTYPE_SHORT), "INT2"));
    CHECK(!strcmp(dbStorageTypeName(DBPROV_SQLITE, CFTYPE_UNICODESTRING), "UTEXT"));
    CHECK(!strcmp(dbStorageCoalesceFunction(DBPROV_SQLITE), "COALESCE"));
    CHECK(dbStorageTypeName(DBPROV_MSSQL, CFTYPE_NULL) == NULL);
    CHECK(dbStorageTypeName(DBPROV_MSSQL, CFTYPE_COUNT) == NULL);
    CHECK(!strcmp(dbStorageCoalesceFunction(DBPROV_MSSQL), "ISNULL"));
    CHECK(!strcmp(dbStorageCoalesceFunction(DBPROV_POSTGRESQL), "COALESCE"));
    estrConcatCharString(&query, "SELECT ContainerId FROM Ents");
    dbStorageAppendLimit(DBPROV_MSSQL, &query, 5);
    CHECK(!strcmp(query, "SELECT ContainerId FROM Ents"));
    dbStorageAppendLimit(DBPROV_POSTGRESQL, &query, 5);
    CHECK(!strcmp(query, "SELECT ContainerId FROM Ents LIMIT 5"));
    estrDestroy(&query);
    dbStorageAppendDaysAgo(DBPROV_MSSQL, &query, 60);
    CHECK(!strcmp(query, "DATEADD(d, -60, GETDATE())"));
    estrDestroy(&query);
    dbStorageAppendDaysAgo(DBPROV_POSTGRESQL, &query, 60);
    CHECK(!strcmp(query, "(LOCALTIMESTAMP - INTERVAL '60 days')"));
    estrDestroy(&query);
    dbStorageAppendDaysAgo(DBPROV_SQLITE, &query, 0);
    CHECK(!strcmp(query, "datetime('now', 'localtime', '0 days')"));
    estrDestroy(&query);
    dbStorageAppendDaysAgo(DBPROV_SQLITE, &query, -7);
    CHECK(!strcmp(query, "datetime('now', 'localtime', '7 days')"));
    estrDestroy(&query);
    return 0;
}
