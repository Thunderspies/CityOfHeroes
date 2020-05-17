#include <utilitieslib/stdtypes.h>
#include "AccountSql.h"
#include "account/AccountCatalog.h"
#include "account/AccountData.h"
#include "AccountDb.hpp"
#include "AccountServer.hpp"
#include "account_inventory.h"
#include "transaction.h"
#include "sql/sqlconn.h"
#include "sql/sqlinclude.h"
#include "sql/sqltask.h"
#include <utilitieslib/components/earray.h>
#include <utilitieslib/components/EString.h>
#include <utilitieslib/components/StashTable.h>
#include <utilitieslib/components/MemoryPool.h>
#include <utilitieslib/utils/error.h>
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/utils/SuperAssert.h>
#include <utilitieslib/utils/utils.h>

#define ASQL_WORKERS (SQLCONN_MAX - 1)
#define ASQL_MAX_RETRIES 30
//#define ASQL_USE_TABLE_VALUED_PARAMETERS

STATIC_ASSERT(sizeof(((asql_account*)NULL)->loyalty_bits) == LOYALTY_BITS/8);

/// Stored procedure cache indices
enum asql_stored_proc {
    ASQL_SEARCH_FOR_ACCOUNT=0,
    ASQL_FIND_OR_CREATE_ACCOUNT,
    ASQL_UPDATE_ACCOUNT,
    ASQL_ADD_MICRO_TRANSACTION,
    ASQL_ADD_GAME_TRANSACTION,
    ASQL_ADD_MULTI_GAME_TRANSACTION,
    ASQL_SAVE_GAME_TRANSACTION,
    ASQL_REVERT_GAME_TRANSACTION,
    ASQL_READ_UNSAVED_GAME_TRANSACTIONS,
    ASQL_STORED_PROCEDURE_MAX
};

/// Stored procedure names
static const char * asql_stored_procedure_names[] = {
    "ASQL_SEARCH_FOR_ACCOUNT",
    "ASQL_FIND_OR_CREATE_ACCOUNT",
    "ASQL_UPDATE_ACCOUNT",
    "ASQL_ADD_MICRO_TRANSACTION",
    "ASQL_ADD_GAME_TRANSACTION",
    "ASQL_ADD_MULTI_GAME_TRANSACTION",
    "ASQL_SAVE_GAME_TRANSACTION",
    "ASQL_REVERT_GAME_TRANSACTION",
    "ASQL_READ_UNSAVED_GAME_TRANSACTIONS",
};
STATIC_ASSERT(ARRAY_SIZE(asql_stored_procedure_names) == ASQL_STORED_PROCEDURE_MAX);

struct asql_inventory_bytes {
    ssize_t sku_id_bytes;
    ssize_t granted_total_bytes;
    ssize_t claimed_total_bytes;
    ssize_t saved_total_bytes;
    ssize_t expires_bytes;
};

struct asql_game_transaction_bytes {
    ssize_t order_id_bytes;
    ssize_t auth_id_bytes;
    ssize_t sku_id_bytes;
    ssize_t transaction_date_bytes;
    ssize_t shard_id_bytes;
    ssize_t ent_id_bytes;
    ssize_t granted_bytes;
    ssize_t claimed_bytes;
    ssize_t csr_did_it_bytes;
};

struct __declspec(align(EXPECTED_WORST_CACHE_LINE_SIZE)) asql_task {
    /// stored procedure to run
    asql_stored_proc proc;

    /// stored procedure return value
    bool success;

    union {
        /// account stored procedure data
        struct {            
            char search_for[ACCOUNT_NAME_MAX+1];
            U32 found_auth_id;
        } search_for_account;

        /// account stored procedure data
        struct {            
            asql_account acc;
            int inv_count;
            asql_inventory * inv_list;
            Account * account;
        } add_update_account;

        /// micro transaction stored procedure data
        struct {
            MicroTransaction * micro_transaction;
        } add_micro;

        /// game transaction stored procedure data
        struct {
            GameTransaction * game_transaction;
        } add_game;

        struct {
            MultiGameTransaction * multi_game_transaction;
        } add_multi_game;

        /// save / revert game transaction stored procedure data
        struct {
            Account * account;
            OrderId order_id;
            asql_flexible_inventory flex_inv;
        } save_revert_game;

        /// read game transaction stored procedure data
        struct {
            Account * account;
            U8 shard_id;
            U32 ent_id;
            int gtx_count;
            asql_game_transaction * gtx_list;
        } read_game;
    };
};

STATIC_ASSERT(sizeof(asql_task) <= EXPECTED_WORST_CACHE_LINE_SIZE*2);

struct asql_connection {
    /// Stored procedures for the connection
    HSTMT proc[ASQL_STORED_PROCEDURE_MAX];
};

struct asql_globals {
    /// auth_id index into a @ref asql_account
    StashTable accounts;
    /// SQL connection state
    asql_connection conn[SQLCONN_MAX];
    /// Asynchronous queries in flight
    int in_flight;
} as;

/// Pool allocator for @ref asql_account
MP_DEFINE(asql_account);
MP_DEFINE(asql_task);

static void * asql_run_entry(void * data, void * user);
static void asql_finish_entry(void * data);

void asql_open() {
    int i;

    MP_CREATE(asql_account, ACCOUNT_INITIAL_CONTAINER_SIZE);
    MP_CREATE(asql_task, ACCOUNT_INITIAL_CONTAINER_SIZE);

    as.accounts = stashTableCreateInt(stashOptimalSize(ACCOUNT_INITIAL_CONTAINER_SIZE));    
    as.in_flight = 0;

    sql_task_init(ASQL_WORKERS, 2048);
    for (i = 0; i < ASQL_WORKERS; i++)
    {
        sql_task_set_callback(i, asql_run_entry, (void*)(intptr_t)(i + 1));
    }
    sql_task_freeze();

    assert(sqlConnInit(SQLCONN_MAX));
    sqlConnSetLogin(g_accountServerState.cfg.sqlLogin);
    
    if (!sqlConnDatabaseConnect(g_accountServerState.cfg.sqlDbName, "ACCOUNTSERVER"))
    {
        FatalErrorf("AccountServer could not connect to SQL database.");
    }

    for (i=0; i<SQLCONN_MAX; i++) {
        memset(as.conn[i].proc, 0, sizeof(as.conn[i].proc));
    }
}

void asql_close() {
    int i;

    while (as.in_flight)
        asql_wait_for_tick();

    sql_task_thaw();
    sql_task_shutdown();

    for (i=0; i<SQLCONN_MAX; i++) {
        int j;

        for (j=0; j<ASQL_STORED_PROCEDURE_MAX; j++) {
            if (as.conn[i].proc[j]) {
                sqlConnStmtFree(as.conn[i].proc[j]);
                as.conn[i].proc[j] = NULL;
            }
        }
    }

    sqlConnShutdown();

    stashTableDestroy(as.accounts);    

    MP_DESTROY(asql_account);
    MP_DESTROY(asql_task);
}

int asql_in_flight() {
    return as.in_flight;
}

void asql_tick() {
    void * data;

    while((data = sql_task_trypop()))
        asql_finish_entry(data);
}

void asql_wait_for_tick() {
    assert(as.in_flight);
    asql_finish_entry(sql_task_pop());
}

static asql_task * asql_get_task() {
    return MP_ALLOC(asql_task);
}

static void asql_push_task(asql_task * task, U32 id) {
    if (!as.in_flight++)
        sql_task_thaw();

    sql_task_push(id & (ASQL_WORKERS-1), task);
}

