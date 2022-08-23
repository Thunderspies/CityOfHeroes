/*\
 *
 *    iopserver.h/c - Copyright 2004, 2005 Cryptic Studios
 *        All Rights Reserved
 *        Confidential property of Cryptic Studios
 *
 *    Handles central server functions item of power game part of Raid server
 *
 */
  
#include "raidserver.h"

#include <utilitieslib/components/StashTable.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/utils/error.h>
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/sysutil.h>
#include "dbserver/servercfg.h"
#include <utilitieslib/utils/FolderCache.h>
#include <utilitieslib/utils/ConsoleDebug.h>
#include <utilitieslib/language/AppLocale.h>
#include <utilitieslib/utils/MemoryMonitor.h>
#include "entity/entVarUpdate.h"        // for PKT_BITS_TO_REP_DB_ID
#include <utilitieslib/network/sock.h>
#include <utilitieslib/utils/mathutil.h>
#include "dbcomm/dbcontainer.h"
#include "dbcomm/dbcomm.h"
#include "container/container_server.h"
#include "container/container_util.h"
#include "dbcomm/dbnamecache.h"
#include <time.h>
#include "gameData/iopdata.h"
#include "iopserver.h"
#include <utilitieslib/utils/textparser.h>
#include "entity/entity.h"
#include "entity/Supergroup.h"
#include "container/containerSupergroup.h"
#include "bases/basedata.h"
#include <utilitieslib/components/MemoryPool.h>
#include <utilitieslib/utils/log.h>

// *********************************************************************************
//  for supergroup missions
// *********************************************************************************
// void* dbFindGlobalMapObj(int mapid, Vec3 pos) { return 0; }
// void* storyTaskInfoAlloc() { return 0; }
// void* storyTaskInfoDestroy() { return 0; }
// void* storyTaskInfoInit() { return 0; }

void ItemOfPowerUpdate(U32 iopid, int deleting);
U32 ItemOfPowerAdd(U32 sgid, char * itemOfPowerName );
void updateSuperGroupWithCorrectItemsOfPower( int concernedSG );

////////////////// TO DO move to shared file
/////////////////////////////////////////////////////////////////////////

SpecialDetail *AddSpecialDetail(Supergroup *supergroup, Detail *pDetail, U32 timeCreation, U32 iFlags)
{
    int i;
    SpecialDetail *pNew = NULL;
    int iSize = eaSize(&supergroup->specialDetails);

    for(i=0; i<iSize; i++)
    {
        if(supergroup->specialDetails[i] == NULL)
        {
            pNew = supergroup->specialDetails[i] = CreateSpecialDetail();
            break;
        }
        else if(supergroup->specialDetails[i]->pDetail == NULL)
        {
            pNew = supergroup->specialDetails[i];
            break;
        }
    }
    if(i==iSize)
    {
        pNew = CreateSpecialDetail();
        eaPush(&supergroup->specialDetails, pNew);
    }

    pNew->pDetail = pDetail;
    pNew->creation_time = timeCreation;
    pNew->iFlags = iFlags;

    return pNew;
}


////////////////// TO DO move to shared file
/////////////////////////////////////////////////////////////////////////


//Poor mans parameters to the container callback
static U32 param_sgid;
static U32 param_dbid;
static U32 param_int;
static U32 *param_idlist;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////// Item of Power Game //////////////////////////////////////////////////////////////////////////////////////


#define DAYS_TO_SECONDS(x) ( 24*60*60*x )

//Current Item Of Power Game
ItemOfPowerGame * g_IoPGame;
#define ID_OF_ONLY_ITEM_OF_POWER_GAME 1 //There is only one item of power game going on a t a time


void logItemOfPowerGameState()
{
    

}


ItemOfPowerGame* GetItemOfPowerGame(void) 
{
    return cstoreGetAdd( g_ItemOfPowerGameStore, ID_OF_ONLY_ITEM_OF_POWER_GAME, 1 );
}

// save to database and notify clients
// TO DO reflect this to everybody
void ItemOfPowerGameUpdate()
{
    ContainerReflectInfo** reflectinfo = 0;
    ItemOfPowerGame* iopGame;
    int iopGameID = ID_OF_ONLY_ITEM_OF_POWER_GAME;
    char* data;

    iopGame = cstoreGetAdd(g_ItemOfPowerGameStore, iopGameID, 1);
    containerReflectAdd(&reflectinfo, CONTAINER_MAPS, -1, 0, 0);

    data = cstorePackage(g_ItemOfPowerGameStore, iopGame, iopGameID);
    dbContainerSendReflection(&reflectinfo, CONTAINER_ITEMOFPOWERGAMES, &data, &iopGameID, 1, CONTAINER_CMD_CREATE_MODIFY);
    free(data);

    containerReflectDestroy(&reflectinfo);
}

U32        *rmParam_idlist; //Start giving these their own to prevent confusion
static int removeItemOfPower( ItemOfPower* iop, U32 iopId )
{
    int losingSG = iop->superGroupOwner;
    int alreadyInList = 0;
    int i;

    //Keep a record of all the SGs affected so I know to update them after this
    if( losingSG )
    {
        for( i = 0 ; i < eaiSize( &rmParam_idlist ) ; i++ )
        {
            if( rmParam_idlist[i] == losingSG )
                alreadyInList = 1;
        }
        if( !alreadyInList )
            eaiPush(&rmParam_idlist, losingSG);
    }

    //Remove from list
    iop->superGroupOwner = 0; //redundant with delete?
    iop->creationTime = 0;
    ItemOfPowerUpdate( iopId, 1 );
    cstoreDelete(g_ItemOfPowerStore, iopId);
    
    return 1;
}

