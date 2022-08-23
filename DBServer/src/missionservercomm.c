#include "missionservercomm.h"
#include "comm_backend.h"
#include <utilitieslib/network/netio.h>
#include <utilitieslib/network/netcomp.h>
#include <utilitieslib/utils/error.h>
#include <utilitieslib/components/EString.h>
#include <utilitieslib/utils/timing.h>

#include "dbserver/servercfg.h"
#include "dbrelay.h"
#include "dbdispatch.h"
#include "entity/entVarUpdate.h"

#include "mission/missionserver_meta.h"
//#include "UtilsNew/meta.h"
#include "mission/MissionSearch.h"
#include <utilitieslib/utils/log.h>

static NetLinkList s_missionlinks = {0};

void missionserver_forceLinkReset(void)
{
    NetLink *missionlink = missionserver_getLink();
    if(missionlink)
    {
        printf("resetting connection to missionserver.\n");
        netRemoveLink(missionlink);
    }
}

NetLink* missionserver_getLink(void)
{
    return SAFE_MEMBER(s_missionlinks.links, size) ? s_missionlinks.links->storage[0] : NULL;
}

int missionserver_secondsSinceUpdate(void)
{
    int secs = 9999;
    NetLink *missionlink = missionserver_getLink();
    if(missionlink)
        return secs = timerCpuSeconds() - missionlink->lastRecvTime;
    return secs;
}

// *********************************************************************************
//  missionlink handler
// *********************************************************************************    

static int s_handleMissionPacket(Packet *pak_in, int cmd, NetLink *missionlink);
static int s_remoteMetaPrintf(const char *fmt, va_list va);

static int s_createMissionLink(NetLink *link)
{
    devassert(s_missionlinks.links->size == 1);
    netLinkSetMaxBufferSize(link, BothBuffers, 8*1024*1024); // Set max size to auto-grow to
    netLinkSetBufferSize(link, BothBuffers, 1*1024*1024);
    link->userData = NULL;
    link->totalQueuedBytes = 0;
    if(!server_cfg.missionserver_max_queueSize)
        server_cfg.missionserver_max_queueSize = DB_MISSIONSERVER_MAX_SENDQUEUE_SIZE_DEFAULT;
    if(!server_cfg.missionserver_max_queueSizePublish)
        server_cfg.missionserver_max_queueSizePublish = DB_MISSIONSERVER_MAX_SENDQUEUEPUBLISH_SIZE_DEFAULT;
    return 1;
}

static int s_deleteMissionLink(NetLink *link)
{
    printf("lost connection to missionserver.\n");
    return 1;
}

void missionserver_commInit(void)
{
    netLinkListAlloc(&s_missionlinks, 2, 0, s_createMissionLink);
    s_missionlinks.destroyCallback = s_deleteMissionLink;
    if(netInit(&s_missionlinks, 0, DEFAULT_MISSIONSERVER_PORT))
        NMAddLinkList(&s_missionlinks, s_handleMissionPacket);
    else
        FatalErrorf("Error listening for missionserver.\n");

    missionserver_meta_register();
    meta_registerPrintfMessager(s_remoteMetaPrintf);
}

static NetLink* getEntLink(int entid)
{
    EntCon *ent = containerPtr(ent_list, entid);
    return dbLinkFromLockId(SAFE_MEMBER(ent, lock_id));
}

static void s_sendMessageToDbid(NetLink *link, U32 entid, const char *msg)
{
    Packet *pak_out = pktCreateEx(link, DBSERVER_MESSAGEENTITY);
    pktSendBitsAuto(pak_out, entid);
    pktSendBitsAuto(pak_out, INFO_ARCHITECT);
    pktSendStringAligned(pak_out, msg);
    pktSend(&pak_out, link);
}

