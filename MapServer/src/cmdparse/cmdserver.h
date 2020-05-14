#ifndef _CMDSERVER_H
#define _CMDSERVER_H

#include <utilitieslib/stdtypes.h>
#include <utilitieslib/network/netio.h>
#include "entity/entVarUpdate.h"

typedef struct ClientLink ClientLink;

typedef struct ClientLink ClientLink;

typedef enum EntityProcessRate
{
    ENT_PROCESS_RATE_NOT_SET,
    ENT_PROCESS_RATE_ALWAYS,
    ENT_PROCESS_RATE_ON_SEND,
    ENT_PROCESS_RATE_ON_ODD_SEND,
    ENT_PROCESS_RATE_COUNT,
} EntityProcessRate;

typedef struct ServerState
{
    U32        *packet_ids; // table to make sure out-of-order packets don't update commands
    char    msg[256];
    int        tcp_port;
    int        udp_port;
    int        fullworld;
    char    entcon_msg[1000];
    int        pause;
    int        netsteps;
    int        nodebug;
    int        notimeout;

    int        show_entity_state;   //debug
    char    entity_to_show_state[128];//debug
    int        netfxdebug; //debug
    int        showHiddenEntities;
    int        check_jfd; //debug
    int        record_coll;
    int        reloadSequencers; //development mode
    int        quickLoadAnims; //dev mode only
    int        nofx; //dev mode only
    int        mission_server_test; //dev mode only
    int        debug_db_id; // dev mode test;

    int        enablePowerDiminishing;                // Enable diminishing returns on powers (for PvP) - default off
    int        enableTravelSuppressionPower;        // Enable travel suppression for PvP - default off
    int        enableHealDecay;                    // Enable heal decay power for PvP - default off
    int        enablePassiveResists;                // Enable passive resist power for PvP - default off
    int        enableSpikeSuppression;                // Enable damage spike suppression for PvP - default off
    int        inspirationSetting;                    // Determines inspiration use (for PvP) - default all, normal only for arena

    int        in_lame_mode;
    U32        lame_mode_switch_abs_time;
    F32        ent_vis_distance_scale;
    int        safe_player_count;
    int        time_all_ents;

//    int        static_map_server;
    unsigned int        logSeqStates : 1;
    unsigned int        logServerTicks : 1;
    unsigned int        levelEditor : 2;                // -leveleditor
    unsigned int        noEncryption : 1;                // -noencrypt makes for faster loading
    unsigned int        silent : 1;                        // -dbquery sets this, no debug printouts

    unsigned int        doNotAutoGroup : 1;    
    unsigned int        dummy_raid_base : 1;            // load the static raid base
    unsigned int        enableBoostDiminishing : 1;        // Enable diminishing returns on boosts (aka Enhancement Diversification)
    unsigned int        skyFadeInteresting : 1;            // Set to 1 if skyFade values need to be sent

    unsigned int        transient : 1;

    int        viewCutScene;    //viewing a cut scene
    Mat4    cutSceneCameraMat;  //since many files dont care what a cutScene is 
    F32        cutSceneDepthOfField; //out here to be parallel with cutScene CameraMat
    Mat4    moveToCutSceneCameraMat;  //since many files dont care what a cutScene is 
    F32        moveToCutSceneDepthOfField; //out here to be parallel with cutScene CameraMat
    F32        cutSceneOver; //out here to be parallel with cutScene CameraMat

    void *    cutScene; //Global cut scene that has frozen the server and is the only thing being played
    int        cutSceneDebug;

    int        doNotCompleteMission;
    int        allowHeroAndVillainPlayerTypesToTeamUp;
    int        allowPrimalAndPraetoriansToTeamUp;

    int        noAI;
    int        noProcess;
    int        noKillCars;
    int        create_bins;    // create all .bin files and then exit
    int     unpack_map;     // write out map and then exit
    int     map_stats;      // creat map files then exit
    int     map_minimaps;      // creat map minimap files then exit

    int        skyFade1, skyFade2;    
    F32        skyFadeWeight;

    int        cod;

    int        validate_spawn_points;
    int        bad_spawn_count;
    int        bad_spawn_max_count;
    Vec3*    bad_spawns;

    unsigned short logSeqStateTarget;    // a entities[] idx
    ClientLink *curClient;  // the client currently being processed
    unsigned int        playerCount;

    int        aiLogFlags;    // MS:    Flags for how much AI logging to do.
    int        beaconProcessOnLoad;

    int        printEntgenMessages;    // JS: Print generator debug messages to the console.
    int        prune;        // auto-checkout lib files that need pruning
    int        tsr;        // load data into shared memory and just sit there
    int        tsr_dbconnect; // When running a MapServer -tsr, connect to the DbServer, and exit when it closes
    int        nostats;    // Don't calculate, load, or store, statistics for players
    float    xpscale;
    float    influencescale;
    float    prestigescale;
    float    aescale;
    float    aeinfluencescale;
    float    aeprestigescale;
    float    ticketscale;
    int     aggrocap;
    int     targetcapmode;

    int        groupDefVersion;

    char    *newScene;
    
    int        disableMeleePrediction;

    int        charinfoserver;
    int        restorecharacter;

    int        logClientPackets;

    F32        test1;
    F32        test2;
    F32        test3;

    int     profiling_memory;
    int     profile;
    int     profile_spikes;

    int        beta_base; // true if beta base is on.
    int        no_base_edit;

    int        lock_doors;

    int        isBetaShard;

    int        clickToSource : 1;
    int        enablePvPLogging : 1;

    // TODO - remove this when we're certain the LWR exit works correctly
    unsigned int disableSpecialReturn : 1;

    int        disableConsiderEnemyLevel;

    // MS: Neat stuff for Martin network timing.

    struct {
        S32    send_on_this_tick;
        S32    count;
        S32    is_odd_send;
        F32    acc;
        F32    update_period;
    } net_send;

    // Flags for what entity process types should run.

    struct {
        U8        max_rate_this_tick;                        // The highest rate to run this tick.
        F32        timestep_acc[ENT_PROCESS_RATE_COUNT];    // TIMESTEP accumulator.
    } process;

    char**    blockedMapKeys;

    int accountCertificationTestStage;

    int client_logout_time;

    U32        idle_exit_timeout;                            // shutdown the server if it is idle for this many minutes, 0 means there is no idle exit
    U32        maintenance_idle;                            // idle minutes before performing periodic minor idle maintenance, 0 means there is no idle maintenance
    U32        maintenance_daily_start;                    // starting hour of window for automatic daily maintenance (start = end means no maintenance window)
    U32        maintenance_daily_end;                        // ending hour of window for automatic daily maintenance
    int MARTY_enabled;

} ServerState;

extern ServerState server_state;

void cmdOldServerStateInit(void);
void serverStateLoad(char *config_file);
void serverParseClientEx(const char *str, ClientLink *client, int access_level);
#define serverParseClient(str,client) serverParseClientEx(str,client,0) // ACCESS_USER
void cmdOldServerParsef(ClientLink *client, char const *fmt, ...);
int cmdOldServerSend(Packet *pak,int all);
void cmdOldServerInitShortcuts(void);
void cmdOldServerSendShortcuts(Packet *pak);
void cmdOldServerSendCommands(Packet *pak, int access_level);
void cmdOldServerInit(void);
void cmdOldServerHandleRelay(char *msg);
void serverSynchTime(void);
void cmdOldServerReadInternal(char *buf);

void cmdCfgLoad(void);

int serverFindSkyByName(const char *name);
void serverSetSkyFade(int sky1, int sky2, F32 weight); // 0.0 = all sky1

void freePlayerControl( ClientLink* client );

void cfg_setIsAutoBuyProducts(bool b);

int cfg_IsBetaShard(void);

void cfg_setIsBetaShard(int data);
void cfg_setIsBetaShard(int data);
void cfg_setMARTY_enabled(int data);
#endif
