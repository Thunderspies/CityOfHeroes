// checksumgen.cpp : Defines the entry point for the console application.
//

#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/wininclude.h>
#include <assert.h>
#include <utilitieslib/utils/piglib.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <utilitieslib/utils/file.h>
#include <utilitieslib/components/genericlist.h>
#include <utilitieslib/assert/assert.h>
#include <utilitieslib/utils/timing.h>
#include <direct.h>
#include <sys/types.h>
#include <sys/utime.h>
#include <utilitieslib/components/SharedMemory.h>
#include <utilitieslib/utils/fileutil.h>
#include <utilitieslib/network/bindiff.h>
#include <zlib/zlib.h>
#include <utilitieslib/network/crypt.h>
#include <utilitieslib/utils/mathutil.h>
#include "filechecksum.h"
#include "patchfileutils.h"
#include <utilitieslib/utils/log.h>
#include <utilitieslib/utils/sysutil.h>

static char gDirToScan[512];
static char gOutputPath[512];
static char gBuildName[512];
int g_server_patch = 0;

static void Usage()
{
    printf("checksumgen: generates checksum file listing size and checksums for all files under a directory.\n");
    printf("checksumgen.exe -o <path> -b <buildName> <DirectoryToScan>\n");
    printf("\t-o <path>: output checksum file path\n");
    printf("\t-b <buildName>: build name, eg \"1950.201102120907.1\"\n");
    printf("\t-nogui: disable all gui\n");
}

int main(int argc,char **argv)
{
    int        i,run_server=1,patch_to_latest=0,rebuild_if_bad=0;
    extern    CRITICAL_SECTION PigCritSec;
    int        errCode = 0;
    char    *execname;

    EXCEPTION_HANDLER_BEGIN
    setAssertMode(ASSERTMODE_DEBUGBUTTONS | ASSERTMODE_FULLDUMP | ASSERTMODE_DATEDMINIDUMPS | ASSERTMODE_ZIPPED);
    
    // print args
    execname = getExecutableName();
    printf(execname);
    for(i=1;i<argc;i++)
        printf(" %s", argv[i]);
    printf("\n");

    // parse args
    for(i=1;i<argc-1;i++)
    {
        if (stricmp(argv[i],"-nogui")==0) {
            setGuiDisable(true);
            setAssertMode(ASSERTMODE_STDERR | ASSERTMODE_EXIT);
        }

        if (stricmp(argv[i],"-o")==0) {
            strcpy(gOutputPath, argv[++i]);    
        }
        if (stricmp(argv[i],"-b")==0) {
            strcpy(gBuildName, argv[++i]);    
        }
    }
    // dir to scan is last arg
    if(i == (argc-1)) {
        strcpy(gDirToScan, argv[i]);
    }

    if( !strlen(gDirToScan) ) {
        printf("Error: No directory param passed in.\n");
        Usage();
        errCode = -1;
    } else if( !strlen(gOutputPath) ) {
        printf("Error: No checksum file path param passed in.\n");
        Usage();
        errCode = -1;
    } else if( !strlen(gBuildName) ) {
        printf("Error: No buildName param passed in.\n");
        Usage();
        errCode = -1;
    }

    if(errCode == 0) {
        printf("Initializing...");
        fileDisableAutoDataDir();
        InitializeCriticalSection(&PigCritSec);
        sharedMemorySetMode(SMM_DISABLED);
        cryptInit(); // takes a few secs to init
        printf("done\n");

        // generate and write checksum file
        printf("Scanning directory: \"%s\"\n", gDirToScan);
        checksumImage(gDirToScan, gOutputPath, gBuildName);
        printf("Done\n");
    }

    EXCEPTION_HANDLER_END 
    exit(errCode);
}