static int s_remoteMetaPrintf(const char *fmt, va_list va)
{
    // hacky mess... this should be completely automated
    CallInfo *call = meta_getCallInfo();
    if(call->route_steps == 1) // 1 steps came from a client (client->map)
    {
        assertmsg(!strchr(fmt, '%'), "only single strings supported for now");
        s_sendMessageToDbid(call->link, call->route_ids[0], fmt);
    }
    return 0;
}

static void s_pktSendGetArcInfo(Packet *pak_out, Packet *pak_in)
{
    int comments;
    pktSendGetBool(pak_out, pak_in);        // owned
    pktSendGetBool(pak_out, pak_in);        // played
    pktSendGetBitsAuto(pak_out, pak_in);    // myVote
    pktSendGetBitsAuto(pak_out, pak_in);    // ratingi
    pktSendGetBitsAuto(pak_out, pak_in);    // total_votes
    pktSendGetBitsAuto(pak_out, pak_in);    // keyword_votes
    pktSendGetBitsAuto(pak_out, pak_in);    // date
    pktSendGetString(pak_out, pak_in);        // header
    comments = pktSendGetBitsAuto(pak_out, pak_in);
    while(comments--)
    {
        pktSendGetBitsAuto(pak_out, pak_in);    // auth id
        pktSendGetBitsAuto(pak_out, pak_in);    // auth region
        pktSendGetBitsAuto(pak_out, pak_in);    // comment type
        pktSendGetString(pak_out, pak_in);        // globalName
        pktSendGetString(pak_out, pak_in);        // complaint
    }
}

