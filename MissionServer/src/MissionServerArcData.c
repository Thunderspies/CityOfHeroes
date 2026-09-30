#include <utilitieslib/stdtypes.h>
#include "MissionServer.h"
#include "MissionServerArcData.h"

#include <utilitieslib/network/crypt.h>
#include <piggle/piggle.h>
#include <utilitieslib/utils/file.h>
#include <utilitieslib/components/StashTable.h>
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/error.h>
#include <utilitieslib/utils/log.h>
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/UtilsNew/Str.h>  //to write the % complete for arc loading
#include <utilitieslib/components/earray.h>
#include "persist/persist.h"

#define ARCS_PER_HOGG(arcid)    ((arcid) < 300000 ? 1000 : 20000)                    // max arcs to store in each hogg
#define FIRSTARCID(arcid)        ((arcid)/ARCS_PER_HOGG(arcid)*ARCS_PER_HOGG(arcid))    // first arcid in the same hogg as the given arc

static U32 s_checksum(U8 *data, int size)
{
    if(size)
    {
        U32 hash[4];
        cryptMD5Init();
        cryptMD5Update(data, size);
        cryptMD5Final(hash);
        return hash[0];
    }
    else
    {
        return 0;
    }
}

static pg_context *arc_context;

static const char *arcArchiveName(pg_source *source)
{
	pg_source_info info;
	return pg_source_inspect(source, &info, NULL) == PG_OK ? info.native_path : "<unavailable>";
}

static pg_source *openArcArchive(const char *path)
{
	pg_source *source = NULL;
	pg_source_options options = { PG_HOGG10, PG_WRITE, PG_CHECKSUM_STORED };
	pg_error error;
	pg_status status;
	if (!arc_context && pg_context_open(&arc_context, &error) != PG_OK)
		FatalErrorf("Cannot initialize arc archives: %s", error.message);
	status = pg_source_open(arc_context, path, &options, &source, &error);
	if (status == PG_RECOVERY_REQUIRED) {
		status = pg_source_recover(arc_context, path, &error);
		if (status == PG_OK)
			status = pg_source_open(arc_context, path, &options, &source, &error);
	}
	if (status == PG_NOT_FOUND) {
		pg_archive_builder *builder = NULL;
		pg_archive_options create = { PG_HOGG10, 0, PG_CHECKSUM_STORED };
		status = pg_archive_builder_create_options(arc_context, path, &create,
			&builder, &error);
		if (status == PG_OK) status = pg_archive_builder_finish(builder, &error);
		if (builder) pg_archive_builder_close(&builder, NULL);
		if (status == PG_OK)
			status = pg_source_open(arc_context, path, &options, &source, &error);
	}
	if (status != PG_OK) FatalErrorf("Cannot open arc archive %s: %s", path, error.message);
	return source;
}

static size_t arcArchiveCount(pg_source *source)
{
	pg_cursor *cursor = NULL;
	pg_file *file = NULL;
	pg_status status = pg_source_files(source, NULL, &cursor, NULL);
	size_t count = 0;
	if (status != PG_OK) FatalErrorf("Cannot enumerate %s", arcArchiveName(source));
	while ((status = pg_cursor_next(cursor, &file, NULL)) == PG_OK) {
		count++;
		pg_file_close(&file, NULL);
	}
	pg_cursor_close(&cursor, NULL);
	if (status != PG_END) FatalErrorf("Cannot enumerate %s", arcArchiveName(source));
	return count;
}

static U8 *readArcStored(pg_file *file, U32 *count)
{
	pg_file_info info;
	pg_reader *reader = NULL;
	pg_status status;
	U8 *data;
	size_t total = 0, bytes;
	*count = 0;
	if (!file || pg_file_inspect(file, &info, NULL) != PG_OK ||
		info.stored_size > INT_MAX) return NULL;
	data = malloc((size_t)info.stored_size + 1);
	if (!data) return NULL;
	status = pg_reader_open(file, PG_READ_STORED, &reader, NULL);
	while (status == PG_OK) {
		status = pg_reader_read(reader, data + total,
			(size_t)info.stored_size + 1 - total, &bytes, NULL);
		total += bytes;
	}
	if (reader) pg_reader_close(&reader, NULL);
	if (status != PG_END) { free(data); return NULL; }
	*count = (U32)total;
	return data;
}

