/***************************************************************************
 *     Copyright (c) 2005-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include "bases/DetailRecipe.h"
#include <utilitieslib/utils/textparser.h>
#include <utilitieslib/utils/structDefines.h>
#include "entity/entity.h"
#include "entity/entPlayer.h"
#include <utilitieslib/utils/eval.h>
#include "entity/salvage.h"
#include "entity/powers.h"
#include "bases/basedata.h"
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/components/MemoryPool.h>
#include "bases/baseparse.h"
#include "entity/Supergroup.h"
#include "entity/salvage.h"
#include "entity/character_base.h"
#include "entity/character_eval.h"
#include "entity/character_inventory.h"
#include "entity/LoadDefCommon.h"
#include <utilitieslib/utils/error.h>
#include "bases/bases.h"
#include <utilitieslib/components/StashTable.h>
#include "entity/RewardToken.h"
#include "entity/Supergroup.h"
#include "gameComm/trayCommon.h"
#include <utilitieslib/language/MessageStoreUtil.h>
#include <utilitieslib/components/StringCache.h>
#include <utilitieslib/components/SharedHeap.h>

#if SERVER
#include "cmdparse/cmdserver.h"
#include "Reward.h"
#include "dbcomm/dbcomm.h"
#include "entity/entGameActions.h"
#include "storyarc/task.h"
#include "player/badges_server.h"
#include "gameSys/raidmapserver.h"
#include "container/team.h"
#include "entity/SgrpServer.h"
#include "gameData/mininghelper.h"
#include "storyarc/playerCreatedStoryarcServer.h"
#include "entity/incarnate_server.h"
#include "entity/PowerInfo.h"
#include "dbcomm/logcomm.h"
#else
#include "cmdparse/cmdgame.h"
#include "UI/uiAuction.h"
#include "entity/PowerInfo.h"
#include "UI/uiChat.h"
#endif //SERVER

#if CLIENT && !TEST_CLIENT
#include "UI/uiStore.h"
#include "gameData/store.h"
#endif

// keep the DetailRecipe in sync
STATIC_ASSERT(IS_GENERICINVENTORYTYPE_COMPATIBLE(DetailRecipe, id, pchName));

static void recipeeval_Init(void);

StaticDefineInt RecipeRarityParseEnum[] =
{
    DEFINE_INT
    { "Common",                kRecipeRarity_Common    },
    { "Uncommon",            kRecipeRarity_Uncommon    },
    { "Rare",                kRecipeRarity_Rare        },
    { "VeryRare",            kRecipeRarity_VeryRare    },
    DEFINE_END
};

StaticDefineInt RecipeTypeParseEnum[] =
{
    DEFINE_INT
    { "Workshop",            kRecipeType_Workshop    },
    { "Drop",                kRecipeType_Drop        },
    { "Memorized",            kRecipeType_Memorized    },
    DEFINE_END
};


static TokenizerParseInfo parse_salvagerequired[] =
{
    { "Amount",   TOK_STRUCTPARAM | TOK_INT( SalvageRequired, amount, 0 ),   },
    { "Salvage",  TOK_STRUCTPARAM | TOK_LINK( SalvageRequired, pSalvage, 0, &g_salvageInfoLink) },
    { "\n",       TOK_END,   0},
    { "", 0, 0 }
};

static TokenizerParseInfo parse_powerrequired[] =
{
    { "Amount",   TOK_STRUCTPARAM | TOK_INT( PowerRequired, amount, 0 ),   },
    { "Salvage",  TOK_STRUCTPARAM | TOK_LINK( PowerRequired, pPower, 0, &g_basepower_InfoLink) },
    { "\n",       TOK_END,   0},
    { "", 0, 0 }
};

StaticDefineInt ParseDetailRecipeFlags[] =
{
    DEFINE_INT
    { "NoGenericBadgeCredit", RECIPE_NO_GENERIC_BADGE_CREDIT    },
    { "NoLevelDisplayed", RECIPE_NO_LEVEL_DISPLAYED    },
    { "NoTrade", RECIPE_NO_TRADE },
    { "Certification", RECIPE_CERTIFICATON},
    { "Voucher", RECIPE_VOUCHER },
    { "LicRequired", RECIPE_INVENTION_LIC_REQUIRED },
    { "NoRefund", RECIPE_NO_REFUND },
    { "FixedLevel", RECIPE_FIXED_LEVEL },
    { "ForceClaimDelay", RECIPE_FORCE_CLAIM_DELAY },
    DEFINE_END
};

static TokenizerParseInfo parse_detailrecipe[] =
{
    { "{",                            TOK_START, 0},
    { "SourceFile",                    TOK_CURRENTFILE                        ( DetailRecipe, pchSourceFile)        },
    { "Name",                        TOK_STRUCTPARAM | TOK_STRING        ( DetailRecipe, pchName, 0 ),           },
    TOKENIZERUIWIDGET_INLINEPARSEINFO(DetailRecipe),
    { "DisplayTabName",                TOK_STRING( DetailRecipe, pchDisplayTabName, 0 ), },
    { "DetailIcon",                    TOK_LINK( DetailRecipe, pDetailIcon, 0, &g_base_detailInfoLink )},
    { "Workshop",                    TOK_LINKARRAY( DetailRecipe, ppWorkshops, &g_base_detailInfoLink )},
    { "Salvage",                    TOK_STRUCT( DetailRecipe, ppSalvagesRequired, parse_salvagerequired ) },
    { "SalvageComponent",            TOK_REDUNDANTNAME | TOK_STRUCT( DetailRecipe, ppSalvagesRequired, parse_salvagerequired ) },
    { "PowerComponent",                TOK_STRUCT( DetailRecipe, ppPowersRequired, parse_powerrequired ) },
    { "AdditionalComponents",        TOK_STRINGARRAY( DetailRecipe, pchAdditionalComponents ),   },
    { "Detail",                        TOK_LINK( DetailRecipe, pDetailReward, 0, &g_base_detailInfoLink )},
    { "DetailReward",                TOK_REDUNDANTNAME | TOK_LINK( DetailRecipe, pDetailReward, 0, &g_base_detailInfoLink )},
    { "Reward",                        TOK_STRINGARRAY( DetailRecipe, pchInventReward),   },
    { "TableReward",                TOK_REDUNDANTNAME | TOK_STRINGARRAY( DetailRecipe, pchInventReward),   },
    { "Enhancement",                TOK_STRING( DetailRecipe, pchEnhancementReward, 0 ),   },
    { "EnhancementReward",            TOK_REDUNDANTNAME | TOK_STRING( DetailRecipe, pchEnhancementReward, 0 ),   },
    { "Recipe",                        TOK_STRING( DetailRecipe, pchRecipeReward, 0 ),   },
    { "RecipeReward",                TOK_REDUNDANTNAME | TOK_STRING( DetailRecipe, pchRecipeReward, 0 ),   },
    { "PowerReward",                TOK_STRING( DetailRecipe, pchPowerReward, 0 ),    },
    { "IncarnateReward",            TOK_STRING( DetailRecipe, pchIncarnateReward, 0 ),    },
    { "Requires",                    TOK_STRINGARRAY( DetailRecipe, ppchVisibleRequires) },
    { "Rarity",                        TOK_INT(DetailRecipe, Rarity, kRecipeRarity_Common), RecipeRarityParseEnum },
    { "Level",                        TOK_INT(DetailRecipe, level,            0)},
    { "LevelMin",                    TOK_INT(DetailRecipe, levelMin,            0)},
    { "LevelMax",                    TOK_INT(DetailRecipe, levelMax,            0)},
    { "CreationCost",                TOK_STRINGARRAY(DetailRecipe, CreationCost)},
    { "SellToVendor",                TOK_INT(DetailRecipe, SellToVendor,    0)},
    { "BuyFromVendor",                TOK_INT(DetailRecipe, BuyFromVendor,    0)},
    { "NumUses",                    TOK_INT(DetailRecipe, NumUses,        1)},
    { "Type",                        TOK_INT(DetailRecipe, Type,        kRecipeType_Workshop), RecipeTypeParseEnum },
    { "MaxInvAmount",                TOK_INT(DetailRecipe, maxInvAmount,    99)},

    { "CreatesEnhancement",            TOK_INT(DetailRecipe, creates_enhancement,    0)},
    { "CreatesInspiration",            TOK_INT(DetailRecipe, creates_inspiration,    0)},
    { "CreatesSalvage",                TOK_INT(DetailRecipe, creates_salvage,    0)},
    { "CreatesRecipe",                TOK_INT(DetailRecipe, creates_recipe,    0)},

    { "CreatesRequires",            TOK_STRINGARRAY( DetailRecipe, ppchCreateRequires)                },
    { "ReceiveRequires",            TOK_STRINGARRAY( DetailRecipe, ppchReceiveRequires)                },
    { "NeverReceiveRequires",        TOK_STRINGARRAY( DetailRecipe, ppchNeverReceiveRequires)        },
    { "AuctionRequires",            TOK_STRINGARRAY( DetailRecipe, ppchAuctionRequires)        },
    { "DisplayCreatesRequiresFail",    TOK_STRING( DetailRecipe, pchDisplayCreateRequiresFail, 0 ),    },
    { "DisplayReceiveRequiresFail",    TOK_STRING( DetailRecipe, pchDisplayReceiveRequiresFail, 0 ),   },
    { "DisplayReceive",                TOK_STRING( DetailRecipe, pchDisplayReceive, 0 ),                },
    { "DisplayAccountItemPurchase",    TOK_STRING( DetailRecipe, pchDisplayAccountItemPurchase, 0 ),   },
    { "DisplayClaimConfirmation",    TOK_STRING( DetailRecipe, pchDisplayClaimConfirmation, 0 ),        },
    
    { "Flags",                        TOK_FLAGS(DetailRecipe, flags, 0), ParseDetailRecipeFlags        },

    { "}",              TOK_END,   0},
    { "", 0, 0 }
};

TokenizerParseInfo ParseDetailRecipeDict[] =
{
    {"DetailRecipe", TOK_STRUCT( DetailRecipeDict, ppRecipes, parse_detailrecipe ) },
    { "", 0, 0 }
};

SHARED_MEMORY DetailRecipeDict g_DetailRecipeDict = {0};

STATIC_ASSERT(IS_GENERICINVENTORYTYPE_COMPATIBLE(DetailRecipe,id,pchName));
STATIC_ASSERT(IS_GENERICINVENTORYDICT_COMPATIBLE(DetailRecipeDict,ppRecipes,haItemNames,itemsById))


ParseLink g_DetailRecipe_Link = {
    (void***)(&g_DetailRecipeDict.ppRecipes),
    {
        { offsetof(DetailRecipe, pchName),      0 },
    }
};

static EvalContext *s_pRecipeEval;

//------------------------------------------------------------
//
//----------------------------------------------------------
static bool RecipeDetailDictPreprocess(ParseTable pti[], DetailRecipeDict *pRecipeDict)
{
    bool ret = true;

    // Validate Requires statements inside recipes
    int i;
    for (i = eaSize(&pRecipeDict->ppRecipes) - 1; i >= 0; i--) {
        const DetailRecipe *recipe = pRecipeDict->ppRecipes[i];
        if(recipe->CreationCost)
        {
            chareval_Validate(recipe->CreationCost,recipe->pchSourceFile);
        }
        if(recipe->ppchVisibleRequires)
        {
            chareval_Validate(recipe->ppchVisibleRequires,recipe->pchSourceFile);
        }
        if(recipe->ppchCreateRequires)
        {
            chareval_Validate(recipe->ppchCreateRequires,recipe->pchSourceFile);
        }
        if(recipe->ppchReceiveRequires)
        {
            chareval_Validate(recipe->ppchReceiveRequires,recipe->pchSourceFile);
        }
        if(recipe->ppchNeverReceiveRequires)
        {
            chareval_Validate(recipe->ppchNeverReceiveRequires,recipe->pchSourceFile);
        }
        if(recipe->ppchAuctionRequires)
        {
            chareval_Validate(recipe->ppchAuctionRequires,recipe->pchSourceFile);
        }
    }

#if CLIENT
    {
        int i, p;
        // Remove things from the client bins
        // @todo SHAREDMEM this probably will cause problems if we want
        // a client and local servers to share memory?
        for (i = eaSize(&pRecipeDict->ppRecipes) - 1; i >= 0; i--) {
            DetailRecipe *recipe = cpp_const_cast(DetailRecipe*)(pRecipeDict->ppRecipes[i]);
            if (recipe->pchInventReward)
            {
                for (p = 0; p < eaSize(&recipe->pchInventReward); p++)
                    StructFreeStringConst(recipe->pchInventReward[p]);
                eaDestroyConst(&recipe->pchInventReward);
            }
            recipe->pchInventReward = NULL;
        }
    }
#endif

    return ret;

}

//------------------------------------------------------------
//  finalize: fixes up any ids in the recipes that are new
// so that they are unique (also facilitates dbid file creation)
//----------------------------------------------------------
/*
static int RecipeDetailDictFinalize(TokenizerParseInfo pti[], DetailRecipeDict *pRecipeDict)
{
    bool res = false;

    // --------------------
    // nothing

    if( pRecipeDict )
    {
        int i;
        int idCur = inventorytype_GetAttribs(kInventoryType_Recipe)->maxId + 1;
        int size = eaSize(&pRecipeDict->ppRecipes);

        recipeeval_Init();
        res = true;

        for( i = 0; i < size; ++i)
        {
            DetailRecipe *r = pRecipeDict->ppRecipes[i];
            
            if( !verify(r) )
            {
                res = false;
                continue;
            }

            // --------------------
            // set the unique ids

            if( !stashFindInt( inventorytype_GetAttribs(kInventoryType_Recipe)->idHash, r->pchName, &r->id ))
            {
                r->id = idCur++;
            }

            // ----------
            // set the icon if not specified

            if (r->pDetail && !r->pDetailIcon)
                r->pDetailIcon = r->pDetail;

            // ----------
            // check the evals

            if( r->ppchRequires )
            {
                char *error = NULL;
                res = eval_Validate(s_pRecipeEval,r->pchName,r->ppchRequires, &error);
                if (!res)
                {
                    ErrorFilenamef(r->pchSourceFile,error);
                }
            }
        }
    }

    // --------------------
    // finally

    return res;

}
*/

