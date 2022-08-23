/***************************************************************************
 *     Copyright (c) 2005-2005, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 *
 ***************************************************************************/
#include "bases/basestorage.h"
#include <utilitieslib/utils/textparser.h>
#include <utilitieslib/network/net_packetutil.h>
#include <utilitieslib/network/net_packet.h>
#include "entity/salvage.h"
#include "entity/character_inventory.h"
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/assert/assert.h>
#include <utilitieslib/utils/error.h>
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/components/EString.h>
#include <utilitieslib/components/MemoryPool.h>
#include "bases/bases.h"
#include "bases/basedata.h"
#include "entity/entity.h"
#include "entity/character_base.h"
#include "entity/Supergroup.h"
#include "entity/powers.h"
#include "bases/DetailRecipe.h"

#if SERVER
#include "bases/basetogroup.h"
#include "dbcomm/logcomm.h"
#include "language/langServerUtil.h"
#include <utilitieslib/utils/timing.h>
#include "dbcomm/dbcomm.h"
#include "gameSys/sgraid.h"
#include "gameSys/raidmapserver.h"
#include "entity/SgrpServer.h"

extern int world_modified;

#endif // SERVER

StoredSalvage *roomdetail_FindStoredSalvage(RoomDetail *detail, const char *nameSalvage, int *resI)
{
    int i;
    for( i = eaSize( &detail->storedSalvage ) - 1; i >= 0; --i)
    {
        StoredSalvage *stored = detail->storedSalvage[i];
        
        if( stored && stored->salvage && 0 == stricmp(nameSalvage, stored->salvage->pchName) )
        {
            if(resI)
                *resI = i;
            return stored;
        }
    }
    return NULL;
}

StoredInspiration *roomdetail_FindStoredInspiration(RoomDetail *detail, const char *pathInsp, int *resI)
{
    int i;
    for( i = eaSize( &detail->storedInspiration ) - 1; i >= 0; --i)
    {
        StoredInspiration *stored = detail->storedInspiration[i];
        
        if( stored && stored->ppow && 0 == stricmp(pathInsp, basepower_ToPath(stored->ppow)) )
        {
            if(resI)
                *resI = i;
            return stored;
        }
    }
    return NULL;
}

StoredEnhancement *roomdetail_FindStoredEnhancement(RoomDetail *detail, const char *pathBasePower, int iLevel, int iNumCombines, int *resI)
{
    int i;
    for( i = eaSize( &detail->storedEnhancement ) - 1; i >= 0; --i)
    {
        StoredEnhancement *stored = detail->storedEnhancement[i];
        Boost *b = stored && stored->enhancement ? stored->enhancement[0] : NULL;

        if( b && 0 == stricmp(pathBasePower, basepower_ToPath(b->ppowBase)) && b->iLevel == iLevel && b->iNumCombines == iNumCombines)
        {
            if(resI)
                *resI = i;
            return stored;
        }
    }
    return NULL;
}

static bool s_ItemInFuncParams(RoomDetail *detail, const char *nameItem)
{
    int i;
    const Detail *def = detail->info;

    for( i = eaSize(&def->ppchFunctionParams) - 1; i >= 0; --i)
    {
        const char *fp = def->ppchFunctionParams[i];
        if(strEndsWith(fp,"*"))
        {
            if(simpleMatch(fp,nameItem))
                break;
        }
        else if(0 == stricmp(nameItem, fp))
        {
            break;
        }  
    }
    return i >= 0;
    
}

static int s_AmountMax(const char **ppchFunctionParams)
{
    int res = 0;
    int i;
    for( i = eaSize(&ppchFunctionParams) - 1; i >= 0; --i)
    {
        const char *str = ppchFunctionParams[i];
        if(strStartsWith(str,"_MAX"))
        {
            sscanf(str,"_MAX=%i", &res);
        }
    }
    return abs(res);
}

typedef struct StoredItem
{
    void *def;
    S32 amount;
} StoredItem;
#define STOREDITEM_COMPAT(TYP) STATIC_ASSERT(OFFSETOF(TYP,amount) == OFFSETOF(StoredItem,amount))
STOREDITEM_COMPAT(StoredSalvage);
STOREDITEM_COMPAT(StoredInspiration);