static void s_backupHogg(const char *hogname)
{
    #if MISSIONSERVER_BACKUP_HOGGS
        if(fileExists(hogname))
        {
            char bakname[MAX_PATH];
            sprintf(bakname, "%s.bak", hogname);
            if(fileExists(bakname))
                fileForceRemove(bakname);
            if(fileCopy(hogname, bakname))
                FatalErrorf("Could not copy %s to %s for backup", hogname, bakname);
        }
    #endif
}

static pg_source* s_loadArcDataHogFile(U32 firstarc, int modifying)
{
    pg_source *hogfile;

    char hogname[MAX_PATH];
    sprintf(hogname, "%s/arcdata_%d-%d.hogg", g_missionServerState.dir, firstarc, firstarc+ARCS_PER_HOGG(firstarc)-1);

    if(modifying)
        s_backupHogg(hogname);

    mkdirtree(hogname);
    hogfile = openArcArchive(hogname);
    if(!hogfile)
        FatalErrorf("Could not open or create %s", hogname);
    return hogfile;
}

#if MISSIONSERVER_CLOSE_HOGGS
static pg_source* s_getArcDataHogFile(U32 arcid, int modifying)
{
    return s_loadArcDataHogFile(FIRSTARCID(arcid), modifying);
}

void missionserver_FlushAllArcData(void)
{
    // do nothing, all hogs should already be destroyed
}
#else
static StashTable s_hogfiles;

static pg_source* s_getArcDataHogFile(U32 arcid, int modifying)
{
    pg_source *hogfile;
    int firstarc = FIRSTARCID(arcid);

    if(!s_hogfiles)
        s_hogfiles = stashTableCreateInt(0);

    if(!stashIntFindPointer(s_hogfiles, firstarc+1, &hogfile))
    {
        hogfile = s_loadArcDataHogFile(firstarc, modifying);
        assert(stashIntAddPointer(s_hogfiles, firstarc+1, hogfile, false));
    }

    return hogfile;
}

static void closeArcArchiveAdapter(void* arg0)
{
    pg_source *source = arg0;
    pg_source_close(&source, NULL);
}

void missionserver_FlushAllArcData(void)
{
    if (s_hogfiles)
        stashTableClearEx(s_hogfiles, NULL, closeArcArchiveAdapter);
}
#endif

static pg_file * s_getArcDataHogFileIndex(pg_source *hogfile, U32 arcid, char *arcname, size_t arcname_size)
{
    char buf[MAX_PATH];
    if(!arcname)
    {
        arcname = buf;
        arcname_size = ARRAY_SIZE(buf);
    }
    sprintf_s(arcname, arcname_size, "arcdata_%d.txt", arcid);
    pg_file *file = NULL;
    pg_source_find(hogfile, arcname, &file, NULL);
    return file;
}

int missionserver_FindArcData(MissionServerArc *arc)
{
    pg_source *hogfile = s_getArcDataHogFile(arc->id, 0);
    pg_file * hogfileindex = s_getArcDataHogFileIndex(hogfile, arc->id, NULL, 0);
    if(MISSIONSERVER_CLOSE_HOGGS)
        pg_source_close(&hogfile, NULL);
    {
        int found = hogfileindex != NULL;
        pg_file_close(&hogfileindex, NULL);
        return found;
    }
}

static void s_writeArcToHogg(MissionServerArc *arc, const char *arcname,
	pg_source *source, pg_file *existing)
{
	pg_write_options options;
	pg_writer *writer = NULL;
	pg_status status;
	pg_error error;
	size_t bytes;
	U32 digest = s_checksum(arc->data, arc->zsize);
	pg_write_options_init(&options, arc->size);
	options.input_size = arc->zsize;
	options.encoding = PG_ZLIB;
	options.entry.compression = PG_COMPRESS_FORCE;
	options.entry.digest_kind = PG_DIGEST_MD5_32;
	options.entry.expected_digest_domain = PG_CHECKSUM_STORED;
	memcpy(options.entry.expected_digest, &digest, sizeof(digest));
	status = existing ? pg_writer_open_file(existing, &options, &writer, &error) :
		pg_writer_open_source(source, arcname, &options, &writer, &error);
	if (status == PG_OK)
		status = pg_writer_write(writer, arc->data, arc->zsize, &bytes, &error);
	if (status == PG_OK) status = pg_writer_finish(writer, &error);
	if (writer) pg_writer_close(&writer, NULL);
	pg_file_close(&existing, NULL);
	if (status != PG_OK)
		FatalErrorf("Cannot write arc %d to %s: %s", arc->id,
			arcArchiveName(source), error.message);
}

