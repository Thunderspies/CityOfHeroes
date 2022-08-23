#include <utilitieslib/stdtypes.h>
#include "UI/uiLogin.h"
#include <utilitieslib/network/rsa.h>
#include <utilitieslib/language/AppLocale.h>
#include "UI/uiQuit.h"
#include <utilitieslib/components/EString.h>
#include "graphics/ttFont.h"
#include <utilitieslib/utils/StringUtil.h>
#include "gameComm/initClient.h"
#include "UI/uiEditText.h"
#include "UI/uiDialog.h"
#include "UI/uiComboBox.h"
#include "UI/uiInclude.h"
#include "UI/uiInput.h"
#include "UI/uiFocus.h"
#include "cmdparse/cmdgame.h"
#include "player/inventory_client.h"
#include "language/langClientUtil.h"
#include "UI/sprite/sprite_font.h"
#include "UI/sprite/sprite_text.h"
#include "UI/sprite/sprite_base.h"
#include "graphics/textureatlas.h"
#include "render/tex.h"
#include "UI/uiUtilMenu.h"
#include "UI/uiUtil.h"
#include "graphics/ttFontUtil.h"
#include "UI/uiChat.h"
#include "player/player.h"
#include "entity/classes.h"
#include "entity/origins.h"
#include "entity/powers.h"
#include "entity/entclient.h"
#include "entity/costume_client.h"
#include "UI/uiGame.h"
#include "entity/entVarUpdate.h"
#include "clientcomm/clientcomm.h"
#include "graphics/gfx.h"
#include "win/win_init.h"
#include "varutils.h"
#include "sound/sound.h"
#include "win/input.h"
#include "clientcomm/autoResumeInfo.h"
#include "clientcomm/authclient.h"
#include "clientcomm/dbclient.h"
#include <utilitieslib/components/earray.h>
#include "uiPowers.h"
#include "UI/uiCostume.h"
#include "UI/uiAvatar.h"
#include "UI/uiGender.h"
#include "UI/uiUtilGame.h"
#include "uiListView.h"
#include "UI/uiToolTip.h"
#include "UI/uiServerTransfer.h"
#include <utilitieslib/utils/utils.h>
#include "entity/costume_client.h"
#include "seq/seq.h"
#include "entity/entity.h"
#include "entity/entPlayer.h"
#include <utilitieslib/utils/file.h>
#include "demo.h"
#include "auth/authUserData.h"
#include "UI/uiReticle.h"
#include "UI/Hybrid/uiRegister.h"
#include "account/AccountData.h"
#include "UI/uiWindows.h"
#include "UI/uiClipper.h"
#include "UI/uiBox.h"
#include "UI/uiScrollBar.h"
#include "UI/uiCursor.h"
#include "UI/uiContextMenu.h"
#include "UI/uiTray.h"
#include "UI/uiPlayerNote.h"
#include "gameComm/trayCommon.h"
#include <utilitieslib/utils/file.h>
#include <utilitieslib/utils/sysutil.h>
#include "UI/uiWindows_init.h"
#include <utilitieslib/utils/osdependent.h>

#include "account/AccountCatalog.h"
#include "account/AccountData.h"

#include "formatter/smf_parse.h"
#include "formatter/smf_main.h"
#include "uiSMFView.h"

#include <utilitieslib/language/MessageStoreUtil.h>
#include <utilitieslib/components/HashFunctions.h>
#include <utilitieslib/version/AppRegCache.h>
#include <utilitieslib/version/AppVersion.h>
#include <utilitieslib/network/crypt.h>

#include "comm_backend.h"
#include "graphics/gfxSettings.h"
#include "render/thread/rt_init.h"

#include "UI/Hybrid/uiHybridMenu.h"
#include "UI/Hybrid/uiLoyaltyTree.h"
#include "UI/Hybrid/uiWebStoreFrame.h"

#include "LWC.h"
#include "LWC_common.h"
#include "graphics/gfxLoadScreens.h"

#define MAX_SERVER_COUNT 50
#define MAX_PASSWORD_LEN 32
// This is how long we wait for a response when checking if a name is
// available.
#define CHECK_NAME_TIMEOUT                    4

// This is how long we wait for a response when renaming a character.
#define RENAME_TIMEOUT                        20
// Is there any reason all of these timeout times are different? -CW

// This is the color of the text on the main login screen. 
//This must change when the artist changes the color of the backdrop image.
#define LOGIN_FOREGROUND_TEXT_COLOR 0xffffffff

// Don't show the "SETTINGS" button
//#define DISABLE_SETTINGS_BUTTON

// Uncomment these to enable them when NCsoft Launcher support is needed
// NDA disabled 08-10-2011 for I21 Open Beta
//#define ENABLE_NDA

static TextAttribs gTextAttr =
{
    /* piBold        */  (int *)0,
    /* piItalic      */  (int *)0,
    /* piColor       */  (int *)0xffffffff,
    /* piColor2      */  (int *)0,
    /* piColorHover  */  (int *)0xffffffff,
    /* piColorSelect */  (int *)0,
    /* piColorSelectBG*/ (int *)0x333333ff,
    /* piScale       */  (int *)(int)(1.1f*100.f),
    /* piFace        */  (int *)&game_12,
    /* piFont        */  (int *)0,
    /* piAnchor      */  (int *)0,
    /* piLink        */  (int *)0,
    /* piLinkBG      */  (int *)0,
    /* piLinkHover   */  (int *)0,
    /* piLinkHoverBG */  (int *)0,
    /* piLinkSelect  */  (int *)0,
    /* piLinkSelectBG*/  (int *)0x666666ff,
    /* piOutline     */  (int *)0,
    /* piShadow      */  (int *)0,
};

//#define SHOW_MAPSERVER_STATUS // Define if you want to see "stuck" and "not started" messages.
static int s_LoginInit = 0;
LoginStage s_loggedIn_serverSelected = LOGIN_STAGE_START;
LoginStage s_PostNDALoginStage = LOGIN_STAGE_START;
static int s_CharacterSelectInit = 0;
static int s_CharacterCreationStarted = 0;

static int s_pendingPlayerNumber = -1;
int gPlayerNumber = 0;
int gPlayerNumberRename = 0;
int gSelectedDbServer = -1;
static char s_playerNameRename[64];

static char gCharacterToBeDeleted[32];
static int gCharacterToBeDeletedSlot;

static char g_ServerName[SERVER_NAME_SIZE];
char g_achAccountName[32];
U8 g_achPassword[MAX_PASSWORD_LEN];  // NOT A STRING!  This is the encrypted password.
int g_iDontSaveName;

char g_shardVisitorData[SHARD_VISITOR_DATA_SIZE];
U32 g_shardVisitorDBID;
U32 g_shardVisitorUID;
U32 g_shardVisitorCookie;
U32 g_shardVisitorAddress;
U16 g_shardVisitorPort;

static SMFBlock *s_labelAccount;
static SMFBlock *s_editAccount;
static SMFBlock *s_rememberAccount;
static SMFBlock *s_labelPassword;
static SMFBlock *s_editPassword;
static SMFBlock *s_linkToAccount;

static ToolTip gServerTips[MAX_SERVER_COUNT] = {0};

static int s_SendInfoToTransgaming = true;

// This is how we keep track of what we're doing on the character select
// screen. The dialogs are non-modal so we need to track a state otherwise
// we can try to switch characters while prompting for deletion or let
// players enter the game halfway through a transfer setup.
typedef enum SelectCharacterType
{
    SELECT_CHARACTER_IDLE,
    SELECT_CHARACTER_CHOOSE,
    SELECT_CHARACTER_DELETE,
    SELECT_CHARACTER_CREATE,
    SELECT_CHARACTER_RENAME,
    SELECT_CHARACTER_CHECK_NAME,
    SELECT_CHARACTER_PLAY,
    SELECT_CHARACTER_REDEEM_SLOT,
    SELECT_CHARACTER_STORE,
    SELECT_CHARACTER_BUY_EXPANSION,
} SelectCharacterType;

// Overall state of the character selection screen. Choosing a character,
// deleting an existing character and so on.
static SelectCharacterType selectCharacterState = SELECT_CHARACTER_CHOOSE;

static bool checkNameWaiting = false;
static U32 checkNameStartTime = 0;

static bool renameWaiting = false;
static U32 renameStartTime = 0;

int g_keepPassword = false;

int gLoyaltyTreeVisible = 0;

__int64 s_retrievingCharacterList_startMS = 0;

char *getAccountDir(char *buff)
{
    char * filepath = getExecutableDir(buff);
    strcat(filepath, "/" );
    strcat(filepath, g_achAccountName );
    return filepath;
}

void restartCharacterSelectScreen(void)
{
    selectCharacterState = SELECT_CHARACTER_CHOOSE;
    start_menu( MENU_LOGIN );
}

void resetCharacterCreationFlag(void)
{
    s_CharacterCreationStarted = 0;
}

void restartLoginScreen()
{
    Entity *e = playerPtr();

    s_loggedIn_serverSelected = LOGIN_STAGE_START;
    s_LoginInit = 0;
    s_CharacterSelectInit = 0;
    selectCharacterState = SELECT_CHARACTER_CHOOSE;
    toggle_3d_game_modes(SHOW_NONE);
    if (isProductionMode() && !g_keepPassword)
    {
        // Encrypt an empty string
        g_achPassword[0] = 0;
        cryptStore(g_achPassword, g_achPassword, sizeof(g_achPassword));
    }
    forceLoginMenu();

    game_state.desaturateDelay = 0.0f;
    game_state.desaturateWeight = 0.0f;
    game_state.desaturateWeightTarget = 0.0f;
    game_state.desaturateFadeTime = 0.0f;
    s_SendInfoToTransgaming = true;

    // Do this to reset the window colors, etc. for the options menu
    resetWindows();

    if (e)
    {
        if (!s_CharacterCreationStarted)
        {
            ent_xlat[e->svr_idx] = 0;
            entFree( e );
            entReset();
            playerSetEnt(0);
        }
        else
        {
            entResetNonPlayer();
        }
    }

}

// Used by EULA, NDA, and Server Select screens
static void handleBackToLogin()
{
    authDisconnect();
    s_loggedIn_serverSelected = LOGIN_STAGE_START;
    restartLoginScreen();
}

static void resetCharacterCreation()
{
    resetCreationMenus();
    gender_reset();
    if (db_info.players && gPlayerNumber < db_info.player_count)
    {
        costume_init(db_info.players[gPlayerNumber].costume);
        dbGetCostume(gPlayerNumber);
    }
}

void resetVillainHeroDependantStuff(int changeCostume)
{
    reskinTextures();
    if(changeCostume)
        resetCostume(1);
    setupUIColors();
    gfxReapplySettings();
    switch(game_state.skin)
    {
        case UISKIN_HEROES:
        default:
            msSetPrepend(menuMessages, NULL);
            break;
        case UISKIN_VILLAINS:
            msSetPrepend(menuMessages, "v_");
            break;
        case UISKIN_PRAETORIANS:
            msSetPrepend(menuMessages, "p_");
            break;
    }
}

void createCharacter(void)
{
    if (!isMenu(MENU_LOGIN) )
        return;

    if( game_state.skin != UISKIN_HEROES )
    {
        // reset stuff cached depending on game type
        game_state.skin = UISKIN_HEROES;
        resetVillainHeroDependantStuff(1);
    }

    dialogClearQueue( 0 );  
    selectCharacterState = SELECT_CHARACTER_IDLE;
    gPlayerNumber = s_pendingPlayerNumber;
    start_menu( MENU_ORIGIN );
    if (!s_CharacterCreationStarted)
    {
        resetCharacterCreation();
        s_CharacterCreationStarted = 1;
    }
    s_CharacterSelectInit = 0;
    setupUIColors();
}

static void deleteCharacterCancel(void * data)
{
    selectCharacterState = SELECT_CHARACTER_CHOOSE;
}

void charSelectPurchaseCancelled(void)
{
    selectCharacterState = SELECT_CHARACTER_CHOOSE;
}

static void refreshCharacterList(void)
{
    // Characters may have arrived, changed or left. We need to resynch
    // our list.
    dbResendPlayers();
}

static void playerSlotsLoad();

void charSelectRefreshCharacterList(void)
{
    // We've no idea what order the list is in now, so force back to the
    // top.

    gPlayerNumber = 0;
    playerSlotsLoad(); 

    costume_init(db_info.players[gPlayerNumber].costume);
    dbGetCostume(gPlayerNumber);

}

static void renameCancel(void * data)
{
    selectCharacterState = SELECT_CHARACTER_CHOOSE;
}

static void renameSubmitNewName(void * data)
{
    renameStartTime = timerSecondsSince2000();
    renameWaiting = true;

    dbRenameCharacter(db_info.players[gPlayerNumberRename].name, dialogGetTextEntry());
}

static void renameResponseOk(void * data)
{
    selectCharacterState = SELECT_CHARACTER_CHOOSE;
}


//--------------------------------------------------------------------------------------
// Check Name
//--------------------------------------------------------------------------------------
static void checkNameCancel(void * data)
{
    selectCharacterState = SELECT_CHARACTER_CHOOSE;
}

static void checkNameSubmit(void * data)
{
    checkNameStartTime = timerSecondsSince2000();
    checkNameWaiting = true;

    dbCheckName( dialogGetTextEntry(), 1, 0 );
}

static void checkNameResponseOk(void * data)
{
    selectCharacterState = SELECT_CHARACTER_CHOOSE;
}

bool characterRenameReceiveResult(char* resultMsg)
{
    bool retval = false;

    renameWaiting = false;

    devassert( resultMsg && resultMsg[0] );

    if( resultMsg && selectCharacterState == SELECT_CHARACTER_RENAME )
    {
        dialogStd( DIALOG_OK, resultMsg, NULL, NULL, renameResponseOk, NULL, 0 );
        sndPlay("N_Error", SOUND_GAME);
        refreshCharacterList();
        retval = true;
    }

    return retval;
}

static const char *getVipFreeRegValue()
{
    static char buf[64];
    char *ptr = buf;
    sprintf_s(buf, sizeof(buf), "ackVipFreeSwitch_%s", auth_info.name);
    while (*ptr)
    {
        *ptr = tolower(*ptr);
        ++ptr;
    }

    return buf;
}

static void disableSlotLockNagForThisAccount()
{
    regPutAppInt(getVipFreeRegValue(), 1);
}

void characterUnlockReceiveResult(void)
{
    disableSlotLockNagForThisAccount();
    dialogStd( DIALOG_OK, "charUnlockSuccess", NULL, NULL, NULL, NULL, 0 );
    sndPlay("N_Error", SOUND_GAME);
    refreshCharacterList();
}

bool charSelectDialog(char* resultMsg)
{
    bool retval = false;

    devassert( resultMsg && resultMsg[0] );

    if( resultMsg )
    {
        dialogStd( DIALOG_OK, resultMsg, NULL, NULL, checkNameResponseOk, NULL, 0 );
        retval = true;
    }

    return retval;
}

//----------------------------------------------------------------------------------------------------------------------------------
// Alloc Slots
//----------------------------------------------------------------------------------------------------------------------------------

static bool isVipPremiumLocked(void)
{
    return cfg_IsVIPShard() && !AccountIsReallyVIPAfterServerSelected();
}

typedef struct
{
    SkuId skuAccessPraetorian;
    SkuId skuAccessArchetype;
    SkuId skuAccessPrimaryPowerset;
    SkuId skuAccessSecondaryPowerset;
    const char *strNoAccessArchetype;
    const char *strNoAccessPrimaryPowerset;
    const char *strNoAccessSecondaryPowerset;
    bool onlyVipShardLocked;
} ClientCheckInfo;

// this check should match parseClientInput.c - MapCheckDisallowedPlayer
static int ClientCheckDisallowedPlayer(const PlayerSlot *slot, ClientCheckInfo *pClientCheck)
{
    static char s_primaryPowerset[64];
    static char s_secondaryPowerset[64];
    int i;
    bool bNoAccess = 0;
    const CharacterClass *pClass = NULL;
    bool bPraetorian = slot->praetorian != kPraetorianProgress_PrimalBorn && slot->praetorian != kPraetorianProgress_NeutralInPrimalTutorial;

    pClientCheck->skuAccessPraetorian = kSkuIdInvalid;
    pClientCheck->skuAccessArchetype = kSkuIdInvalid;
    pClientCheck->skuAccessPrimaryPowerset = kSkuIdInvalid;
    pClientCheck->skuAccessSecondaryPowerset = kSkuIdInvalid;
    pClientCheck->strNoAccessPrimaryPowerset = 0;
    pClientCheck->strNoAccessSecondaryPowerset = 0;
    pClientCheck->onlyVipShardLocked = 0;

    if (!slot->invalid && slot->playerslot_class && slot->playerslot_class[0] != 0)
        pClass = slot->invalid?0:classes_GetPtrFromName(&g_CharacterClasses, slot->playerslot_class);

    if( bPraetorian && !authUserHasProvisionalRogueAccess((U32*)db_info.auth_user_data) && !AccountHasStoreProduct(&db_info.account_inventory, SKU("ctexgoro")) )
    {
        bNoAccess = 1;
        pClientCheck->skuAccessPraetorian = skuIdFromString("ctexgoro");
    }

    // HYBRID : Checks for valid AT and Powersets
    if (pClass)
    {
        // checking class restrictions
        if (class_MatchesSpecialRestriction(pClass, "Kheldian") && !authUserHasKheldian((U32*)db_info.auth_user_data))
        {
            devassertmsg(pClass->pchStoreRestrictions && pClass->pchProductCode, "Kheldian Special Restriction with no StoreRequires or ProductCode");
            if (!accountEval(inventoryClient_GetAcctInventorySet(), db_info.loyaltyStatus, db_info.loyaltyPointsEarned, (U32 *) db_info.auth_user_data, pClass->pchStoreRestrictions))
            {
                pClientCheck->skuAccessArchetype = skuIdFromString(pClass->pchProductCode);
                pClientCheck->strNoAccessArchetype = pClass->pchDisplayName;
            }
        } else if ((class_MatchesSpecialRestriction(pClass, "ArachnosSoldier") || class_MatchesSpecialRestriction(pClass, "ArachnosWidow")) &&
                    !authUserHasArachnosAccess((U32*)db_info.auth_user_data))
        {
            devassertmsg(pClass->pchStoreRestrictions && pClass->pchProductCode, "ArachnosSoldier/ArachnosWidow Special Restriction with no StoreRequires or ProductCode");
            if (!accountEval(inventoryClient_GetAcctInventorySet(), db_info.loyaltyStatus, db_info.loyaltyPointsEarned, (U32 *) db_info.auth_user_data, pClass->pchStoreRestrictions))
            {
                pClientCheck->skuAccessArchetype = skuIdFromString(pClass->pchProductCode);
                pClientCheck->strNoAccessArchetype = pClass->pchDisplayName;
            }
        } else {
            if (pClass && pClass->pchStoreRestrictions && pClass->pchProductCode)
            {
                if (!accountEval(inventoryClient_GetAcctInventorySet(), db_info.loyaltyStatus, db_info.loyaltyPointsEarned, (U32 *) db_info.auth_user_data, pClass->pchStoreRestrictions))
                {
                    pClientCheck->skuAccessArchetype = skuIdFromString(pClass->pchProductCode);
                    pClientCheck->strNoAccessArchetype = pClass->pchDisplayName;
                }
            }
        }

        if (!skuIdIsNull(pClientCheck->skuAccessArchetype))
        {
            bNoAccess = 1;
        }

        // checking powerset restrictions

        // primary
        for(i = eaSize(&pClass->pcat[kCategory_Primary]->ppPowerSets)-1; i>=0; i--)
        {
            const BasePowerSet *pPowerSetPri = pClass->pcat[kCategory_Primary]->ppPowerSets[i];

            if (pPowerSetPri != NULL && stricmp(slot->primaryPower, pPowerSetPri->pchName) == 0)
            {
                if (pPowerSetPri->pchAccountRequires != NULL && !accountEval(inventoryClient_GetAcctInventorySet(), db_info.loyaltyStatus, db_info.loyaltyPointsEarned, (U32 *) db_info.auth_user_data, pPowerSetPri->pchAccountRequires))
                {
                    pClientCheck->strNoAccessPrimaryPowerset = pPowerSetPri->pchDisplayName;
                    pClientCheck->skuAccessPrimaryPowerset = skuIdFromString(pPowerSetPri->pchAccountProduct);
                }
                break;
            }
        }

        // secondary
        for(i = eaSize(&pClass->pcat[kCategory_Secondary]->ppPowerSets)-1; i>=0; i--)
        {
            const BasePowerSet *pPowerSetSec = pClass->pcat[kCategory_Secondary]->ppPowerSets[i];

            if (pPowerSetSec != NULL && stricmp(slot->secondaryPower, pPowerSetSec->pchName) == 0)
            {
                if (pPowerSetSec->pchAccountRequires != NULL && !accountEval(inventoryClient_GetAcctInventorySet(), db_info.loyaltyStatus, db_info.loyaltyPointsEarned, (U32 *) db_info.auth_user_data, pPowerSetSec->pchAccountRequires))
                {
                    pClientCheck->strNoAccessSecondaryPowerset = pPowerSetSec->pchDisplayName;
                    pClientCheck->skuAccessPrimaryPowerset = skuIdFromString(pPowerSetSec->pchAccountProduct);
                }
                break;
            }
        }

        if (pClientCheck->strNoAccessPrimaryPowerset || pClientCheck->strNoAccessSecondaryPowerset)
        {
            bNoAccess = 1;
        }
    }

    if (slot->slot_lock != SLOT_LOCK_NONE)
    {
        bNoAccess = 1;
    }

    // ADD ADDITIONAL CHECKS ABOVE THIS LINE OR pClientCheck->onlyVipShardLocked WILL BE WRONG!
    // ------------------------------------------------------------------------
    // ADD ADDITIONAL CHECKS ABOVE THIS LINE OR pClientCheck->onlyVipShardLocked WILL BE WRONG!

    // does not need to be shadowed by mapserver, handled in handleChoosePlayer(...) in clientcomm.c in dbserver
    if (isVipPremiumLocked())
    {
        pClientCheck->onlyVipShardLocked = !bNoAccess;
        bNoAccess = 1;
    }

    // DO NOT ADD ADDITIONAL CHECKS HERE OR pClientCheck->onlyVipShardLocked WILL BE WRONG!
    //
    //  X X  X X  X X  X X  X X  X X  X X  X X  X X  X X  X X  X X  X X  X X
    //   O    O    O    O    O    O    O    O    O    O    O    O    O    O
    //
    // DO NOT ADD ADDITIONAL CHECKS HERE OR pClientCheck->onlyVipShardLocked WILL BE WRONG!

    return bNoAccess;
}