/**
* @note If any bug fixes are made here also fix @ref salvage_CreateTabNameHash.
*/
static bool detailrecipe_CreateTabNameHash(DetailRecipeDict *pdict, bool shared_memory)
{
    bool ret = true;
    int i;

    StashTableIterator iter;
    StashElement elem;

    StashTable tempItemsFromTabName = stashTableCreateWithStringKeys(eaSize(&pdict->ppRecipes), StashDefault);

    // hash lists into a temporary table
    for (i = 0; i < eaSize(&pdict->ppRecipes); i++)
    {
        DetailRecipe *recipe = cpp_const_cast(DetailRecipe*)(pdict->ppRecipes[i]);
        DetailRecipe **ppRec = NULL;

        if (!stashFindElement(tempItemsFromTabName, recipe->pchDisplayTabName, &elem))
        {
            eaCreateWithCapacity(&ppRec, 16);
            stashAddPointerAndGetElement(tempItemsFromTabName, recipe->pchDisplayTabName, ppRec, false, &elem);
        }

        ppRec = (DetailRecipe**)stashElementGetPointer(elem);
        eaPush(&ppRec, recipe);
        stashElementSetPointer(elem, ppRec);
    }

    assert(!pdict->itemsFromTabName);
    pdict->itemsFromTabName = stashTableCreateWithStringKeys(stashOptimalSize(stashGetValidElementCount(tempItemsFromTabName)), stashShared(shared_memory));

    // build the final table from the temporary one
    stashGetIterator(tempItemsFromTabName, &iter);    
    while (stashGetNextElement(&iter, &elem))
    {
        DetailRecipe **tempRec = (DetailRecipe**)stashElementGetPointer(elem);
        DetailRecipe **sharedRec = NULL;
        
        if (shared_memory)
        {
            eaCompressConst(&sharedRec, &tempRec, customSharedMalloc, NULL);
            eaDestroy(&tempRec);
        }
        else
            sharedRec = tempRec;
    
        stashAddPointerConst(pdict->itemsFromTabName, stashElementGetStringKey(elem), sharedRec, false);
    }

    stashTableDestroy(tempItemsFromTabName);

    return ret;
}

