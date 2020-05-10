/***************************************************************************
 *     Copyright (c) 2000-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#define _USE_MATH_DEFINES
#include <math.h>
#include "UI/uiBuff.h"
#include <utilitieslib/components/estring.h>
#include "entity/character_workshop.h"
#include "player/inventory_client.h"
#include <utilitieslib/network/netio.h>
#include <utilitieslib/network/net_packetutil.h>
#include "entity/entity.h"
#include "cmdparse/cmdcommon.h"
#include "cmdparse/cmdcontrols.h"
#include <utilitieslib/utils/wininclude.h>
#include "entity/entrecv.h"
#include "entity/entclient.h"
#include "cmdparse/cmdgame.h"
#include <utilitieslib/network/netcomp.h>
#include "clientcomm/clientcomm.h"
#include "UI/uiNet.h"
#include "UI/uiGame.h"
#include "UI/uiConsole.h"
#include "UI/uiWindows.h"
#include "UI/uiWindows_init.h"
#include "player/player.h"
#include <utilitieslib/utils/timing.h>
#include "graphics/font.h"
#include <utilitieslib/assert/assert.h>
#include "graphics/camera.h"
#include "graphics/gfx.h"
#include "varutils.h"
#include "entity/costume_client.h"
#include "entity/entVarUpdate.h"
#include "UI/uiGroupWindow.h"
#include "group/groupdynrecv.h"
#include "comm_game.h"
#include "gameComm/npc.h"            // For npcDefsGetCostume()
#include "entity/PowerInfo.h"
#include "entity/character_base.h"    // For structure Character
#include "entity/character_level.h"
#include "entity/character_net_client.h"
#include "entity/origins.h"
#include "entity/classes.h"
#include "entity/attrib_net.h"
#include "gameComm/initClient.h"
#include "entity/entDebug.h"
#include "entity/entStrings.h"
#include "gameComm/wdwbase.h"
#include "UI/uiNet.h"
#include <utilitieslib/utils/error.h>
#include <utilitieslib/utils/strings_opt.h>
#include "UI/uiCursor.h"
#include "UI/uiTray.h"
#include "UI/uiInspiration.h"
#include <utilitieslib/components/Earray.h>
#include "entity/EntPlayer.h"
#include "player/playerState.h"
#include <utilitieslib/components/StringCache.h>
#include "demo.h"
#include "UI/uiKeybind.h"
#include <utilitieslib/utils/utils.h>
#include "entity/motion.h"
#include "gameData/costume_data.h"
#include "gameComm/dooranimclient.h"
#include "UI/uiChat.h"
#include "UI/sprite/sprite_text.h"
#include "player/badges_client.h"
#include "entity/netfx.h"
#include "UI/uiTarget.h"
#include "seq/seq.h"
#include "gameComm/itemselect.h"
#include "entity/Invention.h"
#include "NovodeX/NwRagdoll.h"
#include "entity/character_net.h"
#include "entity/powers.h"
#include "UI/uiPet.h"
#include "entity/character_workshop.h"
#include "UI/uiOptions.h"
#include "storyarc/contactclient.h"
#include "entity/rewardtoken.h"
#include "entity/Supergroup.h"
#include "bases/basedata.h"
#include "bases/DetailRecipe.h"
#include "UI/uidialog.h"
#include "UI/uiPlayerNote.h"
#include "UI/uiRecipeInventory.h"
#include "float.h"
#include "arena/arenagame.h"
#include "graphics/gfxSettings.h"
#include "UI/uiLogin.h"
#include "UI/uiTailor.h"
#include <utilitieslib/components/StashTable.h>
#include "seq/AnimBitList.h"
#include "graphics/sun.h"
#include <utilitieslib/language/MessageStoreUtil.h>
#include "demo.h"
#include <utilitieslib/utils/Quat.h>
#include "entity/TaskforceParams.h"
#include <utilitieslib/components/HashFunctions.h>
#include "win/win_init.h"
#include "storyarc/playerCreatedStoryarcClient.h"
#include "entity/teamup.h"
#include "UI/uiMissionReview.h"
#include "entity/power_customization.h"
#include "storyarc/pnpcCommon.h"
#include "clientComm/authclient.h"

static U32    last_packet_abs_time;
static int  ent_create;     // Shared between various entRecv helper functions.
// Need this guy over in entclient.c so the deferred costume changes in cc emotes can be recorded
int    demo_recording;

static int    interp_data_level;
static int    interp_data_precision;

static U32    last_abs_time;

F32     server_time;
U32     local_time_bias;
F32     auto_delay = 0.4;


#if 0
    #undef START_BIT_COUNT
    #undef STOP_BIT_COUNT
    #define START_BIT_COUNT(pak, func) PERFINFO_AUTO_START(func, 1)
    #define STOP_BIT_COUNT(pak) PERFINFO_AUTO_STOP()
#endif

// Out of order delete detection scheme:
// Keep a history of at what time (packet id) each svr_idx has been deleted, if we ever
// get a create that is dated before when the specified svr_idx was deleted, then we
// can ignore it.  If we get a delete, and there is an entity with the specified svr_idx
// that was created any time before this delete, it is safe to delete it (even if this delete
// was for a different entity that we haven't received the create fore yet).
typedef struct DeleteHistory { // This structure could just contain the packet_id, the other fields are just for debugging
    U32 pkt_id;
    int idx;
    int id;
} DeleteHistory;

StashTable htDeletes=0;

static void initDeletesHashtable(void)
{
    if (!htDeletes) {
        htDeletes = stashTableCreateInt(256);
    }
}

static void freeQueuedDelete(void* mem)
{
    free(mem);
}

void clearQueuedDeletes(void)
{
    if (htDeletes) {
        stashTableClearEx(htDeletes, NULL, freeQueuedDelete);
    }
}

static DeleteHistory* checkDelete(int idx)
{
    DeleteHistory* pResult;

    if ( htDeletes && stashIntFindPointer(htDeletes, idx, &pResult))
        return pResult;
    return NULL;
}

static void addDelete(U32 pkt_id, int idx, int id)
{
    DeleteHistory *dh = checkDelete(idx);
    if (dh) {
        // This entity has already been deleted once
        if (pkt_id > dh->pkt_id) {
            dh->pkt_id = pkt_id;
            dh->idx = idx;
            dh->id = id;
        } else {
            // The first delete we received was newer
            //printf("Warning: received two deletes out of order for entity with svr_idx=%d and ID=%d or %d\n", idx, id, dh->id);
        }
    } else {
        initDeletesHashtable();
        // this is the first time this srv_idx has been deleted
        dh = malloc(sizeof(DeleteHistory));
        dh->id = id;
        dh->idx = idx;
        dh->pkt_id = pkt_id;
        stashIntAddPointer(htDeletes, idx, dh, false);
    }
}

void entReceiveDeletes(Packet *pak)
{
    int     del_count,i,idx,id;
    Entity* e;
    Entity* controlledPlayer = playerPtr();
    Entity* ownedPlayer = ownedPlayerPtr();

    del_count = pktGetBitsPack(pak,1);
    for(i=0;i<del_count;i++)
    {
        idx = pktGetBitsPack(pak,PKT_BITS_TO_REP_SVR_IDX);
        id = pktGetBitsPack(pak,PKT_BITS_TO_REP_SVR_IDX);
        assert(idx);
        //printf("P%04d IDX%04d ID%04d DEL ", pak->id, idx, id);
        addDelete(pak->id, idx, id);
        e = ent_xlat[idx];

        if(demo_recording)
        {
            demoSetEntIdx(idx);
            demoRecordDelete();
        }

        if(!e)
        { // Never created the entity or
            // We've received a delete for an entity before we got the create
            //printf("Warning: received out of order delete for entity with svr_idx=%d and ID=%d\n", idx, id);
        }
        else if (e->pkt_id_create > pak->id) // We have an entity, but it's newer than the delete message, leave the entity around!
        {
            //printf("Warning: received out of order delete on reused entity with svr_idx=%d and ID=%d\n", idx, id);
        }
        else if (e && e->pkt_id_create < pak->id) // ent may have been re-used before delete message showed up.
        {
            // We have an entity, and it was created before this delete message, kill it!

            if(ent_xlat[idx] == ownedPlayer)
                printf("WARNING: Freeing owned player entity.\n");
            if(ent_xlat[idx] == controlledPlayer)
                printf("WARNING: Freeing controlled player entity.\n");

            e->waitingForFadeOut = 1;
            e->fadedirection = ENT_FADING_OUT;
            ent_xlat[e->svr_idx] = 0;
            e->svr_idx = -1;

            //printf("deleting ent svr_idx %d, id %d, owner %d, type %d\n", idx, id, e->owner, ENTTYPE(e));
        }
    }
    if(demo_recording)
    {
        demoSetEntIdx(0);
    }
}

// See a create on an entity, so create it
// See an entity update, but entity doesn't exist!
//   Assume ent create packet is late, so assign dummy ent until create packet shows up
// If the ent update gfx_idx doesn't match, assume both delete and create haven't showed up yet,
//   so delete old ent, assign dummy ent
// If you see a delete cmd, but it's older than the current ent - wait maybe this can't happen?

static Entity        dummy_ent;
static MotionState    dummy_ent_motion;
static Costume        *dummy_ent_costume = NULL;
static PowerCustomizationList *dummy_ent_powerCust = NULL;

static Entity *checkEntCreate(Packet *pak,int idx,int *ent_create)
{
    int             make_player=0,create,last_ent,type=ENTTYPE_NOT_SPECIFIED, fadein, inPhase, randSeed,db_id=-1,has_name=0;
    Entity          *e;
    char            ent_name[256];
    int             created_vars = FALSE,access_level;
    int             id, do_not_create=0;
    EntityRef       erCreator = 0;
    EntityRef       erOwner = 0;
    const CharacterClass *pclass = NULL;
    const CharacterOrigin *porigin = NULL;
    int             iTitle=0, iRank=0;
    char            pchVillGroupName[1000] = "";
    char            pchVillDefFilename[MAX_PATH] = "";
    char            pchVillInternalName[1000] = "";
    char            pchDisplayClassNameOverride[1000] = "";
    char            favoriteWeaponAnimList[1000] = "";
    char            pnpcName[128] = "";
    int                contactHandle;
    char            pchOriginTitle[128] = "";
    char            pchCommonTitle[128] = "";
    char            pchTheTitle[10] = "";
    int                iBadgeTitle=0;
    int                iTitleColors[2] = {0};
    char            pchSpecialTitle[1000] = "";
    char            pchChatHandle[32] = "";
    char            specialPowerDisplayName[1000] = "";
    char            petName[128] = "";
    PlayerType        playerType = kPlayerType_Hero;
    PlayerSubType    playerSubType = kPlayerSubType_Normal;
    PraetorianProgress    praetorianProgress = kPraetorianProgress_PrimalBorn;
    PlayerType        playerTypeByLocation = kPlayerType_Hero;
    const BasePower    *ppowCreatedMe = 0;
    int                specialPowerState=0;
    U32                bCommandablePet = 0;
    U32                bVisiblePet = 0;
    U32                bDismissablePet = 0;
    bool            playerIsBeingCritter = false;
    int                idDetail = 0;
    
    if(dummy_ent_costume == NULL)
    {
        // JS:    Stupid hack to make sure the entity is initialized correctly.
        //        playerVarAlloc doesn't do this unless the entity is a player entity.
        //        But since the enttype isn't stored in the entity anymore, our static
        //        dummy_ent doesn't get a costume in playerVarAlloc()
        // JE:  playerVarAlloc now does this anyway, but we might need this
        //      costume anyway (since NPCs will blow it away)... I'm leaving this in
        //      for now

        dummy_ent_costume = costume_create(GetBodyPartCount());
        costume_init(dummy_ent_costume);
    }
    if (dummy_ent_powerCust == NULL)
    {
        dummy_ent_powerCust = StructAllocRaw(sizeof(PowerCustomizationList));
    }
    if(dummy_ent.pchar == NULL)
    {
        playerVarAlloc(&dummy_ent, ENTTYPE_PLAYER);

        // HACK: hack hack hack hack hack hack hack hack hack hack hack hack
        porigin = origins_GetPtrFromName(&g_CharacterOrigins, "Natural");
        pclass = classes_GetPtrFromName(&g_CharacterClasses, "Class_Scrapper");
        character_Create(dummy_ent.pchar, &dummy_ent, porigin, pclass);
    }
    if(dummy_ent.pl == NULL)
    {
        dummy_ent.pl = calloc(sizeof(EntPlayer), 1);
    }
    if(dummy_ent.motion == NULL)
    {
        dummy_ent.motion = &dummy_ent_motion;
    }
    // HACK: The costume pointer on the dummy_ent might be blown away if the
    // entity is an NPC. So, reassign it each time to the scratch costume.
    dummy_ent.costume = costume_as_const( dummy_ent_costume );
    //assert(eaSize(&dummy_ent.costume->parts)==GetBodyPartCount());

    create = pktGetBits(pak,1); // full update is for objects that were just created
    if (create)
    {
        DeleteHistory *dh;

        START_BIT_COUNT(pak, "top");

            last_ent = pktGetBits(pak,1);
            if (last_ent)
            {
                //Since sequencer allocates memory to memory pools that can be freed, dummy ent can't be allowed to keep a sequecner
                entSetSeq(&dummy_ent, NULL);

                STOP_BIT_COUNT(pak);

                return 0;
            }

            id = pktGetBitsPack(pak,PKT_BITS_TO_REP_SVR_IDX);

            //printf("P%04d IDX%04d ID%04d CRE ", pak->id, idx, id);

            // Look for this entity in the queued_deletes
            dh = checkDelete(idx);
            if (dh && dh->pkt_id > pak->id) {
                //printf("Not creating old entity that has already been deleted (ID=%d, svr_idx=%d)!\n", id, idx);
                do_not_create=1;
            }

            type = pktGetBitsPack(pak,2);
            if (type == ENTTYPE_PLAYER)
            {
                make_player = pktGetBits(pak,1);
                if (make_player)
                {
                    notifyReceivedCharacter();
                    //printf("*** MAKING PLAYER ***\n");
                    access_level = pktGetBitsPack(pak,1);
                }
                db_id = pktGetBitsPack(pak,PKT_BITS_TO_REP_DB_ID);
            }
            else
            {

                // Get the pet creator and owner if there is one
                // TODO: Will this work when a map is first loaded and there
                //   are also pets created? If a pet is in a lower index slot
                //   than the creator, will this work?
                if(pktGetBits(pak, 1))
                {
                    Entity *pOwner = entFromId(pktGetBitsPack(pak, PKT_BITS_TO_REP_SVR_IDX));
                    if(pOwner!=NULL)
                    {
                        erOwner = erGetRef(pOwner);
                    }

                    pOwner = entFromId(pktGetBitsPack(pak, PKT_BITS_TO_REP_SVR_IDX));
                    if(pOwner!=NULL)
                    {
                        erCreator = erGetRef(pOwner);
                    }

                    if( pktGetBits(pak, 1) )
                        ppowCreatedMe = basepower_Receive(pak, &g_PowerDictionary, 0);
                    else
                        ppowCreatedMe = 0;

                    bCommandablePet = pktGetBits( pak, 1 );
                    bVisiblePet = pktGetBits( pak, 1 );
                    bDismissablePet = pktGetBits( pak, 1 );
                    //TO DO move all pet related info to a single struct in the entity
                    strcpy(specialPowerDisplayName, pktGetIfSetString(pak));
                    specialPowerState = pktGetBitsPack( pak, 1 );
                    strcpy(petName, pktGetIfSetString(pak));
                }
            }

        STOP_BIT_COUNT(pak);

        if(make_player)
        {
            START_BIT_COUNT(pak, "ME");
        }
        else
        {
            switch(type)
            {
                #define CASE(x) case ENTTYPE_##x: START_BIT_COUNT(pak, #x); break

                CASE(PLAYER);
                CASE(CRITTER);
                CASE(NPC);
                CASE(CAR);
                CASE(DOOR);
                default:
                    START_BIT_COUNT(pak, "OTHER");
                    break;

                #undef CASE
            }
        }
            if(type==ENTTYPE_PLAYER || type==ENTTYPE_CRITTER)
            {
                const CharacterClasses *pClasses = &g_CharacterClasses;
                const CharacterOrigins *pOrigins = &g_CharacterOrigins;
                int index;

                // get critter form
                playerIsBeingCritter = pktGetBitsPack(pak,1);

                START_BIT_COUNT(pak, "class/origin");

                    if(type == ENTTYPE_CRITTER || playerIsBeingCritter )
                    {
                        pClasses = &g_VillainClasses;
                        pOrigins = &g_VillainOrigins;
                    }

                    // Get class.

                    index = pktGetBitsPack(pak, 1);

                    if(index >= 0 && index < eaSize(&pClasses->ppClasses))
                        pclass = pClasses->ppClasses[index];

                    // Get origin.

                    index = pktGetBitsPack(pak, 1);

                    if(index >= 0 && index < eaSize(&pOrigins->ppOrigins))
                        porigin = pOrigins->ppOrigins[index];

                    if( pktGetBits( pak, 1 ) )
                    {
                        playerType = pktGetBits(pak, 1); STATIC_INFUNC_ASSERT(kPlayerType_Count <= (1<<1))
                        playerSubType = pktGetBits(pak,2); STATIC_INFUNC_ASSERT(kPlayerSubType_Count <= (1<<2))
                        praetorianProgress = pktGetBits(pak,3);    STATIC_INFUNC_ASSERT(kPraetorianProgress_Count <= (1<<3))
                        iTitle = pktGetBitsPack(pak,5);
                        strcpy(pchTheTitle, pktGetIfSetString(pak));
                        strcpy(pchCommonTitle, pktGetIfSetString(pak));
                        strcpy(pchOriginTitle, pktGetIfSetString(pak));
                        iTitleColors[0] = pktGetBitsAuto(pak);
                        iTitleColors[1] = pktGetBitsAuto(pak);
                        iBadgeTitle = pktGetBits(pak, 32);
                        strcpy(pchSpecialTitle, pktGetIfSetString(pak));
                        strcpy(pchChatHandle, pktGetIfSetString(pak));
                    }

                    if (pktGetBits(pak, 1))
                    {
                        playerTypeByLocation = pktGetBits(pak, 1); STATIC_INFUNC_ASSERT(kPlayerType_Count <= (1<<1))
                    }

                    #ifdef TEST_CLIENT
                        if (!pclass)
                            pclass = g_CharacterClasses.ppClasses[0];
                        if (!porigin)
                            porigin = g_CharacterOrigins.ppOrigins[0];
                    #endif

                STOP_BIT_COUNT(pak);
            }

            START_BIT_COUNT(pak, "name");
                has_name = pktGetBits(pak,1);
                if (has_name)
                    strcpy(ent_name,pktGetString(pak));

                fadein = pktGetBits(pak, 1); //mm is e->fade set on the server?
                inPhase = pktGetBits(pak, 1);

                randSeed = pktGetBits(pak,32); //for synchronizing client gfx
            STOP_BIT_COUNT(pak);

            START_BIT_COUNT(pak, "group");
                if(pktGetBits(pak, 1))
                {
                    iRank = pktGetBitsPack(pak, 2);

                    strcpy(pchVillGroupName, pktGetString(pak));
                    if (pktGetBits(pak, 1))
                    {
                        strcpy(pchVillDefFilename, pktGetString(pak));
                        strcpy(pchVillInternalName, pktGetString(pak));
                    }
                    idDetail = pktGetIfSetBitsPack(pak,5);
                }
                else
                {
                    iRank = 0;
                }
            STOP_BIT_COUNT(pak);

            START_BIT_COUNT(pak, "classNameOverride");
            if(pktGetBits(pak, 1))
            {
                strcpy(pchDisplayClassNameOverride, pktGetString(pak));
            }
            STOP_BIT_COUNT(pak);
            START_BIT_COUNT(pak, "favoriteWeaponAnimList");
            if(pktGetBits(pak, 1))
            {
                strcpy(favoriteWeaponAnimList, pktGetString(pak));
            }
            STOP_BIT_COUNT(pak);
            if(pktGetBits(pak, 1))
            {
                strcpy(pnpcName, pktGetString(pak));
            }
            contactHandle = pktGetBitsAuto(pak);

        STOP_BIT_COUNT(pak);
    }

    e = ent_xlat[idx];
    assertmsg(!e || e->svr_idx == idx, "Please get Garth or another programmer. Entity svr_idx corrupted!"); // this cannot be caused by out of order packets

    if (!e || pak->id > e->pkt_id_create || e->svr_idx != idx)
    {
        // When the new ent packet for the same svr_idx shows up before the delete packet
        if (e && create)
        {
            //printf("Warning: re-using entity (received a second create before a delete) (new svr_idx=%d, ID=%d, old svr_idx=%d, ID=%d)\n", idx, id, e->svr_idx, e->id);
            ent_xlat[idx] = 0;
            if(demo_recording)
            {
                demoSetEntIdx(idx);
                demoRecordDelete();
            }
            e->svr_idx = 0;
            entFree(e);
            e = 0;
        }

        *ent_create = !e;

        if (!e && create && !do_not_create)
        {
            //printf("creating ent svr_idx %d, id %d, type %d\n", idx, id, type);

            e = ent_xlat[idx] = entCreate(0);
            e->randSeed = randSeed; //for synchronizing client gfx
            e->pkt_id_create = pak->id;
            SET_ENTTYPE(e) = type;
            if (!make_player)
            {
                playerVarAlloc(e, type);
                created_vars = TRUE;
                if(type==ENTTYPE_PLAYER || type==ENTTYPE_CRITTER)
                {
                    character_Create(e->pchar, e, porigin, pclass);
                }

                if(type==ENTTYPE_CRITTER)
                {
                    e->erOwner = erOwner;
                    e->erCreator = erCreator;
                    e->pchar->ppowCreatedMe = ppowCreatedMe;
                    e->commandablePet = bCommandablePet;
                    e->petVisibility = bVisiblePet;
                    e->petDismissable = bDismissablePet;

                    if( specialPowerDisplayName && specialPowerDisplayName[0] )
                        e->specialPowerDisplayName = strdup( specialPowerDisplayName );
                    e->specialPowerState = specialPowerState;
                    if( petName && petName[0] ) {
                        if (!e->petName || strlen(petName) > strlen(e->petName)) {
                            SAFE_FREE(e->petName);
                            e->petName = strdup( petName );
                        } else {
                            strcpy(e->petName,petName);
                        }
                    }
                }
            }
            e->db_id = db_id;
            entTrackDbId(e);
        }
    } else if (e && create && e->pkt_id_create > pak->id) {
        // There is an entity sitting around, and that entity has
        // been created *after* this create arrived, therefore
        // this create must be really out of order, just discard it
        e = NULL; // Set to dummy_ent below
        assert(!make_player);
    }

    if (e && pak->id < e->pkt_id_create)
    {
        // This is an update packet for an entity that was created after this update was sent, discard it,
        // it must be refering to some other entity!
        e = NULL;
    }

    if (!e)
    {
        e = &dummy_ent;
        created_vars = TRUE;
    }

    if (make_player)
    {
        //conPrintf("got player %d",idx);
        if (e == &dummy_ent)
            assert(0);
        e->motion->falling = 1;
        e->access_level = access_level;
        playerSetEnt(e);
        strcpy_s(e->auth_name, ARRAY_SIZE_CHECKED(e->auth_name), auth_info.name);
        commGetAccountServerInventory();        // request update to account inventory
    }

    if( create && !created_vars )
    {
        assert(type != ENTTYPE_NOT_SPECIFIED);
        playerVarAlloc(e, type);
        character_Create(e->pchar, e, porigin, pclass);
    }

    if (has_name)
    {
        strcpy( e->name, ent_name);
    }

    if( create && e ) //mm fade
    {
        e->curr_xlucency = 1;
        e->xlucency = 1;

        if (!inPhase)
        {
            e->currfade = 0;
            e->fadedirection = ENT_FADING_OUT;
        }
        else if (fadein)
        {
            e->currfade = 0;
            e->fadedirection = ENT_FADING_IN;
        }
        else
        {
            e->currfade = 255;
            e->fadedirection = ENT_NOT_FADING;
        }

        e->iRank = iRank;

        if( pchVillGroupName[0] != '\0' )
            e->pchVillGroupName = strdup(pchVillGroupName);
        if( pchDisplayClassNameOverride[0] != '\0' )
            e->pchDisplayClassNameOverride = strdup(pchDisplayClassNameOverride);
        if( favoriteWeaponAnimList[0] != '\0' )
            alSetFavoriteWeaponAnimListOnEntity( e, favoriteWeaponAnimList );
        if( pnpcName[0] != '\0')
        {
            e->persistentNPCDef = PNPCFindDef(pnpcName);
            if (e->persistentNPCDef)
            {
                e->bitfieldVisionPhasesDefault = e->persistentNPCDef->bitfieldVisionPhases;
                e->exclusiveVisionPhaseDefault = e->persistentNPCDef->exclusiveVisionPhase;
            }
            else
            {
                e->bitfieldVisionPhasesDefault = 1;
                e->exclusiveVisionPhaseDefault = 0;
            }
        }
        e->contactHandle = contactHandle;
        if( pchVillDefFilename[0] != '\0' )
            e->villainDefFilename = strdup(pchVillDefFilename);
        if( pchVillInternalName[0] != '\0' )
            e->villainInternalName = strdup(pchVillInternalName);

        e->idDetail = idDetail;

        if( e->pl )
        {
            e->pl->isBeingCritter = playerIsBeingCritter;
            e->pl->playerType = playerType;
            e->pl->playerSubType = playerSubType;
            e->pl->praetorianProgress = praetorianProgress;
            e->pl->titleThe = iTitle;
            e->pl->titleColors[0] = iTitleColors[0];
            e->pl->titleColors[1] = iTitleColors[1];
            if(pchTheTitle[0])
                strncpyt( e->pl->titleTheText, pchTheTitle, 10 );
            if(pchCommonTitle[0])
                strncpyt( e->pl->titleCommon, pchCommonTitle, 32 );
            if(pchOriginTitle[0])
                strncpyt( e->pl->titleOrigin, pchOriginTitle, 32 );
            if(iBadgeTitle)
                e->pl->titleBadge = iBadgeTitle;
            if(pchSpecialTitle[0])
                strncpyt( e->pl->titleSpecial, pchSpecialTitle, 128 );
            if(pchChatHandle[0])
            {
                strncpyt( e->pl->chat_handle, pchChatHandle, 32 );
                playerNote_addGlobalName( e->name, e->pl->chat_handle );
            }
        }

        if (e->pchar)
        {
            e->pchar->playerTypeByLocation = playerTypeByLocation;
        }
    }

    if(create)
    {
        e->id = id;
        if(demo_recording)
        {
            demoRecordCreate(e->name);
        }
    }

    *ent_create = create;

    //Since sequencer allocates memory to memory pools that can be freed, dummy ent can't be allowed to keep a sequecner
    entSetSeq(&dummy_ent, NULL);

    return e;
}


static int entGetPosition(Packet *pak,Entity *e,int ent_create,int* sub_pos_out,int* oo_position)
{
    int     update_pos,sub_pos,ipos[3],pos_size,pos_mask,shift,j,pos_info[3];
    int        oo_packet = 0;
    int        lo, hi;

    START_BIT_COUNT(pak, "update_pos");
        update_pos = pktGetBits(pak,3);
    STOP_BIT_COUNT(pak);

    if (!update_pos)
        return 0;

    assert(ARRAY_SIZE(e->pkt_id_pos) == POS_BITS);

    //if(ENTTYPE(e) != ENTTYPE_PLAYER)
    //{
    //    printf("pos_update: %d - %1.3f\t%1.3f\t%1.3f\n", update_pos, e->mat[3][0], e->mat[3][1], e->mat[3][2]);
    //}

    shift = update_pos - 1;
    sub_pos = *sub_pos_out = (update_pos != 7);
    pos_size = 256 << shift;
    pos_mask = 255 << shift;


    if(sub_pos)
    {
        START_BIT_COUNT(pak, "sub pos");
    }
    else
    {
        START_BIT_COUNT(pak, "full pos");
    }
    {
        for(j=0;j<3;j++)
        {
            if (sub_pos)
            {
                pos_info[j] = pktGetBits(pak,8) << shift;
            }
            else
            {
                pos_info[j] = pktGetBits(pak,POS_BITS);
            }
        }
    }
    STOP_BIT_COUNT(pak);

    if(sub_pos)
    {
        lo = shift;
        hi = shift + 8;
    }
    else
    {
        lo = 0;
        hi = POS_BITS;
    }

    PERFINFO_AUTO_START("check ids", 1);
    {
        U32 pak_id = pak->id;
        int bit, bit_id;
        int net_ipos[3];

        copyVec3(e->net_ipos, net_ipos);

        if (sub_pos)
        {
            copyVec3(net_ipos,ipos);
            ipos[0] = (ipos[0] & ~pos_mask) | pos_info[0];
            ipos[1] = (ipos[1] & ~pos_mask) | pos_info[1];
            ipos[2] = (ipos[2] & ~pos_mask) | pos_info[2];
        }
        else
        {
            copyVec3(pos_info, ipos);
        }

        for(bit_id = lo, bit = 1 << lo; bit_id < hi; bit_id++, bit <<= 1)
        {
            if(pak_id < e->pkt_id_pos[bit_id])
            {
                oo_packet = 1;
                ipos[0] = (ipos[0] & ~bit) | (net_ipos[0] & bit);
                ipos[1] = (ipos[1] & ~bit) | (net_ipos[1] & bit);
                ipos[2] = (ipos[2] & ~bit) | (net_ipos[2] & bit);
            }
            else
            {
                e->pkt_id_pos[bit_id] = pak->id;
            }
        }

        for(j = 0; j < 3; j++)
        {
            int t;
            t = ipos[j];
            t -= (1 << (POS_BITS-1));
            e->net_pos[j] = t / POS_SCALE;
        }

        copyVec3(ipos,e->net_ipos);

        *oo_position = oo_packet;
    }
    PERFINFO_AUTO_STOP();

    return 1;
}

static int entGetOrientation(Packet *pak,Entity *e)
{
    int     update_qrot,j;
    F32     qelem;

    START_BIT_COUNT(pak, "update_qrot");
        update_qrot = pktGetIfSetBits(pak,3);
    STOP_BIT_COUNT(pak);

    if (update_qrot)
    {
        int oo_packet = 0;

        for(j=0;j<3;j++)
        {
            if (update_qrot & (1 << j))
            {
                PERFINFO_RUN(
                    switch(j){
                        case 0:START_BIT_COUNT(pak, "qrotX");break;
                        case 1:START_BIT_COUNT(pak, "qrotY");break;
                        case 2:START_BIT_COUNT(pak, "qrotZ");break;
                    }
                );
                    qelem = unpackQuatElem(pktGetBits(pak,9),9);
                STOP_BIT_COUNT(pak);

                if (pak->id > e->pkt_id_qrot[j])
                {
                    e->pkt_id_qrot[j] = pak->id;
                    e->net_qrot[j] = qelem;
                }
                else
                {
                    oo_packet = 1;
                }
            }
        }

        // We changed one of our qrot elements, so we need to recalc W
        // Note that W is assumed positive, so be sure we packed it that way
        quatCalcWFromXYZ(e->net_qrot);
        quatNormalize(e->net_qrot);
        //quatInverse(e->net_qrot, e->net_qrot);


        return !oo_packet;
    }
    return 0;
}


// Grab the bits as if you are going to use the ragdoll, but ignore it
static int entGetRagdollIgnore(Packet *pak,Entity *e)
{
    U8     num_bones;

    START_BIT_COUNT(pak, "ragdoll_num_bones");
    num_bones = pktGetIfSetBits(pak,5);
    STOP_BIT_COUNT(pak);

    if (num_bones)
    {
        START_BIT_COUNT(pak, "ragdollQuats");
        {
            int j;
            for (j=0; j<num_bones*3;++j)
            {
                pktGetBits(pak,NUM_BITS_PER_RAGDOLL_QUAT_ELEM);
            }
        }
        STOP_BIT_COUNT(pak);
        return 1;
    }
    return 0;
}

#ifdef RAGDOLL
static int entGetRagdoll(Packet *pak,Entity *e)
{
    bool transitioning_out = false;
    U8     num_bones;
    int res = 0;
    
    START_BIT_COUNT(pak, "ragdoll_num_bones");
    num_bones = pktGetIfSetBits(pak,5);
    STOP_BIT_COUNT(pak);

    if (num_bones)
    {
        bool bCreatedRagdoll = false;

        assert( e->seq );
        if (!e->ragdoll)
        {
            Mat4 mWorldSpaceMat;
            mulMat4(ENTMAT(e), e->seq->gfx_root->child->mat, mWorldSpaceMat);

            e->ragdoll = nwCreateRagdollNoPhysics(e);

            mat3ToQuat(mWorldSpaceMat, e->ragdoll->qCurrentRot);
            quatNormalize(e->ragdoll->qCurrentRot);

            bCreatedRagdoll = true;
        }

        START_BIT_COUNT(pak, "ragdollQuats");
        {
            int j;

            if (!e->ragdollCompressed)
                e->ragdollCompressed = malloc(sizeof(int) * num_bones * 3);

            // First, stamp the current state of the ragdoll (corresponding to the anim state)
            // To our ragdoll history, so we interp from where we are to the first real ragdoll
            // frame
            if ( bCreatedRagdoll && e->ragdoll )
            {
                compressRagdoll(e->ragdoll, e->ragdollCompressed);
                decompressRagdoll(e->ragdoll, e->ragdollCompressed, global_state.client_abs );
            }

            for (j=0; j<num_bones*3;++j)
            {
                e->ragdollCompressed[j] = pktGetBits(pak,NUM_BITS_PER_RAGDOLL_QUAT_ELEM);
            }

            if( e->ragdoll ) // otherwise, the info is just lost...
                decompressRagdoll(e->ragdoll, e->ragdollCompressed, global_state.abs_time );

        }
        STOP_BIT_COUNT(pak);
        res = 1;

//         printf("entGetRagdoll(svr_time;client_time),%i,%i\n",global_state.abs_time,global_state.client_abs);
    }
    else
    {
        if (e->ragdoll && e->ragdollFramesLeft <= 0)
        {
//             printf("entGetRagdoll(ragdollframesleft=>.8),%f\n",e->ragdollFramesLeft);
            e->maxRagdollFramesLeft = 0.8f;
            e->ragdollFramesLeft = e->maxRagdollFramesLeft;
            zeroVec3(e->ragdollHipBonePos);
            assert(e->nonRagdollBoneRecord == NULL);
            initNonRagdollBoneRecord(&e->nonRagdollBoneRecord, e->seq, e->seq->gfx_root->child);
            transitioning_out = true;
        }
    }    
    
    if((res || transitioning_out) && demoIsRecording() && e && pak)
    {
        char* pcHexString = num_bones?hexStringFromIntList(e->ragdollCompressed, num_bones*3):NULL;
        demoSetEntIdx(e->svr_idx);
        demoRecord("EntRagdoll %i %i %i %s",num_bones,
                   global_state.abs_time, global_state.client_abs,
                   (num_bones?pcHexString:""));
        if (pcHexString)
            free(pcHexString);
        /*
        printf("Hips: %d %d %d\n", e->ragdollCompressed[30], e->ragdollCompressed[31], e->ragdollCompressed[32]);
        if ( num_bones )
        {
            const char* pcString = hexStringFromIntList(e->ragdollCompressed, num_bones * 3);
            int* pIntList = intListFromHexString(pcString, num_bones * 3);
            int iDiff = memcmp(pIntList, e->ragdollCompressed, ARRAY_SIZE(e->ragdollCompressed));
            U32 uiHash = hashString(pcString, true);
            printf("Hash = 0x%x\n", uiHash);
            printf("Diff = %d\n", iDiff);
        }
        */
    }
    
    return res;
}

