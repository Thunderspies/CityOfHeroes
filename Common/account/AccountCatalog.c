
#include "account/AccountData.h"
#include "account/AccountCatalog.h"
#include <utilitieslib/utils/SuperAssert.h>
#include <utilitieslib/components/MemoryPool.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/utils/textparser.h>
#include "entity/LoadDefCommon.h"
#include <utilitieslib/components/StashTable.h>
#include <utilitieslib/network/net_structdefs.h>
#include <utilitieslib/network/net_packet_common.h>
#include <utilitieslib/network/crypt.h>
#include <utilitieslib/utils/utils.h>

#ifdef CLIENT
#include "clientcomm/dbclient.h"
#include "clientcomm/clientcomm.h"
#include "player/inventory_client.h"
#endif

#if defined(CLIENT) || defined(SERVER)
#include "auth/authUserData.h"
#endif

#if defined(CLIENT) && !defined(TEST_CLIENT)
#include "formatter/smf_main.h"
#include <utilitieslib/language/MessageStoreUtil.h>
#endif

#if defined(SERVER)
#include "entity/entity.h"
#include "entity/entPlayer.h"
#include "player/badges.h"
#include "entity/RewardToken.h"
#include "dbcomm/dbcomm.h"
#include "entity/costume.h"
#include "gameData/costume_data.h"
#include "player/badges_server.h"
#include <utilitieslib/utils/file.h>
#endif

// This is how long we wait in seconds for the account server to reply to
// us with the complete catalog (via the DB server).
#define ACCOUNT_CATALOG_FETCH_TIMEOUT    10
#define ACCOUNT_CATALOG_FETCH_TIMEOUT_RETRY    30

// The name of the .def file with all the account products in it.
#define PRODUCT_CATALOG_DEF_FILENAME    "defs/account/product_catalog.def"

typedef enum AccountCatalogType
{
    ACCOUNT_CATALOG_UNINITIALIZED,
    ACCOUNT_CATALOG_FETCHING,
    ACCOUNT_CATALOG_INITIALIZED,
    ACCOUNT_CATALOG_ERROR_TIMEOUT,        // AccountServer is offline
    ACCOUNT_CATALOG_ERROR_NO_SERVICE,    // MTX processing is offline
    ACCOUNT_CATALOG_ERROR_EMPTY
} AccountCatalogType;

#if defined(CLIENT) && !defined(TEST_CLIENT)
#define DISPLAY_COST_STRING_LEN        50

typedef struct AccountCatalogDisplay
{
    SMFBlock *SMFContext;
    char* longDescTranslated;
    char costString[DISPLAY_COST_STRING_LEN];
} AccountCatalogDisplay;
#endif

typedef struct AccountCatalogList
{
    const AccountProduct** catalog;
    cStashTable skuIdIndex;
    cStashTable recipeIndex;
} AccountCatalogList;

// These names must match the authbit names. The fulfillment code sets the
// authbit by name.
static StaticDefineInt parse_ProductAuthBitEnum[] =
{
    DEFINE_INT
    {    "RogueAccess",        PRODUCT_AUTHBIT_ROGUE_ACCESS },
    {    "RogueCompleteBox",    PRODUCT_AUTHBIT_ROGUE_COMPLETE },
    DEFINE_END
};

static StaticDefineInt parse_InventoryTypeEnum[] =
{
    DEFINE_INT
    {    "Certification",            kAccountInventoryType_Certification },
    {    "Voucher",                    kAccountInventoryType_Voucher },
    {    "PlayerInventory",            kAccountInventoryType_PlayerInventory },
    DEFINE_END
};

StaticDefineInt parse_AccountProductFlags[] =
{
    DEFINE_INT
    {"Unpublished",    PRODFLAG_NOT_PUBLISHED    },
    {"FreeForVIP",    PRODFLAG_FREE_FOR_VIP    },
    DEFINE_END
};

