/***************************************************************************
 *     Copyright (c) 2005-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include <utilitieslib/utils/textparser.h>
#include <utilitieslib/utils/structDefines.h>
#include "bases/DetailRecipe.h"
#include "entity/character_inventory.h"
#include <utilitieslib/components/earray.h>
#include <utilitieslib/components/EString.h>
#include "entity/powers.h"
#include <utilitieslib/utils/error.h>
#include <utilitieslib/components/StashTable.h>
#include "group/group.h"
#include "bases/baseupkeep.h"
#include "entity/LoadDefCommon.h"
#include <utilitieslib/utils/eval.h>
#include "entity/character_eval.h"
#include "entity/entity.h"
#include <utilitieslib/components/SharedHeap.h>

#if SERVER
#include "cmdparse/cmdserver.h"
#else
#include "cmdparse/cmdgame.h"
#endif

#include "bases/basedata.h"
#include "bases/baseparse.h"

#define PARSE_NAME_AND_HELP(structname) \
    { "Name",               TOK_STRUCTPARAM | TOK_STRING(structname, pchName,0)              }, \
    { "DisplayName",        TOK_STRING(structname, pchDisplayName,0)       }, \
    { "DisplayHelp",        TOK_STRING(structname, pchDisplayHelp,0)       }, \
    { "DisplayShortHelp",   TOK_STRING(structname, pchDisplayShortHelp,0)  }

/***************************************************************************/
/***************************************************************************/
/***************************************************************************/

StaticDefineInt SurfaceEnum[] =
{
    DEFINE_INT
    { "Floor",        kSurface_Floor    },
    { "Wall",         kSurface_Wall     },
    { "Ceiling",      kSurface_Ceiling  },
    DEFINE_END
};

StaticDefineInt DetailFunctionEnum[] =
{
    DEFINE_INT
    { "None",                kDetailFunction_None               },
    { "Workshop",            kDetailFunction_Workshop           },
    { "Teleporter",            kDetailFunction_Teleporter         },
    { "Turret",                kDetailFunction_Turret             },
    { "Field",                kDetailFunction_Field              },
    { "Anchor",                kDetailFunction_Anchor             },
    { "Battery",            kDetailFunction_Battery            },
    { "Trap",                kDetailFunction_Trap               },
    { "RaidTeleporter",        kDetailFunction_RaidTeleporter     },
    { "ItemOfPower",        kDetailFunction_ItemOfPower        },
    { "ItemOfPowerMount",    kDetailFunction_ItemOfPowerMount   },
    { "Medivac",            kDetailFunction_Medivac            },
    { "Contact",            kDetailFunction_Contact            },    // Standalone version of "AuxContact"
    { "Store",                kDetailFunction_Store              },
    { "StorageSalvage",        kDetailFunction_StorageSalvage        },
    { "StorageInspiration",    kDetailFunction_StorageInspiration    },
    { "StorageEnhancement",    kDetailFunction_StorageEnhancement    },
    { "AuxSpot",            kDetailFunction_AuxSpot            },
    { "AuxApplyPower",        kDetailFunction_AuxApplyPower      },
    { "AuxGrantPower",        kDetailFunction_AuxGrantPower      },
    { "TPBeacon",            kDetailFunction_AuxTeleBeacon      },
    { "AuxBoostEnergy",        kDetailFunction_AuxBoostEnergy     },
    { "AuxBoostControl",    kDetailFunction_AuxBoostControl    },
    { "MedivacImprover",    kDetailFunction_AuxMedivacImprover },
    { "AuxWorkshopRepair",    kDetailFunction_AuxWorkshopRepair  },
    { "AuxContact",            kDetailFunction_AuxContact         },
    DEFINE_END
};
STATIC_ASSERT(ARRAY_SIZE(DetailFunctionEnum) == kDetailFunction_Count+2);

const char *detailfunction_ToStr(DetailFunction e)
{
    return StaticDefineIntRevLookup(DetailFunctionEnum,e);
}


static TokenizerParseInfo parse_DetailRequirement[] =
{
    { "{",        TOK_START,  0},
    { "Detail",   TOK_LINK( DetailRequirement, pDetail, 0, &g_base_detailInfoLink )}, 
    { "Amount",   TOK_INT( DetailRequirement, amount, 0 ) },
    { "}",        TOK_END,    0},
    { "", 0, 0 }
};