static int s_AmountTotal(RoomDetail *d)
{
    int res = 0;
    int i, iLists;
    StoredItem **itemlists[] = 
        {
            (StoredItem **)d->storedSalvage,
            (StoredItem **)d->storedInspiration,
            (StoredItem **)d->storedEnhancement
        };

    for( iLists = 0; iLists < ARRAY_SIZE(itemlists); ++iLists )
    {
        StoredItem **items = itemlists[iLists];
        for( i = eaSize(&items) - 1; i >= 0; --i)
        {
            if(verify(items[i]))
                res += MAX(items[i]->amount,0);
        }
    }
    return res;
}

bool s_checkCanAdj(int amount, int amtStored, int amtMax, int amtTotal, bool inFuncParams)
{
    bool res = false;
    int amtNew = amount+amtStored;
    if(amount > 0 && !inFuncParams) // item not allowed to add
        res = false;
    else if( amtNew < 0 ) // we'll have a negative amount
        res = false;
    else if(amount > 0 && amtMax && (amount+amtTotal) > amtMax) // we'll have too many stored
        res = false;
    else
        res = true;
    return res;
}

int roomdetail_MaxCount( RoomDetail *detail)
{
    return s_AmountMax(detail->info->ppchFunctionParams);
}

int roomdetail_CurrentCount( RoomDetail *detail )
{
    return s_AmountTotal(detail);
}

bool roomdetail_CanDeleteStorage(RoomDetail *detail)
{
    int i;
    if(!detail)
        return TRUE;
    
    for( i = eaSize(&detail->storedInspiration) - 1; i >= 0; --i)
    {
        StoredInspiration *stored = detail->storedInspiration[i];
        if(stored && stored->ppow)
            return FALSE;
    }
    
    for( i = eaSize(&detail->storedEnhancement) - 1; i >= 0; --i)
    {
        StoredEnhancement *stored = detail->storedEnhancement[i];
        Boost *b = stored && stored->enhancement ? stored->enhancement[0] : NULL;
        if(b)
            return FALSE;
    }
    
    for( i = eaSize(&detail->storedSalvage) - 1; i >= 0; --i)
    {
        StoredSalvage *stored = detail->storedSalvage[i];
        if(stored && stored->salvage)
            return FALSE;
    }
    return TRUE;
}

bool roomdetail_CanAdjSalvage(RoomDetail *detail, const char *nameItem, int amount)
{
    bool res = false;
    
    if(nameItem && detail && detail->info && detail->info->eFunction == kDetailFunction_StorageSalvage )
    {
        const Detail *def = detail->info;
        int amountStored = 0;
        int amtMax;
        int amtTotal;
        bool inFuncParams = false;
        StoredSalvage *stored = roomdetail_FindStoredSalvage(detail, nameItem, NULL);
        amtMax = s_AmountMax(def->ppchFunctionParams);
        amtTotal = s_AmountTotal(detail);
        inFuncParams = s_ItemInFuncParams(detail, nameItem);
        
        if(stored)
        {
            amountStored = stored->amount;
        }
        res = s_checkCanAdj(amount, amountStored, amtMax, amtTotal, inFuncParams);
    }

    // ----------
    // finally

    return res;
}

bool roomdetail_CanAdjInspiration(RoomDetail *detail, const char *pathInsp, int amount)
{
    bool res = false;
    
    if( detail && detail->info && detail->info->eFunction == kDetailFunction_StorageInspiration && pathInsp )
    {
        const Detail *def = detail->info;
        int amountStored = 0;
        int amtMax;
        int amtTotal;
        bool inFuncParams = false;
        StoredInspiration *stored;
        stored = roomdetail_FindStoredInspiration(detail, pathInsp, NULL);
        amtMax = s_AmountMax(def->ppchFunctionParams);
        amtTotal = s_AmountTotal(detail);
        inFuncParams = s_ItemInFuncParams(detail, pathInsp);
        
        if(stored)
        {
            amountStored = stored->amount;
        }
        res = s_checkCanAdj(amount, amountStored, amtMax, amtTotal, inFuncParams);
    }

    // ----------
    // finally

    return res;
}

