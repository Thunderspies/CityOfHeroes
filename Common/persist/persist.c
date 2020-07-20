#include "utilitieslib/stdtypes.h"
#include "persist_internal.h"
#include "persist_flat.h"
#include "persist_diffbig.h"
#include "persist_diffsmall.h"
// #include "persist_sql.h"

#include "utilitieslib/utils/wininclude.h"
#include "utilitieslib/assert/assert.h"
#include "utilitieslib/components/earray.h"
#include "utilitieslib/components/StashTable.h"
#include "utilitieslib/utils/textparser.h"
#include "utilitieslib/utils/tokenstore.h"
#include "utilitieslib/utils/utils.h"
#include "utilitieslib/utils/error.h"
#include "utilitieslib/utils/timing.h"
#include "utilitieslib/utils/fileutil.h"
#include "utilitieslib/components/HashFunctions.h"
#include "utilitieslib/utils/sysutil.h"
#include "utilitieslib/utils/log.h"

#include "utilitieslib/UtilsNew/Bits.h"
#include "utilitieslib/UtilsNew/Str.h"

typedef int(*persist_handler)(PersistInfo*);
typedef int(*persist_merger)(PersistInfo*,int write);
typedef DirtyType(*persist_structer)(PersistInfo*,void*);

static StashTable s_infos;
static PersistInfo **s_infolist;
static CRITICAL_SECTION s_mergecs;
static int s_syncedmergetimer;
static int s_syncedmergetime;

static int panicker_memory(PersistInfo *info) { return 1; } // don't bother trying to persist a memory-only dictionary in an emergency
static DirtyType structer_memory(PersistInfo *info, void *structptr) { return DIRTY_CLEAN; } // there's nothing to update, so the db is always clean

static persist_handler s_initializers[] =
{
    NULL,
    NULL,
    initializer_diffbig,
    initializer_diffsmall,
//     initializer_sql,
};
STATIC_ASSERT(ARRAY_SIZE(s_initializers) == PERSIST_COUNT);

static persist_handler s_loaders[] =
{
    NULL,
    loader_flat,
    loader_diffbig,
    loader_diffsmall,
//     loader_sql,
};
STATIC_ASSERT(ARRAY_SIZE(s_loaders) == PERSIST_COUNT);

static persist_merger s_mergers[] = // NULL means no journal
{
    NULL,
    NULL,
    merger_diffbig,
    merger_diffsmall,
//     NULL,
};
STATIC_ASSERT(ARRAY_SIZE(s_mergers) == PERSIST_COUNT);

static persist_structer s_adders[] =
// called when adding new structs, return value is the dirty state of the struct
// NULL here means the struct is always dirty after being added (no implicit flush)
// if the struct is dirty, it will additionally be passed to the backend's dirtier
{
    structer_memory,
    NULL,
    adder_diffbig,
    NULL, // handled by dirtier_diffsmall
//     adder_sql,
};
STATIC_ASSERT(ARRAY_SIZE(s_adders) == PERSIST_COUNT);

static persist_structer s_dirtiers[] =
// called from persist_Dirty and may be called when a new struct is added
// return value is the dirty state of the struct
// NULL here means the struct is always dirty (no implicit flush)
{
    structer_memory,
    NULL,
    NULL,
    dirtier_diffsmall,
//     dirtier_sql,
};
STATIC_ASSERT(ARRAY_SIZE(s_dirtiers) == PERSIST_COUNT);

static persist_structer s_journaler[] =
// called from persist_Dirty when a custom journal line is specified
// return value is the dirty state of the struct
// NULL here means use s_dirtiers on the struct instead
{
    structer_memory,
    NULL,
    NULL,
    journaler_diffsmall,
//     NULL,
};
STATIC_ASSERT(ARRAY_SIZE(s_dirtiers) == PERSIST_COUNT);

static persist_structer s_removers[] =
// called from persist_RemoveAndDestroy
// return value is the dirty state of the db
// NULL here means the removal always makes the db dirty (no implicit flush)
{
    structer_memory,
    NULL,
    remover_diffbig,
    remover_diffsmall,
//     remover_sql,
};
STATIC_ASSERT(ARRAY_SIZE(s_removers) == PERSIST_COUNT);

static persist_handler s_tickers[] =
{
    NULL,
    NULL,
    NULL,
    ticker_diffsmall,
//     NULL,
};
STATIC_ASSERT(ARRAY_SIZE(s_tickers) == PERSIST_COUNT);

static persist_handler s_flushers[] = // NULL means flushes are implicit
{
    NULL,
    flusher_flat,
    flusher_diffbig,
    flusher_diffsmall,
//     NULL,
};
STATIC_ASSERT(ARRAY_SIZE(s_flushers) == PERSIST_COUNT);

static persist_handler s_syncers[] = // NULL means 'never flushing'
{
    // these must be thread-safe
    NULL,
    NULL,
    syncer_diffbig,
    NULL
//     syncer_sql,
};
STATIC_ASSERT(ARRAY_SIZE(s_syncers) == PERSIST_COUNT);

static persist_handler s_panickers[] = // NULL means dump to disk
{
    panicker_memory,
    NULL,
    NULL,
    NULL,
//     panicker_sql,
};
STATIC_ASSERT(ARRAY_SIZE(s_panickers) == PERSIST_COUNT);

// Internal ///////////////////////////////////////////////////////////////////

// a little hack to allow looking for 0s in a stash (adding 0 is still incorrect)
bool stashIntFindPointerHack(cStashTable table, int iKey, void** ppValue)
{
    if(iKey)
        return stashIntFindPointer(table, iKey, ppValue);
    if(ppValue)
        *ppValue = NULL;
    return 0;
}

static int s_isInfoLoaded(PersistInfo *info)
{
    return info->state == PERSISTINFO_CLEAN || info->state == PERSISTINFO_DIRTY;
}

static int s_isInfoFlushing(PersistInfo *info)
{
    if(!s_isInfoLoaded(info) && info->state != PERSISTINFO_QUITTING)
        return 0;
    // if there's no handler to check, the backend is always synchronous
    return s_syncers[info->config->type] ? s_syncers[info->config->type](info) : 0;
}

static int s_isInfoMerging(PersistInfo *info)
{
    if(info->merging == PERSISTINFO_MERGING)
    {
        // the only reasonable way for an info to be in the merging state is if
        // it merges in a background thread.  this check shouldn't be called
        // from merge routines (which should already know the merge state), and
        // synchronous merges should be done by now.
        assert(!(info->config->flags & PERSIST_NOTHREADS));
        if(info->merge_thread)
            return 1;
        else // the thread has finished
            info->merging = PERSISTINFO_NOTMERGING;
    }

    return 0;
}

static void s_waitForFlush(PersistInfo *info, int wait_for_merge)
{
    while(    s_isInfoFlushing(info) ||
            (wait_for_merge && s_isInfoMerging(info)) )
    {
        Sleep(1);
    }
}

static void s_startFlush(PersistInfo *info, int quitting)
{
    assert(info->merging != PERSISTINFO_MERGEPENDING);
        // this info already flushed and requested a merge that was never
        // handled.  this is an error in the main system, not the backend.

    if(s_isInfoMerging(info))
    {
        LOG(LOG_TEXT_JOURNAL, LOG_LEVEL_VERBOSE, LOG_CONSOLE_ALWAYS, "Warning: merge process for %s still running, will try again later.", info->typeName);
        return;
    }

    if(s_isInfoFlushing(info))
    {
        LOG(LOG_TEXT_JOURNAL, LOG_LEVEL_VERBOSE, LOG_CONSOLE_ALWAYS, "Warning: starting a flush while one is in progress, waiting for previous flush to finish");
        s_waitForFlush(info, 0);
    }

    assert(info->merging != PERSISTINFO_MERGEPENDING);
        // this info previously started a flush and did not request a merge,
        // but has now requested one. this  is not allowed.  backends must
        // request a merge during the flush-start step and the main system will
        // automatically wait for the flush to finish before merging.  this is
        // an error 


    if(    s_flushers[info->config->type] &&
        !s_flushers[info->config->type](info) )
    {
        FatalErrorf("Could not start flushing %s data, please see the console and logs for more information.", info->typeName);
    }

    if(quitting)
    {
        // infos that are quitting won't try to merge, in the interest of
        // shutting down quickly. merging can be safely handled on the next load
        info->state = PERSISTINFO_QUITTING;
        if (info->merge_recovery)
        {
            // attempt one more flush - to catch outstanding diffs
            if(    s_flushers[info->config->type] &&
                !s_flushers[info->config->type](info) )
            {
                FatalErrorf("Could not start flushing %s data, please see the console and logs for more information.", info->typeName);
            }
            info->merge_recovery = false;
        }
    }
    else
    {
        if (info->merge_recovery)
        {
            info->merge_recovery = false;
        } else {
            info->state = PERSISTINFO_CLEAN;
            if(info->timer)
                timerStart(info->timer);
        }
    }
}