static TokenizerParseInfo ParseDetail[] =
{
    { "{", TOK_START, 0},

    PARSE_NAME_AND_HELP(Detail),

    { "Category",                TOK_STRING(Detail, pchCategory,0)           },
    { "DisplayTabName",          TOK_STRING(Detail, pchDisplayTabName,0)     },

    { "GroupName",               TOK_STRING(Detail, pchGroupName,0)               },
    { "GroupNameUnpowered",      TOK_STRING(Detail, pchGroupNameUnpowered,0)      },
    { "GroupNameDestroyed",      TOK_STRING(Detail, pchGroupNameDestroyed,0)      },
    { "GroupNameMount",          TOK_STRING(Detail, pchGroupNameMount,0)          },
    { "GroupNameUnpoweredMount", TOK_STRING(Detail, pchGroupNameUnpoweredMount,0) },
    { "GroupNameBaseEdit",       TOK_STRING(Detail, pchGroupNameBaseEdit,0)       },

    { "Surface",                 TOK_INT(Detail, eSurface, -1), SurfaceEnum },
    { "Replacer",                TOK_INT(Detail, iReplacer,0)             },
    { "HasVolumeTrigger",        TOK_BOOLFLAG(Detail, bHasVolumeTrigger,0)     },
    { "Mounted",                 TOK_BOOLFLAG(Detail, bMounted,0)              },
    { "CannotDelete",            TOK_BOOLFLAG(Detail, bCannotDelete,0)         },
    { "WarnDelete",              TOK_BOOLFLAG(Detail, bWarnDelete,0)             },
    { "DoNotBlock",              TOK_BOOLFLAG(Detail, bDoNotBlock,0)           },
    { "AnimatedEnt",             TOK_BOOLFLAG(Detail, bAnimatedEnt,0)          },
    { "AccessPermissions",       TOK_BOOLFLAG(Detail, bIsStorage,0)              },
    { "Requires",                TOK_STRINGARRAY( Detail, ppchRequires),       },

    { "CostPrestige",            TOK_INT(Detail, iCostPrestige,        0)},
    { "CostInfluence",           TOK_INT(Detail, iCostInfluence,       0)},
    { "UpkeepPrestige",          TOK_INT(Detail, iUpkeepPrestige,      0)},
    { "UpkeepInfluence",         TOK_INT(Detail, iUpkeepInfluence,     0)},

    { "EnergyConsume",           TOK_INT(Detail, iEnergyConsume,       0)},
    { "EnergyProduce",           TOK_INT(Detail, iEnergyProduce,       0)},
    { "ControlConsume",          TOK_INT(Detail, iControlConsume,      0)},
    { "ControlProduce",          TOK_INT(Detail, iControlProduce,      0)},

    { "MustBeReachable",         TOK_BOOL(Detail, bMustBeReachable,     0)},
    { "Obsolete",                TOK_BOOL(Detail, bObsolete,            0)},

    { "Lifetime",                TOK_INT(Detail, iLifetime,0)             },
    { "RepairChance",            TOK_F32(Detail, fRepairChance,0)         },

    { "EntityDef",               TOK_STRING(Detail, pchEntityDef,0)          },
    { "Level",                   TOK_INT(Detail, iLevel,0)                },
    { "Behavior",                TOK_STRING(Detail, pchBehavior,0)           },
    { "DecayTo",                 TOK_STRING(Detail, pchDecaysTo,0)           },
    { "DecayedLife",             TOK_INT(Detail, iDecayLife,0)                },

    { "AuxAllowed",              TOK_STRINGARRAY(Detail, ppchAuxAllowed)     },
    { "MaxAuxAllowed",           TOK_INT(Detail, iMaxAuxAllowed,0)     },

    { "Function",                TOK_INT(Detail, eFunction,          0), DetailFunctionEnum },
    { "FunctionParams",          TOK_STRINGARRAY(Detail, ppchFunctionParams)  },

    //{ "Power",                   TOK_LINK,        offsetof( Detail, pPower ),              (int)&g_basepower_InfoLink },
    { "Power",                   TOK_STRING( Detail, pchPower,0 )},

    { "DisplayFloatDestroyed",   TOK_STRING(Detail, pchDisplayFloatDestroyed,0)},
    { "DisplayFloatUnpowered",   TOK_STRING(Detail, pchDisplayFloatUnpowered,0)},
    { "DisplayFloatUncontrolled",TOK_STRING(Detail, pchDisplayFloatUncontrolled,0)},

    { "}",                       TOK_END,      0 },
    { "", 0, 0 }
};

TokenizerParseInfo ParseDetailDict[] =
{
    { "Item",            TOK_REDUNDANTNAME|TOK_STRUCT(DetailDict, ppDetails, ParseDetail) },
    { "Detail",          TOK_STRUCT(DetailDict, ppDetails, ParseDetail) },
    { "", 0, 0 }
};