static asql_account * asql_find_account(U32 auth_id) {
    asql_account * ret = NULL;

    assert(auth_id);

    if (!stashIntFindPointer(as.accounts, auth_id, reinterpret_cast<void**>(&ret)))
        return NULL;

    return ret;
}

static HSTMT asql_prepare(SqlConn conn, asql_stored_proc proc, char const* cmd) {
    int rc;

    // one time bind
    if (as.conn[conn].proc[proc]) {
        sqlConnStmtUnbindParams(as.conn[conn].proc[proc]);
        sqlConnStmtCloseCursor(as.conn[conn].proc[proc]);
        return as.conn[conn].proc[proc];
    }

    as.conn[conn].proc[proc] = sqlConnStmtAlloc(conn);

    rc = sqlConnStmtPrepare(as.conn[conn].proc[proc], cmd, SQL_NTS, conn);
    assert(SQL_SUCCEEDED(rc));

    return as.conn[conn].proc[proc];
}

static void asql_consume_all_errors(HSTMT stmt, const char *original_command) {
    SQLRETURN ret;
    // Consume the rest of the error stream so the caller does not have to
    while ((ret = SQLMoreResults(stmt)) != SQL_NO_DATA)
    {
        if (ret != SQL_SUCCESS)
            sqlConnStmtPrintError(stmt, original_command);
    }
}

static bool asql_execute_procedure(SqlConn conn, asql_stored_proc proc) {
    HSTMT stmt = as.conn[conn].proc[proc];
    SQLRETURN ret = 0;
    unsigned i;
    char sql_state[SQL_SQLSTATE_SIZE+1] = {0};

    for (i=0; i<ASQL_MAX_RETRIES; i++) {
        ret = _sqlConnStmtExecute(stmt, conn);
        if (SQL_SHOW_ERROR(ret))
        {
            sqlConnStmtPrintErrorAndGetState(stmt, asql_stored_procedure_names[proc], sql_state);
            asql_consume_all_errors(stmt, asql_stored_procedure_names[proc]);
        }

        if (!SQL_NEEDS_RETRY(ret) || SQLSTATE_SKIP_RETRY(sql_state))
            break;

        SQLCancel(stmt);
        Sleep(randInt(1000) + 1000);
    }

    if (SQL_SUCCEEDED(ret))
        return true;

    return false;
}

void asql_sync_product_types() {
    SQLRETURN ret;

    long product_type_ids[kAccountInventoryType_Count];
    char names[kAccountInventoryType_Count][128];
    ssize_t name_bytes[kAccountInventoryType_Count];

    for (unsigned i=0; i<kAccountInventoryType_Count; i++) {
        product_type_ids[i] = i;

        const char * name = accountCatalogGetProductTypeString((AccountInventoryType)i);
        name_bytes[i] = strlen(name);
        assert(name_bytes[i] < sizeof(*names));
        memcpy(names[i], name, name_bytes[i]);
        devassert(names[i][0] != 0); // verify name is not null
    }

    HSTMT stmt = sqlConnStmtAlloc(SQLCONN_FOREGROUND);

#ifdef ASQL_USE_TABLE_VALUED_PARAMETERS
    size_t rows = kAccountInventoryType_Count;

    sqlConnStmtStartBindParamTableValued(stmt, 1, kAccountInventoryType_Count, L"TVP_product_type", &rows);
    sqlConnStmtBindParam(stmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, product_type_ids, sizeof(*product_type_ids), NULL);
    sqlConnStmtBindParam(stmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, ARRAYSIZE(*names), 0, names, sizeof(*names), name_bytes);
    sqlConnStmtEndBindParamTableValued(stmt);

    ret = sqlConnStmtExecDirect(stmt,
        "MERGE INTO product_type AS target"
        " USING ? AS source"
        " ON target.product_type_id = source.product_type_id"
        " WHEN MATCHED THEN UPDATE SET name = source.name"
        " WHEN NOT MATCHED BY TARGET THEN INSERT (product_type_id, name) VALUES (source.product_type_id, source.name)"
        " WHEN NOT MATCHED BY SOURCE THEN DELETE;",
        SQL_NTS, SQLCONN_FOREGROUND, false);
#else
    ret = sqlConnStmtExecDirect(stmt, "CREATE TABLE #tmp_product_type (product_type_id int, name varchar(128));", SQL_NTS, SQLCONN_FOREGROUND, false);
    
    if (SQL_SUCCEEDED(ret)) {
        sqlConnStmtBindParamArray(stmt, kAccountInventoryType_Count, SQL_PARAM_BIND_BY_COLUMN);
        sqlConnStmtBindParam(stmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, product_type_ids, sizeof(*product_type_ids), NULL);
        sqlConnStmtBindParam(stmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, ARRAYSIZE(*names), 0, names, sizeof(*names), name_bytes);
        
        ret = sqlConnStmtExecDirect(stmt, "INSERT INTO #tmp_product_type VALUES (?, ?);", SQL_NTS, SQLCONN_FOREGROUND, false);

        sqlConnStmtBindParamArray(stmt, 1, SQL_PARAM_BIND_BY_COLUMN);
        sqlConnStmtUnbindParams(stmt);
    }

    if (SQL_SUCCEEDED(ret)) {
        ret = sqlConnStmtExecDirect(stmt,
            "MERGE INTO product_type AS target"
            " USING #tmp_product_type AS source"
            " ON target.product_type_id = source.product_type_id"
            " WHEN MATCHED THEN UPDATE SET name = source.name"
            " WHEN NOT MATCHED BY TARGET THEN INSERT (product_type_id, name) VALUES (source.product_type_id, source.name)"
            " WHEN NOT MATCHED BY SOURCE THEN DELETE;",
            SQL_NTS, SQLCONN_FOREGROUND, false);
    }
#endif

    sqlConnStmtFree(stmt);

    if (!SQL_SUCCEEDED(ret))
        FatalErrorf("Could not update the product_type table");
}

