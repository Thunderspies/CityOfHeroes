#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/file.h>
#include <utilitieslib/utils/error.h>
#include "dbserver/servercfg.h"
#include <utilitieslib/utils/timing.h>
#include "container_tplt.h"
#include "container_tplt_utils.h"
#include "container_merge.h"
#include "container.h"
#include <utilitieslib/components/StashTable.h>
#include <utilitieslib/utils/sysutil.h>
#include <utilitieslib/components/EString.h>
#include <utilitieslib/components/earray.h>
#include "sql_fifo.h"
#include <utilitieslib/utils/log.h>
#include <utilitieslib/utils/genericDialog.h>
#include <utilitieslib/utils/mathutil.h>
#include "sql/sqlinclude.h" // for MS SQL 2005 HACK

typedef struct
{
    char    table[256];
    char    key[256];
    char    foreign_table[256];
} ForeignKey;

ForeignKey *foreign_keys;
int foreign_key_count,foreign_key_max;

static StashTable referenced_tables;

void tpltRegisterForeignKeyConstraint(char *table, char *key, char *foreign_table)
{
    ForeignKey    *fk;

    fk = dynArrayAdd(&foreign_keys,sizeof(foreign_keys[0]),&foreign_key_count,&foreign_key_max,1);
    strcpy(fk->table,table);
    strcpy(fk->key,key);
    strcpy(fk->foreign_table,foreign_table);
}

void tpltSetAllForeignKeyConstraintsAsync(void)
{
    int            i;
    ForeignKey    *fk;

    for(i=0;i<foreign_key_count;i++)
    {
        fk = &foreign_keys[i];
        sqlAddForeignKeyConstraintAsync(fk->table, fk->key, fk->foreign_table);
    }
}

TableInfo *tpltFindTable(ContainerTemplate *tplt, char *name)
{
    int        i;

    for(i=0;i<tplt->table_count;i++)
    {
        if (stricmp(tplt->tables[i].name,name)==0)
            return &tplt->tables[i];
    }
    return 0;
}

static void setField(ColumnInfo *field, const char *name, enum ContainerFieldType data_type, int bytes, int column_size, const char *data_type_name)
{
    memset(field, 0, sizeof(*field));
    strcpy(field->name, name);
    field->data_type = data_type;
    field->num_bytes = bytes;
    field->column_size = column_size;
    if (column_size)
        sprintf(field->data_type_name, "%s(%d)", data_type_name, column_size);
    else
        strcpy(field->data_type_name, data_type_name);
}

void hashColumnNames(TableInfo *table)
{
    int        i;

    if (table->name_hashes)
        stashTableDestroy(table->name_hashes);
    table->name_hashes = stashTableCreateWithStringKeys(table->num_columns * 2, StashDeepCopyKeys);

    for(i=0;i<table->num_columns;i++)
        stashAddInt(table->name_hashes,table->columns[i].name,i, false);
}

void hashAllNames(ContainerTemplate *tplt, const char * fname)
{
    int            i,j,k,count=1,max_slots = 0;
    TableInfo    *table;
    char        buf[1000];
    int stash_size = 0;

    // count the number of possible lines
    for(i=0;i<tplt->table_count;i++)
    {
        table = &tplt->tables[i];
        stash_size += MIN(table->array_count,1) * table->num_columns;
    }

    if (tplt->all_hashes)
        stashTableDestroy(tplt->all_hashes);
    tplt->all_hashes = stashTableCreateWithStringKeys(stashOptimalSize(stash_size), StashDeepCopyKeys);

    for(i=0;i<tplt->table_count;i++)
    {
        table = &tplt->tables[i];
        table->all_hash_first_idx = count;
        hashColumnNames(table);
        for(k=0;k<table->array_count || k==0;k++)
        {
            assert(table->num_columns <= MAX_QUERY_RESULTS);
            for(j=0;j<table->num_columns;j++)
            {
                if (!table->array_count)
                    strcpy(buf, table->columns[j].name);
                else if (table->columns[j].idx_by_attr)
                {
                    const char *s;

                    if (k < eaSize(&table->columns[j].attr->names) && k > 0)
                        s = table->columns[j].attr->names[k];
                    else
                        s = "";
                    sprintf(buf,"%s[%s].%s",table->name,s,table->columns[j].name);
                }
                else
                    sprintf(buf,"%s[%d].%s",table->name,k,table->columns[j].name);
                if (count >= max_slots)
                {
                    if (!max_slots)
                        max_slots = 100;
                    max_slots *= 2;
                    tplt->slots = realloc(tplt->slots,sizeof(SlotInfo) * max_slots);
                }
                tplt->slots[count].idx = j;
                tplt->slots[count].sub_id = k;
                tplt->slots[count].table = table;
                //strcpy(tplt->slots[count].test_name,buf);
                stashAddInt(tplt->all_hashes,buf,count++, false);
            }
        }
    }
    if (tplt->lines)
        free(tplt->lines);
    tplt->max_lines = count;
    tplt->lines = calloc(count,sizeof(tplt->lines[0]));
}

