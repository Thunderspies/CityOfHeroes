// ZONE EVENT SCRIPT

// This script runs Scripted Zone Events.

// For handling displayStrings for the compass / ui window, take a look at
// textStdUnformattedConst() to get the string complete with {xyz} sequences

#include "script/scriptutil.h"
#include "script/scriptengine.h"
#include "storyarc/mission.h"
#include <utilitieslib/utils/eval.h>
#include <utilitieslib/utils/timing.h>
#include "gameComm/svr_chat.h"
#include <utilitieslib/utils/file.h>
#include "entity/character_eval.h"
#include <utilitieslib/components/StashTable.h>
#include "malloc.h"
#include <utilitieslib/language/MessageStoreUtil.h>
#include "ScriptedZoneEvent.h"
#include "script/Scripthook/ScriptHookInternal.h"
#include "entity/character_karma.h"
#include "ScriptedZoneEventKarma.h"
#include "cmdparse/cmdserver.h"
#include "dbcomm/staticMapInfo.h"
#include "dbcomm/dbcomm.h"
#include "entity/character_inventory.h"
#include "group/groupdbmodify.h"
#include <utilitieslib/utils/log.h>
#include "gameSys/Turnstile.h"
#include "player/pmotion.h"

#include "AutoGen/ScriptedZoneEvent_c_ast.h"
#include "dbcomm/logcomm.h"

#define UISTATE_GLOBAL                        0
#define UISTATE_BYVOLUME                    1

#define BOSS_MAX_HEALTH 100
#define STAGETHRESH    "StageThresh"

// Definition  of a script waypoint.
typedef struct ScriptWaypoint
{
    char *waypointName;                        // Name of this waypoint definition, used to reference it
    char *icon;                                // name of tga files to use as the minimap icon
    char *text;                                // text tag for the waypoint
    int compass;                            // display on compass
    struct ScriptWaypoint *next;            
} ScriptWaypoint;

static TokenizerParseInfo ParseWaypoint[] =
{
    { ".",                        TOK_STRUCTPARAM | TOK_STRING(ScriptWaypoint, waypointName, 0)        },
    { ".",                        TOK_STRUCTPARAM | TOK_STRING(ScriptWaypoint, icon, 0)                },
    { ".",                        TOK_STRUCTPARAM | TOK_STRING(ScriptWaypoint, text, 0)                },
    { ".",                        TOK_STRUCTPARAM | TOK_INT(ScriptWaypoint, compass, 0)                },
    { "\n",                        TOK_END, 0                                                            },
    { "", 0, 0 }
};

// This handles placing a set of waypoints based on a ScriptWaypoint.
typedef struct ScriptWaypointPlace
{
    char *waypoint;                            // Name of the ScriptWaypoint to use
    char *location;                            // Location to place this at, i.e. the name of the target scriptmarker
    int *mapWaypoints;                        // earray of Waypoint ID's
    struct ScriptWaypointPlace *next;
} ScriptWaypointPlace;

static TokenizerParseInfo ParseWaypointPlace[] =
{
    { ".",                        TOK_STRUCTPARAM | TOK_STRING(ScriptWaypointPlace, waypoint, 0),                        },
    { ".",                        TOK_STRUCTPARAM | TOK_STRING(ScriptWaypointPlace, location, 0)                        },
    { "\n",                        TOK_END, 0                                                            },
    { "", 0, 0 }
};

// This handles all aspects of a glowie.  The TPI reads in some of the members of this, the rest are initialised to 0 / NULL
// The "non-TPI" values are used to process the glowie at run time.
typedef struct ScriptGlowie
{
    char *glowieName;                        // Internal name of the glowie, used to identify it in the script
    char *displayName;                        // Display name.
    char *location;                            // Base name of the script markers where this will land.  Full name of script markers is "'location'_NN", NN is a two digit number starting at 01
    char **quantityStr;                        // Quantity to place, as given in the script.  This can be less than the number of markers available.
    int quantity;
    int numLoc;                                // Number of locations that exist on the map.  Determined at runtime, by counting up all the script markers
    int resetDelay;                            // Length of time in seconds before a completed glowie resets
    char *model;                            // Name of the model to use for this
    char *charRequires;                        // Requires statement you must pass to be able to interact with the glowie.  See MissionObjective::charRequires.
    char *charRequiresFailedText;            // String to display if the player fails the charRequires.  Will default if NULL.
    char *interact;                            // Interact text
    char *interrupt;                        // Interrupt text
    char *complete;                            // Complete text
    char *barText;                            // Progress bar text
    char *guardSpawn;                        // Spawn group to fire to guard this glowie
    char *guardProtect;                        // String present to use if the guards have to be killed first
    char *waypoint;                            // Waypoint used to show where these glowies are
    char *reward;                            // Reward handed out to whoever actually clicks on the glowie
    char *effectInactive;                    // FX strings to correspond to the special fx to play when glowie is in specified state
    char *effectActive;
    char *effectCooldown;
    char *effectCompletion;
    char *effectFailure;
    int interactTime;                        // Interact duration
    U8 vanishOnComplete;                    // Determine what the glowie does when it's completed.  Does it vanish, or just stay put and stop glowing
    U8 rememberLocations;                    // Remember which glowie locations have been used, and give preference to others the next time they're used
    U8 isMarked;                            // flag if active glowies get a waypoint
    struct ScriptGlowie *next;                // Link to next glowie, used for easy searching
    GLOWIEDEF glowieDef;                    // Pointer to script GLOWIEDEF
    ScriptWaypointPlace placedWaypoint;        // Holding tank for waypoints;
    ENTITY *glowies;                        // Array of glowie ENTITY structs
    U32 *glowieTimers;                        // Array of timers
    U32 *glowieSpots;                        // Array of location indices
    U8 *glowieUsed;                            // Array of used flags
} ScriptGlowie;

static TokenizerParseInfo ParseGlowie[] =
{
    { "{",                        TOK_START, 0                                                        },
    { "",                        TOK_STRUCTPARAM | TOK_STRING(ScriptGlowie, glowieName, 0)            },
    { "DisplayName",            TOK_STRING(ScriptGlowie, displayName, 0)                            },
    { "Location",                TOK_STRING(ScriptGlowie, location, 0)                                },
    { "Quantity",                TOK_STRINGARRAY(ScriptGlowie, quantityStr)                            },
    { "ResetDelay",                TOK_INT(ScriptGlowie, resetDelay, 0)                                },
    { "Model",                    TOK_STRING(ScriptGlowie, model, 0)                                    },
    { "Interact",                TOK_STRING(ScriptGlowie, interact, 0)                                },
    { "Interrupt",                TOK_STRING(ScriptGlowie, interrupt, 0)                                },
    { "Complete",                TOK_STRING(ScriptGlowie, complete, 0)                                },
    { "BarText",                TOK_STRING(ScriptGlowie, barText, 0)                                },
    { "GuardSpawn",                TOK_STRING(ScriptGlowie, guardSpawn, 0)                                },
    { "GuardProtect",            TOK_STRING(ScriptGlowie, guardProtect, 0)                            },
    { "Waypoint",                TOK_STRING(ScriptGlowie, waypoint, 0)                                },
    { "Reward",                    TOK_STRING(ScriptGlowie, reward, 0)                                    },
    { "InteractTime",            TOK_INT(ScriptGlowie, interactTime, 0)                                },
    { "VanishOnComplete",        TOK_BOOLFLAG(ScriptGlowie, vanishOnComplete, 0)                        },
    { "Rememberlocations",        TOK_BOOLFLAG(ScriptGlowie, rememberLocations, 0)                    },
    { "EffectInactive",            TOK_STRING(ScriptGlowie, effectInactive, 0)                            },
    { "EffectActive",            TOK_STRING(ScriptGlowie, effectActive, 0)                            },
    { "EffectCooldown",            TOK_STRING(ScriptGlowie, effectCooldown, 0)                            },
    { "EffectCompletion",        TOK_STRING(ScriptGlowie, effectCompletion, 0)                        },
    { "EffectFailure",            TOK_STRING(ScriptGlowie, effectFailure, 0)                            },
    { "CharRequires",            TOK_STRING(ScriptGlowie, charRequires, 0)                            },
    { "CharRequiresFailedText",    TOK_STRING(ScriptGlowie, charRequiresFailedText, 0)                    },
    { "}",                        TOK_END, 0                                                            },
    { "", 0, 0 }
};

AUTO_ENUM;
typedef enum glowieControlType
{
    glowieControl_Place,
    glowieControl_Remove,
    glowieControl_Activate,
    glowieControl_Deactivate,
    glowieControl_Mark,
    glowieControl_ClearMemory,
} glowieControlType;

typedef struct ScriptGlowieControl
{
    char *controlName;                        // Name of this control block
    glowieControlType type;                    // What we're doing to the glowie - see enum glowieControlType just above
    char *glowieName;                        // Name of the glowie we're controlling
} ScriptGlowieControl;

static TokenizerParseInfo ParseGlowieControl[] =
{
    { "{",                        TOK_START, 0                                                        },
    { "",                        TOK_STRUCTPARAM | TOK_STRING(ScriptGlowieControl, controlName, 0)    },
    { "Type",                    TOK_INT(ScriptGlowieControl, type, 0), glowieControlTypeEnum        },
    { "Glowie",                    TOK_STRING(ScriptGlowieControl, glowieName, 0)                        },
    { "}",                        TOK_END, 0                                                            },
    { "", 0, 0 }
};

typedef struct ScriptSpawn
{
    char *spawnName;                        // Name of this spawn definition
    char *location;                            // Location - this hooks to properties in the appropriate spawndef
    char *group;                            // Group - this also hooks to the spawndef
    char *team;                                // Team to place these in, this is the name of a team specified in a ScriptTeamRespawn
    char *appearBehavior;                    // Optional behavior to play when they spawn in
    char *moveToMarker;                        // Optional marker for them to move to
    U8 friendly;                            // Flag, if set they con friendly to the players.  This could be used to create defendable critters.
    char *killBehavior;                        // Optional behavior to play to kill them
    char *spawnDef;                            // Spawndef to use
    struct ScriptSpawn *next;                // Internal - link to next ScriptSpawn
    int active;                                // Internal - flag if they're currently active or not
    ENCOUNTERGROUP *groups;                    // Internal - earray of encountergroups
} ScriptSpawn;

static TokenizerParseInfo ParseSpawn[] =
{
    { "{",                        TOK_START, 0                                                        },
    { "",                        TOK_STRUCTPARAM | TOK_STRING(ScriptSpawn, spawnName, 0)                },
    { "Location",                TOK_STRING(ScriptSpawn, location, 0)                                },
    { "Group",                    TOK_STRING(ScriptSpawn, group, 0)                                    },
    { "Team",                    TOK_STRING(ScriptSpawn, team, 0)                                    },
    { "SpawnBehavior",            TOK_STRING(ScriptSpawn, appearBehavior, 0)                            },
    { "MoveToMarker",            TOK_STRING(ScriptSpawn, moveToMarker, 0)                            },
    { "Friendly",                TOK_BOOLFLAG(ScriptSpawn, friendly, 0)                                },
    { "KillBehavior",            TOK_STRING(ScriptSpawn, killBehavior, 0)                            },
    { "}",                        TOK_END, 0                                                            },
    { "", 0, 0 }
};

typedef struct ScriptTeamRespawnSize
{
    int playerCount;                        // The player counts used to determine the proper minimum and full counts.  The last value that is less than or equal to the current player count is used.
    int minimumCount;                        // The minimum counts of critters in the team.  If time > min and time < max and current count < min count, a fountain wave is triggered.
    int fullCount;                            // The full counts of critters in the team.  Whenever a fountain wave is triggered, spawns are repopulated until the total count of creatures on the team is >= full count.
} ScriptTeamRespawnSize;

typedef struct ScriptTeamRespawn
{
    char *respawnName;                        // Name of this respawn def.
    char *teamName;                            // Name of the team (this is not a script TEAM name)
    char *creator;                            // Name of the boss that we think created us, which means we follow that guy
    char *petTeam;                            // When a member of the team in teamName creates a pet, place the pet on this team
    int minRespawnTime;                        // The minimum amount of time before the team can trigger a fountain wave if in fountain spawn mode.  At this point, minimum counts are checked to determine if the wave should trigger.
    int maxRespawnTime;                        // The maximum amount of time before the team can trigger a fountain wave if in fountain spawn mode.  At this point, the minimum counts are ignored.
    ScriptTeamRespawnSize **teamSizeCounts;    // teamSizeCounts are optional.  If not specified, this is not a fountain spawn.  If specified, all arrays must be the same length.
    int minimumCount;                        // DEPRECATED.  Replaced with teamSizeCounts.
    struct ScriptTeamRespawn *next;            // Internal - link to next
    int active;                                // Internal - is this team active?
    int spawnCount;                            // Internal - number of ScriptSpawns in this team
    int tickCount;                            // Tick count used to count up to heartbeat
    const char *creatorEnt;                    // Internal - if creator is specified, this gets the ENTITY for the critter in question.
    int flags;                                // Team spawn flags
} ScriptTeamRespawn;

AUTO_ENUM;
typedef enum teamRespawnFlags
{
    teamRespawnFlags_UsePlayerCount = 1,
    teamRespawnFlags_PlaceHolder = 1 << 1,            //    the autostruct doesn't like having only 1 flag
} teamRespawnFlags;

static TokenizerParseInfo ParseTeamRespawnSize[] =
{
    { ".",                    TOK_STRUCTPARAM | TOK_INT(ScriptTeamRespawnSize, playerCount, 0)        },
    { ".",                    TOK_STRUCTPARAM | TOK_INT(ScriptTeamRespawnSize, minimumCount, 0)        },
    { ".",                    TOK_STRUCTPARAM | TOK_INT(ScriptTeamRespawnSize, fullCount, 0)            },
    { "\n",                    TOK_END, 0                                                                },
    { "", 0, 0 }
};

static TokenizerParseInfo ParseTeamRespawn[] =
{
    { "{",                        TOK_START, 0                                                        },
    { "",                        TOK_STRUCTPARAM | TOK_STRING(ScriptTeamRespawn, respawnName, 0)        },
    { "Team",                    TOK_STRING(ScriptTeamRespawn, teamName, 0)                            },
    { "Creator",                TOK_STRING(ScriptTeamRespawn, creator, 0)                            },
    { "PetTeam",                TOK_STRING(ScriptTeamRespawn, petTeam, 0)                            },
    { "MinRespawnTime",            TOK_INT(ScriptTeamRespawn, minRespawnTime, 0)                        },
    { "HeartBeat",                TOK_REDUNDANTNAME | TOK_INT(ScriptTeamRespawn, minRespawnTime, 0)    },
    { "MaxRespawnTime",            TOK_INT(ScriptTeamRespawn, maxRespawnTime, 0)                        },
    { "RespawnMinCount",        TOK_INT(ScriptTeamRespawn, minimumCount, 0)                            },
    { "Count",                    TOK_STRUCT(ScriptTeamRespawn, teamSizeCounts, ParseTeamRespawnSize)    },
    { "RespawnRate",            TOK_IGNORE                                                            },
    { "Flags",                    TOK_FLAGS(ScriptTeamRespawn, flags, 0), teamRespawnFlagsEnum    },
    { "}",                        TOK_END, 0                                                            },
    { "", 0, 0 }
};

AUTO_ENUM;
typedef enum teamControlType
{
    teamControl_Activate,
    teamControl_Deactivate,
    teamControl_DoBehavior,
    teamControl_Detach,
    teamControl_ForceSpawn,
    teamControl_SetHealth,
} teamControlType;

typedef struct ScriptTeamControl
{
    char *controlName;                        // Name of this control
    teamControlType type;                    // Type - see teamControlType just above
    char *team;                                // Team to apply this to
    char *teamGroup;                        // TeamGroup to apply this to
    char *behavior;                            // Optional behavior applied for teamControl_DoBehavior
    FRACTION health;                        // Health % to set the first entity (boss) to for teamControl_SetHealth
} ScriptTeamControl;

static TokenizerParseInfo ParseTeamControl[] =
{
    { "{",                        TOK_START, 0                                                        },
    { "",                        TOK_STRUCTPARAM | TOK_STRING(ScriptTeamControl, controlName, 0)        },
    { "Type",                    TOK_INT(ScriptTeamControl, type, 0), teamControlTypeEnum            },
    { "Team",                    TOK_STRING(ScriptTeamControl, team, 0)                                },
    { "TeamGroup",                TOK_STRING(ScriptTeamControl, teamGroup, 0)                            },
    { "Behavior",                TOK_STRING(ScriptTeamControl, behavior, 0)                            },
    { "Health",                    TOK_F32(ScriptTeamControl, health, 1.0f)                                },
    { "}",                        TOK_END, 0                                                            },
    { "", 0, 0 }
};

typedef struct ScriptTeamGroup
{
    char *name;                                // teamGroup definition to affect
    char **addTeams;                        // list of teams to add to the teamGroup
    char **removeTeams;                        // list of teams to remove from this teamGroup
} ScriptTeamGroup;

static TokenizerParseInfo ParseTeamGroup[] =
{
    { "{",                        TOK_START, 0                                                        },
    { "",                        TOK_STRUCTPARAM | TOK_STRING(ScriptTeamGroup, name, 0)                },
    { "AddTeams",                TOK_STRINGARRAY(ScriptTeamGroup, addTeams)                            },
    { "RemoveTeams",            TOK_STRINGARRAY(ScriptTeamGroup, removeTeams)                        },
    { "}",                        TOK_END, 0                                                            },
    { "", 0, 0 }
};

typedef struct ScriptDoor
{
    char *doorName;                            // Name of the door, this matches an actual door in the map
    struct ScriptDoor *next;                // Internal - link to next
    int state;                                // State of door: locked or unlocked
} ScriptDoor;

static TokenizerParseInfo ParseDoor[] =
{
    { ".",                        TOK_STRUCTPARAM | TOK_STRING(ScriptDoor, doorName, 0)                },
    { "\n",                        TOK_END, 0                                                            },
    { "", 0, 0 }
};

AUTO_ENUM;
typedef enum doorControlType
{
    doorControl_Unlock,
    doorControl_Lock,
} doorControlType;

typedef struct ScriptDoorControl
{
    char *controlName;                        // Name of this control
    doorControlType type;                    // What we're doing: see doorControlType above
    char *doorName;                            // Door we're doing it to.

} ScriptDoorControl;

static TokenizerParseInfo ParseDoorControl[] =
{
    { "{",                        TOK_START, 0                                                        },
    { "",                        TOK_STRUCTPARAM | TOK_STRING(ScriptDoorControl, controlName, 0)        },
    { "Type",                    TOK_INT(ScriptDoorControl, type, 0), doorControlTypeEnum            },
    { "Door",                    TOK_STRING(ScriptDoorControl, doorName, 0)                            },
    { "}",                        TOK_END, 0                                                            },
    { "", 0, 0 }
};

typedef struct ScriptPort
{
    char *portName;
    struct ScriptPort *next;
    int active;
} ScriptPort;

static TokenizerParseInfo ParsePort[] =
{
    { ".",                        TOK_STRUCTPARAM | TOK_STRING(ScriptPort, portName, 0)                },
    { "\n",                        TOK_END, 0                                                            },
    { "", 0, 0 }
};

AUTO_ENUM;
typedef enum portControlType
{
    portControl_Activate,
    portControl_Deactivate,
} portControlType;

typedef struct ScriptPortControl
{
    char *controlName;
    portControlType type;
    char *portName;
} ScriptPortControl;

static TokenizerParseInfo ParsePortControl[] =
{
    { "{",                        TOK_START, 0                                                        },
    { "",                        TOK_STRUCTPARAM | TOK_STRING(ScriptPortControl, controlName, 0)        },
    { "Type",                    TOK_INT(ScriptPortControl, type, 0), portControlTypeEnum            },
    { "Porter",                    TOK_STRING(ScriptPortControl, portName, 0)                            },
    { "}",                        TOK_END, 0                                                            },
    { "", 0, 0 }
};

typedef struct ScriptGurney
{
    char *gurneyName;
    char *volume;
    struct ScriptGurney *next;
    int active;
} ScriptGurney;

static TokenizerParseInfo ParseGurney[] =
{
    { "{",                        TOK_START, 0                                                        },
    { "",                        TOK_STRUCTPARAM | TOK_STRING(ScriptGurney, gurneyName, 0)            },
    { "Volume",                    TOK_STRING(ScriptGurney, volume, 0)                                    },
    { "}",                        TOK_END, 0                                                            },
    { "", 0, 0 }
};

AUTO_ENUM;
typedef enum gurneyControlType
{
    gurneyControl_EnableRezWindow,
    gurneyControl_DisableRezWindow,
    gurneyControl_Activate,
    gurneyControl_Deactivate,
} gurneyControlType;

typedef struct ScriptGurneyControl
{
    char *controlName;
    gurneyControlType type;
    char *gurneyName;
} ScriptGurneyControl;

static TokenizerParseInfo ParseGurneyControl[] =
{
    { "{",                        TOK_START, 0                                                        },
    { "",                        TOK_STRUCTPARAM | TOK_STRING(ScriptGurneyControl, controlName, 0)    },
    { "Gurney",                    TOK_STRING(ScriptGurneyControl, gurneyName, 0)                        },
    { "Type",                    TOK_INT(ScriptGurneyControl, type, 0), gurneyControlTypeEnum        },
    { "}",                        TOK_END, 0                                                            },
    { "", 0, 0 }
};

typedef struct ScriptSound
{
    char *soundName;
    char *fileName;
    int channel;
    float volumeLevel;
    char *volumeName;
} ScriptSound;

StaticDefineInt ParseSoundChannelEnum[] =
{
    DEFINE_INT
    { "Music",        SOUND_MUSIC        },
    { "SFX",        SOUND_GAME        },
    { "Voiceover",    SOUND_VOICEOVER    },
    DEFINE_END
};

static TokenizerParseInfo ParseSound[] =
{
    { "{",                        TOK_START, 0                                                        },
    { "",                        TOK_STRUCTPARAM | TOK_STRING(ScriptSound, soundName, 0)                },
    { "Sound",                    TOK_STRING(ScriptSound, fileName, 0)                                },
    { "Channel",                TOK_INT(ScriptSound, channel, SOUND_MUSIC), ParseSoundChannelEnum    },
    { "VolumeLevel",            TOK_F32(ScriptSound, volumeLevel, 1.0f)                                },
    { "VolumeName",                TOK_STRING(ScriptSound, volumeName, 0)                                },
    { "}",                        TOK_END, 0                                                            },
    { "", 0, 0 }
};

typedef struct ScriptFadeSound
{
    char *soundName;
    int channel;
    float fadeTime;
    char *volumeName;
} ScriptFadeSound;

static TokenizerParseInfo ParseFadeSound[] = 
{
    { "{",                        TOK_START, 0                                                        },
    { "",                        TOK_STRUCTPARAM | TOK_STRING(ScriptFadeSound, soundName, 0)                },
    { "Channel",                TOK_INT(ScriptFadeSound, channel, SOUND_MUSIC), ParseSoundChannelEnum    },
    { "FadeTime",                TOK_F32(ScriptFadeSound, fadeTime, 1.0f)                                },
    { "VolumeName",                TOK_STRING(ScriptFadeSound, volumeName, 0)                                },
    { "}",                        TOK_END, 0                                                            },
    { "", 0, 0 }
};

typedef struct ScriptFloater
{
    char *floaterName;
    char *message;
    char *color;
    char *volume;
    int alertType;
} ScriptFloater;

StaticDefineInt SZE_floaterAttentionTypes[] =
{
    DEFINE_INT
    { "Attention", kFloaterStyle_Attention},
    { "PriorityAlert", kFloaterStyle_PriorityAlert},
    DEFINE_END
};
static TokenizerParseInfo ParseFloater[] =
{
    { "{",                        TOK_START, 0                                                        },
    { "",                        TOK_STRUCTPARAM | TOK_STRING(ScriptFloater, floaterName, 0)            },
    { "Message",                TOK_STRING(ScriptFloater, message, 0)                                },
    { "Color",                    TOK_STRING(ScriptFloater, color, 0)                                    },
    { "Volume",                    TOK_STRING(ScriptFloater, volume, 0)                                },
    { "AlertType",                TOK_INT(ScriptFloater, alertType, kFloaterStyle_Attention), SZE_floaterAttentionTypes        },
    { "}",                        TOK_END, 0                                                            },
    { "", 0, 0 }
};

typedef struct ScriptCaption
{
    char *captionName;
    char *message;
    char *volume;
} ScriptCaption;

