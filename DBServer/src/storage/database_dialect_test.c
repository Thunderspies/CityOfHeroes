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
    return 0;
}