void persist_startMerge(PersistInfo *info)
{
    assert(info->merging == PERSISTINFO_NOTMERGING); // FIXME: info->merging == PERSISTINFO_MERGEPENDING could be valid here
    assert(s_flushers[info->config->type]); // merging is not currently done if there is no flush
    info->merging = PERSISTINFO_MERGEPENDING;
}

static void s_mergeChangesSync(PersistInfo **infos, int count)
{
    extern char g_exe_name[];
    char *sys_call = Str_temp();
    int i;

    Str_catf(&sys_call, "\"%s\"", getExecutableName());
    for(i = 0; i < count; i++)
    {
        PersistInfo *info = infos[i];
        assert(info->merging == PERSISTINFO_MERGING);
        if(s_syncers[info->config->type]) // don't use s_waitForFlush, which checks members owned by the main thread
            while(s_syncers[info->config->type](info))
                Sleep(1);
        Str_catf(&sys_call, " -plmerge %s", info->typeName);
    }
    Str_catf(&sys_call, " -quit");

    system(sys_call);

    Str_destroy(&sys_call);
}

static DWORD WINAPI s_thread_mergeChanges(LPVOID lpParam)
{
    EXCEPTION_HANDLER_BEGIN

    PersistInfo **infos = lpParam;
    int i, count = ap_size(&infos);

    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_LOWEST);

    s_mergeChangesSync(infos, count);

    EnterCriticalSection(&s_mergecs);
    for(i = 0; i < count; i++)
        infos[i]->merge_thread = 0;
    LeaveCriticalSection(&s_mergecs);

    ap_destroy(&infos, NULL);

    EXCEPTION_HANDLER_END
    return 0;
}

static void s_mergeChanges(PersistInfo **infos, int count)
{
    int i;
    PersistInfo **sync = (PersistInfo**)ap_temp();
    PersistInfo **async = NULL; // given to s_mergerthread

    for(i = 0; i < count; i++)
    {
        PersistInfo *info = infos[i];
        assert(info->merging == PERSISTINFO_MERGEPENDING);
        if(info->state != PERSISTINFO_QUITTING)
        {
            info->merging = PERSISTINFO_MERGING;
            ap_push((info->config->flags & PERSIST_NOTHREADS) ? &sync : &async, info);
        }
    }

    if(async) // ap_size(&async)
    {
        HANDLE thread;
        int threadid = 1;
        EnterCriticalSection(&s_mergecs);
        thread = CreateThread(0, 0, s_thread_mergeChanges, async, 0, &threadid);
        assert(threadid);
        for(i = 0; i < ap_size(&async); i++)
            async[i]->merge_thread = threadid;
        CloseHandle(thread); // let it run and exit on its own, coordinate via info->merge_thread
        LeaveCriticalSection(&s_mergecs);
    }

    if(ap_size(&sync))
    {
        s_mergeChangesSync(sync, ap_size(&sync));
        for(i = 0; i < ap_size(&sync); i++)
            infos[i]->merging = PERSISTINFO_NOTMERGING;
    }

    ap_destroy(&sync, NULL);
}

unsigned int persist_hashStruct(PersistInfo *info, const void *structptr, int hashSeed)
{
    ParseTable *tpi = info->settings.tpi;
    int i;
    for(i = 0; i < eaiSize(&info->key_idxs); i++)
    {
        int column = info->key_idxs[i];
        switch(TOK_GET_TYPE(tpi[column].type))
        {
            xcase TOK_U8_X:
            {
                int val = TokenStoreGetU8(tpi, column, (void*)structptr, 0);
                hashSeed = burtlehash2(&val, 1, hashSeed);
            }
            xcase TOK_INT_X:
            {
                int val = TokenStoreGetInt(tpi, column, (void*)structptr, 0);
                hashSeed = burtlehash2(&val, 1, hashSeed);
            }
            xcase TOK_STRING_X:
            {
                char *val = TokenStoreGetString(tpi, column, (void*)structptr, 0);
                hashSeed = burtlehash3StashDefault(val, (ub4)strlen(val), hashSeed);
            }
            xdefault:
                assertmsg(0, "unsupported key type");
        }
    }
    return hashSeed;
}

int persist_compStruct(PersistInfo *info, const void *struct1, const void *struct2)
{
    ParseTable *tpi = info->settings.tpi;
    int i, it = 0;
    for(i = 0; i < eaiSize(&info->key_idxs); i++)
    {
        int column = info->key_idxs[i];
        switch(TOK_GET_TYPE(tpi[column].type))
        {
            xcase TOK_U8_X:
                it = TokenStoreGetU8(tpi, column, (void*)struct1, 0) - TokenStoreGetU8(tpi, column, (void*)struct1, 0);
            xcase TOK_INT_X:
                it = TokenStoreGetInt(tpi, column, (void*)struct1, 0) - TokenStoreGetInt(tpi, column, (void*)struct2, 0);
            xcase TOK_STRING_X:
                it = stricmp(TokenStoreGetString(tpi, column, (void*)struct1, 0), TokenStoreGetString(tpi, column, (void*)struct2, 0));
            xdefault:
                assertmsg(0, "unsupported key type");
        }
        if(it)
            return it > 0 ? 1 : -1;
    }
    return 0;
}

intptr_t persist_getKey(PersistInfo *info, void *structptr)
{
    ParseTable *tpi = info->settings.tpi;
    switch(info->keytype)
    {
        xcase KEY_VOID:
            return 0;
        xcase KEY_INT:
            switch(TOK_GET_TYPE(tpi[info->key_idxs[0]].type))
            {
                xcase TOK_U8_X:        return TokenStoreGetU8(tpi, info->key_idxs[0], structptr, 0);
                xcase TOK_INT_X:    return TokenStoreGetInt(tpi, info->key_idxs[0], structptr, 0);
                xdefault:            assertmsg(0, "unsupported key type");
            }
        xcase KEY_STRING:
            return (intptr_t)TokenStoreGetString(tpi, info->key_idxs[0], structptr, 0);
        xcase KEY_MULTI:
            return (intptr_t)structptr;
        xdefault:
            assertmsg(0, "invalid key type");
            return 0;
    }
    return 0;
}

intptr_t persist_dupKey(PersistInfo *info, void *structptr)
{
    ParseTable *tpi = info->settings.tpi;
    intptr_t key = 0;
    switch(info->keytype)
    {
        xcase KEY_VOID:
            key = 0;
        xcase KEY_INT:
            switch(TOK_GET_TYPE(tpi[info->key_idxs[0]].type))
            {
                xcase TOK_U8_X:        key = TokenStoreGetU8(tpi, info->key_idxs[0], structptr, 0);
                xcase TOK_INT_X:    key = TokenStoreGetInt(tpi, info->key_idxs[0], structptr, 0);
                xdefault:            assertmsg(0, "unsupported key type");
            }
        xcase KEY_STRING:
        {
            char *s = TokenStoreGetString(tpi, info->key_idxs[0], structptr, 0);
            key = (intptr_t)(s ? strdup(s) : NULL);
        }
        xcase KEY_MULTI:
        {
            void *structkey = calloc(1, info->key_size);
            if(info->key_flat)
            {
                memcpy(structkey, structptr, info->key_size);
            }
            else
            {
                int i;
                for(i = 0; i < eaiSize(&info->key_idxs); i++)
                {
                    int column = info->key_idxs[i];
                    switch(TOK_GET_TYPE(tpi[column].type))
                    {
                        xcase TOK_U8_X:
                            TokenStoreSetU8(tpi, column, structkey, 0, TokenStoreGetU8(tpi, column, structptr, 0));
                        xcase TOK_INT_X:
                            TokenStoreSetInt(tpi, column, structkey, 0, TokenStoreGetInt(tpi, column, structptr, 0));
                        xcase TOK_STRING_X:
                            TokenStoreSetString(tpi, column, structkey, 0, TokenStoreGetString(tpi, column, structptr, 0), 0, 0);
                        xdefault:
                            assertmsg(0, "unsupported key type");
                    }
                }
            }
            key = (intptr_t)structkey;
        }
        xdefault:
            assertmsg(0, "invalid key type");
    }
    return key;
}

void persist_freeKey(PersistInfo *info, intptr_t key)
{
    switch(info->keytype)
    {
        xcase KEY_VOID:
        xcase KEY_INT:
        xcase KEY_STRING:
            if(key)
                free((void*)key);
        xcase KEY_MULTI:
            if(key)
            {
                if(!info->key_flat)
                    StructClear(info->settings.tpi, (void*)key);
                free((void*)key);
            }
        xdefault:
            assertmsg(0, "invalid key type");
    }
}

static PersistInfo* s_getInfo(const char *type)
{
    PersistInfo *info;
    assertmsg(stashFindPointer(s_infos, type, &info), "searching an unregistered persist type");
    return info;
}

