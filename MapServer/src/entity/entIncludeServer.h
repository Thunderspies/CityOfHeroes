
#if ENT_INCLUDE_PART == 1
    // This is stuff that gets included in the "Entity" struct on the server.

    /* Start server-only members ***********************************************/

    Vec3                    static_pos;            // position of player on last static map

    //server net vars

    Quat                net_qrot;            // temp value, precalced for efficiency
    int                        net_iqrot[3];        // compressed integer quaternions (3 ints!)
    Vec3                    client_pos;         // where the client thinks he is
    Vec3                    old_net_pos;        // my previous net_pos
    Quat                old_net_qrot;        // my previous net_pyr

    U32                        birthtime;            // from dbSecondsSince2000()
    U32                        freed_time;            // when I was freed (so I don't get reused for a while).

    U8                        pos_shift;
    U8                        pos_send_sub;

    Packet*                    send_packet;        // The cached update packet containing info about me that is sent to everyone who can see me.

    DbCommState                dbcomm_state;
    
    ClientLink*                myMaster;

    //Queued Chat //TO DO make dynamic struct
    QueuedChat * queuedChat;        //dialog given as several bubbles
    int queuedChatCount;            //For emote chat
    int queuedChatCountMax;            //For emote chat
    F32 queuedChatTimeStop;            //How long I'm suppressing other sticky bits with my emotes
    char * queuedChatTurnToFaceOnArrival; //In the event that this is a goto tag, remember who you are going to face 
    //End Queued Chat

    F32 pvpZoneTimer;    //Character is not allowed to become pvp active until this timer counts down to zero
    EntityRef lastSelectedPet;

    EntityRef attributesTarget;
    bool newAttribTarget;

    HitLocation **ppHitLocs; // The hit tests against geometry this character performed for the most recent attack

    EntityRef audience; //id of the entity I am currently talking to.
    EntityRef objective; //id of the entity I am currently interested in
    int    cutSceneHandle; //If I'm currently an actor in a cutscene, which one?
    //Mat4Ptr pointOfInterest; //A point that is interesting to the speaker.  Used by Emote Chat system. Alloced becuase not many use it

    char * sayOnClick;  //what I have to say when I'm clicked on.
    char * rewardOnClick; //what I give a guy who clicks on me
    OnClickCondition **onClickConditionList;
    char * sayOnKillHero; //What I say when I kill a hero
    ScriptClosureList        onClickHandler;
    ScriptClosureList        scriptedContactHandler;
    StashTable dialogLookup;                // mapping of entities to dialogs in progress

    // SpawnArea related
    
    struct GeneratorInst*    parentGenerator;    // Which generator created me?
    struct GeneratedEntDef*    entityDef;          // Link to definition of entity as defined in SpawnArea file

    // Story arc related stuff
    
    EncounterEntityInfo*    encounterInfo;      // info for entities that belong to an encounter spawn point
    PNPCInfo*                persistentNPCInfo;  // info for persistent npc's
    StoryInfo*                storyInfo;            // story arc info attached to a player

    // Flags for taskforce challenges messaging
    int                        tf_challenge_failed_msg        : 1;

    const VillainDef*        villainDef;
    const char*                aiConfig;            // if non-NULL, use this instead of villainDef->aiConfig
    PCC_Critter*            pcc;
    DamageTracker**            who_damaged_me;        //For critters to track who has hurt them.
    int                        villainRank;        // Actual rank villain was spawned at

    int                        client_console;

    Packet*                    entityPacket;

    int                     playerLocale;

    U32                        passwordMD5[4];                // password hash sent to chatserver (allows external clients to login))
    PowerRef**    powerDefChange;                // These definition of the power has changed.
    PowerRef**    activeStatusChange;            // The active status on these powers have changed since last ent update.
    PowerRef**    enabledStatusChange;        // Either bEnabled or bDisabledManually on these powers has changed since last ent update.
    PowerRef**    rechargeStatusChange;        // These powers are being recharged since last ent update.
    PowerRef**    usageStatusChange;            // These powers have changed in remaining uses/time
    int*                    inspirationStatusChange;    // These inspiration slots have changed since last ent update.
    int*                    boostStatusChange;            // These boost slots have changed since last ent update.

    Character*                pcharLast;                    // For doing diffs
    
    // Delayed pointer to teamup, used for Active Player reward tokens
    // this is a shallow copy pointer if it's pointing to the same Teamup as teamup
    // but is its own structure if it's pointing to something different
    Teamup                    *teamup_activePlayer;

    U32                        teamupTimer_activePlayer;    // Moment at which teamup_activePlayer = teamup.

    MegaGridNode*            megaGridNode;
    MegaGridNode*            megaGridNodeCollision;
    MegaGridNode*            megaGridNodePlayer;
    
    Beacon*                    nearestBeacon;

    #define ENT_POS_HISTORY_SIZE 50
    
    struct {
        Vec3                pos[ENT_POS_HISTORY_SIZE];
        //Vec3                pyr[ENT_POS_HISTORY_SIZE];
        Quat            qrot[ENT_POS_HISTORY_SIZE];
        U32                    time[ENT_POS_HISTORY_SIZE];
        int                    count;
        int                    net_begin;
        int                 end;
        int                    generic_end;
    } pos_history;
    
    #define ENT_DIST_HISTORY_SIZE 60

    struct {
        U16                    dist[ENT_DIST_HISTORY_SIZE];
        U16                    total;
        int                    nextIndex;
        int                    maxIndex;
        Vec3                last_pos;
    } dist_history;
        
    InterpPosition            interp_positions[7];

    int                        numFramesWithoutChange;
    U8                        fakeRagdollFramesSent;// Fake ragdoll has been sent

    MissionObjectiveInfo    *missionObjective;        // Is this entity a mission objective?
    MissionObjectiveInfo    *interactingObjective;    // Is this entity interacting with a mission objective?

    U64                        entity_time_used;

    U8                        auctionPersistFlag;        // should this ent be persisted?
    F32                        auctionPersistTimer;    // timer for persisting entity after 
    
    BeaconDynamicInfo*        beaconDynamicInfo;    // This stores the dynamic beacon information for doors that open/close.

    ScriptUIEntInfo**        scriptUIData;    // Keeps track of scriptUis attached to this player
    void*                    scriptData;        // generic script data for encounter scripts controlling this entity
    U8                        dontOverrideName        : 1; // Dont allow the villaindef to override the name


    int idRoom;            // For bases: the room id where this entity was spawned

    EntityRef*                petList;
    int                        petList_size;
    int                        petList_max;
    U32                        last_day_jobs_start;// time day jobs time accumulation was started.  Reset to zero when used by DayJobs
    U32                        lastAutoCommandRunTime;

    int                        BloodyBayGangID;            // Gang ID assigned by the bloody bay script
    int                        revokeBadTipsOnTeamupLoad;

    U32                        activePlayerRevision; // not persisted

    char *                    mission_gurney_target; // Override for spawntarget for mission gurneys

    int*                    scriptids; // eai of Entity scritp ids running on this entity

    /* End server-only members *************************************************/