static ParseTable parse_ProductCatalogItem[] =
{
    { "Title",                TOK_STRING(AccountProduct, title, 0), 0 },
    { "SKU",                TOK_FIXEDCHAR(AccountProduct, sku_id.c), 0 },
    { "InventoryType",        TOK_INT(AccountProduct, invType, kAccountInventoryType_Count), parse_InventoryTypeEnum },
    { "ProductFlags",        TOK_FLAGS(AccountProduct,productFlags,0), parse_AccountProductFlags        },
    { "InventoryCount",        TOK_INT(AccountProduct, invCount, 0), 0 },
    { "GrantLimit",            TOK_INT(AccountProduct, grantLimit, 0), 0 },
    { "InventoryMax",        TOK_REDUNDANTNAME | TOK_INT(AccountProduct, grantLimit, 0), 0 }, // deprecated name
    { "ExpirationSecs",        TOK_INT(AccountProduct, expirationSecs, 0), 0 },
    { "Global",                TOK_BOOL(AccountProduct, isGlobal, 0), 0 },
    { "AuthBit",            TOK_INT(AccountProduct, authBit, PRODUCT_AUTHBIT_INVALID), parse_ProductAuthBitEnum },
    { "Recipe",                TOK_STRING(AccountProduct, recipe, 0), 0 },
    { "ItemKey",            TOK_REDUNDANTNAME | TOK_STRING(AccountProduct, recipe, 0), 0 }, // deprecated name
    { "StartDate",            TOK_INT(AccountProduct, productStartDt, 0), 0 },
    { "EndDate",            TOK_INT(AccountProduct, productEndDt, 0), 0 },
    { "End",                TOK_END, 0 },
    { "",                    0, 0 }
};

static ParseTable parse_ProductCatalog[] =
{
    { "CatalogItem",    TOK_STRUCT(AccountCatalogList, catalog, parse_ProductCatalogItem), 0 },
    { "",                0, 0 }
};

static const char *productTypeNames[] = {
    "Certification",
    "Voucher",
    "PlayerInventory",
};
STATIC_ASSERT(ARRAY_SIZE(productTypeNames) == kAccountInventoryType_Count);

#if !defined(ACCOUNTSERVER)
static int                        s_CatalogFetchStartTime;
static AccountCatalogType        s_AccountCatalogState = ACCOUNT_CATALOG_UNINITIALIZED;
#endif

static U32                        s_AuthTimeout = 0;
static AccountStoreAccessInfo    s_StoreAccessInfo = { 0 };
static char s_MtxEnvironment[255] = { 0 };
static AccountCatalogList s_ProductCatalog;
                                    // A time handed us by the server.
static U32                        s_MasterTimeStampFromServer = 0;
                                    // What our clock said the time was when s_MasterTimeStampFromServer
                                    // was updated.
static U32                        s_LocalClockAtLastMasterTimeStampUpdate = 0;
static U32                        s_CatalogTimeStampTestOffsetSecs = 0;

#if defined(CLIENT) && !defined(TEST_CLIENT)
static AccountCatalogDisplay* productDisplays;
#endif

static void accountGetCatalog(void);
static void initCatalogCache(void);
static void reindexCatalog(void);
static void indexCatalogItem( const AccountProduct* pProduct, int catalogArrayIndex );
static void releaseCatalogIndex(void);
#if defined(CLIENT) || defined(SERVER)
static void catalogLazyUpdate(void);
#endif
static const AccountProduct* getCatalogProduct(SkuId sku_id);
static const AccountProduct* getCatalogProductEvenIfUnready(SkuId sku_id);

static void initCatalogCache(void)
{
    s_ProductCatalog.catalog = NULL;
    s_ProductCatalog.skuIdIndex = NULL;
    s_ProductCatalog.recipeIndex = NULL;
}

