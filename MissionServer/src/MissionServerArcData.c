#include <utilitieslib/stdtypes.h>
#include "MissionServer.h"
#include "MissionServerArcData.h"

#include <utilitieslib/network/crypt.h>
#include <utilitieslib/utils/piglib.h>
#include <utilitieslib/utils/hoglib.h>
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

static HogFile* s_loadArcDataHogFile(U32 firstarc, int modifying)
{
    HogFile *hogfile;

    char hogname[MAX_PATH];
    sprintf(hogname, "%s/arcdata_%d-%d.hogg", g_missionServerState.dir, firstarc, firstarc+ARCS_PER_HOGG(firstarc)-1);

    if(modifying)
        s_backupHogg(hogname);

    mkdirtree(hogname);
    hogfile = hogFileReadOrCreate(hogname, NULL);
    if(!hogfile)
        FatalErrorf("Could not open or create %s", hogname);
    return hogfile;
}

#if MISSIONSERVER_CLOSE_HOGGS
static HogFile* s_getArcDataHogFile(U32 arcid, int modifying)
{
    return s_loadArcDataHogFile(FIRSTARCID(arcid), modifying);
}

void missionserver_FlushAllArcData(void)
{
    // do nothing, all hogs should already be destroyed
}
#else
static StashTable s_hogfiles;

static HogFile* s_getArcDataHogFile(U32 arcid, int modifying)
{
    HogFile *hogfile;
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

static void hogFileDestroyAdapter(void* arg0)
{
    hogFileDestroy((HogFile *)arg0);
}

void missionserver_FlushAllArcData(void)
{
    if (s_hogfiles)
        stashTableClearEx(s_hogfiles, NULL, hogFileDestroyAdapter);
}
#endif

static HogFileIndex s_getArcDataHogFileIndex(HogFile *hogfile, U32 arcid, char *arcname, size_t arcname_size)
{
    char buf[MAX_PATH];
    if(!arcname)
    {
        arcname = buf;
        arcname_size = ARRAY_SIZE(buf);
    }
    sprintf_s(arcname, arcname_size, "arcdata_%d.txt", arcid);
    return hogFileFind(hogfile, arcname);
}

int missionserver_FindArcData(MissionServerArc *arc)
{
    HogFile *hogfile = s_getArcDataHogFile(arc->id, 0);
    HogFileIndex hogfileindex = s_getArcDataHogFileIndex(hogfile, arc->id, NULL, 0);
    if(MISSIONSERVER_CLOSE_HOGGS)
        hogFileDestroy(hogfile);
    return hogfileindex != HOG_INVALID_INDEX;
}

static void s_writeArcToHogg(MissionServerArc *arc, const char *arcname, 
                              HogFile *hogfile, HogFileIndex hogfileindex)
{
    // this requires a HogFileIndex, so the caller can deal with logging
    int result;
    NewPigEntry entry = {0};
    entry.fname = strdup(arcname);
    entry.data = malloc(arc->zsize);
    memcpy(entry.data, arc->data, arc->zsize);
    entry.size = arc->size;
    entry.pack_size = arc->zsize;
    entry.dont_pack = 0;
    entry.must_pack = 1;
    entry.checksum[0] = s_checksum(arc->data, arc->zsize); // this is a hack, the checksum is on the zipped data!
    entry.timestamp = 0;

    if(hogfileindex == HOG_INVALID_INDEX)
    {
        if(result = hogFileModifyAdd2(hogfile, &entry))
            FatalErrorf("hog error %d when adding arc %d to %s", result, arc->id, hogFileGetArchiveFileName(hogfile));
    }
    else
    {
        if(result = hogFileModifyUpdate2(hogfile, hogfileindex, &entry, false))
            FatalErrorf("hog error %d when updating arc %d in %s", result, arc->id, hogFileGetArchiveFileName(hogfile));
    }
}

void missionserver_UpdateArcData(MissionServerArc *arc, U8 *data)
{
    char arcname[MAX_PATH];
    HogFile *hogfile = s_getArcDataHogFile(arc->id, 1);
    HogFileIndex hogfileindex = s_getArcDataHogFileIndex(hogfile, arc->id, SAFESTR(arcname));

    assert(data);
    SAFE_FREE(arc->data);
    arc->data = data;

    s_writeArcToHogg(arc, arcname, hogfile, hogfileindex);

    #if MISSIONSERVER_CLOSE_HOGGS
        hogFileDestroy(hogfile);
    #endif
}

HogFile* missionserver_LoadArcDataInternal(MissionServerArc *arc, int *changed)
{
    U32 count;
    HogFile *hogfile = s_getArcDataHogFile(arc->id, 0);
    HogFileIndex hogfileindex = s_getArcDataHogFileIndex(hogfile, arc->id, NULL, 0);

    arc->data = hogFileExtractCompressed(hogfile, hogfileindex, &count);
    if(!arc->data)
    {
        if(!g_missionServerState.forceLoadDb)
            FatalErrorf("failed to extract arc %d in %s", arc->id, hogFileGetArchiveFileName(hogfile));
        else
        {
            LOG( LOG_MISSIONSERVER, LOG_LEVEL_VERBOSE, 0, "Arc %d not found in hogg!  Removing.", arc->id);
            if(changed)
                *changed = 1;
            return NULL;
        }
    }
    if(count != arc->zsize)
    {
        if(!g_missionServerState.forceLoadDb)
            FatalErrorf("read wrong number of bytes %d when reading arc %d in %s", count, arc->id, hogFileGetArchiveFileName(hogfile));
        else
        {
            LOG( LOG_MISSIONSERVER, LOG_LEVEL_VERBOSE, 0, "discrepancy in arc %d (%d bytes expected, %d found).  Changing expectations.", arc->id, arc->zsize, count);
            arc->zsize = count;
            if(changed)
                *changed = 1;
        }
    }

    // this is a hack, we've stored the checksum of the zipped data!
    if(hogFileGetFileChecksum(hogfile, hogfileindex) != s_checksum(arc->data, arc->zsize))
        FatalErrorf("bad checksum when reading arc %d in %s", arc->id, hogFileGetArchiveFileName(hogfile));

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
    HogFile *lasthog = NULL;
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
            HogFile *hogfile = missionserver_LoadArcDataInternal(arcs[i], &changed);
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
                if(hogfile && lasthog && hogfile != lasthog)
                {
                    hogFileDestroy(lasthog);
                    lasthog = hogfile;
                }
            #endif
        }
    }

    #if MISSIONSERVER_CLOSE_HOGGS
        if(lasthog)
            hogFileDestroy(lasthog);
    #endif
    Str_destroy(&buf);
