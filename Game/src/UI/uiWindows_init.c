
#include "gameComm/wdwbase.h"
#include "uiBaseStorage.h"
#include "uiInvent.h"
#include "uiRecipeInv.h"
#include "uiSalvage.h"
#include "UI/uiStoredSalvage.h"
#include "uiInventory.h"
#include "entity/entity.h"
#include "entity/entPlayer.h"
#include "player/player.h"
#include "win/win_init.h"
#include "entity/character_base.h"
#include "arena/arenagame.h"
#include "cmdparse/cmdgame.h"

#include "UI/uiWindows_init.h"
#include "UI/uiWindows.h"
#include "UI/uiStatus.h"
#include "UI/uiDock.h"
#include "UI/uiPowerInventory.h"
#include "UI/uiTarget.h"
#include "UI/uiChat.h"
#include "UI/uiCompass.h"
#include "UI/uiAutomap.h"
#include "UI/uiTray.h"
#include "UI/uiGroupWindow.h" 
#include "UI/uiFriend.h"
#include "UI/uiContactDialog.h"
#include "UI/uiInspiration.h"
#include "UI/uiSupergroup.h"
#include "UI/uiEmail.h"
#include "UI/uiContact.h"
#include "UI/uiMission.h"
#include "UI/uiClue.h"
#include "UI/uiTrade.h"
#include "UI/uiQuit.h" 
#include "UI/uiInfo.h"
#include "UI/uiUtil.h"
#include "uiHelp.h"
#include "UI/uiMissionSummary.h"
#include "UI/uiBrowser.h"
#include "UI/uiLfg.h"
#include "uiStore.h"
#include "UI/uiComment.h"
#include "UI/uiPetition.h"
#include "UI/uiTitleSelect.h"
#include "uiDeath.h"
#include "UI/uiMapSelect.h"
#include "UI/uiCostumeSelect.h"
#include "UI/uiEnhancement.h"
#include "UI/uiBadges.h"
#include "uiRewardChoice.h"
#include <utilitieslib/utils/utils.h>
#include "uiChatOptions.h"
#include "UI/uiChatUtil.h"
#include "UI/uiChannel.h"
#include "UI/uiArena.h"
#include "UI/uiArenaList.h"
#include "UI/uiArenaResult.h"
#include "UI/uiArenaJoin.h"
#include "uiRenderStats.h"
#include "UI/uiBaseProps.h"
#include "UI/uiBaseInventory.h"
#include "UI/uiBaseRoom.h"
#include "uiConceptInv.h"
#include "UI/uiSGList.h"
#include "UI/uiSGRaidList.h"
#include "UI/uiPet.h"
#include "UI/uiOptions.h"
#include "uiArenaGladiator.h"
#include "UI/uiRecipeInventory.h"
#include "UI/uiChannelSearch.h"
#include "uiBaseLog.h"
#include "UI/uiPlaque.h"
#include "UI/uiRaidResult.h"
#include "uiAmountSlider.h"
#include "UI/uiCombatNumbers.h"
#include "UI/uiCombatMonitor.h"
#include "uiTrialReminder.h"
#include "UI/uiTraySingle.h"
#include "uiColorPicker.h"
#include "UI/uiPlayerNote.h"
#include "UI/uiRecentTeam.h"
#include "UI/uiGame.h"
#include "UI/uiCustomWindow.h"
#include "UI/uiMissionMaker.h"
#include "UI/uiMissionSearch.h"
#include "UI/uiMissionReview.h"
#include "UI/uiCustomVillainGroupWindow.h"
#include "uiMissionComment.h"
#include "UI/uiIncarnate.h"
#include "UI/uiPopHelp.h"
#include "UI/uiScript.h"
#include "UI/uiAuction.h"
#include "UI/uiTurnstile.h"
#include "UI/uiContactFinder.h"
#include "UI/Hybrid/uiLoyaltyTree.h"
#include "UI/Hybrid/uiWebStoreFrame.h"

#include "UI/Hybrid/uiLWC.h"
#include "UI/Hybrid/uiLoyaltyTreeAccess.h"
#include "uiSalvageOpen.h"
#include "UI/uiConvertEnhancement.h"
#include "uiNewFeatures.h"

#include "editorUI.h"
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/file.h>
#include <utilitieslib/utils/sysutil.h>
#include <utilitieslib/language/MessageStoreUtil.h>
#include "uiLevelingpact.h"
#include "render/renderstats.h"

static int windows_initialized = FALSE;

typedef struct ChildDefault
{
    int        child; 
    char    name[128]; 
    char    command[128];
}ChildDefault;

typedef struct ResizeLimit
{
    int type;
    int min_ht;
    int min_wd;
    int max_ht;
    int max_wd;
}ResizeLimit;

typedef struct WindowDefault
{ 
    int id;

    // These represent both the anchor point within the window
    //   AND the point within the screen that the window is anchored.
    WindowHorizAnchor horizAnchor;
    WindowVertAnchor vertAnchor;

    float xoff, yoff, wd, ht;
    int (*code)();

    int default_state;
    int radius;
    int save;

    ChildDefault children[WINDOW_MAX_CHILDREN];
    ResizeLimit limits;
    int noFrame;
    int maximized;

}WindowDefault;