ContainerTemplate *tpltLoad(int dblist_id,char *table_name,char *fname)
{
    char* strtype,*mem;
    int count, type, data_len, is_attr;
    ContainerTemplate* tplt;
    ColumnInfo* field = NULL;
    TableInfo* table = 0;
    CtnrLineState state;
    int column_size;

    TODO(); // Code cleanup needed to improve clarity for future audits

    mem = fileAlloc(fname,0);
    if (!mem)
        return NULL;

    tplt = calloc(sizeof(ContainerTemplate),1);
    ctnrLineInit(&state,mem,table_name);
    tplt->dblist_id = dblist_id;

    while(ctnrLineGet(&state))
    {
        is_attr = 0;
        count = state.count - 1;
        if (count < 1)
            continue;

        type = dataType(state.args[1], &column_size, &data_len, &strtype);
        if (stricmp(state.args[1],"attribute")==0)
            is_attr = 1;

        table = tpltFindTable(tplt,state.table);
        if (!table)
        {
            tplt->table_count++;
            tplt->tables = realloc(tplt->tables,tplt->table_count * sizeof(tplt->tables[0]));
            table = &tplt->tables[tplt->table_count-1];
            memset(table,0,sizeof(*table));
            strcpy(table->name,state.table);

            // ContainerId
            field = table->columns = realloc(0,1 * sizeof(table->columns[0]));
            setField(field, "ContainerId", CFTYPE_INT, sizeof(int), 0, getContainerFieldType(CFTYPE_INT));
            field->reserved_word = 1;
            table->num_columns = 1;

            // SubId or Active
            field = table->columns = realloc(field,2 * sizeof(*field));
            memmove(&field[1],&field[0],sizeof(*field));
            table->num_columns = 2;
            if (stricmp(table_name,state.table)!=0)
            {
                strcpy(field[1].name,"SubId");
                field[1].is_sub_id_field = 1;
            }
            else
            {
                strcpy(field[1].name,"Active");
                if (gDatabaseProvider == DBPROV_MSSQL)
                    strcpy(field[0].data_type_name,"int identity");
            }
        }
        if (1 || state.table_idx <= 0)
        {
            extern AttributeList *var_attrs;

            // MS SQL 2008 R2 Native Client 10.0 has no support for
            // SQL_GD_ANY_COLUMN so unbound columns must appear after the last
            // bound column.  We could relax this restriction if we change
            // sqlTableSelect() to explicitly request unbounded columns last.
            if(!CFTYPE_IS_DYNAMIC(type) && table->num_columns)
                assertmsgf(!CFTYPE_IS_DYNAMIC(table->columns[table->num_columns-1].data_type),
                    "Out of order unbounded column found %s in %s.\nAll unbounded (max) columns must be at the end of the table unless the database supports SQL_GD_ANY_COLUMN.", field->name, table->name);

            table->num_columns++;
            table->columns = realloc(table->columns,table->num_columns * sizeof(table->columns[0]));

            field = &table->columns[table->num_columns-1];
            memset(field,0,sizeof(*field));

            field->data_type = type;
            field->column_size = column_size;
            field->num_bytes = data_len;

            strcpy(field->name,state.field);
            strcpy(field->data_type_name, strtype);

            if (is_attr)
                field->attr = var_attrs;
            if (state.count > 2 && stricmp(state.args[2],"indexed")==0)
                field->indexed = 1;
        }

        table->array_count = state.table_idx;
        if (table->array_count <= 0) {
            table->table_type = TT_CONTAINER;
            table->array_count = 0;
        } else {
            table->table_type = TT_SUBCONTAINER;
        }
    }
    cntrLineFree(&state);
    free(mem);
    hashAllNames(tplt, fname);
    {
        int        i,j;

        for(i=1;i<tplt->table_count;i++)
            tpltRegisterForeignKeyConstraint(tplt->tables[i].name,"ContainerId", tplt->tables[0].name);
        for(i=0;i<tplt->table_count;i++)
        {
            for(j=0;j<tplt->tables[i].num_columns;j++)
            {
                if (tplt->tables[i].columns[j].attr)
                {
                    if (tplt->tables[i].columns[j].idx_by_attr)
                        tpltRegisterForeignKeyConstraint(tplt->tables[i].name,"SubId","Attributes");
                    else
                        tpltRegisterForeignKeyConstraint(tplt->tables[i].name,tplt->tables[i].columns[j].name,"Attributes");
                }
            }
        }
    }
    return tplt;
}

