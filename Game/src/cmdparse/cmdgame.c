#include <stdlib.h>
#include <stdarg.h>
#include <utilitieslib/stdtypes.h>
#include "cmdparse/cmdaccountserver.h"
#include "bases/DetailRecipe.h"
#include "entity/character_workshop.h"
#include "entity/RewardItemType.h"
#include "entity/Invention.h"
#include <stdio.h>
#include <utilitieslib/stdtypes.h>
#include "string.h"
#include "entity/entity.h"
#include "UI/uiConsole.h"
#include <utilitieslib/components/MRUList.h>
#include "win/win_init.h"
#include "cmdparse/cmdgame.h"
#include <utilitieslib/utils/cmdoldparse.h>
#include "cmdparse/cmdcommon.h"
#include "cmdparse/cmdcommon_enum.h"
#include "cmdparse/cmdcontrols.h"
#include <utilitieslib/utils/file.h>
#include <utilitieslib/utils/fileutil.h>
#include "UI/uiGroupWindow.h"
#include "win/input.h"
#include "graphics/camera.h"
#include <utilitieslib/utils/memcheck.h>
#include "win/input.h"
#include "clientcomm/clientcomm.h"
#include "render/tex.h"
#include "graphics/gfx.h"
#include "graphics/gfxDebug.h"
#include "edit/edit_cmd.h"
#include "edit/edit_net.h"
#include "player/player.h" //mm
#include "graphics/FX/fx.h"     //mm
#include <utilitieslib/components/ArrayOld.h>    //js
#include "utils/debugUtils.h"
#include "graphics/ttFont.h"
#include "player/player.h" // for access_level
#include "sound/sound.h"
#include "UI/uiChat.h"
#include "win/win_cursor.h"
#include "UI/uiAutomap.h"
#include <utilitieslib/language/AppLocale.h>
#include <utilitieslib/utils/utils.h>
#include "entity/entclient.h"
#include "UI/uiCursor.h"
#include "UI/uiWindows.h"
#include "UI/uiWindows_init.h"
#include "UI/uiTarget.h"
#include "UI/uiGame.h"
#include "UI/uiDock.h"
#include "UI/uiReticle.h"
#include "utils/position.h"
#include "clientcomm/clientcomm.h"
#include "gameComm/wdwbase.h"
#include "language/langClientUtil.h"
#include <utilitieslib/utils/timing.h>
#include "entity/entDebug.h"
#include <utilitieslib/utils/error.h>
#include "win/input.h"
#include "entity/character_base.h"
#include "entity/character_inventory.h"
#include "entity/costume_client.h"
#include "player/playerSticky.h"
#include "UI/uiTeam.h"
#include "UI/uiGame.h"
#include "UI/uiContactDialog.h"
#include "UI/uiInspiration.h"
#include "UI/uiContextMenu.h"
#include <utilitieslib/utils/strings_opt.h>
#include "entity/character_level.h"
#include "storyarc/contactClient.h"
#include "testClient/testLevelup.h"
#include "render/rendermodel.h"
#include "render/renderUtil.h"
#include <utilitieslib/utils/sysutil.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/components/SharedMemory.h>
#include "demo.h"
#include "gameData/menudef.h"
#include "edit/edit_select.h"
#include "edit/edit_cmd_select.h"
#include "player/playerState.h"
#include "gameComm/dooranimclient.h"
#include "UI/uiKeybind.h"
#include "UI/uiTray.h"
#include <utilitieslib/version/AppVersion.h>
#include "graphics/truetype/ttDebug.h"
#include <utilitieslib/network/netio.h>
#include "comm_game.h"
#include "UI/uiSuperRegistration.h"
#include "storyarc/storyarcClient.h"
#include "render/renderUtil.h"
#include <utilitieslib/utils/MemoryMonitor.h>
#include "UI/uiNet.h"
#include "UI/uiQuit.h"
#include <utilitieslib/components/EString.h>
#include "UI/uiPetition.h"
#include <utilitieslib/utils/memlog.h>
#include "UI/uiMapSelect.h"
#include <utilitieslib/version/AppRegCache.h>
#include "clientcomm/authclient.h"
#include "entity/scriptDebugClient.h"
#include "UI/uiChatUtil.h"
#include "gameComm/chatClient.h"
#include "UI/sprite/sprite_text.h"
#include "render/texWords.h"
//#include "UI/Hybrid/uiBody.h"
#include "UI/uiChat.h"
#include "UI/uiOptions.h"
#include "UI/uiDialog.h"
#include "gameData/costume_data.h"
#include "arena/arenagame.h"
#include <utilitieslib/utils/FolderCache.h>
#include "UI/uiFriend.h"
#include "chatdb.h"
#include "UI/uiChannel.h"
#include "UI/uiSupergroup.h"
#include "UI/uiArenaList.h"
#include "render/renderperf.h"
#include "group/groupfileload.h"
#include "seq/anim.h"
#include "entity/Concept.h"
#include "UI/uiInfo.h"
#include "render/model.h"
#include "graphics/textureatlas.h"
#include "render/renderstats.h"
#include "bases/baseedit.h"
#include "bases/baseclientsend.h"
#include "entity/powers.h"
#include "UI/uiBaseInput.h"
#include "bases/bases.h"
#include "entity/entDebugPrivate.h"
#include "NovodeX/NwWrapper.h"
#include "group/groupnovodex.h"
#include "UI/uiPet.h"
#include "entity/teamCommon.h"
#include "entity/Supergroup.h"
#include "graphics/gfxSettings.h"
#include "graphics/sun.h"
#include "clientError.h"
#include "UI/uiLogin.h"
#include "gameData/costume_diff.h"
#include "entity/entPlayer.h"
#include <utilitieslib/components/StashTable.h>
#include <utilitieslib/components/StringTable.h>
#include "edit/ClickToSource.h"
#include "graphics/groupMiniTrackers.h"
#include "gameData/costume_critter.h"
#include "imageServer.h"
#include <utilitieslib/language/MessageStoreUtil.h>
#include "auth/authUserData.h"
#include "UI/uiCostume.h"
#include "UI/uiPowerInfo.h"
#include "UI/uiCombatMonitor.h"
#include "UI/uiCombatNumbers.h"
#include "UI/uiTextLink.h"
#include "UI/uiTraySingle.h"
#include "UI/uiLfg.h"
#include "UI/uiPlayerNote.h"
#include "UI/uiCustomWindow.h"
#include "formatter/smf_interact.h"
#include "graphics/seqgraphics.h"    //for CMD_CLEARMMTCACHE
#include "UI/uiMissionMakerScrollSet.h"
#include <utilitieslib/utils/osdependent.h>
#include "storyarc/playerCreatedStoryarcClient.h"
#include "uiLevelingpact.h"
#include "UI/uiMissionSearch.h"
#include "UI/uiClue.h"
#include "UI/uiMissionMaker.h"
#include "UI/uiPictureBrowser.h"
#include "mission/missionserver_meta.h"
#include "UI/uiPowerCust.h"
#include "entity/power_customization.h"
#include "entity/power_customization_client.h"
#include "render/cubemap.h"
#include "UI/uiPopHelp.h"
#include "player/pophelp.h"
#include "UI/uiEmail.h"
#include "UI/uiStoredSalvage.h"
#include "UI/uiTurnstile.h"
#include "render/renderWater.h"
#include "UI/uiUtilMenu.h"
#include "clientcomm/dbclient.h"
#include "UI/Hybrid/uiRedirect.h"
#include "UI/Hybrid/uiWebStoreFrame.h"
#include "LWC.h"
#include "graphics/gfxDevHUD.h"
#include <utilitieslib/utils/memtrack.h>
#include "player/inventory_client.h"
#include "account/AccountCatalog.h"
#include "UI/Hybrid/uiBody.h"
#include "bases/basedata.h"
#include "entity/origins.h"

GameState game_state;
_CrtMemState g_memstate;

extern AuthInfo    auth_info;
extern int    g_win_ignore_popups;

extern int gPrintShardCommCmds;
KeyBindProfile game_binds_profile;

static int    staticCmdTimeStamp;

char        bind_key_name[128], bind_command[256];

#ifndef FINAL
ClientDebugState g_client_debug_state;
#endif

//------------------------------------------------------------
// cmd enums
// -AB: added 2nd alt tray cmd  :2005 Jan 28 05:04 PM
// -AB: added goto_tray_alt/alt2, next/prev_tray_alt2 :2005 Feb 02 05:10 PM
//----------------------------------------------------------
enum
{
    CMD_SCREENSIZE = SVCMD_LASTCMD,
    CMD_STATESAVE,
    CMD_STATELOAD,
    CMD_STATEINIT,
    CMD_CMDS,
    CMD_CMDLIST,
    CMD_MAKE_MESSAGESTORE,
    CMD_CMDUSAGE,
    CMD_VARSET,
    CMD_BIND,
    CMD_SHOW_BIND,
    CMD_UNBIND,
    CMD_BIND_LOAD,
    CMD_BIND_LOAD_FILE,
    CMD_BIND_LOAD_FILE_SILENT,
    CMD_BIND_SAVE,
    CMD_BIND_SAVE_FILE,
    CMD_BIND_SAVE_FILE_SILENT,
    CMD_WDW_LOAD,
    CMD_WDW_LOAD_FILE,
    CMD_WDW_SAVE,
    CMD_WDW_SAVE_FILE,
    CMD_CHAT_LOAD,
    CMD_CHAT_LOAD_FILE,
    CMD_CHAT_SAVE,
    CMD_CHAT_SAVE_FILE,
    CMD_OPTION_LOAD,
    CMD_OPTION_LOAD_FILE,
    CMD_OPTION_SAVE,
    CMD_OPTION_SAVE_FILE,
    CMD_TEXUSAGE,
    CMD_TEXRES,
    CMD_VISSCALE,
    CMD_CLEARINPUT,
    CMD_CONNECT,
    CMD_EDIT,
    CMD_SETGEO,
    CMD_SETBODY,
    CMD_SETTEX,
    CMD_SETCOLOR,
    CMD_FIRE1,
    CMD_IMAGESERVER,
    CMD_PRINTSEQ,
    CMD_FXTIMERS,
    CMD_FXINFOOFF,
    CMD_RESETPARTHIGH,
    CMD_SHOWSTATE,
#if NOVODEX
    CMD_NX_DEBUG,
    CMD_NX_HARDWARE,
    CMD_NX_CLEAR_CACHE,
    CMD_NX_THREADED,
    CMD_NX_INIT_WORLD,
    CMD_NX_DEINIT_WORLD,
    CMD_NX_INFO,
    CMD_NX_COREDUMP,
    CMD_NX_PHYSX_CONNECT,
    CMD_NX_ENTCOLLISION,
#endif
    CMD_NOFILECHANGECHECK,
    CMD_CMALLOC,
    CMD_CAMRESET,
    CMD_CUBEMAPS,
    CMD_CLEAR_TT_CACHE,
    CMD_QUIT,
    CMD_QUIT_TO_LOGIN,
    CMD_QUIT_TO_CHARACTER_SELECT,
    CMD_VTUNE,
    CMD_PLAYSAMPLE,
    CMD_PLAYALLSOUNDS,
    CMD_PLAYMUSIC,
    CMD_PLAYVOICEOVER,
    CMD_STOPMUSIC,
    CMD_VOLUME,
    CMD_MUTE,
    CMD_UNMUTE,
    CMD_REPEATSOUND,
    CMD_RESET_SOUNDS,
    CMD_SOUND_DISABLE,
    CMD_DUCKVOLUME,
    CMD_FOLLOW,
    CMD_RELOAD_TEXT,
    CMD_LOCALE,
    CMD_EXEC,
    CMD_CONSOLE,
    CMD_NODEBUG,
    CMD_NOMINIDUMP,
    CMD_NOERRORREPORT,
    CMD_PACKETDEBUG,
    CMD_PACKETLOGGING,

    CMD_TARGET_ENEMY_NEAR,
    CMD_TARGET_ENEMY_FAR,
    CMD_TARGET_ENEMY_NEXT,
    CMD_TARGET_ENEMY_PREV,

    CMD_TARGET_FRIEND_NEAR,
    CMD_TARGET_FRIEND_FAR,
    CMD_TARGET_FRIEND_NEXT,
    CMD_TARGET_FRIEND_PREV,

    CMD_TARGET_CUSTOM_NEAR,
    CMD_TARGET_CUSTOM_FAR,
    CMD_TARGET_CUSTOM_NEXT,
    CMD_TARGET_CUSTOM_PREV,

    CMD_INTERACT,
    CMD_MOUSE_LOOK,
    CMD_INFO,
    CMD_INFO_TAB,
    CMD_INFO_SELF,
    CMD_INFO_SELF_TAB,
    CMD_START_CHAT,
    CMD_START_CHAT_PREFIX,
    CMD_SAY,
    CMD_CONSOLE_CHAT,
    CMD_UNSELECT,
    CMD_BONESCALE,
    CMD_TEST_BONE_SLIDERS,
    CMD_LOADMISSIONMAP,
    CMD_LOADMAP,
    CMD_COPYDEBUGPOS,
    CMD_PROFILER_RECORD,
    CMD_PROFILER_STOP,
    CMD_PROFILER_PLAY,
    CMD_RELOADENTDEBUGFILEMENUS,
    CMD_CAMERAFOLLOWENTITY,
    CMD_SELECTANYENTITY,
    CMD_CLEARDEBUGLINES,
    CMD_CLEARMMTCACHE,
    CMD_TOGGLEMMSTILL,
    CMD_MAPIMAGECREATE,
    CMD_MAPALLSHOW,
    CMD_PASTE_DEBUGGER_POS_0,
    CMD_PASTE_DEBUGGER_POS_1,
    CMD_PASTE_DEBUGGER_POS_2,
    CMD_NEXTFX,
    CMD_MEMTRACK_CAPTURE,
    CMD_MEMTRACK_MARK,
    CMD_MEM_CHECK,
    CMD_MEM_CHECKPOINT,
    CMD_MEM_DUMP,
    CMD_VERBOSE_PRINTF,
    CMD_NOWINIO,
    CMD_CAMDIST,
    CMD_CAMDISTADJUST,
    CMD_QUIT_TEAM,
    CMD_LEVEL,
    CMD_SAVETEX,
    CMD_NORMALMAP_TEST,
    CMD_GFXDEBUG,
    CMD_RENDERSCALE,
    CMD_RENDERSIZE,
    CMD_ENABLEVBOS,
    CMD_NOVBOS,
    CMD_MAX_TEXUNITS,
    CMD_SHADER_DETAIL,
    CMD_OLD_VIDEO_TEST,
    CMD_GFX_DEV_HUD,
    CMD_SHADER_TEST,
    CMD_SHADER_TEST_SPEC_ONLY,
    CMD_SHADER_TEST_DIFFUSE_ONLY,
    CMD_SHADER_TEST_NORMALS_ONLY,
    CMD_SHADER_TEST_TANGENTS_ONLY,
    CMD_SHADER_TEST_BASEMAT_ONLY,
    CMD_SHADER_TEST_BLENDMAT_ONLY,
    CMD_DEMO_CONFIG,
    CMD_TOGGLE_USECG,
    CMD_TOGGLE_DXT5NM,
    CMD_SHOW_DOF,
    CMD_DOF_TEST,
    CMD_GFX_TEST,
    CMD_USEHDR,
    CMD_USEDOF,
    CMD_USEBUMPMAPS,
    CMD_USEHQ,
    CMD_USECUBEMAP,
    CMD_USEFP,
    CMD_USEFANCYWATER,
    CMD_SHADER_PERF_TEST,
    CMD_UNLOAD_GFX,
    CMD_REBUILD_MINITRACKERS,
    CMD_REDUCEMIP,
    CMD_TEXANISO,
    CMD_ANTIALIASING,
    CMD_UNLOAD_MODELS,
    CMD_RELOAD_TRICKS,
    CMD_TIMESTEPSCALE,
    CMD_TIMEDEBUG,
    CMD_TEXWORDSLOAD,
    CMD_TEXWORDEDITOR,
    CMD_TEXWORDFLUSH,
    CMD_MEMCHECK,
    CMD_MMONITOR,
    CMD_MEMORYUSEDUMP,
    CMD_MEMLOG_ECHO,
    CMD_MEMLOG_DUMP,
    CMD_ECHO,
    CMD_STRINGTABLE_MEM_DUMP,
    CMD_STASHTABLE_MEM_DUMP,
    CMD_SHOW_LOAD_MEM_USAGE,
    CMD_COMETOME,
    CMD_GETPOS,
    CMD_WHEREAMI,
    CMD_CONTACTDIALOG_CLOSE,
    CMD_ACTIVATE_INSPIRATION_SLOT,
    CMD_NO_THREAD,
    CMD_CONTEXT_MENU,
    CMD_SUPERGROUP_REGISTER,
    CMD_START_MENU,
    CMD_TAILOR_MENU,
    CMD_ULTRA_TAILOR_MENU,
    CMD_WEB_BROWSER_MENU,
    CMD_WEB_BROWSER_GOTO_URL,
    CMD_WEB_BROWSER_GET_WEB_RESET_DATA,
    CMD_LEVELUP,
    CMD_SHOWCONTACTLIST,
    CMD_SHOWTASKLIST,
    CMD_PERFTEST,
    CMD_PERFTESTSUITE,
    CMD_NOSHAREDMEMORY,
    CMD_QUICKLOAD,
    CMD_ASSERT,
    CMD_REPLY,
    CMD_POPMENU,
    CMD_QUICKCHAT,
    CMD_AUCTIONHOUSE_WINDOW,
    CMD_MISSIONSEARCH_WINDOW,
    CMD_MISSIONMAKER_WINDOW,
    CMD_MISSIONSEARCH_CHOICE,
    CMD_QUICKPLACEONME,
    CMD_DOORFADE,
    CMD_SETFADE,
    CMD_ENTERDOOR,
    CMD_ENTERDOORVOLUME,
    CMD_ENTERDOORFROMSGID,
    CMD_DEMOPLAY,
    CMD_DEMORECORD,
    CMD_DEMORECORD_AUTO,
    CMD_DEMOSTOP,
    CMD_DEMODUMPTGA,
    CMD_NOP,
    CMD_MACRO,
    CMD_MACRO_SLOT,
    CMD_MANAGE_ENHANCEMENTS,
    CMD_SCREENSHOT,
    CMD_RELOADSEQUENCERS,
    CMD_SHOULDERSCALE,
    CMD_SETDEBUGENTITY,
    CMD_SCRIPTDEBUG_SET,
    CMD_SCRIPTDEBUG_VARTOP,
    CMD_POWEXEC_NAME,
    CMD_POWEXEC_LOCATION,
    CMD_POWEXEC_SLOT,
    CMD_POWEXEC_TOGGLE_ON,
    CMD_POWEXEC_TOGGLE_OFF,
    CMD_POWEXEC_ALTSLOT,
    CMD_POWEXEC_SERVER_SLOT,
    CMD_POWEXEC_TRAY,
    CMD_POWEXEC_AUTO,
    CMD_POWEXEC_ABORT,
    CMD_POWEXEC_UNQUEUE,
    CMD_INSPEXEC_SLOT,
    CMD_INSPEXEC_TRAY,
    CMD_INSPEXEC_NAME,
    CMD_INSPEXEC_PET_NAME,
    CMD_INSPEXEC_PET_TARGET,
    CMD_MINIMAP_RELOAD,
    CMD_LOGFILEACCESS,
    CMD_WINDOW_RESETALL,
    CMD_WINDOW_SHOW,
    CMD_WINDOW_SHOW_DBG,
    CMD_WINDOW_HIDE,
    CMD_WINDOW_TOGGLE,
    CMD_MAIL_VIEW,
    CMD_WINDOW_NAMES,
    CMD_WINDOW_COLOR,
    CMD_WINDOW_CHAT,
    CMD_WINDOW_TRAY,
    CMD_WINDOW_TARGET,
    CMD_WINDOW_NAV,
    CMD_WINDOW_MAP,
    CMD_WINDOW_MENU,
    CMD_CHAT_OPTIONS_MENU,
    CMD_PET_OPTIONS_MENU,
    CMD_WINDOW_POWERS,
    CMD_FULLSCREEN,
    CMD_TTDEBUG,
    CMD_COPYCHAT,
    CMD_BUG,
    CMD_BUG_INTERNAL,
    CMD_CBUG,
    CMD_CSR_BUG,
    CMD_QA_BUGREPORT,
    CMD_MISSIONMAP,
    CMD_MAP_FOG,
    CMD_TRAYTOGGLE,
    CMD_TRAY_NEXT,
    CMD_TRAY_PREV,
    CMD_TRAY_NEXT_ALT,
    CMD_TRAY_PREV_ALT,
    CMD_TRAY_GOTO,
    CMD_TEAM_SELECT,
    CMD_PET_SELECT,
    CMD_PET_SELECT_NAME,
    CMD_SOUVENIRCLUE_PRINT,
    CMD_SOUVENIRCLUE_GET_DETAIL,
    CMD_RELEASE,
    CMD_PETITION,
    CMD_COSTUME_CHANGE,
    CMD_EMOTE_COSTUME_CHANGE,
    CMD_CHAT_CHANNEL_CYCLE,
    CMD_CHAT_CHANNEL_SET,
    CMD_CHAT_HIDE_PRIMARY_CHAT,
    CMD_SCREENSHOT_TGA,
    CMD_SHARDCOMM_CREATE,
    CMD_SHARDCOMM_JOIN,
    CMD_SHARDCOMM_LEAVE,
    CMD_SHARDCOMM_INVITEDENY,
    CMD_SHARDCOMM_USER_MODE,
    CMD_SHARDCOMM_CHAN_MODE,
    CMD_SHARDCOMM_CHAN_LIST,
    CMD_SHARDCOMM_CHAN_INVITE_TEAM,
    CMD_SHARDCOMM_CHAN_INVITE_GF,
    CMD_SHARDCOMM_WATCHING,
    CMD_SHARDCOMM_CHAN_MOTD,
    CMD_SHARDCOMM_CHAN_DESC,
    CMD_SHARDCOMM_CHAN_TIMEOUT,
    CMD_SHARDCOMM_GIGNORELIST,
    CMD_SHARDCOMM_NAME,
    CMD_SHARDCOMM_FRIENDS,
    CMD_SHARDCOMM_WHOLOCAL,
    CMD_SHARDCOMM_WHOLOCALINVITE,
    CMD_SHARDCOMM_WHOLOCALLEAGUEINVITE,
    CMD_MYHANDLE,
    CMD_TAB_CREATE,
    CMD_TAB_CLOSE,
    CMD_TAB_TOGGLE,
    CMD_TAB_NEXT,
    CMD_TAB_PREV,
    CMD_TAB_GLOBAL_NEXT,
    CMD_TAB_GLOBAL_PREV,
    CMD_TAB_SELECT,
    CMD_CHAT_WINDOW_DUMP,
    CMD_WINDOW_SET_SCALE,
    CMD_CHATLOG,
    CMD_ASSIST,
    CMD_ASSIST_NAME,
    CMD_TARGET_NAME,
    CMD_DIALOGYES,
    CMD_DIALOGNO,
    CMD_DIALOGANSWER,
    CMD_COSTUME_SAVE,
    CMD_COSTUME_LOAD,
    CMD_COSTUME_RELOAD,
    CMD_SKILLS_ACTIVE,
    CMD_CLICKTOMOVE,
    CMD_CLICKTOMOVE_TOGGLE,
    CMD_TARGET_CONFUSION,
    CMD_POWEXEC_ALT2SLOT,
    CMD_TRAY_MOMENTARY_ALT_TOGGLE,
    CMD_TRAY_MOMENTARY_ALT2_TOGGLE,
    CMD_TRAY_GOTO_TRAY,
    CMD_TRAY_NEXT_TRAY,
    CMD_TRAY_PREV_TRAY,
    CMD_TRAY_GOTO_ALT,
    CMD_TRAY_GOTO_ALT2,
    CMD_TRAY_NEXT_ALT2,
    CMD_TRAY_PREV_ALT2,
    CMD_TRAY_STICKY,
    CMD_TRAY_STICKY_ALT2,
    CMD_TRAY_CLEAR,
    CMD_SALVAGEDEFS_LIST,
    CMD_CONCEPTDEFS_LIST,
    CMD_RECIPEDEFS_LIST,
    CMD_SALVAGEINV_LIST,
    CMD_CONCEPTINV_LIST,
    CMD_RECIPEINV_LIST,
    CMD_DETAILINV_LIST,
    CMD_SALVAGE_REVERSE_ENGINEER,
    CMD_INVENT_SELECT_RECIPE,
    CMD_INVENT_SLOT_CONCEPT,
    CMD_INVENT_UNSLOT,
    CMD_INVENT_HARDEN,
    CMD_INVENT_FINALIZE,
    CMD_INVENT_CANCEL,
    CMD_ENHANCEMENT_LIST,
    CMD_INVENTION_LIST,
    CMD_UIINVENTORY_VISIBILITY,
    CMD_VERSION,
    CMD_ATLAS_DISPLAY_NEXT,
    CMD_ATLAS_DISPLAY_PREV,
    CMD_ADD_CARD_STATS,
    CMD_REM_CARD_STATS,
    CMD_ROOMHEIGHT,
    CMD_DOORWAY,
    CMD_MAKEROOM,
    CMD_ROOMLOOK,
    CMD_ROOMLIGHT,
    CMD_ROOMDETAIL,
    CMD_SIMPLESTATS,
    CMD_EDIT_BASE,
    CMD_BASE_WORKSHOP_SETMODE, // start/stop
    CMD_BASE_WORKSHOP_RECIPES_LIST, // show valid
    CMD_BASE_WORKSHOP_RECIPE_CREATE, // create by name
    CMD_LOCAL_TIME,
    CMD_ARENA_MANAGEPETS,
    CMD_PETCOMMAND,
    CMD_PETCOMMAND_BY_NAME,
    CMD_PETCOMMAND_BY_POWER,
    CMD_PETCOMMAND_ALL,
    CMD_PETSAY,
    CMD_PETSAY_BY_NAME,
    CMD_PETSAY_BY_POWER,
    CMD_PETSAY_ALL,
    CMD_PETRENAME,
    CMD_PETRENAME_BY_NAME,
    CMD_DEBUGSEAMS,
    CMD_SCREENSHOT_TITLE,
    CMD_CREATEHERO,
    CMD_CREATEVILLAIN,
    CMD_MARKPOWOPEN,
    CMD_MARKPOWCLOSED,
    CMD_HIDETRANS,
    CMD_RESET_SEPS,
    CMD_RECALCLIGHTS,
    CMD_RECALCWELDS,
    CMD_DUMPPARSERALLOCS,
    CMD_TEXTPARSERDEBUG,
    CMD_TOGGLEFIXEDFUNCTIONFP,
    CMD_TOGGLEFIXEDFUNCTIONVP,
    CMD_COSTUME_DIFF,
    CMD_CAMTURN,
    CMD_PLAYERTURN,
    CMD_FACE,
    CMD_WINDOW_CLOSE_EXTRA,
    CMD_CLEARCHAT,
    CMD_RESET_KEYBINDS,
    CMD_CRITTER_CYCLE,
    CMD_CTS_TOGGLE,
    CMD_SAVE_CSV,
    CMD_LOAD_CSV,
    CMD_TRAININGROOM_CLIENT,
    CMD_AUTH_LOCAL_SHOW,
    CMD_AUTH_LOCAL_SET,
    CMD_CRASHCLIENT,
    CMD_UI_REGION_SQUISH,
    CMD_PERSONALSTORAGE_WINDOW,
    CMD_BUYCOH,
    CMD_MONITORATTRIBUTE,
    CMD_STOPMONITORATTRIBUTE,
    CMD_DELETEINSP,
    CMD_COMBINEINSP,
    CMD_CHATLINK,
    CMD_TEXTLINK,
    CMD_NEWTRAY,
    CMD_CHATLINKGLOBAL,
    CMD_CHATLINKCHANNEL,
    CMD_HIDE,
    CMD_HIDE_SEARCH,
    CMD_HIDE_SG,
    CMD_HIDE_FRIENDS,
    CMD_HIDE_GFRIENDS,
    CMD_HIDE_GCHANNELS,
    CMD_UNHIDE,
    CMD_UNHIDE_SEARCH,
    CMD_UNHIDE_SG,
    CMD_UNHIDE_FRIENDS,
    CMD_UNHIDE_GFRIENDS,
    CMD_UNHIDE_GCHANNELS,
    CMD_HIDE_ALL,
    CMD_UNHIDE_ALL,
    CMD_HIDE_TELL,
    CMD_UNHIDE_TELL,
    CMD_HIDE_INVITE,
    CMD_UNHIDE_INVITE,
    CMD_SET_POWERINFO_CLASS,
    CMD_SGKICK,
    CMD_MOUSE_SPEED,
    CMD_MOUSE_INVERT,
    CMD_SPEED_TURN,
    CMD_OPTION_LIST,
    CMD_OPTION_SET,
    CMD_OPTION_TOGGLE,
    CMD_PLAYER_NOTE,
    CMD_PLAYER_NOTE_LOCAL,
    CMD_TEST_LOGIN_FAIL,
    CMD_CUSTOM_WINDOW,
    CMD_CUSTOM_TOGGLE,
    CMD_TEST_MISSIONMAKERCONTACTCOSTUME,
    CMD_MISSIONMAKER_NAV,
    CMD_MISSIONMAKER_NAV2,
    CMD_LEVELINGPACT_QUIT,
    CMD_ARCHITECTSAVETEST,
    CMD_ARCHITECTSAVEEXIT,
    CMD_ARCHITECTFIXERRORS,
    CMD_ARCHITECTEXIT,
    CMD_ARCHITECTREPUBLISH,
    CMD_ARCHITECT_CSRSEARCH_AUTHORED,
    CMD_ARCHITECT_CSRSEARCH_COMPLAINED,
    CMD_ARCHITECT_CSRSEARCH_VOTED,
    CMD_ARCHITECT_CSRSEARCH_PLAYED,
    CMD_ARCHITECT_GET_FILE,
    CMD_ARCHITECT_TOGGLE_SAVE_COMPRESSION,
    CMD_ARCHITECT_IGNORE_UNLOCKABLE_VALIDATION,
    CMD_SMF_DRAWDEBUGBBOXES,
    CMD_GENDER_MENU,
    CMD_DEBUG_BUILD,
    CMD_OPEN_CUSTOM_VG,
    CMD_SHOW_INFO,
    CMD_OPEN_HELP,
    CMD_MAKEMAPTEXTURES,
    CMD_TESTPOWERCUST,
    CMD_SET_CG_SHADER_PATH,
    CMD_CREATEPRAETORIAN,
    CMD_UISKIN,
    CMD_RELOAD_POP_HELP,
    CMD_SCRIPT_WINDOW,
    CMD_DETACHED_CAMERA,
    CMD_EMAILSEND,
    CMD_EMAILSENDATTACHMENT,
    CMD_TURNSTILE_REQUEST_EVENT_LIST,
    CMD_TURNSTILE_QUEUE_FOR_EVENTS,
    CMD_TURNSTILE_REMOVE_FROM_QUEUE,
    CMD_TURNSTILE_EVENT_RESPONSE,
    CMD_NAGPRAETORIAN,
    CMD_CLEAR_NDA_SIGNATURE,
    CMD_CREATE_ACCESS_KEY,
    CMD_TEST_ACCESS_KEY,
    CMD_HEADSHOT_CACHE_CLEAR,
    CMD_REDIRECT,
    CMD_LWCDEBUG_SET_DATA_STAGE,
    CMD_LWCDEBUG_MODE,
    CMD_LWCDOWNLOADRATE,
    CMD_WEB_STORE,
    CMD_WEB_STORE_OPEN_PRODUCT,
    CMD_ACCOUNT_DEBUG_BUY_PRODUCT,
    CMD_ACCOUNT_QUERY_INVENTORY,
    CMD_ACCOUNT_QUERY_INVENTORY_ACTIVE,
    CMD_CATALOG_DEBUG_PUBLISH_PRODUCT,
    CMD_QUERY_DEBUG_PUBLISH_PRODUCT,
    CMD_ACCOUNT_LOYALTY_BUY,
    CMD_SHOW_PURCHASES,
    CMD_TEST_CAN_START_STATIC_MAP,
    CMD_SHOW_OPTIONS,
    CMD_FORCE_CUT_SCENE_LETTERBOX,
    CMD_SET_SERVERGROUP,
    CMD_MACRO_IMAGE,
	CMD_SG_ENTER_PASSCODE,
};

#define MAX_SCROLL_OUT_DIST 80.0

extern AudioState g_audio_state;
static int cmd_access_level;

static int    tmp_int,tmp_int2,tmp_int3,tmp_int4;
static F32    tmp_f32,tmp_f322,tmp_f323,tmp_f324,tmp_f325;
static char tmp_str[1000], tmp_str2[1000], tmp_str3[1000], tmp_str4[256];
static Vec3 tmp_vec,tmp_vec2;

static int tmp_int_special[9] = {0}; // these are tmp_ints that are unique to a command, but don't need a value in game_state.

static void *tmp_var_list[] = { // List of tmp_* vars in order to automatically add CMDF_HIDEPRINT
    &tmp_int,&tmp_int2,&tmp_int3,&tmp_int4,
    &tmp_f32, &tmp_f322, &tmp_f323, &tmp_f324, &tmp_f325,
    &tmp_str, &tmp_str2, &tmp_str3, &tmp_str4,
    &tmp_vec, &tmp_vec2,
};

static int    paste_debug_pos_state;
static Vec3 paste_debug_pos;

int    mem_check_frequency;

int g_tweakui = 0;

extern int timing_force_server_tz;

#ifndef FINAL
extern int g_force_production_mode;
extern int g_force_qa_mode;
#endif

extern F32 gClickToMoveDist;
extern int gClickToMoveButton;
extern F32 gfTargetConfusion;
extern int g_mapSepType;
extern int gShowHiddenDetails;
void resetSeps(void);

extern F32 g_BeaconDebugRadius;

Cmd game_cmds[] =
{    // Name, optional command (0 for NADA), followed by up to 4 data fields, using types and address.,
    { 0, "fullscreen",    CMD_FULLSCREEN, { {CMDINT(game_state.fullscreen) }},0,
                        "Sets video mode to fullscreen."  },
    { 9, "fullscreen_toggle",    0, { {CMDINT(game_state.fullscreen_toggle) }},0,
                        "Toggles video mode to/from fullscreen."  },
#ifndef FINAL
    { 9, "productionmode",    0, { {CMDINT(g_force_production_mode) }},0,
                        "Force usage of production mode when using development files."  },
    { 1, "qamode",    0, { {CMDINT(g_force_qa_mode) }},0,
                        "Force usage of QA mode."  },
#endif
    { 0, "maximize",    0, { {CMDINT(game_state.maximized) }},0,
                        "Maximizes the window."  },
    { 0, "disable2D",    0, { {CMDINT(game_state.disable2Dgraphics) }},0,
                        "Disables 2D sprite drawing."  },
    { 9, "ttdebug",        CMD_TTDEBUG, {{CMDINT(tmp_int)}}, CMDF_HIDEVARS,
                        "enable true type debugging" },
    { 9, "ortho",        0, { {CMDINT(game_state.ortho) } }, 0,
                        "puts game into orthographic mode" },
    { 0, "maxrtframes", 0, { {CMDINT(game_state.allow_frames_buffered) }},0,
                        "How many frames ahead to allow buffering."  },
    { 0, "screen", CMD_SCREENSIZE, { {CMDINT(game_state.screen_x) }, { CMDINT(game_state.screen_y)} },0,
                        "Sets X and Y screen dimensions. Should be constrained to 640x480,1024x768, 1280x1024, 1600x1200, etc"  },
    { 0, "quit", CMD_QUIT, {{0}},0,
                        "Quits game." },
    { 0, "quittologin", CMD_QUIT_TO_LOGIN, {{0}},0,
                        "quits to login screen" },
    { 0, "quittocharacterselect", CMD_QUIT_TO_CHARACTER_SELECT, {{0}}, 0,
                        "quits to character select" },
    { 9, "stateload", CMD_STATELOAD, {{0}},0,
                        "loads keyboard binds" },
    { 9, "stateinit", CMD_STATEINIT, {{0}},0,
                        "initializes keyboard binds" },
    { 1, "cmds", CMD_CMDS, {{CMDSTR(tmp_str)}},CMDF_HIDEVARS,
                        "prints out client commands for commands containing <string>" },
    { 9, "cmdms", CMD_MAKE_MESSAGESTORE, {{0}}, 0,
                        "Makes Command Message store for translators" },
    { 0, "cmdlist", CMD_CMDLIST, {{0}}, 0,
                        "Prints out all commands available." },
    { 2, "cmdusage", CMD_CMDUSAGE, {{CMDSTR(tmp_str)}},CMDF_HIDEVARS,
                        "prints out usage of client commands for commands containing <string>" },
    { 1, "var", CMD_VARSET, {{CMDINT(tmp_int)}, {CMDSENTENCE(tmp_str)}},CMDF_HIDEVARS,
                        "Sets a string variable which can be referenced via %#" },
    { 0, "bind", CMD_BIND, {{ CMDSTR(bind_key_name)}, { CMDSENTENCE(bind_command)}},0,
                        "Binds a key to a command - 'bind k ++forward' will set k to toggle running forward." },
    { 0, "show_bind", CMD_SHOW_BIND, {{ CMDSTR(bind_key_name)}}, 0,
                        "Returns the command currently binded to the key."},
    { 0, "unbind", CMD_UNBIND, {{ CMDSTR(bind_key_name)}},0,
                        "Unbinds a bound key (sets it to default).  If the default action is not desired, use: /bind <keyname> \"nop\" " },
    { 0, "bind_load_file", CMD_BIND_LOAD_FILE, {{ CMDSTR(tmp_str) }}, 0,
                        "Reads a list of keybinds from a file." },
    { 0, "bind_load_file_silent", CMD_BIND_LOAD_FILE_SILENT, {{ CMDSTR(tmp_str) }}, 0,
                        "Reads a list of keybinds from a file." },
    { 0, "bind_load", CMD_BIND_LOAD, {{ 0 }}, 0,
                        "Reads a list of keybinds from <INSTALL DIR>/keybinds.txt." },
    { 0, "bind_save", CMD_BIND_SAVE, {{ 0 }}, 0,
                        "Saves all keybinds to <INSTALL DIR>/keybinds.txt." },
    { 0, "bind_save_file", CMD_BIND_SAVE_FILE, {{ CMDSTR(tmp_str) }}, 0,
                        "Saves all keybinds to specified file." },
    { 0, "bind_save_file_silent", CMD_BIND_SAVE_FILE_SILENT, {{ CMDSTR(tmp_str) }}, 0,
                        "Saves all keybinds to specified file." },
    { 0, "wdw_load_file", CMD_WDW_LOAD_FILE, {{ CMDSTR(tmp_str) }}, 0,
                        "Reads window configuration file." },
    { 0, "wdw_load", CMD_WDW_LOAD, {{ 0 }}, 0,
                        "Reads window configuration from <INSTALL DIR>/wdw.txt." },
    { 0, "wdw_save", CMD_WDW_SAVE, {{ 0 }}, 0,
                        "Saves window configuration to <INSTALL DIR>/wdw.txt." },
    { 0, "wdw_save_file", CMD_WDW_SAVE_FILE, {{ CMDSTR(tmp_str) }}, 0,
                        "Saves window configuration to specified file." },
    { 0, "option_load_file", CMD_OPTION_LOAD_FILE, {{ CMDSTR(tmp_str) }}, 0,
        "Reads option configuration file." },
    { 0, "option_load", CMD_OPTION_LOAD, {{ 0 }}, 0,
        "Reads option configuration from <INSTALL DIR>/options.txt." },
    { 0, "option_save", CMD_OPTION_SAVE, {{ 0 }}, 0,
        "Saves window configuration to <INSTALL DIR>/options.txt." },
    { 0, "option_save_file", CMD_OPTION_SAVE_FILE, {{ CMDSTR(tmp_str) }}, 0,
        "Saves option configuration to specified file." },
    { 0, "chat_load_file", CMD_CHAT_LOAD_FILE, {{ CMDSTR(tmp_str) }}, 0,
                        "Reads chat configuration file." },
    { 0, "chat_load", CMD_CHAT_LOAD, {{ 0 }}, 0,
                        "Reads chat configuration from <INSTALL DIR>/chat.txt." },
    { 0, "chat_save", CMD_CHAT_SAVE, {{ 0 }}, 0,
                        "Saves chat configuration to <INSTALL DIR>/chat.txt." },
    { 0, "chat_save_file", CMD_CHAT_SAVE_FILE, {{ CMDSTR(tmp_str) }}, 0,
                        "Saves chat configuration to specified file." },
    { 1, "debug_info", 0, {{ CMDINT(game_state.info) }},0,
                        "<0/1> 1=turns on printing debug info like current position, polys draw, etc." },
    { 9, "texusage", CMD_TEXUSAGE, { {CMDSTR(tmp_str)}},CMDF_HIDEVARS,
                        "prints list and sizes of all loaded textures that match the given string. use \"\" to match all"  },
    { 9, "texusage_res", CMD_TEXRES, { {CMDINT(tmp_int)}, {CMDINT(tmp_int2)}},CMDF_HIDEVARS,
                    "prints list and sizes of all loaded textures that are equal to or larger than the given resolution (width x height)."  },
    { 9, "seq_info", 0, {{ CMDINT(game_state.seq_info) }},0,
                        "prints out basic sequencer info for ents playing on client." },
    { 9, "fov", 0, {{ PARSETYPE_FLOAT, &game_state.fov_1st }},0,
                        "field of view in degrees" },
    { 9, "fov_3rd", 0, {{ PARSETYPE_FLOAT, &game_state.fov_3rd }},0,
                        "third person field of view in degrees" },
    { 9, "floor_info", 0, {{ CMDINT(game_state.floor_info) }},0,
                        "debug command to let you know stats of the surface poly you're standing on" },
    { 9, "wireframe", 0, {{ CMDINT(game_state.wireframe), 3 }},0,
                        "Draws polys in three different wireframe modes" },
    { 9, "ambient_info", 0, {{ CMDINT(game_state.ambient_info) }},0,
                        "prints list of currently audible ambient sounds" },
    { 9, "sound_info", 0, {{ CMDINT(game_state.sound_info) }},0,
                        "prints list of currently playing sounds" },
    { 9, "surround_sound", 0, {{ CMDINT(game_state.surround_sound) }},0,
                        "sets sound to 2d sound or 3d sound" },
    { 9, "nearz", 0, {{ PARSETYPE_FLOAT, &game_state.nearz}},0,
                        "sets near plane Z for testing" },
    { 9, "farz", 0, {{ PARSETYPE_FLOAT, &game_state.farz}},0,
                        "sets far plane Z for testing" },
    { 0, "netgraph", 0, {{ CMDINT(game_state.netgraph) }},0,
                        "Displays network connection information." },
    { 9, "bitgraph", 0, {{ CMDINT(game_state.bitgraph) }},0,
                        "Testing packed bits." },
    { 9, "netfloaters", 0, {{ CMDINT(game_state.netfloaters) }},0,
                        "displays bandwidth usage per entity as floaters" },
    { 0, "maxfps", 0, {{ CMDINT(game_state.maxfps) }},0,
                        "Limits max frames per second." },
    { 0, "maxInactiveFps", 0, {{ CMDINT(game_state.maxInactiveFps)}},0,
                        "Limits max frames per second while the game is not in the foreground." },
    { 0, "maxMenuFps", 0, {{ CMDINT(game_state.maxMenuFps)}},0,
						"Limits max frames per second while the game is in a fullscreen menu." },
    { 0, "showfps", 0, {{ PARSETYPE_FLOAT, &game_state.showfps }},0,
                        "Show current framerate (1 = on, 0 = off)." },
    { 9, "showActiveVolume", 0, {{ CMDINT(game_state.showActiveVolume) }},0,
                        "Show current material volume (1 = on, 0 = off)." },
    { 0, "graphfps", 0, {{ CMDINT(game_state.graphfps) }},0,
                        "Graph current framerate (1 = SWAP, 2 = GPU, 4 = CPU, 8 = SLI)." },
    { 0, "sliClear", 0, {{ CMDINT(game_state.sliClear) }},0,
                        "Clear each FBO the first time it is used in the frame to help SLI/CF (0 to disable)." },
    { 0, "sliFBOs", 0, {{ CMDINT(game_state.sliFBOs) }},0,
                        "Number of SLI/CF framebuffers to allocate (1 to disable)." },
    { 0, "sliLimit", 0, {{ CMDINT(game_state.sliLimit) }},0,
                        "Limit number of SLI/CF frames to allow in parallel (0 to disable limiter)." },
    { 0, "usenvfence", 0, {{ CMDINT(game_state.usenvfence) }},0,
                        "Use NV fences instead of ARB queries." },
    { 1, "gpumarkers", 0, {{ CMDINT(game_state.gpu_markers) }},0,
                        "Enable GPU markers for profiling OpenGL state." },
    { 9, "framedelay", 0, {{ CMDINT(game_state.frame_delay) }}, 0,
                        "add a delay to each frame, slows down time on client" },
    { 9, "edit", CMD_EDIT, {{ CMDINT(game_state.edit) }},0,
                        "puts game in edit mode if can_edit=1" },
    { 1, "version",        CMD_VERSION, {{0}}, 0,
                        "print exe version" },
    { 9, "fire1",        CMD_FIRE1,{{0}},0,
                        "Fire the fx in your tfx.txt file.  For artists working on fx with no corresponding power yet" },
    { 0, "imageServer", CMD_IMAGESERVER,{ {CMDSTR(game_state.imageServerSource)}, { CMDSTR(game_state.imageServerTarget)} },CMDF_HIDEPRINT,
                        "Sets game to Image Server mode.  Next two strings are the source directory and the target directory" },
    { 9, "imageServerDebug", 0,{{ CMDINT(game_state.imageServerDebug) }},0,
                        "Debugging flag for image server, forces graphics buffer flip" },
    { 0, "notga", 0, {{ CMDINT(game_state.noSaveTga) }},CMDF_HIDEPRINT,
                        "Disables saving of .TGA files in image server mode" },
    { 0, "nojpg", 0, {{ CMDINT(game_state.noSaveJpg)}},CMDF_HIDEPRINT,
                        "Disables saving of .JPG files in image server mode" },
    { 9, "fxpower", 0, {{ PARSETYPE_FLOAT, &game_state.fxpower }},0,
                        "woomer testing fx" },
    { 9, "martinboxes", 0, {{ CMDINT(game_state.martinboxes) }},0,
                        "turn everybody into a box" },
    { 9, "rph",  CMD_RESETPARTHIGH,{{0}},0,
                        "reset highest particle count counter (active when fxdebug is true)(bound to '.')" },
    { 9, "nextfx",  CMD_NEXTFX,{{0}},0,
                        "show details of next fx system in debug area" },
    { 9, "bonescale",  CMD_BONESCALE, {{ PARSETYPE_FLOAT, &game_state.bonescale }},0,
                        "force a bonescale ratio value onto the player. -1.0 to 1.0" },
    { 9, "nopredictplayerseq", 0, {{ CMDINT(game_state.nopredictplayerseq) }},0,
                        "set to 1 to disable client-side animation prediction." },
    { 9, "nointerpolation", 0, {{ CMDINT(game_state.nointerpolation) }},0,
                        "set to 1 to disable interpolation between animations and between keyframes" },
    { 9, "fxtimers",    CMD_FXTIMERS, {{ CMDINT(tmp_int) }}, 0,
                        "enable individual fx autotimers" },
    { 9, "fxinfooff",      CMD_FXINFOOFF,{{0}},0,
                        "disable on screen error log (bound to '/') " },
    { 9, "fxinfo",  0, {{ CMDINT(game_state.fxinfo) }},0,
                        "set state of on screen error log. set to 0 for only very important errors, 1 for all fx log info, 2 for disable altogether" },
    { 9, "checkcostume",  0, {{ CMDINT(game_state.checkcostume) }},0,
                        "print costume ent 1 == currently selected, 2 == me " },
    { 9, "checklod",  0, {{ CMDINT(game_state.checklod) }},0,
                        "print costume ent 1 == currently selected, 2 == me " },
    { 9, "checkvolume",  0, {{ CMDINT(game_state.checkVolume) }},0,
                        "show volumes entiti is in: 1 == currently selected, 2 == me " },
    { 9, "fxdebug",  0, {{ CMDINT(game_state.fxdebug) }},0,
                        "set to 1 to enable fx debug info and reloading of changed fx data files" },
    { 9, "fxontarget",  0, {{ CMDINT(game_state.fxontarget) }},0,
                        "places debug FX on target instead of self" },
    { 9, "cameraDebug",  0, {{ CMDINT(game_state.cameraDebug) }},0,
                        "1 Draw Camera shadow + 2 Draw Camera rays + 4 Print cam data + 8 Disable Camera Y sping + 16 Fix Cam pos with cameraDebugPos + 32 Old Cam" },
    { 9, "cameraDebugPos",  0, {{ PARSETYPE_FLOAT, &game_state.cameraDebugPos[0] },{ PARSETYPE_FLOAT, &game_state.cameraDebugPos[1] },{ PARSETYPE_FLOAT, &game_state.cameraDebugPos[2] }},0,
                        "Position relative to character to fix the camera (use with cameraDebug & 4) used so you can see cameraDebug visualization from another perspective" },
    { 9, "selectionDebug",  0, {{ CMDINT(game_state.selectionDebug) }},0,
                        "1 Prints info on what the selection code is thinking" },
    { 9, "maxparticles",  0, {{ CMDINT(game_state.maxParticles)}},0,
                        "sets the max number of particles" },
    { 9, "animdebug",  0, {{ CMDINT(game_state.animdebug) }},0,
                        "various animation debug flags" },
    { 9, "frameGrab",  0, {{ CMDINT(game_state.framegrab) }},0,
                        "grap a frame" },
    { 9, "frameGrabShow",  0, {{ PARSETYPE_FLOAT, &game_state.framegrab_show }},0,
                        "show grabbed frame" },
    { 9, "renderdump", 0, {{ CMDINT(game_state.renderdump) }},0,
                        "dump the render buffers from various points in the renderer" },
    { 9, "gamma",  0, {{ PARSETYPE_FLOAT, &game_state.gamma}},0,
                        "sets gamma" },
    { 9, "savetex",  CMD_SAVETEX, {{ CMDSTR(tmp_str) }},0,
                        "set to 1 to enable gfx debug info" },
    { 9, "hdrthumb",  0, {{ CMDINT(game_state.hdrThumbnail) }},0,
                        "show HDR thumbnails" },
    { 9, "showalpha",  0, {{ CMDINT(game_state.tintAlphaObjects) }},0,
                        "tints alpha sorted objects red" },
    { 9, "showoccluders",  0, {{ CMDINT(game_state.tintOccluders) }},0,
                        "tints occluders green, potential occluders blue, and non-occluders red" },
    { 9, "scaleEntityAmbient",  0, {{ CMDINT(game_state.scaleEntityAmbient) }},0,
                        "Scale, rather than clamp, entity ambient lights that exceed the maximum ambient." },
    { 9, "maxEntAmbient", 0, {{ PARSETYPE_FLOAT, &game_state.maxEntAmbient}},0,
                        "Maximum entity ambient in the range.  Defaults to 0.2" },
    { 9, "specLightMode",  0, {{ CMDINT(game_state.specLightMode) }},0,
                        "0 = [1,1,1] (default); 1 = use diffuse light color; 2 = use diffuse light color, and scale value to 1" },
    { 9, "meshIndex",  0, {{ CMDINT(game_state.meshIndex) }},0,
                        "draw only this mesh index when rendering sorted list in drawList_ex (for isolating a single piece of geo)" },
    { 9, "normalscale", 0, {{ PARSETYPE_FLOAT, &game_state.normal_scale }},0,
                        "set the scale for debug normals" },
    { 9, "normal_map_test",  CMD_NORMALMAP_TEST, {{ CMDSENTENCE(tmp_str) }},0,
                        "Options:\n"
                        " 'off' (or 0),\n"
                        " 'use bumpmap1' (or 1),\n"
                        " 'use dxt5nm-multiply1' (or 2),\n"
                        " 'use none' (or 3),\n"
                        " 'view bumpmap1' (or 4),\n"
                        " 'view dxt5nm-multiply1' (or 5)\n"
                        " 'show test_materials (6)\n" },
    { 9, "normals",  CMD_GFXDEBUG, {{ CMDINT(game_state.gfxdebug) }},0,
                        "set to 1 to enable gfx debug info" },
    { 9, "gfxdebug",  CMD_GFXDEBUG, {{ CMDINT(game_state.gfxdebug) }},0,
                        "set to 1 to enable gfx debug info" },
    { 9, "gfxdebugseams",  CMD_DEBUGSEAMS, {{ CMDINT(tmp_int_special[0]) }},0,
                        "show texture seams on entities" },
    { 9, "gfxdebugname",    0,  {{ CMDSTR(game_state.gfxdebugname)}},0,
                        "name of geometry to debug" },
    { 9, "renderinfo",  0, {{ CMDINT(game_state.renderinfo) }},0,
                        "set to 1 to enable render state debug info and misc" },
    { 9, "lightdebug",  0, {{ CMDINT(game_state.lightdebug), 3 }},0,
                        "0: disable, 1: final float color, 2: skyfile color, 3: skyfile web hex" },
    { 9, "reloadSeqs",  CMD_RELOADSEQUENCERS, {{ 0 }}, 0,
                        "development mode reload sequencers and animations" },
    { 9, "shoulderscale",CMD_SHOULDERSCALE, {{ PARSETYPE_FLOAT, &tmp_f32 }}, 0,
                        "development mode increment shoulder scale by given value" },
    { 9, "fixArmReg",    0, {{ CMDINT(game_state.fixArmReg) }}, 0,
                        "toggle inverse kinematics to fix arms if shoulders are scaled" },
    { 9, "fixLegReg",    0, {{ CMDINT(game_state.fixLegReg) }}, 0,
                        "toggle inverse kinematics to fix arms if shoulders are scaled" },
    { 9, "fixRegDbg",    0, {{ CMDINT(game_state.fixRegDbg) }}, 0,
                        "toggle debug info for arm correction (see '/fixArmReg')" },
    { 9, "showSpecs",  0, {{ CMDINT(game_state.showSpecs) }},0,
                        "print your CPU specs to the screen" },
    { 9, "quickLoadAnims",  0, {{ CMDINT(game_state.quickLoadAnims) }},0,
                        "Dev mode loading anims fast" },
    { 9, "nofx",  0, {{ CMDINT(game_state.nofx) }},0,
                        "Dev mode loading fx fast" },
    { 9, "quickLoadFonts",  0, {{ CMDINT(game_state.quickLoadFonts) }},0,
                        "Dev mode loading only one font" },
    { 0, "cov",  CMD_UISKIN, {{ CMDINT(tmp_int) }},0,
                        "" },
    { 0, "uiskin",  CMD_UISKIN, {{ CMDINT(tmp_int) }},0,
                        "" },
    { 9, "cod",  0, {{ CMDINT(game_state.cod) }},0,
                        "Dev mode test D_ data" },
    { 9, "setdebugentity",CMD_SETDEBUGENTITY, {{ CMDINT(tmp_int) }}, CMDF_HIDEVARS,
                        "set the entity id that various things use as a server debug entity" },
    { 9, "scriptdebug",    CMD_SCRIPTDEBUG_SET, {{ CMDINT(tmp_int) }}, CMDF_HIDEVARS,
                        "turns script debugging on or off" },
    { 9, "scriptvartop",CMD_SCRIPTDEBUG_VARTOP, {{ CMDINT(tmp_int) }, { CMDINT(tmp_int2) }}, CMDF_HIDEVARS,
                        "sets the top number var to show" },
    { 9, "showstate",    CMD_SHOWSTATE,  {{ CMDSTR(game_state.entity_to_show_state)}},0,
                        "blarg" },
    { 9, "chatdebug",    0, { {CMDINT(gPrintShardCommCmds) }},0,
                        "print out all shard commands received from server"  },
    //******************
    { 9, "wackyxlate",  0, {{ CMDINT(game_state.wackyxlate) }},0,
                        "temp for playing with anim positions" },
    //******************
    #if NOVODEX
    { 9, "physics",    0, {{ CMDINT(nx_state.allowed) }}, 0,
                        "turns on novodex"    },
    { 9, "nx_debug",    CMD_NX_DEBUG, {{ CMDINT(nx_state.debug) }}, 0,
                        "enables NovodeX debugging" },
    { 9, "nx_hardware",    CMD_NX_HARDWARE, {{ CMDINT(tmp_int) }}, 0,
                        "enables NovodeX hardware" },
    { 9, "nx_threaded", CMD_NX_THREADED, {{ 0 }}, 0,
                        "enabled NovodeX multithreading" },
    { 9, "nx_entcollision", CMD_NX_ENTCOLLISION, {{ CMDINT(nx_state.entCollision) }}, 0,
                        "enabled NovodeX multithreading" },
    { 9, "nx_initworld",CMD_NX_INIT_WORLD, {{ 0 }}, 0,
                        "creates NovodeX world geometry data" },
    { 9, "nx_deinitworld",        CMD_NX_DEINIT_WORLD, {{ 0 }}, 0,
                        "stops novodex and clears all memory usage" },
    { 9, "nx_clearcache",CMD_NX_CLEAR_CACHE, {{ 0 }}, 0,
                        "clears all cached novodex data" },
    { 9, "nx_info",        CMD_NX_INFO, {{ 0 }}, 0,
                        "print NovodeX info to the console" },
    { 9, "nx_cordumpe",    CMD_NX_COREDUMP, {{ 0 }}, 0,
                        "dumps novodex scene to nxcore.dmp" },
    { 9, "nx_client_PhysXDebug", CMD_NX_PHYSX_CONNECT, {{ 0 }}, 0,
                        "Connects client PhysX to physx visual debugger."},
    #endif
    { 9, "nofilechangecheck",  CMD_NOFILECHANGECHECK, {{ CMDINT(global_state.no_file_change_check_cmd_set) }},0,
                        "enables dynamic checking for file changes" },
    { 9, "netfxdebug",  0, {{ CMDINT(game_state.netfxdebug) }},0,
                        "various networking fx debug flags" },
    { 9, "fxkill",  0, {{ CMDINT(game_state.fxkill) }},0,
                        "set to 1 to kill all currently active fx" },
    { 9, "fxprint",  0, {{ CMDINT(game_state.fxprint) }},0,
                        "set to 1 to print to game console all fx and particle systems currently playing" },
    { 9, "fogcolor", 0, {{ PARSETYPE_FLOAT, &game_state.fogcolor[0] },{ PARSETYPE_FLOAT, &game_state.fogcolor[1] },{ PARSETYPE_FLOAT, &game_state.fogcolor[2] }},0,
                        "blarg" },
    { 9, "showtime",  0, {{ CMDINT(game_state.showtime) }},0,
                        "shows time of day on the screen" },
    { 9, "spin360", 0, {{ CMDINT(game_state.spin360) }},0,
                        "makes 360 degree movies. the count is the number of steps to use" },
    { 9, "cubemaps", CMD_CUBEMAPS,{{0}},0,
                        "dumps the six images needed to make a cubemap from the current eye position" },
    { 0, "camdist", CMD_CAMDIST, {{ PARSETYPE_FLOAT, &tmp_f32 }},0,
                        "Sets the distance in feet that the third person camera pulls back behind the player." },
    { 0, "camdistadjust", CMD_CAMDISTADJUST, {{ CMDINT(tmp_int)}},CMDF_HIDEVARS|CMDF_HIDEPRINT,
                        "adjusts the camera distance relative to the current camera distance.  Reads mousewheel for input." },
    { 9, "cmalloc", CMD_CMALLOC,{{0}},0,
                        "dumps all mallocs made by the client to a log file. you can do the same thing for the mapserver with smalloc" },
    { 9, "tri_hist", 0, {{ CMDINT(game_state.tri_hist) }},0,
                        "prints a histogram of the triangle counts of all objects drawn" },
    { 9, "tri_cutoff", 0, {{ CMDINT(game_state.tri_cutoff) }},0,
                        "performance testing feature to not draw objects with less than the given number of triangles" },
    { 0, "camreset", CMD_CAMRESET,{{0}},0,
                        "Camreset is bound to the PageDown key (default) to reset the camera behind the player." },
    { 0, "demoplay", CMD_DEMOPLAY, {{ CMDSENTENCE(demo_state.demoname)}},CMDF_HIDEVARS|CMDF_HIDEPRINT,
                        "Name of demo to play back" },
    { 0, "demoloop", 0, {{ CMDINT(demo_state.demo_loop) }},CMDF_HIDEPRINT,
                        "Number of times to loop demo before exiting" },
    { 0, "demoframestats", 0, {{ CMDINT(demo_state.demo_framestats) }},CMDF_HIDEPRINT,
                        "whether or not to log performance info for every frame of the demo" },
    { 0, "demopause", 0, {{ CMDINT(demo_state.demo_pause) }},CMDF_HIDEPRINT,
                        "point to stop and loop same frame (in msec)" },
    { 0, "demospeedscale", 0, {{ CMDINT(demo_state.demo_speed_scale)}},CMDF_HIDEPRINT,
                        "speed multiple to play back demo at" },
    { 0, "demorecord", CMD_DEMORECORD, {{ CMDSTR(demo_state.demoname)}},CMDF_HIDEVARS,
                        "Record a demo to the given name" },
    { 0, "demorecord_auto", CMD_DEMORECORD_AUTO, {0},0,
                        "Record a demo with automatically generated name" },
    { 0, "demostop", CMD_DEMOSTOP, {0},0,
                        "Stop demo record/play" },
    { 0, "demodump", 0, {{ CMDINT(demo_state.demo_dump) }},CMDF_HIDEPRINT,
                        "dump frames to jpg files" },
    { 0, "demodumptga", CMD_DEMODUMPTGA, {{ CMDINT(demo_state.demo_dump_tga) }},CMDF_HIDEPRINT,
                        "dump frames to tga files" },
    { 0, "demofps", CMD_DEMOSTOP, {{ CMDINT(demo_state.demo_fps) }},CMDF_HIDEPRINT,
                        "set demo playback frames per second" },
    { 0, "demohidenames", 0, {{ CMDINT(demo_state.demo_hide_names) }}, CMDF_HIDEPRINT,
                        "hides names in demo playback." },
    { 0, "demohidedamage", 0, {{ CMDINT(demo_state.demo_hide_damage) }}, CMDF_HIDEPRINT,
                        "hides damage in demo playback." },
    { 0, "demohidechat", 0, {{ CMDINT(demo_state.demo_hide_chat) }}, CMDF_HIDEPRINT,
                        "hides chat in demo playback." },
    { 0, "demohideallentityui", 0, {{ CMDINT(demo_state.demo_hide_all_entity_ui) }}, CMDF_HIDEPRINT,
                        "hides all ui in demo playback." },
    { 9, "maptest", 0, {{ CMDINT(game_state.maptest) }},0,
                        "set this to -1 to test mission maps without getting a mission" },
    { 9, "can_edit", 0, {{ CMDINT(game_state.can_edit) }},0,
                        "controls whether or not you can get into the editor. can set it from the command line with -can_edit" },
    { 9, "cttc", CMD_CLEAR_TT_CACHE,{{0}},0,
                        "For truetype font testing.  Currently does nothing." },
#define PARSE_TEST_VAR(i) \
    { 9, "test" #i, 0, {{ PARSETYPE_FLOAT, &game_state.test##i }}, 0,                \
                        "temporary debugging params odds and ends" },        \
    { 9, "test" #i "flicker", 0, {{ PARSETYPE_S32, &game_state.test##i##flicker }}, 0,    \
                        "temporary debugging params odds and ends" },
    PARSE_TEST_VAR(1)
    PARSE_TEST_VAR(2)
    PARSE_TEST_VAR(3)
    PARSE_TEST_VAR(4)
    PARSE_TEST_VAR(5)
    PARSE_TEST_VAR(6)
    PARSE_TEST_VAR(7)
    PARSE_TEST_VAR(8)
    PARSE_TEST_VAR(9)
    PARSE_TEST_VAR(10)

    { 9, "testInt1", 0, {{ CMDINT(game_state.testInt1) }}, 0,
                        "temporary debugging params odds and ends" },
    { 9, "testDisplay", 0, {{ CMDINT(game_state.testDisplayValues) }},0,
                        "temporary debugging params odds and ends" },

    { 9, "clothDebug", 0, {{ CMDINT(game_state.clothDebug) }},0,
                        "Bitfield for cloth debug flags" },
    { 9, "clothWindMax", 0, {{ PARSETYPE_FLOAT, &game_state.clothWindMax}},0,
                        "Cloth debug option" },
    { 9, "clothRipplePeriod", 0, {{ PARSETYPE_FLOAT, &game_state.clothRipplePeriod}},0,
                        "Cloth debug option" },
    { 9, "clothRippleRepeat", 0, {{ PARSETYPE_FLOAT, &game_state.clothRippleRepeat}},0,
                        "Cloth debug option" },
    { 9, "clothRippleScale", 0, {{ PARSETYPE_FLOAT, &game_state.clothRippleScale}},0,
                        "Cloth debug option" },
    { 9, "clothWindScale", 0, {{ PARSETYPE_FLOAT, &game_state.clothWindScale}},0,
                        "Cloth debug option" },
    { 9, "clothLOD", 0, {{ CMDINT(game_state.clothLOD)}},0,
                        "Cloth debug option" },

    { 0, "reduce_mip", CMD_REDUCEMIP, {{ CMDINT(game_state.mipLevel) }},CMDF_HIDEPRINT,
                        "Reduces the resolution of textures to only use the reduced (mip-map) textures.  Must pass as command line arg -reduce_mip or you need to subsequently run unloadgfx" },
    { 0, "reduce_min", 0, {{ CMDINT(game_state.reduce_min) }},CMDF_HIDEPRINT,
                        "Sets the minimum size that textures will be reduced to (requires -reduce_mip > 0)" },
    { 0, "texLodBias", 0, {{ CMDINT(game_state.texLodBias) }},CMDF_HIDEPRINT,
                        "Reduces the texture LOD bias for better compatibility with anisotropic filtering (values from 0 - 2 are valid)" },
    { 0, "texAniso", CMD_TEXANISO, {{ CMDINT(game_state.texAnisotropic) }}, 0,
                        "Sets the amount of anisotropic filtering to use, reloads textures" },
    { 0, "maxAniso", 0, {{ CMDINT(rdr_caps.maxTexAnisotropic)}}, CMDF_HIDEPRINT,
                        "Shows the maximum anisotropic your card allows" },
    { 0, "antialiasing", CMD_ANTIALIASING, {{ CMDINT(game_state.antialiasing) }}, CMDF_HIDEPRINT,
                        "Sets the amount of full screen antialiasing" },
    { 0, "fsaa",        CMD_ANTIALIASING, {{ CMDINT(game_state.antialiasing) }}, 0,
                        "Sets the amount of full screen antialiasing" },
    { 0, "splatShadowBias", 0, {{ PARSETYPE_FLOAT, &game_state.splatShadowBias }},CMDF_HIDEPRINT,
                        "change how far from the camera to give people shadows" },
    { 0, "vis_scale",    CMD_VISSCALE, {{ PARSETYPE_FLOAT, &game_state.vis_scale }},0,
                        "Controls world detail 1.0=default" },
    { 9, "draw_scale",    0, {{ PARSETYPE_FLOAT, &game_state.draw_scale }},0,
                        "Controls draw distance 1.0=default" },
    { 9, "lod_scale",    0, {{ PARSETYPE_FLOAT, &game_state.lod_scale }},0,
                        "Controls lod range 1.0=default" },
    { 9, "lod_fade_range",    0, {{ PARSETYPE_FLOAT, &game_state.lod_fade_range }},0,
                        "Controls lod near/far fade muting 1.0=default" },
    { 0, "nopixshaders", 0, {{ CMDINT(game_state.nopixshaders) }}, CMDF_HIDEPRINT,
                        "Turns off pixel shaders." },
    { 0, "novbos", CMD_NOVBOS, {{ CMDINT(game_state.noVBOs) }}, CMDF_HIDEPRINT,
                        "Turns off vertex buffer object extension." },
    { 0, "maxtexunits", CMD_MAX_TEXUNITS, {{ CMDINT(game_state.maxTexUnits)}}, CMDF_HIDEPRINT,
                        "Limits number of textures used, set to 4 to emulate GF 4/5 path" },
    { 0, "shaderDetail", CMD_SHADER_DETAIL, {{ CMDINT(tmp_int)}}, CMDF_HIDEVARS,
                        "Changes the shader detail level" },
    { 9, "oldvid", CMD_OLD_VIDEO_TEST, {{ CMDINT(game_state.oldVideoTest)}}, 0,
                        "Debug testing of old video rendering paths" },
    { 9, "shadertest",  CMD_SHADER_TEST, {{ CMDINT(game_state.shaderTest), 14 }},0,
                        "Set to enable various gfx debug info" },
    { 9, "showDOF", CMD_SHOW_DOF, {{ CMDINT(tmp_int_special[3])}}, 0,
                        "Shows DOF values" },
    { 9, "gfxtest",  CMD_GFX_TEST, {{ 0 }},0,
                        "Run a graphics performance test" },
    { 9, "nofancywater", 0, {{ CMDINT(game_state.nofancywater)}}, CMDF_HIDEPRINT,
                        "Disable pretty water" },
    { 9, "writeProcessedShaders", 0, {{ CMDINT(game_state.writeProcessedShaders)}}, 0,
                        "Writes resulting shaders after post processing to disk" },
    { 9, "nvPerfShaders", 0, {{ CMDINT(game_state.nvPerfShaders)}}, 0,
                        "Runs nvPerf on post-processed shaders" },
    { 0, "nohdr", 0, {{ CMDINT(game_state.nohdr)}}, CMDF_HIDEPRINT,
                        "Disable HDR lighting effects." },
    { 0, "useHDR", CMD_USEHDR, {{ CMDINT(tmp_int_special[1])}}, CMDF_HIDEVARS,
                        "Use HDR lighting effects (Bloom/tonemapping) if available" },
    { 0, "useDOF", CMD_USEDOF, {{ CMDINT(tmp_int_special[2])}}, CMDF_HIDEVARS,
                        "Use Depth of Field effects if available" },
    { 0, "useBumpmaps", CMD_USEBUMPMAPS, {{ CMDINT(tmp_int_special[6])}}, 0,
                        "Use bumpmaps" },
    { 0, "useHQ", CMD_USEHQ, {{ CMDINT(game_state.useHQ)}}, 0,
                        "Allow use of High Quality shader variants" },
    { 0, "useCubemap", CMD_USECUBEMAP, {{ CMDINT(tmp_int_special[5])}}, 0,
                        "Use cubemap" },
    { 0, "useFP", CMD_USEFP, {{ CMDINT(tmp_int)}}, CMDF_HIDEVARS,
                        "Use a floating point render target for HDR lighting effects if available" },
    { 0, "useWater", CMD_USEFANCYWATER, {{ CMDINT(tmp_int)}}, CMDF_HIDEVARS,
                        "Use fancy water effects if available" },
    { 9, "useViewCache", 0, {{ CMDINT(game_state.useViewCache)}}, CMDF_HIDEPRINT,
                        "Enable the shader cache" },
    { 0, "useARBassembly", 0, {{ CMDINT(game_state.useARBassembly) }}, CMDF_HIDEPRINT,
                        "forces use of ARB assembly shader path" },
    { 0, "noARBfp", 0, {{ CMDINT(game_state.noARBfp) }}, CMDF_HIDEPRINT,
                        "disables use of ARBfp shader path, uses vendor-specific fragment shaders instead" },
    { 0, "noNV", 0, {{ CMDINT(game_state.noNV) }}, CMDF_HIDEPRINT,
                        "disables use of NV1X/NV2X shader path, uses only ARB shaders instead" },
    { 0, "noATI", 0, {{ CMDINT(game_state.noATI) }}, CMDF_HIDEPRINT,
                        "disables use of R200 shader path, uses only ARB shaders instead" },
    { 0, "NV1X", 0, {{ CMDINT(game_state.forceNv1x) }}, CMDF_HIDEPRINT,
                        "Forces NV1X rendering path" },
    { 0, "NV2X", 0, {{ CMDINT(game_state.forceNv2x) }}, CMDF_HIDEPRINT,
                        "Forces NV2X rendering path" },
    { 0, "R200", 0, {{ CMDINT(game_state.forceR200) }}, CMDF_HIDEPRINT,
                        "Forces R200rendering path" },
    { 9, "nochip", 0, {{ CMDINT(game_state.forceNoChip) }}, CMDF_HIDEPRINT,
                        "Forces rdr_caps.chip == 0" },
    { 9, "useChipDebug", 0, {{ CMDINT(game_state.useChipDebug) }}, CMDF_HIDEPRINT,
                        "Forces rdr_caps for eumlating and debugging a specific hardware scenario" },
    { 0, "noSunFlare", 0, {{ CMDINT(game_state.no_sun_flare)}}, 0,
                        "Disables sun flare for performance debugging" },
    { 9, "noNP2", 0, {{ CMDINT(game_state.noNP2)}}, CMDF_HIDEPRINT,
                        "Disables ARB_texture_non_power_of_two" },
    { 0, "noPBuffers", 0, {{ CMDINT(game_state.noPBuffers)}}, CMDF_HIDEPRINT,
                        "Disables PBuffers" },
    { 0, "usePBuffers", 0, {{ CMDINT(game_state.usePBuffers)}}, CMDF_HIDEPRINT,
                        "Force use of PBuffers even if on a suspected unsupported video card" },
    { 0, "useFBOs", 0, {{ CMDINT(game_state.useFBOs)}}, CMDF_HIDEPRINT,
                        "Use FBOs, if supported, for off-screen rendering" },
    { 0, "useCg", CMD_TOGGLE_USECG, {{ CMDINT(tmp_int)}}, CMDF_HIDEPRINT,
                        "Use Cg shaders instead of ARB" },
    { 0, "dxt5nm_normal_maps", CMD_TOGGLE_DXT5NM, {{ CMDINT(tmp_int) }},CMDF_HIDEPRINT,
                        "1 = Use DXT5nm cvompressed normal maps, 0 = Use DXT5 normal maps (old mode)" },
    { 0, "shaderCache", 0, {{ CMDINT(game_state.shaderCache)}}, CMDF_HIDEPRINT,
                        "Enable the shader cache" },
    { 0, "shader_init_logging", 0, {{ CMDINT(game_state.shaderInitLogging)}}, CMDF_HIDEPRINT,
                        "Set to 1 to view shader initialization details in the console." },
    { 9, "gfx_hud", CMD_GFX_DEV_HUD, {{ 0 }}, 0,
                        "Toggle the graphics dev HUD" },
    { 0, "shader_optimization", CMD_SHADER_TEST, {{ CMDINT(game_state.shaderOptimizationOverride)}}, CMDF_HIDEPRINT,
                        "Values: 0-3, or -1 to return to default. Causes shaders to reload with the new optimization hint." },
    { 0, "hardconsts", 0, {{ CMDINT(game_state.use_hard_shader_constants)}}, CMDF_HIDEPRINT,
                        "Use hard shader constants instead of Cg to setup shader params" },
    { 0, "prevshaders", CMD_SHADER_TEST, {{ CMDINT(game_state.use_prev_shaders)}}, CMDF_HIDEPRINT,
                        "Use previous Cg shader set found in 'cgfx/prev' subfolder for comparison/debugging" },
    { 0, "nopopup", 0, {{ CMDINT(g_win_ignore_popups)}}, CMDF_HIDEPRINT,
        "Enable or disable popup error dialogs" },
    { 0, "useMRTs", CMD_SHADER_TEST, {{ CMDINT(game_state.useMRTs)}}, CMDF_HIDEPRINT,
                        "Use MRTs, for DoF effect debugging" },
    { 0, "noBump", CMD_SHADER_TEST, {{ CMDINT(game_state.noBump)}}, CMDF_HIDEPRINT,
                        "disable bump maps by forcing unperturbed normal" },
    { 9, "diffuseOnly", CMD_SHADER_TEST_DIFFUSE_ONLY, {{ CMDINT(tmp_int)}}, CMDF_HIDEPRINT,
                        "shader debug mode - display diffuse light only" },
    { 9, "specOnly", CMD_SHADER_TEST_SPEC_ONLY, {{ CMDINT(tmp_int)}}, CMDF_HIDEPRINT,
                        "shader debug mode - display specular light only" },
    { 9, "normalsOnly", CMD_SHADER_TEST_NORMALS_ONLY, {{ CMDINT(tmp_int)}}, CMDF_HIDEPRINT,
                        "shader debug mode - display normal map value only" },
    { 9, "tangentsOnly", CMD_SHADER_TEST_TANGENTS_ONLY, {{ CMDINT(tmp_int)}}, CMDF_HIDEPRINT,
                        "shader debug mode - display tangent values only" },
    { 9, "baseMatOnly", CMD_SHADER_TEST_BASEMAT_ONLY, {{ CMDINT(tmp_int)}}, CMDF_HIDEPRINT,
                        "shader debug mode - display base material only" },
    { 9, "blendMatOnly", CMD_SHADER_TEST_BLENDMAT_ONLY, {{ CMDINT(tmp_int)}}, CMDF_HIDEPRINT,
                        "shader debug mode - display blend material only" },
    { 9, "democonfig", CMD_DEMO_CONFIG, {{ 0 }}, 0,
                        "configure settings for demo" },
    { 0, "cgShaderPath", CMD_SET_CG_SHADER_PATH, {{ CMDSTR(tmp_str) }}, 0,
                        "Parent directory for \"shaders/cgfx\". If relative, path is resolved using the .exe directory." },
    { 0, "cgSaveShaderListing", CMD_SHADER_TEST, {{ CMDINT(game_state.saveCgShaderListing)}}, CMDF_HIDEPRINT,
                        "Save Cg shader listings in the current directory" },
    { 0, "noRTT", 0, {{ CMDINT(game_state.noRTT)}}, CMDF_HIDEPRINT,
                        "Disables RTT PBuffers" },
    { 0, "no_nv_clip", 0, {{ CMDINT(game_state.no_nv_clip)}}, CMDF_HIDEPRINT,
                        "Disable using nVidia specific vertex profiles for user clipping. (use on command line)" },
    { 0, "renderScaleX", 0, {{ PARSETYPE_FLOAT, &game_state.renderScaleX}}, 0,
                        "Changes the horizontal scale at which the 3D world is rendered relative to your screen size" },
    { 0, "renderScaleY", 0, {{ PARSETYPE_FLOAT, &game_state.renderScaleY}}, 0,
                        "Changes the vertical scale at which the 3D world is rendered relative to your screen size" },
    { 0, "renderScale", CMD_RENDERSCALE, {{ PARSETYPE_FLOAT, &tmp_f32}}, 0,
                        "Changes the scale at which the 3D world is rendered relative to your screen size" },
    { 0, "renderSize", CMD_RENDERSIZE, {{ CMDINT(tmp_int)}, { CMDINT(tmp_int2) }}, 0,
                        "Changes the size at which the 3D world is rendered" },
    { 0, "useRenderScale", 0, {{ CMDINT(game_state.useRenderScale)}}, 0,
                        "Enables/disables render scaling feature" },
    { 0, "renderScaleFilter", 0, {{ CMDINT(game_state.renderScaleFilter)}}, 0,
                        "Changes method of filtering used in renderscaling" },
    { 0, "bloomWeight", 0, {{ PARSETYPE_FLOAT, &game_state.bloomWeight}}, 0,
                        "Sets bloom scale, valid values 0.0 - 2.0" },
    { 0, "bloomScale", 0, {{ CMDINT(game_state.bloomScale)}}, 0,
                        "Sets bloom blur size, valid values of 2 or 4" },
    { 0, "dofWeight", 0, {{ PARSETYPE_FLOAT, &game_state.dofWeight}}, 0,
                        "Sets DOF scale, valid values 0.0 - 2.0" },
    { 9, "desaturate", 0, {{ PARSETYPE_FLOAT, &game_state.desaturateWeightTarget}}, 0,
                        "Sets desaturation value" },
    { 0, "useTexEnvCombine", 0, {{ CMDINT(game_state.useTexEnvCombine) }}, CMDF_HIDEPRINT,
                        "forces use of texture_env_combine shader path" },
    { 0, "compatibleCursors", 0, {{ CMDINT(game_state.compatibleCursors)}}, 0,
                        "Enables useage of basic Windows mouse cursors instead of graphical cursors (command line option)" },
    { 0, "enablevbos", CMD_ENABLEVBOS, {{ CMDINT(game_state.enableVBOs) }}, CMDF_HIDEPRINT,
                        "turns on vertex buffer object extension" },
    { 9, "enablevbosforparticles", 0, {{ CMDINT(game_state.enableVBOsforparticles) }}, 0,
                        "turns off vertex buffer object extension" },
    { 9, "disablevbosforparticles", 0, {{ CMDINT(game_state.disableVBOsforparticles) }}, 0,
                        "turns off vertex buffer object extension" },
    { 9, "novertshaders", 0, {{ CMDINT(game_state.novertshaders) }}, 0,
                        "turns off vertex shaders" },
    { 9, "noparticles", 0, {{ CMDINT(game_state.noparticles) }}, 0,
                        "turns off particle system" },
    { 9, "disablesky", 0, {{ CMDINT(game_state.disableSky) }}, 0,
                        "turns off sky rendering and sun glare" },
    { 9, "perftest",    CMD_PERFTEST, {{ CMDINT(tmp_int) },{ CMDINT(tmp_int2) }}, CMDF_HIDEVARS,
                        "graphics perf test <thing to test> <times to run test>" },
    { 9, "perftestsuite",CMD_PERFTESTSUITE, {{ CMDINT(tmp_int) }}, CMDF_HIDEVARS,
                        "graphics perf test suite <times to run test>" },
    { 9, "fogdist", 0, {{ PARSETYPE_FLOAT, &game_state.fogdist[0] },{ PARSETYPE_FLOAT, &game_state.fogdist[1] }},0,
                        "manually override fog distance values" },
    { 9, "dofFocus", 0, {{ PARSETYPE_FLOAT, &game_state.dof_values.focusDistance }},0,
                        "manually override DOF values" },
    { 9, "dofFocusValue", 0, {{ PARSETYPE_FLOAT, &game_state.dof_values.focusValue}},0,
                        "manually override DOF values" },
    { 9, "dofNearDist", 0, {{ PARSETYPE_FLOAT, &game_state.dof_values.nearDist }},0,
                        "manually override DOF values" },
    { 9, "dofNearValue", 0, {{ PARSETYPE_FLOAT, &game_state.dof_values.nearValue }},0,
                        "manually override DOF values" },
    { 9, "dofFarDist", 0, {{ PARSETYPE_FLOAT, &game_state.dof_values.farDist }},0,
                        "manually override DOF values" },
    { 9, "dofFarValue", 0, {{ PARSETYPE_FLOAT, &game_state.dof_values.farValue }},0,
                        "manually override DOF values" },
    { 9, "dofTest", CMD_DOF_TEST, {{ CMDINT(tmp_int) }, {PARSETYPE_FLOAT, &tmp_f32}, {PARSETYPE_FLOAT, &tmp_f325}, {PARSETYPE_FLOAT, &tmp_f322}, {PARSETYPE_FLOAT, &tmp_f323}, {PARSETYPE_FLOAT, &tmp_f324}}, CMDF_HIDEVARS,
                        "Test DOF.  <switchtype 0/1> <focusdist> <focusValue> <neardist> <fardist> <duration>" },
    { 0, "shadowvol", 0, {{ CMDINT(game_state.shadowvol) }},0,
                        "Controls whether or not shadow volumes are drawn." },
    { 0, "noStencilShadows", 0, {{ CMDINT(game_state.noStencilShadows) }},CMDF_HIDEPRINT,
                        "Command line option to disable stencil shadows" },
    { 0, "ss", 0, {{ CMDINT(game_state.disableSimpleShadows) }},0,
                        "Controls whether or not simple shadows are drawn." },
    { 9, "ssdebug", 0, {{ CMDINT(game_state.simpleShadowDebug) }},0,
                        "Print simple shadow debug info" },
    { 9, "shadowMode", 0, {{ CMDINT(game_state.shadowMode) }},0,
                        "Sets the shadow mode (0 = off, 1 = stencil, 2 = shadow maps)" },
    { 9, "showPerf", 0, {{ CMDINT(game_state.showPerf) }},0,
                        "Print performance settings" },
    { 9, "showDepth", 0, {{ CMDINT(game_state.showDepthComplexity) }},0,
                        "show Depth Complexity" },
    { 9, "noFog", 0, {{ CMDINT(game_state.noFog) }},0,
                        "disables fog in GL while the game clips like the fog is still on" },
    { 9, "whiteParticles", 0, {{ CMDINT(game_state.whiteParticles) }},0,
                        "white Particles" },
    { 9, "tintfxbytype", 0, {{ CMDINT(game_state.tintFxByType) }},0,
                        "Tint FX based on power type" },
    { 0, "suppressCloseFx", 0, {{ CMDINT(game_state.suppressCloseFx) }},CMDF_HIDEVARS,
                        "Hide all personal FX when the camera is closer than the suppressCloseFxDist" },
    { 0, "suppressCloseFxDist", 0, {{ PARSETYPE_FLOAT, &game_state.suppressCloseFxDist }},CMDF_HIDEVARS,
                        "Within this camera distance, personal FX will be suppressed." },
    { 9, "headshot", 0, {{ CMDINT(game_state.headShot) }},0,
                        "Take a head shot of this costume number, print it on the screen" },
    { 1, "headshot_clear", CMD_HEADSHOT_CACHE_CLEAR, {{ 0 }},0,
                        "Clears the headshot cache so that contact icons are regenerated." },
    { 9, "groupshot", 0, {CMDSTR(game_state.groupShot)},0,
                        "Take a thumbnail of a group" },
    { 9, "perf",  0, {{ CMDINT(game_state.perf) }},0,
                        "various performance flags" },
    { 9, "waterDebug",  0, {{ CMDINT(game_state.waterDebug) }},0,
                        "water debug flags" },
    { 9, "waterReflectionDebug",  0, {{ CMDINT(game_state.waterReflectionDebug) }},0,
                        "water reflection debug flags" },
    { 9, "reflectionEnable",  0, {{ CMDINT(game_state.reflectionEnable) }},0,
                        "enable planar reflections, 1 for water, 2 for buildings" },
    { 9, "separate_reflection_viewport",  0, {{ CMDINT(game_state.separate_reflection_viewport) }},0,
                        "enable second viewport for building planar reflections" },
    { 9, "allow_simple_water_reflection",  0, {{ CMDINT(game_state.allow_simple_water_reflection) }},0,
                        "use second viewport for sky-only water reflections if separate_reflection_viewport is false" },
    { 9, "planar_reflection_mip_bias", 0, {{ PARSETYPE_FLOAT, &game_state.planar_reflection_mip_bias }},0,
                        "planar reflection MIP bias" },
    { 9, "cubemap_reflection_mip_bias", 0, {{ PARSETYPE_FLOAT, &game_state.cubemap_reflection_mip_bias }},0,
                        "cubemap reflection MIP bias" },
    { 9, "reflectionDebug",  0, {{ CMDINT(game_state.reflectionDebug) }},0,
                        "reflection debug flags" },
    { 9, "reflectionDebugName",    0,  {{ CMDSTR(game_state.reflectiondebugname)}},0,
                        "name of geometry to debug for reflections" },
    { 9, "gen_static_cubemaps",  0, {{ CMDINT(game_state.gen_static_cubemaps) }},0,
                        "generate all static cubemaps now (time consuming!)" },
    { 9, "spriteSortDebug",  0, {{ CMDINT(game_state.spriteSortDebug) }},0,
                        "sprite sort debug flags" },
    { 9, "numShadowCascades",  0, {{ CMDINT(game_state.shadowmap_num_cascades) }},0,
                        "number of shadow cascades to render and use" },
    { 9, "shadowMapDebug",  0, {{ CMDINT(game_state.shadowMapDebug) }},0,
                        "shadow map debug flags" },
    { 9, "shadowMapBind",  0, {{ CMDINT(game_state.shadowMapBind) }},0,
                        "shadow map texture to bind to player" },
    { 0, "pbuftest",  0, {{ CMDINT(game_state.pbuftest) }},CMDF_HIDEPRINT,
                        "thrash PBuffers" },
    { 0, "lodbias", 0, {{ PARSETYPE_FLOAT, &game_state.LODBias }},CMDF_HIDEPRINT,
                        "multiplier to LOD distances for entities 0.5 = switch twice as near 2.0 means switch twice as far" },
    { 0, "nothread", CMD_NO_THREAD,{{ CMDINT(tmp_int) }},CMDF_HIDEVARS|CMDF_HIDEPRINT,
                        "Disables threaded texture loading" },
    { 1, "slowtexload", 0,{{ CMDINT(delay_texture_loading)}}, 0,
                        "Delays texture loading speed to simulate a slow computer" },
    { 1, "mapname", 0, {{ CMDSTR(game_state.world_name)}},0,
                        "prints name of map for people running in fullscreen" },
    { 2, "see_everything", 0, {{ CMDINT(game_state.see_everything) }},0,
                        "<0/1> Shows spawn points, triggers, pathfinding beacons, etc." },
    { 9, "beacon_radius", 0, {{ PARSETYPE_FLOAT, &g_BeaconDebugRadius }}, 0,
                        "radius of visibility sphere for beacon debugging" },
    { 9, "camera_shake", 0, {{ PARSETYPE_FLOAT, &game_state.camera_shake }},0,
                        "the camera shake amplitude.  falls off by the value of camera_shake_falloff." },
    { 9, "camera_shake_falloff", 0, {{ PARSETYPE_FLOAT, &game_state.camera_shake_falloff }},0,
                        "how much the camera shake is reduced over time." },
    { 9, "camera_blur", 0, {{ PARSETYPE_FLOAT, &game_state.camera_blur }},0,
                        "the camera blur amplitude.  falls off by the value of camera_blur_falloff." },
    { 9, "camera_blur_falloff", 0, {{ PARSETYPE_FLOAT, &game_state.camera_blur_falloff }},0,
                        "how much the camera blur is reduced over time." },
    { 9, "vtune", CMD_VTUNE, {{ CMDINT(game_state.vtuneCollectInfo) }}, 0,
                        "pause/resume vtune performance info collection" },
    { 9, "playsound", CMD_PLAYSAMPLE, {{ CMDSENTENCE(game_state.song_name)}}, CMDF_HIDEVARS,
                        "play the given ogg file" },
    { 9, "playallsounds", CMD_PLAYALLSOUNDS, {{CMDINT(tmp_int)}}, CMDF_HIDEVARS,
                        "play all sounds starting at the given index (0 to start from beginning, -1 to disable)" },
    { 9, "playmusic", CMD_PLAYMUSIC, {{CMDSTR(game_state.song_name)}, { PARSETYPE_FLOAT, &tmp_f32}, {CMDINT(tmp_int)}}, CMDF_HIDEVARS,
                        "play the given sound file on the music channel (3 args: filename, volume (0 - 1.0), sustain (0/1)" },    
    { 9, "playvo", CMD_PLAYVOICEOVER, {{CMDSTR(game_state.song_name)}}, CMDF_HIDEVARS,
                        "play the given sound file on the voiceover channel" },    
    { 9, "stopmusic", CMD_STOPMUSIC, {{ PARSETYPE_FLOAT, &tmp_f32}}, CMDF_HIDEVARS,
                        "stop playing sound on the music channel (fadeout time in secs" },
    { 9, "volume", CMD_VOLUME, { {CMDSTR(tmp_str)}, {PARSETYPE_FLOAT, &tmp_f32}}, 0,
                        "set master volume, [music/fx/vo] volume" },
    { 9, "duckvolume", CMD_DUCKVOLUME, { {CMDSTR(tmp_str)}, {PARSETYPE_FLOAT, &tmp_f32},{PARSETYPE_FLOAT, &tmp_f322},{PARSETYPE_FLOAT, &tmp_f323}}, 0,
                        "duck volume, [music/fx/vo] volume, fadeOutTime, fadeInTime" },
    { 9, "repeatsound", CMD_REPEATSOUND, {{CMDSTR(game_state.song_name)}, { PARSETYPE_FLOAT, &tmp_f32}, {CMDINT(tmp_int)}}, CMDF_HIDEVARS,
                        "repeat the given sound file (3 args: filename, interval, count" },    
    { 9, "disablesound", CMD_SOUND_DISABLE, {{CMDINT(tmp_int)}}, CMDF_HIDEVARS,
                        "disable (1) or enable (0) the sound system" },    
    { 9, "mute", CMD_MUTE, {{ CMDSTR(tmp_str) }}, 0,
                        "mute sound type (none/fx/music/sphere/script/vo/other/all)" },
    { 9, "unmute", CMD_UNMUTE, {{ CMDSTR(tmp_str) }}, 0,
                        "unmute sound type (none/fx/music/sphere/script/vo/other/all)" },
    { 9, "maxSoundChannels", 0, {{ CMDINT(game_state.maxSoundChannels) }},0,
                        "max number of sounds that can play at once" },
    { 9, "maxSoundSpheres", 0, {{ CMDINT(game_state.maxSoundSpheres) }},0,
                        "max number of ambient sounds (spheres + scripts) that can play at once" },
    { 9, "maxPcmCacheMB", 0, {{ PARSETYPE_FLOAT, &game_state.maxPcmCacheMB }},0,
                        "max size of pcm cache in MB" },
    { 9, "maxOggToPcm", 0, {{ CMDINT(game_state.maxOggToPcm) }},0,
                        "max size of ogg file that can be cached to pcm" },
    { 9, "maxSoundCacheMB", 0, {{ PARSETYPE_FLOAT, &game_state.maxSoundCacheMB }},0,
                        "max size total sound data that is cached" },
    { 9, "ignoreSoundRamp", 0, {{ CMDINT(game_state.ignoreSoundRamp) }},0,
                        "ignore the sound ramp value (force inner radius = outer radius" },
    { 0, "soundDebugName", 0, {{ CMDSTR(game_state.soundDebugName)}}, CMDF_HIDEPRINT,
                        "name of sound to debug (will only play this sound name)" },
    { 0, "follow", CMD_FOLLOW, {{ 0 }}, 0,
                        "set follow mode, 1 = follow selected target, 0 = stop following" },
    { 9, "reloadtext", CMD_RELOAD_TEXT,{{0}}, 0,
                        "reloads menu text message store" },
    { 9, "locale", CMD_LOCALE, {{ CMDINT(tmp_int) }}, CMDF_HIDEVARS,
                        "Change locale and reload menu text message store" },
    { 9, "exec", CMD_EXEC, {{ CMDSTR(tmp_str)}}, CMDF_HIDEVARS,
                        "Executes a script file" },
    { 9, "startupexec", 0, {{ CMDSTR(game_state.startupExec)}}, 0,
                        "Executes a script file once the game gets to the login screen" },
    { 0, "stopinactivedisplay", 0, {{ CMDINT(game_state.stopInactiveDisplay) }}, 0,
                        "Stops rendering when the game is not the foreground application." },
    { 9, "entdebugclient", 0, {{ CMDINT(game_state.ent_debug_client) }}, 0,
                        "turns on local entity debugging features." },
    { 9, "debugcollision", 0, {{ CMDINT(game_state.debug_collision) }}, 0,
                        "turns on entity collision debugging" },
    { 9, "cryptic", 0, {{ CMDINT(game_state.cryptic) }}, 0,
                        "remembers your login / password." },
    { 9, "localmapserver", 0, {{ CMDINT(game_state.local_map_server) }}, 0,
                        "connects to the mapserver running at your IP address." },
    { 9, "iamanartist", 0, {{ CMDINT(game_state.iAmAnArtist)}}, 0,
                        "Used to poke values into IAmAnArtist" },
    { 9, "imanartist", 0, {{ CMDINT(game_state.iAmAnArtist)}}, 0,
                        "Used to poke values into IAmAnArtist" },
    { 9, "noversioncheck", 0, {{ CMDINT(game_state.no_version_check) }}, 0,
                        "allow connecting to wrong version of mapserver." },
    { 9, "asklocalmapserver", 0, {{ CMDINT(game_state.ask_local_map_server) }}, 0,
                        "asks if you want to connect to a local map server." },
    { 9, "editnpc", 0, {{ CMDINT(game_state.editnpc) }}, 0,
                        "Allows user to edit npcs." },
    { 9, "asknoaudio", 0, {{ CMDINT(game_state.ask_no_audio) }}, 0,
                        "asks if you want to disable audio." },
    { 9, "askquicklogin", 0, {{ CMDINT(game_state.ask_quick_login) }}, 0,
                        "asks if you want to use quick login." },
    { 0, "db", 0, {{ CMDSTR(game_state.cs_address)}}, CMDF_HIDEPRINT,
                        "dbserver ip string." },
    { 0, "cs", 0, {{ CMDSTR(game_state.cs_address)}}, CMDF_HIDEPRINT,
                        "dbserver ip string." },
    { 0, "auth", 0, {{ CMDSTR(game_state.auth_address)}}, CMDF_HIDEPRINT,
                        "auth server ip string." },
	{ 0, "servergroup", CMD_SET_SERVERGROUP, {{ CMDINT(tmp_int) }}, CMDF_HIDEPRINT,
                        "server group to request from authserver." },
    { 9, "createbins", 0, {{ CMDINT(game_state.create_bins) }}, 0,
                        "forces creation of all .bin files, then exits" },
    { 9, "nogui", 0, {{ CMDINT(tmp_int) }}, CMDF_HIDEVARS|CMDF_HIDEPRINT,
                        "disables the GUI, useful with createbins" },
    { 9, "costume_build_diff", 0, {{ CMDINT(game_state.costume_diff) }}, 0,
                        "forces costume diff check" },
    { 9, "noaudio", 0, {{ CMDINT(g_audio_state.noaudio) }}, 0,
                        "disable sound loading and playing." },
    { 9, "server", 0, {{ CMDSTR(game_state.address)}}, 0,
                        "address of localmapserver to connect to." },
    { 0, "console", CMD_CONSOLE, {{ CMDINT(tmp_int) }}, CMDF_HIDEVARS|CMDF_HIDEPRINT,
                        "turns on debug console (where printfs go)" },
    { 9, "quicklogin", 0, {{ CMDINT(game_state.quick_login) }}, 0,
                        "auto logs you into your account" },
    { 0, "nolastauthor", 0, {{ CMDINT(g_disableLastAuthor) }}, CMDF_HIDEPRINT,
                        "turns off error printing" },
    { 0, "profiling_memory", 0, {{ CMDINT(game_state.profiling_memory) } }, CMDF_HIDEPRINT,
                        "Set the number of MB of memory to use for profiling" },
    { 0, "profile", 0, {{ CMDINT(game_state.profile) } }, CMDF_HIDEPRINT,
                        "Save a profile of the current frame for up to the specified number of frames" },
    { 0, "profile_spikes", 0, {{ CMDINT(game_state.profile_spikes) } }, CMDF_HIDEPRINT,
                        "Save profiles of any frame longer than the specified time in ms" },
    { 0, "nodebug", CMD_NODEBUG, {{ CMDINT(game_state.nodebug) }}, CMDF_HIDEPRINT,
                        "turns off error printing" },
    { 0, "nominidump", CMD_NOMINIDUMP, {{ CMDINT(tmp_int)}}, CMDF_HIDEVARS|CMDF_HIDEPRINT,
                        "disables writing of a minidump upon crash" },
    { 0, "noreport", CMD_NOERRORREPORT, {{ CMDINT(tmp_int)}}, CMDF_HIDEVARS|CMDF_HIDEPRINT,
                        "do not default to error reporting window on crash" },
    { 9, "packetdebug", CMD_PACKETDEBUG, {{ CMDINT(tmp_int) }}, CMDF_HIDEVARS,
                        "turns on packet debugging" },
    { 9, "packetlogging", CMD_PACKETLOGGING, {{ CMDINT(tmp_int) }}, CMDF_HIDEVARS,
                        "turns on (packetlogging 1) or off (packetlogging 0) packet logging" },
    { 0, "mtu",        0, {{ CMDINT(game_state.packet_mtu)}}, CMDF_HIDEPRINT,
                        "Sets the MTU size for client-generated packets (command line option)" },
    { 0, "mouse_look", CMD_MOUSE_LOOK, {{ CMDINT(tmp_int) }}, CMDF_HIDEVARS,
                        "Command key for mouselook" },
    { 0, "info", CMD_INFO, {{ 0 }}, 0,
                        "Opens the info window for the current target (yourself if you have no target)." },
    { 0, "info_tab", CMD_INFO_TAB, {{ CMDINT(tmp_int) }}, 0,
                        "Opens the specified tab of the info window for the current target (yourself if you have no target)." },
    { 0, "info_self", CMD_INFO_SELF, {{ 0 }}, 0,
                        "Opens the info window for yourself." },
    { 0, "info_self_tab", CMD_INFO_SELF_TAB, {{ CMDINT(tmp_int) }}, 0,
                        "Opens the specified tab of the info window for yourself." },

    { 0, "toggle_enemy", CMD_TARGET_ENEMY_NEXT, {{ 0 }}, 0,
                        "Cycles through targetable enemies." },

    { 0, "toggle_enemy_prev", CMD_TARGET_ENEMY_PREV, {{ 0 }}, 0,
                        "Cycles through targetable enemies (in reverse)." },
    { 0, "target_enemy_near", CMD_TARGET_ENEMY_NEAR, {{ 0 }}, 0,
                        "Targets the nearest enemy." },
    { 0, "target_enemy_far", CMD_TARGET_ENEMY_FAR, {{ 0 }}, 0,
                        "Targets the farthest enemy." },
    { 0, "target_enemy_next", CMD_TARGET_ENEMY_NEXT, {{ 0 }}, 0,
                        "Cycles through visible targetable enemies in near to far order." },
    { 0, "target_enemy_prev", CMD_TARGET_ENEMY_PREV, {{ 0 }}, 0,
                        "Cycles through visible targetable enemies in far to near order." },

    { 0, "target_friend_near", CMD_TARGET_FRIEND_NEAR, {{ 0 }}, 0,
                        "Targets the nearest friend." },
    { 0, "target_friend_far", CMD_TARGET_FRIEND_FAR, {{ 0 }}, 0,
                        "Targets the farthest friend." },
    { 0, "target_friend_next", CMD_TARGET_FRIEND_NEXT, {{ 0 }}, 0,
                        "Cycles through visible targetable friends in near to far order." },
    { 0, "target_friend_prev", CMD_TARGET_FRIEND_PREV, {{ 0 }}, 0,
                        "Cycles through visible targetable friends in far to near order." },

    { 0, "target_custom_near", CMD_TARGET_CUSTOM_NEAR, {{ CMDSENTENCE(tmp_str) }}, 0,
        "Targets the nearest match.\n"
        "Parameters:\n"
        "enemy - Hostile enemies\n"
        "friend - Friendlies (including pets)\n"
        "defeated - 0 HP targets\n"
        "alive - Living targets\n"
        "mypet - Inlcude only your pets\n"
        "notmypet - Exclude your pets\n"
        "base - Include only passive base items\n"
        "notbase - Exlude passive base items\n"
        "teammate - Include only teammates\n"
        "notteammate - Exclude teammates\n"
        "Other token will be matched against name for specific targeting\n"},
    { 0, "target_custom_far", CMD_TARGET_CUSTOM_FAR, {{ CMDSENTENCE(tmp_str) }}, 0,
        "Targets the farthest match.\n"
        "Parameters:\n"
        "enemy - Hostile enemies\n"
        "friend - Friendlies (including pets)\n"
        "defeated - 0 HP targets\n"
        "alive - Living targets\n"
        "mypet - Inlcude only your pets\n"
        "notmypet - Exclude your pets\n"
        "base - Include only passive base items\n"
        "notbase - Exlude passive base items\n"
        "teammate - Include only teammates\n"
        "notteammate - Exclude teammates\n"
        "Other token will be matched against name for specific targeting\n" },
    { 0, "target_custom_next", CMD_TARGET_CUSTOM_NEXT, {{ CMDSENTENCE(tmp_str) }}, 0,
        "Cycles through matching targets in near to far order.\n"
        "Parameters:\n"
        "enemy - Hostile enemies\n"
        "friend - Friendlies (including pets)\n"
        "defeated - 0 HP targets\n"
        "alive - Living targets\n"
        "mypet - Inlcude only your pets\n"
        "notmypet - Exclude your pets\n"
        "base - Include only passive base items\n"
        "notbase - Exlude passive base items\n"
        "teammate - Include only teammates\n"
        "notteammate - Exclude teammates\n"
        "Other token will be matched against name for specific targeting\n" },
    { 0, "target_custom_prev", CMD_TARGET_CUSTOM_PREV, {{ CMDSENTENCE(tmp_str) }}, 0,
        "Cycles through matching targets in far to near order.\n"
        "Parameters:\n"
        "enemy - Hostile enemies\n"
        "friend - Friendlies (including pets)\n"
        "defeated - 0 HP targets\n"
        "alive - Living targets\n"
        "mypet - Inlcude only your pets\n"
        "notmypet - Exclude your pets\n"
        "base - Include only passive base items\n"
        "notbase - Exlude passive base items\n"
        "teammate - Include only teammates\n"
        "notteammate - Exclude teammates\n"
        "Other token will be matched against name for specific targeting\n" },
    { 9, "target_confusion", CMD_TARGET_CONFUSION, {{ PARSETYPE_FLOAT, &tmp_f32}}, 0,
                        "Sets the probability (0.0 - 1.0) of switching targets when you are confused." },
    { 9, "placeentity", 0, {{ CMDINT(game_state.place_entity) }}, 0,
                        "enables place-entity mode" },
    { 9, "debug_camera", 0, {{ CMDINT(debug_state.debugCamera.editMode) }}, 0,
                        "enables the debug camera editor" },
    { 9, "supervis", 0, {{ CMDINT(game_state.super_vis)}}, 0,
                        "see entities from very far away" },
    { 0, "unselect", CMD_UNSELECT, {{ 0 }}, 0,
                        "unselects currently selected thing" },
    { 9, "loadmap", CMD_LOADMAP, {{ CMDSTR(tmp_str)}}, 0 ,
                        "loads the specified map" },
    { 9, "loadmissionmap", CMD_LOADMISSIONMAP, {{ CMDSTR(tmp_str)}}, CMDF_HIDEVARS,
                        "as for loadmap, but server pretends you are starting a mission - for debugging" },
    { 9, "copydebugpos", CMD_COPYDEBUGPOS, {{ 0 }}, 0 ,
                        "copies to the clipboard the commands necessary to return to current position" },
    { 9, "runperfinfo_game", 0, {{ CMDINT(timing_state.runperfinfo_init) }}, 0 ,
                        "runs performance info on client" },
    { 9, "resetperfinfo_game", 0, {{ CMDINT(timing_state.resetperfinfo_init) }}, 0 ,
                        "resets performance info on client" },
    { 9, "perfinfo_nothread", 0, {{ CMDINT(timing_state.autoTimerNoThreadCheck) }}, 0 ,
                        "disables thread checking for auto timers" },
    { 9, "perfinfomaxstack_game",    0, {{ CMDINT(timing_state.autoStackMaxDepth_init) }},0,
                        "Sets the depth of the performance monitor stack." },
    { 0, "profiler_record", CMD_PROFILER_RECORD, {{ CMDSTR(tmp_str) }}, CMDF_HIDEVARS,
                        "Record client profiler information to specified file." },
    { 0, "profiler_stop", CMD_PROFILER_STOP, {{0}}, CMDF_HIDEVARS,
                        "Stop recording profiler information." },
    { 9, "profiler_play", CMD_PROFILER_PLAY, {{ CMDSTR(tmp_str) }}, CMDF_HIDEVARS,
                        "Playback recorded profiler file." },
    { 9, "reloadentdebugfilemenus", CMD_RELOADENTDEBUGFILEMENUS, {{ 0 }}, 0 ,
                        "reload your local debugging menus" },
    { 9, "camerafollowentity", CMD_CAMERAFOLLOWENTITY, {{ 0 }}, 0 ,
                        "toggle following the selected entity with the camera" },
    { 9, "camerasharedslave", 0, {{ CMDINT(game_state.camera_shared_slave) }}, 0 ,
                        "Toggle inheriting camera position from a second client" },
    { 9, "camerasharedmaster", 0, {{ CMDINT(game_state.camera_shared_master) }}, 0 ,
                        "Toggle sharing camera position to a second client" },
    { 9, "selectanyentity", CMD_SELECTANYENTITY, {{ CMDINT(game_state.select_any_entity) }}, 0 ,
                        "toggle ability to select any entity" },
    { 9, "cleardebuglines", CMD_CLEARDEBUGLINES, {{ 0 }}, 0,
                        "clears debug lines" },
    { 9, "clear_mm_thumb_cache", CMD_CLEARMMTCACHE, {{ 0 }}, 0,
                        "clears the thumbnail images cache for the Mission editor" },
    { 9, "set_mm_motion", CMD_TOGGLEMMSTILL, {{ CMDINT(tmp_int) }}, 0,
    "use 1 to turn motion images on, 0 to turn them off" },

    { 9, "batch_create_map_images", CMD_MAPIMAGECREATE, {{ CMDSTR(tmp_str) }}, 0,
    "creates map images for the Mission Architect" },

    { 1, "show_all_minimaps", CMD_MAPALLSHOW, {{ CMDINT(tmp_int) }}, 0,
    "Enables a dropdown of all game minimaps from the map window." },
    

    // MS: For pasting a position from the debug window, giant hack!!!

    { 9, "[0]", CMD_PASTE_DEBUGGER_POS_0, {{ PARSETYPE_FLOAT, &tmp_f32 }, { CMDSTR(tmp_str)}}, CMDF_HIDEVARS ,
                        "paste the first line from the debug window" },
    { 9, "[0x0]", CMD_PASTE_DEBUGGER_POS_0, {{ PARSETYPE_FLOAT, &tmp_f32 }, { CMDSTR(tmp_str)}}, CMDF_HIDEVARS ,
                        "paste the first line from the debug window" },
    { 9, "[1]", CMD_PASTE_DEBUGGER_POS_1, {{ PARSETYPE_FLOAT, &tmp_f32 }, { CMDSTR(tmp_str)}}, CMDF_HIDEVARS ,
                        "paste the second line from the debug window" },
    { 9, "[0x1]", CMD_PASTE_DEBUGGER_POS_1, {{ PARSETYPE_FLOAT, &tmp_f32 }, { CMDSTR(tmp_str)}}, CMDF_HIDEVARS ,
                        "paste the second line from the debug window" },
    { 9, "[2]", CMD_PASTE_DEBUGGER_POS_2, {{ PARSETYPE_FLOAT, &tmp_f32 }, { CMDSTR(tmp_str)}}, CMDF_HIDEVARS ,
                        "paste the third line from the debug window" },
    { 9, "[0x2]", CMD_PASTE_DEBUGGER_POS_2, {{ PARSETYPE_FLOAT, &tmp_f32 }, { CMDSTR(tmp_str)}}, CMDF_HIDEVARS ,
                        "paste the third line from the debug window" },
    { 9, "memtrack",    CMD_MEMTRACK_CAPTURE, {{ CMDINT(tmp_int) }}, 0,
                        "enable/disable memtrack data capture (on special builds)" },
    { 9, "memtrack_mark",CMD_MEMTRACK_MARK, {{ CMDSENTENCE(tmp_str)}}, CMDF_HIDEVARS,
                        "Place supplied string into the memtrack capture stream as a mark" },
    { 9, "memcheckpoint",    CMD_MEM_CHECKPOINT, {{ 0 }}, 0,
                        "mark the current state of the heap" },
    { 9, "memdump",            CMD_MEM_DUMP, {{ 0 }}, 0,
                        "dump difference since last checkpoint" },
    { 9, "memchecknow",        CMD_MEM_CHECK, {{ 0 }}, 0,
                        "does a _CrtMemCheck" },
    { 9, "verbose",        CMD_VERBOSE_PRINTF, {{ CMDINT(tmp_int) }}, CMDF_HIDEVARS,
                        "turns on verbose printf messages" },
    { 9, "nowinio",        CMD_NOWINIO, {{ CMDINT(tmp_int) }}, CMDF_HIDEVARS,
                        "turns off win io" },
    { 9, "memonitor",    CMD_MMONITOR, {{ 0 }}, 0,
                        "Turns memory monitor graphs on/off" },
    { 9, "memoryUseDump",    CMD_MEMORYUSEDUMP, {{ 0 }}, 0,
                        "Turns memory monitor graphs on/off" },
    { 9, "memlog_echo",    CMD_MEMLOG_ECHO, {{ 0 }}, 0,
                        "Dumps the contents of the memLog to the console" },
    { 9, "memlog",        CMD_MEMLOG_DUMP, {{ 0 }}, 0,
                        "Dumps the contents of the memLog to the console" },
    { 9, "echo",        CMD_ECHO, {{ CMDSENTENCE(tmp_str) }}, 0,
                        "Echos text to the screen" },
    { 9, "stringtablememdump",    CMD_STRINGTABLE_MEM_DUMP, {{ 0 }}, 0,
                        "Dumps the mem usage of the string tables to the console" },
    { 9, "stashtablememdump",    CMD_STASHTABLE_MEM_DUMP, {{ 0 }}, 0,
                        "Dumps the mem usage of the hash tables to the console" },
    { 9, "showLoadMemUsage",    CMD_SHOW_LOAD_MEM_USAGE, {{ CMDINT(tmp_int) }}, 0,
                        "Print working set and private size for each load stage" },
    { 9, "shaderperftest",    CMD_SHADER_PERF_TEST, {{ 0 }}, 0,
                        "unloads all textures (causing them to be reloaded dynamically)"},
    { 0, "unloadgfx",    CMD_UNLOAD_GFX, {{ 0 }}, CMDF_HIDEPRINT,
                        "unloads all textures (causing them to be reloaded dynamically)"},
    { 0, "reloadgfx",    CMD_UNLOAD_GFX, {{ 0 }}, 0,
                        "unloads all textures (causing them to be reloaded dynamically)"},
    { 9, "rebuildMiniTrackers",    CMD_REBUILD_MINITRACKERS, {{ 0 }}, 0,
                        "Rebuilds minitrackers/texture swaps"},
    { 9, "unloadmodels",    CMD_UNLOAD_MODELS, {{ 0 }}, 0,
                        "unloads all models (causing them to be reloaded dynamically)"},
    { 9, "reloadmodels",    CMD_UNLOAD_MODELS, {{ 0 }}, 0,
                        "unloads all models (causing them to be reloaded dynamically)"},
    { 9, "reloadtricks",    CMD_RELOAD_TRICKS, {{ 0 }}, 0,
                        "reloads all trick based data"},
    { 9, "timestepscale", CMD_TIMESTEPSCALE, {{ PARSETYPE_FLOAT, &tmp_f32}}, 0,
                        "dispaly timing info in the TexWords system"},
    { 9, "texwordsload",CMD_TEXWORDSLOAD, {{ CMDSENTENCE(tmp_str) }}, CMDF_HIDEVARS,
                        "reload texWords files and reset the texWords locale/texture search path"},
    { 9, "texlocale",    CMD_TEXWORDSLOAD, {{ CMDSENTENCE(tmp_str) }}, CMDF_HIDEVARS,
                        "reload texWords files and reset the texWords locale/texture search path"},
    { 0, "texwordeditor",CMD_TEXWORDEDITOR, {{ CMDSENTENCE(tmp_str) }}, CMDF_HIDEVARS,
                        "edit the text layout for translatable textures"},
    { 9, "texwordflush",CMD_TEXWORDFLUSH, {{ 0 }}, CMDF_HIDEVARS,
                        "flush the texWordsRenderingThread quickly"},
    { 9, "texwordverbose", 0, {{ CMDINT(game_state.texWordVerbose) }}, 0,
                        "dispaly timing info in the TexWords system"},
    { 9, "texwordpps", 0, {{ CMDINT(game_state.texWordPPS)}},0,
                        "Show current TexWords pixels per second rendering speed" },
    { 9, "showCustomErrors", 0, {{ CMDINT(game_state.showCustomErrors)}}, 0,
                        "Shows custom texture errors on start-up"},
    { 9, "memcheck",    CMD_MEMCHECK, {{ CMDINT(mem_check_frequency) }}, CMDF_HIDEVARS,
                        "sets memcheck frequency"},
    { 9, "showpointers",    0, {{ CMDINT(game_state.bShowPointers) }}, 0,
                        "shows pointers under player"},
    { 9, "level",        CMD_LEVEL, {{ 0 }}, 0,
                        "go to leveling screen"},
    { 1, "mc",            0, {{CMDINT(game_state.showMouseCoords)}}, 0,
                        "Show Mouse Coordinates" },
    { 0, "whereami",    CMD_WHEREAMI, {{ 0 }}, 0,
                        "Tells the name of the shard/map you are on."},
    { 0, "loc",            CMD_GETPOS, {{ 0 }}, 0,
                        "Get current position."},
    { 0, "getpos",        CMD_GETPOS, {{ 0 }}, 0,
                        "Get current position."},
    { 9, "cometome",    CMD_COMETOME, {{ 0 }}, 0,
                        "broadcasts current position for the TestClient's to go to"},
    { 2, "editcostume",    0, {{CMDINT(game_state.reloadCostume)}}, 0,
                        "reloads the costume bin (command line flag)" },
    { 9, "costumeReload",    CMD_COSTUME_RELOAD, {{ 0 }}, 0,
                        "reloads the costume data files on the fly" },
    { 9, "cdclose",        CMD_CONTACTDIALOG_CLOSE, {{0}}, 0,
                        "Force the contact dialog to close" },
    { 9, "register",    CMD_SUPERGROUP_REGISTER, {{0}}, 0,
                        "Activate an context menu slot" },
    { 9, "ttdebug",        0, {{ CMDINT(game_state.ttDebug) }}, 0,
                        "Truetype debug/test mode" },
    { 9, "map_showplayers",        0, {{ CMDINT(game_state.bMapShowPlayers) }}, 0,
                        "Truetype debug/test mode" },
    { 2, "levelup",        CMD_LEVELUP, {{ CMDINT(tmp_int) }}, CMDF_HIDEVARS,
                        "levelup <level>  Increases the character's level to the given level and purchase powers" },
    { 1, "showcontactlist",    CMD_SHOWCONTACTLIST, {{ 0 }}, 0,
                        "Print out the client contact list" },
    { 1, "showtasklist",    CMD_SHOWTASKLIST, {{ 0 }}, 0,
                        "Print out the client task list" },
    { 0, "autoreply",    CMD_REPLY, {{0}}, 0,
                        "Start a reply for client."    },
    { 9, "accessviolation",    0, {{ PARSETYPE_S32, (void*)1 }}, 0,
                        "Cause an access violation!"    },
    { 9, "assert",        CMD_ASSERT, {{ 0 }}, 0,
                        "Cause an assert!"    },
    { 9, "nosharedmemory",    CMD_NOSHAREDMEMORY, {{ CMDINT(tmp_int) }}, CMDF_HIDEVARS,
                        "Disables shared memory"    },
    { 9, "quickloadtextures",    CMD_QUICKLOAD, {{ CMDINT(tmp_int) }}, CMDF_HIDEVARS,
                        "Misc stuff to load the game quickly without regards to data integrity"    },
    { 9, "notrickreload",    0, {{ CMDINT(g_disableTrickReload)}}, CMDF_HIDEVARS,
                        "Disallow trick reloading"    },
    { 9, "quickplaceonme", CMD_QUICKPLACEONME, {{ 0 }}, 0,
                        "places the current quickplacement object where I'm standing" },
    { 9, "doorfade",    CMD_DOORFADE, {{ CMDINT(dooranim_doorfade) }}, CMDF_HIDEVARS,
                        "determines whether client will fade in and out when doing door animations" },
    { 9, "setfade",        CMD_SETFADE, {{ PARSETYPE_FLOAT, &tmp_f32 }}, CMDF_HIDEVARS,
                        "determines whether client will fade in and out when doing door animations" },
    { 0, "enterdoor",    CMD_ENTERDOOR, {{ PARSETYPE_VEC3, tmp_vec }, { CMDSTR(tmp_str) }}, CMDF_HIDEVARS|CMDF_HIDEPRINT,
                        "" },
    { 0, "enterdoorvolume",    CMD_ENTERDOORVOLUME, {{ CMDSTR(tmp_str) }}, CMDF_HIDEVARS|CMDF_HIDEPRINT,
                        "" },
    { 0, "enter_base_from_sgid",    CMD_ENTERDOORFROMSGID, {{CMDINT(tmp_int)},{ CMDSTR(tmp_str) }}, CMDF_HIDEVARS|CMDF_HIDEPRINT,
                        "" },
    { 0, "cursorcache",    0, {{ CMDINT(game_state.cursor_cache) }}, 0,
                        "enable cursor cache for smoother cursor changes" },
    { 0, "ctm_toggle",    CMD_CLICKTOMOVE_TOGGLE, {{ 0 }}, CMDF_HIDEVARS,
                        "click-to-move toggle"},
    { 0, "ctm_invert",    0, {{CMDINT(gClickToMoveButton)}}, CMDF_HIDEVARS|CMDF_HIDEPRINT,
                        "on demand click-to-move keybind"},
    { 0, "clicktomove", CMD_CLICKTOMOVE, {{CMDINT(tmp_int)}},0,
                        "enables click-to-move" },
    { 0, "ctm",            CMD_CLICKTOMOVE, {{CMDINT(tmp_int)}},0,
                        "enables click-to-move" },
    { 9, "ctm_dist",    0, {{ PARSETYPE_FLOAT, &gClickToMoveDist }},0,
                        "changes click-to-move selection distance" },
    // Chat
    { 0, "beginchat", CMD_START_CHAT_PREFIX, {{ CMDSENTENCE(tmp_str)}}, CMDF_HIDEVARS,
                        "Starts chat-entry mode with given string." },
    { 0, "startchat", CMD_START_CHAT, {{ 0 }}, 0,
                        "Starts chat-entry mode." },
    { 0, "slashchat", CMD_CONSOLE_CHAT, {{ 0 }}, 0,
                        "Starts chat-entry mode with slash." },
    { 0, "say",          CMD_SAY, {{ CMDSENTENCE(tmp_str)}}, CMDF_HIDEVARS,
                        "Sends the given text on the current chat channel." },
    { 0, "s",          CMD_SAY, {{ CMDSENTENCE(tmp_str)}}, CMDF_HIDEVARS,
                        "Sends the given text on the current chat channel." },
    { 0, "chat_set",    CMD_CHAT_CHANNEL_SET, {{ CMDSENTENCE(tmp_str)}}, CMDF_HIDEVARS,
                        "Sets the channel to the given string." },
    { 0, "chat_cycle",    CMD_CHAT_CHANNEL_CYCLE, {{0}}, CMDF_HIDEVARS,
                        "Cycles through the default chat channels." },
    { 0, "hideprimarychat",    CMD_CHAT_HIDE_PRIMARY_CHAT, {{0}}, CMDF_HIDEVARS,
                        "Hide/unhide primary chat window text messages." },
    { 0, "tabcreate",    CMD_TAB_CREATE, {{ CMDINT(tmp_int)},{CMDINT(tmp_int2)},{CMDSENTENCE(tmp_str)}}, CMDF_HIDEVARS|CMDF_HIDEPRINT,
                        "Create new chat tab. Specify window (0-4), pane(0 top, 1 bottom) and tab name." },
    { 0, "tabclose",    CMD_TAB_CLOSE, {{CMDSENTENCE(tmp_str)}}, CMDF_HIDEVARS|CMDF_HIDEPRINT,
                        "Close/delete chat tab" },
    { 0, "tabtoggle",    CMD_TAB_TOGGLE, {{0}}, CMDF_HIDEVARS,
                        "Make the previously active chat tab the new active tab. Used to flip between two tabs." },
    { 0, "tabnext",        CMD_TAB_NEXT, {{ CMDINT(tmp_int)}}, CMDF_HIDEVARS,
                        "Cycle forward through all chat tabs in indicated chat window (0-4)"},
    { 0, "tabprev",        CMD_TAB_PREV, {{ CMDINT(tmp_int)}}, CMDF_HIDEVARS,
                        "Cycle backward through all chat tabs in indicated chat window (0-4)"},
    { 0, "tabglobalnext", CMD_TAB_GLOBAL_NEXT, {{0}}, CMDF_HIDEVARS,
                        "Cycle forward through all chat tabs in all windows. Will open the corresponding chat window if necessary."},
    { 0, "tabglobalprev", CMD_TAB_GLOBAL_PREV, {{0}}, CMDF_HIDEVARS,
                        "Cycle backward through all chat tabs in all windows. Will open the corresponding chat window if necessary."},
    { 0, "tabselect",    CMD_TAB_SELECT,    {{CMDSENTENCE(tmp_str)}}, CMDF_HIDEVARS,
                        "Select the given chat tab. Will open the corresponding chat window if necessary." },

    // Chatserver commands
    { 0, "change_handle",CMD_SHARDCOMM_NAME, {{ CMDSENTENCE(tmp_str)}}, CMDF_HIDEVARS,
                        "Change your global user name, if allowed\n"
                        "Syntax: change_handle <HANDLE>"},
    { 0, "chan_create",    CMD_SHARDCOMM_CREATE, {{ CMDSENTENCE(tmp_str)}}, CMDF_HIDEVARS,
                        "Create a new chat channel\n"
                        "Syntax: chan_create <CHANNEL NAME>"},
    { 0, "chan_join",    CMD_SHARDCOMM_JOIN, {{ CMDSENTENCE(tmp_str)}}, CMDF_HIDEVARS,
                        "Join an existing chat channel\n"
                        "Syntax: chan_join <CHANNEL NAME>"},
    { 0, "chan_leave",    CMD_SHARDCOMM_LEAVE, {{ CMDSENTENCE(tmp_str)}}, CMDF_HIDEVARS,
                        "Leave a chat channel\n"
                        "Syntax: chan_leave <CHANNEL NAME>" },
    { 0, "chan_invitedeny",    CMD_SHARDCOMM_INVITEDENY, {{ CMDSTR(tmp_str)}, {CMDSTR(tmp_str2)}}, CMDF_HIDEVARS, 
                        "chat channel invite denied\n"
                        "Syntax: chan_invitedeny <CHANNEL NAME> <INVITER>" },
    { 0, "chan_invite_team",CMD_SHARDCOMM_CHAN_INVITE_TEAM, {{CMDSENTENCE(tmp_str)}}, CMDF_HIDEVARS,
                        "Invite your entire team or taskforce to a chat channel\n"
                        "Syntax: chan_invite_team <CHANNEL NAME>" },
    { 0, "chan_invite_gf",CMD_SHARDCOMM_CHAN_INVITE_GF, {{CMDSENTENCE(tmp_str)}}, CMDF_HIDEVARS,
                        "Invite your entire global friends list to a chat channel\n"
                        "Syntax: chan_invite_gf <CHANNEL NAME>" },
    { 0, "chan_user_mode",    CMD_SHARDCOMM_USER_MODE, {{CMDSTR(tmp_str)}, {CMDSTR(tmp_str2)}, { CMDSTR(tmp_str3)}}, CMDF_HIDEVARS,
                        "Sets user permissions for specified user on channel.\n"
                        "You must have operator status to set permissions.\n"
                        "Syntax: chan_user_mode <CHANNEL NAME> <PLAYER_NAME> <OPTIONS...>\n"
                        "Valid Options:\n"
                        "\t-join\tkicks user from channel\n"
                        "\t+send / -send\tgives/removes user ability to send messages to channel\n"
                        "\t+operator / -operator\tgives/removes operator status from another user in the channel\n"},
    { 0, "chan_mode",    CMD_SHARDCOMM_CHAN_MODE, {{CMDSTR(tmp_str)}, { CMDSENTENCE(tmp_str2)}}, CMDF_HIDEVARS,
                        "Changes default access rights for new user who joins the channel. If you set -join, no one can join.\n"
                        "Syntax: chan_mode <CHANNEL NAME> <OPTIONS...>"
                        "Valid Options:\n"
                        "\t-join\tkicks user from channel\n"
                        "\t+send / -send\tgives/removes user ability to send messages to channel\n"
                        "\t+operator / -operator\tgives/removes operator status from another user in the channel\n" },
    { 0, "chan_members",CMD_SHARDCOMM_CHAN_LIST, {{ CMDSENTENCE(tmp_str)}}, CMDF_HIDEVARS,
                        "List all members of channel\n"
                        "Syntax: chan_members <CHANNEL NAME>" },
    { 0, "chan_motd",    CMD_SHARDCOMM_CHAN_MOTD, {{CMDSTR(tmp_str)}, { CMDSENTENCE(tmp_str2)}}, CMDF_HIDEVARS,
                        "Set the channel's Message Of The Day, which is sent to everyone that joins the channel\n"
                        "Syntax: chan_motd <CHANNEL NAME> <MESSAGE>" },
    { 0, "chan_desc",    CMD_SHARDCOMM_CHAN_DESC, {{CMDSTR(tmp_str)}, { CMDSENTENCE(tmp_str2)}}, CMDF_HIDEVARS,
                        "Set the channel's time out duration\n"
                        "Syntax: chan_desc <CHANNEL NAME> <TIME IN DAYS>" },
    { 0, "chan_timeout",    CMD_SHARDCOMM_CHAN_TIMEOUT, {{CMDSTR(tmp_str)},{ CMDINT(tmp_int)}}, CMDF_HIDEVARS,
                        "Set the channel's description\n"
                        "Syntax: chan_desc <CHANNEL NAME> <DESCRIPTION>" },
    { 0, "watching",    CMD_SHARDCOMM_WATCHING, {{0}}, CMDF_HIDEVARS,
                        "List all channels that you belong to."},
    { 0, "gfriends",    CMD_SHARDCOMM_FRIENDS, {{0}}, CMDF_HIDEVARS,
                        "Display all members of your global friends list."},
    { 0, "gignoring",    CMD_SHARDCOMM_GIGNORELIST, {{ 0 }}, 0,
                        "Lists the players on your global chat ignore list"},
    { 0, "myhandle",    CMD_MYHANDLE, {{0}}, CMDF_HIDEVARS,
                        "Display your chat handle."},
    { 0, "get_local_name", CMD_SHARDCOMM_WHOLOCAL, {{ CMDSENTENCE(tmp_str) }}, 0,
                        "Get Server / Local Character name from globalname" },
    { 0, "get_local_invite", CMD_SHARDCOMM_WHOLOCALINVITE, {{ CMDSENTENCE(tmp_str) }}, 0,
                        "Invite to team from globalname" },
    { 0, "get_local_league_invite", CMD_SHARDCOMM_WHOLOCALLEAGUEINVITE, {{ CMDSENTENCE(tmp_str) }}, 0,
        "Invite to league from globalname" },
    // Menus
    { 0, "ContextMenu",    CMD_CONTEXT_MENU, {{CMDINT(tmp_int)}}, CMDF_HIDEVARS|CMDF_HIDEPRINT,
                        "Activate an context menu slot." },
    { 9, "start_menu",    CMD_START_MENU, {{CMDINT(tmp_int)}}, CMDF_HIDEVARS,
                        "Activate an ingame menu" },
    { 9, "tailor",        CMD_TAILOR_MENU, {{ 0 }}, CMDF_HIDEVARS,
                        "Activate the tailor menu" },
    { 9, "ultra_tailor",CMD_ULTRA_TAILOR_MENU, {{ 0 }}, CMDF_HIDEVARS,
                        "Activate the ultra tailor menu" },
    { 1, "web",         CMD_WEB_BROWSER_MENU, {{ 0 }}, CMDF_HIDEVARS,
                        "Activate the in game web browser" },
    { 1, "url",         CMD_WEB_BROWSER_GOTO_URL, {{ CMDSENTENCE(tmp_str) }}, CMDF_HIDEVARS,
                        "goto url (activate in game browser if necessary)" },
    { 9, "get_web_reset_data",CMD_WEB_BROWSER_GET_WEB_RESET_DATA, {{ CMDSENTENCE(tmp_str) }}, CMDF_HIDEVARS,
                        "get web data from url" },
    { 0, "popmenu",        CMD_POPMENU, {{ CMDSTR(tmp_str)}}, CMDF_HIDEVARS|CMDF_HIDEPRINT,
                        "Pops up the named menu at the current mouse location" },
    { 0, "quickchat",    CMD_QUICKCHAT, {{ 0 }}, 0,
                        "Pops up the quickchat menu." },
    { 0, "auctionhouse", CMD_AUCTIONHOUSE_WINDOW, {{ 0 }}, CMDF_HIDEVARS,
                        "Activate the Auction House Menu" },
    { 0, "ah", CMD_AUCTIONHOUSE_WINDOW, {{ 0 }}, CMDF_HIDEVARS,
                        "Activate the Auction House Menu" },
    { 0, "wentworths", CMD_AUCTIONHOUSE_WINDOW, {{ 0 }}, CMDF_HIDEVARS,
                        "Activate the Auction House Menu" },
    { 0, "blackmarket", CMD_AUCTIONHOUSE_WINDOW, {{ 0 }}, CMDF_HIDEVARS,
                        "Activate the Auction House Menu" },
    { 0, "vault", CMD_PERSONALSTORAGE_WINDOW, {{ 0 }}, CMDF_HIDEVARS,
                        "Activate the personal storage Menu" },
    { 0, "missionsearch", CMD_MISSIONSEARCH_WINDOW, {{ 0 }}, CMDF_HIDEVARS,
                        "Activate the mission search menu" },
    { 0, "architect",    CMD_MISSIONSEARCH_WINDOW, {{ 0 }}, CMDF_HIDEVARS,
                        "Activate the mission search menu" },
    { 0, "missionmake", CMD_MISSIONMAKER_WINDOW, {{ 0 }}, CMDF_HIDEVARS,
                        "Activate the My Arcs menu of Mission Search" },
    { 0, "mmentry", CMD_MISSIONSEARCH_CHOICE, {{ 0 }}, CMDF_HIDEVARS,
                        "Choose between making and starting a mission maker story arc" },
    { 9, "cvg",    CMD_OPEN_CUSTOM_VG, {{ 0 }}, CMDF_HIDEVARS,
                        "custom villain group test" },
    { 9, "show_info", CMD_SHOW_INFO, {{ 0 }}, CMDF_HIDEVARS,
                        "Show useful debug info about me" },

    // Window manipulation
    { 0, "window_resetall",    CMD_WINDOW_RESETALL, {{0}}, CMDF_HIDEVARS,
                        "Resets all window locations, sizes, and visibility to their defaults." },
    { 0, "window_toggle",    CMD_WINDOW_TOGGLE, {{ CMDSTR(tmp_str)}}, CMDF_HIDEVARS,
                        "Show a window if hidden, hide a window if shown. (Synonym: toggle)" },
    { 0, "toggle",            CMD_WINDOW_TOGGLE, {{ CMDSTR(tmp_str)}}, CMDF_HIDEVARS,
                        "Show a window if hidden, hide a window if shown. (Synonym: window_toggle)" },
    { 0, "mailview",        CMD_MAIL_VIEW, {{ CMDSTR(tmp_str)}}, CMDF_HIDEVARS,
                        "Sets which view to use when on the mail tab" },
    { 0, "window_show",        CMD_WINDOW_SHOW, {{ CMDSTR(tmp_str)}}, CMDF_HIDEVARS,
                        "Forces the given window to be shown. (Synonym: show)" },
    { 0, "show",            CMD_WINDOW_SHOW, {{ CMDSTR(tmp_str)}}, CMDF_HIDEVARS,
                        "Forces the given window to be shown. (Synonym: window_show)" },
    { 9, "showdbg",            CMD_WINDOW_SHOW_DBG, {{ CMDSTR(tmp_str)}}, CMDF_HIDEVARS,
                        "Forces the given window to be shown. (Synonym: window_show)" },
    { 0, "window_hide",        CMD_WINDOW_HIDE, {{ CMDSTR(tmp_str)}}, CMDF_HIDEVARS,
                        "Forces the given window to be hidden." },
    { 0, "window_names",    CMD_WINDOW_NAMES, {{0}}, 0,
                        "Lists the names of windows for window_show, window_toggle, window_hide, and window_scale commands" },
    { 0, "window_color",    CMD_WINDOW_COLOR, {{ CMDINT(gWindowRGBA[0]) },{ CMDINT(gWindowRGBA[1]) },{ CMDINT(gWindowRGBA[2]) },{ CMDINT(gWindowRGBA[3]) },}, CMDF_HIDEVARS,
                        "Changes the window colors." },
    { 0, "windowcolor",        CMD_WINDOW_COLOR, {{ CMDINT(gWindowRGBA[0]) },{ CMDINT(gWindowRGBA[1]) },{ CMDINT(gWindowRGBA[2]) },{ CMDINT(gWindowRGBA[3]) },}, CMDF_HIDEVARS,
                        "Changes the window colors." },
    { 0, "chat",            CMD_WINDOW_CHAT, {{0}}, 0,
                        "Toggles the chat window." },
    { 0, "tray",            CMD_WINDOW_TRAY, {{0}}, 0,
                        "Toggles the tray window" },
    { 0, "target",        CMD_WINDOW_TARGET, {{0}}, 0,
                        "Toggles the target window." },
    { 0, "nav",                CMD_WINDOW_NAV, {{0}}, 0,
                        "Toggles the navigation window." },
    { 0, "map",                CMD_WINDOW_MAP, {{0}}, 0,
                        "Toggles the map window." },
    { 0, "menu",            CMD_WINDOW_MENU, {{0}}, 0,
                        "Toggles the menu." },
    { 0, "chatoptions",        CMD_CHAT_OPTIONS_MENU, {{ CMDINT(tmp_int) }}, 0,
                        "Toggles chat options for specified window (0-4)" },
    { 0, "petoptions",        CMD_PET_OPTIONS_MENU, {{0}}, 0,
                        "Displays pet option context menu." },
    { 0, "powers",            CMD_WINDOW_POWERS, {{0}}, 0,
                        "Toggles the power inventory." },
    { 0, "alttray",            CMD_TRAY_MOMENTARY_ALT_TOGGLE, {{ CMDINT(tmp_int) }}, 0,
                        "Toggle the secondary tray while a key is being pressed. (for keybinds)" },
    { 0, "alt2tray",            CMD_TRAY_MOMENTARY_ALT2_TOGGLE, {{ CMDINT(tmp_int) }}, 0,
                        "Toggle the secondary tray while a key is being pressed. (for keybinds)" },
    { 0, "alttraysticky",    CMD_TRAYTOGGLE, {{ 0 }}, 0,
                        "Toggle the secondary tray." },
    { 0, "tray_sticky",        CMD_TRAY_STICKY, {{ CMDINT(tmp_int) },{ CMDINT(tmp_int2) }}, CMDF_HIDEVARS,
                        "Set the sticky-state of the specified tray.\n" \
          "Syntax: tray_sticky <SHOWING TRAY NUM> <0 for non sticky, sticky otherwise>"},
    { 0, "tray_sticky_alt2",        CMD_TRAY_STICKY_ALT2, {{ 0 }}, CMDF_HIDEVARS,
                        "Toggle the sticky-state of the alt2 tray."},
    { 0, "next_tray",        CMD_TRAY_NEXT, {{ 0 }}, 0,
                        "Go to next tray." },
    { 0, "prev_tray",        CMD_TRAY_PREV, {{ 0 }}, 0,
                        "Go to previous tray." },
    { 0, "next_tray_alt",    CMD_TRAY_NEXT_ALT, {{ 0 }}, 0,
                        "Go to next secondary tray." },
    { 0, "prev_tray_alt",    CMD_TRAY_PREV_ALT, {{ 0 }}, 0,
                        "Go to previous secondary tray." },
    { 0, "next_tray_alt2",    CMD_TRAY_NEXT_ALT2, {{ 0 }}, 0,
                        "Go to next tertiary tray." },
    { 0, "prev_tray_alt2",    CMD_TRAY_PREV_ALT2, {{ 0 }}, 0,
                        "Go to previous tertiary tray." },
    { 0, "next_trays_tray",    CMD_TRAY_NEXT_TRAY, {{ CMDINT(tmp_int) }}, 0,
                        "Go to next trays tray slot." },
    { 0, "prev_trays_tray",    CMD_TRAY_PREV_TRAY, {{ CMDINT(tmp_int) }}, 0,
                        "Go to previous trays tray slot." },
    { 0, "goto_tray",        CMD_TRAY_GOTO, {{ CMDINT(tmp_int) }}, CMDF_HIDEVARS,
                        "Go to specified tray number." },
    { 0, "goto_tray_alt",        CMD_TRAY_GOTO_ALT, {{ CMDINT(tmp_int) }}, CMDF_HIDEVARS,
                        "Go to specified tray number." },
    { 0, "goto_tray_alt2",        CMD_TRAY_GOTO_ALT2, {{ CMDINT(tmp_int) }}, CMDF_HIDEVARS,
                        "Go to specified tray number." },
    { 0, "goto_trays_tray",        CMD_TRAY_GOTO_TRAY, {{ CMDINT(tmp_int) },{ CMDINT(tmp_int2) }}, CMDF_HIDEVARS,
                        "Go to specified tray number in the specified tray.\n" \
                        "Syntax: goto_trays_tray <SHOWING TRAY NUM> <TRAY NUM between 1 and 10>"},
    { 0, "clear_tray",    CMD_TRAY_CLEAR, {{0}}, 0,
                        "Clear all trays (excepting macros)" },
    { 0, "team_select",        CMD_TEAM_SELECT, {{ CMDINT(tmp_int) }}, CMDF_HIDEVARS,
                        "Select Team member." },
    { 0, "pet_select",        CMD_PET_SELECT, {{ CMDINT(tmp_int) }}, CMDF_HIDEVARS,
                        "Select Pet." },
    { 0, "pet_select_name",        CMD_PET_SELECT_NAME, {{ CMDSENTENCE(tmp_str) }}, CMDF_HIDEVARS,
                        "Select Pet." },
    { 0, "unlevelingpact",        CMD_LEVELINGPACT_QUIT, {{ 0 }}, CMDF_HIDEVARS,
    "Bring up the dialog for quitting a leveling pact." },
    // Power and inspiration execution
    { 0, "powexec_name",    CMD_POWEXEC_NAME, {{ CMDSENTENCE(tmp_str)}}, CMDF_HIDEVARS,
                        "Executes a power with the given name." },
    { 0, "powexec_location",	CMD_POWEXEC_LOCATION, {{ CMDSTR(tmp_str)},{CMDSENTENCE(tmp_str2)}}, CMDF_HIDEVARS,
						"Executes a power at a specified location with the given name." },
    { 0, "powexec_slot",    CMD_POWEXEC_SLOT, {{ CMDINT(tmp_int) }}, CMDF_HIDEVARS,
                        "Executes the given power slot from the current tray." },
    { 0, "powexec_toggleon", CMD_POWEXEC_TOGGLE_ON, {{ CMDSENTENCE(tmp_str)}}, CMDF_HIDEVARS,
                        "Toggles a given power on. If its already on, does nothing." },
    { 0, "powexec_toggleoff", CMD_POWEXEC_TOGGLE_OFF, {{ CMDSENTENCE(tmp_str)}}, CMDF_HIDEVARS,
                        "Toggles a given power off. If its already off, does nothing." },
    { 0, "powexec_altslot",    CMD_POWEXEC_ALTSLOT, {{ CMDINT(tmp_int) }}, CMDF_HIDEVARS,
                        "Executes the given power slot from the alternate tray." },
    { 0, "powexec_alt2slot",    CMD_POWEXEC_ALT2SLOT, {{ CMDINT(tmp_int) }}, CMDF_HIDEVARS,
                        "Executes the given power slot from the second alternate tray." },
    { 0, "powexec_serverslot",    CMD_POWEXEC_SERVER_SLOT, {{ CMDINT(tmp_int) }}, CMDF_HIDEVARS,
                        "Executes the given power slot from the server-controlled tray." },
    { 0, "powexec_tray",    CMD_POWEXEC_TRAY, {{ CMDINT(tmp_int) }, { CMDINT(tmp_int2) }}, CMDF_HIDEVARS,
                        "Executes a power in the given slot and tray." },
    { 0, "powexec_abort",    CMD_POWEXEC_ABORT, {0}, 0,
                        "Cancels the auto-attack power and the queued power." },
    { 0, "powexec_unqueue",    CMD_POWEXEC_UNQUEUE, {0}, 0,
                        "Cancels the queued power." },
    { 0, "powexec_auto",    CMD_POWEXEC_AUTO, {{ CMDSENTENCE(tmp_str)}}, CMDF_HIDEVARS,
                        "Sets the auto-attack power to given named power (or blank to shut it off, or toggles if it's on already)." },
    { 0, "inspirationSlot",    CMD_INSPEXEC_SLOT, {{CMDINT(tmp_int)}}, CMDF_HIDEVARS,
                        "Activate an inspiration slot in the first row." },
    { 0, "inspexec_slot",    CMD_INSPEXEC_SLOT, {{CMDINT(tmp_int)}}, CMDF_HIDEVARS,
                        "Activate an inspiration slot in the first row." },
    { 0, "inspexec_tray",    CMD_INSPEXEC_TRAY, {{CMDINT(tmp_int)}, {CMDINT(tmp_int2)}}, CMDF_HIDEVARS,
                        "Activate an inspiration slot in the given row and column." },
    { 0, "inspexec_name",    CMD_INSPEXEC_NAME, {{CMDSENTENCE(tmp_str)}}, CMDF_HIDEVARS,
                        "Activate an inspiration by name." },
    { 0, "inspexec_pet_name",    CMD_INSPEXEC_PET_NAME, {{ CMDSTR(tmp_str)},{CMDSENTENCE(tmp_str2)}}, CMDF_HIDEVARS,
                        "Activate an inspiration on a pet by name. Inspiration Name then Pet Name" },
    { 0, "inspexec_pet_target",    CMD_INSPEXEC_PET_TARGET, {{CMDSENTENCE(tmp_str)}}, CMDF_HIDEVARS,
                        "Activate an inspiration on a pet by name. Takes Inspiration Name" },
    { 0, "nop",            CMD_NOP, {{ 0 }}, CMDF_HIDEPRINT,
                        "does nothing" },
    { 0, "macro",        CMD_MACRO, {{ CMDSTR(tmp_str)},{ CMDSENTENCE(tmp_str2)}}, CMDF_HIDEVARS,
                        "Add a macro to first empty slot.\n"
                        "macro <name> <command"},
	{ 0, "macro_image",		CMD_MACRO_IMAGE, {{ CMDSTR(tmp_str)},{ CMDSTR(tmp_str2)},{ CMDSENTENCE(tmp_str3)}}, CMDF_HIDEVARS,
						"Add a macro to first empty slot, using any texture as the power icon.\n"
						"macro_image <image> <name> <command>"},
    { 0, "macroslot",    CMD_MACRO_SLOT, { { CMDINT(tmp_int) },{ CMDSTR(tmp_str)},{ CMDSENTENCE(tmp_str2)} }, CMDF_HIDEVARS,
                        "Add a macro to provided slot.\n"
                        "macro <slot> <name> <command>"},
    { 0, "manage",        CMD_MANAGE_ENHANCEMENTS, {{0}}, 0,
                        "Go to the enhancement management screen" },
    { 0, "screenshot",    CMD_SCREENSHOT, {{0}}, 0,
                        "Save a .jpg format screenshot." },
    { 0, "screenshottitle",    CMD_SCREENSHOT_TITLE, {CMDSENTENCE(tmp_str)}, 0,
                        "Save a .jpg format screenshot with the given title." },
    { 0, "screenshottga",CMD_SCREENSHOT_TGA, {{0}}, 0,
                        "Save a .tga format screenshot." },
    { 0, "screenshotui",0, {{ CMDINT(game_state.screenshot_ui) }}, 0,
                        "Enables or disables the ui for screenshots (1=ui on, 0=ui off)" },
    { 9, "tutorial",    0, {{ CMDINT(game_state.tutorial) }}, 0,
                        "Turns on tutorial." },
    { 9, "minimapreload",    CMD_MINIMAP_RELOAD, {{0}}, 0,
                        "Reloads the mini-map." },
    { 9, "logfileaccess",    CMD_LOGFILEACCESS, {{CMDINT(tmp_int)}}, CMDF_HIDEVARS,
                        "generate log file of all files loaded" },
    { 0, "copychat",    CMD_COPYCHAT, {CMDSENTENCE(tmp_str)}, CMDF_HIDEVARS,
                        "Copy the entire chat history from specified chat Tab into the clipboard." },
    { 0, "bug",            CMD_BUG, {{CMDSENTENCE(tmp_str)}}, CMDF_HIDEVARS,
                        "Report a bug. Enter a short description of any length." },
    { 0, "bug_internal", CMD_BUG_INTERNAL, {{CMDINT(tmp_int)}, {CMDSENTENCE(tmp_str)}}, CMDF_HIDEVARS|CMDF_HIDEPRINT,
                        "Internal command used to process /bug commands" },
    { 0, "cbug",        CMD_CBUG, {{CMDSENTENCE(tmp_str)}}, CMDF_HIDEVARS|CMDF_HIDEPRINT,
                        "Report a bug (Cryptic Internal Only). Enter a short description of any length." },
    { 2, "csrbug",        CMD_CSR_BUG, {{ CMDSTR(tmp_str)},{ CMDSENTENCE(tmp_str2)}}, CMDF_HIDEVARS,
                        "Submit a bug report for another player."
                        "\nIncludes full bug report info for player, as well CSR's position info is included."
                        "\nSyntax: <Player name> <Summary>"},
    { 2, "qabugreport",    CMD_QA_BUGREPORT, {{CMDSENTENCE(tmp_str)}}, CMDF_HIDEVARS,
                        "Creates a /bug report on your local machine, stored in 'bug_report.log'" },
    { 9, "alwaysmissionmap", CMD_MISSIONMAP, {{CMDINT(game_state.alwaysMission)}}, CMDF_HIDEVARS,
                        "Make the mission map load correctly for game designers" },
    { 4, "noMapFog",    CMD_MAP_FOG, {{CMDINT(game_state.noMapFog)}}, CMDF_HIDEVARS,
                        "Hide the map fog" },
    { 2, "scprint",        CMD_SOUVENIRCLUE_PRINT, {{0}}, 0,
                        "Print all the souvenir clues the player has" },
    { 3, "scgetdetail",    CMD_SOUVENIRCLUE_GET_DETAIL, {{CMDINT(tmp_int)}}, CMDF_HIDEVARS,
                        "Get the detail text on the specified souvenir clue" },
    { 0, "release",        CMD_RELEASE, {{0}}, CMDF_HIDEVARS,
                        "Activate medicom unit fot emergency medical transport." },
    { 0, "petition",    CMD_PETITION, {{CMDSENTENCE(tmp_str)}}, CMDF_HIDEVARS,
                        "add user petition (stuck, cheated, etc) to the database" },
    { 0, "project",        0, {{CMDSENTENCE(app_registry_name)}}, CMDF_HIDEVARS|CMDF_HIDEPRINT,
                        "set project registry key (Coh / CohTest / CohBeta)" },
    { 0, "exitlaunch",    0, {{CMDSENTENCE(game_state.exit_launch)}}, CMDF_HIDEVARS|CMDF_HIDEPRINT,
                        "set program to run when game exits" },
    { 0, "cc",            CMD_COSTUME_CHANGE, {{ CMDINT(tmp_int) }}, 0,
                        "Change costume." },
    { 0, "costume_change", CMD_COSTUME_CHANGE, {{ CMDINT(tmp_int) }}, 0,
                        "Change costume." },
    { 0, "cc_e",        CMD_EMOTE_COSTUME_CHANGE, {{ CMDINT(tmp_int) }, { CMDSTR(tmp_str) }}, 0,
                        "Uses an emote to change costumes." },
    { 0, "cc_emote",     CMD_EMOTE_COSTUME_CHANGE, {{ CMDINT(tmp_int) }, { CMDSTR(tmp_str) }}, 0,
                        "Uses an emote to change costumes." },
    { 0, "window_scale", CMD_WINDOW_SET_SCALE, {{CMDSTR(tmp_str)}, { PARSETYPE_FLOAT, &tmp_f32 }}, 0,
                        "Change a single window scale." },
    { 0, "logchat",        CMD_CHATLOG, {{0}}, 0,
                        "Toggle chat logging" },
    { 0, "assist",        CMD_ASSIST, {{0}}, 0,
                        "Change your current target to selected allies target." },
    { 0, "assist_name",    CMD_ASSIST_NAME, {{CMDSENTENCE(tmp_str)}}, 0,
                        "Change your current target to a specified allies target." },
    { 0, "target_name",    CMD_TARGET_NAME, {{CMDSENTENCE(tmp_str)}}, 0,
                        "Change your current target to any entity matching name specified." },
    { 0, "dialog_yes",    CMD_DIALOGYES, {{0}}, 0,
                        "Answer OK, Yes, or Accept to current dialog" },
    { 0, "dialog_no",    CMD_DIALOGNO, {{0}}, 0,
                        "Answer OK, No, or Cancel to current dialog" },
    { 0, "dialog_answer",CMD_DIALOGANSWER, {{CMDSENTENCE(tmp_str)}}, 0,
                        "Answer dialog with button matching provided text" },

    { 9, "costume_save",CMD_COSTUME_SAVE, {{CMDSTR(tmp_str)}}, 0,
                        "Saves the costume to specified file" },
    { 9, "costume_load",CMD_COSTUME_LOAD, {{CMDSTR(tmp_str)}}, 0,
                        "Loads the costume from a specified file" },
    { 0, "renderthread",0, {{CMDINT(tmp_int)}}, 0,    // actually handled by parseargs0
                        "" },
    { 0, "norenderthread",0, {{CMDINT(tmp_int)}}, 0,    // actually handled by parseargs0
                        "" },
    { 9, "renderthreaddebug",0, {{CMDINT(game_state.renderThreadDebug)}}, 0,
                        "Forces render thread to stall until a synchronous transaction is queued" },
    { 0, "maxColorTrackerVerts",0, {{CMDINT(game_state.maxColorTrackerVerts)}}, 0,
                        "Maximum number of world object vertices to relight per frame" },
    { 0, "fullRelight",0, {{CMDINT(game_state.fullRelight)}}, 0,
                        "Do not cap number of relit vertices per frame" },
    { 0, "useNewColorPicker",0, {{CMDINT(game_state.useNewColorPicker)}}, 0,
                        "Use new color picker in editor." },
	{ 0, "sg_enter_passcode", CMD_SG_ENTER_PASSCODE, {{0}}, 0,
						"Enter a supergroup base access passcode." },

#ifdef E3_SCREENSHOT
    { 0, "e3screenshot", 0, {{ CMDINT(game_state.e3screenshot) }}, 0,
                        "Enables special e3 2004 screenshot mode" },
#endif

    { 9, "texoff",        0, {{ CMDINT(game_state.texoff) }}, 0,
                        "Turns world texture changes off."},
    { 9, "scomp",        0, {{ CMDINT(game_state.showcomplexity)}}, 0,
                        "Displays the scene complexity information."},
    { 9, "rdrstats",    0, {{ CMDINT(game_state.showrdrstats)}}, 0,
                        "Displays the render statistics information."},
    { 9, "costbars",    0, {{ CMDINT(game_state.costbars) }}, 0,
                        "Display detailed scene complexity bars."},
    { 9, "simplestats",    CMD_SIMPLESTATS, {{ 0 }}, 0,
                        "Show render stats for artists."},
    { 9, "costgraph",    0, {{ CMDINT(game_state.costgraph) }},0,
                        "Displays scene complexity information." },
    { 9, "addcardstats",CMD_ADD_CARD_STATS, {{ CMDSTR(tmp_str) }},0,
                        "Add stats for a card, by name." },
    { 9, "remcardstats",CMD_REM_CARD_STATS, {{ CMDSTR(tmp_str) }},0,
                        "Remove stats for a card, by name." },
    { 9, "cardstats",    0, {{ CMDINT(game_state.stats_cards) }},0,
                        "Bitfield.  Sets which cards to display on the cost bars and cost graph." },
    { 2, "mapsetpos",    0, {{ CMDINT(game_state.mapsetpos) }}, 0,
                        "Allows setting position by right clicking on the map."},
    { 9, "conorhack",    0, {{ CMDINT(game_state.conorhack) }}, 0,
                        "Enables Conor's hack of the day."},
//    { 9, "occlusiontest",0, {{ CMDINT(game_state.occlusiontest) }}, 0,
//                        "Gives an estimate on how many static world objects are being drawn and how many are actually visible."},
    { 9, "atlas_stats", 0, {{ CMDINT(game_state.atlas_stats) }}, 0,
                        "Displays the stats for the texture atlas system."},
    { 9, "atlas_display", 0, {{ CMDINT(game_state.atlas_display) }}, 0,
                        "Selects which texture atlas to display with atlas_stats."},
    { 9, "atlas_display_next", CMD_ATLAS_DISPLAY_NEXT, {{ 0 }}, 0,
                        "Selects next texture atlas to display with atlas_stats."},
    { 9, "atlas_display_prev", CMD_ATLAS_DISPLAY_PREV, {{ 0 }}, 0,
                        "Selects previous texture atlas to display with atlas_stats."},
    { 9, "dont_atlas",    0, {{ CMDINT(game_state.dont_atlas) }}, 0,
                        "Command line option to disable texture atlasing."},
    { 9, "debug_cubemap_texture",0, {{CMDINT(game_state.debugCubemapTexture)}}, 0,
                        "Crash the game as soon as we're unable to find the cubemap texture.  Support in debugging #22683" },

    { 9, "mapsep",        0, {{ CMDINT(g_mapSepType) }}, 0,
                        "Set separation of icons on mini map." },
    { 9, "resetseps",    CMD_RESET_SEPS, {{0}}, 0, "Reset separation of icons on mini map." },

    { 9, "ls_salvagedefs", CMD_SALVAGEDEFS_LIST, {{0}}, 0, "List the salvage defs." },
    { 9, "ls_conceptdefs", CMD_CONCEPTDEFS_LIST, {{0}}, 0, "List the concept defs." },
    { 9, "ls_recipedefs", CMD_RECIPEDEFS_LIST, {{0}}, 0, "List the recipe defs." },
    { 9, "ls_salvage", CMD_SALVAGEINV_LIST, {{0}}, 0, "List the player's salvage." },
    // Reverse engineering is an old, deprecated system. This command gets
    // typoed for "rw_salvage" sometimes, so it's better just to disable it.
//    { 9, "re_salvage", CMD_SALVAGE_REVERSE_ENGINEER, {{ CMDINT(tmp_int) }}, 0, "reverse engineer the the salvage item <index>." },
    { 9, "ls_concept", CMD_CONCEPTINV_LIST, {{0}}, 0, "List the player's concepts." },
    { 9, "ls_recipe", CMD_RECIPEINV_LIST, {{0}}, 0, "List the player's recipes." },
    { 9, "ls_detail", CMD_DETAILINV_LIST, {{0}}, 0, "List the player's salvage." },
    { 9, "invent_recipe", CMD_INVENT_SELECT_RECIPE, {{ CMDINT(tmp_int) }, { CMDINT(tmp_int2) }}, 0, "select the recipe for hardening by index.\n <inv index> <level to invent at>" },
    { 9, "ls_invent", CMD_INVENTION_LIST, {{0}}, 0, "select the recipe for hardening by index." },
    { 9, "uiinventory_show", CMD_UIINVENTORY_VISIBILITY, {{ CMDINT(tmp_int) }}, 0, "select the recipe for hardening by index." },
    { 9, "slot_invent", CMD_INVENT_SLOT_CONCEPT, {{ CMDINT(tmp_int) },{CMDINT(tmp_int2)}}, 0, "slot the concept item <conceptindex> into slot <slotindex>." },
    { 9, "unslot_invent", CMD_INVENT_UNSLOT, {{CMDINT(tmp_int)}}, 0, "unslot slot <slotindex>." },
    { 9, "harden_invent", CMD_INVENT_HARDEN, {{ CMDINT(tmp_int) }}, 0, "harden the slotted concept in this recipe by <slot idx>." },
    { 9, "fin_invent", CMD_INVENT_FINALIZE, {{0}}, 0, "finish inventing" },
    { 9, "cancel_invent", CMD_INVENT_CANCEL, {{0}}, 0, "cancel inventing" },
    { 9, "ls_boost", CMD_ENHANCEMENT_LIST, {{ CMDINT(tmp_int) }}, 0, "show the vars of an ehancement." },
    { 9, "pickPets", CMD_ARENA_MANAGEPETS,  {{0}}, 0,
                        "Show UI for Managing your pet army" },
    { 9, "gladiators", CMD_ARENA_MANAGEPETS,  {{0}}, 0,
                    "Show UI for Managing your pet army" },
    { 9, "norestexfree",0, {{ CMDINT(game_state.noTexFreeOnResChange) }}, 0,
                        "Prevents freeing textures on a change of screen resolution."},
    { 9, "hideCollisionVolumes",0, {{ CMDINT(game_state.hideCollisionVolumes)}}, 0,
                        "Hides collision volumes when in the editor."},
    { 9, "zocclusion_off",0, {{ CMDINT(game_state.zocclusion_off) }}, 0,
                        "Force z-occlusion off."},
    { 9, "zo_debug",0, {{ CMDINT(game_state.zocclusion_debug), 4 }}, 0,
                        "Turn on z-occlusion test debugging."},
    { 9, "cubeface",0, {{ CMDINT(game_state.display_cubeface) }}, 0,
                        "display the given cubemap face (1-6)"},
    { 9, "forceCubeFace",0, {{ CMDINT(game_state.forceCubeFace) }}, 0,
                        "render only the given cubemap face every frame (0-6)"},
    { 9, "charsInCubemap",0, {{ CMDINT(game_state.charsInCubemap) }}, 0,
                        "render characters in cubemap"},
    { 9, "staticcubemapterrain",0, {{ CMDINT(game_state.static_cubemap_terrain) }}, 0,
                        "use static cubemap for terrain"},
    { 9, "staticcubemapchars",0, {{ CMDINT(game_state.static_cubemap_chars) }}, 0,
                        "use static cubemap for chars"},
    { 9, "cubemapUpdate4Faces",0, {{ CMDINT(game_state.cubemapUpdate4Faces) }}, 0,
                        "Update cubemap faces 4 at a time"},
    { 9, "cubemapUpdate3Faces",0, {{ CMDINT(game_state.cubemapUpdate3Faces) }}, 0,
                        "Update cubemap faces 3 at a time"},
    { 9, "roomheight",CMD_ROOMHEIGHT,{{ CMDINT(tmp_int) },{ CMDINT(tmp_int2) }},0,
                        "test base block editing <floor> <ceiling>"},
    { 9, "doorway",CMD_DOORWAY,{{ CMDINT(tmp_int) },{ CMDINT(tmp_int2) },{ CMDINT(tmp_int3) }},0,
                        "test base doorway editing <x> <z> <height>"},
    { 9, "makeroom",CMD_MAKEROOM,{{ CMDINT(tmp_int) },{ CMDINT(tmp_int2) },{ CMDINT(tmp_int3) },{PARSETYPE_VEC3, tmp_vec},},0,
                        "test create room <x> <z>"},
    { 9, "editbase",CMD_EDIT_BASE,{{ CMDINT(tmp_int) },},0,
                        "enable base editing"},
    { 9, "base_workshop_mode",CMD_BASE_WORKSHOP_SETMODE,{{ CMDINT(tmp_int) },},0,
                        "toggle workshop mode"},
    { 9, "ls_base_recipes",CMD_BASE_WORKSHOP_RECIPES_LIST,{{0}},0,
                        "show available base detail recipes."},
    { 9, "base_recipe_craft",CMD_BASE_WORKSHOP_RECIPE_CREATE,{{ CMDSTR(tmp_str) }},0,
                        "craft a recipe by name (use list recipes command to see names)"},
    { 9, "roomlook",CMD_ROOMLOOK,{{ CMDINT(game_state.room_look[0]) },{ CMDINT(game_state.room_look[1]) },},0,
                        "pick a room to look at"},
    { 9, "roomlight",CMD_ROOMLIGHT,{{ CMDINT(tmp_int) },{PARSETYPE_VEC3, tmp_vec},},0,
                        "swap tex <type> to <name> for current room"},
    { 9, "roomdetail",CMD_ROOMDETAIL,{{PARSETYPE_VEC3, tmp_vec},},0,
                        "moves current detail object to <x> <y> <z>"},
    { 9, "randombase",0,{{ CMDINT(game_state.random_base) }},0,
                        "test detail objects <cmd 0=add, 1=del, 2=move> <x> <y> <z>"},
    { 9, "baseblocksize",0,{{ CMDINT(game_state.base_block_size) }},0,
                        ""},
    { 0, "localtime", CMD_LOCAL_TIME, {{0}}, 0,
                        "Prints local time (on your computer)"},
    { 9, "timeoffset", 0, {{CMDINT(timing_debug_offset)}}, 0,
                        "Seconds to offset the timing functions from the real time (debug)" },
    { 9, "timedebug",    CMD_TIMEDEBUG, {{0}}, 0,
                        "Show debug info about time" },
    { 0, "autoperf", 0, {{ CMDINT(game_state.auto_perf) }},0,
                        "Automatically change world detail for performance."},
    { 0, "petcom",        CMD_PETCOMMAND, {{CMDSENTENCE(tmp_str)}}, 0,
                        "Set the stance or action of current pet."
                        "\nSyntax: petcom_name \"<pet name>\" <stance, action, or both>"},
    { 0, "petcom_name",    CMD_PETCOMMAND_BY_NAME, {{CMDSTR(tmp_str)},{CMDSENTENCE(tmp_str2)}}, 0,
                        "Set the stance or action of a specific pet."
                        "\nSyntax: petcom_name \"<pet name>\" <stance, action, or both>"},
    { 0, "petcom_pow",    CMD_PETCOMMAND_BY_POWER, {{CMDSTR(tmp_str)},{CMDSENTENCE(tmp_str2)}}, 0,
                        "Set the stance or action of a all pets cast by power."
                        "\nSyntax: petcom_name \"<power name>\" <stance, action, or both>"},
    { 0, "petcom_all",    CMD_PETCOMMAND_ALL, {{CMDSENTENCE(tmp_str)}}, 0,
                        "Set the stance or action of a specific pet."
                        "\nSyntax: petcom_name <stance, action, or both>"},
    { 0, "petsay",        CMD_PETSAY, {{CMDSENTENCE(tmp_str)}}, 0,
                        "Make your current pet say something or emote."},
    { 0, "petsay_name",    CMD_PETSAY_BY_NAME, {{CMDSTR(tmp_str)},{CMDSENTENCE(tmp_str2)}}, 0,
                        "Make the named pet say something or emote."},
    { 0, "petsay_pow",    CMD_PETSAY_BY_POWER, {{CMDSTR(tmp_str)},{CMDSENTENCE(tmp_str2)}}, 0,
                        "Make all pets created by given power say something or emote."},
    { 0, "petsay_all",    CMD_PETSAY_ALL, {{CMDSENTENCE(tmp_str)}}, 0,
                        "Make the named pet say something or emote."},
    { 0, "petrename",    CMD_PETRENAME, {{CMDSTR(tmp_str)}}, 0,
                        "Rename your current pet."},
    { 0, "petrename_name",    CMD_PETRENAME_BY_NAME, {{CMDSTR(tmp_str)},{CMDSTR(tmp_str2)}}, 0,
                        "Rename the named pet."},
    { 1, "console_scale", 0, {{ PARSETYPE_FLOAT, &game_state.console_scale}}, 0,
                        "Scales the console."},
    { 9, "forcelod",    0, {{ CMDINT(game_state.lod_entity_override) }}, 0,
                        "Forces all entity LODs to the given value - 1."},
    { 0, "safemode",    0, {{ CMDINT(game_state.safemode) }}, CMDF_HIDEPRINT,
                        "Forces most compatible graphics and audio settings."},
    { 0, "nooverride",    0, {{ CMDINT(tmp_int)}}, CMDF_HIDEPRINT,
                        "Disallows override files (command line option)."},
    { 0, "createHero",        CMD_CREATEHERO, {{ 0 }}, CMDF_HIDEPRINT,
                        "Create a hero."},
    { 0, "createVillain",    CMD_CREATEVILLAIN, {{ 0 }}, CMDF_HIDEPRINT,
                        "Create a villain."},
    { 0, "createPraetorian", CMD_CREATEPRAETORIAN, {{ 0 }}, CMDF_HIDEPRINT,
                        "Create a Praetorian."},
    { 0, "nagPraetorian", CMD_NAGPRAETORIAN, {{ 0 }}, CMDF_HIDEPRINT,
                        "Explain the need to buy Going Rogue."},
    { 0, "markPowOpen",        CMD_MARKPOWOPEN, {{ CMDINT(tmp_int)  }}, CMDF_HIDEPRINT,
                        "Mark power open"},
    { 0, "markPowClosed",    CMD_MARKPOWCLOSED, {{ CMDINT(tmp_int)  }}, CMDF_HIDEPRINT,
                        "Mark power closed"},
    { 9, "base_hidden_details", 0, {{CMDINT(gShowHiddenDetails)}}, 0,
                        "Set rather the base inventory shows all types of items, including hidden ones"},
    { 9, "hidetrans",    CMD_HIDETRANS, {{ CMDINT(tmp_int) }}, CMDF_HIDEPRINT,
                        "Hide translation errors."},
    { 9, "recalclights", CMD_RECALCLIGHTS, {{ 0 }}, 0,
                        "Recalclate static (vertex) lighting." },
    { 9, "recalcwelds", CMD_RECALCWELDS, {{ 0 }}, 0,
                        "Recalclate welded models." },
    { 9, "nowelding",    0, {{ CMDINT(game_state.no_welding) }}, 0,
                        "Turn off object welding."},
    { 9, "dumpparserstructs", CMD_DUMPPARSERALLOCS, {{ 0 }}, 0,
                        "Dump ParserAllocStruct log to the console."},
    { 9, "textparserdebug", CMD_TEXTPARSERDEBUG, {{ 0 }}, 0,
                        "Run debug command on textparser." },
    { 9, "showportals", 0, {{ CMDINT(game_state.show_portals) }}, 0,
                        "Draw the bounding boxes of tray portals in purple."},
    { 9, "editor_mouse_invert", 0, {{ CMDINT(control_state.editor_mouse_invert) }},0,
                        "Invert meaning of mouseY for mouselook in the editor." },
    { 9, "missioncontrol", 0, {{ CMDINT(game_state.missioncontrol)}},0,
                        "Toggles Mission Control." },
    { 9, "toggle_fffp",    CMD_TOGGLEFIXEDFUNCTIONFP, {{ 0 }}, 0,
                        "Toggle using a fragment program for fixed function pipeline operations." },
    { 9, "toggle_ffvp",    CMD_TOGGLEFIXEDFUNCTIONVP, {{ 0 }}, 0,
                        "Toggle using a vertex program for fixed function pipeline operations." },
    { 9, "costume_diff", CMD_COSTUME_DIFF, {{ CMDINT(tmp_int) }}, 0,
                        "View changes to the costume data" },
    { 9, "debugtex",    0, {{ PARSETYPE_FLOAT, &game_state.debug_tex }}, 0,
                        "Show debug texture, with scaling." },
    { 0, "camturn",        CMD_CAMTURN, {0},0,
                        "Turn camera to match player" },
    { 0, "playerturn",    CMD_PLAYERTURN, {0},0,
                        "Turn player to match camera" },
    { 0, "face",        CMD_FACE, {0},0,
                        "Turn player to face target" },
    { 0, "windowcloseextra", CMD_WINDOW_CLOSE_EXTRA, {{0}}, 0,
                        "Leave fullscreen, close dialogs, and close non-essential windows" },
    { 0, "gamereturn", CMD_WINDOW_CLOSE_EXTRA, {{0}}, 0,
                        "Leave fullscreen, close dialogs, and close non-essential windows" },
    { 0, "clearchat",    CMD_CLEARCHAT, {{ 0 }}, 0,
                        "Clear all chat buffers" },
    { 0, "keybind_reset", CMD_RESET_KEYBINDS, {{0}}, 0,
                        "Reset keybinds." },
    { 0, "unbind_all", CMD_RESET_KEYBINDS, {{0}}, 0,
                        "Reset keybinds." },
    { 9, "critter_costume_cycle", CMD_CRITTER_CYCLE, {{0}}, 0,
                        "Make costume master from all available npc parts" },
    { 9, "search_visible", 0, {{ CMDINT(game_state.search_visible) }}, 0,
                        "Creates a search result window in the editor of all objects being drawn at the time of the command." },
    { 9, "transformToolbar", 0, {{ CMDINT(game_state.transformToolbar) }}, 0,
                        "Toggles the transform toolbar in the editor." },
    { 1, "ctstoggle",    CMD_CTS_TOGGLE, {{ CMDINT(tmp_int) }}, 0,
                        "Toggles the click to source display." },
    { 9, "save_csv",    CMD_SAVE_CSV, {{ CMDSTR(tmp_str) }}, 0,
                        "Save current player's costume to a CSV file." },
    { 9, "load_csv",    CMD_LOAD_CSV, {{ CMDSTR(tmp_str) }}, 0,
                        "load a costume from a CSV file onto the current player." },
    { 9, "trainingroom",    CMD_TRAININGROOM_CLIENT, {{ CMDINT(game_state.trainingshard) }}, 0,
                        "load a costume from a CSV file onto the current player." },
    { 9, "authlocalshow",    CMD_AUTH_LOCAL_SHOW, {{0}}, 0,
                        "Show the auth bits on the client, mainly only used for character creation." },
    { 9, "authlocalset",    CMD_AUTH_LOCAL_SET, {{CMDSTR(tmp_str)},{CMDINT(tmp_int)}}, 0,
                        "Change an auth bit on the client, see 'authlocalshow' for a list, also Raw0-3 and OrRaw0-3." },
    { 9, "ui_region_squish",        CMD_UI_REGION_SQUISH, {{PARSETYPE_FLOAT, &LEFT_COLUMN_SQUISH, sizeof(LEFT_COLUMN_SQUISH)}}, 0,
                        "squish left column by this much (0.0 - 1.0)" },
    { 1, "crashclient",    CMD_CRASHCLIENT,{{0}},0,
                        "blarg" },
    { 0, "monitor_attribute", CMD_MONITORATTRIBUTE, {{CMDSENTENCE(tmp_str)}}, 0,
                        "Adds attribute to Attribute Monitor" },
    { 0, "stop_monitor_attribute", CMD_STOPMONITORATTRIBUTE, {{CMDSENTENCE(tmp_str)}}, 0,
                        "Removes attribute from the Attribute Monitor" },
    { 0, "insp_combine", CMD_COMBINEINSP, {{CMDSTR(tmp_str)},{CMDSTR(tmp_str2)}}, 0,
                        "Combines 3 of same type inspiration into one new type" },
    { 0, "insp_delete", CMD_DELETEINSP, {{CMDSENTENCE(tmp_str)}}, 0,
                        "Removes and inspiration" },
    { 0, "link_interact", CMD_CHATLINK, {{CMDSENTENCE(tmp_str)}}, 0,
                        "Activates Context Menu for link name" },
    { 0, "link_info", CMD_TEXTLINK, {{CMDSENTENCE(tmp_str)}}, 0,
                        "Provides info window on matching link name" },
    { 0, "link_interact_global", CMD_CHATLINKGLOBAL, {{CMDSTR(tmp_str)},{CMDSENTENCE(tmp_str2)}}, 0,
                        "Activates Context Menu for link global name" },
    { 0, "link_channel", CMD_CHATLINKCHANNEL, {{CMDSENTENCE(tmp_str)}}, 0,
                        "Activates Context Menu for link channel name" },
    { 0, "showNewTray", CMD_NEWTRAY, {{0}}, 0,
                        "Opens another Tray Window" },
    { 0, "hide", CMD_HIDE, {{0}}, 0,
                        "Opens Hide Options" },
    { 0, "ghide", CMD_HIDE, {{0}}, 0,
                        "Opens Hide Options" },
    { 0, "hide_search", CMD_HIDE_SEARCH, {{0}}, 0,
                        "Hides from search" },
    { 0, "hide_sg", CMD_HIDE_SG, {{0}}, 0,
                        "Hides from Supergroup" },
    { 0, "hide_friends", CMD_HIDE_FRIENDS, {{0}}, 0,
                        "Hides from Friends" },
    { 0, "hide_gfriends", CMD_HIDE_GFRIENDS, {{0}}, 0,
                        "Hides from Global Friends " },
    { 0, "hide_gchannels", CMD_HIDE_GCHANNELS, {{0}}, 0,
                        "Hides from Global Channels" },
    { 0, "unhide", CMD_HIDE, {{0}}, 0,
                        "Opens Hide Options" },
    { 0, "gunhide", CMD_HIDE, {{0}}, 0,
                        "Opens Hide Options" },
    { 0, "unhide_search", CMD_UNHIDE_SEARCH, {{0}}, 0,
                        "unHides from search" },
    { 0, "unhide_sg", CMD_UNHIDE_SG, {{0}}, 0,
                        "unHides from Supergroup" },
    { 0, "unhide_friends", CMD_UNHIDE_FRIENDS, {{0}}, 0,
                        "unHides from Friends" },
    { 0, "unhide_gfriends", CMD_UNHIDE_GFRIENDS, {{0}}, 0,
                        "unHides from Global Friends " },
    { 0, "unhide_gchannels", CMD_UNHIDE_GCHANNELS, {{0}}, 0,
                        "unHides from Global Channels" },
    { 0, "hide_tell", CMD_HIDE_TELL, {{0}}, 0,
                        "Hides from tells" },
    { 0, "unhide_tell", CMD_UNHIDE_TELL, {{0}}, 0,
                        "unHides from tells" },
    { 0, "hide_invite", CMD_HIDE_INVITE, {{0}}, 0,
                        "Hides from invites" },
    { 0, "unhide_invite", CMD_UNHIDE_INVITE, {{0}}, 0,
                        "unHides from invites" },
    { 0, "hide_all", CMD_HIDE_ALL, {{0}}, 0,
                        "Sets all hide setting on" },
    { 0, "unhide_all", CMD_UNHIDE_ALL, {{0}}, 0,
                        "Sets all hide settings off" },
    { 0, "set_powerinfo_class", CMD_SET_POWERINFO_CLASS, {{0}}, CMDF_HIDEPRINT,
        "Brings up context menu for choosing class for power info display" },
    { 0, "sgk", CMD_SGKICK, {{CMDSENTENCE(tmp_str)}}, 0,
        "Kicks player from supergroup" },
    { 0, "sgkick", CMD_SGKICK, {{CMDSENTENCE(tmp_str)}}, 0,
        "Kicks player from supergroup" },
    { 0, "mouse_speed",    CMD_MOUSE_SPEED, {{ PARSETYPE_FLOAT, &tmp_f32 }}, 0,
        "Sets the mouse speed" },
    { 0, "mouse_invert",    CMD_MOUSE_INVERT, {{ CMDINT(tmp_int) }}, 0,
         "Inverts the mouse"},
    { 0, "speed_turn", CMD_SPEED_TURN, {{ PARSETYPE_FLOAT, &tmp_f32 }}, 0,
        "Sets the turning speed" },
    { 0, "option_list",    CMD_OPTION_LIST, {{0}}, 0,
        "Lists option names." },
    { 0, "option_set", CMD_OPTION_SET, {{CMDSTR(tmp_str)}, {CMDSTR(tmp_str2)} }, 0,
        "Sets an Option" },
    { 0, "option_toggle",    CMD_OPTION_TOGGLE, {{CMDSTR(tmp_str)}}, 0,
        "Toggles an Option" },
    { 0, "playernote",    CMD_PLAYER_NOTE, {{CMDSTR(tmp_str)}}, 0,
        "Opens note window for a given global playername" },
    { 0, "playernotelocal",    CMD_PLAYER_NOTE_LOCAL, {{CMDSTR(tmp_str)}}, 0,
        "Opens note window for a given local playername. (requires a valid name)" },        
    { 1, "url_env",    0, { {CMDINT(game_state.url_env) }},0,
                        "use URLs when in debug mode: 0 = default 1 = dev 2 = live."  },
    { 1, "test_login_fail",    CMD_TEST_LOGIN_FAIL, {{0}},0,
                        "pop open the login failure dialog."  },
    { 1, "custom_window",    CMD_CUSTOM_WINDOW, {{CMDSTR(tmp_str)}},0,
            "Create a new custom window"  },
    { 1, "custom_window_toggle",    CMD_CUSTOM_TOGGLE, {{CMDSENTENCE(tmp_str)}},0,
            "Open/Close a custom window"  },
    { 0, "mmscrollsetViewList", CMD_MISSIONMAKER_NAV, {{CMDSENTENCE(tmp_str)}}, CMDF_HIDEPRINT,
            "Open the scroll selector" },
    { 0, "mmscrollsetToggleRegion", CMD_MISSIONMAKER_NAV2, {{CMDSENTENCE(tmp_str)}}, CMDF_HIDEPRINT,
            "Toggle a region open" },
    { 0, "architectsaveandtest", CMD_ARCHITECTSAVETEST, {{0}}, CMDF_HIDEPRINT,
            "Save mission being edited and return to search window" },
    { 0, "architectsaveandexit", CMD_ARCHITECTSAVEEXIT, {{0}}, CMDF_HIDEPRINT,
            "Save mission being edited and test" },
    { 0, "architectfixerrors", CMD_ARCHITECTFIXERRORS, {{0}}, CMDF_HIDEPRINT,
            "Fix Errors" },
    { 0, "architectexit", CMD_ARCHITECTEXIT, {{0}}, CMDF_HIDEPRINT,
            "Return to the mission maker" },
    { 0, "architectrepublish", CMD_ARCHITECTREPUBLISH, {{0}}, CMDF_HIDEPRINT,
            "Republish mission" },
    { 1, "architect_search_authored", CMD_ARCHITECT_CSRSEARCH_AUTHORED, {{CMDINT(tmp_int)},{CMDSTR(tmp_str)}}, CMDF_HIDEPRINT,
            "special search for arcs authored by <authid> <authregion> , results will show in architect window" },
    { 1, "architect_search_reported", CMD_ARCHITECT_CSRSEARCH_COMPLAINED, {{CMDINT(tmp_int)},{CMDSTR(tmp_str)}}, CMDF_HIDEPRINT,
            "special search for arcs reported for content by <authid> <authregion> , results will show in architect window" },
    { 1, "architect_search_voted", CMD_ARCHITECT_CSRSEARCH_VOTED, {{CMDINT(tmp_int)},{CMDSTR(tmp_str)}}, CMDF_HIDEPRINT,
            "special search for arcs voted on by <authid> <authregion> , results will show in architect window" },
    { 1, "architect_search_played", CMD_ARCHITECT_CSRSEARCH_PLAYED, {{CMDINT(tmp_int)},{CMDSTR(tmp_str)}}, CMDF_HIDEPRINT,
            "special search for arcs plyed by <authid> <authregion> , results will show in architect window" },
    { 1, "architect_get_file", CMD_ARCHITECT_GET_FILE, {{CMDINT(tmp_int)}}, CMDF_HIDEPRINT,
            "downloads the specified arc" },
    { 0, "architect_save_compressed_costumes", CMD_ARCHITECT_TOGGLE_SAVE_COMPRESSION, {{CMDINT(tmp_int)}}, CMDF_HIDEPRINT , 
            "Toggle save file output to show compressed or uncompressed." },
    { 9, "architect_ignore_unlockable_validation", CMD_ARCHITECT_IGNORE_UNLOCKABLE_VALIDATION, {{CMDINT(tmp_int)}}, CMDF_HIDEPRINT, 
            "1 will temporarily ignore local critter validation. For debug/testing only. Will disable itself when editing a local arc."                },
    { 9, "smf_drawDebugBoxes", CMD_SMF_DRAWDEBUGBBOXES, {{CMDINT(tmp_int)}}, CMDF_HIDEPRINT , 
            "Draw smf debug boxes. Valid options are 0,1,2." },
    { 9, "gender", CMD_GENDER_MENU, {{0}}, CMDF_HIDEPRINT, 
        "Gender Menu"    },
    { 0, "helpwindow", CMD_OPEN_HELP, {{0}}, 0,
            "Show help window" },
    { 9, "makemaptextures", CMD_MAKEMAPTEXTURES, {{0}}, 0, 
            "Generate mission maker map textures for current map" }, 
    { 9, "testPowerCust", CMD_TESTPOWERCUST, {{CMDINT(tmp_int)},{CMDSTR(tmp_str)},{CMDSTR(tmp_str2) }},0,
        "testPowerCust <n> <color1> <color2> - Grant all primary and secondary powers, select the nth power theme, and set primary and secondary tint colors (FF0000 = saturated red)." },
    { 9, "debugPowerCustAnim", 0, {{ CMDINT(game_state.debugPowerCustAnim) }},0,
                        "Print state bits for animations displayed in the power customization menu." },
    { 9, "disable2passalphawater", 0, {{ CMDINT(game_state.disable_two_pass_alpha_water_objects) }}, 0,
                        "Disables two pass alpha object rendering for underware objects" },
    { 9, "waterRefractionSkewNormalScale", 0, {{ PARSETYPE_FLOAT, &game_state.waterRefractionSkewNormalScale}},0,
                        "scales the water skew contribution from the bump map" },
    { 9, "waterRefractionSkewDepthScale", 0, {{ PARSETYPE_FLOAT, &game_state.waterRefractionSkewDepthScale}},0,
                        "scales the water skew contribution from the depth difference" },
    { 9, "waterRefractionSkewMinimum", 0, {{ PARSETYPE_FLOAT, &game_state.waterRefractionSkewMinimum}},0,
                        "sets the minimum skew amount" },
    { 9, "waterReflectionOverride", 0, {{ CMDINT(game_state.waterReflectionOverride) }},0,
                        "ignore trick water reflection strenght and skew scale values" },
    { 9, "waterReflectionSkewScale", 0, {{ PARSETYPE_FLOAT, &game_state.waterReflectionSkewScale}},0,
                        "sets the amount of skewing of the reflection" },
    { 9, "waterReflectionStrength", 0, {{ PARSETYPE_FLOAT, &game_state.waterReflectionStrength}},0,
                        "sets the strength of the reflection" },
    { 9, "waterReflectionNightReduction", 0, {{ PARSETYPE_FLOAT, &game_state.waterReflectionNightReduction}},0,
                        "percent to reduce the strength of the reflection at night" },
    { 9, "waterFresnelBias", 0, {{ PARSETYPE_FLOAT, &game_state.waterFresnelBias}},0,
                        "sets the bias value of the fresnel approximation equation" },
    { 9, "waterFresnelScale", 0, {{ PARSETYPE_FLOAT, &game_state.waterFresnelScale}},0,
                        "sets the scale value of the fresnel approximation equation" },
    { 9, "waterFresnelPower", 0, {{ PARSETYPE_FLOAT, &game_state.waterFresnelPower}},0,
                        "sets the power value of the fresnel approximation equation" },
    { 9, "reflectivityOverride", 0, {{ CMDINT(game_state.reflectivityOverride) }},0,
                        "Override fresnel reflection values from trick" },
    { 9, "buildingFresnelBias", 0, {{ PARSETYPE_FLOAT, &game_state.buildingFresnelBias}},0,
                        "sets the bias value of the fresnel approximation equation" },
    { 9, "buildingFresnelScale", 0, {{ PARSETYPE_FLOAT, &game_state.buildingFresnelScale}},0,
                        "sets the scale value of the fresnel approximation equation" },
    { 9, "buildingFresnelPower", 0, {{ PARSETYPE_FLOAT, &game_state.buildingFresnelPower}},0,
                        "sets the power value of the fresnel approximation equation" },
    { 9, "buildingReflectionNightReduction", 0, {{ PARSETYPE_FLOAT, &game_state.buildingReflectionNightReduction}},0,
                        "percent to reduce the strength of the reflection at night" },
    { 9, "skinnedFresnelBias", 0, {{ PARSETYPE_FLOAT, &game_state.skinnedFresnelBias}},0,
                        "sets the bias value of the fresnel approximation equation" },
    { 9, "skinnedFresnelScale", 0, {{ PARSETYPE_FLOAT, &game_state.skinnedFresnelScale}},0,
                        "sets the scale value of the fresnel approximation equation" },
    { 9, "skinnedFresnelPower", 0, {{ PARSETYPE_FLOAT, &game_state.skinnedFresnelPower}},0,
                        "sets the power value of the fresnel approximation equation" },

    { 9, "reflectNormals", 0, {{ CMDINT(game_state.reflectNormals) }}, 0,
                        "enable use of normalmap normals for reflections" },
    { 9, "cubemapAttenuationStart", 0, {{ PARSETYPE_FLOAT, &game_state.cubemapAttenuationStart}},0,
                        "distance from camera to start attenuating cubemap reflectivity (in model radius units)" },
    { 9, "cubemapAttenuationEnd", 0, {{ PARSETYPE_FLOAT, &game_state.cubemapAttenuationEnd}},0,
                        "distance from camera to end attenuating cubemap reflectivity (in model radius units)" },
    { 9, "cubemapAttenuationSkinStart", 0, {{ PARSETYPE_FLOAT, &game_state.cubemapAttenuationSkinStart}},0,
                        "distance from camera to start attenuating cubemap reflectivity (in model radius units) for skinned models" },
    { 9, "cubemapAttenuationSkinEnd", 0, {{ PARSETYPE_FLOAT, &game_state.cubemapAttenuationSkinEnd}},0,
                        "distance from camera to end attenuating cubemap reflectivity (in model radius units) for skinned models" },
    { 9, "newfeaturetoggle", 0, {{ CMDINT(game_state.newfeaturetoggle) }},0,
                        "toggle new features on/off (1=ssao, 2=allbutssao, 3=all" },

    { 9, "batchdump", 0, {{ CMDINT(game_state.dbg_log_batches) }},0,
                        "create batch dump logs (opaque, alpha, etc.) for each viewport at root of c drive " },

    { 9, "reloadpophelp", CMD_RELOAD_POP_HELP, {{0}}, 0,
                        "Reload Pop Help" },

    { 9, "script_window", CMD_SCRIPT_WINDOW, {{ CMDINT(tmp_int) }}, 0,
                        "Pop open the zone event UI window" },

    // TODO: remove this once the zowie crash bug is verified fixed
    { 9, "test_zowie_crash", 0, {{ CMDINT(game_state.test_zowie_crash) }}, 0,
                        "Test the client zowie crash" },

    { 9, "detach_camera", CMD_DETACHED_CAMERA, {{ CMDINT(tmp_int) }}, 0,
                        "detach camera from player in regular game mode" },

    // TODO: Remove this once ATI memory leak regarding glEnable(GL_POLYGON_STIPPLE) in rt_shadow.c is fixed (11/06/09)
    { 0, "ati_stipple_leak",    0,  {{ CMDINT(game_state.ati_stipple_leak)}},CMDF_HIDEPRINT,
                        "enable call to glEnable(GL_POLYGON_STIPPLE) which causes memory leaks on ATI GPUs" },
    { 0, "ati_stencil_leak",    0,  {{ CMDINT(game_state.ati_stencil_leak)}},CMDF_HIDEPRINT,
                        "enable stencil FBO's which causes memory leaks on ATI GPUs" },

    { 9, "force_fallback_material", 0, {{ CMDINT(game_state.force_fallback_material) }}, 0,
                        "force the use of fallback materials" },

    // this is clientside catch to double check email inventory size
    { 0, "emailsend",        CMD_EMAILSEND, {{CMDSTR(tmp_str)},{CMDSTR(tmp_str2)},{CMDSENTENCE(tmp_str3)}}, CMDF_HIDEVARS,
                        "Send message <player names> <subject> <body> <influence>" },
    { 0, "emailsendattachment",        CMD_EMAILSENDATTACHMENT, {{CMDSTR(tmp_str)},{CMDSTR(tmp_str2)},{CMDINT(tmp_int)},{CMDINT(tmp_int2)},{CMDINT(tmp_int3)},{CMDSENTENCE(tmp_str3)}}, CMDF_HIDEVARS,
                        "Send message <player names> <subject> <body> <influence>" },

    // Turnstile server commands
    { 0, "lfg_request_event_list",        CMD_TURNSTILE_REQUEST_EVENT_LIST, {{ 0 }}, 0,
                        "Request LFG system event list" },
    { 9, "lfg_queue_for_events",        CMD_TURNSTILE_QUEUE_FOR_EVENTS, {{ CMDSENTENCE(tmp_str) }}, CMDF_HIDEVARS,
                        "Queue for events in LFG system" },
    { 0, "lfg_remove_from_queue",        CMD_TURNSTILE_REMOVE_FROM_QUEUE, {{ 0 }}, 0,
                        "Remove me or my team / league from LFG queue" },
    { 0, "lfg_event_response",            CMD_TURNSTILE_EVENT_RESPONSE, {{ CMDSTR(tmp_str) }}, 0,
                        "Accept / reject offer to join event" },
    
    { 0, "supportHardwareLights", 0, {{ CMDINT(game_state.supportHardwareLights) }}, 0,
                        "Enable support for LightFX/AlienFX case lights on startup" },
    { 9, "enableHardwareLights", 0, {{ CMDINT(game_state.enableHardwareLights) }}, 0,
                        "Enable LightFX/AlienFX case lights" },
    { 9, "emulateHardwareLights", 0, {{ CMDINT(game_state.emulateHardwareLights) }}, 0,
                        "Emulate LightFX/AlienFX case lights via an on-screen color swatch" },

    { 0, "priorityBoost", 0, {{ CMDINT(game_state.priorityBoost) }}, 0,
                        "Set the game process priority to Above Normal rather than Normal when running in the foreground." },
    { 9, "skipeula", 0, {{ CMDINT(game_state.skipEULANDA) }}, 0,
                        "Skip EULA/NDA in non-production modes" },
    { 9, "forceNDA", 0, {{ CMDINT(game_state.forceNDA) }}, 0,
                        "Force prompting of the NDA" },
    { 9, "clearNDASign", CMD_CLEAR_NDA_SIGNATURE, {{0}}, CMDF_HIDEPRINT, 
        "Clears NDA signature for Beta and Test"    },
    //*********
    // DEBUG/DEVELOPMENT ONLY
#ifndef FINAL
    { 9, "submeshIndex",  0, {{ CMDINT(g_client_debug_state.submeshIndex) }},0,
                        "view only this submesh index in a world model (use in conjunction with hideothers in editor to view a single mesh in a model)" },
    { 9, "modelFilter",  0, {{ CMDSTR(g_client_debug_state.modelFilter) }},0,
                        "Renders only models containing this string" },
    { 9, "trace_tex",  0, {{ CMDSTR(g_client_debug_state.trace_tex) }},0,
                        "Debug/Trace texture binds containing this string" },
    { 9, "trace_trick",  0, {{ CMDSTR(g_client_debug_state.trace_trick) }},0,
                        "Debug/Trace tricks containing this string" },
    { 9, "createAccessKey", CMD_CREATE_ACCESS_KEY, {{CMDSTR(tmp_str)},{CMDSTR(tmp_str2)}}, CMDF_HIDEPRINT, 
                        "Create an access key file (first arg is user name, second arg is access leve (1-10)"    },
    { 9, "testAccessKey", CMD_TEST_ACCESS_KEY, {{0}}, CMDF_HIDEPRINT, 
                        "Test access key file (will print out user and level if encrypted key access is enabled)"    },
    { 9, "pigg_logging",  0, {{ CMDINT(g_client_debug_state.pigg_logging) }},0,
                        "Enable pigg logging"    },
    //*********
#endif
    { 0, "redirect", CMD_REDIRECT, {{CMDINT(tmp_int)}}, CMDF_HIDEPRINT,
                        "Internal command so smf can trigger the redirect menu" },
    { 8, "web_store", CMD_WEB_STORE, {{0}}, CMDF_HIDEVARS,
                        "Open Web based store" },
    { 0, "web_store_open_product", CMD_WEB_STORE_OPEN_PRODUCT, {{CMDSTR(tmp_str)}}, CMDF_HIDEPRINT,
                        "Open Web based store to product page" },
    { 8, "lwcdebug",  CMD_LWCDEBUG_MODE, {{CMDINT(tmp_int)}},0,
                        "Set LWC debug mode"    },
    { 8, "lwcdebug_setdatastage",  CMD_LWCDEBUG_SET_DATA_STAGE, {{CMDINT(tmp_int)}},0,
                        "Set LWC data stage"    },
    { 8, "lwc_downloadrate",  CMD_LWCDOWNLOADRATE, {{CMDINT(tmp_int)}},0,
                        "Set LWC download rate (0 - normal, 1 - fastest, 2 - paused)"    },

    { 6, "buyproduct",  CMD_ACCOUNT_DEBUG_BUY_PRODUCT, {{CMDSTR(tmp_str)},{CMDINT(tmp_int)}},0,
                        "Buy the specified product sku and quantty from the store."    },
    { 7, "publish_product",  CMD_CATALOG_DEBUG_PUBLISH_PRODUCT, {{CMDSTR(tmp_str)},{CMDINT(tmp_int)}},0,
                        "Publish or unpublish the specified product sku (1=publish, 0=unpublish)."    },
    { 3, "query_account_inventory", CMD_ACCOUNT_QUERY_INVENTORY, {{CMDSTR(tmp_str)}},0,
                        "Query account inventory for a product SKU." },
    { 3, "query_account_inventory_active", CMD_ACCOUNT_QUERY_INVENTORY_ACTIVE, {{CMDSTR(tmp_str)}},0,
                        "Query the active portions of the account inventory for a product SKU." },
    { 3, "query_publish_product", CMD_QUERY_DEBUG_PUBLISH_PRODUCT, {{CMDSTR(tmp_str)}},0,
                        "Query publish status for a product SKU." },                        
    { 6, "account_loyalty_buy",  CMD_ACCOUNT_LOYALTY_BUY, {{CMDSTR(tmp_str)}},0,
                        "Buy the specified loyalty node."    },
    { 0, "my_purchases",    CMD_SHOW_PURCHASES, {{0}}, 0,
                        "Show the list of purchases you have access to" },                    
    { 9, "testCanStartStaticMap", CMD_TEST_CAN_START_STATIC_MAP, {{CMDSTR(tmp_str)}}, CMDF_HIDEPRINT, "Test canStartStaticMap" },
    { 9, "showoptions", CMD_SHOW_OPTIONS, {{0}}, CMDF_HIDEVARS, "Show the options menu" },
    { 9, "cutscene_letterbox",    0, { {CMDINT(game_state.forceCutSceneLetterbox) }}, 0 ,
                        "Force cut scene letterbox effect (0 off, 1 on)."  },

    { 0, "newAccountURL", 0, {{ CMDSTR(game_state.newAccountURL)}}, CMDF_HIDEPRINT,
                        "URL for new account creation website" },
    
    { 0, "linkAccountURL", 0, {{ CMDSTR(game_state.linkAccountURL)}}, CMDF_HIDEPRINT,
                        "URL for link account website" },

    { 0 },
};


CmdList game_cmdlist =
{
    {{ game_cmds },
     { client_control_cmds },
     { control_cmds },
    { 0 }}
};

void BugReport(const char * desc, int mode); // used by /bug, /cbug and /bugqa
void BugReportInternal(const char * msg, int userSubmitted);
static void jpeg_screenshot(char *title);


static access_override;

void cmdAccessOverride(int level)
{
    access_override = level;
}

char *encrypedKeyIssuedTo=NULL;
static U32 access_key[4] = {0x380CAE0A, 0xE5E0B642, 0xBC7B4E03, 0x6D88A9E7};
static const char *keyfile = "./dev_access.key";
static bool keyAccessFound = false;
static U32 nextKeyAccessCheck = 0;
#define KEY_DAYA_LEN 256
int encryptedKeyedAccessLevel()
{
    static int value=0;
    U32 currentTime = timerCpuSeconds();

    if (!keyAccessFound) {
        U32 currentTime = timerCpuSeconds();

        if (nextKeyAccessCheck <= currentTime) {
            if (fileExists(keyfile)) {
                int datalen;
                char *data = fileAlloc(keyfile, NULL);
                char *args[4];
                int numargs;
                BLOWFISH_CTX blowfish_ctx;
                cryptBlowfishInit(&blowfish_ctx, (U8*)access_key, sizeof(access_key));
                datalen = KEY_DAYA_LEN; //*((U32*)data);
                cryptBlowfishDecrypt(&blowfish_ctx,data, KEY_DAYA_LEN);
                numargs = tokenize_line_safe(data, args, ARRAY_SIZE(args), NULL);
                if (numargs==3 && stricmp(args[0], "PARAGON")==0)
                {
                    encrypedKeyIssuedTo = strdup(args[2]);
                    value = atoi(args[1]);
                    keyAccessFound = true;
                }
                fileFree(data);
            }

            nextKeyAccessCheck = currentTime + 15; // check again in 15 seconds for the dev_access.key file
        }

    }

    return value;
}

#ifndef FINAL
static bool createEncryptedKeyAccessLevel(const char* userName, int level)
{
    BLOWFISH_CTX blowfish_ctx;
    FILE * file;
    char fullText[KEY_DAYA_LEN];

    // if want more security, can enable this code to use current machine userName, and
    //    then would want to add corresponding code in encryptedKeyedAccessLevel to only
    //    allow key to be used if current userName is same as that in the file.
    //char userName[64];
    //DWORD size = ARRAY_SIZE(userName);
    //if( !GetUserName(userName, &size))
    //    return false;

    // only allow key creation in dev mode
    if( !isDevelopmentMode() )
    {
        conPrintf("Key creation not allowed in production build, use dev build to create key.");
        return false;
    }

    if(level <= 0 || level > 10)
    {
        conPrintf("Invalid access key level %d", level);
        return false;
    }

    memset(fullText, 0, ARRAY_SIZE(fullText));
    sprintf(fullText,"PARAGON %d %s", level, userName);
    cryptBlowfishInit(&blowfish_ctx, (U8*)access_key, sizeof(access_key));
    cryptBlowfishEncrypt(&blowfish_ctx, (unsigned char *)fullText, KEY_DAYA_LEN);

    // write to file
    file = fopen(keyfile,"wb");
    if (!file)
        return false;

    fwrite(fullText, KEY_DAYA_LEN, 1, file);
    fclose(file);
    conPrintf("Wrote file '%s' for user '%s', access level %d", keyfile, userName, level);
    keyAccessFound = false; // reset so can use new key right away
    nextKeyAccessCheck = 0;

    return true;
}
#endif

static void sgPasscodeDlgHandler(void * data)
{
	char *passcode;
	char buf[1000];
	U32 i;

	if (isMapSelectOpen()) {
		mapSelectClose();
		passcode = dialogGetTextEntry();
		// Stupid way to ensure only one word is sent, to avoid
		// the "Unknown command" error message.
		for (i = 0; i < strlen(passcode); i++) {
			if(passcode[i] == ' ') {
				passcode[i] = '\0';
				break;
			}
		}
		if (strlen(passcode) > 0) {
			sprintf(buf, "enter_base_from_passcode %s", passcode);
			cmdParse(buf);
		}
	}
}

int cmdAccessLevel()
{
    Entity    *p = ownedPlayerPtr();
    int encryptedLevel, playerLevel;

    // When playing demos, allow all sorts of performance commands, etc
    if (isDevelopmentMode())
    {
        if(demoIsPlaying())
            return ACCESS_DEBUG;
        if ((game_state.game_mode == SHOW_TEMPLATE || game_state.game_mode == SHOW_NONE) && !commConnected()) // To allow console commands in character creation under development
            return ACCESS_DEBUG;
    }

    // Check for access level key (overrides client-side accesslevel only)
    encryptedLevel = encryptedKeyedAccessLevel();
    playerLevel = p ? p->access_level : access_override; // access override used whenever no player

    return MAX(encryptedLevel, playerLevel);
}

static void cmdOldGameCheckCmds()
{
    int i, j, k, m;
    for (j=0; game_cmdlist.groups[j].cmds; j++) {
        Cmd *cmds = game_cmdlist.groups[j].cmds;
        for(i=0;cmds[i].name;i++)
        {
            bool b=false;
            Cmd *cmd = &cmds[i];
            if (cmd->flags & CMDF_HIDEVARS)
                continue;
            for (k=0; k < cmd->num_args && !b; k++)
            {
                DataDesc *dd = &cmd->data[k];
                for (m=0; m<ARRAY_SIZE(tmp_var_list) && !b; m++) {
                    if (dd->ptr == tmp_var_list[m]) {
/*                        Errorf("\
Found command \"%s\" using tmp_* but not marked as CMDF_HIDEVARS.\nThis means that if someone types the \
command with no parameters it will show the value of the tmp_* variable (which may have been the parameter \
to someone else's command if this is a server-side command), and will fail to display the handy usage \
instructions.", cmd->name); */
                        // Okay, so there are too many wrong to error, let's just set these smartlyish...
                        cmd->flags |= CMDF_HIDEVARS;
                        b=true;
                    }
                }
            }
        }
    }
}

void updateControlState(ControlId id, MovementInputSet set, int state, int time_stamp)
{
    ControlStateChange* csc;
    int i;
    int on = 0;

    // JS:    A hack to track when how many f,b,l,r movements there has been.
    //        This is used by the follow code to detect when some player movement has happened.
    //        Use an array of update counts if this ever needs to become more generic.
    if(set == MOVE_INPUT_CMD && id < CONTROLID_HORIZONTAL_MAX){
        control_state.movementControlUpdateCount++;
    }

    // Set the state for this control in this input set.

    state = state ? 1 : 0;

    if(((control_state.move_input[set] >> id) & 1) == state){
        return;
    }

    // Ignore player movement commands if in forced movement mode.
    if (set == MOVE_INPUT_CMD && control_state.followForced)
        return;

    if(state)
        control_state.move_input[set] |= (1 << id);
    else
        control_state.move_input[set] &= ~(1 << id);

    // Calculate the total state for all input sets.

    for(i = 0; i < MOVE_INPUT_COUNT; i++)
    {
        on |= (control_state.move_input[i] >> id) & 1;
    }

    // If I haven't sent a control packet yet,
    //   or the mapserver is lagging,
    //   or no state change,
    //   then ignore control change.

    if(    on &&
        !control_state.nocoll &&
        !control_state.alwaysmobile &&
        (    !control_state.first_packet_sent ||
            !control_state.mapserver_responding))
    {
        return;
    }

    // Create a CSC and store the control change information.

    csc = createControlStateChange();

    csc->control_id = id;
    csc->time_stamp_ms = time_stamp;
    csc->state = on;

    // Add the CSC to the process list.

    addControlStateChange(&control_state.csc_to_process_list, csc, 0);
}

//------------------------------------------------------------
// helper for passing or failing the execution of a power
// slot : the slot param, as passed to the command (i.e. the actual slot +1)
// -AB: created :2005 Jan 28 04:57 PM
//----------------------------------------------------------
static bool can_powexec_slot( CurTrayType tray, int slot )
{
    return !(gMacroOnstack &&
        (trayslot_istype(playerPtr(), slot-1, playerPtr()->pl->tray->current_trays[ tray ], kTrayItemType_Macro ) ||
        trayslot_istype(playerPtr(), slot-1, playerPtr()->pl->tray->current_trays[ tray ], kTrayItemType_MacroHideName )) );
}

//-------------------------------
// Converts the internal map name to something more human friendly
// placing result in supplied buffer.
//--------------------------------
char *clean_map_name(char *dest, int dest_size)
{
    char buffer[256];
    char *sp, *dp;

    if (dest_size <= 0)
        return dest;

    if(game_state.map_instance_id > 1)
        sprintf_s(SAFESTR(buffer), "%s_%d", game_state.world_name, game_state.map_instance_id);
    else
        Strcpy(buffer, game_state.world_name);
    msPrintf(menuMessages, dest, dest_size, buffer);

    sp = strrchr(dest, '/');
    if (sp) 
    {
        dp = dest;
        sp++;
        while(*sp)
        {
            *dp = *sp;
            dp++;
            sp++;
            if (*sp == '.') {
                *dp = 0;
                break;
            }
        }
    }
    return dest;
}

int cmdGameParse(char *str, int x, int y)
{
    int         original_state_first = control_state.first;
    Cmd            *cmd;
    CmdContext    output = {0};
    char        buffer1000[1001];

    output.found_cmd = false;
    output.access_level = cmdAccessLevel();

    cmd = cmdOldRead(&game_cmdlist,str,&output);

    // cmdRead above can modify the first person state.  If that happens,
    // we update the mapserver.
    if (original_state_first != control_state.first)
        sendThirdPerson();

    if (output.msg[0])
    {
        if (!cmd && !output.found_cmd && commConnected())
            commAddInput(str);
        else //if ( commConnected() )
            conPrintf("%s",output.msg);
        return 0;
    }
    if (!cmd)
        return 0;
    switch(cmd->num)
    {
        case CMD_QUIT:
        {
            Entity *e = playerPtr();
            if (e) // quit anyway if e is NULL...
                e->logout_login = 0;
            commSendQuitGame(0);
        }
        xcase CMD_RESET_SEPS:
            resetSeps();
        xcase CMD_QUIT_TO_LOGIN:
        {
            Entity *e = playerPtr();
            if (e) // quit anyway if e is NULL...
                e->logout_login = 1;
            commSendQuitGame(0);
        }
        xcase CMD_QUIT_TO_CHARACTER_SELECT:
        {
            Entity *e = playerPtr();
            if (e) // quit anyway if e is NULL...
                e->logout_login = 2;
            commSendQuitGame(0);
        }
        xcase CMD_EDIT:
            // don't use the edit state toggle for now.
            if (game_state.can_edit)
            {
                extern Vec3 view_pos;
                editSetMode(1, 0);
                hwcursorReloadDefault();
                copyMat3(cam_info.cammat,cam_info.mat);
                copyVec3(cam_info.cammat[3],cam_info.mat[3]);
            }
        xcase CMD_SCREENSIZE:
            windowSetSize(game_state.screen_x, game_state.screen_y);
        xcase CMD_STATESAVE:
            gameStateSave();
        xcase CMD_STATELOAD:
            gameStateLoad();
        xcase CMD_STATEINIT:
            gameStateInit();
        xcase CMD_VISSCALE:
            if (cmdAccessLevel()==ACCESS_USER) {
                if (game_state.vis_scale < 0.5) {
                    game_state.vis_scale = 0.5;
                } else if (game_state.vis_scale > WORLD_DETAIL_LIMIT) {
                    game_state.vis_scale = WORLD_DETAIL_LIMIT;
                }
            }

            setVisScale(game_state.vis_scale, 1);

        xcase CMD_SAVETEX:
            {
                BasicTexture *bind = texLoadBasic(tmp_str, TEX_LOAD_NOW_CALLED_FROM_MAIN_THREAD, TEX_FOR_UTIL);
                if (bind == white_tex) {
                    conPrintf("Failed to find texture %s", tmp_str);
                } else {
                    gfxTextureDump(texDemandLoadActual(bind), tmp_str);
                }
            }
        xcase CMD_GFXDEBUG:
            if (game_state.gfxdebug && rdr_caps.use_vbos)
            {
                // Disable VBOs
                modelResetVBOs(true);
                rdrQueueFlush();
                rdr_caps.use_vbos = 0;
            }
        xcase CMD_NORMALMAP_TEST:
            setNormalMapTestMode(tmp_str);
        xcase CMD_RENDERSCALE:
            game_state.renderScaleX = game_state.renderScaleY = tmp_f32;
            if (tmp_f32!=1)
                game_state.useRenderScale = RENDERSCALE_SCALE;
        xcase CMD_RENDERSIZE:
            game_state.renderScaleX = tmp_int;
            game_state.renderScaleY = tmp_int2;
            game_state.useRenderScale = RENDERSCALE_FIXED;
        xcase CMD_ENABLEVBOS:
            if (rdr_caps.filled_in) // It's after graphics are initialized
            {
                // Change this on the fly
                if (rdr_caps.use_vbos != game_state.enableVBOs && !game_state.noVBOs)
                {
                    modelResetVBOs(true);
                    rdrQueueFlush();
                    rdr_caps.use_vbos = game_state.enableVBOs && !game_state.noVBOs;
                }
            }
        xcase CMD_NOVBOS:
            if (rdr_caps.filled_in) // It's after graphics are initialized
            {
                // Change this on the fly
                if (rdr_caps.use_vbos != !game_state.noVBOs)
                {
                    modelResetVBOs(true);
                    rdrQueueFlush();
                    rdr_caps.use_vbos = !game_state.noVBOs;
                }
            }
        xcase CMD_MAX_TEXUNITS:
            if (rdr_caps.filled_in) // It's after graphics are initialized
            {
                if (!(rdr_caps.chip & ARBFP)) {
                    // For people hitting the oldvid key on a card not supporting ARBfp
                    game_state.maxTexUnits = 4;
                }
                // Change this on the fly
                if (rdr_caps.num_texunits != game_state.maxTexUnits)
                {
                    static GfxFeatures saved_features=0;
                    rdrQueueFlush();
                    rdrSetNumberOfTexUnits( game_state.maxTexUnits );
                    if (rdr_caps.num_texunits <= 4) {
                        saved_features = rdr_caps.features;
                        // Disable all features but bumpmaps
                        rdrToggleFeatures(0, ~0 & ~(GFXF_BUMPMAPS|GFXF_BUMPMAPS_WORLD), false);
                        //rdrToggleFeatures(0, ~0, false);
                    } else {
                        rdrToggleFeatures(saved_features, 0, false);
                    }
                }
            }
        xcase CMD_SHADER_DETAIL:
            {
                int gfx_features_enable=0,gfx_features_disable=0;
                shaderDetailToFeatures(tmp_int, &gfx_features_enable, &gfx_features_disable);
                rdrToggleFeatures(gfx_features_enable, gfx_features_disable, false);
            }
        xcase CMD_OLD_VIDEO_TEST:
            if (game_state.oldVideoTest)
                cmdParse("maxTexUnits 4");
            else
                cmdParse("maxTexUnits 16");
        xcase CMD_GFX_DEV_HUD:
            gfxDevHudToggle();
        xcase CMD_SHADER_TEST:
            if (rdr_caps.filled_in) {
                reloadShaderCallback(NULL, 1);
            }
        xcase CMD_SHADER_TEST_SPEC_ONLY:
        case  CMD_SHADER_TEST_DIFFUSE_ONLY:
        case  CMD_SHADER_TEST_NORMALS_ONLY:
        case  CMD_SHADER_TEST_TANGENTS_ONLY:
            tmp_int = (tmp_int ? (SHADERTEST_SPEC_ONLY + (cmd->num - CMD_SHADER_TEST_SPEC_ONLY)) : 0);
            if( tmp_int != game_state.shaderTestMode1 ) {
                game_state.shaderTestMode1 = tmp_int;
                reloadShaderCallback(NULL, 1);
            }

        xcase CMD_SHADER_TEST_BASEMAT_ONLY:
        case  CMD_SHADER_TEST_BLENDMAT_ONLY:
            tmp_int = (tmp_int ? (SHADERTEST_BASEMAT_ONLY + (cmd->num - CMD_SHADER_TEST_BASEMAT_ONLY)) : 0);
            if( tmp_int != game_state.shaderTestMode2 ) {
                game_state.shaderTestMode2 = tmp_int;            
                reloadShaderCallback(NULL, 1);
            }
        xcase CMD_HEADSHOT_CACHE_CLEAR:
            seqClearHeadshotCache();

        xcase CMD_DEMO_CONFIG:
            // force settings for the demo du jour
            cmdParse("setpospyr -5504.30 -16.00 -1926.04 0.1632 0.0070 0.0000");
            cmdParse("camreset");
            cmdParse("camdist 30");
            cmdParse("usedof 0");    // turn off dof
            cmdParse("usehdr 0");    // turn off bloom and desaturate
            cmdParse("timeset 16"); cmdParse("timescale 0"); cmdParse("timeset 16"); // force fixed demo time
            cmdParse("separate_reflection_viewport 1"); // two separate viewports for planar reflection (one for water, one for bldgs)
            game_state.reflectionEnable = 3; // enable both water and planar building reflections
            cmdParse("forcecubeface -1");    // render every face each frame (slow, but demo machine should handle it no problem)
            // adjust shadowmap cascades
            game_state.shadowFarMax = 1000.0f;
            game_state.shadowSplitLambda = 0.85f;
            // turn on character only planar reflectors
            game_state.reflection_planar_mode = kReflectEntitiesOnly;

        xcase CMD_TOGGLE_USECG:
            if (game_state.useCg != tmp_int)
            {
                game_state.useCg = tmp_int;
                setCgShaderMode(game_state.useCg);
            }
        xcase CMD_TOGGLE_DXT5NM:
            setUseDXT5nmNormals(tmp_int);
        xcase CMD_SET_CG_SHADER_PATH:
            setCgShaderPathCallback( tmp_str );
            if (rdr_caps.filled_in)
                reloadShaderCallback(NULL, 1);        
        xcase CMD_SHOW_DOF:
            if (tmp_int_special[3])
                cmdParse("shadertest 9");
            else
                cmdParse("shadertest 0");
        xcase CMD_DOF_TEST:
            setCustomDepthOfField(tmp_int, tmp_f32, tmp_f325, tmp_f322, 1.0, tmp_f323, 1.0, tmp_f324, &g_sun);
        xcase CMD_GFX_TEST:
            printf("The gfxtest command is deprecated.\n");
        xcase CMD_USEHDR:
            if (tmp_int_special[1]==1) {
                rdrToggleFeatures(GFXF_BLOOM|GFXF_TONEMAP|GFXF_DESATURATE, 0, false);
            } else if (tmp_int_special[1] == 2) {
                rdrToggleFeatures(GFXF_BLOOM, GFXF_TONEMAP, false);
            } else if (tmp_int_special[1] == 3) {
                rdrToggleFeatures(GFXF_TONEMAP, GFXF_BLOOM, false);
            } else {
                rdrToggleFeatures(0, GFXF_BLOOM|GFXF_TONEMAP|GFXF_DESATURATE, false);
            }
            if (tmp_int_special[1] == BLOOM_REGULAR)
                game_state.bloomScale = 2;
            else if (tmp_int_special[1] == BLOOM_HEAVY)
                game_state.bloomScale = 4;
        xcase CMD_USEDOF:
            if (tmp_int_special[2]) {
                rdrToggleFeatures(GFXF_DOF, 0, false);
            } else {
                rdrToggleFeatures(0, GFXF_DOF, false);
            }
        xcase CMD_USEFP:
            if (tmp_int)
                rdrToggleFeatures(GFXF_FPRENDER, 0, false);
            else
                rdrToggleFeatures(0, GFXF_FPRENDER, false);
        xcase CMD_USEFANCYWATER:
            game_state.waterMode = tmp_int;
        xcase CMD_USEBUMPMAPS:
            if (tmp_int_special[6]) {
                rdrToggleFeatures(GFXF_BUMPMAPS, 0, false);
            } else {
                rdrToggleFeatures(0, GFXF_BUMPMAPS, false);
            }
        xcase CMD_USEHQ:
            if (tmp_int_special[4]) {
                rdrToggleFeatures(GFXF_HQBUMP|GFXF_MULTITEX_HQBUMP, 0, false);
            } else {
                rdrToggleFeatures(0, GFXF_HQBUMP|GFXF_MULTITEX_HQBUMP, false);
            }
        xcase CMD_USECUBEMAP:
            if( rdr_caps.allowed_features & GFXF_CUBEMAP )
            {
                switch( tmp_int_special[5] )
                {
                    case 1: game_state.cubemapMode = CUBEMAP_LOWQUALITY; break;
                    case 2: game_state.cubemapMode = CUBEMAP_HIGHQUALITY; break;
                    case 3: game_state.cubemapMode = CUBEMAP_DEBUG_SUPERHQ; break;
                    default:
                    case 0: game_state.cubemapMode = CUBEMAP_OFF; break;
                }
            }
        xcase CMD_SHADER_PERF_TEST:
            gfxShaderPerfTest();
        xcase CMD_UNLOAD_GFX:
            gfxResetTextures();
        xcase CMD_REBUILD_MINITRACKERS:
            groupDrawBuildMiniTrackers();
        xcase CMD_REDUCEMIP:
            game_state.actualMipLevel        = game_state.mipLevel + 1;
            if (game_state.actualMipLevel < 0)
                game_state.actualMipLevel = 0;
            game_state.entityMipLevel        = game_state.mipLevel;
            if (rdr_caps.filled_in) // past startup
                gfxResetTextures();
        xcase CMD_TEXANISO:
            texResetAnisotropic();
        xcase CMD_ANTIALIASING:
            game_state.antialiasing = MAX(game_state.antialiasing, 1);
        xcase CMD_UNLOAD_MODELS:
            modelFreeAllCache(0);
        xcase CMD_RELOAD_TRICKS:
            trickReloadPostProcess();
        xcase CMD_TIMESTEPSCALE:
            if (commConnected()) {
                commAddInput(str);
            } else {
                server_visible_state.timestepscale = tmp_f32;
            }
        xcase CMD_TIMEDEBUG:
            conPrintf("localtime:");
            cmdParse("localtime");
            conPrintf("timing debug offset is %i", timing_debug_offset);
            conPrintf("calculated server offset is %i", timing_s2000_delta);
            conPrintf("client believes the server time is %s", timerMakeOffsetDateStringNoYearNoSecFromSecondsSince2000(timerServerTime()));
            conPrintf("client believes the different in time zones with server is %i", timerServerTZAdjust());
            conPrintf("servertime:");
            cmdParse("servertime");
        xcase CMD_TEXWORDSLOAD:
            texWordsReloadText(tmp_str);
            texWordsLoad(tmp_str);
        xcase CMD_TEXWORDEDITOR:
            estrPrintCharString(&game_state.texWordEdit, tmp_str);
            if (editMode())
            {
                texWordsEditor(1);
            }
        xcase CMD_TEXWORDFLUSH:
            texWordsFlush();
        xcase CMD_CMDS:
            cmdOldPrintList(game_cmds,0,cmdAccessLevel(),tmp_str,1,0);
            cmdOldPrintList(control_cmds,0,cmdAccessLevel(),tmp_str,0,0);
            cmdOldPrintList(client_control_cmds,0,cmdAccessLevel(),tmp_str,0,0);
        xcase CMD_MAKE_MESSAGESTORE:
             cmdOldSaveMessageStore( &game_cmdlist, "cmdMessagesClient.txt" );
            conPrintf( "Client Commands saved." );
        xcase CMD_CMDLIST:
            cmdOldPrintList(game_cmds,0,cmdAccessLevel(),NULL,0,0);
            cmdOldPrintList(control_cmds,0,cmdAccessLevel(),NULL,0,0);
            cmdOldPrintList(client_control_cmds,0,cmdAccessLevel(),NULL,0,0);
            cmdParse("scmdlist");
        xcase CMD_CMDUSAGE:
            cmdOldPrintList(game_cmds,0,cmdAccessLevel(),tmp_str,1,1);
            cmdOldPrintList(control_cmds,0,cmdAccessLevel(),tmp_str,0,1);
            cmdOldPrintList(client_control_cmds,0,cmdAccessLevel(),tmp_str,0,1);
        xcase CMD_VARSET:
            cmdOldVarsAdd(tmp_int, tmp_str);
        xcase CMD_BIND:
            bindKey(bind_key_name,bind_command,1);
        xcase CMD_SHOW_BIND:
            displayKeybind(bind_key_name);
        xcase CMD_UNBIND:
            bindKey(bind_key_name,NULL,1);
        xcase CMD_BIND_LOAD:
            bindListLoad(NULL,0);
        xcase CMD_BIND_LOAD_FILE:
            bindListLoad(tmp_str,0);
        xcase CMD_BIND_LOAD_FILE_SILENT:
            bindListLoad(tmp_str,1);
        xcase CMD_BIND_SAVE:
            bindListSave(NULL,0);
        xcase CMD_BIND_SAVE_FILE:
            bindListSave(tmp_str,0);
        xcase CMD_BIND_SAVE_FILE_SILENT:
            bindListSave(tmp_str,1);
        xcase CMD_WDW_LOAD:
            wdwLoad(NULL);
        xcase CMD_WDW_LOAD_FILE:
            wdwLoad(tmp_str);
        xcase CMD_WDW_SAVE:
            wdwSave(NULL);
        xcase CMD_WDW_SAVE_FILE:
            wdwSave(tmp_str);
        xcase CMD_OPTION_LOAD:
            optionLoad(NULL);
        xcase CMD_OPTION_LOAD_FILE:
            optionLoad(tmp_str);
        xcase CMD_OPTION_SAVE:
            optionSave(NULL);
        xcase CMD_OPTION_SAVE_FILE:
            optionSave(tmp_str);
        xcase CMD_CHAT_LOAD:
            loadChatSettings(NULL);
        xcase CMD_CHAT_LOAD_FILE:
            loadChatSettings(tmp_str);
        xcase CMD_CHAT_SAVE:
            saveChatSettings(NULL);
        xcase CMD_CHAT_SAVE_FILE:
            saveChatSettings(tmp_str);
        xcase CMD_TEXUSAGE:
            texShowUsage(tmp_str);
        xcase CMD_TEXRES:
            texShowUsageRes(tmp_int, tmp_int2);
        xcase CMD_CLEARINPUT:
            inpClear();
        xcase CMD_FIRE1:
            fxTestFxInTfxTxt();
        xcase CMD_IMAGESERVER:
            game_state.imageServer = 1;
            g_audio_state.noaudio = 1;
            //setAssertMode(ASSERTMODE_EXIT); // let's assert so we can debug this, shall we?
        xcase CMD_RESETPARTHIGH:
            fxClearPartHigh();
            game_state.resetparthigh = 1;
        xcase CMD_NEXTFX:
            fxSelectNextFxForDetailedInfo();
        xcase CMD_FXTIMERS:
            fxSetIndividualAutoTimersOn(tmp_int);
        xcase CMD_BONESCALE:
            {
            }//changeBoneScale( playerPtr()->seq, game_state.bonescale );
        xcase CMD_RELOADSEQUENCERS:
            {
                //send it to the server, too to synch with the client
                cmdParse( "reloadSeqsSvr 1" );
                game_state.reloadSequencers = 1;
            }//changeBoneScale( playerPtr()->seq, game_state.bonescale );
        xcase CMD_SHOULDERSCALE:
            {
                Entity * e = playerPtr();
                if(e && e->seq)
                {
                    e->seq->shoulderScale += tmp_f32;
                    e->seq->shoulderScale = MINMAX(e->seq->shoulderScale, -1, 1);

                    if(ABS(e->seq->shoulderScale) <= 0.025)
                        e->seq->shoulderScale = 0;
                }
            }
        xcase CMD_CMALLOC:
            memCheckDumpAllocs();
        xcase CMD_CONNECT:
        {
            commDisconnect();
            commConnect(game_state.address,game_state.port,0);
        }
        xcase CMD_CAMRESET:
        {
            zeroVec3( control_state.cam_pyr_offset );
            game_state.camdist = 10.0;
        }
        xcase CMD_CUBEMAPS:
        {
            game_state.cubemap_flags |= (kGenerateStatic|kSaveFacesOnGen);
        }
        xcase CMD_SETDEBUGENTITY:
        {
            //Set the selected entity as the debug entity on the client, then tell the server to do the same
            game_state.store_entity_id = tmp_int;
            sprintf_s(SAFESTR(buffer1000), "setdebugentitysvr %d", tmp_int );
            cmdParse( buffer1000 );

        }
        xcase CMD_SCRIPTDEBUG_SET:
        {
            ScriptDebugSet(tmp_int);
        }
        xcase CMD_SCRIPTDEBUG_VARTOP:
        {
            ScriptDebugSetVarTop(tmp_int, tmp_int2);
        }
        xcase CMD_SHOWSTATE:
        {
            if( stricmp( game_state.entity_to_show_state, "me" ) == 0 )
            {
                char buf[1000];
                //Set the selected entity as the debug entity on the client, then tell the server to do the same
                game_state.store_entity_id = playerPtr()->svr_idx;
                sprintf_s( SAFESTR(buf), "setdebugentitysvr %d", playerPtr()->svr_idx );
                cmdParse( buf );
                Strncpyt( game_state.entity_to_show_state, "selected" );
            }

            game_state.show_entity_state = 1;
            sprintf_s( SAFESTR(buffer1000), "showstatesvr %s", game_state.entity_to_show_state );
            cmdParse( buffer1000 );
        }
        #if NOVODEX
            xcase CMD_NX_THREADED:
            {
                nx_state.threaded = tmp_int ? 1 : 0;
            }
            xcase CMD_NX_CLEAR_CACHE:
            {
                destroyConvexShapeCache();
            }
            xcase CMD_NX_HARDWARE:
            {
                nx_state.softwareOnly = !tmp_int;
                // Only reinit novodex if something changed
                if (
                    ( nx_state.softwareOnly && nx_state.hardware )
                    || ( !nx_state.softwareOnly && !nx_state.hardware )
                    )
                {
                    nwDeinitializeNovodex();
                    nx_state.gameInPhysicsPossibleState = 1; // turn this on despite risks
                    nwInitializeNovodex();
                }
            }
            xcase CMD_NX_INIT_WORLD:
            {
                if ( !nx_state.initialized )
                {
                    nx_state.gameInPhysicsPossibleState = 1; // turn this on despite risks
                    nwInitializeNovodex();
                }
            }
            xcase CMD_NX_DEINIT_WORLD:
            {
                if ( nx_state.initialized )
                {
                    nwDeinitializeNovodex();
                }
            }
            xcase CMD_NX_INFO:
            {
                nwPrintInfoToConsole(1);
            }
            xcase CMD_NX_COREDUMP:
            {
                nwCoreDump();
            }
            xcase CMD_NX_PHYSX_CONNECT:
            {
                nwConnectToPhysXDebug();
            }
        #endif
        xcase CMD_NOFILECHANGECHECK:
        {
            sprintf_s(SAFESTR(buffer1000), "nofilechangecheck_server %d", global_state.no_file_change_check_cmd_set);
            cmdParse(buffer1000);
            global_state.no_file_change_check = global_state.no_file_change_check_cmd_set;
        }
        xcase CMD_CLEAR_TT_CACHE:
        {
            ttFMClearCache(fontManager);
        }
//#define USE_VTUNE //JE: Do *not* check code in with this enabled, it requires VTuneAPI.dll, which we do not (can not?) include with our patches
#ifdef USE_VTUNE
        xcase CMD_VTUNE:
        {
            extern void VTResume();
            extern void VTPause();
            if(game_state.vtuneCollectInfo)
                VTResume();
            else
                VTPause();
        }
#endif //USE_VTUNE

        xcase CMD_PLAYSAMPLE:
            // random owner channel so as to never interrupt the previous playsound call
            sndPlay(game_state.song_name, SOUND_FX_BASE + randInt(SOUND_FX_LAST - SOUND_FX_BASE));
        xcase CMD_PLAYMUSIC:
        {
            int flags = SOUND_PLAY_FLAGS_INTERRUPT;
            if(tmp_int) flags |= SOUND_PLAY_FLAGS_SUSTAIN;
            sndPlayEx(game_state.song_name, SOUND_MUSIC, tmp_f32, flags);
        }
        xcase CMD_STOPMUSIC:
            sndStop(SOUND_MUSIC, tmp_f32);
        xcase CMD_PLAYVOICEOVER:
            sndPlayEx(game_state.song_name, SOUND_VOICEOVER, 1.f, SOUND_PLAY_FLAGS_INTERRUPT);
        xcase CMD_VOLUME:
        {
            int volType = SOUND_VOLUME_MUSIC;
            if(tmp_str[0] == 'f' || tmp_str[0] == 'F')
                volType = SOUND_VOLUME_FX;
            else if(tmp_str[0] == 'v' || tmp_str[0] == 'V')
                volType = SOUND_VOLUME_VOICEOVER;
            sndVolumeControl(volType, tmp_f32);
        }
        xcase CMD_MUTE:
        case CMD_UNMUTE:
            if( !sndMute(tmp_str, (cmd->num==CMD_MUTE)) ) {
                conPrintf("Invalid sound type \"%s\", options are none/fx/music/sphere/script/vo/other/all\n", tmp_str);
            }
        xcase CMD_RESET_SOUNDS:
            sndFreeAll();
        xcase CMD_SOUND_DISABLE:
            if(tmp_int) {
                sndExit();
                g_audio_state.noaudio = 1;
            } else {
                g_audio_state.noaudio = 0;
                sndExit();
                sndInit();
            }
#if AUDIO_DEBUG
        xcase CMD_REPEATSOUND:
            // play the given sound and repeat at the given interval (for testing max instance functionality)
            sndTestRepeat(game_state.song_name, tmp_f32, tmp_int);
        xcase CMD_PLAYALLSOUNDS:
        {
            // play all sounds starting at the given index
            extern int gPAS_CurSoundIndex;
            gPAS_CurSoundIndex = tmp_int;
            memtrack_mark(0, gPAS_CurSoundIndex == -1 ? "PlayAllSounds_STOP" : "PlayAllSounds_START");
        }
        xcase CMD_DUCKVOLUME:
        {
            int volType = SOUND_VOLUME_MUSIC;
            if(tmp_str[0] == 'f' || tmp_str[0] == 'F')
                volType = SOUND_VOLUME_FX;
            else if(tmp_str[0] == 'v' || tmp_str[0] == 'V')
                volType = SOUND_VOLUME_VOICEOVER;
            sndSetDuckingParams(volType, tmp_f32, tmp_f322, tmp_f323);
        }
#endif // AUDIO_DEBUG
        xcase CMD_FOLLOW:
            playerToggleFollowMode();
        xcase CMD_LOCALE:
        {
            // If the new locale is not valid, then don't do anything.
            if(!locIDIsValidForPlayers(tmp_int)){
                addSystemChatMsg(textStd("BadLocale"), INFO_USER_ERROR, 0);
                break;
            }

            setCurrentLocale(tmp_int);
            locSetIDInRegistry(getCurrentLocale());
            reloadClientMessageStores(getCurrentLocale());

            // Tell the server that the player wishes to switch locale.
            sprintf_s(SAFESTR(tmp_str), "playerlocale %i", getCurrentLocale());
            cmdParse(tmp_str);
        }
        xcase CMD_EXEC:
        {
            char* fileName;
            FILE* f;

            fileName = fileLocateRead_s(tmp_str, NULL, 0);

            if(!fileName)
            {
                sprintf_s(SAFESTR(buffer1000), "%s.txt", tmp_str);

                fileName = fileLocateRead_s(buffer1000, NULL, 0);
            }

            f = fileOpen(fileName, "rt");
            fileLocateFree(fileName);

            if(!f)
            {
                sprintf_s(SAFESTR(buffer1000), "%s.txt", tmp_str);

                f = fileOpen(buffer1000, "rt");

                if(!f)
                {
                    conPrintf("Can't exec command file: %s", tmp_str);
                }
            }

            if(f)
            {
                while(fgets(buffer1000, ARRAY_SIZE(buffer1000) - 1, f))
                {
                    int len = strlen(buffer1000);

                    if(buffer1000[len-1] == '\n')
                        buffer1000[len-1] = '\0';

                    if(buffer1000[0] != '#')
                    {
                        cmdParse(buffer1000);
                    }
                }

                fileClose(f);
            }
        }
        xcase CMD_RELOAD_TEXT:
            reloadClientMessageStores(getCurrentLocale());
        xcase CMD_CONSOLE:
        {
            newConsoleWindow();
            if (!IsUsingCider())
            {
                ShowWindow(compatibleGetConsoleWindow(), tmp_int?SW_SHOW:SW_HIDE);
                sprintf_s(SAFESTR(buffer1000), "COH Console: %s", getExecutableName());
                setConsoleTitle(buffer1000);
            }
        }
        xcase CMD_NODEBUG:
        {
            game_state.info                = 0;
            game_state.fxdebug            = 0;
            game_state.showfps            = 0;
            game_state.showActiveVolume    = 0;
            game_state.gfxdebug            = 0;
            game_state.nodebug            = 1;
        }
        xcase CMD_NOMINIDUMP:
            if (tmp_int)
                setAssertMode(tmp_int? getAssertMode() & ~ASSERTMODE_MINIDUMP : getAssertMode() | ASSERTMODE_MINIDUMP);
        xcase CMD_NOERRORREPORT:
            if (tmp_int)
                setAssertMode(tmp_int? getAssertMode() & ~ASSERTMODE_ERRORREPORT : getAssertMode() | ASSERTMODE_ERRORREPORT);
        xcase CMD_PACKETDEBUG:
            if (isDevelopmentOrQAMode())
                pktSetDebugInfo();
        xcase CMD_PACKETLOGGING:
            commLogPackets(tmp_int);
        xcase CMD_TARGET_ENEMY_NEXT:
            selectTarget( 1, 0, 1, 1, 0, 0, 0, 0 );
        xcase CMD_TARGET_ENEMY_PREV:
            selectTarget( 1, 1, 1, 1, 0, 0, 0, 0 );
        xcase CMD_TARGET_ENEMY_NEAR:
            selectTarget( 0, 0, 1, 1, 0, 0, 0, 0 );
        xcase CMD_TARGET_ENEMY_FAR:
            selectTarget( 0, 1, 1, 1, 0, 0, 0, 0 );
        xcase CMD_TARGET_FRIEND_NEXT:
            selectTarget( 1, 0, -1, 1, 0, 0, 0, 0 );
        xcase CMD_TARGET_FRIEND_PREV:
            selectTarget( 1, 1, -1, 1, 0, 0, 0, 0 );
        xcase CMD_TARGET_FRIEND_NEAR:
            selectTarget( 0, 0, -1, 1, 0, 0, 0, 0 );
        xcase CMD_TARGET_FRIEND_FAR:
            selectTarget( 0, 1, -1, 1, 0, 0, 0, 0 );
        xcase CMD_TARGET_CUSTOM_NEXT:
            customTarget( 1, 0, tmp_str );
        xcase CMD_TARGET_CUSTOM_PREV:
            customTarget( 1, 1, tmp_str );
        xcase CMD_TARGET_CUSTOM_NEAR:
            customTarget( 0, 0, tmp_str );
        xcase CMD_TARGET_CUSTOM_FAR:
            customTarget( 0, 1, tmp_str );
        xcase CMD_TARGET_CONFUSION:
        {
            CLAMP(tmp_f32,0.0,1.0);
            gfTargetConfusion = tmp_f32;
        }
        xcase CMD_MOUSE_LOOK:
        {
             control_state.canlook = tmp_int;
        }
        xcase CMD_INFO:
        {
            info_Entity(current_target ? current_target : playerPtr(), 0);
        }
        xcase CMD_INFO_TAB:
        {
            info_Entity(current_target ? current_target : playerPtr(), tmp_int-1);
        }
        xcase CMD_INFO_SELF:
        {
            info_Entity(playerPtr(),0);
        }
        xcase CMD_INFO_SELF_TAB:
        {
            info_Entity(playerPtr(),tmp_int-1);
        }
        xcase CMD_START_CHAT:
        {
            uiChatStartChatting(NULL, true);
        }
        xcase CMD_START_CHAT_PREFIX:
        {
            uiChatStartChatting(tmp_str, true);
        }
        xcase CMD_CONSOLE_CHAT:
        {
            uiChatStartChatting("/", false);
        }
        xcase CMD_UNSELECT:
        {
            if(!current_target){
                game_state.camera_follow_entity = 0;
            }

            current_target = 0;
            gSelectedDBID = 0;
            gSelectedHandle[0] = 0;
        }
        xcase CMD_LOADMISSIONMAP:
            // FALL THROUGH
        case CMD_LOADMAP:
        {
            if (isDevelopmentMode()) {
                if (cmd->num == CMD_LOADMISSIONMAP)
                {
                    cmdParse("onmissionmap 1");
                }
                if(stricmp(tmp_str, game_state.world_name))
                {
                    editLoad(tmp_str, 0);
                }
                if (cmd->num == CMD_LOADMISSIONMAP)
                {
                    cmdParse("initmap");
                    cmdParse("gotospawn MissionStart");
                }
            } else {
                addSystemChatMsg("LoadMapsInDevOnly", INFO_USER_ERROR, 0);
            }
        }
        xcase CMD_COPYDEBUGPOS:
        {
            char* mapname = game_state.world_name;

            mapname = strstri(game_state.world_name, "maps");

            sprintf_s(SAFESTR(buffer1000),
                    "loadmap %s\nsetpospyr %1.2f %1.2f %1.2f %1.4f %1.4f %1.4f\ncamdist %1.4f\nthird %d\n",
                    mapname ? mapname : game_state.world_name,
                    posParamsXYZ(&cam_info),
                    vecParamsXYZ(control_state.pyr.cur),
                    game_state.camdist,
                    !control_state.first);

            winCopyToClipboard(buffer1000);
        }
        xcase CMD_PROFILER_RECORD:
        {
            fxSetIndividualAutoTimersOn(1);
            timing_state.autoTimerDisablePopupErrors = 1;
            timing_state.autoStackMaxDepth_init = 19;
            SAFE_FREE(timing_state.autoTimerRecordFileName);
            timing_state.autoTimerRecordFileName = strdup(tmp_str);
        }
        xcase CMD_PROFILER_STOP:
        {
            timing_state.autoTimerProfilerCloseFile = 1;
            timing_state.runperfinfo_init = 0;
            timing_state.resetperfinfo_init = 1;
        }
        xcase CMD_PROFILER_PLAY:
        {
            if(playerPtr() && playerPtr()->access_level)
            {
                timing_state.autoTimerDisablePopupErrors = 0;
                timing_state.autoTimerPlaybackStepSize = 1;
                timing_state.autoTimerPlaybackState = 1;
                SAFE_FREE(timing_state.autoTimerPlaybackFileName);
                timing_state.autoTimerPlaybackFileName = strdup(tmp_str);
            }
        }
        xcase CMD_RELOADENTDEBUGFILEMENUS:
        {
            entDebugLoadFileMenus(1);
        }
        xcase CMD_CAMERAFOLLOWENTITY:
        {
            Entity* cur_cam_ent = erGetEnt(game_state.camera_follow_entity);
            if(!current_target || current_target == cur_cam_ent)
            {
                game_state.camera_follow_entity = 0;
            }
            else if(current_target)
            {
                game_state.camera_follow_entity = erGetRef(current_target);
            }
        }
        xcase CMD_CLEARDEBUGLINES:
        {
            entDebugClearLines();
        }
        xcase CMD_TOGGLEMMSTILL:
        {
            picturebrowser_setAnimation(tmp_int);
        }
        xcase CMD_CLEARMMTCACHE:
        {
            seqClearMMCache();
        }
        xcase CMD_MAPIMAGECREATE:
        {
            groupLoadMakeAllMapImages(tmp_str);
        }
        xcase CMD_MAPALLSHOW:
        {
            automap_showAllMaps(tmp_int);
        }
        xcase CMD_PASTE_DEBUGGER_POS_0:
        {
            paste_debug_pos_state = 1;
            paste_debug_pos[0] = tmp_f32;
        }
        xcase CMD_PASTE_DEBUGGER_POS_1:
        {
            if(paste_debug_pos_state == 1){
                paste_debug_pos_state = 2;
                paste_debug_pos[1] = tmp_f32;
            }
        }
        xcase CMD_PASTE_DEBUGGER_POS_2:
        {
            if(paste_debug_pos_state == 2){
                Entity* controlledPlayer = controlledPlayerPtr();

                paste_debug_pos_state = 0;
                paste_debug_pos[2] = tmp_f32;

                sprintf_s(SAFESTR(buffer1000),
                        "setpos %f %f %f",
                        paste_debug_pos[0],
                        paste_debug_pos[1],
                        paste_debug_pos[2]);

                cmdParse(buffer1000);

                if(controlledPlayer){
                    entUpdatePosInstantaneous(controlledPlayer, paste_debug_pos);
                }
            }
        }
#if MEMTRACK_BUILD
        xcase CMD_MEMTRACK_CAPTURE:
            memtrack_capture_enable(tmp_int);
        xcase CMD_MEMTRACK_MARK:
            memtrack_mark(0, tmp_str);
#else
        xcase CMD_MEMTRACK_CAPTURE:
        case CMD_MEMTRACK_MARK:
            conPrintf("This client was not built with memtrack support\n");
#endif
        xcase CMD_MEM_CHECK:
            if (heapValidateAll())
                conPrintf("Memory ok\n");
            else
                conPrintf("Memory error\n");
        xcase CMD_MEM_CHECKPOINT:
            _CrtMemCheckpoint(&g_memstate);
        xcase CMD_MEM_DUMP:
            _CrtMemDumpAllObjectsSince(&g_memstate);
        xcase CMD_VERBOSE_PRINTF:
            errorSetVerboseLevel(tmp_int);
        xcase CMD_NOWINIO:
            fileDisableWinIO(tmp_int);
        xcase CMD_CAMDIST:
            {
                if( tmp_f32 < 0 )
                    tmp_f32 = 0;
                if( baseedit_Mode() )
                {
                    if(tmp_f32 >= 800)
                        tmp_f32 = 800;
                }
                else if( isProductionMode() && tmp_f32 > 120 )
                    tmp_f32 = 120;
                game_state.camdist = tmp_f32;
            }
        xcase CMD_CAMDISTADJUST:
            {
                float camdistAdjustment = inpLevel(INP_MOUSEWHEEL) * -3.0;

                if(game_state.scrolled) // already used mouse wheel input on a scroll bar
                    break;

                 if(game_state.ortho)
                 {
                    game_state.ortho_zoom += camdistAdjustment * 10;
                }

                // Is the player trying to pull the camera back?
                if(camdistAdjustment > 0.0)
                {
                    // The player is implicitly asking for the 3rd person camera.
                    // If the player is not in the third person camera mode, put the
                    // player in it now.
                    if(control_state.first)
                    {
                        game_state.camdist = 0.0;
                        control_state.first = 0;
                        sendThirdPerson();
                    }

                    //Used by obstruction finder
                    cam_info.triedToScrollBackThisFrame = 1;
                }

                game_state.camdist += camdistAdjustment;

                if(game_state.camdist < 0.0)
                    game_state.camdist = 0.0;
                 if( baseedit_Mode() )
                {
                    int max = 800;

                    if( game_state.edit_base==kBaseEdit_Plot )
                        max = 1500;

                     if(game_state.camdist >= max)
                        game_state.camdist = max;
                }
                else if(game_state.camdist >= MAX_SCROLL_OUT_DIST)
                    game_state.camdist = MAX_SCROLL_OUT_DIST;
            }

        xcase CONCMD_NOTIMEOUT:
            comm_link.notimeout = control_state.notimeout;

        xcase CONCMD_FIRST:
            // The user asked to be transitioned to the 3rd person camera.
            // Make sure the camera is going to pull back at least a little to indicate the mode switch.
            if(game_state.camdist == 0.0)
                game_state.camdist = 10.0;
        xcase CONCMD_THIRD:
            // And now presenting the stupidest line of code in the project.

            control_state.first = !control_state.first;
            sendThirdPerson();

            // The user asked to be transitioned to the 3rd person camera.
            // Make sure the camera is going to pull back at least a little to indicate the mode switch.

            if(game_state.camdist == 0.0)
                game_state.camdist = 10.0;

        xcase CONCMD_FORWARD:
            {
                if(optionGet(kUO_CamFree))
                {
                    control_state.pyr.cur[1] = addAngle(control_state.pyr.cur[1],control_state.cam_pyr_offset[1]);
                    control_state.cam_pyr_offset[1] = 0;
                }
                if( control_state.move_input_state )
                    cmdParse( "autorun 0" );
                updateControlState(CONTROLID_FORWARD, MOVE_INPUT_CMD, control_state.move_input_state, cmdGetTimeStamp());
            }
        xcase CONCMD_FORWARD_MOUSECTRL:
            {
                if (control_state.move_input_state)
                {
                    timerStart(game_state.ctm_autorun_timer);
                    cmdParse( "autorun 0" );
                    updateControlState(CONTROLID_FORWARD, MOVE_INPUT_CMD, 1, cmdGetTimeStamp());
                }
                else
                {
                    if (timerElapsed(game_state.ctm_autorun_timer) > 2.0f)
                    {
                        updateControlState(CONTROLID_FORWARD, MOVE_INPUT_OTHER, 1, cmdGetTimeStamp());
                        cmdParse( "autorun 1" );
                    }

                    updateControlState(CONTROLID_FORWARD, MOVE_INPUT_CMD, 0, cmdGetTimeStamp());

                }
            }
        xcase CONCMD_BACKWARD:
            {
                if (optionGet(kUO_CamFree))
                {
                    control_state.pyr.cur[1] = addAngle(control_state.pyr.cur[1],control_state.cam_pyr_offset[1]);
                    control_state.cam_pyr_offset[1] = 0;
                }
                cmdParse( "autorun 0" );
                 updateControlState(CONTROLID_BACKWARD, MOVE_INPUT_CMD, control_state.move_input_state, cmdGetTimeStamp());
            }
        xcase CONCMD_LEFT:
            if (optionGet(kUO_CamFree))
            {
                control_state.pyr.cur[1] = addAngle(control_state.pyr.cur[1],control_state.cam_pyr_offset[1]);
                control_state.cam_pyr_offset[1] = 0;
            }
            updateControlState(CONTROLID_LEFT, MOVE_INPUT_CMD, control_state.move_input_state, cmdGetTimeStamp());

        xcase CONCMD_RIGHT:
            if (optionGet(kUO_CamFree))
            {
                control_state.pyr.cur[1] = addAngle(control_state.pyr.cur[1],control_state.cam_pyr_offset[1]);
                control_state.cam_pyr_offset[1] = 0;
            }
            updateControlState(CONTROLID_RIGHT, MOVE_INPUT_CMD, control_state.move_input_state, cmdGetTimeStamp());

        xcase CONCMD_UP:
            updateControlState(CONTROLID_UP, MOVE_INPUT_CMD, control_state.move_input_state, cmdGetTimeStamp());

        xcase CONCMD_DOWN:
            updateControlState(CONTROLID_DOWN, MOVE_INPUT_CMD, control_state.move_input_state, cmdGetTimeStamp());

        xcase CONCMD_NOCOLL:
            {
                if(control_state.first_packet_sent)
                {
                    ControlStateChange* csc = createControlStateChange();

                    csc->control_id = CONTROLID_NOCOLL;
                    csc->state2 = (control_state.nocoll>1) ? 2 : control_state.nocoll;
                    csc->time_stamp_ms = control_state.time.last_send_ms;

                    addControlStateChange(&control_state.csc_processed_list, csc, 1);
                }
            }

        xcase CMD_QUIT_TEAM:
            {
                team_Quit(0);
            }
        xcase CMD_MEMCHECK:
            #define  CLEAR_CRT_DEBUG_FIELD(a) _CrtSetDbgFlag(~(a) & _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG))
            #define  SET_CRT_DEBUG_FIELD(a)   _CrtSetDbgFlag((a) | _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG))

            if (mem_check_frequency == 1)
            {
                SET_CRT_DEBUG_FIELD((_CRTDBG_ALLOC_MEM_DF|_CRTDBG_CHECK_ALWAYS_DF));
                //SET_CRT_DEBUG_FIELD((_CRTDBG_ALLOC_MEM_DF));
            }
            if (mem_check_frequency == 0)
            {
                CLEAR_CRT_DEBUG_FIELD((_CRTDBG_ALLOC_MEM_DF|_CRTDBG_CHECK_ALWAYS_DF));
            }
        xcase CMD_MMONITOR:
            memMonitorDisplayStats();
        xcase CMD_MEMLOG_DUMP:
            memlog_dump(0);
        xcase CMD_MEMORYUSEDUMP:
            memMonitorLogStats();
        xcase CMD_MEMLOG_ECHO:
            memlog_setCallback(0, conPrintf);
        xcase CMD_ECHO:
            status_printf("%s", tmp_str);
        xcase CMD_STRINGTABLE_MEM_DUMP:
            printStringTableMemUsage();
        xcase CMD_STASHTABLE_MEM_DUMP:
            printStashTableMemDump();
        xcase CMD_SHOW_LOAD_MEM_USAGE:
            setShowLoadMemUsage(tmp_int);
        xcase CMD_GETPOS:
            {
                Entity *player = playerPtr();
                if (player) {
                    sprintf_s(SAFESTR(buffer1000), "%1.1f %1.1f %1.1f", ENTPOSPARAMS(player));
                    addSystemChatMsg(buffer1000, INFO_SVR_COM, 0);
                }
            }
        xcase CMD_WHEREAMI:
            {
                Entity *player = playerPtr();
                if (player) {
                    char mapName[256];
                    char buffer[256];

                    clean_map_name(SAFESTR(mapName));
                    msPrintf(menuMessages, SAFESTR(buffer), game_state.shard_name);

                    sprintf_s(SAFESTR(buffer1000), "Server: %s\n", buffer);
                    strcatf(buffer1000, "Zone/Mission: %s\n", mapName);
                    if (player->access_level > ACCESS_USER) {
                        strcatf(buffer1000, "Position: %1.1f %1.1f %1.1f\n", ENTPOSPARAMS(player));
                        strcatf(buffer1000, "Server IP: %s:%d", makeIpStr(comm_link.addr.sin_addr.S_un.S_addr), comm_link.addr.sin_port);
                    }
                    addSystemChatMsg(buffer1000, INFO_DEBUG_INFO, 0);
                }
            }
        xcase CMD_COMETOME:
            {
                Entity *player = playerPtr();
                if (player) {
                    cmdParse("b @accesslevel 9");
                    sprintf_s(SAFESTR(buffer1000), "b @setpos %1.3f %1.3f %1.3f", ENTPOSPARAMS(player));
                    cmdParse(buffer1000);
                }
            }
        xcase CMD_NO_THREAD:
            {
                game_state.disableGeometryBackgroundLoader = 1;//doesn't disable thread, just asynchronous loading
                texDisableThreadedLoading();
            }
        xcase CMD_LEVEL:
            {
              //    if( character_CanBuyBoost( playerPtr()->pchar ) )
                //{
          //            start_menu( MENU_LEVEL_SPEC ); //MENU_LEVEL_SPEC );
                //}
                //else
                     start_menu( MENU_LEVEL_POWER );
            }
        xcase CMD_CONTACTDIALOG_CLOSE:
            cd_clear();
        xcase CMD_CONTEXT_MENU:
            contextMenu_activateElement( tmp_int );
        xcase CMD_SUPERGROUP_REGISTER:
            srEnterRegistrationScreen();
        xcase CMD_START_MENU:
            start_menu(tmp_int);
        xcase CMD_WEB_BROWSER_MENU:
            //if (current_menu() != MENU_WEB_BROWSER)
            //    start_menu(MENU_WEB_BROWSER);
        xcase CMD_WEB_BROWSER_GOTO_URL:
            //if( webStoreFrameIsVisible() )
            //    uiWebBrowser_goto_url(tmp_str);    // web store frame is visible, just change URL
            //else
            //    webOpenURLNoStore(tmp_str);    // open browser and goto url
        xcase CMD_WEB_BROWSER_GET_WEB_RESET_DATA:
            //uiWebBrowser_get_web_reset_data(tmp_str);
        xcase CMD_TAILOR_MENU:
            start_menu(MENU_TAILOR);
            requestSGColorData();
        xcase CMD_ULTRA_TAILOR_MENU:
            genderChangeMenuStart();
#if 0
        xcase CMD_ACCESSLEVEL:
            if (playerPtr())
            {
                char    buf[100];
                int        okay=1;

                /*  Code to warn external people (won't work for servers in Korea, etc, do we care?)
                if (strncmp(makeIpStr(getHostLocalIp()), "172.31.", 7)!=0) {
                    if (IDYES!=MessageBox(hwnd, "This command is for Cryptic/NCSoft employee debugging only.  If you are not a Cryptic/NCSoft employee, clicking yes to this dialog will cause your account to be TERMINATED.  Are you sure you wish to execute this command?", str, MB_YESNO | MB_ICONERROR)) {
                        okay=0;
                    }
                }
                */
                if (okay) {
                    playerPtr()->access_level = tmp_int;
                    sprintf_s(SAFESTR(buf),"s_accesslevel %d",tmp_int);
                    cmdParse(buf);
                }
            }
#endif
        xcase CMD_LEVELUP:
            if (playerPtr())
            {
                sprintf_s(SAFESTR(buffer1000), "levelupxp %d", tmp_int);
                cmdParse(buffer1000);

                testLevelup(tmp_int);
            }
        xcase CMD_SHOWCONTACTLIST:
        {
            ContactListDebugPrint();
        }
        xcase CMD_SHOWTASKLIST:
        {
            TaskListDebugPrint();
        }
        xcase CMD_PERFTEST:
            renderPerfTest(tmp_int, tmp_int2, 0);
        xcase CMD_PERFTESTSUITE:
            renderPerfTestSuite(tmp_int);
        xcase CMD_NOSHAREDMEMORY:
            sharedMemorySetMode(SMM_DISABLED);
        xcase CMD_QUICKLOAD:
            quickload = tmp_int;
        xcase CMD_ASSERT:
            assert(!"Someone typed /assert!");
        xcase CMD_REPLY:
            chatInitiateReply();
        xcase CMD_POPMENU:
            menu_Show(tmp_str, -1, -1);
        xcase CMD_QUICKCHAT:
        {
            int xqc, yqc, wqc, hqc;
            if(!game_state.edit)
            {
                GetQuickChatZone(&xqc, &yqc, &wqc, &hqc);
                menu_Show("QuickChat", xqc+wqc/2, yqc+hqc/2);
            }
        }
        xcase CMD_AUCTIONHOUSE_WINDOW:
        {
            Entity *e = playerPtr();

            if (e && e->pl && accountLoyaltyRewardHasNode(e->pl->loyaltyStatus, "RemoteAuction") )        
            { 
                if (game_state.mission_map)
                {
                    addSystemChatMsg(textStd("RemoteAuctionUnavailable"), INFO_USER_ERROR,0);
                } else {
                    window_openClose(WDW_AUCTION);
                }
            }
        }
        xcase CMD_PERSONALSTORAGE_WINDOW:
        {
            Entity *e = playerPtr();
            if (e && e->pl && accountLoyaltyRewardHasNode(e->pl->loyaltyStatus, "RemoteVault") )        
                ShowStoredSalvage(0,0);
        }
        xcase CMD_MISSIONSEARCH_CHOICE:
            //missionsearch_IntroChoice();
            missionsearch_MakeMission(0);
        xcase CMD_MISSIONMAKER_WINDOW:
            missionsearch_MakeMission(0);
        xcase CMD_MISSIONSEARCH_WINDOW:
            missionsearch_PlayMission(0);
        xcase CMD_OPEN_CUSTOM_VG:
            window_openClose(WDW_CUSTOMVILLAINGROUP);
        xcase CMD_QUICKPLACEONME:
        {
            if(!sel.parent)
            {
                sel.fake_ref.def = groupDefFind(edit_state.quickPlacementObject);
                if (sel.fake_ref.def)// && !sel.lib_load)
                {
                    copyMat3(edit_state.quickPlacementMat3, sel.fake_ref.mat);
                    unSelect(1);
                    selCopy(ENTPOS(playerPtr()));
                    editCmdPaste();
                }
            }
        }
        xcase CMD_DEMOPLAY:
            ;//demoStartPlay(demo_state.demoname);
        xcase CMD_DEMORECORD:
            demoStartRecord(demo_state.demoname);
        xcase CMD_DEMORECORD_AUTO:
        {
            char datestamp[260];
            timerMakeDateString_fileFriendly_s(SAFESTR(datestamp));
            sprintf_s(SAFESTR(demo_state.demoname), "demo-%s", datestamp);
            demoStartRecord(demo_state.demoname);
        }
        xcase CMD_DEMOSTOP:
            demoStop();
        xcase CMD_SETFADE:
            DoorAnimSetFade(tmp_f32);
        xcase CMD_ENTERDOOR:
            mapSelectClose();

            if (strcmp(tmp_str, "-1")==0)
                break;
            enterDoorClient(tmp_vec, tmp_str);
        xcase CMD_ENTERDOORVOLUME:
            mapSelectClose();

            if (strcmp(tmp_str, "-1")==0)
                break;
            enterDoorClient(NULL, tmp_str);
        xcase CMD_ENTERDOORFROMSGID:
            if (isMapSelectOpen())
            {
            mapSelectClose();
            if (strcmp(tmp_str, "-1")==0)
                break;
            enterDoorFromSgid(tmp_int, tmp_str);
            }
        xcase CMD_NOP:
            {}
        xcase CMD_SG_ENTER_PASSCODE:
			if (isMapSelectOpen()) {
				dialogNameEdit(textStd("Enter Supergroup Base Access Passcode:"), NULL, NULL, sgPasscodeDlgHandler, NULL, 30, 0);
			}
        xcase CMD_MACRO:
            macro_create( tmp_str, tmp_str2, NULL, -1 );
        xcase CMD_MACRO_IMAGE:
			macro_create( tmp_str2, tmp_str3, tmp_str, -1 );
        xcase CMD_MACRO_SLOT:
            macro_create( tmp_str, tmp_str2, NULL, tmp_int );
        xcase CMD_MANAGE_ENHANCEMENTS:
            start_menu( MENU_COMBINE_SPEC );
        xcase CMD_SCREENSHOT:
            jpeg_screenshot(NULL);
        xcase CMD_SCREENSHOT_TITLE:
            jpeg_screenshot(tmp_str);
        xcase CMD_SCREENSHOT_TGA:
            gfxScreenDump("screenshots", 0, "tga");
        xcase CMD_POWEXEC_NAME:
            trayslot_findAndSelect(playerPtr(), tmp_str, 0);
        xcase CMD_POWEXEC_LOCATION:
			trayslot_findAndSelectLocation(playerPtr(), tmp_str, tmp_str2);
        xcase CMD_POWEXEC_SLOT:
            //if(tmp_int>0 && tmp_int<=TRAY_SLOTS && playerPtr()!=NULL)
            {
                Entity *e = playerPtr();
                if (e)
                {
                    if (character_IsInPowerMode(e->pchar, kPowerMode_ServerTrayOverride))
                    {
                        trayslot_select(playerPtr(), kTrayCategory_ServerControlled, 0, tmp_int-1);
                    }
                    else
                    {
                        CurTrayType tray = tray_razer_adjust( kCurTrayType_Primary );

                        if( INRANGE(tray, kCurTrayType_Primary, kCurTrayType_Count) && can_powexec_slot( tray, tmp_int - 1 ) )
                            trayslot_select(e, kTrayCategory_PlayerControlled, e->pl->tray->current_trays[tray], tmp_int-1);
                    }
                }
            }

        xcase CMD_POWEXEC_ALTSLOT:
            //if(tmp_int>0 && tmp_int<=TRAY_SLOTS && playerPtr()!=NULL)
            {
                CurTrayType tray = tray_razer_adjust( kCurTrayType_Alt );

                if( INRANGE(tray, kCurTrayType_Primary, kCurTrayType_Count) && can_powexec_slot( tray, tmp_int ) )
                    trayslot_select(playerPtr(), kTrayCategory_PlayerControlled, playerPtr()->pl->tray->current_trays[tray], tmp_int-1);
            }

             // -AB: added for new tray support :2005 Jan 28 05:05 PM
        xcase CMD_POWEXEC_ALT2SLOT:
            //if(tmp_int>0 && tmp_int<=TRAY_SLOTS && playerPtr()!=NULL)
            {
                CurTrayType tray = tray_razer_adjust( kCurTrayType_Alt2 );

                if( INRANGE(tray, kCurTrayType_Primary, kCurTrayType_Count) && can_powexec_slot( tray, tmp_int ) )
                    trayslot_select(playerPtr(), kTrayCategory_PlayerControlled, playerPtr()->pl->tray->current_trays[tray], tmp_int-1);
            }

        xcase CMD_POWEXEC_SERVER_SLOT:
            //if(tmp_int>0 && tmp_int<=TRAY_SLOTS && playerPtr()!=NULL)
            {
                trayslot_select(playerPtr(), kTrayCategory_ServerControlled, 0, tmp_int-1);
            }

        xcase CMD_POWEXEC_TRAY:
            //if(tmp_int>0 && tmp_int<=TRAY_SLOTS && tmp_int2>0 && tmp_int2<=TOTAL_PLAYER_TRAYS && playerPtr()!=NULL)
            {
                if (!gMacroOnstack || !trayslot_istype(playerPtr(), tmp_int-1, tmp_int2-1, kTrayItemType_Macro))
                    trayslot_select(playerPtr(), kTrayCategory_PlayerControlled, tmp_int2-1, tmp_int-1);
            }
        xcase CMD_POWEXEC_TOGGLE_ON:
            trayslot_findAndSelect(playerPtr(), tmp_str, 1);
        xcase CMD_POWEXEC_TOGGLE_OFF:
            trayslot_findAndSelect(playerPtr(), tmp_str, -1);
        xcase CMD_POWEXEC_ABORT:
            entity_CancelWindupPower(playerPtr());
            tray_CancelQueuedPower();
            trayobj_ExitStance(playerPtr());
            trayslot_findAndSetDefaultPower(playerPtr(), NULL);

        xcase CMD_POWEXEC_UNQUEUE:
            tray_CancelQueuedPower();
            trayobj_ExitStance(playerPtr());

        xcase CMD_POWEXEC_AUTO:
            trayslot_findAndSetDefaultPower(playerPtr(), tmp_str);

        xcase CMD_INSPEXEC_SLOT:
            inspiration_activateSlot( tmp_int-1, 0 ); // params validated internally

        xcase CMD_INSPEXEC_TRAY:
            inspiration_activateSlot( tmp_int2-1, tmp_int-1 ); // params validated internally

        xcase CMD_INSPEXEC_NAME:
            inspiration_findAndActivateSlot( tmp_str );
        xcase CMD_INSPEXEC_PET_NAME:
            pet_inspireByName(tmp_str, tmp_str2);
        xcase CMD_INSPEXEC_PET_TARGET:
            pet_inspireTarget(tmp_str);
        xcase CMD_LOGFILEACCESS:
            fileSetLogging(1);
        xcase CMD_MINIMAP_RELOAD:
            miniMapLoad();
        xcase CMD_WINDOW_RESETALL:
            windows_ResetLocations();
        xcase CMD_WINDOW_TOGGLE:
            windows_Toggle(tmp_str);
        xcase CMD_MAIL_VIEW:
            email_setView(tmp_str);
        xcase CMD_WINDOW_NAMES:
            window_Names();
        xcase CMD_WINDOW_SHOW:
            windows_Show(tmp_str);
        xcase CMD_WINDOW_SHOW_DBG:
            windows_ShowDbg(tmp_str);
        xcase CMD_WINDOW_HIDE:
            windows_Hide(tmp_str);
        xcase CMD_WINDOW_COLOR:
            windows_ChangeColor();
        xcase CMD_WINDOW_CHAT:
            window_openClose( WDW_CHAT_BOX );
        xcase CMD_WINDOW_TRAY:
            window_openClose( WDW_TRAY );
        xcase CMD_WINDOW_TARGET:
            window_openClose( WDW_TARGET );
        xcase CMD_WINDOW_NAV:
            window_openClose( WDW_COMPASS );
        xcase CMD_WINDOW_MAP:
            window_openClose( WDW_MAP );
        xcase CMD_WINDOW_MENU:
             dock_open();
        xcase CMD_CHAT_OPTIONS_MENU:
            {
                if(tmp_int < 0 || tmp_int >= MAX_CHAT_WINDOWS)
                    addSystemChatMsg(textStd("BadWindowRange"), INFO_USER_ERROR,0);
                else
                    openChatOptionsMenu(tmp_int);
            }
        xcase CMD_PET_OPTIONS_MENU:
            petoption_open();
        xcase CMD_WINDOW_POWERS:
            window_openClose( WDW_POWERLIST );
        xcase CMD_FULLSCREEN:
            // Windowed mode not supported under Cider.
            game_state.fullscreen |= IsUsingCider();
        xcase CMD_TTDEBUG:
            ttDebugEnable(tmp_int);
        xcase CMD_SAY:
            uiChatSendToCurrentChannel(tmp_str);
        xcase CMD_COPYCHAT:
            DebugCopyChatToClipboard(tmp_str);
        xcase CMD_BUG:
            {
//                if ( AccountIsVIP(inventoryClient_GetAcctInventorySet(), inventoryClient_GetAcctStatusFlags()))
                if ( true )
                {
                    setPetitionMode(PETITION_BUG);
                    setPetitionSummary(tmp_str);
                    window_setMode(WDW_PETITION, WINDOW_DISPLAYING);
                } else {
                    addSystemChatMsg(textStd("InvalidPermissionVIP"), INFO_USER_ERROR, 0 );
                }

            }
        xcase CMD_BUG_INTERNAL:
            {
//                if ( AccountIsVIP(inventoryClient_GetAcctInventorySet(), inventoryClient_GetAcctStatusFlags()))
                if ( true )
                {
                    // used to send /bug report after user closes Bug window
                    BugReportInternal(tmp_str, tmp_int);
                    if(tmp_int)        // the /csrbug command will set tmp_int == 0, so no message is displayed
                        conPrintf(textStd("BugLogged"));
                } else {
                    addSystemChatMsg(textStd("InvalidPermissionVIP"), INFO_USER_ERROR, 0 );
                }
            }
        xcase CMD_CBUG:
            {
//                if ( AccountIsVIP(inventoryClient_GetAcctInventorySet(), inventoryClient_GetAcctStatusFlags()))
                if ( true )
                {
                    BugReport(tmp_str, BUG_REPORT_INTERNAL);
                    conPrintf(textStd("CBugLogged"));
                } else {
                    addSystemChatMsg(textStd("InvalidPermissionVIP"), INFO_USER_ERROR, 0 );
                }
            }
        xcase CMD_QA_BUGREPORT:
            {
                BugReport(tmp_str, BUG_REPORT_LOCAL); // indicates store report in client log
                conPrintf(textStd("QABug"));
            }
        xcase CMD_TRAY_MOMENTARY_ALT_TOGGLE:
            {
                if( !game_state.razerMouseTray )
                    tray_momentary_alt_toggle( kCurTrayMask_ALT, tmp_int );
            }
        xcase CMD_TRAY_MOMENTARY_ALT2_TOGGLE:
            {
                if( game_state.razerMouseTray )
                    tray_momentary_alt_toggle( kCurTrayMask_ALT, tmp_int );
                else
                    tray_momentary_alt_toggle( kCurTrayMask_ALT2, tmp_int );
            }
        xcase CMD_TRAYTOGGLE:
            tray_toggleMode();
        xcase CMD_TRAY_STICKY:
            // only call on alt trays
            if( INRANGE( tmp_int, kCurTrayType_Alt, kCurTrayType_Count ) )
             tray_setSticky( tmp_int, tmp_int2 );
        xcase CMD_TRAY_STICKY_ALT2:
            // only call on alt trays
             tray_toggleSticky( kCurTrayType_Alt2 );
        xcase CMD_TRAY_NEXT:
            tray_change( tray_razer_adjust( kCurTrayType_Primary ), 1 );
        xcase CMD_TRAY_PREV:
            tray_change( tray_razer_adjust( kCurTrayType_Primary ), 0 );
        xcase CMD_TRAY_NEXT_ALT:
            tray_change( tray_razer_adjust( kCurTrayType_Alt ), 1 );
        xcase CMD_TRAY_PREV_ALT:
            tray_change( tray_razer_adjust( kCurTrayType_Alt ), 0 );
        xcase CMD_TRAY_NEXT_ALT2:
            tray_change( tray_razer_adjust( kCurTrayType_Alt2 ), 1 );
        xcase CMD_TRAY_PREV_ALT2:
            tray_change( tray_razer_adjust( kCurTrayType_Alt2 ), 0 );
        xcase CMD_TRAY_NEXT_TRAY:
            tray_change( tmp_int-1, 1 );
        xcase CMD_TRAY_PREV_TRAY:
            tray_change( tmp_int-1, 0 );
        xcase CMD_TRAY_GOTO:
            tray_goto( tmp_int - 1 );
        xcase CMD_TRAY_GOTO_ALT:
            tray_goto_tray( kCurTrayType_Alt, tmp_int - 1 );
        xcase CMD_TRAY_GOTO_ALT2:
            tray_goto_tray( kCurTrayType_Alt2, tmp_int - 1 );
        xcase CMD_TRAY_GOTO_TRAY:
             tray_goto_tray( tmp_int - 1, tmp_int2 - 1 );
        xcase CMD_TRAY_CLEAR:
            tray_Clear();
        xcase CMD_TEAM_SELECT:
            {
                Entity *e = playerPtr();
                team_select( e->teamup ? &e->teamup->members : NULL,tmp_int );
            }
        xcase CMD_PET_SELECT:
            pet_select( tmp_int );
        xcase CMD_PET_SELECT_NAME:
            pet_selectName( tmp_str );
        xcase CMD_LEVELINGPACT_QUIT:
            levelingpact_quitWindow(NULL);
        xcase CMD_SOUVENIRCLUE_PRINT:
            scPrintAll();

        xcase CMD_SOUVENIRCLUE_GET_DETAIL:
            scRequestDetails(getClueByID(tmp_int));
        xcase CMD_RELEASE:
            respawnYes(0);
        xcase CMD_PETITION:
            {
//                if ( AccountIsVIP(inventoryClient_GetAcctInventorySet(), inventoryClient_GetAcctStatusFlags()))
                if ( true )
                {
                    setPetitionMode(PETITION_COMMAND_LINE);
                    setPetitionSummary(tmp_str);
                    window_setMode(WDW_PETITION, WINDOW_DISPLAYING);
                } else {
                    addSystemChatMsg(textStd("InvalidPermissionVIP"), INFO_USER_ERROR, 0 );
                }
            }
        xcase CMD_COSTUME_CHANGE:
            {
                if( costume_change( playerPtr(), tmp_int ) )
                {
                    commAddInput(str);
                }
            }
        xcase CMD_EMOTE_COSTUME_CHANGE:
            {
                if( costume_change_emote_check( playerPtr(), tmp_int ) )
                {
                    commAddInput(str);
                }
            }
        xcase CMD_CHAT_CHANNEL_SET:
            chatChannelSet( tmp_str );
        xcase CMD_CHAT_CHANNEL_CYCLE:
            chatChannelCycle();
        xcase CMD_CHAT_HIDE_PRIMARY_CHAT:
            hidePrimaryChat();
        xcase CMD_TAB_CREATE:
            chatFilterOpen(tmp_str, tmp_int, tmp_int2);
        xcase CMD_TAB_CLOSE:
            chatFilterClose(tmp_str);
        xcase CMD_TAB_TOGGLE:
            chatFilterToggle();
        xcase CMD_TAB_NEXT:
            chatFilterCycle(tmp_int, 0);
        xcase CMD_TAB_PREV:
            chatFilterCycle(tmp_int, 1);
        xcase CMD_TAB_GLOBAL_NEXT:
            chatFilterGlobalCycle(0);
        xcase CMD_TAB_GLOBAL_PREV:
            chatFilterGlobalCycle(1);
        xcase CMD_TAB_SELECT:
            chatFilterSelect(tmp_str);
        xcase CMD_SHARDCOMM_NAME:
            sendShardCmd("name", "%s", tmp_str);
        xcase CMD_SHARDCOMM_GIGNORELIST:
            sendShardCmd("ignoring", "");
        xcase CMD_SHARDCOMM_CREATE:
            addChannelSlashCmd("create", tmp_str, 0, 0);
        xcase CMD_SHARDCOMM_JOIN:
            addChannelSlashCmd("join", tmp_str, 0, 0);
        xcase CMD_SHARDCOMM_LEAVE:
            sendShardCmd("leave", "%s", tmp_str);
        xcase CMD_SHARDCOMM_INVITEDENY:
            sendShardCmd("denyinvite", "%s %s", tmp_str, tmp_str2);
        xcase CMD_SHARDCOMM_CHAN_INVITE_TEAM:
            {
                if(canInviteToChannel(tmp_str))
                {
                    ChatChannel * channel = GetChannel(tmp_str);
                    int i;
                    Entity * e = playerPtr();
                    char * cmd= 0;
                    int count=0;

                    TeamMembers * members = 0;
                    if(e->teamup)
                        members = &e->teamup->members;
                    else if(e->taskforce)
                        members = &e->taskforce->members;
                    else
                    {
                        addSystemChatMsg(textStd("CommandRequiresTeamOrTaskForce"), INFO_USER_ERROR, 0 );
                        break;
                    }

                    estrConcatf(&cmd, "\"%s\" %d", escapeString(tmp_str), kChatId_DbId);

                    for(i=0;i<members->count;i++)
                    {
                        if(e->db_id != members->ids[i])
                        {
                            estrConcatf(&cmd, " %d", members->ids[i]);
                            count++;
                        }
                    }

                    if(count)
                        sendShardCmd("Invite", cmd);
                    else
                        addSystemChatMsg(textStd("NoTeamMembersToInvite"), INFO_USER_ERROR, 0 );

                    estrDestroy(&cmd);
                }
            }
        xcase CMD_SHARDCOMM_CHAN_INVITE_GF:
            {
                if(canInviteToChannel(tmp_str))
                {
                    ChatChannel * channel = GetChannel(tmp_str);
                    int i, count=0;
                    char * cmd = 0;

                    estrConcatf(&cmd, "\"%s\" %d", escapeString(tmp_str), kChatId_Handle);

                    for(i=0;i<eaSize(&gGlobalFriends);i++)
                    {
                        GlobalFriend * gf = gGlobalFriends[i];
                        if(!GetChannelUser(channel, gf->handle))
                        {
                            estrConcatf(&cmd, " \"%s\"", escapeString(gf->handle));
                            count++;
                        }
                    }

                    if(count)
                        sendShardCmd("Invite", cmd);
                    else
                        addSystemChatMsg(textStd("NoGlobalFriendsToInvite"), INFO_USER_ERROR, 0 );

                    estrDestroy(&cmd);
                }
            }
        xcase CMD_SHARDCOMM_USER_MODE:
            sendShardCmd("usermode", "%s %s %s", tmp_str, tmp_str2, tmp_str3);
        xcase CMD_SHARDCOMM_CHAN_LIST:
            sendShardCmd("chanlist", "%s", tmp_str);
        xcase CMD_SHARDCOMM_CHAN_MODE:
            sendShardCmd("chanmode", "%s %s", tmp_str, tmp_str2);
        xcase CMD_SHARDCOMM_CHAN_MOTD:
            sendShardCmd("chanmotd", "%s %s", tmp_str, tmp_str2);
        xcase CMD_SHARDCOMM_CHAN_DESC:
            sendShardCmd("chandesc", "%s %s", tmp_str, tmp_str2);
        xcase CMD_SHARDCOMM_CHAN_TIMEOUT:
            sendShardCmd("ChanSetTimeout", "%s %d", tmp_str, tmp_int);
        xcase CMD_SHARDCOMM_WATCHING:
            sendShardCmd("watching", "");
        xcase CMD_SHARDCOMM_WHOLOCAL:
            sendShardCmd("GetLocal", "%s", tmp_str );
        xcase CMD_SHARDCOMM_WHOLOCALINVITE:
            sendShardCmd("GetLocalInvite", "%s", tmp_str );
        xcase CMD_SHARDCOMM_WHOLOCALLEAGUEINVITE:
            sendShardCmd("GetLocalLeagueInvite", "%s", tmp_str );
        xcase CMD_DEMODUMPTGA:
            demo_state.demo_dump = 1;
        xcase CMD_SHARDCOMM_FRIENDS:
            {
                int i;
                conPrintf(textStd("GlobalFriendsListStr"));
                for(i = 0; i < eaSize(&gGlobalFriends); i++)
                {
                    if (gGlobalFriends[i]->status == GFStatus_Online || gGlobalFriends[i]->status == GFStatus_Playing)
                    {
                        conPrintf("@%s: %s - %s", gGlobalFriends[i]->handle, textStd("OnlineStr"), gGlobalFriends[i]->name);
                    }
                    else
                    {
                        conPrintf("@%s: %s", gGlobalFriends[i]->handle, textStd("OfflineStr"));
                    }
                }
            }
        xcase CMD_MYHANDLE:
            {
                if(UsingChatServer())
                    addSystemChatMsg( textStd("YourChatHandleIs", game_state.chatHandle), INFO_SVR_COM, 0 );
                else
                    addSystemChatMsg( textStd("NotConnectedToChatServer"), INFO_USER_ERROR, 0 );
            }
        xcase CMD_WINDOW_SET_SCALE:
        {
            Wdw * wdw = windows_Find( tmp_str, 0, WDW_FIND_SCALE );
            if( wdw )
                window_setScale( wdw->id, tmp_f32 );
        }
        xcase CMD_CHATLOG:
        {
            optionToggle(kUO_LogChat, 1);
            if( optionGet(kUO_LogChat) )
                addSystemChatMsg( textStd("ChatNowLogging"), INFO_SVR_COM, 0 );
            else
                addSystemChatMsg( textStd("ChatNotLogging"), INFO_SVR_COM, 0 );

        }
        xcase CMD_CLICKTOMOVE:
        {
            optionSet( kUO_ClickToMove, tmp_int, 1 );

            if( optionGet( kUO_ClickToMove) )
                addSystemChatMsg( textStd("ClickToMoveEnabled"), INFO_SVR_COM, 0 );
            else
                addSystemChatMsg( textStd("ClickToMoveDisabled"), INFO_SVR_COM, 0 );
        }
        xcase CMD_CLICKTOMOVE_TOGGLE:
        {
            optionToggle(kUO_ClickToMove, 1);

            if( optionGet( kUO_ClickToMove) )
                addSystemChatMsg( textStd("ClickToMoveEnabled"), INFO_SVR_COM, 0 );
            else
                addSystemChatMsg( textStd("ClickToMoveDisabled"), INFO_SVR_COM, 0 );
        }
        xcase CMD_ASSIST:
            targetAssist(NULL);
        xcase CMD_ASSIST_NAME:
            targetAssist(tmp_str);
        xcase CMD_TARGET_NAME:
            setCurrentTargetByName(tmp_str);
        xcase CMD_DIALOGYES:
            dialogYesNo(1);
        xcase CMD_DIALOGNO:
            dialogYesNo(0);
        xcase CMD_DIALOGANSWER:
            dialogAnswer( tmp_str );
        xcase CMD_COSTUME_SAVE:
        {
             if( costume_save( tmp_str ) )
                addSystemChatMsg( "Costume Saved", INFO_DEBUG_INFO, 0);
            else
                addSystemChatMsg( "Costume save failed", INFO_DEBUG_INFO, 0 );
        }
        xcase CMD_COSTUME_LOAD:
        {
            if( costume_load( tmp_str ) )
                addSystemChatMsg( "Costume Loaded", INFO_DEBUG_INFO, 0 );
            else
                addSystemChatMsg( "Costume load failed", INFO_DEBUG_INFO, 0 );
        }
        xcase CMD_COSTUME_RELOAD:
            reloadCostumes();
        xcase CMD_CSR_BUG:
        {
            char * infoEstr = NULL;
            char * reportEstr = NULL;
            char * pch;

            if(!stricmp(playerPtr()->name, tmp_str))
            {
                conPrintf(textStd("UseCSROnOther"));
                break;
            }

            // fill info string here
            estrConcatf(&infoEstr, "%s\\n", textStd("PlayerNameColon",playerPtr()->name));
            estrConcatf(&infoEstr, "%s\\n", textStd("AuthNameColon",auth_info.name));
            estrConcatf(&infoEstr, "\\n%s\\nloadmap %s\\nsetpospyr %1.2f %1.2f %1.2f %1.4f %1.4f %1.4f\\ncamdist %1.4f\\nthird %d\\n",
                        textStd("PositionColon"),
                        strstri(game_state.world_name, "maps"),
                        posX(&cam_info), posY(&cam_info), posZ(&cam_info),
                        control_state.pyr.cur[0], control_state.pyr.cur[1], control_state.pyr.cur[2],
                        game_state.camdist,
                        !control_state.first);


            // Clean up the strings. No \r, \n, or <>
            // (player info)
            for(pch=infoEstr; *pch!='\0'; pch++)
            {
                if(*pch=='\r' || *pch=='\n' || *pch=='<' || *pch=='>')
                {
                    *pch = ' ';
                }
            }

            // (summary)
            for(pch=tmp_str2; *pch!='\0'; pch++)
            {
                if(*pch=='\r' || *pch=='\n' || *pch=='<' || *pch=='>')
                {
                    *pch = ' ';
                }
            }

            estrConcatf(&reportEstr, "csrbug_internal \"%s\" \"%s\" \"%s\"", tmp_str, tmp_str2, infoEstr);
            cmdParse(reportEstr);
            estrDestroy(&reportEstr);
            estrDestroy(&infoEstr);
        }
        xcase CMD_SALVAGEDEFS_LIST:
        {
            int i;
            int n = eaSize(&g_SalvageDict.ppSalvageItems);

            for( i = 0; i<n; ++i )
            {
                SalvageItem const*s =  g_SalvageDict.ppSalvageItems[i];
                if(s)
                {
                    conPrintf("%d:%s;", i, s->pchName);
                }
            }
        }
        xcase CMD_CONCEPTDEFS_LIST:
        {
            int i;
            for( i = 1; conceptdef_ValidId(i) ; ++i )
            {
                ConceptDef const *s = conceptdef_GetById( i );
                if(s)
                {
                    conPrintf("%d:%s;", i, s->name);
                }
            }
        }
        xcase CMD_RECIPEDEFS_LIST:
        {
            int i;
            const DetailRecipe *r = NULL;
            for( i = 1; (r = recipe_GetItemById(i)); ++i )
            {
                conPrintf("%d:%s;", i, r->pchName);
            }
        }
        xcase CMD_SALVAGEINV_LIST:
        {
            Entity * e = playerPtr();
            if( e && e->pchar )
            {
                SalvageInventoryItem **invs = e->pchar->salvageInv;
                int numItems = eaSize(&invs);
                int i;

                conPrintf("num items: %d", numItems );

                for( i = 0; i < numItems ; ++i )
                {
                    if( invs[i] && invs[i]->salvage )
                    {
                        conPrintf("    %d;%s;amount=%d;chal_pts=%d;disp_name=%s",i,invs[i]->salvage->pchName,invs[i]->amount,invs[i]->salvage->challenge_points, invs[i]->salvage->ui.pchDisplayName);
                    }
                }
            }
        }
        xcase CMD_CONCEPTINV_LIST:
         {
             Entity * e = playerPtr();
             if( e && e->pchar )
             {
                 int i;
                 conPrintf("Inventory Items: ----------------------------------------\n");

                 for( i = 0; i < eaSize( &e->pchar->conceptInv ); ++i )
                 {
                     conPrintf("%d  %s: %d vars<%.2f,%.2f,%.2f,%.2f>", i, e->pchar->conceptInv[i]->concept?e->pchar->conceptInv[i]->concept->def->name : "<null>", e->pchar->conceptInv[i]->amount, e->pchar->conceptInv[i]->concept->afVars[0],e->pchar->conceptInv[i]->concept->afVars[1],e->pchar->conceptInv[i]->concept->afVars[2],e->pchar->conceptInv[i]->concept->afVars[3]);
                 }
                 conPrintf("------------------------------------------------------------\n");
             }
        }
        xcase CMD_RECIPEINV_LIST:
         {
             Entity * e = playerPtr();
             if( e && e->pchar )
             {
                 int i;
                 conPrintf("Inventory Items: ----------------------------------------\n");
                 for( i = eaSize( &e->pchar->recipeInv ) - 1; i >= 0; --i)
                 {
                     conPrintf("  %s: %d\n", e->pchar->recipeInv[i]->recipe?e->pchar->recipeInv[i]->recipe->pchName : "<null>", e->pchar->recipeInv[i]->amount);
                 }
                 conPrintf("------------------------------------------------------------\n");
             }
        }
        xcase CMD_DETAILINV_LIST:
         {
             Entity * e = playerPtr();
             if( e && e->pchar )
             {
                 int i;
                 conPrintf("Inventory Items: ----------------------------------------\n");
                 for( i = eaSize( &e->pchar->detailInv ) - 1; i >= 0; --i)
                 {
                     conPrintf("  %s: %d\n", e->pchar->detailInv[i]->item?e->pchar->detailInv[i]->item->pchName : "<null>", e->pchar->detailInv[i]->amount);
                 }
                 conPrintf("------------------------------------------------------------\n");
             }
        }
        xcase CMD_SALVAGE_REVERSE_ENGINEER:
        {
            Entity * e = playerPtr();
            if( e && e->pchar )
            {
                if( EAINRANGE( tmp_int, e->pchar->salvageInv ))
                {
                    crafting_reverseengineer_send( tmp_int );
                }
                else
                {
                    conPrintf("invalid param %d", tmp_int2 );
                }
            }
            else
            {
                conPrintf("no pchar or no entity for reverse engineer");
            }
        }
        xcase CMD_INVENT_SELECT_RECIPE:
        {
            Entity *e = playerPtr();
            if( e && e->pchar )
            {
                // todo: make sure character has room in inventory for boost.
                if( character_InventingStart( e->pchar, tmp_int ) )
                {
                    invent_SendSelectrecipe( tmp_int, tmp_int2 - 1 );
                    conPrintf("selecting recipe %s\n", e->pchar->invention->recipe->name );
                }
                else
                {
                    conPrintf("couldn't select recipe %d\n", tmp_int );
                }
            }
            else
            {
                conPrintf("no pchar or no entity for slotting.\n");
            }
        }
        xcase CMD_INVENT_SLOT_CONCEPT:
        {
            Entity *e = playerPtr();

            if( !EAINRANGE( tmp_int, e->pchar->conceptInv ) || !e->pchar->conceptInv[tmp_int])
            {
                conPrintf("%d not a valid concept index", tmp_int);
            }
            else if( !AINRANGE( tmp_int2, e->pchar->invention->slots ))
            {
                conPrintf("%d not a valid slot index", tmp_int2 );
            }
            else
            {
                invent_SendSlotconcept( tmp_int2, tmp_int );
                conPrintf("slotted.");
            }
        }
        xcase CMD_INVENT_UNSLOT:
        {
            Entity *e = playerPtr();

            if(!character_IsInventing(e->pchar))
            {
                conPrintf("character not inventing.");
            }
            if( !AINRANGE( tmp_int, e->pchar->invention->slots ))
            {
                conPrintf("%d not a valid slot index", tmp_int );
            }
            else if( e->pchar->invention->slots[tmp_int].state != kSlotState_Slotted )
            {
                conPrintf("Slot %d is not slotted with a concept. its state is %s", tmp_int, slotstate_Str(e->pchar->invention->slots[tmp_int].state));
            }
            else
            {
                invent_SendUnSlot( tmp_int );
                conPrintf("slotted.");
            }
        }
        xcase CMD_INVENT_HARDEN:
        {
            Entity *e = playerPtr();
            if( e && e->pchar )
            {
                if( !INRANGE0( tmp_int, ARRAY_SIZE( e->pchar->invention->slots ) ))
                {
                    conPrintf("%d is out of range.", tmp_int);
                }
                else if( e->pchar->invention->slots[tmp_int].state != kSlotState_Slotted)
                {
                    conPrintf("slot %d has no concepts ready to be hardened.", tmp_int);
                }
                {
                    // if the harden worked
                    invent_SendHardenslot(tmp_int);
                    conPrintf("hardened slot %d", tmp_int);
                }
            }
            else
            {
                conPrintf("no pchar or no entity for slotting");
            }
        }
        xcase CMD_INVENT_FINALIZE:
        {
            Entity *e = playerPtr();
            if( e && e->pchar )
            {
                invent_SendFinalize();
            }
            else
            {
                conPrintf("no pchar or no entity");
            }
        }
        xcase CMD_INVENT_CANCEL:
        {
            Entity *e = playerPtr();
            if( e && e->pchar )
            {
                invent_SendCancel();
            }
            else
            {
                conPrintf("no pchar or no entity");
            }
        }
        xcase CMD_INVENTION_LIST:
        {
            Entity *e = playerPtr();
            if( e && e->pchar && e->pchar->invention )
            {
                Invention *inv = e->pchar->invention;
                conPrintf("invention state is '%s'", inventionstate_Str(inv->state));
                if(inv->state == kInventionState_Invent )
                {
                    int i;

                    // --------------------
                    // boost

                    if( verify( INRANGE0( inv->boostInvIdx, CHAR_BOOST_MAX) ))
                    {
                        int i;
                        Boost *b = e->pchar->aBoosts[inv->boostInvIdx];

                        // vars
                        conPrintf("\tafVars: <%.2f,%.2f,%.2f,%.2f>",b->afVars[0],b->afVars[1],b->afVars[2],b->afVars[3]);

                        // level

                        // powerup slots
                        conPrintf("\tPowerup Slots:");
                        for( i = 0; powerupslot_Valid( &b->aPowerupSlots[i] ); ++i )
                        {
                            conPrintf("\t\t%d: powerup %s type %s", i, b->aPowerupSlots[i].reward.name, rewarditemtype_Str(b->aPowerupSlots[i].reward.type));
                        }
                    }

                    // --------------------
                    // slots

                    for( i = 0; AINRANGE( i, inv->slots ); ++i )
                    {
                        int j;
                        InventionSlot *slot = inv->slots + i;
                        conPrintf("\t%d slot: '%s'", i, slotstate_Str( slot->state ));
                        for( j = 0; j < eaSize(&slot->concepts); ++j )
                        {
                            ConceptItem *cpt = slot->concepts[j];
                            conPrintf("\t\t%d: %s",j, (cpt && cpt->def) ? cpt->def->name : "<null>");
                        }
                    }
                }
            }
            else
            {
                conPrintf("no invention.");
            }

        }
        xcase CMD_UIINVENTORY_VISIBILITY:
         {
             if( tmp_int )
             {
                 window_addChild( wdwGetWindow(WDW_TRAY), WDW_INVENTORY, "Inventory", NULL );
             }
             else
             {
                 window_removeChild( wdwGetWindow(WDW_TRAY), "Inventory" );
             }
         }
        xcase CMD_ENHANCEMENT_LIST:
        {
            Entity *e = playerPtr();
            if( e && e->pchar )
            {
                Character *p = e->pchar;
                if( INRANGE0( tmp_int, CHAR_BOOST_MAX ))
                {
                    Boost *boost = p->aBoosts[tmp_int];
                    if( boost )
                    {
                        int i;
                        conPrintf("%d: %s %f,%f,%f,%f",tmp_int, boost->ppowBase->pchName,
                                  boost->afVars[0],boost->afVars[1],boost->afVars[2],boost->afVars[3]);

                        conPrintf("powerup slots:");
                        for( i = 0; i < ARRAY_SIZE(boost->aPowerupSlots); ++i )
                        {
                            PowerupSlot *ps = boost->aPowerupSlots+i;
                            conPrintf("%d: %s %s %s", i, ps->filled?"< >":"<*>",
                                      rewarditemtype_Str(ps->reward.type),ps->reward.name);
                        }
                    }
                    else
                    {
                        conPrintf("NULL boost at %d",tmp_int);
                    }
                }
                else
                {
                    conPrintf("invalid id %d",tmp_int);
                }
            }
            else
            {
                conPrintf("no pchar or no entity for slotting");
            }
        }
        xcase CMD_VERSION:
        {
            const char *ver = getExecutablePatchVersion(NULL);
            if (ver)
                conPrintf(ver);
        }
        xcase CMD_ATLAS_DISPLAY_NEXT:
        {
            game_state.atlas_display++;
        }
        xcase CMD_ATLAS_DISPLAY_PREV:
        {
            game_state.atlas_display--;
        }
        xcase CMD_ADD_CARD_STATS:
        {
            rdrStatsAddCard(tmp_str);
        }
        xcase CMD_REM_CARD_STATS:
        {
            rdrStatsRemoveCard(tmp_str);
        }
        xcase CMD_SIMPLESTATS:
        {
            window_openClose(WDW_RENDER_STATS);
        }
        xcase CMD_ROOMHEIGHT:
        {
            int height[2] = {tmp_int,tmp_int2};
            baseedit_SetCurBlockHeights(height);
        }
        xcase CMD_DOORWAY:
        {
            int block[2] = {tmp_int,tmp_int2};
            F32 heights[2] = {8, tmp_int3};
            baseClientSendDoorway(g_refCurRoom.p,block,heights);
        }
        xcase CMD_MAKEROOM:
        {
            int        size[2];
            F32        height[2] = {0,48};

            size[0] = tmp_int2;
            size[1] = tmp_int3;
            baseClientSendRoomUpdate(tmp_int,size,tmp_vec,height,0,0);
        }
        xcase CMD_ROOMLOOK:
        {
            int tmp[2];
            tmp[0]=tmp_int;
            tmp[1]=tmp_int2;
//            baseLookRoom(tmp, NULL);
        }
        xcase CMD_ROOMLIGHT:
            baseedit_SetCurRoomLight(tmp_int,tmp_vec);
        xcase CMD_ROOMDETAIL:
        {
            Mat4 mat;
            copyMat4(unitmat, mat);
            copyVec3(tmp_vec, mat[3]);
            baseedit_MoveCurDetail(mat);
        }
        xcase CMD_LOCAL_TIME:
        {
            addSystemChatMsg(textStd( "LocalTime", timerMakeOffsetDateStringNoYearNoSecFromSecondsSince2000(timerSecondsSince2000())), INFO_SVR_COM, 0 );
        }
        xcase CMD_EDIT_BASE:
        {
            baseEditToggle( tmp_int );
        }
        xcase CMD_BASE_WORKSHOP_SETMODE:
        {
            if(tmp_int)
            {
                requestArchitectInventory();
                window_setMode(WDW_RECIPEINVENTORY, WINDOW_GROWING);
            }
            else
            {
                window_setMode(WDW_RECIPEINVENTORY, WINDOW_SHRINKING);
            }
        }
        xcase CMD_BASE_WORKSHOP_RECIPES_LIST:
        {
            int i;
            Entity *e = playerPtr();
            const DetailRecipe **rs = entity_GetValidDetailRecipes(e, NULL, e->pl->workshopInteracting);
            conPrintf("recipes: --------------------");
            for( i = 0; i < eaSize(&rs); ++i )
            {
                int j;
                int sizeSalReq = eaSize(&rs[i]->ppSalvagesRequired);
                conPrintf("%i: %s", rs[i]->id, rs[i]->pchName);
                for(j = 0; j < sizeSalReq; ++j)
                {
                    const SalvageRequired *s = rs[i]->ppSalvagesRequired[j];
                    conPrintf("\t\t%s, amount %d", s->pSalvage->pchName, s->amount);
                }
            }
            eaDestroyConst(&rs);
        }
        xcase CMD_BASE_WORKSHOP_RECIPE_CREATE:
        {
            Entity *e = playerPtr();
            if( verify( e && e->pchar ))
            {
                Character *p = e->pchar;

/*                if( !character_IsWorkshopping(p))
                {
                    conPrintf("not currently workshopping.");
                }
                else if( !character_WorkshopCanSendCmd(p) )
                {
                    conPrintf("current workshop state '%s' does not allow creation.", workshopstate_ToStr( p->workshop->state ));
                }
                else
                {*/
                    if(!character_WorkshopSendDetailRecipeBuild( p, tmp_str, DETAILRECIPE_USE_RECIPE_LEVEL, false ))
                    {
                        conPrintf("can't invent '%s'", tmp_str);
                    }
//                }
            }
        }
        xcase CMD_ARENA_MANAGEPETS:
        {
            arena_ManagePets();
        }
        xcase CMD_PETCOMMAND:
            pet_command( tmp_str, NULL, kPetCommandType_Current );
        xcase CMD_PETCOMMAND_BY_NAME:
            pet_command( tmp_str2, tmp_str, kPetCommandType_Name );
        xcase CMD_PETCOMMAND_BY_POWER:
            pet_command( tmp_str2, tmp_str, kPetCommandType_Power );
        xcase CMD_PETCOMMAND_ALL:
            pet_command( tmp_str, NULL, kPetCommandType_All );
        xcase CMD_PETSAY:
            pet_say( tmp_str, NULL, kPetCommandType_Current );
        xcase CMD_PETSAY_BY_NAME:
            pet_say( tmp_str2, tmp_str, kPetCommandType_Name );
        xcase CMD_PETSAY_BY_POWER:
            pet_say( tmp_str2, tmp_str, kPetCommandType_Power );
        xcase CMD_PETSAY_ALL:
            pet_say( tmp_str, NULL, kPetCommandType_All );
        xcase CMD_PETRENAME:
            pet_rename(tmp_str,NULL,kPetCommandType_Current);
        xcase CMD_PETRENAME_BY_NAME:
            pet_rename(tmp_str2,tmp_str,kPetCommandType_Name);
        xcase CMD_DEBUGSEAMS:
        {
            if (tmp_int_special[0])
            {
                char buf[256];
                sprintf_s(SAFESTR(buf), "gfxdebug %d", GFXDEBUG_SEAMS);
                cmdParse(buf);
            }
            else
            {
                cmdParse("gfxdebug 0");
            }
        }
        xcase CMD_CREATEHERO:
            createCharacter();
        xcase CMD_CREATEVILLAIN:
            createCharacter();
        xcase CMD_CREATEPRAETORIAN:
            createCharacter();
        xcase CMD_NAGPRAETORIAN:
            dialogGoingRogueNag(GRNAG_CHARSELECT);
        xcase CMD_UISKIN:
            if (tmp_int >= UISKIN_HEROES && tmp_int <= UISKIN_PRAETORIANS)
            {
                if (tmp_int != game_state.skin)
                {
                    game_state.skin = tmp_int;
                    resetVillainHeroDependantStuff(0);
                }
            }
        xcase CMD_MARKPOWOPEN:
            markPowerOpen(tmp_int);
        xcase CMD_MARKPOWCLOSED:
            markPowerClosed(tmp_int);
        xcase CMD_HIDETRANS:
            msSetHideTranslationErrors(tmp_int);
        xcase CMD_RECALCLIGHTS:
            colorTrackerDeleteAll();
        xcase CMD_RECALCWELDS:
            freeWeldedModels();
        xcase CMD_DUMPPARSERALLOCS:
            ParserDumpStructAllocs();
        xcase CMD_TEXTPARSERDEBUG:
            TestTextParser();
        xcase CMD_TOGGLEFIXEDFUNCTIONFP:
            rdr_caps.use_fixedfunctionfp = !rdr_caps.use_fixedfunctionfp;
            conPrintf("rdr_caps.use_fixedfunctionfp = %d", rdr_caps.use_fixedfunctionfp);
        xcase CMD_TOGGLEFIXEDFUNCTIONVP:
            rdr_caps.use_fixedfunctionvp = !rdr_caps.use_fixedfunctionvp;
            conPrintf("rdr_caps.use_fixedfunctionvp = %d", rdr_caps.use_fixedfunctionvp);
        xcase CMD_COSTUME_DIFF:
            costumeReportDiff( tmp_int );
        xcase CMD_CAMTURN:
            control_state.cam_pyr_offset[1] = 0;
        xcase CMD_PLAYERTURN:
            control_state.pyr.cur[1] = addAngle(control_state.pyr.cur[1],control_state.cam_pyr_offset[1]);
            control_state.cam_pyr_offset[1] = 0;
        xcase CMD_FACE:
            playerToggleFollowMode();
            playerDoFollowMode();
            playerToggleFollowMode();
        xcase CMD_WINDOW_CLOSE_EXTRA:
            returnToGame();
        xcase CMD_CLEARCHAT:
            clearAllChat();
        xcase CMD_CRITTER_CYCLE:
            cycleCritterCostumes(1,0);
        xcase CMD_RESET_KEYBINDS:
        {
            Entity *e = playerPtr();
            keybind_reset( e->pl->keyProfile, &game_binds_profile);
        }
        xcase CMD_CTS_TOGGLE:
        {
            g_ctsstate ^= tmp_int;
        }
        xcase CMD_SAVE_CSV:
        {
            Entity * e = playerPtr();
            imageserver_WriteToCSV(e->costume, NULL, tmp_str ? tmp_str : "C:\temp.csv");
        }
        xcase CMD_LOAD_CSV:
        {
            Entity * e = playerPtr();
            Costume* mutable_costume = costume_as_mutable_cast(e->costume);
            imageserver_ReadFromCSV(&mutable_costume, NULL, tmp_str ? tmp_str : "C:\temp.csv");
            costume_Apply(e);
        }
        xcase CMD_AUTH_LOCAL_SHOW:
        {
            conShowAuthUserData();
        }
        xcase CMD_AUTH_LOCAL_SET:
        {
            conSetAuthUserData(tmp_str,tmp_int);
        }
        xcase CMD_CRASHCLIENT:
        {
            volatile int a=1,b=0;
            a = a / b;
            printf("%d",a);
        }
        xcase CMD_MONITORATTRIBUTE:
            combatNumbers_Monitor( tmp_str );
        xcase CMD_STOPMONITORATTRIBUTE:
            combatNumbers_StopMonitor( tmp_str );
        xcase CMD_COMBINEINSP:
            inspirationMerge( tmp_str, tmp_str2 );
        xcase CMD_DELETEINSP:
            inspirationDeleteByName( tmp_str );
        xcase CMD_CHATLINK:
            chatLink( tmp_str );
        xcase CMD_TEXTLINK:
            textLink( tmp_str );
        xcase CMD_CHATLINKGLOBAL:
            chatLinkGlobal( tmp_str, tmp_str2);
        xcase CMD_CHATLINKCHANNEL:
            chatLinkChannel( tmp_str );
        xcase CMD_NEWTRAY:
            showNewTray();
        xcase CMD_HIDE:
            hideShowDialog(0);
        xcase CMD_HIDE_SEARCH:
            hideSetValue( HIDE_SEARCH, 1 );
        xcase CMD_HIDE_SG:
            hideSetValue( HIDE_SG, 1 );
        xcase CMD_HIDE_FRIENDS:
            hideSetValue( HIDE_FRIENDS, 1 );
        xcase CMD_HIDE_GFRIENDS:
            hideSetValue( HIDE_GLOBAL_FRIENDS, 1 );
        xcase CMD_HIDE_GCHANNELS:
            hideSetValue( HIDE_GLOBAL_CHANNELS, 1 );
        xcase CMD_HIDE_TELL:
            hideSetValue( HIDE_TELL, 1 );
        xcase CMD_UNHIDE_TELL:
            hideSetValue( HIDE_TELL, 0 );
        xcase CMD_HIDE_INVITE:
            hideSetValue( CMD_HIDE_INVITE, 1 );
        xcase CMD_UNHIDE_INVITE:
            hideSetValue( CMD_UNHIDE_INVITE, 0 );
        xcase CMD_UNHIDE:
            hideShowDialog(1);
        xcase CMD_UNHIDE_SEARCH:
            hideSetValue( HIDE_SEARCH, 0);
        xcase CMD_UNHIDE_SG:
            hideSetValue( HIDE_SG, 0 );
        xcase CMD_UNHIDE_FRIENDS:
            hideSetValue( HIDE_FRIENDS, 0 );
        xcase CMD_UNHIDE_GFRIENDS:
            hideSetValue( HIDE_GLOBAL_FRIENDS,0 );
        xcase CMD_UNHIDE_GCHANNELS:
            hideSetValue( HIDE_GLOBAL_CHANNELS, 0 );
        xcase CMD_HIDE_ALL:
            hideSetAll( 1 );
        xcase CMD_UNHIDE_ALL:
            hideSetAll( 0 );
        xcase CMD_SET_POWERINFO_CLASS:
            powerInfoClassSelector();
        xcase CMD_SGKICK:
            sgroup_kickName(tmp_str);
        xcase CMD_MOUSE_SPEED:
            optionSetf( kUO_MouseSpeed, tmp_f32, 1 );
        xcase CMD_MOUSE_INVERT:
            optionSet( kUO_MouseInvert, tmp_int, 1 );
        xcase CMD_SPEED_TURN:
            optionSetf( kUO_SpeedTurn, tmp_f32, 1 );
        xcase CMD_OPTION_LIST:
            optionList();
        xcase CMD_OPTION_SET:
            optionSetByName( tmp_str, tmp_str2 );
        xcase CMD_OPTION_TOGGLE:
            optionToggleByName( tmp_str );
        xcase CMD_PLAYER_NOTE:
            playerNote_GetWindow( tmp_str, 1 );
        xcase CMD_PLAYER_NOTE_LOCAL:
            playerNote_GetWindow( tmp_str, 0 );
        xcase CMD_TEST_LOGIN_FAIL:
            //dialogStd( DIALOG_YES_NO, "NotPaidBrowserPrompt", NULL, NULL, BrowserOpenLoginFailure, NULL, 0 );
        xcase CMD_CUSTOM_WINDOW:
            createCustomWindow(tmp_str);
        xcase CMD_CUSTOM_TOGGLE:
            customWindowToggle(tmp_str);
        xcase CMD_MISSIONMAKER_NAV:
            mmscrollsetViewList(tmp_str, 0);
        xcase CMD_MISSIONMAKER_NAV2:
            mmscrollsetViewList(tmp_str, 1);
        xcase CMD_ARCHITECTSAVETEST:
            missionMakerSave(MM_TEST);
        xcase CMD_ARCHITECTSAVEEXIT:
            missionMakerSave(MM_EXIT);
        xcase CMD_ARCHITECTFIXERRORS:
            missionMakerFixErrors(0);
        xcase CMD_ARCHITECTEXIT:
            missionMakerExitDialog(0);
        xcase CMD_ARCHITECTREPUBLISH:
            missionMakerRepublish();
        xcase CMD_ARCHITECT_CSRSEARCH_AUTHORED:
        case  CMD_ARCHITECT_CSRSEARCH_COMPLAINED:
        case  CMD_ARCHITECT_CSRSEARCH_VOTED:
        case  CMD_ARCHITECT_CSRSEARCH_PLAYED:
        {
            char *buf = estrTemp();
            estrConcatf(&buf, "%d %s", tmp_int, tmp_str);
            missionserver_game_requestSearchPage(cmd->num-CMD_ARCHITECT_CSRSEARCH_AUTHORED+MISSIONSEARCH_CSR_AUTHORED,
                                                    buf, 0, kMissionSearchViewedFilter_All, 0, 0);
            estrDestroy(&buf);
        }
        xcase CMD_ARCHITECT_GET_FILE:
            game_state.architect_dumpid = tmp_int;
            missionserver_game_requestArcData_viewArc(tmp_int);
        xcase CMD_ARCHITECT_TOGGLE_SAVE_COMPRESSION:
            toggleSaveCompressedCostumeArc(tmp_int);
        xcase CMD_ARCHITECT_IGNORE_UNLOCKABLE_VALIDATION:
            MMtoggleArchitectLocalValidation(tmp_int | MM_IGNORE_FROM_CONSOLE);
        xcase CMD_SMF_DRAWDEBUGBBOXES:
            smf_setDebugDrawMode(tmp_int);
        xcase CMD_SHOW_INFO:
            {
                Entity *e;
                int i;
                if (e = playerPtr())
                {
                    conPrintf("dbid: %d", e->db_id);
                    if (e->supergroup)
                    {
                        conPrintf("sgid: %d", e->supergroup_id);
                        if (e->sgStats)
                        {
                            for (i = eaSize(&e->sgStats) - 1; i >= 0; i--)
                            {
                                if (e->sgStats[i]->db_id == e->db_id)
                                {
                                    conPrintf("rank: %d", e->sgStats[i]->rank);
                                }
                            }
                        }
                    }
                }
            }
        xcase CMD_GENDER_MENU:
            genderChangeMenuStart();
        xcase CMD_OPEN_HELP:
            window_openClose(WDW_HELP);
        xcase CMD_MAKEMAPTEXTURES:
            ImageCapture_WriteMapImages(game_state.world_name, NULL,1);
        xcase CMD_TESTPOWERCUST:
            testPowerCust(tmp_int, tmp_str, tmp_str2);
        xcase CMD_RELOAD_POP_HELP:
            reloadPopHelp();
        xcase CMD_SCRIPT_WINDOW:
            {
            void detachScriptUIWindow(int detach);
            detachScriptUIWindow(!!tmp_int);
            }
        xcase CMD_DETACHED_CAMERA:
            toggleDetachedCamControls(!!tmp_int, &cam_info);
        xcase CMD_EMAILSEND:
            {
                if( globalEmailIsFull() )
                    dialogStd(DIALOG_OK,"EmailInboxFull", NULL, NULL,0,0,1);
                else
                    commAddInput(str);                    
            }
        xcase CMD_EMAILSENDATTACHMENT:
            {
                if( globalEmailIsFull() )
                    dialogStd(DIALOG_OK,"EmailInboxFull", NULL, NULL,0,0,1);
                else
                    commAddInput(str);                
            }

        xcase CMD_TURNSTILE_REQUEST_EVENT_LIST:
            {
                turnstileGame_generateRequestEventList();
            }
        xcase CMD_TURNSTILE_QUEUE_FOR_EVENTS:
            {
                turnstileGame_generateQueueForEventsByString(tmp_str);
            }
        xcase CMD_TURNSTILE_REMOVE_FROM_QUEUE:
            {
                turnstileGame_generateRemoveFromQueue();
            }
        xcase CMD_TURNSTILE_EVENT_RESPONSE:
            {
                turnstileGame_generateEventResponse(tmp_str);
            }
        xcase CMD_CLEAR_NDA_SIGNATURE:
            dbSignNDA(0x7FFFFFFF, 0);
            dbSignNDA(0x7FFFFFFF, 1);
#ifndef FINAL
        xcase CMD_CREATE_ACCESS_KEY:
            createEncryptedKeyAccessLevel(tmp_str, atoi(tmp_str2) );
        xcase CMD_TEST_ACCESS_KEY:
            {
                if( !fileExists(keyfile) )
                    conPrintf("Access key file '%s' not found in game directory", keyfile);
                else
                {
                    int level = encryptedKeyedAccessLevel();
                    conPrintf("dev_access.key file found for user '%s', access level %d", encrypedKeyIssuedTo ? encrypedKeyIssuedTo:"NONE", level);
                }
            }
#endif
        xcase CMD_REDIRECT:
            {
                redirectMenuTrigger(tmp_int);
            }
        xcase CMD_WEB_STORE:
            {
                //webStoreOpenDefault();
            }
        xcase CMD_WEB_STORE_OPEN_PRODUCT:
            {
                webStoreOpenProduct(tmp_str);
                cd_clear();  //HACK: close any open dialog when going to the store
            }
        xcase CMD_LWCDEBUG_SET_DATA_STAGE:
            {
                LWC_SetCurrentDataStage(tmp_int);
            }
        xcase CMD_LWCDEBUG_MODE:
            {
                LWCDebug_SetMode(tmp_int);
            }
        xcase CMD_LWCDOWNLOADRATE:
            {
                LWC_SetDownloadRate(tmp_int);
            }
        xcase CMD_ACCOUNT_DEBUG_BUY_PRODUCT:
            {
#ifndef FINAL
				AccountStoreBuyProduct( auth_info.uid, skuIdFromString(tmp_str) /* sku */, tmp_int /* quantity */ ); 
#else
				conPrintf("Debug commands not available in release client");
#endif
            }
        xcase CMD_CATALOG_DEBUG_PUBLISH_PRODUCT:
            {
#ifndef FINAL
                AccountStorePublishProduct( auth_info.uid, skuIdFromString(tmp_str), ( tmp_int ? true : false ) );
#else
                conPrintf("Debug commands not available in release client");
#endif
            }
        xcase CMD_ACCOUNT_QUERY_INVENTORY:
            {
                int itemCount = AccountGetStoreProductCount( inventoryClient_GetAcctInventorySet(), skuIdFromString(tmp_str), false );
                conPrintf( "Inventory balance for SKU '%s': %d", tmp_str, itemCount );
            }
        xcase CMD_ACCOUNT_QUERY_INVENTORY_ACTIVE:
            {
                int itemCount = AccountGetStoreProductCount( inventoryClient_GetAcctInventorySet(), skuIdFromString(tmp_str), true );
                conPrintf( "Active inventory balance for SKU '%s': %d", tmp_str, itemCount );
            }
        xcase CMD_QUERY_DEBUG_PUBLISH_PRODUCT:
            {
                int itemCount = accountCatalogIsSkuPublished( skuIdFromString(tmp_str) );
                conPrintf( "Publish status for SKU '%s': %d", tmp_str, itemCount );
            }
        xcase CMD_ACCOUNT_LOYALTY_BUY:
            {            
                extern NetLink db_comm_link;
                char buffer[200];

                if ( ! db_comm_link.connected )
                {
                    Entity *e = playerPtr();
                    LoyaltyRewardNode *node = accountLoyaltyRewardFindNode(tmp_str);
                    if (node != NULL && accountLoyaltyRewardCanBuyNode(inventoryClient_GetAcctInventorySet(), e->pl->account_inventory.accountStatusFlags, e->pl->loyaltyStatus, e->pl->loyaltyPointsUnspent, node))
                    {
                        // Send this off to the mapserver
                        sprintf_s( buffer, ARRAY_SIZE(buffer), "account_loyalty_client_buy %s", tmp_str );
                        cmdParse(buffer);
                    }
                } else {
                    dbLoyaltyRewardBuyItem(tmp_str);
                }
            }
        xcase CMD_SHOW_PURCHASES:
            {
                int i;
                Entity *e = playerPtr();
                AccountInventorySet* invSet = inventoryClient_GetAcctInventorySet();
                char *s = estrTemp();

                estrConcatStaticCharArray(&s, "<table><tr><td><b>Name</b></td><td><b>Purchased</b></td><td><b>Used</b></td><td><b>Expires</b></td><td><b>Product</b></td></tr>");

                for (i = 0; i < eaSize(&invSet->invArr); i++)
                {
                    AccountInventory *inv = invSet->invArr[i];
                    const char *name = accountCatalogGetTitle(inv->sku_id);
                    char expires_str[256];
                    if (inv->expires)
                        timerMakeDateStringFromSecondsSince2000(expires_str, inv->expires);
                    else
                        strcpy(expires_str, "Never");

                    estrConcatf(&s, "<tr><td>%s</td><td>%d</td><td>%d</td><td>%s</td><td>%.8s</td></tr>", name, inv->granted_total, inv->claimed_total, expires_str, inv->sku_id.c);
                }

                estrConcatStaticCharArray(&s, "</table>");
                estrConcatf(&s, "Displayed %d purchases (%s)", eaSize(&invSet->invArr), SAFE_MEMBER2(e, pl, accountInformationAuthoritative) ? "from account server" : "from cache");

                InfoBoxDbg(INFO_DEBUG_INFO, s);
                estrDestroy(&s);
            }
        xcase CMD_TEST_CAN_START_STATIC_MAP:
            {
                int mapid = atoi(tmp_str);
                conPrintf("dbCheckCanStartStaticMap(%d) = %d", mapid, dbCheckCanStartStaticMap(mapid, control_state.notimeout?NO_TIMEOUT:90));
            }
        xcase CMD_SHOW_OPTIONS:
            {
                windows_Show("options");
            }
        xcase CMD_SET_SERVERGROUP:
            {
                if (game_state.numServerGroups >= MAX_SERVER_GROUPS)
                {
                    printf("Too many server groups specified!");
                }
                else
                {
                    game_state.serverGroup[game_state.numServerGroups] = tmp_int;
                    game_state.numServerGroups++;
                }
            }
        xdefault:
#ifndef FINAL
            // @todo -AB: some kind of security here? :05/08/07
            if(INRANGE(cmd->num,ClientAccountCmd_None,ClientAccountCmd_Count) && isDevelopmentMode())
            {
                extern NetLink db_comm_link;
                if(!db_comm_link.connected)
                    conPrintf("dbserver not connected.");
                else
                    AccountServer_ClientCommand(cmd,0,auth_info.uid,0,str);
            }
#endif
            break;
    }

    return 1;
}

void cmdSetTimeStamp(int timeStamp)
{
    staticCmdTimeStamp = timeStamp;
}

int cmdGetTimeStamp()
{
    return staticCmdTimeStamp;
}


// Any commands available via some keyboard binding can be invoked directly
// by passing in a string to this function.  Profile trickle rules apply.
// Returns 1 if all commands from the string were parsed successfully.
int cmdParse(char * str)
{
    int x, y;
    inpMousePos(&x, &y);
    return cmdParseEx( str, x, y );
}

void cmdParsef(char const *fmt, ...)
{
    char str[1000];
    va_list ap;
    int ret;

    va_start(ap, fmt);
    ret = _vsnprintf(str, ARRAY_SIZE(str)-1, fmt, ap);
    va_end(ap);
    if (ret==-1)
        str[ARRAY_SIZE(str)-1] = '\0';
    cmdParse(str);
}

int cmdParseEx(char *str, int x, int y)
{
    int i;
    int parseResult;
    KeyBindProfile *curProfile;
    char *pch;
    int iRet = 1;

    while(str)
    {
        int iCntQuote=1;

        pch=strstr(str, "$$");
        while(pch!=NULL && (iCntQuote&1))
        {
            char *pchQuote=str;
            iCntQuote=0;

            // Check to see if this is inside a quoted string.
            // This is a little naive; it just counts the quotes up to
            //   this point. If there's an odd number, we're in a quoted
            //   section.
            while((pchQuote=strchr(pchQuote,'"'))!=NULL && pchQuote<pch)
            {
                pchQuote++;
                iCntQuote++;
            }

            // Inside a quote, look for the next one
            if(iCntQuote&1)
            {
                pch=strstr(pch+2, "$$");
            }
        }

        if(pch!=NULL)
        {
            *pch = '\0';
        }

        // "bindlist" has to override everything else, otherwise it won't work without key trickle.

        if(!striucmp(str, "bindlist") && isDevelopmentMode() )
        {
            bindListBinds();
        }
        else if(!striucmp(str, "bind_save"))
        {
            bindListSave(NULL,0);
        }
        else
        {
            for(i = eaSize(&kbProfiles)-1; i >= 0; i--)
            {
                curProfile = kbProfiles[i];
                if (!curProfile->parser)
                {
                    // Something bad happened to our keyprofile! Try to recover
                    curProfile->parser = cmdGameParse;
                }
                parseResult = curProfile->parser(str, x, y);

                iRet = iRet && parseResult;

                // If the command has been handled, break...
                // Or, if the command cannot be trickle down, break...
                if (parseResult || !curProfile->trickleKeys)
                    break;
            }
        }

        if(pch!=NULL)
        {
            *pch = '$'; // I have to go wash my hands now.
            pch += 2;
        }

        // Go to the next command
        str = pch;
    }

    return iRet;
}

void gameStateSave()
{
    FILE    *file;
    int        i;
    char    buf[1000];

    if( isProductionMode() )
    {
        return;
    }
    else
        file = fileOpen( "c:/config.txt", "wt" );

    if (!file)
        return;
    for(i=0;game_cmds[i].name;i++)
    {
        if (game_cmds[i].num_args  < 1 || game_cmds[i].num == CMD_BIND || stricmp(game_cmds[i].name, "accessviolation")==0)
            continue;
        cmdOldPrint(&game_cmds[i],buf,1000,0,0);
        fprintf(file,"%s\n",buf);
    }
    for(i=0;control_cmds[i].name;i++)
    {
        cmdOldPrint(&control_cmds[i],buf,1000,0,0);
        fprintf(file,"%s\n",buf);
    }
    for(i=0;i<BIND_MAX;i++)
    {
        // only saving binds related to game play.
        if (game_binds_profile.binds[i].command[0])
            fprintf(file,"bind %s \"%s\"\n",
                keyName(i),
                ((KeyBindProfile*)eaGet(&kbProfiles, eaSize(&kbProfiles)-1))->binds[i].command );
    }
    fclose(file);
}

void gameStateInit()
{
    // Some early arguments such as safemode are already set by the time we enter this function,
    // so you cannot blindly zero the entire contents of the game_state variable here.

    cmdOldInit(game_cmds);
    cmdOldGameCheckCmds();

    bindKeyProfile(&game_binds_profile);
    ZeroStruct(&game_binds_profile);
    game_binds_profile.name = "Game Commands";
    game_binds_profile.parser = cmdGameParse;

    game_state.screen_x = 800;// + 8;
    game_state.screen_y = 600;// + 25;
    game_state.fullscreen = 0;
    game_state.fov_1st = 55;
    game_state.fov_3rd = 55;
    game_state.fov_custom = 0;
    // MS: This seems to be approximately the biggest nearz can be without clipping in first-person mode.
    game_state.nearz = 0.73;
    game_state.farz = 20000.f;
    game_state.scene[0] = 0;
    game_state.maxfps = 0;
    game_state.maxMenuFps = 120;
	game_state.maxInactiveFps = 60;
    game_state.port = 0;
    game_state.showhelp = 2;
    game_state.draw_scale = 1;
    game_state.lod_scale = 1;
    game_state.lod_fade_range = 1;
    game_state.normal_scale = 0.5;

    game_state.meshIndex = -1;

    if (strnicmp(getExecutablePatchVersion(NULL),"dev",3)==0)
    game_state.showfps = 1;
    game_state.camdist = 10;
    game_state.minimized = 0;

    // only allow default language (english) when in texWord mode
    if(!game_state.texWordEdit)
        setCurrentLocale(locGetIDInRegistry());

    game_state.mipLevel = game_state.actualMipLevel = game_state.entityMipLevel = 0;
    game_state.reduce_min = 16;
    game_state.ortho_zoom = 1000;

    game_state.shadowvol = 0; //disabled for now
    game_state.LODBias = 1.0;
    game_state.bShowPointers = false;

    game_state.skyFade2 = 1;

    game_state.packet_mtu = 548;

    if(duGetIntDebugSetting("StopInactiveDisplay"))
        game_state.stopInactiveDisplay = 1;
    else
        game_state.stopInactiveDisplay = 0;

    game_state.maxSoundChannels = 32;        // max sounds that can play at once
    game_state.maxSoundSpheres = 16;            // max ambient sounds (that come from sound spheres and sound scripts) playing at once
    game_state.maxPcmCacheMB = 32.f;            // size of pcm cache
    game_state.maxOggToPcm = 40000;            // max size of an ogg file that is allowed to be cached as pcm (any larger is always streamed as ogg from memory)
    game_state.maxSoundCacheMB = 25.f; // max size of all cached sound files (oldest get booted from ram if go over this)

    Strcpy(game_state.address,"127.0.0.1");
    Strcpy(game_state.scene,"scene.txt");

    updateClientFrameTimestamp();

    game_state.fixArmReg = 1;
    game_state.fixLegReg = 1;

    game_state.ctm_autorun_timer = timerAlloc();
    TIMESTEP = 1;

    game_state.suppressCloseFxDist = 3.0f;

    // ab: force cloth lod to 1 (apparently supposed to be this always, anyway)
    game_state.clothLOD = 1;

    // Always try to use FBOs
    //if (IsUsingCider())
    game_state.useFBOs = 1;
    
    game_state.useCg = 1;
    if (game_state.safemode)
    {
        // Disable shader cache in safe mode
        game_state.shaderCache = 0;
    }
    else
    {
        game_state.shaderCache = 1;
    }

    game_state.shaderInitLogging = 0;
    game_state.shaderOptimizationOverride = -1;    // -1 means, "no override"

    game_state.waterRefractionSkewNormalScale = 0.13f;
    game_state.waterRefractionSkewDepthScale = 10.0f;
    game_state.waterRefractionSkewMinimum = 0.07f;
    game_state.waterReflectionSkewScale = 0.15f;
    game_state.waterReflectionStrength = 1.0;    // tbd, remove this since essentially does same as fresnel?  -- 0.6f;
    game_state.waterReflectionOverride = 0;
    game_state.waterReflectionNightReduction = 0.25f; 
    game_state.waterFresnelBias = 0.1f;
    game_state.waterFresnelScale = 0.9f;
    game_state.waterFresnelPower = 3.0f;
    game_state.reflectivityOverride = 0;
    game_state.buildingFresnelBias = 0.2f;
    game_state.buildingFresnelScale = 0.7f;
    game_state.buildingFresnelPower = 3.0f;
    game_state.buildingReflectionNightReduction = 0.75f; 
    game_state.skinnedFresnelBias = 0.0f;
    game_state.skinnedFresnelScale = 0.0f;
    game_state.skinnedFresnelPower = 0.0f;
    game_state.planar_reflection_mip_bias = 0.0f;
    game_state.cubemap_reflection_mip_bias = 0.0f;
    game_state.reflectionEnable = 1; // water on by default
    game_state.separate_reflection_viewport = 1;
    game_state.allow_simple_water_reflection = 0;
    game_state.reflection_generic_fbo_size = 512;
    game_state.reflection_planar_mode = kReflectNearby;
    game_state.reflection_planar_lod_scale = 0.0005f;            // only render very nearby geo to get floors, ledges, nearby pillars, etc.
    game_state.reflection_planar_lod_model_override = MAX_LODS;    // control much world detail we render into the reflection, at this time only valid values are 0 for 'no override' and MAX_LODS to use the lowest detail lod.
    game_state.reflection_planar_fade_begin = 20.0f;            // begin fading out reflection detail to fall back on static env map
    game_state.reflection_planar_fade_end    = 40.0f;            // finish fading out reflection detail to fall back on static env map
    game_state.reflection_water_lod_scale = 0.5f;                // used to scale what is rendered into reflection (for settings other than 'ultra')
    game_state.reflection_water_lod_model_override = MAX_LODS;    // control much world detail we render into the reflection, at this time only valid values are 0 for 'no override' and MAX_LODS to use the lowest detail lod.
    game_state.reflection_distortion_strength = 1.0f;
    game_state.reflection_base_strength = 1.0f;
    game_state.reflection_distort_mix= 0.5f;
    game_state.reflection_base_normal_fade_near = 100.0f;
    game_state.reflection_base_normal_fade_range = 40.0f;
    game_state.reflection_cubemap_lod_model_override = MAX_LODS;// control much world detail we render into the reflection, at this time only valid values are 0 for 'no override' and MAX_LODS to use the lowest detail lod.
    game_state.reflection_proj_clip_mode = kReflectProjClip_All;

    game_state.cubemap_flags = kSaveFacesOnGen | kCubemapStaticGen_RenderAlpha | kCubemapStaticGen_RenderParticles;

    game_state.cubemap_size = 256;
    game_state.cubemap_size_gen = 128;
    game_state.cubemap_size_capture = 512;
    game_state.cubemapLodScale_gen = 1.0f;

    game_state.display_cubeface = 0;
    game_state.static_cubemap_terrain = 1;    // use static by default for terrain
    game_state.static_cubemap_chars = 0;    // use dynamic by default for chars
    game_state.reflectNormals = 1;
    game_state.cubemapAttenuationStart = 4.0f;
    game_state.cubemapAttenuationEnd = 8.0f;
    game_state.cubemapAttenuationSkinStart = 4.0f;
    game_state.cubemapAttenuationSkinEnd = 8.0f;
    game_state.forceCubeFace = 0;
    game_state.charsInCubemap = 0;
    game_state.cubemapUpdate4Faces = 0;
    game_state.cubemapUpdate3Faces = 0; // set to 1 once issues worked out

    game_state.profiling_memory = 64;

    game_state.reflection_planar_entity_vis_limit_dist_from_player = 50.0f;
    game_state.reflection_cubemap_entity_vis_limit_dist_from_player = 50.0f;
    game_state.reflection_water_entity_vis_limit_dist_from_player = 75.0f;

    game_state.water_reflection_min_screen_size = 0.05f;
    gfx_state.reflectionCandidateScore = -FLT_MAX; // set initial gfx state so reflections dont attempt to render on first frame...better place for this?
    gfx_state.waterReflectionScore = -FLT_MAX;

    game_state.reflectiondebugname[0] = 0;
    
    game_state.normalmap_test_mode = 0;

    game_state.use_hard_shader_constants = 1;
    game_state.use_prev_shaders = 0;
    game_state.useViewCache = 0;
    game_state.maxColorTrackerVerts = 5000;
    game_state.useHQ = 1;
    game_state.useChipDebug = 0;

    game_state.force_fallback_material = 0;

    game_state.useNewColorPicker = game_state.texWordEdit ? 0 : 1; // fpe disabled when in texWord mode, crashes due to missing server
    game_state.transformToolbar = 1;
    game_state.inactiveDisplay = 1;

    game_state.numServerGroups = 0;
    for (int r = 0; r < MAX_SERVER_GROUPS; r++)
    {
        game_state.serverGroup[r] = 0;
    }

    //****
    // DEBUG/DEVELOPMENT ONLY
#ifndef FINAL
    g_client_debug_state.submeshIndex = -1;
    g_client_debug_state.this_sprite_only = -1;
#endif //!FINAL
}

int client_frame_timestamp;
void updateClientFrameTimestamp()
{
    // client_frame_timestamp exists so that tex.h does not have to include cmdgame.h
    client_frame_timestamp = game_state.client_frame_timestamp = timerCpuTicks();
}


void gameStateLoad()
{
    FILE    *file;
    char    buf[1000];
    Cmd        *cmd;
    CmdContext context = {0};

    //gameStateInit();
    if( isProductionMode() )
    {
        file = fileOpen("config.txt","rt");
    }
    else
        file = fileOpen( "c:/config.txt", "rt" );

    if (!file)
        return;
    while(fgets(buf,sizeof(buf),file))
    {
        context.access_level = cmdAccessLevel();
        cmd = cmdOldRead(&game_cmdlist,buf,&context);
        if (cmd && cmd->num == CMD_BIND)
            bindKey(bind_key_name,bind_command,0);
    }
    fclose(file);
}




// handles results of Bug UI window (/bug only), calls BugReport() with formatted results
void BugReportInternal(const char * msg, int userSubmitted)
{
    char *pch;
    char *pchDesc;
    char *pchEnd;
    int iCat;
    char * report = NULL;

    // "<cat=5> <summary=xxx> <desc=yyy>"
    // xxx and yyy won't have any \r, \n, <, or >s in them.
    if(!(pch=strstr(msg, "<cat="))) return;
    iCat=atoi(pch+5);

    if(!(pch=strstr(msg, "<summary="))) return;
    pch+=strlen("<summary=");
    if(!(pchEnd=strchr(pch, '>'))) return;
    *pchEnd = 0;

    if(!(pchDesc=strstr(pchEnd+1, "<desc="))) return;
    pchDesc+=strlen("<desc=");
    if(!(pchEnd=strchr(pchDesc, '>'))) return;
    *pchEnd = 0;

    estrClear(&report);
    estrConcatf(&report, "Category: %d\nSummary: %s\nDescription: %s\n", iCat, pch, pchDesc);
    BugReport(report, (userSubmitted ? BUG_REPORT_NORMAL : BUG_REPORT_CSR));
    estrDestroy(&report);
}

// If clientLog == 1, bug report is stored in client log file, else it is stored in server log
void BugReport(const char * desc, int mode)
{
    Packet * pak;
    char* mapname = strstri(game_state.world_name, "maps");
    char specBuf[SPEC_BUF_SIZE];
    const char * memStats = memMonitorGetStats(NULL, 80, filterOneMeg);

    {
        rdrGetSystemSpecs( &systemSpecs );
        rdrGetSystemSpecString( &systemSpecs, specBuf );
        //TO DO maybe add rdr caps and gfx settings
    }

    pak = pktCreateEx(&comm_link, CLIENT_BUG);
        pktSendBitsPack(pak, 2, mode);
        pktSendString(pak, desc);
        pktSendString(pak, getExecutablePatchVersion(NULL));
        pktSendString(pak, mapname);
        pktSendString(pak, specBuf);
        pktSendString(pak, memStats);
        pktSendF32(pak, posX(&cam_info));
        pktSendF32(pak, posY(&cam_info));
        pktSendF32(pak, posZ(&cam_info));
        pktSendF32(pak, control_state.pyr.cur[0]);
        pktSendF32(pak, control_state.pyr.cur[1]);
        pktSendF32(pak, control_state.pyr.cur[2]);
        pktSendF32(pak, game_state.camdist);
        pktSendBitsPack(pak, 1, !control_state.first);
    commSendPkt(pak);
}

// Some functions for saving info in jpeg screenshots

static void AddVersionTag(unsigned char **pestr)
{
    estrConcatStaticCharArray(pestr, "\x1c\x02\x00\x00\x02\x00\x02"); // version
}

static void AddIptcTag(unsigned char **pestr, unsigned char tag, char *pch)
{
    short len = strlen(pch);
    if(len>0 && len < 0xffff)
    {
        estrConcatStaticCharArray(pestr, "\x1c\x02");
        estrConcatChar(pestr, tag);
        estrConcatChar(pestr, *(((char*)(&len))+1));
        estrConcatChar(pestr, *(((char*)(&len))+0));
        estrConcatCharString(pestr, pch);
    }
}

static void CreatePhotoshopBlock(unsigned char **pestrDest, unsigned char **pestr)
{
    int len = estrLength(pestr);
    if(len&1)
    {
        estrConcatChar(pestr, 0);
        len++;
    }

    estrConcatStaticCharArray(pestrDest, "Photoshop 3.0\x00");
    estrConcatStaticCharArray(pestrDest, "8BIM\x04\x04\x00\x00");
    estrConcatChar(pestrDest, *(((char*)(&len))+3));
    estrConcatChar(pestrDest, *(((char*)(&len))+2));
    estrConcatChar(pestrDest, *(((char*)(&len))+1));
    estrConcatChar(pestrDest, *(((char*)(&len))+0));
    estrConcatFixedWidth(pestrDest, *pestr, len);
}


static void CreateScreenshotInfo(char **pestr, char *title)
{
    Entity *player = playerPtr();

    if (player)
    {
        char *estrData;
        char buffer1000[1001];

        estrCreate(&estrData);
        AddVersionTag(&estrData); // must be first

        AddIptcTag(&estrData, 25, "City of Heroes");
        AddIptcTag(&estrData, 25, "coh");
        AddIptcTag(&estrData, 25, "cohtagged");
        sprintf_s(SAFESTR(buffer1000), "coh:x=%1.0f\n", ENTPOSX(player));     AddIptcTag(&estrData, 25, buffer1000);
        sprintf_s(SAFESTR(buffer1000), "coh:y=%1.0f\n", ENTPOSY(player));     AddIptcTag(&estrData, 25, buffer1000);
        sprintf_s(SAFESTR(buffer1000), "coh:z=%1.0f\n", ENTPOSZ(player));     AddIptcTag(&estrData, 25, buffer1000);

        {
            char *pos;
            char another1000[1001];

            if(pos = strrchr(game_state.world_name, '/'))
                Strcpy(another1000, pos+1);
            else if(pos = strrchr(game_state.world_name, '\\'))
                Strcpy(another1000, pos+1);
            else
                Strcpy(another1000, game_state.world_name);

            if(pos=strrchr(another1000, '.'))
                *pos = '\0';

            sprintf_s(SAFESTR(buffer1000), "coh:zone=%s\n", another1000);
            AddIptcTag(&estrData, 25, buffer1000);
        }

        msPrintf(menuMessages, SAFESTR(buffer1000), game_state.world_name);
        if(!strchr(buffer1000, '/'))
            AddIptcTag(&estrData, 25, buffer1000);

        if(title)
            AddIptcTag(&estrData, 5, title);
        else
            AddIptcTag(&estrData, 5, timerGetTimeString());

        *buffer1000 = '\0';
        if(player->teamup && player->teamup->members.count>1)
        {
            int i;
            for(i=0; i<player->teamup->members.count; i++)
            {
                strcat(buffer1000, player->teamup->members.names[i]);
                if(i< player->teamup->members.count-1)
                strcat(buffer1000, ", ");
            }
        }
        else
        {
            Strcpy(buffer1000, player->name);
        }
        AddIptcTag(&estrData, 120, buffer1000);

        CreatePhotoshopBlock(pestr, &estrData);

        estrDestroy(&estrData);
    }
}

static void jpeg_screenshot(char *title)
{
    char *estr;
    estrCreate(&estr);

    CreateScreenshotInfo(&estr, title);
    gfxScreenDumpEx("screenshots", 0, estr, estrLength(&estr), "jpg");

    estrDestroy(&estr);
}

// earray of server commands
StringArray serverCommands;
int serverCommandsInitialized = 0;

void cmdAddServerCommand(char * scmd)
{
    if ( !serverCommandsInitialized )
    {
        eaCreate(&serverCommands);
        serverCommandsInitialized = 1;
    }
    if (cmdIsServerCommand(scmd)) //don't leak
        return;
    eaPush(&serverCommands,strdup(scmd));
}

int cmdIsServerCommand(char *str)
{
    int i = 0, num = eaSize(&serverCommands);

    for (;i < num; ++i)
    {
        if ( !stricmp(str, serverCommands[i]) )
            return 1;
    }

    return 0;
}

int cmdCompleteComand(char *str, char *out, int searchID, int searchBackwards)
{
     static int num_cmds = sizeof(game_cmds)/sizeof(Cmd) - 1,
         num_client_cmds = 0,
         num_control_cmds = 0,
         num_svr_cmds = 0;

     int i,
        cmd_offset = 0,
        server_cmd_offset = 0,
        client_cmd_offset = 0,
        control_cmd_offset = 0,
        total_cmds = 0;

    //static int server_searchID = 0;
    char searchStr[256] = {0}, foundStr[256] = {0};
    MRUList *consolemru = conGetMRUList();
    int num_mru_commands = 0;
    int effi;
    char uniquemrucommands[32][1024];

    if ( !str || str[0] == 0 )
    {
        out[0] = 0;
        return 0;
    }

    for (i=consolemru->count-1; i>=0; i--)
    {
        char cmd[1024];
        bool good = true;
        int j;
        Strncpyt(cmd, consolemru->values[i]);
        if (strchr(cmd, ' '))
            *strchr(cmd, ' ')=0;
        for (j=0; j<num_mru_commands; j++) {
            if (stricmp(uniquemrucommands[j], cmd)==0) {
                good = false;
            }
        }
        if (good) {
            Strcpy(uniquemrucommands[num_mru_commands], cmd);
            num_mru_commands++;
        }
    }

    if ( !num_svr_cmds )
        num_svr_cmds = eaSize(&serverCommands);

    if ( !num_client_cmds )
    {
        if ( client_control_cmds )
        {
            i = 0;

            for (;;++i )
            {
                if ( client_control_cmds[i].name )
                    continue;
                break;
            }
        }
            num_client_cmds = i;
    }

    if ( !num_control_cmds )
    {
        if ( control_cmds )
        {
            i = 0;
            for (;;++i )
            {
                if ( control_cmds[i].name )
                    continue;
                break;
            }
        }
        num_control_cmds = i;
    }

    // figure out command offsets
    cmd_offset = num_mru_commands,
    server_cmd_offset = cmd_offset + num_cmds,
    client_cmd_offset = server_cmd_offset + num_svr_cmds,
    control_cmd_offset = client_cmd_offset + num_client_cmds,
    total_cmds = control_cmd_offset + num_control_cmds;


        Strcpy( searchStr, stripUnderscores(str) );

        i = searchBackwards ? searchID - 1 : searchID + 1;

        //for ( ; i < num_cmds; ++i )
        while ( i != searchID )
        {
            if ( i >=1 && i < cmd_offset + 1) {
                effi = i - 1;
                Strcpy( foundStr, uniquemrucommands[effi]);
                if ( strStartsWith(foundStr, searchStr) )
                {
                    strcpy(out, foundStr);
                    return i;
                }
            }
            else if ( i >= cmd_offset + 1 && i < server_cmd_offset + 1 )
            {
                effi = i - cmd_offset - 1;
                Strcpy( foundStr, stripUnderscores(game_cmds[effi].name) );

                if ( !(game_cmds[effi].flags & CMDF_HIDEPRINT) &&
                    game_cmds[effi].access_level <= cmdAccessLevel() && strStartsWith(foundStr, searchStr) )
                {
                    //strcpy(out, game_cmds[i].name);
                    strcpy(out, foundStr);
                    return i;
                }
            }
            else if ( i >= server_cmd_offset + 1 && i < client_cmd_offset + 1 )
            {
                effi = i - server_cmd_offset - 1;
                Strcpy( foundStr, stripUnderscores(serverCommands[effi]) );

                if ( strStartsWith(foundStr, searchStr) )
                {
                    //strcpy(out, serverCommands[i - num_cmds]);
                    strcpy(out, foundStr);
                    return i;
                }
            }
            else if ( i >= client_cmd_offset + 1 && i < control_cmd_offset + 1 )
            {
                effi = i - client_cmd_offset - 1;
                Strcpy( foundStr, stripUnderscores(client_control_cmds[effi].name) );

                if ( !(client_control_cmds[effi].flags & CMDF_HIDEPRINT) &&
                    client_control_cmds[effi].access_level <= cmdAccessLevel() &&
                    strStartsWith(foundStr, searchStr) )
                {
                    //strcpy(out, game_cmds[i].name);
                    strcpy(out, foundStr);
                    return i;
                }
            }
            else if ( i >= control_cmd_offset + 1 && i < total_cmds + 1 )
            {
                effi = i - control_cmd_offset - 1;
                Strcpy( foundStr, stripUnderscores(control_cmds[effi].name) );

                if ( !(control_cmds[effi].flags & CMDF_HIDEPRINT) &&
                    control_cmds[effi].access_level <= cmdAccessLevel() &&
                    strStartsWith(foundStr, searchStr) )
                {
                    //strcpy(out, game_cmds[i].name);
                    strcpy(out, foundStr);
                    return i;
                }
            }

            searchBackwards ? --i : ++i;

            if ( i >= total_cmds + 1 ) i = 0;
            else if ( i < 0 ) i = total_cmds;
        }

    if ( searchID && searchID != -1 )
    {
        if ( searchID < num_mru_commands + 1 && searchID - 1 > 0 )
        {
            int idx = searchID - 1;
            strcpy(out, idx >= 0 ? uniquemrucommands[idx] : "");
        }
        if ( searchID < num_mru_commands + num_cmds + 1 )
        {
            int idx = searchID - num_mru_commands - 1;
            strcpy(out, idx >= 0 ? game_cmds[idx].name : "");
        }
        else if ( searchID < num_mru_commands + num_cmds + num_svr_cmds + 1 )
        {
            int idx = searchID - num_mru_commands - num_cmds - 1;
            strcpy(out, idx >= 0 ? serverCommands[idx] : "");
        }
        else
            out[0] = 0;
    }
    else
        out[0] = 0;
    return 0;
}

int cfg_IsBetaShard(void)
{
    return game_state.isBetaShard;
}

int cfg_IsVIPShard(void)
{
    return game_state.isVIPShard;
}
int cfg_NagGoingRoguePurchase(void)
{
    return game_state.GRNagAndPurchase;
}

void cfg_setIsBetaShard(int data)
{
    game_state.isBetaShard = data;
}

void cfg_setIsVIPShard(int data)
{
    game_state.isVIPShard = data;
}


void cfg_setNagGoingRoguePurchase(int data)
{
    game_state.GRNagAndPurchase = data;
}