int entGetRagdoll_ForDemoPlayback(Entity *e, int num_bones, int *ragdollCompressed, int svr_last_frame_dt, int svr_client_dt, int frame_dt)
{
    int res = 0;
    
    if (num_bones)
    {
        bool bCreatedRagdoll = false;
        //printf("Hips: %d %d %d\n", ragdollCompressed[30], ragdollCompressed[31], ragdollCompressed[32]);

        assert( e->seq );
        if (!e->ragdoll)
        {
            Mat4 mWorldSpaceMat;
            mulMat4(ENTMAT(e), e->seq->gfx_root->child->mat, mWorldSpaceMat);

            e->ragdoll = nwCreateRagdollNoPhysics(e);

            mat3ToQuat(mWorldSpaceMat, e->ragdoll->qCurrentRot);
            quatNormalize(e->ragdoll->qCurrentRot);

            bCreatedRagdoll = true;
        }

        {
            int j;
            S32 svr_time = 0;

            if (!e->ragdollCompressed)
                e->ragdollCompressed = malloc(sizeof(int) * num_bones * 3);

            // First, stamp the current state of the ragdoll (corresponding to the anim state)
            // To our ragdoll history, so we interp from where we are to the first real ragdoll
            // frame
            if ( bCreatedRagdoll && e->ragdoll )
            {
                compressRagdoll(e->ragdoll, e->ragdollCompressed);
                decompressRagdoll(e->ragdoll, e->ragdollCompressed, global_state.client_abs + frame_dt);
            }

            for (j=0; j<num_bones*3;++j)
                e->ragdollCompressed[j] = ragdollCompressed[j];

//             for (j=0; j<=e->ragdoll->hist_latest; ++j)
//                 svr_time = MAX((U32)svr_time,e->ragdoll->absTimeHist[j]);

            if(!bCreatedRagdoll && e->ragdoll)
                svr_time = e->ragdoll->absTimeHist[e->ragdoll->hist_latest] + svr_last_frame_dt;
            else
                svr_time = global_state.client_abs + frame_dt + svr_client_dt;


            if( e->ragdoll ) // otherwise, the info is just lost...
            {
                //printf("Decompress Hips: %d %d %d\n", e->ragdollCompressed[30], e->ragdollCompressed[31], e->ragdollCompressed[32]);
                decompressRagdoll(e->ragdoll, e->ragdollCompressed, svr_time);
//                 printf("demoPlayFrame(num_bones;svr_time;demo_time),%i,%i,%i\n",
//                     num_bones,svr_time,global_state.client_abs);
            }
        }

        res = 1;
    }
    else    {
        if (e->ragdoll && e->ragdollFramesLeft <= 0)
        {
//             printf("entGetRagdoll(ragdollframesleft=>.8),%f\n",e->ragdollFramesLeft);
            e->maxRagdollFramesLeft = 0.8f;
            e->ragdollFramesLeft = e->maxRagdollFramesLeft;
            zeroVec3(e->ragdollHipBonePos);
            assert(e->nonRagdollBoneRecord == NULL);
            initNonRagdollBoneRecord(&e->nonRagdollBoneRecord, e->seq, e->seq->gfx_root->child);
        }
    }
    
    return res;
}
#endif








