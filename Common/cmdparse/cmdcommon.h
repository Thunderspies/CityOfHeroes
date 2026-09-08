#ifndef _CMDCOMMON_H
#define _CMDCOMMON_H

#include <utilitieslib/utils/cmdoldparse.h>
#include <time.h>
#include <utilitieslib/stdtypes.h>
#include "entity/entityRef.h"
#include "limits.h"
#include "entity/ragdoll.h"

// MS: The ControlId enums shall not be re-ordered, lest the reorderer desire the wrath of Santa.

typedef enum ControlId
{
    CONTROLID_FORWARD = 0,
    CONTROLID_BACKWARD,
    CONTROLID_LEFT,
    CONTROLID_RIGHT,

    CONTROLID_HORIZONTAL_MAX,

    CONTROLID_UP = CONTROLID_HORIZONTAL_MAX,
    CONTROLID_DOWN,

    CONTROLID_BINARY_MAX,

    CONTROLID_PITCH = CONTROLID_BINARY_MAX,
    CONTROLID_YAW,

    CONTROLID_ANGLE_MAX,

    CONTROLID_RUN_PHYSICS = CONTROLID_ANGLE_MAX,

    CONTROLID_APPLY_SERVER_STATE,

    CONTROLID_NOCOLL,

} ControlId;

#if CLIENT
    typedef U32 MovementControlTimes[CONTROLID_BINARY_MAX];
#else
    typedef U16 MovementControlTimes[CONTROLID_BINARY_MAX];
#endif

typedef enum MovementInputSet
{
    MOVE_INPUT_CMD,
    MOVE_INPUT_OTHER,
    MOVE_INPUT_COUNT,
} MovementInputSet;

typedef struct ServerControlState ServerControlState;

#if SERVER
    typedef struct ServerControlStateList
    {
        int                            count;
        ServerControlState*            head;
        ServerControlState*            tail;
    } ServerControlStateList;
#endif

typedef struct ControlStateChange ControlStateChange;

typedef struct ControlStateChangeList
{
    int                            count;
    ControlStateChange*            head;
    ControlStateChange*            tail;
} ControlStateChangeList;

typedef struct SimulatedNetConditions
{
    int                            lag;
    int                            lag_vary;
    int                            pl;
    int                            oo_packets;    // Intentionally deliver packets out of order.
} SimulatedNetConditions;

#if SERVER
    typedef struct ControlConnStats {
        S32            highest_packet_id;

        F32            check_acc;

        U32            min_total;
        U32            max_total;

        U32            hit_end_count;
        S32            conncheck_count;

        U32            received_controls : 1;
    } ControlConnStats;
#endif

typedef struct ControlLogEntry
{
    time_t            actualTime;
    U32                mmTime;
    S64                qpc;
    S64                qpcFreq;
    F32                timestep;
    F32                timestep_noscale;
} ControlLogEntry;

typedef struct FuturePush FuturePush;