static int s_handleMissionPacket(Packet *pak_in, int cmd, NetLink *missionlink)
{
    if(missionlink != missionserver_getLink()) // only allow one link
    {
        Packet *pak_out = pktCreateEx(missionlink, MISSION_CLIENT_ERROR);
        pktSendString(pak_out, "Too many connections!");
        pktSend(&pak_out, missionlink);
        return 1;
    }

    switch (cmd)
    {
        xcase MISSION_SERVER_CONNECT:
        {
            Packet *pak_out = pktCreateEx(missionlink, MISSION_CLIENT_CONNECT);
            pktSendBitsAuto(pak_out, DBSERVER_MISSION_PROTOCOL_VERSION);
            pktSendString(pak_out, server_cfg.shard_name[0] ? server_cfg.shard_name : server_cfg.db_server);
            pktSend(&pak_out, missionlink);

            printf_stderr("MissionServer connecting at %s\n", makeIpStr(missionlink->addr.sin_addr.S_un.S_addr));

            missionlink->compressionDefault = 1;
            if( server_cfg.isLoggingMaster )    
                sendLogLevels(missionlink, MISSION_CLIENT_SET_LOG_LEVELS);
        }

        xcase MISSION_SERVER_MESSAGEENTITY:
        {
            int entid = pktGetBitsAuto(pak_in);
            NetLink *entlink = getEntLink(entid);
            if(entlink)
            {
                const char *fmt;
                Packet *pak_out = pktCreateEx(entlink, DBSERVER_MESSAGEENTITY);
                pktSendBitsAuto(pak_out, entid);
                pktSendGetBitsAuto(pak_out, pak_in);    // channel
                fmt = pktSendGetStringAligned(pak_out, pak_in);
                while(fmt = strchr(fmt, '%'))
                {
                    switch(*++fmt)
                    {
                        xcase 's':    pktSendGetStringAligned(pak_out, pak_in);
                        xcase 'd':    pktSendGetBitsAuto(pak_out, pak_in);
                        xcase '%':    // do nothing
                        xdefault:    assertmsgf(0, "Unhandled printf type '%c'", *fmt);
                    }
                }
                pktSend(&pak_out, entlink);
            }
        }

        xcase MISSION_SERVER_SEARCHPAGE:
        {
            int entid = pktGetBitsAuto(pak_in);
            NetLink *entlink = getEntLink(entid);
            if(entlink)
            {
                int arcid;
                Packet *pak_out = pktCreateEx(entlink, DBSERVER_MISSIONSERVER_SEARCHPAGE);
                pktSendBitsAuto(pak_out, entid);            // entid
                pktSendGetBitsAuto(pak_out, pak_in);        // category
                pktSendGetString(pak_out, pak_in);            // context
                pktSendGetBitsAuto(pak_out, pak_in);        // page
                pktSendGetBitsAuto(pak_out, pak_in);        // page count
                while(arcid = pktSendGetBitsAuto(pak_out, pak_in))
                {
                    if(arcid < 0)
                        pktSendGetString(pak_out, pak_in);    // section
                    else
                        s_pktSendGetArcInfo(pak_out, pak_in);
                }
                pktSend(&pak_out, entlink);
            }
        }
        xcase MISSION_SERVER_ALLARCS:
        {
            int mapId = pktGetBitsAuto(pak_in);
            NetLink *maplink = dbLinkFromLockId(mapId);
            if(maplink)
            {
                Packet *pak_out = pktCreateEx(maplink, DBSERVER_MISSIONSERVER_ALLARCS);
                pktSendGetBitsAuto(pak_out, pak_in);    //total arcs
                pktSendGetBitsAuto(pak_out, pak_in);    //nInvalid
                pktSendGetBitsAuto(pak_out, pak_in);    //nUnpublished
                while(pktSendGetBitsAuto(pak_out, pak_in));        //all of the ids
                //ends by sending 0.
                pktSend(&pak_out, maplink);
            }
        }
        xcase MISSION_SERVER_ARCINFO:
        {
            int entid = pktGetBitsAuto(pak_in);
            NetLink *entlink = getEntLink(entid);
            if(entlink)
            {
                Packet *pak_out = pktCreateEx(entlink, DBSERVER_MISSIONSERVER_ARCINFO);
                pktSendBitsAuto(pak_out, entid);
                while(pktSendGetBitsAuto(pak_out, pak_in))        // arcid
                    if(pktSendGetBool(pak_out, pak_in))            // found
                        s_pktSendGetArcInfo(pak_out, pak_in);
                pktSend(&pak_out, entlink);
            }
        }

        xcase MISSION_SERVER_BANSTATUS:
        {
            int mapid = pktGetBitsAuto(pak_in);
            NetLink *maplink = dbLinkFromLockId(mapid);
            if(maplink)
            {
                Packet *pak_out = pktCreateEx(maplink, DBSERVER_MISSIONSERVER_BAN_STATUS);
                pktSendGetBitsAuto(pak_out, pak_in);    // arc id
                pktSendGetBitsAuto(pak_out, pak_in);    // ban status
                pktSendGetBitsAuto(pak_out, pak_in);    // honors
                pktSend(&pak_out, maplink);
            }
        }

        xcase MISSION_SERVER_ARCDATA:
        {
            int entid = pktGetBitsAuto(pak_in);
            int mapid = pktGetBitsAuto(pak_in);            // used for maptest
            NetLink *entlink = entid?getEntLink(entid):dbLinkFromLockId(mapid);

            if(entlink)
            {
                Packet *pak_out = pktCreateEx(entlink, DBSERVER_MISSIONSERVER_ARCDATA);
                pktSendBitsAuto(pak_out, entid);
                pktSendGetBitsAuto(pak_out, pak_in);    // arcid
                pktSendGetBitsAuto(pak_out, pak_in);    // mode
                pktSendGetBitsAuto(pak_out, pak_in);    // devchoice
                if(pktSendGetBool(pak_out, pak_in))        // found
                {
                    pktSendGetBitsAuto(pak_out, pak_in);// authorid
                    pktSendGetBitsAuto(pak_out, pak_in);// ratingi
                    pktSendGetZipped(pak_out, pak_in);    // arc data
                }
                pktSend(&pak_out, entlink);
            }
        }

        xcase MISSION_SERVER_ARCDATA_OTHERUSER:
        {
            int entid = pktGetBitsAuto(pak_in);
            int mapid = pktGetBitsAuto(pak_in);
            NetLink *entlink = entid?getEntLink(entid):dbLinkFromLockId(mapid);

            if(entlink)
            {
                Packet *pak_out = pktCreateEx(entlink, DBSERVER_MISSIONSERVER_ARCDATA_OTHERUSER);
                int found;
                pktSendBitsAuto(pak_out, entid);
                pktSendGetBitsAuto(pak_out, pak_in);    // arcid
                found = pktGetBitsPack(pak_in, 1);
                pktSendBitsPack(pak_out, 1, found);
                if(found)
                {
                    pktSendGetZipped(pak_out, pak_in);    // arc data
                }
                pktSend(&pak_out, entlink);
            }
        }

        xcase MISSION_SERVER_SENDPETITION:
        {
            char *authname = pktGetStringTemp(pak_in);
            char *name = pktGetStringTemp(pak_in);
            U32 ss2000 = pktGetBitsAuto(pak_in);
            char *summary = pktGetStringTemp(pak_in);
            char *msg = pktGetStringTemp(pak_in);
            PetitionCategory cat = pktGetBitsAuto(pak_in);
            char *notes = pktGetStringTemp(pak_in);

            handleSendPetition(authname, name, ss2000, summary, msg, cat, notes);
        }

        xcase MISSION_SERVER_COMMENT:
            handleMissionServerEmail(pak_in);

        xcase MISSION_SERVER_INVENTORY:
        {
            int entid = pktGetBitsAuto(pak_in);
            NetLink *entlink = getEntLink(entid);
            if(entlink)
            {
                Packet *pak_out = pktCreateEx(entlink, DBSERVER_MISSIONSERVER_INVENTORY);
                int costumekeys;
                int count;

                pktSendBitsAuto(pak_out, entid);
                pktSendGetBitsAuto(pak_out, pak_in);    // authid
                pktSendGetBitsAuto(pak_out, pak_in);    // ban expiry
                pktSendGetBitsAuto(pak_out, pak_in);    // current tickets
                pktSendGetBitsAuto(pak_out, pak_in);    // lifetime tickets
                pktSendGetBitsAuto(pak_out, pak_in);    // overflow tickets
                pktSendGetBitsAuto(pak_out, pak_in);    // badge flags
                pktSendGetBitsAuto(pak_out, pak_in);    // best arc stars
                pktSendGetBitsAuto(pak_out, pak_in);    // publish slots
                pktSendGetBitsAuto(pak_out, pak_in);    // publish slots used
                pktSendGetBitsAuto(pak_out, pak_in);    // permanent arcs
                pktSendGetBitsAuto(pak_out, pak_in);    // invalid arcs
                pktSendGetBitsAuto(pak_out, pak_in);    // arcs with playable errors
                pktSendGetBitsAuto(pak_out, pak_in);    // uncirculated slots
                pktSendGetBitsAuto(pak_out, pak_in);    // created completions
                costumekeys = pktGetBitsAuto(pak_in);
                pktSendBitsAuto(pak_out, costumekeys);

                for (count = 0; count < costumekeys; count++)
                {
                    pktSendGetString(pak_out, pak_in);
                }

                pktSend(&pak_out, entlink);
            }
            else
            {
                Packet *pak_out = pktCreateEx(missionlink, MISSION_CLIENT_NOT_PRESENT);
                pktSendGetBitsAuto(pak_out, pak_in);    // authid
                pktSend(&pak_out, missionlink);
            }
        }

        xcase MISSION_SERVER_CLAIM_TICKETS:
        {
            int entid = pktGetBitsAuto(pak_in);
            NetLink *entlink = getEntLink(entid);
            if(entlink)
            {
                Packet *pak_out = pktCreateEx(entlink, DBSERVER_MISSIONSERVER_CLAIM_TICKETS);
                pktSendBitsAuto(pak_out, entid);
                pktSendGetBitsAuto(pak_out, pak_in);    // authid
                pktSendGetBitsAuto(pak_out, pak_in);    // tickets claimed
                pktSendGetBitsAuto(pak_out, pak_in);    // tickets remaining
                pktSendGetString(pak_out, pak_in);        // success/error message
                pktSend(&pak_out, entlink);
            }
            else
            {
                Packet *pak_out = pktCreateEx(missionlink, MISSION_CLIENT_NOT_PRESENT);
                pktSendGetBitsAuto(pak_out, pak_in);    // authid
                pktSend(&pak_out, missionlink);
            }
        }

        xcase MISSION_SERVER_ITEM_BOUGHT:
        {
            Packet *pak_out;
            int entid = pktGetBitsAuto(pak_in);
            int authid = pktGetBitsAuto(pak_in);
            int cost = pktGetBitsAuto(pak_in);
            int flags = pktGetBitsAuto(pak_in);
            NetLink *entlink = getEntLink(entid);

            if( !entlink )
            {
                Packet *pak_out = pktCreateEx(missionlink, MISSION_CLIENT_NOT_PRESENT);
                pktSendGetBitsAuto(pak_out, pak_in);    // authid
                pktSend(&pak_out, missionlink);
            }
            else if ( flags == kPurchaseFail )
            {
                pak_out = pktCreateEx(entlink, DBSERVER_MISSIONSERVER_ITEM_REFUND);
                pktSendBitsAuto(pak_out, entid);
                pktSendBitsAuto(pak_out, authid);
                pktSendBitsAuto(pak_out, cost);
                pktSendBitsAuto(pak_out, flags);
                pktSend(&pak_out, entlink);
            }
            else
            {
                Packet *pak_out = pktCreateEx(entlink, DBSERVER_MISSIONSERVER_ITEM_BOUGHT);
                pktSendBitsAuto(pak_out, entid);
                pktSendBitsAuto(pak_out, authid);
                pktSendBitsAuto(pak_out, cost);
                pktSendBitsAuto(pak_out, flags);
                pktSend(&pak_out, entlink);
            }
        }

        xcase MISSION_SERVER_OVERFLOW_GRANTED:
        {
            int entid = pktGetBitsAuto(pak_in);
            NetLink *entlink = getEntLink(entid);

            if(entlink)
            {
                Packet *pak_out = pktCreateEx(entlink, DBSERVER_MISSIONSERVER_OVERFLOW_GRANTED);
                pktSendBitsAuto(pak_out, entid);
                pktSendGetBitsAuto(pak_out, pak_in);    // authid
                pktSendGetBitsAuto(pak_out, pak_in);    // overflow tickets granted
                pktSendGetBitsAuto(pak_out, pak_in);    // new overflow total
                pktSend(&pak_out, entlink);
            }
            else
            {
                Packet *pak_out = pktCreateEx(missionlink, MISSION_CLIENT_NOT_PRESENT);
                pktSendGetBitsAuto(pak_out, pak_in);    // authid
                pktSend(&pak_out, missionlink);
            }
        }

        xdefault:
            devassertmsg(0, "invalid command from missionserver");
    };

    return 1;
}

