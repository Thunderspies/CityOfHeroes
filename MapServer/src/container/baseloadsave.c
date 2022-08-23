#include "dbcomm/dbcomm.h"
#include <utilitieslib/network/netio.h>
#include <utilitieslib/utils/utils.h>
#include "comm_backend.h"
#include <utilitieslib/components/EString.h>
#include "group/groupfilesave.h"
#include <utilitieslib/utils/timing.h>
#include "cmdparse/cmdserver.h"
#include <utilitieslib/utils/file.h>
#include <utilitieslib/utils/timing.h>
#include "container/dbcontainerpack.h"
#if !DBQUERY
#include "bases/bases.h"
#include "bases/baseparse.h"
#endif

typedef struct DBBase {
    int supergroupid;
    int userid;
    char *ziptext;
    char *oldtext; // deprecated
} DBBase;

LineDesc base_line_desc[] =
{
    // Each entry descibes a player base
    // There can only be one base per supergroup or player

    {{ PACKTYPE_CONREF, CONTAINER_ENTS,            "UserId",        OFFSET(DBBase, userid),            INOUT(0,0), LINEDESCFLAG_INDEXEDCOLUMN    },
        "DB ID - The character ID who owns the base, or zero if it's a supergroup base"},

    {{ PACKTYPE_CONREF, CONTAINER_SUPERGROUPS,    "SupergroupId",    OFFSET(DBBase, supergroupid),    INOUT(0,0), LINEDESCFLAG_INDEXEDCOLUMN    },
        "DB ID - The Supergroup ID who owns the base, or zero if it's a personal hideout"},

    {{ PACKTYPE_TEXTBLOB, 0,                    "Data",            OFFSET(DBBase, oldtext)                                        },
        "Compressed base data (old format)"},

    {{ PACKTYPE_LARGE_ESTRING_BINARY, 0,        "ZipData",        OFFSET(DBBase, ziptext)                                        },
        "Compressed base data"},

    { 0 },
};

StructDesc base_desc[] =
{
    0,
    {AT_NOT_ARRAY,{0}},
    base_line_desc
};

int s_findBase(int supergroupid, int userid)
{
    char value[20];
    int id;

    assert(!supergroupid ^ !userid); // supergroupid ^^ userid (logical xor)

    if(supergroupid)
    {
        sprintf(value, "%d", supergroupid);
        id = dbSyncContainerFindByElement(CONTAINER_BASE, "SupergroupId", value, NULL, 0);
    }
    else
    {
        sprintf(value, "%d", userid);
        id = dbSyncContainerFindByElement(CONTAINER_BASE, "UserId", value, NULL, 0);
    }

    return id <= 0 ? 0 : id;
}

static void s_saveBase(int id, int supergroupid, int userid, char *text)
{
    char *data;
    DBBase base = {0};
    int ret;

    assert(id > 0);

    base.userid = userid;
    base.supergroupid = supergroupid;
    if(text)
    {
        base.ziptext = estrTemp();
        estrPackStr2(&base.ziptext, text);
    }

    ret = dbSyncContainerRequest(CONTAINER_BASE, id, CONTAINER_CMD_LOCK_AND_LOAD, 1);
    assertmsg(ret, "dbSyncContainerRequest failed in s_saveBase");

    data = dbContainerPackage(base_desc, &base);
    dbAsyncContainerUpdate(CONTAINER_BASE, id, CONTAINER_CMD_UNLOCK, data, 0);
    free(data);

    estrDestroy(&base.ziptext);
}

static int s_findOrAddBase(int supergroupid, int userid)
{
    int id;

    // try to find it
    id = s_findBase(supergroupid, userid);

    if(!id)
    {
        // couldn't find it, create it

        // note that there's a race condition here where two bases could be
        // created for the same owners. this is exacerbated by the fact that
        // dbSyncContainerFindByElement() does a foreground SQL query whereas
        // db*ContainerUpdate() does a background SQL query, which
        // means that two rapid calls to s_findOrAddBase() (even on the same
        // map) could both decide to create bases if the FindByElement SQL
        // executes before the Update SQL.
        dbSyncContainerCreate(CONTAINER_BASE, &id);
        s_saveBase(id, supergroupid, userid, NULL);
    }

    return id;
}

static DBBase s_requested_base;

