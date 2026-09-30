#include "characterTransfer.h"
#include <stdio.h>
#include <utilitieslib/utils/FileSystem.h>
#include <utilitieslib/utils/wininclude.h>
#include <utilitieslib/utils/fileutil.h>
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/timing.h>


extern CharacterTransfer characterTransfer = { "C:\\characterTransfer", "mapserver.exe", "mapserver.exe" };

static int copyACharacter(const char *source, const char *dest, int sourceContainerId, const char *charname)
{
    char cmdline[1024];
    int ret;
    printf("Copying char #%d%s%s%s from %s to %s...\n", sourceContainerId, (charname&&charname[0])?" (\"":"", (charname&&charname[0])?charname:"", (charname&&charname[0])?"\")":"", source, dest);
    printf("\tGrabbing character... ");
    sprintf(cmdline, "%s -db %s -getcharacter %d > TEMP.TXT", characterTransfer.getMapserver, source, sourceContainerId);
    ret = system(cmdline);
    if (ret) {
        printf("Failed to get.\n");
        return 1;
    }
    printf("Putting character... ");
    sprintf(cmdline, "%s -db %s -putcharacter TEMP.TXT > RESULT.TXT", characterTransfer.putMapserver, dest);
    ret = system(cmdline);
    if (ret) {
        printf("Failed to put.\n");
        return 2;
    }
    printf("Succeeded\n");
    return 0;
}


static void characterTransferCallback(const char *relPath, int when)
{
    char fullpath[MAX_PATH];
    char *data, *line;
    char source[256], dest[256], charname[256];
    int sourceContainerId;
    int succeeded=0, ret;
    char *args[5];
    int num_args;
#define DELIMS "\t, \r\n"
    if (strchr(relPath+1, '/')) {
        // Don't process things in subdirectories
        return;
    }
    sprintf(fullpath, "%s%s", characterTransfer.rootPath, relPath);
    printf("Processing request file: %s\n", fullpath);
    fileWaitForExclusiveAccess(fullpath);
    data = fileAlloc(fullpath, NULL);
    for (line=data; *line; line++) {
        if (*line==',')
            *line = '\t';
    }
    line = data;
    while (line) {
        num_args = tokenize_line_safe(line, args, ARRAY_SIZE(args), &line);
        if (num_args == 0)
            continue;
        if (num_args < 3) {
            printf("Invalid number of arguments on line: %d\n", num_args);
        } else {
            Strncpyt(source, args[0]);
            if (1!=sscanf(args[1], "%d", &sourceContainerId)) {
                printf("Invalid format, error parsing 2nd token: %s; expected integer\n", args[1]);
            } else {
                Strncpyt(dest, args[2]);
                if (num_args >=4) {
                    Strncpyt(charname, args[3]);
                } else {
                    strcpy(charname, "");
                }
                // Do it!
                ret = copyACharacter(source, dest, sourceContainerId, charname);
                if (ret==0)
                    succeeded = 1;
            }
        }
    }
    free(data);
    {
        // Save Request and intermediate data
        char newname[MAX_PATH];
        char newname2[MAX_PATH];
        char cmdline[1024];
        if (succeeded) {
            sprintf(newname, "%s/processed%s", characterTransfer.rootPath, relPath);
        } else {
            sprintf(newname, "%s/failed%s", characterTransfer.rootPath, relPath);
        }
        mkdirtree(newname);
        backSlashes(fullpath);
        backSlashes(newname);
        sprintf(cmdline, "move /Y %s %s > nul", fullpath, newname);
        system(cmdline);

        // temp.txt
        strcpy(newname2, newname);
        strcat(newname2, ".data.txt");
        sprintf(cmdline, "move /Y TEMP.TXT %s > nul", newname2);
        system(cmdline);

        // result.txt
        strcpy(newname2, newname);
        strcat(newname2, ".result.txt");
        sprintf(cmdline, "move /Y RESULT.TXT %s > nul", newname2);
        system(cmdline);
    }
}

static void characterTransferCallbackFileChanged(const FileChange *change, void *user)
{
	(void)user;
	characterTransferCallback(change->entry.path, change->kind);
}


static FileScanAction characterTransferProcessor(char *dir, struct _finddata32_t* data)
{
    char fullpath[MAX_PATH];
    sprintf(fullpath, "%s/%s", dir, data->name);
    if (simpleMatch("*.csv", fullpath)) {
        char relpath[MAX_PATH];
        sprintf(relpath, "/%s", data->name);
        characterTransferCallback(relpath, 0);
    }
    return FSA_NO_EXPLORE_DIRECTORY;
}

void characterTransferMonitor()
{
    static FileSystem * fcCharacterTransfer = 0;
    char filesToMonitor[1024];
    int timer;

    // Look for pending transfers
    printf("###Processing existing requests...\n");
    fileScanDirRecurseEx(characterTransfer.rootPath, characterTransferProcessor);

    sprintf( filesToMonitor, "*.csv" ); // , characterTransfer.rootPath );
    fcCharacterTransfer = fileSystemCreate();
    fileSystemAddSource(fcCharacterTransfer, characterTransfer.rootPath, 0);
    fileSystemSubscribe(NULL, filesToMonitor, FILE_CHANGE_UPDATE|FILE_CHANGE_SHARED, characterTransferCallbackFileChanged, NULL);
    fileSystemCallbacksEnabled(1);

    printf("###Monitoring for new requests in %s\\%s...\n", characterTransfer.rootPath, filesToMonitor);
    timer = timerAlloc();
    timerStart(timer);
    while (1) {
        if (timerElapsed(timer) > 60) {
            // Look for files that showed up that the folder cache missed
            //  this can happen if the server is hung for a long time
            fileScanDirRecurseEx(characterTransfer.rootPath, characterTransferProcessor);
            timerStart(timer);
        }
        fileSystemDispatch(); // Just to get Update
        Sleep(1);
    }
}
