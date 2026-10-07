#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <utilitieslib/utils/wininclude.h>
#include "sql/sqlinclude.h"
#include <utilitieslib/utils/utils.h>
#include "container_sql.h"
#include "container_diff.h"
#include "container_merge.h"
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/utils/error.h>
#include <utilitieslib/assert/assert.h>
#include <utilitieslib/utils/Stackdump.h>
#include <utilitieslib/utils/strings_opt.h>
#include <utilitieslib/utils/mathutil.h>
#include "dbserver/servercfg.h"
#include "sql_fifo.h"
#include "dbinit.h"
#include "storage/database.h"
#include <utilitieslib/utils/ConvertUtf.h>
#include <utilitieslib/components/EString.h>
#include <utilitieslib/components/StashTable.h>
#include <utilitieslib/utils/StringUtil.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/utils/file.h>
#include <utilitieslib/utils/log.h>
#include <utilitieslib/components/HashFunctions.h>

#define strdupa(str) strcpy(_alloca(strlen(str)+1), str)
#define strlwrdupa(str) strlwr(strdupa(str))
#define struprdupa(str) strupr(strdupa(str))

/**
* Enable storing of data in SQL as binary, otherwise use an escaped string.
*
* @note Right now we store data as escaped string in a binary column in the
* tables.  In order to make the switch, the data would have to be unescaped in
* the database using a script before running with a build that has this
* enabled.
*/
#define ACTUALLY_STORE_BINARY_AS_BINARY 0

/** 
* A structure for storing information about each parameter bind
* that requires a temporary buffer until after the SQL statement
* has been closed.
*/ 
typedef struct sqlBindTemp {
    size_t bytes;
    union {
        char data[0];
        wchar_t wdata[0];
    };
} sqlBindTemp;

static INLINEDBG char getHexFromBin(int value)
{
    if (value < 10)
        return value + '0';
    return value - 10 + 'a';
}

static INLINEDBG char getBinFromHex(int value)
{
    if (value >= 'a' && value <= 'f')
        return 10 + value - 'a';
    if (value >= '0' && value <= '9')
        return value - '0';
    return 0;
}

static unsigned char* hexStrToBinStr2(char* hexString,U8 *binStr)
{
    int i, len;

    len = (int)strlen(hexString) / 2; // !this will let the last character fall off if we had an odd length string 
    for(i = 0; i < len; i++)
        binStr[i] = (getBinFromHex(hexString[i*2]) << 4) | getBinFromHex(hexString[i*2+1]);
    binStr[i] = 0;
    return binStr;
}

static char* binStrToHexStr2(unsigned char* binArray, char *hexStr)
{
    int i,len=-1;

    len = (int)strlen(binArray);

    for(i=len-1;i>=0;i--)
    {
        hexStr[i*2+0] = getHexFromBin(binArray[i] >> 4);
        hexStr[i*2+1] = getHexFromBin(binArray[i] & 15);
    }
    hexStr[len*2] = '\0';
    return hexStr;
}

bool sqlCreateTable(char *name, TableType table_type)
{
    char buf[SHORT_SQL_STRING];
    int buf_len;
    bool ret;

    // Verify that the table name has no namespace in it
    devassert(!strchr(name, '.'));

    switch(table_type) {
        xcase TT_ATTRIBUTE:
            buf_len = sprintf(buf, gDatabaseProvider == DBPROV_SQLITE ? "CREATE TABLE %s (Id INTEGER NOT NULL PRIMARY KEY);" : "CREATE TABLE dbo.%s (Id INTEGER NOT NULL PRIMARY KEY);", name);
        xcase TT_CONTAINER:
            switch (gDatabaseProvider) {
                xcase DBPROV_MSSQL:
                    buf_len = sprintf(buf, "CREATE TABLE dbo.%s (ContainerId INTEGER NOT NULL IDENTITY(1,1) PRIMARY KEY, Active INTEGER);", name);
                xcase DBPROV_POSTGRESQL:
                    buf_len = sprintf(buf, "CREATE TABLE dbo.%s (ContainerId SERIAL NOT NULL PRIMARY KEY, Active INTEGER);", name);
                xcase DBPROV_SQLITE:
                    buf_len = sprintf(buf, "CREATE TABLE %s (ContainerId INTEGER PRIMARY KEY AUTOINCREMENT, Active INTEGER);", name);
                DBPROV_XDEFAULT();
            }
        xcase TT_SUBCONTAINER:
            if (gDatabaseProvider == DBPROV_SQLITE)
                buf_len = sprintf(buf, "CREATE TABLE %s (ContainerId INTEGER NOT NULL, SubId INTEGER NOT NULL, PRIMARY KEY (ContainerId, SubId));", name);
            else
                buf_len = sprintf(buf, "CREATE TABLE dbo.%s (ContainerId INTEGER NOT NULL, SubId INTEGER NOT NULL);"
                            "ALTER TABLE dbo.%s ADD CONSTRAINT PK_%s PRIMARY KEY (ContainerId, SubId);",
                            name, name, name);
        xdefault:
            FatalErrorf("Cannot create unknown table type for %s", name);
    }

    ret = sqlExecDdl(DDL_ADD, buf, buf_len);

    if (ret && server_cfg.change_db_owner_from[0])
    {
        // requested by joe phillips
        sqlExecDdlf(DDL_ADD, "sp_changeobjectowner '%s.%s','dbo'", server_cfg.change_db_owner_from, name);
    }

    return ret;
}

bool sqlAddField(char *table, char *name, char *type)
{
    if (gDatabaseProvider == DBPROV_SQLITE)
        return sqlExecDdlf(DDL_ADD, "ALTER TABLE %s ADD %s %s;", table, name, type);
    return sqlExecDdlf(DDL_ADD, "ALTER TABLE dbo.%s ADD %s %s;", table, name, type);
}

bool sqlChangeFieldType(char *table, char *name, char *newtype)
{
    char buf[SHORT_SQL_STRING];
    int buf_len;

    switch (gDatabaseProvider) {
        xcase DBPROV_MSSQL:
            buf_len = sprintf(buf, "ALTER TABLE dbo.%s ALTER COLUMN %s %s;", table, name, newtype);
        xcase DBPROV_POSTGRESQL:
            buf_len = sprintf(buf, "ALTER TABLE dbo.%s ALTER COLUMN %s TYPE %s;", table, name, newtype);
        DBPROV_XDEFAULT();
    }

    return sqlExecDdl(DDL_ADD, buf, buf_len);
}

bool sqlDeleteField(char *table, char *name, char *type)
{
    return sqlExecDdlf(DDL_ADD, "ALTER TABLE dbo.%s DROP COLUMN %s;", table, name);
}

/** 
* Execute a semicolon seperated list SQL database commands if 
* the specified DDL command type is allowed by the server.
*/ 
bool sqlExecDdl(DdlType ddl_type, char *str, int str_len)
{
    sqlCheckDdl(ddl_type);
    if (gDatabaseProvider == DBPROV_SQLITE)
    {
        DbStorageError error = {0};
        DbStorage *storage = dbGetStorage();
        DbStorageResult result = dbStorageExecuteNative(storage->ops->connection(storage, SQLCONN_FOREGROUND), str, &error);
        if (result != DB_STORAGE_OK)
            Errorf("SQLite DDL failed: %s\nSQL: %s", error.message, str);
        return result == DB_STORAGE_OK;
    }
    return SQL_SUCCEEDED(sqlConnExecDirectMany(str, str_len, SQLCONN_FOREGROUND, true));
}

static const char *skipSqlWhitespace(const char *text)
{
    while (isspace((unsigned char)*text))
        ++text;
    return text;
}

static bool parseSqliteSelectModifier(const char *modifier, bool *distinct, int *limit)
{
    char *end;
    long parsedLimit;

    *distinct = false;
    *limit = 0;
    modifier = skipSqlWhitespace(modifier);
    if (!*modifier)
        return true;
    if (_strnicmp(modifier, "distinct", 8) == 0 && !*skipSqlWhitespace(modifier + 8))
    {
        *distinct = true;
        return true;
    }
    if (_strnicmp(modifier, "top", 3) == 0 && isspace((unsigned char)modifier[3]))
        modifier = skipSqlWhitespace(modifier + 3);
    else if (_strnicmp(modifier, "limit", 5) == 0 && isspace((unsigned char)modifier[5]))
        modifier = skipSqlWhitespace(modifier + 5);
    else
        return false;

    parsedLimit = strtol(modifier, &end, 10);
    if (end == modifier || parsedLimit <= 0 || parsedLimit > INT_MAX || *skipSqlWhitespace(end))
        return false;
    *limit = (int)parsedLimit;
    return true;
}

/** 
* Execute a semicolon seperated list SQL database commands if 
* the specified DDL command type is allowed by the server with 
* printf style formatting. 
*/
bool sqlExecDdlf(DdlType ddl_type, char *fmt, ...)
{
    bool ret;
    char *buf = estrTemp();
    
    va_list va;
    va_start(va, fmt);
    estrConcatfv(&buf, fmt, va);
    va_end(va);

    ret = sqlExecDdl(ddl_type, buf, estrLength(&buf));

    estrDestroy(&buf);

    return ret;
}

/** 
* Add a SQL database foreign key constraint from a column on the
* first table to be validated against the primary key of a 
* second table.
*/ 
void sqlAddForeignKeyConstraintAsync(char *table, char *key, char *foreign_table)
{
    char buf[SHORT_SQL_STRING];
    int buf_len;

    // SQLite keys are created and validated with the complete table template.
    if (gDatabaseProvider == DBPROV_SQLITE)
        return;
    switch (gDatabaseProvider) {
        xcase DBPROV_MSSQL:
            buf_len = sprintf(buf, "IF NOT EXISTS (SELECT constraint_name FROM information_schema.table_constraints WHERE constraint_name = 'FK_%s_%s_%s') ALTER TABLE dbo.%s ADD CONSTRAINT FK_%s_%s_%s FOREIGN KEY (%s) REFERENCES %s;", table, key, foreign_table, table, table, key, foreign_table, key, foreign_table);
        xcase DBPROV_POSTGRESQL:
            buf_len = sprintf(buf, "DO $$BEGIN IF NOT EXISTS (SELECT constraint_name FROM information_schema.table_constraints WHERE constraint_name = 'fk_%s_%s_%s') THEN ALTER TABLE dbo.%s ADD CONSTRAINT FK_%s_%s_%s FOREIGN KEY (%s) REFERENCES dbo.%s; END IF; END$$;", strlwrdupa(table), strlwrdupa(key), strlwrdupa(foreign_table), table, table, key, foreign_table, key, foreign_table);
        DBPROV_XDEFAULT();
    }
    sqlExecAsync(buf, buf_len);
}