// *********************************************************************************
//  messages to missionserver
// *********************************************************************************

void missionserver_db_remoteCall(int mapid, Packet *pak_in, NetLink *link)
{
    meta_handleRemote(link, pak_in, mapid, 11);
}

void missionserver_db_publishArc(Packet *pak_in, NetLink *link)
{
    NetLink *missionlink = missionserver_getLink();
    if(missionlink && missionlink->totalQueuedBytes > server_cfg.missionserver_max_queueSizePublish)
    {
        int entid = pktGetBitsAuto(pak_in);
        s_sendMessageToDbid(link, entid, "MissionServerTooBusy");
    }
    else if(missionlink)
    {
        Packet *pak_out = pktCreateEx(missionlink, MISSION_CLIENT_PUBLISHARC);
        pktSendGetBitsAuto(pak_out, pak_in);    // entid
        pktSendGetBitsAuto(pak_out, pak_in);    // arcid
        pktSendGetBitsAuto(pak_out, pak_in);    // authid
        pktSendGetString(pak_out, pak_in);        // authname
        pktSendGetBitsAuto(pak_out, pak_in);    // access level
        pktSendGetBitsAuto(pak_out, pak_in);    // initial keywords
        pktSendGetBitsAuto(pak_out, pak_in);    // publish flags
        pktSendGetString(pak_out, pak_in);        // search header
        pktSendGetZipped(pak_out, pak_in);        // arc data
        pktSend(&pak_out, missionlink);
    }
    else
    {
        // reply with failure
        int entid = pktGetBitsAuto(pak_in);
        s_sendMessageToDbid(link, entid, "MissionServerNoConnection");
    }
}