static void s_setKey(PersistInfo *info, void *structptr, int autoinc, va_list key)
{
    ParseTable *tpi = info->settings.tpi;
    int i;
    for(i = 0; i < eaiSize(&info->key_idxs); i++)
    {
        int column = info->key_idxs[i];
        switch(TOK_GET_TYPE(tpi[column].type))
        {
            xcase TOK_U8_X:        TokenStoreSetU8(tpi, column, structptr, 0, (autoinc && column == info->key_autoinc ? info->key_last+1 : va_arg(key, U8)));
            xcase TOK_INT_X:    TokenStoreSetInt(tpi, column, structptr, 0, (autoinc && column == info->key_autoinc ? info->key_last+1 : va_arg(key, int)));
            xcase TOK_STRING_X:    TokenStoreSetString(tpi, column, structptr, 0, va_arg(key, char*), NULL, NULL);
            xdefault:            assertmsg(0, "unsupported key type");
        }
    }
}

static void s_addRow(PersistInfo *info, void *structptr, int idx, int call_backend)
{
    ParseTable *tpi = info->settings.tpi;
    RowInfo row = {0};
    DirtyType dirty = DIRTY_CLEAN;
    row.idx = idx < 0 ? eaPush(&info->structptrs, structptr) : idx;
    if(call_backend)
    {
        dirty = s_adders[info->config->type] ? s_adders[info->config->type](info, structptr) : DIRTY_ROW;
        if(dirty == DIRTY_ROW && s_dirtiers[info->config->type])
            dirty = s_dirtiers[info->config->type](info, structptr);
        row.dirty = (dirty == DIRTY_ROW);
    }
    assert(stashAddressAddInt(info->rows, structptr, row.i, false));
    switch(info->keytype)
    {
        xcase KEY_VOID:
            assert(row.idx == 0); // only allow one object
        xcase KEY_INT:
            switch(TOK_GET_TYPE(tpi[info->key_idxs[0]].type))
            {
                xcase TOK_U8_X:        assert(stashIntAddPointer(info->lookup, TokenStoreGetU8(tpi, info->key_idxs[0], structptr, 0), structptr, false));
                xcase TOK_INT_X:    assert(stashIntAddPointer(info->lookup, TokenStoreGetInt(tpi, info->key_idxs[0], structptr, 0), structptr, false));
                xdefault:            assert(0);
            }
        xcase KEY_STRING:
            assert(stashAddPointer(info->lookup, TokenStoreGetString(tpi, info->key_idxs[0], structptr, 0), structptr, false));
        xcase KEY_MULTI:
            assert(stashAddPointer(info->lookup, structptr, structptr, false));
        xdefault:
            assertmsg(0, "invalid key type");
    }
    if(info->key_autoinc >= 0)
    {
        int val = 0;
        switch(TOK_GET_TYPE(tpi[info->key_autoinc].type))
        {
            xcase TOK_U8_X:        val = TokenStoreGetU8(tpi, info->key_autoinc, structptr, 0);
            xcase TOK_INT_X:    val = TokenStoreGetInt(tpi, info->key_autoinc, structptr, 0);
            xdefault:            assertmsg(0, "unsupported key type");
        }
        if(val > info->key_last)
            info->key_last = val;
    }
    if(call_backend)
    {
        assertmsg(s_isInfoLoaded(info), "modifying a database that isn't loaded");
        if(dirty != DIRTY_CLEAN)
            info->state = PERSISTINFO_DIRTY;
    }
}

void* s_createAndAdd(PersistInfo *info, int autoinc, va_list key)
{
    void *structptr = StructAllocRaw(info->settings.size);
    StructInit(structptr, info->settings.size, info->settings.tpi);
    s_setKey(info, structptr, autoinc, key);
    s_addRow(info, structptr, -1, 1);
    return structptr;
}

void persist_mergeReplace(PersistInfo *info, void *structptr)
{
    ParseTable *tpi = info->settings.tpi;
    StashElement elem = NULL;
    void *old = NULL;

    // FIXME: I'd prefer to grab the StashElement and update the key directly,
    //        but I don't feel comfortable exposing stashtables' keys
    switch(info->keytype)
    {
        xcase KEY_VOID:
            if(eaSizeUnsafe(&info->structptrs) > 0)
                old = info->structptrs[0];
        xcase KEY_INT:
        {
            int key = 0;
            switch(TOK_GET_TYPE(tpi[info->key_idxs[0]].type))
            {
                xcase TOK_U8_X:        key = TokenStoreGetU8(tpi, info->key_idxs[0], structptr, 0);
                xcase TOK_INT_X:    key = TokenStoreGetInt(tpi, info->key_idxs[0], structptr, 0);
                xdefault:            assert(0);
            }
            if(key > 0 && stashIntRemovePointer(info->lookup, key, &old))
                assert(stashIntAddPointer(info->lookup, key, structptr, false));
        }
        xcase KEY_STRING:
        {
            char *key = TokenStoreGetString(tpi, info->key_idxs[0], structptr, 0);
            if(stashRemovePointer(info->lookup, key, &old))
                assert(stashAddPointer(info->lookup, key, structptr, false));
        }
        xcase KEY_MULTI:
            if(stashRemovePointer(info->lookup, structptr, &old))
                assert(stashAddPointer(info->lookup, structptr, structptr, false));
        xdefault:
            assertmsg(0, "invalid key type");
    }

    if(old) // replacing, correct the reverse lookup and destroy the old object
    {
        RowInfo row = {0};
        assert(stashAddressRemoveInt(info->rows, old, &row.i));
        assert(!row.dirty); // we're merging, nothing should be dirty
        assert(info->structptrs[row.idx] == old); // internal inconsistency?
        info->structptrs[row.idx] = structptr;
        assert(stashAddressAddInt(info->rows, structptr, row.i, false));

        StructDestroy(tpi, old);
    }
    else // brand new
    {
        s_addRow(info, structptr, -1, 0);
    }
}

static void* s_findStruct(PersistInfo *info, va_list key)
{
    void *structptr = NULL;
    switch(info->keytype)
    {
        xcase KEY_VOID:
            structptr = eaGet(&info->structptrs, 0);
        xcase KEY_INT:
            stashIntFindPointerHack(info->lookup, va_arg(key, int), &structptr);
        xcase KEY_STRING:
            stashFindPointer(info->lookup, va_arg(key, char*), &structptr);
        xcase KEY_MULTI:
        {
            void *structkey = _alloca(info->key_size);
            memset(structkey, 0, info->key_size);
            s_setKey(info, structkey, 0, key);
            stashFindPointer(info->lookup, structkey, &structptr);
            if(!info->key_flat)
                StructClear(info->settings.tpi, structkey);
        }
        xdefault:
            assertmsg(0, "invalid key type");
    };
    return structptr; // set to NULL if not found;
}

void s_finishRemove(PersistInfo *info, RowInfo row, void *structptr)
{
    unsigned last_idx = eaSizeUnsafe(&info->structptrs)-1;
    assert(last_idx >= 0);
    if(info->settings.params & PERSIST_ORDERED)
    {
        // this may be slow... stepping through every element is going to be faster if row.idx is low
        for(; row.idx < last_idx; row.idx++)
        {
            StashElement elem;
            RowInfo replacerow;
            void *replaceptr = info->structptrs[row.idx+1];
            info->structptrs[row.idx] = replaceptr;
            assert(stashAddressFindElement(info->rows, replaceptr, &elem)); // internal inconsistency?
            replacerow.i = stashElementGetInt(elem);
            assert(replacerow.idx == row.idx+1); // internal inconsistency?
            replacerow.idx = row.idx;
            stashElementSetInt(elem, replacerow.i);
        }
    }
    else
    {
        // remove from structptrs (replace with the last item)
        if(row.idx < last_idx)
        {
            StashElement elem;
            RowInfo replacerow;
            void *replaceptr = info->structptrs[last_idx];
            info->structptrs[row.idx] = replaceptr;
            assert(stashAddressFindElement(info->rows, replaceptr, &elem)); // internal inconsistency?
            replacerow.i = stashElementGetInt(elem);
            assert(replacerow.idx == last_idx); // internal inconsistency?
            replacerow.idx = row.idx;
            stashElementSetInt(elem, replacerow.i);
        }
        // else 
    }
    eaPop(&info->structptrs);

    // destroy it
    StructDestroy(info->settings.tpi, structptr);
}

