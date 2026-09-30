#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <utilitieslib/stdtypes.h>
#include <utilitieslib/network/crypt.h>
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/file.h>
#include "patchfileutils.h"
#include <utilitieslib/utils/error.h>
#include "filechecksum.h"
#include "patchutils.h"
#include <utilitieslib/utils/sysutil.h>
#include <utilitieslib/components/StashTable.h>
#include <utilitieslib/utils/SuperAssert.h>
#include <utilitieslib/AppVersionDefines.h>
#include <utilitieslib/utils/log.h>
// #define ENABLE_CHECKSUM_LOGGING ; ab


#define LOGID "CohUpdater"

void checksumMem(void *mem,U32 len,U32 result[4])
{
    cryptMD5Update(mem,len);
    cryptMD5Final(result);
}

ImageCheck *checksumDuplicate(ImageCheck *image,int include_pigged)
{
    int i, j;
    ImageCheck *dst = calloc(1,sizeof(*image));
    
    dst->build_name = strdup(image->build_name);
    dst->bytecount = image->bytecount;
    dst->exists_on_server = image->exists_on_server;
    dst->fname = strdup(image->fname);
    dst->pigs_opened = image->pigs_opened;
    dst->timestamp = image->timestamp;
    dst->filenames = stashTableCreateWithStringKeys(1000,StashDeepCopyKeys);

    for (i = 0; i < image->file_count; i++)
    {
        if (!image->files[i]->pig_name || include_pigged)
        {
            FileCheck *file = calloc(sizeof(*file),1);
            memcpy(file, image->files[i], sizeof(*file));
            file->name = strdup(image->files[i]->name);
            file->pig_name = file->pig_name?strdup(image->files[i]->pig_name):0;
            file->partial_count = file->partial_max = 0;
            file->partials = 0;
            for (j = 0; j < image->files[i]->partial_count; j++)
            {
                Checksum *check = dynArrayAdd(&file->partials,sizeof(file->partials[0]),&file->partial_count,&file->partial_max,1);
                CopyStructs(check, &image->files[i]->partials[j], 1);
            }

            dynArrayAddp(&dst->files,&dst->file_count,&dst->file_max,file);
            stashAddPointer(dst->filenames,file->name,file,false);
        }
    }

    assert(!include_pigged || dst->file_count == image->file_count);

    return dst;
}

void checksumUpdateFile(ImageCheck *image,FileCheck *dst_file)
{
    FileCheck *file=0;
    int j;

    if (stashFindPointer(image->filenames, dst_file->name, &file))
    {
        SAFE_FREE(file->pig_name);
        SAFE_FREE(file->partials);
        SAFE_FREE(file->name);

        memcpy(file, dst_file, sizeof(*file));
        file->name = strdup(dst_file->name);
        file->pig_name = file->pig_name?strdup(dst_file->pig_name):0;
        file->partial_count = file->partial_max = 0;
        file->partials = 0;
        file->full_status = CHECKSUM_MATCHES_MANIFEST;
        for (j = 0; j < dst_file->partial_count; j++)
        {
            Checksum *check = dynArrayAdd(&file->partials,sizeof(file->partials[0]),&file->partial_count,&file->partial_max,1);
            CopyStructs(check, &dst_file->partials[j], 1);
        }
    }
    else
    {
        file = calloc(sizeof(*file),1);
        memcpy(file, dst_file, sizeof(*file));
        file->name = strdup(dst_file->name);
        file->pig_name = file->pig_name?strdup(dst_file->pig_name):0;
        file->partial_count = file->partial_max = 0;
        file->partials = 0;
        file->full_status = CHECKSUM_MATCHES_MANIFEST;
        for (j = 0; j < dst_file->partial_count; j++)
        {
            Checksum *check = dynArrayAdd(&file->partials,sizeof(file->partials[0]),&file->partial_count,&file->partial_max,1);
            CopyStructs(check, &dst_file->partials[j], 1);
        }

        dynArrayAddp(&image->files,&image->file_count,&image->file_max,file);
        stashAddPointer(image->filenames,file->name,file,false);
    }
}