#if !defined(SERVER)
void accountCatalogInit(void)
{
#if !defined(UNITTEST)
#if defined(CLIENT)
    if (s_AccountCatalogState == ACCOUNT_CATALOG_UNINITIALIZED)
#endif
    {
        const char *bin_filename = MakeBinFilename(PRODUCT_CATALOG_DEF_FILENAME);
    
#if defined(CLIENT)
        s_AccountCatalogState = ACCOUNT_CATALOG_FETCHING;
#endif

        initCatalogCache();

        TODO(); // Move this data into shared memory
#ifdef ACCOUNTSERVER
        ParserLoadFiles(NULL, PRODUCT_CATALOG_DEF_FILENAME, bin_filename, PARSER_SERVERONLY, parse_ProductCatalog, &s_ProductCatalog, NULL, NULL, NULL);
#else
        ParserLoadFiles(NULL, PRODUCT_CATALOG_DEF_FILENAME, bin_filename, 0, parse_ProductCatalog, &s_ProductCatalog, NULL, NULL, NULL);
#endif

        accountCatalog_SetCatalogTimeStamp( timerSecondsSince2000() );    // if we're a client, the server may update this later.

        reindexCatalog();

#if defined(CLIENT)
        accountCatalogDone();
#endif
    }
#endif
}
#endif

void accountCatalog_SetMtxEnvironment(const char* mtxEnvironment)
{
    strcpy_s(s_MtxEnvironment, ARRAY_SIZE(s_MtxEnvironment), mtxEnvironment);
}

const char*    accountCatalog_GetMtxEnvironment()
{
    return s_MtxEnvironment;
}

void accountCatalog_SetCatalogTimeStamp( U32 secsSince2000 )
{
    s_MasterTimeStampFromServer                = ( secsSince2000 + accountCatalog_GetTimeStampTestOffsetSecs() );
    s_LocalClockAtLastMasterTimeStampUpdate = timerSecondsSince2000();
}

U32 accountCatalog_GetCatalogTimeStamp( void )
{
    return ( s_MasterTimeStampFromServer + ( timerSecondsSince2000() - s_LocalClockAtLastMasterTimeStampUpdate ) );
}

void accountCatalog_SetTimeStampTestOffsetSecs( U32 numSecs )
{
    s_CatalogTimeStampTestOffsetSecs = numSecs;
}

U32 accountCatalog_GetTimeStampTestOffsetSecs( void )
{
    return s_CatalogTimeStampTestOffsetSecs;
}

static void releaseCatalogIndex(void)
{
    TODO(); // Free from shared memory
    if (s_ProductCatalog.skuIdIndex)
        stashTableDestroyConst(s_ProductCatalog.skuIdIndex);
    if (s_ProductCatalog.recipeIndex)
        stashTableDestroyConst(s_ProductCatalog.recipeIndex);
}

const AccountProduct** getAccountCatalog()
{
    return s_ProductCatalog.catalog;
}

static void reindexCatalog(void)
{
    int i;

    releaseCatalogIndex();

    TODO(); // Move these indices to shared memory
    s_ProductCatalog.skuIdIndex = stashTableCreateSkuIndex(stashOptimalSize(eaSize(&s_ProductCatalog.catalog)));
    s_ProductCatalog.recipeIndex = stashTableCreateWithStringKeys(stashOptimalSize(eaSize(&s_ProductCatalog.catalog)), StashDefault);
    
    for (i=0; i < eaSize(&s_ProductCatalog.catalog); i++)
    {
        const AccountProduct * product = s_ProductCatalog.catalog[i];

        if(skuIdFromString(skuIdAsString(product->sku_id)).u64 != product->sku_id.u64)
            ErrorFilenamef(PRODUCT_CATALOG_DEF_FILENAME, "Invalid SKU '%.8s'", product->sku_id.c);

        // Key is a SkuId; value is the AccountProduct.
        stashAddPointerConst(s_ProductCatalog.skuIdIndex, product->sku_id.c, product, true);

        if (product->recipe && product->recipe[0])
        {
            // Key is a recipe string; value is the AccountProduct.
            stashAddPointerConst(s_ProductCatalog.recipeIndex, product->recipe, product, true);
        }
    }
}