#elif ENT_INCLUDE_PART == 2
    // This is stuff that gets included in the "EntityInfo" struct on the server.
    AIVars*                ai;
    F32                    fadeDist;
    
    U8                    entProcessRate;

    U8                    sleeping                    : 1;
    U8                    paused                        : 1;
    U8                    kill_me_please                : 1;// Kills this entity next time it is processed.
    U8                    sent_self                    : 1;
    U8                    net_prepared                : 1;
    U8                    send_hires_motion            : 1;// Set when I use hires motion.
    U8                    has_processed                : 1;
    U8                    seq_coll_door_or_worldgroup    : 1;// set if seq->type->collisionType is DOOR or WORLDGROUP
#elif ENT_INCLUDE_PART == 3
    // These are just defines and whatever else should be included in entity.h on the server.

    extern Vec3                        entpos[];

    
    #define SET_ENTAI_BY_ID(id)                (SET_ENTINFO_BY_ID(id).ai)
    #define SET_ENTPAUSED_BY_ID(id)            (SET_ENTINFO_BY_ID(id).paused)
    #define SET_ENTSLEEPING_BY_ID(id)        (SET_ENTINFO_BY_ID(id).sleeping)
    
    #define SET_ENTAI(e)                    (SET_ENTINFO(e).ai)
    #define SET_ENTPAUSED(e)                (SET_ENTINFO(e).paused)
    #define SET_ENTSLEEPING(e)                (SET_ENTINFO(e).sleeping)
    #define SET_ENTFADE(e)                    (SET_ENTINFO(e).fadeDist)

    #define ENTAI_BY_ID(id)                    (ENTINFO_BY_ID(id).ai)
    #define ENTPAUSED_BY_ID(id)                (ENTINFO_BY_ID(id).paused)
    #define ENTSLEEPING_BY_ID(id)            (ENTINFO_BY_ID(id).sleeping)
    #define ENTFADE_BY_ID(id)                (ENTINFO_BY_ID(id).fadeDist)

    #define ENTAI(e)                        (ENTINFO(e).ai)
    #define ENTPAUSED(e)                    (ENTINFO(e).paused)
    #define ENTSLEEPING(e)                    (ENTINFO(e).sleeping)
    #define ENTFADE(e)                        (ENTINFO(e).fadeDist)

    #define ENTPOS_BY_ID(id)                ((const F32*)entpos[id])
    #define ENTPOS(e)                        ENTPOS_BY_ID((e)->owner)
    #define ENTPOSX(e)                        ENTPOS((e))[0]
    #define ENTPOSY(e)                        ENTPOS((e))[1]
    #define ENTPOSZ(e)                        ENTPOS((e))[2]
    #define ENTPOSPARAMS(e)                    ENTPOSX(e), ENTPOSY(e), ENTPOSZ(e)
    #define MAX_CONTRIB 31
    #define MAX_BUFFS 50

    typedef struct EncounterEntityInfo        EncounterEntityInfo;
    typedef struct PNPCInfo                    PNPCInfo;
    typedef struct StoryInfo                StoryInfo;
    typedef struct MissionObjectiveInfo        MissionObjectiveInfo;
    typedef struct StoryTaskInfo            StoryTaskInfo;
    typedef struct PowerRef        PowerRef;
    typedef struct MegaGridNode             MegaGridNode;
    typedef struct Beacon                    Beacon;
    typedef struct BeaconBlock                BeaconBlock;
    typedef struct BeaconDynamicInfo        BeaconDynamicInfo;
    typedef struct ScriptClosure            ScriptClosure;
    typedef ScriptClosure**                    ScriptClosureList;
    typedef struct ScriptUIEntInfo            ScriptUIEntInfo;

    typedef intptr_t ContactHandle;
    typedef int MapTransferType;

    typedef enum MapXferStep
    {
        MAPXFER_NO_XFER = 0,
        MAPXFER_TELL_CLIENT_IDLE,
        MAPXFER_WAIT_MAP_STARTING,
        MAPXFER_WAIT_CLIENT_IDLE,
        MAPXFER_WAIT_NEW_MAPSERVER_READY,
        MAPXFER_DONE,
    } MapXferStep;

    typedef struct DbCommState
    {
        U8      in_map_xfer;        // Player ent is in process of moving to new map server
        U8        waiting_map_xfer;    // Waiting for handleDbNewMapReady() or handleDbNewMapFail()
        MapTransferType xfer_type;
        MapXferStep    map_xfer_step;
        int     new_map_id;
        int     client_waiting;
        int        map_ready;            // Flagged when the map is ready for receiving a transfer

        U32     login_cookie;
    } DbCommState;
    typedef struct BasePower BasePower;
    typedef struct UniqueBuffMod
    {
        float fMag;
        int iHash;
    }UniqueBuffMod;

    typedef struct UniqueBuff
    {
        unsigned int uiID;
        EntityRef er;
        const BasePower *ppow;
        float time_remaining;
        int mod_cnt;
        int old_mod_cnt;
        int addedOrChangedThisTick;
        UniqueBuffMod mod[MAX_CONTRIB];
        UniqueBuffMod old_mod[MAX_CONTRIB];
    } UniqueBuff;