char commandStrings[8][100];

void debugShowNetfx( Entity * e, NetFx * netfx )
{
    if( !e->seq )
        return;
    strcpy( commandStrings[CREATE_ONESHOT_FX],        "ONESHOT   " );
    strcpy( commandStrings[CREATE_MAINTAINED_FX],    "MAINTAINED" );
    strcpy( commandStrings[DESTROY_FX],                "DESTROY   " );

    printf("   %s %.3d %3.1f/%.2d %s %s  ",
        e->seq->type->name, netfx->net_id, netfx->clientTimer, netfx->clientTriggerFx,  commandStrings[netfx->command], stringFromReference( netfx->handle ) );
}

//Get all the FX sent down by the server for this entity.  Will be processed in the next entClientUpdate.
NetFx *entrecvAllocNetFx(Entity *e)
{
    NetFx   *newNetfx;

    newNetfx = dynArrayAdd(&e->netfx_list,sizeof(e->netfx_list[0]),&e->netfx_count,&e->netfx_max,1);

    return newNetfx;
}
// paired with entsend.c:entSendNetFx
static void entReceiveNetFx(Packet *pak,Entity *e)
{
    int     i,count;
    NetFx   *netfx;

    count = pktGetBitsPack(pak,1);
    for(i=0;i<count;i++)
    {
        START_BIT_COUNT(pak, "netfx");
        netfx                    = entrecvAllocNetFx(e);
        netfx->command            = pktGetBits(pak,2);
        netfx->net_id            = pktGetBits(pak,32);
        netfx->pitchToTarget    = pktGetBits(pak,1);//move this out of fx someday...
        netfx->powerTypeflags    = pktGetIfSetBits(pak,4);
        netfx->handle            = pktGetIfSetBitsPack(pak,12);
        netfx->hasColorTint     = pktGetBits(pak,1);
        if (netfx->hasColorTint) {
            netfx->colorTint.primary.integer = pktGetBits(pak, 32);
            netfx->colorTint.secondary.integer = pktGetBits(pak, 32);
        }
        netfx->clientTimer        = pktGetIfSetBitsPack(pak,4);
        netfx->clientTriggerFx    = pktGetIfSetBits(pak,32);
        netfx->duration            = pktGetIfSetF32( pak );
        netfx->radius            = pktGetIfSetF32( pak );
        netfx->power            = pktGetIfSetBits( pak, 4 );
        netfx->debris            = pktGetIfSetBits( pak, 32 );

        netfx->originType = pktGetIfSetBits( pak, 2 );
        if( netfx->originType == NETFX_POSITION )
        {
            netfx->originPos[0] = pktGetF32( pak );
            netfx->originPos[1] = pktGetF32( pak );
            netfx->originPos[2] = pktGetF32( pak );
        }
        else if ( netfx->originType == NETFX_ENTITY )
        {
            netfx->originEnt    = pktGetIfSetBitsPack(pak,8);
            netfx->bone_id      = pktGetBitsPack(pak,2);
        }
        else Errorf( textStd("NetBug") );

        //Send the Target
        netfx->targetType = pktGetIfSetBits( pak, 2 );
        if( netfx->targetType == NETFX_POSITION )
        {
            netfx->targetPos[0] = pktGetF32( pak );
            netfx->targetPos[1] = pktGetF32( pak );
            netfx->targetPos[2] = pktGetF32( pak );
            netfx->targetEnt = pktGetIfSetBitsPack(pak,12);
        }
        else if( netfx->targetType == NETFX_ENTITY )
        {
            netfx->targetEnt = pktGetIfSetBitsPack(pak,12);
        }
        else Errorf( textStd("NetBug") );
        STOP_BIT_COUNT(pak);

        assert(netfx->command);
        assert(netfx->net_id);

        //Demoplayer
        if(demo_recording)
        {
            demoSetAbsTimeOffset((netfx->clientTimer * ABSTIME_SECOND)/30);//last_packet_abs_time + (netfx->clientTimer * ABSTIME_SECOND)/30 - demoGetAbsTime());
            demoRecordFx(netfx);
            demoSetAbsTimeOffset(0);
        }
        //End demoplayer

        //Debug
        if( game_state.netfxdebug && e == SELECTED_ENT  )
        {
            debugShowNetfx( e, netfx );
        }
        //End Debug

    }
}

float decodeOffset(char char_offset, int max_bits, int cur_bits)
{
    static float offset_table[131];
    static int offset_table_init = 0;

    int abs_offset;

    if(!char_offset)
    {
        return 0;
    }

    if(!offset_table_init)
    {
        void entGetNetInterpOffsets(float* array, int size);

        entGetNetInterpOffsets(offset_table, ARRAY_SIZE(offset_table));

        offset_table_init = 1;
    }

    abs_offset = abs(char_offset);

    return offset_table[abs_offset + 1] * (char_offset < 0 ? -1 : 1) * (1 << (max_bits - cur_bits));
}

static const int binpos_parent[7] = { 0, 0, 0, 1, 1, 2, 2 };

static int entReceiveInterpPositions(Packet* pak, Entity* e, InterpPosition ip[7])
{
    int bits = 5 + interp_data_precision;
    char binpos_level[7] = { 1, 2, 2, 3, 3, 3, 3 };
    int i;
    int got_interp_pos = 0;

    START_BIT_COUNT(pak, "BinTrees");
        START_BIT_COUNT(pak, "init");

            if(pktGetBits(pak, 1))
            {
                ip[0].y_used = 1;
                ip[0].xz_used = 1;
            }
            else
            {
                START_BIT_COUNT(pak, "extra bit");
                    ip[0].y_used = pktGetBits(pak, 1);
                STOP_BIT_COUNT(pak);

                ip[0].xz_used = !ip[0].y_used;
            }

        STOP_BIT_COUNT(pak);

        START_BIT_COUNT(pak, "Y");

            // Get the Y tree.

            for(i = 0; i < 7; i++)
            {
                if((!i || ip[binpos_parent[i]].y_used) && binpos_level[i] <= interp_data_level)
                {
                    if(i)
                    {
                        ip[i].y_used = pktGetBits(pak,1);
                    }

                    if(ip[i].y_used)
                    {
                        int get_bits = bits - binpos_level[i];
                        int shift_bits = 8 - get_bits;
                        int negative;

                        got_interp_pos = 1;

                        negative = pktGetBits(pak,1);
                        ip[i].offsets[1] = pktGetBits(pak,get_bits);
                        if(negative)
                            ip[i].offsets[1] *= -1;

                        //if(e->type == ENTTYPE_CRITTER)
                        //{
                        //    printf(    "recv y:  %d\n",
                        //            abs(ip[i].offsets[1]) & ((1 << get_bits) - 1));
                        //}
                    }
                }
                else
                {
                    ip[i].y_used = 0;
                }
            }

        STOP_BIT_COUNT(pak);

        START_BIT_COUNT(pak, "XZ");

            // Get the XZ tree.

            for(i = 0; i < 7; i++)
            {
                if((!i || ip[binpos_parent[i]].xz_used) && binpos_level[i] <= interp_data_level)
                {
                    if(i)
                    {
                        ip[i].xz_used = pktGetBits(pak,1);
                    }

                    if(ip[i].xz_used)
                    {
                        int get_bits = bits - binpos_level[i];
                        int shift_bits = 8 - get_bits;
                        int negative;

                        got_interp_pos = 1;

                        negative = pktGetBits(pak,1);
                        ip[i].offsets[0] = pktGetBits(pak,get_bits);
                        if(negative)
                            ip[i].offsets[0] *= -1;

                        negative = pktGetBits(pak,1);
                        ip[i].offsets[2] = pktGetBits(pak,get_bits);
                        if(negative)
                            ip[i].offsets[2] *= -1;

                        //if(e->type == ENTTYPE_CRITTER)
                        //{
                        //    printf(    "recv xz: %d\t%d\n",
                        //            abs(ip[i].offsets[0]) & ((1 << get_bits) - 1),
                        //            abs(ip[i].offsets[2]) & ((1 << get_bits) - 1));
                        //}
                    }
                }
                else
                {
                    ip[i].xz_used = 0;
                }
            }

        STOP_BIT_COUNT(pak);
    STOP_BIT_COUNT(pak);

    return got_interp_pos;
}

static void entReceiveUnpackInterpPositions(Entity* e, InterpPosition ip[7])
{
    // This array translates from the binary-tree-position index to the actual position-array index.

    static const int binpos_to_pos[9] = { 3, 1, 5, 0, 2, 4, 6, 7, 8 };

    // This array says which position-array indexes are the parent position of the binary-tree-position.
    // Indexes 0-6 are the middle position because the math is nicer, so you get this beautifulness:
    //
    //        7 = source position.
    //        8 = target position.

    static const int binpos_to_parentbinpos[7][2] = {
        { 7, 8 },
        { 7, 0 },
        { 0, 8 },
        { 7, 1 },
        { 1, 0 },
        { 0, 2 },
        { 2, 8 }
    };

    // This array says how many bits to use for each binary-tree position index.

    static const char binpos_level[7] = { 1, 2, 2, 3, 3, 3, 3 };

    MotionHist mh_head = e->motion_hist[e->motion_hist_latest];
    MotionHist* mh_prev = e->motion_hist + (e->motion_hist_latest - 1 + ARRAY_SIZE(e->motion_hist)) % ARRAY_SIZE(e->motion_hist);
    U32 base_time = mh_prev->abs_time;
    Quat base_qrot;
    int i;
    Vec3 unpacked_pos[9];
    int used[7];
    int used_count = 0;

    // Interp the quat until I come up with something better..

    copyQuat(mh_prev->qrot, base_qrot);

    /*
    pyr_diff[0] = subAngle(mh_head.pyr[0], base_pyr[0]);
    pyr_diff[1] = subAngle(mh_head.pyr[1], base_pyr[1]);
    pyr_diff[2] = subAngle(mh_head.pyr[2], base_pyr[2]);
    */

    // Copy the source and target position so they can be looked up as parent positions.

    copyVec3(mh_prev->pos, unpacked_pos[7]);
    copyVec3(mh_head.pos, unpacked_pos[8]);

    #define PRINT_OFFSETS 0

    for(i = 0; i < 7; i++)
    {
        if(ip[i].y_used || ip[i].xz_used)
        {
            Vec3 temp_pos;
            Vec3 diff_vec;
            int max_bits = 5 + interp_data_precision;
            int cur_bits = max_bits + 1 - binpos_level[i];

            // Calculate the midpoint.

            copyVec3(unpacked_pos[binpos_to_pos[binpos_to_parentbinpos[i][0]]], temp_pos);
            addVec3(unpacked_pos[binpos_to_pos[binpos_to_parentbinpos[i][1]]], temp_pos, temp_pos);
            scaleVec3(temp_pos, 0.5, temp_pos);

            copyVec3(temp_pos, diff_vec);

            if(ip[i].y_used)
            {
                temp_pos[1] += decodeOffset(ip[i].offsets[1], max_bits, cur_bits);

                #if PRINT_OFFSETS
                    printf(    "%d: adding to Y: %f\t(%d, %d)\n",
                            i,
                            decodeOffset(ip[i].offsets[1], max_bits, cur_bits),
                            cur_bits,
                            ip[i].offsets[1]);
                #endif
            }

            if(ip[i].xz_used)
            {
                temp_pos[0] += decodeOffset(ip[i].offsets[0], max_bits, cur_bits);
                temp_pos[2] += decodeOffset(ip[i].offsets[2], max_bits, cur_bits);

                #if PRINT_OFFSETS
                    printf(    "%d: adding to X: %f\t(%d, %d)\n",
                            i,
                            decodeOffset(ip[i].offsets[0], max_bits, cur_bits),
                            cur_bits,
                            ip[i].offsets[0]);

                    printf(    "%d: adding to Z: %f\t(%d, %d)\n",
                            i,
                            decodeOffset(ip[i].offsets[2], max_bits, cur_bits),
                            cur_bits,
                            ip[i].offsets[2]);
                #endif
            }

            subVec3(temp_pos, diff_vec, diff_vec);

            copyVec3(temp_pos, unpacked_pos[binpos_to_pos[i]]);
            used[binpos_to_pos[i]] = 1;
            used_count++;
        }
        else
        {
            used[binpos_to_pos[i]] = 0;
        }
    }

    #define PRINT_POSITIONS 0

    #if PRINT_POSITIONS
        if(e == current_target)
        {
            printf("%d : %d --------------------------------------------------------------\n", e->svr_idx, ABS_TIME);

            printf("%f\t%f\t%f\t\t%f\t%f\t%f\n", vecParamsXYZ(mh_prev->pos), quatParamsXYZW(mh_prev->qrot));
        }
    #endif

    if(used_count)
    {
        U32 time_step = mh_head.abs_time - base_time;

        //memmove(mh + 1 + used_count, mh + 1, sizeof(*mh) * (ARRAY_SIZE(e->motion_hist) - used_count - 1));

        for(i = 0; i < 7; i++)
        {
            if(used[i])
            {
                float scale = (float)(i + 1) / 8.0f;
//                int k;
                MotionHist* mh = e->motion_hist + e->motion_hist_latest;

                mh->abs_time = base_time + (time_step * (i+1)) / 8;
                copyVec3(unpacked_pos[i], mh->pos);
                // interp quat
                quatInterp(scale, base_qrot, mh_head.qrot, mh->qrot );
                /*
                for(k = 0; k < 3; k++)
                    mh->pyr[k] = addAngle(base_pyr[k], pyr_diff[k] * scale);
                    */

                #if PRINT_POSITIONS
                    if(e == current_target)
                    {
                        printf("%f\t%f\t%f\t\t%f\t%f\t%f\n", vecParamsXYZ(mh->pos), quatParamsXYZW(mh->qrot));
                    }
                #endif

                if(demo_recording)
                {
                    demoSetAbsTimeOffset(mh->abs_time - demoGetAbsTime());
                    demoRecordPos(mh->pos);
                    demoSetAbsTimeOffset(0);
                }

                e->motion_hist_latest = (e->motion_hist_latest + 1) % ARRAY_SIZE(e->motion_hist);
            }
        }

        e->motion_hist[e->motion_hist_latest] = mh_head;
    }

    #if PRINT_POSITIONS
        if(e == current_target)
        {
            printf(    "%f\t%f\t%f\t\t%f\t%f\t%f\n",
                    vecParamsXYZ(e->motion_hist[e->motion_hist_latest].pos),
                    quatParamsXYZW(e->motion_hist[e->motion_hist_latest].qrot));
        }
    #endif
}

static void entReceivePosUpdate(Packet *pak, Entity *e, int ent_create, int odd_send)
{
    int move_instantly=0;
    InterpPosition ip[7];
    int got_qrot=0,got_pos=0;
    int oo_position = 0;
    int got_interp_pos = 0;
    int sub_pos = 0;

    START_BIT_COUNT(pak, "entGetPosition");
        got_pos = entGetPosition(pak, e, ent_create, &sub_pos, &oo_position);

        if(ent_create)
        {
            move_instantly = 1;
            memset(ip, 0, sizeof(ip));
        }
        else if(got_pos)
        {
            int extra_info;
            int gotvecs = 0;

            START_BIT_COUNT(pak, "extras");
                START_BIT_COUNT(pak, "extra_info");
                    extra_info = pktGetBits(pak, 1);
                STOP_BIT_COUNT(pak);

                if(extra_info)
                {
                    // Receive the bit that makes the movement instantaneous instead of interpolated.

                    START_BIT_COUNT(pak, "move_instantly");
                        move_instantly = pktGetBits(pak, 1);
                    STOP_BIT_COUNT(pak);

                    // Receive the interp offset data.

                    if(!move_instantly && interp_data_level)
                    {
                        got_interp_pos = entReceiveInterpPositions(pak, e, ip);
                    }
                }
                else
                {
                    move_instantly = 0;
                    memset(ip, 0, sizeof(ip));
                }
            STOP_BIT_COUNT(pak);
        }
    STOP_BIT_COUNT(pak);

    if(e->seq)
    {
        e->seq->moved_instantly = move_instantly;
    }

    START_BIT_COUNT(pak, "entGetOrientation");
        got_qrot = entGetOrientation(pak,e);
    STOP_BIT_COUNT(pak);




    PERFINFO_AUTO_START("store position", 1);
        if(    (got_pos || got_qrot ) &&
            !server_visible_state.pause &&
            (odd_send || !ENTINFO(e).send_on_odd_send))
        {
            MotionHist* mh = e->motion_hist + e->motion_hist_latest;

            if(!oo_position)
            {
                e->motion_hist_latest = (e->motion_hist_latest + 1) % ARRAY_SIZE(e->motion_hist);

                mh = e->motion_hist + e->motion_hist_latest;

                mh->abs_time = global_state.abs_time;
            }

            copyVec3(e->net_pos,mh->pos);
            copyQuat(e->net_qrot,mh->qrot);

            if(!oo_position)
            {
                if(move_instantly)
                {
                    mh->abs_time = global_state.client_abs;
                }
                else if(got_interp_pos && interp_data_level)
                {
                    entReceiveUnpackInterpPositions(e, ip);
                }
            }
        }
    PERFINFO_AUTO_STOP();

#ifndef TEST_CLIENT // TestClients always want to have the latest position!
    if (ent_create)
#endif
    {
        Mat4 newmat;
        copyVec3(e->net_pos,newmat[3]);
        quatToMat(e->net_qrot, newmat);
        entUpdateMat4Interpolated(e, newmat);
        //createMat3YPR(e->mat,e->net_pyr);
    }

    PERFINFO_AUTO_START("demo", 1);
        if(demo_recording)
        {
            if (got_pos)
                demoRecordPos(e->net_pos);
            if (got_qrot)
            {
                Mat3 mat;
                Vec3 pyr;
                //ST: keeping demo's PYR based for now, so generate a PYR
                // from our qrot
                quatToMat(e->net_qrot, mat);
                getMat3YPR(mat, pyr);
                demoRecordPyr(pyr);
            }
        }
    PERFINFO_AUTO_STOP();
}