static bool verifyTableColumns(TableInfo *table)
{
    int i;
    int count;
    ColumnInfo * field_descs = NULL;
    int * rebuild_columns = NULL;

    count = sqlGetTableInfo(table->name, &field_descs);

    eaiCreate(&rebuild_columns);

    // Check for inserts, data mismatch
    for (i=0; i<count && i<table->num_columns; i++)
    {
        ColumnInfo * field = &field_descs[i];
        ColumnInfo * new_field = &table->columns[i];

        if (stricmp(new_field->name, field->name)!=0)
        {
            Errorf("New field %s.%s %s added to middle of list at %s", table->name, new_field->name, new_field->data_type_name, field->name);
            eaiDestroy(&rebuild_columns);
            free(field_descs);
            sqlFifoFinish();
            return false;
        }

        if (stricmp(new_field->data_type_name, field->data_type_name)!=0)
        {
            char index[SHORT_SQL_STRING];

            LOG(LOG_DEBUG_LOG, LOG_LEVEL_IMPORTANT, LOG_CONSOLE, "Mismatch for field: %s.%s (old: %s, new: %s)", table->name, field->name, field->data_type_name, new_field->data_type_name);

            sprintf(index, "%s_ind", field->name);
            sqlRemoveIndexAsync(index, table->name);

            eaiPush(&rebuild_columns, i);
            continue;
        }

        // Don't validate deprecated types
        if (new_field->data_type == CFTYPE_TEXTBLOB || new_field->data_type == CFTYPE_BLOB)
            continue;

        assert(new_field->num_bytes >= field->num_bytes);
        assert(new_field->column_size == field->column_size);
    }

    if (eaiSize(&rebuild_columns))
    {
        sqlFifoFinish();
            
        while (eaiSize(&rebuild_columns))
        {
            int i = eaiPop(&rebuild_columns);
            ColumnInfo * field = &field_descs[i];
            ColumnInfo * new_field = &table->columns[i];

            printf_stderr("Fixing %s.%s ...", table->name, field->name);

            if (!sqlChangeFieldType(table->name, field->name, new_field->data_type_name))
            {
                Errorf("\nFailed to change field type: %s.%s %s -> %s\n", table->name, new_field->name, field->data_type_name, new_field->data_type_name);
                eaiDestroy(&rebuild_columns);
                free(field_descs);
                return false;
            }

            printf_stderr("Done\n");
        }
    }

    eaiDestroy(&rebuild_columns);
    free(field_descs);
    return true;
}