SHARED_MEMORY DetailDict g_DetailDict;

STATIC_ASSERT(IS_GENERICINVENTORYTYPE_COMPATIBLE(Detail,id,pchName));
STATIC_ASSERT(IS_GENERICINVENTORYDICT_COMPATIBLE(DetailDict,ppDetails,haItemNames,itemsById))

static TokenizerParseInfo ParseDetailCategory[] =
{
    { "{",              TOK_START,                        0},

    PARSE_NAME_AND_HELP(DetailCategory),

    { "EnergyPriority",  TOK_INT(DetailCategory, iEnergyPriority,0)  },
    { "ControlPriority", TOK_INT(DetailCategory, iControlPriority,0) },

    // Used for shared memory only
    { "*DetailPointer",  TOK_AUTOINTEARRAY(DetailCategory, ppDetails)        },

    { "}",               TOK_END,      0 },
    { "", 0, 0 }
};


TokenizerParseInfo ParseDetailCategoryDict[] =
{
    { "DetailCategory", TOK_STRUCT(DetailCategoryDict, ppCats, ParseDetailCategory) },
    { "", 0, 0 }
};

SHARED_MEMORY DetailCategoryDict g_DetailCategoryDict;

/***************************************************************************/
/***************************************************************************/
/***************************************************************************/

extern StaticDefineInt BoolEnum[];


static TokenizerParseInfo ParseRoomCategory[] =
{
    { "{", TOK_START, 0},
    PARSE_NAME_AND_HELP(RoomCategory),
    { "}", TOK_END,   0},
    { "", 0, 0 }
};

TokenizerParseInfo ParseRoomCategoryDict[] =
{
    { "RoomCategory", TOK_STRUCT(RoomCategoryDict, ppRoomCats, ParseRoomCategory) },
    { "", 0, 0 }
};

SHARED_MEMORY RoomCategoryDict g_RoomCategoryDict;


/***************************************************************************/
/***************************************************************************/
/***************************************************************************/

TokenizerParseInfo ParseDetailCategoryLimit[] =
{
    { "{",          TOK_START,                        0,                                          },
    { "Category",   TOK_STRUCTPARAM | TOK_STRING(DetailCategoryLimit, pchCatName,0),  },
    { "Limit",      TOK_INT(DetailCategoryLimit, iLimit,0 ),     },
    { "}",          TOK_END,                          0,                                          },
    { "", 0, 0 }
};

static TokenizerParseInfo ParseRoomTemplate[] =
{
    { "{",              TOK_START,                        0},

    PARSE_NAME_AND_HELP(RoomTemplate),

    { "Length",         TOK_INT(RoomTemplate, iLength,0),            },
    { "Width",          TOK_INT(RoomTemplate, iWidth,0),             },
    { "AllowTeleport",  TOK_BOOL(RoomTemplate, bAllowTeleport,0),     },
    { "CostPrestige",   TOK_INT(RoomTemplate, iCostPrestige,      0)},
    { "CostInfluence",  TOK_INT(RoomTemplate, iCostInfluence,     0)},
    { "UpkeepPrestige", TOK_INT(RoomTemplate, iUpkeepPrestige,    0)},
    { "UpkeepInfluence",TOK_INT(RoomTemplate, iUpkeepInfluence,   0)},
    { "Room",           TOK_STRING(RoomTemplate, pchRoomCat,0),         },
    { "DetailAllowed",  TOK_STRUCT(RoomTemplate, ppDetailCatLimits, ParseDetailCategoryLimit)},
    { "Requires",       TOK_STRINGARRAY(RoomTemplate, ppchRequires),       },
    { "}",              TOK_END,      0 },
    { "", 0, 0 }
};

TokenizerParseInfo ParseRoomTemplateDict[] =
{
    { "RoomTemplate",    TOK_STRUCT(RoomTemplateDict, ppRooms,  ParseRoomTemplate) },
    { "", 0, 0 }
};

SHARED_MEMORY RoomTemplateDict g_RoomTemplateDict;

