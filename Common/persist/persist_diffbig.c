#include "utilitieslib/stdtypes.h"
#include "persist_flat.h"
#include "persist_diffbig.h"
#include "utilitieslib/utils/textparser.h"
#include "utilitieslib/components/earray.h"
#include "utilitieslib/components/StashTable.h"
#include "utilitieslib/assert/assert.h"
#include "utilitieslib/components/MemoryPool.h"
#include "utilitieslib/utils/wininclude.h"
#include "utilitieslib/utils/utils.h"
#include "utilitieslib/utils/file.h"
#include "utilitieslib/components/EString.h"
#include "utilitieslib/utils/error.h"
#include "utilitieslib/utils/log.h"

// GGFIXME: the asserts and fatal errors here should panic instead
// GGFIXME: there are a lot of return values (for file operations and whatnot) that aren't checked

typedef struct BackendInfo
{
    CRITICAL_SECTION cs;
    HANDLE thread;
    char *estr;
    StashTable killed;
    ParseTable parse_diff[3];
    ParseTable parse_diffline[4];
    char fn_diff[MAX_PATH];
} BackendInfo;

typedef struct DiffLine
{
    void *structptr;
    union {
        U8 b;
        int i;
        void *s;
    } killed;
} DiffLine;
MP_DEFINE(DiffLine);

typedef struct DiffBig
{
    int last_key;
    DiffLine **diff;
} DiffBig;

int initializer_diffbig(PersistInfo *info)
{
    BackendInfo *backend = info->backend = calloc(1, sizeof(*info->backend));

    sprintf(backend->fn_diff, "%s.diff", info->config->fname);

    // set up a stash for monitoring deletions, this should match info->lookup
    switch(info->keytype)
    {
        xcase KEY_VOID:
        xcase KEY_INT:
            backend->killed = stashTableCreateInt(0);
        xcase KEY_STRING:
            backend->killed = stashTableCreateWithStringKeys(0, StashDeepCopyKeys);
        xcase KEY_MULTI:
            backend->killed = info->key_flat
                            ? stashTableCreateFixedSize(0, info->key_size)
                            : stashTableCreateExternalFunctions(0, StashDefault, persist_hashStruct, persist_compStruct, info);
        xdefault:
            assertmsg(0, "invalid key type");
    }

    // Build parse tables for the diff, something conceptually like this:
    // ParseTable parse_diffline[] =
    // {
    //     { "Replace",    TOK_OPTIONALSTRUCT(DiffLine, structptr, info->tpi)    },
    //     { "Delete",        TOK_OPTIONALSTRUCT(DiffLine, killed.s, info->tpi)    },
    //     { "End",        TOK_END                                                },
    //     { "", 0 }
    // };
    // 
    // ParseTable s_parse_diff[] =
    // {
    //     { "LastKey",    TOK_INT(DiffBig, last_key, 0)                },
    //     { "Diff",        TOK_STRUCT(DiffBig, diff, parse_diffline)    },
    //     { "", 0 }
    // };

    backend->parse_diff[0].name = "LastKey";
    backend->parse_diff[0].type = TOK_INT_X;
    backend->parse_diff[0].storeoffset = offsetof(DiffBig, last_key);
    backend->parse_diff[1].name = "Diff";
    backend->parse_diff[1].type = TOK_INDIRECT|TOK_EARRAY|TOK_STRUCT_X;
    backend->parse_diff[1].storeoffset = offsetof(DiffBig, diff);
    backend->parse_diff[1].param = sizeof(DiffLine);
    backend->parse_diff[1].subtable = backend->parse_diffline;
    backend->parse_diff[2].name = "";

    backend->parse_diffline[0].name = "Replace";
    backend->parse_diffline[0].type = TOK_INDIRECT|TOK_STRUCT_X;
    backend->parse_diffline[0].storeoffset = offsetof(DiffLine, structptr);
    backend->parse_diffline[0].param = info->settings.size;
    backend->parse_diffline[0].subtable = info->settings.tpi;
    switch(info->keytype)
    {
        xcase KEY_VOID:
            backend->parse_diffline[1].name = "Delete";
            backend->parse_diffline[1].type = TOK_BOOLFLAG_X;
            backend->parse_diffline[1].storeoffset = offsetof(DiffLine, killed.b);
        xcase KEY_INT:
            backend->parse_diffline[1].name = "Delete";
            backend->parse_diffline[1].type = TOK_INT_X;
            backend->parse_diffline[1].storeoffset = offsetof(DiffLine, killed.i);
        xcase KEY_STRING:
            backend->parse_diffline[1].name = "Delete";
            backend->parse_diffline[1].type = TOK_INDIRECT|TOK_STRING_X;
            backend->parse_diffline[1].storeoffset = offsetof(DiffLine, killed.s);
        xcase KEY_MULTI:
            backend->parse_diffline[1].name = "Delete";
            backend->parse_diffline[1].type = TOK_INDIRECT|TOK_STRUCT_X;
            backend->parse_diffline[1].storeoffset = offsetof(DiffLine, killed.s);
            backend->parse_diffline[1].param = info->key_size;
            backend->parse_diffline[1].subtable = info->settings.tpi; // TODO: this could be a custom, more efficient parse table
        xdefault:
            assertmsg(0, "invalid key type");
    }
    backend->parse_diffline[2].name = "End";
    backend->parse_diffline[2].type = TOK_END;
    backend->parse_diffline[3].name = "";

    InitializeCriticalSection(&backend->cs);

    return 1;
}