static bool updateTable(TableInfo *table)
{
    int i;
    int old_count;
    ColumnInfo * field_descs = NULL;

    old_count = sqlGetTableInfo(table->name, &field_descs);

    // Check for deletes
    for (i=0; i<old_count; i++)
    {
        int idx;
        ColumnInfo * field = &field_descs[i];

        if (!stashFindInt(table->name_hashes, field->name, &idx))
        {
            LOG(LOG_DEBUG_LOG, LOG_LEVEL_IMPORTANT, LOG_CONSOLE, "Deleting old field: %s.%s\n", table->name, field->name);
            
            sqlRemoveForeignKeyConstraintAsync(table->name, field->name, "Attributes");
            sqlDropForeignKeysForTable(table->name);
            sqlFifoFinish();

            if (!sqlDeleteField(table->name, field->name, field->data_type_name)) {
                Errorf("Failed to delete field: %s.%s\n", table->name, field->name);
                return false;
            }

            memmove(&field_descs[i], &field_descs[i+1], (old_count-i-1) * sizeof(field_descs[0]));
            i--;
            old_count--;
        }
    }

    if (old_count && !verifyTableColumns(table))
        return false;

    if (!old_count && !sqlCreateTable(table->name, table->table_type))
        return false;

    // Add any new columns to the end
    for (i=old_count; i<table->num_columns; i++)
    {
        ColumnInfo * new_field = &table->columns[i];

        if (!new_field->reserved_word)
        {
            if (old_count)
                LOG(LOG_DEBUG_LOG, LOG_LEVEL_IMPORTANT, LOG_CONSOLE, "Adding field %s.%s %s\n", table->name, new_field->name, new_field->data_type_name);

            if (!sqlAddField(table->name, new_field->name, new_field->data_type_name)) {
                Errorf("Failed to add field: %s.%s\n", table->name, new_field->name);
                return false;
            }
        }
    }

    // Add any required indices
    for (i=0; i<table->num_columns; i++)
    {
        ColumnInfo * field = &table->columns[i];

        field->exists_in_sql = 1;

        if (field->indexed)
        {
            char index[SHORT_SQL_STRING];
            sprintf(index, "%s_ind", field->name);
            sqlAddIndexAsync(index, table->name, field->name);
        }
    }

    free(field_descs);

    return verifyTableColumns(table);
}

void tpltAddMembership(int memberlist_id, ContainerTemplate *group)
{
    DbList *memberlist = dbListPtr(memberlist_id);
    assertmsg(!group->member_id, "container already has membership registered");
    group->member_id = memberlist_id;
    sprintf(group->member_field_name, "%sId", group->tables->name);

    if(memberlist->tplt)
    {
        int i, j;
        assert(memberlist->special_columns);
        for(i = 0; memberlist->special_columns[i].name; i++)
        {
            SpecialColumn *column = &memberlist->special_columns[i];
            if(column->cmd == CMD_MEMBER && column->member_of == group->dblist_id)
            {
                char *s = strchr(column->name, '[');
                assertmsg(!group->member_table, "group container member has multiple matching special columns");
                if(s)
                {
                    char *buf = estrTemp();
                    estrConcatFixedWidth(&buf, column->name, s - column->name);
                    for(j = 1; j < memberlist->tplt->table_count; j++)
                    {
                        if(0 == stricmp(buf, memberlist->tplt->tables[j].name))
                        {
                            group->member_table = &memberlist->tplt->tables[j];
                            break;
                        }
                    }
                    assertmsg(group->member_table, "group container member has no matching table");
                }
                else
                {
                    group->member_table = memberlist->tplt->tables;
                }
            }
        }
        assertmsg(group->member_table, "group container member has no matching special columns");
    }
}

static int readAllTableData(ContainerTemplate *tplt,char *table_name,char ***multi_buf_ptr,TableInfo *new_table)
{
    TableInfo table;
    int    i;

    strcpy(table.name, table_name);
    table.num_columns = sqlGetTableInfo(table_name, &table.columns);
    if (!table.num_columns)
        return 0;

    table.array_count = 0;

    // Mark the ContainerId as reserved
    table.columns[0].reserved_word = 1;

    // Mark the SubId as reserved if this table has one
    if (strcmp(table.columns[1].name,"SubId")==0)
    {
        table.array_count = 1;
        table.columns[1].reserved_word = 1;
    }

    // Copy the table attributes
    for(i=0;i<table.num_columns;i++)
    {
        int j;

        for(j=0;j<new_table->num_columns;j++)
        {
            if (stricmp(table.columns[i].name, new_table->columns[j].name)==0)
            {
                table.columns[i].idx_by_attr = new_table->columns[j].idx_by_attr;
                table.columns[i].attr = new_table->columns[j].attr;
                break;
            }
        }
    }

    return sqlTableReadMulti(tplt, &table, -1, multi_buf_ptr, SQLCONN_FOREGROUND);
}