void missionserver_db_requestSearchPage(int mapid, Packet *pak_in, NetLink *link)
{
    NetLink *missionlink = missionserver_getLink();

    if(missionlink && missionlink->totalQueuedBytes > server_cfg.missionserver_max_queueSize)
    {
        int entid = pktGetBitsAuto(pak_in);
        s_sendMessageToDbid(link, entid, "MissionServerTooBusy");
    }
    else if(missionlink)
    {
        Packet *pak_out = pktCreateEx(missionlink, MISSION_CLIENT_SEARCHPAGE);
        pktSendGetBitsAuto(pak_out, pak_in);    // entid
        pktSendGetBitsAuto(pak_out, pak_in);    // authid
        pktSendGetBitsAuto(pak_out, pak_in);    // access level
        pktSendGetBitsAuto(pak_out, pak_in);    // category
        pktSendGetString(pak_out, pak_in);        // context
        pktSendGetBitsAuto(pak_out, pak_in);    // page
        pktSendGetBitsAuto(pak_out, pak_in);    // only arcs not voted on
        pktSendGetBitsAuto(pak_out, pak_in);    // filter by level range
        pktSendGetBitsAuto(pak_out, pak_in);    // author of specific arc
        pktSend(&pak_out, missionlink);
    }
    // else let them request again later
}