#elif ENT_INCLUDE_PART == 4
    UniqueBuff aBuffs[MAX_BUFFS];
    int iCntBuffs;
    int *diffBuffs;
    
    U8                        noPhysics                : 1;

    // Net flags should be together.

    U8                        pos_update                : 3;
    U8                        qrot_update                : 3;
    U8                        move_update                : 1;
    U8                        team_buff_update        : 1;// Set this flag when sending the team buffs to the entity. Also used for send league mate buffs
    U8                        team_buff_full_update    : 1;
    U8                        tf_params_update        : 1;
    U8                        target_update            : 1;
    U8                        clear_target            : 1;
    U8                        general_update            : 1;// Set this flag and the entity will be sent (used for fields that are always sent)
    U8                        logout_update            : 2;
    U8                        supergroup_update        : 1;
    U8                        power_modes_update        : 1;// Used to determine if power modes need to be sent.

    U8                        send_xlucency            : 1;// Whether or not xlucency has changed
    U8                        draw_update                : 1;// Whether the draw state (noDrawOnClient) has changed.
    U8                        cached_send_packet        : 1;// Whether or not the generic packet for this entity has been cached
    U8                        move_instantly            : 1;// Should the client interpolate the next positional change.
    U8                        firstRagdollFrame        : 1;// Ragdoll has been created but not yet sent to clients
    U8                        fakeRagdoll                : 1;// Ragdoll is the last to be sent, and has no novodex presence
    U8                        lastRagdollFrame        : 1;// Ragdoll is gone, but we need to send this info to the client
    U8                        team_update                : 1;// Set when my team flags (good and evil) have changed.
    U8                        collision_update        : 1;// Set when my no_ent_collision flag has changed.
    U8                        send_on_odd_send_update    : 1;// Set when my send_on_odd_send flag has changed.
    U8                        resend_hp                : 1;
    U8                        level_update            : 1;// Set when leveling up and and when respecing
    U8                        updated_powerCust        : 1;    //    set when there is a new power customization and the fx's need to be udpated
    U8                        send_all_powerCust        : 1;    //    set when the client needs a new copy of all the power customzations (new costume slot/login)
    U8                        send_costume            : 1;
    U8                        send_all_costume        : 1;
    U8                        status_effects_update    : 1;// Set when status effects are changed
    U8                        pvp_update                : 1;// Set when pvp mode has changed
    U8                        petinfo_update            : 1;// Send petname or other pet related updates
    U8                        world_update            : 1;// Client requested a world state update
    U8                        helper_status_update    : 1;
    U8                        name_update                : 1;// Displayed name of this entity has changed

    U8                        sentStats                : 1;// Was the entity stat sent?

    // This other crap should not mingle with net stuff.

    U8                        admin                    : 1;// I am an admin, dont count me toward stuff like mission completion
    U8                        invincible                : 3;//
    U8                        untargetable            : 3;
    U8                        alwaysHit                : 2;// MS: this is funny because it has the word "shit" in it. :)
    U8                        alwaysGetHit            : 1;
    U8                      noDamageOnLanding        : 1;// Will I be spared from falling damage on next landing?
    U8                        unstoppable                : 1;// Can't be affected by movement-limiting powers.
    U8                        doNotTriggerSpawns        : 1;// Will not trigger new zone spawns
    U8                        initialPositionSet        : 1;// I've been put somewhere, used by pet zoning code to delay spawning.
    U8                        sentItemLocations        : 1;  // I've been sent the locations of all the items remaining
    U8                        sentCritterLocations    : 1;  // I've been sent the location of all the critter encounters remaining
    U8                      adminFrozen                : 1;// Server says... Don't let me move at all (for players).
    U8                        doorFrozen                : 1;// Server says... Frozen until door stuff finished.
    U8                        controlsFrozen            : 1;// Server says... Start ignoring my controls as soon as the client ACKs it.
    U8                        corrupted_dont_save        : 1;// I got corrupted (usually via cheating) and should not be sent to dbserver when disconnect / get kicked
    U8                        in_door_volume            : 1;// Whether I'm currently in a door volume.
    U8                        do_not_face_target        : 1;// Disable facing target when using powers.
    U8                        taggedAsScriptFound        : 1;// Temporary kludge while I get death callbacks working
    U8                        owned                    : 1;// Whether I'm currently owned by this mapserver (i.e. in the PLAYER_ENTS_OWNED array)
    U8                        demand_loaded            : 1;// Whether I was loaded for a relay command
    U8                        is_map_xfer                : 1;// Was this my first login, or did I transfer from another map?

    U8                        fledMap                    : 1;// I'm dead because I ran away, not because somebody killed me
    U8                        notAttackedByJustAnyone : 1;// Enemy AI needs a special flag to target me, (provided by scripts)
    U8                        cutSceneActor            : 1;//I am currently an actor in a cut scene (so don't make me do certain things)
    U8                        cutSceneState           : 1;//am I set to cut scene state (needed because folks can show up mid cutScene

    U8                         IAmInEmoteChatMode        : 1; //For emote chat
    U8                        IAmMovingToMyChatTarget : 1; //For emote chat

    U8                        auc_trust_gameclient    : 1;
    U8                        dayjob_applied            : 1;
    U8                        admin_changed_sg        : 1; // tracks sg swapping for gms who edit another sg's base

    U8                        ready                    : 1; // Entity fully initialized and ready to be interacted with?
    ClientLink*                client;
#endif
