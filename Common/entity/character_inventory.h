/***************************************************************************
 *     Copyright (c) 2005-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#ifndef CHARACTER_INVENTORY_H
#define CHARACTER_INVENTORY_H

#include <utilitieslib/utils/textparser_types.h>
#include <utilitieslib/stdtypes.h>
#include <utilitieslib/assert/assert.h>

// forward decls
typedef struct StashTableImp *StashTable;
typedef const struct StashTableImp *cStashTable;
typedef struct SalvageItem SalvageItem;
typedef struct ConceptItem ConceptItem;
typedef struct Character Character;
typedef struct BasePower BasePower;
typedef struct PowerDictionary PowerDictionary;
typedef struct DetailDict DetailDict;
typedef struct DetailRecipe DetailRecipe;
typedef struct Detail Detail;
typedef struct DetailCategoryDict DetailCategoryDict;

//------------------------------------------------------------
//  The types of inventory.
// do not reorder, if you want to add an enum see:
//      - character_GetInvInfo: (0 for id if NA)
//    - character_GetInvHandleByType: (NULL if NA)
//    - character_AddInventory: (assert if NA, should not happen)
//    - character_SetInventory: (assert if NA, should not happen)
//    - GetItemsByIdArrayRef: (NULL if NA)
//      - s_GetInvAndItem: 
//      - inventory_GetDbTableName
//    - also handle loading the hashes:
//       * basedetail_CreateDetailInvHashes
//----------------------------------------------------------
typedef enum InventoryType
{ 
     kInventoryType_Salvage,
    kInventoryType_Concept,
     kInventoryType_Recipe,
    kInventoryType_BaseDetail,
    kInventoryType_StoredSalvage, // personal invention storage
    // # of items
    kInventoryType_Count,
} InventoryType;

#define CONCEPT_NUM_FIELDS 5
#define DB_MAX_FIELDS 1024
#define CONCEPT_MAX_PER_ROW ((DB_MAX_FIELDS)/(CONCEPT_NUM_FIELDS))
#define CONCEPT_MAX_ROWS 20

bool inventorytype_Valid(InventoryType t);
char *inventorytype_StrFromType(InventoryType t);

// ------------------------------------------------------------
// inventory types

typedef enum InventoryId
{
    kInventoryId_Invalid = 0,
} InventoryId;


//------------------------------------------------------------
//  Base for inventory sizes based on level.
//----------------------------------------------------------

typedef struct InventorySizeDefinition
{
    const int *piAmountAtLevel;
    const int *piAmountAtLevelFree;
} InventorySizeDefinition;

typedef struct InventorySizes 
{
    InventorySizeDefinition        salvageSizes;
    InventorySizeDefinition        recipeSizes;
    InventorySizeDefinition        auctionSizes;
    InventorySizeDefinition        storedSalvageSizes;
} InventorySizes;

typedef struct InventoryLoyaltyBonusSizeDefinition
{
    const char *name;
    int salvageBonus;
    int recipeBonus;
    int vaultBonus;
    int auctionBonus;
} InventoryLoyaltyBonusSizeDefinition;

typedef struct InventoryLoyaltyBonusSizes 
{
    const InventoryLoyaltyBonusSizeDefinition **bonusList;
    cStashTable st_LoyaltyBonus;
} InventoryLoyaltyBonusSizes;

extern SHARED_MEMORY InventorySizes g_InventorySizeDefs;
extern SHARED_MEMORY InventoryLoyaltyBonusSizes g_InventoryLoyaltyBonusSizeDefs;
extern TokenizerParseInfo ParseInventoryDefinitions[];
extern TokenizerParseInfo ParseInventoryLoyaltyBonusDefinitions[];

//------------------------------------------------------------
//  Base for inventory types.
//----------------------------------------------------------
typedef struct GenericInventoryType
{
    int id;
    char const *name;
} GenericInventoryType;

#define IS_GENERICINVENTORYTYPE_COMPATIBLE(typeName, tid, tname) \
   (OFFSETOF(typeName,tid) == OFFSETOF(GenericInventoryType,id) \
   && OFFSETOF(typeName,tname) == OFFSETOF(GenericInventoryType,name))

//------------------------------------------------------------
//  helper, represents all inventory items
//----------------------------------------------------------
typedef struct GenericInvItem
{
    void const *item;
    U32 amount;
} GenericInvItem; 

// helper for checking struct compatibility with GenericInvItem, 1 if compatible
#define GENERICINVITEM_COMPATIBLE(typeName, itemMemberName, amountMemberName ) ((OFFSETOF(typeName,itemMemberName) == OFFSETOF(GenericInvItem,item)) && (OFFSETOF(typeName, amountMemberName) == OFFSETOF(GenericInvItem, amount)) && sizeof(typeName) == sizeof(GenericInvItem))


//------------------------------------------------------------
//  Represents an type of salvage in the users inventory and how
//many of that type the user has.
//  salvage : the type of salvage
//  amount: the number of items in inventory
//  -AB: created :2005 Feb 24 11:09 AM
//----------------------------------------------------------
typedef struct SalvageInventoryItem
{
    SalvageItem const *salvage;
    U32 amount;
} SalvageInventoryItem;
STATIC_ASSERT(GENERICINVITEM_COMPATIBLE( SalvageInventoryItem, salvage, amount ));

//------------------------------------------------------------
//  A representation of a concept in inventory
//----------------------------------------------------------
typedef struct ConceptInventoryItem
{
    ConceptItem *concept;
    U32 amount;
} ConceptInventoryItem;
STATIC_ASSERT(GENERICINVITEM_COMPATIBLE( ConceptInventoryItem, concept, amount ));


//------------------------------------------------------------
//  Since recipes are actually base powers, this struct provides
// a map from one to another. the name member == recipe->pchName.
// needed to maintain unique ids across data changes.
//----------------------------------------------------------
typedef struct RecipeItem
{ 
    int id;
    const char *name;
    const BasePower *recipe;
} RecipeItem;
STATIC_ASSERT(IS_GENERICINVENTORYTYPE_COMPATIBLE(RecipeItem,id,name));

//------------------------------------------------------------
//  a recipe in a character's inventory
//----------------------------------------------------------
typedef struct RecipeInventoryItem
{ 
    DetailRecipe *recipe;
    U32 amount;
} RecipeInventoryItem;
STATIC_ASSERT(GENERICINVITEM_COMPATIBLE( RecipeInventoryItem, recipe, amount ));

//------------------------------------------------------------
//  a recipe in a character's inventory
//----------------------------------------------------------
typedef struct DetailInventoryItem
{ 
    const Detail *item;
    U32 amount;
} DetailInventoryItem;
STATIC_ASSERT(GENERICINVITEM_COMPATIBLE( DetailInventoryItem, item, amount ));

DetailInventoryItem *detailinventoryitem_Create();

//------------------------------------------------------------
//  generic dictionary for tracking inventory
//----------------------------------------------------------
typedef struct GenericInvDictionary
{
    const GenericInventoryType* const* ppItems;
    cStashTable haItemNames;
    const GenericInventoryType* const* itemsById;
} GenericInvDictionary;
#define IS_GENERICINVENTORYDICT_COMPATIBLE(typeName, tppItems, thaItemNames, titemsById) \
    (OFFSETOF(typeName,tppItems) == OFFSETOF(GenericInvDictionary,ppItems)) \
    && (OFFSETOF(typeName,thaItemNames) == OFFSETOF(GenericInvDictionary,haItemNames)) \
    && (OFFSETOF(typeName,titemsById) == OFFSETOF(GenericInvDictionary,itemsById))

// -------------------
// loading

void powerdictionary_CreateRecipeHashes(PowerDictionary *pdict, bool shared_memory);
void basedetail_CreateDetailInvHashes(DetailDict * ddict, bool shared_memory);
//--------------------
// generic inventory functions


// only add on server
int genericinv_GetInvIdFromItem(InventoryType type, void *ptr);
int character_AdjustInventoryEx(Character *p, InventoryType type, int id, int amount, const char *context, int bNoInvWarning, int autoOpenSalvage);
int character_AdjustInventory(Character *p, InventoryType type, int id, int amount, const char *context, int bNoInvWarning);
void character_SetInventory(Character *p, InventoryType type, int id, int idx, int amount, char const *context);
void character_ClearInventory(Character *p, InventoryType type, char const *context);

//------------------------------------------------------------
// These both provide information about a type, and an index. they are:
//   - used to automatically send/receive packets on update
//   - used to package the inventory for sending/receiving from db
//   - character_SendInventories: when sending dirty data
//----------------------------------------------------------
void character_GetInvInfo(Character *p, int *id, int *amount, const char **name, InventoryType type, int idx);

// The number of different unique types of the specified inventory
int character_GetInvSize(Character *p, InventoryType type);

// The maximum inventory size of the specified inventory
int character_GetInvTotalSize(Character *p, InventoryType type);
int character_GetAuctionInvTotalSize(Character *p);
int character_GetBoostInvTotalSize(Character *p);

#if SERVER
int character_GetInvTotalSizeByLevel(int level, InventoryType type);
int character_GetAuctionInvTotalSizeByLevel(int level);
#endif

// The total number of items in the specified inventory
int character_CountInventory(Character *p, InventoryType type);

#if SERVER
void character_UpdateInvSizes(Character *p);
void character_CopyInventory(Character *pcharOrig, Character *pcharNew);
#endif

bool character_InventoryFull(Character *pchar, InventoryType type);
int character_InventoryEmptySlots(Character *pchar, InventoryType type);
int character_InventoryEmptySlotsById(Character *p, InventoryType type, int id);
int character_InventoryUsedSlots(Character *pchar, InventoryType type);
bool character_CanAddInventory(Character *p, InventoryType type, int id, int amount);

int character_GetItemAmount(Character *p, InventoryType type, int id);

void character_InventoryFree(Character *pchar);
void character_SetInvTotalSize(Character *p, InventoryType type, int size);

// --------------------
// db

typedef struct AttribFileDict AttribFileDict;

void genericinvdict_CreateHashes(GenericInvDictionary *pdict, AttribFileDict const *dict, bool shared_memory);
char *inventorytype_GetIdmapFilename( InventoryType type );

char const* inventory_GetDbTableName( InventoryType type );

#ifdef SERVER
void inventory_WriteDbIds();
AttribFileDict inventory_GetMergedAttribs(InventoryType type);
void genericinvtype_WriteDbIds(const GenericInventoryType* const* types, AttribFileDict const *attribs, char const*fname);
AttribFileDict genericinvtype_MergeAttribFileDict(const GenericInventoryType* const* types,AttribFileDict const*attribs);
void attribfiledict_Cleanup( AttribFileDict *dict );
#endif


//--------------------
// salvage

bool salvageinventoryitem_Valid(SalvageInventoryItem *inv);
bool salvageitem_IsInvention(SalvageItem const *item);

// Make absolute sure we can do the exact adjustment
bool character_GetAdjustSalvageCap(Character *p, SalvageItem const *pSalvageItem, int amount );
bool character_CanAdjustSalvage(Character *p, SalvageItem const *pSalvageItem, int amount );

// Allow actions that will be partially successful
bool character_CanSalvageBeChanged(Character *p, SalvageItem const *pSalvageItem, int amount );

bool character_SalvageIsFull(Character *p, SalvageItem const *pSalvageItem );
int character_SalvageEmptySlots(Character *p, SalvageItem const *pSalvageItem );
int character_SalvageCount(Character *p, SalvageItem const *pSalvageItem );

int character_AdjustSalvageEx(Character *p, SalvageItem const *pSalvageItem, int amount, const char *context, int bNoInvWarning, int autoOpenSalvage );
int character_AdjustSalvage(Character *p, SalvageItem const *pSalvageItem, int amount, const char *context, int bNoInvWarning );
int character_FindSalvageInvIdx(Character *c, const SalvageInventoryItem *sal);
SalvageInventoryItem *character_FindSalvageInv(Character *c, const char *name);

void character_ReverseEngineerSalvage(Character *pchar, int idx);
void character_SetSalvageInvCurrentCount(Character *p);

// stored salvage

int character_AdjustStoredSalvage(Character *p, SalvageItem const *pSalvageItem, int amount, const char *context, int bNoInvWarning );
int character_StoredSalvageEmptySlots(Character *p, SalvageItem const *pSalvageItem );
bool character_CanAdjustStoredSalvage(Character *p, SalvageItem const *pSalvageItem, int amount );
SalvageInventoryItem *character_FindStoredSalvageInv(Character *c, char *name);
int character_GetAdjustStoredSalvageCap(Character *p, SalvageItem const *pSalvageItem, int amount );
void character_SetStoredSalvageInvCurrentCount(Character *p);

int character_StoredSalvageCount(Character *p, SalvageItem const *pSalvageItem);

//--------------------
// concepts

bool conceptinventoryitem_Valid(ConceptInventoryItem *inv);

int character_AdjustConcept(Character *p, int defId, F32 *afVars, int amount);

void character_SetConcept( Character *p, int defId, F32 *afVars, int idx, int amount );
void character_AddRollConcept( Character *p, int defId, int amount );
U32 character_RemoveConcept(Character *p, int invIdx, int amount);


// --------------------
// recipes
// note: these are packed in columns now. its important
// to maintain position in the earray when loading/saving.


bool recipeinventoryitem_Valid(RecipeInventoryItem *inv);
const DetailRecipe *recipe_GetItem(const char *name);
const DetailRecipe *recipe_GetItemById( int id );
int recipe_GetMaxId();
bool basepower_IsRecipe(BasePower const *p) ;

bool character_CanRecipeBeChanged(Character *p, DetailRecipe const *pRecipe, int amount );
int character_AdjustRecipe(Character *p, DetailRecipe const *item, int amount, const char *context);
int character_RecipeCount(Character *p, DetailRecipe const *pRecipe );
void character_SetRecipeInvCurrentCount(Character *p);

// returns whether any owned recipes can use the given salvage, pushes recipes on to useful_to
bool character_IsSalvageUseful(Character *p, SalvageItem const *pSalvageItem, const DetailRecipe ***useful_to);


// --------------------
// details

const Detail *detail_GetItem( char const *name );
 
int character_AddBaseDetail(Character *p,char const *detailName, const char *context);
    //simple helper function. use adjustInventory for anything more complicated

// ------------------------------------------------------------
// attribfile 

typedef struct AttribFileItem
{
    int id;
    const char *name;
} AttribFileItem;

typedef struct AttribFileDict
{ 
    const AttribFileItem **ppAttribItems;
    cStashTable idHash;
    cStashTable nameFromId;
    int maxId;
} AttribFileDict;

TokenizerParseInfo *attribfiledict_GetParseInfo();
AttribFileDict const* inventorytype_GetAttribs( InventoryType type );
void load_InventoryAttribFiles( bool bNewAttribs );
bool attribfiledict_FinalProcess(ParseTable pti[], AttribFileDict *dict, bool shared_memory);

#endif //CHARACTER_INVENTORY_H