void asql_sync_products() {
    SQLRETURN ret;

    asql_sync_product_types();

    const AccountProduct** products = accountCatalogGetEnabledProducts();

    size_t size = eaSize(&products);
    assert(size);

    SkuId * sku_ids = new SkuId[size];
    ssize_t * sku_bytes = new ssize_t[size];

    char (*names)[128] = new char[size][128];
    ssize_t * name_bytes = new ssize_t[size];

    long * product_type_ids = new long[size];

    long * grant_limits = new long[size];
    ssize_t * grant_limit_bytes = new ssize_t[size];

    long * expiration_seconds = new long[size];
    ssize_t * expiration_second_bytes = new ssize_t[size];

    for (unsigned i=0; i<size; i++) {
        sku_ids[i] = products[i]->sku_id;
        sku_bytes[i] = sizeof(SkuId);

        devassert(products[i]->title); // verify name is set
        name_bytes[i] = strlen(products[i]->title);
        assert(name_bytes[i] < sizeof(*names));
        memcpy(names[i], products[i]->title, name_bytes[i]);
        devassert(names[i][0] != 0); // verify name is not null
        devassert(names[i][0] != 'P' || !isdigit(names[i][1]) || !isdigit(names[i][2])); // verify this is not a pstring

        product_type_ids[i] = products[i]->invType;
        devassert(product_type_ids[i] >= 0 && product_type_ids[i] < kAccountInventoryType_Count);

        grant_limits[i] = products[i]->grantLimit;
        grant_limit_bytes[i] = products[i]->grantLimit ? sizeof(*grant_limits) : SQL_NULL_DATA;

        expiration_seconds[i] = products[i]->expirationSecs;
        expiration_second_bytes[i] = products[i]->expirationSecs ? sizeof(*expiration_seconds) : SQL_NULL_DATA;
    }

    HSTMT stmt = sqlConnStmtAlloc(SQLCONN_FOREGROUND);

#ifdef ASQL_USE_TABLE_VALUED_PARAMETERS
    sqlConnStmtStartBindParamTableValued(stmt, 1, size, L"TVP_product", &size);
    sqlConnStmtBindParam(stmt, 1, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_CHAR, ARRAY_SIZE(sku_ids->c), 0, sku_ids, sizeof(*sku_ids), sku_bytes);
    sqlConnStmtBindParam(stmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, ARRAYSIZE(*names), 0, names, sizeof(*names), name_bytes);
    sqlConnStmtBindParam(stmt, 3, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, product_type_ids, sizeof(*product_type_ids), NULL);
    sqlConnStmtBindParam(stmt, 4, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, grant_limits, sizeof(*grant_limits), grant_limit_bytes);
    sqlConnStmtBindParam(stmt, 5, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, expiration_seconds, sizeof(*expiration_seconds), expiration_second_bytes);
    sqlConnStmtEndBindParamTableValued(stmt);

    ret = sqlConnStmtExecDirect(stmt,
        "MERGE INTO product AS target"
        " USING ? AS source"
        " ON target.sku_id = source.sku_id"
        " WHEN MATCHED THEN UPDATE SET name = source.name, product_type_id = source.product_type_id, grant_limit = source.grant_limit, expiration_seconds = source.expiration_seconds"
        " WHEN NOT MATCHED BY TARGET THEN INSERT (sku_id, name, product_type_id, grant_limit, expiration_seconds) VALUES (source.sku_id, source.name, source.product_type_id, source.grant_limit, source.expiration_seconds)"
        " WHEN NOT MATCHED BY SOURCE THEN DELETE;",
        SQL_NTS, SQLCONN_FOREGROUND, false);
#else
    ret = sqlConnStmtExecDirect(stmt,
        "CREATE TABLE #tmp_product (sku_id char(8), name varchar(128), product_type_id int, grant_limit int, expiration_seconds int);",
        SQL_NTS, SQLCONN_FOREGROUND, false);

    if (SQL_SUCCEEDED(ret)) {
        sqlConnStmtBindParamArray(stmt, size, SQL_PARAM_BIND_BY_COLUMN);
        sqlConnStmtBindParam(stmt, 1, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_CHAR, ARRAY_SIZE(sku_ids->c), 0, sku_ids, sizeof(*sku_ids), sku_bytes);
        sqlConnStmtBindParam(stmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, ARRAYSIZE(*names), 0, names, sizeof(*names), name_bytes);
        sqlConnStmtBindParam(stmt, 3, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, product_type_ids, sizeof(*product_type_ids), NULL);
        sqlConnStmtBindParam(stmt, 4, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, grant_limits, sizeof(*grant_limits), grant_limit_bytes);
        sqlConnStmtBindParam(stmt, 5, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, expiration_seconds, sizeof(*expiration_seconds), expiration_second_bytes);

        ret = sqlConnStmtExecDirect(stmt, "INSERT INTO #tmp_product VALUES (?, ?, ?, ?, ?);", SQL_NTS, SQLCONN_FOREGROUND, false);

        sqlConnStmtBindParamArray(stmt, 1, SQL_PARAM_BIND_BY_COLUMN);
        sqlConnStmtUnbindParams(stmt);
    }

    if (SQL_SUCCEEDED(ret)) {
        ret = sqlConnStmtExecDirect(stmt,
            "MERGE INTO product AS target"
            " USING #tmp_product AS source"
            " ON target.sku_id = source.sku_id"
            " WHEN MATCHED THEN UPDATE SET name = source.name, product_type_id = source.product_type_id, grant_limit = source.grant_limit, expiration_seconds = source.expiration_seconds"
            " WHEN NOT MATCHED BY TARGET THEN INSERT (sku_id, name, product_type_id, grant_limit, expiration_seconds) VALUES (source.sku_id, source.name, source.product_type_id, source.grant_limit, source.expiration_seconds)"
            " WHEN NOT MATCHED BY SOURCE THEN DELETE;",
            SQL_NTS, SQLCONN_FOREGROUND, false);
    }
#endif

    sqlConnStmtFree(stmt);

    if (!SQL_SUCCEEDED(ret))
        FatalErrorf("Could not update the product table.  Likely either a product is listed twice or there is an attempt to delete a in-use product.");

    delete [] sku_ids;
    delete [] sku_bytes;

    delete [] names;
    delete [] name_bytes;

    delete [] product_type_ids;

    delete [] grant_limits;
    delete [] grant_limit_bytes;

    delete [] expiration_seconds;
    delete [] expiration_second_bytes;

    eaDestroyConst(&products);
}

bool asql_search_for_account(SqlConn conn, const char *auth_name_or_id, U32 *found_auth_id)
{
    HSTMT stmt = asql_prepare(conn, ASQL_SEARCH_FOR_ACCOUNT, "SELECT TOP 1 auth_id FROM account WHERE auth_id=? OR name=?;");

    ssize_t found_auth_id_bytes = sizeof(*found_auth_id);

    sqlConnStmtBindCol(stmt, 1, SQL_C_LONG, found_auth_id, sizeof(*found_auth_id), &found_auth_id_bytes);

    long search_for_id = atoi(auth_name_or_id);
    ssize_t search_for_id_bytes = search_for_id ? sizeof(search_for_id) : SQL_NULL_DATA;
    
    ssize_t auth_name_or_id_bytes = SQL_NTS;

    sqlConnStmtBindParam(stmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &search_for_id, sizeof(search_for_id), &search_for_id_bytes);
    sqlConnStmtBindParam(stmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZEOF2(asql_account, name), 0, (void*)auth_name_or_id, SIZEOF2(asql_account, name), &auth_name_or_id_bytes);

    if (!asql_execute_procedure(conn, ASQL_SEARCH_FOR_ACCOUNT))
        return false;

    int ret = _sqlConnStmtFetch(stmt, SQLCONN_FOREGROUND);
    if (ret != SQL_SUCCESS)
        sqlConnStmtPrintError(stmt, NULL);
    if(!SQL_SUCCEEDED(ret))
        return false;

    if (found_auth_id_bytes == SQL_NULL_DATA)
        *found_auth_id = 0;
    else
        devassert(found_auth_id_bytes == sizeof(*found_auth_id));
    return true;
}

void asql_search_for_account_finish(bool success, const char *auth_name_or_id, U32 found_auth_id)
{
    AccountServer_NotifyAccountFound(auth_name_or_id, found_auth_id);
}

void asql_search_for_account_async(const char *auth_name_or_id)
{
    assert(auth_name_or_id);

    asql_task * task = asql_get_task();
    task->proc = ASQL_SEARCH_FOR_ACCOUNT;
    strncpy(task->search_for_account.search_for, auth_name_or_id, sizeof(task->search_for_account.search_for));
    asql_push_task(task, rand()); // this function cannot run in a deterministic manner
}

static void asql_bind_inventory_columns(HSTMT stmt, asql_inventory * inv, asql_inventory_bytes * bytes) {
    sqlConnStmtBindCol(stmt, 1, SQL_C_BINARY, inv->sku_id.c, sizeof(inv->sku_id), &bytes->sku_id_bytes);
    sqlConnStmtBindCol(stmt, 2, SQL_C_LONG, &inv->granted_total, sizeof(inv->granted_total), &bytes->granted_total_bytes);
    sqlConnStmtBindCol(stmt, 3, SQL_C_LONG, &inv->claimed_total, sizeof(inv->claimed_total), &bytes->claimed_total_bytes);
    sqlConnStmtBindCol(stmt, 4, SQL_C_LONG, &inv->saved_total, sizeof(inv->saved_total), &bytes->saved_total_bytes);
    sqlConnStmtBindCol(stmt, 5, SQL_C_TYPE_TIMESTAMP, &inv->expires, sizeof(inv->expires), &bytes->expires_bytes);
}

