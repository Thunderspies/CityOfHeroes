/***************************************************************************
 *     Copyright (c) 2006-2007, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 *
 * Module Description:
 * Keeps track of the journal that represents the non-merged state of the database.
 * 
 * the XactLog relies on three types of files on disk:
 * - active journal: This is where the journal is currently being written. xactions.log
 * - recorded parts: previously active parts of the active journal stored in numerically ascending
 *   'parts'. xactions.log.part001,xactions.log.part002, etc.
 * - merge parts: these are files that have no open transactions. they are renamed recorded 
 *   parts: xactions.log.part001 might become xactmerge.log.part001, for example. 
 *
 ***************************************************************************/

#include "utilitieslib/stdtypes.h"
#include "textcrcjournal.h"
#include "utilitieslib/assert/assert.h"
#include "utilitieslib/network/crypt.h"
#include "utilitieslib/utils/timing.h"
#include "utilitieslib/utils/textparser.h"
#include "utilitieslib/components/EString.h"
#include "utilitieslib/utils/file.h"
#include "utilitieslib/utils/utils.h"
#include "utilitieslib/utils/error.h"
#include "utilitieslib/utils/mathutil.h"
#include "utilitieslib/components/MemoryPool.h"
#include "utilitieslib/utils/wininclude.h" // Sleep
#include "utilitieslib/utils/log.h"

#define TEXTCRCJOURNAL_VERSION 20070417
typedef struct TextCRCJournalVersion
{
    bool crcd;
} TextCRCJournalVersion ;
//TextCRCJournalVersion g_curTextCRCJournalVersion = {0};
 
static TextCRCJournalVersion s_TextCRCJournalVersion_Init(U32 version)
{
    TextCRCJournalVersion res = {0};    
    switch ( version )
    {
    case 0:
        break; // do nothing
    case 20070105:
        FatalErrorf("Old logfiles found.  Please run the previous server once to merge files, then delete the resultant Xactions.log, which should contain a single header line.");
        break;
    case 20070417:
        res.crcd = true;
        break;
    default:
        Errorf("unknown logfile version %i\n",version);
        break;
    };
    return res;
}

static void s_FileRename(
                const char* oldPath, 
                const char* newPath,
                TextCRCJournal *journal, 
                const char* actionDescr        // E.g., "Rolling journal"
            )
{
    int retries = 0;
    int retrySleep = ( journal->sleep_ms_between_retries != 0 ) ? 
                        journal->sleep_ms_between_retries : 
                        TextCRCJournal_DefaultRetrySleepMS;
                        
    log_printf(0,"%s. Renaming %s to %s.", actionDescr, oldPath, newPath );
    while( rename( oldPath, newPath ) != 0 )
    {
        char buf[1024];
        strerror_s(SAFESTR(buf), errno);
        ++retries;
        printf("%s. Failed to rename %s to %s after %d %s - %s.\n",
                actionDescr, oldPath, newPath, retries, ((retries==1) ? "try":"tries"), buf);
        if (( journal->roll_journal_retries >= 0 ) && ( retries > journal->roll_journal_retries ))
        {
            FatalErrorf("%s. Failed to rename %s to %s after %d %s. Giving up!", actionDescr, 
                            oldPath, newPath, retries, ((retries==1) ? "try":"tries") );
        }
        Sleep(retrySleep);
    }
    if ( retries > 0 )
    {
        printf("%s. File rename succeeded after %d %s.\n", actionDescr, retries, ((retries==1) ? "try":"tries") );
    }
}

static TextCRCJournalVersion s_TextCRCJournal_ExtractVersion(char *line)
{
    U32 version = 0;
    if(line && *line == '#')
    {
        char *s = strstr(line+1,"\tFileVersion");
        if(s)
            s = strchr(s+1,'\t');
        if(s)
            version = atoi(s+1);
    }
    return s_TextCRCJournalVersion_Init(version);
}

static bool s_TextCRCJournal_Open(TextCRCJournal *journal)
{
    if(!devassert(journal->fn_journal))
        return false;

    if(!journal->fp_journal)
    {
        journal->fp_journal = fopen(journal->fn_journal,"w+b");
        if(!journal->fp_journal)
            FatalErrorf("couldn't create journal %s",journal->fn_journal);
        if(fprintf(journal->fp_journal,"#\tFileVersion\t%i\n",TEXTCRCJOURNAL_VERSION) < 0)
            FatalErrorf("could not write journal header");
    }

    return journal->fp_journal != NULL;
}

static int s_FindHighestLogPart(char *directory, char *stem)
{
    int res = 0;
    int i;
    int n;
    char **files = fileScanDir(directory,&n);
    char *match_expr = NULL;
    estrPrintf(&match_expr,"%s.part*",stem);

    for(i = 0; i < n; ++i)
    {
        char *f = files[i];
        
        if(f && simpleMatch(match_expr,f))
        {
            char *lm = getLastMatch();
            int m;
            if(lm)
            {
                m = atoi(lm);
                res = MAX(m,res);
            }
        }
    }

    estrDestroy(&match_expr);
    fileScanDirFreeNames(files,n);
    return res;
}