void missionserver_UpdateArcData(MissionServerArc *arc, U8 *data)
{
    char arcname[MAX_PATH];
    pg_source *hogfile = s_getArcDataHogFile(arc->id, 1);
    pg_file * hogfileindex = s_getArcDataHogFileIndex(hogfile, arc->id, SAFESTR(arcname));

    assert(data);
    SAFE_FREE(arc->data);
    arc->data = data;

    s_writeArcToHogg(arc, arcname, hogfile, hogfileindex);

    #if MISSIONSERVER_CLOSE_HOGGS
        pg_source_close(&hogfile, NULL);
    #endif
}

pg_source* missionserver_LoadArcDataInternal(MissionServerArc *arc, int *changed)
{
    U32 count;
    pg_source *hogfile = s_getArcDataHogFile(arc->id, 0);
    pg_file * hogfileindex = s_getArcDataHogFileIndex(hogfile, arc->id, NULL, 0);

    arc->data = readArcStored(hogfileindex, &count);
    pg_file_close(&hogfileindex, NULL);
    if(!arc->data)
    {
        if(!g_missionServerState.forceLoadDb)
            FatalErrorf("failed to extract arc %d in %s", arc->id, arcArchiveName(hogfile));
        else
        {
            LOG( LOG_MISSIONSERVER, LOG_LEVEL_VERBOSE, 0, "Arc %d not found in hogg!  Removing.", arc->id);
            if(changed)
                *changed = 1;
            if (MISSIONSERVER_CLOSE_HOGGS)
                pg_source_close(&hogfile, NULL);
            return NULL;
        }
    }
    if(count != arc->zsize)
    {
        if(!g_missionServerState.forceLoadDb)
            FatalErrorf("read wrong number of bytes %d when reading arc %d in %s", count, arc->id, arcArchiveName(hogfile));
        else
        {
            LOG( LOG_MISSIONSERVER, LOG_LEVEL_VERBOSE, 0, "discrepancy in arc %d (%d bytes expected, %d found).  Changing expectations.", arc->id, arc->zsize, count);
            arc->zsize = count;
            if(changed)
                *changed = 1;
        }
    }

    // Piggle verifies the stored-byte checksum through EOF before publishing data.

    return hogfile;
}

static MissionServerArc **s_arcdata_missingArcs = 0;
static MissionServerArc **s_arcdata_changedArcs = 0;

void missionserver_getLoadMismatches(MissionServerArc ***removed, MissionServerArc ***changed)
{
    *removed = s_arcdata_missingArcs;
    *changed = s_arcdata_changedArcs;
}

void missionserver_clearLoadMismatches()
{
    eaDestroy(&s_arcdata_missingArcs);
    eaDestroy(&s_arcdata_changedArcs);
}

void missionserver_LoadAllArcData(MissionServerArc **arcs, int count)
{
#if MISSIONSERVER_LOADALL_HOGGS
    int i, percent = -1;
    char *buf = Str_temp();
    for(i = 0; i < count; i++)
    {
        if(i*100/count != percent) // by time would probably be better :P
        {
            percent = i*100/count;
            Str_printf(&buf, "%d: MissionServer. Loading Arcs: arc %d (%d). %d %%", _getpid(), i, count, percent);
            setConsoleTitle(buf);
        }
        if(!arcs[i]->unpublished)
        {
            int changed = 0;
            pg_source *hogfile = missionserver_LoadArcDataInternal(arcs[i], &changed);
            if(changed)
            {
                eaPush(&s_arcdata_changedArcs, arcs[i]);
            }

            if(!hogfile)
            {
                assert(g_missionServerState.forceLoadDb);    //we better be loading from a backup
                eaPush(&s_arcdata_missingArcs, arcs[i]);
            }
            #if MISSIONSERVER_CLOSE_HOGGS
                if (hogfile) pg_source_close(&hogfile, NULL);
            #endif
        }
    }

    Str_destroy(&buf);
#endif
}

void missionserver_RestoreArcData(MissionServerArc *arc)
{
    if(!arc->data)
    {
        pg_source *hogfile = missionserver_LoadArcDataInternal(arc, NULL);
        #if MISSIONSERVER_CLOSE_HOGGS
            pg_source_close(&hogfile, NULL);
        #endif
    }
}

static pg_source *s_hogfile_archive;