static bool asql_check_inventory_results(asql_inventory * inv, asql_inventory_bytes * bytes) {
    bool ok = true;

    ok &= devassert(bytes->sku_id_bytes == sizeof(inv->sku_id));
    ok &= devassert(bytes->granted_total_bytes == sizeof(inv->granted_total));
    ok &= devassert(bytes->claimed_total_bytes == sizeof(inv->claimed_total));
    ok &= devassert(bytes->saved_total_bytes == sizeof(inv->saved_total));

    if (bytes->expires_bytes == SQL_NULL_DATA)
        memset(&inv->expires, 0, sizeof(inv->expires));
    else
        ok &= devassert(bytes->expires_bytes == sizeof(inv->expires));

    return ok;
}

static void asql_init_flexible_inventory(asql_flexible_inventory * flex_inv) {
    flex_inv->count = 0;
    flex_inv->list_size = 0;
    flex_inv->list = 0;
}

static void asql_add_flexible_inventory(asql_flexible_inventory * flex_inv, asql_inventory * inv) {
    if (!flex_inv->list && flex_inv->count == 0) {
        memcpy(&flex_inv->single, inv, sizeof(asql_inventory));
        flex_inv->count++;
        return;
    }

    bool copy_first = !flex_inv->list;

    void * next = dynArrayFit(reinterpret_cast<void**>(&flex_inv->list), sizeof(asql_inventory), &flex_inv->list_size, flex_inv->count++);
    memcpy(next, inv, sizeof(asql_inventory));

    if (copy_first)
        memcpy(flex_inv->list, &flex_inv->single, sizeof(asql_inventory));
}

static void asql_free_flexible_inventory(asql_flexible_inventory * flex_inv) {
    SAFE_FREE(flex_inv->list);
}

static bool asql_find_or_create_account(SqlConn conn, asql_account * acc, asql_inventory ** inv_list, int * inv_count) {
    *inv_count = 0;
    *inv_list = NULL;

    HSTMT stmt = asql_prepare(conn, ASQL_FIND_OR_CREATE_ACCOUNT, "{CALL dbo.SP_find_or_create_account (@auth_id=?, @name=?, @loyalty_bits=?, @last_loyalty_point_count=?, @loyalty_points_spent=?, @last_email_date=?, @last_num_emails_sent=?, @free_xfer_date=?)}");

    ssize_t name_size = acc->name[0] ? strlen(acc->name) : SQL_NULL_DATA;
    ssize_t loyalty_bits_size = LOYALTY_SQL_BYTES;
    ssize_t last_email_date_size = sizeof(acc->last_email_date);
    ssize_t free_xfer_date_size = sizeof(acc->free_xfer_date);

    sqlConnStmtBindParam(stmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &acc->auth_id, sizeof(acc->auth_id), NULL);
    sqlConnStmtBindParam(stmt, 2, SQL_PARAM_INPUT_OUTPUT, SQL_C_CHAR, SQL_VARCHAR, ARRAY_SIZE(acc->name)-1, 0, &acc->name, sizeof(acc->name), &name_size);
    sqlConnStmtBindParam(stmt, 3, SQL_PARAM_OUTPUT, SQL_C_BINARY, SQL_BINARY, LOYALTY_SQL_BYTES, 0, &acc->loyalty_bits, sizeof(acc->loyalty_bits), &loyalty_bits_size);
    sqlConnStmtBindParam(stmt, 4, SQL_PARAM_OUTPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &acc->last_loyalty_point_count, sizeof(acc->last_loyalty_point_count), NULL);
    sqlConnStmtBindParam(stmt, 5, SQL_PARAM_OUTPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &acc->loyalty_points_spent, sizeof(acc->loyalty_points_spent), NULL);
    sqlConnStmtBindParam(stmt, 6, SQL_PARAM_OUTPUT, SQL_C_TYPE_TIMESTAMP, SQL_TYPE_TIMESTAMP, MSSQL_SMALLDATETIME_PRECISION, MSSQL_SMALLDATETIME_DECIMALDIGITS, &acc->last_email_date, sizeof(acc->last_email_date), &last_email_date_size);
    sqlConnStmtBindParam(stmt, 7, SQL_PARAM_OUTPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &acc->last_num_emails_sent, sizeof(acc->last_num_emails_sent), NULL);
    sqlConnStmtBindParam(stmt, 8, SQL_PARAM_OUTPUT, SQL_C_TYPE_TIMESTAMP, SQL_TYPE_TIMESTAMP, MSSQL_SMALLDATETIME_PRECISION, MSSQL_SMALLDATETIME_DECIMALDIGITS, &acc->free_xfer_date, sizeof(acc->free_xfer_date), &free_xfer_date_size);

    asql_inventory inv;
    asql_inventory_bytes bytes;
    asql_bind_inventory_columns(stmt, &inv, &bytes);

    if (!asql_execute_procedure(conn, ASQL_FIND_OR_CREATE_ACCOUNT))
        return false;

    devassert(loyalty_bits_size == LOYALTY_SQL_BYTES);

    if (last_email_date_size == SQL_NULL_DATA)
        memset(&acc->last_email_date, 0, sizeof(acc->last_email_date));

    if (free_xfer_date_size == SQL_NULL_DATA)
        memset(&acc->free_xfer_date, 0, sizeof(acc->free_xfer_date));

    int limit = 0;
    int ret;
    while ((ret = _sqlConnStmtFetch(stmt, conn)) != SQL_NO_DATA) {
        if (ret != SQL_SUCCESS)
            sqlConnStmtPrintError(stmt, asql_stored_procedure_names[ASQL_FIND_OR_CREATE_ACCOUNT]);

        if(!SQL_SUCCEEDED(ret))
            continue;

        if (!asql_check_inventory_results(&inv, &bytes))
            return false;

        void * next = dynArrayFit(reinterpret_cast<void**>(inv_list), sizeof(asql_inventory), &limit, (*inv_count)++);
        memcpy(next, &inv, sizeof(asql_inventory));
    }

    return true;
}

static void asql_find_or_create_account_finish(bool success, Account * account, asql_account * acc, asql_inventory * inv_list, int inv_count) {
    devassert(acc->auth_id == account->auth_id);

    if (!success) {
        // retry
        asql_find_or_create_account_async(account);
        goto cleanup;
    }

    account->asql = asql_find_account(account->auth_id);
    if (!account->asql)
        account->asql = MP_ALLOC(asql_account);

    memcpy(account->asql, acc, sizeof(asql_account));
    assert(account->asql->auth_id == account->auth_id);

    assert(stashIntAddPointer(as.accounts, account->asql->auth_id, account->asql, false));

    accountInventory_LoadFinished(account, inv_list, inv_count);
    AccountDb_LoadFinished(account);

cleanup:
    if (inv_list)
        free(inv_list);
}

void asql_find_or_create_account_async(Account * account) {
    assert(!account->asql);

    asql_task * task = asql_get_task();
    task->proc = ASQL_FIND_OR_CREATE_ACCOUNT;
    task->add_update_account.account = account;
    task->add_update_account.acc.auth_id = account->auth_id;
    strncpy(task->add_update_account.acc.name, account->auth_name, sizeof(task->add_update_account.acc.name));
    asql_push_task(task, account->auth_id);
}