WindowDefault window_defaults[] = 
{
    { WDW_DOCK,                        kWdwAnchor_Left,    kWdwAnchor_Top, 0, 0, 0, 0, dock_draw, ALWAYS_CLOSED, 0, 0, }, 
    { WDW_STAT_BARS,                kWdwAnchor_Right,    kWdwAnchor_Top, 0, 0, 330, 65, statusWindow, DEFAULT_OPEN, 0, 1, 
        {
            {0, "ChatTab",   "chat"  }, 
            {0, "TrayTab",   "tray"  }, 
            {0, "TargetTab", "target"},
            {0, "NavTab",    "nav"   }, 
            {0, "MenuTab",   "menu"  }
        }, 
    },
    { WDW_TARGET,                    kWdwAnchor_Left,    kWdwAnchor_Top, 0, 0, 236, 66, targetWindow, DEFAULT_OPEN, R22, 1,
        {{WDW_TARGET_OPTIONS, "ActionTab",}}, },
    { WDW_TRAY,                        kWdwAnchor_Right,    kWdwAnchor_Bottom, 0, 0, 440, 0, trayWindow, DEFAULT_OPEN, R22, 1,
        {
            {WDW_POWERLIST,        "PowersTab",                                    },
            {WDW_INSPIRATION,    "InspirationTab",                                }, 
            {WDW_ENHANCEMENT,    "ManageTab"                                        }, 
            {0,                    "UiInventoryTypeSalvage",    "toggle salvage",    }, 
            {0,                    "RecipesTab",                "toggle recipe"        },
            {0,                    "+",                        "showNewTray"        },
        },
    },
    { WDW_TRAY_1,                    kWdwAnchor_Left,    kWdwAnchor_Top, 0, 100, 450, 50, trayWindowSingle, DEFAULT_CLOSED, R22, 1, {{0}}, { TRAY_FIXED_SIZES, 50, 50, 450, 450 }, 1},
    { WDW_TRAY_2,                    kWdwAnchor_Left,    kWdwAnchor_Top, 50, 200, 450, 50, trayWindowSingle, DEFAULT_CLOSED, R22, 1, {{0}}, { TRAY_FIXED_SIZES, 50, 50, 450, 450 }, 1},
    { WDW_TRAY_3,                    kWdwAnchor_Left,    kWdwAnchor_Top, 100, 300, 450, 50, trayWindowSingle, DEFAULT_CLOSED, R22, 1, {{0}}, { TRAY_FIXED_SIZES, 50, 50, 450, 450 }, 1},
    { WDW_TRAY_4,                    kWdwAnchor_Left,    kWdwAnchor_Top, 150, 400, 450, 50, trayWindowSingle, DEFAULT_CLOSED, R22, 1, {{0}}, { TRAY_FIXED_SIZES, 50, 50, 450, 450 }, 1},
    { WDW_TRAY_5,                    kWdwAnchor_Left,    kWdwAnchor_Top, 200, 500, 450, 50, trayWindowSingle, DEFAULT_CLOSED, R22, 1, {{0}}, { TRAY_FIXED_SIZES, 50, 50, 450, 450 }, 1},
    { WDW_TRAY_6,                    kWdwAnchor_Left,    kWdwAnchor_Top, 250, 600, 450, 50, trayWindowSingle, DEFAULT_CLOSED, R22, 1, {{0}}, { TRAY_FIXED_SIZES, 50, 50, 450, 450 }, 1},
    { WDW_TRAY_7,                    kWdwAnchor_Left,    kWdwAnchor_Top, 300, 700, 450, 50, trayWindowSingle, DEFAULT_CLOSED, R22, 1, {{0}}, { TRAY_FIXED_SIZES, 50, 50, 450, 450 }, 1},
    { WDW_TRAY_8,                    kWdwAnchor_Left,    kWdwAnchor_Top, 350, 800, 450, 50, trayWindowSingle, DEFAULT_CLOSED, R22, 1, {{0}}, { TRAY_FIXED_SIZES, 50, 50, 450, 450 }, 1},
    { WDW_COMPASS,                    kWdwAnchor_Center,    kWdwAnchor_Top, 0, 0, 280, 60, compassWindow, DEFAULT_OPEN, R22, 1,
        {
            {WDW_MAP,     "MapTab"        },
            {WDW_CONTACT, "ContactTab" },
            {WDW_MISSION, "MissionTab" },
            {WDW_CLUE,    "ClueTab"    },
            {WDW_BADGES,  "BadgeTab"    }
        }, 
        {{0}}, 0 
    },
    { WDW_CHAT_BOX,                    kWdwAnchor_Left,    kWdwAnchor_Bottom, 0, 0, 360, 200, chatWindow, DEFAULT_OPEN, R10, 1,
        {
            {WDW_GROUP,        "TeamTab"}, 
            {WDW_LEAGUE,       "LeagueTab"}, 
            {WDW_FRIENDS,      "FriendsTab"}, 
            {WDW_SUPERGROUP,   "SupergroupTab"}, 
            {WDW_EMAIL,        "EmailTab"}, 
            {WDW_TURNSTILE,    "LFGTab"},
            {0,                "TabOptions", "chatoptions 0"},
            {WDW_CHAT_CHILD_1, "1"}, 
            {WDW_CHAT_CHILD_2, "2"}, 
            {WDW_CHAT_CHILD_3, "3"},
            {WDW_CHAT_CHILD_4, "4"}
        }, 
        { RESIZABLE,  90, 420, 1600, 1800 }, 0
    },
    { WDW_CHAT_CHILD_1,                kWdwAnchor_Left,    kWdwAnchor_Bottom, 200,  0, 360, 200, chatWindow,           DEFAULT_CLOSED, R10, 1,
        {{0, "TabOptions", "chatoptions 1"}}, { RESIZABLE,  70, 290, 1600, 1800 },  },
    { WDW_CHAT_CHILD_2,                kWdwAnchor_Left,    kWdwAnchor_Bottom, 210,  0, 360, 200, chatWindow,           DEFAULT_CLOSED, R10, 1,
        {{0, "TabOptions", "chatoptions 2"}}, { RESIZABLE,  70, 290, 1600, 1800 }},
    { WDW_CHAT_CHILD_3,                kWdwAnchor_Left,    kWdwAnchor_Bottom, 220,  0, 360, 200, chatWindow,           DEFAULT_CLOSED, R10, 1,
        {{0, "TabOptions", "chatoptions 3"}}, { RESIZABLE,  70, 290, 1600, 1800 }, },
    { WDW_CHAT_CHILD_4,                kWdwAnchor_Left,    kWdwAnchor_Bottom, 230,  0, 360, 200, chatWindow,           DEFAULT_CLOSED, R10, 1,
        {{0, "TabOptions", "chatoptions 4"}}, { RESIZABLE,  70, 290, 1600, 1800 },},
    { WDW_GROUP,                    kWdwAnchor_Left,    kWdwAnchor_Top, 0,    0,    245, 34,  groupWindow,          DEFAULT_CLOSED, R10, 1,
        {{0, "PetString", "toggle pet"}}, },
    { WDW_LEAGUE,                    kWdwAnchor_Left,    kWdwAnchor_Top, 0,    0,    245, 34,  groupWindow,          DEFAULT_CLOSED, R10, 1,
            {{0}}, },

    { WDW_ENHANCEMENT,                kWdwAnchor_Left,    kWdwAnchor_Top, 0,    0,    440, 73,  enhancementWindow,    DEFAULT_CLOSED, R10, 1,
        {    
            {0, "ManageForRealTab", "Manage" }, 
            {WDW_CONVERT_ENHANCEMENT, "ConvertEnhancementString" },
//            {0, "Recipes2Tab", "toggle recipe" }
        }, 
    },
    { WDW_PET,                        kWdwAnchor_Left,    kWdwAnchor_Top, 0,    300,  250, 300, petWindow,            DEFAULT_CLOSED, R10, 1 },
    { WDW_POWERLIST,                kWdwAnchor_Left,    kWdwAnchor_Top, 500,  160,  440, 227, powersWindow,         DEFAULT_CLOSED, R10, 1,
        {
            {0, "CombatNumbersString",        "toggle combatnumbers"    },         
            {0, "IncarnateAbilitiesString",    "toggle incarnate"        },
        },
    },
    { WDW_CONTACT_DIALOG,            kWdwAnchor_Center,    kWdwAnchor_Middle, 0, 0,    600, 400, contactWindow,        ALWAYS_CLOSED,  R10, 0 },
    { WDW_INSPIRATION,                kWdwAnchor_Right,    kWdwAnchor_Bottom, 0, -200, 0,   52,  inspirationWindow,    DEFAULT_OPEN,   R10, 1, {{0}}, { NOT_RESIZABLE, 52, 88, 148, 196 }, },
    { WDW_MISSION,                    kWdwAnchor_Left,    kWdwAnchor_Top, 0,    0,    440,  35, missionWindow,        DEFAULT_CLOSED, R10, 1 },
    { WDW_QUIT,                        kWdwAnchor_Center,    kWdwAnchor_Middle, 0,  0,   200, 115, quitWindow,           ALWAYS_CLOSED,  R10, 0 },
    { WDW_HELP,                        kWdwAnchor_Center,    kWdwAnchor_Middle, 0,  0,   600, 300, helpWindow,           ALWAYS_CLOSED,  R10, 0 },
    { WDW_MISSION_SUMMARY,            kWdwAnchor_Center,    kWdwAnchor_Middle, 0,  0,   400, 300, missionSummaryWindow, ALWAYS_CLOSED,  R10, 0 },
    { WDW_TARGET_OPTIONS,            kWdwAnchor_Left,    kWdwAnchor_Top, 0,    0,    180, 66,  targetOptionsWindow,  DEFAULT_CLOSED, R10, 1 },
    { WDW_STORE,                    kWdwAnchor_Center,    kWdwAnchor_Middle, 0,  0,   630, 480, storeWindow,          ALWAYS_CLOSED,  R10, 0 },
    { WDW_DIALOG,                    kWdwAnchor_Left,    kWdwAnchor_Top, 0,    0,    0,   0,   dock_draw,            ALWAYS_CLOSED,  R10, 0 }, 
    { WDW_BETA_COMMENT,                kWdwAnchor_Center,    kWdwAnchor_Middle, 0,  0,   450, 250, commentWindow,        ALWAYS_CLOSED,  R10, 0 },
    { WDW_PETITION,                    kWdwAnchor_Center,    kWdwAnchor_Middle, 0,  0,   400, 320, petitionWindow,       ALWAYS_CLOSED,  R10, 0 },
    { WDW_TITLE_SELECT,                kWdwAnchor_Center,    kWdwAnchor_Middle, 0,  0,   415, 530, titleselectWindow,    ALWAYS_CLOSED,  R10, 0 },
    { WDW_DEATH,                    kWdwAnchor_Center,    kWdwAnchor_Middle, 0,  0,   280, 100, deathWindow,          ALWAYS_CLOSED,  R10, 1 },
    { WDW_COSTUME_SELECT,            kWdwAnchor_Center,    kWdwAnchor_Middle, 0,  0,   707, 276, costumeSelectWindow,  ALWAYS_CLOSED,  R10, 1 },
    { WDW_REWARD_CHOICE,            kWdwAnchor_Center,    kWdwAnchor_Middle, 0,  0,   570, 276, rewardChoiceWindow,   ALWAYS_CLOSED,  R10, 0 },
    { WDW_ARENA_LIST,                kWdwAnchor_Center,    kWdwAnchor_Middle, 0,  0,   630, 480, arenaListWindow,      ALWAYS_CLOSED,  R10, 0 },
    { WDW_ARENA_RESULT,                kWdwAnchor_Center,    kWdwAnchor_Middle, 0,  0,   630, 480, arenaResultWindow,    ALWAYS_CLOSED,  R10, 0 },
    { WDW_ARENA_JOIN,                kWdwAnchor_Center,    kWdwAnchor_Middle, 0,  0,   280, 100, arenaJoinWindow,      ALWAYS_CLOSED,  R10, 0 },
#if RDRSTATS_ON
    { WDW_RENDER_STATS,                kWdwAnchor_Left,    kWdwAnchor_Top, 250,  0,    330, 186, rdrStatsWindow,       ALWAYS_CLOSED,  R10, 0 },
#endif
    { WDW_CHAT_OPTIONS,                kWdwAnchor_Left,    kWdwAnchor_Top, 200,  200,  570, 380, chatTabWindow,        ALWAYS_CLOSED,  R10, 1, {{0}}, { RESIZABLE, 380, 500, 500, 750 }, },
    { WDW_MAP,                        kWdwAnchor_Left,    kWdwAnchor_Top, 0,    0,    384, 384, mapWindow,            DEFAULT_CLOSED, R10, 1, {{0}}, { RESIZABLE, 192, 256, 1600, 1800 }, },
    { WDW_FRIENDS,                    kWdwAnchor_Left,    kWdwAnchor_Top, 0,    0,    270, 260, friendWindow,         DEFAULT_CLOSED, R10, 1, {{0}}, { RESIZABLE, 100, 134, 1600, 1800 }, },
    { WDW_SUPERGROUP,                kWdwAnchor_Left,    kWdwAnchor_Top, 0,    0,    440, 300, supergroupWindow,     DEFAULT_CLOSED, R10, 1, {{0}}, { RESIZABLE, 200, 340, 1600, 1800 }, },
    { WDW_EMAIL,                    kWdwAnchor_Left,    kWdwAnchor_Top, 0,    0,    440, 320, emailWindow,          ALWAYS_CLOSED,  R10, 1, {
            {0, "MailString", "mailview mail"},
            {0, "CertificationString", "mailview certification"},
            {0, "VoucherString", "mailview voucher"}}, { RESIZABLE, 320, 320, 1600, 1800 }, }, 
    { WDW_EMAIL_COMPOSE,            kWdwAnchor_Left,    kWdwAnchor_Top, 300,  300,  440, 200, emailComposeWindow,   ALWAYS_CLOSED,  R10, 1, {{0}}, { RESIZABLE, 200, 400, 1600, 1800 }, }, 
    { WDW_CONTACT,                    kWdwAnchor_Left,    kWdwAnchor_Top, 0,    0,    400, 318, contactsWindow,       DEFAULT_CLOSED, R10, 1, {{0}}, { VERTONLY,  250, 400, 1600, 400 }, },
    { WDW_CLUE,                        kWdwAnchor_Left,    kWdwAnchor_Top, 0,    0,    400, 200, clueWindow,           DEFAULT_CLOSED, R10, 1, {{0}}, { VERTONLY,  200, 400, 1600, 400 }, },
    { WDW_TRADE,                    kWdwAnchor_Center,    kWdwAnchor_Middle, 0,  0,   630, 480, tradeWindow,          ALWAYS_CLOSED,  R10, 0, {{0}}, { VERTONLY,  270, 630, 1600, 630 }, },  
    { WDW_INFO,                        kWdwAnchor_Left,    kWdwAnchor_Top, 300,  300,  400, 200, infoWindow,           DEFAULT_CLOSED, R10, 1, {{0}}, { RESIZABLE, 300, 400, 1600, 1800 }, }, 
    { WDW_BROWSER,                    kWdwAnchor_Center,    kWdwAnchor_Middle, 0,  0,   500, 350, browserWindow,        ALWAYS_CLOSED,  R10, 0, {{0}}, { RESIZABLE, 200, 340, 1600, 1800 }, }, 
    { WDW_LFG,                        kWdwAnchor_Center,    kWdwAnchor_Middle, 0,  0,   490, 424, lfgWindow,            DEFAULT_CLOSED, R10, 0, {{0}}, { RESIZABLE, 450, 490, 1600, 1600 }, },   
    { WDW_MAP_SELECT,                kWdwAnchor_Center,    kWdwAnchor_Middle, 0,  0,   500, 450, mapSelectWindow,      ALWAYS_CLOSED,  R10, 0, {{0}}, { RESIZABLE, 400, 200, 640, 600 }, },  
    { WDW_BADGES,                    kWdwAnchor_Left,    kWdwAnchor_Top, 0,    0,    440, 200, badgesWindow,         DEFAULT_CLOSED, R10, 1, {{0}}, { RESIZABLE, 200, 400, 600, 550 }, }, 
    { WDW_ARENA_CREATE,                kWdwAnchor_Center,    kWdwAnchor_Middle, 0,  0,   660, 480, arenaCreateWindow,    ALWAYS_CLOSED,  R10, 0, {{0}}, { VERTONLY,  300, 660, 1800, 660 }, },
    { WDW_SUPERGROUP_LIST,            kWdwAnchor_Center,    kWdwAnchor_Middle, 0,  0,   630, 300, supergroupListWindow, ALWAYS_CLOSED,  R10, 0, {{0}}, { VERTONLY,  100, 630, 1800, 630 }, }, 
    { WDW_BASE_PROPS,                kWdwAnchor_Right,    kWdwAnchor_Top, 0, 0,    200, 250, basepropsWindow,      ALWAYS_CLOSED,  R10, 1, {{0}}, { HORZONLY, 200, 150, 400, 400 }, },
    { WDW_BASE_INVENTORY,            kWdwAnchor_Center,    kWdwAnchor_Middle, 0,  0,   440, 150, baseInventoryWindow,  ALWAYS_CLOSED,  R10, 1, {{0}}, { VERTONLY,  100, 440, 250, 440 }, },  
    { WDW_BASE_ROOM,                kWdwAnchor_Left,    kWdwAnchor_Top, 0,    0,    250, 505, baseRoomWindow,       ALWAYS_CLOSED,  R10, 1, {{0}}, { 0 },},
    { WDW_INVENTORY,                kWdwAnchor_Left,    kWdwAnchor_Top, 100,  100,  250, 300, inventoryWindow,      ALWAYS_CLOSED,  R10, 1, {{0}}, { RESIZABLE, 200, 150, 550, 750 }, },
    { WDW_SALVAGE,                    kWdwAnchor_Left,    kWdwAnchor_Top, 150,  150,  270, 300, salvageWindow,        ALWAYS_CLOSED,  R10, 1, {{0}}, { RESIZABLE, 300, 400, 1600, 1800 }, },
    { WDW_STOREDSALVAGE,            kWdwAnchor_Left,    kWdwAnchor_Top, 150,  150,  250, 300, storedSalvageWindow,        ALWAYS_CLOSED,  R10, 1, {{0}}, { RESIZABLE, 300, 400, 1600, 1800 }, },
    { WDW_CONCEPTINV,                kWdwAnchor_Left,    kWdwAnchor_Top, 200,  200,  250, 300, conceptinvWindow,     ALWAYS_CLOSED,  R10, 1, {{0}}, { RESIZABLE, 200, 150, 550, 750 }, },
    { WDW_RECIPEINV,                kWdwAnchor_Left,    kWdwAnchor_Top, 250,  250,  250, 300, recipeinvWindow,      ALWAYS_CLOSED,  R10, 1, {{0}}, { RESIZABLE, 200, 150, 550, 750 }, },
    { WDW_INVENT,                    kWdwAnchor_Left,    kWdwAnchor_Top, 300,  300,  250, 300, inventWindow,         ALWAYS_CLOSED,  R10, 1, {{0}}, { RESIZABLE, 200, 150, 550, 750 }, },
    { WDW_OPTIONS,                    kWdwAnchor_Left,    kWdwAnchor_Top, 0,    100,  580, 400, optionsWindow,        ALWAYS_CLOSED,  R10, 1, {{0}}, { RESIZABLE, 200, 580, 1600, 1800 }, },
    { WDW_SGRAID_LIST,                kWdwAnchor_Center,    kWdwAnchor_Middle, 0,  0,   480, 300, sgRaidListWindow,       ALWAYS_CLOSED,  R10, 0, {{0}}, { VERTONLY,  100, 480, 800, 480 }, }, 
    { WDW_ARENA_GLADIATOR_PICKER,    kWdwAnchor_Center,    kWdwAnchor_Middle, 0,  0,750,550, arenaGladiatorPickerWindow, ALWAYS_CLOSED, R10, 0, {{0}},{ RESIZABLE, 200, 400, 1600, 1800 }, },
    { WDW_SGRAID_TIME,                kWdwAnchor_Center,    kWdwAnchor_Middle, 0,  0,   300, 250, sgRaidTimeWindow,       ALWAYS_CLOSED,  R10, 0, {{0}}, { 0 }, }, 
    { WDW_SGRAID_SIZE,                kWdwAnchor_Center,    kWdwAnchor_Middle, 0,  0,   200, 200, sgRaidSizeWindow,       ALWAYS_CLOSED,  R10, 0, {{0}}, { 0 }, }, 
    { WDW_EDITOR_UI_WINDOW_1,        kWdwAnchor_Center,    kWdwAnchor_Middle, 0,  0,   200, 200, editorUIDrawWindow,   DEFAULT_OPEN,   R10, 0, {{0}}, { 0 }, }, 
    { WDW_EDITOR_UI_WINDOW_2,        kWdwAnchor_Center,    kWdwAnchor_Middle, 0,  0,   200, 200, editorUIDrawWindow,   DEFAULT_OPEN,   R10, 0, {{0}}, { 0 }, }, 
    { WDW_EDITOR_UI_WINDOW_3,        kWdwAnchor_Center,    kWdwAnchor_Middle, 0,  0,   200, 200, editorUIDrawWindow,   DEFAULT_OPEN,   R10, 0, {{0}}, { 0 }, }, 
    { WDW_EDITOR_UI_WINDOW_4,        kWdwAnchor_Center,    kWdwAnchor_Middle, 0,  0,   200, 200, editorUIDrawWindow,   DEFAULT_OPEN,   R10, 0, {{0}}, { 0 }, }, 
    { WDW_EDITOR_UI_WINDOW_5,        kWdwAnchor_Center,    kWdwAnchor_Middle, 0,  0,   200, 200, editorUIDrawWindow,   DEFAULT_OPEN,   R10, 0, {{0}}, { 0 }, }, 
    { WDW_CHANNEL_SEARCH,            kWdwAnchor_Center,    kWdwAnchor_Middle, 0,  0,   630, 300, channelSearchWindow,  ALWAYS_CLOSED,  R10, 0, {{0}}, { VERTONLY,  200, 630, 800, 630 }, }, 
    { WDW_BASE_STORAGE,                kWdwAnchor_Left,    kWdwAnchor_Top, 350,  350,  460, 400, basestorageWindow,    ALWAYS_CLOSED,  R10, 1, {{0}}, { RESIZABLE, 200, 460, 1600, 1800 }, },
    { WDW_BASE_LOG,                    kWdwAnchor_Left,    kWdwAnchor_Top, 350,  350,  400, 400, baselogWindow,        ALWAYS_CLOSED,  R10, 1, {{0}}, { RESIZABLE, 200, 400, 1600, 1800 }, },
    { WDW_EDITOR_UI_WINDOW_6,        kWdwAnchor_Center,    kWdwAnchor_Middle, 0,  0,   200, 200, editorUIDrawWindow,   DEFAULT_OPEN,   R10, 0, {{0}}, { 0 }, }, 
    { WDW_EDITOR_UI_WINDOW_7,        kWdwAnchor_Center,    kWdwAnchor_Middle, 0,  0,   200, 200, editorUIDrawWindow,   DEFAULT_OPEN,   R10, 0, {{0}}, { 0 }, }, 
    { WDW_EDITOR_UI_WINDOW_8,        kWdwAnchor_Center,    kWdwAnchor_Middle, 0,  0,   200, 200, editorUIDrawWindow,   DEFAULT_OPEN,   R10, 0, {{0}}, { 0 }, }, 
    { WDW_EDITOR_UI_WINDOW_9,        kWdwAnchor_Center,    kWdwAnchor_Middle, 0,  0,   200, 200, editorUIDrawWindow,   DEFAULT_OPEN,   R10, 0, {{0}}, { 0 }, }, 
    { WDW_EDITOR_UI_WINDOW_10,        kWdwAnchor_Center,    kWdwAnchor_Middle, 0,  0,  200, 200, editorUIDrawWindow,   DEFAULT_OPEN,   R10, 0, {{0}}, { 0 }, }, 
    { WDW_PLAQUE,                    kWdwAnchor_Center,    kWdwAnchor_Middle, 0,  0,  550, 400, plaqueShowDialog,     ALWAYS_CLOSED,  R10, 0, {{0}}, { RESIZABLE,  300, 550, 1800, 1800 }, }, 
    { WDW_SGRAID_STARTTIME,            kWdwAnchor_Center,    kWdwAnchor_Middle, 0,  0,  300, 150, sgRaidStartTimeWindow, ALWAYS_CLOSED,  R10, 0, {{0}}, { 0 }, }, 
    { WDW_RAIDRESULT,                kWdwAnchor_Center,    kWdwAnchor_Middle, 0,  0,  400, 300, uiRaidResultDisplay,   DEFAULT_CLOSED,  R10, 0 }, 
    { WDW_RECIPEINVENTORY,            kWdwAnchor_Left,    kWdwAnchor_Top, 350,  350,  600, 400, recipeInventoryWindow, ALWAYS_CLOSED,  R10, 1, {{0}}, { RESIZABLE, 250, 600, 1800, 1800 }, },
    { WDW_AMOUNTSLIDER,                kWdwAnchor_Center,    kWdwAnchor_Middle, 0,  0,  147, 110,  amountSliderWindow,   ALWAYS_CLOSED,  R10, 0, {{0}}, { 0 }, },
    { WDW_COMBATNUMBERS,            kWdwAnchor_Center,    kWdwAnchor_Middle, 0,  0,  533, 600, combatNumbersWindow,    DEFAULT_CLOSED, R10, 1, {{0}}, { RESIZABLE, 250, 400, 1600, 1800 }, },
    { WDW_COMBATMONITOR,            kWdwAnchor_Left,    kWdwAnchor_Top, 800, 200, 1, 1,        combatMonitorWindow,    DEFAULT_CLOSED, R10, 1, {{0}}, {0}, 1},
    { WDW_TRIALREMINDER,            kWdwAnchor_Center,    kWdwAnchor_Middle, 0,  0, 1, 1,        trialReminderWindow,    DEFAULT_CLOSED, R10, 0, {{0}}, {0}, 1},
    { WDW_COLORPICKER,                kWdwAnchor_Center,    kWdwAnchor_Middle, 0,  0, 300, 250,    chatColorPickerWindow,  ALWAYS_CLOSED, R10, 0, },
    { WDW_PLAYERNOTE,                kWdwAnchor_Center,    kWdwAnchor_Middle, 0,  0, 300, 250,    playerNoteWindow,        DEFAULT_CLOSED, R10, 0, {{0}}, { RESIZABLE, 250, 400, 1600, 1800 }, }, // TODO REMOVE RESIZE?
    { WDW_RECENTTEAM,                kWdwAnchor_Center,    kWdwAnchor_Middle, 0,  0, 300, 250,    recentTeamWindow,        DEFAULT_CLOSED, R10, 0, {{0}}, { RESIZABLE, 250, 400, 1600, 1800 }, },
    { WDW_MISSIONMAKER,                kWdwAnchor_Center,    kWdwAnchor_Middle, 0,  0, 950, 850,    missionMakerWindow,        ALWAYS_CLOSED, R10, 1, {{0}}, { RESIZABLE, 550, 900, 1600, 1800 }, 0, 1 }, // TODO REMOVE RESIZE?
    { WDW_MISSIONSEARCH,            kWdwAnchor_Center,    kWdwAnchor_Middle, 0,  0, 750, 550,    missionsearch_Window,    ALWAYS_CLOSED, R10, 1, {{0}}, { RESIZABLE, 400, 720, 1600, 1800 }, },
    { WDW_MISSIONREVIEW,            kWdwAnchor_Center,    kWdwAnchor_Middle, 0,  0, 480, 480,   missionReviewWindow,    ALWAYS_CLOSED, R10, 1, {{0}}, { RESIZABLE, 180, 350, 1600, 1800 }, },
    { WDW_BADGEMONITOR,                kWdwAnchor_Left,    kWdwAnchor_Top, 800, 200, 1, 1,        badgeMonitorWindow,        DEFAULT_CLOSED, R10, 0, {{0}}, {0}, 1},
    { WDW_CUSTOMVILLAINGROUP,        kWdwAnchor_Center,    kWdwAnchor_Middle, 0,  0, 350, 200,    customVillainGroupWindow,    ALWAYS_CLOSED, R10, 0, {{0}}, { RESIZABLE, 550, 1000, 1600, 1800 }, 0, 1},
    { WDW_BASE_STORAGE_PERMISSIONS,    kWdwAnchor_Center,    kWdwAnchor_Middle, 0,  0, 250, 250, basepermissionsWindow,       ALWAYS_CLOSED,  R10, 1, {{0}}, { 0 }, },
    { WDW_ARENA_OPTIONS,            kWdwAnchor_Center,    kWdwAnchor_Middle, 0,  0, 660, 480,   arenaOptionsWindow,     ALWAYS_CLOSED, R10, 0, {{0}}, { 0 }, },
    { WDW_MISSIONCOMMENT,            kWdwAnchor_Center,    kWdwAnchor_Middle, 0,  0, 400, 400,     missionCommentWindow,   ALWAYS_CLOSED, R10, 0, {{0}}, { RESIZABLE, 250, 400, 1600, 1800 }, },
    { WDW_INCARNATE,                kWdwAnchor_Center,    kWdwAnchor_Middle, 0,  0, 700, 345,    incarnateWindow,        DEFAULT_CLOSED, R10, 0, {{0}}, {RESIZABLE, 345, 700, 1600, 1800 }, },
    { WDW_POP_HELP,                    kWdwAnchor_Right,    kWdwAnchor_Middle, 0, 0,  48, 240, popHelpWindow,          DEFAULT_OPEN, R10, 1, {{WDW_POP_HELP_TEXT,""}}, { 0 }, 1, 0 },
    { WDW_POP_HELP_TEXT,            kWdwAnchor_Right,    kWdwAnchor_Middle, -50,  0, 385, 385,   popHelpTextWindow,      ALWAYS_CLOSED,  R10, 1, {{0}}, { VERTONLY,  100, 385, 2000, 385 }, },
    { WDW_SCRIPT_UI,                kWdwAnchor_Right,    kWdwAnchor_Top, -70,  150, 350, 250,    scriptUIWindow,         ALWAYS_CLOSED,  R10, 1, {{0}}, },
    { WDW_AUCTION,                    kWdwAnchor_Center,    kWdwAnchor_Middle, 0,  0,  800, 550,    auctionWindow,            ALWAYS_CLOSED,  R10, 1, {{0}}, { RESIZABLE, 430, 740, 1800, 1800 }, },
    { WDW_KARMA_UI,                    kWdwAnchor_Center,    kWdwAnchor_Middle, 0,  0, 450, 250,        scriptUIWindow,         ALWAYS_CLOSED,  R10, 1, {{0}}, },
    { WDW_TURNSTILE,                kWdwAnchor_Center,    kWdwAnchor_Middle, 0,  0,  400, 300,    turnstileWindow,        ALWAYS_CLOSED,  R10, 1, {{0}}, { RESIZABLE, 300, 400, 1600, 1800 }, },
    { WDW_TURNSTILE_DIALOG,            kWdwAnchor_Center,    kWdwAnchor_Middle, 0,  0,  300, 200,    turnstileDialogWindow,  DEFAULT_CLOSED,  R10, 1, {{0}}, },
    { WDW_TRAY_RAZER,                kWdwAnchor_Right,    kWdwAnchor_Top, 0, 299, 130, 170,    trayWindowSingleRazer,    DEFAULT_CLOSED, R22, 1, {{0}}, { TRAY_FIXED_SIZES, 50, 50, 450, 450 }, 1},
    { WDW_CONTACT_FINDER,            kWdwAnchor_Center,    kWdwAnchor_Middle, 0,  0,  385, 385,  contactFinderWindow,    ALWAYS_CLOSED, R10, 0, {{0}}, },
    { WDW_LOYALTY_TREE,                kWdwAnchor_Center,    kWdwAnchor_Middle, 0,  0,  800, 670,  loyaltyTreeWindow,    DEFAULT_CLOSED, R10, 0, {{0}}, {0}, 1},
    { WDW_LOYALTY_TREE_ACCESS,        kWdwAnchor_Right,    kWdwAnchor_Top, -10,  115,  50, 50,  loyaltyTreeAccessWindow,    ALWAYS_CLOSED, R22, 1, {{0}}, {0}, 1},
    { WDW_LWC_UI,                    kWdwAnchor_Left,    kWdwAnchor_Top, 50,  90,  100, 50,    uiLWCWindow,    DEFAULT_OPEN, R22, 0, {{0}}, {0}, 1},
    { WDW_SALVAGE_OPEN,                kWdwAnchor_Center,    kWdwAnchor_Middle, 0,  0,  1000, 400,  salvageOpenWindow,    ALWAYS_CLOSED, R10, 0, {{0}}, {0}, 1},
    { WDW_CONVERT_ENHANCEMENT,        kWdwAnchor_Left,    kWdwAnchor_Top, 0,    0,    440, 300, convertEnhancementWindow,    DEFAULT_CLOSED, R10, 0},
    { WDW_NEW_FEATURES,                kWdwAnchor_Center,    kWdwAnchor_Top, 0,  150,  300, 20,  newFeaturesWindow,    DEFAULT_CLOSED, R10, 1},
    // if you add a new window, don't forget to set up the commandableness of it in the s_aUserWindows array
};