void checksumFile(char *fname,Checksum *check,int update_stats)
{
    U8            *chunk_mem;
    U32            size,chunk_size,i;
    FILE        *file;
#define CHUNK_SIZE (1<<20)

    size = fileSize(fname);
    chunk_mem = malloc(CHUNK_SIZE);
    file = fopen(fname,"rb");
    if (!file)
        goto fail;
    for(i=0;i<size;i+=CHUNK_SIZE)
    {
        chunk_size = MIN(CHUNK_SIZE,size - i);
        if (fread(chunk_mem,1,chunk_size,file) != (int)chunk_size)
            goto fail;
        cryptMD5Update(chunk_mem,chunk_size);
        if (update_stats)
            xferStatsUpdate(chunk_size);
    }
    cryptMD5Final(check->values);
    check->size = size;
fail:
    fclose(file);
    free(chunk_mem);
}

void checksumMultipleFiles(char **fnames, unsigned int nFiles, Checksum *check, int update_stats)
{
    U8            *chunk_mem;
    U32            size,chunk_size,total_size,i,curFile;
    FILE        *file;
#define CHUNK_SIZE (1<<20)

    total_size = 0;
    for ( curFile = 0; curFile < nFiles; curFile++ )
    {
        chunk_mem = malloc(CHUNK_SIZE);

        LOG_DEBUG( "opening file %s", fnames[curFile]);

        file = fopen(fnames[curFile],"rb");
        if (file)
        {
            size = fileSize(fnames[curFile]);
            total_size += size;
            
            for(i=0;i<size;i+=CHUNK_SIZE)
            {
                chunk_size = MIN(CHUNK_SIZE,size - i);
                if (fread(chunk_mem,1,chunk_size,file) != (int)chunk_size)
                {
                    free(chunk_mem);

                    return;
                }
                cryptMD5Update(chunk_mem,chunk_size);
                if (update_stats)
                    xferStatsUpdate(chunk_size);
            }

            fclose(file);
        }

        if ( chunk_mem )
            free(chunk_mem);
    }

    cryptMD5Final(check->values);
    check->size = total_size;
}

char *checksumMakeString(ImageCheck *image,char *build_name,int full_manifest)
{
    int            i;
    U32            *hash;
    FileCheck    *file;
    StuffBuff    sb;
    PigCache    cache = {0};

    initStuffBuff(&sb,1000);
    addStringToStuffBuff(&sb,"Version %d\n",CHECKSUMFILE_VERSION);
    addStringToStuffBuff(&sb,"Build \"%s\"\n",build_name);
    for(i=0;i<image->file_count;i++)
    {
        file = image->files[i];
        if (!file->full_status)
            continue;
        hash = file->full.values;
        if (file->pig_name)
            addStringToStuffBuff(&sb,"file \"%s\" \"%s\"\n",file->name,file->pig_name);
        else
            addStringToStuffBuff(&sb,"file \"%s\"\n",file->name);
        addStringToStuffBuff(&sb,"\ttime %u\n",(U32)file->timestamp);
        if (full_manifest && file->pig_name)
        {
            U32 packed_size;

            addStringToStuffBuff(&sb,"\tfull %-8u  %08x %08x %08x %08x\n",(U32)file->full.size,hash[0],hash[1],hash[2],hash[3]);
            //addStringToStuffBuff(&sb,"\tfull %-8u  %08x\n",(U32)file->full.size,hash[0]);
            if (file->pig_name)
            {
                packed_size = compressedFileSize(&cache, image->fname,file->name,file->pig_name);
                if (packed_size)
                    addStringToStuffBuff(&sb,"\tpack %-8u\n",packed_size);
            }
        }
        else
            addStringToStuffBuff(&sb,"\tfull %-8u  %08x %08x %08x %08x\n",(U32)file->full.size,hash[0],hash[1],hash[2],hash[3]);
    }
    closeOpenPigs(&cache);
    return sb.buff;
}

