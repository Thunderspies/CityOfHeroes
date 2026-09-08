#include "dbrelay.h"
#include "comm_backend.h"
#include <utilitieslib/utils/error.h>
#include "mapxfer.h"
#include <utilitieslib/utils/utils.h>
#include "dbserver/servercfg.h"
#include "container.h"
#include "sql_fifo.h"
#include "dblog.h"
#include <utilitieslib/utils/log.h>
#include "clientcomm.h"
#include "accountservercomm.h"

static void handleAsyncRelay(int ent_id, char *msg, int preferred_lockid, bool account_read_attempted);

typedef struct {
    int ent_id;
    char *msg;
    int preferred_lockid;
    bool account_read_attempted;
} RelayMsg;

typedef struct
{
    int        count,max;
    RelayMsg *msgs;
} RelayQueue;

RelayQueue relay_queue = {0};

static void addRelayToQueue(int ent_id, char *msg, int preferred_lockid, bool account_read_attempted)
{
    RelayMsg *m = dynArrayAdd(&relay_queue.msgs, sizeof(RelayMsg), &relay_queue.count, &relay_queue.max, 1);
    m->ent_id = ent_id;
    m->msg = strdup(msg);
    m->preferred_lockid = preferred_lockid;
    m->account_read_attempted = account_read_attempted;
}

int dbRelayQueueCheckSingle(RelayMsg *m)
{
    EntCon *ent_con = containerPtr(ent_list, m->ent_id);
    if (!ent_con) {
        // Either he was just unloaded or he's still being loaded
        if (sqlIsAsyncReadPending(CONTAINER_ENTS, m->ent_id)) {
            // Wait until it finishes reading
            return 0;
        }
        // it's been unloaded, queue the process up from the beginning
        return 1;
    } else {
        ShardAccountCon *shardaccount_con = containerPtr(shardaccounts_list, ent_con->auth_id);
        if (!shardaccount_con) {
            // Either it was just unloaded or it's still being loaded
            if (sqlIsAsyncReadPending(CONTAINER_SHARDACCOUNT, ent_con->auth_id)) {
                // Wait until it finishes reading
                return 0;
            }
            // it doesn't exist but that's okay, we'll deal with it in handleAsyncRelay
            return 1;
        } else {
            // Entity and ShardAccount is loaded
            if (ent_con->lock_id) {
                // Someone has it locked!  Send the relay to that map!
                return 1;
            }
            if (ent_con->callback_link && ent_con->is_gameclient) {
                // Someone is logging into this character at the moment, but it's not locked
                // we want to wait until the player has finished logging in and a mapserver has it locked
                // and/or it gets unloaded
                return 0;
            }
            // It's not locked and no one is logging in on it, our read must have finished
            return 1;
        }
    }
}

void dbRelayQueueCheck(void) // Should be called whenever entities are loaded or unloaded
{
    int i;
    // Walk from tail to head so we do not get stuck in a endless loop if we
    // are constantly adding the same item to the tail of the list
    for (i=relay_queue.count-1; i>=0; i--) {
        if (dbRelayQueueCheckSingle(&relay_queue.msgs[i])) {
            RelayMsg m = relay_queue.msgs[i]; // Copy to stack
            // Remove from array (fill from the end)
            if (i != relay_queue.count-1) {
                relay_queue.msgs[i] = relay_queue.msgs[relay_queue.count-1];
            }
            relay_queue.count--;
            handleAsyncRelay(m.ent_id, m.msg, m.preferred_lockid, m.account_read_attempted);
            free(m.msg);
        }
    }
}

void dbRelayQueueRemove(int ent_id)
{
    int i;
    for (i=0; i<relay_queue.count; i++) {
        RelayMsg *m = &relay_queue.msgs[i];
        if (m->ent_id == ent_id) {
            SAFE_FREE(m->msg);
            // Remove from array (fill from the end)
            if (i != relay_queue.count-1) {
                relay_queue.msgs[i] = relay_queue.msgs[relay_queue.count-1];
            }
            relay_queue.count--;
            i--;
        }
    }
}


