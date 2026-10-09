
#include <utilitieslib/stdtypes.h>
#include "container_tplt_utils.h"
#include "container.h"
#include "container_sql.h"
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/strings_opt.h>
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/components/StashTable.h>
#include <utilitieslib/utils/error.h>
#include <utilitieslib/utils/wininclude.h>
#include "sql/sqlconn.h"
#include "storage/database.h"
#include <utilitieslib/utils/log.h>
#include <utilitieslib/utils/SuperAssert.h>

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <utilitieslib/assert/assert.h>
#include <sqlext.h>

DatabaseProvider gDatabaseProvider = DBPROV_UNKNOWN;
int gDatabaseVersion = 0;

ContainerFieldInfo g_containerfieldinfo[] =
{
    {  0,            SQL_C_DEFAULT,          SQL_UNKNOWN_TYPE,    }, // CFTYPE_NULL
    {  1,            SQL_C_TINYINT,          SQL_TINYINT,         }, // CFTYPE_BYTE
    {  2,            SQL_C_SHORT,            SQL_SMALLINT,        }, // CFTYPE_SHORT
    {  4,            SQL_C_LONG,             SQL_INTEGER,         }, // CFTYPE_INT
    {  4,            SQL_C_FLOAT,            SQL_REAL,            }, // CFTYPE_FLOAT
    {  0,            SQL_C_WCHAR,            SQL_WVARCHAR,        }, // CFTYPE_UNICODESTRING
    {  0,            SQL_C_CHAR,             SQL_VARCHAR,         }, // CFTYPE_ANSISTRING
    { 16,            SQL_C_TYPE_TIMESTAMP,   SQL_TYPE_TIMESTAMP,  }, // CFTYPE_DATETIME
    { -1,            SQL_C_BINARY,           SQL_VARBINARY,       }, // CFTYPE_BINARY_MAX
    { -1,            SQL_C_WCHAR,            SQL_WVARCHAR,        }, // CFTYPE_UNICODESTRING_MAX
    { -1,            SQL_C_CHAR,             SQL_VARCHAR,         }, // CFTYPE_ANSISTRING_MAX
    { -1,            SQL_C_BINARY,           SQL_LONGVARBINARY,   }, // CFTYPE_BLOB
};
STATIC_ASSERT(ARRAY_SIZE(g_containerfieldinfo) == CFTYPE_COUNT);

static int field_must_start_with_cr=1;

char * getContainerFieldType(enum ContainerFieldType field_type)
{
    return (char *)dbStorageTypeName(gDatabaseProvider, field_type);
}

void setFieldRequiresCR(int yes)
{
    field_must_start_with_cr = yes;
}

char *findFieldText(char *data,char *field,char *buf)
{
char    *s,*str,searchname[100] = "\n";

    if (field_must_start_with_cr)
        strcpy(&searchname[1],field);
    else
        strcpy(&searchname[0],field);
    strcat(searchname," ");
    s = strstr(data,searchname);
    if (!s)
    {
        if (strncmp(data,field,strlen(field)) != 0)
            return 0;
        s = data-1;
    }
    str = s + strlen(searchname);
    if (*str == '"')
        str++;
    s = strchr(str,'\n');
    if (!s)
        s = str + strlen(str);
    if (s[-1] == '"')
        s--;
    strncpy(buf,str,s-str);
    buf[s-str] = 0;
    return buf;
}

int dataType(char *str, int *column_size, int *num_bytes, char **sql_type_name)
{
    static char static_sql_type_name[128];
    int type = CFTYPE_NULL;
    int length = 0;
    char * type_name;
    char * length_str;

    type_name = strtok(str, "[");
    length_str = strtok(NULL, "]");

    if (stricmp(type_name, "int1")==0) {
        type = CFTYPE_BYTE;
    } else if (stricmp(type_name, "int2")==0) {
        type = CFTYPE_SHORT;
    } else if (stricmp(type_name, "int4")==0) {
        type = CFTYPE_INT;
    } else if (stricmp(type_name, "float4")==0) {
        type = CFTYPE_FLOAT;
    } else if (stricmp(type_name, "unicodestring")==0) {
        type = CFTYPE_UNICODESTRING;
    } else if (stricmp(type_name, "ansistring")==0) {
        type = CFTYPE_ANSISTRING;
    } else if (stricmp(type_name, "datetime")==0) {
        type = CFTYPE_DATETIME;
    } else if (stricmp(type_name, "binary(max)")==0) {
        type = CFTYPE_BINARY_MAX;
    } else if (stricmp(type_name, "unicodestring(max)")==0) {
        type = CFTYPE_UNICODESTRING_MAX;
    } else if (stricmp(type_name, "ansistring(max)")==0) {
        type = CFTYPE_ANSISTRING_MAX;
    } else {
        FatalErrorf("Unknown data type: %s\n", type_name);
    }

    if (length_str)
        length = atoi(length_str);

    switch (type) {
        case CFTYPE_UNICODESTRING:
        {
            sprintf(static_sql_type_name, "%s(%d)", getContainerFieldType(type), length);
            *column_size = length;
            *num_bytes = (length+1)*2; // wide characters
            *sql_type_name = static_sql_type_name;
            break;
        }
        case CFTYPE_ANSISTRING:
        {
            sprintf(static_sql_type_name, "%s(%d)", getContainerFieldType(type), length);
            *column_size = length;
            *num_bytes = length+1;
            *sql_type_name = static_sql_type_name;
            break;
        }
        default:
        {
            *column_size = 0;
            *num_bytes = g_containerfieldinfo[type].access_size;
            *sql_type_name = getContainerFieldType(type);
        }
    }

    return type;
}