void checksumMakeEmpty(char *fname,char *build_name)
{
    ImageCheck image = {0};
    char        *mem = checksumMakeString(&image,build_name,0);
    safeWriteFile(fname, "wt", mem, (U32)strlen(mem));
    free(mem);
}

static char *ignoredClientFiles[] = 
{
    "uninstall.exe",
    "addcache.xml",            // launcher installs this one
    "old.CohUpdater.exe",    // launcher installs this one
    "LauncherID.ini",
    "dxdiag.html",
    "NClauncherlog.txt",
    0
};

extern int g_server_patch;

void checksumWrite(ImageCheck *image,char *checksum_name)
{
    char *checksum_text = checksumMakeString(image, image->build_name, 0);
    if (checksum_text)
    {
        safeWriteFile(checksum_name, "wb", checksum_text, (U32)strlen(checksum_text));
        free(checksum_text);
    }
    else
    {
        safeDeleteFile(checksum_name);
    }
}

void checksumImage(const char *image_dir,const char *checksum_name,const char *image_name)
{
    char        **names;
    int            i,j,count;
    StuffBuff    sb;
    S64            total_bytes=0;
    int            isClientImage = !g_server_patch && (strEndsWith(checksum_name, "coh.checksum") || strEndsWith(checksum_name, "client.chksum"));
    char *execname = getExecutableName();

    initStuffBuff(&sb,1000);
    names = getDirFiles(image_dir,&count);

    for(i=0;i<count;i++)
        total_bytes += safeFileSize(names[i]);
    xferStatsInit("StatsChecksumming",NULL,0,total_bytes);
    addStringToStuffBuff(&sb,"Version %d\n",CHECKSUMFILE_VERSION);
    if (image_name)
        addStringToStuffBuff(&sb,"Build \"%s\"\n",image_name);
    for(i=0;i<count;i++)
    {
        char    *name;
        int        len;
        Checksum    check;
        U32        *hash=check.values;

        name = names[i] + strlen(image_dir) + 1;
        LOG_DEBUG( __FUNCTION__ ": names[i]=%s. name=%s", names[i],name );
        if (isClientImage)
        {
            if (strEndsWith(execname, name))
            {
                LOG_DEBUG( __FUNCTION__ ": skipping => strEndsWith(execname=%s)", execname);
                continue;
            }
            if (simpleMatch("CohUpdater*", name)) // fpe 4/7/11 -- changed from "CohUpdater*.exe"
            {    
                LOG_DEBUG( __FUNCTION__ ": skipping => simpleMatch(CohUpdater*.exe)");
                continue;
            }
            if (simpleMatch("readme*.rtf", name) || simpleMatch("logs/*", name) || simpleMatch("*.checksum", name) ||
                    simpleMatch("*.chksum", name) || simpleMatch("dev_access.*", name) )
            {        
                continue;
            }
            for (j = 0; ignoredClientFiles[j]; j++)
            {
                if (stricmp(ignoredClientFiles[j], name)==0)
                    break;
            }
            if (ignoredClientFiles[j])
                continue;
        }

        len = fileSize(names[i]);
        checksumFile(names[i],&check,1);
        addStringToStuffBuff(&sb,"file \"%s\"\n",name);
        addStringToStuffBuff(&sb,"\ttime %u\n",fileLastChangedAltStat(names[i]));
        addStringToStuffBuff(&sb,"\tfull %-8u  %08x %08x %08x %08x\n",len,hash[0],hash[1],hash[2],hash[3]);
    }
    xferStatsFinish();
    safeWriteFile(checksum_name, "wt", sb.buff, (U32)strlen(sb.buff));
    freeStuffBuff(&sb);
}

