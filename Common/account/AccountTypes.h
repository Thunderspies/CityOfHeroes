#ifndef ACCOUNTTYPES_H
#define ACCOUNTTYPES_H

#include <utilitieslib/utils/structDefines.h>

C_DECLARATIONS_BEGIN

/// Default size for account related containers
#define ACCOUNT_INITIAL_CONTAINER_SIZE 65536

/// Number of serialized loyalty bits
#define LOYALTY_BITS 128

/// Number of serialized @ref OrderId bits
#define ORDER_ID_BITS 128

/// Number of characters to store an @ref OrderId string
#define ORDER_ID_STRING_LENGTH 37

/// Maximum number of subtransactions allowed in a multi-game transaction
#define MAX_MULTI_GAME_TRANSACTIONS 8

// TODO:  Integrate this with the RarityEnums in salvage and recipes and wherever else?
extern StaticDefineInt AccountTypes_RarityEnum[];

typedef struct StashTableImp *StashTable;

/**
* Union for accessing the 64bit SKU value the optimized 64bit unsigned
* integer representation of a SKU.
*
* @note Sometimes referred to as the "Product Code"
*/
typedef union SkuId {
    U64 u64;
    U32 u32[2];
    char c[8]; // adding room for a null terminator would dealign the type
} SkuId;

extern const SkuId kSkuIdInvalid;

/**
* Union for accessing the 128bit order UUID.
* @note This is stored in Microsoft's convention where the lower 16 bytes
*       are little-endian and the upper 16 bytes are big-endian.
*/
typedef union OrderId {
    U64 u64[2];
    U32 u32[4];
    U16 u16[8];
    char c[16]; // adding room for a null terminator would dealign the type
} OrderId;

extern const OrderId kOrderIdInvalid;

/// @{
#define SKU(_sku_string) skuIdFromString(_sku_string)
SkuId skuIdFromString(const char * sku_string);
char * skuIdAsString(SkuId sku_id);
int skuIdCmp(SkuId a, SkuId b);
bool skuIdEquals(SkuId a, SkuId b);
bool skuIdIsNull(SkuId sku_id);
/// @}

/// @{
StashTable stashTableCreateSkuIndexEx(U32 uiInitialSize, const char* pcFile, U32 uiLine);
#define stashTableCreateSkuIndex(uiInitialSize) stashTableCreateSkuIndexEx(uiInitialSize, __FILE__, __LINE__ )
/// @}

/// @{
OrderId orderIdFromString(const char * order_string);
char * orderIdAsString(OrderId order_id);
int orderIdCmp(OrderId a, OrderId b);
bool orderIdEquals(OrderId a, OrderId b);
bool orderIdIsNull(OrderId order_id);
/// @}

C_DECLARATIONS_END

#endif