/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
static TokenizerParseInfo ParseBasePlot[] =
{
    { "{",              TOK_START,                        0},

    PARSE_NAME_AND_HELP(BasePlot),

    { "DisplayTabName", TOK_STRING(BasePlot, pchDisplayTabName,0) },
    { "GroupName",      TOK_STRING(BasePlot, pchGroupName,0)      },
    { "Length",         TOK_INT(BasePlot, iLength,0),          },
    { "Width",          TOK_INT(BasePlot, iWidth,0),           },
    { "MaxRaidParty",   TOK_INT(BasePlot, iMaxRaidParty,0),    },
    { "CostPrestige",   TOK_INT(BasePlot, iCostPrestige,    0)},
    { "CostInfluence",  TOK_INT(BasePlot, iCostInfluence,   0)},
    { "UpkeepPrestige", TOK_INT(BasePlot, iUpkeepPrestige,  0)},
    { "UpkeepInfluence",TOK_INT(BasePlot, iUpkeepInfluence, 0)},
    { "DetailAllowed",  TOK_STRUCT(BasePlot, ppDetailCatLimits, ParseDetailCategoryLimit) },
    { "Requires",       TOK_STRINGARRAY(BasePlot, ppchRequires),     },
    { "}",              TOK_END,         0 },
    { "", 0, 0 }
};

TokenizerParseInfo ParseBasePlotDict[] = 
{
    { "Plot",            TOK_STRUCT(BasePlotDict, ppPlots,  ParseBasePlot) },
    { "", 0, 0 }
};

SHARED_MEMORY BasePlotDict g_BasePlotDict;

/***************************************************************************/
/***************************************************************************/
/***************************************************************************/



/**********************************************************************func*
 * basedate_findRoomCategory
 *
 */
static const RoomCategory * basedate_findRoomCategory( char * pchName )
{
    int i;
    for( i = eaSize(&g_RoomCategoryDict.ppRoomCats)-1; i>=0; i-- )
    {
        const RoomCategory * rc = g_RoomCategoryDict.ppRoomCats[i];
        if( stricmp( rc->pchName, pchName ) == 0 )
            return rc;
    }

    return NULL;
}

/**********************************************************************func*
 * RoomDictFinalize
 *
 */
static bool RoomTemplateDictFinalize(TokenizerParseInfo pti[], RoomTemplateDict *pRoomTemplateDict)
{
    int i;
    int iNumRooms = eaSize(&pRoomTemplateDict->ppRooms);
    for(i=0; i<iNumRooms; i++)
    {
        int j, iNumDetailCatLimits;

        RoomTemplate *pRoom = (RoomTemplate*)pRoomTemplateDict->ppRooms[i];
        RoomCategory *pRoomCat = (RoomCategory*)basedate_findRoomCategory( pRoom->pchRoomCat );

        if( pRoomCat )
            pRoom->pRoomCat = pRoomCat;
        else
            ErrorFilenamef( "rooms.template", "BASE DATA:  Room %s, RoomCategory %s not found", pRoom->pchName, pRoom->pchRoomCat );

        iNumDetailCatLimits = eaSize(&pRoom->ppDetailCatLimits);
        for(j=0; j<iNumDetailCatLimits; j++)
        {
            DetailCategoryLimit *pDetailCatLimit = pRoom->ppDetailCatLimits[j];
            const DetailCategory *pDetailCat = basedata_GetDetailCategoryByName(pDetailCatLimit->pchCatName);

            if( pDetailCat )
                pDetailCatLimit->pCat = pDetailCat;
            else
                ErrorFilenamef( "rooms.template", "BASE DATA:  Room %s, DetailAllowed %s not found", pRoom->pchName, pDetailCatLimit->pchCatName );
        }
    }

    return 1;
}

/**********************************************************************func*
 * DictVerify
 *
 */