void missionserver_db_requestAllArcs(int mapid, Packet *pak_in, NetLink *link)
{
    NetLink *missionlink = missionserver_getLink();
    int lockId = dbLockIdFromLink(link);
    if(missionlink)
    {
        Packet *pak_out = pktCreateEx(missionlink, MISSION_CLIENT_REQUESTALL);
        pktSendBitsAuto(pak_out, lockId);
        pktSendGetBool(pak_out, pak_in);    //unpublished 
        pktSendGetBool(pak_out, pak_in);    //invalid
        pktSend(&pak_out, missionlink);
    }
    // else let them request again later
}


void missionserver_db_requestArcInfo(int mapid, Packet *pak_in, NetLink *link)
{
    NetLink *missionlink = missionserver_getLink();
    if(missionlink && missionlink->totalQueuedBytes > server_cfg.missionserver_max_queueSize)
    {
        int entid = pktGetBitsAuto(pak_in);
        s_sendMessageToDbid(link, entid, "MissionServerTooBusy");
    }
    else if(missionlink)
    {
        Packet *pak_out = pktCreateEx(missionlink, MISSION_CLIENT_ARCINFO);
        pktSendGetBitsAuto(pak_out, pak_in);        // entid
        pktSendGetBitsAuto(pak_out, pak_in);        // authid
        pktSendGetBitsAuto(pak_out, pak_in);        // access level
        while(pktSendGetBitsAuto(pak_out, pak_in))    // arcid
            ;
        pktSend(&pak_out, missionlink);
    }
    // else let them request again later
}

