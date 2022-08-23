/***************************************************************************
 *     Copyright (c) 2004-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include <utilitieslib/network/net_packet.h>
#include <utilitieslib/network/net_packetutil.h>
#include <utilitieslib/components/earray.h>
#include "entity/entity.h"
#include "entity/entPlayer.h"

#include "player/badges.h"
#include "UI/uiBadges.h"

#include "player/badges_client.h"
#include "entity/Supergroup.h"
#include "player/player.h"
#include "clientcomm/clientcomm.h"
#include <utilitieslib/language/MessageStoreUtil.h>
#include "cmdparse/cmdgame.h"
#include <utilitieslib/components/bitfield.h>

char *g_pchBadgeText[BADGE_ENT_MAX_BADGES];
char *g_pchBadgeFilename[BADGE_ENT_MAX_BADGES];
char *g_pchBadgeButton[BADGE_ENT_MAX_BADGES];

char *g_pchSgBadgeText[BADGE_SG_MAX_BADGES];
char *g_pchSgBadgeFilename[BADGE_SG_MAX_BADGES];
char *g_pchSgBadgeButton[BADGE_SG_MAX_BADGES];

// Set to 1 and it treats all badges as known, set to 2 and it treats all
// as visible.
int g_DebugBadgeDisplayMode = 0;

void entity_ReceiveBadgeUpdate(Entity *e, Packet *pak)
{
    int i;
    int n;

    // Badge update
    //
    // 0: No badge
    // 1: badge update
    //    badge index
    //    1: owned
    //       button
    //    0: not owned
    //       visible
    //       known
    //       meter

    g_DebugBadgeDisplayMode = pktGetBitsPack(pak, 4);

    while(pktGetBits(pak, 1)==1)
    {
        int iIdx = pktGetBitsPack(pak, 6);
        const BadgeDef *badge = badge_GetBadgeByIdx(iIdx);
        if(pktGetBits(pak, 1))
        {
            // owned
            char *pchButton = (badge && badge->pchDisplayButton) ? strdup(textStd(badge->pchDisplayButton)) : NULL;
            char *ctsFilename = pktGetBits(pak, 1)?strdup(pktGetString(pak)):NULL;
            if(e && e->pl && iIdx < BADGE_ENT_MAX_BADGES)
            {
                e->pl->aiBadges[iIdx] = BADGE_FULL_OWNERSHIP;

                if(g_pchBadgeText[iIdx]!=NULL)
                    free(g_pchBadgeText[iIdx]);
                g_pchBadgeText[iIdx] = strdup(textStd(badge ? badge->pchDisplayText[ENT_IS_VILLAIN(e) ?  1 : 0] : "UnknownBadgeText"));

                if(g_pchBadgeButton[iIdx]!=NULL)
                    free(g_pchBadgeButton[iIdx]);
                g_pchBadgeButton[iIdx] = pchButton;

                free(g_pchBadgeFilename[iIdx]);
                g_pchBadgeFilename[iIdx] = ctsFilename;
            }
            else
            {
                free(pchButton);
                free(ctsFilename);
            }
        }
        else
        {
            int iVisible = pktGetBits(pak, 1);
            int iKnown = pktGetBits(pak, 1);
            int iMeter = pktGetIfSetBits(pak, 20);
            char *ctsFilename = pktGetBits(pak, 1)?strdup(pktGetString(pak)):NULL;

            if(e && e->pl && iIdx < BADGE_ENT_MAX_BADGES)
            {
                e->pl->aiBadges[iIdx] = 0;
                if(iVisible)
                    BADGE_SET_VISIBLE(e->pl->aiBadges[iIdx]);
                if(iKnown)
                    BADGE_SET_KNOWN(e->pl->aiBadges[iIdx]);

                BADGE_SET_COMPLETION(e->pl->aiBadges[iIdx], iMeter);

                if(g_pchBadgeText[iIdx]!=NULL)
                    free(g_pchBadgeText[iIdx]);
                g_pchBadgeText[iIdx] = strdup(textStd((iKnown && badge) ? badge->pchDisplayHint[ENT_IS_VILLAIN(e) ?  1 : 0] : "UnknownBadgeText"));

                if(g_pchBadgeButton[iIdx]!=NULL)
                {
                    free(g_pchBadgeButton[iIdx]);
                    g_pchBadgeButton[iIdx] = NULL;
                }

                free(g_pchBadgeFilename[iIdx]);
                g_pchBadgeFilename[iIdx] = ctsFilename;
            }
            else
            {
                free(ctsFilename);
            }
        }

        badgeReparse();
    }

    if(pktGetBits(pak,1))
    {
        // how deep is the currently-stored history?
        n = pktGetBits(pak,8);

        for(i=0; i<n; i++)
        {
            e->pl->recentBadges[i].idx = pktGetBits(pak,16);
            e->pl->recentBadges[i].timeAwarded = pktGetBits(pak,32);
        }

        // clear out any old data just in case the history has shrunk
        for(i=n; i<BADGE_ENT_RECENT_BADGES; i++)
        {
            e->pl->recentBadges[i].idx = 0;
            e->pl->recentBadges[i].timeAwarded = 0;
        }
    }

    if( pktGetBits(pak,1) )
    {
        // create the stats
        if( e->supergroup )
        {
            if( !e->supergroup->badgeStates.eaiStates )
            {
                eaiCreate( &e->supergroup->badgeStates.eaiStates );
            }

            // set size
            if( eaiSize(&e->supergroup->badgeStates.eaiStates) < BADGE_SG_MAX_STATS )
            {
                eaiSetSize( &e->supergroup->badgeStates.eaiStates, BADGE_SG_MAX_STATS );
            }
        }

        // get them
        while(pktGetBits(pak, 1)==1)
        {
            int iIdx = pktGetBitsPack(pak, 6);
            const BadgeDef* sgBadge = badge_GetSgroupBadgeByIdx(iIdx);
            if(pktGetBits(pak, 1))
            {
                // owned
                char *pchButton = (sgBadge->pchDisplayButton && sgBadge) ? strdup(textStd(sgBadge->pchDisplayButton)) : NULL;
                char *ctsFilename = pktGetBits(pak, 1)?strdup(pktGetString(pak)):NULL;

                if(e && e->supergroup && iIdx < BADGE_SG_MAX_BADGES)
                {
                    e->supergroup->badgeStates.eaiStates[iIdx] = BADGE_FULL_OWNERSHIP;

                    if(g_pchSgBadgeText[iIdx]!=NULL)
                        free(g_pchSgBadgeText[iIdx]);
                    g_pchSgBadgeText[iIdx] = strdup(textStd(sgBadge ? sgBadge->pchDisplayText[0] : "UnknownBadgeText"));

                    if(g_pchSgBadgeButton[iIdx]!=NULL)
                        free(g_pchSgBadgeButton[iIdx]);
                    g_pchSgBadgeButton[iIdx] = pchButton;

                    free(g_pchSgBadgeFilename[iIdx]);
                    g_pchSgBadgeFilename[iIdx] = ctsFilename;
                }
                else
                {
                    free(pchButton);
                    free(ctsFilename);
                }
            }
            else
            {
                int iVisible = pktGetBits(pak, 1);
                int iKnown = pktGetBits(pak, 1);
                int iMeter = pktGetIfSetBits(pak, 20);
                char *ctsFilename = pktGetBits(pak, 1)?strdup(pktGetString(pak)):NULL;

                // fill 
                if(e && e->supergroup && iIdx < BADGE_SG_MAX_BADGES)
                {
                    e->supergroup->badgeStates.eaiStates[iIdx] = 0;
                    if(iVisible)
                        BADGE_SET_VISIBLE(e->supergroup->badgeStates.eaiStates[iIdx]);
                    if(iKnown)
                        BADGE_SET_KNOWN(e->supergroup->badgeStates.eaiStates[iIdx]);

                    BADGE_SET_COMPLETION(e->supergroup->badgeStates.eaiStates[iIdx], iMeter);

                    if(g_pchSgBadgeText[iIdx]!=NULL)
                        free(g_pchSgBadgeText[iIdx]);
                    g_pchSgBadgeText[iIdx] = strdup(textStd((iKnown && sgBadge) ? sgBadge->pchDisplayHint[ENT_IS_VILLAIN(e) ?  1 : 0] : "UnknownBadgeText"));

                    if(g_pchSgBadgeButton[iIdx]!=NULL)
                    {
                        free(g_pchSgBadgeButton[iIdx]);
                        g_pchSgBadgeButton[iIdx] = NULL;
                    }

                    free(g_pchSgBadgeFilename[iIdx]);
                    g_pchSgBadgeFilename[iIdx] = ctsFilename;
                }
                else
                {
                    free(ctsFilename);
                }
            }

            badgeReparse();
        }        
    }
}

void badgeMonitor_ReceiveFromServer(Packet *pak)
{
    if(pak)
    {
        badgeMonitor_ReceiveInfo(playerPtr(), pak);
    }
}

void badgeMonitor_SendToServer(Entity *e)
{
    if(e && e->pl)
    {
        START_INPUT_PACKET(pak, CLIENTINP_UPDATE_BADGE_MONITOR_LIST);
        badgeMonitor_SendInfo(e, pak);
        END_INPUT_PACKET;
    }
}

/* End of File */