void RestartItemOfPowerGame()
{
    ItemOfPowerInfo * info;
    int i;
    int sgCount;

    LOG( LOG_IOP, LOG_LEVEL_VERBOSE, 0, " ---------------- Restarting Existing IoP game -------------------\n");

    RemoveAllScheduledRaids( true ); //Issue refunds
    LOG( LOG_IOP, LOG_LEVEL_VERBOSE, 0, " ------ All Scheduled Raids Removed ----\n");

    //Get Rid of Everyone's Items of Power
    if (!rmParam_idlist) eaiCreate(&rmParam_idlist);
    eaiSetSize(&rmParam_idlist, 0);
    cstoreForEach(g_ItemOfPowerStore, removeItemOfPower);
    sgCount = eaiSize( &rmParam_idlist );
    for( i = 0 ; i < sgCount ; i++ )
    {
        updateSuperGroupWithCorrectItemsOfPower( rmParam_idlist[i] );
    }

    LOG( LOG_IOP, LOG_LEVEL_VERBOSE, 0, " ------ All Existing Iops Deleted ----\n");

    //Add all the unique Items of Power to the DB right away
    for( i = 0 ; i < eaSize( &g_itemOfPowerInfoList.infos ) ; i++ )
    {    
        info = g_itemOfPowerInfoList.infos[i];
        if( info->unique )
        {
            ItemOfPowerAdd( 0, info->name );
        }
    }

    LOG( LOG_IOP, LOG_LEVEL_VERBOSE, 0, " ------ Unique Iops added ----\n");
 
    //Reset the Clock on the Item of Power Game
    g_IoPGame = cstoreGetAdd(g_ItemOfPowerGameStore, ID_OF_ONLY_ITEM_OF_POWER_GAME, 1 );

    g_IoPGame->state &= IOP_GAME_DEBUG_ALLOW_RAIDS_AND_TRIALS; //Clear any state except allow raids and trials
    g_IoPGame->state |= IOP_GAME_CATHEDRAL_OPEN;
    g_IoPGame->startTime = dbSecondsSince2000();
    ItemOfPowerGameUpdate();
}

 
void SetCathedralClosed()
{
    LOG( LOG_IOP, LOG_LEVEL_VERBOSE, 0, " ---------------- Closing Cathedral of Pain -------------------\n");
    g_IoPGame = cstoreGetAdd(g_ItemOfPowerGameStore, ID_OF_ONLY_ITEM_OF_POWER_GAME, 1 );
    g_IoPGame->state &= IOP_GAME_DEBUG_ALLOW_RAIDS_AND_TRIALS; //Clear any state except allow raids and trials
    g_IoPGame->state |= IOP_GAME_CATHEDRAL_CLOSED;
    ItemOfPowerGameUpdate();
}

void StopItemOfPowerGame()
{
    LOG( LOG_IOP, LOG_LEVEL_VERBOSE, 0, " ---------------- Stopping Item of POwer Game  -------------------\n");
    g_IoPGame = cstoreGetAdd(g_ItemOfPowerGameStore, ID_OF_ONLY_ITEM_OF_POWER_GAME, 1 );

    logItemOfPowerGameState();

    g_IoPGame->state &= IOP_GAME_DEBUG_ALLOW_RAIDS_AND_TRIALS; //Clear any state except allow raids and trials
    g_IoPGame->state |= IOP_GAME_NOT_RUNNING;
    ItemOfPowerGameUpdate();
}

void DebugAllowRaidsAndTrials()
{
    LOG( LOG_IOP, LOG_LEVEL_VERBOSE, 0, " ---------------- Enabling Debug Allow Item of Power Game Raids and Trials  -------------------\n");
    g_IoPGame = cstoreGetAdd(g_ItemOfPowerGameStore, ID_OF_ONLY_ITEM_OF_POWER_GAME, 1 );

    logItemOfPowerGameState();

    g_IoPGame->state |= IOP_GAME_DEBUG_ALLOW_RAIDS_AND_TRIALS; //Clear any state except allow raids and trials

    ItemOfPowerGameUpdate();
}

void NoDebugAllowRaidsAndTrials()
{
    LOG( LOG_IOP, LOG_LEVEL_VERBOSE, 0, " ---------------- Disabling Debug Allow Item of Power Game Raids and Trials  -------------------\n");
    g_IoPGame = cstoreGetAdd(g_ItemOfPowerGameStore, ID_OF_ONLY_ITEM_OF_POWER_GAME, 1 );

    logItemOfPowerGameState();

    g_IoPGame->state &= ~IOP_GAME_DEBUG_ALLOW_RAIDS_AND_TRIALS; 

    ItemOfPowerGameUpdate();
}


void CheckItemOfPowerGame(void)
{
    int timeNow;

    g_IoPGame = cstoreGetAdd(g_ItemOfPowerGameStore, ID_OF_ONLY_ITEM_OF_POWER_GAME, 1 );

    //If this DB has never had any item of power game, start one up.
    if( g_IoPGame->state == IOP_GAME_NEVER_STARTED )
        RestartItemOfPowerGame();

    if( !(g_IoPGame->state & IOP_GAME_NOT_RUNNING) ) //If the game hasn't been explicitly turned off
    {
        timeNow = dbSecondsSince2000() - g_IoPGame->startTime; 

        //If the Last Game is done, restart the game
        if( timeNow > DAYS_TO_SECONDS( g_itemOfPowerInfoList.daysGameLasts ) )
        {
            RestartItemOfPowerGame();
        }
        else if( (g_IoPGame->state & IOP_GAME_CATHEDRAL_OPEN) && timeNow > DAYS_TO_SECONDS( g_itemOfPowerInfoList.daysCathedralIsOpen ) )
        {
            SetCathedralClosed();
        }
    }
}

