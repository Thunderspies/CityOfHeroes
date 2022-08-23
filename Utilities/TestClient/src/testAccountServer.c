#include <utilitieslib/stdtypes.h>
#include "testAccountServer.h"
#include <utilitieslib/utils/SuperAssert.h>
#include <utilitieslib/utils/timing.h>
#include "account/AccountCatalog.h"
#include "account/AccountData.h"
#include "chatter.h"
#include "player/player.h"
#include "entity/entity.h"
#include "clientcomm/dbclient.h"
#include "clientcomm/clientcomm.h"
#include <utilitieslib/network/net_packet.h>
#include "entity/entVarUpdate.h"
#include <utilitieslib/components/earray.h>
#include "testUtil.h"
#include "gameComm/initClient.h"
#include "gameData/costume_data.h"
#include <utilitieslib/utils/utils.h>
#include "UI/uiNet.h"
#include "cmdparse/cmdgame.h"
#include "entity/character_inventory.h"
#include "UI/uiChat.h"
#include <utilitieslib/language/MessageStoreUtil.h>
#include "testClientInclude.h"
#include "bases/DetailRecipe.h"
#include "bases/basedata.h"
#include <utilitieslib/components/EString.h>
#include "entity/entPlayer.h"
#include "entity/character_eval.h"

// We'd like to see the catalog arrive faster than this for good
// performance.
#define TEST_CATALOG_TIMEOUT            15

// This is how long we wait before re-fetching the catalog to check if
// the product list has changed so it has something we can use.
#define TEST_CATALOG_RETRY_TIMEOUT        180

// This is how long we wait for the system to retry - if we still don't
// get anything after this long, there's a big problem.
#define TEST_CATALOG_WAIT_TIMEOUT        120

// This is how long we wait for the character transfer shard list to get
// sent. We keep waiting but print a message after this time.
#define TEST_SHARD_LIST_TIMEOUT            15

// This is how long we wait for the purchase to take effect before
// printing a message.
#define TEST_PURCHASE_TIMEOUT            60

// This is how long we wait to be able to reconnect to the DBServer before
// printing a warning message.
#define TEST_LOGIN_RECONNECT_TIMEOUT    20

// The longest a shard name could be (this is more than generous
// compared to the actual names).
#define CHAR_TRANSFER_MAX_SHARD_NAME    60

// The longest a character name could be (this is also more generous
// than the game really permits names to be).
#define CHAR_TRANSFER_MAX_CHAR_NAME        30

typedef enum TestAccountType
{
    TEST_ACCOUNT_IDLE,
    TEST_ACCOUNT_GET_CATALOG,
    TEST_ACCOUNT_CATALOG_RETRY_WAIT,
    TEST_ACCOUNT_CATALOG_TIMEOUT_WAIT,
    TEST_ACCOUNT_GET_TRANSFER_SHARD_LIST,
    TEST_ACCOUNT_LOGIN_RECONNECT,
    TEST_ACCOUNT_EXECUTE_REQUESTS,
} TestAccountType;

typedef struct DestinationShard
{
    char shardName[CHAR_TRANSFER_MAX_SHARD_NAME];
    int freeSlots;
} DestinationShard;

static TestAccountType testAccountState = TEST_ACCOUNT_IDLE;
static U32 testAccountStateTime = 0;
static bool testAccountShardTransfer = false;

static DestinationShard **testAccountShards = NULL;
static testAccountPendingTransfer = false;
static char testAccountCharTransferOldName[CHAR_TRANSFER_MAX_CHAR_NAME];
static char testAccountCharTransferNewName[CHAR_TRANSFER_MAX_CHAR_NAME];

MP_DEFINE(DestinationShard);

static void prepareAccountTest(void);

static void changeState(TestAccountType new_state);
static U32 getElapsedStateTime(void);
static void resetElapsedStateTime(void);

static void checkCatalogReady(void);
static void checkCatalogRetryWait(void);
static void checkCatalogTimeoutWait(void);
static void checkGetTransferShardList(void);
static void checkGetBillingProfile(void);
static void checkGetPaymentPreview(void);
static void checkPurchase(void);
static void checkLoginReconnect(void);
static void executeAccountRequests(void);

static void startGetCatalog(void);
static void startGetTransferShardList(void);

void testAccountServer(void)
{
    switch (testAccountState)
    {
        case TEST_ACCOUNT_IDLE:
            prepareAccountTest();
            break;

        case TEST_ACCOUNT_GET_CATALOG:
            checkCatalogReady();
            break;

        case TEST_ACCOUNT_CATALOG_RETRY_WAIT:
            checkCatalogRetryWait();
            break;

        case TEST_ACCOUNT_CATALOG_TIMEOUT_WAIT:
            checkCatalogTimeoutWait();
            break;

        case TEST_ACCOUNT_GET_TRANSFER_SHARD_LIST:
            checkGetTransferShardList();
            break;

        case TEST_ACCOUNT_LOGIN_RECONNECT:
            checkLoginReconnect();
            break;

        case TEST_ACCOUNT_EXECUTE_REQUESTS:
            executeAccountRequests();

        default:
            break;
    }
}