static void s_removeAndDestroy(PersistInfo *info, void *structptr)
{
    RowInfo row;
    void *structwas = NULL;
    DirtyType dirty;
    ParseTable *tpi = info->settings.tpi;

    // locate and remove from rows
    assertmsg(stashAddressRemoveInt(info->rows, structptr, &row.i), "searching for an untracked persisted struct");
    assertmsg(!row.readonly, "modified a locked element");
    assert(structptr == info->structptrs[row.idx]); // internal inconsistency?

    // remove from lookup
    switch(info->keytype)
    {
        xcase KEY_VOID:
            structwas = eaGet(&info->structptrs, 0);
            assert(eaSizeUnsafe(&info->structptrs));
        xcase KEY_INT:
            switch(TOK_GET_TYPE(tpi[info->key_idxs[0]].type))
            {
                xcase TOK_U8_X:        assert(stashIntRemovePointer(info->lookup, TokenStoreGetU8(tpi, info->key_idxs[0], structptr, 0), &structwas));
                xcase TOK_INT_X:    assert(stashIntRemovePointer(info->lookup, TokenStoreGetInt(tpi, info->key_idxs[0], structptr, 0), &structwas));
                xdefault:            assertmsg(0, "unsupported key type");
            }
        xcase KEY_STRING:
            assert(stashRemovePointer(info->lookup, TokenStoreGetString(tpi, info->key_idxs[0], structptr, 0), &structwas));
        xcase KEY_MULTI:
            assert(stashRemovePointer(info->lookup, structptr, &structwas));
        xdefault:
            assertmsg(0, "invalid key type");
    }
    assert(structptr == structwas); // internal inconsistency?

    dirty = s_removers[info->config->type] ? s_removers[info->config->type](info, structptr) : DIRTY_DBONLY;
    assert(dirty != DIRTY_ROW); // there's no row to dirty

    // finish the job
    s_finishRemove(info, row, structptr);
    if(dirty != DIRTY_CLEAN)
        info->state = PERSISTINFO_DIRTY;
}


void persist_mergeDelete(PersistInfo *info, intptr_t key)
{
    void* structptr = NULL;
    RowInfo row;

    switch(info->keytype)
    {
        xcase KEY_VOID:
            structptr = eaGet(&info->structptrs, 0);
        xcase KEY_INT:
            stashIntRemovePointer(info->lookup, key, &structptr);
        xcase KEY_STRING:
            stashRemovePointer(info->lookup, (char*)key, &structptr);
        xcase KEY_MULTI:
            stashRemovePointer(info->lookup, (void*)key, &structptr);
        xdefault:
            assertmsg(0, "invalid key type");
    }
    if(!structptr) // this is okay e.g. for DIFFBIG, which doesn't know whether a row is in the db at the time of deletion
        return;

    // remove from rows
    stashAddressRemoveInt(info->rows, structptr, &row.i);

    // finish the job
    s_finishRemove(info, row, structptr);
}

void persist_mergeLastKey(PersistInfo *info, int last_key)
{
    if(devassert(last_key >= info->key_last))
        info->key_last = last_key;
}

void persist_mergeRemoveJournal(PersistInfo *info, char *fname)
{
    // FIXME: check return values
    if(info->config->flags & PERSIST_KEEPALLJOURNALS)
    {
        char backup[MAX_PATH];
        struct tm time = {0};
        timerMakeTimeStructFromSecondsSince2000(timerSecondsSince2000(), &time); // Note: localized time
        sprintf(backup,"%s.%.4i%.2i%.2i%.2i%.2i%.2i", fname, time.tm_year+1900, time.tm_mon+1, time.tm_mday, time.tm_hour, time.tm_min, time.tm_sec);
        fileMove(fname, backup);
    }
    else if(info->config->flags & PERSIST_KEEPLASTJOURNAL)
    {
        fileRenameToBak(fname);
    }
    else
    {
        (void)unlink(fname);
    }
}

static char* s_getName(PersistInfo *info)
{
    return info->config->structname ? info->config->structname : info->typeName;
}

static void s_sprintPanicName(char *buf, size_t buf_size, PersistInfo *info)
{
    if(info->config->fname)
        sprintf_s(buf, buf_size, "%s.panic", info->config->fname);
    else
        sprintf_s(buf, buf_size, "%s.txt.panic", s_getName(info));
}

// Interface //////////////////////////////////////////////////////////////////

int s_checkConfig(PersistConfig *config)
{
    int ret = 1;

    if(!INRANGE0(config->type, PERSIST_COUNT))
    {
        ErrorFilenamef(config->configfile, "Unknown persist type %d", config->type);
        ret = 0;
    }

    if(    config->type == PERSIST_FLAT ||
        config->type == PERSIST_DIFF_BIG ||
        config->type == PERSIST_DIFF_SMALL )
    {
        if(!config->fname || !config->fname[0])
        {
            ErrorFilenamef(config->configfile, "Persist types 'Flat', 'DiffBig', and 'DiffSmall' require a 'File' setting");
            ret = 0;
        }
    }

    if(config->type == PERSIST_DIFF_SMALL)
    {

    }

    if(config->flags & PERSIST_CRCJOURNAL)
        ErrorFilenamef(config->configfile, "Warning 'ChecksumJournal' is not implemented");

//     if( config->type == PERSIST_SQL &&
//         (!config->server || !config->server[0] ||
//         (!config->user || !config->user[0] ||
//         (!config->pass || !config->pass[0] ||
//         (!config->db || !config->db[0] ||
//         (!config->table || !config->table[0] ) )
//     {
//         ErrorFilenamef(config->configfile, "Persist type 'Sql' requires 'Server', 'User', 'Pass', 'Db', and 'Table'", config->type);
//         ret = 0;
//     }

    return ret;
}

int persist_RegisterType(const char *type, PersistSettings *settings, PersistConfig *config)
{
    PersistInfo *info;
    assert(type && settings && settings->tpi && settings->size && config);
    assert(    (settings->linetpi && settings->linesize && settings->journalplayer) ||
            (!settings->linetpi && !settings->linesize && !settings->journalplayer) );

    if(!s_checkConfig(config))
        return 0;

    if(!s_infos)
    {
        s_infos = stashTableCreateWithStringKeys(0, StashDefault);
        InitializeCriticalSection(&s_mergecs);
    }

    info = calloc(1, sizeof(*info));
    info->typeName = strdup(type);
    info->settings = *settings; // struct copy
    info->config = StructCompress(parse_PersistConfig, config, sizeof(*config), NULL, NULL, NULL); // make a copy of the config
    info->merge_recovery = false;

    assertmsg(stashAddPointer(s_infos, type, info, 0), "double-registering persist info");
    ap_push(&s_infolist, info);
    return 1;
}

int persist_RegisterQuick(const char *type, size_t size, ParseTable *tpi, PersistConfig *config, PersistParams params)
{
    PersistSettings settings = {0};
    settings.size = size;
    settings.tpi = tpi;
    settings.params = params;
    return persist_RegisterType(type, &settings, config);
}

int persist_RegisterJournaled(    const char *type, size_t size, ParseTable *tpi,
                                size_t linesize, ParseTable *linetpi, JournalPlayer journalplayer,
                                PersistConfig *config, PersistParams params)
{
    PersistSettings settings = {0};
    settings.size = size;
    settings.tpi = tpi;
    settings.params = params;
    settings.linesize = linesize;
    settings.linetpi = linetpi;
    settings.journalplayer = journalplayer;
    return persist_RegisterType(type, &settings, config);
}