bool isCathdralOfPainOpenAtThisTime( U32 time )
{
    g_IoPGame = cstoreGetAdd(g_ItemOfPowerGameStore, ID_OF_ONLY_ITEM_OF_POWER_GAME, 1 );

    //Accomodate debug command -- if the Cathedral has been closed by hand, allow raids to be scheduled henceforth
    if( !(g_IoPGame->state & IOP_GAME_CATHEDRAL_OPEN) || (g_IoPGame->state & IOP_GAME_DEBUG_ALLOW_RAIDS_AND_TRIALS) )
        return false;

    if( g_IoPGame->startTime < time && time <= g_IoPGame->startTime + DAYS_TO_SECONDS( g_itemOfPowerInfoList.daysCathedralIsOpen ) )
        return true;

    return false;
}

bool isCurrentGameRunningAtThisTime( U32 time )
{
    g_IoPGame = cstoreGetAdd(g_ItemOfPowerGameStore, ID_OF_ONLY_ITEM_OF_POWER_GAME, 1 );

    if( g_IoPGame->startTime < time && time < g_IoPGame->startTime + DAYS_TO_SECONDS( g_itemOfPowerInfoList.daysGameLasts ) )
        return true;

    return false;
}

bool isScheduledRaidsAllowed(void)
{
    return g_itemOfPowerInfoList.allowScheduledRaids;
}

void handleItemOfPowerGameSetState(Packet* pak, U32 listid, U32 cid)
{
    char * newState;

    newState = pktGetString(pak);

    if( !newState )
    {
        dbContainerSendReceipt(CONTAINER_ERR_CANT_COMPLETE, "ServerError");
        return;
    }

    //There is only one Item of Power Game, and it is always in some state
    g_IoPGame = cstoreGetAdd(g_ItemOfPowerGameStore, ID_OF_ONLY_ITEM_OF_POWER_GAME, 1 );

    if( 0 == stricmp( newState, "Restart") )
    {
        RestartItemOfPowerGame();
    }
    else if( 0 == stricmp( newState, "CloseCathedral") )
    {
        SetCathedralClosed();
    }
    else if( 0 == stricmp( newState, "End") )
    {
        StopItemOfPowerGame();
    }
    else if( 0 == strnicmp( newState, "Debug", 5 ) )
    {
        DebugAllowRaidsAndTrials();
    }
    else if( 0 == strnicmp( newState, "NoDebug", 7) )
    {
        NoDebugAllowRaidsAndTrials();
    }
}