/** 
* Removes a foreign key contraint from a SQL database table.
*/ 
void sqlRemoveForeignKeyConstraintAsync(char *table, char *key, char *foreign_table)
{
    char buf[SHORT_SQL_STRING];
    int buf_len;

    // A changed SQLite key is rejected by schema validation before startup.
    if (gDatabaseProvider == DBPROV_SQLITE)
        return;
    switch (gDatabaseProvider) {
        xcase DBPROV_MSSQL:
            buf_len = sprintf(buf, "IF EXISTS (SELECT constraint_name FROM information_schema.table_constraints WHERE constraint_name = 'FK_%s_%s_%s') ALTER TABLE dbo.%s DROP CONSTRAINT FK_%s_%s_%s;", table, key, foreign_table, table, table, key, foreign_table);
        xcase DBPROV_POSTGRESQL:
            buf_len = sprintf(buf, "DO $$BEGIN IF EXISTS (SELECT constraint_name FROM information_schema.table_constraints WHERE constraint_name = 'fk_%s_%s_%s') THEN ALTER TABLE dbo.%s DROP CONSTRAINT FK_%s_%s_%s; END IF; END$$;", strlwrdupa(table), strlwrdupa(key), strlwrdupa(foreign_table), table, table, key, foreign_table);
        DBPROV_XDEFAULT();
    }
    
    sqlExecAsync(buf, buf_len);
}

/**
* Add a named index to a SQL database table that contains both 
* the table's primary key and columns provided by with the 
* fields parameter. 
*
* @note Uses a hash of the table name to pick a single thread 
*       for all index operations on that table to avoid locking
*       problems.
*/
void sqlAddIndexAsync(char *index, char *table, char *fields)
{
    char buf[SHORT_SQL_STRING];
    int buf_len;

    switch (gDatabaseProvider) {
        xcase DBPROV_MSSQL:
            buf_len = sprintf(buf, "IF NOT EXISTS (SELECT sys.indexes.name FROM sys.indexes JOIN sys.objects on sys.indexes.object_id=sys.objects.object_id WHERE sys.indexes.name = N'%s' and sys.objects.name=N'%s') CREATE INDEX %s ON dbo.%s (%s);", index, table, index, table, fields);
        xcase DBPROV_POSTGRESQL:
            buf_len = sprintf(buf, "DO $$BEGIN IF NOT EXISTS (SELECT relname FROM pg_catalog.pg_class JOIN pg_catalog.pg_index ON pg_catalog.pg_class.oid = pg_catalog.pg_index.indexrelid WHERE relname = '%s' AND indrelid = '%s'::regclass::oid) THEN CREATE INDEX %s ON dbo.%s (%s); END IF; END$$;", strlwrdupa(index), strlwrdupa(table), index, table, fields);
        xcase DBPROV_SQLITE:
            buf_len = sprintf(buf, "CREATE INDEX IF NOT EXISTS %s_%s ON %s (%s);", table, index, table, fields);
        DBPROV_XDEFAULT();
    }

    sqlExecAsyncEx(buf, buf_len, hashStringInsensitive(table), false);
}

/**
* Remove an index by name from a SQL database table.
*
* @note Uses a hash of the table name to pick a single thread 
*       for all index operations on that table to avoid locking
*       problems.
*/
void sqlRemoveIndexAsync(char *index, char *table)
{
    char buf[SHORT_SQL_STRING];
    int buf_len;

    switch (gDatabaseProvider) {
        xcase DBPROV_MSSQL:
            buf_len = sprintf(buf, "IF EXISTS (SELECT sys.indexes.name FROM sys.indexes JOIN sys.objects on sys.indexes.object_id=sys.objects.object_id WHERE sys.indexes.name = N'%s' and sys.objects.name=N'%s') DROP INDEX %s ON dbo.%s;", index, table, index, table);
        xcase DBPROV_POSTGRESQL:
            buf_len = sprintf(buf, "DO $$BEGIN IF EXISTS (SELECT relname FROM pg_catalog.pg_class JOIN pg_catalog.pg_index ON pg_catalog.pg_class.oid = pg_catalog.pg_index.indexrelid WHERE relname = '%s' AND indrelid = '%s'::regclass::oid) THEN DROP INDEX %s ON dbo.%s; END IF; END$$;", strlwrdupa(index), strlwrdupa(table), index, table);
        xcase DBPROV_SQLITE:
            buf_len = sprintf(buf, "DROP INDEX IF EXISTS %s_%s;", table, index);
        DBPROV_XDEFAULT();
    }

    sqlExecAsyncEx(buf, buf_len, hashStringInsensitive(table), false);
}

void sqlDeleteRowInternal(char *table, int container_id, SqlConn conn)
{
    char buf[SHORT_SQL_STRING];
    int buf_len;

    buf_len = sprintf(buf,"DELETE FROM dbo.%s WHERE ContainerId = %d;", table, container_id);
    if (gDatabaseProvider == DBPROV_SQLITE)
    {
        DbStorageError error = {0};
        DbStorage *storage = dbGetStorage();
        if (dbStorageExecuteNative(storage->ops->connection(storage, conn), buf, &error) != DB_STORAGE_OK)
            FatalErrorf("Unable to delete SQLite container row: %s\n%s", error.message, buf);
    }
    else
        sqlConnExecDirect(buf, buf_len, conn, false);
}

void sqlDeleteContainer(ContainerTemplate *tplt, int container_id)
{
    int            i;
    TableInfo    *table;

    if (!tplt || tplt->dont_write_to_sql)
        return;

    if (gDatabaseProvider == DBPROV_SQLITE) {
        char *sql = NULL;
        for (i = tplt->table_count - 1; i >= 0; --i)
            estrConcatf(&sql, "DELETE FROM \"%s\" WHERE ContainerId=%d;", tplt->tables[i].name, container_id);
        sqlExecAsync(sql, estrLength(&sql));
        estrDestroy(&sql); return;
    }

    for(i=tplt->table_count-1;i>=0;i--)
    {
        table = &tplt->tables[i];
        sqlDeleteRowAsync(table->name, container_id);
    }

    return;
}

static void sqlFlushStatement(HSTMT stmt, char **estr, unsigned *bind, SqlConn conn)
{
#ifdef FULLDEBUG
    assert(estrUTF8CharacterCount(estr));
#endif

    sqlConnStmtExecDirectMany(stmt, *estr, estrLength(estr), conn, false);
    estrClear(estr);

    sqlConnStmtUnbindCols(stmt);
    sqlConnStmtUnbindParams(stmt);
    sqlConnStmtCloseCursor(stmt);
    *bind = 0;
}

static void sqlContainerAddOrDelRows(ContainerTemplate *tplt, int *container_id, LineList *diff, char **estr, unsigned *bind, HSTMT stmt, SqlConn conn)
{
    unsigned i;
    unsigned diff_cmd_count = diff->cmd_count;

    for (i=0; i<diff_cmd_count; i++) {
        RowAddDel *cmd = &diff->row_cmds[i];
        SlotInfo *slot = &tplt->slots[cmd->idx];
        char *name = slot->table->name;

        if (cmd->add) {
            switch (slot->table->table_type) {
                xcase TT_CONTAINER:
                    switch (gDatabaseProvider) {
                        xcase DBPROV_MSSQL:
                            estrConcatf(estr, "SET IDENTITY_INSERT dbo.%s ON; INSERT INTO dbo.%s (ContainerId) VALUES (?); SET IDENTITY_INSERT dbo.%s OFF;", name, name, name);
                            bindInputParameter(stmt, (*bind)++, CFTYPE_INT, container_id, NULL);
                        xcase DBPROV_POSTGRESQL:
                            estrConcatf(estr, "SELECT setval(pg_get_serial_sequence('dbo.%s', 'containerid'), ?); INSERT INTO dbo.%s (ContainerId) VALUES (?);", name, name);
                            bindInputParameter(stmt, (*bind)++, CFTYPE_INT, container_id, NULL);
                            bindInputParameter(stmt, (*bind)++, CFTYPE_INT, container_id, NULL);
                        DBPROV_XDEFAULT();
                    }                    
                xcase TT_SUBCONTAINER:
                    estrConcatf(estr, "INSERT INTO dbo.%s (ContainerId,SubId) VALUES (?,?);", name);
                    bindInputParameter(stmt, (*bind)++, CFTYPE_INT, container_id, NULL);
                    bindInputParameter(stmt, (*bind)++, CFTYPE_INT, &slot->sub_id, NULL);                    
                xdefault:
                    FatalErrorf(__FUNCTION__ "does not implement table type %d", slot->table->table_type);
            }
        } else {
            switch (slot->table->table_type) {
                xcase TT_CONTAINER:
                    estrConcatf(estr, "DELETE FROM dbo.%s WHERE ContainerId=?;", name);
                    bindInputParameter(stmt, (*bind)++, CFTYPE_INT, container_id, NULL);
                xcase TT_SUBCONTAINER:
                    estrConcatf(estr, "DELETE FROM dbo.%s WHERE ContainerId=? AND SubId=?;", name);
                    bindInputParameter(stmt, (*bind)++, CFTYPE_INT, container_id, NULL);
                    bindInputParameter(stmt, (*bind)++, CFTYPE_INT, &slot->sub_id, NULL);
                xdefault:
                    FatalErrorf(__FUNCTION__ "does not implement table type %d", slot->table->table_type);
            }
        }

        if (estrLength(estr) > LONG_SQL_STRING || *bind >= FLUSH_BINDS) {
            sqlFlushStatement(stmt, estr, bind, conn);
        }
    }
}