static bool detailrecipe_CreateCategoryHash(DetailRecipeDict *pdict, bool shared_memory)
{
    bool ret = true;
    int i;

    StashTable tempCategoryNames = stashTableCreateWithStringKeys(eaSize(&pdict->ppRecipes), StashDefault);
    StashTable tempCategoryMinLevel = stashTableCreateWithStringKeys(eaSize(&pdict->ppRecipes), StashDefault);
    StashTable tempCategoryMaxLevel = stashTableCreateWithStringKeys(eaSize(&pdict->ppRecipes), StashDefault);

    // hash lists into a temporary table
    for (i = 0; i < eaSize(&pdict->ppRecipes); i++)
    {
        DetailRecipe *recipe = cpp_const_cast(DetailRecipe*)(pdict->ppRecipes[i]);

        char cat_name[256];
        char * level_str;
        char * level_str_end = NULL;
        int level;

        StashElement elem;

        if (!recipe->level)
            continue;

        // copy
        Strcpy(cat_name, recipe->pchName);

        // skip objects who do not have level
        level_str = strrchr(cat_name, '_');
        if (!level_str)
            continue;
        level = strtod(level_str+1, &level_str_end);
        if (!level_str_end || *level_str_end)
            continue;
        if (level != recipe->level)
        {
            if(level != 0) {
                // per Tim don't display a warning for recipes that end in "_0": These recipes all have constant results regardless 
                //    of level, but probably required a level supplied anyway (probably for display purposes).  We probably can’t 
                //    change either half of that data at this point without breaking something.
                Errorf("recipe level %d does not match the name %s", recipe->level, recipe->pchName);
            }
            ret = false;
        }

        // strip the level off the short name
        *level_str = 0;
        assert(strlen(cat_name) > 0);

        // see if it already exists
        if (!stashFindElement(tempCategoryNames, cat_name, &elem))
        {
            const char * shared_cat_name = shared_memory ? allocAddSharedString(cat_name) : allocAddString(cat_name);

            stashAddPointer(tempCategoryNames, shared_cat_name, recipe, false);
            stashAddInt(tempCategoryMinLevel, shared_cat_name, recipe->level, false);
            stashAddInt(tempCategoryMaxLevel, shared_cat_name, recipe->level, false);
        }
        else
        {
            int min, max;

            const char * shared_cat_name = stashElementGetStringKey(elem);

            // check min level
            if (stashFindInt(tempCategoryMinLevel, shared_cat_name, &min) && min > recipe->level)
            {
                stashAddInt(tempCategoryMinLevel, shared_cat_name, recipe->level, true);
            }

            // check max level
            if (stashFindInt(tempCategoryMaxLevel, shared_cat_name, &max) && max < recipe->level)
            {
                stashAddInt(tempCategoryMaxLevel, shared_cat_name, recipe->level, true);
            }
        }
    }

    assert(!pdict->haCategoryNames);
    pdict->haCategoryNames = stashTableCreateWithStringKeys(stashOptimalSize(stashGetValidElementCount(tempCategoryNames)), stashShared(shared_memory));
    stashTableMergeConst(pdict->haCategoryNames, tempCategoryNames, false);
    stashTableDestroy(tempCategoryNames);

    assert(!pdict->haCategoryMinLevel);
    pdict->haCategoryMinLevel = stashTableCreateWithStringKeys(stashOptimalSize(stashGetValidElementCount(tempCategoryMinLevel)), stashShared(shared_memory));
    stashTableMergeConst(pdict->haCategoryMinLevel, tempCategoryMinLevel, true);
    stashTableDestroy(tempCategoryMinLevel);

    assert(!pdict->haCategoryMaxLevel);
    pdict->haCategoryMaxLevel = stashTableCreateWithStringKeys(stashOptimalSize(stashGetValidElementCount(tempCategoryMaxLevel)), stashShared(shared_memory));
    stashTableMergeConst(pdict->haCategoryMaxLevel, tempCategoryMaxLevel, true);
    stashTableDestroy(tempCategoryMaxLevel);

    return ret;
}

static bool detailrecipe_CreateEnhancementHash(DetailRecipeDict *pdict, bool shared_memory)
{
    bool ret = true;
    int i;

    StashTable tempItemsFromEnhancementName = stashTableCreateWithStringKeys(eaSize(&pdict->ppRecipes), StashDefault);

    // hash lists into a temporary table
    for (i = 0; i < eaSize(&pdict->ppRecipes); i++)
    {
        DetailRecipe *recipe = cpp_const_cast(DetailRecipe*)(pdict->ppRecipes[i]);

        char buf[1024];
        const char * shared_name;

        if (!recipe->pchEnhancementReward)
            continue;

        sprintf_s(buf, 1024, "%s_%d", recipe->pchEnhancementReward, recipe->level);

        shared_name = shared_memory ? allocAddSharedString(buf) : allocAddString(buf);

        if (!stashAddPointerConst(tempItemsFromEnhancementName, shared_name, recipe, false))
        {
            // Can have multiple recipes that make the same enhancement.  This is simply used as a check
            //        to see if there are any recipes for a particular enhancement.
            // Errorf("duplicate recipe for enhancement %s at level %d", recipe->pchName, recipe->level );
            // ret = false;
        }
    }

    assert(!pdict->itemsFromEnhancementName);
    pdict->itemsFromEnhancementName = stashTableCreateWithStringKeys(stashOptimalSize(stashGetValidElementCount(tempItemsFromEnhancementName)), stashShared(shared_memory));
    stashTableMergeConst(pdict->itemsFromEnhancementName, tempItemsFromEnhancementName, false);
    stashTableDestroy(tempItemsFromEnhancementName);

    return ret;
}

static bool detailrecipe_CreateSalvageComponentHash(DetailRecipeDict *pdict, bool shared_memory)
{
#if CLIENT
    bool ret = true;
    int i;

    StashTableIterator iter;
    StashElement elem;

    StashTable tempSalvageComponent = stashTableCreateWithStringKeys(eaSize(&pdict->ppRecipes), StashDefault);

    // hash lists into a temporary table
    for (i = 0; i < eaSize(&pdict->ppRecipes); i++)
    {
        DetailRecipe *recipe = cpp_const_cast(DetailRecipe*)(pdict->ppRecipes[i]);

        int j;

        // this is a stash of all the recipes a given piece of salvage uses
        if( recipe->Type != kRecipeType_Drop )
            continue;

        for (j = 0; j < eaSize(&recipe->ppSalvagesRequired); j++) 
        {
            SalvageItem *salvage = cpp_const_cast(SalvageItem*)(recipe->ppSalvagesRequired[j]->pSalvage);
            DetailRecipe **ppRec = NULL;

            if( salvageitem_IsInvention(salvage) )
            {
                if (!stashFindElement(tempSalvageComponent, recipe->ppSalvagesRequired[j]->pSalvage->pchName, &elem))
                {
                    eaCreateWithCapacity(&ppRec, 16);
                    stashAddPointerAndGetElement(tempSalvageComponent, recipe->ppSalvagesRequired[j]->pSalvage->pchName, ppRec, false, &elem);
                }

                ppRec = stashElementGetPointer(elem);
                eaPush(&ppRec, recipe);
                stashElementSetPointer(elem, ppRec);
            }
        }
    }

    assert(!pdict->stBySalvageComponent);
    pdict->stBySalvageComponent = stashTableCreateWithStringKeys(stashOptimalSize(stashGetValidElementCount(tempSalvageComponent)), stashShared(shared_memory));

    // build the final table from the temporary one
    stashGetIterator(tempSalvageComponent, &iter);    
    while (stashGetNextElement(&iter, &elem))
    {
        DetailRecipe **tempRec = stashElementGetPointer(elem);
        DetailRecipe **sharedRec = NULL;

        if (shared_memory)
        {
            eaCompress(&sharedRec, &tempRec, customSharedMalloc, NULL);
            eaDestroy(&tempRec);
        }
        else
            sharedRec = tempRec;

        stashAddPointerConst(pdict->itemsFromTabName, stashElementGetStringKey(elem), sharedRec, false);
    }

    stashTableDestroy(tempSalvageComponent);

    return ret;
#else
    return true;
#endif
}

//------------------------------------------------------------
// init the salvage table
//----------------------------------------------------------
static bool detailrecipe_FinalProcess(ParseTable pti[], DetailRecipeDict *pdict, bool shared_memory)
{
    bool ret = true;
    int i;
    int rarityCount = 0;

    DetailRecipe **tempItemsById = NULL;

    struct MaxIdxHashTablePair
    {
        U32 nextIdx;
        cStashTable ht;
    } idHt = {0};

    // --------------------
    // get the id hashes

    AttribFileDict const*dict = inventorytype_GetAttribs( kInventoryType_Recipe );
    idHt.ht = dict->idHash;

    // set the next index
    // returns max index found, so we count after that. works even for zero case
    idHt.nextIdx = 1 + dict->maxId;

    // --------------------
    //  create the hash table

    assert(!pdict->haItemNames);
    pdict->haItemNames = stashTableCreateWithStringKeys( stashOptimalSize(eaSize(&pdict->ppRecipes)), stashShared(shared_memory) );

    // --------------------
    // fill hash, set ids

    for( i = 0; i < eaSize( &pdict->ppRecipes ); ++i )
    {
        DetailRecipe *recipe = cpp_const_cast(DetailRecipe*)(pdict->ppRecipes[i]);
        int idx, j=0;

        // --------------------
        // hash of names

        // set id
        if( stashFindInt( idHt.ht, recipe->pchName, &idx ) )
        {
            recipe->id = idx; // use the existing value
        }
        else
        {
            recipe->id = idHt.nextIdx++; // grab the highest and incr
        }

        // add the hash
        if( !stashAddPointerConst(pdict->haItemNames, recipe->pchName, recipe, false) )
        {
            Errorf("duplicate recipe name %s", recipe->pchName );
            ret = false;
        }
    }

    ret &= detailrecipe_CreateTabNameHash(pdict, shared_memory);
    ret &= detailrecipe_CreateCategoryHash(pdict, shared_memory);
    ret &= detailrecipe_CreateEnhancementHash(pdict, shared_memory);
    ret &= detailrecipe_CreateSalvageComponentHash(pdict, shared_memory);

    // --------------------
    // do the items by id

    eaCreateWithCapacityConst(&tempItemsById, idHt.nextIdx);
    eaSetSize(&tempItemsById, idHt.nextIdx);

    for( i = eaSize( &pdict->ppRecipes ) - 1; i >= 0; --i)
    {
        DetailRecipe *recipe = cpp_const_cast(DetailRecipe*)(pdict->ppRecipes[i]);
        assert(eaSet(&tempItemsById, recipe, recipe->id));
    }

    // Pack into shared memory
    if (shared_memory)
    {
        assert(!pdict->itemsById);
        eaCompressConst(&pdict->itemsById, &tempItemsById, customSharedMalloc, NULL);
        eaDestroy(&tempItemsById);
    }
    else
        pdict->itemsById = tempItemsById;

    return ret;
}

