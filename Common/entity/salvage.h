/***************************************************************************
 *     Copyright (c) 2005-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#ifndef SALVAGE_H__
#define SALVAGE_H__ 

#include <utilitieslib/utils/structDefines.h>
#include <utilitieslib/stdtypes.h>
#include "TokenizerUiWidget.h"

typedef struct StructLink StructLink;
#define ParseLink StructLink
typedef struct Detail Detail;
typedef const struct StashTableImp*    cStashTable;

//------------------------------------------------------------
// the rarity of the salvage item
//
// if you change this:
// see salvage.c(24):StaticDefineInt RarityEnum[] 
// see menuMessages.ms which relies on the strings here
// -AB: created :2005 Feb 15 12:08 PM
//----------------------------------------------------------

typedef enum SalvageRarity
{
    kSalvageRarity_Ubiquitous,
    kSalvageRarity_Common,
    kSalvageRarity_Uncommon,
    kSalvageRarity_Rare,
    kSalvageRarity_VeryRare,
    kSalvageRarity_Unique,
    kSalvageRarity_Count,

    // helper enums
    kSalvageRarity_Invalid = kSalvageRarity_Count
} SalvageRarity;

extern StaticDefineInt RarityEnum[];

typedef enum SalvageType
{
    kSalvageType_Base,                    // used to build base items
    kSalvageType_Invention,                // used to build IOs
    kSalvageType_Token,                    // used by NPCs/Events to grant players bonuses/special rewards
    kSalvageType_Incarnate,                // used to build Incarnate Abilities
    kSalvageType_Count,

    // helper enums
    kSalvageType_Invalid = kSalvageType_Count
} SalvageType;

typedef enum SalvageImmediateUseStatus {
    kSalvageImmediateUseFlag_OK             = 0,
    kSalvageImmediateUseFlag_NotApplicable  = ( 1 << 0 ),
    kSalvageImmediateUseFlag_MissingPrereqs = ( 1 << 1 ),
} SalvageImmediateUseStatus;

char const* salvage_RarityToStr( SalvageRarity r );
int salvage_ValidRarity( int rarity );

//------------------------------------------------------------
//  helper for any salvage id specific enums
//----------------------------------------------------------
typedef enum SalvageId
{
    kSalvageId_Invalid = 0,
} SalvageIndex;

//------------------------------------------------------------
// flags
//------------------------------------------------------------
#define    SALVAGE_NOTRADE            (1 << 0)    // this salvage cannot be traded in person or email or base storage
#define    SALVAGE_NODELETE        (1 << 1)    // this salvage cannot be deleted
#define SALVAGE_IMMEDIATE       (1 << 2)    // this salvage can be used from the salvage tray
#define SALVAGE_AUTO_OPEN        (1 << 3)    // this salvage is automatically opened as soon as its awarded
                                            // doesn't work with SALVAGE_IMMEDIATE
#define SALVAGE_NOAUCTION        (1 << 4)    // this salvage cannot be traded on the AH

//------------------------------------------------------------
// Info about a salvage item.
//----------------------------------------------------------
typedef struct SalvageItem
{
    U32 salId;                                // the id of the item by rarity (for the db)
    const char *pchName;                    // internal name
    TokenizerUiWidget ui;

    const char *pchDisplayTabName;          // designer-specified replacement for rarity
    const char *pchDisplayDropMsg;            // override for floater message when found
    SalvageType    type;                        // what kind of salvage is this?    
    SalvageRarity rarity;

    U32 maxInvAmount;                        // the max number in inventory, 0 -> unlimited
    U32 sellAmount;                            // the amount this salvage will sell for

    int challenge_points;                    // how hard it is to reverse engineer
    U32 minRevEngLevel;                        // the minimum level to reverse engineer

    const char **ppchRewardTables;            // table when opened
    const char **ppchOpenRequires;            // requirement to open
    const char *pchDisplayOpenRequiresFail;    // message displayed if open requires fails.

    const char **ppchAuctionRequires;        // The requirements expression to determine if this salvage can be listed in the AuctionHouse. 
                                            // If empty, there are no requirements needed to be listed in the AuctionHouse.

    const Detail **ppWorkshops;                // Workshops where this salvage can be used. This is informational, not a restriction.

    int    flags;                                // cannot trade or place on CH/BM

    const char *pchStoreProduct;            // store product linked to this salvage

} SalvageItem;

// forward decl
struct StashTableImp;
typedef struct StashTableImp *StashTable;
typedef const struct StashTableImp *cStashTable;

//------------------------------------------------------------
// the salvage container
// -AB: created :2005 Feb 15 05:45 PM
//----------------------------------------------------------
typedef struct SalvageDictionary
{
    // Defines a set of related categories. (Examples include character and
    // villain)
    const SalvageItem **ppSalvageItems;
    cStashTable haItemNames;
    const SalvageItem **itemsById;

    cStashTable itemsFromTabName; // SalvageItem**
} SalvageDictionary;

typedef struct SalvageTrackedByEnt
{
    char *salvageName;
    const SalvageItem *item;
    char *displayName;
}SalvageTrackedByEnt;

typedef struct SalvageTrackedByEntList
{
    SalvageTrackedByEnt **ppTrackedSalvage;
}SalvageTrackedByEntList;
// global inst of dict
extern SHARED_MEMORY SalvageDictionary g_SalvageDict;
extern SERVER_SHARED_MEMORY SalvageTrackedByEntList g_SalvageTrackedList;
extern ParseLink g_salvageInfoLink;

// forward decl
typedef struct ParseTable ParseTable;
#define TokenizerParseInfo ParseTable

TokenizerParseInfo* salvageTrackedByEntParseInfo();
TokenizerParseInfo* salvage_GetParseInfo();

bool salvage_FinalProcess(TokenizerParseInfo *pti, SalvageDictionary *pdict, bool shared_memory);

const SalvageItem* salvage_GetItem( char const *name );
int salvage_ValidId( int id );
const SalvageItem* salvage_GetItemById( int id );

#endif //SALVAGE_H__

/* End of File */