// Window Init Functions
//-------------------------------------------------------------------------------------------------------------------
//
int windowDefaultsInitialized = 0;

// add a child - modifies child also!
//
void window_initChild( Wdw *wdw, int child, int num, char*name, char *command )
{
    memset(&wdw->children[num], 0, sizeof(ChildWindow) );

    wdw->children[num].idx = child;
    wdw->children[num].opacity = .5f;

    if( command && strlen(command) )
    {
        wdw->children[num].command = malloc( strlen(command)+1 );
        strcpy( wdw->children[num].command, command );
    }
    else
        wdw->children[num].command = 0;

    strncpyt( wdw->children[num].name, name, 32 );

    if( child )
    {
        wdw->children[num].wdw = &winDefs[child];
        winDefs[child].parent = wdw;
        winDefs[child].loc.locked = TRUE;
    }
}

// set the basic values for a window
static void window_init( WindowDefault * def, int force_size )
{
    Entity *e = playerPtr();
    Wdw * wdw;
    int w, h;
    UISkin skin = UISKIN_HEROES;

    if (e)
    {
        if (ENT_IS_IN_PRAETORIA(e))
            skin = UISKIN_PRAETORIANS;
        else if (ENT_IS_VILLAIN(e))
            skin = UISKIN_VILLAINS;
    }
    else
    {
        skin = game_state.skin;
    }

    windowClientSize( &w, &h );
    wdw = windowDefaultsInitialized ? wdwGetWindow(def->id):wdwCreate(def->id,0);

    wdw->code = def->code;

    wdw->loc.wd = def->wd;
    if( !wdw->loc.ht || force_size )
        wdw->loc.ht = def->ht;

    if( def->horizAnchor == kWdwAnchor_Right )
        wdw->loc.xp = w + def->xoff;
    else if( def->horizAnchor == kWdwAnchor_Center )
        wdw->loc.xp = w / 2 + def->xoff;
    else
        wdw->loc.xp = def->xoff;

    if( def->vertAnchor == kWdwAnchor_Bottom )
        wdw->loc.yp = h + def->yoff;
    else if( def->vertAnchor == kWdwAnchor_Middle )
        wdw->loc.yp = h / 2 + def->yoff;
    else
        wdw->loc.yp = def->yoff;



    wdw->loc.draggable_frame = def->limits.type;
    if (def->limits.max_wd && def->limits.min_wd && def->limits.max_ht && def->limits.min_ht)
    {
        wdw->max_wd = def->limits.max_wd; 
        wdw->min_wd = def->limits.min_wd; 
        wdw->max_ht = def->limits.max_ht; 
        wdw->min_ht = def->limits.min_ht; 
    }
    else
    {
        wdw->max_wd = wdw->min_wd = def->wd;
        wdw->max_ht = wdw->min_ht = def->ht;
    }

    wdw->loc.start_shrunk = def->default_state;
    wdw->radius = def->radius + PIX3;
    wdw->save = def->save;
    wdw->noFrame = def->noFrame;

    if( def->maximized )
        wdw->loc.maximized = 1;

    if (wdw->loc.color == DEFAULT_HERO_WINDOW_COLOR ||
        wdw->loc.color == DEFAULT_VILLAIN_WINDOW_COLOR ||
        wdw->loc.color == DEFAULT_PRAETORIAN_WINDOW_COLOR || 
        wdw->loc.color == 0)    //hasn't been set yet.
    {
        if (skin == UISKIN_PRAETORIANS)
        {
            wdw->loc.color = DEFAULT_PRAETORIAN_WINDOW_COLOR;
            wdw->loc.back_color = 0x00000088;
        }
        else if (skin == UISKIN_VILLAINS)
        {
            wdw->loc.color = DEFAULT_VILLAIN_WINDOW_COLOR;
            wdw->loc.back_color = 0x00000088;
        }
        else
        {
            wdw->loc.color = DEFAULT_HERO_WINDOW_COLOR;
            wdw->loc.back_color = 0x00000088;
        }
    }

    wdw->loc.sc = 1.f;

    while( wdw->num_children < WINDOW_MAX_CHILDREN && def->children[wdw->num_children].name && def->children[wdw->num_children].name[0] )
    {
        ChildDefault *child = &def->children[wdw->num_children];
        window_initChild( wdw, child->child, wdw->num_children, child->name, child->command );
        wdw->num_children++;
    }
}

