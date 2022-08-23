/***************************************************************************
 *     Copyright (c) 2004-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/utils/ConsoleDebug.h>
#include <utilitieslib/components/EString.h>
#include <utilitieslib/utils/FolderCache.h>
#include <utilitieslib/utils/fileutil.h>
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/components/MemoryPool.h>
#include <utilitieslib/components/StashTable.h>

#include "dbcomm.h"

#include "characterInfo.h"

#include "imageServer.h"
#include "entity/entPlayer.h"
#include "comm_backend.h"
#include <utilitieslib/components/earray.h>

#define strdupa(str) strcpy(_alloca(strlen(str)+1), str);
#define INDEX_FILE_COMMENT "# SafeName,Shard,Name,CharacterID,PlayerType,Origin,Archetype,Level,SupergroupID,SuperGroupName,LastActiveTimeSecs,LastActive,URL,PageUpdateTimeSecs,PageUpdateTime,Deleted,XPEarned,Influence,Healing,PvEVictories,PvEDefeats,PvPVictories,PvPDefeats,PvPReputation,BadgesEarned,HoursPlayed,Friends\n"

//#define TRACK_ACTIVE_STATUS
//#define REPORT_BAN_WHERES

// From characterHTML.c
int GenCharacterByName(char *pchName, FILE *fp, bool bUseCaption, bool showBanned, bool forceCostumeRefresh);
int GenCharacter(int dbid, FILE *fp, bool bUseCaption, bool showBanned);

#include <utilitieslib/network/crypt.h>

#define CRC_DATA(x) cryptAdler32Update((void *)&(x), sizeof(x))
#define CRC_STRING(x) if ((x) && *(x)) cryptAdler32Update((const U8 *)(x), (int)(strlen(x)))

static char *s_NoAccessLevelQualifier = "";
static char *s_AccessLevelQualifier = "Ents.AccessLevel is null AND ";

U32 costume_getCRC( Costume * costume )
{
    int i, j, count;

    if( !costume )
        return 0;

    cryptAdler32Init();


     CRC_DATA(costume->appearance.bodytype);
    CRC_DATA(costume->appearance.colorSkin.integer);

    for(i=0;i<MAX_BODY_SCALES;i++)
        CRC_DATA(costume->appearance.fScales[i]);

    CRC_DATA(costume->appearance.iNumParts);

    for(i=0; i<costume->appearance.iNumParts; i++)
    {
        CRC_STRING(costume->parts[i]->pchGeom);
        CRC_STRING(costume->parts[i]->pchTex1);
        CRC_STRING(costume->parts[i]->pchTex2);
        CRC_STRING(costume->parts[i]->pchFxName);

        if (isNullOrNone(costume->parts[i]->pchFxName))
            count = 2;
        else
            count = 4;
        CRC_DATA(count);
        for (j=0; j<count; j++)
        {
            CRC_DATA(costume->parts[i]->color[j].integer);
        }

        CRC_STRING(costume->parts[i]->displayName);
        CRC_STRING(costume->parts[i]->regionName);
        CRC_STRING(costume->parts[i]->bodySetName);
    }

    return cryptAdler32Final();
}


CharacterInfoSettings g_CharacterInfoSettings = {
    false, // delete missing
    false, // alpha subdir
    5,
    0,
    0, // iSecsDelta
    NULL,
    "",
    "",
    "Never",
    "shard",
    "c:/CharInfo/template.html",
    "c:/CharInfo/Req/shard",
    "c:/CharInfo/images",
    "c:/CharInfo/HTML/shard",
    "c:/CharInfo/HTML/shard",
    "c:/CharInfo/db",
    "c:/CharInfo/Costume",
    "../../Images",
    ".",
    "..",
    "page_generator", // default log file name
    false,    // show accesslevel > 0 characters
    false, // show captions
    false, // ignore last update
    7, // min level (0-based, so 8 or better by default)
    90,    // delete inactive characters after this many days
    false, // no html
    false,    // show banned
    true,    // use 'refresh mode' (get all within active time, use existence cache to govern updates)
    12,        // how many megabytes of free disk space are needed
    5,        // how long to wait for disk space to free up
};

static U32 s_uNowActive;


typedef struct OldExistInfo
{
    int dbid;
    U32 uLastActive;
    bool bMarkedOnline;
    U32 iCostumeCRC[MAX_COSTUMES];
} OldExistInfo;

typedef struct AccountBanInfo
{
    bool bDeleted;
    bool bPreserved;
} AccountBanInfo;

MP_DEFINE(AccountBanInfo);

ParseTable parse_ExistInfo[] =
{
//     { "ExistInfo",         TOK_IGNORE | TOK_PARSETABLE_INFO, sizeof(ExistInfo), 0, NULL, 0 },
    { "{",                TOK_START, 0 },
    { "name",           TOK_STRUCTPARAM | TOK_STRING(ExistInfo, name, 0)},
    { "dbid",            TOK_AUTOINT(ExistInfo, dbid, 0), NULL },
    { "uLastActive",    TOK_INT(ExistInfo, uLastActive, 0), 0, TOK_FORMAT_DATESS2000},
    { "MarkedOnline",    TOK_AUTOINT(ExistInfo, bMarkedOnline, 0), NULL },
    { "Generated",        TOK_AUTOINT(ExistInfo, bGenerated, 0), NULL },
    { "CostumeCRC",        TOK_FIXED_ARRAY | TOK_AUTOINTARRAY(ExistInfo, iCostumeCRC, TYPE_ARRAY_SIZE(ExistInfo, iCostumeCRC)), NULL },
    { "}",                TOK_END, 0 },
    { "", 0, 0 }
};

typedef struct ExistInfos
{
    ExistInfo **eis;
} ExistInfos;



TokenizerParseInfo parse_ExistInfos[] =
{
    {    "ExistInfo", TOK_STRUCT(ExistInfos,eis, parse_ExistInfo)},
    {    "", 0, 0 }
};
static StashTable existinfo_from_name;
static StashTable s_bannedAuthTable = 0;

static void CharacterInfoFileMonitor(bool full_scan);
static void s_UpdateQuery(BOOL force);
static void s_ExistenceQuery(BOOL force);
static void CharacterActiveMonitor(void);
static void AccountBanMonitor(bool full_scan);

static void MarkOnlineState(char *pchSafeName, ExistInfo *pei);

static bool writeExistInfoCacheBinary(char *filename)
{
    U32 numEntries;
    U32 neg1 = 0xFFFFFFFF;
    U32 neg2 = 0xFFFFFFFE;
    StashTableIterator hashIter;
    StashElement elem;
    FILE *f;
    const char *hashingKey;
    size_t keyLength;
    int count;

    if (!existinfo_from_name)
        return false;

    numEntries = 0;
    stashGetIterator(existinfo_from_name, &hashIter);
    while (stashGetNextElement(&hashIter, &elem))
    {
        if (stashElementGetPointer(elem))
            numEntries++;
    }

    mkdirtree(filename);
    backSlashes(filename);
    f = fopen(filename, "wb");
    if (!f)
    {
        filelog_printf(g_CharacterInfoSettings.achLogFileName,
            "Error creating existence cache file '%s' for write\n", filename);
        return false;
    }

    // TODO: All these fwrite()s need to check their return values
    // and log errors on failure - guarding against disc filling up.
    if (fwrite(&neg2, sizeof(neg2), 1, f) != 1)
    {
        filelog_printf(g_CharacterInfoSettings.achLogFileName,
            "Error writing new existence cache file '%s' version\n", filename);

        fclose(f);
        return false;
    }

    if (fwrite(&numEntries, sizeof(numEntries), 1, f) != 1)
    {
        filelog_printf(g_CharacterInfoSettings.achLogFileName,
            "Error writing new existence cache file '%s' entry count\n",
            filename);

        fclose(f);
        return false;
    }

    count = 1;
    stashGetIterator(existinfo_from_name, &hashIter);
    while (stashGetNextElement(&hashIter, &elem))
    {
        if (stashElementGetPointer(elem))
        {
            hashingKey = stashElementGetStringKey(elem);
            keyLength = strlen(hashingKey);

            if (fwrite(&keyLength, sizeof(keyLength), 1, f) != 1)
            {
                filelog_printf(g_CharacterInfoSettings.achLogFileName,
                    "Error writing existence cache '%s' entry %d hash\n",
                    filename, count);

                fclose(f);
                return false;
            }

            if (fwrite(hashingKey, sizeof(char), keyLength + 1, f) != keyLength + 1)
            {
                filelog_printf(g_CharacterInfoSettings.achLogFileName,
                    "Error writing existence cache '%s' entry %d key\n",
                    filename, count);

                fclose(f);
                return false;
            }

            if (fwrite(stashElementGetPointer(elem), sizeof(ExistInfo), 1, f) != 1)
            {
                filelog_printf(g_CharacterInfoSettings.achLogFileName,
                    "Error writing existence cache '%s' entry %d info\n",
                    filename, count);

                fclose(f);
                return false;
            }

            count++;
        }
    }

    // This is actually NONZERO for success because we're writing to the
    // native filesystem so under the hood it's IO_WINIO and will call
    // CloseHandle() which works the opposite of all the other close-file
    // calls.
    if (fclose(f) == 0)
    {
        filelog_printf(g_CharacterInfoSettings.achLogFileName,
            "Error closing existence cache file '%s'\n", filename);
        return false;
    }

    return true;
}

static bool writeExistInfoCache(char *filename)
{
    StashTableIterator hashIter;
    StashElement elem;
    ExistInfos dict = {0};

    // we only want the eis in the stashtable
    stashGetIterator(existinfo_from_name, &hashIter);
    while (stashGetNextElement(&hashIter, &elem))
        eaPush(&dict.eis,stashElementGetPointer(elem));
     
    if(!ParserWriteTextFile(filename,parse_ExistInfos,&dict,0,0))
    {
        filelog_printf(g_CharacterInfoSettings.achLogFileName,
            "Error closing existence cache file '%s'\n", filename);
        return false;
    }
    eaDestroy(&dict.eis);
    return TRUE;
}


static void readExistInfoCacheBinary(const char *filename)
{
    FILE *f;
    U32 numEntries;
    char hashingKey[MAX_PATH];
    int keyLength;
    OldExistInfo oei;
    ExistInfo *pei;
    U32 i;
    int j;

    if(existinfo_from_name)
        stashTableDestroy(existinfo_from_name);

    printf("Updating character file list...");
    printf("\b\n");
    existinfo_from_name = stashTableCreateWithStringKeys(5000, StashDeepCopyKeys | StashCaseSensitive );

    f = fopen(filename, "rb");
    if (!f)
        return;

    fread(&numEntries, sizeof(numEntries), 1, f);
    if (numEntries == 0xFFFFFFFF)
    {
        // version 2
        fread(&numEntries, sizeof(numEntries), 1, f);
        for (i = 0; i < numEntries; i++)
        {
            pei = StructAllocRaw(sizeof(ExistInfo)); // set to zero

            fread(&keyLength, sizeof(keyLength), 1, f);
            fread(hashingKey, sizeof(char), keyLength + 1, f);
            fread(&oei, sizeof(OldExistInfo), 1, f);

            if (ferror(f))
            {
                filelog_printf(g_CharacterInfoSettings.achLogFileName,
                    "Error reading existence cache '%s' line %d\n",
                    filename, i);
            }

            pei->bMarkedOnline = oei.bMarkedOnline;
            pei->dbid = oei.dbid;
            pei->uLastActive = oei.uLastActive;
            pei->bGenerated = false;

            for (j = 0; j < MAX_COSTUMES; j++)
            {
                pei->iCostumeCRC[j] = oei.iCostumeCRC[j];
            }

            stashAddPointer(existinfo_from_name, hashingKey, pei, false);
        }
    }
    else if (numEntries == 0xFFFFFFFE)
    {
        // version 3
        fread(&numEntries, sizeof(numEntries), 1, f);
        for (i = 0; i < numEntries; i++)
        {
            pei = StructAllocRaw(sizeof(ExistInfo)); // set to zero

            fread(&keyLength, sizeof(keyLength), 1, f);
            fread(hashingKey, sizeof(char), keyLength + 1, f);
#if defined(_M_X64)
            #pragma message("This code will not run when compiled in 64 bit, it is however compile-able to simplyfy batch builds") 
#else
            STATIC_INFUNC_ASSERT(sizeof(ExistInfo) - 4 == 52);
#endif
            fread(pei, sizeof(ExistInfo) - 4, 1, f);

            if (ferror(f))
            {
                filelog_printf(g_CharacterInfoSettings.achLogFileName,
                    "Error reading existence cache '%s' line %d\n",
                    filename, i);
            }

            stashAddPointer(existinfo_from_name, hashingKey, pei, false);
        }
    }
    else
    {
        for (i = 0; i < numEntries; i++)
        {
            pei = StructAllocRaw(sizeof(ExistInfo)); // set to zero

            fread(&keyLength, sizeof(keyLength), 1, f);
            fread(hashingKey, sizeof(char), keyLength + 1, f);
            fread(&pei->dbid, sizeof(pei->dbid), 1, f);
            fread(&pei->uLastActive, sizeof(pei->uLastActive), 1, f);
            fread(&pei->bMarkedOnline, sizeof(pei->bMarkedOnline), 1, f);
            fread(&pei->iCostumeCRC[0], sizeof(pei->iCostumeCRC[0]), 1, f);
            for (j = 1; j < MAX_COSTUMES; j++)
                pei->iCostumeCRC[j] = 0;

            if (ferror(f))
            {
                filelog_printf(g_CharacterInfoSettings.achLogFileName,
                    "Error reading existence cache '%s' line %d\n",
                    filename, i);
            }

            stashAddPointer(existinfo_from_name, hashingKey, pei, false);
        }
    }

    fclose(f);
}

static void readExistInfoCache(char *filename)
{
    int i;
    ExistInfos dict = {0};
     
    if(existinfo_from_name)
        stashTableDestroy(existinfo_from_name);        
    existinfo_from_name = stashTableCreateWithStringKeys(5000, StashDeepCopyKeys | StashCaseSensitive );

    ParserReadTextFile(filename,parse_ExistInfos,&dict);
    for( i = eaSize(&dict.eis) - 1; i >= 0; --i)
    {
        ExistInfo *ei = dict.eis[i];
        if(!stashAddPointer(existinfo_from_name, ei->name, ei, FALSE))
            FatalErrorf("duplicate name %s", ei->name);
    }
    eaDestroy(&dict.eis);
}


static void saveExistInfoCache(void)
{
    char cacheDB[MAX_PATH];
    char cacheDBTemp[MAX_PATH];
    char cacheDBOld[MAX_PATH];
    bool success;

    sprintf(cacheDB, "%s/existinfo%s%s%s%s.cache.txt",
        g_CharacterInfoSettings.achRequestPath,
        *g_CharacterInfoSettings.achNameStart||*g_CharacterInfoSettings.achNameEnd?".":"",
        g_CharacterInfoSettings.achNameStart,
        *g_CharacterInfoSettings.achNameStart||*g_CharacterInfoSettings.achNameEnd?"-":"",
        g_CharacterInfoSettings.achNameEnd);
    backSlashes(cacheDB);
    sprintf(cacheDBTemp, "%s.temp", cacheDB);
    sprintf(cacheDBOld, "%s.old", cacheDB);
    success = writeExistInfoCache(cacheDBTemp);

    if (success)
    {
        if (fileExists(cacheDBTemp))
        {
            if (fileExists(cacheDB))
            {
                if (rename(cacheDB, cacheDBOld) == 0)
                {
                    if (rename(cacheDBTemp, cacheDB) == 0)
                    {
                        if (!DeleteFileA(cacheDBOld))
                        {
                            filelog_printf(g_CharacterInfoSettings.achLogFileName,
                                "Could not delete old existence cache '%s'\n",
                                cacheDBOld);
                        }
                    }
                    else
                    {
                        filelog_printf(g_CharacterInfoSettings.achLogFileName,
                            "Could not rename existence cache from '%s' to '%s'\n",
                            cacheDBTemp, cacheDB);

                        if (rename(cacheDBOld, cacheDB))
                        {
                            filelog_printf(g_CharacterInfoSettings.achLogFileName,
                                "Could not restore old existence cache '%s'\n",
                                cacheDBOld);
                        }
                    }
                }
                else
                {
                    filelog_printf(g_CharacterInfoSettings.achLogFileName,
                        "Could not rename old existence cache from '%s' to '%s'\n",
                        cacheDB, cacheDBOld);
                }
            }
            else
            {
                if (rename(cacheDBTemp, cacheDB) != 0)
                {
                    filelog_printf(g_CharacterInfoSettings.achLogFileName,
                        "Could not rename new existence cache from '%s' to '%s'\n",
                        cacheDBTemp, cacheDB);
                }
            }
        }
        else
        {
            filelog_printf(g_CharacterInfoSettings.achLogFileName,
                "New existence cache file '%s' disappeared!\n", cacheDBTemp);
        }
    }
    else
    {
        filelog_printf(g_CharacterInfoSettings.achLogFileName,
            "Could not successfully create existence cache '%s' - deleting\n", cacheDBTemp);
        DeleteFileA(cacheDBTemp);
    }
}

void loadExistInfoCache(void)
{
    char cacheDB[MAX_PATH];
    sprintf(cacheDB, "%s/existinfo%s%s%s%s.cache",
        g_CharacterInfoSettings.achRequestPath,
        *g_CharacterInfoSettings.achNameStart||*g_CharacterInfoSettings.achNameEnd?".":"",
        g_CharacterInfoSettings.achNameStart,
        *g_CharacterInfoSettings.achNameStart||*g_CharacterInfoSettings.achNameEnd?"-":"",
        g_CharacterInfoSettings.achNameEnd);
    backSlashes(cacheDB);
    if(fileExists(cacheDB)) // migrate binary to text representation
    {
        StashTableIterator hashIter;
        StashElement elem;

        readExistInfoCacheBinary(cacheDB);

        // fixup name
        stashGetIterator(existinfo_from_name, &hashIter);
        while (stashGetNextElement(&hashIter, &elem))
        {
            ExistInfo *ei = stashElementGetPointer(elem);
            char *name = StructAllocString(stashElementGetStringKey(elem));
            ei->name = name;
        }

        // write out the text and get rid of binary
        saveExistInfoCache();
        fileMoveToBackup(cacheDB);
    }
    else
    {
        strcat(cacheDB,".txt");
        readExistInfoCache(cacheDB);
    }
}

static void conRunHourly(void)
{
    printf("console: running hourly query\n");
    s_UpdateQuery(TRUE);
}

static void conRunFullExist(void)
{
    printf("console: running full existence query\n");
    s_ExistenceQuery(TRUE);    
}
 
static void conCheckBans(void)
{
    printf("console: running full account bans query\n");
    AccountBanMonitor(TRUE);
}

static void conWriteCsvs(void)
{
    RefreshModeType cur = g_CharacterInfoSettings.RefreshMode;
    BOOL cur_no_html = g_CharacterInfoSettings.bNoHTML;
    printf("console: rewriting csvs\n");
    printf("running existence\n");
    g_CharacterInfoSettings.RefreshMode = RefreshModeType_Always;
    g_CharacterInfoSettings.bNoHTML = TRUE;
    
    s_ExistenceQuery(TRUE);
    s_UpdateQuery(TRUE);

    g_CharacterInfoSettings.bNoHTML = cur_no_html;
    g_CharacterInfoSettings.RefreshMode = cur;

    printf("re-writing csvs\n");    
}



ConsoleDebugMenu charweb_debug[] =
{
    { 0,        "General commands:",            NULL,                    NULL        },
    { 'h',        "hourly save",    conRunHourly,            NULL        },
    { 'e',        "full existence query", conRunFullExist,            NULL        },
    { 'f',        "check bans", conCheckBans,            NULL        },
    { 'C',        "rewrite csvs", conWriteCsvs,            NULL        },
};
    

void UpdateConsoleTitle(void)
{
    static char *buf = NULL;
    estrPrintf(&buf, "CharWeb built " __DATE__ "; shard %s",
        g_CharacterInfoSettings.achShardName);
    setConsoleTitle(buf);
}

/**********************************************************************func*
 * characterInfoMonitor
 *
 */
