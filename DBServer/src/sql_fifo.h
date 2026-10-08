#ifndef SQL_FIFO_H
#define SQL_FIFO_H

#include "container_sql.h"
#include <utilitieslib/network/netio.h>
#include "..\Common\ClientLogin\clientcommLogin.h"

#ifndef SQL_NTS
#define SQL_NTS                   (-3)
#endif

typedef void (*ReadColumnCallback)(Packet *pak, U8 *cols, int col_count, ColumnInfo **field_ptrs, void *data);

typedef struct ContainerTemplate ContainerTemplate;
typedef struct DbContainer DbContainer;
typedef struct LineList LineList;

void sqlFifoInit(void);
void sqlFifoShutdown(void);

void sqlFifoTick(void);
void sqlFifoFinish(void);
/* Drain earlier SQLite work before a foreground operation; suppressed while
 * executing a queue item, so deferred callbacks cannot run inside a statement. */
void sqlFifoFlushBeforeForeground(void);
void sqlFifoBarrier(void);
int sqlFifoWorstQueueDepth(void);
void sqlFifoEnableTransacted(int enable);

// HACK
void sqlFifoDisableNMMonitorIfMemoryLow();

// synchronous
char *sqlReadColumnsSlow(TableInfo *table, char *limit, char *col_names, char *where, int *count_ptr, ColumnInfo **field_ptrs);

// asynchronous
bool sqlIsAsyncReadPending(int list_id, int container_id);
void sqlFifoTickWhileReadPending(int list_id, int container_id);
bool sqlIsAsyncWritePending(int list_id, int container_id);
void sqlFifoTickWhileWritePending(int list_id, int container_id);
void sqlReadContainerAsync(int list_id, int container_id, NetLink *link, GameClientLink *game, void *input_data);
void sqlContainerUpdateAsync(ContainerTemplate *tplt, int container_id, LineList *diff_list);
void sqlContainerUpdateFromScratchAsync(ContainerTemplate *tplt, int container_id, LineList *line_list, bool single_table);
void sqlReadColumnsAsync(TableInfo *table, char *limit, char *col_names, char *where, ReadColumnCallback callback, void* callbackData, Packet *pak, int container_id);
void sqlReadColumnsAsyncWithDebugDelay(TableInfo *table, char *limit, char *col_names, char *where, ReadColumnCallback callback, void* callbackData, Packet *pak, int container_id, int debugDelayMS);
void sqlDeleteRowAsync(char *table, int container_id);
void sqlExecAsync(char *str, int str_len);
void sqlExecAsyncEx(char *str, int str_len, int container_id, bool utf8);

#endif