static bool asql_update_account(SqlConn conn, asql_account * acc) {
    ssize_t loyalty_bits_size = LOYALTY_SQL_BYTES;
    ssize_t last_email_date_is_null = SQL_NULL_DATA;
    ssize_t free_xfer_date_is_null = SQL_NULL_DATA;

    HSTMT stmt = asql_prepare(conn, ASQL_UPDATE_ACCOUNT, "{CALL dbo.SP_update_account (@auth_id=?, @loyalty_bits=?, @loyalty_point_count=?, @loyalty_points_spent=?, @last_email_date=?, @last_num_emails_sent=?, @free_xfer_date=?)}");
    sqlConnStmtBindParam(stmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &acc->auth_id, sizeof(acc->auth_id), NULL);
    sqlConnStmtBindParam(stmt, 2, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_BINARY, LOYALTY_SQL_BYTES, 0, &acc->loyalty_bits, sizeof(acc->loyalty_bits), &loyalty_bits_size);
    sqlConnStmtBindParam(stmt, 3, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &acc->last_loyalty_point_count, sizeof(acc->last_loyalty_point_count), NULL);
    sqlConnStmtBindParam(stmt, 4, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &acc->loyalty_points_spent, sizeof(acc->loyalty_points_spent), NULL);
    sqlConnStmtBindParam(stmt, 5, SQL_PARAM_INPUT, SQL_C_TYPE_TIMESTAMP, SQL_TYPE_TIMESTAMP, MSSQL_SMALLDATETIME_PRECISION, MSSQL_SMALLDATETIME_DECIMALDIGITS, &acc->last_email_date, sizeof(acc->last_email_date), (acc->last_email_date.year==0) ? &last_email_date_is_null : NULL);
    sqlConnStmtBindParam(stmt, 6, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &acc->last_num_emails_sent, sizeof(acc->last_num_emails_sent), NULL);
    sqlConnStmtBindParam(stmt, 7, SQL_PARAM_INPUT, SQL_C_TYPE_TIMESTAMP, SQL_TYPE_TIMESTAMP, MSSQL_SMALLDATETIME_PRECISION, MSSQL_SMALLDATETIME_DECIMALDIGITS, &acc->free_xfer_date, sizeof(acc->free_xfer_date), (acc->free_xfer_date.year==0) ? &free_xfer_date_is_null : NULL);

    return asql_execute_procedure(conn, ASQL_UPDATE_ACCOUNT);
}

static void asql_update_account_finish(bool success, Account * account, asql_account * acc) {
    // retry on failure
    if (!success)
        asql_update_account_async(account);
}

void asql_update_account_async(Account * account) {
    asql_task * task = asql_get_task();
    task->proc = ASQL_UPDATE_ACCOUNT;
    task->add_update_account.account = account;
    memcpy(&task->add_update_account.acc, account->asql, sizeof(asql_account));
    asql_push_task(task, account->auth_id);
}

static bool asql_add_micro_transaction(SqlConn conn, asql_micro_transaction * mtx, asql_inventory * inv) {
    ssize_t sku_bytes = sizeof(mtx->sku_id);

    HSTMT stmt = asql_prepare(conn, ASQL_ADD_MICRO_TRANSACTION, "{CALL dbo.SP_add_micro_transaction (@order_id=?, @auth_id=?, @sku_id=?, @transaction_date=?, @quantity=?, @points=?)}");
    sqlConnStmtBindParam(stmt, 1, SQL_PARAM_INPUT, SQL_C_GUID, SQL_GUID, 0, 0, &mtx->order_id, sizeof(mtx->order_id), NULL);
    sqlConnStmtBindParam(stmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &mtx->auth_id, sizeof(mtx->auth_id), NULL);
    sqlConnStmtBindParam(stmt, 3, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_CHAR, ARRAY_SIZE(mtx->sku_id.c), 0, &mtx->sku_id.c, sizeof(mtx->sku_id), &sku_bytes);
    sqlConnStmtBindParam(stmt, 4, SQL_PARAM_INPUT, SQL_C_TYPE_TIMESTAMP, SQL_TYPE_TIMESTAMP, MSSQL_DATETIME_PRECISION, MSSQL_DATETIME_DECIMALDIGITS, &mtx->transaction_date, sizeof(mtx->transaction_date), NULL);
    sqlConnStmtBindParam(stmt, 5, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &mtx->quantity, sizeof(mtx->quantity), NULL);
    sqlConnStmtBindParam(stmt, 6, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &mtx->points, sizeof(mtx->points), NULL);

    asql_inventory_bytes bytes;
    asql_bind_inventory_columns(stmt, inv, &bytes);

    if (!asql_execute_procedure(conn, ASQL_ADD_MICRO_TRANSACTION))
        return false;

    int ret = _sqlConnStmtFetch(stmt, conn);
    if (ret != SQL_SUCCESS)
        sqlConnStmtPrintError(stmt, asql_stored_procedure_names[ASQL_ADD_MICRO_TRANSACTION]);
    if(!SQL_SUCCEEDED(ret))
        return false;

    return asql_check_inventory_results(inv, &bytes);
}

static void asql_add_micro_transaction_finish(bool success, MicroTransaction * transaction) {
    Transaction_MicroFinished(success, transaction);
}

void asql_add_micro_transaction_async(MicroTransaction * transaction) {
    asql_task * task = asql_get_task();
    task->proc = ASQL_ADD_MICRO_TRANSACTION;
    task->add_micro.micro_transaction = transaction;
    asql_push_task(task, transaction->account->auth_id);
}

static bool asql_add_game_transaction(SqlConn conn, asql_game_transaction * gtx, asql_inventory * inv) {
    ssize_t sku_bytes = sizeof(gtx->sku_id);
    ssize_t shard_id_is_null = SQL_NULL_DATA;
    ssize_t ent_id_is_null = SQL_NULL_DATA;
    ssize_t granted_is_null = SQL_NULL_DATA;
    ssize_t claimed_is_null = SQL_NULL_DATA;
    ssize_t saved_is_null = SQL_NULL_DATA;

    HSTMT stmt = asql_prepare(conn, ASQL_ADD_GAME_TRANSACTION, "{CALL dbo.SP_add_game_transaction (@order_id=?, @auth_id=?, @sku_id=?, @transaction_date=?, @csr_did_it=?, @shard_id=?, @ent_id=?, @granted=?, @claimed=?)}");
    sqlConnStmtBindParam(stmt, 1, SQL_PARAM_INPUT, SQL_C_GUID, SQL_GUID, 0, 0, &gtx->order_id, sizeof(gtx->order_id), NULL);
    sqlConnStmtBindParam(stmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &gtx->auth_id, sizeof(gtx->auth_id), NULL);
    sqlConnStmtBindParam(stmt, 3, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_CHAR, ARRAY_SIZE(gtx->sku_id.c), 0, &gtx->sku_id.c, sizeof(gtx->sku_id), &sku_bytes);
    sqlConnStmtBindParam(stmt, 4, SQL_PARAM_INPUT, SQL_C_TYPE_TIMESTAMP, SQL_TYPE_TIMESTAMP, MSSQL_DATETIME_PRECISION, MSSQL_DATETIME_DECIMALDIGITS, &gtx->transaction_date, sizeof(gtx->transaction_date), NULL);
    sqlConnStmtBindParam(stmt, 5, SQL_PARAM_INPUT, SQL_C_BIT, SQL_BIT, 0, 0, &gtx->csr_did_it, sizeof(gtx->csr_did_it), 0);
    sqlConnStmtBindParam(stmt, 6, SQL_PARAM_INPUT, SQL_C_TINYINT, SQL_TINYINT, 0, 0, &gtx->shard_id, sizeof(gtx->shard_id), (gtx->shard_id>0) ? NULL : &shard_id_is_null);
    sqlConnStmtBindParam(stmt, 7, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &gtx->ent_id, sizeof(gtx->ent_id), (gtx->ent_id>0) ? NULL : &ent_id_is_null);
    sqlConnStmtBindParam(stmt, 8, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &gtx->granted, sizeof(gtx->granted), (gtx->granted!=0) ? NULL : &granted_is_null);
    sqlConnStmtBindParam(stmt, 9, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &gtx->claimed, sizeof(gtx->claimed), (gtx->claimed!=0 || gtx->granted==0) ? NULL : &claimed_is_null);

    asql_inventory_bytes bytes;
    asql_bind_inventory_columns(stmt, inv, &bytes);

    if (!asql_execute_procedure(conn, ASQL_ADD_GAME_TRANSACTION))
        return false;

    int ret = _sqlConnStmtFetch(stmt, conn);
    if (ret != SQL_SUCCESS)
        sqlConnStmtPrintError(stmt, asql_stored_procedure_names[ASQL_ADD_GAME_TRANSACTION]);
    if(!SQL_SUCCEEDED(ret))
        return false;

    return asql_check_inventory_results(inv, &bytes);
}