void characterInfoMonitor(void)
{
    char ach[MAX_PATH];

    // This puts my temp files where I want them.
    _putenv("TMP=");

    if(g_CharacterInfoSettings.iIntervalMinutes==0) g_CharacterInfoSettings.iIntervalMinutes = 5;

    printf("--------------------------------------------------------------\n");
    printf("Character HTML server (v%s) started\n", XML_VERSION_STRING);
    printf("--------------------------------------------------------------\n");
    printf("Generating character HTML pages for shard %s.\n", g_CharacterInfoSettings.achShardName);
    printf("Creating HTML in %s\n", g_CharacterInfoSettings.achHTMLDestPath);
    printf("Creating images in %s via costume server in %s\n", g_CharacterInfoSettings.achCharacterImageDestPath, g_CharacterInfoSettings.achCostumeRequestPath);

    filelog_printf(g_CharacterInfoSettings.achLogFileName,
        "----------------------------------------------------\n");
    filelog_printf(g_CharacterInfoSettings.achLogFileName,
        "Character page generator %s started\n", XML_VERSION_STRING);
    filelog_printf(g_CharacterInfoSettings.achLogFileName,
        "----------------------------------------------------\n");
    filelog_printf(g_CharacterInfoSettings.achLogFileName,
        "Creating character pages for shard %s\n",
        g_CharacterInfoSettings.achShardName);

    sprintf(ach, "%s/*.csv", g_CharacterInfoSettings.achRequestPath);
    mkdirtree(ach);
    sprintf(ach, "%s/*.html", g_CharacterInfoSettings.achHTMLDestPath);
    mkdirtree(ach);
    sprintf(ach, "%s/*.jpg", g_CharacterInfoSettings.achCharacterImageDestPath);
    mkdirtree(ach);
    sprintf(ach, "%s/*.csv", g_CharacterInfoSettings.achCostumeRequestPath);
    mkdirtree(ach);
    sprintf(ach, "%s/*.csv", g_CharacterInfoSettings.achWebDBPath);
    mkdirtree(ach);

    if(g_CharacterInfoSettings.pchTemplateString==NULL)
    {
        int iSize = fileSize(g_CharacterInfoSettings.achTemplate);
        FILE *fpIn = fopen(g_CharacterInfoSettings.achTemplate, "r");

        if(fpIn==NULL)
        {
            g_CharacterInfoSettings.pchTemplateString = strdup("{HeroHTML}");
        }
        else
        {
            g_CharacterInfoSettings.pchTemplateString = calloc(iSize*2+1, 1); // EOL transform might make it bigger, I think.
            iSize = fread(g_CharacterInfoSettings.pchTemplateString, iSize, 1, fpIn);
            fclose(fpIn);
        }
    }

    if(dbTryConnect(15.0, DEFAULT_DB_PORT))
    {
        U32 uNow = timerSecondsSince2000();
        bool full_file_scan = true;

        while(1)
        {
            if(!db_comm_link.connected)
            {
                // We got disconnected.
                printf("*** Warning: DBServer %s disconnected. Reconnecting... ", db_state.server_name);
                filelog_printf(g_CharacterInfoSettings.achLogFileName,
                    "DBServer %s disconnected!\n", db_state.server_name);

                if(dbTryConnect(15.0, DEFAULT_DB_PORT))
                {
                    printf("Reconnected!\n");
                    filelog_printf(g_CharacterInfoSettings.achLogFileName,
                        "Reconnected to DBServer\n");
                }
                else
                {
                    dbClearContainerIDCache();
                    printf("Failed! Sleeping for a minute.\n");
                    filelog_printf(g_CharacterInfoSettings.achLogFileName,
                        "Failed to reconnect - retrying in 60 seconds...\n");
                    Sleep(60);
                }
            }
            else
            {
                AccountBanMonitor(full_file_scan);
                s_ExistenceQuery(FALSE);     // 90 day query, check for deletes
                s_UpdateQuery(FALSE);        // hourly query, who is new/updated
            }

            CharacterInfoFileMonitor(full_file_scan);
            full_file_scan = false;
            CharacterActiveMonitor();
            Sleep(1);

            // Every once in a while we force a scan as a safety feature
            // because the caching doesn't work over network shares.
            if ((timerSecondsSince2000() - uNow) > 3600)
            {
                uNow = timerSecondsSince2000();
                full_file_scan = true;
            }

            DoConsoleDebugMenu(charweb_debug);
            UpdateConsoleTitle();
        }

        if(db_comm_link.connected)
        {
            netSendDisconnect(&db_comm_link,4.f);
        }
    }
    else
    {
        printf("*** Error: Unable to connect to db\n");
        filelog_printf(g_CharacterInfoSettings.achLogFileName,
            "Unable to connect to DBServer %s - aborting!",
            db_state.server_name);

        while (1)
        {
            Sleep(3600);
        }
    }
}