bool window_addChild( Wdw *wdw, int child, char*name, char *command )
{
    bool res = false;

    if( verify( wdw && INRANGE0( child, MAX_WINDOW_COUNT ) && name ))
    {
        int i;
        for( i = 0; i < wdw->num_children; ++i ) 
        {
            if( wdw->children[i].idx == child )
            {
                break;
            }
        }

        // not a duplicate
        if( i == wdw->num_children)
        {
            window_initChild( wdw, child, wdw->num_children++, name, command );
            res = true;
        }
    }

    // --------------------
    // finally

    return res;
}

bool window_removeChild(Wdw *wdw, char *name )
{
    bool res = false;
    if( verify( wdw && name ))
    {
        int i;
        for( i = 0; i < wdw->num_children; ++i ) 
        {
            if( 0 == strncmp(name, wdw->children[i].name, ARRAY_SIZE( wdw->children[i].name )))
            {
                break;
            }
        }

        // move the next ones down
        if( i < wdw->num_children )
        {
            CopyStructs(wdw->children+i,wdw->children+i+1, wdw->num_children - i - 1 );
            wdw->num_children--;
        }
    }
    return res;
}


//----------------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------
// This is where most of the setup for the windows happens
//
void windows_initDefaults(int force_size)
{
    int i, count;

    count = ARRAY_SIZE(window_defaults);

    for( i = 0; i < count; i++ )
        window_init( &window_defaults[i], force_size );

    // clamp min and max window sizes
    for( i = 0; i < MAX_WINDOW_COUNT; i++ )
    {
        winDefs[i].loc.wd = MINMAX( winDefs[i].loc.wd, winDefs[i].min_wd, winDefs[i].max_wd );
        winDefs[i].loc.ht = MINMAX( winDefs[i].loc.ht, winDefs[i].min_ht, winDefs[i].max_ht );
    }

    // special hacks

    winDefs[WDW_TRAY].loc.ht = 0.f;

    if (windowDefaultsInitialized && (playerIsMasterMind() && optionGet(kUO_ShowPets)) )
        winDefs[WDW_PET].loc.start_shrunk = DEFAULT_OPEN;

    if( playerIsMasterMind() )
        window_addChild(wdwGetWindow(WDW_PET), 0, "PetOptions", "petoptions" );
    else
        window_removeChild(wdwGetWindow(WDW_PET), "PetOptions" );

    windowDefaultsInitialized = TRUE;

    loadCustomWindows();
}