static void asql_add_game_transaction_finish(bool success, GameTransaction * transaction) {
    Transaction_GameFinished(success, transaction);
}

void asql_add_game_transaction_async(GameTransaction * transaction) {
    asql_task * task = asql_get_task();
    task->proc = ASQL_ADD_GAME_TRANSACTION;
    task->add_game.game_transaction = transaction;
    asql_push_task(task, transaction->account->auth_id);
}

// assumes an arbitrary number of output select statements in the SQL
// assumes precisely one resulting row from each select statement
static bool asql_read_inventory_rows(SqlConn conn, HSTMT stmt, asql_flexible_inventory * flex_inv, asql_inventory * inv, asql_inventory_bytes *bytes, const char *original_command) {
    bool ok = true;    

    asql_init_flexible_inventory(flex_inv);

    do {
        SQLRETURN ret;
        SQLSMALLINT num_cols;

        ret = SQLNumResultCols(stmt, &num_cols);
        if (ret != SQL_SUCCESS)
            sqlConnStmtPrintError(stmt, original_command);

        if (SQL_SUCCEEDED(ret) && num_cols) {
            ret = _sqlConnStmtFetch(stmt, conn);
            if (ret != SQL_SUCCESS)
                sqlConnStmtPrintError(stmt, original_command);
            if (ret < 0)
                ok = false;

            if (SQL_SUCCEEDED(ret)) {
                if (!asql_check_inventory_results(inv, bytes))
                    ok = false;

                asql_add_flexible_inventory(flex_inv, inv);

                ret = _sqlConnStmtFetch(stmt, conn);
                devassert(ret == SQL_NO_DATA);
            }
        }

        ret = SQLMoreResults(stmt);
        if (ret == SQL_NO_DATA)
            break;

        if (ret != SQL_SUCCESS) {
            sqlConnStmtPrintError(stmt, original_command);
            asql_consume_all_errors(stmt, original_command);
            return false;
        }
    } while (true);

    return ok;
}

static bool asql_add_multi_game_transaction(SqlConn conn, MultiGameTransaction *transaction) {
    // The order here must match the order in the TVP_game_transaction type in the database!
    OrderId order_ids[MAX_MULTI_GAME_TRANSACTIONS];

    U32 auth_ids[MAX_MULTI_GAME_TRANSACTIONS];

    SkuId sku_ids[MAX_MULTI_GAME_TRANSACTIONS];
    ssize_t sku_bytes[MAX_MULTI_GAME_TRANSACTIONS];

    SQL_TIMESTAMP_STRUCT transaction_dates[MAX_MULTI_GAME_TRANSACTIONS];

    U8 shard_ids[MAX_MULTI_GAME_TRANSACTIONS];
    ssize_t shard_bytes[MAX_MULTI_GAME_TRANSACTIONS];

    U32 ent_ids[MAX_MULTI_GAME_TRANSACTIONS];
    ssize_t ent_bytes[MAX_MULTI_GAME_TRANSACTIONS];

    long granted_values[MAX_MULTI_GAME_TRANSACTIONS];
    ssize_t granted_bytes[MAX_MULTI_GAME_TRANSACTIONS];

    long claimed_values[MAX_MULTI_GAME_TRANSACTIONS];
    ssize_t claimed_bytes[MAX_MULTI_GAME_TRANSACTIONS];

    U8 csr_did_its[MAX_MULTI_GAME_TRANSACTIONS];

    for (signed index = 0; index < transaction->count; index++) {
        order_ids[index] = transaction->transactions[index].order_id;

        auth_ids[index] = transaction->transactions[index].auth_id;

        sku_ids[index] = transaction->transactions[index].sku_id;
        sku_bytes[index] = sizeof(SkuId);

        transaction_dates[index] = transaction->transactions[index].transaction_date;

        shard_ids[index] = transaction->transactions[index].shard_id;
        shard_bytes[index] = shard_ids[index] ? sizeof(shard_ids[index]) : SQL_NULL_DATA;

        ent_ids[index] = transaction->transactions[index].ent_id;
        ent_bytes[index] = ent_ids[index] ? sizeof(ent_ids[index]) : SQL_NULL_DATA;

        granted_values[index] = transaction->transactions[index].granted;
        granted_bytes[index] = granted_values[index] ? sizeof(granted_values[index]) : SQL_NULL_DATA;

        claimed_values[index] = transaction->transactions[index].claimed;
        claimed_bytes[index] = (claimed_values[index] || !granted_values[index]) ? sizeof(claimed_values[index]) : SQL_NULL_DATA;

        csr_did_its[index] = transaction->transactions[index].csr_did_it;
    }

    HSTMT stmt = asql_prepare(conn, ASQL_ADD_MULTI_GAME_TRANSACTION, "{CALL dbo.SP_add_multi_game_transaction (@game_transactions=?, @parent_order_id=?)}");

    sqlConnStmtStartBindParamTableValued(stmt, 1, MAX_MULTI_GAME_TRANSACTIONS, L"TVP_game_transaction", &transaction->count);
    sqlConnStmtBindParam(stmt, 1, SQL_PARAM_INPUT, SQL_C_GUID, SQL_GUID, 0, 0, order_ids, sizeof(order_ids[0]), NULL);
    sqlConnStmtBindParam(stmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, auth_ids, sizeof(auth_ids[0]), NULL);
    sqlConnStmtBindParam(stmt, 3, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_CHAR, ARRAY_SIZE(sku_ids->c), 0, sku_ids, sizeof(sku_ids[0]), sku_bytes);
    sqlConnStmtBindParam(stmt, 4, SQL_PARAM_INPUT, SQL_C_TYPE_TIMESTAMP, SQL_TYPE_TIMESTAMP, MSSQL_DATETIME_PRECISION, MSSQL_DATETIME_DECIMALDIGITS, transaction_dates, sizeof(transaction_dates[0]), NULL);
    sqlConnStmtBindParam(stmt, 5, SQL_PARAM_INPUT, SQL_C_TINYINT, SQL_TINYINT, 0, 0, shard_ids, sizeof(shard_ids[0]), shard_bytes);
    sqlConnStmtBindParam(stmt, 6, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, ent_ids, sizeof(ent_ids[0]), ent_bytes);
    sqlConnStmtBindParam(stmt, 7, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, granted_values, sizeof(granted_values[0]), granted_bytes);
    sqlConnStmtBindParam(stmt, 8, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, claimed_values, sizeof(claimed_values[0]), claimed_bytes);
    sqlConnStmtBindParam(stmt, 9, SQL_PARAM_INPUT, SQL_C_BIT, SQL_BIT, 0, 0, csr_did_its, sizeof(csr_did_its[0]), NULL);
    sqlConnStmtEndBindParamTableValued(stmt);

    sqlConnStmtBindParam(stmt, 2, SQL_PARAM_INPUT, SQL_C_GUID, SQL_GUID, 0, 0, &transaction->order_id, sizeof(transaction->order_id), NULL);

    asql_inventory inv;
    asql_inventory_bytes bytes;
    asql_bind_inventory_columns(stmt, &inv, &bytes);

    if (!asql_execute_procedure(conn, ASQL_ADD_MULTI_GAME_TRANSACTION))
        return false;

    if (!asql_read_inventory_rows(conn, stmt, &transaction->flex_inv, &inv, &bytes, asql_stored_procedure_names[ASQL_ADD_MULTI_GAME_TRANSACTION]))
        return false;
    if (!devassert(transaction->flex_inv.count == transaction->count))
        return false;

    return true;
}