static void s_cleanupMerge(PersistInfo *info)
{
    persist_mergeRemoveJournal(info, info->backend->fn_diff);
}

int loader_diffbig(PersistInfo *info)
{
    return persist_loadTextDb(info, s_cleanupMerge);
}

DirtyType adder_diffbig(PersistInfo *info, void *structptr)
{
    // we no longer need to record this struct's removal in the diff
    switch(info->keytype)
    {
        xcase KEY_VOID:
            info->backend->killed = U32_TO_PTR(0);
        xcase KEY_INT:
            stashIntRemovePointer(info->backend->killed, persist_getKey(info, structptr), NULL);
        xcase KEY_STRING:
            stashRemovePointer(info->backend->killed, (char*)persist_getKey(info, structptr), NULL);
        xcase KEY_MULTI:
        {
            void *key;
            if(stashRemovePointer(info->backend->killed, (void*)persist_getKey(info, structptr), &key))
                persist_freeKey(info, (intptr_t)key);
        }
        xdefault:
            assertmsg(0, "invalid key type");
    }

    return DIRTY_ROW;
}

DirtyType remover_diffbig(PersistInfo *info, void *structptr)
{
    // record the removal for diffing
    switch(info->keytype)
    {
        xcase KEY_VOID:
            info->backend->killed = U32_TO_PTR(1);
        xcase KEY_INT:
        {
            int key = persist_getKey(info, structptr);
            assert(stashIntAddPointer(info->backend->killed, key, NULL, false));
        }
        xcase KEY_STRING:
        {
            char *key = (char*)persist_getKey(info, structptr); // let the stashTable do its own deep copy into a string table
            assert(stashAddPointer(info->backend->killed, key, NULL, false));
        }
        xcase KEY_MULTI:
        {
            void *key = (void*)persist_dupKey(info, structptr);
            assert(stashAddPointer(info->backend->killed, key, key, false));
        }
        xdefault:
            assertmsg(0, "invalid key type");
    }

    return DIRTY_DBONLY;
}

int merger_diffbig(PersistInfo *info, int write)
{
    int i;
    DiffBig diffbig = {0};

    // merge any journals
    if(!fileExists(info->backend->fn_diff))
        return 1; // nothing to merge

    if(!ParserReadTextFile(info->backend->fn_diff, info->backend->parse_diff, &diffbig))
    {
        LOG(LOG_TEXT_JOURNAL, LOG_LEVEL_VERBOSE, LOG_CONSOLE_ALWAYS, "Failed to read journal %s", info->backend->fn_diff);
        return 0;
    }

    for(i = 0; i < eaSize(&diffbig.diff); i++)
    {
        DiffLine *line = diffbig.diff[i];
        assert(!line->structptr != !line->killed.s); // line->structptr ^^ (line->killed.s || line->killed.i || line->killed.b)
        if(line->structptr)
        {
            persist_mergeReplace(info, line->structptr);
            line->structptr = NULL; // don't destroy
        }
        else
        {
            switch(info->keytype)
            {
                xcase KEY_VOID:
                    persist_mergeDelete(info, 0);
                xcase KEY_INT:
                    persist_mergeDelete(info, line->killed.i);
                xcase KEY_STRING:
                    persist_mergeDelete(info, (intptr_t)line->killed.s);
                xcase KEY_MULTI:
                    persist_mergeDelete(info, (intptr_t)line->killed.s);
                xdefault:
                    assertmsg(0, "invalid key type");
            }
        }
    }

    persist_mergeLastKey(info, diffbig.last_key);

    if(write)
    {
        if(!persist_flushTextDb(info, s_cleanupMerge))
            FatalErrorf("Failed to write merged %s database!", info->typeName);
    }
    else
    {
        persist_startMerge(info);
    }

    StructClear(info->backend->parse_diff, &diffbig);
    return 1;
}

static unsigned __stdcall s_flusherthread(LPVOID lpParam)
{
    EXCEPTION_HANDLER_BEGIN

    PersistInfo *info = lpParam;
    FILE *fout;

    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_LOWEST);

    fout = fileOpen(info->backend->fn_diff, "wb");
    fwrite(info->backend->estr, estrLength(&info->backend->estr), sizeof(char), fout);
    fclose(fout);

    estrDestroy(&info->backend->estr);

    EnterCriticalSection(&info->backend->cs);
    CloseHandle(info->backend->thread);
    info->backend->thread = NULL;
    LeaveCriticalSection(&info->backend->cs);

    EXCEPTION_HANDLER_END
    return 0;
}