static void slotRedeemCancel(void * data)
{
    selectCharacterState = SELECT_CHARACTER_CHOOSE;
}

static char s_unlockCharacter[64];
static void slotRedeemAndUnlockSubmit(char *charName)
{
    if (CanAllocateCharSlots())
    {
        dialogStd( DIALOG_OK, textStd("redeemSlotProcessing"), NULL, NULL, slotRedeemCancel, NULL, DLGFLAG_NO_TRANSLATE );
        if (charName)
        {
            strcpy(s_unlockCharacter, charName);
        }
        else
        {
            s_unlockCharacter[0] = '\0';
        }

        dbRedeemSlot();
    }
    else
    {
        dialogStd( DIALOG_OK, textStd("redeemSlotProcessingFailed"), NULL, NULL, slotRedeemCancel, NULL, DLGFLAG_NO_TRANSLATE );
    }
}

static void slotRedeemSubmit(void * data)
{
    slotRedeemAndUnlockSubmit(NULL);
}

//----------------------------------------------------------------------------------------------------------------------------------
// Start Commands
//----------------------------------------------------------------------------------------------------------------------------------

static void startRename(int playerNumberRename)
{
    sndPlay( "select", SOUND_GAME );

    selectCharacterState = SELECT_CHARACTER_RENAME;
    gPlayerNumberRename = playerNumberRename;

    dialogSetTextEntry( db_info.players[playerNumberRename].name );
    dialogStd( DIALOG_NAME_TEXT_ENTRY, "PaidRenameChoose",    NULL, NULL,    renameSubmitNewName, renameCancel, DLGFLAG_NAMES_ONLY );
}

static U32 rename_claim_delay;
static void startRenameTokenRedemption(void *unused)
{
    strcpy(s_playerNameRename, db_info.players[gPlayerNumber].name);
    dbRedeemRenameToken(auth_info.uid, db_info.players[gPlayerNumber].name, gPlayerNumber);
    rename_claim_delay = timerSecondsSince2000();
}

static void startRedeemSlot(void)
{
    selectCharacterState = SELECT_CHARACTER_REDEEM_SLOT;
    dialogSetTextEntry( "" );
    dialogStd( DIALOG_ACCEPT_CANCEL, textStd("redeemSlotWarning", textStd(db_info.shardname)), NULL, NULL, slotRedeemSubmit, slotRedeemCancel, DLGFLAG_NAMES_ONLY );
    sndPlay("N_Error", SOUND_GAME);
}

void renameCharacterRightAwayIfPossible(void)
{
    if (s_playerNameRename[0] != '\0' && selectCharacterState == SELECT_CHARACTER_CHOOSE)
    {
        int i;
        for (i = 0; i < db_info.player_count; ++i)
        {
            if (stricmp(db_info.players[i].name, s_playerNameRename) == 0 && !db_info.players[i].invalid && db_info.players[i].db_flags & DBFLAG_RENAMEABLE)
            {
                startRename(i);
                break;
            }
        }

        s_playerNameRename[0] = '\0';
    }
}

void unlockCharacterRightAwayIfPossible(void)
{
    if (s_unlockCharacter[0] != '\0')
    {
        dbUnlockCharacter(s_unlockCharacter);
        s_unlockCharacter[0] = '\0';
    }
}

void GRNagCancelledLogin(void)
{
    // This gets called from the cancel callback for the GR buy/nag dialogs,
    // if they are showing at login or character select. This way we don't
    // have to expose our state machine.
    selectCharacterState = SELECT_CHARACTER_CHOOSE;
}

static char const *manageaccount_addresses[LOCALE_ID_COUNT + 1] =
{
    "https://secure.ncsoft-url-stripped-todo.com/cgi-bin/plaync_login.pl?language=en",
    NULL,
    NULL,
    NULL,
    NULL,
    "https://secure.ncsoft-url-stripped-todo.com/cgi-bin/plaync_login.pl?language=de",
    "https://secure.ncsoft-url-stripped-todo.com/cgi-bin/plaync_login.pl?language=fr",
    "https://secure.ncsoft-url-stripped-todo.com/cgi-bin/plaync_login.pl?language=es",
    // UK English alternate URL comes at the end of the list
    "https://secure.ncsoft-url-stripped-todo.com/cgi-bin/plaync_login.pl?language=en-gb"
};

//----------------------------------------------------------------------------------------------------------------------------------
// Select Character Screen
//----------------------------------------------------------------------------------------------------------------------------------

typedef struct PlayerSlotEntry
{
    char *pchAccount;
    char *pchServer;
    char *pchName;
    int slot;
}PlayerSlotEntry;

PlayerSlotEntry **ppPlayerSlots;

static void addPlayerSlotEntry( char * account, char * server, char * name, int slot )
{
    PlayerSlotEntry * pSE;
    int i, found = 0;

    // don't add emptyes
    if( stricmp( name, "EMPTY") == 0 )
        return;

    // check duplicates
    for( i = 0; i < eaSize(&ppPlayerSlots); i++ )
    {
        if(    strcmp( name, ppPlayerSlots[i]->pchName ) == 0 &&
            strcmp( account, ppPlayerSlots[i]->pchAccount ) == 0 && 
            strcmp( server, ppPlayerSlots[i]->pchServer ) == 0 )
        {
            ppPlayerSlots[i]->slot = slot;
            return;
        }
    }

    {
        pSE = calloc( 1, sizeof(PlayerSlotEntry) );
        pSE->pchAccount = strdup(account);
        pSE->pchServer = strdup(server);
        pSE->pchName = strdup(name);
        pSE->slot = slot;
        eaPush( &ppPlayerSlots, pSE );
    }
}

static void playerSlotFree(PlayerSlotEntry *pSlot)
{
    if (!pSlot)
        return;

    SAFE_FREE(pSlot->pchAccount);
    SAFE_FREE(pSlot->pchServer);
    SAFE_FREE(pSlot->pchName);
    SAFE_FREE(pSlot);
}

static void playerSlotsSave()
{
    int i, count = 0;
    FILE *file;
    char filename[FILENAME_MAX];
    char buff[MAX_PATH];
    char *pchServer = g_ServerName;

    if( g_iDontSaveName || g_ServerName[0] == 0 )
        return;

    if(isDevelopmentMode())
        strcpy(filename, "C:/playerslot.txt");
    else
        sprintf(filename, "%s/playerslot.txt", getAccountDir(buff) );

    file = fileOpen( filename, "wt" );
    if(!file)
    {
        makeDirectoriesForFile(filename);
        file = fileOpen( filename, "wt" );
        if(!file)
            return;
    }

    for( i = eaSize(&ppPlayerSlots)-1; i>=0; i-- )
    {
        int j,found = 0;
    
        // don't save deleted character
        for(j=0;j<db_info.max_slots;j++)
        { 
            if( strcmp(g_achAccountName, ppPlayerSlots[i]->pchAccount) != 0 ||
                strcmp( pchServer,ppPlayerSlots[i]->pchServer) != 0 || 
                strcmp( ppPlayerSlots[i]->pchName, db_info.players[j].name ) == 0 ) // name must match if its same account/server otherwise leave it
                found = 1;
        }
        if( found )
            fprintf(file, "\"%s\" \"%s\" \"%s\" \"%i\"\n", ppPlayerSlots[i]->pchAccount, ppPlayerSlots[i]->pchServer, ppPlayerSlots[i]->pchName, ppPlayerSlots[i]->slot );
        else
        {
            playerSlotFree(eaRemove(&ppPlayerSlots,i));
        }
    }

    fclose(file);
}

int playerSlot_FirstOpen()
{
    int i, slot = 0;
    char *pchServer = g_ServerName;

    for( i = 0; i < eaSize(&ppPlayerSlots) && slot < db_info.max_slots; i++ )
    {
        if( strcmp(g_achAccountName, ppPlayerSlots[i]->pchAccount) == 0 &&
            strcmp( pchServer,ppPlayerSlots[i]->pchServer) == 0 &&  
            ppPlayerSlots[i]->slot == slot )
        {
            slot++;
            i = 0;
        }
    }
    return slot;
}


static void playerSlotsLoad()
{
    FILE *file;
    char buf[1000];
    char filename[FILENAME_MAX];
    char *pchServer = g_ServerName;
    int i,j, rename_idx=-1;
    int slot;

    if(g_ServerName[0] == 0)
        return;

    if(isDevelopmentMode())
        strcpy(filename, "C:/playerslot.txt");
    else
        sprintf(filename, "%s/playerslot.txt", getAccountDir(buf) );

    file = fileOpen( filename, "rt" );
    if(!file)
    {
        makeDirectoriesForFile(filename);
        file = fileOpen( filename, "rt" );
        if(!file)
            return;
    }
    eaDestroyEx(&ppPlayerSlots, playerSlotFree);

    while(fgets(buf,sizeof(buf),file))
    {
        char *args[4];
        int count = tokenize_line_safe( buf, args, 4, 0 );
        if( count == 4 )
            addPlayerSlotEntry( args[0],  args[1], args[2], atoi(args[3]) );
    }
    fclose(file); 

    // first look for any missing characters, 
    for( i = eaSize(&ppPlayerSlots)-1; i>=0; i-- )
    {
        int j,found = 0;
        PlayerSlotEntry *pSlot;

        // remove deleted characters
        for(j=0;j<db_info.max_slots;j++)
        { 
            if( strcmp(g_achAccountName, ppPlayerSlots[i]->pchAccount) != 0 ||
                strcmp( pchServer,ppPlayerSlots[i]->pchServer) != 0 || 
                strcmp( ppPlayerSlots[i]->pchName, db_info.players[j].name ) == 0 ) // name must match if its same account/server otherwise leave it
                found = 1;
        }
        if( !found ) //assume the character is missing because its been renamed
        {
            pSlot = eaRemove(&ppPlayerSlots,i);
            rename_idx = pSlot->slot;
            playerSlotFree(pSlot);
        }
    }

    // now make sure every slot is displaying somewhere
    for(i=0;i<db_info.player_count;i++)
    { 
        int found = 0;
        for(j=0;j<eaSize(&ppPlayerSlots);j++)
        {
            if( strcmp(db_info.players[i].name, ppPlayerSlots[j]->pchName) == 0 &&
                strcmp(g_achAccountName, ppPlayerSlots[j]->pchAccount) == 0 &&
                strcmp(pchServer,ppPlayerSlots[j]->pchServer) == 0 )
            {
                found = 1;
                break;
            }
        }
        
        if(!found) // add it to list
        {
            if( rename_idx>=0 ) // a character has been deleted this update, so the new character must be the old one renamed
            {
                addPlayerSlotEntry( g_achAccountName, pchServer, db_info.players[i].name, rename_idx );  
                rename_idx = -1;
            }
            else
                addPlayerSlotEntry( g_achAccountName, pchServer, db_info.players[i].name, playerSlot_FirstOpen() );  
        }
    }

    // lastly make sure no slots are doubled
    for( i = 0; i < eaSize(&ppPlayerSlots); i++ )
    {
        // check for out of bound slot
        if( ppPlayerSlots[i]->slot >= 48 )
            ppPlayerSlots[i]->slot = playerSlot_FirstOpen();

        for(j=i+1;j<eaSize(&ppPlayerSlots);j++)
        {
            if( strcmp(g_achAccountName, ppPlayerSlots[i]->pchAccount) == 0 &&
                strcmp( pchServer,ppPlayerSlots[i]->pchServer) == 0 &&                  
                strcmp(g_achAccountName, ppPlayerSlots[j]->pchAccount) == 0 &&
                strcmp( pchServer,ppPlayerSlots[j]->pchServer) == 0 &&                  
                ppPlayerSlots[i]->slot == ppPlayerSlots[j]->slot )
            {
                 ppPlayerSlots[j]->slot = playerSlot_FirstOpen();
            }
        }
    }

    // Search player slots for someone on this server
    slot = 2000000000;
    for (j = 0; j < eaSize(&ppPlayerSlots); j++)
    {
        if( strcmp(g_achAccountName, ppPlayerSlots[j]->pchAccount) == 0 &&
            strcmp( pchServer,ppPlayerSlots[j]->pchServer) == 0 &&
            ppPlayerSlots[j]->slot < slot)
        {
            // Found someone in a lower slot than current minimum
            slot = ppPlayerSlots[j]->slot;
            // Save slot index;
            i = j;

        }
    }

    // Did we find any active slots?
    if (slot < 2000000000)
    {
        // Convert to db slot number
        for(j=0;j<db_info.max_slots;j++)
        { 
            if( strcmp( ppPlayerSlots[i]->pchName, db_info.players[j].name ) == 0 )
            {
                gPlayerNumber = j;
                costume_init(db_info.players[gPlayerNumber].costume);
                dbGetCostume(gPlayerNumber);        
                break;
            }
        }
    }

    playerSlotsSave();
}

static bool isStoreHidingCharacterSlots(void)
{
    bool retval = false;

    if (selectCharacterState == SELECT_CHARACTER_REDEEM_SLOT)
        retval = true;
    else
        retval = characterTransferAreSlotsHidden();

    return retval;
}

static PlayerSlotEntry * playerSlot_Find( char *pchAccount, char *pchServer, char * pchName )
{
    int i;
    for( i = 0; i < eaSize(&ppPlayerSlots); i++ )
    {
        if( strcmp(pchAccount, ppPlayerSlots[i]->pchAccount ) == 0 &&
            strcmp(pchServer, ppPlayerSlots[i]->pchServer ) == 0 &&
            strcmp(pchName, ppPlayerSlots[i]->pchName ) == 0 )
        {
            return ppPlayerSlots[i];
        }
    }
    return 0;
}

static void deleteCharacter(int *pNumber)
{
    char * pchServer = textStd( g_ServerName );
    if( dbDeletePlayer( *pNumber, control_state.notimeout?NO_TIMEOUT:60 ) )
    {
        extern int player_slot;
        PlayerSlotEntry *pSlot = playerSlot_Find( g_achAccountName, pchServer, db_info.players[*pNumber].name );
        if( pSlot )
            eaFindAndRemove( &ppPlayerSlots, pSlot );
        db_info.player_count--;

        player_slot = *pNumber;
        db_info.players[*pNumber].slot_lock = SLOT_LOCK_NONE;
        strcpy( db_info.players[*pNumber].name, "EMPTY" );
        costume_init(db_info.players[*pNumber].costume);

        *pNumber = 0;
        costume_init(db_info.players[*pNumber].costume);
        dbGetCostume(*pNumber);
    }
}

static void deleteCharacterOk(void * data)
{
    if( isDevelopmentMode() )
    {
        deleteCharacter(&gPlayerNumber);
    }
    else
    {
        if( dialogGetTextEntry() &&
            strcmp( dialogGetTextEntry(), gCharacterToBeDeleted ) == 0 &&
            strcmp( db_info.players[gCharacterToBeDeletedSlot].name, dialogGetTextEntry() ) == 0 )
        {
            deleteCharacter(&gCharacterToBeDeletedSlot);
        }
    }

    playerSlotsSave();
    selectCharacterState = SELECT_CHARACTER_CHOOSE;
}

#define DRAWN_SLOTS 13
#define LINE_HEIGHT (660.f/DRAWN_SLOTS)

static void createCharacterCancel(void * data){ selectCharacterState = SELECT_CHARACTER_CHOOSE; }



static playerSlotMove( int src_idx, int dest_idx, char * pchName )
{
    PlayerSlotEntry *pSrc = 0, *pDest = 0;
    char * pchServer = textStd( g_ServerName );
    int i;

     for( i = 0; i < eaSize(&ppPlayerSlots); i++ )
    {
        if(    src_idx == ppPlayerSlots[i]->slot &&
            strcmp( g_achAccountName, ppPlayerSlots[i]->pchAccount ) == 0 && strcmp( pchServer, ppPlayerSlots[i]->pchServer ) == 0 )
        {
            pSrc = ppPlayerSlots[i];
        }
        if(    dest_idx == ppPlayerSlots[i]->slot &&
            strcmp( g_achAccountName, ppPlayerSlots[i]->pchAccount ) == 0 && strcmp( pchServer, ppPlayerSlots[i]->pchServer ) == 0 )
        {
            pDest = ppPlayerSlots[i];
        }
    }

    if( pDest && pSrc)
    {
        int tmp;
        tmp = pDest->slot;
        pDest->slot = pSrc->slot;
        pSrc->slot = tmp;
    }
    else if( !pDest && pSrc )
    {
        pSrc->slot = dest_idx;
        pDest = playerSlot_Find( g_achAccountName, pchServer, db_info.players[dest_idx].name );
        if( !pDest && db_info.players[dest_idx].name && stricmp( db_info.players[dest_idx].name, "EMPTY" ) != 0 && strcmp(pSrc->pchName, db_info.players[dest_idx].name) != 0  )
            addPlayerSlotEntry( g_achAccountName, pchServer, db_info.players[dest_idx].name, src_idx ); 
    }
    else if( pDest && !pSrc )
    {
        pDest->slot = src_idx;
        pSrc = playerSlot_Find( g_achAccountName, pchServer, db_info.players[src_idx].name );
        if( db_info.players[src_idx].name && stricmp( db_info.players[src_idx].name, "EMPTY" ) != 0 && strcmp(pDest->pchName, db_info.players[src_idx].name) != 0  )
            addPlayerSlotEntry( g_achAccountName, pchServer, db_info.players[src_idx].name, dest_idx ); 
    }
    else
    {
        addPlayerSlotEntry( g_achAccountName, pchServer, pchName, dest_idx ); 
        if( db_info.players[dest_idx].name && stricmp( db_info.players[dest_idx].name, "EMPTY" ) != 0 && strcmp(pchName, db_info.players[dest_idx].name ) != 0 )
            addPlayerSlotEntry( g_achAccountName, pchServer, db_info.players[dest_idx].name, src_idx ); 
    }
    playerSlotsSave();
}

static PlayerSlot* playerSlotGet( int slot_idx, int *actual_slot )
{
    char * pchServer = textStd( g_ServerName );
    int i, j;

     for( i = eaSize(&ppPlayerSlots)-1; i>=0; i-- )
    {
        if(    slot_idx == ppPlayerSlots[i]->slot &&
            strcmp( g_achAccountName, ppPlayerSlots[i]->pchAccount ) == 0 && strcmp( pchServer, ppPlayerSlots[i]->pchServer ) == 0 )
        {
            for(j=0;j<db_info.max_slots;j++)
            { 
                if( strcmp( ppPlayerSlots[i]->pchName, db_info.players[j].name ) == 0 )
                {
                    *actual_slot = j;
                    return &db_info.players[j];
                }
            }

            // This slot/name was renamed or removed
            playerSlotFree(eaRemove( &ppPlayerSlots, i ));
        }
    }

    // Couldn't find an entry for this slot, check and see if what would regularly be there is somewhere else
      for( i = 0; i < eaSize(&ppPlayerSlots); i++ )
    {
        if(    strcmp( g_achAccountName, ppPlayerSlots[i]->pchAccount ) == 0 && strcmp( pchServer, ppPlayerSlots[i]->pchServer ) == 0 )
        {
            if( slot_idx < db_info.max_slots && strcmp( ppPlayerSlots[i]->pchName, db_info.players[slot_idx].name ) == 0  && ppPlayerSlots[i]->slot != slot_idx )
            {
                *actual_slot = -1;
                return 0;
            }
        }
    }

    // default action
    *actual_slot = slot_idx;
    return &db_info.players[slot_idx];
}

static void createCharacterAndStuff(int slot)
{
    int i;
    dialogClearQueue( 0 );
    s_pendingPlayerNumber = slot;

    for(i=0;i<db_info.max_slots && s_pendingPlayerNumber<0;i++)
    { 
        if( stricmp(db_info.players[i].name, "EMPTY")==0)
            s_pendingPlayerNumber = i;
    }
    selectCharacterState = SELECT_CHARACTER_CREATE;
    createCharacter();
}

static void openStoreSlots(void *unused)
{
    webStoreOpenProduct("svcslots");
}

static void openStoreRename(void *unused)
{
    webStoreOpenProduct("svrename");
}

static void openStoreXfer(void *unused)
{
    webStoreOpenProduct("svsvtran");
}

static void openStoreLocks(void * data)
{
    const ShoppingCart * cart = (const ShoppingCart *) data;
    webStoreAddMultipleToCart(cart, 0, cart->itemCount);
}

static void openStoreVIP(void *unused)
{
    //webOpenUpgradeToVIP();
}

static void unlockCharacter(void *unused)
{
    dbUnlockCharacter(db_info.players[gPlayerNumber].name);
}

static void verifyUnlock(char *message, void(*cb)(void*), void *data)
{
    dialogStdCB(DIALOG_YES_NO, message, NULL, NULL, cb, NULL, 0, data);
    sndPlay("N_Error", SOUND_GAME);
}