//----------------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------

void windows_ResetLocations(void)
{
    int z;

    resetPrimaryChatSize();

    windows_initDefaults(1);
    for( z = 0; z < MAX_WINDOW_COUNT+custom_window_count; z++ )
    {
        winDefs[z].loc.sc = 1.f;

        if( z == WDW_DIALOG )
            continue;

        if( winDefs[z].loc.start_shrunk )
        {
            window_setMode(z, WINDOW_DOCKED );
        }
        else
        {
            window_setMode(z, WINDOW_DISPLAYING);
        }

        winDefs[z].loc.maximized = 0;

        // Force an update of the window parameters
        if(!playerPtr() || !shell_menu())
            window_UpdateServer(z);
    }
}

void closeUnimportantWindows()
{
    int z;

    for( z = 0; z < MAX_WINDOW_COUNT+custom_window_count; z++ )
    {
        // ALWAYS_CLOSED windows
        if( winDefs[z].loc.start_shrunk )
            window_setMode(z, WINDOW_DOCKED );

        // DEFAULT_CLOSED windows, Korea has deemed should be closed
        if( z == WDW_POWERLIST ||
            z == WDW_TARGET_OPTIONS ||
            z == WDW_INFO ||
            z == WDW_LFG )
        {
            window_setMode(z, WINDOW_DOCKED );
        }

        window_UpdateServer(z);
    }
}