int flusher_diffbig(PersistInfo *info)
{
    int i;
    DiffBig diffbig = {0};
    StashTableIterator iter;
    StashElement elem;

    if(fileExists(info->backend->fn_diff) && info->merge_recovery == false)
    {
        // so forwhat ever reason, the last time the diff file was made, it was not completely processed. Try processing it again.
        LOG(LOG_TEXT_JOURNAL, LOG_LEVEL_VERBOSE, LOG_CONSOLE_ALWAYS, "flusher_diffbig: Previous big-diff %s journal %s already exists!", info->typeName, info->backend->fn_diff);

        if(info->backend->thread)
        {
            // shouldn't get here since this is checked before this function is called
            LOG(LOG_TEXT_JOURNAL, LOG_LEVEL_VERBOSE, LOG_CONSOLE_ALWAYS, "flusher_diffbig: Previous big-diff %s journal thread still writing!", info->typeName);
            return 1;
        }

        info->merge_recovery = true;
        persist_startMerge(info);
    } 
    else 
    {
        MemoryPool mpLines = createMemoryPool();
        initMemoryPool(mpLines, sizeof(DiffLine), 1024);
        mpSetMode(mpLines, ZeroMemoryBit);

        // build the list, clear dirtiness
        for(i = 0; i < eaSizeUnsafe(&info->structptrs); i++)
        {
            void *structptr = info->structptrs[i];
            RowInfo row;
            stashAddressFindElement(info->rows, structptr, &elem);
            row.i = stashElementGetInt(elem);
            if(row.dirty)
            {
                DiffLine *line = mpAlloc(mpLines);
                line->structptr = structptr;
                eaPush(&diffbig.diff, line);
                row.dirty = 0;
                stashElementSetInt(elem, row.i);
            }
        }
        if(info->keytype == KEY_VOID)
        {
            if(info->backend->killed)
            {
                DiffLine *line = mpAlloc(mpLines);
                line->killed.b = 1;
                eaPush(&diffbig.diff, line);
            }
        }
        else
        {
            stashGetIterator(info->backend->killed, &iter);
            while(stashGetNextElement(&iter, &elem))
            {
                DiffLine *line = mpAlloc(mpLines);
                switch(info->keytype)
                {
                    xcase KEY_INT:
                        line->killed.i = stashElementGetIntKey(elem);
                    xcase KEY_STRING:
                        line->killed.s = cpp_const_cast(char*)(stashElementGetStringKey(elem));
                    xcase KEY_MULTI:
                        line->killed.s = stashElementGetPointer(elem);
                    xdefault:
                        assertmsg(0, "invalid key type");
                }
                eaPush(&diffbig.diff, line);
            }
        }

        if(eaSize(&diffbig.diff))
        {
            diffbig.last_key = info->key_last;

            if (fileExists(info->backend->fn_diff) && info->merge_recovery == true)
            {
                FILE *fout;

                // we are unable to shutdown due to recovery - attempt to append outstanding diff to existing diff
                ParserWriteText(&info->backend->estr, info->backend->parse_diff, &diffbig, 0, 0);
                fout = fileOpen(info->backend->fn_diff, "ab");
                fwrite(info->backend->estr, estrLength(&info->backend->estr), sizeof(char), fout);
                fclose(fout);
                estrDestroy(&info->backend->estr);
            } else {
                // these fatal errors can cause data loss!
                if(info->backend->thread)
                    FatalErrorf("Previous big-diff %s journal thread still writing!", info->typeName);
                if(fileExists(info->backend->fn_diff))
                    FatalErrorf("Previous big-diff %s journal %s already exists!", info->typeName, info->backend->fn_diff);

                // serialize
                if(info->config->flags & PERSIST_NOTHREADS)
                {
                    ParserWriteTextFile(info->backend->fn_diff, info->backend->parse_diff, &diffbig, 0, 0);
                }
                else
                {
                    ParserWriteText(&info->backend->estr, info->backend->parse_diff, &diffbig, 0, 0);
                    EnterCriticalSection(&info->backend->cs);
                    assert(!info->backend->thread);
                    info->backend->thread = CreateThread(0,0,s_flusherthread,info,0,0);
                    assert(info->backend->thread);
                    LeaveCriticalSection(&info->backend->cs);
                }
                persist_startMerge(info);
            }
        }

        // cleanup
        switch(info->keytype)
        {
            xcase KEY_VOID: // nothing to destroy
            xcase KEY_INT: // nothing to destroy
            xcase KEY_STRING: // destroyed by stashTableClear
            xcase KEY_MULTI:
                stashGetIterator(info->backend->killed, &iter);
                while(stashGetNextElement(&iter, &elem))
                    persist_freeKey(info, (intptr_t)stashElementGetPointer(elem));
            xdefault:
                assertmsg(0, "invalid key type");
        }
        stashTableClear(info->backend->killed);
        eaDestroy(&diffbig.diff); // DiffLines will be destroyed by MP_DESTROY
        destroyMemoryPool(mpLines);
    }

    return 1;
}

int syncer_diffbig(PersistInfo *info)
{
    // relying on changes to info->backend->thread being atomic (this is unsafe on many platforms)
    return info->backend && info->backend->thread ? 1 : 0;
}
