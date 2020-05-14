#include <stdio.h>
#include "cmdparse/AppServerCmd.h"
#include "cmdparse/cmdauction.h"
#include "cmdparse/cmdaccountserver.h"
#include "gameComm/traycommon.h"
#include "auction/AuctionClient.h"
#include "bases/basestorage.h"
#include "bases/baseupkeep.h"
#include "cmdparse/cmdstatserver.h"
#include "bases/DetailRecipe.h"
#include "entity/SuperGroup.h"
#include "player/BadgeStats.h"
#include "container/mapgroup.h"
#include "entity/SgrpStats.h"
#include "entity/concept.h"
#include <string.h>
#include <time.h>
#include <utilitieslib/assert/assert.h>
#include <utilitieslib/utils/file.h>
#include <utilitieslib/stdtypes.h>
#include <utilitieslib/utils/cmdoldparse.h>
#include "cmdparse/cmdserver.h"
#include <utilitieslib/network/netio.h>
#include "cmdparse/cmdcommon.h"
#include "svr/svr_base.h"
#include <utilitieslib/network/netio.h>
#include "entity/entity.h"
#include "entity/entplayer.h"
#include "generator/entgenLoader.h"
#include "missionMapInit.h"
#include "beacon/beacon.h"
#include "beacon/beaconConnection.h"
#include "beacon/beaconDebug.h"
#include "beacon/beaconFile.h"
#include "beacon/beaconPath.h"
#include "beacon/beaconClientServerPrivate.h"
#include "gameComm/svr_chat.h"
#include <utilitieslib/utils/utils.h>
#include "gameComm/sendToClient.h"
#include "entity/entserver.h"
#include "dbcomm/dbcomm.h"
#include "dbcomm/dbcontainer.h"
#include "dbcomm/dbquery.h"
#include "entcon.h"
#include "comm_game.h"
#include "entity/entVarUpdate.h"
#include "varutils.h"
#include <utilitieslib/language/AppLocale.h>
#include "player/clientEntityLink.h"
#include "cmdparse/cmdserverdebug.h"
#include "cmdparse/cmdservercsr.h"
#include "generator/encounterprivate.h"
#include "ai/entai.h"
#include "entity/origins.h"
#include "entity/character_base.h"
#include "entity/character_inventory.h"
#include "entity/character_level.h"
#include "entity/character_combat.h"
#include "entity/powers.h"
#include "group/groupnetdb.h"
#include "group/groupdynsend.h"
#include "gameComm/NpcServer.h"
#include "entity/load_def.h"
#include <utilitieslib/components/earray.h>
#include "container/team.h"
#include "dbcomm/dbmapxfer.h"
#include <utilitieslib/utils/MemoryMonitor.h>
#include "Reward.h"
#include "cmdparse/cmdenum.h"
#include "cmdchat.h"
#include "svr/svr_player.h"
#include <utilitieslib/components/StringCache.h>
#include "dbcomm/dbDoor.h"
#include "container/containerEmail.h"
#include "storyarc/storyarcInterface.h"
#include "storyarc/pnpc.h"
#include "entity/entGameActions.h"
#include "door.h"
#include <utilitieslib/utils/error.h>
#include "language/langServerUtil.h"
#include "language/commonLangUtil.h"
#include "entity/powerinfo.h"
#include <utilitieslib/components/SharedMemory.h>
#include "entity/costume.h"
#include "entity/entPlayer.h"
#include "entity/pvillain.h"
#include "gamesys/dooranim.h"
#include "storyarc/storyarcprivate.h"
#include "storyarc/task.h"
#include <utilitieslib/utils/strings_opt.h>
#include "dbcomm/staticmapinfo.h"
#include "kiosk.h"
#include <utilitieslib/components/EString.h>
#include "storyarc/TeamTask.h"
#include "filter/profanity.h"
#include "gridcoll/gridcollperftest.h"
#include <utilitieslib/utils/memlog.h>
#include "TeamReward.h"
#include "svr/svr_tick.h"
#include "svr/gloophook.h"
#include "filter/validate_name.h"
#include "entity/motion.h"
#include "script/scriptengine.h"
#include "script/scriptdebug.h"
#include "dbcomm/shardcomm.h"
#include "dbcomm/shardcommtest.h"
#include "player/badges_server.h"
#include "container/containerloadsave.h"
#include "script/ScriptHook/ScriptHookInternal.h"
#include "script/ZoneEvents/ScriptedZoneEvent.h"
#include "script/ZoneEvents/HalloweenEvent.h"
#include "gameSys/arenamapserver.h"
#include "gameSys/arenamap.h"
#include "gameSys/arenakiosk.h"
#include "gameSys/raidmapserver.h"
#include "entity/netfx.h"
#include "entity/buddy_server.h"
#include "ai/entaiprivate.h"
#include "chatdb.h"
#include "dbcomm/dbnamecache.h"
#include "entity/salvage.h"
#include "entity/Proficiency.h"
#include "entity/concept.h"
#include "dbgHelper.h"
#include <utilitieslib/utils/eval.h>
#include "bases/bases.h"
#include "bases/baseparse.h"
#include "bases/basetogroup.h"
#include "bases/baselegal.h"
#include "bases/baseserverrecv.h"
#include "group/groupjournal.h"
#include "group/group.h"
#include "gameSys/sgraid.h"
#include "gridcoll/gridcache.h"
#include "container/containerInventory.h"
#include "gameSys/petarena.h"
#include "NovodeX/NwWrapper.h"
#include "NovodeX/NwRagdoll.h"
#include "seq/seqskeleton.h"
#include "gameSys/raidmapserver.h"
#include "seq/seqanimate.h"
#include "group/groupnovodex.h"
#include "entity/SgrpServer.h"
#include "gameComm/pet.h"
#include "entity/character_pet.h"
#include "entity/SgrpBadges.h"
#include "bases/baseserver.h"
#include "bases/basedata.h"
#include "MissionControl.h"
#include <utilitieslib/components/StringTable.h>
#include "gameComm/VillainDef.h"
#include "dbcomm/dbbackup.h"
#include "comm_backend.h"
#include "entity/boostset.h"
#include "auth/authUserData.h"
#include "entity/character_eval.h"
#include "entity/character_combat_eval.h"
#include "auction/AuctionData.h"
#include <utilitieslib/network/net_packetutil.h>
#include "storyarc/taskforce.h"
#include "entity/attrib_description.h"
#include "account/AccountInventory.h"
#include <utilitieslib/components/HashFunctions.h>
#include <utilitieslib/language/MessageStoreUtil.h>
#include "account/AccountCatalog.h"
#include "account/AccountData.h"
#include "storyarc/missionspec.h"
#include "entity/pvp.h"
#include "storyarc/playerCreatedStoryArc.h"
#include "storyarc/playerCreatedStoryarcServer.h"
#include "player/pmotion.h"
#include "storyarc/storyarcutil.h"
#include "entity/incarnate_server.h"
#include "gameSys/turnstile.h"
#include "group/groupdbmodify.h"
#include "dbcomm/logcomm.h"
#include <utilitieslib/utils/log.h>
#include <utilitieslib/UtilsNew/meta.h>
#include <utilitieslib/UtilsNew/Array.h>
#include "mission/missionserver_meta.h"
#include "entity/mapHistory.h"
#include "storyarc/contactdef.h"
#include "storyarc/contact.h"
#include "entity/character_karma.h"
#include "entity/power_customization.h"
#include <utilitieslib/utils/process_util.h>
#include "entity/alignment_shift.h"
#include "entity/character_tick.h"
#include "filter/titles.h"
#include "container/eventHistory.h"
#include "cmdparse/autoCommands.h"
#include "storyarc/pnpcCommon.h"
#include "LWC_common.h"
#include "gameComm/NewFeatures.h"
#include "utilitieslib/utils/timing.h"
#include <utilitieslib/network/crypt.h>
#include "group/groupfileload.h"

ServerState server_state;

static int status_server_id;
static int list_id,container_id,map_id;
static Vec3 setpos, setpyr;
static char group_msg[100];
static char group_name[128], player_name[128];
static char afk[2000];

static int tmp_int,tmp_int2,tmp_int3,tmp_int4,tmp_int5,tmp_int6,tmp_int7;
static F32 tmp_float;
static F32 tmp_f32s[4];

static Vec3 tmp_vec, tmp_pos;
static char tmp_str[20000], tmp_str2[20000], tmp_str3[20000];

static void *tmp_var_list[] = { // List of tmp_* vars in order to automatically add CMDF_HIDEPRINT
    &tmp_int,&tmp_int2,&tmp_int3,&tmp_int4,&tmp_int5,&tmp_int6,&tmp_int7,
    &tmp_float,
    &tmp_vec, &tmp_pos,
    &tmp_str, &tmp_str2, &tmp_str3,
};

static Vec3 beaconPathSrc, beaconPathDst;
static int entity_id;
static int invincible_state, unstoppable_state, untargetable_state, alwayshit_state;

static void serverExecCmd(Cmd *cmd, ClientLink *client, char *source_str, Entity *e, CmdContext *output);

#define MAX_BUG_MSG_SIZE  2000   // max length of message obtained from "sbug" command

extern StaticDefine ParseIncarnateDefines[]; // defined in load_def.c

// 0 - User commands
// 1 - View only CSR commands (cannot modify a player, except to move their position)
// 2 - Gets a few admin type commands ( kick, ban, silence, rename )
// 3 - Can modify a player, run zone events
// 4 - Can give rewards to players
// 9 - Debug level commands, only NCsoft personel should use
// 10 - Internal commands, issued by the mapserver

Cmd server_cmds[] =
{
    { 9, "entsave",        SCMD_ENTSAVE,{{0}},0,
                        "save players, write mapname.txt, flush dbserver cache" },
    { 4, "ent_corrupted",SCMD_ENTCORRUPTED, {{0}}, 0,
                        "Shows if the entity if marked as corrupted." },
    { 4, "ent_set_corrupted",SCMD_SETENTCORRUPTED, {{ CMDINT(tmp_int) }}, 0,
                        "Sets the corrupted flag on an entity." },
    { 9, "ent_delete",    SCMD_ENTDELETE, {{CMDINT(tmp_int)},{CMDSTR(tmp_str)}}, 0,
                        "Deletes entity <entid> <charactername>" },
    { 9, "entgenload",    SCMD_ENTGENLOAD,{{0}},0,
                        "blarg" },
    //{ 9, "beaconprocess",SCMD_BEACONPROCESS,{{0}},0,
    //                    "Finds connections between combat beacons" },
    //{ 9, "beaconprocessforced",SCMD_BEACONPROCESSFORCED,{{0}},0,
    //                    "Finds connections between combat beacons, doesn't check if file is up to date" },
    { 9, "beaconprocesstraffic",SCMD_BEACONPROCESSTRAFFIC,{{0}},0,
                        "Finds and connects traffic beacons." },
    { 9, "beaconprocessnpc",SCMD_BEACONPROCESSNPC,{{0}},0,
                        "Finds and connects NPC beacons." },
    //{ 9, "beacongenerate",SCMD_BEACONGENERATE,{{0}},0,
    //                    "Makes beacons from geometry." },
    { 9, "beaconreadfile",SCMD_BEACONREADFILE,{{0}},0,
                        "Reloads the beacon file." },
    { 9, "beaconreadthisfile",SCMD_BEACONREADTHISFILE,{{ CMDSTR(tmp_str) }},0,
                        "Load a specific beacon file." },
    { 9, "beaconwritefile",SCMD_BEACONWRITEFILE,{{0}},0,
                        "Writes out the beacon file." },
    { 9, "beaconfix",    SCMD_BEACONFIX,{{CMDINT(tmp_int)}}, CMDF_HIDEVARS,
                        "This fixes beacons that have had their wrapper group removed and various other problems.  Runs automatically at load time" },
    { 9, "beaconpathdebug",    SCMD_BEACONPATHDEBUG,{{ CMDINT(tmp_int) }},0,
                        "Pathfinding debugging stuff." },
    { 9, "beaconresettemp",    SCMD_BEACONRESETTEMP,{{ 0 }},0,
                        "Reset temporary beacon information (pathfinding optimization)" },
    { 9, "beacongotocluster", SCMD_BEACONGOTOCLUSTER, {{ CMDINT(tmp_int) }}, 0,
                        "Goto a beacon cluster by index." },
    { 9, "beacongetvar", SCMD_BEACONGETVAR, {{ CMDSTR(tmp_str) }}, CMDF_HIDEVARS,
                        "Get a beacon debug var." },
    { 9, "beaconsetvar", SCMD_BEACONSETVAR, {{ CMDSTR(tmp_str) }, { CMDINT(tmp_int) }}, CMDF_HIDEVARS,
                        "Set a beacon debug var." },
    { 9, "beaconcheckfile", SCMD_BEACONCHECKFILE, {{ 0 }}, 0,
                        "Tells whether or not the beacon file is up to date for this map." },
    { 9, "beaconrequest", SCMD_BEACONREQUEST, {{ 0 }}, 0,
                        "Requests a beacon file for the current map." },
    { 9, "beaconmasterserver", SCMD_BEACONMASTERSERVER, {{ CMDSTR(tmp_str) }}, CMDF_HIDEVARS,
                        "Set the BeaconMasterServer to use for beacon requests." },
    { 9, "testnpcdta",    SCMD_TESTNPCDTA, {{ CMDSENTENCE(server_state.entcon_msg)}},0,
                        "Test a guy in NPC.dta." },
    { 9, "testnpcs",    SCMD_TESTNPCS,{{CMDINT(tmp_int)}, {CMDINT(tmp_int2)}},0,
                        "testnpcs <initial npc number> <spawn count>\n"
                        "Test npcs + villains from NPC.dta\n"},
    { 9, "testgentypes",SCMD_TESTGENTYPES,{{0}},0,
                        "Verify all referenced NPCs and Critters in the loaded SpawnArea defs are valid\n"},
    { 9, "printent",    SCMD_PRINT_ENT, {{ CMDSENTENCE(server_state.entcon_msg)}},0,
                        "blarg" },
    { 9, "netsteps",    0, {{ CMDINT(server_state.netsteps)}},0,
                        "set network update divisor. default is to send one net update every 8 server ticks. set to 1 to get 30 net updates/sec" },
    { 9, "snotimeout",    0, {{ CMDINT(server_state.notimeout)}},0,
                        "sets the notimeout flag on all links initially" },
    { 9, "serverbreak",    SCMD_SERVER_BREAK,{{0}},0,
                        "blarg" },
    { 9, "server_error_sent_count", 0, {{ CMDINT(server_error_sent_count) }}, 0,
                        "the count of errors sent to admin clients." },
    { 9, "smalloc",        SCMD_SMALLOC,{{0}},0,
                        "count how much memory the server has malloc'd and print to a log file" },
    { 9, "heapinfo",    SCMD_HEAPINFO,{{0}},0,
                        "dump some heap info to the server console" },
    { 9, "heaplog",        SCMD_HEAPLOG,{{0}},0,
                        "dump some heap info to log files" },
    { 9, "returnall",    SCMD_RETURNALL,{{0}},0,
                        "if this is a mission map, return all players to static map and exit" },
    { 9, "sendmsg",        SCMD_SENDMSG, {{ CMDINT(list_id) },{ CMDINT(container_id) },{ CMDSENTENCE(group_msg)}},0,
                        "test send message blah blah" },
    { 9, "crashnow",    SCMD_CRASHNOW,{{0}},0,
                        "blarg" },
    { 9, "fatalerror",    SCMD_FATAL_ERROR_NOW,{{0}},0,
                        "Call FatalErrorf(), for testing purposes." },
    { 9, "sendpacket",    SCMD_SENDPACKET,{{ CMDINT(tmp_int) }},0,
                        "Sends a packet of X KB to the client" },
    { 9, "showstatesvr",SCMD_SHOWSTATE,  {{ CMDSTR(server_state.entity_to_show_state) }},0,
                        "blarg" },
    { 4, "youSay",        SCMD_YOUSAY, {{ CMDSENTENCE( tmp_str )}}, CMDF_HIDEVARS | CMDF_RETURNONERROR,
                        "set selected entity as debug entity on server, and have it say the given string" },
    { 4, "youSayOnClick",SCMD_YOUSAYONCLICK, {{ CMDSENTENCE( tmp_str )}}, CMDF_HIDEVARS | CMDF_RETURNONERROR,
                        "set selected entity as debug entity on server, and have it say the given string when clicked" },
    { 9, "checkjfd",    0,  {{ CMDINT(server_state.check_jfd) }},0,
                        "blarg" },
    { 9, "quickLoadAnims",  0, {{ CMDINT(server_state.quickLoadAnims) }},0,
                        "dev mode don't preload anims" },
    { 9, "netfxdebugsvr",    0,  {{ CMDINT(server_state.netfxdebug) }},0,
                        "blarg" },
    { 9, "showHiddenEntities",    0,  {{ CMDINT(server_state.showHiddenEntities) }},0,
                        "blarg" },
    { 9, "reloadSeqsSvr",    0,  {{ CMDINT(server_state.reloadSequencers) }},0,
                        "development mode only reload sequencers and animations" },
    { 9, "recordcoll",  0,  {{ CMDINT(server_state.record_coll) }},0,
                        "blarg" },
    { 9, "reloadpriority",    SCMD_RELOADPRIORITY,    {{0}},0,
                        "Reloads all priority list files." },
    { 9, "reloadaiconfig",    SCMD_RELOADAICONFIG,    {{0}},0,
                        "Reloads all AI config files." },
    { 2, "getentdebugmenu",    SCMD_GETENTDEBUGMENU,    {{0}},0,
                        "Gets the entity debug menu." },
    { 2, "mmm",            SCMD_GETENTDEBUGMENU,    {{0}},0,
                        "Gets the entity debug menu, aka the Magic Martin Menu." },
    { 9, "initmap",        SCMD_INITMAP,{{0}},0,
                        "Reinitialize the map" },
    { 9, "playercount",    0,                 {{ CMDINT(server_state.playerCount)}},0,
                        "for testing entity generator code" },
    { 9, "clearailog",    SCMD_CLEARAI_LOG, {{0}}, 0,
                        "Clears the AI logs for all entities." },
    { 9, "entdebuginfo",SCMD_ENTDEBUGINFO, {{ CMDINT(tmp_int) }}, 0,
                        "bit flags for debug info display" },
    { 0, "runnerdebug",SCMD_RUNNERDEBUG, {{ CMDINT(tmp_int) }}, 0,
						"Enable limited debugging for a possible critter run-away bug" },
    { 9, "setdebugvar", SCMD_SETDEBUGVAR, {{ CMDSTR(tmp_str) }, { CMDINT(tmp_int) }}, CMDF_HIDEVARS,
                        "Set a debug var." },
    { 9, "getdebugvar", SCMD_GETDEBUGVAR, {{ CMDSTR(tmp_str) }}, CMDF_HIDEVARS,
                        "Get a debug var." },
    { 9, "sendautotimers",SCMD_SENDAUTOTIMERS, {{ CMDINT(tmp_int) }}, 0,
                        "enable sending auto timers to your client" },
    { 9, "setdebugentitysvr",SCMD_SETDEBUGENTITY, {{ CMDINT(tmp_int) }}, 0,
                        "set the entity id that various things use as a server debug entity" },
    { 2, "debugmenuflags",SCMD_DEBUGMENUFLAGS, {{ CMDINT(tmp_int) }}, 0,
                        "bit flags for what options to show on debug menu." },
    { 9, "interpdatalevel",SCMD_INTERPDATALEVEL, {{ CMDINT(tmp_int) }}, 0,
                        "level of extra interpolation data to send, higher is better." },
    { 9, "interpdataprecision",SCMD_INTERPDATAPRECISION, {{ CMDINT(tmp_int) }}, 0,
                        "number of bits of precision in interp data (4-8)." },
    { 9, "ailog",        0, {{ CMDINT(server_state.aiLogFlags) }},CMDF_APPLYOPERATION,
                        "Sets the types of AI logging that is done." },
    { 9, "noai",        0, {{ CMDINT(server_state.noAI) }},0,
                        "Disables AI, but not physics." },
    { 9, "noprocess",    0, {{ CMDINT(server_state.noProcess) }},0,
                        "Disables processing, but not AI." },
    { 9, "nokillcars",    0, {{ CMDINT(server_state.noKillCars) }},0,
                        "Disables killing of cars that have no beacons" },
    { 9, "skyFade1",    SCMD_SETSKYFADE, {{ CMDINT(server_state.skyFade1) }},0,
                        "manually override sky fading values" },
    { 9, "skyFade2",    SCMD_SETSKYFADE, {{ CMDINT(server_state.skyFade2) }},0,
                        "manually override sky fading values" },
    { 9, "skyFadeWeight", SCMD_SETSKYFADE, {{ PARSETYPE_FLOAT, &server_state.skyFadeWeight }},0,
                        "manually override sky fading values" },
    { 9, "cutSceneDebug",    0, {{ CMDINT(server_state.cutSceneDebug) }},0,
                        "Prints debug info about running cutscenes." },
    { 9, "doNotCompleteMission",    0, {{ CMDINT(server_state.doNotCompleteMission) }},0,
                        "Do not let this mission be completed -- because it can be a pain to get there." },
    { 9, "resetperfinfo_server",    0, {{ CMDINT(timing_state.resetperfinfo_init) }},0,
                        "Resets all performance info." },
    { 9, "runperfinfo_server",    0, {{ CMDINT(timing_state.runperfinfo_init) }},0,
                        "Runs performance info for this many ticks.  Set to -1 to run forever." },
    { 9, "perfinfomaxstack_server",    0, {{ CMDINT(timing_state.autoStackMaxDepth_init) }},0,
                        "Sets the depth of the performance monitor stack." },
    { 9, "perfinfosetbreak_server",    SCMD_PERFINFOSETBREAK, {{ CMDINT(tmp_int) }, { CMDINT(tmp_int2) }, { CMDINT(tmp_int3) } },0,
                        "Set a breakpoint on a particular auto timer." },
    { 9, "perfinfo_time_all_ents",    0, {{ CMDINT(server_state.time_all_ents) }},0,
                        "Enables timing all entities individually." },
    { 9, "perfinfo_enable_ent_timer",    SCMD_PERFINFO_ENABLE_ENT_TIMER, {{ CMDINT(tmp_int) }},0,
                        "Enables timing an entity." },
    { 9, "perfinfo_disable_ent_timers",    SCMD_PERFINFO_DISABLE_ENT_TIMERS, {{ 0 }},0,
                        "Disable all entity timers." },
    { 9, "clientpacketlog", SCMD_PERFINFO_CLIENT_PACKET_LOG, {{ CMDINT(tmp_int) }, { CMDINT(tmp_int2) }}, 0,
                        "Enable logging client packets." },
    { 9, "clientpacketlogpause", SCMD_PERFINFO_CLIENT_PACKET_LOG_PAUSE, {{ CMDINT(tmp_int) }}, 0,
                        "Toggle pausing client packet log." },
    { 9, "processtimes",SCMD_PROCESSTIMES, {{0}}, 0,
                        "Report current uptime, kernel time and user time in seconds." },
    { 9, "memorysizes",    SCMD_MEMORYSIZES, {{0}}, 0,
                        "Report current and peak working set and page file sizes in bytes." },
    { 9, "nofilechangecheck_server",  0, {{ CMDINT(global_state.no_file_change_check) }},0,
                        "enables dynamic checking for file changes" },
    { 1, "moveentitytome",    SCMD_MOVE_ENTITY_TO_ME, {{ 0 }},0,
                        "moves selected entity to my location" },
    { 9, "entcontrol",    SCMD_ENTCONTROL, {{ CMDSENTENCE(server_state.entcon_msg)}},0,
                        "'entcontrol 0 kill' to kill currently selected npc" },
    { 3, "ec",            SCMD_ENTCONTROL, {{ CMDSENTENCE(server_state.entcon_msg)}},0,
                        "ec is abbreviation for entcontrol" },
    { 1, "setpos",        SCMD_SET_POS,        {{ PARSETYPE_FLOAT, &setpos[0] }, { PARSETYPE_FLOAT, &setpos[1] }, { PARSETYPE_FLOAT, &setpos[2] } },CMDF_HIDEVARS,
                        "move me to <x> <y> <z> on this map" },
    { 1, "setpospyr",    SCMD_SET_POS_PYR,    {    { PARSETYPE_FLOAT, &setpos[0] }, { PARSETYPE_FLOAT, &setpos[1] }, { PARSETYPE_FLOAT, &setpos[2] },
                                                { PARSETYPE_FLOAT, &setpyr[0] }, { PARSETYPE_FLOAT, &setpyr[1] }, { PARSETYPE_FLOAT, &setpyr[2] } },CMDF_HIDEVARS,
                        "move me to <x> <y> <z> on this map, set pyr" },
    { 1, "mapmove",        SCMD_MAPMOVE,        {{ CMDINT(map_id) }},0,
                        "move to <map id>. see maps.db for map ids" },
    { 4, "mapmovespawn",SCMD_MAPMOVESPAWN,{{CMDINT(map_id)},{CMDSTR(tmp_str)}},CMDF_HIDEVARS,
                        "move to <mad id> and spawn marker <spawn_name>" },
    { 9, "shardjump",    SCMD_SHARDJUMP,        {{ CMDSTR(tmp_str) }}, 0,
                        "move to shard <shard name>." },
    { 9, "shardxfer_init", SCMD_SHARDXFER_INIT, {{ CMDSTR(tmp_str) }, { CMDINT(tmp_int) }, { CMDINT(tmp_int2) }, { CMDSTR(tmp_str2) }}, CMDF_HIDEVARS|CMDF_HIDEPRINT,
                        "from accountserver" }, // this should probably be moved to cmdaccountserver.c
    { 9, "shardxfer_complete", SCMD_SHARDXFER_COMPLETE, {{ CMDINT(tmp_int) }, { CMDINT(tmp_int2) }, { CMDINT(tmp_int3) }, { CMDINT(tmp_int4) }}, CMDF_HIDEVARS|CMDF_HIDEPRINT,
                        "from accountserver" }, // Only used for visitor transfers, but works hand in hand with the above
    { 9, "shardxfer_jump", SCMD_SHARDXFER_JUMP, {{ 0 }}, CMDF_HIDEVARS|CMDF_HIDEPRINT,
                        "from accountserver" }, // Only used for visitor transfers, but works hand in hand with the above
    { 1, "nextplayer",    SCMD_NEXTPLAYER,{{0}},0,
                        "go to next player" },
    { 1, "nextnpc",        SCMD_NEXTNPC,{{0}},0,
                        "go to next npc" },
    { 1, "nextcritter",    SCMD_NEXTCRITTER,{{0}},0,
                        "go to next critter" },
    { 1, "nextawakecritter",    SCMD_NEXTAWAKECRITTER,{{0}},0,
                        "go to next critter" },
    { 1, "nextcar",        SCMD_NEXTCAR,{{0}},0,
                        "go to next car" },
    { 1, "nextitem",    SCMD_NEXTITEM,{{0}},0,
                        "go to next item" },
    { 1, "nextreset",    SCMD_NEXTRESET,{{0}},0,
                        "reset next counter" },
    { 9, "nextseq",        SCMD_NEXTSEQ,{{CMDSTR(tmp_str)}},0,
                        "go to next entity with the given sequencer name" },
    { 9, "nextnpccluster",    SCMD_NEXTNPCCLUSTER,{{0}},0,
                        "go to next npc beacon cluster" },
    { 1, "gotoent",        SCMD_GOTOENT, {{ CMDINT(entity_id) }},0,
                        "go to an entity by id." },
    { 1, "gotoentbyname",    SCMD_GOTOENTBYNAME, {{ CMDSTR(tmp_str) }},0,
                        "go to first entity with matching name." },
    { 9, "dbquery",        SCMD_DBQUERY,        {{ CMDSENTENCE(tmp_str) }},0,
                        "there should be some more detailed docs for this command offline" },
    { 1, "invincible",    SCMD_INVINCIBLE,    {{ CMDINT(invincible_state) }}, 0,
                        "<0/1> 1=invincible, 0=normal" },
    { 1, "unstoppable",    SCMD_UNSTOPPABLE,    {{ CMDINT(unstoppable_state) }}, 0,
                        "<0/1> 1=unstoppable, 0=normal" },
    { 2, "donottriggerspawns",    SCMD_DONOTTRIGGERSPAWNS,    {{ CMDINT(tmp_int) }}, 0,
                        "<0/1> 1=donottriggerspawns, 0=normal" },
    { 2, "alwayshit",    SCMD_ALWAYSHIT,    {{ CMDINT(alwayshit_state) }}, 0,
                        "<0/1> 1=always hit, 0=normal" },
    { 1, "untargetable",SCMD_UNTARGETABLE,    {{ CMDINT(untargetable_state) }}, 0,
                        "<0/1> 1=untargetable, 0=normal" },
    { 2, "scmds",        SCMD_SCMDS,{{CMDSTR(tmp_str)}},CMDF_HIDEVARS,
                        "print server commands containing <string>" },
    { 0, "scmdlist",    SCMD_CMDLIST,{{0}},CMDF_HIDEVARS | CMDF_HIDEPRINT,
                        "Print available commands" },
    { 9, "scmdms",        SCMD_MAKE_MESSAGESTORE,{{0}}, CMDF_HIDEVARS,
                        "Saves commands the need to be translated" },
    { 2, "scmdusage",    SCMD_SCMDUSAGE,{{CMDSTR(tmp_str)}},CMDF_HIDEVARS,
                        "print usage of server commands containing <string>" },
    { 1, "serverwho",    SCMD_SERVERWHO,{{CMDINT(status_server_id)}},CMDF_HIDEVARS,
                        "get npc/player counts for <mapserver id> use -1 to get total for all mapservers." },
    { 1, "supergroupwho",    SCMD_SUPERGROUPWHO,{{CMDSENTENCE(group_name)}},CMDF_HIDEVARS,
                        "get info on <supergroup>" },
    { 1, "teamupwho",    SCMD_TEAMUPWHO,{{CMDINT(tmp_int)}},CMDF_HIDEVARS,
                        "get info on <teamup>" },
    { 1, "raidwho",        SCMD_RAIDWHO,{{CMDINT(tmp_int)}},CMDF_HIDEVARS,
                        "get info on <raid>" },
    { 1, "levelingpactwho",    SCMD_LEVELINGPACTWHO,{{CMDINT(tmp_int)}},CMDF_HIDEVARS,
                        "get info on <levelingpact>" },
    { 0, "who",            SCMD_WHO,{{CMDSENTENCE(player_name)}},CMDF_HIDEVARS,
                        "get info on <player>" },
    { 0, "whoall",        SCMD_WHOALL,{{0}},CMDF_HIDEVARS,
                        "Print who's on this map." },
    { 1, "status",        SCMD_STATUS,{{CMDINT(status_server_id)}},CMDF_HIDEVARS,
                        "get status of <mapserver id>, use -1 to get all mapservers" },
    { 2, "csr",            SCMD_CSR,{{CMDSTR(player_name)},{CMDSENTENCE(tmp_str)}}, 0,
                        "execute <command> as if you were <player> (e.g. \"csr Joe levelupxp 10\")" },
    { 9, "csr_long",    SCMD_CSR_LONG,{{CMDINT(tmp_int)}, {CMDINT(tmp_int2)}, {CMDSTR(player_name)},{CMDSENTENCE(tmp_str)}}, 0,
                        "execute <command> as if you were <player> and had <access_level> (e.g. \"csr_long 3 1234 Joe levelupxp 10\")" },
    { 2, "csr_radius",    SCMD_CSR_RADIUS,{{PARSETYPE_FLOAT, &tmp_float}, {CMDSENTENCE(tmp_str)}}, 0,
                        "execute <command> as if csr for all players in specified radius (-1 for whole map)" },
    { 2, "csr_offline",    SCMD_CSR_OFFLINE,{{CMDSTR(player_name)},{CMDSENTENCE(tmp_str)}}, 0,
                        "execute <command> as if you were <player>, even if player is offline (e.g. \"csr_offline Joe levelupxp 10\")" },
    { 9, "csr_offline_long",    SCMD_CSR_OFFLINE_LONG,{{CMDINT(tmp_int)}, {CMDINT(tmp_int2)}, {CMDINT(tmp_int3)},{CMDSENTENCE(tmp_str)}}, 0,
                        "execute <command> as if you were <player> and had <access_level>, even if player is offline (e.g. \"csr_offline_long 3 1234 2345 levelupxp 10\")" },
    { 1, "altaccess",    SCMD_ALTACCESS,{{CMDSTR(player_name)},{CMDINT(tmp_int)}}, 0,
                        "Give GM Access to an Alternate Character up to your current Access Level /altaccess <CharacterName> <AccessLevel>" },
    { 0, "played",    SCMD_PLAYTIME,{{0}}, 0,
                            "Show how much time has been played on this character." },
    { 1, "tmsg",        SCMD_TMSG,{{CMDINT(tmp_int)},{CMDSENTENCE(tmp_str)}},CMDF_HIDEVARS,
                        "send <teamup id> a <message>" },
    { 9, "silenceall",    SCMD_SILENCEALL,{{CMDINT(tmp_int)}},CMDF_HIDEVARS,
                        "<0/1> set to 1=global silence on, 0=normal" },
    { 2, "silence",        SCMD_SILENCE,{{CMDSTR(player_name)},{CMDINT(tmp_int)}},CMDF_HIDEVARS,
                        "ban <player> from general chat for <number of minutes>" },
    { 2, "banchat",        SCMD_BANCHAT,{{CMDSTR(player_name)},{CMDINT(tmp_int)}},CMDF_HIDEVARS,
                        "ban <player> from general chat for <number of days>" },
    { 9, "banchat_relay",SCMD_BANCHAT_RELAY,{{CMDSTR(player_name)},{CMDINT(tmp_int)},{CMDINT(tmp_int2)},{CMDINT(tmp_int3)}},CMDF_HIDEVARS,
                        "internal command used by /banchat and /silence" },
    { 9, "svrsilentkick",SCMD_SILENTKICK,{{CMDSTR(player_name)}},CMDF_HIDEVARS,
                        "kick <player> without informing the game client" },
    { 2, "svrkick",        SCMD_KICK,{{CMDSTR(player_name)},{CMDSENTENCE(tmp_str)}},CMDF_HIDEVARS,
                        "kick <player> for <reason>" },
    { 2, "svrban",        SCMD_BAN,{{CMDSTR(player_name)},{CMDSENTENCE(tmp_str)}},CMDF_HIDEVARS,
                        "ban <player> for <reason>" },
    { 2, "svrunban",    SCMD_UNBAN,{{CMDSTR(player_name)}},CMDF_HIDEVARS,
                        "unban <player>" },
    { 1, "invisible",    SCMD_INVISIBLE,{{CMDINT(tmp_int)}},CMDF_HIDEVARS,
                        "<0/1> 1=invisible ,0=visible" },
    { 1, "teleport",    SCMD_TELEPORT,{{CMDSENTENCE(player_name)}},CMDF_HIDEVARS,
                        "Teleport the <player> to me" },
    { 2, "mapmovepos",    SCMD_MAPMOVEPOS,{{CMDSTR(player_name)},{CMDINT(tmp_int)},{PARSETYPE_VEC3,setpos},{CMDINT(tmp_int2)},{CMDINT(tmp_int3)}},CMDF_HIDEVARS,
                        "Helper function for teleport, goto, contact, and door stuff" },
    { 2, "mapmoveposandselectcontact", SCMD_MAPMOVEPOS_ANDSELECTCONTACT, {{CMDSTR(player_name)},{CMDINT(tmp_int)},{PARSETYPE_VEC3,setpos},{CMDINT(tmp_int2)},{CMDINT(tmp_int3)},{CMDSTR(tmp_str)}},CMDF_HIDEVARS,
                        "Helper function for contact finder" },
    { 2, "offlinemove",    SCMD_OFFLINE_MOVE, {{CMDSTR(player_name)},{CMDINT(tmp_int)},{PARSETYPE_VEC3,setpos}},CMDF_HIDEVARS,
                        "CSR command for moving an offline player to a specific static map and position" },
    { 2, "freeze",        SCMD_FREEZE,{{CMDSTR(player_name)},{CMDINT(tmp_int)}},CMDF_HIDEVARS,
                        "Freeze <player> <0/1> so he can't move" },
    { 4, "spawnnpc",    SCMD_SPAWNNPC, {{CMDSTR( tmp_str)}}, CMDF_HIDEVARS, 
                        0},
    { 4, "spawn",        SCMD_SPAWN, {{CMDSTR(tmp_str)}}, CMDF_HIDEVARS,
                        "The command spawns a specific type or group of Critters or NPCs.\n"
                        "The syntax is: spawn <CritterName or NPCName or SpawnBindingName>\n"
                        "Any of the names attached to an NPC block or a Critter block in\n"
                        "N:/game/data/SpawnArea/Global.txt can be used in place of CritterName or NPCName.\n"},
    { 4, "spawnvillain", SCMD_SPAWNVILLAIN, {{CMDSENTENCE(tmp_str)}}, CMDF_HIDEVARS, 
                        0},
    { 9, "spawnmob",    SCMD_SPAWN_MOB, {{CMDINT(tmp_int)}, {CMDINT(tmp_int2)}, {CMDINT(tmp_int3)}}, CMDF_HIDEVARS,
                        "This command spawns a mob of random villain critters.\n"
                        "The syntax is: spawnmob <number of villains> <min level> <max_level>\n"
                        "Only villains within the given level range will attempt to be spawned.\n"},    
    { 4, "spawnpet",    SCMD_SPAWNPET, {{CMDSENTENCE(tmp_str)}}, CMDF_HIDEVARS, 0},
    { 4, "spawnmany",    SCMD_SPAWNMANY, {{CMDSTR(tmp_str)}, {CMDINT(tmp_int)}}, CMDF_HIDEVARS,
                        "The command spawns a number of a specific type or group of Critters or NPCs.\n"
                        "The syntax is: spawnmany <CritterName or NPCName or SpawnBindingName> <Count>\n"
                        "Any of the names attached to an NPC block or a Critter block in\n"
                        "N:/game/data/SpawnArea/Global.txt can be used in place of CritterName or NPCName.\n"},
    { 4, "spawnmanyvillains",    SCMD_SPAWNMANYVILLAINS, {{CMDSTR(tmp_str)}, {CMDINT(tmp_int)}}, CMDF_HIDEVARS,
                        "The command spawns a number of villains.\n"
                        "The syntax is: spawnmanyvillains <VillainName> <Count>\n"},
    { 4, "spawnobjective", SCMD_SPAWNOBJECTIVE, {{CMDSTR(tmp_str)}}, CMDF_HIDEVARS,
                        "This command spawns an objective model as it would be seen in a mission.\n"
                        "The syntax is: spawnobjective <ModelName>\n"},
    { 4, "spawndoppelganger", SCMD_SPAWNDOPPELGANGER, {{0}}, CMDF_HIDEPRINT,
                    "spawn a doppleganger" },
    { 4, "doppelgangertest", SCMD_SPAWNDOPPELGANGERTEST,  {{CMDSENTENCE(tmp_str)}}, CMDF_HIDEVARS, 
                        "spawns a dopplgange with keywords" },
    { 9, "spawnmissionnpcs", SCMD_SPAWNMISSIONNPCS, {{CMDINT(tmp_int)}}, CMDF_HIDEVARS,
                        "Spawn <n> random NPC's who will run around scared.  They can spawn from any encounter location\n"},
    { 9, "raoulcmd",     SCMD_RAOULCMD, {{ 0 }}, CMDF_HIDEVARS,
                        "Raoul's personal command" },
    { 9, "settest1",    0, {{ PARSETYPE_FLOAT, &server_state.test1 }},0,
                        "Set server test variable 1" },
    { 9, "settest2",    0, {{ PARSETYPE_FLOAT, &server_state.test2 }},0,
                        "Set server test variable 2" },
    { 9, "settest3",    0, {{ PARSETYPE_FLOAT, &server_state.test3 }},0,
                        "Set server test variable 3" },
    { 0, "ignore",         SCMD_IGNORE, {{ CMDSENTENCE(tmp_str) }}, CMDF_HIDEVARS,
                        "Ignore Player" },
    { 0, "gignore",         SCMD_IGNORE, {{ CMDSENTENCE(tmp_str) }}, CMDF_HIDEVARS,
                        "Ignore Player" },
    { 0, "ignore_spammer", SCMD_IGNORE_SPAMMER, {{ CMDSENTENCE(tmp_str) }}, CMDF_HIDEVARS,
                        "Ignore As Spammer" },
    { 0, "ignore_spammer_auth", SCMD_IGNORE_SPAMMER_AUTH, {{ CMDINT(tmp_int) },{ CMDSENTENCE(tmp_str) }}, CMDF_HIDEVARS|CMDF_HIDEPRINT,
                        "Ignore As Spammer" },
    { 0, "unignore",     SCMD_UIGNORE, {{ CMDSENTENCE(tmp_str) }}, CMDF_HIDEVARS,
                        "Unignore User" },
    { 0, "gunignore",     SCMD_UIGNORE, {{ CMDSENTENCE(tmp_str) }}, CMDF_HIDEVARS,
                        "Unignore User" },
    { 0, "ignorelist",    SCMD_IGNORE_LIST, {{ 0 }}, 0,
                        "Displays a list of ignored users" },
    { 3, "ignore_spammer_threshold", SCMD_IGNORETHRESHOLD, {{CMDINT(tmp_int)}}, 0,
                            "set the number of ignores needed to auto-chatbanned someone to [int], 0 disables" },
    { 3, "ignore_spammer_multiplier", SCMD_IGNOREMULTIPLIER, {{CMDINT(tmp_int)}}, 0,
                            "set the number of ignores generated by ignore_spammer to [int]" },
    { 3, "ignore_spammer_duration", SCMD_IGNOREDURATION, {{CMDINT(tmp_int)}}, 0,
                            "set how long auto-chatbanned players are silenced to [int] seconds" },
    { 2, "releaseplayer",    SCMD_RELEASE_PLAYER_CONTROL, {{ 0 }}, 0,
                            "Frees controlled player character." },
    { 9, "enablecontrollog",SCMD_ENABLE_CONTROL_LOG, {{ CMDINT(tmp_int) }, { CMDINT(tmp_int2) }}, CMDF_HIDEVARS,
                            "enable control log on a client for an amount of time." },
    { 1, "smapname",        SCMD_SMAPNAME, {{ CMDINT(tmp_int) }}, CMDF_HIDEVARS,
                            "get name of <map id>" },
    { 1, "maplist",            SCMD_MAPLIST, {{ 0 }}, 0,
                            "Displays a list of all static maps" },
    { 0, "playerlocale",    SCMD_PLAYERLOCALE, {{ CMDINT(tmp_int) }}, CMDF_HIDEVARS|CMDF_HIDEPRINT,
                            "Changes the locale of a player"},
    { 9, "entgenmsg", 0, {{ CMDINT(server_state.printEntgenMessages) }}, 0,
                            "print debug messages when generating entities"},
    { 9, "grpfindtest", SCMD_GROUPFIND_TEST, {{0}}, 0,
                            "perform group find on all trays in the map"},
    { 9, "validatespawns",    0, {{ CMDINT(server_state.validate_spawn_points) }}, CMDF_HIDEVARS,
                            "Enables collecting of bad (embedded in stuff) spawn points from encounters." },
    { 9, "nextbadspawn",    SCMD_NEXTBADSPAWN, {{0}}, CMDF_HIDEVARS,
                            "Goes to next bad spawn point." },
    { 9, "nextbadvolume",    SCMD_NEXTBADVOLUME, {{0}}, CMDF_HIDEVARS,
                            "Goes to next pair of intersecting material volumes." },
    { 9, "rescanbadvolume",    SCMD_RESCANBADVOLUME, {{0}}, CMDF_HIDEVARS,
                            "Rebuilds the list of overlapping volumes." },
    { 1, "nextspawn",        SCMD_NEXTSPAWN, {{0}}, CMDF_HIDEVARS,
                            "Goes to next active encounter." },
    { 1, "nextspawnpoint",    SCMD_NEXTSPAWNPOINT, {{0}}, CMDF_HIDEVARS,
                            "Goes to next encounter group." },
    { 1, "nextmissionspawn",SCMD_NEXTMISSIONSPAWN, {{0}}, CMDF_HIDEVARS,
                            "Goes to next spawn overridden by mission system." },
    { 1, "nextumissionspawn",SCMD_NEXTUMISSIONSPAWN, {{0}}, CMDF_HIDEVARS,
                            "Goes to next unconquered spawn overridden by mission system." },
    { 1, "nextunconqueredmissionspawn",SCMD_NEXTUMISSIONSPAWN, {{0}}, CMDF_HIDEVARS,
                            "Goes to next unconquered spawn overridden by mission system." },
    { 9, "encounterreset",    SCMD_ENCOUNTER_RESET, {{CMDSTR(tmp_str)}}, CMDF_HIDEVARS,
                            "Reset all random encounter spawn points, or a specific one" },
    { 3, "encounterdebug",    SCMD_ENCOUNTER_DEBUG, {{ CMDINT(tmp_int) }}, CMDF_HIDEVARS,
                            "Turns encounter debugging on or off"},
    { 9, "encounterreload", SCMD_ENCOUNTER_RELOAD, {{ 0 }}, 0,
                            "Reload encounter spawn definitions" },
    { 4, "encounterteamsize",    SCMD_ENCOUNTER_TEAMSIZE, {{ CMDINT(tmp_int) }}, CMDF_HIDEVARS,
                            "Force the encounter spawns to pretend you have a team size of X"},
    { 4, "encounterspawn",    SCMD_ENCOUNTER_SPAWN, {{CMDSTR(tmp_str)}}, CMDF_HIDEVARS,
                            "Spawn a specific encounter group (manual spawning groups only)" },
    { 4, "encounterspawnclosest",SCMD_ENCOUNTER_SPAWN_CLOSEST, {{ 0 }}, CMDF_HIDEVARS,
                            "Spawn the closest encounter group to the player" },
    { 4, "encounterignoregroups", SCMD_ENCOUNTER_IGNORE_GROUPS, {{ CMDINT(tmp_int) }}, CMDF_HIDEVARS,
                            "Causes the encounter system to ignore how encounters are grouped, -1 to return to default" },
    { 4, "entityencounterspawn",    SCMD_ENTITYENCOUNTER_SPAWN, {{ CMDINT(tmp_int) }}, CMDF_HIDEVARS,
                            "Respawn the encounter for specified entity id." },
    { 4, "encounteralwaysspawn",    SCMD_ENCOUNTER_ALWAYS_SPAWN, {{ CMDINT(tmp_int) }}, CMDF_HIDEVARS,
                            "Make all encounters go off instead of checking spawn probabilities (for testing)" },
    { 4, "encountermem",    SCMD_ENCOUNTER_MEMUSAGE, {{0}}, CMDF_HIDEVARS,
                            "Show memory usage for the encounter system" },
    { 4, "encounterneighborhoodlist", SCMD_ENCOUNTER_NEIGHBORHOOD, {{0}}, CMDF_HIDEVARS,
                            "List all encounters on map, broken down by neighborhood" },
    { 9, "encounterprocessing", SCMD_ENCOUNTER_PROCESSING, {{ CMDINT(tmp_int) }}, CMDF_HIDEVARS,
                            "Turn on or off all encounter processing" },
    { 9, "encounterstat",    SCMD_ENCOUNTER_STAT, {{ 0 }}, CMDF_HIDEVARS,
                            "Show how many encounters are running" },
    { 9, "encountertweak",    SCMD_ENCOUNTER_SET_TWEAK, {{CMDINT(tmp_int)}, {CMDINT(tmp_int2)}, {CMDINT(tmp_int3)}}, CMDF_HIDEVARS,
                            "Change the encounter spawn numbers (dangerous)" },
    { 9, "encounterpanicthreshold",    SCMD_ENCOUNTER_PANIC_THRESHOLD, {{PARSETYPE_FLOAT, &tmp_vec[0]}, {PARSETYPE_FLOAT, &tmp_vec[1]}, {CMDINT(tmp_int)}}, CMDF_HIDEVARS,
                            "Change the encounter spawn numbers (dangerous)" },
    { 9, "encountermode",    SCMD_ENCOUNTER_MODE, {{CMDSTR(tmp_str)}}, CMDF_HIDEVARS,
                            "Change the encounter mode to city/mission" },
    { 9, "encountercoverage",SCMD_ENCOUNTER_COVERAGE, {{ 0 }}, CMDF_HIDEVARS,
                            "Print what villains and what layouts this city zone has" },
    { 9, "encounterminautostart", SCMD_ENCOUNTER_MINAUTOSTART, {{CMDINT(tmp_int)}}, CMDF_HIDEVARS,
                            "Add a minimum for the autostart time (for debugging autostart spawns)" },
    { 9, "critterlimits",    SCMD_CRITTER_LIMITS, {{CMDINT(tmp_int)}, {CMDINT(tmp_int2)}}, CMDF_HIDEVARS,
                            "Change the min/max number of critters on map (0 for no limit)" },
    { 9, "scriptdefstart",    SCMD_SCRIPTDEF_START, {{CMDSTR(tmp_str)}}, CMDF_HIDEVARS,
                            "Syntax: <ScriptDefPath> Starts any non-Location type scriptDef on the targeted entity" },
    { 9, "scriptluastart",    SCMD_SCRIPTLUA_START, {{CMDSTR(tmp_str)}, {CMDSTR(tmp_str2)}}, CMDF_HIDEVARS,
                            "Syntax: <Lua(Zone|Mission|Encounter|Entity)> <LuaPath> Starts a script running the given Lua file on the targeted entity" },
    { 9, "scriptluastring",    SCMD_SCRIPTLUA_START_STRING, {{CMDSTR(tmp_str)}, {CMDSENTENCE(tmp_str2)}}, CMDF_HIDEVARS,
                            "Syntax: <Lua(Zone|Mission|Encounter|Entity)> <LuaString> Starts a script running the given Lua string on the targeted entity" },
    { 9, "scriptluaexec",    SCMD_SCRIPTLUA_EXEC, {{CMDINT(tmp_int)}, {CMDSENTENCE(tmp_str)}}, CMDF_HIDEVARS,
                            "Syntax: <ScriptId> <LuaString> Runs the given Lua string inside the specified script" },
    { 2, "zonescriptstart",    SCMD_ZONESCRIPT_START, {{CMDSTR(tmp_str)}}, CMDF_HIDEVARS,
                            "Start a particular zone script" },
    { 2, "zoneeventstart",    SCMD_ZONEEVENT_START, {{CMDSTR(tmp_str)}}, CMDF_HIDEVARS,
                            "Start a particular zone event" },
    { 2, "zoneeventstop",    SCMD_ZONEEVENT_STOP, {{CMDSTR(tmp_str)}}, CMDF_HIDEVARS,
                            "Stop a particular zone event" },
    { 2, "zoneeventsignal",    SCMD_ZONEEVENT_SIGNAL, {{CMDSTR(tmp_str)}, {CMDSTR(tmp_str2)}}, CMDF_HIDEVARS,
                            "Send a named signal to a zone event" },
    { 2, "shardeventstart",    SCMD_SHARDEVENT_START, {{CMDSTR(tmp_str)}}, CMDF_HIDEVARS,
                            "Start a shard event" },
    { 2, "shardeventstop",    SCMD_SHARDEVENT_STOP, {{ 0 }}, CMDF_HIDEVARS,
                            "Stops the running shard event" },
    { 2, "shardeventsignal",SCMD_SHARDEVENT_SIGNAL, {{CMDSTR(tmp_str)}}, CMDF_HIDEVARS,
                            "Send a named signal to the shard event" },
    { 3, "scriptlocationprint",SCMD_SCRIPTLOCATION_PRINT, {{ 0 }}, CMDF_HIDEVARS,
                            "Print a list of script locations on this map" },
    { 3, "scriptlocationstart",SCMD_SCRIPTLOCATION_START, {{CMDSTR(tmp_str)}}, CMDF_HIDEVARS,
                            "Starts a named script location" },
    { 3, "scriptlocationstop",SCMD_SCRIPTLOCATION_STOP, {{CMDSTR(tmp_str)}}, CMDF_HIDEVARS,
                            "Stops a named script location" },
    { 3, "scriptlocationsignal",SCMD_SCRIPTLOCATION_SIGNAL, {{CMDSTR(tmp_str)}, {CMDSTR(tmp_str2)}}, CMDF_HIDEVARS,
                            "Sends a signal to a named script location" },
    { 9, "scriptdebugserver",SCMD_SCRIPT_DEBUGSET,    {{ CMDINT(tmp_int) }, { CMDINT(tmp_int2) }},0,
                            "turns on script debugging for client" },
    { 9, "scriptshowvars",    SCMD_SCRIPT_SHOWVARS,    {{ CMDINT(tmp_int) }},0,
                            "turns on showing script vars to client" },
    { 9, "scriptsetvar",    SCMD_SCRIPT_SETVAR,    {{ CMDINT(tmp_int) }, {CMDSTR(tmp_str)}, {CMDSTR(tmp_str2)}},0,
                            "turns on showing script vars to client" },
    { 9, "scriptpause",        SCMD_SCRIPT_PAUSE,    {{ CMDINT(tmp_int) }, { CMDINT(tmp_int2) }},0,
                            "pauses the selected script" },
    { 9, "scriptreset",        SCMD_SCRIPT_RESET,    {{ CMDINT(tmp_int) }},0,
                            "resets and restarts the script" },
    { 9, "scriptstop",        SCMD_SCRIPT_STOP,    {{ CMDINT(tmp_int) }},0,
                            "stops the script" },
    { 9, "scriptsignal",    SCMD_SCRIPT_SIGNAL,    {{ CMDINT(tmp_int) }, {CMDSENTENCE(tmp_str)}},0,
                            "signals the script" },
    { 9, "scriptcombatlevel",        SCMD_SCRIPT_COMBAT_LEVEL,    {{ CMDINT(tmp_int) }},0,
                            "sets the exemplar/sidekick of everyone in the zone to this level + 1.  Use 0 to unset." },
    { 9, "destroy",            SCMD_DESTROY, {{ CMDINT(tmp_int) },{ CMDINT(tmp_int2) },{ CMDINT(tmp_int3) }}, CMDF_HIDEVARS,
                            "test destroy world geometry"},
    { 2, "omnipotent",        SCMD_OMNIPOTENT, {{ 0 }}, 0,
                            "Gives character all powers someone of his class can have."},        
    { 9, "defsreload",        SCMD_DEFS_RELOAD, {{ 0 }}, 0,
                            "Reload class, origin, and power defs. Warning: Will leak some memory."},
    { 3, "inspire",            SCMD_INSPIRE, {{ CMDSTR(tmp_str) }, { CMDSTR(tmp_str2) }}, CMDF_HIDEVARS,
                            "Gives the player a specific inspiration."},
    { 9, "inspirex",        SCMD_INSPIREX, {{ 0 }}, 0,
                            "Gives the player a random inspiration."},
    { 9, "inspirearena",    SCMD_INSPIRE_ARENA, {{ 0 }}, 0,
                            "Give the player a standard set of arena inspirations."},
    { 0, "mergeInsp",       SCMD_INSPMERGE, {{ CMDSTR(tmp_str) }, { CMDSTR(tmp_str2) }}, CMDF_HIDEVARS|CMDF_HIDEPRINT, 
                            "Combine 3 inspirations into one of a different type",     },
    { 3, "boost",            SCMD_BOOST, {{ CMDSTR(tmp_str) }, { CMDSTR(tmp_str2) }, { CMDINT(tmp_int) }}, CMDF_HIDEVARS,
                            "Gives the player a specific enhancement."},
    { 3, "boostset",        SCMD_BOOSTSET, {{ CMDSTR(tmp_str) }, { CMDINT(tmp_int) }}, CMDF_HIDEVARS,
                            "Gives the player the full set of enhancements."},
    { 9, "boostx",            SCMD_BOOSTX, {{CMDINT(tmp_int)}}, CMDF_RETURNONERROR,
                            "Gives the player a random enhancement. optional param is #combines"},
    { 9, "boosts_setlevel",    SCMD_BOOSTS_SETLEVEL, {{ CMDINT(tmp_int) }}, CMDF_HIDEVARS,
                            "Sets all slotted boosts to the given level."},                            
    { 0, "boost_convert",    SCMD_CONVERT_BOOST, {{ CMDINT(tmp_int) }, { CMDSTR(tmp_str) }}, CMDF_HIDEVARS,
                            "Converts the specified boost into something else. <idx> <conversion set>"},
    { 2, "temppower",        SCMD_TEMP_POWER, {{ CMDSTR(tmp_str) }, { CMDSTR(tmp_str2) }}, CMDF_HIDEVARS,
                            "Gives the player a specific temp power."},
    { 2, "temppower_revoke",SCMD_TEMP_POWER_REVOKE, {{ CMDSTR(tmp_str) }, { CMDSTR(tmp_str2) }}, CMDF_HIDEVARS,
                            "Removes specific temp power from a player."},
    { 9, "temppowerx",        SCMD_TEMP_POWERX, {{ 0 }}, 0,
                            "Gives the player a random temp power."},
    { 9, "pvp_switch",        SCMD_PVP_SWITCH, {{ CMDINT(tmp_int) }}, 0,
                            "Sets your permanent PvP preference. (1 turns on, 0 turns off)."},
    { 9, "pvp_active",        SCMD_PVP_ACTIVE, {{ CMDINT(tmp_int) }}, 0,
                            "Sets your current PvP mode. (1 turns on, 0 turns off)."},
    { 9, "pvp_logging",        SCMD_PVP_LOGGING, {{ CMDINT(tmp_int) }},0,
                            "enable/disable pvp logging [on/off]" },
    { 9, "combatstatsreset",            SCMD_COMBATSTATSRESET, {{ 0 }}, 0,
                            "Reset all combat stats."},
    { 9, "combatstatsdump",                SCMD_COMBATSTATSDUMP, {{ 0 }}, 0,
                            "Dump combat stats for individual ents to combat.log."},
    { 9, "combatstatsdump_player",        SCMD_COMBATSTATSDUMP_PLAYER, {{ 0 }}, 0,
                            "Dump combat stats for all player entities and pets to combat.log."},
    { 9, "combatstatsdump_agg",            SCMD_COMBATSTATSDUMP_AGGREGATE, {{ 0 }}, 0,
                            "Dump all aggregate combat stats for all powers used to combat.log."},
    { 9, "combatstatsdump_aggregate",    SCMD_COMBATSTATSDUMP_AGGREGATE, {{ 0 }}, 0,
                            "Dump all aggregate combat stats for all powers used to combat.log."},
    { 9, "teamcreate",        SCMD_TEAMCREATE, {{ CMDINT(tmp_int) }}, CMDF_HIDEVARS,
                            "5 = teamup, 6 = supergroup"},
    { 9, "teamleave",        SCMD_TEAMLEAVE, {{ CMDINT(tmp_int) }}, CMDF_HIDEVARS,
                            "5 = teamup, 6 = supergroup"},
    { 9, "teamjoin",        SCMD_TEAMJOIN, {{ CMDINT(tmp_int) }}, CMDF_HIDEVARS,
                            "5 = teamup, 6 = supergroup"},
    { 9, "smmonitor",        SCMD_MEMMONITOR, {{ 0 }}, 0,
                            "dump server memory usage"},
    { 9, "smemchecknow",    SCMD_MEM_CHECK, {{ 0 }}, 0,
                            "does a server side _CrtMemCheck" },
    { 9, "stringtablememdumpserver",    SCMD_STRINGTABLE_MEM_DUMP, {{ 0 }}, 0,
                            "Dumps the mem usage of the string tables to the console" },
    { 9, "stashtablememdumpserver",    SCMD_STASHTABLE_MEM_DUMP, {{ 0 }}, 0,
                            "Dumps the mem usage of the hash tables to the console" },
    { 2, "storyarcprint",    SCMD_STORYARC_PRINT, {{ 0 }}, 0,
                            "print information on your story arc" },
    { 2, "storyarcdetail",    SCMD_STORYARC_DETAILS, {{ 0 }}, 0,
                            "more detailed information on your story arc" },
    { 9, "storyarcreload",    SCMD_STORYARC_RELOAD, {{ 0 }}, 0,
                            "reloads story arc scripts (contacts, missions, tasks); affects everyone on server" },
    { 9, "sadialog",        SCMD_STORYARC_COMPLETE_DIALOG_DEBUG, {{ 0 }}, 0,
                            "sends the client a test storyarc complete dialog" },
    { 3, "fixupstoryarcs",    SCMD_FIXUP_STORYARCS, {{ 0 }}, 0,
                            "clears any story arc episode issued tasks which have not been completed and are not in the player's tasklist" },
    { 2, "contactdetail",    SCMD_CONTACT_DETAILS, {{ CMDSTR(tmp_str) }}, CMDF_HIDEVARS,
                            "detailed info on the state of a given contact" },
    { 9, "reward",            SCMD_REWARD, {{ CMDSTR(tmp_str)}, { CMDINT(tmp_int) }}, CMDF_HIDEVARS,
                            "Give your entity a reward for defeating a villain of the specified level"},
    { 9, "rewardreload",    SCMD_REWARD_RELOAD, {{ 0 }}, 0,
                            "Reload reward definitions"},
    { 9, "rewardinfo",        SCMD_REWARD_INFO, {{ CMDINT(tmp_int) }}, CMDF_HIDEVARS,
                            "Print reward debugging info in server console window"},
    { 9, "rewarddefapply",    SCMD_REWARDDEF_APPLY, {{CMDINT(tmp_int)}, {CMDSTR(tmp_str)}, {CMDINT(tmp_int2)}, {CMDINT(tmp_int3)}, {CMDINT(tmp_int4)}}, CMDF_HIDEVARS,
                            "(internal command) rewarddefapply <db_id> <reward_def_name> <villain group> <level> <source>"},
    { 9, "rewardstory",        SCMD_REWARD_STORY, {{CMDINT(tmp_int)}, {CMDINT(tmp_int2)}, {CMDINT(tmp_int3)}, {CMDINT(tmp_int4)}, {CMDINT(tmp_int5)}, {CMDSTR(tmp_str2)}, {CMDSTR(tmp_str)}}, CMDF_HIDEVARS,
                            "(internal command) rewardstory <db_id> <villain group> <level> <level adjust> <source> <storyarc> <reward_def_name>"},
    { 9, "rewarddebugtarget", SCMD_REWARD_DEBUG_TARGET, {{CMDINT(tmp_int)}}, CMDF_HIDEVARS,
                            "Aggregate rewards from killing the the current target X iterations" },
    { 3, "getcontacttask",    SCMD_CONTACT_GETTASK, {{CMDSTR(tmp_str)}, {CMDSTR(tmp_str2)}}, CMDF_HIDEVARS,
                            "get a specific task, specify contact and logical name" },
    { 3, "gettask",            SCMD_GETTASK, {{CMDSTR(tmp_str)}}, CMDF_HIDEVARS,
                            "get a specific task, specify the logical name of task (a suitable contact will be randomly selected)" },
    // 'gettaskqa' is identical to 'gettask', but provided for backwards compatibility
    { 4, "gettaskqa",        SCMD_GETTASK, {{CMDSTR(tmp_str)}}, CMDF_HIDEVARS,
                            "get a specific task, specify the logical name of task (a suitable contact will be randomly selected) (Identical to '/gettask')" },
    { 4, "getcontactstoryarc", SCMD_CONTACT_GETSTORYARC, {{CMDSTR(tmp_str)}, {CMDSTR(tmp_str2)}}, CMDF_HIDEVARS,
                            "get a specific storyarc, specify contact and storyarc filenames" },
    { 3, "getstoryarc",        SCMD_GETSTORYARC, {{CMDSTR(tmp_str)}}, CMDF_HIDEVARS,
                            "get a specific storyarc, specify the logical name (a suitable contact will be randomly selected)" },
    { 4, "getstoryarctask", SCMD_GETSTORYARCTASK, {{CMDSTR(tmp_str)}, {CMDSTR(tmp_str2)}}, CMDF_HIDEVARS,
                            "get a specific storyarc, specify the story arc and episode" },
    { 2, "accepttask",        SCMD_CONTACT_ACCEPTTASK, {{CMDSTR(tmp_str)}, {CMDINT(tmp_int)}}, 0,
                            "get issued a task, specify contact and whether you want a long one (1) or short (0)" },
    { 4, "startmission",    SCMD_STARTMISSION, {{CMDSTR(tmp_str)}, {CMDSTR(tmp_str2)}}, CMDF_HIDEVARS,
                            "Assign and teleport to a specific mission and map.\n"
                            "Syntax: startmission <mission name> <mission map name>\n"
                            "If '*' is given for map name, a default map will be selected"},
    { 4, "startstoryarcmission",SCMD_STARTSTORYARCMISSION, {{CMDSTR(tmp_str)}, {CMDSTR(tmp_str2)}, {CMDSTR(tmp_str3)}}, CMDF_HIDEVARS,
                            "Assign and teleport to a specific mission and map.\n"
                            "Syntax: startmission <arc name> <episode name> <map name>\n"
                            "If '*' is given for map name, a default map will be selected"},
    { 0, "requestexitmission",    SCMD_MISSION_USEREXIT, {{ CMDINT(tmp_int) }}, CMDF_HIDEVARS|CMDF_HIDEPRINT,
                            "leave mission map once completed" },
    { 0, "enterdoorteleport",    SCMD_REQUEST_TELEPORT, {{CMDSTR(tmp_str)}, {CMDINT(tmp_int)}}, CMDF_HIDEVARS|CMDF_HIDEPRINT,
                            "request teleport to another map" },
    { 0, "requestexitmissionalt",    SCMD_REQUEST_MISSION_EXIT_ALT, {{CMDSTR(tmp_str)}, {CMDINT(tmp_int)}}, CMDF_HIDEVARS|CMDF_HIDEPRINT,
                            "exit mission and teleport to another map" },
    { 0, "enterscriptdoor",    SCMD_REQUEST_SCRIPT_ENTER_DOOR, {{CMDINT(tmp_int)}, {CMDSTR(tmp_str)}, {CMDINT(tmp_int2)}}, CMDF_HIDEVARS|CMDF_HIDEPRINT,
                            "enter a script controlled door" },
    { 9, "initmission",        SCMD_MISSION_INIT, {{0}}, 0,
                            "Reinitializes the mission as if the player just entered." },
    { 9, "missionreseed",    SCMD_MISSION_RESEED, {{0}}, 0,
                            "Reseeds and initializes the mission as if the player just entered." },
    { 2, "exitmission",        SCMD_MISSION_DEBUGEXIT, {{0}}, 0,
                            "jump out of mission map" },
    { 2, "exitbase",        SCMD_MISSION_DEBUGEXIT, {{0}}, 0,
                            "jump out of your base" },
    { 2, "enterbase",        SCMD_BASE_DEBUGENTER, {{0}}, 0,
                            "jump into your base" },
    { 9, "enterbasebysgid", SCMD_BASE_ENTERBYSGID , {{CMDINT(tmp_int)}}, 0,
                            "jump into your base" },
    { 0, "enter_base_from_passcode", SCMD_BASE_ENTERBYPASSCODE , {{CMDSTR(tmp_str)}}, 0,
							"jump into a base using its passcode" },                            
    { 9, "enterbaseexplicit",    SCMD_BASE_EXPLICIT, {{CMDSTR(tmp_str)}}, 0,
                            "jump to the specified base/raid/apt map" },
    { 9, "returntobase",    SCMD_BASE_RETURN, {{CMDSTR(tmp_str)}}, 0,
                            "return to your base from a raid" },
    { 9, "apartment",        SCMD_APARTMENT, {{0}}, 0,
                            "jump into your apartment" },
    { 9, "missionkickme",    SCMD_MISSION_DEBUGKICK, {{0}}, 0,
                            "ask the mission to kick you from map (debug)" },
    { 9, "missionunkickme",    SCMD_MISSION_DEBUGUNKICK, {{0}}, 0,
                            "ask the mission to stop kicking you from map (debug)" },
    { 9, "resetpredict",    SCMD_RESETPREDICT, {{0}}, 0,
                            "Reset my prediction vars." },
    { 9, "printcontrolqueue",SCMD_PRINTCONTROLQUEUE, {{0}}, 0,
                            "Print my control queue to the mapserver console." },
    { 9, "missionaddtime",    SCMD_MISSION_DEBUG_ADDTIME, {{ CMDINT(tmp_int) }}, 0,
                            "Debugging, adds time to your current mission" },
    { 9, "rewardbonustime", SCMD_REWARD_BONUS_TIME, {{ CMDINT(tmp_int) },{ CMDINT(tmp_int2) },{ CMDINT(tmp_int3) },{ CMDINT(tmp_int4) }}, 0,
                            "Relays that the player has been granted bonus time for a task <db_id> <task id> <task subid> <bonustime>" },
    { 9, "newspaperteamcompleteinternal", SCMD_NEWSPAPER_TEAM_COMPLETE_INTERNAL, {{ CMDINT(tmp_int) },{ CMDINT(tmp_int2) },{ CMDINT(tmp_int3) }}, 0,
                            "Relays that a newspaper or broker task was completed" },
    { 9, "taskteamcompleteinternal", SCMD_TASK_TEAM_COMPLETE_INTERNAL, {{CMDSTR(tmp_str)}}, 0,
                            "taskteamcompleteinternal <db_id> <task id> <task subid> <task cpos> <bPlayerCreated> <leveladjust> <level> <seed>" },
    { 9, "taskcompleteinternal",    SCMD_TASK_COMPLETE_INTERNAL, {{ CMDINT(tmp_int) },{ CMDINT(tmp_int2) },{ CMDINT(tmp_int3) },{ CMDINT(tmp_int4)},{CMDINT(tmp_int5)},{CMDINT(tmp_int6)}}, 0,
                            "(internal command) taskcompleteinternal <db_id> <task id> <task subid> <task cpos> <bPlayerCreated> <success>" },
    { 9, "taskadvancecomplete",    SCMD_TASK_ADVANCE_COMPLETE, {{ CMDINT(tmp_int) }}, 0,
                            "(internal command) taskadvancecomplete <db_id>" },
    { 9, "objectivecompleteinternal",    SCMD_OBJECTIVE_COMPLETE_INTERNAL, {{ CMDINT(tmp_int) },{ CMDINT(tmp_int2) },{ CMDINT(tmp_int3) },{ CMDINT(tmp_int4)},{CMDINT(tmp_int5)},{CMDINT(tmp_int6)},{CMDINT(tmp_int7)}}, 0,
                            "(internal command) objectivecompleteinternal <db_id> <task id> <task subid> <task cpos> <bPlayerCreated> <objective num> <success>" },
    { 9, "missionrequestshutdown",    SCMD_MISSION_REQUEST_SHUTDOWN, {{ CMDINT(tmp_int) },{ CMDINT(tmp_int2) },{ CMDINT(tmp_int3) },{ CMDINT(tmp_int4)},{CMDINT(tmp_int5)},{CMDINT(tmp_int6)}}, 0,
                            "(internal command) missionrequestshutdown <map_id> <owner_id> <task id> <task subid> <task cpos> <bPlayerCreated>" },
    { 9, "missionforceshutdown",    SCMD_MISSION_FORCE_SHUTDOWN, {{ CMDINT(tmp_int) },{ CMDINT(tmp_int2) },{ CMDINT(tmp_int3) },{ CMDINT(tmp_int4)},{CMDINT(tmp_int5)},{CMDINT(tmp_int6)}}, 0,
                            "(internal command) missionforceshutdown <map_id> <owner_id> <task id> <task subid> <task cpos> <bPlayerCreated>" },
    { 9, "missionokshutdown",    SCMD_MISSION_OK_SHUTDOWN, {{0}}, 0,
                            "(internal command) missionokshutdown" },
    { 9, "missionchangeowner",    SCMD_MISSION_CHANGE_OWNER, {{ CMDINT(tmp_int) },{ CMDINT(tmp_int2) },{ CMDINT(tmp_int3) },{ CMDINT(tmp_int4)},{ CMDINT(tmp_int5)}, }, 0,
                            "(internal command) missionchangeowner <owner_id> <task id> <task subid> <task cpos>" },
    { 9, "clueaddremoveinternal",    SCMD_CLUE_ADDREMOVE_INTERNAL, {{ CMDINT(tmp_int) },{ CMDINT(tmp_int2) },{ CMDINT(tmp_int3) },{ CMDINT(tmp_int4) },{ CMDSTR(tmp_str) }}, 0,
                            "(internal command) clueaddremoveinternal <adding?> <db_id> <task id> <task subid> <clue name>" },
    { 9, "clueaddremove",    SCMD_CLUE_ADDREMOVE, {{ CMDINT(tmp_int) },{ CMDSTR(tmp_str)},{ CMDINT(tmp_int2) } }, 0,
                            "add or remove clue to the Xth task on your list - clueaddremove <task id> <cluename> <adding?>" },
    { 9, "missionreattachinternal", SCMD_MISSION_REATTACH_INTERNAL, {{ CMDINT(tmp_int) }, { CMDINT(tmp_int2) }}, CMDF_HIDEVARS,
                            "(internal command) missionreattachinternal <db_id> <map_id>" },
    { 9, "missionkickplayerinternal", SCMD_MISSION_KICKPLAYER_INTERNAL, {{ CMDINT(tmp_int) }}, CMDF_HIDEVARS,
                            "(internal command) missionkickplayerinternal <db_id>" },
    { 2, "completemission",    SCMD_MISSION_COMPLETE, {{0}}, 0,
                            "complete the mission for your current map" },
    { 2, "completetask",    SCMD_TASK_COMPLETE, {{ CMDINT(tmp_int) }}, CMDF_HIDEVARS,
                            "complete the Xth task on your list" },
    { 2, "completesgtask",    SCMD_SG_TASK_COMPLETE, {{0}}, CMDF_HIDEVARS,
                            "Complete the supergroup task" },
    { 3, "failsgtask",        SCMD_SG_TASK_FAIL, {{0}}, CMDF_HIDEVARS,
                            "Fail your supergroup task" },
    { 3, "failtask",        SCMD_TASK_FAIL, {{ CMDINT(tmp_int) }}, CMDF_HIDEVARS,
                            "fail the Xth task on your list" },
    { 9, "taskselectvalidate",    SCMD_TASK_SELECT_VALIDATE, {{ CMDINT(tmp_int) },{ CMDINT(tmp_int2) },{ CMDINT(tmp_int3) },{ CMDINT(tmp_int4)},{CMDINT(tmp_int5)}}, 0,
                            "(internal command) taskselectvalidate <db_id> <task id> <task subid> <task cpos> <bPlayerCreated>" },
    { 9, "taskselectvalidateack", SCMD_TASK_SELECT_VALIDATE_ACK, {{ CMDINT(tmp_int) },{ CMDINT(tmp_int2) },{ CMDINT(tmp_int3) },{ CMDINT(tmp_int4)},{CMDINT(tmp_int5)},{CMDINT(tmp_int6)},{CMDINT(tmp_int7)}}, 0,
                            "(internal command) taskselectvalidateack <db_id> <task id> <task subid> <task cpos> <bPlayerCreated> <valid> <response dbid>" },
    { 9, "flashbackteamrequiresvalidate", SCMD_FLASHBACK_TEAM_REQUIRES_VALIDATE, {{ CMDINT(tmp_int) },{ CMDINT(tmp_int2) },{ CMDINT(tmp_int3) },{ CMDINT(tmp_int4) },{ CMDINT(tmp_int5) }}, 0,
                            "(internal command) flashbackteamrequiresvalidate <db_id> <filename>" },
    { 9, "flashbackteamrequiresvalidateack", SCMD_FLASHBACK_TEAM_REQUIRES_VALIDATE_ACK, {{ CMDINT(tmp_int) },{ CMDINT(tmp_int2) },{ CMDINT(tmp_int3) },{ CMDINT(tmp_int4) },{ CMDINT(tmp_int5) },{ CMDINT(tmp_int6) },{ CMDINT(tmp_int7) }}, 0,
                            "(internal command) flashbackteamrequiresvalidateack <db_id> <result>" },
    { 9, "taskstarttimer",    SCMD_TASK_STARTTIMER, {{ CMDINT(tmp_int) },{ CMDINT(tmp_int2) },{ CMDINT(tmp_int3) },{ CMDINT(tmp_int4)}}, 0,
                            "taskstarttimer <owner_id> <task id> <task subid> <timeoutvalue in seconds>" },
    { 9, "taskstartnofailtimer",    SCMD_TASK_STARTTIMER_NOFAIL, {{ CMDINT(tmp_int) },{ CMDINT(tmp_int2) },{ CMDINT(tmp_int3) },{ CMDINT(tmp_int4)}}, 0,
                            "Starts the task timer in no-fail mode.  If timeout value is -1, starts counting up from now.  Timeout is in seconds." },
    { 9, "taskcleartimer",    SCMD_TASK_CLEARTIMER, {{ CMDINT(tmp_int) },{ CMDINT(tmp_int2) },{ CMDINT(tmp_int3) }}, 0,
                            "Clears the task timer." },
    { 9, "showcutscene",    SCMD_SHOW_CUTSCENE, {{ 0 }}, CMDF_HIDEVARS,
                            "Shows the cutscene on the current mission" },
    { 9, "taskforcecreate",    SCMD_TASKFORCE_CREATE, {{ 0 }}, CMDF_HIDEVARS,
                            "create a basic task force (no contacts)" },
    { 9, "taskforcedestroy",SCMD_TASKFORCE_DESTROY, {{ 0 }}, CMDF_HIDEVARS,
                            "destroy your current task force" },
    { 9, "taskforcemode",    SCMD_TASKFORCE_MODE, {{ CMDINT(tmp_int) }}, CMDF_HIDEVARS,
                            "set your task force mode to x" },
    { 9, "taskforcejoin",    SCMD_TASKFORCE_JOIN, {{ CMDSTR(tmp_str) }}, 0,
                            "Add yourself to player's task force" },
    { 2, "levelup_xp",        SCMD_LEVEL_UP_XP, {{ CMDINT(tmp_int) }}, CMDF_HIDEVARS,
                            "levelup_xp <level>  Increases the character's level to the given level." },
    { 2, "levelup_wisdom",    SCMD_LEVEL_UP_WISDOM, {{ CMDINT(tmp_int) }}, CMDF_HIDEVARS,
                            "levelup_wisdom <level>  Increases the character's wisdom level to the given level." },
    { 2, "train",            SCMD_TRAIN,    {{ CMDINT(tmp_int) }}, CMDF_HIDEVARS,
                            "Train command, 0 = power, 1 = skill" },
    { 9, "spacketdebug",    SCMD_PACKETDEBUG, {{ 0 }}, 0,
                            "turns on packet debugging from client" },
    { 9, "slocale",            SCMD_LOCALE, {{ CMDSTR(tmp_str)}}, CMDF_HIDEVARS,
                            "sets the server locale" },
    { 9, "showallcontacts",    SCMD_CONTACT_SHOW, {{0}}, 0,
                            "Show all available contacts." },
    { 9, "dataminecontacts",SCMD_CONTACT_DATAMINE, {{ CMDINT(tmp_int) }}, CMDF_HIDEVARS,
                            "Dump datamining info for contacts. 0 for mission counts, 1 for more detail." },
    { 9, "showmycontacts",    SCMD_CONTACT_SHOW_MINE, {{0}}, 0,
                            "Show all contacts the player has." },
    { 2, "addcontact",        SCMD_CONTACT_ADD, {{CMDSTR(tmp_str)}}, CMDF_HIDEVARS,
                            "Add a single contact to the player, specify filename" },
    { 3, "add_all_contacts",SCMD_CONTACT_ADD_ALL, {{0}}, CMDF_HIDEVARS,
                            "Adds all contacts to the player. Used for testing and debugging only" },
    { 2, "gotocontact",        SCMD_CONTACT_GOTO, {{CMDSTR(tmp_str)}}, CMDF_HIDEVARS,
                            "Teleport immediately to the contact position, across mapservers" },
    { 2, "gotoandselectcontact",        SCMD_CONTACT_GOTO_AND_SELECT, {{CMDSTR(tmp_str)}}, CMDF_HIDEVARS,
                            "Teleport immediately to the contact position, across mapservers, then add the contact to your list and select it in the UI" },
    { 9, "contactintroductions", SCMD_CONTACT_INTRODUCTIONS, {{CMDSTR(tmp_str)}}, CMDF_HIDEVARS,
                            "Show how the specified contact will do introductions" },
    { 3, "contactintropeer", SCMD_CONTACT_INTROPEER, {{CMDSTR(tmp_str)}}, CMDF_HIDEVARS,
                            "Force the contact to introduce you to a peer" },
    { 3, "contactintronext", SCMD_CONTACT_INTRONEXT, {{CMDSTR(tmp_str)}}, CMDF_HIDEVARS,
                            "Force the contact to introduce you to the next level" },
    { 9, "contactdialog",    SCMD_CONTACT_INTERACT, {{CMDSTR(tmp_str)}}, CMDF_HIDEVARS,
                            "Talk to the specified contact." },
//     { 0, "newspaper",        SCMD_CONTACT_NEWSPAPER, {{0}}, CMDF_HIDEVARS,
//                             "Open up the newspaper." }, // Using cell phone interface for newspapers now
    { 10, "validateContacts",SCMD_ROGUE_VERIFYCONTACTS, {{0}}, CMDF_HIDEVARS,
                            "Validate all the contact data.  Give error messages where appropriate." },
    { 10, "validateMyContacts",SCMD_ROGUE_VERIFYMYCONTACTS, {{0}}, CMDF_HIDEVARS,
                            "Validate all the contact data.  Give error messages where appropriate." },
    { 10, "sendMessageIfOnAlignmentMission", SCMD_ROGUE_SENDMESSAGEIFONALIGNMENTMISSION, {{CMDINT(tmp_int)}, {CMDINT(tmp_int2)}}, CMDF_HIDEVARS,
                            "Send a message to the specified db_id (tmp_int2) if I (tmp_int) am on an Alignment mission." },
    { 10, "printMessageOnAlignmentMission", SCMD_ROGUE_PRINTMESSAGEONALIGNMENTMISSION, {{CMDINT(tmp_int)}}, CMDF_HIDEVARS,
                            "Pop up a message box telling me about the alignment mission that I can't earn points for." },
    { 9, "contactcell",        SCMD_CONTACT_CELL, {{CMDSTR(tmp_str)}}, CMDF_HIDEVARS,
                            "Talk to the specified contact on cell phone." },
    { 9, "contactgetall",    SCMD_CONTACT_GETALL, {{0}}, 0,
                            "Resend all contact status info." },
    { 9, "contactcxp",        SCMD_CONTACT_SETCXP, {{CMDSTR(tmp_str)}, {CMDINT(tmp_int)}}, CMDF_HIDEVARS,
                            "set your contact points" },
    { 9, "taskgetall",        SCMD_TASK_GETALL, {{0}}, 0,
                            "Resend all task status info." },
    { 9, "taskdetailpage",    SCMD_TASK_GETDETAIL, {{CMDINT(tmp_int)}}, CMDF_HIDEVARS,
                            "Get detail on the given task" },
    { 9, "uniquetaskinfo",    SCMD_UNIQUETASK_INFO, {{0}}, CMDF_HIDEVARS,
                            "Show info on all unique tasks" },
    { 3, "influence",        SCMD_INFLUENCE, {{CMDINT(tmp_int)}}, CMDF_HIDEVARS,
                            "Set player influence to the given number." },
    { 3, "sg_influence",    SCMD_SG_INFLUENCE, {{CMDINT(tmp_int)}}, CMDF_HIDEVARS,
                            "Set player's supergroup influence to the given number." },
    { 3, "prestige",        SCMD_PRESTIGE, {{CMDINT(tmp_int)}}, CMDF_HIDEVARS,
                            "Set player's supergroup prestige to the given number." },
    { 9, "experience",        SCMD_EXPERIENCE, {{CMDINT(tmp_int)}}, CMDF_HIDEVARS,
                            "Set player experience to the given number." },
    { 9, "xpdebt",            SCMD_XPDEBT, {{CMDINT(tmp_int)}}, CMDF_HIDEVARS,
                            "Set player experience debt to the given number." },
    { 9, "wisdom",            SCMD_WISDOM, {{CMDINT(tmp_int)}}, CMDF_HIDEVARS,
                            "Set player wisdom to the given number." },
    { 9, "showdoors",        SCMD_DOORS_SHOW, {{0}}, 0,
                            "Prints a list of all doors" },
    { 9, "showpnpcs",        SCMD_PNPC_SHOW, {{0}}, 0,
                            "Prints all the persistent npcs on this map" },
    { 9, "showvisitlocations",    SCMD_VISITLOCATIONS_SHOW, {{0}}, 0,
                            "Prints all the visit locations on this map" },
    { 9, "showmissioninfo",    SCMD_MISSION_SHOW, {{0}}, 0,
                            "Prints out mission info" },
    { 9, "showmissiondoors",SCMD_DOORS_SHOW_MISSION, {{0}}, 0,
                            "Prints a list of all mission doors" },
    { 9, "showmissiondoortypes",SCMD_DOORS_SHOW_MISSION_TYPES, {{0}}, 0,
                            "Prints a list of the mission door types available in this zone" },
    { 9, "showmissioncontact", SCMD_MISSION_SHOWFAKECONTACT, {{CMDINT(tmp_int)}}, CMDF_HIDEVARS,
                            "Shows the fake mission contact for all players (server-wide)" },
    { 9, "storyinforeset",    SCMD_STORY_INFO_RESET, {{0}}, 0,
                            "Resets the player's StoryInfo structure" },
    { 9, "storytogglecontact",  SCMD_STORY_TOGGLE_CONTACT, {{0}}, 0,
                            "Toggles between the two starting contacts for villains(Burke/Kalinda)" },
    { 9, "goingroguetipsreset", SCMD_GOING_ROGUE_TIPS_RESET, {{0}}, 0,
                            "Dismisses all tip contacts and resets all alignment point timers." },
    { 9, "pnpcreload",        SCMD_PNPC_RELOAD, {{0}}, 0,
                            "Reloads all persistent NPCs from beacons and script files" },
    { 9, "dooranimenter",    SCMD_DOORANIM_ENTER, {{0}}, 0,
                            "Find a nearby door and run the enter animation" },
    { 9, "dooranimenterarena",    SCMD_DOORANIM_ENTER_ARENA, {{0}}, 0,
                            "Act like you are heading into the arena" },
    { 9, "dooranimexit",    SCMD_DOORANIM_EXIT, {{0}}, 0,
                            "Find a nearby door and run the exit animation" },
    { 9, "dooranimexitarena",SCMD_DOORANIM_EXIT_ARENA, {{0}}, 0,
                            "Act like you are emerging onto an arena map" },
    { 9, "famestringadd",    SCMD_RANDOMFAME_ADD, {{CMDSENTENCE(tmp_str)}}, 0,
                            "Add a string to your random fame table" },
    { 9, "famestringping",    SCMD_RANDOMFAME_TEST, {{0}}, 0,
                            "See if a nearby npc will say your random fame string" },
    { 0, "emailheaders",    SCMD_EMAILHEADERS, {{0}}, CMDF_HIDEPRINT,
                            "request email headers" },
    { 0, "emailread",        SCMD_EMAILREAD, {{CMDINT(tmp_int)}}, CMDF_HIDEVARS|CMDF_HIDEPRINT,
                            "Request message <message num>" },
    { 0, "emaildelete",        SCMD_EMAILDELETE, {{CMDINT(tmp_int)}}, CMDF_HIDEVARS,
                            "Delete message <message num>" },
    { 0, "emailsend",        SCMD_EMAILSEND, {{CMDSTR(tmp_str)},{CMDSTR(tmp_str2)},{CMDSENTENCE(tmp_str3)}}, CMDF_HIDEVARS,
                            "Send message <player names> <subject> <body>" },
    { 0, "emailsendattachment",        SCMD_EMAILSENDATTACHMENT, {{CMDSTR(tmp_str)},{CMDSTR(tmp_str2)},{CMDINT(tmp_int)},{CMDINT(tmp_int2)},{CMDINT(tmp_int3)},{CMDSENTENCE(tmp_str3)}}, CMDF_HIDEVARS,
                            "Send message <player names> <subject> <body> <influence>" },
    { 3, "emailsystem",        SCMD_EMAILSYSTEM, {{CMDSTR(player_name)},{CMDSTR(tmp_str)},{CMDSTR(tmp_str2)},{CMDINT(tmp_int)},{CMDSTR(tmp_str3)},{CMDINT(tmp_int2)},}, CMDF_HIDEVARS,
                            "Send message <system name> <subject> <body> <influence> <attachment> <delay>" },
    { 9, "xpscale",            0, {{ PARSETYPE_FLOAT, &server_state.xpscale}}, 0,
                            "Scales the amount of XP awarded."},
    { 9, "influencescale",            0, {{ PARSETYPE_FLOAT, &server_state.influencescale}}, 0,
                            "Scales the amount of influence awarded."},
    { 9, "prestigescale",            0, {{ PARSETYPE_FLOAT, &server_state.prestigescale}}, 0,
                            "Scales the amount of prestige awarded."},
    { 9, "aescale",            0, {{ PARSETYPE_FLOAT, &server_state.aescale}}, 0,
                            "Scales the amount of AE XP awarded."},
    { 9, "aeinfluencescale",            0, {{ PARSETYPE_FLOAT, &server_state.aeinfluencescale}}, 0,
                            "Scales the amount of AE influence awarded."},
    { 9, "aeprestigescale",            0, {{ PARSETYPE_FLOAT, &server_state.aeprestigescale}}, 0,
                            "Scales the amount of AE prestige awarded."},
    { 9, "showobjectives",    SCMD_SHOW_OBJECTIVES, {{ 0 }}, 0,
                            "Shows a list of objectives for the mission and their current status"},
    { 9, "gotoobjective",    SCMD_GOTO_OBJECTIVE, {{ CMDINT(tmp_int) }}, CMDF_HIDEVARS,
                            "Takes the player to the specified objective"},
    { 1, "nextobjective",    SCMD_NEXT_OBJECTIVE, {{ 0 }}, 0,
                            "Takes the player to the next objective"},
    { 1, "csr_assist",        SCMD_ASSIST, {{CMDSTR(player_name)}}, CMDF_HIDEVARS,
                            "Help the given player by autotargeting whatever he targets"},
    { 0, "accesslevel",        SCMD_ACCESSLEVEL, {{ CMDINT(tmp_int) }}, CMDF_HIDEVARS|CMDF_HIDEPRINT,
                            "force me to this access level"},
    { 0, "stuck",            SCMD_STUCK, {{ 0 }}, 0,
                            "Try to get unstuck."},
    { 0, "sync",            SCMD_SYNC, {{ 0 }}, 0,
                            "Try to resync."},
    { 0, "synch",            SCMD_SYNC, {{ 0 }}, 0,
                            "Try to resync."},
    { 9, "onmissionmap",    SCMD_FORCE_MISSION_LOAD, {{ CMDINT(tmp_int) }}, CMDF_HIDEVARS,
                            "load the mapserver as if it were a mission map - for debugging" },
    { 1, "missionmapstats",    SCMD_MISSION_STATS, {{ 0 }}, 0,
                            "print some stats about mission beacons on map" },
    { 1, "missionmapstatstofile", SCMD_MISSION_STATS_TO_FILE, {{ 0 }}, 0,
                            "Save the stats about mission beacons on the map to a file." },
    { 1, "mapcheckstats",    SCMD_MAP_CHECK_STATS, {{CMDINT(tmp_int)}}, 0,
                            "checks if the current randomly generated map meets the missions.spec reqs" },
    { 1, "mapstats_checkall", SCMD_ALL_MAP_CHECK_STATS, {{0}}, 0,
                            "Checks all maps against the missions.spec"},
    { 1, "servermissioncontrol",    SCMD_MISSION_CONTROL, {{CMDINT(tmp_int)}}, 0,
                            "Request from the client for some info for Mission Control" },
    { 9, "gotospawn",        SCMD_GOTO_SPAWN, {{ CMDSTR(tmp_str) }}, CMDF_HIDEVARS,
                            "jump to specified door location on current map - POPUP ERROR MESSAGE" },
    { 9, "scanlog",            SCMD_SCAN_LOG, {{ CMDSENTENCE(tmp_str) }}, CMDF_HIDEVARS,
                            "scans the dbserver log cache for the given terms" },
    { 9, "net_profile",        SCMD_NET_PROFILE, {{ CMDINT(tmp_int) }}, 0,
                            "Dumps statistics collected in net_profile.c" },
    { 9, "nosharedmemory",    SCMD_NOSHAREDMEMORY, {{ CMDINT(tmp_int) }}, CMDF_HIDEVARS,
                            "Disables shared memory"    },
    { 9, "server_profiling_memory", 0, {{ CMDINT(server_state.profiling_memory) } }, 0,
                            "Set the number of MB of memory to use for profiling" },
    { 9, "server_profile", 0, {{ CMDINT(server_state.profile) } }, 0,
                            "Save a profile of the current frame for up to the specified number of frames" },
    { 9, "server_profile_spikes", 0, {{ CMDINT(server_state.profile_spikes) } }, 0,
                            "Save profiles of any frame longer than the specified time in ms" },
    { 1, "benpc",            SCMD_BE_VILLAIN, {{CMDSTR(tmp_str)}}, 0,
                            "Allows you to become a villain or npc" },
    { 1, "beself",            SCMD_BE_SELF, {{0}}, 0,
                            "Switches you to your normal model and costume" },
    { 0, "afk",                SCMD_AFK, {{ CMDSENTENCE(afk) }}, CMDF_HIDEVARS | CMDF_RETURNONERROR,
                            "Marks the player as Away From Keyboard (with given message)" },
    { 9, "becritter",        SCMD_BE_VILLAIN_FOR_REAL, {{ CMDSTR(tmp_str) }, { CMDINT(tmp_int) }}, CMDF_HIDEVARS,
                            "Get the costume and powers of a villain of the given level." },
    { 0, "copydebuginfo",    SCMD_DEBUG_COPYINFO, {{ 0 }}, CMDF_HIDEPRINT,
                            "Gather a bunch of debug info, print it, and copy it to the clipboard" },
    { 1, "debug",            SCMD_DEBUG, {{ CMDSTR(player_name)}}, CMDF_HIDEVARS,
                            "print out basic player info"},
    { 1, "debug_detail",    SCMD_DEBUG_DETAIL, {{ CMDSTR(player_name)}}, CMDF_HIDEVARS, // entdebug, dump entity info.
                            "print out detailed player info"},
    { 1, "debug_tray",        SCMD_DEBUG_TRAY, {{ CMDSTR(player_name)}}, CMDF_HIDEVARS,
                            "print out a player's tray contents"},
    { 1, "debug_spec",        SCMD_DEBUG_SPECIALIZATIONS, {{ CMDSTR(player_name)}}, CMDF_HIDEVARS,
                            "print out a player's specializations"},
    { 1, "goto",            SCMD_GOTO, {{ CMDSENTENCE(player_name)}}, CMDF_HIDEVARS,
                            "move yourself to a player's map and location"},
    { 9, "goto_internal",    SCMD_GOTO_INTERNAL, {{ CMDSTR(player_name)}, {CMDSTR(tmp_str)}}, CMDF_HIDEVARS,
                            "helper function for /goto"},
    { 9, "editmessage",        SCMD_EDITMESSAGE, {{CMDSTR(tmp_str)}}, CMDF_HIDEVARS,
                            "Sends an edit message to the client (for output in scripts/macros)"},
    { 1, "gotomission",        SCMD_GOTOMISSION, {{ 0}}, CMDF_HIDEVARS,
                            "Teleport to mission entrance. Will work across maps." },
    { 9, "testmission",        SCMD_TESTMISSION, {{CMDSTR(tmp_str)}}, CMDF_HIDEVARS,
                            "Get a specific task & teleports to mission door, if possible. (Combines 'gettaskqa' and 'gotomission')" },
    { 9, "stats",            SCMD_STATS, {{CMDSTR(player_name)}}, CMDF_HIDEVARS,
                            "Shows a player's stats" },
    { 9, "clearstats",        SCMD_CLEAR_STATS, {{CMDSTR(player_name)}}, CMDF_HIDEVARS,
                            "Clears a player's stats" },
    { 0, "kiosk",            SCMD_KIOSK, {{CMDSTR(tmp_str)}, {CMDSTR(tmp_str2)}}, CMDF_HIDEVARS,
                            "Pop up the kiosk info for the nearest kiosk. (Assuming you're close enough.)" },
    { 0, "nojumprepeat",    SCMD_NOJUMPREPEAT, {{ CMDINT(tmp_int) }}, CMDF_HIDEVARS,
                            "Disable jump auto-repeat" },
    { 9, "missionx",        SCMD_MISSIONX, {{0}}, CMDF_HIDEVARS,
                            "Assign yourself a random mission of a compatible status level. (Level 1-5 == Status Level 1, 6-10 == 2, etc)" },
    { 9, "missionxmap",        SCMD_MISSIONXMAP, {{0}}, CMDF_HIDEVARS,
                            "Assign yourself a random mission that requires transfer to an instance map. Also see 'missionx'." },
    { 3, "scget",            SCMD_SOUVENIRCLUE_GET, {{CMDSTR(tmp_str)}}, CMDF_HIDEVARS,
                            "Give the user the specified souvenir clue" },
    { 9, "scremove",        SCMD_SOUVENIRCLUE_REMOVE, {{CMDSTR(tmp_str)}}, CMDF_HIDEVARS,
                            "Remove the specified souvenir clue from user, for debugging only" },
    { 10, "scapply",        SCMD_SOUVENIRCLUE_APPLY, {{CMDINT(tmp_int)}, {CMDSTR(tmp_str)}}, CMDF_HIDEVARS,
                            "(internal) Give souvenir clue to db_id" },
    { 9, "debug_power",        SCMD_DEBUG_POWER, {{CMDSTR(tmp_str)},{CMDSTR(tmp_str2)},{CMDSTR(tmp_str3)}}, CMDF_HIDEVARS,
                            "show the power's enhancement results" },
    { 0, "teamtask",        SCMD_TEAMTASK_SET, {{CMDINT(tmp_int)}, {CMDINT(tmp_int2)}, {CMDINT(tmp_int3)}}, CMDF_HIDEVARS|CMDF_HIDEPRINT,
                            "Set the team task" },
    { 0, "abandontask",        SCMD_TEAMTASK_ABANDON, {{CMDINT(tmp_int)}, {CMDINT(tmp_int2)}}, CMDF_HIDEVARS|CMDF_HIDEPRINT,
                            "Abandon a task" },
    { 0, "petition_internal", SCMD_PETITION_INTERNAL, {{CMDSENTENCE(tmp_str)}}, CMDF_HIDEVARS|CMDF_HIDEPRINT,
                            "internal command used by Petition Window for submission" },
    { 2, "title_1",            SCMD_TITLE_1, {{CMDSENTENCE(tmp_str)}}, CMDF_HIDEVARS,
                            "Set the character's first title" },
    { 2, "title_2",            SCMD_TITLE_2, {{CMDSENTENCE(tmp_str)}}, CMDF_HIDEVARS,
                            "Set the character's second title" },
    { 2, "title_the",        SCMD_TITLE_THE, {{CMDINT(tmp_int)}, {CMDINT(tmp_int2)}, {CMDSENTENCE(tmp_str)}}, CMDF_HIDEVARS,
                            "Set the character's The setting" },
    { 2, "title_special",    SCMD_TITLE_SPECIAL, {{CMDSENTENCE(tmp_str)}}, CMDF_HIDEVARS,
                            "Set the character's special, free-form title" },
    { 2, "title_expires",    SCMD_TITLE_SPECIAL_EXPIRES, {{PARSETYPE_FLOAT, &tmp_float}}, CMDF_HIDEVARS,
                            "When does the special title expire in hours." },
    { 2, "title_and_expires_special",    SCMD_TITLE_AND_EXPIRES_SPECIAL, {{CMDSTR(tmp_str)}, {PARSETYPE_FLOAT, &tmp_float}}, CMDF_HIDEVARS,
                            "Set the character's special, free-form title and expiration duration" },
    { 2, "csr_title_and_expires_special",    SCMD_CSR_TITLE_AND_EXPIRES_SPECIAL, {{CMDSTR(tmp_str)},{CMDSTR(tmp_str2)}, {PARSETYPE_FLOAT, &tmp_float}}, CMDF_HIDEVARS,
                            "Set the character's special, free-form title and expiration duration" },
    { 3, "fx_special",        SCMD_FX_SPECIAL, {{CMDSENTENCE(tmp_str)}}, CMDF_HIDEVARS,
                            "Set the character's special costume FX" },
    { 3, "fx_expires",        SCMD_FX_SPECIAL_EXPIRES, {{PARSETYPE_FLOAT, &tmp_float}}, CMDF_HIDEVARS,
                            "When does the special costume FX expire in hours." },
    { 9, "safe_player_count",    SCMD_SAFE_PLAYER_COUNT, {{CMDINT(server_state.safe_player_count)}}, 0,
                            "The count of players that causes the server to degrade itself." },
    { 9, "saccessviolation", 0, {{PARSETYPE_S32, (void*)1}}, 0,
                            "Cause the server to crash with an accessviolation" },
    { 9, "delinkme",        SCMD_DELINKME, {0}, CMDF_HIDEVARS,
                            "delinks this map from dbserver." },
    { 4, "mapshutdown",        SCMD_MAP_SHUTDOWN, {CMDSENTENCE(tmp_str)}, CMDF_HIDEVARS,
                            "Emergency shutdown command, attempts to save all current entities and shut down the map." },
    { 9, "collrecord",        SCMD_COLLRECORD, {0}, CMDF_HIDEVARS,
                            "start recording collision queries." },
    { 9, "collrecordstop",    SCMD_COLLRECORDSTOP, {0}, CMDF_HIDEVARS,
                            "stop recording collision queries." },
    { 9, "nextdoor",        SCMD_NEXT_DOOR, {{ 0 }}, CMDF_HIDEVARS,
                            "Cycle through doors on current map"},
    { 9, "nextmissiondoor",    SCMD_NEXT_MISSIONDOOR, {{ 0 }}, CMDF_HIDEVARS,
                            "Cycle through all mission doors on current map."},
    { 9, "showtaskdoor",    SCMD_SHOW_TASKDOOR, {{ CMDSTR(tmp_str) }}, CMDF_HIDEVARS,
                            "Show all of the potential doors for a task."},
    { 9, "teamlogdump",        SCMD_TEAMLOG_DUMP, {0}, CMDF_HIDEVARS,
                            "Dump the team log to the debug window" },
    { 9, "teamlogecho",        SCMD_TEAMLOG_ECHO, {{CMDINT(tmp_int)}}, CMDF_HIDEVARS,
                            "Start echoing the team log to the server console" },
    { 9, "trickortreat",    SCMD_TRICK_OR_TREAT, {{CMDINT(tmp_int)}}, CMDF_HIDEVARS,
                            "Set Trick-or-Treat mode" },
    { 9, "s_timeoffsetsecs",    0, {{CMDINT(timing_debug_offset)}}, 0,
                            "Seconds to offset the timing functions ON THE MAP from the real time (debug)" },
    { 9, "s_timeoffset",    SCMD_TIMEOFFSET, {{CMDSTR(tmp_str)}},0,
                            "set the mapserver time offset with the format days:hours:minutes:seconds.  If you want to set the offset to 2 hours, 3 minutes, you'd use s_timeoffset 0:2:3:0" },
    { 9, "s_timedebug",        SCMD_TIMEDEBUG, {{ 0 }}, CMDF_HIDEVARS,
                            "Get debug information on the timing offset ON THE MAP (debug)" },
    { 9, "timeset",            SCMD_TIMESET, {{ PARSETYPE_FLOAT, &tmp_float }},0,
                            "sets current time of day for all maps" },
    { 9, "timesetall",        SCMD_TIMESETALL, {{ PARSETYPE_FLOAT, &tmp_float }},0,
                            "sets current time of day for all maps" },
    { 9, "playermorph",        SCMD_PLAYER_MORPH, {{CMDSTR(tmp_str)}, {CMDSTR(tmp_str2) }, {CMDSTR(tmp_str3) }, { CMDINT(tmp_int) }},0,
                            "Changes a player into another common character.\nSpecify the AT, Primary Power, Secondary Power, level." },
    { 9, "packageent",        SCMD_REQUEST_PACKAGEDENT, {0},0,
                            "Packages and saves the player into a local file so that the player can be that can be reloaded later." },
    { 2, "playerrename",    SCMD_PLAYER_RENAME, {{CMDSTR(player_name)}, {CMDSTR(tmp_str) }},0,
                            "Changes a player's name.\nSpecify the current name first, then the new name." },
    { 2, "set_player_rename_token",    SCMD_SET_PLAYER_RENAME_TOKEN, {CMDINT(tmp_int)},0,
                            "Set this player's rename token state: 1 on 0 off" },
    { 2, "unlock_character",    SCMD_UNLOCK_CHARACTER, {0},0,
                            "Remove the slot lock on a character" },
    { 3, "adjust_server_slots",    SCMD_ADJUST_SERVER_SLOTS, {{ CMDINT(tmp_int) }, { CMDINT(tmp_int2) }},0,
                            "Adjust the number of  on a character" },
    { 1, "playerrename_paid",    SCMD_PLAYER_RENAME_PAID, {{ CMDSTR(tmp_str)}, {CMDINT(tmp_int)}, {CMDSTR(player_name)}, {CMDSTR(tmp_str2) }}, 0,
                            "Changes a player's name (if they have a rename token).\nSpecify the current name first, then the new name." },
    { 3, "playerrename_reserved",    SCMD_PLAYER_RENAME_RESERVED, {{CMDSTR(player_name)}, {CMDSTR(tmp_str) }},CMDF_HIDEVARS|CMDF_HIDEPRINT,
                            "Changes a player's name (allows using reserved names).\nSpecify the current name first, then the new name." },
    { 9, "playerrenamerelay",SCMD_PLAYER_RENAME_RELAY, {{ CMDINT(tmp_int) }, {CMDSTR(tmp_str)}, { CMDINT(tmp_int2) }, { CMDINT(tmp_int3)} },0,
                            "Internal command (player_id, new_name, admin_id, allow_reserved)" },
    { 9, "check_player_name",    SCMD_CHECK_PLAYER_NAME, {{CMDINT(tmp_int)}, {CMDINT(tmp_int2)}, {CMDSTR(tmp_str)}}, CMDF_HIDEVARS|CMDF_HIDEPRINT,
                            "Check whether a player name is in use or not" },
    { 9, "order_rename",    SCMD_ORDER_RENAME, {{CMDSTR(tmp_str)}}, CMDF_HIDEVARS|CMDF_HIDEPRINT,
                            "AccountServer command" }, // this should probably be moved to cmdaccountserver.c
    { 9, "order_respec",    SCMD_ORDER_RESPEC, {{CMDSTR(tmp_str)}}, CMDF_HIDEVARS|CMDF_HIDEPRINT,
                            "AccountServer command" }, 
    { 2, "sgrename",        SCMD_SGROUP_RENAME, {{CMDSTR(player_name)}, {CMDSTR(tmp_str)}, {CMDSTR(tmp_str2)}},0,
                            "Changes a supergroup's name.\nSpecify the name of a player in the supergroup, the old supergroup name, then the new name of the supergroup." },
    { 9, "sgrenamerelay",    SCMD_SGROUP_RENAME_RELAY, {{ CMDINT(tmp_int) }, {CMDSTR(tmp_str)}, {CMDSTR(tmp_str2)}, { CMDINT(tmp_int2) }},0,
                            "Internal command (player_id, old_sg_name, new_sg_name, admin_id)" },
    { 9, "arena_join",        SCMD_ARENA_JOIN, {{ CMDINT(tmp_int) }, { CMDINT(tmp_int2) }}, CMDF_HIDEVARS,
                            "Join an arena event" },
    { 9, "arena_create",    SCMD_ARENA_CREATE, {{ CMDSTR(tmp_str) }}, CMDF_HIDEVARS,
                            "Create a new named arena event" },
    { 9, "arena_destroy",    SCMD_ARENA_DESTROY, {{ CMDINT(tmp_int) }, { CMDINT(tmp_int2) }}, CMDF_HIDEVARS,
                            "Destroy an existing arena event" },
    { 9, "arena_setside",    SCMD_ARENA_SETSIDE, {{ CMDINT(tmp_int) }, { CMDINT(tmp_int2) }, { CMDINT(tmp_int3) }}, CMDF_HIDEVARS,
                            "Set what side player is for this event" },
    { 9, "arena_ready",        SCMD_ARENA_READY, {{ CMDINT(tmp_int) }, { CMDINT(tmp_int2) }, { CMDINT(tmp_int3) }}, CMDF_HIDEVARS,
                            "Set player ready state for an event" },
    { 9, "arena_setmap",    SCMD_ARENA_SETMAP, {{ CMDINT(tmp_int) }, { CMDINT(tmp_int2) }, { CMDSTR(tmp_str) }}, CMDF_HIDEVARS,
                            "Set the map for an event" },
    { 9, "arena_camera",    SCMD_ARENA_CAMERA, {{ 0    }}, 0,
                            "Test command that turns the player into a camera"},
    { 9, "arena_uncamera",    SCMD_ARENA_UNCAMERA, {{ 0    }}, 0,
                            "Test command that turns the player back from being a camera"},
    { 9, "arena_enter",        SCMD_ARENA_ENTER, {{ CMDINT(tmp_int) }, { CMDINT(tmp_int2) }}, CMDF_HIDEVARS,
                            "Starts the event and enters this player" },
    { 9, "arena_popup",        SCMD_ARENA_POPUP, {{ CMDINT(tmp_int) }}, CMDF_HIDEVARS,
                            "Pop up the event's create window"},
    { 9, "arena_debug",        SCMD_ARENA_DEBUG, {{ 0 }}, 0,
                            "Show detail information for each event you belong to"},
    { 2, "arena_player_stats",    SCMD_ARENA_PLAYER_STATS, {{ 0 }}, 0,
                            "Show arena stats (wins, losses, etc"},
    { 0, "arena_list",        SCMD_ARENA_KIOSK, {{ 0 }}, 0,
                            "Open the arena list window no matter where you are.  Opens the score window if you are in an Arena match."},
    { 0, "arena_score",        SCMD_ARENA_SCORE, {{ 0 }}, 0,
                            "Open the arena score window if you are in an Arena match.  Does nothing if you aren't in an Arena match."},
    { 9, "sgraid_length",    SCMD_SGRAID_LENGTH, { {CMDINT(tmp_int)} }, 0,
                            "Set the time for the supergroup raid"},
    { 9, "sgraid_size",        SCMD_SGRAID_SIZE, { {CMDINT(tmp_int)} }, 0,
                            "Set the time for the force field to respawn"},
    { 9, "sgraid_challenge",SCMD_SGRAID_CHALLENGE, { {CMDSTR(player_name)} }, 0,
                            "Challenge the given supergroup to the next available time slot"},
    { 9, "sgraid_challenge_internal",SCMD_SGRAID_CHALLENGE_INTERNAL, { {CMDINT(tmp_int)} }, 0,
                            "Challenge the given supergroup to the next available time slot"},
    { 9, "sgraid_warp",        SCMD_SGRAID_WARP, { {CMDINT(tmp_int)} }, 0,
                            "Test warping to an attacker location in a supergroup base"},
    { 9, "sgraid_setvar",    SCMD_SGRAID_SETVAR, { { CMDSTR(tmp_str)}, {CMDSTR(tmp_str2)} }, 0,
                            "Set any var on the raidserver"},
    { 9, "sgraid_stat",        SCMD_SGRAID_STAT, { { 0 } }, 0,
                            "Look at what mapserver is getting for your raid stats"},
    { 9, "sgraid_baseinfo",    SCMD_SGRAID_BASEINFO, { { CMDINT(tmp_int) }, { CMDINT(tmp_int2) } }, 0,
                            "Manually set raid size & mount availability, sgraid_baseinfo <raidsize> <openmount?>"},
    { 0, "sgraid_window",    SCMD_SGRAID_WINDOW, { { CMDINT(tmp_int) }, { CMDINT(tmp_int2) } }, CMDF_HIDEVARS,
                            "Set your supergroup raid window <daybits> <hour>"},
    { 3, "csrbug_internal",    SCMD_CSR_BUG_INTERNAL, {{ CMDSTR(player_name)}, {CMDSTR(tmp_str)}, {CMDSTR(tmp_str2)}}, CMDF_HIDEVARS,
                            "Internal command for processing /csrbug command"},
    { 3, "powers_cashin",    SCMD_POWERS_CASH_IN, {{ 0 }},0,
                            "Cashes in all enhancements a player has slotted and removes them from all powers. Keeps inventory enhancements." },
    { 3, "powers_reset",    SCMD_POWERS_RESET, {{ CMDSTR(tmp_str)}, {CMDSTR(tmp_str2)}, {CMDSTR(tmp_str3) }},CMDF_RETURNONERROR,
                            "Removes all of a player's powers and sets their primary and secondary power sets. Player can go to trainer and train back up to their level. ([primary set] [first power] [secondary set])" },
    { 9, "powers_buypower_dev",    SCMD_POWERS_BUY_POWER_DEV, {{ CMDSTR(tmp_str)}, {CMDSTR(tmp_str2) }, {CMDSTR(tmp_str3) }},0,
                            "Developer version of powers_buypower.  Buys a power for the character. ([category] [power set] [power])" },
    { 9, "powers_revoke_dev", SCMD_POWERS_REVOKE_DEV, {{ CMDSTR(tmp_str)}, {CMDSTR(tmp_str2) }, {CMDSTR(tmp_str3) }},0,
                            "Revoke specified power from the character. ([category] [power set] [power])" },
    { 9, "powers_cancel_dev", SCMD_POWERS_CANCEL_DEV, {{ CMDINT(tmp_int) }, {CMDSTR(tmp_str)}},0,
                            "Cancel all effects of specified power from the character. ([ent id] [category.powerset.power])" },
    { 0, "powers_cancel",    SCMD_POWERS_CANCEL, {{ CMDINT(tmp_int) }, {CMDSTR(tmp_str)}},0,
                            "Cancel all effects of specified power from the character if power is cancelable and target is you or your pet. ([ent id] [category.powerset.power])" },
    { 9, "autoenhance",        SCMD_AUTOENHANCE, {{ 0 }},0,
                            "Place Standard enhancements in all slots" },
    { 9, "autoenhanceIO",    SCMD_AUTOENHANCEIO, {{ 0 }},0,
                            "Try to place IO's in all slots" },
    { 9, "autoenhanceset",    SCMD_AUTOENHANCESET, {{ 0 }},0,
                            "Try to place Invention sets in all slots" },
    { 9, "maxslots",        SCMD_MAXBOOSTS, {{ 0 }},0,
                            "Get every slot possible" },
    { 9, "powers_recalc",    SCMD_RECALC_STR, {{ 0 }},0,
                            "Force all ents to recalc their power strengths" },
    { 9, "setPowerDiminish",SCMD_SET_POWERDIMINISH, {{ CMDINT(server_state.enablePowerDiminishing)}},0,
                            "enable/disable power diminishing [on/off]" },
    { 9, "showPowerDiminish",SCMD_SHOW_POWERDIMINISH, {{ 0 }},0,
                            "display current state of power diminishing" },

    { 9, "showPowerTargetInfo",    SCMD_SHOW_POWER_TARGET_INFO, {{ CMDSTR(tmp_str)}, {CMDSTR(tmp_str2) }, {CMDSTR(tmp_str3) }},0,
                            "Developer version of powers_buypower.  Buys a power for the character. ([category] [power set] [power])" },
    { 3, "powers_buypower",    SCMD_POWERS_BUY_POWER, {{ CMDSTR(tmp_str)}, {CMDSTR(tmp_str2) }, {CMDSTR(tmp_str3) }},0,
                            "Buys a power for the character. ([category] [power set] [power])" },
    { 2, "powers_check",    SCMD_POWERS_CHECK, {{ 0 }}, 0,
                            "Does a check on a player's powers and triesm to determine if they're sane. Suggest fixes if they aren't" },
    { 1, "powers_info",        SCMD_POWERS_INFO, {{0}}, 0,
                            "Lists all powers and the level they were bought at." },
    { 1, "powers_show",        SCMD_POWERS_INFO, {{0}}, 0,
                            "Lists all powers and the level they were bought at." },
    { 1, "powers_info_build",        SCMD_POWERS_INFO_BUILD, {{CMDINT(tmp_int)}}, 0,
                            "Lists all powers and the level they were bought at." },
    { 1, "powers_show_build",        SCMD_POWERS_INFO_BUILD, {{CMDINT(tmp_int)}}, 0,
                            "Lists all powers and the level they were bought at." },
    { 3, "powers_set_level",SCMD_POWERS_SET_LEVEL, {{CMDSTR(tmp_str)},{CMDSTR(tmp_str2)},{CMDSTR(tmp_str3)},{CMDINT(tmp_int)}}, 0,
                            "Sets the level the given power was bought at to the given value. [cat] [set] [pow] [level]" },
    { 3, "power_set_level",SCMD_POWERS_SET_LEVEL, {{CMDSTR(tmp_str)},{CMDSTR(tmp_str2)},{CMDSTR(tmp_str3)},{CMDINT(tmp_int)}}, 0,
                            "Sets the level the given power was bought at to the given value. [cat] [set] [pow] [level]" },
    { 4, "influence_add",    SCMD_INFLUENCE_ADD, {{CMDINT(tmp_int)}}, CMDF_HIDEVARS,
                            "Adds the given influence." },
    { 3, "cs_pactmember_inf_add",    SCMD_PACTMEMBER_INFLUENCE_ADD, {{CMDINT(tmp_int)}}, CMDF_HIDEVARS,
                            "Adds the given influence or infamy." },
    { 9, "pactmember_inf_add",    SCMD_PACTMEMBER_INFLUENCE_ADD, {{CMDINT(tmp_int)}}, CMDF_HIDEVARS,
                            "Adds the given influence or infamy." },
    { 9, "pactmember_experience_get",    SCMD_PACTMEMBER_EXPERIENCE_GET, {{CMDINT(tmp_int)}}, CMDF_HIDEVARS,
                            "<internal> adds player's full xp to a pact" },
    { 9, "levelingpact_exit",    SCMD_LEVELINGPACT_EXIT, {{CMDINT(tmp_int)}, {CMDINT(tmp_int2)}}, CMDF_HIDEVARS,
                            "Adds the given experience and updates the level time of a member of a leveling pact even if the pact has already been dissolved." },
    { 3, "sg_influence_add",    SCMD_SG_INFLUENCE_ADD, {{CMDINT(tmp_int)}}, CMDF_HIDEVARS,
                            "Adds the given influence to player's supergroup." },
    { 9, "power_color_p1",    SCMD_POWER_COLOR_P1, {{CMDINT(tmp_int)}, {CMDINT(tmp_int2)}, {CMDINT(tmp_int3)}}, CMDF_HIDEVARS,
                            "Set Primary powerset first color." },
    { 9, "pcp1",            SCMD_POWER_COLOR_P1, {{CMDINT(tmp_int)}, {CMDINT(tmp_int2)}, {CMDINT(tmp_int3)}}, CMDF_HIDEVARS,
                            "Set Primary powerset first color." },
    { 9, "power_color_p2",    SCMD_POWER_COLOR_P2, {{CMDINT(tmp_int)}, {CMDINT(tmp_int2)}, {CMDINT(tmp_int3)}}, CMDF_HIDEVARS,
                            "Set Primary powerset second color." },
    { 9, "pcp2",            SCMD_POWER_COLOR_P2, {{CMDINT(tmp_int)}, {CMDINT(tmp_int2)}, {CMDINT(tmp_int3)}}, CMDF_HIDEVARS,
                            "Set Primary powerset second color." },
    { 9, "power_color_s1",    SCMD_POWER_COLOR_S1, {{CMDINT(tmp_int)}, {CMDINT(tmp_int2)}, {CMDINT(tmp_int3)}}, CMDF_HIDEVARS,
                            "Set Secondary powerset first color." },
    { 9, "pcs1",            SCMD_POWER_COLOR_S1, {{CMDINT(tmp_int)}, {CMDINT(tmp_int2)}, {CMDINT(tmp_int3)}}, CMDF_HIDEVARS,
                            "Set Secondary powerset first color." },
    { 9, "power_color_s2",    SCMD_POWER_COLOR_S2, {{CMDINT(tmp_int)}, {CMDINT(tmp_int2)}, {CMDINT(tmp_int3)}}, CMDF_HIDEVARS,
                            "Set Secondary powerset second color." },
    { 9, "pcs2",            SCMD_POWER_COLOR_S2, {{CMDINT(tmp_int)}, {CMDINT(tmp_int2)}, {CMDINT(tmp_int3)}}, CMDF_HIDEVARS,
                            "Set Secondary powerset second color." },
                        
    { 3, "prestige_add",    SCMD_PRESTIGE_ADD, {{CMDINT(tmp_int)}}, CMDF_HIDEVARS,
                            "Adds the given prestige to player's supergroup." },
    { 4, "experience_add",    SCMD_EXPERIENCE_ADD, {{CMDINT(tmp_int)}}, CMDF_HIDEVARS,
                            "Adds the given experience." },
    { 4, "xpdebt_remove",    SCMD_XPDEBT_REMOVE, {{CMDINT(tmp_int)}}, CMDF_HIDEVARS,
                            "Removes the given experience debt." },
    { 4, "wisdom_add",        SCMD_WISDOM_ADD, {{CMDINT(tmp_int)}}, CMDF_HIDEVARS,
                            "Adds the given wisdom." },
    { 4, "reputation",        SCMD_REPUTATION, {{PARSETYPE_FLOAT, &tmp_float}}, CMDF_HIDEVARS,
                            "Sets your reputation." },
    { 4, "reputation_add",    SCMD_REPUTATION_ADD, {{PARSETYPE_FLOAT, &tmp_float}}, CMDF_HIDEVARS,
                            "Adds the given reputation." },
    { 0, "shardcomm",        SCMD_SHARDCOMM, {{CMDSENTENCE(tmp_str)}}, CMDF_HIDEVARS|CMDF_HIDEPRINT,
                            "send string to shardcomm" },
    { 2, "badge_show",        SCMD_BADGE_SHOW, {{0}}, 0,
                            "Lists the badges owned by the player." },
    { 2, "sgrp_badge_show", SCMD_SGRP_BADGE_SHOW, {{0}}, 0,
                            "Lists the badges owned by the player's supergroup." },
    { 2, "badges_show",        SCMD_BADGE_SHOW, {{0}}, 0,
                            "Lists the badges owned by the player. (synonym for badge_show)" },
    { 2, "badge_show_all",    SCMD_BADGE_SHOW_ALL, {{0}}, 0,
                            "Lists all badges, with the badges owned by the player marked." },
    { 2, "badges_show_all",    SCMD_BADGE_SHOW_ALL, {{0}}, 0,
                            "Lists all badges, with the badges owned by the player marked. (synonym for badge_show_all)" },
    { 2, "badge_add",        SCMD_BADGE_GRANT, {{CMDSTR(tmp_str)}}, 0,
                            "Gives the named badge to the player." },
    { 2, "badge_grant",        SCMD_BADGE_GRANT, {{CMDSTR(tmp_str)}}, 0,
                            "Gives the named badge to the player." },
    { 2, "badge_grant_bits",SCMD_BADGE_GRANT_BITS, {{CMDSTR(tmp_str)}}, 0,
                            "set the badges owned by the player. a string of hex values: 0f13c...." },
    { 2, "badge_show_bits",SCMD_BADGE_SHOW_BITS, {{0}}, 0,
                            "show the badges owned by the player. a string of hex values: 0f13c...." },
    { 9, "badge_add_all",    SCMD_BADGE_GRANT_ALL, {0}, 0,
                            "Gives all badges to the player." },
    { 9, "badge_grant_all",    SCMD_BADGE_GRANT_ALL, {0}, 0,
                            "Gives all badges to the player." },
    { 9, "badge_remove_all", SCMD_BADGE_REVOKE_ALL, {0}, 0,
                            "Removes all badges from the player." },
    { 9, "badge_revoke_all", SCMD_BADGE_REVOKE_ALL, {0}, 0,
                            "Removes all badges from the player." },
    { 2, "badge_remove",    SCMD_BADGE_REVOKE, {{CMDSTR(tmp_str)}}, 0,
                            "Removes the named badge from the player." },
    { 2, "badge_revoke",    SCMD_BADGE_REVOKE, {{CMDSTR(tmp_str)}}, 0,
                            "Removes the named badge from the player." },
    { 2, "badge_stat_show",    SCMD_BADGE_STAT_SHOW, {{CMDSTR(tmp_str)}}, CMDF_RETURNONERROR,
                            "Show all of the basic stats tracked for badges for the player. Optional parameter shows stats with the given string in the stat name." },
    { 2, "badge_stats_show",    SCMD_BADGE_STAT_SHOW, {{CMDSTR(tmp_str)}}, CMDF_RETURNONERROR,
                            "Show all of the basic stats tracked for badges for the player. Optional parameter shows stats with the given string in the stat name. (synonym of badge_stat_show)" },
    { 2, "badge_stat_set",    SCMD_BADGE_STAT_SET, {{CMDSTR(tmp_str)}, {CMDINT(tmp_int)}}, 0,
                            "Sets the given basic badge stat for the player." },
    { 2, "badge_stat_add",    SCMD_BADGE_STAT_ADD, {{CMDSTR(tmp_str)}, {CMDINT(tmp_int)}}, 0,
                            "Adds the given value to the given basic badge stat for the player." },
    { 9, "badge_stat_add_relay",SCMD_BADGE_STAT_ADD_RELAY, {{CMDSTR(tmp_str)}, {CMDINT(tmp_int)}, {CMDINT(tmp_int2)}}, 0,
                            "Adds the given value to the given basic badge stat for the player." },
    { 0, "badge_button_use",SCMD_BADGE_BUTTON_USE, {{CMDINT(tmp_int)}}, CMDF_HIDEVARS|CMDF_RETURNONERROR|CMDF_HIDEPRINT,
                            "Claim a badge button reward." },
    { 2, "Architect_Token_Grant", SCMD_ARCHITECT_TOKEN, {{CMDSTR(tmp_str)}}, 0,
                            "Grant a mission server token to the player" },
    { 2, "Architect_Slot_Grant", SCMD_ARCHITECT_SLOT, {{CMDINT(tmp_int)}}, 0,
                            "Grant X publish slots to the player" },
    // Chatserver commands 
    { 0, "gfriend",            SCMD_SHARDCOMM_FRIEND, {{CMDSENTENCE(player_name)}}, CMDF_HIDEVARS,
                            "Add a player to your global friends list."},
    { 0, "gunfriend",        SCMD_SHARDCOMM_UNFRIEND, {{CMDSENTENCE(player_name)}}, CMDF_HIDEVARS,
                            "Remove a player from your global friends list."},
    { 0, "chan_invite",        SCMD_SHARDCOMM_CHAN_INVITE, {{CMDSTR(tmp_str)}, { CMDSENTENCE(player_name)}}, CMDF_HIDEVARS,
                            "Invite player or chat handle to a chat channel\n"
                            "Syntax: chan_invite <CHANNEL NAME> <USER NAME>" },
    { 0, "ginvite",            SCMD_SHARDCOMM_CHAN_INVITE, {{CMDSTR(tmp_str)}, { CMDSENTENCE(player_name)}}, CMDF_HIDEVARS,
                            "Invite player or chat handle to a chat channel\n"
                            "Syntax: ginvite <CHANNEL NAME> <USER NAME>" },
    { 0, "chan_invite_sg",    SCMD_SHARDCOMM_CHAN_INVITE_SG, {{CMDSTR(tmp_str)}, {CMDINT(tmp_int)}}, CMDF_HIDEVARS,
                            "Invite your entire supergroup to a chat channel. Only leaders may use this command.\n"
                            "You must specify the minimum rank to invite:\n"
                            " 0 - invite entire supergroup (members, captains and leaders)\n"
                            " 1 - invite captains and leaders only"
                            " 2 - invite leaders only\n"
                            "Syntax: chan_invite_sg <CHANNEL NAME> <RANK>" },
    { 0, "ginvite_sg",        SCMD_SHARDCOMM_CHAN_INVITE_SG, {{CMDSTR(tmp_str)}, {CMDINT(tmp_int)}}, CMDF_HIDEVARS,
                            "Invite your entire supergroup to a chat channel. Only leaders may use this command.\n"
                            "You must specify the minimum rank to invite:\n"
                            " 0 - invite entire supergroup (members, captains and leaders)\n"
                            " 1 - invite captains and leaders only"
                            " 2 - invite leaders only\n"
                            "Syntax: ginvite_sg <CHANNEL NAME> <RANK>" },
    { 0, "chan_send",        SCMD_SHARDCOMM_SEND, {{CMDSTR(tmp_str)}, { CMDSENTENCE(tmp_str2)}}, CMDF_HIDEVARS,
                            "Send message to chat channel. You must be in the channel and have Send priviledges.\n"
                            "(alias is \"/send\")\n"
                            "Syntax: chan_send <CHANNEL NAME> <MESSAGE>" },
    { 0, "send",            SCMD_SHARDCOMM_SEND, {{CMDSTR(tmp_str)}, { CMDSENTENCE(tmp_str2)}}, CMDF_HIDEVARS,
                            "Send message to chat channel. You must be in the channel and have Send priviledges\n"
                            "Syntax: send <CHANNEL NAME> <MESSAGE>" },
    { 0, "gmotd",            SCMD_SHARDCOMM_GMOTD, {{0}}, CMDF_HIDEVARS,
                            "View the global message again." },
    { 0, "get_global_name", SCMD_SHARDCOMM_GETGLOBAL, {{CMDSENTENCE(tmp_str)}}, 0,
                            "Gets the global name from character name." },
    { 0, "get_global_silent",    SCMD_SHARDCOMM_GETGLOBALSILENT, {{CMDSENTENCE(tmp_str)}}, 0,
                            "Gets the global name without reporting results to chat window" },
    { 0, "gmail_claim",        SCMD_SHARDCOMM_GMAILCLAIM, {{CMDINT(tmp_int)}}, 0, 
                            "Claim Attachments on a Global Email" },
    { 0, "gmail_return",    SCMD_SHARDCOMM_GMAILRETURN, {{CMDINT(tmp_int)}}, 0, 
                            "Claim Attachments on a Global Email" },
    { 2, "chan_kill",        SCMD_SHARDCOMM_CSR_KILLCHANNEL, {{CMDSENTENCE(tmp_str)}}, CMDF_HIDEVARS,
                            "Shutdown a chat channel"},
    { 2, "handlerename",    SCMD_SHARDCOMM_CSR_NAME, {{CMDSTR(tmp_str)}, {CMDSENTENCE(tmp_str2)}}, CMDF_HIDEVARS,
                            "Change a player's chat handle"},
    { 3, "silenceall_channels",    SCMD_SHARDCOMM_CSR_SILENCEALL, {{0}}, CMDF_HIDEVARS,
                            "Blocks everyone from channel chat"},
    { 2, "unsilenceall_channels",SCMD_SHARDCOMM_CSR_UNSILENCEALL, {{0}}, CMDF_HIDEVARS,
                            "Restores channel chat"},
    { 9, "chatserver_shutdown",SCMD_SHARDCOMM_CSR_SHUTDOWN, {{CMDSENTENCE(tmp_str)}}, CMDF_HIDEVARS,
                            "Broadcast message to all users and shutdown the Chatserver."},
    { 2, "sendall",            SCMD_SHARDCOMM_CSR_SENDALL, {{CMDSENTENCE(tmp_str)}}, CMDF_HIDEVARS,
                            "Send message to all players logged in to chatserver"},
    { 2, "silence_handle",    SCMD_SHARDCOMM_CSR_SILENCE_HANDLE, {{CMDSTR(tmp_str)}, {CMDINT(tmp_int)}}, CMDF_HIDEVARS,
                            "Silence a handle from all channel chat."},
    { 4, "allow_change_handle_all", SCMD_SHARDCOMM_CSR_RENAMEALL, {{0}}, CMDF_HIDEVARS,
                            "Allow ALL players to change their Global Handle again."},
    { 2, "grant_handle_change", SCMD_SHARDCOMM_CSR_GRANTRENAME, {{CMDSTR(tmp_str)}}, CMDF_HIDEVARS,
                            "Allow a player to change their Global Handle again."},
    { 3, "chan_members_mode", SCMD_SHARDCOMM_CSR_MEMBERSMODE, {{CMDSTR(tmp_str)}, {CMDSTR(tmp_str2)}}, CMDF_HIDEVARS,
                            "Changes permissions for all members of a channel.\n"
                            "Syntax: chan_members_mode <CHANNEL NAME> <OPTIONS...>\n"
                            "Valid Options:\n"
                            "\t-join\tkicks user from channel\n"
                            "\t+send / -send\tgives/removes user ability to send messages to channel\n"
                            "\t+operator / -operator\tgives/removes operator status from another user in the channel\n"},
    { 2, "global_remove_handle", SCMD_SHARDCOMM_CSR_REMOVEALL, {{CMDSTR(tmp_str)}}, CMDF_HIDEVARS,
                            "Removes the player from all global friends lists, ignore lists and channels."},
    { 1, "gethandle",        SCMD_GETHANDLE, {{CMDSENTENCE(player_name)}}, CMDF_HIDEVARS,
                            "Displays a player's global chat handle"},
    { 9, "chat_cmd_relay",    SCMD_CHAT_CMD_RELAY, {{CMDSTR(tmp_str)},{CMDINT(tmp_int)},{CMDINT(tmp_int2)},{CMDSENTENCE(player_name)}}, CMDF_HIDEVARS,
                            "Internal command for inserting player's chat handle and exectuting a command."},
    { 9, "gethandle_relay", SCMD_GETHANDLE_RELAY, {{CMDINT(tmp_int)},{CMDINT(tmp_int2)}}, CMDF_HIDEVARS,
                            "Relay command to get a player's global chat handle"},
    { 2, "check_mail_sent", SCMD_SHARDCOMM_CSR_CHECKMAILSENT, {{CMDSENTENCE(player_name)}}, CMDF_HIDEVARS,
                            "Allows GM to check a players gmails" },
    { 2, "check_mail_recv", SCMD_SHARDCOMM_CSR_CHECKMAILRECV, {{CMDSENTENCE(player_name)}}, CMDF_HIDEVARS,
                            "Allows GM to check a players gmails" },
    { 4, "bounce_mail_sent", SCMD_SHARDCOMM_CSR_BOUNCEMAILSENT, {{CMDINT(tmp_int)},{CMDSENTENCE(player_name)}}, CMDF_HIDEVARS,
                            "Allows GM to bounce a players gmails" },
    { 4, "bounce_mail_recv", SCMD_SHARDCOMM_CSR_BOUNCEMAILRECV, {{CMDINT(tmp_int)},{CMDSENTENCE(player_name)}}, CMDF_HIDEVARS,
                            "Allows GM to bounce a players gmails" },
    { 2, "check_mail_pending", SCMD_CSR_GMAILPENDING,    {{CMDSENTENCE(player_name)}}, CMDF_HIDEVARS,
                            "Show pending gmails on this player."},
    { 3, "respec_grant",    SCMD_GRANT_RESPEC, {{0}}, 0,
                            "Give a free respec" },
    { 0, "respec_status",    SCMD_RESPEC_STATUS, {{0}}, CMDF_HIDEPRINT,
                            "Find out how many respecs are available" },
    { 3, "respec_grant_token",    SCMD_GRANT_RESPEC_TOKEN, {{CMDINT(tmp_int)}}, 0,
                            "Modify and existing token" },
    { 3, "respec_grant_counter",    SCMD_GRANT_COUNTER_RESPEC, {{CMDINT(tmp_int)}}, 0,
                            "increment available counter respecs by specified amount"},
    { 3, "cape_grant",        SCMD_GRANT_CAPE, {{0}}, 0,
                            "unlock capes" },
    { 3, "glow_grant",        SCMD_GRANT_GLOW, {{0}}, 0,
                            "unlock glow" },
    { 3, "costumepart_grant",    SCMD_GRANT_COSTUMEPART, {{CMDSENTENCE(tmp_str)}}, CMDF_HIDEVARS,
                            "grant a reward costume part"},
    { 4, "respec_remove",    SCMD_REMOVE_RESPEC, {{0}}, 0,
                            "take away free respec" },
    { 3, "respec_remove_token",    SCMD_REMOVE_RESPEC_TOKEN, {{CMDINT(tmp_int)}}, 0,
                            "Modify and existing token" },
    { 3, "respec_remove_counter",    SCMD_REMOVE_COUNTER_RESPEC, {{CMDINT(tmp_int)}}, 0,
                            "decrement available counter respecs by specified amount" },
    { 3, "cape_remove",        SCMD_REMOVE_CAPE, {{0}}, 0,
                            "lock capes" },
    { 3, "glow_remove",        SCMD_REMOVE_GLOW, {{0}}, 0,
                            "lock glow" },
    { 3, "costumepart_remove",    SCMD_REMOVE_COSTUMEPART, {{CMDSENTENCE(tmp_str)}}, CMDF_HIDEVARS,
                            "remove a reward costume part"},
    { 9, "newScene",        SCMD_NEW_SCENE, {{ CMDINT(tmp_int) }},0,
                            "new scene file" },
    { 2, "costume_reset",    SCMD_RESET_COSTUME, {{ CMDINT(tmp_int) }},0,
                            "reset a players costume" },
    { 2, "costume_add_tailor",SCMD_ADD_TAILOR, {{0}},0,
                            "gives the player a free tailor session" },
    { 2, "costume_add_tailor_per_slot",SCMD_ADD_TAILOR_PER_SLOT, {{0}},0,
                            "gives the player a free tailor session for each owned costume slot" },
    { 2, "costume_free_tailor",SCMD_FREE_TAILOR, {{ CMDINT(tmp_int) }},0,
                            "sets the player's free tailor sessions" },
    { 2, "costume_ultra_tailor", SCMD_ULTRATAILOR_GRANT, {{0}}, 0,
                            "Gives player tailor with ability to change gender/scale" },
    { 0, "tailor_status",    SCMD_FREE_TAILOR_STATUS, {{0}}, CMDF_HIDEPRINT,
                            "Find out how many free tailor sessions are available" },
    { 3, "villain_list", SCMD_CSR_CRITTER_LIST, {{0}}, 0,
                            "Print a list of all the villains on map" },
    { 3, "goto_id", SCMD_CSR_GOTO_CRITTER_ID, {{CMDINT(tmp_int)}}, 0,
                            "Goto a villain ID" },
    { 3, "goto_name", SCMD_CSR_GOTO_CRITTER_NAME, {{CMDSENTENCE(tmp_str)}}, 0,
                            "Goto next villain with given name" },
    { 3, "authkick", SCMD_CSR_AUTHKICK, {{CMDSTR(player_name)},{CMDSENTENCE(tmp_str)}}, 0,
                            "Kick an authname: <name> <reason>" },
    { 3, "archvillain", SCMD_CSR_NEXT_ARCHVILLAIN, {{0}}, 0,
                            "Goto the next archvillain" },
    { 9, "listen", SCMD_LISTEN, {{CMDSENTENCE(player_name)}}, 0,
                            "Receive all chat going to a user" },
    { 3, "viewattributes", SCMD_ATTRIBUTETARGET, {{0}}, 0, 
                            "View the attributes of the player (while they are on same map)"    },
    { 0, "petViewAttributes", SCMD_PETATTRIBUTETARGET, {{CMDINT(tmp_int)}}, CMDF_HIDEVARS|CMDF_HIDEPRINT, 
                            "View the attributes of the player's pet"},
    { 0, "clearAttributeView", SCMD_CLEARATTRIBUTETARGET, {{0}}, 0, 
                            "Clear the attribute target"    },
    { 9, "auth_user_data_set", SCMD_AUTHUSERDATA_SET, {{CMDSTR(tmp_str)}, {CMDINT(tmp_int)}}, 0,
                            "Set auth user data values. See value names via /auth_user_data_show, also Raw[0-3] and OrRaw[0-3]." },
    { 9, "auth_user_data_show", SCMD_AUTHUSERDATA_SHOW, {{0}}, 0,
                            "Shows auth user data values." },
    { 0, "respec", SCMD_RESPEC, {{0}}, 0,
                            "Go to respec screen if you have recieved a free holiday respec" },
    { 9, "alwaysknockplayers", 0, {{ CMDINT(ai_state.alwaysKnockPlayers) }}, 0,
                            "Knock players every time they get hit." },
    { 9, "no_melee_prediction", 0, {{ CMDINT(server_state.disableMeleePrediction) }}, 0,
                            "Disables server-side melee-target backwards-in-time position prediction." },
    { 9, "disableConsiderEnemyLevel", 0, {{ CMDINT(server_state.disableConsiderEnemyLevel) }}, 0,
                            "Debug disables ai considering your level when deciding whether ot attack you." },
    { 2, "description_set", SCMD_CSR_DESCRIPTION_SET, {{ CMDSENTENCE(tmp_str) }}, 0,
                            "Change a players description" },
    { 2, "setdifficultylevel",        SCMD_DIFFICULTY_SET_LEVEL, {{CMDINT(tmp_int)}}, 0,
                            "Set your level (no influence charge)" },
    { 2, "setdifficultyteamsize",        SCMD_DIFFICULTY_SET_TEAMSIZE, {{CMDINT(tmp_int)}}, 0,
                            "Set your team size (no influence charge)" },
    { 2, "setdifficultyav",        SCMD_DIFFICULTY_SET_AV, {{CMDINT(tmp_int)}}, 0,
                            "Set your AV spawning (no influence charge). 1 to make AV's always appear, 0 for always EB" },
    { 2, "setdifficultyboss",        SCMD_DIFFICULTY_SET_BOSS, {{CMDINT(tmp_int)}}, 0,
                            "Set your Boss downgrade (no influence charge).  1 to make bosses not downgrade when solo" },
    { 0, "getarenastats",    SCMD_GET_RATED_ARENA_STATS, {{0}}, 0,
                            "Get your arena stats." },
    { 0, "get_rated_arena_stats",    SCMD_GET_RATED_ARENA_STATS, {{0}}, 0,
                            "Get your arena stats." },
    { 0, "get_all_arena_stats",    SCMD_GET_ALL_ARENA_STATS, {{0}}, 0,
                            "Get your arena stats." },
    { 0, "#",                0, {{CMDSENTENCE(tmp_str)}}, CMDF_HIDEPRINT,
                            "Comment. Ignores rest of the line."},
    { 4, "rw_salvage",            SCMD_SALVAGE_GRANT, {{ CMDSTR(tmp_str)},{CMDINT(tmp_int)}}, CMDF_HIDEVARS,
                            "Give your entity one of the specified salvage items"},
    { 9, "rw_conceptitem",            SCMD_REWARD_CONCEPTITEM, {{ CMDSTR(tmp_str)},{CMDINT(tmp_int) },{ PARSETYPE_FLOAT, &tmp_f32s[0] },{ PARSETYPE_FLOAT, &tmp_f32s[1] },{ PARSETYPE_FLOAT, &tmp_f32s[2] },{ PARSETYPE_FLOAT, &tmp_f32s[3] }}, CMDF_HIDEVARS,
                            "Give your entity a conceptitem with given values:\n<name> <amount> <var0> <var1> <var2> <var3>"},
    { 9, "rw_conceptdef",            SCMD_REWARD_CONCEPTDEF, {{CMDSTR(tmp_str)},{ CMDINT(tmp_int) }}, CMDF_HIDEVARS,
                            "Give your entity a conceptdef with rolled values:\n<name>"},
    { 4, "rw_recipe",SCMD_REWARD_RECIPE, {{ CMDSTR(tmp_str)}}, CMDF_HIDEVARS,
                            "Give your entity one of the specified recipe items"},
    { 9, "rw_recipex",SCMD_REWARD_RECIPE_ALL, {{0}}, CMDF_HIDEVARS,
                            "give a bunch of recipes: testing only!!!"},
    { 9, "rw_detail",SCMD_DETAIL_GRANT, {{ CMDSTR(tmp_str)}}, CMDF_HIDEVARS,
                            "Give your entity one of the specified details"},
    { 9, "rw_detailrecipe",SCMD_REWARD_DETAILRECIPE, {{CMDSTR(tmp_str)},{CMDINT(tmp_int)}}, CMDF_HIDEVARS,
                            "Give your entity one of the specified detailrecipe items, 1 if infinite"},
    { 2, "rw_costume_slot",    SCMD_COSTUME_SLOT_GRANT, {{0}}, CMDF_HIDEVARS,
                            "Give character an additional costume slot"},                
    { 2, "check_costume_slots",    SCMD_CHECK_COSTUME_SLOTS, {{0}}, CMDF_HIDEVARS,
                            "Prints out costume slot information"},                
    { 9, "rw_invention_all",    SCMD_INVENTION_REWARDALL, {{0}}, CMDF_HIDEVARS,
                            "give every salvage"},
    { 9, "clear_inv",        SCMD_INV_CLEARALL, {{0}}, CMDF_HIDEVARS,
                            "Clears all of your inventory"},
    { 4, "rm_recipe",        SCMD_REMOVE_RECIPE, {{ CMDSTR(tmp_str)}}, CMDF_HIDEVARS,
                            "Remove recipe from your inventory"},

    { 9, "debug_expr",        SCMD_DEBUG_EXPR, {{ CMDINT(tmp_int) }}, CMDF_HIDEVARS,
                            "Turns on or off combat expression debugging."},
    { 0, "servertime",        SCMD_SERVER_TIME, {0},0,
                            "Print the current server time"},
    { 0, "citytime",		SCMD_CITY_TIME, {0},0,
							"Print the current city time"},                            
    { 9, "fill_powerupslot",    SCMD_FILL_POWERUP_SLOT, {{CMDINT(tmp_int)},{CMDINT(tmp_int2)}}, CMDF_HIDEVARS,
                            "Unlocks the skill system" },
    { 2, "base_save",        SCMD_BASE_SAVE, {{ 0 }}, CMDF_HIDEVARS,
                            "Saves the current base" },
    { 9, "base_teleport",    SCMD_TELEPORT_TO_BASE, {{ 0 }}, 0,
                            "Teleport into your supergroups base, if possible" },
    { 4, "base_destroy",    SCMD_BASE_DESTROY, {{ 0 }}, CMDF_HIDEVARS,
                            "LAST RESORT.  CLoses map and removes the base entirely." },
    { 9, "makepets",        SCMD_ARENA_MAKEPETS, {0},0,
                            "Debug: makes the pets in your pet army"},
#if NOVODEX
    { 9, "nxs_init_world",    SCMD_NOVODEX_INIT_WORLD, {{ 0 }}, 0,
                            "creates NovodeX world geometry data" },
    { 9, "nxs_deinit_world",SCMD_NOVODEX_DEINIT_WORLD,  {0},0,
                            "deletes NovodeX world geometry data" },
    { 9, "nxs_debug",        0, {{ CMDINT(nx_state.debug) }}, 0,
                            "enables NovodeX debugging" },
    { 9, "nxs_maxActorCount",0, {{ CMDINT(nx_state.maxDynamicActorCount) }}, 0,
                            "max total dynamic actors per instance" },
#endif
    { 9, "rwtoken",    SCMD_REWARD_TOKEN, {{ CMDSTR(tmp_str) }, {CMDINT(tmp_int)}}, 0,
                            "reward the named token to the player" },
    { 9, "rwtokentoplayer", SCMD_REWARD_TOKEN_TO_PLAYER, {{ CMDSTR(tmp_str) }, {CMDINT(tmp_int)}}, 0,
                            "reward the named token to the player with the specified db_id." },
    { 9, "rmtoken",    SCMD_REMOVE_TOKEN, {{ CMDSTR(tmp_str) }}, 0,
                            "remove the named token from the player" },
    { 9, "rmtokenfromplayer", SCMD_REMOVE_TOKEN_FROM_PLAYER, {{ CMDSTR(tmp_str) }, {CMDINT(tmp_int)}}, 0,
                            "remove the named token from the player with the specified db_id." },
    { 3, "lstoken",SCMD_LIST_TOKENS , {0}, 0,
                            "list tokens for the player" },
    { 4, "lsactivetoken",    SCMD_LIST_ACTIVE_PLAYER_TOKENS, {0}, 0,
                            "list active player tokens for the player's team" },
    { 4, "lsphase", SCMD_LIST_PHASES, {0}, 0,
                            "list current phases seen by the player" },
    { 3, "isvip", SCMD_IS_VIP, {0}, 0,
                            "returns whether the player is currently a VIP." },
    { 9, "map_rwtoken",        SCMD_MREWARD_TOKEN, {{ CMDSTR(tmp_str) }, {CMDINT(tmp_int)}}, 0,
                            "reward the named token to the map" },
    { 9, "map_rmtoken",        SCMD_MREMOVE_TOKEN, {{ CMDSTR(tmp_str) }}, 0,
                            "remove the named token to the map" },
    { 9, "map_lstoken",        SCMD_MLIST_TOKENS , {0}, 0,
                            "list tokens for the map" },
    { 9, "sgrp_rwtoken",    SCMD_SGRP_REWARD_TOKEN, {{ CMDSTR(tmp_str) }}, 0,
                            "reward the named token to the player's supergroup" },
    { 9, "sgrp_rmtoken",    SCMD_SGRP_REMOVE_TOKEN, {{ CMDSTR(tmp_str) }}, 0,
                            "remove the named token to the player's supergroup" },
    { 9, "sgrp_lstoken",SCMD_SGRP_LIST_TOKENS , {0}, 0,
                            "list tokens in the player's supergroup" },
    { 9, "sgrp_stat", SCMD_SGRP_STAT, {{ CMDSTR(tmp_str) }, {CMDINT(tmp_int)}}, 0,
                            "update supergroup stats sgrp_stat <stat name> amount" },
    { 0, "show_petnames", SCMD_SHOW_PETNAMES, {0}, 0,
                            "Displays the names of all your named pets" },
    { 0, "clear_petnames", SCMD_CLEAR_PETNAMES, {0}, 0,
                            "Clear the names of all your named pets" },
    { 0, "release_pets", SCMD_KILL_ALLPETS, {0}, 0,
                            "Release your current pets" },

    { 9, "sgrp_badgestates", SCMD_SGRP_BADGESTATES, {{CMDINT(tmp_int)}, {CMDSTR(tmp_str)},{CMDINT(tmp_int2)}}, 0,
                            "set the stats for a supergroup. (used by statserver)." },
    { 9, "serve_floater", SCMD_SERVEFLOATER, {{CMDSTR(tmp_str)},{PARSETYPE_FLOAT,&tmp_float},{CMDINT(tmp_int)}}, 0,
                            "serves a floater up to a given entity." },
    { 9, "sgrp_badgeaawardnotify_relay", SCMD_SGRPBADGEAWARD_NOTIFY_RELAY, {{CMDSTR(tmp_str)},{CMDINT(tmp_int)}}, 0,
                            "<badgename>" },
    { 3, "salvage_names", SCMD_SALVAGE_NAMES, {0}, 0,
                            "List internal and display names of all salvage"},
    { 3, "salvage_list", SCMD_SALVAGE_LIST, {0}, 0,
                            "List all the salvage owned by character"},
    { 4, "salvage_grant", SCMD_SALVAGE_GRANT, {{CMDSTR(tmp_str)},{CMDINT(tmp_int)}}, 0,
                            "Adds the given salvage to character" },
    { 4, "salvage_revoke", SCMD_SALVAGE_REVOKE, {{CMDSTR(tmp_str)},{CMDINT(tmp_int)}}, 0,
                            "Revokes the given salvage from character" },
    { 3, "detail_categories", SCMD_DETAIL_CAT_NAMES, {0}, 0,
                            "List the names of all detail categories"},
    { 3, "detail_names", SCMD_DETAIL_NAMES, {{CMDSTR(tmp_str)}}, 0,
                            "List the internal and display names of all details in an internal category"},
    { 3, "detail_list", SCMD_DETAIL_LIST, {0}, 0,
                            "List all the details owned by character"},
    { 4, "detail_grant", SCMD_DETAIL_GRANT, {{CMDSTR(tmp_str)}}, 0,
                            "Adds the given detail to character" },
    { 4, "detail_revoke", SCMD_DETAIL_REVOKE, {{CMDSTR(tmp_str)}}, 0,
                            "Revokes the given detail from character" },
    { 3, "sg_detail_names", SCMD_SG_DETAIL_NAMES, {0}, 0,
                            "List the internal and display names of all special details"},
    { 3, "sg_detail_list", SCMD_SG_DETAIL_LIST, {0}, 0,
                            "List all the special details owned by this supergroup"},
    { 4, "sg_detail_grant", SCMD_SG_DETAIL_GRANT, {{CMDSTR(tmp_str)}}, 0,
                            "Adds the given detail to the supergroup" },
    { 4, "sg_detail_revoke", SCMD_SG_DETAIL_REVOKE, {{CMDSTR(tmp_str)},{CMDINT(tmp_int)}}, 0,
                            "Revokes the given detail with given creation time from supergroup" },
    { 3, "sg_set_rank",        SCMD_SG_SET_RANK, {{CMDINT(tmp_int)}}, 0,
                            "Sets the rank of this person directly (without checking for permission)" },
    { 9, "sg_updatemember", SCMD_SG_UPDATEMEMBER, {{CMDINT(tmp_int)}}, 0,
                            "Flags a supergroup member for an update, regardless of which mapserver he/she is on" },
    { 9, "sg_taskcomplete", SCMD_SGRP_TASK_COMPLETE, {{CMDINT(tmp_int)}}, 0,
                            "Flags a player to let them know that their SG has completed an SG mission, regardless of which mapserver he/she is on" },

                            // updated with new IoP Game (7/2008)
    { 9, "sgiop_grant",        SCMD_SG_ITEM_OF_POWER_GRANT, { { CMDSTR(tmp_str) }, {CMDINT(tmp_int)} }, 0,
                            "<name> <duration> Debug: Grant player's SG Item of Power"},
    { 9, "sgiop_revoke",    SCMD_SG_ITEM_OF_POWER_REVOKE, { { CMDSTR(tmp_str) } }, 0,
                            "Debug: Revoke player's SG Item of Power"},
    { 9, "sgiop_list",        SCMD_SG_ITEM_OF_POWER_LIST, { 0 }, 0,
                            "Debug: Show list of player's SG Item of Power"},

    { 9, "sgiop_grant_new",    SCMD_SG_ITEM_OF_POWER_GRANT_NEW, { {CMDINT(tmp_int)}}, 0,
                            "Debug: Instruct the Raid Server to Grant SG #X a New, Random Item of Power ( 0 == My SG )"},
    { 9, "sgiop_synch",        SCMD_SG_ITEM_OF_POWER_SYNCH, { {CMDINT(tmp_int)} }, 0,
                            "Debug: Instruct the SG #X To Synchronize its Items of Power with the Raidserver ( 0 == My SG )"},
    { 9, "sgiop_transfer",    SCMD_SG_ITEM_OF_POWER_TRANSFER, {{CMDINT(tmp_int)}, {CMDINT(tmp_int2)},{CMDINT(tmp_int3)}}, 0,
                            "Unused: Instruct the Raid Server to transfer Item of Power from Sg 1 to Sg 2"},
    { 9, "sgiop_update",    SCMD_SG_ITEM_OF_POWER_UPDATE, {{CMDINT(tmp_int)}}, 0,
                            "Unused: Instruct the Raid Server to tell this Super Group about all its Items of Power"},
    { 9, "sgIopGameState",    SCMD_SG_ITEM_OF_POWER_GAME_SET_STATE, {{ CMDSTR(tmp_str) }}, 0,
                            "Instruct the Raid Server Set the Item of Power Game State on this shard. \nRestart, CloseCathedral, or End, \nplus DebugAllowRaidsAndTrials and NoDebug"},
    { 9, "showIopGame",        SCMD_SG_ITEM_OF_POWER_GAME_SHOW_SERVER_STATE, {0}, 0,
                            "Show everything this server currently knows about the Item of POwer Game"},

    { 1, "sg_grant_raid_points",    SCMD_SG_GRANT_RAID_POINTS, { {CMDINT(tmp_int)}, {CMDINT(tmp_int2)}}, 0,
                            "Grants points to current SG, <points> <isRaid>"},
    { 1, "sg_show_raid_points",    SCMD_SG_SHOW_RAID_POINTS, {0}, 0,
                            "show current raid points"},
	{ 0, "sg_passcode",	SCMD_SG_PASSCODE, {{ CMDSTR(tmp_str) }}, 0,
							"Sets the Supergroup Base access passcode."},
    { 0, "sg_music",	SCMD_SG_MUSIC, {{ CMDSTR(tmp_str) }}, 0,
							"Sets the Supergroup Base background music."},                            

    { 9, "imebug",            SCMD_TEST_IMEBUG,{0},0,
                            "test ime bug"},

    { 1, "sg_csr_join",        SCMD_SG_CSR_JOIN, {{CMDINT(tmp_int)}}, 0,
                            "Allows a CSR to addthemselves to a SG" },
    { 9, "baseaccess_froment_relay", SCMD_BASEACCESS_FROMENT_RELAY, {{CMDINT(tmp_int)}, {CMDINT(tmp_int2)}, {CMDINT(tmp_int3)}, {CMDINT(tmp_int4)}}, 0, 
                            "get the baseaccess for a given ent: <idEntRequesting> <base entry requested> <id ent>"},
    { 9, "baseaccess_response_relay", SCMD_BASEACCESS_RESPONSE_RELAY, {{CMDSTR(tmp_str)}, {CMDINT(tmp_int)}}, 0, 
                            "get the baseaccess for a given ent: <'[idSgrp,baseAccess,base name\t]+'> <idEnt>. can take multiple if separated by tabs"},

    { 9, "sgrp_base_prestige_fixed", SCMD_SGRP_BASE_PRESTIGE_FIXED, {{CMDINT(tmp_int)}}, 0, 
                            "clear the base prestige fixed flag"},
    { 1, "sgrp_base_prestige_show", SCMD_SGRP_BASE_PRESTIGE_SHOW, {0}, 0, 
                            "show the amount of base prestige"},
    { 1, "sgrp_base_prestige_set", SCMD_SGRP_BASE_PRESTIGE_SET, {{CMDINT(tmp_int)}}, 0, 
                            "set the amount of base prestige"},

    { 1, "storage_adj", SCMD_BASE_STORAGE_ADJUST, {{CMDINT(tmp_int)}, {CMDSTR(tmp_str)}, {CMDINT(tmp_int2)}}, CMDF_RETURNONERROR, 
                            "usage: <adj amount> <name item to adjust> <optional:level> adjust the named salvage by N for the currently selected detail"},
    { 1, "storage_set", SCMD_BASE_STORAGE_SET, {{CMDINT(tmp_int)}, {CMDSTR(tmp_str)}, {CMDINT(tmp_int2)}}, CMDF_RETURNONERROR, 
                            "usage: <set amount> <name item to adjust> <optional:level> set the named item to N for the currently selected detail"},

    { 4, "offline_character", SCMD_OFFLINE_CHARACTER, {{CMDSENTENCE(tmp_str)}}, 0, 
                            "usage: <db_id> or <char name>. Force a character to go offline, as if their account were inactive"},
    { 5, "restore_deleted_char", SCMD_OFFLINE_RESTORE_DELETED, {{CMDSENTENCE(tmp_str)}}, 0,
                            "usage: <auth> <char name> <sequence-#> <MM/YYYY>. Restores character deleted during this time period." },
    { 4, "list_deleted_chars", SCMD_OFFLINE_LIST_DELETED, {{CMDSENTENCE(tmp_str)}}, 0, 
                            "usage: <auth> <MM/YYYY> Lists characters in the deletion file for this user for this time period."},
                            
    { 9, "editvillaincostume", SCMD_EDIT_VILLAIN_COSTUME, {{CMDINT(tmp_int)},{CMDSENTENCE(tmp_str)}}, 0,
                            "Edit critter costume by villain def name." },
    { 9, "editnpccostume",    SCMD_EDIT_NPC_COSTUME, {{CMDSENTENCE(tmp_str)}}, 0,
                            "Edit critter costume by costume def name." },
    { 9, "debug_powers",    SCMD_DEBUG_POWERS, {{ CMDINT(g_ulPowersDebug) }}, CMDF_HIDEVARS,
                            "Controls powers debugging"},
    { 2, "backup_player",    SCMD_BACKUP_PLAYER, {{CMDSENTENCE(tmp_str)}}, 0,
                            "Force a backup of a currently logged in player" },
    { 2, "backup_search",    SCMD_BACKUP_SEARCH, {{CMDSENTENCE(tmp_str)}}, 0,
                            "Get list of backups available for a player" },
    { 2, "backup_view",        SCMD_BACKUP_VIEW, {{ CMDINT(tmp_int) },{CMDSENTENCE(tmp_str)}}, 0,                        
                            "View indepth backup from current search" },
    { 2, "backup_apply",    SCMD_BACKUP_APPLY, {{ CMDINT(tmp_int) },{CMDSENTENCE(tmp_str)}}, 0,
                            "Apply selected backup, player must be off line" },
    { 2, "backup_sg",        SCMD_BACKUP_SG, {{CMDSENTENCE(tmp_str)}}, 0,
                            "Force a backup of players supergroup" },
    { 2, "backup_search_sg",SCMD_BACKUP_SEARCH_SG, {{CMDSENTENCE(tmp_str)}}, 0,
                            "Get list of backups available for a supergroup" },
    { 2, "backup_view_sg",    SCMD_BACKUP_VIEW_SG, {{ CMDINT(tmp_int) },{CMDSENTENCE(tmp_str)}}, 0,                        
                            "View in depth backup text from current search" },
    { 2, "backup_apply_sg",    SCMD_BACKUP_APPLY_SG, {{ CMDINT(tmp_int) },{CMDSENTENCE(tmp_str)}}, 0,
                            "Apply selected backup, supergroup must be off line" },
    { 9, "forcelogout",     SCMD_FORCE_LOGOOUT, { {CMDINT(tmp_int)}}, 0,
                            "Debug Kick" },
    { 9, "playereval",        SCMD_PLAYER_EVAL, { {CMDSENTENCE(tmp_str)}}, 0,
                            "Run player eval" },
    { 9, "combateval",        SCMD_COMBAT_EVAL, {{CMDSTR(tmp_str2)}, {CMDSENTENCE(tmp_str)}}, 0,
                            "Run combat eval using my current target and the specified power." },
    { 2, "complete_stuck_mission", SCMD_COMPLETE_STUCK_MISSION, {{CMDINT(tmp_int)}}, 0,
                            "completes the current mission. arg for complete successfully/failed. for use only when stuck. you get one use per week." },
    //{ 1, "auc_add_salvage", SCMD_AUCTION_ADD_SALVAGEITEM, {{CMDINT(tmp_int2)},{CMDINT(tmp_int3)},{CMDINT(tmp_int4)}}, 0,
                            //"<idx> <amt> <sell price> request current auction inventory" },
    { 9, "auc_trust_gameclient", SCMD_AUCTION_TRUST_GAMECLIENT, {{CMDINT(tmp_int)}}, 0,
                            "use in conjunction with cmd auc_SellAndBuyAll to hammer the auction house" },
    { 2, "acc_unlock",        SCMD_ACC_UNLOCK,{0},CMDF_HIDEVARS,
                            "USE ACC_ACCOUNTINFO FIRST! Make sure all orders have succeeded or failed!\n"
                            "remove account server lock, use with csroffline" },
    { 0, "conprint",        SCMD_CONPRINT, {{CMDSTR(tmp_str)}}, CMDF_HIDEPRINT,
                            "echo <str> to the console" },
    { 9, "dump_storyarcinfo", SCMD_DUMP_STORYARCINFO,{0},CMDF_HIDEVARS,
                            "USE ACC_ACCOUNTINFO FIRST! Make sure all orders have succeeded or failed!\n"
                             "test the csv printing code" },
    { 6, "account_grant_charslot", SCMD_ACCOUNT_GRANT_CHARSLOT, {{CMDINT(tmp_int)}}, 0,
                            "grants <int> more character slots for current account" },
    { 6, "account_certification_buy", SCMD_ACCOUNT_CERTIFICATION_BUY, {{CMDINT(tmp_int)},{CMDINT(tmp_int2)},{CMDSENTENCE(tmp_str)}}, 0,
                            "buys a <0=cert/1=voucher> <count> <description>" },
    { 0, "account_certification_claim", SCMD_ACCOUNT_CERTIFICATION_CLAIM, {{CMDSENTENCE(tmp_str)}}, 0,
                        "claims certification" },
    { 0, "account_certification_refund", SCMD_ACCOUNT_CERTIFICATION_REFUND, {{CMDSENTENCE(tmp_str)}}, 0,
                            "refunds the price of certification to player" },
    { 6, "account_certification_delete", SCMD_ACCOUNT_CERTIFICATION_DELETE, {{CMDSENTENCE(tmp_str)}}, 0,
                            "hides certification from player (adds to delete count)" },
    { 6, "account_certification_erase", SCMD_ACCOUNT_CERTIFICATION_ERASE, {{CMDSENTENCE(tmp_str)}}, 0,
                            "removes certification from player inventory (for debugging)" },
    { 3, "account_certification_show", SCMD_ACCOUNT_CERTIFICATION_SHOW, {{0}}, 0,
                            "displays all certification records in player inventory (for debugging)" },
    { 8, "account_certification_delete_all", SCMD_ACCOUNT_CERTIFICATION_DELETE_ALL, {{0}}, 0,
                            "removes all certification records from player inventory (for debugging)" },
    { 9, "account_certification_test", SCMD_ACCOUNT_CERTIFICATION_TEST, {{CMDINT(tmp_int)}}, 0,
                            "Four digit number: thousands place: 0 -> message is dropped, 1 -> message failed and respond.  Hundreds place: 0 -> only affect the next message, 1 -> affect all messages until /account_certification_test is run again to clear the setting.  Tens/ones:  index to denote where in the certification process to fail.  Currently used values are 1 through 8." },
    { 3, "account_inventory_list", SCMD_ACCOUNT_INVENTORY_LIST, {0}, 0,
                            "list the account inventory for current account" },
    { 4, "account_inventory_change", SCMD_ACCOUNT_INVENTORY_CHANGE, {{CMDSTR(tmp_str)},{CMDINT(tmp_int)}}, 0,
                            "modifies account item <product> by <delta>" },
    { 0, "account_loyalty_client_buy", SCMD_ACCOUNT_LOYALTY_BUY, {{CMDSTR(tmp_str)}}, CMDF_HIDEVARS|CMDF_HIDEPRINT,
                            "purchases loyalty reward <name> for current account" },
    { 6, "account_loyalty_refund", SCMD_ACCOUNT_LOYALTY_REFUND, {{CMDSENTENCE(tmp_str)}}, 0,
                            "refunds loyalty reward <name> for current account" },                            
    { 4, "account_loyalty_earned", SCMD_ACCOUNT_LOYALTY_EARNED, {{CMDINT(tmp_int)}}, 0,
                            "adds <amount> to earned loyalty points for current account" },
    { 3, "account_loyalty_show", SCMD_ACCOUNT_LOYALTY_SHOW, {0}, 0,
                            "displays the state of the loyalty rewards for the current account" },
    { 6, "account_loyalty_reset", SCMD_ACCOUNT_LOYALTY_RESET, {0}, 0,
                            "resets all loyalty rewards for current account" },                    
    { 9, "relay_conprintf", SCMD_RELAY_CONPRINTF, {{CMDINT(tmp_int)},{CMDSENTENCE(tmp_str)}}, 0,
                            "Relays conprintf commands" },
    { 3, "unspamme",        SCMD_UNSPAMME, {0}, 0,
                            "Unbans and removes spam flags from this player" },
    { 1, "select_build",    SCMD_SELECT_BUILD, {{CMDINT(tmp_int)}}, 0,
                            "Select current build" },
    { 1, "architect_userinfo",    SCMD_ARCHITECT_USERINFO, {{0}}, 0,
                            "Print architect user info" },
    { 1, "architect_otheruserinfo",    SCMD_ARCHITECT_OTHERUSERINFO, {{CMDSTR(tmp_str)},{CMDSTR(tmp_str2)}}, 0,
                            "Print architect user info about player <authname> <region>" },
    { 1, "architect_useridinfo", SCMD_ARCHITECT_USERIDINFO, {{CMDINT(tmp_int)},{CMDSTR(tmp_str2)}}, 0,
                            "Print architect user info" },
    { 9, "relay_architect_join", SCMD_ARCHITECT_JOIN_RELAY, {{CMDINT(tmp_int)},{CMDINT(tmp_int2)},{CMDINT(tmp_int3)}}, 0,
                            "send message to teammate to put him on the taskforce" },
    { 0, "architect_claim_tickets", SCMD_ARCHITECT_CLAIM_TICKETS, {{CMDINT(tmp_int)}}, 0,
                            "Claim architect tickets earned from authored story arcs" },
    { 2, "architect_grant_tickets", SCMD_ARCHITECT_GRANT_TICKETS, {{CMDINT(tmp_int)}}, 0,
                            "Grant architect tickets to an account." },
    { 2, "architect_grant_overflow_tickets", SCMD_ARCHITECT_GRANT_OVERFLOW_TICKETS, {{CMDINT(tmp_int)}}, 0,
                            "Grant architect overflow tickets to an account." },
    { 2, "architect_set_best_arc_stars", SCMD_ARCHITECT_SET_BEST_ARC_STARS, {{ CMDINT(tmp_int)}}, 0,
                            "Set maximum stars earned from any arc." },
    { 2, "architect_csr_get_arc", SCMD_ARCHITECT_CSR_GET_ARC, {{ CMDINT(tmp_int)}}, 0,
                            "Get the arc with the given ID and save it to local disc." },
    { 9, "dayjob",        SCMD_DAYJOB_DEBUG, {{ CMDINT(tmp_int) }}, 0, "Add <seconds> to day job." },

    { 2, "dbflags",                SCMD_DBFLAGS_SHOW, {0}, 0,
            "show DbFlags status" },
    { 2, "dbflags_show",        SCMD_DBFLAGS_SHOW, {0}, 0,
            "show DbFlags status" },
    { 2, "dbflag_show",            SCMD_DBFLAGS_SHOW, {0}, 0,
            "show DbFlags status" },
    { 2, "dbflag_set",            SCMD_DBFLAGS_SET, { {CMDSTR(tmp_str)}, {CMDINT(tmp_int)}}, 0,
            "set DbFlag <name> to <value>" },

    { 0, "architect_invisible", SCMD_ARCHITECT_INVISIBLE, {0}, 0,
            "Toggle invisibility on an architect test mode map" },
    { 0, "architect_invincible", SCMD_ARCHITECT_INVINCIBLE, {0}, 0,
            "Toggle invincibility on an architect test mode map"  },
    { 0, "architect_completemisison", SCMD_ARCHITECT_COMPLETEMISSION, {0}, 0,
            "Complete the current mission in Architect Test Mode" },
    { 0, "architect_nextobjective", SCMD_ARCHITECT_NEXTOBJECTIVE, {0}, 0,
            "Go to next object on an architect test mode map" },
    { 0, "architect_nextcritter", SCMD_ARCHITECT_NEXTCRITTER, {0}, 0,
            "Go to the next critter on an architect test mode map" },
    { 0, "architect_killtarget", SCMD_ARCHITECT_KILLTARGET, {0}, 0,
            "kill the currently selected target on an architect test mode map" },
    { 0, "architect_loginupdate", SCMD_ARCHITECT_LOGINUPDATE, {0},0,
            "Report how many architect tickets are waiting to be claimed" },
    { 1, "rogue_stats", SCMD_ROGUE_STATS, {0}, 0,
            "Get current going-rogue-relevant stats" },
    { 1, "roguepoints_add_paragon", SCMD_ROGUE_ADDPARAGON, {0}, 0,
            "Add a paragon point (roguepoints_paragon reward token)" },
    { 1, "roguepoints_add_hero", SCMD_ROGUE_ADDHERO, {0}, 0,
            "Add a hero point (roguepoints_hero reward token)" },
    { 1, "roguepoints_add_vigilante", SCMD_ROGUE_ADDVIGILANTE, {0}, 0,
            "Add a vigilante point (roguepoints_vigilante reward token)" },
    { 1, "roguepoints_add_rogue", SCMD_ROGUE_ADDROGUE, {0}, 0,
            "Add a rogue point (roguepoints_rogue reward token)" },
    { 1, "roguepoints_add_villain", SCMD_ROGUE_ADDVILLAIN, {0}, 0,
            "Add a villain point (roguepoints_villain reward token)" },
    { 1, "roguepoints_add_tyrant", SCMD_ROGUE_ADDTYRANT, {0}, 0,
            "Add a tyrant point (roguepoints_tyrant reward token)" },
    { 1, "roguepoints_reset_paragon", SCMD_ROGUE_RESETPARAGON, {0}, 0,
            "Reset paragon points to zero (roguepoints_paragon reward token)" },
    { 1, "roguepoints_reset_hero", SCMD_ROGUE_RESETHERO, {0}, 0,
            "Reset hero points to zero (roguepoints_hero reward token)" },
    { 1, "roguepoints_reset_vigilante", SCMD_ROGUE_RESETVIGILANTE, {0}, 0,
            "Reset vigilante points to zero (roguepoints_vigilante reward token)" },
    { 1, "roguepoints_reset_rogue", SCMD_ROGUE_RESETROGUE, {0}, 0,
            "Reset rogue points to zero (roguepoints_rogue reward token)" },
    { 1, "roguepoints_reset_villain", SCMD_ROGUE_RESETVILLAIN, {0}, 0,
            "Reset villain points to zero (roguepoints_villain reward token)" },
    { 1, "roguepoints_reset_tyrant", SCMD_ROGUE_RESETTYRANT, {0}, 0,
            "Reset tyrant points to zero (roguepoints_tyrant reward token)" },
    { 1, "roguepoints_set_paragon", SCMD_ROGUE_SETPARAGON, {{CMDINT(tmp_int)}}, 0,
            "Set paragon points to tmp_int (roguepoints_paragon reward token)" },
    { 1, "roguepoints_set_hero", SCMD_ROGUE_SETHERO, {{CMDINT(tmp_int)}}, 0,
            "Set hero points to tmp_int (roguepoints_hero reward token)" },
    { 1, "roguepoints_set_vigilante", SCMD_ROGUE_SETVIGILANTE, {{CMDINT(tmp_int)}}, 0,
            "Set vigilante points to tmp_int (roguepoints_vigilante reward token)" },
    { 1, "roguepoints_set_rogue", SCMD_ROGUE_SETROGUE, {{CMDINT(tmp_int)}}, 0,
            "Set rogue points to tmp_int (roguepoints_rogue reward token)" },
    { 1, "roguepoints_set_villain", SCMD_ROGUE_SETVILLAIN, {{CMDINT(tmp_int)}}, 0,
            "Set villain points to tmp_int (roguepoints_villain reward token)" },
    { 1, "roguepoints_set_tyrant", SCMD_ROGUE_SETTYRANT, {{CMDINT(tmp_int)}}, 0,
            "Set tyrant points to tmp_int (roguepoints_tyrant reward token)" },
    { 1, "newalignment_switch_paragon", SCMD_ROGUE_SWITCHPARAGON, {0}, 0,
            "Switch to paragon alignment, reset roguepoints" },
    { 1, "newalignment_switch_hero", SCMD_ROGUE_SWITCHHERO, {0}, 0,
            "Switch to hero alignment, reset roguepoints" },
    { 1, "newalignment_switch_vigilante", SCMD_ROGUE_SWITCHVIGILANTE, {0}, 0,
            "Switch to hero vigilante, reset roguepoints" },
    { 1, "newalignment_switch_rogue", SCMD_ROGUE_SWITCHROGUE, {0}, 0,
            "Switch to rogue alignment, reset roguepoints" },
    { 1, "newalignment_switch_villain", SCMD_ROGUE_SWITCHVILLAIN, {0}, 0,
            "Switch to villain alignment, reset roguepoints" },
    { 1, "newalignment_switch_tyrant", SCMD_ROGUE_SWITCHTYRANT, {0}, 0,
            "Switch to tyrant alignment, reset roguepoints" },
    { 1, "alignment_tip_drop", SCMD_ALIGNMENT_DROPTIP, {0}, 0,
            "Drop a tip for the going rogue alignment system" },
    { 1, "alignment_reset_timers", SCMD_ALIGNMENT_RESETTIMERS, {0}, 0,
            "Reset the timers that limit you to earning five alignment points per day" },
    { 1, "alignment_reset_single_timer", SCMD_ALIGNMENT_RESETSINGLETIMER, {{CMDINT(tmp_int)}}, 0,
            "Reset a single timer that is limiting your alignment point earnings." },
    { 1, "tip_drop_designer", SCMD_DROPTIP_DESIGNER, {{CMDSTR(tmp_str)}}, 0,
            "Drop a designer tip specified." },

    { 1, "tip_drop_designer_show_all", SCMD_DROPTIP_DESIGNER_LIST_ALL, {0}, 0,
            "Show which tips can be dropped"},
    { 9, "pop_help", SCMD_POP_HELP, {{CMDSTR(tmp_str)}}, 0,
            "Invoke a pop help message" },
    { 9, "pop_help_reset", SCMD_POP_HELP_RESET, {0}, 0,
            "Reset all pop help messages to untriggered" },

    { 9, "incarnateslot_activate", SCMD_INCARNATE_SLOT_ACTIVATE, {{CMDSTR(tmp_str)}}, 0,
                            "Activates the named Incarnate slot if it is unlocked." },
    { 9, "incarnateslot_activate_all", SCMD_INCARNATE_SLOT_ACTIVATE_ALL, {0}, 0,
                            "Activates all Incarnate slots that aren't locked." },
    { 9, "incarnateslot_deactivate", SCMD_INCARNATE_SLOT_DEACTIVATE, {{CMDSTR(tmp_str)}}, 0,
                            "Deactivates the named Incarnate slot." },
    { 9, "incarnateslot_deactivate_all", SCMD_INCARNATE_SLOT_DEACTIVATE_ALL, {0}, 0,
                            "Deactivates all Incarnate slots." },
    { 9, "incarnateslot_debugprint", SCMD_INCARNATE_SLOT_DEBUGPRINT, {{CMDSTR(tmp_str)}}, 0,
                            "Outputs to chat whether the named Incarnate slot is locked or not." },
    { 9, "incarnateslot_debugprint_all", SCMD_INCARNATE_SLOT_DEBUGPRINT_ALL, {0}, 0,
                            "Outputs to chat whether all Incarnate slots are locked or not." },
    { 9, "incarnateslot_reward_xp", SCMD_INCARNATE_SLOT_REWARD_EXPERIENCE, {{CMDSTR(tmp_str)}, {CMDINT(tmp_int)}}, 0,
                            "Rewards the player with Incarnate XP.  [Type] [Count]" },

    { 9, "power_disable", SCMD_POWERS_DISABLE, {{CMDSTR(tmp_str)}, {CMDSTR(tmp_str2)}, {CMDSTR(tmp_str3)}}, 0,
                            "Disables the given power. [cat] [set] [pow]" },
    { 9, "power_enable", SCMD_POWERS_ENABLE, {{CMDSTR(tmp_str)}, {CMDSTR(tmp_str2)}, {CMDSTR(tmp_str3)}}, 0,
                            "Enables the given power. [cat] [set] [pow]" },
    { 9, "power_debugprint_disabled", SCMD_POWERS_DEBUGPRINT_DISABLED, {{CMDSTR(tmp_str)}, {CMDSTR(tmp_str2)}, {CMDSTR(tmp_str3)}}, 0,
                            "Prints whether the given power is disabled or enabled. [cat] [set] [pow]" },
    { 9, "incarnate_grant", SCMD_INCARNATE_GRANT, {{CMDSTR(tmp_str)}, {CMDSTR(tmp_str2)}}, 0,
                            "Grants the given Incarnate Ability, but does not slot that ability. [slot name] [abil name]" },
    { 9, "incarnate_revoke", SCMD_INCARNATE_REVOKE, {{CMDSTR(tmp_str)}, {CMDSTR(tmp_str2)}}, 0,
                            "Revokes the given Incarnate Ability. [slot name] [abil name]" },
    { 9, "incarnate_grant_all", SCMD_INCARNATE_GRANT_ALL, {0}, 0,
                            "Grants all Incarnate Abilities, but does not slot any of them." },
    { 9, "incarnate_revoke_all", SCMD_INCARNATE_REVOKE_ALL, {0}, 0,
                            "Revokes all Incarnate Abilities." },
    { 9, "incarnate_grant_all_by_slot", SCMD_INCARNATE_GRANT_ALL_BY_SLOT, {{CMDSTR(tmp_str)}}, 0,
                            "Grants all Incarnate Abilities that fit into the given slot, but does not slot any of those abilities. [slot name]" },
    { 9, "incarnate_revoke_all_by_slot", SCMD_INCARNATE_REVOKE_ALL_BY_SLOT, {{CMDSTR(tmp_str)}}, 0,
                            "Revokes all Incarnate Abilities that fit into the given slot. [slot name]" },
    { 9, "incarnate_debugprint_has_in_inventory", SCMD_INCARNATE_DEBUGPRINT_HAS_IN_INVENTORY, {{CMDSTR(tmp_str)}, {CMDSTR(tmp_str2)}}, 0,
                            "Prints whether you have the specified Incarnate Ability or not. [slot name] [abil name]" },
    { 0, "incarnate_equip", SCMD_INCARNATE_EQUIP, {{CMDSTR(tmp_str)}, {CMDSTR(tmp_str2)}}, 0,
                            "Equips the specified Incarnate Ability.  Does nothing if you don't have it or its slot is locked.  Also does nothing if you or your teammates are in combat, if the currently equipped IA is recharging, or if it's been less than five minutes since that slot was last equipped.  [slot name] [abil name]" },
    { 0, "incarnate_unequip", SCMD_INCARNATE_UNEQUIP, {{CMDSTR(tmp_str)}, {CMDSTR(tmp_str2)}}, 0,
                            "Unequips the specified Incarnate Ability.  Does nothing if you don't have it or its slot is locked.  Also does nothing if you or your teammates are in combat, if the currently equipped IA is recharging, or if it's been less than five minutes since that slot was last equipped.  [slot name] [abil name]" },
    { 0, "incarnate_unequip_by_slot", SCMD_INCARNATE_UNEQUIP_BY_SLOT, {{CMDSTR(tmp_str)}}, 0,
                            "Unequips whatever ability is in the specified Incarnate slot.  Does nothing if the slot is locked or empty.  Also does nothing if you or your teammates are in combat, if the currently equipped IA is recharging, or if it's been less than five minutes since that slot was last equipped.  [slot name]" },
    { 0, "incarnate_unequip_all", SCMD_INCARNATE_UNEQUIP_ALL, {0}, 0,
                            "Unequips all equipped Incarnate Abilities." },
    { 9, "incarnate_force_equip", SCMD_INCARNATE_FORCE_EQUIP, {{CMDSTR(tmp_str)}, {CMDSTR(tmp_str2)}}, 0,
                            "Equips the specified Incarnate Ability.  Does nothing if you don't have it or its slot is locked.  Ignores other restrictions on equipping IAs.  [slot name] [abil name]" },
    { 9, "incarnate_force_unequip", SCMD_INCARNATE_FORCE_UNEQUIP, {{CMDSTR(tmp_str)}, {CMDSTR(tmp_str2)}}, 0,
                            "Unequips the specified Incarnate Ability.  Does nothing if you don't have it or its slot is locked.  Ignores other restrictions on equipping IAs.  [slot name] [abil name]" },
    { 9, "incarnate_force_unequip_by_slot", SCMD_INCARNATE_FORCE_UNEQUIP_BY_SLOT, {{CMDSTR(tmp_str)}}, 0,
                            "Unequips whatever ability is in the specified Incarnate slot.  Does nothing if the slot is locked or empty.  Ignores other restrictions on equipping IAs.  [slot name] [abil name]" },
    { 9, "incarnate_debugprint_is_equipped", SCMD_INCARNATE_DEBUGPRINT_IS_EQUIPPED, {{CMDSTR(tmp_str)}, {CMDSTR(tmp_str2)}}, 0,
                            "Prints whether the specified Incarnate Ability is equipped or not. [slot name] [abil name]" },
    { 9, "incarnate_debugprint_get_equipped", SCMD_INCARNATE_DEBUGPRINT_GET_EQUIPPED, {{CMDSTR(tmp_str)}}, 0,
                            "Prints what ability is equipped in the specified Incarnate slot. [slot name]" },

    { 9, "start_zone_event", SCMD_START_ZONE_EVENT, {{ CMDSTR(tmp_str) }, { CMDINT(tmp_int) }}, 0,
            "Load and start a zone event" },
    { 9, "stop_zone_event", SCMD_STOP_ZONE_EVENT, {{ CMDSTR(tmp_str) }}, 0,
            "Stop a zone event" },
    { 9, "goto_stage", SCMD_GOTO_STAGE, {{ CMDSTR(tmp_str) }, { CMDSTR(tmp_str2) }}, 0,
            "Go to a particular stage in a zone event" },
    { 9, "disable_zone_event", SCMD_DISABLE_ZONE_EVENT, {{ CMDSTR(tmp_str) }}, 0,
            "Disable a zone event, to prevent it being started" },
    { 9, "zone_event_kill_debug", SCMD_ZONE_EVENT_KILL_DEBUG, {{CMDINT(tmp_int)}}, 0,
            "Toggle zone event debug info for kills" },
    { 9, "turnstile_debug_set_map_info", SCMD_SET_TURNSTILE_MAP_INFO, {{CMDINT(tmp_int)},{CMDINT(tmp_int2)},{CMDINT(tmp_int3)}}, 0,
        "Debug command to set the map info for local mapserver" },
    { 9, "turnstile_debug_get_map_info", SCMD_GET_TURNSTILE_MAP_INFO, {{0}}, 0,
        "Debug command to get the map info for local mapserver" },
    { 9, "zone_event_set_karma_mod",    SCMD_ZONE_EVENT_KARMA_MODIFY, {{CMDSTR(tmp_str)}, {CMDSTR(tmp_str2)}, {CMDINT(tmp_int)}}, 0,
            "Override karma modifier value"},

//    { 9, "debug_db_id", 0, {{CMDINT(server_state.debug_db_id)}}, 0,
//            "Select db_id for debugging" },
//    { 0, "assert_on_bs_errors", SCMD_ASSERT_ON_BITSTREAM_ERRORS, {{CMDINT(tmp_int)}}, 0,
//            "Tells debug dbid's client to assert on a BS error" },

    { 9, "set_combat_mod_shift", SCMD_SET_COMBAT_MOD_SHIFT, {{ CMDINT(tmp_int) }}, 0,
            "Sets your CombatModShift to the specified value (0-6 is valid)." },
    { 9, "goto_marker", SCMD_GOTO_MARKER, {{ CMDSTR(tmp_str) }}, 0,
            "Go to a named script marker" },
    { 0, "title_change", SCMD_CHANGE_TITLE, {{ 0 }}, 0,
            "players can now change their titles" },
    { 9, "named_teleport", SCMD_NAMED_TELEPORT, {{ CMDSTR(tmp_str) }}, 0,
            "Go to a named location (map.spawn) or special place (eg. \"Ouroboros\")." },

    { 9, "fakeauc_add_salvage", SCMD_FAKEAUCTION_ADD_SALVAGE, {{CMDSTR(tmp_str)}, {CMDINT(tmp_int)}, {CMDINT(tmp_int2)}}, 0,
        "Pump salvage on AuctionServer for fake seller: <salvagename> <copies> <price>" },
    { 9, "fakeauc_add_recipe", SCMD_FAKEAUCTION_ADD_RECIPE, {{CMDSTR(tmp_str)}, {CMDINT(tmp_int)}, {CMDINT(tmp_int2)}, {CMDINT(tmp_int3)}}, 0,
        "Pump recipe on AuctionServer for fake seller: <item> <copies> <price> <ingredients>" },
    { 9, "fakeauc_add_set", SCMD_FAKEAUCTION_ADD_SET,  {{CMDSTR(tmp_str)}, {CMDINT(tmp_int)}, {CMDINT(tmp_int2)}, {CMDINT(tmp_int3)}, {CMDINT(tmp_int4)}}, 0,
        "Pump set on AuctionServer for fake seller: <setbase> <minlvl> <maxlvl> <copies> <price>" },
    { 9, "fakeauc_purge", SCMD_FAKEAUCTION_PURGE, {{ 0 }}, 0,
        "Purge AuctionServer of all fake sales" },
    { 9, "lock_doors", SCMD_LOCK_DOORS, {{ CMDINT(tmp_int) }}, 0,
        "Lock/unlock all doors" },
    { 9, "mempooldebug", SCMD_ENABLEMEMPOOLDEBUG, {{ 0 }}, 0,
        "turns on mempool debugging" },

    { 9, "tss_xfer_out", SCMD_TSS_XFER_OUT,  {{ CMDSTR(tmp_str) }}, 0,
        "Tell the turnstileserver to shard visitor xfer the character out" },
    { 9, "tss_xfer_back", SCMD_TSS_XFER_BACK, {{ 0 }}, 0,
        "Tell the turnstileserver to shard visitor xfer the character back" },

    { 9, "group_hide", SCMD_GROUP_HIDE, {{ CMDSTR(tmp_str) }, { CMDINT(tmp_int) }}, 0,
        "Set the gameplay visibility state for a named world group. 1 to hide the group; 0 to unhide" },

    { 2, "eventhistory_find", SCMD_EVENTHISTORY_FIND, {{CMDINT(tmp_int)}, {CMDINT(tmp_int2)}, {CMDSTR(tmp_str)}, {CMDSTR(tmp_str2)}}, 0,
        "Search for Karma event history rewards." },

    { 9, "autocommand_add", SCMD_AUTOCOMMAND_ADD, {{CMDSTR(tmp_str)}, {CMDINT(tmp_int)}, {CMDINT(tmp_int2)}, {CMDINT(tmp_int3)}}, 0,
        "Create an Auto Command that will be run at a specified number of days, hours, and minutes from now.  Format:  /autocommand_create \"command_name param1 param2 ... paramN\" days hours minutes" },
    { 9, "autocommand_delete", SCMD_AUTOCOMMAND_DELETE, {{CMDINT(tmp_int)}}, 0,
        "Delete the Auto Command specified by the ID parameter" },
    { 9, "autocommand_showall", SCMD_AUTOCOMMAND_SHOWALL, {{0}}, 0,
        "Display all Auto Commands." },
    { 9, "autocommand_showbycommand", SCMD_AUTOCOMMAND_SHOWBYCOMMAND, {{CMDSTR(tmp_str)}}, 0,
        "Display all Auto Commands that run the specified command." },
    { 9, "autocommand_testrun", SCMD_AUTOCOMMAND_TESTRUN, {{CMDSTR(tmp_str)}}, 0,
        "Perform a test run on the AutoCommands system as though you just logged in now and the last time you logged in was at the specified time.  Format:  \"MM/DD/YYYY HH:MM\"" },

    { 9, "weeklyTF_addToken", SCMD_REWARD_WEEKLY_TF_ADD, {{CMDSTR(tmp_str)}}, 0,
        "Adds the token to the weekly TF token list" },
    { 9, "weeklyTF_removeToken", SCMD_REWARD_WEEKLY_TF_REMOVE, {{CMDSTR(tmp_str)}}, 0,
        "Removes the token from the weekly TF token list" },
    { 9, "weeklyTF_setEpochTime", SCMD_REWARD_WEEKLY_TF_SET_EPOCH_TIME, {{CMDSTR(tmp_str)}, {CMDSTR(tmp_str2)}}, 0,
        "Set the time when the epoch starts" },
    { 9, "weeklyTF_db_updateTokenList", SCMD_REWARD_DB_WEEKLY_TF_UPDATE, {{CMDSTR(tmp_str)}}, 0,
        "DB relay command to update the weekly token list" },
    { 3, "weeklyTF_printActiveTokens", SCMD_REWARD_WEEKLY_TF_PRINT_ACTIVE, {{0}}, 0,
        "Prints the current weekly TF token list" },
    { 9, "weeklyTF_printAllToken", SCMD_REWARD_WEEKLY_TF_PRINT_ALL, {{0}}, 0,
        "Prints the list of all available weekly TF tokens" },

    { 9, "contactdebugoutputflowchartfile", SCMD_CONTACT_DEBUG_OUTPUT_FLOWCHART_FILE, {{0}}, 0,
        "Creates the C:/contactFlowchartInfo.txt file that is used as input for the flowchart program." },

    { 0, "sethelperstatus", SCMD_SET_HELPER_STATUS, {{CMDINT(tmp_int)}}, 0,
        "Sets your helper status.  0 = off, 1 = help me!, 2 = mentor" },
    { 9, "testlogging", SCMD_TEST_LOGGING, {{0}}, 0,
        "Prints out a log message for each logging level" },
    { 9, "nx_server_PhysXDebug", SCMD_NX_PHYSX_CONNECT, {{ 0 }}, 0,
        "Connects server PhysX to physx visual debugger." },
    { 9, "setragdoll", SCMD_NX_RAGDOLL_COUNT, {{CMDINT(tmp_int)}}, 0,
            "Sets the number of ragdolls allowed on this server" },
    { 9, "accessiblecontactdebug_getfirst", SCMD_ACCESSIBLE_CONTACTS_DEBUG_GET_FIRST, {{0}}, 0,
        "Resets your current accessible contact index to zero and then prints out what that contact is." },
    { 9, "accessiblecontactdebug_getnext", SCMD_ACCESSIBLE_CONTACTS_DEBUG_GET_NEXT, {{0}}, 0,
        "Increases your current accessible contact index by one and then prints out what that contact is." },
    { 9, "accessiblecontactdebug_getprevious", SCMD_ACCESSIBLE_CONTACTS_DEBUG_GET_PREVIOUS, {{0}}, 0,
        "Decreases your current accessible contact index by one and then prints out what that contact is." },
    { 9, "accessiblecontactdebug_getcurrent", SCMD_ACCESSIBLE_CONTACTS_DEBUG_GET_CURRENT, {{0}}, 0,
        "Prints out what the currently accessible contact is." },
    { 0, "contactfinder_showcurrent", SCMD_ACCESSIBLE_CONTACTS_SHOW_CURRENT, {{0}}, 0,
        "Shows the current contact in the Contact Finder window." },
    { 0, "contactfinder_shownext", SCMD_ACCESSIBLE_CONTACTS_SHOW_NEXT, {{0}}, 0,
        "Shows the next contact in the Contact Finder window." },
    { 0, "contactfinder_showprevious", SCMD_ACCESSIBLE_CONTACTS_SHOW_PREVIOUS, {{0}}, 0,
        "Shows the previous contact in the Contact Finder window." },
    { 0, "contactfinder_teleporttocurrent", SCMD_ACCESSIBLE_CONTACTS_TELEPORT_TO_CURRENT, {{0}}, 0,
        "Teleports you to the contact currently detailed in the Contact Finder window.  Only works if you have not yet been introduced to the contact." },
    { 0, "contactfinder_selectcurrent",    SCMD_ACCESSIBLE_CONTACTS_SELECT_CURRENT, {{0}}, 0,
        "Selects the contact currently detailed in the Contact Finder window.  Only works if you have already been introduced to the contact." },

    { 0, "debug_disableautodismiss", SCMD_DEBUG_DISABLE_AUTO_DISMISS, {{CMDINT(tmp_int)}}, 0,
        "Enables you to turn on and off auto-dismissal of contacts." },
    { 9, "get_MARTY_status", SCMD_GET_MARTY_STATUS, {{0}}, 0, 
        "get MARTY for current map" },
    { 9, "set_MARTY_status", SCMD_SET_MARTY_STATUS, {{CMDINT(tmp_int)}}, 0, 
        "Enable/disable MARTY for entire shard" },
    { 9, "SetMARTYStatusRelay", SCMD_SET_MARTY_STATUS_RELAY, {{CMDINT(tmp_int)}}, 0,
        "Enable/disable MARTY for server" },
    { 2, "csr_clearMARTY",        SCMD_CSR_CLEAR_MARTY_HISTORY,{{CMDSTR(player_name)}},CMDF_HIDEVARS,
        "clear MARTY history of <player>" },
    { 2, "csr_printMARTY",        SCMD_CSR_PRINT_MARTY_HISTORY,{{CMDSTR(player_name)}},CMDF_HIDEVARS,
        "print MARTY history of <player>" },
    { 9, "nokick",        SCMD_NOKICK,{{CMDINT(tmp_int)}},CMDF_HIDEVARS,
        "sets the noKick flag on player" },
    { 9, "idle_exit_timeout",    0, {{CMDINT(server_state.idle_exit_timeout)}},0,
        "shutdown the server if it is idle for this many minutes, 0 means there is no idle exit timeout" },
    { 9, "client_logout_time",    0, {{CMDINT(server_state.client_logout_time)}},0,
        "timeout in seconds for for the disconnect countdown when players are logging out" },
    { 9, "flashback_left_reward_apply",    SCMD_FLASHBACK_LEFT_REWARD_APPLY, {{CMDINT(tmp_int)}, CMDINT(tmp_int2)}, 0,
        "Tell the specified player to reward himself FlashbackLeft from the specified story arc." },
    { 9, "debug_set_vip",    SCMD_DEBUG_SET_VIP, {{CMDINT(tmp_int)}}, 0,
        "Sets the vip state on development shards." },
    { 5, "account_recover_unsaved",    SCMD_ACCOUNT_RECOVER_UNSAVED, {{0}}, 0,
        "Attempts force recovery of unsaved transactions." },
    { 9, "open_salvage_by_name", SCMD_OPEN_SALVAGE_BY_NAME, {{CMDSTR(tmp_str)}}, 0,
        "Open a salvage.  Takes a string parameter that is the internal name of the salvage to open.  You must have that salvage in your inventory."    },
    { 3, "support_home", SCMD_SUPPORT_HOME, {{0}}, 0, "Show the support home page" },
    { 3, "support_kb", SCMD_SUPPORT_KB, {{CMDINT(tmp_int)}}, 0, "Show a support KB article" },
    { 9, "show_lua_lib", SCMD_SHOW_LUA_LIB, {{CMDSTR(tmp_str)}}, 0, "Show functions of a specific Lua Library" },
    { 9, "show_lua_all", SCMD_SHOW_LUA_ALL, {{0}}, 0, "Show functions of all Lua Libraries" },
    { 9, "new_feature_open", SCMD_NEW_FEATURE_OPEN, {{CMDINT(tmp_int)}}, 0, "Opens the New Feature on the client <featureId> " },
    { 9, "display_product_page", SCMD_DISPLAY_PRODUCT_PAGE, {{CMDSTR(tmp_str)}}, 0, "Open a store product page on the client" },
    { 9, "force_queue_for_events", SCMD_FORCE_QUEUE_FOR_EVENT, {{CMDSTR(tmp_str)}}, 0, "Force queue client for turnstile event, by internal event name." },
    { 9, "set_zmq_connect_state", SCMD_SET_ZMQ_CONNECT_STATE, {{CMDINT(tmp_int)}}, 0, 
         "set_zmq_connect_state <0 or 1> - 0: disconnects ZMQ socket, stopping logserver from sending log messages to CoH Metrics system, 1: connects ZMQ socket." },
    { 9, "get_zmq_status", SCMD_GET_ZMQ_STATUS, {{0}}, 0, "get status of CoH Metrics system's ZeroMQ socket" },
    { 9, "bin_map", SCMD_BIN_MAP, {{0}}, 0, "Write a single bin file for the current map" },    
    { 0 },
};

static char* getBadgeOwnedBits(Entity *e)
{
    static StuffBuff sb = {0};
    if(!sb.buff)
        initStuffBuff( &sb, 512);
    else
        clearStuffBuff( &sb );
    
    if(e)
        badge_PackageOwned(e, &sb);
    return sb.buff;
}


void cmdOldServerStateInit()
{
    memset(&server_visible_state,0,sizeof(server_visible_state));

    //server_state.pause = 0;
    server_state.logSeqStates = 0;
    server_state.logServerTicks = 0;
    server_state.xpscale = 1.0;
    server_state.influencescale = 1.0;
    server_state.prestigescale = 1.0;
    server_state.aescale = 1.0;
    server_state.aeinfluencescale = 1.0;
    server_state.aeprestigescale = 1.0;
    server_state.ticketscale = 1.0;
    server_state.client_logout_time = 30;
    server_state.aggrocap = 17;
    server_state.targetcapmode = 1;
    server_visible_state.time = 0;
    server_visible_state.timescale = DAY_TIMESCALE;

    server_state.enablePowerDiminishing = false;
    server_state.enableTravelSuppressionPower = false;
    server_state.enableHealDecay = false;
    server_state.enablePassiveResists = false;
    server_state.enableSpikeSuppression = false;
    server_state.inspirationSetting = ARENA_ALL_INSPIRATIONS;
}

CmdList server_cmdlist =
{
    {
        { control_cmds },
        { server_visible_cmds },
        { server_cmds },
        { chat_cmds },
        { client_sgstat_cmds },
        { server_sgstat_cmds },
        { g_auction_cmds }, 
        { g_auction_res },
        { g_accountserver_cmds_server },
        { 0 }
    }
};

void serverStateLoad(char *config_file)
{
    FILE    *file;
    char    buf[1000];
    Cmd        *cmd;
    CmdContext context = {0};

    context.access_level = ACCESS_INTERNAL;

    cmdOldServerStateInit();
    file = fileOpen(config_file, "rt");
    if(!file)
        file = fileOpen("svrconfig.txt","rt");
    if (!file)
        file = fileOpen("./svrconfig.txt","rt");
    if (!file)
        file = fileOpen("c:/svrconfig.txt","rt");
    if (!file)
        return;
    while(fgets(buf,sizeof(buf),file))
    {
        cmd = cmdOldRead(&server_cmdlist,buf,&context);
        if(cmd)
        {
            serverExecCmd(cmd, NULL, buf, NULL, NULL);
        }
        else
        {
            extern void parseArgs2(int argc,char **argv);
            char *args[100],*line2;
            int count;

            // HACK: &args[1] is used since parseArgs2 ignores the first
            //   parameter.
            count = tokenize_line_safe(buf, &args[1], ARRAY_SIZE(args)-1, &line2);
            parseArgs2(count+1, args);
        }
    }
    fileClose(file);
}

static char *fitUnescapeString(char *esc_str,char **buf,int *maxlen)
{
    const char    *s;

    s = unescapeString(esc_str);
    dynArrayFit(buf,1,maxlen,strlen(s)+1);
    strcpy(*buf,s);
    return *buf;
}

int hide_mask = ((1<<HIDE_SG)|(1<<HIDE_FRIENDS)|(1<<HIDE_GLOBAL_FRIENDS)|(1<<HIDE_GLOBAL_CHANNELS)|(1<<HIDE_SEARCH));
static const char *expandSingleVar(const char *variable,int *cmdlen,ClientLink *client)
{
    Entity    *e;

    if (strnicmp(variable,"target",strlen("target"))==0 )
    {
        *cmdlen = strlen("target");
        if (client->controls.selected_ent_db_id)
        {
            OnlinePlayerInfo *opi = dbGetOnlinePlayerInfo(client->controls.selected_ent_db_id);
            if( opi && (!(opi->hidefield&hide_mask)) )
                return dbPlayerNameFromId(client->controls.selected_ent_db_id);
        }

        e = validEntFromId(client->controls.selected_ent_server_index);
        if(!e) // don't need to follow hide mask here, they are selected on same map
            return 0;
        return e->name;
    }
    else if (strnicmp(variable,"archetype",strlen("archetype"))==0)
    {
        *cmdlen = strlen("archetype");

        if (!client->entity || !client->entity->pchar || !client->entity->pchar->pclass )
            return 0;

        return clientPrintf(client,client->entity->pchar->pclass->pchDisplayName);
    }
    else if (strnicmp(variable,"origin",strlen("origin"))==0)
    {
        *cmdlen = strlen("origin");

        if (!client->entity || !client->entity->pchar || !client->entity->pchar->porigin)
            return 0;

        return clientPrintf(client,client->entity->pchar->porigin->pchDisplayName);
    }
    else if (strnicmp(variable,"name",strlen("name"))==0)
    {
        *cmdlen = strlen("name");

        if (!client->entity)
            return 0;

        return client->entity->name;
    }
    else if (strnicmp(variable,"battlecry",strlen("battlecry"))==0)
    {
        *cmdlen = strlen("battlecry");

        if (!client->entity || !client->entity->strings )
            return 0;

        return client->entity->strings->motto;
    }
    else if (strnicmp(variable,"level",strlen("level"))==0)
    {
        static char s_achLevel[10];

        *cmdlen = strlen("level");

        if (!client->entity || !client->entity->pchar)
            return 0;

        sprintf(s_achLevel, "%d", client->entity->pchar->iLevel+1);
        return s_achLevel;
    }
    else if (strnicmp(variable,"primary",strlen("primary"))==0)
    {
        int i;
        *cmdlen = strlen("primary");
        e = client->entity; 
        if (!e || !e->pchar)
            return 0;
        
        for( i = 0; i < eaSize(&e->pchar->ppPowerSets); i++ )
        {
            if( e->pchar->ppPowerSets[i]->psetBase->pcatParent == e->pchar->pclass->pcat[kCategory_Primary])
                return localizedPrintf(e, e->pchar->ppPowerSets[i]->psetBase->pchDisplayName);
        }
    }
    else if (strnicmp(variable,"secondary",strlen("secondary"))==0)
    {
        int i;
        *cmdlen = strlen("secondary");
        e = client->entity;
        if (!e || !e->pchar)
            return 0;

        for( i = 0; i < eaSize(&e->pchar->ppPowerSets); i++ )
        {
            if( e->pchar->ppPowerSets[i]->psetBase->pcatParent == e->pchar->pclass->pcat[kCategory_Secondary])
                return localizedPrintf(e, e->pchar->ppPowerSets[i]->psetBase->pchDisplayName);
        }
    }
    else if (strnicmp(variable,"supergroup",strlen("supergroup"))==0)
    {
        *cmdlen = strlen("supergroup");
        e = client->entity;
        if (!e || !e->supergroup)
            return 0;
        if( e->supergroup && e->supergroup->name )
            return e->supergroup->name;
        else
            return localizedPrintf(e, "NoSupergroupString" );
    }
    else if (strnicmp(variable,"sgmotto",strlen("sgmotto"))==0)
    {
        *cmdlen = strlen("sgmotto");
        e = client->entity;
        if (!e || !e->supergroup)
            return 0;
        if( e->supergroup && e->supergroup->motto )
            return e->supergroup->motto;
        else
            return localizedPrintf(e, "NoSupergroupString" );
    }
    else if (strnicmp(variable,"title1",strlen("title1"))==0)
    {
        *cmdlen = strlen("title1");
        e = client->entity;
        if (!e || !e->pl || !e->pl->titleCommon)
            return 0;
        return e->pl->titleCommon;
    }
    else if (strnicmp(variable,"title2",strlen("title2"))==0)
    {
        *cmdlen = strlen("title2");
        e = client->entity;
        if (!e || !e->pl || !e->pl->titleOrigin)
            return 0;
        return e->pl->titleOrigin;
    }
    else if (strnicmp(variable,"server",strlen("server"))==0)
    {
        *cmdlen = strlen("server");
        return localizedPrintf(0, db_state.server_name);
    }
    else if (strnicmp(variable,"side",strlen("side"))==0)
    {
        *cmdlen = strlen("side");
        e = client->entity;
        if (!e || !e->pl)
            return 0;
        if (ENT_IS_HERO(e))
            return localizedPrintf(e, "HeroOnlyString" );
        else
            return localizedPrintf(e, "VillainOnlyString" );
    }

    return 0;
}

static void expandVars(const char *in_str,char *out_str,int max_out,ClientLink *client)
{
    const char *s;
    const char *var;
    int idx=0,cmdlen;

    if (!client)
    {
        strncpy(out_str,in_str,max_out);
        return;
    }
    for(s=in_str;*s;s++)
    {
        if (*s != '$')
        {
            if (idx >= max_out)
                break;
            out_str[idx++] = *s;
            continue;
        }
        var = expandSingleVar(s+1,&cmdlen,client);
        if (var)
        {
            int        len = MIN((int)strlen(var),max_out-idx);

            strncpy(&out_str[idx],var,len);
            idx += len;
            s += cmdlen;
        }
        else
        {
            if (idx >= max_out)
                break;
            out_str[idx++] = *s;
        }
    }
    out_str[idx] = 0;
}


void log_printPowersMultiLine(char *str)
{
    if(str && *str)
    {
        char *pos;
        char *start = str;

        while(pos = strchr(start, '\n'))
        {
            *pos = '\0';
            LOG( LOG_POWERS, LOG_LEVEL_IMPORTANT, 0, "%s", start);
            *pos = '\n';
            start = pos+1;
        }

        if(*start && *start!='\n')
            LOG( LOG_POWERS, LOG_LEVEL_IMPORTANT, 0, "%s", start);
    }
}

#define ENT_MISSION_STUCK_TOKEN "CompleteStuckMission"
#define ENT_MISSION_STUCK_RETRY_DELAY (60*60*24*3) // 3 days
void entity_MissionStuckSetComplete(Entity *e, ClientLink *client, int success)
{
    RewardToken *t = getRewardToken(e,ENT_MISSION_STUCK_TOKEN);

    if(!devassert(e && client))
        return;
    
    if( t && dbSecondsSince2000() < ENT_MISSION_STUCK_RETRY_DELAY + t->timer )
    {
        char datestr[256];
        timerMakeLogDateStringFromSecondsSince2000(datestr, ENT_MISSION_STUCK_RETRY_DELAY + t->timer);
        
        conPrintf(client, clientPrintf(client,"MissionStuckRetry_TooSoon",datestr));
    }
    else
    {
        TaskDebugComplete(e,0,success);
        giveRewardToken(e,ENT_MISSION_STUCK_TOKEN);
        conPrintf(client, clientPrintf(client,(success?"MissionStuckRetry_Success":"MissionStuckRetry_Failure")));
    }
}

void commandSelectContact(Entity *player, char *contactFilename)
{
    ContactHandle chandle = ContactGetHandle(contactFilename);
    player->storyInfo->delayedSelectedContact = chandle;
}

void commandMapMovePos(Entity *player, int map_id, Vec3 position, int exact, int findBest)
{
    if (map_id != db_state.map_id)
    {
        sprintf(player->spawn_target, "%0.2f:%0.2f:%0.2f:%d", position[0], position[1], position[2], exact);
        player->adminFrozen = 1;
        player->db_flags |= DBFLAG_TELEPORT_XFER;
        dbAsyncMapMove(player, map_id, NULL, XFER_STATIC | (findBest?XFER_FINDBESTMAP:0));
    }
    else
    {
        // skip mapmove & move near player's position
        setInitialTeleportPosition(player, position, exact);
    }
}

Entity **g_dbg_aucitemstatusreqs = NULL;
static void serverExecCmd(Cmd *cmd, ClientLink *client, char *source_str, Entity *e, CmdContext *output)
{
    char *str = source_str;
    ClientLink *admin = client;

    if ((cmd->access_level > ACCESS_USER) && e) {
        LOG((cmd->access_level < ACCESS_DEBUG) ? LOG_ACCESS_CMDS : LOG_INTERNAL_CMDS, LOG_LEVEL_IMPORTANT, 0, "%s:\"%s\"", e->name, source_str);
    }

    switch(cmd->num)
    {
        xcase SVCMD_TIME:
            sprintf(output->msg,"time %f",server_visible_state.time);
            cmdOldServerReadInternal(output->msg);
        xcase SVCMD_TIMESTEPSCALE:
        {
            if(!isDevelopmentMode())
            {
                server_visible_state.timestepscale = 1;
            }
            else
            {
                if(server_visible_state.timestepscale <= 0)
                {
                    server_visible_state.timestepscale = 1;
                }
                else
                {
                    server_visible_state.timestepscale = MINMAX(server_visible_state.timestepscale, 0.00001, 4);
                }
            }
        }
        xcase SCMD_TIMEOFFSET:
        {
            if(tmp_str)
            {
                int oldOffset = timing_debug_offset;
                int nDays = 0, nHours = 0, nMinutes = 0, nSeconds = 0;
                int i;
                char *intString = strtok(tmp_str, ":");
                
                for(i = 0; i < 4 && intString; i++)
                {
                    int units = atoi_ignore_nonnumeric(intString);
                    switch (i)
                    {
                        xcase 0: //days 86400 seconds
                            nDays = units;
                        xcase 1: //hours 3600 seconds
                            nHours = units;
                        xcase 2: //minutes 60 seconds
                            nMinutes = units;
                        xcase 3: //seconds 1 second
                            nSeconds = units;
                    }
                    intString = strtok(NULL, ":");
                }

                if(i == 4 && intString == NULL)
                {
                    timing_debug_offset = nDays*86400 + nHours*3600 + nMinutes*60 + nSeconds;
                    conPrintf(client, "timing debug offset changed from %d to %d or %d days:%d hours:%d minutes: %d seconds, debug server time %s", oldOffset, timing_debug_offset, nDays, nHours, nMinutes, nSeconds, timerMakeOffsetDateStringNoYearNoSecFromSecondsSince2000(dbSecondsSince2000()));
                }
                else
                {
                    conPrintf(client, "ERROR!  Please make sure you're setting the time with days:hours:minutes:seconds.  If you want to set it to 2 hours and 2 seconds, that would be 0:2:0:2");
                }
            }
        }
        xcase SCMD_TIMEDEBUG:
        {
            conPrintf(client, "timing debug offset is %d, debug server time %s", timing_debug_offset, timerMakeOffsetDateStringNoYearNoSecFromSecondsSince2000(dbSecondsSince2000()));
        }
        xcase SCMD_TIMESET:
        {
            svrTimeSet(tmp_float);
        }
        xcase SCMD_TIMESETALL:
        {
            char    buf[100];

            sprintf(buf,"cmdrelay\ntimeset %f",tmp_float);
            sendToAllServers(buf);
        }
        xcase SVCMD_PVPMAP:
        {
            int n, i;

            // turn on default PvP settings
            server_state.enablePowerDiminishing = server_visible_state.isPvP;
            server_state.enableTravelSuppressionPower = server_visible_state.isPvP;
            server_state.enableHealDecay = server_visible_state.isPvP;
            server_state.enablePassiveResists = server_visible_state.isPvP;
            server_state.enableSpikeSuppression = false;
            server_state.inspirationSetting = ARENA_ALL_INSPIRATIONS;

            sprintf(output->msg,"pvpmap %i",server_visible_state.isPvP);
            cmdOldServerReadInternal(output->msg);

            // setup anyone who's already on the map
            n = NumEntitiesInTeam(ALL_PLAYERS_READYORNOT);
            for (i = 0; i < n; i++)
            {
                Entity* e = EntTeamInternal(ALL_PLAYERS_READYORNOT, i, NULL);

                pvp_ApplyPvPPowers(e);
            }
        }
        xcase SVCMD_FOG_DIST:
            sprintf(output->msg,"svr_fog_dist %f",server_visible_state.fog_dist);
            cmdOldServerReadInternal(output->msg);
        xcase SVCMD_FOG_COLOR:
            sprintf(output->msg,"svr_fog_color %f %f %f", server_visible_state.fog_color[0], server_visible_state.fog_color[1], server_visible_state.fog_color[2] );
            cmdOldServerReadInternal(output->msg);
        xcase SCMD_CLEARAI_LOG:
        {
            int i;
            for(i = 0; i < entities_max; i++){
                void aiLogClear(Entity*);

                aiLogClear(entFromId(i));
            }
        }

        xcase SCMD_ENTDEBUGINFO:
            printEntDebugInfo(client,tmp_int,output);
        xcase SCMD_RUNNERDEBUG:
			if (tmp_int)
				client->entDebugInfo = ENTDEBUGINFO_RUNNERDEBUG;
			else
				client->entDebugInfo = 0;            
        xcase SCMD_SETDEBUGVAR:
            clientLinkSetDebugVar(client, tmp_str, (void*)(intptr_t)tmp_int);
        xcase SCMD_GETDEBUGVAR:
            conPrintf(client, "%s == %d", tmp_str, (int)(intptr_t)clientLinkGetDebugVar(client, tmp_str));
        xcase SCMD_SENDAUTOTIMERS:
            client->sendAutoTimers = tmp_int;
        xcase SCMD_SETDEBUGENTITY:
            entSendSetInterpStoreEntityId(tmp_int);
        xcase SCMD_DEBUG_COPYINFO:
            // TODO..
        xcase SCMD_DEBUGMENUFLAGS:
            cmdOldApplyOperation(&client->debugMenuFlags, tmp_int, output);
        xcase SCMD_INTERPDATALEVEL:
            client->interpDataLevel = max(0, min(3, tmp_int));
            if(client->interpDataLevel)
                conPrintf(client, clientPrintf( client, "InterpSet", client->interpDataLevel));
            else
                conPrintf(client, clientPrintf( client, "InterpDisabled"));
        xcase SCMD_INTERPDATAPRECISION:
            client->interpDataPrecision = max(0, min(3, tmp_int));
            conPrintf(client, clientPrintf(client, "InterpPrec"), client->interpDataPrecision);
        xcase SCMD_SETSKYFADE:
            serverSetSkyFade(server_state.skyFade1, server_state.skyFade2, server_state.skyFadeWeight);
        xcase SCMD_SET_POS:
            entUpdatePosInstantaneous(e, setpos );
            zeroVec3(e->motion->vel);
        xcase SCMD_SET_POS_PYR:
        {
            Mat4 mat;
            createMat3YPR( mat, setpyr );
            entSetMat3(e, mat);
            if(e->client)
                copyVec3(setpyr, e->client->controls.pyr.cur);
            entUpdatePosInstantaneous( e, setpos );
            zeroVec3(e->motion->vel);
        }

        xcase SCMD_SCMDS:
            cmdOldPrintList(server_cmds,client,client->entity->access_level,tmp_str,1,0);
            cmdOldPrintList(server_visible_cmds,client,client->entity->access_level,tmp_str,0,0);
            cmdOldPrintList(chat_cmds,client,client->entity->access_level,tmp_str,0,0);
            cmdOldPrintList(client_sgstat_cmds,client,client->entity->access_level,tmp_str,0,0);
            cmdOldPrintList(g_auction_cmds,client,client->entity->access_level,tmp_str,0,0);
            cmdOldPrintList(g_accountserver_cmds_server,client,client->entity->access_level,tmp_str,0,0);
            cmdOldPrintList(meta_getLegacyCmds(), client, client->entity->access_level, tmp_str, 0, 0);

        xcase SCMD_CMDLIST:
            cmdOldPrintList(server_cmds,client,client->entity->access_level,NULL,0,0);
            cmdOldPrintList(server_visible_cmds,client,client->entity->access_level,NULL,0,0);
            cmdOldPrintList(chat_cmds,client,client->entity->access_level,NULL,0,0);
        xcase SCMD_MAKE_MESSAGESTORE:
            cmdOldSaveMessageStore( &server_cmdlist, "cmdMessagesServer.txt" );
            conPrintf( client, "Server Commands saved." );
        xcase SCMD_SCMDUSAGE:
            cmdOldPrintList(server_cmds,client,client->entity->access_level,tmp_str,1,1);
        xcase SCMD_DBQUERY:
            conPrintf(client,"\n%s",dbQuery(tmp_str));
        xcase SCMD_STATUS:
        {
            char    buf[1000];

            sprintf(buf,"-getstatus 1 %d",status_server_id);
            conPrintf(client,"%s",dbQuery(buf));
        }
        xcase SCMD_CSR:
        {
            char buf[1000];
            int online;
            Entity * target;

            int id = dbPlayerIdFromName(player_name);

            if (!e) break;

            target = entFromDbId( id );
            online = player_online(player_name);

            if (id < 0 || // Character doesn't exist or
                (!target && !online) || // He's not online and he's not on this mapserver or
                (target && target->logout_timer)) // He's on this mapserver, and online, but he's currently marked as "quitting"
            {
                conPrintf(client, "%s", clientPrintf(client,"playerNotFound", player_name) );
                break;
            }

            sprintf(buf, "csr_long %d %d \"%s\" %s", client->entity->access_level, client->entity->db_id, player_name, tmp_str);
            serverParseClient(buf, NULL); // NULL to get internal access level
        }
        xcase SCMD_CSR_RADIUS:
        {
            int affected = 0;
            if(!client->entity) break;

            if(tmp_float < 0)
                affected = csrMap(client, tmp_str);
            else
                affected = csrRadius(client, tmp_float, tmp_str);
                
            conPrintf(client, "%s", clientPrintf(client,"CSRRadiusAffected", affected) );
        }
        xcase SCMD_CSR_LONG:
            csrCsr(client, tmp_int, tmp_int2, player_name, tmp_str, str);
        xcase SCMD_CSR_OFFLINE:
        {
            char buf[1000];

            int id = dbPlayerIdFromName(player_name);

            if (!e) break;

            if (id < 0) // Doesn't exist
            {
                conPrintf(client, "%s", clientPrintf(client,"playerNotFound", player_name) );
                break;
            }

            sprintf(buf, "csr_offline_long %d %d %d %s", client->entity->access_level, client->entity->db_id, id, tmp_str);
            serverParseClient(buf, NULL); // NULL to get internal access level
        }
        xcase SCMD_CSR_OFFLINE_LONG:
            csrCsrOffline(client, tmp_int, tmp_int2, tmp_int3, tmp_str, str);
        xcase SCMD_ALTACCESS:
        {
            int id = dbPlayerIdFromName(player_name);

            if (!e) break;

            if (id < 0) // Doesn't exist
            {
                conPrintf(client, "%s", clientPrintf(client, "playerNotFound", player_name));
                break;
            }
            char tAccessLevel = client->entity->access_level;
            //get targetauthname and myauthname
            int tMyAuthId = authIdFromName(client->entity->name);
            int tTargetAuthId = authIdFromName(player_name);

            if (tmp_int > tAccessLevel || tmp_int < 0)
            {
                conPrintf(client, clientPrintf(client, "Your Access Level is not high enough to grant an Access_Level of that value"));
            }
            else
            {
                //write our sql statement for use below
                char sql_Accesscommand[2000];
                snprintf(sql_Accesscommand, sizeof(sql_Accesscommand), "UPDATE dbo.ents SET AccessLevel = %d WHERE name = N'%s' AND active IS NULL;",
                    tmp_int,
                    escapeString(player_name));

                //check current access level 12
                if (tAccessLevel > 12)
                {
                    //Change Access Level to specified
                    dbExecuteSql(sql_Accesscommand);
                    conPrintf(client, clientPrintf(client, "Access Level Changed"));
                }
                //if not on an Access_Level 11 character check if player_name is part of my account
                else if (tMyAuthId == tTargetAuthId)
                {
                    //Change Access Level to specified
                    dbExecuteSql(sql_Accesscommand);
                    conPrintf(client, clientPrintf(client, "Access Level Changed"));
                }
                else
                {
                    //Inform user that they cannot modify someone else since they aren't access level 11
                    conPrintf(client, clientPrintf(client, "You cannot change another users access level!"));
                }
            }
        }
        xcase SCMD_PLAYTIME :
        {
            int totalSeconds = e->total_time;
            int days = totalSeconds / SECONDS_PER_DAY;
            totalSeconds = totalSeconds % SECONDS_PER_DAY;
            int hours = totalSeconds / SECONDS_PER_HOUR;
            totalSeconds = totalSeconds % SECONDS_PER_HOUR;
            int minutes = totalSeconds / SECONDS_PER_MINUTE;
            int seconds = totalSeconds % SECONDS_PER_MINUTE;
            conPrintf(client, "Time Played: %d Days, %d hours, %d minutes, and %d seconds", days, hours, minutes, seconds);
        }
        xcase SCMD_ENTSAVE:
            entSaveAll(client->entity);
        xcase SCMD_ENTCORRUPTED:
            if(e)
            {
                conPrintf(client, "%s\n",
                    clientPrintf(client, e->corrupted_dont_save ? "entCorrupted" : "entNotCorrupted", e->name) );
            }
        xcase SCMD_SETENTCORRUPTED:
            if(e)
            {
                e->corrupted_dont_save = tmp_int ? 1 : 0;
                conPrintf(client, "%s\n",
                    clientPrintf(client, e->corrupted_dont_save ? "entSetCorrupted" : "entSetNotCorrupted", e->name) );
            }

        xcase SCMD_ENTDELETE:
        {
            int id = dbPlayerIdFromName(tmp_str);
            if(id > 0 && id == tmp_int)
            {
                Packet *pak = pktCreateEx(&db_comm_link, DBCLIENT_DELETE_PLAYER);
                pktSendBitsAuto(pak, id);
                pktSend(&pak, &db_comm_link);

                conPrintf(client, "Deleting %s", tmp_str);
            }
            else
            {
                conPrintf(client, "DbId and Name don't match.");
            }
        }

        xcase SCMD_ENTGENLOAD:
        {
            extern char world_name[];
            spawnAreaReload(world_name);
        }
        xcase SCMD_BEACONPROCESS:
        {
            if(isDevelopmentMode()){
                beaconProcess(0, 0, 0);
                beaconPathInit(1);
            }
        }
        xcase SCMD_BEACONPROCESSFORCED:
        {
            if(isDevelopmentMode()){
                beaconProcess(1, 0, 0);
                beaconPathInit(1);
            }
        }
        xcase SCMD_BEACONPROCESSTRAFFIC:
        {
            if(isDevelopmentMode()){
                beaconProcessTrafficBeacons();
            }
        }
        xcase SCMD_BEACONPROCESSNPC:
        {
            if(isDevelopmentMode()){
                beaconProcessNPCBeacons();
            }
        }
        xcase SCMD_BEACONGENERATE:
        {
            if(isDevelopmentMode()){
                beaconProcess(1, 0, 1);
                beaconPathInit(1);
            }
        }
        xcase SCMD_BEACONREADFILE:
        {
            int verbose_level = errorGetVerboseLevel();
            errorSetVerboseLevel(2);
            beaconReload();
            errorSetVerboseLevel(verbose_level);
        }
        xcase SCMD_BEACONREADTHISFILE:
        {
            void aiClearBeaconReferences();
            int verbose_level = errorGetVerboseLevel();
            errorSetVerboseLevel(2);
            aiClearBeaconReferences();
            readBeaconFile(tmp_str);
            beaconRebuildBlocks(0, 1);
            errorSetVerboseLevel(verbose_level);
        }
        xcase SCMD_BEACONWRITEFILE:
        {
            if(isDevelopmentMode()){
                beaconWriteCurrentFile(1);
            }
        }
        xcase SCMD_BEACONFIX:
        {
            if(isDevelopmentMode()){
                //void temp_beaconRepairConnections();
                //void aiClearBeaconReferences();

                //aiClearBeaconReferences();
                //beaconFix();
                //temp_beaconRepairConnections();

                beaconRebuildBlocks(0, tmp_int);
            }
        }
        xcase SCMD_BEACONPATHDEBUG:
        {
            beaconSetDebugClient(tmp_int ? client->entity : NULL);
        }
        xcase SCMD_BEACONRESETTEMP:
        {
            beaconPathResetTemp();
        }
        xcase SCMD_BEACONGOTOCLUSTER:
        {
            beaconGotoCluster(client, tmp_int);
        }
        xcase SCMD_BEACONGETVAR:
        {
            conPrintf(client, "%s => %d", tmp_str, beaconGetDebugVar(tmp_str));
        }
        xcase SCMD_BEACONSETVAR:
        {
            conPrintf(client, "%s <= %d", tmp_str, beaconSetDebugVar(tmp_str, tmp_int));
        }
        xcase SCMD_BEACONCHECKFILE:
        {
            if(isDevelopmentMode())
            {
                if(beaconDoesTheBeaconFileMatchTheMap(0))
                {
                    conPrintf(client, "The beacon file matches!\n");
                }
                else
                {
                    conPrintf(client, "The beacon file DOES NOT MATCH!!!\n");
                }
            }
            else
            {
                conPrintf(client, "You can't check the beacon file in production mode!\n");
            }
        }
        xcase SCMD_BEACONREQUEST:
        {
            beaconRequestBeaconizing(db_state.map_name);
        }
        xcase SCMD_BEACONMASTERSERVER:
        {
            if(!stricmp(tmp_str, "none")){
                beaconRequestSetMasterServerAddress(NULL);
            }else{
                beaconRequestSetMasterServerAddress(tmp_str);
            }
        }
        xcase SCMD_PERFINFOSETBREAK:
        {
            timerSetBreakPoint(tmp_int, tmp_int2, tmp_int3);
        }
        xcase SCMD_PERFINFO_ENABLE_ENT_TIMER:
        {
            enableEntityTimer(tmp_int);
        }
        xcase SCMD_PERFINFO_DISABLE_ENT_TIMERS:
        {
            disableEntityTimers();
        }
        xcase SCMD_PERFINFO_CLIENT_PACKET_LOG:
        {
            clientPacketLogSetEnabled(tmp_int, tmp_int2);
        }
        xcase SCMD_PERFINFO_CLIENT_PACKET_LOG_PAUSE:
        {
            clientPacketLogSetPaused(tmp_int);
        }
        xcase SCMD_PROCESSTIMES:
        {
            double up, kernel, user;
            timeProcessTimes(&up, &kernel, &user);
            conPrintf(client,    "%15f secs uptime\n"
                                "%15f secs kernel time\n"
                                "%15f secs user time\n",
                                up, kernel, user );
        }
        xcase SCMD_MEMORYSIZES:
        {
            size_t wsbytes, peakwsbytes, pfbytes, peakpfbytes;
            procMemorySizes(&wsbytes, &peakwsbytes, &pfbytes, &peakpfbytes);
            conPrintf(client,    "%10d bytes working set\n"
                                "%10d bytes peak working set\n"
                                "%10d bytes page file\n"
                                "%10d bytes peak page file\n",
                                wsbytes, peakwsbytes, pfbytes, peakpfbytes );
        }
        xcase SCMD_MOVE_ENTITY_TO_ME:
        {
            if(e) {
                entControl(client, "0 setpos me");
            }
        }
        xcase SCMD_ENTCONTROL:
        {
            entControl(client, server_state.entcon_msg);
        }
        xcase SCMD_TESTNPCDTA:
        {
            //for testing only
            Vec3 v;
            Entity * e;
            copyVec3(ENTPOS(clientGetViewedEntity(client)), v);
            v[0] += 20;
            e = npcCreate(server_state.entcon_msg, ENTTYPE_NPC);
            if(!e)
            {
                conPrintf(client, clientPrintf(client,"InvalidNPC", server_state.entcon_msg));
                break;
            }
            entUpdatePosInstantaneous(e,v);
        }
        xcase SCMD_TESTNPCS:
            testNpcs(client,eaSize(&npcDefList.npcDefs),tmp_int,tmp_int2);
        xcase SCMD_TESTGENTYPES:
            entgenValidateAllGeneratedTypes();
        xcase SCMD_PRINT_ENT:
        {
            printf("%s\n",server_state.entcon_msg);
        }
        xcase SCMD_SERVER_BREAK:
#ifdef _M_X64
            DebugBreak();
#else
            __asm int 3        // break!
#endif
            break;
        xcase SCMD_SMALLOC:
            memCheckDumpAllocs();
        xcase SCMD_HEAPINFO:
            heapInfoPrint();
        xcase SCMD_HEAPLOG:
            heapInfoLog();
        xcase SCMD_RETURNALL:
            MissionShutdown("SCMD_RETURNALL");
        xcase SCMD_SENDMSG:
            dbBroadcastMsg(list_id,&container_id, INFO_SVR_COM, 0, 1,group_msg);
        xcase SCMD_SHOWSTATE:
            server_state.show_entity_state = 1;
        xcase SCMD_YOUSAY:
            {
                ScriptVarsTable vars = {0};
                Entity * selectedEnt;
                int entID;

                if( client )
                    entID = client->controls.selected_ent_server_index;
                selectedEnt = validEntFromId(entID);

                if( selectedEnt )
                {
                    selectedEnt->audience = erGetRef(e);
                    selectedEnt->objective = erGetRef(e);

                    if(EncounterVarsFromEntity(selectedEnt))
                        ScriptVarsTableScopeCopy(&vars, EncounterVarsFromEntity(selectedEnt));
                    saBuildScriptVarsTable(&vars, e, SAFE_MEMBER(e, db_id), 0, NULL, NULL, NULL, NULL, NULL, 0, 0, false);

                    saUtilEntSays(selectedEnt, ENTTYPE(selectedEnt) == ENTTYPE_NPC, saUtilTableAndEntityLocalize(tmp_str, &vars, e));
                }
            }
        xcase SCMD_YOUSAYONCLICK:
            {
                Entity * selectedEnt;
                int entID;

                if( client )
                    entID = client->controls.selected_ent_server_index;
                selectedEnt = validEntFromId(entID);

                if( selectedEnt )
                {
                    selectedEnt->audience = erGetRef(e);
                    selectedEnt->objective = erGetRef(e);
                    setSayOnClick( selectedEnt, tmp_str  );
                }
            }
        xcase SCMD_MAPMOVE:
            {
                if (map_id != db_state.map_id)
                {
                    LWC_STAGE mapStage;
                    if (LWC_CheckMapIDReady(map_id, LWC_GetCurrentStage(e), &mapStage))
                    {
                        client->entity->adminFrozen = 1;
                        client->entity->db_flags |= DBFLAG_MAPMOVE;
                        dbAsyncMapMove(client->entity,map_id,NULL,XFER_STATIC);
                    }
                    else
                    {
                        sendDoorMsg(e, DOOR_MSG_FAIL, localizedPrintf(e, "LWCDataNotReadyMap", mapStage));
                    }
                }
                else
                {
                    conPrintf(admin, clientPrintf(client,"AlreadyOnMap",map_id));
                }
            }
        xcase SCMD_MAPMOVESPAWN:
            {
                char errmsg[200];
                Vec3 nowhere = {0};
                errmsg[0] = 0;
                DoorAnimEnter(e, NULL, nowhere, map_id, NULL, tmp_str, XFER_STATIC, errmsg);
                if (errmsg[0])
                    conPrintf(admin,localizedPrintf(e,"MapMoveSpawnFail",errmsg));
            }
        xcase SCMD_SHARDJUMP:
            client->entity->adminFrozen = 1;
            dbAsyncShardJump(client->entity,tmp_str);
        xcase SCMD_SHARDXFER_INIT:
        {
            OrderId order_id;
            order_id = orderIdFromString(tmp_str);
            dbShardXferInit(client->entity, order_id, tmp_int, tmp_int2, tmp_str2);
        }
        xcase SCMD_SHARDXFER_COMPLETE:
        {
            dbShardXferComplete(client->entity, tmp_int, tmp_int2, tmp_int3, tmp_int4);
        }
        xcase SCMD_SHARDXFER_JUMP:
        {
            dbShardXferJump(client->entity);
        }
        xcase SCMD_NEXTPLAYER:
            gotoNextEntity(ENTTYPE_PLAYER, client, 0);
        xcase SCMD_NEXTNPC:
            gotoNextEntity(ENTTYPE_NPC, client, 0);
        xcase SCMD_NEXTCRITTER:
            gotoNextEntity(ENTTYPE_CRITTER, client, 0);
        xcase SCMD_NEXTAWAKECRITTER:
            gotoNextEntity(ENTTYPE_CRITTER, client, 1);
        xcase SCMD_NEXTCAR:
            gotoNextEntity(ENTTYPE_CAR, client, 0);
        xcase SCMD_NEXTITEM:
            gotoNextEntity(ENTTYPE_MISSION_ITEM, client, 0);
        xcase SCMD_NEXTRESET:
            gotoNextReset(client, 0);
        xcase SCMD_GOTOENT:
            gotoEntity(entity_id, client);
        xcase SCMD_GOTOENTBYNAME:
            gotoEntityByName(tmp_str, client);
        xcase SCMD_NEXTSEQ:
            gotoNextSeq(tmp_str, client);
        xcase SCMD_NEXTNPCCLUSTER:
            gotoNextNPCCluster(client);
        xcase SCMD_CRASHNOW:
        {
            volatile int a=1,b=0;
            a = a / b;
            printf("%d",a);
        }
        xcase SCMD_FATAL_ERROR_NOW:
            FatalErrorf("Calling FatalErrorf() from " __FUNCTION__ "()." );
        xcase SCMD_SENDPACKET:
        {
            Packet *pak = pktCreateEx(client->link, SERVER_LARGE_PACKET);
            char buffer[1024];
            int i;
            for (i=0; i<tmp_int; i++) {
                pktSendBitsArray(pak, 8*1024, buffer);
            }
            pktSend(&pak, client->link);
            lnkFlushAll();
        }
        xcase SCMD_INVINCIBLE:
            csrInvincible(client,invincible_state);
        xcase SCMD_UNSTOPPABLE:
            csrUnstoppable(client,unstoppable_state);
        xcase SCMD_DONOTTRIGGERSPAWNS:
            csrDoNotTriggerSpawns(client, tmp_int);
        xcase SCMD_ALWAYSHIT:
            csrAlwaysHit(client,alwayshit_state);
        xcase SCMD_UNTARGETABLE:
            csrUntargetable(client,untargetable_state);
        xcase SCMD_RELOADPRIORITY:
            if(isProductionMode()){
                conPrintf(client, clientPrintf(client,"CantLoadPriority"));
                break;
            }
            aiPriorityLoadFiles(1);
        xcase SCMD_RELOADAICONFIG:
            if(isProductionMode()){
                conPrintf(client, clientPrintf(client,"CantLoadAI"));
                break;
            }
            aiInitLoadConfigFiles(1);
        xcase SCMD_GETENTDEBUGMENU:
            aiDebugSendEntDebugMenu(client);
        xcase SCMD_INITMAP:
            if( isDevelopmentMode() && sharedMemoryGetMode() == SMM_DISABLED) {
                server_error_sent_count = 0;
                if (OnMissionMap())
                {
                    serverParseClient("initmission", client);
                }
                else
                {
                    initMap(1);                
                    MissionDebugCheckAllMapStats(client, client->entity);
                }
            } else {
                conPrintf(client, "When shared memory is enabled, /initmap is not supported");
            }
        xcase SCMD_SERVERWHO:
            csrServerWho(client,status_server_id);
        xcase SCMD_WHO:
            {
                if( e->access_level )
                    csrWho(client,player_name);
                else
                {
                    char buf[1000];
                    sprintf( buf, "sea %s", player_name );
                    serverParseClient( buf, client );
                }
            }
        xcase SCMD_WHOALL:
            csrWhoAll(client);
        xcase SCMD_SUPERGROUPWHO:
            csrSupergroupWho(client,group_name);
        xcase SCMD_TEAMUPWHO:
         case SCMD_RAIDWHO:
         case SCMD_LEVELINGPACTWHO:
            csrGroupWho(client, cmd->name, tmp_int);
        xcase SCMD_TMSG:
            chatSendToTeamup( client->entity,tmp_str, 0);
        xcase SCMD_SILENCEALL:
        {
            char    buf[100];

            sprintf(buf,"silenceall %d", (tmp_int ? 1 : 0));
            sendToAllServers(buf);

            if(tmp_int)
                shardCommSend(e, true, "CsrSilenceAll");
            else
                shardCommSend(e, true, "CsrUnsilenceAll");
        }
        xcase SCMD_SILENCE: /* fall through */
        case SCMD_BANCHAT:
            {
                if(e)
                {
                    int min = (cmd->num == SCMD_BANCHAT ? (tmp_int * 60 * 24) : tmp_int);
                    char buf[2000];
                    sprintf(buf, "banchat_relay \"%s\" %d %d %d", player_name, min, getAuthId(e), e->db_id);
                    serverParseClient(buf, 0);
                }
            }
        xcase SCMD_BANCHAT_RELAY:
            csrBanChat(player_name,str,tmp_int,tmp_int2, tmp_int3);
        xcase SCMD_SILENTKICK:
        {
            int result;
            Entity *ent;

            if(stricmp(player_name, "self") == 0)
                ent = e;
            else
                ent = findEntOrRelay(admin,player_name,str, &result );

            if (ent)
            {
                ClientLink* kickclient = clientFromEnt(ent);
                if(kickclient->link) 
                {
                    LOG_ENT( ent, LOG_ENTITY, LOG_LEVEL_VERBOSE, 0, "netRemoveLink, SilentKick");
                    netRemoveLink(kickclient->link);
                }
            }
        }
        xcase SCMD_KICK:
            csrKick(admin,player_name,str,tmp_str,0);
        xcase SCMD_BAN:
            csrBan(admin,player_name,str,tmp_str);
        xcase SCMD_UNBAN:
            csrUnban(admin,player_name,str);
        xcase SCMD_INVISIBLE:
            SET_ENTHIDE(client->entity) = tmp_int;
            client->entity->draw_update = 1;
            conPrintf(client,clientPrintf(client,"YouAreNow",ENTHIDE(client->entity) ? "invisiblestring" : "visiblestring"));
        xcase SCMD_TELEPORT:
        {
            char        buf[1000];
            const F32    *pos;

            pos = ENTPOS(client->entity);
            sprintf(buf,"mapmovepos \"%s\" %d %f %f %f 0 0",player_name,db_state.map_id,pos[0],pos[1],pos[2]);
            serverParseClient(buf,client);
        }
        xcase SCMD_NAMED_TELEPORT:
        {
            if (e && e->pchar && tmp_str && strlen(tmp_str))
            {
                character_NamedTeleport(e->pchar, 0, tmp_str);
            }
        }
        xcase SCMD_REWARD_BONUS_TIME:
        {
            // This code assumes the timer is countdown
            Entity* player;
            int db_id, task_id, tasksub_id, bonusTime, online;

            db_id = tmp_int;
            task_id = tmp_int2;
            tasksub_id = tmp_int3;
            bonusTime = tmp_int4;

            if (relayCommandStart(admin, db_id, str, &player, &online))
            {
                StoryTaskInfo* task = TaskGetInfo(player, tempStoryTaskHandle(task_id, tasksub_id,0, TaskForceIsOnArchitect(player)));
                if (task && task->timeout)
                {
                    task->timeout += bonusTime;
                    task->timezero += bonusTime;
                    if (TaskIsTeamTask(task, player) && player->teamup->activetask->timeout && teamLock(player, CONTAINER_TEAMUPS))
                    {
                        player->teamup->activetask->timeout += bonusTime;
                        player->teamup->activetask->timezero += bonusTime;
                        teamUpdateUnlock(player, CONTAINER_TEAMUPS);
                    }
                }
                relayCommandEnd(&player, &online);
            }
        }

        xcase SCMD_MISSION_DEBUG_ADDTIME:
        {
            int timeAdded = tmp_int;
            StoryRewardBonusTime(timeAdded);
        }

        xcase SCMD_TASK_STARTTIMER:
        {
            Entity* player;
            int db_id, task_id, tasksub_id, taskTimeout, online;
            U32 timeLimit;

            db_id = tmp_int;
            task_id = tmp_int2;
            tasksub_id = tmp_int3;
            taskTimeout = tmp_int4;
            timeLimit = dbSecondsSince2000() + taskTimeout;

            if (g_activemission)
            {
                g_activemission->failOnTimeout = true;
                g_activemission->timerType = -1;
                g_activemission->timeout = timeLimit;
                g_activemission->timezero = timeLimit;
            }

            if (relayCommandStart(admin, db_id, str, &player, &online))
            {
                StoryTaskInfo* task = TaskGetInfo(player, tempStoryTaskHandle(task_id, tasksub_id,0, TaskForceIsOnArchitect(player)));
                if (task && !task->timeout)
                {
                    task->failOnTimeout = true;
                    task->timerType = -1;
                    task->timeout = timeLimit;
                    task->timezero = timeLimit;
                    if (TaskIsTeamTask(task, player) && !player->teamup->activetask->timeout && teamLock(player, CONTAINER_TEAMUPS))
                    {
                        player->teamup->activetask->failOnTimeout = true;
                        player->teamup->activetask->timerType = -1;
                        player->teamup->activetask->timeout = timeLimit;
                        player->teamup->activetask->timezero = timeLimit;
                        teamUpdateUnlock(player, CONTAINER_TEAMUPS);
                    }
                }
                relayCommandEnd(&player, &online);
            } 

            MissionRefreshAllTeamupStatus();
        }

        xcase SCMD_TASK_STARTTIMER_NOFAIL:
        {
            Entity* player;
            int db_id, task_id, tasksub_id, timerLimit, online;
            U32 timeout, timezero;
            int timerType = 1;

            db_id = tmp_int;
            task_id = tmp_int2;
            tasksub_id = tmp_int3;
            timerLimit = tmp_int4;
            
            // Unencoding hack to get a fifth variable in
            if (timerLimit < 0)
            {
                timerLimit = -timerLimit;
                timerType = -1;
            }

            timeout = dbSecondsSince2000() + timerLimit;

            if (timerType == 1 && timerLimit > 0)
            {
                timezero = dbSecondsSince2000();
            }
            else
            {
                timezero = timeout;
            }

            if (g_activemission)
            {
                g_activemission->failOnTimeout = false;
                g_activemission->timerType = timerType;
                g_activemission->timeout = timeout;
                g_activemission->timezero = timezero;
            }

            if (relayCommandStart(admin, db_id, str, &player, &online))
            {
                StoryTaskInfo* task = TaskGetInfo(player, tempStoryTaskHandle(task_id, tasksub_id,0, TaskForceIsOnArchitect(player)));
                if (task && !task->timeout)
                {
                    task->failOnTimeout = false;
                    task->timerType = timerType;
                    task->timeout = timeout;
                    task->timezero = timezero;
                    if (TaskIsTeamTask(task, player) && !player->teamup->activetask->timeout && teamLock(player, CONTAINER_TEAMUPS))
                    {
                        player->teamup->activetask->failOnTimeout = false;
                        player->teamup->activetask->timerType = timerType;
                        player->teamup->activetask->timeout = timeout;
                        player->teamup->activetask->timezero = timezero;
                        teamUpdateUnlock(player, CONTAINER_TEAMUPS);
                    }
                }
                relayCommandEnd(&player, &online);
            }

            MissionRefreshAllTeamupStatus();
        }

        xcase SCMD_TASK_CLEARTIMER:
        {
            Entity* player;
            int db_id, task_id, tasksub_id, online;

            db_id = tmp_int;
            task_id = tmp_int2;
            tasksub_id = tmp_int3;
            
            if (g_activemission)
            {
                g_activemission->failOnTimeout = false;
                g_activemission->timerType = 0;
                g_activemission->timeout = 0;
                g_activemission->timezero = 0;
            }

            if (relayCommandStart(admin, db_id, str, &player, &online))
            {
                StoryTaskInfo* task = TaskGetInfo(player, tempStoryTaskHandle(task_id, tasksub_id,0, TaskForceIsOnArchitect(player)));
                if (task && !task->timeout)
                {
                    task->failOnTimeout = false;
                    task->timerType = 0;
                    task->timeout = 0;
                    task->timezero = 0;
                    if (TaskIsTeamTask(task, player) && !player->teamup->activetask->timeout && teamLock(player, CONTAINER_TEAMUPS))
                    {
                        player->teamup->activetask->failOnTimeout = false;
                        player->teamup->activetask->timerType = 0;
                        player->teamup->activetask->timeout = 0;
                        player->teamup->activetask->timezero = 0;
                        teamUpdateUnlock(player, CONTAINER_TEAMUPS);
                    }
                }
                relayCommandEnd(&player, &online);
            }

            MissionRefreshAllTeamupStatus();
        }

        xcase SCMD_NEWSPAPER_TEAM_COMPLETE_INTERNAL:
        {
            Entity* player;
            int db_id, contact, contactPoints, online;

            db_id = tmp_int;
            contact = tmp_int2;
            contactPoints = tmp_int3;

            if (relayCommandStart(admin, db_id, str, &player, &online))
            {
                const ContactDef* contactdef = ContactDefinition(contact);
                StoryInfo* info = StoryArcLockInfo(player);
                StoryContactInfo* broker = ContactGetInfo(info, contact);
                if (!broker && contactdef)
                    broker = ContactGetInfo(info, contactdef->brokerFriend);
                if (broker && contactPoints)
                {
                    broker->contactPoints += contactPoints;
                    ContactMarkDirty(player, info, broker);
                }
                StoryArcUnlockInfo(player);
                relayCommandEnd(&player, &online);
            }
        }

        xcase SCMD_TASK_TEAM_COMPLETE_INTERNAL:
        {
            Entity *e;
            int        db_id,leveladjust,online,level,seed;
            StoryTaskHandle sahandle;

            sscanf(tmp_str, "%i:%Ii:%i:%i:%hhi:%i:%i:%i", &db_id, SAHANDLE_SCANARGSPOS(sahandle), &leveladjust, &level, &seed);
            if (relayCommandStart(admin, db_id, str, &e, &online))
            {
                StoryInfo* info = StoryArcLockInfo(e);
                StoryTaskInfo* task = StoryArcGetAssignedTask(info, &sahandle);
                int giveSecondary = 0;
                const StoryTask* taskdef;


                // Get the correct taskdef
                if (task)
                    taskdef = task->def;
                else
                    taskdef = TaskDefinition(&sahandle);

                // If demand loaded, just flag the task as team complete-able, otherwise prompt them
                if (task && TASK_INPROGRESS(task->state))
                {
                    if (e->demand_loaded)
                        task->teamCompleted = level;
                    else
                        giveSecondary = TaskPromptTeamComplete(e, info, task, leveladjust, level);
                }
                else // if they dont have the task, or they have it completed, just reward them
                {
                    giveSecondary = 1;
                }

                // Give secondary reward if appropriate
                if (giveSecondary)
                {
                    const StoryReward* reward = NULL;
                    if (taskdef && eaSize(&taskdef->taskSuccess))
                        reward = taskdef->taskSuccess[0];
                    if (reward && eaSize(&reward->secondaryReward))
                    {
                        ScriptVarsTable vars = {0};
                        if(task)
                            saBuildScriptVarsTable(&vars, e, e->db_id, TaskGetContactHandle(info, task), task, NULL, NULL, NULL, NULL, 0, 0, false);
                        else
                            saBuildScriptVarsTable(&vars, e, e->db_id, 0, NULL, NULL, &sahandle, NULL, NULL, 0, seed, false);
                        MissionGrantSecondaryReward(db_id, reward->secondaryReward, getVillainGroupFromTask(taskdef), level, leveladjust, &vars);
                    }

                    if (TaskForceIsOnFlashback(e))
                    {
                        if (taskdef && taskdef->taskSuccessFlashback)
                            reward = taskdef->taskSuccessFlashback[0];
                        if (reward && eaSize(&reward->secondaryReward))
                        {
                            ScriptVarsTable vars = {0};
                            if(task)
                                saBuildScriptVarsTable(&vars, e, e->db_id, TaskGetContactHandle(info, task), task, NULL, NULL, NULL, NULL, 0, 0, false);
                            else
                                saBuildScriptVarsTable(&vars, e, e->db_id, 0, NULL, NULL, &sahandle, NULL, NULL, 0, seed, false);
                            MissionGrantSecondaryReward(db_id, reward->secondaryReward, getVillainGroupFromTask(taskdef), level, leveladjust, &vars);
                        }
                    }
                    else
                    {
                        if (taskdef && taskdef->taskSuccessNoFlashback)
                            reward = taskdef->taskSuccessNoFlashback[0];
                        if (reward && eaSize(&reward->secondaryReward))
                        {
                            ScriptVarsTable vars = {0};
                            if(task)
                                saBuildScriptVarsTable(&vars, e, e->db_id, TaskGetContactHandle(info, task), task, NULL, NULL, NULL, NULL, 0, 0, false);
                            else
                                saBuildScriptVarsTable(&vars, e, e->db_id, 0, NULL, NULL, &sahandle, NULL, NULL, 0, seed, false);
                            MissionGrantSecondaryReward(db_id, reward->secondaryReward, getVillainGroupFromTask(taskdef), level, leveladjust, &vars);
                        }
                    }
                }
                // If this was relayed, make sure to update their badge info
                badge_RecordMissionComplete(e, taskdef->logicalname?taskdef->logicalname:taskdef->filename, getVillainGroupFromTask(taskdef));
                StoryArcUnlockInfo(e);
                relayCommandEnd(&e, &online);
            }
        }
        xcase SCMD_TASK_COMPLETE_INTERNAL:
        {
            // Command issued when a mission completes.
            Entity *e;
            int        db_id,success,online;
            StoryTaskHandle sahandle;

            db_id = tmp_int;
            sahandle.context = tmp_int2;
            sahandle.subhandle = tmp_int3;
            sahandle.compoundPos = tmp_int4;
            sahandle.bPlayerCreated = tmp_int5;
            success = tmp_int6;

            if (relayCommandStart(admin, db_id, str, &e, &online))
            {
                const StoryTask* taskDef = TaskParentDefinition(&sahandle);
                if(taskDef && taskDef->type == TASK_COMPOUND)
                    TaskMarkComplete(e, &sahandle, success);
                else
                    TaskSetComplete(e, &sahandle, 0, success);
                relayCommandEnd(&e, &online);
            }
        }
        xcase SCMD_TASK_ADVANCE_COMPLETE:
        {
            // command issued when mission closes and owner needs to advance
            Entity* e;
            int        online, db_id = tmp_int;

            if (relayCommandStart(admin, db_id, str, &e, &online))
            {
                TaskAdvanceCompleted(e);
                if (e->teamup && e->teamup->activetask->assignedDbId == e->db_id &&
                    !TaskInProgress(e, &e->teamup->activetask->sahandle))
                {
                    TeamTaskClear(e);
                    TeamTaskBroadcastComplete(e, (e->teamup->activetask->state != TASK_FAILED));
                }
                relayCommandEnd(&e, &online);
            }
        }
        xcase SCMD_OBJECTIVE_COMPLETE_INTERNAL:
        {
            Entity *e;
            int        db_id,obj_num,success,online;
            StoryTaskHandle sa;

            db_id = tmp_int;
            sa.context = tmp_int2;
            sa.subhandle = tmp_int3;
            sa.compoundPos = tmp_int4;
            sa.bPlayerCreated = tmp_int5;
            obj_num = tmp_int6;
            success = tmp_int7;

            if (relayCommandStart(admin, db_id, str, &e, &online))
            {
                TaskReceiveObjectiveComplete(e, &sa, obj_num, success);
                relayCommandEnd(&e, &online);
            }
        }
        xcase SCMD_CLUE_ADDREMOVE_INTERNAL:
        {
            Entity *e;
            int        adding,db_id,task_id,tasksub_id,online;

            adding = tmp_int;
            db_id = tmp_int2;
            task_id = tmp_int3;
            tasksub_id = tmp_int4;

            if (relayCommandStart(admin, db_id, str, &e, &online))
            {
                ClueAddRemoveInternal(e, tempStoryTaskHandle(task_id, tasksub_id, 0, TaskForceIsOnArchitect(e) ), tmp_str, adding);
                relayCommandEnd(&e, &online);
            }
        }
        xcase SCMD_CLUE_ADDREMOVE:
            ClueDebugAddRemove(client->entity, tmp_int, tmp_str, tmp_int2);
        xcase SCMD_MISSION_REQUEST_SHUTDOWN:
        {
            Entity *e;
            int        map_id,db_id,online;
            StoryTaskHandle sa;

            map_id = tmp_int;
            db_id = tmp_int2;
            sa.context = tmp_int3;
            sa.subhandle = tmp_int4;
            sa.compoundPos = tmp_int5;
            sa.bPlayerCreated = tmp_int6;

            if (relayCommandStart(admin, db_id, str, &e, &online))
            {
                if (TaskMissionRequestShutdown(e, &sa, map_id))
                {
                    sendToServer(map_id, "cmdrelay\nmissionokshutdown");
                }
                relayCommandEnd(&e, &online);
            }
        }
        xcase SCMD_MISSION_FORCE_SHUTDOWN:
        {
            Entity *e;
            int        map_id,db_id,online;
            StoryTaskHandle sa;

            map_id = tmp_int;
            db_id = tmp_int2;
            sa.context = tmp_int3;
            sa.subhandle = tmp_int4;
            sa.compoundPos = tmp_int5;
            sa.bPlayerCreated = tmp_int6;

            if (relayCommandStart(admin, db_id, str, &e, &online))
            {
                TaskMissionForceShutdown(e, &sa);
                relayCommandEnd(&e, &online);
            }
        }
        xcase SCMD_MISSION_OK_SHUTDOWN:
        {
            if (OnMissionMap()) MissionShutdown("SCMD_MISSION_OK_SHUTDOWN && OnMissionMap()");
        }
        xcase SCMD_MISSION_CHANGE_OWNER:
        {
            StoryTaskHandle sa;
            sa.context = tmp_int2;
            sa.subhandle = tmp_int3;
            sa.compoundPos = tmp_int4;
            sa.bPlayerCreated = tmp_int5;
            MissionChangeOwner(tmp_int, &sa );
        }
        xcase SCMD_MISSION_REATTACH_INTERNAL:
        {
            Entity* e;
            int db_id;

            db_id = tmp_int;
            map_id = tmp_int2;
            e = findEntOrRelayById(admin, db_id, str, NULL);
            if (e)
            {
                MissionReattachOwner(e, map_id);
            }
        }
        xcase SCMD_MISSION_KICKPLAYER_INTERNAL:
        {
            Entity* e;
            int db_id;

            db_id = tmp_int;
            e = findEntOrRelayById(admin, db_id, str, NULL);
            if (e)
            {
                MissionKickPlayer(e);
            }
        }
        xcase SCMD_MAPMOVEPOS:
        {
            int result;
            Entity *e = findEntOrRelay(admin,player_name,str,&result);

            if (e)
            {
                LWC_STAGE mapStage;
                if (LWC_CheckMapIDReady(tmp_int, LWC_GetCurrentStage(e), &mapStage))
                {
                    commandMapMovePos(e, tmp_int, setpos, tmp_int2, tmp_int3);
                }
                else
                {
                    sendDoorMsg(e, DOOR_MSG_FAIL, localizedPrintf(e, "LWCDataNotReadyMap", mapStage));
                }
            }
        }
        xcase SCMD_MAPMOVEPOS_ANDSELECTCONTACT:
        {
            int result;
            Entity *e = findEntOrRelay(admin, player_name, str, &result);

            if (e)
            {
                LWC_STAGE mapStage;
                if (LWC_CheckMapIDReady(tmp_int, LWC_GetCurrentStage(e), &mapStage))
                {
                    commandSelectContact(e, tmp_str);
                    commandMapMovePos(e, tmp_int, setpos, tmp_int2, tmp_int3);
                }
                else
                {
                    sendDoorMsg(e, DOOR_MSG_FAIL, localizedPrintf(e, "LWCDataNotReadyMap", mapStage));
                }
            }
        }
        xcase SCMD_OFFLINE_MOVE:
        {
            int map_id = tmp_int;

            if ( dbPlayerIdFromName( player_name ) <= 0)    // this check might be duplicated in player_online() check below...
            {
                conPrintf(client, clientPrintf(client,"PlayerDoesntExist", player_name));
            }
            else if( !staticMapInfoFind(map_id) )
            {
                conPrintf(client, clientPrintf(client,"MapNotStatic", map_id));
            }
            else if( player_online(player_name) )
            {
                conPrintf(client, clientPrintf(client,"PlayerOnline", player_name));
            }
            else
            {
                char sql_command[2000];

                // move player to map id & position
                sprintf(sql_command, "UPDATE dbo.ents SET mapid = %d, staticmapid = %d, posX = %f, posY = %f, posZ = %f WHERE name = N'%s' AND active IS NULL;",
                    map_id,
                    map_id,
                    setpos[0],
                    setpos[1],
                    setpos[2],
                    escapeString(player_name));

                dbExecuteSql(sql_command);

                conPrintf(client, clientPrintf(client,"PlayerMoved", player_name, map_id));
            }
        }
        xcase SCMD_FREEZE:
        {
            int result = 0;
            Entity *e = findEntOrRelay(admin,player_name,str,&result);
            if (e)
            {
                e->adminFrozen = tmp_int;
                conPrintf(e->client, localizedPrintf(e, "YouHaveBeen", tmp_int ? "frozenstr" : "unfrozenstr"));
            }

            // send message to player who entered command
            if(    result
                && (!e || (e->client != client)))
            {
                conPrintf(client,  clientPrintf(client,"PlayerHasBeen", player_name, tmp_int ? "frozenstr" : "unfrozenstr"));
            }
        }
        xcase SCMD_SPAWNVILLAIN:
            spawnVillain(tmp_str, client, 1);
        xcase SCMD_SPAWNMANYVILLAINS:
            spawnVillain(tmp_str, client, tmp_int);
        xcase SCMD_SPAWN_MOB:
        {
            int num_spawned = spawnVillainMob(tmp_int, tmp_int2, tmp_int3, client);
            conPrintf(client,  clientPrintf(client,"spawnmob: spawned mob of %d randomvillains",num_spawned) );
        }
        xcase SCMD_SPAWNPET:
            spawnPet(tmp_str, client, 1);
        xcase SCMD_SPAWNNPC:
            spawnNPC(tmp_str, client, 1);
        xcase SCMD_SPAWN:
            spawn(tmp_str, client, 1);
        xcase SCMD_SPAWNMANY:
            spawn(tmp_str, client, tmp_int);
        xcase SCMD_SPAWNOBJECTIVE:
            SpawnObjectiveItem(client, tmp_str);
        xcase SCMD_SPAWNMISSIONNPCS:
            EncounterSpawnMissionNPCs(tmp_int);
        xcase SCMD_SPAWNDOPPELGANGER:
            {
                Mat4 mat;
                Entity * ent;
                int i;

                char * doppple_tests[]= { "Tall", "Short","Inverse","Reverse","BlackAndWhite","Ghost","Shadow", "Demon","Angel","RandomPowers" };

                for( i=0; i < ARRAY_SIZE(doppple_tests); i++ )
                {
                    copyMat4( ENTMAT(e), mat );
                    ent = villainCreateDoppelganger( doppple_tests[i], doppple_tests[i], 0, VR_BOSS, 0, 50, 0, 0, e, 0, 0 );
                    entUpdateMat4Instantaneous(ent, mat);
                    aiInit(ent, NULL);
                }
            }
            xcase SCMD_SPAWNDOPPELGANGERTEST:
            {
                Mat4 mat;
                Entity * ent;

                copyMat4( ENTMAT(e), mat );
                ent = villainCreateDoppelganger( tmp_str, tmp_str, 0, VR_BOSS, 0, 50, 0, 0, e, 0, 0 );
                entUpdateMat4Instantaneous(ent, mat);
                aiInit(ent, NULL);
            }
        xcase SCMD_RAOULCMD:
            conPrintf(client, clientPrintf(client,"RaoulCmd", client->entity));
        xcase SCMD_IGNORE:
            addUserToIgnoreList( client, tmp_str, 0, client->entity, false );
        xcase SCMD_IGNORE_SPAMMER:
            addUserToIgnoreList( client, tmp_str, 0, client->entity, true );
        xcase SCMD_IGNORE_SPAMMER_AUTH:
            addUserToIgnoreList( client, tmp_str, tmp_int, client->entity, true );
        xcase SCMD_UIGNORE:
            removeUserFromIgnoreList( client, tmp_str, client->entity );
        xcase SCMD_IGNORE_LIST:
            shardCommSendf(client->entity,true,"ignoring");
        xcase SCMD_IGNORETHRESHOLD:
            shardCommSendf(client->entity,true,"setSpamThreshold %i", tmp_int);
        xcase SCMD_IGNOREMULTIPLIER:
            shardCommSendf(client->entity,true,"setSpamMultiplier %i", tmp_int);
        xcase SCMD_IGNOREDURATION:
            shardCommSendf(client->entity,true,"setSpamDuration %i", tmp_int);
        xcase SCMD_REQUEST_PLAYER_CONTROL:
            getPlayerControl( client, 1 );
        xcase SCMD_REQUEST_PLAYER_VIEW:
            getPlayerControl( client, 0 );
        xcase SCMD_RELEASE_PLAYER_CONTROL:
            freePlayerControl( client );
        xcase SCMD_ENABLE_CONTROL_LOG:
            {
                Entity* e = validEntFromId(tmp_int);

                if(e && e->client)
                {
                    START_PACKET(pak, e, SERVER_ENABLE_CONTROL_LOG);
                    pktSendBitsPack(pak, 1, tmp_int2);
                    END_PACKET
                }
            }
        xcase SCMD_SMAPNAME:
            conPrintf(admin,clientPrintf(admin,"MapNameIs",tmp_int,dbGetMapName(tmp_int)));
        xcase SCMD_MAPLIST:
            staticMapPrint(client);
        xcase SCMD_PLAYERLOCALE:
            // If the new locale is not valid, then don't do anything.
            if(!locIDIsValidForPlayers(tmp_int)){
                conPrintf( client, "InvalidLocale" );
            }else{
                client->entity->playerLocale = tmp_int;
            }
        xcase SCMD_NEXTBADSPAWN:
            gotoNextBadSpawn(client);
        xcase SCMD_NEXTBADVOLUME:
            gotoNextBadVolume(client);
        xcase SCMD_RESCANBADVOLUME:
            rescanBadVolume();
        xcase SCMD_NEXTSPAWN:
            gotoNextSpawn(client, FINDTYPE_SPAWN);
        xcase SCMD_NEXTSPAWNPOINT:
            gotoNextSpawn(client, FINDTYPE_SPAWNPOINT);
        xcase SCMD_NEXTMISSIONSPAWN:
            gotoNextMissionSpawn(client, 0);
        xcase SCMD_NEXTUMISSIONSPAWN:
            gotoNextMissionSpawn(client, 1);
        xcase SCMD_ENCOUNTER_RESET:
            if (tmp_str[0]) 
                conPrintf(client, clientPrintf(client,"EncounterReset", tmp_str));
            else 
                conPrintf(client, clientPrintf(client,"EncounterResetAll"));
            EncounterForceReset(client, tmp_str);
        xcase SCMD_ENCOUNTER_DEBUG:
            conPrintf(client, clientPrintf(client,"EncounterDebug", tmp_int? "ON": "OFF"));
            EncounterDebugInfo(tmp_int);
        xcase SCMD_ENCOUNTER_RELOAD:
            conPrintf(client, clientPrintf(client,"SpawnReload"));
            EncounterForceReload(client->entity);
        xcase SCMD_ENCOUNTER_TEAMSIZE:
            if (tmp_int > 0) 
                conPrintf(client, clientPrintf(client,"EncounterTeamSet", tmp_int));
            else 
                conPrintf(client, clientPrintf(client,"EncounterTeamNorm"));
            EncounterForceTeamSize(tmp_int);
        xcase SCMD_ENCOUNTER_IGNORE_GROUPS:
            if (tmp_int == 0) 
                conPrintf(client, clientPrintf(client,"EncounterMission"));
            else if (tmp_int == 1) 
                conPrintf(client, clientPrintf(client,"EncounterCity"));
            else 
                conPrintf(client, clientPrintf(client,"EncounterNorm"));
            EncounterForceIgnoreGroup(tmp_int);
            EncounterForceReset(client, "");
        xcase SCMD_ENCOUNTER_SPAWN:
            conPrintf(client, clientPrintf(client,"Spawning", tmp_str));
            EncounterForceSpawn(client, tmp_str);
        xcase SCMD_ENCOUNTER_SPAWN_CLOSEST:
            conPrintf(client, clientPrintf(client,"Spawning closest encounter"));
            EncounterForceSpawnClosest(client);
        xcase SCMD_ENTITYENCOUNTER_SPAWN:
            {
                Entity* e = validEntFromId(tmp_int);

                if(e && e->encounterInfo)
                {
                    EncounterGroup* group = e->encounterInfo->parentGroup;
                    EncounterCleanup(group);
                    EncounterForceSpawnGroup(client, group);
                }
            }
        xcase SCMD_ENCOUNTER_ALWAYS_SPAWN:
            conPrintf(client, clientPrintf(client,"AlwaysSpawn", tmp_int?"onstring":"offstring"));
            EncounterForceSpawnAlways(tmp_int);
        xcase SCMD_ENCOUNTER_MEMUSAGE:
            EncounterMemUsage(client);
        xcase SCMD_ENCOUNTER_NEIGHBORHOOD:
            EncounterNeighborhoodList(client);
        xcase SCMD_ENCOUNTER_PROCESSING:
            {
                int flag = tmp_int;
                conPrintf(client, "Processing was %i", EncounterAllowProcessing(flag));
                if (flag >= 0)
                    conPrintf(client, "Setting encounter processing to %i", flag);
                else
                    conPrintf(client, "(No change)");
            }
        xcase SCMD_ENCOUNTER_STAT:
            EncounterDebugStats(client);
        xcase SCMD_ENCOUNTER_SET_TWEAK:
            EncounterSetTweakNumbers(tmp_int, tmp_int2, tmp_int3);
        xcase SCMD_ENCOUNTER_PANIC_THRESHOLD:
            EncounterSetPanicThresholds(tmp_vec[0], tmp_vec[1], tmp_int);
        xcase SCMD_ENCOUNTER_MODE:
            if (0==stricmp(tmp_str, "city"))
            {
                EncounterSetMissionMode(0);
                conPrintf(client, "Treating encounters as normal for a city zone (ignoring grouping)");
            }
            else if (0==stricmp(tmp_str, "mission"))
            {
                EncounterSetMissionMode(1);
                conPrintf(client, "Treating encounters as normal for mission map (honor grouping)");
            }
            else
            {
                conPrintf(client, "Set encountermode \"Mission\" or \"City\"");
                break;
            }
            conPrintf(client, "Reloading spawndefs and world geometry\n");
            EncounterForceReload(client->entity);
        xcase SCMD_ENCOUNTER_COVERAGE:
            DebugPrintEncounterCoverage(client);
        xcase SCMD_ENCOUNTER_MINAUTOSTART:
            EncounterSetMinAutostart(client, tmp_int);
        xcase SCMD_CRITTER_LIMITS:
            EncounterSetCritterLimits(tmp_int, tmp_int2);
        xcase SCMD_SCRIPTDEF_START:
            {
                const ScriptDef *pScriptDef = ScriptDefFromFilename(tmp_str);
                e = validEntFromId(client->controls.selected_ent_server_index);

                if (pScriptDef)
                {
                    CmdScriptDefStart(pScriptDef, e);
                } else {
                    conPrintf(client, clientPrintf(client,"ZoneScriptNotStarted"), tmp_str);
                }
            }
        xcase SCMD_SCRIPTLUA_START:
            {
                e = validEntFromId(client->controls.selected_ent_server_index);

                CmdScriptLuaStart(tmp_str, tmp_str2, NULL, e);
            }
        xcase SCMD_SCRIPTLUA_START_STRING:
            {
                e = validEntFromId(client->controls.selected_ent_server_index);

                CmdScriptLuaStart(tmp_str, NULL, tmp_str2, e);
            }
        xcase SCMD_SCRIPTLUA_EXEC:
            {
                ScriptEnvironment *preserved = g_currentScript;
                g_currentScript = ScriptFromRunId(tmp_int);
                if(g_currentScript && g_currentScript->lua_interpreter)
                    lua_exec(tmp_str, 0, 0);
                g_currentScript = preserved;
            }
        xcase SCMD_ZONESCRIPT_START:
            {
                const ScriptDef *pScriptDef = ScriptDefFromFilename(tmp_str);

                if (pScriptDef)
                {
                    ScriptBeginDef(pScriptDef, SCRIPT_ZONE, NULL, "ZoneScript start", tmp_str);
                } else {
                    conPrintf(client, clientPrintf(client,"ZoneScriptNotStarted"), tmp_str);
                }
            }
        xcase SCMD_ZONEEVENT_START:
            {
                int id = ScriptBegin(tmp_str, NULL, SCRIPT_ZONE, NULL, NULL, tmp_str);
                if (id)
                    conPrintf(client, clientPrintf(client,"ZoneEventStart", tmp_str, id));
                else
                    conPrintf(client, clientPrintf(client,"NotAZoneEvent"), tmp_str);
            }
        xcase SCMD_ZONEEVENT_STOP:
            if (ScriptEnd(ScriptRunIdFromName(tmp_str, SCRIPT_ZONE),0))
                conPrintf(client, clientPrintf(client,"ZoneEventStopped", tmp_str));
            else
                conPrintf(client, clientPrintf(client,"ZoneEventWasntRun", tmp_str));
        xcase SCMD_ZONEEVENT_SIGNAL:
            if (ScriptSendSignal(ScriptRunIdFromName(tmp_str, SCRIPT_ZONE), tmp_str2))
                conPrintf(client, clientPrintf(client,"ZoneEventSignal", tmp_str, tmp_str2));
            else
                conPrintf(client, localizedPrintf(client->entity,"ZoneEventWasntRun", tmp_str));
        xcase SCMD_SHARDEVENT_START:
            ShardEventBegin(client, tmp_str);
            badge_RecordSystemChange();
        xcase SCMD_SHARDEVENT_STOP:
            ShardEventEnd(client);
            badge_RecordSystemChange();
        xcase SCMD_SHARDEVENT_SIGNAL:
            ShardEventSignal(client, tmp_str);
            badge_RecordSystemChange();
        xcase SCMD_SCRIPTLOCATION_PRINT:
            ScriptLocationDebugPrint(client);
        xcase SCMD_SCRIPTLOCATION_START:
            ScriptLocationStart(client, tmp_str);
        xcase SCMD_SCRIPTLOCATION_STOP:
            ScriptLocationStop(client, tmp_str);
        xcase SCMD_SCRIPTLOCATION_SIGNAL:
            ScriptLocationSignal(client, tmp_str, tmp_str2);
        xcase SCMD_SCRIPT_DEBUGSET:
            ScriptDebugSetServer(client, tmp_int, tmp_int2);
        xcase SCMD_SCRIPT_SHOWVARS:
            ScriptDebugShowVars(client, tmp_int);
        xcase SCMD_SCRIPT_SETVAR:
            ScriptDebugSetVar(client, tmp_int, tmp_str, tmp_str2);
        xcase SCMD_SCRIPT_PAUSE:
            if (!ScriptPause(tmp_int, tmp_int2))
                conPrintf(client, "Invalid script id %i", tmp_int);
        xcase SCMD_SCRIPT_RESET:
            if (!ScriptReset(tmp_int))
                conPrintf(client, "Invalid script id %i", tmp_int);
        xcase SCMD_SCRIPT_STOP:
            if (!ScriptEnd(tmp_int,0))
                conPrintf(client, "Invalid script id %i", tmp_int);
        xcase SCMD_SCRIPT_SIGNAL:
            if (!ScriptSendSignal(tmp_int, tmp_str))
                conPrintf(client, "Invalid script id %i", tmp_int);
        xcase SCMD_SCRIPT_COMBAT_LEVEL:
            SetScriptCombatLevel(tmp_int);
        xcase SCMD_GROUPFIND_TEST:
            printf("--------------------- GroupFind test begin -------------------\n");
            groupProcessDef(&group_info, GroupFindTrayTest);
            printf("--------------------- GroupFind test end -------------------\n");
        xcase SCMD_DESTROY:
            groupDynFind("grp3",zerovec3,-1);
            groupDynSetHpAndRepair(tmp_int,tmp_int2,tmp_int3);
        xcase SCMD_OMNIPOTENT:
            conPrintf(client, clientPrintf(client,"OmnipotentString"));
            character_GrantAllPowers(e->pchar);
            e->pl->doNotKick = 1;
        xcase SCMD_DEFS_RELOAD:
            if( isDevelopmentMode() && sharedMemoryGetMode() == SMM_DISABLED) {
                conPrintf(client, "Reloading classes, origins, and powers...\n");
                load_AllDefsReload();
            } else {
                conPrintf(client, "When shared memory is enabled, /defsreload is not supported!");
            }
        xcase SCMD_INSPIRE:
            {
                const BasePowerSet *pset = powerdict_GetBasePowerSetByName(&g_PowerDictionary, "Inspirations", tmp_str);
                if(pset!=NULL)
                {
                    const BasePower *ppow = baseset_GetBasePowerByName(pset, tmp_str2);
                    if(ppow!=NULL)
                    {
                        conPrintf(client, clientPrintf(client,"InspGrant", pset->pchName, ppow->pchName));
                        character_AddInspiration(e->pchar, ppow, "scmd");
                    }
                    else
                    {
                        conPrintf(client, clientPrintf(client,"InspPowInvalid", tmp_str2));
                    }
                }
                else
                {
                    conPrintf(client, clientPrintf(client,"InspSetInvalid", tmp_str));
                }
            }

        xcase SCMD_INSPIREX:
            {
                const PowerCategory *pcat = powerdict_GetCategoryByName(&g_PowerDictionary, "Inspirations");
                int iset, ipow;

                if(pcat!=NULL)
                {
                    iset = rand() % eaSize(&pcat->ppPowerSets);
                    ipow = rand() % eaSize(&pcat->ppPowerSets[iset]->ppPowers);

                    conPrintf(client, "Granting random inspiration %s.%s\n", pcat->ppPowerSets[iset]->pchName, pcat->ppPowerSets[iset]->ppPowers[ipow]->pchName);
                    character_AddInspiration(e->pchar, pcat->ppPowerSets[iset]->ppPowers[ipow], "scmdx");
                }
            }
        xcase SCMD_INSPIRE_ARENA:
            {
                ArenaGiveStdInspirations(e);
            }
        xcase SCMD_INSPMERGE:
            character_MergeInspiration( e, tmp_str, tmp_str2 );
        xcase SCMD_BOOST:
            {
                const PowerCategory *pcat = powerdict_GetCategoryByName(&g_PowerDictionary, "Boosts");
                int level = tmp_int;
                if(pcat!=NULL)
                {
                    const BasePowerSet *pset = category_GetBaseSetByName(pcat, tmp_str);
                    if(pset!=NULL)
                    {
                        const BasePower *ppow = baseset_GetBasePowerByName(pset, tmp_str2);
                        if(ppow!=NULL)
                        {
                            conPrintf(client, clientPrintf(client,"BoostGrant", pset->pchName, ppow->pchName, level));
                            character_AddBoost(e->pchar, ppow, level-1, 0, "scmd");
                        }
                        else
                        {
                            conPrintf(client, clientPrintf(client,"BoostPowInvalid", tmp_str2));
                        }
                    }
                    else
                    {
                        conPrintf(client, clientPrintf(client,"BoostSetInvalid", tmp_str));
                    }
                }
            }
        xcase SCMD_BOOSTSET:
            {
                int i;
                const BoostSet *pSet = boostset_FindByName(tmp_str);
                int level = tmp_int;
                if (pSet)
                {
                    const PowerCategory *pcat = powerdict_GetCategoryByName(&g_PowerDictionary, "Boosts");
                    if(pcat!=NULL)
                    {
                        for (i = 0; i < eaSize(&pSet->ppBoostLists); i++)
                        {                        
                            const BasePowerSet *pset = category_GetBaseSetByName(pcat, pSet->ppBoostLists[i]->ppBoosts[0]->pchName);
                            if(pset!=NULL)
                            {
                                const BasePower *ppow = baseset_GetBasePowerByName(pset, pSet->ppBoostLists[i]->ppBoosts[0]->pchName);
                                if(ppow!=NULL)
                                {
                                    conPrintf(client, clientPrintf(client,"BoostGrant", pset->pchName, ppow->pchName, level));
                                    character_AddBoost(e->pchar, ppow, level-1, 0, "scmdset");
                                }
                                else
                                {
                                    conPrintf(client, clientPrintf(client,"BoostPowInvalid", tmp_str2));
                                }
                            }
                            else
                            {
                                conPrintf(client, clientPrintf(client,"BoostSetInvalid", tmp_str));
                            }
                        }
                    }                    
                } else {
                    conPrintf(client, clientPrintf(client,"BoostSetInvalid", tmp_str));
                }
            }
        xcase SCMD_BOOSTX:
            {
                const PowerCategory *pcat = powerdict_GetCategoryByName(&g_PowerDictionary, "Boosts");
                int iset, ipow;
                if(pcat!=NULL)
                {
                    int level = client->entity->pchar->iLevel+1;
                    iset = rand() % eaSize(&pcat->ppPowerSets);
                    ipow = rand() % eaSize(&pcat->ppPowerSets[iset]->ppPowers);
                    conPrintf(client, "Granting random enhancement %s.%s.%i #%i\n", pcat->ppPowerSets[iset]->pchName, pcat->ppPowerSets[iset]->ppPowers[ipow]->pchName,level, tmp_int);
                    character_AddBoost(client->entity->pchar, pcat->ppPowerSets[iset]->ppPowers[ipow], level, MINMAX(tmp_int,0,3), "scmdx");
                }
            }

        xcase SCMD_BOOSTS_SETLEVEL:
            {
                character_BoostsSetLevel(client->entity->pchar, tmp_int-1);
            }

        xcase SCMD_CONVERT_BOOST:
            {
                int boostIdx, sendSuccess;
                const char * oldName = NULL;
                int oldNameCRC;
                if (!e || !e->pchar || tmp_int >= CHAR_BOOST_MAX)
                    return;

                if (boostSet_getBoostsetIfConversionParametersVerified(e->pchar, tmp_int, tmp_str))
                {
                    //save the old name
                    oldNameCRC = e->pchar->aBoosts[tmp_int]->ppowBase->crcFullName;
                    oldName = e->pchar->aBoosts[tmp_int]->ppowBase->pchDisplayName;
                }
                else
                {
                    //no name, this will error in boostset_Convert
                    oldNameCRC = 0;
                }
                boostIdx = boostset_Convert(e, tmp_int, tmp_str);

                START_PACKET(pak, e, SERVER_CONVERT_ENHANCEMENT_RESULT);
                //send the old name first
                pktSendBitsAuto(pak, oldNameCRC);
                if (boostIdx != -1)
                {
                    pktSendBitsAuto(pak, e->pchar->aBoosts[boostIdx]->ppowBase->crcFullName);
                    sendSuccess = 1;
                }
                else
                {
                    pktSendBitsAuto(pak, 0);
                    sendSuccess = 0;
                }
                END_PACKET
                if (sendSuccess)
                {
                    const char * rarity = e->pchar->aBoosts[boostIdx]->ppowBase->pBoostSetMember->ppchConversionGroups[0];
                    char * convertMsg;
                    conPrintf(client, "%s successfully converted to %s.", textStd(oldName), textStd(e->pchar->aBoosts[boostIdx]->ppowBase->pchDisplayName));
                    estrCreate(&convertMsg);
                    estrPrintf(&convertMsg, "Float%sEnhancementConverted", rarity);
                    serveFloater(e, e, convertMsg, 0.0f, kFloaterStyle_Attention, 0xffff00ff);
                    estrDestroy(&convertMsg);
                    
                    LOG_ENT( e, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0,
                        "[Boost:Conversion] %s (Level %d) converted to %s (Level %d)", 
                        textStd(oldName),
                        e->pchar->aBoosts[tmp_int]->iLevel + 1,
                        textStd(e->pchar->aBoosts[boostIdx]->ppowBase->pchDisplayName),
                        e->pchar->aBoosts[boostIdx]->iLevel + 1);
                }
                else
                {
                    conPrintf(client, "Enhancement Conversion failed.\n");
                }

            }
        xcase SCMD_TEMP_POWER:
            {
                const PowerCategory *pcat = powerdict_GetCategoryByName(&g_PowerDictionary, "Temporary_Powers");
                if(pcat!=NULL)
                {
                    const BasePowerSet *pset = category_GetBaseSetByName(pcat, tmp_str);
                    if(pset!=NULL)
                    {
                        const BasePower *ppowBase = baseset_GetBasePowerByName(pset, tmp_str2);
                        if(ppowBase!=NULL)
                        {
                            conPrintf(client, clientPrintf(client,"TempPowGrant", pset->pchName, ppowBase->pchName));

                            if(character_AddRewardPower(e->pchar, ppowBase))
                                conPrintf(client, "Granting temp power %s\n", dbg_BasePowerStr(ppowBase));
                            else
                                conPrintf(client, "Failed to grant temp power %s\n", dbg_BasePowerStr(ppowBase));
                        }
                        else
                        {
                            conPrintf(client, clientPrintf(client,"TempPowPowInvalid", tmp_str2));
                        }
                    }
                    else
                    {
                        conPrintf(client, clientPrintf(client,"TempPowSetInvalid", tmp_str));
                    }
                }
            }

        xcase SCMD_TEMP_POWERX:
            {
                const PowerCategory *pcat = powerdict_GetCategoryByName(&g_PowerDictionary, "Temporary_Powers");
                int iset, ipow;

                if(pcat!=NULL)
                {
                    iset = rand() % eaSize(&pcat->ppPowerSets);
                    ipow = rand() % eaSize(&pcat->ppPowerSets[iset]->ppPowers);

                    if(character_AddRewardPower(e->pchar, pcat->ppPowerSets[iset]->ppPowers[ipow]))
                        conPrintf(client, "Granting temp power %s\n", dbg_BasePowerStr(pcat->ppPowerSets[iset]->ppPowers[ipow]));
                    else
                        conPrintf(client, "Failed to grant temp power %s\n", dbg_BasePowerStr(pcat->ppPowerSets[iset]->ppPowers[ipow]));
                }
            }

        xcase SCMD_TEMP_POWER_REVOKE:
            {
                const PowerCategory *pcat = powerdict_GetCategoryByName(&g_PowerDictionary, "Temporary_Powers");
                if(pcat!=NULL)
                {
                    const BasePowerSet *pset = category_GetBaseSetByName(pcat, tmp_str);
                    if(pset!=NULL)
                    {
                        const BasePower *ppowBase = baseset_GetBasePowerByName(pset, tmp_str2);
                        if(ppowBase!=NULL)
                        {
                            conPrintf(client, clientPrintf(client,"TempPowRevoke", pset->pchName, ppowBase->pchName));

                            character_RemoveBasePower(e->pchar, ppowBase);
                        }
                        else
                        {
                            conPrintf(client, clientPrintf(client,"TempPowPowInvalid", tmp_str2));
                        }
                    }
                    else
                    {
                        conPrintf(client, clientPrintf(client,"TempPowSetInvalid", tmp_str));
                    }
                }
            }

        xcase SCMD_PVP_SWITCH:
            csrPvPSwitch(client,tmp_int);

        xcase SCMD_PVP_ACTIVE:
            csrPvPActive(client,tmp_int);

        xcase SCMD_PVP_LOGGING:
            {
                if ( server_state.enablePvPLogging == tmp_int)
                {
                    conPrintf(client, clientPrintf(client,"PvP Logging already in this state (%d)", tmp_int));                    
                } else {
                    if (server_state.enablePvPLogging)
                    {
                        // close and send log to client
                        pvp_LogStop(client->entity);
                    } else {
                        // save client link
                        pvp_LogStart(client->entity);
                    }
                    server_state.enablePvPLogging = tmp_int;
                }
            }

        xcase SCMD_COMBATSTATSRESET:
        {
            time_t ttime;
            int i;

            for(i=1;i<entities_max;i++)
            {
                if(entity_state[i] & ENTITY_IN_USE)
                {
                    Entity *e = entities[i];
                    if(e && e->pchar && character_IsInitialized(e->pchar))
                    {
                        character_ResetStats(e->pchar);
                    }
                }
            }

            if (isDevelopmentMode() && sharedMemoryGetMode() == SMM_DISABLED /*!isSharedMemory(&g_PowerDictionary.ppPowers)*/)
                powerdict_ResetStats(cpp_const_cast(PowerDictionary*)(&g_PowerDictionary));
            else
                conPrintf(client, "When shared memory is enabled, /combatstatsreset is not supported");

            estrClear(&g_estrDestroyedCritters);
            estrClear(&g_estrDestroyedPlayers);
            estrClear(&g_estrDestroyedPets);

            time(&ttime);
            LOG( LOG_POWERS, LOG_LEVEL_VERBOSE, 0, "---- Combat Stats Reset at %.24s\n", ctime(&ttime));
            g_uTimeCombatStatsStarted = timerSecondsSince2000();
        }

        xcase SCMD_COMBATSTATSDUMP:
        case SCMD_COMBATSTATSDUMP_PLAYER:
        {
            int i;
            time_t ttime;
            char *estr;

            time(&ttime);

            LOG( LOG_POWERS, LOG_LEVEL_IMPORTANT, 0, "---- Combat Stats Dump at %.24s\n", ctime(&ttime));
            LOG( LOG_POWERS, LOG_LEVEL_IMPORTANT, 0, "Character\tClass\tCategory\tPower Set\tPower\tDamage Given\tDPS\tHealing Given\tHPS\tPower Attempts\tHits\tMisses\tHit %%\tMiss %%\tDamage Received\tHealing Received\tBeen Hit\tBeen Missed\tKnockbacks\tKills\n");

            estrCreate(&estr);
            for(i=1;i<entities_max;i++)
            {
                if(entity_state[i] & ENTITY_IN_USE)
                {
                    Entity *e = entities[i];
                    if(e && e->pchar && character_IsInitialized(e->pchar)
                        && (cmd->num==SCMD_COMBATSTATSDUMP || ENTTYPE(e)==ENTTYPE_PLAYER) || erGetDbId(e->erOwner)!=0)
                    {
                        estrClear(&estr);
                        character_DumpStats(e->pchar, &estr);
                        log_printPowersMultiLine(estr);
                    }
                }
            }
            estrDestroy(&estr);

            {
                if(g_estrDestroyedPlayers && *g_estrDestroyedPlayers)
                {
                    LOG( LOG_POWERS, LOG_LEVEL_IMPORTANT, 0, "------ Destroyed Players\n");
                    log_printPowersMultiLine( g_estrDestroyedPlayers);
                }
                if(g_estrDestroyedPets && *g_estrDestroyedPets)
                {
                    LOG( LOG_POWERS, LOG_LEVEL_IMPORTANT, 0, "------ Destroyed Pets\n");
                    log_printPowersMultiLine(g_estrDestroyedPets);
                }
                if(cmd->num==SCMD_COMBATSTATSDUMP && g_estrDestroyedCritters && *g_estrDestroyedCritters)
                {
                    LOG( LOG_POWERS, LOG_LEVEL_IMPORTANT, 0, "------ Destroyed Critters\n");
                    log_printPowersMultiLine(g_estrDestroyedCritters);
                }
            }

        }
        xcase SCMD_COMBATSTATSDUMP_AGGREGATE:
        {
            time_t ttime;
            time(&ttime);
            LOG( LOG_POWERS, LOG_LEVEL_IMPORTANT, 0, "---- Aggregate Combat Stats Dump at %.24s\n", ctime(&ttime));
            LOG( LOG_POWERS, LOG_LEVEL_IMPORTANT, 0, "Character\tClass\tCategory\tPower Set\tPower\tDamage Given\tDPS\tHealing Given\tHPS\tPower Attempts\tHits\tMisses\tHit %%\tMiss %%\n");
            powerdict_DumpStats(&g_PowerDictionary);
        }

        xcase SCMD_TEAMCREATE:
            if (tmp_int != CONTAINER_TEAMUPS && tmp_int != CONTAINER_SUPERGROUPS && tmp_int != CONTAINER_TASKFORCES)
                conPrintf(client, "Invalid db_list id specified");
            else
                teamCreate(client->entity,tmp_int);
        xcase SCMD_TEAMLEAVE:
            if (tmp_int != CONTAINER_TEAMUPS && tmp_int != CONTAINER_SUPERGROUPS && tmp_int != CONTAINER_TASKFORCES)
                conPrintf(client, "Invalid db_list id specified");
            else
                teamDelMember(client->entity,tmp_int,0,0);
        xcase SCMD_TEAMJOIN:
        xcase SCMD_MEMMONITOR:
            memMonitorDisplayStats();
        xcase SCMD_MEM_CHECK:
            if (heapValidateAll())
                conPrintf(client, "Memory ok\n");
            else
                conPrintf(client, "Memory error\n");
        xcase SCMD_STRINGTABLE_MEM_DUMP:
            printStringTableMemUsage();
        xcase SCMD_STASHTABLE_MEM_DUMP:
            printStashTableMemDump();
        xcase SCMD_STORYARC_PRINT:
            StoryArcDebugPrint(client, e, 0);
        xcase SCMD_STORYARC_DETAILS:
            StoryArcDebugPrint(client, e, 1);
        xcase SCMD_STORYARC_RELOAD:
            StoryArcReloadAll(e);
        xcase SCMD_STORYARC_COMPLETE_DIALOG_DEBUG:
            TaskForceCompleteStatus(e);
//            serveDesaturateInformation(e, 10.0f, 0.8f, 0.0f, 2.0f);
        xcase SCMD_FIXUP_STORYARCS:
            fixupStoryArcs(e);
        xcase SCMD_CONTACT_GETTASK:
            TaskForceGet(client, tmp_str, tmp_str2);
        xcase SCMD_GETTASK:
            TaskForceGetUnknownContact(client, tmp_str);
        xcase SCMD_CONTACT_GETSTORYARC:
            StoryArcForceGet(client, tmp_str, tmp_str2);
        xcase SCMD_GETSTORYARC:
            StoryArcForceGetUnknownContact(client, tmp_str);
        xcase SCMD_GETSTORYARCTASK:
            StoryArcForceGetTask(client, tmp_str, tmp_str2);
        xcase SCMD_CONTACT_ACCEPTTASK:
            ContactDebugAcceptTask(client, tmp_str, tmp_int);
        xcase SCMD_STARTMISSION:
            StartMission(client, tmp_str, tmp_str2);
        xcase SCMD_STARTSTORYARCMISSION:
            StartStoryArcMission(client, tmp_str, tmp_str2, tmp_str3);
        xcase SCMD_MISSION_USEREXIT:
            if(e && (MissionPlayerAllowedTeleportExit() || OnArenaMap() ))
            {
                DoorEntry *door; 
                if (g_ArchitectTaskForce && entMode(e, ENT_DEAD)) // if the ask to leave an arcitect mission while dead, send them to hospital
                {
                    dbBeginGurneyXfer(e, 0);
                }
                else if(!g_ArchitectTaskForce && MissionSuccess() && (door = dbFindAlternateMissionExit()) != NULL) // else if it's a mission with an alternate exit, and they completed successfully, send them to the target
                {
                    leaveMissionViaAlternateDoor(e, door);
                }
                else
                {
                    leaveMission(e, tmp_int, 0);
                }
            }
        xcase SCMD_REQUEST_TELEPORT:
            if(e && e->pl && e->pl->usingTeleportPower)
            {
                LWC_STAGE mapStage;
                if (tmp_int == db_state.map_id) 
                {
                    // move to specified location on this map
                    DoorEntry *door = dbFindSpawnLocation(tmp_str);

                    if (door) 
                    {
                        entUpdatePosInstantaneous(e, door->mat[3]);
                        e->move_instantly = 1;
                    }
                } else if (!LWC_CheckMapIDReady(tmp_int, LWC_GetCurrentStage(e), &mapStage)) {
                    sendDoorMsg(e, DOOR_MSG_FAIL, localizedPrintf(e, "LWCDataNotReadyMap", mapStage));
                } else {
                    e->adminFrozen = 1;
                    e->db_flags |= DBFLAG_DOOR_XFER;
                    e->pl->usingTeleportPower = false;
                    if (tmp_str)    
                        strcpy(e->spawn_target, tmp_str);
                    dbAsyncMapMove(e, tmp_int, NULL, XFER_STATIC | XFER_FINDBESTMAP);        
                }
            }
        xcase SCMD_REQUEST_MISSION_EXIT_ALT:
            if (MissionSuccess() && dbFindAlternateMissionExit() && tmp_str && tmp_str[0] && tmp_int > 0)
            {
                LWC_STAGE mapStage;
                if (LWC_CheckMapIDReady(tmp_int, LWC_GetCurrentStage(e), &mapStage))
                {
                e->adminFrozen = 1;
                e->db_flags |= DBFLAG_DOOR_XFER;
                strcpy(e->spawn_target, tmp_str);
                dbAsyncMapMove(e, tmp_int, NULL, XFER_STATIC | XFER_FINDBESTMAP);        
            }
                else
                {
                    sendDoorMsg(e, DOOR_MSG_FAIL, localizedPrintf(e, "LWCDataNotReadyMap", mapStage));
                }
            }
        xcase SCMD_REQUEST_SCRIPT_ENTER_DOOR:
            {
                enterScriptDoor(e, NULL, tmp_str, tmp_int, tmp_int2);
            }
        xcase SCMD_MISSION_RESEED:
            MissionReseed();
        case SCMD_MISSION_INIT: // Reseed falls through to init
            if (client && client->entity)
            {
                if (g_activemission)
                {
                    if( isDevelopmentMode() && sharedMemoryGetMode() == SMM_DISABLED) {
                    MissionLoad(1, db_state.map_id, db_state.map_name);
                    MissionForceUninit();
                    MissionForceReinit(client->entity);
                    MissionDebugCheckAllMapStats(client, client->entity);
                    conPrintf(client, "Reinitializing the current mission.\n");
                    } else {
                        conPrintf(client, "When shared memory is enabled, /v is not supported");
                    }
                }
                else
                {
                    conPrintf(client, "You have no mission to initialize.\n");
                }
            }
        xcase SCMD_MISSION_DEBUGEXIT:
            leaveMission(client->entity, 0, 0);
        xcase SCMD_MISSION_DEBUGKICK:
            MissionKickPlayer(client->entity);
        xcase SCMD_MISSION_DEBUGUNKICK:
            MissionUnkickPlayer(client->entity);
        xcase SCMD_BASE_DEBUGENTER:
            {
                tmp_str[0] = 0;
                SGBaseEnter(client->entity, "PlayerSpawn", tmp_str);
                if (tmp_str[0])
                    conPrintf(client, "%s", tmp_str);
            }
        xcase SCMD_BASE_ENTERBYSGID:
            {
                if( e && tmp_int > 0 )
                {
                    tmp_str[0] = 0;
                    SGBaseEnterFromSgid(client->entity, "PlayerSpawn", tmp_int, tmp_str);
                    if (tmp_str[0])
                        conPrintf(client, "%s", tmp_str);
                }
                else
                {
                    conPrintf(client, "invalid supergroup id %d", tmp_int);
                }
            }
        xcase SCMD_BASE_ENTERBYPASSCODE:
			{
				U32 md5hash[4];
				U32 i;
				int split = 0;
				int passcode;
				int sgid;
				Supergroup * sg;
				char msg[100];
				
				if (!e) break;

				for (i = 0; i < strlen(tmp_str); i++) {
					if(tmp_str[i] == '-') split = i;
					tmp_str[i] = toupper(tmp_str[i]);
				}

				if (split == 0) {
					chatSendToPlayer(e->db_id, "You have entered an invalid base access passcode.", INFO_USER_ERROR, 0);
					break;
				}

				tmp_str[split] = '\0';
				sgid = atoi(&tmp_str[split+1]);

				sg = sgrpFromSgId(sgid);
				if (!sg) {
					sprintf(msg, "Unable to load the supergroup container %i.", sgid);
					chatSendToPlayer(e->db_id, msg, INFO_USER_ERROR, 0);
				} else {
					cryptMD5Init();
					cryptMD5Update((U8*)tmp_str, strlen(tmp_str));
					cryptMD5Final(md5hash);
					passcode = md5hash[0] ^ md5hash[1] ^ md5hash[2] ^ md5hash[3];

					if (passcode == sg->passcode) {
						sprintf(msg, "Passcode accepted for %s.", sg->name);
						chatSendToPlayer(e->db_id, msg, INFO_SVR_COM, 0);
						e->pl->passcode = passcode;
						SGBaseEnterFromSgid(client->entity, "PlayerSpawn", sgid, tmp_str);
					} else {
						sprintf(msg, "You have entered an incorrect passcode for %s.", sg->name);
						chatSendToPlayer(e->db_id, msg, INFO_USER_ERROR, 0);
					}
				}
			}            
        xcase SCMD_BASE_EXPLICIT:
        {
            char errmsg[200] = "";
            DoorAnimEnter(client->entity, NULL, zerovec3, 0, tmp_str, "PlayerSpawn", XFER_BASE, NULL);
            if (errmsg[0])
                conPrintf(admin, localizedPrintf(client->entity, "MapMoveSpawnFail", errmsg));
        }
        xcase SCMD_BASE_RETURN:
        {
            tmp_str[0] = 0;
            SGBaseEnter(client->entity, "BaseReturn", tmp_str);
            if (tmp_str[0])
                conPrintf(client, "%s", tmp_str);
        }
        xcase SCMD_APARTMENT:
        {
            sprintf(tmp_str, "B:0:%d", client->entity->db_id);
            DoorAnimEnter(client->entity, NULL, zerovec3, 0, tmp_str, "PlayerSpawn", XFER_BASE, NULL);
        }
        xcase SCMD_MISSION_COMPLETE:
            MissionSetComplete(1);
        xcase SCMD_TASK_COMPLETE: // fall through -- is this kind of hackery really needed? -CW
        case SCMD_TASK_FAIL:
            TaskDebugComplete(client->entity, tmp_int, cmd->num == SCMD_TASK_COMPLETE);
        xcase SCMD_TASK_SELECT_VALIDATE:
        {
            Entity *e;
            int        db_id, online;
            StoryTaskHandle sa;

            db_id = tmp_int;
            sa.context = tmp_int2;
            sa.subhandle = tmp_int3;
            sa.compoundPos = tmp_int4;
            sa.bPlayerCreated = tmp_int5;

            if (relayCommandStart(admin, db_id, str, &e, &online))
            {
                TeamTaskValidateSelection(e, &sa);
                relayCommandEnd(&e, &online);
            }
        }
        xcase SCMD_TASK_SELECT_VALIDATE_ACK:
        {
            Entity *e;
            int        db_id, online, valid, responder;
            StoryTaskHandle sa;

            db_id = tmp_int;
            sa.context = tmp_int2;
            sa.subhandle = tmp_int3;
            sa.compoundPos = tmp_int4;
            sa.bPlayerCreated = tmp_int5;
            valid = tmp_int6;
            responder = tmp_int7;

            if (relayCommandStart(admin, db_id, str, &e, &online))
            {
                TeamTaskValidateSelectionAck(e, &sa, valid, responder);
                relayCommandEnd(&e, &online);
            }
        }
        xcase SCMD_FLASHBACK_TEAM_REQUIRES_VALIDATE:
        {
            Entity *e;
            int        db_id, online;
            StoryTaskHandle sa;

            db_id = tmp_int;
            sa.context = tmp_int2;
            sa.subhandle = tmp_int3;
            sa.compoundPos = tmp_int4;
            sa.bPlayerCreated = tmp_int5;

            if (relayCommandStart(admin, db_id, str, &e, &online))
            {
                StoryArcFlashbackTeamRequiresValidate(e, &sa);
                relayCommandEnd(&e, &online);
            }
        }
        xcase SCMD_FLASHBACK_TEAM_REQUIRES_VALIDATE_ACK:
        {
            Entity *e;
            int        db_id, online;
            StoryTaskHandle sa;

            db_id = tmp_int;
            sa.context = tmp_int2;
            sa.subhandle = tmp_int3;
            sa.compoundPos = tmp_int4;
            sa.bPlayerCreated = tmp_int5;

            if (relayCommandStart(admin, db_id, str, &e, &online))
            {
                StoryArcFlashbackTeamRequiresValidateAck(e, &sa, tmp_int6, tmp_int7);
                relayCommandEnd(&e, &online);
            }
        }
        xcase SCMD_SG_TASK_COMPLETE: // fall through
        case SCMD_SG_TASK_FAIL:
            TaskDebugComplete(client->entity, -1, cmd->num == SCMD_SG_TASK_COMPLETE);
        xcase SCMD_SHOW_CUTSCENE:
            ShowCutScene(client->entity);
        xcase SCMD_TASKFORCE_CREATE:
            TaskForceCreateDebug(client->entity, client);
        xcase SCMD_TASKFORCE_DESTROY:
            TaskForceDestroy(client->entity);
        xcase SCMD_TASKFORCE_MODE:
            TaskForceModeChange(client->entity, tmp_int);
        xcase SCMD_TASKFORCE_JOIN:
            TaskForceJoin(client->entity, tmp_str);
        xcase SCMD_REWARD:
        {
            int level = tmp_int;
            char *reward = NULL;

            strdup_alloca(reward, tmp_str);

            if(!e)
                break;
            conPrintf(client, "Awarding reward %s at level %i", reward, level);
            rewardFindDefAndApplyToEnt(e, (const char**)eaFromPointerUnsafe(reward), VG_NONE, level, true, REWARDSOURCE_CSR, NULL);
        }
        xcase SCMD_REWARD_RELOAD:
            rewardReadDefFiles();
        xcase SCMD_REWARD_INFO:
            rewardDisplayDebugInfo(tmp_int);
        xcase SCMD_REWARD_STORY:
        {
            Entity *e;
            int        db_id, villaingroup, level, levelAdjust, online;
            RewardSource source;

            db_id = tmp_int;
            villaingroup = tmp_int2;
            level = tmp_int3;
            levelAdjust = tmp_int4;
            source = tmp_int5;

            if (relayCommandStart(admin, db_id, str, &e, &online))
            {
                int rewardLevel;
                char * reward;
                char **rewardTables = NULL;
        
                // set up and storyarc info
                rewardVarClear();
                if (stricmp(tmp_str2, "none") != 0)
                {
                    rewardVarSet("StoryArcName", tmp_str2);
                }

                // Parse the reward up using the semicolon as the delimiter
                reward = strtok(tmp_str, ":");
                while(reward)
                {
                    eaPush(&rewardTables, reward);
                    reward = strtok(NULL, ":");
                }

                // calc level as player if not set due to sgrp badge reward forwarding
                if (level == REWARDLEVELPARAM_USEPLAYERLEVEL)
                    level = character_CalcEffectiveCombatLevel(e->pchar)+1;

                // Design decision to cap all story rewards at the players level to prevent power leveling exploits
                rewardLevel = rewardStoryAdjustLevel(e, level, levelAdjust);

                // Now apply the reward normally
                rewardFindDefAndApplyToEnt(e, rewardTables, villaingroup, rewardLevel, true, source, NULL);

                // Cleanup
                eaDestroy(&rewardTables);
                rewardVarClear();

                relayCommandEnd(&e, &online);
            }
        }
        xcase SCMD_REWARD_DEBUG_TARGET:
        {
            Entity *target;

            if(!e || !e->pchar )
                break;

            target = erGetEnt(e->pchar->erTargetInstantaneous);
            if(!target)
                break;
            rewardDebugTarget( e, target, tmp_int );
        }

        xcase SCMD_REWARDDEF_APPLY:
        {
            Entity *e;
            int        db_id, villaingroup, level, online;
            RewardSource source;

            db_id = tmp_int;
            villaingroup = tmp_int2;
            level = tmp_int3;
            source = tmp_int4;

            assert(source < REWARDSOURCE_COUNT);

            if (relayCommandStart(admin, db_id, str, &e, &online))
            {
                char * reward;
                char **rewardTables = NULL;

                reward = strtok(tmp_str,":");
                while(reward)
                {
                    eaPush(&rewardTables, reward);
                    reward = strtok(NULL,":");
                }

                // calc level as player if not set
                // due to sgrp badge reward forwarding
                if( level == REWARDLEVELPARAM_USEPLAYERLEVEL )
                    level = character_CalcEffectiveCombatLevel(e->pchar)+1;

                rewardFindDefAndApplyToEnt(e, rewardTables, villaingroup, level, true, source, NULL);
                eaDestroy(&rewardTables);
                relayCommandEnd(&e, &online);
            }
        }
        xcase SCMD_REWARD_CONCEPTITEM:
        {
            const ConceptDef *si = conceptdef_Get( tmp_str );
            if(!e)
                break;

            if( si )
            {
                character_AdjustConcept(e->pchar, si->id, tmp_f32s, tmp_int);
                conPrintf(client, "Awarding concept %s", tmp_str );
            }
            else
            {
                conPrintf(client, "concept %s not found", tmp_str );
            }
        }
        xcase SCMD_REWARD_CONCEPTDEF:
        {
            const ConceptDef *si = conceptdef_Get( tmp_str );
            if(!e)
                break;

            if( si )
            {
                character_AddRollConcept( e->pchar, si->id, tmp_int );
                conPrintf(client, "Awarding concept %s", tmp_str );
            }
            else
            {
                conPrintf(client, "concept %s not found", tmp_str );
            }
        }
        xcase SCMD_REWARD_RECIPE:
        {
             const DetailRecipe *si = NULL;
             if(!e)
                 break;

             si = recipe_GetItem( tmp_str );
             if( si )
             {
                if (si->Type == kRecipeType_Drop) 
                {
                     conPrintf(client, "Awarding recipe %s", tmp_str );
                     character_AdjustRecipe( e->pchar, si, 1, "scmd" );
                } else {
                     conPrintf(client, "Can't award global recipe %s", tmp_str );
                }
             }
             else
             {
                 conPrintf(client, "recipe %s not found", tmp_str );
             }
        }
        xcase SCMD_REWARD_RECIPE_ALL:
        {
            int i;
             if(!e)
                 break;
            conPrintf(client,"Awarding all recipes");
            for( i = MIN(eaSize(&g_DetailRecipeDict.ppRecipes) - 1,1000); i >= 0; --i)
            {
                character_AdjustRecipe( e->pchar, recipe_GetItem(g_DetailRecipeDict.ppRecipes[i]->pchName), 1, "scmdall");
            }
        }
        xcase SCMD_REWARD_DETAILRECIPE:
        {
             const DetailRecipe *si = NULL;
             if(!e)
                 break;

            if( !e->supergroup )
            {
                conPrintf(client,"can't give recipe, player has no supergroup.");
                break;
            }

             si = detailrecipedict_RecipeFromName(tmp_str);
             if( si )
             {
                 conPrintf(client, "Awarding detailrecipe %s", tmp_str );
                sgrp_AdjRecipe(e, si, 1, tmp_int);
             }
             else
             {
                 conPrintf(client, "detailrecipe %s not found", tmp_str );
             }
        }
        xcase SCMD_COSTUME_SLOT_GRANT:
        {
            char buf[256];

            if(!e)
                break;
    
            sprintf_s(buf, 256, "reward costumeslot %d", e->pchar->iLevel);
            serverParseClientEx(buf, client, ACCESS_INTERNAL);
        }
        xcase SCMD_CHECK_COSTUME_SLOTS:
        {
            StoryInfo* info = NULL;

            if(!e || !e->pl)
                break;

            conPrintf(client, "CheckCostumSlots: Only displays costume slots earned through the game");
            conPrintf(client, "1 at the start");
            conPrintf(client, "%d earned through the game", e->pl->num_costume_slots);

            if (ContactDebugPlayerHasFinishedTask(e, "contacts/StoreNPCs/tasks/SergeTask.taskset", "TailorTask1") || 
                ContactDebugPlayerHasFinishedTask(e, "contacts/StoreNPCs/tasks/SergeTask.taskset", "TailorTask1.1"))
            {
                conPrintf(client, "Serge's task (tailorTask1.1) completed");
            }
            if (ContactDebugPlayerHasFinishedTask(e, "contacts/StoreNPCs/tasks/LaurenTask.taskset", "TailorTask1"))
            {
                conPrintf(client, "Lauren's task (TailorTask1) completed");
            }
            if (ContactDebugPlayerHasFinishedTask(e, "contacts/StoreNPCs/tasks/CarsonTask.taskset", "TailorTask1"))
            {
                conPrintf(client, "Carson's task (TailorTask1) completed");
            }
            if (ContactDebugPlayerHasFinishedTask(e, "v_contacts/villain_EAT/tasks/SL2.5_Johnson.taskset", "SL2.5_Johnson_Mission2"))
            {
                conPrintf(client, "Johnson's task (SL2.5_Johnson_Mission2) completed");
            }
            if (ContactDebugPlayerHasFinishedTask(e, "contacts/v_storeNPCs/tasks/SL4_Facemaker_CostumeTask.taskset", "SL4_Facemaker_Mission"))
            {
                conPrintf(client, "Facemaker's task (SL4_Facemaker_Mission) completed");
            }
            if (ContactDebugPlayerHasFinishedTask(e, "contacts/v_storeNPCs/tasks/SL6_Glenda_CostumeTask.taskset", "SL6_Glenda_Mission"))
            {
                conPrintf(client, "Glenda's task (SL6_Glenda_Mission) completed");
            }
            if (ContactDebugPlayerHasFinishedTask(e, "contacts/v_storeNPCs/tasks/SL8_Linda_CostumeTask.taskset", "SL8_Linda_Mission"))
            {
                conPrintf(client, "Linda's task (SL8_Linda_Mission) completed");
            }
            if (getRewardToken(e, "Halloween2006CostumeSlot") != NULL)
            {
                conPrintf(client, "Halloween Salvage completed");
            }

        }
        xcase SCMD_INVENTION_REWARDALL:
        {
            int i;
            int n;

             if(!e)
                 break;

            n = eaSize(&g_SalvageDict.ppSalvageItems);
            for( i = 0; i < n; ++i )
            {
                SalvageItem const *si = g_SalvageDict.ppSalvageItems[i];
                if( si )
                {
                    // add 10 salvage
                    character_AdjustSalvage( e->pchar, si, 10, "scmdall", false );
                }
            }
        }
        xcase SCMD_INV_CLEARALL:
        {
            if(!e)
                break;
            character_ClearInventory(e->pchar,kInventoryType_Recipe,"SCMD_INV_CLEARALL");
            character_ClearInventory(e->pchar,kInventoryType_BaseDetail,"SCMD_INV_CLEARALL");
            character_ClearInventory(e->pchar,kInventoryType_Salvage,"SCMD_INV_CLEARALL");
            conPrintf(client, "Cleared inventory");
        }
        xcase SCMD_REMOVE_RECIPE:
        {
             const DetailRecipe *si = NULL;
             if(!e)
                 break;

             si = recipe_GetItem( tmp_str );
             if( si )
             {
                 conPrintf(client, "Removing recipe %s", tmp_str );
                 character_AdjustRecipe( e->pchar, si, -1, "scmd" );
             }
             else
             {
                 conPrintf(client, "recipe %s not found", tmp_str );
             }
        }
        xcase SCMD_RESETPREDICT:
            svrResetClientControls(client, 1);
        xcase SCMD_PRINTCONTROLQUEUE:
            svrPrintControlQueue(client);
        xcase SCMD_LEVEL_UP_XP:
            if(e->pchar->pclass)
            {
                if(tmp_int==0)
                {
                    tmp_int = e->pchar->iLevel+1;
                }
                else
                {
                    // input is 1-based, make it 0-based
                    tmp_int = tmp_int-1;
                }

                if(tmp_int<MAX_PLAYER_LEVEL && tmp_int >=0)
                {
                    e->general_update = 1;
                    e->pchar->iExperiencePoints=g_ExperienceTables.aTables.piRequired[tmp_int];
                    character_LevelFinalize(e->pchar, true);
                    if( character_IsMentor(e->pchar) )
                        character_updateTeamLevel(e->pchar);
                }
                e->pl->csrModified = true;
                conPrintf(client, clientPrintf(client,"LevelupXP", e->pchar->iExperiencePoints));
                //does not work on leveling-pacted characters
                if(e->levelingpact)
                {
                    conPrintf(client, clientPrintf(client,"LevelupXPInLPact"));
                }
            }
            else
            {
                conPrintf(client, clientPrintf(client,"Character appears to be invalid. Not attempting XP increase."));
            }
        xcase SCMD_TRAIN:
            e->pl->levelling = 1;
            START_PACKET(pak, e, SERVER_LEVEL_UP);
            pktSendBits(pak, kPowerSystem_Numbits, (bool)tmp_int);
            pktSendBitsPack(pak,4,character_GetLevel(e->pchar));
            character_VerifyPowerSets(e->pchar);
            END_PACKET
        xcase SCMD_LOCALE:
            setCurrentLocale(locGetIDByNameOrID(tmp_str));
            setCurrentMenuMessages();
            conPrintf(client, "Server set to locale %s", locGetName(getCurrentLocale()));
        xcase SCMD_CONTACT_SHOW:
            ContactDebugShowAll(client);
        xcase SCMD_CONTACT_DATAMINE:
        {
            ContactDebugDatamine(client, tmp_int ? true : false);
        }
        xcase SCMD_CONTACT_SHOW_MINE:
            if (e)
                ContactDebugShowMine(client, e);
        xcase SCMD_CONTACT_ADD:
            if (e)
                ContactDebugAdd(client, e, tmp_str);
        xcase SCMD_CONTACT_ADD_ALL:
            if (e)
                ContactDebugAddAll(client, e);
        xcase SCMD_CONTACT_GOTO:
            if (e)
                ContactGoto(client, e, tmp_str, 0);
        xcase SCMD_CONTACT_GOTO_AND_SELECT:
            if (e)
                ContactGoto(client, e, tmp_str, 1);
        xcase SCMD_CONTACT_INTRODUCTIONS:
            if (e)
                ContactDebugShowIntroductions(client, tmp_str);
        xcase SCMD_CONTACT_INTROPEER:
            if (e)
                ContactDebugGetIntroduction(client, tmp_str, 0);
        xcase SCMD_CONTACT_INTRONEXT:
            if (e)
                ContactDebugGetIntroduction(client, tmp_str, 1);
        xcase SCMD_CONTACT_INTERACT:
            if(e)
                ContactDebugInteract(client, tmp_str, 0);
//         xcase SCMD_CONTACT_NEWSPAPER:
//             if(e)
//                 ContactNewspaperInteract(client);
        xcase SCMD_ROGUE_VERIFYCONTACTS:
            if(e)
            {
                char *temp = estrTemp();
                ContactValidateAllManually(&temp);
                chatSendToPlayer( client->entity->db_id, temp, INFO_SVR_COM, 0 );
                estrDestroy(&temp);
            }
        xcase SCMD_ROGUE_VERIFYMYCONTACTS:
            if(e)
            {
                char *temp = estrTemp();
                ContactValidateEntityAll(e, &temp);
                if(!strlen(temp))
                    estrPrintCharString(&temp, "No Errors found.");
                chatSendToPlayer( client->entity->db_id, temp, INFO_SVR_COM, 0 );
                estrDestroy(&temp);
            }
        xcase SCMD_ROGUE_SENDMESSAGEIFONALIGNMENTMISSION:
            {
                Entity *ent = findEntOrRelayById(admin, tmp_int, str, NULL);

                if (ent && ent->storyInfo && ent->teamup && ent->teamup->activetask)
                {
                    StoryTaskInfo *taskInfo = TaskGetInfo(ent, &ent->teamup->activetask->sahandle);
                    if (taskInfo)
                    {
                        ContactHandle contactHandle = TaskGetContactHandle(ent->storyInfo, taskInfo);
                        const ContactDef *contact = ContactDefinition(contactHandle);
                        if (contact && contact->tipType == TIPTYPE_ALIGNMENT)
                        {
                            char command[50];
                            sprintf(command, "%s %i", "printMessageOnAlignmentMission", tmp_int2);
                            serverParseClient(command, NULL);
                        }
                    }
                }
            }
        xcase SCMD_ROGUE_PRINTMESSAGEONALIGNMENTMISSION:
            {
                Entity *ent = findEntOrRelayById(admin, tmp_int, str, NULL);

                if (ent)
                {
                    sendDialog(ent, "CannotEarnAlignmentPointsFromCurrentMission");
                }
            }
        xcase SCMD_CONTACT_CELL:
            if(e)
                ContactDebugInteract(client, tmp_str, 1);
        xcase SCMD_CONTACT_GETALL:
            if(e)
            {
                ContactStatusSendAll(e);
                ContactStatusAccessibleSendAll(e);
            }
        xcase SCMD_TASK_GETALL:
            if (e)
                TaskStatusSendAll(e);
        xcase SCMD_TASK_GETDETAIL:
            if (e)
                TaskDebugDetailPage(client, e, tmp_int);
        xcase SCMD_UNIQUETASK_INFO:
            if (e)
                UniqueTaskDebugPage(client, e);
        xcase SCMD_CONTACT_DETAILS:
            if (e)
                ContactDebugShowDetail(client, e, tmp_str);
        xcase SCMD_CONTACT_SETCXP:
            if (e)
                ContactDebugSetCxp(client, e, tmp_str, tmp_int);
        xcase SCMD_INFLUENCE:
            if(e && e->pchar)
            {
                ent_SetInfluence(e, tmp_int);
            }
        xcase SCMD_INFLUENCE_ADD:
            if(e && e->pchar && e->pl)
            {
                S64 infCap = accountLoyaltyRewardGetInfluenceCap(e->pl->loyaltyPointsEarned, ent_GetProductInventory( e ), e->pl->account_inventory.accountStatusFlags);
 
                if (((S64) e->pchar->iInfluencePoints + (S64) tmp_int > infCap ||
                        e->pchar->iInfluencePoints < -1000000) &&
                    e->pchar->iInfluencePoints < infCap) //cap influence
                {
                    e->pchar->iInfluencePoints = (int) infCap;
                }
                else if (e->pchar->iInfluencePoints + tmp_int < 0)
                {
                    e->pchar->iInfluencePoints  = 0;
                } else {
                    e->pchar->iInfluencePoints += tmp_int;
                }
            }
        xcase SCMD_LEVELINGPACT_EXIT:
            if(e && e->pchar && e->pl && tmp_int>0)
            {
                int oldLevel = character_CalcExperienceLevel(e->pchar);
                int iXPShouldBe = tmp_int / MAX(2, tmp_int2);
                int iXPReceived = character_IncreaseExperienceNoDebt(e->pchar, iXPShouldBe);
                devassert(tmp_int2 >= 2);
                if(iXPReceived < 0)
                    dbLog("LevelingPactExit", e, "SCMD_LEVELINGPACT_EXIT: negative xp %d",iXPReceived);
                else
                    stat_AddXPReceived(e, iXPReceived);
                if(iXPReceived > 0) // don't mention fallout from rounding errors
                {
                    //sendInfoBox(e, INFO_REWARD, "ExperienceYouReceivedLevelingPact", iXPReceived);
                    if(character_CalcExperienceLevelByExp(iXPShouldBe) > oldLevel)
                        levelupApply(e, oldLevel);
                }
            }
        xcase SCMD_PACTMEMBER_INFLUENCE_ADD:
            if(e && e->pchar)
            {
                //influence first
                ent_AdjInfluence(e, tmp_int, "drop");
                stat_AddInfluenceReceived(e->pl, tmp_int);
                badge_RecordInfluence(e, tmp_int);
            }

        xcase SCMD_PACTMEMBER_EXPERIENCE_GET:
            // this command takes the pact id as a parameter to avoid any timing issues (perhaps excessively)
            if(e && e->pchar)
                SgrpStat_SendPassthru(e->db_id, 0, "statserver_levelingpact_csrstart %d %d %d", tmp_int, e->pchar->iExperiencePoints, e->total_time + SecondsSince2000()- e->last_login);

        xcase SCMD_PRESTIGE:
            if(e && e->supergroup)
            {
                if( teamLock( e, CONTAINER_SUPERGROUPS ) )
                {
                    e->supergroup->prestige = tmp_int;
                    if (e->supergroup->prestige > MAX_INFLUENCE ||
                        e->supergroup->prestige < -1000000) //cap prestige
                    {
                        e->supergroup->prestige = MAX_INFLUENCE;
                    }
                    teamUpdateUnlock( e, CONTAINER_SUPERGROUPS );
                    e->supergroup_update = 1;
                }
            }
        xcase SCMD_PRESTIGE_ADD:
            if(e && e->pchar)
            {
                if( teamLock( e, CONTAINER_SUPERGROUPS ) )
                {
                    e->supergroup->prestige += tmp_int;
                    if (e->supergroup->prestige > MAX_INFLUENCE ||
                        e->supergroup->prestige < -1000000) //cap prestige
                    {
                        e->supergroup->prestige = MAX_INFLUENCE;
                    }
                    teamUpdateUnlock( e, CONTAINER_SUPERGROUPS );
                    e->supergroup_update = 1;
                }
            }
        xcase SCMD_SG_INFLUENCE:
            if(e && e->supergroup)
            {
                if( teamLock( e, CONTAINER_SUPERGROUPS ) )
                {
                    e->supergroup->influence = tmp_int;
                    if (e->supergroup->influence > MAX_INFLUENCE ||
                        e->supergroup->influence < -1000000) //cap prestige
                    {
                        e->supergroup->influence = MAX_INFLUENCE;
                    }
                    else if (e->supergroup->influence < 0)
                    {
                        e->supergroup->influence = 0;
                    }
                    teamUpdateUnlock( e, CONTAINER_SUPERGROUPS );
                    e->supergroup_update = 1;
                }
            }
        xcase SCMD_SG_INFLUENCE_ADD:
            if(e && e->pchar)
            {
                if( teamLock( e, CONTAINER_SUPERGROUPS ) )
                {
                    e->supergroup->influence += tmp_int;
                    if (e->supergroup->influence > MAX_INFLUENCE ||
                        e->supergroup->influence < -1000000) //cap prestige
                    {
                        e->supergroup->influence = MAX_INFLUENCE;
                    }
                    else if (e->supergroup->influence < 0)
                    {
                        e->supergroup->influence = 0;
                    }
                    teamUpdateUnlock( e, CONTAINER_SUPERGROUPS );
                    e->supergroup_update = 1;
                }
            }
        xcase SCMD_EXPERIENCE:
            if(e
                && e->pchar
                && tmp_int > e->pchar->iExperiencePoints)
            {
                e->pchar->iExperiencePoints =
                    MIN(tmp_int,g_ExperienceTables.aTables.piRequired[MAX_PLAYER_SECURITY_LEVEL-1]);
                e->pchar->iCombatLevel = character_CalcCombatLevel(e->pchar);
                character_HandleCombatLevelChange(e->pchar);

            }
        xcase SCMD_EXPERIENCE_ADD:
            if(e && e->pchar && tmp_int > 0)
            {
                e->pchar->iExperiencePoints +=
                    MIN(tmp_int,g_ExperienceTables.aTables.piRequired[MAX_PLAYER_SECURITY_LEVEL-1]-e->pchar->iExperiencePoints);
                e->pchar->iCombatLevel = character_CalcCombatLevel(e->pchar);
                character_HandleCombatLevelChange(e->pchar);
            }
        xcase SCMD_XPDEBT:
            if(e
                && e->pchar
                && tmp_int >= 0)
            {
                e->pchar->iExperienceDebt = tmp_int;
                character_CapExperienceDebt(e->pchar);
            }
        xcase SCMD_XPDEBT_REMOVE:
            if(e && e->pchar && tmp_int > 0)
            {
                e->pchar->iExperienceDebt -= tmp_int;
                character_CapExperienceDebt(e->pchar);
            }

        xcase SCMD_WISDOM:
            if(e
                && e->pchar
                && tmp_int > e->pchar->iWisdomPoints)
            {
                e->pchar->iWisdomPoints = tmp_int;
                character_LevelFinalize(e->pchar, true);
            }
        xcase SCMD_WISDOM_ADD:
            if(e && e->pchar && tmp_int > 0)
            {
                e->pchar->iWisdomPoints += tmp_int;
                character_LevelFinalize(e->pchar, true);
            }
        xcase SCMD_REPUTATION:
            if(e && e->pl)
            {
                playerSetReputation(e,tmp_float);
            }
        xcase SCMD_REPUTATION_ADD:
            if(e && e->pl)
            {
                playerAddReputation(e,tmp_float);
            }
        xcase SCMD_SHARDCOMM:
            shardCommSend(e,true,tmp_str);
        xcase SCMD_MISSION_SHOW:
            MissionShowState(client);
        xcase SCMD_MISSION_SHOWFAKECONTACT:
            MissionSetShowFakeContact(tmp_int);
            MissionSendRefresh(client->entity);
        xcase SCMD_PNPC_SHOW:
            PNPCPrintAllOnMap(client);
        xcase SCMD_VISITLOCATIONS_SHOW:
            VisitLocationPrintAllOnMap(client);
        xcase SCMD_DOORS_SHOW:
            dbDebugPrintTrackedDoors(client, 0);
        xcase SCMD_DOORS_SHOW_MISSION:
            dbDebugPrintTrackedDoors(client, 1);
        xcase SCMD_DOORS_SHOW_MISSION_TYPES:
            dbDebugPrintMissionDoorTypes(client);
        xcase SCMD_STORY_INFO_RESET:
            if(e)
            {
                StoryArcInfoReset(e);
            }
        xcase SCMD_STORY_TOGGLE_CONTACT:
            if (e)
            {
                e->db_flags ^= DBFLAG_ALT_CONTACT;
                StoryArcInfoReset(e);
            }
        xcase SCMD_GOING_ROGUE_TIPS_RESET:
            if (e)
            {
                alignmentshift_resetAllTips(e);
            }
        xcase SCMD_PNPC_RELOAD:
            PNPCReload();
        xcase SCMD_DOORANIM_ENTER:
            DoorAnimDebugEnter(client, e, 0);
        xcase SCMD_DOORANIM_ENTER_ARENA:
            DoorAnimDebugEnter(client, e, 1);
        xcase SCMD_DOORANIM_EXIT:
            DoorAnimDebugExit(client, e, 0);
        xcase SCMD_DOORANIM_EXIT_ARENA:
            DoorAnimDebugExit(client, e, 1);
        xcase SCMD_RANDOMFAME_ADD:
            if (e)
                RandomFameAdd(e, tmp_str);
        xcase SCMD_RANDOMFAME_TEST:
            if (e && e->pl)
                e->pl->lastFameTime = 0;
        xcase SCMD_EMAILHEADERS:
            if (e)
                emailGetHeaders(e->db_id);
        xcase SCMD_EMAILREAD:
            if (e)
                emailGetMessage(e->db_id,tmp_int);
        xcase SCMD_EMAILDELETE:
            if (e)
                emailDeleteMessage(e,tmp_int);
        xcase SCMD_EMAILSEND:
        {
            static    char    *subj,*recips,*body;
            static    int        subj_max,recips_max,body_max;

            fitUnescapeString(tmp_str2,&subj,&subj_max);
            fitUnescapeString(tmp_str,&recips,&recips_max);
            fitUnescapeString(tmp_str3,&body,&body_max); 

            emailHandleSendCmd(e, subj, recips, body, 0, 0, 0, client);
        }
        xcase SCMD_EMAILSENDATTACHMENT:
        {
            static    char    *subj,*recips,*body;
            static    int        subj_max,recips_max,body_max;

            fitUnescapeString(tmp_str2,&subj,&subj_max);
            fitUnescapeString(tmp_str,&recips,&recips_max);
            fitUnescapeString(tmp_str3,&body,&body_max); 

            emailHandleSendCmd(e, subj, recips, body, tmp_int, tmp_int2, tmp_int3, client);
        }
        xcase SCMD_EMAILSYSTEM:
            createSystemEmail( e,  player_name, tmp_str, tmp_str2, tmp_int, tmp_str3, tmp_int2 );
        xcase SCMD_SHOW_OBJECTIVES:
            MissionPrintObjectives(client);

        xcase SCMD_GOTO_OBJECTIVE:
            MissionGotoObjective(client, tmp_int);

        xcase SCMD_NEXT_OBJECTIVE:
            gotoNextObjective(client);

        xcase SCMD_ASSIST:
            if(e)
            {
                character_AssistByName(e->pchar, player_name);
            }
        xcase SCMD_ACCESSLEVEL:
        {
            char    db_ip[100];

            strcpy(db_ip,makeIpStr(db_comm_link.addr.sin_addr.S_un.S_addr));
            // cryptic's local server, or local map editing
            if (   strncmp(db_ip,"172.31.80",9)==0            // NCNC Desktops 
                || strncmp(db_ip,"172.31.84",9)==0             // NCNC Shards
                || strncmp(db_ip,"172.31.26",9)==0             // NCNC Proxy IP, in case it grabbed that
                || strncmp(db_ip,"172.30.67.73",12)==0         // QA1
                || strncmp(db_ip,"172.30.67.74",12)==0         // QA2
                || strncmp(db_ip,"172.30.67.75",12)==0         // QA3
                || db_state.local_server)
            {
                client->entity->access_level = tmp_int;
                client->ready = CLIENTSTATE_RE_REQALLENTS;
                shardCommLogout(client->entity);
                shardCommLogin(client->entity,1);
            }
            else
            {
                conPrintf(client,clientPrintf(client,"AccessLevelInGame"));
                if (client->link)
                    LOG_ENT(client->entity, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, 
                            "Security:Accesslevel External used /accesslevel: Auth:%s IP:%s (command ignored)",
                            client->entity->auth_name, makeIpStr(client->link->addr.sin_addr.S_un.S_addr));
            }
        }

        xcase SCMD_STUCK:
            svrPlayerFindLastGoodPos(client->entity);
        xcase SCMD_SYNC:
            entSyncControl(e);
        xcase SCMD_FORCE_MISSION_LOAD:
            MissionForceLoad(tmp_int);
        xcase SCMD_MISSION_STATS:
            server_error_sent_count = 0;
            MissionDebugPrintMapStats(client, client->entity, dbGetMapName(getMapID()));
        xcase SCMD_MISSION_STATS_TO_FILE:
            server_error_sent_count = 0;
            MissionDebugSaveMapStatsToFile(client->entity, db_state.map_name);
        xcase SCMD_MAP_CHECK_STATS:
            server_error_sent_count = 0;
            MissionDebugCheckMapStats(client, client->entity, tmp_int, dbGetMapName(getMapID()));
        xcase SCMD_ALL_MAP_CHECK_STATS:
            if (isDevelopmentMode() && sharedMemoryGetMode() == SMM_DISABLED) {
            server_error_sent_count = 0;
            MissionDebugCheckAllMapStats(client, client->entity);
            } else {
                conPrintf(client, "When shared memory is enabled, /mapstats_checkall is not supported");
            }
        xcase SCMD_MISSION_CONTROL:
            server_error_sent_count = 0;
            MissionControlHandleRequest(client, client->entity, tmp_int);
        xcase SCMD_GOTO_SPAWN:
            if (e)
            {
                char errmsg[200];
                Vec3 nowhere = {0};
                errmsg[0] = 0;
                DoorAnimEnter(e, NULL, nowhere, 0, NULL, tmp_str, XFER_STATIC, errmsg);
                if (errmsg[0])
                {
                    Errorf(errmsg);
                }
            }
        xcase SCMD_SCAN_LOG:
        {
            char    buf[1000];

            sprintf(buf,"-scanlog %s",tmp_str);
            conPrintf(client,"%s",dbQuery(buf));
        }
        xcase SCMD_NET_PROFILE:
            if (pktGetDebugInfo())
            {
                Errorf("Packet debug is currently enabled. Temporarily disabling it while the profiling is happening. If you want to have to collect stats while packet debug info is being sent, remove asserts in pkt send.");
            }
            netprofile_resetStats();
            netprofile_setTimer(tmp_int);
        xcase SCMD_NOSHAREDMEMORY:
            sharedMemorySetMode(SMM_DISABLED);
        xcase SCMD_BE_VILLAIN:
        {
            int        index;
            const NPCDef *npc = npcFindByName(tmp_str, &index);
            if (index)
            {
                const cCostume *npcCostume = npcDefsGetCostume(index, 0);

                if(npcCostume)
                {
                    entSetImmutableCostumeUnowned( e, npcCostume ); // e->costume = npcCostume;
                    svrChangeBody(e, entTypeFileName(e));
                    e->npcIndex = index;
                    e->send_costume = 1;
                    e->pl->npc_costume = index;
                }
            }
        }
        xcase SCMD_BE_SELF:
        {
            entSetMutableCostumeUnowned( e, costume_current(e) ); // e->costume = cpp_reinterpret_cast(cCostume*)(e->pl->costume[e->pl->current_costume]);
            e->powerCust = e->pl->powerCustomizationLists[e->pl->current_powerCust];
            svrChangeBody(e, entTypeFileName(e));
            e->npcIndex = 0;
            e->costumePriority = 0;
            e->send_costume = 1;
            e->pl->npc_costume = 0;
        }
        xcase SCMD_BE_VILLAIN_FOR_REAL:
            if(e && ENTTYPE(e)==ENTTYPE_PLAYER)
            {
                BecomeVillain(e, tmp_str, tmp_int);
                e->send_costume = 1;
                e->pl->npc_costume = e->npcIndex;
            }
        xcase SCMD_AFK:
            if(e && ENTTYPE(e)==ENTTYPE_PLAYER)
            {
                if(e->pl)
                {
                    if(afk[0]=='\0' || IsAnyWordProfane(afk))
                        strcpy( afk, localizedPrintf(e,"AFK") );

                    e->pl->afk = 1;
                    e->pl->send_afk_string = 1;
                    strncpy(e->pl->afk_string, afk, MAX_AFK_STRING);
                }
                afk[0]='\0';
            }
        xcase SCMD_DEBUG:
            {
                char * estr = NULL;
                conPrintf(client, csrPlayerInfo(&estr, client, player_name, 0));
                estrDestroy(&estr);
            }
        xcase SCMD_DEBUG_DETAIL:
            {
                char * estr = NULL;
                conPrintf(client, csrPlayerInfo(&estr, client, player_name, 1));
                estrDestroy(&estr);
            }

        xcase SCMD_DEBUG_TRAY:
            {
                char * estr = NULL;
                conPrintf(client, csrPlayerTrayInfo(&estr, client, player_name));
                estrDestroy(&estr);
            }

        xcase SCMD_DEBUG_SPECIALIZATIONS:
            {
                char * estr = NULL;
                conPrintf(client, csrPlayerSpecializationInfo(&estr, client, player_name));
                estrDestroy(&estr);
            }

        xcase SCMD_DEBUG_POWER:
            {
                const PowerCategory *pcat = powerdict_GetCategoryByName(&g_PowerDictionary, tmp_str);
                if(pcat!=NULL)
                {
                    const BasePowerSet *pset = category_GetBaseSetByName(pcat, tmp_str2);
                    if(pset!=NULL)
                    {
                        const BasePower *ppowBase = baseset_GetBasePowerByName(pset, tmp_str3);
                        if(ppowBase!=NULL)
                        {
                            Power *ppow = character_OwnsPower(client->entity->pchar, ppowBase);
                            if(ppow)
                            {
                                conPrintf(client, "Watching power %s.%s.%s\n", pcat->pchName, pset->pchName, ppowBase->pchName);
                                client->entity->pchar->stats.ppowLastAttempted = ppow;
                            }
                            else
                            {
                                conPrintf(client, "Player doesn't have %s.%s.%s\n", pcat->pchName, pset->pchName, ppowBase->pchName);
                            }
                        }
                        else
                        {
                            conPrintf(client, "Power doesn't exist (%s)\n", tmp_str3);
                        }
                    }
                    else
                    {
                        conPrintf(client, "Power set doesn't exist (%s)\n", tmp_str2);
                    }
                }
                else
                {
                    conPrintf(client, "Category set doesn't exist (%s)\n", tmp_str);
                }
            }

        xcase SCMD_GOTO:
            {
                char buf[1000];
                if (dbPlayerIdFromName(player_name) < 0)
                {
                    conPrintf(client,clientPrintf(client,"PlayerDoesntExist",player_name));
                }
                else
                {
                    sprintf(buf,"goto_internal \"%s\" \"%s\"", player_name, client->entity->name);
                    serverParseClient(buf, 0);
                }
            }
        xcase SCMD_GOTO_INTERNAL:
            {
                int result;
                Entity *e = findEntOrRelay(admin,player_name,str,&result);
                if(e)
                {
                    char buf[1000];
                    sprintf(buf,"mapmovepos \"%s\" %d %f %f %f 0 0",tmp_str, e->map_id, ENTPOSPARAMS(e));
                    serverParseClient(buf, 0);
                }
            }
        xcase SCMD_EDITMESSAGE:
            sendEditMessage(client->link, 0, "%s", tmp_str);
        xcase SCMD_GOTOMISSION:
            MissionGotoMissionDoor(client);
        xcase SCMD_TESTMISSION:
            {
                char cmd[1000];
                sprintf(cmd, "gettask %200s", tmp_str);
                serverParseClient(cmd, client);
                serverParseClient("gotomission", client);
            }
        xcase SCMD_STATS:
            {
                Entity * target;
                int id = dbPlayerIdFromName(player_name);
                target = entFromDbId(id);
                if(!target) target=e;
                stat_Dump(client, target);
            }
        xcase SCMD_CLEAR_STATS:
            {
                Entity * target;
                int id = dbPlayerIdFromName(player_name);
                target = entFromDbId(id);
                if(!target) target=e;
                stat_Init(target->pl);
            }
        xcase SCMD_KIOSK:
            {
                if(!stricmp(tmp_str, "arena"))
                    ArenaRequestLeaderBoard(e);
                else
                    kiosk_Navigate(e, tmp_str, tmp_str2);
            }
        xcase SCMD_NOJUMPREPEAT:
            if(e)
            {
                setNoJumpRepeat(e, tmp_int);
            }
        xcase SCMD_MISSIONX:
        {
            MissionAssignRandom(client);
        }
        xcase SCMD_MISSIONXMAP:
        {
            MissionAssignRandomWithInstance(client);
        }
        xcase SCMD_SOUVENIRCLUE_GET:
        {
            scApplyToEnt(e, (const char**)eaFromPointerUnsafe(tmp_str));
        }
        xcase SCMD_SOUVENIRCLUE_REMOVE:
        {
            if(e && e->storyInfo)
                scRemoveDebug(e, tmp_str);
        }
        xcase SCMD_SOUVENIRCLUE_APPLY:
        {
            Entity *e;
            int        db_id, online;

            db_id = tmp_int;
            if (relayCommandStart(admin, db_id, str, &e, &online))
            {
                char *clue, **clues;

                eaCreate(&clues);
                for (clue = strtok(tmp_str, ":"); clue; clue = strtok(NULL, ":"))
                    eaPush(&clues, clue);

                scApplyToEnt(e, clues);
                eaDestroy(&clues);
                relayCommandEnd(&e, &online);
            }
        }

        xcase SCMD_TEAMTASK_SET:
        {
            char* errorMsg;
            TeamTaskSelectError error;

            if (!e->teamup)
                teamCreate(e, CONTAINER_TEAMUPS);

            error = TeamTaskSelect(e, tmp_int, tempStoryTaskHandle(tmp_int2, tmp_int3,0,  TaskForceIsOnArchitect(e)));
            errorMsg = TeamTaskSelectGetError(error);
            if (errorMsg)
                sendChatMsg(e, localizedPrintf(e, errorMsg), INFO_SVR_COM, 0);
        }
        xcase SCMD_TEAMTASK_ABANDON:
        {
            int context = tmp_int;
            int subhandle = tmp_int2;
            StoryInfo *storyInfo = e->storyInfo;
            StoryContactInfo *contactInfo;
            StoryTaskHandle taskHandle;
            StoryTaskInfo *taskInfo;

            taskHandle.context = context;
            taskHandle.subhandle = subhandle;
            taskInfo = TaskGetInfo(e, &taskHandle);
            if (taskInfo)
            {
                contactInfo = TaskGetContact(storyInfo, taskInfo);
            
                if (storyInfo && contactInfo && taskInfo && TaskCanBeAbandoned(e, taskInfo))
                {
                    TaskAbandon(e, storyInfo, contactInfo, taskInfo, 0);
                    sendChatMsg(e, localizedPrintf(e, "MissionAbandon_Success"), INFO_SVR_COM, 0);
                }
                else
                {
                    sendChatMsg(e, localizedPrintf(e, "MissionAbandon_Unable"), INFO_SVR_COM, 0);
                }
            }
            else
            {
                sendChatMsg(e, localizedPrintf(e, "MissionAbandon_Unable"), INFO_SVR_COM, 0);
            }
        }
        xcase SCMD_PETITION_INTERNAL:
            if (e && e->pl)
            {
                U32 time = dbSecondsSince2000();
                if( e->pl->lastPetitionSent && time - e->pl->lastPetitionSent < 300 )    //5 min.  Players and RMT'ers sometimes spam CS.  Weird.
                {
                    sendChatMsg(e, localizedPrintf(e, "PetitionWaitMsg"), INFO_SVR_COM, 0);
                }
//                else if (!AccountIsVIP(ent_GetProductInventory(e), e->pl->account_inventory.accountStatusFlags))
//                {
//                    sendChatMsg(e, localizedPrintf(e, "InvalidPermissionVIP"), INFO_SVR_COM, 0 );
//                }
                else
                {
                    csrPetition(e,tmp_str);
                    e->pl->lastPetitionSent = time;
                }
            }
        xcase SCMD_TITLE_1:
            if(e && e->pl)
            {
                strncpyt(e->pl->titleCommon, tmp_str, 32);
                e->send_costume = 1;
            }
        xcase SCMD_TITLE_2:
            if(e && e->pl)
            {
                strncpyt(e->pl->titleOrigin, tmp_str, 32);
                e->send_costume = 1;
            }
        xcase SCMD_TITLE_THE:
            if(e && e->pl)
            {
                strncpyt(e->pl->titleTheText, tmp_str, 10);
                e->name_gender = tmp_int;
                e->pl->attachArticle = tmp_int2;
                e->send_costume = 1;
            }
        xcase SCMD_TITLE_SPECIAL:
            if(e && e->pl)
            {
                strncpyt(e->pl->titleSpecial, tmp_str, 128);
                e->draw_update = 1;
            }
        xcase SCMD_TITLE_SPECIAL_EXPIRES:
            if(e && e->pl)
            {
                e->pl->titleSpecialExpires =
                    (U32)(tmp_float*60*60+dbSecondsSince2000());
            }
        xcase SCMD_TITLE_AND_EXPIRES_SPECIAL:
            if(e && e->pl)
            {
                strncpyt(e->pl->titleSpecial, tmp_str, 128);
                e->draw_update = 1;
                e->pl->titleSpecialExpires =
                    (U32)(tmp_float*60*60+dbSecondsSince2000());
            }
        xcase SCMD_CSR_TITLE_AND_EXPIRES_SPECIAL:
        {
            char buf[1000];
            sprintf(buf, "csr \"%s\" titleandexpiresspecial \"%s\" %f", tmp_str, tmp_str2, tmp_float);
            serverParseClient(buf, client); 
            conPrintf(client, clientPrintf(client,"SpecialTitleSet"));
        }
        xcase SCMD_FX_SPECIAL:
            if(e && e->pl)
            {
                strncpyt(e->pl->costumeFxSpecial, tmp_str, 128);
                e->send_costume = 1;
            }
        xcase SCMD_FX_SPECIAL_EXPIRES:
            if(e && e->pl)
            {
                e->pl->costumeFxSpecialExpires =
                    (U32)(tmp_float*60*60+dbSecondsSince2000());
            }
        xcase SCMD_DELINKME:
            dbDelinkMe("/delinkme");
        xcase SCMD_MAP_SHUTDOWN:
            {
                Packet *pak = pktCreateEx(&db_comm_link, DBCLIENT_EMERGENCY_SHUTDOWN);
                pktSendString(pak, tmp_str);
                pktSend(&pak, &db_comm_link);
            }
        xcase SCMD_COLLRECORD:
            gridCollRecordStart();
        xcase SCMD_COLLRECORDSTOP:
            gridCollRecordStop();
        xcase SCMD_NEXT_DOOR:
            gotoNextDoor(client, 0);
        xcase SCMD_NEXT_MISSIONDOOR:
            gotoNextDoor(client, 1);
        xcase SCMD_SHOW_TASKDOOR:
            MissionDoorListFromTask(client, client->entity, tmp_str);
        xcase SCMD_TEAMLOG_DUMP:
            memlog_dump(&g_teamlog);
        xcase SCMD_TEAMLOG_ECHO:
            memlog_setCallback(&g_teamlog, teamlogEcho);
        xcase SCMD_TRICK_OR_TREAT:
            TrickOrTreatMode(tmp_int);
        xcase SCMD_PLAYER_MORPH:
            // do morphy stuff here
            if (e)
                morphCharacter(client, e, tmp_str, tmp_str2, tmp_str3, tmp_int);
        xcase SCMD_REQUEST_PACKAGEDENT:
            if (e && e->pl && e->pchar) {
                char *package = packageEnt(e);
                if(package)
                {
                    START_PACKET( pak, e, SERVER_SEND_PACKAGEDENT )
                    pktSendString(pak, package);
                    END_PACKET
                }
            }
        xcase SCMD_PLAYER_RENAME_RESERVED:
            {
                char buf[2000];
                int playerID;

                if( player_name[0] == 0 || tmp_str[0] == 0 )
                {
                    conPrintf(client, clientPrintf(client,"PlayerRenameBadFormat"));
                    break;
                }
                playerID = dbPlayerIdFromName( player_name );
                if (playerID <= 0)
                {
                    conPrintf(client, clientPrintf(client,"PlayerDoesntExist", player_name));
                    break;
                }
                sprintf(buf, "playerrenamerelay %i \"%s\" %i 1", playerID, tmp_str, e->db_id);
                serverParseClient(buf, NULL);
            }
        xcase SCMD_PLAYER_RENAME_PAID:
            {
                int db_id;
                char* authname;
                char* newname;
                Packet* pak;
                ValidateNameResult vnr;

                db_id = tmp_int;
                authname = tmp_str;
                newname = tmp_str2;

                pak = pktCreateEx(&db_comm_link, DBCLIENT_RENAME_RESPONSE);
                pktSendString(pak, authname);

                vnr = ValidateName(newname, client->entity->auth_name,
                    1, MAX_PLAYER_NAME_LEN(client->entity));

                if (!(client->entity->db_flags & DBFLAG_RENAMEABLE))
                {
                    pktSendString(pak, "PaidRenameNoToken");
                }
                else if (vnr != VNR_Valid)
                {
                    pktSendString(pak, "PaidRenameInvalidName");
                }
                else if (dbSyncPlayerRename(db_id, newname))
                {
                    pktSendString(pak, "PaidRenameSuccess");

                    RandomFameWipe(client->entity);
                    Strncpyt(client->entity->name, newname);
                    client->entity->db_flags &= ~DBFLAG_RENAMEABLE;
                    client->entity->name_update = 1;
                }
                else
                {
                    pktSendString(pak, "PaidRenameDuplicateName");
                }

                pktSend(&pak, &db_comm_link);
            }
        xcase SCMD_PLAYER_RENAME:
            {
                char buf[2000];
                int playerID;

                if( player_name[0] == 0 || tmp_str[0] == 0 )
                {
                    conPrintf(client, clientPrintf(client,"PlayerRenameBadFormat"));
                    break;
                }
                playerID = dbPlayerIdFromName( player_name );
                if (playerID <= 0)
                {
                    conPrintf(client, clientPrintf(client,"PlayerDoesntExist", player_name));
                    break;
                }
                sprintf(buf, "playerrenamerelay %i \"%s\" %i 0", playerID, tmp_str, e->db_id);
                serverParseClient(buf, NULL);
            }
        xcase SCMD_PLAYER_RENAME_RELAY:
            {
                Entity *e;
                int        db_id, admin_id, online, allow_reserved;
                char*    newname;
                char*    buf;

                db_id = tmp_int;
                newname = tmp_str;
                admin_id = tmp_int2;
                allow_reserved = tmp_int3;

                if (relayCommandStart(admin, db_id, str, &e, &online))
                {
                    ValidateNameResult vnr = ValidateName(newname, e->auth_name, 1,MAX_PLAYER_NAME_LEN(e));
                    if (!(vnr == VNR_Valid || allow_reserved && vnr == VNR_Reserved))
                    {
                        buf = localizedPrintf(e,"RenameInvalid", e->name, newname);
                    }
                    else if (dbSyncPlayerRename(db_id, newname))
                    {
                        RandomFameWipe(e);
                        buf = localizedPrintf(e,"RenameSuccess", e->name, newname);
                        Strncpyt(e->name, newname);
                        e->name_update = 1;
                    }
                    else
                    {
                        buf = localizedPrintf(e,"RenameExists", e->name, newname);
                    }
                    chatSendToPlayer(admin_id, buf, INFO_USER_ERROR, 0);
                    relayCommandEnd(&e, &online);
                }
            }
        xcase SCMD_CHECK_PLAYER_NAME:
            {
                U32 authId;
                int tempLockName;
                char* name;
                Packet* pak;
                ValidateNameResult vnr;
                char *clientAuthName = NULL;

                authId = tmp_int;
                tempLockName = tmp_int2;
                name = tmp_str;

                pak = pktCreateEx(&db_comm_link, DBCLIENT_CHECK_NAME_RESPONSE);
                pktSendBitsAuto(pak, authId);

                if (client && client->entity && client->entity->auth_name)
                {
                    clientAuthName = client->entity->auth_name;
                }
                vnr = ValidateName(name, clientAuthName,                //    I suspect that this could be just accountName
                    1, MAX_PLAYER_NAME_LEN(e));

                pktSendString(pak, name); //send back the name in case it needs to be locked

                switch (vnr)
                {
                    case VNR_Valid:
                        pktSendBitsAuto(pak, 1);  //CheckNameSuccess
                        break;

                    case VNR_Malformed:
                    case VNR_TooLong:
                    case VNR_TooShort:
                    case VNR_Profane:
                    case VNR_Reserved:
                    default:
                        pktSendBitsAuto(pak, 0);  //CheckNameFail
                        break;
                }
                pktSendBitsAuto(pak, tempLockName);
                pktSend(&pak, &db_comm_link);
            }
        xcase SCMD_SET_PLAYER_RENAME_TOKEN:
            {
                if(!e)
                    break;
                if(tmp_int)
                    e->db_flags |= DBFLAG_RENAMEABLE;
                else
                    e->db_flags &= (~DBFLAG_RENAMEABLE);
                conPrintf(client,"rename bit set");
            }
        xcase SCMD_UNLOCK_CHARACTER:
            {
                if(e && e->pl)
                {
                    if (dbSyncPlayerUnlock(e->db_id))
                    {
                        Packet *pak_out;

                        e->pl->slotLockLevel = SLOT_LOCK_NONE;

                        // send request up the chain!
                        pak_out = pktCreateEx(&db_comm_link, DBCLIENT_UNLOCK_CHARACTER_RESPONSE);
                        pktSendString(pak_out, e->auth_name);
                        pktSend(&pak_out,&db_comm_link);
                    }
                }
            }
        xcase SCMD_ADJUST_SERVER_SLOTS:
            {
                U32 auth_id = tmp_int;
                int delta = tmp_int2;
                if (dbSyncAccountAdjustServerSlots(auth_id, delta))
                {
                    conPrintf(client,"Server slots were adjusted. Use acc_tokeninfo AUTHNAME to verify.");
                }
                else
                {
                    conPrintf(client,"Server slots were NOT adjusted.");
                }
            }
        xcase SCMD_ORDER_RENAME:
            {
                OrderId order_id;
                order_id = orderIdFromString(tmp_str);

                if(e && e->pl)
                {
                    if(!e->pl->accountServerLock[0])
                    {
                        Packet *pak_out;

                        e->db_flags |= DBFLAG_RENAMEABLE;

                        entMarkOrderCompleted(e, order_id);

                        pak_out = pktCreateEx(&db_comm_link,DBCLIENT_ACCOUNTSERVER_ORDERRENAME);
                        pktSendBitsAuto2(pak_out, order_id.u64[0]);
                        pktSendBitsAuto2(pak_out, order_id.u64[1]);
                        pktSend(&pak_out,&db_comm_link);

                        dbLog("AccountServer",e,"Set rename flag");
                    }
                    else
                    {
                        dbLog("AccountServer",e,"Character locked by another AccountServer transaction, will try again later.");
                    }
                }
                else
                {
                    dbLog("AccountServer",e,"Unable to set rename flag, AccountServer will try again later.");
                }
            }

        xcase SCMD_ORDER_RESPEC:
            {
                OrderId order_id;
                order_id = orderIdFromString(tmp_str);

                if(e && e->pl)
                {
                    if(!e->pl->accountServerLock[0])
                    {
                        int counter = (e->pl->respecTokens >> 24);
                        Packet *pak_out;

                        counter++;

                        if (counter <= 255)
                        {
                            counter = counter << 24;

                            // clear out old entries
                            e->pl->respecTokens &= 0x00ffffff;

                            // put new counter in place
                            e->pl->respecTokens |= counter;

                            entMarkOrderCompleted(e, order_id);

                            pak_out = pktCreateEx(&db_comm_link, DBCLIENT_ACCOUNTSERVER_ORDERRESPEC);
                            pktSendBitsAuto2(pak_out, order_id.u64[0]);
                            pktSendBitsAuto2(pak_out, order_id.u64[1]);
                            pktSend(&pak_out,&db_comm_link);

                            dbLog("AccountServer",e,"Add respec");
                        }
                        else
                        {
                            dbLog("AccountServer",e,"Character has too many respecs, will try again later.");
                        }
                    }
                    else
                    {
                        dbLog("AccountServer",e,"Character locked by another AccountServer transaction, will try again later.");
                    }
                }
                else
                {
                    dbLog("AccountServer",e,"Unable to set respec flag, AccountServer will try again later.");
                }
            }

        xcase SCMD_SGROUP_RENAME:
            {
                char buf[2000];
                int playerID;

                if( player_name[0] == 0 || tmp_str[0] == 0 || tmp_str2[0] == 0 )
                {
                    conPrintf(client, clientPrintf(client,"SgRenameFormat"));
                    break;
                }
                playerID = dbPlayerIdFromName( player_name );
                if (playerID <= 0)
                {
                    conPrintf(client, clientPrintf(client,"PlayerDoesntExist", player_name));
                    break;
                }
                sprintf(buf, "sgrenamerelay %i \"%s\" \"%s\" %i", playerID, tmp_str, tmp_str2, e->db_id);
                serverParseClient(buf, NULL);
            }
            xcase SCMD_SGROUP_RENAME_RELAY:
            {
                Entity *e;
                int        db_id, admin_id, online;
                char    *newname, *oldname;

                db_id = tmp_int;
                oldname = tmp_str;
                newname = tmp_str2;
                admin_id = tmp_int2;

                if (relayCommandStart(admin, db_id, str, &e, &online))
                {
                    sgroup_rename(e, oldname, newname, admin_id);
                    relayCommandEnd(&e, &online);
                }
            }
            xcase SCMD_ARENA_JOIN:
            {
                ArenaRequestJoin(client->entity, tmp_int, tmp_int2, false, true);
            }
            xcase SCMD_ARENA_CREATE:
            {
                if (!ArenaCreateEvent(client->entity, tmp_str))
                    conPrintf(client, "Request failed");
            }
            xcase SCMD_ARENA_SETSIDE:
            {
                if (!ArenaSetSide(client->entity, tmp_int, tmp_int2, tmp_int3))
                    conPrintf(client, "Request failed");
            }
            xcase SCMD_ARENA_READY:
            {
                //if (!ArenaSetReady(client->entity, tmp_int, tmp_int2, tmp_int3))
                    conPrintf(client, "Request failed");
            }
            xcase SCMD_ARENA_SETMAP:
            {
                if (!ArenaSetMap(client->entity, tmp_int, tmp_int2, tmp_str))
                    conPrintf(client, "Request failed");
            }
            xcase SCMD_ARENA_ENTER:
            {
                if (!ArenaEnter(client->entity, tmp_int, tmp_int2))
                    conPrintf(client, "Request failed");
            }
            xcase SCMD_ARENA_CAMERA:
                ArenaSwitchCamera(client->entity, 1);
                conPrintf(client, "Turned into camera");
            xcase SCMD_ARENA_UNCAMERA:
                ArenaSwitchCamera(client->entity, 0);
                conPrintf(client, "Turned back from being camera");
            xcase SCMD_ARENA_POPUP:
                SendServerRunEventWindow(FindEventDetail(tmp_int, 0, NULL), client->entity);
                conPrintf(client, "Popping up arena window");
            xcase SCMD_ARENA_DEBUG:
                if (client && client->entity)
                    ArenaSendDebugInfo(client->entity->db_id);
            xcase SCMD_ARENA_PLAYER_STATS:
                if (client && client->entity)
                    ArenaRequestPlayerStats(client->entity->db_id);
            xcase SCMD_ARENA_KIOSK:
                if (client && client->entity)
                    ArenaKioskOpen(client->entity);
            xcase SCMD_ARENA_SCORE:
                if (client && client->entity)
                {
                    ArenaRequestCurrentResults(client->entity);
                    ArenaNotifyFinish(client->entity); // bit hacky way to open the arena result window
                }
            xcase SCMD_SGRAID_LENGTH:
                sprintf(tmp_str, "%i", tmp_int);
                RaidDebugSetVar(client, "raidlength", tmp_str);
            xcase SCMD_SGRAID_SIZE:
                sprintf(tmp_str, "%i", tmp_int);
                RaidDebugSetVar(client, "raidsize", tmp_str);
            xcase SCMD_SGRAID_SETVAR:
                RaidDebugSetVar(client, tmp_str, tmp_str2);
            xcase SCMD_SGRAID_STAT:
                sprintf(tmp_str, "SGAttacking %i, SGDefending %i, PlayerAttacking %i, PlayerDefending %i",
                    RaidSGIsAttacking(client->entity->supergroup_id),
                    RaidSGIsDefending(client->entity->supergroup_id),
                    RaidPlayerIsAttacking(client->entity),
                    RaidPlayerIsDefending(client->entity));
                conPrintf(client, "%s", tmp_str);
            xcase SCMD_SGRAID_BASEINFO:
                if (SGBaseId())
                {
                    RaidSetBaseInfo(SGBaseId(), tmp_int, tmp_int2);
                    conPrintf(client, "Setting this base's info raid size to %i, mount to %i", tmp_int, tmp_int2);
                }
                else if (client->entity && client->entity->supergroup_id)
                {
                    RaidSetBaseInfo(client->entity->supergroup_id, tmp_int, tmp_int2);
                    conPrintf(client, "Setting your supergroup's raid size to %i, mount to %i", tmp_int, tmp_int2);
                }
                else
                    conPrintf(client, "You need to be in a supergroup or in a supergroup base.");
            xcase SCMD_SGRAID_WINDOW:
                RaidSetWindow(client->entity, tmp_int, tmp_int2);
            xcase SCMD_SGRAID_CHALLENGE:
                if (!RaidDebugChallenge(client->entity, player_name))
                    conPrintf(client, "I can't find supergroup %s", player_name);
            xcase SCMD_SGRAID_CHALLENGE_INTERNAL:
                if (tmp_int == client->entity->supergroup_id)
                    conPrintf(client, "You can't challenge yourself");
                else if (!RaidChallenge(client->entity, tmp_int))
                    conPrintf(client, "You don't belong to a supergroup");
            xcase SCMD_SGRAID_WARP:
                RaidAttackerWarp(client->entity, tmp_int, 1);
            xcase SCMD_CSR_BUG_INTERNAL:
            {
                int result = 0;
                Entity *e = findEntOrRelay(admin,player_name,str,&result);
                if (e)
                {
                    Packet * pak = pktCreate();
                    pktSendBitsPack(pak, 1, SERVER_CSR_BUG_REPORT);
                    pktSendString(pak, tmp_str);        // summary
                    pktSendString(pak, tmp_str2);    // CSR player info
                    pktSend(&pak, e->client->link);
                }

                // send message to player who entered command
                if(    result
                    && (!e || (e->client != admin)))
                {
                    conPrintf(admin, clientPrintf(admin,"BugreportSubmitted", player_name));
                }
            }
            xcase SCMD_POWERS_CASH_IN:
            {
                csrPowersCashInEnhancements(client);
            }
            xcase SCMD_POWERS_RESET:
            {
                // 0 or 3 params only (prevents "powers_reset powers_reset" on a /csr)
                if(tmp_str && tmp_str[0]!='\0'
                   && tmp_str2 && tmp_str2[0]!='\0'
                   && tmp_str3 && tmp_str3[0]!='\0')
                {
                    csrPowersReset(client, tmp_str, tmp_str2, tmp_str3);
                }
                else
                {
                    csrPowersReset(client, NULL, NULL, NULL);
                }
            }
            xcase SCMD_POWERS_BUY_POWER_DEV:
            {
                csrPowersBuyDev(client, tmp_str, tmp_str2, tmp_str3);
            }
            xcase SCMD_POWERS_REVOKE_DEV:
            {
                const PowerCategory *pcat = powerdict_GetCategoryByName(&g_PowerDictionary, tmp_str);
                if(pcat!=NULL)
                {
                    const BasePowerSet *pset = category_GetBaseSetByName(pcat, tmp_str2);
                    if(pset!=NULL)
                    {
                        const BasePower *ppowBase = baseset_GetBasePowerByName(pset, tmp_str3);
                        if(ppowBase!=NULL)
                        {
                            conPrintf(client, clientPrintf(client,"RevokePowRevoke", pcat->pchName, pset->pchName, ppowBase->pchName));
                            character_RemoveBasePower(e->pchar, ppowBase);
                        }
                        else
                        {
                            conPrintf(client, clientPrintf(client,"RevokePowPowInvalid", tmp_str3));
                        }
                    }
                    else
                    {
                        conPrintf(client, clientPrintf(client,"RevokePowSetInvalid", tmp_str2));
                    }
                }
                else
                {
                    conPrintf(client, clientPrintf(client,"RevokePowCatInvalid", tmp_str));
                }
            }
            xcase SCMD_POWERS_CANCEL_DEV:
            {
                Entity *target;
                const BasePower *ppowBase = powerdict_GetBasePowerByFullName(&g_PowerDictionary, tmp_str);
                if(ppowBase == NULL)
                    return;
                
                target = validEntFromId(tmp_int);
                if(!target || !target->pchar)
                    return;
                
                character_CancelModsByBasePower(target->pchar, ppowBase);
            }
            xcase SCMD_POWERS_CANCEL:
            {
                Entity *e = NULL;
                Entity *target;
                const BasePower *ppowBase;
                bool allowedToCancel = false;
                
                if(client && client->entity)
                {
                    e = client->entity;
                    allowedToCancel = client->entity->access_level >= ACCESS_DEBUG;
                }

                ppowBase = powerdict_GetBasePowerByFullName(&g_PowerDictionary, tmp_str);
                if(ppowBase == NULL) return;
                if(!allowedToCancel && !ppowBase->bCancelable) return;
                
                target = validEntFromId(tmp_int);
                if(!target || !target->pchar) return;
                if(!allowedToCancel)
                {
                    if(e && (e == target)) allowedToCancel = true;
                    else if(e && target && target->erOwner && e == erGetEnt(target->erOwner) && !target->petByScript) allowedToCancel = true;
                }
                
                if(allowedToCancel) character_CancelModsByBasePower(target->pchar, ppowBase);
            }
            xcase SCMD_AUTOENHANCE:
            {
                csrPowersAutoEnhance(client->entity, 0);
            }
            xcase SCMD_AUTOENHANCEIO:
            {
                csrPowersAutoEnhance(client->entity, 1);
            }
            xcase SCMD_AUTOENHANCESET:
            {
                csrPowersAutoEnhance(client->entity, 3);
            }
            xcase SCMD_MAXBOOSTS:
            {
                csrPowersMaxSlots(client->entity);
            }
            xcase SCMD_SET_POWERDIMINISH:
            case SCMD_RECALC_STR:
            {
                int i;

                for(i=1;i<entities_max;i++)
                {
                    Entity *e = entities[i];
                    if ((entity_state[i] & ENTITY_IN_USE) && e && e->pchar)     // I see dead people (not)
                    {
                        e->pchar->bRecalcStrengths = true;
                    }
                }
            }
            xcase SCMD_SHOW_POWERDIMINISH:
            {
                if (server_state.enablePowerDiminishing)
                    conPrintf(client, "Power Diminishing On\n");
                else
                    conPrintf(client, "Power Diminishing Off\n");
            }
            xcase SCMD_SHOW_POWER_TARGET_INFO:
            {
                csrPowersDebug(client, tmp_str, tmp_str2, tmp_str3);
            }
            xcase SCMD_POWERS_BUY_POWER:
            {
                csrPowersBuy(client, tmp_str, tmp_str2, tmp_str3);
            }
            xcase SCMD_POWERS_CHECK:
            {
                csrPowersCheck(client);
            }
            xcase SCMD_SGRP_BADGE_SHOW:
            {
                if( e && e->supergroup )
                {
                    int i;
                    Supergroup *sg = e->supergroup;

                    conPrintf(client, "badges owned:\n");
                    for(i = 0; i < BADGE_SG_MAX_BADGES; ++i)
                    {
                        if(sgrpbadges_IsOwned(&sg->badgeStates, i))
                        {
                            BadgeDef *def;
                            if( stashIntFindPointer(g_SgroupBadges.hashByIdx, i, &def) )
                            {
                                conPrintf(client,"%d: %s\n", i, def->pchName);
                            }
                            else
                            {
                                conPrintf(client,"%d: <has no name>\n", i );
                            }
                        }
                    }
                }
                else
                {
                    conPrintf(client, "can't reward. either no ent or no supergroup.");
                }
            }
            xcase SCMD_BADGE_SHOW:
            {
                csrBadgesShow(client, 0);
            }
            xcase SCMD_BADGE_SHOW_ALL:
            {
                csrBadgesShow(client, 1);
            }
            xcase SCMD_BADGE_STAT_SHOW:
            {
                char *restrict = NULL;
                strdup_alloca(restrict, tmp_str);

                csrBadgeStatsShow(client, restrict);
            }
            xcase SCMD_BADGE_GRANT:
            {
                if(e)
                {
                    if(badge_Award(e, tmp_str, true))
                    {
                        conPrintf(client, "Badge '%s' awarded to player '%s'.\n", tmp_str, e->name);
                        LOG_ENT(e, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, "CSR:BadgeGrant %s", tmp_str);
                    }
                    else
                    {
                        conPrintf(client, "There is no badge named '%s'.\n", tmp_str);
                    }
                }
            }
            xcase SCMD_BADGE_GRANT_BITS:
            {
                if(e)
                {
                    char *bits;
                    
                    // verify
                    bits = tmp_str;
                    
                    for( bits = tmp_str; bits && *bits ; ++bits )
                    {
                        if( !INRANGE(*bits,'0',('9'+1)) && !INRANGE(*bits, 'A',('F'+1)) && !INRANGE(*bits, 'a',('f'+1)))
                        {
                            conPrintf(client, "invalid character %c in string %s", *bits, tmp_str);
                            break;
                        }
                    }
                    if( !bits || *bits )
                        break;
                    bits = tmp_str;
                    

                    // set the new ones
                    {
                        char *oldbits,*newbits;
                        strdup_alloca(oldbits,getBadgeOwnedBits(e));
                        conPrintf(client, "old bits: %s", oldbits);
                        badge_UnpackOwned(e, bits);
                        newbits = getBadgeOwnedBits(e);
                        if( 0 != stricmp(oldbits,newbits) )
                        {
                            conPrintf(client, "success. new bits: %s", newbits);
                            badge_ClearUpdatedAll(e);
                        }
                        else
                        {
                            conPrintf(client, "failed. check input string");
                        }
                    }
                }
            }
            xcase SCMD_BADGE_SHOW_BITS:
            {
                conPrintf(client,"bits: %s", getBadgeOwnedBits(e));
            }
            xcase SCMD_BADGE_GRANT_ALL:
            {
                if(e)
                    csrBadgeGrantAll(e);
            }
            xcase SCMD_BADGE_REVOKE_ALL:
            {
                if (e)
                    csrBadgeRevokeAll(e);
            }
            xcase SCMD_BADGE_REVOKE:
            {
                if(e)
                {
                    if(badge_Revoke(e, tmp_str))
                    {
                        conPrintf(client, "Badge '%s' revoked from player '%s'.\n", tmp_str, e->name);
                        dbLog("CSR:BadgeRevoke", e, "%s", tmp_str);
                    }
                    else
                    {
                        conPrintf(client, "There is no badge named '%s'.\n", tmp_str);
                    }
                }
            }
            xcase SCMD_BADGE_STAT_SET:
            {
                int statValue = tmp_int;

                if(e)
                {
                    if(badge_StatSet(e, tmp_str, statValue))
                    {
                        conPrintf(client, "Set stat '%s' to %d on player '%s'.\n", tmp_str, statValue, e->name);
                        dbLog("CSR:BadgeStatSet", e, "%s set to %d. Now %d", tmp_str, statValue, badge_StatGet(e, tmp_str));
                    }
                    else
                    {
                        conPrintf(client, "There is no badge stat named '%s'.\n", tmp_str);
                    }
                }
            }
            xcase SCMD_BADGE_STAT_ADD_RELAY:
            {
                int res = 0;
                int db_id = tmp_int2;
                int statValue = tmp_int;
                Entity *e = findEntOrRelayById(admin,db_id,str, &res);

                if(e)
                {
                    if(badge_StatAdd(e, tmp_str, statValue))
                    {
                        conPrintf(client, "Added %d to stat '%s' on player '%s'.\n", statValue, tmp_str, e->name);
                        dbLog("CSR:BadgeStatAdd", e, "Added %d to %s. Now %d", statValue, tmp_str, badge_StatGet(e, tmp_str));
                    }
                    else
                    {
                        conPrintf(client, "There is no badge stat named '%s'.\n", tmp_str);
                    }
                }
            }
            xcase SCMD_BADGE_STAT_ADD:
            {
                int statValue = tmp_int;
                if(e)
                {
                    if(badge_StatAdd(e, tmp_str, statValue))
                    {
                        conPrintf(client, "Added %d to stat '%s' on player '%s'.\n", statValue, tmp_str, e->name);
                        dbLog("CSR:BadgeStatAdd", e, "Added %d to %s. Now %d", statValue, tmp_str, badge_StatGet(e, tmp_str));
                    }
                    else
                    {
                        conPrintf(client, "There is no badge stat named '%s'.\n", tmp_str);
                    }
                }
            }
            xcase SCMD_BADGE_BUTTON_USE:
            {
                if(e)
                {
                    badge_ButtonUse(e,tmp_int);
                }
            }
            xcase SCMD_ARCHITECT_TOKEN:
            {
                playerCreatedStoryArc_BuyUpgrade(e, tmp_str, 0, 0);
            }
            xcase SCMD_ARCHITECT_SLOT:
            {
                playerCreatedStoryArc_BuyUpgrade(e, 0, tmp_int, 0);
            }
            xcase SCMD_ARCHITECT_INVISIBLE:
            {
                if( g_ArchitectTaskForce && g_ArchitectTaskForce->architect_flags&ARCHITECT_TEST )
                {
                    g_ArchitectCheated = 1;
                    e->untargetable = !e->untargetable;
                    conPrintf(client, localizedPrintf(e,e->untargetable ? "YouAreUnTargetable" : "YouAreTargetable" ));
                }
            }
            xcase SCMD_ARCHITECT_INVINCIBLE:
            {
                if( g_ArchitectTaskForce && g_ArchitectTaskForce->architect_flags&ARCHITECT_TEST )
                {
                    g_ArchitectCheated = 1;
                    e->invincible = !e->invincible;
                    e->unstoppable = e->invincible;
                    conPrintf(client, clientPrintf(client,"YouAreInvincible", e->invincible ? "NowString" : "NoLonger"));
                }
            }
            xcase SCMD_ARCHITECT_COMPLETEMISSION:
            {
                if( e && e->taskforce && e->taskforce->architect_flags&ARCHITECT_TEST )
                {
                    g_ArchitectCheated = 1;
                    TaskDebugComplete(e, 0, 1);
                }    
            }
            xcase SCMD_ARCHITECT_NEXTOBJECTIVE:
            {
                if( g_ArchitectTaskForce && g_ArchitectTaskForce->architect_flags&ARCHITECT_TEST )
                {
                    g_ArchitectCheated = 1;
                    gotoNextObjective(client);
                }    
            }
            xcase SCMD_ARCHITECT_NEXTCRITTER:
            {
                if( g_ArchitectTaskForce && g_ArchitectTaskForce->architect_flags&ARCHITECT_TEST )
                {
                    g_ArchitectCheated = 1;
                    gotoNextEntity(ENTTYPE_CRITTER, client, 0);
                }    
            }
            xcase SCMD_ARCHITECT_KILLTARGET:
            {
                if( g_ArchitectTaskForce && g_ArchitectTaskForce->architect_flags&ARCHITECT_TEST )
                {
                    Entity *target = validEntFromId(client->controls.selected_ent_server_index);
                    if(!target || !target->pchar)
                        return;
                    g_ArchitectCheated = 1;
                    target->pchar->attrCur.fHitPoints = 0;
                }    
            }
            xcase SCMD_ARCHITECT_LOGINUPDATE:
            {
                if( e->mission_inventory && e->mission_inventory->tickets )
                {
                    conPrintf(client, localizedPrintf(e, "ArchitectClaimableTickets", e->mission_inventory->tickets));
                }
            }
            xcase SCMD_POWERS_INFO:
            {
                csrPowerInfo(client);
            }
            xcase SCMD_POWERS_INFO_BUILD:
            {
                csrPowerInfoBuild(client, tmp_int);
            }
            xcase SCMD_POWERS_SET_LEVEL:
            {
                csrPowerChangeLevelBought(client, tmp_str, tmp_str2, tmp_str3, tmp_int);
            }
            xcase SCMD_SHARDCOMM_FRIEND:
                handleGenericShardCommCmd(client, e, "friend", player_name);
            xcase SCMD_SHARDCOMM_UNFRIEND:
                handleGenericShardCommCmd(client, e, "unfriend", player_name);
            xcase SCMD_SHARDCOMM_CHAN_INVITE:
            {
                char cmd[2000];
                 sprintf(cmd, "invite \"%s\" %d", escapeString(tmp_str), kChatId_Handle);
                handleGenericShardCommCmd(client, e, cmd, player_name);
            }
            xcase SCMD_SHARDCOMM_CHAN_INVITE_SG:
            {
                if(e)
                {
                    if(!chatServerRunning())
                    {
                        chatSendToPlayer(e->db_id,localizedPrintf(e,"NotConnectedToChatServer"), INFO_USER_ERROR, 0);
                        return;
                    }
                    else if(tmp_int < 0 || tmp_int > 4)
                    {
                        chatSendToPlayer(e->db_id, localizedPrintf(e,"BadSupergroupChannelInviteRank"), INFO_USER_ERROR, 0 );
                        break;
                    }
                    else if(!e->supergroup ||
                        !sgroup_hasPermission(e, SG_PERM_ALLIANCE))
                    {
                        chatSendToPlayer(e->db_id, localizedPrintf(e,"CommandRequiresSupergroupLeader"), INFO_USER_ERROR, 0 );
                        break;
                    }

                    dbReqSgChannelInvite(e, tmp_str, tmp_int);
                }
            }
            xcase SCMD_SHARDCOMM_GMOTD:
                handleGenericShardCommCmd(client, e, "GlobalMotd", NULL);
            xcase SCMD_SHARDCOMM_GETGLOBAL:
            {
                int auth = authIdFromName(tmp_str);
                if( auth )
                    handleGenericAuthShardCommCmd(client, e, "GetGlobal", auth );
                else
                    chatSendToPlayer(e->db_id, localizedPrintf(e, "InvalidUser", tmp_str ), INFO_USER_ERROR, 0 );
            }
            xcase SCMD_SHARDCOMM_GETGLOBALSILENT:
            {
                int auth = authIdFromName(tmp_str);
                if( auth )
                    handleGenericAuthShardCommCmd(client, e, "GetGlobalSilent", auth );
            }
            xcase SCMD_SHARDCOMM_GMAILCLAIM:
            {

                if (e == NULL || e->pl == NULL)
                    return;
 
                if (OnArenaMap())
                {
                    chatSendToPlayer( e->db_id, localizedPrintf(e,"CantClaimItemsInArena"), INFO_USER_ERROR, 0 );
                    return;
                }

                // check for pending
                if (e->pl->gmail_pending_state != ENT_GMAIL_NONE || 
                    *e->pl->gmail_pending_inventory != 0 || e->pl->gmail_pending_banked_influence > 0)                
                {
                    checkPendingTransactions(e);
                    chatSendToPlayer( e->db_id, localizedPrintf(e,"GmailOutstandingPendingTransaction"), INFO_USER_ERROR, 0 );
                    return;
                }

                // send request to claim
                shardCommSendf(e, 1, "GmailClaimRequest %i", tmp_int);
                e->pl->gmail_pending_state = ENT_GMAIL_SEND_CLAIM_REQUEST;
                e->pl->gmail_pending_mail_id = tmp_int;
                e->pl->gmail_pending_requestTime = timerSecondsSince2000();
                e->auctionPersistFlag = true;

/*                int i;
                int matchFound =0;
                for (i = 0; i < MAX_GMAIL; ++i)
                {
                    GMailClaimState *mail = &e->pl->gmailClaimState[i];
                    if (mail->mail_id == tmp_int)
                    {
                        devassert(mail->claim_state);
                        matchFound = 1;
                        if (mail->claim_state == ENT_GMAIL_CLAIM_WAIT_CLEAR)
                        {
                            //    clear the item from the players view
                            char buff[100];
                            chatSendToPlayer( e->db_id, localizedPrintf(e,"AttachmentAlreadyClaimed"), INFO_USER_ERROR, 0 );
                            sprintf(buff, "GmailClaim %i %i", tmp_int, 0);    //    last parameter is unused
                            START_PACKET(pak, e, SERVER_SHARDCOMM);
                            pktSendString(pak,buff);
                            END_PACKET;

                            //    clear the item from the chatserver
                            shardCommSendf(e, 1, "GmailClear %i %i", tmp_int, 1);        //    1 for success to clear
                        }
                        else
                        {
                            mail->claim_state = ENT_GMAIL_CLAIM_TRYING;
                            shardCommSendf(e, 1, "GmailClaim %i", tmp_int);
                        }
                        break;
                    }
                }
                if (!matchFound)
                {
                    for (i = 0; i < MAX_GMAIL; ++i)
                    {
                        GMailClaimState *mail = &e->pl->gmailClaimState[i];
                        if (!mail->mail_id)
                        {
                            devassert(mail->claim_state == ENT_GMAIL_CLAIM_NONE);
                            matchFound = 1;
                            mail->mail_id = tmp_int;
                            mail->claim_state = ENT_GMAIL_CLAIM_TRYING;
                            shardCommSendf(e, 1, "GmailClaim %i", tmp_int);
                            break;
                        }
                    }
                    if (!matchFound)
                    {
                        chatSendToPlayer( e->db_id, localizedPrintf(e,"GMailTooManyClaims"), INFO_USER_ERROR, 0 );
                    }
                }
*/
            }
            xcase SCMD_SHARDCOMM_GMAILRETURN:
            {
                shardCommSendf(e, 1, "GmailReturn %i", tmp_int);
            }
            xcase SCMD_CHAT_CMD_RELAY:
            {
                int result;
                Entity *e = findEntOrRelay(admin,player_name,str,&result);
                if(e)
                {
                    char buf[2000];
                    char buf2[2000];
                    strcpy(buf2, unescapeString(tmp_str));
                    sprintf(buf, "%s \"%s\"", buf2, escapeString(e->pl->chat_handle));
                    shardCommSendInternal(tmp_int, buf);
                }
            }
            xcase SCMD_SHARDCOMM_CSR_NAME:
            {
                char buf[1000];
                strcpy(buf, escapeString(tmp_str));
                shardCommSendf(e, true, "CsrName \"%s\" \"%s\"", buf, escapeString(tmp_str2));
            }
            xcase SCMD_SHARDCOMM_CSR_KILLCHANNEL:
            {
                 shardCommSendf(e, true, "CsrChanKill \"%s\"", escapeString(tmp_str));
            }
            xcase SCMD_SHARDCOMM_SEND:
             {
                char buf[1000];
                if( OnArenaMap() )
                {
                    if(ArenaMapIsParticipant(e))
                        chatSendToPlayer( e->db_id, localizedPrintf(e,"NotAllowedInArena"), INFO_USER_ERROR, 0 );
                    else
                        chatSendToPlayer( e->db_id, localizedPrintf(e,"ObserverCannotSpeak"), INFO_USER_ERROR, 0 );
                    break;
                }
                if( chatBanned(e,tmp_str)||entIsTrial(e) )
                    break;
                strncpyt(buf, escapeString(tmp_str), sizeof(buf));
                shardCommSendf(e, true, "Send \"%s\" \"%s\"", buf, escapeString(tmp_str2));
            }
            xcase SCMD_SHARDCOMM_CSR_SENDALL:
            {
                shardCommSendf(e, true, "CsrSendAll \"%s\"", escapeString(tmp_str));
            }
            xcase SCMD_SHARDCOMM_CSR_SILENCEALL:
            {
                shardCommSend(e, true, "CsrSilenceAll");
            }
            xcase SCMD_SHARDCOMM_CSR_UNSILENCEALL:
            {
                shardCommSend(e, true, "CsrUnsilenceAll");
            }
            xcase SCMD_SHARDCOMM_CSR_SHUTDOWN:
            {
                shardCommSendf(e, true, "Shutdown \"%s\"", escapeString(tmp_str));
            }
            xcase SCMD_SHARDCOMM_CSR_SILENCE_HANDLE:
            {
                shardCommSendf(e, true, "CsrSilence \"%s\" %d", escapeString(tmp_str), tmp_int);
            }
            xcase SCMD_SHARDCOMM_CSR_RENAMEALL:
            {
                shardCommSend(e, true, "CsrRenameAll");
            }
            xcase SCMD_SHARDCOMM_CSR_GRANTRENAME:
            {
                shardCommSendf(e, true, "CsrGrantRename \"%s\"", escapeString(tmp_str));
            }
            xcase SCMD_SHARDCOMM_CSR_MEMBERSMODE:
            {
                 char buf[1000];
                strcpy(buf, escapeString(tmp_str));
                shardCommSendf(e, true, "CsrMembersMode \"%s\" \"%s\"", buf, escapeString(tmp_str2));
            }
            xcase SCMD_SHARDCOMM_CSR_REMOVEALL:
            {
                shardCommSendf(e, true, "CsrRemoveAll \"%s\"", escapeString(tmp_str));
            }
            xcase SCMD_SHARDCOMM_CSR_CHECKMAILSENT:
            {
                shardCommSendf(e, true, "CsrCheckMailSent \"%s\"", escapeString(player_name));
            }
            xcase SCMD_SHARDCOMM_CSR_CHECKMAILRECV:
            {
                shardCommSendf(e, true, "CsrCheckMailReceived \"%s\"", escapeString(player_name));
            }
            xcase SCMD_SHARDCOMM_CSR_BOUNCEMAILSENT:
            {
                shardCommSendf(e, true, "CsrBounceMailSent \"%s\"  %d", escapeString(player_name), tmp_int);
            }
            xcase SCMD_SHARDCOMM_CSR_BOUNCEMAILRECV:
            {
                shardCommSendf(e, true, "CsrBounceMailReceived \"%s\"  %d", escapeString(player_name), tmp_int);
            }
            xcase SCMD_CSR_GMAILPENDING:
            {
                int result = 0;
                Entity *target = findEntOrRelay(NULL, player_name, str,&result);
                if (target && target->pl)
                {
                    char *estr = NULL;
                    estrConcatf(&estr, "Pending Inf: %i\n", target->pl->gmail_pending_banked_influence);
                    estrConcatf(&estr, "Pending Inventory: %s\n", target->pl->gmail_pending_inventory[0] ? target->pl->gmail_pending_inventory : "none");
                    estrConcatf(&estr, "Pending Attachment: %s\n", target->pl->gmail_pending_attachment[0] ? target->pl->gmail_pending_attachment : "none");
                    conPrintf(admin, estr);
                    estrDestroy(&estr);
                }
            }
            xcase SCMD_GETHANDLE:
            {
                Entity * target;
                int id = dbPlayerIdFromName(player_name);
                target = entFromDbId( id );

                if( id > 0 )
                {
                    if(chatServerRunning())
                    {
                        if(target && target->pl )
                        {
                            chatSendToPlayer(e->db_id, localizedPrintf(e,"CsrPlayerHandle", target->name, target->pl->chat_handle), INFO_DEBUG_INFO, 0);
                        }
                        else
                        {
                            char buf[1000];
                            sprintf( buf, "gethandle_relay %i %i", id, e->db_id );
                            serverParseClient( buf, NULL );
                        }
                    }
                    else
                    {
                        chatSendToPlayer(e->db_id, localizedPrintf(e,"NotConnectedToChatServer"), INFO_USER_ERROR, 0);
                    }
                }
                else
                    conPrintf(client, clientPrintf(client, "playerNotFound", player_name ));
            }
            xcase SCMD_GETHANDLE_RELAY:
            {
                int online;
                Entity *e;

                if (tmp_int <= 0 || tmp_int2 <= 0) break;
                if (relayCommandStart(NULL, tmp_int, str, &e, &online))
                {
                    chatSendToPlayer(tmp_int2, localizedPrintf(e,"CsrPlayerHandle", e->name, e->pl->chat_handle), INFO_DEBUG_INFO, 0);
                    relayCommandEnd(&e, &online);
                }
            }
            xcase SCMD_GRANT_RESPEC:
            {
                if(e)
                {
                    e->pl->respecTokens |= (TOKEN_AQUIRED<<6);
                    conPrintf(client, localizedPrintf(e,"RespecGranted", e->name));
                }
            }
            xcase SCMD_RESPEC_STATUS:
            {
                if(e)
                {
                    int count = 0;
                    int i, counter;

                    // free respec
                    if( e->pl->respecTokens&(TOKEN_AQUIRED<<6) )
                        count++;

                    // counter repecs
                    counter = (e->pl->respecTokens >> 24);
                    count += counter;

                    // trial respecs
                    if(e->pl->respecTokens&(TOKEN_AQUIRED<<0) && !(e->pl->respecTokens&(TOKEN_USED<<0)))
                        count++;
                    if(e->pl->respecTokens&(TOKEN_AQUIRED<<2) && !(e->pl->respecTokens&(TOKEN_USED<<2)))
                        count++;
                    if(e->pl->respecTokens&(TOKEN_AQUIRED<<4) && !(e->pl->respecTokens&(TOKEN_USED<<4)))
                        count++;

                    // patron and vet respecs
                    for (i = 0; i < 4; i++)
                    {
                        if(e->pl->respecTokens&(TOKEN_AQUIRED<<(8+(i*2))) && !(e->pl->respecTokens&(TOKEN_USED<<(8+(i*2)))))
                            count++;
                    }
                    if(count)
                        chatSendToPlayer(e->db_id, localizedPrintf(e,"UnclaimedRespecs", count ), INFO_SVR_COM, 0 );

                }
            }
             xcase SCMD_GRANT_RESPEC_TOKEN:
            {
                int token = tmp_int;
                  if(e && token > 0 && token <= 3)
                {
                    conPrintf(client, localizedPrintf(e,"RespecTokenGranted", e->name, token));
                    token = (token-1)*2;
                    e->pl->respecTokens &= ~(TOKEN_USED<<token);
                    e->pl->respecTokens |= (TOKEN_AQUIRED<<token);
                }
            }
            xcase SCMD_GRANT_COUNTER_RESPEC:
            {
                if(e && tmp_int > 0 && tmp_int <= 255)
                {
                    int counter = (e->pl->respecTokens >> 24);

                    counter += tmp_int;

                    if (counter > 255)
                        counter = 255;

                    counter = counter << 24;

                    // clear out old entries
                    e->pl->respecTokens &= 0x00ffffff;

                    // put new counter in place
                    e->pl->respecTokens |= counter;

                    conPrintf(client, localizedPrintf(e,"RespecGranted", e->name));
                }
            }
            xcase SCMD_GRANT_CAPE:
            {
                 if(e)
                {
                    conPrintf(client, localizedPrintf(e,"CapeGranted", e->name));
                    costume_Award(e,"Back_Regular_Cape", true ,0);
                }
            }
            xcase SCMD_GRANT_GLOW:
            {
                if(e)
                {
                    conPrintf(client, localizedPrintf(e,"GlowGranted", e->name));
                    costume_Award(e,"Auras_Common", true, 0);
//                    e->pl->glowieUnlocked = true;
                    e->send_costume = true;
                }
            }
            xcase SCMD_GRANT_COSTUMEPART:
            {
                 costume_Award( e, tmp_str, true, 0 );
            }
            xcase SCMD_REMOVE_RESPEC:
            {
                if(e)
                {
                    e->pl->respecTokens &= ~(TOKEN_AQUIRED<<6);
                    conPrintf(client, localizedPrintf(e,"RespecRemoved", e->name));
                } 
            }
            xcase SCMD_REMOVE_RESPEC_TOKEN:
            {  
                int token = tmp_int;

                  if(e && token >0 && token <= 7)
                {
                    conPrintf(client, localizedPrintf(e,"RespecTokenRemoved", e->name, token));
                    token = (token-1)*2;
                    e->pl->respecTokens &= ~(TOKEN_AQUIRED<<token);
                    e->pl->respecTokens &= ~(TOKEN_USED<<token);
                }
            }
            xcase SCMD_REMOVE_COUNTER_RESPEC:
            if(e && tmp_int > 0 && tmp_int <= 255)
            {
                int counter = (e->pl->respecTokens >> 24);

                counter -= tmp_int;

                if (counter < 0)
                    counter = 0;

                counter = counter << 24;

                // clear out old entries
                e->pl->respecTokens &= 0x00ffffff;

                // put new counter in place
                e->pl->respecTokens |= counter;

                conPrintf(client, localizedPrintf(e,"RespecRemoved", e->name));
            }
            xcase SCMD_REMOVE_CAPE:
            {
                if(e)
                {
                    costume_Unaward(e,"Back_Regular_Cape", 0);
                    conPrintf(client, localizedPrintf(e,"CapeRemoved", e->name));
                }
            }
            xcase SCMD_REMOVE_GLOW:
            {
                if(e)
                {
                    costume_Unaward(e,"Auras_Common", 0);
                    conPrintf(client, localizedPrintf(e,"GlowRemoved", e->name));
                }
            }
            xcase SCMD_REMOVE_COSTUMEPART:
            {
                costume_Unaward( e, tmp_str, 0 );
            }
            xcase SCMD_NEW_SCENE:
            {
                if( server_state.newScene )
                    free(server_state.newScene);
                server_state.newScene = malloc(strlen(tmp_str)+1);
                strcpy(server_state.newScene, tmp_str);
            }
            xcase SCMD_RESET_COSTUME:
            {
                if( e )
                {
                    if( tmp_int > MAX_COSTUMES || tmp_int < 0 || tmp_int >= costume_get_num_slots(e) )
                    {
                        conPrintf(client, localizedPrintf(e,"DidNotResetCostumeInvalidNumber", e->name));
                    }
                    else
                    {
                        costume_setDefault(e, tmp_int);
                        chatSendToPlayer( e->db_id, localizedPrintf(e,"YourCostumeHasBeenReset"), INFO_ADMIN_COM, 0 );
                        conPrintf(client, localizedPrintf(e,"YouResetCostume", tmp_int, e->name));
                    }
                }
            }
            xcase SCMD_ADD_TAILOR:
            {
                if( e )
                {
                    e->pl->freeTailorSessions++;
                    chatSendToPlayer( e->db_id, localizedPrintf(e,"YouHaveFreeTailor", 1), INFO_ADMIN_COM, 0 );
                    conPrintf(client, localizedPrintf(e,"YouGavePlayerFreeTailor", e->name, 1));
                }
            }
            xcase SCMD_ADD_TAILOR_PER_SLOT:
            {
                if( e )
                {
                    int slots = costume_get_num_slots(e);
                    e->pl->freeTailorSessions += slots;
                    chatSendToPlayer( e->db_id, localizedPrintf(e,"YouHaveFreeTailor", slots), INFO_ADMIN_COM, 0 );
                    conPrintf(client, localizedPrintf(e,"YouGavePlayerFreeTailor", e->name, slots));
                }
            }
            xcase SCMD_FREE_TAILOR:
            {
                if( e )
                {
                    if( tmp_int >= 0 )
                    {
                        e->pl->freeTailorSessions = tmp_int;
                        chatSendToPlayer( e->db_id, localizedPrintf(e,"YouHaveFreeTailor", tmp_int), INFO_ADMIN_COM, 0 );
                        conPrintf(client, localizedPrintf(e,"YouGavePlayerFreeTailor", e->name, tmp_int));
                    }
                    else
                        conPrintf( client, localizedPrintf(e,"NoNegativeFreeTailor") );
                }
            }
            xcase SCMD_ULTRATAILOR_GRANT:
            {
                if( e )
                {
                    e->pl->ultraTailor = true;
                    e->pl->freeTailorSessions = costume_get_num_slots(e);

                    chatSendToPlayer( e->db_id, localizedPrintf(e,"YouHaveFreeUltraTailor", tmp_int), INFO_ADMIN_COM, 0 );
                    conPrintf(client, localizedPrintf(e,"YouGavePlayerFreeUltraTailor", e->name, tmp_int));

                    chatSendToPlayer( e->db_id, localizedPrintf(e,"YouHaveFreeTailor", e->pl->freeTailorSessions), INFO_ADMIN_COM, 0 );
                    conPrintf(client, localizedPrintf(e,"YouGavePlayerFreeTailor", e->name, e->pl->freeTailorSessions));
                }
            }
            xcase SCMD_FREE_TAILOR_STATUS:
            {
                if(e)
                {
                    if(e->pl->freeTailorSessions)
                        chatSendToPlayer(e->db_id, localizedPrintf(e,"UnclaimedFreeTailor", e->pl->freeTailorSessions ), INFO_SVR_COM, 0 );
                    else 
                        chatSendToPlayer(e->db_id, localizedPrintf(e,"NoFreeTailor" ), INFO_SVR_COM, 0 );
                }
            }
            xcase SCMD_CSR_CRITTER_LIST:
                csrCritterList( client );
            xcase SCMD_CSR_GOTO_CRITTER_ID:
                csrNextCritterById( client, tmp_int );
            xcase SCMD_CSR_GOTO_CRITTER_NAME:
                csrNextCritterByName( client, tmp_str );
            xcase SCMD_CSR_AUTHKICK:
                csrAuthKick( client, player_name );
            xcase SCMD_CSR_NEXT_ARCHVILLAIN:
                csrNextArchVillain( client );
            xcase SCMD_LISTEN:
                csrListen( client, player_name );
            xcase SCMD_ATTRIBUTETARGET:
                csrViewAttributesTarget( client->entity );
            xcase SCMD_PETATTRIBUTETARGET:
                petViewAttributes( client->entity, tmp_int );
            xcase SCMD_CLEARATTRIBUTETARGET:
                clearViewAttributes( client->entity );
            xcase SCMD_AUTHUSERDATA_SET:
            {
                U32 auOld[AUTH_DWORDS];

                if(!e || !e->pl)
                    break;

                memcpy(auOld, e->pl->auth_user_data, sizeof(auOld));
                if(conSetAuthUserData(client, tmp_str, tmp_int))
                {
                    if( character_IsInitialized(e->pchar) )
                        costumeAwardAuthParts( e );

                    MarkModifiedBadges(g_hashBadgeStatUsage, e->pl->aiBadges, "*auth");
                    dbLog("CSR:AuthUserData", e, "Set %s to %d  (was:%08x %08x %08x %08x, now:%08x %08x %08x %08x)",
                        tmp_str, tmp_int,
                        auOld[0], auOld[1], auOld[2], auOld[3],
                        e->pl->auth_user_data[0], e->pl->auth_user_data[1], e->pl->auth_user_data[2], e->pl->auth_user_data[3]);
                }
            }
            xcase SCMD_AUTHUSERDATA_SHOW:
                conShowAuthUserData(client);
            xcase SCMD_RESPEC:
                {
                    Entity *e = client->entity;

                    if( e->pl->taskforce_mode )
                    {
                        sendChatMsg( e, clientPrintf(client,"NoRespecInTFMode"), INFO_USER_ERROR, 0 );
                    }
                    // Always allow devs to use /respec - if you're high enough access level, just unilaterally turn on the bit.
                    else if (e->pl->respecTokens & (TOKEN_AQUIRED << 6) || (e->access_level >= 9 && (e->pl->respecTokens |= (TOKEN_AQUIRED << 6))))
                    {
                        e->pl->isRespeccing = 1;
                        START_PACKET( pak, e, SERVER_RESPEC_OPEN );
                        END_PACKET
                    }
                    else
                        sendChatMsg( e, clientPrintf(client,"NoFreeRespec"), INFO_USER_ERROR, 0 );
                }
            xcase SCMD_CSR_DESCRIPTION_SET:
                csrEditDescription( client->entity, tmp_str );

            xcase SCMD_DIFFICULTY_SET_LEVEL:
                difficultySetForPlayer(client->entity, tmp_int, 0, -1, -1);
            xcase SCMD_DIFFICULTY_SET_TEAMSIZE:
                difficultySetForPlayer(client->entity, MIN_LEVEL_ADJUST-1, tmp_int, -1, -1);
            xcase SCMD_DIFFICULTY_SET_AV:
                difficultySetForPlayer(client->entity, MIN_LEVEL_ADJUST-1, 0, tmp_int, -1);
            xcase SCMD_DIFFICULTY_SET_BOSS:
                difficultySetForPlayer(client->entity, MIN_LEVEL_ADJUST-1, 0, -1, tmp_int);
            xcase SCMD_GET_RATED_ARENA_STATS:
                ArenaRequestUpdatePlayer(client->entity->db_id, client->entity->db_id, 0);
            xcase SCMD_GET_ALL_ARENA_STATS:
                ArenaRequestUpdatePlayer(client->entity->db_id, client->entity->db_id, 2);
            xcase SCMD_DEBUG_EXPR:
                {
                    extern EvalContext *s_pCombatEval;
                    eval_SetDebug(s_pCombatEval, tmp_int);
                }
            xcase SCMD_SERVER_TIME:
                chatSendToPlayer(client->entity->db_id, clientPrintf(client, "ServerTime", timerMakeOffsetDateStringNoYearNoSecFromSecondsSince2000(dbSecondsSince2000())), INFO_SVR_COM, 0 );
            xcase SCMD_CITY_TIME:
				chatSendToPlayer(client->entity->db_id, clientPrintf(client, "NPCCityTime", (int)server_visible_state.time, (int)(fmod(server_visible_state.time,1) * 60)), INFO_SVR_COM, 0 );                
            xcase SCMD_FILL_POWERUP_SLOT:
            {
                Character *p;
                if(!e)
                    break;
                p = e->pchar;


                if(p)
                {
                    if( !INRANGE0(tmp_int, CHAR_BOOST_MAX))
                    {
                        conPrintf(client,"invalid boost id %d", tmp_int);
                    }
                    else if( !p->aBoosts[tmp_int] )
                    {
                        conPrintf(client,"No boost at %d", tmp_int);
                    }
                    else if( !AINRANGE( tmp_int2, p->aBoosts[tmp_int]->aPowerupSlots) )
                    {
                        conPrintf(client,"invalid powerup slot %d", tmp_int2);
                    }
                    else if( !p->aBoosts[tmp_int]->aPowerupSlots[tmp_int2].reward.name )
                    {
                        conPrintf(client,"boost doesn't have a powerupslot at %d", tmp_int2);
                    }
                    else
                    {
                        p->aBoosts[tmp_int]->aPowerupSlots[tmp_int2].filled = true;
                        conPrintf(client,"slot filled.");
                    }
                }
            }
            xcase SCMD_BASE_SAVE:
            {
                if (playerCanModifyBase(e,&g_base))
                {
                    sgroup_SaveBase(&g_base, NULL, 1);
                }
            }
            xcase SCMD_BASE_DESTROY:
                {
                    if( e )
                        csrDestroyBase( 0, e->supergroup_id );
                }
            xcase SCMD_TELEPORT_TO_BASE:
                {
                    if (!e)
                        break;

                    if( e && e->supergroup )
                    {
                        const BaseUpkeepPeriod *period = sgroup_GetUpkeepPeriod(e->supergroup);
                        bool canEnter = period ? !period->denyBaseEntry : true;
                        if( !canEnter )
                        {
                            int rentDue = e->supergroup ? e->supergroup->upkeep.prtgRentDue : 0;
                            chatSendToPlayer(e->db_id,localizedPrintf(e,"DoorLockedRentDue", rentDue),INFO_SVR_COM,0);
                            break;
                        }
                    }

                    if (!sgroup_CanPortToBase(e)) {
                        chatSendToPlayer(e->db_id,localizedPrintf(e,"CantTeleportToBase"),INFO_USER_ERROR,0);
                        break;
                    }
                    tmp_str[0] = 0;
                    SGBaseEnter(client->entity, "TeleporterSpawn", tmp_str);
                    if (tmp_str[0])
                        chatSendToPlayer(e->db_id, tmp_str,INFO_SVR_COM,0);
                }

            xcase SCMD_ARENA_MAKEPETS:
                ArenaCreateArenaPets( e );
#if NOVODEX
            xcase SCMD_NOVODEX_INIT_WORLD:
                {
                    nwInitializeNovodex();
                }
            xcase SCMD_NOVODEX_DEINIT_WORLD:
                {
                    nwDeinitializeNovodex();
                }
#endif // NOVODEX
             xcase SCMD_REWARD_TOKEN:
             {
                 if( e && rewardIsPermanentRewardToken( tmp_str ))
                 {
                     RewardToken *token = setRewardToken(e, tmp_str, tmp_int);

                     if( token == NULL)
                     {
                         conPrintf(client, "unable to give '%s' as rewardtoken.",tmp_str);
                     }
                 }
                 else
                 {
                     conPrintf(client, "token '%s' is not a reward token.", tmp_str);
                 }
             }
             xcase SCMD_REWARD_TOKEN_TO_PLAYER:
             {
                 // Don't have any console output, this is used by standard player-facing rewards
                 int online;
                 Entity *e;

                 if (relayCommandStart(NULL, tmp_int, str, &e, &online))
                 {
                     giveRewardToken(e, tmp_str);
                     relayCommandEnd(&e, &online);
                 }
             }
             xcase SCMD_REMOVE_TOKEN:
             {
                 if( e ) //&& rewardIsPermanentRewardToken( tmp_str ))
                 {
                     removeRewardToken(e,tmp_str);
                 }
                 else
                 {
                     conPrintf(client, "token '%s' is not a reward token.", tmp_str);
                 }
             }
             xcase SCMD_REMOVE_TOKEN_FROM_PLAYER:
             {
                 // Don't have any console output, this is used by standard player-facing rewards
                 int online;
                 Entity *e;

                 if (relayCommandStart(NULL, tmp_int, str, &e, &online))
                 {
                     removeRewardToken(e, tmp_str);
                     relayCommandEnd(&e, &online);
                 }
             }
             xcase SCMD_LIST_TOKENS:
             {
                 int i;

                 conPrintf(client, "reward tokens:");
                 for( i = eaSize( &e->pl->rewardTokens ) - 1; i >= 0; --i)
                 {
                     char str[128];
                     if( e->pl->rewardTokens[i] )
                     {
                         RewardToken *tok = e->pl->rewardTokens[i];
                         timerMakeDateStringFromSecondsSince2000(str, tok->timer );
                         conPrintf(client, "\t\t%s:%d:(%s)",tok->reward, tok->val, str);
                     }
                     else
                     {
                         conPrintf(client, "\t\t NULL reward at %d",i);
                     }
                 }
                 conPrintf(client, "active player reward tokens:");
                 for( i = eaSize( &e->pl->activePlayerRewardTokens ) - 1; i >= 0; --i)
                 {
                     char str[128];
                     if( e->pl->activePlayerRewardTokens[i] )
                     {
                         RewardToken *tok = e->pl->activePlayerRewardTokens[i];
                         timerMakeDateStringFromSecondsSince2000(str, tok->timer );
                         conPrintf(client, "\t\t%s:%d:(%s)",tok->reward, tok->val, str);
                     }
                     else
                     {
                         conPrintf(client, "\t\t NULL reward at %d",i);
                     }
                 }
             }
             xcase SCMD_LIST_ACTIVE_PLAYER_TOKENS:
             {
                int i;

                if (e->teamup_activePlayer)
                {
                    conPrintf(client, "active player reward tokens on team:");
                    for( i = eaSize( &e->teamup_activePlayer->activePlayerRewardTokens ) - 1; i >= 0; --i)
                    {
                        char str[128];
                        if( e->teamup_activePlayer->activePlayerRewardTokens[i] )
                        {
                            RewardToken *tok = e->teamup_activePlayer->activePlayerRewardTokens[i];
                            timerMakeDateStringFromSecondsSince2000(str, tok->timer );
                            conPrintf(client, "\t\t%s:%d:(%s)",tok->reward, tok->val, str);
                        }
                        else
                        {
                            conPrintf(client, "\t\t NULL reward at %d",i);
                        }
                    }
                }
                else
                {
                    conPrintf(client, "active player reward tokens on player:");
                    for( i = eaSize( &e->pl->activePlayerRewardTokens ) - 1; i >= 0; --i)
                    {
                        char str[128];
                        if( e->pl->activePlayerRewardTokens[i] )
                        {
                            RewardToken *tok = e->pl->activePlayerRewardTokens[i];
                            timerMakeDateStringFromSecondsSince2000(str, tok->timer );
                            conPrintf(client, "\t\t%s:%d:(%s)",tok->reward, tok->val, str);
                        }
                        else
                        {
                            conPrintf(client, "\t\t NULL reward at %d",i);
                        }
                    }
                }
             }
             xcase SCMD_LIST_PHASES:
             {
                int i;

                conPrintf(client, "Bitfield vision phases on player:");
                for (i = 0; i < eaSize(&g_visionPhaseNames.visionPhases); i++)
                {
                    if (e->bitfieldVisionPhases & (1 << i))
                    {
                        if (i > 0)
                        {
                            conPrintf(client, "Phase #%d (%s)", i + 1, g_visionPhaseNames.visionPhases[i]);
                        }
                        else
                        {
                            conPrintf(client, "Phase #1 (\"Prime\")");
                        }
                    }
                }

                conPrintf(client, "Exclusive vision phase on player: %s", g_exclusiveVisionPhaseNames.visionPhases[e->exclusiveVisionPhase]);
             }
             xcase SCMD_IS_VIP:
             {
                 if (AccountIsVIP(ent_GetProductInventory(e), e->pl->account_inventory.accountStatusFlags))
                 {
                     conPrintf(client, "This account is VIP.");
                 }
                 else
                 {
                     conPrintf(client, "This account is not VIP.");
                 }
             }
             xcase SCMD_MREWARD_TOKEN:
             {
                 MapDataToken *token = NULL;

                 mapGroupLock();
                if ((token = giveMapDataToken(tmp_str)) == NULL)
                {
                    conPrintf(client, "unable to give '%s' as map token.", tmp_str);
                } else {
                    token->val = tmp_int;
                    conPrintf(client, "'%s' given as map token, value %d.", tmp_str, tmp_int);
                }
                mapGroupUpdateUnlock();

             }
             xcase SCMD_MREMOVE_TOKEN:
             {
                 mapGroupLock();
                removeMapDataToken(tmp_str);
                conPrintf(client, "'%s' removed as map token.", tmp_str, tmp_int);
                mapGroupUpdateUnlock();
             }
             xcase SCMD_MLIST_TOKENS:
             {
                MapDataToken *retval = NULL;
                int count, i;

                count = eaSize(&g_mapgroup.data);
                for( i = 0 ; i < count ; i++ )
                {
                    if( g_mapgroup.data[i]->name )
                    {
                        conPrintf(client, "\t\t%s:%d",g_mapgroup.data[i]->name, g_mapgroup.data[i]->val );
                    }
                }
             }
            xcase SCMD_SGRP_REWARD_TOKEN:
             {

                 if( e && e->supergroup )
                 {
                     if( 0 <= entity_AddSupergroupRewardToken( e, tmp_str ) )
                     {
                         conPrintf( client, "rewarded token %s", tmp_str );
                     }
                     else
                     {
                         conPrintf( client, "couldn't reward token %s, may be in inventory already", tmp_str );
                     }
                 }
                 else
                 {
                     conPrintf( client, "entity does not have supergroup." );
                 }
             }
            xcase SCMD_SGRP_REMOVE_TOKEN:
             {
                 if( e && e->supergroup )
                 {
                     if( entity_RemoveSupergroupRewardToken( e, tmp_str ) )
                     {
                         conPrintf( client, "removed token %s", tmp_str );
                     }
                     else
                     {
                         conPrintf( client, "couldn't remove token %s", tmp_str );
                     }
                 }
                 else
                 {
                     conPrintf( client, "entity does not have supergroup." );
                 }
             }
            xcase SCMD_SGRP_LIST_TOKENS:
             {
                  if( e && e->supergroup )
                 {
                     int i;
                     for( i = 0; i < eaSize( &e->supergroup->rewardTokens); ++i )
                     {
                         RewardToken *t = e->supergroup->rewardTokens[i];
                         conPrintf( client, t->reward );
                     }
                 }
                 else
                 {
                     conPrintf(client, "entity does not have supergroup.");
                 }
             }
            xcase SCMD_SGRP_STAT:
            {
                if( e && e->supergroup )
                {
                    if( stashFindPointerReturnPointer( g_BadgeStatsSgroup.idxStatFromName, tmp_str ) )
                    {
                        sgrpstats_StatAdj( e->supergroup_id, tmp_str, tmp_int );
                    }
                    else
                    {
                        conPrintf(client, "unknown stat '%s'", tmp_str );
                    }
                }
            }
            xcase SCMD_SHOW_PETNAMES:
            {
                if(e)
                {
                    int i;
                    conPrintf(client, "%s\n",localizedPrintf(e,"PlayerPetNames"));
                    for (i = 0; i < eaSize(&e->pl->petNames);i++) {
                        conPrintf(client, "%s: %s\n",e->pl->petNames[i]->pchEntityDef,e->pl->petNames[i]->petName);
                    }
                }
            }
            xcase SCMD_CLEAR_PETNAMES:
            {
                if( e)
                {
                    conPrintf(client, "%s\n",localizedPrintf(e,"ClearedPetNames"));
                    eaClearEx(&e->pl->petNames,PetNameDestroy);
                    eaClear(&e->pl->petNames);
                    petResetPetNames(e);
                }
            }
            xcase SCMD_KILL_ALLPETS:
            {
                if( e)
                {
                    character_DestroyAllFollowerPets(e->pchar);
                }
            }
            xcase SCMD_SGRP_BADGESTATES:
            {

                int res = 0;
                int db_id = tmp_int2;
                Entity *e = findEntOrRelayById(admin,db_id,str, &res);

                if (!res) // We sychonously got a res back
                {
                    dbLog("CSR:SgrpBadgeStates", e, "relay of cmd '%s' failed", str);
                }
                else if( e && e->supergroup_id )
                {
                    // @todo -AB: move to SgrpServer.c when Raoul gets his stuff checked in :08/16/05
                    int n = tmp_int;
                    char *idstatpairs = tmp_str;
                    int i;
                    U32 *eaiStates = sgrpbadge_eaiStatesFromId( e->db_id );

                    // --------------------
                    // read

                    for( i = 0; i < n && idstatpairs; ++i )
                    {
                        int idx;
                        int val;
                        sscanf(idstatpairs, "%d,%d", &idx, &val);

                        if(INRANGE0(idx, BADGE_SG_MAX_BADGES))
                        {
                            eaiStates[idx] = val;
                        }

                        idstatpairs = strchr( idstatpairs, ';' );
                        if( idstatpairs )
                        {
                            idstatpairs++;
                        }
                    }
                }
            }
            xcase SCMD_SERVEFLOATER:
            {
                if( e )
                {
                    char *msg = tmp_str;
                    F32 delay = tmp_float;
                    EFloaterStyle style = tmp_int;

                    if( !INRANGE0( style,  kFloaterStyle_Count ))
                    {
                        conPrintf( client, "invalid style %d", style );
                    }
                    else if( msg )
                    {
                        serveFloater( e, e, msg, delay, style, 0);
                    }
                }
            }
            xcase SCMD_SGRPBADGEAWARD_NOTIFY_RELAY:
            {
                int res = 0;
                int db_id = tmp_int;
                Entity *e = findEntOrRelayById(admin,db_id,str, &res);

                if (!res) // We sychonously got a res back
                {
                    conPrintf(client,"relay failed.");
                }
                else if( e && e->supergroup )
                {
                    char *nameBadge = tmp_str;
                    const BadgeDef *pdef = stashFindPointerReturnPointerConst( g_SgroupBadges.hashByName, nameBadge );

                    if( pdef )
                    {
                        sendInfoBox(e, INFO_REWARD, "SgrpBadgeYouReceived", e->supergroup->name, printLocalizedEnt(pdef->pchDisplayTitle[ENT_IS_VILLAIN(e)?1:0], e));
                        serveFloater(e, e, "FloatEarnedBadge", 0.0f, kFloaterStyle_Attention, 0);
                    }
                    else
                    {
                        conPrintf(client, "couldn't find sgrp badge named '%s'", nameBadge);
                    }
                }
            }
            xcase SCMD_SALVAGE_NAMES:
            {
                int i;
                int n = eaSize(&g_SalvageDict.ppSalvageItems);
                conPrintf(client, clientPrintf(client,"SalvageNameDesc"));
                for( i = 0; i<n; ++i )
                {
                    SalvageItem const*s =  g_SalvageDict.ppSalvageItems[i];
                    if(s)
                    {

                        if (s->maxInvAmount)
                            conPrintf(client, "%s %s %d",s->pchName,clientPrintf(client,s->ui.pchDisplayName),s->maxInvAmount);
                        else
                            conPrintf(client, "%s %s",s->pchName,clientPrintf(client,s->ui.pchDisplayName));

                    }
                }
            }
            xcase SCMD_SALVAGE_LIST:
            {
                SalvageInventoryItem **invs;
                int numItems,i;
                if( !e)
                    break;
                invs = e->pchar->salvageInv;
                numItems = eaSize(&invs);
                conPrintf(client, localizedPrintf(e,"SalvageListDesc",e->name));
                for( i = 0; i < numItems ; ++i )
                {
                    if( invs[i] && invs[i]->amount > 0 && invs[i]->salvage )
                    {
                        conPrintf(client, "%d %s", invs[i]->amount, invs[i]->salvage ? invs[i]->salvage->pchName : "[NULL SALVAGE]" );
                    }
                }
            }
            xcase SCMD_SALVAGE_GRANT:
            {
                const SalvageItem *si = NULL;
                int count = tmp_int;
                char *pSalvageName = NULL;

                if(!e || strlen(tmp_str) == 0)
                    break;

                if (count <= 0)
                    count = 1;

                pSalvageName = strdup(tmp_str);
                si = salvage_GetItem( pSalvageName );
                if( si )
                {
                    conPrintf(client, localizedPrintf(e,"YouGaveSalvage",e->name, pSalvageName, count));
                    character_AdjustSalvage( e->pchar, si, count, "scmd", false );
                }
                else
                {
                    conPrintf(client, localizedPrintf(e,"SalvageNotFound", pSalvageName));
                }
                SAFE_FREE(pSalvageName);
            }
            xcase SCMD_SALVAGE_REVOKE:
            {
                const SalvageItem *si = NULL;
                int count = tmp_int;
                char *pSalvageName = NULL;

                if(!e || strlen(tmp_str) == 0)
                    break;
                if (count <= 0)
                    count = 1;

                pSalvageName = strdup(tmp_str);
                si = salvage_GetItem( tmp_str );
                if( si )
                {
                    conPrintf(client, localizedPrintf(e,"YouTookSalvage",e->name, pSalvageName,count));
                    character_AdjustSalvage( e->pchar, si, count * -1, "scmd", false );
                }
                else
                {
                    conPrintf(client, localizedPrintf(e,"SalvageNotFound",pSalvageName));
                }
                SAFE_FREE(pSalvageName);

            }
            xcase SCMD_DETAIL_CAT_NAMES:
            {
                int i;
                int n = eaSize(&g_DetailCategoryDict.ppCats);
                conPrintf(client, clientPrintf(client,"DetailCategoryDesc"));
                for( i = 0; i<n; ++i )
                {
                    DetailCategory const*s =  g_DetailCategoryDict.ppCats[i];
                    if(s)
                    {
                        conPrintf(client, "%s %s", s->pchName,clientPrintf(client,s->pchDisplayName));
                    }
                }
            }
            xcase SCMD_DETAIL_NAMES:
            {
                int i;
                int n = eaSize(&g_DetailDict.ppDetails);
                conPrintf(client, clientPrintf(client,"DetailNameDesc"),tmp_str);
                for( i = 0; i<n; ++i )
                {
                    Detail const*s = g_DetailDict.ppDetails[i];
                    if(s && s->pchCategory && stricmp(s->pchCategory,tmp_str) == 0)
                    {
                        conPrintf(client, "%s %s", s->pchName,localizedPrintf(client->entity,s->pchDisplayName));
                    }
                }
            }
            xcase SCMD_DETAIL_LIST:
            {
                DetailInventoryItem **invs;
                int numItems,i;
                if( !e)
                    break;
                invs = e->pchar->detailInv;
                numItems = eaSize(&invs);
                conPrintf(client, clientPrintf(client,"DetailListDesc",e->name));
                for( i = 0; i < numItems ; ++i )
                {
                    if( invs[i] && invs[i]->amount > 0 )
                    {
                        conPrintf(client, "%s %d", invs[i]->item->pchName, invs[i]->amount);
                    }
                }
            }
            xcase SCMD_DETAIL_GRANT:
            {
                const Detail *si = NULL;
                if(!e)
                    break;

                si  = detail_GetItem( tmp_str );
                if( si )
                {
                    conPrintf(client, clientPrintf(client,"YouGaveDetail",e->name, tmp_str));
                    character_AdjustInventory(e->pchar, kInventoryType_BaseDetail, si->id, 1, "scmd", false );
                }
                else
                {
                    conPrintf(client, clientPrintf(client,"DetailNotFound",tmp_str));
                }
            }
            xcase SCMD_DETAIL_REVOKE:
            {
                const Detail *si = NULL;
                if(!e)
                    break;

                si  = detail_GetItem( tmp_str );
                if( si )
                {
                    conPrintf(client, clientPrintf(client,"YouTookDetail",e->name, tmp_str));
                    character_AdjustInventory(e->pchar, kInventoryType_BaseDetail, si->id, -1, "scmd", false );
                }
                else
                {
                    conPrintf(client, clientPrintf(client,"DetailNotFound",tmp_str));
                }
            }
            xcase SCMD_SG_DETAIL_NAMES:
            {
                int i;
                int n = eaSize(&g_DetailDict.ppDetails);
                conPrintf(client, clientPrintf(client,"SGDetailNameDesc"));
                for( i = 0; i<n; ++i )
                {
                    Detail const*s =  g_DetailDict.ppDetails[i];
                    if(s && s->bSpecial)
                    {
                        conPrintf(client, "%s %s", s->pchName,clientPrintf(client,s->pchDisplayName));
                    }
                }
            }
            xcase SCMD_SG_DETAIL_LIST:
            {
                SpecialDetail **invs;
                int numItems,i;
                if( !e)
                    break;
                if (!e->supergroup)
                {
                    conPrintf(client, clientPrintf(client,"PlayerNoSupergroup",e->name));
                    break;
                }
                invs = e->supergroup->specialDetails;
                numItems = eaSize(&invs);
                conPrintf(client, clientPrintf(client,"SGDetailListDesc",e->supergroup->name));
                for( i = 0; i < numItems ; ++i )
                {
                    if( invs[i] && invs[i]->pDetail )
                    {
                        conPrintf(client, "%s %d", invs[i]->pDetail->pchName,invs[i]->creation_time);
                    }
                }
            }
            xcase SCMD_SG_DETAIL_GRANT:
            {
                const Detail *pDetail;
                if(!e)
                    break;
                if (!e->supergroup)
                {
                    conPrintf(client, clientPrintf(client,"PlayerNoSupergroup",e->name));
                    break;
                }
                pDetail = basedata_GetDetailByName(tmp_str);
                if(pDetail && pDetail->bSpecial)
                {
                    conPrintf(client, clientPrintf(client,"YouGaveSGDetail",e->supergroup->name, tmp_str));
                    sgroup_AddSpecialDetail(e, pDetail, dbSecondsSince2000(), 0, DETAIL_FLAGS_AUTOPLACE);
                }
                else
                {
                    conPrintf(client, clientPrintf(client,"DetailNotFound",tmp_str));
                }
            }
            xcase SCMD_SG_DETAIL_REVOKE:
            {
                const Detail *pDetail;
                int id = tmp_int;

                if(!e)
                    break;
                if (!e->supergroup)
                {
                    conPrintf(client, clientPrintf(client,"PlayerNoSupergroup",e->name));
                    break;
                }
                pDetail = basedata_GetDetailByName(tmp_str);
                if(pDetail && pDetail->bSpecial)
                {
                    conPrintf(client, clientPrintf(client,"YouTookSGDetail",e->supergroup->name, tmp_str, id));
                    sgroup_RemoveSpecialDetail(e, pDetail, id);
                }
                else
                {
                    conPrintf(client, clientPrintf(client,"DetailNotFound",tmp_str));
                }
            }
            xcase SCMD_SG_SET_RANK:
            {
                if(e)
                {
                    sgroup_setGeneric(e, SG_PERM_ANYONE, sgroup_genSetRank, &e->db_id, &tmp_int, "SetRank");
                }
            }
            xcase SCMD_SG_UPDATEMEMBER:
            {
                Entity* e = findEntOrRelayById(admin, tmp_int, str, NULL);
                if (e)
                    e->supergroup_update = 1;
            }
            xcase SCMD_SGRP_TASK_COMPLETE:
            {
                Entity* e = findEntOrRelayById(admin, tmp_int, str, NULL);
                if (e)
                {
                    // send update of SG to client
                    e->supergroup_update = 1;

                    // check to see if their team task is the SG task
                    TeamTaskCheckForSGComplete(e);
                }
            }
            ///////////////////// Items of Power ///////////////////////////////////////////////

            xcase SCMD_SG_ITEM_OF_POWER_GRANT: // Updated (7/08)
            {
                if (!ItemOfPowerGrant2( client->entity, tmp_str, tmp_int ))
                    conPrintf(client, "Unable to grant IoP");
            }
            xcase SCMD_SG_ITEM_OF_POWER_REVOKE: // Updated (7/08)
            {
                if (!ItemOfPowerRevoke2( client->entity, tmp_str ))
                    conPrintf(client, "You don't belong to a supergroup");
            }
            xcase SCMD_SG_ITEM_OF_POWER_LIST: // Updated (7/08)
            {
                if (client->entity && client->entity->supergroup)
                {
                    int i, count = eaSize(&client->entity->supergroup->specialDetails);
                    char *time;

                    conPrintf(client, "---------------------------------------------"); 
                    for(i=0; i<count; i++)
                    {
                        SpecialDetail *pSDetail = client->entity->supergroup->specialDetails[i];

                        if (pSDetail->pDetail)
                        {
                            if (pSDetail->expiration_time > 0)
                            {
                                time = timerMakeOffsetDateStringNoYearNoSecFromSecondsSince2000(pSDetail->expiration_time);
                                conPrintf(client, "IoP %s (expires on %s)", pSDetail->pDetail->pchName, time);
                            } else {
                                conPrintf(client, "IoP %s (no expire)", pSDetail->pDetail->pchName);
                            }
                        }
                    }
                    if (count == 0)
                        conPrintf(client, "No Items of Power"); 
                    conPrintf(client, "---------------------------------------------"); 
                } else {
                    conPrintf(client, "You don't belong to a supergroup");
                }
            }

            xcase SCMD_SG_ITEM_OF_POWER_GRANT_NEW:
            {
                //Debug, so 0 == my SG
                if( !tmp_int )
                    tmp_int = client->entity->supergroup_id;

                if (!ItemOfPowerGrantNew(client->entity, tmp_int))
                    conPrintf(client, "You don't belong to a supergroup");
            }

            xcase SCMD_SG_ITEM_OF_POWER_SYNCH:
            {
                //Debug, so 0 == my SG
                if( !tmp_int )
                    tmp_int = client->entity->supergroup_id;

                if (!ItemOfPowerSGSynch( tmp_int ) )
                    conPrintf(client, "You don't belong to a supergroup");
            }
            xcase SCMD_SG_ITEM_OF_POWER_TRANSFER:
            {
                //Debug, so 0 == my SG
                if( !tmp_int )
                    tmp_int = client->entity->supergroup_id;

                if (!ItemOfPowerTransfer( client->entity, tmp_int, tmp_int2, tmp_str, tmp_int3 ))
                    conPrintf(client, "You don't belong to a supergroup");
            }
            xcase SCMD_SG_ITEM_OF_POWER_UPDATE:
            {
                conPrintf(client, "Delete this command, it doesn't do anything anymore");
            }
		xcase SCMD_SG_PASSCODE:
			{
				U32 md5hash[4];
				U32 i;
				bool isalpha = true;

				if( !e )
					break;

				if( !e->supergroup_id ) {
					chatSendToPlayer(e->db_id, "You are not in a Supergroup.", INFO_USER_ERROR, 0);
					break;
				}

				if( !sgroup_hasPermission(e, SG_PERM_CAN_SET_BASEENTRY_PERMISSION)) {
					chatSendToPlayer(e->db_id, "Only members who can set the base entry permission can change the base access passcode.", INFO_USER_ERROR, 0);
					break;
				}

				if(strlen(tmp_str) == 0) {
					chatSendToPlayer(e->db_id, "The supergroup base access passcode cannot be empty.", INFO_USER_ERROR, 0);
					break;
				}

				if(strlen(tmp_str) > 20) {
					chatSendToPlayer(e->db_id, "The supergroup base access passcode cannot exceed 20 characters.", INFO_USER_ERROR, 0);
					break;
				}

				for (i = 0; i < strlen(tmp_str); i++) {
					if(!isalnum(tmp_str[i])) {
						// Can't break two levels at once, so set a flag and check it afterwards.
						isalpha = false;
						break;
					}
					// Ignore case, since the passcode is meant to be shared and might be shared verbally.
					tmp_str[i] = toupper(tmp_str[i]);
				}
				
				if (!isalpha) {
					chatSendToPlayer(e->db_id, "The supergroup base access passcode must contain only letters and numbers.", INFO_USER_ERROR, 0);
					break;
				}

				cryptMD5Init();
				cryptMD5Update((U8*)tmp_str, strlen(tmp_str));
				cryptMD5Final(md5hash);

				if(teamLock(e, CONTAINER_SUPERGROUPS))
				{
					char msg[100];
					e->supergroup->passcode = md5hash[0] ^ md5hash[1] ^ md5hash[2] ^ md5hash[3];
					teamUpdateUnlock(e, CONTAINER_SUPERGROUPS);
					sprintf(msg, "Your supergroup base access passcode is now %s-%d", tmp_str, e->supergroup_id);
					chatSendToPlayer(e->db_id, msg, INFO_SVR_COM, 0);
				}
				else
				{
					chatSendToPlayer(e->db_id, "Unable to set the supergroup base access passcode.", INFO_USER_ERROR, 0);
				}

			}break;

		xcase SCMD_SG_MUSIC:
			{
				if( !e )
					break;

				if( !e->supergroup_id ) {
					chatSendToPlayer(e->db_id, "You are not in a Supergroup.", INFO_USER_ERROR, 0);
					break;
				}

				if( !sgroup_hasPermission(e, SG_PERM_BASE_EDIT)) {
					chatSendToPlayer(e->db_id, "Only members who can edit the base can change the base background music.", INFO_USER_ERROR, 0);
					break;
				}

				if(strlen(tmp_str) > 49) {
					chatSendToPlayer(e->db_id, "Unable to set the supergroup background music: the file name is too long.", INFO_USER_ERROR, 0);
					break;
				}

				if(teamLock(e, CONTAINER_SUPERGROUPS))
				{
					char msg[100];
					strncpyt(e->supergroup->music,tmp_str,50);
					teamUpdateUnlock(e, CONTAINER_SUPERGROUPS);
					sprintf(msg, "Your supergroup background music is now %s.", strlen(e->supergroup->music)?e->supergroup->music:"disabled");
					chatSendToPlayer(e->db_id, msg, INFO_SVR_COM, 0);
				}
				else
				{
					chatSendToPlayer(e->db_id, "Unable to set the supergroup background music.", INFO_USER_ERROR, 0);
				}

			}break;

            xcase SCMD_SG_ITEM_OF_POWER_GAME_SET_STATE:
            {
                ItemOfPowerGameSetState(client->entity, tmp_str);
            }
            xcase SCMD_SG_ITEM_OF_POWER_GAME_SHOW_SERVER_STATE:
            {
                ShowItemOfPowerGameStatus( client );
            }
            ////////////////// End Items of Power ///////////////////////////////////////////

            ////////////////// Raid Points //////////////////////////////////////////////////
            xcase SCMD_SG_GRANT_RAID_POINTS:
            {
                if (client->entity && client->entity->supergroup)
                {
                    entity_AddSupergroupRaidPoints(client->entity, tmp_int, tmp_int2);
                    conPrintf(client, "Added %d Raid Points to SG %s", tmp_int, client->entity->supergroup->name);
                } else {
                    conPrintf(client, "Player does not belong to SG");
                }
            }
            xcase SCMD_SG_SHOW_RAID_POINTS:
            {
                if (client->entity && client->entity->supergroup)
                {
                    int points, raidpoints, raidtime, coppoints, coptime;
                    char time[256];

                    conPrintf(client, "---------------------------------------------"); 
                    conPrintf(client, "Raid Point Info for SG %s", client->entity->supergroup->name);
                    conPrintf(client, "---------------------------------------------"); 
                    entity_GetSupergroupRaidPoints(e, &points, &raidpoints, &raidtime, &coppoints, &coptime);

                    conPrintf(client,     "Current Points: %d", points);

                    if (raidtime > 0)
                    {
                        timerMakeOffsetStringFromSeconds(time, raidtime);
                        conPrintf(client, "Raid Points:    %d (expires in %s)", raidpoints, time);
                    }
                    if (coppoints > 0)
                    {
                        timerMakeOffsetStringFromSeconds(time, coptime);
                        conPrintf(client, "CoP Points:     %d (expires in %s)", coppoints, time);
                    }
                } else {
                    conPrintf(client, "Player does not belong to SG");
                }
            }
            ////////////////// End Raid Points //////////////////////////////////////////////////

            xcase SCMD_TEST_IMEBUG: 
            {
                char power[] = 
                    {
                        0x20, 0xec, 0x88, 0x98, 0xed, 0x98, 0xb8,
                        0xeb, 0x8b, 0xa8, 0x9c, 0xec, 0x8a, 0xa4, 0xed, 0x8a, 0xb8, 0xeb,
                        0x84, 0xa4, 0xeb, 0xa9, 0x94, 0xec, 0x8b, 0x9c, 0xec, 0x8a, 0xa4,
                        0xeb, 0xb0, 0xb0, 0xed, 0x8b, 0x80, 0x20, 0xeb, 0xa9, 0x94, 0xec, 0x9d,
                        0xeb, 0x93, 0xa0, 0xec, 0x9d, 0x98, 0x20, 0xed, 0x88, 0xac, 0xec, 0x82,
                        0xac, 0xeb, 0x93, 0xb8, 0x94, 0xeb, 0x9e, 0x99, 0x20, 0xec, 0x8a, 0xa4,
                        0xec, 0x99, 0x84, 0xec, 0x9d, 0x98, 0
                    };
                
                char *strId = "P4057054054";
                conPrintf(client, "string is: %s",power);
            }
            xcase SCMD_SG_CSR_JOIN:
            {
                if( e )
                    csrJoinSupergroup( e, tmp_int );
            }
            xcase SCMD_BASEACCESS_FROMENT_RELAY:
            {
                int idEntRequesting = tmp_int;
                SgrpBaseEntryPermission entryRequested = tmp_int2;
                int idEntRequested = tmp_int3;
                int sideEntRequesting = tmp_int4;
                Entity *e = findEntOrRelayById( admin, idEntRequested, str, NULL );
                if( e && e->supergroup && e->supergroup_id && e->pl && e->pl->playerType == sideEntRequesting
                    && sgrpbaseentrypermission_Valid(entryRequested))
                {
                    char *cmdResponse = NULL;
                    BaseAccess resAccess=sgrp_BaseAccessFromSgrp( e->supergroup, entryRequested );
                    estrCreate(&cmdResponse);
                    estrPrintf(&cmdResponse,"baseaccess_response_relay \"%d,%d,%s\t\" %d", e->supergroup_id, resAccess, e->supergroup->name, idEntRequesting );
                    serverParseClient( cmdResponse, NULL );
                    estrDestroy(&cmdResponse);
                }
            }
            xcase SCMD_BASEACCESS_RESPONSE_RELAY:
            {
                char *responses = tmp_str;
                int idEnt = tmp_int;
                Entity *e = findEntOrRelayById( admin, idEnt, str, NULL );
                if( e )
                {
                    char *iResp;
                    for(iResp = responses; iResp && iResp != ((char*)1) && *iResp; iResp = strchr(iResp,'\t')+1)
                    {
                        int idSgrp;
                        BaseAccess baseAccess;
                        char baseName[128] = "";
                        char *basenameEnd = NULL;
                        
                        idSgrp = atoi(iResp);
                        iResp = strchr(iResp,',');
                        if(!iResp)
                            continue;
                        
                        baseAccess = atoi(++iResp);
                        iResp = strchr(iResp,',');
                        if(!iResp)
                            continue;

                        ++iResp;
                        basenameEnd = strchr(iResp,'\t');
                        if(!basenameEnd)
                            basenameEnd = iResp + strlen(iResp);
                        
                        strncpyt(baseName, iResp, MIN(ARRAY_SIZE( baseName ), (basenameEnd - iResp + 1)));
                                
                        if(baseName[0])
                        {
                            SendBaseAccess(e, baseName, idSgrp, baseAccess);
                        }
                        else
                        {
                            dbLog("CSR:BaseAccessResponse", e, "command '%s' failed to parse", cmd->name);
                        }
                    }
                }
            }
            xcase SCMD_SGRP_BASE_PRESTIGE_FIXED: 
            {
                if( e )
                {
                    if( e->supergroup )
                    {
                        if( teamLock(e, CONTAINER_SUPERGROUPS) )
                        {
                            if(tmp_int)
                            {
                                e->supergroup->flags |= SG_FLAGS_FIXED_BAD_BASE_PRESTIGE;
                                conPrintf(client,"set flag");
                            }
                            else
                            {
                                e->supergroup->flags &= ~SG_FLAGS_FIXED_BAD_BASE_PRESTIGE;
                                conPrintf(client,"cleared flag");
                            }
                            teamUpdateUnlock(e, CONTAINER_SUPERGROUPS);
                        }
                        else
                        {
                            conPrintf(client, "lock failed for supergroup");
                        }
                    }
                    else
                    {
                        conPrintf(client, "not in a supergroup");
                    }
                }
                else
                {
                    conPrintf(client, "no controlled entity");
                }
            }
            xcase SCMD_SGRP_BASE_PRESTIGE_SHOW:
            {
                 if( e )
                 {
                     if( e->supergroup )
                     {
                         conPrintf(client, "base prestige: %i", e->supergroup->prestigeBase);
                     }
                     else
                     {
                         conPrintf(client, "not in a supergroup");
                     }
                 }
                 else
                 {
                     conPrintf(client, "no controlled entity");
                 }
            }
            xcase SCMD_SGRP_BASE_PRESTIGE_SET:
            {
                if( e )
                {
                    if( e->supergroup )
                    {
                        if( teamLock(e, CONTAINER_SUPERGROUPS) )
                        {
                            e->supergroup->prestigeBase = tmp_int;
                            teamUpdateUnlock(e, CONTAINER_SUPERGROUPS);
                        }
                        else
                        {
                            conPrintf(client, "lock failed for supergroup");
                        }
                    }
                    else
                    {
                        conPrintf(client, "not in a supergroup");
                    }
                }
                else
                {
                    conPrintf(client, "no controlled entity");
                }                
            }

            xcase SCMD_POWER_COLOR_P1:
            case SCMD_POWER_COLOR_P2:
            case SCMD_POWER_COLOR_S1:
            case SCMD_POWER_COLOR_S2:
            {
                if( e && e->pl )
                {                
                    unsigned int color;
                    PowerCustomizationList *powerCustList;

                    if (tmp_int > 255)
                        tmp_int = 255;
                    else if (tmp_int < 0)
                        tmp_int = 0;
                    if (tmp_int2 > 255)
                        tmp_int2 = 255;
                    else if (tmp_int2 < 0)
                        tmp_int2 = 0;
                    if (tmp_int3 > 255)
                        tmp_int3 = 255;
                    else if (tmp_int3 < 0)
                        tmp_int3 = 0;

                    color = tmp_int | tmp_int2 << 8 | tmp_int3 << 16 | 0xff000000;

                    powerCustList = powerCustList_current(e);

                    if (powerCustList)
                    {
                        int i;
                        PowerCustomization** pcust = powerCustList->powerCustomizations;
                        const PowerCategory* category = 
                            (cmd->num == SCMD_POWER_COLOR_P1 || cmd->num == SCMD_POWER_COLOR_P2) ?
                                e->pchar->pclass->pcat[kCategory_Primary] :
                                e->pchar->pclass->pcat[kCategory_Secondary];
                        for (i = 0; i < eaSize(&powerCustList->powerCustomizations); ++i)
                        {
                            const BasePower *power = powerCustList->powerCustomizations[i]->power;
                            if (power && power->psetParent->pcatParent == category)
                            {
                                if (cmd->num == SCMD_POWER_COLOR_P1 || cmd->num == SCMD_POWER_COLOR_S1)
                                    powerCustList->powerCustomizations[i]->customTint.primary.integer = color;
                                else
                                    powerCustList->powerCustomizations[i]->customTint.secondary.integer = color;
                            }
                        }
                    }
                }
            }

            xcase SCMD_BASE_STORAGE_ADJUST:
            case SCMD_BASE_STORAGE_SET:
            {
                char *nameSal = tmp_str;
                int iLevel = tmp_int2;
                int amt = tmp_int;
                RoomDetail *detail = (e && e->pl && e->pl->detailInteracting) ? baseGetDetail(&g_base, e->pl->detailInteracting, NULL ) : NULL;
                if( detail && nameSal )
                {
                    switch ( detail->info->eFunction )
                    {
                    case kDetailFunction_StorageSalvage:
                        if(!nameSal || !*nameSal)
                            conPrintf(client, "usage: <amount> <salvage name>");
                        else if( !roomdetail_CanAdjSalvage(detail, nameSal, amt) )
                            conPrintf( client, "can't adjust %s by %i in selected detail", nameSal, amt);
                        else
                            (cmd->num==SCMD_BASE_STORAGE_ADJUST?roomdetail_AdjSalvage:roomdetail_SetSalvage)(detail, nameSal, amt);
                        break;
                    case kDetailFunction_StorageInspiration:
                        if(!nameSal || !*nameSal)
                            conPrintf(client, "usage: <amount> <inspiration path> ");
                        else if( !roomdetail_CanAdjInspiration(detail, nameSal, amt) )
                            conPrintf( client, "can't adjust %s by %i in selected detail", nameSal, amt);
                        else
                            (cmd->num==SCMD_BASE_STORAGE_ADJUST?roomdetail_AdjInspiration:roomdetail_SetInspiration)(detail, nameSal, amt);
                        break;
                    case kDetailFunction_StorageEnhancement:
                        if(!nameSal || !*nameSal )
                            conPrintf(client, "usage: <amount> <enhancement path> <level>");
                        else if( !roomdetail_CanAdjEnhancement(detail, nameSal, iLevel, 0, amt) )
                            conPrintf( client, "can't adjust %s by %i in selected detail", nameSal, amt);
                        else
                            (cmd->num==SCMD_BASE_STORAGE_ADJUST?roomdetail_AdjEnhancement:roomdetail_SetEnhancement)(detail, nameSal, iLevel, 0, amt);
                        break;
                    default:
                        conPrintf(client, "this detail can't be adjusted.");
                    };

                }
                else
                {
                    conPrintf( client, "no detail selected.");
                } 
            }
            xcase SCMD_OFFLINE_CHARACTER:
                dbOfflineCharacter( client, tmp_str);
            xcase SCMD_OFFLINE_RESTORE_DELETED:
                dbRestoreDeletedChar( client, tmp_str );
            xcase SCMD_OFFLINE_LIST_DELETED:
                dbListDeletedChars( client, tmp_str );
            xcase SCMD_EDIT_VILLAIN_COSTUME:
                editVillainCostume( e, tmp_str, tmp_int );
            xcase SCMD_EDIT_NPC_COSTUME:
                editNpcCostume( e, tmp_str );
            xcase SCMD_BACKUP_PLAYER:
                dbbackup( client, tmp_str, CONTAINER_ENTS );
            xcase SCMD_BACKUP_SEARCH:
                dbbackup_search( client, tmp_str, CONTAINER_ENTS );
            xcase SCMD_BACKUP_VIEW:
                dbbackup_view( client, tmp_str, tmp_int, CONTAINER_ENTS );
            xcase SCMD_BACKUP_APPLY:
                dbbackup_apply( client, tmp_str, tmp_int, CONTAINER_ENTS );
            xcase SCMD_BACKUP_SG:
                dbbackup( client, tmp_str, CONTAINER_SUPERGROUPS );
            xcase SCMD_BACKUP_SEARCH_SG:
                dbbackup_search( client, tmp_str, CONTAINER_SUPERGROUPS  );
            xcase SCMD_BACKUP_VIEW_SG:
                dbbackup_view( client, tmp_str, tmp_int, CONTAINER_SUPERGROUPS  );
            xcase SCMD_BACKUP_APPLY_SG:
                dbbackup_apply( client, tmp_str, tmp_int, CONTAINER_SUPERGROUPS  );
            xcase SCMD_FORCE_LOGOOUT:
                forceLogout( e->db_id, tmp_int );
            xcase SCMD_PLAYER_EVAL:
                {
                    char        *eval;
                    char        *argv[100];
                    StringArray expr;
                    int            argc;
                    int            i;

                    eval = strdup(tmp_str);
                    argc = tokenize_line(eval, argv, 0);
                    if (argc > 0)
                    {
                        eaCreate(&expr);
                        for (i = 0; i < argc; i++)
                            eaPush(&expr, argv[i]);

                        conPrintf( client, localizedPrintf(e, "%d", chareval_Eval(e->pchar, expr, "*slash command*")));
                        eaDestroy(&expr);
                    }
                    if (eval)
                        free(eval);
                }
            xcase SCMD_COMBAT_EVAL:
                {
                    char        *eval;
                    char        *argv[100];
                    StringArray expr;
                    int            argc;
                    int            i;
                    Entity        *eTarget = NULL;
                    Power        *ppow = character_OwnsPower(e->pchar, powerdict_GetBasePowerByFullName(&g_PowerDictionary, tmp_str2));

                    ClientLink    *pClient = clientFromEnt(e);
                    if (pClient)
                    {
                        eTarget = validEntFromId(pClient->controls.selected_ent_server_index);
                    }

                    if (!ppow)
                        break;

                    // Store test info to ensure any calls don't fail.
                    combateval_StoreToHitInfo(0.0f, 0.0f, false, false);
                    combateval_StoreAttribCalcInfo(0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
                    combateval_StoreCustomFXToken(NULL);
                    combateval_StoreTargetsHit(0);
                    combateval_StoreChance(0.0f, 0.0f);

                    eval = strdup(tmp_str);
                    argc = tokenize_line(eval, argv, 0);
                    if (argc > 0)
                    {
                        eaCreate(&expr);
                        for (i = 0; i < argc; i++)
                            eaPush(&expr, argv[i]);

                        conPrintf(client, localizedPrintf(e, "%d", combateval_Eval(e, eTarget, ppow, expr, ppow->ppowBase->pchSourceFile)));
                        eaDestroy(&expr);
                    }
                    if (eval)
                        free(eval);
                }
            xcase SCMD_COMPLETE_STUCK_MISSION:
                entity_MissionStuckSetComplete(e, client, tmp_int);
            xcase SCMD_AUCTION_REQ_INV:
                 ent_ReqAuctionInv(e);
            xcase SCMD_AUCTION_REQ_INV_FAILED:
             {
                 int idEnt = tmp_int;
                 Entity *e = findEntOrRelayById( admin, idEnt, str, NULL );
                 char *reason = tmp_str;                 
                 conPrintf( client, localizedPrintf(e,reason));
             }
            //Following xcase seems shady and does not appear to be used.
            //Why would CS pass an index to an internal array?
            //Keeping it in in case I'm wrong and CS actually wants this. -JH
            //xcase SCMD_AUCTION_ADD_SALVAGEITEM:      
            //{
            //    int idx = tmp_int2;
            //    int amt = tmp_int3;
            //    int price = tmp_int4;
            //    char *name_sal = NULL;
            //    
            //    SalvageInventoryItem *s;
            //    
            //    if(!e)
            //        break;

            //    s = eaGet(&e->pchar->salvageInv,idx);
            //    
            //    if(!s||!s->salvage)
            //    {
            //        conPrintf(client, "can't add itm at index %i",idx);
            //        break; 
            //    }
            //    strdup_alloca(name_sal,s->salvage->pchName);
            //    
            //    if(character_AdjustSalvage(e->pchar, s->salvage, -amt, "auction", false)<0) 
            //    {
            //        conPrintf(client, "can't remove %i salvage of type %s",amt,s->salvage->pchName);
            //        break; 
            //    }
            //    
            //    ent_AddAuctionInvItemXact(e, auction_identifier(kTrayItemType_Salvage, name_sal, 0), 0, amt, price, AuctionInvItemStatus_ForSale, -1); 
            //    conPrintf(client, "added salvage to auction inventory.");
            //}
            xcase SCMD_AUCTION_TRUST_GAMECLIENT: 
                if(!e)
                    break;
                e->auc_trust_gameclient = tmp_int?1:0;
            xcase SCMD_ACC_UNLOCK:
                if(e && e->pl)
                {
                    e->pl->accountServerLock[0] = '\0';
                    e->pl->shardVisitorData[0] = '\0';
                }
            xcase SCMD_CONPRINT:
                 conPrintf(client,"%s",tmp_str);
            xcase SCMD_DUMP_STORYARCINFO:
                 DumpSpawndefs();
            xcase SCMD_ACCOUNT_GRANT_CHARSLOT:
                if (e)
                {
                    // send request up the chain!
                    Packet *pak_out = pktCreateEx(&db_comm_link, DBCLIENT_GRANT_CHARSLOT);
                    pktSendBitsAuto(pak_out, getAuthId(e));
                    pktSendBitsAuto(pak_out, tmp_int); 
                    pktSend(&pak_out,&db_comm_link);
                    conPrintf(client, "account %s granted %d character slots", e->auth_name, tmp_int);
                }
            xcase SCMD_ACCOUNT_CERTIFICATION_TEST:
                {
                    int position = tmp_int % 100;

                    if (position == 0)
                    {
                        server_state.accountCertificationTestStage = 0;
                        // still send this to the other servers
                    }

                    if (position == 1 || position == 5)
                    {
                        server_state.accountCertificationTestStage = tmp_int;
                    }
                    else
                    {
                        Packet *pak_out = pktCreateEx(&db_comm_link, DBCLIENT_ACCOUNTSERVER_CERTIFICATION_TEST);
                        pktSendBitsAuto(pak_out, tmp_int); 
                        pktSend(&pak_out,&db_comm_link);
                    }
                }
            xcase SCMD_ACCOUNT_CERTIFICATION_BUY:
                if (e && e->pl) 
                {
                    U32 time = dbSecondsSince2000();
                    const DetailRecipe *r = detailrecipedict_RecipeFromName(tmp_str);                    
                    const AccountProduct *p = accountCatalogGetProductByRecipe(tmp_str);
                    
                    if (server_state.accountCertificationTestStage % 100 == 1)
                    {
                        int flags = server_state.accountCertificationTestStage / 100;

                        if (flags % 10 == 0)
                        {
                            server_state.accountCertificationTestStage = 0;
                        }

                        if (flags / 10 == 1)
                        {
                            // there's no response that truly must happen if this message fails...
                        }

                        break;
                    }

                    if(!r)
                        chatSendToPlayer(e->db_id, localizedPrintf(e,"CertificationCantFindRecipe"), INFO_USER_ERROR, 0 );
                    else if(!p)
                        chatSendToPlayer(e->db_id, localizedPrintf(e,"CertificationCantFindProduct"), INFO_USER_ERROR, 0 );
                    else if (tmp_int != 0 && tmp_int != 1)
                        chatSendToPlayer(e->db_id, localizedPrintf(e,"CertificationInvalidType"), INFO_USER_ERROR, 0 );                    
                    else if (tmp_int == 1 && !(r->flags & RECIPE_VOUCHER) && (p->invType != kAccountInventoryType_Voucher)) 
                        chatSendToPlayer(e->db_id, localizedPrintf(e,"CertificationWrongTypeVoucher"), INFO_USER_ERROR, 0 );
                    else if (tmp_int == 0 && !(r->flags & RECIPE_CERTIFICATON) && (p->invType != kAccountInventoryType_Certification))
                        chatSendToPlayer(e->db_id, localizedPrintf(e,"CertificationWrongTypeCertification"), INFO_USER_ERROR, 0 );                    
                    else if( time - e->pl->last_certification_request < 2 )
                        chatSendToPlayer(e->db_id, localizedPrintf(e,"CertificationThrottled"), INFO_USER_ERROR, 0 );
                    else if (!e->pl->account_inv_loaded)
                    {
                        dbRelayGetAccountServerInventory(e->auth_id);
                        chatSendToPlayer(e->db_id, localizedPrintf(e,"CannotBuyCertNoInventory"), INFO_USER_ERROR, 0 );
                    }
                    else
                    {
                        CertificationRecord *pRecord = certificationRecordInHistory(e, tmp_str);

                        if( !pRecord ) 
                        {
                            pRecord = certificationRecord_Create();
                            estrConcatCharString(&pRecord->pchRecipe, tmp_str);
                            eaPush(&e->pl->ppCertificationHistory, pRecord);
                        }

                        // send request up the chain!
                        if( !certificationRecord_Locked(pRecord) )
                        {
                            Packet *pak_out = pktCreateEx(&db_comm_link, DBCLIENT_ACCOUNTSERVER_CERTIFICATION_GRANT);
                            pktSendBitsAuto(pak_out, e->auth_id);
                            pktSendBitsAuto2(pak_out, p->sku_id.u64);
                            pktSendBitsAuto(pak_out, e->db_id); 
                            pktSendBitsAuto(pak_out, tmp_int2); 
                            pktSend(&pak_out,&db_comm_link);

                            pRecord->status = kCertStatus_GrantRequested; // no other actions may be performed on this record until acknowledged
                            pRecord->timed_locked = time;
                            e->pl->last_certification_request = time;
                        }
                        else
                            chatSendToPlayer(e->db_id, localizedPrintf(e,"CannotBuyCertGrantPending"), INFO_USER_ERROR, 0 );
                    }
                }
            xcase SCMD_ACCOUNT_CERTIFICATION_REFUND:
                if (e && e->pl) 
                {
                    chatSendToPlayer(e->db_id, localizedPrintf(e,"CertificationRefundsAreDisabled"), INFO_USER_ERROR, 0 );

                    TODO();
#if 0
                    U32 time = dbSecondsSince2000();

                    if (server_state.accountCertificationTestStage % 100 == 1)
                    {
                        int flags = server_state.accountCertificationTestStage / 100;

                        if (flags % 10 == 0)
                        {
                            server_state.accountCertificationTestStage = 0;
                        }

                        if (flags / 10 == 1)
                        {
                            // there's no response that truly must happen if this message fails...
                        }

                        break;
                    }

                    if( time - e->pl->last_certification_request < 2 )
                        chatSendToPlayer(e->db_id, localizedPrintf(e,"CertificationThrottled"), INFO_USER_ERROR, 0 );
                    else
                    {
                        CertificationRecord *pRecord = certificationRecordInHistory(e, tmp_str);
                        const AccountProduct *pProduct = accountCatalogGetProductByRecipe(tmp_str);
                        AccountInventory *pItem = AccountInventoryFindItem(e, pProduct ? pProduct->sku_id : kSkuIdInvalid);
                        const DetailRecipe * pRecipe = detailrecipedict_RecipeFromName(tmp_str);

                        if(!pItem || !tmp_str)
                        {
                            dbRelayGetAccountServerInventory(e->auth_id);
                            // Error message or log?
                            break; // how can we have record of item that doesn't exist?
                        }

                        if( pRecipe != NULL && pRecipe->flags & RECIPE_NO_REFUND )
                        {
                            chatSendToPlayer(e->db_id, localizedPrintf(e,"CannotRefundCertificationNotAllowed"), INFO_USER_ERROR, 0 );
                            break;
                        }

                        // Time check?
                        if( timerSecondsSince2000() - pItem->date > REFUND_TIME_OUT )
                        {
                            chatSendToPlayer(e->db_id, localizedPrintf(e,"CannotRefundCertificationTooOld"), INFO_USER_ERROR, 0 );
                            dbRelayGetAccountServerInventory(e->auth_id);
                            break;
                        }

                        if( !pRecord )
                        {
                            pRecord = certificationRecord_Create();
                            estrConcatCharString(&pRecord->pchRecipe, tmp_str);
                            eaPush(&e->pl->ppCertificationHistory, pRecord);
                        }

                        if( pRecord->claimed >= pItem->granted_total )
                        {
                            chatSendToPlayer(e->db_id, localizedPrintf(e,"CannotRefundCertificationClaimed"), INFO_USER_ERROR, 0 );
                            dbRelayGetAccountServerInventory(e->auth_id);
                            break;
                        }

                        if( pRecord && !certificationRecord_Locked(pRecord) )
                        {
                            Packet *pak_out = pktCreateEx(&db_comm_link, DBCLIENT_ACCOUNTSERVER_CERTIFICATION_REFUND);
                            pktSendBitsAuto(pak_out, e->auth_id);
                            pktSendBitsAuto2(pak_out, pItem->sku_id.u64);
                            pktSendBitsAuto(pak_out,e->db_id);
                            pktSend(&pak_out,&db_comm_link);

                            pRecord->status = kCertStatus_RefundRequested; // no other actions may be performed on this record until acknowledged
                            pRecord->timed_locked = time;
                            e->pl->last_certification_request = time;
                        }
                        else
                            chatSendToPlayer(e->db_id, localizedPrintf(e,"CannotRefundCertificationPending"), INFO_USER_ERROR, 0 );
                    }
#endif
                }
            xcase SCMD_ACCOUNT_CERTIFICATION_DELETE:
                if (e && e->pl) 
                {
                    CertificationRecord *pRecord = certificationRecordInHistory(e, tmp_str);
                    AccountInventory *pItem = AccountInventoryFindCert(e, tmp_str);
                    if(!pItem)
                        break; // how can we have record of item that doesn't exist?

                    if( !pRecord )
                    {
                        pRecord = certificationRecord_Create();
                        estrConcatCharString(&pRecord->pchRecipe, tmp_str);
                        eaPush(&e->pl->ppCertificationHistory, pRecord);
                    }

                    if( pRecord->deleted >= pItem->granted_total || certificationRecord_Locked(pRecord) )
                        break; // cannot delete more than we have // Error message?

                    pRecord->deleted = pItem->granted_total;
                }
            xcase SCMD_ACCOUNT_CERTIFICATION_ERASE:
                if (e && e->pl) 
                {
                    CertificationRecord *pRecord = certificationRecordInHistory(e, tmp_str);
                    AccountInventory *pItem = AccountInventoryFindCert(e, tmp_str);
                    if(!pItem)
                        break; // how can we have record of item that doesn't exist?
                    pItem->granted_total = 0;
                    pRecord->claimed = 0;
                    pRecord->status = kCertStatus_None;
                }
            xcase SCMD_ACCOUNT_CERTIFICATION_SHOW:
                if (e && e->pl) 
                {
                    int i;

                    for( i = eaSize(&e->pl->ppCertificationHistory)-1; i>=0; i-- )
                    {
                        CertificationRecord *pCert = e->pl->ppCertificationHistory[i];
                        if (pCert != NULL)
                            conPrintf(client, "\t\t%s claimed:%d deleted:%d status:%d",pCert->pchRecipe, pCert->claimed, pCert->deleted, pCert->status);
                    }
                }
            xcase SCMD_ACCOUNT_CERTIFICATION_DELETE_ALL:
                if (e && e->pl) 
                {
                    certificationRecord_DestroyAll(e);
                }
            xcase SCMD_ACCOUNT_CERTIFICATION_CLAIM:
                {
                    if (e && e->pl && tmp_str) 
                    {
                        U32 time = dbSecondsSince2000();
                        const DetailRecipe * pRecipe = detailrecipedict_RecipeFromName(tmp_str);

                        if (server_state.accountCertificationTestStage % 100 == 1)
                        {
                            int flags = server_state.accountCertificationTestStage / 100;

                            if (flags % 10 == 0)
                            {
                                server_state.accountCertificationTestStage = 0;
                            }

                            if (flags / 10 == 1)
                            {
                                // there's no response that truly must happen if this message fails...
                            }

                            break;
                        }
                        
                        if( time - e->pl->last_certification_request < 2 )
                            chatSendToPlayer(e->db_id, localizedPrintf(e,"CertificationThrottled"), INFO_USER_ERROR, 0 );
                        else if (pRecipe && (pRecipe->flags & RECIPE_INVENTION_LIC_REQUIRED) != 0 && !AccountHasStoreProduct(ent_GetProductInventory( e ), SKU("coliinvn")))
                        {
                            chatSendToPlayer(e->db_id, localizedPrintf(e,"UiInventoryNoInventionLicense"), INFO_USER_ERROR, 0 );
                        }
                        else
                        {
                            CertificationRecord *pRecord = certificationRecordInHistory(e, tmp_str);
                            AccountInventory *pItem = AccountInventoryFindCert(e, tmp_str);

                            if(!pItem || !tmp_str)
                            {
                                // Error message or log?
                                break; // how can we have record of item that doesn't exist?
                            }

                            if( !pRecord ) 
                            {
                                pRecord = certificationRecord_Create();
                                estrConcatCharString(&pRecord->pchRecipe, tmp_str);
                                eaPush(&e->pl->ppCertificationHistory, pRecord);
                            }

                            if (( pItem->invType == kAccountInventoryType_Certification && pRecord->claimed >= pItem->granted_total ) ||
                                ( pItem->invType == kAccountInventoryType_Voucher && pItem->claimed_total >= pItem->granted_total ))
                            {
                                chatSendToPlayer(e->db_id, localizedPrintf(e,"CertificationAlreadyClaimed"), INFO_USER_ERROR, 0 );
                                break;
                            }

                            // technically, the accountserver could be left out of the loop for a certification since any character is allowed one,
                            // but we will make sure accountserver acknowledges it in case of a refund later
                            if( !certificationRecord_Locked(pRecord) )
                            {
                                Packet *pak_out = pktCreateEx(&db_comm_link, DBCLIENT_ACCOUNTSERVER_CERTIFICATION_CLAIM);
                                pktSendBitsAuto(pak_out, e->auth_id);
                                pktSendBitsAuto2(pak_out, pItem->sku_id.u64);
                                pktSendBitsAuto(pak_out, e->db_id);
                                pktSend(&pak_out,&db_comm_link);

                                pRecord->status = kCertStatus_ClaimRequested; // no other actions may be performed on this record until acknowledged
                                pRecord->timed_locked = time;
                                e->pl->last_certification_request = time;
                            }
                            else
                                chatSendToPlayer(e->db_id, localizedPrintf(e,"CannotClaimCertificationPending"), INFO_USER_ERROR, 0 );
                        }
                    }
                }
            xcase SCMD_ACCOUNT_INVENTORY_LIST:
                 if (e && e->pl && e->pl->account_inv_loaded)
                 {
                     int i;
                     conPrintf(client, "account inventory:");
                     for( i = eaSize( &e->pl->account_inventory.invArr ) - 1; i >= 0; --i)
                     {
                         char str[128];
                         if( e->pl->account_inventory.invArr[i] )
                         {
                             AccountInventory    *tok = e->pl->account_inventory.invArr[i];
                             timerMakeDateStringFromSecondsSince2000(str, tok->expires );
                             conPrintf(client, "\t\t%.8s = %d(%s)", tok->sku_id.c, tok->granted_total, str);
                             if ( tok->invType == kAccountInventoryType_Certification )
                                 conPrintf( client, "\t\t\tDesc: %s, Claimed: %i", tok->recipe, tok->claimed_total );
                             else if ( tok->invType == kAccountInventoryType_PlayerInventory )
                                 conPrintf( client, "\t\t\tDesc: %s", tok->recipe);
                         }
                         else
                         {
                             conPrintf(client, "\t\t NULL reward at %d",i);
                         }
                     }
                 }
            xcase SCMD_ACCOUNT_INVENTORY_CHANGE:
                 if (e && e->pl)
                 {
                     // send request up the chain!
                     Packet *pak_out = pktCreateEx(&db_comm_link, DBCLIENT_ACCOUNTSERVER_CHANGE_INV);
                     pktSendBitsAuto(pak_out, e->auth_id);
                     pktSendBitsAuto2(pak_out, skuIdFromString(tmp_str).u64);
                     pktSendBitsAuto(pak_out, tmp_int); 
                     pktSend(&pak_out,&db_comm_link);
                 }
            xcase SCMD_ACCOUNT_LOYALTY_BUY:
                 if (e && e->pl)
                 {
                     LoyaltyRewardNode *node = accountLoyaltyRewardFindNode(tmp_str);
                     // see if this even valid
                     if (node != NULL && accountLoyaltyRewardCanBuyNode(ent_GetProductInventory( e ), e->pl->account_inventory.accountStatusFlags, e->pl->loyaltyStatus, e->pl->loyaltyPointsUnspent, node))
                     {
                         // send request up the chain!
                         Packet *pak_out = pktCreateEx(&db_comm_link, DBCLIENT_ACCOUNTSERVER_LOYALTY_CHANGE);
                         pktSendBitsAuto(pak_out, e->auth_id);
                         pktSendBitsAuto(pak_out, node->index); 
                         pktSendBool(pak_out, 1); 
                         pktSend(&pak_out,&db_comm_link);
                     }
                 }
            xcase SCMD_ACCOUNT_LOYALTY_REFUND:
                 if (e && e->pl)
                 {
                     // see if this even valid
                     LoyaltyRewardNode *node = accountLoyaltyRewardFindNode(tmp_str);

                     if (node != NULL && accountLoyaltyRewardHasNode(e->pl->loyaltyStatus,tmp_str))
                     {
                         // send request up the chain!
                         Packet *pak_out = pktCreateEx(&db_comm_link, DBCLIENT_ACCOUNTSERVER_LOYALTY_CHANGE);
                         pktSendBitsAuto(pak_out, e->auth_id);
                         pktSendBitsAuto(pak_out, node->index); 
                         pktSendBool(pak_out, 0); 
                         pktSend(&pak_out,&db_comm_link);
                     }
                 }
            xcase SCMD_ACCOUNT_LOYALTY_EARNED:
                 if (e && e->pl)
                 {
                     // send request up the chain!
                     Packet *pak_out = pktCreateEx(&db_comm_link, DBCLIENT_ACCOUNTSERVER_LOYALTY_EARNED_CHANGE);
                     pktSendBitsAuto(pak_out, e->auth_id);
                     pktSendBitsAuto(pak_out, tmp_int); 
                     pktSend(&pak_out,&db_comm_link);
                 }
            xcase SCMD_ACCOUNT_LOYALTY_SHOW:
                 if (e && e->pl)
                 {
                     int tierIdx, nodeIdx;
                     conPrintf(client, "------------------------------\nEarned Loyalty Points: %d\nUnspent Loyalty Points: %d\n", e->pl->loyaltyPointsEarned, e->pl->loyaltyPointsUnspent);


                     // validate names and set indexes
                     for (tierIdx = 0; tierIdx < eaSize(&(g_accountLoyaltyRewardTree.tiers)); tierIdx++)
                     {
                         LoyaltyRewardTier *tier = g_accountLoyaltyRewardTree.tiers[tierIdx];

                         if (accountLoyaltyRewardUnlockedTier(e->pl->loyaltyStatus, tier->name))
                             conPrintf(client, "Tier: %s unlocked\n", tier->name); 

                         for (nodeIdx = 0; nodeIdx < eaSize(&tier->nodeList); nodeIdx++)
                         {
                             LoyaltyRewardNode *node = tier->nodeList[nodeIdx];

                             if (accountLoyaltyRewardHasNode(e->pl->loyaltyStatus, node->name))
                                 conPrintf(client, "    Node: %s\n", node->name);                            
                         }
                     }

                     conPrintf(client, "\nLevels Earned\n");                            
                     for (tierIdx = 0; tierIdx < eaSize(&(g_accountLoyaltyRewardTree.levels)); tierIdx++)
                     {
                        if (accountLoyaltyRewardHasThisLevel(e->pl->loyaltyPointsEarned, g_accountLoyaltyRewardTree.levels[tierIdx]))
                            conPrintf(client, "    %s\n", g_accountLoyaltyRewardTree.levels[tierIdx]->name); 
                     }

                 }
             xcase SCMD_ACCOUNT_LOYALTY_RESET:
                 if (e && e->pl)
                 {
                     // send request up the chain!
                     Packet *pak_out = pktCreateEx(&db_comm_link, DBCLIENT_ACCOUNTSERVER_LOYALTY_RESET);
                     pktSendBitsAuto(pak_out, e->auth_id);
                     pktSend(&pak_out,&db_comm_link);
                 }
                 
             xcase SCMD_RELAY_CONPRINTF:
                 {
                    Entity *e = findEntOrRelayById(admin, tmp_int, str, NULL);
                    if( e )
                    {
                        START_PACKET( pak1, e, SERVER_CON_PRINTF );
                        pktSendString( pak1, unescapeString(tmp_str) );
                        END_PACKET
                    }
                 }
            xcase SCMD_UNSPAMME:
                 {
                     shardCommSendf(e, false, "unSpamMe" );
                     e->pl->is_a_spammer = 0;
                     e->chat_ban_expire = 0;
                 }
            xcase SCMD_SELECT_BUILD:
                {
                    if (--tmp_int >= 0 && tmp_int < MAX_BUILD_NUM  && tmp_int < character_MaxBuildsAvailable(e))
                    {
                        character_SelectBuild(e, tmp_int, 1);
                    }
                }
            xcase SCMD_ARCHITECT_USERINFO:
                if(client && e)
                    missionserver_map_printUserInfo(e, client->message_dbid ? client->message_dbid : e->db_id, NULL, NULL);
            xcase SCMD_ARCHITECT_OTHERUSERINFO:
                if(client && e)
                    missionserver_map_printUserInfo(e, client->message_dbid ? client->message_dbid : e->db_id, tmp_str, tmp_str2);
            xcase SCMD_ARCHITECT_USERIDINFO:
                if(client && e)
                    missionserver_map_printUserInfoById(e, client->message_dbid ? client->message_dbid : e->db_id, tmp_int, tmp_str2);
            xcase SCMD_ARCHITECT_JOIN_RELAY:
                {
                    Entity *e = findEntOrRelayById(admin, tmp_int, str, NULL);
                    if(e)
                    {
                        teamMateEnterArchitectTaskforce( e, tmp_int2, tmp_int3 );
                    }
                }
            xcase SCMD_ARCHITECT_CLAIM_TICKETS:
                {
                    missionserver_map_claimTickets(e, tmp_int);
                }
            xcase SCMD_ARCHITECT_GRANT_TICKETS:
                {
                    missionserver_map_grantTickets(e, tmp_int);
                }
            xcase SCMD_ARCHITECT_GRANT_OVERFLOW_TICKETS:
                {
                    missionserver_map_grantOverflowTickets(e, tmp_int);
                }
            xcase SCMD_ARCHITECT_SET_BEST_ARC_STARS:
                {
                    missionserver_map_setBestArcStars(e, tmp_int);
                }
            xcase SCMD_ARCHITECT_CSR_GET_ARC:
                {
                    missionserver_map_requestArcData_otherUser(e, tmp_int);
                }
            xcase SCMD_DAYJOB_DEBUG:
                if(e)
                {
                    e->dayjob_applied=0;
                    e->last_day_jobs_start = timerSecondsSince2000() - tmp_int;
                }

            xcase SCMD_ROGUE_SWITCHPARAGON:
                {
                    RewardToken *rt = giveRewardToken(e, "newalignment_paragon");
                    conPrintf(client, "newalignment_paragon Points: %d\n", rt ? rt->val : 0);
                }
            xcase SCMD_ROGUE_SWITCHHERO:
                {
                    RewardToken *rt = giveRewardToken(e, "newalignment_hero");
                    conPrintf(client, "newalignment_hero Points: %d\n", rt ? rt->val : 0);
                }
            xcase SCMD_ROGUE_SWITCHVIGILANTE:
                {
                    RewardToken *rt = giveRewardToken(e, "newalignment_vigilante");
                    conPrintf(client, "newalignment_vigilante Points: %d\n", rt ? rt->val : 0);
                }
            xcase SCMD_ROGUE_SWITCHROGUE:
                {
                    RewardToken *rt = giveRewardToken(e, "newalignment_rogue");
                    conPrintf(client, "newalignment_rogue Points: %d\n", rt ? rt->val : 0);
                }
            xcase SCMD_ROGUE_SWITCHVILLAIN:
                {
                    RewardToken *rt = giveRewardToken(e, "newalignment_villain");
                    conPrintf(client, "newalignment_villain Points: %d\n", rt ? rt->val : 0);
                }
            xcase SCMD_ROGUE_SWITCHTYRANT:
                {
                    RewardToken *rt = giveRewardToken(e, "newalignment_tyrant");
                    conPrintf(client, "newalignment_tyrant Points: %d\n", rt ? rt->val : 0);
                }

            xcase SCMD_ALIGNMENT_DROPTIP:
                {
                    RewardToken *rt = giveRewardToken(e, "alignmenttiptoken_generic");
                    conPrintf(client, "alignmenttiptoken_generic count: %d\n", rt?rt->val : 0);
                }

            xcase SCMD_ALIGNMENT_RESETTIMERS:
                {
                    if (e)
                        alignmentshift_resetTimers(e);
                }

            xcase SCMD_ALIGNMENT_RESETSINGLETIMER:
                {
                    if (e)
                        alignmentshift_resetSingleTimer(e, tmp_int - 1);
                }

            xcase SCMD_DROPTIP_DESIGNER:
                {
                    if (tmp_str)
                    {
                        int i;
                        for (i = 0; i < eaSize(&g_DesignerContactTipTypes.designerTips); ++i)
                        {
                            const DesignerContactTip *d_tip = g_DesignerContactTipTypes.designerTips[i];
                            if (stricmp(d_tip->tokenTypeStr, tmp_str) == 0)
                            {
                                RewardToken *rt = giveRewardToken(e, d_tip->rewardTokenName);
                                conPrintf(client, "%s: %d\n", d_tip->rewardTokenName, rt?rt->val : 0);
                            }
                        }
                    }
                }
            xcase SCMD_DROPTIP_DESIGNER_LIST_ALL:
                {
                    int i;
                    for (i = 0; i < eaSize(&g_DesignerContactTipTypes.designerTips); ++i)
                    {
                        const DesignerContactTip *d_tip = g_DesignerContactTipTypes.designerTips[i];
                        RewardToken *rt = giveRewardToken(e, d_tip->rewardTokenName);
                        conPrintf(client, "Tip Type: %s\nReward token: %s\n\n", d_tip->tokenTypeStr, d_tip->rewardTokenName);
                    }
                }
            xcase SCMD_ROGUE_STATS:
                if(e && e->pl && e->pchar)
                {
                    char *buf = estrTemp();
                    alignmentshift_printRogueStats(e, &buf);
                    conPrintf(client, "%s", buf);
                    estrDestroy(&buf);
                }

            xcase SCMD_ROGUE_ADDPARAGON:
            {
                if (e)
                {
                    giveRewardToken(e, "roguepoints_award_paragon");
                    conPrintf(client, "Paragon Point added.\n");
                }
            }

            xcase SCMD_ROGUE_ADDHERO:
            {
                if (e)
                {
                    giveRewardToken(e, "roguepoints_award_hero");
                    conPrintf(client, "Hero Point added.\n");
                }
            }

            xcase SCMD_ROGUE_ADDVIGILANTE:
            {
                if (e)
                {
                    giveRewardToken(e, "roguepoints_award_vigilante");
                    conPrintf(client, "Vigilante Point added.\n");
                }
            }

            xcase SCMD_ROGUE_ADDROGUE:
            {
                if (e)
                {
                    giveRewardToken(e, "roguepoints_award_rogue");
                    conPrintf(client, "Rogue Point added.\n");
                }
            }

            xcase SCMD_ROGUE_ADDVILLAIN:
            {
                if (e)
                {
                    giveRewardToken(e, "roguepoints_award_villain");
                    conPrintf(client, "Villain Point added.\n");
                }
            }

            xcase SCMD_ROGUE_ADDTYRANT:
            {
                if (e)
                {
                    giveRewardToken(e, "roguepoints_award_tyrant");
                    conPrintf(client, "Tyrant Point added.\n");
                }
            }

            xcase SCMD_ROGUE_RESETPARAGON:
            {
                giveRewardToken(e, "roguepoints_reset_paragon");
                conPrintf(client, "Paragon Points reset.\n");
            }

            xcase SCMD_ROGUE_RESETHERO:
            {
                giveRewardToken(e, "roguepoints_reset_hero");
                conPrintf(client, "Hero Points reset.\n");
            }

            xcase SCMD_ROGUE_RESETVIGILANTE:
            {
                giveRewardToken(e, "roguepoints_reset_vigilante");
                conPrintf(client, "Vigilante Points reset.\n");
            }

            xcase SCMD_ROGUE_RESETROGUE:
            {
                giveRewardToken(e, "roguepoints_reset_rogue");
                conPrintf(client, "Rogue Points reset.\n");
            }

            xcase SCMD_ROGUE_RESETVILLAIN:
            {
                giveRewardToken(e, "roguepoints_reset_villain");
                conPrintf(client, "Villain Points reset.\n");
            }

            xcase SCMD_ROGUE_RESETTYRANT:
            {
                giveRewardToken(e, "roguepoints_reset_tyrant");
                conPrintf(client, "Tyrant Points reset.\n");
            }

            xcase SCMD_ROGUE_SETPARAGON:
            {
                int points = MINMAX(tmp_int, 0, maxAlignmentPoints_Hero);
                setRewardToken(e, "roguepoints_paragon", points);
                conPrintf(client, "Paragon Points set to %d.\n", points);
                LOG_ENT_OLD(e, "GoingRogue:AlignmentPointSet Player set Paragon points to %d.", points);
                ContactMarkAllDirty(e, e->storyInfo);
            }

            xcase SCMD_ROGUE_SETHERO:
            {
                int points = MINMAX(tmp_int, 0, maxAlignmentPoints_Hero);
                setRewardToken(e, "roguepoints_hero", points);
                conPrintf(client, "Hero Points set to %d.\n", points);
                LOG_ENT_OLD(e, "GoingRogue:AlignmentPointSet Player set Hero points to %d.", points);
                ContactMarkAllDirty(e, e->storyInfo);
            }

            xcase SCMD_ROGUE_SETVIGILANTE:
            {
                int points = MINMAX(tmp_int, 0, maxAlignmentPoints_Vigilante);
                setRewardToken(e, "roguepoints_vigilante", points);
                conPrintf(client, "Vigilante Points set to %d.\n", points);
                LOG_ENT_OLD(e, "GoingRogue:AlignmentPointSet Player set Vigilante points to %d.", points);
                ContactMarkAllDirty(e, e->storyInfo);
            }

            xcase SCMD_ROGUE_SETROGUE:
            {
                int points = MINMAX(tmp_int, 0, maxAlignmentPoints_Rogue);
                setRewardToken(e, "roguepoints_rogue", points);
                conPrintf(client, "Rogue Points set to %d.\n", points);
                LOG_ENT_OLD(e, "GoingRogue:AlignmentPointSet Player set Rogue points to %d.", points);
                ContactMarkAllDirty(e, e->storyInfo);
            }

            xcase SCMD_ROGUE_SETVILLAIN:
            {
                int points = MINMAX(tmp_int, 0, maxAlignmentPoints_Villain);
                setRewardToken(e, "roguepoints_villain", points);
                conPrintf(client, "Villain Points set to %d.\n", points);
                LOG_ENT_OLD(e, "GoingRogue:AlignmentPointSet Player set Villain points to %d.", points);
                ContactMarkAllDirty(e, e->storyInfo);
            }

            xcase SCMD_ROGUE_SETTYRANT:
            {
                int points = MINMAX(tmp_int, 0, maxAlignmentPoints_Villain);
                setRewardToken(e, "roguepoints_tyrant", points);
                conPrintf(client, "Tyrant Points set to %d.\n", points);
                LOG_ENT_OLD(e, "GoingRogue:AlignmentPointSet Player set Tyrant points to %d.", points);
                ContactMarkAllDirty(e, e->storyInfo);
            }

            xcase SCMD_INCARNATE_SLOT_ACTIVATE:
                {
                    int retval = IncarnateSlot_activate(e, IncarnateSlot_getByInternalName(tmp_str));
                    conPrintf(client, "Incarnate slot %s %s activated.", tmp_str, (retval ? "has not been" : "has been"));
                }
            xcase SCMD_INCARNATE_SLOT_ACTIVATE_ALL:
                {
                    IncarnateSlot_activateAll(e);
                    conPrintf(client, "All Incarnate slots have been activated.");
                }
            xcase SCMD_INCARNATE_SLOT_DEACTIVATE:
                {
                    int retval = IncarnateSlot_deactivate(e, IncarnateSlot_getByInternalName(tmp_str));
                    conPrintf(client, "Incarnate slot %s %s deactivated.", tmp_str, (retval ? "has not been" : "has been"));
                }
            xcase SCMD_INCARNATE_SLOT_DEACTIVATE_ALL:
                {
                    IncarnateSlot_deactivateAll(e);
                    conPrintf(client, "All Incarnate slots have been deactivated.");
                }
            xcase SCMD_INCARNATE_SLOT_DEBUGPRINT:
                {
                    IncarnateSlot_debugPrint(e, client, IncarnateSlot_getByInternalName(tmp_str));
                }
            xcase SCMD_INCARNATE_SLOT_DEBUGPRINT_ALL:
                {
                    IncarnateSlot_debugPrintAll(e, client);
                }
            xcase SCMD_INCARNATE_SLOT_REWARD_EXPERIENCE:
                {
                    const char *subtype = StaticDefineLookup(ParseIncarnateDefines, tmp_str);

                    if (!subtype)
                    {
                        conPrintf(client, "Improper type string in /incarnateslot_reward_xp");
                        return;
                    }

                    Incarnate_RewardPoints(e, atoi(subtype), tmp_int);
                }
            xcase SCMD_POWERS_DISABLE:
                {
                    const BasePower *ppowBase = powerdict_GetBasePowerByName(&g_PowerDictionary, tmp_str, tmp_str2, tmp_str3);

                    if(ppowBase)
                    {
                        Power *ppow = character_OwnsPower(e->pchar, ppowBase);

                        if(ppow)
                        {
                            character_ManuallyDisablePower(e->pchar, ppow);
                            conPrintf(client, "Power %s has been disabled.", ppow->ppowBase->pchName);
                        }
                    }
                }
            xcase SCMD_POWERS_ENABLE:
                {
                    const BasePower *ppowBase = powerdict_GetBasePowerByName(&g_PowerDictionary, tmp_str, tmp_str2, tmp_str3);

                    if(ppowBase)
                    {
                        Power *ppow = character_OwnsPower(e->pchar, ppowBase);

                        if(ppow)
                        {
                            character_ManuallyEnablePower(e->pchar, ppow);

                            if (ppow->ppowBase->eType == kPowerType_Auto || ppow->ppowBase->eType == kPowerType_GlobalBoost)
                            {
                                character_ActivateAllAutoPowers(e->pchar);
                            }

                            conPrintf(client, "Power %s has been enabled.", ppow->ppowBase->pchName);
                        }
                    }
                }
            xcase SCMD_POWERS_DEBUGPRINT_DISABLED:
                {
                    const BasePower *ppowBase = powerdict_GetBasePowerByName(&g_PowerDictionary, tmp_str, tmp_str2, tmp_str3);

                    if(ppowBase)
                    {
                        Power *ppow = character_OwnsPower(e->pchar, ppowBase);

                        if(ppow)
                        {
                            conPrintf(client, "Power %s is %s.", ppow->ppowBase->pchName, ppow->bDisabledManually ? "disabled manually" : "not disabled manually");
                        }
                    }
                }
            xcase SCMD_INCARNATE_GRANT:
                {
                    int retval = Incarnate_grant(e, IncarnateSlot_getByInternalName(tmp_str), tmp_str2);
                    conPrintf(client, "Incarnate ability %s %s granted.", tmp_str2, (retval ? "has not been" : "has been"));
                }
            xcase SCMD_INCARNATE_REVOKE:
                {
                    int retval = Incarnate_revoke(e, IncarnateSlot_getByInternalName(tmp_str), tmp_str2);
                    conPrintf(client, "Incarnate ability %s %s revoked.", tmp_str2, (retval ? "has not been" : "has been"));
                }
            xcase SCMD_INCARNATE_GRANT_ALL:
                {
                    Incarnate_grantAll(e);
                    conPrintf(client, "All Incarnate abilities have been granted.");
                }
            xcase SCMD_INCARNATE_REVOKE_ALL:
                {
                    Incarnate_revokeAll(e);
                    conPrintf(client, "All Incarnate abilities have been revoked.");
                }
            xcase SCMD_INCARNATE_GRANT_ALL_BY_SLOT:
                {
                    Incarnate_grantAllBySlot(e, IncarnateSlot_getByInternalName(tmp_str));
                    conPrintf(client, "All Incarnate abilities in slot %s have been granted.", tmp_str);
                }
            xcase SCMD_INCARNATE_REVOKE_ALL_BY_SLOT:
                {
                    Incarnate_revokeAllBySlot(e, IncarnateSlot_getByInternalName(tmp_str));
                    conPrintf(client, "All Incarnate abilities in slot %s have been revoked.", tmp_str);
                }
            xcase SCMD_INCARNATE_DEBUGPRINT_HAS_IN_INVENTORY:
                {
                    conPrintf(client, "You %s Incarnate ability %s.", (Incarnate_hasInInventory(e, IncarnateSlot_getByInternalName(tmp_str), tmp_str2) ? "have" : "do not have"), tmp_str2);
                }
            xcase SCMD_INCARNATE_EQUIP:
                {
                    IncarnateSlot incarnateSlot = IncarnateSlot_getByInternalName(tmp_str);
                    IncarnateEquipResult retval = Incarnate_equip(e, incarnateSlot, tmp_str2);
                    const BasePower *incarnateAbility = Incarnate_getBasePower(incarnateSlot, tmp_str2);
                    if (!incarnateAbility)
                    {
                        sendInfoBox(e, INFO_SVR_COM, "IncarnateAbilityNotFound", textStd(tmp_str2));
                    }
                    else if (retval == kIncarnateEquipResult_Success)
                    {
                        sendInfoBox(e, INFO_SVR_COM, "IncarnateAbilityEquipSuccess", textStd(incarnateAbility->pchDisplayName));
                    }
                    else if (retval == kIncarnateEquipResult_FailureSlotCooldown)
                    {
                        sendInfoBox(e, INFO_SVR_COM, "IncarnateAbilityEquipFailureCooldown", textStd(incarnateAbility->pchDisplayName));
                    }
                    else if (retval == kIncarnateEquipResult_FailurePowerRecharging)
                    {
                        sendInfoBox(e, INFO_SVR_COM, "IncarnateAbilityEquipFailureRecharge", textStd(incarnateAbility->pchDisplayName));
                    }
                    else if (retval == kIncarnateEquipResult_FailureInCombat)
                    {
                        sendInfoBox(e, INFO_SVR_COM, "IncarnateAbilityEquipFailureInCombat", textStd(incarnateAbility->pchDisplayName));
                    }
                    else //if (retval == kIncarnateEquipResult_Failure)
                    {
                        sendInfoBox(e, INFO_SVR_COM, "IncarnateAbilityEquipFailure", textStd(incarnateAbility->pchDisplayName));
                    }
                }
            xcase SCMD_INCARNATE_UNEQUIP:
                {
                    IncarnateSlot incarnateSlot = IncarnateSlot_getByInternalName(tmp_str);
                    IncarnateEquipResult retval = Incarnate_unequip(e, incarnateSlot, tmp_str2);
                    const BasePower *incarnateAbility = Incarnate_getBasePower(incarnateSlot, tmp_str2);
                    if (!incarnateAbility)
                    {
                        sendInfoBox(e, INFO_SVR_COM, "IncarnateAbilityNotFound", textStd(tmp_str2));
                    }
                    else if (retval == kIncarnateEquipResult_Success)
                    {
                        sendInfoBox(e, INFO_SVR_COM, "IncarnateAbilityUnequipSuccess", textStd(incarnateAbility->pchDisplayName));
                    }
                    else if (retval == kIncarnateEquipResult_FailureSlotCooldown)
                    {
                        sendInfoBox(e, INFO_SVR_COM, "IncarnateAbilityUnequipFailureCooldown", textStd(incarnateAbility->pchDisplayName));
                    }
                    else if (retval == kIncarnateEquipResult_FailurePowerRecharging)
                    {
                        sendInfoBox(e, INFO_SVR_COM, "IncarnateAbilityUnequipFailureRecharge", textStd(incarnateAbility->pchDisplayName));
                    }
                    else if (retval == kIncarnateEquipResult_FailureInCombat)
                    {
                        sendInfoBox(e, INFO_SVR_COM, "IncarnateAbilityUnequipFailureInCombat", textStd(incarnateAbility->pchDisplayName));
                    }
                    else //if (retval == kIncarnateEquipResult_Failure)
                    {
                        sendInfoBox(e, INFO_SVR_COM, "IncarnateAbilityUnequipFailure", textStd(incarnateAbility->pchDisplayName));
                    }
                }
            xcase SCMD_INCARNATE_UNEQUIP_BY_SLOT:
                {
                    IncarnateEquipResult retval = Incarnate_unequipBySlot(e, IncarnateSlot_getByInternalName(tmp_str));
                    if (retval == kIncarnateEquipResult_Success)
                    {
                        sendInfoBox(e, INFO_SVR_COM, "IncarnateAbilityUnequipBySlot", tmp_str);
                    }
                    else if (retval == kIncarnateEquipResult_FailureSlotCooldown)
                    {
                        sendInfoBox(e, INFO_SVR_COM, "IncarnateAbilityUnequipBySlotFailureCooldown", tmp_str);
                    }
                    else if (retval == kIncarnateEquipResult_FailurePowerRecharging)
                    {
                        sendInfoBox(e, INFO_SVR_COM, "IncarnateAbilityUnequipBySlotFailureRecharge", tmp_str);
                    }
                    else if (retval == kIncarnateEquipResult_FailureInCombat)
                    {
                        sendInfoBox(e, INFO_SVR_COM, "IncarnateAbilityUnequipBySlotFailureInCombat", tmp_str);
                    }
                    else //if (retval == kIncarnateEquipResult_Failure)
                    {
                        sendInfoBox(e, INFO_SVR_COM, "IncarnateAbilityUnequipBySlotFailure", tmp_str);
                    }
                }
            xcase SCMD_INCARNATE_UNEQUIP_ALL:
                {
                    Incarnate_unequipAll(e);
                    sendInfoBox(e, INFO_SVR_COM, "IncarnateAbilityUnequipAll");
                }
            xcase SCMD_INCARNATE_FORCE_EQUIP:
                {
                    int retval = Incarnate_equipForced(e, IncarnateSlot_getByInternalName(tmp_str), tmp_str2);
                    conPrintf(client, "Incarnate ability %s %s equipped.", tmp_str2, retval ? "has not been" : "has been");
                }
            xcase SCMD_INCARNATE_FORCE_UNEQUIP:
                {
                    int retval = Incarnate_unequipForced(e, IncarnateSlot_getByInternalName(tmp_str), tmp_str2);
                    conPrintf(client, "Incarnate ability %s %s unequipped.", tmp_str2, retval ? "has not been" : "has been");
                }
            xcase SCMD_INCARNATE_FORCE_UNEQUIP_BY_SLOT:
                {
                    int retval = Incarnate_unequipBySlotForced(e, IncarnateSlot_getByInternalName(tmp_str));
                    conPrintf(client, "Incarnate slot %s %s cleared.", tmp_str, retval ? "has not been" : "has been");
                }
            xcase SCMD_INCARNATE_DEBUGPRINT_IS_EQUIPPED:
                {
                    conPrintf(client, "Incarnate ability %s %s equipped.", tmp_str2, (Incarnate_isEquipped(e, IncarnateSlot_getByInternalName(tmp_str), tmp_str2) ? "is" : "is not"));
                }
            xcase SCMD_INCARNATE_DEBUGPRINT_GET_EQUIPPED:
                {
                    Power *incarnatePower = Incarnate_getEquipped(e, IncarnateSlot_getByInternalName(tmp_str));
                    if (incarnatePower)
                    {
                        conPrintf(client, "Incarnate ability %s is equipped in slot %s.", incarnatePower->ppowBase->pchDisplayName, tmp_str);
                    }
                    else
                    {
                        conPrintf(client, "You have no Incarnate ability equipped in slot %s.", tmp_str);
                    }
                }
            xcase SCMD_START_ZONE_EVENT:
            {
                ScriptedZoneEventStartFile(tmp_str, tmp_int != 0);
            }
            xcase SCMD_STOP_ZONE_EVENT:
            {
                ScriptedZoneEventStopEvent(tmp_str);
            }
            xcase SCMD_GOTO_STAGE:
            {
                ScriptedZoneEventGotoStage(tmp_str, tmp_str2);
            }
            xcase SCMD_DISABLE_ZONE_EVENT:
            {
                ScriptedZoneEventDisableEvent(tmp_str);
            }
            xcase SCMD_SET_COMBAT_MOD_SHIFT:
            {
                SetCombatModShift(e, tmp_int);
            }
            xcase SCMD_GOTO_MARKER:
            {
                ScriptGotoMarker(e, tmp_str);
            }
            xcase SCMD_CHANGE_TITLE:
            {
                START_PACKET(pak, e, SERVER_NEW_TITLE);
                pktSendBits(pak, 1, e->pchar->iLevel >= ORIGIN_TITLE_LEVEL);
                pktSendBits(pak, 1, accountLoyaltyRewardHasNode(e->pl->loyaltyStatus, "BaseTeleporter"));
//                pktSendBits(pak, 1, authUserMonthsPlayed(e->pl->auth_user_data) >= 24);
                END_PACKET

            }
            xcase SCMD_ZONE_EVENT_KILL_DEBUG:
            {
                ScriptedZoneEventKillDebug(tmp_int);
            }
            xcase SCMD_SET_TURNSTILE_MAP_INFO:
            {
                int eventID = tmp_int;
                int missionID = tmp_int2;
                int eventLocked = tmp_int3;
                turnstileMapserver_setMapInfo(eventID, missionID, eventLocked);
                serverParseClient("turnstile_debug_get_map_info", client);
                chatSendToPlayer( client->entity->db_id, "Remember to reset this info to \"0 0 0\" when done", INFO_SVR_COM, 0 );
            }
            xcase SCMD_GET_TURNSTILE_MAP_INFO:
            {
                int eventID;
                int missionID;
                int eventLocked;
                char buff[64];
                turnstileMapserver_getMapInfo(&eventID, &missionID);
                eventLocked = turnstileMapserver_eventLockedByGroup();
                sprintf(buff, "EventID:%i MissionID:%i EventLocked? %s", eventID, missionID, eventLocked ? "yes" : "no");
                chatSendToPlayer( client->entity->db_id, buff, INFO_SVR_COM, 0 );
            }
//            xcase SCMD_ASSERT_ON_BITSTREAM_ERRORS:
//            {
//                if (e && e->db_id && server_state.debug_db_id && e->db_id == server_state.debug_db_id)
//                {
//                    // Not yet
//                    //START_PACKET(pak, e, SERVER_ENABLE_ASSERT_ON_BS_ERRORS);
//                    //pktSendBitsPack(pak, 1, !!tmp_int);
//                    //END_PACKET
//                }
//            }
            xcase SCMD_ZONE_EVENT_KARMA_MODIFY:
            {
                int errorCode = CharacterKarma_SetKarmaValue(tmp_str, tmp_str2, tmp_int);
                switch (errorCode)
                {
                case 1:
                    chatSendToPlayer( client->entity->db_id, "Failed to set. Invalid var (2nd string)", INFO_SVR_COM, 0 );
                    break;
                case 2:
                    chatSendToPlayer( client->entity->db_id, "Failed to set. Invalid type (1st string)", INFO_SVR_COM, 0 );
                    break;
                case 0:
                    chatSendToPlayer( client->entity->db_id, "Karma value successfully set", INFO_SVR_COM, 0 );
                    break;
                default:
                    assert(0);
                }
            }
            xcase SCMD_POP_HELP:
            {
                sendPopHelp(e, tmp_str);
            }
            xcase SCMD_POP_HELP_RESET:
            {
                if (e->pl)
                {
                    memset(e->pl->popHelpStatus, 0, sizeof(e->pl->popHelpStatus));
                    memset(e->pl->popHelpLatest, 0, sizeof(e->pl->popHelpLatest));
                }
                sendPopHelpReset(e);
            }
            xcase SCMD_DBFLAGS_SHOW:
            {
                if (e)
                {
                    csrPrintDbFlags(client, e);
                }
            }
            xcase SCMD_DBFLAGS_SET:
            {
                if (e)
                {
                    csrSetDbFlag(client, e, tmp_str, tmp_int);
                }
            }
        xcase SCMD_FAKEAUCTION_ADD_SALVAGE:
        {
            const SalvageItem *salv = NULL;
            AuctionItem *item = NULL;
            AuctionMultiContext **multictx = NULL;

            if(tmp_int <= 0)
            {
                conPrintf(client, "Can't sell %d of that!", tmp_int);
                break;
            }

            if(tmp_int2 <= 0)
            {
                conPrintf(client, "Can't have price of %d!", tmp_int2);
                break;
            }

            if(stashFindPointerConst(g_SalvageDict.haItemNames, tmp_str, &salv) && salv)
            {
                stashFindPointer(auctionData_GetDict()->stAuctionItems, auction_identifier(kTrayItemType_Salvage, salv->pchName, 0), &item);
            }

            if(item)
            {
                AuctionPrepFakeItemList(&multictx, item, tmp_int, tmp_int2);
                conPrintf(client, "Pumping %d (%d each) of %s.", tmp_int, tmp_int2, tmp_str);
                AuctionSendFakeItemList(&multictx);
            }
            else
            {
                conPrintf(client, "Couldn't find salvage '%s'!", tmp_str);
            }
        }
        xcase SCMD_FAKEAUCTION_ADD_RECIPE:
        {
            const DetailRecipe *recipe = NULL;
            AuctionItem *item = NULL;
            AuctionMultiContext **multictx = NULL;
            int count;
            const SalvageItem *salv = NULL;

            if(tmp_int <= 0)
            {
                conPrintf(client, "Can't sell %d of that!", tmp_int);
                break;
            }

            if(tmp_int2 <= 0)
            {
                conPrintf(client, "Can't have price of %d!", tmp_int2);
                break;
            }

            if (stashFindPointerConst(g_DetailRecipeDict.haItemNames, tmp_str, &recipe) && recipe)
            {
                stashFindPointer(auctionData_GetDict()->stAuctionItems, auction_identifier(kTrayItemType_Recipe, recipe->pchName, recipe->level), &item);
            }

            if(item)
            {
                AuctionPrepFakeItemList(&multictx, item, tmp_int, tmp_int2);

                // Should we also pump the salvage needed to make this?
                if (tmp_int3)
                {
                    for (count = 0; count < eaSize(&recipe->ppSalvagesRequired); count++)
                    {
                        salv = recipe->ppSalvagesRequired[count]->pSalvage;

                        if (salv && stashFindPointer(auctionData_GetDict()->stAuctionItems, auction_identifier(kTrayItemType_Salvage, salv->pchName, 0), &item))
                        {
                            // As much as needed to build all the recipes.
                            AuctionPrepFakeItemList(&multictx, item, tmp_int * recipe->ppSalvagesRequired[count]->amount, tmp_int2);
                        }
                    }
                }

                conPrintf(client, "Pumping %d (%d each) of %s%s.", tmp_int, tmp_int2, tmp_str, tmp_int3 ? " (plus salvage)" : "");
                AuctionSendFakeItemList(&multictx);
            }
            else
            {
                conPrintf(client, "Couldn't find recipe '%s'!", tmp_str);
            }
        }
        xcase SCMD_FAKEAUCTION_ADD_SET:
        {
            int minlvl;
            int maxlvl;
            char recipename[1000];
            int lvl;
            const DetailRecipe *recipe = NULL;
            AuctionItem *item = NULL;
            AuctionMultiContext **multictx = NULL;
            int count;
            const SalvageItem *salv = NULL;

            if(tmp_int3 <= 0)
            {
                conPrintf(client, "Can't sell %d of that!", tmp_int);
                break;
            }

            if(tmp_int4 <= 0)
            {
                conPrintf(client, "Can't have price of %d!", tmp_int2);
                break;
            }

            if(!stashFindPointerConst(g_DetailRecipeDict.haCategoryNames, tmp_str, NULL))
            {
                conPrintf(client, "Can't find recipe set '%s'!", tmp_str);
                break;
            }

            stashFindInt(g_DetailRecipeDict.haCategoryMinLevel, tmp_str, &minlvl);
            stashFindInt(g_DetailRecipeDict.haCategoryMaxLevel, tmp_str, &maxlvl);

            if(tmp_int > minlvl)
                minlvl = tmp_int;

            if(tmp_int2 < maxlvl)
                maxlvl = tmp_int2;

            if(minlvl > maxlvl)
            {
                conPrintf(client, "Invalid level range %d-%d!", minlvl, maxlvl);
                break;
            }

            for(lvl = minlvl; lvl <= maxlvl; lvl++)
            {
                // Can't point at things from the previous level's run
                recipe = NULL;
                item = NULL;

                sprintf(recipename, "%s_%d", tmp_str, lvl);

                if (stashFindPointerConst(g_DetailRecipeDict.haItemNames, recipename, &recipe) && recipe)
                {
                    stashFindPointer(auctionData_GetDict()->stAuctionItems, auction_identifier(kTrayItemType_Recipe, recipe->pchName, recipe->level), &item);
                }

                if(item)
                {
                    AuctionPrepFakeItemList(&multictx, item, tmp_int3, tmp_int4);

                    for (count = 0; count < eaSize(&recipe->ppSalvagesRequired); count++)
                    {
                        salv = recipe->ppSalvagesRequired[count]->pSalvage;

                        if (salv && stashFindPointer(auctionData_GetDict()->stAuctionItems, auction_identifier(kTrayItemType_Salvage, salv->pchName, 0), &item))
                        {
                            // As much as needed to build all the recipes.
                            AuctionPrepFakeItemList(&multictx, item, tmp_int3 * recipe->ppSalvagesRequired[count]->amount, tmp_int4);
                        }
                    }

                    conPrintf(client, "Pumping %d (%d each) of level %d %s (plus salvage).", tmp_int3, tmp_int4, lvl, tmp_str);
                }

                AuctionSendFakeItemList(&multictx);
            }

            conPrintf(client, "Done");
        }
        xcase SCMD_FAKEAUCTION_PURGE:
        {
            Packet *pak = pktCreateEx(&db_comm_link, DBCLIENT_AUCTION_PURGE_FAKE);
             pktSendBitsAuto(pak, 1);
            pktSendString(pak, "!!FakeName!!");
            pktSend(&pak, &db_comm_link);
        }
        xcase SCMD_LOCK_DOORS:
        {
            server_state.lock_doors = tmp_int ? 1 : 0;
            if(server_state.lock_doors)
                conPrintf(client, "All doors locked.");
            else
                conPrintf(client, "All doors unlocked.");
        }
        xcase SCMD_ENABLEMEMPOOLDEBUG:
            mpEnableSuperFreeDebugging();

        xcase SCMD_TSS_XFER_OUT:
            if (client->entity)
            {
                turnstileMapserver_shardXferOut(client->entity, tmp_str);
            }

        xcase SCMD_TSS_XFER_BACK:
            if (client->entity)
            {
                turnstileMapserver_shardXferBack(client->entity);
            }

        xcase SCMD_GROUP_HIDE:
            if (!groupdbHideForGameplay(groupDefFindWithLoad(tmp_str), tmp_int))
                Errorf("Couldn't find group '%s' for hiding.", tmp_str);

        xcase SCMD_EVENTHISTORY_FIND:
            {
                U32 min_time;
                U32 max_time;

                // The times are elapsed days.
                if (tmp_int && tmp_int > 0)
                    min_time = timerSecondsSince2000() - (tmp_int * 24*60*60);
                if (tmp_int2 && tmp_int2 > 0)
                    max_time = timerSecondsSince2000() - (tmp_int2 * 24*60*60);

                if (max_time && min_time && max_time < min_time)
                {
                    conPrintf(client, "Invalid time range of %d to %d days.", tmp_int, tmp_int2);
                }
                else if ((!tmp_str || !strlen(tmp_str)) &&
                    (!tmp_str2 || !strlen(tmp_str2)))
                {
                    conPrintf(client, "Must specify an authname or character name.");
                }
                else if (min_time || max_time ||
                    (tmp_str && strlen(tmp_str)) ||
                    (tmp_str2 && strlen(tmp_str2)))
                {
                    eventHistory_FindOnDB(client->entity->db_id, min_time, max_time, tmp_str, tmp_str2);
                }
            }

        xcase SCMD_AUTOCOMMAND_ADD:
            {
                autoCommand_add(e, tmp_str, tmp_int, tmp_int2, tmp_int3);
            }
        xcase SCMD_AUTOCOMMAND_DELETE:
            {
                autoCommand_delete(e, tmp_int);
            }
        xcase SCMD_AUTOCOMMAND_SHOWALL:
            {
                autoCommand_showAll(e);
            }
        xcase SCMD_AUTOCOMMAND_SHOWBYCOMMAND:
            {
                autoCommand_showByCommand(e, tmp_str);
            }
        xcase SCMD_AUTOCOMMAND_TESTRUN:
            {
                if (e)
                {
                    int timestamp;
                    int realTime = e->lastAutoCommandRunTime;

                    tmp_str[127] = '\0'; // hack to ensure the timer function doesn't overflow
                    timestamp = timerParseDateStringToSecondsSince2000NoSeconds(tmp_str);

                    conPrintf(client, "Running test on AutoCommands as though last login was time %s", timerMakeDateStringFromSecondsSince2000NoSeconds(timestamp));
                    e->lastAutoCommandRunTime = timestamp;
                    autoCommand_run(client, e, 1);
                    e->lastAutoCommandRunTime = realTime;
                }
            }
        xcase SCMD_REWARD_WEEKLY_TF_ADD:
            {
                Packet *pak_out = pktCreateEx(&db_comm_link, DBCLIENT_MAP_WEEKLY_TF_ADD_TOKEN);
                pktSendString(pak_out, tmp_str);
                pktSend(&pak_out,&db_comm_link);
                conPrintf(client, "Warning: This command does not edit weeklyTF.cfg. If weeklyTF.cfg is reloaded, it will override all changes made with this command.");
            }
        xcase SCMD_REWARD_WEEKLY_TF_REMOVE:
            {
                Packet *pak_out = pktCreateEx(&db_comm_link, DBCLIENT_MAP_WEEKLY_TF_REMOVE_TOKEN);
                pktSendString(pak_out, tmp_str);
                pktSend(&pak_out,&db_comm_link);
                conPrintf(client, "Warning: This command does not edit weeklyTF.cfg. If weeklyTF.cfg is reloaded, it will override all changes made with this command.");
            }
        xcase SCMD_REWARD_WEEKLY_TF_SET_EPOCH_TIME:
            {
                Packet *pak_out = pktCreateEx(&db_comm_link, DBCLIENT_MAP_WEEKLY_TF_SET_EPOCH_TIME);
                pktSendString(pak_out, tmp_str);
                pktSendString(pak_out, tmp_str2);
                pktSend(&pak_out,&db_comm_link);
                conPrintf(client, "Warning: This command does not edit weeklyTF.cfg. If weeklyTF.cfg is reloaded, it will override all changes made with this command.");
            }
        xcase SCMD_REWARD_DB_WEEKLY_TF_UPDATE:
            {
                reward_SetWeeklyTFList(tmp_str);
                reward_UpdateWeeklyTFTokenList();
            }
        xcase SCMD_REWARD_WEEKLY_TF_PRINT_ACTIVE:
            {
                char *buffer = estrTemp();
                reward_PrintWeeklyTFTokenListActive(&buffer);
                conPrintf(client, "%s", buffer);
                estrDestroy(&buffer);
            }
        xcase SCMD_REWARD_WEEKLY_TF_PRINT_ALL:
            {
                char *buffer = estrTemp();
                reward_PrintWeeklyTFTokenListAll(&buffer);
                conPrintf(client, "%s", buffer);
                estrDestroy(&buffer);
            }
        xcase SCMD_CONTACT_DEBUG_OUTPUT_FLOWCHART_FILE:
            {
                ContactDebugOutputFileForFlowchart();
            }
        xcase SCMD_SET_HELPER_STATUS:
            {
                entSetHelperStatus(e, tmp_int);
            }
        xcase SCMD_TEST_LOGGING:
            {
                Packet *pak_out = pktCreateEx(&db_comm_link, DBCLIENT_TEST_LOGGING); 
                pktSend(&pak_out, &db_comm_link);

                if( ArenaSyncConnect() )
                {
                    pak_out = pktCreateEx(&arena_comm_link, ARENACLIENT_TEST_LOG_LEVELS); 
                    pktSend(&pak_out, &arena_comm_link);
                }

                pak_out = dbContainerRelayCreate(RAIDCLIENT_TEST_LOG_LEVELS, CONTAINER_BASERAIDS, 0, 0, 0);
                pktSend(&pak_out, &db_comm_link);

                LOG_ENT( e, LOG_DEPRECATED, LOG_LEVEL_ALERT, LOG_CONSOLE, "Mapserver Log Line Test (With Ent): Alert" );
                LOG( LOG_DEPRECATED, LOG_LEVEL_ALERT, LOG_CONSOLE, "Mapserver Log Line Test: Alert" );
                LOG( LOG_DEPRECATED, LOG_LEVEL_IMPORTANT, LOG_CONSOLE, "Mapserver Log Line Test: Important" );
                LOG( LOG_DEPRECATED, LOG_LEVEL_VERBOSE, LOG_CONSOLE, "Mapserver Log Line Test: Verbose" );
                LOG( LOG_DEPRECATED, LOG_LEVEL_DEPRECATED, LOG_CONSOLE, "Mapserver Log Line Test: Deprecated" );
                LOG( LOG_DEPRECATED, LOG_LEVEL_DEBUG, LOG_CONSOLE, "Mapserver Log Line Test: Debug" );
            }
        xcase SCMD_NX_PHYSX_CONNECT:
            {
#if NOVODEX
                nwConnectToPhysXDebug();
#endif
            }
        xcase SCMD_NX_RAGDOLL_COUNT:
            {
#if !BEACONIZER
                nwSetRagdollCount(tmp_int);
#endif
            }
        xcase SCMD_ACCESSIBLE_CONTACTS_DEBUG_GET_FIRST:
            {
                if (e && e->pl)
                {
                    e->pl->currentAccessibleContactIndex = AccessibleContacts_FindFirst(e);
                    AccessibleContacts_DebugOutput(e);
                }
            }
        xcase SCMD_ACCESSIBLE_CONTACTS_DEBUG_GET_NEXT:
            {
                e->pl->currentAccessibleContactIndex = AccessibleContacts_FindNext(e);
                AccessibleContacts_DebugOutput(e);
            }
        xcase SCMD_ACCESSIBLE_CONTACTS_DEBUG_GET_PREVIOUS:
            {
                e->pl->currentAccessibleContactIndex = AccessibleContacts_FindPrevious(e);
                AccessibleContacts_DebugOutput(e);
            }
        xcase SCMD_ACCESSIBLE_CONTACTS_DEBUG_GET_CURRENT:
            {
                AccessibleContacts_DebugOutput(e);
            }
        xcase SCMD_ACCESSIBLE_CONTACTS_SHOW_CURRENT:
            {
                AccessibleContacts_ShowCurrent(e);
            }
        xcase SCMD_ACCESSIBLE_CONTACTS_SHOW_NEXT:
            {
                e->pl->currentAccessibleContactIndex = AccessibleContacts_FindNext(e);
                AccessibleContacts_ShowCurrent(e);
            }
        xcase SCMD_ACCESSIBLE_CONTACTS_SHOW_PREVIOUS:
            {
                e->pl->currentAccessibleContactIndex = AccessibleContacts_FindPrevious(e);
                AccessibleContacts_ShowCurrent(e);
            }
        xcase SCMD_ACCESSIBLE_CONTACTS_TELEPORT_TO_CURRENT:
            {
                AccessibleContacts_TeleportToCurrent(client, e);
            }
        xcase SCMD_ACCESSIBLE_CONTACTS_SELECT_CURRENT:
            {
                AccessibleContacts_SelectCurrent(client, e);
            }
        xcase SCMD_DEBUG_DISABLE_AUTO_DISMISS:
            {
                g_disableAutoDismiss = tmp_int;
                conPrintf(client, "Debug disable of auto-dismissal of contacts is %s", g_disableAutoDismiss ? "on" : "off");
            }
        xcase SCMD_GET_MARTY_STATUS:
            {
                conPrintf(client, "Marty status:%s", server_state.MARTY_enabled ? "on" : "off");
            }
        xcase SCMD_SET_MARTY_STATUS:
            {
                Packet *pak_out = pktCreateEx(&db_comm_link, DBCLIENT_MAP_SET_MARTY_STATUS);
                pktSendBits(pak_out, 1, tmp_int ? 1: 0);
                pktSend(&pak_out,&db_comm_link);
                conPrintf(client, "Warning: This command does not edit the servers.cfg so it will not be saved if the db restarts");
            }
        xcase SCMD_SET_MARTY_STATUS_RELAY:
            {
                cfg_setMARTY_enabled(tmp_int);
            }
        xcase SCMD_CSR_CLEAR_MARTY_HISTORY:
            {
                csrClearMARTYHistory(admin, player_name,str, tmp_str);
            }
        xcase SCMD_CSR_PRINT_MARTY_HISTORY:
            {
                csrPrintMARTYHistory(admin, player_name, str);
            }
        xcase SCMD_NOKICK:
            {
                if (e && e->pl)
                {
                    e->pl->doNotKick = tmp_int;

                    conPrintf(client, localizedPrintf( e, (e->pl->doNotKick? "Do Not Kick On" : "Do Not Kick Off") ));
                }
            }
        xcase SCMD_FLASHBACK_LEFT_REWARD_APPLY:
            {
                // Assumes this slash command was only run if the player is on a flashback taskforce
                Entity *e;
                int online = 0;

                if (relayCommandStart(NULL, tmp_int, str, &e, &online))
                {
                    const StoryTaskHandle handle = {tmp_int2};
                    const StoryArc *arc = StoryArcDefinition(&handle);
                    StoryReward **reward = arc ? arc->rewardFlashbackLeft : NULL;
                    StoryInfo *info = e->taskforce ? e->taskforce->storyInfo : NULL;
                    StoryTaskInfo *task = info ? info->tasks[0] : NULL;
                    StoryContactInfo *contact = info ? info->contactInfos[0] : NULL;

                    if (reward)
                        StoryRewardApply(*reward, e, info, task, contact, NULL, NULL);
                    relayCommandEnd(&e, &online);
                }
            }
        xcase SCMD_DEBUG_SET_VIP:
            {
                if (e && e->pl)
                {
                    Packet *pak = pktCreateEx(&db_comm_link, DBCLIENT_DEBUG_SET_VIP);
                    pktSendBitsAuto(pak, e->auth_id);
                    pktSendBitsAuto(pak, tmp_int);
                    pktSend(&pak, &db_comm_link);
                }
            }
        xcase SCMD_ACCOUNT_RECOVER_UNSAVED:
            {
                if (e && e->pl)
                {
                    if (eaSize(&e->pl->completed_orders))
                    {
                        conPrintf(client, "Character still has %d completed orders pending save", eaSize(&e->pl->completed_orders));
                    }
                    else
                    {
                        Packet *pak = pktCreateEx(&db_comm_link, DBCLIENT_ACCOUNTSERVER_RECOVER_UNSAVED);
                        pktSendBitsAuto(pak, e->auth_id);
                        pktSendBitsAuto(pak, e->db_id);
                        pktSend(&pak, &db_comm_link);
                    }
                }
            }
        xcase SCMD_OPEN_SALVAGE_BY_NAME:
            {
                if (e)
                {
                    openSalvageByName(e, tmp_str);
                }
            }
        xcase SCMD_SUPPORT_HOME:
            {
                START_PACKET(pak, e, SERVER_SUPPORT_HOME);
                END_PACKET
            }
        xcase SCMD_SUPPORT_KB:
            {
                START_PACKET(pak, e, SERVER_SUPPORT_KB);
                pktSendBitsAuto(pak, tmp_int);
                END_PACKET
            }
        xcase SCMD_SHOW_LUA_LIB:
            {
                char buf[1000];
                sprintf(buf, "Listing functions for %s Lua library:\n", tmp_str);
                //Gather functions
                printCOHLuaLibraries(tmp_str, buf);
                strcat(buf, "\n");
                conPrintf(client, buf);
            }
        xcase SCMD_SHOW_LUA_ALL:
            {
                char buf[1000] = "Listing all Lua library functions:\n";
                //Gather functions
                printCOHLuaLibraries(NULL, buf);
                strcat(buf, "\n");
                conPrintf(client, buf);
            }
        xcase SCMD_NEW_FEATURE_OPEN:
            {
                runNewFeatureCommand(client, tmp_int);
            }
        xcase SCMD_DISPLAY_PRODUCT_PAGE:
            {
                START_PACKET(pak, e, SERVER_DISPLAY_PRODUCT_PAGE);
                pktSendString(pak, tmp_str);
                END_PACKET
            }
        xcase SCMD_FORCE_QUEUE_FOR_EVENT:
            {
                START_PACKET(pak, e, SERVER_FORCE_QUEUE_FOR_EVENT);
                pktSendString(pak, tmp_str);
                END_PACKET
            }
        xcase SCMD_SET_ZMQ_CONNECT_STATE:
            {
                if (log_comm_link.connected)
                {
                    Packet *pak = pktCreateEx(&log_comm_link, LOGCLIENT_ZEROMQ_SET_CONNECT_STATE);
                    pktSendBitsAuto(pak, tmp_int);
                    pktSend(&pak, &log_comm_link);
                    conPrintf(client, "Warning: This command does not edit servers.cfg, so it will not be saved if the logserver restarts");
                }
                else
                {
                    conPrintf(client, "Slash command failed. Communication link to logserver is not connected.");
                }
            }
        xcase SCMD_GET_ZMQ_STATUS:
            {
                if (log_comm_link.connected)
                {
                    Packet *pak = pktCreateEx(&log_comm_link, LOGCLIENT_ZEROMQ_GET_STATUS);
                    pktSendBitsAuto(pak, e->db_id);
                    pktSend(&pak, &log_comm_link);
                }
                else
                {
                    conPrintf(client, "Slash command failed. Communication link to logserver is not connected.");
                }                
            }
        xcase SCMD_BIN_MAP:
			groupBinCurrent(world_name);            
        xdefault:
            {
            }
    }

    // Clear out the strings so they don't hang around.
    *tmp_str= *tmp_str2 = *tmp_str3 = '\0';
}

//----------------------------------------
//  handle a command from the statserver, usually in response to
//----------------------------------------
static void sgrpstatserverCommand(Cmd *cmd, ClientLink *client, char *source_str, Entity *e, CmdContext *output)
{
    char *str = source_str;
    ClientLink *admin = client;
    AppServerCmdState *state = &g_appserver_cmd_state;

    // ----------
    // mask global static variables

    int tmp_int = state->ints[0];
    int tmp_int2 = state->ints[1];
    int tmp_int3 = state->ints[2];
    int tmp_int4 = state->ints[3];
    int tmp_int5 = state->ints[4];
    int tmp_int6 = state->ints[5];
    int tmp_int7 = state->ints[6];
    char *tmp_str = state->strs[0];
    char *tmp_str2 = state->strs[1];
    int idEntMessage = e ? e->db_id : 0;
    int idSgrp = state->idSgrp > 0 ? state->idSgrp : (e ? e->supergroup_id : 0);

    // check for a redirect:
    // if the message_dbid is set, use that as the response id
    if( client && client->message_dbid && client->entity->db_id != client->message_dbid)
    {
        idEntMessage = client->message_dbid;
    }

    if (cmd->access_level > ACCESS_USER && e) {
        LOG((cmd->access_level < ACCESS_DEBUG) ? LOG_ACCESS_CMDS : LOG_INTERNAL_CMDS, LOG_LEVEL_IMPORTANT, 0, "%s:\"%s\"", e->name, source_str);
    }

    switch(cmd->num)
    {
    case SGRPSTATCMD_PAYRENTFREE:
        tmp_int = 0;            // fall through to setrent command
    case SGRPSTATCMD_SETRENT:
    {
        Supergroup sg = {0};
        if( sgroup_LockAndLoad( idSgrp, &sg ))
        {
            int rentDue = tmp_int;

            // ----------
            // set rent due

            sg.upkeep.prtgRentDue = rentDue;

            // ----------
            // if we're paying off the rent, reset the due date

            if( rentDue == 0 )
            {
                sg.upkeep.secsRentDueDate = dbSecondsSince2000() + g_baseUpkeep.periodRent;
            }

            // ----------
            // finally, unlock

            sgroup_UpdateUnlock( idSgrp, &sg );
        }
        else
        {
            conPrintf(client, clientPrintf(client, "SGStatLockFailed",idSgrp));
        }
    }
    break;
    case SGRPSTATCMD_SETRENTOVERDUE:
    {
        Supergroup sg = {0};
        if( sgroup_LockAndLoad( idSgrp, &sg ))
        {
            int periodsBack = tmp_int;
            sg.upkeep.secsRentDueDate = dbSecondsSince2000() - g_baseUpkeep.periodRent*periodsBack;

            // ----------
            // finally,

            // unlock
            sgroup_UpdateUnlock( idSgrp, &sg );

            // send update to server
            SgrpStat_SendPassthru(idEntMessage,idSgrp, "sgstat_sgrp_update %d", idSgrp);
        }
        else
        {
            conPrintf(client, clientPrintf(client, "SGStatLockFailed",idSgrp));
        }
    }
    break;
    case SGRPSTATCMD_HAMMER:
    {
        int i,j;
        for( i = 0; i < tmp_int2; ++i ) 
        {
            for( j = 0; j < tmp_int; ++j ) 
            {
                
            }
        }
    }
    break;
    case SGRPSTATCMD_STATADJ:
    case SGRPSTATCMD_PASSTHRU:
    default:
    {
        Packet *pak_out = pktCreateEx(&db_comm_link, DBCLIENT_SEND_STATSERVER_CMD);
        pktSendBitsPack(pak_out, 1, idEntMessage); // okay if 0, won't get response
        pktSendBitsPack(pak_out, 1, idSgrp);
        pktSendString(pak_out, str);
        pktSend(&pak_out,&db_comm_link);
    }
    break;
    }

    // ----------
    // clear out the state

    ZeroStruct(&g_appserver_cmd_state);
}

static void commandFromStatserver(Cmd *cmd, ClientLink *client, char *source_str, Entity *debugEnt , CmdContext *output)
{
    char *str = source_str;
    ClientLink *admin = client;
    AppServerCmdState *state = &g_appserver_cmd_state;

    // ----------
    // mask global static variables

    int idEnt = state->idEnt;
    Entity *e = idEnt > 0 ? findEntOrRelayById(admin, idEnt, str, NULL) : debugEnt;

    int tmp_int = state->ints[0];
    int tmp_int2 = state->ints[1];
    int tmp_int3 = state->ints[2];
    int tmp_int4 = state->ints[3];
    int tmp_int5 = state->ints[4];
    int tmp_int6 = state->ints[5];
    int tmp_int7 = state->ints[6];
    char *tmp_str = state->strs[0];
    char *tmp_str2 = state->strs[1];

    switch(cmd->num)
    {
    case SCMD_SGRP_BASE_RENTDUE_FROMENTID_RELAY:
    {
        Entity* e;
        int db_id;

        db_id = tmp_int;
        e = findEntOrRelayById(admin, db_id, str, NULL);
        if( e && e->supergroup )
        {
             char *msg = NULL;
            int p = sgroup_nUpkeepPeriodsLate( e->supergroup );
            const BaseUpkeepPeriod *bup = sgroup_GetUpkeepPeriod( e->supergroup );

            if( bup && p > 0)
            {
                int rentDue = e->supergroup->upkeep.prtgRentDue;

                // ----------
                // build and send the chat string

                estrCreate(&msg);

                // tell player what they owe
                estrPrintCharString(&msg, (p == 1 ? localizedPrintf(e,"SgBaseFirstRentDueMsg", rentDue)
                                                    : localizedPrintf(e,"SgBaseNthRentDueMsg", rentDue, p)));

                // append any shutdown messages
                if( bup->shutBaseDown )
                {
                    estrConcatf(&msg," %s", localizedPrintf(e,"SgBaseRentDueMsgShutDown"));
                }
                if( bup->denyBaseEntry )
                {
                    estrConcatf(&msg," %s", localizedPrintf(e,"SgBaseRentDueMsgDenyEntry"));
                }

                // send msg to sgrp chat window
                chatSendToPlayer(db_id, msg, INFO_SUPERGROUP_COM , 0);

                // ----------
                // update the base, if it applies

                if( bup->shutBaseDown )
                {

                }
                if( bup->denyBaseEntry )
                {
                    // nothing, but in future boot from the base
                }

                // finally
                estrDestroy(&msg);
            }
        }
    }
    break;
    case SERVER_SGRPSTATCMD_STATADJ:
    case SERVER_SGRPSTATCMD_CONPRINTF_RESPONSE:
    {
        if( e )
        {
            char *src = state->strs[0];
            char *iStr;
            char *resp = NULL;

            // ----------
            // the statserver doesn't have menu messages loaded, so parse the response
            // string and look for any braces at word boundaries. this is a quick halfway
            // measure to get some translation into the statserver strings. may need to do
            // more later.

            estrCreateEx(&resp, strlen(str));
            
            for(iStr = src; iStr && *iStr; ++iStr)
            {
                bool addChar = true;

                // see if the translation character is matched at start of word
                if(*iStr == '{')
                {
                    char *end = strchr(iStr,'}'); // no recursive '{' handling here
                    if( end )
                    {
                        iStr++;
                        *end = 0;
                        estrConcatCharString(&resp, clientPrintf(e->client, iStr));
                        iStr = end;
                        addChar = false;
                    }
                }

                if(addChar)
                {
                    estrConcatChar(&resp, *iStr);
                }
            }

            conPrintf( e->client, localizedPrintf(e, resp));

            estrDestroy(&resp);

        }
    }
    break;
    case SERVER_SGRPSTATCMD_CMDFAILED_RESPONSE:
    case SERVER_SGRPSTATCMD_CMDFAILED2_RESPONSE:
    {
        if( e )
        {
            conPrintf( e->client, localizedPrintf(e, "SgrpStatCmdFailed", localizedPrintf( e, state->strs[0], state->strs[1] )));
        }
    }
    break;
    case SERVER_SGRPSTATCMD_LOCALIZED_MESSAGE:
        if(e)
            chatSendToPlayer(e->db_id, localizedPrintf(e, state->strs[0], state->strs[1]), state->ints[0], 0);
    break;
    case SERVER_SGRPSTATCMD_STATSHOW_RESPONSE:
    {
        if( e )
        {
            char *pchStat = (*state->strs[0] == '*') ? "" : state->strs[0];
            char *valIdxNameTuples = state->strs[1];

            conPrintf(client, "%s\n", localizedPrintf(e,"BadgesStatsFor", e->name, e->db_id, db_state.server_name));

            if(*pchStat!='\0')
                conPrintf(e->client, "   %s\n", localizedPrintf(e,"Searchingfor", pchStat));

            while(valIdxNameTuples)
            {
                int iVal;
                int idx;
                if( 2 == sscanf(valIdxNameTuples,"%d,%d;", &iVal, &idx) )
                {
                    char *pchName;
                    if( stashIntFindPointer(g_BadgeStatsSgroup.statnameFromId, idx, &pchName) )
                    {
                        conPrintf(e->client, "   %6d %s (%d)\n", iVal, pchName, idx);
                    }
                }
                valIdxNameTuples = strchr( valIdxNameTuples+1, ';' );
                if(valIdxNameTuples)
                {
                    valIdxNameTuples++;
                }
            }
        }
    }
    break;
    default:
    {
    }
    break;
    }

    // ----------
    // clear out the state

    ZeroStruct(&g_appserver_cmd_state);
}

static void auctionserverCommand(Cmd *cmd, ClientLink *client, char *source_str, Entity *e, CmdContext *output)
{
    int idEntMessage = e ? e->db_id : 0;
    Packet *pak_out = NULL;

    pak_out = ent_AuctionPktCreateEx(e, DBCLIENT_SEND_AUCTIONSERVER_CMD);
    if(!pak_out)
        return;

    pktSendBitsAuto(pak_out,SAFE_MEMBER(client,message_dbid));
    pktSendString(pak_out,source_str);
    pktSend(&pak_out,&db_comm_link);
}

static void commandFromAuction(Cmd *cmd, ClientLink *client, char *source_str, Entity *ent, CmdContext *output)
{
    AppServerCmdState *state = &g_appserver_cmd_state;
    
    // ----------
    // mask global static variables

    // int idEnt = state->idEnt;
    // Entity *e = idEnt > 0 ? entFromDbIdEvenSleeping(idEnt) : debugEnt;
    Entity *e = client ? client->entity : ent;

    int tmp_int = state->ints[0];
    int tmp_int2 = state->ints[1];
    int tmp_int3 = state->ints[2];
    int tmp_int4 = state->ints[3];
    int tmp_int5 = state->ints[4];
    int tmp_int6 = state->ints[5];
    int tmp_int7 = state->ints[6];
    char *tmp_str = state->strs[0];
    char *tmp_str2 = state->strs[1];

    if(!client && e)
        client = e->client;

    switch ( cmd->num )
    {
    case AuctionCmdRes_GetInv: 
        {
            conPrintf(client,"auction inventory: %s",unescapeString(tmp_str));
        }
        break;
    case AuctionCmdRes_Err: 
        {
            if(e && e->access_level)
                conPrintf(client,"error: %s",unescapeString(tmp_str));
        }
        break;
    case AuctionCmdRes_LoginUpdate:
        {
            AuctionInventory *inv = NULL;
            int i;
            int sold = 0;
            int bought = 0;
            
            if(!e) // this command just generates a message for the entity
                break;

            AuctionInventory_FromStr(&inv,tmp_str);
            if(!inv)
                break;
            
            for( i = eaSize(&inv->items) - 1; i >= 0; --i)
            {
                AuctionInvItem *itm = inv->items[i];
                if(!itm)
                    continue;
                if(itm->auction_status == AuctionInvItemStatus_ForSale || itm->auction_status == AuctionInvItemStatus_Sold)
                    sold += itm->amtOther;
                if(itm->auction_status == AuctionInvItemStatus_Bidding || itm->auction_status == AuctionInvItemStatus_Bought)
                    bought += itm->amtStored;
            }

            if(sold || bought)
                sendInfoBox(e, INFO_AUCTION, "Auction_LoginUpdate", sold, bought);
                
            AuctionInventory_Destroy(inv);
        }
        break;
    default:
        break;
    };
}



static void commandFromAccount(Cmd *cmd, ClientLink *client, char *source_str, Entity *ent , CmdContext *output)
{
    AppServerCmdState *state = &g_appserver_cmd_state;
    
    // ----------
    // mask global static variables

    // int idEnt = state->idEnt;
    // Entity *e = idEnt > 0 ? entFromDbIdEvenSleeping(idEnt) : debugEnt;
    Entity *e = client ? client->entity : ent;

    int tmp_int = state->ints[0];
    int tmp_int2 = state->ints[1];
    int tmp_int3 = state->ints[2];
    int tmp_int4 = state->ints[3];
    int tmp_int5 = state->ints[4];
    int tmp_int6 = state->ints[5];
    int tmp_int7 = state->ints[6];
    char *tmp_str = state->strs[0];
    char *tmp_str2 = state->strs[1];

    if(!client && e)
        client = e->client;

    switch ( cmd->num )
    {
    case AuctionCmdRes_Err: 
    {
        if(e && e->access_level)
            conPrintf(client,"%s",unescapeString(tmp_str));
        break;
    }
    break;
    default:
        break;
    }
}

int serverFindSkyByName(const char *name)
{
    int i, count = eaSize(&scene_info.sky_names);

    for (i = 0; i < count; i++)
        if (stricmp(name, scene_info.sky_names[i]) == 0)
            return i;

    return -1;
}

void serverSetSkyFade(int sky1, int sky2, F32 weight) // 0.0 = all sky1
{
    server_state.skyFade1 = sky1;
    server_state.skyFade2 = sky2;
    server_state.skyFadeWeight = weight;

    server_state.skyFadeInteresting = server_state.skyFadeWeight!=0.0 &&
        (server_state.skyFade1!=0 || server_state.skyFade2!=0) ||
        server_state.skyFade1!=0;
}

static int s_remoteMetaWarning(const char *fmt, va_list va)
{
    CallInfo *call = meta_getCallInfo();
    if(call->forward_id)
    {
        Entity *e = entFromDbIdEvenSleeping(call->forward_id);
        if(e)
        {
            char *str = estrTemp();
            estrConcatfv(&str, fmt, va);
            dbLog("cheater", e, "%s", str);
            estrDestroy(&str);
        }
    }
    return 0;
}


void serverParseClientEx(const char *cmd_str, ClientLink *client, int access_level)
{
    Cmd*            cmd;
    CmdContext        output = {0};
    Entity*            e = NULL;
    static            char    str[20000];
    char *pch;

    while(cmd_str)
    {
        int iCntQuote=1;

        pch=strstr(cmd_str, "$$");
        while(pch!=NULL && (iCntQuote&1))
        {
            const char *pchQuote=cmd_str;
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
            *pch = '\0'; // TODO: Is this safe? cmd_str isn't const... but
        }

        expandVars(cmd_str,str,sizeof(str)-1,client);
        if (client)
        {
            e = clientGetControlledEntity(client);
            cmdOldAddDataSwap(&output,&control_state,sizeof(ControlState),&client->controls);
            output.access_level = e?e->access_level:ACCESS_USER;
            MAX1(output.access_level,access_level);
        }
        else
        {
            output.access_level = ACCESS_INTERNAL; //JE: Had to change this for all of the chat commands (i.e. sginvite_long) to work  Is this the right thing to do?
        }

        output.found_cmd = false;
        cmd = cmdOldRead(&server_cmdlist,str,&output);

        cmdOldClearDataSwap(&output);

        if(cmd)
        {
            PERFINFO_AUTO_START("cmdServer", 1);
            PERFINFO_AUTO_START_STATIC(cmd->name, &cmd->perfInfo, 1);


            if(INRANGE( cmd->num, SCMD_TOTAL_COMMANDS, CHATCMD_TOTAL_COMMANDS))
            {
                if (client) {
                    LOG_ENT( client->entity, LOG_CHAT, LOG_LEVEL_IMPORTANT, 0, "Chat Command: %s", str );
                }

                chatCommand( cmd, client, str );
            }
            else if( INRANGE( cmd->num, CHATCMD_TOTAL_COMMANDS, SGRPSTATCMD_TOTAL_COMMANDS))
            {
                if (client) {
                    LOG_ENT(client->entity, LOG_CHAT, LOG_LEVEL_IMPORTANT, 0, "StatServer Command: %s", str );
                }

                sgrpstatserverCommand(cmd, client, str, e, &output);
            }
            else if(INRANGE(cmd->num,SGRPSTATCMD_TOTAL_COMMANDS,SERVER_SGRPSTATCMD_TOTAL_COMMANDS))
            {
                if (client) {
                    LOG_ENT( client->entity, LOG_CHAT, LOG_LEVEL_IMPORTANT, 0, "Command From StatServer: %s", str );
                }

                commandFromStatserver(cmd, client, str, e, &output);
            }
            else if( INRANGE( cmd->num, ClientAuctionCmd_None,ClientAuctionCmd_Count))
            {
                if (client) {
                    LOG_ENT( client->entity, LOG_CHAT, LOG_LEVEL_IMPORTANT, 0, "Auction Command: %s", str );
                }

                auctionserverCommand(cmd, client, str, e, &output);
            }
            else if(INRANGE(cmd->num,AuctionCmdRes_None,AuctionCmdRes_Count))
            {
                if (client) {
                    LOG_ENT( client->entity, LOG_CHAT, LOG_LEVEL_IMPORTANT, 0, "Command From Auction: %s", str );
                }

                commandFromAuction(cmd, client, str, e, &output);
            }
            else if( INRANGE( cmd->num, ClientAccountCmd_None,ClientAccountCmd_Count))
            {
                int message_dbid = SAFE_MEMBER(client,message_dbid) ? client->message_dbid : SAFE_MEMBER(e,db_id);
                if (client) {
                    LOG_ENT( client->entity, LOG_CHAT, LOG_LEVEL_IMPORTANT, 0, "Account Command: %s", str );
                }

                AccountServer_ClientCommand(cmd, message_dbid, SAFE_MEMBER(e,auth_id), SAFE_MEMBER(e,db_id), str);
            }
            else if(INRANGE(cmd->num,AccountCmdRes_None,AccountCmdRes_Count))
            {
                if (client) {
                    LOG_ENT( client->entity, LOG_CHAT, LOG_LEVEL_IMPORTANT, 0, "Command From Account: %s", str );
                }

                commandFromAccount(cmd, client, str, e, &output);
            }
            else if (!output.msg[0] || cmd->flags & CMDF_RETURNONERROR)
            {
                //this prevents test clients from spamming "Command: client_pos_id 7" log messages to the metrics system
                #if !defined(TEST_CLIENT)
                    if (client) {
                        LOG_ENT( client->entity, LOG_CHAT, LOG_LEVEL_IMPORTANT, 0, "Command: %s", str );
                    }
                #endif

                serverExecCmd(cmd, client, str, e, &output);
            }

            PERFINFO_AUTO_STOP(); // Stop the command.
            PERFINFO_AUTO_STOP(); // Stop for the command group.
        }
        else if (e)
        {
            meta_handleConsole(str, e->db_id, output.access_level, SAFESTR(output.msg));
        }

        if (output.msg[0] && client && client->entity && (client->message_dbid == client->entity->db_id || strstri(output.msg, textStd("UnknownCommandString", ""))))
            conPrintf(client, "%s", output.msg);
        else if (output.msg[0] && !client) // make sure internal messages get printed to console
            printf("%s", output.msg);

        if(pch!=NULL)
        {
            *pch = '$'; // I have to go wash my hands now.
            pch += 2;
        }

        // Go to the next command
        cmd_str=pch;
    }
}

//int debug_db_id(void)
//{
//    return server_state.debug_db_id;
//}

void cmdOldServerParsef(ClientLink *client, char const *fmt, ...)
{
    char str[20000];
    va_list ap;
    int ret;

    va_start(ap, fmt);
    ret = _vsnprintf(str, ARRAY_SIZE(str)-1, fmt, ap);
    va_end(ap);
    if (ret==-1)
        str[ARRAY_SIZE(str)-1] = '\0';
    serverParseClientEx(str, client, 10);
}




int cmdOldServerSend(Packet *pak,int all)
{
    return cmdOldSend(pak,server_visible_cmds,0,all);
}

void cmdOldServerInitShortcuts()
{
    int        i;
    Cmd        *cmds;

    cmds = control_cmds;
    for(i=0;cmds[i].name;i++)
        cmdOldAddShortcut(cmds[i].name);
    cmds = server_visible_cmds;
    for(i=0;cmds[i].name;i++)
        cmdOldAddShortcut(cmds[i].name);

}

void cmdOldServerSendShortcuts(Packet *pak)
{
    int        i;
    char    *s;

    for(i=1;;i++)
    {
        s = cmdOldShortcutName(i);
        if (!s)
            break;
        pktSendBitsPack(pak,1,i);
        pktSendString(pak,s);
    }
    pktSendBitsPack(pak,1,-1);
}

void cmdOldServerSendCommands(Packet *pak, int access_level)
{
    int        i,num = sizeof(server_cmds)/sizeof(Cmd) - 1;
    StringArray accessible_svr_cmds;

    eaCreate(&accessible_svr_cmds);

    // figure out which commands we have access to
    for(i=0;i<num;i++)
    {
        if ( server_cmds[i].access_level <= access_level && !(server_cmds[i].flags & CMDF_HIDEPRINT) )
        {
            eaPush(&accessible_svr_cmds, server_cmds[i].name);
        }
    }

    num = eaSize(&accessible_svr_cmds);

    // send the commands
    pktSendBitsPack(pak,1,num);
    for(i=0;i<num;i++)
    {
        pktSendString(pak,accessible_svr_cmds[i]);
    }

    eaDestroy(&accessible_svr_cmds);

}

void serverSynchTime()
{
    U32            time;
    double        tf;
    char        buf[100];

    time = dbSecondsSince2000();
    tf = time * DAY_TIMESCALE / (60 * 60);
    tf = fmod(tf,24.f);
    sprintf(buf,"time %f",tf);
    cmdOldServerReadInternal(buf);
}

void cmdOldServerReadInternal(char *buf)
{
    static CmdContext context;
    context.access_level = ACCESS_INTERNAL;
    cmdOldRead(&server_cmdlist,buf,&context);
}

void cmdOldServerHandleRelay(char *msg)
{
    char    *args[100],*line2;
    int        count;

    count = tokenize_line(msg,args,&line2);
    if (!count)
        return;
    if (stricmp(args[0],"silenceall")==0)
    {
        db_state.silence_all = atoi(args[1]);
    }
    else if (stricmp(args[0],"cmdrelay")==0)
    {
        serverParseClient(line2,0);
    }
    else if (stricmp(args[0],"server_sgstat_cmd")==0)
    {
        // indirectly set the entity id
        g_appserver_cmd_state.idEnt = (count == 2 && args[1]) ? atoi(args[1]) : 0;
        serverParseClient(line2,0);
    }
    else if (stricmp(args[0],"cmdrelay_dbid")==0)
    {
        int ent_dbid = (count >= 2 && args[1]) ? atoi(args[1]) : 0;
        Entity *ent = ent_dbid ? entFromDbIdEvenSleeping(ent_dbid) : NULL;
        int message_dbid = (count >= 3 && args[2]) ? atoi(args[2]) : 0;

        g_appserver_cmd_state.idEnt = ent_dbid; // not being used, but just in case

        if(ent)//devasser
        {
            // see cmdservercsr.c csrCsrOffline()
            ClientLink clientLink = {0};
            bool forced_client = false;

            if(!ent->client)
            {
                // Need to have a client with a controlled entity in order to run serverParseClient
                svrClientLinkInitialize(&clientLink, NULL);
                clientLink.entity = ent;
                ent->client = &clientLink;
                forced_client = true;
            }

            if (ent->client->message_dbid == 0)
                ent->client->message_dbid = message_dbid;

            serverParseClientEx(line2, ent->client, ACCESS_INTERNAL);

            if (ent->client->message_dbid == message_dbid)
                ent->client->message_dbid = 0;

            if (forced_client)
            {
                clientLink.entity = NULL;
                ent->client = NULL;
                svrClientLinkDeinitialize(&clientLink);
            }
        }
        else if (ent_dbid)
        {
            dbForceRelayCmdToMapByEnt(ent_dbid, line2);
        }
    }
    else if (stricmp(args[0],"cmdrelay_db_no_ent")==0)
    {
        serverParseClientEx(line2, 0, ACCESS_INTERNAL);
    }
}


static void cmdOldServerCheckCmds()
{
    int i, j, k, m;
    for (j=0; server_cmdlist.groups[j].cmds; j++) {
        Cmd *cmds = server_cmdlist.groups[j].cmds;
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
Found command \"%s\" using tmp_* in cmdServer.c but not marked as CMDF_HIDEVARS.\nThis means that if someone types the \
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

void cmdOldServerInit()
{
    //db_state.server_synch_time_cb    = serverSynchTime;
    //db_state.relay_cmd_cb            = cmdServerHandleRelay;
    cmdOldInit(server_cmds);
    cmdOldInit(chat_cmds);
    cmdOldInit(client_sgstat_cmds);
    cmdOldInit(server_sgstat_cmds);
    cmdOldInit(g_auction_cmds);
    cmdOldInit(g_auction_res);
    cmdOldInit(g_accountserver_cmds_server);

    cmdOldServerCheckCmds();
    cmdOldChatCheckCmds();

    missionserver_meta_register();
    meta_registerRemoteMessager(s_remoteMetaWarning);
}


int cfg_IsBetaShard(void)
{
    return server_state.isBetaShard;
}

void cfg_setIsBetaShard(int data)
{
    server_state.isBetaShard = data;
}

void cfg_setMARTY_enabled(int data)
{
    server_state.MARTY_enabled = data;
}

void cfg_setIsAutoBuyProducts(bool b)
{
    Cmd* cmd = g_accountserver_cmds_server;
    while(cmd->name)
    {
        if (strcmp(cmd->name, "acc_debug_buyproduct") == 0)
        {
            cmd->access_level = b ? 0 : 6;
            break;
        }
        cmd++;
    }
}

static void cmdCfgReloadCallback(const char *relPath, int when)
{
    cmdCfgLoad();
}

typedef struct CmdConfigLine
{
    char *command;
    int access_level;
} CmdConfigLine;

typedef struct CmdConfig
{
    CmdConfigLine **lines;
} CmdConfig;

static TokenizerParseInfo ParseCmdConfigLine[] = {
    { "",            TOK_STRUCTPARAM | TOK_STRING(CmdConfigLine, command, 0)    },
    { "",            TOK_STRUCTPARAM | TOK_INT(CmdConfigLine, access_level, 0) },
    { "\n",            TOK_END, 0 },
    { "", 0, 0 }
};

static TokenizerParseInfo ParseCmdConfig[] = {
    { "Cmd",        TOK_STRUCT(CmdConfig, lines, ParseCmdConfigLine), },
    { "", 0, 0 }
};

void cmdCfgLoad()
{
    const char* relFilename = "server/db/commands.cfg";
    static bool loaded_once = false;
    CmdConfig config = { 0 };
    int i;

    printf("Loading %s\n", relFilename);

    StructInitFields(ParseCmdConfig, &config);
    if (ParserLoadFiles(NULL, relFilename, NULL, PARSER_EMPTYOK | PARSER_OPTIONALFLAG, ParseCmdConfig, &config, NULL, NULL, NULL))
    {
        for (i = 0; i < eaSize(&config.lines); ++i)
        {
            CmdConfigLine *ln = config.lines[i];
            Cmd *c = cmdOldListFind(&server_cmdlist, ln->command, NULL, NULL);

            if (c)
                c->access_level = ln->access_level;
        }
    }
    StructClear(ParseCmdConfig, &config);

    if (!loaded_once) {
        FolderCacheSetCallback(FOLDER_CACHE_CALLBACK_UPDATE|FOLDER_CACHE_CALLBACK_CAN_USE_SHARED_MEM, relFilename, cmdCfgReloadCallback);
    }

    loaded_once = true;
}