/**********************************************************************func*
 * CharacterInfoCallback
 *
 */
static void CharacterInfoCallback(const char *relPath, int when)
{
#define DELIMS "\t, \r\n"
    char fullpath[MAX_PATH];
    char *data, *line;
    int succeeded=0, ret;
    int num_args;
    int file_arg;
    char *args[5];
    char achName[256];
    char *pchName;
    FILE *fp;

    if (strchr(relPath+1, '/'))
    {
        // Don't process things in subdirectories
        return;
    }

    pchName = strdupa(_tempnam(g_CharacterInfoSettings.achWebDBPath, g_CharacterInfoSettings.achShardName));
    fp = fopen(pchName, "w");
    fprintf(fp,INDEX_FILE_COMMENT);

    sprintf(fullpath, "%s%s", g_CharacterInfoSettings.achRequestPath, relPath);

    g_CharacterInfoSettings.uLastUpdate=timerSecondsSince2000();
    timerMakeDateStringFromSecondsSince2000(g_CharacterInfoSettings.achLastUpdate, g_CharacterInfoSettings.uLastUpdate);

    printf("----- %s\n", g_CharacterInfoSettings.achLastUpdate);
    printf("Processing request file: %s\n", fullpath);
    fileWaitForExclusiveAccess(fullpath);
    data = fileAlloc(fullpath, NULL);
    for (line=data; *line; line++)
    {
        if (*line==',')
            *line = '\t';
    }
    line = data;
    file_arg = 1;
    while (line)
    {
        num_args = tokenize_line_safe(line, args, ARRAY_SIZE(args), &line);
        if (num_args == 0)
            continue;
        if (num_args < 1)
        {
            printf("Invalid number of arguments on line: %d\n", num_args);
            filelog_printf(g_CharacterInfoSettings.achLogFileName,
                "Invalid arguments in '%s', after argument %d\n", relPath, file_arg);
        }
        else
        {
            file_arg++;

            Strncpyt(achName, args[0]);

            // Force-regeneration of characters causes all their costumes
            // to get rebuilt as well, in case they are corrupt for some
            // reason.
            ret = GenCharacterByName(achName, fp, g_CharacterInfoSettings.bUseCaptions, g_CharacterInfoSettings.bShowBanned, true);

            if (ret==0)
                succeeded = 1;
        }
    }
    free(data);
    {
        // Save Request and intermediate data
        char newname[MAX_PATH];
        char cmdline[1024];
        if (succeeded)
        {
            sprintf(newname, "%s/processed%s", g_CharacterInfoSettings.achRequestPath, relPath);
        }
        else
        {
            sprintf(newname, "%s/failed%s", g_CharacterInfoSettings.achRequestPath, relPath);
        }
        mkdirtree(newname);
        backSlashes(fullpath);
        backSlashes(newname);
        sprintf(cmdline, "move /Y %s %s > nul", fullpath, newname);
        system(cmdline);
    }

    if(fp)
    {
        fprintf(fp, "# End\n");
        fclose(fp);
    }


    printf("Done.\n");
}