static int s_loadInfo(PersistInfo *info, int merging, PersistConfig *config_was, void ***structptrs)
{
    int i;
    char panicname[MAX_PATH];
    PersistConfig *config_willbe = NULL;
    ParseTable *tpi = info->settings.tpi;

    assertmsg(!!merging + !!config_was + !!structptrs <= 1, "more than one persist load type");
    assertmsg(info->state == PERSISTINFO_NOTLOADED, "double-loading persist type");
    info->state = PERSISTINFO_LOADING;

    if(config_was && !s_checkConfig(config_was))
    {
        // error message given above
        info->state = PERSISTINFO_ERROR;
        return 0;
    }

    if(config_was)
    {
        config_willbe = info->config;
        info->config = config_was;
    }

    s_sprintPanicName(SAFESTR(panicname), info);
    if(fileExists(panicname))
    {
        Errorf("ERROR: Attempting to load %s database after a catastrophic error.", info->typeName);
        info->state = PERSISTINFO_ERROR;
        return 0;
    }

    info->key_autoinc = -1;
    info->rows = stashTableCreateAddress(0);
    FORALL_PARSETABLE(tpi, i)
    {
        if(tpi[i].type & TOK_PRIMARY_KEY)
        {
            assert(!(tpi[i].type & TOK_EARRAY) && !(tpi[i].type & TOK_FIXED_ARRAY));
            eaiPush(&info->key_idxs, i);
            if(tpi[i].type & TOK_AUTO_INCREMENT)
            {
                assertmsg(TOK_GET_TYPE(tpi[i].type) == TOK_U8_X || TOK_GET_TYPE(tpi[i].type) == TOK_INT_X, "invalid autoincrement type");
                assertmsg(info->key_autoinc < 0, "multiple autoincrement fields");
                info->key_autoinc = i;
            }
        }
    }

    if(!eaiSize(&info->key_idxs))
    {
        info->keytype = KEY_VOID;
    }
    else if(eaiSize(&info->key_idxs) == 1)
    {
        switch(TOK_GET_TYPE(tpi[info->key_idxs[0]].type))
        {
            case TOK_U8_X:
            case TOK_INT_X:
                info->keytype = KEY_INT;
                info->lookup = stashTableCreateInt(0);
                break;

            case TOK_STRING_X:
                info->keytype = KEY_STRING;
                info->lookup = stashTableCreateWithStringKeys(0, StashDefault);
                break;

            default:
                assertmsg(0, "unsupported key type");
        }
    }
    else
    {
        Bits bits = Bits_temp();

        info->key_flat = 1;
        for(i = 0; i < eaiSize(&info->key_idxs); i++)
        {
            ParseTable *column = &tpi[info->key_idxs[i]];

            if(column->type & TOK_INDIRECT)
            {
                info->key_flat = 0;
                Bits_setrun(&bits, column->storeoffset, sizeof(void*));
                continue;
            }

            switch(TOK_GET_TYPE(column->type))
            {
                xcase TOK_U8_X:        Bits_set(&bits, column->storeoffset);
                xcase TOK_INT_X:    Bits_setrun(&bits, column->storeoffset, 4);
                xcase TOK_STRING_X:
                    // this is tricky, because everything after the terminator won't be maintained
                    // Bits_setrun(&bits, column->storeoffset, column->param);
                    info->key_flat = 0;
                xdefault:            assertmsg(0, "unsupported key type");
            }
        }

        info->keytype = KEY_MULTI;
        info->key_size = Bits_highestset(&bits)+1;
        info->key_flat = info->key_flat && info->key_size == Bits_countset(&bits);
        info->lookup = info->key_flat // a potential optimization if key_size <= sizeof(int) would be to use int keys
                        ? stashTableCreateFixedSize(0, info->key_size)
                        : stashTableCreateExternalFunctions(0, StashDefault, persist_hashStruct, persist_compStruct, info);

        if(!info->key_flat && isDevelopmentMode())
            printf("Warning: key structure is poor.\n"); // front load the struct with flat members and no gaps to make things a little faster

        Bits_destroy(&bits);
    }

    if(s_initializers[info->config->type])
    {
        if(!s_initializers[info->config->type](info))
        {
            // this condition may be assertable
            printf("ERROR: Could not initialize %s", info->typeName);
            info->state = PERSISTINFO_ERROR;
            return 0;
        }
    }

    if(structptrs)
    {
        info->structptrs = *structptrs;
        *structptrs = NULL;
        for(i = 0; i < eaSizeUnsafe(&info->structptrs); i++)
            s_addRow(info, info->structptrs[i], i, 1);
    }
    else
    {
        if(s_loaders[info->config->type])
        {
            if(!s_loaders[info->config->type](info))
            {
                printf("ERROR: Could not load %s database\n", info->typeName);
                info->state = PERSISTINFO_ERROR;
                return 0;
            }
        }

        if(config_was)
        {
            // we are converting from an old database.
            // now that we've loaded the structs, tell the system it actually has a different backend
            info->config = config_willbe;

            info->backend = NULL;    // this is probably causing a leak, but we
                                    // shouldn't be sticking around after a conversion
                                    // anyway.  fix it if you need to :P
        }

        for(i = 0; i < eaSizeUnsafe(&info->structptrs); i++)
            s_addRow(info, info->structptrs[i], i, !!config_was);

        if(s_mergers[info->config->type])
        {
            // GGFIXME: this is broken if we're converting to a new backend and there's an existing diff
            if(!s_mergers[info->config->type](info, !!merging))
            {
                printf("ERROR: Could not merge journal for %s database\n", info->typeName);
                info->state = PERSISTINFO_ERROR;
                return 0;
            }
        }
    }

    if(config_was || structptrs)
    {
        LOG(LOG_TEXT_JOURNAL, LOG_LEVEL_VERBOSE, LOG_CONSOLE_ALWAYS, "Flushing new %s db", s_getName(info));
        info->state = PERSISTINFO_DIRTY;
        s_startFlush(info, 0);
        s_waitForFlush(info, 1);
        if(info->merging == PERSISTINFO_MERGEPENDING)
        {
            info->merging = PERSISTINFO_MERGING;
            s_mergeChangesSync(&info, 1);
            info->merging = PERSISTINFO_NOTMERGING;
        }
        LOG(LOG_TEXT_JOURNAL, LOG_LEVEL_VERBOSE, LOG_CONSOLE_ALWAYS, "Finished new %s db", s_getName(info));
    }

    info->state = PERSISTINFO_CLEAN;
    if(info->config->flush_time)
        info->timer = timerAlloc();
    if(!(info->config->flags & PERSIST_DONTSYNCMERGES))
    {
        if(!s_syncedmergetimer)
        {
            s_syncedmergetimer = timerAlloc();
            s_syncedmergetime = info->config->flush_time;
        }
        if(info->config->flush_time < s_syncedmergetime)
            s_syncedmergetime = info->config->flush_time;
    }

    return 1;
}

int persist_Load(const char *type, int merging, PersistConfig *config_was, void ***structptrs)
{
    PersistInfo *info = s_getInfo(type);
    if(!s_loadInfo(info, merging, config_was, structptrs))
        return 0;
    if(info->merging == PERSISTINFO_MERGEPENDING)
        s_mergeChanges(&info, 1);
    return 1;
}

int persist_Count(const char *type)
{
    PersistInfo *info = s_getInfo(type);
    assertmsg(s_isInfoLoaded(info), "reading a database that isn't loaded");
    return eaSizeUnsafe(&info->structptrs);
}

int persist_CountSafe(const char *type)
{
    PersistInfo *info = s_getInfo(type);
    if(!s_isInfoLoaded(info))
    {
        LOG(LOG_TEXT_JOURNAL, LOG_LEVEL_VERBOSE, LOG_CONSOLE_ALWAYS, "counting a database that isn't loaded - treating as empty");
        return 0;
    }
    return eaSizeUnsafe(&info->structptrs);
}

void** persist_GetAll(const char *type)
{
    PersistInfo *info = s_getInfo(type);
    assertmsg(s_isInfoLoaded(info), "modifying a database that isn't loaded");
    return info->structptrs;
}

void* persist_Find(const char *type, ...)
{
    PersistInfo *info = s_getInfo(type);
    void *structptr;
    assertmsg(info->state == PERSISTINFO_LOADING || s_isInfoLoaded(info), "modifying a database that isn't loaded");
    VA_START(key, type);
    structptr = s_findStruct(info, key);
    VA_END();
    return structptr;
}

void* persist_FindOrAdd(const char *type, ...)
{
    PersistInfo *info = s_getInfo(type);
    void *structptr;
    assertmsg(s_isInfoLoaded(info), "modifying a database that isn't loaded");
    VA_START(key, type);
    structptr = s_findStruct(info, key);
    if(!structptr)
        structptr = s_createAndAdd(info, 0, key);
    VA_END();
    return structptr;
}

void* persist_Add(const char *type, ...)
{
    PersistInfo *info = s_getInfo(type);
    void *structptr;
    assertmsg(s_isInfoLoaded(info), "modifying a database that isn't loaded");
    VA_START(key, type);
    structptr = s_findStruct(info, key) ? NULL : s_createAndAdd(info, 0, key);
    VA_END();
    return structptr;
}

void* persist_New(const char *type, ...)
{
    PersistInfo *info = s_getInfo(type);
    void *structptr;
    assertmsg(s_isInfoLoaded(info), "modifying a database that isn't loaded");
    assertmsg(info->key_autoinc >= 0, "calling new on a non-autoincrementing type");
    VA_START(key, type);
    structptr = s_createAndAdd(info, 1, key);
    VA_END();
    return structptr;
}

int persist_FindAndDestroy(const char *type, ...)
{
    PersistInfo *info = s_getInfo(type);
    void *structptr;
    assertmsg(s_isInfoLoaded(info), "modifying a database that isn't loaded");
    VA_START(key, type);
    structptr = s_findStruct(info, key);
    VA_END();
    if(!structptr)
        return 0;
    s_removeAndDestroy(info, structptr);
    return 1;
}

void persist_RemoveAndDestroy(const char *type, void *structptr)
{
    PersistInfo *info = s_getInfo(type);
    assertmsg(s_isInfoLoaded(info), "modifying a database that isn't loaded");
    s_removeAndDestroy(info, structptr);
}

void persist_RemoveAndDestroyList(const char *type, void **structptrs, int count)
{
    int i;
    PersistInfo *info = s_getInfo(type);
    assertmsg(s_isInfoLoaded(info), "modifying a database that isn't loaded");
    for(i = 0; i < count; i++) // FIXME: we should pass the list around, to let backends be more efficient
        s_removeAndDestroy(info, structptrs[i]);
}

