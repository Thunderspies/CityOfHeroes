/***************************************************************************
 *     Copyright (c) 2011, Paragon Studios
 *     All Rights Reserved
 *     Confidential Property of Paragon Studios
 */
#include "game.h"
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/SuperAssert.h>
#include "clienterror.h"
#include <utilitieslib/utils/FolderCache.h>
#include "clientcomm/autoResumeInfo.h"
#include "cmdparse/cmdgame.h"
#include <utilitieslib/utils/textparser.h>
#include "UI/uiAutomap.h"
#include <utilitieslib/utils/file.h>
#include <utilitieslib/version/AppRegCache.h>
#include <utilitieslib/utils/RegistryReader.h>
#include <utilitieslib/utils/piglib.h>
#include <utilitieslib/utils/sysutil.h>
#include <utilitieslib/utils/genericDialog.h>
#include <utilitieslib/network/crypt.h>
#include <utilitieslib/utils/StringUtil.h>
#include <utilitieslib/utils/RegistryReader.h>
#include <utilitieslib/AppVersionDefines.h> // CHECKSUMFILE_VERSION
#include <sys/stat.h>
#include <utilitieslib/utils/winfiletime.h>
#include "win/win_init.h"
#include <utilitieslib/language/AppLocale.h>
#include <utilitieslib/utils/osdependent.h>

extern void *extractFromFS(const char *name, U32 *count);
extern HWND hlogo;

typedef struct
{
    const char    *filePath;
    int            len;
    U32            values[4];
} CheckFile;

#define MAX_FILE_COUNT    256
typedef struct
{
    const char    *buildVersion;
    int            numFiles;
    CheckFile    files[MAX_FILE_COUNT];
} CheckFiles;

static S64 s_total_len, s_current_len;
static int s_current_file, s_total_file;
static char* s_chunk_mem;


// This code taken mostly from CohUpdater.  Structs simplified for our needs.
static CheckFiles* ParseCheckSumData(char * data)
{
    int            i, count;
    bool        bSuccess = false;
    CheckFiles    *cf;
    char        *s,*args[100];
    CheckFile    *file = NULL;

    cf = (CheckFiles*)calloc(sizeof(CheckFiles), 1);
    s = data;

    while(count = tokenize_line(s,args,&s))
    {
        if (stricmp(args[0],"Version")==0)
        {
            int version = atoi(args[1]);
            if (version != CHECKSUMFILE_VERSION)
            {
                printf("Failed to parse checksum file: got version %d, expected version %d\n", version, CHECKSUMFILE_VERSION);
                goto exit;
            }
        }
        else if (stricmp(args[0],"Build")==0)
        {
            cf->buildVersion = args[1];
        }
        else if (stricmp(args[0],"File")==0)
        {
            file = &cf->files[cf->numFiles++];
            assert(cf->numFiles < MAX_FILE_COUNT);
            file->filePath = args[1];
        }
        else if (stricmp(args[0],"Time")==0)
        {
            // we ignore time since launcher changes file mod times
        }
        else if (stricmp(args[0],"Full")==0)
        {
            file->len = strtoul(args[1],0,10);
            for(i=0;i<count-2;i++)
                file->values[i] = strtoul(args[i+2],0,16);
        }
    }

    bSuccess = true;

exit:
    if(!bSuccess) {
        free(cf);
        cf = NULL;
    }
    return cf;
}

static int getFileSize(const char* path)
{
    FILE * file = fopen(path,"rb");
    int fsize;
    if (!file) return 0;
    fseek(file, 0, SEEK_END);
    fsize = ftell(file);
    fclose(file);

    // or better to use: struct _stat32* statInfo;  _stat32(path, statInfo);  ??

    return fsize;
}

