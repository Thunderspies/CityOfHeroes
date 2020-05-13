// entPlayer.h - player specific fields of entity.  Allocated iff ENTTYPE_PLAYER

#ifndef __ENTPLAYER_H
#define __ENTPLAYER_H

#ifdef SERVER

#include "gameData/arenaref.h"  // ArenaRef
#include "entity/entityRef.h"
#include "chatdefs.h"
#include "MARTY.h"
#ifndef PL_STATS_H__
#include "player/pl_stats.h"
#endif
#endif

#include "chatdefs.h"
#include "entity/chatSettings.h"
#include "player/badges.h"
#include "entity/gametypes.h"
#include "entity/attrib_description.h"
#include "account/AccountData.h"
#include "entity/rewardtoken.h"
#include "auth/auth.h"
#include "turnstile/turnstileservercommon.h"


typedef struct DoorAnimState DoorAnimState;
typedef struct TradeData TradeData;
typedef struct Costume Costume;
typedef struct PowerCustomizationList PowerCustomizationList;
typedef struct WdwBase WdwBase;
typedef struct KeyBind KeyBind;
typedef struct ServerKeyBind ServerKeyBind;
typedef struct ArenaEvent ArenaEvent;
typedef struct PetName PetName;
typedef struct RoomDetail RoomDetail;
typedef struct MapHistory MapHistory;
typedef struct StoryArc StoryArc;
typedef struct CertificationRecord CertificationRecord;
typedef struct Tray Tray;

#define MAX_STATIC_MAPS        90
#define MAX_MAPVISIT_CELLS (32*32*8)
#define MAX_AFK_STRING        48
#define MAX_FAME_STRING        128
#define NUM_FAME_STRINGS    10
#define MAX_SELECTABLE_COSTUMES            10
#define MAX_COSTUMES                    10
#define MAX_EARNED_COSTUMES                4
#define MAX_EXTRA_COSTUMES    (MAX_SELECTABLE_COSTUMES-1)
#define MAX_REWARD_CHOICE    128
#define MAX_REWARD_TABLES    10
#define MAX_DEFEAT_RECORDS    16
#define    MAX_BUILD_NUM        8
#define BUILD_NAME_LEN        32
#define SPECIAL_MAP_RETURN_DATA_LEN        128

#define TASKFORCE_ARCHITECT 1
#define TASKFORCE_DEFAULT    2

#define FIRST_RESPEC        24
#define SECOND_RESPEC        34
#define THIRD_RESPEC        44

// be SURE you know which to use: ENT_IS_HERO or ENT_IS_ON_BLUE_SIDE!
#define ENT_IS_HERO(e)        ((e && e->pl) ? (e->pl->playerType == kPlayerType_Hero) : 0)
#define ENT_IS_ON_BLUE_SIDE(e)    ((e && e->pchar) ? (e->pchar->playerTypeByLocation == kPlayerType_Hero) : 0)

// be SURE you know which to use: ENT_IS_VILLAIN or ENT_IS_ON_RED_SIDE!
#define ENT_IS_VILLAIN(e)    ((e && e->pl) ? (e->pl->playerType == kPlayerType_Villain) : 0)
#define ENT_IS_ON_RED_SIDE(e)    ((e && e->pchar) ? (e->pchar->playerTypeByLocation == kPlayerType_Villain) : 0)

#define ENT_IS_ROGUE(e)        ((e && e->pl) ? (e->pl->playerSubType == kPlayerSubType_Rogue) : 0)
#define ENT_IS_IN_PRAETORIA(e)    ((e && e->pl) ? (e->pl->praetorianProgress == kPraetorianProgress_Tutorial || e->pl->praetorianProgress == kPraetorianProgress_Praetoria) : 0)
#define ENT_IS_LEAVING_PRAETORIA(e)    ((e && e->pl) ? (e->pl->praetorianProgress == kPraetorianProgress_TravelHero || e->pl->praetorianProgress == kPraetorianProgress_TravelVillain) : 0)
#define ENT_IS_EX_PRAETORIAN(e)    ((e && e->pl) ? (e->pl->praetorianProgress == kPraetorianProgress_PrimalEarth || e->pl->praetorianProgress == kPraetorianProgress_TravelHero || e->pl->praetorianProgress == kPraetorianProgress_TravelVillain) : 0)
#define ENT_IS_PRIMAL(e)    ((e && e->pl) ? (e->pl->praetorianProgress == kPraetorianProgress_PrimalBorn || e->pl->praetorianProgress == kPraetorianProgress_NeutralInPrimalTutorial) : 0)
#define ENT_IS_PRAETORIAN(e)    ((e && e->pl) ? (e->pl->praetorianProgress != kPraetorianProgress_PrimalBorn && e->pl->praetorianProgress != kPraetorianProgress_NeutralInPrimalTutorial) : 0)