FileCheck *checksumAddFile(ImageCheck *image,const char *fname,char *pig_name, int outputErrors)
{
    FileCheck    *file;
    int            bad = 0;

    if (image->filenames && (!image->files || !image->file_count))
    {
        stashTableDestroy(image->filenames);
        image->filenames = 0;
    }

    file = calloc(sizeof(*file),1);
    dynArrayAddp(&image->files,&image->file_count,&image->file_max,file);
    file->name = strdup(fname);

    LOG_DEBUG( __FUNCTION__ ": file->name=%s, image->file_count=%i", file->name, image->file_count );

    if (!image->filenames)
    {
        image->filenames = stashTableCreateWithStringKeys(1000,StashDeepCopyKeys);
        LOG_DEBUG( __FUNCTION__ ": created stashtable for image->fname=%s", image->fname);
    }
    
    if (!stashAddPointer(image->filenames,file->name,file, false))
    {
        FileCheck    *orig_file;
        stashFindPointer(image->filenames,file->name, &orig_file);

        if ( outputErrors )
        {
            if ( orig_file->pig_name && pig_name )
                Errorf("Duplicate file: %s (%s and %s)",file->name,orig_file->pig_name,pig_name);
            else
                Errorf("Duplicate file: %s", file->name);
        }
        bad = 1;
    }
    else
    {
        LOG_DEBUG( __FUNCTION__ ": added %s to %s(%i)", file->name, image->fname, image->filenames);
    }

    return file;
}

int checksumOpenPigs(ImageCheck *image, char *image_dir, int validate_piggs)
{
	pg_context *context = NULL;
	int result = 1;
	const int archive_count = image->file_count;
	if (image->pigs_opened) return 1;
	if (pg_context_open(&context, NULL) != PG_OK) return 0;
	for (int i = 0; i < archive_count; i++) {
		FileCheck *archive = image->files[i];
		pg_source *source = NULL;
		pg_cursor *cursor = NULL;
		pg_file *selected = NULL;
		pg_status status;
		char path[MAX_PATH];
		if (!strEndsWith(archive->name, ".pigg") &&
			!strEndsWith(archive->name, ".hogg")) continue;
		sprintf(path, "%s/%s", image_dir, archive->name);
		status = pg_source_open(context, path, NULL, &source, NULL);
		if (status != PG_OK) { result = 0; continue; }
		if (validate_piggs && pg_source_validate(source, NULL) != PG_OK) result = 0;
		status = pg_source_files(source, NULL, &cursor, NULL);
		if (status != PG_OK) { result = 0; pg_source_close(&source, NULL); continue; }
		while ((status = pg_cursor_next(cursor, &selected, NULL)) == PG_OK) {
			pg_file_info info;
			FileCheck *file;
			pg_file_inspect(selected, &info, NULL);
			if (info.logical_size > UINT32_MAX) {
				result = 0;
				pg_file_close(&selected, NULL);
				continue;
			}
			file = checksumAddFile(image, info.canonical_name, archive->name, 1);
			file->full.size = info.logical_size;
			file->pig_name = strdup(archive->name);
			file->timestamp = info.mtime;
			file->full_status = archive->full_status;
			file->is_compressed = info.encoding == PG_ZLIB;
			if (info.digest_kind == PG_DIGEST_MD5 &&
				info.checksum_domain == PG_CHECKSUM_LOGICAL)
				memcpy(file->full.values, info.digest, sizeof(file->full.values));
			else {
				pg_reader *reader = NULL;
				unsigned char buffer[65536];
				size_t bytes;
				pg_status read_status = pg_reader_open(selected, PG_READ_LOGICAL,
					&reader, NULL);
				cryptMD5Init();
				while (read_status == PG_OK) {
					read_status = pg_reader_read(reader, buffer, sizeof(buffer), &bytes, NULL);
					if (read_status == PG_OK) cryptMD5Update(buffer, bytes);
				}
				if (reader) pg_reader_close(&reader, NULL);
				if (read_status != PG_END) { result = 0; file->full_status = 0; }
				else cryptMD5Final(file->full.values);
			}
			pg_file_close(&selected, NULL);
		}
		if (status != PG_END) result = 0;
		pg_cursor_close(&cursor, NULL);
		pg_source_close(&source, NULL);
	}
	pg_context_close(&context, NULL);
	image->pigs_opened = 1;
	return result;
}

