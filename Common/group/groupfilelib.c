#include "group/groupfilelib.h"
#include <utilitieslib/components/StashTable.h>
#include <string.h>
#include <utilitieslib/utils/utils.h>
#include "group/group.h"
#include <utilitieslib/assert/assert.h>
#include <utilitieslib/utils/error.h>
#include <utilitieslib/utils/strings_opt.h>
#include <utilitieslib/utils/fileutil.h>
#include <utilitieslib/utils/FolderCache.h>
#include "edit/Menu.h"
#include <utilitieslib/utils/textparser.h>
#include <utilitieslib/components/earray.h>
#include "group/groupfileload.h"
#include <utilitieslib/utils/serialize.h>
#include <utilitieslib/components/SharedMemory.h>
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/network/crypt.h>
#include "bases/bases.h"
#include "entity/LoadDefCommon.h"
#include <utilitieslib/utils/structInternals.h>

typedef struct GroupLibNameEntry
{
    char        *name;
    union {
        struct {
            short        dir_idx;
            U8            is_rootname;
        };
        int intvalue;
    };
    GroupBounds    bounds;
} GroupLibNameEntry;

typedef struct GroupNames {
    GroupFileEntry **group_files;
    GroupLibNameEntry **group_libnames;
} GroupNames;

GroupNames group_names;

// Format for writing/reading .bin file (not actually parsed text)
TokenizerParseInfo GroupFileEntrySaveInfo[] =
{
    { "",    TOK_STRUCTPARAM|TOK_STRING(GroupFileEntry, name, 0) },
    { "", 0, 0 }
};

TokenizerParseInfo GroupLibNameEntrySaveInfo[] =
{
    { "",    TOK_STRUCTPARAM|TOK_STRING(GroupLibNameEntry, name, 0) },
    { "",    TOK_STRUCTPARAM|TOK_INT(GroupLibNameEntry, intvalue, 0) },
    { "", 0, 0 }
};

TokenizerParseInfo GroupNamesSaveInfo[] = {
    { "F",    TOK_STRUCT(GroupNames,group_files, GroupFileEntrySaveInfo)},
    { "N",    TOK_STRUCT(GroupNames,group_libnames, GroupLibNameEntrySaveInfo)},
    { "", 0, 0 }
};

StashTable                    group_name_hashes;
StashTable                    group_file_hashes;

static char                    binfile_fullpath[1000];

extern Menu * libraryMenu;
extern void   libraryMenuClickFunc(MenuEntry *,void *);

static char *groupGetPublicName(const char *name,char *pub_name,int len)
{
    char    *s;

    strncpyt(pub_name,name,len);
    if (pub_name[strlen(pub_name)-1] == '&')
    {
        s = strrchr(pub_name,'_');
        if (s)
            *s = 0;
    }
    return pub_name;
}

static bool groupNameIdx(const char *name,int *idx)
{
    char    buf[1000];
    const char* cs, *s2;

    cs = strrchr(name,'/');
    if (!cs++)
        cs = name;
    s2 = strstriConst(cs,"__");
    if (s2)
    {
        strcpy(buf,cs);
        buf[s2-cs] = 0;
        cs = buf;
    }
    return stashFindInt(group_name_hashes,cs,idx);
}

static void addLibNameToHashes(GroupLibNameEntry *gn, int idx)
{
    if (!stashAddInt(group_name_hashes, gn->name, idx, false))
    {
        char    *s,*old_libname,*new_libname,old_txtname[1000],new_txtname[1000],old_author[1000],new_author[1000];
        char old_txtname_fixed[MAX_PATH];
        int new_idx = idx;

        stashFindInt(group_name_hashes,gn->name,&idx);
        old_libname = objectLibraryNameFromIdx(idx);
        new_libname = group_names.group_files[gn->dir_idx]->name;

        strcpy(old_txtname,objectLibraryPathFromObj(old_libname));
        strcpy(new_txtname,new_libname);
        s = strrchr(old_txtname,'.');
        *s = 0;
        s = strrchr(new_txtname,'.');
        *s = 0;
        strcat(old_txtname,".txt");
        strcat(new_txtname,".txt");
        forwardSlashes(old_txtname);
        forwardSlashes(new_txtname);

        if(!strStartsWith(old_txtname, "object_library/"))
            sprintf(old_txtname_fixed, "object_library/%s", old_txtname);
        else
            strcpy(old_txtname_fixed, old_txtname);

        if(!stricmp(old_txtname_fixed, new_txtname))
        {
            // seems like someone renamed a -> b -> a ... leak any old stuff and pretend it doesn't exist
            printf("re-rename detected (leaking even more old data)\n");
            if(!stashAddInt(group_name_hashes, gn->name, new_idx, true))
                FatalErrorf("could not add %s/%s to groupname hash\n", new_libname, gn->name);
            return;
        }

        if (fileNewer(old_txtname,old_libname))
            old_libname = old_txtname;
        if (fileNewer(new_txtname,new_libname))
            new_libname = new_txtname;
        _unlink(fileLocateWrite_s(binfile_fullpath, NULL, 0));
        FatalErrorf("dup name: %s/%s (%s)\n          %s/%s (%s)\n\n\n",old_libname,gn->name,old_author,new_libname,gn->name,new_author);
    }
}