static void startCharacterUnlocking(char *strPlayerClass, const ClientCheckInfo *pClientCheck, SlotLock slotLock)
{
    char *str = NULL;
    static ShoppingCart sCharacterUnlockCart;

    if (slotLock == SLOT_LOCK_OFFLINE)
    {
        // character is offline, online the character before dealing with anything else
        dbMakePlayerOnline(gPlayerNumber);
        dialogStd(DIALOG_OK, textStd("WeAreRestoringYourCharacterPleaseWait"), NULL, NULL, NULL, NULL, 0);
        return;
    }

    if (!sCharacterUnlockCart.items)
    {
        sCharacterUnlockCart.items = calloc(10, sizeof(SkuId));
    }
    sCharacterUnlockCart.itemCount = 0;
    estrCreate(&str);
    estrConcatCharString(&str, textStd("CharLockMessageHeader"));
    if (!skuIdIsNull(pClientCheck->skuAccessPraetorian))
    {
        estrConcatCharString(&str, textStd("CharLockMessagePraetorian"));
        sCharacterUnlockCart.items[sCharacterUnlockCart.itemCount++] = pClientCheck->skuAccessPraetorian;
    }
    if (!skuIdIsNull(pClientCheck->skuAccessArchetype))
    {
        estrConcatCharString(&str, textStd("CharLockMessageArchetype", textStd(pClientCheck->strNoAccessArchetype)));
        sCharacterUnlockCart.items[sCharacterUnlockCart.itemCount++] = pClientCheck->skuAccessArchetype;
    }
    if (pClientCheck->strNoAccessPrimaryPowerset)
    {
        estrConcatCharString(&str, textStd("CharLockMessagePowerset", textStd(pClientCheck->strNoAccessPrimaryPowerset)));
        sCharacterUnlockCart.items[sCharacterUnlockCart.itemCount++] = pClientCheck->skuAccessPrimaryPowerset;
    }
    if (pClientCheck->strNoAccessSecondaryPowerset)
    {
        estrConcatCharString(&str, textStd("CharLockMessagePowerset", textStd(pClientCheck->strNoAccessSecondaryPowerset)));
        sCharacterUnlockCart.items[sCharacterUnlockCart.itemCount++] = pClientCheck->skuAccessSecondaryPowerset;
    }
    if (slotLock == SLOT_LOCK_VIP_FREE)
    {
        estrConcatCharString(&str, textStd("CharLockMessageSlot"));
        sCharacterUnlockCart.items[sCharacterUnlockCart.itemCount++] = skuIdFromString("svcslots");
    }

    if (slotLock == SLOT_LOCK_VIP_FREE)
    {
        if (getTotalServerSlots() > getSlotUnlockedPlayerCount())
        {
            if (!skuIdIsNull(pClientCheck->skuAccessPraetorian) || !skuIdIsNull(pClientCheck->skuAccessArchetype) || pClientCheck->strNoAccessPrimaryPowerset || pClientCheck->strNoAccessSecondaryPowerset)
            {
                estrConcatCharString(&str, textStd("CharLockMessageFooterHaveServerSlotNeedOthers"));
                verifyUnlock(str, unlockCharacter, NULL);
            }
            else
            {
                estrConcatCharString(&str, textStd("CharLockMessageFooterHaveServerSlot"));
                verifyUnlock(str, unlockCharacter, NULL);
            }
        }
        else
        {
            if (inventoryClient_GetAcctAuthoritativeState() == ACCOUNT_SERVER_UP)
            {
                if (AccountGetStoreProductCount(&db_info.account_inventory, SKU("svcslots"), true) > 0)
                {
                    if (!skuIdIsNull(pClientCheck->skuAccessPraetorian) || !skuIdIsNull(pClientCheck->skuAccessArchetype) || pClientCheck->strNoAccessPrimaryPowerset || pClientCheck->strNoAccessSecondaryPowerset)
                    {
                        estrConcatCharString(&str, textStd("CharLockMessageFooterHaveGlobalSlotNeedOthers"));
                        verifyUnlock(str, slotRedeemAndUnlockSubmit, db_info.players[gPlayerNumber].name);
                    }
                    else
                    {
                        estrConcatCharString(&str, textStd("CharLockMessageFooterHaveGlobalSlot"));
                        verifyUnlock(str, slotRedeemAndUnlockSubmit, db_info.players[gPlayerNumber].name);
                    }
                }
                else
                {
                    estrConcatCharString(&str, textStd("CharLockMessageFooterNoSlotsLeft"));
                    verifyUnlock(str, openStoreLocks, &sCharacterUnlockCart);
                }
            }
            else
            {
                estrConcatCharString(&str, textStd("CharLockMessageAccountNotLoaded"));
                dialogStd(DIALOG_OK, str, NULL, NULL, NULL, NULL, 0);
            }
        }
    }
    else
    {
        if (inventoryClient_GetAcctAuthoritativeState() == ACCOUNT_SERVER_UP)
        {
            estrConcatCharString(&str, textStd("CharLockMessageFooter"));
            verifyUnlock(str, openStoreLocks, &sCharacterUnlockCart);
        }
        else
        {
            estrConcatCharString(&str, textStd("CharLockMessageAccountNotLoaded"));
            dialogStd(DIALOG_OK, str, NULL, NULL, NULL, NULL, 0);
        }
    }

    estrDestroy(&str);
}

static void drawCharacterSlot(int slot_idx, int bActualSlot, int xpos, int ypos, int *free_slots, int *free_player_unlockable_slots, int *first_create_slot, int * enterWithCurrentSelectedCharacter, int * hoveredSlotIdx, CBox * hoveredSlotCBox)
{
    CBox box;
    int clr = 0xffffff33;
    int color = CLR_WHITE;
    F32 wd = 320.f;
    int mouseover = false;
    AtlasTex* icon, *icon_overlay = 0; 
     float icon_sc = MAX(MIN(1.0f, 9.f/(float)DRAWN_SLOTS), 0.25f); 
    float font_sc = MAX(MIN(1.5f, 25.f/(float)DRAWN_SLOTS), 0.25f);
    int offset = 16, actual_slot = slot_idx;
    bool bCreateCharSlot = 0;
    bool bUnlockCharSlot = 0;
    PlayerSlot * pSlot = NULL;
    static HybridElement **bars = NULL;
    int accountIsVIP = AccountIsReallyVIPAfterServerSelected();
    F32 xposSc, yposSc, textXSc, textYSc;
    calculatePositionScales(&xposSc, &yposSc);
    calculateSpriteScales(&textXSc, &textYSc);

    if (bActualSlot)
    {
        pSlot = &db_info.players[slot_idx];
    } else {
        pSlot = slot_idx>=0?playerSlotGet( slot_idx, &actual_slot ):0;
    }

    BuildScaledCBox( &box, xpos, ypos, wd*xposSc, (LINE_HEIGHT-1)*yposSc);

    if (!pSlot || stricmp(pSlot->name, "EMPTY")==0 || actual_slot < 0)
    {
        if(*free_slots > 0)
        {
            if (*first_create_slot == -1)
            {
                *first_create_slot = actual_slot;
            }

            bCreateCharSlot = !cfg_IsVIPShard() || accountIsVIP;
            (*free_slots)--;
        }
        else if (*free_player_unlockable_slots > 0 || accountIsVIP)
        {
            bUnlockCharSlot = !cfg_IsVIPShard() || accountIsVIP;
        }

        (*free_player_unlockable_slots)--;
    }

     if( !isDown(MS_LEFT) && mouseCollision(&box) && cursor.dragging )
    {
        if( cursor.drag_obj.type == kTrayItemType_PlayerSlot )
            playerSlotMove( cursor.drag_obj.invIdx, slot_idx, cursor.drag_obj.shortName );
        trayobj_stopDragging();
    }

    if(bCreateCharSlot || bUnlockCharSlot || !pSlot || stricmp(pSlot->name, "EMPTY")==0 )
    {
          if( selectCharacterState == SELECT_CHARACTER_CHOOSE && mouseCollision(&box) )
        {
            clr = 0xffffff66;

            if( isDown(MS_LEFT) )
                clr = 0xffffffff;

              if (mouseClickHit( &box, MS_LEFT))
            {
                if( bCreateCharSlot )
                {
                    createCharacterAndStuff(actual_slot);
                }
                else if ( bUnlockCharSlot )
                {
                    if (inventoryClient_GetAcctAuthoritativeState() == ACCOUNT_SERVER_UP)
                    {
                        if (AccountGetStoreProductCount(&db_info.account_inventory, SKU("svcslots"), true) > 0)
                        {
                            startRedeemSlot();
                        }
                        else
                        {
                            dialogStd(DIALOG_YES_NO, "AskStoreSlots", NULL, NULL, openStoreSlots, NULL, 0);
                        }
                    }
                    else
                    {
                        dialogStd(DIALOG_OK, "InventoryNotLoadedSlots", NULL, NULL, NULL, NULL, 0);
                    }
                }
            }
        }
        else
            color = clr;

        {
            int doubleclickable = 1;
            // bar
            if (eaSize(&bars) <= slot_idx)
            {
                eaSetSize(&bars, slot_idx + 1);
            }
            if (!bars[slot_idx])
            {
                bars[slot_idx] = calloc(1, sizeof(HybridElement));
            }
            if (bCreateCharSlot)
            {
                bars[slot_idx]->text = "CreateCharacterString";
                bars[slot_idx]->text_flags = 0;    // Make sure it gets translated
                doubleclickable = 0;
            }
            else if (bUnlockCharSlot)
            {
                bars[slot_idx]->text = "UnlockableSlot";
                bars[slot_idx]->text_flags = 0;    // Make sure it gets translated
                doubleclickable = 0;
            }
            else
            {
                bars[slot_idx]->text = "UnavailableSlot";
                bars[slot_idx]->text_flags = 0;    // Make sure it gets translated
                doubleclickable = 0;
            }
            if (D_MOUSEDOUBLEHIT == drawHybridBar(bars[slot_idx], gPlayerNumber == actual_slot, xpos + offset*xposSc, ypos + 24.f*yposSc, 120.f, 350.f, 0.65f, HB_TAIL_RIGHT|HB_FLASH, 1.f, H_ALIGN_LEFT, V_ALIGN_CENTER )
                && doubleclickable)
            {
                *enterWithCurrentSelectedCharacter = 1;
            }
        }

        // Create Character
        font( &game_14 );
        font_color( color, color );

         if( bCreateCharSlot )
        {
            icon_overlay = atlasLoadTexture("icon_createcharacter_0.tga");
             display_sprite_positional( icon_overlay, xpos + offset*xposSc/2.f, ypos + LINE_HEIGHT*yposSc/2, 121.f, 1.f, 1.f, clr, H_ALIGN_CENTER, V_ALIGN_CENTER );
        }
        else
        {
            icon_overlay = atlasLoadTexture("CreateSymbol_Lock.tga");
             display_sprite_positional( icon_overlay, xpos + offset*xposSc/2.f, ypos + LINE_HEIGHT*yposSc/2.f, 121.f, icon_sc, icon_sc, clr, H_ALIGN_CENTER, V_ALIGN_CENTER );
        }
    }
    else
    {
        int foundServer = FALSE;

        // Ignore mouse clicks if something else is going on otherwise
        // we can change the slot underneath a delete or transfer.
        if( (selectCharacterState == SELECT_CHARACTER_CHOOSE) && mouseCollision(&box) )
        {
            clr = 0xffffff66;
            mouseover = true;

            if( isDown(MS_LEFT) )
                clr = 0xffffffff;

            if (mouseDown(MS_LEFT))
            {
                if(!pSlot || stricmp(pSlot->name, "EMPTY")==0)
                {
                    dialogClearQueue( 0 );
                    sndPlay( "charcreate", SOUND_GAME );
                    gPlayerNumber = actual_slot;
                    doCreatePlayer(0);
                    start_menu( MENU_ORIGIN );
                    resetCharacterCreation();
                }
                else
                {
                    sndPlay( "lock5", SOUND_GAME );
                    if(pSlot->costume->parts[0]->pchGeom==NULL)
                        dbGetCostume(actual_slot);

                    gPlayerNumber = actual_slot;
                    doDelAllCostumeFx(playerPtrForShell(0), 0);
                    if(db_info.players[gPlayerNumber].invalid)
                    {
                        dialogStd(DIALOG_OK, "InvalidPlayerInfo", NULL, NULL, NULL, NULL, 0);
                    }
                }
            }
        }

        // keep the current selection highlighted
        if(gPlayerNumber == actual_slot)
        {
            Entity *e = playerPtr();
            color = CLR_WHITE;
            clr = CLR_WHITE;

            if(e)
            {
                if( !pSlot->costume->appearance.convertedScale )
                    costume_retrofitBoneScale(pSlot->costume);

                // use the entity to host our slot costume for manipulation
                {
                    EntCostumeRestoreBlock costume_save;
                    entDetachCostume(e, &costume_save );

                    // swap the slot costume onto the entity
                    entSetMutableCostumeUnowned( e, pSlot->costume );
                    costume_Apply(e);

                    // HACK: Grrrr.
                    scaleHero(e, (((e->costume->appearance.fScale/100.0f + 1.0f)*.80f)-1.0f)*100.0f);

                    entRestoreDetachedCostume( e, &costume_save );    // restore original costume
                }
            }
        }
        else
            color = SELECT_FROM_UISKIN(CLR_PARAGON, CLR_VILLAGON, CLR_ROGUEAGON);

        // display the user name
        {
            char    buf[1000];      
            int selected = false;
            static float timer = 0;
            float sc = 1.5f;
            float inner_bar_sc = 0.5f*font_sc;
            float outer_bar_sc = 0.8f*font_sc;

            int fColor;

            const CharacterClass *pClass = pSlot->invalid?0:classes_GetPtrFromName(&g_CharacterClasses, pSlot->playerslot_class);
            const CharacterOrigin *pOrigin = pSlot->invalid?0:origins_GetPtrFromName(&g_CharacterOrigins, pSlot->origin);

            const char *alignment = NULL;

            if(pSlot->praetorian == kPraetorianProgress_Tutorial)
            {
                icon = atlasLoadTexture( "createsymbol_Praetorian.tga" );
                alignment = "PraetorianString";
            }
            else if(pSlot->praetorian == kPraetorianProgress_Praetoria)
            {
                if(pSlot->type == kPlayerType_Hero)
                {
                    icon = atlasLoadTexture( "createsymbol_Resistance.tga" );
                    alignment = "ResistanceStr";
                }
                else
                {
                    icon = atlasLoadTexture( "createsymbol_Praetorian.tga" );
                    alignment = "LoyalistStr";
                }
            }
            else if(pSlot->praetorian == kPraetorianProgress_TravelHero)
            {
                icon = atlasLoadTexture( "createsymbol_hero.tga" );
                alignment = "HeroAlignment";
            }
            else if(pSlot->praetorian == kPraetorianProgress_TravelVillain)
            {
                icon = atlasLoadTexture( "createsymbol_villain.tga" );
                alignment = "VillainAlignment";
            }
            else if (pSlot->praetorian == kPraetorianProgress_NeutralInPrimalTutorial)
            {
                icon = atlasLoadTexture( "createsymbol_neutral.tga" );
            }
            else if(pSlot->type)
            {
                if(pSlot->subtype == kPlayerSubType_Rogue)
                {
                    icon = atlasLoadTexture( "createsymbol_Rogue.tga" );
                    alignment = "RogueAlignment";
                }
                else
                {
                    icon = atlasLoadTexture( "createsymbol_Villain.tga" );
                    alignment = "VillainAlignment";
                }
            }
            else
            {
                if(pSlot->subtype == kPlayerSubType_Rogue)
                {
                    icon = atlasLoadTexture( "createsymbol_Vigilante.tga" );
                    alignment = "VigilanteAlignment";
                }
                else
                {
                    icon = atlasLoadTexture( "createsymbol_hero.tga" );
                    alignment = "HeroAlignment";
                }
            }

            if(!pSlot->invalid)
            {
                // side icon 
                CBox cbox;
                ClientCheckInfo unused;

                BuildScaledCBox(&cbox, xpos + (offset-icon->width*icon_sc)*xposSc/2.f, ypos + (LINE_HEIGHT-icon->height*icon_sc)*yposSc/2.f, icon->width*icon_sc*xposSc, icon->height*icon_sc*ypos);
                if (alignment && mouseCollision(&cbox))
                {
                    setHybridTooltip(&cbox, alignment);
                }
                display_sprite_positional( icon, xpos + offset*xposSc/2.f, ypos + LINE_HEIGHT*yposSc/2.f, 121.f, icon_sc, icon_sc, clr, H_ALIGN_CENTER, V_ALIGN_CENTER );
                if (pSlot && ClientCheckDisallowedPlayer(pSlot, &unused))
                {
                    icon_overlay = atlasLoadTexture("CreateSymbol_Lock.tga");
                     display_sprite_positional( icon_overlay, xpos + offset*xposSc/2.f, ypos + LINE_HEIGHT*yposSc/2.f, 121.f, icon_sc, icon_sc, clr, H_ALIGN_CENTER, V_ALIGN_CENTER );
                }
            }

            {
                // bar
                static HybridElement **bars = NULL;
                int barRet;
                if (eaSize(&bars) <= slot_idx)
                {
                    eaSetSize(&bars, slot_idx + 1);
                }
                if (!bars[slot_idx])
                {
                    bars[slot_idx] = calloc(1, sizeof(HybridElement));
                }
                bars[slot_idx]->text = pSlot->name;
                bars[slot_idx]->text_flags = NO_MSPRINT; // Make sure the character name does not get translated
                barRet = drawHybridBar(bars[slot_idx], gPlayerNumber == actual_slot, xpos + offset*xposSc, ypos + 24.f*yposSc, 120.f, 350.f, 0.7f, HB_TAIL_RIGHT|HB_FLASH, 1.f, H_ALIGN_LEFT, V_ALIGN_CENTER );
                if (D_MOUSEDOUBLEHIT == barRet)
                {
                    *enterWithCurrentSelectedCharacter = 1;
                }
                if (D_MOUSEOVER == barRet)
                {
                    *hoveredSlotIdx = actual_slot;
                    BuildScaledCBox(hoveredSlotCBox, xpos + offset*xposSc, ypos + 5.8f*yposSc, 350.f*xposSc, 36.4f*yposSc );
                }
            }

            if( mouseLeftDrag(&box) && !pSlot->invalid)
            {
                TrayObj tmp;
                builPlayerSlotTrayObj( &tmp, slot_idx, pSlot->name );
                trayobj_startDragging( &tmp, icon, icon );
            }

            if(gPlayerNumber == actual_slot)
            {
                timer += TIMESTEP*.1;
                sc = (0.75f)* outer_bar_sc + .25f*sinf(timer);

#if SHOW_MAPSERVER_STATUS
                if (pSlot->map_seconds < 0)
                    sprintf(buf,"%s (Not Started)", textStdEx(TRANSLATE_NOPREPEND, pSlot->map_name));
                else if (pSlot->map_seconds < 15)
#endif
                {
                    sprintf(buf,"%s", textStd("MapServerText", textStdEx(TRANSLATE_NOPREPEND, pSlot->map_name)));
                }
#if SHOW_MAPSERVER_STATUS
                else
                    sprintf(buf,"%s (Stuck %d:%02d)",textStdEx(TRANSLATE_NOPREPEND, pSlot->map_name),pSlot->map_seconds/60,pSlot->map_seconds%60);
#endif
                
                clipperPush(NULL); //remove clipper for now
                font(&title_14);
                font_outl(0);
                fColor = CLR_WHITE;
                font_color(fColor, fColor);
                cprntEx(512.0f, 750.0f, 120.0f, 1.f, 1.f, CENTER_X|NO_PREPEND, buf);
                font_outl(1);
                clipperPop();  //restore clipper
            }

            setSpriteScaleMode(SPRITE_XY_NO_SCALE);
            fColor = gPlayerNumber == actual_slot ? CLR_WHITE : CLR_WHITE & 0xffffff66;
            font( &hybridbold_12 );
            font_color(fColor, fColor);
            { 
                   int last_online_days = ((timerSecondsSince2000()-pSlot->last_online) / 86400);
                sprintf(buf, "%s - ", textStd("LevelLevel",pSlot->level+1));
                sprintf(buf, "%s%s ", buf, textStd(pOrigin?pOrigin->pchDisplayName:"OfflineString"));
                sprintf(buf, "%s%s ", buf, textStd(pClass?pClass->pchDisplayName:"OfflineString"));
                if( last_online_days < 40000)
                    sprintf(buf, "%s- %i %s", buf, last_online_days, textStd("DaysOffline") ); 
                prnt(xpos + 35.f*xposSc, ypos + (LINE_HEIGHT/2 + 18*font_sc)*yposSc, 125.f, 0.65f*font_sc*textXSc, 0.65f*font_sc*textYSc, buf);
            }
            setSpriteScaleMode(SPRITE_Y_UNIFORM_SCALE);
        }
    }
}

static float characterPageOffset;
static float f_CharSelectTarget = 0;
void setCharPage( int forward )
{
     if( forward && f_CharSelectTarget < 1000.f ) 
        f_CharSelectTarget += 500.f;
    else if( !forward && f_CharSelectTarget > 0 )
        f_CharSelectTarget -= 500.f;
}