//TO DO this : update the Item of Power game state.
//You just request from a newly started Mapserver asking for the current state of the Item of Power game.
void handleItemOfPowerGameRequestUpdate(Packet* pak, U32 listid, U32 cid)
{
    ContainerReflectInfo** reflectinfo = 0;
    ItemOfPower* iopGame;
    int iopGameID = ID_OF_ONLY_ITEM_OF_POWER_GAME;
    char* data;

    iopGame = cstoreGetAdd(g_ItemOfPowerGameStore, iopGameID, 1);
    containerReflectAdd(&reflectinfo, REFLECT_LOCKID, dbRelayLockId(), 0, 0);

    data = cstorePackage(g_ItemOfPowerGameStore, iopGame, iopGameID);
    dbContainerSendReflection(&reflectinfo, CONTAINER_ITEMOFPOWERGAMES, &data, &iopGameID, 1, CONTAINER_CMD_REFLECT);
    free(data);

    containerReflectDestroy(&reflectinfo);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////// Items of Power//////////////////////////////////////////////////////////////////////////////////////

//Look through the infos to see if this iop is unique
int isUniqueItemOfPower( char * name )
{
    ItemOfPowerInfo * info;
    int i;
    int isUnique = 0;

    if( !name )
        return 0;

    for( i = 0 ; i < eaSize( &g_itemOfPowerInfoList.infos ) ; i++ )
    {    
        info = g_itemOfPowerInfoList.infos[i];
        if( 0 == stricmp( info->name, name ) )
        {
            if( info->unique )
                isUnique = 1;
            break;
        }
    }
    return isUnique;
}


// delete any IoPs relating to this supergroup
int itemOfPowerSGDeleteIter(ItemOfPower* iop, U32 raidid)
{
    int concernedSG = iop->superGroupOwner;
    if (iop->superGroupOwner == param_sgid)
    {
        iop->superGroupOwner = 0;
        ItemOfPowerUpdate( iop->id, 0 );
        //Unneeded, since SG is gone: updateSuperGroupWithCorrectItemsOfPower( concernedSG );
    }
    return 1;
}


void handleSupergroupDeleteForItemsOfPower(U32 sgid)
{
    {  //Start Log 
        char buf[200];
        char * sgName = dbSupergroupNameFromId(sgid);
        LOG( LOG_IOP, LOG_LEVEL_VERBOSE, 0, "CMD Supergroup Delete : %d %s at %s",
            sgid, 
            sgName?sgName:"NONE",
            timerMakeDateStringFromSecondsSince2000( buf, dbSecondsSince2000() ) 
            );
    }    //End log

    param_sgid = sgid;
    cstoreForEach(g_ItemOfPowerStore, itemOfPowerSGDeleteIter);

    {  //Start Log 
        LOG( LOG_IOP, LOG_LEVEL_VERBOSE, 0, "ENDCMD Supergroup Delete" );
    }    //End log
}

static int gatherSGsItemsOfPower(ItemOfPower* iop, U32 iopId)
{
    if (iop->superGroupOwner == param_sgid )
        eaiPush(&param_idlist, iopId);
    return 1;
}

//Informs the player of all his supergroup's items of power
void PlayerFullItemOfPowerUpdate(U32 sgid, U32 dbid, int add)
{
    int i, count;
    ContainerReflectInfo** reflect = 0;
    char** data = 0;

    // get all raids associated player's supergroup
    param_sgid = sgid;
    if (!param_idlist) eaiCreate(&param_idlist);
    eaiSetSize(&param_idlist, 0);
    cstoreForEach(g_ItemOfPowerStore, gatherSGsItemsOfPower);
    count = eaiSize(&param_idlist);
    if (!count) return; // done

    // package all data
    eaCreate(&data);
    for (i = 0; i < eaiSize(&param_idlist); i++)
    {
        ItemOfPower * iop = cstoreGet(g_ItemOfPowerStore, param_idlist[i]);
        eaPush(&data, cstorePackage(g_ItemOfPowerStore, iop, param_idlist[i])); 
    }

    // send a reflection of all raids to player
    containerReflectAdd(&reflect, CONTAINER_ENTS, dbid, 0, !add);
    dbContainerSendReflection(&reflect, CONTAINER_ITEMSOFPOWER, data, param_idlist, eaiSize(&param_idlist), CONTAINER_CMD_REFLECT);

    // cleanup
    eaDestroyEx(&data, NULL);
    containerReflectDestroy(&reflect);
}



int getItemOfPowerFromSGByNameAndCreationTime( int sgid, const char * nameOfItemOfPowerCaptured, int creationTimeOfItemOfPowerCaptured )
{
    int foundIopID = 0;
    int count, i; 

    //Get all the Items of Power
    param_sgid = sgid;
    if (!param_idlist) eaiCreate(&param_idlist);
    eaiSetSize(&param_idlist, 0);
    cstoreForEach(g_ItemOfPowerStore, gatherSGsItemsOfPower);
    count = eaiSize( &param_idlist );

    for( i = 0 ; i < count ; i++ )
    {
        ItemOfPower * iop = cstoreGet(g_ItemOfPowerStore, param_idlist[i]);
        if( strstri( iop->name, nameOfItemOfPowerCaptured ) && creationTimeOfItemOfPowerCaptured == iop->creationTime ) //TO DO check this, do I need to send the creation time, too
        {
            foundIopID = iop->id;
            break;
        }
    }

    return foundIopID;    
}


//Specialty function just for use in this place -- assumes param_idlist has just been filled out
//and a supergroup's Items of Power are being completely iterated over
//-1 in the param list means this IoP has already been accounted for
static int accountForThisItemOfPower( const char * name, int creationTime )
{
    int i, count;
    bool res = false;
    count = eaiSize(&param_idlist);
    
    for( i = 0 ; i < count ; i++ )
    {
        if( param_idlist[i] != -1 )
        {
            ItemOfPower * iop = cstoreGet(g_ItemOfPowerStore, param_idlist[i]);
            if( 0 == stricmp( iop->name, name ) && creationTime == iop->creationTime )
            {
                param_idlist[i] = -1;
                res = true;
                break;
            }
        }
    }
    return res;
}

////Give this Supergroup all the ItemsOfPower it has
void updateSuperGroupWithCorrectItemsOfPower( int concernedSG )
{
    Supergroup sg = {0};
    int i;
    int sgIopCount;
    int raidIopCount;

    //////Fill e->supergroup SUPERGROUP's current list of all this supergroup's ItemsOfPower (dummy entity)
    if( !concernedSG )
        return;
    if( !sgroup_LockAndLoad(concernedSG, &sg) )
        return;

    sgIopCount = eaSize(&sg.specialDetails);

    //////Fill out param_idlist with RAIDSERVER's (authoritative) list of all this supergroup's ItemsOfPower
    //(This list is only for the log, the list is renewed below, after the special detail list is allowed to add 
    //or remove it's own updates
    param_sgid = concernedSG; //poor man's parameter to gatherSGsItemsOfPower
    if (!param_idlist) eaiCreate(&param_idlist); //poor man's parameter to gatherSGsItemsOfPower
    eaiSetSize(&param_idlist, 0);
    cstoreForEach(g_ItemOfPowerStore, gatherSGsItemsOfPower);
    raidIopCount = eaiSize(&param_idlist);

    {//Start Log 
        char buf[200];
        int someIOPs = 0;
        LOG( LOG_IOP, LOG_LEVEL_VERBOSE, 0, "\t\tUpdating Concerned SG %d(%s)" , 
            concernedSG, concernedSG?dbSupergroupNameFromId(concernedSG):"NONE" );
        if( raidIopCount )
        {
            LOG( LOG_IOP, LOG_LEVEL_VERBOSE, 0, "\t\t Raid Server says SG should have:");
            for (i = 0; i < raidIopCount ; i++)
            {
                ItemOfPower * iop = cstoreGet(g_ItemOfPowerStore, param_idlist[i]);
                LOG( LOG_IOP, LOG_LEVEL_VERBOSE, 0, "\t\t  %s ID:%d SG:%d Create Time: %s",
                    iop->name, 
                    iop->id,
                    iop->superGroupOwner,
                    timerMakeDateStringFromSecondsSince2000( buf, iop->creationTime ) );
            }
        }
        else
        {
            LOG( LOG_IOP, LOG_LEVEL_VERBOSE, 0,"\t\t Raid Server says SG should have NO ITEMS OF POWER");
        }

        LOG( LOG_IOP, LOG_LEVEL_VERBOSE, 0, "\t\t Before Synch, SG says it has these:");
        for( i=0 ; i < sgIopCount ; i++ )
        {
            SpecialDetail * pSD = sg.specialDetails[i];
            if( pSD && pSD->pDetail && pSD->iFlags & DETAIL_FLAGS_RAIDSERVER_OWNED_ITEM_OF_POWER )
            {    
                LOG( LOG_IOP, LOG_LEVEL_VERBOSE, 0, "\t\t  Name:%s Create Time: %s Flags: %d",
                    pSD->pDetail->pchName,
                    timerMakeDateStringFromSecondsSince2000( buf, pSD->creation_time ),
                    pSD->iFlags );
                someIOPs = 1;
            }
        }
        
        if( !someIOPs )
        {
            LOG( LOG_IOP, LOG_LEVEL_VERBOSE, 0, "\t\t  NO ITEMS OF POWER:");
        }

    }//End log

    //////Update RAIDSERVER to match SUPERGROUP
    //1. Update Raidserver's list with any special Details Flaged as to be added or removed
    for( i=0 ; i < sgIopCount ; i++ )
    {
        SpecialDetail * pSD = sg.specialDetails[i];
        if( pSD && pSD->pDetail && pSD->iFlags & DETAIL_FLAGS_RAIDSERVER_OWNED_ITEM_OF_POWER )
        {    
            int iopId;
            ItemOfPower * iop = 0;

            if( pSD->iFlags & DETAIL_FLAGS_RAIDSERVER_REMOVE_THIS_ITEM_OF_POWER )
            {
                iopId = getItemOfPowerFromSGByNameAndCreationTime( concernedSG, pSD->pDetail->pchName, pSD->creation_time );
                iop = cstoreGet(g_ItemOfPowerStore, iopId);
                if( iop )
                {
                    iop->superGroupOwner = 0; //redundant with delete?
                    iop->creationTime = 0;
                    ItemOfPowerUpdate( iopId, 1 );
                    cstoreDelete(g_ItemOfPowerStore, iopId);
                }

                DestroySpecialDetail(pSD);
                sg.specialDetails[i] = NULL;
            }
            else if( pSD->iFlags & DETAIL_FLAGS_NEW_IOP_RAIDSERVER_NEEDS_TO_KNOW_ABOUT )
            {
                int i;
                ItemOfPowerInfo * info = 0;
                for( i = 0 ; i < eaSize( &g_itemOfPowerInfoList.infos ) ; i++ )
                {    
                    info = g_itemOfPowerInfoList.infos[i];
                    if( info->name && 0 == stricmp( info->name, pSD->pDetail->pchName ) )
                        break;
                }
                if( !info )
                    ;//ERROR!!!

                iop = cstoreAdd(g_ItemOfPowerStore);
                strncpy( iop->name, info->name, MAX_IOP_NAME_LEN );
                iop->superGroupOwner = concernedSG;
                iop->creationTime = pSD->creation_time;
                ItemOfPowerUpdate (iop->id, 0 );

                pSD->iFlags &= ~DETAIL_FLAGS_NEW_IOP_RAIDSERVER_NEEDS_TO_KNOW_ABOUT;
            }
        }
    }

    //////Update SUPERGROUP's list to match RAIDSERVER's
    //2. Validate all this SUPERGROUP's current IOP list, removing those that shouldn't be there 

    //////Fill out param_idlist with RAIDSERVER's (authoritative) list of all this supergroup's ItemsOfPower
    param_sgid = concernedSG; //poor man's parameter to gatherSGsItemsOfPower
    if (!param_idlist) eaiCreate(&param_idlist); //poor man's parameter to gatherSGsItemsOfPower
    eaiSetSize(&param_idlist, 0);
    cstoreForEach(g_ItemOfPowerStore, gatherSGsItemsOfPower);
    raidIopCount = eaiSize(&param_idlist);

    for( i=0 ; i < sgIopCount ; i++ )
    {
        SpecialDetail * pSD = sg.specialDetails[i];
        if( pSD && pSD->pDetail && pSD->iFlags & DETAIL_FLAGS_RAIDSERVER_OWNED_ITEM_OF_POWER )
        {
            if( !accountForThisItemOfPower( pSD->pDetail->pchName, pSD->creation_time ) )
            {
                DestroySpecialDetail(pSD);
                sg.specialDetails[i] = NULL;
            }
        }
    }    

    //2. Add all IOPs that SUPERGROUP doesn't have, but RAIDSERVER says it should
    for (i = 0; i < raidIopCount ; i++)
    {
        if( param_idlist[i] != -1 )
        {
            Detail *pDetail;
            ItemOfPower * iop;
            iop = cstoreGet(g_ItemOfPowerStore, param_idlist[i]);
            pDetail = getDummyDetail( iop->name );
            AddSpecialDetail(&sg, pDetail, iop->creationTime, DETAIL_FLAGS_AUTOPLACE | DETAIL_FLAGS_RAIDSERVER_OWNED_ITEM_OF_POWER );
        }
    }

    {//Start Log 
        char buf[200];
        int someIOPs = 0;
        sgIopCount = eaSize(&sg.specialDetails);

        LOG( LOG_IOP, LOG_LEVEL_VERBOSE, 0, "\t\t After Synch, SG says it has these:");
        for( i=0 ; i < sgIopCount ; i++ )
        {
            SpecialDetail * pSD = sg.specialDetails[i];
            if( pSD && pSD->pDetail && pSD->iFlags & DETAIL_FLAGS_RAIDSERVER_OWNED_ITEM_OF_POWER )
            {    
                LOG( LOG_IOP, LOG_LEVEL_VERBOSE, 0, "\t\t  Name:%s Create Time: %s Flags: %d",
                    pSD->pDetail->pchName,
                    timerMakeDateStringFromSecondsSince2000( buf, pSD->creation_time ),
                    pSD->iFlags );
                someIOPs = 1;
            }
        }
        
        if( !someIOPs )
        {
            LOG( LOG_IOP, LOG_LEVEL_VERBOSE, 0, "\t\t  NO ITEMS OF POWER:");
        }

        log_printf("iop", "\t\tEnd Updating Concerned SG " );
    }//End log
    
    sgroup_UpdateUnlock(concernedSG, &sg);
}

// save to database and notify clients
void ItemOfPowerUpdate( U32 iopid, int deleting )
{
    ContainerReflectInfo** reflectinfo = 0;
    ItemOfPower* iop = cstoreGet(g_ItemOfPowerStore, iopid);
    int sgOwner = iop->superGroupOwner;

    {  //Start Log 
        char buf[200];
        LOG( LOG_IOP, LOG_LEVEL_VERBOSE, 0, "\tUpdating IoP %d %s -- Owner:%d(%s) Created:%s" , 
            iop->id,
            iop->name, 
            iop->superGroupOwner, iop->superGroupOwner?dbSupergroupNameFromId(iop->superGroupOwner):"NONE", 
            timerMakeDateStringFromSecondsSince2000( buf, iop->creationTime ) );
    }    //End log

    containerReflectAdd(&reflectinfo, CONTAINER_SUPERGROUPS, iop->superGroupOwner, 0, deleting);

    if (deleting)
    {
        dbContainerSendReflection(&reflectinfo, CONTAINER_ITEMSOFPOWER, NULL, &iopid, 1, CONTAINER_CMD_DELETE);
    }
    else
    {
        char* data = cstorePackage(g_ItemOfPowerStore, iop, iopid);
        dbContainerSendReflection(&reflectinfo, CONTAINER_ITEMSOFPOWER, &data, &iopid, 1, CONTAINER_CMD_CREATE_MODIFY);
        free(data);
    }
    containerReflectDestroy(&reflectinfo);
}

//Add new Item of Power to the DB with a new owner
U32 ItemOfPowerAdd(U32 sgid, char * itemOfPowerName )
{
    U32 now = timerSecondsSince2000();

    ItemOfPower * iop = cstoreAdd(g_ItemOfPowerStore);
    if (iop)
    {
        strncpy( iop->name, itemOfPowerName, MAX_IOP_NAME_LEN );
        iop->superGroupOwner = sgid;
        iop->creationTime = now;
    }
    ItemOfPowerUpdate (iop->id, 0 );
    return iop->id;
}



static int getAvailableUniques(ItemOfPower* iop, U32 iopID)
{
    if ( !iop->superGroupOwner && isUniqueItemOfPower( iop->name ) ) 
    {
        eaiPush(&param_idlist, iopID);
    }
    return 1;
}

void handleItemOfPowerGrantNew(Packet* pak, U32 listid, U32 cid)
{
    U32 sgid;
    int gotUnique;
    char * IoPName = 0;

    sgid = pktGetBitsPack(pak, 1);

    if (!sgid)
    {
        dbContainerSendReceipt(CONTAINER_ERR_CANT_COMPLETE, "RaidNotInSupergroup");
        return;
    }

    {  //Start Log 
        char buf[200];
        LOG( LOG_IOP, LOG_LEVEL_VERBOSE, 0, "CMD Item of Power Grant New : SG %d (%s)", 
            sgid, 
            sgid?dbSupergroupNameFromId(sgid):"NONE", 
            timerMakeDateStringFromSecondsSince2000( buf, dbSecondsSince2000() ) );
    }    //End log

    //Did you get a unique IoP?
    gotUnique = randInt(100) < g_itemOfPowerInfoList.chanceOfGettingUnique ? 1 : 0;

    
    if( gotUnique )
    {
        int count;

        //Get all the Unique Items of Power
        if (!param_idlist) eaiCreate(&param_idlist);
        eaiSetSize(&param_idlist, 0);
        cstoreForEach(g_ItemOfPowerStore, getAvailableUniques);
        count = eaiSize( &param_idlist );

        //Pick Randomly among the available unique IoPs
        if( count )
        {
            ItemOfPower * iop;
            int iopYouGotId = param_idlist[ randInt( count ) ];
            iop = cstoreGet(g_ItemOfPowerStore, iopYouGotId);
            iop->superGroupOwner = sgid;
            ItemOfPowerUpdate(iop->id, 0);
            updateSuperGroupWithCorrectItemsOfPower( iop->superGroupOwner );
        }
        else
        {
            gotUnique = 0; //There aren't any Uniques to be had
        }
        
    }
    
    //If you couldn't get a unique because you rolled bad, or none are available, get a generic.
    if( !gotUnique )
    {
        ItemOfPowerInfo * info;
        ItemOfPowerInfo ** genericInfos = 0;
        int count, i;
        
        //Don't really need to do this over and over
        eaCreate(&genericInfos);
        for( i = 0 ; i < eaSize( &g_itemOfPowerInfoList.infos ) ; i++ )
        {    
            info = g_itemOfPowerInfoList.infos[i];
            if( !info->unique )
                eaPush( &genericInfos, info );
        }
        count = eaSize( &genericInfos );
        info = genericInfos[ randInt(count) ];

        eaDestroy( &genericInfos );
        
        if( !info )
        {
            dbContainerSendReceipt(CONTAINER_ERR_CANT_COMPLETE, "Can't find any IoPs to give you whatsoever.");
            LOG( LOG_IOP, LOG_LEVEL_VERBOSE, 0, "ENDCMD Item of Power Grant Failed: Can't find any IoPs to give you whatsoever." );
            return;
        }

        ItemOfPowerAdd( sgid, info->name );
        updateSuperGroupWithCorrectItemsOfPower( sgid );
    }

    VerifyAllExistingRaids();

    LOG( LOG_IOP, LOG_LEVEL_VERBOSE, 0, "ENDCMD Item of Power Grant New Complete" );

    dbContainerSendReceipt(0, 0);
}

//Grant not by ID, but by IoP name -- Debug only function
void handleItemOfPowerGrant(Packet* pak, U32 listid, U32 cid)
{
    int i;
    ItemOfPowerInfo * info = 0;
    int sgid;
    char * iopName;

    sgid = pktGetBitsPack(pak, 1);
    iopName = pktGetString( pak );

    {  //Start Log 
        char buf[200];
        LOG( LOG_IOP, LOG_LEVEL_VERBOSE, 0, "CMD Item of Power Grant By Name: SG %d (%s) %s", 
            sgid, 
            sgid?dbSupergroupNameFromId(sgid):"NONE", 
            timerMakeDateStringFromSecondsSince2000( buf, dbSecondsSince2000() ),
            iopName);
    }    //End log

    for( i = 0 ; i < eaSize( &g_itemOfPowerInfoList.infos ) ; i++ )
    {    
        info = g_itemOfPowerInfoList.infos[i];
        if( info->name && 0 == stricmp( info->name, iopName ) )
            break;
    }

    if( !info )
    {
        dbContainerSendReceipt(CONTAINER_ERR_CANT_COMPLETE, "Can't find any IoPs to give you whatsoever.");
        LOG( LOG_IOP, LOG_LEVEL_VERBOSE, 0, "ENDCMD Item of Power Grant Failed: No IoP by the name %s.", iopName );
        return;
    }

    ItemOfPowerAdd( sgid, info->name );

    LOG( LOG_IOP, LOG_LEVEL_VERBOSE, 0, "ENDCMD Item of Power Grant by Name %s.", iopName );
}

//Take the Item Away from SG 1, and give it to SG 2 (or no one)
void iopTransfer( int sgid, int newSgid, int iopID )
{
    ItemOfPower * iop = cstoreGet(g_ItemOfPowerStore, iopID);

    if( !iop || !sgid || !iopID )
        return;

    //Take the Item Away from SG 1, and give it to SG 2 (or no one)
    iop->superGroupOwner = newSgid;
    ItemOfPowerUpdate(iop->id, 0 );
    updateSuperGroupWithCorrectItemsOfPower( sgid );
    if( newSgid )
        updateSuperGroupWithCorrectItemsOfPower( newSgid );

    ///If loser has no more IOPs, unschedule his defenses
    if( sgid && 0 == NumItemsOfPower( sgid ) )
        RemoveSGsScheduledRaids( sgid, false, true, true ); //remove defends, not attacks, issue refunds

    VerifyAllExistingRaids();
}

void handleItemOfPowerTransfer(Packet* pak, U32 listid, U32 cid)
{
    U32 sgid;
    U32 newSgid;
    U32 iopID;
    ItemOfPower * iop;
    char * iopName;
    int iopTimeCreated;
        
    sgid    = pktGetBitsPack(pak, 1);
    newSgid    = pktGetBitsPack(pak, 1); 

    iopName = pktGetString( pak );
    iopTimeCreated = pktGetBitsPack( pak, 1 );

    {  //Start Log 
        char buf[200];
        char buf2[200];
        char * sgName = dbSupergroupNameFromId(sgid);
        char * newSgName = dbSupergroupNameFromId(newSgid);

        LOG( LOG_IOP, LOG_LEVEL_VERBOSE, 0, "CMD Item of Power Transfer : %s %s From SG %d(%s) To SG %d(%s) at %s ",
            iopName,
            timerMakeDateStringFromSecondsSince2000( buf, iopTimeCreated ) ,
            sgid, 
            sgName?sgName:"NONE",
            newSgid, 
            newSgName?newSgName:"NONE",
            timerMakeDateStringFromSecondsSince2000( buf2, dbSecondsSince2000() ) 
            );
    }    //End log

    if( sgid == newSgid )
    {
        dbContainerSendReceipt(CONTAINER_ERR_CANT_COMPLETE, "IoPSelf");
        LOG( LOG_IOP, LOG_LEVEL_VERBOSE, 0, "ENDCMD Failed Item of Power Transfer: A Supergroup can't give an Item of Power to itself" );
        return;
    }

    iopID = getItemOfPowerFromSGByNameAndCreationTime( sgid, iopName, iopTimeCreated );
    iop = cstoreGet(g_ItemOfPowerStore, iopID);

    if( !iop )
    {
        dbContainerSendReceipt(CONTAINER_ERR_CANT_COMPLETE, "IoPNoSource");
        LOG( LOG_IOP, LOG_LEVEL_VERBOSE, 0, "ENDCMD Failed Item of Power Transfer: That Supergroup doesn't have that Item to Lose" );
        return;
    }

    iopTransfer( sgid, newSgid, iopID );

    LOG( LOG_IOP, LOG_LEVEL_VERBOSE, 0, "ENDCMD Item of Power Transfer" );
}

void handleItemOfPowerSGSynch(Packet* pak, U32 listid, U32 cid)
{
    int sgid;

    sgid = pktGetBitsPack(pak, 1);

    {  //Start Log 
        char buf2[200];
        char * sgName = dbSupergroupNameFromId(sgid);
        LOG( LOG_IOP, LOG_LEVEL_VERBOSE, 0, "CMD Item of Power Synch SG %d(%s) at %s",
            sgid, 
            sgName?sgName:"NONE",
            timerMakeDateStringFromSecondsSince2000( buf2, dbSecondsSince2000() ) 
            );
    }    //End log

    if( sgid )
        updateSuperGroupWithCorrectItemsOfPower( sgid );

    ///If loser has no more IOPs, unschedule his defenses
    if( sgid && 0 == NumItemsOfPower( sgid ) )
        RemoveSGsScheduledRaids( sgid, false, true, true ); //remove defends, not attacks, issue refunds

    VerifyAllExistingRaids();

    LOG( LOG_IOP, LOG_LEVEL_VERBOSE, 0, "ENDCMD Item of Power Synch" );

}


int getRandomIopFromSG( int sgid )
{
    int count; 

    //Get all this SG's Items of Power
    param_sgid = sgid;
    if (!param_idlist) eaiCreate(&param_idlist);
    eaiSetSize(&param_idlist, 0);
    cstoreForEach(g_ItemOfPowerStore, gatherSGsItemsOfPower);
    count = eaiSize( &param_idlist );

    if( count )
        return param_idlist[ randInt( count ) ];
    return 0;
}


static Supergroup * s_sgroupForReceivecontainers = NULL;
static int sgroup_ReceiveContainers(Packet *pak,int cmd,NetLink *link)
{
    int                list_id,count;
    ContainerInfo    ci;

    list_id            = pktGetBitsPack(pak,1);
    count            = pktGetBitsPack(pak,1);

    assert(count == 1);

    if (!dbReadContainer(pak,&ci,list_id))
        return false;

    if(verify( s_sgroupForReceivecontainers ))
    {
        clearSupergroup(s_sgroupForReceivecontainers);
        sgroup_DbContainerUnpack( s_sgroupForReceivecontainers, ci.data );
    }
    return true;
}

//----------------------------------------
// fills the dummy supergroup with supergroup fields
//----------------------------------------
bool sgroup_LockAndLoad(int sgid, Supergroup *sg)
{
    bool res = false;

    if(verify( sg ))
    {
        s_sgroupForReceivecontainers = sg;
        res = (dbSyncContainerRequestCustom(CONTAINER_SUPERGROUPS, sgid, CONTAINER_CMD_LOCK_AND_LOAD, sgroup_ReceiveContainers) != 0);
    }

    // ----------
    // finally

    return res;
}

//----------------------------------------
// unlocks and updates the dummy supergroup, and frees any fields attached to it
// DO NOT PASS A 'REAL' SUPERGROUP TO THIS, JUST THE ONE FILLED BY 'sgroup_LockAndLoad'
//----------------------------------------
void sgroup_UpdateUnlock(int sgid, Supergroup *sg)
{
    if(verify( sg ))
    {
        // ----------
        // update the container

        char *container_str = packageSuperGroup(sg);
        bool ret = dbSyncContainerUpdate(CONTAINER_SUPERGROUPS,sgid,CONTAINER_CMD_UNLOCK,container_str);
        free(container_str);

        // ----------
        // clear the supergroup

        clearSupergroup(sg);

        // ----------
        // log

        //dbLogDetails("SuperGroup", sg->name, sgid, "sgroup_UpdateUnlock(%s)", ret ? "success" : "failed" );
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////
///////// Print Game State to the Console



static int printIOP( ItemOfPower* iop, U32 iopId)
{
    char buf[200];

    timerMakeDateStringFromSecondsSince2000(buf, iop->creationTime); 
    printf("\t%s%s (id %d) \tOwned by SG %d \tCreated at %s\n", isUniqueItemOfPower( iop->name )?"*U*":" G ",
        iop->name, iop->id, iop->superGroupOwner, buf );
    return 1;
}

// NumItemsOfPower
static int countItemsOfPower(ItemOfPower* iop, U32 iopId)
{
    if (iop->superGroupOwner == param_sgid)
        param_int++;
    return 1;
}
int NumItemsOfPower(U32 sgid)
{
    param_sgid = sgid;
    param_int = 0;
    cstoreForEach(g_ItemOfPowerStore, countItemsOfPower);
    return param_int;
}
void ShowAllItemsOfPower(void)
{
    char buf[200];

    timerMakeDateStringFromSecondsSince2000(buf, g_IoPGame->startTime);

    printf( "\nCurrent Item Of Power Game: %d, Started: %s State: %s\n", g_IoPGame->id, buf, stringFromGameState(g_IoPGame->state) );
    printf("All Items Of Power:\n");
    cstoreForEach( g_ItemOfPowerStore, printIOP );
}