#if defined(CLIENT) || defined(SERVER)
void accountCatalogRequest(void)
{
    catalogLazyUpdate();

    if (s_AccountCatalogState != ACCOUNT_CATALOG_FETCHING)
    {
        accountGetCatalog();
    }
}

#if 0 // no longer used
void accountCatalogUpdate(SkuId sku_id, U32 stateFlags)
{
    const AccountProduct* product;

    // We might be called out of order if we time out just before the
    // data arrives, so it's not an error.
    // And we need to be able to update the catalog after it
    // is initialized (for example, to do a live "un-publish").
    if ((s_AccountCatalogState == ACCOUNT_CATALOG_FETCHING) ||
        (s_AccountCatalogState == ACCOUNT_CATALOG_INITIALIZED))
    {
        product = getCatalogProductEvenIfUnready(sku_id);
        if (product)
        {
            product->productFlags = stateFlags;
        }
    }
}
#endif

#endif

void accountCatalogReportMissingSkuId(SkuId sku_id)
{
    Errorf("Catalog does not have the SKU '%.8s'", sku_id.c);
}

#if defined(FULLDEBUG) && !defined(DBSERVER) && !defined(QUEUESERVER)
void accountCatalogValidateSkuId(SkuId sku_id)
{
    if (!accountCatalogGetProduct(sku_id))
        accountCatalogReportMissingSkuId(sku_id);
}
#endif

const AccountStoreAccessInfo* accountCatalog_GetStoreAccessInfo( void )
{
    return &s_StoreAccessInfo;
}

bool accountCatalog_IsAutoBuyEnabled()
{
    return (s_StoreAccessInfo.playSpanStoreFlags & STOREFLAG_AUTO_BUY_PRODUCTS) != 0;
}

void accountCatalog_ReleaseStoreAccessInfo( void )
{
    free( (void*)s_StoreAccessInfo.playSpanCatalog );
}

#if defined(CLIENT) || defined(SERVER)
void accountCatalogDone(void)
{
    // As above, we might have timed out or whatever before the last
    // catalog item arrives (and thus the 'done' is triggered) so we
    // must handle that condition gracefully.
    if (s_AccountCatalogState == ACCOUNT_CATALOG_FETCHING)
    {
        if (eaSize(&s_ProductCatalog.catalog) > 0)
        {
            s_AccountCatalogState = ACCOUNT_CATALOG_INITIALIZED;
        }
        else
        {
            s_AccountCatalogState = ACCOUNT_CATALOG_ERROR_EMPTY;
        }
    }
}
#endif

#if defined(CLIENT) || defined(SERVER)
static void accountGetCatalog(void)
{
    s_CatalogFetchStartTime = timerSecondsSince2000();
    s_AccountCatalogState = ACCOUNT_CATALOG_FETCHING;

#if defined(CLIENT)
    if (!dbGetAccountServerCatalogAsync())
    {
        // TODO: Check the return of this and throw an error, because it'll
        // mean we tried to get the catalog while not attached to ANYTHING.
        commGetAccountServerCatalogAsync();
    }
#endif
}

static void catalogLazyUpdate(void)
{
    // Since we only ever receive data from the server, we don't need
    // a polling function - we can just update our state when someone
    // queries us or asks us for data.
    switch (s_AccountCatalogState)
    {
        case ACCOUNT_CATALOG_FETCHING:
            if (timerSecondsSince2000() - s_CatalogFetchStartTime >
                ACCOUNT_CATALOG_FETCH_TIMEOUT)
            {
                s_AccountCatalogState = ACCOUNT_CATALOG_ERROR_TIMEOUT;
                s_CatalogFetchStartTime = timerSecondsSince2000();
            }

            break;
        case ACCOUNT_CATALOG_ERROR_TIMEOUT:
        case ACCOUNT_CATALOG_ERROR_NO_SERVICE:
            if(timerSecondsSince2000() - s_CatalogFetchStartTime >
                   ACCOUNT_CATALOG_FETCH_TIMEOUT_RETRY)
            {
                accountGetCatalog();
            }
            break;
        default:
            break;
    }
}
#endif

