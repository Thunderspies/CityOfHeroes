#include "groupjournal.h"
#include <utilitieslib/utils/mathutil.h>
#include "group/group.h"
#include <utilitieslib/utils/SuperAssert.h>
#include "group/groupgrid.h"
#include "group/grouptrack.h"
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/utils/utils.h>

static GroupDef* groupDefDupSimple(GroupDef *def)
{
    GroupDef *ret = calloc(1, sizeof(*ret));
    copyGroupDef(ret, def);
    return ret;
}
static void groupDefFreeSimple(void* defData)
{
    GroupDef * def = (GroupDef *)defData;
    PERFINFO_AUTO_START("groupDefFreeSimple", 1);
    uncopyGroupDef(def);
    free(def);
    PERFINFO_AUTO_STOP();
}

typedef struct JournalEntryRef {
    GroupDef *def;
    Mat4 mat;
} JournalEntryRef;

typedef struct JournalEntry {
    char *cmd;
    JournalEntryRef **refs;    // the state of group_info.refs when the journal was created
    GroupDef **defs;        // pre-modified versions of touched defs
} JournalEntry;

static JournalEntry ** journal;
static int journalCurrentEntry;    // index to where the next journal entry goes
static int journalAddingEntry;    // true after journalBeginEntry, false after journalEndEntry

static JournalEntry* journalEntryCreate(const char *cmd)
{
    int i;
    JournalEntry *je;

    PERFINFO_AUTO_START("journalEntryCreate", 1);
    je = calloc(1, sizeof(JournalEntry));
    je->cmd = strdup(cmd);
    for(i = 0; i < group_info.ref_max; i++)
    {
        JournalEntryRef *ref = NULL;
        if(group_info.refs[i] && group_info.refs[i]->def)
        {
            ref = malloc(sizeof(*ref));
            ref->def = group_info.refs[i]->def;
            copyMat4(group_info.refs[i]->mat, ref->mat);
        }
        eaPush(&je->refs, ref);
    }
    PERFINFO_AUTO_STOP();

    return je;
}

static void journalEntryDestroy(JournalEntry * je)
{
    free(je->cmd);
    eaDestroyEx(&je->refs, NULL);
    eaDestroyEx(&je->defs, groupDefFreeSimple);
    free(je);
}

void journalBeginEntry(const char *cmd)
{
    PERFINFO_AUTO_START("journalBeginEntry", 1);
    assert(!journalAddingEntry);
    journalAddingEntry=1;

    // since we're starting a new journal entry, we have to clear out anything
    // past our current entry in the journal
    while(eaSize(&journal) > journalCurrentEntry)
        journalEntryDestroy(eaRemoveFast(&journal, journalCurrentEntry));

    // now we can add in the new journal entry
    eaPush(&journal, journalEntryCreate(cmd));
    PERFINFO_AUTO_STOP();
}

void journalDef(GroupDef *def)
{
    PERFINFO_AUTO_START("journalDef", 1);
    assert(journalAddingEntry);
    eaPush(&journal[journalCurrentEntry]->defs, groupDefDupSimple(def));
    PERFINFO_AUTO_STOP();
}

void journalEndEntry(void)
{
    PERFINFO_AUTO_START("journalEndEntry", 1);
    assert(journalAddingEntry);
    journalAddingEntry=0;
    journalCurrentEntry++;
    PERFINFO_AUTO_STOP();
}


static void journalDirtyRef(int index)
{
    PERFINFO_AUTO_START("journalDirtyRef", 1);
    group_info.refs[index]->mod_time=group_info.ref_mod_time;
    if (group_info.refs[index]->def) {
        group_info.refs[index]->def->mod_time=group_info.ref_mod_time;
        group_info.refs[index]->def->file->mod_time=group_info.ref_mod_time;
        if (!group_info.loading)
            group_info.refs[index]->def->file->unsaved=1;    
        group_info.refs[index]->def->saved=0;
    }
    PERFINFO_AUTO_STOP();
}

static void journalDirtyDef(GroupDef *def)
{
    PERFINFO_AUTO_START("journalDirtyDef", 1);
    def->mod_time=group_info.ref_mod_time;
    def->file->mod_time=group_info.ref_mod_time;
    def->saved=0;
    PERFINFO_AUTO_STOP();
}