bool roomdetail_CanAdjEnhancement(RoomDetail *detail, const char *pathBasePower, int iLevel, int iNumCombines, int amount)
{
    bool res = false;
    
    if(detail && detail->info && detail->info->eFunction == kDetailFunction_StorageEnhancement && pathBasePower )
    {
        const Detail *def = detail->info;
        int amountStored = 0;
        int amtMax;
        int amtTotal;
        bool inFuncParams = false;
        StoredEnhancement *stored;
        stored = roomdetail_FindStoredEnhancement(detail, pathBasePower, iLevel, iNumCombines, NULL);
        amtMax = s_AmountMax(def->ppchFunctionParams);
        amtTotal = s_AmountTotal(detail);
        inFuncParams = s_ItemInFuncParams(detail, pathBasePower);
        
        if(stored)
        {
            amountStored = stored->amount;
        }
        res = s_checkCanAdj(amount, amountStored, amtMax, amtTotal, inFuncParams);
    }

    // ----------
    // finally

    return res;
}

int roomdetail_AdjSalvage(RoomDetail *detail, const char *nameSalvage, int amount)
{
    int iSS = -1;
    StoredSalvage *stored = roomdetail_FindStoredSalvage(detail, nameSalvage, &iSS);
    
    if( stored && stored->salvage )
    {
        stored->amount = MAX(stored->amount+amount,0);
        
        if( stored->amount == 0 )
        {
            storedsalvage_Destroy(stored);
            eaRemove(&detail->storedSalvage,iSS);
        }
    }
    else
    {
        StoredSalvage *sal = storedsalvage_Create();
        sal->salvage = salvage_GetItem( nameSalvage );    
        sal->amount += amount;
        if(verify(sal->salvage))
        {
            iSS = eaPush(&detail->storedSalvage, sal);
            stored = sal;
        }
        else
        {
            storedsalvage_Destroy(sal);
        }
    }

#if SERVER
    {
        BaseRoom *room = baseGetRoom(&g_base,detail->parentRoom);
        if(verify(room))
            room->mod_time++;
        world_modified=1;
        baseToDefs(&g_base, 0);
    }
#endif

    return iSS;
}

int roomdetail_SetSalvage(RoomDetail *detail, const char *nameSalvage, int amt)
{
    int adj = amt;
    StoredSalvage *stored = roomdetail_FindStoredSalvage(detail, nameSalvage, NULL);
    if(stored)
        adj = amt - stored->amount;
    return roomdetail_AdjSalvage(detail, nameSalvage, adj );
}


int roomdetail_AdjInspiration(RoomDetail *detail, char *pathInsp, int amount)
{
    int iSS = -1;
    StoredInspiration *stored = roomdetail_FindStoredInspiration(detail, pathInsp, &iSS);

    if( stored && stored->ppow )
    {
        stored->amount = MAX(stored->amount+amount,0);
        
        if( stored->amount == 0 )
        {
            storedinspiration_Destroy(stored);
            eaRemove(&detail->storedInspiration,iSS);
        }
    }
    else
    {
        StoredInspiration *sal = storedinspiration_Create();
        sal->ppow = basepower_FromPath( pathInsp );    
        sal->amount += amount;
        if(verify(sal->ppow))
        {
            iSS = eaPush(&detail->storedInspiration, sal);
        }
        else
        {
            storedinspiration_Destroy(sal);
            Errorf("couldn't find power %s for adding inspiration to base storage", pathInsp);
        }
    }

#if SERVER 
    {
        BaseRoom *room = baseGetRoom(&g_base,detail->parentRoom);
        if(verify(room))
            room->mod_time++;
        world_modified=1;
        baseToDefs(&g_base, 0);
    }
#endif 
 
    return iSS;
}
 
int roomdetail_SetInspiration(RoomDetail *detail, char *nameInspiration, int amt)
{
    int adj = amt;
    StoredInspiration *stored = roomdetail_FindStoredInspiration(detail, nameInspiration, NULL);
    if(stored)
        adj = amt - stored->amount;
    return roomdetail_AdjInspiration(detail, nameInspiration, adj );
}