static int DetailDictVerify(TokenizerParseInfo pti[], DetailDict *pDetailDict)
{
    int i;
    int iRet = 1;
    int iNumDetails = eaSize(&g_DetailDict.ppDetails);

    for(i=0; i<iNumDetails; i++)
    {
        const Detail *pDetail = g_DetailDict.ppDetails[i];

        if(pDetail->pchGroupName && !groupDefFindWithLoad(pDetail->pchGroupName))
        {
            ErrorFilenamef(pDetail->pchGroupName, "Unable to find required geometry: %s", pDetail->pchGroupName);
            iRet = 0;
        }

        if(!pDetail->bMounted && pDetail->pchEntityDef)
        {
            if(pDetail->pchGroupNameDestroyed
                && stricmp(pDetail->pchGroupNameDestroyed, "dim")!=0
                && !groupDefFindWithLoad(pDetail->pchGroupNameDestroyed))
            {
                ErrorFilenamef(pDetail->pchGroupNameDestroyed, "Unable to find required geometry: %s", pDetail->pchGroupNameDestroyed);
                iRet = 0;
            }
        }

        if(pDetail->iEnergyConsume>0 || pDetail->iControlConsume>0)
        {
            if(pDetail->pchGroupNameUnpowered 
                && stricmp(pDetail->pchGroupNameUnpowered, "dim")!=0
                && !groupDefFindWithLoad(pDetail->pchGroupNameUnpowered))
            {
                ErrorFilenamef(pDetail->pchGroupNameUnpowered, "Unable to find required geometry: %s (%d)", pDetail->pchGroupNameUnpowered, pDetail->iEnergyConsume);
                iRet = 0;
            }
        }

        if(pDetail->bMounted)
        {
            if(pDetail->pchGroupNameMount && !groupDefFindWithLoad(pDetail->pchGroupNameMount))
            {
                ErrorFilenamef(pDetail->pchGroupNameMount, "Unable to find required geometry: %s", pDetail->pchGroupNameMount);
                iRet = 0;
            }

            if(pDetail->pchGroupNameUnpoweredMount 
                && stricmp(pDetail->pchGroupNameUnpowered, "dim")!=0
                && !groupDefFindWithLoad(pDetail->pchGroupNameUnpoweredMount))
            {
                ErrorFilenamef(pDetail->pchGroupNameUnpoweredMount, "Unable to find required geometry: %s", pDetail->pchGroupNameUnpoweredMount);
                iRet = 0;
            }
        }
    }

    return 1;
}
#if CLIENT
static TokenizerParseInfo ParseDestroyUnusedDetail[] =
{
    { "EntityDef",               TOK_STRING(Detail, pchEntityDef,0)          },
    { "Behavior",                TOK_STRING(Detail, pchBehavior,0)           },
    //{ "Power",                   TOK_LINK,        offsetof( Detail, pPower ),              (int)&g_basepower_InfoLink },
    { "Power",                   TOK_STRING( Detail, pchPower,0 )},
    { 0 }
};
#endif

/**********************************************************************func*
 * DetailDictPreprocess
 *
 */
static bool DetailDictPreprocess(TokenizerParseInfo pti[], DetailDict *pDetailDict)
{
    int idx = 0;
    int j;
    int iNumDetails = eaSize(&pDetailDict->ppDetails);

    for(j=0; j<iNumDetails; j++)
    {
        Detail *pDetail = (Detail*)pDetailDict->ppDetails[j];
        if(pDetail->eSurface == -1)
        {
            int l = (int)strlen(pDetail->pchGroupName);

            if(l>2 && pDetail->pchGroupName[l-2]=='_')
            {
                switch(pDetail->pchGroupName[l-1])
                {
                    case 'W':
                    case 'w':
                        pDetail->eSurface = kSurface_Wall;
                        break;

                    case 'C':
                    case 'c':
                        pDetail->eSurface = kSurface_Ceiling;
                        break;

                    default:
                        pDetail->eSurface = kSurface_Floor;
                        break;
                }
            }
            else
            {
                pDetail->eSurface = kSurface_Floor;
            }
        }

        if(pDetail->pchGroupName 
            && pDetail->pchGroupName[0]!='*' // A leading * means to use the exact name given
            && (strEndsWith(pDetail->pchGroupName,"_f")
                || strEndsWith(pDetail->pchGroupName,"_w")
                || strEndsWith(pDetail->pchGroupName,"_c")))
        {
            // The _N_P of the basic object was not included, so we try to build all this stuff automatically
            char achBuffer[1024];

            if(!pDetail->pchGroupNameUnpowered)
            {
                strcpy(achBuffer, pDetail->pchGroupName);
                strcat(achBuffer, pDetail->bAnimatedEnt ? "_N_P" : "_N_U");
                pDetail->pchGroupNameUnpowered = ParserAllocString(achBuffer);
            }

            if(!pDetail->pchGroupNameDestroyed)
            {
                strcpy(achBuffer, pDetail->pchGroupName);
                if (pDetail->bMounted) strcat(achBuffer, "_MNT");
                strcat(achBuffer, "_X_X");
                pDetail->pchGroupNameDestroyed = ParserAllocString(achBuffer);
            }

            if(!pDetail->pchGroupNameMount)
            {
                strcpy(achBuffer, pDetail->pchGroupName);
                strcat(achBuffer, "_MNT_N_P");
                pDetail->pchGroupNameMount = ParserAllocString(achBuffer);
            }

            if(!pDetail->pchGroupNameUnpoweredMount)
            {
                strcpy(achBuffer, pDetail->pchGroupName);
                strcat(achBuffer, "_MNT_N_U");
                pDetail->pchGroupNameUnpoweredMount = ParserAllocString(achBuffer);
            }

            // Fix up the root name last
            strcpy(achBuffer, pDetail->pchGroupName);
            strcat(achBuffer, "_N_P");
            StructFreeStringConst(pDetail->pchGroupName);
            pDetail->pchGroupName = ParserAllocString(achBuffer);
        }
        else if(pDetail->pchGroupName && pDetail->pchGroupName[0]=='*')
        {
            // Strip off the leading *
            strcpy((char*)&pDetail->pchGroupName[0], &pDetail->pchGroupName[1]);
        }


        if(!pDetail->pchBehavior)
        {
            pDetail->pchBehavior = ParserAllocString("DoNothing");
        }

        if( !pDetail->pchDisplayTabName )
            pDetail->pchDisplayTabName = ParserAllocString(pDetail->pchCategory);

        if (pDetail->pchEntityDef)
            pDetail->bMustBeReachable = 1;
    }

#if CLIENT 
    // Prune things that shouldn't exist on the client
    // @todo SHAREDMEM look into ramifications when memory is shared between client and local servers?
    for(j=0; j<iNumDetails; j++)
    {
        Detail *pDetail = cpp_const_cast(Detail*)(pDetailDict->ppDetails[j]);
        ParserDestroyStruct(ParseDestroyUnusedDetail,pDetail);
    }

#endif

//    return DetailDictVerify(pti, pDetailDict);
    return 1;
}