static int entReceiveSeqMoveUpdate(Packet *pak, Entity *e, bool oo_packet)
{
    int move_updated = pktGetBits(pak,1);

    if (move_updated)
    {
        int move_idx;
        U16    net_move_change_time;
        U16    move_bits = 0; STATIC_INFUNC_ASSERT(NEXTMOVEBIT_COUNT <= 16);

        /*Unravel move update so e->move_update is only set if the move has actually been updated*/
        move_idx = pktGetIfSetBitsPack(pak,8);
        net_move_change_time = pktGetIfSetBitsPack(pak,4);
        move_bits = pktGetIfSetBits(pak,NEXTMOVEBIT_COUNT);

        if (!oo_packet) // Do not update if this is received out of order?
        {
            if(ENTINFO(e).send_on_odd_send )
                e->move_change_timer = 0;
            else
                e->move_change_timer = net_move_change_time + auto_delay - TIMESTEP + (last_abs_time - global_state.client_abs) / 100.0;

            if( e->move_change_timer < 0.0 )
            {
                e->move_change_timer = 0.0f;
            }

            assert(move_idx >= 0);
            e->next_move_idx = move_idx;
            e->next_move_bits = move_bits;
            if(demo_recording && e->seq)
            {
                demoSetAbsTimeOffset((net_move_change_time * ABSTIME_SECOND)/30);//last_packet_abs_time + (net_move_change_time * ABSTIME_SECOND)/30 - demoGetAbsTime());
                demoRecordMove(e,move_idx,move_bits);
                demoSetAbsTimeOffset(0);
            }
        }
    }
    return move_updated;
}


/*If the server sends moves to be triggered later, read each one into an Entity->TriggeredMove
struct to be handled next time clientUpdate is called for this ent.
*/
static int entReceiveSeqTriggeredMoves(Packet * pak, Entity * e, bool oo_packet)
{
    TriggeredMove * tm;
    TriggeredMove dummy;
    int i, count;
    U8 net_move_change_time = 0; //Get move change time for each one and delay by that much

    count = pktGetBitsPack(pak,1);
    for(i = 0 ; i < count; i++)
    {
        if(e->triggered_move_count >= MAX_TRIGGEREDMOVES)
        {
            if( 0 )Errorf("NETMOVE ERROR: Too many triggered moves recv'd before clientUpdate was called to handle them\n");
            tm = &dummy;
        }
        else if(oo_packet)
        {
            tm = &dummy;
        }
        else
        {
            tm = &e->triggered_moves[e->triggered_move_count++];
        }

        tm->idx                = pktGetBitsPack(pak,10);
        tm->ticksToDelay    = pktGetBitsPack(pak,6) + net_move_change_time;
        tm->triggerFxNetId  = pktGetIfSetBitsPack(pak,16);
    }
    return count;
}

static int entReceiveLogoutUpdate(Packet *pak, Entity *e, bool oo_packet) {
    int     logout_update = pktGetBits(pak,1);
    if (logout_update)
    {
        unsigned int logout_bad_connection = pktGetBits(pak,1);
        F32 logout_timer = 30 * pktGetIfSetBitsPack(pak,5);
        if (!oo_packet) {
            e->logout_bad_connection = logout_bad_connection;
            e->logout_timer = logout_timer;
        }
    }
    return logout_update;
}

static int entReceiveTitles(Packet *pak, Entity *e, bool oo_packet)
{
    if(pktGetBits(pak, 1))
    {
        char name[MAX_NAME_LEN];
        int titleThe;
        char titleTheText[10];
        char titleCommon[32];
        char titleOrigin[32];
        int titleBadge;
        char titleSpecial[128];
        int glowieUnlocked;
        int gender;
        int name_gender;
        int attachArticle;
        int respecTokens;
        int lastTurnstileEventID;
        int lastTurnstileMissionID;
        int helperStatus;
        int titleColors[2] = {0};


        strncpyt( name, pktGetString(pak), ARRAY_SIZE(e->name) );
        titleThe = pktGetBitsPack( pak, 5 );
        strncpyt( titleTheText, pktGetIfSetString( pak ), 10);
        strncpyt( titleCommon, pktGetIfSetString( pak ), 32);
        strncpyt( titleOrigin, pktGetIfSetString( pak ), 32);
        titleColors[0] = pktGetBitsAuto(pak);
        titleColors[1] = pktGetBitsAuto(pak);
        titleBadge = pktGetBits( pak, 32 );
        strncpyt( titleSpecial, pktGetIfSetString( pak ), 128);
        glowieUnlocked = pktGetBits(pak, 1);
        gender = pktGetBits(pak, 2);
        name_gender = pktGetBits( pak, 2 );
        attachArticle = pktGetBits(pak, 1);
        respecTokens = pktGetBits( pak, 32 );
        lastTurnstileEventID = pktGetBits(pak, 32);
        lastTurnstileMissionID = pktGetBits(pak,32);
        helperStatus = pktGetBitsAuto(pak);
        if(e && e->pl && !oo_packet)
        {
            strncpyt( e->name, name, ARRAY_SIZE(e->name) );
            e->pl->titleThe = titleThe;
            strncpyt( e->pl->titleTheText, titleTheText, 10);
            strncpyt( e->pl->titleCommon, titleCommon, 32);
            strncpyt( e->pl->titleOrigin, titleOrigin, 32);
            e->pl->titleColors[0] = titleColors[0];
            e->pl->titleColors[1] = titleColors[1];
            e->pl->titleBadge = titleBadge;
            strncpyt( e->pl->titleSpecial, titleSpecial, 128);
            e->pl->glowieUnlocked = glowieUnlocked;
            e->gender = gender;
            e->name_gender = name_gender;
            e->pl->attachArticle = attachArticle;
            e->pl->respecTokens = respecTokens;
            e->pl->lastTurnstileEventID = lastTurnstileEventID;
            e->pl->lastTurnstileMissionID = lastTurnstileMissionID;
            e->pl->helperStatus = helperStatus;
        }

        return 1;
    }

    return 0;
}

static int entReceiveXlucency(Packet *pak, Entity *e, bool oo_packet)
{
    F32 xlucency = ((float)(255-pktGetIfSetBits(pak,8)))/255.0f;

    if (!oo_packet)
    {
        e->xlucency = xlucency;
    }

    return 1;
}
static int menuCanSetCostumeAndPowerCust()
{
#if !TEST_CLIENT
    return (!isCurrentMenu_PowerCustomization() && !isMenu(MENU_LOAD_POWER_CUST) && !isMenu(MENU_GENDER) && !isMenu(MENU_BODY)
        && !isMenu(MENU_TAILOR) && !isMenu(MENU_SUPERCOSTUME) && !isMenu(MENU_SUPER_REG)
        && !isMenu(MENU_LOADCOSTUME)&& !isMenu(MENU_SAVECOSTUME));
#else
    return 0;
#endif
}
static int entReceivePowerCust(Packet *pak, Entity *e, bool oo_packet)
{
    AppearanceType type;
    int current_powerCust;
    int num_costume_slots;
    type = pktGetIfSetBitsPack(pak, 2);
    switch (type)
    {
        case AT_ENTIRE_COSTUME:
            START_BIT_COUNT(pak, "FullCostume");
            if (pktGetBits(pak, 1))
            {
                //    recieving self
                current_powerCust = pktGetBits( pak, 8 );
                num_costume_slots = pktGetBits( pak, 8 );
                if (!oo_packet)
                {
                    e->pl->current_powerCust = current_powerCust;
                    e->pl->num_costume_slots = num_costume_slots;
                }

                if (pktGetBits(pak, 1))        //    multiple powerCust
                {
                    int i;
                    int numSlots = pktGetBits(pak, 4);
                    for( i = 0; (i < numSlots) && i < MAX_COSTUMES; i++ )
                    {
                        powerCustList_receive( pak, e, oo_packet ? dummy_ent_powerCust : e->pl->powerCustomizationLists[i] );
                    }
                }
                else                        //    just one
                {
                    powerCustList_receive(pak, e, oo_packet ? dummy_ent_powerCust : e->pl->powerCustomizationLists[e->pl->current_powerCust] );
                }
                if( (e != playerPtr() || menuCanSetCostumeAndPowerCust()) && !oo_packet)
                {
                    e->powerCust = e->pl->powerCustomizationLists[e->pl->current_powerCust];
                }
            }
            STOP_BIT_COUNT(pak);
            break;
        case AT_OWNED_COSTUME:
            START_BIT_COUNT(pak, "PCCCostume");            
            if (!oo_packet)
            {
                if (!e->ownedPowerCustomizationList)    e->ownedPowerCustomizationList = StructAllocRaw(sizeof(PowerCustomizationList));
                powerCustList_receive(pak, e, e->ownedPowerCustomizationList);
                if (e->ownedPowerCustomizationList && (e != playerPtr()))
                {
                    e->powerCust = e->ownedPowerCustomizationList;
                    costume_Apply(e);
                }
            }
            STOP_BIT_COUNT(pak);
            break;
    }
    return 1;
}
static int entReceiveCostume(Packet *pak, Entity *e, bool oo_packet)
{
    //TODO: Do any of these costume types care about out of order packets?
    //    if so, perhaps we need to split this into more virtual streams to keep track of out of order stuff
    // Need deferred out here because demorecords need to know about it.
    bool deferred = false;
    AppearanceType type;
    SeqInst    *seq = e->seq;

    type = pktGetIfSetBitsPack(pak, 2);
    switch(type)
    {
    case AT_ENTIRE_COSTUME:
        {
            bool bGotCostume=false;
            bool other_player;
            int previous_costume = e->pl->current_costume;
            int current_costume;
            int num_costume_slots = e->pl->num_costume_slots;
            //printf("%d: AT_ENTIRE_COSTUME: %s %s\n", pak->id, e->name, oo_packet?"OOP":"");
            START_BIT_COUNT(pak, "FullCostume");
             {
                if( pktGetBits( pak, 1 ) ) // are we recieving our own costume?
                {
                    ent_ReceiveCostumeRewards(pak, e); // don't care about OOP
                    deferred = pktGetBits( pak, 1 );
                    current_costume = pktGetBits( pak, 8 );
                    num_costume_slots = pktGetBits( pak, 8 );
                    if (!oo_packet)
                    {
                        e->pl->current_costume   = current_costume;
                        e->pl->num_costume_slots = num_costume_slots;
                    }
                }
                else
                {
                    deferred = pktGetBits( pak, 1 );
                }

                // The server has no concept of when the change actually happens.  So it'll comntinue to send the deferred flag
                // for the full five seconds.  We only care about the first run through here.  However doing things like toggling
                /// PPD hardsuit / halloween costumes / Granite armor etc. will send a costume change, but we want to ignore the
                // spurious deferred flag.
                // In reality, I should fix this on the server, but I have no clue how to clear the deferred flag once the costume change
                // emote has been sent to all clients.
                if (timerSecondsSince2000() <= e->pl->pendingCostumeIgnoreTime || oo_packet)
                {
                    deferred = false;
                }

                if( pktGetBits( pak, 1 ) ) // are we recieving multiple costumes?
                {
                    int i;
                    int recievingCount = pktGetBits( pak, 4 );

                    e->pl->num_costumes_stored = recievingCount;
                    for( i = 0; i < recievingCount; i++ )
                        bGotCostume |= costume_receive( pak, oo_packet?dummy_ent_costume:e->pl->costume[i] );
                    other_player = 0;
                }
                else
                {
                    if (deferred && e->pl->pendingCostume == NULL)
                    {
                        e->pl->pendingCostume = costume_create(GetBodyPartCount());
                        costume_init(e->pl->pendingCostume);
                    }

                    bGotCostume = costume_receive( pak, oo_packet ? dummy_ent_costume : deferred ? e->pl->pendingCostume : e->pl->costume[e->pl->current_costume] );
                    other_player = 1;
                }

                if( (e != playerPtr() || menuCanSetCostumeAndPowerCust()) && !oo_packet)
                {
                    if (deferred)
                    {
                        e->pl->pendingCostumeForceTime = timerSecondsSince2000() + CC_EMOTE_BASIC_TIMEOUT;
                        // Set the ignore timer to 15 seconds, that provides a 5 second grace period vs the 10 second timer.
                        // This will not be an issue in practice, since the client enforces a 30 second delaty, and the server
                        // enforces a 20 second delay.
                        e->pl->pendingCostumeIgnoreTime = timerSecondsSince2000() + CC_EMOTE_IGNORE_EXTRA_TIMEOUT;
                        if (other_player)
                        {
                            e->pl->pendingCostumeNumber = 1000;
                        }
                        else
                        {
                            e->pl->pendingCostume = e->pl->costume[e->pl->current_costume];
                            e->pl->pendingCostumeNumber = e->pl->current_costume + 1;
                            e->pl->current_costume = previous_costume;
                            e->pl->current_powerCust = previous_costume;
                        }
                    }
                    else
                    {
                        e->costume = costume_as_const( e->pl->costume[e->pl->current_costume] );
                    }
                }

            }
            STOP_BIT_COUNT(pak);

              if( pktGetBits( pak, 1 ) )
            {
                START_BIT_COUNT(pak, "SupergroupCostume");
                {
                    bool b;
                    int iMode, iHideEmblem;
                    
                    if (deferred && !shell_menu() && e->pl->supergroup_mode && 
                                    (e != playerPtr() || menuCanSetCostumeAndPowerCust()))
                    {
                        if (e->pl->pendingCostume == NULL || e->pl->pendingCostume == e->pl->costume[current_costume])
                        {
                            e->pl->pendingCostume = costume_create(GetBodyPartCount());
                            costume_init(e->pl->pendingCostume);
                        }
                        b = costume_receive( pak, e->pl->pendingCostume );
                        e->pl->pendingCostumeForceTime = timerSecondsSince2000() + CC_EMOTE_BASIC_TIMEOUT;
                        // Set the ignore timer to 15 seconds, that provides a 5 second grace period vs the 10 second timer.
                        // This will not be an issue in practice, since the client enforces a 30 second delaty, and the server
                        // enforces a 20 second delay.
                        e->pl->pendingCostumeIgnoreTime = timerSecondsSince2000() + CC_EMOTE_IGNORE_EXTRA_TIMEOUT;
                        e->pl->pendingCostumeNumber |= 256;
                    }
                    else
                    {
                        b = costume_receive( pak, oo_packet?dummy_ent_costume:e->pl->superCostume );
                    }
                    iMode = pktGetBits( pak, 1 );
                    iHideEmblem = pktGetBits( pak, 1 );
                    if(b && !oo_packet)
                    {
                        e->pl->supergroup_mode = iMode;
                        e->pl->hide_supergroup_emblem = iHideEmblem;
                        // DGNOTE -- I ought to combine these three nested if's by use of a few &&'s
                        if( !shell_menu() )
                        {
                            if( e->pl->supergroup_mode && (e != playerPtr() || menuCanSetCostumeAndPowerCust())  )
                            {
                                if (!deferred)
                                {
                                    e->costume = costume_as_const( e->pl->superCostume );
                                }
                            }
                        }
                    }
                }
                STOP_BIT_COUNT(pak);
            }

            if ( pktGetBits(pak, 1) )
            {
                START_BIT_COUNT(pak, "CostumeFxSpecial");
                {
                    char *newCostumeFx=pktGetString(pak);
                    if (!oo_packet && e->pl) {
                        Strncpyt(e->pl->costumeFxSpecial, newCostumeFx);
                        fxCleanFileName(e->pl->costumeFxSpecial, e->pl->costumeFxSpecial);
                        // Check for "none" values
                        if (stricmp(e->pl->costumeFxSpecial, "0")==0 ||
                            stricmp(e->pl->costumeFxSpecial, "NONE")==0 ||
                            stricmp(e->pl->costumeFxSpecial, "OFF")==0)
                        {
                            e->pl->costumeFxSpecial[0]='\0';
                        }
                        bGotCostume = true;
                    }
                }
                STOP_BIT_COUNT(pak);
            } else {
                if (!oo_packet && e->pl && e->pl->costumeFxSpecial[0]) {
                    e->pl->costumeFxSpecial[0] = '\0';
                    bGotCostume = true;
                }
            }

            if(!oo_packet && bGotCostume && (e != playerPtr() || !isMenu(MENU_SUPERCOSTUME )) )
            {
                extern int g_costume_apply_hack; // In case you're in the shell when you get a costume update
                g_costume_apply_hack = 1;
                costume_Apply(e);
                g_costume_apply_hack = 0;
            }

            if(!pktGetBits(pak,1)) // are we falling through to AT_NPC_COSTUME?
            {
                e->npcIndex = 0;
                break;
            }
        }
        // Inentional fall-through
    case AT_NPC_COSTUME:
        {
            int npcIndex, costumeIndex;
            const char *s;
            //printf("%d: AT_NPC_COSTUME: %s %s\n", pak->id, e->name, oo_packet?"OOP":"");

            START_BIT_COUNT(pak, "NpcCostume");

            npcIndex = pktGetBitsPack(pak, 12);
            costumeIndex = pktGetBitsPack(pak, 1);
            s = pktGetIfSetString(pak);
            if (pktGetBitsPack(pak, 1))
            {
                if (e->customNPC_costume)
                {
                    StructDestroy(ParseCustomNPCCostume, e->customNPC_costume);
                }
                e->customNPC_costume = StructAllocRaw(sizeof(CustomNPCCostume));
                NPCcostume_receive(pak, e->customNPC_costume);
            }
            if (!oo_packet)
            {
                if(playerPtr()==e && (isMenu(MENU_TAILOR) || isMenu(MENU_GENDER) || isMenu(MENU_BODY)) && npcIndex)
                {
                    tailor_exit(0);
                }

                if(s && *s)
                {
                    e->costumeWorldGroup = strdup(s);
                }
                if (e->pl && e->pl->pendingCostumeNumber != 0)
                {
                    // DGNOTE 4/24/2009
                    // Receiving an NPC costume request while there's a real costume change in escrow does so much bad stuff
                    // it's impossible to describe.  With other bugfixes, this should happen so infrequently that we can probably
                    // just toss the NPC costume request.
                    // If this *does* ever get called, try the following for a fix.  Apply the NPC costume change here, by taking the
                    // code in the "else" section of this if / else block, and moving it outside, so it is always executed.  Then do all
                    // the changes that happen when the pending costume is applied here.  Look for another DGNOTE tag in entclient.c with
                    // the same date as this one.  Replicate most everything in there except the actual changes to e->costume.
                }
                else
                {
                    e->npcIndex = npcIndex;
                    e->costumeIndex = costumeIndex;
                    if( e != playerPtr() || (menuCanSetCostumeAndPowerCust()) )
                    {
                        e->costume = npcDefsGetCostume(e->npcIndex, e->costumeIndex);
                    }
                }

                #ifdef TEST_CLIENT // We know nothing about npcs or their costumes, even though we look like one
                    if (!e->costume && e->pl && e->pl->costume[e->pl->current_costume]) {
                        entUseCopyOfCostume( e, e->pl->costume[e->pl->current_costume] );
                    }
                #endif
                if (e->customNPC_costume)
                {
                    Costume* mutable_costume = entUseCopyOfCurrentCostumeAndOwn(e);
                    costumeApplyNPCColorsToCostume(mutable_costume, e->customNPC_costume);
                }
                if(e->costume && ( e != playerPtr() || menuCanSetCostumeAndPowerCust() ) )
                {
                    costume_Apply(e);
                }
            }

            STOP_BIT_COUNT(pak);
        }
        break;
    case AT_SEQ_NAME:
        {
            const char *s;
            //printf("%d: AT_SEQ_NAME: %s %s\n", pak->id, e->name, oo_packet?"OOP":"");

            s = pktGetString(pak);
            if( !oo_packet && ENTTYPE(e) == ENTTYPE_MISSION_ITEM )
            {
                entSetSeq(e, seqLoadInst( (char *)s, 0, SEQ_LOAD_FULL, e->randSeed, e  ));
            }

            s = pktGetIfSetString( pak );
            if( !oo_packet && s && *s )
                e->costumeWorldGroup = strdup(s);
        }
        break;
    case AT_OWNED_COSTUME:
        {
            //printf("%d: AT_NPC_COSTUME: %s %s\n", pak->id, e->name, oo_packet?"OOP":"");

            START_BIT_COUNT(pak, "PCCCostume");            
            if (!oo_packet)
            {
                // @todo SHAREDMEM NEEDS_REVIEW we can handle this better
                e->ownedCostume = costume_create(GetBodyPartCount());
                costume_receive(pak, e->ownedCostume);    
                if (e->ownedCostume && (e != playerPtr()))
                {
                    e->costume = costume_as_const(e->ownedCostume);
                    costume_Apply(e);
                }
                else
                {
                    TODO(); // @todo, do we ever hit this case and just leave ownedCostume hanging?
                }
            }
            STOP_BIT_COUNT(pak);
        }
        break;
    case AT_NO_APPEARANCE:
    default:
        //printf("%d: AT_NO_APPEARANCE: %s %s\n", pak->id, e->name, oo_packet?"OOP":"");
        if(ent_create)
        {
            if(ENTTYPE(e) != ENTTYPE_PLAYER)
                log_printf( "syslogerrs" ,"entReceive: created player ent w/o getcostume\n");
            else
                log_printf( "syslogerrs" ,"entReceive: created villain ent w/o assignVisuals\n");
        }
        return 0;
        break;
    }


    if(demo_recording)
    {
        // If this is a deferred change (i.e. for a costume change emote) we don't bother to record the change.
        // In the case of a deferred change, at this stage the new costume hasn't been applied, it's effectively in escrow.
        // That actual application happens later in entclient.c, towards the bottom of int runSequencerMoveUpdate(...)
        if (!deferred)
        {
            demoRecordAppearance(e);
        }
        if(seq != e->seq)
        {
            demoRecordMove(e,e->next_move_idx,e->next_move_bits);
        }
    }

#ifndef TEST_CLIENT
    if(!e->seq)
        log_printf( "syslogerrs" ,"entReceive: No seq after entReceive!\n");
#endif

    return 1;
}