/**********************************************************************func*
 * CharacterInfoProcessor
 *
 */
static FileScanAction CharacterInfoProcessor(char *dir, struct _finddata32_t *data)
{
    char fullpath[MAX_PATH];
    sprintf(fullpath, "%s/%s", dir, data->name);
    if (simpleMatch("*.csv", fullpath))
    {
        char relpath[MAX_PATH];
        sprintf(relpath, "/%s", data->name);
        CharacterInfoCallback(relpath, 0);
    }
    return FSA_NO_EXPLORE_DIRECTORY;
}

/**********************************************************************func*
 * CharacterInfoFileMonitor
 *
 */
static void CharacterInfoFileMonitor(bool full_scan)
{
    static FolderCache *fcCharacterInfoSettings = 0;
    char fullpath[MAX_PATH];
    sprintf(fullpath, "%s", g_CharacterInfoSettings.achRequestPath);

    if(fcCharacterInfoSettings==0)
    {
        printf("--------------------------------------------------------------\n");
        printf("Monitoring for new file requests in %s/%s\n", fullpath, "*.csv");
        printf("--------------------------------------------------------------\n");

        filelog_printf(g_CharacterInfoSettings.achLogFileName,
            "Monitoring for new .csv request files\n");

        // Look for pending transfers
        printf("Processing existing file requests...\n");

        fileScanDirRecurseEx(fullpath, CharacterInfoProcessor);

        fcCharacterInfoSettings = FolderCacheCreate();
        FolderCacheAddFolder(fcCharacterInfoSettings, fullpath, 0);
        FolderCacheSetCallback(FOLDER_CACHE_CALLBACK_UPDATE|FOLDER_CACHE_CALLBACK_CAN_USE_SHARED_MEM, "*.csv", CharacterInfoCallback);
        FolderCacheEnableCallbacks(1);

        printf("  Done.\n");
    }

    // Take the full path scan in case the folder caching misses it for
    // some reason.
    if (full_scan)
    {
        fileScanDirRecurseEx(fullpath, CharacterInfoProcessor);
    }
    else
    {
        FolderCacheQuery(fcCharacterInfoSettings, ""); // Just to get Update
    }
}

/**********************************************************************func*
 * DumpCharactersCallback
 *
 */