static bool PromptUserBeforeFullScan(bool bFirstPrompt)
{
    int ok = 0;
    char * str;
    int locale = locGetIDInRegistry();

    if(IsUsingCider()) {
        // disable full verify on mac since progress dialog doesn't work
        return false;
    }

    // First time thru, user gets an ok/cancel prompt
    if(bFirstPrompt) {
        if(locale == LOCALE_ID_GERMAN)
            str = "Ein neuer Patch oder Reparatur wurde festgestellt. City of Heroes wird nun die Dateien berprfen.";
        else if(locale == LOCALE_ID_FRENCH)
            str = "Nouvelle version ou Rparation dtecte. Vrification des fichiers City of Heroes amorce.";
        else
            str = "New Patch or Repair has been detected. City of Heroes will now verify the files.";
        ok = winMsgOkCancelParented(hlogo, str);
    }

    // If they cancel that prompt, or this is not the first prompt, let the know the consequences of skipping the verify step
    //    and give them a chance to reconsider.
    if(!ok) {
        if(locale == LOCALE_ID_GERMAN)
            str = "Mchten Sie die berprfung abbrechen? Dies knnte aufgrund von beschdigten Dateien weitere Spielabstrze veranlassen." 
                    " Wenn Sie Ja whlen, und City of Heroes nicht mehr laufen knnen, men Sie die Installation wieder Reparieren."
                    " Dies wird die Dateien neu berprfen, um jegliche beschdigte Dateien zu reparieren.";
        else if(locale == LOCALE_ID_FRENCH)
            str = "Souhaitez-vous annuler la vrification des fichiers ? Cela pourrait augmenter le risque de plantage d  la corruption de certains fichiers."
                    " Si vous cliquez sur Oui et n'arrivez ensuite plus  lancer City of Heroes, il vous faudra Rparer cette installation."
                    " Cela aura pour effet de vrifier les donnes du jeu et de rparer tout fichier corrompu.";
        else
            str =    "Would you like to cancel the file verification? Doing so puts your game at risk of random crashes due to corrupted files."
                    " If you click Yes and are no longer able to run City of Heroes you will need to Repair this installation,"
                    " which will re-verify the game data and fix any corrupted files you may have.";

        if(!winMsgYesNoParented(hlogo, str) )
        {
            // user chose no, which means they changed their mind and do NOT want to cancel
            ok = 1;
        }
    }
    return (ok != 0);
}

#define CHUNK_SIZE (1<<20)
#define BUFLEN 128
static bool CalculateChecksum(const char* path, int len, U32 values[4], int timer, bool *pCancelled)
{
    FILE * file;
    int i, chunk_size, wbufLen;
    wchar_t wbuf1[BUFLEN], wbuf2[BUFLEN];
    char    buf1[BUFLEN];
    int        locale = locGetIDInRegistry();

    file = fopen(path,"rb");
    if (!file) return false;

    for(i=0; i<len; i+=CHUNK_SIZE)
    {
        chunk_size = MIN(CHUNK_SIZE, len - i);
        if (fread(s_chunk_mem,1,chunk_size, file) != (int)chunk_size)
            return false;
        cryptMD5Update(s_chunk_mem, chunk_size);
        s_current_len += chunk_size;

        if (timerElapsed(timer) > 0.1f) {
            // update progress
            if(locale == LOCALE_ID_GERMAN)
                sprintf(buf1, "Datei %d / %d wird verarbeitet", s_current_file, s_total_file);
            else if(locale == LOCALE_ID_FRENCH)
                sprintf(buf1, "Traitement du fichier %d / %d", s_current_file, s_total_file);
            else
                sprintf(buf1, "Processing file %d / %d", s_current_file, s_total_file);

            wbufLen = UTF8ToWideStrConvert(buf1, wbuf1, ARRAY_SIZE(wbuf1)-1); wbuf1[wbufLen] = '\0';
            wbufLen = UTF8ToWideStrConvert(path, wbuf2, ARRAY_SIZE(wbuf2)-1); wbuf2[wbufLen] = '\0';
    
            if(updateProgressDialog(s_current_len/1024, s_total_len/1024, wbuf1, wbuf2)) {
                stopProgressDialog(); // have to kill it so that our yes/no dialog is on top
                assert(hlogo != NULL);
                *pCancelled = !PromptUserBeforeFullScan(false);
                if(*pCancelled) {
                    break; // dont treat this as an error here, just abort processing this file
                } else {
                    // resume progress dialog, will pick up where we left off
                    wchar_t *verifyingFiles;
                    if(locale == LOCALE_ID_GERMAN)
                        verifyingFiles = L"Dateien werden berprft";
                    else if(locale == LOCALE_ID_FRENCH)
                        verifyingFiles = L"Vrification des fichiers";
                    else
                        verifyingFiles = L"Verifying Files";
                    startProgressDialog(hlogo, verifyingFiles, L"Cancel Requested");
                    updateProgressDialog(s_current_len/1024, s_total_len/1024, wbuf1, wbuf2);
                }
            }

            timerStart(timer);
        }
    }
    fclose(file);
    cryptMD5Final(values);

    return true;
}