static bool rebuildTableDataInPlace(ContainerTemplate *tplt, TableInfo *table)
{
    TableInfo tmpTable;
    int i;
    char * columns = NULL;
    char identity_insert_on[SHORT_SQL_STRING] = {0};
    char identity_insert_off[SHORT_SQL_STRING] = {0};
    ColumnInfo * field_descs = NULL;
    int old_count;
    bool ret;

    // Setup the temporary table
    memcpy(&tmpTable, table, sizeof(TableInfo));
    strcat(tmpTable.name, "_tmp");

    sqlCheckDdl(DDL_REBUILDTABLE);
    printf("Creating tempoary table %s for in place rebuild\n", tmpTable.name);
    updateTable(&tmpTable);

    old_count = sqlGetTableInfo(table->name, &field_descs);

    // Build the list of columns to copy
    estrCreate(&columns);
    for (i=0; i<old_count; i++) {
        int idx;

        if (!stashFindInt(table->name_hashes, field_descs[i].name, &idx))
            continue;

        estrConcatf(&columns, "%s,", table->columns[idx].name);
    }
    estrSetLength(&columns, estrLength(&columns)-1);
    assert(estrLength(&columns));

    if (table->table_type == TT_CONTAINER)
    {
        sprintf(identity_insert_on, " SET IDENTITY_INSERT dbo.%s_tmp ON;", table->name);
        sprintf(identity_insert_off, " SET IDENTITY_INSERT dbo.%s_tmp OFF;", table->name);
    }

    // Copy to temporary table
    ret = sqlExecDdlf(DDL_REBUILDTABLE,
        "TRUNCATE TABLE dbo.%s_tmp;%s INSERT INTO dbo.%s_tmp (%s) SELECT %s FROM dbo.%s;%s",
        table->name, identity_insert_on, table->name, columns, columns, table->name, identity_insert_off);

    if (ret)
    {
        // Drop original table
        sqlDropTable(table->name);

        // Create new table
        updateTable(table);

        if (table->table_type == TT_CONTAINER)
        {
            sprintf(identity_insert_on, " SET IDENTITY_INSERT dbo.%s ON;", table->name);
            sprintf(identity_insert_off, " SET IDENTITY_INSERT dbo.%s OFF;", table->name);
        }

        // Copy to final location
        ret = sqlExecDdlf(DDL_REBUILDTABLE,
            "%s INSERT INTO dbo.%s (%s) SELECT %s FROM dbo.%s_tmp;%s",
            identity_insert_on, table->name, columns, columns, table->name, identity_insert_off);
    }

    estrDestroy(&columns);
    return ret;
}

static void rebuildTableDataBruteForce(ContainerTemplate *tplt,TableInfo *table)
{
    int count;
    char **multi_buf;
    int minrows=1;
    int i;

    printf("Reading table %s into memory\n", table->name);
    count = readAllTableData(tplt, table->name, &multi_buf, table);

    sqlCheckDdl(DDL_REBUILDTABLE);
    printf("Dropping table %s and rebuilding\n", table->name);
    sqlDropTable(table->name);
    updateTable(table);

    if (table != &tplt->tables[0])
        minrows++;

    for(i=0; i<count; i++)
    {
        static LineList    full_list;

        int container_id;
        char buf[1000];
        char *s;        
        char *data;

        //tpltScrubData(tplt,data);
        memToLineList(multi_buf[i], &full_list);
        data = lineListToText(tplt, &full_list, 0);
        s = findFieldText(data, "ContainerId", buf);
        if (!s)
            continue;

        container_id = atoi(s);
        s = strchr(data,'\n') + 1; // skip past ContainerId

        textToLineList(tplt, s, &full_list, 0);
        if (full_list.count >= minrows)
            sqlContainerUpdateFromScratchAsync(tplt, container_id, &full_list, 1);

        free(multi_buf[i]);
        if (!(i&(512-1)))
        {
            printf(" inserting into %s %d/%d         \r", table->name, i, count);
            fflush(fileWrap(stdout));
        }
    }
    printf(" finished %s %d/%d                  \n\n", table->name, i, count);
    if (multi_buf)
        free(multi_buf);
}

static void markTableReferenced(char *name)
{
    if (!referenced_tables)
        referenced_tables = stashTableCreateWithStringKeys(100, StashDeepCopyKeys);

    stashAddInt(referenced_tables, name, 0, false);
}

void tpltDropUnreferencedTables(void)
{
    int i;
    int count;
    char **names = sqlReadTableNames(&count);

    for(i=0; i<count; i++)
    {
        int index;

        if (!stashFindInt(referenced_tables, names[i], &index))
        {
            printf("Dropping unreferenced table: %s\n",names[i]);
            sqlDropTable(names[i]);
        }
    }
    sqlReadTableNamesFree(names, count);
}