static void asql_add_multi_game_transaction_finish(bool success, MultiGameTransaction * transaction) {
    Transaction_MultiGameFinished(success, transaction);
    asql_free_flexible_inventory(&transaction->flex_inv);
}

void asql_add_multi_game_transaction_async(MultiGameTransaction * transaction) {
    asql_task * task = asql_get_task();
    task->proc = ASQL_ADD_MULTI_GAME_TRANSACTION;
    task->add_multi_game.multi_game_transaction = transaction;
    asql_push_task(task, transaction->account->auth_id);
}

static bool asql_save_game_transaction(SqlConn conn, U32 auth_id, OrderId order_id, asql_flexible_inventory * flex_inv) {
    HSTMT stmt = asql_prepare(conn, ASQL_SAVE_GAME_TRANSACTION, "{CALL dbo.SP_save_game_transaction (@auth_id=?, @order_id=?)}");
    sqlConnStmtBindParam(stmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &auth_id, sizeof(auth_id), NULL);
    sqlConnStmtBindParam(stmt, 2, SQL_PARAM_INPUT, SQL_C_GUID, SQL_GUID, 0, 0, &order_id, sizeof(order_id), NULL);

    asql_inventory inv;
    asql_inventory_bytes bytes;
    asql_bind_inventory_columns(stmt, &inv, &bytes);

    if (!asql_execute_procedure(conn, ASQL_SAVE_GAME_TRANSACTION))
        return false;

    return asql_read_inventory_rows(conn, stmt, flex_inv, &inv, &bytes, asql_stored_procedure_names[ASQL_SAVE_GAME_TRANSACTION]);
}

static void asql_save_game_transaction_finish(bool success, Account * acc, OrderId order_id, asql_flexible_inventory * flex_inv) {
    if (success)
        accountInventory_UpdateInventoryFromFlexSQL(acc, flex_inv);

    asql_free_flexible_inventory(flex_inv);

    AccountServer_NotifySaved(acc, order_id, success);
}

void asql_save_game_transaction_async(Account * acc, OrderId order_id) {
    asql_task * task = asql_get_task();
    task->proc = ASQL_SAVE_GAME_TRANSACTION;
    task->save_revert_game.account = acc;
    task->save_revert_game.order_id = order_id;
    asql_push_task(task, acc->auth_id);
}

static bool asql_revert_game_transaction(SqlConn conn, U32 auth_id, OrderId order_id, asql_flexible_inventory * flex_inv) {
    HSTMT stmt = asql_prepare(conn, ASQL_REVERT_GAME_TRANSACTION, "{CALL dbo.SP_revert_game_transaction (@auth_id=?, @search_id=?)}");
    sqlConnStmtBindParam(stmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &auth_id, sizeof(auth_id), NULL);
    sqlConnStmtBindParam(stmt, 2, SQL_PARAM_INPUT, SQL_C_GUID, SQL_GUID, 0, 0, &order_id, sizeof(order_id), NULL);

    asql_inventory inv;
    asql_inventory_bytes bytes;
    asql_bind_inventory_columns(stmt, &inv, &bytes);

    if (!asql_execute_procedure(conn, ASQL_REVERT_GAME_TRANSACTION))
        return false;

    return asql_read_inventory_rows(conn, stmt, flex_inv, &inv, &bytes, asql_stored_procedure_names[ASQL_REVERT_GAME_TRANSACTION]);
}

static void asql_revert_game_transaction_finish(bool success, Account * acc, OrderId order_id, asql_flexible_inventory * flex_inv) {
    if (success)
        accountInventory_UpdateInventoryFromFlexSQL(acc, flex_inv);

    asql_free_flexible_inventory(flex_inv);
}

void asql_revert_game_transaction_async(Account * acc, OrderId order_id) {
    asql_task * task = asql_get_task();
    task->proc = ASQL_REVERT_GAME_TRANSACTION;
    task->save_revert_game.account = acc;
    task->save_revert_game.order_id = order_id;
    asql_push_task(task, acc->auth_id);
}

static void asql_bind_game_transaction_columns(HSTMT stmt, asql_game_transaction * gtx, asql_game_transaction_bytes * bytes) {
    sqlConnStmtBindCol(stmt, 1, SQL_C_GUID, &gtx->order_id, sizeof(gtx->order_id), &bytes->order_id_bytes);
    sqlConnStmtBindCol(stmt, 2, SQL_C_LONG, &gtx->auth_id, sizeof(gtx->auth_id), &bytes->auth_id_bytes);
    sqlConnStmtBindCol(stmt, 3, SQL_C_BINARY, &gtx->sku_id.c, sizeof(gtx->sku_id), &bytes->sku_id_bytes);
    sqlConnStmtBindCol(stmt, 4, SQL_C_TYPE_TIMESTAMP, &gtx->transaction_date, sizeof(gtx->transaction_date), &bytes->transaction_date_bytes);
    sqlConnStmtBindCol(stmt, 5, SQL_C_TINYINT, &gtx->shard_id, sizeof(gtx->shard_id), &bytes->shard_id_bytes);
    sqlConnStmtBindCol(stmt, 6, SQL_C_LONG, &gtx->ent_id, sizeof(gtx->ent_id), &bytes->ent_id_bytes);
    sqlConnStmtBindCol(stmt, 7, SQL_C_LONG, &gtx->granted, sizeof(gtx->granted), &bytes->granted_bytes);
    sqlConnStmtBindCol(stmt, 8, SQL_C_LONG, &gtx->claimed, sizeof(gtx->claimed), &bytes->claimed_bytes);
    sqlConnStmtBindCol(stmt, 9, SQL_C_BIT, &gtx->csr_did_it, sizeof(gtx->csr_did_it), &bytes->csr_did_it_bytes);
}

static bool asql_check_game_transaction_results(asql_game_transaction * gtx, asql_game_transaction_bytes * bytes) {
    bool ok = true;

    ok &= devassert(bytes->order_id_bytes == sizeof(gtx->order_id));
    ok &= devassert(bytes->auth_id_bytes == sizeof(gtx->auth_id));
    ok &= devassert(bytes->sku_id_bytes == sizeof(gtx->sku_id));
    ok &= devassert(bytes->transaction_date_bytes == sizeof(gtx->transaction_date));
    ok &= devassert(bytes->csr_did_it_bytes == sizeof(gtx->csr_did_it));

    if (bytes->shard_id_bytes == SQL_NULL_DATA)
        gtx->shard_id = 0;
    else
        ok &= devassert(bytes->shard_id_bytes == sizeof(gtx->shard_id));

    if (bytes->ent_id_bytes == SQL_NULL_DATA)
        gtx->ent_id = 0;
    else
        ok &= devassert(bytes->ent_id_bytes == sizeof(gtx->ent_id));

    if (bytes->granted_bytes == SQL_NULL_DATA)
        gtx->granted = 0;
    else
        ok &= devassert(bytes->granted_bytes == sizeof(gtx->granted));

    if (bytes->claimed_bytes == SQL_NULL_DATA)
        gtx->claimed = 0;
    else
        ok &= devassert(bytes->claimed_bytes == sizeof(gtx->claimed));

    return ok;
}