void testAccountAddCharacterCount(char *shard, int count)
{
    DestinationShard *new_dest_shard;

    new_dest_shard = MP_ALLOC(DestinationShard);

    strcpy_s(new_dest_shard->shardName, CHAR_TRANSFER_MAX_SHARD_NAME, shard);
    new_dest_shard->freeSlots = 12 - count;

    eaPush(&testAccountShards, new_dest_shard);

    // All the error reporting must come last otherwise memory will be
    // left dangling.
    if (new_dest_shard->freeSlots < 0)
    {
        sendChatToLauncher("More characters (%d) than possible slots on shard '%s' - restarting", count, shard);
        startGetCatalog();
    }

    if (count < 0)
    {
        sendChatToLauncher("Negative number of characters (%d) on shard '%s' - restarting", count, shard);
        startGetCatalog();
    }
}

void testAccountReceiveCharacterCountsDone(void)
{
    if (eaSize(&testAccountShards) == 0)
    {
        sendChatToLauncher("No destination shards provided - restarting");
        startGetCatalog();
    }
    else
    {
        sendChatToLauncher("Received %d destination shards", eaSize(&testAccountShards));

        startGetCatalog();
    }
}

void testAccountCharacterCountsError(char *errmsg)
{
    sendChatToLauncher("Destination shard error from server (%s) - restarting",errmsg);
    startGetCatalog();
}

bool testAccountIsTransferPending(void)
{
    return testAccountPendingTransfer;
}

char *testAccountGetNewCharName(void)
{
    return testAccountCharTransferNewName;
}

char *testAccountGetOriginalCharName(void)
{
    return testAccountCharTransferOldName;
}

void testAccountRevertCharName(void)
{
    Entity *plyr;

    sendChatToLauncher("Successful transfer - changing name back to %s from %s", testAccountCharTransferOldName, testAccountCharTransferNewName);

    plyr = playerPtr();

    // We'll have been given a rename token as part of the transfer process
    // because of the name collision so this will use it up and restore
    // our name.
    START_INPUT_PACKET(pak,CLIENTINP_UPDATE_NAME);
    pktSendString(pak, testAccountCharTransferOldName);
    END_INPUT_PACKET

    // The transfer happened, so kick/login is no longer expected.
    testAccountPendingTransfer = false;

    // This also concludes the transfer process, since the character is
    // now ready for a new purchase.
    startGetCatalog();
}

bool IsTestAccountServer_SuperPacksOptionActivated(void)
{
    return (g_testModeAccount & TEST_ACCOUNTSERVER_SUPERPACKS);
}

// This is needed for logging back in to the DBServer.
extern int loginWrapper(void);
extern char character_name[256];
extern int gPlayerNumber;

void testAccountLoginReconnect(void)
{
    bool old_found = false;
    bool new_found = false;

    // Nothing else can happen while we're disconnected from the servers
    // because we can't get or send any traffic.
    changeState(TEST_ACCOUNT_LOGIN_RECONNECT);

    // We may have to log in multiple times because we might beat the transfer process.
    while (!new_found)
    {
        old_found = false;
        new_found = false;

        // Keep trying the auth etc until we get in.
        while (!loginWrapper());

        gPlayerNumber = 0;

        while (!new_found && !old_found && gPlayerNumber < db_info.player_count)
        {
            if (stricmp(testAccountGetNewCharName(), db_info.players[gPlayerNumber].name) == 0)
            {
                new_found = true;
            }

            if (stricmp(testAccountGetOriginalCharName(), db_info.players[gPlayerNumber].name) == 0)
            {
                old_found = true;
            }

            if (!new_found && !old_found)
            {
                gPlayerNumber++;
            }
        }

        // The new name not being there might mean the transfer hasn't happened yet (in which
        // case the old name should still be there and we can just disconnect and try again)
        // or something has gone really wrong and we cannot recover.
        if (!new_found)
        {
            if (!old_found)
            {
                testClientLog(TCLOG_FATAL, "Couldn't find renamed character after transfer\n");
                FatalErrorf("Failed re-connect after character transfer");
            }
            else
            {
                dbDisconnect();
            }
        }
    }

    if(choosePlayerWrapper( gPlayerNumber, 0 ))
    {
        strcpy(character_name, db_info.players[gPlayerNumber].name);
        dbDisconnect();
        tryConnect();
        if( !commConnected() ) {
            testClientLog(TCLOG_FATAL, "Couldn't reconnect to MapServer after character transfer\n");
            FatalErrorf("Failed MapServer reconnect after character transfer");
        } else {
            extern int do_not_assert_on_bad_indexed_string_read;
            do_not_assert_on_bad_indexed_string_read = 1;
            // Fake costumes being loaded for entReceive to be happy
            costume_HashTable = stashTableCreateWithStringKeys(1, StashDeepCopyKeys);
            costume_HashTableExtraInfo = stashTableCreateWithStringKeys(1, StashDeepCopyKeys);
            commReqScene(0);

            // Now we're logged back in we can restore our name.
            testAccountRevertCharName();
        }
    }
    else
    {
        testClientLog(TCLOG_FATAL, "Couldn't choose character correctly after character transfer\n");
        FatalErrorf("Failed character choice after character transfer");
    }
}

