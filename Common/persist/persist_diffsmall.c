#include "utilitieslib/stdtypes.h"
#include "persist_flat.h"
#include "persist_diffsmall.h"
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
#include "textcrcjournal.h"

// GGFIXME: the asserts and fatal errors here should panic instead
// GGFIXME: there are a lot of return values (for file operations and whatnot) that aren't checked

typedef struct BackendInfo
{
    char directory[MAX_PATH];
    char fn_journal[MAX_PATH];
    char fn_merge[MAX_PATH];
    ParseTable parse_diffline[5];
    TextCRCJournal journal;
    intptr_t key_last;
} BackendInfo;

typedef struct DiffLine
{
    void *structptr;
    void *lineptr;
    intptr_t key_last;
    union {
        U8 b;
        int i;
        void *s;
    } killed;
} DiffLine;
MP_DEFINE(DiffLine);

int initializer_diffsmall(PersistInfo *info)
{
    char *s;
    BackendInfo *backend = info->backend = calloc(1, sizeof(*info->backend));

    // set up the journal
    strcpy(backend->directory, info->config->fname);
    forwardSlashes(backend->directory);
    s = strrchr(backend->directory, '/');
    if(s)
        *s = '\0';
    else
        backend->directory[0] = '\0';
    sprintf(backend->fn_journal, "%s.journal", info->config->fname);
    sprintf(backend->fn_merge, "%s.merging", info->config->fname);

    backend->journal.directory = backend->directory;
    backend->journal.fn_journal = backend->fn_journal;
    backend->journal.fn_merge = backend->fn_merge;
    backend->journal.max_journal_size = info->config->max_journal_size;
    backend->journal.roll_journal_retries = info->config->roll_journal_tries - 1;
    backend->journal.assert_on_line_error = info->settings.assert_on_line_error ? true : false;

    // Build parse tables for the diff, something conceptually like this:
    // ParseTable parse_diffline[] =
    // {
    //     { "Replace",    TOK_OPTIONALSTRUCT(DiffLine, structptr, info->tpi)            },
    //     { "Custom",        TOK_OPTIONALSTRUCT(DiffLine, lineptr, info->config->linetpi    },
    //     { "Delete",        TOK_OPTIONALSTRUCT(DiffLine, killed.s, info->tpi)            },
    //    { "LastKey",    TOK_INT(DiffLine, key_last, 0)                                },
    //     { "", 0 }
    // };

    backend->parse_diffline[0].name = "Replace";
    backend->parse_diffline[0].type = TOK_INDIRECT|TOK_STRUCT_X;
    backend->parse_diffline[0].storeoffset = offsetof(DiffLine, structptr);
    backend->parse_diffline[0].param = info->settings.size;
    backend->parse_diffline[0].subtable = info->settings.tpi;

    backend->parse_diffline[1].name = "Custom";
    backend->parse_diffline[1].type = TOK_INDIRECT|TOK_STRUCT_X;
    backend->parse_diffline[1].storeoffset = offsetof(DiffLine, lineptr);
    backend->parse_diffline[1].param = info->settings.linesize;
    backend->parse_diffline[1].subtable = info->settings.linetpi;

    switch(info->keytype)
    {
        xcase KEY_VOID:
            backend->parse_diffline[2].name = "Delete";
            backend->parse_diffline[2].type = TOK_BOOLFLAG_X;
            backend->parse_diffline[2].storeoffset = offsetof(DiffLine, killed.b);
        xcase KEY_INT:
            backend->parse_diffline[2].name = "Delete";
            backend->parse_diffline[2].type = TOK_INT_X;
            backend->parse_diffline[2].storeoffset = offsetof(DiffLine, killed.i);
        xcase KEY_STRING:
            backend->parse_diffline[2].name = "Delete";
            backend->parse_diffline[2].type = TOK_INDIRECT|TOK_STRING_X;
            backend->parse_diffline[2].storeoffset = offsetof(DiffLine, killed.s);
        xcase KEY_MULTI:
            backend->parse_diffline[2].name = "Delete";
            backend->parse_diffline[2].type = TOK_INDIRECT|TOK_STRUCT_X;
            backend->parse_diffline[2].storeoffset = offsetof(DiffLine, killed.s);
            backend->parse_diffline[2].param = info->settings.size; // TODO: this could be info->key_size, if we had a custom parse table
            backend->parse_diffline[2].subtable = info->settings.tpi; // TODO: this could be a custom, more efficient parse table
        xdefault:
            assertmsg(0, "invalid key type");
    }

    backend->parse_diffline[3].name = "LastKey";
    backend->parse_diffline[3].type = sizeof(intptr_t) == 8 ? TOK_INT64_X : TOK_INT_X;
    backend->parse_diffline[3].storeoffset = offsetof(DiffLine, key_last);

    backend->parse_diffline[4].name = "";

    return 1;
}