static TokenizerParseInfo ParseCaption[] =
{
    { "{",                        TOK_START, 0                                                        },
    { "",                        TOK_STRUCTPARAM | TOK_STRING(ScriptCaption, captionName, 0)            },
    { "Message",                TOK_STRING(ScriptCaption, message, 0)                                },
    { "Volume",                    TOK_STRING(ScriptCaption, volume, 0)                                },
    { "}",                        TOK_END, 0                                                            },
    { "", 0, 0 }
};

static TokenizerParseInfo ParseReward[] =
{
    { "{",                        TOK_START, 0                                                        },
    { "",                        TOK_STRUCTPARAM | TOK_STRING(ScriptReward, rewardName, 0)            },
    { "Class",                    TOK_INT(ScriptReward, deprecated, 0)                                },
    { "PointsNeeded",            TOK_INT(ScriptReward, pointsNeeded, 0)                                },
    { "PercentileNeeded",        TOK_F32(ScriptReward, percentileNeeded, -1)                            },
    { "NumStagesRequired",        TOK_INT(ScriptReward, numStagesRequired, -1)                        },
    { "Count",                    TOK_INT(ScriptReward, deprecated, 0)                                },
    { "Display",                TOK_BOOLFLAG(ScriptReward, deprecatedChar, 0)                            },
    { "RewardTable",            TOK_STRING(ScriptReward, rewardTable, 0)                            },
    { "}",                        TOK_END, 0                                                            },
    { "", 0, 0 }
};

typedef struct ScriptRewardGroup
{
    char *rewardGroupName;
    char *bonusGroup;    //    deprcated as well
    ScriptReward **rewards;
    int deprecated;
    U8 minorStageReward;        //    minor do not reset karma and do not require full participation
} ScriptRewardGroup;
extern StaticDefineInt BoolEnum[];
static TokenizerParseInfo ParseRewardGroup[] =
{
    { "{",                        TOK_START, 0                                                        },
    { "",                        TOK_STRUCTPARAM | TOK_STRING(ScriptRewardGroup, rewardGroupName, 0)    },
    { "RandomRoll",                TOK_INT(ScriptRewardGroup, deprecated, 0)                            },
    { "EventMax",                TOK_INT(ScriptRewardGroup, deprecated, 0)                            },
    { "BonusGroup",                TOK_STRING(ScriptRewardGroup, bonusGroup, 0)                        },
    { "MinorStageReward",        TOK_BOOLFLAG(ScriptRewardGroup, minorStageReward, 0),                 },
    { "Reward",                    TOK_STRUCT(ScriptRewardGroup, rewards, ParseReward)                    },
    { "}",                        TOK_END, 0                                                            },
    { "", 0, 0 }
};

typedef struct ScriptTimer
{
    char *timerName;
    char *duration;
    char *minDuration;
    char *maxDuration;
    U8 displayFlag;
    U32 endTime;
    U32 origDuration;
    char *uiTimerVariable;
    int isActionTimer;            // If true, this timer was created by an action.  If false, it was declared statically
    int isEventTimer;            // If true, this timer is independent of stages.  If false, it is tied to a specific stage
                                // Both isActionTimer and isEventTimer can be true simultaneously.  They are independent variables.
    int bonusTime;
} ScriptTimer;

static TokenizerParseInfo ParseTimer[] =
{
    { "{",                        TOK_START, 0                                                        },
    { "",                        TOK_STRUCTPARAM | TOK_STRING(ScriptTimer, timerName, 0)                },
    { "Duration",                TOK_STRING(ScriptTimer, duration, 0)                                },
    { "MinDuration",            TOK_STRING(ScriptTimer, minDuration, 0)                                },
    { "MaxDuration",            TOK_STRING(ScriptTimer, maxDuration, 0)                                },
    { "Display",                TOK_BOOLFLAG(ScriptTimer, displayFlag, 0)                            },
    { "}",                        TOK_END, 0                                                            },
    { "", 0, 0 }
};

AUTO_ENUM;
typedef enum timerControlType
{
    timerControl_Add,
    timerControl_Subtract,
    timerControl_Reset,
    timerControl_Delete,
} timerControlType;

typedef struct ScriptTimerControl
{
    char *controlName;
    char *timerName;
    timerControlType type;
    char *duration;
} ScriptTimerControl;

static TokenizerParseInfo ParseTimerControl[] =
{
    { "{",                        TOK_START, 0                                                        },
    { "",                        TOK_STRUCTPARAM | TOK_STRING(ScriptTimerControl, controlName, 0)    },
    { "Timer",                    TOK_STRING(ScriptTimerControl, timerName, 0)                        },
    { "Type",                    TOK_INT(ScriptTimerControl, type, 0), timerControlTypeEnum            },
    { "Duration",                TOK_STRING(ScriptTimerControl, duration, 0)                            },
    { "}",                        TOK_END, 0                                                            },
    { "", 0, 0 }
};

// This handles awarding a badgestat based on how long the event took
typedef struct ScriptEventTimer
{
    char *time;                                // Duration of the event
    char *badgestat;                        // Badge Stat to award
} ScriptEventTimer;

static TokenizerParseInfo ParseEventTimer[] =
{
    { ".",                        TOK_STRUCTPARAM | TOK_STRING(ScriptEventTimer, time, 0),            },
    { ".",                        TOK_STRUCTPARAM | TOK_STRING(ScriptEventTimer, badgestat, 0)        },
    { "\n",                        TOK_END, 0                                                            },
    { "", 0, 0 }
};

StaticDefineInt TrialStatusEnum[] =
{
    DEFINE_INT
    { "Success",    trialStatus_Success},
    { "Failure",    trialStatus_Failure},
    DEFINE_END
};

typedef struct ScriptAction
{
    char *actionName;
    char *debugStr;
    char *logStr;
    char *slashCommand;
    char *bling;    // Deprecated
    char *delay;
    char *objective;
    char **reset;
    char **resetObjs;
    char **setScriptValue;
    char **setMapToken;
    char *removeMapToken;
    char **setInstanceToken;
    char *removeInstanceToken;
    char **groupsToShow;
    char **groupsToHide;
    ScriptGlowie **glowies;
    ScriptGlowieControl **glowieControls;
    ScriptSpawn **spawns;
    ScriptTeamRespawn **teams;
    ScriptTeamControl **teamControls;
    ScriptTeamGroup **teamGroups;
    ScriptDoor **doors;
    ScriptDoorControl **doorControls;
    ScriptPort **ports;
    ScriptPortControl **portControls;
    ScriptGurney **gurneys;
    ScriptGurneyControl **gurneyControls;
    ScriptSound **sounds;
    ScriptFadeSound **soundFades;
    ScriptFloater **floaters;
    ScriptCaption **captions;
    ScriptRewardGroup **rewardGroup;
    int zeMinPlayerLevel;
    int zeMaxPlayerLevel;
    ScriptWaypoint **waypoints;
    ScriptWaypointPlace **placedWaypoints;
    ScriptTimer **timers;
    ScriptTimer **eventTimers;
    ScriptTimerControl **timerControls;
    ScriptEventTimer **eventTimer;
    TrialStatus completeTrial;
    U8 UIChange;
    U8 resetReward;
    U8 removeWaypoints;
    U8 eventTimerStart;
} ScriptAction;

typedef struct DelayedScriptAction
{
    int delayActivationTime;
    ScriptAction *action;
} DelayedScriptAction;

static TokenizerParseInfo ParseAction[] =
{
    { "{",                        TOK_START, 0                                                        },
    { "",                        TOK_STRUCTPARAM | TOK_STRING(ScriptAction, actionName, 0)            },
    { "DebugPrint",                TOK_STRING(ScriptAction, debugStr, 0)                                },
    { "Log",                    TOK_STRING(ScriptAction, logStr, 0)                                    },
    { "SlashCommand",            TOK_STRING(ScriptAction, slashCommand, 0)                            },
    { "Reward",                    TOK_STRING(ScriptAction, bling, 0)                                    },
    { "Delay",                    TOK_STRING(ScriptAction, delay, 0)                                    },
    { "Objective",                TOK_STRING(ScriptAction, objective, 0)                                },
    { "ResetObjectives",        TOK_STRINGARRAY(ScriptAction, resetObjs)                            },
    { "Reset",                    TOK_STRINGARRAY(ScriptAction, reset)                                },
    { "SetScriptValue",            TOK_STRINGARRAY(ScriptAction, setScriptValue)                        },
    { "SetMapToken",            TOK_STRINGARRAY(ScriptAction, setMapToken)                            },
    { "RemoveMapToken",            TOK_STRING(ScriptAction, removeMapToken, 0)                            },
    { "SetInstanceToken",        TOK_STRINGARRAY(ScriptAction, setInstanceToken)                        },
    { "RemoveInstanceToken",    TOK_STRING(ScriptAction, removeInstanceToken, 0)                    },
    { "GroupShow",                TOK_STRINGARRAY(ScriptAction, groupsToShow)                            },
    { "GroupHide",                TOK_STRINGARRAY(ScriptAction, groupsToHide)                            },
    { "UIChange",                TOK_BOOLFLAG(ScriptAction, UIChange, 0)                                },
    { "ResetReward",            TOK_BOOLFLAG(ScriptAction, resetReward, 0)                            },
    { "RemoveWaypoints",        TOK_BOOLFLAG(ScriptAction, removeWaypoints, 0)                        },
    { "EventTimerStart",        TOK_BOOLFLAG(ScriptAction, eventTimerStart, 0)                        },
    { "TrialComplete",            TOK_INT(ScriptAction, completeTrial, trialStatus_None), TrialStatusEnum    },
    { "GlowieCreate",            TOK_STRUCT(ScriptAction, glowies, ParseGlowie)                        },
    { "GlowieControl",            TOK_STRUCT(ScriptAction, glowieControls, ParseGlowieControl)        },
    { "SpawnCreate",            TOK_STRUCT(ScriptAction, spawns, ParseSpawn)                        },
    { "TeamCreate",                TOK_STRUCT(ScriptAction, teams, ParseTeamRespawn)                    },
    { "TeamControl",            TOK_STRUCT(ScriptAction, teamControls, ParseTeamControl)            },
    { "TeamGroup",                TOK_STRUCT(ScriptAction, teamGroups, ParseTeamGroup)                },
    { "DoorCreate",                TOK_STRUCT(ScriptAction, doors, ParseDoor)                            },
    { "DoorControl",            TOK_STRUCT(ScriptAction, doorControls, ParseDoorControl)            },
    { "PorterCreate",            TOK_STRUCT(ScriptAction, ports, ParsePort)                            },
    { "PorterControl",            TOK_STRUCT(ScriptAction, portControls, ParsePortControl)            },
    { "GurneyCreate",            TOK_STRUCT(ScriptAction, gurneys, ParseGurney)                        },
    { "GurneyControl",            TOK_STRUCT(ScriptAction, gurneyControls, ParseGurneyControl)        },
    { "SetMinPlayerLevel",        TOK_INT(ScriptAction, zeMinPlayerLevel, 0)                            },
    { "SetMaxPlayerLevel",        TOK_INT(ScriptAction, zeMaxPlayerLevel, 0)                            },
    { "WaypointCreate",            TOK_STRUCT(ScriptAction, waypoints, ParseWaypoint)                    },
    { "WaypointPlace",            TOK_STRUCT(ScriptAction, placedWaypoints, ParseWaypointPlace)        },
    { "PlaySound",                TOK_STRUCT(ScriptAction, sounds, ParseSound)                        },
    { "FadeSound",                TOK_STRUCT(ScriptAction, soundFades, ParseFadeSound)                },
    { "Floater",                TOK_STRUCT(ScriptAction, floaters, ParseFloater)                    },
    { "Caption",                TOK_STRUCT(ScriptAction, captions, ParseCaption)                    },
    { "RewardGroup",            TOK_STRUCT(ScriptAction, rewardGroup, ParseRewardGroup)                },
    { "Timer",                    TOK_STRUCT(ScriptAction, timers, ParseTimer)                        },
    { "EventTimer",                TOK_STRUCT(ScriptAction, eventTimers, ParseTimer)                    },
    { "TimerControl",            TOK_STRUCT(ScriptAction, timerControls, ParseTimerControl)            },
    { "EventTimerStop",            TOK_STRUCT(ScriptAction, eventTimer, ParseEventTimer)                },
    { "}",                        TOK_END, 0                                                            },
    { "", 0, 0 }
};

typedef struct ScriptStartup
{
    ScriptAction **actions;
} ScriptStartup;

static TokenizerParseInfo ParseStartup[] =
{
    { "{",                        TOK_START, 0                                                        },
    { "Action",                    TOK_STRUCT(ScriptStartup, actions, ParseAction)                        },
    { "}",                        TOK_END, 0                                                            },
    { "", 0, 0 }
};

typedef struct ScriptShutdown
{
    ScriptAction **actions;
} ScriptShutdown;

static TokenizerParseInfo ParseShutdown[] =
{
    { "{",                        TOK_START, 0                                                        },
    { "Action",                    TOK_STRUCT(ScriptShutdown, actions, ParseAction)                    },
    { "}",                        TOK_END, 0                                                            },
    { "", 0, 0 }
};

typedef struct ScriptTrigger
{
    char *triggerName;
    char **required;
    char *nextStage;
    ScriptAction **actions;
    int fired;
} ScriptTrigger;

static TokenizerParseInfo ParseTrigger[] =
{
    { "{",                        TOK_START, 0                                                        },
    { "",                        TOK_STRUCTPARAM | TOK_STRING(ScriptTrigger, triggerName, 0)            },
    { "Requires",                TOK_STRINGARRAY(ScriptTrigger, required)                            },
    { "NextStage",                TOK_STRING(ScriptTrigger, nextStage, 0)                                },
    { "Action",                    TOK_STRUCT(ScriptTrigger, actions, ParseAction)                        },
    { "}",                        TOK_END, 0                                                            },
    { "", 0, 0 }
};

AUTO_ENUM;
typedef enum triggerDeckType
{
    triggerDeck_NeverShuffle,
    triggerDeck_ShuffleAlways,
    triggerDeck_ShuffleAtCount,
    triggerDeck_ShuffleAtEnd,
} triggerDeckType;

typedef struct ScriptTriggerDeck
{
    char *triggerDeckName;
    triggerDeckType type;
    char **required;
    ScriptTrigger **triggers;
    int fired;
    int totalFired;
    int shuffleAt;
    U8 noShuffle;
} ScriptTriggerDeck;

static TokenizerParseInfo ParseTriggerDeck[] =
{
    { "{",                        TOK_START, 0                                                        },
    { "",                        TOK_STRUCTPARAM | TOK_STRING(ScriptTriggerDeck, triggerDeckName, 0)    },
    { "Requires",                TOK_STRINGARRAY(ScriptTriggerDeck, required)                        },
    { "Trigger",                TOK_STRUCT(ScriptTriggerDeck, triggers, ParseTrigger)                },
    { "Type",                    TOK_INT(ScriptTriggerDeck, type, 0), triggerDeckTypeEnum            },
    { "ShuffleAt",                TOK_INT(ScriptTriggerDeck, shuffleAt, 0)                            },
    { "NoShuffle",                TOK_BOOLFLAG(ScriptTriggerDeck, noShuffle, 0)                        },
    { "}",                        TOK_END, 0                                                            },
    { "", 0, 0 }
};

AUTO_ENUM;
typedef enum counterType
{
    counter_VillainGroup,
    counter_Team,
    counter_Name,
    counter_Click,
    counter_ZEPlayers,
    counter_ZEPlayersAlive,
    counter_ZEPlayersPhased,
    counter_ZEPlayersActive,
    counter_BossHealth,
    counter_PlayerDeaths,
} counterType;

AUTO_ENUM;
typedef enum counterDirection
{
    counter_Up = 0,
    counter_Down = 1,
} counterDirection;

typedef struct ScriptCounter
{
    char *counterName;
    counterType type;
    counterDirection direction;
    char *villainGroup;
    char *team;
    char *name;
    char *volumeName;
    char *glowie;
    char *displayProgress;
    int target;
    int progress;
    char **evalTarget;
} ScriptCounter;

static TokenizerParseInfo ParseCounter[] =
{
    { "{",                        TOK_START, 0                                                        },
    { "",                        TOK_STRUCTPARAM | TOK_STRING(ScriptCounter, counterName, 0)            },
    { "Type",                    TOK_INT(ScriptCounter, type, 0), counterTypeEnum                    },
    { "Direction",                TOK_INT(ScriptCounter, direction, 0), counterDirectionEnum            },
    { "VillainGroup",            TOK_STRING(ScriptCounter, villainGroup, 0)                            },
    { "Team",                    TOK_STRING(ScriptCounter, team, 0)                                    },
    { "Name",                    TOK_STRING(ScriptCounter, name, 0)                                    },
    { "VolumeName",                TOK_STRING(ScriptCounter, volumeName, 0)                            },
    { "Glowie",                    TOK_STRING(ScriptCounter, glowie, 0)                                },
    { "Count",                    TOK_STRINGARRAY(ScriptCounter, evalTarget)                            },
    { "DisplayProgress",        TOK_STRING(ScriptCounter, displayProgress, 0)                        },
    { "}",                        TOK_END, 0                                                            },
    { "", 0, 0 }
};

AUTO_ENUM;
typedef enum objectiveType
{
    objective_VillainGroup,
    objective_Team,
    objective_Named,
    objective_Glowie,
    objective_Defendable,
} objectiveType;

typedef struct ScriptObjective
{
    char *objectiveName;
    objectiveType type;
    char *villainGroup;
    char *team;
    char *name;
    char *glowie;
    char *defendable;
    char *volumeName;
    int teamKarmaValue;
    int teamKarmaLife;
    int karmaValue;
    int karmaLife;
    int deprecated;
} ScriptObjective;

static TokenizerParseInfo ParseObjective[] =
{
    { "{",                        TOK_START, 0                                                        },
    { "",                        TOK_STRUCTPARAM | TOK_STRING(ScriptObjective, objectiveName, 0)        },
    { "Type",                    TOK_INT(ScriptObjective, type, 0), objectiveTypeEnum                },
    { "VillainGroup",            TOK_STRING(ScriptObjective, villainGroup, 0)                        },
    { "Team",                    TOK_STRING(ScriptObjective, team, 0)                                },
    { "Name",                    TOK_STRING(ScriptObjective, name, 0)                                },
    { "Glowie",                    TOK_STRING(ScriptObjective, glowie, 0)                                },
    { "Defendable",                TOK_STRING(ScriptObjective, defendable, 0)                            },
    { "VolumeName",                TOK_STRING(ScriptObjective, volumeName, 0)                            },
    { "Value",                    TOK_INT(ScriptObjective, deprecated, 0)                                },
    { "KarmaLife",                TOK_INT(ScriptObjective, karmaLife, 0)                                },
    { "KarmaValue",                TOK_INT(ScriptObjective, karmaValue, 0)                                },
    { "TeamKarmaLife",            TOK_INT(ScriptObjective, teamKarmaLife, 0)                            },
    { "TeamKarmaValue",            TOK_INT(ScriptObjective, teamKarmaValue, 0)                            },

    { "}",                        TOK_END, 0                                                            },
    { "", 0, 0 }
};

AUTO_ENUM;
typedef enum itemType
{
    itemType_BarBlue,
    itemType_BarGold,
    itemType_BarRed,
    itemType_Text,
    itemType_CenterText,
    itemType_Timer
} itemType;

typedef struct ScriptZEUIItem
{
    char *itemName;
    int type;
    char *text;
    char *shortText;
    char *tooltip;
    char *timer;
    char **current;
    char **target;
    char **hideReq;
    int widgetId;
    int visibleLastTick;
    U8 updateTarget;
    char currentVar[16];
    char targetVar[16];
    char *findEntity;
    char findVar[16];
} ScriptZEUIItem;

static TokenizerParseInfo ParseUIItem[] =
{
    { "{",                        TOK_START, 0                                                        },
    { "",                        TOK_STRUCTPARAM | TOK_STRING(ScriptZEUIItem, itemName, 0)            },
    { "Type",                    TOK_INT(ScriptZEUIItem, type, 0), itemTypeEnum                        },
    { "Text",                    TOK_STRING(ScriptZEUIItem, text, 0)                                    },
    { "ShortText",                TOK_STRING(ScriptZEUIItem, shortText, 0)                            },
    { "Tooltip",                TOK_STRING(ScriptZEUIItem, tooltip, 0)                                },
    { "Current",                TOK_STRINGARRAY(ScriptZEUIItem, current)                            },
    { "Target",                    TOK_STRINGARRAY(ScriptZEUIItem, target)                                },
    { "Timer",                    TOK_STRING(ScriptZEUIItem, timer, 0)                                },
    { "HideIf",                    TOK_STRINGARRAY(ScriptZEUIItem, hideReq)                            },
    { "OnClickFindEnt",            TOK_STRING(ScriptZEUIItem, findEntity, 0)                            },
    { "UpdateTarget",            TOK_BOOLFLAG(ScriptZEUIItem, updateTarget, 0)                        },
    { "}",                        TOK_END, 0                                                            },
    { "", 0, 0 }
};

typedef struct ScriptZEUICollection
{
    char *collName;
    char **required;
    ScriptZEUIItem **items;
} ScriptZEUICollection;

static TokenizerParseInfo ParseUICollection[] =
{
    { "{",                        TOK_START, 0                                                        },
    { "",                        TOK_STRUCTPARAM | TOK_STRING(ScriptZEUICollection, collName, 0)        },
    { "Requires",                TOK_STRINGARRAY(ScriptZEUICollection, required)                        },
    { "UIItem",                    TOK_STRUCT(ScriptZEUICollection, items, ParseUIItem)                },
    { "}",                        TOK_END, 0                                                            },
    { "", 0, 0 }
};

typedef struct ScriptStage
{
    char *stageName;
    char *displayName;
    char *toolTip;
    ScriptStartup **startup;
    ScriptShutdown **shutdown;
    ScriptTrigger **triggers;
    ScriptTriggerDeck **triggerDecks;
    ScriptTimer **timers;
    ScriptTimer **eventTimers;
    ScriptCounter **counters;
    ScriptObjective **objectives;
    int maxPoints;
    ScriptZEUICollection **UICollections;
    ScriptTimer *displayTimer;
    int karmaThreshold;
    char** activeReqs;
    U8 awardKarma;
} ScriptStage;

static TokenizerParseInfo ParseStage[] =
{
    { "{",                        TOK_START, 0                                                        },
    { "",                        TOK_STRUCTPARAM | TOK_STRING(ScriptStage, stageName, 0)                },
    { "DisplayName",            TOK_STRING(ScriptStage, displayName, 0)                                },
    { "ToolTip",                TOK_STRING(ScriptStage, toolTip, 0)                                    },
    { "StartUp",                TOK_STRUCT(ScriptStage, startup, ParseStartup)                        },
    { "ShutDown",                TOK_STRUCT(ScriptStage, shutdown, ParseShutdown)                    },
    { "Trigger",                TOK_STRUCT(ScriptStage, triggers, ParseTrigger)                        },
    { "TriggerDeck",            TOK_STRUCT(ScriptStage, triggerDecks, ParseTriggerDeck)                },
    { "Timer",                    TOK_STRUCT(ScriptStage, timers, ParseTimer)                            },
    { "EventTimer",                TOK_STRUCT(ScriptStage, eventTimers, ParseTimer)                    },
    { "Counter",                TOK_STRUCT(ScriptStage, counters, ParseCounter)                        },
    { "Objective",                TOK_STRUCT(ScriptStage, objectives, ParseObjective)                    },
    { "MaxPoints",                TOK_INT(ScriptStage, maxPoints, 0)                                    },
    { "UICollection",            TOK_STRUCT(ScriptStage, UICollections, ParseUICollection)            },
    { "AwardKarma",                TOK_BOOLFLAG(ScriptStage, awardKarma, 0)                            },
    { "KarmaThreshold",            TOK_INT(ScriptStage, karmaThreshold, 0)                                },
    { "StageActiveRequires",    TOK_STRINGARRAY(ScriptStage, activeReqs)                            },
    { "}",                        TOK_END, 0                                                            },
    { "", 0, 0 }
};

