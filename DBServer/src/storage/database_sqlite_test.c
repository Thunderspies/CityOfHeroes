#include "database_sqlite_schema.h"
#include <utilitieslib/utils/wininclude.h>
#include <stdio.h>
#include <utilitieslib/components/EString.h>

#define CHECK(x) do { if (!(x)) { fprintf(stderr, "Line %d: %s (%s)\n", __LINE__, #x, error.message); return 1; } } while (0)

static S64 scalar(DbStorageConnection *connection, const char *sql)
{
    DbStorageError error = {0};
    DbStorageStatement *statement = connection->ops->prepare(connection, sql, strlen(sql), &error);
    S64 result = -1;
    if (statement && statement->ops->step(statement, &error) == DB_STORAGE_ROW) {
        ContainerValue value = dbStorageColumn(statement, 0);
        if (value.type == CONTAINER_VALUE_INT) result = *(const S64 *)value.data;
    }
    if (statement) statement->ops->finalize(statement);
    return result;
}

int main(void)
{
    char directory[MAX_PATH], path[MAX_PATH];
    DbStorageError error = {0};
    DbStorage *storage, *second;
    DbStorageConnection *connection, *other;
    DbStorageStatement *statement;
    ContainerValue value, first, secondValue;
    S64 integer = -5000000000LL;
    double real = 1.25;
    unsigned char blob[] = {0, 1, 255, 0, 7};
    char text[] = "dbo.O'Brien \xc3\xa9 \xf0\x9f\x98\x80";
    char *normalized;
    char *activePlayers = NULL;
    DWORD started, elapsed;
    ColumnInfo columns[4] = {0}, childColumns[3] = {0};
    TableInfo table = {0}, child = {0};
    DbStorageForeignKey key = {"ContainerId", "Parents", "ContainerId"};
    GetTempPathA(sizeof(directory), directory);
    CHECK(GetTempFileNameA(directory, "cox", 0, path));
    storage = dbStorageCreateSqlite(path, &error); CHECK(storage);
    connection = storage->ops->connection(storage, SQLCONN_FOREGROUND);
    CHECK(storage->ops->capabilities(storage).workerCount == 0);
    CHECK(scalar(connection, "PRAGMA foreign_keys") == 1);
    CHECK(scalar(connection, "PRAGMA synchronous") == 2);
    statement = connection->ops->prepare(connection, "PRAGMA journal_mode", strlen("PRAGMA journal_mode"), &error); CHECK(statement);
    CHECK(statement->ops->step(statement, &error) == DB_STORAGE_ROW);
    CHECK(!strcmp(dbStorageColumn(statement, 0).data, "wal"));
    statement->ops->finalize(statement);
    normalized = dbStorageNormalizeSqlite("SELECT ISNULL(dbo.T.Id,0), 'dbo.ISNULL(x)', \"dbo.Name\" -- dbo.T\n/* ISNULL dbo.T */");
    CHECK(normalized && !strcmp(normalized, "SELECT COALESCE(T.Id,0), 'dbo.ISNULL(x)', \"dbo.Name\" -- dbo.T\n/* ISNULL dbo.T */")); free(normalized);
    normalized = dbStorageNormalizeSqlite("SELECT N'O''Brien dbo.ISNULL(x)', n'caf\xc3\xa9', 'N''quoted', \"N'literal\", N_column -- N'comment'\n/* n'comment' */");
    CHECK(normalized && !strcmp(normalized, "SELECT 'O''Brien dbo.ISNULL(x)', 'caf\xc3\xa9', 'N''quoted', \"N'literal\", N_column -- N'comment'\n/* n'comment' */")); free(normalized);
    CHECK(!connection->ops->prepare(connection, "SELECT 1; SELECT 2", strlen("SELECT 1; SELECT 2"), &error));
    CHECK(!connection->ops->prepare(connection, "broken SQL", strlen("broken SQL"), &error));
    CHECK(!connection->ops->prepare(connection, "", strlen(""), &error));
    CHECK(dbStorageExecuteNative(connection,
        "CREATE TABLE AuctionPlayers(ContainerId INTEGER PRIMARY KEY, LastActive DTEXT);"
        "INSERT INTO AuctionPlayers VALUES(1,datetime('now','localtime')),"
        "(2,datetime('now','localtime','-61 days')),(3,datetime('now','localtime','-60 days')),(4,NULL)", &error) == DB_STORAGE_OK);
    estrPrintf(&activePlayers, "SELECT ContainerId FROM AuctionPlayers WHERE LastActive > ");
    dbStorageAppendDaysAgo(DBPROV_SQLITE, &activePlayers, 60);
    statement = connection->ops->prepare(connection, activePlayers, strlen(activePlayers), &error); CHECK(statement);
    CHECK(statement->ops->step(statement, &error) == DB_STORAGE_ROW);
    CHECK(*(const S64 *)dbStorageColumn(statement, 0).data == 1);
    CHECK(statement->ops->step(statement, &error) == DB_STORAGE_DONE);
    statement->ops->finalize(statement);
    estrDestroy(&activePlayers);
    CHECK(dbStorageExecuteNative(connection, "BEGIN", &error) == DB_STORAGE_ERROR);
    CHECK(dbStorageExecuteNative(connection, "CREATE TABLE ValuesTest(i INTEGER,r REAL,t TEXT,b BLOB,n TEXT,e TEXT,z BLOB)", &error) == DB_STORAGE_OK);
    statement = connection->ops->prepare(connection, "INSERT INTO ValuesTest VALUES(?,?,?,?,?,?,?)", strlen("INSERT INTO ValuesTest VALUES(?,?,?,?,?,?,?)"), &error); CHECK(statement);
    value.type = CONTAINER_VALUE_INT; value.data = &integer; value.size = sizeof(integer);
    CHECK(dbStorageBind(statement, 1, value, &error) == DB_STORAGE_OK);
    value.type = CONTAINER_VALUE_FLOAT; value.data = &real; value.size = sizeof(real);
    CHECK(dbStorageBind(statement, 2, value, &error) == DB_STORAGE_OK);
    value.type = CONTAINER_VALUE_TEXT; value.data = text; value.size = strlen(text);
    CHECK(dbStorageBind(statement, 3, value, &error) == DB_STORAGE_OK);
    value.type = CONTAINER_VALUE_BLOB; value.data = blob; value.size = sizeof(blob);
    CHECK(dbStorageBind(statement, 4, value, &error) == DB_STORAGE_OK);
    value.type = CONTAINER_VALUE_NULL; value.data = NULL; value.size = 0;
    CHECK(dbStorageBind(statement, 5, value, &error) == DB_STORAGE_OK);
    value.type = CONTAINER_VALUE_TEXT;
    CHECK(dbStorageBind(statement, 6, value, &error) == DB_STORAGE_OK);
    value.type = CONTAINER_VALUE_BLOB;
    CHECK(dbStorageBind(statement, 7, value, &error) == DB_STORAGE_OK);
    blob[1] = 9; /* Bind ownership must survive caller changes. */
    CHECK(statement->ops->execute(statement, &error) == DB_STORAGE_DONE);
    statement->ops->finalize(statement);
    storage->ops->destroy(storage);
    storage = dbStorageCreateSqlite(path, &error); CHECK(storage);
    connection = storage->ops->connection(storage, 0);
    statement = connection->ops->prepare(connection, "SELECT * FROM ValuesTest", strlen("SELECT * FROM ValuesTest"), &error); CHECK(statement);
    CHECK(statement->ops->step(statement, &error) == DB_STORAGE_ROW);
    first = dbStorageColumn(statement, 0); secondValue = dbStorageColumn(statement, 1);
    CHECK(*(const S64 *)first.data == integer && *(const double *)secondValue.data == real);
    CHECK(!strcmp(dbStorageColumn(statement, 2).data, text));
    blob[1] = 1; value = dbStorageColumn(statement, 3);
    CHECK(value.type == CONTAINER_VALUE_BLOB && value.size == sizeof(blob) && !memcmp(value.data, blob, sizeof(blob)));
    CHECK(dbStorageColumn(statement, 4).type == CONTAINER_VALUE_NULL);
    CHECK(dbStorageColumn(statement, 5).type == CONTAINER_VALUE_TEXT && !dbStorageColumn(statement, 5).size);
    CHECK(dbStorageColumn(statement, 6).type == CONTAINER_VALUE_BLOB && !dbStorageColumn(statement, 6).size);
    CHECK(statement->ops->step(statement, &error) == DB_STORAGE_DONE);
    statement->ops->reset(statement);
    CHECK(statement->ops->step(statement, &error) == DB_STORAGE_ROW);
    statement->ops->finalize(statement);
    strcpy(table.name, "Parents"); table.table_type = TT_CONTAINER; table.columns = columns; table.num_columns = 3;
    strcpy(columns[0].name, "ContainerId"); strcpy(columns[0].data_type_name, "INTEGER"); columns[0].reserved_word = 1;
    strcpy(columns[1].name, "Active"); strcpy(columns[1].data_type_name, "INTEGER"); columns[1].reserved_word = 1;
    strcpy(columns[2].name, "Name"); strcpy(columns[2].data_type_name, "UTEXT(64)");
    CHECK(dbStorageSqliteEnsureTable(connection, &table, NULL, 0, &error));
    strcpy(child.name, "Children"); child.table_type = TT_SUBCONTAINER; child.columns = childColumns; child.num_columns = 3;
    childColumns[0] = columns[0]; childColumns[1] = columns[1];
    strcpy(childColumns[1].name, "SubId"); childColumns[1].is_sub_id_field = 1;
    strcpy(childColumns[2].name, "Value"); strcpy(childColumns[2].data_type_name, "INTEGER");
    CHECK(dbStorageSqliteEnsureTable(connection, &child, &key, 1, &error));
    key.target = "OtherId";
    CHECK(!dbStorageSqliteEnsureTable(connection, &child, &key, 1, &error));
    key.target = "ContainerId";
    CHECK(!dbStorageSqliteEnsureTable(connection, &child, NULL, 0, &error));
    CHECK(dbStorageSqliteEnsureTable(connection, &child, &key, 1, &error));
    CHECK(connection->ops->transaction(connection, true, false, &error) == DB_STORAGE_OK);
    CHECK(dbStorageExecuteNative(connection, "INSERT INTO Children VALUES(41,0,7); INSERT INTO Parents(ContainerId,Name) VALUES(41,'Persisted')", &error) == DB_STORAGE_OK);
    CHECK(connection->ops->transaction(connection, false, true, &error) == DB_STORAGE_OK);
    CHECK(connection->ops->transaction(connection, true, false, &error) == DB_STORAGE_OK);
    CHECK(dbStorageExecuteNative(connection, "INSERT INTO Parents(ContainerId) VALUES(42)", &error) == DB_STORAGE_OK);
    CHECK(dbStorageExecuteNative(connection, "INSERT INTO Children VALUES(41,0,8)", &error) == DB_STORAGE_ERROR);
    CHECK(connection->ops->transaction(connection, false, false, &error) == DB_STORAGE_OK);
    CHECK(scalar(connection, "SELECT COUNT(*) FROM Parents") == 1);
    CHECK(connection->ops->transaction(connection, true, false, &error) == DB_STORAGE_OK);
    CHECK(dbStorageExecuteNative(connection, "INSERT INTO Children VALUES(99,0,8)", &error) == DB_STORAGE_OK);
    CHECK(connection->ops->transaction(connection, false, true, &error) == DB_STORAGE_ERROR);
    CHECK(connection->ops->transaction(connection, false, false, &error) == DB_STORAGE_OK);
    CHECK(scalar(connection, "SELECT COUNT(*) FROM Children") == 1);
    CHECK(connection->ops->transaction(connection, true, false, &error) == DB_STORAGE_OK);
    CHECK(connection->ops->transaction(connection, true, false, &error) == DB_STORAGE_OK);
    CHECK(dbStorageExecuteNative(connection, "INSERT INTO Parents(ContainerId) VALUES(43)", &error) == DB_STORAGE_OK);
    CHECK(connection->ops->transaction(connection, false, false, &error) == DB_STORAGE_OK);
    CHECK(connection->ops->transaction(connection, false, true, &error) == DB_STORAGE_OK);
    CHECK(scalar(connection, "SELECT COUNT(*) FROM Parents") == 1);
    strcpy(columns[3].name, "Added"); strcpy(columns[3].data_type_name, "INT1"); table.num_columns = 4;
    CHECK(dbStorageSqliteEnsureTable(connection, &table, NULL, 0, &error));
    CHECK(dbStorageSqliteEnsureTable(connection, &child, &key, 1, &error));
    table.num_columns = 3; CHECK(!dbStorageSqliteEnsureTable(connection, &table, NULL, 0, &error));
    CHECK(strstr(error.message, "reset the disposable database"));
    table.num_columns = 4; strcpy(columns[2].data_type_name, "TEXT(64)");
    CHECK(!dbStorageSqliteEnsureTable(connection, &table, NULL, 0, &error));
    strcpy(columns[2].data_type_name, "UTEXT(64)"); strcpy(columns[2].name, "Renamed");
    CHECK(!dbStorageSqliteEnsureTable(connection, &table, NULL, 0, &error));
    strcpy(columns[2].name, "Name"); CHECK(dbStorageSqliteEnsureTable(connection, &table, NULL, 0, &error));
    second = dbStorageCreateSqlite(path, &error); CHECK(second); other = second->ops->connection(second, 0);
    CHECK(connection->ops->transaction(connection, true, false, &error) == DB_STORAGE_OK);
    CHECK(scalar(other, "SELECT COUNT(*) FROM Parents") == 1); /* WAL readers. */
    started = GetTickCount();
    CHECK(dbStorageExecuteNative(other, "INSERT INTO Parents(ContainerId) VALUES(55)", &error) == DB_STORAGE_RETRY);
    elapsed = GetTickCount() - started;
    CHECK(error.retryable && elapsed >= 4000 && elapsed < 10000);
    CHECK(connection->ops->transaction(connection, false, false, &error) == DB_STORAGE_OK);
    CHECK(dbStorageExecuteNative(other, "INSERT INTO Parents(ContainerId) VALUES(55)", &error) == DB_STORAGE_OK);
    second->ops->destroy(second);
    CHECK(connection->ops->transaction(connection, true, false, &error) == DB_STORAGE_OK);
    CHECK(dbStorageExecuteNative(connection, "DELETE FROM Children WHERE ContainerId=41; DELETE FROM Parents WHERE ContainerId IN(41,55)", &error) == DB_STORAGE_OK);
    CHECK(connection->ops->transaction(connection, false, true, &error) == DB_STORAGE_OK);
    storage->ops->destroy(storage);
    storage = dbStorageCreateSqlite(path, &error); CHECK(storage); connection = storage->ops->connection(storage, 0);
    CHECK(scalar(connection, "SELECT seq FROM sqlite_sequence WHERE name='Parents'") == 55);
    CHECK(scalar(connection, "SELECT COUNT(*) FROM Children") == 0);
    CHECK(dbStorageExecuteNative(connection, "DBCC CHECKIDENT('Parents')", &error) == DB_STORAGE_ERROR);
    storage->ops->destroy(storage);
    CHECK(DeleteFileA(path));
    puts("SQLite storage contracts passed");
    return 0;
}