static const AccountProduct* getCatalogProductEvenIfUnready(SkuId sku_id)
{
    const AccountProduct* product = NULL;
    if (!stashFindPointerConst(s_ProductCatalog.skuIdIndex, sku_id.c, &product)) {
        accountCatalogReportMissingSkuId(sku_id);
        return NULL;
    }
    return product;
}

static const AccountProduct* getCatalogProduct(SkuId sku_id)
{
    const AccountProduct* retval = NULL;

    // Clients only care about items that Opie tells us we can sell and they
    // need to know the price etc so they must wait for its report. Servers
    // only care about the product definitions.
#if defined(CLIENT)
    catalogLazyUpdate();

    if (s_AccountCatalogState == ACCOUNT_CATALOG_INITIALIZED)
#endif
    {
        retval = getCatalogProductEvenIfUnready(sku_id);
    }

    return retval;
}

#if defined(CLIENT) || defined(SERVER)
bool accountCatalogIsUninitialized(void)
{
    bool retval;

    catalogLazyUpdate();

    retval = false;

    if (s_AccountCatalogState == ACCOUNT_CATALOG_UNINITIALIZED)
    {
        retval = true;
    }

    return retval;
}

AccountCatalogReadiness accountCatalogIsReady(void)
{
    AccountCatalogReadiness retval;

    catalogLazyUpdate();

    retval = READINESS_NONE;

    if (s_AccountCatalogState == ACCOUNT_CATALOG_INITIALIZED)
    {
        retval = READINESS_FULL;
    }

    return retval;
}

bool accountCatalogIsEmpty(void)
{
    bool retval;

    catalogLazyUpdate();

    retval = false;

    if (s_AccountCatalogState == ACCOUNT_CATALOG_ERROR_EMPTY)
    {
        retval = true;
    }

    return retval;
}

bool accountCatalogIsOffline(void)
{
    bool retval;

    catalogLazyUpdate();

    retval = false;

    if (s_AccountCatalogState == ACCOUNT_CATALOG_ERROR_TIMEOUT ||
        s_AccountCatalogState == ACCOUNT_CATALOG_ERROR_NO_SERVICE)
    {
        retval = true;
    }

    return retval;
}
#endif

#if defined(SERVER)

static bool awardCostumeTokens(SkuId sku_id, char** costumeTokens, Entity* e)
{
    bool retval = true;
    int count;

    for (count = 0; count < eaSize(&costumeTokens); count++)
    {
        if (costumeTokens[count])
        {
            if (costume_isReward(costumeTokens[count]))
            {
                costume_Award(e, costumeTokens[count], true, 0);
            }
            else
            {
                Errorf("Unable to award costume token %s",    costumeTokens[count]);
                dbLog("AccountCatalog", e,    "Unable to award costume token %s",    costumeTokens[count]);

                retval = false;
            }
        }
        else
        {
            Errorf("NULL costume token at entry %d in list for SKU '%.8s'", count + 1, sku_id);
            dbLog("AccountCatalog", e,    "NULL costume token at entry %d in list for SKU '%.8s'",
                count + 1, sku_id);

            retval = false;
        }
    }

    return retval;
}

static bool awardBadges(SkuId sku_id, char** badges, Entity* e)
{
    bool retval = true;
    int count;

    for (count = 0; count < eaSize(&badges); count++)
    {
        if (badges[count])
        {
            if (!badge_Award(e, badges[count], true))
            {
                Errorf("Unable to award badge %s",    badges[count]);
                dbLog("AccountCatalog", e, "Unable to award badge %s", badges[count]);

                retval = false;
            }
        }
        else
        {
            Errorf("NULL badge name at entry %d in list for SKU '%.8s'", count + 1, sku_id);
            dbLog("AccountCatalog", e, "NULL badge name at entry %d in list for SKU '%.8s'", count + 1, sku_id);

            retval = false;
        }
    }

    return retval;
}