//------------------------------------------------------------
//  Load the recipes for the base details
//----------------------------------------------------------
void detailrecipes_Load(void)
{
#if SERVER 
    int flags = PARSER_SERVERONLY;
#else
    int flags = 0;
#endif
    ParserLoadFilesShared(MakeSharedMemoryName("SM_BaseDetailRecipes"), "defs", ".recipe", "baserecipes.bin",
                          flags, ParseDetailRecipeDict, &g_DetailRecipeDict, sizeof(DetailRecipeDict),
                          NULL, NULL, RecipeDetailDictPreprocess, NULL, detailrecipe_FinalProcess);
}

//------------------------------------------------------------
//  Check if the recipe can be used
//----------------------------------------------------------
bool entity_DetailRecipeUsable(Entity *e, DetailRecipe *pDef)
{
// see badges_load.c:LoadBadgeStatUsage when ready
    return verify(false);
}


/**********************************************************************func*
 * recipeeval_RecipeFetch
 *
 */
void recipeeval_RecipeFetch(EvalContext *pcontext)
{
    DetailRecipe *precipe;
    bool bFound = eval_FetchInt(pcontext, "Recipe", (int *)&precipe);
    const char *rhs = eval_StringPop(pcontext);

    if(bFound && precipe && rhs)
    {
        if(stricmp(rhs, "category")==0 || strnicmp(rhs, "tab", 3)==0)
        {
            eval_StringPush(pcontext, precipe->pchDisplayTabName);
        }
        else
        {
            eval_IntPush(pcontext, 0);
        }
        return;
    }

    eval_IntPush(pcontext, 0);
}

/**********************************************************************func*
 * recipeeval_EmptyDetailFetchHelper
 *
 */
static void recipeeval_EmptyDetailFetchHelper(EvalContext *pcontext, const char *rhs)
{
    if(rhs)
    {
        if(stricmp(rhs, "category")==0
            || strnicmp(rhs, "tab", 3)==0
            || stricmp(rhs, "function")==0
            || stricmp(rhs, "params")==0
            || stricmp(rhs, "name")==0)
        {
            eval_StringPush(pcontext, "*");
        }
        else
        {
            eval_IntPush(pcontext, 0);
        }
        return;
    }

    eval_IntPush(pcontext, 0);
}

/**********************************************************************func*
 * recipeeval_DetailFetch
 *
 */
void recipeeval_DetailFetch(EvalContext *pcontext)
{
    RoomDetail *pdetail;
    bool bFound = eval_FetchInt(pcontext, "Detail", (int *)&pdetail);
    const char *rhs = eval_StringPop(pcontext);

    if(bFound && rhs && !pdetail)
    {
        recipeeval_EmptyDetailFetchHelper(pcontext, rhs);
        return;
    }

    if(bFound && rhs && pdetail)
    {
        if(stricmp(rhs, "category")==0 || strnicmp(rhs, "tab", 3)==0)
        {
            eval_StringPush(pcontext, pdetail->info->pchDisplayTabName);
        }
        else if(stricmp(rhs, "function")==0)
        {
            const char *pch = detail_ReverseLookupFunction(pdetail->info->eFunction);
            eval_StringPush(pcontext, pch);
        }
        else if(stricmp(rhs, "params")==0)
        {
            if(pdetail->info->ppchFunctionParams && eaSize(&pdetail->info->ppchFunctionParams)>0)
            {
                eval_StringPush(pcontext, pdetail->info->ppchFunctionParams[0]);
            }
            else
            {
                eval_StringPush(pcontext, "none");
            }
        }
        else if(stricmp(rhs, "name")==0)
        {
            eval_StringPush(pcontext, pdetail->info->pchName);
        }
        else
        {
            eval_IntPush(pcontext, 0);
        }
        return;
    }

    eval_IntPush(pcontext, 0);
}


/**********************************************************************func*
 * recipeeval_Init
 *
 */
static void recipeeval_Init(void)
{
    static bool s_bInitDone = false;

    if(s_bInitDone)
        return;

    // Create the evaluation context used by everyone.
    s_pRecipeEval = eval_Create();

    // Add in all basic entity and supergroup EvalFuncs
    chareval_AddDefaultFuncs(s_pRecipeEval);

    // Add in special cases for recipes.
    eval_ReRegisterFunc(s_pRecipeEval, "auto>",   chareval_EntityFetch,   1, 1);
    eval_RegisterFunc(s_pRecipeEval, "detail>", recipeeval_DetailFetch, 1, 1);
    eval_RegisterFunc(s_pRecipeEval, "recipe>", recipeeval_RecipeFetch, 1, 1);

    s_bInitDone=true;
}


/**********************************************************************func*
 * recipeeval_Eval
 *
 */
static bool recipeeval_Eval(const DetailRecipe *pRecipe, Entity *e, RoomDetail *pSrc, const char * const * ppchExpr)
{
    if(!ppchExpr)
        return true;

    if(!s_pRecipeEval)
        recipeeval_Init();

    eval_StoreInt(s_pRecipeEval, "Entity", (intptr_t)e);
    eval_StoreInt(s_pRecipeEval, "DetailSrc", (intptr_t)pSrc);
    eval_StoreInt(s_pRecipeEval, "Recipe", (intptr_t)pRecipe);
    eval_ClearStack(s_pRecipeEval);

    eval_Evaluate(s_pRecipeEval, ppchExpr);
    return (eval_IntPeek(s_pRecipeEval) != 0);
}

//------------------------------------------------------------
// entity_IsAtWorkShop
// Helper function used to determine if the player is at a workbench or not
//------------------------------------------------------------
int entity_IsAtWorkShop(Entity *e, RoomDetail *pSrc)
{

    if (!e || !e->pchar)
        return false;

    if (e->pl->workshopInteracting[0] != 0)
        return true;

    if (pSrc && pSrc->info)
    {
        return (stricmp(pSrc->info->pchCategory, "workshop") == 0 || 
                stricmp(pSrc->info->pchCategory, "empowerment") == 0);
    }

    return false;
}

//------------------------------------------------------------
//  Get the recipes that match
// returns EArray of details, must be cleaned up by user.
//
// NOTE: this is the recipes the entity has the ability to
// purchase, it does not check the salvage requirements, just the
// availability ones.
//----------------------------------------------------------
const DetailRecipe **entity_GetValidDetailRecipes(Entity *e, RoomDetail *pSrc, char *pWorkshopType)
{
    const DetailRecipe **res = NULL;
    int i, j;
    bool bForceRecipe = false;

    int isAtWorkbench = entity_IsAtWorkShop(e, pSrc) || (pWorkshopType && (stricmp(pWorkshopType, "Worktable_Incarnate") == 0));

#if SERVER
    bForceRecipe = server_state.beta_base != 0;
#else
    bForceRecipe = game_state.beta_base != 0;
#endif

    // --------------------
    // iterate over recipes to see if valid

    for(i = eaSize( &g_DetailRecipeDict.ppRecipes ) - 1; i >= 0; --i)
    {
        const DetailRecipe *pr = g_DetailRecipeDict.ppRecipes[i];
        int size, workshop = false;

        if(!pr)
            continue;

        // check to see if this is a valid workshop
        if ( isAtWorkbench ) // bWorkshopDetail || pWorkshopType[0])
        {
            // we are at a workshop, so only show the things that can be made there
            size = eaSize(&pr->ppWorkshops);
            for(j = 0; j < size; j++)
            {
                if (pr->ppWorkshops[j] != NULL) 
                {
                    if(pSrc && pr->ppWorkshops[j] == pSrc->info)
                    {
                        workshop = true;
                        break;
                    }
                    if(pWorkshopType && stricmp(pWorkshopType, pr->ppWorkshops[j]->pchName) == 0)
                    {
                        workshop = true;
                        break;
                    }
                }
            }
        } 
        else
        {
            // not at a workshop, so it won't filter
            workshop = true;
        }

        if (workshop) 
        {
            if (isAtWorkbench && (pr->Type == kRecipeType_Workshop || pr->BuyFromVendor > 0))
            {
                // global recipe 
                if(bForceRecipe || recipeeval_Eval(pr, e, pSrc, pr->ppchVisibleRequires) != 0)
                {
                    eaPushConst(&res, pr);
                }
            }
            else
            {
                // Check if player has this recipe
                if (bForceRecipe || character_RecipeCount(e->pchar, pr) > 0 || 
                    ( pr->Type == kRecipeType_Memorized && recipeeval_Eval(pr, e, pSrc, pr->ppchVisibleRequires) != 0))
                {
                    eaPushConst(&res, pr);
                }
            }
        }
    }

    return res;
}