void persist_Dirty(const char *type, void *structptr, void *lineptr)
{
    StashElement elem;
    RowInfo row;
    DirtyType dirty;
    PersistInfo *info = s_getInfo(type);
    assertmsg(s_isInfoLoaded(info), "modifying a database that isn't loaded");
    assertmsg(stashAddressFindElement(info->rows, structptr, &elem), "searching for an untracked persisted struct");
    // GGFIXME: check that primary key fields haven't changed?
    row.i = stashElementGetInt(elem);
    assertmsg(!row.readonly, "modified a locked element");
    assert(structptr == info->structptrs[row.idx]); // internal inconsistency?

    if(lineptr && s_journaler[info->config->type])
        dirty = s_journaler[info->config->type](info, lineptr);
    else if(s_dirtiers[info->config->type])
        dirty = s_dirtiers[info->config->type](info, structptr);
    else
        dirty = DIRTY_ROW;

    row.dirty = (dirty == DIRTY_ROW);
    stashElementSetInt(elem, row.i);
    if(dirty != DIRTY_CLEAN)
        info->state = PERSISTINFO_DIRTY;
}

int persist_ResetAutoIncrement(const char *type)
{
    int i;
    intptr_t key_last = 0;
    ParseTable *tpi;
    PersistInfo *info = s_getInfo(type);
    assertmsg(s_isInfoLoaded(info), "modifying a database that isn't loaded");
    assert(info->key_autoinc >= 0);

    key_last = 0;
    tpi = info->settings.tpi;
    for(i = 0; i < eaSizeUnsafe(&info->structptrs); i++)
    {
        intptr_t key = 0;
        switch(TOK_GET_TYPE(tpi[info->key_autoinc].type))
        {
            xcase TOK_U8_X:        key = TokenStoreGetU8(tpi, info->key_autoinc, info->structptrs[i], 0);
            xcase TOK_INT_X:    key = TokenStoreGetInt(tpi, info->key_autoinc, info->structptrs[i], 0);
            xdefault:            assertmsg(0, "unsupported key type");
        }
        if(key > key_last)
            key_last = key;
    }

    assert(key_last <= info->key_last);
    if(key_last < info->key_last)
    {
        info->key_last = key_last;
        info->state = PERSISTINFO_DIRTY;
    }
    return (int)info->key_last; // FIXME: this is a bad cast
}

void persist_FlushStart(const char *type, int quitting)
{
    PersistInfo *info = s_getInfo(type);
    assertmsg(s_isInfoLoaded(info), "modifying a database that isn't loaded"); // this may be too aggressive

    s_startFlush(info, quitting);

    if(info->merging == PERSISTINFO_MERGEPENDING)
        s_mergeChanges(&info, 1);
}

int persist_IsFlushing(const char *type)
{
    PersistInfo *info = s_getInfo(type);
    return s_isInfoFlushing(info) || s_isInfoMerging(info);
}

void persist_FlushSync(const char *type)
{
    PersistInfo *info = s_getInfo(type);
    s_waitForFlush(info, 1);
}

int persist_LoadAll(void)
{
    int i;
    int success = 1;
    PersistInfo **pending_merges = (PersistInfo**)ap_temp();

    for(i = 0; i < ap_size(&s_infolist); i++)
    {
        PersistInfo *info = s_infolist[i];
        if(info->state == PERSISTINFO_NOTLOADED)
        {
            loadstart_printf("Loading %s...", s_getName(info)); // TODO: put in a control for noise
            if(!s_loadInfo(info, 0, NULL, NULL))
                success = 0;
            else if(info->merging == PERSISTINFO_MERGEPENDING)
                ap_push(&pending_merges, info);
            loadend_printf("");
        }
    }
    s_mergeChanges(pending_merges, ap_size(&pending_merges));

    ap_destroy(&pending_merges, NULL);
    return success;
}

void persist_FlushAll(PersistFlushAll type)
{
    int i;
    int syncronized = -1;
    int skipped = 0;
    int expired = 0;
    PersistInfo **pending_flushes;
    PersistInfo **pending_flushes_buf = (PersistInfo**)ap_temp();
    PersistInfo **pending_merges = (PersistInfo**)ap_temp();

    assert(INRANGE0(type, PERSIST_FLUSHALLTYPES));

    if(type == PERSIST_TICK)
    {
        for(i = 0; i < ap_size(&s_infolist); i++)
        {
            PersistInfo *info = s_infolist[i];

            // give backends a chance to do any maintenance
            if(s_tickers[info->config->type])
                s_tickers[info->config->type](info);

            if(!s_flushers[info->config->type]) // implicit flushing (though syncing may be required)
                continue;

            // in a given tick only either a) flush/merge one unsynced db b) flush/merge all synced dbs
            if(info->merge_ticked)
                continue;

            if(syncronized < 0) // first unticked db, attempt to flush it
            {
                syncronized = !(info->config->flags & PERSIST_DONTSYNCMERGES);
                if(syncronized)
                    expired = !s_syncedmergetimer || timerElapsed(s_syncedmergetimer) > s_syncedmergetime;
                else
                    expired = !info->timer || timerElapsed(info->timer) > info->config->flush_time;
            }
            else if(!syncronized ||                                        // we're attempting to flush an unsynchronized db, don't flush any more
                    (info->config->flags & PERSIST_DONTSYNCMERGES) )    // we're attempting to flush all synchronized dbs, but this one is unsynchronized
            {
                skipped++;
                continue;
            }

            info->merge_ticked = 1;
            if(expired)
                ap_push(&pending_flushes_buf, info);
        }
        pending_flushes = pending_flushes_buf;
    }
    else
    {
        pending_flushes = s_infolist;
    }

    for(i = 0; i < ap_size(&pending_flushes); i++)
    {
        PersistInfo *info = pending_flushes[i];
        int flushing;

        if(!s_flushers[info->config->type]) // implicit flushing (though syncing may be required)
        {
            assert(info->state != PERSISTINFO_DIRTY);    // i don't think it makes sense for row operations to mark the db dirty if it has no need to start flushing
            continue;                                    // if it does make sense, the db probably needs to be marked clean now...
        }

        if(info->state != PERSISTINFO_DIRTY)
            continue;

        flushing = s_syncers[info->config->type] && s_syncers[info->config->type](info);
        if(!flushing || !info->timer || timerElapsed(info->timer) < info->config->fail_time)
        {
            loadstart_printf("Flushing %s...", s_getName(info)); // TODO: put in a control for noise
            s_startFlush(info, type == PERSIST_SHUTDOWN);
            if(info->merging == PERSISTINFO_MERGEPENDING)
                ap_push(&pending_merges, info);
            loadend_printf("");
        }
        else if(timerElapsed(info->timer) > info->config->fail_time)
        {
            LOG(LOG_TEXT_JOURNAL, LOG_LEVEL_VERBOSE, LOG_CONSOLE_ALWAYS, "Merge (%s) has been running for over %ds. Doing a synchronous dump to flat text.", s_getName(info), MAX(info->config->fail_time, info->config->flush_time));
            if(persist_Panic())
                FatalErrorf("Merge process stalled. Attempted to save the current db with .panic extensions.\nPlease see the console for more info!");
            else
                FatalErrorf("DOUBLE PANIC!\nMerge process stalled. Attempted to save the current db with .panic extensions.\nPlease see the console for more info!");
        }
    }

    s_mergeChanges(pending_merges, ap_size(&pending_merges));

    if(!skipped)
    {
        for(i = 0; i < ap_size(&s_infolist); i++)
        {
            PersistInfo *info = s_infolist[i];
            info->merge_ticked = 0;
        }
    }

    if(syncronized > 0 && expired)
    {
        assert(type == PERSIST_TICK);
        if(s_syncedmergetimer)
            timerStart(s_syncedmergetimer);
    }

    if( type == PERSIST_MERGEALLSYNC ||
        type == PERSIST_SHUTDOWN )
    {
        for(i = 0; i < ap_size(&s_infolist); i++)
        {
            PersistInfo *info = s_infolist[i];
            if(s_isInfoFlushing(info) || s_isInfoMerging(info))
            {
                loadstart_printf("Waiting for %s...", s_getName(info)); // TODO: put in a control for noise
                s_waitForFlush(info, 1);
                loadend_printf("");
            }
        }
    }

    ap_destroy(&pending_merges, NULL);
    ap_destroy(&pending_flushes_buf, NULL);
}