void missionserver_db_requestBanStatus(int mapid, Packet *pak_in, NetLink *link)
{
    NetLink *missionlink = missionserver_getLink();
    if(missionlink)
    {
        Packet *pak_out = pktCreateEx(missionlink, MISSION_CLIENT_BANSTATUS);
        pktSendBitsAuto(pak_out, dbLockIdFromLink(link));
        pktSendGetBitsAuto(pak_out, pak_in);        // arcid
        pktSend(&pak_out, missionlink);
    }
    // else let them request again later
}

void missionserver_db_requestArcData(int mapid, Packet *pak_in, NetLink *link)
{
    NetLink *missionlink = missionserver_getLink();
    int returnMapId = dbLockIdFromLink(link);
    //TODO: potentially change this so that maps aren't throttled out.
    if(missionlink && missionlink->totalQueuedBytes <= server_cfg.missionserver_max_queueSize)
    {
        Packet *pak_out = pktCreateEx(missionlink, MISSION_CLIENT_ARCDATA);
        pktSendGetBitsAuto(pak_out, pak_in);    // entid
        pktSendGetBitsAuto(pak_out, pak_in);    // arcid
        pktSendGetBitsAuto(pak_out, pak_in);    // authid
        pktSendBitsAuto(pak_out, returnMapId);        // this is necessary for map tests
        pktSendGetBitsAuto(pak_out, pak_in);    // access level
        pktSendGetBitsAuto(pak_out, pak_in);    // mode
        pktSendGetBitsAuto(pak_out, pak_in);    // devchoicebit
        pktSend(&pak_out, missionlink);
    }
    else if (missionlink)
    {
        // reply with failure (bleh)
        Packet *pak_out = pktCreateEx(link, DBSERVER_MISSIONSERVER_ARCDATA);
        int entid = pktSendGetBitsAuto(pak_out, pak_in);            // entid
        pktSendGetBitsAuto(pak_out, pak_in);            // arcid
        pktGetBitsAuto(pak_in);                            // authid (no passthrough)
        pktGetBitsAuto(pak_in);                            // accesslevel (no passthrough)
        pktSendGetBitsAuto(pak_out, pak_in);            // mode
        pktSendGetBitsAuto(pak_out, pak_in);            // devchoice
        pktSendBool(pak_out, false);                    // found
        pktSend(&pak_out, link);

        s_sendMessageToDbid(link, entid, "MissionServerTooBusy");
    }
    else
    {
        // reply with failure (bleh)
        Packet *pak_out = pktCreateEx(link, DBSERVER_MISSIONSERVER_ARCDATA);
        pktSendGetBitsAuto(pak_out, pak_in);            // entid
        pktSendGetBitsAuto(pak_out, pak_in);            // arcid
        pktGetBitsAuto(pak_in);                            // authid (no passthrough)
        pktGetBitsAuto(pak_in);                            // accesslevel (no passthrough)
        pktSendGetBitsAuto(pak_out, pak_in);            // mode
        pktSendGetBitsAuto(pak_out, pak_in);            // devchoice
        pktSendBool(pak_out, false);                    // found
        pktSend(&pak_out, link);
    }
}

void missionserver_db_requestArcDataOtherUser(int mapid, Packet *pak_in, NetLink *link)
{
    NetLink *missionlink = missionserver_getLink();
    int returnMapId = dbLockIdFromLink(link);
    if(missionlink)
    {
        Packet *pak_out = pktCreateEx(missionlink, MISSION_CLIENT_ARCDATA_OTHERUSER);
        pktSendGetBitsAuto(pak_out, pak_in);            // entid
        pktSendGetBitsAuto(pak_out, pak_in);            // arcid
        pktSendBitsAuto(pak_out, returnMapId);
        pktSend(&pak_out, missionlink);
    }
    else
    {
        Packet *pak_out = pktCreateEx(link, DBSERVER_MISSIONSERVER_ARCDATA_OTHERUSER);
        pktSendGetBitsAuto(pak_out, pak_in);            // entid
        pktSendGetBitsAuto(pak_out, pak_in);            // arcid
        pktSendBitsPack(pak_out, 1, 0);                    // no data
        pktSend(&pak_out, link);
    }
}