static int entReceiveTargetUpdate(Packet *pak, Entity *e, bool oo_packet, int bIsSelf)
{
    if(pktGetBits(pak, 1))
    {
        int has_target = pktGetBits(pak, 1);
        int iIdxTarget = has_target?pktGetBitsPack(pak, PKT_BITS_TO_REP_SVR_IDX):0;
        int has_assist = pktGetBits(pak, 1);
        int iIdxAssist = has_assist?pktGetBitsPack(pak, PKT_BITS_TO_REP_SVR_IDX):0;
        int has_taunter = pktGetBits(pak, 1);
        int iIdxTaunter = has_taunter?pktGetBitsPack(pak, PKT_BITS_TO_REP_SVR_IDX):0;

        Entity *eTarget = validEntFromId(iIdxTarget);
        Entity *eAssist = validEntFromId(iIdxAssist);
        Entity *eTaunter = validEntFromId(iIdxTaunter);

        int clear_target = pktGetBits(pak,1) && bIsSelf;

        int iNumPlacates = pktGetBitsPack(pak,2);
        int iPlacater;
        bool bGoodPacket = (e!=NULL && e->pchar!=NULL && !oo_packet);

        if (bIsSelf && bGoodPacket && g_pperPlacators)
        {
            int i;
            for (i = eaSize(&g_pperPlacators)-1; i >= 0; i--)
            {
                free(g_pperPlacators[i]);
            }
            eaDestroy(&g_pperPlacators);
        }

        for (iPlacater = 0; iPlacater < iNumPlacates; iPlacater++)
        {
            int iIdxPlacater = pktGetBitsPack(pak,PKT_BITS_TO_REP_SVR_IDX);
            if (bIsSelf && bGoodPacket && iIdxPlacater)
            {
                EntityRef *per = calloc(1,sizeof(EntityRef));
                *per = erGetRef(validEntFromId(iIdxPlacater));
                eaPush(&g_pperPlacators,per);
            }
        }

        if(bGoodPacket)
        {
            // Safe to call erGetRef when eTarget is NULL.
            e->pchar->erTargetInstantaneous = erGetRef(eTarget);
            e->pchar->erAssist = erGetRef(eAssist);
            if (bIsSelf) g_erTaunter = erGetRef(eTaunter);
        }
        if (clear_target)
        {
            current_target = NULL;
            gSelectedDBID = 0;
            gSelectedHandle[0] = 0;
            e->confuseRand = rand(); // Actual time wasn't random enough
        }
        return 1;
    }
    return 0;
}

static int entReceiveCharacterStats(Packet *pak, Entity *e, int oo_packet)
{
    // Receive basic entity stat updates here.

    Character* pchar = e->pchar;
    int hasNewStats;

    if (!pktGetBits(pak, 1)) {
        // This is the general case, which means:
        //  we have a pchar (don't really care here)
        //  we don't have a sidekick (gotta clear it if it's there)
        //  our stats haven't changed (do nothing)
        return 1;
    }

    hasNewStats = pktGetBits(pak, 1);

    if(hasNewStats)
    {
        // Stats
        START_BIT_COUNT(pak, "basic stats");
        sdUnpackDiff(SendBasicCharacter, pak, e->pchar, &e->pkt_id_basicAttrDef, true);
        STOP_BIT_COUNT(pak);

        if(pchar)
        {
            // JE: Removed oo_packet check because it could cause this code to never get
            //     ran (i.e. when the XP was received in order, yet this packet was out of
            //   order, since stats oop checking is done on a per-channel basis)
            character_LevelFinalize(e->pchar, false); // Must be after getting XP
        }
    }
    return hasNewStats;
}

static int entReceiveStateMode(Packet *pak, Entity *e, int oo_packet)
{
    int newmode;
    if( pktGetBits( pak, 1 ) )
    {
        newmode = pktGetIfSetBitsPack( pak, TOTAL_ENT_MODES );
        if (!oo_packet) {
            e->state.mode = newmode;
        }
        return 1;
    }
    return 0;
}

static int entReceiveBuffs(Packet *pak, Entity *e, bool oo_packet)
{
    if(pktGetBits(pak, 1))
    {
        character_ReceiveBuffs(pak, e, oo_packet);
        return 1;
    }
    return 0;
}

static int entReceiveStatusEffects(Packet *pak, Entity *e, bool oo_packet)
{
    if(pktGetBits(pak, 1))
    {
        character_ReceiveStatusEffects(pak, e->pchar, oo_packet);
        return 1;
    }
    return 0;
}

static int entReceiveSendOnOddSend(Packet *pak, Entity *e, bool oo_packet)
{
    int send_on_odd_send = pktGetBits(pak, 1);

    if (!oo_packet) {
        SET_ENTINFO(e).send_on_odd_send = send_on_odd_send;
    }

    return 1;
}

static void entRecieveTaskforceArchitectInfo(Packet *pak, Entity *e)
{
    if( !pktGetBits(pak,1) ) // are we getting an update?
    {
        e->onArchitect = 0;
        return;
    }

    e->onArchitect = pktGetBits(pak, 1);
    if(e->onArchitect) 
    {
        Costume * costume = 0;
        char *contact_name;
        int architect_flags, architect_id, contact;
        devassert(e->taskforce);

        architect_flags = pktGetBitsAuto(pak);
        architect_id = pktGetBitsAuto(pak);
        contact = pktGetBitsAuto(pak);
        contact_name = pktGetString(pak);

        if (e->taskforce)
        {
            e->taskforce->architect_id = architect_id;
            e->taskforce->architect_flags = architect_flags;

            if (contact != e->taskforce->architect_contact_override)
            {
                e->taskforce->architect_useCachedCostume = 0;
                e->taskforce->architect_contact_override = contact;
            }
            if (stricmp(e->taskforce->architect_contact_name, contact_name) != 0)
            {
                e->taskforce->architect_useCachedCostume = 0;
                strcpy(e->taskforce->architect_contact_name, contact_name);
            }
        }

        if ( pktGetBits(pak,1) )
        {
            if( pktGetBits(pak, 1) )
            {
                costume = costume_create(GetBodyPartCount());
                costume_init(costume);
                costume_receive( pak, costume );
            }
            if (e->taskforce)
            {
                if (costume)
                {
                    if ( e->taskforce->architect_contact_costume ) 
                    {
                        costume_destroy(e->taskforce->architect_contact_costume);
                    }
                    e->taskforce->architect_contact_costume = costume;        
                    e->taskforce->architect_useCachedCostume = 0;
                }
            }

        }
        else
        {
            if ( e->taskforce->architect_contact_costume )
            {
                costume_destroy(e->taskforce->architect_contact_costume);
            }
            e->taskforce->architect_contact_costume = NULL;
        }
        if( pktGetBits(pak,1) )
        {
            CustomNPCCostume *costume = NULL;
            if ( pktGetBits(pak, 1) )
            {
                costume = StructAllocRaw(sizeof(CustomNPCCostume));
                NPCcostume_receive(pak, costume);
            }
            if (e->taskforce)
            {
                if (costume)
                {
                    if (e->taskforce->architect_contact_npc_costume)
                    {
                        StructDestroy(ParseCustomNPCCostume, e->taskforce->architect_contact_npc_costume);
                    }
                    e->taskforce->architect_contact_npc_costume = costume;
                    e->taskforce->architect_useCachedCostume = 0;
                }
            }
        }
        else
        {
            if ( e->taskforce->architect_contact_npc_costume )
            {
                StructDestroy(ParseCustomNPCCostume, e->taskforce->architect_contact_npc_costume);
            }
            e->taskforce->architect_contact_npc_costume = NULL;
        }
        if( pktGetBits(pak,1) )
        {
            char *pchName = strdup(pktGetString(pak));
            char *pchAuthor = strdup(pktGetString(pak));
            char *pchDescription = strdup(pktGetString(pak));
            int rating = pktGetBitsAuto(pak);
            int euro = pktGetBitsAuto(pak);

            e->architect_invincible = pktGetBitsAuto(pak);
            e->architect_untargetable = pktGetBitsAuto(pak);

            if (e->taskforce)
                missionReview_Set(e->taskforce->architect_id, e->taskforce->architect_flags, pchName, pchAuthor, pchDescription, rating, euro );
        
            SAFE_FREE(pchName);
            SAFE_FREE(pchAuthor);
            SAFE_FREE(pchDescription);
        }
    }
}

static int entReceiveTaskforceParameters(Packet *pak, Entity *e)
{
    U32 paramsSet;
    U32 count;
    U32 param;

     e->onFlashback = pktGetBitsPack(pak, 1);

    if(pktGetBits(pak, 1))
    {
        devassert(e->taskforce);

        paramsSet = pktGetBitsAuto(pak);

        if (e->taskforce)
        {
            e->taskforce->parametersSet = paramsSet;
        }

        for (count = 0; count < MAX_TASKFORCE_PARAMETERS; count++)
        {
            if (paramsSet & (1 << count))
            {
                param = pktGetBitsAuto(pak);

                if (e->taskforce)
                {
                    e->taskforce->parameters[count] = param;
                }
            }
        }

        // The server stores times in its own timebase. To send them to
        // us it subtracts its current time. We can then add back our
        // current time to get the right value, thereby compensating for
        // whatever different timezone or clock difference is between us.
        if (TaskForceIsParameterEnabled(e, TFPARAM_TIME_LIMIT))
        {
            e->taskforce->parameters[TFPARAM_TIME_LIMIT - 1] +=
                timerSecondsSince2000();
        }

        if (TaskForceIsParameterEnabled(e, TFPARAM_START_TIME))
        {
            e->taskforce->parameters[TFPARAM_START_TIME - 1] +=
                timerSecondsSince2000();
        }
    }


    return 1;
}

static int entReceiveAllyID(Packet *pak, Entity *e, bool oo_packet)
{
    int iAllyID = pktGetBitsPack(pak, 2);
    int iGangID = pktGetBitsPack(pak, 4);

    if (!oo_packet && e && e->pchar) {
        e->pchar->iAllyID = iAllyID;
        e->pchar->iGangID = iGangID;

//        calcSeqTypeName( e->seq, e->pchar ? e->pchar->iAllyID : 0, ENTTYPE(e)==ENTTYPE_PLAYER ? 1 : 0 );
    }

    return 1;
}



static int entReceiveLevelingpactInfo(Packet *pak, Entity *e, bool oo_packet)
{
    int levelingpactId;
    int memberCount;
    int i;

    if(pktGetBits(pak, 1))
    {
        levelingpactId = pktGetBitsAuto(pak);
        if (!oo_packet && e && e->pchar) {
            e->levelingpact_id = levelingpactId;
        }

        if(levelingpactId == 0)
        {
            if(e->levelingpact)
            {
                levelingpactListUiUpdate();
                destroyLevelingpact(e->levelingpact);
                e->levelingpact = 0;
            }
            return 1;
        }


        // if we're in a leveling pact
        if ( e && !e->levelingpact )
        {
            levelingpactListUiUpdate();
            e->levelingpact = createLevelingpact();
            e->levelingpact->members.ids            = calloc( MAX_LEVELINGPACT_MEMBERS, sizeof(int) );
            e->levelingpact->members.onMapserver    = calloc( MAX_LEVELINGPACT_MEMBERS, sizeof(int) );
            e->levelingpact->members.names            = calloc( MAX_LEVELINGPACT_MEMBERS, sizeof(MemberName) );
            e->levelingpact->members.mapIds            = calloc( MAX_LEVELINGPACT_MEMBERS, sizeof(int) );
        }


        memberCount = pktGetBitsAuto(pak);
        if (!oo_packet && e && e->pchar) 
        {
            e->levelingpact->members.count  = memberCount;
            e->levelingpact->count = e->levelingpact->members.count;


            if (e->levelingpact->members.count > MAX_LEVELINGPACT_MEMBERS)
            {
                assert(!"You are on a team with more than max members, something has gone wrong, or you cheat.");
            }

            for( i = 0; i < e->levelingpact->members.count; i++ )
            {
                e->levelingpact->members.ids[i] = pktGetBitsAuto(pak);
                e->levelingpact->members.onMapserver[i] = FALSE;

                if( pktGetBits(pak, 1) )
                {
                    strcpy( e->levelingpact->members.names[i], pktGetString( pak ));
                }
                else
                {
                    Entity *pactMember = entFromDbId(e->levelingpact->members.ids[i]) ;
                    if(pactMember)
                    {
                        strcpy( e->levelingpact->members.names[i], pactMember->name);
                    }
                    e->levelingpact->members.onMapserver[i] = TRUE;
                }
            }
        }

        return 1;
    }
    return 0;
}

static int entReceivePvP(Packet *pak, Entity *e, bool oo_packet)
{
    bool bPvPSwitch = pktGetBits(pak, 1);
    bool bPvPActive = pktGetBits(pak, 1);
    int iPvPTimer = pktGetBitsPack(pak, 5);
    bool bIsGanker = pktGetBits(pak, 1);

    if(!oo_packet && e && e->pl)
        e->pl->pvpSwitch = bPvPSwitch;
    

    if(!oo_packet && e && e->pchar)
        e->pchar->bPvPActive = bPvPActive;
    

    if(!oo_packet && e)
    {
        e->pvpClientZoneTimer = iPvPTimer;
        e->pvpUpdateTime = timerCpuTicks();
    }
    

    if(!oo_packet && e && e->pl)
        e->pl->bIsGanker = bIsGanker;
    
    return 1;
}

static int entReceiveEntCollision(Packet *pak, Entity *e, bool oo_packet)
{
    bool bNoEntCollision = pktGetBits(pak, 1);

    // If this is the playerPtr, then no_ent_collision comes from
    // the control state instead.

    if(!oo_packet && e!=playerPtr())
    {
        e->motion->input.no_ent_collision = bNoEntCollision;
    }
    return 1;
}

static int entReceiveEntSelectable(Packet *pak, Entity *e, bool oo_packet)
{
    bool bNoSelectable = pktGetBits(pak, 1);

    // Only use this if it's not an OoO packet and the entity isn't a player.
    if(!oo_packet && e->pl == NULL)
    {
        e->notSelectable = bNoSelectable;
    }
    else
    {
        e->notSelectable = 0;
    }
    return 1;
}

static int entReceiveNoDrawOnClient(Packet *pak, Entity *e, bool oo_packet)
{
    bool bNoDraw = pktGetBits(pak, 1);

    // If this is the playerPtr, then bNoDraw comes from
    // the control state instead.
    // Q: Is that true?

    if(!oo_packet && e!=playerPtr())
    {
        e->noDrawOnClient = bNoDraw;
    }
    return 1;
}

static int entReceiveShowOnMap(Packet *pak, Entity *e, bool oo_packet)
{
    bool bShowOnMap = pktGetBits(pak, 1);

    if(!oo_packet && e!=playerPtr())
    {
        e->showOnMap = bShowOnMap;
    }
    return 1;
}

static int entReceiveContactOrPnpc(Packet *pak, Entity *e, bool oo_packet)
{
    bool bContactOrPnpc = pktGetBits(pak, 1);
    bool bContactArchitect = pktGetBits(pak, 1);

    if(!oo_packet && e!=playerPtr())
    {
        e->contactOrPnpc = bContactOrPnpc;
        e->contactArchitect = bContactArchitect;
    }
    return 1;
}

static int entReceiveNameUpdate(Packet *pak, Entity *e, bool oo_packet)
{
    const char *name;

    name=pktGetIfSetString(pak);

    if(!oo_packet && e!=playerPtr() && name && name[0] && e->name)
    {
        strcpy(e->name, name);
    }
    return 1;
}

static int entReceivePetName(Packet *pak, Entity *e, bool oo_packet)
{

    const char *petName;

    petName=pktGetIfSetString(pak);
    if( petName && petName[0] ) {
        if (!e->petName || strlen(petName) > strlen(e->petName)) {
            SAFE_FREE(e->petName);
            e->petName = strdup( petName );
        } else {
            strcpy(e->petName,petName);
        }
    }
    return 1;
}