static void sqlContainerUpdateRows(ContainerTemplate *tplt, int *container_id, LineList *diff, char **estr, unsigned *bind, HSTMT stmt, SqlConn conn, sqlBindTemp *** bind_temps)
{
    unsigned i;
    unsigned diff_count = diff->count;

    for (i=0; i<diff_count; )
    {
        SlotInfo    *last_slot = &tplt->slots[diff->lines[i].idx];

        estrConcatf(estr, "UPDATE dbo.%s SET ", last_slot->table->name);
        for (; i<diff_count; i++)
        {
            LineTracker    *line    = &diff->lines[i];
            SlotInfo    *slot    = &tplt->slots[line->idx];
            TableInfo    *table    = slot->table;
            ColumnInfo    *col    = &table->columns[slot->idx];

            if (slot->sub_id != last_slot->sub_id || table != last_slot->table)
                break;

            TODO(); // Rip out this deprecated funcionality
            if (col->data_type == CFTYPE_TEXTBLOB) {                
                char *s = diff->text + line->str_idx;
                if(*s)
                {
                    char *hexbuf = _alloca(strlen(s)*2+1);
                    binStrToHexStr2(s,hexbuf);
                    estrConcatf(estr,"%s='HEXX%s',",col->name,hexbuf);
                }
                else
                {
                    estrConcatf(estr,"%s=NULL,",col->name);
                }
                continue;
            }

            estrConcatf(estr, "%s=?,", col->name);
            switch(col->data_type)
            {
                default:
#ifdef FULLDEBUG
                    assert(strlen(diff->text + line->str_idx) == line->size);
#endif
                    bindInputParameter(stmt, (*bind)++, CFTYPE_ANSISTRING, diff->text + line->str_idx, &line->size);
                    break;
                case CFTYPE_UNICODESTRING:
                case CFTYPE_UNICODESTRING_MAX:
                {
                    sqlBindTemp * temp = calloc(sizeof(sqlBindTemp) + (line->size+1) * sizeof(wchar_t), 1);
                    int wsize = MultiByteToWideChar(CP_UTF8, 0, diff->text + line->str_idx, (int)line->size, temp->wdata, (int)line->size + 1);
                    temp->wdata[wsize] = 0;
#ifdef FULLDEBUG
                    assert(UTF16GetLength(temp->wdata) == UTF8GetLength(diff->text + line->str_idx));
#endif
                    temp->bytes = wsize * sizeof(wchar_t);
                    bindInputParameter(stmt, (*bind)++, CFTYPE_UNICODESTRING, temp->wdata, &temp->bytes);
                    eaPush(bind_temps, temp);
                    break;
                }
                case CFTYPE_BINARY_MAX:
                {
#if ACTUALLY_STORE_BINARY_AS_BINARY
                    int int_size;
                    sqlBindTemp * temp = malloc(sizeof(sqlBindTemp) + line->size);
                    unescapeDataToBuffer(temp->data, diff->text + line->str_idx, &int_size);
                    temp->bytes = int_size;
                    bindInputParameter(stmt, (*bind)++, CFTYPE_BINARY_MAX, temp->data, &temp->bytes);
                    eaPush(bind_temps, temp);
#else
                    bindInputParameter(stmt, (*bind)++, CFTYPE_BINARY_MAX, diff->text + line->str_idx, &line->size);
#endif
                    break;
                }
                case CFTYPE_INT:
                case CFTYPE_SHORT:
                case CFTYPE_BYTE:
                    bindInputParameter(stmt, (*bind)++, CFTYPE_INT, line->ival ? &line->ival : NULL, NULL);
                    break;
                case CFTYPE_FLOAT:
                    bindInputParameter(stmt, (*bind)++, CFTYPE_FLOAT, &line->fval, NULL);
                    break;
            }
        }

        estrSetLength(estr, estrLength(estr)-1);
        if (!last_slot->table->array_count) {
            estrConcatStaticCharArray(estr, " WHERE ContainerId=?;");
            bindInputParameter(stmt, (*bind)++, CFTYPE_INT, container_id, NULL);
        } else {
            estrConcatStaticCharArray(estr, " WHERE ContainerId=? AND SubId=?;");
            bindInputParameter(stmt, (*bind)++, CFTYPE_INT, container_id, NULL);
            bindInputParameter(stmt, (*bind)++, CFTYPE_INT, &last_slot->sub_id, NULL);
        }

        if (estrLength(estr) > LONG_SQL_STRING || *bind >= FLUSH_BINDS) {
            sqlFlushStatement(stmt, estr, bind, conn);
        }
    }
}

static bool sqliteExecuteStatement(DbStorageConnection *connection, const char *query, ContainerValue *values, int valueCount, DbStorageError *error)
{
    DbStorageStatement *statement = connection->ops->prepare(connection, query, strlen(query), error);
    DbStorageResult result;
    int i;
    if (!statement) return false;
    for (i = 0; i < valueCount; ++i) if (dbStorageBind(statement, i + 1, values[i], error) != DB_STORAGE_OK) {
        connection->ops->finalize(statement); return false;
    }
    result = connection->ops->execute(statement, error);
    connection->ops->finalize(statement);
    return result == DB_STORAGE_DONE;
}

static ContainerValue sqliteLineValue(LineList *diff, LineTracker *line, ColumnInfo *column, S64 *integerValue, double *floatValue)
{
    ContainerValue value = { CONTAINER_VALUE_NULL, NULL, 0 };

    switch (column->data_type)
    {
        xcase CFTYPE_INT:
        case CFTYPE_SHORT:
        case CFTYPE_BYTE:
            if (line->ival)
            {
                *integerValue = line->ival;
                value.type = CONTAINER_VALUE_INT;
                value.data = integerValue;
                value.size = sizeof(*integerValue);
            }
        xcase CFTYPE_FLOAT:
            *floatValue = line->fval;
            value.type = CONTAINER_VALUE_FLOAT;
            value.data = floatValue;
            value.size = sizeof(*floatValue);
        xdefault:
            value.type = column->data_type == CFTYPE_DATETIME ? CONTAINER_VALUE_DATETIME : column->data_type == CFTYPE_BINARY_MAX || column->data_type == CFTYPE_BLOB ? CONTAINER_VALUE_BLOB : CONTAINER_VALUE_TEXT;
            value.data = diff->text + line->str_idx;
            value.size = line->size;
    }
    return value;
}

static void sqlContainerUpdateSqlite(ContainerTemplate *tplt, int container_id, LineList *diff, SqlConn conn)
{
    DbStorage *storage = dbGetStorage();
    DbStorageConnection *connection = storage->ops->connection(storage, conn);
    DbStorageError error = {0};
    ContainerValue values[MAX_QUERY_RESULTS];
    S64 integerValues[MAX_QUERY_RESULTS];
    double floatValues[MAX_QUERY_RESULTS];
    S64 sqliteContainerId = container_id;
    int i;

    if (storage->ops->transaction(connection, true, false, &error) != DB_STORAGE_OK)
        FatalErrorf("Unable to begin SQLite container update: %s", error.message);

    for (i = 0; i < diff->cmd_count; ++i)
    {
        RowAddDel *command = &diff->row_cmds[i];
        SlotInfo *slot = &tplt->slots[command->idx];
        char query[512];
        int valueCount = 1;

        values[0].type = CONTAINER_VALUE_INT;
        values[0].data = &sqliteContainerId;
        values[0].size = sizeof(sqliteContainerId);
        if (slot->table->table_type == TT_SUBCONTAINER)
        {
            integerValues[1] = slot->sub_id;
            values[1].type = CONTAINER_VALUE_INT;
            values[1].data = &integerValues[1];
            values[1].size = sizeof(integerValues[1]);
            valueCount = 2;
        }

        if (command->add && slot->table->table_type == TT_CONTAINER)
            sprintf(query, "INSERT INTO \"%s\" (ContainerId) VALUES (?);", slot->table->name);
        else if (command->add)
            sprintf(query, "INSERT INTO \"%s\" (ContainerId, SubId) VALUES (?, ?);", slot->table->name);
        else if (slot->table->table_type == TT_CONTAINER)
            sprintf(query, "DELETE FROM \"%s\" WHERE ContainerId = ?;", slot->table->name);
        else
            sprintf(query, "DELETE FROM \"%s\" WHERE ContainerId = ? AND SubId = ?;", slot->table->name);
        if (!sqliteExecuteStatement(connection, query, values, valueCount, &error)) goto rollback;
    }

    for (i = 0; i < diff->count; )
    {
        LineTracker *firstLine = &diff->lines[i];
        SlotInfo *firstSlot = &tplt->slots[firstLine->idx];
        char *query = NULL;
        int valueCount = 0;

        estrPrintf(&query, "UPDATE \"%s\" SET ", firstSlot->table->name);
        for (; i < diff->count; ++i)
        {
            LineTracker *line = &diff->lines[i];
            SlotInfo *slot = &tplt->slots[line->idx];
            ColumnInfo *column;
            if (slot->table != firstSlot->table || slot->sub_id != firstSlot->sub_id)
                break;
            column = &slot->table->columns[slot->idx];
            if (valueCount >= MAX_QUERY_RESULTS - 2) break;
            estrConcatf(&query, "%s\"%s\" = ?", valueCount ? ", " : "", column->name);
            values[valueCount] = sqliteLineValue(diff, line, column, &integerValues[valueCount], &floatValues[valueCount]);
            ++valueCount;
        }
        estrConcatStaticCharArray(&query, " WHERE ContainerId = ?");
        integerValues[valueCount] = container_id;
        values[valueCount].type = CONTAINER_VALUE_INT;
        values[valueCount].data = &integerValues[valueCount];
        values[valueCount].size = sizeof(integerValues[valueCount]);
        ++valueCount;
        if (firstSlot->table->array_count)
        {
            estrConcatStaticCharArray(&query, " AND SubId = ?");
            integerValues[valueCount] = firstSlot->sub_id;
            values[valueCount].type = CONTAINER_VALUE_INT;
            values[valueCount].data = &integerValues[valueCount];
            values[valueCount].size = sizeof(integerValues[valueCount]);
            ++valueCount;
        }
        estrConcatStaticCharArray(&query, ";");
        if (!sqliteExecuteStatement(connection, query, values, valueCount, &error)) { estrDestroy(&query); goto rollback; }
        estrDestroy(&query);
    }

    if (storage->ops->transaction(connection, false, true, &error) != DB_STORAGE_OK)
        goto rollback;
    return;
rollback:
    storage->ops->transaction(connection, false, false, NULL);
    FatalErrorf("SQLite container update rolled back: %s", error.message);
}

/**
* @note Need an use array of temporary buffers in order to do this (string classes
* will move the memory on a reallocation).
*/
void sqlContainerUpdateInternal(ContainerTemplate *tplt, int container_id, LineList *diff, SqlConn conn)
{
    static char *estrs[SQLCONN_MAX] = {0,};
    static sqlBindTemp** bind_temps[SQLCONN_MAX] = {0,};
    unsigned bind = 0;
    HSTMT stmt;

    if (gDatabaseProvider == DBPROV_SQLITE)
    {
        sqlContainerUpdateSqlite(tplt, container_id, diff, conn);
        return;
    }

    stmt = sqlConnStmtAlloc(conn);

    estrClear(&estrs[conn]);

    if (!bind_temps[conn])
        eaCreate(&bind_temps[conn]);

    sqlContainerAddOrDelRows(tplt, &container_id, diff, &estrs[conn], &bind, stmt, conn);
    sqlContainerUpdateRows(tplt, &container_id, diff, &estrs[conn], &bind, stmt, conn, &bind_temps[conn]);

    if (estrLength(&estrs[conn]))
        sqlFlushStatement(stmt, &estrs[conn], &bind, conn);

    sqlConnStmtFree(stmt);

    eaDestroyEx(&bind_temps[conn], NULL);
}

static void* s_getDataAtExec(HSTMT stmt, ColumnInfo *field, int column_idx, ssize_t *results, char *original_command, SqlConn conn)
{
    return sqlConnStmtGetData(stmt, column_idx+1, g_containerfieldinfo[field->data_type].access_type, results, original_command, conn);
}