#define ENT_CAN_GO_HERO(e)    ((e && e->pl) ? (e->pl->playerType == kPlayerType_Hero || e->pl->playerSubType == kPlayerSubType_Rogue) : 0)
#define ENT_CAN_GO_VILLAIN(e)    ((e && e->pl) ? (e->pl->playerType == kPlayerType_Villain || e->pl->playerSubType == kPlayerSubType_Rogue) : 0)
#define ENT_CAN_GO_PRAETORIAN(e)    ((e && e->pl) ? (e->pl->praetorianProgress == kPraetorianProgress_Praetoria) : 0)
#define ENT_CAN_GO_PRAETORIAN_TUTORIAL(e)    ((e && e->pl) ? (e->pl->praetorianProgress == kPraetorianProgress_Tutorial) : 0)

#define BLOCK_INVITE_TIME 10
#define BLOCK_ACCEPT_TIME 20

typedef enum EntAlignmentType
{
    ENT_ALIGNMENT_TYPE_HERO,
    ENT_ALIGNMENT_TYPE_ROGUE,
    ENT_ALIGNMENT_TYPE_VILLAIN,
    ENT_ALIGNMENT_TYPE_VIGILANTE,
    ENT_ALIGNMENT_TYPE_COUNT,
    ENT_ALIGNMENT_TYPE_INVALID,
}EntAlignmentType;

typedef enum GmailClaimStateEnum
{
    ENT_GMAIL_CLAIM_NONE,
    ENT_GMAIL_CLAIM_TRYING,
    ENT_GMAIL_CLAIM_WAIT_CLEAR,
    ENT_GMAIL_CLAIM_COUNT,
} GmailClaimStateEnum;

typedef enum GmailPendingStateEnum
{
    ENT_GMAIL_NONE,
    ENT_GMAIL_SEND_XACT_REQUEST,
    ENT_GMAIL_SEND_COMMIT_REQUEST,
    ENT_GMAIL_SEND_CLAIM_REQUEST,
    ENT_GMAIL_SEND_CLAIM_COMMIT_REQUEST,
    ENT_GMAIL_COUNT,
} GmailPendingStateEnum;

typedef struct VisitedMap
{
    int        db_id;
    int        map_id;
    int        opaque_fog;
    U32        cells[(MAX_MAPVISIT_CELLS)/32];
} VisitedMap;


typedef struct DefeatRecord
{
    int victor_id;
    unsigned long defeat_time;
} DefeatRecord;

typedef struct RewardDBStore
{
    char                pendingRewardName[MAX_REWARD_CHOICE];
    int                    pendingRewardVgroup;
    int                    pendingRewardLevel;
}RewardDBStore;

typedef struct GMailClaimState
{
    int mail_id;
    GmailClaimStateEnum claim_state;
}GMailClaimState;

/*
typedef struct GMailPending
{
    int                        mail_id;
    int                        xact_id;
    GmailPendingStateEnum    state;
    int                        influence;
    int                        requestTime;
    char                    subject[MAX_GMAIL_SUBJECT];
    char                    body[MAX_GMAIL_BODY];
    char                    attachment[255];
    char                    to[32];
} GMailPending;
*/