int window_Isfading( Wdw *win)
{
    if( optionGet(kUO_WindowFade) ) // all windows are fading
        return 1;

    if( win->id >= WDW_TRAY_1 && win->id <= WDW_TRAY_8 && optionGet(kUO_FadeExtraTrays) )
        return 1;

    if( (win->id == WDW_CHAT_BOX && optionGet(kUO_ChatFade)) ||
        (win->id == WDW_CHAT_CHILD_1 && optionGet(kUO_Chat1Fade)) ||
        (win->id == WDW_CHAT_CHILD_2 && optionGet(kUO_Chat2Fade)) ||
        (win->id == WDW_CHAT_CHILD_3 && optionGet(kUO_Chat3Fade)) ||
        (win->id == WDW_CHAT_CHILD_4 && optionGet(kUO_Chat4Fade)) ||
        (win->id == WDW_COMPASS && optionGet(kUO_CompassFade)) )
    {
        return 1;
    }

    return 0;
}


static int windowIsDifferentFromDefault( Wdw * win, WindowDefault *def )
{
    if( win->loc.sc != 1.f || win->loc.wd != def->wd || win->loc.ht != def->ht || win->loc.maximized != def->maximized )
        return true;

    {
        Entity *e = playerPtr();
        int color, bcolor;

        UISkin skin = UISKIN_HEROES;

        if (e)
        {
            if (ENT_IS_IN_PRAETORIA(e))
                skin = UISKIN_PRAETORIANS;
            else if (ENT_IS_VILLAIN(e))
                skin = UISKIN_VILLAINS;
        }
        else
        {
            skin = game_state.skin;
        }

        
        if (skin == UISKIN_PRAETORIANS)
        {
            color = 0x707072ff;
            bcolor = 0x00000088;
        }
        else if (skin == UISKIN_VILLAINS)
        {
            color = 0xa62929ff;
            bcolor = 0x00000088;
        }
        else
        {
            color = 0x3399ffff;
            bcolor= 0x00000088;
        }

        if( color != win->loc.color || bcolor != win->loc.back_color )
            return true;
    }

    if( !win->parent )
    {
        int x, y, w, h;
        int leftX, upperY;

        window_getUpperLeftPos(win->id, &leftX, &upperY);
        windowClientSize( &w, &h );

        if( def->horizAnchor == kWdwAnchor_Right )
            x = w + def->xoff;
        else if( def->horizAnchor == kWdwAnchor_Center )
            x = w / 2 + def->xoff;
        else
            x = def->xoff;

        if( def->vertAnchor == kWdwAnchor_Bottom )
            y = h + def->yoff;
        else if( def->vertAnchor == kWdwAnchor_Middle )
            y = h / 2 + def->yoff;
        else
            y = def->yoff;

        if( leftX != x || upperY != y )
            return true;
    }

    if( (def->default_state == DEFAULT_OPEN && win->loc.mode != WINDOW_DISPLAYING) ||
        (def->default_state == DEFAULT_CLOSED && win->loc.mode != WINDOW_DOCKED ) )
    {
        return true;
    }

    return false;
}