static void queueAsyncRelay(int ent_id, char *msg, int preferred_lockid, bool account_read_attempted)
{
    // Check to see if he is already loaded for some other purpose (i.e. someone logging in)
    EntCon *ent_con = containerPtr(ent_list, ent_id);
    if (ent_con)
    {
        ShardAccountCon *shardaccount_con = containerPtr(shardaccounts_list, ent_con->auth_id);
        if (shardaccount_con || sqlIsAsyncReadPending(CONTAINER_SHARDACCOUNT, ent_con->auth_id)) {
            // Someone else already has an async read going, we just want to run this relay command when
            // that one finishes

            // Just queue the relay command
        } else {
            //queue up async read of shard account
            sqlReadContainerAsync(CONTAINER_SHARDACCOUNT, ent_con->auth_id, NULL, NULL, NULL);
            account_read_attempted = true;
        }
    // check to see if he's already asynchronously being read
    } else if (sqlIsAsyncReadPending(CONTAINER_ENTS, ent_id)) {
        // Someone else already has an async read going, we just want to run this relay command when
        // that one finishes (if it's a login, this'll get routed to his map, if it's
        // another relay command he will be either locked or unloaded when this occurs
        // and this function will be called a second time)

        // Just queue the relay command
    } else {
        //queue up async read of entity
        sqlReadContainerAsync(CONTAINER_ENTS, ent_id, NULL, NULL, NULL);
    }
    // broadcast of relay will occur when the entity is loaded or on a map
    addRelayToQueue(ent_id, msg, preferred_lockid, account_read_attempted);
}

static NetLink *randomMapLink(int static_only)
{
    int i;
    int count=0;
    int idx;
    for(i=0; i<map_list->num_alloced; i++)
    {
        MapCon* map_con = (MapCon*)map_list->containers[i];
        if (map_con->active && !map_con->starting && map_con->link) {
            if (!static_only || map_con->is_static) {
                count++;
            }
        }
    }
    if (!count)
        return NULL;
    idx = rand()%count;
    for(i=0; i<map_list->num_alloced; i++)
    {
        MapCon* map_con = (MapCon*)map_list->containers[i];
        if (map_con->active && !map_con->starting && map_con->link) {
            if (!static_only || map_con->is_static) {
                if (!idx) {
                    return map_con->link;
                }
                idx--;
            }
        }
    }
    assert(false); // Can't get here
    return NULL;
}

