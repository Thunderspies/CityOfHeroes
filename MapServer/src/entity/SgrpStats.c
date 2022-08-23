/***************************************************************************
 *     Copyright (c) 2005-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include "SgrpStats.h"
#include <utilitieslib/utils/error.h>
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

// for rewards
#include "entity/entity.h"
#include "entity/entPlayer.h"
#include "SgrpServer.h"
#include "entity/Supergroup.h"
#include <utilitieslib/utils/log.h>

#if STATSERVER
#include "statdb.h"
#include "statserver.h"
#endif

// ------------------------------------------------------------
// sgrp stats aggregation

typedef struct SgrpStatAdj
{
//    int sgrpId;
//    char const *name;
    int adj;
} SgrpStatAdj;

MP_DEFINE(SgrpStatAdj);
static SgrpStatAdj* sgrpstatadj_Create( int adj )
{
    SgrpStatAdj *res = NULL;

    // create the pool, arbitrary number
    MP_CREATE(SgrpStatAdj, 64);
    res = MP_ALLOC( SgrpStatAdj );
    if( verify( res ))
    {
        res->adj = adj;
    }

    // --------------------
    // finally, return

    return res;
}

static void sgrpstatadj_Destroy( SgrpStatAdj *hItem )
{
    if(hItem)
    {
        MP_FREE(SgrpStatAdj, hItem);
    }
}

// ----------------------------------------
// the link, and accumulated stats
// a hash by supergroup id
// of a hash by stat name
// of SgrpStatAdj objects
static struct SgrpStatInfo
{
    StashTable adjHashFromSgrpId;
    bool changed;
} s_stats = {0};


//------------------------------------------------------------
//  queue up stats to send to stat sever (via db server)
//----------------------------------------------------------
bool sgrpstats_StatAdj(int idSgrp, char const *stat, int adjAmt )
{
    bool res = false;
    
    if( !s_stats.adjHashFromSgrpId )
    {
        s_stats.adjHashFromSgrpId = stashTableCreateInt(1000);
        
    }

    if( verify( idSgrp > 0 && stat ) && stashFindPointerReturnPointer( g_BadgeStatsSgroup.idxStatFromName, stat ) )
    {
        StashTable hashAcumdAdjs;
        if( !stashIntFindPointer(s_stats.adjHashFromSgrpId, idSgrp, &hashAcumdAdjs) )
        {
            hashAcumdAdjs = stashTableCreateWithStringKeys( 128, StashDeepCopyKeys );
            stashIntAddPointer( s_stats.adjHashFromSgrpId, idSgrp, hashAcumdAdjs, false);
        }

        if( verify( hashAcumdAdjs ))
        {
            SgrpStatAdj *p = stashFindPointerReturnPointer( hashAcumdAdjs, stat );
            if( !p )
            {
                stashAddPointer( hashAcumdAdjs, stat, sgrpstatadj_Create( adjAmt ), false);
            }
            else
            { 
                p->adj += adjAmt;
            }
            
            res = true;
        }

        s_stats.changed = true;
    }

    // ----------
    // finally

    return res;
}


//------------------------------------------------------------
//  send the queued stats
//----------------------------------------------------------


//------------------------------------------------------------
// @see dbserver/statservercomm.c:sgrpstats_RelayAdjs when making changes
//----------------------------------------------------------
void sgrpstats_SendQueued(Packet *pak)
{
    if( verify(pak) )
    {
        StashTableIterator hiIdSgs = {0};
        StashElement elem;
        int nIds = 0;
        int iNumSent;

        // ----------
        // count the number of supergroups with ids to send

        for(stashGetIterator( s_stats.adjHashFromSgrpId,&hiIdSgs); stashGetNextElement(&hiIdSgs, &elem);)
        {
            StashTable hashAdjs = cpp_reinterpret_cast(StashTable)(stashElementGetPointer(elem));
            if(verify(hashAdjs))
            {
                if( stashGetValidElementCount(hashAdjs)>0 )
                {
                    nIds++;
                }
            }
            else
            {
                LOG( LOG_STATSERVER, LOG_LEVEL_VERBOSE, 0, __FUNCTION__": verify failed: sgid %d has no hashtable", stashElementGetIntKey(elem)); 
            }

        }

        // ----------
        // send the info

        // send the number of ids
        pktSendBitsPack( pak, SGRPSTATS_NIDS_PACKBITS, nIds); 

        // send each hash
        for(iNumSent = 0, stashGetIterator( s_stats.adjHashFromSgrpId, &hiIdSgs ); (stashGetNextElement(&hiIdSgs, &elem));)
        {
            StashTable hashAdjs = cpp_reinterpret_cast(StashTable)(stashElementGetPointer(elem));
            StashTableIterator iHashAdjs = {0};
            StashElement helem;
            int idSgrp = stashElementGetIntKey(elem);
            int nAdjs = stashGetValidElementCount( hashAdjs );
            int j;
            
            if( nAdjs > 0 )
            {
                pktSendBitsPack( pak, SGRPSTATS_IDSGRP_PACKBITS, idSgrp); 
                pktSendBitsPack( pak, SGRPSTATS_NADJS_PACKBITS, nAdjs); 
                for( j = 0, stashGetIterator( hashAdjs, &iHashAdjs); (stashGetNextElement(&iHashAdjs, &helem)); ++j ) 
                {
                    const char *stat = stashElementGetStringKey( helem );
                    SgrpStatAdj *adj = stashElementGetPointer( helem );
                    int idStat = (int)(intptr_t)stashFindPointerReturnPointer( g_BadgeStatsSgroup.idxStatFromName, stat );
                    int adjAmt = verify(adj) ? adj->adj : 0;
                    
                    pktSendBitsPack( pak, SGRPSTATS_IDSTAT_PACKBITS, idStat); 
                    pktSendBitsPack( pak, SGRPSTATS_ADJAMT_PACKBITS, adjAmt);
                }
                
                // finally, clean the hash table, increment the count
                stashTableClearEx(  hashAdjs, NULL, sgrpstatadj_Destroy );
                iNumSent++;
            }
        }
        assert(iNumSent == nIds);
    }
}



//------------------------------------------------------------
//  Does not clear any existing stats
//----------------------------------------------------------
void sgrpstats_ReceiveToQueue(Packet *pak)
{
    int nIds = pktGetBitsPack( pak, SGRPSTATS_NIDS_PACKBITS); 
    int i;
    for( i = 0; i < nIds; ++i ) 
    {
        int idSgrp = pktGetBitsPack( pak, SGRPSTATS_IDSGRP_PACKBITS); 
        int nAdjs = pktGetBitsPack( pak, SGRPSTATS_NADJS_PACKBITS); 
        int j;
        
        for( j = 0; j < nAdjs; ++j ) 
        {
            int idStat = pktGetBitsPack( pak, SGRPSTATS_IDSTAT_PACKBITS); 
            int adjAmt = pktGetBitsPack( pak, SGRPSTATS_ADJAMT_PACKBITS);
            char *stat;
            if ( !stashIntFindPointer( g_BadgeStatsSgroup.statnameFromId, idStat, &stat ) )
                stat = NULL;

            sgrpstats_StatAdj( idSgrp, stat, adjAmt );
        }
    }
}


void sgrpstats_FlushToDb(NetLink *link)
{
    if( verify( link ) && s_stats.changed )
    {
        Packet* pak = pktCreateEx(link,DBCLIENT_SGRP_STATSADJ);
        sgrpstats_SendQueued(pak);
        pktSend( &pak, link );
        s_stats.changed = false;
    }
}


#if STATSERVER
//------------------------------------------------------------
//  Flush the accumulated stats to the stats hashtable
//----------------------------------------------------------
void sgrpstats_FlushToSgrps()
{
    StashTableIterator iSgrpIds = {0};
    StashElement elem;
    int nIds = stashGetValidElementCount( s_stats.adjHashFromSgrpId );
    int i;        

    for(i = 0, stashGetIterator( s_stats.adjHashFromSgrpId, &iSgrpIds ); (stashGetNextElement(&iSgrpIds, &elem)); ++i)
    {
        StashTable hashAdjs = cpp_reinterpret_cast(StashTable)(stashElementGetPointer(elem));
        StashTableIterator iHashAdjs = {0};
        StashElement helem;
        int idSgrp = stashElementGetIntKey(elem);
        Supergroup *sg = stat_sgrpFromIdSgrp(idSgrp, true);

        for( stashGetIterator( hashAdjs, &iHashAdjs); sg && stashGetNextElement(&iHashAdjs, &helem);) 
        {
            const char *stat = stashElementGetStringKey( helem );
            SgrpStatAdj *adj = stashElementGetPointer( helem );
            int adjAmt = adj ? adj->adj : 0;

            sgrp_BadgeStatAdd( sg, stat, adjAmt );
        }

        // ----------
        // mark these stats as dirty

        if( stashGetValidElementCount( hashAdjs ) > 0 )
        {
            statdb_SetSgStatDirty( idSgrp );
        }

        // ----------
        // finally, cleanup the hash table

        stashTableClearEx( hashAdjs, NULL, sgrpstatadj_Destroy );
    }
    verify(i == nIds);
    s_stats.changed = false;
} 
#endif // STATSERVER



//------------------------------------------------------------
//  check if sgrp stats have changed
//----------------------------------------------------------
bool sgrpstats_Changed()
{
    return s_stats.changed;
}


//------------------------------------------------------------
//  set the changed state
//----------------------------------------------------------
void sgrpstats_SetChanged(bool changed)
{
    s_stats.changed = changed;
}