typedef struct ControlState
{
    U32*                        packet_ids;        // table to make sure out-of-order packets don't update commands

    struct {
        Vec3                    cur;            // player pyr
        Vec3                    motion;

        #if CLIENT
            U32                    prev[3];        // pyr last time used for controls.
            U32                    send[3];        // pyr last time sent to server.
        #endif
    } pyr;

    S32                            using_pitch;

    struct {
        U8                        down_now;
        U8                        use_now;
        U8                        affected_last_frame;

        #if CLIENT
            U8                    send;
        #endif

        MovementControlTimes    start_ms;
        MovementControlTimes    total_ms;
        
        U32                        max_ms;
    } dir_key;

    S32                            controls_input_ignored_this_csc;

    S32                            mlook,canlook;
    S32                            turnright,turnleft;
    S32                            cam_rotate;
    S32                            first;
    S32                            nocoll;
    S32                            nosync;
    S32                            controldebug;
    S32                            alwaysmobile;

    SimulatedNetConditions        snc_server;

    S32                            netreps;
    S32                            repredict;
    S32                            client_pos_id;
    S32                            anim_test[10];
    S32                            editor_mouse_invert;
    S32                            predict;
    F32                            speed_scale; // just used by client for testing stuff
    S32                            notimeout;
    S32                            autorun;
    F32                            inp_vel_scale; // [0-1], client can decide how much he wants input to be used.
    F32                            max_speed_scale; // How much to scale max_speed by in physics.

    S32                            follow;            // client entity in follow mode.
    EntityRef                    followTarget;    // the entity to be followed.
    S32                            followMovementCount;    // Movement count when the follow mode was set.
    U8                            followForced;    // the player cannot choose to stop this follow
                                                // ARM NOTE: I noticed playerCheckForcedMove() exists, but I'm not playing well with that right now...

    F32                            pitch_scale;
    F32                            pitch_offset;

    Vec3                        start_pos;
    Vec3                        end_pos;
    
    #if SERVER
        U32                        start_abs_time;
        U32                        stop_abs_time;
        U32                        cur_abs_time;
    #endif

    U8                            received_update_id;

    U32                            selected_ent_server_index;

    ServerControlState*            server_state;
    FuturePush*                    future_push_list;

    U32                            recover_from_landing_timer;
    
    Vec3                        last_inp_vel;
    F32                            last_vel_yaw;
    F32                            yaw_swap_timer;
    S32                            yaw_swap_backwards;
    
    S32                            no_strafe;
    
    S32                            net_error_correction;
    
    U32                            record_motion;
    
    struct {
        EntityRef                erEntityToFace;
        Vec3                    locToFace;
        F32                        timeRemaining;

        U32                        hold_pyr : 1;

        #if CLIENT
            U32                    turnCamera : 1;
        #endif
    } forcedTurn;
    
    struct {
        ControlLogEntry            entries[100];
        S32                        cur;
        
        #ifdef CLIENT
            S32                    enabled;
            S32                    duration;
            S32                    start_time;
        #endif
    } controlLog;

    #if SERVER
        S32                        server_pos_id;
        U32                        server_pos_id_sent;

        ControlStateChangeList    csc_list;

        U8                        sent_update_id;

        U16                        last_net_id;
    
        struct {
            F32                    acc;

            struct {
                Vec3            pos;
                Vec3            vel;
                S32                net_id;
            } latest, sent;
        } phys;

        struct {
            struct {
                F32                ms_acc;
                U16                ms;
            } playback;

            U32                    total_ms;
        } time;

        ServerControlStateList    queued_server_states;
        U32                        cur_server_state_sent : 1;

        S32                        print_physics_step_count;

        S32                        physics_disabled;

        S32                        selected_ent_db_id;

        ControlConnStats        connStats;
    #endif

    #if CLIENT
        F32                        timestep_acc;

        // Note that the camera can only rotate, but not move independently of the player.
        // The position of the player dictates the relative position of the camera.
        Vec3                    cam_pyr_offset;    // camera pyr relative to the player pyr

        S32                        ignore;

        ControlStateChangeList    csc_processed_list;

        ControlStateChangeList    csc_to_process_list;

        S32                        first_packet_sent;
        S32                        mapserver_responding;        // Disallow player movement when the mapserver seems down.

        S32                        move_input_state;

        U8                        move_input[MOVE_INPUT_COUNT];

        SimulatedNetConditions    snc_client;

        struct {
            struct {
                Vec3            pos;
                Vec3            vel;
                U16                net_id;
            } step[100];

            S32                    end;
            S32                    cur;
        } svr_phys;
        
        struct {
            U16                    cur;
            U16                    send;
        } net_id;

        struct {
            U32                    last_send_ms;
        } time;

        U32                        movementControlUpdateCount;
        
        U32                        server_state_pkt_id;
        U32                        forced_move_pkt_id;
        U32                        server_pos_pkt_id;

        S32                        zoomin;
        S32                        zoomout;
        S32                        lookup;
        S32                        lookdown;
        int                        no_ragdoll;
        int                        detached_camera;
    #endif
} ControlState;

