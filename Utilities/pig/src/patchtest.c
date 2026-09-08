#include "patchtest.h"
#include "utilitieslib/utils/wininclude.h"
#include "utilitieslib/utils/piglib.h"
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utilitieslib/utils/file.h"
#include "utilitieslib/utils/utils.h"
#include "utilitieslib/components/genericlist.h"

#include "utilitieslib/utils/timing.h"
#include <direct.h>
#include <sys/types.h>
#include <sys/utime.h>
#include "utilitieslib/components/SharedMemory.h"
#include "utilitieslib/network/crypt.h"
#include "zlib/zlib.h"
#include "utilitieslib/utils/winfiletime.h"
#include "utilitieslib/utils/hoglib.h"
#include "utilitieslib/utils/MemoryMonitor.h"
#include "utilitieslib/components/StashTable.h"
#include "patchtest.h"
#include "utilitieslib/components/earray.h"
#include "utilitieslib/components/MemoryPool.h"
//#include "fileutil2.h"

#if 0
int delete_it=0;
char *hog_file_name = "C:/pigtest/patchtest.hogg";

typedef struct PatchTestVersion {
    NewPigEntry **files;
    char *filename;
} PatchTestVersion;

static PatchTestVersion **versions;
MP_DEFINE(NewPigEntry);

void destroyNewPigEntry(void* entryData)
{
    NewPigEntry * entry = (NewPigEntry *)entryData;
    SAFE_FREE(entry->fname);
    MP_FREE(NewPigEntry, entry);
}

NewPigEntry **parseFile(const char *fname)
{
    int len, count;
    char *mem = fileAlloc(fname, &len);
    char *args[100];
    char *s;
    NewPigEntry **ret=NULL;

    MP_CREATE(NewPigEntry, 2048);
    s = mem;
    while(s && (count=tokenize_line_safe(s,args,ARRAY_SIZE(args),&s))>=0)
    {
        NewPigEntry *entry;
        char filename[MAX_PATH];
        int i;
        int ret2;
        if (count == 0 || args[0][0]=='#')
            continue;
        assert(count >= 9);

        entry = MP_ALLOC(NewPigEntry);
        ret2 = sscanf(args[0], "%08x%08x%08x%08x", &entry->checksum[0], &entry->checksum[1], &entry->checksum[2], &entry->checksum[3]);
        assert(ret2==4);
        ret2 = sscanf(args[1], "%d/", &entry->pack_size);
        assert(ret2==1);
        ret2 = sscanf(args[2], "%d", &entry->size);
        assert(ret2==1);
        //sprintf(timestamp, "%s %s %s %s %s", args[3], args[4], args[5], args[6], args[7]);
        filename[0]=0;
        for (i=8; i<count; i++) {
            strcat(filename, args[i]);
            if (i!=count-1)
                strcat(filename, " ");
        }
        entry->fname = strdup(filename);        
        eaPush(&ret, entry);
    }
    free(mem);
    return ret;
}

static int cmpVersions(const void* aData, const void* bData)
{
    const PatchTestVersion ** a = (const PatchTestVersion **)aData;
    const PatchTestVersion ** b = (const PatchTestVersion **)bData;
    unsigned char *fna = (*a)->filename + strlen("C:/lists/");
    unsigned char *fnb = (*b)->filename + strlen("C:/lists/");
    while (fna && fnb && isdigit(fna[0]) && isdigit(fnb[0]))
    {
        int vera, verb;
        sscanf(fna, "%d", &vera);
        sscanf(fnb, "%d", &verb);
        if (vera!=verb) {
            return vera - verb;
        }
        fna = strchr(fna, '.');
        if (fna)
            fna++;
        fnb = strchr(fnb, '.');
        if (fnb)
            fnb++;
    }
    return stricmp((*a)->filename, (*b)->filename);
}

void loadAllChecksums()
{
    int count;
    int i;
    char **file_names = fileScanDir("C:/lists", &count);
    for (i=0; i<count; i++)
    {
        PatchTestVersion *version = calloc(sizeof(PatchTestVersion), 1);
        version->filename = file_names[i];
        version->files = NULL; //parseFile(file_names[i]);
        eaPush(&versions, version);
    }
    eaQSort(versions, cmpVersions);
}

extern int updateHogFile(NewPigEntry *entries, int count, const char *output);

void patchTo(NewPigEntry **file_list)
{
    int count = eaSize(&file_list);
    int i;
    NewPigEntry *entries = calloc(sizeof(NewPigEntry), count);
    for (i=0; i<count; i++) {
        entries[i] = *file_list[i];
        entries[i].data = (void*)1;
    }
    if (fileExists(hog_file_name) && !delete_it) {
        // Update!
        updateHogFile(entries, count, hog_file_name);
    } else {
        // Create
        pigCreateFromMem(entries, count, hog_file_name, HOG_VERSION);
    }
    SAFE_FREE(entries);
}

static void doit(int i)
{
    loadstart_printf("Loading file list...");
    versions[i]->files = parseFile(versions[i]->filename);
    loadend_printf("done.");
    patchTo(versions[i]->files);
    loadstart_printf("Freeing PigEntries...");
    eaDestroyEx(&versions[i]->files, destroyNewPigEntry);
    loadend_printf("done.");
}

F32 getFragmentation()
{
    F32 ret;
    HogFile *hog_file;
    loadstart_printf("Re-loading hogg file...");
    hog_file = hogFileRead(hog_file_name, NULL, PIGERR_ASSERT, NULL, HOG_DEFAULT);
    if (!hog_file)
    {
        loadend_printf("done.");
        assert(0); // Need better error recovery code!
        return 0;
    }
    loadend_printf("done.");
    ret = hogFileCalcFragmentation(hog_file)*100;
    loadstart_printf("Unloading hogg file...");
    hogFileDestroy(hog_file);
    SAFE_FREE(hog_file);
    loadend_printf("done.");
    return ret;
}

int doPatchTest(void)
{
    extern int hog_mode_no_data;
    extern int g_inside_pool_malloc;
    F32 *fragmentations=NULL;
    int i;
    int count, start;
    hog_mode_no_data = 1;
    g_inside_pool_malloc = 1; // Disable this, as it makes things look like they leak and doesn't deal with this kind of data well
    loadstart_printf("Loading checksums...");
    loadAllChecksums();
    loadend_printf("done (%d versions).", eaSize(&versions));

    count = eaSize(&versions);
    start=0;
    //count=31;
    for (i=start; i<count; i++)
    {
        F32 frag;
        printf("%d/%d: %s\n", i+1, count, versions[i]->filename + strlen("c:/lists/"));
        delete_it = i==0;
        doit(i);
        frag = getFragmentation();
        printf("\nAfter patching to %s; fragmentation: %1.1f\n\n", versions[i]->filename + strlen("c:/lists/"), frag);
        eafPush(&fragmentations, frag);
    }

    printf("\nFragmentation Report:\n");
    for (i=start; i<count; i++)
    {
        printf("%15s: %1.1f\n", versions[i]->filename, fragmentations[i]);
    }

    i = _getch();
    return 0;
}

#else
int doPatchTest(void)
{
    printf("doPatchTest disabled\n");
    return 0;
}
#endif