static void drawAllCharacterSlots(bool only_current, int *first_create_slot, int * enterWithCurrentSelectedCharacter, int * hoveredSlotIdx, CBox * hoveredSlotCBox)
{
    int i, count=0;
    int y = 70;
     int display_slots = db_info.base_slots + MAX_SLOTS_PER_SHARD;
    int actual_slots = MAX(getTotalServerSlots(), db_info.player_count);
    int free_slots = getTotalServerSlots() - getSlotUnlockedPlayerCount();
    int free_player_unlockable_slots = MAX_SLOTS_PER_SHARD - getSlotUnlockedPlayerCount();
    UIBox box;
    CBox cbox, cbox2;
    int x = 100-characterPageOffset;
    F32 slideSpeed = 25.f;
    int bulletSelected;
    int pageStep = 500;
    int pageMax = 1500;
    static HybridElement sButtonBackPage = {0, NULL, "PreviousCharacterPage", "icon_browsearrow_rotate_0"};
    static HybridElement sButtonNextPage = {0, NULL, "NextCharacterPage", "icon_browsearrow_rotate_0"};
    static HybridElement sButtonBullet[4] = 
    {
        {0, NULL, NULL, "icon_pageindicator_0"},
        {0, NULL, NULL, "icon_pageindicator_0"},
        {0, NULL, NULL, "icon_pageindicator_0"},
        {0, NULL, NULL, "icon_pageindicator_0"}
    };
    F32 xposSc, yposSc;
    calculatePositionScales(&xposSc, &yposSc);
    *hoveredSlotIdx = -1; //reset hovered

    font( &game_12 );
    font_color( CLR_WHITE, 0xbbbbffff );

    if(only_current)
        drawCharacterSlot(gPlayerNumber, true, x*xposSc, y*yposSc, &free_slots, &free_player_unlockable_slots, first_create_slot, enterWithCurrentSelectedCharacter, hoveredSlotIdx, hoveredSlotCBox);
    else
    {
        int left_button_flags;
        int right_button_flags;
        F32 prev_offset;
        if(stricmp(db_info.players[gPlayerNumber].name, "EMPTY")==0 && s_pendingPlayerNumber == -1)
        {
            // Attempt to reselect a character if they have any.
            int j;
            for(j=0; j<db_info.max_slots; j++)
            {
                if( stricmp(db_info.players[j].name, "EMPTY")!=0 ) 
                {
                    gPlayerNumber = j; 

                    if( db_info.players[j].seconds_till_offline )
                    {
                        costume_init(db_info.players[j].costume);
                        dbGetCostume(gPlayerNumber);
                    }
                    break;
                }
            }
        }

        if(cursor.dragging)
        {
            {
                BuildScaledCBox( &cbox, pageStep*xposSc, 35*yposSc, 50*xposSc, 645*yposSc );
                if( mouseCollision(&cbox) )
                {
                    f_CharSelectTarget = MIN(pageMax, (((int)characterPageOffset / pageStep) + 1) * pageStep);
                }
            }
            {
                    BuildScaledCBox( &cbox, 20*xposSc, 35*yposSc, 50*xposSc, 600*yposSc );
                BuildScaledCBox( &cbox2, 20*xposSc, 635*yposSc, 70*xposSc, 40*yposSc );
                if( mouseCollision(&cbox) || mouseCollision(&cbox2) )
                {
                    f_CharSelectTarget = MAX(0, (((int)characterPageOffset - 1) / pageStep) * pageStep); // 1 is an epsilon that is much smaller than pageStep
                }
            }
        }

        left_button_flags = HB_FLIP_H | HB_DRAW_BACKING;
        if (f_CharSelectTarget <= 0)
        {
            left_button_flags |= HB_DISABLED;
        }
        if (D_MOUSEHIT == drawHybridButton(&sButtonBackPage, 200.f*xposSc, 700.f*yposSc, ZOOM_Z, 0.6f, CLR_WHITE, left_button_flags))
        {
            sndPlay("N_SelectSmall", SOUND_GAME);
            sndPlay("N_CharPageScroll", SOUND_UI_ALTERNATE);
            f_CharSelectTarget -= pageStep;
            if (f_CharSelectTarget < 0)
            {
                f_CharSelectTarget = 0;
            }
        }

        right_button_flags = HB_DRAW_BACKING;
        if (f_CharSelectTarget >= pageMax)
        {
            right_button_flags |= HB_DISABLED;
        }
        if (D_MOUSEHIT == drawHybridButton(&sButtonNextPage, 350.f*xposSc, 700.f*yposSc, ZOOM_Z, 0.6f, CLR_WHITE, right_button_flags))
        {
            sndPlay("N_SelectSmall", SOUND_GAME);
            sndPlay("N_CharPageScroll", SOUND_UI_ALTERNATE);
            f_CharSelectTarget += pageStep;
            if (f_CharSelectTarget > pageMax)
            {
                f_CharSelectTarget = pageMax;
            }
        }

        for(i = 0; i < 4; ++i) // hard-coded 4 pages
        {
            char pageText[64];
            bulletSelected = f_CharSelectTarget == i * pageStep;
            sprintf(pageText, "%s", textStd("CharacterPage", i+1));
            sButtonBullet[i].desc1 = pageText;
            if (D_MOUSEHIT == drawHybridButton(&sButtonBullet[i], (250.f+i*16.f)*xposSc, 700.f*yposSc, ZOOM_Z, 1.f, bulletSelected ? CLR_WHITE : 0x777777ff, HB_LIGHT_UP))
            {
                sndPlay("N_SelectSmall", SOUND_GAME);
                sndPlay("N_CharPageScroll", SOUND_UI_ALTERNATE);
                f_CharSelectTarget = i * pageStep;
            }
        }
        
        prev_offset = characterPageOffset;
        if( characterPageOffset < f_CharSelectTarget )
        {
            characterPageOffset += TIMESTEP*slideSpeed;
            if( characterPageOffset >= f_CharSelectTarget )
            {
                characterPageOffset = f_CharSelectTarget;
            }
            else
            {
                F32 pageBoundary;
                //check if we need to renew the pagescroll sound
                for (i = 1, pageBoundary = i * pageStep; pageBoundary < f_CharSelectTarget; ++i, pageBoundary = i * pageStep)
                {
                    if (prev_offset < pageBoundary && pageBoundary < characterPageOffset)
                    {
                        sndPlay("N_CharPageScroll", SOUND_UI_ALTERNATE);
                        break;
                    }
                }
            }
        }
        else
        {
            characterPageOffset -= TIMESTEP*slideSpeed;
            if( characterPageOffset <= f_CharSelectTarget )
            {
                characterPageOffset = f_CharSelectTarget;
            }
            else
            {
                F32 pageBoundary;
                //check if we need to renew the pagescroll sound
                for (i = 3, pageBoundary = i * pageStep; pageBoundary > f_CharSelectTarget; --i, pageBoundary = i * pageStep)
                {
                    if (prev_offset > pageBoundary && pageBoundary > characterPageOffset)
                    {
                        sndPlay("N_CharPageScroll", SOUND_UI_ALTERNATE);
                        break;
                    }
                }
            }
        }

         uiBoxDefineScaled(&box, 70*xposSc, 30*yposSc, 400*xposSc, 700*yposSc ); 
         clipperPush( &box );
          for(i = 0; i < display_slots; i++, count++)
        {
             drawCharacterSlot(i, false, x*xposSc, (y+count*LINE_HEIGHT)*yposSc, &free_slots, &free_player_unlockable_slots, first_create_slot, enterWithCurrentSelectedCharacter, hoveredSlotIdx, hoveredSlotCBox);
            if( count >= 11 )
            {
                 x += pageStep;
                count = -1;
            }
        }
        clipperPop();
    }
}

static void s_returnToServerSelect(void *notused)
{
    char password[MAX_PASSWORD_LEN];
    g_ServerName[0] = 0;
    toggle_3d_game_modes(SHOW_NONE);
    dialogClearQueue( 0 );
    gPlayerNumber = 0;
    doDelAllCostumeFx(playerPtr(), 0);
    dbDisconnect();
    cryptRetrieve(password, g_achPassword, sizeof(g_achPassword));
    if (authReloginIfNeeded(password))
    {
        s_loggedIn_serverSelected = LOGIN_STAGE_SERVER_SELECT;
    }
    else
    {
        dialogStd( DIALOG_OK, authGetError(), NULL, NULL, 0, NULL, 0 );
        s_loggedIn_serverSelected = LOGIN_STAGE_START;
    }
    s_CharacterSelectInit = 0;
    selectCharacterState = SELECT_CHARACTER_CHOOSE;
    s_SendInfoToTransgaming = true;
    memset(password, 0, sizeof(password));
}

static void drawTopBar(F32 cx, F32 y)
{
    AtlasTex *back[] = 
    {
        atlasLoadTexture("toptab_long_0"),
        atlasLoadTexture("toptab_long_1"),
        atlasLoadTexture("toptab_long_2")
    };
    AtlasTex * topBase = atlasLoadTexture("toptab_long_base_tip_0");
    AtlasTex * mid = atlasLoadTexture("toptab_long_mid_0");
    AtlasTex * midBase = atlasLoadTexture("toptab_long_base_mid_0");
    int i;

    display_sprite_positional(topBase, cx, y, 10.f, 1.f, 1.f, CLR_H_TAB_BACK&0xffffff7f, H_ALIGN_CENTER, V_ALIGN_TOP );
    for (i = 0; i < 3; ++i)
    {
        display_sprite_positional(back[i], cx, y, 10.f, 1.f, 1.0f, HYBRID_FRAME_COLOR[i]&0xffffff64, H_ALIGN_CENTER, V_ALIGN_TOP );
    }
    y += back[0]->height;
    display_sprite_positional_blend(midBase, cx, y, 10.f, 1.f, 200.f/midBase->height, CLR_H_TAB_BACK&0xffffff7f, CLR_H_TAB_BACK&0xffffff7f, CLR_H_TAB_BACK&0xffffff00, CLR_H_TAB_BACK&0xffffff00, H_ALIGN_CENTER, V_ALIGN_TOP );
    display_sprite_positional_blend(mid, cx, y, 10.f, 1.f, 200.f/mid->height, HYBRID_FRAME_COLOR[0]&0xffffff64, HYBRID_FRAME_COLOR[0]&0xffffff64, HYBRID_FRAME_COLOR[0]&0xffffff00, HYBRID_FRAME_COLOR[0]&0xffffff00, H_ALIGN_CENTER, V_ALIGN_TOP );
}

static int drawVIPButton(F32 cx, F32 cy)
{
    static F32 spinAngle;
    AtlasTex *back1 = atlasLoadTexture("icon_VIP_full_1");
    AtlasTex *back2 = atlasLoadTexture("icon_VIP_full_0");
    CBox cbox;
    int pulse;
    F32 xposSc, yposSc;
    calculatePositionScales(&xposSc, &yposSc);

    BuildScaledCBox(&cbox, cx-back1->width*xposSc/2.f, cy-back1->height*yposSc/2.f, back1->width*xposSc, back1->height*yposSc);
    spinAngle += TIMESTEP * 0.01f;
    spinAngle = fmodf(spinAngle, TWOPI);
    pulse = min(max((int)(255.f * ((sin(spinAngle*16.f) / 4.f) + 0.75f)), 0), 255);
    display_sprite_positional_rotated( back1, cx, cy, 10.f, 1.f, 1.f, 0xfff9e500 | pulse, spinAngle, 0, H_ALIGN_CENTER, V_ALIGN_CENTER );
    display_sprite_positional( back2, cx, cy, 10.f, 1.f, 1.f, 0xffffffff, H_ALIGN_CENTER, V_ALIGN_CENTER );
    return mouseClickHit(&cbox, MS_LEFT) ? D_MOUSEHIT : D_NONE;
}

static void drawGlobalSlotsInfo()
{
    if (s_loggedIn_serverSelected == LOGIN_STAGE_CHARACTER_SELECT)
    {
        font( &hybridbold_12 );
        font_color( CLR_WHITE, CLR_WHITE );
        if (db_info.players)
        {
            cprntEx(75.f, 39.f, 120.0f, 1.f, 1.f, 0, "serverSlotsLabel", getSlotUnlockedPlayerCount(), getTotalServerSlots());
        }
        if (inventoryClient_GetAcctAuthoritativeState() == ACCOUNT_SERVER_UP)
        {
            cprntEx(75.f, 24.f, 120.0f, 1.f, 1.f, 0, "globalSlotsLabel", AvailableUnallocatedCharSlots());
        }
    }
}

static int countPendingRenames()
{
    int i, count = 0;
    for(i = 0; i < db_info.max_slots; ++i)
    {
        if (db_info.players[i].db_flags & DBFLAG_RENAMEABLE)
        {
            ++count;
        }
    }
    return count;
}

static void drawTokenCounts()
{
    int pendingRenames = countPendingRenames();
    F32 renameOffset = 0.f;
    font( &hybridbold_12 );
    font_color( CLR_WHITE, CLR_WHITE );
    if (pendingRenames)
    {
        cprntEx(605.f, 39.f, 120.0f, 1.f, 1.f, 0, "charRenamePendingTokensLabel", pendingRenames);
        renameOffset = -15.f;
    }
    cprntEx(605.f, 39.f+renameOffset, 120.0f, 1.f, 1.f, 0, "charRenameTokensLabel", AccountGetStoreProductCount(&db_info.account_inventory, SKU("svrename"), true));
    cprntEx(775.f, 39.f, 120.0f, 1.f, 1.f, 0, "shardXfersTokenLabel", db_info.shardXferFreeOnly ?
                            db_info.shardXferTokens : AccountGetStoreProductCount(&db_info.account_inventory, SKU("svsvtran"), true));
}

static void drawUnspentPointsInfo()
{
    font( &hybridbold_12 );
    font_color( CLR_WHITE, CLR_WHITE );
    cprntEx(290.f, 39.f, 120.0f, 1.f, 1.f, 0, "unspentTokensLabel", db_info.loyaltyPointsUnspent);
}

static void drawHybridCommon_CharacterSelectOrInQueue(int nextLocked)
{
    F32 xposSc, yposSc, xp, yp;
    float screenScaleX, screenScaleY, scale;
    scale = screenScaleX = screenScaleY = 1.f;

    drawHybridCommon(HYBRID_MENU_NO_NAVIGATION);

    drawTopBar(DEFAULT_SCRN_WD/2.f, 3.f);
    drawGlobalSlotsInfo();
    if (s_loggedIn_serverSelected == LOGIN_STAGE_CHARACTER_SELECT && inventoryClient_GetAcctAuthoritativeState() == ACCOUNT_SERVER_UP)
    {
        drawUnspentPointsInfo();
        drawTokenCounts();
    }

    if (gLoyaltyTreeVisible)
    {
        F32 xsc, ysc;
        calc_scrn_scaling(&xsc, &ysc);
        loyaltyTreeMenuTick(ysc);  //centered
    }

    setScalingModeAndOption(SPRITE_Y_UNIFORM_SCALE, SSO_SCALE_TEXTURE);
    calculatePositionScales(&xposSc, &yposSc);
    xp = DEFAULT_SCRN_WD;
    yp = 0.f;
    applyPointToScreenScalingFactorf(&xp, &yp);
    drawLWCUI(xp-72.f*xposSc, 250.f*yposSc, 5.f, scale);

    if (s_loggedIn_serverSelected == LOGIN_STAGE_CHARACTER_SELECT && inventoryClient_GetAcctAuthoritativeState() == ACCOUNT_SERVER_UP && !AccountIsReallyVIPAfterServerSelected())
    {
        F32 textXSc, textYSc;
        calculateSpriteScales(&textXSc, &textYSc);
        if (drawVIPButton(xp - 74.f*xposSc, 138.f*yposSc))
        {
            dialogStd(DIALOG_YES_NO, "BecomeVIPPrompt", NULL, NULL, openStoreVIP, NULL, 0);
            sndPlay("N_Error", SOUND_GAME);
        }

        setSpriteScaleMode(SPRITE_XY_NO_SCALE);
        font( &hybridbold_12 );
        font_color( CLR_WHITE, CLR_WHITE );
        cprntEx(xp - 72.f*xposSc, 200.f*yposSc, 120.0f, textXSc, textYSc, CENTER_X, "VIPButtonCaption");
    }

    setScalingModeAndOption(SPRITE_XY_SCALE, SSO_SCALE_ALL);
    if ( drawHybridNext( DIR_LEFT_NO_NAV, nextLocked, "BackToChooseServerString" ))
    {
        if(s_loggedIn_serverSelected == LOGIN_STAGE_IN_QUEUE)
        {
            dialogTimed(DIALOG_YES_NO, "VerifyLeaveQueue", NULL, NULL, s_returnToServerSelect, NULL, 0, 0);
        }
        else
        {
            s_returnToServerSelect(NULL);
        }
    }
}

//tooltip should already be an estr
static void createLockedCharacterTooltip(char ** tooltip, const ClientCheckInfo *pClientCheck, SlotLock slotLock)
{
    estrConcatCharString(tooltip, textStd("CharLockTooltipHeader"));
    if (!skuIdIsNull(pClientCheck->skuAccessPraetorian))
    {
        estrConcatCharString(tooltip, textStd("CharLockMessagePraetorian"));
    }
    if (!skuIdIsNull(pClientCheck->skuAccessArchetype))
    {
        estrConcatCharString(tooltip, textStd("CharLockMessageArchetype", textStd(pClientCheck->strNoAccessArchetype) ));
    }
    if (pClientCheck->strNoAccessPrimaryPowerset)
    {
        estrConcatCharString(tooltip, textStd("CharLockMessagePowerset", textStd(pClientCheck->strNoAccessPrimaryPowerset)));
    }
    if (pClientCheck->strNoAccessSecondaryPowerset)
    {
        estrConcatCharString(tooltip, textStd("CharLockMessagePowerset", textStd(pClientCheck->strNoAccessSecondaryPowerset)));
    }
    if (isVipPremiumLocked())
    {
        estrConcatCharString(tooltip, textStd("CharLockMessageVIP"));
    }
    if (slotLock != SLOT_LOCK_NONE)
    {
        estrConcatCharString(tooltip, textStd("CharLockMessageSlot"));
    }
}

static int accountSlotLockNagAck()
{
    return regGetAppInt(getVipFreeRegValue(), 0);
}

static int isAnyCharacterSlotIsLocked()
{
    return db_info.player_count != getSlotUnlockedPlayerCount();
}

static bool s_nagCbValue = false;
static bool s_naggedThisSession;
static DialogCheckbox nagCheckbox[] =
{
    {    "SlotLockNagDisable", &s_nagCbValue, 0    }
};

static void slotLockDialogFunc(void *unused)
{
    s_naggedThisSession = true;
    if (s_nagCbValue)
    {
        disableSlotLockNagForThisAccount();
    }
}

static void handleCharacterSlotLockMessaging()
{
    if (db_info.showPremiumSlotLockNag && !s_naggedThisSession && !accountSlotLockNagAck() && isAnyCharacterSlotIsLocked())
    {
        const char *text = AccountIsReallyVIPAfterServerSelected() ? "SlotLockExplanationVip" : "SlotLockExplanationPremium";
        dialog(DIALOG_OK, -1, -1, -1, 300, textStd(text, AvailableUnallocatedCharSlots(), getTotalServerSlots() - getSlotUnlockedPlayerCount(), db_info.shardname), NULL, slotLockDialogFunc, NULL, NULL, 0, NULL, nagCheckbox, 1, 0, 0, 0);
    }
}

static bool sPromptedLWCThisSession = false;
static void handleLWCMessaging()
{
    const char* msgName;
    LWC_STAGE curStage;

    if(sPromptedLWCThisSession)
        return;

    sPromptedLWCThisSession = true;
    if( !LWC_IsActive() )
        return;

    // we get here one time only per exe run, and only if LWC is running.  Check current stage and give appropriate message.
    curStage = LWC_GetLastCompletedDataStage();
    switch(curStage)
    {
        case LWC_STAGE_1: msgName = "LWCStartupMsgStage2"; break;
        case LWC_STAGE_2: msgName = "LWCStartupMsgStage3"; break;
        case LWC_STAGE_3: msgName = "LWCStartupMsgStage4"; break;
        default: msgName = NULL; break; // shouldn't get here
    }

    if(msgName) {
        dialogStd(DIALOG_OK, textStd(msgName), NULL, NULL, NULL, NULL, 0); 
    }
}