static int sameMat4(Mat4 a, Mat4 b)
{
    return     sameVec3(a[0], b[0]) &&
            sameVec3(a[1], b[1]) &&
            sameVec3(a[2], b[2]) &&
            sameVec3(a[3], b[3]);
}

static int journalDefChanged(GroupDef *def, JournalEntry *je)
{
    int i;
    for(i = eaSize(&je->defs)-1; i >= 0; --i)
        if(defContainCount(def, groupDefFind(je->defs[i]->name)))
            return 1;
    return 0;
}

static void journalRestoreEntry(void)
{
    JournalEntry * je=journal[journalCurrentEntry];
    int i;

    PERFINFO_AUTO_START("journalRestoreEntry", 1);

    // snapshot the current state of the refs
    journal[journalCurrentEntry] = journalEntryCreate(je->cmd);
    group_info.ref_mod_time++;

    // process defs in reverse, to preserve ordering
    PERFINFO_AUTO_START("journalReplaceDef", 1);
    for(i = eaSize(&je->defs)-1; i >=0; --i)
    {
        GroupDef temp;
        GroupDef *def_journaled = je->defs[i];
        GroupDef *def_world = groupDefFind(def_journaled->name);

        temp = *def_journaled;
        *def_journaled = *def_world;
        *def_world = temp;

        journalDirtyDef(def_world);
        eaPush(&journal[journalCurrentEntry]->defs, def_journaled); // now def_journaled contains what was in the world, in reverse
    }
    PERFINFO_AUTO_STOP();

    PERFINFO_AUTO_START("journalRestoreRef", 1);
    for(i = 0; i < eaSize(&je->refs) || i < group_info.ref_max; i++)
    {
        int dirty = 0;
        JournalEntryRef *ref_journaled = eaGet(&je->refs, i);
        DefTracker *ref_world = groupRefPtr(i);

        if(ref_journaled)
        {
            dirty =    ref_journaled->def != ref_world->def ||                // the ref is being added or changed
                    !sameMat4(ref_journaled->mat, ref_world->mat) ||    // the ref is being moved
                    journalDefChanged(ref_world->def, je);                // something within the ref changed
        }
        else
        {
            dirty = !!ref_world->def;                                    // the ref is being removed
        }

        if(dirty)
        {
            if(ref_world->def)
            {
                // out with the old
                groupRefGridDel(ref_world);
                trackerClose(ref_world);
            }

            if(ref_journaled)
            {
                // and in with the new
                ref_world->def = ref_journaled->def;
                copyMat4(ref_journaled->mat, ref_world->mat);
                groupRefActivate(ref_world);
            }
            else
            {
                ref_world->def = NULL;
            }

            journalDirtyRef(i);
        }
    }
    PERFINFO_AUTO_STOP();

    eaDestroy(&je->defs); // don't destroy contents, which are now in journal[journalCurrentEntry]->defs
    journalEntryDestroy(je);
    PERFINFO_AUTO_STOP();
}

void journalUndo(void)
{
    assert(!journalAddingEntry);
    if(journalUndosRemaining())
    {
        journalCurrentEntry--;
        journalRestoreEntry();
    }
}

void journalRedo(void)
{
    assert(!journalAddingEntry);
    if(journalRedosRemaining())
    {
        journalRestoreEntry();
        journalCurrentEntry++;
    }
}

int journalUndosRemaining(void)
{
    assert(!journalAddingEntry);
    return journalCurrentEntry;
}

int journalRedosRemaining(void)
{
    assert(!journalAddingEntry);
    return eaSize(&journal)-journalCurrentEntry;
}

static void journalEntryDestroy(JournalEntry * je);
static void journalEntryDestroyCallback(void* arg0)
{
    journalEntryDestroy((JournalEntry *)arg0);
}

void journalReset(void)
{
    PERFINFO_AUTO_START("journalReset", 1);
    assert(!journalAddingEntry);
    eaDestroyEx(&journal,journalEntryDestroyCallback);
    journalCurrentEntry=0;
    PERFINFO_AUTO_STOP();
}