static char* s_MakePartFilename(char *res, int n, char *stem, int part_id)
{
    assert(res && stem && part_id > 0);
    sprintf_s(res,n,"%s.part%.3i",stem,part_id);
    return res;
}

void TextCRCJournal_RollJournal(TextCRCJournal *journal)
{
    char fn_rolled[MAX_PATH];
    int new_part_id;

    if(!devassert(journal && journal->directory && journal->fn_journal))
        return;

    if(journal->fp_journal)
    {
        fclose(journal->fp_journal);
        journal->fp_journal = NULL;
    }

    new_part_id = s_FindHighestLogPart(journal->directory,journal->fn_journal) + 1;
    s_MakePartFilename(SAFESTR(fn_rolled),journal->fn_journal,new_part_id);

    if(!fileExists(journal->fn_journal))
    {
        LOG( LOG_TEXT_JOURNAL, LOG_LEVEL_VERBOSE, LOG_CONSOLE_ALWAYS, "journal %s does not exist (attempted to roll to part%.3i)",journal->fn_journal,new_part_id);
    }
    else
    {
        s_FileRename( journal->fn_journal, fn_rolled, journal, "Rolling journal file" );
        journal->merge_pending = true;
    }

    if(journal->log_timer)
        timerStart(journal->log_timer);
}

void TextCRCJournal_Tick(TextCRCJournal *journal)
{
    if(!devassert(journal && (journal->max_journal_size || journal->max_journal_time)))
        return;

    if(journal->max_journal_time && !journal->log_timer)
        journal->log_timer = timerAlloc();

    if(journal->fp_journal)
    {
        if(journal->max_journal_time && timerElapsed(journal->log_timer) > journal->max_journal_time)
        {
            TextCRCJournal_RollJournal(journal);
        }
        else if(journal->max_journal_size)
        {
            U32 file_size;
            fflush(journal->fp_journal);
            file_size = ftell(journal->fp_journal);
            if(file_size > journal->max_journal_size)
                TextCRCJournal_RollJournal(journal);
        }
    }
}

void TextCRCJournal_WriteLine(TextCRCJournal *journal, void *line_struct, ParseTable *parse_table)
{
    char *line_str = NULL;
    U32 crc = 0;

    // TextCRCJournal_Open will create the journal, if necessary
    if(!line_struct || !s_TextCRCJournal_Open(journal))
        return;

    estrStackCreate(&line_str, 1024);
    if(!ParserWriteTextEscaped(&line_str,parse_table,line_struct,0,0))
         FatalErrorf("could not serialize journal line");

    crc = cryptAdler32Str(line_str, estrLength(&line_str));
    if(fprintf(journal->fp_journal,"%i\t%s\n",crc,line_str) < 0)
        FatalErrorf("could not write journal line");
    fflush(journal->fp_journal);
    
    estrDestroy(&line_str);
}

static void s_TextCRCJournal_ReadLine(void *hline, char *str, TextCRCJournalVersion *logfile_version, ParseTable *parse_table, int linenum)
{
     if(!devassert(hline&&str))
        return;

    if(logfile_version && logfile_version->crcd)
    {
        U32 crc = 0;
        U32 crc_calcd = 0;

        crc = atoi(str);
        str = strchr(str,'\t');

        if(!str)
        {
            FatalErrorf("invalid line %s, missing crc tab",str);
            return;
        }
        str++;

        crc_calcd = cryptAdler32Str(str,0);
        if(crc && crc != crc_calcd)
        {
            FatalErrorf("line %i: crc mismatch stored/calcd %i != %i",linenum,crc,crc_calcd);
            return;
        }
    }

     if(!ParserReadTextEscaped(&str,parse_table,hline))
         FatalErrorf("couldn't parse line %s",str);
}