int checksumLoadFromMem(char *mem,ImageCheck *image, int outputErrors)
{
    int            count,ret=0;
    char        *s,*args[100];
    FileCheck    *file = NULL;

    if (!mem)
        return 0;
    image->manifest_string = mem;
    s = mem = strdup(mem);
    while(count = tokenize_line(s,args,&s))
    {
        if (stricmp(args[0],"Version")==0)
        {
            if (atoi(args[1]) != CHECKSUMFILE_VERSION)
            {
                LOG_DEBUG( __FUNCTION__ ": exiting => atoi(args[1]=%s) != CHECKSUMFILE_VERSION=%i",args[1], CHECKSUMFILE_VERSION);
                goto exit;
            }
        }
        if (stricmp(args[0],"Build")==0)
        {
            image->build_name = strdup(args[1]);
            LOG_DEBUG( __FUNCTION__ ": image->build_name=%s", image->build_name);
        }
        else if (stricmp(args[0],"File")==0)
        {
            file = checksumAddFile(image,args[1],0,outputErrors);
            if (count >= 3)
            {
                file->pig_name = strdup(args[2]);
                LOG_DEBUG( __FUNCTION__ ": file->pig_name = %s", file->pig_name );
            }
        }
        else if (stricmp(args[0],"Time")==0)
        {
            file->timestamp = strtoul(args[1],0,10);
            if (file->timestamp > image->timestamp)
                image->timestamp = file->timestamp;
        }
        else if (stricmp(args[0],"Full")==0)
        {
            int        i;

            file->full.size = strtoul(args[1],0,10);
            image->bytecount += file->full.size;
            for(i=0;i<count-2;i++)
                file->full.values[i] = strtoul(args[i+2],0,16);
        }
        else if (stricmp(args[0],"Pack")==0)
        {
            file->data_pack_size = strtoul(args[1],0,10);
        }
        else if (stricmp(args[0],"Part")==0)
        {
            Checksum    *check;
            int            i;

            check = dynArrayAdd(&file->partials,sizeof(file->partials[0]),&file->partial_count,&file->partial_max,1);
            check->size = strtoul(args[1],0,10);
            for(i=0;i<4;i++)
                check->values[i] = strtoul(args[i+2],0,16);
        }
    }
    ret = 1;
    exit:
    free(mem);
    LOG_DEBUG( __FUNCTION__ ": return %i", ret);
    return ret;
}

void checksumFree(ImageCheck *image)
{
    int            i;
    FileCheck    *file;

    for(i=0;i<image->file_count;i++)
    {
        file = image->files[i];
        free(file->name);
        free(file->pig_name);
        free(file);
    }
    if (image->filenames)
        stashTableDestroy(image->filenames);
    free(image->build_name);
    free(image->fname);
    free(image->files);
    memset(image,0,sizeof(*image));
}

int checksumLoad(char *checksum_name,ImageCheck *image)
{
    char    *mem;
    int        len,ret;

    mem = loadNativeFile(checksum_name,&len);
    ret = checksumLoadFromMem(mem,image,1);
    if (!ret)
        free(mem);
    return ret;
}

int checksumMatch(const Checksum *a,const Checksum *b)
{
    if (a->size != b->size)
        return 0;
    if (!a->values[1] || !b->values[1])
        return a->values[0] == b->values[0];
    return memcmp(a->values,b->values,sizeof(a->values))==0;
}

ImageCheck *matchImage(ImageCheck **images,int image_count,ImageCheck *client,char *err_msg)
{
    int            i,j,build_matched=0;

    for(i=0;i<image_count;i++)
    {
        if (client->build_name && stricmp(images[i]->build_name,client->build_name)!=0)
            continue;
        build_matched=1;
        for(j=0;j<client->file_count && j<images[i]->file_count;j++)
        {
            if (!checksumMatch(&client->files[j]->full,&images[i]->files[j]->full))
            {
                if (client->build_name)
                {
                    sprintf(err_msg,"client file %s does not match server file %s (build %s)",client->files[j]->name,images[i]->files[j]->name,client->build_name);
                    return 0;
                }
                break;
            }
        }
        if (j == client->file_count)
        {
            if (images[i]->file_count == j || images[i]->files[j]->pig_name)
            return images[i];
        }
    }
    if (!build_matched)
        sprintf(err_msg,"Server doesn't have build %s",client->build_name);
    return 0;
}