static int addLibName(char *objname,int dir_idx,int is_rootname)
{
    GroupLibNameEntry    *gn;
    gn = ParserAllocStruct(sizeof(GroupLibNameEntry));
    gn->name = ParserAllocString(objname);
    gn->dir_idx = dir_idx;
    gn->is_rootname = is_rootname;
    return eaPush(&group_names.group_libnames, gn);
}

static int getDirIdx(const char *name,int search)
{
    const char            *fname=0;
    int                idx;
    GroupFileEntry    *gf;

    if (search)
        fname = strstriConst(name,"object_library");
    if (!fname)
        fname = name;
    if (!stashFindInt(group_file_hashes,fname,&idx))
    {
        gf = ParserAllocStruct(sizeof(GroupFileEntry));
        gf->loaded = 0;
        gf->checked_out = 0;
        gf->name = ParserAllocString(fname);
        idx = eaPush(&group_names.group_files, gf);
        stashAddInt(group_file_hashes, gf->name, idx,false);
    }
    return idx;
}

char * skipStr(char * str,char * skip)
{
    char * s=strstr(str,skip);
    if (!s)
        return str;
    return s+strlen(skip);
}

void objectLibraryDeleteFullName(char *_fname)
{
    char    *objname,fname[1000];
    GroupDef *def;
    strcpy(fname, _fname);

    objname = strrchr(fname,'/');
    if (!objname)
        return;
    *objname++ = 0;

    def = groupDefFind(objname);
    if (def)
        def->in_use = 0;

    stashRemovePointer(group_name_hashes,objname, NULL);
#ifdef CLIENT
    //deletes an item from the libraryMenu, this is untested
    //as long as this function is called in the same manner as
    //objectLibraryAddName it should work just fine
    if (libraryMenu!=NULL) {
        char fullName[1000];
        strcpy(fullName,fname);
        strcat(fullName,"/");
        strcat(fullName,objname);
        deleteEntryFromMenu(libraryMenu,skipStr(fullName,"object_library/"));
    }
#endif
}

void objectLibraryDeleteName(GroupDef *def)
{
    char    fname[1000];

    if (!groupInLib(def))
        return;
    groupDefGetFullName(def,SAFESTR(fname));
    objectLibraryDeleteFullName(fname);
}

void objectLibraryAddToMenu(char *fullName)
{
#ifdef CLIENT
    if (libraryMenu!=NULL) {
        addEntryToMenu(libraryMenu,skipStr(fullName,"object_library/"),libraryMenuClickFunc,0);
    }
#endif
}