typedef struct EntPlayer
{

    // UI settings
    unsigned int    uiSettings;            // bitfields of data below used to save to db
    unsigned int    uiSettings2;
    unsigned int    uiSettings3;
    unsigned int    uiSettings4;
    unsigned int    showSettings;
    unsigned int    topChatChannels;    // deprecated
    unsigned int    botChatChannels;    // deprecated

    int                    csrModified;
    int                    dateCreated;
    PlayerType            playerType; // Hero or Villain (see PlayerType enum)
    PlayerSubType        playerSubType;
    PraetorianProgress    praetorianProgress;

    int            autoAcceptTeamBelow;
    int            autoAcceptTeamAbove;

    int            chatSendChannel;    // the channel the user is sending to
    int            lastPrivateSender;  // dbid of last person who sent private
    int            lastPrivateSendee;  // dbid of last person I sent a private too
    int            lastChatSent;
    int            chatSpams;
    U32            lastAuctionMsgSent;
    int            auctionSpams;
    U32            lastPetitionSent;
    
    float        chat_divider;
    int         dock_mode;
    int            inspiration_mode;
    int            teambuff_display;

    int    useOldTeamUI;                // true if we're using old team ui
    int    hideUnclaimableCert;
    int    blinkCertifications;
    int voucherSingleCharacterPrompt;
    int newCertificationPrompt;

    Tray*        tray;

    WdwBase*    winLocs;

    char        keyProfile[32];
    int            keybind_count;    

    int            buffSettings;

    ChatSettings chat_settings;
    int            chatBubbleTextColor;
    int            chatBubbleBackColor;

    // I was originally going to make some big complicated fancy
    // structure for this, but Mark convinced me to just be stupid.
    // I just felt like sharing -CW
    int        divSuperName;
    int        divSuperMap;
    int        divSuperTitle;
    int        divEmailFrom;
    int        divEmailSubject;
    int        divFriendName;
    int        divLfgName;
    int        divLfgMap;

    int disableEmail;
    int friendSgEmailOnly;
    int gmailFriendOnly;
    //----------------------------
    int first;
    int options_saved;
    int mouse_invert;
    F32 mouse_speed;
    F32 turn_speed;
    F32 mouseScrollSpeed;
    int chat_font_size;

    int fading_chat;
    int fading_chat1;
    int fading_chat2;
    int fading_chat3;
    int fading_chat4;
    int fading_nav;
    int fading_tray;

    int showToolTips;
    int allowProfanity;
    int showBalloon;
    int declineGifts;
    int declineTeamGifts;
    int promptTeamTeleport;
    int capesUnlocked;
    int glowieUnlocked;
    int showPets;
    int showSalvage;
    int windowFade;
    int logChat;
    int logPrivateMessages;
    int sgHideHeader;
    int sgHideButtons;
    int clicktomove;
    int disableDrag;
    int showTimer;
    int showPetBuffs;
    int preventPetIconDrag;
    int showPetControls;
    int advancedPetControls;
    int disablePetSay;
    int enableTeamPetSay;
    int teamCompleteOption;
    int disablePetNames;
    int hidePlacePrompt;
    int hideDeletePrompt;
    int hideUnslotPrompt;
    int hideDeleteSalvagePrompt;
    int hideUsefulSalvageWarning;
    int hideOpenSalvageWarning;
    int hideDeleteRecipePrompt;
    int hideFeePrompt;
    int hideInspirationFull;
    int hideSalvageFull;
    int hideRecipeFull;
    int hideEnhancementFull;
    int disableLoadingTips;
    int freeCamera;
    int    helpChatAdded;
    int enableJoystick;
    int hideLoyaltyTreeAccessButton;
    int hideStoreAccessButton;
    int autoFlipSuperPackCards;
    int hideConvertConfirmPrompt;
    int hideStorePiecesState;
    F32 cursorScale;

    int showEnemyTells;
    int showEnemyBroadcast;
    int hideEnemyLocal;
    int hideCoopPrompt;
    int staticColorsPerName;

    int declineSuperGroupInvite;
    int declineTradeInvite;

    int eShowArchetype;
    int eShowSupergroup;
    int eShowPlayerName;
    int eShowPlayerBars;
    int eShowVillainName;
    int eShowVillainBars;
    int eShowPlayerReticles;
    int eShowVillainReticles;
    int eShowAssistReticles;
    int eShowOwnerName;
    int eShowPlayerRating;
    int mousePitchOption;
    int attachArticle;
    
    int contactSortByName;
    int contactSortByZone;
    int contactSortByRelationship;
    int contactSortByActive;

    int recipeHideUnOwned;
    int recipeHideMissingParts;
    int recipeHideUnOwnedBench;
    int recipeHideMissingPartsBench;
    
    int deprecated;
    int reclaim_deprecated;
    unsigned int mapOptions;        // bitfield
    unsigned int mapOptions2;        // bitfield
    unsigned int mapOptionRevision;

    float tooltipDelay;

    int reverseMouseButtons;
    int disableCameraShake;
    int disableMouseScroll;

    int noXP;
    int noXPExemplar;
    int popHelpDisable;
    int forcedPopHelpDisable;
    int    newFeaturesVersion;
    int	passcode;

    int ArchitectNav;
    int ArchitectTips;
    int ArchitectAutoSave;
    int ArchitectBlockComments;
    U32 architectRecentSearches[4];
    U32 architectRecentPublishes[4];

    //---------------------------

    int            lfg;                // looking for group
    int            hidden;                // hide from lists
    char        comment[128];        // search comment field
    
    int            last_inviter_dbid;
    int            last_invite_time;

    int            league_accept_blocking_time;

    int            inviter_dbid;        // the person that invited them to a team, supergroup, or trade
    int            inviter_sgid;        // sgid of inviter - only set if command setting _dbid cares
    int            supergroup_mode;    // is player wearing supergroup costume?
    int            hide_supergroup_emblem; //player doesn't want to display their supergorup emblem

    int            *levelingpact_invites;

    int            taskforce_mode;             // is player working for a task force?
    int            pendingArchitectTickets; // count of queued up tickets
    int            architectMissionsCompleted; // bitfield to track which missions the player was there for the completion of.

    int            npc_costume;        // if non-zero, the index of the npc costume to use
    int            respecTokens;        // bitfield for respec tokens
    int            freeTailorSessions; // number of times the player can tailor without charge
    int            ultraTailor;
    int            prestige;            // cummulative prestige earned
    U32            timeLastInvalidArcWarning;    // the last dbSecondsSince2000 that the player got the mission architect invalid arc warning


    TradeData    *trade;

    int            current_costume;
    int            current_powerCust;
    int            current_supercostume;
    int            num_costume_slots;                // this is the number of extra costume slots unlocked in the game (not bought)
    int            num_costumes_stored;            // this is the number of costume slots that are persisted in the DB
    int            last_costume_change;
    Costume        *costume[MAX_COSTUMES];
    PowerCustomizationList *powerCustomizationLists[MAX_COSTUMES];
    Costume        *superCostume;

    int         pendingCostumeNumber;    // costume number "escrow" for costume change emotes
    Costume        *pendingCostume;        // costume struct "escrow" for costume change emotes
    U32            pendingCostumeForceTime;    // Client: timeout for delayed costume change
                                            // Server: timed flag that costume changes must be flagged as delayed.
#if CLIENT
    U32            pendingCostumeIgnoreTime;    // Client: timeout for ignoring spurious deferred costume changes
#endif

    int            validateCostume;    // if set, the primary costume should be validated on receipt of account inventory
    int            doNotKick;            // if set, the character will not be kicked for invalid cosutmes, ATs and Powersets

    SGColorBits superColorsPrimaryU; // these are bitfields
    SGColorBits superColorsSecondaryU;
    SGColorBits superColorsPrimary2U;
    SGColorBits superColorsSecondary2U;
    SGColorBits superColorsTertiaryU;
    SGColorBits superColorsQuaternaryU;

    DoorAnimState *door_anim;            // state if the player is animating through a door - client & server

    int                    titleThe;
    char                titleTheText[10];
    char                titleCommon[32];
    char                titleOrigin[32];
    char                titleSpecial[128];
    U32                    titleSpecialExpires;        // seconds since 2000 when the title expires. 0 for never.
    int                    iTitlesChosen;                // Remembers if they've chosen their titles yet.
    int                    titleBadge;                    // index to badge of title they've choosen
    int                    titleColors[2];

    char                costumeFxSpecial[128];
    U32                    costumeFxSpecialExpires;    // seconds since 2000 when the FX expires. 0 for never.

    U32                    old_auth_user_data[AUTH_DWORDS_ORIG];        // Deprecated, but has to be kept around
    U32                    auth_user_data[AUTH_DWORDS];    // stuff auth server knows about the user's account (where the game was bought, prestige powers, etc.)
    int                    account_inv_loaded;            // is the account inventory loaded from the account server
    AccountInventorySet    account_inventory;        // account inventory
    CertificationRecord ** ppCertificationHistory;
    U8                    loyaltyStatus[LOYALTY_BITS/8];            // loyalty reward status
    U32                    loyaltyPointsUnspent;                        
    U32                    loyaltyPointsEarned;
    U32                    virtualCurrencyBal;
    U32                    lastEmailTime;
    U32                    lastNumEmailsSent;
    U32                    accountInformationAuthoritative;
    U32                    accountInformationCacheTime;

    OrderId                **completed_orders;
    OrderId                **pending_orders;
    OrderId                pendingCertification; // a certification we have paid for, but not received
    U32                    last_certification_request;

    VisitedMap            **visited_maps;

    char                afk_string[MAX_AFK_STRING]; // string to display when AFK

    U32                    aiBadges[BADGE_ENT_MAX_BADGES]; // array of all badge status
    RecentBadge            recentBadges[BADGE_ENT_RECENT_BADGES];    // array of recently-won badges and their award times
    int                    debugBadgeLevel;            // 0 = normal, 1 = all known, 2 = all visible
    BadgeMonitorInfo    badgeMonitorInfo[MAX_BADGE_MONITOR_ENTRIES];
    U32                    popHelpStatus[POP_HELP_STATUS_SIZE];    // bit array of which pop help messages have been read/dismissed
    int                    popHelpLatest[POP_HELP_LATEST_SIZE]; //last pop helps received.
    #if CLIENT
     int                popHelpIsValid;        //Flag set when popHelp has actually been loaded from server
    #endif

    RewardDBStore        pendingReward;
    RewardDBStore        queuedRewards[MAX_REWARD_TABLES];
    char                pendingRewardSent[MAX_REWARD_CHOICE]; // used to prevent Moral Choice tables from spamming the net layer.
    int                    pendingRewardVisibleBitField;
    int*                pendingRewardDisabledBitField;

    int                    csrListener;

    int                    pvpSwitch;            // true if the player is interested in PvP content
    int                    bIsGanker;            // true if the player has a sub-ganker reputation

    int                    skillsUnlocked;            // true if player has visited the teacher for the first time.

    U32                    lastClientStatePktId;    // id of last client state packet (to avoid OO packets)

    U32*        baseRaids;                // earray of base raid ids that player belongs to

    int            detailInteracting;            // The base detail that player is interacting with
    char        workshopInteracting[32];    // The name of workshop the player is interacting with

    CombatMonitorStat    combatMonitorStats[MAX_COMBAT_STATS];
    char        chat_handle[32];

    char                primarySet[MAX_NAME_LEN];    // The category and powerset originally picked as primary
    char                secondarySet[MAX_NAME_LEN];    // And the one originally picked as secondary

    int            multiBuildsSetUp;

    char        buildNames[MAX_BUILD_NUM][BUILD_NAME_LEN];    // Names for builds.

    char        specialMapReturnData[SPECIAL_MAP_RETURN_DATA_LEN];    // used to return people from one static map to another.  Format is "tag=payload".
    int            specialMapReturnInProgress;

    RewardToken        **rewardTokens;
    RewardToken        **activePlayerRewardTokens;
    RewardToken        **ArchitectRewardTokens;

    int            displayAlignmentStatsToOthers;

    U32                lastTurnstileEventID;        // Last turnstile event I was in
    U32                lastTurnstileMissionID;        // Last mission instance I was in
    U32                lastTurnstileStartTime;        // Turnstile start time (so we don't trip over old sessions)
    int                helperStatus; // 0 - need to open a popup, 1 - newbie, 2 - mentor/veteran, 3 - standard

// server-only section

#ifndef SERVER
    KeyBind*    keybinds; // the keybindings for this particular entity
#else
    ServerKeyBind*    keybinds; // the keybindings for this particular entity

    Vec3        last_static_pos;        // position of player on the last static map
    Vec3        last_static_pyr;        // orientation of player on the last static map
    Vec3        last_good_pos[8];        // known good positions for stuck command

    char        fameStrings[NUM_FAME_STRINGS][MAX_FAME_STRING];    // strings that may be spoken by NPC's about this player
    U32            lastFameTime;            // dbseconds when last fame said or last movement occurred

    PlayerStats stats[kStatPeriod_Count];        // A variety of stats kept on the character.

    int            npcStore;                        // if non-zero, the player is at the store owned by this npc.

    int            banned;                            // true if player is banned. (needs to be an int since it goes through containerloadsave)
    char        accountServerLock[ORDER_ID_STRING_LENGTH];            // OrderId in string form
    char        shardVisitorData[SHARD_VISITOR_DATA_SIZE];            // data used during shard visitor transfer

    unsigned int            ignore_count;        // how many times they've been ignored on this map (not persisted)
    U32                        ignored_spam;        // how many counted ignores they've used on this map (not persisted)
    U32                        is_a_spammer;        // silently silence all chat (persisted in uiSettings2)
    bool                    send_spam_petition;    // waiting for a message to attach to a petition and start the ban (not persisted)

    unsigned int    uiDelayedInvokeID;        // ID of power being delayed.
    U32                timeDelayedExpire;        // The time that the offer expires.

    int            aiBadgeStats[BADGE_ENT_MAX_STATS];        // array of all badge statistics
    U32            aiBadgesOwned[BADGE_ENT_BITFIELD_SIZE];    // bit array of which badges are owned

    PetName         **petNames;
    ArenaRef        **arenaEvents;
    MapHistory        **mapHistory;

    int            arenaMapSide;                    // when on an arena map
    int            arenaMapDefeated;                // I have been defeated
    ArenaRef    arenaActiveEvent;                // the event I'm currently active in (if any)
                                                
    U32            lastTrickOrTreat;                // dbSecondsSince2000, not persisted

    U32            lastSGModeChange;                // dbSecondsSince2000
    U32            timeInSGMode;                    // seconds
                                                                           
    int         chatBeta;
                                                
    int            arena_paid;                        // id of the last arena paid for
    int            arena_paid_amount;                // amount of last payment
    int            arena_prize_amount;                // amount of last prize
    int            arenaTeleportTimer;                // used to drop players from events if they don't teleport
    int            arenaTeleportEventId;            // the eventid of the event the player would teleport to
    int            arenaTeleportUniqueId;            // the uniqueid of the event the player would teleport to

    int            arenaKioskId;                    // id into global kiosk array - the one I'm interacting with
    ArenaEvent** arenaKioskSend;                // buffer of events to send
    int            arenaKioskSendNext;                // next event to send

    U32            last_arena_death;                // time of death in arena (timerSecondsSince2000)
    U32            arena_lives_stock;                // number of lives left for respawning in the arena

    float        reputationPoints;                // The reputation score of this player
    U32            reputationUpdateTime;            // The seconds since 2000 when the reputation was last recalculated

    DefeatRecord    pvpDefeatList[MAX_DEFEAT_RECORDS];    // Track players that have killed me and when they did so
    GMailClaimState gmailClaimState[MAX_GMAIL];

    MARTY_RewardData MARTY_rewardData[MARTY_ExperienceTypeCount];

    int                architectMode;                    // What architect mode we're in

    EntityRef        workshopEntity;                // Ent ref if this workshop is an entity

    Vec3        volume_reject_pos;                // Used for 'volume reject' teleporting

    unsigned int    isRespeccing        : 1;    // true if player is in respec menu (to avoid recieving gifts)
    unsigned int    send_afk_string     : 1;    // true if the afk string has changed.
    unsigned int    at_kiosk            : 1;    // true if the kiosk is open
    unsigned int    levelling           : 1;    // true if player is levelling up.
    unsigned int    arenaReadyRoom      : 1;    // player is in the arena ready room (DOES NOTHING)
    unsigned int    arenaLobby            : 1;    // player is in the arena lobby
    unsigned int    poppedUpSchedEvent    : 1;    // event window has already been popped up
    unsigned int    arenaKioskOpen      : 1;    // player has the kiosk window open
    unsigned int    arenaKioskCheckDist : 1;    // keep checking player distance to kiosk (unset if using slash command)
    unsigned int    arenaAmCamera        : 1;    // player is currently a camera
    unsigned int    arenaDeathProcessed    : 2;    // tracks the various phases of arena resurrection (alive->handledeath->handlerespawn->tickphasetwo->alive)
    unsigned int    arenaKioskSendInit  : 1;    // send start packet for arenaKioskSend
    unsigned int    arenaTeleportAccepted :1;    // wants to be teleported to the lobby for scheduled event or invite
    unsigned int    at_auction            : 1;    // true if player is viewing auction 
    unsigned int    usingTeleportPower    : 1;    // true if player is using a teleport power
    unsigned int    architectKioskOpen  : 1;    // true if player is using  architect kiosk
    unsigned int    rejectPosVolume        : 1;    // true if current volume is somewhere the player cannot be reject-teleported back to
    unsigned int    rejectPosInited        : 1;    // true if we've set the reject-teleport destination at least once
    unsigned int    inTurnstileQueue    : 1;    // true if we're queued in the turnstile
    
    U32                homeDBID;                    // dbid on home shard, only relevant when visiting.
    U32                homeShard;                    // Home shard number.
    U32                remoteDBID;                    // Remote dbid when visiting.  This is a flag to the home shard that the character is remote.
    U32                remoteShard;                // Remote shard when visiting.
    U32                visitStartTime;                // secondsSince2000 at which the character started the visit.  Used to ensure they go back home eventually.
    U32                homeSGID;                    // SupergroupID on home shard
    U32                homeLPID;                    // Leveling pact ID on home shard

    int                desiredTeamNumber;            // Team number I want to be on in end game raid league
                                                //   if this is -2, I'm not waiting to be invited to a turnstile event
    U32                isTurnstileTeamLeader;        // Set me as the leader of my team
                                                //    0 is not a leader
                                                //    1 is team leader
                                                //    2 is league leader

    U32                turnstileTeamInterlock;        // Set to the time when this player did something that means we shouldn't allow turnstile or team operations
                                                // Times out after a rather arbitrary five seconds

    int                clickedZowieIndex;            // index of zowie I've clicked on

    int                        gmail_pending_mail_id;
    int                        gmail_pending_xact_id;
    GmailPendingStateEnum    gmail_pending_state;
    int                        gmail_pending_influence;
    int                        gmail_pending_requestTime;
    char                    gmail_pending_subject[MAX_GMAIL_SUBJECT*2];
    char                    *gmail_pending_body;                            // estring
    char                    gmail_pending_attachment[255];
    char                    gmail_pending_to[32];

    char                    gmail_pending_inventory[255];                    // tried to put this into inventory, but failed
    int                        gmail_pending_banked_influence;                    // tried to put this into inventory, but failed

    int                currentAccessibleContactIndex;    // not persisted

    SlotLock        slotLockLevel;

    char            webstore_link[9];    // null-terminated eight-character string

#endif // SERVER

    // Fields not persisted to DB ////////////////////////////////////////////////

    bool isBeingCritter;            // set through 'becritter' command

    // Bitfields at the end ////////////////////////////////////////////////

    unsigned int inArchitectArea;     // true if player is using  architect, count of # of areas you're in, because of enter/exit ordering

    unsigned int afk : 1;             // true if player is away from keyboard
    unsigned int lwc_stage : 3;          // 0-4, value of current staged install

    unsigned int webHideBasics;       // Hide basic info (XP, Debt) from others on web page
    unsigned int webHideStats;        // Hide kiosk stats from others on web page
    unsigned int webHidePowers;       // Hide powers from others on web page
    unsigned int webHideBadges;       // Hide badges from others on web page
    unsigned int webHideFriends;      // Hide friends from others on web page
    unsigned int disableInGameAdvertizing;   // PLayers in game advertizing setting
    unsigned int musicTrack;	// Last music track heard in a supergroup base.
} EntPlayer;

#endif // __ENTPLAYER_H