static char row_data[SQLCONN_MAX][MAX_QUERY_SIZE];
static size_t row_results[SQLCONN_MAX][MAX_QUERY_RESULTS];
static int glob_container_id[SQLCONN_MAX];

static void s_bindField(HSTMT stmt, ColumnInfo *field, int column_idx, int *data_idx, SqlConn conn)
{
    if(CFTYPE_IS_DYNAMIC(field->data_type)) {
        assert(field->num_bytes == -1);
        return;
    }

    bindOutputColumn(stmt, column_idx, field->data_type, field->num_bytes, &row_data[conn][*data_idx], &row_results[conn][column_idx]);

    assert(field->num_bytes > 0);
    (*data_idx) += field->num_bytes;
}

static void* s_getField(HSTMT stmt, ColumnInfo *field, int column_idx, int *data_idx, SqlConn conn, char *original_command)
{
    S8 *data = &row_data[conn][*data_idx];
    SQLLEN *results = row_results[conn] + column_idx;

    switch(field->data_type)
    {
        case CFTYPE_TEXTBLOB:
        {
            data = s_getDataAtExec(stmt, field, column_idx, results, original_command, conn);
            if (strncmp(data, "HEXX", 4)==0)
                hexStrToBinStr2(data + 4, data);
            return data;
        }
        case CFTYPE_BLOB:
        case CFTYPE_BINARY_MAX:
        case CFTYPE_UNICODESTRING_MAX:
        case CFTYPE_ANSISTRING_MAX:
        {
            return s_getDataAtExec(stmt, field, column_idx, results, original_command, conn);
        }
    };

    assert(field->num_bytes > 0);
    (*data_idx) += field->num_bytes;
    return data;
}

static HSTMT sqlTableSelect(TableInfo *table, int *id_ptr, SqlConn conn)
{
    int idx;
    int col;
    SQLRETURN retcode;
    HSTMT stmt;

    stmt = table->sql_select_link[conn];
    if (stmt)
        return stmt;

    table->sql_select_link[conn] = stmt = sqlConnStmtAlloc(conn);

    estrPrintf(&table->sql_select_query[conn], "SELECT * FROM dbo.%s WHERE ContainerId = ?;", table->name);

    retcode = sqlConnStmtPrepare(stmt, table->sql_select_query[conn], estrLength(&table->sql_select_query[conn]), conn);
    bindInputParameter(stmt, 0, CFTYPE_INT, id_ptr, NULL);

    idx = 0;
    assert(table->num_columns <= MAX_QUERY_RESULTS);
    for(col = 0; col < table->num_columns; col++)
        s_bindField(stmt, &table->columns[col], col, &idx, conn);
    assert(idx <= MAX_QUERY_SIZE);
    
    return stmt;
}

static bool addMemToLine(LineList *list, LineTracker *line, char *s, int s_len)
{
    char *base;
    int int_size;

    if (!s_len)
        return 0;

    line->str_idx = list->text_count;
    line->is_str = 1;

    base = dynArrayAdd(&list->text, 1, &list->text_count, &list->text_max, escapeDataMaxChars(s_len)+1);
    escapeDataToBuffer(base, s, s_len, &int_size);
    line->size = int_size;
    base[line->size] = 0;

    // Trim excess space
    list->text_count = (int)(line->str_idx + line->size + 1);

    return true;
}

static bool addWStrToLine(LineList *list, LineTracker *line, wchar_t *ws, int ws_len)
{
    char *base;
    int worst_utf8_size;

    if (!ws_len && !(ws_len = (int)wcslen(ws)))
        return 0;

#ifdef FULLDEBUG
    assert(ws_len == wcslen(ws));
#endif

    line->str_idx = list->text_count;
    line->is_str = 1;

    // Allocate 3/2 the storage of the original string, which is the worst case for a UTF16 -> UTF8 conversion
    worst_utf8_size = ws_len*3+1;
    base = dynArrayAdd(&list->text, 1, &list->text_count, &list->text_max, worst_utf8_size);
    line->size = WideCharToMultiByte(CP_UTF8, 0, ws, ws_len, base, worst_utf8_size, NULL, NULL);
    base[line->size] = 0;

#ifdef FULLDEBUG
    assert(UTF16GetLength(ws) == UTF8GetLength(base));
#endif

    // Trim extra space
    list->text_count = (int)(line->str_idx + line->size + 1);

    return true;
}

static bool addStrToLine(LineList *list, LineTracker *line, char *s, int s_len)
{
    char *base;

    if (!s_len && !(s_len = (int)strlen(s)))
        return 0;

#ifdef FULLDEBUG
    assert(s_len == strlen(s));
#endif

    line->str_idx = list->text_count;
    line->is_str = 1;
    line->size = s_len;

    base = dynArrayAdd(&list->text, 1, &list->text_count, &list->text_max, s_len+1);
    memcpy(base, s, s_len);
    base[s_len] = 0;

    return true;
}

/**
* @note This function should parallel structToLine() in dbcontainerpack.c
*/
static int readRow(HSTMT stmt, ContainerTemplate *tplt, TableInfo *table, LineList *list, SqlConn conn, int read_reserved, char *original_command)
{
    SQLLEN *results = row_results[conn];
    bool found = false;
    int sub_id = -1; // -1 indicates this is not a subcontainer
    int idx = 0;
    int col;

    if(table->array_count)
    {
        // Assume that SubId is hard coded in the buffer at offset 4...
        sub_id = *(int*)(&row_data[conn][4]);

        if (sub_id >= table->array_count)    // read too many rows
            return 0;
    }

    assert(table->num_columns <= MAX_QUERY_RESULTS);
    for(col = 0; col < table->num_columns; col++)
    {
        ColumnInfo *field = &table->columns[col];
        void *data;
        int cmd_idx;
        int line_count;
        LineTracker    *line;

        data = s_getField(stmt, field, col, &idx, conn, original_command);
        if(!data)
            return -1;

        found = true;

        if ((results[col] == SQL_NULL_DATA) || (field->reserved_word && !read_reserved) || field->is_sub_id_field)
            continue;

        if (read_reserved)
        {
            char table_field[256];

            if (field->reserved_word || sub_id < 0)
                strcpy(table_field, field->name);
            else
                sprintf(table_field, "%s[%d].%s", table->name, sub_id, field->name);

            if (!stashFindInt(tplt->all_hashes, table_field, &cmd_idx))
                continue;
        }
        else
        {
            cmd_idx = table->all_hash_first_idx + col;
            if (sub_id >= 0)
                cmd_idx += sub_id * table->num_columns;
        }

        // Grow to make room for the new line, but do not mark it as added unless we retrieve a valid "non-deafault" value
        line_count = list->count;
        line = dynArrayAdd(&list->lines, sizeof(list->lines[0]), &line_count, &list->max_lines, 1);
        line->is_str = 0;
        line->idx = cmd_idx;

        switch(field->data_type)
        {
            xcase CFTYPE_INT:
#ifdef FULLDEBUG
                assert(results[col] == sizeof(int));
#endif
                if(!(line->ival = *(int *)data))
                    continue;

                // Validate the range of the attribute
                if ((field->attr) && ((line->ival < 1) || (line->ival >= eaSize(&field->attr->names))))
                    continue;

            xcase CFTYPE_SHORT:
#ifdef FULLDEBUG
                assert(results[col] == sizeof(short));
#endif
                if(!(line->ival = *(short *)data))
                    continue;

            xcase CFTYPE_BYTE:
#ifdef FULLDEBUG
                assert(results[col] == sizeof(U8));
#endif
                if(!(line->ival = *(U8 *)data))
                    continue;

            xcase CFTYPE_FLOAT:
#ifdef FULLDEBUG
                assert(results[col] == sizeof(float));
#endif
                if(!(line->fval = *(float *)data))
                    continue;

            xcase CFTYPE_BINARY_MAX:
#if ACTUALLY_STORE_BINARY_AS_BINARY
                if(!addMemToLine(list, line, data, results[col]))
                    continue;
#else
                if(!addStrToLine(list, line, data, results[col]))
                    continue;
#endif

            xcase CFTYPE_UNICODESTRING:
            case CFTYPE_UNICODESTRING_MAX:
                if(!addWStrToLine(list, line, data, results[col]/2))
                    continue;

            xcase CFTYPE_ANSISTRING:
             case CFTYPE_ANSISTRING_MAX:
             case CFTYPE_TEXTBLOB:
             case CFTYPE_BLOB:
                if(!addStrToLine(list, line, data, results[col]))
                    continue;

            xcase CFTYPE_DATETIME:
            {
                SQL_TIMESTAMP_STRUCT *t = (void *)(data);
                char buf[128];
                int buf_len;

#ifdef FULLDEBUG
                assert(results[col] == sizeof(SQL_TIMESTAMP_STRUCT));
#endif

                if (t->year < 2001 || t->year > 2100) //old corruption lead to many 2136 dates, we should ignore them
                {
                    // If this date looks invalid, report it as null
                    continue;
                }
                buf_len = sprintf(buf,"%04d-%02d-%02d %02d:%02d:%02d", t->year, t->month, t->day, t->hour, t->minute, t->second);
                addStrToLine(list, line, buf, buf_len);
            }

            xdefault:
                assertmsgf(0, "unhandled line type %d", field->data_type);
        }

        // Got a valid "non-default" value, mark the line as valid
        list->count = line_count;
    }

    // No non-NULL values were found for this row, in order to indicate the
    // existence of this row in the database, add a fake entry.
    if (!list->count) {
        LineTracker    *line = dynArrayAdd(&list->lines, sizeof(list->lines[0]), &list->count, &list->max_lines, 1);
        line->str_idx = FAKE_STR_IDX;
        line->is_str = 1;
        line->size = 0;

        // Point at the ContainerId field
        line->idx = table->all_hash_first_idx;
        if (sub_id >= 0)
            line->idx += sub_id * table->num_columns;
    }

    return found;
}