static void DumpCharactersCallback(Packet *pak, int evil, int count)
{
    if(count>0)
    {
        int i;
        U32 uLastCacheTime;
        FILE *fp = NULL;
        char *pchName;
        U32 num_processed = 0;

        g_CharacterInfoSettings.uLastUpdate=timerSecondsSince2000();
        timerMakeDateStringFromSecondsSince2000(g_CharacterInfoSettings.achLastUpdate, g_CharacterInfoSettings.uLastUpdate);
        printf("----- %s\n", g_CharacterInfoSettings.achLastUpdate);
        printf("Processing DB update of %d characters.\n", count);
        filelog_printf(g_CharacterInfoSettings.achLogFileName,
            "DBServer returned %d characters\n", count);

        pchName = strdupa(_tempnam(g_CharacterInfoSettings.achWebDBPath, g_CharacterInfoSettings.achShardName));
        fp = fopen(pchName, "w");
        fprintf(fp,INDEX_FILE_COMMENT);

        uLastCacheTime = timerSecondsSince2000();

        for(i=0; i<count; i++)
        {
            int dbid = pktGetBitsPack(pak,1);
            U32 lastactive = pktGetBits(pak,32);

            // check if the lastactive in our cache matches the dbserver
             if (CheckGeneratedExistsInfo(dbid,lastactive))
                continue;
            num_processed++;
            
            // Make sure there's enough space for us to write the data and
            // a clean copy of the existence cache.
            while (fileGetFreeDiskSpace(g_CharacterInfoSettings.achCostumeRequestPath) < (g_CharacterInfoSettings.iMinDiskFreeMB * 1024 * 1024) ||
                fileGetFreeDiskSpace(g_CharacterInfoSettings.achRequestPath) < (g_CharacterInfoSettings.iMinDiskFreeMB * 1024 * 1024))
            {
                printf("Waiting for drive space to free up!\n");
                Sleep((g_CharacterInfoSettings.iLowDiskStallTime * 60 * 1000));
            }

            if (GenCharacter(dbid, fp, g_CharacterInfoSettings.bUseCaptions, g_CharacterInfoSettings.bShowBanned) == -1)
            {
                dbCommShutdown();
            }

            if(!db_comm_link.connected)
            {
                break;
            }

            // Don't wait for a whole pass to write the existence cache, keep
            // it updated as we go so that if we run out of disc space or
            // crash we won't lose progress.
            if ((timerSecondsSince2000() - uLastCacheTime) > 3*60)
            {
                saveExistInfoCache();
                uLastCacheTime = timerSecondsSince2000();
            }
        }

        if(fp)
        {
            fprintf(fp, "# End\n");
            fclose(fp);
        }


        saveExistInfoCache();
        uLastCacheTime = timerSecondsSince2000();
        printf("Done, processed %i.\n",num_processed);
    }
}

/**********************************************************************func*
 * look for new characters, delete missing
 *
 */
static void s_UpdateQuery(BOOL force)
{
    static bool firstTime = true;
    static U32 uLastUpdate = 0;
    U32 uNow = timerSecondsSince2000()+g_CharacterInfoSettings.iSecsDelta;

    if(force || uNow > uLastUpdate+60*g_CharacterInfoSettings.iIntervalMinutes)
    {
        static PerformanceInfo* perfInfo;
        char *pchWhere;
        char fullpath[MAX_PATH];
        char achDate[64];
        char achInactiveDate[64];
        char *pchAccessLevel = s_AccessLevelQualifier;
        FILE *fp;

        estrCreate(&pchWhere);

        sprintf(fullpath, "%s/lastupdate%s%s%s%s.txt",
            g_CharacterInfoSettings.achRequestPath,
            *g_CharacterInfoSettings.achNameStart||*g_CharacterInfoSettings.achNameEnd?".":"",
            g_CharacterInfoSettings.achNameStart,
            *g_CharacterInfoSettings.achNameStart||*g_CharacterInfoSettings.achNameEnd?"-":"",
            g_CharacterInfoSettings.achNameEnd);

        if(uLastUpdate==0)
        {
            char achBuf[21];

            fp = fopen(fullpath, "rt");
            if(fp)
            {
                fgets(achBuf, 20, fp);
                uLastUpdate = atoi(achBuf);
                fclose(fp);
            }

            if(uLastUpdate==0)
            {
                uLastUpdate = uNow-(1*24); // one hour ago
            }

            timerMakeDateStringFromSecondsSince2000(achDate, uLastUpdate);
            printf("--------------------------------------------------------------\n");
            printf("Monitoring database on %s for character updates.\n", db_state.server_name);
            printf("Last update from DB: %s\n", achDate);
            printf("--------------------------------------------------------------\n");
            filelog_printf(g_CharacterInfoSettings.achLogFileName,
                "Monitoring DBServer %s for character updates since %s\n",
                db_state.server_name, achDate);
        }

        uLastUpdate -= 20; // Give a little slack.
        timerMakeDateStringFromSecondsSince2000(achDate, uLastUpdate);

        // In refresh mode we want everyone who's logged in since the cutoff
        // date, which is N days ago.
        timerMakeDateStringFromSecondsSince2000(achInactiveDate, uNow - g_CharacterInfoSettings.iInactiveDeleteTimeDays*60*60*24);

        if (g_CharacterInfoSettings.bIncludeAccessLevelChars)
            pchAccessLevel = s_NoAccessLevelQualifier;

//         if (firstTime && g_CharacterInfoSettings.bIgnoreLastUpdate)
//             estrConcatf(&pchWhere, "WHERE %sEnts.Level>=%d", pchAccessLevel, g_CharacterInfoSettings.iMinLevel);
        if ((firstTime && g_CharacterInfoSettings.bIgnoreLastUpdate)
            || g_CharacterInfoSettings.RefreshMode)
            estrConcatf(&pchWhere, "WHERE %sEnts.Level>=%d AND Ents.LastActive>'%s'", pchAccessLevel, g_CharacterInfoSettings.iMinLevel, achInactiveDate);
        else
            estrConcatf(&pchWhere, "WHERE %sEnts.Level>=%d AND Ents.LastActive>'%s'", pchAccessLevel, g_CharacterInfoSettings.iMinLevel, achDate);

        if(!g_CharacterInfoSettings.bShowBanned)
            estrConcatf(&pchWhere, " AND isnull(banned,0)=0");

        if(*g_CharacterInfoSettings.achNameStart)
        {
            estrConcatf(&pchWhere, " AND lower(Ents.Name)>='%s'", g_CharacterInfoSettings.achNameStart);
        }
        if(*g_CharacterInfoSettings.achNameEnd)
        {
            estrConcatf(&pchWhere, " AND lower(Ents.Name)<'%s'", g_CharacterInfoSettings.achNameEnd);
        }

        uLastUpdate = timerSecondsSince2000()+g_CharacterInfoSettings.iSecsDelta;

        dbReqCustomData(CONTAINER_ENTS, "Ents", "", pchWhere, "ContainerId, LastActive", DumpCharactersCallback, 0);
        uLastUpdate = uNow;

        dbMessageScanUntil("DBCLIENT_REQ_CUSTOM_DATA", &perfInfo);

        if(db_comm_link.connected)
        {
            fp = fopen(fullpath, "w");
            if(fp)
            {
                fprintf(fp, "%d\n// %s", uLastUpdate, achDate);
                fclose(fp);
            }
        }

        estrDestroy(&pchWhere);
        saveExistInfoCache();
    }

    if (firstTime)
        firstTime = false;
}

/**********************************************************************func*
 * whirl
 *
 */
static void whirl(void)
{
    static char ach[] = { '/', '-', '\\', '|' };
    static int i = 0;

    printf("\b%c", ach[i++]);

    if(i>=ARRAY_SIZE(ach))
        i=0;
}


/**********************************************************************func*
 * ExistsInDBProc
 *
 */
static void ExistsInDBProc(Packet *pak, int evil, int count)
{
    int i;

    for(i=0; i<count; i++)
    {
        char ach[64];

        int dbid = pktGetBitsPack(pak, 1);
        char *pchName = pktGetString(pak);
        U32 uLastActive = pktGetBits(pak, 32);

        strcpy(ach, pchName);
        imageserver_MakeSafeFilename(ach);

        UpdateExistsInfo(ach, dbid, uLastActive);
        whirl();
    }
}

/**********************************************************************func*
 * ClearDbidProc
 *
 */
static int ClearDbidProc(StashElement elem)
{
    ExistInfo *pei = (ExistInfo *)stashElementGetPointer(elem);
    pei->dbid = 0;
    return 1;
}

static FILE *s_fpDeleteLog = NULL;
/**********************************************************************func*
 * RemoveDeletedProc
 *
 */