static int entReceivePetInfo(Packet *pak, Entity *e, bool oo_packet)
{
    if(pktGetBits(pak, 1))
    {
         const BasePower *ppowCreatedMe = NULL;
        int    specialPowerState=0;
        bool bppowCreated=0;
        U32    bCommandablePet=0,bVisiblePet=0,bDismissablePet=0;
        char specialPowerDisplayName[1000] = "";

        Entity *pOwner = entFromId(pktGetBitsPack(pak, PKT_BITS_TO_REP_SVR_IDX));

        if(pOwner!=NULL && !oo_packet)
        {
            e->erOwner = erGetRef(pOwner);
        }

        pOwner = entFromId(pktGetBitsPack(pak, PKT_BITS_TO_REP_SVR_IDX));
        if(pOwner!=NULL && !oo_packet)
        {
            e->erCreator = erGetRef(pOwner);
        }

        bppowCreated = pktGetBits(pak, 1);
        if(bppowCreated)
        {
            ppowCreatedMe = basepower_Receive(pak, &g_PowerDictionary, e);
        }

        bCommandablePet = pktGetBits( pak, 1 );
        bVisiblePet = pktGetBits( pak, 1 );
        bDismissablePet = pktGetBits( pak, 1 );

        //TO DO move all pet related info to a single struct in the entity
        strcpy(specialPowerDisplayName, pktGetIfSetString(pak));
        specialPowerState = pktGetBitsPack( pak, 1 );
        entReceivePetName(pak, e, oo_packet);

        if(!oo_packet)
        {
            if(bppowCreated)
                e->pchar->ppowCreatedMe = ppowCreatedMe;
            e->commandablePet = bCommandablePet;
            e->petVisibility = bVisiblePet;
            e->petDismissable = bDismissablePet;
            if(specialPowerDisplayName[0])
            {
                if(e->specialPowerDisplayName && strlen(e->specialPowerDisplayName) < strlen(specialPowerDisplayName))
                {
                    SAFE_FREE(e->specialPowerDisplayName);
                    e->specialPowerDisplayName = strdup(specialPowerDisplayName);
                }
                else
                {
                    strcpy(e->specialPowerDisplayName, specialPowerDisplayName);
                }
            }
            else
            {
                SAFE_FREE(e->specialPowerDisplayName);
            }
            e->specialPowerState = specialPowerState;
        }
    }
    else
    {
        // Server says this isn't a pet.  But it might have been before, so
        //  clean out the relevant info
        e->erOwner = 0;
        e->erCreator = 0;
        e->commandablePet = 0;
        e->petVisibility = 0;
        e->petDismissable = 0;
        SAFE_FREE(e->petName);
        SAFE_FREE(e->specialPowerDisplayName);
        e->specialPowerState = 0;
    }
    return 1;
}

static int entReceiveAFK(Packet *pak, Entity *e, bool oo_packet)
{
    bool bAFK = pktGetBits(pak, 1);
    if (bAFK) {
        bool bMsg = pktGetBits(pak, 1);
        char *pch = NULL;

        if(bMsg)
        {
            pch = pktGetString(pak);
        }

        if(!oo_packet)
        {
            if(pch)
            {
                strncpy(e->pl->afk_string, pch, MAX_AFK_STRING);
            }
        }
    }
    if (!oo_packet && e->pl) {
        e->pl->afk = bAFK;
    }

    return 1;
}

static int entReceiveOtherSupergroupInfo(Packet *pak, Entity *e, bool oo_packet)
{
    if(pktGetBits(pak, 1))
    {
        memset(e->supergroup_name, 0, sizeof(e->supergroup_name));
        memset(e->supergroup_emblem, 0, sizeof(e->supergroup_emblem));

        if((e->supergroup_id=pktGetBitsPack(pak, 2))!=0)
        {
            const char *pch = NULL;
            U32    color[2];

            pch = pktGetString(pak);
            if(!oo_packet)
            {

                strncpy(e->supergroup_name, pch, sizeof(e->supergroup_name)-1);
            }

            pch = pktGetString(pak);
            if(!oo_packet)
            {
                if ( stashFindElementConst( costume_HashTable, pch , NULL) )
                    strncpy(e->supergroup_emblem, pch, sizeof(e->supergroup_emblem)-1);
                else
                    strcpy( e->supergroup_emblem, "" );
            }

            color[0] = pktGetBits(pak, 32);
            color[1] = pktGetBits(pak, 32);

            if(!oo_packet)
            {
                e->supergroup_color[0] = color[0];
                e->supergroup_color[1] = color[1];
            }

            if (!e->supergroup)
                e->supergroup = createSupergroup();

            e->supergroup->spacesForItemsOfPower = pktGetBitsPack(pak, 2);

            // ----------
            // receive the supergroup active task
            if (pktGetBits(pak, 1))
            {
                int selected = 0;
                if (!e->supergroup->sgMission)
                {
                    e->supergroup->sgMission = TaskStatusCreate();
                    selected = 1; // We just got the task, so select it
                }
                TaskStatusReceive(e->supergroup->sgMission, -1, 0, pak);
                if (selected)
                    compass_SetTaskDestination(&waypointDest, e->supergroup->sgMission);
            }
            else
            {
                if (e->supergroup && e->supergroup->sgMission)
                {
                    TaskStatusDestroy(e->supergroup->sgMission);
                    e->supergroup->sgMission = NULL;
                    compass_ClearAllMatchingId(SG_LOCATION);
                }
            }

            // ----------
            // receive the tokens

            {
                int i;
                int n = pktGetBitsAuto(pak);

                for( i = 0; i < n; ++i )
                {
                    char *tokenName = pktGetString(pak);
                    int val = pktGetBitsPack(pak, 1);
                    int time = pktGetBitsPack(pak, 1);
                    int idx = rewardtoken_IdxFromName( &e->supergroup->rewardTokens, tokenName );

                    if( idx < 0 )
                    {
                        idx = rewardtoken_Award(&e->supergroup->rewardTokens, tokenName, e->db_id );
                    }

                    if (EAINRANGE(idx, e->supergroup->rewardTokens))
                    {
                        e->supergroup->rewardTokens[idx]->val = val;
                        e->supergroup->rewardTokens[idx]->timer = time;
                    }
                }

                // Get the supergroups recipes.
                eaClearEx(&e->supergroup->invRecipes, DestroyRecipeInvItem);
                n = pktGetBitsAuto(pak);
                for( i = 0; i < n; ++i )
                {
                    int count = -1;
                    char *pchName;
                    const DetailRecipe *pRecipe = NULL;

                    if(pktGetBitsPack(pak,1))
                    {
                        // Unlimited
                        count = -1;
                        pchName = pktGetString(pak);
                    }
                    else
                    {
                        count = pktGetBitsPack(pak, 3);
                        pchName = pktGetString(pak);
                    }

                    pRecipe = detailrecipedict_RecipeFromName(pchName);
                    if(pRecipe && count)
                    {
                        sgrp_SetRecipeNoLock(e->supergroup, pRecipe, count, count<0 ? true : false);
                    }
                }

                // get special details
                eaDestroy(&e->supergroup->specialDetails);
                n = pktGetBitsAuto(pak);
                for (i = 0; i < n; i++)
                {
                    char *pName = pktGetString(pak);
                    SpecialDetail *pNew = (SpecialDetail *) malloc(sizeof(SpecialDetail));

                    pNew->creation_time = pktGetBitsAuto(pak);
                    pNew->expiration_time = pktGetBitsAuto(pak);
                    pNew->iFlags = pktGetBitsAuto(pak);

                    pNew->pDetail = basedata_GetDetailByName(pName);

                    if (!pNew->pDetail)
                    {
                        SAFE_FREE(pNew);
                    } else {
                        pNew->bValid = true;
                        eaPush(&e->supergroup->specialDetails, pNew);
                    }
                }
#ifndef TEST_CLIENT
                if (playerPtr() != NULL && e != NULL && e->db_id == playerPtr()->db_id)
                    recipeInventoryReparse();
#endif
            }
        }
        return 1;
    }


    return 0;
}

static void entReceiveAiAnimList(Packet *pak, Entity *e)
{
    if( pktGetBits(pak,1) )
    {
        const char * newAnimList = pktGetIfSetString( pak );
        alClientAssignAnimList( e, newAnimList );
    }
}


float test;
static int entReceive(Packet *pak,int idx,int odd_send)
{
    #define UPDATE_PACKET_ID(x) {if(pak->id > x) x = pak->id;}
    #define IS_OOP(x) (pak->id < x)

    int isSelf = 0;
    int rare_bits;
    int medium_rare_bits;
    int very_rare_bits;
    int pchar_things;
    Entity  *e;
    int bitpos = bsGetCursorBitPosition(&pak->stream);

    if(demo_recording)
    {
        demoSetEntIdx(idx);
    }

    START_BIT_COUNT(pak, "checkEntCreate");
        e = checkEntCreate(pak,idx,&ent_create);
    STOP_BIT_COUNT(pak);

    if (!e) // end of ent list
        return 0;

    e->svr_idx = idx;

    PERFINFO_RUN(
        if(ent_create)
        {
            if(odd_send)
            {
                START_BIT_COUNT(pak, "create:ODD");
            }
            else
            {
                START_BIT_COUNT(pak, "create:EVEN");
            }
        }
        else if(ENTINFO(e).send_on_odd_send)
        {
            START_BIT_COUNT(pak, "odd send");
        }
        else
        {
            START_BIT_COUNT(pak, "always send");
        }
    );

    switch(ENTTYPE(e))
    {
        #define CASE(x) case ENTTYPE_##x: START_BIT_COUNT(pak, #x); break

        case ENTTYPE_PLAYER:
            if(e == playerPtr())
            {
                isSelf = 1;
                START_BIT_COUNT(pak, "ME");
            }
            else
            {
                START_BIT_COUNT(pak, "PLAYER");
            }
            break;
        CASE(CRITTER);
        CASE(NPC);
        CASE(CAR);
        CASE(DOOR);
        default:
            if(e == &dummy_ent)
            {
                START_BIT_COUNT(pak, "DUMMY");
            }
            else
            {
                START_BIT_COUNT(pak, "OTHER");
            }
            break;
    }

    START_BIT_COUNT(pak, "rare_bits");
        rare_bits = pktGetBits(pak, 1);
        medium_rare_bits = rare_bits ? pktGetBits(pak, 1) : 0;
        very_rare_bits = rare_bits ? pktGetBits(pak, 1) : 0;
    STOP_BIT_COUNT(pak);

    if(medium_rare_bits)
    {
        START_BIT_COUNT(pak, "entReceiveStateMode");
            if(entReceiveStateMode(pak, e, IS_OOP(e->pkt_id_state_mode)))
                UPDATE_PACKET_ID(e->pkt_id_state_mode);
        STOP_BIT_COUNT(pak);
    }

    // Receive the position and qrot.

    START_BIT_COUNT(pak, "entReceivePosUpdate");
        entReceivePosUpdate(pak,e,ent_create,odd_send);
    STOP_BIT_COUNT(pak);

    if(rare_bits)
    {
        START_BIT_COUNT(pak, "entReceiveSeqMoveUpdate");
            if (entReceiveSeqMoveUpdate(pak, e, IS_OOP(e->pkt_id_seq_move)))
            {
                UPDATE_PACKET_ID(e->pkt_id_seq_move);

                // If the entity is being created, start the animation right away.

                if(ent_create)
                    e->move_change_timer = 0;
            }
        STOP_BIT_COUNT(pak);
    }

    if(medium_rare_bits)
    {
        START_BIT_COUNT(pak, "entReceiveSeqTriggeredMoves");
            if (entReceiveSeqTriggeredMoves(pak, e, IS_OOP(e->pkt_id_seq_triggermove)))
                UPDATE_PACKET_ID(e->pkt_id_seq_triggermove);
        STOP_BIT_COUNT(pak);

        START_BIT_COUNT(pak, "entReceiveAiAnimList");
            //TO DO does this need to manage OOP? It could screw up, but it seems unlikely, and not drastic if it does
            //the worst would be briefly showing the wrong FX or playing the wrong animation
            entReceiveAiAnimList(pak, e);
        STOP_BIT_COUNT(pak);
    }

    START_BIT_COUNT(pak, "pchar_things");
        pchar_things = pktGetBits(pak, 1);
    STOP_BIT_COUNT(pak);

    if(pktGetBits(pak, 1) && pchar_things)
    {
        START_BIT_COUNT(pak, "entReceiveNetFx");
            // OoOP not handled?
            entReceiveNetFx(pak,e);
        STOP_BIT_COUNT(pak);
    }

    if(medium_rare_bits)
    {
        START_BIT_COUNT(pak, "entReceiveCostume");
            if (entReceiveCostume(pak, e, IS_OOP(e->pkt_id_costume)))
                UPDATE_PACKET_ID(e->pkt_id_costume);
        STOP_BIT_COUNT(pak);
        START_BIT_COUNT(pak, "entReceiveXlucency");
            if (entReceiveXlucency(pak, e, IS_OOP(e->pkt_id_xlucency)))
                UPDATE_PACKET_ID(e->pkt_id_xlucency);
        STOP_BIT_COUNT(pak);
    }

    if(very_rare_bits)
    {
        START_BIT_COUNT(pak, "entReceivePowerCust")
            if (entReceivePowerCust(pak, e, IS_OOP(e->pkt_id_powerCust)))
                UPDATE_PACKET_ID(e->pkt_id_powerCust);
        STOP_BIT_COUNT(pak);
        START_BIT_COUNT(pak, "entReceiveTitles");
            if (entReceiveTitles(pak, e, IS_OOP(e->pkt_id_titles)))
                UPDATE_PACKET_ID(e->pkt_id_titles);
        STOP_BIT_COUNT(pak);
    }

    // Note that the ragdoll update must happen after the costume is received
    // since we need the sequencer to exist.
    START_BIT_COUNT(pak, "entGetRagdoll");
#ifdef RAGDOLL
    if ( e->seq ) // we might be an out of order packet, just throw away ragdoll info in case
        entGetRagdoll(pak,e);
    else
        entGetRagdollIgnore(pak,e);
#else
    entGetRagdollIgnore(pak, e);
#endif
    STOP_BIT_COUNT(pak);


#ifndef TEST_CLIENT
    //Debug
    if( e != &dummy_ent && ent_create && !e->seq)
        assertmsg( 0, "Entity received and no sequencer created for it!");
    //end debug
#endif

    // Handle OoOP for CharacterStats

    if(pchar_things)
    {
        START_BIT_COUNT(pak, "entReceiveCharacterStats");
            if (entReceiveCharacterStats(pak, e, IS_OOP(e->pkt_id_character_stats)))
            {
                UPDATE_PACKET_ID(e->pkt_id_character_stats);

                if(demo_recording)
                {
                    demoRecordStatChanges(e);
                }
            }
        STOP_BIT_COUNT(pak);
    }

    e->bitfieldVisionPhases = pktGetBitsAuto(pak);
    e->exclusiveVisionPhase = pktGetBitsAuto(pak);

    e->shouldIConYellow = pktGetBitsAuto(pak);

    // Get Buff information if there is any.
    if(pchar_things)
    {
        //! @todo e->pchar can be NULL.  How does this occur?  Should we prevent that from occurring?
        if (e->pchar)
        {
            e->pchar->iCombatModShift = pktGetBitsAuto(pak);
        }
        else
        {
            pktGetBitsAuto(pak); // eat the combat mod shift...
        }

        START_BIT_COUNT(pak, "entReceiveTargetUpdate");
            if (entReceiveTargetUpdate(pak, e, IS_OOP(e->pkt_id_targetting), isSelf))
                UPDATE_PACKET_ID(e->pkt_id_targetting);
        STOP_BIT_COUNT(pak);

        START_BIT_COUNT(pak, "entReceiveStatusEffects");
            if (entReceiveStatusEffects(pak, e, IS_OOP(e->pkt_id_status_effects)))
                UPDATE_PACKET_ID(e->pkt_id_status_effects);
        STOP_BIT_COUNT(pak);

        if (isSelf)
        {
            START_BIT_COUNT(pak, "character_ReceiveInventorySizes");
            character_ReceiveInventorySizes(e->pchar, pak);
            STOP_BIT_COUNT(pak);
        }
    }

    if(very_rare_bits)
    {
        START_BIT_COUNT(pak, "rare things");
        {
            bool stream_oo_packet;
            if (isSelf)
            {    
                START_BIT_COUNT(pak, "entReceiveTaskforceParameters");
                    entRecieveTaskforceArchitectInfo(pak, e);
                    entReceiveTaskforceParameters(pak, e);
                STOP_BIT_COUNT(pak);
            }

            stream_oo_packet = IS_OOP(e->pkt_id_rare_bits);

            // These are just part of the main stream, no separate packet ids are kept

            START_BIT_COUNT(pak, "entReceiveSendOnOddSend");
                entReceiveSendOnOddSend(pak, e, stream_oo_packet);
            STOP_BIT_COUNT(pak);

            START_BIT_COUNT(pak, "entReceiveAllyID");
                entReceiveAllyID(pak, e, stream_oo_packet);
            STOP_BIT_COUNT(pak);

            START_BIT_COUNT(pak, "entReceiveIgnoreCombatMods");
            {
                U32 bIgnoreCombatMods;
                int ignoreCombatModLevelShift;

                bIgnoreCombatMods = pktGetBitsPack(pak, 1);
                // DGNOTE 8/2/2010 - Yes, it is my intent to offset this by 8.  Without this, -1 would take 32 bits to send.
                // Doing the offset thing allows anything down to and including -8 to go as a positive number, taking far fewer bits.
                // Look for a DGNOTE with the same date stamp in entsend.c for where this is packaged up.
                ignoreCombatModLevelShift = bIgnoreCombatMods ? pktGetBitsAuto(pak) - 8 : 0;
                if (!stream_oo_packet && e && e->pchar)
                {
                    e->pchar->bIgnoreCombatMods = bIgnoreCombatMods;
                    e->pchar->ignoreCombatModLevelShift = ignoreCombatModLevelShift;
                }
            }
            STOP_BIT_COUNT(pak);

            START_BIT_COUNT(pak, "PlayerType");
            {
                PlayerType playerType = pktGetBits( pak, 1 );
                PlayerSubType playerSubType = pktGetBits(pak, 2);
                PraetorianProgress praetorianProgress = pktGetBits(pak, 3);
                PlayerType playerTypeByLocation = pktGetBits(pak, 1);

                STATIC_INFUNC_ASSERT(kPlayerType_Count <= (1<<1))
                STATIC_INFUNC_ASSERT(kPlayerSubType_Count <= (1<<2))
                STATIC_INFUNC_ASSERT(kPraetorianProgress_Count <= (1<<3))
                STATIC_INFUNC_ASSERT(kPlayerType_Count <= (1<<1))

                if (e && e->pl && e->pchar && !stream_oo_packet) {
                    int oldType = e->pl->playerType;
                    int newSkin;
                    e->pl->playerType = playerType;
                    e->pl->playerSubType = playerSubType;
                    e->pl->praetorianProgress = praetorianProgress;
                    e->pchar->playerTypeByLocation = playerTypeByLocation;

                    newSkin = UISKIN_FROM_ENT(e);

                    if(isSelf && (oldType != e->pl->playerType || game_state.skin != newSkin))
                    {
                        game_state.skin = newSkin;

                        resetVillainHeroDependantStuff(0);
                        //change event channel
                        if (newSkin == UISKIN_HEROES)
                        {
                            setHeroVillainEventChannel(1);
                        }
                        else if (newSkin == UISKIN_VILLAINS)
                        {
                            setHeroVillainEventChannel(0);
                        }
                    }

                    calcSeqTypeName( e->seq, e->pl ? e->pl->playerType : 0, e->pl ? ENT_IS_IN_PRAETORIA(e) : 0, ENTTYPE(e)==ENTTYPE_PLAYER ? 1 : 0 );
                }
            }
            STOP_BIT_COUNT(pak);

            START_BIT_COUNT(pak, "entReceivePvP");
                entReceivePvP(pak, e, stream_oo_packet);
            STOP_BIT_COUNT(pak);

            START_BIT_COUNT(pak, "entReceiveEntCollision");
                entReceiveEntCollision(pak, e, stream_oo_packet);
            STOP_BIT_COUNT(pak);

            START_BIT_COUNT(pak, "entReceiveEntSelectable");
                entReceiveEntSelectable(pak, e, stream_oo_packet);
            STOP_BIT_COUNT(pak);

            START_BIT_COUNT(pak, "entReceiveNoDrawOnClient");
                entReceiveNoDrawOnClient(pak, e, stream_oo_packet);
            STOP_BIT_COUNT(pak);

            START_BIT_COUNT(pak, "entReceiveShowOnMap");
                entReceiveShowOnMap(pak, e, stream_oo_packet);
            STOP_BIT_COUNT(pak);

            START_BIT_COUNT(pak, "entReceiveContactOrPnpc");
            entReceiveContactOrPnpc(pak, e, stream_oo_packet);
            STOP_BIT_COUNT(pak);

            START_BIT_COUNT(pak, "entReceiveAlwaysCon");
            {
                U32 alwaysCon = pktGetBits(pak, 1);
                if (!stream_oo_packet)
                    e->alwaysCon = alwaysCon;
            }
            STOP_BIT_COUNT(pak);

            START_BIT_COUNT(pak, "entReceiveSeeThroughWalls");
            {
                U32 seeThroughWalls = pktGetBits(pak, 1);
                if (!stream_oo_packet)
                    e->seeThroughWalls = seeThroughWalls;
            }
            STOP_BIT_COUNT(pak);

            START_BIT_COUNT(pak, "entReceiveNameUpdate");
                entReceiveNameUpdate(pak, e, stream_oo_packet);
            STOP_BIT_COUNT(pak);

            START_BIT_COUNT(pak, "entReceivePetInfo");
            entReceivePetInfo(pak, e, stream_oo_packet);
            STOP_BIT_COUNT(pak);

            START_BIT_COUNT(pak, "entReceiveAFK");
                entReceiveAFK(pak, e, stream_oo_packet);
            STOP_BIT_COUNT(pak);

            START_BIT_COUNT(pak, "entReceiveOtherSupergroupInfo");
                if(entReceiveOtherSupergroupInfo(pak, e, IS_OOP(e->pkt_id_supergroup)))
                    UPDATE_PACKET_ID(e->pkt_id_supergroup);
            STOP_BIT_COUNT(pak);

            START_BIT_COUNT(pak, "entReceiveLevelingpactInfo");
            entReceiveLevelingpactInfo(pak, e, stream_oo_packet);
            STOP_BIT_COUNT(pak);

            // Put data that is only valid for players here (i.e. afk, logout_update)

            START_BIT_COUNT(pak, "entReceiveLogoutUpdate");
                if (entReceiveLogoutUpdate(pak, e, IS_OOP(e->pkt_id_logout)))
                    UPDATE_PACKET_ID(e->pkt_id_logout);
            STOP_BIT_COUNT(pak);
        }
        STOP_BIT_COUNT(pak);
    }

    if (pktGetBits(pak, 1))        //    teammate packet
    {
        START_BIT_COUNT(pak, "entReceiveBuffs");
        if (entReceiveBuffs(pak, e, IS_OOP(e->pkt_id_buffs)))
            UPDATE_PACKET_ID(e->pkt_id_buffs);
        STOP_BIT_COUNT(pak);
    }

    // If there are any outstanding floaters on this entity, attach them.

    DistributeUnclaimedFloaters(e);

    if(very_rare_bits){
        UPDATE_PACKET_ID(e->pkt_id_rare_bits);
    }

#ifndef TEST_CLIENT
    //Debug
    if( e != &dummy_ent && ent_create && !e->seq)
        assertmsg( 0, "Entity received and sequencer has been erased!");
    //end debug
#endif

    STOP_BIT_COUNT(pak); // Matches create/odd_send/always.

    STOP_BIT_COUNT(pak); // Matches enttype

    if (game_state.netfloaters) {
        char pch[128];
        bitpos = bsGetCursorBitPosition(&pak->stream) - bitpos;
        sprintf(pch, "%d.%d", bitpos >> 4, bitpos & 0x7);
        addFloatingInfo(e->svr_idx, pch, 0xbfbfff00, 0x7f7fff00, 0x7f7fff00, 1.8f, MAX_FLOAT_DMG_TIME, 0.0f, kFloaterStyle_Damage, 0);
    }

    if( e != &dummy_ent && entIsMyPet(e) )
        pet_Add( e->svr_idx );

    //This is a bit of a hack because strangely you sometimes get a run out of door animation move, but you don't actually get the
    //position change until the next tick. Without the instantly flag set on the move's receiption, I doesn't know not to be delayed, and it winds up
    //waiting -- thus creating a bug where the guy stands in front of the door for a tick before running out of it.  This fix is a hack because it removes
    //the delay on *all* moves waiting to play when the moved_instantly flag is set, whether it's a run out of door anim or not.  This should
    //not hurt anything, but the real fix would be to make sure the server always sent down the position change and the run out of door move in the same packet.
    //PS Thia totally doesn't work -- I think because sometimes the move comes *after* the moved instantly.  I just put in a DoNotDelay flag on some moves,
    //but I leave this tortuous comment in as a reminder to figure out on the server why the pos change and the seq move aren't always in the same update.
    if( e->seq && e->seq->moved_instantly && e->move_change_timer > 0.0 )
    {
        e->move_change_timer = 0;
    }

    return 1;
}