void TextCRCJournal_DoMerge(TextCRCJournal *journal, bool (*process)(void*,void*), size_t size, ParseTable *parse_table, void *userdata)
{
    int i;
    char log_fname[MAX_PATH];
    int highest_part = s_FindHighestLogPart(journal->directory,journal->fn_merge);
    void *line_struct = _alloca(size);

    LOG( LOG_TEXT_JOURNAL, LOG_LEVEL_VERBOSE, LOG_CONSOLE_ALWAYS,"playlogs:starting");

    for(i = 1; i <= highest_part; ++i)
    {
        int i_line = 1;
        int len = 0;
        char *fn = log_fname;
        char *buf;
        char *line, *strtok_context = NULL;
        U32 version = 0;
        TextCRCJournalVersion logfile_version = {0};

        // ab: should probably check file existence in case of crash during rename phase
        s_MakePartFilename(SAFESTR(log_fname),journal->fn_merge,i);
        buf = fileAlloc(fn,&len);
        if(!buf)
            continue;

        // we are actually merging something, remember that we need to spawn a merge process
        // if we are already in the merge process, this isn't checked and won't do anything
        journal->merge_pending = true;

        // TODO: make merge process spam not mix with main process spam
        loadstart_printf("merging %s...",fn);
        LOG( LOG_TEXT_JOURNAL, LOG_LEVEL_VERBOSE, LOG_CONSOLE_ALWAYS, "playlogs:recovering file %s",fn);
        line = strtok_s(buf,"\n",&strtok_context);

        // first line is header
        if(line && *line == '#')
            logfile_version = s_TextCRCJournal_ExtractVersion(line);
        
        while(line)
        {
            char *tmp = strchr(line,'\r');
            if(tmp)
                *tmp = 0;

            if(line[0] != '#')
            {
                memset(line_struct, 0, size);
                s_TextCRCJournal_ReadLine(line_struct, line, &logfile_version, parse_table, i_line);
                if(process(line_struct, userdata))
                {
                    LOG( LOG_TEXT_JOURNAL, LOG_LEVEL_VERBOSE, LOG_CONSOLE_ALWAYS, "TextCRCJournal Line %d: %s", i_line, line);
                    if(journal->assert_on_line_error)
                        FatalErrorFilenamef(fn, "Fatal error during journal playback on line %d:\n"
                                                "%s\n\n"
                                                "Further playback could cause inconsistency!",
                                                i_line, line);
                }
                StructClear(parse_table, line_struct);
            }

            line = strtok_s(NULL,"\n", &strtok_context);
            i_line++;
        }
        free(buf);
        loadend_printf("");
    }
    LOG( LOG_TEXT_JOURNAL, LOG_LEVEL_VERBOSE, LOG_CONSOLE_ALWAYS, "playlogs:done");
}

void TextCRCJournal_CleanupMerge(TextCRCJournal *journal, bool archive)
{
    int i;
    char log_fname[MAX_PATH];
    int highest_version = s_FindHighestLogPart(journal->directory,journal->fn_merge);

    // these are for archiving
    char fn_merged[MAX_PATH];
    char *backup_folder = NULL;
    char *backup_fn = NULL;
    int merged_highest_version = 0; // start at 1
    bool made_folder = false;

    if(archive)
    {
        U32 timestamp = timerSecondsSince2000();
        estrPrintf(&backup_folder,"%smerge_%i/",journal->directory,timestamp);
        estrPrintf(&backup_fn,"%s%s",backup_folder,"merged.log");
    }

    LOG( LOG_TEXT_JOURNAL, LOG_LEVEL_VERBOSE, LOG_CONSOLE_ALWAYS, "%s %i merge files",(archive?"archiving":"deleting"),highest_version);
    for(i = 1; i <= highest_version; ++i)
    {
        char *fn = s_MakePartFilename(SAFESTR(log_fname),journal->fn_merge,i);

        if(archive)
        {
            if(fileExists(fn))
            {
                if(!made_folder)
                {
                    mkdirtree(backup_folder);
                    made_folder = true;
                }
                s_MakePartFilename(SAFESTR(fn_merged),backup_fn,++merged_highest_version);
                s_FileRename( fn, fn_merged, journal, "Cleaning up after merge" );
            }
        }
        else // delete it
        {
            if(0!=fileForceRemove(fn) && fileExists(fn)) // must succeed
                FatalErrorf("during merge: couldn't consume file %s", fn);
        }
    }
    LOG( LOG_TEXT_JOURNAL, LOG_LEVEL_VERBOSE, LOG_CONSOLE_ALWAYS, "done %s merge files",(archive?"archiving":"deleting"));

    if(archive)
    {
        estrDestroy(&backup_folder);
        estrDestroy(&backup_fn);
    }
}


void TextCRCJournal_WriteMergeFiles(TextCRCJournal *journal)
{
    int i;
    char fn[MAX_PATH];
    char fn_merge[MAX_PATH];
    int log_tail_version = s_FindHighestLogPart(journal->directory,journal->fn_journal)+1;
    int merge_highest_version = s_FindHighestLogPart(journal->directory,journal->fn_merge);

    LOG( LOG_TEXT_JOURNAL, LOG_LEVEL_VERBOSE, LOG_CONSOLE_ALWAYS, "write merge files. highest existing: %i, moving %i more",merge_highest_version,log_tail_version-1);
    // move from oldest to newest for consistency. 
    for(i = 1; i < log_tail_version; ++i)
    {
        int retries = 0;
        s_MakePartFilename(SAFESTR(fn),journal->fn_journal,i);
        if(!fileExists(fn))
            continue;
        s_MakePartFilename(SAFESTR(fn_merge),journal->fn_merge,++merge_highest_version);
        s_FileRename( fn, fn_merge, journal, "Writing merge files" );
    }

    journal->merge_pending = false;
}