typedef struct ScriptDefinition
{
    char *scriptName;
    char *displayName;
    char *firstStage;
    char *stopStage;
    char *restartStage;
    char *eventVolume;
    char *instance;
    ScriptStage **stages;
    ScriptStage *currentStage;
    char *nextStage;
    ScriptGlowie *glowies;        // These will be buried so deep it's worth taking the time to form a linked list for rapid search.
    ScriptSpawn *spawns;        // In hindsight, I ought to think about using either earrays or stashtables for these
    ScriptTeamRespawn *teams;
    ScriptDoor *doors;
    ScriptPort *ports;
    ScriptGurney *gurneys;
    ScriptWaypoint *waypoints;
    ScriptWaypointPlace *placedWaypoints;
    DelayedScriptAction **delayedActions;
    ScriptTimer **timers;
    StashTable teamGroup;        // stashtable, indexed by teamGroup string, payload is an earray of team names in that grouping
    char *currentBGMusicName;
    float currentBGMusicVolume;
} ScriptDefinition;

static TokenizerParseInfo ParseScript[] =
{
    { "Name",                    TOK_STRING(ScriptDefinition, scriptName, 0)                            },
    { "DisplayName",            TOK_STRING(ScriptDefinition, displayName, 0)                        },
    { "FirstStage",                TOK_STRING(ScriptDefinition, firstStage, 0)                            },
    { "StopStage",                TOK_STRING(ScriptDefinition, stopStage, 0)                            },
    { "RestartStage",            TOK_STRING(ScriptDefinition, restartStage, 0)                        },
    { "EventVolume",            TOK_STRING(ScriptDefinition, eventVolume, 0)                        },
    { "Stage",                    TOK_STRUCT(ScriptDefinition, stages, ParseStage)                    },
    { "", 0, 0 }
};

static ScriptDefinition *currentScript = NULL;
static EvalContext *evalContext = NULL;
static int killDebugFlag = 0;
static char **disabledScripts = NULL;

// Used for spawn creation
static char tempCreationTeam[] = ".Temp.Creation.Team.";
static char forceCreationTeam[] = ".Temp.ForceCreation.Team";

///////////////////////////////////////////////////////////////////////////////////////
// I do this all over the place ...
// Yeah, it is the inverse of StringEmpty, but this makes better syntactic sense
//
#define IsValidStr(str) (str != NULL && str[0])

///////////////////////////////////////////////////////////////////////////////////////
// And a couyple of forward declarations
//
void InitCounter(ScriptCounter *counter);
void InitTimer(ScriptStage *stage, ScriptTimer *timer);
void CheckGurney(TEAM team);
void UpdateGurney(TEAM team, STRING gurney);
void SetPlacedWaypoints(ENTITY player);

///////////////////////////////////////////////////////////////////////////////////////
// Dump a formatted message to everyone's console
//
void ScriptedZoneEventSendDebugPrintf(int logFlags, char *format, ...)
{
    va_list ap;
    char buf[16384];

    va_start(ap, format);
    vsnprintf(buf, 16384, format, ap);
    buf[16383] = 0;
    va_end(ap);

    if (isDevelopmentMode())
    {
        chatSendDebug(buf);
    }
    if (logFlags & SZE_Log_Reward)
    {
        LOG(LOG_SZE_REWARDS, LOG_LEVEL_IMPORTANT, 0, "%s", buf);
    }

}

///////////////////////////////////////////////////////////////////////////
// Set and get current scriptdefinition address.  We explicitly printf and sscanf
// ourselves, rather than using Var[GS]etNumber, since our way is 64 bit safe
//
static ScriptDefinition *GetCurrentScript()
{
    const char *address = VarGet("MyAddress");
    void* ptr = NULL;

    if (StringEmpty(address) || sscanf(address, "%p", &ptr) != 1)
    {
        assert(0 && "Bad script address");
    }
    return (ScriptDefinition *)ptr;
}

static ScriptDefinition *GetCurrentScriptDontAssert()
{
    const char *address = VarGet("MyAddress");
    void *ptr;

    if (StringEmpty(address) || sscanf(address, "%p", &ptr) != 1)
    {
        return 0;
    }
    return (ScriptDefinition *) ptr;
}

// Replace the three illegal team name characters with safe ones.
// This introduces a tiny risk of collision.  If that ever becomes an issue, do something else like b64 encoding the name
static const char *TeamNameSanitize(const char *name)
{
    int i;
    static char *cleanName;

    cleanName = StringCopySafe(name);
    for (i = 0; cleanName[i]; i++)
    {
        if (cleanName[i] == ':')
        {
            cleanName[i] = '~';
        }
        else if (cleanName[i] == '_')
        {
            cleanName[i] = '`';
        }
        else if (cleanName[i] == ',')
        {
            cleanName[i] = '|';
        }
    }
    return cleanName;
}

static STRING GetScriptVolumeTeam(const char *volumeName)
{
    assert(volumeName && IsValidStr(volumeName));
    if (volumeName && IsValidStr(volumeName))
    {
        return StringAdd("VolumePlayers.", TeamNameSanitize(volumeName));
    }
    else
    {
        return NULL;
    }
}

static STRING GetScriptTeam()
{
    ScriptDefinition *script = GetCurrentScript();
    return (script && IsValidStr(script->eventVolume)) ? GetScriptVolumeTeam(script->eventVolume) : ALL_PLAYERS;
}

static void SZE_TrialComplete(TrialStatus status)
{
    if (OnMissionMap())
    {
        SetTrialStatus(status);
        SendTrialStatus(GetScriptTeam());
    }
    else
    {
        // Maybe complain that an "CompleteIncarnateTrial" is meaningless when in a static map.
        // Either that or just shut ourselves down.
    }
}

static void SetCurrentScript(ScriptDefinition *script)
{
    char address[64];        // Plenty enough space for a 64 bit pointer

    address[63] = 0;
    snprintf(address, 64, "%p", script);
    if (address[63])
    {
        assert(0 && "Script address conversion failure");
    }
    VarSet("MyAddress", address);
}

static void ScriptedZoneEventReset()
{
}

static int ScriptedZoneEventGetActivePlayersForSpawning()
{
    STRING activeTeam = GetScriptTeam();
    int allPlayers = NumEntitiesInTeam(activeTeam);
    int count = ScriptKarmaGetActivePlayers(g_currentScript->id);

    if (!count)    count = allPlayers;                //    we just started, and noone's made a move, just count everyone as active
    return MAX(MAX(count, allPlayers*0.5f), 1);    //    min of at least 1/2 the people in the zone, zero is special, make sure to be at least 1
}

///////////////////////////////////////////////////////////////////////////////////////
// Clean up - end the script and free memory
//
static void ScriptedZoneEventShutdown()
{
    ScriptDefinition *script;
    ScriptGlowie *glowie;
    ScriptSpawn *spawn;
    ScriptStage *stage;
    ScriptCounter *counter;
    ScriptTimer *timer;
    int i;
    int j;

    ScriptDisableKarma();

    // Get the current script struct first
    script = GetCurrentScript();
    EndScript();

    if (script)
    {
        // Clean up memory allocated on behalf of the glowies
        for (glowie = script->glowies; glowie; glowie = glowie->next)
        {
            if (glowie->glowies)
            {
                for (i = 0; i < glowie->quantity; i++)
                {
                    if (glowie->glowies[i])
                    {
                        freeConst(glowie->glowies[i]);
                    }
                }
                freeConst(glowie->glowies);
                glowie->glowies = NULL;
            }
            if (glowie->glowieTimers)
            {
                free(glowie->glowieTimers);
                glowie->glowieTimers = NULL;
            }
            if (glowie->glowieSpots)
            {
                free(glowie->glowieSpots);
                glowie->glowieSpots = NULL;
            }
            if (glowie->glowieUsed)
            {
                free(glowie->glowieUsed);
                glowie->glowieUsed = NULL;
            }
        }
        // Clean up memory allocated on behalf of the spawns
        for (spawn = script->spawns; spawn; spawn = spawn->next)
        {
            eaDestroyExConst(&spawn->groups, NULL);
        }
        // Clean up memory allocated on behalf of the counter UI and stage UI
        for (i = eaSize(&script->stages) - 1; i >= 0; i--)
        {
            stage = script->stages[i];
            // This logic is deprecated.  Some day I will remove it.
            for (j = eaSize(&stage->counters) - 1; j >= 0; j--)
            {
                counter = stage->counters[j];
            }
            for (j = eaSize(&stage->timers) - 1; j >= 0; j--)
            {
                timer = stage->timers[j];
                if (timer && timer->uiTimerVariable)
                {
                    free(timer->uiTimerVariable);
                }
            }
        }

        if (script->currentBGMusicName)
        {
            free(script->currentBGMusicName);
        }

        if (script->delayedActions)
        {
            eaDestroy(&script->delayedActions);
        }

        // Didn't use StructAlloc to get this, so we just Clear it
        StructClear(ParseScript, script);
        // And free, because we actually used malloc
        free(script);
    }
}

///////////////////////////////////////////////////////////////////////////////////////
// Evaluate an optional required, return true if it's not provided
//
static int ScriptedZoneEvent_Eval(char **eval)
{
    // If the required is absent, it defaults to succeeding
    if (eval == NULL)
    {
        return 1;
    }
    // Otherwise evaluate it
    eval_ClearStack(evalContext);
    eval_Evaluate(evalContext, eval);
    return eval_IntPeek(evalContext);
}

///////////////////////////////////////////////////////////////////////////////////////
// Convert a duration as a string to an integer count of seconds
//
static NUMBER stringToDuration(char *duration)
{
    NUMBER seconds = 0;
    NUMBER minutes = 0;
    NUMBER hours = 0;

    while (*duration)
    {
        if (isdigit(*duration))
        {
            seconds = seconds * 10 + *duration - '0';
        }
        else if (*duration == ':')
        {
            hours = minutes * 60;
            minutes = seconds * 60;
            seconds = 0;
        }
        else
        {
            // Complain long and loud
            return 0;
        }
        duration++;
    }
    return hours + minutes + seconds;
}

///////////////////////////////////////////////////////////////////////////////////////
// Create a unique string to salt UI item names.  I'm not entirely convinced I need
// this, but it's here for now.  The linker will ditch it if nobody uses it.
//
static STRING uiUnique()
{
    static int unique = 0;

    return NumberToString(unique++);
}

///////////////////////////////////////////////////////////////////////////////////////
// Format the time remaining in a timer to a string.  This makes some assumptions as to
// how exactly to format, based on the original duration.
//
static STRING timerToFormattedTimeRemaining(ScriptTimer *timer)
{
    U32 hours;
    U32 minutes;
    U32 seconds;
    char buffer[16];                // if we ever go over nnn:nn:nn we need to rethink the design of this stage

    // Time remaining in seconds
    seconds = timer->endTime + timer->bonusTime - timerSecondsSince2000();
    // Don't go negative ...
    if (seconds < 0)
    {
        seconds = 0;
    }

    // one hour or greater, format as h:mm:ss
    if (timer->origDuration >= 60 * 60)
    {
        minutes = seconds / 60;
        seconds %= 60;
        hours = minutes / 60;
        minutes %= 60;
        _snprintf(buffer, sizeof(buffer), "%d:%02d:%02d", hours, minutes, seconds);
    }
    // one to fifty nine minutes, format as m:ss
    else if (timer->origDuration >= 60)
    {
        minutes = seconds / 60;
        seconds %= 60;
        _snprintf(buffer, sizeof(buffer), "%d:%02d", minutes, seconds);
    }
    // under a minute format as just seconds
    else
    {
        _snprintf(buffer, sizeof(buffer), "%d", seconds);
    }
    // Zero terminate, since _snprintf does not in the case of a buffer overflow
    buffer[sizeof(buffer) - 1] = 0;
    return StringCopy(buffer);
}

