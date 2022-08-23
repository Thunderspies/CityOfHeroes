/***************************************************************************
 *     Copyright (c) 2005-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include "entity/RewardToken.h"
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/components/StringCache.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/components/MemoryPool.h>
#include "entity/entity.h"
#include "entity/entPlayer.h"
#include "entity/Supergroup.h"

MP_DEFINE(RewardToken);
RewardToken* rewardtoken_Create(void)
{
    RewardToken *res = NULL;

    // create the pool, arbitrary number
    MP_CREATE(RewardToken, 64);
    res = MP_ALLOC( RewardToken );
    if( verify( res ))
    {
        //...
    }

    // --------------------
    // finally, return

    return res;
}

RewardToken *rewardtoken_Copy(RewardToken *srcToken)
{
    RewardToken *destToken = rewardtoken_Create();

    destToken->reward = allocAddString((char*) srcToken->reward);
    destToken->val = srcToken->val;
    destToken->timer = srcToken->timer;

    return destToken;
}

void rewardtoken_Destroy( RewardToken *item )
{
    MP_FREE(RewardToken, item);
}


static int rewardtoken_NameEq(RewardToken *prt, char const *rewardtokenPiece)
{
    return verify(prt && prt->reward && rewardtokenPiece) ? stricmp(prt->reward, rewardtokenPiece)==0 : 0;
}

int rewardtoken_Award(RewardToken ***hTokens, char const * rewardtokenpiece, int val)
{
    return rewardtoken_AwardEx(hTokens, rewardtokenpiece, val, true);
}

int rewardtoken_AwardEx(RewardToken ***hTokens, char const * rewardtokenpiece, int val, bool updateTime)
{
    int res = -1;
    
    if(verify(hTokens && rewardtokenpiece))
    {
        RewardToken *rewardtokenToken = NULL;

        if(!EAINRANGE( (res = rewardtoken_IdxFromName( hTokens, rewardtokenpiece )), (*hTokens)))
        {
            rewardtokenToken = rewardtoken_Create();
            rewardtokenToken->reward = allocAddString((char*)rewardtokenpiece);
            rewardtokenToken->val = val;             
            res = eaPush(hTokens, rewardtokenToken);
        }
        else
        {
            rewardtokenToken = (*hTokens)[res];
        }

        // time always overridden if this is called
        if(updateTime && rewardtokenToken)
            rewardtokenToken->timer = timerSecondsSince2000();
    }

    // --------------------
    // finally

    return res;
}

bool rewardtoken_Unaward( RewardToken ***hTokens, char const * rewardtokenPiece)
{
    bool res = false;
    
    if( verify( hTokens && rewardtokenPiece ))
    {
        int i = rewardtoken_IdxFromName(hTokens, rewardtokenPiece);
        if( EAINRANGE( i, *hTokens ))
        {
            RewardToken *token = eaRemove(hTokens, i);
            res = token != NULL;
            rewardtoken_Destroy(token);
        }
    }

    // --------------------
    // finally

    return res; 
}

int rewardtoken_IdxFromName( RewardToken ***hTokens, char const *rewardtokenPiece)
{
    int res = -1;
    int i;

    if( verify( hTokens && rewardtokenPiece ))
    {    
        for( i = eaSize( hTokens ) - 1; i >= 0; --i)
        {
            if( rewardtoken_NameEq( (*hTokens)[i], rewardtokenPiece ) )
            {
                res = i;
            }
        }
    }
    
    // -------------------`-
    // finally

    return res;
}



/**********************************************************************func*
* getRewardToken
*
*/
RewardToken * getRewardToken( Entity * e, const char * rewardTokenName )
{
    RewardToken * rewardToken = 0;

    if( e && e->pl && rewardTokenName )
    {
        int i = rewardtoken_IdxFromName(&e->pl->rewardTokens, rewardTokenName );
        if( EAINRANGE( i, e->pl->rewardTokens))
        {
            rewardToken = e->pl->rewardTokens[i];
        }
        else if (EAINRANGE((i = rewardtoken_IdxFromName(&e->pl->activePlayerRewardTokens, rewardTokenName)), e->pl->activePlayerRewardTokens))
        {
            rewardToken = e->pl->activePlayerRewardTokens[i];
        }
        else if( e->supergroup )
        {
            i = rewardtoken_IdxFromName(&e->supergroup->rewardTokens, rewardTokenName );
            rewardToken = EAINRANGE( i, e->supergroup->rewardTokens) ? e->supergroup->rewardTokens[i] : NULL;
        }
    }
    return rewardToken;
}