int  windowNeedsSaving(int wdw)
{
    Wdw * win = wdwGetWindow(wdw);
    int i, count;

    if( !win->save )
        return false;

    count = ARRAY_SIZE(window_defaults);

    for( i = 0; i < count; i++ )
    {
        if( window_defaults[i].id == wdw )
            return windowIsDifferentFromDefault( win, &window_defaults[i] );
    }

    // didn't find window in defualt list...um...lets not save
    return false;
}

int  windowIsSaved(int wdw) 
{
    int i, count;

    count = ARRAY_SIZE(window_defaults);

    for( i = 0; i < count; i++ )
    {
        if( window_defaults[i].id == wdw )
            return window_defaults[i].save;            
    }
    return false;
}


void window_HandleResize( int newWd, int newHt, int oldWd, int oldHt )
{
    int i, setX = 0, setY = 0;

    if( newWd && newWd > oldWd )
        setX = TRUE;

    if( newHt && newHt > oldHt )
        setY = TRUE;

    for( i = 0; i < MAX_WINDOW_COUNT+custom_window_count; i++ )
    {
        int leftX, upperY;
        float newX = -1, newY = -1;
        WdwBase * wdw = &winDefs[i].loc;

        window_getUpperLeftPos(i, &leftX, &upperY);

        if( setX && leftX + wdw->wd == oldWd )
            newX = (float) newWd - wdw->wd;

        if( setY && upperY + wdw->ht == oldHt )
            newY = (float) newHt - wdw->ht;

        window_setUpperLeftPos(i, newX, newY);
    }
}


void resetWindows(void)
{
    int i, count;

    count = ARRAY_SIZE(window_defaults);

    // Reset all of the window colors
    for( i = 0; i < count; i++ )
    {
        Wdw *wdw = wdwGetWindow(window_defaults[i].id);
        wdw->loc.color = 0;
    }

    windows_initialized = FALSE;
    winDefs[0].loc.mode = WINDOW_UNINITIALIZED;
    initWindows();
}

int initWindows(void)
{
    int z;

    if(windows_initialized)
        return TRUE;

    for( z = 0; z < MAX_WINDOW_COUNT+custom_window_count; z++ )
    {
        winDefs[z].timer            = 0.f;
        winDefs[z].window_depth     = z;
    }

    if( winDefs[0].loc.mode == WINDOW_UNINITIALIZED )
        windows_ResetLocations();
    else
    {
        // Apply all the saved state to the actual windows.
        for( z = 0; z < MAX_WINDOW_COUNT+custom_window_count; z++ )
        {
            if ( winDefs[z].loc.mode == WINDOW_SHRINKING || winDefs[z].loc.mode == WINDOW_DOCKED  )
                window_setMode( z, winDefs[z].loc.mode );
        }
    }

    windows_initialized = TRUE;
    return TRUE;
}