static bool asql_read_unsaved_game_transactions(SqlConn conn, U32 auth_id, U8 shard_id, U32 ent_id, asql_game_transaction ** gtx_list, int * gtx_count) {
    *gtx_count = 0;
    *gtx_list = NULL;

    ssize_t shard_id_is_null = SQL_NULL_DATA;
    ssize_t ent_id_is_null = SQL_NULL_DATA;

    HSTMT stmt = asql_prepare(conn, ASQL_READ_UNSAVED_GAME_TRANSACTIONS, "{CALL dbo.SP_read_unsaved_game_transactions (@auth_id=?, @shard_id=?, @ent_id=?)}");
    sqlConnStmtBindParam(stmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &auth_id, sizeof(auth_id), NULL);
    sqlConnStmtBindParam(stmt, 2, SQL_PARAM_INPUT, SQL_C_TINYINT, SQL_TINYINT, 0, 0, &shard_id, sizeof(shard_id), (shard_id>0) ? NULL : &shard_id_is_null);
    sqlConnStmtBindParam(stmt, 3, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &ent_id, sizeof(ent_id), (ent_id>0) ? NULL : &ent_id_is_null);

    asql_game_transaction gtx;
    asql_game_transaction_bytes bytes;
    asql_bind_game_transaction_columns(stmt, &gtx, &bytes);

    if (!asql_execute_procedure(conn, ASQL_READ_UNSAVED_GAME_TRANSACTIONS))
        return false;

    int limit = 0;
    int ret;
    while ((ret = _sqlConnStmtFetch(stmt, conn)) != SQL_NO_DATA) {
        if (ret != SQL_SUCCESS)
            sqlConnStmtPrintError(stmt, asql_stored_procedure_names[ASQL_READ_UNSAVED_GAME_TRANSACTIONS]);

        if(!SQL_SUCCEEDED(ret))
            continue;

        if (!asql_check_game_transaction_results(&gtx, &bytes))
            return false;

        void * next = dynArrayFit(reinterpret_cast<void**>(gtx_list), sizeof(asql_game_transaction), &limit, (*gtx_count)++);
        memcpy(next, &gtx, sizeof(asql_game_transaction));
    }

    return true;
}

static void asql_read_unsaved_game_transactions_finish(bool success, Account * acc, asql_game_transaction * gtx_list, int gtx_count) {
    Transaction_GameRecoverUnsavedCallback(success, acc, gtx_list, gtx_count);
}

void asql_read_unsaved_game_transactions_async(Account * acc, U8 shard_id, U32 ent_id) {
    asql_task * task = asql_get_task();
    task->proc = ASQL_READ_UNSAVED_GAME_TRANSACTIONS;
    task->read_game.account = acc;
    task->read_game.shard_id = shard_id;
    task->read_game.ent_id = ent_id;
    asql_push_task(task, acc->auth_id);
}

static void * asql_run_entry(void * data, void * user)
{
    asql_task * task = static_cast<asql_task*>(data);
    SqlConn conn = (SqlConn)(ptrdiff_t)user;
    
    switch (task->proc) {
        case ASQL_SEARCH_FOR_ACCOUNT:
            task->success = asql_search_for_account(conn, task->search_for_account.search_for, &task->search_for_account.found_auth_id);
            break;
        case ASQL_FIND_OR_CREATE_ACCOUNT:
            task->success = asql_find_or_create_account(conn, &task->add_update_account.acc, &task->add_update_account.inv_list, &task->add_update_account.inv_count);
            break;
        case ASQL_UPDATE_ACCOUNT:
            task->success = asql_update_account(conn, &task->add_update_account.acc);
            break;
        case ASQL_ADD_MICRO_TRANSACTION:
            task->success = asql_add_micro_transaction(conn, &task->add_micro.micro_transaction->mtx, &task->add_micro.micro_transaction->inv);
            break;
        case ASQL_ADD_GAME_TRANSACTION:
            task->success = asql_add_game_transaction(conn, &task->add_game.game_transaction->gtx, &task->add_game.game_transaction->inv);
            break;
        case ASQL_ADD_MULTI_GAME_TRANSACTION:
            task->success = asql_add_multi_game_transaction(conn, task->add_multi_game.multi_game_transaction);
            break;
        case ASQL_SAVE_GAME_TRANSACTION:
            task->success = asql_save_game_transaction(conn, task->save_revert_game.account->auth_id, task->save_revert_game.order_id, &task->save_revert_game.flex_inv);
            break;
        case ASQL_REVERT_GAME_TRANSACTION:
            task->success = asql_revert_game_transaction(conn, task->save_revert_game.account->auth_id, task->save_revert_game.order_id, &task->save_revert_game.flex_inv);
            break;
        case ASQL_READ_UNSAVED_GAME_TRANSACTIONS:
            task->success = asql_read_unsaved_game_transactions(conn, task->read_game.account->auth_id, task->read_game.shard_id, task->read_game.ent_id, &task->read_game.gtx_list, &task->read_game.gtx_count);
            break;
    }

    // make sure the cursor is closed, as there is only one open cursor per connection allowed
    sqlConnStmtCloseCursor(as.conn[conn].proc[task->proc]);

    return data;
}

static void asql_finish_entry(void * data)
{
    asql_task * task = (asql_task*)data;
    
    if (!--as.in_flight)
        sql_task_freeze();

    switch (task->proc) {
        case ASQL_SEARCH_FOR_ACCOUNT:
            asql_search_for_account_finish(task->success, task->search_for_account.search_for, task->search_for_account.found_auth_id);
            break;
        case ASQL_FIND_OR_CREATE_ACCOUNT:
            asql_find_or_create_account_finish(task->success, task->add_update_account.account, &task->add_update_account.acc, task->add_update_account.inv_list, task->add_update_account.inv_count);
            break;
        case ASQL_UPDATE_ACCOUNT:
            asql_update_account_finish(task->success, task->add_update_account.account, &task->add_update_account.acc);
            break;
        case ASQL_ADD_MICRO_TRANSACTION:
            asql_add_micro_transaction_finish(task->success, task->add_micro.micro_transaction);
            break;
        case ASQL_ADD_GAME_TRANSACTION:
            asql_add_game_transaction_finish(task->success, task->add_game.game_transaction);
            break;
        case ASQL_ADD_MULTI_GAME_TRANSACTION:
            asql_add_multi_game_transaction_finish(task->success, task->add_multi_game.multi_game_transaction);
            break;
        case ASQL_SAVE_GAME_TRANSACTION:
            asql_save_game_transaction_finish(task->success, task->save_revert_game.account, task->save_revert_game.order_id, &task->save_revert_game.flex_inv);
            break;
        case ASQL_REVERT_GAME_TRANSACTION:
            asql_revert_game_transaction_finish(task->success, task->save_revert_game.account, task->save_revert_game.order_id, &task->save_revert_game.flex_inv);
            break;
        case ASQL_READ_UNSAVED_GAME_TRANSACTIONS:
            asql_read_unsaved_game_transactions_finish(task->success, task->read_game.account, task->read_game.gtx_list, task->read_game.gtx_count);
            break;
    }

    MP_FREE(asql_task, data);
}