void tpltUpdateSqlcolumns(ContainerTemplate *tplt)
{
    int j;
    char buf[SHORT_SQL_STRING];
    int buf_len;

    assert(tplt);

    for(j=0;j<tplt->table_count;j++)
    {
        if (!updateTable(&tplt->tables[j]))
        {
            if(!rebuildTableDataInPlace(tplt, &tplt->tables[j]))
            {
                static bool s_oktoall = false;
                if (!s_oktoall) {
                    switch(okToAllCancelDialog("Falling back to the old method for rebuilding a table.\r\n\r\n"
                                               "This will take a LONG time if the table is large.\r\n\r\n"
                                               "Do you wish to continue?", "Database table rebuild needed"))
                    {
                    case IDOKTOALL:
                        s_oktoall = true;
                    case IDOK:
                        break;
                    default:
                        exit(1); 
                    }
                }

                rebuildTableDataBruteForce(tplt, &tplt->tables[j]);
            }
        }
        sqlFifoTick();
    }

    sqlFifoBarrier();
    buf_len = sprintf(buf, "UPDATE dbo.%s SET Active = 0 WHERE Active > 0;", tplt->tables->name);
    sqlExecAsync(buf, buf_len);

    for(j=0;j<tplt->table_count;j++)
        markTableReferenced(tplt->tables[j].name);

    sqlFifoTick();
}

static bool loadAttributesFromFile(const char * fname, AttributeList * attr)
{
    int count;
    char *args[2];
    char *mem = fileAlloc(fname,0);
    char *s = mem;

    eaCreateWithCapacityConst(&attr->names, 1024);
    attr->hash_table = stashTableCreateWithStringKeys(10000, StashDeepCopyKeys);

    while((count=tokenize_line(s,args,&s)))
    {
        int idx;
        StashElement element;

        if (count != 2)
        {
            FatalErrorf("Error parsing line in '%s'!\n", fname);
            return false;
        }

        if(strHasSQLEscapeChars(args[1]))
        {
            FatalErrorf("Bad attribute name \"%s\" in vars.attribute for index \"%s\" - fix the name!\n", args[1], args[0]);
            return false;
        }

        idx = atoi(args[0]);
        stashAddIntAndGetElement(attr->hash_table, _strlwr(args[1]), idx, false, &element);
        eaSetForcedConst(&attr->names, stashElementGetStringKey(element), idx);
    }

    free(mem);
    return true;
}