static void s_cleanupMerge(PersistInfo *info)
{
    TextCRCJournal_CleanupMerge(&info->backend->journal, (info->config->flags & PERSIST_KEEPALLJOURNALS) ? true : false);
}

int loader_diffsmall(PersistInfo *info)
{
    if(persist_loadTextDb(info, s_cleanupMerge))
    {
        info->backend->key_last = info->key_last;
        return 1;
    }
    return 0;
}

DirtyType dirtier_diffsmall(PersistInfo *info, void *structptr)
{
    DiffLine line = {0};
    line.structptr = structptr;
    TextCRCJournal_WriteLine(&info->backend->journal, &line, info->backend->parse_diffline);
    return DIRTY_DBONLY; // the row isn't dirty, since it's been journaled
}

DirtyType journaler_diffsmall(PersistInfo *info, void *lineptr)
{
    DiffLine line = {0};
    line.lineptr = lineptr;
    TextCRCJournal_WriteLine(&info->backend->journal, &line, info->backend->parse_diffline);
    return DIRTY_DBONLY; // the row isn't dirty, since it's been journaled
}

DirtyType remover_diffsmall(PersistInfo *info, void *structptr)
{
    DiffLine line = {0};
    switch(info->keytype)
    {
        xcase KEY_VOID:
            line.killed.b = 1;
        xcase KEY_INT:
            line.killed.i = (int)persist_getKey(info, structptr);
        xcase KEY_STRING:
            line.killed.s = (char*)persist_getKey(info, structptr);
        xcase KEY_MULTI:
            line.killed.s = (void*)persist_getKey(info, structptr);
        xdefault:
            assertmsg(0, "invalid key type");
    }
    TextCRCJournal_WriteLine(&info->backend->journal, &line, info->backend->parse_diffline);
    return DIRTY_DBONLY;
}

static bool s_processLine(void* lineData, void* infoData)
{
    DiffLine * line = (DiffLine *)lineData;
    PersistInfo * info = (PersistInfo *)infoData;
    assert(!!line->structptr + !!line->lineptr + !!line->killed.s + !!line->key_last == 1); // it's invalid for more than one to be defined // FIXME: this shouldn't be an assert

    if(line->structptr)
    {
        persist_mergeReplace(info, line->structptr);
        line->structptr = NULL; // took ownership of structptr, don't destroy it
    }

    if(line->lineptr)
    {
        info->settings.journalplayer(line->lineptr);
        // let textcrcjournal destroy lineptr
    }

    if(line->killed.s)
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

    if(line->key_last)
        info->key_last = line->key_last-1;

    return false;
}

int merger_diffsmall(PersistInfo *info, int write)
{
    if(!write)
        TextCRCJournal_RollJournal(&info->backend->journal);
    TextCRCJournal_WriteMergeFiles(&info->backend->journal);
    TextCRCJournal_DoMerge(&info->backend->journal, s_processLine, sizeof(DiffLine), info->backend->parse_diffline, info);

    if(write)
    {
        if(!persist_flushTextDb(info, s_cleanupMerge))
            FatalErrorf("Failed to write merged %s database!", info->typeName);
    }
    else if(info->backend->journal.merge_pending)
    {
        persist_startMerge(info);
    }

    return 1;
}

int ticker_diffsmall(PersistInfo *info)
{
    TextCRCJournal_Tick(&info->backend->journal); // roll by size
    return 1;
}

int flusher_diffsmall(PersistInfo *info)
{
    if(info->key_last < info->backend->key_last) // this detects if key_last has been reset
    {
        DiffLine line = {0};
        line.key_last = info->key_last+1; // 0 means 'not a LastKey update'
        TextCRCJournal_WriteLine(&info->backend->journal, &line, info->backend->parse_diffline);
        info->backend->key_last = info->key_last;
    }
    TextCRCJournal_RollJournal(&info->backend->journal); // roll by time or request
    if(info->backend->journal.merge_pending)
        persist_startMerge(info);
    return 1;
}