static void receiveSkyFade(Packet *pak)
{
    F32 skyFadeWeight;
    int skyFade1, skyFade2;
    bool interesting = pktGetBits(pak, 1);
    if (interesting) {
        skyFadeWeight = pktGetF32(pak);
        skyFade1 = pktGetBitsPack(pak, 2);
        skyFade2 = pktGetBitsPack(pak, 2);
    } else {
        skyFadeWeight = 0.0;
        skyFade1 = 0;
        skyFade2 = 1;
    }
    sunSetSkyFadeClient(skyFade1, skyFade2, skyFadeWeight);

    if( demoIsRecording() )
    {
        demoRecordSkyFile(skyFade1, skyFade2, skyFadeWeight);
    }

}

static void calcAutoDelay(U32 old_abs_time,U32 new_abs_time)
{
    static      int     too_much_delay,too_little_delay;
    F32         dt;

    dt = (new_abs_time - old_abs_time) / (30.f * 100.f);
    if (dt > auto_delay)
        too_little_delay++;
    if (dt + 0.1 < auto_delay)
        too_much_delay++;

    if (too_much_delay - too_little_delay > 50)
    {
        auto_delay -= 0.1;
        too_much_delay = too_little_delay = 0;
    }

    if (too_little_delay - too_much_delay  > 50)
    {
        auto_delay += 0.1;
        too_much_delay = too_little_delay = 0;
    }
}

static CmdList cmd_list =
{
    {{ control_cmds },
    { server_visible_cmds },
    { 0 }}
};

void entReceiveInit()
{
    extern U32 last_forced_move_pakid;
    clearQueuedDeletes();
    cmdOldResetPacketIds(&cmd_list);
    last_forced_move_pakid = 0;
}

//
//
void sendCharacterToServer(    Packet *pak, Entity    *e )
{
    // Somehow, people seem to be getting here with a bad entity.
    // This assert will stop that, and hopefully get us a crash dump so
    // we can see what is going on.
    assertmsg(e && e->name && e->name[0]!='\0', "Invalid entity being sent to server.");

    pktSendBitsPack( pak, 1, CLIENTINP_SENDING_CHARACTER );

    pktSendBits(pak, 1, e->pl->playerType);

    character_SendCreate(pak, e->pchar);

    sendTray(pak, e);

    costume_send(pak, costume_as_mutable_cast(e->costume), 1);
    powerCustList_send(pak, e->powerCust);

    pktSendBitsPack( pak, 5, e->pl->titleThe );
    pktSendIfSetString(pak, e->pl->titleTheText);
    pktSendBits(pak, 2, e->name_gender);
    pktSendBits(pak, 1, e->pl->attachArticle);
    sendEntStrings( pak, e->strings );

    pak->reliable = TRUE;
}


static F32 test_timer_elapsed = 0;
U32 last_pkt_id;
//
//

// entSendUpdate + svrTickDoNetSend (mixed in)
void entReceiveUpdate(Packet *pak,int full_update)
{
    int            i,idx,idx_delta,odd_send,oo_packet=0;
    U32            abs_time;
    U32            db_time;
    F32            ticklen,dt;
    Entity*        player;
    CmdContext  context = {0};

    extern int      glob_client_pos_id;

    demo_recording = demoIsRecording();

    if (full_update)
    {
        entReset();
        playerSetEnt(0);
        control_state.client_pos_id = 0;
        glob_client_pos_id = 0;
    }

    odd_send = pktGetBits(pak,1);

    START_BIT_COUNT(pak, "entReceiveUpdate");
        START_BIT_COUNT(pak, "top");
            START_BIT_COUNT(pak, "cmdReceive");
                context.access_level = ACCESS_INTERNAL;
                cmdOldReceive(pak,&cmd_list,&context);
            STOP_BIT_COUNT(pak);

            START_BIT_COUNT(pak, "abs_time");
                last_abs_time = server_visible_state.abs_time;
                abs_time = server_visible_state.abs_time = pktGetBits(pak,32);
            STOP_BIT_COUNT(pak);

            START_BIT_COUNT(pak, "db_time");
                db_time = pktGetBits(pak,32);
                setServerS2000(db_time);
                setAssertShardTime(db_time);
            STOP_BIT_COUNT(pak);

            demoSetEntIdx(0);
            demoSetAbsTime(abs_time);
            //demoRecord("PACKET");

            dt = (abs_time - local_time_bias) / (30.f * 100.f) - (test_timer_elapsed + (TIMESTEP / 30.0));

            if (ABS(dt) > 0.1)
            {
                local_time_bias += dt * (30.f * 100.f);
            }

            ticklen = (abs_time - global_state.abs_time) / 100.f;

            calcAutoDelay(global_state.abs_time,abs_time);
            last_packet_abs_time = global_state.abs_time;
            global_state.abs_time = abs_time;
            server_time = (abs_time - local_time_bias) / (30.f * 100.f);

            for(i=ARRAY_SIZE(global_state.time_since)-1;i>0;i--)
            {
                global_state.time_since[i] = global_state.time_since[i-1] + ticklen;
            }

            global_state.time_since[0] = ticklen;

            if(pktGetBits(pak, 1))
            {
                // Normal data, no special stuff.

                game_state.hasEntDebugInfo = 0;
                interp_data_level = 2;
                interp_data_precision = 1;
            }
            else
            {
                // Receive bit indicating presence of entdebuginfo.

                game_state.hasEntDebugInfo = pktGetBits(pak, 1);

                // Receive bit indicating presence of interp data settings.

                if(pktGetBits(pak, 1))
                {
                    interp_data_level = pktGetBits(pak, 2);
                    interp_data_precision = pktGetBits(pak, 2);
                }
                else
                {
                    interp_data_level = 0;
                }
            }

            //printf("receiving packet: %d : %d bytes\n", pak->id, pak->stream.size);

            // Check if the packet is out of order, or, as I like to call it, OOO.

            //printf("receiving packet: %d : %d bytes\n", pak->id, pak->stream.size);

            if (pak->id < last_pkt_id)
            {
                oo_packet = 1;
            }
            else
            {
                last_pkt_id = pak->id;
            }
        STOP_BIT_COUNT(pak);

        // Receive entities.
        START_BIT_COUNT(pak, "entReceive");
            idx = -1;
            for(;;)
            {
                idx_delta = pktGetBitsPack(pak,1) + 1;
                idx += idx_delta;

                if (!entReceive(pak,idx,odd_send))
                    break;
            }
        STOP_BIT_COUNT(pak);

        // Receive the global debugging crap.

        START_BIT_COUNT(pak, "bottom");
            #ifndef TEST_CLIENT
                if(game_state.hasEntDebugInfo)
                {
                    int svr_idx;

                    while(svr_idx = pktGetBitsPack(pak, 10)){
                        Entity* e = entFromId(svr_idx);
                        if (!e) e = &dummy_ent;

                        START_BIT_COUNT(pak, "receiveEntDebugInfo");
                            // Read the debug info.
                            receiveEntDebugInfo(e, pak);
                        STOP_BIT_COUNT(pak);
                    }
                }

                START_BIT_COUNT(pak, "receiveGlobalEntDebugInfo");
                    receiveGlobalEntDebugInfo(pak);
                STOP_BIT_COUNT(pak);
            #endif

            player = playerPtr();

            assert(player);

            // Receive control state.

            START_BIT_COUNT(pak, "playerReceiveControlState");
                playerReceiveControlState(pak);
            STOP_BIT_COUNT(pak);

            // Receive deletes here, of course.

            START_BIT_COUNT(pak, "entReceiveDeletes");
                entReceiveDeletes(pak);
            STOP_BIT_COUNT(pak);

            // Get the character definition during full update

            if(full_update)
            {
                START_BIT_COUNT(pak, "receiveCharacterFromServer");
                    receiveCharacterFromServer(pak, player);
                STOP_BIT_COUNT(pak);
            } else {
                START_BIT_COUNT(pak, "receiveFullStats");
                    sdUnpackDiff(SendCharacterToSelf, pak, player->pchar, &player->pkt_id_fullAttrDef, true);
                STOP_BIT_COUNT(pak);

                PERFINFO_AUTO_START("character_LevelFinalize", 1);
                    character_LevelFinalize(player->pchar, true); // Must be after getting XP
                PERFINFO_AUTO_STOP();
            }

            // Read Power activation updates

            START_BIT_COUNT(pak, "receivePowerInfoUpdate");
                entity_ReceivePowerInfoUpdate(player, pak);
            STOP_BIT_COUNT(pak);

            START_BIT_COUNT(pak, "ReceivePowerModeUpdate");
                entity_ReceivePowerModeUpdate(player, pak);
            STOP_BIT_COUNT(pak);

            // Badges, badges, badges, badges, badges, badges, mushroom, mushroom!

            START_BIT_COUNT(pak, "ReceiveBadgeUpdate");
                entity_ReceiveBadgeUpdate(player, pak);
            STOP_BIT_COUNT(pak);

            if(full_update)
            {
                START_BIT_COUNT(pak, "ReceiveBadgeMonitorUpdate");
                    badgeMonitor_ReceiveInfo(player, pak);
                STOP_BIT_COUNT(pak);
            }

            START_BIT_COUNT(pak, "ReceiveGenericinventoryUpdate");
                entity_ReceiveInvUpdate(player, pak);
            STOP_BIT_COUNT(pak);


            START_BIT_COUNT(pak, "ReceiveInventionUpdate")
                entity_ReceiveInventionUpdate(player, pak);
            STOP_BIT_COUNT(pak);

            START_BIT_COUNT(pak, "ReceiveWorkshopUpdate")
                strncpy( player->pl->workshopInteracting, pktGetString(pak), 32);
            STOP_BIT_COUNT(pak);

            // read in team list

            START_BIT_COUNT(pak, "receiveTeamList");
            if (!pktGetBits(pak, 1))
                receiveTeamList(pak, playerPtr());
            STOP_BIT_COUNT(pak);

            START_BIT_COUNT(pak, "receiveSuperStats");
                receiveSuperStats(pak, playerPtr());
            STOP_BIT_COUNT(pak);

            START_BIT_COUNT(pak, "receiveLeague");
            if (!pktGetBits(pak, 1))
                receiveLeagueList(pak, playerPtr());
            STOP_BIT_COUNT(pak);

            START_BIT_COUNT(pak, "receiveAttribDescription");
            if (pktGetBits(pak, 1))
                receiveAttribDescription(playerPtr(), pak, 1, 0 );
            STOP_BIT_COUNT(pak);

            START_BIT_COUNT(pak, "groupDynReceive");
                groupDynReceive(pak);
            STOP_BIT_COUNT(pak);

            START_BIT_COUNT(pak, "receiveSkyFade");
                receiveSkyFade(pak);
            STOP_BIT_COUNT(pak);

            // Receive the QROT for a slave.

            if(pktGetBits(pak, 1))
            {
                control_state.pyr.cur[0] = pktGetF32(pak);
                control_state.pyr.cur[1] = pktGetF32(pak);
                control_state.pyr.cur[2] = pktGetF32(pak);
            }

            // Get rid of any remaining unclaimed floaters,
            ClearUnclaimedFloaters();
        STOP_BIT_COUNT(pak);
    STOP_BIT_COUNT(pak);
}

void entCalcInterp(Entity *e, Mat4 mat, U32 client_abs, Quat out_qrot)
{
    MotionHist*    old;
    MotionHist*    next;
    MotionHist*    mh = e->motion_hist;
    int            latest = e->motion_hist_latest;
    int         i,j;
    Vec3        pos;
    Quat        qrot;
    int            size = ARRAY_SIZE(e->motion_hist);

    for(i=0;i<size;i++)
    {
        old = mh + (latest - i + size) % size;
        if (client_abs > old->abs_time)
            break;
    }

    if (i > 0 && i < ARRAY_SIZE(e->motion_hist) && old->abs_time)
    {
        F32     dt,time_step,ratio;
        Vec3 oldPos;


#ifdef RAGDOLL
        U32 uiSecondValidRagdollTimestamp;
        U32 uiFirstValidRagdollTimestamp;
#endif

        // F32    tf
        next = mh + (latest - i + 1 + size) % size;
        time_step = next->abs_time - old->abs_time;
        dt = client_abs - old->abs_time;
        ratio = dt / time_step;



#ifdef RAGDOLL

        if ( e->ragdoll )
        {
            uiFirstValidRagdollTimestamp = firstValidRagdollTimestamp(e->ragdoll);
            uiSecondValidRagdollTimestamp = secondValidRagdollTimestamp(e->ragdoll);
        }


        if ( e->ragdoll && old->abs_time < uiSecondValidRagdollTimestamp && next->abs_time >= uiSecondValidRagdollTimestamp )
        {
            // Usually we'll hit here the first transition




            // We are interpolating between the last non-ragdoll frame and the first ragdoll frame
            // Use offset for the target for this one frame only
            addVec3( old->pos, e->ragdoll_offset_pos, oldPos );
            //copyVec3( old->pos, oldPos );
            /*
            copyQuat( e->ragdoll_offset_qrot, qInverse);
            quatMultiply(qInverse, old->qrot, oldQrot);
            copyQuat( unitQuat, oldQrot);
            */
            copyQuat(unitquat, qrot);
            for(j=0;j<3;j++)
            {
                pos[j] = ((1.f - ratio) * oldPos[j] + ratio * next->pos[j]);

                //tf = subAngle(next->pyr[j],old->pyr[j]);
                //pyr[j] = addAngle(old->pyr[j],tf*ratio);
            }
        }
        else if ( e->ragdoll && next->abs_time > uiFirstValidRagdollTimestamp && next->abs_time < uiSecondValidRagdollTimestamp )
        {

            // Rarely, we'll hit here the first transition

            Vec3 newPos;
            addVec3( old->pos, e->ragdoll_offset_pos, oldPos );
            addVec3( next->pos, e->ragdoll_offset_pos, newPos );
            //copyVec3( old->pos, oldPos );
            /*
            copyQuat( e->ragdoll_offset_qrot, qInverse);
            quatMultiply(qInverse, old->qrot, oldQrot);
            copyQuat( unitQuat, oldQrot);
            */
            copyQuat(unitquat, qrot);
            for(j=0;j<3;j++)
            {
                pos[j] = ((1.f - ratio) * oldPos[j] + ratio * newPos[j]);

                //tf = subAngle(next->pyr[j],old->pyr[j]);
                //pyr[j] = addAngle(old->pyr[j],tf*ratio);
            }
        }
        else if ( e->ragdollFramesLeft > 0 )
        {
            MotionHist*    latestHist = mh + latest;
            copyQuat(latestHist->qrot, qrot);
            copyVec3( latestHist->pos, pos );
        }
        else
#endif
        {
            copyVec3( old->pos, oldPos );
            // Interp quat
            quatInterp(ratio, old->qrot, next->qrot, qrot);
            for(j=0;j<3;j++)
            {
                pos[j] = ((1.f - ratio) * oldPos[j] + ratio * next->pos[j]);

                //tf = subAngle(next->pyr[j],old->pyr[j]);
                //pyr[j] = addAngle(old->pyr[j],tf*ratio);
            }
        }

    }
    else
    {
        next = mh + latest;
        old = mh + (latest - 1 + size) % size;

        if(ENTTYPE(e) != ENTTYPE_CAR && e->fadedirection == ENT_FADING_OUT)
        {
            Vec3 diff;
            U32 time_diff;
            float scale;

            time_diff = next->abs_time - old->abs_time;
            scale = (float)(client_abs - old->abs_time) / (time_diff ? time_diff : 1);

            subVec3(next->pos, old->pos, diff);
            scaleVec3(diff, scale, diff);
            addVec3(old->pos, diff, pos);
        }
        else
        {
            copyVec3(next->pos, old->pos);
            copyQuat(next->qrot, old->qrot);
            copyVec3(next->pos, pos);
            next->abs_time = client_abs;

            //printf("hit end of buffer!!!\n");
        }

        copyQuat(next->qrot, qrot);
    }

    // MS: Remove
    //if(0 && e == current_target)
    //{
    //    printf("\n%d ----------------------------------\n", global_state.global_frame_count);

    //    for(i = 0; i < ARRAY_SIZE(e->motion_hist) - 1; i++)
    //    {
    //        float secs = (e->motion_hist[i].abs_time - e->motion_hist[i+1].abs_time) / 3000.0f;

    //        printf("%1.3f, ", distance3(e->motion_hist[i].pos,e->motion_hist[i+1].pos) / secs);
    //    }

    //    printf("\n");

    //    current_target = NULL;
    //}

    //if (!nearSameVec3(pos,e->mat[3]))
    //{
        copyVec3(pos,mat[3]);
    //}

    quatToMat(qrot, mat);
    //createMat3YPR(mat,pyr);

    if(out_qrot)
    {
        copyQuat(qrot, out_qrot);
    }
}