static void prepareAccountTest(void)
{
    Entity *plyr;

    plyr = playerPtr();
    
    if (plyr && plyr->db_flags & DBFLAG_RENAMEABLE)
    {
        sendChatToLauncher("Player already has a rename token - consuming");

        // Clear the name on the server otherwise the purchase will fail.
        START_INPUT_PACKET(pak,CLIENTINP_UPDATE_NAME);
        pktSendString(pak, plyr->name);
        END_INPUT_PACKET

        plyr->db_flags &= ~DBFLAG_RENAMEABLE;
    }

    startGetCatalog();
}

static void checkCatalogReady(void)
{
    if (accountCatalogIsReady())
    {
        // Start buying.
        sendChatToLauncher("Got the account catalog - time to buy!");

        if (testAccountShardTransfer)
        {
            startGetTransferShardList();
        }
        else
        {
            sendChatToLauncher("Starting the request execution loop!");
            changeState(TEST_ACCOUNT_EXECUTE_REQUESTS);
        }
    }
    else
    {
        if (accountCatalogIsOffline())
        {
            sendChatToLauncher("Account catalog is down - restarting");
            accountCatalogRequest();
        }
        else
        {
            if (getElapsedStateTime() > TEST_CATALOG_TIMEOUT)
            {
                sendChatToLauncher("Catalog hasn't arrived - waiting for it");
                changeState(TEST_ACCOUNT_CATALOG_TIMEOUT_WAIT);
            }
        }
    }
}

static void checkCatalogRetryWait(void)
{
    if (getElapsedStateTime() > TEST_CATALOG_RETRY_TIMEOUT)
    {
        sendChatToLauncher("Requesting the catalog again to check for product changes");
        startGetCatalog();
    }
}

static void checkCatalogTimeoutWait(void)
{
    if (accountCatalogIsReady())
    {
        sendChatToLauncher("Got the account catalog (finally) - time to buy!");

        if (testAccountShardTransfer)
        {
            startGetTransferShardList();
        }
        else
        {
            startGetCatalog();
        }
    }
    else
    {
        if (getElapsedStateTime() > TEST_CATALOG_WAIT_TIMEOUT)
        {
            sendChatToLauncher("Catalog still hasn't arrived - keep waiting");
            resetElapsedStateTime();
        }
    }
}

static void checkGetTransferShardList(void)
{
    if (getElapsedStateTime() > TEST_SHARD_LIST_TIMEOUT)
    {
        sendChatToLauncher("Destination shard list hasn't arrived yet - keep waiting");
        resetElapsedStateTime();
    }
}

static void checkPurchase(void)
{
    Entity *plyr;

    plyr = playerPtr();

    if (plyr->db_flags & DBFLAG_RENAMEABLE)
    {
        sendChatToLauncher("Purchase arrived - success!");

        plyr->db_flags &= ~DBFLAG_RENAMEABLE;
        startGetCatalog();
    }
    else
    {
        if (getElapsedStateTime() > TEST_PURCHASE_TIMEOUT)
        {
            sendChatToLauncher("Purchase not detected yet - still waiting");
            resetElapsedStateTime();
        }
    }
}

static void checkLoginReconnect(void)
{
    if (getElapsedStateTime() > TEST_LOGIN_RECONNECT_TIMEOUT)
    {
        sendChatToLauncher("Post-transfer DBServer reconnect not happened yet - still trying");
        resetElapsedStateTime();
    }
}

static void openSuperPacks(void)
{
    int salvageIndex;
    SalvageInventoryItem **salvageInventory = playerPtr()->pchar->salvageInv;

    for (salvageIndex = eaSize(&salvageInventory) - 1; salvageIndex >= 0; --salvageIndex)
    {
        const SalvageItem *salvage = salvageInventory[salvageIndex]->salvage;
        if (salvage && salvage->ppchRewardTables)
        {
            addSystemChatMsg(textStd("Attempting to open a salvage named %s", salvage->pchName), INFO_SVR_COM, 0);
            sendSalvageOpen(salvage->pchName, 1);
        }
    }
}