static int readSqliteRow(DbStorageStatement *statement, ContainerTemplate *tplt, TableInfo *table, LineList *list, int read_reserved)
{
    int sub_id = -1;
    int col;
    int initialCount = list->count;

    if (table->array_count)
    {
        for (col = 0; col < table->num_columns; ++col)
        {
            if (table->columns[col].is_sub_id_field)
            {
                ContainerValue value = dbStorageColumn(statement, col);
                if (value.type == CONTAINER_VALUE_INT)
                    sub_id = (int)*(const S64 *)value.data;
                break;
            }
        }
        if (sub_id < 0 || sub_id >= table->array_count)
            return 0;
    }

    for (col = 0; col < table->num_columns; ++col)
    {
        ColumnInfo *field = &table->columns[col];
        ContainerValue value = dbStorageColumn(statement, col);
        LineTracker *line;
        int cmd_idx;
        int line_count;

        if (value.type == CONTAINER_VALUE_NULL || (field->reserved_word && !read_reserved) || field->is_sub_id_field)
            continue;

        if (read_reserved)
        {
            char table_field[256];
            if (field->reserved_word || sub_id < 0)
                strcpy(table_field, field->name);
            else
                sprintf(table_field, "%s[%d].%s", table->name, sub_id, field->name);
            if (!stashFindInt(tplt->all_hashes, table_field, &cmd_idx))
                continue;
        }
        else
        {
            cmd_idx = table->all_hash_first_idx + col;
            if (sub_id >= 0)
                cmd_idx += sub_id * table->num_columns;
        }

        line_count = list->count;
        line = dynArrayAdd(&list->lines, sizeof(list->lines[0]), &line_count, &list->max_lines, 1);
        line->is_str = 0;
        line->idx = cmd_idx;

        switch (field->data_type)
        {
            xcase CFTYPE_INT:
            case CFTYPE_SHORT:
            case CFTYPE_BYTE:
                line->ival = value.type == CONTAINER_VALUE_INT ? (int)*(const S64 *)value.data : atoi(value.data);
                if (!line->ival || (field->attr && (line->ival < 1 || line->ival >= eaSize(&field->attr->names))))
                    continue;
            xcase CFTYPE_FLOAT:
                if (value.type == CONTAINER_VALUE_FLOAT)
                    line->fval = (float)*(const double *)value.data;
                else if (value.type == CONTAINER_VALUE_INT)
                    line->fval = (float)*(const S64 *)value.data;
                else
                    line->fval = (float)atof(value.data);
                if (!line->fval)
                    continue;
            xcase CFTYPE_BINARY_MAX:
#if ACTUALLY_STORE_BINARY_AS_BINARY
                if (!addMemToLine(list, line, (char *)value.data, (int)value.size))
                    continue;
#else
                if (!addStrToLine(list, line, (char *)value.data, (int)value.size))
                    continue;
#endif
            xcase CFTYPE_UNICODESTRING:
            case CFTYPE_UNICODESTRING_MAX:
            case CFTYPE_ANSISTRING:
            case CFTYPE_ANSISTRING_MAX:
            case CFTYPE_TEXTBLOB:
            case CFTYPE_BLOB:
                if (!addStrToLine(list, line, (char *)value.data, (int)value.size))
                    continue;
            xcase CFTYPE_DATETIME:
            {
                int year;
                if (sscanf(value.data, "%d", &year) != 1 || year < 2001 || year > 2100 ||
                    !addStrToLine(list, line, (char *)value.data, (int)value.size))
                    continue;
            }
            xdefault:
                assertmsgf(0, "unhandled SQLite line type %d", field->data_type);
        }
        list->count = line_count;
    }

    if (list->count == initialCount)
    {
        LineTracker *line = dynArrayAdd(&list->lines, sizeof(list->lines[0]), &list->count, &list->max_lines, 1);
        line->str_idx = FAKE_STR_IDX;
        line->is_str = 1;
        line->size = 0;
        line->idx = table->all_hash_first_idx;
        if (sub_id >= 0)
            line->idx += sub_id * table->num_columns;
    }
    return 1;
}

static int sqlTableRead(ContainerTemplate *tplt, TableInfo *table, int container_id, LineList *list, SqlConn conn)
{
    HSTMT        stmt;
    RETCODE        retcode;
    int            found=0;
    int            multi_count=0;

    if (gDatabaseProvider == DBPROV_SQLITE)
    {
        DbStorage *storage = dbGetStorage();
        DbStorageConnection *connection = storage->ops->connection(storage, conn);
        DbStorageError error = {0};
        DbStorageStatement *statement;
        DbStorageResult result;
        ContainerValue id = { CONTAINER_VALUE_INT, NULL, sizeof(S64) };
        S64 sqliteContainerId = container_id;
        char *query = NULL;

        id.data = &sqliteContainerId;
        estrPrintf(&query, "SELECT * FROM \"%s\" WHERE ContainerId = ?;", table->name);
        statement = storage->ops->prepare(connection, query, estrLength(&query), &error);
        if (!statement || dbStorageBind(statement, 1, id, &error) != DB_STORAGE_OK)
            FatalErrorf("Unable to prepare SQLite container read: %s\n%s", error.message, query);
        while ((result = storage->ops->step(statement, &error)) == DB_STORAGE_ROW)
            found |= readSqliteRow(statement, tplt, table, list, 0);
        storage->ops->finalize(statement);
        if (result == DB_STORAGE_ERROR || result == DB_STORAGE_RETRY)
            FatalErrorf("Unable to execute SQLite container read: %s\n%s", error.message, query);
        estrDestroy(&query);
        return found;
    }

    stmt = sqlTableSelect(table, &glob_container_id[conn], conn);
    glob_container_id[conn] = container_id;

    retcode = _sqlConnStmtExecute(stmt, conn);
    if (SQL_SHOW_ERROR(retcode))
        sqlConnStmtPrintError(stmt, table->sql_select_query[conn]);

    while((retcode = _sqlConnStmtFetch(stmt, conn)) != SQL_NO_DATA)
    {
        int r;

        if (retcode != SQL_SUCCESS)
            sqlConnStmtPrintError(stmt, table->sql_select_query[conn]);

        if (!SQL_SUCCEEDED(retcode))
            break;

        r = readRow(stmt, tplt, table, list, conn, 0, table->sql_select_query[conn]);
        if(r < 0)
            break;
        found |= r;
    }

    sqlConnStmtCloseCursor(stmt);

    return found;
}

int sqlTableReadMulti(ContainerTemplate *tplt, TableInfo *table, int container_id, char ***multi_buf_ptr, SqlConn conn)
{
    HSTMT stmt;
    char cmd_buf[SHORT_SQL_STRING];
    int cmd_len;

    RETCODE        retcode;
    int            idx,col,cant_find=1;
    int            multi_count=0;
    char        **multi_buf=0;
    LineList    list = {0};

    TODO(); // Code cleanup needed to improve clarity for future audits

    stmt = sqlConnStmtAlloc(conn);

    cmd_len = sprintf(cmd_buf, "SELECT * FROM %s;", table->name);

    idx = 0;
    assert(table->num_columns <= MAX_QUERY_RESULTS);
    for(col = 0; col < table->num_columns; col++)
        s_bindField(stmt, &table->columns[col], col, &idx, conn);
    assert(idx <= MAX_QUERY_SIZE);

    retcode = sqlConnStmtExecDirect(stmt, cmd_buf, cmd_len, conn, true);

    while((retcode = _sqlConnStmtFetch(stmt, conn)) != SQL_NO_DATA)
    {
        int        size;
        char    *mem;

        if (retcode != SQL_SUCCESS)
            sqlConnStmtPrintError(stmt, cmd_buf);

        if(!SQL_SUCCEEDED(retcode))
            continue;

        if(readRow(stmt, tplt, table, &list, conn, 1, cmd_buf) < 0)
            break;

        multi_count++;
        multi_buf = realloc(multi_buf,multi_count * sizeof(char **));
        mem = lineListToMem(&list, &size, 0, 0);
        multi_buf[multi_count-1] = malloc(size);
        memcpy(multi_buf[multi_count-1],mem,size);
        reinitLineList(&list);
    }

    sqlConnStmtFree(stmt);

    *multi_buf_ptr = multi_buf;
    freeLineList(&list);
    return multi_count;
}

char *sqlContainerRead(ContainerTemplate *tplt, int container_id, SqlConn conn)
{
    if (conn == SQLCONN_FOREGROUND) sqlFifoFlushBeforeForeground();
    static LineList lists[SQLCONN_MAX];
    static void *mem_buf[SQLCONN_MAX];
    static int mem_max[SQLCONN_MAX];

    LineList *list = &lists[conn];
    int i, size;
    void *mem;

    if(!tplt)
        return 0;

    reinitLineList(list);

    for(i = 0; i < tplt->table_count; i++)
        if(!sqlTableRead(tplt, &tplt->tables[i], container_id, list, conn) && !i)
            return 0;// Primary table read failed - container doesn't exist.

    lineListToMem(list, &size, &mem_buf[conn], &mem_max[conn]);
    mem = malloc(size);
    memcpy(mem, mem_buf[conn], size);

    return mem;
}

/** 
* Return a single integer result from the result of executing a
* command on the SQL database.
* 
* @param bind_list An optional list of parameter
*                  binds terminated by an entry of type @ref
*                  CFTYPE_NULL.
*/ 
int sqlGetSingleValue(char *cmd, int cmd_len, BindList * bind_list, SqlConn conn)
{
    if (conn == SQLCONN_FOREGROUND) sqlFifoFlushBeforeForeground();
    if (gDatabaseProvider == DBPROV_SQLITE)
    {
        DbStorage *storage = dbGetStorage();
        DbStorageConnection *connection = storage->ops->connection(storage, conn);
        DbStorageError error = {0};
        DbStorageStatement *statement;
        DbStorageResult result;
        char *sqliteCommand = NULL;
        int count = 0;
        int bind = 1;

        estrConcatFixedWidth(&sqliteCommand, cmd, cmd_len == SQL_NTS ? (int)strlen(cmd) : cmd_len);
        statement = storage->ops->prepare(connection, sqliteCommand, estrLength(&sqliteCommand), &error);
        if (!statement)
            FatalErrorf("Unable to prepare SQLite query: %s\n%s", error.message, sqliteCommand);
        while (bind_list && bind_list->data_type)
        {
            ContainerValue value = { CONTAINER_VALUE_NULL, NULL, 0 };
            S64 integerValue;
            double floatValue;

            if (bind_list->data)
            {
                switch (bind_list->data_type)
                {
                    xcase CFTYPE_BYTE:
                        integerValue = *(const U8 *)bind_list->data;
                        value.type = CONTAINER_VALUE_INT;
                        value.data = &integerValue;
                        value.size = sizeof(integerValue);
                    xcase CFTYPE_SHORT:
                        integerValue = *(const S16 *)bind_list->data;
                        value.type = CONTAINER_VALUE_INT;
                        value.data = &integerValue;
                        value.size = sizeof(integerValue);
                    xcase CFTYPE_INT:
                        integerValue = *(const S32 *)bind_list->data;
                        value.type = CONTAINER_VALUE_INT;
                        value.data = &integerValue;
                        value.size = sizeof(integerValue);
                    xcase CFTYPE_FLOAT:
                        floatValue = *(const float *)bind_list->data;
                        value.type = CONTAINER_VALUE_FLOAT;
                        value.data = &floatValue;
                        value.size = sizeof(floatValue);
                    xdefault:
                        value.type = CONTAINER_VALUE_TEXT;
                        value.data = bind_list->data;
                        value.size = strlen(bind_list->data);
                }
            }
            if (dbStorageBind(statement, bind++, value, &error) != DB_STORAGE_OK)
                FatalErrorf("Unable to bind SQLite query: %s", error.message);
            ++bind_list;
        }
        result = storage->ops->step(statement, &error);
        if (result == DB_STORAGE_ROW)
        {
            ContainerValue value = dbStorageColumn(statement, 0);
            if (value.type == CONTAINER_VALUE_INT)
                count = (int)*(const S64 *)value.data;
            else if (value.type == CONTAINER_VALUE_FLOAT)
                count = (int)*(const double *)value.data;
        }
        else if (result != DB_STORAGE_DONE)
            FatalErrorf("Unable to execute SQLite query: %s\n%s", error.message, sqliteCommand);
        storage->ops->finalize(statement);
        estrDestroy(&sqliteCommand);
        return count;
    }

    HSTMT stmt;
    SQLRETURN retcode;
    int count = 0;

    stmt = sqlConnStmtAlloc(conn);

    if (bind_list) {
        int bind;
        for (bind = 0; bind_list->data_type; bind++, bind_list++) {
            bindInputParameter(stmt, bind, bind_list->data_type, bind_list->data, NULL);
        }        
    }

    bindOutputColumn(stmt, 0, CFTYPE_INT, sizeof(count), &count, &row_results[conn][0]);

    retcode = sqlConnStmtExecDirect(stmt, cmd, cmd_len, conn, true);
    if (SQL_SUCCEEDED(retcode)) {
        retcode = _sqlConnStmtFetch(stmt, conn);
        if (SQL_SHOW_ERROR(retcode))
            sqlConnStmtPrintError(stmt, cmd);
    }

    sqlConnStmtFree(stmt);
    return count;
}

