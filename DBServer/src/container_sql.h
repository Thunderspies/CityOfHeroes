#ifndef _CONTAINER_SQL_H
#define _CONTAINER_SQL_H

#include <utilitieslib/stdtypes.h>
#include "container_tplt_utils.h"
#include "sql/sqlconn.h"

typedef struct StashTableImp *StashTable;
typedef const struct StashTableImp *cStashTable;
typedef struct LineList LineList;
struct DbList;

typedef enum DdlType
{
    DDL_ATTRIBUTES,
    DDL_ADD,
    DDL_DEL,
    DDL_REBUILDTABLE,
    DDL_ALTERCOLUMN,
} DdlType;

typedef struct BindList
{
    enum ContainerFieldType data_type;
    void * data;
} BindList;

typedef struct AttributeList
{
    StashTable hash_table;
    const char **names;
} AttributeList;

typedef struct ColumnInfo
{
    char    name[128];
    enum ContainerFieldType data_type;
    int        num_bytes;
    int        column_size;
    char    data_type_name[32];
    U8        exists_in_sql;
    U8        reserved_word;
    U8        idx_by_attr;
    U8        is_sub_id_field;
    U8        indexed;
    AttributeList    *attr;
} ColumnInfo;

typedef enum TableType {
    TT_UNKNOWN=0,
    TT_ATTRIBUTE,
    TT_CONTAINER,
    TT_SUBCONTAINER
} TableType;

typedef struct TableInfo
{
    ColumnInfo    *columns;
    int            num_columns;
    int            array_count;
    TableType   table_type;
    char        name[128];
    StashTable    name_hashes;
    int            all_hash_first_idx;

    StashTable    sql_updatelink_hashes;
    void        *sql_select_link[SQLCONN_MAX];
    char        *sql_select_query[SQLCONN_MAX];
} TableInfo;

typedef struct SlotInfo
{
    TableInfo    *table;
    int            sub_id;
    int            idx;
//    char        test_name[128];
} SlotInfo;

typedef struct ContainerTemplate
{
    TableInfo    *tables;
    int            table_count;
    TableInfo    *member_table;
    char        member_field_name[100];
    int            dblist_id;
    int            member_id;
    StashTable    all_hashes;
    int            max_lines;
    char        **lines;
    SlotInfo    *slots;
    U32            dont_write_to_sql : 1;
} ContainerTemplate;

// the comments below ignore dbimport.c and dbperf.c, which are not used during normal operation

// externally used in dbInit() only, before main loop!
void sqlAddForeignKeyConstraintAsync(char *table, char *key, char *foreign_table); // via tpltSetAllForeignKeyConstraints()
void sqlRemoveForeignKeyConstraintAsync(char *table, char *key, char *foreign_table);
void sqlAddIndexAsync(char *index, char *table, char *fields);
void sqlRemoveIndexAsync(char *index, char *table);
char **sqlReadTableNames(int *countp); // via tpltDropUnreferencedTables()
void sqlReadTableNamesFree(char **names, int count); // via tpltDropUnreferencedTables()
bool sqlExecDdl(DdlType ddl_type, char *str, int str_len);
bool sqlExecDdlf(DdlType ddl_type, char *fmt, ...);

// externally used in dbInit() only, before main loop! (via tpltUpdateSqlcolumns() or its subroutines)
bool sqlCreateTable(char *name, TableType table_type);
bool sqlAddField(char *table, char *name, char *type);
bool sqlChangeFieldType(char *table, char *name, char *newtype);
bool sqlDeleteField(char *table, char *name, char *type);
void sqlDropForeignKeysForTable(char *tablename);
int sqlTableReadMulti(ContainerTemplate *tplt,TableInfo *table,int container_id,char ***multi_buf_ptr,SqlConn conn); // used to temporarily store all data while rebuilding a table
int sqlGetTableInfo(char *table_name,ColumnInfo **columns_ptr);
int sqlDropTable(char *table); // also via tpltDropUnreferencedTables()
void sqlCheckDdl(DdlType type); // also in dbInit() itself

// externally used during the main loop (async only)
void sqlDeleteContainer(ContainerTemplate *tplt, int container_id);
void sqlDeleteRowInternal(char *table, int container_id, SqlConn conn);

// externally used during the main loop (sync and async)
char *sqlContainerRead(ContainerTemplate *tplt, int container_id, SqlConn conn);
int sqlGetSingleValue(char *expr, int str_len, BindList * bind_list, SqlConn conn);
char *sqlReadColumnsInternal(TableInfo *table, char *limit, char *col_names, char *where, int *count_ptr, ColumnInfo **field_ptrs, SqlConn conn, int debugDelayMS);
void sqlContainerUpdateInternal(ContainerTemplate *tplt, int container_id, void *bin_data, SqlConn conn);

// startup test
void testDataBaseTypes(struct DbList * list);

#endif