static void startCertificationGrants(void)
{
    const DetailRecipe **grantRecipes = NULL;
    int recipeIndex;
    Character *pchar = playerPtr()->pchar;
    int recipeCount;
    const DetailRecipe *recipe;

    if (!grantRecipes)
    {
        for (recipeIndex = eaSize(&g_DetailRecipeDict.ppRecipes) - 1; recipeIndex >= 0; --recipeIndex)
        {
            recipe = g_DetailRecipeDict.ppRecipes[recipeIndex];

            if ((recipe->flags & RECIPE_CERTIFICATON || recipe->flags & RECIPE_VOUCHER)
                && stricmp(recipe->ppWorkshops[0]->pchName, "worktable_alt"))
            {
                eaPushConst(&grantRecipes, recipe);
            }
        }
    }

    recipeCount = eaSize(&grantRecipes);
    for (recipeIndex = recipeCount - 1; recipeIndex >= 0; --recipeIndex)
    {
        recipe = grantRecipes[recipeIndex];

        if (!character_DetailRecipeCreatable(pchar, recipe, NULL, false, false, -1))
        {
            eaRemoveConst(&grantRecipes, recipeIndex);
        }
    }

    recipeCount = eaSize(&grantRecipes);
    if (recipeCount > 0)
    {
        char * estr;
    
        recipeIndex = rand() % recipeCount;
        recipe = grantRecipes[recipeIndex];

        estrCreate(&estr);
        estrPrintf(&estr, "account_certification_buy %i 1 %s", (recipe->flags & RECIPE_VOUCHER) != 0, recipe->pchName);
        commAddInput(estr);
        estrDestroy(&estr);

        addSystemChatMsg(textStd("Attempting to grant a certification from the recipe named %s", recipe->pchName), INFO_SVR_COM, 0);
    }
}

static void startCertificationClaims(void)
{
    AccountInventorySet* invSet;
    Entity * e = playerPtr();
    int i;
    if(!e->pl->account_inv_loaded)
    {
        return;
    }

    invSet = ent_GetProductInventory( e );
    for (i = 0; i < eaSize(&invSet->invArr); i++)
    {
        AccountInventory *pItem = invSet->invArr[i];
        int available = pItem->granted_total - pItem->claimed_total;
        const DetailRecipe *pRecipe = detailrecipedict_RecipeFromName(pItem->recipe);

        if (pItem->invType != kAccountInventoryType_Certification && pItem->invType != kAccountInventoryType_Voucher)
            continue;

        if (available > 0 && character_DetailRecipeCreatable(e->pchar, pRecipe, NULL, false, false, 1))
        {
            char *cmd = 0;
            estrPrintf(&cmd, "account_certification_claim %s", pItem->recipe);
            cmdParse(cmd);
            estrDestroy(&cmd);

            addSystemChatMsg(textStd("Attempting to claim a certification named %s", pItem->recipe), INFO_SVR_COM, 0);

            break;
        }
    }
}

static void executeAccountRequests(void)
{
    if (g_testModeAccount & TEST_ACCOUNTSERVER_SUPERPACKS)
    {
        openSuperPacks();
    }

    if (g_testModeAccount & TEST_ACCOUNTSERVER_CERTIFICATION_GRANTS)
    {
        startCertificationGrants();
    }

    if (g_testModeAccount & TEST_ACCOUNTSERVER_CERTIFICATION_CLAIMS)
    {
        startCertificationClaims();
    }
}

static void startGetCatalog(void)
{
    // This is the easiest place to clear out memory from transactions
    // whether successful or not because all transactions begin by
    // getting the product catalogue.
    eaDestroy(&testAccountShards);
    MP_DESTROY(DestinationShard);

    //accountCatalogRequest();
    changeState(TEST_ACCOUNT_GET_CATALOG);
}

static void startGetTransferShardList(void)
{
    MP_CREATE(DestinationShard, 3);

    if (!dbGetCharacterCountsAsync())
    {
        commGetCharacterCountsAsync();
    }

    changeState(TEST_ACCOUNT_GET_TRANSFER_SHARD_LIST);
}

static void changeState(TestAccountType new_state)
{
    devassert(new_state != TEST_ACCOUNT_IDLE);

    testAccountState = new_state;
    testAccountStateTime = timerSecondsSince2000();
}

static U32 getElapsedStateTime(void)
{
    return timerSecondsSince2000() - testAccountStateTime;
}

static void resetElapsedStateTime(void)
{
    testAccountStateTime = timerSecondsSince2000();
}
