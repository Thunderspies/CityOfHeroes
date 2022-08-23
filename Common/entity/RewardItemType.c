/***************************************************************************
 *     Copyright (c) 2005-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include <utilitieslib/assert/assert.h>
#include "bases/DetailRecipe.h"
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/textparser.h>
#include "entity/RewardItemType.h"
#include "entity/salvage.h"
#include "entity/Concept.h"
#include "entity/Proficiency.h"

StaticDefineInt RewardItemTypeEnum[] =
{
    DEFINE_INT
    {    "Power",            kRewardItemType_Power                },
    {    "Salvage",            kRewardItemType_Salvage                },
    {    "Concept",            kRewardItemType_Concept                },
    {    "Proficiency",        kRewardItemType_Proficiency            },
    {    "DetailRecipe",        kRewardItemType_DetailRecipe        },
    {    "Detail",            kRewardItemType_Detail                },
    {    "Token",            kRewardItemType_Token                },
    {    "RewardTable",        kRewardItemType_RewardTable            },
    {    "RewardTokenCount",    kRewardItemType_RewardTokenCount    },
    {    "IncarnatePoints",    kRewardItemType_IncarnatePoints        },
    {    "AccountProduct",    kRewardItemType_AccountProduct        },
    DEFINE_END
};


//------------------------------------------------------------
//  find the id for this named reward. returns 0 if not found.
//----------------------------------------------------------
int rewarditemtype_IdFromName(RewardItemType type, char const *name)
{
    int res = 0;
    
    if( name == NULL )
    {
        // this case is fine
        res = 0;
    }
    else
    {
        switch ( type )
        {
        xcase kRewardItemType_Power:
            {
                assertmsg(0, "kRewardItemType_Power not handled yet.");
            }
        xcase kRewardItemType_Salvage:
            {
                // for salvage, we pack the rarity in as well
                const SalvageItem *s = salvage_GetItem( name );
                res = s ? s->salId : 0; 
            }
        xcase kRewardItemType_Concept:
            {
                assertmsg(0, "kRewardItemType_Concept not handled yet.");
            }
        xcase kRewardItemType_Proficiency:
            {
                ProficiencyItem const *p = proficiency_GetItem( name );
                res = verify(p) ? p->id : 0;
            }
        xcase kRewardItemType_DetailRecipe:
            {
                const DetailRecipe *r = detailrecipedict_RecipeFromName( name );
                res = r ? r->id : 0; 
            }
        xcase kRewardItemType_Detail:
            {
                assertmsg(0, "kRewardItemType_Detail not handled yet.");
            }
        xcase kRewardItemType_Token:
            {
                assertmsg(0, "kRewardItemType_Token not handled yet.");
            }
        xcase kRewardItemType_IncarnatePoints:
            {
                assertmsg(0, "kRewardItemType_IncarnatePoints not handled yet.");
            }
        xcase kRewardItemType_AccountProduct:
            {
                assertmsg(0, "kRewardItemType_AccountProduct not handled yet.");
            }
        xdefault:
            {
                assertmsg(0, "Invalid RewardItemType.");
            }
        };
    }

    // --------------------
    // finally
    
    return res;
}


//------------------------------------------------------------
//  gets the persisted version of a rewarditem by its type
//----------------------------------------------------------
char const *rewarditemtype_NameFromId(RewardItemType type, int id)
{
    char const *res = NULL;
    
    if( id == 0 )
    {
        // a zero id always means NULL name
        res = NULL;
    }
    else
    {
        switch ( type )
        {
        xcase kRewardItemType_Power:
            {
                assertmsg(0, "kRewardItemType_Power not handled yet.");
            }
        xcase kRewardItemType_Salvage:
            {
                SalvageItem const *s;

                s = salvage_GetItemById( id );
                res = s ? s->pchName : NULL;
            }
        xcase kRewardItemType_Concept:
            {
                assertmsg(0, "kRewardItemType_Concept not handled yet.");            
            }
        xcase kRewardItemType_Proficiency:
            {
                ProficiencyItem const *p = proficiency_GetItemById( id );
                res = p ? p->name : NULL;
            }
        xcase kRewardItemType_DetailRecipe:
            {
                DetailRecipe const *r;

                r = detailrecipedict_RecipeFromId( id );
                res = r ? r->pchName : NULL;
            }
        xcase kRewardItemType_Detail:
            {
                assertmsg(0, "kRewardItemType_Detail not handled yet.");
            }
        xcase kRewardItemType_Token:
            {
                assertmsg(0, "kRewardItemType_Token not handled yet.");
            }
        xcase kRewardItemType_IncarnatePoints:
            {
                assertmsg(0, "kRewardItemType_IncarnatePoints not handled yet.");
            }
        xcase kRewardItemType_AccountProduct:
            {
                assertmsg(0, "kRewardItemType_AccountProduct not handled yet.");
            }
        xdefault:
            {
                assertmsg(0, "Invalid RewardItemType.");
            }
        };
    }

    // --------------------
    // finally
    
    return res;
}

char *rewarditemtype_Str(RewardItemType t)
{
    static char *ss[] = 
        {
            "kRewardItemType_Power",
            "kRewardItemType_Salvage",
            "kRewardItemType_Concept",
            "kRewardItemType_Proficiency",
            "kRewardItemType_DetailRecipe",
            "kRewardItemType_Detail",
            "kRewardItemType_Token",
            "kRewardItemType_RewardTable",
            "kRewardItemType_RewardTokenCount",
            "kRewardItemType_IncarnatePoints",
            "kRewardItemType_AccountProduct",
            "kRewardItemType_Count"
        };
    return AINRANGE( t, ss ) ? ss[t] : "<invalid rewarditemtype>";
}