static void selectCharacter()
{
    AtlasTex * floorIcon;
    PlayerSlot * slot;
    AtlasTex * botline = atlasLoadTexture("botline_0");

    ClientCheckInfo clientCheck;
    bool bNoAccess = 0;
    bool bEmpty = 0;
    bool bAlmostLegalCharacterSelected;
    bool bLegalCharacterSelected;
    bool bLegalCharacterSelectedTransfer;
    int enableDeleteButton;
    int w, h;
    static int trial_approach = 1;
    char *enter_str = NULL;
    int legalCharacterFlags;
    int legalCharacterFlagsTransfer;
    int first_create_slot = -1;
    int lockedByVIPShard = isVipPremiumLocked();
    int enterWithCurrentSelectedCharacter = 0;
    int hoveredSlotIdx;
    CBox hoveredSlotCBox;
    int lockedEntry = 0;
    char *tooltip = NULL;
    AccountServerStatus accountStatus = inventoryClient_GetAcctAuthoritativeState();
    static HybridElement sButtonTransfer = {0, NULL, "charTransferButton", "icon_transfer"};
    static HybridElement sButtonRename = {0, NULL, "charRenameButton", "icon_rename"};
    static HybridElement sButtonCreate = {0, NULL, "charCreateButton", "icon_createcharacter"};
    static HybridElement sButtonLoyalty = {0, NULL, "loyaltyTreeButton", "icon_loyaltytree"};
    static HybridElement sButtonLock = {0, NULL, NULL, "icon_character_lock"};
    static HybridElement sButtonDelete = {0, NULL, "charDeleteButton", "icon_deletecharacter"};

    if( !db_info.players )
    {
        return;
    }

    windowClientSizeThisFrame( &w, &h );

    slot = &db_info.players[gPlayerNumber];
    if(slot->praetorian == kPraetorianProgress_Tutorial)
    {
        floorIcon = atlasLoadTexture( "flooricon_praetorianlogo" );
    }
    else if(slot->praetorian == kPraetorianProgress_Praetoria)
    {
        if(slot->type == kPlayerType_Hero)
        {
            floorIcon = atlasLoadTexture( "flooricon_resistancelogo" );
        }
        else
        {
            floorIcon = atlasLoadTexture( "flooricon_loyalistlogo" );
        }
    }
    else if(slot->praetorian == kPraetorianProgress_TravelHero)
    {
        floorIcon = atlasLoadTexture( "flooricon_herologo" );
    }
    else if(slot->praetorian == kPraetorianProgress_TravelVillain)
    {
        floorIcon = atlasLoadTexture( "flooricon_villainlogo" );
    }
    else if (slot->praetorian == kPraetorianProgress_NeutralInPrimalTutorial)
    {
        floorIcon = atlasLoadTexture( "flooricon_hero_villainlogo" );
    }
    else if(slot->type)
    {
        if(slot->subtype == kPlayerSubType_Rogue)
        {
            floorIcon = atlasLoadTexture( "flooricon_roguelogo" );
        }
        else
        {
            floorIcon = atlasLoadTexture( "flooricon_villainlogo" );
        }
    }
    else
    {
        if(slot->subtype == kPlayerSubType_Rogue)
        {
            floorIcon = atlasLoadTexture( "flooricon_vigilantelogo" );
        }
        else
        {
            floorIcon = atlasLoadTexture( "flooricon_herologo" );
        }
    }

    if(stricmp(slot->name, "EMPTY" )==0)
        bEmpty = 1;

    if( slot->seconds_till_offline && !bEmpty && !slot->invalid )
    {
        setSpriteScaleMode(SPRITE_Y_UNIFORM_SCALE);
         display_sprite_positional( floorIcon, 690.f, 652.f, 5, 1.f, 1.f, 0xffffff60, H_ALIGN_CENTER, V_ALIGN_TOP );

        if( !isStoreHidingCharacterSlots() )
        {
            // Get rid of the arrows when the slots and name etc are also
            // hidden, just to keep everything tidy looking.
            if (selectCharacterState != SELECT_CHARACTER_STORE)
            {
                drawAvatarControls(695.f, 670.f, NULL, 1);
            }
            moveAvatar(AVATAR_LOGIN);
        }
        setSpriteScaleMode(SPRITE_XY_SCALE);
    }

    if(!s_CharacterSelectInit)
    {
        s_CharacterSelectInit = 1;

        playerSlotsLoad();
        playerNote_Load();

        // ppd pointers should be persisted if character creation has been started even if booted to login.
        // This state fix-up prevents a crash, but ppd.player shouldn't be 0 if s_CharacterCreationStarted is 1.
        if (s_CharacterCreationStarted && !playerPtr())
        {
            s_CharacterCreationStarted = 0;
        }

        if (!s_CharacterCreationStarted)
        {
            Entity *e;

            doCreatePlayer(0);

            e = playerPtr();

            // set initial values
            costume_SetScale(costume_as_mutable_cast(e->costume), 0);
            costume_SetBoneScale(costume_as_mutable_cast(e->costume), 0);
            changeBoneScale(e->seq, 0);
        }
        player_being_created = 0;

        playerTurn( 0 );
        s_pendingPlayerNumber = -1;

        if( slot->seconds_till_offline && (!slot->costume || !slot->costume->parts[0] || !slot->costume->parts[0]->pchGeom))
        {
            dbGetCostume(gPlayerNumber);
        }
    }

    if(selectCharacterState == SELECT_CHARACTER_CHOOSE)
    {
        // Send information to Transgaming every time a paying Mac user logs in.
        if (IsUsingCider() && s_SendInfoToTransgaming && AccountIsReallyVIPAfterServerSelected())
        {
            s_SendInfoToTransgaming = false;
            NotifyTransgamingOfLogin(g_achAccountName);
        }
    }

    setScalingModeAndOption(SPRITE_Y_UNIFORM_SCALE, SSO_SCALE_TEXTURE);
    drawAllCharacterSlots( characterTransferAreSlotsHidden(), &first_create_slot, &enterWithCurrentSelectedCharacter, &hoveredSlotIdx, &hoveredSlotCBox);
    setScalingModeAndOption(SPRITE_XY_SCALE, SSO_SCALE_ALL);

    if (hoveredSlotIdx > -1)
    {
        PlayerSlot * hoverslot = &db_info.players[hoveredSlotIdx];
        if (stricmp(hoverslot->name, "EMPTY") != 0) //no need to create a tooltip for an empty slot
        {
            bNoAccess = ClientCheckDisallowedPlayer(hoverslot, &clientCheck);
            if (bNoAccess)
            {
                estrCreate(&tooltip);
                createLockedCharacterTooltip(&tooltip, &clientCheck, hoverslot->slot_lock);
                setHybridTooltip(&hoveredSlotCBox, tooltip);
                estrDestroy(&tooltip);
            }
        }
    }

    if( !slot->seconds_till_offline || bEmpty )
        toggle_3d_game_modes(SHOW_NONE);
    else
        toggle_3d_game_modes(SHOW_TEMPLATE);

    if( !slot->seconds_till_offline && !bEmpty )
    {
        drawMenuFrame( R12, 600, 110, 50, 260, 520,CLR_WHITE, 0x00000088, 0 );
        smf_ParseAndDisplay( NULL, textStd("OfflineCharMsg"), 612, 122, 50, 228, 488, 1, 1, &gTextAttr, NULL, 0, true);  
    }

    bNoAccess = ClientCheckDisallowedPlayer(slot, &clientCheck);

    handleCharacterSlotLockMessaging();
    
    handleLWCMessaging();

    // Have we spent too long waiting for the name response?
    if( checkNameWaiting && ( timerSecondsSince2000() - checkNameStartTime ) > CHECK_NAME_TIMEOUT )
    {
        checkNameWaiting = false;
        dialogStd( DIALOG_OK, "checkNameTimeout", NULL, NULL, checkNameResponseOk, NULL, 0 );
    }

    // What about the rename response?
    if( renameWaiting && ( timerSecondsSince2000() - renameStartTime ) > RENAME_TIMEOUT )
    {
        renameWaiting = false;
        dialogStd( DIALOG_OK, "renameTimeout", NULL, NULL, renameResponseOk, NULL, 0 );
    }

    bAlmostLegalCharacterSelected = selectCharacterState == SELECT_CHARACTER_CHOOSE && !slot->invalid && !bEmpty;
    bLegalCharacterSelected = bAlmostLegalCharacterSelected && !bNoAccess;
    bLegalCharacterSelectedTransfer = bAlmostLegalCharacterSelected && (!bNoAccess || clientCheck.onlyVipShardLocked);

    enter_str = "PlayGameString";

    if( !bEmpty )
    {
        //    bad hack =/
        //    rather than adding more flags for maps to say what kind of location they are,
        //    we assume that if they are on a P_ map, it was a praetorian map
        //    I really hope we don't change from that naming convention o.O
        int isPraetorianMap = slot->map_name && strStartsWith(slot->map_name, "P_");
        if( slot->praetorian == kPraetorianProgress_Tutorial || slot->praetorian == kPraetorianProgress_Praetoria || isPraetorianMap)
            enter_str = "Praetorian_PlayGameString";
        else if( slot->typeByLocation == kPlayerType_Villain )
            enter_str = "Villain_PlayGameString";
        else
            enter_str = "Hero_PlayGameString";
    }

    // Allow the character delete button to be clicked if we're on a
    // valid character and haven't started doing anything else.
    enableDeleteButton = selectCharacterState == SELECT_CHARACTER_CHOOSE && !bEmpty;

    legalCharacterFlags = bLegalCharacterSelected ? HB_DRAW_BACKING : (HB_DISABLED|HB_DRAW_BACKING);
    legalCharacterFlagsTransfer = bLegalCharacterSelectedTransfer ? HB_DRAW_BACKING : (HB_DISABLED|HB_DRAW_BACKING);

    setScalingModeAndOption(SPRITE_Y_UNIFORM_SCALE, SSO_SCALE_ALL);
    if (D_MOUSEHIT == drawHybridButton(&sButtonTransfer, 740.f, 31.f, 120.f, 1.f, CLR_WHITE, legalCharacterFlagsTransfer))
    {
        if (inventoryClient_GetAcctAuthoritativeState() == ACCOUNT_SERVER_UP)
        {
            if (db_info.shardXferFreeOnly)
            {
                if (db_info.shardXferTokens > 0)
                {
                    serverTransferStartRedeem(charSelectPurchaseCancelled);
                    selectCharacterState = SELECT_CHARACTER_STORE;
                    sndPlay("N_Error", SOUND_GAME);
                }
                else
                {
                    dialogStd(DIALOG_OK, "NoRemaningTransferTokens", NULL, NULL, NULL, NULL, 0);
                    sndPlay("N_Error", SOUND_GAME);
                }
            }
            else
            {
                if (AccountGetStoreProductCount(&db_info.account_inventory, SKU("svsvtran"), true) > 0)
                {
                    serverTransferStartRedeem(charSelectPurchaseCancelled);
                    selectCharacterState = SELECT_CHARACTER_STORE;
                    sndPlay("N_Error", SOUND_GAME);
                }
                else // If they don't have a transfer token already, maybe they can buy one.
                {
                    dialogStd(DIALOG_YES_NO, "AskStoreXfer", NULL, NULL, openStoreXfer, NULL, 0);
                    sndPlay("N_Error", SOUND_GAME);
                }
            }
        }
        else
        {
            dialogStd(DIALOG_OK, "InventoryNotLoadedXfer", NULL, NULL, NULL, NULL, 0);
            sndPlay("N_Error", SOUND_GAME);
        }
    }

    if (bLegalCharacterSelected && db_info.players[gPlayerNumber].db_flags & DBFLAG_RENAMEABLE)
    {
        if (D_MOUSEHIT == drawHybridButton(&sButtonRename, 570.f, 31.f, 120.f, 1.f, CLR_WHITE, HB_DRAW_BACKING))
        {
            startRename(gPlayerNumber);
        }
    }
    // If the character is not renamable, maybe they can spend a rename token.
    else if (inventoryClient_GetAcctAuthoritativeState() == ACCOUNT_SERVER_UP)
    {
        if (AccountGetStoreProductCount(&db_info.account_inventory, SKU("svrename"), true) > 0)
        {
            int flags = legalCharacterFlags;
            if (timerSecondsSince2000() - rename_claim_delay < 5)
            {
                flags |= HB_DISABLED;
            }

            if (D_MOUSEHIT == drawHybridButton(&sButtonRename, 570.f, 31.f, 120.f, 1.f, CLR_WHITE, flags))
            {
                dialogStd(DIALOG_YES_NO, "RenameTokenSpendConfirmation", NULL, NULL, startRenameTokenRedemption, NULL, 0);
                sndPlay("N_Error", SOUND_GAME);
            }
        }
        // If they don't have a rename token already, maybe they can buy one.
        else
        {
            if (D_MOUSEHIT == drawHybridButton(&sButtonRename, 570.f, 31.f, 120.f, 1.f, CLR_WHITE, legalCharacterFlags))
            {
                dialogStd(DIALOG_YES_NO, "AskStoreRename", NULL, NULL, openStoreRename, NULL, 0);
                sndPlay("N_Error", SOUND_GAME);
            }
        }
    }
    else
    {
        if (D_MOUSEHIT == drawHybridButton(&sButtonRename, 570.f, 31.f, 120.f, 1.f, CLR_WHITE, legalCharacterFlags))
        {
            dialogStd(DIALOG_OK, "InventoryNotLoadedRename", NULL, NULL, NULL, NULL, 0);
            sndPlay("N_Error", SOUND_GAME);
        }
    }
    if (D_MOUSEHIT == drawHybridButton(&sButtonCreate, 45.f, 31.f, 120.f, 1.f, CLR_WHITE, HB_DRAW_BACKING | (lockedByVIPShard ? HB_DISABLED : 0)))
    {
        if (first_create_slot != -1)
        {
            createCharacterAndStuff(first_create_slot);
        }
        else if (inventoryClient_GetAcctAuthoritativeState() == ACCOUNT_SERVER_UP)
        {
            if (48 - db_info.base_slots + getVIPAdjustedBaseSlots() <= db_info.player_count)
            {
                int isVIP = AccountIsReallyVIPAfterServerSelected();
                char *message = isVIP ? "HitShardCharacterLimit" : "HitShardCharacterLimitFree";
                dialogStd(DIALOG_OK, message, NULL, NULL, NULL, NULL, 0);
            }
            else if (AccountGetStoreProductCount(&db_info.account_inventory, SKU("svcslots"), true) > 0)
            {
                startRedeemSlot();
            }
            else
            {
                dialogStd(DIALOG_YES_NO, "AskStoreSlots", NULL, NULL, openStoreSlots, NULL, 0);
            }
        }
        else
        {
            dialogStd(DIALOG_OK, "InventoryNotLoadedSlots", NULL, NULL, NULL, NULL, 0);
        }
    }
    if (D_MOUSEHIT == drawHybridButton(&sButtonLoyalty, 260.f, 31.f, 120.f, 1.f, CLR_WHITE, HB_DRAW_BACKING))
    {
        gLoyaltyTreeVisible = 1;
    }
    if (!bEmpty && bNoAccess)
    {
        estrCreate(&tooltip);
        createLockedCharacterTooltip(&tooltip, &clientCheck, slot->slot_lock);
        sButtonLock.desc1 = tooltip;
        if (D_MOUSEHIT == drawHybridButton(&sButtonLock, 485.f, 31.f, 120.f, 1.f, CLR_WHITE, HB_DRAW_BACKING))
        {
            startCharacterUnlocking(slot->playerslot_class, &clientCheck, slot->slot_lock);
        }

        estrDestroy(&tooltip);
    }

    if (D_MOUSEHIT == drawHybridButton(&sButtonDelete, 900.f, 31.f, 120.f, 1.f, 0xbb0000ff, enableDeleteButton ? HB_DRAW_BACKING : (HB_DISABLED|HB_DRAW_BACKING)))
    {
        //Delete character here.
        selectCharacterState = SELECT_CHARACTER_DELETE;
        if( isDevelopmentMode() )
            dialogStd( DIALOG_YES_NO, textStd("Really Delete Character?",slot->name) , NULL, NULL, deleteCharacterOk, deleteCharacterCancel, 0 );
        else
        {
            strcpy( gCharacterToBeDeleted, slot->name );
            gCharacterToBeDeletedSlot = gPlayerNumber;
            dialogStd( DIALOG_OK_CANCEL_TEXT_ENTRY, textStd("DeleteCharacter?", gCharacterToBeDeleted) , NULL, NULL, deleteCharacterOk, deleteCharacterCancel, 0 );
        }
        sndPlay("N_Error", SOUND_GAME);
    }

    setScalingModeAndOption(SPRITE_XY_SCALE, SSO_SCALE_ALL);
    // bottom line
    display_sprite(botline, 512.f - botline->width/2.f, 723.f, 4.f, 1.f, 1.f, CLR_WHITE);
    lockedEntry = !bAlmostLegalCharacterSelected || lockedByVIPShard;
    if( drawHybridNext( DIR_RIGHT_LOGIN, lockedEntry, enter_str ) || (enterWithCurrentSelectedCharacter && !lockedEntry))
    {
        if (bLegalCharacterSelected)
        {
            LWC_STAGE required_stage = LWC_GetRequiredDataStageForMap(slot->map_name);

            if (LWC_CheckStageAndShowMessageIfNotReady(required_stage) && !db_info.players[gPlayerNumber].invalid)
            {
                g_ServerName[0] = 0;
                gLoggingIn = true;
                selectCharacterState = SELECT_CHARACTER_PLAY;
                sndPlay("N_Launch", SOUND_GAME);
                toggle_3d_game_modes(SHOW_NONE);
                dialogClearQueue( 0 );

                // Make sure any windows we used during the shell are closed (and for
                // that matter, anything left open by any previously logged in player).
                window_closeAlways();
                doCreatePlayer(0);

                player_being_created = 0;
                s_loggedIn_serverSelected = LOGIN_STAGE_LEAVING;

                if( !choosePlayerWrapper( gPlayerNumber, 0 ) )
                {
                    char    *s = dbGetError();

                    if (!s || !s[0])
                        s = textStd("NoMapResponse");
                    dialogStd( DIALOG_OK, s, NULL, NULL, 0, NULL, 0 );

                    // Don't go back to the login screen if the response was MapFailedOverloadProtection
                    if (stricmp(s,textStd("MapFailedOverloadProtection")) == 0)
                    {
                        // Allow the player to stay at the character selection screen
                        selectCharacterState = SELECT_CHARACTER_CHOOSE;
                    }
                    else
                    {
                        // Bump the player out to the login screen (probably got disconnected from the dbserver)
                        restartLoginScreen();
                    }
                }
                else
                {
                    dbDisconnect();  //Character being resumed, no longer need connServer.
                    player_being_created = FALSE;
                    if(!tryConnect())
                    {
                        dialogStd( DIALOG_OK, "NoMapserverConn", NULL, NULL, NULL, NULL, 0 );
                        restartLoginScreen();
                    }
                    else
                    {
                        gfxReload(1);    //Asks mapServer to send a world to load and creates an entity.
                        gfxPrepareforLoadingScreen();
                        if (commConnected()) //to do, remove from gfx.c
                        {
                            if(commReqScene(1))
                            {
                                start_menu( MENU_ORIGIN );
                                resetCharacterCreation();
                            }
                        }
                    }
                }
            }
        }
        else
        {
            startCharacterUnlocking(slot->playerslot_class, &clientCheck, slot->slot_lock);
        }
    }

    if( selectCharacterState == SELECT_CHARACTER_STORE )
    {
        characterTransferHandleProgress( 1.f, 1.f );
    }
}

//-------------------------------------------------------------------------------------------------------------
// Server Select
//-------------------------------------------------------------------------------------------------------------

#define SERVER_X    660
#define SERVER_Y    88
#define SERVER_WD   328
#define SERVER_HT   670
#define SERVER_LIST_Y    266
#define SERVER_LIST_HT    434
#define SERVER_LIST_ELEMENT_Y    31

int loginToDbServer( int idx,char *err_msg )
{
    int     ret,auth_logged_in=0;
    char    *s;

    err_msg[0] = 0;
    if (game_state.auth_address[0]) 
    {
        BOOL failed = FALSE;
        acSendAboutToPlay(auth_info.servers[idx].id);

        if(auth_info.auth2_enabled == Auth2EnabledState_QueueServer)
        {
            failed = !authWaitFor(AC_HANDOFFTOQUEUE);
            //ab: some kind of queue server code here, if we ever do it.
        }

        if(!failed)
            failed = !authWaitFor(AC_PLAY_OK);
        
        if (failed)
        {
            s = authGetError();
            if (s)
                strcpy(err_msg,s);
            return LOGIN_STAGE_START;
        }
        auth_logged_in = 1;
    }

    // cleaning up cache information - new information will be requested by login
    db_info.bonus_slots = 0;
    db_info.shardXferTokens = 0;
    db_info.shardname[0] = 0;

    // also clean up auth data because the DbServer will set the necessary
    // bits for us
    memset(db_info.auth_user_data, 0, AUTH_BYTES);

    AccountInventorySet_Release( &db_info.account_inventory );

    ret = dbConnect(makeIpStr(auth_info.servers[idx].ip),auth_info.servers[idx].port,
                    auth_info.uid,auth_info.game_key, g_achAccountName,
                    game_state.local_map_server | game_state.no_version_check, control_state.notimeout?NO_TIMEOUT:90.0f,control_state.notimeout);
    if (!ret || ret == DBGAMESERVER_MSG)
    {
        s = dbGetError();
        if (s)
            strcpy(err_msg,s);
        acSendLogout();
        ret = LOGIN_STAGE_START;
    } 
    else
    {
        Strncpyt(game_state.shard_name, auth_info.servers[idx].name);
        if(ret == DBGAMESERVER_SEND_PLAYERS)
        {
            ret = LOGIN_STAGE_NDA;
            s_PostNDALoginStage = LOGIN_STAGE_CHARACTER_SELECT;
        }
        else if(ret == DBGAMESERVER_QUEUE_POSITION)
        {
            ret = LOGIN_STAGE_NDA;
            s_PostNDALoginStage = LOGIN_STAGE_IN_QUEUE;
        }
        else
        {
            ret = LOGIN_STAGE_START;
        }
    }

    if (auth_logged_in)
        authDisconnect();

    return ret;
}

//-------------------------------------------------------------------------------------------------------------
// Automatic connect to dbserver used when doing a shard visitor transfer
//-------------------------------------------------------------------------------------------------------------
int jumpToDbServer()
{
    int ret;
    char shardName[1024];
    char *colonPos;

    // This tech will not play well with local mapserver.  Don't even think about trying.
    if (game_state.local_map_server)
    {
        // Any time anything goes wrong, just punt back to the main login screen.
        g_shardVisitorData[0] = 0;
        g_shardVisitorDBID = 0;
        g_shardVisitorUID = 0;
        g_shardVisitorCookie = 0;
        g_shardVisitorAddress = 0;
        g_shardVisitorPort = 0;
        s_LoginInit = 0;
        return 0;
    }

    // Much of this is cut and paste from loginToDbServer(...) just above
    // cleaning up cache information - new information will be requested by login
    db_info.bonus_slots = 0;
    db_info.shardXferTokens = 0;
    db_info.shardname[0] = 0;

    // also clean up auth data because the DbServer will set the necessary
    // bits for us
    memset(db_info.auth_user_data, 0, AUTH_BYTES);

    AccountInventorySet_Release( &db_info.account_inventory );

    // Need to make sure g_achAccountName is correctly saved, even for players that do not have "save account name" checked
    ret = dbConnect(makeIpStr(g_shardVisitorAddress), g_shardVisitorPort, g_shardVisitorUID, g_shardVisitorCookie, g_achAccountName,
                    game_state.no_version_check, control_state.notimeout?NO_TIMEOUT:90.0f,control_state.notimeout);
    if (!ret || ret == DBGAMESERVER_MSG)
    {
        dbGetError();
        g_shardVisitorData[0] = 0;
        g_shardVisitorDBID = 0;
        g_shardVisitorUID = 0;
        g_shardVisitorCookie = 0;
        g_shardVisitorAddress = 0;
        g_shardVisitorPort = 0;
        s_LoginInit = 0;
        s_loggedIn_serverSelected = LOGIN_STAGE_START;
        return 0;
    } 
    strncpyt(shardName, g_shardVisitorData, sizeof(shardName));
    colonPos = strchr(shardName, ':');
    if (colonPos != NULL)
    {
        *colonPos = 0;
    }
    Strncpyt(game_state.shard_name, shardName);
    return 1;
}

void selectJumpedCharacter()
{
    if (!chooseVisitingPlayerWrapper(g_shardVisitorDBID))
    {
        char *s = dbGetError();

        if (!s || !s[0])
        {
            s = textStd("NoMapResponse");
        }
        dialogStd( DIALOG_OK, s, NULL, NULL, 0, NULL, 0 );
        restartLoginScreen();
    }
    else
    {
        dbDisconnect();  //Character being resumed, no longer need connServer.
        player_being_created = FALSE;
        if(!tryConnect())
        {
            dialogStd( DIALOG_OK, "NoMapserverConn", NULL, NULL, NULL, NULL, 0 );
            restartLoginScreen();
        }
        else
        {
            gfxReload(1);    //Asks mapServer to send a world to load and creates an entity.
            if (commConnected()) //to do, remove from gfx.c
            {
                if(commReqScene(1))
                {
                    start_menu( MENU_ORIGIN );
                    resetCharacterCreation();
                }
            }
        }
    }
}