int entNetUpdate()
{
    int     i,count=0;
    Entity  *e;
    Entity  *controlledPlayer = controlledPlayerPtr();
    U32        new_client_abs;
    U32        new_client_abs_slow;
    int        diff_client_abs;
    int        diff_client_abs_slow;
    F32        add_time;


    static U32 last_client_abs;

    //add_time = timerElapsedAndStart(test_timer) * server_visible_state.timestepscale;

    // MS: I'm gonna use TIMESTEP instead, because the movement is way smoother.  Bruce, come kill me whenever.
    //     TIMESTEP is already scaled by timestepscale, so no need to do it again here.

    add_time = (TIMESTEP / 30.0);

    test_timer_elapsed += add_time;

    new_client_abs = ((U32)((test_timer_elapsed - auto_delay) * 30.f * 100.f)) + local_time_bias;
    new_client_abs_slow = ((U32)((test_timer_elapsed - (auto_delay * 2)) * 30.f * 100.f)) + local_time_bias;

    diff_client_abs = new_client_abs - global_state.client_abs;
    diff_client_abs_slow = new_client_abs_slow - global_state.client_abs_slow;

    if(diff_client_abs > 0 || diff_client_abs < -10000)
    {
        global_state.client_abs = new_client_abs;
        if(global_state.client_abs > global_state.abs_time)
            global_state.client_abs = global_state.abs_time;
    }
    if(diff_client_abs_slow > 0 || diff_client_abs_slow < -10000)
    {
        global_state.client_abs_slow = new_client_abs_slow;
        if(global_state.client_abs_slow > global_state.abs_time)
            global_state.client_abs_slow = global_state.abs_time;
    }

    //printf("diff: %d\n", (((new_client_abs / 3) % 1000) - (timeGetTime() % 1000) + 1000) % 1000);

    //if(diff_client_abs < -10000)
    //    printf("time going backwards: %d\n", diff_client_abs);

    //printf(    "%1.3f elapsed\tdelay %1.3f\tbias %u\ttime %u\tdiff %d%s\n",
    //        time_elapsed,
    //        auto_delay,
    //        local_time_bias,
    //        global_state.client_abs,
    //        global_state.client_abs - last_client_abs,
    //        global_state.client_abs - last_client_abs ? "" : " *****************");

    last_client_abs = global_state.client_abs;

    for(i=1;i<entities_max;i++)
    {
        Mat4 newmat;

        if (!(entity_state[i] & ENTITY_IN_USE ))
        {
            continue;
        }

        count++;
        e = entities[i];
        if (e->demo_id)
            continue;
        
        if (e->showInMenu)
            continue;

        if (e == controlledPlayer && control_state.predict)
        {
            continue;
        }
        if (e==playerPtrForShell(0) || e->showInMenu)
            continue;

        if (ENTTYPE_BY_ID(i) == ENTTYPE_PLAYER && e->logout_timer > 0)
        {
            e->logout_timer -= TIMESTEP;
            if (e->logout_timer < 0)
                e->logout_timer = 0;
        }

        copyMat4(ENTMAT(e), newmat);

        entCalcInterp(e, newmat, ENTINFO(e).send_on_odd_send ? global_state.client_abs_slow : global_state.client_abs, e->cur_interp_qrot);

        entUpdateMat4Interpolated(e, newmat);

        copyVec3(ENTPOS(e),e->motion->last_pos);
    }

    return count;
}

//
// pulled from uiPopHelp.c, so testclient won't complain.
//

static void receiveFullPopHelp( Packet * pak, EntPlayer *pl)
{
    int i;

    memset(&pl->popHelpStatus[0], 0, sizeof(pl->popHelpStatus));

    for(i = 0; i < POP_HELP_STATUS_SIZE; i++)
        pl->popHelpStatus[i] = pktGetBits(pak, 32);

    for(i = 0; i < POP_HELP_LATEST_SIZE; i++)
        pl->popHelpLatest[i] = pktGetBits(pak, 32);
}


// The following functions were moved from entClient.c

//
//
//
void receiveCharacterFromServer( Packet *pak, Entity *e )
{
    int i, count, old_skin;

    START_BIT_COUNT(pak, "server_state");
        game_state.enableBoostDiminishing = pktGetBitsPack(pak, 1);
        game_state.enablePowerDiminishing = pktGetBitsPack(pak, 1);
        game_state.no_base_edit = pktGetBitsPack(pak, 1);
        game_state.beta_base = pktGetBitsPack(pak, 2);
        game_state.base_raid = pktGetBitsPack(pak, 1);
    STOP_BIT_COUNT(pak);

    START_BIT_COUNT(pak, "auth user data");
        pktGetBitsArray(pak, 8*sizeof(e->pl->auth_user_data), e->pl->auth_user_data);
    STOP_BIT_COUNT(pak);

    START_BIT_COUNT(pak, "PlayerType");
        old_skin = game_state.skin;
        e->pl->playerType = pktGetBits( pak, 1 ); STATIC_INFUNC_ASSERT(kPlayerType_Count <= (1<<1))
        e->pl->playerSubType = pktGetBits(pak,2); STATIC_INFUNC_ASSERT(kPlayerSubType_Count <= (1<<2))
        e->pl->praetorianProgress = pktGetBits(pak,3); STATIC_INFUNC_ASSERT(kPraetorianProgress_Count <= (1<<3))
        e->pchar->playerTypeByLocation = pktGetBits(pak,1); STATIC_INFUNC_ASSERT(kPlayerType_Count <= (1<<1))

        game_state.skin = UISKIN_FROM_ENT(e);

        if (old_skin != game_state.skin)
            resetVillainHeroDependantStuff(1);

        calcSeqTypeName( e->seq, e->pl ? e->pl->playerType : 0, e->pl ? ENT_IS_IN_PRAETORIA(e) : 0, ENTTYPE(e)==ENTTYPE_PLAYER ? 1 : 0 );
    STOP_BIT_COUNT(pak);

    START_BIT_COUNT(pak, "character_Receive");
        character_Receive(pak, e->pchar, e);
    STOP_BIT_COUNT(pak);

    START_BIT_COUNT(pak, "OriginalPowerSets");
        strncpy( e->pl->primarySet, pktGetString(pak), MAX_NAME_LEN );
        strncpy( e->pl->secondarySet, pktGetString(pak), MAX_NAME_LEN );
    STOP_BIT_COUNT(pak);

    START_BIT_COUNT(pak, "AllyID");
        entReceiveAllyID(pak, e, 0);
    STOP_BIT_COUNT(pak);

    START_BIT_COUNT(pak, "PvPFlags");
        entReceivePvP(pak, e, 0);
    STOP_BIT_COUNT(pak);

    START_BIT_COUNT(pak, "SendFullCharacter");
        sdUnpackDiff(SendCharacterToSelf, pak, e->pchar, &e->pkt_id_fullAttrDef, true);
    STOP_BIT_COUNT(pak);

    START_BIT_COUNT(pak, "receiveTray");
        receiveTray( pak, e );
    STOP_BIT_COUNT(pak);

    START_BIT_COUNT(pak, "receiveTrayMode");
        receiveTrayMode( pak );
    STOP_BIT_COUNT(pak);

    tray_init();

    START_BIT_COUNT(pak, "name");
        strncpy( e->name, pktGetString(pak), 32);
    STOP_BIT_COUNT(pak);

    if ( !e->strings )
    {
        e->strings = createEntStrings();
        initEntStrings( e->strings );
    }

    START_BIT_COUNT(pak, "receiveEntStrings");
        receiveEntStrings( pak, e->strings );
    STOP_BIT_COUNT(pak);


    START_BIT_COUNT(pak, "receivePopHelp");
        receiveFullPopHelp( pak, e->pl );
    STOP_BIT_COUNT(pak);


    START_BIT_COUNT(pak, "receiveWindows");
    {
        int newchar = 0;
        int w, h;
        windows_initDefaults(0);
        windowClientSizeThisFrame( &w, &h );
        for(i=0; i<MAX_WINDOW_COUNT; i++)
        {
            int win;
            WdwBase wdw = {0};

            receiveWindowIdx(pak, &win);
            receiveWindow(pak, &wdw);

            // Don't bother doing anything if the server sent
            // uninitialized values over.
            // JS:    Why does the server send over uninitialized windows?
            // BZ:  The server sends over uninitialized windows on new character
            // CW:  Default windows are initialized.
            if(wdw.mode == WINDOW_UNINITIALIZED) 
            {
                //newchar = 1;
                continue;
            }

            #ifndef TEST_CLIENT
                if(win>=0 && win<MAX_WINDOW_COUNT)
                {
                    WdwBase *pwin;
                    Wdw *window = wdwGetWindow(win);
                    pwin = &window->loc;
                    assert(pwin);

                    if( wdw.draggable_frame )
                    {
                        int start_shrunk = pwin->start_shrunk;
                        *pwin = wdw;
                        pwin->start_shrunk = start_shrunk;
                    }
                    else
                    {
                        pwin->xp = wdw.xp;
                         pwin->yp = wdw.yp;
                        pwin->mode = wdw.mode;
                        pwin->locked = wdw.locked;
                        pwin->color = wdw.color;
                        pwin->sc = wdw.sc;
                        pwin->maximized = wdw.maximized;

                        if( pwin->color == 0 )
                            pwin->color = 0xffffffff;

                        pwin->back_color = wdw.back_color;

                        if( pwin->back_color == 0 )
                            pwin->back_color = 0x00000088;
                    }

                    // now force minimum window size constraints
                    window->loc.wd = MINMAX( window->loc.wd, window->min_wd, window->max_wd );
                    window->loc.ht = MINMAX( window->loc.ht, window->min_ht, window->max_ht );
                    if (window->loc.yp < h / 8)
                    {
                        window->below = 1;
                    }

                    if( !winDefs[win].maximizeable )
                        pwin->maximized = 0;
                }
            #endif
        }
    //    window_closeAlways();  // moved to commServerWaitOK
    }
    STOP_BIT_COUNT(pak);

#if !defined(TEST_CLIENT)
    // The overall window RGBA is set from the options window when it's
    // closed, so we should set it here when the window defs are first
    // loaded as well. This makes the initial colour and opacity be
    // correct.
    if( wdwGetWindow( WDW_STAT_BARS )->loc.color != 0 &&
        wdwGetWindow( WDW_STAT_BARS )->loc.back_color != 0 )
    {
        int clr;
        int clr_back;

        clr = wdwGetWindow( WDW_STAT_BARS )->loc.color;
        clr_back = wdwGetWindow( WDW_STAT_BARS )->loc.back_color;
        gWindowRGBA[0] = clr >> 24 & 0xff;
        gWindowRGBA[1] = clr >> 16 & 0xff;
        gWindowRGBA[2] = clr >> 8 & 0xff;
        gWindowRGBA[3] = clr_back & 0xff;

        // We can't just call window_ChangeColor() here because it'd send
        // the data to the server and we don't want that, plus it does
        // the bit packing/unpacking again which is wasteful.
        for( i = 0; i < MAX_WINDOW_COUNT; i++ )
        {
            wdwGetWindow( i )->loc.color = clr;
            wdwGetWindow( i )->loc.back_color = clr_back;
        }
    }
#endif

    count = pktGetBitsAuto( pak );
    for( i = 0; i < count; i++ )
    {
        char * tokenName = pktGetString(pak);
        int val = pktGetBitsPack(pak, 1);
        int time = pktGetBitsPack(pak, 1);

        int idx = rewardtoken_IdxFromName( &e->pl->rewardTokens, tokenName );
        if( idx < 0 )
            idx = rewardtoken_Award(&e->pl->rewardTokens, tokenName, val );

        if (EAINRANGE(idx, e->pl->rewardTokens))
        {
            e->pl->rewardTokens[idx]->val = val;
            e->pl->rewardTokens[idx]->timer = time;
        }
    }

    count = pktGetBitsAuto( pak );
    for( i = 0; i < count; i++ )
    {
        char * tokenName = pktGetString(pak);
        int val = pktGetBitsPack(pak, 1);
        int time = pktGetBitsPack(pak, 1);

        int idx = rewardtoken_IdxFromName( &e->pl->activePlayerRewardTokens, tokenName );
        if( idx < 0 )
            idx = rewardtoken_Award(&e->pl->activePlayerRewardTokens, tokenName, val );

        if (EAINRANGE(idx, e->pl->activePlayerRewardTokens))
        {
            e->pl->activePlayerRewardTokens[idx]->val = val;
            e->pl->activePlayerRewardTokens[idx]->timer = time;
        }
    }

    START_BIT_COUNT(pak, "lfg");
        e->pl->lfg = pktGetBits( pak, 10);
        e->pl->hidden = pktGetBits( pak, 10);
    STOP_BIT_COUNT(pak);

    START_BIT_COUNT(pak, "supergroup");
        e->pl->supergroup_mode = pktGetBits( pak, 1);
        if( e->pl->supergroup_mode )
            e->costume = costume_as_const(e->pl->superCostume);
        e->pl->hide_supergroup_emblem = pktGetBits( pak, 1);
        costume_Apply(e);
    STOP_BIT_COUNT(pak);

    START_BIT_COUNT(pak, "skills");
        e->pl->skillsUnlocked = pktGetBits( pak, 1);
    STOP_BIT_COUNT(pak);

    START_BIT_COUNT(pak, "receiveTeamBuffMode");
        receiveTeamBuffMode( pak );
    STOP_BIT_COUNT(pak);

    START_BIT_COUNT(pak, "receiveDockMode");
        receiveDockMode( pak );
    STOP_BIT_COUNT(pak);

    START_BIT_COUNT(pak, "receiveDockMode");
        receiveInspirationMode( pak );
    STOP_BIT_COUNT(pak);

    START_BIT_COUNT(pak, "newFeaturesVersion");
        e->pl->newFeaturesVersion = pktGetBits(pak, 32);
    STOP_BIT_COUNT(pak);

    START_BIT_COUNT(pak, "receiveChatSettings");
        receiveChatSettingsFromServer( pak, e );
    STOP_BIT_COUNT(pak);

    START_BIT_COUNT(pak, "receiveTitles");
        receiveTitles( pak );
    STOP_BIT_COUNT(pak);

    START_BIT_COUNT(pak, "receiveDescription");
        receiveDescription( pak );
    STOP_BIT_COUNT(pak);

    START_BIT_COUNT(pak, "receiveComment");
        strncpyt( e->pl->comment, pktGetString(pak), 128);
    STOP_BIT_COUNT(pak);

    if( !e->pl->keybinds )
        e->pl->keybinds = calloc( 1, sizeof(KeyBind)*BIND_MAX );

    START_BIT_COUNT(pak, "keybinds");
        keybinds_recieve( pak, e->pl->keyProfile, e->pl->keybinds );
        keybinds_init( e->pl->keyProfile, &game_binds_profile, e->pl->keybinds );
    STOP_BIT_COUNT(pak);

    if( pktGetBits( pak, 1) )
    {
        START_BIT_COUNT(pak, "receiveOptions");
            receiveOptions( pak );
        STOP_BIT_COUNT(pak);
    }
    else
    {
        START_BIT_COUNT(pak, "mouse settings");
        optionSet(kUO_MouseInvert, pktGetBits(pak,1), 0);
        optionSetf( kUO_MouseSpeed, pktGetF32(pak), 0);
        optionSetf(kUO_SpeedTurn, pktGetF32(pak), 0);
        STOP_BIT_COUNT(pak);
#ifndef TEST_CLIENT
        setNewOptions();
#endif
    }

    receiveDividers(pak);

    control_state.first = pktGetBits( pak, 1 );
    control_state.nocoll = pktGetBits( pak, 2 );

    START_BIT_COUNT(pak, "receiveFriendsList");
        receiveFriendsList( pak, e );
    STOP_BIT_COUNT(pak);
    
    START_BIT_COUNT(pak, "receiveCombatMonitorStats");
        receiveCombatMonitorStats(e, pak);
    STOP_BIT_COUNT(pak);

    if( pktGetBits(pak, 1) ) // this player gets a rename
    {
#ifdef TEST_CLIENT
        e->db_flags |= DBFLAG_RENAMEABLE;
#endif
        dialogChooseNameImmediate(true);
    }

    pets_Clear();

    gameStateLoad();
}


/* End of File */    // JE: do we really need a comment letting us know this?
                    // MS: Is there another way to know you're at the end of the file?
                    // SP: Old habits die hard. During the Jurassic, not having a CR
                    //     on the last line of a file would screw up if they were
                    //     #included. No joke. So, the comment always has a CR after it.
                    //     Or should. Anyway, once every 5 years it's useful when you
                    //     need to look at precompiled output.