#endif
}

void missionserver_RestoreArcData(MissionServerArc *arc)
{
    if(!arc->data)
    {
        HogFile *hogfile = missionserver_LoadArcDataInternal(arc, NULL);
        #if MISSIONSERVER_CLOSE_HOGGS
            hogFileDestroy(hogfile);
        #endif
    }
}

static HogFile *s_hogfile_archive;

static HogFile* s_getArchiveHogg(void)
{
    static int reentry = 0;
    reentry++;

    if(!s_hogfile_archive)
    {
        char hogname[MAX_PATH];
        sprintf(hogname, "%s/arcdata_deleted.hogg", g_missionServerState.dir);

        s_backupHogg(hogname);

        mkdirtree(hogname);
        s_hogfile_archive = hogFileReadOrCreate(hogname, NULL);
        if(!s_hogfile_archive)
            FatalErrorf("Could not open or create %s", hogname);
    }

    if(hogFileGetNumFiles(s_hogfile_archive) >= ARCS_PER_HOGG(INT_MAX))
    {
        char hogname[MAX_PATH];
        char bakname[MAX_PATH];
        U32 timess2000;
        struct tm time = {0};
        int retry = 0;
        assert(reentry == 1); // can reenter once, after the hogg fills, don't overwrite anything

        strcpy(hogname, hogFileGetArchiveFileName(s_hogfile_archive));
        hogFileDestroy(s_hogfile_archive);
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

    HogFile *hogfile;
    HogFileIndex hogfileindex;

    // make sure we've loaded the old data
    if(!arc->data)
        missionserver_LoadArcDataInternal(arc, NULL);
    assert(arc->data);

    // first put the data in the backup hogg
    hogfile = s_getArchiveHogg();
    hogfileindex = s_getArcDataHogFileIndex(hogfile, arc->id, SAFESTR(arcname));
    if(hogfileindex != HOG_INVALID_INDEX)
        LOG( LOG_MISSIONSERVER, LOG_LEVEL_VERBOSE, 0, "Warning: found an archived arc with the same id (%d) while archiving, overwriting.", arc->id);
    s_writeArcToHogg(arc, arcname, hogfile, hogfileindex);

    // then remove it from the live hogg
    hogfile = s_getArcDataHogFile(arc->id, 1);
    hogfileindex = s_getArcDataHogFileIndex(hogfile, arc->id, SAFESTR(arcname));
    if(hogfileindex != HOG_INVALID_INDEX)
    {
        if(result = hogFileModifyDelete(hogfile, hogfileindex))
            LOG( LOG_MISSIONSERVER, LOG_LEVEL_VERBOSE, 0, "Warning: hog error %d when deleting arc %d from %s, ignoring", result, arc->id, hogFileGetArchiveFileName(hogfile));
    }
    else
    {
        LOG( LOG_MISSIONSERVER, LOG_LEVEL_VERBOSE, 0, "Warning: could not find arc %d in %s for deletion, ignoring.", arc->id, hogFileGetArchiveFileName(hogfile));
    }
    #if MISSIONSERVER_CLOSE_HOGGS
        hogFileDestroy(hogfile);
    #endif

    // then let it go free
    SAFE_FREE(arc->data);
}

void missionserver_CloseArcDataArchive(void)
{
    if(s_hogfile_archive)
    {
        hogFileDestroy(s_hogfile_archive);
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
        HogFile *hogfile = missionserver_LoadArcDataInternal(arc, NULL);
        #if MISSIONSERVER_CLOSE_HOGGS
            hogFileDestroy(hogfile);
        #endif
    }

    return arc->data;
}