// Figure the x and y positions of an element.  basis is the absolute position given in coords on a 
// virtual DEFAULT_SCRN_WD x DEFAULT_SCRN_HT display.
static int xPosition(float scaleX, float scaleY, float xIn)
{
    return round(scaleX < scaleY
        ? scaleX * xIn
        : (scaleX * 0.5f * (float) DEFAULT_SCRN_WD + (xIn - (float) DEFAULT_SCRN_WD * 0.5f) * scaleY));
}

static int yPosition(float scaleX, float scaleY, float yIn)
{
    return round(scaleX < scaleY
        ? (scaleY * 0.5f * (float) DEFAULT_SCRN_HT + (yIn - (float) DEFAULT_SCRN_HT * 0.5f) * scaleX)
        : scaleY * yIn);
}

void respondToDbServerLogin(int idx, char *err_msg, char *server_name)
{
    if( s_loggedIn_serverSelected == LOGIN_STAGE_CHARACTER_SELECT || s_loggedIn_serverSelected == LOGIN_STAGE_IN_QUEUE ||
        s_PostNDALoginStage == LOGIN_STAGE_CHARACTER_SELECT || s_PostNDALoginStage == LOGIN_STAGE_IN_QUEUE)
    {
        gSelectedDbServer = idx;
        //s_loggedIn_serverSelected = LOGIN_STAGE_CHARACTER_SELECT;

        // Craft the server name now, while server->name is still valid.
        _snprintf(g_ServerName, SERVER_NAME_SIZE - 1, "%s", textStd(server_name));
        g_ServerName[SERVER_NAME_SIZE - 1] = 0;

        if( gPlayerNumber > db_info.player_count )
            gPlayerNumber = 0;
        if (game_state.skin == UISKIN_PRAETORIANS && !authUserHasProvisionalRogueAccess((U32 *)db_info.auth_user_data) && !AccountHasStoreProduct(&db_info.account_inventory, SKU("ctexgoro")))
        {
            game_state.skin = UISKIN_HEROES;
            resetVillainHeroDependantStuff(1);
        }
    }
    else
    {
        if (!err_msg[0])
            strcpy(err_msg, textStd("NoResponseFromServer"));
        dialogStd( DIALOG_OK, err_msg, NULL, NULL, 0, NULL, 0 );
        restartLoginScreen();
    }
}

static bool drawServerSlot( int idx, float y )
{
    AtlasTex *circle = atlasLoadTexture("MissionPicker_icon_round.tga");

    char svr[256],err_msg[1000];
    CBox box;
    U32 maxUserCount;
    int txtclr = CLR_WHITE, circlr[3] = { 0xffffff22, 0xffffff22, 0xffffff22 };

    ServerInfo * server = &auth_info.servers[idx];
    float screenScaleX, screenScaleY;
    float textScale;
    float UIScale;

    textScale =  screenScaleX = screenScaleY = 1.f;

    // DGNOTE 7/27/2010
    // For now, set this to textScale, i.e. the minumum of ScaleX and ScaleY.  That works as long as we letterbox the splash screen. If we ever get the artwork for a
    // widescreen splash, which we then procedurally select S and T for, this can probably be changed to screenScaleX.  The point about a separate variable is that
    // it's set once, use everywhere, therefore modification becomes relatively trivial.
    UIScale = textScale; 

    // If the auth server tells us 0 is the max, then it's not working
    // properly. Assume 4500 instead.
    if(server->max_user_count==0)
        server->max_user_count=4500;

    maxUserCount = server->max_user_count;

    // if the auth has specified the server type, handle
    switch ( server->server_type )
    {
        case kShardType_Live:
            if( game_state.trainingshard )
                return false;
            break;
        case kShardType_Training:
            if( !game_state.trainingshard )
                return false;
            break;
        case kShardType_None: 
        default:
            break; // ignore
    };
    
    if(server->name[0])
        sprintf( svr, "%s", server->name );
    else
        strcpy( svr, "UNNAMED");

    if( server->server_status && ( !maxUserCount || server->curr_user_count < server->max_user_count || game_state.cryptic ) )
    {
        float ratio = maxUserCount?((float)server->curr_user_count/maxUserCount):0;
        int canClick = 1;
        // height is explicitly fixed size, doesn't scale, this keeps the list looking clean at all scales
        BuildCBox( &box, xPosition(screenScaleX, screenScaleY, SERVER_X), y, SERVER_WD * UIScale, SERVER_LIST_ELEMENT_Y - 1);
        //drawBox(&box, 21, 0x00ff00ff, 0);

        if (server->isVIP && (!AccountIsVIPByPayStat(auth_info.pay_stat) && !AccountIsPremiumByLoyalty(auth_info.loyalty)))
        {
            txtclr = CLR_GREY;
            circlr[0] = circlr[1] = circlr[2] = CLR_GREY;
            setToolTip( &gServerTips[idx], &box, "VIPShard", 0, MENU_LOGIN, 0 );
            canClick = 0;
        }
        else if ( maxUserCount <= server->curr_user_count )
        {
            if(maxUserCount!=server->max_user_count)
            {
                txtclr = CLR_GREYED_RED;
                circlr[0] = circlr[1] = circlr[2] = CLR_GREYED_RED;
                setToolTip( &gServerTips[idx], &box, "Queueing", 0, MENU_LOGIN, 0 );
            }
            else
            {
                txtclr = CLR_GREY;
                circlr[0] = circlr[1] = circlr[2] = CLR_GREY;
                setToolTip( &gServerTips[idx], &box, "Full", 0, MENU_LOGIN, 0 );
            }
        }
        else if( server->curr_user_count < 250 )
        {
            circlr[0] = 0xb4ff00ff;
            setToolTip( &gServerTips[idx], &box, "LightLoad", 0, MENU_LOGIN, 0 );
        }
        else if ( server->curr_user_count < 1100 )
        {
            circlr[0] = circlr[1] = CLR_YELLOW;
            setToolTip( &gServerTips[idx], &box, "MediumLoad", 0, MENU_LOGIN, 0 );
        }
        else
        {
            circlr[0] = circlr[1] = circlr[2] = CLR_RED;
            setToolTip( &gServerTips[idx], &box, "HeavyLoad", 0, MENU_LOGIN, 0 );
        }
        
        if( mouseCollision( &box ) || (game_state.editnpc && idx==0) )
        {
            if( isDown( MS_LEFT ) )
            {
                // height is explicitly fixed size, doesn't scale, this keeps the list looking clean at all scales
                drawFlatFrame( PIX3, R10, xPosition(screenScaleX, screenScaleY, SERVER_X + 20), y, 15, (SERVER_WD - 40) * UIScale, SERVER_LIST_ELEMENT_Y - 1, 1.f, 0x2797d8ff, 0x2797d8ff );
                txtclr = CLR_GREEN;
            }
            else
            {
                // height is explicitly fixed size, doesn't scale, this keeps the list looking clean at all scales
                drawFlatFrame( PIX3, R10, xPosition(screenScaleX, screenScaleY, SERVER_X + 20), y, 15, (SERVER_WD - 40) * UIScale, SERVER_LIST_ELEMENT_Y - 1, 1.f, 0x5e7d9bff, 0x5e7d9bff );
            }

            if( canClick && (mouseClickHit( &box, MS_LEFT) || (game_state.editnpc && idx==0) ) )
            {
                sndPlay("N_Select", SOUND_GAME);
                s_loggedIn_serverSelected = loginToDbServer(idx,err_msg);
                respondToDbServerLogin(idx,err_msg,server->name);
            }
        }
        else
        {
            // height is explicitly fixed size, doesn't scale, this keeps the list looking clean at all scales
            drawFlatFrame( PIX3, R10, xPosition(screenScaleX, screenScaleY, SERVER_X + 20), y, 15, (SERVER_WD - 40) * UIScale, SERVER_LIST_ELEMENT_Y - 1, 1.f, 0x7d95acff, 0x7d95acff );
        }
    }
    else
    {
        BuildCBox( &box, SERVER_X, y, SERVER_WD, 30 );

        if( server->server_status )
            setToolTip( &gServerTips[idx], &box, "Full", 0, MENU_LOGIN, 0 );
        else
            setToolTip( &gServerTips[idx], &box, "ServerDown", 0, MENU_LOGIN, 0 );

        txtclr = CLR_GREY;
        circlr[0] = circlr[1] = circlr[2] = CLR_GREY;
    }

    font(&hybridbold_12);
    font_color( txtclr, txtclr );
    prnt( xPosition(screenScaleX, screenScaleY, 760), y + 20 * UIScale, 20, textScale, textScale, svr );

    display_sprite( circle, xPosition(screenScaleX, screenScaleY, 735), y + 9 * UIScale, 21, .4f * textScale, .4f * textScale, circlr[0] );
    display_sprite( circle, xPosition(screenScaleX, screenScaleY, 720), y + 9 * UIScale, 21, .4f * textScale, .4f * textScale, circlr[1] );
    display_sprite( circle, xPosition(screenScaleX, screenScaleY, 705), y + 9 * UIScale, 21, .4f * textScale, .4f * textScale, circlr[2] );
    return true;
}

static int compareServerInfo(const void* item1, const void* item2)
{
    const ServerInfo* s1 = (ServerInfo*)item1;
    const ServerInfo* s2 = (ServerInfo*)item2;

    float ratio1 = (float)s1->curr_user_count/s1->max_user_count;
    float ratio2 = (float)s2->curr_user_count/s2->max_user_count;

    if( !s1->server_status && !s2->server_status )
        return stricmp(s1->name, s2->name);
    if( !s1->server_status )
        return 1;
    if( !s2->server_status )
        return -1;

    if( !s1->max_user_count || !s2->max_user_count )
    {
        if( s1->max_user_count == s2->max_user_count )
            return stricmp(s1->name, s2->name);
        else
            return s1->curr_user_count-s2->curr_user_count;
    }

    if(ratio1 != ratio2)
    {
        if( ratio1 > ratio2 )
            return 1;
        else
            return -1;
    }

    return stricmp(s1->name, s2->name);
}

static void serverSlots()
{
    int i;
    static F32 offset = 0.0f;
    CBox box;
    float screenScaleX, screenScaleY;
    float textScale;
    float UIScale;
    float currY;

    textScale = screenScaleX = screenScaleY = 1.f;

    // DGNOTE 7/27/2010
    // For now, set this to textScale, i.e. the minumum of ScaleX and ScaleY.  That works as long as we letterbox the splash screen. If we ever get the artwork for a
    // widescreen splash, which we then procedurally select S and T for, this can probably be changed to screenScaleX.  The point about a separate variable is that
    // it's set once, use everywhere, therefore modification becomes relatively trivial.
    UIScale = textScale;

    qsort( auth_info.servers, auth_info.server_count, sizeof(ServerInfo), compareServerInfo );

    BuildCBox(&box, xPosition(screenScaleX, screenScaleY, SERVER_X),
        yPosition(screenScaleX, screenScaleY, SERVER_LIST_Y), SERVER_WD * UIScale,
        SERVER_LIST_HT * UIScale);
    drawHybridScrollBar(xPosition(screenScaleX, screenScaleY, SERVER_X + SERVER_WD)/UIScale - 25.0f, 
        yPosition(screenScaleX, screenScaleY, SERVER_LIST_Y)/UIScale + 10.0f, 100, 
        SERVER_LIST_HT - 20.0f, &offset, 
        ((auth_info.server_count)*SERVER_LIST_ELEMENT_Y) - SERVER_LIST_HT, &box);
    set_scissor(1);
    scissor_dims(xPosition(screenScaleX, screenScaleY, SERVER_X), 
        yPosition(screenScaleX, screenScaleY, SERVER_LIST_Y), SERVER_WD * UIScale,
        SERVER_LIST_HT * UIScale);

    // NOTE:  I do the yPosition translation once and then add a static amount
    // This ensures that the items in the list are always evenly spaced
    // scale the value of SERVER_LIST_ELEMENT_Y to a rounded integer if you want to scale the size
    currY = yPosition(screenScaleX, screenScaleY, SERVER_LIST_Y - offset);

    // first display last server
    if( auth_info.last_login_server_id )
    {
        for( i = 0; i < auth_info.server_count; i++ )
        {
            if( auth_info.servers[i].id == auth_info.last_login_server_id )
            {
                if (drawServerSlot(i, currY))
                {
                    currY += SERVER_LIST_ELEMENT_Y;
                }
            }
        }
    }

    for( i = 0; i < auth_info.server_count; i++ )
    {
        if (auth_info.servers[i].id == auth_info.last_login_server_id)
        {
            continue;
        }
        if (drawServerSlot(i, currY))
        {
            currY += SERVER_LIST_ELEMENT_Y;
        }
    }
    set_scissor(0);
}

static void serverFrame()
{
    static float scale = 0.f;
    static SMFBlock *s_serverSelectLabelSMF = 0;
    float screenScaleX, screenScaleY;
    float textScale;
    float UIScale;
    static HybridElement sButtonBack = {0, "BackToLogInString"};

    if (!s_serverSelectLabelSMF)
    {
        s_serverSelectLabelSMF = smfBlock_Create();
        smf_SetFlags(s_serverSelectLabelSMF, SMFEditMode_Unselectable, SMFLineBreakMode_SingleLine, 
            SMFInputMode_AnyTextNoTagsOrCodes, 0, SMFScrollMode_ExternalOnly,
            SMFOutputMode_StripAllTagsAndCodes, SMFDisplayMode_AllCharacters, SMFContextMenuMode_None,
            SMAlignment_Center, 0, 0, 0);
        smf_SetRawText(s_serverSelectLabelSMF, textStd("ChooseServer"), false);
    }

    textScale = screenScaleX = screenScaleY = 1.f;

    // DGNOTE 7/27/2010
    // For now, set this to textScale, i.e. the minumum of ScaleX and ScaleY.  That works as long as we letterbox the splash screen. If we ever get the artwork for a
    // widescreen splash, which we then procedurally select S and T for, this can probably be changed to screenScaleX.  The point about a separate variable is that
    // it's set once, use everywhere, therefore modification becomes relatively trivial.
    UIScale = textScale;

    if( s_loggedIn_serverSelected == LOGIN_STAGE_SERVER_SELECT )
    {
        scale += TIMESTEP*.1f;
        if( scale > 1.0f )
            scale = 1.0f;
    }
    else
    {
        scale -= TIMESTEP*.1f;
        if( scale < 0.f )
        {
            scale = 0.0f;
            return;
        }
    }

    if( scale < 1.f )
        return;

    gTextAttr_WhiteTitle24Italics.piColor = (int *)LOGIN_FOREGROUND_TEXT_COLOR;  //set custom color depending on current artist
    smf_Display(s_serverSelectLabelSMF, 
                xPosition(screenScaleX, screenScaleY, SERVER_X),
                yPosition(screenScaleX, screenScaleY, (SERVER_Y + 140)), 20,
                SERVER_WD, 0, 0, 0, &gTextAttr_WhiteTitle24Italics, 0);
    gTextAttr_WhiteTitle24Italics.piColor = (int *)0xffffffff;  //revert color

    serverSlots();

    if (D_MOUSEHIT == drawHybridBarButton(&sButtonBack, xPosition(screenScaleX, screenScaleY, SERVER_X + SERVER_WD / 2), 
        yPosition(screenScaleX, screenScaleY, (SERVER_Y + SERVER_HT - 30)), 20.0f, 
        170 * UIScale, UIScale*0.7f, 0x9c2206ff,
        HYBRID_BAR_BUTTON_NO_OUTLINE))
    {
        sndPlay("N_Deselect", SOUND_GAME);
        handleBackToLogin();
    }
}

// Common placements
#define EULA_X            625
#define EULA_Y            250
#define EULA_WD            375
#define EULA_HT            488
#define EULA_INT_HT        (EULA_HT - 60)
#define EULA_BUTTON_WD    140
#define EULA_BUTTON_HT    20
#define EULA_BUTTON_Y    (EULA_Y + EULA_INT_HT + 35)

//-----------------------------------------------------------------------------------------------------------
// NDA screen
//-----------------------------------------------------------------------------------------------------------

static U32 getNDAVersion()
{
    const char *appName = regGetAppName();
    const char *buildVersion = getExecutablePatchVersion(appName);

    if (buildVersion)
    {
        // Need the issue.  Use the branch number (first number in the buildVersion string).
        return atoi(buildVersion);
    }

    return 0;
}

static bool ShouldShowNDA()
{
#ifdef ENABLE_NDA
    const char *appName = regGetAppName();

    if (game_state.skipEULANDA)
        return false;

    if (game_state.forceNDA)
        return true;

    // Only show the NDA for US test and US Beta (not EU Test (euCohTest) or EU Beta (euCohBeta))
    //if (strstriConst(appName, "CohTest") || strstriConst(appName, "CohBeta"))
    if ((stricmp(appName, "CohTest") == 0) || (stricmp(appName, "CohBeta") == 0))
    {
        U32 currentNDAVersion = getNDAVersion();
        U32 signedNDAVersion = 0xFFFFFFFF;

        if (db_info.account_inventory.invArr != NULL)
        {
            SkuId sku_id;
            AccountInventory* pInv;

            if (stricmp(appName, "CohTest") == 0)
            {
                sku_id = SKU("nda_test");
            }
            else
            {
                devassert(stricmp(appName, "CohBeta") == 0);
                sku_id = SKU("nda_beta");
            }

            pInv = AccountInventorySet_Find(&db_info.account_inventory, sku_id);
            if ( pInv )
            {
                signedNDAVersion = pInv->granted_total;
            }
        }

        // check if the player has already signed the latest version of the NDA
        if (signedNDAVersion != currentNDAVersion)
            return true;
    }

    return false;
#else
    return false;
#endif
}

static void LogNDAAcceptance()
{
    const char *appName = regGetAppName();
    U32 NDAVersion = getNDAVersion();

    if (stricmp(appName, "CohTest") == 0)
    {
        dbSignNDA(NDAVersion, 0);
    }
    else
    {
        devassert(stricmp(appName, "CohBeta") == 0);
        dbSignNDA(NDAVersion, 1);
    }
}

// Used for both EULA's and NDA's.
void handleRejectOk(void *unused)
{
    windowExit(0);
}

// Used for both EULA's and NDA's.
int handleURLs(char *url)
{
    ShellExecuteA(NULL,NULL,url,NULL,NULL,0);
    return 1;
}

static void ndaFrame()
{
    static float scale = 0.f;
    static F32 offset = 0.0f;
    static F32 textHeight;
    float screenScaleX, screenScaleY;
    float textScale;
    float UIScale;
    int disagreeButtonX, agreeButtonX, backButtonX;
    static SMFBlock *s_ndaSMF = NULL;
    float oldScale = scale;
    int backButtonColor = SELECT_FROM_UISKIN( CLR_BTN_HERO, CLR_BTN_VILLAIN, CLR_BTN_ROGUE );
    char *NDAAgree;
    char *NDAReject;
    char *NDABack;
    char *NDARejectDialog;
    char *NDAText;
    CBox box;
    static HybridElement sButtonAgree;
    static HybridElement sButtonReject;
    static HybridElement sButtonBack;

    textScale = screenScaleX = screenScaleY = 1.f;

    // DGNOTE 7/27/2010
    // For now, set this to textScale, i.e. the minumum of ScaleX and ScaleY.  That works as long as we letterbox the splash screen. If we ever get the artwork for a
    // widescreen splash, which we then procedurally select S and T for, this can probably be changed to screenScaleX.  The point about a separate variable is that
    // it's set once, use everywhere, therefore modification becomes relatively trivial.
    UIScale = textScale;

    if( s_loggedIn_serverSelected == LOGIN_STAGE_NDA )
    {
        // See if we should skip the NDA screen
        if (!ShouldShowNDA())
        {
            s_loggedIn_serverSelected = s_PostNDALoginStage;
            // Make sure the "Retrieving character list" message is shown after the NDA dialog
            s_retrievingCharacterList_startMS = timerMsecsSince2000();
            return;
        }

        scale += TIMESTEP*.1f;
        if( scale > 1.0f )
            scale = 1.0f;
    }
    else
    {
        scale -= TIMESTEP*.1f;
        if( scale < 0.f )
        {
            scale = 0.0f;
            return;
        }
    }

    if( scale < 1.f )
        return;

    if (locIsBritish(getCurrentLocale()))
    {
        NDAAgree        = "NDAAgree-UK";
        NDAReject        = "NDAReject-UK";
        NDABack            = "NDABack-UK";
        NDARejectDialog    = "NDARejectDialog-UK";
        NDAText            = "NDAText-UK";
    }
    else
    {
        NDAAgree        = "NDAAgree";
        NDAReject        = "NDAReject";
        NDABack            = "NDABack";
        NDARejectDialog    = "NDARejectDialog";
        NDAText            = "NDAText";
    }
    sButtonAgree.text = NDAAgree;
    sButtonReject.text = NDAReject;
    sButtonBack.text = NDABack;

    if(s_ndaSMF==NULL)
    {
        s_ndaSMF = smfBlock_Create();
        smf_SetFlags(s_ndaSMF, SMFEditMode_ReadOnly, SMFLineBreakMode_MultiLineBreakOnWhitespace, 
            SMFInputMode_AnyTextNoTagsOrCodes, 0, SMFScrollMode_ExternalOnly,
            SMFOutputMode_StripAllTagsAndCodes, SMFDisplayMode_AllCharacters, SMFContextMenuMode_None,
            SMAlignment_Left, 0, 0, 0);
        smf_SetRawText(s_ndaSMF, textStd(NDAText), 0);
        smf_SetScissorsBox(s_ndaSMF, EULA_X, EULA_Y, EULA_WD, EULA_INT_HT);
    }

    // If the game is running slow, then it might draw the NDA buttons and Accept on the same frame
    // as the EULA.  Do this to avoid this problem.
    if (oldScale == 0.0f)
        return;

    gTextAttr_WhiteNoOutline12.piColor = (int *)LOGIN_FOREGROUND_TEXT_COLOR;  //set custom color depending on current artist
    textHeight = smf_Display(s_ndaSMF, EULA_X, EULA_Y - offset, 20.0f, EULA_WD, 0, 0, 0, &gTextAttr_WhiteNoOutline12, handleURLs);
    gTextAttr_WhiteNoOutline12.piColor = (int *)0xffffffff;  //revert color

    BuildCBox(&box, EULA_X, EULA_Y, EULA_WD, EULA_INT_HT);
    //drawBox(&box, 20, 0xff0000ff, 0);
    drawHybridScrollBar(EULA_X + EULA_WD - 10.0f, EULA_Y + 10.0f, 20.0f, EULA_INT_HT - 20.0f, &offset, textHeight - EULA_INT_HT, &box);

    agreeButtonX = xPosition(screenScaleX, screenScaleY, EULA_X + EULA_WD - EULA_BUTTON_WD*0.7f / 2);
    if (D_MOUSEHIT == drawHybridBarButton(&sButtonAgree, agreeButtonX, 
        yPosition(screenScaleX, screenScaleY, EULA_BUTTON_Y), 20.0f, 
        EULA_BUTTON_WD * UIScale, UIScale*0.7f, 0x648d0cff,
        HYBRID_BAR_BUTTON_NO_OUTLINE))
    {
        dialogClearQueue( 0 );

        smfBlock_Destroy(s_ndaSMF);
        s_ndaSMF = NULL;

        s_loggedIn_serverSelected = s_PostNDALoginStage;

        // log acceptance time and version
        LogNDAAcceptance();
    }

    disagreeButtonX = xPosition(screenScaleX, screenScaleY, EULA_X + EULA_BUTTON_WD*0.7f / 2);
    if (D_MOUSEHIT == drawHybridBarButton(&sButtonReject, disagreeButtonX, 
        yPosition(screenScaleX, screenScaleY, EULA_BUTTON_Y), 20.0f, 
        EULA_BUTTON_WD * UIScale, UIScale*0.7f, 0x9c2206ff,
        HYBRID_BAR_BUTTON_NO_OUTLINE))
    {
        dbSendQuit();

        smfBlock_Destroy(s_ndaSMF);
        s_ndaSMF = NULL;

        s_loggedIn_serverSelected = LOGIN_STAGE_REJECT_EULA_NDA;
        dialogStd( DIALOG_OK, NDARejectDialog, NULL, NULL, handleRejectOk, NULL, 0 );
        sndPlay("N_Error", SOUND_GAME);
    }

    backButtonX = xPosition(screenScaleX, screenScaleY, EULA_X + EULA_WD / 2);
    if (D_MOUSEHIT == drawHybridBarButton(&sButtonBack, backButtonX, 
        yPosition(screenScaleX, screenScaleY, EULA_BUTTON_Y), 20.0f, 
        EULA_BUTTON_WD * UIScale, UIScale*0.7f, 0x2797d8,
        HYBRID_BAR_BUTTON_NO_OUTLINE))
    {
        s_returnToServerSelect(NULL);
    }
}
//-----------------------------------------------------------------------------------------------------------
// EULA screen
//-----------------------------------------------------------------------------------------------------------
static int s_LastAccountChecksum[4] = {0,0,0,0};