void objectLibraryAddFullName(char *_fname,int is_rootname)
{
    char    *objname,fname[1000],dirname[1000],*s;
    int        dir_idx,idx,len;
    strcpy(fname, _fname);
    strcpy(dirname, _fname);

    objname = strrchr(dirname,'/');
    if (!objname)
        return;
    *objname++ = 0;

    if (stashFindInt(group_name_hashes,objname,&idx))
        return;

    // Convert /path/file/object to /path/file/file.geo
    s = strrchr(fname,'/');
    *s=0;
    s = strrchr(fname,'/');
    if (!s)
        s=fname;
    if (*s=='/') s++;
    len = (int)strlen(s);
    strcat(fname, "/");
    strncat(fname, s, len);
    strcpy(s+len+1+len, ".geo");
    dir_idx = getDirIdx(fname, 1);

//JE: It used to do the following, but that would fail to add new things
//    for(dir_idx=0;dir_idx<eaSize(&group_names.group_files);dir_idx++)
//    {
//        if (strstri(group_names.group_files[dir_idx]->name,dirname))
//            break;
//    }
    idx = addLibName(objname,dir_idx,is_rootname);
    addLibNameToHashes(group_names.group_libnames[idx], idx);

    //this creates the full path, including objname, removes the leading '/'
    //and adds it to the object library menu
    {
        char fullName[1000];
        strcpy(fullName,dirname);
        strcat(fullName,"/");
        strcat(fullName,objname);
        objectLibraryAddToMenu(fullName);
    }
}

void objectLibraryAddName(GroupDef *def)
{
    char    fname[1000];

    if (!groupInLib(def) || groupIsPrivate(def))
        return;
    groupDefGetFullName(def,SAFESTR(fname));
    objectLibraryAddFullName(fname,0);
}

static void addFileNameToHashes(GroupFileEntry *gf, int idx)
{
    int new_idx;
    if (!stashFindInt(group_file_hashes,gf->name,&new_idx))
    {
        stashAddInt(group_file_hashes, gf->name, idx,false);
    } else {
        assert(new_idx == idx);
    }
}

int objectLibraryCount()
{
    return eaSize(&group_names.group_libnames);
}

char *objectLibraryNameFromIdx(int i)
{
    return group_names.group_libnames[i]->name;
}

char *objectLibraryPathFromIdx(int i)
{
    return group_names.group_files[group_names.group_libnames[i]->dir_idx]->name;
}

int objectLibraryFileCount(void)
{
    return eaSize(&group_names.group_files);
}

GroupFileEntry *objectLibraryEntryFromIdx(int idx)
{
    assert(idx < objectLibraryFileCount());
    return group_names.group_files[idx];
}

GroupFileEntry *objectLibraryEntryFromObj(const char *obj_name)
{
    int                    idx;
    GroupLibNameEntry    *gn;
    char                pub_name[1000];

    if (!groupNameIdx(groupGetPublicName(obj_name,pub_name,999),&idx))
        return 0;
    gn = group_names.group_libnames[idx];
    return objectLibraryEntryFromIdx(gn->dir_idx);
}

GroupFileEntry *objectLibraryEntryFromFname(const char *fname)
{
    int        idx;
    char    geo_name[MAX_PATH];

    changeFileExt(fname,".geo",geo_name);
    forwardSlashes(geo_name);
    idx = getDirIdx(geo_name,1);
    return objectLibraryEntryFromIdx(idx);
}

char *objectLibraryPathFromObj(const char *obj_name)
{
    GroupFileEntry    *gf;

    if (!(gf = objectLibraryEntryFromObj(obj_name)))
        return 0;
    return gf->name;
}

char *objectLibraryPathFromFname(const char *fname)
{
    int        idx;

    idx = getDirIdx(fname,1);
    return group_names.group_files[idx]->name;
}

static void loadGroupNames(char *fname, __time32_t date)
{
    char    *s,*mem,*str,*args[100],geo_name[1000];
    int        count;
    int        is_rootname=0, dir_idx;

    str = mem = fileAlloc(fname,0);
    strcpy(geo_name,fname);
    s = strrchr(geo_name,'.');
    if (s) {
        if (stricmp(s, ".rootnames")==0)
            is_rootname = 1;
        strcpy(s,".geo");
    }
    dir_idx = getDirIdx(geo_name,0); // make sure this starts with object_library/ !!!
    while(str)
    {
        count = tokenize_line(str,args,&str);
        if (count == 2 && stricmp(args[0],"Def")==0 && args[1][strlen(args[1])-1] != '&')
        {
            addLibName(args[1],dir_idx, is_rootname);
        }
    }
    free(mem);
}

static int cmpNames(const void* aData, const void* bData)
{
    const GroupLibNameEntry ** a = (const GroupLibNameEntry **)aData;
    const GroupLibNameEntry ** b = (const GroupLibNameEntry **)bData;
    int aidx = (*a)->dir_idx;
    int bidx = (*b)->dir_idx;
    if (aidx!=bidx)
        return stricmp(group_names.group_files[aidx]->name, group_names.group_files[bidx]->name);
    return stricmp((*a)->name,(*b)->name);
}