static int s_processBase(Packet *pak, int cmd, NetLink *link)
{
    ContainerInfo    ci;

    int list_id = pktGetBitsPack(pak,1);
    int count = pktGetBitsPack(pak,1);

    assert(list_id == CONTAINER_BASE && count == 1);
    if(!dbReadContainer(pak, &ci, list_id))
        return false;

    dbContainerUnpack(base_desc, ci.data, &s_requested_base);
    return true;
}

static void s_loadBase(int id, int *supergroupid, int *userid, char **text)
{
    memset(&s_requested_base, 0, sizeof(s_requested_base));
    dbSyncContainerRequestCustom(CONTAINER_BASE, id, CONTAINER_CMD_TEMPLOAD, s_processBase);

    if(s_requested_base.oldtext)
    {
        // this is ugly, but it keeps the rest of the function nice
        char *text = estrTemp();

        if(!s_requested_base.ziptext)
            estrPackStr2(&s_requested_base.ziptext, unescapeAndUnpack(s_requested_base.oldtext));
        // else we already have new data in there somehow, just delete the old data
        SAFE_FREE(s_requested_base.oldtext);

        estrUnpackStr(&text, &s_requested_base.ziptext);
        s_saveBase(id, s_requested_base.supergroupid, s_requested_base.userid, text);
        estrDestroy(&text);
    }

    if(supergroupid)
        *supergroupid = s_requested_base.supergroupid;
    if(userid)
        *userid = s_requested_base.userid;
    if(text)
        estrUnpackStr(text, &s_requested_base.ziptext);

    estrDestroy(&s_requested_base.ziptext);
}

void baseSaveText(int supergroupid, int userid, char *text)
{
    s_saveBase(s_findOrAddBase(supergroupid, userid), supergroupid, userid, text);
}

char* baseLoadText(int supergroupid, int userid)
{
    int id = s_findBase(supergroupid, userid);
    if(id)
    {
        static char *text = NULL;

        int sgtest, utest;

        estrClear(&text);
        s_loadBase(id, &sgtest, &utest, &text);
        assert(sgtest == supergroupid && utest == userid);
        return text ? text : "";
    }
    else
    {
        return "";
    }
}

#if !DBQUERY

static int dbmap_id;

char *baseLoadMapFromDb(int supergroupid,int userid)
{
    if(    db_state.map_data &&
        supergroupid == db_state.map_supergroupid &&
        userid == db_state.map_userid )
    {
        return db_state.map_data;
    }

    // hack for loading a reasonable raiding base
    if (server_state.dummy_raid_base)
    {
        int bufsize = 200000;
        int re;
        char* buf;
        FILE* f;

        db_state.map_supergroupid = supergroupid;
        db_state.map_userid = userid;

        f = fileOpen("maps/_testing/baseraid.txt", "rt");
        if (f)
        {
            buf = calloc(bufsize, 1);    // go with 200k for now
            re = fread(buf, 1, bufsize, f);
            if (re != bufsize && re != 0 && re != EOF)
            {
                buf[re] = 0;
                estrPrintCharString(&db_state.map_data, buf);
                free(buf);
                fileClose(f);
                return db_state.map_data;
            }
            fileClose(f);
        }
        Errorf("!! couldn't read from baseraid.txt");
        estrDestroy(&db_state.map_data);
    }

    db_state.map_supergroupid = supergroupid;
    db_state.map_userid = userid;
    dbmap_id = s_findOrAddBase(supergroupid, userid);
    s_loadBase(dbmap_id, NULL, NULL, &db_state.map_data);
    return db_state.map_data ? db_state.map_data : "";
}

void baseSaveMapToDb(void)
{
    if (!db_state.map_supergroupid && !db_state.map_userid)
        return;

    estrClear(&db_state.map_data);
    if (!baseToStr(&db_state.map_data,&g_base))
        return;

    // @testing-remove ab: write out the base
    if (0 && isDevelopmentMode()) 
    {
        char tmp[MAX_PATH];
        FILE* f;
        sprintf(tmp,"c:\\base_%i.txt", g_base.curr_id);
        f = fopen(tmp,"wt");
        //f = fopen("c:\\base.txt", "wt");
        if (f) {
            fwrite(db_state.map_data, 1, estrLength(&db_state.map_data), f);
            fclose(f);
        }
    }

    s_saveBase(dbmap_id, db_state.map_supergroupid, db_state.map_userid, db_state.map_data);
}

#endif
