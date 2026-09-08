/***************************************************************************
 *     Copyright (c) 2006-2007, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 *
 *    Set up your TextCRCJournal and pass it, along with a struct representing
 *        Db changes to WriteLine.  During merge, DoMerge will pass these
 *        structs in the same order to to 'process'.  'process' should return
 *        true if the journal line should be written with log_printf(0,...).
 *    Periodically call Tick to roll the journal over, or call RollJournal
 *        to do it manually.  This will set 'merge_pending'.  It's okay to
 *        have many journals waiting to be merged.
 *    While there is a merge pending, spawn a separate process for merging and
 *        clear 'merge_pending'.
 ***************************************************************************/
#ifndef TEXTCRCJOURNAL_H
#define TEXTCRCJOURNAL_H

#include <utilitieslib/stdtypes.h>
#include "utilitieslib/utils/file.h"

typedef struct ParseTable ParseTable;

typedef struct TextCRCJournal
{
    // set these
    char *directory;
    char *fn_journal;
    char *fn_merge;
    U32 max_journal_size;            // 0 = don't roll over by size
    U32 max_journal_time;            // 0 = don't roll over by time
    int roll_journal_retries;        // # of retries allowed for failed file ops. -1 = retry forever
    int sleep_ms_between_retries;    // time (in milliseconds) to wait before retrying file ops. Defaults to TextCRCJournal_DefaultRetrySleepMS.
    bool assert_on_line_error;

    // check these
    bool merge_pending; // true when journals are waiting to be merged 

    // internal use
    FILE *fp_journal;
    U32 log_timer;
} TextCRCJournal;

// Default TextCRCJournal::sleep_ms_between_retries if not set (i.e., 0).
#define TextCRCJournal_DefaultRetrySleepMS 500


// main process routines
void TextCRCJournal_WriteLine(TextCRCJournal *journal, void *data, ParseTable *parse_table);
void TextCRCJournal_Tick(TextCRCJournal *journal);                // closes the live journal and renames it when necessary
void TextCRCJournal_RollJournal(TextCRCJournal *journal);        // closes the live journal and renames it immediately
void TextCRCJournal_WriteMergeFiles(TextCRCJournal *journal);    // renames pending journals (segregates them for the merge process)

// merge process routines
// the process function should be of the form:
// bool processLine(StructType *data, void *userdata);
// 'data' is the the same as what was passed in for WriteLine
// 'userdata' is what was passed in to DoMerge
// the return value is true on error
void TextCRCJournal_DoMerge(TextCRCJournal *journal, bool (*process)(void*,void*),
                            size_t size, ParseTable *parse_table, void *userdata);    // merges the segregated journals
void TextCRCJournal_CleanupMerge(TextCRCJournal *journal, bool archive);            // deletes (or archives) merged journals

#endif //TEXTCRCJOURNAL_H