static void loadGroupNamesFromFileList(FileList *diskfiles)
{
    ParserDestroyStruct(GroupNamesSaveInfo, &group_names); // Clean up anything left over during a failed load
    FileListForEach(diskfiles, loadGroupNames);
    eaQSort(group_names.group_libnames,cmpNames);
}

static FileList object_lib_disk = 0;

static FileScanAction populateFileList(char* dir, struct _finddata32_t* data)
{
    char buffer[512];
    int len,underscore;

    underscore = data->name[0] == '_';
    len = (int)strlen(data->name);
    if (( (len > 4 && !stricmp(".txt", data->name+len-4)) || (len > 10 && stricmp(".rootnames",data->name+len-10)==0)) && !underscore)
    {
        sprintf(buffer, "%s/%s", dir, data->name);
        FileListInsert(&object_lib_disk, buffer, 0);
    }
    if ((data->attrib & _A_SUBDIR) && underscore)
        return FSA_NO_EXPLORE_DIRECTORY;
    return FSA_EXPLORE_DIRECTORY;
}

// returns 1 if no changes made
static int UpdateObjectLibraryBin(int production, int force_rebuild)
{
    static char *binfile_name = "bin/defnames.bin";
    SimpleBufHandle binfile=0;
    int uptodate = 1;
    int crc;
    int savedpos;
    FileList object_lib_bin = 0;

    if (!production || force_rebuild)
    {
        // read file list from disk
        FileListCreate(&object_lib_disk);
        fileScanAllDataDirs("object_library", populateFileList);
    }

    if (!fileExists(binfile_name))
    {
        verbose_printf("UpdateObjectLibraryBin: file %s not found, rebuilding\n", binfile_name);
        uptodate = 0;
        goto finishup;
    }

    // open bin, check crc, check header
    crc = ParserCRCFromParseInfo(GroupNamesSaveInfo, NULL);
    binfile = SerializeReadOpen(binfile_name, PARSE_SIG, crc, production);
    if (!binfile)
    {
        verbose_printf("UpdateObjectLibraryBin: binfile %s is the incorrect version, rebuilding\n", binfile_name);
        uptodate = 0;
        goto finishup;
    }
    savedpos = SimpleBufTell(binfile);

    // read file list from bin
    if (!FileListRead(&object_lib_bin, binfile))
    {
        verbose_printf("UpdateObjectLibraryBin: binfile %s is the incorrect version, rebuilding\n", binfile_name);
        uptodate = 0;
        goto finishup;
    }

    if (!production || force_rebuild)
    {
        if (!FileListIsBinUpToDate(&object_lib_bin, &object_lib_disk))
        {
            uptodate = 0;
            goto finishup;
        }
    }

    SimpleBufSeek(binfile, savedpos, SEEK_SET); // Reset position to *before* file list

    if (uptodate)
    {
        // read all data
        if (!ParserReadBinaryFile(binfile, NULL, GroupNamesSaveInfo, &group_names, NULL, NULL))
        {
            binfile = 0; // ParserReadBinaryFile closes the stream
            uptodate = 0;
            goto finishup;
        }
        binfile = 0; // ParserReadBinaryFile closes the stream
    }

finishup:
    FileListDestroy(&object_lib_bin);
    if (binfile)
        SerializeClose(binfile);
    
    if (!uptodate)
    {
        // generate def names from .rootnames and .txts
        loadGroupNamesFromFileList(&object_lib_disk);

        // Write everything to bin
        ParserWriteBinaryFile(binfile_name, GroupNamesSaveInfo, &group_names, &object_lib_disk, NULL, 0, 0);
    }
    FileListDestroy(&object_lib_disk);

    return uptodate;
}