void containerInitIndex(IndexedContainer *cont_lines,const char *data,int create)
{
    char                *str,*s;
    ContainerLine        *line;
    
    cont_lines->count = 0; //WAS: memset(cont_lines,0,sizeof(*cont_lines));
    dynArrayFit(&cont_lines->data,1,&cont_lines->data_max, (int)strlen(data)+1);
    strcpy(cont_lines->data,data);

    // preallocate some space
    dynArrayFit(&cont_lines->lines, sizeof(ContainerLine), &cont_lines->max_lines, MAX_QUERY_RESULTS);
    
    if (create)
    {
        cont_lines->hash_table = stashTableCreateWithStringKeys(stashOptimalSize(MAX_QUERY_RESULTS), StashDeepCopyKeys);
    }
    else
        stashTableClear(cont_lines->hash_table);
    
    str = cont_lines->data;
    for(;;)
    {
        if (!str[0])
            break;
        s = strchr(str,' ');
        if (!s)
            break;
        *s = 0;

        dynArrayFit(&cont_lines->lines, sizeof(ContainerLine), &cont_lines->max_lines, cont_lines->count+1);
        line = &cont_lines->lines[cont_lines->count++];
        line->field = str;
        line->value = s + 1;
        s = strchr(s+1,'\n');
        if (s)
        {
            if (s[-1] == '\r')
                s[-1] = 0;
            *s = 0;
            str = s + 1;
        }
        stashAddInt(cont_lines->hash_table,line->field,cont_lines->count-1, false);
        if (!s)
            break;
    }
}

void containerFreeIndex(IndexedContainer *cont)
{
    if (!cont->data)
        return;
    free(cont->data);
    cont->data = 0;
    stashTableDestroy(cont->hash_table);
}

int isNumber(unsigned char *s)
{
    if (isdigit(*s))
        return 1;
    if (*s == '-' && isdigit(s[1]))
        return 1;
    return 0;
}

int valueNotNull(char *s)
{
    if (isNumber(s))
    {
        for(;*s;s++)
        {
            if (*s > '0' && *s <= '9')
                return 1;
        }
    }
    else
    {
        if (stricmp(s,"\"nothing\"")==0 || stricmp(s,"nothing")==0 || stricmp(s,"\"none\"")==0 || stricmp(s,"none")==0 || strcmp(s,FAKE_ROW_PLACEHOLDER)==0)
            return 0;
        for(;*s;s++)
        {
            if (*s != '"')
                return 1;
        }
    }
    return 0;
}

int ctnrLineGet(CtnrLineState *state)
{
    unsigned char    *s,*s2;

    state->count = 0;
    while(!state->count)
    {
        if (!state->curr)
            return 0;
        state->count = tokenize_line(state->curr,state->args,&state->curr);
    }

    state->last_table_idx = state->table_idx;
    strcpy(state->last_table,state->table);
    state->field = state->args[0];
    state->value = state->args[1];
    s = strchr(state->args[0],'[');
    if (s)
    {
        *s++ = 0;
        s2 = strchr(s,']');
        *s2 = 0;
        state->idx_name = s;
        if (!isdigit((unsigned char)*s))
            state->table_idx = -2;
        else
            state->table_idx = atoi(s);
        state->field = 2 + s2;
        state->table = state->args[0];
    }
    return 1;
}

static char *ctnrLineBuf=0;
static int ctnrLineBuf_size=0;
static bool ctnrLineBuf_inuse=false;