// This function is needed if we launch from NcLauncher because Launcher does 
//    not validate the installation before running.  COHUpdater does a validate so 
//    we don't do another here in that case.
bool game_validateChecksums(bool bForceFullVerify, bool *pCancelled)
{
    char        *data = NULL;
    const char    *checksumFileName = "client.chksum";
    U32            len;
    int            i, crashedLastTime = 0, timer;
    CheckFiles    *checkFiles = NULL;
    bool        bSuccess = true, bCancelled, bFullVerify = false, bDoingFullVerify = false;
    __time32_t    modTime = 0;
    struct _stat32 sbuf;
    RegReader    rr;

    *pCancelled = bCancelled = false;
    if(!game_state.usedLauncher && !bForceFullVerify)
        return true;

#if 0 // enable this block if only want to verify when -verify passed on command line
    if(!bForceFullVerify)
        return true;
#endif

    rr = createRegReader();
    initRegReader(rr, regGetAppKey());

    // tbd, cmdAccessLevel() >= 9 only gets noverify?
    if(game_state.noVerify) {
        printf("Skipping verify step due to -noverify command line option\n");
        bCancelled = true;
        goto exit;
    }

    printf("Validating files against checksum file: \"%s\"\n", checksumFileName);

    // read the checksum file
    data = extractFromFS(checksumFileName, &len);
    if( !data ) {
        printf("Failed to open \"%s\" file for calculating checksum!\n", checksumFileName);
        bSuccess = false; // set to true if we want to make checksum verification optional
        goto exit;
    }

    // parse to binary representation
    checkFiles = ParseCheckSumData(data);
    if(!checkFiles) {
        bSuccess = false;
        goto exit; // already printed error to console
    }
    
    // We force a full verify if the client previously crashed
    if( rrReadInt(rr,  "VerifyOnNextUpdate", &crashedLastTime) ) {
        bFullVerify = true;
        rrDelete(rr, "VerifyOnNextUpdate");
    }

    // Do quick verify by size first  (TBD, add LWC functionality here eventually to only check files as they are downloaded?)
    s_total_len = 0;
    for(i=0; i<checkFiles->numFiles; i++) {
        CheckFile* ckfile = &checkFiles->files[i];
        len = getFileSize(ckfile->filePath);
        if(len != ckfile->len) {
            printf("Validation failed due to size mismatch for file \"%s\":  expected %d, got %d\n", ckfile->filePath, ckfile->len, len);
            bSuccess = false;
            goto exit;
        }
        s_total_len += len;
    }

    if(_AltStat(checksumFileName, &sbuf) != -1) { // shouldnt fail since already read file so it exists
        __time32_t lastModTime = 0;
        modTime = sbuf.st_mtime;
        if( !rrReadInt(rr,  "ChecksumFileTime", &lastModTime) )
            lastModTime = 0;
        bFullVerify = bFullVerify || !fileDatesEqual(lastModTime, modTime, true);
    }

    // Any reason to continue with full verify step?
    bDoingFullVerify = bSuccess && (bFullVerify || bForceFullVerify);
    if(!bDoingFullVerify) goto exit; 

    // Let the user know what we're doing, and give them a chance to bail.
    assert(hlogo != NULL); // we rely on splash screen to be up (used as parent for dialogs, otherwise get dialogs coming up behind it)
    bCancelled = !PromptUserBeforeFullScan(true);
    if(bCancelled) goto exit; 

    // For full verify, calc checksum for each file and compare to expected value
    if(bFullVerify && !bForceFullVerify)
        rrWriteInt(rr, "ChecksumFileTime", 0); // clear any previous mod time so that we will always do full verify on launch until successful
    
    {
        wchar_t *verifyingFiles;
        int locale = locGetIDInRegistry();
        if(locale == LOCALE_ID_GERMAN)
            verifyingFiles = L"Dateien werden berprft";
        else if(locale == LOCALE_ID_FRENCH)
            verifyingFiles = L"Vrification des fichiers";
        else
            verifyingFiles = L"Verifying Files";
        if( !startProgressDialog(hlogo, verifyingFiles, L"Cancel Requested") ) {
            printf("Failed to initialize progress dialog, aborting full verify\n");
            bCancelled = true;
            goto exit;
        }
    }

    timer = timerAlloc();
    cryptMD5Init();
    s_current_len = 0;
    s_current_file = 0;
    s_total_file = checkFiles->numFiles;
    s_chunk_mem = calloc(1, CHUNK_SIZE);

    timerStart(timer);
    updateProgressDialog(0, s_total_len/1024, NULL, NULL);
    for(i=0; i<checkFiles->numFiles && !bCancelled; i++) {
        CheckFile* ckfile = &checkFiles->files[i];
        U32 cksum[4];

        s_current_file += 1;
        if(!CalculateChecksum(ckfile->filePath, ckfile->len, cksum, timer, &bCancelled) ) {
            printf("Validation failed due to checksum calculation failure for file \"%s\"\n", ckfile->filePath);
            bSuccess = false;
            break;
        }
        
        if(!bCancelled && memcmp(cksum, ckfile->values, sizeof(cksum)) != 0) {
            printf("Validation failed due to checksum mis-match for file \"%s\"\n", ckfile->filePath);
            bSuccess = false;
            break;
        }
    }
    stopProgressDialog();

    timerFree(timer);

exit:
    printf("%s validation %s\n", bDoingFullVerify ? "Full":"Fast", bCancelled ? "cancelled." : (bSuccess ? "succeeded." : "FAILED!"));

    // Cancel is treated as success as far as modTime is concerned, ie we trust the user to know that verification is
    //    not required (that is, he has been warned of the consequences and is on his own).
    if(bSuccess || bCancelled) {
        // write last successful checksum file mod time to registry so we know when we need to do a full verify again
        rrWriteInt(rr, "ChecksumFileTime", (int)modTime);
    }
    
    if( (!bSuccess || bCancelled) && !bForceFullVerify ) {
        // Corrupt piggs (or user cancelled).  Write current version to registry under "LastPiggCorruption"
        //    field  (64 chars max).  This is used to gather stats from system specs.
        const char * version = "FailedVersion"; // generic for when fail to even parse the checksum file to get version
        char buf[64];
        if(checkFiles) version = checkFiles->buildVersion; // trust checksum file to have correct version
        sprintf(buf, "%s%s", version, bCancelled ? " [SKIPPED]": "" );
        rrWriteString(rr, "LastPiggCorruption", buf);
    }

    // free data
    if(s_chunk_mem) { free(s_chunk_mem); s_chunk_mem = NULL; }
    if(checkFiles) free(checkFiles);
    if(data) free(data);
    destroyRegReader(rr);

    *pCancelled = bCancelled;
    return bSuccess && !bCancelled;
}

