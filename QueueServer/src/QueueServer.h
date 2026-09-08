#pragma once
#include <utilitieslib/network/net_link.h>
#include <utilitieslib/components/StashTable.h>

typedef struct AllowedAuthnames
{
    char *denied_msg;
    char **allowed_names;

    // not parsed 
    StashTable allowed_stash;
} AllowedAuthnames;


typedef struct QueueServerState
{
    char                server_name[256];
    NetLink                db_comm_link;
    U32                    max_activePlayers;
    U32                    overload_protection_active;
    AllowedAuthnames    allow_list;
    U32                    current_players;
    U32                    waiting_players;
    F32                    loginsPerSecond;

    int tick_timer;
    int tick_count;
    int packet_timer;
    int packet_count;

    U32                    authLimitedOnlyAutoAllow:1;    //this means that we're not letting anyone in except for the auto-allows.
    U32                    isAccountServerActive:1;

} QueueServerState;