//------------------------------------------------------------
// see if a detail recipe is creatable.
// param hInventory: contains the inventory items needed to create.
//       NOTE: the index of this will correspond exactly with
//       the index in ppSalvagesRequired.
//  onlyConsumeOrOnlyGrant will only check one half of the validation,
// -1 for consume
// 0 both checks (except RecieveRequires which is only relevant to Account Items)
// 1 for grant (including RecieveRequires)
//----------------------------------------------------------
bool character_DetailRecipeCreatable(Character *c, const DetailRecipe *dr, SalvageInventoryItem ***hInventory, 
                                     bool bCheckRecipeInventory, bool bUseCoupon, int onlyConsumeOrOnlyGrant )
{
    bool res = true;
    int i;

    if( onlyConsumeOrOnlyGrant <= 0 )
    {
        //    the recipe isn't visible. how are you making this? lag?
        if (res && dr->ppchVisibleRequires != NULL)
        {
            if (!chareval_Eval(c, dr->ppchVisibleRequires, dr->pchSourceFile))
            {
                res = false;
            }
        }

        if( verify( c && c->entParent && dr ) )
        {
            int size = eaSize( &dr->ppSalvagesRequired ); // go from the front so order is preserved

            // check for license
            if (dr->flags & RECIPE_INVENTION_LIC_REQUIRED)
            {
                // HYBRID : Verify invention license product code
                if (!AccountCanUseInvention(&c->entParent->pl->account_inventory, c->entParent->pl->account_inventory.accountStatusFlags))
                    res = false;
            }

            // check the salvages
            for(i = 0;i < size; ++i)
            {
                const SalvageRequired *sr = dr->ppSalvagesRequired[i];
                SalvageInventoryItem *si = sr && sr->pSalvage ? character_FindSalvageInv( c, sr->pSalvage->pchName ) : NULL;
                if( si )
                {
                    // set the result
                    res = res && si->amount >= sr->amount;

                    // also save the inventory, if applicable.
                    if( hInventory )
                    {
                        eaPush( hInventory, si );
                    }
                }
                else
                {
                    res = false;
                    if( hInventory )
                    {
                        eaPush( hInventory, NULL);
                    }
                }
            }

            // check the power components
            size = eaSize(&dr->ppPowersRequired);
            for (i = 0; i < size; i++)
            {
                // NOTE:  pr->amount is ignored!
                const PowerRequired *pr = dr->ppPowersRequired[i];
                Power *pPow = (pr && pr->pPower) ? character_OwnsPower(c, pr->pPower) : NULL;
                if (pPow)
                {
                    bool isCharged = true; // is not recharging
#if SERVER
                        PowerListItem *ppowListItem;
                        PowerListIter iter;

                        for(ppowListItem = powlist_GetFirst(&c->listRechargingPowers, &iter);
                            ppowListItem != NULL;
                            ppowListItem = powlist_GetNext(&iter))
                        {
                            if (ppowListItem->ppow == pPow)
                            {
                                isCharged = false;
                                break;
                            }
                        }
#else
                    if (!onlyConsumeOrOnlyGrant)
                    {
                        PowerRef ppowRef;
                        ppowRef.buildNum = c->iCurBuild;
                        ppowRef.powerSet = pPow->psetParent->idx;
                        ppowRef.power = pPow->idx;
                        {
                            PowerRechargeTimer *prt = c && c->entParent ? 
                                powerInfo_PowerGetRechargeTimer(c->entParent->powerInfo, ppowRef) : NULL;
                            if (prt && prt->rechargeCountdown)
                            {
                                isCharged = false;
                                addSystemChatMsg(textStd("CraftingPowerRechargingError"), INFO_SVR_COM, 0);
                            }
                        }
                    }
#endif
                    res &= isCharged; // Alter this if we want to pay attention to pr->amount
                }
                else
                {
                    res = false;
                }
            }

            // check the influence cost
            if (res && dr->CreationCost != NULL)
            {
                int cost = chareval_Eval(c, dr->CreationCost, dr->pchSourceFile);

                if (bUseCoupon)
                {
                    // check for coupon
                    SalvageInventoryItem *sii = NULL;
                    sii = character_FindSalvageInv(c, INVENTION_COUPON_SALVAGE);
                    if( sii && sii->amount > 0)
                    {
                        cost = (int) ((float) cost * INVENTION_COUPON_DISCOUNT);
                    } else {
                        res = false;    // asked to use coupon, but don't have any
                    }
                }

                if (cost > 0 && c->iInfluencePoints < cost)
                    res = false;
            }

            // add additional requirements here
            for (i = 0; i < eaSize(&dr->pchAdditionalComponents); i++)
            {
                if (stricmp(dr->pchAdditionalComponents[i], "raidpoints") == 0)
                {
                    if (!c->entParent->supergroup)
                    {
                        // not in a SG
                        res = false;
                    } else {
                        int amount = atoi(dr->pchAdditionalComponents[i+1]);
                        int idx = rewardtoken_IdxFromName( &c->entParent->supergroup->rewardTokens, "raidpoints" );

                        if (EAINRANGE(idx, c->entParent->supergroup->rewardTokens))
                        {
                            if (c->entParent->supergroup->rewardTokens[idx]->val < amount || 
                                !sgroup_hasPermissionByName( c->entParent, "SgPermissionAllowedToSpendRaidPoints"))
                            {
                                res = false;
                            }
                        } else {
                            // didn't find any raid points on this SG
                            res = false;
                        }
                        i++;
                    }
                } 
                else if  (stricmp(dr->pchAdditionalComponents[i], "prestige") == 0)
                {
                    if (!c->entParent->supergroup)
                    {
                        // not in a SG
                        res = false;
                    } else {
                        int amount = atoi(dr->pchAdditionalComponents[++i]);

                        if (c->entParent->supergroup->prestige < amount  || 
                            !sgroup_hasPermissionByName( c->entParent, "SgPermissionAllowedToSpendRaidPoints"))
                            res = false;
                    }
                }
                else if  (stricmp(dr->pchAdditionalComponents[i], "iop") == 0)
                {
                    if (!c->entParent->supergroup)
                    {
                        // not in a SG
                        res = false;
                    } else {
                        int j, count = eaSize(&c->entParent->supergroup->specialDetails);
                        int found = false;
                        for (j = 0; j < count; j++)
                        {
                            if (c->entParent->supergroup->specialDetails[j]->pDetail && 
                                stricmp(dr->pchAdditionalComponents[i+1], c->entParent->supergroup->specialDetails[j]->pDetail->pchName) == 0)
                            {
                                found = true;
                                break;
                            }

                        }

                        if (!found)
                            res = false;
                        i++;
                    }
                } else {
                    // unknown
                }
            }

            // check if this recipe requires a recipe from the player
            if (bCheckRecipeInventory && res && dr->Type == kRecipeType_Drop)
            {
                if (character_RecipeCount(c, dr) <= 0)
                    res = false;
            }

            if (res && dr->ppchCreateRequires != NULL)
            {
                if (!chareval_Eval(c, dr->ppchCreateRequires, dr->pchSourceFile))
                    res = false;
            }
        }
    }

    if( onlyConsumeOrOnlyGrant >= 0 )
    {
        // check creates requirements
        if (res && dr->creates_salvage)
        {
            if (c->salvageInvCurrentCount + dr->creates_salvage > character_GetInvTotalSize(c, kInventoryType_Salvage))
                res = false;
        }
        if (res && dr->creates_enhancement)
        {
            if (dr->creates_enhancement > character_BoostInventoryEmptySlots(c))
                res = false;
        }
        if (res && dr->creates_recipe)
        {
            if (c->recipeInvCurrentCount + dr->creates_recipe > character_GetInvTotalSize(c, kInventoryType_Recipe))
                res = false;
        }
        if (res && dr->creates_inspiration)
        {
            if (dr->creates_inspiration > character_InspirationInventoryEmptySlots(c))
                res = false;
        }
        if (res && dr->pchPowerReward) // ignores dr->amount, enforces only one copy of a power
        {
            if (!character_CanBuyPowerWithoutOverflow(c, powerdict_GetBasePowerByFullName(&g_PowerDictionary, dr->pchPowerReward)))
                res = false;
        }
        if (res && dr->pchIncarnateReward) // ignores dr->amount, enforces only one copy of a power
        {
            if (character_OwnsPower(c, powerdict_GetBasePowerByFullName(&g_PowerDictionary, dr->pchIncarnateReward)))
                res = false;
        }
    }

    if( onlyConsumeOrOnlyGrant >= 1 )
    {
        // check RecieveRequires as well (Account Items only)
        if (res && dr->ppchReceiveRequires != NULL)
        {
            if (!chareval_Eval(c, dr->ppchReceiveRequires, dr->pchSourceFile))
                res = false;
        }
    }

    // --------------------
    // finally

    return res;
}