void wdwSave(char * pch)
{
    int i, count = 0;
    FILE *file;
    char filename[FILENAME_MAX];
    char dir[FILENAME_MAX];

    if(pch) 
        strcpy(filename,pch);
    else if(isDevelopmentMode())
        strcpy(filename, "C:/wdw.txt");
    else
        sprintf(filename, "%s/wdw.txt", getExecutableDir(dir));

    file = fileOpen( filename, "wt" );
    if (!file)
    {
        addSystemChatMsg("UnableWdwSave", INFO_USER_ERROR, 0);
        return;
    }

    fprintf(file, "Revision 2\n");

    count = ARRAY_SIZE(window_defaults);
    for( i = 0; i < count; i++ )
    {
        Wdw * win = wdwGetWindow(window_defaults[i].id );

        if(!win->save)
            continue;
        if(!windowIsDifferentFromDefault( win, &window_defaults[i] ))
            continue;

        if(!windows_GetNameByNumber(win->id))
            continue;

        fprintf(file, "%s %i %i %i %i %f %x %x %i %i\n", 
            windows_GetNameByNumber(win->id), win->loc.xp, win->loc.yp, win->loc.wd, win->loc.ht, win->loc.sc, win->loc.color, win->loc.back_color, win->loc.locked, win->loc.mode );
    }

    addSystemChatMsg( textStd("WdwSaved", filename), INFO_SVR_COM, 0);
    fclose(file);
}

void wdwLoad(char *pch)
{
    FILE *file;
    char buf[1000];
    char filename[FILENAME_MAX];
    char dir[FILENAME_MAX];
    int revision = 0;

    if(pch)
        strcpy( filename, pch);
    else if(isDevelopmentMode())
        strcpy(filename, "C:/wdw.txt");
    else
        sprintf(filename, "%s/wdw.txt", getExecutableDir(dir));

    file = fileOpen( filename, "rt" );
    if(!file)
    {
        if(pch)
            addSystemChatMsg(textStd("CantReadWdw", pch), INFO_USER_ERROR, 0);
        return;
    }

    windows_ResetLocations();
    while(fgets(buf,sizeof(buf),file))
    {
        char *args[10];
        int count = tokenize_line_safe( buf, args, 10, 0 );
        if( count != 10 )
        {
            if (!strnicmp(args[0], "Revision", 8))
            {
                revision = atoi(args[1]);
            }
            else
            {
                addSystemChatMsg( textStd("InvalidWdW"), INFO_USER_ERROR, 0 );
            }
        }
        else
        {
            Wdw * pwdw = windows_Find( args[0], 0, WDW_FIND_ANY );
            if(pwdw && pwdw->save)
            {
                int hex_int;
                int x = atoi(args[1]);
                int y = atoi(args[2]);
                int wd = atoi(args[3]);
                int ht = atoi(args[4]);
                F32 sc = atof(args[5]);

                if (pwdw->id == WDW_TRAY && revision < 1)
                    y += ht;

                if (pwdw->min_ht && pwdw->max_ht && pwdw->min_wd && pwdw->max_wd)
                {
                    pwdw->loc.wd = MINMAX(wd, pwdw->min_wd, pwdw->max_wd);
                    pwdw->loc.ht = MINMAX(ht, pwdw->min_ht, pwdw->max_ht);
                }

                pwdw->loc.xp = x;
                pwdw->loc.yp = y;
                window_setScale(pwdw->id, sc);
                sscanf(args[6],"%x", &hex_int);
                window_SetColor(pwdw->id, hex_int );
                
                pwdw->loc.locked = atoi(args[8]);
                if( windows_Find( args[0], 0, WDW_FIND_OPEN) )
                    window_setMode(pwdw->id, atoi(args[9]));

            }
        }
    }

    // **************************
    // Translation for revision 1
    // **************************

    // if option < 2, this work will be done in init_map_optSettings.
    if (revision < 1 && optionGet(kUO_MapOptionRevision) >= 2)
    {
        fixWindowHeightValuesToNewAttachPoints();
    }

    // **************************
    // Translation for revision 2
    // **************************

    // if revision < 1, fixWindowHeightValuesToNewAttachPoints will do this work.
    // if option < 3, this work will be done in init_map_optSettings.
    if (revision == 1 && optionGet(kUO_MapOptionRevision) >= 3)
    {
        fixInspirationTrayToNewAttachPoint();
    }

    // if option < 3, this work will be done in init_map_optSettings.
    if (revision < 2 && optionGet(kUO_MapOptionRevision) >= 3)
    {
        fixInspirationTrayToDealWithRealWidthAndHeight();
    }

    // send to server
    window_UpdateServerAllWindows();

    addSystemChatMsg( textStd("WdwLoaded", filename), INFO_SVR_COM, 0);
    fclose(file);
}

void window_getAnchors(int wdw, WindowHorizAnchor *horiz, WindowVertAnchor *vert)
{
    int defaultIndex, defaultCount = ARRAY_SIZE(window_defaults);
    WindowDefault *windowDefault = NULL;
    
    for    (defaultIndex = 0; defaultIndex < defaultCount; defaultIndex++ )
    {
        if (window_defaults[defaultIndex].id == wdw)
        {
            windowDefault = &window_defaults[defaultIndex];
        }
    }

    if (!windowDefault)
        return;

    if (horiz)
        (*horiz) = windowDefault->horizAnchor;
    if (vert)
        (*vert) = windowDefault->vertAnchor;
}

// Be very careful with this function!
// Because we can change the values in the window defaults after the fact,
// this function will do different things as time goes on.
// For example, after the initial implementation, I changed the Inspiration tray from
// upper-left to lower-right.  This means that anything going through this function after
// that change gets translated, *but anything that went through this function before does not!*
// The fixInspirationTray...() functions below this try to address this problem.
// I'm now fairly sure this was not the proper way to implement this function, but it's too late...
void fixWindowHeightValuesToNewAttachPoints(void)
{
    Entity *e = playerPtr();
    int winIndex;
    int defaultIndex, defaultCount = ARRAY_SIZE(window_defaults);

    for (winIndex = 0; winIndex < (MAX_WINDOW_COUNT + MAX_CUSTOM_WINDOW_COUNT); winIndex++)
    {
        WindowDefault *windowDefault = NULL;

        for    (defaultIndex = 0; defaultIndex < defaultCount; defaultIndex++ )
        {
            if (window_defaults[defaultIndex].id == winIndex)
            {
                windowDefault = &window_defaults[defaultIndex];
            }
        }

        if (!windowDefault)
            continue;

        switch (windowDefault->horizAnchor)
        {
        case kWdwAnchor_Left:
            winDefs[winIndex].loc.xp += 0.0f;
        xcase kWdwAnchor_Center:
            winDefs[winIndex].loc.xp += winDefs[winIndex].loc.wd / 2;
        xcase kWdwAnchor_Right:
            winDefs[winIndex].loc.xp += winDefs[winIndex].loc.wd;
        }

        switch (windowDefault->vertAnchor)
        {
        case kWdwAnchor_Top:
            winDefs[winIndex].loc.yp += 0.0f;
        xcase kWdwAnchor_Middle:
            winDefs[winIndex].loc.yp += winDefs[winIndex].loc.ht / 2;
        xcase kWdwAnchor_Bottom:
            winDefs[winIndex].loc.yp += winDefs[winIndex].loc.ht;
        }
    }
}

// This function is necessary due to the implementation of fixWindowHeightValuesToNewAttachPoints().
// See that function for a detailed comment as to why.
void fixInspirationTrayToNewAttachPoint(void)
{
    winDefs[WDW_INSPIRATION].loc.xp += winDefs[WDW_INSPIRATION].loc.wd * winDefs[WDW_INSPIRATION].loc.sc;
    winDefs[WDW_INSPIRATION].loc.yp += winDefs[WDW_INSPIRATION].loc.ht * winDefs[WDW_INSPIRATION].loc.sc;
}