static void handleAsyncRelay(int ent_id, char *msg, int preferred_lockid, bool account_read_attempted)
{
    // This callback occurs after the entity has been loaded, and it needs to get
    // shipped off to a map along with the relay command and then removed from the map
    // immediately after the relay command is executed
    EntCon  *ent_con;
    ShardAccountCon *shardaccount_con;
    NetLink *map_link;
    MapCon    *map_con;
    //printf("Async relay on ent %d : %-30s\n", ent_id, msg);
    if (server_cfg.log_relay_verbose) 
    {
        LOG_OLD( "Async relay on ent %d : %-30s", ent_id, msg);
    }

    ent_con = containerPtr(ent_list, ent_id);
    if (!ent_con) {
        // The entity is no longer loaded, perhaps another relay command operated on him, queue it up again!
        //printf("Entity failed to load, queueing it up again!\n");
        if (server_cfg.log_relay_verbose) 
        {
            LOG_OLD( "handleAsyncRelay  Entity failed to load, queueing it up again!");
        }
        queueAsyncRelay(ent_id, msg, preferred_lockid, false);
        return;
    }
    shardaccount_con = containerPtr(shardaccounts_list, ent_con->auth_id);
    if (!shardaccount_con) {
        if (account_read_attempted)
        {
            // The shard account wasn't loaded. We're going to assume that it doesn't exist because we never unload shard accounts.
            shardaccount_con = createShardAccountContainer(ent_con->auth_id, ent_con->account);
        }
        else
        {
            queueAsyncRelay(ent_id, msg, preferred_lockid, false);
            return;
        }
    }
    if (ent_con->lock_id) {
        // Someone has it locked!  Send the relay to that map!
        // If it's locked because another relay was just sent there, this will arrive after the kick
        //  command and be sent back through the DbServer
        map_link = dbLinkFromLockId(ent_con->lock_id);
        //printf("\tHe's locked!\n");
        if (map_link)
        {
            Packet    *pak = pktCreateEx(map_link,DBSERVER_RELAY_CMD);
            //printf("\tNow he's online.  Relaying to map %d\n", ((DBClientLink*)map_link->userData)->map_id);
            if (server_cfg.log_relay_verbose) 
            {
                LOG_OLD( "handleAsyncRelay He's locked and online, relaying to map %d", ((DBClientLink*)map_link->userData)->map_id);
            }
            pktSendString(pak,msg);
            pktSend(&pak,map_link);
            return;
        } 
        else
        {
            LOG_OLD( "handleAsyncRelay Entity locked but no map link found (relay command for Ent %d)", ent_con->id);
            if (server_cfg.log_relay_verbose) 
            {
                LOG_OLD( "handleAsyncRelay  Entity locked but no map link found!");
            }
        }
    }
    if (ent_con->callback_link && ent_con->is_gameclient) {
        assert(0); // This should be filter in the check function above
    }
    // He has been demand loaded
    ent_con->demand_loaded = 1;
    if (server_cfg.log_relay_verbose) 
    {
        LOG_OLD( "handleAsyncRelay  Setting demand_loaded on %d, (is_map_xfer: %d)", ent_con->id, ent_con->is_map_xfer);
    }

    // Find a suitable map to send the entity and relay command to
    map_link = NULL;
    if (preferred_lockid) {
        map_link = dbLinkFromLockId(preferred_lockid);
    }
    if (!map_link) {
        map_con = containerPtr(map_list, ent_con->static_map_id);
        if (map_con && map_con->active && !map_con->starting && map_con->link) {
            map_link = map_con->link;
        }
    }
    if (!map_link)
        map_link = randomMapLink(1);
    if (!map_link)
        map_link = randomMapLink(0);
    if (!map_link)
    {
        char errmsg[1000];
        static base_map_id = 0;
        if (!base_map_id)
            base_map_id = determineStartMapId(0);
        map_con = startMapIfStatic(NULL, NULL, base_map_id, errmsg, "handleAsyncRelay");
        if (!map_con) {
            dbMemLog(__FUNCTION__, "failed: %s", errmsg);
            if (server_cfg.log_relay_verbose) 
                LOG_OLD( "handleAsyncRelay  No maps are ready, queueing it up again!");
        }
        queueAsyncRelay(ent_id, msg, preferred_lockid, true);
    } 
    else 
    {
        Packet    *pak;
        // Send the entity and lock him
        containerSendList(ent_list, &ent_id, 1/*count*/, 1/*lock*/, map_link, 0);
        // Send the non-authoritative account info
        accountInventoryRelayForRelay(map_link, shardaccount_con, ent_id);
        // Send the relay command
        pak = pktCreateEx(map_link,DBSERVER_RELAY_CMD);
        pktSendString(pak,msg);
        pktSend(&pak,map_link);
        ent_con->demand_loaded = 0; // Clear this out so if this entity logs in before he's unloaded the flag goes away!
        if (server_cfg.log_relay_verbose) 
        {
            LOG_OLD( "handleAsyncRelay  Sent to map %d, cleared demand_loaded: %d, is_map_xfer: %d", ((DBClientLink*)map_link->userData)->map_id, ent_con->demand_loaded, ent_con->is_map_xfer);
        }
        dbForceLogout(ent_id,-2);
    }
}

