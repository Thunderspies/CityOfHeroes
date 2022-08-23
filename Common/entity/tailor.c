#include "costume.h"
#include "power_customization.h"
#include <utilitieslib/network/net_packet.h>
#include "entPlayer.h"
#include "entity.h"
#include "character_inventory.h"
#include "gameComm/svr_chat.h"
#include "language/langServerUtil.h"
#include "player/badges_server.h"
#include "dbcomm/logcomm.h"

int processTailorCost(Packet *pak, Entity *e, int genderChange, Costume *costume, PowerCustomizationList *powerCustList)
{
    int hasDiscountSalvage;
    int hasVetReward;
    const SalvageItem * salvageItem;
    int useFreeTailor = pktGetBits(pak,2);
    int useDayJobCoupon = useFreeTailor >> 1 & 1;
    int costumeCost = 0;
    int powerCustCost = 0;
    int total = 0;

    useFreeTailor = useFreeTailor & 1;

    // Pass, deduct influence
    if( useFreeTailor && ! e->pl->freeTailorSessions )
    {
        costume_destroy(costume);
        powerCustList_destroy(powerCustList);
        return 0;
    }


    salvageItem = salvage_GetItem(TAILOR_SALVAGE_TOKEN_NAME);
    hasDiscountSalvage = character_CanAdjustSalvage(e->pchar, salvageItem, -1);
    hasVetReward = getRewardToken(e, TAILOR_VET_REWARD_TOKEN_NAME) ? 1 : 0;

    if( useDayJobCoupon && !hasDiscountSalvage )
    {
        costume_destroy(costume);
        powerCustList_destroy(powerCustList);
        return 0;
    }


    if( useFreeTailor )
    {
        e->pl->freeTailorSessions--;
        chatSendToPlayer( e->db_id, localizedPrintf(e,"FreeSessionsRemaining",e->pl->freeTailorSessions), INFO_SVR_COM, 0 );
    }
    else
    {
        costumeCost = tailor_CalcTotalCost(e, costume_as_const(costume), genderChange);
        powerCustCost = powerCust_GetTotalCost(e, e->pl->powerCustomizationLists[e->pl->current_powerCust], powerCustList);
        total = costumeCost + powerCustCost;
        if (hasVetReward)
        {
            total = (total * ((100 - TAILOR_VET_REWARD_DISCOUNT_PERCENTAGE) / 100.f));
        } 
        else if (useDayJobCoupon)
        {
            total = (total * ((100 - TAILOR_SALVAGE_DISCOUNT_PERCENTAGE) / 100.f));
        }

        // more expensive costume than they can afford.
        //    or less than 0
        if((total > e->pchar->iInfluencePoints ) || (total < 0))
        {
            costume_destroy(costume);
            powerCustList_destroy(powerCustList);
            return 0;
        }

        if (!hasVetReward && useDayJobCoupon)
        {
            character_AdjustSalvage(e->pchar, salvageItem, -1, "tailor", false );
        }

        // We're subtracting money from the player but adding to the stat so
        // we have to do it separately.
        ent_AdjInfluence(e, -total, NULL);
        badge_StatAdd(e, "inf.costume", total);
    }
    LOG_ENT(e, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, "Costume:Tailor Spent Influence: %d", total);
    return 1;
}