static void UpdateLastEULAAccept()
{
    int accountNameLen = strlen(g_achAccountName);

    cryptMD5Update((U8*)g_achAccountName, accountNameLen);
    cryptMD5Final(s_LastAccountChecksum);
}

static bool ShouldShowEULA(const char *accountName)
{
    if (game_state.skipEULANDA && !isProductionMode())
    {
        return false;
    }

    // See if the account name matches the last account that tried to log in
    // during this gaming session.  If you restart the game, then you will
    // need to re-accept the EULA
    if (accountName)
    {
        int accountNameLen = strlen(accountName);
        U32 checksum[4];

        cryptMD5Update((U8*)accountName, accountNameLen);
        cryptMD5Final(checksum);

        if (checksum[0] == s_LastAccountChecksum[0] && 
            checksum[1] == s_LastAccountChecksum[1] && 
            checksum[2] == s_LastAccountChecksum[2] && 
            checksum[3] == s_LastAccountChecksum[3])
        {
            return false;
        }
    }

    return true;
}

static void eulaFrame()
{
    static float scale = 0.f;
    static F32 offset = 0.0f;
    static F32 textHeight;
    float screenScaleX, screenScaleY;
    float textScale;
    float UIScale;
    int disagreeButtonX, agreeButtonX, backButtonX;
    static SMFBlock *s_eulaSMF = NULL;
    int backButtonColor = SELECT_FROM_UISKIN( CLR_BTN_HERO, CLR_BTN_VILLAIN, CLR_BTN_ROGUE );
    char *EULAAgree;
    char *EULAReject;
    char *EULABack;
    char *EULARejectDialog;
    char *EULAText;
    CBox box;
    static HybridElement sButtonAgree;
    static HybridElement sButtonReject;
    static HybridElement sButtonBack;

    textScale = screenScaleX =  screenScaleY = 1.f;

    // DGNOTE 7/27/2010
    // For now, set this to textScale, i.e. the minumum of ScaleX and ScaleY.  That works as long as we letterbox the splash screen. If we ever get the artwork for a
    // widescreen splash, which we then procedurally select S and T for, this can probably be changed to screenScaleX.  The point about a separate variable is that
    // it's set once, use everywhere, therefore modification becomes relatively trivial.
    UIScale = textScale;

    if( s_loggedIn_serverSelected == LOGIN_STAGE_EULA )
    {
        // See if we should skip the EULA screen
        if (!ShouldShowEULA(s_editAccount->outputString))
        {
            s_loggedIn_serverSelected = LOGIN_STAGE_SERVER_SELECT;
            return;
        }
        
        scale += TIMESTEP*.1f;
        if( scale > 1.0f )
            scale = 1.0f;
    }
    else
    {
        scale -= TIMESTEP*.1f;
        if( scale < 0.f )
        {
            scale = 0.0f;
            return;
        }
    }

    if( scale < 1.f )
        return;

    if (locIsBritish(getCurrentLocale()))
    {
        EULAAgree            = "EulaAgree-UK";
        EULAReject            = "EulaReject-UK";
        EULABack            = "EulaBack-UK";
        EULARejectDialog    = "EulaRejectDialog-UK";
        EULAText            = "EulaText-UK";
    }
    else
    {
        EULAAgree            = "EulaAgree";
        EULAReject            = "EulaReject";
        EULABack            = "EulaBack";
        EULARejectDialog    = "EulaRejectDialog";
        EULAText            = "EulaText";
    }
    sButtonAgree.text = EULAAgree;
    sButtonReject.text = EULAReject;
    sButtonBack.text = EULABack;

    if(s_eulaSMF==NULL)
    {
        s_eulaSMF = smfBlock_Create();
        smf_SetFlags(s_eulaSMF, SMFEditMode_ReadOnly, SMFLineBreakMode_MultiLineBreakOnWhitespace, 
            SMFInputMode_AnyTextNoTagsOrCodes, 0, SMFScrollMode_ExternalOnly,
            SMFOutputMode_StripAllTagsAndCodes, SMFDisplayMode_AllCharacters, SMFContextMenuMode_None,
            SMAlignment_Left, 0, 0, 0);
        smf_SetRawText(s_eulaSMF, textStd(EULAText), 0);
        smf_SetScissorsBox(s_eulaSMF, EULA_X, EULA_Y, EULA_WD, EULA_INT_HT);
    }

    gTextAttr_WhiteNoOutline12.piColor = (int *)LOGIN_FOREGROUND_TEXT_COLOR;  //set custom color depending on current artist
    textHeight = smf_Display(s_eulaSMF, EULA_X, EULA_Y - offset, 20.0f, EULA_WD, 0, 0, 0, &gTextAttr_WhiteNoOutline12, handleURLs);
    gTextAttr_WhiteNoOutline12.piColor = (int *)0xffffffff;  //revert color

    BuildCBox(&box, EULA_X, EULA_Y, EULA_WD, EULA_INT_HT);
    //drawBox(&box, 20, 0xff0000ff, 0);
    drawHybridScrollBar(EULA_X + EULA_WD - 10.0f, EULA_Y + 10.0f, 20.0f, EULA_INT_HT - 20.0f, &offset, textHeight - EULA_INT_HT, &box);
    
    agreeButtonX = xPosition(screenScaleX, screenScaleY, EULA_X + EULA_WD - EULA_BUTTON_WD*0.7f / 2);
    if (D_MOUSEHIT == drawHybridBarButton(&sButtonAgree, agreeButtonX, 
                        yPosition(screenScaleX, screenScaleY, EULA_BUTTON_Y), 20.0f, 
                        EULA_BUTTON_WD * UIScale, UIScale*0.7f, 0x648d0cff,
                        HYBRID_BAR_BUTTON_NO_OUTLINE))
    {
        dialogClearQueue( 0 );

        s_loggedIn_serverSelected = LOGIN_STAGE_SERVER_SELECT;

        smfBlock_Destroy(s_eulaSMF);
        s_eulaSMF = NULL;

        UpdateLastEULAAccept();
    }

    disagreeButtonX = xPosition(screenScaleX, screenScaleY, EULA_X + EULA_BUTTON_WD*0.7f / 2);
    if (D_MOUSEHIT == drawHybridBarButton(&sButtonReject, disagreeButtonX, 
                        yPosition(screenScaleX, screenScaleY, EULA_BUTTON_Y), 20.0f, 
                        EULA_BUTTON_WD * UIScale, UIScale*0.7f, 0x9c2206ff,
                        HYBRID_BAR_BUTTON_NO_OUTLINE))
    {
        dbSendQuit();

        smfBlock_Destroy(s_eulaSMF);
        s_eulaSMF = NULL;

        s_loggedIn_serverSelected = LOGIN_STAGE_REJECT_EULA_NDA;
        dialogStd( DIALOG_OK, EULARejectDialog, NULL, NULL, handleRejectOk, NULL, 0 );
        sndPlay("N_Error", SOUND_GAME);
    }

    backButtonX = xPosition(screenScaleX, screenScaleY, EULA_X + EULA_WD / 2);
    if (D_MOUSEHIT == drawHybridBarButton(&sButtonBack, backButtonX, 
                        yPosition(screenScaleX, screenScaleY, EULA_BUTTON_Y), 20.0f, 
                        EULA_BUTTON_WD * UIScale, UIScale*0.7f, 0x2797d8,
                        HYBRID_BAR_BUTTON_NO_OUTLINE))
    {
        handleBackToLogin();
    }
}

//-----------------------------------------------------------------------------------------------------------
// Log In
//-----------------------------------------------------------------------------------------------------------

#define LOGIN_X            642
#define LOGIN_Y            240
#define LOGIN_WD        342
#define LOGIN_HT        278
#define LOGIN_BUTTON_Y    (LOGIN_Y + 245)
#define LOGIN_BUTTON_WD    165
#define WEB_X            (LOGIN_X)
#define WEB_Y            (LOGIN_Y + LOGIN_HT + 20)
#define WEB_WD            (LOGIN_WD)
#define WEB_HT            105
#define WEB_BUTTON_WD    (LOGIN_WD)
#define SETTINGS_BUTTON_WD    (LOGIN_WD - 140)

static void debug_putCharacter(char *fpath, char *auth)
{
    char *tmpfname;
    static char *tmp = NULL;
    char *fileauth;
    char *filetext;
    FILE *fp;

    if(!game_state.cs_address[0])
    {
        dialogStd(DIALOG_OK,"no dbserver address specified to connect to.", NULL, NULL, NULL, NULL, 0 );
        return;
    }
    
    filetext = fileAlloc(fpath,NULL);
    if(!filetext)
    {
        estrPrintf(&tmp,"Failed to open file %s",fpath);
        dialogStd( DIALOG_OK, tmp, NULL, NULL, NULL, NULL, 0 );
        return;
    }
    
    fileauth = strstr(filetext,"AuthName");
    if(fileauth)
        fileauth = strstr(fileauth,"\"");
    if(!fileauth)
    {
        estrPrintf(&tmp,"Get failed. %s, file=%s",filetext,fpath);
        dialogStd( DIALOG_OK, tmp, NULL, NULL, NULL, NULL, 0 );
        goto done;
    }

    *fileauth++ = '%';
    *fileauth++ = 's';
    while(*fileauth != '\r' && *fileauth != '\n')
        *fileauth++ = ' ';
    
    estrPrintf(&tmp,filetext,auth);
    tmpfname = fileTempName();
    fp = fopen(tmpfname,"w");
    fprintf(fp,"%s",tmp);
    fclose(fp);
    {
        char path[MAX_PATH]="";
        getcwd(path, ARRAY_SIZE(path)-2);
        estrPrintf(&tmp,"c:/game%s/tools/util/dbquery.exe -db %s -putcharacter %s",
            strstri(path, "fix") ? "fix" : strstri(path, "future") ? "future" : "",
            game_state.cs_address, tmpfname);
    }
    if(systemf(tmp)!=0)
    {
        estrPrintf(&tmp,"Failed to execute put %s", tmp);
        dialogStd( DIALOG_OK, tmp, NULL, NULL, NULL, NULL, 0 );
        goto done;
    }
    else
    {
        estrPrintf(&tmp,"%s added to %s under auth %s",getFileName(fpath),game_state.cs_address, auth );
        dialogStd(DIALOG_OK, tmp, NULL,NULL,NULL,NULL,0);
    }
done:
    free(filetext);
}


static void debug_DrawGetCharacter(void)
{
    static BOOL g_getting = FALSE;
    static ComboBox shards = {0};
    static char **shardnames = NULL; // combobox needs earray.
    static char *fpath = NULL;
    static SMFBlock *g_charname = NULL;
    static char *button_msg = NULL;
    static char *cache_dir = NULL;
    int color = CLR_PARAGON;
    F32 dy = 0.f;
    F32 ddy = 30;
    F32 y = -ddy + 100.0f;
    const char *shardname=NULL;
    int button_flags = 0;
    int i;
    BOOL gotchar = FALSE;
    static bool show_menu = false;
    int char_entry_ypos;

    // ----------
    // do the UI

#define NEXT_Y (y+(dy+=ddy))
#define CHARFETCH_REMOTE_SCRIPT "c:/game/tools/util/charfetch_remote.bat"

    if(!fileExists(CHARFETCH_REMOTE_SCRIPT) || !fileExists("c:/game/tools/util/dbquery.exe"))
        return;

    if(!show_menu)
    {
        if( D_MOUSEHIT == drawStdButtonTopLeft( 10, NEXT_Y, 20.0f, 180.f, 30.0f, CLR_YELLOW, "Enable CharFetch Menu", 1.0f, button_flags ))
            show_menu = true;
        return;
    }
    else
    {
        if( D_MOUSEHIT == drawStdButtonTopLeft( 10, NEXT_Y, 20.0f, 180.f, 30.0f, CLR_YELLOW, "Disable CharFetch Menu", 1.0f, button_flags ))
            show_menu = false;
    }

    if(!cache_dir)
    {
        char *tmp = getenv("USERNAME");
        estrPrintf(&cache_dir,"n:/nobackup/CharacterCache/%s/",tmp?tmp:"default");
        mkdirtree(cache_dir);
    }

    if(!g_charname)
    {
        char *src_shardnames = fileAlloc("n:/nobackup/CharacterCache/src_shards.txt",NULL);
        char *shard;
        dy += 5;
        if(src_shardnames)
        {
            FOR_STRTOK(shard,"\r\n",src_shardnames)
                eaPush(&shardnames,shard);
        }
        else
            eaPush(&shardnames,"CharacterCache/src_shards.txt not found");

        combobox_init(&shards, 10, NEXT_Y, 20.f, 300.f, 30.f, 500.f, shardnames,0);
        g_charname = smfBlock_Create();
        smf_SetFlags(g_charname, SMFEditMode_ReadWrite, SMFLineBreakMode_SingleLine,
            SMFInputMode_AnyTextNoTagsOrCodes, MAX_PLAYER_NAME_LEN(NULL), SMFScrollMode_ExternalOnly,
            SMFOutputMode_StripAllTagsAndCodes, SMFDisplayMode_AllCharacters, SMFContextMenuMode_None,
            SMAlignment_Left, 0, 0, 0);
        smf_SetRawText(g_charname, "character name", false);
    }
    else
        NEXT_Y;
    dy += 13;

    char_entry_ypos = NEXT_Y;
    smf_SetScissorsBox(g_charname, 20, char_entry_ypos, WEB_WD-30.f, 30.f);
    smf_Display(g_charname, 20, char_entry_ypos, 20, WEB_WD-30.f, 30.f, 0, 0, &gTextAttr_LightHybridBlueHybrid12, 0);

    combobox_display(&shards);
    shardname = eaGetConst(&shards.strings,shards.currChoice);
    if(!shardname || !strlen(s_editAccount->outputString) || !strlen(g_charname->outputString) || !strcmp("character name", g_charname->outputString))
        button_flags = BUTTON_LOCKED;
    
    if(!button_flags)
        estrPrintf(&button_msg,"Fetch '%s' from '%s' and Put to %s as %s's", g_charname->outputString, shardname, game_state.cs_address, s_editAccount->outputString);
    else
        estrPrintCharString(&button_msg,"Fetch Character");

    dy -= 3;
    if( D_MOUSEHIT == drawStdButtonTopLeft( 10, NEXT_Y, 20.0f, 500.f, 30.0f, color, button_msg, 1.0f, button_flags ))
    {
        static char *fname = NULL;
        static char *tmp = NULL;
        char *charname = g_charname->outputString;
        if(charname)
        {
            estrPrintf(&fname,"%s_%s.char",shardname,charname);
            strToFileName(fname);
            estrPrintf(&fpath,"%s%s",cache_dir,fname);
            estrPrintf(&tmp, CHARFETCH_REMOTE_SCRIPT " %s %s %s",shardname,fpath,charname);
            if(0!=system(tmp))
                dialogStd( DIALOG_OK, "Failed to Get Character.", NULL, NULL, NULL, NULL, 0 );
            else 
                gotchar = TRUE;
        }
        if(gotchar)
        {
            debug_putCharacter(fpath, s_editAccount->outputString);
        }
    }
    
    // ----------
    // put character
    {
        static ComboBox chars = {0}; 
        static int n = 0;
        static char **files = NULL;
        static const char **charfnames = NULL;
        const char *fname = NULL;

        if(gotchar)
        {
            comboboxClear(&chars);
            if(files)
                fileScanDirFreeNames(files,n);
            eaDestroyConst(&charfnames);
        }

        if(!charfnames)
        {
            files = fileScanDir(cache_dir,&n);
            for( i = 0; i < n; ++i ) 
            {
                const char *f = getFileName(files[i]);
                if(strEndsWith(f,".char"))
                    eaPushConst(&charfnames,f);
            }
            if(charfnames)
                combobox_init(&chars,10,NEXT_Y,20.f,WEB_WD - 30.f, 30.f, 500.f, charfnames, 0);        
        }
        else
            NEXT_Y;

        if(charfnames)
        {
            combobox_display(&chars);
            fname = eaGetConst(&chars.strings,chars.currChoice);

            if(!strlen(s_editAccount->outputString) || !fname)
                button_flags = BUTTON_LOCKED;
            else
                button_flags = 0;

            if( D_MOUSEHIT == drawStdButtonTopLeft( 10, NEXT_Y, 20.0f, WEB_WD - 30.0f, 30.0f, color, "Put Existing Character", 1.0f, button_flags ))
            {
                ComboBox chars = {0}; 
                int n;
                char **files = fileScanDir(cache_dir,&n);
                char **charfnames = NULL;
                estrPrintf(&fpath,"%s/%s",cache_dir,fname);
                debug_putCharacter(fpath, s_editAccount->outputString);
            }
        }
    }
}


int loginToAuthServer(int retry)
{
    char password[MAX_PASSWORD_LEN];
    assert(sizeof(password) == sizeof(g_achPassword));
    password[0] = 0;
    cryptRetrieve(password, g_achPassword, sizeof(password));
    if(!g_achAccountName[0] || !password[0] )
    {
        dialogStd( DIALOG_OK, "AccountInfoRequired", NULL, NULL, NULL, NULL, 0 );
        sndPlay("N_Error", SOUND_GAME);
        memset(password, 0, sizeof(password));
        return FALSE;
    }

    if (!authLogin(g_achAccountName,password))
    {
        if (authIsErrorAccountAlreadyLoggedIn() && retry)
        {
            retry--;
            memset(password, 0, sizeof(password));
            return loginToAuthServer(retry);
        }
        else
        {
            dialogStd( DIALOG_OK, authGetError(), NULL, NULL, 0, NULL, 0 );
        }
        memset(password, 0, sizeof(password));
        return FALSE;
    }

    if( game_state.cryptic )
        saveAutoResumeInfoCryptic();

    saveAutoResumeInfoToRegistry();

    // reset some game states just to make sure items do not copy over from auth to auth
    db_info.bonus_slots = 0;
    db_info.shardXferTokens = 0;
    db_info.shardname[0] = 0;

    AccountInventorySet_Release( &db_info.account_inventory );

    memset(password, 0, sizeof(password));
    return TRUE;
}