char *sqlReadColumnsInternal(TableInfo *table, char *limit, char *col_names, char *where, int *count_ptr, ColumnInfo **field_ptrs, SqlConn conn, int debugDelayMS)
{
    if (conn == SQLCONN_FOREGROUND) sqlFifoFlushBeforeForeground();
    int col_count;
    int i;
    int retcode;
    HSTMT stmt;
    int count = 0;

    static char *cmd_buf[SQLCONN_MAX];

    char *dyn_mem = NULL;
    int dyn_mem_max = 0;
    int dyn_mem_idx = 0;

    int rec_size=0;
    char *args[200];

    if (!limit)
        limit = "";
    if (!where)
        where = "";

    if (debugDelayMS)
    {
        printf("Sleeping for %d MS due to debug setting! (%s)\n", debugDelayMS, where);
        Sleep(debugDelayMS);
        printf("Done sleeping.\n");
    }

    TODO(); // Code cleanup needed to improve clarity for future audits

    col_count = tokenize_line(strdupa(col_names), args, 0);
    assert(col_count <= MAX_QUERY_RESULTS);

    // Strip the trailing commas and verify that all entries are space delimited
    for(i=0; i<col_count-1; i++) 
    {
        int comma = (int)strlen(args[i]) - 1;
        assert(args[i][comma] == ',');
        args[i][comma] = 0;
        assert(!strchr(args[i], ','));
    }

    // the %s.%s below will end up making a statement like 'SELECT Ents.ContainerId, Name, ...',
    // which seems weird to me (being explicit only for the first column), but
    // some of our existing queries require us to be explicit for at least that
    // column.  // TODO: it'd be safer (and almost as fast) to be explicit for every column
    switch (gDatabaseProvider) {
        xcase DBPROV_MSSQL:
            estrPrintf(&cmd_buf[conn], "SELECT %s dbo.%s.%s FROM dbo.%s %s;", limit, table->name, col_names, table->name, where);
        xcase DBPROV_POSTGRESQL:
            estrPrintf(&cmd_buf[conn], "SELECT dbo.%s.%s FROM dbo.%s %s %s;", table->name, col_names, table->name, where, limit);
        xcase DBPROV_SQLITE:
        {
            bool distinct;
            int rowLimit;
            if (!parseSqliteSelectModifier(limit, &distinct, &rowLimit))
                FatalErrorf("Unsupported SQLite SELECT modifier: %s", limit);
            estrPrintf(&cmd_buf[conn], "SELECT %s%s FROM \"%s\" %s", distinct ? "DISTINCT " : "", col_names, table->name, where);
            if (rowLimit)
                estrConcatf(&cmd_buf[conn], " LIMIT %d", rowLimit);
            estrConcatChar(&cmd_buf[conn], ';');
        }
        DBPROV_XDEFAULT();
    }

    stmt = gDatabaseProvider == DBPROV_SQLITE ? NULL : sqlConnStmtAlloc(conn);

    for(i = 0; i < col_count; i++)
    {
        int col;
        for(col = 0; col < table->num_columns; col++)
        {
            ColumnInfo *field = &table->columns[col];
            if(stricmp(args[i], field->name)==0)
            {
                // this doesn't work because i don't have a good way to pass the data around and ensure that the it will be freed
                assert(!CFTYPE_IS_DYNAMIC(field->data_type));

                if (gDatabaseProvider == DBPROV_SQLITE)
                    rec_size += field->num_bytes;
                else
                    s_bindField(stmt, field, i, &rec_size, conn);

                field_ptrs[i] = field;
                break;
            }
        }
        if(col >= table->num_columns)
        {
            LOG(LOG_ERROR, LOG_LEVEL_IMPORTANT, LOG_CONSOLE, "Requested column name %s.%s doesn't exist.\n", table->name, args[i]);
            return 0;
        }
    }
    assert(rec_size <= MAX_QUERY_SIZE);

    field_ptrs[i] = 0;

    if (gDatabaseProvider == DBPROV_SQLITE)
    {
        DbStorage *storage = dbGetStorage();
        DbStorageConnection *connection = storage->ops->connection(storage, conn);
        DbStorageError error = {0};
        DbStorageStatement *statement = storage->ops->prepare(connection, cmd_buf[conn], estrLength(&cmd_buf[conn]), &error);
        DbStorageResult result;

        if (!statement)
            FatalErrorf("Unable to prepare SQLite query: %s\n%s", error.message, cmd_buf[conn]);
        while ((result = storage->ops->step(statement, &error)) == DB_STORAGE_ROW)
        {
            char *record;
            int offset = 0;

            dynArrayFit(&dyn_mem, 1, &dyn_mem_max, (count + 1) * rec_size);
            record = dyn_mem + count * rec_size;
            memset(record, 0, rec_size);
            for (i = 0; i < col_count; ++i)
            {
                ColumnInfo *field = field_ptrs[i];
                ContainerValue value = dbStorageColumn(statement, i);
                char *destination = record + offset;

                if (value.type != CONTAINER_VALUE_NULL)
                {
                    switch (field->data_type)
                    {
                        xcase CFTYPE_BYTE: *(U8 *)destination = (U8)*(const S64 *)value.data;
                        xcase CFTYPE_SHORT: *(S16 *)destination = (S16)*(const S64 *)value.data;
                        xcase CFTYPE_INT: *(S32 *)destination = (S32)*(const S64 *)value.data;
                        xcase CFTYPE_FLOAT:
                            *(float *)destination = value.type == CONTAINER_VALUE_FLOAT ? (float)*(const double *)value.data : (float)*(const S64 *)value.data;
                        xcase CFTYPE_ANSISTRING:
                            memcpy(destination, value.data, MIN(value.size, (size_t)field->num_bytes - 1));
                        xcase CFTYPE_UNICODESTRING:
                            MultiByteToWideChar(CP_UTF8, 0, value.data, (int)value.size, (WCHAR *)destination, field->num_bytes / sizeof(WCHAR) - 1);
                        xcase CFTYPE_DATETIME:
                        {
                            SQL_TIMESTAMP_STRUCT *timestamp = (SQL_TIMESTAMP_STRUCT *)destination;
                            sscanf(value.data, "%hd-%hu-%hu %hu:%hu:%hu", &timestamp->year, &timestamp->month, &timestamp->day,
                                &timestamp->hour, &timestamp->minute, &timestamp->second);
                        }
                        xdefault: assertmsgf(0, "Unsupported fixed SQLite result type %d", field->data_type);
                    }
                }
                offset += field->num_bytes;
            }
            ++count;
        }
        storage->ops->finalize(statement);
        if (result != DB_STORAGE_DONE)
            FatalErrorf("Unable to execute SQLite query: %s\n%s", error.message, cmd_buf[conn]);
        *count_ptr = count;
        return dyn_mem;
    }

    retcode = sqlConnStmtExecDirect(stmt, cmd_buf[conn], estrLength(&cmd_buf[conn]), conn, true);
    if (!SQL_SUCCEEDED(retcode))
    {
        *count_ptr = 0;
        return 0;
    }

    while (true)
    {
        int idx = 0;

        TODO(); // do we need this memset?
        memset(row_data[conn], 0, rec_size);

        retcode = _sqlConnStmtFetch(stmt, conn);
        if (retcode == SQL_NO_DATA)
            break;

        count++;
        if (retcode != SQL_SUCCESS)
            sqlConnStmtPrintError(stmt, cmd_buf[conn]);

        if(!SQL_SUCCEEDED(retcode))
            break;

        for(i = 0; i < col_count; i++)
        {
            void *data = s_getField(stmt, field_ptrs[i], i, &idx, conn, cmd_buf[conn]);
            if(!data)
                break;
        }
        if(i < col_count)
            break;

        dynArrayFit(&dyn_mem, 1, &dyn_mem_max, (count+1)*rec_size);
        memcpy(dyn_mem + dyn_mem_idx, row_data[conn], rec_size);
        dyn_mem_idx += rec_size;
    }

    sqlConnStmtFree(stmt);

    *count_ptr = count;

    return dyn_mem;
}