#if SERVER

//------------------------------------------------------------
//  Create the detail recipe and add it to the inventory.
// returns 1 on success, 0 on failure to add (full inventory, etc), -1 on failed requirements
//----------------------------------------------------------

int character_DetailRecipeConsumeRequirements(Character *pchar, const DetailRecipe *dr, int iLevel, int bUseCoupon, SalvageInventoryItem ***hInventory)
{
    int i, res = -1;
    int cost = 0;
    SalvageInventoryItem **invs = NULL;

    if( !hInventory )
    {
        if( !character_DetailRecipeCreatable( pchar, dr, &invs, true, bUseCoupon != 0, -1 ) )
            return res;
    }
    else
        invs = *(hInventory);

    // If we actually created something, use up the ingredients
    for( i = eaSize( &dr->ppSalvagesRequired ) - 1; i >= 0; --i)
    {
        int amount = dr->ppSalvagesRequired[i] ? dr->ppSalvagesRequired[i]->amount : 0;
        const SalvageItem *ps = dr->ppSalvagesRequired[i] ? dr->ppSalvagesRequired[i]->pSalvage : NULL;

        // --------------------
        // deduct salvage

        if( verify(invs[i] && invs[i]->salvage && ps && ps == invs[i]->salvage && invs[i]->amount >= (U32)amount  ))
        {
            character_AdjustSalvage( pchar, ps, -amount, "recipe", false );
            LOG_ENT(pchar->entParent, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, "Invent:Salvage:Remove Used %d of %s salvage",amount, ps->pchName);
        }
    }

    for (i = eaSize(&dr->ppPowersRequired) - 1; i >= 0; --i)
    {
        // ignore amount
        const BasePower *basepow = dr->ppPowersRequired[i] ? dr->ppPowersRequired[i]->pPower : NULL;

        if (verify(character_OwnsPower(pchar, basepow)))
        {
            character_RemoveBasePower(pchar, basepow);
            LOG_ENT( pchar->entParent, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[Inv]:Pow:Remove Used %s power", basepow->pchName);
        }
    }

    // remove influence cost
    if (dr->CreationCost != NULL)
        cost = chareval_Eval(pchar, dr->CreationCost, dr->pchSourceFile);

    if (bUseCoupon)
    {
        const SalvageItem *si = salvage_GetItem( INVENTION_COUPON_SALVAGE );
        if( si )
        {
            if(character_CanAdjustSalvage(pchar, si, -1) )
            {
                character_AdjustSalvage( pchar, si, -1, "Recipe", false);
                cost = (int) ((float) cost * INVENTION_COUPON_DISCOUNT); 
            }
        }
    }

    ent_AdjInfluence(pchar->entParent, -cost, NULL);
    badge_StatAdd(pchar->entParent, "inf.invention", cost);
    dbLog("Invent:Influence:Remove",pchar->entParent,"Used %d influence", cost);

    // remove recipe if its personal
    if (dr->Type == kRecipeType_Drop)
    {
        character_AdjustRecipe(pchar, dr, -1, "used");
        dbLog("Invent:Recipe:Remove",pchar->entParent,"Used %s recipe", dr->pchName);
    } else {
        DATA_MINE_RECIPE(dr, "null", "create", 1);
    }

    // remove any additional requirements
    for (i = 0; i < eaSize(&dr->pchAdditionalComponents); i++)
    {
        if (stricmp(dr->pchAdditionalComponents[i], "raidpoints") == 0)
        {
            if (pchar->entParent->supergroup && teamLock(pchar->entParent, CONTAINER_SUPERGROUPS))
            {
                int amount = atoi(dr->pchAdditionalComponents[++i]);
                int idx = rewardtoken_IdxFromName( &pchar->entParent->supergroup->rewardTokens, "raidpoints" );

                if (EAINRANGE(idx, pchar->entParent->supergroup->rewardTokens))
                {
                    if (pchar->entParent->supergroup->rewardTokens[idx]->val >= amount)
                    {
                        pchar->entParent->supergroup->rewardTokens[idx]->val -= amount;
                    } else {
                        pchar->entParent->supergroup->rewardTokens[idx]->val = 0;
                    }
                }
                teamUpdateUnlock( pchar->entParent, CONTAINER_SUPERGROUPS );
            }
        } 
        else if (stricmp(dr->pchAdditionalComponents[i], "prestige") == 0)
        {
            if (pchar->entParent->supergroup && teamLock(pchar->entParent, CONTAINER_SUPERGROUPS))
            {
                int amount = atoi(dr->pchAdditionalComponents[++i]);

                pchar->entParent->supergroup->prestige -= amount;

                if (pchar->entParent->supergroup->prestige < 0)
                    pchar->entParent->supergroup->prestige = 0;
                teamUpdateUnlock( pchar->entParent, CONTAINER_SUPERGROUPS );
            }
        }
        else if  (stricmp(dr->pchAdditionalComponents[i], "iop") == 0)
        {
            ItemOfPowerRevoke2( pchar->entParent, dr->pchAdditionalComponents[++i] );
        } else {
            // unknown
        }
    }

    // give a signal to the mission system in case its needed there
    InventionTaskCraft(pchar->entParent, dr->pchName);

    // record badge credit
    badge_RecordInventionCreated(pchar->entParent,dr->pchName, (dr->flags & RECIPE_NO_GENERIC_BADGE_CREDIT));

    LOG_ENT( pchar->entParent, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "Base:Rec:Create %s ",dr->pchName);

    return 1;
}

static bool character_canDetailRecipeRefundRequirements(Character *pchar, const DetailRecipe *dr, int iLevel)
{
    int i;

    for( i = eaSize( &dr->ppSalvagesRequired ) - 1; i >= 0; --i)
    {
        int amount = dr->ppSalvagesRequired[i] ? dr->ppSalvagesRequired[i]->amount : 0;
        const SalvageItem *ps = dr->ppSalvagesRequired[i] ? dr->ppSalvagesRequired[i]->pSalvage : NULL;

        // --------------------
        // refund salvage
        if( !character_CanAdjustSalvage( pchar, ps, amount ))
            return false;
    }

    return true;
}

int character_DetailRecipeRefundRequirements(Character *pchar, const DetailRecipe *dr, int iLevel)
{
    int i;
    int cost = 0;
    
    if( !character_canDetailRecipeRefundRequirements(pchar, dr, iLevel) )
        return 0;

    for( i = eaSize( &dr->ppSalvagesRequired ) - 1; i >= 0; --i)
    {
        int amount = dr->ppSalvagesRequired[i] ? dr->ppSalvagesRequired[i]->amount : 0;
        const SalvageItem *ps = dr->ppSalvagesRequired[i] ? dr->ppSalvagesRequired[i]->pSalvage : NULL;

        // --------------------
        // refund salvage
        character_AdjustSalvage( pchar, ps, amount, "recipe", false );
        dbLog("Reward:RefundSalvage", pchar->entParent, "received amount %d", amount);
        sendInfoBox(pchar->entParent, INFO_REWARD, "SalvageYouReceivedNum", amount, ps->ui.pchDisplayName);
        serveFloater(pchar->entParent, pchar->entParent, "FloatFoundSalvage", 0.0f, kFloaterStyle_Attention, 0);
        LOG_ENT( pchar->entParent, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[Inv]:Refund:Sal %d x %s", amount, ps->pchName);
    }

    for (i = eaSize(&dr->ppPowersRequired) - 1; i >= 0; --i)
    {
        // ignore amount
        const BasePower *pPower = dr->ppPowersRequired[i] ? dr->ppPowersRequired[i]->pPower : NULL;

        if (pPower && !character_OwnsPower(pchar, pPower)) // ignores dr->amount, enforces only one copy of a power
        {
            PowerSet *pSet = character_OwnsPowerSet(pchar, pPower->psetParent);
            if (!pSet)
            {
                pSet = character_BuyPowerSet(pchar, pPower->psetParent);
            }

            if (pSet)
            {
                Power *ppow = character_BuyPower(pchar, pSet, pPower, 0);

                eaPush(&pchar->entParent->powerDefChange, powerRef_CreateFromPower(pchar->iCurBuild, ppow));
                LOG_ENT( pchar->entParent, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[Inv]:Refund:Tpow %s", dr->pchPowerReward);
                sendInfoBox(pchar->entParent, INFO_REWARD, "DetailYouReceived", pPower->pchDisplayName);
                serveFloater(pchar->entParent, pchar->entParent, "FloatCreatePower", 0.0f, kFloaterStyle_Attention, 0);
            }
        }
    }

    // remove influence cost
    cost = chareval_Eval(pchar, dr->CreationCost, dr->pchSourceFile);
    ent_AdjInfluence(pchar->entParent, cost, NULL);

    // refund any additional requirements
    for (i = 0; i < eaSize(&dr->pchAdditionalComponents); i++)
    {
        if (stricmp(dr->pchAdditionalComponents[i], "raidpoints") == 0)
        {
            if (pchar->entParent->supergroup && teamLock(pchar->entParent, CONTAINER_SUPERGROUPS))
            {
                int amount = atoi(dr->pchAdditionalComponents[++i]);
                int idx = rewardtoken_IdxFromName( &pchar->entParent->supergroup->rewardTokens, "raidpoints" );

                if (EAINRANGE(idx, pchar->entParent->supergroup->rewardTokens))
                {
                    pchar->entParent->supergroup->rewardTokens[idx]->val += amount;
                }
                teamUpdateUnlock( pchar->entParent, CONTAINER_SUPERGROUPS );
            }
        } 
        else if (stricmp(dr->pchAdditionalComponents[i], "prestige") == 0)
        {
            if (pchar->entParent->supergroup && teamLock(pchar->entParent, CONTAINER_SUPERGROUPS))
            {
                int amount = atoi(dr->pchAdditionalComponents[++i]);

                pchar->entParent->supergroup->prestige += amount;

                if (pchar->entParent->supergroup->prestige < 0)
                    pchar->entParent->supergroup->prestige = 0;
                teamUpdateUnlock( pchar->entParent, CONTAINER_SUPERGROUPS );
            }
        }
        else 
        {
            // unknown
        }
    }

    LOG_ENT( pchar->entParent, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "Base:Recipe:Refund %s",dr->pchName);
    return 1;
}

int character_DetailRecipeGrantResults(Character *pchar, const DetailRecipe *dr, int iLevel, int bUseCoupon, int * bArchitect, int verified)
{

    int res = -1;

    if(!verified && !character_DetailRecipeCreatable( pchar, dr, 0, true, bUseCoupon != 0, 1 ))
        return res;

    if (dr->pDetailReward)
    {
        int id = character_AddBaseDetail( pchar, dr->pDetailReward->pchName, "recipe" );
        if (id >= 0)
        {
            res = 1;
            LOG_ENT( pchar->entParent, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[Inv]:Rcv:Det %s", dr->pDetailReward->pchName);
            sendInfoBox(pchar->entParent, INFO_REWARD, "DetailYouReceived",dr->pDetailReward->pchDisplayName);
            serveFloater(pchar->entParent, pchar->entParent, "FloatFoundBaseDetail", 0.0f, kFloaterStyle_Attention, 0);
        }
        else
            res = 0;
    }
    else if (dr->pchInventReward) 
    {
        char *pCopy = NULL;
        char *pType = NULL;
        char *pTime = NULL;
        int i;
        int time;

        for (i = 0; i < eaSize(&dr->pchInventReward); i++)
        {
            strdup_alloca(pCopy, dr->pchInventReward[i]);

            pType = strtok(pCopy, ".");

            // check to see if this is a 'special' reward
            if (pType && stricmp(pType, "iopgrant") == 0)
            {
                pType = strtok(NULL, ".");
                pTime = strtok(NULL, ".");
                res = ItemOfPowerGrant2(pchar->entParent, pType, atoi(pTime));
            } 
            else if (pType && stricmp(pType, "iopupgrade") == 0)
            {
                SpecialDetail *pSDetail = NULL;
                char *pTypeFrom = NULL;
                char *pPercent = NULL;

                pType = strtok(NULL, ".");
                pTypeFrom = strtok(NULL, ".");
                pPercent = strtok(NULL, ".");
                pTime = strtok(NULL, ".");

                pSDetail = FindSpecialDetailByName(pchar->entParent->supergroup, pTypeFrom);

                if (pSDetail && pSDetail->pDetail)
                {
                    time = (int) ((float) (pSDetail->expiration_time - timerSecondsSince2000()) * ((float) atof(pPercent) / 100.0f)) + atoi(pTime);
                    res = ItemOfPowerGrant2(pchar->entParent, pType, time);
                } else {
                    res = false;
                }
            }
            else if (pType && stricmp(pType, "iopextend") == 0)
            {
                SpecialDetail *pSDetail = NULL;

                pType = strtok(NULL, ".");
                pTime = strtok(NULL, ".");
                pSDetail = FindSpecialDetailByName(pchar->entParent->supergroup, pType);

                if (pSDetail && pSDetail->pDetail)
                {
                    sgroup_ExtendSpecialDetail(pchar->entParent, pSDetail->pDetail, pSDetail->creation_time, atoi(pTime));
                    res = 1;
                } else {
                    res = false;
                }
            }
            else if (pType && stricmp(pType, "salvage") == 0) 
            {
                char *pAmount = NULL;
                const SalvageItem *si = NULL;
                int    iAmount = 0; 

                pType = strtok(NULL, ".");
                pAmount = strtok(NULL, ".");

                if (pAmount)
                    iAmount = atoi(pAmount);

                if (pType)
                    si = salvage_GetItem( pType );

                if (si != NULL && pAmount > 0 && character_CanAdjustSalvage(pchar, si, iAmount))
                {
                    character_AdjustSalvage( pchar, si, iAmount, "recipe", false );
                    LOG_ENT( pchar->entParent, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[Inv]:Rcv:Sal %d x %s", iAmount, pType);
                    sendInfoBox(pchar->entParent, INFO_REWARD, "SalvageYouReceivedNum", iAmount, si->ui.pchDisplayName);
                    serveFloater(pchar->entParent, pchar->entParent, "FloatFoundSalvage", 0.0f, kFloaterStyle_Attention, 0);
                    res = 1;
                } else {
                    res = false;
                }

            } 
            else if (pType && stricmp(pType, "architect") == 0) 
            {
                char * token = strtok(NULL, ".");
                playerCreatedStoryArc_BuyUpgrade(pchar->entParent, token, 0, dr->ppSalvagesRequired[0]->amount );
                res = 1;
                *bArchitect = 1; 
            }
            else if (pType && stricmp(pType, "architectslot") == 0) 
            {
                char * token = strtok(NULL, ".");
                playerCreatedStoryArc_BuyUpgrade(pchar->entParent, token, 1, dr->ppSalvagesRequired[0]->amount );
                res = 1;
                *bArchitect = 1; 
            }
            else
            {
                res = rewardFindDefAndApplyToEnt( pchar->entParent, (const char**)eaFromPointerUnsafe(dr->pchInventReward[i]), VG_NONE, 1, true, REWARDSOURCE_RECIPE, NULL);
            }
        }
    }

    if (dr->pchEnhancementReward)
    {
        const BasePower *ppowBase = powerdict_GetBasePowerByFullName(&g_PowerDictionary, dr->pchEnhancementReward);

        if (ppowBase)
        {
            int createLevel = iLevel;

            if (dr->levelMin > iLevel)
                createLevel = 0;
            if (dr->levelMax < iLevel)
                createLevel = 0;

            if (dr->level > 0)
                createLevel = dr->level;

            if (createLevel > 0)
            {
                if (character_AddBoost(pchar, ppowBase, createLevel - 1, 0, "recipe") >= 0)
                {
                    res = 1;

                    LOG_ENT( pchar->entParent, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[Inv]:Rcv:Enh %s", dr->pchEnhancementReward);
                    sendInfoBox(pchar->entParent, INFO_REWARD, "DetailYouReceived", ppowBase->pchDisplayName);
                    serveFloater(pchar->entParent, pchar->entParent, "FloatCreateEnhancement", 0.0f, kFloaterStyle_Attention, 0);
                }
            }
        }
    }

    if (dr->pchRecipeReward)
    {                
        const DetailRecipe *pRecipe;
        char recipeName[256];
        int createLevel = iLevel;

        if (dr->levelMin > iLevel)
            createLevel = 0;
        if (dr->levelMax < iLevel)
            createLevel = 0;

        if (dr->level > 0)
            createLevel = dr->level;

        if (createLevel > 0)
        {
            sprintf_s(recipeName, 256, "%s_%d", dr->pchRecipeReward, createLevel);

            pRecipe = detailrecipedict_RecipeFromName(recipeName);
            if (pRecipe && pRecipe->Type == kRecipeType_Drop) 
            {
                if (character_AdjustRecipe( pchar, pRecipe, 1, "recipe" ) >= 0)
                {
                    res = 1;
                    LOG_ENT(pchar->entParent, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[Inv]:Rcv:Rec %s", dr->pchRecipeReward);
                    sendInfoBox(pchar->entParent, INFO_REWARD, "DetailYouReceived", pRecipe->ui.pchDisplayName);
                    serveFloater(pchar->entParent, pchar->entParent, "FloatCreateRecipe", 0.0f, kFloaterStyle_Attention, 0);
                }
            }

        }
    }

    if (dr->pchPowerReward)
    {
        if (strnicmp(dr->pchPowerReward, "Inspirations", 12) == 0)
        {
            const BasePower *pPower = powerdict_GetBasePowerByFullName(&g_PowerDictionary, dr->pchPowerReward);

            if (!character_InspirationInventoryFull(pchar))
            {
                character_AddInspiration(pchar, pPower, "recipe");
                res = 1;
                LOG_ENT( pchar->entParent, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[Inv]:Rcv:Tpow %s", dr->pchPowerReward);
                sendInfoBox(pchar->entParent, INFO_REWARD, "DetailYouReceived", pPower->pchDisplayName);
                serveFloater(pchar->entParent, pchar->entParent, "FloatFoundInspiration", 0.0f, kFloaterStyle_Attention, 0); 
            } 
        }
        else
        {
            const BasePower *pPower = powerdict_GetBasePowerByFullName(&g_PowerDictionary, dr->pchPowerReward);
            // ignore level info

            if (pPower && character_CanBuyPowerWithoutOverflow(pchar, pPower)) // ignores dr->amount, enforces only one copy of a power
            {
                PowerSet *pSet = character_OwnsPowerSet(pchar, pPower->psetParent);
                if (!pSet)
                {
                    pSet = character_BuyPowerSet(pchar, pPower->psetParent);
                }

                if (pSet)
                {
                    Power *ppow = character_BuyPower(pchar, pSet, pPower, 0);

                    eaPush(&pchar->entParent->powerDefChange, powerRef_CreateFromPower(pchar->iCurBuild, ppow));
                    res = 1;
                    LOG_ENT( pchar->entParent, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[Inv]:Rcv:Tpow %s", dr->pchPowerReward);
                    sendInfoBox(pchar->entParent, INFO_REWARD, "DetailYouReceived", pPower->pchDisplayName);
                    serveFloater(pchar->entParent, pchar->entParent, "FloatCreatePower", 0.0f, kFloaterStyle_Attention, 0);
                }
            }
        }
    }

    if (dr->pchIncarnateReward)
    {
        char incarnateRewardNameCopy[256];
        char *category;
        char *powerset;
        char *powername;
        IncarnateSlot slot;
        const BasePower *pPower = powerdict_GetBasePowerByFullName(&g_PowerDictionary, dr->pchIncarnateReward);

        strncpy(incarnateRewardNameCopy, dr->pchIncarnateReward, 255);
        category = strtok(incarnateRewardNameCopy, ".");
        powerset = strtok(NULL, ".");
        powername = strtok(NULL, ".");

        if (!stricmp(category, "Incarnate") && (slot = IncarnateSlot_getByInternalName(powerset)) != kIncarnateSlot_Count)
        {
            if (!Incarnate_grant(pchar->entParent, slot, powername))
            {
                char *tempString = 0;
                estrClear(&tempString);
                switch (dr->Rarity)
                {
                case kRecipeRarity_Ubiquitous: // unused, but just to be sure...
                case kRecipeRarity_Common:
                    estrConcatStaticCharArray(&tempString, "Common");
                xcase kRecipeRarity_Uncommon:
                    estrConcatStaticCharArray(&tempString, "Uncommon");
                xcase kRecipeRarity_Rare:
                    estrConcatStaticCharArray(&tempString, "Rare");
                xcase kRecipeRarity_VeryRare:
                    estrConcatStaticCharArray(&tempString, "VeryRare");
                }
                estrConcatStaticCharArray(&tempString, "IncarnateAbilitiesCrafted");
                badge_StatAdd(pchar->entParent, tempString, 1);

                res = 1;
                LOG_ENT( pchar->entParent, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[Inv]:Rcv:Inc %s", dr->pchIncarnateReward);
                sendInfoBox(pchar->entParent, INFO_REWARD, "DetailYouReceived", pPower->pchDisplayName);
                serveFloater(pchar->entParent, pchar->entParent, "FloatCreateIncarnate", 0.0f, kFloaterStyle_Attention, 0);
            }
        }
    }

    if(res == 1 && dr->pchDisplayReceive)
        sendInfoBox(pchar->entParent, INFO_REWARD, dr->pchDisplayReceive);

    return res;
}

int character_DetailRecipeCreate(Character *pchar, const DetailRecipe *dr, int iLevel, int bUseCoupon)
{
    int res = -1;
    int bArchitect = 0;

    if( verify( pchar && dr && (dr->pDetailReward || dr->pchInventReward || dr->pchEnhancementReward 
                                || dr->pchRecipeReward || dr->pchPowerReward || dr->pchIncarnateReward ))) 
    {
        SalvageInventoryItem **invs = NULL;

        if(character_DetailRecipeCreatable( pchar, dr, &invs, true, bUseCoupon != 0, 0 ))
        {
    
            // Try to apply the reward
            res = character_DetailRecipeGrantResults(pchar, dr, iLevel, bUseCoupon, &bArchitect, 1);
            
            if (res > 0 )             
            {
                if( !bArchitect ) // let architect transaction handle itself
                    character_DetailRecipeConsumeRequirements(pchar, dr, iLevel, bUseCoupon, &invs );
            }
            else
            {
                LOG_ENT( pchar->entParent, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "Base:Recipe:Create ERROR: Recipe %s not created, nothing to be rewarded",dr->pchName);
            }
        }
        else
        {
            LOG_ENT( pchar->entParent, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "Base:Recipe:Create ERROR: Recipe %s not created, did not satisfy requirements",dr->pchName);
        }

        eaDestroy(&invs); //clear out results, we don't care
    }
    // --------------------
    // finally

    return res;
}
#endif

//------------------------------------------------------------
//  get a recipe from the global dict.
//----------------------------------------------------------
const DetailRecipe* detailrecipedict_RecipeFromName(const char *name)
{
     return verify(name) ? (const DetailRecipe *)ParserLinkFromString(&g_DetailRecipe_Link, name) : NULL;
}

const DetailRecipe* detailrecipedict_RecipeFromId(int id)
{
    int size = eaSize(&g_DetailRecipeDict.ppRecipes);
    int i;

    for( i = 0; i < size; ++i)
    {
        const DetailRecipe *r = g_DetailRecipeDict.ppRecipes[i];

        if (r->id == id)
            return r;
    }
    return NULL;    
}

/**
* @note This function references a stash table that is not verified as correct at load time.
*/
const DetailRecipe* detailrecipedict_RecipeFromEnhancementAndLevel(const char *enhName, const int level)
{    
    cStashElement elt = NULL;
    char buf[1024];

    sprintf_s(buf, 1024, "%s_%d", enhName, level);

    if (stashFindElementConst( g_DetailRecipeDict.itemsFromEnhancementName, buf, &elt))
        return cpp_reinterpret_cast(const DetailRecipe*)(stashElementGetPointerConst(elt));
    else
        return NULL;
}

bool detailrecipedict_IsBoostTradeable(const BasePower *power, int level, const char *authFrom, const char *authTo)
{
    if (!power || !power->bBoostTradeable)
        return false;

    if (baseset_IsCrafted(power->psetParent)) {
        char *name = basepower_ToPath(power);
        const DetailRecipe *recipe = detailrecipedict_RecipeFromEnhancementAndLevel(name, level+1);
        
        if (!recipe || (recipe->flags & RECIPE_NO_TRADE))
            return false;
    }

    if (power->bBoostAccountBound && stricmp(authFrom, authTo) != 0)
        return false;

    return true;
}

const DetailRecipe* detailrecipedict_RecipeFromCatagoryLevel(const char *name, int level)
{
    size_t            size = strlen(name) * 2;
    char            *buf = (char *) malloc(size);
    const DetailRecipe *retval = NULL;

    if (buf)
    {
        sprintf_s(buf, size, "%s_%d", name, level);
        if (verify(name))
            retval = (const DetailRecipe *) ParserLinkFromString(&g_DetailRecipe_Link, buf);
    
        free(buf);
    }

    return retval;
}



int detailrecipedict_CatagoryMin(const char *catName)
{    
    int retval;

    if (!stashFindInt(g_DetailRecipeDict.haCategoryMinLevel, catName, &retval))
        return -1;

    return retval;
}


int detailrecipedict_CatagoryMax(const char *catName)
{    
    int retval;

    if (!stashFindInt(g_DetailRecipeDict.haCategoryMaxLevel, catName, &retval))
        return -1;

    return retval;
}

int detailrecipedict_CatagoryExists(const char *catName)
{    
    cStashElement elt = NULL;

    // see if it exists
    return stashFindElementConst( g_DetailRecipeDict.haCategoryNames, catName, &elt);

}

/* End of File */