int objectLibraryLoadShared(int production, int force_rebuild)
{
    void *pTemp;
    const char *pchSMName = MakeSharedMemoryName("defnames.bin");
    SharedMemoryHandle *shared_memory=NULL;
    SM_AcquireResult ret;
    int myret=0;

    ret = sharedMemoryAcquire(&shared_memory, pchSMName);
    if (ret==SMAR_DataAcquired) {
        StructCompress(GroupNamesSaveInfo, sharedMemoryGetDataPtr(shared_memory), sizeof(group_names), &group_names, NULL, NULL); 
        myret = 1;
    } else if (ret==SMAR_Error) {
        // An error occurred with the shared memory, just do what we would normally do
        myret = UpdateObjectLibraryBin(production, force_rebuild);
    } else if (ret==SMAR_FirstCaller) {
        // Load data and copy to shared memory
        unsigned long size;
        // Load data
        myret = UpdateObjectLibraryBin(production, force_rebuild);
        // Copy to shared memory
        size = (unsigned long)ParserGetStructMemoryUsage(GroupNamesSaveInfo, &group_names, sizeof(group_names));
        sharedMemorySetSize(shared_memory, size);
        pTemp = ParserCompressStruct(GroupNamesSaveInfo, &group_names, sizeof(group_names), NULL, sharedMemoryAlloc, shared_memory);
        sharedMemoryUnlock(shared_memory);
    }

    return myret;
}

void objectLibraryLoadNames(int force_rebuild)
{
    int        i, from_binfile;
    static int init;

    if (init)
        return;
    init = 1;
    group_file_hashes = stashTableCreateWithStringKeys(600, StashDefault); // No deep copy

    strcpy(binfile_fullpath,"bin/defnames.bin");

    from_binfile = objectLibraryLoadShared(isProductionMode(), force_rebuild);

    if (stashGetValidElementCount(group_file_hashes)) {
        stashTableDestroy(group_file_hashes); // This hash table may have pointed to data before it was copied to shared memory
        group_file_hashes = stashTableCreateWithStringKeys(600, StashDefault); // No deep copy
    }

    for(i=eaSize(&group_names.group_files)-1; i>=0; i--)
        addFileNameToHashes(group_names.group_files[i], i);

    group_name_hashes = stashTableCreateWithStringKeys(10000, StashDefault); // No deep copy
    for(i=eaSize(&group_names.group_libnames)-1; i>=0; i--)
        addLibNameToHashes(group_names.group_libnames[i], i);

    writeConsole(OUTPUT_INFO, "Loaded grouplibs: %d files, %d names", eaSize(&group_names.group_files), eaSize(&group_names.group_libnames));
}

void objectLibraryClearTags()
{
    int        i;

    for(i=0;i<eaSize(&group_names.group_files);i++)
        group_names.group_files[i]->loaded = 0;
}

void objectLibraryAddNewDef(char *objname, char *geoname)
{
    char txtname[1024];
    char *s;
    GroupDef *def;
    strcpy(txtname, geoname);
    s = strstri(txtname, ".geo");
    if (s)
        strcpy(s, ".txt");

    if (groupFileFind(txtname))
    {
        def = groupDefFind(objname);
        assert(!def || def->file);
        if (!def)
            def = groupDefNew(groupFileFind(txtname));
        else
            def->in_use = 1;
        assert(def->file);

        //addModel
        def->model = groupModelFind(objname);
        if (!def->model)
            Errorf("Cant find root geo %s",objname);
        groupSetTrickFlags(def);
        groupDefName(def,txtname,objname);    //GROUPNAME
        groupSetBounds(def);
        groupSetVisBounds(def);
        def->init = 1;
        def->saved = 1;
    }
}

