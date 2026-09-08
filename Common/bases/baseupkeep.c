/***************************************************************************
 *     Copyright (c) 2005-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include "bases/baseupkeep.h"
#include "entity/Supergroup.h"
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/assert/assert.h>
#include <utilitieslib/utils/error.h>
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/components/MemoryPool.h>
#include <utilitieslib/components/StashTable.h>
#include <utilitieslib/utils/textparser.h>

#if SERVER || STATSERVER
#include "dbcomm/dbcomm.h"
#endif // SERVER || STATSERVER

static TokenizerParseInfo ParseRentRange[] =
{
    { "RentMax",    TOK_STRUCTPARAM | TOK_INT(RentRange, nRentMax, 0)},
    { "Rate",        TOK_STRUCTPARAM | TOK_F32(RentRange, taxRate, 0)},
    {    "\n",        TOK_END,                            0                },
    { "", 0, 0 }
};

static TokenizerParseInfo ParseBaseUpkeepPeriod[] =
{
    { "{",              TOK_START,                        0},
    { "Period",         TOK_STRUCTPARAM | TOK_INT(BaseUpkeepPeriod, period,0),          },
    { "ShutBaseDown",   TOK_BOOLFLAG(BaseUpkeepPeriod, shutBaseDown,0),          },
    { "DenyBaseEntry",  TOK_BOOLFLAG(BaseUpkeepPeriod, denyBaseEntry,0),          },

    { "}",              TOK_END,      0 },
    { "", 0, 0 }
};


TokenizerParseInfo ParseBaseUpkeep[] = 
{
    { "filename",                TOK_CURRENTFILE(BaseUpkeep,filenameData)},
    { "RentPeriodSeconds",        TOK_INT(BaseUpkeep,periodRent,0)},
    { "RentRange",                TOK_STRUCT(BaseUpkeep, ppRanges,ParseRentRange)},
    { "RentDueDateResetPeriod", TOK_INT(BaseUpkeep, periodResetRentDue,0)},
    { "LatePeriod",                TOK_STRUCT(BaseUpkeep, ppPeriods, ParseBaseUpkeepPeriod) } ,
    { "", 0, 0 }
};

SHARED_MEMORY BaseUpkeep g_baseUpkeep;

F32 baseupkeep_PctFromPrestige(int prestige)
{
    F32 rateRes = 0.f;
    int i;
    int n = eaSize(&g_baseUpkeep.ppRanges);
    const RentRange *rHi = NULL;
    const RentRange *rLo = NULL;
    
    for( i = 0; i < n; ++i ) 
    {
        rHi = g_baseUpkeep.ppRanges[i];
        if( verify(rHi) && rHi->nRentMax > prestige )
        {
            break;
        }
        rLo = rHi;
    }

    // --------------------
    // lerp the tax rate

    if(verify(rHi))
    {
        if(i == n)
        {
            // special case: tax rate capped at highest
            // (alternative, extrapolate from n-1 and n-2?)
            rateRes = rHi->taxRate;
        }
        else
        {
            F32 taxLo = rLo ? rLo->taxRate : 0.f;
            F32 taxHi = rHi->taxRate;
            
            F32 rentLo = rLo ? rLo->nRentMax : 0.f;
            F32 rentHi = rHi->nRentMax;
            
            F32 dTax = taxHi - taxLo;
            F32 dRent = rentHi - rentLo;
            F32 t = (prestige - rentLo)/dRent; // % of rent
            rateRes = t*dTax + taxLo;
        }
    }
    
    return rateRes;
}


/***************************************************************************/
/***************************************************************************/
/***************************************************************************/

//----------------------------------------
//  check the parsed data
//----------------------------------------
static bool UpkeepDictFinalize(ParseTable* pti, void* structptr)
{
    BaseUpkeep * pUpkeepDict = (BaseUpkeep *)structptr;
    int i;
    int n;
    
    if(!pUpkeepDict)
    {
        return false;                //BREAK IN FLOW 
    }    
    
    // ----------
    // test range
    
    if( pUpkeepDict->periodRent <= 0 )
    {
        ErrorFilenamef(pUpkeepDict->filenameData,"rent %d period must be greater than 0", pUpkeepDict->periodRent );
        return false;
    }
    
    // --------------------
    // test rents
    
    n = eaSize(&pUpkeepDict->ppRanges);
    for(i = 0; i < n-2; ++i) // skip last value
    {
        const RentRange *r = pUpkeepDict->ppRanges[i];
        const RentRange *rNext = pUpkeepDict->ppRanges[i+1];
        if(r && rNext && 
            (r->nRentMax < 0
            || rNext->nRentMax <= r->nRentMax ))
        {
            ErrorFilenamef(pUpkeepDict->filenameData, "rent range %d is not greater than %d, or < 0", rNext->nRentMax, r->nRentMax);
            return false;
        }
        
    }
    
    return true;
}

//----------------------------------------
//  load the base upkeep data
//----------------------------------------
void baseupkeep_Load()
{
    ParserLoadFilesShared("SM_Upkeeps", NULL, "defs/v_bases/baseupkeep.def", "baseupkeep.bin",
                          0, ParseBaseUpkeep, &g_baseUpkeep, sizeof(BaseUpkeep),
                          NULL, NULL, NULL, UpkeepDictFinalize, NULL);
}

// *********************************************************************************
// upkeep periods
// *********************************************************************************

#if SERVER || STATSERVER
//----------------------------------------
//  the number of periods late in rent
// returns the number of periods late, or zero if not late
//----------------------------------------
int sgroup_nUpkeepPeriodsLate(Supergroup *sg)
{
    int res = 0;
    if(verify( sg && g_baseUpkeep.periodRent > 0))
    {
        U32 time = dbSecondsSince2000();
        S32 dt = (S32)time - (S32)sg->upkeep.secsRentDueDate;
        res = sg->upkeep.secsRentDueDate == 0 ? 0 : dt/g_baseUpkeep.periodRent;
    }
    return MAX(res,0); // zero is the min, just trim it if this is negative.
}
#endif // SERVER

#if SERVER || STATSERVER
//----------------------------------------
//  get the period that the supergroup currently is in   
//----------------------------------------
const BaseUpkeepPeriod *sgroup_GetUpkeepPeriod(Supergroup *sg)
{
    if(sg && devassert(eaSize(&g_baseUpkeep.ppPeriods)))
    {
        int p = sgroup_nUpkeepPeriodsLate( sg );
        p = MINMAX(p, 0, eaSize(&g_baseUpkeep.ppPeriods) - 1);
        return g_baseUpkeep.ppPeriods[p];
    }
    return NULL;
}
#endif // SERVER

/***************************************************************************/
/***************************************************************************/
/***************************************************************************/