static pg_source* s_getArchiveHogg(void)
{
    static int reentry = 0;
    reentry++;

    if(!s_hogfile_archive)
    {
        char hogname[MAX_PATH];
        sprintf(hogname, "%s/arcdata_deleted.hogg", g_missionServerState.dir);

        s_backupHogg(hogname);

        mkdirtree(hogname);
        s_hogfile_archive = openArcArchive(hogname);
        if(!s_hogfile_archive)
            FatalErrorf("Could not open or create %s", hogname);
    }

    if(arcArchiveCount(s_hogfile_archive) >= ARCS_PER_HOGG(INT_MAX))
    {
        char hogname[MAX_PATH];
        char bakname[MAX_PATH];
        U32 timess2000;
        struct tm time = {0};
        int retry = 0;
        assert(reentry == 1); // can reenter once, after the hogg fills, don't overwrite anything

        strcpy(hogname, arcArchiveName(s_hogfile_archive));
        pg_source_close(&s_hogfile_archive, NULL);
        s_hogfile_archive = NULL;

        timess2000 = timerSecondsSince2000();
        timerMakeTimeStructFromSecondsSince2000(timess2000, &time);
        sprintf(bakname, "%s/arcdata_deleted_%d_%.4i%.2i%.2i%.2i%.2i%.2i.hogg", g_missionServerState.dir, timess2000,
                time.tm_year+1900, time.tm_mon+1, time.tm_mday, time.tm_hour, time.tm_min, time.tm_sec);

        while(rename(hogname, bakname))
        {
            char buf[1024];
            strerror_s(SAFESTR(buf), errno);
            Sleep(1000);
            if(++retry > 5)
                FatalErrorf("Could not move arc data archive from %s to %s", hogname, bakname);
            LOG( LOG_MISSIONSERVER, LOG_LEVEL_VERBOSE, 0,"moving %s to %s (%s) (retry %i).", hogname, bakname, buf, retry);
        }

        s_getArchiveHogg();
    }

    assert(s_hogfile_archive);
    reentry--;
    return s_hogfile_archive;
}

void missionserver_ArchiveArcData(MissionServerArc *arc)
{
    int result;
    char arcname[MAX_PATH];

    pg_source *hogfile;
    pg_file * hogfileindex;

    // make sure we've loaded the old data
    if(!arc->data)
        missionserver_RestoreArcData(arc);
    assert(arc->data);

    // first put the data in the backup hogg
    hogfile = s_getArchiveHogg();
    hogfileindex = s_getArcDataHogFileIndex(hogfile, arc->id, SAFESTR(arcname));
    if(hogfileindex != NULL)
        LOG( LOG_MISSIONSERVER, LOG_LEVEL_VERBOSE, 0, "Warning: found an archived arc with the same id (%d) while archiving, overwriting.", arc->id);
    s_writeArcToHogg(arc, arcname, hogfile, hogfileindex);

    // then remove it from the live hogg
    hogfile = s_getArcDataHogFile(arc->id, 1);
    hogfileindex = s_getArcDataHogFileIndex(hogfile, arc->id, SAFESTR(arcname));
    if(hogfileindex != NULL)
    {
        if((result = pg_file_delete(hogfileindex, NULL)) != PG_OK)
            LOG( LOG_MISSIONSERVER, LOG_LEVEL_VERBOSE, 0, "Warning: hog error %d when deleting arc %d from %s, ignoring", result, arc->id, arcArchiveName(hogfile));
    }
    else
    {
        LOG( LOG_MISSIONSERVER, LOG_LEVEL_VERBOSE, 0, "Warning: could not find arc %d in %s for deletion, ignoring.", arc->id, arcArchiveName(hogfile));
    }
    pg_file_close(&hogfileindex, NULL);
    #if MISSIONSERVER_CLOSE_HOGGS
        pg_source_close(&hogfile, NULL);
    #endif

    // then let it go free
    SAFE_FREE(arc->data);
}

void missionserver_CloseArcDataArchive(void)
{
    if(s_hogfile_archive)
    {
        pg_source_close(&s_hogfile_archive, NULL);
        s_hogfile_archive = NULL;
    }
}

U8* missionserver_GetArcData(MissionServerArc *arc)
{
#if MISSIONSERVER_LOADALL_HOGGS
    assert(arc->unpublished || arc->data);
#endif

    if(!arc->data)
    {
        pg_source *hogfile = missionserver_LoadArcDataInternal(arc, NULL);
        #if MISSIONSERVER_CLOSE_HOGGS
            pg_source_close(&hogfile, NULL);
        #endif
    }

    return arc->data;
}