void objectLibraryProcessNameChanges(const char *_fname, char **obj_names)
{
    char fname[MAX_PATH], *s;
    int dir_idx, i;
    int *found=NULL;
    StashElement element;
    StashTableIterator it;
    int num_objs = eaSize(&obj_names);
    strcpy(fname, _fname);
    s = strchr(fname, '.');
    if (s)
        strcpy(s, ".geo");
    {
        char        fname_txt[MAX_PATH];
        GroupFile    *file;

        changeFileExt((char*)_fname,".txt",fname_txt);
        file = groupFileFind(fname_txt);
        if (file)
            file->modified_on_disk = 1;
    }        
    eaiCreate(&found);
    eaiSetSize(&found, num_objs);
    memset(found, 0, sizeof(int)*eaiSize(&found));

    forwardSlashes(fname);
    dir_idx = getDirIdx(fname, 1);

    // Search group_name_hashes, remove any  with same idx and not in obj_names
    stashGetIterator(group_name_hashes, &it);
    while(0 != (stashGetNextElement(&it, &element))) {
        int libname_idx = (int)(intptr_t)stashElementGetPointer(element);
        GroupLibNameEntry *gn = group_names.group_libnames[libname_idx];
        if (!gn->is_rootname)
            continue;
        if (gn->dir_idx == dir_idx) {
            const char *name = stashElementGetStringKey(element);
            bool foundit=false;
            for (i=0; i<num_objs && !foundit; i++) {
                if (stricmp(name, obj_names[i])==0) {
                    // Found it!
                    found[i] = 1;
                    foundit=true;
                }
            }
            if (!foundit) {
                // This one has been deleted
                char fullname[1000];
                printf("    Def %s has been removed\n", name);
                strcpy(fullname, fname);
                s = strrchr(fullname, '/');
                if (s)
                    *(++s)=0;
                strcat(fullname, name);
                log_printf("getvrml.log","Deleted %s - %s",fullname,skipStr(fullname,"object_library/"));
                objectLibraryDeleteFullName(skipStr(fullname,"object_library/"));
            }
        }
    }

    // Add new ones
    for (i=0; i<num_objs; i++) {
        if (!found[i]) {
            char fullname[1000];
            char fullobjname[1000];
            int idx;
            if (strstri(obj_names[i], ".geo")) // Not sure how these are getting in here, but let's skip them!
                continue;
            printf("    Def %s has been added\n", obj_names[i]);
            strcpy(fullname, fname);
//            s = strrchr(fullname, '/');
//            if (s)
//                *(++s)=0;
//            strcat(fullname, obj_names[i]);
            dir_idx = getDirIdx(fullname, 1);
            idx = addLibName(obj_names[i],dir_idx,1);
            addLibNameToHashes(group_names.group_libnames[idx], idx);
            objectLibraryAddNewDef(obj_names[i],fullname);
            strcpy(fullobjname, fullname);
            s = strrchr(fullobjname, '/');
            *++s=0;
            strcat(fullobjname, obj_names[i]);
            log_printf("getvrml.log","Added %s - %s",fullobjname,skipStr(fullobjname,"object_library/"));
            objectLibraryAddToMenu(skipStr(fullobjname,"object_library/"));
        }
    }

    eaiDestroy(&found);
}

void objectLibrarySetBounds(const char *objname,GroupBounds *bounds)
{
    int                    idxcheck=-1;
    GroupLibNameEntry    *gn;

    if (!groupNameIdx(objname,&idxcheck))
        return; // assert(0);
    gn = group_names.group_libnames[idxcheck];
    gn->bounds = *bounds;
}

int objectLibraryGetBounds(const char *objname,GroupDef *def)
{
    int                    idx;
    GroupLibNameEntry    *gn;

    if (!groupNameIdx(objname,&idx))
        return 0;
    gn = group_names.group_libnames[idx];
    memcpy(def->min,&gn->bounds,sizeof(GroupBounds));
    return 1;
}

typedef struct LibObjEntry
{
    GroupBounds;
    char        *name;
} LibObjEntry;

typedef struct
{
    LibObjEntry        **entries;
} LibFileDesc;

static TokenizerParseInfo parse_lib_obj[] = {
    { "",                TOK_STRUCTPARAM | TOK_STRING(LibObjEntry,name, 0)},
    { "Bounds",            TOK_RAW_S(LibObjEntry,min,sizeof(GroupBounds)) },
    { "End",            TOK_END,            0},
    { "", 0, 0 }
};

static TokenizerParseInfo parse_lib_desc[] = {
    { "Entry",            TOK_STRUCT(LibFileDesc,entries,parse_lib_obj) },
    { "", 0, 0 }
};

static char *boundsFileName(const char *groupfile_name,char *boundsname, size_t boundsname_size)
{
    char    buf[MAX_PATH];

    sprintf(buf,"geobin/%s",groupfile_name);
    changeFileExt_s(buf,".bounds",boundsname, boundsname_size);
    return boundsname;
}

static int boundsEqual(GroupBounds *cache_bound,char *cache_name,GroupBounds *bound,const char *name)
{
    if (strcmp(name,cache_name)!=0)
        return 0;
    if (memcmp(bound,cache_bound,sizeof(*bound))!=0)
        return 0;
    return 1;
}