void sendToEnt(DBClientLink *client, int ent_id, int force, char *msg)
{
    EntCon  *ent_con;
    NetLink *map_link=NULL;
    int server_id = 0;
    Packet    *response=NULL;

    if (client)
        response = pktCreateEx(client->link, DBSERVER_RELAY_CMD_RESPONSE);

    //printf("got relay cmd on ent %d%s: %-.30s\n", ent_id, force?" (FORCE)":"", msg);
    ent_con = containerPtr(ent_list, ent_id);
    if (ent_con && ent_con->lock_id)
    {
        // He's online and he's locked by a mapserver
        map_link = dbLinkFromLockId(ent_con->lock_id);
        if (client)
        {
            pktSendBitsPack(response, 1, 1); // succeded
            pktSendBitsPack(response, 1, ((DBClientLink*)map_link->userData)->map_id); // map to be routed to
        }
        //printf("\tOnline and relayed to map %d\n", ((DBClientLink*)map_link->userData)->map_id);
    }
    else
    {
        if (force)
        {
            //printf("\tOffline and forced!\n");
            // He's offline, but we need him loaded and sent to a mapserver
            // Determine if this is a local mapserver
            MapCon *map_con = client ? containerPtr(map_list, client->map_id) : NULL;
            int is_localmapserver = map_con?map_con->edit_mode:0;
            queueAsyncRelay(ent_id, msg, is_localmapserver?client->lock_id:0, false);
            if (client)
            {
                pktSendBitsPack(response, 1, 1); // DbServer will ensure success
                pktSendBitsPack(response, 1, 0); // Not online currently
            }
        }
        else
        {
            //printf("\tOffline (failed)\n");
            if (client)
                pktSendBitsPack(response, 1, 0); // failed
        }
    }

    if (client)
    {
        // Send response telling mapserver if he's online or not in the case of a non-forced send
        pktSend(&response, client->link);
    }

    if (map_link)
    {
        // Needs to be *AFTER* sending the response above in case we relay back to the same map
        Packet    *pak = pktCreateEx(map_link,DBSERVER_RELAY_CMD);
        pktSendString(pak,msg);
        pktSend(&pak,map_link);
    }
}

void handleSendToEnt(DBClientLink *client, Packet *pak_in)
{
    char    *msg;
    int        ent_id, force;

    ent_id = pktGetBitsPack(pak_in,1);
    force = pktGetBitsPack(pak_in,1);
    msg = pktGetString(pak_in);

    sendToEnt(client, ent_id, force, msg);
}

void handleSendToGroupMembers(DBClientLink *client, Packet *pak_in)
{
    char    msg[1024];
    char    *endmsg = NULL;
    int        team_id, team_type, i;
    DbList *list;
    GroupCon *group;

    team_id = pktGetBitsPack(pak_in,1);
    team_type = pktGetBitsPack(pak_in,1);
    strncpy(msg, pktGetString(pak_in), 1024);
    endmsg = msg + strlen(msg);

    list = dbListPtr(team_type);
    if (!list)
        return;

    group = containerPtr(list, team_id);
    if (!group)
        return;

    for (i = 0; i < group->member_id_count; i++)
    {
        // append the entity dbid to the msg
        sprintf(endmsg, " %d", group->member_ids[i]);
        sendToEnt(NULL, group->member_ids[i], 0, msg);
    }
}

void sendToServers(int server_id, char *msg, int static_only)
{
    int        i;
    MapCon    *map;
    Packet    *pak;

    if(server_id >= 0)
    {
        map = containerPtr(map_list,server_id);
        if (!map || !map->active)
            return;
        pak = pktCreateEx(map->link,DBSERVER_RELAY_CMD);
        pktSendString(pak,msg);
        pktSend(&pak,map->link);
    }
    else
    {
        for(i=0;i<map_list->num_alloced;i++)
        {
            map = (MapCon *) map_list->containers[i];
            if(!static_only || map->is_static)
            {
                if (map->active)
                {
                    pak = pktCreateEx(map->link,DBSERVER_RELAY_CMD);
                    pktSendString(pak,msg);
                    pktSend(&pak,map->link);
                }
            }
        }
    }
}

void handleSendToAllServers(Packet *pak_in)
{
    char    *msg;
    int        server_id;

    server_id = pktGetBitsPack(pak_in,1);
    msg = pktGetString(pak_in);

    sendToServers(server_id, msg, 0);
}