/**********************************************************************func*
 * DetailDictPostprocess
 *
 */
static bool DetailDictPostprocess(TokenizerParseInfo pti[], DetailDict *pDetailDict)
{
    int j;
    int idx = 0;
    int iNumDetails = eaSize(&pDetailDict->ppDetails);

    for(j=0; j<iNumDetails; j++)
    {
        Detail * pDetail = (Detail*)pDetailDict->ppDetails[j];

        pDetail->uniqueId = idx++;

        if(pDetail->eFunction == kDetailFunction_ItemOfPower
            || pDetail->eFunction == kDetailFunction_Medivac
            || pDetail->eFunction == kDetailFunction_AuxTeleBeacon)
        {
            pDetail->bSpecial = true;
        }
    }

    return 1;
}

/**********************************************************************func*
 * DetailDictFinalize
 *
 */

static void addToTabLists( DetailDict * detailDict, const Detail * detail )
{
    StashElement elem;
    Detail **list;
    Detail **new_list = NULL;

    if( !detail->pchDisplayTabName )
        return;

    if (!stashFindElementConst(detailDict->tabLists, detail->pchDisplayTabName, &elem))
    {
        // Create a new entry
        bool bStashed;

        eaCreate(&new_list);
        bStashed = stashAddPointerAndGetElementConst(detailDict->tabLists, detail->pchDisplayTabName, new_list, false, &elem);
        assert(bStashed);
    }

    // Add to the existing entry
    assert(elem);
    list = (Detail**)stashElementGetPointer(elem);
    eaPushConst(&list, detail);
    stashElementSetPointer(elem, list);    // push can realloc and relocate list
}

/**********************************************************************func*
 * baseCreateTabLists
 *
 */
static bool baseCreateTabLists(DetailDict * detailDict, bool shared_memory)
{
    bool ret = true;
    int i;
    int iNumDetails = eaSize(&detailDict->ppDetails);

    // build out the stash and the tab lists completely in private memory
    // (shared stash tables can only be used with shared heap memory addresses so we can't
    // just build the tab lists privately)
    assert(!detailDict->tabLists);
    detailDict->tabLists = stashTableCreateWithStringKeys(stashOptimalSize(iNumDetails), stashShared(false));

    for(i=0; i<iNumDetails; i++)
    {
        addToTabLists( detailDict, detailDict->ppDetails[i] );
    }

    // if we are using shared memory we need to move the finalized tab lists
    // over to the shared heap and replace the stash table
    if (shared_memory)
    {
        cStashTable    dict_private = detailDict->tabLists;
        cStashTableIterator iTabList = {0};
        StashElement elem;
        int i;

        // stash will resize if it gets 75% full so need to set intial size so that doesn't happen in shared memory
        detailDict->tabLists = stashTableCreateWithStringKeys( stashOptimalSize(stashGetOccupiedSlots(dict_private)), stashShared(shared_memory));

        for( i = 0, stashGetIteratorConst( dict_private, &iTabList); stashGetNextElementConst(&iTabList, &elem); ++i ) 
        {
            Detail **list_private = (Detail**)stashElementGetPointer( elem );
            Detail **list_shared;

            eaCompressConst(&list_shared, &list_private, customSharedMalloc, NULL);
            stashAddPointerConst(detailDict->tabLists, stashElementGetKey(elem), list_shared, false );
            eaDestroy(&list_private);
        }
        stashTableDestroyConst(dict_private);
    }

    return ret;
}