static int objectLibraryLoadBoundsFileInternal(const char *groupfile_name,U32 checksum,GroupFile *file)
{
    char            boundsname[MAX_PATH];
    int                i,j,success=0;
    LibFileDesc        lib_desc = {0};
    LibObjEntry        *entry;
    GroupFileEntry    *libfile;

    libfile = objectLibraryEntryFromFname(groupfile_name);
    if (libfile->bounds_checksum == checksum)
        return 1;
    boundsFileName(groupfile_name,SAFESTR(boundsname));
    
    if (!ParserReadBinaryFile(0, boundsname, parse_lib_desc, &lib_desc, NULL, NULL))
        return 0;
    cryptAdler32Init();
    for(j=-1,i=0;i<eaSize(&lib_desc.entries);i++)
    {
        entry = lib_desc.entries[i];
        cryptAdler32Update((void*)entry->min, (int)sizeof(GroupBounds));
        cryptAdler32Update((void*)entry->name, (int)strlen(entry->name));

        if (file)
        {
            GroupDef    *def = 0;
            for(++j;j<file->count;j++)
            {
                if (groupIsPublic(file->defs[j]))
                {
                    def = file->defs[j];
                    break;
                }
            }
            if (!def || !boundsEqual((void*)entry->min,entry->name,(void*)def->min,def->name))
                break;
        }
    }
    libfile->bounds_checksum = cryptAdler32Final();
    if (checksum == libfile->bounds_checksum)
    {
        success = 1;
        for(i=0;i<eaSize(&lib_desc.entries);i++)
            objectLibrarySetBounds(lib_desc.entries[i]->name,(GroupBounds*)lib_desc.entries[i]);
    }
    ParserDestroyStruct(parse_lib_desc, &lib_desc);
    return success;
}

int objectLibraryLoadBoundsFile(const char *groupfile_name,U32 checksum)
{
    return objectLibraryLoadBoundsFileInternal(groupfile_name,checksum,0);
}

void objectLibraryWriteBoundsFile(GroupFile *file)
{
    LibObjEntry    *entry;
    int            i,count=0,success;
    GroupDef    *def;
    LibFileDesc    lib_desc = {0};
    char        boundsname[MAX_PATH],fullpath[MAX_PATH];

    if (isProductionMode())
        return;
    for(i=0;i<file->count;i++)
    {
        def = file->defs[i];
        if (!groupIsPublic(def))
            continue;
        entry = ParserAllocStruct(sizeof(*entry));
        entry->name = ParserAllocString(def->name);
        eaPush(&lib_desc.entries,entry);
        memcpy(entry->min,def->min,sizeof(GroupBounds));
    }
    FileListInsert(&g_parselist, file->fullname, 0);
    boundsFileName(file->fullname,SAFESTR(boundsname));
    fileLocateWrite(boundsname,fullpath);
    mkdirtree(fullpath);
    success = ParserWriteBinaryFile(fullpath, parse_lib_desc, &lib_desc, &g_parselist, NULL, 0, 0);
    FileListDestroy(&g_parselist);
    ParserDestroyStruct(parse_lib_desc, &lib_desc);
}

void objectLibraryWriteBoundsFileIfDifferent(GroupFile *file)
{
    int            i;
    GroupDef    *def;

    if (!file->idx)
        return;
    cryptAdler32Init();
    for(i=0;i<file->count;i++)
    {
        def = file->defs[i];
        if (!groupIsPublic(def))
            continue;
        cryptAdler32Update((void*)def->min, (int)sizeof(GroupBounds));
        cryptAdler32Update((void*)def->name, (int)strlen(def->name));
    }
    file->bounds_checksum = cryptAdler32Final();
    if (!objectLibraryLoadBoundsFile(file->fullname,file->bounds_checksum))
        objectLibraryWriteBoundsFile(file);
}


void makeBaseMakeReferenceArrays(void)
{
    int i;

    for( i = eaSize(&group_names.group_libnames)-1; i >= 0; i-- )
    {
        if( strncmp( group_names.group_libnames[i]->name, "_base_", 6 ) != 0 )
            continue;

        baseAddReferenceName( group_names.group_libnames[i]->name );
    }

    baseMakeAllReferenceArray();
}