static bool checkOrAwardAuthBit(ProductAuthBit bit, Entity *e)
{
    bool retval = false;

    // In development mode authbits are per-character rather than per-account
    // so we rely on the inventory to tell us when to award global products
    // and set authbits as a consequence. In production mode we use the
    // authbits as an additional check so that we don't have to write more
    // code to add inventory to old products awarded based on authbits.
    if (isDevelopmentMode())
    {
        retval = true;

        switch (bit)
        {
            case PRODUCT_AUTHBIT_ROGUE_ACCESS:
                authUserSetRogueAccess(e->pl->auth_user_data, 1);
                break;

            case PRODUCT_AUTHBIT_ROGUE_COMPLETE:
                authUserSetRogueCompleteBox(e->pl->auth_user_data, 1);
                break;
        }
    }
    else
    {
        switch (bit)
        {
            case PRODUCT_AUTHBIT_ROGUE_ACCESS:
                retval = authUserHasRogueAccess(e->pl->auth_user_data);
                break;

            case PRODUCT_AUTHBIT_ROGUE_COMPLETE:
                retval = authUserHasRogueCompleteBox(e->pl->auth_user_data);
                break;
        }
    }

    return retval;
}

void accountCatalogServerAwardGlobalProducts(Entity *e)
{
    int count;
    const AccountProduct* product;
    bool award;

    if (e && e->pl && e->pl->account_inv_loaded)
    {
        for (count = 0; count < eaSize(&s_ProductCatalog.catalog); count++)
        {
            product = s_ProductCatalog.catalog[count];
            award = false;

            if (product && product->isGlobal)
            {
                // If the product lists an inventory item of some sort we
                // need to find it in the inventory.
                if (0) // this would need to be re-implemented somehow
                {
                    award = ( AccountInventorySet_Find( &e->pl->account_inventory, product->sku_id) != NULL );
                }

                if (award && product->authBit != PRODUCT_AUTHBIT_INVALID)
                {
                    award = checkOrAwardAuthBit(product->authBit, e);
                }

                if (award)
                {
                    //awardCostumeTokens(product->sku_id, product->costumeTokens, e);
                    //awardBadges(product->sku_id, product->badges, e);
                }
            }
        }
    }
}
#endif

bool accountCatalogIsProductAvailable(SkuId sku_id)
{
    bool retval;
    const AccountProduct* product;

    retval = false;
    product = getCatalogProductEvenIfUnready(sku_id);

    // The server doesn't send data to us for products it either doesn't
    // know about or which have been disabled, so unless it's in our list
    // and the server list, it's no good.
    if (product && accountCatalogIsProductPublished( product ))
    {
        retval = true;
    }

    return retval;
}

const char* accountCatalogGetTitle(SkuId sku_id)
{
    const char* retval;
    const AccountProduct* product;

    retval = NULL;
    product = getCatalogProduct(sku_id);

    if (product)
    {
        retval = product->title;
    }

    return retval;
}

bool accountCatalogIsSkuPublished(SkuId sku_id)
{
    return accountCatalogIsProductPublished(accountCatalogGetProduct(sku_id));
}

const char * accountCatalogGetProductTypeString(AccountInventoryType invType) {
    assert(invType >= 0 && invType < kAccountInventoryType_Count);
    return productTypeNames[invType];
}

// Every product which exists in the catalog is considered to be 'published'
// unless it is explicitly marked 'un-published' or is not within its
// active date range.
bool accountCatalogIsProductPublished(const AccountProduct* prod)
{
    if (( prod == NULL ) || ( prod->productFlags & PRODFLAG_NOT_PUBLISHED ))
    {
        return false;
    }
    else
    {
        U32 now = accountCatalog_GetCatalogTimeStamp();
        if (( prod->productStartDt > now ) ||
            (( prod->productEndDt != 0 ) && ( prod->productEndDt < now )))
        {
            return false;
        }
    }
    return true;
}

const AccountProduct* accountCatalogGetProduct(SkuId sku_id)
{
    const AccountProduct* retval = NULL;

    retval = getCatalogProduct(sku_id);

    return retval;
}

