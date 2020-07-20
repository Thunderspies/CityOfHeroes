#include "texloaddesc.h"
#include <utilitieslib/utils/utils.h>
#include <string.h>
#include <assert.h>
#include <utilitieslib/utils/error.h>
#include <sys/stat.h>
#include <utilitieslib/utils/fileutil.h>
#include <utilitieslib/components/StashTable.h>
#include <io.h>
#include <sys/types.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <share.h>
#include <utilitieslib/utils/wininclude.h>
#include <utilitieslib/utils/FolderCache.h>


TexInf    *tex_infs = NULL;
int        tex_inf_count=0,tex_inf_max=0;
static StashTable htTexInfs=0;

static char *root_folder=NULL;

static char *texFixName(const char *name)
{
    static char buffer[MAX_PATH];
    char *s;
    strcpy(buffer, name);
    forwardSlashes(buffer);
    if (s = strrchr(buffer, '/')) {
        strcpy(buffer, s+1);
    }
    if (s = strrchr(buffer, '.')) {
        *s = 0;
    }
    return buffer;
}

TexInf *getTexInf(const char *name)
{
    int i;
    if (htTexInfs==0) {
        htTexInfs = stashTableCreateWithStringKeys(10000, StashDeepCopyKeys);
        for(i=0;i<tex_inf_count;i++)
        {
            stashAddInt(htTexInfs, texFixName(tex_infs[i].name), i+1, false);
        }
    }
    if (stashFindInt(htTexInfs, texFixName(name), &i)) {
        return &tex_infs[i-1];
    } else {
        return NULL;
    }
}

// Gets a tfh from a .texture file
#define HEADER_READ_SIZE    (sizeof(ti->tfh))
FileScanAction texLoadDescProcessor(char *dir, struct _finddata32_t* data) {
    char fullpath[MAX_PATH];
    TexInf    *ti;
    FILE *f;
    int bytes_read;
    static __declspec(align(128)) char buf[HEADER_READ_SIZE];

    if (data->name[0]=='_')
        return FSA_NO_EXPLORE_DIRECTORY;
    // Given a filename, load it's header into tex_infs
    sprintf(fullpath, "%s/%s", dir, data->name);
    forwardSlashes(fullpath);
    if (!strrchr(fullpath, '.') || stricmp(strrchr(fullpath, '.'), ".texture")!=0) {
        // not a .texture file
        return FSA_EXPLORE_DIRECTORY;
    }
    assert(!(data->attrib & _S_IFDIR));
    if (strnicmp(fullpath, root_folder, strlen(root_folder))!=0) {
        // This is not in the primary folder?  Skip it.  Only happens when not running out of .piggs, I think...
        return FSA_EXPLORE_DIRECTORY;
    }

    if (tex_inf_count >= tex_inf_max)
    {
        tex_inf_max = (tex_inf_max + 100) * 2;
        tex_infs = realloc(tex_infs,sizeof(TexInf) * tex_inf_max);
    }
    ti = &tex_infs[tex_inf_count];
    ti->name = strdup(fullpath + strlen(root_folder));
    if (ti->name[0]=='/') ti->name++;

    f = fileOpen(fullpath, "rbR"); // R = random access, since we are only reading texture header
    if (!f) {
        printf("Error opening .texture file : %s\nTry running gettex again after closing the game\n", fullpath);
        return FSA_EXPLORE_DIRECTORY;
    }

    // read only the texture header
    bytes_read = fread(buf, 1, HEADER_READ_SIZE, f);
    fclose(f);
    assert(bytes_read >= sizeof(ti->tfh));
    memcpy(&ti->tfh, buf, sizeof(ti->tfh));

    tex_inf_count++;
    return FSA_EXPLORE_DIRECTORY;
}


static void texLoadDesc(char *folder) // must be like texture_library\  (can't be texture_library\subfolder\)
{
    loadstart_printf("Loading texture flags from %s...", folder);
    root_folder = strdup(folder);
    forwardSlashes(root_folder);
    fileScanAllDataDirs(folder, texLoadDescProcessor);
    loadend_printf("");

}



static FILE* gettex_lock_handle=NULL;
static char *lockfilename = "c:\\gettex.lock";
static void releaseGettexLock() {
    fclose(gettex_lock_handle);
    fileForceRemove(lockfilename);
    gettex_lock_handle = 0;
}

static void waitForGettexLock() {
    fileForceRemove(lockfilename);
    while ((gettex_lock_handle = fopen(lockfilename, "wl")) == NULL) {
        Sleep(500);
        fileForceRemove(lockfilename);
    }
    if (gettex_lock_handle != NULL) {
        fprintf(gettex_lock_handle, "texLoadDesc\r\n");
    }
}

static void texChanged(const char *relpath, int when)
{
    int i=0;
    TextureFileHeader tfh;
    TexInf *ti;
    FILE *f;

    fileWaitForExclusiveAccess(relpath);
    waitForGettexLock();

    f = fileOpen(relpath, "rb");
    if (!f) {
        printf("Error opening .texture file : %s\n", relpath);
        releaseGettexLock();
        return;
    }
    fread(&tfh, sizeof(tfh), 1, f);
    fclose(f);

    if (ti = getTexInf(relpath)) {
        // Already in list, just update
    } else {
        // Add to list
        if (tex_inf_count >= tex_inf_max)
        {
            tex_inf_max = (tex_inf_max + 100) * 2;
            tex_infs = realloc(tex_infs,sizeof(TexInf) * tex_inf_max);
        }
        ti = &tex_infs[tex_inf_count];
        ti->name = strdup(relpath);
        if (ti->name[0] == '/')
            ti->name++;
        stashAddInt(htTexInfs, texFixName(ti->name), tex_inf_count+1, false);
        tex_inf_count++;
    }
    ti->tfh = tfh;

    releaseGettexLock();
}



void texLoadAllInfo(int reloadcallback)
{
    texLoadDesc("texture_library/");
    if (reloadcallback) {
        // Yes for GetVrml, no for GetTex
        FolderCacheSetCallback(FOLDER_CACHE_CALLBACK_UPDATE, "texture_library/*.texture", texChanged);
    }
}