static int timeMatch(U32 t1,U32 t2)
{
    int        dt;

    dt = t1-t2;
    if (ABS(dt)<=1)
        return 1;
    return 0;
}

int checksumVerify(char *image_dir,ImageCheck *image,ImageCheck *patch,ImageCheckType full_check,int check_pigs)
{
    int            i,failed_checksum=0;
    char        fname[MAX_PATH];
    FileCheck    *file,*patch_file=0;
    U32            file_size,file_time;
    S64            check_total=0;
    PigCache    cache = {0};

    LOG_DEBUG( __FUNCTION__ ": checksumVerify(image_dir=%s,patch->fname=%s,full_check=%i,check_pigs=%i",image_dir,patch?patch->fname:"<NULL patch>",full_check,check_pigs);

    //cacheQuickFileInfo(image_dir);
    if (!image->file_count)
    {
        LOG_DEBUG( __FUNCTION__ ": no image->file_count, returning");
        return 1;
    }
    
    for(i=0;i<image->file_count;i++)
    {
        file = image->files[i];
        LOG_DEBUG( __FUNCTION__ ": image->files[i]->pig_name=%s,files->name=%s", file->pig_name,file->name);

        if (check_pigs == !file->pig_name)
        {
            LOG_DEBUG( __FUNCTION__ ": checkpigs == !file->pig_name. continue");
            continue;
        }
        
        if (patch)
        {
            bool res = stashFindPointer(patch->filenames,file->name, &patch_file);
            if ( patch_file )
                LOG_DEBUG( __FUNCTION__ ": got patch from stash. has %i. patch_file->name=%s,patch_file->pig_name=%s", res, patch_file->name,patch_file->pig_name);
        }
        
        sprintf(fname,"%s/%s",image_dir,file->name);

        if (1)//project_list.slow_validate || !quickFileInfo(fname,&file_size,&file_time))
        {
            file_time = fileLastChangedAltStat(fname);
            file_size = safeFileSize(fname);
        }
        if (file_size == (U32)-1)
            file_size = 0;
        file->full_status = 0;
        if (fileExists(fname) && file_size == file->full.size && timeMatch(file_time,file->timestamp))
        {
            LOG_DEBUG( __FUNCTION__ ": file->full_stats==CHECKSUM_MATCHES_MANIFEST");
            file->full_status = CHECKSUM_MATCHES_MANIFEST;
        }
        else if (fileExists(fname) && patch_file && file_size == patch_file->full.size && timeMatch(file_time,patch_file->timestamp))
        {
            file->full_status = CHECKSUM_MATCHES_PATCH;
            LOG_DEBUG( LOGID, __FUNCTION__ ": file->full_stats==CHECKSUM_MATCHES_PATCH");
        }
        else if (!fileExists(fname) && patch && !patch_file)
        {
            file->full_status = CHECKSUM_MATCHES_PATCH;
            LOG_DEBUG( __FUNCTION__ ": file->full_stats==CHECKSUM_MATCHES_PATCH");
        }

//         file->full_status = 0;

        if (!file->full_status)
        {
            failed_checksum = 1;
            LOG_DEBUG( __FUNCTION__ ": failed_checksum=true");
        }
        if (full_check == CHECKSUM_ALWAYS)
        {
            file->full_status = 0;
            LOG_DEBUG( __FUNCTION__ ": cleared full_status");
        }
        if (!file->full_status && full_check != CHECKSUM_NEVER)
        {
            check_total += MAX(file_size,file->full.size);
            LOG_DEBUG( __FUNCTION__ ": no match, check_total=%i", check_total);
        }
    }
    if (!check_total)
    {
        LOG_DEBUG( __FUNCTION__ ": !check_total. return !failed_checksum = %i", !failed_checksum);
        return !failed_checksum;
    }
    

    failed_checksum = 0;
    if (check_pigs)
    {
        LOG_DEBUG( __FUNCTION__ ": xferStatsInit(\"StatsFindingBadFiles\",0,check_total=%i);", check_total);
        xferStatsInit("StatsFindingBadFiles",NULL,0,check_total);
    }
    else
    {
        LOG_DEBUG( __FUNCTION__ ": xferStatsInit(\"StatsVerifyingImage\",0,check_total=%i);", check_total);
        xferStatsInit("StatsVerifyingImage",NULL,0,check_total);
    }
    
    LOG_DEBUG( __FUNCTION__ ": image->file_count=%i",image->file_count);
    for(i=0;i<image->file_count;i++)
    {
        U8            *mem;
        U32            size;
        Checksum    check;

        file = image->files[i];
        sprintf(fname,"%s/%s",image_dir,file->name);
        LOG_DEBUG( __FUNCTION__ ": fname=%s",fname);

        if (check_pigs == !file->pig_name || file->full_status)
        {
            LOG_DEBUG( __FUNCTION__ ": continuing, file->pig_name=%s, file->full_status=%i", file->pig_name, file->full_status);
            continue;
        }
        
        if (patch)
            stashFindPointer(patch->filenames,file->name, &patch_file);
        file_size = safeFileSize(fname);
        if (file_size == (U32)-1)
            file_size = 0;
        mem = loadFileData(&cache,image_dir,file->name,file->pig_name,&size,0);
        if (mem)
        {
            check.size = size;
            checksumMem(mem,check.size,check.values);
            if (checksumMatch(&check,&file->full))
            {
                file_time = fileLastChangedAltStat(fname);
                if (file_time != file->timestamp && !check_pigs)
                    safeFileSetTimestamp(fname, file->timestamp);
                file->full_status = CHECKSUM_MATCHES_MANIFEST;
            }
            if (patch_file && checksumMatch(&check,&patch_file->full))
                file->full_status = CHECKSUM_MATCHES_PATCH;
            free(mem);
        }
        xferStatsUpdate(size);
        if (!file->full_status)
        {
            failed_checksum = 1;
            LOG_DEBUG( __FUNCTION__ ": failed checksum image_dir=%s,file->name=%s,file->pig_name=%s,size=%i",image_dir,file->name,file->pig_name,size);
        }
        
    }
    closeOpenPigs(&cache);
    xferStatsFinish();


    LOG_DEBUG( __FUNCTION__ ":returning, failed_checksum=%i",failed_checksum);

    return !failed_checksum;
}