/**********************************************************************func*
 * baseTabList_get
 *
 */
const Detail ** baseTabList_get(const char * pchName)
{
    const Detail ** detail;

    if (!stashFindPointerConst(g_DetailDict.tabLists, pchName, cpp_reinterpret_cast(const Detail**)(&detail)))
        return NULL;

    return detail;
}

/**********************************************************************func*
 * DetailCategoryDictPostprocess
 *
 */
static bool DetailCategoryDictPostprocess(TokenizerParseInfo pti[], DetailCategoryDict *pDetailDict)
{
    int i,idx=0;
    int iNumDetails = eaSize(&g_DetailDict.ppDetails);

    for(i=0; i<iNumDetails; i++)
    {
        const Detail *pDetail = g_DetailDict.ppDetails[i];
        const DetailCategory *pCat = basedata_GetDetailCategoryByNameEx(pDetailDict, pDetail->pchCategory);
        if(pCat)
        {
            eaPushConst(cpp_const_cast(cccEArrayHandle*)(&pCat->ppDetails), pDetail);
        }
    }

    return 1;
}

/**********************************************************************func*
 * PlotDictFinalize
 *
 */
static bool PlotDictFinalize(TokenizerParseInfo pti[], BasePlotDict *pPlotDict)
{
    int i;

    for( i = eaSize(&pPlotDict->ppPlots)-1; i >= 0; i-- )
    {
        int j,iNumDetailCatLimits;
        const BasePlot * plot = pPlotDict->ppPlots[i];

        if( (plot->iLength%4) != 0 || (plot->iWidth%4) != 0 )
        {
            Errorf( "Base Plot (%s) did not have dimensions divisible by 4 (%i x %i)", plot->pchName, plot->iLength, plot->iWidth );
            eaRemoveConst(&pPlotDict->ppPlots, i);
        }

        iNumDetailCatLimits = eaSize(&plot->ppDetailCatLimits);
        for(j=0; j<iNumDetailCatLimits; j++)
        {
            DetailCategoryLimit *pDetailCatLimit = plot->ppDetailCatLimits[j];
            const DetailCategory *pDetailCat = basedata_GetDetailCategoryByName(pDetailCatLimit->pchCatName);

            if( pDetailCat )
                pDetailCatLimit->pCat = pDetailCat;
            else
                ErrorFilenamef( "base.plot", "BASE PLOT:  %s, DetailAllowed %s not found", plot->pchName, pDetailCatLimit->pchCatName );
        }
    }

    return true;
}




/**********************************************************************func*
 * basedata_GetDetailCategoryByNameEx
 *
 */
const DetailCategory *basedata_GetDetailCategoryByNameEx(const DetailCategoryDict *pDict, const char *pch)
{
    if(pDict && pch && *pch)
    {
        int i;
        int n = eaSize(&pDict->ppCats);

        for(i=0; i<n; i++)
        {
            if(pDict->ppCats[i]->pchName && stricmp(pch, pDict->ppCats[i]->pchName)==0)
            {
                return pDict->ppCats[i];
            }
        }
    }

    return NULL;
}

/**********************************************************************func*
 * basedata_GetDetailCategoryByName
 *
 */
const DetailCategory *basedata_GetDetailCategoryByName(const char *pch)
{
    return basedata_GetDetailCategoryByNameEx(&g_DetailCategoryDict, pch);
}

/**********************************************************************func*
 * basedata_GetRoomTemplateByName
 *
 */
const RoomTemplate *basedata_GetRoomTemplateByName(const char *pch)
{
    return (const RoomTemplate *)ParserLinkFromString(&g_base_roomInfoLink, pch);
}

/**********************************************************************func*
 * basedata_GetBasePlotByName
 *
 */
const BasePlot *basedata_GetBasePlotByName(const char * pch)
{
    return (const BasePlot *)ParserLinkFromString(&g_base_BasePlotLink, pch);
}

/**********************************************************************func*
 * basedata_GetDetailByName
 *
 */
const Detail *basedata_GetDetailByName(const char *pch)
{
    return (const Detail *)ParserLinkFromString(&g_base_detailInfoLink, pch);
}