static int RemoveDeletedProc(StashElement elem)
{
    ExistInfo *pei = (ExistInfo *)stashElementGetPointer(elem);
    const char *pch = stashElementGetStringKey(elem);

    if(pei->dbid)
        return 1; // skip

//         char ach[MAX_PATH];
//         char achCostume[50];
//         int count;

        // ab: don't delete files
//         strcpy(ach, MakeCharacterFilename(pch, ".xml"));
//         unlink(ach);

//         strcpy(ach, MakeCharacterFilename(pch, ".html"));
//         printf("    Removing deleted character page '%s'\n", ach);
//         unlink(ach);

//         strcpy(ach, MakePictureFilename(pch, "_status.gif"));
//         unlink(ach);

//         strcpy(ach, MakePictureFilename(pch, ".jpg"));
//         unlink(ach);

//         for (count = 0; count < 5; count++)
//         {
//             sprintf(achCostume, "_Costume%d.jpg", count + 1);
//             strcpy(ach, MakePictureFilename(pch, achCostume));
//             unlink(ach);
//         }

//         strcpy(ach, MakePictureFilename(pch, "_Thumbnail.jpg"));
//         unlink(ach);

    if(s_fpDeleteLog)
    {
        fprintf(s_fpDeleteLog,"%s,%s,,,,,,,,,,,%s,,,1\n",
                pch,
                g_CharacterInfoSettings.achShardName,
                MakeCharacterURL(pch, ".html"));
    }
    
    stashRemovePointer(existinfo_from_name, pch, NULL);
    StructFree(pei);
    return 1;
}

static void LogBannedCharactersByAuthCallback(Packet *pak, int unused_dbid, int data_count)
{
    int count;
    int dbid;
    char *charname;

    for (count = 0; count < data_count; count++)
    {
        dbid = pktGetBitsPack(pak, 1);
        charname = (char *)(unescapeString(pktGetString(pak)));

        if (s_fpDeleteLog)
        {
            fprintf(s_fpDeleteLog,"%s,%s,,,,,,,,,,,%s,,,1\n",
                charname,
                g_CharacterInfoSettings.achShardName,
                MakeCharacterURL(charname, ".html"));
        }

        // We can't force them to be updated if they are unbanned (that's
        // controlled by the DB query of the last active time) but we can
        // ensure their costume is regenerated by setting the CRCs to zero.
        ResetCostumeCRCs(dbid);
    }
}

// gets banned characters by authname 
static void CheckAccountBanProc(void)
{
    StashTableIterator iter;
    StashElement elem;
    AccountBanInfo *pei;
    const char *pch;
    int bucket = 0;
    char *pwhere = NULL;

    stashGetIterator(s_bannedAuthTable, &iter);

    while (stashGetNextElement(&iter, &elem))
    {
        pei = (AccountBanInfo *)stashElementGetPointer(elem);
        pch = stashElementGetStringKey(elem);

        if (!pei->bDeleted)
        {
            if (!bucket)
            {
                estrCreate(&pwhere);
                estrPrintf(&pwhere, "WHERE AuthName = '%s'", escapeString(pch));
            }
            else
            {
                estrConcatf(&pwhere, " OR AuthName = '%s'", escapeString(pch));
            }

            bucket++;

            if (bucket == 20)
            {
#ifdef REPORT_BAN_WHERES
                filelog_printf("ban_queries", pwhere);
#endif
                dbReqCustomData(CONTAINER_ENTS, "Ents", "", pwhere, "ContainerId, Name", LogBannedCharactersByAuthCallback, 0);
                dbMessageScanUntil("DBCLIENT_REQ_CUSTOM_DATA", NULL);
                estrDestroy(&pwhere);
                bucket = 0;
            }

            pei->bDeleted = true;
        }
    }

    if (bucket > 0)
    {
#ifdef REPORT_BAN_WHERES
        filelog_printf("ban_queries", pwhere);
#endif
        dbReqCustomData(CONTAINER_ENTS, "Ents", "", pwhere, "ContainerId, Name", LogBannedCharactersByAuthCallback, 0);
        dbMessageScanUntil("DBCLIENT_REQ_CUSTOM_DATA", NULL);
        estrDestroy(&pwhere);
    }
}

bool IsAuthnameOnBannedList(char *pchAuthname)
{
    bool retval = false;
    void *nameptr;

    if (pchAuthname)
    {
        if (stashFindPointer(s_bannedAuthTable, pchAuthname, &nameptr))
        {
            retval = true;
        }
    }

    return retval;
}

static char **s_BannedList = NULL;
/**********************************************************************func*
 * LogBannedCharacter
 *
 */
void LogBannedCharacter(char *pchName)
{
    char *str;
    int len;

    if (!s_BannedList)
    {
        eaCreate(&s_BannedList);
    }

    if (pchName)
    {
        len = (int)strlen(pchName) + 1;
        str = malloc(len);

        strcpy_s(str, len, pchName);
        str[len - 1] = '\0';
        eaPush(&s_BannedList, str);
    }
}

/**********************************************************************func*
 * AddBannedCharactersToDeleteLog
 *
 */
void AddBannedCharactersToDeleteLog(void)
{
    int count;

    // Go through the array and write a delete entry for every name in it.
    for (count = 0; count < eaSize(&s_BannedList); count++)
    {
        if (s_BannedList[count])
        {
            fprintf(s_fpDeleteLog,"%s,%s,,,,,,,,,,,%s,,,1\n",
                s_BannedList[count],
                g_CharacterInfoSettings.achShardName,
                MakeCharacterURL(s_BannedList[count], ".html"));
        }

        // Clear up the copy of the name so we don't leak memory.
        free(s_BannedList[count]);
    }

    // 
    eaDestroy(&s_BannedList);
}

static void AccountBanCallback(const char *relPath, int when)
{
#define DELIMS "\t, \r\n"
    char fullpath[MAX_PATH];
    char *data, *line;
    int succeeded=0;
    int num_args;
    int file_arg;
    char *args[5];
    char achName[256];
    char *pchName;

    if (strchr(relPath+1, '/'))
    {
        // Don't process things in subdirectories
        return;
    }

    pchName = strdupa(_tempnam(g_CharacterInfoSettings.achWebDBPath, g_CharacterInfoSettings.achShardName));

    sprintf(fullpath, "%s%s", g_CharacterInfoSettings.achRequestPath, relPath);

    g_CharacterInfoSettings.uLastUpdate=timerSecondsSince2000();
    timerMakeDateStringFromSecondsSince2000(g_CharacterInfoSettings.achLastUpdate, g_CharacterInfoSettings.uLastUpdate);

    printf("----- %s\n", g_CharacterInfoSettings.achLastUpdate);
    printf("Merging account ban file: %s\n", fullpath);
    fileWaitForExclusiveAccess(fullpath);
    data = fileAlloc(fullpath, NULL);
    for (line=data; *line; line++)
    {
        if (*line==',')
            *line = '\t';
    }
    line = data;
    file_arg = 1;
    while (line)
    {
        num_args = tokenize_line_safe(line, args, ARRAY_SIZE(args), &line);
        if (num_args == 0)
            continue;
        if (num_args < 1)
        {
            printf("Invalid number of arguments on line: %d\n", num_args);
            filelog_printf(g_CharacterInfoSettings.achLogFileName,
                "Invalid arguments in '%s', after argument %d\n", relPath, file_arg);
        }
        else
        {
            AccountBanInfo *newAuth;

            file_arg++;

            Strncpyt(achName, args[0]);

            if (!stashFindPointer(s_bannedAuthTable, achName, &newAuth))
            {
                newAuth = MP_ALLOC(AccountBanInfo);
                newAuth->bDeleted = false;
                newAuth->bPreserved = true;
                stashAddPointer(s_bannedAuthTable, achName, newAuth, true);
            }
            else
            {
                // We already knew this name, so mark it to be kept.
                newAuth->bPreserved = true;
            }
        }
    }
    free(data);

    printf("Done.\n");
}