/** 
* Query the list of columns for a given table in the SQL
* database as an array of @ref ColumnInfo structures.
*/ 
int sqlGetTableInfo(char *table_name,ColumnInfo **columns_ptr)
{
    HSTMT stmt;
    int count;
    ColumnInfo *columns = NULL;
    char * schema_name = "dbo";

    SQLCHAR column_name[256];
    SQLLEN column_name_bytes;

    SQLSMALLINT data_type;
    SQLLEN data_type_bytes;

    SQLCHAR type_name[256];
    SQLLEN type_name_bytes;

    SQLINTEGER column_size;
    SQLLEN column_size_bytes;

    SQLINTEGER buffer_length;
    SQLLEN buffer_length_bytes;

    SQLINTEGER char_octet_length;
    SQLLEN char_octet_length_bytes;

    if (gDatabaseProvider == DBPROV_SQLITE)
    {
        char query[512];
        DbStorage *storage = dbGetStorage();
        DbStorageConnection *connection = storage->ops->connection(storage, SQLCONN_FOREGROUND);
        DbStorageError error = {0};
        DbStorageStatement *statement;

        sprintf(query, "PRAGMA table_info(\"%s\")", table_name);
        statement = storage->ops->prepare(connection, query, strlen(query), &error);
        if (!statement)
            FatalErrorf("Unable to inspect SQLite table %s: %s", table_name, error.message);
        DbStorageResult result;
        for (count = 0; (result = storage->ops->step(statement, &error)) == DB_STORAGE_ROW; ++count)
        {
            ContainerValue nameValue = dbStorageColumn(statement, 1);
            ContainerValue typeValue = dbStorageColumn(statement, 2);
            const char *typeName = typeValue.data;
            columns = realloc(columns, (count + 1) * sizeof(*columns));
            memset(&columns[count], 0, sizeof(columns[count]));
            snprintf(columns[count].name, sizeof(columns[count].name), "%.*s", (int)nameValue.size, (const char *)nameValue.data);
            snprintf(columns[count].data_type_name, sizeof(columns[count].data_type_name), "%.*s", (int)typeValue.size, typeName);
            if (!stricmp(typeName, "INTEGER") || !stricmp(typeName, "INT1") || !stricmp(typeName, "INT2")) {
                columns[count].data_type = !stricmp(typeName, "INT1") ? CFTYPE_BYTE : !stricmp(typeName, "INT2") ? CFTYPE_SHORT : CFTYPE_INT;
                columns[count].num_bytes = g_containerfieldinfo[columns[count].data_type].access_size;
            } else if (!stricmp(typeName, "REAL")) {
                columns[count].data_type = CFTYPE_FLOAT; columns[count].num_bytes = 4;
            } else if (!stricmp(typeName, "BLOB")) {
                columns[count].data_type = CFTYPE_BINARY_MAX; columns[count].num_bytes = -1;
            } else if (!stricmp(typeName, "DTEXT")) {
                columns[count].data_type = CFTYPE_DATETIME; columns[count].num_bytes = sizeof(SQL_TIMESTAMP_STRUCT);
            } else if (!stricmp(typeName, "TEXT") || !stricmp(typeName, "UTEXT")) {
                columns[count].data_type = *typeName == 'U' ? CFTYPE_UNICODESTRING_MAX : CFTYPE_ANSISTRING_MAX;
                columns[count].num_bytes = -1;
            } else if (!_strnicmp(typeName, "TEXT(", 5) || !_strnicmp(typeName, "UTEXT(", 6)) {
                bool unicode = toupper((unsigned char)*typeName) == 'U';
                char *end;
                long width = strtol(typeName + (unicode ? 6 : 5), &end, 10);
                if (width <= 0 || width >= INT_MAX / 2 || *end != ')' || end[1])
                    FatalErrorf("Unknown SQLite type '%s' in '%s'", typeName, table_name);
                columns[count].data_type = unicode ? CFTYPE_UNICODESTRING : CFTYPE_ANSISTRING;
                columns[count].column_size = (int)width;
                columns[count].num_bytes = ((int)width + 1) * (unicode ? 2 : 1);
            } else FatalErrorf("Unknown SQLite type '%s' for '%s.%s'", typeName, table_name, columns[count].name);
        }
        storage->ops->finalize(statement);
        if (result != DB_STORAGE_DONE)
            FatalErrorf("Unable to inspect SQLite table %s: %s", table_name, error.message);
        *columns_ptr = columns;
        return count;
    }

    stmt = sqlConnStmtAlloc(SQLCONN_FOREGROUND);

    switch (gDatabaseProvider)
    {
        xcase DBPROV_MSSQL:
            schema_name = schema_name;
            table_name = table_name;
        xcase DBPROV_POSTGRESQL:
            schema_name = strlwrdupa(schema_name);
            table_name = strlwrdupa(table_name);
        DBPROV_XDEFAULT();
    }

    if (!SQL_SUCCEEDED(SQLColumnsA(stmt, NULL, 0, schema_name, SQL_NTS, table_name, SQL_NTS, NULL, 0)))
    {
        sqlConnStmtFree(stmt);
        return 0;
    }

    assert(SQL_SUCCEEDED(SQLBindCol(stmt, 4, SQL_C_CHAR, column_name, ARRAY_SIZE(column_name), &column_name_bytes)));
    assert(SQL_SUCCEEDED(SQLBindCol(stmt, 5, SQL_C_SSHORT, &data_type, 0, &data_type_bytes)));
    assert(SQL_SUCCEEDED(SQLBindCol(stmt, 6, SQL_C_CHAR, type_name, ARRAY_SIZE(type_name), &type_name_bytes)));
    assert(SQL_SUCCEEDED(SQLBindCol(stmt, 7, SQL_C_SLONG, &column_size, 0, &column_size_bytes)));
    assert(SQL_SUCCEEDED(SQLBindCol(stmt, 8, SQL_C_SLONG, &buffer_length, 0, &buffer_length_bytes)));
    assert(SQL_SUCCEEDED(SQLBindCol(stmt, 16, SQL_C_SLONG, &char_octet_length, 0, &char_octet_length_bytes)));

    for(count=0; ; count++)
    {
        SQLRETURN retcode;
        int i;

        retcode = _sqlConnStmtFetch(stmt, SQLCONN_FOREGROUND);
        if (retcode == SQL_NO_DATA)
            break;

        if (retcode != SQL_SUCCESS)
            sqlConnStmtPrintErrorf(stmt, "SQLColumns <%s>", table_name);

        if (!SQL_SUCCEEDED(retcode))
            continue;

        assert(column_name_bytes > 0);
        assert(data_type_bytes > 0);
        assert(type_name_bytes > 0);
        assert(column_size_bytes > 0);
        assert(buffer_length_bytes > 0);

        // Override buffer length with octet length when necessary
        if (char_octet_length_bytes > 0 && char_octet_length > buffer_length)
            buffer_length = char_octet_length;

        columns = realloc(columns, (count+1) * sizeof(*columns));
        memset(&columns[count], 0, sizeof(*columns));

        strcpy(columns[count].name, column_name);

        columns[count].data_type = CFTYPE_NULL;
        columns[count].num_bytes = buffer_length;
        columns[count].column_size = 0;

        for (i = 1; i < CFTYPE_COUNT; i++)
        {
            if ((g_containerfieldinfo[i].actual_type == data_type) && ((g_containerfieldinfo[i].access_size == -1) == (column_size == 0 || column_size == INT_MAX)))
            {
                columns[count].data_type = i;
                break;
            }
        }
        if (i == CFTYPE_COUNT)
            FatalErrorf("Unknown type '%s' (%d) for column '%s' in '%s'", type_name, data_type, column_name, table_name);

        if (CFTYPE_IS_LEGACY(columns[count].data_type))
        {
            columns[count].num_bytes = -1;
            strcpy(columns[count].data_type_name, type_name);
        }
        else if (CFTYPE_IS_DYNAMIC(columns[count].data_type))
        {
            columns[count].num_bytes = -1;
            sprintf(columns[count].data_type_name, "%s(max)", type_name);
        }
        else if (CFTYPE_IS_ARRAY(columns[count].data_type))
        {
            columns[count].column_size = column_size;
            sprintf(columns[count].data_type_name, "%s(%d)", type_name, column_size);
        }
        else
        {
            strcpy(columns[count].data_type_name, type_name);
        }
    }

    sqlConnStmtFree(stmt);

    *columns_ptr = columns;
    return count;
}


void sqlDropForeignKeysForTable(char *table_name)
{
    if (gDatabaseProvider == DBPROV_SQLITE)
        return;
    SQLLEN    pktable_schem_ind,pktable_name_ind,pkcolumn_name_ind,
            fktable_schem_ind,fktable_name_ind,fkcolumn_name_ind,
            fkey_name_ind,pkey_name_ind,update_ind,delete_ind;
    char    pktable_schem_s[129],pktable_name_s[129],pkcolumn_name_s[129],
            fktable_schem_s[129],fktable_name_s[129],fkcolumn_name_s[129],
            fkey_name_s[129],pkey_name_s[129];
    short    update_rule,delete_rule;
    int        i,rc;
    SQLHSTMT      stmt;
    char    **fk_names=0;
    int        fk_count=0,fk_max=0;
    char    buf[SHORT_SQL_STRING];

    char * schema_name = "dbo";

    TODO(); // Code cleanup needed to improve clarity for future audits

    stmt = sqlConnStmtAlloc(SQLCONN_FOREGROUND);

    switch (gDatabaseProvider) {
        xcase DBPROV_MSSQL:
            schema_name = schema_name;
            table_name = table_name;
        xcase DBPROV_POSTGRESQL:
            schema_name = strlwrdupa(schema_name);
            table_name = strlwrdupa(table_name);
        DBPROV_XDEFAULT();
    }

    rc = SQLForeignKeysA(stmt, NULL, 0, schema_name, SQL_NTS, table_name, SQL_NTS, NULL, 0, NULL, 0, NULL, 0);
    rc = SQLBindCol(stmt, 2, SQL_C_CHAR, (SQLPOINTER) pktable_schem_s, 129, &pktable_schem_ind);
    rc = SQLBindCol(stmt, 3, SQL_C_CHAR, (SQLPOINTER) pktable_name_s, 129, &pktable_name_ind);
    rc = SQLBindCol(stmt, 4, SQL_C_CHAR, (SQLPOINTER) pkcolumn_name_s, 129, &pkcolumn_name_ind);
    rc = SQLBindCol(stmt, 6, SQL_C_CHAR, (SQLPOINTER) fktable_schem_s, 129, &fktable_schem_ind);
    rc = SQLBindCol(stmt, 7, SQL_C_CHAR, (SQLPOINTER) fktable_name_s, 129, &fktable_name_ind);
    rc = SQLBindCol(stmt, 8, SQL_C_CHAR, (SQLPOINTER) fkcolumn_name_s, 129, &fkcolumn_name_ind);
    rc = SQLBindCol(stmt, 10, SQL_C_SHORT, (SQLPOINTER) &update_rule,  0, &update_ind);
    rc = SQLBindCol(stmt, 11, SQL_C_SHORT, (SQLPOINTER) &delete_rule,  0, &delete_ind);
    rc = SQLBindCol(stmt, 12, SQL_C_CHAR, (SQLPOINTER) fkey_name_s, 129, &fkey_name_ind);
    rc = SQLBindCol(stmt, 13, SQL_C_CHAR, (SQLPOINTER) pkey_name_s, 129, &pkey_name_ind);

    //printf("Primary Key and Foreign Keys for %s.%s\n", "", tablename);
    /* Fetch each row, and display */
    while ((rc = _sqlConnStmtFetch(stmt, SQLCONN_FOREGROUND)) == SQL_SUCCESS)
    {
        if (fkey_name_ind > 0 )
        {
            sprintf(buf,"ALTER TABLE dbo.%s DROP CONSTRAINT %s;",fktable_name_s,fkey_name_s);
            dynArrayAddp(&fk_names, &fk_count, &fk_max, strdup(buf));
        }
    }

    sqlConnStmtFree(stmt);

    for(i=0;i<fk_count;i++)
    {
        sqlExecDdl(DDL_DEL, fk_names[i], SQL_NTS);
        free(fk_names[i]);
    }
    free(fk_names);
}

