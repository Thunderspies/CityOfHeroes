#include "utilitieslib/components/FilespecMap.h"
#include "utilitieslib/components/earray.h"
#include "utilitieslib/utils/utils.h"
#include "utilitieslib/assert/assert.h"
#include "utilitieslib/utils/strings_opt.h"

typedef struct FilespecMapElement
{
    char *filespec;
    void *data;
    bool isExt;
    bool isRecursive;
    int numSlashes;
} FilespecMapElement;

typedef struct FilespecMap
{
    FilespecMapElement **elements;
} FilespecMap;

FilespecMap *filespecMapCreate(void)
{
    FilespecMap *ret = calloc(sizeof(FilespecMap), 1);
    return ret;
}

void filespecMapDestroy(FilespecMap *handle)
{
    eaDestroyEx(&handle->elements, NULL);
    free(handle);
}

int sortByNumSlashes(const FilespecMapElement **a, const FilespecMapElement **b)
{
    if ((*a)->isExt != (*b)->isExt) {
        // Those with extensions first!
        return (*b)->isExt - (*a)->isExt;
    }
    if ((*a)->numSlashes != (*b)->numSlashes) {
        // Those with more slashes first!
        return (*b)->numSlashes - (*a)->numSlashes;
    }

    if ((*a)->isRecursive != (*b)->isRecursive) {
        //   folder/* 0
        //   folder/+ 1
        //   -> show things in folder/ , but not subfolders
        // Those non-recursive first!
        return (*a)->isRecursive - (*b)->isRecursive;
    }
    return 0;
}

void filespecMapAdd(FilespecMap *handle, const char *spec_in, void *data)
{
    char spec[MAX_PATH];
    FilespecMapElement *elem = calloc(sizeof(*elem), 1);
    char *star;
    char *slash;
    int i;

    // Clean up input
    if (spec_in[0]=='/' || spec_in[0]=='\\')
        spec_in++;
    Strncpyt(spec, spec_in);
    forwardSlashes(spec);
    if (strEndsWith(spec, "/+")) {
        spec[strlen(spec)-1] = '*';
        elem->isRecursive = false;
    } else {
        elem->isRecursive = true;
    }

    // Search for duplicates
    for (i=0; i<eaSize(&handle->elements); i++) {
        FilespecMapElement *elem = handle->elements[i];
        if (stricmp(elem->filespec, spec)==0) {
            // Remove it!
            eaRemoveFast(&handle->elements, i);
            break;
        }
    }

    // Add element, verify
    star = strrchr(spec, '*');
    assertmsg(star == strchr(spec, '*'), "Only one wildcard allowed");
    assertmsg(star[1]=='\0' || star[1]=='.', "Must be in the form of [path/]*[.ext]");
    assertmsg(star == spec || star[-1]=='/', "Must be in the form of [path/]*[.ext]");
    elem->filespec = strdup(spec);
    elem->data = data;
    slash = spec-1;
    while (slash = strchr(slash+1, '/'))
        elem->numSlashes++;
    if (!strEndsWith(spec, "*")) {
        elem->isExt = true;
    }
    eaPush(&handle->elements, elem);
    eaQSort(handle->elements, sortByNumSlashes);
}

bool filespecMapGet(FilespecMap *handle, const char *filename, void **result)
{
    int i;
    for (i=0; i<eaSize(&handle->elements); i++) {
        FilespecMapElement *elem = handle->elements[i];
        if (simpleMatch(elem->filespec, filename)) {
            // It matched!
            if (!elem->isRecursive) {
                char *lastmatch = getLastMatch();
                if (strchr(lastmatch, '/'))
                    continue;
            }
            *result = elem->data;
            return true;
        }
    }
    *result = NULL;
    return false;
}

bool filespecMapGetExact(FilespecMap *handle, const char *spec, void **result)
{
    int i;
    for (i=0; i<eaSize(&handle->elements); i++) {
        FilespecMapElement *elem = handle->elements[i];
        if (stricmp(elem->filespec, spec)==0) {
            *result = elem->data;
            return true;
        }
    }
    *result = NULL;
    return false;
}


void filespecMapAddInt(FilespecMap *handle, const char *spec, int data)
{
    filespecMapAdd(handle, spec, (void*)(uintptr_t)data);
}

bool filespecMapGetInt(FilespecMap *handle, const char *filename, int *result)
{
    uintptr_t res;
    bool ret = filespecMapGet(handle, filename, (void**)&res);
    *result = (int)res;
    return ret;
}

bool filespecMapGetExactInt(FilespecMap *handle, const char *spec, int *result)
{
    return filespecMapGetExact(handle, spec, (void**)result);
}

int filespecMapGetNumElements(FilespecMap *handle)
{
    return eaSize(&handle->elements);
}