int persist_Panic(void)
{
    int i;
    int success = 1;

    LOG(LOG_TEXT_JOURNAL, LOG_LEVEL_VERBOSE, LOG_CONSOLE_ALWAYS, "persist_Panic(), attempting to dump all databases to disk");

    // do all the direct-to-disk dumping first
    for(i = 0; i < ap_size(&s_infolist); i++)
    {
        PersistInfo *info = s_infolist[i];
        if(s_isInfoLoaded(info) && !s_panickers[info->config->type])
        {
            ParseTable tpi_panic[] =
            {
                { s_getName(info), TOK_INDIRECT|TOK_EARRAY|TOK_STRUCT_X, 0, info->settings.size, info->settings.tpi },
                { "", 0 }
            };

            char fname[MAX_PATH];
            s_sprintPanicName(SAFESTR(fname), info);
            if(!ParserWriteTextFile(fname, tpi_panic, &info->structptrs, 0, 0))
            {
                LOG(LOG_TEXT_JOURNAL, LOG_LEVEL_VERBOSE, LOG_CONSOLE_ALWAYS, "ERROR: Unable to dump %s database to %s during panic! Continuing with any other databases.", s_getName(info), fname);
                success = 0;
            }
        }
    }

    // then do all the special handlers
    for(i = 0; i < ap_size(&s_infolist); i++)
    {
        PersistInfo *info = s_infolist[i];
        if(s_isInfoLoaded(info) && s_panickers[info->config->type])
        {
            if(!s_panickers[info->config->type](info))
            {
                LOG(LOG_TEXT_JOURNAL, LOG_LEVEL_VERBOSE, LOG_CONSOLE_ALWAYS, "ERROR: Special (%d) panic handler for %s database failed! Continuing with any other databases.", info->config->type, s_getName(info));
                success = 0;
            }
        }
    }

    return success;
}

// HTTP Output /////////////////////////////////////////////////////////////////
#include <utilitieslib/UtilsNew/Str.h>
#include <utilitieslib/components/EString.h> // required for ParserWriteText
#include <utilitieslib/utils/textparserUtils.h>
#define sendline(fmt, ...)    Str_catf(str, fmt"\n", __VA_ARGS__)
#define rowclass            ((row++)&1)

static void s_persist_http_info(char **str, PersistInfo *info, char **keys, char **values, int count, char *fragment);
static void s_persist_http_structptr(char **str, PersistInfo *info, void *structptr, char **keys, char **values, int count, char *fragment);

static char* strStartsWithAdvance(char *haystack, char *needle)
{
    size_t needle_len;
    if(!needle || !haystack)
        return haystack;
    needle_len = strlen(needle);
    if(strnicmp(haystack, needle, needle_len)==0)
        return haystack + needle_len;
    return NULL;
}

static int strCountChars(char *haystack, char needle)
{
    int count = 0;
    if(haystack)
        while(haystack = strchr(haystack, needle))
            haystack++, count++;
    return count;
}

static const char* getValue(const char *key, const char **keys, const char **vals, int count)
{
    int i;
    for(i = 0; i < count; i++)
        if(stricmp(key, keys[i]) == 0)
            return vals[i];
    return NULL;
}

static void* s_http_findStruct(PersistInfo *info, char *path)
{
    void *structptr = NULL;
    switch(info->keytype)
    {
        xcase KEY_VOID:
            structptr = eaGet(&info->structptrs, 0);
        xcase KEY_INT:
            stashIntFindPointerHack(info->lookup, atoi(path), &structptr);
        xcase KEY_STRING:
        {
            char *s;
            if(s = strchr(path, '/'))
                *s = '\0';
            stashFindPointer(info->lookup, path, &structptr);
        }
        xcase KEY_MULTI:
        {
            ParseTable *tpi = info->settings.tpi;
            int i;
            void *structkey = _alloca(info->key_size);
            memset(structkey, 0, info->key_size);
            for(i = 0; i < eaiSize(&info->key_idxs); i++)
            {
                int column = info->key_idxs[i];
                char *key = path;
                if(path = strchr(path, '/'))
                    *(path++) = '\0';
                switch(TOK_GET_TYPE(tpi[column].type))
                {
                    xcase TOK_U8_X:        TokenStoreSetU8(tpi, column, structkey, 0, atoi(key));
                    xcase TOK_INT_X:    TokenStoreSetInt(tpi, column, structkey, 0, atoi(key));
                    xcase TOK_STRING_X:    TokenStoreSetString(tpi, column, structkey, 0, key, NULL, NULL);
                    xdefault:            assertmsg(0, "unsupported key type");
                }
            }
            stashFindPointer(info->lookup, structkey, &structptr);
            if(!info->key_flat) // fixed keys are always flat (no allocs)
                StructClear(tpi, structkey);
        }
        xdefault:
            assertmsg(0, "invalid key type");
    };
    return structptr; // set to NULL if not found;
}

void persist_http_summary(char **str, char **keys, char **values, int count)
{
    int row = 0;
    sendline("<table>");
    sendline("<tr class=rowt><th colspan=7>Persist Layer Summary");
    if(ap_size(&s_infolist))
    {
        int i, synced = 0, verbose = 0;

        for(i = 0; i < count; i++)
            if(streq(keys[i], "verbose"))
                verbose = 1;

        sendline("<tr class=rowh><th>Name<th>Type<th>Count<th>Load<th>Merge<th>Flush In<th><a href='?%s'>Memory</a>", verbose ? "" : "verbose");
        for(i = 0; i < ap_size(&s_infolist); i++)
            if(!(s_infolist[i]->config->flags & PERSIST_DONTSYNCMERGES))
                synced++;
        for(; synced >= 0; synced = synced ? 0 : -1) // i must be bored
        {
            int shown_synced = 0, shown_verbose = 0;
            for(i = 0; i < ap_size(&s_infolist); i++)
            {
                PersistInfo *info = s_infolist[i];
                if(!synced == !!(s_infolist[i]->config->flags & PERSIST_DONTSYNCMERGES))
                {
                    Str_catf(str, "<tr class=row%d><td><a href='/persist/%s/'>%s</a><td>%s<td>%i<td>%s<td>%s",
                                    rowclass,
                                    info->typeName,
                                    s_getName(info),
                                    StaticDefineIntRevLookup(PersistTypeEnum, info->config->type),
                                    eaSizeUnsafe(&info->structptrs),
                                    StaticDefineIntRevLookup(PersistLoadStateEnum, info->state),
                                    StaticDefineIntRevLookup(PersistMergeStateEnum, info->merging)
                            );
                    if(!synced)
                    {
                        if(info->timer)
                            Str_catf(str, "<td>%ds", info->config->flush_time - (int)timerElapsed(info->timer));
                        else
                            Str_catf(str, "<td>N/A");
                    }
                    else if(!shown_synced)
                    {
                        if(s_syncedmergetimer)
                            Str_catf(str, "<td rowspan=%d>%ds", synced, s_syncedmergetime - (int)timerElapsed(s_syncedmergetimer));
                        else
                            Str_catf(str, "<td rowspan=%d>N/A", synced);
                        shown_synced = 1;
                    }
                    if(verbose)
                    {
                        int j;
                        size_t usage = 0;
                        for(j = 0; j < eaSize(&info->structptrs); j++)
                            usage += StructGetMemoryUsage(info->settings.tpi, info->structptrs[j], info->settings.size);
                        Str_catf(str, "<td>%d bytes", usage);
                    }
                    else if(!shown_verbose)
                    {
                        Str_catf(str, "<td rowspan=%d><a href='?verbose'>Show<br>(Slow)</a>", ap_size(&s_infolist));
                        shown_verbose = 1;
                    }
                    Str_catf(str, "\n");
                }
            }
        }
    }
    else
    {
        sendline("<tr class=row%d><td class=nodata>No persisted objects.", rowclass);
    }
    sendline("</table>");
}

void persist_http_page(char **str, char *path, char **keys, char **values, int count, char *fragment)
{
    char *type;
    PersistInfo *info;

    type = strStartsWithAdvance(path, "persist/");
    if(!type || !type[0])
    {
        persist_http_summary(str, keys, values, count);
        return;
    }

    if(path = strchr(type, '/'))
        *(path++) = '\0';
    info = s_getInfo(type);

    if(info && path && path[0])
        s_persist_http_structptr(str, info, s_http_findStruct(info, path), keys, values, count, fragment);
    else
        s_persist_http_info(str, info, keys, values, count, fragment);
}