static int verifyAttributesTable(const char *tablename, AttributeList * attr)
{
    int i;
    int idx;
    TableInfo table;
    ColumnInfo columns[2];
    char *cols;
    int col_count;
    ColumnInfo *field_ptrs[3];
    bool retried = false;
    int attr_id = 0;
    int prev_attr_id;

    ZeroStruct(&table);
    ZeroArray(columns);

    strcpy(table.name, tablename);
    table.table_type = TT_ATTRIBUTE;
    table.num_columns = 2;
    table.columns = columns;

    setField(&table.columns[0], "Id", CFTYPE_INT, sizeof(int), 0, getContainerFieldType(CFTYPE_INT));
    table.columns[0].reserved_word = 1;

    setField(&table.columns[1], "Name", CFTYPE_ANSISTRING, MAX_ATTRIBNAME_LEN, MAX_ATTRIBNAME_LEN, getContainerFieldType(CFTYPE_ANSISTRING));

    hashColumnNames(&table);
    updateTable(&table);

retry:
    cols = sqlReadColumnsSlow(&table, 0, "Id, Name", "ORDER BY Id", &col_count, field_ptrs);
    idx = 0;
    prev_attr_id = 0;

    for(i=0; i<col_count; i++)
    {
        char *name;

        attr_id = *(int *)(&cols[idx]);
        idx += field_ptrs[0]->num_bytes;

        name = &cols[idx];
        idx += field_ptrs[1]->num_bytes;

        // Find and repair holes
        for (++prev_attr_id; prev_attr_id != attr_id; prev_attr_id++)
        {
            char cmd[SHORT_SQL_STRING];
            int cmd_len;
            HSTMT stmt;

            if (prev_attr_id >= eaSize(&attr->names))
            {
                Errorf("Hole between values from sql server (Id=%d, Name=%s) does not exist in %s!\n(Hopefully this just means the code was rolled back.)", prev_attr_id, name, tablename);
                continue;
            }

            Errorf("Repairing hole in attributes (Id=%d, Name=%s)", prev_attr_id, name);

            stmt = sqlConnStmtAlloc(SQLCONN_FOREGROUND);
            cmd_len = sprintf(cmd, "INSERT INTO dbo.%s (Id, Name) VALUES (?, ?)", tablename);
            bindInputParameter(stmt, 0, CFTYPE_INT, &prev_attr_id, NULL);
            bindInputParameter(stmt, 1, CFTYPE_ANSISTRING, attr->names[prev_attr_id], NULL);
            sqlConnStmtExecDirect(stmt, cmd, cmd_len, SQLCONN_FOREGROUND, false);
            sqlConnStmtFree(stmt);
        }
        assert(prev_attr_id == attr_id);

        if (attr_id >= eaSize(&attr->names))
        {
            Errorf("Value from sql server (Id=%d, Name=%s) does not exist in %s!\n(Hopefully this just means the code was rolled back.)", attr_id, name, tablename);
        }
        else if (stricmp(attr->names[attr_id], name)!=0)
        {
            char temp[MAX_ATTRIBNAME_LEN];

            strcpy(temp, name);
            forwardSlashes(temp);

            if(stricmp(attr->names[attr_id], temp)==0)
            {
                char cmd[SHORT_SQL_STRING];
                int cmd_len;
                HSTMT stmt;

                Errorf("Adjusting slashes in attributes (Id=%d, Name=%s)", attr_id, name);

                stmt = sqlConnStmtAlloc(SQLCONN_FOREGROUND);
                cmd_len = sprintf(cmd, "UPDATE dbo.%s SET Name=? WHERE Id = ?", tablename);
                bindInputParameter(stmt, 0, CFTYPE_ANSISTRING, attr->names[attr_id], NULL);
                bindInputParameter(stmt, 1, CFTYPE_INT, &attr_id, NULL);
                sqlConnStmtExecDirect(stmt, cmd, cmd_len, SQLCONN_FOREGROUND, false);
                sqlConnStmtFree(stmt);

                goto retry;
            }
            else if (stricmp(attr->names[attr_id],name)!=0) 
            {
                char cmd[SHORT_SQL_STRING];
                int cmd_len;

                if (!name[0]) {
                    HSTMT stmt;

                    Errorf("Empty name in attributes (Id=%d, Name=%s)", attr_id, name);

                    // Fix missing name
                    stmt = sqlConnStmtAlloc(SQLCONN_FOREGROUND);
                    cmd_len = sprintf(cmd, "UPDATE dbo.%s SET Name=? WHERE Id = ?", tablename);
                    bindInputParameter(stmt, 0, CFTYPE_ANSISTRING, attr->names[attr_id], NULL);
                    bindInputParameter(stmt, 1, CFTYPE_INT, &attr_id, NULL);
                    sqlConnStmtExecDirect(stmt, cmd, cmd_len, SQLCONN_FOREGROUND, false);
                    sqlConnStmtFree(stmt);
                    continue;
                }

                Errorf("Value from sql server (Id=%d, Name=%s) does not match %s! (Name=%s), attempting to auto-fix.", attr_id, name, tablename, attr->names[attr_id]);

                cmd_len = sprintf(cmd, "DELETE FROM dbo.%s WHERE ID >= %d;", tablename, attr_id);
                sqlConnExecDirect(cmd, cmd_len, SQLCONN_FOREGROUND, false);
                retried = true;
                goto retry;
            }

            FatalErrorf("Value from sql server (Id=%d, Name=%s) does not match %s! (Name=%s)", attr_id, name, tablename, attr->names[attr_id]);
        }
    }

    stashTableDestroy(table.name_hashes);
    free(cols);

    assert(prev_attr_id == attr_id);
    return attr_id;
}