static FileScanAction AccountBanProcessor(char *dir, struct _finddata32_t *data)
{
    char fullpath[MAX_PATH];
    sprintf(fullpath, "%s/%s", dir, data->name);
    if (simpleMatch("*.ban", fullpath))
    {
        char relpath[MAX_PATH];
        sprintf(relpath, "/%s", data->name);
        AccountBanCallback(relpath, 0);
    }
    return FSA_NO_EXPLORE_DIRECTORY;
}


//----------------------------------------
//  check for new .ban files to ban by auth name
//----------------------------------------
static void AccountBanMonitor(bool full_scan)
{
    static FolderCache *fcAccountBanSettings = 0;
    char fullpath[MAX_PATH];
    bool scanned = false;

    sprintf(fullpath, "%s", g_CharacterInfoSettings.achRequestPath);

    if(!s_bannedAuthTable)
    {
        s_bannedAuthTable = stashTableCreateWithStringKeys(5000, StashDeepCopyKeys | StashCaseSensitive );
        MP_CREATE(AccountBanInfo, 5000);
    }

    if(fcAccountBanSettings==0)
    {
        printf("--------------------------------------------------------------\n");
        printf("Monitoring for account ban lists in %s/%s\n", fullpath, "*.ban");
        printf("--------------------------------------------------------------\n");

        filelog_printf(g_CharacterInfoSettings.achLogFileName,
            "Monitoring for new .ban files\n");

        // Look for pending transfers
        printf("Processing initial ban files...\n");

        fileScanDirRecurseEx(fullpath, AccountBanProcessor);
        scanned = true;

        fcAccountBanSettings = FolderCacheCreate();
        FolderCacheAddFolder(fcAccountBanSettings, fullpath, 0);
        FolderCacheSetCallback(FOLDER_CACHE_CALLBACK_UPDATE|FOLDER_CACHE_CALLBACK_CAN_USE_SHARED_MEM, "*.ban", AccountBanCallback);
        FolderCacheEnableCallbacks(1);

        printf("  Done.\n");
    }

    if (full_scan)
    {
        // No need to double-scan in the initial setup.
        if (!scanned)
        {
            StashTableIterator iter;
            StashElement elem;
            AccountBanInfo* info;

            stashGetIterator(s_bannedAuthTable, &iter);

            // Set everything as 'not preserved' so we'll only keep what's
            // in the files.
            while (stashGetNextElement(&iter, &elem))
            {
                if (elem)
                {
                    info = (AccountBanInfo*)stashElementGetPointer(elem);
                    info->bPreserved = false;
                }
            }

            // Go through all the '.ban' files. This will gather up all the
            // new authnames to ban and mark any we already knew to be
            // preserved.
            fileScanDirRecurseEx(fullpath, AccountBanProcessor);

            // Remove everything that doesn't have the preserved flag set (ie.
            // anything we had before but which is not in the files we found
            // and has thus been unbanned).
            stashGetIterator(s_bannedAuthTable, &iter);

            while (stashGetNextElement(&iter, &elem))
            {
                if (elem)
                {
                    info = (AccountBanInfo*)stashElementGetPointer(elem);

                    if (!(info->bPreserved))
                    {
                        stashRemovePointer(s_bannedAuthTable, stashElementGetStringKey(elem), NULL);
                        MP_FREE(AccountBanInfo, info);
                    }
                }
            }
        }
    }
    else
    {
        FolderCacheQuery(fcAccountBanSettings, ""); // Just to get Update
    }
}

/**********************************************************************func*
 * 
 *
 */
static void s_ExistenceQuery(BOOL force)
{
    static U32 uLastUpdate = 0;
    U32 uNow = timerSecondsSince2000()+g_CharacterInfoSettings.iSecsDelta;

    if(!existinfo_from_name)
        loadExistInfoCache();

    if(!g_CharacterInfoSettings.bDeleteMissing)
        return;

    if(force || uNow > uLastUpdate+60*60*3) // update every 3 hours
    {
        char *pchAccessLevel = s_AccessLevelQualifier;
        char *achWhere = 0;
        char achDate[64];
        char *pchName;
        printf("Updating character existence list...\n");
        filelog_printf(g_CharacterInfoSettings.achLogFileName,
                       "Updating character existence list\n");
        
        // Clear out all the dbids
        stashForEachElement(existinfo_from_name, ClearDbidProc);
        
        // Get a list of all characters who have been online in the last
        //   60 days.
        // This has a side effect of setting the dbids to non-zero if
        //   they exist.
        uLastUpdate = uNow - (60*60*24* g_CharacterInfoSettings.iInactiveDeleteTimeDays);
        timerMakeDateStringFromSecondsSince2000(achDate, uLastUpdate);
        
        if (g_CharacterInfoSettings.bIncludeAccessLevelChars)
            pchAccessLevel = s_NoAccessLevelQualifier;
        
        estrPrintf(&achWhere, "WHERE %sEnts.Level>=%d AND Ents.LastActive>'%s'", pchAccessLevel, g_CharacterInfoSettings.iMinLevel, achDate);
        if(!g_CharacterInfoSettings.bShowBanned)
            estrConcatf(&achWhere, " AND isnull(banned,0)=0");
        
        dbReqCustomData(CONTAINER_ENTS, "Ents", "", achWhere, "ContainerId, Name, LastActive", ExistsInDBProc, 0);
        dbMessageScanUntil("DBCLIENT_REQ_CUSTOM_DATA", NULL);
        printf("\b");
        
        estrDestroy(&achWhere);
        
        pchName = strdupa(_tempnam(g_CharacterInfoSettings.achWebDBPath, g_CharacterInfoSettings.achShardName));
        s_fpDeleteLog = fopen(pchName, "w");
        fprintf(s_fpDeleteLog,INDEX_FILE_COMMENT);
        
        // Any names with a dbid of 0 got deleted
        stashForEachElement(existinfo_from_name, RemoveDeletedProc);
        
        CheckAccountBanProc();
        
        if(s_fpDeleteLog)
        {
            AddBannedCharactersToDeleteLog();
            
            fprintf(s_fpDeleteLog, "# End\n");
            fclose(s_fpDeleteLog);
            s_fpDeleteLog = NULL;
        }
        
        uLastUpdate = uNow;
        printf("  Done.\n");
    }
}


/**********************************************************************func*
 * CheckExistsInfo
 *
 */
bool CheckGeneratedExistsInfo(int dbid, U32 lastactive)
{
    bool retval;
    StashTableIterator iter;
    StashElement elem;
    ExistInfo *pei;

    // if we're ignoring the timestamp 
    if(g_CharacterInfoSettings.RefreshMode == RefreshModeType_Always)
        return FALSE;

    retval = false;

    stashGetIterator(existinfo_from_name, &iter);

    while (!retval && stashGetNextElement(&iter, &elem))
    {
        pei = (ExistInfo *)stashElementGetPointer(elem);

        if (pei && pei->dbid == dbid && pei->bGenerated 
            && pei->uLastActive == lastactive)
        {
            retval = true;
        }
    }

    return retval;
}

static ExistInfo *ExistInfoFromDbid(const char **hres_name, int dbid)
{
    StashTableIterator hashIter;
    StashElement elem;
    stashGetIterator(existinfo_from_name, &hashIter);
    while (stashGetNextElement(&hashIter, &elem))
    {
        ExistInfo *pei = stashElementGetPointer(elem);
        if (pei->dbid == dbid)
        {
            if(hres_name)
                *hres_name = stashElementGetStringKey(elem);
            return pei;
        }
    }
    return NULL;
}

/**********************************************************************func*
 * UpdateExistsInfo
 *
 */
