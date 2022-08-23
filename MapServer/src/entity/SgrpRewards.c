/***************************************************************************
 *     Copyright (c) 2005-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include <utilitieslib/stdtypes.h>
#include "SgrpStats.h"
#include <utilitieslib/utils/file.h>
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/components/MemoryPool.h>
#include <utilitieslib/components/StashTable.h>
#include <utilitieslib/assert/assert.h>
#include <utilitieslib/components/MemoryPool.h>
#include <utilitieslib/network/netio.h>
#include "comm_backend.h"
#include "SgrpBadges.h"
#include "player/BadgeStats.h"
#include <utilitieslib/utils/textparser.h>

#include "entity/entity.h"
#include "entity/entPlayer.h"
#include "SgrpServer.h"
#include "entity/Supergroup.h"
#include "entity/character_base.h"
#include "dbcomm/dbcomm.h"
#include "dbcomm/logcomm.h"

//----------------------------------------
//  info about a queued reward
//----------------------------------------
typedef struct QueuedSgrpReward
{
    int dPrestigeAmt;
} QueuedSgrpReward;


//----------------------------------------
//  rewards to apply to the supergroup
//----------------------------------------
static struct SgrpRewardInfo
{
    StashTable prestigeFromSgrpId;
     bool changed;
} s_rewards = {0};


MP_DEFINE(QueuedSgrpReward);
static QueuedSgrpReward* queuedsgrpreward_Create(int adjPrestigeAmt )
{
    QueuedSgrpReward *res = NULL;

    // create the pool, arbitrary number
    MP_CREATE(QueuedSgrpReward, 64);
    res = MP_ALLOC( QueuedSgrpReward );
    if( verify( res ))
    {
        res->dPrestigeAmt = adjPrestigeAmt;
    }
    // --------------------
    // finally, return

    return res;
}

static void queuedsgrpreward_Destroy( QueuedSgrpReward *hItem )
{
    if(hItem)
    {
        MP_FREE(QueuedSgrpReward, hItem);
    }
}


void sgrprewards_PrestigeAdj(Entity *e, int adjAmt )
{
    if( !s_rewards.prestigeFromSgrpId )
    {
        s_rewards.prestigeFromSgrpId = stashTableCreateInt(1000);

    }

    if( verify( e && e->pl )
        && e->pl->supergroup_mode && verify( e->supergroup ))
    {
        int idSgrp = e->supergroup_id;
        QueuedSgrpReward *rew;
        
        if (!stashIntFindPointer(s_rewards.prestigeFromSgrpId, idSgrp, &rew))
            rew = NULL;

        // track cummulative on player
        e->pl->prestige += adjAmt;
        
        // --------------------
        // queue the award itself

        if( rew )
        {
            rew->dPrestigeAmt += adjAmt;
        }
        else
        {
            rew = queuedsgrpreward_Create(adjAmt);
            stashIntAddPointer(s_rewards.prestigeFromSgrpId, idSgrp, rew, false);
        }

        // --------------------
        // finally

        s_rewards.changed = true;
    }
}


//------------------------------------------------------------
//  lock and apply any pending rewards
//----------------------------------------------------------
#define SECS_SGRP_REWARD_UPDATE_INTERVAL (60*10)
void sgrprewards_Tick(bool flush)
{
    static U32 s_secsNextUpdate = 0;
    
    if( s_rewards.changed && (s_secsNextUpdate < dbSecondsSince2000() || flush) )
    {
        StashTableIterator iSgrpIds = {0};
        StashElement elem;
        int nIds = stashGetValidElementCount( s_rewards.prestigeFromSgrpId );
        int i;

        for(i = 0, stashGetIterator( s_rewards.prestigeFromSgrpId, &iSgrpIds ); stashGetNextElement(&iSgrpIds, &elem); ++i)
        {
            int idSgrp = stashElementGetIntKey(elem);
            QueuedSgrpReward *rew = stashElementGetPointer(elem);
            Supergroup sg = {0};

            // --------------------
            // lock and update the sgrp

            if( rew && rew->dPrestigeAmt != 0
                && sgroup_LockAndLoad( idSgrp, &sg ))
            {
                sg.prestige += rew->dPrestigeAmt;
                if (sg.prestige > MAX_INFLUENCE) //cap prestige
                {
                    sg.prestige = MAX_INFLUENCE;
                }
                
                sgroup_UpdateAllMembersAnywhere(&sg);
                sgroup_UpdateUnlock( idSgrp, &sg );
                
                // --------------------
                // clean up
                
                stashIntRemovePointer(s_rewards.prestigeFromSgrpId, idSgrp, NULL);
                queuedsgrpreward_Destroy( rew );
            }
            else
            {
                LOG_SG( sg.name, idSgrp, LOG_LEVEL_VERBOSE, 0, "SgrpRewards failed to lock sgrp %i", idSgrp);
            }
        }

        // ----------
        // finally

        // clear changed state
        s_rewards.changed = false;

        // set timer
        s_secsNextUpdate = dbSecondsSince2000() + SECS_SGRP_REWARD_UPDATE_INTERVAL;
    }
}