extern ControlState control_state;
extern ControlId opposite_control_id[CONTROLID_BINARY_MAX];


#ifdef CLIENT
    extern Cmd client_control_cmds[];
#endif

extern Cmd control_cmds[];

typedef struct GlobalState
{
    F32        frame_time_30;                // time that has passed, scaled by server_visible_state.timestepscale.
    F32        frame_time_30_real;            // the real-world timestep.
    F32        frame_time_30_discarded;    // the time that was discarded when TIMESTEP was clamped.
    F32        time_since[100];
    U32        abs_time_history[100];
    U32        abs_time;
    int        global_frame_count;

    int        no_file_change_check_cmd_set; //Cmd turning off file change check is set
    int        no_file_change_check;            //above plus Client side per tick data change check

    #if CLIENT
    U32        cur_timeGetTime;    
    
    U32        client_abs;
    U32        client_abs_slow;
    U32        client_abs_latency; // current latency between client and server, in abstime
    #endif
} GlobalState;

extern GlobalState global_state;

#define TIMESTEP global_state.frame_time_30
#define TIMESTEP_NOSCALE global_state.frame_time_30_real
#define ABS_TIME global_state.abs_time
#define ABS_TIME_SINCE(x) ((x) ? (unsigned int)(ABS_TIME - (x)) : UINT_MAX)
#define TICKS_TO_ABS(x) ((x) * 100)
#define SEC_TO_ABS(x) ((x) * TICKS_TO_ABS(30))
#define ABS_TO_SEC(x) ((x) / SEC_TO_ABS(1))

typedef struct
{
    U32        *packet_ids; // table to make sure out-of-order packets don't update commands
    F32        time;
    F32        timescale;
    F32        timestepscale;
    F32        time_offset;

    F32        fog_dist;
    Vec3    fog_color;

    char    scene[128];
    int        pause;
    int        disablegurneys;
    int        isPvP;
    U32        abs_time;
} ServerVisibleState;

extern ServerVisibleState server_visible_state;
extern Cmd server_visible_cmds[];

//Seems like a nice place to stick this
#ifdef SERVER //debug
#define STATE_STRUCT server_state
#define SELECTED_ENT validEntFromId(entSendGetInterpStoreEntityId())
#ifdef RAGDOLL
#define CONTEXT_CORRECT_LOADTYPE LOAD_ALL // If we have server side ragdoll, we need all the animation data
#else
#define CONTEXT_CORRECT_LOADTYPE LOAD_SKELETONS_ONLY
#endif
#else
#define STATE_STRUCT game_state
#define SELECTED_ENT validEntFromId(game_state.store_entity_id)
#define CONTEXT_CORRECT_LOADTYPE LOAD_ALL
#endif

void cmdOldCommonInit();
void cmdOldPrintList(Cmd *cmds,void *client,int access_level,char *match_str,int print_header,int show_usage);
ControlStateChange* createControlStateChange();
void destroyControlStateChange(ControlStateChange* csc);
void addControlStateChange(ControlStateChangeList* list, ControlStateChange* csc, int adjust_time);
void destroyControlStateChangeList(ControlStateChangeList* list);
void destroyControlStateChangeListHead(ControlStateChangeList* list);

#if SERVER
    ServerControlState* createServerControlState();
    void destroyServerControlState(ServerControlState* scs);
    void addServerControlState(ServerControlStateList* list, ServerControlState* scs);
    ServerControlState* getQueuedServerControlState(ControlState* controls);
    const ServerControlState* getLatestServerControlState(ControlState* controls);
    void destroyControlState(ControlState *controls);
#else
    void addControlLogEntry(ControlState* controls);
#endif

void destroyFuturePush(FuturePush* fp);
void destroyFuturePushList(ControlState* controls);

#if SERVER
    FuturePush* addFuturePush(ControlState* controls, U32 abs_diff);
#else
    FuturePush* addFuturePush(ControlState* controls);
#endif    

#endif