int safeOverwriteFileAndChecksum(char *fname,char *how,void *mem,U32 len,Checksum *dst_check,int fatal)
{
    char    *s,tempname[MAX_PATH];
    Checksum    check;

    strcpy(tempname,fname);
    s = strrchr(tempname,'/');
    if (!s++)
        s = tempname;
    strcpy(s,"tempfile");
    safeWriteFile(tempname,how,mem,len);
    if (dst_check)
    {
        checksumFile(tempname,&check,0);
        if (!checksumMatch(dst_check,&check))
        {
            safeDeleteFile(tempname);
            if (!fatal)
            {
                fileMsgAlert("ErrFileChecksum",fname);
                return 0;
            }
            msgAlertFatal("ErrNewFileChecksum",fname);
        }
    }
    // if this file is the updater itself, it may have already been patched
    if (fileExists(fname) && dst_check)
    {
        checksumFile(fname,&check,0);
        if (checksumMatch(dst_check,&check))
            return 1;
    }
    safeRenameFile(tempname,fname);
    return 1;
}


int getPatchFileChecksum(char *fname, Checksum *check)
{
    FILE        *file;

    file = fopen(fname,"rb");
    if (!file)
        return 0;
    if (fread(check,1,sizeof(*check),file) != sizeof(*check))
    {
        fclose(file);
        return 0;
    }
    fclose(file);
    return 1;
}