void missionserver_db_requestInventory(int mapid, Packet *pak_in, NetLink *link)
{
    NetLink *missionlink = missionserver_getLink();
    if(missionlink)
    {
        Packet *pak_out = pktCreateEx(missionlink, MISSION_CLIENT_INVENTORY);
        pktSendGetBitsAuto(pak_out, pak_in);    // entid
        pktSendGetBitsAuto(pak_out, pak_in);    // authid
        pktSend(&pak_out, missionlink);
    }
    else
    {
        int entid = pktGetBitsAuto(pak_in);
        s_sendMessageToDbid(link, entid, "MissionServerNoConnection");
    }
}

void missionserver_db_claimTickets(int mapid, Packet *pak_in, NetLink *link)
{
    NetLink *missionlink = missionserver_getLink();
    if(missionlink)
    {
        Packet *pak_out = pktCreateEx(missionlink, MISSION_CLIENT_CLAIM_TICKETS);
        pktSendGetBitsAuto(pak_out, pak_in);    // entid
        pktSendGetBitsAuto(pak_out, pak_in);    // authid
        pktSendGetBitsAuto(pak_out, pak_in);    // number of tickets
        pktSend(&pak_out, missionlink);
    }
    else
    {
        int entid = pktGetBitsAuto(pak_in);
        s_sendMessageToDbid(link, entid, "MissionServerNoConnection");
    }
}

void missionserver_db_buyItem(int mapid, Packet *pak_in, NetLink *link)
{
    NetLink *missionlink = missionserver_getLink();
    if(missionlink)
    {
        Packet *pak_out = pktCreateEx(missionlink, MISSION_CLIENT_BUY_ITEM);
        U32 flags;

        pktSendGetBitsAuto(pak_out, pak_in);    // entid
        pktSendGetBitsAuto(pak_out, pak_in);    // authid
        pktSendGetBitsAuto(pak_out, pak_in);    // cost on mapserver

        flags = pktGetBitsAuto(pak_in);
        pktSendBitsAuto(pak_out, flags);

        if(flags & kPurchaseUnlockKey)
            pktSendGetString(pak_out, pak_in);

        if(flags & kPurchasePublishSlots)
            pktSendGetBitsAuto(pak_out, pak_in);

        pktSend(&pak_out, missionlink);
    }
    else
    {
        Packet *pak_out;
        int entid = pktGetBitsAuto(pak_in);
        int authid = pktGetBitsAuto(pak_in);
        int cost = pktGetBitsAuto(pak_in);
        int flags = pktGetBitsAuto(pak_in);

        s_sendMessageToDbid(link, entid, "MissionServerNoConnection");

        // The MapServer would refund their tickets when the round-trip
        // timed out, but doing this means they get them back quicker.
        pak_out = pktCreateEx(link, DBSERVER_MISSIONSERVER_ITEM_REFUND);
        pktSendBitsAuto(pak_out, entid);
        pktSendBitsAuto(pak_out, authid);
        pktSendBitsAuto(pak_out, cost);
        pktSendBitsAuto(pak_out, flags);
        pktSend(&pak_out, link);
    }
}

void handleMissionServerEmail( Packet * pak_in )
{
    MapCon    *map;
    int i;

    for(i=0;i<map_list->num_alloced;i++)
    {
        map = (MapCon *) map_list->containers[i];
        if( map->is_static && map->active)
        {
            Packet    *pak_out = pktCreateEx(map->link,DBSERVER_CREATE_EMAIL);
            pktSendGetString(pak_out,pak_in); // author
            pktSendGetString(pak_out,pak_in); // title
            pktSendGetBitsAuto(pak_out,pak_in); // author id
            pktSendGetBitsAuto(pak_out,pak_in); // commenter id
            pktSendGetString(pak_out,pak_in); // comment
            pktSend(&pak_out,map->link);
            return;
        }
    }
};