void ctnrLineInit(CtnrLineState *state,char *data,char *table_name)
{
    int len=0;
    if (data)
    {
        len = (int)strlen(data) + 1;
    }

    assert(!ctnrLineBuf_inuse);
    ctnrLineBuf_inuse = true;

    memset(state,0,sizeof(*state));
    if (len > ctnrLineBuf_size)
    {
        ctnrLineBuf = realloc(ctnrLineBuf, MAX(len+512, 10*1024));
    }
    state->buf = ctnrLineBuf;
    memcpy(ctnrLineBuf, data, len);
    state->table = table_name;
    state->curr = state->buf;
    state->table_idx = -1;
}

void cntrLineFree(CtnrLineState *state)
{
    state->buf = 0;
    assert(ctnrLineBuf_inuse);
    ctnrLineBuf_inuse = false;
}

void checkEntCorrupted(char *data)
{
    static char    *trays[] = { "CurrentTray ","CurrentAltTray " };
    U8            *s;
    int            i,val;

    if (!data)
        return;
    s = strstr(data,"\"(null)\"");
    assert(!s);
    for(s=data;*s;s++)
    {
        if (isprint(*s) || *s == '\n' || *s == '\r')
            continue;
        assert(0 && "container data corrupted");
    }
    for(i=0;i<ARRAY_SIZE(trays);i++)
    {
        s = strstr(data,trays[i]);
        if (s)
        {
            s += strlen(trays[i]);
            val = atoi(s);
            assert(val >=0 && val <= 9);
        }
    }
}

#ifdef DBSERVER
void odbcInitialSetup(void)
{
    unsigned i;
    char on_conn_str[SHORT_SQL_STRING];
    int on_conn_str_len;
    int timeout;

    printf_stderr("SQL_GETDATA_EXTENSIONS = 0x%08x\n", sqlConnGetInfo(SQL_GETDATA_EXTENSIONS, false, SQLCONN_FOREGROUND));

    // Verify connection timeout
    timeout = sqlConnGetInfo(SQL_ATTR_QUERY_TIMEOUT, false, SQLCONN_FOREGROUND);
    assertmsgf(!timeout, "SQL server is set to timeout after %d seconds", timeout);
    
    switch (gDatabaseProvider) {
        xdefault:
            //assert(sqlConnSetConnectAttr(SQL_ATTR_TXN_ISOLATION, (SQLPOINTER)SQL_TXN_READ_UNCOMMITTED, i));
            //assert(sqlConnSetConnectAttr(SQL_ATTR_TXN_ISOLATION, (SQLPOINTER)SQL_TXN_READ_COMMITTED, i));;
            //assert(sqlConnSetConnectAttr(SQL_ATTR_TXN_ISOLATION, (SQLPOINTER)SQL_TXN_REPEATABLE_READ, i));
            //on_conn_str_len = sprintf(on_conn_str, "SET TRANSACTION ISOLATION LEVEL SNAPSHOT;");
            on_conn_str_len = 0;
    }

    if (!on_conn_str_len)
        return;

    for (i=0; i<SQLCONN_MAX; i++) {
        sqlConnExecDirectMany(on_conn_str, on_conn_str_len, i, true);
    }
}

void queryDatabaseVersion(void)
{
    char cmd[SHORT_SQL_STRING];
    int cmd_len;

    switch (gDatabaseProvider) {
        xcase DBPROV_MSSQL:
            cmd_len = sprintf(cmd, "SELECT compatibility_level FROM sys.databases WHERE name=db_name();");
        xcase DBPROV_POSTGRESQL:
            cmd_len = sprintf(cmd, "SELECT CAST(CAST(substring(character_value FROM '[[:digit:]]+\\.[[:digit:]]+') AS float)*100 as integer) FROM information_schema.sql_implementation_info WHERE implementation_info_name = 'DBMS VERSION';");
        DBPROV_XDEFAULT();
    }

    gDatabaseVersion = sqlGetSingleValue(cmd, cmd_len, NULL, SQLCONN_FOREGROUND);
}

void bindInputParameter(HSTMT stmt, int index, enum ContainerFieldType type, const void * data, size_t * size)
{
    static const SQLLEN no_data = SQL_NULL_DATA;

    sqlConnStmtBindParam(stmt, index+1, SQL_PARAM_INPUT, g_containerfieldinfo[type].access_type, g_containerfieldinfo[type].actual_type, 0, 0, cpp_const_cast(void*)(data), size ? *size : 0, data ?size : (SQLLEN*)&no_data);
}

void bindOutputColumn(HSTMT stmt, int index, enum ContainerFieldType type, size_t size, void * data, size_t * count)
{
    sqlConnStmtBindCol(stmt, index+1, g_containerfieldinfo[type].access_type, data, size, count);
}
#endif