AttributeList *tpltLoadAttributes(char *fname, char *alt_fname, char *tablename)
{
    int attr_id = 0;
    AttributeList *attr = calloc(1, sizeof(*attr));

    // Verify that the table name has no namespace in it
    devassert(!strchr(tablename, '.'));

    // Mark the table so it does not get deleted
    markTableReferenced(tablename);

    // Load the file off disk into the AttributeList    
    loadAttributesFromFile(fileExists(fname) ? fname : alt_fname, attr);

    attr_id = verifyAttributesTable(tablename, attr);
    if (eaSize(&attr->names) > attr_id+1) {
        int i;
        int next = 0;
        int rows = 0;
        char * buf = NULL;
        HSTMT stmt = NULL;

        estrCreateEx(&buf, LONG_SQL_STRING);

        for(i=attr_id+1; i<eaSize(&attr->names); i++)
        {
            if (attr_id)
                LOG(LOG_DEBUG_LOG, LOG_LEVEL_IMPORTANT, LOG_CONSOLE, "Adding attribute: \"%s\"\n", attr->names[i]);

            if (!next) {
                sqlCheckDdl(DDL_ATTRIBUTES);
                stmt = sqlConnStmtAlloc(SQLCONN_FOREGROUND);
                // MS SQL 2005 hack
                if (gDatabaseProvider == DBPROV_MSSQL && gDatabaseVersion < 100)
                    estrClear(&buf);
                else
                    estrPrintf(&buf, "INSERT INTO dbo.%s (Id, Name) VALUES ", tablename);
            }

            // MS SQL 2005 hack
            if (gDatabaseProvider == DBPROV_MSSQL && gDatabaseVersion < 100)
                estrConcatf(&buf, "INSERT INTO dbo.%s (Id,Name) VALUES (%d,?);", tablename, i);
            else
                estrConcatf(&buf, "(%d,?),", i);

            rows++;
            bindInputParameter(stmt, next++, CFTYPE_ANSISTRING, attr->names[i], NULL);

            if (next >= FLUSH_BINDS || rows >= 1000)
            {
                buf[estrLength(&buf)-1] = ';';
                if (sqlConnStmtExecDirectMany(stmt, buf, estrLength(&buf), SQLCONN_FOREGROUND, false))
                    FatalErrorf("Fatal SQL error");
                sqlConnStmtFree(stmt);

                // Reset next and rows now
                next = 0;
                rows = 0;
            }
        }
        if (rows)
        {
            buf[estrLength(&buf)-1] = ';';
            if (sqlConnStmtExecDirectMany(stmt, buf, estrLength(&buf), SQLCONN_FOREGROUND, false))
                FatalErrorf("Fatal SQL error");
            sqlConnStmtFree(stmt);
        }
        
        estrDestroy(&buf);
    }
    return attr;
}

void tpltDeleteAll(ContainerTemplate *tplt, char *null_field, bool renumber)
{
    unsigned i;
    char *buf = estrTemp();
    char *master = tplt->tables->name;

    for(i=tplt->table_count-1; i>0; i--)
    {
        char *sub = tplt->tables[i].name;

        switch (gDatabaseProvider) {
            xcase DBPROV_MSSQL:
                estrPrintf(&buf, "DELETE dbo.%s FROM dbo.%s INNER JOIN dbo.%s ON %s.ContainerID = %s.ContainerID", sub, sub, master, master, sub);
                if (null_field)
                    estrConcatf(&buf, " WHERE %s.%s IS NULL", master, null_field);
            xcase DBPROV_POSTGRESQL:
                estrPrintf(&buf, "DELETE FROM dbo.%s USING dbo.%s WHERE %s.ContainerID = %s.ContainerID", sub, master, master, sub);
                if (null_field)
                    estrConcatf(&buf, " AND %s.%s IS NULL", master, null_field);
            DBPROV_XDEFAULT();
        }

        estrConcatChar(&buf, ';');
        sqlExecAsync(buf, estrLength(&buf));
        sqlFifoTick();
    }
    
    sqlFifoBarrier();
    estrPrintf(&buf, "DELETE FROM dbo.%s", master);
    if (null_field)
        estrConcatf(&buf, " WHERE %s IS NULL", null_field);
    estrConcatChar(&buf, ';');
    sqlExecAsync(buf, estrLength(&buf));

    if (renumber)
    {
        sqlFifoBarrier();

        switch (gDatabaseProvider) {
            xcase DBPROV_MSSQL:
                estrPrintf(&buf, "DBCC CHECKIDENT('%s', RESEED, 0) WITH NO_INFOMSGS;", master);
            xcase DBPROV_POSTGRESQL:
                estrPrintf(&buf, "ALTER SEQUENCE dbo.%s_containerid_seq RESTART;", master);
            DBPROV_XDEFAULT();
        }
        
        sqlExecAsync(buf, estrLength(&buf));
    }

    sqlFifoTick();
}