const AccountProduct* accountCatalogGetProductByRecipe(const char* recipe)
{
    const AccountProduct* product = NULL;
    if (!stashFindPointerConst(s_ProductCatalog.recipeIndex, recipe, &product)) {
        Errorf("Catalog does not have an item with the recipe '%s'", recipe);
        return NULL;
    }
    return product;
}

static const AccountProduct** productsGather(void)
{
    const AccountProduct** retval = NULL;
    int count;

#if defined(CLIENT) || defined(SERVER)
    catalogLazyUpdate();
    if (s_AccountCatalogState == ACCOUNT_CATALOG_INITIALIZED)
#endif
    {
        for (count = 0; count < eaSize(&(s_ProductCatalog.catalog)); count++)
        {
            if (!retval)
            {
                eaCreateConst(&retval);
            }

            eaPushConst(&retval, s_ProductCatalog.catalog[count]);
        }
    }

    return retval;
}

const AccountProduct** accountCatalogGetEnabledProducts()
{
    return productsGather();
}

void accountCatalog_CacheAcctServerCatalogUpdate( Packet* pak_in )
{
    //
    //    This function handles the common code's account catalog    update.
    //
    AccountStoreAccessInfo*    info = NULL;
    
    // our time stamp for product date calcs,
    accountCatalog_SetCatalogTimeStamp( pktGetBitsAuto(pak_in) );
    
    // Auth timeout (is this needed/wanted?)
    s_AuthTimeout = pktGetBitsAuto( pak_in );

    // Save the account server's name
    accountCatalog_SetMtxEnvironment( pktGetString( pak_in ) );

    accountCatalog_ReleaseStoreAccessInfo();
    info = cpp_const_cast(AccountStoreAccessInfo*)(accountCatalog_GetStoreAccessInfo());    

    // Store URL info
    info->playSpanCatalog            = strdup( pktGetString(pak_in) );
    info->playSpanStoreFlags                = pktGetBitsAuto(pak_in);
}

void accountCatalog_AddAcctServerCatalogToPacket( Packet* pak_out )
{        
    const AccountStoreAccessInfo* info = accountCatalog_GetStoreAccessInfo();
    
    pktSendBitsAuto(pak_out,accountCatalog_GetCatalogTimeStamp() );            // our time stamp for product date calcs,
    pktSendBitsAuto(pak_out,s_AuthTimeout);                                    // Auth timeout
    pktSendString(pak_out,accountCatalog_GetMtxEnvironment());
            
    pktSendString(pak_out,   info->playSpanCatalog );
    pktSendBitsAuto(pak_out, info->playSpanStoreFlags );
}

void accountCatalog_RelayServerCatalogPacket( Packet* pak_in, Packet* pak_out)
{
    //-----------------------------------------------------------------------
    //
    //
    //    See also, passSendAccountServerCatalog() in the QueueServer!!
    //
    //
    //-----------------------------------------------------------------------

    pktSendGetBitsAuto(pak_out,pak_in);        // Account Catalog time stamp
    pktSendGetBitsAuto(pak_out,pak_in);        // Auth timeout
    pktSendGetString(pak_out,pak_in);        // MTX Environment

    pktSendGetString(pak_out,pak_in);            // playSpanCatalog
    pktSendGetBitsAuto(pak_out,pak_in);            // playSpanStoreFlags
}

#ifdef SERVER
void accountCatalogGenerateServerBin(void)
{
    const char *bin_filename;

    bin_filename = MakeBinFilename(PRODUCT_CATALOG_DEF_FILENAME);

    initCatalogCache();

    ParserLoadFilesShared(MakeSharedMemoryName(bin_filename), NULL,
        PRODUCT_CATALOG_DEF_FILENAME, bin_filename, 0, parse_ProductCatalog,
        &s_ProductCatalog, sizeof(s_ProductCatalog), NULL, NULL, NULL, NULL,
        NULL);

    accountCatalog_SetCatalogTimeStamp( timerSecondsSince2000() );

    reindexCatalog();
}
#endif