int roomdetail_AdjEnhancement(RoomDetail *detail, char *pathBasePower, int iLevel, int iNumCombines, int amount)
{
    int iSS = -1;
    StoredEnhancement *stored = roomdetail_FindStoredEnhancement(detail, pathBasePower, iLevel, iNumCombines, &iSS);

    if( stored && stored->enhancement && stored->enhancement[0] )
    {
        stored->amount = MAX(stored->amount+amount,0);
        
        if( stored->amount == 0 )
        {
            storedenhancement_Destroy(stored);
            eaRemove(&detail->storedEnhancement,iSS);
        }
    }
    else
    {
        const BasePower *ppowBase = basepower_FromPath( pathBasePower );    
        if(verify(ppowBase))
        {
            StoredEnhancement *sal = storedenhancement_Create(ppowBase, iLevel, iNumCombines, amount);
            iSS = eaPush(&detail->storedEnhancement, sal);
        }
        else
        {
            Errorf("couldn't find power %s for adding enhancement to base storage", pathBasePower);
        }
    }

#if SERVER
    {
        BaseRoom *room = baseGetRoom(&g_base,detail->parentRoom);
        if(verify(room))
            room->mod_time++;
        world_modified=1;
        baseToDefs(&g_base, 0);
    }
#endif

    return iSS;
}

int roomdetail_SetEnhancement(RoomDetail *detail, char *pathBasePower, int iLevel, int iNumCombines, int amt)
{
    int adj = amt;
    StoredEnhancement *stored = roomdetail_FindStoredEnhancement(detail, pathBasePower, iLevel, iNumCombines, NULL);
    if(stored)
        adj = amt - stored->amount;
    return roomdetail_AdjEnhancement(detail, pathBasePower, iLevel, iNumCombines, adj );
}
//     // ----------
//     // finally

//     return res;
// }


#if SERVER
void ReceiveStoredSalvageAdjustFromInv(Character *p, Packet *pak)
{
    int idRoomDetail = pktGetBitsAuto(pak);
    bool isNeg = pktGetBitsPack(pak, 2);
    int amount = (isNeg?-1:1)*pktGetBitsAuto(pak);
    char *nameSalvage;
    Entity *e = (p ? p->entParent : NULL);

    strdup_alloca(nameSalvage,pktGetString(pak));

    if( !g_base.rooms )
    {
        dbLog("cheater", e, "no base active at the moment");
    }
    else if( !idRoomDetail )
    {
        dbLog("cheater", e, "invalid idRoomDetail %d", idRoomDetail);
        return;
    }
    else if( !nameSalvage )
    {
        dbLog("cheater", e, "no string sent for salvage");
        return;
    }
    else if (!e || !e->supergroup || e->supergroup_id != g_base.supergroup_id)
    {
        dbLog("cheater", e, "not in a SG or not in my SG base");
        return;
    }
    else if(!verify( p && pak ))
    {
        return;            // BREAK IN FLOW 
    }
#if SERVER
    else if( RaidIsRunning() )
    {
        return;
    }
#endif
    else
    {
        BaseRoom *room = NULL;
        RoomDetail *detail = baseGetDetail(&g_base, idRoomDetail, &room);
        SalvageItem const *sal = salvage_GetItem(nameSalvage);

        if(!sal)
        {
            dbLog("cheater", e, "can't find salvage in roomdetail %i named %s", idRoomDetail, nameSalvage);
        }
        else if( !character_CanSalvageBeChanged( p, sal, -amount) )
        {
            dbLog("cheater", e, "can't adjust salvage %s by %i", nameSalvage, -amount);
        }
        else if( !entity_CanAdjStoredSalvage(e, detail, sal, amount) )
        {
            // might fail if client is out of date, or raid is active            
//             dbLog("cheater", e, "can't adjust roomdetail %i's item %s", idRoomDetail, nameSalvage);
        }
        else
        {
            const SalvageItem* sal = salvage_GetItem( nameSalvage );
            int player_has = character_SalvageCount(p, sal);
            int i;
            StoredSalvage *itm;

            if (amount > 0) {
                amount = MIN(player_has, amount);
            }
            else if(sal)
            {
                amount = - (S32)MIN(((U32)abs(amount)), sal->maxInvAmount - character_SalvageCount(p,sal));
            }
            
            i = roomdetail_AdjSalvage(detail, nameSalvage, amount);
            itm = EAINRANGE(i,detail->storedSalvage) ? detail->storedSalvage[i] : NULL;
            character_AdjustSalvage(p, sal, -amount, NULL, false); 
            
#if SERVER
            // record first person to add item.
            if( itm && amount > 0 && !itm->nameEntFrom[0] )
            {
                Strncpyt(itm->nameEntFrom, e->name);
            }

            if(sal)
            {
                char * datestr = timerMakeOffsetDateStringNoYearNoSecFromSecondsSince2000(dbSecondsSince2000());
                roomdetail_AddLogMsg(detail, localizedPrintf( e, 
                    amount < 0 ? "BaseStorageLogMessageRemItem" : "BaseStorageLogMessageAddItem", 
                    datestr, e->name, abs(amount), sal->ui.pchDisplayName, detail->info->pchDisplayName));
            }
            sgroup_SaveBase(&g_base,NULL,1);
            LOG_ENT( e, LOG_SUPERGROUP, LOG_LEVEL_IMPORTANT, 0, "Base:Storage adjusted %i salvage %s to detail %s(%i)", amount, nameSalvage, detail->info->pchName, detail->id );

#endif
        }
    }
}
#endif // SERVER