///////////////////////////////////////////////////////////////////////////////////////
// Find a stage in the given script with the given name.
//
static ScriptStage *FindStageByName(ScriptDefinition *script, char *name)
{
    int i;

    for (i = eaSize(&script->stages) - 1; i >= 0; i--)
    {
        ScriptStage *currentStage = script->stages[i];
        if (stricmp(currentStage->stageName, name) == 0)
        {
            return currentStage;
        }
    }
    // Should we complain here?  Reaching here probably means there's an error in the script
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////
// Find a counter in the given stage with the given name.
//
static ScriptCounter *FindCounterByName(ScriptStage *stage, const char *name)
{
    int i;

    for (i = eaSize(&stage->counters) - 1; i >= 0; i--)
    {
        ScriptCounter *currentCounter = stage->counters[i];
        if (stricmp(currentCounter->counterName, name) == 0)
        {
            return currentCounter;
        }
    }
    // Reaching here may or may not be an error.  Since the evaluator functions are
    // overloaded and can take either a counter or timer name, it's quite possible we
    // could reach this point if we get handed the name of a timer.  Same issue with
    // the bottom of FindTimerByName(...)
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////
// Find a timer in the given stage with the given name.
// BOTH def AND stage ARE REQUIRED FOR ANY PLACES THAT ACCESS TIMERS, STAGE AND EVENT TIMERS SHOULD BE INTERCHANGEABLE
// Only pass one or the other when you are setting timers.
//
static ScriptTimer *FindTimerByName(ScriptDefinition *def, ScriptStage *stage, const char *name)
{
    int i;

    assert(name && "Timer needs a name");
    if (stage)
    {
        for (i = eaSize(&stage->timers) - 1; i >= 0; i--)
        {
            ScriptTimer *currentTimer = stage->timers[i];
            if (stricmp(currentTimer->timerName, name) == 0)
            {
                return currentTimer;
            }
        }
    }
    if (def)
    {
        for (i = eaSize(&def->timers) - 1; i >= 0; i--)
        {
            ScriptTimer *currentTimer = def->timers[i];
            if (stricmp(currentTimer->timerName, name) == 0)
            {
                return currentTimer;
            }
        }
    }

    return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////
// Find a glowie in the given script with the given name.
//
static ScriptGlowie *FindGlowieByName(ScriptDefinition *script, char *name)
{
    ScriptGlowie *glowie;

    if (!IsValidStr(name))
    {
        return NULL;
    }
    for (glowie = script->glowies; glowie; glowie = glowie->next)
    {
        if (stricmp(glowie->glowieName, name) == 0)
        {
            return glowie;
        }
    }
    // No, it's not necessarily an error to reach here.  We call this when setting up a glowie the
    // first time to see if it's already neen initialized and added to the list.  That usage expects
    // to reach this point.
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////
// Find a spawn in the given script with the given name.
//
static ScriptSpawn *FindSpawnByName(ScriptDefinition *script, char *name)
{
    ScriptSpawn *spawn;

    if (!IsValidStr(name))
    {
        return NULL;
    }
    for (spawn = script->spawns; spawn; spawn = spawn->next)
    {
        if (stricmp(spawn->spawnName, name) == 0)
        {
            return spawn;
        }
    }
    // No, it's not necessarily an error to reach here.  We call this when setting up a spawn the
    // first time to see if it's already been initialized and added to the list.  That usage expects
    // to reach this point.
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////
// Find a team in the given script with the given name.
//
static ScriptTeamRespawn *FindTeamByName(ScriptDefinition *script, const char *name)
{
    ScriptTeamRespawn *team;

    if (!IsValidStr(name))
    {
        return NULL;
    }
    for (team = script->teams; team; team = team->next)
    {
        if (stricmp(team->teamName, name) == 0)
        {
            return team;
        }
    }
    // No, it's not necessarily an error to reach here.  We call this when setting up a team the
    // first time to see if it's already neen initialized and added to the list.  That usage expects
    // to reach this point.
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////
// Find a door in the given script with the given name.
//
static ScriptDoor *FindDoorByName(ScriptDefinition *script, char *name)
{
    ScriptDoor *door;

    if (!IsValidStr(name))
    {
        return NULL;
    }
    for (door = script->doors; door; door = door->next)
    {
        if (stricmp(door->doorName, name) == 0)
        {
            return door;
        }
    }
    // No, it's not necessarily an error to reach here.  We call this when setting up a door the
    // first time to see if it's already neen initialized and added to the list.  That usage expects
    // to reach this point.
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////
// Find a port in the given script with the given name.
//
static ScriptPort *FindPortByName(ScriptDefinition *script, char *name)
{
    ScriptPort *port;

    if (!IsValidStr(name))
    {
        return NULL;
    }
    for (port = script->ports; port; port = port->next)
    {
        if (stricmp(port->portName, name) == 0)
        {
            return port;
        }
    }
    // No, it's not necessarily an error to reach here.  We call this when setting up a port the
    // first time to see if it's already neen initialized and added to the list.  That usage expects
    // to reach this point.
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////
// Find a gurney in the given script with the given name.
//
static ScriptGurney *FindGurneyByName(ScriptDefinition *script, char *name)
{
    ScriptGurney *gurney;

    if (!IsValidStr(name))
    {
        return NULL;
    }
    for (gurney = script->gurneys; gurney; gurney = gurney->next)
    {
        if (stricmp(gurney->gurneyName, name) == 0)
        {
            return gurney;
        }
    }
    // No, it's not necessarily an error to reach here.  We call this when setting up a port the
    // first time to see if it's already neen initialized and added to the list.  That usage expects
    // to reach this point.
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////
// Find a waypoint in the given script with the given name.
//
static ScriptWaypoint *FindWaypointByName(ScriptDefinition *script, char *name)
{
    ScriptWaypoint *waypoint;

    if (!IsValidStr(name))
    {
        return NULL;
    }
    for (waypoint = script->waypoints; waypoint; waypoint = waypoint->next)
    {
        if (stricmp(waypoint->waypointName, name) == 0)
        {
            return waypoint;
        }
    }
    // No, it's not necessarily an error to reach here.  We call this when setting up a waypoint the
    // first time to see if it's already neen initialized and added to the list.  That usage expects
    // to reach this point.
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////
// Takes either a timer or a counter name and returns current progress (i.e. count up).
//
static void scripteval_Count(EvalContext *pcontext)
{
    int result = 0;
    const char *itemName = eval_StringPop(pcontext);
    ScriptCounter *counter;
    ScriptTimer *timer;
    
    if (currentScript && itemName)
    {
        if (counter = FindCounterByName(currentScript->currentStage, itemName))
        {
            // found a counter - return it's current count
            result = counter->progress;
        }
        else if (timer = FindTimerByName(currentScript, currentScript->currentStage, itemName))
        {
            // found a timer - return it's elapsed duration
            result = timer->origDuration - (timer->endTime - timerSecondsSince2000());
        }
    }
    eval_IntPush(pcontext, result);
}

///////////////////////////////////////////////////////////////////////////////////////
// Takes a counter name and returns the target.  Only meaningful on a counter type target
//
static void scripteval_Target(EvalContext *pcontext)
{
    int result = 0;
    const char *itemName = eval_StringPop(pcontext);
    ScriptCounter *counter;
    
    if (currentScript && itemName)
    {
        if (counter = FindCounterByName(currentScript->currentStage, itemName))
        {
            // found a counter - return it's current target
            result = counter->target;
        }
    }
    eval_IntPush(pcontext, result);
}

///////////////////////////////////////////////////////////////////////////////////////
// Takes either a timer or a counter name and returns remaining count/time.
//
static void scripteval_Remaining(EvalContext *pcontext)
{
    int result = 0;
    const char *itemName = eval_StringPop(pcontext);
    ScriptCounter *counter;
    ScriptTimer *timer;

    if (currentScript && itemName)
    {
        if (counter = FindCounterByName(currentScript->currentStage, itemName))
        {
            if (counter->type == counter_Team && counter->direction == counter_Down)
            {
                // team counter counting down, return kill count left
                result = counter->progress - counter->target;
            }
            else if (counter->type == counter_BossHealth)
            {
                result = BOSS_MAX_HEALTH - counter->progress;
            }
            else
            {
                // found a counter - return it's remaining count
                result = counter->target - counter->progress;
            }
        }
        else if (timer = FindTimerByName(currentScript, currentScript->currentStage, itemName))
        {
            // found a timer - return it's remaining duration
            result = timer->endTime + timer->bonusTime - timerSecondsSince2000();
        }
    }
    eval_IntPush(pcontext, result);
}

extern void pmotionScriptVolumeCheck( Entity * e );
static void InitVolumeTeams()
{
    int i;
    ENTITY player;
    Entity *e;

    for (i = NumEntitiesInTeam(ALL_PLAYERS_READYORNOT); i > 0; i--)
    {
        player = GetEntityFromTeam(ALL_PLAYERS_READYORNOT, i);
        e = EntTeamInternal(player, 0, NULL);
        if(e)
            pmotionScriptVolumeCheck(e);
    }
}

static void ScriptedZoneEventResetKarma()
{
    int i;
    int numStages = VarGetNumber("TotalStages");
    for (i = 0; i < numStages; ++i)
    {
        char buff[30];
        sprintf(buff, "%s%i", STAGETHRESH, i);
        VarSetNumber(buff, 0);
    }
    VarSetNumber("TotalStages", 0);
    ScriptResetKarma();
}

///////////////////////////////////////////////////////////////////////////////////////
// Takes either a timer or a counter name and returns 1 or 0 based on whether it's
// complete or not.
//
static void scripteval_Complete(EvalContext *pcontext)
{
    int result = 0;
    const char *itemName = eval_StringPop(pcontext);
    ScriptCounter *counter;
    ScriptTimer *timer;

    if (currentScript && itemName)
    {
        if (counter = FindCounterByName(currentScript->currentStage, itemName))
        {
            if (counter->type == counter_Team && counter->direction == counter_Down)
            {
                // Team counter counting down, we succeed when we're below the target
                result = counter->progress <= counter->target;
            }
            else if (counter->type == counter_BossHealth)
            {
                result = counter->progress == 0;
            }
            else
            {
                // found a counter - return whether it's count has reached the target
                result =  counter->progress >= counter->target;
            }
        }
        else if (timer = FindTimerByName(currentScript, currentScript->currentStage, itemName))
        {
            // found a timer - return whether current time is past it's end time
            result = timerSecondsSince2000() >= timer->endTime + timer->bonusTime;
        }
    }
    eval_IntPush(pcontext, result);
}

///////////////////////////////////////////////////////////////////////////////////////
// Determines if an objective is complete or not.
// We don't use the standard MissionObjective? operator, since that fails unless you're
// on a mission map, while this has to succeed in city zones.
//
static void scripteval_ObjectiveSucceeded(EvalContext *pcontext)
{
    int result = 0;
    const char *objectiveName = eval_StringPop(pcontext);

    if (currentScript && objectiveName)
    {
        if(OnMissionMap())
        {
            result = MissionObjectiveIsComplete(objectiveName, 1);
        }
        else
        {
            result = StringToNumber(GetInstanceVar(objectiveName)) == ZONE_OBJECTIVE_SUCCESS;
        }
    }
    eval_IntPush(pcontext, result);
}

///////////////////////////////////////////////////////////////////////////////////////
// Returns the result of a previously evaluated Eval statement
//
static void scripteval_EvalValue(EvalContext *pcontext)
{
    int result = 0;
    const char *valueName = eval_StringPop(pcontext);

    if (currentScript && valueName)
    {
        result = VarGetNumber(StringAdd("SymTab%", valueName));
    }
    eval_IntPush(pcontext, result);
}

static void scripteval_DevMode(EvalContext *pcontext)
{
    eval_IntPush(pcontext, isDevelopmentMode());
}

static void scripteval_EventLockedByGroup(EvalContext *pcontext)
{
    eval_IntPush(pcontext, turnstileMapserver_eventLockedByGroup());
}

static void scripteval_TurnstileMissionName(EvalContext *pcontext)
{
    int missionID = 0;

    turnstileMapserver_getMapInfo(NULL, &missionID);

    if(EAINRANGE(missionID, turnstileConfigDef.missions))
        eval_StringPush(pcontext, turnstileConfigDef.missions[missionID]->name);
    else
        eval_StringPush(pcontext, "none");
}

static void scripteval_HasMapToken(EvalContext *pcontext)
{
    int result = 0;
    const char *valueName = eval_StringPop(pcontext);

    if (currentScript && valueName)
    {
        eval_IntPush(pcontext, MapHasDataToken(valueName));
    }
    else
    {
        eval_IntPush(pcontext, result);
    }
}

////////////////////////////////////////////////////////////////////////////////////////
// All manner of stuff arrives here.  Currently, it's two broad classes: variable requests
// and script control commands.
//
static int ScriptedZoneEventOnMessageReceived(STRING param)
{
    char *instance;
    char *type;
    char *variable;
    const char *myInstance;
    ScriptDefinition *script;
    ScriptDoor *door;
    ScriptStage *stage;

    script = GetCurrentScript();
    if (script == NULL)
    {
        // Give up on this instance if we can't get the local script structure, but let other instances see it.
        // This should probably be considered an error
        return 0;
    }

    // param is formatted as follows: ScriptInstance.Type[.Varname]
    strdup_alloca(instance, param);
    type = strchr(instance, '.');
    if (type == NULL)
    {
        // Force this to give up - argument is not properly formatted
        return 1;
    }
    *type++ = 0;
    if (!*type)
    {
        // Force this to give up - argument is not properly formatted
        return 1;
    }
    // pull off a variable, but don't error check yet.  That allows a type to arrive here without any futher parameters
    variable = strchr(type, '.');
    if (variable != NULL)
    {
        *variable++ = 0;
    }
    myInstance = VarGet("ZoneEventName");
    if (IsValidStr(myInstance) && StringEqual(instance, myInstance))
    {
        // We got a hit - drill in a bit deeper.
        if (stricmp(type, "Door") == 0)
        {
            // Check for a valid variable here.
            if (variable && *variable && (door = FindDoorByName(script, variable)) != NULL)
            {
                g_scriptCharEvalResult = door->state;
            }
        }
        if (stricmp(type, "Eval") == 0)
        {
            // Check for a valid variable here.
            if (variable && *variable)
            {
                g_scriptCharEvalResult = VarGetNumber(StringAdd("SymTab%", variable));
            }
        }
        if (stricmp(type, "Stop") == 0)
        {
            // This will cause the script to commit suicide - there can't be a stage whose name is a single space
            // so this will fail and shut down this instance.
            script->nextStage = " ";
        }
        if (stricmp(type, "Goto") == 0)
        {
            // Check for a valid variable here.
            if (variable && *variable && (stage = FindStageByName(script, variable)) != NULL)
            {
                // Set the nextStage to point to the stage name iself.  The stage name is persistent, variable is not,
                // and stage->stageName also requires no cleanup.
                script->nextStage = stage->stageName;
            }
        }
        if (stricmp(type, "GetID") == 0)
        {
            g_scriptCharEvalResult = g_currentScript->id+1;
        }
        // If we need to add other types, they'd go here
        // We got a hit, tell the closure we're all done
        return 1;
    }
    // Params are good, but not our instance.  Try the next one
    return 0;
}

///////////////////////////////////////////////////////////////////////////////////////
// Called when a player enters a named volume.  We add the player to a team specific to this
// volume, which helps with a lot of other housekeeping.  Also, if this is the volume
// specific to the event, set the UI
//
static void ScriptedZoneEventOnEnterVolume(ENTITY player, STRING name)
{
    int i;
    int n;
    NUMBER uiState;
    ScriptDefinition *script;
    ScriptStage *stage;
    ScriptZEUICollection *UICollection = NULL;
    const char *collection;

    // Put me in the team of players in this volume.
    if (!IsEntityOnScriptTeam(player, GetScriptVolumeTeam(name)))
        SetScriptTeam(player, GetScriptVolumeTeam(name));
    script = GetCurrentScript();
    uiState = VarGetNumber("uiState");
    if (script && uiState == UISTATE_BYVOLUME && script->currentStage && IsValidStr(script->eventVolume) && StringEqual(script->eventVolume, name))
    {
        stage = script->currentStage;
        n = eaSize(&stage->UICollections);
        for (i = 0; i < n; i++)
        {
            UICollection = stage->UICollections[i];
            collection = StringAdd("Collection:", UICollection->collName);
            if (ScriptUICollectionExists(collection) && ScriptedZoneEvent_Eval(UICollection->required))
            {
                // If they just popped out and back in
                if (IsEntityOnScriptTeam(player, "VolumeExited"))
                {
                    // take them off the recent exit team
                    SwitchScriptTeam(player, "VolumeExited", NULL);
                    // and nuke this players exit timer
                    TimerRemove(EntVar(player, "UITimer"));
                }
                // Finally show the UI.
                ScriptUIShow(collection, player);
                // DGNOTE 7/21/2010
                // ScriotUIShow(...) has been changed to reattach the UI so that legacy scripts can't wind up with their
                // UI detached.  Because of this we need to explicity detach the UI after every call to ScriptUIShow(...);
                ScriptUISendDetachCommand(player, 1);
                VarSet(EntVar(player, "CurrentUI"), collection);
                ScriptDBLog("ZoneEvent:UI", player, "Showing UI %s on enter volume", UICollection->collName);
                ScriptSetKarmaContainer(player);
            }
        }

        if (script->currentBGMusicName)
        {
            SendPlayerSound(player, script->currentBGMusicName, SOUND_MUSIC, script->currentBGMusicVolume);
        }

        SetPlacedWaypoints(player);
    }
}

///////////////////////////////////////////////////////////////////////////////////////
// Called when a player leaves a named volume.  remove them from the volume team, and
// set the timer to nuke the UI.  We don't nuke the UI immediately, we delay five seconds.
// That covers for the case of a player running along the edge of the volume, and prevents
// the UI flickering.  I'll probably need to add an immediate up date flag to show you're not
// in the volume, but that will have to wait til we have the real UI.
//
static void ScriptedZoneEventOnExitVolume(ENTITY player, STRING name)
{
    const char *collection;
    NUMBER uiState;
    ScriptDefinition *script;

    // Remove player from volume team
    SwitchScriptTeam(player, GetScriptVolumeTeam(name), NULL);
    script = GetCurrentScript();
    uiState = VarGetNumber("uiState");
    // If we're doing the UI by volume
    if (script && uiState == UISTATE_BYVOLUME && IsValidStr(script->eventVolume) && StringEqual(script->eventVolume, name))
    {
        collection = VarGet(EntVar(player, "CurrentUI"));
        if (IsValidStr(collection) && ScriptUICollectionExists(collection))
        {
            // If the player just left the UI volume, set a timer and put them on the just exited team
            SetScriptTeam(player, "VolumeExited");
            TimerSet(EntVar(player, "UITimer"), 5.0f / 60.0f);
        }

        if (script->currentBGMusicName)
        {
            ResetPlayerMusic(player);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////
// Called when a player enters the map.  Put them on the PlayersOn Map team for now.  As
// time progresses, we may want to separate players into two teams: resistance / loyalists.
//
static int ScriptedZoneEventOnEnterMap(ENTITY player)
{
    int i;
    int n;
    NUMBER uiState;
    ScriptDefinition *script;
    ScriptStage *stage;
    ScriptZEUICollection *UICollection;
    const char *collection;

    script = GetCurrentScript();
    if (script  == NULL)
    {
        return 0;
    }

    stage = script->currentStage;
    if (stage == NULL)
    {
        return 0;
    }
    uiState = VarGetNumber("uiState");
    if (uiState == UISTATE_GLOBAL)
    {
        n = eaSize(&stage->UICollections);
        for (i = 0; i < n; i++)
        {
            UICollection = stage->UICollections[i];
            collection = StringAdd("Collection:", UICollection->collName);
            if (ScriptUICollectionExists(collection) && ScriptedZoneEvent_Eval(UICollection->required))
            {
                // If they just popped out and back in
                if (IsEntityOnScriptTeam(player, "VolumeExited"))
                {
                    // take them off the recent exit team
                    SwitchScriptTeam(player, "VolumeExited", NULL);
                    // and nuke this players exit timer
                    TimerRemove(EntVar(player, "UITimer"));
                }
                // Finally show the UI.
                ScriptUIShow(collection, player);
                // DGNOTE 7/21/2010
                // And detach it.
                ScriptUISendDetachCommand(player, 1);
                VarSet(EntVar(player, "CurrentUI"), collection);
                ScriptDBLog("ZoneEvent:UI", player, "Showing UI %s on enter map", UICollection->collName);
                ScriptSetKarmaContainer(player);
            }
        }

        if (script->currentBGMusicName)
        {
            SendPlayerSound(player, script->currentBGMusicName, SOUND_MUSIC, script->currentBGMusicVolume);
        }

        SetPlacedWaypoints(player);
    }
    // if this script has a volume associated, and the player is in that volume, trip the volume enter call.
    if (uiState == UISTATE_BYVOLUME && IsValidStr(script->eventVolume) && EntityInArea(player, StringAdd("trigger:", script->eventVolume)))
    {
        ScriptedZoneEventOnEnterVolume(player, script->eventVolume);
    }

    if(IsEntityPlayer(player) && GetHealth(player) <= 0.0)
        CheckGurney(player);

    return 0;
}

///////////////////////////////////////////////////////////////////////////////////////
// Called when a player leaves the map.  Remove them from any teams they might be on,
// reset the UI, and do other assorted cleanup.
//
static int ScriptedZoneEventOnExitMap(ENTITY player)
{
    const char *collection;

    // If I have a UI volume exit timer running
    if (IsEntityOnScriptTeam(player, "VolumeExited"))
    {
        // nuke it
        TimerRemove(EntVar(player, "UITimer"));
    }
    // Remove me from all teams
    LeaveAllScriptTeams(player);

    if(IsEntityPlayer(player))
        UpdateGurney(player, NULL);

    collection = VarGet(EntVar(player, "CurrentUI"));
    if (IsValidStr(collection))
    {
        // hide the UI.  Hope this is harmless if nothing on display
        ScriptUIHide(collection, player);
        ScriptDBLog("ZoneEvent:UI", player, "Hiding UI %s on leave map", collection);
        ScriptClearKarmaContainer(player);
    }
    VarSet(EntVar(player, "CurrentUI"), NULL);
     SendPlayerFadeSound(player, SOUND_MUSIC, 0);
    //ScriptUISendDetachCommand(player, 0);
    return 0;
}

///////////////////////////////////////////////////////////////////////////////////////
// Called when anything defeats anything else.  We look for players defeating critters,
// to see if this advances the count.
// May Need to relax the restriction on the victor, since there may be instances when we
// care about NPC's defeating other NPC's
//
static void ScriptedZoneEventOnEntityDefeat(ENTITY player, ENTITY victor)
{
    int j;
    ScriptDefinition *script = GetCurrentScript();
    ScriptStage *stage = script->currentStage;
    ScriptCounter *counter;
    NUMBER victimVillainGroupID;
    NUMBER desiredVillainGroupID;
    
    if (killDebugFlag)
    {
        chatSendDebug("Entity defeat detected");
    }

    // Destroy this guy's PetTeam EntVar, so we don't clog up the symbol table
    VarSetEmpty(EntVar(player, "PetTeam"));

    // Player defeating a critter - this will probably want to change.  There are cases where we want to 
    // track critters defeating other critters.
    if (IsEntityCritter(player))
    {
        // If I'm running (i.e. have an active stage)
        if (stage != NULL)
        {
            // scan all counters in this stage
            for (j = eaSize(&stage->counters) - 1; j >= 0; j--)
            {
                counter = stage->counters[j];
                // for villain group kill counters with a meaningful villain group
                if (counter->type == counter_VillainGroup && IsValidStr(counter->villainGroup))
                {
                    desiredVillainGroupID = GetVillainGroupIDFromName(counter->villainGroup);
                    victimVillainGroupID = GetVillainGroupID(player);
                    if (killDebugFlag)
                    {
                        ScriptedZoneEventSendDebugPrintf(0,"Found a villaingroup counter for '%s', desiredVillaingGroup = %d (%s), victimVillaingGroup = %d (%s)",
                                            counter->villainGroup, desiredVillainGroupID, villainGroupGetName(desiredVillainGroupID), 
                                            victimVillainGroupID, villainGroupGetName(victimVillainGroupID));
                    }
                    // If the victim's group matches
                    if (desiredVillainGroupID && desiredVillainGroupID == victimVillainGroupID)
                    {
                        // If a volume was given, make sure he's in it.
                        if (!IsValidStr(counter->volumeName) || StringEqual(victor, TEAM_NONE) || EntityInArea(victor, StringAdd("trigger:", counter->volumeName)))
                        {
                            // At last.  Something to count.  But only if we didn't hit the target
                            if (counter->progress < counter->target)
                            {
                                counter->progress++;
                            }
                        }
                        else
                        {
                            if (killDebugFlag)
                            {
                                ScriptedZoneEventSendDebugPrintf(0,"Rejected, not in volume %s", counter->volumeName);
                            }
                        }
                    }
                    else
                    {
                        if (killDebugFlag)
                        {
                            chatSendDebug("Rejected, either desiredVillaingGroup = 0 or desiredVillaingGroup != victimVillaingGroup");
                        }
                    }
                }
                // for team kill counters with a meaningful team
                else if (counter->type == counter_Team && IsValidStr(counter->team))
                {
                    if (IsEntityOnScriptTeam(player, counter->team))
                    {
                        if (counter->direction == counter_Down)
                        {
                            counter->progress = NumEntitiesInTeam(counter->team);
                        }
                        else
                        {
                            // At last.  Something to count.  But only if we didn't hit the target
                            if (counter->progress < counter->target)
                            {
                                counter->progress++;
                            }
                        }
                    }
                }
                // for named kill counters with a meaningful name
                else if (counter->type == counter_Name && IsValidStr(counter->name))
                {
                    if (StringEqual(GetVillainDefName(player), counter->name))
                    {
                        if (counter->direction == counter_Down)
                        {
                            counter->progress--;
                            if (counter->progress < 0)
                            {
                                devassert(0 && "Named counter progress was negative");
                                counter->progress = 0;
                            }
                        }
                        else
                        {
                            if (counter->progress < counter->target)
                            {
                                counter->progress++;
                            }
                        }
                    }
                }

            }
            for (j = (eaSize(&stage->objectives)-1); j >= 0; --j)
            {
                int isObjDefeat = 0;
                ScriptObjective *objective = stage->objectives[j];
                // for glowie objectives with a meaningful glowie
                if (IsValidStr(objective->volumeName) && !EntityInArea(player, StringAdd("trigger:", objective->volumeName)))
                {
                    continue;
                }

                //    else
                switch (objective->type)
                {
                case objective_Team:
                    if (IsValidStr(objective->team))
                    {
                        isObjDefeat = IsEntityOnScriptTeam(player, objective->team);
                    }
                    break;
                case objective_Named:
                    if (IsValidStr(objective->name))
                    {
                        isObjDefeat = StringEqual(GetVillainDefName(player), objective->name);
                    }
                case objective_VillainGroup:
                    if (IsValidStr(objective->villainGroup))
                    {
                        victimVillainGroupID = GetVillainGroupID(player);
                        desiredVillainGroupID = GetVillainGroupIDFromName(objective->villainGroup);
                        isObjDefeat = (desiredVillainGroupID == victimVillainGroupID);
                    }
                    break;
                default:
                    break;
                }
                if (isObjDefeat)
                {
                    if (IsKarmaEnabled())
                    {
                        ScriptAddObjectiveKarma(victor, objective->teamKarmaValue, objective->teamKarmaLife, objective->karmaValue, objective->karmaLife);
                    }
                }
            }
        }
        else
        {
            if (killDebugFlag)
            {
                chatSendDebug("Rejected, can't find current stage");
            }
        }
    }
    else if(IsEntityPlayer(player))
    {
        CheckGurney(player);
        if (stage != NULL)
        {
            for (j = eaSize(&stage->counters) - 1; j >= 0; j--)
            {
                counter = stage->counters[j];
                // for player death counters
                if (counter->type == counter_PlayerDeaths)
                {
                    if (counter->direction == counter_Down)
                    {
                        counter->progress--;
                        if (counter->progress < 0)
                        {
                            //    the number of deaths may actually go below zero if multiple people die in the same tick before
                            //    any designer intended stage change. just floor it to zero instead of letting it go negative
                            counter->progress = 0;
                        }
                    }
                    else
                    {
                        if (counter->progress < counter->target)
                        {
                            counter->progress++;
                        }
                    }
                }
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////
// Called whenever an entity is created.  The only reason this is here is to see if we're
// a pet, and if so whether our owner wants us on a team.  Note that the way we get the
// desired team is to set an EntVar on the owner that specifies the team.  This is set
// when FireSpawn(...) actually spawns critters
//
void ScriptedZoneEventOnEntityCreate(ENTITY player)
{
    ENTITY owner;
    TEAM petTeam;
    ScriptDefinition *script;

    owner = GetTrueOwner(player);
    if (!StringEqual(owner, TEAM_NONE))
    {
        petTeam = VarGet(EntVar(owner, "PetTeam"));
        if (IsValidStr(petTeam))
        {
            SetScriptTeam(player, petTeam);
        }
    }
    // If I'm running (i.e. have an active stage)
    script = GetCurrentScript();
    if (script)
    {
        ScriptStage *stage = script->currentStage;
        if (stage != NULL)
        {
            int j;
            // scan all counters in this stage
            for (j = eaSize(&stage->counters) - 1; j >= 0; j--)
            {
                ScriptCounter *counter = stage->counters[j];
                if (counter->type == counter_Name && IsValidStr(counter->name))
                {
                    if (StringEqual(GetVillainDefName(player), counter->name))
                    {
                        if (counter->direction == counter_Down)
                        {
                            counter->progress++;
                        }
                    }
                }
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////
// Called when a player first clicks a glowie.  Do nothing for now, if we need to 
// arrange it that only certain players can do this, this is where that would happen.
// return 1; to disallow the player clicking
//
static int GlowieInteract(ENTITY player, ENTITY target)
{
    int which;
    int j;
    int p;
    char index[4];
    ScriptDefinition *script;
    ScriptGlowie *glowie;
    TEAM guardSpawnTeam;

    which = -1;
    script = GetCurrentScript();
    if (script)
    {
        // For each glowie this script knows about
        for (glowie = script->glowies; glowie; glowie = glowie->next)
        {
            // Look through all placed glowies (overloaded term :o )
            for (j = 0; j < glowie->quantity; j++)
            {
                // See if this is the one that was clicked on
                if (StringEqual(glowie->glowies[j], target))
                {
                    which = j;
                    break;
                }
            }
            // It occurs to me it's probably an error to reach here with which == -1.  However, there's damn all I can do
            // if that happens, except fail gracefully and do nothing.
            // We do the heavy lifting here, even though we're still in the outer for loop - we just break out at the bottom.
            if (which != -1)
            {
                if (IsValidStr(glowie->guardSpawn) && IsValidStr(glowie->guardProtect))
                {
                    p = glowie->glowieSpots[which];
                    index[0] = '-';
                    index[1] = p / 10 + '0';
                    index[2] = p % 10 + '0';
                    index[3] = 0;
                    guardSpawnTeam = StringAdd(StringAdd(glowie->guardSpawn, index), ".GuardSpawn");        
                    if (NumEntitiesInTeam(guardSpawnTeam))
                    {
                        ScriptSendDialog(player, StringLocalize(glowie->guardProtect));
                        return 1;
                    }
                }
                return 0;
            }
        }
    }
    return 0;
}

///////////////////////////////////////////////////////////////////////////////////////
// Called when glowie is complete.  Look for the glowie in any counters that are
// active, and increase the count if any found.
//
static int GlowieComplete(ENTITY player, ENTITY target)
{
    int which;
    int j;
    ScriptDefinition *script;
    ScriptStage *stage;
    ScriptCounter *counter;
    ScriptGlowie *glowie;
    ScriptObjective *objective;

    which = -1;
    script = GetCurrentScript();
    if (script)
    {
        // For each glowie this script knows about
        for (glowie = script->glowies; glowie; glowie = glowie->next)
        {
            // Look through all placed glowies (overloaded term :o )
            for (j = 0; j < glowie->quantity; j++)
            {
                // See if this is the one that was clicked on
                if (StringEqual(glowie->glowies[j], target))
                {
                    which = j;
                    break;
                }
            }
            // It occurs to me it's probably an error to reach here with which == -1.  However, there's damn all I can do
            // if that happens, except fail gracefully and do nothing.
            // We do the heavy lifting here, even though we're still in the outer for loop - we just break out at the bottom.
            if (which != -1)
            {
                // Update any counters that might be watching
                stage = script->currentStage;
                if (stage != NULL)
                {
                    for (j = eaSize(&stage->counters) - 1; j >= 0; j--)
                    {
                        counter = stage->counters[j];
                        if (counter->type == counter_Click && IsValidStr(counter->glowie))
                        {
                            if (StringEqual(counter->glowie, glowie->glowieName))
                            {
                                if (counter->progress < counter->target)
                                {
                                    counter->progress++;
                                }
                            }
                        }
                    }

                    // scan all objectives in this stage
                    for (j = eaSize(&stage->objectives) - 1; j >= 0; j--)
                    {
                        objective = stage->objectives[j];
                        // for glowie objectives with a meaningful glowie
                        if (objective->type == objective_Glowie && IsValidStr(objective->glowie) && StringEqual(objective->glowie, glowie->glowieName))
                        {
                            if (IsKarmaEnabled())
                            {
                                if (objective->teamKarmaValue || objective->karmaValue)
                                {
                                    int k;
                                    for (k = NumEntitiesInTeam(GetScriptTeam()); k > 0; --k )
                                    {
                                        ENTITY target = GetEntityFromTeam(GetScriptTeam(), k);
                                        ScriptGlowieBubbleActivate(player, target);
                                    }
                                    ScriptAddObjectiveKarma(player, objective->teamKarmaValue, objective->teamKarmaLife, objective->karmaValue, objective->karmaLife);
                                }
                            }
                        }
                    }
                }

                // If there's a reward on this glowie, award it now
                if (glowie->reward)
                {
                    EntityGrantReward(player, glowie->reward);
                }

                // Handle resetting the glowie
                if (glowie->resetDelay)
                {
                    // Delayed reset
                    glowie->glowieTimers[which] = timerSecondsSince2000() + glowie->resetDelay;
                    // Turn it off
                    GlowieClearState(glowie->glowies[which]);
                    // If they vanish and reappear, blow it away completely
                    if (glowie->vanishOnComplete)
                    {
                        // Blow it away completely
                        GlowieRemove(glowie->glowies[which]);
                        freeConst(glowie->glowies[which]);
                        glowie->glowies[which] = NULL;
                    }

                    // Blow away the waypoint if it exists
                    if (glowie->placedWaypoint.mapWaypoints != NULL && glowie->placedWaypoint.mapWaypoints[which] > 0)
                    {
                        ClearWaypoint(ALL_PLAYERS, glowie->placedWaypoint.mapWaypoints[which]);
                    }
                }
                else
                {
                    // Not delayed, just turn it back on
                    GlowieSetState(glowie->glowies[which]);
                }

                break;
            }
        }
    }
    return 1;
}

#define OFF_IN_LEFT_FIELD "coord:1000000.0,0.0,0.0"

///////////////////////////////////////////////////////////////////////////////////////
// Init a glowie def.  Do this one time only to set everything up.  The Place, Remove,
// Activate and Deactivate routines will take care of actually making them available.
//
static void InitGlowie(ScriptGlowie *glowie)
{
    ScriptDefinition *script;

    glowie->numLoc = CountMarkers(glowie->location);
    devassert(glowie->quantityStr);
    if (glowie->quantityStr)
    {
        glowie->quantity = ScriptedZoneEvent_Eval(glowie->quantityStr);
    }
    if (glowie->quantity <= 0)
    {
        // Do something intelligent if no quantity specified ...
        glowie->quantity = 1;
    }
    if (glowie->quantity > glowie->numLoc)
    {
        // ... or it's stupid
        glowie->quantity = glowie->numLoc;
    }

    glowie->glowies = malloc(sizeof(ENTITY) * glowie->quantity);
    glowie->glowieTimers = malloc(sizeof(U32) * glowie->quantity);
    glowie->glowieSpots = malloc(sizeof(U32) * glowie->quantity);
    glowie->glowieUsed = malloc(sizeof(U8) * glowie->numLoc);
    if (glowie->glowies == NULL || glowie->glowieTimers == NULL || glowie->glowieSpots == NULL || glowie->glowieUsed == NULL)
    {
        // Clean up - make no assumptions whatsoever about which mallocs failed or succeeded.
        if (glowie->glowies != NULL)
        {
            freeConst(glowie->glowies);
        }
        if (glowie->glowieTimers != NULL)
        {
            free(glowie->glowieTimers);
        }
        if (glowie->glowieSpots != NULL)
        {
            free(glowie->glowieSpots);
        }
        if (glowie->glowieUsed != NULL)
        {
            free(glowie->glowieUsed);
        }
        glowie->glowies = NULL;
        glowie->glowieTimers = NULL;
        glowie->glowieSpots = NULL;
        glowie->glowieUsed = NULL;
        return;
    }
    devassert(glowie->displayName);
    glowie->glowieDef = GlowieCreateDef(glowie->displayName ? StringLocalize(glowie->displayName) : "Glowie", glowie->model,
                                        glowie->interact, glowie->interrupt,
                                        glowie->complete, glowie->barText, glowie->interactTime);
    GlowieSetDescriptions(glowie->glowieDef, NULL, NULL);
    
    if(IsValidStr(glowie->effectInactive))
        GlowieAddEffect(glowie->glowieDef, kGlowieEffectInactive, glowie->effectInactive);
    if(IsValidStr(glowie->effectActive))
        GlowieAddEffect(glowie->glowieDef, kGlowieEffectActive, glowie->effectActive);
    if(IsValidStr(glowie->effectCooldown))
        GlowieAddEffect(glowie->glowieDef, kGlowieEffectCooldown, glowie->effectCooldown);
    if(IsValidStr(glowie->effectCompletion))
        GlowieAddEffect(glowie->glowieDef, kGlowieEffectCompletion, glowie->effectCompletion);
    if(IsValidStr(glowie->effectFailure))
        GlowieAddEffect(glowie->glowieDef, kGlowieEffectFailure, glowie->effectFailure);

    // glowie defs are ref counted??  So "lock" it in place with a dummy create
    GlowiePlace(glowie->glowieDef, OFF_IN_LEFT_FIELD, false, GlowieInteract, GlowieComplete);
    // clear the four arrays
    memset((void*)glowie->glowies, 0, sizeof(ENTITY) * glowie->quantity);
    memset(glowie->glowieTimers, 0, sizeof(U32) * glowie->quantity);
    memset(glowie->glowieSpots, 0, sizeof(U32) * glowie->quantity);
    memset(glowie->glowieUsed, 0, sizeof(U8) * glowie->numLoc);

    if (glowie->charRequires)
        GlowieSetCharRequires(glowie->glowieDef, glowie->charRequires, glowie->charRequiresFailedText);
    
    // And hook into the list.  Calling this a second time on the same glowie would be a "bad thing" (tm),
    // since I suspect it'd create a loop in the linked list.  However the one place that calls this checks
    // that we're not yet in the list, so this should never happen.
    script = GetCurrentScript();
    glowie->next = script->glowies;
    script->glowies = glowie;
}

// Place the glowies on the map, however they are not activated.
static void PlaceGlowies(ScriptGlowie *glowie)
{
    int i;
    int p;
    int available;
    int swap;
    int temp;
    LOCATION loc;
    char index[4];
    int locArray[MAX_MARKER_COUNT];

    available = 0;
    for (i = 0; i < glowie->numLoc; i++)
    {
        if (glowie->glowieUsed[i] == 0)
        {
            locArray[available++] = i + 1;
        }
    }
    if (glowie->quantity <= available)
    {
        // Shuffle using Fisher-Yates
        for (i = available - 1; i >= 1; i--)
        {
            swap = RandomNumber(0, i);
            temp = locArray[swap];
            locArray[swap] = locArray[i];
            locArray[i] = temp;
        }
    }
    else
    {
        // Not enough space for this lot.  Just forget everything and start again
        available = glowie->numLoc;
        for (i = 0; i < glowie->numLoc; i++)
        {
            if (glowie->glowieUsed[i] == 0)
            {
                locArray[i] = i + 1;
            }
        }
        if (glowie->quantity < available)
        {
            // Shuffle using Fisher-Yates
            for (i = available - 1; i >= 1; i--)
            {
                swap = RandomNumber(0, i);
                temp = locArray[swap];
                locArray[swap] = locArray[i];
                locArray[i] = temp;
            }
        }
    }

    for (i = 0; i < glowie->quantity; i++)
    {
        p = locArray[i];
        if (glowie->glowies[i] == NULL)
        {
            index[0] = '_';
            index[1] = p / 10 + '0';
            index[2] = p % 10 + '0';
            index[3] = 0;
            loc = StringAdd(StringAdd("marker:", glowie->location), index);
            glowie->glowies[i] = strdup(GlowiePlace(glowie->glowieDef, loc, false, GlowieInteract, GlowieComplete));
            if (glowie->glowies[i])
            {
                GlowieClearState(glowie->glowies[i]);
                glowie->glowieSpots[i] = p;
            }
        }
        glowie->glowieTimers[i] = 0;
    }
}

static void ActivateGlowies(ScriptGlowie *glowie)
{
    int i;
    int p;
    char index[4];
    LOCATION loc;
    STRING spawn;
    ENCOUNTERGROUP group;
    TEAM guardSpawnTeam;

    for (i = 0; i < glowie->quantity; i++)
    {
        if (glowie->glowies[i])
        {
            GlowieSetState(glowie->glowies[i]);

            p = glowie->glowieSpots[i];
            index[0] = '_';
            index[1] = p / 10 + '0';
            index[2] = p % 10 + '0';
            index[3] = 0;
            loc = StringAdd(StringAdd("marker:", glowie->location), index);

            if (IsValidStr(glowie->guardSpawn))
            {
                index[0] = '-';
                spawn = StringAdd(glowie->guardSpawn, index);
                group = FindEncounterGroupByRadius(loc, 0, 50, spawn, 0, 0);
                if (!StringEqual(group, "location:none"))
                {
                    Spawn(1, tempCreationTeam, "UseCanSpawns", group, spawn, 0, ScriptedZoneEventGetActivePlayersForSpawning());
                    SetAIPriorityList(tempCreationTeam, "Combat");
                    DetachSpawn(tempCreationTeam);
                    guardSpawnTeam = StringAdd(spawn, ".GuardSpawn");        
                    SwitchScriptTeam(tempCreationTeam, tempCreationTeam, guardSpawnTeam);
                }
            }
            if (glowie->isMarked && glowie->placedWaypoint.mapWaypoints[i] > 0)
            {
                SetWaypoint(GetScriptTeam(), glowie->placedWaypoint.mapWaypoints[i]);
            }
        }
        glowie->glowieTimers[i] = 0;
    }
}

static void DeactivateGlowies(ScriptGlowie *glowie)
{
    int i;

    for (i = 0; i < glowie->quantity; i++)
    {
        if (glowie->glowies[i])
        {
            GlowieClearState(glowie->glowies[i]);
        }
        glowie->glowieTimers[i] = 0;
    }
    if (glowie->placedWaypoint.mapWaypoints != NULL)
    {
        for (i = ea32Size(&glowie->placedWaypoint.mapWaypoints) - 1; i >= 0; i--)
        {
            if (glowie->placedWaypoint.mapWaypoints[i] > 0)
            {
                ClearWaypoint(ALL_PLAYERS, glowie->placedWaypoint.mapWaypoints[i]);
            }
        }
    }
}

// Deactivate and remove this set of glowies.
static void RemoveGlowies(ScriptGlowie *glowie)
{
    int i;

    for (i = 0; i < glowie->quantity; i++)
    {
        if (glowie->glowies[i])
        {
            GlowieClearState(glowie->glowies[i]);
            GlowieRemove(glowie->glowies[i]);
            freeConst(glowie->glowies[i]);
            glowie->glowies[i] = NULL;
        }
        glowie->glowieTimers[i] = 0;
        if (glowie->rememberLocations)
        {
            glowie->glowieUsed[glowie->glowieSpots[i] - 1] = 1;
        }
    }

    if (glowie->placedWaypoint.mapWaypoints != NULL)
    {
        for (i = ea32Size(&glowie->placedWaypoint.mapWaypoints) - 1; i >= 0; i--)
        {
            if (glowie->placedWaypoint.mapWaypoints[i] > 0)
            {
                ClearWaypoint(ALL_PLAYERS, glowie->placedWaypoint.mapWaypoints[i]);
                DestroyWaypoint(glowie->placedWaypoint.mapWaypoints[i]);
                glowie->placedWaypoint.mapWaypoints[i] = 0;
            }
        }
        ea32Destroy(&glowie->placedWaypoint.mapWaypoints);
        glowie->placedWaypoint.mapWaypoints = NULL;
    }
    glowie->isMarked = 0;
}

static void MarkGlowie(ScriptGlowie *glowie)
{
    int i;
    int p;
    int mapWaypoint;
    LOCATION location;
    ScriptWaypoint *waypoint;
    ScriptWaypointPlace *placedWaypoint;
    char index[4];
    Vec3 vec;
    ScriptDefinition *script;

    script = GetCurrentScript();
    if (glowie->waypoint == NULL || (waypoint = FindWaypointByName(script, glowie->waypoint)) == NULL)
    {
        return;
    }

    placedWaypoint = &glowie->placedWaypoint;
    glowie->isMarked = 1;

    if (placedWaypoint->next == NULL)
    {
        // Link into the script wide list of placed waypoints
        placedWaypoint->next = script->placedWaypoints;
        script->placedWaypoints = placedWaypoint;
    }

    if (placedWaypoint->mapWaypoints == NULL)
    {
        ea32Create(&placedWaypoint->mapWaypoints);
    }
    ea32SetCapacity(&placedWaypoint->mapWaypoints, glowie->quantity);
    memset(placedWaypoint->mapWaypoints, 0, glowie->quantity * sizeof(U32));

    placedWaypoint->waypoint = glowie->waypoint;

    for (i = 0; i < glowie->quantity; i++)
    {
        if (glowie->glowies[i])
        {
            p = glowie->glowieSpots[i];
            index[0] = '_';
            index[1] = p / 10 + '0';
            index[2] = p % 10 + '0';
            index[3] = 0;
            location = StringAdd(StringAdd("marker:", glowie->location), index);

            if (GetPointFromLocation(location, vec) == 0)
            {
                // This should never happen, but in the unlikely event that it does, fail gracefully
                continue;
            }

            // Try and creat the waypoint.
            mapWaypoint = CreateWaypoint(location, waypoint->text, waypoint->icon, NULL, false, false, -1.0f);
            if (mapWaypoint <= 0)
            {
                // This should never happen, but in the unlikely event that it does, fail gracefully
                continue;
            }
            placedWaypoint->mapWaypoints[i] = mapWaypoint;
        }
    }
}

static void InitSpawn(ScriptSpawn *spawn)
{
    ENCOUNTERGROUPITERATOR iter;
    ENCOUNTERGROUP group;
    ScriptDefinition *script;

    // Link it into the list.
    script = GetCurrentScript();
    spawn->next = script->spawns;
    script->spawns = spawn;
    spawn->active = 0;

    EncounterGroupsInitIterator(&iter, NULL, 0.0f, 0.0f, NULL, spawn->group, NULL);
    eaCreateConst(&spawn->groups);
    for (;;)
    {
        group = EncounterGroupsFindNext(&iter);
        if (StringEqual(group, LOCATION_NONE))
        {
            break;
        }
        eaPushConst(&spawn->groups, strdup(group));
    }
    if (eaSize(&spawn->groups) == 0)
    {
        group = FindEncounterGroupByRadius(StringAdd("marker:", spawn->location),
                                            0, 50, spawn->group, 0, 0);  
        if (!StringEqual(group, LOCATION_NONE))
        {
            eaPushConst(&spawn->groups, strdup(group));
        }
    }
}

static void InitTeam(ScriptTeamRespawn *team)
{
    ScriptDefinition *script;

    // Just link it into the list for now.
    script = GetCurrentScript();
    team->next = script->teams;
    script->teams = team;
    team->active = 0;
}
static void ScriptedZoneEvent_AttachPets(ScriptTeamRespawn *team, TEAM tempCreationTeam)
{
    devassert(team && team->creator);
    devassert(tempCreationTeam);
    if (team && team->creator && tempCreationTeam)
    {
        const char *creatorTeam = NULL;
        if (stricmp(team->creator, "PlayerTeam") == 0)
        {
            creatorTeam = GetScriptTeam();
        }
        else if (ScriptTeamValid(team->creator))
        {
            creatorTeam = team->creator;
        }
        if (creatorTeam)
        {
            int numEnts = NumEntitiesInTeam(creatorTeam);
            if ( numEnts > 0)
            {
                team->creatorEnt = GetEntityFromTeam(creatorTeam, RandomNumber(1,numEnts));
            }
            else
            {
                // See comments at function definition for why team->creatorEnt is both a parmeter and the return value.
                team->creatorEnt = GetVillainByDefName(creatorTeam, team->creatorEnt);
            }
            SetFollowers( team->creatorEnt, tempCreationTeam);
        }
    }
}
static void FireSpawn(ENCOUNTERGROUP group, ScriptSpawn *spawn, ScriptTeamRespawn *team, int size, int fountainSpawn)
{
    int i;
    int n;
    ENTITY spawnedCritter;

    // Place them initially in a separate team, so we can detach them, adjust their behavior, etc. without
    // disturbing anything else.
    Spawn(1, tempCreationTeam, "UseCanSpawns", group, spawn->group, 0, size);
    if (IsValidStr(spawn->appearBehavior))
    {
        AddAIBehavior(tempCreationTeam, spawn->appearBehavior); 
    }
    if (IsValidStr(spawn->moveToMarker))
    {
        SetAIMoveTarget(tempCreationTeam, StringAdd("marker:", spawn->moveToMarker), HIGH_PRIORITY, 0, 1, 30.0f);
    }
    if (spawn->friendly)
    {
        if (server_state.allowHeroAndVillainPlayerTypesToTeamUp)
        {
            SetAIGang(tempCreationTeam, GANG_OF_PLAYERS_WHEN_IN_COED_TEAMUP);
        }
        else if (MAP_ALLOWS_VILLAINS_ONLY(MapSpecFromMapId(db_state.base_map_id)))
        {
            SetAIAlly(tempCreationTeam, kAllyID_Villain);
        }
        else
        {
            SetAIAlly(tempCreationTeam, kAllyID_Hero);
        }
    }

    if (fountainSpawn)
    {
        DetachSpawn(tempCreationTeam);
    }

    if (IsValidStr(team->creator))
    {
        ScriptedZoneEvent_AttachPets(team, tempCreationTeam);
    }
    // If a petTeam is given, then for every critter in this team set an EntVar for them that names
    // the team their pets should join.  This is used in ScriptedZoneEventOnEntityCreate(...) to
    // hook pets to the correct team.
    if (IsValidStr(team->petTeam))
    {
        n = NumEntitiesInTeam(tempCreationTeam);
        for (i = 1; i <= n; i++)
        {
            spawnedCritter = GetEntityFromTeam(tempCreationTeam, i);
            VarSet(EntVar(spawnedCritter, "PetTeam"), team->petTeam);
        }
    }
    SwitchScriptTeam(tempCreationTeam, tempCreationTeam, spawn->team);
}

static void ActivateSpawn(ScriptSpawn *spawn, ScriptTeamRespawn *team, int fountainSpawn)
{
    int i;
    ENCOUNTERGROUP group;

    if (!spawn->active)
    {
        for (i = eaSize(&spawn->groups) - 1; i >= 0; i--)
        {
            group = spawn->groups[i];
            // I'm not entirely convinced this test will ever reject anything.
            if (StringEqual(group, "location:none") == 0)
            {
                int size; 
                if (team->flags & teamRespawnFlags_UsePlayerCount)
                {
                    STRING activeTeam = GetScriptTeam();
                    size = NumEntitiesInTeam(activeTeam);
                }
                else
                {
                    size = ScriptedZoneEventGetActivePlayersForSpawning();
                }
                FireSpawn(group, spawn, team, size, fountainSpawn);
            }
        }
        spawn->active = 1;
    }
}

static void DeactivateSpawn(ScriptSpawn *spawn)
{
    if (spawn->active)
    {
        if (IsValidStr(spawn->killBehavior))
        {
            SetAIPriorityList(spawn->team, spawn->killBehavior); 
        }
        spawn->active = 0;
    }
}

// Figure the minimum and full count values for 
static void DetermineCountsForFountainSpawn(ScriptTeamRespawn *team, int currPlayerCount, int *pMinCount, int *pFullCount)
{
    int minCount = 0;
    int fullCount = 0;
    int numTeamSizeCount = 0;

    // Determine proper counts
    if (numTeamSizeCount = eaSize(&team->teamSizeCounts)) // if there are size counts
    {
        for (numTeamSizeCount--; numTeamSizeCount >= 0; numTeamSizeCount--) // initialize with a decrement so I'm in the array
        {
            if (team->teamSizeCounts[numTeamSizeCount]->playerCount <= currPlayerCount)
            {
                minCount = team->teamSizeCounts[numTeamSizeCount]->minimumCount;
                fullCount = team->teamSizeCounts[numTeamSizeCount]->fullCount;
                break;
            }
        }
    }
    else
    {
        minCount = team->minimumCount;
        fullCount = NumEntitiesInTeam(team->teamName) + 1; // ensure we trigger precisely one spawndef repopulation
    }
    if (pMinCount != NULL)
    {
        *pMinCount = minCount;
    }
    if (pFullCount != NULL)
    {
        *pFullCount = fullCount;
    }
}

// Repopulate spawndefs for a team
static void RepopulateSpawnDefsForFountainSpawn(ScriptDefinition *script, ScriptTeamRespawn *team, int currPlayerCount, int fullCount)
{
    ScriptSpawn **matchingSpawnList = NULL;
    ScriptSpawn *spawn;
    int randomSpawn = randInt(team->spawnCount);
    int numSpawns = 0;
    for (spawn = script->spawns; spawn; spawn = spawn->next)
    {
        if (StringEqual(team->teamName, spawn->team))
        {
            eaPush(&matchingSpawnList, spawn);
        }
    }

    numSpawns = eaSize(&matchingSpawnList);
    if (numSpawns)
    {
        int maxLoops = 100;
        int i = 0;
        int spawnIndex = 0;
        int groupIndex = 0;
        int randomGroup = 0;
        while (NumEntitiesInTeam(team->teamName) < fullCount && i < maxLoops)
        {
            int numGroups = 0;
            ENCOUNTERGROUP group;
            spawn = matchingSpawnList[(randomSpawn + spawnIndex) % numSpawns];
            numGroups = eaSize(&spawn->groups);
            if (numGroups)
            {
                if (groupIndex == 0)
                {
                    randomGroup = randInt(numGroups);
                }

                group = spawn->groups[(randomGroup + groupIndex ) % numGroups];
                if (!StringEmpty(group) && !StringEqual(group, "location:none"))
                {
                    FireSpawn(group, spawn, team, currPlayerCount, 1); /* */
                    spawn->active = 1;
                }

                groupIndex++;
            }
            if (groupIndex >= numGroups)
            {
                groupIndex = 0;
                spawnIndex++;
            }
            i++;
        }
    }
    eaDestroy(&matchingSpawnList);

    team->tickCount = 0;
}

// Trigger a single team's worth of spawns for a fountain spawn.
static void TriggerFountainWavesForTeam(ScriptDefinition *script, int forceWave, ScriptTeamRespawn *team)
{
    int currPlayerCount = 0;
    int minCount = 0;
    int fullCount = 0;
    
    if (team->active && team->spawnCount && // is spawnable team
        team->minRespawnTime && (team->minimumCount || eaSize(&team->teamSizeCounts))) // is in fountain mode, allowed to not have a max time.
    {
        int size; 
        if (team->flags & teamRespawnFlags_UsePlayerCount)
        {
            STRING activeTeam = GetScriptTeam();
            size = NumEntitiesInTeam(activeTeam);
        }
        else
        {
            size = ScriptedZoneEventGetActivePlayersForSpawning();
        }

        // Team is a valid fountain spawn at this point
        ++team->tickCount;
        currPlayerCount = max(size, 1);

        DetermineCountsForFountainSpawn(team, currPlayerCount, &minCount, &fullCount);

        if (minCount > 0 && fullCount > 0)
        {
            if ((team->tickCount >= team->minRespawnTime && IsValidStr(team->teamName) && NumEntitiesInTeam(team->teamName) < minCount)
                || (team->maxRespawnTime > 0 && team->tickCount >= team->maxRespawnTime)
                || forceWave)
            {
                RepopulateSpawnDefsForFountainSpawn(script, team, currPlayerCount, fullCount);
            }
        }
    }
}

// Trigger every fountain spawn the script knows about
static void TriggerAllFountainWaves(ScriptDefinition *script, int forceWave)
{
    ScriptTeamRespawn *team;

    for (team = script->teams; team; team = team->next)
    {
        TriggerFountainWavesForTeam(script, forceWave, team);
    }
}

//
static void InitDoor(ScriptDoor *door)
{
    ScriptDefinition *script;

    script = GetCurrentScript();
    door->next = script->doors;
    script->doors = door;
    door->state = 0;
}

static void UnlockZoneDoor(ScriptDoor *door)
{
    door->state = 1;
}

static void LockZoneDoor(ScriptDoor *door)
{
    door->state = 0;
}

static void InitPort(ScriptPort *port)
{
    ScriptDefinition *script;

    script = GetCurrentScript();
    port->next = script->ports;
    script->ports = port;
    port->active = 0;
}

static void ActivatePort(ScriptPort *port)
{
    port->active = 1;
}

static void DeactivatePort(ScriptPort *port)
{
    port->active = 0;
}

static void InitGurney(ScriptGurney *gurney)
{
    ScriptDefinition *script;

    script = GetCurrentScript();
    gurney->next = script->gurneys;
    script->gurneys = gurney;
    gurney->active = 0;
}

static void ActivateGurney(ScriptGurney *gurney)
{
    gurney->active = 1;
    CheckGurney(GetScriptTeam());
}

static void DeactivateGurney(ScriptGurney *gurney)
{
    gurney->active = 0;
    CheckGurney(GetScriptTeam());
}

static void InitWaypoint(ScriptWaypoint *waypoint)
{
    ScriptDefinition *script;

    script = GetCurrentScript();
    waypoint->next = script->waypoints;
    script->waypoints = waypoint;
}

static void PlaceWaypoint(ScriptWaypointPlace *placedWaypoint)
{
    int i;
    int mapWaypoint;
    LOCATION basename;
    LOCATION location;
    ScriptWaypoint *waypoint;
    char index[4];
    Vec3 vec;
    ScriptDefinition *script;

    // Use the presence of the next link to decide if this has already been placed.  If so, don't do it a second time.
    if (placedWaypoint->next != NULL)
    {
        return;
    }

    if (placedWaypoint->mapWaypoints != NULL)
    {
        ea32Destroy(&placedWaypoint->mapWaypoints);
        placedWaypoint->mapWaypoints = NULL;
    }

    script = GetCurrentScript();
    waypoint = FindWaypointByName(script, placedWaypoint->waypoint);
    if (waypoint == NULL)
    {
        return;
    }

    // Link into the script wide list of placed waypoints
    placedWaypoint->next = script->placedWaypoints;
    script->placedWaypoints = placedWaypoint;

    // First look directly for the location.  If we find that, just place a single waypoint and finish
    basename = StringAdd("marker:", placedWaypoint->location);
    if (GetPointFromLocation(basename, vec))
    {
        mapWaypoint = CreateWaypoint(basename, waypoint->text, waypoint->icon, NULL, waypoint->compass, false, -1.0f);
        if (mapWaypoint > 0)
        {
            ea32Create(&placedWaypoint->mapWaypoints);
            ea32Push(&placedWaypoint->mapWaypoints, (U32) mapWaypoint);
            SetWaypoint(GetScriptTeam(), mapWaypoint);
        }
        return;
    }
    // Didn't find the base name.  Search for Location_01 etc. and place multiple copies on these
    i = 1;
    for (;;)
    {
        index[0] = '_';
        index[1] = i / 10 + '0';
        index[2] = i % 10 + '0';
        index[3] = 0;
        i++;

        // This will eventually fail, when it walks off the end of the waypoints
        location = StringAdd(basename, index);
        if (GetPointFromLocation(location, vec) == 0)
        {
            return;
        }

        // Try and creat the waypoint.
        mapWaypoint = CreateWaypoint(location, waypoint->text, waypoint->icon, NULL, waypoint->compass, false, -1.0f);
        if (mapWaypoint <= 0)
        {
            return;
        }
        if (placedWaypoint->mapWaypoints == NULL)
        {
            // Create the array first time through
            ea32Create(&placedWaypoint->mapWaypoints);
        }
        ea32Push(&placedWaypoint->mapWaypoints, (U32) mapWaypoint);
        SetWaypoint(GetScriptTeam(), mapWaypoint);
    }
    // NOTREACHED
}

static void RemoveWaypoints()
{
    int i;
    int mapWaypoint;
    ScriptWaypointPlace *placedWaypoint;
    ScriptDefinition *script;

    script = GetCurrentScript();
    ClearAllWaypoints(ALL_PLAYERS);

    while ((placedWaypoint = script->placedWaypoints) != NULL)
    {
        script->placedWaypoints = placedWaypoint->next;
        placedWaypoint->next = NULL;
        for (i = ea32Size(&placedWaypoint->mapWaypoints) - 1; i >= 0; i--)
        {
            mapWaypoint = placedWaypoint->mapWaypoints[i];
            DestroyWaypoint(mapWaypoint);
        }
    }
}

static void SetPlacedWaypoints(ENTITY player)
{
    int i;
    int mapWaypoint;
    ScriptWaypointPlace *placedWaypoint;
    ScriptDefinition *script;

    script = GetCurrentScript();

    placedWaypoint = script->placedWaypoints;
    while(placedWaypoint != NULL)
    {
        for (i = ea32Size(&placedWaypoint->mapWaypoints) - 1; i >= 0; i--)
        {
            mapWaypoint = placedWaypoint->mapWaypoints[i];
            SetWaypoint(player, mapWaypoint);
        }
        placedWaypoint = placedWaypoint->next;
    }
}

static void UpdateGurney(TEAM team, STRING gurney)
{
    int i;
    ENTITY player;

    for (i = NumEntitiesInTeam(team); i > 0; i--)
    {
        player = GetEntityFromTeam(team, i);
        OverridePrisonGurneyWithTarget(player, 0, gurney);
    }
}

static bool EntityInVolume(ENTITY player, AREA volume)
{
    if(!IsValidStr(volume) || EntityInArea(player, StringAdd("trigger:", volume)))
        return true;

    return false;
}

static void CheckGurney(TEAM team)
{
    ScriptDefinition *script;
    ScriptGurney *gurney;
    bool gurneyOverridden = false;
    ENTITY player;
    int i, n;
    
    script = GetCurrentScript();
    if(!script) return;
    
    EntTeamInternalEx(team, -1, &n, 0, 1);
    for(i = n - 1; i >= 0; i--)
    {
        player = EntityNameFromEnt(EntTeamInternalEx(team, i, NULL, 0, 1));
        if(!player) break;

        for (gurney = script->gurneys; gurney; gurney = gurney->next)
        {
            if(!gurney->active) continue;

            if (gurney->volume && EntityInVolume(player, gurney->volume))
            {
                // Update special gurneys which have a volume (takes precedence over sze wide gurneys)
                UpdateGurney(player, gurney->gurneyName);
                gurneyOverridden = true;
                break;
            }
            else if (!gurney->volume && EntityInVolume(player, script->eventVolume))
            {
                // Update special gurneys which have no volume (sze wide)
                UpdateGurney(player, gurney->gurneyName);
                gurneyOverridden = true;
                break;
            }
        }

        if(!gurneyOverridden) // No active special gurneys, so make sure to clear that off the player
            UpdateGurney(player, NULL);
    }
}

static void ScriptUpdateZEPlayerCounters(int forceUpdate)
{
    if (((g_currentKarmaTime+g_currentScript->id) % 2) || forceUpdate)//    every other server tick. trying to lighten the load a little bit
    {
        ScriptDefinition *scriptDef = GetCurrentScript();
        if (scriptDef)
        {
            ScriptStage *currentStage = scriptDef->currentStage;
            if (currentStage)
            {
                int j;
                for (j = eaSize(&currentStage->counters) - 1; j >= 0; j--)
                {
                    ScriptCounter *counter = currentStage->counters[j];
                    STRING activeTeam = counter->volumeName ? GetScriptVolumeTeam(counter->volumeName) : GetScriptTeam();
                    switch (counter->type)
                    {
                    case counter_ZEPlayers:
                        {
                            counter->progress = NumEntitiesInTeamEvenDead(activeTeam);
                        }break;
                    case counter_ZEPlayersAlive:
                        {
                            int i;
                            counter->progress = 0;
                            for (i = NumEntitiesInTeam(activeTeam); i > 0; --i )
                            {
                                Entity *e = EntTeamInternal(activeTeam, i-1, 0);
                                if (e && e->pchar && e->pchar->attrCur.fHitPoints > 0)
                                    counter->progress++;
                            }
                        }break;
                    case counter_ZEPlayersPhased:
                        {
                            int i;
                            counter->progress = 0;
                            for (i = NumEntitiesInTeam(activeTeam); i > 0; --i )
                            {
                                Entity *e = EntTeamInternal(activeTeam, i-1, 0);
                                if (e && e->pchar && !(e->pchar->bitfieldCombatPhases & 1))
                                    counter->progress++;
                            }
                        }break;
                    case counter_ZEPlayersActive:
                        {
                            int i;
                            counter->progress = 0;
                            for (i = NumEntitiesInTeamEvenDead(activeTeam); i > 0; --i )
                            {
                                Entity *e = EntTeamInternalEx(activeTeam, i-1, 0, 0, 1);
                                if (e && ScriptKarmaIsPlayerActive(e->db_id))
                                    counter->progress++;
                            }
                        }break;
                    }
                }
            }
        }
    }
}
#define SZE_UI_HIDDEN 1
#define SZE_UI_UNSET 0
#define SZE_UI_VISIBLE -1
#define TICK_COLLETION (1)
#define TICK_WIDGET (1<<1)
static void ScriptUpdateUICollections(int forceUpdate)
{
    static int collectionToken = 0;
    ScriptDefinition *scriptDef = GetCurrentScript();
    if (scriptDef)
    {
        ScriptStage *currentStage = scriptDef->currentStage;
        if (currentStage)
        {
            int j;
            const char *volumeTeam = GetScriptTeam();
            for (j = eaSize(&currentStage->UICollections) - 1; j >= 0; j--)
            {
                if ((((collectionToken&TICK_COLLETION)? 1 : 0) == (j%2)) || forceUpdate)
                {
                    ScriptZEUICollection *collection = currentStage->UICollections[j];
                    int i;
                    for (i = eaSize(&collection->items)-1; i >= 0; --i)
                    {
                        ScriptZEUIItem *item = collection->items[i];
                        if ((((collectionToken&TICK_WIDGET) ? 1 : 0) == (i%2)) || forceUpdate)
                        {
                            if (item->hideReq)
                            {
                                if (ScriptedZoneEvent_Eval(item->hideReq))
                                {
                                    if ((item->visibleLastTick == SZE_UI_UNSET) || (item->visibleLastTick == SZE_UI_VISIBLE))
                                    {
                                        ScriptUIWidgetHidden(StringAdd("UIWidget", NumberToString(item->widgetId)), 1);
                                        item->visibleLastTick = SZE_UI_HIDDEN;
                                    }
                                }
                                else
                                {
                                    if ((item->visibleLastTick == SZE_UI_UNSET) || (item->visibleLastTick == SZE_UI_HIDDEN))
                                    {
                                        ScriptUIWidgetHidden(StringAdd("UIWidget", NumberToString(item->widgetId)), 0);
                                        item->visibleLastTick = SZE_UI_VISIBLE;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    if (collectionToken >= 3)
        collectionToken=0;
    else
        collectionToken++;
}
static void HandleReset(ScriptStage *currentStage, char ***reset)
{
    int i;
    for (i = eaSize(reset) - 1; i >= 0; i--)
    {
        // Reset counters, timers and triggers.
        // This takes the most rational course of action in the event that the same name is used for two or
        // more of the object types we can reset here.  Namely it resets everything it thinks matches the
        // given names.  Therefore if you have a counter, a trigger and a timer that all share the same
        // name, if that name is given here, all three will be reset.
        // I'm not adding code to explicitly enforce namespace disambiguation, however there will be benefits
        // if design avoid name collisions.  Most notably it'll keep things far more readable.

        ScriptCounter *counter;
        ScriptTimer *timer;
        int j;

        if (counter = FindCounterByName(currentStage, (*reset)[i]))
        {
            InitCounter(counter);
            ScriptUpdateZEPlayerCounters(1);
        }

        if (timer = FindTimerByName(GetCurrentScript(), currentStage, (*reset)[i]))
        {
            InitTimer(currentStage, timer);
        }

        // Check all triggers and reset any that match
        for (j = eaSize(&currentStage->triggers) - 1; j >= 0; j--)
        {
            ScriptTrigger *trigger = currentStage->triggers[j];
            if (StringEqual(trigger->triggerName, (*reset)[i]))
            {
                trigger->fired = 0;
            }
        }

        // Check all triggerdecks and reset any that match - this doesn't reset the triggers within
        for (j = eaSize(&currentStage->triggerDecks) - 1; j >= 0; j--)
        {
            ScriptTriggerDeck *triggerDeck = currentStage->triggerDecks[j];
            if (StringEqual(triggerDeck->triggerDeckName, (*reset)[i]))
            {
                triggerDeck->fired = 0;
            }
        }
    }
}

void ScriptedZoneEvent_SetScriptValue(char **eval)
{
    ScriptDefinition *preserved = currentScript; // we may be called here from the powers system, which can identify our ScriptEnvironment but not our ScriptDefinition
    int n;

    currentScript = GetCurrentScript();

    n = eaSize(&eval);
    if (n >= 2)
    {
        eval_ClearStack(evalContext);
        eval_EvaluateEx(evalContext, &eval[1], eaSize(&eval) - 1);
        VarSetNumber(StringAdd("SymTab%", eval[0]), eval_IntPeek(evalContext));
    }

    currentScript = preserved;
}

void ScriptedZoneEvent_SetMapToken(char **eval)
{
    int n = eaSize(&eval);
    if (n >= 2)
    {
        char *val = eaRemove(&eval, 0);        //    remove the first parameter since it is not meant to be evaluated. it is the value to be set
        eval_ClearStack(evalContext);
        eval_Evaluate(evalContext, eval);
        eaInsert(&eval, val, 0);            //    reset the string back to its original form
        MapSetDataToken(val, eval_IntPeek(evalContext));
    }
}

void ScriptedZoneEvent_SetInstanceToken(char **eval)
{
    int n = eaSize(&eval);
    if (n >= 2)
    {
        char *val = eaRemove(&eval, 0);        //    remove the first parameter since it is not meant to be evaluated. it is the value to be set
        eval_ClearStack(evalContext);
        eval_Evaluate(evalContext, eval);
        eaInsert(&eval, val, 0);            //    reset the string back to its original form
        InstanceVarSet(val, eval_StringPeek(evalContext));
    }
}

static void HandleGroupHideOrShow(char ***groupNames, int shouldHide)
{
    int i;
    for (i = eaSize(groupNames) - 1; i >= 0; i--)
    {
        if (!groupdbHideForGameplay(groupDefFindWithLoad((*groupNames)[i]), shouldHide))
        {
            Errorf("Couldn't find group '%s' for hiding.", (*groupNames)[i]);
        }
    }
}

// Update timer variables.  timerName, if present, is used to filter which timer we should update.
static void ScriptedZoneEventUpdateTimers(char *timerName)
{
    int i;
    ScriptDefinition *script;
    ScriptStage *currentStage;
    ScriptTimer *timer;

    script = GetCurrentScript();
    currentStage = script->currentStage;
    for (i = eaSize(&currentStage->timers) - 1; i >= 0; i--)
    {
        timer = currentStage->timers[i];
        if (timer->timerName != NULL && (timerName == NULL || stricmp(timerName, timer->timerName) == 0))
        {
            VarSetNumber(timer->uiTimerVariable, timer->endTime + timer->bonusTime);
        }
    }
    for (i = eaSize(&script->timers) - 1; i >= 0; i--)
    {
        timer = script->timers[i];
        if (timer->timerName != NULL && (timerName == NULL || stricmp(timerName, timer->timerName) == 0))
        {
            VarSetNumber(timer->uiTimerVariable, timer->endTime + timer->bonusTime);
        }
    }
}

static void InitTimer(ScriptStage *stage, ScriptTimer *timer)
{
    NUMBER duration;
    NUMBER minDuration;
    NUMBER maxDuration;

    if (IsValidStr(timer->minDuration) && IsValidStr(timer->maxDuration))
    {
        minDuration = stringToDuration(timer->minDuration);
        maxDuration = stringToDuration(timer->maxDuration);
    }
    else
    {
        minDuration = 0;
        maxDuration = 0;
    }

    if (IsValidStr(timer->duration))
    {
        duration = stringToDuration(timer->duration);
    }
    else
    {
        duration = 0;
    }

    if (duration != 0)
    {
        timer->origDuration = duration;
    }
    else if (maxDuration != 0)
    {
        timer->origDuration = minDuration + randInt(maxDuration - minDuration + 1);
    }

    // I should probably complain if we reach here with origDuration containing something very silly, either <= 0 or >= a day (or so)
    timer->endTime = timerSecondsSince2000() + timer->origDuration;
    // Results are undefined but not fatal if two or more timers in a stage have their display flags set
    if (timer->displayFlag)
    {
        stage->displayTimer = timer;
    }
    timer->bonusTime = 0;

    if (timer->uiTimerVariable)
    {
        free(timer->uiTimerVariable);
    }
    timer->uiTimerVariable = strdup(StringAdd(timer->timerName, "Var"));
    ScriptedZoneEventUpdateTimers(timer->timerName);
}

static void ProcessTeamGroup(ScriptTeamGroup *teamGroup)
{
    int i;
    bool teamGroupExists = false;
    char **teamList = NULL;
    ScriptDefinition *script = GetCurrentScript();

    if(!script->teamGroup)
    {
        script->teamGroup = stashTableCreateWithStringKeys(20, StashDefault);
    }

    teamGroupExists = stashFindPointer(script->teamGroup, teamGroup->name, (void **)&teamList);

    if(teamGroup->removeTeams && !teamGroupExists)
        Errorf("Removing Team(s) from TeamGroup %s which doesn't exist", teamGroup->name);

    for(i = eaSize(&teamGroup->removeTeams) - 1; i >= 0; i--)
    {
        int result = StringArrayFind(teamList, teamGroup->removeTeams[i]);
        
        if (result != -1)
            eaRemove(&teamList,result);
        else
            Errorf("Removing Team %s from TeamGroup %s which doesn't contain it",
                    teamGroup->removeTeams[i], teamGroup->name);
    }

    if(teamGroup->addTeams && !teamGroupExists)
    {
        eaCreate(&teamList);
    }

    for(i = eaSize(&teamGroup->addTeams) - 1; i >= 0; i--)
    {
        if (StringArrayFind(teamList, teamGroup->addTeams[i]) == -1)
            eaPush(&teamList,teamGroup->addTeams[i]);
        else
            Errorf("Adding Team %s to TeamGroup %s which already contains it",
                    teamGroup->addTeams[i], teamGroup->name);
    }
    
    stashAddPointer(script->teamGroup, teamGroup->name, teamList, true);
}

static void ProcessTeamControlEx(ScriptTeamControl *teamControl, const char* teamName)
{
    ScriptTeamRespawn *team = NULL;
    ScriptSpawn *spawn = NULL;
    ScriptDefinition *script = GetCurrentScript();

    if (IsValidStr(teamName))
    {
        int teamMinCount = 0;
        if ((team = FindTeamByName(script,teamName)) != NULL)
        {
            if (teamControl->type == teamControl_Activate)
            {
                team->active = 1;
                team->tickCount = 0;
                team->spawnCount = 0;
            }
            else if (teamControl->type == teamControl_ForceSpawn)
            {
                // Reset the spawn count on a forcespawn.  Since this will later go through the same code path as the activate call, we
                // have to do this otherwise we'll wind up with team->spawnCount two, three or more times as large as it should be.
                team->spawnCount = 0;
            }
            else if (teamControl->type == teamControl_Deactivate)
            {
                team->active = 0;
            }
            else if (teamControl->type == teamControl_DoBehavior && team->active && IsValidStr(teamControl->behavior))
            {
                SetAIPriorityList(teamName, teamControl->behavior); 
            }
            else if (teamControl->type == teamControl_SetHealth && team->active)
            {
                int n = NumEntitiesInTeam(teamName);
                int i;

                for(i = 1; i <= n; i++)
                    SetHealth(GetEntityFromTeam(teamName, i), teamControl->health, 0);
            }
            else if (teamControl->type == teamControl_Detach)
            {
                DetachSpawn(teamName);
            }

            teamMinCount = (team->minimumCount || eaSize(&team->teamSizeCounts));
        
            for (spawn = script->spawns; spawn; spawn = spawn->next)
            {
                if (StringEqual(teamName, spawn->team))
                {
                    if (teamControl->type == teamControl_Activate || teamControl->type == teamControl_ForceSpawn)
                    {
                        if (!team->minRespawnTime || !teamMinCount)
                        {
                            if (teamControl->type == teamControl_ForceSpawn)
                            {
                                // If this is a ForceSpawn, we're respawning a team that's already active.  Detach the current members, so the
                                // ActivateSpawn(...) call doesn't blow them away when it inits the spawndefs.  Note that if this still does
                                // wacky stuff because there's too many members of the team (which I don't think it will), just duplicate the
                                // logic around the call to TriggerFountainWaves(...); below that temporarily moves the current members to
                                // a different team for the duration of the spawn action.  In this context the most likely meaning of "wacky stuff"
                                // is not spawning enough critters, although meltdown of nearby racks of servers in the datacenter cannot be eliminated.
                                DetachSpawn(teamName);
                            }
                            ActivateSpawn(spawn, team, teamMinCount);
                        }
                        team->spawnCount++;
                    }
                    if (teamControl->type == teamControl_Deactivate)
                    {
                        DeactivateSpawn(spawn);
                    }
                }
            }
            // Changed to teamMinCount because it makes this now match up under De Morgan's laws with the inverse test controlling
            // the call to ActivateSpawn(...) above.  This is just a readability change, it has no effect at all on the actual
            // operation of the code.  Note that De Morgan's only applies to the first two tests, the checks for teamControl->type
            // are identical: in that context the two blocks of code have the same control.
            if (team->minRespawnTime && teamMinCount && (teamControl->type == teamControl_Activate || teamControl->type == teamControl_ForceSpawn))
            {
                // If we're doing a forcespawn, we need to escrow all members of the team in a holding tank.  This will create the illusion that
                // the team is empty, so TriggerFountainWaves(...) will repopulate fully, based on the current player count.  We will then put them
                // back in the correct team after so that they can be controlled.
                if (teamControl->type == teamControl_ForceSpawn)
                {
                    SwitchScriptTeam(teamName, teamName, forceCreationTeam);
                }
                TriggerFountainWavesForTeam(script, true, team);
                if (teamControl->type == teamControl_ForceSpawn)
                {
                    SwitchScriptTeam(forceCreationTeam, forceCreationTeam, teamControl->team);
                }
            }
        }
        else
        {
            Errorf("TeamControl %s references non-existant team: %s",
                                teamControl->controlName ? teamControl->controlName : "unnamed",
                                teamName);
        }
    }
}

static void ProcessTeamControl(ScriptTeamControl *teamControl)
{
    if(IsValidStr(teamControl->team))
    {
        ProcessTeamControlEx(teamControl, teamControl->team);
    }
    if(IsValidStr(teamControl->teamGroup))
    {
        int i;
        char **teamList = NULL;
        ScriptDefinition *script = GetCurrentScript();

        if(stashFindPointer(script->teamGroup, teamControl->teamGroup, (void **)&teamList))
        {
            for(i = eaSize(&teamList) - 1; i >= 0; i--)
            {
                ProcessTeamControlEx(teamControl, teamList[i]);
            }
        }
    }
}

// Process an action block.  Since there's no type info in here, we do everything.  The only place there's a dependency is the
// glowie commands.  These are done here in a reasonable order, so doing something like:
// GlowiePlace myGlowie
// GlowieActivate myGlowie
// will do the correct thing.  Rule of least surprises and all that.  Keeping this in mind, it'll probably be wise to encourage
// design to keep it to one command per action block, and rely on the guaranteed order of execution of Action blocks themselves.
// TODO refactor this and extract the larger handling blocks as separate routines like HandleRewardGroup(...) above.
static void ProcessAction(ScriptAction *action, int respectDelay)
{
    int i;
    int n;
    int color;
    NUMBER delay;
    STRING msgTeam;
    TEAM rewardTeam;
    ENTITY rewardPlayer;
    LOCATION doorLoc;
    ScriptGlowie *glowie;
    ScriptSpawn *spawn;
    ScriptTeamRespawn *team;
    ScriptDoor *door;
    ScriptPort *port;
    ScriptGurney *gurney;
    ScriptGurneyControl *gurneyControl;
    ScriptSound *sound;
    ScriptFadeSound *soundFade;
    ScriptFloater *floater;
    ScriptCaption *caption;
    ScriptWaypoint *waypoint;
    ScriptWaypointPlace *placedWaypoint;
    ScriptEventTimer *eventTimer;
    ScriptDefinition *script;
    ScriptTimer *timer;
    ScriptTimerControl *timerControl;
    ScriptStage *currentStage;

    script = GetCurrentScript();
    currentStage = script->currentStage;

    if (respectDelay && IsValidStr(action->delay))
    {
        delay = stringToDuration(action->delay);
        if (delay != 0)
        {
            DelayedScriptAction *delayedAction = malloc(sizeof(DelayedScriptAction));
            delayedAction->delayActivationTime = SecondsSince2000() + delay;
            delayedAction->action = action;
            eaPush(&script->delayedActions, delayedAction);
            return;
        }
    }
    if (isDevelopmentMode() && IsValidStr(action->debugStr))
    {
        // Debug messages only in dev mode.
        // Huh - why are we textStd'ing this?
        chatSendDebug(textStd(action->debugStr));
    }
    if (isDevelopmentMode() && IsValidStr(action->slashCommand))
    {
        // Fire a slash command in dev mode.
        serverParseClientEx(action->slashCommand, NULL, 10);
    }
    if (IsValidStr(action->logStr))
    {
        // Log a custom log message to the sze_rewards log for CS purposes
        if(isDevelopmentMode())
            chatSendDebug(textStd("%s %s", "SZE Log:", action->logStr));

        TeamSZERewardLogMsg(GetScriptTeam(), VarGet("ZoneEventName"), action->logStr);
    }

    for (i = eaSize(&action->teamGroups) - 1; i >= 0; i--)
    {
        ProcessTeamGroup(action->teamGroups[i]);
    }

    if (IsValidStr(action->bling))
    {
        // Reward
        //chatSendDebug("Old style Reward Actions have been deprecated.  Please use the new RewardGroup.");
        // DGNOTE 3/3/2010 - reactivated.
        rewardTeam = GetScriptTeam();
        n = NumEntitiesInTeam(rewardTeam);
        for (i = 1; i <= n; i++)
        {
            rewardPlayer = GetEntityFromTeam(rewardTeam, i);
            EntityGrantReward(rewardPlayer, action->bling);
        }
    }
    if (IsValidStr(action->objective))
    {
        if (OnMissionMap())
        {
            SetMissionObjectiveComplete(OBJECTIVE_SUCCESS, action->objective);
        }
        else
        {
            SetInstanceVar(action->objective, NumberToString(ZONE_OBJECTIVE_SUCCESS));
        }
    }
    for (i = eaSize(&action->resetObjs) - 1; i >= 0; i--)
    {
        SetInstanceVar(action->resetObjs[i], NumberToString(ZONE_OBJECTIVE_UNSET));
    }
    HandleReset(script->currentStage, &action->reset);
    if (eaSize(&action->setScriptValue) >= 2)
    {
        ScriptedZoneEvent_SetScriptValue(action->setScriptValue);
    }
    if (eaSize(&action->setMapToken) >= 2)
    {
        ScriptedZoneEvent_SetMapToken(action->setMapToken);
    }
    if (IsValidStr(action->removeMapToken))
    {
        MapRemoveDataToken(action->removeMapToken);
    }
    if (eaSize(&action->setInstanceToken) >= 2)
    {
        ScriptedZoneEvent_SetInstanceToken(action->setInstanceToken);
    }
    if (IsValidStr(action->removeInstanceToken))
    {
        InstanceVarRemove(action->removeInstanceToken);
    }
    HandleGroupHideOrShow(&action->groupsToShow, 0);
    HandleGroupHideOrShow(&action->groupsToHide, 1);
    for (i = eaSize(&action->glowies) - 1; i >= 0; i--)
    {
        glowie = action->glowies[i];
        if (FindGlowieByName(script, glowie->glowieName) == NULL)
        {
            InitGlowie(glowie);
        }
    }

    // Iterate through the glowieControl list twice.  
    for (i = eaSize(&action->glowieControls) - 1; i >= 0; i--)
    {
        if ((glowie = FindGlowieByName(script, action->glowieControls[i]->glowieName)) != NULL)
        {
            // This looks a little counterintuitive.  I check for glowieControl_Place and glowieControl_Deactivate
            // in this first loop, and then glowieControl_Activate and glowieControl_Remove in the second one.
            // The reason for this is that there are only two pairs of operations that make sense: "Place and Activate" is one
            // "Deactivate and Remove" is the other.  Both these pairs need to be done in their respective orders.  The net
            // result of which is that I do the two "first"s in this loop and the two "second"s in the other.
            if (action->glowieControls[i]->type == glowieControl_Place)
            {
                PlaceGlowies(glowie);
            }
            if (action->glowieControls[i]->type == glowieControl_Deactivate)
            {
                DeactivateGlowies(glowie);
            }
        }
    }

    for (i = eaSize(&action->glowieControls) - 1; i >= 0; i--)
    {
        if ((glowie = FindGlowieByName(script, action->glowieControls[i]->glowieName)) != NULL)
        {
            if (action->glowieControls[i]->type == glowieControl_Remove)
            {
                RemoveGlowies(glowie);
            }
            // DGNOTE 1/15/2010
            // Added the mark command to the second loop, and move activate to the third.  This
            // makes the order of execution place, mark, activate
            if (action->glowieControls[i]->type == glowieControl_Mark)
            {
                MarkGlowie(glowie);
            }
        }
    }

    for (i = eaSize(&action->glowieControls) - 1; i >= 0; i--)
    {
        if ((glowie = FindGlowieByName(script, action->glowieControls[i]->glowieName)) != NULL)
        {
            // DGNOTE 12/1/2009
            // Added a third loop for clear memory.  This makes sense to happen after glowie removal
            // So we now need three loops for that: deactivate / remove / forget
            if (action->glowieControls[i]->type == glowieControl_ClearMemory)
            {
                memset(glowie->glowieUsed, 0, sizeof(U8) * glowie->numLoc);
            }
            // DGNOTE 1/15/2010
            // Moved activate to the third loop, since mark has been added to the second loop.
            if (action->glowieControls[i]->type == glowieControl_Activate)
            {
                ActivateGlowies(glowie);
            }
        }
    }

    for (i = eaSize(&action->spawns) - 1; i >= 0; i--)
    {
        spawn = action->spawns[i];
        if (FindSpawnByName(script, spawn->spawnName) == NULL)
        {
            InitSpawn(spawn);
        }
    }

    for (i = eaSize(&action->teams) - 1; i >= 0; i--)
    {
        team = action->teams[i];
        if (FindTeamByName(script, team->teamName) == NULL)
        {
            InitTeam(team);
        }
    }

    for (i = eaSize(&action->teamControls) - 1; i >= 0; i--)
    {
        ProcessTeamControl(action->teamControls[i]);
    }

    for (i = eaSize(&action->doors) - 1; i >= 0; i--)
    {
        door = action->doors[i];
        if (FindDoorByName(script, door->doorName) == NULL)
        {
            InitDoor(door);
        }
    }

    for (i = eaSize(&action->doorControls) - 1; i >= 0; i--)
    {
        if ((door = FindDoorByName(script, action->doorControls[i]->doorName)) != NULL)
        {
            if (action->doorControls[i]->type == doorControl_Unlock)
            {
                UnlockZoneDoor(door);
            }
            if (action->doorControls[i]->type == doorControl_Lock)
            {
                LockZoneDoor(door);
            }
        }
        else
        {
            doorLoc = StringAdd("marker:", action->doorControls[i]->doorName);
            if (action->doorControls[i]->type == doorControl_Unlock)
            {
                UnlockDoor(doorLoc);
            }
            if (action->doorControls[i]->type == doorControl_Lock)
            {
                LockDoor(doorLoc, "doorLocked");
            }
        }
    }

    for (i = eaSize(&action->ports) - 1; i >= 0; i--)
    {
        port = action->ports[i];
        if (FindPortByName(script, port->portName) == NULL)
        {
            InitPort(port);
        }
    }

    for (i = eaSize(&action->portControls) - 1; i >= 0; i--)
    {
        if ((port = FindPortByName(script, action->portControls[i]->portName)) != NULL)
        {
            if (action->portControls[i]->type == portControl_Activate)
            {
                ActivatePort(port);
            }
            if (action->portControls[i]->type == portControl_Deactivate)
            {
                DeactivatePort(port);
            }
        }
    }

    for (i = eaSize(&action->gurneys) - 1; i >= 0; i--)
    {
        gurney = action->gurneys[i];
        if (FindGurneyByName(script, gurney->gurneyName) == NULL)
        {
            InitGurney(gurney);
        }
    }

    for (i = eaSize(&action->gurneyControls) - 1; i >= 0; i--)
    {
        gurneyControl = action->gurneyControls[i];
        switch(gurneyControl->type)
        {
            xcase gurneyControl_EnableRezWindow:
                DisableGurneysOnThisMap(0);
            xcase gurneyControl_DisableRezWindow:
                DisableGurneysOnThisMap(1);
        }
        if ((gurney = FindGurneyByName(script, action->gurneyControls[i]->gurneyName)) != NULL)
        {
            if (action->gurneyControls[i]->type == gurneyControl_Activate)
            {
                ActivateGurney(gurney);
            }
            if (action->gurneyControls[i]->type == gurneyControl_Deactivate)
            {
                DeactivateGurney(gurney);
            }
        }
    }

    for (i = eaSize(&action->soundFades) - 1; i >= 0; i--)
    {
        soundFade = action->soundFades[i];
        if (soundFade->fadeTime >= 0.0f)
        {
            msgTeam = IsValidStr(soundFade->volumeName) ? GetScriptVolumeTeam(soundFade->volumeName) : GetScriptTeam();
            SendPlayerFadeSound(msgTeam, soundFade->channel, soundFade->fadeTime);
            if (soundFade->channel == SOUND_MUSIC)
            {
                if (script->currentBGMusicName)
                {
                    free(script->currentBGMusicName);
                    script->currentBGMusicName = 0;
                }
                script->currentBGMusicVolume = 0.0f;
            }
        }
    }

    for (i = eaSize(&action->sounds) - 1; i >= 0; i--)
    {
        sound = action->sounds[i];
        if (IsValidStr(sound->fileName))
        {
            msgTeam = IsValidStr(sound->volumeName) ? GetScriptVolumeTeam(sound->volumeName) : GetScriptTeam();
            SendPlayerSound(msgTeam, sound->fileName, sound->channel, sound->volumeLevel);
            if (sound->channel == SOUND_MUSIC && strstri(sound->fileName, "_loop"))
            {
                if (script->currentBGMusicName)
                {
                    free(script->currentBGMusicName);
                }
                script->currentBGMusicName = strdup(sound->fileName);
                script->currentBGMusicVolume = sound->volumeLevel;
            }
        }
    }

    n = eaSize(&action->floaters);
    // count up to process in order. This makes multiple floaters do the right thing
    for (i = 0; i < n; i++)
    {
        floater = action->floaters[i];
        if (IsValidStr(floater->message))
        {
            if (!IsValidStr(floater->color) || sscanf(floater->color, "%x", &color) != 1)
            {
                color = 0xffff00;
            }
            // color is 0xrrggbbaa.  Don't ask me why ...
            color = (color & 0xffffff) << 8 | 0x000000ff;
            msgTeam = IsValidStr(floater->volume) ? GetScriptVolumeTeam(floater->volume) : GetScriptTeam();
            SendPlayerAttentionMessageWithColor(msgTeam, floater->message, color, floater->alertType);
        }
    }

    n = eaSize(&action->captions);
    for (i = 0; i < n; i++)
    {
        caption = action->captions[i];
        if (IsValidStr(caption->message))
        {
            msgTeam = IsValidStr(caption->volume) ? GetScriptVolumeTeam(caption->volume) : GetScriptTeam();
            SendPlayerCaption(msgTeam, caption->message);
        }
    }

    //    Do all the minor rewards first so that the reset in the major rewards doesn't affect it
    for (i = eaSize(&action->rewardGroup) - 1; i >= 0; i--)
    {
        if (action->rewardGroup[i]->minorStageReward)
        {
            HandleRewardGroup(action->rewardGroup[i]->rewards, NULL, !action->rewardGroup[i]->minorStageReward, script->displayName);
        }
    }
    
    //    Do all the major rewards after so that the reset in the major rewards doesn't affect the minor
    for (i = eaSize(&action->rewardGroup) - 1; i >= 0; i--)
    {
        if (!action->rewardGroup[i]->minorStageReward)
        {
            int j;
            int numStages = VarGetNumber("TotalStages");
            int *stageThreshs = NULL;
            
            for (j = 0; j < numStages; ++j)
            {
                char buff[30];
                sprintf(buff, "%s%i", STAGETHRESH, j);
                eaiPush(&stageThreshs, VarGetNumber(buff));
            }

            HandleRewardGroup(action->rewardGroup[i]->rewards, stageThreshs, !action->rewardGroup[i]->minorStageReward, script->displayName);
            eaiDestroy(&stageThreshs);
            ScriptedZoneEventResetKarma();
        }
    }

    if (action->UIChange)
    {
        VarSetNumber("UiChange", 1);
    }
    if (action->resetReward)
    {
        ScriptedZoneEventResetKarma();
    }

    if (action->zeMinPlayerLevel)
    {
        SetScriptMinCombatLevel(action->zeMinPlayerLevel);
    }

    if (action->zeMaxPlayerLevel)
    {
        SetScriptMaxCombatLevel(action->zeMaxPlayerLevel);
    }

    for (i = eaSize(&action->waypoints) - 1; i >= 0; i--)
    {
        waypoint = action->waypoints[i];
        if (FindWaypointByName(script, waypoint->waypointName) == NULL)
        {
            InitWaypoint(waypoint);
        }
    }

    // Remove first
    if (action->removeWaypoints)
    {
        RemoveWaypoints();
    }

    // Place second.  That way, and action that both removes waypoints and places new ones will
    // do the right thing.
    for (i = eaSize(&action->placedWaypoints) - 1; i >= 0; i--)
    {
        placedWaypoint = action->placedWaypoints[i];
        PlaceWaypoint(placedWaypoint);
    }

    if (eaSize(&action->timers) && script && script->currentStage)
    {
        for (i = eaSize(&action->timers)-1; i >= 0; --i)
        {
            timer = FindTimerByName(0, currentStage, action->timers[i]->timerName);

            if (timer == NULL)
            {
                //    Add this timer to the stage timers
                timer = StructAllocRaw(sizeof(ScriptTimer));
                StructCopy(ParseTimer, action->timers[i], timer, 0, 0);
                timer->isActionTimer = 1;
                eaPush(&currentStage->timers, timer);
                InitTimer(currentStage, timer);
            } 
            else
            {
                if (timer->isActionTimer)
                {
                    InitTimer(currentStage, timer);
                }
                else
                {
                    Errorf("Current Stage already has a timer with the name %s.", action->timers[i]->timerName);
                }
            }
        }
    }

    if (eaSize(&action->eventTimers) && script && script->currentStage)
    {
        for (i = eaSize(&action->eventTimers)-1; i >= 0; --i)
        {
            timer = FindTimerByName(GetCurrentScript(), 0, action->eventTimers[i]->timerName);

            if (timer == NULL)
            {
                //    Add this timer to the event timers
                timer = StructAllocRaw(sizeof(ScriptTimer));
                StructCopy(ParseTimer, action->eventTimers[i], timer, 0, 0);
                timer->isActionTimer = 1;
                timer->isEventTimer = 1;
                eaPush(&GetCurrentScript()->timers, timer);
                InitTimer(currentStage, timer);
            } 
            else
            {
                if (timer->isActionTimer)
                {
                    InitTimer(currentStage, timer);
                }
                else
                {
                    Errorf("Event already has a timer with the name %s.", action->timers[i]->timerName);
                }
            }
        }        
    }

    for (i = eaSize(&action->timerControls) - 1; i >= 0; i--)
    {
        timerControl = action->timerControls[i];
        timer = FindTimerByName(GetCurrentScript(), currentStage, timerControl->timerName);

        if (timer)
        {
            switch (timerControl->type)
            {
            xcase timerControl_Add:
                timer->bonusTime += stringToDuration(timerControl->duration);
            xcase timerControl_Subtract:
                timer->bonusTime = max(timer->bonusTime - stringToDuration(timerControl->duration), 0);
            xcase timerControl_Reset:
                timer->bonusTime = 0;
            xcase timerControl_Delete:
                ; // nothing here
            }
            if (timer->uiTimerVariable)
            {
                VarSetNumber(timer->uiTimerVariable, timer->endTime + timer->bonusTime);
            }

            if (timerControl->type == timerControl_Delete)
            {
                ScriptDefinition *script = GetCurrentScript();

                for (i = eaSize(&currentStage->timers) - 1; i >= 0; i--)
                {
                    if (currentStage->timers[i] == timer)
                    {
                        StructDestroy(ParseTimer, timer);
                        eaRemove(&currentStage->timers, i);
                    }
                }
                for (i = eaSize(&script->timers) - 1; i >= 0; i--)
                {
                    if (script->timers[i] == timer)
                    {
                        StructDestroy(ParseTimer, timer);
                        eaRemove(&script->timers, i);
                    }
                }
            }
        }
    }

    if (eaSize(&action->eventTimer))
    {
        // Another case where we only process the first
        eventTimer = action->eventTimer[0];
        if (VarGetNumber("EventTimer") + stringToDuration(eventTimer->time) >= SecondsSince2000() && IsValidStr(eventTimer->badgestat))
        {
            GiveBadgeStat(GetScriptTeam(), eventTimer->badgestat, 1);
        }
    }
    if (action->eventTimerStart)
    {
        VarSetNumber("EventTimer", SecondsSince2000());
    }
    if (action->completeTrial != trialStatus_None)
    {
        SZE_TrialComplete(action->completeTrial);
    }
}

static void ProcessStartup(ScriptStartup *startup)
{
    int i;
    int n;

    n = eaSize(&startup->actions);
    // Count up to force order of execution to match order that Action blocks were specified
    for (i = 0; i < n; i++)
    {
        ProcessAction(startup->actions[i], 1);
    }
}

static void InitCounter(ScriptCounter *counter)
{
//    char *uiTextName;

    if (eaSize(&counter->evalTarget))
    {
        //    add eval statement here
        counter->target = ScriptedZoneEvent_Eval(counter->evalTarget);
    }

    // Init to zero for countup's and to current team size for count downs.  I should probably complain if I see a countdown Villaingroup
    // counter, since we don't have the a way to enumerate the number of live critters in a villainGroup.
    counter->progress = counter->type == counter_Team && counter->direction == counter_Down ?  NumEntitiesInTeam(counter->team) : 0;
}

static void InitUICollection(ScriptStage *stage, ScriptZEUICollection *UICollection)
{
    int i;
    int n;
    static int uniq = 0;
    ScriptZEUIItem *UIitem;
    ScriptDefinition *script;
    char **widgetNames = NULL;
    char uiScriptWidget[128];
    char uiStageWidget[128];
    const char *uiScriptTitle;
    const char *uiStageTitle;
    const char *widgetName;
    char *shortText;
    char *toolTip;

    script = GetCurrentScript();
    n = eaSize(&UICollection->items);
    if (!ScriptUICollectionExists(StringAdd("Collection:", UICollection->collName)))
    {
        uiScriptTitle = StringAdd(stage->stageName, "ScriptTitle");
        uiStageTitle = StringAdd(stage->stageName, "StageTitle");
        // I ought to toss a dev warning if either of script->displayName or stage->displayName are empty
        VarSet(uiScriptTitle, script->displayName);
        VarSet(uiStageTitle, stage->displayName);

        _snprintf(uiScriptWidget, sizeof(uiScriptWidget), "Script%d%s", ++uniq, stage->stageName);
        uiScriptWidget[sizeof(uiScriptWidget) - 1] = 0;
        ScriptUITitle(uiScriptWidget, uiScriptTitle, " ");

        _snprintf(uiStageWidget, sizeof(uiStageWidget), "Stage%d%s", ++uniq, stage->stageName);
        uiStageWidget[sizeof(uiStageWidget) - 1] = 0;

        toolTip = IsValidStr(stage->toolTip) ? stage->toolTip : "";
        if (stage->displayTimer)
        {
            if (stage->displayTimer->uiTimerVariable != NULL)
            {
                VarSetNumber(stage->displayTimer->uiTimerVariable, stringToDuration(stage->displayTimer->duration) >= 3600 ? 0 : SecondsSince2000() + 1800);
                // Only set this up if the strdup succeed.  I very much doubt it will fail, but if it does, at least the mapserver won't crash.
                ScriptUITimer(uiStageWidget, stage->displayTimer->uiTimerVariable, uiStageTitle, toolTip);
            }
            else
            {
                // Something failed with timer display setup.  Drop back 10, and punt: don't disply the timer.
                ScriptUIList(uiStageWidget, toolTip, uiStageTitle, "ffffffff", " ", "ffffffff");
            }
        }
        else
        {
            ScriptUIList(uiStageWidget, toolTip, uiStageTitle, "ffffffff", " ", "ffffffff");
        }
        eaPush(&widgetNames, uiScriptWidget);
        eaPush(&widgetNames, uiStageWidget);

        for (i = 0; i < n; i++)
        {
            UIitem = UICollection->items[i];
            
            UIitem->widgetId = ++uniq;
            widgetName = StringAdd("UIWidget", NumberToString(UIitem->widgetId));
            

            // This is based on a cap of 9,000 UI widgets.  I somehow don't think we'll ever hit that
            assertmsg(uniq <= 9000, "Fatal error.  Widget count over 9000");
            _snprintf(UIitem->currentVar, sizeof(UIitem->currentVar), "CurVar%d", uniq);
            UIitem->currentVar[sizeof(UIitem->currentVar) - 1] = 0;
            _snprintf(UIitem->targetVar, sizeof(UIitem->targetVar), "TgtVar%d", uniq);
            UIitem->targetVar[sizeof(UIitem->targetVar) - 1] = 0;
            _snprintf(UIitem->findVar, sizeof(UIitem->findVar), "FindVar%d", uniq);
            UIitem->findVar[sizeof(UIitem->findVar) - 1] = 0;

            shortText = IsValidStr(UIitem->shortText) ? UIitem->shortText : UIitem->text;

            switch (UIitem->type)
            {
            xcase itemType_BarBlue:
                ScriptUIMeterEx(widgetName, UIitem->text, shortText, UIitem->currentVar, UIitem->targetVar, "Blue", UIitem->tooltip ? UIitem->tooltip : "", UIitem->findVar);
            xcase itemType_BarGold:
                ScriptUIMeterEx(widgetName, UIitem->text, shortText, UIitem->currentVar, UIitem->targetVar, "Gold", UIitem->tooltip ? UIitem->tooltip : "", UIitem->findVar);
            xcase itemType_BarRed:
                ScriptUIMeterEx(widgetName, UIitem->text, shortText, UIitem->currentVar, UIitem->targetVar, "Red", UIitem->tooltip ? UIitem->tooltip : "", UIitem->findVar);

            xcase itemType_Text:
                ScriptUIText(widgetName, UIitem->text, " ", UIitem->tooltip ? UIitem->tooltip : "");
            xcase itemType_CenterText:
                ScriptUIText(widgetName, UIitem->text, "center", UIitem->tooltip ? UIitem->tooltip : "");

            xcase itemType_Timer:
                ScriptUITimer(widgetName, StringAdd(UIitem->timer, "Var"), UIitem->text, UIitem->tooltip ? UIitem->tooltip : "");
            }
            eaPushConst(&widgetNames, widgetName);
        }
        ScriptUICreateCollectionEx(UICollection->collName, i + 2, widgetNames);
    }
    for (i = 0; i < n; i++)
    {
        UIitem = UICollection->items[i];
        VarSetNumber(UIitem->currentVar, ScriptedZoneEvent_Eval(UIitem->current));
        VarSetNumber(UIitem->targetVar, ScriptedZoneEvent_Eval(UIitem->target));
        SetUpEntityTrackingForUI(UIitem->findVar, UIitem->findEntity);
    }
    if (stage->displayTimer && stage->displayTimer->uiTimerVariable)
    {
        VarSetNumber(stage->displayTimer->uiTimerVariable, stage->displayTimer->endTime + stage->displayTimer->bonusTime);
    }
    eaDestroy(&widgetNames);
}

static void InitTrigger(ScriptTrigger *trigger)
{
    trigger->fired = 0;
}

static void ShuffleTriggerDeck(ScriptTriggerDeck *triggerDeck)
{
    int i, j;
    triggerDeck->totalFired = 0;
    if(!triggerDeck->noShuffle) 
    {
        for (i = eaSize(&triggerDeck->triggers) - 1; i >= 1; i--)
        {
            j = RandomNumber(0, i);
            eaSwap(&triggerDeck->triggers, i, j);
        }
    }

    for (i = eaSize(&triggerDeck->triggers) - 1; i >= 0; i--)
    {
        InitTrigger(triggerDeck->triggers[i]);
    }
}

static void InitTriggerDeck(ScriptTriggerDeck *triggerDeck)
{
    triggerDeck->fired = 0;
    ShuffleTriggerDeck(triggerDeck);
}

static int ProcessTriggers(ScriptTrigger **triggers, int processLimit)
{
    int i, j, n, m, triggersFired = 0;
    ScriptTrigger *currentTrigger = NULL;
    ScriptDefinition *script;

    script = GetCurrentScript();

    n = eaSize(&triggers);
    for (i = 0; i < n; i++)
    {
        currentTrigger = triggers[i];

        // Triggers are oneshots, skip them if they have already fired
        if (!currentTrigger->fired && ScriptedZoneEvent_Eval(currentTrigger->required))
        {
            // Mark the trigger as fired.  Do this first so that a reset of this trigger in one of the action blocks will turn this into a repeatable trigger
            currentTrigger->fired = 1;

            m = eaSize(&currentTrigger->actions);
            // Count up to force order of execution to match order that Action blocks were specified
            for (j = 0; j < m; j++)
            {
                ProcessAction(currentTrigger->actions[j], 1);
            }
            if (currentTrigger->nextStage)
            {
                script->nextStage = currentTrigger->nextStage;
            }

            triggersFired++;
            if(processLimit > 0 && triggersFired >= processLimit)
                break;
        }
    }

    return triggersFired;
}

static int ProcessTriggerDecks(ScriptTriggerDeck **triggerDecks, int processLimit)
{
    int i, triggerDecksFired = 0;
    ScriptTriggerDeck *currentTriggerDeck = NULL;

    for (i = eaSize(&triggerDecks) - 1; i >= 0; i--)
    {
        currentTriggerDeck = triggerDecks[i];

        // Trigger Decks only fire once per trigger until reset
        if (!currentTriggerDeck->fired && ScriptedZoneEvent_Eval(currentTriggerDeck->required))
        {
            currentTriggerDeck->fired = 1;

            currentTriggerDeck->totalFired += ProcessTriggers(currentTriggerDeck->triggers, 1);

            // Now handle potential reshuffling of the deck
            switch(currentTriggerDeck->type)
            {
                xcase triggerDeck_ShuffleAlways:
                {
                    ShuffleTriggerDeck(currentTriggerDeck);
                }
                xcase triggerDeck_ShuffleAtEnd:
                {
                    if(currentTriggerDeck->totalFired >= eaSize(&currentTriggerDeck->triggers))
                        ShuffleTriggerDeck(currentTriggerDeck);
                }
                xcase triggerDeck_ShuffleAtCount:
                {
                    if(currentTriggerDeck->totalFired >= currentTriggerDeck->shuffleAt)
                        ShuffleTriggerDeck(currentTriggerDeck);
                }
            }

            triggerDecksFired++;
            if(processLimit > 0 && triggerDecksFired >= processLimit)
                break;
        }
    }

    return triggerDecksFired;
}

static void ProcessShutdown(ScriptShutdown *shutdown)
{
    int i;
    int n;

    n = eaSize(&shutdown->actions);
    // Count up to force order of execution to match order that Action blocks were specified
    for (i = 0; i < n; i++)
    {
        ProcessAction(shutdown->actions[i], 1);
    }
}

static void InitStage(ScriptStage *stage)
{
    int i;

    // Count up to force order of execution to match order that Startup blocks were specified
    for (i = 0; i <  eaSize(&stage->startup); i++)
    {
        ProcessStartup(stage->startup[i]);
    }
    for (i = eaSize(&stage->counters) - 1; i >= 0; i--)
    {
        InitCounter(stage->counters[i]);
    }
    ScriptUpdateZEPlayerCounters(1);
    for (i = eaSize(&stage->timers) - 1; i >= 0; i--)
    {
        if (stage->timers[i]->isActionTimer)
        {
            //    server had this lying around from a previous instance
            //    Remove it so it doesn't trip a required on accident
            StructDestroy(ParseTimer, stage->timers[i]);
            eaRemove(&stage->timers, i);
        }
        else
        {
            InitTimer(stage, stage->timers[i]);
        }
    }

    for (i = eaSize(&stage->eventTimers)-1; i >= 0; --i)
    {
        ScriptTimer *timer = FindTimerByName(GetCurrentScript(), 0, stage->eventTimers[i]->timerName);

        if (timer == NULL)
        {
            //    Add this timer to the event timers
            timer = StructAllocRaw(sizeof(ScriptTimer));
            StructCopy(ParseTimer, stage->eventTimers[i], timer, 0, 0);
            timer->isEventTimer = 1;
            eaPush(&GetCurrentScript()->timers, timer);
            InitTimer(stage, timer);
        } 
        else
        {
            if (timer->isActionTimer)
            {
                InitTimer(stage, timer);
            }
            else
            {
                Errorf("Event already has a timer with the name %s.", stage->eventTimers[i]->timerName);
            }
        }
    }

    for (i = eaSize(&stage->UICollections) - 1; i >= 0; i--)
    {
        InitUICollection(stage, stage->UICollections[i]);
    }

    ScriptUpdateUICollections(1);
    for (i = eaSize(&stage->triggers) - 1; i >= 0; i--)
    {
        InitTrigger(stage->triggers[i]);
    }
    for (i = eaSize(&stage->triggerDecks) - 1; i >= 0; i--)
    {
        InitTriggerDeck(stage->triggerDecks[i]);
    }

    ScriptedZoneEventUpdateTimers(NULL);

    for (i = NumEntitiesInTeam(ALL_PLAYERS_READYORNOT); i > 0; i--)
    {
        ENTITY player = GetEntityFromTeam(ALL_PLAYERS_READYORNOT, i);    
        ScriptedZoneEventOnEnterMap(player);        //    this will also handle volume entry if the uistate is volume
    }
}
static void ScriptHandleStageKarma(ScriptDefinition *script)
{
    assert(script && script->currentStage);
    if (script && script->currentStage)
    {
        if (IsKarmaEnabled())
        {
            char buff[30];
            int currentStage = VarGetNumber("TotalStages");
            sprintf(buff, "%s%i", STAGETHRESH, currentStage);
            VarSetNumber(buff, script->currentStage->karmaThreshold);
            VarSetNumber("TotalStages", currentStage + 1);

            ScriptKarmaAdvanceStage();
        }
    }
}

static void ShutdownStage(ScriptStage *stage)
{
    int i;
    int n;
    const char *collection;
    ENTITY player;

    n = eaSize(&stage->shutdown);
    // Count up to force order of execution to match order that Shutdown blocks were specified
    for (i = 0; i < n; i++)
    {
        ProcessShutdown(stage->shutdown[i]);
    }
    // Nuke all UIs
    for (i = NumEntitiesInTeam(ALL_PLAYERS_READYORNOT); i > 0; i--)
    {
        player = GetEntityFromTeam(ALL_PLAYERS_READYORNOT, i);
        collection = VarGet(EntVar(player, "CurrentUI"));
        if (IsValidStr(collection) && ScriptUICollectionExists(collection))
        {
            ScriptUIHide(collection, player);
            ScriptDBLog("ZoneEvent:UI", player, "Hiding UI %s shutdown stage", collection);
            ScriptClearKarmaContainer(player);
        }
        VarSet(EntVar(player, "CurrentUI"), NULL);
    }
}

void ScriptedZoneEvent(void)
{
    int startupfailed;
    int i;
    int j;
    int k;
    int n;
    int uiState;
    const char *baseName;
    ScriptDefinition *script;
    ScriptGlowie *glowie;
    ScriptPort *port;
    ScriptStage *stage;
    ScriptZEUICollection *UICollection = NULL;
    LOCATION loc;
    ENTITY player;
    const char *instanceName;
    const char *collection;
    char index[4];
    char fullName[MAX_PATH];
    extern EvalContext *s_pCombatEval;

    //////////////////////////////////////////////////////////////////////////
    INITIAL_STATE

        //////////////////////////////////////////////////////////////////////
        ON_ENTRY
        
            if (evalContext == NULL)
            {
                evalContext = eval_Create();
                assert(evalContext);

                eval_RegisterFunc(evalContext, "count>",                scripteval_Count,                1, 1);
                eval_RegisterFunc(evalContext, "target>",                scripteval_Target,                1, 1);
                eval_RegisterFunc(evalContext, "elapsed>",                scripteval_Count,                1, 1);
                eval_RegisterFunc(evalContext, "remaining>",            scripteval_Remaining,            1, 1);
                eval_RegisterFunc(evalContext, "complete>",                scripteval_Complete,            1, 1);
                eval_RegisterFunc(evalContext, "objectivesucceeded>",    scripteval_ObjectiveSucceeded,    1, 1);
                eval_RegisterFunc(evalContext, "scriptvalue>",            scripteval_EvalValue,            1, 1);
                eval_RegisterFunc(evalContext, "devMode?",                scripteval_DevMode,                0, 1);
                eval_RegisterFunc(evalContext, "eventLockedByGroup?",    scripteval_EventLockedByGroup,    0, 1);
                eval_RegisterFunc(evalContext, "system>",                chareval_SystemFetch,            1, 1);
                eval_RegisterFunc(evalContext, "turnstileMissionName>",    scripteval_TurnstileMissionName,1, 1);
            }

            script = malloc(sizeof(ScriptDefinition));
            baseName = VarGet("ZoneEventFile");

            startupfailed = 0;
            if (script == NULL || StringEmpty(baseName))
            {
                startupfailed = 1;
                chatSendDebug("Script Startup failed, script == NULL (out of memory) or baseName not found");
                SetCurrentScript(NULL);
                if (script)
                {
                    free(script);
                    script = NULL;
                }
            }
            else
            {
                strncpyt(fullName, baseName, MAX_PATH);
                strncpyt(&fullName[strlen(fullName)], ".zoneevent", (int)(MAX_PATH - strlen(fullName)));
                fullName[MAX_PATH - 1] = 0;

                memset(script, 0, sizeof(ScriptDefinition));
                if (ParserLoadFiles(SCRIPTS_DIR, fullName, NULL, 0, ParseScript, script, NULL, NULL, NULL))
                {
                    script->currentStage = NULL;
                    script->nextStage = script->firstStage;
                    SetCurrentScript(script);
                }
                else
                {
                    free(script);
                    startupfailed = 1;
                    chatSendDebug("Script Startup failed, parse error on .zoneevent file.  Check mapserver console for errors");
                    SetCurrentScript(NULL);
                    script = NULL;
                }
            }

            // I do this check here in the script startup.  That way, this will get done no matter how the script is started.
            // Run through the list of disabled events in servers.cfg and if we're one of them, commit suicide
            instanceName = VarGet("ZoneEventName");
            if (instanceName && script != NULL)
            {
                for (i = eaSize(&disabledScripts) - 1; i >= 0; i--)
                {
                    if (disabledScripts[i] && stricmp(disabledScripts[i], instanceName) == 0)
                    {
                        free(script);
                        startupfailed = 1;
                        chatSendDebug("Zone Event disabled");
                        SetCurrentScript(NULL);
                        script = NULL;
                        // If someone specifies this zone event twice, without this break we'd come through here twice,
                        // which would crash trying to free(NULL) on the second pass.
                        break;
                    }
                }
            }

            if (startupfailed)
            {
                SET_STATE("ScriptShutdown");
            }
            else
            {
                OnPlayerEnteringMap(ScriptedZoneEventOnEnterMap);
                OnPlayerExitingMap(ScriptedZoneEventOnExitMap);
                OnPlayerEntersVolume(ScriptedZoneEventOnEnterVolume);
                OnPlayerLeavesVolume(ScriptedZoneEventOnExitVolume);
                OnEntityDefeated(ScriptedZoneEventOnEntityDefeat);
                OnEntityCreated(ScriptedZoneEventOnEntityCreate);
                OnScriptMessage(ScriptedZoneEventOnMessageReceived);

                uiState = script == NULL || !IsValidStr(script->eventVolume) ? UISTATE_GLOBAL : UISTATE_BYVOLUME;
                VarSetNumber("uiState", uiState);
                VarSetNumber("UiChange", 0);

                InitVolumeTeams();
                ScriptedZoneEventResetKarma();
                SET_STATE("RunningScript");
            }
        END_ENTRY
        //////////////////////////////////////////////////////////////////////
        {
            // Nothing to do now, it was all handled in the ON_ENTRY block
        }

    //////////////////////////////////////////////////////////////////////////
    STATE("RunningScript") ///////////////////////////////////////////////////
    
        ON_ENTRY
        END_ENTRY

        script = GetCurrentScript();
        // Kinda a hack, but a few routines deeply nested need to know the current script.  Most notably requires evaluators.
        // Since it's a royal P.I.T.A. to pass this all the way down to evaluators, I punt the problem and use a global.
        currentScript = script;
        if (script)
        {
            // Stage transition first.
            if (script->nextStage != NULL)
            {
                ScriptTimer *uiTimer = NULL;

                if (script->currentStage)
                {
                    //    give out karma from old stage
                    //    instead of doing this when the next stage is triggered
                    //    we do this here (when the current stage ends) so that if we use a dev command to jump stages,
                    //    we still get the karma from the current stage
                    ScriptHandleStageKarma(script);
                    ShutdownStage(script->currentStage);
                    if (script->currentStage->displayTimer && script->currentStage->displayTimer->isEventTimer)
                        uiTimer = script->currentStage->displayTimer;
                }

                script->currentStage = FindStageByName(script, script->nextStage);
                if (script->currentStage == NULL)
                {
                    if (uiTimer)
                        script->currentStage->displayTimer = uiTimer;

                    // I probably should complain here, in the event that script->nextStage is not " ".  Setting nextStage
                    // to a string holding a single space is used to trip this code to shut down a zone event.
                    // Set script to NULL so we don't do anything else in here
                    if (strcmp(script->nextStage, " "))
                    {
                        chatSendDebug("Script shutting down in response to shutdown command: next stage == \" \"");
                    }
                    else
                    {
                        char str[4096];
                        _snprintf(str, 4096, "Script shutting down: next stage not found \"%s\"", script->nextStage);
                        str[4095] = 0;
                        chatSendDebug(str);
                    }
                    script = NULL;
                    SET_STATE("ScriptShutdown");
                }
                else
                {
                    if (script->currentStage->awardKarma)
                    {
                        if (!IsKarmaEnabled())        //    if we couldn't find it, add it now
                            ScriptEnableKarma();
                    }
                    else
                    {
                        ScriptDisableKarma();
                    }

                    // Remove any pending delayedActions
                    // This was apparently being relied upon to make certain scripts work, so we are not going to clear this now
                    // However, running actions not in the stage they were declared it is very risky behavior and ought to be removed in the data
                    //eaDestroy(&script->delayedActions);
                    //script->delayedActions = NULL;


                    InitStage(script->currentStage);
                    script->nextStage = NULL;
                }
            }
        }
        // If we survived the stage transition ...
        if (script)
        {        
            PERFINFO_AUTO_START("SZEActions", 1);

            ProcessTriggerDecks(script->currentStage->triggerDecks, 0);
            ProcessTriggers(script->currentStage->triggers, 0);

            PERFINFO_AUTO_STOP();
            PERFINFO_AUTO_START("SZECounterUpdate", 1);
            // Update UI counters
            for (j = eaSize(&script->currentStage->UICollections) - 1; j >= 0; j--)
            {
                ScriptZEUICollection *currentCollection = script->currentStage->UICollections[j];

                for (k = eaSize(&currentCollection->items) - 1; k >= 0; k--)
                {
                    ScriptZEUIItem *currentItem = currentCollection->items[k];
                    VarSetNumber(currentItem->currentVar, ScriptedZoneEvent_Eval(currentItem->current));
                    if (currentItem->updateTarget)
                        VarSetNumber(currentItem->targetVar, ScriptedZoneEvent_Eval(currentItem->target));
                    SetUpEntityTrackingForUI(currentItem->findVar, currentItem->findEntity);
                }
            }
            PERFINFO_AUTO_STOP();
            PERFINFO_AUTO_START("SZEActions", 1);
            
            for(i = eaSize(&script->delayedActions) - 1; i >= 0; i--) // backwards due to possible removals
            {
                DelayedScriptAction *delayedAction = script->delayedActions[i];

                // Is it time to activate this guy?
                if(SecondsSince2000() > delayedAction->delayActivationTime)
                {
                    // If so, process it, but tell ProcessAction(...) to ignore the fact that this is a delayed actio`
                    // This can never modify script->delayedActions or this loop will be incredibly wrong
                    ProcessAction(delayedAction->action, 0);

                    eaRemoveAndDestroy(&script->delayedActions, i, NULL);
                }
            }

            if (DoEvery("FirePorters", 5.0f / 60.0f, NULL)) // fire porters every five seconds
            {
                // Check for any teleport volumes that need to be fired
                for (port = script->ports; port; port = port->next)
                {
                    if (port->active)
                    {
                        TeleportToLocation(GetScriptVolumeTeam(port->portName), StringAdd("marker:", port->portName));
                    }
                }
            }
            PERFINFO_AUTO_STOP();
            PERFINFO_AUTO_START("SZEFountains", 1);
            if (DoEvery("FireSpawns", 1.0f / 60.0f, NULL)) // fire spawns every second
            {
                TriggerAllFountainWaves(script, false);

                // Check for any glowies that need to be reset
                for (glowie = script->glowies; glowie; glowie = glowie->next)
                {
                    for (j = 0; j < glowie->quantity; j++)
                    {
                        if (glowie->glowieTimers[j] && timerSecondsSince2000() > glowie->glowieTimers[j])
                        {
                            if (glowie->glowies[j] == NULL)
                            {
                                index[0] = '_';
                                index[1] = (j + 1) / 10 + '0';
                                index[2] = (j + 1) % 10 + '0';
                                index[3] = 0;
                                loc    = StringAdd(StringAdd("marker:", glowie->location), index);
                                glowie->glowies[j] = strdup(GlowiePlace(glowie->glowieDef, loc, false, GlowieInteract, GlowieComplete));
                            }
                            if (glowie->glowies[j] != NULL)
                            {
                                GlowieSetState(glowie->glowies[j]);
                                if (glowie->isMarked && glowie->placedWaypoint.mapWaypoints[j] > 0)
                                {
                                    SetWaypoint(GetScriptTeam(), glowie->placedWaypoint.mapWaypoints[j]);
                                }
                            }
                            glowie->glowieTimers[j] = 0;
                        }
                    }
                }
                // Check all players in UI timeout state.
                n = NumEntitiesInTeam("VolumeExited");
                for (j = 0; j < n; j++)
                {
                    player = GetEntityFromTeam("VolumeExited", j + 1);
                    if (TimerElapsed(EntVar(player, "UITimer")))
                    {
                        TimerRemove(EntVar(player, "UITimer"));

                        collection = VarGet(EntVar(player, "CurrentUI"));
                        if (IsValidStr(collection) && ScriptUICollectionExists(collection))
                        {
                            ScriptUIHide(collection, player);
                            ScriptDBLog("ZoneEvent:UI", player, "Hiding UI %s leave volume delayed", collection);
                            ScriptClearKarmaContainer(player);
                        }
                        VarSet(EntVar(player, "CurrentUI"), NULL);
                        SwitchScriptTeam(player, "VolumeExited", NULL);
                    }
                }
            }
            PERFINFO_AUTO_STOP();
            if (VarGetNumber("UiChange"))
            {
                stage = script->currentStage;
                n = eaSize(&stage->UICollections);
                ScriptedZoneEventUpdateTimers(NULL);
                uiState = VarGetNumber("uiState");
                if (uiState == UISTATE_GLOBAL)
                {
                    for (i = NumEntitiesInTeam(ALL_PLAYERS_READYORNOT); i > 0; i--)
                    {
                        player = GetEntityFromTeam(ALL_PLAYERS_READYORNOT, i);
                        collection = VarGet(EntVar(player, "CurrentUI"));
                        if (IsValidStr(collection))
                        {
                            ScriptUIHide(collection, player);
                            VarSet(EntVar(player, "CurrentUI"), NULL);
                            ScriptClearKarmaContainer(player);
                        }
                        ScriptDBLog("ZoneEvent:UI", player, "Hiding UI %s due to UI change", collection);
                        for (j = 0; j < n; j++)
                        {
                            UICollection = stage->UICollections[j];
                            collection = StringAdd("Collection:", UICollection->collName);
                            if (ScriptUICollectionExists(collection) && ScriptedZoneEvent_Eval(UICollection->required))
                            {
                                // Show the UI.
                                ScriptUIShow(collection, player);
                                // DGNOTE 7/21/2010
                                // And detach it.
                                ScriptUISendDetachCommand(player, 1);
                                VarSet(EntVar(player, "CurrentUI"), collection);
                                ScriptDBLog("ZoneEvent:UI", player, "Showing UI %s due to UI change", UICollection->collName);
                                ScriptSetKarmaContainer(player);
                                break;
                            }
                        }
                    }
                }
                if (uiState == UISTATE_BYVOLUME && IsValidStr(script->eventVolume))
                {
                    for (i = NumEntitiesInTeam(ALL_PLAYERS_READYORNOT); i > 0; i--)
                    {
                        player = GetEntityFromTeam(ALL_PLAYERS_READYORNOT, i);
                        collection = VarGet(EntVar(player, "CurrentUI"));
                        if (IsValidStr(collection))
                        {
                            ScriptUIHide(collection, player);
                            VarSet(EntVar(player, "CurrentUI"), NULL);
                            ScriptClearKarmaContainer(player);
                        }
                        ScriptDBLog("ZoneEvent:UI", player, "Hiding UI %s due to UI change", UICollection->collName);
                        for (j = 0; j < n; j++)
                        {
                            UICollection = stage->UICollections[j];
                            collection = StringAdd("Collection:", UICollection->collName);
                            if (ScriptUICollectionExists(collection) && ScriptedZoneEvent_Eval(UICollection->required) &&
                                EntityInArea(player, StringAdd("trigger:", script->eventVolume)))
                            {
                                // If they just popped out and back in
                                if (IsEntityOnScriptTeam(player, "VolumeExited"))
                                {
                                    // take them off the recent exit team
                                    SwitchScriptTeam(player, "VolumeExited", NULL);
                                    // and nuke this players exit timer
                                    TimerRemove(EntVar(player, "UITimer"));
                                }

                                // Show the UI.
                                ScriptUIShow(collection, player);
                                // DGNOTE 7/21/2010
                                // And detach it.
                                ScriptUISendDetachCommand(player, 1);
                                VarSet(EntVar(player, "CurrentUI"), collection);
                                ScriptDBLog("ZoneEvent:UI", player, "Showing UI %s due to UI change", UICollection->collName);
                                ScriptSetKarmaContainer(player);
                                break;
                            }
                        }
                    }
                }
                VarSetNumber("UiChange", 0);
            }
            if (IsKarmaEnabled())
            {
                PERFINFO_AUTO_START("SZEKarmaUpdates", 1);
                if (ScriptedZoneEvent_Eval(currentScript->currentStage->activeReqs))
                {
                    ScriptUpdateKarma(false);
                    if (!VarGetNumber("LastZEStatus"))
                    {
                        VarSetNumber("LastZEStatus", 1);
                        chatSendDebug("ZoneEventActive");
                    }
                }
                else
                {
                    ScriptUpdateKarma(true);
                    if (VarGetNumber("LastZEStatus"))
                    {
                        VarSetNumber("LastZEStatus", 0);
                        chatSendDebug("ZoneEventInactive");
                    }
                }
                PERFINFO_AUTO_STOP();
            }
            else
            {
                VarSetNumber("LastKarmaUpdate", 0);
            }
            PERFINFO_AUTO_START("SZEPlayerCounters", 1);
            ScriptUpdateZEPlayerCounters(0);
            ScriptUpdateUICollections(0);
            if (script->currentStage)
            {
                int j;
                for (j = eaSize(&script->currentStage->counters) - 1; j >= 0; j--)
                {
                    ScriptCounter *counter = script->currentStage->counters[j];
                    switch (counter->type)
                    {
                    case counter_BossHealth:
                        {
                            if (IsValidStr(counter->team))
                            {
                                FRACTION bossHp = GetHealth( counter->team );
                                counter->progress = ceil(bossHp*BOSS_MAX_HEALTH);
                            }
                        }
                        break;
                    case counter_Team:
                        {
                            if (counter->direction == counter_Down && IsValidStr(counter->team))
                            {
                                counter->progress = NumEntitiesInTeam(counter->team);
                            }
                        }
                        break;
                    }
                }
            }
            PERFINFO_AUTO_STOP();
        }
        currentScript = NULL;

    //////////////////////////////////////////////////////////////////////////
    STATE("ScriptShutdown") //////////////////////////////////////////////////
        ScriptedZoneEventShutdown();
    END_STATES

    ON_STOPSIGNAL
        SET_STATE("ScriptShutdown");
    END_SIGNAL
}

void ScriptedZoneEventInit()
{
    SetScriptName("ScriptedZoneEvent");
    SetScriptFunc(ScriptedZoneEvent);
    SetScriptType(SCRIPT_ZONE);

    SetScriptVar("ZoneEventFile", "", 0);
    SetScriptVar("ZoneEventName", "", 0);
}

void ScriptedMissionEventInit()
{
    SetScriptName("ScriptedMissionEvent");
    SetScriptFunc(ScriptedZoneEvent);
    SetScriptType(SCRIPT_MISSION);

    SetScriptVar("ZoneEventFile", "", 0);
    SetScriptVar("ZoneEventName", "", 0);
}

// Routines used elsewhere to control zone events
// Maybe some day I'll refactor this and place them in a separate source file.
int ScriptedZoneEventStartFile(char *file, int isMission)
{
    const ScriptDef *pScriptDef;
    char fullName[MAX_PATH];

    strncpyt(fullName, "scriptdefs\\", MAX_PATH);
    strncpyt(&fullName[strlen(fullName)], file, (int)(MAX_PATH - strlen(fullName)));
    strncpyt(&fullName[strlen(fullName)], ".scriptdef", (int)(MAX_PATH - strlen(fullName)));
    fullName[MAX_PATH - 1] = 0;

    if (pScriptDef = ScriptDefFromFilename(fullName))
    {
        return ScriptBeginDef(pScriptDef, isMission ? SCRIPT_MISSION : SCRIPT_ZONE, NULL, "ZoneScript start", fullName);
    }
    return 0;
}

void ScriptedZoneEventStopEvent(char *event)
{
    char string[1024];

    strncpyt(string, event, 1000);
    strncpyt(&string[strlen(string)], ".stop", (int)(1024 - strlen(string)));

    ScriptSendScriptMessage(string);
}

void ScriptedZoneEventGotoStage(char *event, char *stage)
{
    char string[1024];

    strncpyt(string, event, 512);
    strncpyt(&string[strlen(string)], ".goto.", (int)(1024 - strlen(string)));
    strncpyt(&string[strlen(string)], stage, (int)(1024 - strlen(string)));

    ScriptSendScriptMessage(string);
}

void ScriptedZoneEventKillDebug(int enable)
{
    killDebugFlag = enable;
}

void ScriptedZoneEventDisableEvent(char *eventName)
{
    char *localName;

    if ((localName = strdup(eventName)) != NULL)
    {
        eaPush(&disabledScripts, localName);
    }
}

void ScriptGotoMarker(Entity *e, char *marker)
{
    Vec3 pos;
    char markerEX[4096];

    strcpy(markerEX, "marker:");
    strcat(markerEX, marker);
    if (GetPointFromLocation(markerEX, pos))
    {
        pos[1] += 10;
        entUpdatePosInstantaneous(e, pos);
    }
}
void ScriptedZoneEventEval_StoreAttribInfo(float fMagnitude, float fDuration)
{
    eval_StoreFloat(evalContext, "Magnitude", fMagnitude);
    eval_StoreFloat(evalContext, "Duration", fDuration);
}

void ScriptedZoneEvent_SetCurrentBGMusic(const char *musicName, float volumeLevel)
{
    ScriptDefinition *scriptDef = GetCurrentScriptDontAssert();
    if (scriptDef)
    {
        if (scriptDef->currentBGMusicName)
        {
            free(scriptDef->currentBGMusicName);
        }
        scriptDef->currentBGMusicName = strdup(musicName);
        scriptDef->currentBGMusicVolume = volumeLevel;
    }
}

#include "AutoGen/ScriptedZoneEvent_c_ast.c"