static bool basedetail_FinalProcess(ParseTable pti[], DetailDict * ddict, bool shared_memory)
{
    basedetail_CreateDetailInvHashes(ddict, shared_memory);

    if( !baseCreateTabLists(ddict, shared_memory) )
        return false;

    return true;
}

/**********************************************************************func*
 * basedata_Load
 *
 */
void basedata_Load(void)
{
#if SERVER 
    int flags = PARSER_SERVERONLY;
#else
    int flags = 0;
#endif
    // If we have Tok_Links to things that are split between server and client, we also
    // need to make seperate shared memory pools for server and client
    ParserLoadFilesShared(MakeSharedMemoryName("SM_Details"), "defs/v_bases", ".details", "details.bin",
        flags, ParseDetailDict, &g_DetailDict, sizeof(DetailDict),
        NULL, NULL, DetailDictPreprocess, DetailDictPostprocess, basedetail_FinalProcess);

    ParserLoadFilesShared(MakeSharedMemoryName("SM_DetailCategories"), "defs/v_bases", ".cats", "detailcats.bin",
        flags, ParseDetailCategoryDict, &g_DetailCategoryDict, sizeof(DetailCategoryDict),
        NULL, NULL, NULL, DetailCategoryDictPostprocess, NULL);

    ParserLoadFilesShared(MakeSharedMemoryName("SM_RoomCategories"), "defs/v_bases", ".category", "roomcategories.bin",
        flags, ParseRoomCategoryDict, &g_RoomCategoryDict, sizeof(RoomCategoryDict),
        NULL, NULL, NULL, NULL, NULL);

    ParserLoadFilesShared(MakeSharedMemoryName("SM_RoomTemplates"), "defs/v_bases", ".template", "roomtemplates.bin",
        flags, ParseRoomTemplateDict, &g_RoomTemplateDict, sizeof(RoomTemplateDict),
        NULL, NULL, NULL, RoomTemplateDictFinalize, NULL);

    ParserLoadFilesShared(MakeSharedMemoryName("SM_Plots"), "defs/v_bases", ".plot", "plots.bin",
        flags, ParseBasePlotDict, &g_BasePlotDict, sizeof(BasePlotDict),
        NULL, NULL, NULL, PlotDictFinalize, NULL);

    baseupkeep_Load();
}

/**********************************************************************func*
 * detailGetThumbnailName
 *
 */
const char *detailGetThumbnailName(const Detail *detail)
{
    static char *pchNameList = NULL;
    if(verify(detail))
    {
        if (detail->bMounted && detail->pchGroupNameMount)
        {
            if (!pchNameList)
                estrCreate(&pchNameList);
            estrPrintf(&pchNameList, "%s,%s", detail->pchGroupName, detail->pchGroupNameMount);
            return pchNameList;
        }
        else
        {
            return detail->pchGroupName;
        }
    }
    else
    {
        return NULL;
    }
}

const char *detail_ReverseLookupFunction(DetailFunction function) {
    return StaticDefineIntRevLookup(DetailFunctionEnum, function);
}


// *********************************************************************************
// eval section 
// *********************************************************************************

//----------------------------------------
//  eval the requires statement.
// This a game-side requires and so doesn't
// have access to much except the badges earned.
//----------------------------------------
static bool requirementsMet( const char** ppchReq, Entity * e )
{
    static EvalContext *eval = NULL;
    bool res = false;

    if( !eval )
    {
        eval = eval_Create();
        chareval_AddDefaultFuncs(eval);
    }

    // ----------
    // eval

    if( ppchReq && e )
    {
        eval_StoreInt(eval, "Entity", (int)(intptr_t)e);
        if(e->supergroup)
            eval_StoreInt(eval, "Supergroup", (int)(intptr_t)e->supergroup);
        eval_ClearStack(eval);

        res = eval_Evaluate(eval, ppchReq);
    }

    // ----------
    // finally

    return res;
}

bool baseDetailMeetsRequires(const Detail * detail, Entity *e)
{
    if( !detail || !detail->ppchRequires )
        return false;

    return requirementsMet( detail->ppchRequires, e );
}

bool baseRoomMeetsRequires(const RoomTemplate *room, Entity *e)
{
     if( !room )
        return false;

    if( !room->ppchRequires )
        return true;

    return requirementsMet( room->ppchRequires, e );
}

bool basePlotMeetsRequires(const BasePlot * plot, Entity *e)
{
    if( !plot )
        return false;

    if( !plot->ppchRequires )
        return true;

    return requirementsMet( plot->ppchRequires, e );
}

/* End of File */