#if SERVER
void ReceiveStoredInspirationAdjustFromInv(Character *p, Packet *pak)
{
    int idRoomDetail = pktGetBitsAuto(pak);
    bool isNeg = pktGetBitsPack(pak, 2);
    int amount = (isNeg?-1:1)*pktGetBitsAuto(pak);
    Entity *e = (p ? p->entParent : NULL);
    const BasePower *ppow = NULL;
    BaseRoom *room = NULL;
    RoomDetail *detail = baseGetDetail(&g_base, idRoomDetail, &room);    
    int icol = pktGetBitsAuto(pak);
    int irow = pktGetBitsAuto(pak);
    char *nameItem;

    if(INRANGE0( icol, CHAR_INSP_MAX_COLS ) && INRANGE0( irow, CHAR_INSP_MAX_ROWS ) )
        ppow = e->pchar->aInspirations[icol][irow];
    
    if( amount >= 0 )
    {
        if( !ppow )
        {
            dbLog("cheater", e, "no inspiration found at %i,%i",irow,icol);
            return;                // BREAK IN FLOW 
        }
    }
    else
    {
        char *pathInspiration = pktGetString(pak);

        if( ppow )
        {
            dbLog("cheater", e, "trying to put inspiration %s at %i %i, but it is occupied", pathInspiration, icol, irow);
            return;                // BREAK IN FLOW 
        }

        ppow = basepower_FromPath(pathInspiration);
        if( !ppow )
        {
            dbLog("cheater", e, "no inspiration found matching %s", pathInspiration);
            return;                // BREAK IN FLOW 
        }
    }

    strdup_alloca(nameItem, basepower_ToPath(ppow));
    
    if( !g_base.rooms )
    {
        dbLog("cheater", e, "no base active at the moment");
    }
    else if( !idRoomDetail )
    {
        dbLog("cheater", e, "invalid idRoomDetail %d", idRoomDetail);
        return;
    }
    else if (!e || !e->supergroup || e->supergroup_id != g_base.supergroup_id)
    {
        dbLog("cheater", e, "not in a SG or not in my SG base");
        return;
    }
    else if(!verify( p && pak ))
    {
        return;            // BREAK IN FLOW 
    }
    else if( !entity_CanAdjStoredInspiration(e, detail, ppow, nameItem, amount) )
    {
        // might fail if client is out of date, or raid is active
//         dbLog("cheater", e, "can't adjust roomdetail %i's item %s", idRoomDetail, nameItem);
        return;
    }
    else
    {
        int i = roomdetail_AdjInspiration(detail, nameItem, amount);
        StoredInspiration *itm = EAINRANGE(i,detail->storedInspiration) ? detail->storedInspiration[i] : NULL;
        if( amount >= 0 )
        {
            character_RemoveInspiration(p,icol,irow,NULL);
            character_CompactInspirations(p);
        }
        else if( INRANGE0( icol, CHAR_INSP_MAX_COLS ) && INRANGE0( irow, CHAR_INSP_MAX_ROWS ) )
        {
            character_SetInspiration(p,ppow,icol,irow);
            character_CompactInspirations(p);
        }
        else
            character_AddInspiration(p, ppow, NULL);

#if SERVER
            // record first person to add item.
            if( itm && amount > 0 && !itm->nameEntFrom[0] )
            {
                Strncpyt(itm->nameEntFrom, e->name);
            }

            {
                char * datestr = timerMakeOffsetDateStringNoYearNoSecFromSecondsSince2000(dbSecondsSince2000());
                roomdetail_AddLogMsg(detail, localizedPrintf( e, 
                    amount < 0 ? "BaseStorageLogMessageRemItem" : "BaseStorageLogMessageAddItem", 
                    datestr, e->name, abs(amount), ppow->pchDisplayName, detail->info->pchDisplayName));

            }
            sgroup_SaveBase(&g_base,NULL,1);

#endif
        
        LOG_ENT( e, LOG_SUPERGROUP, LOG_LEVEL_IMPORTANT, 0, "Base:Storage adjusted %i inspiration %s to detail %s(%i)", amount, nameItem, detail->info->pchName, detail->id );
    }
}
#endif // SERVER