static void s_persist_http_info(char **str, PersistInfo *info, char **keys, char **values, int count, char *fragment)
{
    int row = 0;
    int cols = 3 + (info ? eaiSize(&info->key_idxs) : 0);
    char *key = Str_temp();
    char *key_path = Str_temp();

    if(info)
    {
        int i, verbose = 0;
        for(i = 0; i < count; i++)
            if(streq(keys[i], "verbose"))
                verbose = 1;

        sendline("<table>");
        sendline("<tr class=rowt><th colspan=2>%s", s_getName(info));
        if(verbose)
        {
            char *dump = estrTemp();
            size_t usage = 0;
            sendline("<tr class=rowh><th colspan=2><a href='?'>Hide detail</a>");
            sendline("<tr class=row%d><td>Rows<td>%d", rowclass, eaSize(&info->structptrs));
            for(i = 0; i < eaSize(&info->structptrs); i++)
                usage += StructGetMemoryUsage(info->settings.tpi, info->structptrs[i], info->settings.size);
            sendline("<tr class=row%d><td>Memory<td>%d bytes", rowclass, usage);
            estrClear(&dump);
            ParserWriteText(&dump, parse_PersistConfig, info->config, 0, 0);
            sendline("<tr class=row%d><td>Config<td><span style='white-space:pre'>%s</span>", rowclass, dump);
            estrClear(&dump);
            ParseTableWriteText(&dump, info->settings.tpi);
            sendline("<tr class=row%d><td>Layout<td><span style='white-space:pre'>%s</span>", rowclass, dump);
            estrDestroy(&dump);
        }
        else
        {
            sendline("<tr class=rowh><th colspan=2><a href='?verbose'>Full detail (slow)</a>");
        }
        sendline("</table>");
    }

    sendline("<table>");
    sendline("<tr class=rowt><th colspan=%d>%s", cols, s_getName(info));
    if(info)
    {
        ParseTable *tpi = info->settings.tpi;
        int i, j;

        Str_catf(str, "<tr class=rowh><th>row");
        for(i = 0; i < eaiSize(&info->key_idxs); i++)
            Str_catf(str, "<th>%s", tpi[info->key_idxs[i]].name);
        Str_catf(str, "<th>locked<th>dirty\n");

        if(eaSizeUnsafe(&info->structptrs))
        {
            for(j = 0; j < eaSizeUnsafe(&info->structptrs); j++)
            {
                RowInfo rowinfo;
                void *structptr = info->structptrs[j];
                stashAddressFindInt(info->rows, structptr, &rowinfo.i);
                Str_clear(&key_path);
                if(eaiSize(&info->key_idxs))
                    Str_catf(str, "<tr class=row%d><td>%d", rowclass, rowinfo.idx);
                else
                    Str_catf(str, "<tr class=row%d><td><a href='0/'>%d</a>", rowclass, rowinfo.idx);
                for(i = 0; i < eaiSize(&info->key_idxs); i++)
                {
                    int column = info->key_idxs[i];
                    switch(TOK_GET_TYPE(tpi[column].type))
                    {
                        xcase TOK_U8_X:
                            Str_printf(&key, "%d", TokenStoreGetU8(tpi, column, structptr, 0));
                        xcase TOK_INT_X:
                            Str_printf(&key, "%d", TokenStoreGetInt(tpi, column, structptr, 0));
                        xcase TOK_STRING_X:
                            Str_printf(&key, "%s", TokenStoreGetString(tpi, column, structptr, 0));
                        xdefault:
                            assertmsg(0, "unsupported key type");
                    }
                    Str_caturi(&key_path, key);
                    Str_catf(&key_path, "/");
                    Str_catf(str, "<td><a href='%s'>%s</a>", key_path, key);
                }
                Str_catf(str, "<td>%s<td>%s\n",
                                rowinfo.readonly ? "&nbsp;" : "&#x2713;",
                                rowinfo.dirty ? "&#x2713;" : "&nbsp;" );
            }
        }
        else
        {
            sendline("<tr class=row%d><td class=nodata colspan=%d>This database is empty.",
                        rowclass,
                        cols );
        }
    }
    else
    {
        // FIXME: send 404
        sendline("<tr class=row%d><td class=nodata colspan=%d>Unknown database.",
                    rowclass,
                    cols );
    }
    sendline("</table>");

    Str_destroy(&key);
    Str_destroy(&key_path);
}

static void s_persist_http_structptr(char **str, PersistInfo *info, void *structptr, char **keys, char **values, int count, char *fragment)
{
    int row = 0;
    int cols = 3;

    sendline("<form><table>");
    sendline("<tr class=rowt><th colspan=%d>%s", cols, s_getName(info));
    if(structptr)
    {
        if(getValue("t", keys, values, count))
        {
            ParseTable tpi_http[] =
            {
                { s_getName(info), TOK_STRUCT_X, 0, info->settings.size, info->settings.tpi },
                { "", 0 }
            };
            char *text = estrTemp();
            U32 tablecrc = ParseTableCRC(tpi_http, NULL);
            U32 structcrc = StructCRC(tpi_http, structptr);

            sendline("<tr class=rowh colspan=%d><th>Text dump", cols);

            if(getValue("m", keys, values, count))
            {
                const char *s;
                U32 newcrc;
                int update = 1;

                newcrc = (s = getValue("p", keys, values, count)) ? atoi(s) : 0;
                if(newcrc != tablecrc)
                {
                    sendline("<tr class=rowe colspan=%d><th>Error: ParseTables changed during modification", cols);
                    update = 0;
                }

                newcrc = (s = getValue("s", keys, values, count)) ? atoi(s) : 0;
                if(newcrc != structcrc)
                {
                    sendline("<tr class=rowe colspan=%d><th>Error: Object changed during modification", cols);
                    update = 0;
                }

                if(!(s = getValue("o", keys, values, count)))
                {
                    sendline("<tr class=rowe colspan=%d><th>Error: Could not read new object", cols);
                    update = 0;
                }

//                 if(update)
//                 {
//                     // This is actually really dangerous... dangling pointers, changed keys, etc. (but cool)
//                     StructClear(tpi_http, structptr);
//                     ParserReadText(s, tpi_http, structptr);
//                     tablecrc = ParseTableCRC(tpi_http, NULL);
//                     structcrc = StructCRC(tpi_http, structptr);
//                     sendline("<tr class=rowi colspan=%d><th>Updated object", cols);
//                 }
            }

            ParserWriteText(&text, tpi_http, structptr, 0, 0);

            Str_catf(str, "<tr class=row%d colspan=%d><td><textarea rows=%d cols=%d name='o'>", rowclass, cols, strCountChars(text, '\n')+1, 100);
            Str_cathtml(str, text);
            Str_catf(str, "</textarea><br>");
            Str_catf(str, "<input type=hidden name='t' value='1'>");
            Str_catf(str, "<input type=hidden name='s' value='%d'>", structcrc);
            Str_catf(str, "<input type=hidden name='p' value='%d'>", tablecrc);
//            Str_catf(str, "<input type=submit name='m' value='Modify'>\n");

            estrDestroy(&text);
        }
// Save this for the new serialization system
//         else if(count && stricmp(keys[0],"e")==0)
//         {
//             RowInfo rowinfo;
//             int i;
// 
//             stashAddressFindInt(info->rows, structptr, &rowinfo.i);
// 
//             sendline("<tr class=rowh><td>Field<td>Value<td><input type=submit name='e' value='Modify'>");
//             FORALL_PARSETABLE(info->tpi, i)
//             {
//                 if(    TOK_GET_TYPE(info->tpi[i].type) == TOK_START ||
//                     TOK_GET_TYPE(info->tpi[i].type) == TOK_END ||
//                     TOK_GET_TYPE(info->tpi[i].type) == TOK_IGNORE )
//                 {
//                     continue;
//                 }
// 
//                 Str_cat(str, "<tr class=row%d><td>%s<td>XX<td>", rowclass, info->tpi[i].name);
//                 if(info->tpi[i].type & TOK_PRIMARY_KEY)
//                 {
//                     Str_cat(str, "<i>Key</i>");
//                     if(info->tpi[i].type & TOK_AUTO_INCREMENT)
//                         Str_cat(str, " <i>Auto-Increment</i>");
//                 }
//                 else if(rowinfo.readonly)
//                 {
//                     Str_cat(str, "<i>Readonly</i>");
//                 }
//                 else if(info->tpi[i].type & TOK_EARRAY || info->tpi[i].type & TOK_FIXED_ARRAY)
//                 {
//                     Str_cat(str, "<i>Array</i>");
//                 }
//                 else
//                 {
//                     switch(TOK_GET_TYPE(info->tpi[i].type))
//                     {
//                         xcase TOK_U8_X:        Str_cat(str, "<input type=text name=%d value=%d>", i, TokenStoreGetU8(info->tpi, i, structptr, 0));
//                         xcase TOK_INT_X:    Str_cat(str, "<input type=text name=%d value=%d>", i, TokenStoreGetInt(info->tpi, i, structptr, 0));
//                         xcase TOK_STRING_X:    Str_cat(str, "<input type=text name=%d value=%s>", i, TokenStoreGetString(info->tpi, i, structptr, 0));
//                         xdefault:            Str_cat(str, "<i>Unsupported</i>");
//                     }
//                 }
//                 Str_cat(str, "\n");
//             }
//         }
        else
        {
            sendline("<tr class=rowh><th colspan=%d>Commands", cols);
            sendline("<tr class=row%d><td colspan=%d><a href='?t'>Text Dump</a>", rowclass, cols);
//            sendline("<tr class=row%d><td colspan=%d><a href='?e'>Modify</a>", rowclass, cols);
        }
    }
    else
    {
        sendline("<tr class=row%d><td colspan=%d class=nodata>Object does not exist.", rowclass, cols);
    }
    sendline("</table></form>");
}

#undef sendline
#undef rowclass
////////////////////////////////////////////////////////////////////////////////

#include "AutoGen/persist_h_ast.c"
#include "AutoGen/persist_internal_h_ast.c"
