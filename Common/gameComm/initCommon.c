//
//
// initCommon.h - shared initialization functions
//-------------------------------------------------------------------------------------------------


#include <utilitieslib/network/netio.h>
#include "varutils.h"
#include "fxinfo.h"
#include "entity/entVarUpdate.h"
#include <utilitieslib/utils/timing.h>
#include "gameData/BodyPart.h"    // for bpReadBodyPartFiles()
#include "gameComm/Npc.h"
#include "gameComm/VillainDef.h"
#include <utilitieslib/utils/error.h>
#include "fxinfo.h"
#include <utilitieslib/utils/FolderCache.h>
#include <utilitieslib/utils/fileutil.h>
#include "seq/seqtype.h"
#include "cmdparse/cmdcommon.h"
#include "account/AccountData.h"

#if SERVER
    #include "Reward.h"
    #include "TeamReward.h"
    #include "entity/entGameActions.h"
    #include "cmdparse/cmdserver.h"
#endif


#if CLIENT
    #include "gameComm/initClient.h"
    #include "clientcomm/clientcomm.h"
    #include "UI/uiDialog.h"
    #include "entity/entclient.h"
    #include "player/player.h"
    #include "UI/uiCostume.h"
    #include "fxbhvr.h"
    #include "graphics/FX/fxfluid.h"
    #include "graphics/FX/fxcapes.h"
    #include "cmdparse/cmdgame.h"
#endif


void cacheRelevantFolders() {
    writeConsole(OUTPUT_DEBUG, "Caching relevant folders");
    FolderCacheRequestTree(folder_cache, "Defs"); // If we're in dynamic mode, this will load this tree for faster file access
    FolderCacheRequestTree(folder_cache, "Menu");
    if (!quickload) {
        FolderCacheRequestTree(folder_cache, "player_library/animations/male");
        FolderCacheRequestTree(folder_cache, "player_library/animations/huge");
        FolderCacheRequestTree(folder_cache, "player_library/animations/fem");
        FolderCacheRequestTree(folder_cache, "player_library/animations/Vahzilok");
    }
}

//
//

void init_menus()
{
#ifdef SERVER
    extern int write_templates;
    if (!write_templates)
#endif
    {
        writeConsole(OUTPUT_DEBUG, "Loading FX info");
        fxPreloadFxInfo();
        writeConsole(OUTPUT_INFO, "Loaded FX info");

        writeConsole(OUTPUT_DEBUG, "Generating FX string handles");
        fxBuildFxStringHandles();
        writeConsole(OUTPUT_INFO, "Generated FX string handles");
    }

    writeConsole(OUTPUT_DEBUG, "Loading loyalty reward tree");
    accountLoyaltyRewardTreeLoad();
    writeConsole(OUTPUT_INFO, "Loaded loyalty reward tree");

#ifdef CLIENT
    writeConsole(OUTPUT_DEBUG, "Loaded FX behaviors");
    fxPreloadBhvrInfo();
    writeConsole(OUTPUT_INFO, "Loaded FX behaviors");

    writeConsole(OUTPUT_DEBUG, "Loaded villain definitions");
    villainReadDefFiles();
    writeConsole(OUTPUT_INFO, "Loaded villain definitions");

#ifdef NOVODEX_FLUIDS
    loadstart_printf("Loading FX fluids.. ");
    fxPreloadFluidInfo();
    loadend_printf("done");
#endif

    if (!STATE_STRUCT.nofx) {
        writeConsole(OUTPUT_DEBUG, "Loading cape FX");
        fxPreloadCapeInfo();
        writeConsole(OUTPUT_INFO, "Loaded cape FX");
    }
#endif

    // FIXME!!!
    //    Move these two somewhere else.
    writeConsole(OUTPUT_DEBUG, "Loading body parts");
    bpReadBodyPartFiles();
    writeConsole(OUTPUT_INFO, "Loaded body parts");

#ifdef SERVER
    if (!server_state.levelEditor)
#endif

    writeConsole(OUTPUT_DEBUG, "Loading NPC definitions");
    npcReadDefFiles();
    writeConsole(OUTPUT_INFO, "Loaded NPC definitions");


#ifdef SERVER
    if (!server_state.tsr)
#endif
    {
        writeConsole(OUTPUT_DEBUG, "Loading ent_types");
        seqTypeLoadFiles();
        writeConsole(OUTPUT_INFO, "Loaded ent_types");
    }


#ifdef SERVER
    if (!server_state.levelEditor)
    {
        loadstart_printf("Loading villain defs.. ");
            villainReadDefFiles();
        loadend_printf("done");

        loadstart_printf("Loading reward tables.. ");
            rewardReadDefFiles();
        loadend_printf("done");

        loadstart_printf("Loading team reward mods.. ");
            teamRewardReadDefFiles();
        loadend_printf("done");
    }
#endif
}