#if SERVER
void ReceiveStoredEnhancementAdjustFromInv(Character *p, Packet *pak)
{
    int idRoomDetail = pktGetBitsAuto(pak);
    bool isNeg = pktGetBitsPack(pak, 2);
    int amount = (isNeg?-1:1)*pktGetBitsAuto(pak);
    Entity *e = (p ? p->entParent : NULL);
    BaseRoom *room = NULL;
    RoomDetail *detail = baseGetDetail(&g_base, idRoomDetail, &room);    
    int iInv = pktGetBitsAuto(pak); // player's inventory
    int iLevel = pktGetBitsAuto(pak);
    int iNumCombines = pktGetBitsAuto(pak);

    const BasePower *ppowBase = NULL;
    Boost *enhancement = NULL;
    char *pathBasePower = NULL;

    if(AINRANGE( iInv, p->aBoosts ))
        enhancement = e->pchar->aBoosts[iInv];
    
    if( amount >= 0 )
    {
        if( !enhancement || !enhancement->ppowBase )
        {
            dbLog("cheater", e, "no enhancement found at %i", iInv);
            return;                // BREAK IN FLOW 
        }
        ppowBase = enhancement->ppowBase;
    }
    else
    {
        char *pathBasePower = pktGetString(pak);

        if( enhancement )
        {
            dbLog("cheater", e, "trying to put enhancement %s at %i, but it is occupied", pathBasePower, iInv );
            return;                // BREAK IN FLOW 
        }

        ppowBase = basepower_FromPath(pathBasePower);
        if( !ppowBase )
        {
            dbLog("cheater", e, "no enhancement found matching %s", pathBasePower);
            return;                // BREAK IN FLOW 
        }
    }

    strdup_alloca(pathBasePower, basepower_ToPath(ppowBase));
    
    if( !g_base.rooms )
    {
        dbLog("cheater", e, "no base active at the moment");
    }
    else if( !idRoomDetail )
    {
        dbLog("cheater", e, "invalid idRoomDetail %d", idRoomDetail);
        return;
    }
    else if (!e || !e->supergroup || e->supergroup_id != g_base.supergroup_id)
    {
        dbLog("cheater", e, "not in a SG or not in my SG base");
        return;
    }
    else if(!verify( p && pak ))
    {
        return;            // BREAK IN FLOW 
    }
    else if( !entity_CanAdjStoredEnhancement(e, detail, ppowBase, pathBasePower, iLevel, iNumCombines, amount) )
    {
        // might fail if client is out of date, or raid is active
//         dbLog("cheater", e, "can't adjust roomdetail %i's item %s", idRoomDetail, pathBasePower);
        return;
    }
    else
    {
        int i;
        StoredEnhancement *itm;

        if( amount >= 0 )
            character_DestroyBoost(p,iInv,NULL);
        else if(AINRANGE( iInv, p->aBoosts ))
            character_SetBoost(p,iInv,ppowBase,iLevel,iNumCombines,NULL,0,NULL,0); // location specified. blow away anything there
        else
            iInv = character_AddBoost(p,ppowBase,iLevel,iNumCombines,NULL); // just find a spot

        if(AINRANGE( iInv, p->aBoosts ))
        {
            i = roomdetail_AdjEnhancement(detail, pathBasePower, iLevel, iNumCombines, amount);
            itm = EAINRANGE(i,detail->storedEnhancement) ? detail->storedEnhancement[i] : NULL;
#if SERVER
            // record first person to add item.
            if( itm && amount > 0 && !itm->nameEntFrom[0] )
            {
                Strncpyt(itm->nameEntFrom, e->name);
            }

            {
                char * datestr = timerMakeOffsetDateStringNoYearNoSecFromSecondsSince2000(dbSecondsSince2000());
                roomdetail_AddLogMsg(detail, localizedPrintf( e, 
                    amount < 0 ? "BaseStorageLogMessageRemItem" : "BaseStorageLogMessageAddItem", 
                    datestr, e->name, abs(amount), ppowBase->pchDisplayName, detail->info->pchDisplayName));
            }
            sgroup_SaveBase(&g_base,NULL,1);
#endif
            LOG_ENT( e, LOG_SUPERGROUP, LOG_LEVEL_IMPORTANT, 0, "Base:Storage adjusted %i enhancement %s to detail %s(%i)", amount, pathBasePower, detail->info->pchName, detail->id );
        }
        else
        {
            LOG_ENT( e, LOG_SUPERGROUP, LOG_LEVEL_IMPORTANT, 0, "Base:Storage couldn't add %s to %s(%i). inventory full.", pathBasePower, detail->info->pchName, detail->id );
        }
    }
}
#endif // SERVER