int sqlDropTable(char *table)
{
    char cmd[SHORT_SQL_STRING];
    int cmd_len;

    sqlDropForeignKeysForTable(table);
    cmd_len = gDatabaseProvider == DBPROV_SQLITE ? sprintf(cmd, "DROP TABLE %s;", table) : sprintf(cmd, "DROP TABLE dbo.%s;",table);
    if (stricmp(table,"Attributes")==0)
        FatalErrorf("ATTRIBUTE TABLE DROP BUG!!! call Paragon immediately!");
    return sqlExecDdl(DDL_REBUILDTABLE, cmd, cmd_len) != -1;
}

void sqlReadTableNamesFree(char **names, int count)
{
    int i;
    for(i=0;i<count;i++)
    {
        free(names[i]);
    }
    free(names);
}

char **sqlReadTableNames(int *countp)
{
    HSTMT stmt;
    U8 tabQualifier[255], tabOwner[255], tabName[255], tabType[255], remarks[255];
    SQLLEN lenTabQualifier, lenTabOwner, lenTableName, lenTableType, lenRemarks;
    int retcode;
    char    **table_names=0;
    int        table_count=0,table_max=0;

    char * schema_name = "dbo";

    TODO(); // Code cleanup needed to improve clarity for future audits

    if (gDatabaseProvider == DBPROV_SQLITE)
    {
        DbStorage *storage = dbGetStorage();
        DbStorageConnection *connection = storage->ops->connection(storage, SQLCONN_FOREGROUND);
        DbStorageError error = {0};
        const char query[] = "SELECT name FROM sqlite_master WHERE type='table' AND name NOT LIKE 'sqlite_%'";
        DbStorageStatement *statement = storage->ops->prepare(connection, query, sizeof(query) - 1, &error);
        DbStorageResult result;

        if (!statement)
            FatalErrorf("Unable to query SQLite table names: %s", error.message);
        while ((result = storage->ops->step(statement, &error)) == DB_STORAGE_ROW)
        {
            ContainerValue value = dbStorageColumn(statement, 0);
            char *name = malloc(value.size + 1);
            memcpy(name, value.data, value.size);
            name[value.size] = 0;
            dynArrayAddp(&table_names, &table_count, &table_max, name);
        }
        storage->ops->finalize(statement);
        if (result != DB_STORAGE_DONE)
            FatalErrorf("Unable to query SQLite table names: %s", error.message);
        *countp = table_count;
        return table_names;
    }

    stmt = sqlConnStmtAlloc(SQLCONN_FOREGROUND);

    switch (gDatabaseProvider) {
        xcase DBPROV_MSSQL:
            schema_name = schema_name;
        xcase DBPROV_POSTGRESQL:
            schema_name = strlwrdupa(schema_name);
        DBPROV_XDEFAULT();
    }

    retcode = SQLTablesA(stmt, NULL, 0, schema_name, SQL_NTS, NULL, 0, "TABLE", SQL_NTS);

    // Bind columns in result set to storage locations
    retcode = SQLBindCol(stmt, 1, SQL_C_CHAR, tabQualifier, 255, &lenTabQualifier);
    retcode = SQLBindCol(stmt, 2, SQL_C_CHAR, tabOwner, 255, &lenTabOwner);
    retcode = SQLBindCol(stmt, 3, SQL_C_CHAR, tabName, 255, &lenTableName);
    retcode = SQLBindCol(stmt, 4, SQL_C_CHAR, tabType, 255, &lenTableType);
    retcode = SQLBindCol(stmt, 5, SQL_C_CHAR, remarks, 255, &lenRemarks);
    // print out the records in the result set

    while ((retcode = _sqlConnStmtFetch(stmt, SQLCONN_FOREGROUND)) == SQL_SUCCESS)
    {
        dynArrayAddp(&table_names, &table_count, &table_max, _strdup(tabName));
    }

    sqlConnStmtFree(stmt);

    *countp = table_count;
    return table_names;
}

/** 
* Validates whether a Data Definition Language (DDL)
* command of the specified type is allowed by the server.
*/
void sqlCheckDdl(DdlType type)
{
    char    *name = "Unknown";

    // this one trumps everything, since you don't want to drop the table, then find out you're not allowed to re-add it
    if (server_cfg.ddl_rebuildtable || server_cfg.sql_allow_all_ddl)
        return;

    switch(type)
    {
        xcase DDL_ATTRIBUTES:
            name = "SqlAddAttributes";
            if (server_cfg.ddl_attributes)
                return;
        xcase DDL_ADD:
            name = "SqlAddColumnOrTable";
            if (server_cfg.ddl_add)
                return;
        xcase DDL_DEL:
            name = "SqlDeleteColumnOrTable";
            if (server_cfg.ddl_del)
                return;
        xcase DDL_REBUILDTABLE:
            name = "SqlRebuildTable";
            if (server_cfg.ddl_rebuildtable)
                return;
        xcase DDL_ALTERCOLUMN:
            name = "SqlAlterColumn";
            if (server_cfg.ddl_altercolumn)
                return;
    }

    FatalErrorf("The previous operation, \"%s\" needed to modify the structure of the\n"
                "database. Since that is disabled, the only thing to do now is quit.\n"
                "Set \"%s 1\" in servers.cfg if you want to allow it.\n"
                "allow it.",name,name);
}

/** 
* Build various test strings to validate that a string of
* special characters can fit in the SQL database table column.
*/ 
static char * testString(char * string, size_t len, bool unicode, bool min)
{
    static const char test_utf8[4] = {0xF0, 0xA4, 0xAD, 0xA2};

    char * s = string;
    size_t i;

    if (min) {
        memset(string, '$', len);
        string[len] = 0;
    } else if (!unicode) {
        for(i=0; i<len; i++) {
            *(s++) = '\\';
            *(s++) = '\\';
        }
        *s = 0;
    } else {
        for(i=0; i<len; i++) {
            *(s++) = test_utf8[0];
            *(s++) = test_utf8[1];
            *(s++) = test_utf8[2];
            *(s++) = test_utf8[3];
        }
        *s = 0;
    }
    return string;
}

/**
* Verify minimum and maximum values for each 
* @ref ContainerFieldType.
*/
static char * testDataBaseValues(ContainerTemplate * tplt, bool min)
{
    char * buf;
    int i;

    estrCreate(&buf);
    estrPrintCharString(&buf, "Active 1\n");

    for(i=0; i<tplt->table_count; i++)
    {
        TableInfo * table = &tplt->tables[i];
        int j;

        for (j=0; j<table->num_columns; j++)
        {
            char tmp[16384];
            ColumnInfo * field = &table->columns[j];                
            if (field->reserved_word)
                continue;

            // HACK
            if (strcmp(field->name, "test_attr")==0)
            {
                estrConcatf(&buf, "%s \"%s\"\n", field->name, min ? "0" : "class_blaster");
                continue;
            }

            assert(sizeof(tmp) > field->column_size * 4 + 1);
            switch (field->data_type)
            {
                xcase CFTYPE_BYTE: estrConcatf(&buf, "%s %d\n", field->name, min ? 1 : UCHAR_MAX);
                xcase CFTYPE_SHORT: estrConcatf(&buf, "%s %d\n", field->name, min ? SHRT_MIN : SHRT_MAX);
                xcase CFTYPE_INT: estrConcatf(&buf, "%s %d\n", field->name, min ? INT_MIN : INT_MAX);
                xcase CFTYPE_FLOAT: estrConcatf(&buf, "%s %s\n", field->name, safe_ftoa(min ? -(FLT_EPSILON*10.0f) : FLT_MAX, tmp));
                xcase CFTYPE_DATETIME: {
                        int time = min ? timerGetSecondsSince2000FromString("2001-01-01 00:00:00") : timerSecondsSince2000();
                        estrConcatf(&buf, "%s \"%s\"\n", field->name, timerMakeDateStringFromSecondsSince2000(tmp, time));
                }
                xcase CFTYPE_UNICODESTRING: estrConcatf(&buf, "%s \"%s\"\n", field->name, testString(tmp, min ? field->column_size : field->column_size/4, true, min));
                xcase CFTYPE_ANSISTRING: estrConcatf(&buf, "%s \"%s\"\n", field->name, testString(tmp, min ? field->column_size : field->column_size/2, false, min));
                xcase CFTYPE_UNICODESTRING_MAX: estrConcatf(&buf, "%s \"%s\"\n", field->name, testString(tmp, sizeof(tmp)/4-1, true, min));
                xcase CFTYPE_ANSISTRING_MAX: estrConcatf(&buf, "%s \"%s\"\n", field->name, testString(tmp, sizeof(tmp)/2-1, false, min));
                xcase CFTYPE_BINARY_MAX: {
                    char escaped[16384*2+1];
                    int escaped_len = sizeof(escaped);
                    int k;
                    int len = min ? 1 : sizeof(tmp);

                    for (k=0; k<len; k++) tmp[k]=k;

                    escapeDataToBuffer(escaped, tmp, len, &escaped_len);
                    escaped[escaped_len] = 0;

                    estrConcatf(&buf, "%s \"%s\"\n", field->name, escaped);
                }
                xdefault: assert(0);
            }
        }
    }

    return buf;
}

/** 
* Run a single test of SQL database types using the container
* text provided.
*/ 
static void testContainerLoadSave(struct DbList * list, int cid, DbContainer ** container, char * text)
{
    // Test update
    containerUpdate(list, cid, text, 1);
    assert(!containerDiff(text, containerGetText(*container))[0]);

    // Test reload
    containerUnload(list, cid);
    *container = containerLoad(list, cid);
    assert(*container);
    assert(!containerDiff(text, containerGetText(*container))[0]);

    // Test no change update
    containerUpdate(list, cid, text, 1);
    assert(!containerDiff(text, containerGetText(*container))[0]);
}

/**
* Verify that database types are working correctly on startup.
*/
void testDataBaseTypes(struct DbList * list)
{
    int i;
    char * text_empty = "Active 1";
    char * text_min = testDataBaseValues(list->tplt, 1);
    char * text_max = testDataBaseValues(list->tplt, 0);

    loadstart_printf("verifying that the database is working correctly");

    for (i=0; i<10; i++)
    {
        DbContainer * container = containerAlloc(list, -1);
        int cid = container->id;;

        testContainerLoadSave(list, cid, &container, text_empty);
        testContainerLoadSave(list, cid, &container, text_min);
        testContainerLoadSave(list, cid, &container, text_max);

        containerDelete(container);
    }

    estrDestroy(&text_min);
    estrDestroy(&text_max);

    loadend_printf("");
}