static void loginFrame()
{
    static float scale = 0;
    static HybridElement heAccounts[] = { {0, "CreateAccountString"}, {1, "ManageAccountString"}, {2, "SettingsString"} };
    int i;
    AtlasTex *mark, *back = white_tex_atlas;
    CBox box;
    Entity *e = playerPtr();
    float screenScaleX, screenScaleY;
    float UIScale;
    float textScale;
    int exitButtonX, loginButtonX;
    AtlasTex *typefield = atlasLoadTexture("loginscreen_typefield");
    int loginButtonFlags;
    static HybridElement sButtonLogin = {0, "LoginString"};
    static HybridElement sButtonExit = {0, "ExitString"};

    textScale = screenScaleX = screenScaleY = 1.f;

    // DGNOTE 7/27/2010
    // For now, set this to textScale, i.e. the minumum of ScaleX and ScaleY.  That works as long as we letterbox the splash screen. If we ever get the artwork for a
    // widescreen splash, which we then procedurally select S and T for, this can probably be changed to screenScaleX.  The point about a separate variable is that
    // it's set once, use everywhere, therefore modification becomes relatively trivial.
    UIScale = textScale;

    if( s_loggedIn_serverSelected == LOGIN_STAGE_START)
    {
        scale += TIMESTEP*.1f;
        if( scale > 1.0f )
            scale = 1.0f;
    }
    else
    {
        scale -= TIMESTEP*.1f;
        if( scale < 0.f )
        {
            scale = 0.0f;
            return;
        }
    }

    for( i = 0; i < MAX_SERVER_COUNT; i++ )
        clearToolTip( &gServerTips[i]  );

    if( scale < 1 )
        return;

    BuildCBox(&box, xPosition(screenScaleX, screenScaleY, LOGIN_X),
        yPosition(screenScaleX, screenScaleY, LOGIN_Y),
        LOGIN_WD * UIScale, (LOGIN_HT + 200) * UIScale);
    //drawBox(&box, 20, 0xff0000ff, 0);


    //*****************************************
    // Account Name Text Entry Field
    //*****************************************

    gTextAttr_LightHybridBlueHybrid12.piScale = (int *)(int)(1.f*SMF_FONT_SCALE*textScale);

    gTextAttr_WhiteTitle24Italics.piColor = (int *)LOGIN_FOREGROUND_TEXT_COLOR;  //set custom color depending on current artist
    smf_Display(s_labelAccount, 
        xPosition(screenScaleX, screenScaleY, LOGIN_X), 
        yPosition(screenScaleX, screenScaleY, LOGIN_Y + 14), 20,
        LOGIN_WD * UIScale, LOGIN_HT * UIScale, 0, 0, &gTextAttr_WhiteTitle24Italics, 0);
    gTextAttr_WhiteTitle24Italics.piColor = (int *)0xffffffff;  //revert color

    display_sprite(typefield, xPosition(screenScaleX, screenScaleY, LOGIN_X),
        yPosition(screenScaleX, screenScaleY, LOGIN_Y + 52), 10, UIScale, UIScale, CLR_WHITE);
    smf_SetScissorsBox(s_editAccount, 
        xPosition(screenScaleX, screenScaleY, LOGIN_X + R12),
        yPosition(screenScaleX, screenScaleY, LOGIN_Y + 60),
        LOGIN_WD * UIScale, LOGIN_HT * UIScale);
    smf_Display(s_editAccount,
        xPosition(screenScaleX, screenScaleY, LOGIN_X + R12), 
        yPosition(screenScaleX, screenScaleY, LOGIN_Y + 60), 20,
        LOGIN_WD * UIScale, LOGIN_HT * UIScale, 0, 0, &gTextAttr_LightHybridBlueHybrid12, 0);

    //*****************************************
    // Remember Account Name Radio Button
    //*****************************************

    BuildCBox(&box,
        xPosition(screenScaleX, screenScaleY, LOGIN_X),
        yPosition(screenScaleX, screenScaleY, LOGIN_Y + 92),
        LOGIN_WD * UIScale, 15*UIScale);
    if( mouseCollision(&box) )
    {
        if( mouseClickHit( &box, MS_LEFT) )
        {
            g_iDontSaveName = !g_iDontSaveName;
            saveAutoResumeInfoToRegistry();
        }
    }

    if( g_iDontSaveName )
        mark = atlasLoadTexture( "loginscreen_checkfield.tga" );
    else
        mark = atlasLoadTexture( "loginscreen_checkfield_selected.tga" );

    gTextAttr_WhiteHybridBold12.piColor = (int *)LOGIN_FOREGROUND_TEXT_COLOR;  //set custom color depending on current artist
    smf_Display(s_rememberAccount, 
        xPosition(screenScaleX, screenScaleY, LOGIN_X), 
        yPosition(screenScaleX, screenScaleY, LOGIN_Y + 92), 20,
        LOGIN_WD * UIScale, LOGIN_HT * UIScale, 0, 0, &gTextAttr_WhiteHybridBold12, 0);
    gTextAttr_WhiteHybridBold12.piColor = (int *)0xffffffff;  //revert color
    display_sprite(mark,
        xPosition(screenScaleX, screenScaleY, LOGIN_X + LOGIN_WD / 2) - 40 * textScale
        - str_wd(&hybridbold_12, textScale, textScale, textStd("RememberAccountName")) / 2 - mark->width / 2 * textScale,
        yPosition(screenScaleX, screenScaleY, LOGIN_Y + 100) - mark->height / 2 * textScale, 20, textScale, textScale,
        CLR_WHITE );

    //*****************************************
    // Account Password Text Entry Field
    //*****************************************

    gTextAttr_WhiteTitle24Italics.piColor = (int *)LOGIN_FOREGROUND_TEXT_COLOR;  //set custom color depending on current artist
    smf_Display(s_labelPassword, 
        xPosition(screenScaleX, screenScaleY, LOGIN_X), 
        yPosition(screenScaleX, screenScaleY, LOGIN_Y + 127), 20,
        LOGIN_WD * UIScale, LOGIN_HT * UIScale, 0, 0, &gTextAttr_WhiteTitle24Italics, 0);
    gTextAttr_WhiteTitle24Italics.piColor = (int *)0xffffffff;  //revert color

    display_sprite(typefield, xPosition(screenScaleX, screenScaleY, LOGIN_X), 
        yPosition(screenScaleX, screenScaleY, LOGIN_Y + 165), 10, UIScale, UIScale, CLR_WHITE);
    smf_SetScissorsBox(s_editPassword, 
        xPosition(screenScaleX, screenScaleY, LOGIN_X + R12), 
        yPosition(screenScaleX, screenScaleY, LOGIN_Y + 173),
        LOGIN_WD * UIScale, LOGIN_HT * UIScale);
    smf_Display(s_editPassword, 
        xPosition(screenScaleX, screenScaleY, LOGIN_X + R12), 
        yPosition(screenScaleX, screenScaleY, LOGIN_Y + 173), 20,
        LOGIN_WD * UIScale, LOGIN_HT * UIScale, 0, 0, &gTextAttr_LightHybridBlueHybrid12, 0);

    //*****************************************
    // Buttons and Input Handling
    //*****************************************

    if (inpEdge(INP_RETURN))
    {
        int stuff = strlen(s_editAccount->outputString);
        stuff = strlen(s_editPassword->outputString);
        sndPlay("N_Select", SOUND_GAME);
    }

    loginButtonX = xPosition(screenScaleX, screenScaleY, LOGIN_X + LOGIN_WD - LOGIN_BUTTON_WD / 2 - 1);
    loginButtonFlags = HYBRID_BAR_BUTTON_NO_OUTLINE;
    if (!strlen(s_editAccount->outputString) || !strlen(s_editPassword->outputString))
    {
        loginButtonFlags |= HYBRID_BAR_BUTTON_DISABLED;
    }

    if (D_MOUSEHIT == drawHybridBarButton(&sButtonLogin, loginButtonX, yPosition(screenScaleX, screenScaleY, LOGIN_BUTTON_Y), 20.0f, LOGIN_BUTTON_WD, UIScale*0.9f, 0x648d0cff, loginButtonFlags)
        || inpEdge( INP_RETURN )
        || game_state.editnpc && strlen(s_editAccount->outputString) && strlen(s_editPassword->outputString))
    {
        char *estr;

        dialogClearQueue( 0 );

        smf_OnLostFocus(s_editAccount, NULL);
        smf_OnLostFocus(s_editPassword, NULL);

        estr=s_editAccount->outputString;
        if(estr)
        {
            // check if the previously used login is being used, otherwise reset character creation
            if (g_achAccountName && strcmp(estr, g_achAccountName) != 0)
            {
                resetCharacterCreationFlag();
            }
            strcpy(g_achAccountName, estr);
        }

        estr=s_editPassword->outputString;
        if(estr && estr[0])
        {
            // cryptStore() assumes both destination and source are the same size
            strncpy_s(g_achPassword, sizeof(g_achPassword), estr, _TRUNCATE);
            cryptStore(g_achPassword, g_achPassword, sizeof(g_achPassword));
        }

        if (loginToAuthServer(0)) {
            s_loggedIn_serverSelected = LOGIN_STAGE_SERVER_SELECT;
        }
    }

    exitButtonX = xPosition(screenScaleX, screenScaleY, LOGIN_X + LOGIN_BUTTON_WD / 2);
    if( D_MOUSEHIT == drawHybridBarButton(&sButtonExit, exitButtonX, 
        yPosition(screenScaleX, screenScaleY, LOGIN_BUTTON_Y), 20.0f, 
        LOGIN_BUTTON_WD, UIScale*0.9f, 0x9c2206ff,
        HYBRID_BAR_BUTTON_NO_OUTLINE))
    {
        sndPlay("N_Deselect", SOUND_GAME);
        windowExit(0);
    }

#ifndef DISABLE_SETTINGS_BUTTON
    if (D_MOUSEHIT == drawHybridBar(&heAccounts[2], 0, 
        xPosition(screenScaleX, screenScaleY, WEB_X + WEB_BUTTON_WD / 2),
        yPosition(screenScaleX, screenScaleY, LOGIN_BUTTON_Y + 70), 20.0f, 
        SETTINGS_BUTTON_WD, UIScale, HB_ROUND_ENDS | HB_ALWAYS_FULL_ALPHA, 1.f, H_ALIGN_LEFT, V_ALIGN_CENTER ))
    {
        windows_Show("options");
    }
#endif

    // dev mode character fetch button
    if((encryptedKeyedAccessLevel() || isDevelopmentMode()) && fileExists("c:/game/tools/util/dbquery.exe"))
        debug_DrawGetCharacter();
}

static int s_loginTimedOut()
{
    static U32 nextMessageUpdate = 0;    //when should we display the disconnect message again?
    U32 currentTime = timerCpuSeconds();
    int secondsSinceUpdate = dbclient_getTimeSinceMessage(currentTime);

    if(secondsSinceUpdate >= 0 &&  secondsSinceUpdate < DB_CONNECTION_TIMEOUT)
        nextMessageUpdate = 0;
    else if (!nextMessageUpdate || nextMessageUpdate<=currentTime)
    {
        nextMessageUpdate = currentTime + DB_CONNECTION_TIMEOUT;    
        return secondsSinceUpdate;
    }

    return 0;    //not time for the message update or we haven't logged out.
}

static void s_quitToLogin(void *notused)
{
    dbSendQuit();
    restartLoginScreen();
    s_loggedIn_serverSelected = LOGIN_STAGE_START;
}

static bool drawQueueInfo( float y )
{
    int position, count;
    int txtclr = CLR_WHITE, circlr[3] = { 0xffffff22, 0xffffff22, 0xffffff22 };
    float textX = 500;
    float textY = 350;
    

    dbclient_getQueueStatus(&position, &count);
    
    font( &title_12_no_outline );
    font_color( txtclr, txtclr );
    if (position > 0)
    {
        cprntEx( textX, textY-10, 20, 2.f, 2.f, CENTER_X, textStd("QueuePosition", g_ServerName, position, count) );
    }
    else
    {
        cprntEx( textX, textY-10, 20, 2.f, 2.f, CENTER_X, textStd("RetrievingCharacterList", g_ServerName) );
    }
    return true;
}

//----------------------------------------------------------------------------------------------------------------------------------
// Login Menu - 3 screens really, Login -> ServerSelect -> CharacterSelect
//----------------------------------------------------------------------------------------------------------------------------------

static chooseUltraModeLow(void*data)
{
    GfxSettings gfxSettings={0};
    gfxGetSettings(&gfxSettings);
    gfxGetQualityAdvancedSettings( &gfxSettings );        
    gfxApplySettings( &gfxSettings, 1, 0 );
}

static chooseUltraModeHigh(void*data)
{
    GfxSettings gfxSettings={0};
    gfxGetSettings(&gfxSettings);
    gfxGetUltraAdvancedSettings( &gfxSettings );        
    gfxApplySettings( &gfxSettings, 1, 0 );
}

#include <utilitieslib/utils/rand.h>

#define RETRIEVING_CHARACTER_LIST_SCREEN_DELAY_MS 1000

void loginMenu()
{
    static int initCharacterSelect = 0;
    Entity *e = playerPtr();
    __int64 time;

    int i;

    int dbTimeoutSeconds = s_loginTimedOut();

    if (dbhasError())
    {
        dialogStd(DIALOG_OK, dbGetError(), NULL, NULL, s_quitToLogin, NULL, 0 );
    }

    if(dbTimeoutSeconds!=0 && s_loggedIn_serverSelected > LOGIN_STAGE_SERVER_SELECT)
    {
        if(dbTimeoutSeconds<0)
            dialogTimed(DIALOG_YES_NO, "LoginTimedOutText", NULL, NULL, s_quitToLogin, NULL, 0, 0);
        else
            dialogTimed(DIALOG_YES_NO, textStd("LoginTimedOutTextSeconds", dbTimeoutSeconds), NULL, NULL, s_quitToLogin, NULL, 0, 0);
    }

    // Use g_shardVisitorDBID as the definitive authority as to whether there's a transfer in progress or not
    if (!g_shardVisitorDBID)
    {
        // Only play the music if we're not here trying to do a shard visitor transfer
        sndPlay( "N_MenuMusic_loop", SOUND_MUSIC );
    }

    if( !s_LoginInit )
    {
        s_LoginInit = 1;

        // Use g_shardVisitorDBID as the definitive authority as to whether there's a transfer in progress or not
        if (g_shardVisitorDBID)
        {
            s_loggedIn_serverSelected = LOGIN_STAGE_SHARD_JUMP_DBCONNECT;
        }
        else
        {
            char password[MAX_PASSWORD_LEN];
            // --------------------
            // account edit
            if (!s_labelAccount)
            {
                s_labelAccount = smfBlock_Create();
                smf_SetFlags(s_labelAccount, SMFEditMode_Unselectable, SMFLineBreakMode_SingleLine, 
                    SMFInputMode_AnyTextNoTagsOrCodes, 0, SMFScrollMode_ExternalOnly,
                    SMFOutputMode_StripAllTagsAndCodes, SMFDisplayMode_AllCharacters, SMFContextMenuMode_None,
                    SMAlignment_Center, 0, 0, 0);
                smf_SetRawText(s_labelAccount, textStd("AccountString"), false);
            }

            if (!s_editAccount)
            {
                s_editAccount = smfBlock_Create();
                smf_SetFlags(s_editAccount, SMFEditMode_ReadWrite, SMFLineBreakMode_SingleLine, 
                    SMFInputMode_Alphanumeric, 14, SMFScrollMode_ExternalOnly,
                    SMFOutputMode_StripAllTagsAndCodes, SMFDisplayMode_AllCharacters, SMFContextMenuMode_None,
                    SMAlignment_Left, 0, "LoginScreen", -1);
                smf_SetCharacterInsertSounds(s_editAccount, "type2", "mouseover");
            }
            smf_SetRawText(s_editAccount, g_achAccountName, false);
            smf_CreateAndParse(g_achAccountName, s_editAccount); // Required so smf_SelectAllText works.
            smf_SelectAllText(s_editAccount);

            if (!s_rememberAccount)
            {
                s_rememberAccount = smfBlock_Create();
                smf_SetFlags(s_rememberAccount, SMFEditMode_Unselectable, SMFLineBreakMode_SingleLine, 
                    SMFInputMode_AnyTextNoTagsOrCodes, 0, SMFScrollMode_ExternalOnly,
                    SMFOutputMode_StripAllTagsAndCodes, SMFDisplayMode_AllCharacters, SMFContextMenuMode_None,
                    SMAlignment_Center, 0, 0, 0);
                smf_SetRawText(s_rememberAccount, textStd("RememberAccountName"), false);
            }

            // --------------------
            // password edit
            if (!s_labelPassword)
            {
                s_labelPassword = smfBlock_Create();
                smf_SetFlags(s_labelPassword, SMFEditMode_Unselectable, SMFLineBreakMode_SingleLine, 
                    SMFInputMode_AnyTextNoTagsOrCodes, 0, SMFScrollMode_ExternalOnly,
                    SMFOutputMode_StripAllTagsAndCodes, SMFDisplayMode_AllCharacters, SMFContextMenuMode_None,
                    SMAlignment_Center, 0, 0, 0);
                smf_SetRawText(s_labelPassword, textStd("PasswordString"), false);
            }

            if (!s_editPassword)
            {
                s_editPassword = smfBlock_Create();
                smf_SetFlags(s_editPassword, SMFEditMode_ReadWrite, SMFLineBreakMode_SingleLine, 
                    SMFInputMode_AnyTextNoTagsOrCodes, 16, SMFScrollMode_ExternalOnly,
                    SMFOutputMode_StripAllTagsAndCodes, SMFDisplayMode_AsterisksOnly, SMFContextMenuMode_None,
                    SMAlignment_Left, 0, "LoginScreen", -1);
                smf_SetCharacterInsertSounds(s_editPassword, "type2", "mouseover");
            }
            cryptRetrieve(password, g_achPassword, sizeof(password));
            smf_SetRawText(s_editPassword, password, false);
            memset(password, 0, sizeof(password));

            // --------------------
            // link to existing account
            if (!s_linkToAccount)
            {
                s_linkToAccount = smfBlock_Create();
                smf_SetFlags(s_linkToAccount, SMFEditMode_Unselectable, SMFLineBreakMode_SingleLine, 
                    SMFInputMode_AnyTextNoTagsOrCodes, 0, SMFScrollMode_ExternalOnly,
                    SMFOutputMode_StripAllTagsAndCodes, SMFDisplayMode_AllCharacters, SMFContextMenuMode_None,
                    SMAlignment_Center, 0, 0, 0);
                smf_SetRawText(s_linkToAccount, textStd("LinkToExistingAccountString"), false);
            }

            // --------------------
            // tooltip
            for ( i = 0; i < MAX_SERVER_COUNT; i++ )
                addToolTip( &gServerTips[i] );

            if (rdr_caps.chip & DX10_CLASS && !game_state.showLoginDialog )
            {
                dialog( DIALOG_TWO_RESPONSE_CANCEL, -1, -1, 400, 300, "UltraModeCapableText", "ChooseUltraModeLow", chooseUltraModeLow, "ChooseUltraModeHigh", chooseUltraModeHigh, 0, 0, 0, 0, 0, 0, 0);
                game_state.showLoginDialog = 1;
                saveAutoResumeInfoToRegistry();
            }
        }
    }

    time = timerMsecsSince2000();

    if( s_loggedIn_serverSelected < LOGIN_STAGE_IN_QUEUE )
    {
        float screenScaleX, screenScaleY;
        float textScale;
        AtlasTex * background = atlasLoadTexture(getDefaultLoadScreenName());

        textScale = screenScaleX = screenScaleY = 1.f;

        display_sprite(background, 0.f, 0.f, 2.f, 1.f, 1.f, CLR_WHITE);

        loginFrame();
        eulaFrame();
        serverFrame();
        ndaFrame();

        if( game_state.oldDriver )             
        {
            font( &game_12 );
            font_color( CLR_RED, CLR_RED );
            prnt( 10 * screenScaleX, 20 * screenScaleY, 1010, textScale, textScale, game_state.oldDriverMessage[0]); 

            font( &game_12 );
            font_color( CLR_WHITE, CLR_WHITE );
            prnt( 10 * screenScaleX, 32 * screenScaleY, 1010, 0.7f * textScale, 0.7f * textScale, game_state.oldDriverMessage[1]); 
            prnt( 10 * screenScaleX, 44 * screenScaleY, 1010, 0.7f * textScale, 0.7f * textScale, game_state.oldDriverMessage[2]); 
        }

        if ( game_state.safemode)
        {
            font(&game_12);
            font_color(CLR_RED, CLR_RED);
            prnt(10 * screenScaleX, 70 * screenScaleY, 1010, textScale, textScale, textStd("SafeModeWarning1"));
            font_color( CLR_WHITE, CLR_WHITE );
            prnt(10 * screenScaleX, 82 * screenScaleY, 1010, 0.7f * textScale, 0.7f * textScale, textStd("SafeModeWarning2"));
            prnt(10 * screenScaleX, 94 * screenScaleY, 1010, 0.7f * textScale, 0.7f * textScale, textStd("SafeModeWarning3"));
        }
        initCharacterSelect = 0;
    }
    else if(s_loggedIn_serverSelected == LOGIN_STAGE_IN_QUEUE || s_retrievingCharacterList_startMS + RETRIEVING_CHARACTER_LIST_SCREEN_DELAY_MS > time)
    {
        initCharacterSelect = 0;

        drawHybridCommon_CharacterSelectOrInQueue(0);

        drawQueueInfo( 100 );            
    }
     else if (s_loggedIn_serverSelected == LOGIN_STAGE_CHARACTER_SELECT)
    {
        if(!initCharacterSelect)
        {
            initCharacterSelect =1;
            //accountCatalogRequest();  // we get this information from the dbserver without having to request it.
            f_CharSelectTarget = characterPageOffset = 0; //reset to the first page
            s_naggedThisSession = false;
        }
        for( i = 0; i < MAX_SERVER_COUNT; i++ )
            clearToolTip( &gServerTips[i]  );

        // Force players to deal with the purchasing UI because it involves
        // money and strong confirmations, and with the rename UI because it
        // doesn't have a 'Cancel' button.
        drawHybridCommon_CharacterSelectOrInQueue(selectCharacterState == SELECT_CHARACTER_STORE || selectCharacterState == SELECT_CHARACTER_RENAME);

        selectCharacter();
    }
     else if (s_loggedIn_serverSelected == LOGIN_STAGE_SHARD_JUMP_DBCONNECT)
    {
        if (jumpToDbServer())
        {
            selectJumpedCharacter();
        }
        g_shardVisitorData[0] = 0;
        g_shardVisitorDBID = 0;
        g_shardVisitorUID = 0;
        g_shardVisitorCookie = 0;
        g_shardVisitorAddress = 0;
        g_shardVisitorPort = 0;
    }
}