bool entity_CanAdjStoredSalvage(Entity *e, RoomDetail *detail, const SalvageItem *salvage, int amount)
{
    if( e && detail && detail->info && detail->info->bIsStorage && salvage )
    {
        if( !sgroup_hasStoragePermission(e, detail->permissions, amount) ||
            !roomdetail_CanAdjSalvage( detail, salvage->pchName, amount) ||
            (salvage->flags & SALVAGE_NOTRADE) ||
            (amount < 0 && character_SalvageIsFull(e->pchar, salvage))
#if SERVER
            || RaidPlayerIsAttacking(e)
            || RaidPlayerIsDefending(e) 
#endif
            )
        {
            return false;
        }
        else
            return true;
    }
    return false;    
}

bool entity_CanAdjStoredInspiration(Entity *e, RoomDetail *detail, const BasePower *ppow, const char *pathInsp, int amount)
{
    if( e && detail && detail->info && detail->info->bIsStorage && pathInsp )
    {
        if( !sgroup_hasStoragePermission(e, detail->permissions, amount) ||
            !roomdetail_CanAdjInspiration( detail, pathInsp, amount) || 
            (amount < 0 && character_InspirationInventoryFull(e->pchar)) ||
            !inspiration_IsTradeable(ppow, e->auth_name, "")
#if SERVER
            || RaidPlayerIsAttacking(e)
            || RaidPlayerIsDefending(e) 
#endif
            )
        {
            return false;
        }
        else
            return true;
    }
    return false;
}

bool entity_CanAdjStoredEnhancement(Entity *e, RoomDetail *detail, const BasePower *ppow, const char *pathInsp, int iLevel, int iNumCombines, int amount)
{
    if( e && detail && detail->info && detail->info->bIsStorage && pathInsp )
    {
        if( !sgroup_hasStoragePermission(e, detail->permissions, amount) ||
            !roomdetail_CanAdjEnhancement( detail, pathInsp, iLevel, iNumCombines, amount) ||
              (amount < 0 && character_BoostInventoryFull(e->pchar) ||
            !detailrecipedict_IsBoostTradeable(ppow, iLevel, e->auth_name, "")) ||
            (iNumCombines > 0 && ppow->bBoostBoostable)
#if SERVER
            || RaidPlayerIsAttacking(e)
            || RaidPlayerIsDefending(e) 
#endif
            )
        {
            return false;
        }
        else
            return true;
    }
    return false;
}

#if SERVER

#define ROOMDETAIL_LOG_MAX_N_MSGS 16

void roomdetail_AddLogMsgv(RoomDetail *d, char *msg, va_list ap)
{
    // used to do more. now just store global info
    BaseAddLogMsgv(msg,ap);
}

void roomdetail_AddLogMsg(RoomDetail *d, char *msg, ...)
{
     va_list ap; 
    va_start(ap, msg);
    roomdetail_AddLogMsgv(d, msg, ap);
    va_end(ap);
}
#endif // SERVER