char *UpdateExistsInfo(const char *pchSafeName, int dbid, U32 uLastActive)
{
    static char *res = NULL;
    StashElement elem;
    ExistInfo *pei = NULL;
    int j;

    stashFindElement(existinfo_from_name, pchSafeName, &elem);
    if(!elem)
    {
        char *old_name = NULL;
        pei = ExistInfoFromDbid(&old_name,dbid);      // grab old name, if any
        if(!pei)
        {
            pei = StructAllocRaw(sizeof(ExistInfo)); // set to zero
            pei->bMarkedOnline = true;
            pei->name = StructAllocString(pchSafeName);
            for (j = 0; j < ARRAY_SIZE(pei->iCostumeCRC) ; j++)
                pei->iCostumeCRC[j] = 0;
        }
        stashAddPointer(existinfo_from_name, pchSafeName, pei, false);
        if(old_name)
        {
            ExistInfo *ei = 0;
            estrPrintCharString(&res,old_name);
            stashRemovePointer(existinfo_from_name,res,&ei);
            assert(ei == pei);
        }
    }
    else
    {
        pei = (ExistInfo *)stashElementGetPointer(elem);
    }

    if(pei)
    {
        pei->dbid = dbid;
        pei->uLastActive = uLastActive;

#ifdef TRACK_ACTIVE_STATUS
        MarkOnlineState(pchSafeName, pei);
#endif
    }
    return res;
}


/**********************************************************************func*
 * SetExistsInfoGenerated
 *
 */
void SetExistsInfoGenerated(const char *pchSafeName, int dbid)
{
    StashElement elem;
    ExistInfo *pei = NULL;

    if (pchSafeName)
    {
        if (stashFindElement(existinfo_from_name, pchSafeName, &elem) && elem)
        {
            pei = (ExistInfo *)stashElementGetPointer(elem);

            if (pei && pei->dbid == dbid)
            {
                pei->bGenerated = true;
            }
        }
    }
}
/**********************************************************************func*
 * ResetCostumeCRCs
 *
 */
bool ResetCostumeCRCs(int dbid)
{
    StashElement elem;
    ExistInfo *pei = NULL;
    StashTableIterator iter;
    bool found = false;
    int count;

    stashGetIterator(existinfo_from_name, &iter);

    while (!found && stashGetNextElement(&iter, &elem))
    {
        pei = (ExistInfo *)stashElementGetPointer(elem);

        if (pei && pei->dbid == dbid)
        {
            found = true;
        }
    }

    if (found)
    {
        for (count = 0; count < MAX_COSTUMES; count++)
        {
            pei->iCostumeCRC[count] = 0;
        }
    }

    return found;
}

/**********************************************************************func*
 * CheckCostumeChange
 *
 */
bool CheckCostumeChange(char *pchSafeName, Costume *pCostume, int costumeNum)
{
    StashElement elem;
    ExistInfo *pei = NULL;

    int iCostumeCRC;

    if (!pCostume || !INRANGE0(costumeNum,ARRAY_SIZE(pei->iCostumeCRC)))
        return false;

    iCostumeCRC = costume_getCRC(pCostume);

    stashFindElement(existinfo_from_name, pchSafeName, &elem);
    if(!elem)
        pei = 0;
    else
        pei = (ExistInfo *)stashElementGetPointer(elem);
    
    if (!pei)
    {
        pei = StructAllocRaw(sizeof(ExistInfo)); // set to zero
        pei->iCostumeCRC[costumeNum] = iCostumeCRC;
        pei->name = StructAllocString(pchSafeName);
        if (elem)
            stashElementSetPointer(elem, pei);
        else
            stashAddPointer(existinfo_from_name, pchSafeName, pei, false);

        return true;
    }

    if (pei->iCostumeCRC[costumeNum] != iCostumeCRC)
    {
        pei->iCostumeCRC[costumeNum] = iCostumeCRC;
        return true;
    }

    return false;
}

#ifdef TRACK_ACTIVE_STATUS
/**********************************************************************func*
 * UpdateActiveProc
 *
 */
static int UpdateActiveProc(StashElement elem)
{
    ExistInfo *pei = (ExistInfo *)stashElementGetPointer(elem);
    char *pch = stashElementGetStringKey(elem);

    if(pei->bMarkedOnline == true
        && pei->uLastActive < s_uNowActive-60*g_CharacterInfoSettings.iIntervalMinutes*2)
    {
        MarkOnlineState(pch, pei);
    }

    return 0;
}
#endif

#ifdef TRACK_ACTIVE_STATUS
/**********************************************************************func*
 * MarkOnlineState
 *
 */
static void MarkOnlineState(char *pchSafeName, ExistInfo *pei)
{
    char achSrc[MAX_PATH];
    char achDest[MAX_PATH];
    char *pchImg = NULL;
    U32 uNow = timerSecondsSince2000()+g_CharacterInfoSettings.iSecsDelta;

    if(pei->uLastActive > uNow-60*g_CharacterInfoSettings.iIntervalMinutes*2)
    {
        if(!pei->bMarkedOnline)
        {
            pchImg = "online";
            pei->bMarkedOnline = true;
        }
    }
    else
    {
        if(pei->bMarkedOnline)
        {
            pchImg = "offline";
            pei->bMarkedOnline = false;
        }
    }

    if(pchImg)
    {
        sprintf(achSrc, "%s/%s.gif", g_CharacterInfoSettings.achImagePath, pchImg);
        strcpy(achDest, MakePictureFilename(pchSafeName, "_status.gif"));
        fileCopy(achSrc, achDest);
    }
}
#endif

/**********************************************************************func*
 * CharacterActiveMonitor
 *
 */
static void CharacterActiveMonitor(void)
{
#ifdef TRACK_ACTIVE_STATUS
    static U32 uLastUpdate = 0;
    s_uNowActive = timerSecondsSince2000()+g_CharacterInfoSettings.iSecsDelta;

    if(existinfo_from_name && s_uNowActive > uLastUpdate+60*g_CharacterInfoSettings.iIntervalMinutes)
    {
        printf("Updating character online status...\n");

        stashForEachElement(existinfo_from_name, UpdateActiveProc);

        uLastUpdate = s_uNowActive;
        printf("  Done.\n");
    }
#endif
}

/**********************************************************************func*
 * MakeCharacterURL
 *
 */
char *MakeCharacterURL(const char *pchSafeName, const char *pchAppend)
{
    return imageserver_MakeSubdirFilename(g_CharacterInfoSettings.achCharacterWebRoot, pchSafeName, pchAppend, g_CharacterInfoSettings.bAlphaSubdir ? pchSafeName[0] : 0);
}

/**********************************************************************func*
 * MakePictureURL
 *
 */
char *MakePictureURL(const char *pchSafeName, const char *pchAppend)
{
    return imageserver_MakeSubdirFilename(g_CharacterInfoSettings.achCharacterImageWebRoot, pchSafeName, pchAppend, g_CharacterInfoSettings.bAlphaSubdir ? pchSafeName[0] : 0);
}

/**********************************************************************func*
 * MakeCharacterFilename
 *
 */
char *MakeCharacterFilename(const char *pchSafeName, const char *pchAppend)
{
    return imageserver_MakeSubdirFilename(g_CharacterInfoSettings.achHTMLDestPath, pchSafeName, pchAppend, g_CharacterInfoSettings.bAlphaSubdir ? pchSafeName[0] : 0);
}

/**********************************************************************func*
 * MakePictureFilename
 *
 */
char *MakePictureFilename(const char *pchSafeName, const char *pchAppend)
{
    return imageserver_MakeSubdirFilename(g_CharacterInfoSettings.achCharacterImageDestPath, pchSafeName, pchAppend, g_CharacterInfoSettings.bAlphaSubdir ? pchSafeName[0] : 0);
}

/**********************************************************************func*
 * ConvertFilenameToLowercase
 *
 */
void ConvertFilenameToLowercase(char *filename)
{
    int count;

    devassert(filename != NULL);

    if (filename)
    {
        for (count = 0; filename[count] != '\0'; count++)
        {
            if (filename[count] >= 'A' && filename[count] <= 'Z')
            {
                filename[count] = _tolower(filename[count]);
            }
        }
    }
}

/* End of File */
